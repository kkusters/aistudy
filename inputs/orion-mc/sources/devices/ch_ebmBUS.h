// CH_EBMBUS.H

#ifndef _CH_EBMBUS_H
#define _CH_EBMBUS_H

#include <time.h>

#include "ct_data.h"
#include "ch_timer.h"

//==============================================================================
//------------------------ ebmBUS - Defines ------------------------------------
//==============================================================================
#define MAX_EBM_BUS  16    // 16 is maximum

#define EBM_BUFFER_SIZE 16

#define PREAMBLE_MASK    ((unsigned char)0x1F)
#define PREAMBLE_SYNC    ((unsigned char)0x01)
#define PREAMBLE_SLAVE   ((unsigned char)0x11)
#define PREAMBLE_MASTER  ((unsigned char)0x04)
#define PREAMBLE_SERVICE ((unsigned char)0x08)

//==============================================================================
//------------------------ ebmBUS - Enum's -------------------------------------
//==============================================================================
typedef enum
{
  cmGetStatus      = (unsigned char)0,
  cmGetActualSpeed = 1,
  cmSetTargetSpeed = 2,
  cmSoftwareReset  = 3,
  cmNoFunction     = 4,
  cmDiagnosis      = 5,
  cmWriteEeprom    = 6,
  cmReadEeprom     = 7
} TebmCommand;

typedef enum
{
  edGroupAddress    = (unsigned char)0x00,
  edFanAddress      = 0x01,
  edOperationModes1 = 0x02,
  edSetTargetValue  = 0x03,
  edNotUsed         = 0x04,
  edPfactor         = 0x05,
  edIfactor         = 0x06,
  edDfactor         = 0x07,
  edMaxRpm1         = 0x08,
  edMaxRpm2         = 0x09,
  edMaxRpm3         = 0x0A,
  edMaxDutyCycle    = 0x0B,
  edMinDutyCycle    = 0x0C,
  edStartDutyCycle  = 0x0D,
  edTargetValue0    = 0x0E,
  edTargetValue1    = 0x0F,
  edOperationModes2 = 0x10,
  edRatingFactor    = 0x11,
  edEepromStatus    = 0x40,
  edIdentification  = 0x4A,
  edHourMeterMSB    = 0x73,
  edHourMeterLSB    = 0x74,
  edRampUp          = 0x95,
  edRampDown        = 0x96,
  edRefVoltageMSB   = 0xC7,
  edRefVoltageLSB   = 0xC8,
  edRefCurrentMSB   = 0xC9,
  edRefCurrentLSB   = 0xCA
} TebmEepromData;

typedef enum
{
  svMotorStatusLowByte  = 0x00,
  svMotorStatusHighByte = 0x01,
  svWarning             = 0x02,
  svDcLinkVoltage       = 0x03,
  svDcLinkCurrent       = 0x04,
  svTempPowerModule     = 0x05,
  svTempMotor           = 0x10,
  svTempElectronics     = 0x20
} TebmStatusVariables;

//==============================================================================
//------------------------ ebmBUS - Typedefs -----------------------------------
//==============================================================================
typedef struct
{
  unsigned Busy          : 1;
  unsigned ShutUp        : 1;
  unsigned ChangeAddress : 1;
  unsigned : 1;
  unsigned : 1;
  unsigned : 1;
  unsigned : 1;
  unsigned : 1;
} TebmBusCtrl;

typedef struct
{
  TebmCommand  Command;
  unsigned char Service;
  unsigned char GroupAddress;
  unsigned char FanAddress;
  unsigned char NrDataBytes;
  unsigned char Data[2];
} TebmMessage;

typedef struct
{
  TebmBusCtrl Ctrl;
  TebmMessage MessageBuffer[EBM_BUFFER_SIZE];
  TTimer      TimeOutTimer;
  TTimer      ShutUpTimer;
  unsigned char NrTimeOuts;
  unsigned char PutIndex;
  unsigned char GetIndex;
  unsigned long Value; // used for temporary storage of registers
  unsigned int  Address;
  s_board_IO_on_off IO;
} TebmBus;

//==============================================================================
//------------------------ ebmBUS - Globals ------------------------------------
//==============================================================================

//==============================================================================
//------------------------ ebmBUS - Control ------------------------------------
//==============================================================================
void ebmBusInit(void);

unsigned char ebmBusAddConnection(int DeviceAddress, s_board_IO_on_off *IO, unsigned char IO_size);
unsigned char ebmBusRequestChangeAddress(int OldAddress, int NewAddress, s_board_IO_on_off *IO, unsigned char IO_size, void (*CompletedFunc)(unsigned char Failure));

void ebmNodeSetMaximumRpm(int DeviceAddress, unsigned int Rpm);

//==============================================================================
//------------------------ ebmBUS - Alarm --------------------------------------
//==============================================================================
//------------------------------------------------------------------------------
unsigned char ebmNodeCreateAlarm(int DeviceAddress, int FunctionCode, int Index, int Value, int Group);
unsigned char ebmNodeAlarmActive(int DeviceAddress);
unsigned char ebmNodeClearAllAlarms(int DeviceAddress, int FunctionCode, int Index);

//==============================================================================
//------------------------ ebmBUS - Main ---------------------------------------
//==============================================================================
//------------------------------------------------------------------------------
void ebmBusMain(void);

#endif
