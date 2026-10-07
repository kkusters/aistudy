// C__NICOTRA_GEBHARDT.C  

#include "ch_define.h"

#include "ch_alarm.h"
#include "ch_alg.h"
#include "ch_mb_device.h"
#include "ch_IO.h"
#include "ch_modbus.h"
#include "ch_tijd.h"
#include "ch_nicotra_gebhardt.h"

//==============================================================================
//------------------------ Nicotra Gebhardt EC-ventilatoren - Prototyping ------
//==============================================================================
//------------------------------------------------------------------------------
static void NicotraGebhardtReceiveMinMaxRpm(TmbErrorCode ErrorCode, TmbMessage *Msg);
static void NicotraGebhardtReceiveWriteSingleRegister(TmbErrorCode ErrorCode, TmbMessage *Msg);

//==============================================================================
//------------------------ Nicotra Gebhardt EC-ventilatoren - Globals ----------
//==============================================================================
//------------------------------------------------------------------------------
static unsigned char ChangeAddress;
static unsigned char OldDeviceAddress;
static unsigned char NewDeviceAddress;

static void (*ChangeAddressCompleted)(unsigned char Failure);

static s_board_IO_on_off *IOec;
static unsigned char IOmax;

//==============================================================================
//------------------------ Nicotra Gebhardt EC-ventilatoren - Communication ----
//==============================================================================
//------------------------ Exceptions ------------------------------------------
static unsigned char NicotraGebhardtHandleException(unsigned char DeviceAddress, TmbErrorCode ErrorCode)
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

unsigned char NicotraGebhardtCreateAlarm(unsigned char DeviceAddress, int FunctionCode, int Index, int Value, int Group)
{
  int i;
  unsigned char Alarm = 0;
  unsigned int ErrorCode1 = 0;
  unsigned int ErrorCode2 = 0;

  if (DeviceAddress == 0)
    return (0);

  i = DeviceAddress - 1;
  if (mbDevice[i].Ctrl.LostCommunication)
  {
    if (alarm_hr_alg.mbDevice[i].Communication == 0)
    {
      NicotraGebhardtClearAllAlarms(DeviceAddress, FunctionCode, Index);
      CreateAlarm(&alarm_hr_alg.mbDevice[i].Communication, FunctionCode + MB_DEVICE_COMMUNICATION_AL, Index, Value, Group, HARD_ALARM);
    }
    Alarm = 1;
  }
  else
  {
    ClearAlarm(&alarm_hr_alg.mbDevice[i].Communication, FunctionCode + MB_DEVICE_COMMUNICATION_AL, Index);
    
    // Handle alarm 1 -> bit 10
	if (alarm_hr_alg.mbDevice[i].WarningCode != mbDevice[i].WarningCode)
    {
      alarm_hr_alg.mbDevice[i].WarningCode = mbDevice[i].WarningCode;
	  ErrorCode1 = mbDevice[i].WarningCode; 

      if (ErrorCode1 == nicoGebMemoryError)
        CreateAlarm(&alarm_hr_alg.mbDevice[i].MemoryError, FunctionCode + MB_DEVICE_MEMORY_ERROR_AL, Index, Value, Group, HARD_ALARM);
      else
        ClearAlarm(&alarm_hr_alg.mbDevice[i].MemoryError,  FunctionCode + MB_DEVICE_MEMORY_ERROR_AL, Index); 

      if (ErrorCode1 == nicoGebShortCircuit)
        CreateAlarm(&alarm_hr_alg.mbDevice[i].ShortCircuit, FunctionCode + MB_DEVICE_SHORT_CIRCUIT_AL, Index, Value, Group, HARD_ALARM);
      else
        ClearAlarm(&alarm_hr_alg.mbDevice[i].ShortCircuit,  FunctionCode + MB_DEVICE_SHORT_CIRCUIT_AL, Index); 

      if (ErrorCode1 == nicoGebLossOfSynchronism)
        CreateAlarm(&alarm_hr_alg.mbDevice[i].LossOfSynchronism, FunctionCode + MB_DEVICE_LOSS_OF_SYNCHRONISM_AL, Index, Value, Group, HARD_ALARM);
      else
        ClearAlarm(&alarm_hr_alg.mbDevice[i].LossOfSynchronism,  FunctionCode + MB_DEVICE_LOSS_OF_SYNCHRONISM_AL, Index); 
    }

    // Handle alarm 2 -> bit 17
	if (alarm_hr_alg.mbDevice[i].AlarmCode != mbDevice[i].ErrorCode)
    {
      alarm_hr_alg.mbDevice[i].AlarmCode = mbDevice[i].ErrorCode;
	  ErrorCode2 = mbDevice[i].ErrorCode; 

      if (ErrorCode2 == nicoGebInputVoltageOutsideRange)
        CreateAlarm(&alarm_hr_alg.mbDevice[i].InputVoltageError, FunctionCode + MB_DEVICE_INPUT_VOLTAGE_ERROR_AL, Index, Value, Group, HARD_ALARM);
      else
        ClearAlarm(&alarm_hr_alg.mbDevice[i].InputVoltageError,  FunctionCode + MB_DEVICE_INPUT_VOLTAGE_ERROR_AL, Index);
     
      if (ErrorCode2 == nicoGebBusOverVoltage)
        CreateAlarm(&alarm_hr_alg.mbDevice[i].HighDcLinkVoltage, FunctionCode + MB_DEVICE_UMAX_AL, Index, Value, Group, HARD_ALARM);
      else
        ClearAlarm(&alarm_hr_alg.mbDevice[i].HighDcLinkVoltage,  FunctionCode + MB_DEVICE_UMAX_AL, Index);
  
      if (ErrorCode2 == nicoGebBusUnderVoltage)
        CreateAlarm(&alarm_hr_alg.mbDevice[i].LowDcLinkVoltage, FunctionCode + MB_DEVICE_UMIN_AL, Index, Value, Group, HARD_ALARM);
      else
        ClearAlarm(&alarm_hr_alg.mbDevice[i].LowDcLinkVoltage,  FunctionCode + MB_DEVICE_UMIN_AL, Index);
              
      if (ErrorCode2 == nicoGebInputRelayNotClosed)
        CreateAlarm(&alarm_hr_alg.mbDevice[i].InputRelayNotClosed, FunctionCode + MB_DEVICE_INPUT_RELAY_NOT_CLOSED_AL, Index, Value, Group, HARD_ALARM);
      else
        ClearAlarm(&alarm_hr_alg.mbDevice[i].InputRelayNotClosed,  FunctionCode + MB_DEVICE_INPUT_RELAY_NOT_CLOSED_AL, Index);

      if (ErrorCode2 == nicoGebMissingPhaseU || ErrorCode2 == nicoGebMissingPhaseV || ErrorCode2 == nicoGebMissingPhaseW)
        CreateAlarm(&alarm_hr_alg.mbDevice[i].InputPhaseError, FunctionCode + MB_DEVICE_INPUT_PHASE_ERROR_AL, Index, Value, Group, HARD_ALARM);
      else
        ClearAlarm(&alarm_hr_alg.mbDevice[i].InputPhaseError,  FunctionCode + MB_DEVICE_INPUT_PHASE_ERROR_AL, Index); 

      if (ErrorCode2 == nicoGebHighStartingCurrent)
        CreateAlarm(&alarm_hr_alg.mbDevice[i].HighStartingCurrent, FunctionCode + MB_DEVICE_HIGH_STARTING_CURRENT_AL, Index, Value, Group, HARD_ALARM);
      else
        ClearAlarm(&alarm_hr_alg.mbDevice[i].HighStartingCurrent,  FunctionCode + MB_DEVICE_HIGH_STARTING_CURRENT_AL, Index);

      if (ErrorCode2 == nicoGebOvertemperature)
        CreateAlarm(&alarm_hr_alg.mbDevice[i].ElectronicBoxOverHeat, FunctionCode + MB_DEVICE_ELECTRONIC_BOX_OVER_HEAT_AL, Index, Value, Group, HARD_ALARM);
      else
        ClearAlarm(&alarm_hr_alg.mbDevice[i].ElectronicBoxOverHeat,  FunctionCode + MB_DEVICE_ELECTRONIC_BOX_OVER_HEAT_AL, Index);

      //if (ErrorCode2 == nicoGebLossOfCommunication)
    }

    if (alarm_hr_alg.mbDevice[i].WarningCode || alarm_hr_alg.mbDevice[i].AlarmCode)
      Alarm = 1;
  }
  
  return (Alarm);
}

unsigned char NicotraGebhardtAlarmActive(unsigned char DeviceAddress)
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

unsigned char NicotraGebhardtClearAllAlarms(unsigned char DeviceAddress, int FunctionCode, int Index)
{
int i;

 if (DeviceAddress == 0)
    return (0);

  i = DeviceAddress - 1;
  alarm_hr_alg.mbDevice[i].AlarmCode = 0;
  alarm_hr_alg.mbDevice[i].WarningCode = 0;

  ClearAlarm(&alarm_hr_alg.mbDevice[i].MemoryError,           FunctionCode + MB_DEVICE_MEMORY_ERROR_AL,             Index); 
  ClearAlarm(&alarm_hr_alg.mbDevice[i].ShortCircuit,          FunctionCode + MB_DEVICE_SHORT_CIRCUIT_AL,            Index); 
  ClearAlarm(&alarm_hr_alg.mbDevice[i].LossOfSynchronism,     FunctionCode + MB_DEVICE_LOSS_OF_SYNCHRONISM_AL,      Index); 
        
  ClearAlarm(&alarm_hr_alg.mbDevice[i].InputVoltageError,     FunctionCode + MB_DEVICE_INPUT_VOLTAGE_ERROR_AL,      Index);
  ClearAlarm(&alarm_hr_alg.mbDevice[i].HighDcLinkVoltage,     FunctionCode + MB_DEVICE_UMAX_AL,                     Index);
  ClearAlarm(&alarm_hr_alg.mbDevice[i].LowDcLinkVoltage,      FunctionCode + MB_DEVICE_UMIN_AL,                     Index);
  ClearAlarm(&alarm_hr_alg.mbDevice[i].InputRelayNotClosed,   FunctionCode + MB_DEVICE_INPUT_RELAY_NOT_CLOSED_AL,   Index);
  ClearAlarm(&alarm_hr_alg.mbDevice[i].InputPhaseError,       FunctionCode + MB_DEVICE_INPUT_PHASE_ERROR_AL,        Index); 
  ClearAlarm(&alarm_hr_alg.mbDevice[i].HighStartingCurrent,   FunctionCode + MB_DEVICE_HIGH_STARTING_CURRENT_AL,    Index);
  ClearAlarm(&alarm_hr_alg.mbDevice[i].ElectronicBoxOverHeat, FunctionCode + MB_DEVICE_ELECTRONIC_BOX_OVER_HEAT_AL, Index);

  return (1);
}

static void NicotraGebhardtResetFailures(unsigned char DeviceAddress)
{
  mbWriteSingleRegister(DeviceAddress, rbNicoGebReset, 1, NicotraGebhardtReceiveWriteSingleRegister);

  if (DeviceAddress)
  {
    mbDevice[DeviceAddress - 1].WarningCode = 0;
    mbDevice[DeviceAddress - 1].ErrorCode = 0;
  }
}

//------------------------ Addressing ------------------------------------------
static void NicotraGebhardtReceiveChangeAddress(TmbErrorCode ErrorCode, TmbMessage *Msg)
{
unsigned int  Register;
unsigned int  Value;
unsigned char Succeeded = 0;

  if (NicotraGebhardtHandleException(Msg->Address, ErrorCode) == 0)
  {
    if ((Msg->Address > 0) && (Msg->NrDataBytes == 4))
    {
      Register = mbReadInt(&Msg->Data[0]);
      Value    = mbReadInt(&Msg->Data[2]);
      switch (Register)
      {
        case rbNicoGebModbusAdress:
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


static void NicotraGebhardtChangeAddress(void)
{
  mbWriteSingleRegister(OldDeviceAddress, rbNicoGebModbusAdress, NewDeviceAddress, NicotraGebhardtReceiveChangeAddress);
}

unsigned char NicotraGebhardtRequestChangeAddress(unsigned char DeviceAddress, unsigned char NewAddress, s_board_IO_on_off *IO, unsigned char IO_size, void (*CompletedFunc)(unsigned char Failure))
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
static void NicotraGebhardtReceiveWriteSingleRegister(TmbErrorCode ErrorCode, TmbMessage *Msg)
{
unsigned int Register;
unsigned int Value;
unsigned int DeviceValueRpm;

  if (NicotraGebhardtHandleException(Msg->Address, ErrorCode) == 0)
  {
    if ((Msg->Address > 0) && (Msg->NrDataBytes == 4))
    {
      Register = mbReadInt(&Msg->Data[0]);
      Value    = mbReadInt(&Msg->Data[2]);
      switch (Register)
      {
	    case rbNicoGebReset:
          mbDevice[Msg->Address - 1].Ctrl.ResetDevice = 0;	 
          break;
        case rbNicoGebInputType:
          if (Value == 0)
            mbReadHoldingRegister(Msg->Address, rbNicoGebMinRpm, 2, NicotraGebhardtReceiveMinMaxRpm);		 
          break;
		case rbNicoGebSpeed:
		  DeviceValueRpm = Value ? Calc_Prop(mbDevice[Msg->Address - 1].MinRpm, mbDevice[Msg->Address - 1].MaxRpm, 1, 100, Value) : 0;
		  if (DeviceValueRpm == (long)mbDevice[Msg->Address - 1].TargetValue)
          {
            mbDevice[Msg->Address - 1].Ctrl.SetTargetValue = 0;
            TimerSet(&mbDevice[Msg->Address - 1].TimerTargetValue, TIMER_5MIN);
          }
        break;
      }
    }
  }
}

static void NicotraGebhardtReceiveMinMaxRpm(TmbErrorCode ErrorCode, TmbMessage *Msg)
{													 
  if (NicotraGebhardtHandleException(Msg->Address, ErrorCode) == 0)
  {
    if ((Msg->Address > 0) && (Msg->NrDataBytes == 5))
    {
      mbDevice[Msg->Address - 1].MinRpm = mbReadInt(&Msg->Data[1]);
      mbDevice[Msg->Address - 1].MaxRpm = mbReadInt(&Msg->Data[3]);
      mbDevice[Msg->Address - 1].Ctrl.Init = 0;
    }
  }
}

static void NicotraGebhardtInitVent(unsigned char DeviceAddress)
{
  mbWriteSingleRegister(DeviceAddress, rbNicoGebInputType, 0, NicotraGebhardtReceiveWriteSingleRegister);
}

static void NicotraGebhardtSetTargetSpeed(unsigned char DeviceAddress)
{
unsigned int TargetValuePercentage;
unsigned int TargetValueRpm;

  TargetValuePercentage = mbDevice[DeviceAddress - 1].TargetValue;

  if (TargetValuePercentage == 0)
    TargetValueRpm = 0;
  else
    TargetValueRpm = Calc_Prop(1, 100, mbDevice[DeviceAddress - 1].MinRpm, mbDevice[DeviceAddress - 1].MaxRpm, TargetValuePercentage); 

  mbWriteSingleRegister(DeviceAddress, rbNicoGebSpeed, TargetValueRpm, NicotraGebhardtReceiveWriteSingleRegister);
}
	   
static void NicotraGebhardtReceiveActualData(TmbErrorCode ErrorCode, TmbMessage *Msg)
{
unsigned int ActualValuePercentage;
unsigned int ActualValueRpm;
unsigned int AlarmCode1;
unsigned int AlarmCode2;

  if (NicotraGebhardtHandleException(Msg->Address, ErrorCode) == 0)
  {
    if ((Msg->Address > 0) && (Msg->NrDataBytes == 65))
    { 
	  //bitposition = (index x 2) + 1
	  //rbNicoGebVersion =	0
	  mbDevice[Msg->Address - 1].Version = mbReadInt(&Msg->Data[1]);

	  //rbNicoGebSpeedMeasuredSpeed = 3
      ActualValueRpm = mbReadInt(&Msg->Data[7]);  
      ActualValuePercentage =  Calc_Prop(mbDevice[Msg->Address - 1].MinRpm, mbDevice[Msg->Address - 1].MaxRpm, 1, 100, ActualValueRpm);    
	  mbDevice[Msg->Address - 1].ActualValue = (ActualValueRpm) ? ActualValuePercentage : 0;

	  //rbNicoGebAlarm1 = 10
	  AlarmCode1 = mbReadInt(&Msg->Data[21]);
	  if (AlarmCode1 != 0)
        mbDevice[Msg->Address - 1].WarningCode = AlarmCode1;

      //rbNicoGebMotorCurrent = 12
	  mbDevice[Msg->Address - 1].ReferenceCurrent = mbReadInt(&Msg->Data[25]);

      //rbNicoGebMotorVoltage = 13
	  mbDevice[Msg->Address - 1].ReferenceVoltage = mbReadInt(&Msg->Data[27]) * 1000;

	  //rbNicoGebInternalTemp = 15
      mbDevice[Msg->Address - 1].TempPowerModule = mbReadInt(&Msg->Data[31]) / 10 ;

	  //rbNicoGebAlarm2 = 17
      AlarmCode2 = mbReadInt(&Msg->Data[35]);
	  if (AlarmCode2 != 0)
        mbDevice[Msg->Address - 1].ErrorCode = AlarmCode2;
      	
      //rbNicoGebMeasuredPower = 31
      mbDevice[Msg->Address - 1].EnergyConsumption = mbReadInt(&Msg->Data[63]);
    }
  }
}

static void NicotraGebhardtGetActualData(unsigned char DeviceAddress)
{
  mbReadInputRegister(DeviceAddress, rbNicoGebVersion, 32, NicotraGebhardtReceiveActualData);
}

//==============================================================================
//------------------------ Nicotra Gebhardt EC-ventilatoren - Main -------------
//==============================================================================
//------------------------------------------------------------------------------
unsigned char NicotraGebhardtMain(int DeviceAddress)
{
int index;
unsigned char returnValue = 0;

  if ((DeviceAddress < 1) || (DeviceAddress > 247))
    return 1;

  index = DeviceAddress - 1;
  if (ChangeAddress)
  {
    NicotraGebhardtChangeAddress();
    returnValue = 0;
  }
  else if (mbDevice[index].Ctrl.Init)
  {
    NicotraGebhardtInitVent(index + 1);
    returnValue = 1;
  }
  else if (mbDevice[index].Ctrl.ResetDevice)
  {
    NicotraGebhardtResetFailures(DeviceAddress);
    returnValue = 1;
  }
  else if (mbDevice[index].Ctrl.SetTargetValue || TimerExpired(&mbDevice[index].TimerTargetValue))
  {
    NicotraGebhardtSetTargetSpeed(index + 1);
    returnValue = 1;
  }
  else
  {
    NicotraGebhardtGetActualData(index + 1);
    returnValue = 1;
  }

  return returnValue;
}

//==============================================================================
//------------------------ End of file -----------------------------------------
//==============================================================================