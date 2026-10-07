// C__EBM_MODBUS.C  

#include "ch_define.h"

#include "ch_alarm.h"
#include "ch_alg.h"
#include "ch_mb_device.h"
#include "ch_IO.h"
#include "ch_modbus.h"
#include "ch_tijd.h"
#include "ch_ebm_modbus.h"

//==============================================================================
//------------------------ ebm modbus - Globals --------------------------------
//==============================================================================
//------------------------------------------------------------------------------
static unsigned char ChangeAddress;
static unsigned char OldDeviceAddr;
static unsigned char NewDeviceAddr;

static unsigned char ChangeMaxRpm = 0;
static unsigned char MaxRpmDeviceAddr = 0;
static unsigned int  NewMaxRpm = 0;

static void (*ChangeAddressCompleted)(unsigned char Failure);

static s_board_IO_on_off *IOec;
static unsigned char IOmax;
static unsigned char init_state = 0;
static unsigned char actual_data_state = 0;

static TTimer WatchdogTimer = {0};

//==============================================================================
//------------------------ ebm modbus - Prototyping ----------------------------
//==============================================================================
//------------------------------------------------------------------------------
static unsigned int convertSpeedToRpm(unsigned int Speed, TmbOperationMode operationMode);

//==============================================================================
//------------------------ ebm modbus - Communication --------------------------
//==============================================================================
//------------------------ Exceptions ------------------------------------------
static unsigned char ebmModbusHandleException(unsigned char DeviceAddress, TmbErrorCode ErrorCode)
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

unsigned char ebmModbusCreateAlarm(unsigned char DeviceAddress, int FunctionCode, int Index, int Value, int Group)
{
int i;
unsigned char Alarm = 0;

  if ((DeviceAddress == 0) || (DeviceAddress > MAX_MB_DEVICE))
    return (0);

  i = DeviceAddress - 1;
  if (mbDevice[i].Ctrl.LostCommunication)
  {
    if (alarm_hr_alg.mbDevice[i].Communication == 0)
      ebmModbusClearAllAlarms(DeviceAddress, FunctionCode, Index);
    CreateAlarm(&alarm_hr_alg.mbDevice[i].Communication, FunctionCode + MB_DEVICE_COMMUNICATION_AL, Index, Value, Group, HARD_ALARM);
    Alarm = 1;
  }
  else
  {
    ClearAlarm(&alarm_hr_alg.mbDevice[i].Communication, FunctionCode + MB_DEVICE_COMMUNICATION_AL, Index);
    if (alarm_hr_alg.mbDevice[i].AlarmCode != mbDevice[i].ErrorCode)
    {
      alarm_hr_alg.mbDevice[i].AlarmCode = mbDevice[i].ErrorCode;

      if (alarm_hr_alg.mbDevice[i].AlarmCode & 0x0080)
        CreateAlarm(&alarm_hr_alg.mbDevice[i].LockedMotor, FunctionCode + MB_DEVICE_LOCKED_MOTOR_AL, Index, Value, Group, HARD_ALARM);
      else
        ClearAlarm(&alarm_hr_alg.mbDevice[i].LockedMotor,  FunctionCode + MB_DEVICE_LOCKED_MOTOR_AL, Index);

      if (alarm_hr_alg.mbDevice[i].AlarmCode & 0x0040)
        CreateAlarm(&alarm_hr_alg.mbDevice[i].HallFailure, FunctionCode + MB_DEVICE_HALL_FAILURE_AL, Index, Value, Group, HARD_ALARM);
      else
        ClearAlarm(&alarm_hr_alg.mbDevice[i].HallFailure,  FunctionCode + MB_DEVICE_HALL_FAILURE_AL, Index);

      if (alarm_hr_alg.mbDevice[i].AlarmCode & 0x0020)
        CreateAlarm(&alarm_hr_alg.mbDevice[i].ThermalMotor, FunctionCode + MB_DEVICE_THERMAL_MOTOR_AL, Index, Value, Group, HARD_ALARM);
      else
        ClearAlarm(&alarm_hr_alg.mbDevice[i].ThermalMotor,  FunctionCode + MB_DEVICE_THERMAL_MOTOR_AL, Index);

      if (alarm_hr_alg.mbDevice[i].AlarmCode & 0x0008)
        CreateAlarm(&alarm_hr_alg.mbDevice[i].CommErrorMasterSlavePIC, FunctionCode + MB_DEVICE_COMM_ERROR_MASTER_SLAVE_AL, Index, Value, Group, HARD_ALARM);
      else
        ClearAlarm(&alarm_hr_alg.mbDevice[i].CommErrorMasterSlavePIC,  FunctionCode + MB_DEVICE_COMM_ERROR_MASTER_SLAVE_AL, Index);

      if (alarm_hr_alg.mbDevice[i].AlarmCode & 0x0004)
        CreateAlarm(&alarm_hr_alg.mbDevice[i].ThermalPowerModule, FunctionCode + MB_DEVICE_THERMAL_POWER_MODULE_AL, Index, Value, Group, HARD_ALARM);
      else
        ClearAlarm(&alarm_hr_alg.mbDevice[i].ThermalPowerModule,  FunctionCode + MB_DEVICE_THERMAL_POWER_MODULE_AL, Index);

      if (alarm_hr_alg.mbDevice[i].AlarmCode & 0x0001)
        CreateAlarm(&alarm_hr_alg.mbDevice[i].PhaseFailure, FunctionCode + MB_DEVICE_PHASE_FAILURE_AL, Index, Value, Group, HARD_ALARM);
      else
        ClearAlarm(&alarm_hr_alg.mbDevice[i].PhaseFailure,  FunctionCode + MB_DEVICE_PHASE_FAILURE_AL, Index);

      if (alarm_hr_alg.mbDevice[i].AlarmCode & 0x4000)
        CreateAlarm(&alarm_hr_alg.mbDevice[i].HighLineVoltage, FunctionCode + MB_DEVICE_HIGH_LINE_VOLTAGE_AL, Index, Value, Group, HARD_ALARM);
      else
        ClearAlarm(&alarm_hr_alg.mbDevice[i].HighLineVoltage,  FunctionCode + MB_DEVICE_HIGH_LINE_VOLTAGE_AL, Index);

      if (alarm_hr_alg.mbDevice[i].AlarmCode & 0x2000)
        CreateAlarm(&alarm_hr_alg.mbDevice[i].LowLineVoltage, FunctionCode + MB_DEVICE_LOW_LINE_VOLTAGE_AL, Index, Value, Group, HARD_ALARM);
      else
        ClearAlarm(&alarm_hr_alg.mbDevice[i].LowLineVoltage,  FunctionCode + MB_DEVICE_LOW_LINE_VOLTAGE_AL, Index);

      if (alarm_hr_alg.mbDevice[i].AlarmCode & 0x1000)
        CreateAlarm(&alarm_hr_alg.mbDevice[i].LowDcLinkVoltage, FunctionCode + MB_DEVICE_LOW_DC_LINK_VOLTAGE_AL, Index, Value, Group, HARD_ALARM);
      else
        ClearAlarm(&alarm_hr_alg.mbDevice[i].LowDcLinkVoltage,  FunctionCode + MB_DEVICE_LOW_DC_LINK_VOLTAGE_AL, Index);

      if (alarm_hr_alg.mbDevice[i].AlarmCode & 0x0800)
        CreateAlarm(&alarm_hr_alg.mbDevice[i].HighDcLinkVoltage, FunctionCode + MB_DEVICE_HIGH_DC_LINK_VOLTAGE_AL, Index, Value, Group, HARD_ALARM);
      else
        ClearAlarm(&alarm_hr_alg.mbDevice[i].HighDcLinkVoltage,  FunctionCode + MB_DEVICE_HIGH_DC_LINK_VOLTAGE_AL, Index);

      if (alarm_hr_alg.mbDevice[i].AlarmCode & 0x0200)
        CreateAlarm(&alarm_hr_alg.mbDevice[i].ElectronicBoxOverHeat, FunctionCode + MB_DEVICE_ELECTRONIC_BOX_OVER_HEAT_AL, Index, Value, Group, HARD_ALARM);
      else
        ClearAlarm(&alarm_hr_alg.mbDevice[i].ElectronicBoxOverHeat,  FunctionCode + MB_DEVICE_ELECTRONIC_BOX_OVER_HEAT_AL, Index);

      if (alarm_hr_alg.mbDevice[i].AlarmCode == 0x0010)
        CreateAlarm(&alarm_hr_alg.mbDevice[i].Unknown, FunctionCode + MB_DEVICE_UNKNOWN_AL, Index, Value, Group, HARD_ALARM);
      else
        ClearAlarm(&alarm_hr_alg.mbDevice[i].Unknown,  FunctionCode + MB_DEVICE_UNKNOWN_AL, Index);
    }

    if (alarm_hr_alg.mbDevice[i].WarningCode != mbDevice[i].WarningCode)
    {
      alarm_hr_alg.mbDevice[i].WarningCode = mbDevice[i].WarningCode;

      if (alarm_hr_alg.mbDevice[i].WarningCode & 0x0200)
        CreateAlarm(&alarm_hr_alg.mbDevice[i].nLow, FunctionCode + MB_DEVICE_N_LOW_AL, Index, Value, Group, ZACHT_ALARM);
      else
        ClearAlarm(&alarm_hr_alg.mbDevice[i].nLow,  FunctionCode + MB_DEVICE_N_LOW_AL, Index);

      if (alarm_hr_alg.mbDevice[i].WarningCode & 0x0080)
        CreateAlarm(&alarm_hr_alg.mbDevice[i].Brake, FunctionCode + MB_DEVICE_BRAKE_AL, Index, Value, Group, ZACHT_ALARM);
      else
        ClearAlarm(&alarm_hr_alg.mbDevice[i].Brake,  FunctionCode + MB_DEVICE_BRAKE_AL, Index);

      if (alarm_hr_alg.mbDevice[i].WarningCode & 0x0040)
        CreateAlarm(&alarm_hr_alg.mbDevice[i].UzLow, FunctionCode + MB_DEVICE_UZ_LOW_AL, Index, Value, Group, ZACHT_ALARM);
      else
        ClearAlarm(&alarm_hr_alg.mbDevice[i].UzLow,  FunctionCode + MB_DEVICE_UZ_LOW_AL, Index);    

      if (alarm_hr_alg.mbDevice[i].WarningCode & 0x0020)
        CreateAlarm(&alarm_hr_alg.mbDevice[i].TEIHigh, FunctionCode + MB_DEVICE_TEI_HIGH_AL, Index, Value, Group, ZACHT_ALARM);
      else
        ClearAlarm(&alarm_hr_alg.mbDevice[i].TEIHigh,  FunctionCode + MB_DEVICE_TEI_HIGH_AL, Index);

      if (alarm_hr_alg.mbDevice[i].WarningCode & 0x0010)
        CreateAlarm(&alarm_hr_alg.mbDevice[i].TMHigh, FunctionCode + MB_DEVICE_TM_HIGH_AL, Index, Value, Group, ZACHT_ALARM);
      else
        ClearAlarm(&alarm_hr_alg.mbDevice[i].TMHigh,  FunctionCode + MB_DEVICE_TM_HIGH_AL, Index);

      if (alarm_hr_alg.mbDevice[i].WarningCode & 0x0008)
        CreateAlarm(&alarm_hr_alg.mbDevice[i].TEHigh, FunctionCode + MB_DEVICE_TE_HIGH_AL, Index, Value, Group, ZACHT_ALARM);
      else
        ClearAlarm(&alarm_hr_alg.mbDevice[i].TEHigh,  FunctionCode + MB_DEVICE_TE_HIGH_AL, Index);

//      if (alarm_hr_alg.mbDevice[i].WarningCode & 0x0004)
//        CreateAlarm(&alarm_hr_alg.mbDevice[i].PLimit, FunctionCode + MB_DEVICE_P_LIMIT_AL, Index, Value, Group, ZACHT_ALARM);
//      else
        ClearAlarm(&alarm_hr_alg.mbDevice[i].PLimit,  FunctionCode + MB_DEVICE_P_LIMIT_AL, Index);

//      if (alarm_hr_alg.mbDevice[i].WarningCode & 0x0001)
//        CreateAlarm(&alarm_hr_alg.mbDevice[i].ILimit, FunctionCode + MB_DEVICE_I_LIMIT_AL, Index, Value, Group, ZACHT_ALARM);
//      else
        ClearAlarm(&alarm_hr_alg.mbDevice[i].ILimit,  FunctionCode + MB_DEVICE_I_LIMIT_AL, Index);
    }
    if (alarm_hr_alg.mbDevice[i].AlarmCode != 0x0000)
      Alarm = 1;
  }
  return (Alarm);
}

unsigned char ebmModbusAlarmActive(unsigned char DeviceAddress)
{
int i;

  if ((DeviceAddress == 0) || (DeviceAddress > MAX_MB_DEVICE))
    return (0);

  i = DeviceAddress - 1;
  if ((alarm_hr_alg.mbDevice[i].AlarmCode) || (mbDevice[i].Ctrl.LostCommunication))
    return (1);
  else
    return (0);
}

unsigned char ebmModbusClearAllAlarms(unsigned char DeviceAddress, int FunctionCode, int Index)
{
int i;

  if ((DeviceAddress == 0) || (DeviceAddress > MAX_MB_DEVICE))
    return (0);

  i = DeviceAddress - 1;
  alarm_hr_alg.mbDevice[i].AlarmCode = 0;
  ClearAlarm(&alarm_hr_alg.mbDevice[i].Unknown,                 FunctionCode + MB_DEVICE_UNKNOWN_AL,                   Index);
  ClearAlarm(&alarm_hr_alg.mbDevice[i].Communication,           FunctionCode + MB_DEVICE_COMMUNICATION_AL,             Index);
  ClearAlarm(&alarm_hr_alg.mbDevice[i].LockedMotor,             FunctionCode + MB_DEVICE_LOCKED_MOTOR_AL,              Index);
  ClearAlarm(&alarm_hr_alg.mbDevice[i].HallFailure,             FunctionCode + MB_DEVICE_HALL_FAILURE_AL,              Index);
  ClearAlarm(&alarm_hr_alg.mbDevice[i].ThermalMotor,            FunctionCode + MB_DEVICE_THERMAL_MOTOR_AL,             Index);
  ClearAlarm(&alarm_hr_alg.mbDevice[i].CommErrorMasterSlavePIC, FunctionCode + MB_DEVICE_COMM_ERROR_MASTER_SLAVE_AL,   Index);
  ClearAlarm(&alarm_hr_alg.mbDevice[i].ThermalPowerModule,      FunctionCode + MB_DEVICE_THERMAL_POWER_MODULE_AL,      Index);
  ClearAlarm(&alarm_hr_alg.mbDevice[i].CommErrorRemoteUnit,     FunctionCode + MB_DEVICE_COMM_ERROR_REMOTE_UNIT_AL,    Index);
  ClearAlarm(&alarm_hr_alg.mbDevice[i].PhaseFailure,            FunctionCode + MB_DEVICE_PHASE_FAILURE_AL,             Index);
  ClearAlarm(&alarm_hr_alg.mbDevice[i].HighLineVoltage,         FunctionCode + MB_DEVICE_HIGH_LINE_VOLTAGE_AL,         Index);
  ClearAlarm(&alarm_hr_alg.mbDevice[i].LowLineVoltage,          FunctionCode + MB_DEVICE_LOW_LINE_VOLTAGE_AL,          Index);
  ClearAlarm(&alarm_hr_alg.mbDevice[i].LowDcLinkVoltage,        FunctionCode + MB_DEVICE_LOW_DC_LINK_VOLTAGE_AL,       Index);
  ClearAlarm(&alarm_hr_alg.mbDevice[i].HighDcLinkVoltage,       FunctionCode + MB_DEVICE_HIGH_DC_LINK_VOLTAGE_AL,      Index);
  ClearAlarm(&alarm_hr_alg.mbDevice[i].ElectronicBoxOverHeat,   FunctionCode + MB_DEVICE_ELECTRONIC_BOX_OVER_HEAT_AL,  Index);

  ClearAlarm(&alarm_hr_alg.mbDevice[i].nLow,                    FunctionCode + MB_DEVICE_N_LOW_AL,                     Index);
  ClearAlarm(&alarm_hr_alg.mbDevice[i].Brake,                   FunctionCode + MB_DEVICE_BRAKE_AL,                     Index);
  ClearAlarm(&alarm_hr_alg.mbDevice[i].UzLow,                   FunctionCode + MB_DEVICE_UZ_LOW_AL,                    Index);    
  ClearAlarm(&alarm_hr_alg.mbDevice[i].TEHigh,                  FunctionCode + MB_DEVICE_TE_HIGH_AL,                   Index);
  ClearAlarm(&alarm_hr_alg.mbDevice[i].TMHigh,                  FunctionCode + MB_DEVICE_TM_HIGH_AL,                   Index);
  ClearAlarm(&alarm_hr_alg.mbDevice[i].TEIHigh,                 FunctionCode + MB_DEVICE_TEI_HIGH_AL,                  Index);
  ClearAlarm(&alarm_hr_alg.mbDevice[i].PLimit,                  FunctionCode + MB_DEVICE_P_LIMIT_AL,                   Index);
  ClearAlarm(&alarm_hr_alg.mbDevice[i].ILimit,                  FunctionCode + MB_DEVICE_I_LIMIT_AL,                   Index);
  return (1);
}

//------------------------ Addressing ------------------------------------------
static void ebmModbusReceiveChangeAddress(TmbErrorCode ErrorCode, TmbMessage *Msg)
{
unsigned int Register;
unsigned int Value;

  if (ebmModbusHandleException(Msg->Address, ErrorCode) == 0)
  {
    if ((Msg->Address > 0) && (Msg->NrDataBytes == 4))
    {
      Register = mbReadInt(&Msg->Data[0]);
      Value    = mbReadInt(&Msg->Data[2]);
      switch (Register)
      {
        case EM_FAN_ADDRESS:
          mbWriteSingleRegister(Msg->Address, EM_RESET, 0x08, ebmModbusReceiveChangeAddress);
          break;
        case EM_RESET:
          if (ChangeAddressCompleted != NULL)
            ChangeAddressCompleted(0);
          ChangeAddressCompleted = NULL;
          ChangeAddress = 0;
          break;
      }
    }
    else
    {
      if (ChangeAddressCompleted != NULL)
        ChangeAddressCompleted(1);
      ChangeAddressCompleted = NULL;
      ChangeAddress = 0;
    }
  }
  else
  {
    if (ChangeAddressCompleted != NULL)
      ChangeAddressCompleted(1);
    ChangeAddressCompleted = NULL;
    ChangeAddress = 0;
  }
}

static void ebmModbusChangeAddress(void)
{
  mbWriteSingleRegister(OldDeviceAddr, EM_FAN_ADDRESS, NewDeviceAddr, ebmModbusReceiveChangeAddress);
}

unsigned char ebmModbusRequestChangeAddress(unsigned char DeviceAddress, unsigned char NewAddress, s_board_IO_on_off *IO, unsigned char IO_size, void (*CompletedFunc)(unsigned char Failure))
{
  if ((DeviceAddress == 0) || (DeviceAddress > MAX_MB_DEVICE) || (NewAddress == 0) || (NewAddress > MAX_MB_DEVICE)) // Wrong address
    return (0);

  OldDeviceAddr = DeviceAddress;
  NewDeviceAddr = NewAddress;
  IOec  = IO;
  IOmax = IO_size;
  ChangeAddressCompleted = CompletedFunc;
  ChangeAddress = 1;

  return (1);
}

//------------------------ Maximum RPM -----------------------------------------
static void ebmModbusReceiveChangeMaxRpm(TmbErrorCode ErrorCode, TmbMessage *Msg)
{
  ebmModbusHandleException(Msg->Address, ErrorCode);
  mbDevice[Msg->Address - 1].Ctrl.Init = 1;
  ChangeMaxRpm = 0;
}

static void ebmModbusChangeMaxRpm(void)
{
  mbWriteSingleRegister(MaxRpmDeviceAddr, EM_MAXIMUM_SPEED, NewMaxRpm, ebmModbusReceiveChangeMaxRpm);
}

unsigned char ebmModbusRequestChangeMaxRpm(unsigned char DeviceAddress, unsigned int MaxRpm)
{
  if ((DeviceAddress == 0) || (DeviceAddress > MAX_MB_DEVICE)) // Wrong address
    return (0);

  MaxRpmDeviceAddr = DeviceAddress;
  NewMaxRpm        = MaxRpm;
  ChangeMaxRpm     = 1;

  return (1);
}

//------------------------ Initialise ------------------------------------------
static void ebmModbusReceiveInitWrite(TmbErrorCode ErrorCode, TmbMessage *Msg)
{
unsigned int id;
unsigned int data[4];

  switch(init_state)
  {
    case 0: // EM_IDENTIFICATION
      if (ebmModbusHandleException(Msg->Address, ErrorCode) == 0)
      {
        if (Msg->NrDataBytes >= 9)
        {
          id = mbReadInt(&Msg->Data[1]);
          if (id == 0x0003)
            mbDevice[Msg->Address - 1].Version = mbReadInt(&Msg->Data[7]);
          else
            mbDevice[Msg->Address - 1].Version = 0;
        }
        init_state = 1;
        mbWriteSingleRegister(Msg->Address, EM_SOURCE_SET_VALUE, 1, ebmModbusReceiveInitWrite);
      }
      break;

    case 1: // EM_SOURCE_SET_VALUE
      if (ebmModbusHandleException(Msg->Address, ErrorCode) == 0)
      {
        init_state = 2;
	    data[0] = 1;
		data[1] = 0;
		data[2] = (mbDevice[Msg->Address - 1].ClosedLoopOpenLoop == omOpenLoop) ? 2 : 0;
        mbWriteMultipleRegister(Msg->Address, EM_PARAMETER_SET_SOURCE, 3, data, ebmModbusReceiveInitWrite);
	  }
      break;

    case 2: // EM_PARAMETER_SET_SOURCE
      if (ebmModbusHandleException(Msg->Address, ErrorCode) == 0)
      {
        init_state = 3;
        mbReadHoldingRegister(Msg->Address, EM_MAXIMUM_SPEED, 2, ebmModbusReceiveInitWrite);
	  }
      break;

    case 3: // EM_MAXIMUM_SPEED
      if (ebmModbusHandleException(Msg->Address, ErrorCode) == 0)
      {
        if (Msg->NrDataBytes >= 3)
          mbDevice[Msg->Address - 1].MaxRpm = mbReadInt(&Msg->Data[1]);
        init_state = 4;
        mbReadHoldingRegister(Msg->Address, EM_REFERENCE_VOLTAGE, 2, ebmModbusReceiveInitWrite);
	  }
      break;

    case 4: // EM_REFERENCE_VOLTAGE
      if (ebmModbusHandleException(Msg->Address, ErrorCode) == 0)
      {
        if (Msg->NrDataBytes >= 5)
        {
          mbDevice[Msg->Address - 1].ReferenceVoltage = mbReadInt(&Msg->Data[1]);
          mbDevice[Msg->Address - 1].ReferenceCurrent = mbReadInt(&Msg->Data[3]);
        }
        init_state = 5;
        data[0] = (((unsigned int)mbDevice[Msg->Address - 1].RampUp   * 4) + 5) / 10;
        data[1] = (((unsigned int)mbDevice[Msg->Address - 1].RampDown * 4) + 5) / 10;
        mbWriteMultipleRegister(Msg->Address, EM_RAMP_UP_CURVE, 2, data, ebmModbusReceiveInitWrite);
      }
      break;

    case 5: // EM_RAMP_UP_CURVE
      if (ebmModbusHandleException(Msg->Address, ErrorCode) != mbeTimeOut) // Not implemented in ecInterface - will answer mbeIllegalDataAddress - continue with init
      {
        init_state = 6;
		data[0] = 0x6570;
		data[1] = 0x4D20;
		data[2] = 0x4543;
		mbWriteMultipleRegister(Msg->Address, EM_PASSWORD, 3, data, ebmModbusReceiveInitWrite);
	  }
	  break;

	case 6:
      if (ebmModbusHandleException(Msg->Address, ErrorCode) != mbeTimeOut) // Not implemented in ecInterface - will answer mbeIllegalDataAddress - continue with init
      {
        init_state = 7;
		if (mbDevice[Msg->Address - 1].WatchdogMode)
		{
		  data[0] = 2;
		  data[1] = 1;
		  data[2] = convertSpeedToRpm(mbDevice[Msg->Address - 1].WatchdogPosition, mbDevice[Msg->Address - 1].ClosedLoopOpenLoop);
		  data[3] = (unsigned int)mbDevice[Msg->Address - 1].WatchdogTime * 10;
		}
		else
		{
		  data[0] = 2;
		  data[1] = 0;
		  data[2] = 0;
		  data[3] = 0;
		}
		mbWriteMultipleRegister(Msg->Address, EM_EMERGENCY_DIRECTION, 4, data, ebmModbusReceiveInitWrite);
	  }
      break;

    case 7: // EM_EMERGENCY_DIRECTION
      if (ebmModbusHandleException(Msg->Address, ErrorCode) != mbeTimeOut) // Not implemented in ecInterface - will answer mbeIllegalDataAddress - continue with init
      {
	    init_state = 8;
        mbWriteSingleRegister(Msg->Address, EM_RESET, 0x02, ebmModbusReceiveInitWrite);
	  }
	  break;

    case 8: // EM_RESET
      if (ebmModbusHandleException(Msg->Address, ErrorCode) == 0)
      {
        mbDevice[Msg->Address - 1].Ctrl.Init = 0;
        init_state = 0;
	  }
      break;

  }
}

static void ebmModbusInitVent(unsigned char DeviceAddress)
{
  init_state = 0;
  mbReadInputRegister(DeviceAddress, EM_IDENTIFICATION, 4, ebmModbusReceiveInitWrite);
}

//------------------------ Speed control ---------------------------------------
static unsigned int convertSpeedToRpm(unsigned int Speed, TmbOperationMode operationMode)
{
  unsigned int Rpm;

  if (operationMode == omOpenLoop)
    Rpm = (((long)65535 * Speed) + 50) / 100;
  else
    Rpm = 640 * Speed;

  return Rpm;
}

static void ebmModbusReceiveWriteSingleRegister(TmbErrorCode ErrorCode, TmbMessage *Msg)
{
unsigned int Register;
unsigned int Value;
unsigned int Rpm;

  if (ebmModbusHandleException(Msg->Address, ErrorCode) == 0)
  {
    if ((Msg->Address > 0) && (Msg->NrDataBytes == 4))
    {
      Register = mbReadInt(&Msg->Data[0]);
      Value    = mbReadInt(&Msg->Data[2]);
      switch (Register)
      {
        case EM_DEFAULT_SET_VALUE:
		  Rpm = convertSpeedToRpm(mbDevice[Msg->Address - 1].TargetValue, mbDevice[Msg->Address - 1].ClosedLoopOpenLoop);
          if (Rpm == Value)
          {
            mbDevice[Msg->Address - 1].Ctrl.SetTargetValue = 0;
            TimerSet(&mbDevice[Msg->Address - 1].TimerTargetValue, TIMER_5MIN);
          }
          break;
      }
    }
  }
}

static void ebmModbusSetTargetSpeed(unsigned char DeviceAddress)
{
  unsigned int Rpm = convertSpeedToRpm(mbDevice[DeviceAddress - 1].TargetValue, mbDevice[DeviceAddress - 1].ClosedLoopOpenLoop);
  mbWriteSingleRegister(DeviceAddress, EM_DEFAULT_SET_VALUE, Rpm, ebmModbusReceiveWriteSingleRegister);
}

static void ebmModbusReceiveActualData(TmbErrorCode ErrorCode, TmbMessage *Msg)
{
  int hours, minutes;
  float u, i, x;
  unsigned int c;

  if (ebmModbusHandleException(Msg->Address, ErrorCode) == 0)
  {
    switch(actual_data_state)
    {
      case 0:
        if (Msg->NrDataBytes >= 7)
        {
		  if(mbDevice[Msg->Address - 1].ClosedLoopOpenLoop == omClosedLoop)
		  {
            mbDevice[Msg->Address - 1].ActualValue    = Calc_Prop_Long(0, 64000, 0, 100, mbReadInt(&Msg->Data[1]));
		  }
          mbDevice[Msg->Address - 1].ErrorCode          = mbReadInt(&Msg->Data[3]);
          mbDevice[Msg->Address - 1].WarningCode        = mbReadInt(&Msg->Data[5]);
        }
        actual_data_state = 1;
        mbReadInputRegister(Msg->Address, EM_CURRENT_POWER, 1, ebmModbusReceiveActualData);
        break;
      case 1:
        if (Msg->NrDataBytes >= 3)
        {
          u = 0.02 * mbDevice[Msg->Address - 1].ReferenceVoltage;
          i = 0.002 * mbDevice[Msg->Address - 1].ReferenceCurrent;
          x = ((float)mbReadInt(&Msg->Data[1])) / 65536;
          c = x * u * i;
          mbDevice[Msg->Address - 1].EnergyConsumption  = ((long)mbDevice[Msg->Address - 1].EnergyConsumption + ((long)c * 2)) / 3;
        }
        actual_data_state = 2;
        mbReadInputRegister(Msg->Address, EM_TEMP_POWER_MODULE, 5, ebmModbusReceiveActualData);
        break;
      case 2:
        if (Msg->NrDataBytes >= 11)
        {
          mbDevice[Msg->Address - 1].TempPowerModule    = mbReadInt(&Msg->Data[1]);
          mbDevice[Msg->Address - 1].TempMotor          = mbReadInt(&Msg->Data[3]);
          mbDevice[Msg->Address - 1].TempElectronics    = mbReadInt(&Msg->Data[5]);
		  if(mbDevice[Msg->Address - 1].ClosedLoopOpenLoop == omOpenLoop)
		  {
            mbDevice[Msg->Address - 1].ActualValue    = Calc_Prop_Long(0, 65536, 0, 100, mbReadInt(&Msg->Data[9]));
		  }
        }
        actual_data_state = 3;
        mbReadHoldingRegister(Msg->Address, EM_OPERATING_HOURS, 2, ebmModbusReceiveActualData);
        break;
      case 3:
        if (Msg->NrDataBytes >= 5)
        {
          hours = mbReadInt(&Msg->Data[1]);
          minutes = mbReadInt(&Msg->Data[3]);
          mbDevice[Msg->Address - 1].RunningHours = ((unsigned long)hours * 3600) + (minutes * 60);
        }
        actual_data_state = 0;
        break;
    }
  }
}

static void ebmModbusGetActualData(unsigned char DeviceAddress)
{
  actual_data_state = 0;
  mbReadInputRegister(DeviceAddress, EM_ACTUAL_SPEED, 3, ebmModbusReceiveActualData);
}

//------------------------ Broadcast -------------------------------------------
static void ebmModbusSendBroadcastMessage(void)
{
  if (mbReadInputRegister(0, EM_ACTUAL_SPEED, 1, 0))
    TimerRestart(&WatchdogTimer);
}

static void ebmModbusInitWatchdogTimer(void)
{
  if (mbDeviceWatchdogEnabled)
  {
    if (WatchdogTimer.Interval == 0)
      TimerSet(&WatchdogTimer, TIMER_1SEC * 10);
  }
  else
  {
    WatchdogTimer.Interval = 0;
  }
}

//==============================================================================
//------------------------ ebm modbus - Main routines --------------------------
//==============================================================================
//------------------------------------------------------------------------------
unsigned char ebmModbusMain(int DeviceAddress)
{
int index;
unsigned char returnValue = 0;

  if ((DeviceAddress < 1) || (DeviceAddress > 247))
    return 1;

  ebmModbusInitWatchdogTimer();

  index = DeviceAddress - 1;
  if (ChangeAddress)
  {
    ebmModbusChangeAddress();
	returnValue = 0;
  }
  else if (ChangeMaxRpm)
  {
    ebmModbusChangeMaxRpm();
	returnValue = 0;
  }
  else if (mbDeviceWatchdogEnabled && TimerExpired(&WatchdogTimer))
  {
    ebmModbusSendBroadcastMessage();
	returnValue = 0;
  }
  else if (mbDevice[index].Ctrl.Init)
  {
    ebmModbusInitVent(index + 1);
	returnValue = 1;
  }
  else if (mbDevice[index].Ctrl.SetTargetValue || TimerExpired(&mbDevice[index].TimerTargetValue))
  {
    ebmModbusSetTargetSpeed(index + 1);
	returnValue = 1;
  }
  else
  {
    ebmModbusGetActualData(index + 1);
	returnValue = 1;
  }
  return returnValue;
}

//==============================================================================
//------------------------ End of file -----------------------------------------
//==============================================================================
