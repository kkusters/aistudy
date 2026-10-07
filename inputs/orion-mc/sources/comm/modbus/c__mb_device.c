// C__MB_DEVICE.C  

#include "ch_define.h"

#include "ch_alarm.h"
#include "ch_alg.h"
#include "ch_IO.h"
#include "ch_belimo_lm24a_mod.h"
#include "ch_ebmBus.h"
#include "ch_ebm_modbus.h"
#include "ch_ecblue_modbus.h"
#include "ch_ecblue_premium.h"
#include "ch_dpt_mod.h"
#include "ch_rosenberg.h"
#include "ch_rosenberg_gen_3.h"
#include "ch_climafan.h"
#include "ch_nicotra_gebhardt.h"
#include "ch_modbus.h"
#include "ch_tijd.h"
#include "ch_mb_device.h"

//==============================================================================
//------------------------ mbDevice - Globals ----------------------------------
//==============================================================================
//------------------------------------------------------------------------------
TmbDevice mbDevice[MAX_MB_DEVICE];
unsigned char mbDeviceWatchdogEnabled = 0;

static unsigned char ebmBusUsed;
static unsigned char ModbusUsed;

//==============================================================================
//------------------------ mbDevice - Init -------------------------------------
//==============================================================================
//------------------------------------------------------------------------------
void mbDeviceInit(void)
{
unsigned char *ptr;
int i;

  ptr = (unsigned char *)&mbDevice;
  for (i = 0; i < sizeof(mbDevice); i++)
    ptr[i] = 0;
  for (i = 0; i < MAX_MB_DEVICE; i++)
  {
    mbDevice[i].PowerFactor = 100;
	mbDevice[i].RampUp   = 12;
	mbDevice[i].RampDown = 12;
	mbDevice[i].WatchdogTime = 25; // maximaal 25 seconde bij embModbus!!!
  }

  ebmBusUsed = 0;
  ModbusUsed = 0;
  mbDeviceWatchdogEnabled = 0;

  ebmBusInit();
  mbInit();
}

static unsigned char isAddressValid(unsigned char DeviceAddress)
{
  return ((DeviceAddress > 0) && (DeviceAddress <= MAX_MB_DEVICE));
}

void mbDeviceSetWatchdogMode(unsigned char DeviceAddress, unsigned char WatchdogEnabled)
{
  if (!isAddressValid(DeviceAddress))
    return;
  mbDevice[DeviceAddress - 1].WatchdogMode = WatchdogEnabled;
  if (WatchdogEnabled)
    mbDeviceWatchdogEnabled = 1;
}

void mbDeviceSetWatchdogPosition(unsigned char DeviceAddress, unsigned char WatchdogPosition)
{
  if (!isAddressValid(DeviceAddress))
    return;
  mbDevice[DeviceAddress - 1].WatchdogPosition = WatchdogPosition;
}

void mbDeviceSetOperationMode(unsigned char DeviceAddress, TmbOperationMode OperationMode)
{
  if ((DeviceAddress == 0) || (DeviceAddress > MAX_MB_DEVICE))
    return;

  switch (OperationMode)
  {
    case omClosedLoop:
	  mbDevice[DeviceAddress - 1].ClosedLoopOpenLoop = 0;
	  break;
	case omOpenLoop:
	  mbDevice[DeviceAddress - 1].ClosedLoopOpenLoop = 1;
	  break;
  }
}

void mbDeviceSetRampUp(unsigned char DeviceAddress, unsigned char RampUp)
{
  if ((DeviceAddress == 0) || (DeviceAddress > MAX_MB_DEVICE))
    return;

  mbDevice[DeviceAddress - 1].RampUp = RampUp;
}

void mbDeviceSetRampDown(unsigned char DeviceAddress, unsigned char RampDown)
{
  if ((DeviceAddress == 0) || (DeviceAddress > MAX_MB_DEVICE))
    return;

  mbDevice[DeviceAddress - 1].RampDown = RampDown;
}

void mbDeviceSetPowerFactor(unsigned char DeviceAddress, unsigned char PowerFactor)
{
  if ((DeviceAddress == 0) || (DeviceAddress > MAX_MB_DEVICE))
    return;

  mbDevice[DeviceAddress - 1].PowerFactor = PowerFactor;
}

void mbDeviceSetSensorType(unsigned char DeviceAddress, unsigned char SensorType)
{
  if ((DeviceAddress == 0) || (DeviceAddress > MAX_MB_DEVICE))
    return;

  mbDevice[DeviceAddress - 1].SensorType = SensorType;
}

void mbResetDevice(unsigned char DeviceAddress)
{
  if ((DeviceAddress == 0) || (DeviceAddress > MAX_MB_DEVICE))
    return;

  mbDevice[DeviceAddress - 1].Ctrl.ResetDevice = 1;
}

//==============================================================================
//------------------------ mbDevice - Commands ---------------------------------
//==============================================================================
//------------------------------------------------------------------------------
unsigned char mbDeviceConnected(unsigned char DeviceAddress)
{
  if ((DeviceAddress == 0) || (DeviceAddress > MAX_MB_DEVICE))
    return (0);

  return (mbDevice[DeviceAddress - 1].Ctrl.Connected);
}


unsigned char mbDeviceConnect(unsigned char DeviceAddress, TmbDeviceType Type, s_board_IO_on_off *IO, unsigned char IO_size)
{
unsigned char Connected = 0;

  if ((DeviceAddress == 0) || (DeviceAddress > MAX_MB_DEVICE))
    return (0);

  switch (Type)
  {
    case dtEbmBus         : Connected = ebmBusAddConnection(DeviceAddress, IO, IO_size); break;
    case dtEbmModbus      : Connected = mbAddConnection    (DeviceAddress, IO, IO_size); break;
    case dtECblueModbus   : Connected = mbAddConnection    (DeviceAddress, IO, IO_size); break;
    case dtECbluePremium  : Connected = mbAddConnection    (DeviceAddress, IO, IO_size); break;
    case dtBelimoModbus   : Connected = mbAddConnection    (DeviceAddress, IO, IO_size); break;
    case dtDptMod         : Connected = mbAddConnection    (DeviceAddress, IO, IO_size); break;
    case dtRosenberg      : Connected = mbAddConnection    (DeviceAddress, IO, IO_size); break;
    case dtRosenbergGen3  : Connected = mbAddConnection    (DeviceAddress, IO, IO_size); break;
    case dtClimafan       : Connected = mbAddConnection    (DeviceAddress, IO, IO_size); break;
    case dtNicotraGebhardt: Connected = mbAddConnection    (DeviceAddress, IO, IO_size); break;
  }
  if (Connected)
  {
    mbDevice[DeviceAddress - 1].Type                = Type;
    mbDevice[DeviceAddress - 1].Ctrl.Connected      = 1;
    mbDevice[DeviceAddress - 1].Ctrl.Init           = 1;
    mbDevice[DeviceAddress - 1].Ctrl.GetManagement  = 1;
    mbDevice[DeviceAddress - 1].Ctrl.SetTargetValue = 1;
    mbDevice[DeviceAddress - 1].Ctrl.GetActualValue = 1;
    switch (Type)
    {
      case dtEbmBus         : ebmBusUsed = 1; mbDevice[DeviceAddress - 1].ErrorCode = 0;          break;
      case dtEbmModbus      : ModbusUsed = 1; mbDevice[DeviceAddress - 1].ErrorCode = 0;          break;
      case dtECblueModbus   : ModbusUsed = 1; mbDevice[DeviceAddress - 1].ErrorCode = 0;          break;
      case dtECbluePremium  : ModbusUsed = 1; mbDevice[DeviceAddress - 1].ErrorCode = ecpNoError; break;
      case dtBelimoModbus   : ModbusUsed = 1; mbDevice[DeviceAddress - 1].ErrorCode = 0;          break;
      case dtDptMod         : ModbusUsed = 1; mbDevice[DeviceAddress - 1].ErrorCode = 0;          break;
      case dtRosenberg      : ModbusUsed = 1; mbDevice[DeviceAddress - 1].ErrorCode = 0;          break;
      case dtRosenbergGen3  : ModbusUsed = 1; mbDevice[DeviceAddress - 1].ErrorCode = 0;          break;
      case dtClimafan       : ModbusUsed = 1; mbDevice[DeviceAddress - 1].ErrorCode = 0;          break;
	  case dtNicotraGebhardt: ModbusUsed = 1; mbDevice[DeviceAddress - 1].ErrorCode = 0;          break;
    }
  }

  return (mbDevice[DeviceAddress - 1].Ctrl.Connected);
}

unsigned char mbDeviceChangeAddress(TmbDeviceType DeviceType, unsigned char DeviceAddress, unsigned char NewAddress, s_board_IO_on_off *IO, unsigned char IO_size, void (*CompletedFunc)(unsigned char Failure))
{
  if ((DeviceAddress == 0) || (DeviceAddress > MAX_MB_DEVICE) || (NewAddress == 0) || (NewAddress > MAX_MB_DEVICE)) // Wrong address
    return (0);

  switch (DeviceType)
  {
    case dtEbmBus         : mbDevice[DeviceAddress - 1].Path = 0; return (ebmBusRequestChangeAddress         (DeviceAddress, NewAddress, IO, IO_size, CompletedFunc));
    case dtEbmModbus      : mbResetPath(DeviceAddress);           return (ebmModbusRequestChangeAddress      (DeviceAddress, NewAddress, IO, IO_size, CompletedFunc));
    case dtECblueModbus   : mbResetPath(DeviceAddress);           return (ECblueModbusRequestChangeAddress   (DeviceAddress, NewAddress, IO, IO_size, CompletedFunc));
    case dtECbluePremium  : mbResetPath(DeviceAddress);           return (ECbluePremiumRequestChangeAddress  (DeviceAddress, NewAddress, IO, IO_size, CompletedFunc));
    case dtBelimoModbus   : break;
    case dtDptMod         : break;
    case dtRosenberg      : mbResetPath(DeviceAddress);           return (RosenbergRequestChangeAddress      (DeviceAddress, NewAddress, IO, IO_size, CompletedFunc)); 
    case dtRosenbergGen3  : mbResetPath(DeviceAddress);           return (RosenbergGen3RequestChangeAddress  (DeviceAddress, NewAddress, IO, IO_size, CompletedFunc));
    case dtClimafan       : mbResetPath(DeviceAddress);           return (ClimafanRequestChangeAddress       (DeviceAddress, NewAddress, IO, IO_size, CompletedFunc));
	case dtNicotraGebhardt: mbResetPath(DeviceAddress);           return (NicotraGebhardtRequestChangeAddress(DeviceAddress, NewAddress, IO, IO_size, CompletedFunc));
  }
  return (0);
}

unsigned char mbDeviceDataValid(unsigned char DeviceAddress)
{
  if ((DeviceAddress == 0) || (DeviceAddress > MAX_MB_DEVICE))
    return (0);

  return (mbDevice[DeviceAddress - 1].Ctrl.DataValid);
}

unsigned char mbDeviceSetTargetValue(unsigned char DeviceAddress, int Value)
{
  if ((DeviceAddress == 0) || (DeviceAddress > MAX_MB_DEVICE))
    return (0);

  if (mbDevice[DeviceAddress - 1].TargetValue != Value)
  {
    mbDevice[DeviceAddress - 1].TargetValue = Value;
    mbDevice[DeviceAddress - 1].Ctrl.SetTargetValue = 1;
    return (1);
  }
  return (0);
}

int mbDeviceGetActualValue(unsigned char DeviceAddress)
{
  if ((DeviceAddress == 0) || (DeviceAddress > MAX_MB_DEVICE))
    return (0);

  return (mbDevice[DeviceAddress - 1].ActualValue);
}

unsigned char mbDeviceLostCommunication(unsigned char DeviceAddress)
{
  if ((DeviceAddress == 0) || (DeviceAddress > MAX_MB_DEVICE))
    return (0);

  return (mbDevice[DeviceAddress - 1].Ctrl.LostCommunication);
}

unsigned int mbDeviceGetErrorCode(unsigned char DeviceAddress)
{
  if ((DeviceAddress == 0) || (DeviceAddress > MAX_MB_DEVICE))
    return (0);

  return (mbDevice[DeviceAddress - 1].ErrorCode);
}

unsigned int mbDeviceGetMaximumRpm(unsigned char DeviceAddress)
{
  if ((DeviceAddress == 0) || (DeviceAddress > MAX_MB_DEVICE))
    return (0);

  return (mbDevice[DeviceAddress - 1].MaxRpm);
}

unsigned int mbDeviceGetVersion(unsigned char DeviceAddress)
{
  if ((DeviceAddress == 0) || (DeviceAddress > MAX_MB_DEVICE))
    return (0);

  return (mbDevice[DeviceAddress - 1].Version);
}

unsigned long mbDeviceGetRunningHours(unsigned char DeviceAddress)
{
  if ((DeviceAddress == 0) || (DeviceAddress > MAX_MB_DEVICE))
    return (0);

  return (mbDevice[DeviceAddress - 1].RunningHours);
}

unsigned int mbDeviceGetEnergyConsumption(unsigned char DeviceAddress)
{
unsigned int Power;

  if ((DeviceAddress == 0) || (DeviceAddress > MAX_MB_DEVICE))
    return (0);

  Power = ((long)mbDevice[DeviceAddress - 1].EnergyConsumption * mbDevice[DeviceAddress - 1].PowerFactor) / 100;
  return (Power);
}

unsigned char mbDeviceGetTempPowerModule(unsigned char DeviceAddress)
{
  if ((DeviceAddress == 0) || (DeviceAddress > MAX_MB_DEVICE))
    return (0);

  return (mbDevice[DeviceAddress - 1].TempPowerModule);
}

unsigned char mbDeviceGetTempMotor(unsigned char DeviceAddress)
{
  if ((DeviceAddress == 0) || (DeviceAddress > MAX_MB_DEVICE))
    return (0);

  return (mbDevice[DeviceAddress - 1].TempMotor);
}

unsigned char mbDeviceGetTempElectronics(unsigned char DeviceAddress)
{
  if ((DeviceAddress == 0) || (DeviceAddress > MAX_MB_DEVICE))
    return (0);

  return (mbDevice[DeviceAddress - 1].TempElectronics);
}

unsigned char mbDeviceGetSensorValue(unsigned char DeviceAddress)
{
  if ((DeviceAddress == 0) || (DeviceAddress > MAX_MB_DEVICE))
    return (0);

  return (mbDevice[DeviceAddress - 1].SensorValue);
}

unsigned int mbDeviceGetAlarmCode(unsigned char DeviceAddress)
{
  if ((DeviceAddress == 0) || (DeviceAddress > MAX_MB_DEVICE) || (mbDevice[DeviceAddress -1].Type == dtRosenbergGen3))
    return (0);

  return (alarm_hr_alg.mbDevice[DeviceAddress - 1].AlarmCode);
}

unsigned char mbDeviceCreateAlarm(int DeviceAddress, int FunctionCode, int Index, int Value, int Group)
{
  if ((DeviceAddress == 0) || (DeviceAddress > MAX_MB_DEVICE) || (mbDevice[DeviceAddress - 1].Ctrl.Connected == 0) || (mbDevice[DeviceAddress - 1].Ctrl.ResetDevice == 1))
    return (0);

  switch (mbDevice[DeviceAddress - 1].Type)
  {
    case dtEbmBus         : return (ebmNodeCreateAlarm         (DeviceAddress, FunctionCode, Index, Value, Group));
    case dtEbmModbus      : return (ebmModbusCreateAlarm       (DeviceAddress, FunctionCode, Index, Value, Group));
    case dtECblueModbus   : return (ECblueModbusCreateAlarm    (DeviceAddress, FunctionCode, Index, Value, Group));
    case dtECbluePremium  : return (ECbluePremiumCreateAlarm   (DeviceAddress, FunctionCode, Index, Value, Group));
    case dtBelimoModbus   : return (BelimoModbusCreateAlarm    (DeviceAddress, FunctionCode, Index, Value, Group));
    case dtDptMod         : return (DptModCreateAlarm          (DeviceAddress, FunctionCode, Index, Value, Group));
    case dtRosenberg      : return (RosenbergCreateAlarm       (DeviceAddress, FunctionCode, Index, Value, Group));
    case dtRosenbergGen3  : return (RosenbergGen3CreateAlarm   (DeviceAddress, FunctionCode, Index, Value, Group));
    case dtClimafan       : return (ClimafanCreateAlarm        (DeviceAddress, FunctionCode, Index, Value, Group));
	case dtNicotraGebhardt: return (NicotraGebhardtCreateAlarm (DeviceAddress, FunctionCode, Index, Value, Group));
  }
  return (0);
}

unsigned char mbDeviceAlarmActive(int DeviceAddress)
{
  if ((DeviceAddress == 0) || (DeviceAddress > MAX_MB_DEVICE) || (mbDevice[DeviceAddress - 1].Ctrl.Connected == 0))
    return (0);

  switch (mbDevice[DeviceAddress - 1].Type)
  {
    case dtEbmBus         : return (ebmNodeAlarmActive         (DeviceAddress));
    case dtEbmModbus      : return (ebmModbusAlarmActive       (DeviceAddress));
    case dtECblueModbus   : return (ECblueModbusAlarmActive    (DeviceAddress));
    case dtECbluePremium  : return (ECbluePremiumAlarmActive   (DeviceAddress));
    case dtBelimoModbus   : return (BelimoModbusAlarmActive    (DeviceAddress));
    case dtDptMod         : return (DptModAlarmActive          (DeviceAddress));
    case dtRosenberg      : return (RosenbergAlarmActive       (DeviceAddress));
    case dtRosenbergGen3  : return (RosenbergGen3AlarmActive   (DeviceAddress));
    case dtClimafan       : return (ClimafanAlarmActive        (DeviceAddress));
	case dtNicotraGebhardt: return (NicotraGebhardtAlarmActive (DeviceAddress));
  }
  return (0);
}

unsigned char mbDeviceClearAllAlarms(int DeviceAddress, int FunctionCode, int Index)
{
  if ((DeviceAddress == 0) || (DeviceAddress > MAX_MB_DEVICE) || (mbDevice[DeviceAddress - 1].Ctrl.Connected == 0))
    return (0);

  switch (mbDevice[DeviceAddress - 1].Type)
  {
    case dtEbmBus         : return (ebmNodeClearAllAlarms        (DeviceAddress, FunctionCode, Index));
    case dtEbmModbus      : return (ebmModbusClearAllAlarms      (DeviceAddress, FunctionCode, Index));
    case dtECblueModbus   : return (ECblueModbusClearAllAlarms   (DeviceAddress, FunctionCode, Index));
    case dtECbluePremium  : return (ECbluePremiumClearAllAlarms  (DeviceAddress, FunctionCode, Index));
    case dtBelimoModbus   : return (BelimoModbusClearAllAlarms   (DeviceAddress, FunctionCode, Index));
    case dtDptMod         : return (DptModClearAllAlarms         (DeviceAddress, FunctionCode, Index));
    case dtRosenberg      : return (RosenbergClearAllAlarms      (DeviceAddress, FunctionCode, Index));
    case dtRosenbergGen3  : return (RosenbergGen3ClearAllAlarms  (DeviceAddress, FunctionCode, Index));
    case dtClimafan       : return (ClimafanClearAllAlarms       (DeviceAddress, FunctionCode, Index));
	case dtNicotraGebhardt: return (NicotraGebhardtClearAllAlarms(DeviceAddress, FunctionCode, Index));
  }
  return (0);
}

void mbDeviceSetMaximumRpm(int DeviceAddress, unsigned int Rpm)
{
  if ((DeviceAddress == 0) || (DeviceAddress > MAX_MB_DEVICE) || (mbDevice[DeviceAddress - 1].Ctrl.Connected == 0))
    return;

  switch (mbDevice[DeviceAddress - 1].Type)
  {
    case dtEbmBus         : ebmNodeSetMaximumRpm        (DeviceAddress, Rpm); break;
    case dtEbmModbus      : ebmModbusRequestChangeMaxRpm(DeviceAddress, Rpm); break;
    case dtECblueModbus   : break;
    case dtECbluePremium  : break;
	case dtBelimoModbus   : break;
	case dtDptMod         : break;
	case dtRosenberg      : break;
	case dtRosenbergGen3  : break;
	case dtClimafan       : break;
	case dtNicotraGebhardt: break;
  }
}

//==============================================================================
//------------------------ mbDevice - Main routines ----------------------------
//==============================================================================
//------------------------------------------------------------------------------
void mbDeviceMain(void)
{
static int index = 0;
unsigned char gotoNext = 1;

  if (ebmBusUsed)
    ebmBusMain();

  if (ModbusUsed)
  {
    if (mbDevice[index].Ctrl.Connected)
    {
	  if (!mbBusy())
	  {
        switch (mbDevice[index].Type)
        {
          case dtEbmModbus      : gotoNext = ebmModbusMain      (index + 1); break;
          case dtECblueModbus   : gotoNext = ECblueModbusMain   (index + 1); break;
          case dtECbluePremium  : gotoNext = ECbluePremiumMain  (index + 1); break;
          case dtBelimoModbus   :            BelimoModbusMain   (index + 1); break;
          case dtDptMod         :            DptModMain         (index + 1); break;
          case dtRosenberg      : gotoNext = RosenbergMain      (index + 1); break;
          case dtRosenbergGen3  : gotoNext = RosenbergGen3Main  (index + 1); break;
          case dtClimafan       : gotoNext = ClimafanMain       (index + 1); break;
          case dtNicotraGebhardt: gotoNext = NicotraGebhardtMain(index + 1); break;
        }
		if (gotoNext)
		  index++;
	  }
    }
    else
    {
      index++;
    }
    if (index >= MAX_MB_DEVICE)
      index = 0;
    mbMain();
  }
}


//==============================================================================
//------------------------ End of file -----------------------------------------
//==============================================================================
