// C__VENT.C  

#include "ch_define.h"

#include "ch_alarm.h"
#include "ch_alg.h"
#include "ch_ds301.h"
#include "ch_mb_device.h"
#include "ch_motorgroep.h"
#include "ch_IO.h"
#include "ch_sd.h"
#include "ch_tijd.h"
#include "ch_device.h"

//==============================================================================
//------------------------ Device - Globals ------------------------------------
//==============================================================================
//------------------------------------------------------------------------------
TDevice Device[MAX_DEVICE];

//==============================================================================
//------------------------ Device - Prototyping --------------------------------
//==============================================================================
//------------------------------------------------------------------------------

//==============================================================================
//------------------------ Device - Initialisation -----------------------------
//==============================================================================
//------------------------------------------------------------------------------
void CreateDevice(TDevice *pDevice, int Number)
{
  pDevice->Number = Number;
  InitDevice(pDevice);
}

void InitDevice(TDevice *pDevice)
{
  TimerSet(&pDevice->Timer_1min,      TIMER_1MIN);
  TimerSet(&pDevice->TimerAlarmDelay, TIMER_1MIN);
}

//==============================================================================
//------------------------ Device - Properties ---------------------------------
//==============================================================================
//------------------------------------------------------------------------------
static int GetDeviceGroup(TDevice *pDevice)
{
int Group;

  Group = (int)opt_app.Device[pDevice->Number].GroupNumber - 1;
  if ((Group < 0) || (Group > MAX_GROUP))
    return (-1);
  else
    return (Group);
}

//==============================================================================
//------------------------ Device - Alarm --------------------------------------
//==============================================================================
//------------------------------------------------------------------------------
void ResetAlarmDevice(TDevice *pDevice)
{
int DeviceAddress;
int Group;

  Group = GetDeviceGroup(pDevice);
  if (Group == -1)
    return;

  DeviceAddress = opt_app.Motorgroup[Group].FirstAddress + (pDevice->Number + 1 - opt_app.Motorgroup[Group].FirstNumber);
  switch (opt_app.Motorgroup[Group].Type)
  {
    case TYPE_VENT:
      ClearAlarm(&alarm_hr_alg.Device[pDevice->Number].Manual,           VENT_MANUAL_AL,             pDevice->Number);
      ClearAlarm(&alarm_hr_alg.Device[pDevice->Number].TargetNotReached, VENT_TARGET_NOT_REACHED_AL, pDevice->Number);
      mbDeviceClearAllAlarms(DeviceAddress, VENT_MB_AL, pDevice->Number);
      break;
    case TYPE_KLEP:
      ClearAlarm(&alarm_hr_alg.Device[pDevice->Number].Manual,           KLEP_MANUAL_AL,             pDevice->Number);
      ClearAlarm(&alarm_hr_alg.Device[pDevice->Number].TargetNotReached, KLEP_TARGET_NOT_REACHED_AL, pDevice->Number);
      ClearAlarm(&alarm_hr_alg.Device[pDevice->Number].LimitSwitch,      KLEP_LIMITSWITCH_AL,        pDevice->Number);
      ClearAlarm(&alarm_hr_alg.Device[pDevice->Number].ExternAlarm,      KLEP_EXTERN_ALARM_AL,       pDevice->Number);
      mbDeviceClearAllAlarms(DeviceAddress, KLEP_MB_AL, pDevice->Number);
      break;
  }
}

//------------------------------------------------------------------------------
unsigned char DeviceGetEmcyBits(TDevice *pDevice)
{
unsigned char Emcy = 0;
int DeviceAddress;
int Group;

  Group = GetDeviceGroup(pDevice);
  if (Group == -1)
    return (0);

  DeviceAddress = opt_app.Motorgroup[Group].FirstAddress + (pDevice->Number + 1 - opt_app.Motorgroup[Group].FirstNumber);

  if (alarm_hr_alg.Device[pDevice->Number].Manual)           Emcy |= MOTOR_EMCY_BIT_3;
  if (alarm_hr_alg.Device[pDevice->Number].TargetNotReached) Emcy |= MOTOR_EMCY_BIT_3;
  if (mbDeviceAlarmActive(DeviceAddress))                    Emcy |= MOTOR_EMCY_BIT_5;

  return (Emcy);
}

//------------------------------------------------------------------------------
static unsigned char CheckDeviceManualAlarm(TDevice *pDevice)
{
unsigned char Alarm = 0;
unsigned int AlarmCode;
int Group;

  Group = GetDeviceGroup(pDevice);
  if (Group == -1)
    return (0);

  switch (opt_app.Motorgroup[Group].Type)
  {
    default:
    case TYPE_VENT: AlarmCode = VENT_MANUAL_AL; break;
    case TYPE_KLEP: AlarmCode = KLEP_MANUAL_AL; break;
  }
  if (val_hr_alg.Device[pDevice->Number].OperationMode == omManual)
  {
    CreateAlarm(&alarm_hr_alg.Device[pDevice->Number].Manual, AlarmCode, pDevice->Number, pDevice->Number + 1, Group + 1, &alarm_hr_alg.Device[pDevice->Number].AlarmHard);
    if ((setp_alg.Motorgroup[Group].DelayAlarmManual == 0) || (TimeAlarmOn(AlarmCode, pDevice->Number) < (setp_alg.Motorgroup[Group].DelayAlarmManual * 60)))
      alarm_hr_alg.Device[pDevice->Number].AlarmHard = 0;
    else
    {
      Alarm = 1;
      alarm_hr_alg.Device[pDevice->Number].AlarmHard = 1;
    }
  }
  else
    ClearAlarm(&alarm_hr_alg.Device[pDevice->Number].Manual, AlarmCode, pDevice->Number);

  return (Alarm);
}

//------------------------------------------------------------------------------
static unsigned char CheckFlapAlarm(TDevice *pDevice)
{
int DeviceAddress;
int Group;

  Group = GetDeviceGroup(pDevice);
  if (Group == -1)
    return (0);

  DeviceAddress = opt_app.Motorgroup[Group].FirstAddress + (pDevice->Number + 1 - opt_app.Motorgroup[Group].FirstNumber);

  switch (opt_app.Motorgroup[Group].SensorType)
  {
    case SENSOR_TYPE_GEEN:
      ClearAlarm(&alarm_hr_alg.Device[pDevice->Number].LimitSwitch, KLEP_LIMITSWITCH_AL,  pDevice->Number);
      ClearAlarm(&alarm_hr_alg.Device[pDevice->Number].ExternAlarm, KLEP_EXTERN_ALARM_AL, pDevice->Number);
      break;
    case SENSOR_TYPE_EINDSCHAKELAAR:
      ClearAlarm(&alarm_hr_alg.Device[pDevice->Number].ExternAlarm, KLEP_EXTERN_ALARM_AL, pDevice->Number);
      if (val_hr_alg.Device[pDevice->Number].ActualValue > 0)
      {
        TimerRestart(&Device[pDevice->Number].TimerAlarmDelay);
        ClearAlarm(&alarm_hr_alg.Device[pDevice->Number].LimitSwitch, KLEP_LIMITSWITCH_AL, pDevice->Number);
      }
      else
      {
        if (mbDeviceGetSensorValue(DeviceAddress) == 0)
        {
          if (TimerExpired(&Device[pDevice->Number].TimerAlarmDelay))
            CreateAlarm(&alarm_hr_alg.Device[pDevice->Number].LimitSwitch, KLEP_LIMITSWITCH_AL, pDevice->Number, pDevice->Number + 1, Group + 1, HARD_ALARM);
        }
        else
        {
          TimerRestart(&Device[pDevice->Number].TimerAlarmDelay);
          ClearAlarm(&alarm_hr_alg.Device[pDevice->Number].LimitSwitch, KLEP_LIMITSWITCH_AL, pDevice->Number);
        }
      }
      break;
    case SENSOR_TYPE_EXTERN_ALARM:
      ClearAlarm(&alarm_hr_alg.Device[pDevice->Number].LimitSwitch, KLEP_LIMITSWITCH_AL, pDevice->Number);
      if (mbDeviceDataValid(DeviceAddress))
      {
        if (mbDeviceGetSensorValue(DeviceAddress) == 0)
          CreateAlarm(&alarm_hr_alg.Device[pDevice->Number].ExternAlarm, KLEP_EXTERN_ALARM_AL, pDevice->Number, pDevice->Number + 1, Group + 1, ZACHT_ALARM);
        else
          ClearAlarm(&alarm_hr_alg.Device[pDevice->Number].ExternAlarm, KLEP_EXTERN_ALARM_AL, pDevice->Number);
      }
      break;
  }
  return (alarm_hr_alg.Device[pDevice->Number].LimitSwitch);
}

static unsigned char CheckDeviceAlarm(TDevice *pDevice)
{
unsigned char AlarmPosition;
unsigned char Alarm = 0;
unsigned int AlarmCode;
int DeviceAddress;
int Group;

  Group = GetDeviceGroup(pDevice);
  if (Group == -1)
    return (0);

  DeviceAddress = opt_app.Motorgroup[Group].FirstAddress + (pDevice->Number + 1 - opt_app.Motorgroup[Group].FirstNumber);

  switch (opt_app.Motorgroup[Group].Type)
  {
    default:
      AlarmCode = VENT_TARGET_NOT_REACHED_AL;
	  break;
    case TYPE_VENT:
      Alarm = mbDeviceCreateAlarm(DeviceAddress, VENT_MB_AL, pDevice->Number, pDevice->Number + 1, Group + 1);
      AlarmCode = VENT_TARGET_NOT_REACHED_AL;
      AlarmPosition = (setp_alg.Motorgroup[Group].DiffPositionAlarm + 5) / 10;
      break;
    case TYPE_KLEP:
      Alarm = mbDeviceCreateAlarm(DeviceAddress, KLEP_MB_AL, pDevice->Number, pDevice->Number + 1, Group + 1);
      AlarmCode = KLEP_TARGET_NOT_REACHED_AL;
      AlarmPosition = (setp_alg.Motorgroup[Group].DiffPositionAlarm + 5) / 10;
      if (val_hr_alg.Device[pDevice->Number].TargetValue == 0)
        AlarmPosition = 1;
      Alarm |= CheckFlapAlarm(pDevice);
      break;
  }

  AlarmPosition = (setp_alg.Motorgroup[Group].DiffPositionAlarm + 5) / 10;

  if ((Motorgroup[Group].AlarmAfw == 0)	|| Alarm || (AlarmPosition == 0) || (abs_int(val_hr_alg.Device[pDevice->Number].ActualValue - val_hr_alg.Device[pDevice->Number].TargetValue) < AlarmPosition))
  {
    Device[pDevice->Number].TimerPositionAlarm = 0;
    TimerRestart(&Device[pDevice->Number].Timer_1min);
    ClearAlarm(&alarm_hr_alg.Device[pDevice->Number].TargetNotReached, AlarmCode, pDevice->Number);
  }
  else if (Device[pDevice->Number].TimerPositionAlarm >= setp_alg.Motorgroup[Group].TimePositionAlarm)
  {
    CreateAlarm(&alarm_hr_alg.Device[pDevice->Number].TargetNotReached, AlarmCode, pDevice->Number, pDevice->Number + 1, Group + 1, HARD_ALARM);
    Alarm = 1;
  }
  else if (TimerExpired(&Device[pDevice->Number].Timer_1min))
  {
    Device[pDevice->Number].TimerPositionAlarm++;
    TimerReset(&Device[pDevice->Number].Timer_1min);
  }

  return (Alarm);
}

//------------------------------------------------------------------------------
unsigned char ControlAlarmDevice(TDevice *pDevice)
{
unsigned char Alarm = 0;

  Alarm |= CheckDeviceManualAlarm(pDevice);
  Alarm |= CheckDeviceAlarm(pDevice);

  return (Alarm);
}

//==============================================================================
//------------------------ Device - Control ------------------------------------
//==============================================================================
//------------------------------------------------------------------------------
static unsigned char VentVrijgegeven(TDevice *pDevice)
{
unsigned int Mask;
int i;

  if (opt_app.VrijgaveVent.Aantal == 0)
    return (1);
  
  if (opt_app.VrijgaveVent.Ventilator[pDevice->Number] == 0)
    return (1);

  for (i = 0; i < opt_app.VrijgaveVent.Aantal; i++)
  {
    Mask = 0x0001 << i;
    if (opt_app.VrijgaveVent.Ventilator[pDevice->Number] & Mask)
    {
      if (IO_Get_Dig_In_Status(&opt_app.VrijgaveVent.DigInVrijgave[i]))
        return (1);
    }
  }

  return (0);
}

void SetTargetValueDevice(TDevice *pDevice, unsigned char Value, s_board_IO_on_off *IO, unsigned char IO_size)
{
int TargetValue;
int DeviceAddress;
int Group;

  Group = GetDeviceGroup(pDevice);
  if (Group == -1)
    return;

  switch (val_hr_alg.Device[pDevice->Number].OperationMode)
  {
    case omAuto:
      switch (opt_app.Motorgroup[Group].Type)
      {
        case TYPE_VENT:
          if ((Value == 0) || ((val_hr_alg.Motorgroup[Group].OperationMode == omAuto) && !VentVrijgegeven(pDevice)))
          {
            TargetValue = 0;
          }
          else
          {
            TargetValue = Value + setp_alg.Device[pDevice->Number].Offset;
            if (TargetValue < 0)
              TargetValue = 0;
            else if (TargetValue > 100)
              TargetValue = 100;
          }
          val_hr_alg.Device[pDevice->Number].TargetValue = TargetValue;
          break;
        case TYPE_KLEP:
          val_hr_alg.Device[pDevice->Number].TargetValue = Value;
          break;
      }
      break;
    case omManual:
      break;
    case omOff:
      val_hr_alg.Device[pDevice->Number].TargetValue = 0;
      break;
  }

  DeviceAddress = opt_app.Motorgroup[Group].FirstAddress + (pDevice->Number + 1 - opt_app.Motorgroup[Group].FirstNumber);

  if (!mbDeviceConnected(DeviceAddress))
  {
    mbDeviceConnect(DeviceAddress, opt_app.Motorgroup[Group].BusType, IO, IO_size);
	mbDeviceSetWatchdogMode(DeviceAddress, opt_app.Motorgroup[Group].WatchdogMode);
	mbDeviceSetWatchdogPosition(DeviceAddress, opt_app.Motorgroup[Group].PositionAtCommunicationFailure);
    switch (opt_app.Motorgroup[Group].Type)
    {
      case TYPE_VENT:
        if (opt_app.VrijgaveVent.Aantal > 0)
          mbDeviceConnect(DeviceAddress, opt_app.Motorgroup[Group].BusType, opt_app.VrijgaveVent.RS485Bus, MAX_VRIJGAVE);
        mbDeviceSetOperationMode(DeviceAddress, opt_app.Motorgroup[Group].ClosedLoopOpenLoop);
        mbDeviceSetRampUp(DeviceAddress, opt_app.Motorgroup[Group].RampUp);
        mbDeviceSetRampDown(DeviceAddress, opt_app.Motorgroup[Group].RampDown);
        mbDeviceSetPowerFactor(DeviceAddress, opt_app.Motorgroup[Group].PowerFactor);
        break;
      case TYPE_KLEP:
        mbDeviceSetSensorType(DeviceAddress, opt_app.Motorgroup[Group].SensorType);
        break;
    }
    InitDevice(pDevice);
  }
  else
  {
    if (mbDeviceSetTargetValue(DeviceAddress, val_hr_alg.Device[pDevice->Number].TargetValue))
    {
      TimerRestart(&Device[pDevice->Number].Timer_1min);
      Device[pDevice->Number].TimerPositionAlarm = 0;
    }
    val_hr_alg.Device[pDevice->Number].ActualValue = mbDeviceGetActualValue(DeviceAddress);
  }
}

int GetFlapSensorValue(TDevice *pDevice)
{
int DeviceAddress;
int Group;

  Group = GetDeviceGroup(pDevice);
  if (Group == -1)
    return (0);

  DeviceAddress = opt_app.Motorgroup[Group].FirstAddress + (pDevice->Number + 1 - opt_app.Motorgroup[Group].FirstNumber);
  return (mbDeviceGetSensorValue(DeviceAddress));
}

//==============================================================================
//------------------------ End of file -----------------------------------------
//==============================================================================
