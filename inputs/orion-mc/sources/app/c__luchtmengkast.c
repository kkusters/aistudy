// C__LUCHTMENGKAST.C  

#include <stdlib.h>
#include <string.h>

#include "ch_define.h"

#include "ch_alarm.h"
#include "ch_alg.h"
#include "ch_ds301.h"
#include "ch_ds401.h"
#include "ch_mb_device.h"
#include "ch_IO.h"
#include "ch_timer.h"
#include "ch_luchtmengkast.h"

//==============================================================================
//------------------------ Luchtmengkast - Defines -----------------------------
//==============================================================================
#define MAX_CORRECTIE      20
#define MAX_AFWIJKING_DICHT 3
#define CORRECTIE_MARGE     2

#define SERVO_BUITENKLEP  1
#define SERVO_BINNENKLEP  2
#define SERVO_VERWARMING  3
#define SERVO_INBLAASVENT 4
#define SERVO_AFBLAASVENT 5

//==============================================================================
//------------------------ Luchtmengkast - Globals -----------------------------
//==============================================================================
static TTimer Timer_100ms;
static unsigned char ClosedLoopOpenLoop;
static unsigned char RampUp;
static unsigned char RampDown;
static unsigned char PowerFactor;
TLuchtmengkast      Luchtmengkast[MAX_LUCHTMENGKAST];
TLuchtmengkastGroep LuchtmengkastGroep[MAX_LUCHTMENGKAST_GROEP];

//==============================================================================
//------------------------ Luchtmengkast - Initialisation ----------------------
//==============================================================================
//------------------------------------------------------------------------------
void LuchtmengkastInit(void)
{
unsigned char i;

  memset(LuchtmengkastGroep, 0, sizeof(LuchtmengkastGroep));
  memset(Luchtmengkast,      0, sizeof(Luchtmengkast));

  TimerSet(&Timer_100ms, TIMER_100MS);

  for (i = 0; i < MAX_LUCHTMENGKAST_GROEP; i++)
  {
    TimerSet(&LuchtmengkastGroep[i].AfblaasventTimer_1s, TIMER_1SEC);
    TimerSet(&LuchtmengkastGroep[i].BovenklepTimer_1s,   TIMER_1SEC);
  }
  for (i = 0; i < MAX_LUCHTMENGKAST; i++)
  {
    TimerSet(&Luchtmengkast[i].Buitenklep.Timer_1min,  TIMER_1MIN);
    TimerSet(&Luchtmengkast[i].Binnenklep.Timer_1min,  TIMER_1MIN);
    TimerSet(&Luchtmengkast[i].Verwarming.Timer_1min,  TIMER_1MIN);
    TimerSet(&Luchtmengkast[i].Afblaasvent.Timer_1min, TIMER_1MIN);
    TimerSet(&Luchtmengkast[i].Inblaasvent.Timer_1min, TIMER_1MIN);

    TimerSet(&Luchtmengkast[i].Buitenklep.Timer_5s,  5 * TIMER_1SEC);
    TimerSet(&Luchtmengkast[i].Binnenklep.Timer_5s,  5 * TIMER_1SEC);
    TimerSet(&Luchtmengkast[i].Verwarming.Timer_5s,  5 * TIMER_1SEC);
    TimerSet(&Luchtmengkast[i].Afblaasvent.Timer_5s, 5 * TIMER_1SEC);
    TimerSet(&Luchtmengkast[i].Inblaasvent.Timer_5s, 5 * TIMER_1SEC);

    TimerSet(&Luchtmengkast[i].Timer_1s, TIMER_1SEC);
  }
}

//==============================================================================
//------------------------ Luchtmengkast - Alarm -------------------------------
//==============================================================================
//------------------------------------------------------------------------------
static unsigned char LuchtmengkastGroepCheckManualAlarm(unsigned char i)
{
  if (val_hr_alg.LuchtmengkastGroep[i].OperationMode == omManual)
  {
    CreateAlarm(&alarm_hr_alg.LuchtmengkastGroep[i].Manual, LUCHTMENGKASTGROEP_MANUAL_AL, i, i + 1, 0, &alarm_hr_alg.LuchtmengkastGroep[i].AlarmHard);
    if ((setp_alg.LuchtmengkastGroep[i].DelayAlarmManual != 0) && (TimeAlarmOn(LUCHTMENGKASTGROEP_MANUAL_AL, i) > (setp_alg.LuchtmengkastGroep[i].DelayAlarmManual * 60)))
      return (1); // Hard alarm
  }
  else
    ClearAlarm(&alarm_hr_alg.LuchtmengkastGroep[i].Manual, LUCHTMENGKASTGROEP_MANUAL_AL, i);

  return (0);
}

static unsigned char LuchtmengkastGroepCheckVorstAlarm(unsigned char i)
{
int nr;
unsigned char Vorst = 0;

  for (nr = 0; nr < MAX_LUCHTMENGKAST; nr++)
  {
    if (opt_app.Luchtmengkast[nr].Enabled && (opt_app.Luchtmengkast[nr].Groep == i))
    {
      if (alarm_hr_alg.Luchtmengkast[nr].Vorst)
        Vorst = 1;
    }
  }
  if (opt_app.LuchtmengkastGroep[i].Binnenklep.TypeSturing == TYPE_STURING_CANOPEN)
  {
    DS401_SetDigitalInput((i * 4) + 1, Vorst);
  }
  return (Vorst);
}

static unsigned char LuchtmengkastGroepCheckDrukverschilAlarm(unsigned char i)
{
int nr;
unsigned char Alarm = 0;

  for (nr = 0; nr < MAX_LUCHTMENGKAST; nr++)
  {
    if (opt_app.Luchtmengkast[nr].Enabled && (opt_app.Luchtmengkast[nr].Groep == i))
    {
      if (alarm_hr_alg.Luchtmengkast[nr].Drukverschil)
        Alarm = 1;
    }
  }
  if (opt_app.LuchtmengkastGroep[i].Binnenklep.TypeSturing == TYPE_STURING_CANOPEN)
  {
    DS401_SetDigitalInput((i * 4) + 2, !Alarm);
  }
  return (0); // zacht alarm
}

static unsigned char LuchtmengkastGroepCheckAlgemeenAlarm(unsigned char i)
{
  if (opt_app.LuchtmengkastGroep[i].Binnenklep.TypeSturing == TYPE_STURING_CANOPEN)
  {
    DS401_SetDigitalInput((i * 4) + 3, !alarm_hr_alg.LuchtmengkastGroep[i].AlarmHard);
  }
  return (0); // geen fysiek alarm
}

static unsigned char LuchtmengkastGroepCheckThermischAlarm(unsigned char i)
{
  if (opt_app.LuchtmengkastGroep[i].Binnenklep.TypeSturing == TYPE_STURING_CANOPEN)
  {
    DS401_SetDigitalInput((i * 4) + 0, 1);
  }
  return (0); // geen fysiek alarm
}

static unsigned char LuchtmengkastCheckManualAlarm(unsigned char i)
{
  if (val_hr_alg.Luchtmengkast[i].OperationMode == omManual)
  {
    CreateAlarm(&alarm_hr_alg.Luchtmengkast[i].Manual, LUCHTMENGKAST_MANUAL_AL, i, i + 1, 0, &alarm_hr_alg.LuchtmengkastGroep[opt_app.Luchtmengkast[i].Groep].AlarmHard);
    if ((setp_alg.LuchtmengkastGroep[opt_app.Luchtmengkast[i].Groep].DelayAlarmManual != 0) && (TimeAlarmOn(LUCHTMENGKAST_MANUAL_AL, i) > (setp_alg.LuchtmengkastGroep[opt_app.Luchtmengkast[i].Groep].DelayAlarmManual * 60)))
      return (1); // Hard alarm
  }
  else
    ClearAlarm(&alarm_hr_alg.Luchtmengkast[i].Manual, LUCHTMENGKAST_MANUAL_AL, i);

  return (0);
}

static unsigned char LuchtmengkastCheckVorstAlarmMaster(unsigned char i)
{
int nr;

  for (nr = 0; nr < MAX_LUCHTMENGKAST; nr++)
  {
    if (opt_app.Luchtmengkast[nr].Enabled && opt_app.Luchtmengkast[nr].DigInVorst.board_type && (opt_app.Luchtmengkast[nr].Groep == opt_app.Luchtmengkast[i].Groep))
    {
      if (alarm_hr_alg.Luchtmengkast[nr].Vorst)
        return (1);
      if (alarm_hr_alg.Luchtmengkast[nr].InblaasventTargetNotReached)
        return (1);
      switch (opt_app.Luchtmengkast[nr].Inblaasvent.TypeSturing)
      {
        case TYPE_STURING_EBMBUS             :
        case TYPE_STURING_EBM_MODBUS         :
        case TYPE_STURING_EC_BLUE_MODBUS     :
        case TYPE_STURING_EC_BLUE_PREMIUM    :
        case TYPE_STURING_MB_ROSENBERG       :
        case TYPE_STURING_MB_CLIMAFAN        :
        case TYPE_STURING_MB_ROSENBERG_GEN3  :	
        case TYPE_STURING_MB_NICOTRA_GEBHARDT:	
          if (mbDeviceAlarmActive(opt_app.Luchtmengkast[nr].Inblaasvent.Adres))
            return (1);
      }
    }
  }
  return (0);
}

static unsigned char LuchtmengkastCheckVorstAlarm(unsigned char i)
{
  if (opt_app.Luchtmengkast[i].DigInVorst.board_type)
  {
    if (!IO_Get_Dig_In_Status(&opt_app.Luchtmengkast[i].DigInVorst))
      CreateAlarm(&alarm_hr_alg.Luchtmengkast[i].Vorst, LUCHTMENGKAST_VORST_AL, i, i + 1, 0, HARD_ALARM);
    val_hr_alg.Luchtmengkast[i].VorstBewaking = alarm_hr_alg.Luchtmengkast[i].Vorst;
  }
  else
  {
    val_hr_alg.Luchtmengkast[i].VorstBewaking = LuchtmengkastCheckVorstAlarmMaster(i);
  }
  if (alarm_hr_alg.Luchtmengkast[i].Vorst)
    return (1);
  else
    return (0);
}

static unsigned char LuchtmengkastCheckExternAlarm(unsigned char i)
{
unsigned char Alarm = 0;

  if (opt_app.Luchtmengkast[i].DigInAlarm.board_type)
  {
    if (!IO_Get_Dig_In_Status(&opt_app.Luchtmengkast[i].DigInAlarm))
      Alarm = 1;
  }

  if (Alarm)
  {
    CreateAlarm(&alarm_hr_alg.Luchtmengkast[i].Extern, LUCHTMENGKAST_EXTERN_AL, i, i + 1, 0, HARD_ALARM);
    return (1);
  }
  else
    ClearAlarm(&alarm_hr_alg.Luchtmengkast[i].Extern, LUCHTMENGKAST_EXTERN_AL, i);

  return (0);
}

static unsigned char LuchtmengkastCheckDrukverschilAlarm(unsigned char i)
{
unsigned char Alarm = 0;

  if (opt_app.Luchtmengkast[i].DigInDrukverschil.board_type)
  {
    if (!IO_Get_Dig_In_Status(&opt_app.Luchtmengkast[i].DigInDrukverschil))
      Alarm = 1;
  }

  if (Alarm)
  {
    CreateAlarm(&alarm_hr_alg.Luchtmengkast[i].Drukverschil, LUCHTMENGKAST_DRUKVERSCHIL_AL, i, i + 1, 0, ZACHT_ALARM);
//    return (1); // zacht alarm
  }
  else
    ClearAlarm(&alarm_hr_alg.Luchtmengkast[i].Drukverschil, LUCHTMENGKAST_DRUKVERSCHIL_AL, i);

  return (0);
}

static unsigned char LuchtmengkastCheckBuitenklepAlarm(unsigned char i)
{
unsigned char AlarmPosition;
int Diff;
int Setp;

  Setp = val_hr_alg.Luchtmengkast[i].Buitenklep.Setpoint;
  Diff = abs_int((int)val_hr_alg.Luchtmengkast[i].Buitenklep.Actual - (int)val_hr_alg.Luchtmengkast[i].Buitenklep.Setpoint);
  AlarmPosition = setp_alg.LuchtmengkastGroep[opt_app.Luchtmengkast[i].Groep].DiffPositionAlarm;
  
//  if (((Setp == 0) && (Diff == 0)) || ((Setp > 0) && (Diff < setp_alg.LuchtmengkastGroep[opt_app.Luchtmengkast[i].Groep].DiffPositionAlarm)))
  if ((AlarmPosition == 0) || ((Setp == 0) && (Diff < MAX_AFWIJKING_DICHT)) || ((Setp > 0) && (Diff < AlarmPosition)))
  {
    Luchtmengkast[i].Buitenklep.AlarmTimer = 0;
    TimerRestart(&Luchtmengkast[i].Buitenklep.Timer_1min);
    if (opt_app.Luchtmengkast[i].TypeKlep == TYPE_KLEP_BINNEN_BUITEN)
      ClearAlarm(&alarm_hr_alg.Luchtmengkast[i].BuitenklepTargetNotReached, LUCHTMENGKAST_BUITENKLEP_TARGET_NOT_REACHED_AL, i);
    else
      ClearAlarm(&alarm_hr_alg.Luchtmengkast[i].RecircklepTargetNotReached, LUCHTMENGKAST_RECIRCKLEP_TARGET_NOT_REACHED_AL, i);
  }
  else if (Luchtmengkast[i].Buitenklep.AlarmTimer >= setp_alg.LuchtmengkastGroep[opt_app.Luchtmengkast[i].Groep].TimePositionAlarm)
  {
    if (opt_app.Luchtmengkast[i].TypeKlep == TYPE_KLEP_BINNEN_BUITEN)
      CreateAlarm(&alarm_hr_alg.Luchtmengkast[i].BuitenklepTargetNotReached, LUCHTMENGKAST_BUITENKLEP_TARGET_NOT_REACHED_AL, i, i + 1, 0, HARD_ALARM);
    else
      CreateAlarm(&alarm_hr_alg.Luchtmengkast[i].RecircklepTargetNotReached, LUCHTMENGKAST_RECIRCKLEP_TARGET_NOT_REACHED_AL, i, i + 1, 0, HARD_ALARM);
    return (1);
  }
  else if (TimerExpired(&Luchtmengkast[i].Buitenklep.Timer_1min))
  {
    Luchtmengkast[i].Buitenklep.AlarmTimer++;
    TimerReset(&Luchtmengkast[i].Buitenklep.Timer_1min);
  }
  return (0);
}

static unsigned char LuchtmengkastCheckBinnenklepAlarm(unsigned char i)
{
unsigned char AlarmPosition;
int Diff;
int Setp;

  if (opt_app.Luchtmengkast[i].BovenklepEnabled || (opt_app.Luchtmengkast[i].TypeKlep == TYPE_KLEP_BINNEN_BUITEN))
  {
    Setp = val_hr_alg.Luchtmengkast[i].Binnenklep.Setpoint;
    Diff = abs_int((int)val_hr_alg.Luchtmengkast[i].Binnenklep.Actual - (int)val_hr_alg.Luchtmengkast[i].Binnenklep.Setpoint);
    AlarmPosition = setp_alg.LuchtmengkastGroep[opt_app.Luchtmengkast[i].Groep].DiffPositionAlarm;
  
//    if (((Setp == 0) && (Diff == 0)) || ((Setp > 0) && (Diff < setp_alg.LuchtmengkastGroep[opt_app.Luchtmengkast[i].Groep].DiffPositionAlarm)))
    if ((AlarmPosition == 0) || ((Setp == 0) && (Diff < MAX_AFWIJKING_DICHT)) || ((Setp > 0) && (Diff < AlarmPosition)))
    {
      Luchtmengkast[i].Binnenklep.AlarmTimer = 0;
      TimerRestart(&Luchtmengkast[i].Binnenklep.Timer_1min);
      if (opt_app.Luchtmengkast[i].TypeKlep == TYPE_KLEP_BINNEN_BUITEN)
        ClearAlarm(&alarm_hr_alg.Luchtmengkast[i].BinnenklepTargetNotReached, LUCHTMENGKAST_BINNENKLEP_TARGET_NOT_REACHED_AL, i);
      else
        ClearAlarm(&alarm_hr_alg.Luchtmengkast[i].BovenklepTargetNotReached, LUCHTMENGKAST_BOVENKLEP_TARGET_NOT_REACHED_AL, i);
    }
    else if (Luchtmengkast[i].Binnenklep.AlarmTimer >= setp_alg.LuchtmengkastGroep[opt_app.Luchtmengkast[i].Groep].TimePositionAlarm)
    {
      if (opt_app.Luchtmengkast[i].TypeKlep == TYPE_KLEP_BINNEN_BUITEN)
        CreateAlarm(&alarm_hr_alg.Luchtmengkast[i].BinnenklepTargetNotReached, LUCHTMENGKAST_BINNENKLEP_TARGET_NOT_REACHED_AL, i, i + 1, 0, HARD_ALARM);
      else
        CreateAlarm(&alarm_hr_alg.Luchtmengkast[i].BovenklepTargetNotReached, LUCHTMENGKAST_BOVENKLEP_TARGET_NOT_REACHED_AL, i, i + 1, 0, HARD_ALARM);
      return (1);
    }
    else if (TimerExpired(&Luchtmengkast[i].Binnenklep.Timer_1min))
    {
      Luchtmengkast[i].Binnenklep.AlarmTimer++;
      TimerReset(&Luchtmengkast[i].Binnenklep.Timer_1min);
    }
  }
  return (0);
}

static unsigned char LuchtmengkastCheckVerwarmingAlarm(unsigned char i)
{
unsigned char AlarmPosition;
int Diff;
int Setp;

  if (opt_app.Luchtmengkast[i].VerwarmingEnabled)
  {
    Setp = val_hr_alg.Luchtmengkast[i].Verwarming.Setpoint;
    Diff = abs_int((int)val_hr_alg.Luchtmengkast[i].Verwarming.Actual - (int)val_hr_alg.Luchtmengkast[i].Verwarming.Setpoint);
	AlarmPosition = setp_alg.LuchtmengkastGroep[opt_app.Luchtmengkast[i].Groep].DiffPositionAlarm;

//    if (((Setp == 0) && (Diff == 0)) || ((Setp > 0) && (Diff < setp_alg.LuchtmengkastGroep[opt_app.Luchtmengkast[i].Groep].DiffPositionAlarm)))
    if ((AlarmPosition == 0) || ((Setp == 0) && (Diff < MAX_AFWIJKING_DICHT)) || ((Setp > 0) && (Diff < AlarmPosition)))
    {
      Luchtmengkast[i].Verwarming.AlarmTimer = 0;
      TimerRestart(&Luchtmengkast[i].Verwarming.Timer_1min);
      ClearAlarm(&alarm_hr_alg.Luchtmengkast[i].VerwarmingTargetNotReached, LUCHTMENGKAST_VERWARMING_TARGET_NOT_REACHED_AL, i);
    }
    else if (Luchtmengkast[i].Verwarming.AlarmTimer >= setp_alg.LuchtmengkastGroep[opt_app.Luchtmengkast[i].Groep].TimePositionAlarm)
    {
      CreateAlarm(&alarm_hr_alg.Luchtmengkast[i].VerwarmingTargetNotReached, LUCHTMENGKAST_VERWARMING_TARGET_NOT_REACHED_AL, i, i + 1, 0, HARD_ALARM);
      return (1);
    }
    else if (TimerExpired(&Luchtmengkast[i].Verwarming.Timer_1min))
    {
      Luchtmengkast[i].Verwarming.AlarmTimer++;
      TimerReset(&Luchtmengkast[i].Verwarming.Timer_1min);
    }
  }
  return (0);
}

static unsigned char LuchtmengkastCheckInblaasventAlarm(unsigned char i)
{
unsigned char AlarmPosition;
unsigned char Alarm = 0;
int Diff;
int Setp;

  switch (opt_app.Luchtmengkast[i].Inblaasvent.TypeSturing)
  {
    case TYPE_STURING_EBMBUS             : 
    case TYPE_STURING_EBM_MODBUS         :
    case TYPE_STURING_EC_BLUE_MODBUS     :
    case TYPE_STURING_EC_BLUE_PREMIUM    :
    case TYPE_STURING_MB_ROSENBERG       :
    case TYPE_STURING_MB_CLIMAFAN        :
    case TYPE_STURING_MB_ROSENBERG_GEN3  :
    case TYPE_STURING_MB_NICOTRA_GEBHARDT:	
      Alarm = mbDeviceCreateAlarm(opt_app.Luchtmengkast[i].Inblaasvent.Adres, LUCHTMENGKAST_INBLAASVENT_MB_AL, i, i + 1, opt_app.Luchtmengkast[i].Groep);
      break;
  }

  Setp = val_hr_alg.Luchtmengkast[i].Inblaasvent.Setpoint;
  Diff = abs_int((int)val_hr_alg.Luchtmengkast[i].Inblaasvent.Actual - (int)val_hr_alg.Luchtmengkast[i].Inblaasvent.Setpoint);
  AlarmPosition = setp_alg.LuchtmengkastGroep[opt_app.Luchtmengkast[i].Groep].DiffPositionAlarm;

//  if (Alarm || ((Setp == 0) && (Diff == 0)) || ((Setp > 0) && (Diff < setp_alg.LuchtmengkastGroep[opt_app.Luchtmengkast[i].Groep].DiffPositionAlarm)))
  if (Alarm || (AlarmPosition == 0) || ((Setp == 0) && (Diff < MAX_AFWIJKING_DICHT)) || ((Setp > 0) && (Diff < AlarmPosition)))
  {
    Luchtmengkast[i].Inblaasvent.AlarmTimer = 0;
    TimerRestart(&Luchtmengkast[i].Inblaasvent.Timer_1min);
    ClearAlarm(&alarm_hr_alg.Luchtmengkast[i].InblaasventTargetNotReached, LUCHTMENGKAST_INBLAASVENT_TARGET_NOT_REACHED_AL, i);
  }
  else if (Luchtmengkast[i].Inblaasvent.AlarmTimer >= setp_alg.LuchtmengkastGroep[opt_app.Luchtmengkast[i].Groep].TimePositionAlarm)
  {
    CreateAlarm(&alarm_hr_alg.Luchtmengkast[i].InblaasventTargetNotReached, LUCHTMENGKAST_INBLAASVENT_TARGET_NOT_REACHED_AL, i, i + 1, 0, HARD_ALARM);
    Alarm = 1;
  }
  else if (TimerExpired(&Luchtmengkast[i].Inblaasvent.Timer_1min))
  {
    Luchtmengkast[i].Inblaasvent.AlarmTimer++;
    TimerReset(&Luchtmengkast[i].Inblaasvent.Timer_1min);
  }

  return (Alarm);
}

static unsigned char LuchtmengkastCheckAfblaasventAlarm(unsigned char i)
{
unsigned char AlarmPosition;
unsigned char Alarm = 0;
unsigned char Bus   = 0;
int Diff;
int Setp;

  if (opt_app.Luchtmengkast[i].AfblaasventEnabled)
  {
    switch (opt_app.Luchtmengkast[i].Afblaasvent.TypeSturing)
    {
      case TYPE_STURING_EBMBUS             :
      case TYPE_STURING_EBM_MODBUS         :
      case TYPE_STURING_EC_BLUE_MODBUS     :
      case TYPE_STURING_EC_BLUE_PREMIUM    :
      case TYPE_STURING_MB_ROSENBERG       :
      case TYPE_STURING_MB_CLIMAFAN        :
      case TYPE_STURING_MB_ROSENBERG_GEN3  :
      case TYPE_STURING_MB_NICOTRA_GEBHARDT:
        Bus   = 1;
        Alarm = mbDeviceCreateAlarm(opt_app.Luchtmengkast[i].Afblaasvent.Adres, LUCHTMENGKAST_AFBLAASVENT_MB_AL, i, i + 1, opt_app.Luchtmengkast[i].Groep);
        break;
    }

    if (opt_app.Luchtmengkast[i].Afblaasvent.AnaIn.board_type || Bus)
    {
      Setp = val_hr_alg.Luchtmengkast[i].Afblaasvent.Setpoint;
      Diff = abs_int((int)val_hr_alg.Luchtmengkast[i].Afblaasvent.Actual - (int)val_hr_alg.Luchtmengkast[i].Afblaasvent.Setpoint);
	  AlarmPosition = setp_alg.LuchtmengkastGroep[opt_app.Luchtmengkast[i].Groep].DiffPositionAlarm;

//      if (Alarm || ((Setp == 0) && (Diff == 0)) || ((Setp > 0) && (Diff < setp_alg.LuchtmengkastGroep[opt_app.Luchtmengkast[i].Groep].DiffPositionAlarm)))
      if (Alarm || (AlarmPosition == 0) || ((Setp == 0) && (Diff < MAX_AFWIJKING_DICHT)) || ((Setp > 0) && (Diff < setp_alg.LuchtmengkastGroep[opt_app.Luchtmengkast[i].Groep].DiffPositionAlarm)))
      {
        Luchtmengkast[i].Afblaasvent.AlarmTimer = 0;
        TimerRestart(&Luchtmengkast[i].Afblaasvent.Timer_1min);
        ClearAlarm(&alarm_hr_alg.Luchtmengkast[i].AfblaasventTargetNotReached, LUCHTMENGKAST_AFBLAASVENT_TARGET_NOT_REACHED_AL, i);
      }
      else if (Luchtmengkast[i].Afblaasvent.AlarmTimer >= setp_alg.LuchtmengkastGroep[opt_app.Luchtmengkast[i].Groep].TimePositionAlarm)
      {
        CreateAlarm(&alarm_hr_alg.Luchtmengkast[i].AfblaasventTargetNotReached, LUCHTMENGKAST_AFBLAASVENT_TARGET_NOT_REACHED_AL, i, i + 1, 0, HARD_ALARM);
        Alarm = 1;
      }
      else if (TimerExpired(&Luchtmengkast[i].Afblaasvent.Timer_1min))
      {
        Luchtmengkast[i].Afblaasvent.AlarmTimer++;
        TimerReset(&Luchtmengkast[i].Afblaasvent.Timer_1min);
      }
    }
  }
  return (Alarm);
}

//------------------------------------------------------------------------------
static void LuchtmengkastGroepCheckAlarm(unsigned char i)
{
unsigned char HardAlarm = 0;

  // Set IO
  if (alarm_hr_alg.LuchtmengkastGroep[i].AlarmHard)
    IO_Set_Dig_Out(&opt_app.LuchtmengkastGroep[i].DigOutAlarm, 0);
  else
    IO_Set_Dig_Out(&opt_app.LuchtmengkastGroep[i].DigOutAlarm, 1);

  HardAlarm |= LuchtmengkastGroepCheckManualAlarm(i);
  HardAlarm |= LuchtmengkastGroepCheckVorstAlarm(i);
  HardAlarm |= LuchtmengkastGroepCheckDrukverschilAlarm(i);
  HardAlarm |= LuchtmengkastGroepCheckAlgemeenAlarm(i);
  HardAlarm |= LuchtmengkastGroepCheckThermischAlarm(i);
  
  alarm_hr_alg.LuchtmengkastGroep[i].AlarmHard = HardAlarm;
}

static void LuchtmengkastCheckAlarm(unsigned char i)
{
unsigned char HardAlarm = 0;

  HardAlarm |= LuchtmengkastCheckManualAlarm(i);
  HardAlarm |= LuchtmengkastCheckVorstAlarm(i);
  HardAlarm |= LuchtmengkastCheckExternAlarm(i);
  HardAlarm |= LuchtmengkastCheckDrukverschilAlarm(i);
  HardAlarm |= LuchtmengkastCheckBuitenklepAlarm(i);
  HardAlarm |= LuchtmengkastCheckBinnenklepAlarm(i);
  HardAlarm |= LuchtmengkastCheckVerwarmingAlarm(i);
  HardAlarm |= LuchtmengkastCheckAfblaasventAlarm(i);
  HardAlarm |= LuchtmengkastCheckInblaasventAlarm(i);
  
  alarm_hr_alg.LuchtmengkastGroep[opt_app.Luchtmengkast[i].Groep].AlarmHard |= HardAlarm;
}

//==============================================================================
//------------------------ Luchtmengkast - Naregelen ---------------------------
//==============================================================================
//------------------------------------------------------------------------------
static void LuchtmengkastNaregelen(unsigned char i)
{
int Streeftemperatuur;
int Correctie;

  if (opt_app.Luchtmengkast[i].Naregelen && !val_hr_alg.Luchtmengkast[i].VorstBewaking && (val_hr_alg.Luchtmengkast[i].OperationMode == omAuto))
  {
    if (TimerExpired(&Luchtmengkast[i].Timer_1s))
    {
      TimerReset(&Luchtmengkast[i].Timer_1s);
      Streeftemperatuur = IO_Get_Ana_In_One(&opt_app.LuchtmengkastGroep[opt_app.Luchtmengkast[i].Groep].Streeftemp);
      Correctie = val_hr_alg.Luchtmengkast[i].Verwarming.Offset;
      if (Streeftemperatuur == TEMP_FOUT)
      {
        val_hr_alg.Luchtmengkast[i].Verwarming.Offset = 0;
        return;
      }
      Calc_Integrated_Pos(val_hr_alg.Luchtmengkast[i].Inblaastemp,
                          Streeftemperatuur,
                          &Correctie,
                          -MAX_CORRECTIE,
                          MAX_CORRECTIE,
                          setp_alg.LuchtmengkastGroep[opt_app.Luchtmengkast[i].Groep].VerwarmingBandbreedte,
                          setp_alg.LuchtmengkastGroep[opt_app.Luchtmengkast[i].Groep].VerwarmingStap,
                          setp_alg.LuchtmengkastGroep[opt_app.Luchtmengkast[i].Groep].VerwarmingHysterese,
                          setp_alg.LuchtmengkastGroep[opt_app.Luchtmengkast[i].Groep].VerwarmingCyclustijd,
                          &Luchtmengkast[i].CyclusTimerVerwarming);

      val_hr_alg.Luchtmengkast[i].Verwarming.Offset = Correctie;
    }

    Correctie  = val_hr_alg.Luchtmengkast[i].Verwarming.Offset;
    Correctie += val_hr_alg.Luchtmengkast[i].Verwarming.Setpoint;
    if (Correctie > 100)
    {
      Correctie = 100;
      val_hr_alg.Luchtmengkast[i].Verwarming.Offset = 100 - val_hr_alg.Luchtmengkast[i].Verwarming.Setpoint;
    }
    if (Correctie < 0)
    {
      Correctie = 0;
      val_hr_alg.Luchtmengkast[i].Verwarming.Offset = -(int)val_hr_alg.Luchtmengkast[i].Verwarming.Setpoint;
    }
    val_hr_alg.Luchtmengkast[i].Verwarming.Setpoint = Correctie;
  }
  else
  {
    val_hr_alg.Luchtmengkast[i].Verwarming.Offset = 0;
    Luchtmengkast[i].CyclusTimerVerwarming = 0;
    TimerRestart(&Luchtmengkast[i].Timer_1s);
  }
}

//==============================================================================
//------------------------ Luchtmengkast - Servo Control -----------------------
//==============================================================================
//------------------------------------------------------------------------------
/*
static unsigned char ServoInControl(TOptServo *Opt, TValHrServoIn *Val)
{
unsigned char Position = 0;

  switch (Opt->TypeSturing)
  {
    case TYPE_STURING_DIGITAAL:
      if (IO_Get_Dig_In_Status(&Opt->Open))
        Val->PositionTime++;
      else if (IO_Get_Dig_In_Status(&Opt->Close))
      {
        if (Val->PositionTime > 0)
          Val->PositionTime--;
      }
      if (Val->PositionTime > Opt->Runtime)
        Val->PositionTime = Opt->Runtime;
      Position = Calc_Perc_Abs(Val->PositionTime, Opt->Runtime, 100);
      break;
    case TYPE_STURING_ANALOOG:
      Position = IO_Get_Ana_In_One(&Opt->AnaIn);
      break;
    default:
      break;
  }
  return (Position);
}
*/
static unsigned char ServoInFeedback(unsigned char servo, unsigned char i, TValHrServoIn *Val, TValHrServoOut *ValFirstUnit)
{
unsigned char Act;
int Max, Pos;
int nr, diff = 0;
TValHrServoOut *Unit;

  Act = Val->PositionPerc;
  for (nr = 0; nr < MAX_LUCHTMENGKAST; nr++)
  {
    if (opt_app.Luchtmengkast[nr].Enabled && (opt_app.Luchtmengkast[nr].Groep == i) && (val_hr_alg.Luchtmengkast[nr].OperationMode != omOff))
    {
      Unit = Get_Ptr(ValFirstUnit, nr);
	  Pos = Unit->Actual;
      if (val_hr_alg.Luchtmengkast[nr].OperationMode == omAuto)
      {
        switch (servo)
        {
          case SERVO_INBLAASVENT:
            Max = setp_alg.LuchtmengkastGroep[i].InblaasventMaximum;
            Pos = Calc_Prop_NoLimit(0, Max + val_hr_alg.Luchtmengkast[nr].Inblaasvent.Offset, 0, Max, val_hr_alg.Luchtmengkast[nr].Inblaasvent.Actual);
            break;
          case SERVO_AFBLAASVENT:
            Max = setp_alg.LuchtmengkastGroep[i].AfblaasventMaximum;
            Pos = Calc_Prop_NoLimit(0, Max + val_hr_alg.Luchtmengkast[nr].Afblaasvent.Offset, 0, Max, val_hr_alg.Luchtmengkast[nr].Afblaasvent.Actual);
            break;
		}
      }
      if (abs_int((int)Val->PositionPerc - Pos) > diff)
      {
        diff = abs_int((int)Val->PositionPerc - Pos);
        Act  = Pos;
      }
    }
  }
  return (Act);
}

static unsigned char ServoInControl(unsigned char servo, unsigned char i, TOptServo *Opt, TValHrServoIn *Val, TValHrServoOut *ValFirstUnit)
{
unsigned char Position = 0;
int bin;

  switch (Opt->TypeSturing)
  {
    case TYPE_STURING_DIGITAAL:
      if (IO_Get_Dig_In_Status(&Opt->Open))
        Val->PositionTime++;
      else if (IO_Get_Dig_In_Status(&Opt->Close))
      {
        if (Val->PositionTime > 0)
          Val->PositionTime--;
      }
      if (Val->PositionTime > Opt->Runtime)
        Val->PositionTime = Opt->Runtime;
      Position = Calc_Perc_Abs(Val->PositionTime, Opt->Runtime, 100);
      IO_Set_Ana_Out(&Opt->AnaOut, Val->PositionPerc);
      break;
    case TYPE_STURING_ANALOOG:
      Position = IO_Get_Ana_In_One(&Opt->AnaIn);
      IO_Set_Ana_Out(&Opt->AnaOut, ServoInFeedback(servo, i, Val, ValFirstUnit));
      break;
    case TYPE_STURING_CANOPEN:
      if (DS401_GetAnalogOutput(Opt->Adres, &bin) == CO_OK)
        Position = Calc_Perc(bin, 0x7FFF, 100);
      else
        Position = Val->PositionPerc;
      DS401_SetAnalogInput(Opt->Adres, Calc_Prop(0, 100, 0x0000, 0x7FFF, ServoInFeedback(servo, i, Val, ValFirstUnit)));
    default:
      break;
  }
  return (Position);
}
/*
static void ServoOutControl(TOptServo *Opt, TValHrServoOut *Val)
{
  switch (Opt->TypeSturing)
  {
    case TYPE_STURING_DIGITAAL:
      Val->Actual = IO_Get_Ana_In_One(&Opt->AnaIn);
      if (Val->Setpoint == 0)
      {
        IO_Set_Dig_Out(&Opt->Open,  0);
        IO_Set_Dig_Out(&Opt->Close, 1);
      }
      else if (Val->Setpoint == 100)
      {
        IO_Set_Dig_Out(&Opt->Close, 0);
        IO_Set_Dig_Out(&Opt->Open,  1);
      }
      else
      {
        if (Val->Actual >= Val->Setpoint)
          IO_Set_Dig_Out(&Opt->Open, 0);
        else if ((Val->Actual < ((int)Val->Setpoint - 2)) || (Val->Setpoint != Val->OldSetpoint))
          IO_Set_Dig_Out(&Opt->Open, 1);

        if (Val->Actual <= Val->Setpoint)
          IO_Set_Dig_Out(&Opt->Close, 0);
        else if ((Val->Actual > ((int)Val->Setpoint + 2)) || (Val->Setpoint != Val->OldSetpoint))
          IO_Set_Dig_Out(&Opt->Close, 1);
      }
      break;
    case TYPE_STURING_ANALOOG:
      Val->Actual = IO_Get_Ana_In_One(&Opt->AnaIn);
      IO_Set_Ana_Out(&Opt->AnaOut, Val->Setpoint);
      break;
  }
  Val->OldSetpoint = Val->Setpoint;
}
*/
static void ServoOutEcVent(TmbDeviceType DeviceType, TOptServo *Opt, TValHrServoOut *Val, TOptServo *OptFirstUnit)
{
int nr;
TOptServo *Unit;
s_board_IO_on_off IO[MAX_LUCHTMENGKAST];

  if (!mbDeviceConnected(Opt->Adres))
  {
    for (nr = 0; nr < MAX_LUCHTMENGKAST; nr++)
    {
      Unit = Get_Ptr(OptFirstUnit, nr);
      IO[nr] = Unit->AnaIn;
    }
    mbDeviceConnect(Opt->Adres, DeviceType, IO, MAX_LUCHTMENGKAST);
    mbDeviceSetOperationMode(Opt->Adres, ClosedLoopOpenLoop);
    mbDeviceSetRampUp(Opt->Adres, RampUp);
    mbDeviceSetRampDown(Opt->Adres, RampDown);
    mbDeviceSetPowerFactor(Opt->Adres, PowerFactor);
  }
  else
  {
    mbDeviceSetTargetValue(Opt->Adres, Val->Setpoint);
    Val->Actual = mbDeviceGetActualValue(Opt->Adres);
  }
}

static void ServoOutControl(TOptServo *Opt, TValHrServoOut *Val, TLuchtmengkastSectie *Glb, TOptServo *OptFirstUnit)
{
  switch (Opt->TypeSturing)
  {
    case TYPE_STURING_DIGITAAL:
      Val->Actual = IO_Get_Ana_In_One(&Opt->AnaIn);
      if (Val->Setpoint == 0)
      {
        IO_Set_Dig_Out(&Opt->Open,  0);
        IO_Set_Dig_Out(&Opt->Close, 1);
      }
      else if (Val->Setpoint == 100)
      {
        IO_Set_Dig_Out(&Opt->Close, 0);
        IO_Set_Dig_Out(&Opt->Open,  1);
      }
      else
      {
        if (Val->Actual >= Val->Setpoint)
          IO_Set_Dig_Out(&Opt->Open, 0);
        else if ((Val->Actual < ((int)Val->Setpoint - 2)) || (Val->Setpoint != Val->OldSetpoint))
          IO_Set_Dig_Out(&Opt->Open, 1);

        if (Val->Actual <= Val->Setpoint)
          IO_Set_Dig_Out(&Opt->Close, 0);
        else if ((Val->Actual > ((int)Val->Setpoint + 2)) || (Val->Setpoint != Val->OldSetpoint))
          IO_Set_Dig_Out(&Opt->Close, 1);
      }
      // Als de terugmelding binnen een bepaalde marge ligt dan mag deze gelijk worden gemaakt aan de gewenste waarde.
      if (abs_int((int)Val->Actual - (int)Val->Setpoint) <= CORRECTIE_MARGE) 
      {
        if (Glb->OldValue != Val->Actual)
        {
          TimerRestart(&Glb->Timer_5s);
          Glb->Correction = 0;
        }
        if ((Glb->Correction == 0) && (TimerExpired(&Glb->Timer_5s)))
          Glb->Correction = 1;
      }
      else
        Glb->Correction = 0;
      Glb->OldValue = Val->Actual;
      if (Glb->Correction)
        Val->Actual = Val->Setpoint;
      break;
    case TYPE_STURING_ANALOOG:
      IO_Set_Ana_Out(&Opt->AnaOut, Val->Setpoint);
      Val->Actual = IO_Get_Ana_In_One(&Opt->AnaIn);
      // Als de terugmelding binnen een bepaalde marge ligt dan mag deze gelijk worden gemaakt aan de gewenste waarde.
      if (abs_int((int)Val->Actual - (int)Val->Setpoint) <= CORRECTIE_MARGE) 
      {
        if (Glb->OldValue != Val->Actual)
        {
          TimerRestart(&Glb->Timer_5s);
          Glb->Correction = 0;
        }
        if ((Glb->Correction == 0) && (TimerExpired(&Glb->Timer_5s)))
          Glb->Correction = 1;
      }
      else
        Glb->Correction = 0;
      Glb->OldValue = Val->Actual;
      if (Glb->Correction)
        Val->Actual = Val->Setpoint;
      break;
    case TYPE_STURING_EBMBUS:
      ServoOutEcVent(dtEbmBus, Opt, Val, OptFirstUnit);
      break;
    case TYPE_STURING_EBM_MODBUS:
      ServoOutEcVent(dtEbmModbus, Opt, Val, OptFirstUnit);
      break;
    case TYPE_STURING_EC_BLUE_MODBUS:
      ServoOutEcVent(dtECblueModbus, Opt, Val, OptFirstUnit);
      break;
    case TYPE_STURING_EC_BLUE_PREMIUM:
      ServoOutEcVent(dtECbluePremium, Opt, Val, OptFirstUnit);
      break;
    case TYPE_STURING_MB_ROSENBERG:
      ServoOutEcVent(dtRosenberg, Opt, Val, OptFirstUnit);
      break;
    case TYPE_STURING_MB_CLIMAFAN:
      ServoOutEcVent(dtClimafan, Opt, Val, OptFirstUnit);
      break;
    case TYPE_STURING_MB_ROSENBERG_GEN3:
      ServoOutEcVent(dtRosenbergGen3, Opt, Val, OptFirstUnit);
      break;
    case TYPE_STURING_MB_NICOTRA_GEBHARDT:
      ServoOutEcVent(dtNicotraGebhardt, Opt, Val, OptFirstUnit);
      break;	  
    default:
      break;
  }
  Val->OldSetpoint = Val->Setpoint;
}

//==============================================================================
//------------------------ Luchtmengkast - Servo groepen -----------------------
//==============================================================================
//--- Buitenklep ---------------------------------------------------------------
static void ServoGroepBuitenklep(unsigned char i)
{
unsigned char Position;

  Position = ServoInControl(SERVO_BUITENKLEP, i, &opt_app.LuchtmengkastGroep[i].Buitenklep, &val_hr_alg.LuchtmengkastGroep[i].Buitenklep, &val_hr_alg.Luchtmengkast[0].Buitenklep);

  switch (val_hr_alg.LuchtmengkastGroep[i].OperationMode)
  {
    case omAuto: val_hr_alg.LuchtmengkastGroep[i].Buitenklep.PositionPerc = Position; break;
    case omOff : val_hr_alg.LuchtmengkastGroep[i].Buitenklep.PositionPerc = 0;        break;
  }
}

//--- Binnenklep/Bovenklep -----------------------------------------------------
static unsigned char BovenklepGroepGekoppeldAanKlep(unsigned char i)
{
int Position = val_hr_alg.LuchtmengkastGroep[i].Binnenklep.PositionPerc;

  if (val_hr_alg.LuchtmengkastGroep[i].Buitenklep.PositionPerc < setp_alg.LuchtmengkastGroep[i].KlepstandUit)
    Position = 0;
  else if ((val_hr_alg.LuchtmengkastGroep[i].Buitenklep.PositionPerc >= setp_alg.LuchtmengkastGroep[i].KlepstandAan) || (val_hr_alg.LuchtmengkastGroep[i].Binnenklep.PositionPerc > 0))
    Position = Calc_Prop(setp_alg.LuchtmengkastGroep[i].KlepstandAan, 100, 1, 100, val_hr_alg.LuchtmengkastGroep[i].Buitenklep.PositionPerc);

  return (Position);
}

static void ServoGroepBinnenklep(unsigned char i)
{
unsigned char Position;

  if (opt_app.LuchtmengkastGroep[i].BovenklepEnabled || (opt_app.LuchtmengkastGroep[i].TypeKlep == TYPE_KLEP_BINNEN_BUITEN))
  {
    if (opt_app.LuchtmengkastGroep[i].BovenklepGekoppeldAanKlep)
      Position = BovenklepGroepGekoppeldAanKlep(i);
    else
      Position = ServoInControl(SERVO_BINNENKLEP, i, &opt_app.LuchtmengkastGroep[i].Binnenklep, &val_hr_alg.LuchtmengkastGroep[i].Binnenklep, &val_hr_alg.Luchtmengkast[0].Binnenklep);

    switch (val_hr_alg.LuchtmengkastGroep[i].OperationMode)
    {
      case omAuto: val_hr_alg.LuchtmengkastGroep[i].Binnenklep.PositionPerc = Position;                                                 break;
      case omOff : val_hr_alg.LuchtmengkastGroep[i].Binnenklep.PositionPerc = opt_app.LuchtmengkastGroep[i].BovenklepEnabled ? 0 : 100; break;
    }
  }
}

//--- Verwarming ---------------------------------------------------------------
static void ServoGroepVerwarming(unsigned char i)
{
unsigned char Position;

  if (opt_app.LuchtmengkastGroep[i].VerwarmingEnabled)
  {
    Position = ServoInControl(SERVO_VERWARMING, i, &opt_app.LuchtmengkastGroep[i].Verwarming, &val_hr_alg.LuchtmengkastGroep[i].Verwarming, &val_hr_alg.Luchtmengkast[0].Verwarming);

    switch (val_hr_alg.LuchtmengkastGroep[i].OperationMode)
    {
      case omAuto: val_hr_alg.LuchtmengkastGroep[i].Verwarming.PositionPerc = Position; break;
      case omOff : val_hr_alg.LuchtmengkastGroep[i].Verwarming.PositionPerc = 0;        break;
    }
  }
}

//--- Inblaasventilator --------------------------------------------------------
static void ServoGroepInblaasvent(unsigned char i)
{
unsigned char Position;

  Position = ServoInControl(SERVO_INBLAASVENT, i, &opt_app.LuchtmengkastGroep[i].Inblaasvent, &val_hr_alg.LuchtmengkastGroep[i].Inblaasvent, &val_hr_alg.Luchtmengkast[0].Inblaasvent);

  switch (val_hr_alg.LuchtmengkastGroep[i].OperationMode)
  {
    case omAuto:
      if ((Position < setp_alg.LuchtmengkastGroep[i].InblaasventMinimum) && (Position > 0))
        Position = setp_alg.LuchtmengkastGroep[i].InblaasventMinimum;
      else if (Position > setp_alg.LuchtmengkastGroep[i].InblaasventMaximum)
        Position = setp_alg.LuchtmengkastGroep[i].InblaasventMaximum;
      val_hr_alg.LuchtmengkastGroep[i].Inblaasvent.PositionPerc = Position;
      break;
    case omOff :
      val_hr_alg.LuchtmengkastGroep[i].Inblaasvent.PositionPerc = 0;
      break;
  }
}

//--- Afblaasventilator --------------------------------------------------------
static unsigned char AfblaasventGroepGekoppeldAanKlep(unsigned char i)
{
int Position = val_hr_alg.LuchtmengkastGroep[i].Afblaasvent.PositionPerc;

  if (val_hr_alg.LuchtmengkastGroep[i].Buitenklep.PositionPerc < setp_alg.LuchtmengkastGroep[i].KlepstandUit)
    Position = 0;
  else if ((val_hr_alg.LuchtmengkastGroep[i].Buitenklep.PositionPerc >= setp_alg.LuchtmengkastGroep[i].KlepstandAan) || (val_hr_alg.LuchtmengkastGroep[i].Afblaasvent.PositionPerc > 0))
    Position = Calc_Prop(setp_alg.LuchtmengkastGroep[i].KlepstandAan, 100, setp_alg.LuchtmengkastGroep[i].AfblaasventMinimum, setp_alg.LuchtmengkastGroep[i].AfblaasventMaximum, val_hr_alg.LuchtmengkastGroep[i].Buitenklep.PositionPerc);

  return (Position);
}

static void ServoGroepAfblaasvent(unsigned char i)
{
unsigned char Position;

  if (opt_app.LuchtmengkastGroep[i].AfblaasventEnabled)
  {
    if (opt_app.LuchtmengkastGroep[i].AfblaasventGekoppeldAanKlep)
      Position = AfblaasventGroepGekoppeldAanKlep(i);
    else
      Position = ServoInControl(SERVO_AFBLAASVENT, i, &opt_app.LuchtmengkastGroep[i].Afblaasvent, &val_hr_alg.LuchtmengkastGroep[i].Afblaasvent, &val_hr_alg.Luchtmengkast[0].Afblaasvent);

    switch (val_hr_alg.LuchtmengkastGroep[i].OperationMode)
    {
      case omAuto:
        if ((Position < setp_alg.LuchtmengkastGroep[i].AfblaasventMinimum) && (Position > 0))
          Position = setp_alg.LuchtmengkastGroep[i].AfblaasventMinimum;
        else if (Position > setp_alg.LuchtmengkastGroep[i].AfblaasventMaximum)
          Position = setp_alg.LuchtmengkastGroep[i].AfblaasventMaximum;
        val_hr_alg.LuchtmengkastGroep[i].Afblaasvent.PositionPerc = Position;
        break;
      case omOff :
        val_hr_alg.LuchtmengkastGroep[i].Afblaasvent.PositionPerc = 0;
        break;
    }
  }
}

//==============================================================================
//------------------------ Luchtmengkast - Servo units -------------------------
//==============================================================================
//--- Buitenklep ---------------------------------------------------------------
static void ServoUnitBuitenklep(unsigned char i)
{
  switch (val_hr_alg.Luchtmengkast[i].OperationMode)
  {
    case omAuto:
      val_hr_alg.Luchtmengkast[i].Buitenklep.Setpoint = val_hr_alg.LuchtmengkastGroep[opt_app.Luchtmengkast[i].Groep].Buitenklep.PositionPerc;
      if (val_hr_alg.Luchtmengkast[i].VorstBewaking)
        val_hr_alg.Luchtmengkast[i].Buitenklep.Setpoint = 0;
      break;
    case omOff :
      val_hr_alg.Luchtmengkast[i].Buitenklep.Setpoint = 0;
      break;
  }
  ServoOutControl(&opt_app.Luchtmengkast[i].Buitenklep, &val_hr_alg.Luchtmengkast[i].Buitenklep, &Luchtmengkast[i].Buitenklep, &opt_app.Luchtmengkast[0].Buitenklep);
}

//--- Binnenklep/Bovenklep -----------------------------------------------------
static unsigned char BovenklepGekoppeldAanKlep(unsigned char i)
{
int Position = val_hr_alg.Luchtmengkast[i].Binnenklep.Setpoint;

  if (val_hr_alg.LuchtmengkastGroep[opt_app.Luchtmengkast[i].Groep].OperationMode == omAuto)
  {
    if (val_hr_alg.Luchtmengkast[i].Buitenklep.Actual < (setp_alg.LuchtmengkastGroep[opt_app.Luchtmengkast[i].Groep].KlepstandUit))
      Position = 0;
    else if ((val_hr_alg.Luchtmengkast[i].Buitenklep.Actual >= setp_alg.LuchtmengkastGroep[opt_app.Luchtmengkast[i].Groep].KlepstandAan) || (val_hr_alg.Luchtmengkast[i].Binnenklep.Setpoint > 0))
      Position = Calc_Prop(setp_alg.LuchtmengkastGroep[opt_app.Luchtmengkast[i].Groep].KlepstandAan, 100, 1, 100, val_hr_alg.Luchtmengkast[i].Buitenklep.Actual);
  }
  else
  {
    Position = val_hr_alg.LuchtmengkastGroep[opt_app.Luchtmengkast[i].Groep].Binnenklep.PositionPerc;
  }
  return (Position);
}

static void ServoUnitBinnenklep(unsigned char i)
{
unsigned char Position;

  if (opt_app.Luchtmengkast[i].BovenklepEnabled || (opt_app.Luchtmengkast[i].TypeKlep == TYPE_KLEP_BINNEN_BUITEN))
  {
    if (opt_app.LuchtmengkastGroep[opt_app.Luchtmengkast[i].Groep].BovenklepGekoppeldAanKlep)
      Position = BovenklepGekoppeldAanKlep(i);
    else
      Position = val_hr_alg.LuchtmengkastGroep[opt_app.Luchtmengkast[i].Groep].Binnenklep.PositionPerc;

    switch (val_hr_alg.Luchtmengkast[i].OperationMode)
    {
      case omAuto:
        val_hr_alg.Luchtmengkast[i].Binnenklep.Setpoint = Position;
        if (val_hr_alg.Luchtmengkast[i].VorstBewaking)
          val_hr_alg.Luchtmengkast[i].Binnenklep.Setpoint = opt_app.Luchtmengkast[i].BovenklepEnabled ? 0 : 100;
        break;
      case omOff :
        val_hr_alg.Luchtmengkast[i].Binnenklep.Setpoint = opt_app.Luchtmengkast[i].BovenklepEnabled ? 0 : 100;
        break;
    }
    ServoOutControl(&opt_app.Luchtmengkast[i].Binnenklep, &val_hr_alg.Luchtmengkast[i].Binnenklep, &Luchtmengkast[i].Binnenklep, &opt_app.Luchtmengkast[0].Binnenklep);
  }
}

//--- Verwarming ---------------------------------------------------------------
static void ServoUnitVerwarming(unsigned char i)
{
  if (opt_app.Luchtmengkast[i].VerwarmingEnabled)
  {
    switch (val_hr_alg.Luchtmengkast[i].OperationMode)
    {
      case omAuto:
        val_hr_alg.Luchtmengkast[i].Verwarming.Setpoint = val_hr_alg.LuchtmengkastGroep[opt_app.Luchtmengkast[i].Groep].Verwarming.PositionPerc;
        if (val_hr_alg.Luchtmengkast[i].VorstBewaking)
          val_hr_alg.Luchtmengkast[i].Verwarming.Setpoint = 0;
        break;
      case omOff :
        val_hr_alg.Luchtmengkast[i].Verwarming.Setpoint = 0;
        break;
    }
    LuchtmengkastNaregelen(i);
    ServoOutControl(&opt_app.Luchtmengkast[i].Verwarming, &val_hr_alg.Luchtmengkast[i].Verwarming, &Luchtmengkast[i].Verwarming, &opt_app.Luchtmengkast[0].Verwarming);
  }
}

//--- Inblaasventilator --------------------------------------------------------
static void ServoUnitInblaasvent(unsigned char i)
{
int Setpoint;
int Max;

  switch (val_hr_alg.Luchtmengkast[i].OperationMode)
  {
    case omAuto:
      Max = setp_alg.LuchtmengkastGroep[opt_app.Luchtmengkast[i].Groep].InblaasventMaximum;
      Setpoint = Calc_Prop_NoLimit(0, Max, 0, Max + val_hr_alg.Luchtmengkast[i].Inblaasvent.Offset, val_hr_alg.LuchtmengkastGroep[opt_app.Luchtmengkast[i].Groep].Inblaasvent.PositionPerc);
      if (Setpoint > 100)
        Setpoint = 100;
      else if (Setpoint < 0)
        Setpoint = 0;
      val_hr_alg.Luchtmengkast[i].Inblaasvent.Setpoint = Setpoint;
      if (val_hr_alg.Luchtmengkast[i].VorstBewaking)
        val_hr_alg.Luchtmengkast[i].Inblaasvent.Setpoint = 0;
      break;
    case omOff :
      val_hr_alg.Luchtmengkast[i].Inblaasvent.Setpoint = 0;
      break;
  }
  ClosedLoopOpenLoop = opt_app.LuchtmengkastInblaasventClosedLoopOpenLoop;
  RampUp      = opt_app.LuchtmengkastInblaasventRampUp;
  RampDown    = opt_app.LuchtmengkastInblaasventRampDown;
  PowerFactor = opt_app.LuchtmengkastInblaasventPowerFactor;
  ServoOutControl(&opt_app.Luchtmengkast[i].Inblaasvent, &val_hr_alg.Luchtmengkast[i].Inblaasvent, &Luchtmengkast[i].Inblaasvent, &opt_app.Luchtmengkast[0].Inblaasvent);
}

//--- Afblaasventilator --------------------------------------------------------
static unsigned char AfblaasventGekoppeldAanKlep(unsigned char i)
{
int Position = val_hr_alg.Luchtmengkast[i].Afblaasvent.Setpoint;

  if (val_hr_alg.LuchtmengkastGroep[opt_app.Luchtmengkast[i].Groep].OperationMode == omAuto)
  {
    if (val_hr_alg.Luchtmengkast[i].Buitenklep.Actual < (setp_alg.LuchtmengkastGroep[opt_app.Luchtmengkast[i].Groep].KlepstandUit))
      Position = 0;
    else if ((val_hr_alg.Luchtmengkast[i].Buitenklep.Actual >= setp_alg.LuchtmengkastGroep[opt_app.Luchtmengkast[i].Groep].KlepstandAan) || (val_hr_alg.Luchtmengkast[i].Afblaasvent.Setpoint > 0))
      Position = Calc_Prop(setp_alg.LuchtmengkastGroep[opt_app.Luchtmengkast[i].Groep].KlepstandAan, 100, setp_alg.LuchtmengkastGroep[opt_app.Luchtmengkast[i].Groep].AfblaasventMinimum, setp_alg.LuchtmengkastGroep[opt_app.Luchtmengkast[i].Groep].AfblaasventMaximum, val_hr_alg.Luchtmengkast[i].Buitenklep.Actual);
  }
  else
  {
    Position = val_hr_alg.LuchtmengkastGroep[opt_app.Luchtmengkast[i].Groep].Afblaasvent.PositionPerc;
  }
  return (Position);
}

static void ServoUnitAfblaasvent(unsigned char i)
{
unsigned char Position = 0;
int Setpoint;
int Max;

  if (opt_app.Luchtmengkast[i].AfblaasventEnabled)
  {
    if (opt_app.LuchtmengkastGroep[opt_app.Luchtmengkast[i].Groep].AfblaasventGekoppeldAanKlep)
      Position = AfblaasventGekoppeldAanKlep(i);
    else
      Position = val_hr_alg.LuchtmengkastGroep[opt_app.Luchtmengkast[i].Groep].Afblaasvent.PositionPerc;

    switch (val_hr_alg.Luchtmengkast[i].OperationMode)
    {
      case omAuto:
        Max = setp_alg.LuchtmengkastGroep[opt_app.Luchtmengkast[i].Groep].AfblaasventMaximum;
        Setpoint = Calc_Prop_NoLimit(0, Max, 0, Max + val_hr_alg.Luchtmengkast[i].Afblaasvent.Offset, Position);
        if (Setpoint > 100)
          Setpoint = 100;
        else if (Setpoint < 0)
          Setpoint = 0;
        val_hr_alg.Luchtmengkast[i].Afblaasvent.Setpoint = Setpoint;
        if (val_hr_alg.Luchtmengkast[i].VorstBewaking)
          val_hr_alg.Luchtmengkast[i].Afblaasvent.Setpoint = 0;
        break;
      case omOff :
        val_hr_alg.Luchtmengkast[i].Afblaasvent.Setpoint = 0;
        break;
    }
    ClosedLoopOpenLoop = opt_app.LuchtmengkastAfblaasventClosedLoopOpenLoop;
    RampUp      = opt_app.LuchtmengkastAfblaasventRampUp;
    RampDown    = opt_app.LuchtmengkastAfblaasventRampDown;
    PowerFactor = opt_app.LuchtmengkastAfblaasventPowerFactor;
    ServoOutControl(&opt_app.Luchtmengkast[i].Afblaasvent, &val_hr_alg.Luchtmengkast[i].Afblaasvent, &Luchtmengkast[i].Afblaasvent, &opt_app.Luchtmengkast[0].Afblaasvent);
  }
}

//==============================================================================
//------------------------ Luchtmengkast - Main --------------------------------
//==============================================================================
//------------------------------------------------------------------------------
void LuchtmengkastMain(void)
{
unsigned char i;

  if (TimerExpired(&Timer_100ms))
  {
    TimerReset(&Timer_100ms);

    for (i = 0; i < MAX_LUCHTMENGKAST_GROEP; i++)
    {
      if (opt_app.LuchtmengkastGroep[i].Enabled)
      {
        LuchtmengkastGroepCheckAlarm(i);

        ServoGroepBuitenklep(i);
        ServoGroepBinnenklep(i);
        ServoGroepVerwarming(i);
        ServoGroepInblaasvent(i);
        ServoGroepAfblaasvent(i);
      }
    }

    for (i = 0; i < MAX_LUCHTMENGKAST; i++)
    {
      if (opt_app.Luchtmengkast[i].Enabled && (opt_app.Luchtmengkast[i].Groep < MAX_LUCHTMENGKAST_GROEP))
      {
        LuchtmengkastCheckAlarm(i);
        
        val_hr_alg.Luchtmengkast[i].Inblaastemp = IO_Get_Ana_In_One(&opt_app.Luchtmengkast[i].AnaInInblaastemp);
        val_hr_alg.Luchtmengkast[i].Mengtemp    = IO_Get_Ana_In_One(&opt_app.Luchtmengkast[i].AnaInMengtemp);

        ServoUnitBuitenklep(i);
        ServoUnitBinnenklep(i);
        ServoUnitVerwarming(i);
        ServoUnitInblaasvent(i);
        ServoUnitAfblaasvent(i);
      }
    }
  }
}

