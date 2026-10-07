// C__DS301.C

#include "ch_define.h"

#include "ch_alarm.h"
#include "ch_motor.h"
#include "ch_motorgroep.h"
#include "ch_device.h"

#include <co_acces.h>
#include <co_emcy.h>
#include <co_pdo.h>
#include <co_stru.h>
#include <co_type.h>

#include "objects.h"
#include "ch_DS301.h"

#ifdef CANopen

static unsigned char NrEmcys = 0;

//------------------------------------------------------------------------------------------------------
RET_T CANopenEmcyReq(UNSIGNED16 errCode, UNSIGNED16 manu1Err, UNSIGNED16 manu2Err, UNSIGNED8  manu3Err)
{
RET_T RetValue = CO_OK;

  if (opt_alg.can_backbone)
  {
    UNSIGNED8 *pData; // pointer to data
    UNSIGNED32  size; // object size

    // get error register
    if (getObjAddr(0x1001, 0, &pData, &size CO_COMMA_LINE_PARA) != CO_OK)
      return(CO_E_NOT_EXIST);

    if (manu1Err >= EMCY_LOST_HEARTBEAT)
      *pData |= 0x10; // set Bit 4 - communication error
    else if (manu1Err > EMCY_NO_ERROR)
    {
      *pData |= 0x80; // set Bit 7 - manufacturer specific
      if (NrEmcys < 0xFF)
        NrEmcys++;
    }

    RetValue = writeEmcyReq(errCode, manu1Err, manu2Err, manu3Err);
  }
  return (RetValue);
}

//------------------------------------------------------------------------------------------------------
RET_T CANopenEmcyErase(UNSIGNED16 errCode, UNSIGNED16 manu1Err, UNSIGNED16 manu2Err, UNSIGNED8  manu3Err)
{
RET_T RetValue = CO_OK;

  errCode; // Not used yet
  if (opt_alg.can_backbone)
  {
    UNSIGNED8 *pData; // pointer to data
    UNSIGNED32  size; // object size

    // get error register
    if (getObjAddr(0x1001, 0, &pData, &size CO_COMMA_LINE_PARA) != CO_OK)
      return(CO_E_NOT_EXIST);

    eraseErr(manu1Err);
    if (manu1Err >= EMCY_LOST_HEARTBEAT)
      *pData &= 0xEF; // clear Bit 4 - communication error
    else if (manu1Err > EMCY_NO_ERROR)
    {
      if (NrEmcys > 0)
        NrEmcys--;
      if (NrEmcys == 0)
        *pData &= 0x7F; // clear Bit 7 - manufacturer specific
    }

    if ((*pData & 0xFE) == 0)
    {
      *pData &= 0xFE; // clear Bit 0 - generic error
      RetValue = writeEmcyReq(0, 0, 0, 0); // all errors gone
    }
    else
    {
      RetValue = writeEmcyReq(0, manu1Err, manu2Err, manu3Err); // still errors active
    }
  }
  return (RetValue);
}

//==============================================================================
//------------------------ Alarm - CANopen emergency producer ------------------
//==============================================================================
void CANopenCreateEmcy(int al_code, int al_index, int al_value)
{
  switch (al_code)
  {
    case MOTORGROUP_MANUAL_AL                       : CANopenEmcyReq(0xFF00, EMCY_GROUP_MANUAL                     | (al_index + 1), 0,                                   al_value); break;
    case MOTOR_MANUAL_AL                            : CANopenEmcyReq(0xFF00, EMCY_MOTOR_MANUAL                     | (al_index + 1), opt_app.Motor[al_index].GroupNumber, al_value); break;
    case MOTOR_EMERGENCYSWITCH_AL                   : CANopenEmcyReq(0xFF02, EMCY_MOTOR_EMERGENCYSWITCH            | (al_index + 1), opt_app.Motor[al_index].GroupNumber, al_value); break;
    case MOTOR_THERMAL_CLOSE_AL                     : CANopenEmcyReq(0xFF01, EMCY_MOTOR_THERMAL_CLOSE              | (al_index + 1), opt_app.Motor[al_index].GroupNumber, al_value); break;
    case MOTOR_THERMAL_OPEN_AL                      : CANopenEmcyReq(0xFF01, EMCY_MOTOR_THERMAL_OPEN               | (al_index + 1), opt_app.Motor[al_index].GroupNumber, al_value); break;
    case MOTOR_BREAK_INPUT_AL                       : CANopenEmcyReq(0xFF0A, EMCY_MOTOR_BREAK_INPUT                | (al_index + 1), opt_app.Motor[al_index].GroupNumber, al_value); break;
    case MOTOR_SPEED_TO_LOW_AL                      : CANopenEmcyReq(0xFF00, EMCY_MOTOR_SPEED_TO_LOW               | (al_index + 1), opt_app.Motor[al_index].GroupNumber, al_value); break;
    case MOTOR_ENCODER_FAILURE_AL                   : CANopenEmcyReq(0xFF00, EMCY_MOTOR_ENCODER_FAILURE            | (al_index + 1), opt_app.Motor[al_index].GroupNumber, al_value); break;
    case MOTOR_NO_FEEDBACK_AL                       : CANopenEmcyReq(0xFF00, EMCY_MOTOR_NO_FEEDBACK                | (al_index + 1), opt_app.Motor[al_index].GroupNumber, al_value); break;
    case MOTOR_NOT_ENOUGH_PULSES_AL                 : CANopenEmcyReq(0xFF00, EMCY_MOTOR_NOT_ENOUGH_PULSES          | (al_index + 1), opt_app.Motor[al_index].GroupNumber, al_value); break;
    case MOTOR_PULSES_TO_FAST_AL                    : CANopenEmcyReq(0xFF00, EMCY_MOTOR_PULSES_TO_FAST             | (al_index + 1), opt_app.Motor[al_index].GroupNumber, al_value); break;
    case MOTOR_NOT_INSTALLED_AL                     : CANopenEmcyReq(0xFF0A, EMCY_MOTOR_NOT_INSTALLED              | (al_index + 1), opt_app.Motor[al_index].GroupNumber, al_value); break;
    case MOTOR_ENCODER_INTERFERENCE_AL              : CANopenEmcyReq(0xFF0A, EMCY_MOTOR_ENCODER_INTERFERENCE       | (al_index + 1), opt_app.Motor[al_index].GroupNumber, al_value); break;
    case MOTOR_INSTALL_MODE_AL                      : CANopenEmcyReq(0xFF0A, EMCY_MOTOR_INSTALL_MODE               | (al_index + 1), opt_app.Motor[al_index].GroupNumber, al_value); break;
    case MOTOR_ERROR_DEVIATION_POSITION_AL          : CANopenEmcyReq(0xFF00, EMCY_MOTOR_ERROR_DEVIATION_POSITION   | (al_index + 1), opt_app.Motor[al_index].GroupNumber, al_value); break;
    case MOTOR_WARNING_DEVIATION_POSITION_AL        : CANopenEmcyReq(0xFF00, EMCY_MOTOR_WARNING_DEVIATION_POSITION | (al_index + 1), opt_app.Motor[al_index].GroupNumber, al_value); break;
    case MOTOR_LIMITSWITCH_SAFETY_SPEED_AL          : CANopenEmcyReq(0xFF03, EMCY_MOTOR_LIMITSWITCH_SAFETY_SPEED   | (al_index + 1), opt_app.Motor[al_index].GroupNumber, al_value); break;
    case MOTOR_DIRECTIONS_NOT_DEFINED_AL            : CANopenEmcyReq(0xFF0A, EMCY_MOTOR_DIRECTIONS_NOT_DEFINED     | (al_index + 1), opt_app.Motor[al_index].GroupNumber, al_value); break;
    case MOTOR_DUALSCREEN_NOT_POSSIBLE_AL           : CANopenEmcyReq(0xFF04, EMCY_MOTOR_DUALSCREEN_NOT_POSSIBLE    | (al_index + 1), opt_app.Motor[al_index].GroupNumber, al_value); break;
    case MOTOR_COMMUNICATION_AL                     : CANopenEmcyReq(0xFF04, EMCY_MOTOR_SLAVE_COMMUNICATION        | (al_index + 1), opt_app.Motor[al_index].GroupNumber, al_value); break;
    case MOTOR_LIMITSWITCHES_NOT_EQUAL_AL           : CANopenEmcyReq(0xFF04, EMCY_MOTOR_LIMITSWITCHES_NOT_EQUAL    | (al_index + 1), opt_app.Motor[al_index].GroupNumber, al_value); break;
    case MOTOR_SPEED_NOT_EQUAL_AL                   : CANopenEmcyReq(0xFF04, EMCY_MOTOR_SPEED_NOT_EQUAL            | (al_index + 1), opt_app.Motor[al_index].GroupNumber, al_value); break;
    case MOTOR_MULTIPLE_MASTER_AL                   : CANopenEmcyReq(0xFF04, EMCY_MOTOR_MULTIPLE_MASTER            | (al_index + 1), opt_app.Motor[al_index].GroupNumber, al_value); break;
    case MOTOR_SLAVE_NOT_INIT_AL                    : CANopenEmcyReq(0xFF04, EMCY_MOTOR_SLAVE_NOT_INIT             | (al_index + 1), opt_app.Motor[al_index].GroupNumber, al_value); break;
    case MOTOR_FREQUENCY_CONTROLLER_AL              : CANopenEmcyReq(0xFF03, EMCY_MOTOR_FREQUENCY_CONTROLLER       | (al_index + 1), opt_app.Motor[al_index].GroupNumber, al_value); break;
    case MOTOR_NOT_SYNCHRONOUS_AL                   : CANopenEmcyReq(0xFF04, EMCY_MOTOR_NOT_SYNCHRONOUS            | (al_index + 1), opt_app.Motor[al_index].GroupNumber, al_value); break;
    case MOTOR_LIMITSWITCH_NOT_REACHED_AL           : CANopenEmcyReq(0xFF04, EMCY_MOTOR_LIMITSWITCH_NOT_REACHED    | (al_index + 1), opt_app.Motor[al_index].GroupNumber, al_value); break;
    case MOTOR_WRONG_DIRECTION_AL                   : CANopenEmcyReq(0xFF00, EMCY_MOTOR_WRONG_DIRECTION            | (al_index + 1), opt_app.Motor[al_index].GroupNumber, al_value); break;
    case MOTOR_LINK_UNKNOWN_AL                      : CANopenEmcyReq(0xFF04, EMCY_MOTOR_LINK_UNKNOWN               | (al_index + 1), opt_app.Motor[al_index].GroupNumber, al_value); break;
    case MOTOR_NOT_RUNNING_AL                       : CANopenEmcyReq(0xFF0A, EMCY_MOTOR_NOT_RUNNING                | (al_index + 1), opt_app.Motor[al_index].GroupNumber, al_value); break;
    case MOTOR_ENCODER_FAILURE_A_AL                 : CANopenEmcyReq(0xFF00, EMCY_MOTOR_ENCODER_FAILURE_A          | (al_index + 1), opt_app.Motor[al_index].GroupNumber, al_value); break;
    case MOTOR_ENCODER_FAILURE_B_AL                 : CANopenEmcyReq(0xFF00, EMCY_MOTOR_ENCODER_FAILURE_B          | (al_index + 1), opt_app.Motor[al_index].GroupNumber, al_value); break;
    case MOTOR_POSITION_NOT_REACHED_AL              : CANopenEmcyReq(0xFF00, EMCY_MOTOR_POSITION_NOT_REACHED       | (al_index + 1), opt_app.Motor[al_index].GroupNumber, al_value); break;
    case MOTOR_UNKNOWN_AL                           : CANopenEmcyReq(0xFF0A, EMCY_MOTOR_UNKNOWN                    | (al_index + 1), opt_app.Motor[al_index].GroupNumber, al_value); break;

    case VENT_MANUAL_AL                             : 
    case KLEP_MANUAL_AL                             : CANopenEmcyReq(0xFF00, EMCY_DEVICE_MANUAL                    | (al_index + 1), opt_app.Device[al_index].GroupNumber,  al_value); break;

    case VENT_TARGET_NOT_REACHED_AL                 : 
    case KLEP_TARGET_NOT_REACHED_AL                 : CANopenEmcyReq(0xFF00, EMCY_DEVICE_TARGET_NOT_REACHED        | (al_index + 1), opt_app.Device[al_index].GroupNumber,  al_value); break;
    case KLEP_LIMITSWITCH_AL                        : CANopenEmcyReq(0xFF00, EMCY_DEVICE_LIMITSWITCH               | (al_index + 1), opt_app.Device[al_index].GroupNumber,  al_value); break;
    case KLEP_EXTERN_ALARM_AL                       : CANopenEmcyReq(0xFF00, EMCY_DEVICE_EXTERN_ALARM              | (al_index + 1), opt_app.Device[al_index].GroupNumber,  al_value); break;

    case VENT_MB_COMMUNICATION_AL                   : CANopenEmcyReq(0xFF0A, EMCY_DEVICE_COMMUNICATION             | (al_index + 1), opt_app.Device[al_index].GroupNumber,  al_value); break;
    case VENT_MB_LOCKED_MOTOR_AL                    : CANopenEmcyReq(0xFF00, EMCY_VENT_LOCKED_MOTOR                | (al_index + 1), opt_app.Device[al_index].GroupNumber,  al_value); break;
    case VENT_MB_HALL_FAILURE_AL                    : CANopenEmcyReq(0xFF00, EMCY_VENT_HALL_FAILURE                | (al_index + 1), opt_app.Device[al_index].GroupNumber,  al_value); break;
    case VENT_MB_THERMAL_MOTOR_AL                   : CANopenEmcyReq(0xFF01, EMCY_VENT_THERMAL_MOTOR               | (al_index + 1), opt_app.Device[al_index].GroupNumber,  al_value); break;
    case VENT_MB_COMM_ERROR_MASTER_SLAVE_AL         : CANopenEmcyReq(0xFF0A, EMCY_VENT_COMM_ERROR_MASTER_SLAVE     | (al_index + 1), opt_app.Device[al_index].GroupNumber,  al_value); break;
    case VENT_MB_THERMAL_POWER_MODULE_AL            : CANopenEmcyReq(0xFF01, EMCY_VENT_THERMAL_POWER_MODULE        | (al_index + 1), opt_app.Device[al_index].GroupNumber,  al_value); break;
    case VENT_MB_COMM_ERROR_REMOTE_UNIT_AL          : CANopenEmcyReq(0xFF0A, EMCY_VENT_COMM_ERROR_REMOTE_UNIT      | (al_index + 1), opt_app.Device[al_index].GroupNumber,  al_value); break;
    case VENT_MB_PHASE_FAILURE_AL                   : CANopenEmcyReq(0xFF0A, EMCY_VENT_PHASE_FAILURE               | (al_index + 1), opt_app.Device[al_index].GroupNumber,  al_value); break;
    case VENT_MB_BRAKE_AL                           : CANopenEmcyReq(0xFF0A, EMCY_VENT_BRAKE                       | (al_index + 1), opt_app.Device[al_index].GroupNumber,  al_value); break;
    case VENT_MB_LOW_LINE_VOLTAGE_AL                : CANopenEmcyReq(0xFF0A, EMCY_VENT_LOW_LINE_VOLTAGE            | (al_index + 1), opt_app.Device[al_index].GroupNumber,  al_value); break;
    case VENT_MB_LOW_DC_LINK_VOLTAGE_AL             : CANopenEmcyReq(0xFF0A, EMCY_VENT_LOW_DC_LINK_VOLTAGE         | (al_index + 1), opt_app.Device[al_index].GroupNumber,  al_value); break;
    case VENT_MB_HIGH_DC_LINK_VOLTAGE_AL            : CANopenEmcyReq(0xFF0A, EMCY_VENT_HIGH_DC_LINK_VOLTAGE        | (al_index + 1), opt_app.Device[al_index].GroupNumber,  al_value); break;
    case VENT_MB_DRIVER_PROBLEM_AL                  : CANopenEmcyReq(0xFF0A, EMCY_VENT_DRIVER_PROBLEM              | (al_index + 1), opt_app.Device[al_index].GroupNumber,  al_value); break;
    case VENT_MB_ELECTRONIC_BOX_OVER_HEAT_AL        : CANopenEmcyReq(0xFF01, EMCY_VENT_ELECTRONIC_BOX_OVER_HEAT    | (al_index + 1), opt_app.Device[al_index].GroupNumber,  al_value); break;
    case VENT_MB_EXCESSIVE_DC_LINK_CURRENT_AL       : CANopenEmcyReq(0xFF0A, EMCY_VENT_EXCESSIVE_DC_LINK_CURRENT   | (al_index + 1), opt_app.Device[al_index].GroupNumber,  al_value); break;
    case VENT_MB_UNKNOWN_AL                         : CANopenEmcyReq(0xFF0A, EMCY_VENT_UNKNOWN                     | (al_index + 1), opt_app.Device[al_index].GroupNumber,  al_value); break;

    case KLEP_MB_COMMUNICATION_AL                   : CANopenEmcyReq(0xFF0A, EMCY_DEVICE_COMMUNICATION             | (al_index + 1), opt_app.Device[al_index].GroupNumber,  al_value); break;
    case DRUKVERSCHIL_MB_COMMUNICATION_AL           : CANopenEmcyReq(0xFF0A, EMCY_DEVICE_COMMUNICATION             | (al_index + 1), 0,                                     al_value); break;
  }
}

void CANopenClearEmcy(int al_code, int al_index)
{
  switch (al_code)
  {
    case MOTORGROUP_MANUAL_AL                       : CANopenEmcyErase(0xFF00, EMCY_GROUP_MANUAL                     | (al_index + 1), 0,                                   0); break;
    case MOTOR_MANUAL_AL                            : CANopenEmcyErase(0xFF00, EMCY_MOTOR_MANUAL                     | (al_index + 1), opt_app.Motor[al_index].GroupNumber, 0); break;
    case MOTOR_EMERGENCYSWITCH_AL                   : CANopenEmcyErase(0xFF02, EMCY_MOTOR_EMERGENCYSWITCH            | (al_index + 1), opt_app.Motor[al_index].GroupNumber, 0); break;
    case MOTOR_THERMAL_CLOSE_AL                     : CANopenEmcyErase(0xFF01, EMCY_MOTOR_THERMAL_CLOSE              | (al_index + 1), opt_app.Motor[al_index].GroupNumber, 0); break;
    case MOTOR_THERMAL_OPEN_AL                      : CANopenEmcyErase(0xFF01, EMCY_MOTOR_THERMAL_OPEN               | (al_index + 1), opt_app.Motor[al_index].GroupNumber, 0); break;
    case MOTOR_BREAK_INPUT_AL                       : CANopenEmcyErase(0xFF0A, EMCY_MOTOR_BREAK_INPUT                | (al_index + 1), opt_app.Motor[al_index].GroupNumber, 0); break;
    case MOTOR_SPEED_TO_LOW_AL                      : CANopenEmcyErase(0xFF00, EMCY_MOTOR_SPEED_TO_LOW               | (al_index + 1), opt_app.Motor[al_index].GroupNumber, 0); break;
    case MOTOR_ENCODER_FAILURE_AL                   : CANopenEmcyErase(0xFF00, EMCY_MOTOR_ENCODER_FAILURE            | (al_index + 1), opt_app.Motor[al_index].GroupNumber, 0); break;
    case MOTOR_NO_FEEDBACK_AL                       : CANopenEmcyErase(0xFF00, EMCY_MOTOR_NO_FEEDBACK                | (al_index + 1), opt_app.Motor[al_index].GroupNumber, 0); break;
    case MOTOR_NOT_ENOUGH_PULSES_AL                 : CANopenEmcyErase(0xFF00, EMCY_MOTOR_NOT_ENOUGH_PULSES          | (al_index + 1), opt_app.Motor[al_index].GroupNumber, 0); break;
    case MOTOR_PULSES_TO_FAST_AL                    : CANopenEmcyErase(0xFF00, EMCY_MOTOR_PULSES_TO_FAST             | (al_index + 1), opt_app.Motor[al_index].GroupNumber, 0); break;
    case MOTOR_NOT_INSTALLED_AL                     : CANopenEmcyErase(0xFF0A, EMCY_MOTOR_NOT_INSTALLED              | (al_index + 1), opt_app.Motor[al_index].GroupNumber, 0); break;
    case MOTOR_ENCODER_INTERFERENCE_AL              : CANopenEmcyErase(0xFF0A, EMCY_MOTOR_ENCODER_INTERFERENCE       | (al_index + 1), opt_app.Motor[al_index].GroupNumber, 0); break;
    case MOTOR_INSTALL_MODE_AL                      : CANopenEmcyErase(0xFF0A, EMCY_MOTOR_INSTALL_MODE               | (al_index + 1), opt_app.Motor[al_index].GroupNumber, 0); break;
    case MOTOR_ERROR_DEVIATION_POSITION_AL          : CANopenEmcyErase(0xFF00, EMCY_MOTOR_ERROR_DEVIATION_POSITION   | (al_index + 1), opt_app.Motor[al_index].GroupNumber, 0); break;
    case MOTOR_WARNING_DEVIATION_POSITION_AL        : CANopenEmcyErase(0xFF00, EMCY_MOTOR_WARNING_DEVIATION_POSITION | (al_index + 1), opt_app.Motor[al_index].GroupNumber, 0); break;
    case MOTOR_LIMITSWITCH_SAFETY_SPEED_AL          : CANopenEmcyErase(0xFF03, EMCY_MOTOR_LIMITSWITCH_SAFETY_SPEED   | (al_index + 1), opt_app.Motor[al_index].GroupNumber, 0); break;
    case MOTOR_DIRECTIONS_NOT_DEFINED_AL            : CANopenEmcyErase(0xFF0A, EMCY_MOTOR_DIRECTIONS_NOT_DEFINED     | (al_index + 1), opt_app.Motor[al_index].GroupNumber, 0); break;
    case MOTOR_DUALSCREEN_NOT_POSSIBLE_AL           : CANopenEmcyErase(0xFF04, EMCY_MOTOR_DUALSCREEN_NOT_POSSIBLE    | (al_index + 1), opt_app.Motor[al_index].GroupNumber, 0); break;
    case MOTOR_COMMUNICATION_AL                     : CANopenEmcyErase(0xFF04, EMCY_MOTOR_SLAVE_COMMUNICATION        | (al_index + 1), opt_app.Motor[al_index].GroupNumber, 0); break;
    case MOTOR_LIMITSWITCHES_NOT_EQUAL_AL           : CANopenEmcyErase(0xFF04, EMCY_MOTOR_LIMITSWITCHES_NOT_EQUAL    | (al_index + 1), opt_app.Motor[al_index].GroupNumber, 0); break;
    case MOTOR_SPEED_NOT_EQUAL_AL                   : CANopenEmcyErase(0xFF04, EMCY_MOTOR_SPEED_NOT_EQUAL            | (al_index + 1), opt_app.Motor[al_index].GroupNumber, 0); break;
    case MOTOR_MULTIPLE_MASTER_AL                   : CANopenEmcyErase(0xFF04, EMCY_MOTOR_MULTIPLE_MASTER            | (al_index + 1), opt_app.Motor[al_index].GroupNumber, 0); break;
    case MOTOR_SLAVE_NOT_INIT_AL                    : CANopenEmcyErase(0xFF04, EMCY_MOTOR_SLAVE_NOT_INIT             | (al_index + 1), opt_app.Motor[al_index].GroupNumber, 0); break;
    case MOTOR_FREQUENCY_CONTROLLER_AL              : CANopenEmcyErase(0xFF03, EMCY_MOTOR_FREQUENCY_CONTROLLER       | (al_index + 1), opt_app.Motor[al_index].GroupNumber, 0); break;
    case MOTOR_NOT_SYNCHRONOUS_AL                   : CANopenEmcyErase(0xFF04, EMCY_MOTOR_NOT_SYNCHRONOUS            | (al_index + 1), opt_app.Motor[al_index].GroupNumber, 0); break;
    case MOTOR_LIMITSWITCH_NOT_REACHED_AL           : CANopenEmcyErase(0xFF04, EMCY_MOTOR_LIMITSWITCH_NOT_REACHED    | (al_index + 1), opt_app.Motor[al_index].GroupNumber, 0); break;
    case MOTOR_WRONG_DIRECTION_AL                   : CANopenEmcyErase(0xFF00, EMCY_MOTOR_WRONG_DIRECTION            | (al_index + 1), opt_app.Motor[al_index].GroupNumber, 0); break;
    case MOTOR_LINK_UNKNOWN_AL                      : CANopenEmcyErase(0xFF04, EMCY_MOTOR_LINK_UNKNOWN               | (al_index + 1), opt_app.Motor[al_index].GroupNumber, 0); break;
    case MOTOR_NOT_RUNNING_AL                       : CANopenEmcyErase(0xFF0A, EMCY_MOTOR_NOT_RUNNING                | (al_index + 1), opt_app.Motor[al_index].GroupNumber, 0); break;
    case MOTOR_ENCODER_FAILURE_A_AL                 : CANopenEmcyErase(0xFF00, EMCY_MOTOR_ENCODER_FAILURE_A          | (al_index + 1), opt_app.Motor[al_index].GroupNumber, 0); break;
    case MOTOR_ENCODER_FAILURE_B_AL                 : CANopenEmcyErase(0xFF00, EMCY_MOTOR_ENCODER_FAILURE_B          | (al_index + 1), opt_app.Motor[al_index].GroupNumber, 0); break;
    case MOTOR_POSITION_NOT_REACHED_AL              : CANopenEmcyErase(0xFF00, EMCY_MOTOR_POSITION_NOT_REACHED       | (al_index + 1), opt_app.Motor[al_index].GroupNumber, 0); break;
    case MOTOR_UNKNOWN_AL                           : CANopenEmcyErase(0xFF0A, EMCY_MOTOR_UNKNOWN                    | (al_index + 1), opt_app.Motor[al_index].GroupNumber, 0); break;

    case VENT_MANUAL_AL                             :
    case KLEP_MANUAL_AL                             : CANopenEmcyErase(0xFF00, EMCY_DEVICE_MANUAL                    | (al_index + 1), opt_app.Device[al_index].GroupNumber,  0); break;

    case VENT_TARGET_NOT_REACHED_AL                 :
    case KLEP_TARGET_NOT_REACHED_AL                 : CANopenEmcyErase(0xFF00, EMCY_DEVICE_TARGET_NOT_REACHED        | (al_index + 1), opt_app.Device[al_index].GroupNumber,  0); break;
    case KLEP_LIMITSWITCH_AL                        : CANopenEmcyErase(0xFF00, EMCY_DEVICE_LIMITSWITCH               | (al_index + 1), opt_app.Device[al_index].GroupNumber,  0); break;
    case KLEP_EXTERN_ALARM_AL                       : CANopenEmcyErase(0xFF00, EMCY_DEVICE_EXTERN_ALARM              | (al_index + 1), opt_app.Device[al_index].GroupNumber,  0); break;

    case VENT_MB_COMMUNICATION_AL                   : CANopenEmcyErase(0xFF0A, EMCY_DEVICE_COMMUNICATION             | (al_index + 1), opt_app.Device[al_index].GroupNumber,  0); break;
    case VENT_MB_LOCKED_MOTOR_AL                    : CANopenEmcyErase(0xFF00, EMCY_VENT_LOCKED_MOTOR                | (al_index + 1), opt_app.Device[al_index].GroupNumber,  0); break;
    case VENT_MB_HALL_FAILURE_AL                    : CANopenEmcyErase(0xFF00, EMCY_VENT_HALL_FAILURE                | (al_index + 1), opt_app.Device[al_index].GroupNumber,  0); break;
    case VENT_MB_THERMAL_MOTOR_AL                   : CANopenEmcyErase(0xFF01, EMCY_VENT_THERMAL_MOTOR               | (al_index + 1), opt_app.Device[al_index].GroupNumber,  0); break;
    case VENT_MB_COMM_ERROR_MASTER_SLAVE_AL         : CANopenEmcyErase(0xFF0A, EMCY_VENT_COMM_ERROR_MASTER_SLAVE     | (al_index + 1), opt_app.Device[al_index].GroupNumber,  0); break;
    case VENT_MB_THERMAL_POWER_MODULE_AL            : CANopenEmcyErase(0xFF01, EMCY_VENT_THERMAL_POWER_MODULE        | (al_index + 1), opt_app.Device[al_index].GroupNumber,  0); break;
    case VENT_MB_COMM_ERROR_REMOTE_UNIT_AL          : CANopenEmcyErase(0xFF0A, EMCY_VENT_COMM_ERROR_REMOTE_UNIT      | (al_index + 1), opt_app.Device[al_index].GroupNumber,  0); break;
    case VENT_MB_PHASE_FAILURE_AL                   : CANopenEmcyErase(0xFF0A, EMCY_VENT_PHASE_FAILURE               | (al_index + 1), opt_app.Device[al_index].GroupNumber,  0); break;
    case VENT_MB_BRAKE_AL                           : CANopenEmcyErase(0xFF0A, EMCY_VENT_BRAKE                       | (al_index + 1), opt_app.Device[al_index].GroupNumber,  0); break;
    case VENT_MB_LOW_LINE_VOLTAGE_AL                : CANopenEmcyErase(0xFF0A, EMCY_VENT_LOW_LINE_VOLTAGE            | (al_index + 1), opt_app.Device[al_index].GroupNumber,  0); break;
    case VENT_MB_LOW_DC_LINK_VOLTAGE_AL             : CANopenEmcyErase(0xFF0A, EMCY_VENT_LOW_DC_LINK_VOLTAGE         | (al_index + 1), opt_app.Device[al_index].GroupNumber,  0); break;
    case VENT_MB_HIGH_DC_LINK_VOLTAGE_AL            : CANopenEmcyErase(0xFF0A, EMCY_VENT_HIGH_DC_LINK_VOLTAGE        | (al_index + 1), opt_app.Device[al_index].GroupNumber,  0); break;
    case VENT_MB_DRIVER_PROBLEM_AL                  : CANopenEmcyErase(0xFF0A, EMCY_VENT_DRIVER_PROBLEM              | (al_index + 1), opt_app.Device[al_index].GroupNumber,  0); break;
    case VENT_MB_ELECTRONIC_BOX_OVER_HEAT_AL        : CANopenEmcyErase(0xFF01, EMCY_VENT_ELECTRONIC_BOX_OVER_HEAT    | (al_index + 1), opt_app.Device[al_index].GroupNumber,  0); break;
    case VENT_MB_EXCESSIVE_DC_LINK_CURRENT_AL       : CANopenEmcyErase(0xFF0A, EMCY_VENT_EXCESSIVE_DC_LINK_CURRENT   | (al_index + 1), opt_app.Device[al_index].GroupNumber,  0); break;
    case VENT_MB_UNKNOWN_AL                         : CANopenEmcyErase(0xFF0A, EMCY_VENT_UNKNOWN                     | (al_index + 1), opt_app.Device[al_index].GroupNumber,  0); break;

    case KLEP_MB_COMMUNICATION_AL                   : CANopenEmcyErase(0xFF0A, EMCY_DEVICE_COMMUNICATION             | (al_index + 1), opt_app.Device[al_index].GroupNumber,  0); break;
    case DRUKVERSCHIL_MB_COMMUNICATION_AL           : CANopenEmcyErase(0xFF0A, EMCY_DEVICE_COMMUNICATION             | (al_index + 1), 0,                                     0); break;
  }
}

#endif // CANopen

