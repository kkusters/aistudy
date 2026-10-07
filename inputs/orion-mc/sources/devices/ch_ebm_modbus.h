// CH_EBM_MODBUS.H

#ifndef _CH_EBM_MODBUS_H
#define _CH_EBM_MODBUS_H

#include <time.h>

#include "ct_data.h"
#include "ch_timer.h"

//==============================================================================
//---------------------------------- EBM modBus --------------------------------
//==============================================================================
// Input registers
#define EM_IDENTIFICATION    0xD000
#define EM_ACTUAL_SPEED      0xD010
#define EM_MOTOR_STATUS      0xD011
#define EM_WARNING           0xD012
#define EM_TEMP_POWER_MODULE 0xD015
#define EM_TEMP_MOTOR        0xD016
#define EM_TEMP_ELECTRONICS  0xD017
#define EM_CURRENT_MOD_LEVEL 0xD019
#define EM_CURRENT_POWER     0xD021

// Holding registers
#define EM_RESET                     0xD000
#define EM_DEFAULT_SET_VALUE         0xD001
#define EM_PASSWORD                  0xD002
#define EM_OPERATING_HOURS           0xD009
#define EM_OPERATING_MINUTES         0xD00A
#define EM_FAN_ADDRESS               0xD100
#define EM_SOURCE_SET_VALUE          0xD101
#define EM_STORE_SET_VALUE	         0xD103
#define EM_PARAMETER_SET_SOURCE      0xD104
#define EM_INTERNAL_PARAMETER_SET    0xD105
#define EM_OPERATION_MODE_SET1       0xD106
#define EM_OPERATION_MODE_SET2       0xD107
#define EM_MAXIMUM_SPEED             0xD119
#define EM_MAXIMUM_PERMISSIBLE_SPEED 0xD11A
#define EM_RAMP_UP_CURVE             0xD11F
#define EM_RAMP_DOWN_CURVE           0xD120
#define EM_EMERGENCY_DIRECTION       0xD15B
#define EM_EMERGENCY_OPERATION       0xD15C
#define EM_EMERGENCY_SET_VALUE       0xD15D
#define EM_EMERGENCY_TIME_LAG        0xD15E
#define EM_OPERATING_HOURS_BACKUP    0xD180
#define EM_REFERENCE_VOLTAGE         0xD1A0
#define EM_REFERENCE_CURRENT         0xD1A1

//==============================================================================
//------------------------ ebm modbus - Control --------------------------------
//==============================================================================
unsigned char ebmModbusRequestChangeAddress(unsigned char DeviceAddress, unsigned char NewAddress, s_board_IO_on_off *IO, unsigned char IO_size, void (*CompletedFunc)(unsigned char Failure));
unsigned char ebmModbusRequestChangeMaxRpm(unsigned char DeviceAddress, unsigned int MaxRpm);

unsigned char ebmModbusCreateAlarm(unsigned char DeviceAddress, int FunctionCode, int Index, int Value, int Group);
unsigned char ebmModbusAlarmActive(unsigned char DeviceAddress);
unsigned char ebmModbusClearAllAlarms(unsigned char DeviceAddress, int FunctionCode, int Index);

//==============================================================================
//------------------------ ebm modbus - Main routines --------------------------
//==============================================================================
//------------------------------------------------------------------------------
unsigned char ebmModbusMain(int DeviceAddress);

#endif
