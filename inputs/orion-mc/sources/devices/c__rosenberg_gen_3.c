// C__ROSENBERG_GEN_3.C  

#include "ch_define.h"

#include "ch_alarm.h"
#include "ch_alg.h"
#include "ch_mb_device.h"
#include "ch_IO.h"
#include "ch_modbus.h"
#include "ch_tijd.h"
#include "ch_rosenberg_gen_3.h"

//==============================================================================
//------------------------ Rosenberg GEN 3 EC-ventilatoren - Prototyping -------
//==============================================================================
//------------------------------------------------------------------------------
static void RosenbergGen3ReceiveVersionNumber(TmbErrorCode ErrorCode, TmbMessage *Msg);
static void RosenbergGen3ReceiveMinMaxRpm(TmbErrorCode ErrorCode, TmbMessage *Msg);
static void RosenbergGen3ReceiveWriteSingleCoil(TmbErrorCode ErrorCode, TmbMessage *Msg);

//==============================================================================
//------------------------ Rosenberg GEN 3 EC-ventilatoren - Globals -----------
//==============================================================================
//------------------------------------------------------------------------------
static unsigned char ChangeAddress;
static unsigned char OldDeviceAddress;
static unsigned char NewDeviceAddress;

static void (*ChangeAddressCompleted)(unsigned char Failure);

static s_board_IO_on_off *IOec;
static unsigned char IOmax;

//==============================================================================
//------------------------ Rosenberg GEN 3 EC-ventilatoren - Communication -----
//==============================================================================
//------------------------ Exceptions ------------------------------------------
static unsigned char RosenbergGen3HandleException(unsigned char DeviceAddress, TmbErrorCode ErrorCode)
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

unsigned char RosenbergGen3CreateAlarm(unsigned char DeviceAddress, int FunctionCode, int Index, int Value, int Group)
{
int i;

unsigned char Alarm = 0;
TRosenbergGen3ErrorCodes error;

  if (DeviceAddress == 0)
    return (0);

  i = DeviceAddress - 1;
  if (mbDevice[i].Ctrl.LostCommunication)
  {
    if (alarm_hr_alg.mbDevice[i].Communication == 0)
    {
      RosenbergGen3ClearAllAlarms(DeviceAddress, FunctionCode, Index);
      CreateAlarm(&alarm_hr_alg.mbDevice[i].Communication, FunctionCode + MB_DEVICE_COMMUNICATION_AL, Index, Value, Group, HARD_ALARM);
    }
    Alarm = 1;
  }
  else
  {
    ClearAlarm(&alarm_hr_alg.mbDevice[i].Communication, FunctionCode + MB_DEVICE_COMMUNICATION_AL, Index);
    
    if ((alarm_hr_alg.mbDevice[i].AlarmCode != mbDevice[i].ErrorCode) || 
        (alarm_hr_alg.mbDevice[i].WarningCode != mbDevice[i].WarningCode))
    {
      error.code.error = mbDevice[i].ErrorCode;
      error.code.warning = mbDevice[i].WarningCode;
      
      alarm_hr_alg.mbDevice[i].AlarmCode = mbDevice[i].ErrorCode;
      alarm_hr_alg.mbDevice[i].WarningCode = mbDevice[i].WarningCode;

      if (error.actual.underVoltage)
        CreateAlarm(&alarm_hr_alg.mbDevice[i].LowDcLinkVoltage, FunctionCode + MB_DEVICE_UMIN_AL, Index, Value, Group, HARD_ALARM);
      else
        ClearAlarm(&alarm_hr_alg.mbDevice[i].LowDcLinkVoltage,  FunctionCode + MB_DEVICE_UMIN_AL, Index);
              
      if (error.actual.overVoltage)
        CreateAlarm(&alarm_hr_alg.mbDevice[i].HighDcLinkVoltage, FunctionCode + MB_DEVICE_UMAX_AL, Index, Value, Group, HARD_ALARM);
      else
        ClearAlarm(&alarm_hr_alg.mbDevice[i].HighDcLinkVoltage,  FunctionCode + MB_DEVICE_UMAX_AL, Index);
      
      if (error.actual.overcurrentMotor)
        CreateAlarm(&alarm_hr_alg.mbDevice[i].ExcessiveDcLinkCurrent, FunctionCode + MB_DEVICE_OVERCURRENT_AL, Index, Value, Group, HARD_ALARM);
      else
        ClearAlarm(&alarm_hr_alg.mbDevice[i].ExcessiveDcLinkCurrent,  FunctionCode + MB_DEVICE_OVERCURRENT_AL, Index); 
 	  
      if (error.actual.overtemperatureElectronic)
        CreateAlarm(&alarm_hr_alg.mbDevice[i].ElectronicBoxOverHeat, FunctionCode + MB_DEVICE_ELECTRONIC_BOX_OVER_HEAT_AL, Index, Value, Group, HARD_ALARM);
      else
        ClearAlarm(&alarm_hr_alg.mbDevice[i].ElectronicBoxOverHeat,  FunctionCode + MB_DEVICE_ELECTRONIC_BOX_OVER_HEAT_AL, Index);
    
      if (error.actual.inputPhaseMissing)
        CreateAlarm(&alarm_hr_alg.mbDevice[i].InputPhaseError, FunctionCode + MB_DEVICE_INPUT_PHASE_ERROR_AL, Index, Value, Group, HARD_ALARM);
      else
        ClearAlarm(&alarm_hr_alg.mbDevice[i].InputPhaseError, FunctionCode + MB_DEVICE_INPUT_PHASE_ERROR_AL, Index);
 
      if (error.actual.blockedRotor)
        CreateAlarm(&alarm_hr_alg.mbDevice[i].LockedMotor, FunctionCode + MB_DEVICE_LOCKED_ROTOR_AL, Index, Value, Group, HARD_ALARM);
      else
        ClearAlarm(&alarm_hr_alg.mbDevice[i].LockedMotor,  FunctionCode + MB_DEVICE_LOCKED_ROTOR_AL, Index);
           	          
      if (error.actual.wrongRotationSense)
        CreateAlarm(&alarm_hr_alg.mbDevice[i].WrongDirection, FunctionCode + MB_DEVICE_WRONG_DIRECTION_AL, Index, Value, Group, HARD_ALARM);
      else
        ClearAlarm(&alarm_hr_alg.mbDevice[i].WrongDirection,  FunctionCode + MB_DEVICE_WRONG_DIRECTION_AL, Index);         
 	      
      if (error.actual.motorPhaseMissing)
        CreateAlarm(&alarm_hr_alg.mbDevice[i].PhaseFailure, FunctionCode + MB_DEVICE_MOTOR_PHASE_ERROR_AL, Index, Value, Group, HARD_ALARM);
      else
        ClearAlarm(&alarm_hr_alg.mbDevice[i].PhaseFailure,  FunctionCode + MB_DEVICE_MOTOR_PHASE_ERROR_AL, Index);
     
      if (error.actual._24VSupplyOverloaded)
        CreateAlarm(&alarm_hr_alg.mbDevice[i]._24VSupplyOverloaded, FunctionCode + MB_DEVICE_24V_SUPPLY_OVERLOADED_AL, Index, Value, Group, HARD_ALARM);
      else
        ClearAlarm(&alarm_hr_alg.mbDevice[i]._24VSupplyOverloaded,  FunctionCode + MB_DEVICE_24V_SUPPLY_OVERLOADED_AL, Index);
       
      if (error.actual.overtemperatureMotor)
        CreateAlarm(&alarm_hr_alg.mbDevice[i].ThermalMotor, FunctionCode + MB_DEVICE_THERMAL_MOTOR_AL, Index, Value, Group, HARD_ALARM);
      else
        ClearAlarm(&alarm_hr_alg.mbDevice[i].ThermalMotor,  FunctionCode + MB_DEVICE_THERMAL_MOTOR_AL, Index);

      if (error.actual.underspeed)
        CreateAlarm(&alarm_hr_alg.mbDevice[i].MotorFault, FunctionCode + MB_DEVICE_UNDERSPEED_AL, Index, Value, Group, HARD_ALARM);
      else
        ClearAlarm(&alarm_hr_alg.mbDevice[i].MotorFault,  FunctionCode + MB_DEVICE_UNDERSPEED_AL, Index);
    }
    
    if (alarm_hr_alg.mbDevice[i].AlarmCode || alarm_hr_alg.mbDevice[i].WarningCode)
      Alarm = 1;
  }
  
  return (Alarm);
}

unsigned char RosenbergGen3AlarmActive(unsigned char DeviceAddress)
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

unsigned char RosenbergGen3ClearAllAlarms(unsigned char DeviceAddress, int FunctionCode, int Index)
{
int i;

  if (DeviceAddress == 0)
    return (0);

  i = DeviceAddress - 1;
  alarm_hr_alg.mbDevice[i].AlarmCode = 0;
  alarm_hr_alg.mbDevice[i].WarningCode = 0;

  ClearAlarm(&alarm_hr_alg.mbDevice[i].Communication,          FunctionCode + MB_DEVICE_COMMUNICATION_AL,            Index);
  ClearAlarm(&alarm_hr_alg.mbDevice[i].LowDcLinkVoltage,       FunctionCode + MB_DEVICE_UMIN_AL,                     Index); 
  ClearAlarm(&alarm_hr_alg.mbDevice[i].HighDcLinkVoltage,      FunctionCode + MB_DEVICE_UMAX_AL,                     Index);  
  ClearAlarm(&alarm_hr_alg.mbDevice[i].ExcessiveDcLinkCurrent, FunctionCode + MB_DEVICE_OVERCURRENT_AL,      		 Index); 
  ClearAlarm(&alarm_hr_alg.mbDevice[i].ElectronicBoxOverHeat,  FunctionCode + MB_DEVICE_ELECTRONIC_BOX_OVER_HEAT_AL, Index);
  ClearAlarm(&alarm_hr_alg.mbDevice[i].InputPhaseError,        FunctionCode + MB_DEVICE_INPUT_PHASE_ERROR_AL,        Index);
  ClearAlarm(&alarm_hr_alg.mbDevice[i].LockedMotor,            FunctionCode + MB_DEVICE_LOCKED_ROTOR_AL,             Index);
  ClearAlarm(&alarm_hr_alg.mbDevice[i].WrongDirection,         FunctionCode + MB_DEVICE_WRONG_DIRECTION_AL,          Index);       
  ClearAlarm(&alarm_hr_alg.mbDevice[i].PhaseFailure,           FunctionCode + MB_DEVICE_MOTOR_PHASE_ERROR_AL,   	 Index);
  ClearAlarm(&alarm_hr_alg.mbDevice[i]._24VSupplyOverloaded,   FunctionCode + MB_DEVICE_24V_SUPPLY_OVERLOADED_AL,    Index); 
  ClearAlarm(&alarm_hr_alg.mbDevice[i].ThermalMotor,           FunctionCode + MB_DEVICE_THERMAL_MOTOR_AL,            Index);       
  ClearAlarm(&alarm_hr_alg.mbDevice[i].MotorFault,             FunctionCode + MB_DEVICE_UNDERSPEED_AL,               Index);

  return (1);
}

static void RosenbergGen3ResetFailures(unsigned char DeviceAddress)
{
  mbWriteSingleCoil(DeviceAddress, rbGen3ResetAlarm, 0xFF00, RosenbergGen3ReceiveWriteSingleCoil);
}

//------------------------ Addressing ------------------------------------------
static void RosenbergGen3ReceiveChangeAddress(TmbErrorCode ErrorCode, TmbMessage *Msg)
{
unsigned int  Register;
unsigned int  Value;
unsigned char Succeeded = 0;

  if (RosenbergGen3HandleException(Msg->Address, ErrorCode) == 0)
  {
    if ((Msg->Address > 0) && (Msg->NrDataBytes == 4))
    {
      Register = mbReadInt(&Msg->Data[0]);
      Value    = mbReadInt(&Msg->Data[2]);
      switch (Register)
      {
        case rbGen3ModbusAdress:
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


static void RosenbergGen3ChangeAddress(void)
{
  mbWriteSingleRegister(OldDeviceAddress, rbGen3ModbusAdress, NewDeviceAddress, RosenbergGen3ReceiveChangeAddress);
}

unsigned char RosenbergGen3RequestChangeAddress(unsigned char DeviceAddress, unsigned char NewAddress, s_board_IO_on_off *IO, unsigned char IO_size, void (*CompletedFunc)(unsigned char Failure))
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
static void RosenbergGen3ReceiveWriteSingleRegister(TmbErrorCode ErrorCode, TmbMessage *Msg)
{
unsigned int Register;
unsigned int Value;

  if (RosenbergGen3HandleException(Msg->Address, ErrorCode) == 0)
  {
    if ((Msg->Address > 0) && (Msg->NrDataBytes == 4))
    {
      Register = mbReadInt(&Msg->Data[0]);
      Value    = mbReadInt(&Msg->Data[2]);
      switch (Register)
      {
        case rbGen3Setpoint:
          if (Value / 100 == (long)mbDevice[Msg->Address - 1].TargetValue)
          {
            if (Value == 0)
              mbWriteSingleCoil(Msg->Address, rbGen3MotorOnOff, 0, RosenbergGen3ReceiveWriteSingleCoil);
            else
              mbWriteSingleCoil(Msg->Address, rbGen3MotorOnOff, 0xFF00, RosenbergGen3ReceiveWriteSingleCoil);
          }
          break;
      }
    }
  }
}

static void RosenbergGen3ReceiveWriteSingleCoil(TmbErrorCode ErrorCode, TmbMessage *Msg)
{
unsigned int Register;
unsigned int Value;

  if (RosenbergGen3HandleException(Msg->Address, ErrorCode) == 0)
  {
    if ((Msg->Address > 0) && (Msg->NrDataBytes == 4))
    {
      Register = mbReadInt(&Msg->Data[0]);
      Value    = mbReadInt(&Msg->Data[2]);
      switch (Register)
      {
        case rbGen3MotorOnOff:
          mbDevice[Msg->Address - 1].Ctrl.SetTargetValue = 0;
          TimerSet(&mbDevice[Msg->Address - 1].TimerTargetValue, TIMER_5MIN);
          break;
        case rbGen3ResetAlarm:
          mbDevice[Msg->Address - 1].Ctrl.ResetDevice = 0;	 
          break;
        case rbGen3ControlMode:
          if (Value == 0)
            mbReadInputRegister(Msg->Address, rbGen3VersionNumber, 1, RosenbergGen3ReceiveVersionNumber);		 
          break;
      }
    }
  }
}

static void RosenbergGen3ReceiveVersionNumber(TmbErrorCode ErrorCode, TmbMessage *Msg)
{
  if (RosenbergGen3HandleException(Msg->Address, ErrorCode) == 0)
  {
    if ((Msg->Address > 0) && (Msg->NrDataBytes == 3))
    {
      mbDevice[Msg->Address - 1].Version = mbReadInt(&Msg->Data[1]);
      mbReadHoldingRegister(Msg->Address, rbGen3MinRpm, 2, RosenbergGen3ReceiveMinMaxRpm);
    }
  }
}

static void RosenbergGen3ReceiveMinMaxRpm(TmbErrorCode ErrorCode, TmbMessage *Msg)
{
  if (RosenbergGen3HandleException(Msg->Address, ErrorCode) == 0)
  {
    if ((Msg->Address > 0) && (Msg->NrDataBytes == 5))
    {
      mbDevice[Msg->Address - 1].MinRpm = mbReadInt(&Msg->Data[1]);
      mbDevice[Msg->Address - 1].MaxRpm = mbReadInt(&Msg->Data[3]);
      mbDevice[Msg->Address - 1].Ctrl.Init = 0;
    }
  }
}

static void RosenbergGen3InitVent(unsigned char DeviceAddress)
{
  mbWriteSingleCoil(DeviceAddress, rbGen3ControlMode, 0, RosenbergGen3ReceiveWriteSingleCoil);
}

static void RosenbergGen3SetTargetSpeed(unsigned char DeviceAddress)
{
unsigned int Target = mbDevice[DeviceAddress - 1].TargetValue;

  unsigned int Speed = (Target) ? Target * 100 : 0;
  mbWriteSingleRegister(DeviceAddress, rbGen3Setpoint, Speed, RosenbergGen3ReceiveWriteSingleRegister);
}
	   
static void RosenbergGen3ReceiveErrorCode(TmbErrorCode ErrorCode, TmbMessage *Msg)
{
  if (RosenbergGen3HandleException(Msg->Address, ErrorCode) == 0)
  {
    if ((Msg->Address > 0) && (Msg->NrDataBytes == 5))
    {
      mbDevice[Msg->Address - 1].ErrorCode = Msg->Data[1] + (Msg->Data[2] << 8);
      mbDevice[Msg->Address - 1].WarningCode = Msg->Data[3] + (Msg->Data[4] << 8);    
    }
  }
}

static void RosenbergGen3ReceiveActualData(TmbErrorCode ErrorCode, TmbMessage *Msg)
{
unsigned int Target;

  if (RosenbergGen3HandleException(Msg->Address, ErrorCode) == 0)
  {
    if ((Msg->Address > 0) && (Msg->NrDataBytes == 15))
    {    
      Target = mbReadInt(&Msg->Data[1]); 
      mbDevice[Msg->Address - 1].ActualValue = (Target) ? Calc_Prop(mbDevice[Msg->Address - 1].MinRpm, mbDevice[Msg->Address - 1].MaxRpm, 1, 100, Target) : 0;
      mbDevice[Msg->Address - 1].TempElectronics = (mbReadInt(&Msg->Data[3]) + 5) / 10;
      mbDevice[Msg->Address - 1].EnergyConsumption = mbReadInt(&Msg->Data[9]);
      mbDevice[Msg->Address - 1].RunningHours = mbReadInt(&Msg->Data[13]) * 60;
      
      mbReadInputStatus(Msg->Address, 0, 32, RosenbergGen3ReceiveErrorCode);
    }
  }
}

static void RosenbergGen3GetActualData(unsigned char DeviceAddress)
{
  mbReadInputRegister(DeviceAddress, rbGen3Speed, 7, RosenbergGen3ReceiveActualData);
}

//==============================================================================
//------------------------ Rosenberg EC-ventilatoren GEN 3 - Main --------------
//==============================================================================
//------------------------------------------------------------------------------
unsigned char RosenbergGen3Main(int DeviceAddress)
{
int index;
unsigned char returnValue = 0;

  if ((DeviceAddress < 1) || (DeviceAddress > 247))
    return 1;

  index = DeviceAddress - 1;
  if (ChangeAddress)
  {
    RosenbergGen3ChangeAddress();
    returnValue = 0;
  }
  else if (mbDevice[index].Ctrl.Init)
  {
    RosenbergGen3InitVent(index + 1);
    returnValue = 1;
  }
  else if (mbDevice[index].Ctrl.ResetDevice)
  {
    RosenbergGen3ResetFailures(DeviceAddress);
    returnValue = 1;
  }
  else if (mbDevice[index].Ctrl.SetTargetValue || TimerExpired(&mbDevice[index].TimerTargetValue))
  {
    RosenbergGen3SetTargetSpeed(index + 1);
    returnValue = 1;
  }
  else
  {
    RosenbergGen3GetActualData(index + 1);
    returnValue = 1;
  }
  return returnValue;
}

//==============================================================================
//------------------------ End of file -----------------------------------------
//==============================================================================
