// CH_ECBLUE_MODBUS.H

#ifndef _CH_ECBLUE_MODBUS_H
#define _CH_ECBLUE_MODBUS_H

#include <time.h>

#include "ct_data.h"

//==============================================================================
//------------------------ ZiehlAbegg ECblue-Modbus - modBus -------------------
//==============================================================================
#define ecbNoError            0x0000
#define ecbIgbtFault          0x0001
#define ecbEarthToGroundFault 0x0002
#define ecbUzkHi              0x0004
#define ecbUzkLo              0x0008
#define ecbUinHi              0x0010
#define ecbUinLo              0x0020
#define ecbLineFault          0x0040
#define ecbTbReserved         0x0080
#define ecbpHallSensor        0x0100
#define ecbMotorBlocked       0x0200
#define ecbPeakCurrunt        0x0400

#define PIN_COMMUNICATIONS_PARAMETERS 3698

typedef enum
{
  ecbFirmware           =  0,
  ecbOperationCondition = 10,
  ecbErrorStatus        = 12,
  ecbSpeed              = 14,
  ecbMotorCurrent       = 15,
  ecbDcVoltage          = 20,
  ecbLineVoltage        = 21,
  ecbTempIGBT           = 22,
  ecbTempInside         = 23,
  ecbTempMCU            = 24,
  ecbTempMotor          = 25
} TECblueModbusInputRegister;

typedef enum
{
  ecbPinInput             =  0,
  ecbSpeedControl         =  2,
  ecbComParameter         =  3,
  ecbControlMode          =  4,
  ecbSetIntern1           =  5,
  ecbSetIntern2           =  6,
  ecbControllerSetupFlags = 16,
  ecbWatchdog             = 17,
  ecbMaximumSpeed         = 20,
  ecbRampTiming           = 25
} TECblueHoldingRegister;

//==============================================================================
//------------------------ ZiehlAbegg ECblue-Modbus - Control ------------------
//==============================================================================
unsigned char ECblueModbusRequestChangeAddress(unsigned char DeviceAddress, unsigned char NewAddress, s_board_IO_on_off *IO, unsigned char IO_size, void (*CompletedFunc)(unsigned char Failure));

unsigned char ECblueModbusCreateAlarm(unsigned char DeviceAddress, int FunctionCode, int Index, int Value, int Group);
unsigned char ECblueModbusAlarmActive(unsigned char DeviceAddress);
unsigned char ECblueModbusClearAllAlarms(unsigned char DeviceAddress, int FunctionCode, int Index);

//==============================================================================
//------------------------ ZiehlAbegg ECblue-Modbus - Main routines ------------
//==============================================================================
//------------------------------------------------------------------------------
unsigned char ECblueModbusMain(int DeviceAddress);

#endif
