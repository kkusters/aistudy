// C__PC_COM.C                             

#include "ch_define.h"

#include "ch_alarm.h"
#include "ch_alg.h"
#include "ch_Asc0.h"
#include "ch_Asc1.h"
#include "ch_can_backbone_appl.h"
#include "ch_can_backbone_pc.h"
#include "ch_can_io.h"
#include "ch_disp.h"
#include "ch_disp_alarm_0.h"
#include "ch_disp_option_0.h"
#include "ch_disp_option_1.h"
#include "ch_disp_password.h"
#include "ch_eep_taal.h"
#include "ch_ethernet.h"
#include "ch_IO.h"
#include "ch_key.h"
#include "ch_lcd_bitmap.h"
#include "ch_lcd_hardware.h"
#include "ch_main.h"  
#include "ch_mb_device.h"
#include "ch_motor.h"
#include "ch_motorgroep.h"
#include "ch_rs232_0.h"
#include "ch_rs232_1.h"
#include "ch_sd.h"
#include "ch_sd_log.h"
#include "ch_string.h"
#include "ch_tijd.h"
#include "ch_device.h"
#include "ch_xml.h"
#include "ch_pc_com.h"

#include <string.h>

#define BUFFERSIZE 1024
#define MAXDATA     100
#define MAXDATA_XML 500
//#define MAXDATA_CAPTURE 100
#define MAXDATA_CAPTURE 255
#define MAXDATA_CAPTURE_FAST 255
// straks weer terugzetten naar 100
// maximum aantal databytes per bericht (1 byte is 2 characters)
// maxdata moet minimaal 20 groot zijn voor zenden module 
#define MINMAXDATA 8
// aantal min max data gegevens dat per keer wordt verzonden
#define ALARMDATA 8
// aantal alarm data gegevens dat per keer wordt verzonden
#define TIMERDATA 24
// aantal timer data gegevens dat per keer wordt verzonden
#define TIMERPERCDATA 12
// aantal timer perc data gegevens dat per keer wordt verzonden
#define HOOFDLICHTCURVEDATA 12
// aantal data gegevens dat per keer wordt verzonden

#define COM_VOOR_BLACKBOX 0
#define COM_VOOR_ALG 1
#define COM_VOOR_AFD 2

unsigned char pc_0_read_configuration_alg = 1; // 1 if orion startup
                                           // reset to zero if CAN do configuration read
unsigned char pc_1_read_configuration_alg = 1; // 1 if orion startup
                                           // reset to zero if CAN do configuration read
unsigned char pc_2_read_configuration_alg = 1; // 1 if orion startup
                                           // reset to zero if CAN do configuration read
unsigned char rs232_0_read_configuration_alg = 1; // 1 if orion startup
                                       // reset to zero if RS232 do configuration read
unsigned char rs232_1_read_configuration_alg = 1; // 1 if orion startup
                                       // reset to zero if RS232 do configuration read
#ifdef ETHERNET
unsigned char ethernet_read_configuration_alg[4] = {1,1,1,1}; // 1 if orion startup
                                       // reset to zero if RS232 do configuration read
#endif // ETHERNET
static unsigned char commando_voor; // 0 = COM_VOOR_BLACKBOX CONFIG READ VIA RS232
                                    // 1 = COM_VOOR_ALG (Pc communicatie algemeen)
                                    // 2 = COM_VOOR_AFD (PC communicatie afdeling)
                                    // 3 = COM_VOOR_OPT
static unsigned char commando_index; // bij COM_VOOR_AFD index nodig om afdeling te bepalen
                                     // bij COM_VOOR_OPT nodig om CAN adres voo retour bericht te bepalen                                     
                                    
static unsigned int  received_adres;          // binnenkomend adres van ontvanger
static unsigned int  received_command;        // binnenkomend commando van PC
static unsigned int  received_sub_command;    // binnenkomend subcommando van PC
static unsigned char received_blok_cnt;       // blok_cnt voor controle bij bericht langer dan MAXDATA
static unsigned char received_lengte;         // lengte DATA binnenkomend bericht van PC
static unsigned char received_ETB_or_ETX = 0; //
static unsigned int  send_adres; // binnenkomend adres van zender (meestal PC)

static char *command_ptr; // pointer naar actuele karakter in command_buffer
static char *answer_ptr; // pointer naar eerste lege positie in answer buffer

unsigned char restore_data_buffer[MAX_BUFFER_RESTORE_DATA];
unsigned char restore_data_busy = 0;
#ifdef SD_CARD
unsigned char read_data_file_busy = 0;
#endif // SD_CARD

static int alarm_code = 0;
static int alarm_val  = 0;
static unsigned long actual_time        = 0;
static unsigned char read_configuration = 0;

//*****************************************************************************
void PC_Table_Read(s_pc_com *pc_com, unsigned char *Table);
void PC_Table_Send(s_pc_com *pc_com, unsigned char *Table);

//-----------------------------------------------------------------------------
s_PcDataTable const FunctionTable[] =
{
  { NULL, 0 }
};

//-----------------------------------------------------------------------------
s_PcDataTable const MainGroupTable[] =
{
  { &alarm_code,              TypeInt  }, //  0 + 2 =  2
  { &alarm_val,               TypeInt  }, //  2 + 2 =  4
  { &actual_time,             TypeLong }, //  4 + 4 =  8
  { &read_configuration,      TypeChar }, //  8 + 1 =  9
  { &opt_alg.change_cnt,      TypeInt  }, //  9 + 2 = 11
  { &setp_alg.sd_card_status, TypeChar }, // 11 + 1 = 12
  { NULL, 0 }
};

//-----------------------------------------------------------------------------
static void Put_Char(char ch)
{
  *answer_ptr++ = ch;
}

static unsigned char Send_Byte(unsigned char value, unsigned char checksum)
{
  value = Hex_To_Asc(value);
  Put_Char(value);
  return (checksum ^ value);
}

static unsigned char Send_Char(unsigned char value, unsigned char checksum)
{
  checksum = Send_Byte(value >> 4,checksum);
  checksum = Send_Byte(value & 0xFF,checksum);
  return (checksum);
}

static unsigned char Send_Int(unsigned int value, unsigned char checksum)
{
  checksum = Send_Char((unsigned char)(value >> 8), checksum);
  checksum = Send_Char(value & 0x00FF, checksum);
  return (checksum);
}

static unsigned char Send_Long(unsigned long value, unsigned char checksum)
{
  checksum = Send_Int((unsigned int)(value >> 16), checksum);
  checksum = Send_Int(value & 0x0000FFFFL, checksum);
  return (checksum);
}

static unsigned char Send_Adres(unsigned int value, unsigned char checksum)
{
  checksum = Send_Byte(value >> 8,checksum);
  checksum = Send_Char(value & 0x00FF, checksum);
  return (checksum);
}

static unsigned char Send_Command(unsigned int value, unsigned char checksum)
{
  checksum = Send_Byte(value >> 8,checksum);
  checksum = Send_Char(value & 0x00FF, checksum);
  return (checksum);
}

//*****************************************************************************
static unsigned char *Get_Char(unsigned char *buffer, unsigned char *val)
{
  *val = *buffer;
  buffer++;
  return (buffer);
}

static unsigned char *Get_Int(unsigned char *buffer, unsigned int *val)
{
unsigned char c1,c2;

  buffer = Get_Char(buffer, &c2);
  buffer = Get_Char(buffer, &c1);
  *val = c2 * 256 + c1;
  return (buffer);
}

static unsigned char *Get_Long(unsigned char *buffer, unsigned long *val)
{
unsigned int i1,i2;

  buffer = Get_Int(buffer, &i2);
  buffer = Get_Int(buffer, &i1);
  *val = i2 * 256l * 256 + i1;
  return (buffer);
}

//*****************************************************************************
static unsigned char Receive_Byte(void)
{
  return (Asc_To_Hex(*command_ptr++));
}

static unsigned char Receive_Char(void)
{
unsigned char value;

  value = Receive_Byte();
  value <<= 4;
  value |= Receive_Byte();
  return (value);
}

static unsigned int Receive_Int(void)
{
unsigned int value;

  value = Receive_Char();
  value <<= 8;
  value |= Receive_Char();
  return (value);
}

static unsigned long Receive_Long(void)
{
unsigned long value;

  value = Receive_Int();
  value <<= 16;
  value |= Receive_Int();
  return (value);
}

static unsigned int Receive_Adres(void)
{
unsigned int value;

  value = Receive_Byte();
  value <<= 8;
  value |= Receive_Char();
  return (value);
}

static unsigned int Receive_Command(void)
{
unsigned int value;

  value = Receive_Byte();
  value <<= 8;
  value |= Receive_Char();
  return (value);
}

//*****************************************************************************
static unsigned char ReceiveAndCheckChar(char *val)
{
unsigned char ret = 0;
char value;

  value = Receive_Byte();
  value <<= 4;
  value |= Receive_Byte();
  if (*val != value)
    ret = 1;
  *val = value;
  return (ret);
}

static unsigned char ReceiveAndCheckInt(int *val)
{
unsigned char ret = 0;
int value;

  value = Receive_Char();
  value <<= 8;
  value |= Receive_Char();
  if (*val != value)
    ret = 1;
  *val = value;
  return (ret);
}

static unsigned char ReceiveAndCheckLong(long *val)
{
unsigned char ret = 0;
long value;

  value = Receive_Int();
  value <<= 16;
  value |= Receive_Int();
  if (*val != value)
    ret = 1;
  *val = value;
  return (ret);
}

//*****************************************************************************

// wordt verzonden bij een bericht dat fout binnenkomt
void PC_Send_Error_Code(unsigned int received_adres, unsigned int send_adres, unsigned int command, unsigned char error)
{
unsigned char checksum;

  Put_Char(STX);
  checksum = STX;
  checksum = Send_Adres(send_adres,checksum); // bericht voor adres
  checksum = Send_Adres(received_adres,checksum); // bericht van adres
  checksum = Send_Command(command,checksum); // command
  checksum = Send_Byte(SUBCOMMANDBEGIN, checksum); // sub command
  checksum = Send_Char(0, checksum); // block_nr
  checksum = Send_Char(0x01,checksum); // aantal data bytes
  checksum = Send_Char(error, checksum); // error

  Send_Char(checksum,0);
  Put_Char(ETX_CH);
  Put_Char(ETX);
  Put_Char(0);
}

//*****************************************************************************

static void PC_Read_Data(s_pc_com *pc_com, unsigned char *end)
{
unsigned char *ptr = pc_com->ptr;
unsigned char endchar = (ptr + MAXDATA < end) ? ETB_CH : ETX_CH;
unsigned char checksum;

  if (endchar == ETB_CH)
    end = ptr + MAXDATA;
  Put_Char(STX);
  checksum = STX;
  checksum = Send_Adres(send_adres/*PC_ADR*/, checksum); // adres
  checksum = Send_Adres(received_adres, checksum); // adres
  checksum = Send_Command(received_command, checksum); // command
  checksum = Send_Byte(received_sub_command, checksum); // sub command
  checksum = Send_Char(pc_com->blok_cnt, checksum); // block_nr
  checksum = Send_Char(end - ptr, checksum);
  while (ptr < end)
  {
    checksum = Send_Char(*ptr, checksum);
    ptr++;
  }
  Send_Char(checksum,0);
  Put_Char(endchar);
  Put_Char(ETX);
  Put_Char(0);
//  if (endchar == ETX_CH)
//    pc_com->command = NOCOMMAND;
}

//-----------------------------------------------------------------------------

static void PC_Write_Data(s_pc_com *pc_com)
{
unsigned char checksum;
unsigned char cnt = received_lengte;

  Put_Char(STX);
  checksum = STX;
  checksum = Send_Adres(send_adres/*PC_ADR*/, checksum); // adres
  checksum = Send_Adres(received_adres, checksum); // adres
  checksum = Send_Command(received_command, checksum); // command
  if (received_ETB_or_ETX == ETX_CH)
  {
    checksum = Send_Byte(SUBCOMMANDEND, checksum); // sub command
    checksum = Send_Char(0, checksum); // block_nr
  }
  else
  {
    checksum = Send_Byte(SUBCOMMANDNEXT, checksum); // sub command
    checksum = Send_Char(pc_com->blok_cnt, checksum); // block_nr
  }
  checksum = Send_Char(0, checksum);
  while (cnt)
  {
    *pc_com->ptr = Receive_Char();
    pc_com->ptr++;
    cnt--;
  }
  Receive_Char(); // verwijder checksum

  Send_Char(checksum,0);
  Put_Char(ETX_CH);
  Put_Char(ETX);
  Put_Char(0);
//  if (received_ETB_or_ETX == ETX_CH)
//    pc_com->command = NOCOMMAND;
}

//*****************************************************************************
//** BLACKBOX COMMANDOS
//*****************************************************************************

//-----------------------------------------------------------------------------
// geeft aan de PC de aangesloten computers terug.
static void PC_Configuration_Read_Blackbox(s_pc_com *pc_com)
{
unsigned char checksum;

  pc_com->command = CONFIGURATIONREAD;
  pc_com->command_timeout = COMMANDPCTIMEOUT;
  switch (received_sub_command)
  {
    case SUBCOMMANDBEGIN:
    case SUBCOMMANDPREV:
    case SUBCOMMANDNEXT:
      Put_Char(STX);
      checksum = STX;
      checksum = Send_Adres(send_adres/*PC_ADR*/, checksum); // adres
      checksum = Send_Adres(received_adres/*BLACKBOX_ADR*/, checksum); // adres 
      checksum = Send_Command(CONFIGURATIONREAD, checksum); // command
      checksum = Send_Byte(received_sub_command, checksum); // sub command
      checksum = Send_Char(0, checksum); // block_nr
      checksum = Send_Char(22, checksum); // aantal bytes data
          
      checksum = Send_Int(opt_alg.password_pc, checksum); // password_pc
      checksum = Send_Int(default_opt_alg.computer, checksum);     // type computer
      checksum = Send_Int(default_opt_alg.soort, checksum);
      checksum = Send_Int(module.type, checksum);
      checksum = Send_Int(module.firma, checksum);
      checksum = Send_Int(default_opt_alg.versie_programma, checksum);     // versie nummer computer
      checksum = Send_Int(default_opt_alg.versie_PC, checksum);     // versie nummer computer
      checksum = Send_Long(module.serie_number, checksum);
      checksum = Send_Int(opt_alg.adres, checksum);      // adres(nr) computer(orion)
      checksum = Send_Int(opt_alg.change_cnt, checksum); // change_cnt veranderd als er een optie gewijzigd wordt

      Send_Char(checksum,0);
      Put_Char(ETX_CH);
      Put_Char(ETX);
      Put_Char(0);
      if (pc_com == &can_backbone_pc_0_com)
        pc_0_read_configuration_alg = 0;
      else if (pc_com == &can_backbone_pc_1_com)
        pc_1_read_configuration_alg = 0;
      else if (pc_com == &can_backbone_pc_2_com)
        pc_2_read_configuration_alg = 0;
      else if (pc_com == &rs232_0_pc_com)
        rs232_0_read_configuration_alg = 0;  
      else if (pc_com == &rs232_1_pc_com)
        rs232_1_read_configuration_alg = 0;  
      #ifdef ETHERNET  
      else if (pc_com == &ethernet_pc_com[0])
        ethernet_read_configuration_alg[0] = 0;  
      else if (pc_com == &ethernet_pc_com[1])
        ethernet_read_configuration_alg[1] = 0;  
      else if (pc_com == &ethernet_pc_com[2])
        ethernet_read_configuration_alg[2] = 0;  
      else if (pc_com == &ethernet_pc_com[3])
        ethernet_read_configuration_alg[3] = 0;  
      #endif // ETHERNET  
      break;
   case SUBCOMMANDEND:
      PC_Send_Error_Code(received_adres, send_adres, NOCOMMAND, ERRORSUBCOMMANDEND);
      break;
  }
}

//-----------------------------------------------------------------------------
static void PC_Time_Write_Blackbox(s_pc_com *pc_com)
{
unsigned char checksum;
time_t t;

  pc_com->command = TIMEWRITE;
  pc_com->command_timeout = COMMANDPCTIMEOUT;
  switch (received_sub_command)
  {
    case SUBCOMMANDBEGIN:
    case SUBCOMMANDPREV:
    case SUBCOMMANDNEXT:
      t = Receive_Long();
      Tijd_PC_Set(t);
      Put_Char(STX);
      checksum = STX;
      checksum = Send_Adres(send_adres/*PC_ADR*/, checksum); //  adres
      checksum = Send_Adres(received_adres/*BLACKBOX_ADR*/, checksum); // adres
      checksum = Send_Command(TIMEWRITE, checksum); //  command
      checksum = Send_Byte(SUBCOMMANDEND, checksum); // sub command
      checksum = Send_Char(0, checksum); // block_nr
      checksum = Send_Char(0, checksum); // aantal data bytes
      Send_Char(checksum,0);
      Put_Char(ETX_CH);
      Put_Char(ETX);
      Put_Char(0);
//        pc_com->command = NOCOMMAND;
      break;
    case SUBCOMMANDEND:
      PC_Send_Error_Code(received_adres, send_adres, NOCOMMAND, ERRORSUBCOMMANDEND);
      break;
  }
}

//*****************************************************************************
//** EINDE BLACKBOX COMMANDOS
//*****************************************************************************

//*****************************************************************************
//** ALGEMEEN COMMANDOS
//*****************************************************************************

static void PC_Option_Read_Algemeen(s_pc_com *pc_com)
{
// is gelijk aan commando PC_Save_Option() alleen commando is anders

  pc_com->command = OPTIONS_ALG_READ;
  pc_com->command_timeout = COMMANDPCTIMEOUT;
  switch (received_sub_command)
  {
    case SUBCOMMANDBEGIN:
      pc_com->ptr = (unsigned char *)&opt_alg;
      pc_com->blok_cnt = 0;
      PC_Read_Data(pc_com, (unsigned char *)&opt_alg.end+1);
      break;
    case SUBCOMMANDPREV:
      PC_Read_Data(pc_com, (unsigned char *)&opt_alg.end+1);
      break;
    case SUBCOMMANDNEXT:
      if (received_blok_cnt == (unsigned char)(pc_com->blok_cnt+1))
      {
        if (pc_com->ptr <= (unsigned char *)&opt_alg.end)
        {
          pc_com->ptr += MAXDATA;
          pc_com->blok_cnt++;
        }
      }
      PC_Read_Data(pc_com, (unsigned char *)&opt_alg.end+1);
      break;
    case SUBCOMMANDEND:
      PC_Send_Error_Code(received_adres, send_adres, NOCOMMAND, ERRORSUBCOMMANDEND);
      break;
  }
}

//-----------------------------------------------------------------------------
static void PC_Option_Read_IO(s_pc_com *pc_com)
{
  pc_com->command = OPTIONS_IO_READ;
  pc_com->command_timeout = COMMANDPCTIMEOUT;
  switch (received_sub_command)
  {
    case SUBCOMMANDBEGIN:
      pc_com->ptr = (unsigned char *)&opt_io;
      pc_com->blok_cnt = 0;
      PC_Read_Data(pc_com, (unsigned char *)&opt_io.end+1);
      break;
    case SUBCOMMANDPREV:
      PC_Read_Data(pc_com, (unsigned char *)&opt_io.end+1);
      break;
    case SUBCOMMANDNEXT:
      if (received_blok_cnt == (unsigned char)(pc_com->blok_cnt+1))
      {
        if (pc_com->ptr <= (unsigned char *)&opt_io.end)
        {
          pc_com->ptr += MAXDATA;
          pc_com->blok_cnt++;
        }
      }
      PC_Read_Data(pc_com, (unsigned char *)&opt_io.end+1);
      break;
    case SUBCOMMANDEND:
      PC_Send_Error_Code(received_adres, send_adres, NOCOMMAND, ERRORSUBCOMMANDEND);
      break;
  }
}

static void PC_Option_Write_IO(s_pc_com *pc_com)
{
unsigned int default_area_size;
unsigned int aantal;
unsigned int computer;
unsigned int soort;

  pc_com->command = OPTIONS_IO_WRITE;
  pc_com->command_timeout = COMMANDPCTIMEOUT;
  switch (received_sub_command)
  {
    case SUBCOMMANDBEGIN:
      if (restore_data_busy != 0)
      {
        PC_Send_Error_Code(received_adres, send_adres, OPTIONS_IO_WRITE, ERRORRESTOREOPTIONSETPOINTBUSY);
      }
      else
      {
        restore_data_busy = TIME_OUT_RESTORE_DATA_BUSY;
        pc_com->old_ptr = pc_com->ptr = restore_data_buffer;
        pc_com->blok_cnt = 1;
        PC_Write_Data(pc_com);
      }  
      break;
    case SUBCOMMANDPREV:
      restore_data_busy = TIME_OUT_RESTORE_DATA_BUSY;
      pc_com->ptr = pc_com->old_ptr;
      PC_Write_Data(pc_com);
      break;
    case SUBCOMMANDNEXT:
      restore_data_busy = TIME_OUT_RESTORE_DATA_BUSY;
      if (received_blok_cnt == pc_com->blok_cnt)
      {
        pc_com->blok_cnt++;
        pc_com->old_ptr = pc_com->ptr;
      }
      else
        pc_com->ptr = pc_com->old_ptr;
      PC_Write_Data(pc_com);
      break;
    case SUBCOMMANDEND:
      restore_data_busy = 0;
      PC_Send_Error_Code(received_adres, send_adres, NOCOMMAND, ERRORSUBCOMMANDEND);
      return;
  }
  if (received_ETB_or_ETX == ETX_CH)
  {
    default_area_size = (unsigned long)&default_opt_io.end - (unsigned long)&default_opt_io;
    aantal =   ((unsigned int)restore_data_buffer[1] << 8) + restore_data_buffer[0];
    computer = ((unsigned int)restore_data_buffer[3] << 8) + restore_data_buffer[2];
    soort =    ((unsigned int)restore_data_buffer[5] << 8) + restore_data_buffer[4];
    if ((computer == default_opt_io.computer) && (soort == default_opt_io.soort))
    {
      if (aantal > default_area_size)
        aantal = default_area_size;
      BlockCopy((unsigned char *)restore_data_buffer,((unsigned char *)&opt_io),aantal);
      if (default_area_size > aantal )
      {
        AddNewOptIO();
      }

      VersieAfhankelijkOptIO();
      pc_0_read_configuration_alg = 1;  
      pc_1_read_configuration_alg = 1;
      pc_2_read_configuration_alg = 1;
      rs232_0_read_configuration_alg = 1;
      rs232_1_read_configuration_alg = 1;
      #ifdef ETHERNET
      ethernet_read_configuration_alg[0] = 1;
      ethernet_read_configuration_alg[1] = 1;
      ethernet_read_configuration_alg[2] = 1;
      ethernet_read_configuration_alg[3] = 1;
      #endif // ETHERNET
      IncrementOptionsChangeCount();
      ModuleCheck();
      CAN_IO_Init_All_Boards();
      install_flag = 0;
      CreateAlarm(&alarm_hr_alg.new_option_from_pc_al, SYSTEEM_AL_NEW_OPTION, 0, 0, 0, ZACHT_ALARM);
      ClearAlarm(&alarm_hr_alg.opt_al, SYSTEEM_AL_OPT, 0);
    }
    else
    {
      CreateAlarm(&alarm_hr_alg.restore_option_setpoint_failed_al, SYSTEEM_AL_RESTORE_OPTION_SETPOINT_FAILED, 0, 0, 0, HARD_ALARM);
    }
    restore_data_busy = 0;
  }
}

//-----------------------------------------------------------------------------
static void PC_Option_Read_App(s_pc_com *pc_com)
{
  pc_com->command = OPTIONS_APP_READ;
  pc_com->command_timeout = COMMANDPCTIMEOUT;
  switch (received_sub_command)
  {
    case SUBCOMMANDBEGIN:
      pc_com->ptr = (unsigned char *)&opt_app;
      pc_com->blok_cnt = 0;
      PC_Read_Data(pc_com, (unsigned char *)&opt_app.end+1);
      break;
    case SUBCOMMANDPREV:
      PC_Read_Data(pc_com, (unsigned char *)&opt_app.end+1);
      break;
    case SUBCOMMANDNEXT:
      if (received_blok_cnt == (unsigned char)(pc_com->blok_cnt+1))
      {
        if (pc_com->ptr <= (unsigned char *)&opt_app.end)
        {
          pc_com->ptr += MAXDATA;
          pc_com->blok_cnt++;
        }
      }
      PC_Read_Data(pc_com, (unsigned char *)&opt_app.end+1);
      break;
    case SUBCOMMANDEND:
      PC_Send_Error_Code(received_adres, send_adres, NOCOMMAND, ERRORSUBCOMMANDEND);
      break;
  }
}

static void PC_Option_Write_App(s_pc_com *pc_com)
{
unsigned int default_area_size;
unsigned int aantal;
unsigned int computer;
unsigned int soort;

  pc_com->command = OPTIONS_APP_WRITE;
  pc_com->command_timeout = COMMANDPCTIMEOUT;
  switch (received_sub_command)
  {
    case SUBCOMMANDBEGIN:
      if (restore_data_busy != 0)
      {
        PC_Send_Error_Code(received_adres, send_adres, OPTIONS_APP_WRITE, ERRORRESTOREOPTIONSETPOINTBUSY);
      }
      else
      {
        restore_data_busy = TIME_OUT_RESTORE_DATA_BUSY;
        pc_com->old_ptr = pc_com->ptr = restore_data_buffer;
        pc_com->blok_cnt = 1;
        PC_Write_Data(pc_com);
      }  
      break;
    case SUBCOMMANDPREV:
      restore_data_busy = TIME_OUT_RESTORE_DATA_BUSY;
      pc_com->ptr = pc_com->old_ptr;
      PC_Write_Data(pc_com);
      break;
    case SUBCOMMANDNEXT:
      restore_data_busy = TIME_OUT_RESTORE_DATA_BUSY;
      if (received_blok_cnt == pc_com->blok_cnt)
      {
        pc_com->blok_cnt++;
        pc_com->old_ptr = pc_com->ptr;
      }
      else
        pc_com->ptr = pc_com->old_ptr;
      PC_Write_Data(pc_com);
      break;
    case SUBCOMMANDEND:
      restore_data_busy = 0;
      PC_Send_Error_Code(received_adres, send_adres, NOCOMMAND, ERRORSUBCOMMANDEND);
      return;
  }
  if (received_ETB_or_ETX == ETX_CH)
  {
    default_area_size = (unsigned long)&default_opt_app.end - (unsigned long)&default_opt_app;
    aantal =   ((unsigned int)restore_data_buffer[1] << 8) + restore_data_buffer[0];
    computer = ((unsigned int)restore_data_buffer[3] << 8) + restore_data_buffer[2];
    soort =    ((unsigned int)restore_data_buffer[5] << 8) + restore_data_buffer[4];
    if ((computer == default_opt_app.computer) && (soort == default_opt_app.soort))
    {
      if (aantal > default_area_size)
        aantal = default_area_size;
      BlockCopy((unsigned char *)restore_data_buffer,((unsigned char *)&opt_app),aantal);
      if (default_area_size > aantal )
      {
        AddNewOptApp();
      }

      VersieAfhankelijkOptApp();
      pc_0_read_configuration_alg = 1;  
      pc_1_read_configuration_alg = 1;
      pc_2_read_configuration_alg = 1;
      rs232_0_read_configuration_alg = 1;
      rs232_1_read_configuration_alg = 1;
      #ifdef ETHERNET
      ethernet_read_configuration_alg[0] = 1;
      ethernet_read_configuration_alg[1] = 1;
      ethernet_read_configuration_alg[2] = 1;
      ethernet_read_configuration_alg[3] = 1;
      #endif // ETHERNET
      IncrementOptionsChangeCount();
      ModuleCheck();
      CheckOptions();
      Init_All_Screen();
      install_flag = 0;
      CreateAlarm(&alarm_hr_alg.new_option_from_pc_al, SYSTEEM_AL_NEW_OPTION, 0, 0, 0, ZACHT_ALARM);
      ClearAlarm(&alarm_hr_alg.opt_al, SYSTEEM_AL_OPT, 0);
    }
    else
    {
      CreateAlarm(&alarm_hr_alg.restore_option_setpoint_failed_al, SYSTEEM_AL_RESTORE_OPTION_SETPOINT_FAILED, 0, 0, 0, HARD_ALARM);
    }
    restore_data_busy = 0;
  }
}

//-----------------------------------------------------------------------------
static void PC_Options_IO_App_Read_Data(s_pc_com *pc_com)
{
unsigned char *ptr = pc_com->ptr;
unsigned char *end = 0;
unsigned char endchar = 0;
unsigned char checksum;

  switch (pc_com->index)
  {
    case 0: // opt_io
      endchar = ETB_CH;
      end = (unsigned char *)&opt_io.end;
      if ((ptr + MAXDATA) < end)
        end = ptr + MAXDATA;
      break;
    case 1: // opt_app
      endchar = ETX_CH;
      end = (unsigned char *)&opt_app.end;
      if ((ptr + MAXDATA) < end)
      {
        endchar = ETB_CH;
        end = ptr + MAXDATA;
      }
      break;
  }

  Put_Char(STX);
  checksum = STX;
  checksum = Send_Adres(send_adres/*PC_ADR*/, checksum); // adres
  checksum = Send_Adres(received_adres, checksum); // adres
  checksum = Send_Command(received_command, checksum); // command
  checksum = Send_Byte(received_sub_command, checksum); // sub command
  checksum = Send_Char(pc_com->blok_cnt, checksum); // block_nr
  checksum = Send_Char(end - ptr, checksum);
  while (ptr < end)
  {
    checksum = Send_Char(*ptr, checksum);
    ptr++;
  }
  Send_Char(checksum,0);
  Put_Char(endchar);
  Put_Char(ETX);
  Put_Char(0);
//  if (endchar == ETX_CH)
//    pc_com->command = NOCOMMAND;
}

static void PC_Options_IO_App_Read(s_pc_com *pc_com)
{
unsigned char *end = 0;

  pc_com->command = OPTIONS_IO_APP_READ;
  pc_com->command_timeout = COMMANDPCTIMEOUT;
  switch (received_sub_command)
  {
    case SUBCOMMANDBEGIN:
      opt_io.area_size  = (unsigned long)&default_opt_io.end  - (unsigned long)&default_opt_io;
      opt_app.area_size = (unsigned long)&default_opt_app.end - (unsigned long)&default_opt_app;
      pc_com->ptr = (unsigned char *)&opt_io;
      pc_com->index = 0;
      pc_com->blok_cnt = 0;
      PC_Options_IO_App_Read_Data(pc_com);
      break;
    case SUBCOMMANDPREV:
      PC_Options_IO_App_Read_Data(pc_com);
      break;
    case SUBCOMMANDNEXT:
      if (received_blok_cnt == (unsigned char)(pc_com->blok_cnt+1))
      {
        switch (pc_com->index)
        {
          case 0: // opt_io
            pc_com->blok_cnt++;
            pc_com->ptr += MAXDATA;
            if (pc_com->ptr >= (unsigned char *)&opt_io.end)
            {
              pc_com->ptr = (unsigned char *)&opt_app;
              pc_com->index = 1;
            }
            break;
          case 1: // opt_app
            pc_com->blok_cnt++;
            pc_com->ptr += MAXDATA;
            break;          
        }
      }
      PC_Options_IO_App_Read_Data(pc_com);
      break;
    case SUBCOMMANDEND:
      PC_Send_Error_Code(received_adres, send_adres, NOCOMMAND, ERRORSUBCOMMANDEND);
      break;
  }
}

static void PC_Options_IO_App_Write(s_pc_com *pc_com)
{
unsigned int default_area_size;
unsigned int aantal;
unsigned int computer;
unsigned int soort, offset;

  pc_com->command = OPTIONS_IO_APP_WRITE;
  pc_com->command_timeout = COMMANDPCTIMEOUT;
  switch (received_sub_command)
  {
    case SUBCOMMANDBEGIN:
      if (restore_data_busy != 0)
      {
        PC_Send_Error_Code(received_adres, send_adres, OPTIONS_IO_APP_WRITE, ERRORRESTOREOPTIONSETPOINTBUSY);
      }
      else
      {
        restore_data_busy = TIME_OUT_RESTORE_DATA_BUSY;
        pc_com->old_ptr = pc_com->ptr = restore_data_buffer;
        pc_com->blok_cnt = 1;
        PC_Write_Data(pc_com);
      }  
      break;
    case SUBCOMMANDPREV:
      restore_data_busy = TIME_OUT_RESTORE_DATA_BUSY;
      pc_com->ptr = pc_com->old_ptr;
      PC_Write_Data(pc_com);
      break;
    case SUBCOMMANDNEXT:
      restore_data_busy = TIME_OUT_RESTORE_DATA_BUSY;
      if (received_blok_cnt == pc_com->blok_cnt)
      {
        pc_com->blok_cnt++;
        pc_com->old_ptr = pc_com->ptr;
      }
      else
        pc_com->ptr = pc_com->old_ptr;
      PC_Write_Data(pc_com);
      break;
    case SUBCOMMANDEND:
      restore_data_busy = 0;
      PC_Send_Error_Code(received_adres, send_adres, NOCOMMAND, ERRORSUBCOMMANDEND);
      return;
  }
  if (received_ETB_or_ETX == ETX_CH)
  {
    // opt_io
    default_area_size = (unsigned long)&default_opt_io.end - (unsigned long)&default_opt_io;
    aantal   = ((unsigned int)restore_data_buffer[1] << 8) + restore_data_buffer[0];
    computer = ((unsigned int)restore_data_buffer[3] << 8) + restore_data_buffer[2];
    soort    = ((unsigned int)restore_data_buffer[5] << 8) + restore_data_buffer[4];
    offset   = aantal;
    if ((computer == default_opt_io.computer) && (soort == default_opt_io.soort))
    {
      if (aantal > default_area_size)
        aantal = default_area_size;
      BlockCopy((unsigned char *)&restore_data_buffer[0],((unsigned char *)&opt_io),aantal);
      if (default_area_size > aantal)
        AddNewOptIO();

      VersieAfhankelijkOptIO();
      pc_0_read_configuration_alg = 1;  
      pc_1_read_configuration_alg = 1;
      pc_2_read_configuration_alg = 1;
      rs232_0_read_configuration_alg = 1;
      rs232_1_read_configuration_alg = 1;
      #ifdef ETHERNET
      ethernet_read_configuration_alg[0] = 1;
      ethernet_read_configuration_alg[1] = 1;
      ethernet_read_configuration_alg[2] = 1;
      ethernet_read_configuration_alg[3] = 1;
      #endif // ETHERNET
      IncrementOptionsChangeCount();
      ModuleCheck();
      CAN_IO_Init_All_Boards();
      install_flag = 0;
      CreateAlarm(&alarm_hr_alg.new_option_from_pc_al, SYSTEEM_AL_NEW_OPTION, 0, 0, 0, ZACHT_ALARM);
      ClearAlarm(&alarm_hr_alg.opt_al, SYSTEEM_AL_OPT, 0);
    }
    else
    {
      CreateAlarm(&alarm_hr_alg.restore_option_setpoint_failed_al, SYSTEEM_AL_RESTORE_OPTION_SETPOINT_FAILED, 0, 0, 0, HARD_ALARM);
    }

    // opt_app
    default_area_size = (unsigned long)&default_opt_app.end - (unsigned long)&default_opt_app;
    aantal =   ((unsigned int)restore_data_buffer[offset + 1] << 8) + restore_data_buffer[offset + 0];
    computer = ((unsigned int)restore_data_buffer[offset + 3] << 8) + restore_data_buffer[offset + 2];
    soort =    ((unsigned int)restore_data_buffer[offset + 5] << 8) + restore_data_buffer[offset + 4];
    if ((computer == default_opt_app.computer) && (soort == default_opt_app.soort))
    {
      if (aantal > default_area_size)
        aantal = default_area_size;
      BlockCopy((unsigned char *)&restore_data_buffer[offset],((unsigned char *)&opt_app),aantal);
      if (default_area_size > aantal)
        AddNewOptApp();

      VersieAfhankelijkOptApp();
      pc_0_read_configuration_alg = 1;  
      pc_1_read_configuration_alg = 1;
      pc_2_read_configuration_alg = 1;
      rs232_0_read_configuration_alg = 1;
      rs232_1_read_configuration_alg = 1;
      #ifdef ETHERNET
      ethernet_read_configuration_alg[0] = 1;
      ethernet_read_configuration_alg[1] = 1;
      ethernet_read_configuration_alg[2] = 1;
      ethernet_read_configuration_alg[3] = 1;
      #endif // ETHERNET
      IncrementOptionsChangeCount();
      ModuleCheck();
      CheckOptions();
      Init_All_Screen();
      install_flag = 0;
      CreateAlarm(&alarm_hr_alg.new_option_from_pc_al, SYSTEEM_AL_NEW_OPTION, 0, 0, 0, ZACHT_ALARM);
      ClearAlarm(&alarm_hr_alg.opt_al, SYSTEEM_AL_OPT, 0);
    }
    else
    {
      CreateAlarm(&alarm_hr_alg.restore_option_setpoint_failed_al, SYSTEEM_AL_RESTORE_OPTION_SETPOINT_FAILED, 0, 0, 0, HARD_ALARM);
    }
    restore_data_busy = 0;
  }
}

//-----------------------------------------------------------------------------
// zend gegevens met maingroep naar PC
void PC_Main_Group_Read_Data_Algemeen(s_pc_com *pc_com)
{
  alarm_code         = 0;
  alarm_val          = 0;
  actual_time        = 0;
  read_configuration = 0;

  if (install_flag && (password_enabled & PASSWORD_ENABLED_OPT_MASK))
    alarm_code = INSTALLATIE_AL;
  else
    alarm_code = alarm_disp_pc.code;

  switch (alarm_disp_pc.pc_type)
  {
    case ALARM_INDEX: alarm_val = alarm_disp_pc.index; break;
    case ALARM_VALUE: alarm_val = alarm_disp_pc.value; break;
  }  

  actual_time = time(0);

  if (pc_com == &can_backbone_pc_0_com)                           //  8
    read_configuration = pc_0_read_configuration_alg;
  else if (pc_com == &can_backbone_pc_1_com)
    read_configuration = pc_1_read_configuration_alg;
  else if (pc_com == &can_backbone_pc_2_com)
    read_configuration = pc_2_read_configuration_alg;
  else if ((pc_com == &rs232_0_orion_com) || (pc_com == &rs232_0_pc_com))
    read_configuration = rs232_0_read_configuration_alg;
  else if ((pc_com == &rs232_1_orion_com) || (pc_com == &rs232_1_pc_com))
    read_configuration = rs232_1_read_configuration_alg;
  #ifdef ETHERNET  
  else if ((pc_com == &ethernet_orion_com[0]) || (pc_com == &ethernet_pc_com[0]))
    read_configuration = ethernet_read_configuration_alg[0];
  else if ((pc_com == &ethernet_orion_com[1]) || (pc_com == &ethernet_pc_com[1]))
    read_configuration = ethernet_read_configuration_alg[1];
  else if ((pc_com == &ethernet_orion_com[2]) || (pc_com == &ethernet_pc_com[2]))
    read_configuration = ethernet_read_configuration_alg[2];
  else if ((pc_com == &ethernet_orion_com[3]) || (pc_com == &ethernet_pc_com[3]))
    read_configuration = ethernet_read_configuration_alg[3];
  #endif // ETHERNET  
}

// PC vraagt voor maingroup
static void PC_Main_Group_Read_Algemeen(s_pc_com *pc_com)
{
  PC_Main_Group_Read_Data_Algemeen(pc_com);
  PC_Table_Read(pc_com, (unsigned char *)&MainGroupTable);
}

// Orion verstuurd maingroup
static void PC_Main_Group_Changed_Algemeen(s_pc_com *pc_com)
{
  PC_Main_Group_Read_Data_Algemeen(pc_com);
  PC_Table_Send(pc_com, (unsigned char *)&MainGroupTable);

  if (received_sub_command == SUBCOMMANDEND)
  {
    if ((pc_com == &rs232_0_orion_com) || (pc_com == &rs232_0_pc_com))
      rs232_0_main_group_blocked = 0;
    else if ((pc_com == &rs232_1_orion_com) || (pc_com == &rs232_1_pc_com))
      rs232_1_main_group_blocked = 0;
    #ifdef ETHERNET
    else if ((pc_com == &ethernet_orion_com[0]) || (pc_com == &ethernet_pc_com[0]))
      ethernet_main_group_blocked[0] = 0;
    else if ((pc_com == &ethernet_orion_com[1]) || (pc_com == &ethernet_pc_com[1]))
      ethernet_main_group_blocked[1] = 0;
    else if ((pc_com == &ethernet_orion_com[2]) || (pc_com == &ethernet_pc_com[2]))
      ethernet_main_group_blocked[2] = 0;
    else if ((pc_com == &ethernet_orion_com[3]) || (pc_com == &ethernet_pc_com[3]))
      ethernet_main_group_blocked[3] = 0;
    #endif // ETHERNET
  }
}

//-----------------------------------------------------------------------------
static void PC_Save_Option_Algemeen(s_pc_com *pc_com)
{
// is gelijk aan commando PC_Option_Read_Algemeen() alleen commando is anders

  pc_com->command = ALLOPTIONSREAD;
  pc_com->command_timeout = COMMANDPCTIMEOUT;
  switch (received_sub_command)
  {
    case SUBCOMMANDBEGIN:
      pc_com->ptr = (unsigned char *)&opt_app;
      pc_com->blok_cnt = 0;
      PC_Read_Data(pc_com, (unsigned char *)&opt_app.end+1);
      break;
    case SUBCOMMANDPREV:
      PC_Read_Data(pc_com, (unsigned char *)&opt_app.end+1);
      break;
    case SUBCOMMANDNEXT:
      if (received_blok_cnt == (unsigned char)(pc_com->blok_cnt+1))
      {
        if (pc_com->ptr <= (unsigned char *)&opt_app.end)
        {
          pc_com->ptr += MAXDATA;
          pc_com->blok_cnt++;
        }
      }
      PC_Read_Data(pc_com, (unsigned char *)&opt_app.end+1);
      break;
    case SUBCOMMANDEND:
      PC_Send_Error_Code(received_adres, send_adres, NOCOMMAND, ERRORSUBCOMMANDEND);
      break;
  }
}

//-----------------------------------------------------------------------------
static void PC_Restore_Option_Algemeen(s_pc_com *pc_com)
{
unsigned int default_area_size;
unsigned int aantal;
unsigned int computer;
unsigned int soort;

  pc_com->command = ALLOPTIONSWRITE;
  pc_com->command_timeout = COMMANDPCTIMEOUT;
  switch (received_sub_command)
  {
    case SUBCOMMANDBEGIN:
      if (restore_data_busy != 0)
      {
        PC_Send_Error_Code(received_adres, send_adres, ALLOPTIONSWRITE, ERRORRESTOREOPTIONSETPOINTBUSY);
      }
      else
      {
        restore_data_busy = TIME_OUT_RESTORE_DATA_BUSY;
        pc_com->old_ptr = pc_com->ptr = restore_data_buffer;
        pc_com->blok_cnt = 1;
        PC_Write_Data(pc_com);
      }  
      break;
    case SUBCOMMANDPREV:
      restore_data_busy = TIME_OUT_RESTORE_DATA_BUSY;
      pc_com->ptr = pc_com->old_ptr;
      PC_Write_Data(pc_com);
      break;
    case SUBCOMMANDNEXT:
      restore_data_busy = TIME_OUT_RESTORE_DATA_BUSY;
      if (received_blok_cnt == pc_com->blok_cnt)
      {
        pc_com->blok_cnt++;
        pc_com->old_ptr = pc_com->ptr;
      }
      else
        pc_com->ptr = pc_com->old_ptr;
      PC_Write_Data(pc_com);
      break;
    case SUBCOMMANDEND:
      restore_data_busy = 0;
      PC_Send_Error_Code(received_adres, send_adres, NOCOMMAND, ERRORSUBCOMMANDEND);
      return;
  }
  if (received_ETB_or_ETX == ETX_CH)
  {
    default_area_size = (unsigned long)&default_opt_app.end - (unsigned long)&default_opt_app;
    aantal   = ((unsigned int)restore_data_buffer[1] << 8) + restore_data_buffer[0];
    computer = ((unsigned int)restore_data_buffer[3] << 8) + restore_data_buffer[2];
    soort    = ((unsigned int)restore_data_buffer[5] << 8) + restore_data_buffer[4];
    if ((computer == default_opt_app.computer) && (soort == default_opt_app.soort))
    {
      if (aantal > default_area_size)
        aantal = default_area_size;
      BlockCopy((unsigned char *)restore_data_buffer,((unsigned char *)&opt_app),aantal);

      if (default_area_size > aantal )
      {
        AddNewOptApp();
      }

//      opt_app.area_size = default_area_size;
//      opt_app.versie_programma = default_opt_app.versie_programma;
      VersieAfhankelijkOptApp();
      pc_0_read_configuration_alg = 1;  
      pc_1_read_configuration_alg = 1;
      pc_2_read_configuration_alg = 1;
      rs232_0_read_configuration_alg = 1;
      rs232_1_read_configuration_alg = 1;
      #ifdef ETHERNET
      ethernet_read_configuration_alg[0] = 1;
      ethernet_read_configuration_alg[1] = 1;
      ethernet_read_configuration_alg[2] = 1;
      ethernet_read_configuration_alg[3] = 1;
      #endif // ETHERNET
      can_backbone_appl_init_switch = 1;
      ModuleCheck();
      install_flag = 0;
      CreateAlarm(&alarm_hr_alg.new_option_from_pc_al, SYSTEEM_AL_NEW_OPTION, 0, 0, 0, ZACHT_ALARM);
      ClearAlarm(&alarm_hr_alg.opt_al, SYSTEEM_AL_OPT, 0);
    }
    else
    {
      CreateAlarm(&alarm_hr_alg.restore_option_setpoint_failed_al, SYSTEEM_AL_RESTORE_OPTION_SETPOINT_FAILED, 0, 0, 0, HARD_ALARM);
    }
    restore_data_busy = 0;
  }
}

//-----------------------------------------------------------------------------
static void PC_Save_Setpoint_Algemeen(s_pc_com *pc_com)
{
  pc_com->command = ALLSETPOINTSREAD;
  pc_com->command_timeout = COMMANDPCTIMEOUT;
  switch (received_sub_command)
  {
    case SUBCOMMANDBEGIN:
      pc_com->ptr = (unsigned char *)&setp_alg;
      pc_com->blok_cnt = 0;
      PC_Read_Data(pc_com, (unsigned char *)&setp_alg.end+1);
      break;
    case SUBCOMMANDPREV:
      PC_Read_Data(pc_com, (unsigned char *)&setp_alg.end+1);
      break;
    case SUBCOMMANDNEXT:
      if (received_blok_cnt == (unsigned char)(pc_com->blok_cnt+1))
      {
        if (pc_com->ptr <= (unsigned char *)&setp_alg.end)
        {
          pc_com->ptr += MAXDATA;
          pc_com->blok_cnt++;
        }
      }
      PC_Read_Data(pc_com, (unsigned char *)&setp_alg.end+1);
      break;
    case SUBCOMMANDEND:
      PC_Send_Error_Code(received_adres, send_adres, NOCOMMAND, ERRORSUBCOMMANDEND);
      break;
  }
}

//-----------------------------------------------------------------------------
static void PC_Restore_Setpoint_Algemeen(s_pc_com *pc_com)
{
unsigned int default_area_size;
unsigned int aantal;
unsigned int computer;
unsigned int soort;

  pc_com->command = ALLSETPOINTSWRITE;
  pc_com->command_timeout = COMMANDPCTIMEOUT;
  switch (received_sub_command)
  {
    case SUBCOMMANDBEGIN:
      if (restore_data_busy != 0)
      {
        PC_Send_Error_Code(received_adres, send_adres, ALLSETPOINTSWRITE, ERRORRESTOREOPTIONSETPOINTBUSY);
      }
      else
      {                     
        restore_data_busy = TIME_OUT_RESTORE_DATA_BUSY;
        pc_com->old_ptr = pc_com->ptr = restore_data_buffer;
        pc_com->blok_cnt = 1;
        PC_Write_Data(pc_com);
      }  
      break;
    case SUBCOMMANDPREV:
      restore_data_busy = TIME_OUT_RESTORE_DATA_BUSY;
      pc_com->ptr = pc_com->old_ptr;
      PC_Write_Data(pc_com);
      break;
    case SUBCOMMANDNEXT:
      restore_data_busy = TIME_OUT_RESTORE_DATA_BUSY;
      if (received_blok_cnt == pc_com->blok_cnt)
      {
        pc_com->blok_cnt++;
        pc_com->old_ptr = pc_com->ptr;
      }
      else
        pc_com->ptr = pc_com->old_ptr;
      PC_Write_Data(pc_com);
      break;
    case SUBCOMMANDEND:
      restore_data_busy = 0;
      PC_Send_Error_Code(received_adres, send_adres, NOCOMMAND, ERRORSUBCOMMANDEND);
      return;
  }
  if (received_ETB_or_ETX == ETX_CH)
  {
    default_area_size = (unsigned long)&default_setp_alg.end - (unsigned long)&default_setp_alg;
    aantal =   ((unsigned int)restore_data_buffer[1] << 8) + restore_data_buffer[0];
    computer = ((unsigned int)restore_data_buffer[3] << 8) + restore_data_buffer[2];
    soort =    ((unsigned int)restore_data_buffer[5] << 8) + restore_data_buffer[4];
    if ((computer == default_opt_alg.computer) &&
        (soort == default_opt_alg.soort))
    {
      if (aantal > default_area_size)
        aantal = default_area_size;
      BlockCopy((unsigned char *)restore_data_buffer,((unsigned char *)&setp_alg),aantal);
      if (((unsigned long)&default_setp_alg.end - (unsigned long)&default_setp_alg) > aantal )
      {
        AddNewSetpAlg();
      }

//      setp_alg.area_size = default_area_size;
      VersieAfhankelijkSetpAlg();
   
//      ForceCalcOptionSetpoint();
      CreateAlarm(&alarm_hr_alg.new_setpoint_from_pc_al, SYSTEEM_AL_NEW_SETPOINT, 0, 0, 0, ZACHT_ALARM); // Melding nieuwe setpoints ontvangen van PC 
      ClearAlarm(&alarm_hr_alg.setp_al, SYSTEEM_AL_SETP, 0);
    }
    else
    {
      CreateAlarm(&alarm_hr_alg.restore_option_setpoint_failed_al, SYSTEEM_AL_RESTORE_OPTION_SETPOINT_FAILED, 0, 0, 0, HARD_ALARM);
    }
    restore_data_busy = 0;
  }
}

//-----------------------------------------------------------------------------
static void PC_Write_Setpoint_Data_Algemeen(void)
{
unsigned char checksum;
unsigned int offset;

  if (received_lengte >= 3)
  {
    offset = Receive_Int();
    switch (received_lengte - 2)
    {
      case 1: // unsigned char of char data
        *(unsigned char *)((long)&setp_alg + offset) = Receive_Char();
        Put_Char(STX);
        checksum = STX;
        checksum = Send_Adres(send_adres/*PC_ADR*/, checksum); //  adres
        checksum = Send_Adres(received_adres, checksum); // adres
        checksum = Send_Command(received_command, checksum); //  command
        checksum = Send_Byte(SUBCOMMANDEND, checksum); // sub command
        checksum = Send_Char(0, checksum); // block_nr
        checksum = Send_Char(0, checksum); // aantal data bytes
        Send_Char(checksum,0);
        Put_Char(ETX_CH);
        Put_Char(ETX);
        Put_Char(0);
        break; 
      case 2: // unsigned int of int data
        *(unsigned int *)((long)&setp_alg + offset) = Receive_Int();
        Put_Char(STX);
        checksum = STX;
        checksum = Send_Adres(send_adres/*PC_ADR*/, checksum); //  adres
        checksum = Send_Adres(received_adres, checksum); // adres
        checksum = Send_Command(received_command, checksum); //  command
        checksum = Send_Byte(SUBCOMMANDEND, checksum); // sub command
        checksum = Send_Char(0, checksum); // block_nr
        checksum = Send_Char(0, checksum); // aantal data bytes
        Send_Char(checksum,0);
        Put_Char(ETX_CH);
        Put_Char(ETX);
        Put_Char(0);
        break;
      case 4: // unsigned long of long data
        *(unsigned long *)((long)&setp_alg + offset) = Receive_Long();
        Put_Char(STX);
        checksum = STX;
        checksum = Send_Adres(send_adres/*PC_ADR*/, checksum); //  adres
        checksum = Send_Adres(received_adres, checksum); // adres
        checksum = Send_Command(received_command, checksum); //  command
        checksum = Send_Byte(SUBCOMMANDEND, checksum); // sub command
        checksum = Send_Char(0, checksum); // block_nr
        checksum = Send_Char(0, checksum); // aantal data bytes
        Send_Char(checksum,0);
        Put_Char(ETX_CH);
        Put_Char(ETX);
        Put_Char(0);
        break; 
    }
  }
  else
    PC_Send_Error_Code(received_adres, send_adres, NOCOMMAND, ERRORLENGTE);
}

static void PC_One_Setpoint_Write_Algemeen(s_pc_com *pc_com)
{
  pc_com->command = received_command;
  pc_com->command_timeout = COMMANDPCTIMEOUT;
  switch (received_sub_command)
  {
    case SUBCOMMANDBEGIN:
    case SUBCOMMANDPREV:
    case SUBCOMMANDNEXT:
      PC_Write_Setpoint_Data_Algemeen();
      break;
    case SUBCOMMANDEND: 
      PC_Send_Error_Code(received_adres, send_adres, NOCOMMAND, ERRORSUBCOMMANDEND);
      break;
  }
}

//-----------------------------------------------------------------------------
static void PC_Alarm_Read_Data_Algemeen(s_pc_com *pc_com)
{
s_alarm_pc_com *ptr_alarm = (s_alarm_pc_com *)pc_com->ptr_receive_buffer;
unsigned char checksum;
int tel = pc_com->cnt;

  if (tel > ALARMDATA)
    tel = ALARMDATA;
  Put_Char(STX);
  checksum = STX;
  checksum = Send_Adres(send_adres, checksum); //  adres
  checksum = Send_Adres(received_adres, checksum); // adres
  checksum = Send_Command(received_command, checksum); //  command
  checksum = Send_Byte(received_sub_command, checksum); // sub command
  checksum = Send_Char(pc_com->blok_cnt, checksum); // block_nr
  checksum = Send_Char(tel * 12, checksum); // aantal data bytes
  while (tel)
  {
    checksum = Send_Int(ptr_alarm[pc_com->index].code,checksum);
    checksum = Send_Int(ptr_alarm[pc_com->index].index_or_value,checksum);
    checksum = Send_Long(ptr_alarm[pc_com->index].on,checksum);
    checksum = Send_Long(ptr_alarm[pc_com->index].off,checksum);
    pc_com->index--;
    tel--;
  } 
  Send_Char(checksum,0);
  if (pc_com->cnt <= ALARMDATA) 
    Put_Char(ETX_CH);
  else
    Put_Char(ETB_CH);
  Put_Char(ETX);
  Put_Char(0);
}

static void PC_Alarm_Read_Algemeen(s_pc_com *pc_com)
{
s_alarm_pc_com *ptr_alarm = (s_alarm_pc_com *)pc_com->ptr_receive_buffer;

  pc_com->command = received_command;
  pc_com->command_timeout = COMMANDPCTIMEOUT;
  switch (received_sub_command)
  {
    case SUBCOMMANDBEGIN:
      pc_com->index = alarm_hr_alg.alarmen.index;
      pc_com->cnt = 0;
      do
      {
        if ((alarm_hr_alg.alarmen.al[pc_com->index].code != GEEN_AL) &&
            (alarm_hr_alg.alarmen.al[pc_com->index].mask & MASK_ALG_PC))
        {    
          pc_com->cnt++;
          ptr_alarm->code = alarm_hr_alg.alarmen.al[pc_com->index].code;
          switch(alarm_hr_alg.alarmen.al[pc_com->index].pc_type)
          {
            case ALARM_VAL_0: ptr_alarm->index_or_value = 0; break;
            case ALARM_INDEX: ptr_alarm->index_or_value = alarm_hr_alg.alarmen.al[pc_com->index].index; break;
            case ALARM_VALUE: ptr_alarm->index_or_value = alarm_hr_alg.alarmen.al[pc_com->index].value; break;
          }
          ptr_alarm->on             = alarm_hr_alg.alarmen.al[pc_com->index].on;
          ptr_alarm->off            = alarm_hr_alg.alarmen.al[pc_com->index].off;
          ptr_alarm++;
        }  
        if (pc_com->index <= 0)
          pc_com->index = ALARM_DISP_VALUES-1;
        else
          pc_com->index--;
      }
      while (pc_com->index != alarm_hr_alg.alarmen.index);
      pc_com->blok_cnt = 0;
      pc_com->index = pc_com->cnt - 1;
      pc_com->old_index = pc_com->index;
      PC_Alarm_Read_Data_Algemeen(pc_com);
      break;
    case SUBCOMMANDPREV:
      pc_com->index = pc_com->old_index;
      PC_Alarm_Read_Data_Algemeen(pc_com);
      break;
    case SUBCOMMANDNEXT:
      if (received_blok_cnt == (unsigned char)(pc_com->blok_cnt+1))
      {
        pc_com->blok_cnt++;
        pc_com->old_index = pc_com->index;
        pc_com->cnt -= ALARMDATA;
      }
      else
        pc_com->index = pc_com->old_index;
      PC_Alarm_Read_Data_Algemeen(pc_com);
      break;
    case SUBCOMMANDEND: 
      PC_Send_Error_Code(received_adres, send_adres, NOCOMMAND, ERRORSUBCOMMANDEND);
      break;
  }
}

static void PC_Alarm_Write_Algemeen(s_pc_com *pc_com)
// reset alarm gegevens
{
unsigned char checksum;

  pc_com->command = ALARMWRITE;
  pc_com->command_timeout = COMMANDPCTIMEOUT;
  switch (received_sub_command)
  {
    case SUBCOMMANDBEGIN:
    case SUBCOMMANDPREV:
    case SUBCOMMANDNEXT:
      Alarm_Reset_Data_PC_Algemeen();
      Put_Char(STX);
      checksum = STX;
      checksum = Send_Adres(send_adres/*PC_ADR*/, checksum); //  adres
      checksum = Send_Adres(received_adres, checksum); // adres
      checksum = Send_Command(ALARMWRITE, checksum); //  command
      checksum = Send_Byte(SUBCOMMANDEND, checksum); // sub command
      checksum = Send_Char(0, checksum); // block_nr
      checksum = Send_Char(0, checksum); // aantal data bytes
      Send_Char(checksum,0);
      Put_Char(ETX_CH);
      Put_Char(ETX);
      Put_Char(0);
//        pc_com->command = NOCOMMAND;
      break;
    case SUBCOMMANDEND:
      PC_Send_Error_Code(received_adres, send_adres, NOCOMMAND, ERRORSUBCOMMANDEND);
      break;
  }
}

//-----------------------------------------------------------------------------
void PC_Table_Read_Data(s_pc_com *pc_com)
{
unsigned char checksum;
unsigned char length = 0;
unsigned char msg_length = 0;
unsigned char aantal_bytes;
s_PcDataTable *Table;
void *value_ptr;

  Put_Char(STX);
  checksum = STX;
  checksum = Send_Adres(send_adres, checksum); //  adres
  checksum = Send_Adres(received_adres, checksum); // adres
  checksum = Send_Command(received_command, checksum); //  command
  checksum = Send_Byte(received_sub_command, checksum); // sub command
  checksum = Send_Char(pc_com->blok_cnt, checksum); // block_nr

  aantal_bytes = 0;
  Table = (s_PcDataTable *)pc_com->ptr;
  while ((aantal_bytes < MAXDATA) && (Table->ptr != NULL)) // bepaal lengte van bericht
  {
    switch (Table->type)
    {
      case TypeChar:
      case TypeCharIndexed: length =  1; break;
      case TypeInt:
      case TypeIntIndexed:  length =  2; break;
      case TypeLong:
      case TypeLongIndexed: length =  4; break;
      case TypeCurve:       length = 43; break;
      case TypeCurveTemp:   length =   8; break;
      case TypeCurveKlok:   length = 100; break;
      case TypeTime:        length =   2; break;
    }
    aantal_bytes += length;
    Table++;
  }
  if (aantal_bytes > MAXDATA)
    msg_length = aantal_bytes - length;
  else
    msg_length = aantal_bytes;
  
  checksum = Send_Char(msg_length, checksum); // aantal data bytes
    
  aantal_bytes = 0;
  Table = (s_PcDataTable *)pc_com->ptr;
  while ((aantal_bytes < msg_length) && (Table->ptr != NULL)) // zend data
  {
    value_ptr = (void *)Get_Ptr(Table->ptr, commando_index); // set pointer at section "commando_index"
    switch (Table->type)
    {
      case TypeChar:
        length = 1;
        checksum = Send_Char(*(unsigned char *)value_ptr, checksum);
        break;
      case TypeInt:
        length = 2;
        checksum = Send_Int(*(unsigned int *)value_ptr, checksum);
        break;
      case TypeLong:
        length = 4;
        checksum = Send_Long(*(unsigned long *)value_ptr, checksum);
        break;
/*
      case TypeCurve:
        length = 43;
        checksum = Send_Int(((s_curve *)value_ptr)->cor, checksum);
        checksum = Send_Char(((s_curve *)value_ptr)->nr, checksum);
        for (loop = 0; loop < 10; loop++)
        {
          checksum = Send_Int(((s_curve *)value_ptr)->p[loop].dag, checksum);
          checksum = Send_Int(((s_curve *)value_ptr)->p[loop].val, checksum);
        }
        break;
      case TypeCurveTemp:
        length = 8;
        checksum = Send_Int(((s_temp_curve *)value_ptr)->p[0].temp, checksum);
        checksum = Send_Int(((s_temp_curve *)value_ptr)->p[0].val,  checksum); 
        checksum = Send_Int(((s_temp_curve *)value_ptr)->p[1].temp, checksum);
        checksum = Send_Int(((s_temp_curve *)value_ptr)->p[1].val,  checksum); 
        break;
      case TypeCurveKlok:
        length = 100;
        checksum = Send_Int(((s_curve_klok *)value_ptr)->on_time, checksum);
        checksum = Send_Int(((s_curve_klok *)value_ptr)->points,  checksum);
        for (loop = 0; loop < 24; loop++)
        {
          checksum = Send_Char(((s_curve_klok *)value_ptr)->p[loop].on.hour,  checksum);
          checksum = Send_Char(((s_curve_klok *)value_ptr)->p[loop].on.min,   checksum);
          checksum = Send_Char(((s_curve_klok *)value_ptr)->p[loop].off.hour, checksum);
          checksum = Send_Char(((s_curve_klok *)value_ptr)->p[loop].off.min,  checksum);
        }
        break;
*/
      case TypeTime:
        length = 2;
        checksum = Send_Char(((s_time *)value_ptr)->hour, checksum);
        checksum = Send_Char(((s_time *)value_ptr)->min,  checksum);
        break;
    }
    aantal_bytes += length;
    Table++;
  }
  Send_Char(checksum, 0);
  if (Table->ptr == NULL) // alle data is verzonden
    Put_Char(ETX_CH);
  else
  {
    pc_com->ptr = (unsigned char *)Table;
    Put_Char(ETB_CH);
  }
  Put_Char(ETX);
  Put_Char(0);
}

// PC vraagt data
void PC_Table_Read(s_pc_com *pc_com, unsigned char *Table)
{
  pc_com->command = received_command;
  pc_com->command_timeout = COMMANDPCTIMEOUT;
  switch (received_sub_command)
  {
    case SUBCOMMANDBEGIN:
      pc_com->blok_cnt = 0;
      pc_com->old_ptr = pc_com->ptr = Table;
      PC_Table_Read_Data(pc_com);
      break;
    case SUBCOMMANDPREV:
      pc_com->ptr = pc_com->old_ptr;
      PC_Table_Read_Data(pc_com);
      break;
    case SUBCOMMANDNEXT:
      if (received_blok_cnt == (unsigned char)(pc_com->blok_cnt+1))
      {
        pc_com->blok_cnt++;
        pc_com->old_ptr = pc_com->ptr;
      }
      else
        pc_com->ptr = pc_com->old_ptr;
      PC_Table_Read_Data(pc_com);
      break;
    case SUBCOMMANDEND: 
      PC_Send_Error_Code(received_adres, send_adres, NOCOMMAND, ERRORSUBCOMMANDEND);
      break;
  }
}

// Orion verstuurd data
void PC_Table_Send(s_pc_com *pc_com, unsigned char *Table)
{
  pc_com->command = received_command;
  pc_com->command_timeout = COMMANDORIONTIMEOUT;
  switch (received_sub_command)
  {
    case SUBCOMMANDBEGIN:
      pc_com->blok_cnt = 0;
      pc_com->old_ptr = pc_com->ptr = Table;
      PC_Table_Read_Data(pc_com);
      break;
    case SUBCOMMANDPREV:
      if (pc_com->error_cnt < COMMANDORIONERRORMAX)
      {
        pc_com->error_cnt++;
        pc_com->ptr = pc_com->old_ptr;
        PC_Table_Read_Data(pc_com);
      }
      else
        pc_com->command = NOCOMMAND;
      break;
    case SUBCOMMANDNEXT:
      if (received_blok_cnt == (unsigned char)(pc_com->blok_cnt+1))
      {
        pc_com->blok_cnt++;
        pc_com->old_ptr = pc_com->ptr;
        pc_com->error_cnt = 0; 
      }
      else
        pc_com->ptr = pc_com->old_ptr;
      PC_Table_Read_Data(pc_com);
      break;
    case SUBCOMMANDEND: 
      pc_com->error_cnt = 0;
      pc_com->command = NOCOMMAND;
      pc_com->command_timeout = COMMANDORIONTIMEOUT;
      break;
  }
}

//-----------------------------------------------------------------------------
static void PC_Module_Read_Data_Algemeen(s_pc_com *pc_com, unsigned char *end)
{
unsigned char endchar = (pc_com->ptr + MAXDATA < end) ? ETB_CH : ETX_CH;
unsigned char checksum;

  if (endchar == ETB_CH)
    end = pc_com->ptr + MAXDATA;
  Put_Char(STX);
  checksum = STX;
  checksum = Send_Adres(send_adres/*PC_ADR*/, checksum); // adres
  checksum = Send_Adres(received_adres, checksum); // adres
  checksum = Send_Command(received_command, checksum); // command
  checksum = Send_Byte(received_sub_command, checksum); // sub command
  checksum = Send_Char(pc_com->blok_cnt, checksum); // block_nr
  checksum = Send_Char(end - pc_com->ptr, checksum);
  if ((unsigned long)pc_com->ptr == (unsigned long)&module)
  {
    checksum = Send_Int(rom.computer,checksum);         // 4
    checksum = Send_Int(rom.soort,checksum);              // 4
    checksum = Send_Int(module.type,checksum);          // 4   
    checksum = Send_Int(module.firma,checksum);         // 4   
    checksum = Send_Int(rom.versie_programma,checksum); // 4
    checksum = Send_Int(rom.versie_PC,checksum);        // 4
    checksum = Send_Long(module.serie_number,checksum); // 8  
    checksum = Send_Int(rom.versie_tekst,checksum);      // 4
    checksum = Send_Int(rom.versie_tekst_inst,checksum);      // 4
    checksum = Send_Int(module.reserve2,checksum);      // 4
    checksum = Send_Int(module.reserve3,checksum);      // 4
    checksum = Send_Int(module.reserve4,checksum);      // 4
    checksum = Send_Int(module.dummy0,checksum);        // 4
    checksum = Send_Int(module.dummy1,checksum);        // 4
    checksum = Send_Int(module.dummy2,checksum);        // 4
    checksum = Send_Int(module.dummy3,checksum);        // 4
    checksum = Send_Int(module.dummy4,checksum);        // 4
    checksum = Send_Int(module.dummy5,checksum);        // 4
    checksum = Send_Int(module.dummy6,checksum);        // 4
    pc_com->ptr += 40;
  }
  while (pc_com->ptr < end)
  {
    checksum = Send_Char(*pc_com->ptr, checksum);
    pc_com->ptr++;
  }
  Send_Char(checksum,0);
  Put_Char(endchar);
  Put_Char(ETX);
  Put_Char(0);
}

static void PC_Module_Read_Algemeen(s_pc_com *pc_com)
{
// is gelijk aan commando PC_Save_Option() alleen commando is anders

  pc_com->command = MODULEREAD;
  pc_com->command_timeout = COMMANDPCTIMEOUT;
  switch (received_sub_command)
  {
    case SUBCOMMANDBEGIN:
      pc_com->old_ptr = pc_com->ptr = (unsigned char *)&module;
      pc_com->blok_cnt = 0;
      PC_Module_Read_Data_Algemeen(pc_com, (unsigned char *)&module+126);
      break;
    case SUBCOMMANDPREV:
      pc_com->ptr = pc_com->old_ptr;
      PC_Module_Read_Data_Algemeen(pc_com, (unsigned char *)&module+126);
      break;
    case SUBCOMMANDNEXT:
      if (received_blok_cnt == (unsigned char)(pc_com->blok_cnt+1))
      {
        pc_com->blok_cnt++;
        pc_com->old_ptr = pc_com->ptr;
      }
      else
        pc_com->ptr = pc_com->old_ptr;
      PC_Module_Read_Data_Algemeen(pc_com, (unsigned char *)&module+126);
      break;
    case SUBCOMMANDEND:
      PC_Send_Error_Code(received_adres, send_adres, NOCOMMAND, ERRORSUBCOMMANDEND);
      break;
  }
}

static void PC_Module_Write_Algemeen(s_pc_com *pc_com)
{
unsigned char *in,*mod;
int loop;

  pc_com->command = MODULEWRITE;
  pc_com->command_timeout = COMMANDPCTIMEOUT;
  switch (received_sub_command)
  {  
    case SUBCOMMANDBEGIN:
      pc_com->old_ptr = pc_com->ptr = (unsigned char *)pc_com->ptr_receive_buffer;
      pc_com->blok_cnt = 1;
      PC_Write_Data(pc_com);
      break;
    case SUBCOMMANDPREV:
      pc_com->ptr = pc_com->old_ptr;
      PC_Write_Data(pc_com);
      break;
    case SUBCOMMANDNEXT:
      if (received_blok_cnt == pc_com->blok_cnt)
      {
        pc_com->blok_cnt++;
        pc_com->old_ptr = pc_com->ptr;
      }
      else
        pc_com->ptr = pc_com->old_ptr;
      PC_Write_Data(pc_com);
      break;
    case SUBCOMMANDEND:
      PC_Send_Error_Code(received_adres, send_adres, NOCOMMAND, ERRORSUBCOMMANDEND);
      return;
  }
  if (received_ETB_or_ETX == ETX_CH)
  {
    in = (unsigned char *)pc_com->ptr_receive_buffer;
    in = Get_Int(in,&module.computer);
    in = Get_Int(in,&module.soort);
    in = Get_Int(in,&module.type);
    in = Get_Int(in,&module.firma);
    in = Get_Int(in,&module.versie_programma);
    in = Get_Int(in,&module.versie_PC);
    in = Get_Long(in,&module.serie_number);
    mod = (unsigned char *)&module.serie_number;
    mod += 4;
    for (loop = 0; loop < 110; loop++)
    {
      *mod++ = *in++;
    }
    module.computer = rom.computer;
    module.soort = rom.soort;
    module.versie_programma = rom.versie_programma;
    module.versie_PC = rom.versie_PC;
    if (opt_alg.type != module.type)
    {
      opt_alg.type = module.type;
      IncrementOptionsChangeCount();
      pc_0_read_configuration_alg = 1;  
      pc_1_read_configuration_alg = 1;
      pc_2_read_configuration_alg = 1;
      rs232_0_read_configuration_alg = 1;
      rs232_1_read_configuration_alg = 1;
      #ifdef ETHERNET
      ethernet_read_configuration_alg[0] = 1;
      ethernet_read_configuration_alg[1] = 1;
      ethernet_read_configuration_alg[2] = 1;
      ethernet_read_configuration_alg[3] = 1;
      #endif // ETHERNET
    }
    if (opt_alg.firma != module.firma)
    {
      opt_alg.firma = module.firma;
      IncrementOptionsChangeCount();
      pc_0_read_configuration_alg = 1;  
      pc_1_read_configuration_alg = 1;
      pc_2_read_configuration_alg = 1;
      rs232_0_read_configuration_alg = 1;
      rs232_1_read_configuration_alg = 1;
      #ifdef ETHERNET
      ethernet_read_configuration_alg[0] = 1;
      ethernet_read_configuration_alg[1] = 1;
      ethernet_read_configuration_alg[2] = 1;
      ethernet_read_configuration_alg[3] = 1;
      #endif // ETHERNET
    }
    if (opt_alg.serie_number != module.serie_number)
    {
      opt_alg.serie_number = module.serie_number;
      IncrementOptionsChangeCount();
      pc_0_read_configuration_alg = 1;  
      pc_1_read_configuration_alg = 1;
      pc_2_read_configuration_alg = 1;
      rs232_0_read_configuration_alg = 1;
      rs232_1_read_configuration_alg = 1;
      #ifdef ETHERNET
      ethernet_read_configuration_alg[0] = 1;
      ethernet_read_configuration_alg[1] = 1;
      ethernet_read_configuration_alg[2] = 1;
      ethernet_read_configuration_alg[3] = 1;
      #endif // ETHERNET
    }
    if (setp_alg.type != module.type)
      setp_alg.type = module.type;
    if (setp_alg.firma != module.firma)
      setp_alg.firma = module.firma;
    if (setp_alg.serie_number != module.serie_number)
      setp_alg.serie_number = module.serie_number;
    vlag_module_write = 1;
    comp_ram_eep_switch = 1; 
    ModuleCheck();
    Init_All_Screen();
    install_flag = 0;
  }
}

//-----------------------------------------------------------------------------
static void PC_Tekst_Read_Data_Algemeen(s_pc_com *pc_com, unsigned long end_adress)
{
unsigned char loop;
unsigned char checksum;
s_tekst_50 *regel;

  Put_Char(STX);
  checksum = STX;
  checksum = Send_Adres(send_adres/*PC_ADR*/, checksum); // adres
  checksum = Send_Adres(received_adres, checksum); // adres
  checksum = Send_Command(received_command, checksum); // command
  checksum = Send_Byte(received_sub_command, checksum); // sub command
  checksum = Send_Char(pc_com->blok_cnt, checksum); // block_nr
  regel = (s_tekst_50 *)pc_com->ptr;
  checksum = Send_Char(regel->max_char + 3, checksum); // lengte
  checksum = Send_Char(regel->max_char, checksum);
  checksum = Send_Char(regel->max_bits, checksum);
  checksum = Send_Char(regel->font_type, checksum);
  for (loop = 0; loop < regel->max_char; loop++)
    checksum = Send_Char(regel->string[loop], checksum);
  pc_com->ptr += 3 + regel->max_char;
  if ((regel->max_char % 2) == 0) // uitlijnen op even byte
    pc_com->ptr += 1;
  Send_Char(checksum,0);
  if ((unsigned long)pc_com->ptr < end_adress)
    Put_Char(ETB_CH);
  else
    Put_Char(ETX_CH);
  Put_Char(ETX);
  Put_Char(0);
}

//-----------------------------------------------------------------------------
void PC_Tekst_Read_Algemeen(unsigned char taal, s_pc_com *pc_com)
{
// is gelijk aan commando PC_Save_Option() alleen commando is anders
unsigned long end_adres;

  pc_com->command = received_command;
  pc_com->command_timeout = COMMANDPCTIMEOUT;
  switch (taal)
  {
    default:
    case ENGELS: end_adres = (unsigned long)&tekst_engels + sizeof(s_tekst); break;
    case NEDERLANDS: end_adres = (unsigned long)&tekst_nederlands + sizeof(s_tekst); break;
    case DUITS: end_adres = (unsigned long)&tekst_duits + sizeof(s_tekst); break;
    case SPAANS: end_adres = (unsigned long)&tekst_spaans + sizeof(s_tekst); break;
    case USER: end_adres = (unsigned long)&tekst + sizeof(s_tekst); break;
  }
  switch (received_sub_command)
  {
    case SUBCOMMANDBEGIN:
      switch (taal)
      {
        default:
        case ENGELS: pc_com->old_ptr = pc_com->ptr = (unsigned char *)&tekst_engels.Gekozen_Taal_14; break;
        case NEDERLANDS: pc_com->old_ptr = pc_com->ptr = (unsigned char *)&tekst_nederlands.Gekozen_Taal_14; break;
        case DUITS: pc_com->old_ptr = pc_com->ptr = (unsigned char *)&tekst_duits.Gekozen_Taal_14; break;
        case SPAANS: pc_com->old_ptr = pc_com->ptr = (unsigned char *)&tekst_spaans.Gekozen_Taal_14; break;
        case USER: pc_com->old_ptr = pc_com->ptr = (unsigned char *)&tekst.Gekozen_Taal_14; break;
      }
      pc_com->blok_cnt = 0;
      PC_Tekst_Read_Data_Algemeen(pc_com,end_adres);
      break;
    case SUBCOMMANDPREV:
      pc_com->ptr = pc_com->old_ptr;
      PC_Tekst_Read_Data_Algemeen(pc_com,end_adres);
      break;
    case SUBCOMMANDNEXT:
      if (received_blok_cnt == (unsigned char)(pc_com->blok_cnt+1))
      {
        pc_com->blok_cnt++;
        pc_com->old_ptr = pc_com->ptr;
      }
      else
        pc_com->ptr = pc_com->old_ptr;
      PC_Tekst_Read_Data_Algemeen(pc_com,end_adres);
      break;
    case SUBCOMMANDEND:
      PC_Send_Error_Code(received_adres, send_adres, NOCOMMAND, ERRORSUBCOMMANDEND);
      break;
  }
}

//-----------------------------------------------------------------------------
void PC_Tekst_Write_Data_Algemeen(s_pc_com *pc_com, unsigned long end_adress)
{
unsigned char loop;
unsigned char checksum;
unsigned char cnt = received_lengte;
s_tekst_50 *regel;

  Put_Char(STX);
  checksum = STX;
  checksum = Send_Adres(send_adres/*PC_ADR*/, checksum); // adres
  checksum = Send_Adres(received_adres, checksum); // adres
  checksum = Send_Command(received_command, checksum); // command
  if (received_ETB_or_ETX == ETX_CH)
  {
    checksum = Send_Byte(SUBCOMMANDEND, checksum); // sub command
    checksum = Send_Char(0, checksum); // block_nr
  }
  else
  {
    checksum = Send_Byte(SUBCOMMANDNEXT, checksum); // sub command
    checksum = Send_Char(pc_com->blok_cnt, checksum); // block_nr
  }  
  checksum = Send_Char(0, checksum);
  if ((unsigned long)pc_com->ptr < end_adress)
  {
    regel = (s_tekst_50 *)pc_com->ptr;
    Receive_Char(); received_lengte--; // max_char
    Receive_Char(); received_lengte--; // max_bits
//    regel->font_type = Receive_Char(); received_lengte--;
    regel->font_type = (Receive_Char() & 0xF0) | (regel->font_type & 0x0F); received_lengte--; // alleen font aanpassen niet grote
    for (loop = 0; loop < regel->max_char - 1; loop++)
    {
      if (received_lengte)
      {
        regel->string[loop] = Receive_Char();
        received_lengte--;
      }
      else 
        regel->string[loop] = 0;  
    }
    regel->string[regel->max_char - 1] = 0;
    while (received_lengte)
    {
      Receive_Char();
      received_lengte--;
    }
    Receive_Char(); // verwijder checksum
    pc_com->ptr += 3 + regel->max_char;
    if ((regel->max_char % 2) == 0) // uitlijnen op even byte
      pc_com->ptr += 1;
//  ASC1_Printf("IN: %s\r",(char *)regel->string);
  }  
  Send_Char(checksum, 0);
  Put_Char(ETX_CH);
  Put_Char(ETX);
  Put_Char(0);
}

void PC_Tekst_Write_Algemeen(s_pc_com *pc_com)
{
  pc_com->command = received_command;
  pc_com->command_timeout = COMMANDPCTIMEOUT;
  switch (received_sub_command)
  {  
    case SUBCOMMANDBEGIN:
      tekst = tekst_engels;
      pc_com->old_ptr = pc_com->ptr = (unsigned char *)&tekst.Gekozen_Taal_14;
      pc_com->blok_cnt = 1;
      PC_Tekst_Write_Data_Algemeen(pc_com,(unsigned long)&tekst + sizeof(tekst));
      break;
    case SUBCOMMANDPREV:
      pc_com->ptr = pc_com->old_ptr;
      PC_Tekst_Write_Data_Algemeen(pc_com,(unsigned long)&tekst + sizeof(tekst));
      break;
    case SUBCOMMANDNEXT:
      if (received_blok_cnt == pc_com->blok_cnt)
      {
        pc_com->blok_cnt++;
        pc_com->old_ptr = pc_com->ptr;
      }
      else
        pc_com->ptr = pc_com->old_ptr;
      PC_Tekst_Write_Data_Algemeen(pc_com,(unsigned long)&tekst + sizeof(tekst));
      break;
    case SUBCOMMANDEND:
      PC_Send_Error_Code(received_adres, send_adres, NOCOMMAND, ERRORSUBCOMMANDEND);
      return;
  }
  if (received_ETB_or_ETX == ETX_CH)
  {
    // write binngekomen data ook naar EEPROM
    // en zet taal op USER
    opt_alg.taalkeuze = USER;
    EEP_taal_tekst_write_switch = 1;
  }
}

//-----------------------------------------------------------------------------
void PC_Tekst_MC_Read(s_pc_com *pc_com)
{
unsigned long end_adres;

  pc_com->command = received_command;
  pc_com->command_timeout = COMMANDPCTIMEOUT;
  end_adres = (unsigned long)&tekst.Status_10;
  switch (received_sub_command)
  {
    case SUBCOMMANDBEGIN:
      pc_com->old_ptr = pc_com->ptr = (unsigned char *)&tekst.Groep_1_10;
      pc_com->blok_cnt = 0;
      PC_Tekst_Read_Data_Algemeen(pc_com,end_adres);
      break;
    case SUBCOMMANDPREV:
      pc_com->ptr = pc_com->old_ptr;
      PC_Tekst_Read_Data_Algemeen(pc_com,end_adres);
      break;
    case SUBCOMMANDNEXT:
      if (received_blok_cnt == (unsigned char)(pc_com->blok_cnt+1))
      {
        pc_com->blok_cnt++;
        pc_com->old_ptr = pc_com->ptr;
      }
      else
        pc_com->ptr = pc_com->old_ptr;
      PC_Tekst_Read_Data_Algemeen(pc_com,end_adres);
      break;
    case SUBCOMMANDEND:
      PC_Send_Error_Code(received_adres, send_adres, NOCOMMAND, ERRORSUBCOMMANDEND);
      break;
  }
}

//-----------------------------------------------------------------------------
void PC_Tekst_MC_Write(s_pc_com *pc_com)
{
  pc_com->command = received_command;
  pc_com->command_timeout = COMMANDPCTIMEOUT;
  switch (received_sub_command)
  {  
    case SUBCOMMANDBEGIN:
      tekst = tekst_nederlands;
      pc_com->old_ptr = pc_com->ptr = (unsigned char *)&tekst.Groep_1_10;
      pc_com->blok_cnt = 1;
      PC_Tekst_Write_Data_Algemeen(pc_com,(unsigned long)&tekst.Status_10);
      break;
    case SUBCOMMANDPREV:
      pc_com->ptr = pc_com->old_ptr;
      PC_Tekst_Write_Data_Algemeen(pc_com,(unsigned long)&tekst.Status_10);
      break;
    case SUBCOMMANDNEXT:
      if (received_blok_cnt == pc_com->blok_cnt)
      {
        pc_com->blok_cnt++;
        pc_com->old_ptr = pc_com->ptr;
      }
      else
        pc_com->ptr = pc_com->old_ptr;
      PC_Tekst_Write_Data_Algemeen(pc_com,(unsigned long)&tekst.Status_10);
      break;
    case SUBCOMMANDEND:
      PC_Send_Error_Code(received_adres, send_adres, NOCOMMAND, ERRORSUBCOMMANDEND);
      return;
  }
  if (received_ETB_or_ETX == ETX_CH)
  {
    // write binngekomen data ook naar EEPROM
    // en zet taal op USER
    strcpy((char *)&tekst.Groep_1_14.string,  (const char *)&tekst.Groep_1_10.string);
    strcpy((char *)&tekst.Groep_2_14.string,  (const char *)&tekst.Groep_2_10.string);
    strcpy((char *)&tekst.Groep_3_14.string,  (const char *)&tekst.Groep_3_10.string);
    strcpy((char *)&tekst.Groep_4_14.string,  (const char *)&tekst.Groep_4_10.string);
    strcpy((char *)&tekst.Groep_5_14.string,  (const char *)&tekst.Groep_5_10.string);
    strcpy((char *)&tekst.Groep_6_14.string,  (const char *)&tekst.Groep_6_10.string);
    strcpy((char *)&tekst.Groep_7_14.string,  (const char *)&tekst.Groep_7_10.string);
    strcpy((char *)&tekst.Groep_8_14.string,  (const char *)&tekst.Groep_8_10.string);
    strcpy((char *)&tekst.Groep_9_14.string,  (const char *)&tekst.Groep_9_10.string);
    strcpy((char *)&tekst.Groep_10_14.string, (const char *)&tekst.Groep_10_10.string);
    strcpy((char *)&tekst.Groep_11_14.string, (const char *)&tekst.Groep_11_10.string);
    strcpy((char *)&tekst.Groep_12_14.string, (const char *)&tekst.Groep_12_10.string);
    strcpy((char *)&tekst.Groep_13_14.string, (const char *)&tekst.Groep_13_10.string);
    strcpy((char *)&tekst.Groep_14_14.string, (const char *)&tekst.Groep_14_10.string);
    strcpy((char *)&tekst.Groep_15_14.string, (const char *)&tekst.Groep_15_10.string);
    strcpy((char *)&tekst.Groep_16_14.string, (const char *)&tekst.Groep_16_10.string);
    strcpy((char *)&tekst.Groep_17_14.string, (const char *)&tekst.Groep_17_10.string);
    strcpy((char *)&tekst.Groep_18_14.string, (const char *)&tekst.Groep_18_10.string);
    strcpy((char *)&tekst.Groep_19_14.string, (const char *)&tekst.Groep_19_10.string);
    strcpy((char *)&tekst.Groep_20_14.string, (const char *)&tekst.Groep_20_10.string);
    strcpy((char *)&tekst.Groep_21_14.string, (const char *)&tekst.Groep_21_10.string);
    strcpy((char *)&tekst.Groep_22_14.string, (const char *)&tekst.Groep_22_10.string);
    strcpy((char *)&tekst.Groep_23_14.string, (const char *)&tekst.Groep_23_10.string);
    strcpy((char *)&tekst.Groep_24_14.string, (const char *)&tekst.Groep_24_10.string);
    strcpy((char *)&tekst.Groep_25_14.string, (const char *)&tekst.Groep_25_10.string);
    strcpy((char *)&tekst.Groep_26_14.string, (const char *)&tekst.Groep_26_10.string);
    strcpy((char *)&tekst.Groep_27_14.string, (const char *)&tekst.Groep_27_10.string);
    strcpy((char *)&tekst.Groep_28_14.string, (const char *)&tekst.Groep_28_10.string);
    strcpy((char *)&tekst.Groep_29_14.string, (const char *)&tekst.Groep_29_10.string);
    strcpy((char *)&tekst.Groep_30_14.string, (const char *)&tekst.Groep_30_10.string);
    strcpy((char *)&tekst.Groep_31_14.string, (const char *)&tekst.Groep_31_10.string);
    strcpy((char *)&tekst.Groep_32_14.string, (const char *)&tekst.Groep_32_10.string);
    opt_alg.taalkeuze = USER;
    EEP_taal_tekst_write_switch = 1;
  }
}

//*****************************************************************************
void PC_Tekst_Inst_Read_Algemeen(unsigned char taal, s_pc_com *pc_com)
{
// is gelijk aan commando PC_Save_Option() alleen commando is anders
unsigned long end_adres;

  pc_com->command = received_command;
  pc_com->command_timeout = COMMANDPCTIMEOUT;
  switch (taal)
  {
    default:
    case ENGELS: end_adres = (unsigned long)&tekst_inst_engels + sizeof(s_tekst_inst); break;
    case NEDERLANDS: end_adres = (unsigned long)&tekst_inst_nederlands + sizeof(s_tekst_inst); break;
    case DUITS: end_adres = (unsigned long)&tekst_inst_duits + sizeof(s_tekst_inst); break;
    case SPAANS: end_adres = (unsigned long)&tekst_inst_spaans + sizeof(s_tekst_inst); break;
    case USER: end_adres = (unsigned long)&tekst_inst + sizeof(s_tekst_inst); break;
  }
  switch (received_sub_command)
  {
    case SUBCOMMANDBEGIN:
      switch (taal)
      {
        default:
        case ENGELS: pc_com->old_ptr = pc_com->ptr = (unsigned char *)&tekst_inst_engels.Gekozen_Taal_14; break;
        case NEDERLANDS: pc_com->old_ptr = pc_com->ptr = (unsigned char *)&tekst_inst_nederlands.Gekozen_Taal_14; break;
        case DUITS: pc_com->old_ptr = pc_com->ptr = (unsigned char *)&tekst_inst_duits.Gekozen_Taal_14; break;
        case SPAANS: pc_com->old_ptr = pc_com->ptr = (unsigned char *)&tekst_inst_spaans.Gekozen_Taal_14; break;
        case USER: pc_com->old_ptr = pc_com->ptr = (unsigned char *)&tekst_inst.Gekozen_Taal_14; break;
      }
      pc_com->blok_cnt = 0;
      PC_Tekst_Read_Data_Algemeen(pc_com,end_adres);
      break;
    case SUBCOMMANDPREV:
      pc_com->ptr = pc_com->old_ptr;
      PC_Tekst_Read_Data_Algemeen(pc_com,end_adres);
      break;
    case SUBCOMMANDNEXT:
      if (received_blok_cnt == (unsigned char)(pc_com->blok_cnt+1))
      {
        pc_com->blok_cnt++;
        pc_com->old_ptr = pc_com->ptr;
      }
      else
        pc_com->ptr = pc_com->old_ptr;
      PC_Tekst_Read_Data_Algemeen(pc_com,end_adres);
      break;
    case SUBCOMMANDEND:
      PC_Send_Error_Code(received_adres, send_adres, NOCOMMAND, ERRORSUBCOMMANDEND);
      break;
  }
}

//-----------------------------------------------------------------------------
void PC_Tekst_Inst_Write_Algemeen(s_pc_com *pc_com)
{
  pc_com->command = received_command;
  pc_com->command_timeout = COMMANDPCTIMEOUT;
  switch (received_sub_command)
  {  
    case SUBCOMMANDBEGIN:
      tekst_inst = tekst_inst_engels;
      pc_com->old_ptr = pc_com->ptr = (unsigned char *)&tekst_inst.Gekozen_Taal_14;
      pc_com->blok_cnt = 1;
      PC_Tekst_Write_Data_Algemeen(pc_com,(unsigned long)&tekst_inst + sizeof(tekst_inst));
      break;
    case SUBCOMMANDPREV:
      pc_com->ptr = pc_com->old_ptr;
      PC_Tekst_Write_Data_Algemeen(pc_com,(unsigned long)&tekst_inst + sizeof(tekst_inst));
      break;
    case SUBCOMMANDNEXT:
      if (received_blok_cnt == pc_com->blok_cnt)
      {
        pc_com->blok_cnt++;
        pc_com->old_ptr = pc_com->ptr;
      }
      else
        pc_com->ptr = pc_com->old_ptr;
      PC_Tekst_Write_Data_Algemeen(pc_com,(unsigned long)&tekst_inst + sizeof(tekst_inst));
      break;
    case SUBCOMMANDEND:
      PC_Send_Error_Code(received_adres, send_adres, NOCOMMAND, ERRORSUBCOMMANDEND);
      return;
  }
  if (received_ETB_or_ETX == ETX_CH)
  {
    // write binngekomen data ook naar EEPROM
    // en zet taal op USER
    opt_alg.taalkeuze = USER;
    EEP_taal_tekst_inst_write_switch = 1;
  }
}

//*****************************************************************************
static void PC_Tekst_Versie_Read_Data_Algemeen(void)
{
unsigned char checksum;

  Put_Char(STX);
  checksum = STX;
  checksum = Send_Adres(send_adres/*PC_ADR*/, checksum); //  adres
  checksum = Send_Adres(received_adres, checksum); // adres
  checksum = Send_Command(received_command, checksum); //  command
  checksum = Send_Byte(received_sub_command, checksum); // sub command
  checksum = Send_Char(0, checksum); // block_nr
  checksum = Send_Char(15, checksum); // aantal data bytes
  
  checksum = Send_Int(rom.versie_tekst, checksum);    //  0
  checksum = Send_Int(rom.versie_tekst_inst, checksum); //  2
  checksum = Send_Char(PC_ENGELS,checksum); // 4 0=Engels
  checksum = Send_Char(PC_NEDERLANDS,checksum); // 5 1=Nederlands
  checksum = Send_Char(PC_DUITS,checksum); // 6 2=Duits
  checksum = Send_Char(PC_SPAANS,checksum); // 7 3=Spaans
  if (EEP_taal_eprom_aanwezig && EEP_taal_tekst_aanwezig && EEP_taal_tekst_inst_aanwezig)
    checksum = Send_Char(PC_USER,checksum); // 8 4=Usertaal
  else  
    checksum = Send_Char(PC_NO,checksum); // 8 4=Usertaal
/*
  if (option.taalkeuze == USER)
    checksum = Send_Char(PC_USER,checksum); // 8 4=Usertaal
  else   
    checksum = Send_Char(PC_NO,checksum); // 8 4=Usertaal
*/
  checksum = Send_Int(tekst.area_size,checksum); // 9 lengte tekst
  checksum = Send_Int(tekst_inst.area_size,checksum); // 13 lengte tekst_inst
  checksum = Send_Char(EEP_taal_eprom_aanwezig,checksum); // 17 taal eeprom aanwezig
  checksum = Send_Char(EEP_taal_tekst_aanwezig && EEP_taal_tekst_inst_aanwezig,checksum); // 18 taal in eeprom aanwezig
  Send_Char(checksum,0);
  Put_Char(ETX_CH);
  Put_Char(ETX);
  Put_Char(0);
}

void PC_Tekst_Versie_Read_Algemeen(s_pc_com *pc_com)
{
  pc_com->command = received_command;
  pc_com->command_timeout = COMMANDPCTIMEOUT;
  switch (received_sub_command)
  {
    case SUBCOMMANDBEGIN:
    case SUBCOMMANDPREV:
    case SUBCOMMANDNEXT:
      PC_Tekst_Versie_Read_Data_Algemeen();
      break;
    case SUBCOMMANDEND: 
      PC_Send_Error_Code(received_adres, send_adres, NOCOMMAND, ERRORSUBCOMMANDEND);
      break;
  }
}

//*****************************************************************************
static s_tekst_read PC_tekst_read;

static void PC_Tekst_Eeprom_Read_Data_Algemeen(s_pc_com *pc_com)
{
unsigned char loop;
unsigned char checksum;

  Put_Char(STX);
  checksum = STX;
  checksum = Send_Adres(send_adres/*PC_ADR*/, checksum); // adres
  checksum = Send_Adres(received_adres, checksum); // adres
  checksum = Send_Command(received_command, checksum); // command
  checksum = Send_Byte(received_sub_command, checksum); // sub command
  checksum = Send_Char(pc_com->blok_cnt, checksum); // block_nr

  checksum = Send_Char(PC_tekst_read.regel.max_char + 3, checksum); // lengte
  checksum = Send_Char(PC_tekst_read.regel.max_char, checksum);
  checksum = Send_Char(PC_tekst_read.regel.max_bits, checksum);
  checksum = Send_Char(PC_tekst_read.regel.font_type, checksum);
  for (loop = 0; loop < PC_tekst_read.regel.max_char; loop++)
    checksum = Send_Char(PC_tekst_read.regel.string[loop], checksum);
//  ASC1_Printf("UIT: %s\r",(char *)PC_tekst_read.regel.string);
  Send_Char(checksum,0);
  if (PC_tekst_read.size_tekst)
    Put_Char(ETB_CH);
  else
    Put_Char(ETX_CH);
  Put_Char(ETX);
  Put_Char(0);
}

//-----------------------------------------------------------------------------
void PC_Tekst_Eeprom_Read_Algemeen(unsigned char tekst, s_pc_com *pc_com)
// tekst = 0 tekst inlezen
// tekst = 1 tekst_inst inlezen
{
// is gelijk aan commando PC_Save_Option() alleen commando is anders
unsigned long end_adres;

  pc_com->command = received_command;
  pc_com->command_timeout = COMMANDPCTIMEOUT;
  end_adres = (unsigned long)&tekst + sizeof(s_tekst);
  switch (received_sub_command)
  {
    case SUBCOMMANDBEGIN:
      pc_com->blok_cnt = 0;
      if (EEP_Tekst_Read_First_String(tekst,&PC_tekst_read) == 0)
      {
        PC_Tekst_Eeprom_Read_Data_Algemeen(pc_com);
      }  
      break;
    case SUBCOMMANDPREV:
      pc_com->ptr = pc_com->old_ptr;
      PC_Tekst_Eeprom_Read_Data_Algemeen(pc_com);
      break;
    case SUBCOMMANDNEXT:
      if (received_blok_cnt == (unsigned char)(pc_com->blok_cnt+1))
      {
        EEP_Tekst_Read_Next_String(&PC_tekst_read);
        pc_com->blok_cnt++;
        pc_com->old_ptr = pc_com->ptr;
      }
      else
        pc_com->ptr = pc_com->old_ptr;
      PC_Tekst_Eeprom_Read_Data_Algemeen(pc_com);
      break;
    case SUBCOMMANDEND:
      PC_Send_Error_Code(received_adres, send_adres, NOCOMMAND, ERRORSUBCOMMANDEND);
      break;
  }
}

//*****************************************************************************
static void PC_Fan_Data_Read_Data(s_pc_com *pc_com)
{
unsigned char checksum;
int i, length;
int AantalIO = 0;

  Put_Char(STX);                                         // 1
  checksum = STX;
  checksum = Send_Adres(send_adres/*PC_ADR*/, checksum); // 3 - adres 
  checksum = Send_Adres(received_adres, checksum);       // 3 - adres
  checksum = Send_Command(received_command, checksum);   // 3 - command
  checksum = Send_Byte(received_sub_command, checksum);  // 1 - sub command
  checksum = Send_Char(pc_com->blok_cnt, checksum);      // 2 - block_nr

  // initialiseer en bepaal lengte
  switch (pc_com->blok_cnt)
  {
    case 0:
      AantalIO = 0;
      for (i = 0; i < MAX_VRIJGAVE; i++)
      {
        if (opt_app.VrijgaveVent.RS485Bus[i].board_type)
          AantalIO++;
      }
      length = ((opt_app.VrijgaveVent.Aantal * 4) + 2) + ((AantalIO * 4) + 2);
      break;
    default:
      length = 128;
      break;
  }

  checksum = Send_Char(length, checksum); // aantal data bytes

  // stuur data
  switch (pc_com->blok_cnt)
  {
    case 0: // Aantal en IO's
      checksum = Send_Int((int)opt_app.VrijgaveVent.Aantal, checksum); // 2
      for (i = 0; i < opt_app.VrijgaveVent.Aantal; i++)
      {
        checksum = Send_Char(opt_app.VrijgaveVent.DigInVrijgave[i].board_type, checksum); // 3
        checksum = Send_Char(opt_app.VrijgaveVent.DigInVrijgave[i].board_nr,   checksum); // 4
        checksum = Send_Char(DIGITAL_INPUT_ID,                                 checksum); // 5
        checksum = Send_Char(opt_app.VrijgaveVent.DigInVrijgave[i].IO_nr,      checksum); // 6
      }
      checksum = Send_Int((int)AantalIO, checksum); // 7
      for (i = 0; i < AantalIO; i++)
      {
        checksum = Send_Char(opt_app.VrijgaveVent.RS485Bus[i].board_type, checksum); //  9
        checksum = Send_Char(opt_app.VrijgaveVent.RS485Bus[i].board_nr,   checksum); // 11
        checksum = Send_Char(DIGITAL_INPUT_ID,                            checksum); // 13
        checksum = Send_Char(opt_app.VrijgaveVent.RS485Bus[i].IO_nr,      checksum); // 15
      }
      break;
    case 1: // Fan 1 t/m 64
      for (i = 0; i < 64; i++)
        checksum = Send_Int(opt_app.VrijgaveVent.Ventilator[i], checksum);
      break;
    case 2: // Fan 65 t/m 128
      for (i = 64; i < 128; i++)
        checksum = Send_Int(opt_app.VrijgaveVent.Ventilator[i], checksum);
      break;
    case 3: // Fan 129 t/m 192
      for (i = 128; i < 192; i++)
        checksum = Send_Int(opt_app.VrijgaveVent.Ventilator[i], checksum);
      break;
    case 4: // Fan 193 t/m 256
      for (i = 192; i < 256; i++)
        checksum = Send_Int(opt_app.VrijgaveVent.Ventilator[i], checksum);
      break;
  }

  Send_Char(checksum, 0);
  if (pc_com->blok_cnt == 4)
    Put_Char(ETX_CH);
  else
    Put_Char(ETB_CH);

  Put_Char(ETX);
  Put_Char(0);
}

static void PC_Fan_Data_Read(s_pc_com *pc_com)
{
  pc_com->command         = received_command;
  pc_com->command_timeout = COMMANDPCTIMEOUT;
  switch (received_sub_command)
  {
    case SUBCOMMANDBEGIN:
      pc_com->blok_cnt = 0;
      PC_Fan_Data_Read_Data(pc_com);
      break;
    case SUBCOMMANDPREV:
      PC_Fan_Data_Read_Data(pc_com);
      break;
    case SUBCOMMANDNEXT:
      if (received_blok_cnt == (unsigned char)(pc_com->blok_cnt+1))
      {
        pc_com->blok_cnt++;
      }
      else
      {
      }
      PC_Fan_Data_Read_Data(pc_com);
      break;
    case SUBCOMMANDEND: 
      PC_Send_Error_Code(received_adres, send_adres, NOCOMMAND, ERRORSUBCOMMANDEND);
      break;
  }
}

static void PC_Fan_Data_Write(s_pc_com *pc_com)
{
unsigned int AantalGroepen;
unsigned int AantalIO;
int i, index;

  pc_com->command = FAN_DATA_WRITE;
  pc_com->command_timeout = COMMANDPCTIMEOUT;
  switch (received_sub_command)
  {
    case SUBCOMMANDBEGIN:
      if (restore_data_busy != 0)
      {
        PC_Send_Error_Code(received_adres, send_adres, FAN_DATA_WRITE, ERRORRESTOREOPTIONSETPOINTBUSY);
      }
      else
      {
        restore_data_busy = TIME_OUT_RESTORE_DATA_BUSY;
        pc_com->old_ptr = pc_com->ptr = restore_data_buffer;
        pc_com->blok_cnt = 1;
        PC_Write_Data(pc_com);
      }  
      break;
    case SUBCOMMANDPREV:
      restore_data_busy = TIME_OUT_RESTORE_DATA_BUSY;
      pc_com->ptr = pc_com->old_ptr;
      PC_Write_Data(pc_com);
      break;
    case SUBCOMMANDNEXT:
      restore_data_busy = TIME_OUT_RESTORE_DATA_BUSY;
      if (received_blok_cnt == pc_com->blok_cnt)
      {
        pc_com->blok_cnt++;
        pc_com->old_ptr = pc_com->ptr;
      }
      else
        pc_com->ptr = pc_com->old_ptr;
      PC_Write_Data(pc_com);
      break;
    case SUBCOMMANDEND:
      restore_data_busy = 0;
      PC_Send_Error_Code(received_adres, send_adres, NOCOMMAND, ERRORSUBCOMMANDEND);
      return;
  }
  if (received_ETB_or_ETX == ETX_CH)
  {
    index = 0;
    AantalGroepen = ((unsigned int)restore_data_buffer[index] << 8) + restore_data_buffer[index + 1];
    if (AantalGroepen > MAX_VRIJGAVE)
      return;
    index = (AantalGroepen * 4) + 2;
    AantalIO = ((unsigned int)restore_data_buffer[index] << 8) + restore_data_buffer[index + 1];
    if (AantalIO > MAX_VRIJGAVE)
      return;
    opt_app.VrijgaveVent.Aantal = AantalGroepen;
    index = 2;
    for (i = 0; i < MAX_VRIJGAVE; i++)
    {
      if (i < opt_app.VrijgaveVent.Aantal)
      {
        opt_app.VrijgaveVent.DigInVrijgave[i].board_type = restore_data_buffer[index + 0];
        opt_app.VrijgaveVent.DigInVrijgave[i].board_nr   = restore_data_buffer[index + 1];
//      opt_app.VrijgaveVent.DigInVrijgave[i].IO_type    = restore_data_buffer[index + 2];
        opt_app.VrijgaveVent.DigInVrijgave[i].IO_nr      = restore_data_buffer[index + 3];
        opt_app.VrijgaveVent.DigInVrijgave[i].on_off     = i + 1;
        IOCreateObject(&opt_app.VrijgaveVent.DigInVrijgave[i], DIGITAL_INPUT_ID, &option_digital_input_vent);
        index += 4;
      }
      else
      {
        opt_app.VrijgaveVent.DigInVrijgave[i] = IO_empty;
      }
    }
    index += 2;
    for (i = 0; i < MAX_VRIJGAVE; i++)
    {
      if (i < AantalIO)
      {
        opt_app.VrijgaveVent.RS485Bus[i].board_type = restore_data_buffer[index + 0];
        opt_app.VrijgaveVent.RS485Bus[i].board_nr   = restore_data_buffer[index + 1];
//      opt_app.VrijgaveVent.RS485Bus[i].IO_type    = restore_data_buffer[index + 2];
        opt_app.VrijgaveVent.RS485Bus[i].IO_nr      = restore_data_buffer[index + 3];
        opt_app.VrijgaveVent.RS485Bus[i].on_off     = 1;
        IOCreateObject(&opt_app.VrijgaveVent.RS485Bus[i], RS485_BUS_ID, &option_RS485_bus_enabled);
        index += 4;
      }
      else
      {
        opt_app.VrijgaveVent.RS485Bus[i] = IO_empty;
      }
    }
    for (i = 0; i < MAX_DEVICE; i++)
    {
      opt_app.VrijgaveVent.Ventilator[i] = ((unsigned int)restore_data_buffer[index] << 8) + restore_data_buffer[index + 1];
      index += 2;
    }
    restore_data_busy = 0;
  }
}


//*****************************************************************************
static void PC_SD_Read_Data_Empty_Stop(s_pc_com *pc_com)
{
unsigned char *ptr = pc_com->ptr;
unsigned char checksum;

  Put_Char(STX);
  checksum = STX;
  checksum = Send_Adres(send_adres, checksum); // adres
  checksum = Send_Adres(received_adres, checksum); // adres
  checksum = Send_Command(received_command, checksum); // command
  checksum = Send_Byte(SUBCOMMANDEND/*received_sub_command*/, checksum); // sub command
  checksum = Send_Char(pc_com->blok_cnt, checksum); // block_nr
  checksum = Send_Char(0, checksum);
  Send_Char(checksum,0);
  Put_Char(ETX_CH);
  Put_Char(ETX);
  Put_Char(0);
}

#ifdef SD_CARD

static void PC_SD_Read_Data_Empty_Wait(s_pc_com *pc_com)
{
unsigned char *ptr = pc_com->ptr;
unsigned char checksum;

  Put_Char(STX);
  checksum = STX;
  checksum = Send_Adres(send_adres, checksum); // adres
  checksum = Send_Adres(received_adres, checksum); // adres
  checksum = Send_Command(received_command, checksum); // command
  checksum = Send_Byte(received_sub_command, checksum); // sub command
  checksum = Send_Char(pc_com->blok_cnt, checksum); // block_nr
  checksum = Send_Char(0, checksum);
  Send_Char(checksum,0);
  Put_Char(ETB_CH);
  Put_Char(ETX);
  Put_Char(0);
}

static void PC_SD_Get_Path_Name(char *path)
// als received lengte is 0 dan wordt path niet veranderd
{
  while (received_lengte)
  {
    *path = Receive_Char();
    path++;
    received_lengte--;
  } 
  *path = 0;
}

static unsigned char PC_SD_Tekst_Or_Hex_File(char *path)
// return 0 tekst file
// return 1 hex file
{
int loop;
int length;

  for (loop = 0; loop < MAX_FILES-1; loop++)
  {
    length = strlen((char *)log_file[loop].file_name_base);
    if ((length > 0) && strncmp((char *)log_file[loop].file_name, path, length) == 0)
    {
      return (log_file[loop].hex);
    }
  }
  return (TEKST_FILE_FORMAT);
}
//-----------------------------------------------------------------------------
static void PC_SD_Directory_Read_Data(s_pc_com *pc_com, unsigned char state) // Inlezen directoy
{
static int wait_cnt;
static unsigned char regel_ingelezen;
unsigned char checksum;
int cnt = 0;
unsigned char ch;
unsigned char endchar;
char path[50];
int loop;

  if ((sd_status & STA_NODISK) || (setp_alg.sd_card_status != 0))
  {
    PC_SD_Read_Data_Empty_Stop(pc_com);
    read_data_file_busy = 0;
    return;
  }
  switch (state)
  {
    case 0: // open file en lees gegevens van sd kaart
      PC_SD_Get_Path_Name(path);
      if (path[0] == 0)
        strcpy(path, "\\");
      SD_Directory_Read_Start(path);
      sd_directory_flag = SD_DIRECTORY_START;
      Put_Char(STX);
      checksum = STX;
      checksum = Send_Adres(send_adres, checksum); // adres
      checksum = Send_Adres(received_adres, checksum); // adres
      checksum = Send_Command(received_command, checksum); // command
      checksum = Send_Byte(received_sub_command, checksum); // sub command
      checksum = Send_Char(pc_com->blok_cnt, checksum); // block_nr
      checksum = Send_Char(val_hr_alg.sd_log.aantal_variabelen * 2 + 1 + 2 + 4 + 4 + 1, checksum);
      
      checksum = Send_Char(setp_alg.sd_card_log_on[LOG], checksum);
      checksum = Send_Int(val_hr_alg.sd_log.interval_in_seconden, checksum);
      if ((setp_alg.sd_card_log_on[LOG] == 0) || (val_hr_alg.sd_log.start_tijd == 0))
        checksum = Send_Long(time(0),checksum);
      else  
        checksum = Send_Long(val_hr_alg.sd_log.start_tijd, checksum);
      checksum = Send_Long(val_hr_alg.sd_log.stop_tijd, checksum);
      checksum = Send_Char(val_hr_alg.sd_log.aantal_variabelen, checksum);
      for (loop = 0; loop < val_hr_alg.sd_log.aantal_variabelen; loop++)
      {
        checksum = Send_Int(val_hr_alg.sd_log.code[loop], checksum);
      }
      Send_Char(checksum,0);
      Put_Char(ETB_CH);
      Put_Char(ETX);                                
      Put_Char(0);
      wait_cnt = 0;
      pc_com->index = 0; // index voor ingelezen data
      regel_ingelezen = 0; // wordt gebruikt om aan te geven dat 1 regel is ingelezen en in buffer;
      break;
    case 1: // start opnieuw inlezen regel als regel aanwezig is anders ga verder met inlezen huidige regel
      if (regel_ingelezen) // regel is ingelezen
      {
        pc_com->index = 0;
        regel_ingelezen = 0; // start met nieuwe regel inlezen
      }
    case 2: 
      while (regel_ingelezen == 0) // wacht totdat regel is ingelezen
      {
        if (sd_directory_buffer.cnt == 0)
        {
          if (((sd_directory_flag == SD_DIRECTORY_STOP) ||
              (sd_directory_flag == SD_DIRECTORY_READY)))
          {
            // error geen correct einde string, stuur einde over zonder data
            pc_com->index = 0;
            regel_ingelezen = 1;
          }
          else if (wait_cnt < 25)
          {
            wait_cnt++;
            PC_SD_Read_Data_Empty_Wait(pc_com); // stuur antwoord zonder data zodat orion tijd krijgt om file te openen en data in buffer gereed te zetten
          }
          else
          {
            // nog nakijken wat er nu ver moet gebeuren
            PC_Send_Error_Code(received_adres, send_adres, pc_com->command, ERRORSDTIMEOUT);
            read_data_file_busy = 0;
          }
          break;
        }
        else
        {
          ch = Buffer_Get_Byte(&sd_directory_buffer);
          if (ch == 0) // complete string ingelezen
            regel_ingelezen = 1; 
          else
          {
            pc_com->ptr_receive_buffer[pc_com->index] = ch;
            pc_com->index++;
          }
        }
      }
      if (regel_ingelezen) // ingelezen regel aanwezig
      {
        wait_cnt = 0;
        Put_Char(STX);
        checksum = STX;
        checksum = Send_Adres(send_adres, checksum); // adres
        checksum = Send_Adres(received_adres, checksum); // adres
        checksum = Send_Command(received_command, checksum); // command
        if ((sd_directory_buffer.cnt == 0) &&
            ((sd_directory_flag == SD_DIRECTORY_STOP) ||
             (sd_directory_flag == SD_DIRECTORY_READY)))
        {
          endchar = ETX_CH;
          checksum = Send_Byte(SUBCOMMANDEND, checksum); // sub command
          read_data_file_busy = 0;  // kan nog fout gaan als laatste bericht niet goed aankomt er tegelijk iemand anders start met opvragen file
        }
        else
        {
          endchar = ETB_CH;
          checksum = Send_Byte(received_sub_command, checksum); // sub command
        }
        checksum = Send_Char(pc_com->blok_cnt, checksum); // block_nr
        checksum = Send_Char(pc_com->index, checksum);
        while (cnt < pc_com->index)
        {
          checksum = Send_Char(pc_com->ptr_receive_buffer[cnt], checksum);
          cnt++;
        }
        Send_Char(checksum,0);
        Put_Char(endchar);
        Put_Char(ETX);                                
        Put_Char(0);
      }
      break;
  }
}

static void PC_SD_Directory_Read(s_pc_com *pc_com)
{
  pc_com->command = received_command;
  pc_com->command_timeout = COMMANDPCTIMEOUT;
  switch (received_sub_command)
  {
    case SUBCOMMANDBEGIN:
      if (read_data_file_busy != 0)
      {
        PC_Send_Error_Code(received_adres, send_adres, pc_com->command, ERRORRESTOREOPTIONSETPOINTBUSY);
      }
      else
      {
        read_data_file_busy = TIME_OUT_RESTORE_DATA_BUSY;
        pc_com->cnt = 0;
        pc_com->blok_cnt = 0;
        PC_SD_Directory_Read_Data(pc_com, 0);
      }  
      break;
    case SUBCOMMANDPREV:
      read_data_file_busy = TIME_OUT_RESTORE_DATA_BUSY;
      PC_SD_Directory_Read_Data(pc_com, 2);
      break;
    case SUBCOMMANDNEXT:
      read_data_file_busy = TIME_OUT_RESTORE_DATA_BUSY;
      if (received_blok_cnt == (unsigned char)(pc_com->blok_cnt+1))
      {
        pc_com->blok_cnt++;
        PC_SD_Directory_Read_Data(pc_com, 1);
      }
      else
        PC_SD_Directory_Read_Data(pc_com, 2);
      break;
    case SUBCOMMANDEND:
      PC_Send_Error_Code(received_adres, send_adres, NOCOMMAND, ERRORSUBCOMMANDEND);
      break;
  }
}

//-----------------------------------------------------------------------------
static void PC_SD_Tekst_File_Read_Data(s_pc_com *pc_com, unsigned char state) // Inlezen file strings
{
static int wait_cnt;
static unsigned char regel_ingelezen;
unsigned char checksum;
int cnt = 0;
unsigned char ch;
unsigned char endchar;

  if ((sd_status & STA_NODISK) || (setp_alg.sd_card_status != 0))
  {
    PC_SD_Read_Data_Empty_Stop(pc_com);
    //PC_Send_Error_Code(received_adres, send_adres, pc_com->command, ERRORNOSDCARD);
    read_data_file_busy = 0;
    return;
  }
  switch (state)
  {
    case 0: // open file en lees gegevens van sd kaart
      PC_SD_Read_Data_Empty_Wait(pc_com); // stuur antwoord zonder data zodat orion tijd krijgt om file te openen en data in buffer gereed te zetten
      wait_cnt = 0;
      pc_com->index = 0; // index voor ingelezen data
      regel_ingelezen = 0; // wordt gebruikt om aan te geven dat 1 regel is ingelezen en in buffer;
      break;
    case 1: // start opnieuw inlezen regel als regel aanwezig is anders ga verder met inlezen huidige regel
      if (regel_ingelezen) // regel is ingelezen
      {
        pc_com->index = 0;
        regel_ingelezen = 0; // start met nieuwe regel inlezen
      }
    case 2:
      while (regel_ingelezen == 0) // wacht totdat regel is ingelezen
      {
        if (log_file[LOG_READ].read.cnt == 0)
        {
          if ((log_file[LOG_READ].read_flag == SD_READ_STOP) ||
              (log_file[LOG_READ].read_flag == SD_READ_READY))
          {
            regel_ingelezen = 1;
          }
          else if (wait_cnt < 25)
          {
            wait_cnt++;
            PC_SD_Read_Data_Empty_Wait(pc_com); // stuur antwoord zonder data zodat orion tijd krijgt om file te openen en data in buffer gereed te zetten
          }
          else
          {
            // nog nakijken wat er nu ver moet gebeuren
            PC_Send_Error_Code(received_adres, send_adres, pc_com->command, ERRORSDTIMEOUT);
            read_data_file_busy = 0;
          }
          break;
        }
        else
        {
          ch = Buffer_Get_Byte(&log_file[LOG_READ].read);
          pc_com->ptr_receive_buffer[pc_com->index] = ch;
          pc_com->index++;
          if (pc_com->index >= 100)
            regel_ingelezen = 1;
        }
      }
      if (regel_ingelezen) // ingelezen regel aanwezig
      {
        state = 1;
        wait_cnt = 0;
        Put_Char(STX);
        checksum = STX;
        checksum = Send_Adres(send_adres, checksum); // adres
        checksum = Send_Adres(received_adres, checksum); // adres
        checksum = Send_Command(received_command, checksum); // command
        if ((log_file[LOG_READ].read.cnt == 0) &&
            ((log_file[LOG_READ].read_flag == SD_READ_STOP) ||
             (log_file[LOG_READ].read_flag == SD_READ_READY)))
        {
          endchar = ETX_CH;
          checksum = Send_Byte(SUBCOMMANDEND, checksum); // sub command
          read_data_file_busy = 0;  // kan nog fout gaan als laatste bericht niet goed aankomt er tegelijk iemand anders start met opvragen file
        }
        else
        {
          endchar = ETB_CH;
          checksum = Send_Byte(received_sub_command, checksum); // sub command
        }
        checksum = Send_Char(pc_com->blok_cnt, checksum); // block_nr
        checksum = Send_Char(pc_com->index, checksum);
        while (cnt < pc_com->index)
        {
          checksum = Send_Char(pc_com->ptr_receive_buffer[cnt],checksum);
          cnt++;
        }
        Send_Char(checksum,0);
        Put_Char(endchar);
        Put_Char(ETX);
        Put_Char(0);
      }
      break;
  }
}

//-----------------------------------------------------------------------------
static void PC_SD_Hex_File_Read_Data(s_pc_com *pc_com, unsigned char state) // Inlezen file alleen getallen char int long
{
static int wait_cnt;
static unsigned char sub_state;
unsigned char checksum;
int cnt = 0;
unsigned char endchar;
static unsigned char blok_cnt = 0;

  if ((sd_status & STA_NODISK) || (setp_alg.sd_card_status != 0))
  {
    PC_SD_Read_Data_Empty_Stop(pc_com);
    //PC_Send_Error_Code(received_adres, send_adres, pc_com->command, ERRORNOSDCARD);
    read_data_file_busy = 0;
    return;
  }
  switch (state)
  {
    case 0: // open file en lees gegevens van sd kaart
      PC_SD_Read_Data_Empty_Wait(pc_com); // stuur antwoord zonder data zodat orion tijd krijgt om file te openen en data in buffer gereed te zetten
      wait_cnt = 0;
      pc_com->index = 0; // index voor ingelezen data
      sub_state = 0; // wordt gebruikt om in state 2 de toestand van het inlezen van een regel bij te houden
      blok_cnt = 0;
      break;
    case 1: // start opnieuw inlezen regel als regel aanwezig is anders ga verder met inlezen huidige regel
      if (sub_state == 3) // regel is ingelezen
      {
        sub_state = 0;
        pc_com->index = 0;
      }
    case 2: // ga verder met inlezen
      switch (sub_state)
      {
        case 0: // controleer of aantal data bytes groter als blok_count + aantal_bytes regel + checksum + '\r\n' (8 bytes)
          if (log_file[LOG_READ].read.cnt < SD_OVERHEAD)
          {                           
            if ((log_file[LOG_READ].read_flag == SD_READ_STOP) ||
                (log_file[LOG_READ].read_flag == SD_READ_READY))
            {
              PC_Send_Error_Code(received_adres, send_adres, pc_com->command, ERRORSDFILES);
              read_data_file_busy = 0;
              sub_state = 0;
            }    
            else if (wait_cnt < 25)
            {
              wait_cnt++;
              PC_SD_Read_Data_Empty_Wait(pc_com); // stuur antwoord zonder data zodat orion tijd krijgt om file te openen en data in buffer gereed te zetten
            }
            else
            {
              PC_Send_Error_Code(received_adres, send_adres, pc_com->command, ERRORSDTIMEOUT);
              read_data_file_busy = 0;
              sub_state = 0;
            }
            return;
          }
          sub_state = 1;
        case 1: // controleer blok_count en lees in aantal characters dat ingelzen moet worden
          log_file[LOG_READ].read.checksum = 0;
          if (blok_cnt != Read_Buffer_Char_Checksum(&log_file[LOG_READ].read))
          {
            PC_Send_Error_Code(received_adres, send_adres, pc_com->command, ERRORSDFILES);
            read_data_file_busy = 0;
            sub_state = 0;
            return;
          }
          pc_com->index = Read_Buffer_Char_Checksum(&log_file[LOG_READ].read);
          if (pc_com->index > 100)
          {
            PC_Send_Error_Code(received_adres, send_adres, pc_com->command, ERRORSDFILES);
            read_data_file_busy = 0;
            sub_state = 0;
            return;
          }
          sub_state = 2;
        case 2: // wacht totdat alle characters ingelezen zijn
          if (log_file[LOG_READ].read.cnt < pc_com->index * 2 + 4) // aantal data bytes * 2 + checksum + \r \n
          {
            if ((log_file[LOG_READ].read_flag == SD_READ_STOP) ||
                (log_file[LOG_READ].read_flag == SD_READ_READY))
            {
              PC_Send_Error_Code(received_adres, send_adres, pc_com->command, ERRORSDFILES);
              read_data_file_busy = 0;
              sub_state = 0;
            }
            if (wait_cnt < 25)
            {
              wait_cnt++;
              PC_SD_Read_Data_Empty_Wait(pc_com); // stuur antwoord zonder data zodat orion tijd krijgt om file te openen en data in buffer gereed te zetten
            }
            else
            {
              PC_Send_Error_Code(received_adres, send_adres, pc_com->command, ERRORSDTIMEOUT);
              read_data_file_busy = 0;
              sub_state = 0;
            }
            return;
          }
          wait_cnt = 0;
          cnt = 0;
          while (cnt < pc_com->index)
          {
            pc_com->ptr_receive_buffer[cnt] = Read_Buffer_Char_Checksum(&log_file[LOG_READ].read);
            cnt++;
          }
          if ((log_file[LOG_READ].read.checksum != Read_Buffer_Char(&log_file[LOG_READ].read)) ||
              ('\r' != Read_Buffer_Byte(&log_file[LOG_READ].read)) ||
              ('\n' != Read_Buffer_Byte(&log_file[LOG_READ].read)))
          {
            PC_Send_Error_Code(received_adres, send_adres, pc_com->command, ERRORSDFILES);
            read_data_file_busy = 0;
            sub_state = 0;
            return;
          }
          blok_cnt++;
          sub_state = 3;
        case 3: // nieuw regel ingelezen
          Put_Char(STX);
          checksum = STX;
          checksum = Send_Adres(send_adres, checksum); // adres
          checksum = Send_Adres(received_adres, checksum); // adres
          checksum = Send_Command(received_command, checksum); // command
          if ((log_file[LOG_READ].read.cnt == 0) &&
              ((log_file[LOG_READ].read_flag == SD_READ_STOP) ||
               (log_file[LOG_READ].read_flag == SD_READ_READY)))
          {
            endchar = ETX_CH;
            checksum = Send_Byte(SUBCOMMANDEND, checksum); // sub command
            read_data_file_busy = 0;  // kan nog fout gaan als laatste bericht niet goed aankomt er tegelijk iemand anders start met opvragen file
          }
          else
          {
            endchar = ETB_CH;
            checksum = Send_Byte(received_sub_command, checksum); // sub command
          }
          checksum = Send_Char(pc_com->blok_cnt, checksum); // block_nr
          checksum = Send_Char(pc_com->index, checksum);
          cnt = 0;
          while (cnt < pc_com->index)
          {
            checksum = Send_Char(pc_com->ptr_receive_buffer[cnt], checksum); // block_nr
            cnt++;
          }
          Send_Char(checksum,0);
          Put_Char(endchar);
          Put_Char(ETX);
          Put_Char(0);
          break;
      }
  }
}

//-----------------------------------------------------------------------------
static unsigned char PC_SD_Log_Checksum_Error(unsigned char *regel, int regel_index)
{
int loop;
unsigned char checksum = 0;

  for (loop = 0; loop < regel_index - 4; loop++)
  {
    checksum ^= regel[loop];
  }
  return (checksum != ((Asc_To_Hex(regel[regel_index - 4]) << 4) + Asc_To_Hex(regel[regel_index - 3])));
}

static void PC_SD_Log_File_Read_Data(s_pc_com *pc_com, unsigned char state) // Inlezen file alleen getallen char int long
{
static int wait_cnt;
static unsigned char sub_state;
unsigned char checksum;
unsigned char endchar;
int loop;

  if ((sd_status & STA_NODISK) || (setp_alg.sd_card_status != 0))
  {
    PC_SD_Read_Data_Empty_Stop(pc_com);
    //PC_Send_Error_Code(received_adres, send_adres, pc_com->command, ERRORNOSDCARD);
    read_data_file_busy = 0;
    return;
  }
  switch (state)
  {
    case 0: // open file en lees gegevens van sd kaart
      PC_SD_Read_Data_Empty_Wait(pc_com); // stuur antwoord zonder data zodat orion tijd krijgt om file te openen en data in buffer gereed te zetten
      wait_cnt = 0;
      sub_state = 0; // wordt gebruikt om in state 2 de toestand van het inlezen van een regel bij te houden
      pc_com->index = 0;
      break;
    case 1: // start opnieuw inlezen regel als regel aanwezig is anders ga verder met inlezen huidige regel
      if (sub_state == 2) // regel is ingelezen
      {
        sub_state = 0;
        pc_com->index = 0;
      }
    case 2: // ga verder met inlezen
      switch (sub_state)
      {
        case 0: // controleer of aantal data bytes groter als blok_count + aantal_bytes regel + checksum + '\r\n' (8 bytes)
          while (log_file[LOG_READ].read.cnt && 
                 (sub_state == 0))
          {
            pc_com->ptr_receive_buffer[pc_com->index] = Read_Buffer_Byte(&log_file[LOG_READ].read);
            if (pc_com->ptr_receive_buffer[pc_com->index] == '\n')
              sub_state = 1;
            pc_com->index++;
          }
          if (sub_state == 0)
          {
            if ((log_file[LOG_READ].read_flag == SD_READ_STOP) ||
                (log_file[LOG_READ].read_flag == SD_READ_READY))
            {
              if (log_file[LOG_READ].read.cnt != 0)
              {
                PC_Send_Error_Code(received_adres, send_adres, pc_com->command, ERRORSDFILES);
              }
              else
              {
                PC_SD_Read_Data_Empty_Stop(pc_com);
              }  
              read_data_file_busy = 0;
              sub_state = 0;
            }    
            else if (wait_cnt < 25)
            {
              wait_cnt++;
              PC_SD_Read_Data_Empty_Wait(pc_com); // stuur antwoord zonder data zodat orion tijd krijgt om file te openen en data in buffer gereed te zetten
            }
            else
            {
              PC_Send_Error_Code(received_adres, send_adres, pc_com->command, ERRORSDTIMEOUT);
              read_data_file_busy = 0;
              sub_state = 0;
            }
            return;
          }
        case 1: // lees in aantal characters dat ingelezen moet worden
          wait_cnt = 0;
          log_file[LOG_READ].read.checksum = 0;
          if ((pc_com->index < 6) || // lengte + checksum + \r + \n
              (pc_com->ptr_receive_buffer[pc_com->index - 2] != '\r') ||
              (((((int)Asc_To_Hex(pc_com->ptr_receive_buffer[0]) << 4) + Asc_To_Hex(pc_com->ptr_receive_buffer[1])) * 2) != pc_com->index - 6) ||
              PC_SD_Log_Checksum_Error(pc_com->ptr_receive_buffer, pc_com->index)) // controleer lengte
          {
            PC_SD_Read_Data_Empty_Wait(pc_com); // gooi deze regel weg en lees volgende regel in
            sub_state = 0;
            pc_com->index = 0;
            break;
          }
          sub_state = 2;
        case 2: // nieuw regel ingelezen
          Put_Char(STX);
          checksum = STX;
          checksum = Send_Adres(send_adres, checksum); // adres
          checksum = Send_Adres(received_adres, checksum); // adres
          checksum = Send_Command(received_command, checksum); // command
          if ((log_file[LOG_READ].read.cnt == 0) &&
              ((log_file[LOG_READ].read_flag == SD_READ_STOP) ||
               (log_file[LOG_READ].read_flag == SD_READ_READY)))
          {
            endchar = ETX_CH;
            checksum = Send_Byte(SUBCOMMANDEND, checksum); // sub command
            read_data_file_busy = 0;  // kan nog fout gaan als laatste bericht niet goed aankomt er tegelijk iemand anders start met opvragen file
          }
          else
          {
            endchar = ETB_CH;
            checksum = Send_Byte(received_sub_command, checksum); // sub command
          }
          checksum = Send_Char(pc_com->blok_cnt, checksum); // block_nr
          for (loop = 0; loop < pc_com->index - 4; loop++)
          {
            Put_Char(pc_com->ptr_receive_buffer[loop]);
            checksum ^= pc_com->ptr_receive_buffer[loop];
          }
          Send_Char(checksum,0);
          Put_Char(endchar);
          Put_Char(ETX);
          Put_Char(0);
          break;
      }
  }
}

static void PC_SD_File_Read(s_pc_com *pc_com)
{
static unsigned char type = TEKST_FILE_FORMAT;
char path[50];

  pc_com->command = received_command;
  pc_com->command_timeout = COMMANDPCTIMEOUT;
  switch (received_sub_command)
  {
    case SUBCOMMANDBEGIN:
      if (read_data_file_busy != 0)
      {
        PC_Send_Error_Code(received_adres, send_adres, pc_com->command, ERRORRESTOREOPTIONSETPOINTBUSY);
      }
      else
      {
        read_data_file_busy = TIME_OUT_RESTORE_DATA_BUSY;
        pc_com->blok_cnt = 0;
        pc_com->cnt = 0;
        PC_SD_Get_Path_Name(path);
        if (path[0] == 0)
        {
          PC_Send_Error_Code(received_adres, send_adres, pc_com->command, ERRORSDFILES);
          return;
        }
        type = PC_SD_Tekst_Or_Hex_File(path);
        SD_File_Read_Start(path, type);
        switch (type)
        {
          case TEKST_FILE_FORMAT: PC_SD_Tekst_File_Read_Data(pc_com, 0); break;
          case HEX_FILE_FORMAT:   PC_SD_Hex_File_Read_Data(pc_com, 0); break;
          case LOG_FILE_FORMAT:   PC_SD_Log_File_Read_Data(pc_com, 0); break;
        }  
      }  
      break;
    case SUBCOMMANDPREV:
      read_data_file_busy = TIME_OUT_RESTORE_DATA_BUSY;
      switch (type)
      {
        case TEKST_FILE_FORMAT: PC_SD_Tekst_File_Read_Data(pc_com, 2); break;
        case HEX_FILE_FORMAT:   PC_SD_Hex_File_Read_Data(pc_com, 2); break;
        case LOG_FILE_FORMAT:   PC_SD_Log_File_Read_Data(pc_com, 2); break;
      }  
      break;
    case SUBCOMMANDNEXT:
      read_data_file_busy = TIME_OUT_RESTORE_DATA_BUSY;
      if (received_blok_cnt == (unsigned char)(pc_com->blok_cnt+1))
      {
        pc_com->blok_cnt++;
        switch (type)
        {
          case TEKST_FILE_FORMAT: PC_SD_Tekst_File_Read_Data(pc_com, 1); break;
          case HEX_FILE_FORMAT:   PC_SD_Hex_File_Read_Data(pc_com, 1); break;
          case LOG_FILE_FORMAT:   PC_SD_Log_File_Read_Data(pc_com, 1); break;
        }   
      }
      else
      {
        switch (type)
        {
          case TEKST_FILE_FORMAT: PC_SD_Tekst_File_Read_Data(pc_com, 2); break;
          case HEX_FILE_FORMAT:   PC_SD_Hex_File_Read_Data(pc_com, 2); break;
          case LOG_FILE_FORMAT:   PC_SD_Log_File_Read_Data(pc_com, 2); break;
        }  
      }    
      break;
    case SUBCOMMANDEND:
      pc_com->blok_cnt = received_blok_cnt;
      PC_SD_Read_Data_Empty_Stop(pc_com);
      read_data_file_busy = 0;
      break;
  }
}

//-----------------------------------------------------------------------------
static void PC_SD_Log_Config_Write_Data(void) // Inlezen file alleen getallen char int long
{
unsigned char checksum;
int loop;
unsigned char aantal_variabelen;
unsigned int interval;
time_t start_tijd;
time_t stop_tijd;
unsigned int index_array[MAX_LOG_VALUES];

  interval = Receive_Int();
  start_tijd = Receive_Long();
  stop_tijd = Receive_Long();
  aantal_variabelen = Receive_Char();
  if (received_lengte != aantal_variabelen * 2 + 2 + 4 + 4 + 1)
  {
    PC_Send_Error_Code(received_adres, send_adres, NOCOMMAND, ERRORLENGTE);
  }  
  else
  {
    if (aantal_variabelen > MAX_LOG_VALUES)
      aantal_variabelen = MAX_LOG_VALUES;
    for (loop = 0; loop < aantal_variabelen; loop++)
      index_array[loop] = Receive_Int();  
    SD_Log_Data_Aanmaken(aantal_variabelen, index_array, interval, start_tijd, stop_tijd);
    Put_Char(STX);
    checksum = STX;
    checksum = Send_Adres(send_adres, checksum); //  adres
    checksum = Send_Adres(received_adres, checksum); // adres
    checksum = Send_Command(received_command, checksum); //  command
    checksum = Send_Byte(SUBCOMMANDEND, checksum); // sub command
    checksum = Send_Char(0, checksum); // block_nr
    checksum = Send_Char(0, checksum); // aantal data bytes
    Send_Char(checksum,0);
    Put_Char(ETX_CH);
    Put_Char(ETX);
    Put_Char(0);
  }  
}

void PC_SD_Log_Config_Write(s_pc_com *pc_com)
{
  pc_com->command = received_command;
  pc_com->command_timeout = COMMANDPCTIMEOUT;
  switch (received_sub_command)
  {
    case SUBCOMMANDBEGIN:
    case SUBCOMMANDPREV:
    case SUBCOMMANDNEXT:
      PC_SD_Log_Config_Write_Data();
      break;
    case SUBCOMMANDEND: 
      PC_Send_Error_Code(received_adres, send_adres, NOCOMMAND, ERRORSUBCOMMANDEND);
      break;
  }    
}

//-----------------------------------------------------------------------------
void PC_SD_File_Delete_Data(s_pc_com *pc_com, int *wait_cnt)
{
  if (SD_File_Delete_Ready())
  {
    PC_SD_Read_Data_Empty_Stop(pc_com);
    read_data_file_busy = 0;
  }  
  else if (*wait_cnt < 25)
  {
    (*wait_cnt)++;
    PC_SD_Read_Data_Empty_Wait(pc_com); // stuur antwoord zonder data zodat orion tijd krijgt om file te openen en data in buffer gereed te zetten
  }
  else
  {
    PC_Send_Error_Code(received_adres, send_adres, pc_com->command, ERRORSDFILES);
    read_data_file_busy = 0;
  }  
}

void PC_SD_File_Delete(s_pc_com *pc_com)
{
static int wait_cnt;
char path[50];

  pc_com->command = received_command;
  pc_com->command_timeout = COMMANDPCTIMEOUT;
  switch (received_sub_command)
  {
    case SUBCOMMANDBEGIN:
      if (read_data_file_busy != 0)
      {
        PC_Send_Error_Code(received_adres, send_adres, pc_com->command, ERRORRESTOREOPTIONSETPOINTBUSY);
      }
      else
      {
        read_data_file_busy = TIME_OUT_RESTORE_DATA_BUSY;
        pc_com->blok_cnt = 0;
        pc_com->cnt = 0;
        PC_SD_Get_Path_Name(path);
        if (path[0] == 0)
        {
          PC_Send_Error_Code(received_adres, send_adres, pc_com->command, ERRORSDFILES);
          read_data_file_busy = 0;
          return;
        }
        wait_cnt = 0;
        SD_File_Delete_Start(path);
        PC_SD_Read_Data_Empty_Wait(pc_com); // stuur antwoord zonder data zodat orion tijd krijgt om file te openen en data in buffer gereed te zetten
      }  
      break;
    case SUBCOMMANDPREV:
      read_data_file_busy = TIME_OUT_RESTORE_DATA_BUSY;
      PC_SD_File_Delete_Data(pc_com, &wait_cnt);
      break;
    case SUBCOMMANDNEXT:
      read_data_file_busy = TIME_OUT_RESTORE_DATA_BUSY;
      if (received_blok_cnt == (unsigned char)(pc_com->blok_cnt+1))
      {
        pc_com->blok_cnt++;
        PC_SD_File_Delete_Data(pc_com, &wait_cnt);
      }
      else
      {
        PC_SD_File_Delete_Data(pc_com, &wait_cnt);
      }    
      break;
    case SUBCOMMANDEND:
      PC_Send_Error_Code(received_adres, send_adres, NOCOMMAND, ERRORSUBCOMMANDEND);
      break;
  }
}

//-----------------------------------------------------------------------------
void PC_SD_Format_Data(s_pc_com *pc_com, int *wait_cnt)
{
  if (SD_Format_Ready())
  {
    PC_SD_Read_Data_Empty_Stop(pc_com);
    read_data_file_busy = 0;
  }  
  else if (*wait_cnt < 2000)
  {
    (*wait_cnt)++;
    PC_SD_Read_Data_Empty_Wait(pc_com); // stuur antwoord zonder data zodat orion tijd krijgt om file te openen en data in buffer gereed te zetten
  }
  else
  {
    PC_Send_Error_Code(received_adres, send_adres, pc_com->command, ERRORSDFILES);
    read_data_file_busy = 0;
  }  
}

void PC_SD_Format(s_pc_com *pc_com)
{
static int wait_cnt;

  pc_com->command = received_command;
  pc_com->command_timeout = COMMANDPCTIMEOUT;
  switch (received_sub_command)
  {
    case SUBCOMMANDBEGIN:
      if (read_data_file_busy != 0)
      {
        PC_Send_Error_Code(received_adres, send_adres, pc_com->command, ERRORRESTOREOPTIONSETPOINTBUSY);
      }
      else
      {
        read_data_file_busy = TIME_OUT_RESTORE_DATA_BUSY;
        pc_com->blok_cnt = 0;
        pc_com->cnt = 0;
        wait_cnt = 0;
        PC_SD_Read_Data_Empty_Wait(pc_com); // stuur antwoord zonder data zodat orion tijd krijgt om file te openen en data in buffer gereed te zetten
        SD_Format_Start();
      }  
      break;
    case SUBCOMMANDPREV:
      read_data_file_busy = TIME_OUT_RESTORE_DATA_BUSY;
      PC_SD_Format_Data(pc_com, &wait_cnt);
      break;
    case SUBCOMMANDNEXT:
      read_data_file_busy = TIME_OUT_RESTORE_DATA_BUSY;
      if (received_blok_cnt == (unsigned char)(pc_com->blok_cnt+1))
      {
        pc_com->blok_cnt++;
        PC_SD_Format_Data(pc_com, &wait_cnt);
      }
      else
      {
        PC_SD_Format_Data(pc_com, &wait_cnt);
      }    
      break;
    case SUBCOMMANDEND:
      PC_Send_Error_Code(received_adres, send_adres, NOCOMMAND, ERRORSUBCOMMANDEND);
      break;
  }
}

void PC_SD_Write_Status(s_pc_com *pc_com)
{
unsigned char checksum;

  pc_com->command = received_command;
  pc_com->command_timeout = COMMANDPCTIMEOUT;
  switch (received_sub_command)
  {
    case SUBCOMMANDBEGIN:
    case SUBCOMMANDPREV:
    case SUBCOMMANDNEXT:
      if (setp_alg.sd_card_status != 3)
        setp_alg.sd_card_status = 0;
      Put_Char(STX);
      checksum = STX;
      checksum = Send_Adres(send_adres/*PC_ADR*/, checksum); //  adres
      checksum = Send_Adres(received_adres, checksum); // adres
      checksum = Send_Command(received_command, checksum); //  command
      checksum = Send_Byte(SUBCOMMANDEND, checksum); // sub command
      checksum = Send_Char(0, checksum); // block_nr
      checksum = Send_Char(0, checksum); // aantal data bytes
      Send_Char(checksum,0);
      Put_Char(ETX_CH);
      Put_Char(ETX);
      Put_Char(0);
      break;
    case SUBCOMMANDEND:
      PC_Send_Error_Code(received_adres, send_adres, NOCOMMAND, ERRORSUBCOMMANDEND);
      break;
  }
}

#endif // SD_CARD
//-----------------------------------------------------------------------------
static void PC_Mem_Dump_Read(s_pc_com *pc_com)
{
unsigned char *ptr;
unsigned char *end;
unsigned char endchar;
unsigned char checksum;
long StartAdres, EndAdres;

  pc_com->command = MEM_DUMP_READ;
  pc_com->command_timeout = COMMANDPCTIMEOUT;
  switch (received_sub_command)
  {
    case SUBCOMMANDBEGIN:
      StartAdres = Receive_Long();
      EndAdres   = Receive_Long();
      pc_com->ptr     = (unsigned char *)StartAdres;
      pc_com->old_ptr = (unsigned char *)EndAdres;
      pc_com->blok_cnt = 0;
      ptr = pc_com->ptr;
      end = pc_com->old_ptr;
      endchar = (ptr + MAXDATA - 4 < pc_com->old_ptr) ? ETB_CH : ETX_CH;
      if (endchar == ETB_CH)
        end = ptr + MAXDATA - 4;
      Put_Char(STX);
      checksum = STX;
      checksum = Send_Adres(send_adres/*PC_ADR*/, checksum); // adres
      checksum = Send_Adres(received_adres, checksum); // adres
      checksum = Send_Command(received_command, checksum); // command
      checksum = Send_Byte(received_sub_command, checksum); // sub command
      checksum = Send_Char(pc_com->blok_cnt, checksum); // block_nr
      checksum = Send_Char(end - ptr + 4, checksum);
      checksum = Send_Long(StartAdres, checksum);
      while (ptr < end)
      {
        checksum = Send_Char(*ptr, checksum);
        ptr++;
      }
      Send_Char(checksum,0);
      Put_Char(endchar);
      Put_Char(ETX);
      Put_Char(0);
      break;
    case SUBCOMMANDPREV:
      PC_Read_Data(pc_com, pc_com->old_ptr);
      break;
    case SUBCOMMANDNEXT:
      if (received_blok_cnt == (unsigned char)(pc_com->blok_cnt+1))
      {
        if (pc_com->ptr <= pc_com->old_ptr)
        {
          if (pc_com->blok_cnt == 0)
            pc_com->ptr += MAXDATA - 4;
          else
            pc_com->ptr += MAXDATA;
          pc_com->blok_cnt++;
        }
      }
      PC_Read_Data(pc_com, pc_com->old_ptr);
      break;
    case SUBCOMMANDEND:
      PC_Send_Error_Code(received_adres, send_adres, NOCOMMAND, ERRORSUBCOMMANDEND);
      break;
  }
}

//-----------------------------------------------------------------------------
static void PC_Mem_Dump_Write(s_pc_com *pc_com)
{
long StartAdres;

  pc_com->command = MEM_DUMP_WRITE;
  pc_com->command_timeout = COMMANDPCTIMEOUT;
  switch (received_sub_command)
  {
    case SUBCOMMANDBEGIN:
      StartAdres = Receive_Long();
      received_lengte -= 4;
      pc_com->old_ptr = pc_com->ptr = (unsigned char *)StartAdres;
      pc_com->blok_cnt = 1;
      PC_Write_Data(pc_com);
      break;
    case SUBCOMMANDPREV:
      pc_com->ptr = pc_com->old_ptr;
      PC_Write_Data(pc_com);
      break;
    case SUBCOMMANDNEXT:
      if (received_blok_cnt == pc_com->blok_cnt)
      {
        pc_com->blok_cnt++;
        pc_com->old_ptr = pc_com->ptr;
      }
      else
        pc_com->ptr = pc_com->old_ptr;
      PC_Write_Data(pc_com);
      break;
    case SUBCOMMANDEND:
      PC_Send_Error_Code(received_adres, send_adres, NOCOMMAND, ERRORSUBCOMMANDEND);
      return;
  }
}

//-----------------------------------------------------------------------------
static void PC_Val_Debug_Read(s_pc_com *pc_com)
{
long Adres;
unsigned char Type;
unsigned char checksum;

  pc_com->command = VAL_DEBUG_READ;
  pc_com->command_timeout = COMMANDPCTIMEOUT;
  switch (received_sub_command)
  {
    case SUBCOMMANDBEGIN:
    case SUBCOMMANDPREV:
    case SUBCOMMANDNEXT:
      Adres = Receive_Long();
      Type  = Receive_Char();
      Put_Char(STX);
      checksum = STX;
      checksum = Send_Adres(send_adres/*PC_ADR*/, checksum); // adres
      checksum = Send_Adres(received_adres, checksum); // adres
      checksum = Send_Command(received_command, checksum); // command
      checksum = Send_Byte(received_sub_command, checksum); // sub command
      checksum = Send_Char(0, checksum); // block_nr
      switch (Type)
      {
        case 0:
          checksum = Send_Char(5, checksum);
          checksum = Send_Long(Adres, checksum);
          checksum = Send_Char(*(unsigned char *)Adres, checksum);
          break;
        case 1:
          checksum = Send_Char(6, checksum);
          checksum = Send_Long(Adres, checksum);
          checksum = Send_Int(*(unsigned int *)Adres, checksum);
          break;
        case 2:
          checksum = Send_Char(8, checksum);
          checksum = Send_Long(Adres, checksum);
          checksum = Send_Long(*(unsigned long *)Adres, checksum);
          break;
      }
      Send_Char(checksum, 0);
      Put_Char(ETX_CH);
      Put_Char(ETX);
      Put_Char(0);
      break;
    case SUBCOMMANDEND:
      PC_Send_Error_Code(received_adres, send_adres, NOCOMMAND, ERRORSUBCOMMANDEND);
      break;
  }
}

//-----------------------------------------------------------------------------
static void PC_Val_Debug_Write_Data(void)
{
unsigned char checksum;
long Adres;

  if (received_lengte >= 5)
  {
    Adres = Receive_Long();
    switch (received_lengte - 4)
    {
      case 1: // unsigned char of char data
        *(unsigned char *)Adres = Receive_Char();
        Put_Char(STX);
        checksum = STX;
        checksum = Send_Adres(send_adres/*PC_ADR*/, checksum); //  adres
        checksum = Send_Adres(received_adres, checksum); // adres
        checksum = Send_Command(received_command, checksum); //  command
        checksum = Send_Byte(SUBCOMMANDEND, checksum); // sub command
        checksum = Send_Char(0, checksum); // block_nr
        checksum = Send_Char(5, checksum); // aantal data bytes
        checksum = Send_Long(Adres, checksum);
        checksum = Send_Char(*(unsigned char *)Adres, checksum);
        Send_Char(checksum,0);
        Put_Char(ETX_CH);
        Put_Char(ETX);
        Put_Char(0);
        break; 
      case 2: // unsigned int of int data
        *(unsigned int *)Adres = Receive_Int();
        Put_Char(STX);
        checksum = STX;
        checksum = Send_Adres(send_adres/*PC_ADR*/, checksum); //  adres
        checksum = Send_Adres(received_adres, checksum); // adres
        checksum = Send_Command(received_command, checksum); //  command
        checksum = Send_Byte(SUBCOMMANDEND, checksum); // sub command
        checksum = Send_Char(0, checksum); // block_nr
        checksum = Send_Char(6, checksum);
        checksum = Send_Long(Adres, checksum);
        checksum = Send_Int(*(unsigned int *)Adres, checksum);
        Send_Char(checksum,0);
        Put_Char(ETX_CH);
        Put_Char(ETX);
        Put_Char(0);
        break;
      case 4: // unsigned long of long data
        *(unsigned long *)Adres = Receive_Long();
        Put_Char(STX);
        checksum = STX;
        checksum = Send_Adres(send_adres/*PC_ADR*/, checksum); //  adres
        checksum = Send_Adres(received_adres, checksum); // adres
        checksum = Send_Command(received_command, checksum); //  command
        checksum = Send_Byte(SUBCOMMANDEND, checksum); // sub command
        checksum = Send_Char(0, checksum); // block_nr
        checksum = Send_Char(8, checksum);
        checksum = Send_Long(Adres, checksum);
        checksum = Send_Long(*(unsigned long *)Adres, checksum);
        Send_Char(checksum,0);
        Put_Char(ETX_CH);
        Put_Char(ETX);
        Put_Char(0);
        break; 
    }
  }
  else
    PC_Send_Error_Code(received_adres, send_adres, NOCOMMAND, ERRORLENGTE);
}

static void PC_Val_Debug_Write(s_pc_com *pc_com)
{
  pc_com->command = received_command;
  pc_com->command_timeout = COMMANDPCTIMEOUT;
  switch (received_sub_command)
  {
    case SUBCOMMANDBEGIN:
    case SUBCOMMANDPREV:
    case SUBCOMMANDNEXT:
      PC_Val_Debug_Write_Data();
      break;
    case SUBCOMMANDEND: 
      PC_Send_Error_Code(received_adres, send_adres, NOCOMMAND, ERRORSUBCOMMANDEND);
      break;
  }
}

//-----------------------------------------------------------------------------
static void PC_Status_Read_Data(s_pc_com *pc_com)
{
unsigned char checksum;
int index, length;
TMotor *pMotor;

  Put_Char(STX);
  checksum = STX;
  checksum = Send_Adres(send_adres/*PC_ADR*/, checksum); //  adres
  checksum = Send_Adres(received_adres, checksum); // adres
  checksum = Send_Command(received_command, checksum); //  command
  checksum = Send_Byte(received_sub_command, checksum); // sub command
  checksum = Send_Char(pc_com->blok_cnt, checksum); // block_nr

  // initialiseer en bepaal lengte
  index = pc_com->index;
  if (pc_com->ptr == NULL)
  {
    pMotor = Motorgroup[index].FirstMotor; // zet pMotor op eerste motor van de groep
    length = 3;                            // eerste 3 bytes voor de groep
  }
  else
  {
    pMotor = (TMotor *)pc_com->ptr;        // zet pMotor op eerst volgende motor van de groep
    length = 0;                            // lengte begint bij de motor
  }
  while (length <= MAXDATA)
  {
    if (pMotor != NULL) // motor data
    {
      length += 8;
      pMotor = pMotor->Next;
    }
    else // volgende groep
    {
      index++;
      if (index < opt_app.NumberMotorgroups)
      {
        length += 3;
        pMotor = Motorgroup[index].FirstMotor;
      }
      else
      {
        break;
      }
    }
  }
  checksum = Send_Char(length, checksum); // aantal data bytes

  // stuur data
  index = pc_com->index;
  if (pc_com->ptr == NULL)
  {
    pMotor = Motorgroup[index].FirstMotor; // zet pMotor op eerste motor van de groep
    length = 3;                            // eerste 3 bytes voor de groep
    checksum = Send_Int(val_hr_alg.Motorgroup[index].PositionPerc, checksum);
    checksum = Send_Char(opt_app.Motorgroup[index].NumberMotors, checksum);
  }
  else
  {
    pMotor = (TMotor *)pc_com->ptr;        // zet pMotor op eerst volgende motor van de groep
    length = 0;                            // lengte begint bij de motor
  }
  while (length <= MAXDATA)
  {
    if (pMotor != NULL) // motor data
    {
      length += 8;
      if (pMotor->Flags.ChangeTimer == 0)
      {
        pMotor->Flags.ChangeTimer = 1;
        TimerSet(&pMotor->Timer_5s, 5 * TIMER_1SEC);
        TimerSet(&pMotor->Timer_1s, TIMER_100MS);
      }
      checksum = Send_Char(pMotor->Number,                                 checksum);
      checksum = Send_Int (val_hr_alg.Motor[pMotor->Number].Position,      checksum);
      checksum = Send_Char(alarm_hr_alg.Motor[pMotor->Number].AlarmCode,   checksum);
      checksum = Send_Char(pMotor->RunningMode,                            checksum); // Stop = 0, Open = 1, Close = 2
      checksum = Send_Char(val_hr_alg.Motor[pMotor->Number].OperationMode, checksum); // Auto = 0, Manual = 1, Off = 2
      checksum = Send_Char(pMotor->Flags.LimitOpen,                        checksum);
      checksum = Send_Char(pMotor->Flags.LimitClose,                       checksum);
      pMotor = pMotor->Next;
    }
    else // volgende groep
    {
      index++;
      if (index < opt_app.NumberMotorgroups)
      {
        length += 3;
        pMotor = Motorgroup[index].FirstMotor;
        checksum = Send_Int(val_hr_alg.Motorgroup[index].PositionPerc, checksum);
        checksum = Send_Char(opt_app.Motorgroup[index].NumberMotors, checksum);
      }
      else
      {
        break;
      }
    }
  }

  pc_com->index = index;
  pc_com->ptr = (unsigned char *)pMotor;

  Send_Char(checksum, 0);
  if (index < opt_app.NumberMotorgroups) 
    Put_Char(ETB_CH);
  else
    Put_Char(ETX_CH);
  Put_Char(ETX);
  Put_Char(0);
}

static void PC_Status_Read(s_pc_com *pc_com)
{
  pc_com->command = received_command;
  pc_com->command_timeout = COMMANDPCTIMEOUT;
  switch (received_sub_command)
  {
    case SUBCOMMANDBEGIN:
      pc_com->index = pc_com->old_index = 0;
      pc_com->ptr = pc_com->old_ptr = 0;
      pc_com->blok_cnt = 0;
      PC_Status_Read_Data(pc_com);
      break;
    case SUBCOMMANDPREV:
      pc_com->index = pc_com->old_index;
      pc_com->ptr   = pc_com->old_ptr;
      PC_Status_Read_Data(pc_com);
      break;
    case SUBCOMMANDNEXT:
      if (received_blok_cnt == (unsigned char)(pc_com->blok_cnt+1))
      {
        pc_com->blok_cnt++;
        pc_com->old_index = pc_com->index;
        pc_com->old_ptr   = pc_com->ptr;
      }
      else
      {
        pc_com->index = pc_com->old_index;
        pc_com->ptr   = pc_com->old_ptr;
      }
      PC_Status_Read_Data(pc_com);
      break;
    case SUBCOMMANDEND: 
      PC_Send_Error_Code(received_adres, send_adres, NOCOMMAND, ERRORSUBCOMMANDEND);
      break;
  }
}

//-----------------------------------------------------------------------------


static void PC_XML_Read_Data(s_pc_com *pc_com)
{
unsigned char checksum;
int index, length;
int DeviceAddress = 0;
TDevice *pDevice;
TXML XML;

  Put_Char(STX);                                         // 1
  checksum = STX;
  checksum = Send_Adres(send_adres, checksum); // 3 - adres 
  checksum = Send_Adres(received_adres, checksum);       // 3 - adres
  checksum = Send_Command(received_command, checksum);   // 3 - command
  checksum = Send_Byte(received_sub_command, checksum);  // 1 - sub command
  checksum = Send_Char(pc_com->blok_cnt, checksum);      // 2 - block_nr

  // initialiseer en bepaal lengte
  length = 0;
  index  = pc_com->index;
  pDevice  = (TDevice *)pc_com->ptr;

  if (pc_com->blok_cnt == 0)
  {
    length += 4;
  }

  while (length <= MAXDATA_XML)
  {
    if (pDevice == NULL) // groep data
    {
      if (index >= opt_app.NumberMotorgroups)
        break;
      if (opt_app.Motorgroup[index].Type == TYPE_VENT)
      {
        if ((length + 11) > MAXDATA_XML)
          break;
        length += 11; // block size group
        pDevice = Motorgroup[index].FirstMotor;
      }
      else if (opt_app.Motorgroup[index].Type == TYPE_KLEP)
      {
        if ((length + 8) > MAXDATA_XML)
          break;
        length += 8; // block size group
        pDevice = Motorgroup[index].FirstMotor;
      }
      index++;
    }
    else // vent data
    {
      if (opt_app.Motorgroup[index - 1].Type == TYPE_VENT)
      {
        if ((length + 19) > MAXDATA_XML)
          break;
        length += 19;
        pDevice = pDevice->Next;
      }
      else if (opt_app.Motorgroup[index - 1].Type == TYPE_KLEP)
      {
        if ((length + 7) > MAXDATA_XML)
          break;
        length += 7;
        pDevice = pDevice->Next;
      }
	  else
	  {
        pDevice = NULL;
	  }
    }
  }
//  checksum = Send_Char(length, checksum); // aantal data bytes
  checksum = Send_Int(length, checksum); // aantal data bytes


  // stuur data
  length = 0;
  index  = pc_com->index;
  pDevice  = (TDevice *)pc_com->ptr;

  if (pc_com->blok_cnt == 0)
  {
    checksum = Send_Int(opt_alg.adres, checksum); // 2
    checksum = Send_Int(VERSIE_XML_PC, checksum); // 4
    length += 4;
  }

  while (length <= MAXDATA_XML)
  {
    if (pDevice == NULL) // groep data
    {
      if (index >= opt_app.NumberMotorgroups)
        break;
      if (opt_app.Motorgroup[index].Type == TYPE_VENT)
      {
        if ((length + 11) > MAXDATA_XML)
          break;

        XML_GetData_Vent_Group(index, &XML);
        checksum = Send_Char(TYPE_VENT,                              checksum); //  1 - groep nummer
        checksum = Send_Char(index + 1,                              checksum); //  1 - groep nummer
        checksum = Send_Char(opt_app.Motorgroup[index].NumberMotors, checksum); //  2 - aantal ventilatoren
        checksum = Send_Char(XML.Vent.Position,                      checksum); //  3 - gemiddelde positie in procenten
        checksum = Send_Int (XML.Vent.EnergyConsumption,             checksum); //  5 - Totale vermogen groep
        checksum = Send_Int (XML.Vent.Rpm,                           checksum); //  7 - gemiddelde snelheid in rpm
        checksum = Send_Char(XML.Vent.Status,                        checksum); //  8 - status
        checksum = Send_Char(XML.Vent.ErrorUrgent,                   checksum); //  9 - urgent alarm
        checksum = Send_Char(XML.Vent.ErrorNotUrgent,                checksum); // 10 - niet urgent alarm

        length += 11; // block size group
        pDevice = Motorgroup[index].FirstMotor;
      }
      else if (opt_app.Motorgroup[index].Type == TYPE_KLEP)
      {
        if ((length + 8) > MAXDATA_XML)
          break;

        XML_GetData_Klep_Group(index, &XML);
        checksum = Send_Char(TYPE_KLEP,                              checksum); //  1 - groep nummer
        checksum = Send_Char(index + 1,                              checksum); //  1 - groep nummer
        checksum = Send_Char(opt_app.Motorgroup[index].NumberMotors, checksum); //  2 - aantal ventilatoren
        checksum = Send_Char(XML.Klep.Position,                      checksum); //  3 - gemiddelde positie in procenten
        checksum = Send_Char(XML.Klep.Motorstatus,                   checksum); //  8 - status
        checksum = Send_Char(XML.Klep.Limitswitch,                   checksum); //  8 - status
        checksum = Send_Char(XML.Klep.ErrorLimitswitch,              checksum); //  9 - urgent alarm
        checksum = Send_Char(XML.Klep.Error,                         checksum); // 10 - niet urgent alarm

        length += 8; // block size group
        pDevice = Motorgroup[index].FirstMotor;
      }
      index++;
    }
    else // vent data
    {
      if (opt_app.Motorgroup[index - 1].Type == TYPE_VENT)
      {
        if ((length + 19) > MAXDATA_XML)
          break;

        DeviceAddress = XML_GetData_Vent(pDevice->Number, &XML);
        
        checksum = Send_Int (pDevice->Number + 1,                           checksum); //  2 - Fan number
        checksum = Send_Char(XML.Vent.Position,                             checksum); //  3 - Position value
        checksum = Send_Int (XML.Vent.EnergyConsumption,                    checksum); //  5 - Actual power consumption value
        checksum = Send_Int (XML.Vent.Rpm,                                  checksum); //  7 - Speed value
        checksum = Send_Char(mbDeviceGetTempMotor(DeviceAddress),           checksum); //  8 - Temp motor value
        checksum = Send_Char(mbDeviceGetTempElectronics(DeviceAddress),     checksum); //  9 - Temp electronics value
        checksum = Send_Char(mbDeviceGetTempPowerModule(DeviceAddress),     checksum); // 10 - Temp power module
        checksum = Send_Char(XML.Vent.Status,                               checksum); // 11 - Motor status value
        checksum = Send_Char(XML.Vent.ErrorUrgent,                          checksum); // 12 - Error urgent value
        checksum = Send_Char(XML.Vent.ErrorNotUrgent,                       checksum); // 13 - Error not urgent value
        checksum = Send_Int (mbDeviceGetErrorCode(DeviceAddress),           checksum); // 15 - Error code
        checksum = Send_Long(mbDeviceGetRunningHours(DeviceAddress) / 3600, checksum); // 19 - Running hours value

        length += 19;
        pDevice = pDevice->Next;
      }
      else if (opt_app.Motorgroup[index - 1].Type == TYPE_KLEP)
      {
        if ((length + 7) > MAXDATA_XML)
          break;

        DeviceAddress = XML_GetData_Klep(pDevice->Number, &XML);
        
        checksum = Send_Int (pDevice->Number + 1,                           checksum); //  2 - Fan number
        checksum = Send_Char(XML.Klep.Position,                             checksum); //  3 - Position value
        checksum = Send_Char(XML.Klep.Motorstatus,                          checksum); // 11 - Motor status value
        checksum = Send_Char(XML.Klep.Limitswitch,                          checksum); // 11 - Motor status value
        checksum = Send_Char(XML.Klep.ErrorLimitswitch,                     checksum); // 12 - Error urgent value
        checksum = Send_Char(XML.Klep.Error,                                checksum); // 13 - Error not urgent value

        length += 7;
        pDevice = pDevice->Next;
      }
	  else
	  {
        pDevice = NULL;
	  }
    }
  }

  pc_com->index = index;
  pc_com->ptr = (unsigned char *)pDevice;

  Send_Char(checksum, 0);
  if ((index >= opt_app.NumberMotorgroups) && (pDevice == NULL))
    Put_Char(ETX_CH);
  else
    Put_Char(ETB_CH);

  Put_Char(ETX);
  Put_Char(0);
}



static void PC_XML_Read(s_pc_com *pc_com)
{
  pc_com->command         = received_command;
  pc_com->command_timeout = COMMANDPCTIMEOUT;
  switch (received_sub_command)
  {
    case SUBCOMMANDBEGIN:
      pc_com->index    = pc_com->old_index = 0;
      pc_com->ptr      = pc_com->old_ptr   = 0;
      pc_com->blok_cnt = 0;
      PC_XML_Read_Data(pc_com);
      break;
    case SUBCOMMANDPREV:
      pc_com->index = pc_com->old_index;
      pc_com->ptr   = pc_com->old_ptr;
      PC_XML_Read_Data(pc_com);
      break;
    case SUBCOMMANDNEXT:
      if (received_blok_cnt == (unsigned char)(pc_com->blok_cnt+1))
      {
        pc_com->blok_cnt++;
        pc_com->old_index = pc_com->index;
        pc_com->old_ptr   = pc_com->ptr;
      }
      else
      {
        pc_com->index = pc_com->old_index;
        pc_com->ptr   = pc_com->old_ptr;
      }
      PC_XML_Read_Data(pc_com);
      break;
    case SUBCOMMANDEND: 
      PC_Send_Error_Code(received_adres, send_adres, NOCOMMAND, ERRORSUBCOMMANDEND);
      break;
  }
}

//-----------------------------------------------------------------------------
static void PC_Motor_Management_Read_Data(s_pc_com *pc_com)
{
unsigned char checksum;
unsigned char index, status;

  index  = Receive_Char();
  status = Receive_Char();

  Put_Char(STX);
  checksum = STX;
  checksum = Send_Adres(send_adres/*PC_ADR*/, checksum); //  adres
  checksum = Send_Adres(received_adres, checksum); // adres
  checksum = Send_Command(received_command, checksum); //  command
  checksum = Send_Byte(received_sub_command, checksum); // sub command
  checksum = Send_Char(pc_com->blok_cnt, checksum); // block_nr

  if (opt_app.Motor[index].Enabled)
  {
    if (status == 0)
      Motor[index].CtrlMotorManagement &= ~MOTOR_MANAGEMENT_SEND_PC_FLAG;

    Motor[index].CtrlMotorManagement |= MOTOR_MANAGEMENT_REQUEST_FLAG;
    if ((Motor[index].CtrlMotorManagement & MOTOR_MANAGEMENT_SEND_PC_FLAG) || (alarm_hr_alg.Motor[index].AlarmCode == acBoardCommunication))
    {
      checksum = Send_Char(12, checksum); // aantal data bytes
    
      checksum = Send_Char(index, checksum);
      checksum = Send_Char(1,     checksum);
      checksum = Send_Long(Motor[index].Management.Runtime,  checksum);
      checksum = Send_Long(Motor[index].Management.Switches, checksum);
      checksum = Send_Int (Motor[index].Management.Failures, checksum);
    }
    else
    {
      checksum = Send_Char(2, checksum); // aantal data bytes

      checksum = Send_Char(index, checksum);
      checksum = Send_Char(1,     checksum);
    }
  }
  else
  {
    checksum = Send_Char(0, checksum); // aantal data bytes
  }

  Send_Char(checksum, 0);
  Put_Char(ETX_CH);
  Put_Char(ETX);
  Put_Char(0);
}

static void PC_Motor_Management_Read(s_pc_com *pc_com)
{
  pc_com->command = received_command;
  pc_com->command_timeout = COMMANDPCTIMEOUT;
  switch (received_sub_command)
  {
    case SUBCOMMANDBEGIN:
      pc_com->blok_cnt = 0;
      PC_Motor_Management_Read_Data(pc_com);
      break;
    case SUBCOMMANDPREV:
      PC_Motor_Management_Read_Data(pc_com);
      break;
    case SUBCOMMANDNEXT:
      if (received_blok_cnt == (unsigned char)(pc_com->blok_cnt+1))
      {
        pc_com->blok_cnt++;
      }
      PC_Motor_Management_Read_Data(pc_com);
      break;
    case SUBCOMMANDEND: 
      PC_Send_Error_Code(received_adres, send_adres, NOCOMMAND, ERRORSUBCOMMANDEND);
      break;
  }
}

//-----------------------------------------------------------------------------
static void PC_Motorgroup_Read_Data(s_pc_com *pc_com)
{
unsigned char checksum;
unsigned char index;

  index = Receive_Char();

  Put_Char(STX);
  checksum = STX;
  checksum = Send_Adres(send_adres/*PC_ADR*/, checksum); //  adres
  checksum = Send_Adres(received_adres, checksum); // adres
  checksum = Send_Command(received_command, checksum); //  command
  checksum = Send_Byte(received_sub_command, checksum); // sub command
  checksum = Send_Char(pc_com->blok_cnt, checksum); // block_nr
 
  if (opt_alg.hoogendoorn_enabled && (index < MAX_GROUP))
  {
    int i;
    unsigned char emcy = MotorgroupGetEmcyBits(&Motorgroup[index]);

    checksum = Send_Char(8, checksum); // aantal bytes data
    checksum = Send_Char(index, checksum);
    checksum = Send_Int(Motorgroup[index].PositionAvg, checksum);
	for (i = 0; i < 5; i++)
	{
	  checksum = Send_Char(emcy & 0x01, checksum);
	  emcy >>= 1;
	}
  }
  else
  {
    checksum = Send_Char(0, checksum); // aantal bytes data
  }

  Send_Char(checksum, 0);
  Put_Char(ETX_CH);
  Put_Char(ETX);
  Put_Char(0);
}

static void PC_Motorgroup_Read(s_pc_com *pc_com)
{
  pc_com->command = received_command;
  pc_com->command_timeout = COMMANDPCTIMEOUT;
  switch (received_sub_command)
  {
    case SUBCOMMANDBEGIN:
    case SUBCOMMANDPREV:
    case SUBCOMMANDNEXT:
      pc_com->blok_cnt = 0;
      PC_Motorgroup_Read_Data(pc_com);
      break;
    case SUBCOMMANDEND: 
      PC_Send_Error_Code(received_adres, send_adres, NOCOMMAND, ERRORSUBCOMMANDEND);
      break;
  }
}

static void PC_Motorgroup_Write(s_pc_com *pc_com)
{
unsigned char checksum;
unsigned char index;
int position;

  pc_com->command = MOTORGROUP_WRITE;
  pc_com->command_timeout = COMMANDPCTIMEOUT;
  switch (received_sub_command)
  {
    case SUBCOMMANDBEGIN:
    case SUBCOMMANDPREV:
    case SUBCOMMANDNEXT:
      index = Receive_Char();
      position = Receive_Int();
	  if ((index < MAX_GROUP) && (position >= 0) && (position <= 1000))
           Motorgroup[index].Setpoint = position;
      Put_Char(STX);
      checksum = STX;
      checksum = Send_Adres(send_adres/*PC_ADR*/, checksum); //  adres
      checksum = Send_Adres(received_adres/*BLACKBOX_ADR*/, checksum); // adres
      checksum = Send_Command(MOTORGROUP_WRITE, checksum); //  command
      checksum = Send_Byte(SUBCOMMANDEND, checksum); // sub command
      checksum = Send_Char(0, checksum); // block_nr
      checksum = Send_Char(0, checksum); // aantal data bytes
      Send_Char(checksum,0);
      Put_Char(ETX_CH);
      Put_Char(ETX);
      Put_Char(0);
      break;
    case SUBCOMMANDEND:
      PC_Send_Error_Code(received_adres, send_adres, NOCOMMAND, ERRORSUBCOMMANDEND);
      break;
  }
}

//*****************************************************************************
//** EINDE ALGEMEEN COMMANDOS
//*****************************************************************************

//*****************************************************************************
//** AFDELING COMMANDOS
//*****************************************************************************

//-----------------------------------------------------------------------------

//*****************************************************************************
//** EINDE AFDELING COMMANDOS
//*****************************************************************************

//*****************************************************************************
//** FWS COMMANDOS
//*****************************************************************************

//*****************************************************************************
//** EINDE FWS COMMANDOS
//*****************************************************************************

void Verwerk_Main_Group_Changed(s_pc_com *pc_com, char *answer_buffer) // wordt gebruikt in C__RS232 om veranderde maingroup automatisch over te zenden
{
  answer_ptr = answer_buffer;
  *answer_ptr = 0;
  commando_voor = COM_VOOR_ALG;
  received_adres = opt_alg.adres;
  send_adres = PC_0_ADR; // JP toegevoegd voor CAN BACKBONE V2
  received_command = MAINGROUPCHANGED;
  received_sub_command = SUBCOMMANDBEGIN;
  received_blok_cnt = 0;
  received_lengte = 0;
  Verwerk_Data(pc_com);
}

//*****************************************************************************
void Copy_LCD_Screen_To_PC_Screen(s_pc_com *pc_com, int first_line)
{
int loop;
unsigned char cnt_0x00 = 0;
unsigned char cnt_0xff = 0;
unsigned char ch;
unsigned char *ptr_source = (unsigned char *)&lcd_pc_screen[first_line][0];
unsigned char *ptr_destination = pc_com->ptr_receive_buffer;

  for (loop = first_line * 30; loop < 128 * 30; loop++)
  {
    ch = *ptr_source++;
    switch (ch)
    {
      case 0x00:
        if (cnt_0xff)
        {
          *ptr_destination++ = 0xff;
          *ptr_destination++ = cnt_0xff;
          cnt_0xff = 0;
        }
        cnt_0x00++;
        if (cnt_0x00 > 250)
        {
          *ptr_destination++ = 0x00;
          *ptr_destination++ = cnt_0x00;
          cnt_0x00 = 0;
        }
        break;
      case 0xff:
        if (cnt_0x00)
        {
          *ptr_destination++ = 0x00;
          *ptr_destination++ = cnt_0x00;
          cnt_0x00 = 0;
        }
        cnt_0xff++;
        if (cnt_0xff > 250)
        {
          *ptr_destination++ = 0xff;
          *ptr_destination++ = cnt_0xff;
          cnt_0xff = 0;
        }
        break;
      default:
        if (cnt_0x00)
        {
          *ptr_destination++ = 0x00;
          *ptr_destination++ = cnt_0x00;
          cnt_0x00 = 0;
        }
        if (cnt_0xff)
        {
          *ptr_destination++ = 0xff;
          *ptr_destination++ = cnt_0xff;
          cnt_0xff = 0;
        }
        *ptr_destination++ = ch;
        break;
    }
  } 
  if (cnt_0x00)
  {
    *ptr_destination++ = 0x00;
    *ptr_destination++ = cnt_0x00;
  }
  if (cnt_0xff)
  {
    *ptr_destination++ = 0xff;
    *ptr_destination++ = cnt_0xff;
  }
  pc_com->old_ptr = ptr_destination;
}

static void PC_Capture_Screen_Data(s_pc_com *pc_com)
{
unsigned char *end = pc_com->old_ptr;
unsigned char *ptr = pc_com->ptr;
unsigned char endchar = (ptr + MAXDATA_CAPTURE < end) ? ETB_CH : ETX_CH;
unsigned char checksum;

  if (endchar == ETB_CH)
    end = ptr + MAXDATA_CAPTURE;
  Put_Char(STX);
  checksum = STX;
  checksum = Send_Adres(send_adres/*PC_ADR*/, checksum); // adres
  checksum = Send_Adres(received_adres, checksum); // adres
  checksum = Send_Command(received_command, checksum); // command
  checksum = Send_Byte(received_sub_command, checksum); // sub command
  checksum = Send_Char(pc_com->blok_cnt, checksum); // block_nr
  checksum = Send_Char(end - ptr, checksum);
  while (ptr < end)
  {
    checksum = Send_Char(*ptr, checksum);
    ptr++;
  }
  Send_Char(checksum,0);
  Put_Char(endchar);
  Put_Char(ETX);
  Put_Char(0);
}

static void PC_Capture_Screen(s_pc_com *pc_com)
{
  pc_com->command = CAPTURESCREEN;
  pc_com->command_timeout = COMMANDPCTIMEOUT;
  switch (received_sub_command)
  {
    case SUBCOMMANDBEGIN:
      Copy_LCD_Screen_To_PC_Screen(pc_com, 0);
      pc_com->ptr = pc_com->ptr_receive_buffer;
      pc_com->blok_cnt = 0;
      PC_Capture_Screen_Data(pc_com);
      break;
    case SUBCOMMANDPREV:
      PC_Capture_Screen_Data(pc_com);
      break;
    case SUBCOMMANDNEXT:
      if (received_blok_cnt == (unsigned char)(pc_com->blok_cnt+1))
      {
        if (pc_com->ptr <= pc_com->old_ptr)
        {
          pc_com->ptr += MAXDATA_CAPTURE;
          pc_com->blok_cnt++;
        }
      }
      PC_Capture_Screen_Data(pc_com);
      break;
    case SUBCOMMANDEND:
      PC_Send_Error_Code(received_adres, send_adres, NOCOMMAND, ERRORSUBCOMMANDEND);
      break;
  }
}

static void PC_Capture_No_Tab_Screen(s_pc_com *pc_com)
{
  pc_com->command = CAPTURESCREENNOTAB;
  pc_com->command_timeout = COMMANDPCTIMEOUT;
  switch (received_sub_command)
  {
    case SUBCOMMANDBEGIN:
      Copy_LCD_Screen_To_PC_Screen(pc_com, 20);
      pc_com->ptr = pc_com->ptr_receive_buffer;
      pc_com->blok_cnt = 0;
      PC_Capture_Screen_Data(pc_com);
      break;
    case SUBCOMMANDPREV:
      PC_Capture_Screen_Data(pc_com);
      break;
    case SUBCOMMANDNEXT:
      if (received_blok_cnt == (unsigned char)(pc_com->blok_cnt+1))
      {
        if (pc_com->ptr <= pc_com->old_ptr)
        {
          pc_com->ptr += MAXDATA_CAPTURE;
          pc_com->blok_cnt++;
        }
      }
      PC_Capture_Screen_Data(pc_com);
      break;
    case SUBCOMMANDEND:
      PC_Send_Error_Code(received_adres, send_adres, NOCOMMAND, ERRORSUBCOMMANDEND);
      break;
  }
}

//*****************************************************************************
// JP start 20-09-07
// LETOP wijziging.
// Slechts een screen catcher (PDA) werkt met de vollen snelheid
// indien meerder aangesloten wordt altijd het gehele scherm overgezonden
// In orion pluim en orion fws wordt nog met 3 schermen gewerkt en kan
// dus op alle 3 de kanalen snel gecommuniceerd worden
unsigned char lcd_pc_screen_old[128*30]; // JP 20-09-07 1 scherm voor rs232 en 2 voor compoorten smartlink
static s_pc_com *capture_pc_com_old = 0;

static void PC_Capture_Screen_Data_Fast(s_pc_com *pc_com)
{
unsigned char *end = pc_com->old_ptr;
unsigned char *ptr = pc_com->ptr;
unsigned char endchar = (ptr + MAXDATA_CAPTURE_FAST < end) ? ETB_CH : ETX_CH;
unsigned char checksum;

  if (endchar == ETB_CH)
    end = ptr + MAXDATA_CAPTURE_FAST;
  Put_Char(STX);
  checksum = STX;
  checksum = Send_Adres(send_adres/*PC_ADR*/, checksum); // adres
  checksum = Send_Adres(received_adres, checksum); // adres
  checksum = Send_Command(received_command, checksum); // command
  checksum = Send_Byte(received_sub_command, checksum); // sub command
  checksum = Send_Char(pc_com->blok_cnt, checksum); // block_nr
  checksum = Send_Char(end - ptr, checksum);
  while (ptr < end)
  {
    checksum = Send_Char(*ptr, checksum);
    ptr++;
  }
  Send_Char(checksum,0);
  Put_Char(endchar);
  Put_Char(ETX);
  Put_Char(0);
}

static long Lengte_Screen_Norm(void)
{
int loop;
int lengte = 0;
unsigned int cnt_0x00 = 0;
unsigned int cnt_0xff = 0;
unsigned char ch;
unsigned char *ptr_source = (unsigned char *)&lcd_pc_screen[0][0];

  for (loop = 0; loop < 128 * 30; loop++)
  {
    ch = *ptr_source++;
    switch (ch)
    {
      case 0x00:
        if (cnt_0xff)
        {
          cnt_0xff = 0;
          lengte += 2;
        }
        cnt_0x00++;
        if (cnt_0x00 > 250)
        {
          cnt_0x00 = 0;
          lengte += 2;
        }
        break;
      case 0xff:
        if (cnt_0x00)
        {
          cnt_0x00 = 0;
          lengte += 2;
        }
        cnt_0xff++;
        if (cnt_0xff > 250)
        {
          cnt_0xff = 0;
          lengte += 2;
        }
        break;
      default:
        if (cnt_0x00)
        {
          cnt_0x00 = 0;
          lengte += 2;
        }
        if (cnt_0xff)
        {
          cnt_0xff = 0;
          lengte += 2;
        }
        lengte++;
        break;
    }
  } 
  if (cnt_0x00)
  {
    lengte += 2;
  }
  if (cnt_0xff)
  {
    lengte += 2;
  }
  return (lengte);
}

static int Lengte_Screen_Diff(unsigned char *ptr_old)
{
int loop;
int lengte = 0;
unsigned int cnt_equal = 0;
unsigned char *ptr_source = (unsigned char *)&lcd_pc_screen[0][0];
int max = Lengte_Screen_Norm();

  for (loop = 0; loop < 128 * 30; loop++)
  {
    if (*ptr_old == *ptr_source)
    {
      cnt_equal++;
      if (cnt_equal > 250)
      {
        lengte += 2;
        cnt_equal = 0;
      }
    }
    else
    {
      if (cnt_equal)
      {
        lengte += 2;
        cnt_equal = 0;
      }
      lengte++;
      if (*ptr_source == 0) // extra 0 verzenden om te detecteren dat verschil byte een 0 is
      {
        lengte++;
      }
    }
    ptr_old++;
    ptr_source++;
    if (lengte > max)
      return (0);
  } 
  if (cnt_equal)
  {
    lengte += 2;
  }
  if (lengte > max)
    return (0);
  else
    return (1);  
//  return (lengte);
}

static void Copy_LCD_Screen_To_PC_Screen_Diff(s_pc_com *pc_com, unsigned char *ptr_old)
{
int loop;
unsigned int cnt_equal = 0;
unsigned char *ptr_source = (unsigned char *)&lcd_pc_screen[0][0];
unsigned char *ptr_destination = pc_com->ptr_receive_buffer;

  *ptr_destination++ = 1;
  for (loop = 0; loop < 128 * 30; loop++)
  {
    if (*ptr_old == *ptr_source)
    {
      cnt_equal++;
      if (cnt_equal > 250)
      {
        *ptr_destination++ = 0x00;
        *ptr_destination++ = cnt_equal;
        cnt_equal = 0;
      }
    }
    else
    {
      if (cnt_equal != 0)
      {
        *ptr_destination++ = 0x00;
        *ptr_destination++ = cnt_equal;
        cnt_equal = 0;
      }
      *ptr_destination++ = *ptr_source;
      if (*ptr_source == 0) // extra 0 verzenden om te detecteren dat verschil byte een 0 is
      {
        *ptr_destination++ = 0;
      }
    }
    *ptr_old = *ptr_source; // JP 20-09-07
    ptr_old++;
    ptr_source++;
  } 
  if (cnt_equal)
  {
    *ptr_destination++ = 0x00;
    *ptr_destination++ = cnt_equal;
  }
  pc_com->old_ptr = ptr_destination;
}

void Copy_LCD_Screen_To_PC_Screen_Norm(s_pc_com *pc_com, unsigned char *ptr_old)
{
int loop;
unsigned int cnt_0x00 = 0;
unsigned int cnt_0xff = 0;
unsigned char ch;
unsigned char *ptr_source = (unsigned char *)&lcd_pc_screen[0][0];
unsigned char *ptr_destination = pc_com->ptr_receive_buffer;

  *ptr_destination++ = 0;
  for (loop = 0; loop < 128 * 30; loop++)
  {
    ch = *ptr_source;
    switch (ch)
    {
      case 0x00:
        if (cnt_0xff)
        {
          *ptr_destination++ = 0xff;
          *ptr_destination++ = cnt_0xff;
          cnt_0xff = 0;
        }
        cnt_0x00++;
        if (cnt_0x00 > 250)
        {
          *ptr_destination++ = 0x00;
          *ptr_destination++ = cnt_0x00;
          cnt_0x00 = 0;
        }
        break;
      case 0xff:
        if (cnt_0x00)
        {
          *ptr_destination++ = 0x00;
          *ptr_destination++ = cnt_0x00;
          cnt_0x00 = 0;
        }
        cnt_0xff++;
        if (cnt_0xff > 250)
        {
          *ptr_destination++ = 0xff;
          *ptr_destination++ = cnt_0xff;
          cnt_0xff = 0;
        }
        break;
      default:
        if (cnt_0x00)
        {
          *ptr_destination++ = 0x00;
          *ptr_destination++ = cnt_0x00;
          cnt_0x00 = 0;
        }
        if (cnt_0xff)
        {
          *ptr_destination++ = 0xff;
          *ptr_destination++ = cnt_0xff;
          cnt_0xff = 0;
        }
        *ptr_destination++ = ch;
        break;
    }
    *ptr_old = *ptr_source; // JP 20-09-07
    ptr_old++;
    ptr_source++;
  } 
  if (cnt_0x00)
  {
    *ptr_destination++ = 0x00;
    *ptr_destination++ = cnt_0x00;
  }
  if (cnt_0xff)
  {
    *ptr_destination++ = 0xff;
    *ptr_destination++ = cnt_0xff;
  }
  pc_com->old_ptr = ptr_destination;
}

void PC_Capture_Screen_Fast(s_pc_com *pc_com)
{
unsigned char *ptr_old; // JP 20-09-07

  pc_com->command = CAPTURESCREENFAST;
  pc_com->command_timeout = COMMANDPCTIMEOUT;
  switch (received_sub_command)
  {
    case SUBCOMMANDBEGIN:
      if (received_lengte == 1)
      {
        ptr_old = lcd_pc_screen_old;
        if ((Receive_Char() == 1) || 
            ((unsigned long)pc_com != (unsigned long)capture_pc_com_old) || 
            (Lengte_Screen_Diff(ptr_old) == 0))
          Copy_LCD_Screen_To_PC_Screen_Norm(pc_com, ptr_old);
        else
          Copy_LCD_Screen_To_PC_Screen_Diff(pc_com, ptr_old);
        capture_pc_com_old = pc_com;  
        pc_com->ptr = pc_com->ptr_receive_buffer;
        pc_com->blok_cnt = 0;
        PC_Capture_Screen_Data_Fast(pc_com);
      }
      else
        PC_Send_Error_Code(received_adres, send_adres, NOCOMMAND, ERRORLENGTE);
      break;
    case SUBCOMMANDPREV:
      PC_Capture_Screen_Data_Fast(pc_com);
      break;
    case SUBCOMMANDNEXT:
      if (received_blok_cnt == (unsigned char)(pc_com->blok_cnt+1))
      {
        if (pc_com->ptr <= pc_com->old_ptr)
        {
          pc_com->ptr += MAXDATA_CAPTURE_FAST;
          pc_com->blok_cnt++;
        }
      }
      PC_Capture_Screen_Data_Fast(pc_com);
      break;
    case SUBCOMMANDEND:
      PC_Send_Error_Code(received_adres, send_adres, NOCOMMAND, ERRORSUBCOMMANDEND);
      break;
  }
}

//-----------------------------------------------------------------------------
static void PC_Receive_Key_Data(void)
{
unsigned char checksum;
unsigned char  key_code;

  if (received_lengte == 1)
  {
    key_code = Receive_Char();
    if ((key_code >= F1) && (key_code <= F6))
    {
      capture_pc_com_old = 0; // zorgt ervoor dat compleet scherm opnieuw wordt opgebouwd
      key_func = key_code;
    }  
    else if ((key_code == PREV) || (key_code == NEXT))
      key_func = key_code;
    else
      key = key_code;
    Put_Char(STX);
    checksum = STX;
    checksum = Send_Adres(send_adres/*PC_ADR*/, checksum); //  adres
    checksum = Send_Adres(received_adres, checksum); // adres
    checksum = Send_Command(received_command, checksum); //  command
    checksum = Send_Byte(SUBCOMMANDEND, checksum); // sub command
    checksum = Send_Char(0, checksum); // block_nr
    checksum = Send_Char(0, checksum); // aantal data bytes
    Send_Char(checksum,0);
    Put_Char(ETX_CH);
    Put_Char(ETX);
    Put_Char(0);
    screen_catcher_time_out = 15;
    Display_Key_Proc(); // JP test versnellen scherm 23-10-07
  }
  else
    PC_Send_Error_Code(received_adres, send_adres, NOCOMMAND, ERRORLENGTE);
}

static void PC_Receive_Key(s_pc_com *pc_com)
{
  pc_com->command = received_command;
  pc_com->command_timeout = COMMANDPCTIMEOUT;
  switch (received_sub_command)
  {
    case SUBCOMMANDBEGIN:
    case SUBCOMMANDPREV:
    case SUBCOMMANDNEXT:
      PC_Receive_Key_Data();
      break;
    case SUBCOMMANDEND: 
      PC_Send_Error_Code(received_adres, send_adres, NOCOMMAND, ERRORSUBCOMMANDEND);
      break;
  }
}

//*****************************************************************************
#ifdef ALARM_TEKST_NAAR_SMARTLINK
static void PC_Alarm_Tekst_Read_Data(void)
{
unsigned char loop;
unsigned char checksum;
char string[100];
unsigned char length;

  Alarm_Create_String_PC(string, &alarm_disp_pc);
  length = strlen(string);
  Put_Char(STX);
  checksum = STX;
  checksum = Send_Adres(send_adres, checksum); // adres
  checksum = Send_Adres(received_adres, checksum); // adres
  checksum = Send_Command(received_command, checksum); // command
  checksum = Send_Byte(received_sub_command, checksum); // sub command
  checksum = Send_Char(0, checksum); // block_nr
  checksum = Send_Char(length + 4 + 2, checksum);
  checksum = Send_Long(alarm_disp_pc.on, checksum);
  checksum = Send_Int(alarm_hard_actief_aantal, checksum);
  for (loop = 0; loop < length; loop++)
    checksum = Send_Char(string[loop], checksum);
  Send_Char(checksum,0);
  Put_Char(ETX_CH);
  Put_Char(ETX);
  Put_Char(0);
  switch (send_adres)
  {
    case PC_0_ADR: alarm_tekstread_mask &= ~(unsigned int)0x0001; break;
    case PC_1_ADR: alarm_tekstread_mask &= ~(unsigned int)0x0002; break;
    case PC_2_ADR: alarm_tekstread_mask &= ~(unsigned int)0x0004; break;
  }  
}

void PC_Alarm_Tekst_Read(s_pc_com *pc_com)
{
  pc_com->command = received_command;
  pc_com->command_timeout = COMMANDPCTIMEOUT;
  switch (received_sub_command)
  {
    case SUBCOMMANDBEGIN:
    case SUBCOMMANDPREV:
    case SUBCOMMANDNEXT:
      PC_Alarm_Tekst_Read_Data();
      break;
    case SUBCOMMANDEND:
      PC_Send_Error_Code(received_adres, send_adres, NOCOMMAND, ERRORSUBCOMMANDEND);
      break;
  }
}
#endif // ALARM_TEKST_NAAR_SMARTLINK
//*****************************************************************************
void PC_Command_Not_Possible(void)
{
  PC_Send_Error_Code(received_adres, send_adres/*PC_ADR*/, NOCOMMAND, ERRORCOMMANDUNKNOWN);
}

void PC_Command_Old_Config_Read(void)
{
  PC_Send_Error_Code(received_adres, send_adres, NOCOMMAND, ERROROLDMODULEASK);
}

//*****************************************************************************
void Verwerk_Data_Blackbox_Command(s_pc_com *pc_com)
{
  switch (received_command)
  {
    case CONFIGURATIONREAD:     PC_Configuration_Read_Blackbox(pc_com); break;

    case TIMEWRITE:             PC_Time_Write_Blackbox(pc_com); break;

    case XML_READ:              PC_XML_Read(pc_com); break;

    case MOTORGROUP_READ:        PC_Motorgroup_Read(pc_com); break;
	case MOTORGROUP_WRITE:       PC_Motorgroup_Write(pc_com); break;

    default:                    PC_Send_Error_Code(received_adres, send_adres, NOCOMMAND, ERRORCOMMANDUNKNOWN); break;
  }
}

void Verwerk_Data_Algemeen_Command(s_pc_com *pc_com)
{
  switch (received_command)
  {
    case CONFIGURATIONREAD:      PC_Configuration_Read_Blackbox(pc_com); break;

    case TIMEWRITE:              PC_Time_Write_Blackbox(pc_com); break;

    case OPTIONS_APP_READ:       PC_Option_Read_App(pc_com); break;
    case OPTIONS_APP_WRITE:      PC_Option_Write_App(pc_com); break;

    case BLACKBOXMAINGROUPREAD:
    case MAINGROUPREAD:          PC_Main_Group_Read_Algemeen(pc_com); break;

    case OPTIONS_IO_READ:        PC_Option_Read_IO(pc_com); break;
    case OPTIONS_IO_WRITE:       PC_Option_Write_IO(pc_com); break;

    case TEKST_MC_READ:          PC_Tekst_MC_Read(pc_com); break;
    case TEKST_MC_WRITE:         PC_Tekst_MC_Write(pc_com); break;

    case OPTIONS_ALG_READ:       PC_Option_Read_Algemeen(pc_com); break;

    case ALLOPTIONSREAD:         PC_Save_Option_Algemeen(pc_com); break;
    case ALLOPTIONSWRITE:        PC_Restore_Option_Algemeen(pc_com); break;

    case ALLSETPOINTSREAD:       PC_Save_Setpoint_Algemeen(pc_com); break;
    case ALLSETPOINTSWRITE:      PC_Restore_Setpoint_Algemeen(pc_com); break;

    case ALARMREAD:              PC_Alarm_Read_Algemeen(pc_com); break;
    case ALARMWRITE:             PC_Alarm_Write_Algemeen(pc_com); break;

    case STATUS_READ:            PC_Status_Read(pc_com); break;
    case STATUS_WRITE:           break;

    case MOTOR_MANAGEMENT_READ:  PC_Motor_Management_Read(pc_com); break;
    case MOTOR_MANAGEMENT_WRITE: break;

    case CAPTURESCREEN:         PC_Capture_Screen(pc_com); break;
    case RECEIVEKEY:            PC_Receive_Key(pc_com); break;
    case CAPTURESCREENNOTAB:    PC_Capture_No_Tab_Screen(pc_com); break;
    case CAPTURESCREENFAST:
      pc_com->fast = SDO_MAX_BLOCKS;
      PC_Capture_Screen_Fast(pc_com); 
      break;

    case MAINGROUPCHANGED:      PC_Main_Group_Changed_Algemeen(pc_com); break;
    
    case MODULEREAD:            PC_Module_Read_Algemeen(pc_com); break;
    case MODULEWRITE:           PC_Module_Write_Algemeen(pc_com); break;

    case TEKSTREAD:             PC_Tekst_Eeprom_Read_Algemeen(0,pc_com); break; //PC_Tekst_Read(USER,pc_com); break;
    case TEKSTWRITE:            PC_Tekst_Write_Algemeen(pc_com); break;
    case TEKSTINSTREAD:         PC_Tekst_Eeprom_Read_Algemeen(1,pc_com); break; //PC_Tekst_Inst_Read(USER,pc_com); break;
    case TEKSTINSTWRITE:        PC_Tekst_Inst_Write_Algemeen(pc_com); break;
    case TEKSTVERSIEREAD:       PC_Tekst_Versie_Read_Algemeen(pc_com); break;
    case TEKSTREAD_0:           PC_Tekst_Read_Algemeen(ENGELS,pc_com); break;
    case TEKSTINSTREAD_0:       PC_Tekst_Inst_Read_Algemeen(ENGELS,pc_com); break;
    case TEKSTREAD_1:           PC_Tekst_Read_Algemeen(NEDERLANDS,pc_com); break;
    case TEKSTINSTREAD_1:       PC_Tekst_Inst_Read_Algemeen(NEDERLANDS,pc_com); break;
    case TEKSTREAD_2:           PC_Tekst_Read_Algemeen(DUITS,pc_com); break;
    case TEKSTINSTREAD_2:       PC_Tekst_Inst_Read_Algemeen(DUITS,pc_com); break;
    case TEKSTREAD_3:           PC_Tekst_Read_Algemeen(SPAANS,pc_com); break;
    case TEKSTINSTREAD_3:       PC_Tekst_Inst_Read_Algemeen(SPAANS,pc_com); break;

    #ifdef ALARM_TEKST_NAAR_SMARTLINK
    case ALARMTEKSTREAD:        PC_Alarm_Tekst_Read(pc_com); break;
    #endif // ALARM_TEKST_NAAR_SMARTLINK
   
    case FAN_DATA_READ :        PC_Fan_Data_Read(pc_com);  break;
    case FAN_DATA_WRITE:        PC_Fan_Data_Write(pc_com); break;

    case OPTIONS_IO_APP_READ :  PC_Options_IO_App_Read(pc_com);  break;
    case OPTIONS_IO_APP_WRITE:  PC_Options_IO_App_Write(pc_com); break;

    #ifdef SD_CARD
    case SD_DIRECTORY_READ:     PC_SD_Directory_Read(pc_com);   break;
    case SD_FILE_READ:          PC_SD_File_Read(pc_com);        break;
    case SD_LOG_CONFIG_WRITE:   PC_SD_Log_Config_Write(pc_com); break;
    case SD_FILE_DELETE:        PC_SD_File_Delete(pc_com);      break;
    case SD_FORMAT:             PC_SD_Format(pc_com);           break;
    case SD_STATUS_WRITE:       PC_SD_Write_Status(pc_com);     break;
    #else // SD_CARD
    case SD_DIRECTORY_READ:     PC_SD_Read_Data_Empty_Stop(pc_com); break;
    case SD_FILE_READ:          PC_SD_Read_Data_Empty_Stop(pc_com); break;
    case SD_LOG_CONFIG_WRITE:   PC_SD_Read_Data_Empty_Stop(pc_com); break;
    case SD_FILE_DELETE:        PC_SD_Read_Data_Empty_Stop(pc_com); break;
    case SD_FORMAT:             PC_SD_Read_Data_Empty_Stop(pc_com); break;
    case SD_STATUS_WRITE:       PC_SD_Read_Data_Empty_Stop(pc_com); break;
    #endif // SD_CARD

    case MEM_DUMP_READ:         PC_Mem_Dump_Read(pc_com); break;
    case MEM_DUMP_WRITE:        PC_Mem_Dump_Write(pc_com); break;
    case VAL_DEBUG_READ:        PC_Val_Debug_Read(pc_com); break;
    case VAL_DEBUG_WRITE:       PC_Val_Debug_Write(pc_com); break;

    default:                    PC_Send_Error_Code(received_adres, send_adres, NOCOMMAND, ERRORCOMMANDUNKNOWN); break;
  }
}

void Verwerk_Data_Command(s_pc_com *pc_com)
{
  #if (SDO_MAX==8)
  if (send_adres == 1020) // PDA
    pc_com->fast = SDO_1_BLOCK;
  else
    pc_com->fast = SDO_8_BLOCKS;
  #else // (SDO_MAX==8)
  pc_com->fast = SDO_1_BLOCK;
  #endif // (SDO_MAX==8)  
  switch (commando_voor)
  {
    case COM_VOOR_BLACKBOX: Verwerk_Data_Blackbox_Command(pc_com); break;
    case COM_VOOR_ALG:      Verwerk_Data_Algemeen_Command(pc_com); break;
//    case COM_VOOR_AFD:      Verwerk_Data_Afdeling_Command(pc_com); break;
    default:                break;
  }
}

void Verwerk_Data(s_pc_com *pc_com)
{
  switch (received_sub_command)
  {
    case SUBCOMMANDBEGIN:
      pc_com->blok_cnt = 0;
      Verwerk_Data_Command(pc_com);
      break;
    case SUBCOMMANDPREV:
      if ((pc_com->command == received_command) &&
          (received_blok_cnt == pc_com->blok_cnt))
        Verwerk_Data_Command(pc_com);
      break;
    case SUBCOMMANDNEXT:
      Verwerk_Data_Command(pc_com); // JP 11-06-08
/*
      if (pc_com->command == received_command)
      {
        if (received_command & 0x0001)
        {
          // WRITE COMMAND
          if (received_blok_cnt == pc_com->blok_cnt)
            Verwerk_Data_Command(pc_com);
        }
        else
        {
          // READ COMMAND
          if (received_blok_cnt == (unsigned char)(pc_com->blok_cnt+1))
            Verwerk_Data_Command(pc_com);
        }
      }
*/
      break;
    case SUBCOMMANDEND:
      if (pc_com->command == received_command)
        Verwerk_Data_Command(pc_com);
      break;
  }
}


int Size_Buffer(char *command_buffer)
{
 int length = 0;

  while (*command_buffer++ != 0)
    length++;
  return (length);
}

unsigned char Calc_Checksum(char *command_buffer)
{
unsigned char checksum = 0;

  while (*command_buffer != 0)
    checksum ^= *command_buffer++;
  return (checksum);
}

char Check_Receive_Send_Adres(void)
// return 1 als receive adres is correct
{
  received_adres = Receive_Adres();
  send_adres = Receive_Adres();
  if (send_adres == 0xFFF)
  {
    return (2);
  }  
  else if (send_adres == PC_0_ADR)
  {
    // PC communicatie
    if (received_adres == BLACKBOX_ADR)
    {
      commando_voor = COM_VOOR_BLACKBOX;
      return (1);
    }
    else if (received_adres == opt_alg.adres)
    {
      commando_voor = COM_VOOR_ALG;
      return (1);
    }
  }
  else if (send_adres == PC_1_ADR)
  {
    // PC communicatie
    if (received_adres == BLACKBOX_ADR)
    {
      commando_voor = COM_VOOR_BLACKBOX;
      return (1);
    }
    else if (received_adres == opt_alg.adres)
    {
      commando_voor = COM_VOOR_ALG;
      return (1);
    }
  }
  else if (send_adres == PC_2_ADR)
  {
    // PC communicatie
    if (received_adres == BLACKBOX_ADR)
    {
      commando_voor = COM_VOOR_BLACKBOX;
      return (1);
    }
    else if (received_adres == opt_alg.adres)
    {
      commando_voor = COM_VOOR_ALG;
      return (1);
    }
  }
  return (0);
}

char Controleer_Data(int *command, char *command_buffer, char *answer_buffer)
{
int lengte;
unsigned char checksum;

  *command = NOCOMMAND;
  command_ptr = command_buffer;
  answer_ptr = answer_buffer;
  *answer_ptr = 0;
  lengte = Size_Buffer(command_ptr);
  if (lengte >= 19) // controleer minimum lengte
  {
    received_ETB_or_ETX = command_ptr[lengte - 2];
    if ((received_ETB_or_ETX == ETX_CH) || // controleer op ETX of ETB
        (received_ETB_or_ETX == ETB_CH))
    {
      checksum = Asc_To_Hex(command_ptr[lengte - 4]);
      checksum <<= 4;
      checksum |= Asc_To_Hex(command_ptr[lengte - 3]);
      command_ptr[lengte - 4] = 0;
      if (checksum == Calc_Checksum(command_ptr)) // controleer of checksum in orde
      {
        command_ptr++;  // verwijder STX karakter
        switch (Check_Receive_Send_Adres())
        {
          case 1:
            // inlezen commando
            received_command = Receive_Command();
            *command = received_command;
            // inlezen sub command
            received_sub_command = Receive_Byte();
            // inlezen block nummer
            received_blok_cnt = Receive_Char();
            // inlezen lengte
            received_lengte = Receive_Char();
            return (0);
          case 2:
            return (2);  
        }
      }
    }
  }
  return (1);
}
























/*static void PC_XML_Read_Data(s_pc_com *pc_com)
{
unsigned char checksum;
int index, length;
int DeviceAddress = 0;
TDevice *pDevice;
TXML XML;

  Put_Char(STX);                                         // 1
  checksum = STX;
  checksum = Send_Adres(send_adres, checksum); // 3 - adres 
  checksum = Send_Adres(received_adres, checksum);       // 3 - adres
  checksum = Send_Command(received_command, checksum);   // 3 - command
  checksum = Send_Byte(received_sub_command, checksum);  // 1 - sub command
  checksum = Send_Char(pc_com->blok_cnt, checksum);      // 2 - block_nr

  // initialiseer en bepaal lengte
  length = 0;
  index  = pc_com->index;
  pDevice  = (TDevice *)pc_com->ptr;

  if (pc_com->blok_cnt == 0)
  {
    length += 4;
  }

  while (length <= MAXDATA_XML)
  {
    if (pDevice == NULL) // groep data
    {
      if (index >= opt_app.NumberMotorgroups)
        break;
      if (opt_app.Motorgroup[index].Type == TYPE_VENT)
      {
        if ((length + 10) > MAXDATA_XML)
          break;
        length += 10; // block size group
        pDevice = Motorgroup[index].FirstMotor;
      }
      index++;
    }
    else // vent data
    {
      if ((length + 19) > MAXDATA_XML)
        break;
      length += 19;
      pDevice = pDevice->Next;
    }
  }
//  checksum = Send_Char(length, checksum); // aantal data bytes
  checksum = Send_Int(length, checksum); // aantal data bytes


  // stuur data
  length = 0;
  index  = pc_com->index;
  pDevice  = (TDevice *)pc_com->ptr;

  if (pc_com->blok_cnt == 0)
  {
    checksum = Send_Int(opt_alg.adres, checksum); // 2
    checksum = Send_Int(VERSIE_XML_PC, checksum); // 4
    length += 4;
  }

  while (length <= MAXDATA_XML)
  {
    if (pDevice == NULL) // groep data
    {
      if (index >= opt_app.NumberMotorgroups)
        break;
      if (opt_app.Motorgroup[index].Type == TYPE_VENT)
      {
        if ((length + 10) > MAXDATA_XML)
          break;

        XML_GetData_Group(index, &XML);
        checksum = Send_Char(index + 1,                              checksum); //  1 - groep nummer
        checksum = Send_Char(index + 1,                              checksum); //  1 - groep nummer
        checksum = Send_Char(opt_app.Motorgroup[index].NumberMotors, checksum); //  2 - aantal ventilatoren
        checksum = Send_Char(XML.Position,                           checksum); //  3 - gemiddelde positie in procenten
        checksum = Send_Int (XML.EnergyConsumption,                  checksum); //  5 - Totale vermogen groep
        checksum = Send_Int (XML.Rpm,                                checksum); //  7 - gemiddelde snelheid in rpm
        checksum = Send_Char(XML.Status,                             checksum); //  8 - status
        checksum = Send_Char(XML.ErrorUrgent,                        checksum); //  9 - urgent alarm
        checksum = Send_Char(XML.ErrorNotUrgent,                     checksum); // 10 - niet urgent alarm

        length += 10; // block size group
        pDevice = Motorgroup[index].FirstMotor;
      }
      index++;
    }
    else // vent data
    {
      if ((length + 19) > MAXDATA_XML)
        break;

      DeviceAddress = XML_GetData_Fan(pDevice->Number, &XML);
      
      checksum = Send_Int (pDevice->Number + 1,                             checksum); //  2 - Fan number
      checksum = Send_Char(XML.Position,                                  checksum); //  3 - Position value
      checksum = Send_Int (XML.EnergyConsumption,                         checksum); //  5 - Actual power consumption value
      checksum = Send_Int (XML.Rpm,                                       checksum); //  7 - Speed value
      checksum = Send_Char(mbDeviceGetTempMotor(DeviceAddress),           checksum); //  8 - Temp motor value
      checksum = Send_Char(mbDeviceGetTempElectronics(DeviceAddress),     checksum); //  9 - Temp electronics value
      checksum = Send_Char(mbDeviceGetTempPowerModule(DeviceAddress),     checksum); // 10 - Temp power module
      checksum = Send_Char(XML.Status,                                    checksum); // 11 - Motor status value
      checksum = Send_Char(XML.ErrorUrgent,                               checksum); // 12 - Error urgent value
      checksum = Send_Char(XML.ErrorNotUrgent,                            checksum); // 13 - Error not urgent value
      checksum = Send_Int (mbDeviceGetErrorCode(DeviceAddress),           checksum); // 15 - Error code
      checksum = Send_Long(mbDeviceGetRunningHours(DeviceAddress) / 3600, checksum); // 19 - Running hours value

      length += 19;
      pDevice = pDevice->Next;
    }
  }

  pc_com->index = index;
  pc_com->ptr = (unsigned char *)pDevice;

  Send_Char(checksum, 0);
  if ((index >= opt_app.NumberMotorgroups) && (pDevice == NULL))
    Put_Char(ETX_CH);
  else
    Put_Char(ETB_CH);

  Put_Char(ETX);
  Put_Char(0);
}

                   */
