// C__ETHERNET.C

#include <string.h> 


#include "ch_define.h"

#ifdef ETHERNET

#include "ch_asc1.h"
#include "ch_can_backbone_appl.h"
#include "ch_can_backbone_pc.h"
#include "ch_pc_com.h" 
#include "ch_sd.h"
#include "ch_tijd.h"
#include "uip.h" 
#include "ch_ethernet.h"

// BUFFERSIZE define in ch_can_backbone!
//#define ETHERNET_BUFFERSIZE (512+20)
#define ETHERNET_BUFFERSIZE (1024+22)

//#pragma class HB=EXTENDED_MEMORY
#pragma noclear
char ethernet_command_buffer[4][ETHERNET_BUFFERSIZE];  // input buffer for data from an PC
char ethernet_answer_buffer[ETHERNET_BUFFERSIZE];   // output buffer for data from an PC
unsigned char ethernet_receive_buffer[4][MAX_BUFFER_COM]; // buffer voor inkomende data (tussen opslag) wordt doorgegeven als bericht compleet binnen
                                                       // in receive buffer moeten opties of setpoints of modules of lcd schaerm passen
                                                       // wordt gebruikt voor tussen opslag van binnenkomend opties totdat alle opties binnen zijn
                                                       // wordt gebruikt voor tussen opslag van binnenkomend setpoints totdat alle opties binnen zijn
                                                       // wordt gebruikt voor tussen opslag van binnenkomend modules totdat alle opties binnen zijn
                                                       // wordt gebruikt voor tussen opslag van uitgaangd lcd_scherm totdat complete scherm verzonden is
#pragma clear
#pragma default_attributes

s_pc_com ethernet_pc_com[4] =    
{
  { ethernet_receive_buffer[0],0,0,0,0,0,0,0,0,0,0 },
  { ethernet_receive_buffer[1],0,0,0,0,0,0,0,0,0,0 },
  { ethernet_receive_buffer[2],0,0,0,0,0,0,0,0,0,0 },
  { ethernet_receive_buffer[3],0,0,0,0,0,0,0,0,0,0 }
};
s_pc_com ethernet_orion_com[4] =
{
  { ethernet_receive_buffer[0],0,0,0,0,0,0,0,0,0,0 },
  { ethernet_receive_buffer[1],0,0,0,0,0,0,0,0,0,0 },
  { ethernet_receive_buffer[2],0,0,0,0,0,0,0,0,0,0 },
  { ethernet_receive_buffer[3],0,0,0,0,0,0,0,0,0,0 }
};
int ethernet_main_group_blocked[4] = {0,0,0,0};

//*****************************************************************************
// default waarden bij initialiseren 
void Ethernet_Init(void)
{
int loop;

  for (loop = 0; loop < 4; loop++)
  {
    ethernet_pc_com[loop].ptr_receive_buffer = ethernet_receive_buffer[loop];
    ethernet_orion_com[loop].command = NOCOMMAND;
    ethernet_orion_com[loop].ptr_receive_buffer = ethernet_receive_buffer[loop];
    ethernet_orion_com[loop].error_cnt = 0;
  }  
}

//*****************************************************************************
void Ethernet_Check_Main_Group_Changed(unsigned char conn_nr)
{
  if (ethernet_main_group_blocked[conn_nr] >= MAINGROUPBLOCKED) 
  {
    ethernet_main_group_blocked[conn_nr] = MAINGROUPBLOCKED;
    if ((ethernet_orion_com[conn_nr].command == NOCOMMAND) && (ethernet_orion_com[conn_nr].error_cnt < COMMANDORIONERRORMAX))
    {
      ethernet_orion_com[conn_nr].command = MAINGROUPCHANGED;
      Verwerk_Main_Group_Changed(&ethernet_orion_com[conn_nr], ethernet_answer_buffer);
      Ethernet_Put_String(conn_nr, ethernet_answer_buffer);
    }
  }
}

void Ethernet_125ms_Time_Out(void)
{
int loop;

  for (loop = 0; loop < 4; loop++)
  {
    if (ethernet_main_group_blocked[loop] < MAINGROUPBLOCKED)
      ethernet_main_group_blocked[loop]++;
    if ((ethernet_orion_com[loop].command != NOCOMMAND))
    {
      if (ethernet_orion_com[loop].command_timeout)
        ethernet_orion_com[loop].command_timeout--;
      else
      {
        if (ethernet_orion_com[loop].error_cnt < COMMANDORIONERRORMAX)
          ethernet_orion_com[loop].error_cnt++;
        ethernet_orion_com[loop].command = NOCOMMAND;
      }  
    }
    if (uip_conns[loop].appstate.connected)
    {
      if (uip_conns[loop].appstate.close_connection_timeout < 400) // als 40 seconden ethernet verbinding bezig is zonder berichten dan verbinding verbreken
        uip_conns[loop].appstate.close_connection_timeout++;
      else
        uip_conns[loop].appstate.close_connection = 1;;
    }    
  }
}

char Ethernet_Get_Char(unsigned char nr)
{
char ch;

  if (uip_conns[nr].appstate.in_get_index != uip_conns[nr].appstate.in_put_index)
  {
    ch = uip_conns[nr].appstate.buffer_in[uip_conns[nr].appstate.in_get_index];
    uip_conns[nr].appstate.in_get_index++;
    uip_conns[nr].appstate.in_get_index %= ETH_BUFFER;
    return (ch);
  }
  return (0);
}

#ifndef TEST_ASC_STANDAARD

int Ethernet_Read_Command(unsigned char nr, char *command_buffer)
{
char ch;
char *ptr = command_buffer;
int length = 0;

  do 
  {
    ch = Ethernet_Get_Char(nr);
    if (ch == 0)
    {
      uip_conns[nr].appstate.receive_cnt = 0;
      *command_buffer = 0;
      return (0);
    }
  }
  while (ch != STX);
  *ptr++ = STX;
  length++;
  do
  {
    ch = Ethernet_Get_Char(nr);
    switch (ch)
    {
      case 0:
        uip_conns[nr].appstate.receive_cnt = 0;
        *command_buffer = 0;
        length = 0;
        return (0);
      case STX:
        ptr = command_buffer;
        *ptr++ = STX;
        length = 0;
        break;
      default:
        *ptr++ = ch;
        length++;
        break;
    }
  }
  while (ch != ETX);
  *ptr = 0;
  uip_conns[nr].appstate.receive_cnt--;
  return (length);
}  
#else // TEST_ASC_STANDAARD
int Ethernet_Read_Command(unsigned char nr, char *command_buffer)
{
char ch;
static char *ptr;
static char state;
static int length;

  while (1)
  {
    ch = Ethernet_Get_Char(nr);
    switch (ch)
    {
      case 0:
        return (0);
      case STX:
        state = 1;
        ptr = command_buffer;
        *ptr = STX;
        ptr++;
        length = 1;
        break;  
      case ETX:
        if (state == 1)
        {
          state = 0;
          *ptr = ETX;
          ptr++;
          *ptr = 0; // JP 18-04-07
          length++;
          return (length);
        }  
        break;
      default:
        if (state == 1)
        {
          *ptr = ch;
          ptr++;
          length++;
        }
        break;
    }
  }
}
#endif // TEST_ASC_STANDAARD

//char eens = 0;
//char jp_test[255] = "<HTML><HEAD><TITLE>404 niet gevonden</TITLE></HEAD><BODY><H1>NIET GEVONDEN</H1></BODY></HTML>/r/n/r/n";
//char jp_msg[255]; 
//int jp_length;
//time_t jp_time;
//struct tm *jp_time_s;
/*
{
  if (eens == 0)
  {
    eens = 1; 
    jp_length = strlen(jp_test);   
    Ethernet_0_Put_String("HTTP/1.1 200 OK\r\n");
    time(&jp_time);
    jp_time_s = gmtime(&jp_time);
    sprintf(jp_msg,"Date: %a, %d %b %Y %H:%M:%S %Z\r\n",jp_time_s);
    Ethernet_0_Put_String(jp_msg);
    Ethernet_0_Put_String("Server: http_server 0.1\r\n");
    Ethernet_0_Put_String("Accept-Ranges: none\r\n");
    sprintf(jp_msg,"Content-Length: %d\r\n",jp_length);
    Ethernet_0_Put_String(jp_msg);
    Ethernet_0_Put_String("Connection: Keep-Alive\r\n");
    Ethernet_0_Put_String("Content-Type: text/html\r\n");
    Ethernet_0_Put_String("\r\n");
    Ethernet_0_Put_String(jp_test);
    Ethernet_0_Put_String("\r\n\r\n");
  }  
}    
*/

#ifdef SD_CARD
void Bijwerken_Ethernet_Communicatie_Log(unsigned char conn_nr, unsigned char status)
{
  File_Printf(COM_FILE, "ETHERNET IP %i.%i.%i.%i %s: %02i-%02i-%04i %02i:%02i:%02i\r\n",
              uip_conns[conn_nr].ripaddr[0] & 0x00FF,
              (uip_conns[conn_nr].ripaddr[0] >> 8) & 0x00FF,
              uip_conns[conn_nr].ripaddr[1] & 0x00FF,
              (uip_conns[conn_nr].ripaddr[1] >> 8) & 0x00FF,
              (status ? "START" : "STOP"),
              tijd.tm_mday, tijd.tm_mon, tijd.tm_year,
              tijd.tm_hour, tijd.tm_min, tijd.tm_sec);
}
#endif // SD_CARD

void Ethernet_Control_Port(unsigned char conn_nr)
{
static unsigned char rainbow_connected[4] = {0,0,0,0};
int commando = NOCOMMAND;
static int length;
unsigned char loop;
#ifdef SD_CARD
unsigned char rainbow_connected_old = rainbow_connected[conn_nr];
#endif // SD_CARD
int aantal_toegewezen = 0;

  if (uip_conns[conn_nr].appstate.connected)
  {
    for (loop = 0; loop < UIP_CONNS; loop++)
    {
      if (uip_conns[loop].appstate.toegewezen)
        aantal_toegewezen++;
    }
    if (aantal_toegewezen < 3)
      uip_conns[conn_nr].appstate.toegewezen = 1;
    if (uip_conns[conn_nr].appstate.toegewezen == 0)
    {
      uip_conns[conn_nr].appstate.receive_cnt == 0;
      if (uip_conns[conn_nr].appstate.send_state == ETH_SEND_NOT_BUSY)
      {
        uip_conns[conn_nr].appstate.close_connection_timeout = 0;
        Ethernet_Put_String(conn_nr, "@FFF07F000000018C3D*\r");
      }
      else
      {
      }
      return;
    }
    else if (uip_conns[conn_nr].appstate.send_state == ETH_SEND_NOT_BUSY) // JP 28-01-09
    {
      #ifndef TEST_ASC_STANDAARD
      if (uip_conns[conn_nr].appstate.receive_cnt)
      {
        length = Ethernet_Read_Command(conn_nr, ethernet_command_buffer[conn_nr]);
      #else // TEST_ASC_STANDAARD
      length = Ethernet_Read_Command(conn_nr, ethernet_command_buffer[conn_nr]);
      if (length)
      {
      #endif // TEST_ASC_STANDAARD
        switch (Controleer_Data(&commando, ethernet_command_buffer[conn_nr], ethernet_answer_buffer)) // geen fout in data string
        {
          case 0: // correct command
            uip_conns[conn_nr].appstate.close_connection_timeout = 0;
            switch (commando)
            {
              case SHUTUPCOMMAND:
                ethernet_orion_com[conn_nr].error_cnt = COMMANDORIONERRORMAX;
                ethernet_orion_com[conn_nr].command = NOCOMMAND;
                ethernet_pc_com[conn_nr].command = NOCOMMAND;
                rainbow_connected[conn_nr] = 0;
                #ifdef ASC1_ETHERNET
                ASC1_Printf("%10li ethernet shutup\r", time(0));
                #endif // ASC1_ETHERNET
                break;
              case MAINGROUPCHANGED:
                Verwerk_Data(&ethernet_orion_com[conn_nr]);
                Ethernet_Put_String(conn_nr, ethernet_answer_buffer);
                break;
              case BLACKBOXMAINGROUPREAD:
                PC_Command_Not_Possible();
                break;
              case CONFIGURATIONREAD:
                rainbow_connected[conn_nr] = 1;
              default:
                rainbow_connected[conn_nr] = 1;
                Verwerk_Data(&ethernet_pc_com[conn_nr]);
                Ethernet_Put_String(conn_nr, ethernet_answer_buffer);
                ethernet_orion_com[conn_nr].command_timeout = COMMANDORIONTIMEOUT;
                if (ethernet_orion_com[conn_nr].error_cnt >= COMMANDORIONERRORMAX)
                {
                  ethernet_orion_com[conn_nr].error_cnt = 0; // er komt weer data binnen dus hoofdgroep kan weer worden overgezonden
                }
                break;
            }    
            break;
          default:
            #ifdef ASC1_ETHERNET
            ASC1_Printf("%10li ethernet error\r", time(0));
            #endif // ASC1_ETHERNET
            break;
          case 2: // only used for modules pro
            uip_conns[conn_nr].appstate.close_connection_timeout = 0;
            PC_Command_Old_Config_Read();
            rainbow_connected[conn_nr] = 1;
            #ifdef ASC1_ETHERNET
            ASC1_Printf("%10li ethernet old config read %x %x\r", time(0), send_adres, received_adres);
            #endif // ASC1_ETHERNET
            Ethernet_Put_String(conn_nr, ethernet_answer_buffer);
            break;  
        }
      #ifndef TEST_ASC_STANDAARD
      }  
      #else // TEST_ASC_STANDAARD
      }  
      #endif // TEST_ASC_STANDAARD
      else
      {
        if (rainbow_connected[conn_nr])
          Ethernet_Check_Main_Group_Changed(conn_nr);
      }
    }
  }
  else
  {
    uip_conns[conn_nr].appstate.close_connection_timeout = 0;
    if (uip_conns[conn_nr].appstate.toegewezen == 1)
    {
      uip_conns[conn_nr].appstate.toegewezen = 0; 
      ethernet_orion_com[conn_nr].error_cnt = COMMANDORIONERRORMAX;
      ethernet_orion_com[conn_nr].command = NOCOMMAND;
      ethernet_pc_com[conn_nr].command = NOCOMMAND;
      rainbow_connected[conn_nr] = 0;
    }  
  }
  #ifdef SD_CARD
  if (rainbow_connected[conn_nr] != rainbow_connected_old)
  {
    Bijwerken_Ethernet_Communicatie_Log(conn_nr, rainbow_connected[conn_nr]);
  }
  #endif // SD_CARD
}

// JP 14-07-08 nog controleren (kan niet worden afgetest omdat er geen rainbow is)
void Ethernet_Control(void)
{
static int fase = 0;

  fase++;
  fase %= 4;
  Ethernet_Control_Port(fase);
/*
  Ethernet_Control_Port(0);
  Ethernet_Control_Port(1);
  Ethernet_Control_Port(2);
  Ethernet_Control_Port(3);
*/
}

#endif // ETHERNET
