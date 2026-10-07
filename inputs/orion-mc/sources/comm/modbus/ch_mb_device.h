// CH_MB_DEVICE.H

#ifndef _CH_MB_DEVICE_H
#define _CH_MB_DEVICE_H

#include <time.h>

#include "ct_data.h"
#include "ch_timer.h"

//==============================================================================
//------------------------ mbDevice - Defines ----------------------------------
//==============================================================================

//==============================================================================
//------------------------ mbDevice - Typedefs ---------------------------------
//==============================================================================
typedef enum
{
  dtEbmBus = 0,
  dtECbluePremium,
  dtEbmModbus,
  dtECblueModbus,
  dtBelimoModbus,
  dtDptMod,
  dtRosenberg,
  dtClimafan,
  dtRosenbergGen3,
  dtNicotraGebhardt
} TmbDeviceType;

typedef enum
{
  omClosedLoop = 0,
  omOpenLoop
} TmbOperationMode;

typedef struct
{
  unsigned Init              : 1;
  unsigned GetManagement     : 1;
  unsigned SetTargetValue    : 1;
  unsigned GetActualValue    : 1;
  unsigned GetTemperature    : 1;
  unsigned Exception         : 1;
  unsigned Connected         : 1;
  unsigned LostCommunication : 1;
  unsigned DataValid         : 1;
  unsigned ResetDevice       : 1;
} TmbDeviceCtrl;

typedef struct
{
  TmbDeviceType Type;
  TmbDeviceCtrl Ctrl;
  unsigned int  Path;
  unsigned int  Version;
  unsigned int  TargetValue;
  unsigned int  ActualValue;
  unsigned char ClosedLoopOpenLoop; // 0 = ClosedLoop; 1 = OpenLoop
  unsigned char TempPowerModule;
  unsigned char TempMotor;
  unsigned char TempElectronics;
  unsigned char RampUp;
  unsigned char RampDown;
  unsigned char PowerFactor;
  unsigned char Min;
  unsigned char Max;
  unsigned char SensorType;
  unsigned char SensorValue;
  unsigned int  MinRpm;
  unsigned int  MaxRpm;
  unsigned long RunningHours;      // seconds
  unsigned int  ReferenceVoltage;  // x 20mV
  unsigned int  ReferenceCurrent;  // x 2mA
  unsigned int  EnergyConsumption; // W
  unsigned int  ErrorCode;
  unsigned int  WarningCode;
  unsigned char WatchdogMode;
  unsigned char WatchdogTime;
  unsigned char WatchdogPosition;
  TTimer TimerTargetValue;
} TmbDevice;

//==============================================================================
//------------------------ mbDevice - Globals ----------------------------------
//==============================================================================
//------------------------------------------------------------------------------
extern TmbDevice mbDevice[MAX_MB_DEVICE];
extern unsigned char mbDeviceWatchdogEnabled;

//==============================================================================
//------------------------ mbDevice - Init -------------------------------------
//==============================================================================
//------------------------------------------------------------------------------
void mbDeviceInit(void);
void mbDeviceSetWatchdogMode(unsigned char DeviceAddress, unsigned char WatchdogEnabled);
void mbDeviceSetWatchdogPosition(unsigned char DeviceAddress, unsigned char WatchdogPosition);
void mbDeviceSetOperationMode(unsigned char DeviceAddress, TmbOperationMode OperationMode);
void mbDeviceSetRampUp(unsigned char DeviceAddress, unsigned char RampUp);
void mbDeviceSetRampDown(unsigned char DeviceAddress, unsigned char RampDown);
void mbDeviceSetPowerFactor(unsigned char DeviceAddress, unsigned char PowerFactor);
void mbDeviceSetSensorType(unsigned char DeviceAddress, unsigned char SensorType);
void mbResetDevice(unsigned char DeviceAddress);

//==============================================================================
//------------------------ mbDevice - Control ----------------------------------
//==============================================================================
unsigned char mbDeviceConnected(unsigned char DeviceAddress);
unsigned char mbDeviceConnect(unsigned char DeviceAddress, TmbDeviceType Type, s_board_IO_on_off *IO, unsigned char IO_size);
unsigned char mbDeviceChangeAddress(TmbDeviceType DeviceType, unsigned char DeviceAddress, unsigned char NewAddress, s_board_IO_on_off *IO, unsigned char IO_size, void (*CompletedFunc)(unsigned char Failure));

unsigned char mbDeviceDataValid(unsigned char DeviceAddress);
unsigned char mbDeviceSetTargetValue(unsigned char DeviceAddress, int Value);
int mbDeviceGetActualValue(unsigned char DeviceAddress);
unsigned char mbDeviceLostCommunication(unsigned char DeviceAddress);
unsigned int  mbDeviceGetErrorCode(unsigned char DeviceAddress);
unsigned int  mbDeviceGetMaximumRpm(unsigned char DeviceAddress);
unsigned int  mbDeviceGetVersion(unsigned char DeviceAddress);
unsigned long mbDeviceGetRunningHours(unsigned char DeviceAddress);
unsigned int  mbDeviceGetEnergyConsumption(unsigned char DeviceAddress);
unsigned char mbDeviceGetTempPowerModule(unsigned char DeviceAddress);
unsigned char mbDeviceGetTempMotor(unsigned char DeviceAddress);
unsigned char mbDeviceGetTempElectronics(unsigned char DeviceAddress);
unsigned char mbDeviceGetSensorValue(unsigned char DeviceAddress);

unsigned char mbDeviceCreateAlarm(int DeviceAddress, int FunctionCode, int Index, int Value, int Group);
unsigned char mbDeviceAlarmActive(int DeviceAddress);
unsigned char mbDeviceClearAllAlarms(int DeviceAddress, int FunctionCode, int Index);

void mbDeviceSetMaximumRpm(int DeviceAddress, unsigned int Rpm);

void mbDeviceMain(void);

#endif
