// CH_ECBLUE_PREMIUM.H

#ifndef _CH_ECBLUE_PREMIUM_H
#define _CH_ECBLUE_PREMIUM_H

#include <time.h>

#include "ct_data.h"
#include "ch_timer.h"

//==============================================================================
//------------------------ ZiehlAbegg ECblue Premium - modBus ------------------
//==============================================================================
typedef enum
{
  ecpSpeed                      = 0,
  ecpErrorCode                  = 11,
  ecpVersionOfProgram           = 12,
  ecpTempIGBT                   = 23,
  ecpTempELKO                   = 24,
  ecpDcVoltage                  = 25,
  ecpSupplyVoltage              = 26,
  ecpMotorCurrent               = 27,
  ecpOperationSecondsController = 28,
  ecpOperationSecondsMotor      = 30
} TECbluePremiumInputRegister;

typedef enum
{
  ecpSettingInternal_1   =   9,
  ecpSettingExternalMode =  17,
  ecpBusAddress          = 100,
  ecpWriteProtectAddress = 103,
  ecpMaximumSpeed        = 113,
  ecpWatchdogMode        = 149,
  ecpWatchdogTime        = 150,
  ecpBusmodeE2           = 151,
  ecpE2                  = 9000
} TECbluePremiumHoldingRegister;

typedef enum
{
  ecpNoError                = 128,
  ecpGeneralError           = 129,
  ecpMotorFault             = 131,
  ecpMotorBlocked           = 132,
  ecpHeatSinkTemperature    = 133,
  ecpGroundFault            = 134,
  ecpHallIcFault            = 135,
  ecpOverCurrent            = 136,
  ecpLineFault              = 137,
  ecpLineInterruptHeatSink  = 138,
  ecpDCvoltageToHigh        = 139,
  ecpWrongDirectionRotation = 140,
  ecpTemperatureLowering    = 141,
  ecpWrongConnection        = 142,
  ecpExternalFault          = 143,
  ecpFactorySettingsLoaded  = 144,
  ecpEepError               = 145,
  ecpRtcFaultGeneral        = 146,
  ecpRtcFaultVoltage        = 147,
  ecpFilterAlarm            = 148,
  ecpTransferError          = 150,
  ecpDataLineFault          = 151,
  ecpDataChecksumFault      = 152,
  ecpSensorFaultInput1      = 160,
  ecpSensorFaultInput2      = 161,
  ecpSensorFaultInput3      = 162
} TECbluePremiumErrorCodes;

//==============================================================================
//------------------------ ECblue Premium - Control ----------------------------
//==============================================================================
unsigned char ECbluePremiumRequestChangeAddress(unsigned char DeviceAddress, unsigned char NewAddress, s_board_IO_on_off *IO, unsigned char IO_size, void (*CompletedFunc)(unsigned char Failure));

unsigned char ECbluePremiumCreateAlarm(unsigned char DeviceAddress, int FunctionCode, int Index, int Value, int Group);
unsigned char ECbluePremiumAlarmActive(unsigned char DeviceAddress);
unsigned char ECbluePremiumClearAllAlarms(unsigned char DeviceAddress, int FunctionCode, int Index);

//==============================================================================
//------------------------ ECblue Premium - Main routines ----------------------
//==============================================================================
//------------------------------------------------------------------------------
unsigned char ECbluePremiumMain(int DeviceAddress);

#endif
