// CH_MOTORGROEP.H

#ifndef _CH_MOTORGROEP_H
#define _CH_MOTORGROEP_H

#include <time.h>

#include "ch_timer.h"

//==============================================================================
//------------------------ Motor - Defines -------------------------------------
//==============================================================================
#define MOTOR_DELAY 30

#define CONTROL_NORMAL     0
#define CONTROL_DUALSCREEN 1
#define CONTROL_CABRIOKAS  2

#define TYPE_RAAM 0
#define TYPE_DOEK 1
#define TYPE_VENT 2
#define TYPE_KLEP 3

#define SENSOR_TYPE_GEEN           0
#define SENSOR_TYPE_EINDSCHAKELAAR 1
#define SENSOR_TYPE_EXTERN_ALARM   2

#define GROUP_EMCY_BIT_1 ((unsigned char)0x01)
#define GROUP_EMCY_BIT_2 ((unsigned char)0x02)
#define GROUP_EMCY_BIT_3 ((unsigned char)0x04)
#define GROUP_EMCY_BIT_4 ((unsigned char)0x08)
#define GROUP_EMCY_BIT_5 ((unsigned char)0x10)

//==============================================================================
//------------------------ Motor - Typedefs ------------------------------------
//==============================================================================
typedef struct SMotorgroup
{
  unsigned char Number;
  unsigned char Disabled;
  unsigned char InitFlag;
  unsigned char Delay;
  unsigned char HiSpeed;
  unsigned char TypeRaam;
  unsigned char TypeDoek;
  unsigned char TypeVent;
  unsigned char TypeKlep;
  unsigned char AlarmAfw;
  int Setpoint;
  int PositionAvg;
  TRunningMode RunningMode;
  TTimer       Timer_100ms;
  TTimer       Timer_1s;
  void *FirstMotor;
  void *LastMotor;
  struct SMotorgroup *Link;
} TMotorgroup;

//==============================================================================
//------------------------ Motorgroup - Globals --------------------------------
//==============================================================================
extern TMotorgroup Motorgroup[MAX_GROUP];

//==============================================================================
//------------------------ Motorgroup - Initialisation -------------------------
//==============================================================================
void CreateMotorgroup(TMotorgroup *pGroup, unsigned char Number);
void InitMotorgroup(TMotorgroup *pGroup);

//==============================================================================
//------------------------ Motorgroup - Position -------------------------------
//==============================================================================
//------------------------------------------------------------------------------
int MotorgroupGetTargetPosition(TMotorgroup *pGroup);
void MotorgroupSetFeedback(TMotorgroup *pGroup, int Position);

//==============================================================================
//------------------------ Motorgroup - Status ---------------------------------
//==============================================================================
//------------------------------------------------------------------------------
unsigned char MotorgroupGetEmcyBits(TMotorgroup *pGroup);

//==============================================================================
//------------------------ Motorgroup - Control --------------------------------
//==============================================================================
void ControlMotorgroup(TMotorgroup *pGroup);

#endif
