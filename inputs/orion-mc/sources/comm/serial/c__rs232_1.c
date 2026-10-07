// C__RS232_1.C                             

#include "ch_define.h"
#include "ch_alarm.h"
#include "ch_alg.h"
#include "ch_Asc1.h"
#include "ch_key.h"
#include "ch_lcd_hardware.h"
#include "ch_main.h"  
#include "ch_pc_com.h"
#include "ch_sd.h"
#include "ch_tijd.h"
#include "ch_timer.h"
#include "ch_rs232_1.h"

#if (PROCESSOR==XC161CJ)

#define RS232_BUFFERSIZE (512+20)

static char RS232_1_command_buffer[RS232_BUFFERSIZE];  // input buffer for data from an PC
static char RS232_1_answer_buffer[RS232_BUFFERSIZE];   // output buffer for data from an PC
static unsigned char rs232_1_receive_buffer[MAX_BUFFER_COM]; // buffer voor inkomende data (tussen opslag) wordt doorgegeven als bericht compleet binnen
                                                                  // in receive buffer moeten opties of setpoints of modules of lcd schaerm passen
                                                                  // wordt gebruikt voor tussen opslag van binnenkomend opties totdat alle opties binnen zijn
                                                                  // wordt gebruikt voor tussen opslag van binnenkomend setpoints totdat alle opties binnen zijn
                                                                  // wordt gebruikt voor tussen opslag van binnenkomend modules totdat alle opties binnen zijn
                                                                  // wordt gebruikt voor tussen opslag van uitgaangd lcd_scherm totdat complete scherm verzonden is
s_pc_com rs232_1_pc_com =    { /*0*/rs232_1_receive_buffer,0,0,0,0,0,0,0,0,0,0};
s_pc_com rs232_1_orion_com = { /*0*/rs232_1_receive_buffer,0,0,0,0,0,0,0,0,0,0};
int rs232_1_main_group_blocked = 0;

//*****************************************************************************

// default waarden bij initialiseren 
void RS232_1_Init(void)
{
  rs232_1_pc_com.ptr_receive_buffer = rs232_1_receive_buffer;
  rs232_1_orion_com.command = NOCOMMAND;
  rs232_1_orion_com.ptr_receive_buffer = rs232_1_receive_buffer;
  rs232_1_orion_com.error_cnt = 0;
}

//*****************************************************************************
void RS232_1_Check_Main_Group_Changed(void)
{
  if (rs232_1_main_group_blocked >= MAINGROUPBLOCKED)
  {
    rs232_1_main_group_blocked = MAINGROUPBLOCKED;
    if ((rs232_1_orion_com.command == NOCOMMAND) &&	(rs232_1_orion_com.error_cnt < COMMANDORIONERRORMAX))
    {
      rs232_1_orion_com.command = MAINGROUPCHANGED;
      Verwerk_Main_Group_Changed(&rs232_1_orion_com, RS232_1_answer_buffer);
      ASC1_Put_String(RS232_1_answer_buffer);
    }
  }
}

void RS232_1_125ms_Time_Out(void)
{
  if (rs232_1_main_group_blocked < MAINGROUPBLOCKED)
    rs232_1_main_group_blocked++;
  if (rs232_1_orion_com.command != NOCOMMAND)
  {
    if (rs232_1_orion_com.command_timeout)
      rs232_1_orion_com.command_timeout--;
    else
    {
      if (rs232_1_orion_com.error_cnt < COMMANDORIONERRORMAX)
        rs232_1_orion_com.error_cnt++;
      rs232_1_orion_com.command = NOCOMMAND;
    }
  }
}

int RS232_1_Read_Command(char *command_buffer)
{
char ch;
char *ptr = command_buffer;

  // verwijder karakters tot aan STX karakters
  do
  {
    ch = ASC1_Get_Char();
    if (ch == 0)
    {
      // geen start karakter gevonden dus geen commando aanwezig
      asc1_receive_cnt = 0;
      *command_buffer = 0;
      return (0);
    }
  }
  while (ch != STX);
  // start karakter gevonden
  *ptr++ = STX;
  // zet karakters tot en met ETX in command_buffer
  do
  {
    ch = ASC1_Get_Char();
    switch (ch)
    {
      case 0:
        // geen eind karakter gevonden dus geen commando aanwezig
        asc1_receive_cnt = 0;
        *command_buffer = 0;
        return (0);
      case STX:
        ptr = command_buffer;
        *ptr++ = STX;
        break;
      default:
        *ptr++ = ch;
        break;
    }
  }
  while (ch != ETX);
  // eind karakter gevonden
  *ptr = 0;
  asc1_receive_cnt--;
  return (1);
}

#ifdef SD_CARD
void Bijwerken_RS232_1_Communicatie_Log(unsigned char status)
{
  File_Printf(COM_FILE, "COM2 %s: %02i-%02i-%04i %02i:%02i:%02i\r\n",
              (status ? "START" : "STOP"),
              tijd.tm_mday, tijd.tm_mon, tijd.tm_year,
              tijd.tm_hour, tijd.tm_min, tijd.tm_sec);
}
#endif // SD_CARD

void RS232_1_Control(void)
{
static s_timer timer_connected;
static unsigned char rainbow_connected = 0;
static unsigned long start_tijd;
int commando = NOCOMMAND;
#ifdef SD_CARD
unsigned char rainbow_connected_old = rainbow_connected;
#endif // SD_CARD

  switch (asc1_transmit_state)
  {
    case TRANSMIT_READY:
      if (asc1_receive_cnt)
      {
        if (RS232_1_Read_Command(RS232_1_command_buffer))
        {
          switch (Controleer_Data(&commando, RS232_1_command_buffer, RS232_1_answer_buffer)) // geen fout in data string
          {
            case 0:
              switch (commando)
              {
                case SHUTUPCOMMAND:
                  rainbow_connected = 0;
                  rs232_1_orion_com.error_cnt = COMMANDORIONERRORMAX;
                  rs232_1_orion_com.command = NOCOMMAND;
                  rs232_1_pc_com.command = NOCOMMAND;
                  break;
                case MAINGROUPCHANGED:
                  Timer_Set(&timer_connected, 60, TIME_BASE_1_SEC);
                  rainbow_connected = 1;
                  Verwerk_Data(&rs232_1_orion_com);
                  ASC1_Put_String(RS232_1_answer_buffer); // schrijf antwoord string weg
                  break;
                case BLACKBOXMAINGROUPREAD:
                  PC_Command_Not_Possible();
                  break;
                default:
                  Timer_Set(&timer_connected, 60, TIME_BASE_1_SEC);
                  rainbow_connected = 1;
                  Verwerk_Data(&rs232_1_pc_com);
                  ASC1_Put_String(RS232_1_answer_buffer); // schrijf antwoord string weg
                  rs232_1_orion_com.command_timeout = COMMANDORIONTIMEOUT;
                  if (rs232_1_orion_com.error_cnt >= COMMANDORIONERRORMAX)
                  {
                    rs232_1_orion_com.error_cnt = 0; // er komt weer data binnen dus hoofdgroep kan weer worden overgezonden
                  }
                  break;
              }
              break;
            case 2: // only used for modules pro
              Timer_Set(&timer_connected, 60, TIME_BASE_1_SEC);
              rainbow_connected = 1;
              PC_Command_Old_Config_Read();
              ASC1_Put_String(RS232_1_answer_buffer); // schrijf antwoord string weg
              break;  
          }
        }
      }
      else
      {
        RS232_1_Check_Main_Group_Changed(); // LET OP GEEN MAINGROUP
      }
      start_tijd = time(0);
      break;
    default:  
    case TRANSMIT_BUSY:
      if (time(0) - start_tijd > 10)
      {
        asc1_transmit_state = 1;
        start_tijd = time(0);
      }  
      break;
  }
  #ifdef SD_CARD
  if (rainbow_connected && Timer_Expired(&timer_connected))
    rainbow_connected = 0;
  if (rainbow_connected != rainbow_connected_old)
  {
    Bijwerken_RS232_1_Communicatie_Log(rainbow_connected);
  }
  #endif // SD_CARD
}

#else // (PROCESSOR==XC161CJ)

void RS232_1_Init(void)
{
}

void RS232_1_125ms_Time_Out(void)
{
}

void RS232_1_Control(void)
{
}

#endif // (PROCESSOR==XC161CJ)
