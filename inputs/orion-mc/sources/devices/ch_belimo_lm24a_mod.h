// CH_BELIMO_LM24A_MOD.H

#ifndef _CH_BELIMO_LM24A_MOD_H
#define _CH_BELIMO_LM24A_MOD_H

#include <time.h>

#include "ct_data.h"

//==============================================================================
//------------------------ Belimo LM24A-MOD ------------------------------------
//==============================================================================
typedef enum
{
  blmSetpoint             =   0,
  blmOverrideControl      =   1,
  blmCommand              =   2,
  blmActuatorType         =   3,
  blmRelativePosition     =   4,
  blmAbsolutePosition     =   5,
  blmSensorValue          =   8,
  blmMalfunction          = 104,
  blmMin                  = 105,
  blmMax                  = 106,
  blmSensorType           = 107,
  blmBusFailPosition      = 108
} TBelimoLm24aModRegister;

//==============================================================================
//------------------------ Belimo LM24A-MOD ------------------------------------
//==============================================================================
unsigned char BelimoModbusCreateAlarm(unsigned char DeviceAddress, int FunctionCode, int Index, int Value, int Group);
unsigned char BelimoModbusAlarmActive(unsigned char DeviceAddress);
unsigned char BelimoModbusClearAllAlarms(unsigned char DeviceAddress, int FunctionCode, int Index);

void BelimoModbusMain(int DeviceAddress);

#endif
