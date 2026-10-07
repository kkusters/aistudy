// C__ECBLUE_MODBUS.C  

#include "ch_define.h"

#include "ch_alarm.h"
#include "ch_alg.h"
#include "ch_mb_device.h"
#include "ch_IO.h"
#include "ch_modbus.h"
#include "ch_tijd.h"
#include "ch_ecblue_modbus.h"

//==============================================================================
//------------------------ ECblue - Globals ------------------------------------
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
//------------------------ ZiehlAbegg ECblue-Modbus - modBus -------------------
//==============================================================================
//------------------------ Exceptions ------------------------------------------
static unsigned char ECblueModbusHandleException(unsigned char DeviceAddress, TmbErrorCode ErrorCode)
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

unsigned char ECblueModbusCreateAlarm(unsigned char DeviceAddress, int FunctionCode, int Index, int Value, int Group)
{
int i;

  if (DeviceAddress == 0)
    return (0);

  i = DeviceAddress - 1;
  if (mbDevice[i].Ctrl.LostCommunication)
  {
    if (alarm_hr_alg.mbDevice[i].Communication == 0)
      ECblueModbusClearAllAlarms(DeviceAddress, FunctionCode, Index);
    CreateAlarm(&alarm_hr_alg.mbDevice[i].Communication, FunctionCode + MB_DEVICE_COMMUNICATION_AL, Index, Value, Group, HARD_ALARM);
  }
  else
  {
    ClearAlarm(&alarm_hr_alg.mbDevice[i].Communication, FunctionCode + MB_DEVICE_COMMUNICATION_AL, Index);
    if (alarm_hr_alg.mbDevice[i].AlarmCode != mbDevice[i].ErrorCode)
    {
      alarm_hr_alg.mbDevice[i].AlarmCode = mbDevice[i].ErrorCode;  

      if (mbDevice[i].ErrorCode & ecbIgbtFault)
        CreateAlarm(&alarm_hr_alg.mbDevice[i].IGBT, FunctionCode + MB_DEVICE_IGBT_FAULT_AL, Index, Value, Group, HARD_ALARM);
      else
        ClearAlarm(&alarm_hr_alg.mbDevice[i].IGBT,  FunctionCode + MB_DEVICE_IGBT_FAULT_AL, Index);

      if (mbDevice[i].ErrorCode & ecbEarthToGroundFault)
        CreateAlarm(&alarm_hr_alg.mbDevice[i].GroundFault, FunctionCode + MB_DEVICE_GROUND_FAULT_AL, Index, Value, Group, HARD_ALARM);
      else
        ClearAlarm(&alarm_hr_alg.mbDevice[i].GroundFault,  FunctionCode + MB_DEVICE_GROUND_FAULT_AL, Index);

      if (mbDevice[i].ErrorCode & ecbUzkHi)
        CreateAlarm(&alarm_hr_alg.mbDevice[i].UzkHi, FunctionCode + MB_DEVICE_UZK_HI_AL, Index, Value, Group, HARD_ALARM);
      else
        ClearAlarm(&alarm_hr_alg.mbDevice[i].UzkHi,  FunctionCode + MB_DEVICE_UZK_HI_AL, Index);

      if (mbDevice[i].ErrorCode & ecbUzkLo)
        CreateAlarm(&alarm_hr_alg.mbDevice[i].UzkLo, FunctionCode + MB_DEVICE_UZK_LO_AL, Index, Value, Group, HARD_ALARM);
      else
        ClearAlarm(&alarm_hr_alg.mbDevice[i].UzkLo,  FunctionCode + MB_DEVICE_UZK_LO_AL, Index);

      if (mbDevice[i].ErrorCode & ecbUinHi)
        CreateAlarm(&alarm_hr_alg.mbDevice[i].UinHi, FunctionCode + MB_DEVICE_UIN_HI_AL, Index, Value, Group, HARD_ALARM);
      else
        ClearAlarm(&alarm_hr_alg.mbDevice[i].UinHi,  FunctionCode + MB_DEVICE_UIN_HI_AL, Index);

      if (mbDevice[i].ErrorCode & ecbUinLo)
        CreateAlarm(&alarm_hr_alg.mbDevice[i].UinLo, FunctionCode + MB_DEVICE_UIN_LO_AL, Index, Value, Group, HARD_ALARM);
      else
        ClearAlarm(&alarm_hr_alg.mbDevice[i].UinLo,  FunctionCode + MB_DEVICE_UIN_LO_AL, Index);

      if (mbDevice[i].ErrorCode & ecbLineFault)
        CreateAlarm(&alarm_hr_alg.mbDevice[i].PhaseFailure, FunctionCode + MB_DEVICE_LINE_FAULT_AL, Index, Value, Group, HARD_ALARM);
      else
        ClearAlarm(&alarm_hr_alg.mbDevice[i].PhaseFailure,  FunctionCode + MB_DEVICE_LINE_FAULT_AL, Index);

      if (mbDevice[i].ErrorCode & ecbpHallSensor)
        CreateAlarm(&alarm_hr_alg.mbDevice[i].HallFailure, FunctionCode + MB_DEVICE_HALL_IC_FAULT_AL, Index, Value, Group, HARD_ALARM);
      else
        ClearAlarm(&alarm_hr_alg.mbDevice[i].HallFailure,  FunctionCode + MB_DEVICE_HALL_IC_FAULT_AL, Index);

      if (mbDevice[i].ErrorCode & ecbMotorBlocked)
        CreateAlarm(&alarm_hr_alg.mbDevice[i].LockedMotor, FunctionCode + MB_DEVICE_MOTOR_BLOCKED_AL, Index, Value, Group, HARD_ALARM);
      else
        ClearAlarm(&alarm_hr_alg.mbDevice[i].LockedMotor,  FunctionCode + MB_DEVICE_MOTOR_BLOCKED_AL, Index);

      if (mbDevice[i].ErrorCode & ecbPeakCurrunt)
        CreateAlarm(&alarm_hr_alg.mbDevice[i].ExcessiveDcLinkCurrent, FunctionCode + MB_DEVICE_OVERCURRENT_AL, Index, Value, Group, HARD_ALARM);
      else
        ClearAlarm(&alarm_hr_alg.mbDevice[i].ExcessiveDcLinkCurrent,  FunctionCode + MB_DEVICE_OVERCURRENT_AL, Index);
    }
    if ((alarm_hr_alg.mbDevice[i].AlarmCode & 0x077F) == ecbNoError)
      return (0);
  }
  return (1);
}

unsigned char ECblueModbusAlarmActive(unsigned char DeviceAddress)
{
int i;

  if (DeviceAddress == 0)
    return (0);

  i = DeviceAddress - 1;
  if (((alarm_hr_alg.mbDevice[i].AlarmCode & 0x077F) != ecbNoError) || (mbDevice[i].Ctrl.LostCommunication))
    return (1);
  else
    return (0);
}

unsigned char ECblueModbusClearAllAlarms(unsigned char DeviceAddress, int FunctionCode, int Index)
{
int i;

  if (DeviceAddress == 0)
    return (0);

  i = DeviceAddress - 1;
  alarm_hr_alg.mbDevice[i].AlarmCode = ecbNoError;

  ClearAlarm(&alarm_hr_alg.mbDevice[i].Communication,          FunctionCode + MB_DEVICE_COMMUNICATION_AL, Index);
  ClearAlarm(&alarm_hr_alg.mbDevice[i].IGBT,                   FunctionCode + MB_DEVICE_IGBT_FAULT_AL,    Index);
  ClearAlarm(&alarm_hr_alg.mbDevice[i].GroundFault,            FunctionCode + MB_DEVICE_GROUND_FAULT_AL,  Index);
  ClearAlarm(&alarm_hr_alg.mbDevice[i].UzkHi,                  FunctionCode + MB_DEVICE_UZK_HI_AL,        Index);
  ClearAlarm(&alarm_hr_alg.mbDevice[i].UzkLo,                  FunctionCode + MB_DEVICE_UZK_LO_AL,        Index);
  ClearAlarm(&alarm_hr_alg.mbDevice[i].UinHi,                  FunctionCode + MB_DEVICE_UIN_HI_AL,        Index);
  ClearAlarm(&alarm_hr_alg.mbDevice[i].UinLo,                  FunctionCode + MB_DEVICE_UIN_LO_AL,        Index);
  ClearAlarm(&alarm_hr_alg.mbDevice[i].PhaseFailure,           FunctionCode + MB_DEVICE_LINE_FAULT_AL,    Index);
  ClearAlarm(&alarm_hr_alg.mbDevice[i].HallFailure,            FunctionCode + MB_DEVICE_HALL_IC_FAULT_AL, Index);
  ClearAlarm(&alarm_hr_alg.mbDevice[i].LockedMotor,            FunctionCode + MB_DEVICE_MOTOR_BLOCKED_AL, Index);
  ClearAlarm(&alarm_hr_alg.mbDevice[i].ExcessiveDcLinkCurrent, FunctionCode + MB_DEVICE_OVERCURRENT_AL,   Index);
  return (1);
}

//------------------------ Addressing ------------------------------------------
static void ECblueModbusCheckChangeAddress(TmbErrorCode ErrorCode, TmbMessage *Msg)
{
unsigned int Value;

  if (ECblueModbusHandleException(Msg->Address, ErrorCode) == 0)
  {
    if ((Msg->Address > 0) && (Msg->NrDataBytes == 3))
    {
      Value = mbReadInt(&Msg->Data[1]);
      if (Value == (((unsigned int)NewDeviceAddress << 8) | 0x0022))
      {
        if (ChangeAddressCompleted != NULL)
          ChangeAddressCompleted(0);
        ChangeAddressCompleted = NULL;
        ChangeAddress = 0;
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
  else
  {
    if (ChangeAddressCompleted != NULL)
      ChangeAddressCompleted(1);
    ChangeAddressCompleted = NULL;
    ChangeAddress = 0;
  }
}

static void ECblueModbusTimeOutChangeAddress(TmbErrorCode ErrorCode, TmbMessage *Msg)
{
  Msg;
  ErrorCode;
  mbReadHoldingRegister(NewDeviceAddress, ecbComParameter, 1, ECblueModbusCheckChangeAddress);
}

static void ECblueModbusReceiveChangeAddress(TmbErrorCode ErrorCode, TmbMessage *Msg)
{
unsigned int Register;
unsigned int Value;

  if (ECblueModbusHandleException(Msg->Address, ErrorCode) == 0)
  {
    if ((Msg->Address > 0) && (Msg->NrDataBytes == 4))
    {
      Register = mbReadInt(&Msg->Data[0]);
      Value    = mbReadInt(&Msg->Data[2]);
      switch (Register)
      {
        case ecbComParameter:
          if (Value == (((unsigned int)NewDeviceAddress << 8) | 0x0022))
          {
            mbWriteSingleRegister(OldDeviceAddress, ecbPinInput, PIN_COMMUNICATIONS_PARAMETERS, ECblueModbusTimeOutChangeAddress);
          }
          else
          {
            if (ChangeAddressCompleted != NULL)
              ChangeAddressCompleted(1);
            ChangeAddressCompleted = NULL;
            ChangeAddress = 0;
          }
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

static void ECblueModbusChangeAddress(void)
{
unsigned int Value;

  Value = NewDeviceAddress;
  Value <<= 8;
  Value |= 0x0022;
  mbWriteSingleRegister(OldDeviceAddress, ecbComParameter, Value, ECblueModbusReceiveChangeAddress);
}

unsigned char ECblueModbusRequestChangeAddress(unsigned char DeviceAddress, unsigned char NewAddress, s_board_IO_on_off *IO, unsigned char IO_size, void (*CompletedFunc)(unsigned char Failure))
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

static void ECblueModbusReceiveWriteSingleRegister(TmbErrorCode ErrorCode, TmbMessage *Msg)
{
unsigned int Register;
unsigned int Value;
unsigned char index;
unsigned int watchdogMode, watchdogPosition;

  if (ECblueModbusHandleException(Msg->Address, ErrorCode) == 0)
  {
    if ((Msg->Address > 0) && (Msg->NrDataBytes == 4))
    {
      Register = mbReadInt(&Msg->Data[0]);
      Value    = mbReadInt(&Msg->Data[2]);
      index = Msg->Address - 1;
      switch (Register)
      {
        case ecbSpeedControl:
          if (mbDevice[index].TargetValue == Value)
          {
            mbDevice[index].Ctrl.SetTargetValue = 0;
            TimerSet(&mbDevice[index].TimerTargetValue, TIMER_5MIN);
          }
          break;
        case ecbControlMode:
          Value = mbDevice[index].RampDown;
          Value <<= 8;
          Value |= mbDevice[index].RampUp;
          mbWriteSingleRegister(Msg->Address, ecbRampTiming, Value, ECblueModbusReceiveWriteSingleRegister);
          break;
        case ecbSetIntern1:
          watchdogMode = mbDevice[index].WatchdogMode ? (mbDevice[index].WatchdogTime << 8) | 0x0002 : 0;
          mbWriteSingleRegister(Msg->Address, ecbWatchdog, watchdogMode, ECblueModbusReceiveWriteSingleRegister);
          break;
        case ecbWatchdog:
          mbDevice[index].Ctrl.Init = 0;
          break;
        case ecbRampTiming:
          watchdogPosition = mbDevice[index].WatchdogMode ? convertToRpm(mbDevice[index].WatchdogPosition, mbDevice[index].MaxRpm) : 0;
          mbWriteSingleRegister(Msg->Address, ecbSetIntern1, watchdogPosition, ECblueModbusReceiveWriteSingleRegister);
          break;
      }
    }
  }
}

static void ECblueModbusReceiveReadHoldingRegister(TmbErrorCode ErrorCode, TmbMessage *Msg)
{
unsigned int ByteCount;
unsigned int Value;

  if (ECblueModbusHandleException(Msg->Address, ErrorCode) == 0)
  {
    if ((Msg->Address > 0) && (Msg->NrDataBytes == 3))
    {
      ByteCount = Msg->Data[0];
      Value     = mbReadInt(&Msg->Data[1]);
      mbDevice[Msg->Address - 1].MaxRpm = Value;
      mbWriteSingleRegister(Msg->Address, ecbControlMode, 0x0003, ECblueModbusReceiveWriteSingleRegister);
    }
  }
}

static void ECblueModbusInitVent(unsigned char DeviceAddress)
{
  mbReadHoldingRegister(DeviceAddress, ecbMaximumSpeed, 1, ECblueModbusReceiveReadHoldingRegister);
}

static void ECblueModbusSetTargetSpeed(unsigned char DeviceAddress)
{
int test;

  test = 1;
  mbWriteSingleRegister(DeviceAddress, ecbSpeedControl, mbDevice[DeviceAddress - 1].TargetValue, ECblueModbusReceiveWriteSingleRegister);
}

//------------------------ Actual data -----------------------------------------
static void ECblueModbusReceiveActualData(TmbErrorCode ErrorCode, TmbMessage *Msg)
{
unsigned int pwr;

  if (ECblueModbusHandleException(Msg->Address, ErrorCode) == 0)
  {
    if ((Msg->Address > 0) && (Msg->NrDataBytes == 29))
    {
      mbDevice[Msg->Address - 1].ErrorCode          = mbReadInt(&Msg->Data[1]);
      mbDevice[Msg->Address - 1].ActualValue        = ((long)mbReadInt(&Msg->Data[5]) * 100) / mbDevice[Msg->Address - 1].MaxRpm;

      mbDevice[Msg->Address - 1].TempPowerModule    = (mbReadInt(&Msg->Data[21]) + 5) / 10;
      mbDevice[Msg->Address - 1].TempElectronics    = (mbReadInt(&Msg->Data[25]) + 5) / 10;
      mbDevice[Msg->Address - 1].TempMotor          = (mbReadInt(&Msg->Data[27]) + 5) / 10;

      pwr = (((long)mbReadInt(&Msg->Data[7]) * (long)mbReadInt(&Msg->Data[17])) + 50) / 100;
      mbDevice[Msg->Address - 1].EnergyConsumption  = ((long)mbDevice[Msg->Address - 1].EnergyConsumption + ((long)pwr * 2)) / 3;
    }
  }
}

static void ECblueModbusGetActualData(unsigned char DeviceAddress)
{
  mbReadInputRegister(DeviceAddress, ecbErrorStatus, 14, ECblueModbusReceiveActualData);
}

//------------------------ Broadcast -------------------------------------------
static void ECblueModbusSendBroadcastMessage(void)
{
  if (mbReadInputRegister(0, ecbSpeed, 1, 0))
    TimerRestart(&WatchdogTimer);
}

static void ECblueModbusInitWatchdogTimer(void)
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
//------------------------ ZiehlAbegg ECblue-Modbus - Main routines ------------
//==============================================================================
//------------------------------------------------------------------------------
unsigned char ECblueModbusMain(int DeviceAddress)
{
int index;
unsigned char returnValue = 0;

  if ((DeviceAddress < 1) || (DeviceAddress > 247))
    return 1;

  ECblueModbusInitWatchdogTimer();

  index = DeviceAddress - 1;
  if (ChangeAddress)
  {
    ECblueModbusChangeAddress();
	returnValue = 0;
  }
  else if (mbDeviceWatchdogEnabled && TimerExpired(&WatchdogTimer))
  {
    ECblueModbusSendBroadcastMessage();
	returnValue = 0;
  }
  else if (mbDevice[index].Ctrl.Init)
  {
    ECblueModbusInitVent(index + 1);
	returnValue = 1;
  }
  else if (mbDevice[index].Ctrl.SetTargetValue || TimerExpired(&mbDevice[index].TimerTargetValue))
  {
    ECblueModbusSetTargetSpeed(index + 1);
	returnValue = 1;
  }
  else
  {
    ECblueModbusGetActualData(index + 1);
	returnValue = 1;
  }
  return returnValue;
}

//==============================================================================
//------------------------ End of file -----------------------------------------
//==============================================================================
