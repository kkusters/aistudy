// CH_DPT_MOD.H

#ifndef _CH_DPT_MOD_H
#define _CH_DPT_MOD_H

#include <time.h>

#include "ct_data.h"

//==============================================================================
//------------------------ DPT-MOD - Typedefs ----------------------------------
//==============================================================================
typedef enum
{
  dptProgramVersion = 0,
  dptPressure       = 1
} TDptModInputRegister;

//==============================================================================
//------------------------ DPT-MOD - Prototyping -------------------------------
//==============================================================================
unsigned char DptModCreateAlarm(unsigned char DeviceAddress, int FunctionCode, int Index, int Value, int Group);
unsigned char DptModAlarmActive(unsigned char DeviceAddress);
unsigned char DptModClearAllAlarms(unsigned char DeviceAddress, int FunctionCode, int Index);

void DptModMain(int DeviceAddress);

#endif
