// C__ALARM.C
 
#include <string.h>

#include "ch_define.h"

#include "ch_const.h"
#include "ch_data.h"
#include "ch_alg.h"
#include "ch_can_backbone_appl.h"
#include "ch_disp.h"
#include "ch_disp_alarm_0.h"
#include "ch_disp_func.h"
#include "ch_disp_option_0.h"
#include "ch_disp_password.h"
#include "ch_disp_start.h"
#include "ch_DS301.h"
#include "ch_IO.h"
#include "ch_IO_05_07.h"
#include "ch_IO_06_14.h"
#include "ch_IO_07_07.h"
#include "ch_IO_08_09.h"
#include "ch_IO_12_06.h"
#include "ch_IO_EKU.h"
#include "ch_IO_H1MC.h"
#include "ch_IO_H2MC.h"
#include "ch_IO_05_07_board.h"
#include "ch_IO_06_14_board.h"  
#include "ch_IO_07_07_board.h"
#include "ch_IO_08_09_board.h"
#include "ch_IO_12_06_board.h"  
#include "ch_IO_EKU_board.h"
#include "ch_IO_H1MC_board.h"
#include "ch_IO_H2MC_board.h"
#include "ch_key.h"
#include "ch_lcd_hardware.h"
#include "ch_luchtmengkast.h"
#include "ch_sd.h"
#include "ch_tijd.h"
#include "ch_device.h"
#include "ch_alarm.h"
#include "ch_mb_device.h"
#include "ch_motorgroep.h"

bit alarm_switch = 1;

s_alarm_disp alarm_disp_act;  // actueel alarm dat weergegeven moet worden en in rainbow wordt getoond
s_alarm_disp alarm_disp_pc;  // actueel alarm dat weergegeven moet worden en in rainbow wordt getoond
unsigned char alarm_disp_ico; // zorgt voor weergave van alarm icoon in tabblad

unsigned char alarm_blink_ico = 0;
unsigned char alarm_disp_blocked = 0; // als waarde gelijk aan 0 en er is een alarm dan wordt het alarm scherm weergegeven
unsigned char al_save_screen_change_flag;
long al_save_screen_val;
#ifdef ALARM_TEKST_NAAR_SMARTLINK
unsigned int alarm_tekstread_mask = 0x8000;
int alarm_hard_actief_aantal = 0; // JP 25-03-10
#endif // def ALARM_TEKST_NAAR_SMARTLINK
unsigned char const zacht_alarm = 0;
unsigned char const hard_alarm = 1;

//==============================================================================
//------------------------ Alarm - Table Handling ------------------------------
//==============================================================================
void GetAlarm(int al_code, TAlarm *Alarm)
{
TAlarm *AlarmData;

  AlarmData = (TAlarm *)&AlarmTable;
  while ((AlarmData->Code != al_code) && (AlarmData->Code != ONBEKEND_AL))
    AlarmData++;
  *Alarm = *AlarmData;
}

//==============================================================================
//------------------------ Alarm - Log Data ------------------------------------
//==============================================================================
#ifdef SD_CARD
char string[100];
#endif // SD_CARD

static unsigned char Search_Next_Free_Alarm_Index(void)
{
int loop;

  for(loop = 0; loop < ALARM_DISP_VALUES; loop++)
  {
    alarm_hr_alg.alarmen.index++;
    alarm_hr_alg.alarmen.index %= ALARM_DISP_VALUES;
	if((alarm_hr_alg.alarmen.al[alarm_hr_alg.alarmen.index].code == GEEN_AL) || (alarm_hr_alg.alarmen.al[alarm_hr_alg.alarmen.index].off))
	  return(1);
  }
  return(0);
}

static unsigned char Alarm_Log_Data_On_Disp(int al_code, int al_index, int al_value, int al_group, unsigned char *al_hard, int priority, unsigned char mask, unsigned char pc_type)
{
  if(Search_Next_Free_Alarm_Index())
  {
    alarm_hr_alg.alarmen.al[alarm_hr_alg.alarmen.index].code = al_code;
    alarm_hr_alg.alarmen.al[alarm_hr_alg.alarmen.index].index = al_index;
    alarm_hr_alg.alarmen.al[alarm_hr_alg.alarmen.index].value = al_value;
    alarm_hr_alg.alarmen.al[alarm_hr_alg.alarmen.index].group = al_group;
    alarm_hr_alg.alarmen.al[alarm_hr_alg.alarmen.index].on = time(0);
    alarm_hr_alg.alarmen.al[alarm_hr_alg.alarmen.index].off = 0;
    alarm_hr_alg.alarmen.al[alarm_hr_alg.alarmen.index].hard = al_hard;
    alarm_hr_alg.alarmen.al[alarm_hr_alg.alarmen.index].priority = priority;
    alarm_hr_alg.alarmen.al[alarm_hr_alg.alarmen.index].mask = mask; 
    if (mask & MASK_AL_HARD)
      alarm_hr_alg.alarmen.al[alarm_hr_alg.alarmen.index].state = (*al_hard) ? AL_HARD : AL_ZACHT;
    else if (mask & MASK_AL_ZACHT)
      alarm_hr_alg.alarmen.al[alarm_hr_alg.alarmen.index].state = AL_ZACHT;
    else // if (mask & MASK_AL_ONDERDRUKT)
      alarm_hr_alg.alarmen.al[alarm_hr_alg.alarmen.index].state = AL_ONDERDRUKT;
    alarm_hr_alg.alarmen.al[alarm_hr_alg.alarmen.index].pc_type = pc_type;
    #ifdef SD_CARD
    Alarm_Create_String_SD(string, &alarm_hr_alg.alarmen.al[alarm_hr_alg.alarmen.index]);
    File_Printf(ALARM_FILE, "ALARM  ON: %02i-%02i-%04i %02i:%02i:%02i %s\r\n", tijd.tm_mday, tijd.tm_mon, tijd.tm_year, tijd.tm_hour, tijd.tm_min, tijd.tm_sec, string);
    #endif // SD_CARD
    if (alarm_hr_alg.alarmen.nr <= ALARM_DISP_VALUES)
      alarm_hr_alg.alarmen.nr++;
    if ((mask & MASK_ALG_DISP) && (alarm_hr_alg.alarmen.nr_actief <= ALARM_DISP_VALUES))
      alarm_hr_alg.alarmen.nr_actief++;
	return(1);
  }
  return(0);
}

static void Alarm_Log_Data_Off_Disp(int al_code, int al_index)
{
int loop = 0;
int index = alarm_hr_alg.alarmen.index;

  do
  {
    if ((alarm_hr_alg.alarmen.al[index].code == al_code) && 
        (alarm_hr_alg.alarmen.al[index].index == al_index) && 
        (alarm_hr_alg.alarmen.al[index].off == 0))
    {
      alarm_hr_alg.alarmen.al[index].off = time(0);
      alarm_hr_alg.alarmen.al[index].state = 0;
      #ifdef SD_CARD
      Alarm_Create_String_SD(string, &alarm_hr_alg.alarmen.al[index]);
      File_Printf(ALARM_FILE, "ALARM OFF: %02i-%02i-%04i %02i:%02i:%02i %4s\r\n", tijd.tm_mday, tijd.tm_mon, tijd.tm_year, tijd.tm_hour, tijd.tm_min, tijd.tm_sec, string);
      #endif // SD_CARD
      if ((alarm_hr_alg.alarmen.nr_actief > 0) &&
          (alarm_hr_alg.alarmen.al[index].mask & MASK_ALG_DISP))
        alarm_hr_alg.alarmen.nr_actief--;
      return;
    }
    index += ALARM_DISP_VALUES - 1;
    index %= ALARM_DISP_VALUES;
    loop++;
  }
  while (loop < ALARM_DISP_VALUES);  
}

static void Alarm_Log_Data_Value_Changed_Disp(int al_code, int al_index, int al_value)
{
int loop = 0;
int index = alarm_hr_alg.alarmen.index;

  do
  {
    if ((alarm_hr_alg.alarmen.al[index].code == al_code) && 
        (alarm_hr_alg.alarmen.al[index].index == al_index) && 
		(alarm_hr_alg.alarmen.al[index].off == 0))
    {
      if(alarm_hr_alg.alarmen.al[index].value != al_value)
	  {
        alarm_hr_alg.alarmen.al[index].value = al_value;
      #ifdef SD_CARD
        Alarm_Create_String_SD(string, &alarm_hr_alg.alarmen.al[index]);
        File_Printf(ALARM_FILE, "ALARM CHANGED: %02i-%02i-%04i %02i:%02i:%02i %4s\r\n", tijd.tm_mday, tijd.tm_mon, tijd.tm_year, tijd.tm_hour, tijd.tm_min, tijd.tm_sec, string);
      #endif // SD_CARD
	  }
      return;
    }
    index += ALARM_DISP_VALUES - 1;
    index %= ALARM_DISP_VALUES;
    loop++;
  }
  while (loop < ALARM_DISP_VALUES);  
}

static void Alarm_Log_Data_Group_Changed(int al_code, int al_index, int al_group)
{
int loop = 0;
int index = alarm_hr_alg.alarmen.index;

  do
  {
    if ((alarm_hr_alg.alarmen.al[index].code == al_code) && 
        (alarm_hr_alg.alarmen.al[index].index == al_index) && 
        (alarm_hr_alg.alarmen.al[index].group != al_group))
    {
      alarm_hr_alg.alarmen.al[index].group = al_group;
#ifdef SD_CARD
      Alarm_Create_String_SD(string, &alarm_hr_alg.alarmen.al[index]);
      File_Printf(ALARM_FILE, "ALARM CHANGED: %02i-%02i-%04i %02i:%02i:%02i %4s\r\n", tijd.tm_mday, tijd.tm_mon, tijd.tm_year, tijd.tm_hour, tijd.tm_min, tijd.tm_sec, string);
#endif // SD_CARD
      return;
    }
    index += ALARM_DISP_VALUES - 1;
    index %= ALARM_DISP_VALUES;
    loop++;
  }
  while (loop < ALARM_DISP_VALUES);  
}

unsigned char CreateAlarm(unsigned char *al, int al_code, int al_index, int al_value, int al_group, unsigned char *al_hard)
{
TAlarm Alarm;
unsigned char mask;

  if (!(*al & AL_ON))
  {
    GetAlarm(al_code, &Alarm);
    mask = Alarm.mask & (MASK_ALG_DISP | MASK_ALG_PC | MASK_AL_HARD | MASK_AL_ZACHT | MASK_AL_ONDERD | MASK_AL_WISSEN);
    if(Alarm_Log_Data_On_Disp(al_code, al_index, al_value, al_group, al_hard, Alarm.Priority, mask, Alarm.pc_type))
	{
      *al |= AL_ON;
      #ifdef CANopen
      CANopenCreateEmcy(al_code, al_index, al_value);
      #endif // CANopen
	  return (1);
	}
  }
  return (0);
}

unsigned char ClearAlarm(unsigned char *al, int al_code, int al_index)
{
TAlarm Alarm;

  if (*al & AL_ON)
  {
    *al = 0;
    GetAlarm(al_code, &Alarm);
    Alarm_Log_Data_Off_Disp(al_code, al_index);
    #ifdef CANopen
    CANopenClearEmcy(al_code, al_index);
    #endif // CANopen
	return (1);
  }
  *al = 0;
  return (0);
} 
     
void CreateAlarmValueChanged(unsigned char *al, int al_code, int al_index, int al_value, int al_group, unsigned char *al_hard)
{
TAlarm Alarm;
unsigned char mask;

  al_hard;
  if (!(*al & AL_ON))
  {
    GetAlarm(al_code, &Alarm);
    mask = Alarm.mask & (MASK_ALG_DISP | MASK_ALG_PC | MASK_AL_HARD | MASK_AL_ZACHT | MASK_AL_ONDERD | MASK_AL_WISSEN);
    if(Alarm_Log_Data_On_Disp(al_code, al_index, al_value, al_group, al_hard, Alarm.Priority, mask, Alarm.pc_type))
      *al |= AL_ON;
  }
  else
  {
    GetAlarm(al_code, &Alarm);
    Alarm_Log_Data_Value_Changed_Disp(al_code, al_index, al_value);
  }
}

void ChangeAlarmGroupBoardIO(s_board_IO_on_off *IO, int al_group)
{
  switch (IO->board_type)
  {
    case IO_06_14_ID: Alarm_Log_Data_Group_Changed(IO_06_14_BOARD_AL, IO->board_nr, al_group); break;
    case IO_12_06_ID: Alarm_Log_Data_Group_Changed(IO_12_06_BOARD_AL, IO->board_nr, al_group); break;
    case IO_08_09_ID: Alarm_Log_Data_Group_Changed(IO_08_09_BOARD_AL, IO->board_nr, al_group); break;
    case IO_05_07_ID: Alarm_Log_Data_Group_Changed(IO_05_07_BOARD_AL, IO->board_nr, al_group); break;
    case IO_07_07_ID: Alarm_Log_Data_Group_Changed(IO_07_07_BOARD_AL, IO->board_nr, al_group); break;
    case IO_H2MC_ID : Alarm_Log_Data_Group_Changed(IO_H2MC_BOARD_AL,  IO->board_nr, al_group); break;
    case IO_H1MC_ID : Alarm_Log_Data_Group_Changed(IO_H1MC_BOARD_AL,  IO->board_nr, al_group); break;
    case IO_EKU_ID  : Alarm_Log_Data_Group_Changed(IO_EKU_BOARD_AL,   IO->board_nr, al_group); break;
  }
}

#ifdef ALARM_TEKST_NAAR_SMARTLINK
void Count_Hard_Alarm_Actief(void) // JP 25-03-10
{
int i;

  alarm_hard_actief_aantal = 0;
  for (i = 0; i < ALARM_DISP_VALUES; i++)
  {
    if ((alarm_hr_alg.alarmen.al[i].code != GEEN_AL) && 
        (alarm_hr_alg.alarmen.al[i].off == 0) &&
        (*alarm_hr_alg.alarmen.al[i].hard != 0))
      alarm_hard_actief_aantal++;
  }    
}
#endif // ALARM_TEKST_NAAR_SMARTLINK

void Get_Alarm_Highest_Priority(s_alarm_disp *al, unsigned char disp)
{
int i;
unsigned char mask;

  if(disp)
	mask = MASK_ALG_DISP;
  else  
    mask = MASK_ALG_PC;
  *al = alarm_geen;
  if (install_flag && (password_enabled & PASSWORD_ENABLED_OPT_MASK))
  {
    al->state = AL_HARD;
    return;
  }
  else 
  {
    for (i = 0; i < ALARM_DISP_VALUES; i++)
    {
      if ((alarm_hr_alg.alarmen.al[i].code != GEEN_AL) && 
          (alarm_hr_alg.alarmen.al[i].off == 0) &&
          (alarm_hr_alg.alarmen.al[i].mask & mask))
      {
        if(((alarm_hr_alg.alarmen.al[i].state == AL_HARD) && (alarm_hr_alg.alarmen.al[i].hard != HARD_ALARM)) ||
           ((alarm_hr_alg.alarmen.al[i].state == AL_ZACHT)&& (alarm_hr_alg.alarmen.al[i].hard != ZACHT_ALARM)))
        {
          alarm_hr_alg.alarmen.al[i].state = (*alarm_hr_alg.alarmen.al[i].hard == 0) ? AL_ZACHT : AL_HARD;
        }

        if (((al->code == SYSTEEM_AL_NEW_OPTION) && (alarm_hr_alg.alarmen.al[i].code == SYSTEEM_AL_NEW_SETPOINT)) ||
            ((al->code == SYSTEEM_AL_NEW_SETPOINT) && (alarm_hr_alg.alarmen.al[i].code == SYSTEEM_AL_NEW_OPTION)))
        {
          al->code = SYSTEEM_AL_NEW_OPTION_SETPOINT;
        }
        else if (al->code == GEEN_AL)
        {
          *al = alarm_hr_alg.alarmen.al[i];
        }
        else if (al->state > alarm_hr_alg.alarmen.al[i].state)
        {
          *al = alarm_hr_alg.alarmen.al[i];
        }
        else if (al->state == alarm_hr_alg.alarmen.al[i].state)  
        {
          if (al->priority > alarm_hr_alg.alarmen.al[i].priority) // hoogste prioriteit
          {
            *al = alarm_hr_alg.alarmen.al[i];
          }
          else if (al->priority == alarm_hr_alg.alarmen.al[i].priority)
          {
            if (al->index > alarm_hr_alg.alarmen.al[i].index) // laagste index
            {
              *al = alarm_hr_alg.alarmen.al[i];
            }
          }
        }
      }
    }
  }
}

time_t TimeAlarmOn(int al_code, int al_index)
{
int loop = 0;
int index = alarm_hr_alg.alarmen.index;

  do
  {
    if ((alarm_hr_alg.alarmen.al[index].code == al_code) && (alarm_hr_alg.alarmen.al[index].index == al_index) && (alarm_hr_alg.alarmen.al[index].off == 0))
    {
      return (time(0) - alarm_hr_alg.alarmen.al[index].on);
    }
    index += ALARM_DISP_VALUES - 1;
    index %= ALARM_DISP_VALUES;
    loop++;
  }
  while (loop < ALARM_DISP_VALUES);  

  return (0);
}

unsigned char AlarmHardGroup(int al_group)
{
int i;

  for (i = 0; i < ALARM_DISP_VALUES; i++)
  {
    if ((alarm_hr_alg.alarmen.al[i].code != GEEN_AL) && (alarm_hr_alg.alarmen.al[i].off == 0) && (*alarm_hr_alg.alarmen.al[i].hard != 0) && (alarm_hr_alg.alarmen.al[i].group == al_group))
	  return (1);
  }
  return (0);
}

unsigned char AlarmActiveGroup(int al_group, int al_code)
{
int i;

  for (i = 0; i < ALARM_DISP_VALUES; i++)
  {
    if ((alarm_hr_alg.alarmen.al[i].code == al_code) && (alarm_hr_alg.alarmen.al[i].off == 0) && (alarm_hr_alg.alarmen.al[i].group == al_group))
	  return (1);
  }
  return (0);
}


void Alarm_Control_Data_Algemeen(void)
{
unsigned char dummy_al = 0;
unsigned char *ptr;
int loop;
time_t t;

  if ((alarm_hr_alg.alarmen.control != 0x5A5A) ||
      (alarm_hr_alg.last_al != 0x5A))
  {
    t = 0;
    for (loop = 0; loop < ALARM_DISP_VALUES; loop++)
    {
      alarm_hr_alg.alarmen.al[loop] = alarm_geen;
      alarm_hr_alg.alarmen.al[loop].off = alarm_hr_alg.alarmen.al[loop].on = t;
    }
    ptr = &alarm_hr_alg.first_al;
    ptr++;
    do
    {
      *ptr = 0;
      ptr++;
    }
    while (ptr != &alarm_hr_alg.last_al);
    alarm_hr_alg.alarmen.index = 0;
    alarm_hr_alg.alarmen.nr = 0;
    alarm_hr_alg.alarmen.nr_actief = 0;
    alarm_hr_alg.alarmen.control = 0x5A5A;
    alarm_hr_alg.last_al = 0x5A;
    CreateAlarm(&dummy_al, SYSTEEM_AL_ALARM_GERESET, 0, 0, 0, ZACHT_ALARM);
    ClearAlarm(&dummy_al, SYSTEEM_AL_ALARM_GERESET, 0);
  }  
}

void Alarm_Log_Opstart_Algemeen(void)
{
unsigned char dummy_al = 0;

  CreateAlarm(&dummy_al, SYSTEEM_AL_OPSTART, 0, 0, 0, ZACHT_ALARM);
  ClearAlarm(&dummy_al, SYSTEEM_AL_OPSTART, 0);
}

void Alarm_Reset_Data_PC_Algemeen(void)
{
unsigned char dummy_al = 0;
int loop;

  for (loop = 0; loop < ALARM_DISP_VALUES; loop++)
  {
    if (alarm_hr_alg.alarmen.al[loop].mask & MASK_ALG_PC) 
    {
      if(alarm_hr_alg.alarmen.al[loop].off)
      {
        alarm_hr_alg.alarmen.al[loop].mask &= (~MASK_ALG_PC);
      }
      else if(Alarm_Reset(&alarm_hr_alg.alarmen.al[loop]))
      {
        alarm_hr_alg.alarmen.al[loop].mask &= (~MASK_ALG_PC);
      }
    }
  }
  CreateAlarm(&dummy_al, SYSTEEM_AL_ALARM_GERESET, 0, 0, 0, ZACHT_ALARM);
  ClearAlarm(&dummy_al, SYSTEEM_AL_ALARM_GERESET, 0);
}

void Alarm_Reset_Data_Algemeen(void)
{
  alarm_hr_alg.alarmen.control = 0;
  Alarm_Control_Data_Algemeen();
}

unsigned char Alarm_Reset_Rosenberg(s_alarm_disp *al)
{
  int alarmCode = (al->code & 0x7FFF) % 100;
  char deviceAddress = 0;
  switch (alarmCode)
  {
    case MB_DEVICE_COMMUNICATION_AL:
    case MB_DEVICE_UMIN_AL:
    case MB_DEVICE_UMAX_AL:                     
    case MB_DEVICE_OVERCURRENT_AL:
    case MB_DEVICE_ELECTRONIC_BOX_OVER_HEAT_AL:
    case MB_DEVICE_INPUT_PHASE_ERROR_AL:       
    case MB_DEVICE_LOCKED_ROTOR_AL:            
    case MB_DEVICE_WRONG_DIRECTION_AL:         
    case MB_DEVICE_MOTOR_PHASE_ERROR_AL:  
    case MB_DEVICE_24V_SUPPLY_OVERLOADED_AL:
    case MB_DEVICE_THERMAL_MOTOR_AL:	
    case MB_DEVICE_UNDERSPEED_AL:	
      deviceAddress = opt_app.Motorgroup[al->group-1].FirstAddress + (al->index + 1 - opt_app.Motorgroup[al->group-1].FirstNumber);
      mbResetDevice(deviceAddress);
   	  return (1); 
  }

  return (0); 
}

unsigned char Alarm_Reset_Nicotra_Gebhardt(s_alarm_disp *al)
{
  int alarmCode = (al->code & 0x7FFF) % 100;
  char deviceAddress = 0;
  switch (alarmCode)
  {
    case MB_DEVICE_COMMUNICATION_AL:
    case MB_DEVICE_MEMORY_ERROR_AL:
    case MB_DEVICE_SHORT_CIRCUIT_AL:
    case MB_DEVICE_LOSS_OF_SYNCHRONISM_AL:
    case MB_DEVICE_INPUT_VOLTAGE_ERROR_AL:
    case MB_DEVICE_UMAX_AL:
    case MB_DEVICE_UMIN_AL:
    case MB_DEVICE_INPUT_RELAY_NOT_CLOSED_AL:
    case MB_DEVICE_INPUT_PHASE_ERROR_AL: 
    case MB_DEVICE_HIGH_STARTING_CURRENT_AL:
    case MB_DEVICE_ELECTRONIC_BOX_OVER_HEAT_AL:
      deviceAddress = opt_app.Motorgroup[al->group-1].FirstAddress + (al->index + 1 - opt_app.Motorgroup[al->group-1].FirstNumber);
      mbResetDevice(deviceAddress);
      return (1); 
  }

  return (0); 
}


//*****************************************************************************
// alarmen die via aktueel alarm scherm gereset kunnen worden
// ****************************************************************************
unsigned char Alarm_Reset(s_alarm_disp *al)
// wordt aangeroepen vanuit Alarm_Return (als toets wordt gedrukt en alarm scherm wordt weergegeven)
//                          Enter_Alarmen_Actief (als enter wordt gedrukt en alarm actiefscherm wordt weergegeven)
{
  switch (al->code & 0x7FFF)
  {
    case ORION_OFF_AL:
      val_hr_alg.orion_switched_off = 0; 
      return (1);
    case SYSTEEM_AL_OPT:
      ClearAlarm(&alarm_hr_alg.opt_al, SYSTEEM_AL_OPT, al->index);
      return (1);
    case SYSTEEM_AL_SETP:
      ClearAlarm(&alarm_hr_alg.setp_al, SYSTEEM_AL_SETP, al->index);
      return(1);
    case SYSTEEM_AL_VAL_HR:
      ClearAlarm(&alarm_hr_alg.val_hr_al, SYSTEEM_AL_VAL_HR, al->index);
      return(1);
    case SYSTEEM_AL_I2C0:
      ClearAlarm(&alarm_hr_alg.I2C0_al, SYSTEEM_AL_I2C0, al->index);
      return (1);
    case SYSTEEM_AL_EEP:
      ClearAlarm(&alarm_hr_alg.EEP_al, SYSTEEM_AL_EEP, al->index);
      return (1);
    case SYSTEEM_AL_EEP_TAAL:
      ClearAlarm(&alarm_hr_alg.EEP_taal_al, SYSTEEM_AL_EEP_TAAL, al->index);
      return (1);
    case SYSTEEM_AL_RTC:
      ClearAlarm(&alarm_hr_alg.RTC_al, SYSTEEM_AL_RTC, al->index);
      return (1);
    case SYSTEEM_AL_TIMER_1ms:
      ClearAlarm(&alarm_hr_alg.timer_1ms_al, SYSTEEM_AL_TIMER_1ms, al->index);
      return (1);
    case SYSTEEM_AL_HTRAP:
      ClearAlarm(&alarm_hr_alg.htrap_al, SYSTEEM_AL_HTRAP, al->index);
      return (1);
    case SYSTEEM_AL_PLL:
      ClearAlarm(&alarm_hr_alg.PLL_al, SYSTEEM_AL_PLL, al->index);
      return (1);
    case ORION_ON_AL:  // niet nodig geen temperaturen
      ClearAlarm(&alarm_hr_alg.orion_on_al, ORION_ON_AL, al->index);
      return (1);
    case SYSTEEM_AL_RESTORE_OPTION_SETPOINT_FAILED:
	  ClearAlarm(&alarm_hr_alg.restore_option_setpoint_failed_al, SYSTEEM_AL_RESTORE_OPTION_SETPOINT_FAILED, al->index);
      return (1);      
    case SYSTEEM_AL_NEW_OPTION_SETPOINT:
	  ClearAlarm(&alarm_hr_alg.new_option_from_pc_al, SYSTEEM_AL_NEW_OPTION, al->index);
	  ClearAlarm(&alarm_hr_alg.new_setpoint_from_pc_al, SYSTEEM_AL_NEW_SETPOINT, al->index);
      return (1);      
    case SYSTEEM_AL_NEW_OPTION:
	  ClearAlarm(&alarm_hr_alg.new_option_from_pc_al, SYSTEEM_AL_NEW_OPTION, al->index);
      return (1);      
    case SYSTEEM_AL_NEW_SETPOINT:
      ClearAlarm(&alarm_hr_alg.new_setpoint_from_pc_al, SYSTEEM_AL_NEW_SETPOINT, al->index);
      return (1);
    case LUCHTMENGKAST_VORST_AL:
      ClearAlarm(&alarm_hr_alg.Luchtmengkast[al->index].Vorst, LUCHTMENGKAST_VORST_AL, al->index);
	  return (1);
  }	

  if (Alarm_Reset_Rosenberg(al))
    return (1);

  if (Alarm_Reset_Nicotra_Gebhardt(al))
    return (1);

  return (0);
}

void Alarm_Return(void)
{
  Prev_Screen();
  screen_ptr->value = al_save_screen_val;
  screen_ptr->change_flag = al_save_screen_change_flag;
}

//*****************************************************************************
void Check_Orion_Off_Alarm(void)
{
  if (setp_alg.regelaar_on == 0)
    CreateAlarm(&alarm_hr_alg.orion_off_al,	ORION_OFF_AL, 0, 0, 0, HARD_ALARM); 
  else
  {
    ClearAlarm(&alarm_hr_alg.orion_off_al,	ORION_OFF_AL, 0); 
  }    		   
}

void Controll_IO_Alarm_Contact(void)
{
int loop;

  for (loop = 0; loop < IO_06_14_MAX; loop++)
  {
    if ((opt_io.IO_06_14[loop].board_component.option & 0x00FF) && (opt_io.IO_06_14[loop].alarm))
    {
      ClearAlarm(&alarm_hr_alg.geen_alarm_contact_al, SYSTEEM_AL_GEEN_ALARM_CONTACT, 0);
      return;  
    }    
  }  
  for (loop = 0; loop < IO_12_06_MAX; loop++)
  {
    if ((opt_io.IO_12_06[loop].board_component.option & 0x00FF) && (opt_io.IO_12_06[loop].alarm))
    {
      ClearAlarm(&alarm_hr_alg.geen_alarm_contact_al, SYSTEEM_AL_GEEN_ALARM_CONTACT, 0);
      return;  
    }    
  }  
  for (loop = 0; loop < IO_08_09_MAX; loop++)
  {
    if ((opt_io.IO_08_09[loop].board_component.option & 0x00FF) && (opt_io.IO_08_09[loop].alarm))
    {
      ClearAlarm(&alarm_hr_alg.geen_alarm_contact_al, SYSTEEM_AL_GEEN_ALARM_CONTACT, 0);
      return;  
    }    
  }  
  for (loop = 0; loop < IO_EKU_MAX; loop++)
  {
    if ((opt_io.IO_EKU[loop].board_component.option & 0x00FF) && (opt_io.IO_EKU[loop].alarm))
    {
      ClearAlarm(&alarm_hr_alg.geen_alarm_contact_al, SYSTEEM_AL_GEEN_ALARM_CONTACT, 0);
      return;  
    }    
  }  
  for (loop = 0; loop < IO_H2MC_MAX; loop++)
  {
    if ((opt_io.IO_H2MC[loop].board_component.option & 0x00FF) && (opt_io.IO_H2MC[loop].alarm))
    {
      ClearAlarm(&alarm_hr_alg.geen_alarm_contact_al, SYSTEEM_AL_GEEN_ALARM_CONTACT, 0);
      return;  
    }    
  }  
  for (loop = 0; loop < IO_H1MC_MAX; loop++)
  {
    if ((opt_io.IO_H1MC[loop].board_component.option & 0x00FF) && (opt_io.IO_H1MC[loop].alarm))
    {
      ClearAlarm(&alarm_hr_alg.geen_alarm_contact_al, SYSTEEM_AL_GEEN_ALARM_CONTACT, 0);
      return;  
    }    
  }  
  for (loop = 0; loop < IO_05_07_MAX; loop++)
  {
    if ((opt_io.IO_05_07[loop].board_component.option & 0x00FF) && (opt_io.IO_05_07[loop].alarm))
    {
      ClearAlarm(&alarm_hr_alg.geen_alarm_contact_al, SYSTEEM_AL_GEEN_ALARM_CONTACT, 0);
      return;  
    }    
  }  
  for (loop = 0; loop < IO_07_07_MAX; loop++)
  {
    if ((opt_io.IO_07_07[loop].board_component.option & 0x00FF) && (opt_io.IO_07_07[loop].alarm))
    {
      ClearAlarm(&alarm_hr_alg.geen_alarm_contact_al, SYSTEEM_AL_GEEN_ALARM_CONTACT, 0);
      return;  
    }    
  }  
  CreateAlarm(&alarm_hr_alg.geen_alarm_contact_al, SYSTEEM_AL_GEEN_ALARM_CONTACT, 0, 0, 0, ZACHT_ALARM);
}

void Alarm_Proc(void) // elke seconde aanroepen (elke seconde alarm_switch zetten)
{
unsigned char hard_alarm;
static unsigned int can_alarm_code_prev = 0;
#ifdef ALARM_TEKST_NAAR_SMARTLINK
static unsigned int can_alarm_hard_actief_aantal_prev = 0;
#endif // ALARM_TEKST_NAAR_SMARTLINK 

  Alarm_Control_Data_Algemeen();
  Controll_IO_Alarm_Contact();
  Check_Orion_Off_Alarm();
  IO_06_14_Alarm_All_Boards();
  IO_12_06_Alarm_All_Boards();
  IO_08_09_Alarm_All_Boards();
  IO_EKU_Alarm_All_Boards();
  IO_H2MC_Alarm_All_Boards();
  IO_H1MC_Alarm_All_Boards();
  IO_05_07_Alarm_All_Boards();
  IO_07_07_Alarm_All_Boards();

  if (alarm_disp_act.code == GEEN_AL)
    alarm_disp_blocked = 0;

  Get_Alarm_Highest_Priority(&alarm_disp_act, 1);

  if(alarm_disp_act.code == GEEN_AL && 
     (!install_flag || ((password_enabled & PASSWORD_ENABLED_OPT_MASK) == 0)) &&
     (alarm_hr_alg.computer_al & AL_ON))  
  {
    alarm_disp_act.code = COMPUTER_AL;
    alarm_disp_act.hard = ZACHT_ALARM;
    alarm_disp_act.state = AL_ZACHT;
    alarm_disp_act.value = alarm_hr_alg.computer_al_nr;
    alarm_disp_act.index = 0;
    alarm_disp_act.pc_type = ALARM_VAL_0;
  }
  if (alarm_disp_act.state == AL_HARD)
  {
    if (alarm_hr_alg.alarmen.nr_actief > 1)
      alarm_disp_ico = 2;
    else
      alarm_disp_ico = 1;
  }
  else if (alarm_disp_act.state == AL_ZACHT)
  {
    if (alarm_hr_alg.alarmen.nr_actief > 1)
      alarm_disp_ico = 4;
    else
      alarm_disp_ico = 3;
  }
  else if (alarm_disp_act.state == AL_ONDERDRUKT)
  {
    alarm_disp_act = alarm_geen;
    if (alarm_hr_alg.alarmen.nr_actief > 1)
      alarm_disp_ico = 6;
    else
      alarm_disp_ico = 5;
  }  
  else
  {
    alarm_disp_act = alarm_geen;
    alarm_disp_ico = 0;
  }

  if (alarm_disp_act.state == AL_ZACHT)
    IO_Set_Dig_Out(&opt_app.Alarm.DigOutZacht, 0);
  else
    IO_Set_Dig_Out(&opt_app.Alarm.DigOutZacht, 1);

  if (alarm_disp_act.code == GEEN_AL)
  {
    if ((unsigned long)screen_ptr == (unsigned long)&screen_alarm_0)
    {
      Prev_Screen();
	  lcd_refresh_fast_switch = 1;
      screen_ptr->value = al_save_screen_val;
      screen_ptr->change_flag = al_save_screen_change_flag;
    }
	laatste_alarm_actueel = alarm_disp_act; // om te zorgen dat laatste alarm weer naar voren komt
	                                        // als het binnen een minuut weer optreed
  }
  else if (((unsigned long)screen_ptr != (unsigned long)&screen_alarm_0) &&
           ((unsigned long)screen_ptr != (unsigned long)&screen_start))
  {
    if (alarm_disp_blocked)
    {
      alarm_disp_blocked--;
      if (alarm_disp_act.code == COMPUTER_AL)
      {
        alarm_disp_ico = 0;
      }  
    }  
    if ((alarm_disp_act.code != laatste_alarm_actueel.code) ||
	    (alarm_disp_act.index != laatste_alarm_actueel.index)||
        (alarm_disp_blocked == 0))
    {
      al_save_screen_change_flag = screen_ptr->change_flag;
      al_save_screen_val = screen_ptr->value;

      If_Exist_Goto_Screen_Alarm_0();
	  lcd_refresh_fast_switch = 1;
      alarm_disp_blocked = 0;
    }
  }	
  hard_alarm = (alarm_disp_act.state == AL_HARD) ? 1 : 0;   

  IO_06_14_Alarm_Check(hard_alarm);  // routine voor doorgeven alarm naar IO_06_14_borden
  IO_12_06_Alarm_Check(hard_alarm);  // routine voor doorgeven alarm naar IO_12_06_borden
  IO_08_09_Alarm_Check(hard_alarm);  // routine voor doorgeven alarm naar IO_08_09_borden
  IO_EKU_Alarm_Check(hard_alarm);    // routine voor doorgeven alarm naar IO_EKU_borden
  IO_H2MC_Alarm_Check(hard_alarm);   // routine voor doorgeven alarm naar IO_H2MC_borden
  IO_H1MC_Alarm_Check(hard_alarm);   // routine voor doorgeven alarm naar IO_H1MC_borden
  IO_05_07_Alarm_Check(hard_alarm);  // routine voor doorgeven alarm naar IO_05_07_borden
  IO_07_07_Alarm_Check(hard_alarm);  // routine voor doorgeven alarm naar IO_05_07_borden
  //***************************************************************************
  // updating alarm to transmit over can backbone
  //***************************************************************************
  Get_Alarm_Highest_Priority(&alarm_disp_pc, 0);
#ifdef ALARM_TEKST_NAAR_SMARTLINK
  Count_Hard_Alarm_Actief();
  if ((alarm_disp_pc.state == AL_HARD) && (alarm_disp_pc.hard != NULL) && (*alarm_disp_pc.hard))
  {
    if ((alarm_disp_pc.code != can_alarm_code_prev) ||
        (alarm_hard_actief_aantal != can_alarm_hard_actief_aantal_prev))
    {
      alarm_tekstread_mask = 0x8007;
      Can_Backbone_Set_Alarm_And_Send(&can_backbone_appl_node_alg.transmit_alarm,
                                      alarm_disp_pc.code,  // alarm_code
                                      alarm_tekstread_mask, // alarm masker om aan tegeven dat er iets gewijzigd is en de alarmtekst opnieuw opgehaald moet worden ( alarm_disp_pc.value,  // alarm_value )
                                      module.computer,  // computer_type
                                      module.soort); // computer_soort
      can_alarm_code_prev = alarm_disp_pc.code;
      can_alarm_hard_actief_aantal_prev = alarm_hard_actief_aantal;
    }                                  
  }
  else
  {
    if (can_alarm_code_prev)
    {
      alarm_tekstread_mask = 0x8000;
      Can_Backbone_Set_Alarm_And_Send(&can_backbone_appl_node_alg.transmit_alarm,
                                      0,  // alarm_code
                                      alarm_tekstread_mask,  // alarm_value
                                      module.computer,  // computer_type
                                      module.soort); // computer_soort
      can_alarm_code_prev = 0;
      can_alarm_hard_actief_aantal_prev = 0;
    }                                  
  }
#else // ALARM_TEKST_NAAR_SMARTLINK
  if ((alarm_disp_pc.state == AL_HARD) && (alarm_disp_pc.hard != NULL) && (*alarm_disp_pc.hard))
  {
    if (alarm_disp_pc.code != can_alarm_code_prev)
    {
      Can_Backbone_Set_Alarm_And_Send(&can_backbone_appl_node_alg.transmit_alarm,
                                      alarm_disp_pc.code,  // alarm_code
                                      alarm_disp_pc.value,  // alarm_value
                                      module.computer,  // computer_type
                                      module.soort); // computer_soort
      can_alarm_code_prev = alarm_disp_pc.code;
    }                                  
  }
  else
  {
    if (can_alarm_code_prev)
    {
      Can_Backbone_Set_Alarm_And_Send(&can_backbone_appl_node_alg.transmit_alarm,
                                      0,  // alarm_code
                                      0,  // alarm_value
                                      module.computer,  // computer_type
                                      module.soort); // computer_soort
      can_alarm_code_prev = 0;
    }                                  
  }
#endif // ALARM_TEKST_NAAR_SMARTLINK
  alarm_switch = 0;
}