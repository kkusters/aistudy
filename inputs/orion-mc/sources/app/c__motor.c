// C__MOTOR.C  

#include "ch_define.h"

#include "ch_alarm.h"
#include "ch_alg.h"
#include "ch_ds301.h"
#include "ch_IO.h"
#include "ch_motor.h"

//==============================================================================
//------------------------ Motor - Globals -------------------------------------
//==============================================================================
//------------------------------------------------------------------------------
TMotor Motor[MAX_MOTOR];

static int MotorState = 0;

//==============================================================================
//------------------------ Motor - Prototyping ---------------------------------
//==============================================================================
//------------------------------------------------------------------------------

//==============================================================================
//------------------------ Motor - Initialisation ------------------------------
//==============================================================================
//------------------------------------------------------------------------------
void CreateMotor(TMotor *pMotor, unsigned char Number)
{
  pMotor->Number     = Number;
  pMotor->Flags.Init = 1;
}

void InitMotor(TMotor *pMotor)
{
  TimerSet(&pMotor->Timer_1s,   TIMER_1SEC);
  TimerSet(&pMotor->Timer_1min, TIMER_1MIN);
  TimerSet(&pMotor->PulseSystem.Timer_1min, TIMER_1MIN);
  pMotor->Flags.Init = 0;
}

//==============================================================================
//------------------------ Motor - Options -------------------------------------
//==============================================================================
//------------------------------------------------------------------------------
static void GetRuntimeMotor(TMotor *pMotor)
{
int Runtime;

  Runtime = IO_Get_Motor_Control_Runtime(&opt_app.Motor[pMotor->Number].IO);
  if (Runtime != 0)
    opt_app.Motor[pMotor->Number].Runtime = Runtime;
}

//==============================================================================
//------------------------ Motor - Alarm ---------------------------------------
//==============================================================================
//------------------------------------------------------------------------------
static void ResetAlarmMotor(TMotor *pMotor)
{
  ClearAlarm(&alarm_hr_alg.Motor[pMotor->Number].Manual,                   MOTOR_MANUAL_AL,                     pMotor->Number);
  ClearAlarm(&alarm_hr_alg.Motor[pMotor->Number].EmergencySwitch,          MOTOR_EMERGENCYSWITCH_AL,            pMotor->Number);
  ClearAlarm(&alarm_hr_alg.Motor[pMotor->Number].ThermalClose,             MOTOR_THERMAL_CLOSE_AL,              pMotor->Number);
  ClearAlarm(&alarm_hr_alg.Motor[pMotor->Number].ThermalOpen,              MOTOR_THERMAL_OPEN_AL,               pMotor->Number);
  ClearAlarm(&alarm_hr_alg.Motor[pMotor->Number].BreakInput,               MOTOR_BREAK_INPUT_AL,                pMotor->Number);
  ClearAlarm(&alarm_hr_alg.Motor[pMotor->Number].SpeedToLow,               MOTOR_SPEED_TO_LOW_AL,               pMotor->Number);
  ClearAlarm(&alarm_hr_alg.Motor[pMotor->Number].EncoderFailure,           MOTOR_ENCODER_FAILURE_AL,            pMotor->Number);
  ClearAlarm(&alarm_hr_alg.Motor[pMotor->Number].NoFeedback,               MOTOR_NO_FEEDBACK_AL,                pMotor->Number);
  ClearAlarm(&alarm_hr_alg.Motor[pMotor->Number].NotEnoughPulses,          MOTOR_NOT_ENOUGH_PULSES_AL,          pMotor->Number);
  ClearAlarm(&alarm_hr_alg.Motor[pMotor->Number].PulsesToFast,             MOTOR_PULSES_TO_FAST_AL,             pMotor->Number);
  ClearAlarm(&alarm_hr_alg.Motor[pMotor->Number].NotInstalled,             MOTOR_NOT_INSTALLED_AL,              pMotor->Number);
  ClearAlarm(&alarm_hr_alg.Motor[pMotor->Number].EncoderInterference,      MOTOR_ENCODER_INTERFERENCE_AL,       pMotor->Number);
  ClearAlarm(&alarm_hr_alg.Motor[pMotor->Number].InstallMode,              MOTOR_INSTALL_MODE_AL,               pMotor->Number);
  ClearAlarm(&alarm_hr_alg.Motor[pMotor->Number].ErrorDeviationPosition,   MOTOR_ERROR_DEVIATION_POSITION_AL,   pMotor->Number);
  ClearAlarm(&alarm_hr_alg.Motor[pMotor->Number].WarningDeviationPosition, MOTOR_WARNING_DEVIATION_POSITION_AL, pMotor->Number);
  ClearAlarm(&alarm_hr_alg.Motor[pMotor->Number].LimitSwitchSafetySpeed,   MOTOR_LIMITSWITCH_SAFETY_SPEED_AL,   pMotor->Number);
  ClearAlarm(&alarm_hr_alg.Motor[pMotor->Number].DirectionsNotDefined,     MOTOR_DIRECTIONS_NOT_DEFINED_AL,     pMotor->Number);
  ClearAlarm(&alarm_hr_alg.Motor[pMotor->Number].DualscreenNotPossible,    MOTOR_DUALSCREEN_NOT_POSSIBLE_AL,    pMotor->Number);
  ClearAlarm(&alarm_hr_alg.Motor[pMotor->Number].ErrorCommunication,       MOTOR_COMMUNICATION_AL,              pMotor->Number);
  ClearAlarm(&alarm_hr_alg.Motor[pMotor->Number].LimitSwitchesNotEqual,    MOTOR_LIMITSWITCHES_NOT_EQUAL_AL,    pMotor->Number);
  ClearAlarm(&alarm_hr_alg.Motor[pMotor->Number].SpeedNotEqual,            MOTOR_SPEED_NOT_EQUAL_AL,            pMotor->Number);
  ClearAlarm(&alarm_hr_alg.Motor[pMotor->Number].MultipleMaster,           MOTOR_MULTIPLE_MASTER_AL,            pMotor->Number);
  ClearAlarm(&alarm_hr_alg.Motor[pMotor->Number].SlaveNotInit,             MOTOR_SLAVE_NOT_INIT_AL,             pMotor->Number);
  ClearAlarm(&alarm_hr_alg.Motor[pMotor->Number].FrequencyController,      MOTOR_FREQUENCY_CONTROLLER_AL,       pMotor->Number);
  ClearAlarm(&alarm_hr_alg.Motor[pMotor->Number].NotSynchronous,           MOTOR_NOT_SYNCHRONOUS_AL,            pMotor->Number);
  ClearAlarm(&alarm_hr_alg.Motor[pMotor->Number].LimitswitchNotReached,    MOTOR_LIMITSWITCH_NOT_REACHED_AL,    pMotor->Number);
  ClearAlarm(&alarm_hr_alg.Motor[pMotor->Number].WrongDirection,           MOTOR_WRONG_DIRECTION_AL,            pMotor->Number);
  ClearAlarm(&alarm_hr_alg.Motor[pMotor->Number].LinkUnknown,              MOTOR_LINK_UNKNOWN_AL,               pMotor->Number);
  ClearAlarm(&alarm_hr_alg.Motor[pMotor->Number].MotorNotRunning,          MOTOR_NOT_RUNNING_AL,                pMotor->Number);
  ClearAlarm(&alarm_hr_alg.Motor[pMotor->Number].EncoderFailureA,          MOTOR_ENCODER_FAILURE_A_AL,          pMotor->Number);
  ClearAlarm(&alarm_hr_alg.Motor[pMotor->Number].EncoderFailureB,          MOTOR_ENCODER_FAILURE_B_AL,          pMotor->Number);
  ClearAlarm(&alarm_hr_alg.Motor[pMotor->Number].Unknown,                  MOTOR_UNKNOWN_AL,                    pMotor->Number);
  ClearAlarm(&alarm_hr_alg.Motor[pMotor->Number].PositionNotReached,       MOTOR_POSITION_NOT_REACHED_AL,       pMotor->Number);
}

//------------------------------------------------------------------------------
unsigned char MotorGetEmcyBits(TMotor *pMotor)
{
unsigned char Emcy = 0;

  if (alarm_hr_alg.Motor[pMotor->Number].Manual)                   Emcy |= MOTOR_EMCY_BIT_3;
  if (alarm_hr_alg.Motor[pMotor->Number].EmergencySwitch)          Emcy |= MOTOR_EMCY_BIT_1;
  if (alarm_hr_alg.Motor[pMotor->Number].ThermalClose)             Emcy |= MOTOR_EMCY_BIT_2;
  if (alarm_hr_alg.Motor[pMotor->Number].ThermalOpen)              Emcy |= MOTOR_EMCY_BIT_2;
  if (alarm_hr_alg.Motor[pMotor->Number].BreakInput)               Emcy |= MOTOR_EMCY_BIT_5;
  if (alarm_hr_alg.Motor[pMotor->Number].SpeedToLow)               Emcy |= MOTOR_EMCY_BIT_3;
  if (alarm_hr_alg.Motor[pMotor->Number].EncoderFailure)           Emcy |= MOTOR_EMCY_BIT_3;
  if (alarm_hr_alg.Motor[pMotor->Number].EncoderFailureA)          Emcy |= MOTOR_EMCY_BIT_3;
  if (alarm_hr_alg.Motor[pMotor->Number].EncoderFailureB)          Emcy |= MOTOR_EMCY_BIT_3;
  if (alarm_hr_alg.Motor[pMotor->Number].NoFeedback)               Emcy |= MOTOR_EMCY_BIT_3;
  if (alarm_hr_alg.Motor[pMotor->Number].NotEnoughPulses)          Emcy |= MOTOR_EMCY_BIT_3;
  if (alarm_hr_alg.Motor[pMotor->Number].PulsesToFast)             Emcy |= MOTOR_EMCY_BIT_3;
  if (alarm_hr_alg.Motor[pMotor->Number].NotInstalled)             Emcy |= MOTOR_EMCY_BIT_5;
  if (alarm_hr_alg.Motor[pMotor->Number].InstallMode)              Emcy |= MOTOR_EMCY_BIT_5;
  if (alarm_hr_alg.Motor[pMotor->Number].DirectionsNotDefined)     Emcy |= MOTOR_EMCY_BIT_5;
  if (alarm_hr_alg.Motor[pMotor->Number].EncoderInterference)      Emcy |= MOTOR_EMCY_BIT_5;
  if (alarm_hr_alg.Motor[pMotor->Number].DualscreenNotPossible)    Emcy |= MOTOR_EMCY_BIT_4;
  if (alarm_hr_alg.Motor[pMotor->Number].ErrorDeviationPosition)   Emcy |= MOTOR_EMCY_BIT_3;
  if (alarm_hr_alg.Motor[pMotor->Number].WarningDeviationPosition) Emcy |= MOTOR_EMCY_BIT_3;
  if (alarm_hr_alg.Motor[pMotor->Number].LimitSwitchSafetySpeed)   Emcy |= MOTOR_EMCY_BIT_5;
  if (alarm_hr_alg.Motor[pMotor->Number].ErrorCommunication)       Emcy |= MOTOR_EMCY_BIT_4;
  if (alarm_hr_alg.Motor[pMotor->Number].LimitSwitchesNotEqual)    Emcy |= MOTOR_EMCY_BIT_4;
  if (alarm_hr_alg.Motor[pMotor->Number].SpeedNotEqual)            Emcy |= MOTOR_EMCY_BIT_4;
  if (alarm_hr_alg.Motor[pMotor->Number].MultipleMaster)           Emcy |= MOTOR_EMCY_BIT_4;
  if (alarm_hr_alg.Motor[pMotor->Number].SlaveNotInit)             Emcy |= MOTOR_EMCY_BIT_4;
  if (alarm_hr_alg.Motor[pMotor->Number].FrequencyController)      Emcy |= MOTOR_EMCY_BIT_5;
  if (alarm_hr_alg.Motor[pMotor->Number].NotSynchronous)           Emcy |= MOTOR_EMCY_BIT_4;
  if (alarm_hr_alg.Motor[pMotor->Number].LimitswitchNotReached)    Emcy |= MOTOR_EMCY_BIT_4;
  if (alarm_hr_alg.Motor[pMotor->Number].WrongDirection)           Emcy |= MOTOR_EMCY_BIT_3;
  if (alarm_hr_alg.Motor[pMotor->Number].LinkUnknown)              Emcy |= MOTOR_EMCY_BIT_4;
  if (alarm_hr_alg.Motor[pMotor->Number].MotorNotRunning)          Emcy |= MOTOR_EMCY_BIT_5;
  if (alarm_hr_alg.Motor[pMotor->Number].Unknown)                  Emcy |= MOTOR_EMCY_BIT_5;
  if (alarm_hr_alg.Motor[pMotor->Number].PositionNotReached)       Emcy |= MOTOR_EMCY_BIT_3;

  return (Emcy);
}

//------------------------------------------------------------------------------
static void CreateAlarmMotor(TMotor *pMotor, TMotorAlarmCode AlarmCode, int AlarmSlave)
{
  switch (AlarmCode)
  {
    case acNoError                  : break;
    case acEmergencySwitch          : CreateAlarm(&alarm_hr_alg.Motor[pMotor->Number].EmergencySwitch,          MOTOR_EMERGENCYSWITCH_AL,            pMotor->Number, AlarmSlave, opt_app.Motor[pMotor->Number].GroupNumber, HARD_ALARM ); break;
    case acThermalClose             : CreateAlarm(&alarm_hr_alg.Motor[pMotor->Number].ThermalClose,             MOTOR_THERMAL_CLOSE_AL,              pMotor->Number, AlarmSlave, opt_app.Motor[pMotor->Number].GroupNumber, HARD_ALARM ); break;
    case acThermalOpen              : CreateAlarm(&alarm_hr_alg.Motor[pMotor->Number].ThermalOpen,              MOTOR_THERMAL_OPEN_AL,               pMotor->Number, AlarmSlave, opt_app.Motor[pMotor->Number].GroupNumber, HARD_ALARM ); break;
    case acFreqControllerInput      : CreateAlarm(&alarm_hr_alg.Motor[pMotor->Number].FrequencyController,      MOTOR_FREQUENCY_CONTROLLER_AL,       pMotor->Number, AlarmSlave, opt_app.Motor[pMotor->Number].GroupNumber, HARD_ALARM ); break;
    case acBreakInput               : CreateAlarm(&alarm_hr_alg.Motor[pMotor->Number].BreakInput,               MOTOR_BREAK_INPUT_AL,                pMotor->Number, AlarmSlave, opt_app.Motor[pMotor->Number].GroupNumber, HARD_ALARM ); break;
    case acErrorCommunication       : CreateAlarm(&alarm_hr_alg.Motor[pMotor->Number].ErrorCommunication,       MOTOR_COMMUNICATION_AL,              pMotor->Number, AlarmSlave, opt_app.Motor[pMotor->Number].GroupNumber, HARD_ALARM ); break;
    case acSpeedNotEqual            : CreateAlarm(&alarm_hr_alg.Motor[pMotor->Number].SpeedNotEqual,            MOTOR_SPEED_NOT_EQUAL_AL,            pMotor->Number, AlarmSlave, opt_app.Motor[pMotor->Number].GroupNumber, HARD_ALARM ); break;
    case acPulseSpeedToLow          : CreateAlarm(&alarm_hr_alg.Motor[pMotor->Number].SpeedToLow,               MOTOR_SPEED_TO_LOW_AL,               pMotor->Number, AlarmSlave, opt_app.Motor[pMotor->Number].GroupNumber, HARD_ALARM ); break;
    case acEncFailure               : CreateAlarm(&alarm_hr_alg.Motor[pMotor->Number].EncoderFailure,           MOTOR_ENCODER_FAILURE_AL,            pMotor->Number, AlarmSlave, opt_app.Motor[pMotor->Number].GroupNumber, &alarm_hr_alg.Motor[pMotor->Number].AlarmEncoderHard); break;
    case acNoFeedback               : CreateAlarm(&alarm_hr_alg.Motor[pMotor->Number].NoFeedback,               MOTOR_NO_FEEDBACK_AL,                pMotor->Number, AlarmSlave, opt_app.Motor[pMotor->Number].GroupNumber, HARD_ALARM ); break;
    case acDeviationPositionAlarm   : CreateAlarm(&alarm_hr_alg.Motor[pMotor->Number].ErrorDeviationPosition,   MOTOR_ERROR_DEVIATION_POSITION_AL,   pMotor->Number, AlarmSlave, opt_app.Motor[pMotor->Number].GroupNumber, HARD_ALARM ); break;
    case acNotSynchronous           : CreateAlarm(&alarm_hr_alg.Motor[pMotor->Number].NotSynchronous,           MOTOR_NOT_SYNCHRONOUS_AL,            pMotor->Number, AlarmSlave, opt_app.Motor[pMotor->Number].GroupNumber, HARD_ALARM ); break;
    case acLimitSwitchNotReached    : CreateAlarm(&alarm_hr_alg.Motor[pMotor->Number].LimitswitchNotReached,    MOTOR_LIMITSWITCH_NOT_REACHED_AL,    pMotor->Number, AlarmSlave, opt_app.Motor[pMotor->Number].GroupNumber, HARD_ALARM ); break;
    case acWrongDirection           : CreateAlarm(&alarm_hr_alg.Motor[pMotor->Number].WrongDirection,           MOTOR_WRONG_DIRECTION_AL,            pMotor->Number, AlarmSlave, opt_app.Motor[pMotor->Number].GroupNumber, HARD_ALARM ); break;
    case acNoPulses                 : CreateAlarm(&alarm_hr_alg.Motor[pMotor->Number].MotorNotRunning,          MOTOR_NOT_RUNNING_AL,                pMotor->Number, AlarmSlave, opt_app.Motor[pMotor->Number].GroupNumber, &alarm_hr_alg.Motor[pMotor->Number].AlarmEncoderHard); break;
    case acNotEnoughPulses          : CreateAlarm(&alarm_hr_alg.Motor[pMotor->Number].NotEnoughPulses,          MOTOR_NOT_ENOUGH_PULSES_AL,          pMotor->Number, AlarmSlave, opt_app.Motor[pMotor->Number].GroupNumber, HARD_ALARM ); break;
    case ecPulseSpeedToFast         : CreateAlarm(&alarm_hr_alg.Motor[pMotor->Number].PulsesToFast,             MOTOR_PULSES_TO_FAST_AL,             pMotor->Number, AlarmSlave, opt_app.Motor[pMotor->Number].GroupNumber, HARD_ALARM ); break;
    case acInstallMode              : CreateAlarm(&alarm_hr_alg.Motor[pMotor->Number].InstallMode,              MOTOR_INSTALL_MODE_AL,               pMotor->Number, AlarmSlave, opt_app.Motor[pMotor->Number].GroupNumber, HARD_ALARM ); break;
    case acDirectionsNotDefined     : CreateAlarm(&alarm_hr_alg.Motor[pMotor->Number].DirectionsNotDefined,     MOTOR_DIRECTIONS_NOT_DEFINED_AL,     pMotor->Number, AlarmSlave, opt_app.Motor[pMotor->Number].GroupNumber, HARD_ALARM ); break;
    case acEncoderFailureA          : CreateAlarm(&alarm_hr_alg.Motor[pMotor->Number].EncoderFailureA,          MOTOR_ENCODER_FAILURE_A_AL,          pMotor->Number, AlarmSlave, opt_app.Motor[pMotor->Number].GroupNumber, HARD_ALARM ); break;
    case acEncoderFailureB          : CreateAlarm(&alarm_hr_alg.Motor[pMotor->Number].EncoderFailureB,          MOTOR_ENCODER_FAILURE_B_AL,          pMotor->Number, AlarmSlave, opt_app.Motor[pMotor->Number].GroupNumber, HARD_ALARM ); break;
    case acMultipleMaster           : CreateAlarm(&alarm_hr_alg.Motor[pMotor->Number].MultipleMaster,           MOTOR_MULTIPLE_MASTER_AL,            pMotor->Number, AlarmSlave, opt_app.Motor[pMotor->Number].GroupNumber, HARD_ALARM ); break;
    case acSlaveNotInit             : CreateAlarm(&alarm_hr_alg.Motor[pMotor->Number].SlaveNotInit,             MOTOR_SLAVE_NOT_INIT_AL,             pMotor->Number, AlarmSlave, opt_app.Motor[pMotor->Number].GroupNumber, HARD_ALARM ); break;
    case acNotInstalled             : CreateAlarm(&alarm_hr_alg.Motor[pMotor->Number].NotInstalled,             MOTOR_NOT_INSTALLED_AL,              pMotor->Number, AlarmSlave, opt_app.Motor[pMotor->Number].GroupNumber, HARD_ALARM ); break;
    case acEncInterference          : CreateAlarm(&alarm_hr_alg.Motor[pMotor->Number].EncoderInterference,      MOTOR_ENCODER_INTERFERENCE_AL,       pMotor->Number, AlarmSlave, opt_app.Motor[pMotor->Number].GroupNumber, HARD_ALARM ); break;
    case acDualScreenNotPossible    : CreateAlarm(&alarm_hr_alg.Motor[pMotor->Number].DualscreenNotPossible,    MOTOR_DUALSCREEN_NOT_POSSIBLE_AL,    pMotor->Number, AlarmSlave, opt_app.Motor[pMotor->Number].GroupNumber, HARD_ALARM ); break;
    case ecDeviationPositionWarning : CreateAlarm(&alarm_hr_alg.Motor[pMotor->Number].WarningDeviationPosition, MOTOR_WARNING_DEVIATION_POSITION_AL, pMotor->Number, AlarmSlave, opt_app.Motor[pMotor->Number].GroupNumber, ZACHT_ALARM); break;
    case acLimitSwitchSafetySpeed   : CreateAlarm(&alarm_hr_alg.Motor[pMotor->Number].LimitSwitchSafetySpeed,   MOTOR_LIMITSWITCH_SAFETY_SPEED_AL,   pMotor->Number, AlarmSlave, opt_app.Motor[pMotor->Number].GroupNumber, ZACHT_ALARM); break;
    case acLimitSwitchesNotEqual    : CreateAlarm(&alarm_hr_alg.Motor[pMotor->Number].LimitSwitchesNotEqual,    MOTOR_LIMITSWITCHES_NOT_EQUAL_AL,    pMotor->Number, AlarmSlave, opt_app.Motor[pMotor->Number].GroupNumber, HARD_ALARM ); break;
    default                         : CreateAlarm(&alarm_hr_alg.Motor[pMotor->Number].Unknown,                  MOTOR_UNKNOWN_AL,                    pMotor->Number, AlarmSlave, opt_app.Motor[pMotor->Number].GroupNumber, ZACHT_ALARM); break;
    case acBoardCommunication       : 
                                      #ifdef CANopen
                                      CANopenEmcyReq(0xFF0A, EMCY_MOTOR_BOARD_COMMUNICATION | (pMotor->Number + 1), opt_app.Motor[pMotor->Number].GroupNumber, 0);
                                      #endif // CANopen
                                      ChangeAlarmGroupBoardIO(&opt_app.Motor[pMotor->Number].IO, opt_app.Motor[pMotor->Number].GroupNumber);
                                      break;
  }
}

//------------------------------------------------------------------------------
static void ClearAlarmMotor(TMotor *pMotor, TMotorAlarmCode AlarmCode)
{
  switch (AlarmCode)
  {
    case acNoError                  : break;
    case acEmergencySwitch          : ClearAlarm(&alarm_hr_alg.Motor[pMotor->Number].EmergencySwitch,          MOTOR_EMERGENCYSWITCH_AL,            pMotor->Number); break;
    case acThermalClose             : ClearAlarm(&alarm_hr_alg.Motor[pMotor->Number].ThermalClose,             MOTOR_THERMAL_CLOSE_AL,              pMotor->Number); break;
    case acThermalOpen              : ClearAlarm(&alarm_hr_alg.Motor[pMotor->Number].ThermalOpen,              MOTOR_THERMAL_OPEN_AL,               pMotor->Number); break;
    case acFreqControllerInput      : ClearAlarm(&alarm_hr_alg.Motor[pMotor->Number].FrequencyController,      MOTOR_FREQUENCY_CONTROLLER_AL,       pMotor->Number); break;
    case acBreakInput               : ClearAlarm(&alarm_hr_alg.Motor[pMotor->Number].BreakInput,               MOTOR_BREAK_INPUT_AL,                pMotor->Number); break;
    case acErrorCommunication       : ClearAlarm(&alarm_hr_alg.Motor[pMotor->Number].ErrorCommunication,       MOTOR_COMMUNICATION_AL,              pMotor->Number); break;
    case acSpeedNotEqual            : ClearAlarm(&alarm_hr_alg.Motor[pMotor->Number].SpeedNotEqual,            MOTOR_SPEED_NOT_EQUAL_AL,            pMotor->Number); break;
    case acPulseSpeedToLow          : ClearAlarm(&alarm_hr_alg.Motor[pMotor->Number].SpeedToLow,               MOTOR_SPEED_TO_LOW_AL,               pMotor->Number); break;
    case acEncFailure               : ClearAlarm(&alarm_hr_alg.Motor[pMotor->Number].EncoderFailure,           MOTOR_ENCODER_FAILURE_AL,            pMotor->Number); break;
    case acNoFeedback               : ClearAlarm(&alarm_hr_alg.Motor[pMotor->Number].NoFeedback,               MOTOR_NO_FEEDBACK_AL,                pMotor->Number); break;
    case acDeviationPositionAlarm   : ClearAlarm(&alarm_hr_alg.Motor[pMotor->Number].ErrorDeviationPosition,   MOTOR_ERROR_DEVIATION_POSITION_AL,   pMotor->Number); break;
    case acNotSynchronous           : ClearAlarm(&alarm_hr_alg.Motor[pMotor->Number].NotSynchronous,           MOTOR_NOT_SYNCHRONOUS_AL,            pMotor->Number); break;
    case acLimitSwitchNotReached    : ClearAlarm(&alarm_hr_alg.Motor[pMotor->Number].LimitswitchNotReached,    MOTOR_LIMITSWITCH_NOT_REACHED_AL,    pMotor->Number); break;
    case acWrongDirection           : ClearAlarm(&alarm_hr_alg.Motor[pMotor->Number].WrongDirection,           MOTOR_WRONG_DIRECTION_AL,            pMotor->Number); break;
    case acNoPulses                 : ClearAlarm(&alarm_hr_alg.Motor[pMotor->Number].MotorNotRunning,          MOTOR_NOT_RUNNING_AL,                pMotor->Number); break;
    case acNotEnoughPulses          : ClearAlarm(&alarm_hr_alg.Motor[pMotor->Number].NotEnoughPulses,          MOTOR_NOT_ENOUGH_PULSES_AL,          pMotor->Number); break;
    case ecPulseSpeedToFast         : ClearAlarm(&alarm_hr_alg.Motor[pMotor->Number].PulsesToFast,             MOTOR_PULSES_TO_FAST_AL,             pMotor->Number); break;
    case acInstallMode              : ClearAlarm(&alarm_hr_alg.Motor[pMotor->Number].InstallMode,              MOTOR_INSTALL_MODE_AL,               pMotor->Number); break;
    case acDirectionsNotDefined     : ClearAlarm(&alarm_hr_alg.Motor[pMotor->Number].DirectionsNotDefined,     MOTOR_DIRECTIONS_NOT_DEFINED_AL,     pMotor->Number); break;
    case acEncoderFailureA          : ClearAlarm(&alarm_hr_alg.Motor[pMotor->Number].EncoderFailureA,          MOTOR_ENCODER_FAILURE_A_AL,          pMotor->Number); break;
    case acEncoderFailureB          : ClearAlarm(&alarm_hr_alg.Motor[pMotor->Number].EncoderFailureB,          MOTOR_ENCODER_FAILURE_B_AL,          pMotor->Number); break;
    case acMultipleMaster           : ClearAlarm(&alarm_hr_alg.Motor[pMotor->Number].MultipleMaster,           MOTOR_MULTIPLE_MASTER_AL,            pMotor->Number); break;
    case acSlaveNotInit             : ClearAlarm(&alarm_hr_alg.Motor[pMotor->Number].SlaveNotInit,             MOTOR_SLAVE_NOT_INIT_AL,             pMotor->Number); break;
    case acNotInstalled             : ClearAlarm(&alarm_hr_alg.Motor[pMotor->Number].NotInstalled,             MOTOR_NOT_INSTALLED_AL,              pMotor->Number); break;
    case acEncInterference          : ClearAlarm(&alarm_hr_alg.Motor[pMotor->Number].EncoderInterference,      MOTOR_ENCODER_INTERFERENCE_AL,       pMotor->Number); break;
    case acDualScreenNotPossible    : ClearAlarm(&alarm_hr_alg.Motor[pMotor->Number].DualscreenNotPossible,    MOTOR_DUALSCREEN_NOT_POSSIBLE_AL,    pMotor->Number); break;
    case ecDeviationPositionWarning : ClearAlarm(&alarm_hr_alg.Motor[pMotor->Number].WarningDeviationPosition, MOTOR_WARNING_DEVIATION_POSITION_AL, pMotor->Number); break;
    case acLimitSwitchSafetySpeed   : ClearAlarm(&alarm_hr_alg.Motor[pMotor->Number].LimitSwitchSafetySpeed,   MOTOR_LIMITSWITCH_SAFETY_SPEED_AL,   pMotor->Number); break;
    case acLimitSwitchesNotEqual    : ClearAlarm(&alarm_hr_alg.Motor[pMotor->Number].LimitSwitchesNotEqual,    MOTOR_LIMITSWITCHES_NOT_EQUAL_AL,    pMotor->Number); break;
    default                         : ClearAlarm(&alarm_hr_alg.Motor[pMotor->Number].Unknown,                  MOTOR_UNKNOWN_AL,                    pMotor->Number); break;
    case acBoardCommunication       : 
                                      #ifdef CANopen
                                      CANopenEmcyErase(0xFF0A, EMCY_MOTOR_BOARD_COMMUNICATION | (pMotor->Number + 1), opt_app.Motor[pMotor->Number].GroupNumber, 0);
                                      #endif // CANopen
                                      break;
  }
}

//------------------------------------------------------------------------------
static void CheckMotorManualAlarm(TMotor *pMotor)
{
  if (val_hr_alg.Motor[pMotor->Number].Overruled || (val_hr_alg.Motor[pMotor->Number].OperationMode == omManual))
  {
    CreateAlarm(&alarm_hr_alg.Motor[pMotor->Number].Manual, MOTOR_MANUAL_AL, pMotor->Number, 0, opt_app.Motor[pMotor->Number].GroupNumber, &alarm_hr_alg.Motor[pMotor->Number].AlarmManualHard);
    if ((setp_alg.Motorgroup[opt_app.Motor[pMotor->Number].GroupNumber - 1].DelayAlarmManual == 0) || (TimeAlarmOn(MOTOR_MANUAL_AL, pMotor->Number) < (setp_alg.Motorgroup[opt_app.Motor[pMotor->Number].GroupNumber - 1].DelayAlarmManual * 60)))
      alarm_hr_alg.Motor[pMotor->Number].AlarmManualHard = 0;
    else
      alarm_hr_alg.Motor[pMotor->Number].AlarmManualHard = 1;
  }
  else
    ClearAlarm(&alarm_hr_alg.Motor[pMotor->Number].Manual, MOTOR_MANUAL_AL, pMotor->Number);
}

//------------------------------------------------------------------------------
static void CheckMotorPositionAlarm(TMotor *pMotor)
{
unsigned char AlarmPosition;
int SetPosition = 0;

  switch (val_hr_alg.Motor[pMotor->Number].OperationMode)
  {
    case omAuto:
      SetPosition = val_hr_alg.Motor[pMotor->Number].PositionAuto;
      break;
    case omManual:
      SetPosition = val_hr_alg.Motor[pMotor->Number].PositionManual;
      break;
    case omOff:
      SetPosition = 0;
      break;
  }

  AlarmPosition = setp_alg.Motorgroup[opt_app.Motor[pMotor->Number].GroupNumber - 1].DiffPositionAlarm;
  if ((AlarmPosition == 0) || (abs_int(val_hr_alg.Motor[pMotor->Number].Position - SetPosition) < AlarmPosition) || val_hr_alg.Motor[pMotor->Number].Overruled || (alarm_hr_alg.Motor[pMotor->Number].AlarmCode != 0))
  {
    TimerRestart(&Motor[pMotor->Number].Timer_1min);
    ClearAlarm(&alarm_hr_alg.Motor[pMotor->Number].PositionNotReached, MOTOR_POSITION_NOT_REACHED_AL, pMotor->Number);
    Motor[pMotor->Number].TimerPositionAlarm = 0;
  }
  else if (TimerExpired(&Motor[pMotor->Number].Timer_1min))
  {
    Motor[pMotor->Number].TimerPositionAlarm++;
    TimerReset(&Motor[pMotor->Number].Timer_1min);
    if (Motor[pMotor->Number].TimerPositionAlarm >= setp_alg.Motorgroup[opt_app.Motor[pMotor->Number].GroupNumber - 1].TimePositionAlarm)
      CreateAlarm(&alarm_hr_alg.Motor[pMotor->Number].PositionNotReached, MOTOR_POSITION_NOT_REACHED_AL, pMotor->Number, 0, opt_app.Motor[pMotor->Number].GroupNumber, HARD_ALARM);
  }
}

//------------------------------------------------------------------------------
static void CheckMotorAlarmCode(TMotor *pMotor)
{
TMotorAlarmCode AlarmCode;
int AlarmSlave;

  if (IO_Get_Board_Alarm(&opt_app.Motor[pMotor->Number].IO))
  {
    AlarmCode  = acBoardCommunication;
    AlarmSlave = 0;
  }
  else
  {
    AlarmCode  = IO_Get_Motor_Alarm_Code(&opt_app.Motor[pMotor->Number].IO);
    AlarmSlave = IO_Get_Motor_Alarm_Slave(&opt_app.Motor[pMotor->Number].IO);
  }
  if ((MotorState & MOTOR_STATE_WARNING) && !(MotorState & MOTOR_STATE_FAILURE_FEEDBACK))
    alarm_hr_alg.Motor[pMotor->Number].AlarmEncoderHard = 0;
  else
    alarm_hr_alg.Motor[pMotor->Number].AlarmEncoderHard = 1;
  if ((alarm_hr_alg.Motor[pMotor->Number].AlarmCode != AlarmCode) || (alarm_hr_alg.Motor[pMotor->Number].AlarmSlave != AlarmSlave))
  {
    ClearAlarmMotor(pMotor, alarm_hr_alg.Motor[pMotor->Number].AlarmCode);
    CreateAlarmMotor(pMotor, AlarmCode, AlarmSlave);
    alarm_hr_alg.Motor[pMotor->Number].AlarmCode  = AlarmCode;
    alarm_hr_alg.Motor[pMotor->Number].AlarmSlave = AlarmSlave;
  }
}

//------------------------------------------------------------------------------
static void ControlAlarmMotor(TMotor *pMotor)
{
  CheckMotorManualAlarm(pMotor);
  CheckMotorAlarmCode(pMotor);
  CheckMotorPositionAlarm(pMotor);
}

//==============================================================================
//------------------------ Motor - Pulse system --------------------------------
//==============================================================================
//------------------------------------------------------------------------------
int SetPulsedPosition(TMotor *pMotor, int Position)
{
int ScaledPosition = Position;
int RealPosition;

  if (pMotor->PulseSystem.Enabled)
  {
    if (Position == 0)
    {
      RealPosition = IO_Get_Motor_Control_Position(&opt_app.Motor[pMotor->Number].IO);
      if (RealPosition < pMotor->PulseSystem.CorrectionOpen)
        pMotor->PulseSystem.CorrectionOpen = RealPosition;
      if (TimerExpired(&pMotor->PulseSystem.Timer_1min))
      {
        TimerReset(&pMotor->PulseSystem.Timer_1min);
        pMotor->PulseSystem.CycleTime++;
        if (pMotor->PulseSystem.CycleTime >= pMotor->PulseSystem.Setpoints->CycleTime)
        {
          pMotor->PulseSystem.CycleTime = 0;
          pMotor->PulseSystem.CorrectionOpen -= pMotor->PulseSystem.Setpoints->PulseWidth;
          if (pMotor->PulseSystem.CorrectionOpen < 0)
            pMotor->PulseSystem.CorrectionOpen = 0;
          else if (pMotor->PulseSystem.CorrectionOpen > pMotor->PulseSystem.Setpoints->PulseZone)
            pMotor->PulseSystem.CorrectionOpen = pMotor->PulseSystem.Setpoints->PulseZone;
        }
      }
      if ((pMotor->PulseSystem.CorrectionOpen + pMotor->PulseSystem.CorrectionClose) != pMotor->PulseSystem.Setpoints->PulseZone)
        pMotor->PulseSystem.CorrectionClose = pMotor->PulseSystem.Setpoints->PulseZone - pMotor->PulseSystem.CorrectionOpen;
    }
    else if (Position == 1000)
    {
      RealPosition = IO_Get_Motor_Control_Position(&opt_app.Motor[pMotor->Number].IO);
      if (RealPosition > (1000 - pMotor->PulseSystem.CorrectionClose))
        pMotor->PulseSystem.CorrectionClose = 1000 - RealPosition;
      if (TimerExpired(&pMotor->PulseSystem.Timer_1min))
      {
        TimerReset(&pMotor->PulseSystem.Timer_1min);
        pMotor->PulseSystem.CycleTime++;
        if (pMotor->PulseSystem.CycleTime >= pMotor->PulseSystem.Setpoints->CycleTime)
        {
          pMotor->PulseSystem.CycleTime = 0;
          pMotor->PulseSystem.CorrectionClose -= pMotor->PulseSystem.Setpoints->PulseWidth;
          if (pMotor->PulseSystem.CorrectionClose < 0)
            pMotor->PulseSystem.CorrectionClose = 0;
          else if (pMotor->PulseSystem.CorrectionClose > pMotor->PulseSystem.Setpoints->PulseZone)
            pMotor->PulseSystem.CorrectionClose = pMotor->PulseSystem.Setpoints->PulseZone;
        }
      }
      if ((pMotor->PulseSystem.CorrectionOpen + pMotor->PulseSystem.CorrectionClose) != pMotor->PulseSystem.Setpoints->PulseZone)
        pMotor->PulseSystem.CorrectionOpen = pMotor->PulseSystem.Setpoints->PulseZone - pMotor->PulseSystem.CorrectionClose;
    }
    else
    {
      if (pMotor->PulseSystem.CorrectionOpen < 0)
        pMotor->PulseSystem.CorrectionOpen = 0;
      else if (pMotor->PulseSystem.CorrectionOpen > pMotor->PulseSystem.Setpoints->PulseZone)
        pMotor->PulseSystem.CorrectionOpen = pMotor->PulseSystem.Setpoints->PulseZone;
      if (pMotor->PulseSystem.CorrectionClose < 0)
        pMotor->PulseSystem.CorrectionClose = 0;
      else if (pMotor->PulseSystem.CorrectionClose > pMotor->PulseSystem.Setpoints->PulseZone)
        pMotor->PulseSystem.CorrectionClose = pMotor->PulseSystem.Setpoints->PulseZone;
      TimerSet(&pMotor->PulseSystem.Timer_1min, TIMER_1MIN);
      pMotor->PulseSystem.CycleTime = 0;
    }
    ScaledPosition = Calc_Prop(0, 1000, pMotor->PulseSystem.CorrectionOpen, 1000 - pMotor->PulseSystem.CorrectionClose, Position);
  }
  return (ScaledPosition);
}

//------------------------------------------------------------------------------
static int GetPulsedPosition(TMotor *pMotor, int Position)
{
int ScaledPosition = Position;

  if (pMotor->PulseSystem.Enabled)
    ScaledPosition = Calc_Prop(pMotor->PulseSystem.CorrectionOpen, 1000 - pMotor->PulseSystem.CorrectionClose, 0, 1000, Position);

  return (ScaledPosition);
}

//==============================================================================
//------------------------ Motor - Control -------------------------------------
//==============================================================================
//------------------------------------------------------------------------------
void SetMotorHold(TMotor *pMotor, TRunningMode RunningMode)
{
  IO_Set_Motor_Control_Hold(&opt_app.Motor[pMotor->Number].IO, RunningMode);
}

void SetMotorRelease(TMotor *pMotor, TRunningMode RunningMode)
{
  IO_Set_Motor_Control_Release(&opt_app.Motor[pMotor->Number].IO, RunningMode);
}

//------------------------------------------------------------------------------
static void GetStateMotor(TMotor *pMotor)
{
  MotorState = IO_Get_Motor_Control_State(&opt_app.Motor[pMotor->Number].IO);
}

//------------------------------------------------------------------------------
static void GetRunningModeMotor(TMotor *pMotor)
{
  //    Manual                          Manual External                 Manual without feedback
  if (((MotorState & 0x0007) == 1) || ((MotorState & 0x0007) == 4) || ((MotorState & 0x0007) == 5))
    val_hr_alg.Motor[pMotor->Number].Overruled = 1;
  else
    val_hr_alg.Motor[pMotor->Number].Overruled = 0;
  if (MotorState & 0x0080)
    pMotor->RunningMode = rmOpen;
  else if (MotorState & 0x0040)
    pMotor->RunningMode = rmClose;
  else
    pMotor->RunningMode = rmStop;
// JL test : H2MC stuurt door bij 0 en 100%, daarom controleren of de eindschakelaars bereikt zijn
//  if (MotorState & 0x0080)
//    pMotor->RunningMode = (MotorState & 0x00A0) ? rmStop : rmOpen;
//  else if (MotorState & 0x0040)
//    pMotor->RunningMode = (MotorState & 0x0050) ? rmStop : rmClose;
//  else
//    pMotor->RunningMode = rmStop;
}

//------------------------------------------------------------------------------
static void GetLimitSwitchesMotor(TMotor *pMotor)
{
  if (MotorState & 0x0010)
    pMotor->Flags.LimitClose = 1;
  else
    pMotor->Flags.LimitClose = 0;
  if (MotorState & 0x0020)
    pMotor->Flags.LimitOpen = 1;
  else
    pMotor->Flags.LimitOpen = 0;
}

//------------------------------------------------------------------------------
static void ControlMotorManagement(TMotor *pMotor)
{
  if (pMotor->CtrlMotorManagement & MOTOR_MANAGEMENT_REQUEST_FLAG)
    IO_Get_Motor_Control_MotorManagement(&opt_app.Motor[pMotor->Number].IO, &pMotor->Management);
}

//------------------------------------------------------------------------------
static void SetPosition(TMotor *pMotor, int Position)
{
int ScaledPosition;

  ScaledPosition = SetPulsedPosition(pMotor, Position); // Pulse system
  if (IO_Set_Motor_Control_Position(&opt_app.Motor[pMotor->Number].IO, ScaledPosition))
  {
    TimerRestart(&Motor[pMotor->Number].Timer_1min);
    Motor[pMotor->Number].TimerPositionAlarm = 0;
  }
}

//------------------------------------------------------------------------------
static int GetPosition(TMotor *pMotor)
{
int ScaledPosition;
int Position;

  Position = IO_Get_Motor_Control_Position(&opt_app.Motor[pMotor->Number].IO);
  ScaledPosition = GetPulsedPosition(pMotor, Position); // Pulse system

  if (ScaledPosition != val_hr_alg.Motor[pMotor->Number].Position)
  {
    TimerRestart(&Motor[pMotor->Number].Timer_1min);
    Motor[pMotor->Number].TimerPositionAlarm = 0;
  }

  return (ScaledPosition);
}

//------------------------------------------------------------------------------
OPERATION_MODE Get_Motor_Operation_Mode(TMotor *pMotor)
{
  OPERATION_MODE return_value = OM_NORMAL_OPERATION;
  TMotorAlarmCode alarm_code = alarm_hr_alg.Motor[pMotor->Number].AlarmCode;
  int isOutOfOrder = 0;  

  if (!opt_app.Motor[pMotor->Number].Enabled)
  {
    return OM_NOT_AVAILABLE;
  }

  if (alarm_code)
  {
    isOutOfOrder = 1;
  }

  if (alarm_code == acFreqControllerInput ||
      alarm_code == acEncoderFailureA ||
      alarm_code == acEncoderFailureB ||
      alarm_code == acEncInterference ||
      alarm_code == acDualScreenNotPossible ||
      alarm_code == ecDeviationPositionWarning ||
      alarm_code == acLimitSwitchSafetySpeed ||
      alarm_code == acWarningManual ||
      alarm_code == acWarningEmergencyPower) // ||
//      ((alarm_code == acEncFailure) && (alarm_hr_alg.Motor[pMotor->Number].AlarmEncoderHard == 0)))
  {
    return_value = OM_FAULT;
    isOutOfOrder = 0;
  }

  if ((alarm_code == acEncFailure) && (alarm_hr_alg.Motor[pMotor->Number].AlarmEncoderHard == 0))
    isOutOfOrder = 0;

  if (val_hr_alg.Motor[pMotor->Number].Overruled ||
      val_hr_alg.Motor[pMotor->Number].OperationMode == omManual)
  {
    return_value = OM_MANUAL;
  }

  if (alarm_code == acInstallMode)
  {
    return_value = OM_IN_MAINTENANCE;
    isOutOfOrder = 0;
  }

  if (isOutOfOrder)
  {
    return_value = OM_OUT_OF_ORDER;
  }

  return return_value;
}

//------------------------------------------------------------------------------
// Bij AtLowLimit/AtHighLimit en rmOpen/rmClose wordt hier geen rekening
// gehouden of het een raam of doek is. Controleren en eventueel aanpassen
// wordt gedaan bij de Motorgroep.
OPERATION_STATE Get_Motor_Operation_State(TMotor *pMotor)
{
  static OPERATION_STATE previous_state[MAX_MOTOR];
  OPERATION_STATE return_value = OS_AT_POSITION;

  if (pMotor->RunningMode == rmClose)
    return_value = OS_GOING_DOWN;

  if (pMotor->RunningMode == rmOpen)
    return_value = OS_GOING_UP;

  if (pMotor->Flags.LimitClose)
    return_value = OS_AT_LOW_LIMIT;

  if (pMotor->Flags.LimitOpen)
    return_value = OS_AT_HIGH_LIMIT;

  if ((pMotor->Flags.HoldOpen || pMotor->Flags.HoldClose) && (previous_state[pMotor->Number] != OS_HELD))
    return_value = OS_HOLDING;

  if ((pMotor->Flags.HoldOpen || pMotor->Flags.HoldClose) && (pMotor->RunningMode == rmStop) && ((previous_state[pMotor->Number] == OS_HOLDING) || (previous_state[pMotor->Number] == OS_HELD)))
    return_value = OS_HELD;

  if (!(pMotor->Flags.HoldOpen || pMotor->Flags.HoldClose) && (previous_state[pMotor->Number] == OS_HELD))
    return_value = OS_RESTARTING;
  
  previous_state[pMotor->Number] = return_value;
  return return_value;
}

//------------------------------------------------------------------------------
void ControlMotor(TMotor *pMotor)
{
  if (opt_app.Motor[pMotor->Number].Enabled)
  {
    if (pMotor->Flags.Init)
      InitMotor(pMotor);

    if (TimerExpired(&pMotor->Timer_1s))
    {
      GetStateMotor(pMotor);
      GetRuntimeMotor(pMotor);
      GetRunningModeMotor(pMotor);
      GetLimitSwitchesMotor(pMotor);
      ControlAlarmMotor(pMotor);
      ControlMotorManagement(pMotor);

      val_hr_alg.Motor[pMotor->Number].Position = GetPosition(pMotor);
      switch (val_hr_alg.Motor[pMotor->Number].OperationMode)
      {
        case omAuto:
          SetPosition(pMotor, val_hr_alg.Motor[pMotor->Number].PositionAuto);
          val_hr_alg.Motor[pMotor->Number].PositionManual = val_hr_alg.Motor[pMotor->Number].Position;
          break;
        case omManual:
          SetPosition(pMotor, val_hr_alg.Motor[pMotor->Number].PositionManual);
          break;
        case omOff:
          SetPosition(pMotor, 0);
          val_hr_alg.Motor[pMotor->Number].PositionManual = val_hr_alg.Motor[pMotor->Number].Position;
          break;
      }
      TimerReset(&pMotor->Timer_1s);
      pMotor->Flags.Disabled = 0;

      if (pMotor->Flags.ChangeTimer && TimerExpired(&pMotor->Timer_5s))
      {
        pMotor->Flags.ChangeTimer = 0;
        TimerSet(&pMotor->Timer_1s, TIMER_1SEC);
      }
    }
  }
  else if (pMotor->Flags.Disabled == 0)
  {
    ResetAlarmMotor(pMotor);
    pMotor->Flags.Disabled = 1;
  }
}

//==============================================================================
//------------------------ End of file -----------------------------------------
//==============================================================================
