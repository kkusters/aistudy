// C__DPT_MOD.C  

#include "ch_define.h"

#include "ch_alarm.h"
#include "ch_alg.h"
#include "ch_mb_device.h"
#include "ch_IO.h"
#include "ch_modbus.h"
#include "ch_dpt_mod.h"

//==============================================================================
//------------------------ DPT-MOD - Globals -----------------------------------
//==============================================================================
//------------------------------------------------------------------------------

//==============================================================================
//------------------------ DPT-MOD - Exeptions ---------------------------------
//==============================================================================
//------------------------ Exceptions ------------------------------------------
static unsigned char DptModHandleException(unsigned char DeviceAddress, TmbErrorCode ErrorCode)
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
      break;
    default:
	  mbDevice[i].Ctrl.Exception         = 1;
	  break;
  }
  return (1);
}

unsigned char DptModCreateAlarm(unsigned char DeviceAddress, int FunctionCode, int Index, int Value, int Group)
{
int i;

  if (DeviceAddress == 0)
    return (0);

  i = DeviceAddress - 1;
  if (mbDevice[i].Ctrl.LostCommunication)
  {
    CreateAlarm(&alarm_hr_alg.mbDevice[i].Communication, FunctionCode + MB_DEVICE_COMMUNICATION_AL, Index, Value, Group, HARD_ALARM);
  }
  else
  {
    ClearAlarm(&alarm_hr_alg.mbDevice[i].Communication, FunctionCode + MB_DEVICE_COMMUNICATION_AL, Index);
	return (0);
  }
  return (1);
}

unsigned char DptModAlarmActive(unsigned char DeviceAddress)
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

unsigned char DptModClearAllAlarms(unsigned char DeviceAddress, int FunctionCode, int Index)
{
int i;

  if (DeviceAddress == 0)
    return (0);

  i = DeviceAddress - 1;
  ClearAlarm(&alarm_hr_alg.mbDevice[i].Communication, FunctionCode + MB_DEVICE_COMMUNICATION_AL, Index);
  return (1);
}

//==============================================================================
//------------------------ DPT-MOD - Communication -----------------------------
//==============================================================================
//------------------------------------------------------------------------------
static void DptModReceiveReadInputRegister(TmbErrorCode ErrorCode, TmbMessage *Msg)
{
  if (DptModHandleException(Msg->Address, ErrorCode) == 0)
  {
    if ((Msg->Address > 0) && (Msg->NrDataBytes == 5))
    {
	  mbDevice[Msg->Address - 1].Version     = mbReadInt(&Msg->Data[1]);
      mbDevice[Msg->Address - 1].ActualValue = mbReadInt(&Msg->Data[3]);
    }
  }
}

static void DptModGetActualValue(unsigned char DeviceAddress)
{
  mbReadInputRegister(DeviceAddress, dptProgramVersion, 2, DptModReceiveReadInputRegister);
}

//==============================================================================
//------------------------ DPT-MOD - Main --------------------------------------
//==============================================================================
//------------------------------------------------------------------------------
void DptModMain(int DeviceAddress)
{
int index;

  if ((DeviceAddress < 1) || (DeviceAddress > 247))
    return;

  index = DeviceAddress - 1;
  DptModGetActualValue(index + 1);
}

//==============================================================================
//------------------------ End of file -----------------------------------------
//==============================================================================
