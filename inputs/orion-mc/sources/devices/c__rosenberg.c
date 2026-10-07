// C__ROSENBERG.C  

#include "ch_define.h"

#include "ch_alarm.h"
#include "ch_alg.h"
#include "ch_mb_device.h"
#include "ch_IO.h"
#include "ch_modbus.h"
#include "ch_tijd.h"
#include "ch_rosenberg.h"

//==============================================================================
//------------------------ Rosenberg EC-ventilatoren - Prototyping -------------
//==============================================================================
//------------------------------------------------------------------------------
static void RosenbergReceiveVersionNumber(TmbErrorCode ErrorCode, TmbMessage *Msg);
static void RosenbergReceiveMaxRpm(TmbErrorCode ErrorCode, TmbMessage *Msg);

//==============================================================================
//------------------------ Rosenberg EC-ventilatoren - Globals -----------------
//==============================================================================
//------------------------------------------------------------------------------
static unsigned char ChangeAddress;
static unsigned char OldDeviceAddress;
static unsigned char NewDeviceAddress;

static void (*ChangeAddressCompleted)(unsigned char Failure);

static s_board_IO_on_off *IOec;
static unsigned char IOmax;

//==============================================================================
//------------------------ Rosenberg EC-ventilatoren - Communication -----------
//==============================================================================
//------------------------ Exceptions ------------------------------------------
static unsigned char RosenbergHandleException(unsigned char DeviceAddress, TmbErrorCode ErrorCode)
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

unsigned char RosenbergCreateAlarm(unsigned char DeviceAddress, int FunctionCode, int Index, int Value, int Group)
{
int i;
unsigned char Alarm = 0;
TRosenbergErrorCodes error;

  if (DeviceAddress == 0)
    return (0);

  i = DeviceAddress - 1;
  if (mbDevice[i].Ctrl.LostCommunication)
  {
    if (alarm_hr_alg.mbDevice[i].Communication == 0)
	{
      RosenbergClearAllAlarms(DeviceAddress, FunctionCode, Index);
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
	  if (error.actual.failurePowerSection)
        CreateAlarm(&alarm_hr_alg.mbDevice[i].ThermalPowerModule, FunctionCode + MB_DEVICE_FAILURE_POWER_SECTION_AL, Index, Value, Group, HARD_ALARM);
      else
        ClearAlarm(&alarm_hr_alg.mbDevice[i].ThermalPowerModule,  FunctionCode + MB_DEVICE_FAILURE_POWER_SECTION_AL, Index);

	  if (error.actual.phaseFailure)
        CreateAlarm(&alarm_hr_alg.mbDevice[i].PhaseFailure, FunctionCode + MB_DEVICE_PHASE_FAILURE_AL, Index, Value, Group, HARD_ALARM);
      else
        ClearAlarm(&alarm_hr_alg.mbDevice[i].PhaseFailure,  FunctionCode + MB_DEVICE_PHASE_FAILURE_AL, Index);

	  if (error.actual.uMax)
        CreateAlarm(&alarm_hr_alg.mbDevice[i].HighDcLinkVoltage, FunctionCode + MB_DEVICE_UMAX_AL, Index, Value, Group, HARD_ALARM);
      else
        ClearAlarm(&alarm_hr_alg.mbDevice[i].HighDcLinkVoltage,  FunctionCode + MB_DEVICE_UMAX_AL, Index);

	  if (error.actual.uMin)
        CreateAlarm(&alarm_hr_alg.mbDevice[i].LowDcLinkVoltage, FunctionCode + MB_DEVICE_UMIN_AL, Index, Value, Group, HARD_ALARM);
      else
        ClearAlarm(&alarm_hr_alg.mbDevice[i].LowDcLinkVoltage,  FunctionCode + MB_DEVICE_UMIN_AL, Index);

      if (error.actual.electronicOvertemperature)
        CreateAlarm(&alarm_hr_alg.mbDevice[i].ElectronicBoxOverHeat, FunctionCode + MB_DEVICE_ELECTRONIC_BOX_OVER_HEAT_AL, Index, Value, Group, HARD_ALARM);
      else
        ClearAlarm(&alarm_hr_alg.mbDevice[i].ElectronicBoxOverHeat,  FunctionCode + MB_DEVICE_ELECTRONIC_BOX_OVER_HEAT_AL, Index);

	  if (error.actual.overcurrent)
        CreateAlarm(&alarm_hr_alg.mbDevice[i].ExcessiveDcLinkCurrent, FunctionCode + MB_DEVICE_OVERCURRENT_AL, Index, Value, Group, HARD_ALARM);
      else
        ClearAlarm(&alarm_hr_alg.mbDevice[i].ExcessiveDcLinkCurrent,  FunctionCode + MB_DEVICE_OVERCURRENT_AL, Index);

	  if (error.actual.motorOvertemperature)
        CreateAlarm(&alarm_hr_alg.mbDevice[i].ThermalMotor, FunctionCode + MB_DEVICE_THERMAL_MOTOR_AL, Index, Value, Group, HARD_ALARM);
      else
        ClearAlarm(&alarm_hr_alg.mbDevice[i].ThermalMotor,  FunctionCode + MB_DEVICE_THERMAL_MOTOR_AL, Index);

	  if (error.actual.overspeed)
        CreateAlarm(&alarm_hr_alg.mbDevice[i].MotorFault, FunctionCode + MB_DEVICE_OVERSPEED_AL, Index, Value, Group, HARD_ALARM);
      else
        ClearAlarm(&alarm_hr_alg.mbDevice[i].MotorFault,  FunctionCode + MB_DEVICE_OVERSPEED_AL, Index);

	  if (error.actual.lockedRotor)
        CreateAlarm(&alarm_hr_alg.mbDevice[i].LockedMotor, FunctionCode + MB_DEVICE_LOCKED_ROTOR_AL, Index, Value, Group, HARD_ALARM);
      else
        ClearAlarm(&alarm_hr_alg.mbDevice[i].LockedMotor,  FunctionCode + MB_DEVICE_LOCKED_ROTOR_AL, Index);
    }
    if (alarm_hr_alg.mbDevice[i].AlarmCode != 0x0000)
      Alarm = 1;
  }
  return (Alarm);
}

unsigned char RosenbergAlarmActive(unsigned char DeviceAddress)
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

unsigned char RosenbergClearAllAlarms(unsigned char DeviceAddress, int FunctionCode, int Index)
{
int i;

  if (DeviceAddress == 0)
    return (0);

  i = DeviceAddress - 1;
  alarm_hr_alg.mbDevice[i].AlarmCode = 0;

  ClearAlarm(&alarm_hr_alg.mbDevice[i].Communication,          FunctionCode + MB_DEVICE_COMMUNICATION_AL,            Index);
  ClearAlarm(&alarm_hr_alg.mbDevice[i].ThermalPowerModule,     FunctionCode + MB_DEVICE_FAILURE_POWER_SECTION_AL,    Index);
  ClearAlarm(&alarm_hr_alg.mbDevice[i].PhaseFailure,           FunctionCode + MB_DEVICE_PHASE_FAILURE_AL,            Index);
  ClearAlarm(&alarm_hr_alg.mbDevice[i].HighDcLinkVoltage,      FunctionCode + MB_DEVICE_UMAX_AL,                     Index);
  ClearAlarm(&alarm_hr_alg.mbDevice[i].LowDcLinkVoltage,       FunctionCode + MB_DEVICE_UMIN_AL,                     Index);
  ClearAlarm(&alarm_hr_alg.mbDevice[i].ElectronicBoxOverHeat,  FunctionCode + MB_DEVICE_ELECTRONIC_BOX_OVER_HEAT_AL, Index);
  ClearAlarm(&alarm_hr_alg.mbDevice[i].ExcessiveDcLinkCurrent, FunctionCode + MB_DEVICE_OVERCURRENT_AL,              Index);
  ClearAlarm(&alarm_hr_alg.mbDevice[i].ThermalMotor,           FunctionCode + MB_DEVICE_THERMAL_MOTOR_AL,            Index);
  ClearAlarm(&alarm_hr_alg.mbDevice[i].MotorFault,             FunctionCode + MB_DEVICE_OVERSPEED_AL,                Index);
  ClearAlarm(&alarm_hr_alg.mbDevice[i].LockedMotor,            FunctionCode + MB_DEVICE_LOCKED_ROTOR_AL,             Index);

  return (1);
}

//------------------------ Addressing ------------------------------------------
static void RosenbergReceiveChangeAddress(TmbErrorCode ErrorCode, TmbMessage *Msg)
{
unsigned int  Register;
unsigned int  Value;
unsigned char Succeeded = 0;

  if (RosenbergHandleException(Msg->Address, ErrorCode) == 0)
  {
    if ((Msg->Address > 0) && (Msg->NrDataBytes == 4))
    {
      Register = mbReadInt(&Msg->Data[0]);
      Value    = mbReadInt(&Msg->Data[2]);
      switch (Register)
      {
        case rbAddress:
		  if ((Value == NewDeviceAddress) && (Msg->Address == NewDeviceAddress))
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

static void RosenbergChangeAddress(void)
{
  mbWriteSingleRegister(OldDeviceAddress, rbAddress, NewDeviceAddress, RosenbergReceiveChangeAddress);
}

unsigned char RosenbergRequestChangeAddress(unsigned char DeviceAddress, unsigned char NewAddress, s_board_IO_on_off *IO, unsigned char IO_size, void (*CompletedFunc)(unsigned char Failure))
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
static void RosenbergReceiveWriteSingleRegister(TmbErrorCode ErrorCode, TmbMessage *Msg)
{
unsigned int Register;
unsigned int Value;
unsigned int Rpm;

  if (RosenbergHandleException(Msg->Address, ErrorCode) == 0)
  {
    if ((Msg->Address > 0) && (Msg->NrDataBytes == 4))
    {
      Register = mbReadInt(&Msg->Data[0]);
      Value    = mbReadInt(&Msg->Data[2]);
      switch (Register)
      {
	    case 37: // Unknown register - has to be set to '0' according to manufacturer
		  if (Value == 0)
		    mbWriteSingleRegister(Msg->Address, rbControlMode, 0, RosenbergReceiveWriteSingleRegister);
		  break;
		case rbControlMode:
		  if (Value == 0)
		    mbWriteSingleRegister(Msg->Address, rbOperatingMode, 1, RosenbergReceiveWriteSingleRegister);
		  break;
		case rbOperatingMode:
		  if (Value == 1)
		    mbReadInputRegister(Msg->Address, rbVersionNumber, 1, RosenbergReceiveVersionNumber);
		  break;
	    case rbSetpointMode:
		  if (Value == 0)
		    mbWriteSingleRegister(Msg->Address, 37, 0, RosenbergReceiveWriteSingleRegister); // Unknown register - has to be set to '0' according to manufacturer
		  break;
		case rbControlRegister:
		  if (Value == 0x0F)
		    mbDevice[Msg->Address - 1].Ctrl.Init = 0;
		  break;
        case rbSetpointRegister:
          Rpm = ((long)mbDevice[Msg->Address - 1].TargetValue * 4096) / 100;
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

static void RosenbergReceiveVersionNumber(TmbErrorCode ErrorCode, TmbMessage *Msg)
{
  if (RosenbergHandleException(Msg->Address, ErrorCode) == 0)
  {
    if ((Msg->Address > 0) && (Msg->NrDataBytes == 3))
    {
      mbDevice[Msg->Address - 1].Version = mbReadInt(&Msg->Data[1]);
	  mbReadInputRegister(Msg->Address, rbMaximumRpm, 1, RosenbergReceiveMaxRpm);
    }
  }
}

static void RosenbergReceiveMaxRpm(TmbErrorCode ErrorCode, TmbMessage *Msg)
{
  if (RosenbergHandleException(Msg->Address, ErrorCode) == 0)
  {
    if ((Msg->Address > 0) && (Msg->NrDataBytes == 3))
    {
      mbDevice[Msg->Address - 1].MaxRpm = mbReadInt(&Msg->Data[1]);
	  if (mbDevice[Msg->Address - 1].MaxRpm > 0)
        mbWriteSingleRegister(Msg->Address, rbControlRegister, 0x0F, RosenbergReceiveWriteSingleRegister);
    }
  }
}

static void RosenbergInitVent(unsigned char DeviceAddress)
{
  mbWriteSingleRegister(DeviceAddress, rbSetpointMode, 0, RosenbergReceiveWriteSingleRegister);
}

//------------------------ Speed control ---------------------------------------
static void RosenbergSetTargetSpeed(unsigned char DeviceAddress)
{
unsigned int Rpm;

  Rpm = ((long)mbDevice[DeviceAddress - 1].TargetValue * 4096) / 100;
  mbWriteSingleRegister(DeviceAddress, rbSetpointRegister, Rpm, RosenbergReceiveWriteSingleRegister);
}

static void RosenbergReceiveInsideTemperature(TmbErrorCode ErrorCode, TmbMessage *Msg)
{
  if (RosenbergHandleException(Msg->Address, ErrorCode) == 0)
  {
    if ((Msg->Address > 0) && (Msg->NrDataBytes == 3))
    {
      mbDevice[Msg->Address - 1].TempElectronics = (mbReadInt(&Msg->Data[1]) + 5) / 10;
    }
  }
}
  
static void RosenbergReceiveErrorCode(TmbErrorCode ErrorCode, TmbMessage *Msg)
{
  if (RosenbergHandleException(Msg->Address, ErrorCode) == 0)
  {
    if ((Msg->Address > 0) && (Msg->NrDataBytes == 3))
    {
      mbDevice[Msg->Address - 1].ErrorCode = mbReadInt(&Msg->Data[1]);
      mbReadInputRegister(Msg->Address, rbInsideTemperature, 1, RosenbergReceiveInsideTemperature);
    }
  }
}

static void RosenbergReceiveActualSpeed(TmbErrorCode ErrorCode, TmbMessage *Msg)
{
  if (RosenbergHandleException(Msg->Address, ErrorCode) == 0)
  {
    if ((Msg->Address > 0) && (Msg->NrDataBytes == 3))
	{
      mbDevice[Msg->Address - 1].ActualValue = ((long)mbReadInt(&Msg->Data[1]) * 100) / mbDevice[Msg->Address - 1].MaxRpm;
      mbReadInputRegister(Msg->Address, rbFailureRegister, 1, RosenbergReceiveErrorCode);
	}
  }
}

static void RosenbergGetActualData(unsigned char DeviceAddress)
{
  mbReadInputRegister(DeviceAddress, rbRevolutionsPerMinute, 1, RosenbergReceiveActualSpeed);
}

//==============================================================================
//------------------------ ECblue Premium - Main routines ----------------------
//==============================================================================
//------------------------------------------------------------------------------
unsigned char RosenbergMain(int DeviceAddress)
{
int index;
unsigned char returnValue = 0;

  if ((DeviceAddress < 1) || (DeviceAddress > 247))
    return 1;

  index = DeviceAddress - 1;
  if (ChangeAddress)
  {
    RosenbergChangeAddress();
	returnValue = 0;
  }
  else if (mbDevice[index].Ctrl.Init)
  {
    RosenbergInitVent(index + 1);
	returnValue = 1;
  }
  else if (mbDevice[index].Ctrl.SetTargetValue || TimerExpired(&mbDevice[index].TimerTargetValue))
  {
    RosenbergSetTargetSpeed(index + 1);
	returnValue = 1;
  }
  else
  {
    RosenbergGetActualData(index + 1);
	returnValue = 1;
  }
  return returnValue;
}

//==============================================================================
//------------------------ End of file -----------------------------------------
//==============================================================================
