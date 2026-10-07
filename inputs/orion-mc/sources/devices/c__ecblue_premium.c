// C__ECBLUE_PREMIUM.C  

#include "ch_define.h"

#include "ch_alarm.h"
#include "ch_alg.h"
#include "ch_mb_device.h"
#include "ch_IO.h"
#include "ch_modbus.h"
#include "ch_tijd.h"
#include "ch_ecblue_premium.h"

//==============================================================================
//------------------------ ECblue Premium - Globals ----------------------------
//==============================================================================
//------------------------------------------------------------------------------
static unsigned char ChangeAddress;
static unsigned char OldDeviceAddress;
static unsigned char NewDeviceAddress;

static void (*ChangeAddressCompleted)(unsigned char Failure);

static s_board_IO_on_off *IOec;
static unsigned char IOmax;

static TTimer WatchdogTimer = {0};

//==============================================================================
//------------------------ ECblue Premium - Communication ----------------------
//==============================================================================
//------------------------ Exceptions ------------------------------------------
static unsigned char ECbluePremiumHandleException(unsigned char DeviceAddress, TmbErrorCode ErrorCode)
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

unsigned char ECbluePremiumCreateAlarm(unsigned char DeviceAddress, int FunctionCode, int Index, int Value, int Group)
{
int i;

  if (DeviceAddress == 0)
    return (0);

  i = DeviceAddress - 1;
  if (mbDevice[i].Ctrl.LostCommunication)
  {
    if (alarm_hr_alg.mbDevice[i].Communication == 0)
      ECbluePremiumClearAllAlarms(DeviceAddress, FunctionCode, Index);
    CreateAlarm(&alarm_hr_alg.mbDevice[i].Communication, FunctionCode + MB_DEVICE_COMMUNICATION_AL, Index, Value, Group, HARD_ALARM);
  }
  else
  {
    ClearAlarm(&alarm_hr_alg.mbDevice[i].Communication, FunctionCode + MB_DEVICE_COMMUNICATION_AL, Index);
    if (alarm_hr_alg.mbDevice[i].AlarmCode != mbDevice[i].ErrorCode)
    {
      switch (alarm_hr_alg.mbDevice[i].AlarmCode)
      {
        case ecpNoError               : break;
        case ecpGeneralError          : ClearAlarm(&alarm_hr_alg.mbDevice[i].GeneralError,            FunctionCode + MB_DEVICE_GENERAL_ERROR_AL,             Index); break;
        case ecpMotorFault            : ClearAlarm(&alarm_hr_alg.mbDevice[i].MotorFault,              FunctionCode + MB_DEVICE_MOTOR_FAULT_AL,               Index); break;
        case ecpMotorBlocked          : ClearAlarm(&alarm_hr_alg.mbDevice[i].LockedMotor,             FunctionCode + MB_DEVICE_MOTOR_BLOCKED_AL,             Index); break;
        case ecpHeatSinkTemperature   : ClearAlarm(&alarm_hr_alg.mbDevice[i].HeatSinkTemperature,     FunctionCode + MB_DEVICE_HEAT_SINK_TEMPERATURE_AL,     Index); break;
        case ecpGroundFault           : ClearAlarm(&alarm_hr_alg.mbDevice[i].GroundFault,             FunctionCode + MB_DEVICE_GROUND_FAULT_AL,              Index); break;
        case ecpHallIcFault           : ClearAlarm(&alarm_hr_alg.mbDevice[i].HallFailure,             FunctionCode + MB_DEVICE_HALL_IC_FAULT_AL,             Index); break;
        case ecpOverCurrent           : ClearAlarm(&alarm_hr_alg.mbDevice[i].ExcessiveDcLinkCurrent,  FunctionCode + MB_DEVICE_OVERCURRENT_AL,               Index); break;
        case ecpLineFault             : ClearAlarm(&alarm_hr_alg.mbDevice[i].PhaseFailure,            FunctionCode + MB_DEVICE_LINE_FAULT_AL,                Index); break;
        case ecpLineInterruptHeatSink : ClearAlarm(&alarm_hr_alg.mbDevice[i].LineIntHeatSinkSensor,   FunctionCode + MB_DEVICE_LINE_INT_HEAT_SINK_SENSOR_AL, Index); break;
        case ecpDCvoltageToHigh       : ClearAlarm(&alarm_hr_alg.mbDevice[i].HighDcLinkVoltage,       FunctionCode + MB_DEVICE_DC_RES_VOLTAGE_TO_HIGH_AL,    Index); break;
        case ecpWrongDirectionRotation: ClearAlarm(&alarm_hr_alg.mbDevice[i].WrongDirection,          FunctionCode + MB_DEVICE_WRONG_DIRECTION_AL,           Index); break;
        case ecpTemperatureLowering   : ClearAlarm(&alarm_hr_alg.mbDevice[i].TemperatureLowering,     FunctionCode + MB_DEVICE_TEMPERATURE_LOWERING_AL,      Index); break;
        case ecpWrongConnection       : ClearAlarm(&alarm_hr_alg.mbDevice[i].WrongConnection,         FunctionCode + MB_DEVICE_WRONG_CONNECTION_AL,          Index); break;
        case ecpExternalFault         : ClearAlarm(&alarm_hr_alg.mbDevice[i].ExternalFault,           FunctionCode + MB_DEVICE_EXTERNAL_FAULT_AL,            Index); break;
        case ecpFactorySettingsLoaded : ClearAlarm(&alarm_hr_alg.mbDevice[i].FactorySettings,         FunctionCode + MB_DEVICE_FACTORY_SETTINGS_AL,          Index); break;
        case ecpEepError              : ClearAlarm(&alarm_hr_alg.mbDevice[i].EepError,                FunctionCode + MB_DEVICE_EEP_ERROR_AL,                 Index); break;
        case ecpRtcFaultGeneral       : ClearAlarm(&alarm_hr_alg.mbDevice[i].RtcGeneralFault,         FunctionCode + MB_DEVICE_RTC_GENERAL_FAULT_AL,         Index); break;
        case ecpRtcFaultVoltage       : ClearAlarm(&alarm_hr_alg.mbDevice[i].RtcVoltageFault,         FunctionCode + MB_DEVICE_RTC_VOLTAGE_FAULT_AL,         Index); break;
        case ecpFilterAlarm           : ClearAlarm(&alarm_hr_alg.mbDevice[i].FilterContamination,     FunctionCode + MB_DEVICE_FILTER_CONTAMINATION_AL,      Index); break;
        case ecpTransferError         : ClearAlarm(&alarm_hr_alg.mbDevice[i].TransferError,           FunctionCode + MB_DEVICE_TRANSFER_ERROR_AL,            Index); break;
        case ecpDataLineFault         : ClearAlarm(&alarm_hr_alg.mbDevice[i].DataConnectionLine,      FunctionCode + MB_DEVICE_DATA_CONNECTION_LINE_AL,      Index); break;
        case ecpDataChecksumFault     : ClearAlarm(&alarm_hr_alg.mbDevice[i].DataConnectionChecksum,  FunctionCode + MB_DEVICE_DATA_CONNECTION_CHECKSUM_AL,  Index); break;
        case ecpSensorFaultInput1     : ClearAlarm(&alarm_hr_alg.mbDevice[i].SensorFaultInput1,       FunctionCode + MB_DEVICE_SENSOR_FAULT_INPUT_1_AL,      Index); break;
        case ecpSensorFaultInput2     : ClearAlarm(&alarm_hr_alg.mbDevice[i].SensorFaultInput2,       FunctionCode + MB_DEVICE_SENSOR_FAULT_INPUT_2_AL,      Index); break;
        case ecpSensorFaultInput3     : ClearAlarm(&alarm_hr_alg.mbDevice[i].SensorFaultInput3,       FunctionCode + MB_DEVICE_SENSOR_FAULT_INPUT_3_AL,      Index); break;
        default                       : ClearAlarm(&alarm_hr_alg.mbDevice[i].Unknown,                 FunctionCode + MB_DEVICE_UNKNOWN_AL,                   Index); break;
      }
      switch (mbDevice[i].ErrorCode)
      {
        case ecpNoError               : ECbluePremiumClearAllAlarms(DeviceAddress, FunctionCode, Index); break;
        case ecpGeneralError          : CreateAlarm(&alarm_hr_alg.mbDevice[i].GeneralError,            FunctionCode + MB_DEVICE_GENERAL_ERROR_AL,             Index, Value, Group, HARD_ALARM); break;
        case ecpMotorFault            : CreateAlarm(&alarm_hr_alg.mbDevice[i].MotorFault,              FunctionCode + MB_DEVICE_MOTOR_FAULT_AL,               Index, Value, Group, HARD_ALARM); break;
        case ecpMotorBlocked          : CreateAlarm(&alarm_hr_alg.mbDevice[i].LockedMotor,             FunctionCode + MB_DEVICE_MOTOR_BLOCKED_AL,             Index, Value, Group, HARD_ALARM); break;
        case ecpHeatSinkTemperature   : CreateAlarm(&alarm_hr_alg.mbDevice[i].HeatSinkTemperature,     FunctionCode + MB_DEVICE_HEAT_SINK_TEMPERATURE_AL,     Index, Value, Group, HARD_ALARM); break;
        case ecpGroundFault           : CreateAlarm(&alarm_hr_alg.mbDevice[i].GroundFault,             FunctionCode + MB_DEVICE_GROUND_FAULT_AL,              Index, Value, Group, HARD_ALARM); break;
        case ecpHallIcFault           : CreateAlarm(&alarm_hr_alg.mbDevice[i].HallFailure,             FunctionCode + MB_DEVICE_HALL_IC_FAULT_AL,             Index, Value, Group, HARD_ALARM); break;
        case ecpOverCurrent           : CreateAlarm(&alarm_hr_alg.mbDevice[i].ExcessiveDcLinkCurrent,  FunctionCode + MB_DEVICE_OVERCURRENT_AL,               Index, Value, Group, HARD_ALARM); break;
        case ecpLineFault             : CreateAlarm(&alarm_hr_alg.mbDevice[i].PhaseFailure,            FunctionCode + MB_DEVICE_LINE_FAULT_AL,                Index, Value, Group, HARD_ALARM); break;
        case ecpLineInterruptHeatSink : CreateAlarm(&alarm_hr_alg.mbDevice[i].LineIntHeatSinkSensor,   FunctionCode + MB_DEVICE_LINE_INT_HEAT_SINK_SENSOR_AL, Index, Value, Group, HARD_ALARM); break;
        case ecpDCvoltageToHigh       : CreateAlarm(&alarm_hr_alg.mbDevice[i].HighDcLinkVoltage,       FunctionCode + MB_DEVICE_DC_RES_VOLTAGE_TO_HIGH_AL,    Index, Value, Group, HARD_ALARM); break;
        case ecpWrongDirectionRotation: CreateAlarm(&alarm_hr_alg.mbDevice[i].WrongDirection,          FunctionCode + MB_DEVICE_WRONG_DIRECTION_AL,           Index, Value, Group, HARD_ALARM); break;
        case ecpTemperatureLowering   : CreateAlarm(&alarm_hr_alg.mbDevice[i].TemperatureLowering,     FunctionCode + MB_DEVICE_TEMPERATURE_LOWERING_AL,      Index, Value, Group, HARD_ALARM); break;
        case ecpWrongConnection       : CreateAlarm(&alarm_hr_alg.mbDevice[i].WrongConnection,         FunctionCode + MB_DEVICE_WRONG_CONNECTION_AL,          Index, Value, Group, HARD_ALARM); break;
        case ecpExternalFault         : CreateAlarm(&alarm_hr_alg.mbDevice[i].ExternalFault,           FunctionCode + MB_DEVICE_EXTERNAL_FAULT_AL,            Index, Value, Group, HARD_ALARM); break;
        case ecpFactorySettingsLoaded : CreateAlarm(&alarm_hr_alg.mbDevice[i].FactorySettings,         FunctionCode + MB_DEVICE_FACTORY_SETTINGS_AL,          Index, Value, Group, HARD_ALARM); break;
        case ecpEepError              : CreateAlarm(&alarm_hr_alg.mbDevice[i].EepError,                FunctionCode + MB_DEVICE_EEP_ERROR_AL,                 Index, Value, Group, HARD_ALARM); break;
        case ecpRtcFaultGeneral       : CreateAlarm(&alarm_hr_alg.mbDevice[i].RtcGeneralFault,         FunctionCode + MB_DEVICE_RTC_GENERAL_FAULT_AL,         Index, Value, Group, HARD_ALARM); break;
        case ecpRtcFaultVoltage       : CreateAlarm(&alarm_hr_alg.mbDevice[i].RtcVoltageFault,         FunctionCode + MB_DEVICE_RTC_VOLTAGE_FAULT_AL,         Index, Value, Group, HARD_ALARM); break;
        case ecpFilterAlarm           : CreateAlarm(&alarm_hr_alg.mbDevice[i].FilterContamination,     FunctionCode + MB_DEVICE_FILTER_CONTAMINATION_AL,      Index, Value, Group, HARD_ALARM); break;
        case ecpTransferError         : CreateAlarm(&alarm_hr_alg.mbDevice[i].TransferError,           FunctionCode + MB_DEVICE_TRANSFER_ERROR_AL,            Index, Value, Group, HARD_ALARM); break;
        case ecpDataLineFault         : CreateAlarm(&alarm_hr_alg.mbDevice[i].DataConnectionLine,      FunctionCode + MB_DEVICE_DATA_CONNECTION_LINE_AL,      Index, Value, Group, HARD_ALARM); break;
        case ecpDataChecksumFault     : CreateAlarm(&alarm_hr_alg.mbDevice[i].DataConnectionChecksum,  FunctionCode + MB_DEVICE_DATA_CONNECTION_CHECKSUM_AL,  Index, Value, Group, HARD_ALARM); break;
        case ecpSensorFaultInput1     : CreateAlarm(&alarm_hr_alg.mbDevice[i].SensorFaultInput1,       FunctionCode + MB_DEVICE_SENSOR_FAULT_INPUT_1_AL,      Index, Value, Group, HARD_ALARM); break;
        case ecpSensorFaultInput2     : CreateAlarm(&alarm_hr_alg.mbDevice[i].SensorFaultInput2,       FunctionCode + MB_DEVICE_SENSOR_FAULT_INPUT_2_AL,      Index, Value, Group, HARD_ALARM); break;
        case ecpSensorFaultInput3     : CreateAlarm(&alarm_hr_alg.mbDevice[i].SensorFaultInput3,       FunctionCode + MB_DEVICE_SENSOR_FAULT_INPUT_3_AL,      Index, Value, Group, HARD_ALARM); break;
        default                       : CreateAlarm(&alarm_hr_alg.mbDevice[i].Unknown,                 FunctionCode + MB_DEVICE_UNKNOWN_AL,                   Index, Value, Group, HARD_ALARM); break;
      }
      alarm_hr_alg.mbDevice[i].AlarmCode = mbDevice[i].ErrorCode;  
    }
    if (alarm_hr_alg.mbDevice[i].AlarmCode == ecpNoError)
      return (0);
  }
  return (1);
}

unsigned char ECbluePremiumAlarmActive(unsigned char DeviceAddress)
{
int i;

  if (DeviceAddress == 0)
    return (0);

  i = DeviceAddress - 1;
  if ((alarm_hr_alg.mbDevice[i].AlarmCode != ecpNoError) || (mbDevice[i].Ctrl.LostCommunication))
    return (1);
  else
    return (0);
}

unsigned char ECbluePremiumClearAllAlarms(unsigned char DeviceAddress, int FunctionCode, int Index)
{
int i;

  if (DeviceAddress == 0)
    return (0);

  i = DeviceAddress - 1;
  alarm_hr_alg.mbDevice[i].AlarmCode = ecpNoError;

  ClearAlarm(&alarm_hr_alg.mbDevice[i].Unknown,                 FunctionCode + MB_DEVICE_UNKNOWN_AL,                   Index);
  ClearAlarm(&alarm_hr_alg.mbDevice[i].Communication,           FunctionCode + MB_DEVICE_COMMUNICATION_AL,             Index);
  ClearAlarm(&alarm_hr_alg.mbDevice[i].GeneralError,            FunctionCode + MB_DEVICE_GENERAL_ERROR_AL,             Index);
  ClearAlarm(&alarm_hr_alg.mbDevice[i].MotorFault,              FunctionCode + MB_DEVICE_MOTOR_FAULT_AL,               Index);
  ClearAlarm(&alarm_hr_alg.mbDevice[i].LockedMotor,             FunctionCode + MB_DEVICE_MOTOR_BLOCKED_AL,             Index);
  ClearAlarm(&alarm_hr_alg.mbDevice[i].HeatSinkTemperature,     FunctionCode + MB_DEVICE_HEAT_SINK_TEMPERATURE_AL,     Index);
  ClearAlarm(&alarm_hr_alg.mbDevice[i].GroundFault,             FunctionCode + MB_DEVICE_GROUND_FAULT_AL,              Index);
  ClearAlarm(&alarm_hr_alg.mbDevice[i].HallFailure,             FunctionCode + MB_DEVICE_HALL_IC_FAULT_AL,             Index);
  ClearAlarm(&alarm_hr_alg.mbDevice[i].ExcessiveDcLinkCurrent,  FunctionCode + MB_DEVICE_OVERCURRENT_AL,               Index);
  ClearAlarm(&alarm_hr_alg.mbDevice[i].PhaseFailure,            FunctionCode + MB_DEVICE_LINE_FAULT_AL,                Index);
  ClearAlarm(&alarm_hr_alg.mbDevice[i].LineIntHeatSinkSensor,   FunctionCode + MB_DEVICE_LINE_INT_HEAT_SINK_SENSOR_AL, Index);
  ClearAlarm(&alarm_hr_alg.mbDevice[i].HighDcLinkVoltage,       FunctionCode + MB_DEVICE_DC_RES_VOLTAGE_TO_HIGH_AL,    Index);
  ClearAlarm(&alarm_hr_alg.mbDevice[i].WrongDirection,          FunctionCode + MB_DEVICE_WRONG_DIRECTION_AL,           Index);
  ClearAlarm(&alarm_hr_alg.mbDevice[i].TemperatureLowering,     FunctionCode + MB_DEVICE_TEMPERATURE_LOWERING_AL,      Index);
  ClearAlarm(&alarm_hr_alg.mbDevice[i].WrongConnection,         FunctionCode + MB_DEVICE_WRONG_CONNECTION_AL,          Index);
  ClearAlarm(&alarm_hr_alg.mbDevice[i].ExternalFault,           FunctionCode + MB_DEVICE_EXTERNAL_FAULT_AL,            Index);
  ClearAlarm(&alarm_hr_alg.mbDevice[i].FactorySettings,         FunctionCode + MB_DEVICE_FACTORY_SETTINGS_AL,          Index);
  ClearAlarm(&alarm_hr_alg.mbDevice[i].EepError,                FunctionCode + MB_DEVICE_EEP_ERROR_AL,                 Index);
  ClearAlarm(&alarm_hr_alg.mbDevice[i].RtcGeneralFault,         FunctionCode + MB_DEVICE_RTC_GENERAL_FAULT_AL,         Index);
  ClearAlarm(&alarm_hr_alg.mbDevice[i].RtcVoltageFault,         FunctionCode + MB_DEVICE_RTC_VOLTAGE_FAULT_AL,         Index);
  ClearAlarm(&alarm_hr_alg.mbDevice[i].FilterContamination,     FunctionCode + MB_DEVICE_FILTER_CONTAMINATION_AL,      Index);
  ClearAlarm(&alarm_hr_alg.mbDevice[i].TransferError,           FunctionCode + MB_DEVICE_TRANSFER_ERROR_AL,            Index);
  ClearAlarm(&alarm_hr_alg.mbDevice[i].DataConnectionLine,      FunctionCode + MB_DEVICE_DATA_CONNECTION_LINE_AL,      Index);
  ClearAlarm(&alarm_hr_alg.mbDevice[i].DataConnectionChecksum,  FunctionCode + MB_DEVICE_DATA_CONNECTION_CHECKSUM_AL,  Index);
  ClearAlarm(&alarm_hr_alg.mbDevice[i].SensorFaultInput1,       FunctionCode + MB_DEVICE_SENSOR_FAULT_INPUT_1_AL,      Index);
  ClearAlarm(&alarm_hr_alg.mbDevice[i].SensorFaultInput2,       FunctionCode + MB_DEVICE_SENSOR_FAULT_INPUT_2_AL,      Index);
  ClearAlarm(&alarm_hr_alg.mbDevice[i].SensorFaultInput3,       FunctionCode + MB_DEVICE_SENSOR_FAULT_INPUT_3_AL,      Index);
  return (1);
}

//------------------------ Addressing ------------------------------------------
static void ECbluePremiumReceiveChangeAddress(TmbErrorCode ErrorCode, TmbMessage *Msg)
{
unsigned int Register;
unsigned int Value;

  if (ECbluePremiumHandleException(Msg->Address, ErrorCode) == 0)
  {
    if ((Msg->Address > 0) && (Msg->NrDataBytes == 4))
    {
      Register = mbReadInt(&Msg->Data[0]);
      Value    = mbReadInt(&Msg->Data[2]);
      switch (Register)
      {
        case ecpWriteProtectAddress:
          mbWriteSingleRegister(Msg->Address, ecpBusAddress, NewDeviceAddress, ECbluePremiumReceiveChangeAddress);
          break;
        case ecpBusAddress:
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

static void ECbluePremiumChangeAddress(void)
{
  mbWriteSingleRegister(OldDeviceAddress, ecpWriteProtectAddress, 1, ECbluePremiumReceiveChangeAddress);
}

unsigned char ECbluePremiumRequestChangeAddress(unsigned char DeviceAddress, unsigned char NewAddress, s_board_IO_on_off *IO, unsigned char IO_size, void (*CompletedFunc)(unsigned char Failure))
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
static unsigned int convertToRpm(unsigned char position, unsigned int maxRpm)
{
  return Calc_Prop(0, 100, 0, maxRpm, position);
}

static void ECbluePremiumReceiveWriteSingleRegister(TmbErrorCode ErrorCode, TmbMessage *Msg)
{
unsigned int Register;
unsigned int Value;
unsigned int data;
unsigned char index;

  if (ECbluePremiumHandleException(Msg->Address, ErrorCode) == 0)
  {
    if ((Msg->Address > 0) && (Msg->NrDataBytes == 4))
    {
      Register = mbReadInt(&Msg->Data[0]);
      Value    = mbReadInt(&Msg->Data[2]);
      index    = Msg->Address - 1;
      switch (Register)
      {
        case ecpSettingInternal_1:
          data = mbDevice[index].WatchdogMode ? convertToRpm(mbDevice[index].WatchdogPosition, mbDevice[index].MaxRpm) : 0;
          if (data == Value)
          {
            data = mbDevice[index].WatchdogMode ? mbDevice[index].WatchdogTime : 0;
            mbWriteSingleRegister(Msg->Address, ecpWatchdogTime, data, ECbluePremiumReceiveWriteSingleRegister);
          }
          break;
        case ecpSettingExternalMode:
          mbWriteSingleRegister(Msg->Address, ecpBusmodeE2, 1, ECbluePremiumReceiveWriteSingleRegister);
          break;
        case ecpWatchdogMode:
          data = mbDevice[index].WatchdogMode ? 2 : 0;
		  if (data == Value)
		  {
            mbDevice[index].Ctrl.Init = 0;
		  }
          break;
        case ecpWatchdogTime:
          data = mbDevice[index].WatchdogMode ? mbDevice[index].WatchdogTime : 0;
		  if (data == Value)
		  {
            data = mbDevice[index].WatchdogMode ? 2 : 0;
            mbWriteSingleRegister(Msg->Address, ecpWatchdogMode, data, ECbluePremiumReceiveWriteSingleRegister);
		  }
          break;
        case ecpBusmodeE2:
          data = mbDevice[index].WatchdogMode ? convertToRpm(mbDevice[index].WatchdogPosition, mbDevice[index].MaxRpm) : 0;
          mbWriteSingleRegister(Msg->Address, ecpSettingInternal_1, data, ECbluePremiumReceiveWriteSingleRegister);
          break;
        case ecpE2:
          data = ((long)mbDevice[index].TargetValue * 32767) / 100;
          if (data == Value)
          {
            mbDevice[index].Ctrl.SetTargetValue = 0;
            TimerSet(&mbDevice[index].TimerTargetValue, TIMER_5MIN);
          }
          break;
      }
    }
  }
}

static void ECbluePremiumReceiveMaxRpm(TmbErrorCode ErrorCode, TmbMessage *Msg)
{
  if (ECbluePremiumHandleException(Msg->Address, ErrorCode) == 0)
  {
    if ((Msg->Address > 0) && (Msg->NrDataBytes == 3))
    {
      mbDevice[Msg->Address - 1].MaxRpm = mbReadInt(&Msg->Data[1]);
      if (mbDevice[Msg->Address - 1].MaxRpm != 0)
        mbWriteSingleRegister(Msg->Address, ecpSettingExternalMode, 1, ECbluePremiumReceiveWriteSingleRegister);
    }
  }
}

static void ECbluePremiumInitVent(unsigned char DeviceAddress)
{
  mbReadHoldingRegister(DeviceAddress, ecpMaximumSpeed, 1, ECbluePremiumReceiveMaxRpm);
}

//------------------------ Speed control ---------------------------------------
static void ECbluePremiumSetTargetSpeed(unsigned char DeviceAddress)
{
unsigned int Speed;

  Speed = ((long)mbDevice[DeviceAddress - 1].TargetValue * 32767) / 100;
  mbWriteSingleRegister(DeviceAddress, ecpE2, Speed, ECbluePremiumReceiveWriteSingleRegister);
}

static void ECbluePremiumReceiveActualData(TmbErrorCode ErrorCode, TmbMessage *Msg)
{
unsigned int pwr;

  if ((Msg->Address > 0) && (ECbluePremiumHandleException(Msg->Address, ErrorCode) == 0))
  {
    if (Msg->NrDataBytes == 65)
    {
      mbDevice[Msg->Address - 1].ActualValue        = ((long)mbReadInt(&Msg->Data[1]) * 100) / mbDevice[Msg->Address - 1].MaxRpm;
      mbDevice[Msg->Address - 1].ErrorCode          = mbReadInt(&Msg->Data[23]);
      mbDevice[Msg->Address - 1].Version            = mbReadInt(&Msg->Data[25]);
      mbDevice[Msg->Address - 1].TempPowerModule    = (mbReadInt(&Msg->Data[47]) - 2740 + 5) / 10;
      mbDevice[Msg->Address - 1].TempElectronics    = (mbReadInt(&Msg->Data[49]) - 2740 + 5) / 10;

      pwr = ((long)mbReadInt(&Msg->Data[51]) * ((long)mbReadInt(&Msg->Data[55]) - 2740) + 50) / 100;
      mbDevice[Msg->Address - 1].EnergyConsumption  = ((long)mbDevice[Msg->Address - 1].EnergyConsumption + ((long)pwr * 2)) / 3;

      mbDevice[Msg->Address - 1].RunningHours       = mbReadLong(&Msg->Data[61]);
    }
  }
}

static void ECbluePremiumReceiveActualDataWatchdogMode(TmbErrorCode ErrorCode, TmbMessage *Msg)
{
unsigned int pwr;

  if ((Msg->Address > 0) && (ECbluePremiumHandleException(Msg->Address, ErrorCode) == 0))
  {
    if (Msg->NrDataBytes == 43)
    {
      mbDevice[Msg->Address - 1].ErrorCode          = mbReadInt(&Msg->Data[1]);
      mbDevice[Msg->Address - 1].Version            = mbReadInt(&Msg->Data[3]);
      mbDevice[Msg->Address - 1].TempPowerModule    = (mbReadInt(&Msg->Data[25]) - 2740 + 5) / 10;
      mbDevice[Msg->Address - 1].TempElectronics    = (mbReadInt(&Msg->Data[27]) - 2740 + 5) / 10;

      pwr = ((long)mbReadInt(&Msg->Data[29]) * ((long)mbReadInt(&Msg->Data[33]) - 2740) + 50) / 100;
      mbDevice[Msg->Address - 1].EnergyConsumption  = ((long)mbDevice[Msg->Address - 1].EnergyConsumption + ((long)pwr * 2)) / 3;

      mbDevice[Msg->Address - 1].RunningHours       = mbReadLong(&Msg->Data[39]);
    }
  }
}

static void ECbluePremiumGetActualData(unsigned char DeviceAddress)
{
  if ((DeviceAddress < 1) || (DeviceAddress > 247))
    return;

  if (mbDevice[DeviceAddress - 1].WatchdogMode)
    mbReadInputRegister(DeviceAddress, ecpErrorCode, 21, ECbluePremiumReceiveActualDataWatchdogMode);
  else
    mbReadInputRegister(DeviceAddress, ecpSpeed, 32, ECbluePremiumReceiveActualData);
}

//------------------------ Broadcast -------------------------------------------
static void ECbluePremiumSendBroadcastMessage(void)
{
  if (mbReadInputRegister(0, ecpSpeed, 1, 0))
    TimerRestart(&WatchdogTimer);
}

static void ECbluePremiumInitWatchdogTimer(void)
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
//------------------------ ECblue Premium - Main routines ----------------------
//==============================================================================
//------------------------------------------------------------------------------
unsigned char ECbluePremiumMain(int DeviceAddress)
{
int index;
unsigned char returnValue = 0;

  if ((DeviceAddress < 1) || (DeviceAddress > 247))
    return 1;

  ECbluePremiumInitWatchdogTimer();

  index = DeviceAddress - 1;
  if (ChangeAddress)
  {
    ECbluePremiumChangeAddress();
	returnValue = 0;
  }
  else if (mbDeviceWatchdogEnabled && TimerExpired(&WatchdogTimer))
  {
    ECbluePremiumSendBroadcastMessage();
	returnValue = 0;
  }
  else if (mbDevice[index].Ctrl.Init)
  {
    ECbluePremiumInitVent(index + 1);
	returnValue = 1;
  }
  else if (mbDevice[index].Ctrl.SetTargetValue || TimerExpired(&mbDevice[index].TimerTargetValue))
  {
    ECbluePremiumSetTargetSpeed(index + 1);
	returnValue = 1;
  }
  else
  {
    ECbluePremiumGetActualData(index + 1);
	returnValue = 1;
  }
  return returnValue;
}

//==============================================================================
//------------------------ End of file -----------------------------------------
//==============================================================================
