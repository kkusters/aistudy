// CH_ROSENBERG.H

#ifndef _CH_ROSENBERG_H
#define _CH_ROSENBERG_H

#include <time.h>

#include "ct_data.h"
#include "ch_timer.h"

//==============================================================================
//------------------------ Rosenberg EC-ventilatoren - modBus ------------------
//==============================================================================
typedef enum
{
  rbMaximumRpm           = 14,
  rbRevolutionSense      = 17,
  rbOperatingMode        = 31,
  rbStatus               = 32,
  rbVersionNumber        = 33,
  rbEnableAutostart      = 34,
  rbControlMode          = 38,
  rbSetpointMode         = 39,
  rbControlRegister      = 41,
  rbStatusRegister       = 42,
  rbSetpointRegister     = 43,
  rbMinimumRpm           = 58,
  rbAddress              = 78,
  rbCommunicationMode    = 79,
  rbRevolutionsPerMinute = 82,
  rbFailureRegister      = 85,
  rbInsideTemperature    = 89
} TRosenbergRegisters;

typedef union
{
  unsigned int code;
  struct
  {
    unsigned failurePowerSection       : 1;
	unsigned phaseFailure              : 1;
	unsigned uMax                      : 1;
	unsigned uMin                      : 1;
	unsigned bit_4                     : 1;
    unsigned electronicOvertemperature : 1;
	unsigned bit_6                     : 1;
	unsigned overcurrent               : 1;
	unsigned motorOvertemperature      : 1;
	unsigned bit_9                     : 1;
	unsigned overspeed                 : 1;
	unsigned lockedRotor               : 1;
	unsigned bit_12                    : 1;
	unsigned bit_13                    : 1;
	unsigned bit_14                    : 1;
	unsigned bit_15                    : 1;
  } actual;
} TRosenbergErrorCodes;

//==============================================================================
//------------------------ Rosenberg EC-ventilatoren - Control -----------------
//==============================================================================
unsigned char RosenbergRequestChangeAddress(unsigned char DeviceAddress, unsigned char NewAddress, s_board_IO_on_off *IO, unsigned char IO_size, void (*CompletedFunc)(unsigned char Failure));

unsigned char RosenbergCreateAlarm(unsigned char DeviceAddress, int FunctionCode, int Index, int Value, int Group);
unsigned char RosenbergAlarmActive(unsigned char DeviceAddress);
unsigned char RosenbergClearAllAlarms(unsigned char DeviceAddress, int FunctionCode, int Index);

//==============================================================================
//------------------------ Rosenberg EC-ventilatoren - Main routines -----------
//==============================================================================
//------------------------------------------------------------------------------
unsigned char RosenbergMain(int DeviceAddress);

#endif
