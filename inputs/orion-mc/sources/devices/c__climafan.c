// C__CLIMAFAN.C  

#include "ch_define.h"

#include "ch_alarm.h"
#include "ch_alg.h"
#include "ch_mb_device.h"
#include "ch_IO.h"
#include "ch_modbus.h"
#include "ch_tijd.h"
#include "ch_climafan.h"		

#define FAN_MODEL_SPECIFICATION_FAN_SPEED_MAX 11

//==============================================================================
//------------------------ Climafan EC-ventilatoren - Prototyping --------------
//==============================================================================
//------------------------------------------------------------------------------
static void ClimafanReceiveVersionNumber(TmbErrorCode ErrorCode, TmbMessage *Msg);
static void ClimafanReceiveMaxRpm(TmbErrorCode ErrorCode, TmbMessage *Msg);
static void ClimafanReceiveRunningHours(TmbErrorCode ErrorCode, TmbMessage *Msg);
static void ClimafanReceivePower(TmbErrorCode ErrorCode, TmbMessage *Msg);
static void ClimafanReceiveReference(TmbErrorCode ErrorCode, TmbMessage *Msg);

//==============================================================================
//------------------------ Climafan EC-ventilatoren - Globals ------------------
//==============================================================================
//------------------------------------------------------------------------------
static unsigned char ChangeAddress;
static unsigned char OldDeviceAddress;
static unsigned char NewDeviceAddress;
static unsigned int FanReference[10];
static void (*ChangeAddressCompleted)(unsigned char Failure);

static s_board_IO_on_off *IOec;
static unsigned char IOmax;

//==============================================================================
//------------------------ Climafan EC-ventilatoren - Communication ------------
//==============================================================================
//------------------------ Exceptions ------------------------------------------
static unsigned char ClimafanHandleException(unsigned char DeviceAddress, TmbErrorCode ErrorCode)
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

unsigned char ClimafanCreateAlarm(unsigned char DeviceAddress, int FunctionCode, int Index, int Value, int Group)
{
int i;
unsigned char Alarm = 0;
TClimafanErrorCodes error;

  if (DeviceAddress == 0)
    return (0);

  i = DeviceAddress - 1;
  if (mbDevice[i].Ctrl.LostCommunication)
  {
    if (alarm_hr_alg.mbDevice[i].Communication == 0)
	{
      ClimafanClearAllAlarms(DeviceAddress, FunctionCode, Index);
      CreateAlarm(&alarm_hr_alg.mbDevice[i].Communication, FunctionCode + MB_DEVICE_COMMUNICATION_AL, Index, Value, Group, HARD_ALARM);
	}
	Alarm = 1;
  }
  else
  {
    ClearAlarm(&alarm_hr_alg.mbDevice[i].Communication, FunctionCode + MB_DEVICE_COMMUNICATION_AL, Index);
    if (alarm_hr_alg.mbDevice[i].AlarmCode != mbDevice[i].ErrorCode)
    {
      alarm_hr_alg.mbDevice[i].AlarmCode = mbDevice[i].ErrorCode;

	  error.code = mbDevice[i].ErrorCode;

	  if (error.actual.dcbusOverCurrentProt)
  	    CreateAlarm(&alarm_hr_alg.mbDevice[i].ILimit, FunctionCode + MB_DEVICE_OVERCURRENT_AL, Index, Value, Group, HARD_ALARM);
	  else
	    ClearAlarm(&alarm_hr_alg.mbDevice[i].ILimit,  FunctionCode + MB_DEVICE_OVERCURRENT_AL, Index);

	  if (error.actual.dcbusOverVoltageProt)
	    CreateAlarm(&alarm_hr_alg.mbDevice[i].HighDcLinkVoltage, FunctionCode + MB_DEVICE_HIGH_DC_LINK_VOLTAGE_AL, Index, Value, Group, HARD_ALARM);
	  else
	    ClearAlarm(&alarm_hr_alg.mbDevice[i].HighDcLinkVoltage,  FunctionCode + MB_DEVICE_HIGH_DC_LINK_VOLTAGE_AL, Index);

	  if (error.actual.dcbusUnderVoltageProt)
	    CreateAlarm(&alarm_hr_alg.mbDevice[i].LowDcLinkVoltage, FunctionCode + MB_DEVICE_LOW_DC_LINK_VOLTAGE_AL, Index, Value, Group, HARD_ALARM);
	  else
	    ClearAlarm(&alarm_hr_alg.mbDevice[i].LowDcLinkVoltage,  FunctionCode + MB_DEVICE_LOW_DC_LINK_VOLTAGE_AL, Index);

	  if (error.actual.eeOverTemperatureProt)
	    CreateAlarm(&alarm_hr_alg.mbDevice[i].ElectronicBoxOverHeat, FunctionCode + MB_DEVICE_ELECTRONIC_BOX_OVER_HEAT_AL, Index, Value, Group, HARD_ALARM);
	  else
	    ClearAlarm(&alarm_hr_alg.mbDevice[i].ElectronicBoxOverHeat,  FunctionCode + MB_DEVICE_ELECTRONIC_BOX_OVER_HEAT_AL, Index);

	  if (error.actual.fanLock)
	    CreateAlarm(&alarm_hr_alg.mbDevice[i].LockedMotor, FunctionCode + MB_DEVICE_LOCKED_ROTOR_AL, Index, Value, Group, HARD_ALARM);
	  else
	    ClearAlarm(&alarm_hr_alg.mbDevice[i].LockedMotor,  FunctionCode + MB_DEVICE_LOCKED_ROTOR_AL, Index);

	  if (error.actual.acPhaseLose)
	    CreateAlarm(&alarm_hr_alg.mbDevice[i].PhaseFailure, FunctionCode + MB_DEVICE_PHASE_FAILURE_AL, Index, Value, Group, HARD_ALARM);
	  else
	    ClearAlarm(&alarm_hr_alg.mbDevice[i].PhaseFailure,  FunctionCode + MB_DEVICE_PHASE_FAILURE_AL, Index);

	  if (error.actual.fanReverseRun)
	    CreateAlarm(&alarm_hr_alg.mbDevice[i].WrongDirection, FunctionCode + MB_DEVICE_WRONG_DIRECTION_AL, Index, Value, Group, HARD_ALARM);
	  else
	    ClearAlarm(&alarm_hr_alg.mbDevice[i].WrongDirection,  FunctionCode + MB_DEVICE_WRONG_DIRECTION_AL, Index);

	  if (error.actual.hallSignal)
	    CreateAlarm(&alarm_hr_alg.mbDevice[i].HallFailure, FunctionCode + MB_DEVICE_HALL_IC_FAULT_AL, Index, Value, Group, HARD_ALARM);
	  else
	    ClearAlarm(&alarm_hr_alg.mbDevice[i].HallFailure,  FunctionCode + MB_DEVICE_HALL_IC_FAULT_AL, Index);

	  if (error.actual.eepromFault)
	    CreateAlarm(&alarm_hr_alg.mbDevice[i].EepError, FunctionCode + MB_DEVICE_EEP_ERROR_AL, Index, Value, Group, HARD_ALARM);
	  else
	    ClearAlarm(&alarm_hr_alg.mbDevice[i].EepError,  FunctionCode + MB_DEVICE_EEP_ERROR_AL, Index);

	  if (error.actual.dcbusPeakOverCurrent)
	    CreateAlarm(&alarm_hr_alg.mbDevice[i].ExcessiveDcLinkCurrent, FunctionCode + MB_DEVICE_EXCESSIVE_DC_LINK_CURRENT_AL, Index, Value, Group, HARD_ALARM);
	  else
	    ClearAlarm(&alarm_hr_alg.mbDevice[i].ExcessiveDcLinkCurrent,  FunctionCode + MB_DEVICE_EXCESSIVE_DC_LINK_CURRENT_AL, Index);

	  if (error.actual.acbusOverVoltageProt)
	    CreateAlarm(&alarm_hr_alg.mbDevice[i].HighLineVoltage, FunctionCode + MB_DEVICE_HIGH_LINE_VOLTAGE_AL, Index, Value, Group, HARD_ALARM);
	  else
	    ClearAlarm(&alarm_hr_alg.mbDevice[i].HighLineVoltage,  FunctionCode + MB_DEVICE_HIGH_LINE_VOLTAGE_AL, Index);

	  if (error.actual.acbusUnderVoltageProt)
	    CreateAlarm(&alarm_hr_alg.mbDevice[i].LowLineVoltage, FunctionCode + MB_DEVICE_LOW_LINE_VOLTAGE_AL, Index, Value, Group, HARD_ALARM);
	  else
	    ClearAlarm(&alarm_hr_alg.mbDevice[i].LowLineVoltage,  FunctionCode + MB_DEVICE_LOW_LINE_VOLTAGE_AL, Index);
    }
    if (alarm_hr_alg.mbDevice[i].AlarmCode != 0x0000)
      Alarm = 1;
  }
  return (Alarm);
}

unsigned char ClimafanAlarmActive(unsigned char DeviceAddress)
{
int i;

  if (DeviceAddress == 0)
    return (0);

  i = DeviceAddress - 1;
  if ((alarm_hr_alg.mbDevice[i].AlarmCode) || (mbDevice[i].Ctrl.LostCommunication))
    return (1);
  else
    return (0);
}

unsigned char ClimafanClearAllAlarms(unsigned char DeviceAddress, int FunctionCode, int Index)
{
int i;

  if (DeviceAddress == 0)
    return (0);

  i = DeviceAddress - 1;
  alarm_hr_alg.mbDevice[i].AlarmCode = 0;

  ClearAlarm(&alarm_hr_alg.mbDevice[i].ILimit,                  FunctionCode + MB_DEVICE_OVERCURRENT_AL,                Index);
  ClearAlarm(&alarm_hr_alg.mbDevice[i].HighDcLinkVoltage,       FunctionCode + MB_DEVICE_HIGH_DC_LINK_VOLTAGE_AL,       Index);
  ClearAlarm(&alarm_hr_alg.mbDevice[i].LowDcLinkVoltage,        FunctionCode + MB_DEVICE_LOW_DC_LINK_VOLTAGE_AL,        Index);
  ClearAlarm(&alarm_hr_alg.mbDevice[i].ElectronicBoxOverHeat,   FunctionCode + MB_DEVICE_ELECTRONIC_BOX_OVER_HEAT_AL,   Index);
  ClearAlarm(&alarm_hr_alg.mbDevice[i].LockedMotor,             FunctionCode + MB_DEVICE_LOCKED_ROTOR_AL,               Index);
  ClearAlarm(&alarm_hr_alg.mbDevice[i].PhaseFailure,            FunctionCode + MB_DEVICE_PHASE_FAILURE_AL,              Index);
  ClearAlarm(&alarm_hr_alg.mbDevice[i].WrongDirection,          FunctionCode + MB_DEVICE_WRONG_DIRECTION_AL,            Index);
  ClearAlarm(&alarm_hr_alg.mbDevice[i].HallFailure,             FunctionCode + MB_DEVICE_HALL_IC_FAULT_AL,              Index);
  ClearAlarm(&alarm_hr_alg.mbDevice[i].EepError,                FunctionCode + MB_DEVICE_EEP_ERROR_AL,                  Index);
  ClearAlarm(&alarm_hr_alg.mbDevice[i].ExcessiveDcLinkCurrent,  FunctionCode + MB_DEVICE_EXCESSIVE_DC_LINK_CURRENT_AL,  Index);
  ClearAlarm(&alarm_hr_alg.mbDevice[i].HighLineVoltage,         FunctionCode + MB_DEVICE_HIGH_LINE_VOLTAGE_AL,          Index);
  ClearAlarm(&alarm_hr_alg.mbDevice[i].LowLineVoltage,          FunctionCode + MB_DEVICE_LOW_LINE_VOLTAGE_AL,           Index);
  return (1);
}

//------------------------ Addressing ------------------------------------------
static void ClimafanReceiveChangeAddress(TmbErrorCode ErrorCode, TmbMessage *Msg)
{
unsigned int  Register;
unsigned int  Value;
unsigned char Succeeded = 0;

  if (ClimafanHandleException(Msg->Address, ErrorCode) == 0)
  {
    if ((Msg->Address > 0) && (Msg->NrDataBytes == 4))
    {
      Register = mbReadInt(&Msg->Data[0]);
      Value    = mbReadInt(&Msg->Data[2]);
      switch (Register)
      {
        case cfFanID:
		  if ((Value == NewDeviceAddress) && (Msg->Address == OldDeviceAddress))
		    Succeeded = 1;
		  break;
      }
    }
  }

  if (ChangeAddressCompleted != NULL)
  {
    if (Succeeded)
      ChangeAddressCompleted(0);
	else
      ChangeAddressCompleted(1);
  }
  ChangeAddressCompleted = NULL;
  ChangeAddress = 0;
}

static void ClimafanChangeAddress(void)
{
  mbWriteSingleRegister(OldDeviceAddress, cfFanID, NewDeviceAddress, ClimafanReceiveChangeAddress);
}

unsigned char ClimafanRequestChangeAddress(unsigned char DeviceAddress, unsigned char NewAddress, s_board_IO_on_off *IO, unsigned char IO_size, void (*CompletedFunc)(unsigned char Failure))
{
  if ((DeviceAddress == 0) || (DeviceAddress > MAX_MB_DEVICE) || (NewAddress == 0) || (NewAddress > MAX_MB_DEVICE)) // Wrong address
    return (0);

  OldDeviceAddress = DeviceAddress;
  NewDeviceAddress = NewAddress;
  IOec  = IO;
  IOmax = IO_size;
  ChangeAddressCompleted = CompletedFunc;
  ChangeAddress = 1;

  return (1);
}

//------------------------ Initialise ------------------------------------------
static void ClimafanReceiveWriteSingleRegister(TmbErrorCode ErrorCode, TmbMessage *Msg)
{
unsigned int Register;
unsigned int Value;
unsigned int Rpm;

  if (ClimafanHandleException(Msg->Address, ErrorCode) == 0)
  {
    if ((Msg->Address > 0) && (Msg->NrDataBytes == 4))
    {
      Register = mbReadInt(&Msg->Data[0]);
      Value    = mbReadInt(&Msg->Data[2]);
      switch (Register)
      {
		case cfFanOperatingMode:
		  if (Value == 2)
		    mbReadHoldingRegister(Msg->Address, cfFanModel, 16, ClimafanReceiveVersionNumber);
		  break;
		case cfFanStop:
		  if (Value == 0)
		    mbDevice[Msg->Address - 1].Ctrl.Init = 0;
		  break;
        case cfFanSpeedSetting:
          Rpm = (((long)mbDevice[Msg->Address - 1].TargetValue * mbDevice[Msg->Address - 1].MaxRpm) + 50) / 100;
          if (Value == Rpm)
          {
            mbDevice[Msg->Address - 1].Ctrl.SetTargetValue = 0;
            TimerSet(&mbDevice[Msg->Address - 1].TimerTargetValue, TIMER_5MIN);
          }
          break;
      }
    }
  }
}

static void ClimafanReceiveVersionNumber(TmbErrorCode ErrorCode, TmbMessage *Msg)
{
  if (ClimafanHandleException(Msg->Address, ErrorCode) == 0)
  {
    if ((Msg->Address > 0) && (Msg->NrDataBytes == 33))
    {
      mbDevice[Msg->Address - 1].Version = mbReadInt(&Msg->Data[5]);
	  mbReadHoldingRegister(Msg->Address, cfFanMaxSpeed, 1, ClimafanReceiveMaxRpm);
    }
  }
}

static void ClimafanReceiveMaxRpm(TmbErrorCode ErrorCode, TmbMessage *Msg)
{
  if (ClimafanHandleException(Msg->Address, ErrorCode) == 0)
  {
    if ((Msg->Address > 0) && (Msg->NrDataBytes == 3))
    {
      mbDevice[Msg->Address - 1].MaxRpm = mbReadInt(&Msg->Data[1]);
	  if (mbDevice[Msg->Address - 1].MaxRpm > 0)
        mbWriteSingleRegister(Msg->Address, cfFanStop, 0, ClimafanReceiveWriteSingleRegister);		//cfFanStop = 0, -> start
    }
  }
}

static void ClimafanInitVent(unsigned char DeviceAddress)
{
  mbWriteSingleRegister(DeviceAddress, cfFanOperatingMode, 2, ClimafanReceiveWriteSingleRegister);	//zet register Fan Operating Mode op 02: 010601000002
}

//------------------------ Speed control ---------------------------------------
static void ClimafanSetTargetSpeed(unsigned char DeviceAddress)
{
unsigned int Rpm;

  Rpm = (((long)mbDevice[DeviceAddress - 1].TargetValue * mbDevice[DeviceAddress - 1].MaxRpm) + 50) / 100;		//omrekenen 0-100% naar toerental 0-1020rpm
  mbWriteSingleRegister(DeviceAddress, cfFanSpeedSetting, Rpm, ClimafanReceiveWriteSingleRegister);
}

static void ClimafanReceiveInsideTemperature(TmbErrorCode ErrorCode, TmbMessage *Msg)
{
  unsigned long result;
  unsigned int sum;

  if (ClimafanHandleException(Msg->Address, ErrorCode) == 0)
  {
    if ((Msg->Address > 0) && (Msg->NrDataBytes == 3))
    {
	  sum = mbReadInt(&Msg->Data[1]);
	  result = (unsigned long) sum * FanReference[7];
	  if (FanReference != 0)
		  result = result / FanReference[6]; 
	  else
		  result = 0; // TODO JP nog bekijken
      mbDevice[Msg->Address - 1].TempElectronics = (unsigned char)result;
      mbReadInputRegister(Msg->Address, cfFanPower, 1, ClimafanReceivePower);
    }
  }
}

static void ClimafanReceivePower(TmbErrorCode ErrorCode, TmbMessage *Msg)
{
  unsigned long result;
  unsigned int sum;

  if (ClimafanHandleException(Msg->Address, ErrorCode) == 0)
  {
    if ((Msg->Address > 0) && (Msg->NrDataBytes == 3))
    {
      sum = mbReadInt(&Msg->Data[1]);
      result = (unsigned long) sum * FanReference[5];
	  if (FanReference[4] != 0)
  	    result = result / FanReference[4];
	  else
	    result = 0; // TODO JP_nog bekijken
      mbDevice[Msg->Address - 1].EnergyConsumption = (unsigned int)result;
      mbReadHoldingRegister(Msg->Address, cfOperatingTimeHours, 1, ClimafanReceiveRunningHours);
    }
  }
}
  
static void ClimafanReceiveRunningHours(TmbErrorCode ErrorCode, TmbMessage *Msg)
{
  unsigned long result;
  unsigned int sum;

  if (ClimafanHandleException(Msg->Address, ErrorCode) == 0)
  {
    if ((Msg->Address > 0) && (Msg->NrDataBytes == 3))
    {
      sum = mbReadInt(&Msg->Data[1]);
	  result = (unsigned long) sum * 3600;
      mbDevice[Msg->Address - 1].RunningHours = result;
    }
  }
}

static void ClimafanReceiveReference(TmbErrorCode ErrorCode, TmbMessage *Msg)
{
  if (ClimafanHandleException(Msg->Address, ErrorCode) == 0)
  {
    if ((Msg->Address > 0) && (Msg->NrDataBytes == 21))
    {
	    FanReference[0] = mbReadInt(&Msg->Data[1]);
	    FanReference[1] = mbReadInt(&Msg->Data[3]);
	    FanReference[2] = mbReadInt(&Msg->Data[5]);
	    FanReference[3] = mbReadInt(&Msg->Data[7]);
	    FanReference[4] = mbReadInt(&Msg->Data[9]);
	    FanReference[5] = mbReadInt(&Msg->Data[11]);
	    FanReference[6] = mbReadInt(&Msg->Data[13]);
	    FanReference[7] = mbReadInt(&Msg->Data[15]);
	    FanReference[8] = mbReadInt(&Msg->Data[17]);
	    FanReference[9] = mbReadInt(&Msg->Data[19]);
        mbReadInputRegister(Msg->Address, cfFanTemperature, 1, ClimafanReceiveInsideTemperature);
    }
  }
}


static void ClimafanReceiveErrorCode(TmbErrorCode ErrorCode, TmbMessage *Msg)
{
  if (ClimafanHandleException(Msg->Address, ErrorCode) == 0)
  {
    if ((Msg->Address > 0) && (Msg->NrDataBytes == 3))
    {
      mbDevice[Msg->Address - 1].ErrorCode = mbReadInt(&Msg->Data[1]);
      mbReadHoldingRegister(Msg->Address, cfFanReference, 10, ClimafanReceiveReference);
    }
  }
}

static void ClimafanReceiveActualSpeed(TmbErrorCode ErrorCode, TmbMessage *Msg)
{
  if (ClimafanHandleException(Msg->Address, ErrorCode) == 0)
  {
    if ((Msg->Address > 0) && (Msg->NrDataBytes == 3))
	{
	  if (mbDevice[Msg->Address - 1].MaxRpm != 0)
        mbDevice[Msg->Address - 1].ActualValue = (((long)mbReadInt(&Msg->Data[1]) * 100) + (mbDevice[Msg->Address - 1].MaxRpm / 2)) / mbDevice[Msg->Address - 1].MaxRpm;
	  else
	    mbDevice[Msg->Address - 1].ActualValue = 0; // TODO JP nog bekijken
      mbReadInputRegister(Msg->Address, cfFanStatus, 1, ClimafanReceiveErrorCode);
	}
  }
}

static void ClimafanGetActualData(unsigned char DeviceAddress)
{
  mbReadInputRegister(DeviceAddress, cfFanActualSpeed, 1, ClimafanReceiveActualSpeed);
}

//==============================================================================
//------------------------ ECblue Premium - Main routines ----------------------
//==============================================================================
//------------------------------------------------------------------------------
unsigned char ClimafanMain(int DeviceAddress)
{
int index;
unsigned char returnValue = 0;

  if ((DeviceAddress < 1) || (DeviceAddress > 247))
    return 1;

  index = DeviceAddress - 1;
  if (ChangeAddress)
  {
    ClimafanChangeAddress();
	returnValue = 0;
  }
  else if (mbDevice[index].Ctrl.Init)
  {
    ClimafanInitVent(index + 1);
	returnValue = 1;
  }
  else if (mbDevice[index].Ctrl.SetTargetValue || TimerExpired(&mbDevice[index].TimerTargetValue))
  {
    ClimafanSetTargetSpeed(index + 1);
	returnValue = 1;
  }
  else
  {
    ClimafanGetActualData(index + 1);
	returnValue = 1;
  }
  return returnValue;
}

//==============================================================================
//------------------------ End of file -----------------------------------------
//==============================================================================
