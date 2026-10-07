// CH_CLIMAFAN.H

#ifndef _CH_CLIMAFAN_H
#define _CH_CLIMAFAN_H

#include <time.h>

#include "ct_data.h"
#include "ch_timer.h"

//==============================================================================
//------------------------ Climafan EC-ventilatoren - modBus -------------------
//==============================================================================

typedef union
{
  unsigned int code;
  struct
  {
    unsigned dcbusOverCurrentProt      : 1;
	unsigned dcbusOverVoltageProt      : 1;
	unsigned dcbusUnderVoltageProt     : 1;
	unsigned eeOverTemperatureProt     : 1;
	unsigned fanLock                   : 1;
	unsigned acPhaseLose               : 1;
	unsigned fanReverseRun             : 1;
	unsigned hallSignal                : 1;
	unsigned eepromFault               : 1;
	unsigned dcbusPeakOverCurrent      : 1;
	unsigned acbusOverVoltageProt      : 1;
	unsigned acbusUnderVoltageProt     : 1;
	unsigned bit_12                    : 1;
	unsigned bit_13                    : 1;
	unsigned bit_14                    : 1;
	unsigned bit_15                    : 1;
  } actual;
} TClimafanErrorCodes;

// Read input registers Function Code 04
typedef enum
{
	cfFanControlMode      =  0,
	cfFanStatus           =  1,
	cfFanActualSpeed      =  2,
	cfFanActualDuty       =  3,
	cfFanDcVoltage        =  4,
	cfFanDcCurrent        =  5,
	cfFanPower            =  6,
	cfFanTemperature      =  7,
	cfFanMonitor          =  8,
	cfAlarmLog            = 0xD00
} TCfanInputRegister;

// Read holding registers Function Code 03
// Write holding registers Function Code 06
typedef enum
{
	cfFanID                 = 0,
	cfFanReference			= 0x0009,
	cfFanModel              = 0x0013,
	cfFanOperatingMode      = 0x0100,
	cfFanDutySetting        = 0x0101,
	cfFanSpeedSetting       = 0x0102,
	cfFanStop               = 0x0103,
	cfFanReset              = 0x0104,
	cfFanMaxSpeed           = 0x0119,
	cfRampUpTime            = 0x011F,
	cfRampDownTime          = 0x0120,
	cfFanMaxPower           = 0x0135,
	cfFanMaxCurrent         = 0x013B,
	cfFanUartBaudrate       = 0x0201,
	cfAlarmLogcount         = 0x0202,
	cfOperatingTimeHours    = 0x0204
} TCfanHoldingRegister;


//==============================================================================
//------------------------ Climafan EC-ventilatoren - Control ------------------
//==============================================================================
unsigned char ClimafanRequestChangeAddress(unsigned char DeviceAddress, unsigned char NewAddress, s_board_IO_on_off *IO, unsigned char IO_size, void (*CompletedFunc)(unsigned char Failure));

unsigned char ClimafanCreateAlarm(unsigned char DeviceAddress, int FunctionCode, int Index, int Value, int Group);
unsigned char ClimafanAlarmActive(unsigned char DeviceAddress);
unsigned char ClimafanClearAllAlarms(unsigned char DeviceAddress, int FunctionCode, int Index);

//==============================================================================
//------------------------ Climafan EC-ventilatoren - Main routines -----------
//==============================================================================
//------------------------------------------------------------------------------
unsigned char ClimafanMain(int DeviceAddress);

#endif
