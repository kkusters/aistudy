// CH_MOTOR.H

#ifndef _CH_MOTOR_H
#define _CH_MOTOR_H

#include <time.h>

#include "ch_timer.h"
#include "ch_bacnet_object.h"

//==============================================================================
//------------------------ Motor - Defines -------------------------------------
//==============================================================================
#define MOTOR_MANAGEMENT_REQUEST_FLAG ((unsigned char)0x01)
#define MOTOR_MANAGEMENT_SEND_PC_FLAG ((unsigned char)0x02)

#define MOTOR_EMCY_BIT_1 ((unsigned char)0x01) // Noodstop actief
#define MOTOR_EMCY_BIT_2 ((unsigned char)0x02) // Thermische beveiliging actief
#define MOTOR_EMCY_BIT_3 ((unsigned char)0x04) // Draaimoment fout (encoder fout)
#define MOTOR_EMCY_BIT_4 ((unsigned char)0x08) // Gelijkloopregeling fout
#define MOTOR_EMCY_BIT_5 ((unsigned char)0x10) // Hardware fout

#define MOTOR_STATE_WARNING          0x0200
#define MOTOR_STATE_FAILURE_FEEDBACK 0x4000

//==============================================================================
//------------------------ Motor - ErrorCodes ----------------------------------
//==============================================================================
typedef enum
{
  // Alarm from motor control (6 bits = max 63)
  acNoError                  =  0,
  acEmergencySwitch          =  2,
  acThermalClose             =  5,
  acThermalOpen              =  6,
  acFreqControllerInput      =  7,
  acBreakInput               =  8,
  acErrorCommunication       = 10,
  acSpeedNotEqual            = 11,
  acPulseSpeedToLow          = 12,
  acEncFailure               = 13,
  acNoFeedback               = 14,
  acDeviationPositionAlarm   = 15,
  acNotSynchronous           = 16,
  acLimitSwitchNotReached    = 17,
  acWrongDirection           = 18,
  acNoPulses                 = 19,
  acNotEnoughPulses          = 20,
  ecPulseSpeedToFast         = 21,
  acInstallMode              = 22,
  acDirectionsNotDefined     = 23,
  acEncoderFailureA          = 24,
  acEncoderFailureB          = 25,
  acMultipleMaster           = 30,
  acSlaveNotInit             = 31,
  acNotInstalled             = 36,
  acEncInterference          = 40,
  acDualScreenNotPossible    = 41,
  ecDeviationPositionWarning = 42,
  acLimitSwitchSafetySpeed   = 43,
  acLimitSwitchesNotEqual    = 44,
  acWarningManual            = 46,
  acWarningEmergencyPower    = 47,
  // Alarm from orion (64 is first alarm not used by motor control)
  acBoardCommunication       = 64
} TMotorAlarmCode;

//==============================================================================
//------------------------ Motor - Typedefs ------------------------------------
//==============================================================================
typedef struct
{
  time_t Runtime;
  long   Switches;
  int    Failures;
} TMotorManagement;

typedef struct
{
  unsigned char Enabled;
  unsigned char CycleTime;
  int CorrectionOpen;
  int CorrectionClose;
  sSetpPulseSystem *Setpoints;
  TTimer Timer_1min;
} TPulseSystem;

typedef struct
{
  unsigned Disabled    : 1;
  unsigned Init        : 1;
  unsigned LimitOpen   : 1;
  unsigned LimitClose  : 1;
  unsigned HoldOpen    : 1;
  unsigned HoldClose   : 1;
  unsigned ChangeTimer : 1;
  unsigned HiSpeed     : 1;
  unsigned Sync        : 1;
} TMotorFlags;

typedef struct SMotor
{
  unsigned char Number;
  unsigned char CtrlMotorManagement;
  unsigned char TimerPositionAlarm;
  TMotorManagement Management;
  TRunningMode     RunningMode;
  TMotorFlags      Flags;
  TTimer           Timer_1s;
  TTimer           Timer_5s;
  TTimer           Timer_1min;
  struct SMotor *Next;
  struct SMotor *Prev;
  struct SMotor *Link;
  TPulseSystem PulseSystem;
} TMotor;

//==============================================================================
//------------------------ Motor - Globals -------------------------------------
//==============================================================================
extern TMotor Motor[MAX_MOTOR];

//==============================================================================
//------------------------ Motorgroup - Initialisation -------------------------
//==============================================================================
void CreateMotor(TMotor *pMotor, unsigned char Number);
void InitMotor(TMotor *pMotor);

//==============================================================================
//------------------------ Motor - Control -------------------------------------
//==============================================================================
unsigned char MotorGetEmcyBits(TMotor *pMotor);
void SetMotorHold(TMotor *pMotor, TRunningMode RunningMode);
void SetMotorRelease(TMotor *pMotor, TRunningMode RunningMode);
OPERATION_MODE Get_Motor_Operation_Mode(TMotor *pMotor);
OPERATION_STATE Get_Motor_Operation_State(TMotor *pMotor);

void ControlMotor(TMotor *pMotor);

#endif
