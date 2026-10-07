// C__BELIMO_LM24A_MOD.C  

#include "ch_define.h"

#include "ch_alarm.h"
#include "ch_alg.h"
#include "ch_mb_device.h"
#include "ch_IO.h"
#include "ch_modbus.h"
#include "ch_belimo_lm24a_mod.h"

//==============================================================================
//------------------------ Belimo - Globals ------------------------------------
//==============================================================================
//------------------------------------------------------------------------------

//==============================================================================
//------------------------ Belimo LM24A-MOD ------------------------------------
//==============================================================================
//------------------------ Exceptions ------------------------------------------
static unsigned char BelimoModbusHandleException(unsigned char DeviceAddress, TmbErrorCode ErrorCode)
{
int i;

  if (DeviceAddress == 0)
    return (0);

  i = DeviceAddress - 1;
  switch (ErrorCode)
  {
    case mbeNoError:
      mbDevice[i].Ctrl.LostCommunication = 0;
	  mbDevice[i].Ctrl.Exception         = 0;
      return (0);
    case mbeTimeOut:
      mbDevice[i].Ctrl.LostCommunication = 1;
	  mbDevice[i].Ctrl.Exception         = 0;
      mbDevice[i].Ctrl.Connected         = 0;
	  mbDevice[i].Ctrl.DataValid         = 0;
      break;
    default:
	  mbDevice[i].Ctrl.Exception         = 1;
	  break;
  }
  return (1);
}

unsigned char BelimoModbusCreateAlarm(unsigned char DeviceAddress, int FunctionCode, int Index, int Value, int Group)
{
int i;

  if (DeviceAddress == 0)
    return (0);

  i = DeviceAddress - 1;
  if (mbDevice[i].Ctrl.LostCommunication)
  {
    if (alarm_hr_alg.mbDevice[i].Communication == 0)
      BelimoModbusClearAllAlarms(DeviceAddress, FunctionCode, Index);
    CreateAlarm(&alarm_hr_alg.mbDevice[i].Communication, FunctionCode + MB_DEVICE_COMMUNICATION_AL, Index, Value, Group, HARD_ALARM);
  }
  else
  {
    ClearAlarm(&alarm_hr_alg.mbDevice[i].Communication, FunctionCode + MB_DEVICE_COMMUNICATION_AL, Index);
	return (0);
  }
  return (1);
}

unsigned char BelimoModbusAlarmActive(unsigned char DeviceAddress)
{
int i;

  if (DeviceAddress == 0)
    return (0);

  i = DeviceAddress - 1;
  if (mbDevice[i].Ctrl.LostCommunication)
    return (1);
  else
    return (0);
}

unsigned char BelimoModbusClearAllAlarms(unsigned char DeviceAddress, int FunctionCode, int Index)
{
int i;

  if (DeviceAddress == 0)
    return (0);

  i = DeviceAddress - 1;
  ClearAlarm(&alarm_hr_alg.mbDevice[i].Communication, FunctionCode + MB_DEVICE_COMMUNICATION_AL, Index);
  return (1);
}

//------------------------ Init node -------------------------------------------
static void BelimoModbusReceiveReadHoldingRegister(TmbErrorCode ErrorCode, TmbMessage *Msg)
{
  if (BelimoModbusHandleException(Msg->Address, ErrorCode) == 0)
  {
    if ((Msg->Address > 0) && (Msg->NrDataBytes == 5))
    {
      mbDevice[Msg->Address - 1].Min = (mbReadInt(&Msg->Data[1]) + 50) / 100;
      mbDevice[Msg->Address - 1].Max = (mbReadInt(&Msg->Data[3]) + 50) / 100;
      mbDevice[Msg->Address - 1].Ctrl.Init = 0;
    }
  }
}

static void BelimoModbusReceiveWriteMultipleRegister(TmbErrorCode ErrorCode, TmbMessage *Msg)
{
  if (BelimoModbusHandleException(Msg->Address, ErrorCode) == 0)
  {
    mbReadHoldingRegister(Msg->Address, blmMin, 2, BelimoModbusReceiveReadHoldingRegister);
  }
}

static void BelimoModbusInitNode(unsigned char DeviceAddress)
{
unsigned int Data[3];

  Data[0] = 4;
  Data[1] = 0;
  Data[2] = 3600;

  mbWriteMultipleRegister(DeviceAddress, blmSensorType, 3, Data, BelimoModbusReceiveWriteMultipleRegister);
}

//------------------------ Actual value ----------------------------------------
static void BelimoModbusReceiveReadInputRegister(TmbErrorCode ErrorCode, TmbMessage *Msg)
{
int Value;

  if (BelimoModbusHandleException(Msg->Address, ErrorCode) == 0)
  {
    if ((Msg->Address > 0) && (Msg->NrDataBytes == 11))
    {
	  Value = (mbReadInt(&Msg->Data[1]) + 50) / 100;
      mbDevice[Msg->Address - 1].ActualValue    = Calc_Prop(mbDevice[Msg->Address - 1].Min, mbDevice[Msg->Address - 1].Max, 0, 100, Value);
	  mbDevice[Msg->Address - 1].SensorValue    = mbReadInt(&Msg->Data[9]);
	  mbDevice[Msg->Address - 1].Ctrl.DataValid = 1;
    }
  }
}

static void BelimoModbusGetActualValue(unsigned char DeviceAddress)
{
  mbReadInputRegister(DeviceAddress, blmRelativePosition, 5, BelimoModbusReceiveReadInputRegister);
}

//------------------------ Target value ----------------------------------------
static void BelimoModbusReceiveSetTargetValue(TmbErrorCode ErrorCode, TmbMessage *Msg)
{
  if (BelimoModbusHandleException(Msg->Address, ErrorCode) == 0)
  {
    mbDevice[Msg->Address - 1].Ctrl.SetTargetValue = 0;
	TimerSet(&mbDevice[Msg->Address - 1].TimerTargetValue, TIMER_5MIN);
  }
}

static void BelimoModbusSetTargetValue(unsigned char DeviceAddress)
{
unsigned int Value[2];

  Value[0] = mbDevice[DeviceAddress - 1].TargetValue * 100;
  Value[1] = (mbDevice[DeviceAddress - 1].TargetValue == 0) ? 2 : 0;
  mbWriteMultipleRegister(DeviceAddress, blmSetpoint, 2, Value, BelimoModbusReceiveSetTargetValue);
}

//==============================================================================
//------------------------ Belimo LM24A-MOD ------------------------------------
//==============================================================================
//------------------------------------------------------------------------------
void BelimoModbusMain(int DeviceAddress)
{
int index;

  if ((DeviceAddress < 1) || (DeviceAddress > 247))
    return;

  index = DeviceAddress - 1;
  if (mbDevice[index].Ctrl.Init)
    BelimoModbusInitNode(index + 1);
  else if (mbDevice[index].Ctrl.SetTargetValue || TimerExpired(&mbDevice[index].TimerTargetValue))
    BelimoModbusSetTargetValue(index + 1);
  else
    BelimoModbusGetActualValue(index + 1);
}

//==============================================================================
//------------------------ End of file -----------------------------------------
//==============================================================================
