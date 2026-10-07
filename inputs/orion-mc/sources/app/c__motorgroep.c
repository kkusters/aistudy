// C__MOTORGROEP.C  

#include <stdlib.h>
#include <string.h>

#include "ch_define.h"

#ifdef CANopen
#include <co_emcy.h>
#include "ch_DS401.h"
#endif // CANopen

#include "ch_alarm.h"
#include "ch_alg.h"
#include "ch_cabriokas.h"
#include "ch_ds301.h"
#include "ch_IO.h"
#include "ch_kiersturing.h"
#include "ch_device.h"
#include "ch_motor.h"
#include "bacapp.h"
#include "ch_bacnet_object.h"
#include "ch_disp_option_0.h"
#include "ch_motorgroep.h"

#include "ch_test.h"
//#define TEST_MC

//==============================================================================
//------------------------ Motorgroup - Defines --------------------------------
//==============================================================================
#define UNDEFINED -1

//==============================================================================
//------------------------ Motorgroup - Globals --------------------------------
//==============================================================================
TMotorgroup Motorgroup[MAX_GROUP];

//==============================================================================
//------------------------ Motorgroup - Test routines --------------------------
//==============================================================================
//------------------------------------------------------------------------------
#ifdef TEST_MC
typedef struct
{
  int Position; // [%]
  int Delay;    // [s]
} THortiDemo;

static THortiDemo const HortiDemo[] =
{
  {  0, 60},{200, 80},{400, 80},{300, 60},{500, 80},{700, 80},{600, 60},{ 800, 80},{1000, 80},
  {900, 60},{700, 80},{500, 80},{600, 60},{400, 80},{200, 80},{300, 60},{ 100, 80},{  40, 60},
  {250, 80},{450, 80},{350, 60},{550, 80},{750, 80},{650, 60},{850, 80},{1000, 80},{ 100, 90}
};

static void Control_Test_MC(TMotorgroup *pGroup)
{
static int loop[MAX_GROUP]  = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
static int delay[MAX_GROUP] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

  if (delay[pGroup->Number] > 0)
    delay[pGroup->Number]--;
  else
  {
    loop[pGroup->Number]++;
    loop[pGroup->Number] %= (sizeof(HortiDemo) / sizeof(THortiDemo));

//    pGroup->Value->PositionPerc = HortiDemo[loop[nr]].Position;
    val_hr_alg.Motorgroup[pGroup->Number].PositionTime = Calc_Prop(0, 1000, 0, opt_app.Motorgroup[pGroup->Number].Runtime, HortiDemo[loop[pGroup->Number]].Position);
    delay[pGroup->Number] = HortiDemo[loop[pGroup->Number]].Delay * 10;
  }
}
#endif // TEST_MC

//==============================================================================
//------------------------ Motorgroup - Initialisation -------------------------
//==============================================================================
//------------------------------------------------------------------------------
void CreateMotorgroup(TMotorgroup *pGroup, unsigned char Number)
{
  pGroup->Number   = Number;
  pGroup->InitFlag = 1;
}

void InitMotorgroup(TMotorgroup *pGroup)
{
  TimerSet(&pGroup->Timer_100ms, TIMER_100MS);
  TimerSet(&pGroup->Timer_1s,    TIMER_1SEC );
  switch (opt_app.Motorgroup[pGroup->Number].Type)
  {
    case TYPE_RAAM:
      pGroup->TypeRaam = 1;
      pGroup->TypeDoek = 0;
      pGroup->TypeVent = 0;
      pGroup->TypeKlep = 0;
      pGroup->AlarmAfw = 1;
      break;
    case TYPE_DOEK:
      pGroup->TypeRaam = 0;
      pGroup->TypeDoek = 1;
      pGroup->TypeVent = 0;
      pGroup->TypeKlep = 0;
      pGroup->AlarmAfw = 1;
      break;
    case TYPE_VENT:
      pGroup->TypeRaam = 0;
      pGroup->TypeDoek = 0;
      pGroup->TypeVent = 1;
      pGroup->TypeKlep = 0;
      pGroup->AlarmAfw = 0;
      break;
    case TYPE_KLEP:
      pGroup->TypeRaam = 0;
      pGroup->TypeDoek = 0;
      pGroup->TypeVent = 0;
      pGroup->TypeKlep = 1;
      pGroup->AlarmAfw = 1;
      break;
  }
  pGroup->Setpoint = UNDEFINED;
  pGroup->InitFlag = 0;
}

//==============================================================================
//------------------------ Motorgroup - Options --------------------------------
//==============================================================================
//------------------------------------------------------------------------------
void GetRuntimeMotorgroup(TMotorgroup *pGroup)
{
TMotor *pMotor;
TMotor *pLink;
int MinRuntime;
int MaxRuntime;

  if (!(opt_alg.can_backbone || opt_alg.BACnet_enabled))
  {
    switch (opt_app.Motorgroup[pGroup->Number].Type)
    {
      case TYPE_RAAM:
      case TYPE_DOEK:
        if (pGroup->FirstMotor != NULL)
        {
          MinRuntime = 0;
          pMotor = pGroup->FirstMotor;
          while (pMotor != NULL)
          {
            if ((MinRuntime == 0) || (opt_app.Motor[pMotor->Number].Runtime < MinRuntime))
              MinRuntime = opt_app.Motor[pMotor->Number].Runtime;
            if ((opt_app.Motorgroup[pGroup->Number].Type == TYPE_DOEK) && (opt_app.Motorgroup[pGroup->Number].ControlType == CONTROL_DUALSCREEN) && (pMotor->Link != NULL))
            {
              pLink = pMotor->Link;
              if (opt_app.Motor[pLink->Number].Runtime < MinRuntime)
                MinRuntime = opt_app.Motor[pLink->Number].Runtime;
            }
            pMotor = pMotor->Next;
          }
          opt_app.Motorgroup[pGroup->Number].Runtime = ((long)MinRuntime * 9) / 10;
        }
        break;
    }
  }
  else if (opt_alg.BACnet_enabled)
  {
    switch (opt_app.Motorgroup[pGroup->Number].Type)
    {
      case TYPE_RAAM:
      case TYPE_DOEK:
        if (pGroup->FirstMotor != NULL)
        {
          MaxRuntime = 0;
          pMotor = pGroup->FirstMotor;
          while (pMotor != NULL)
          {
            if (opt_app.Motor[pMotor->Number].Runtime > MaxRuntime)
            {
              MaxRuntime = opt_app.Motor[pMotor->Number].Runtime;
            }

            pMotor = pMotor->Next;
          }
          opt_app.Motorgroup[pGroup->Number].Runtime = MaxRuntime;
        }
        break;
    }
  }
}

//==============================================================================
//------------------------ Motorgroup - Alarm ----------------------------------
//==============================================================================
//------------------------------------------------------------------------------
static void CheckMotorgroupManualAlarm(TMotorgroup *pGroup)
{
  if (val_hr_alg.Motorgroup[pGroup->Number].OperationMode == omManual)
  {
    CreateAlarm(&alarm_hr_alg.Motorgroup[pGroup->Number].Manual, MOTORGROUP_MANUAL_AL, pGroup->Number, 0, pGroup->Number + 1, &alarm_hr_alg.Motorgroup[pGroup->Number].AlarmHard);
    if ((setp_alg.Motorgroup[pGroup->Number].DelayAlarmManual == 0) || (TimeAlarmOn(MOTORGROUP_MANUAL_AL, pGroup->Number) < (setp_alg.Motorgroup[pGroup->Number].DelayAlarmManual * 60)))
      alarm_hr_alg.Motorgroup[pGroup->Number].AlarmHard = 0;
    else
      alarm_hr_alg.Motorgroup[pGroup->Number].AlarmHard = 1;
  }
  else
    ClearAlarm(&alarm_hr_alg.Motorgroup[pGroup->Number].Manual, MOTORGROUP_MANUAL_AL, pGroup->Number);
}

//------------------------------------------------------------------------------
unsigned char MotorgroupGetEmcyBits(TMotorgroup *pGroup)
{
  unsigned char Emcy = 0;
  TMotor *pMotor;
  TDevice *pDevice;

  if (alarm_hr_alg.Motorgroup[pGroup->Number].Manual)
    Emcy |= MOTOR_EMCY_BIT_3;

  switch (opt_app.Motorgroup[pGroup->Number].Type)
  {
    case TYPE_RAAM:
    case TYPE_DOEK:
      pMotor = pGroup->FirstMotor;
      while (pMotor != NULL)
      {
        Emcy |= MotorGetEmcyBits(pMotor);
        pMotor = pMotor->Next;
      }
      break;
    case TYPE_VENT:
      pDevice = pGroup->FirstMotor;
      while (pDevice != NULL)
      {
        Emcy |= DeviceGetEmcyBits(pDevice);
        pDevice = pDevice->Next;
      }
      break;
  }

  return (Emcy);
}

//------------------------------------------------------------------------------
static void ControlAlarmMotorgroup(TMotorgroup *pGroup)
{
TDevice *pDevice;
unsigned char NrAlarms = 0;

  CheckMotorgroupManualAlarm(pGroup);

  switch (opt_app.Motorgroup[pGroup->Number].Type)
  {
    case TYPE_VENT:
    case TYPE_KLEP:
      pDevice = pGroup->FirstMotor;
      while (pDevice != NULL)
      {
        if (ControlAlarmDevice(pDevice))
          NrAlarms++;
        pDevice = pDevice->Next;
      }
      break;
  }

  // Set IO
  if (AlarmHardGroup(pGroup->Number + 1))
    IO_Set_Dig_Out(&opt_app.Motorgroup[pGroup->Number].DigOutAlarm, 0);
  else
    IO_Set_Dig_Out(&opt_app.Motorgroup[pGroup->Number].DigOutAlarm, 1);

  if (NrAlarms >= setp_alg.Ventgroup[pGroup->Number].AlarmUrgent)
    IO_Set_Dig_Out(&opt_app.Ventgroup[pGroup->Number].DigOutAlarmUrgent, 0);
  else
    IO_Set_Dig_Out(&opt_app.Ventgroup[pGroup->Number].DigOutAlarmUrgent, 1);

#ifdef CANopen
  if (opt_alg.can_backbone)
  {
    unsigned char EmcyBits;
    int i;

    EmcyBits = MotorgroupGetEmcyBits(pGroup);

    for (i = 0; i < 5; i++)
    {
      DS401_SetDigitalInput((opt_app.Motorgroup[pGroup->Number].Number * 5) + i, EmcyBits & 0x01);
      EmcyBits >>= 1;
    }
  }
#endif // CANopen
}

//------------------------------------------------------------------------------
void ResetAlarmMotorgroup(TMotorgroup *pGroup)
{
  ClearAlarm(&alarm_hr_alg.Motorgroup[pGroup->Number].Manual, MOTORGROUP_MANUAL_AL, pGroup->Number);
}

//==============================================================================
//------------------------ Motorgroup - Position -------------------------------
//==============================================================================
//------------------------------------------------------------------------------
int MotorgroupGetTargetPosition(TMotorgroup *pGroup)
{
  switch (val_hr_alg.Motorgroup[pGroup->Number].OperationMode)
  {
    case omAuto:
      if (opt_alg.can_backbone)
      {
        int bin;
        if (DS401_GetAnalogOutput(pGroup->Number, &bin) == CO_OK)
          val_hr_alg.Motorgroup[pGroup->Number].PositionPerc = Calc_Perc(bin, 0x7FFF, 1000);
      }
      else if (opt_alg.BACnet_enabled)
      {
        if (Bacnet_Object_Has_Valid_Value(pGroup->Number, POSITION_CONTROLLER_DESIRED_POSITION))
          val_hr_alg.Motorgroup[pGroup->Number].PositionPerc = *(int *)Get_Bacnet_Object_Value(pGroup->Number, POSITION_CONTROLLER_DESIRED_POSITION);
      }
	  else if (opt_alg.hoogendoorn_enabled)
	  {
	    if (pGroup->Setpoint != UNDEFINED)
	      val_hr_alg.Motorgroup[pGroup->Number].PositionPerc = pGroup->Setpoint;
	  }
      else if (opt_app.Motorgroup[pGroup->Number].AnalogInput)
      {
        val_hr_alg.Motorgroup[pGroup->Number].PositionPerc = IO_Get_Ana_In_One(&opt_app.Motorgroup[pGroup->Number].AnaInPosition);
        switch (opt_app.Motorgroup[pGroup->Number].Type)
        {
          case TYPE_VENT:
          case TYPE_KLEP:
            val_hr_alg.Motorgroup[pGroup->Number].PositionPerc *= 10;
            break;
        }
      }
      else
      {
        val_hr_alg.Motorgroup[pGroup->Number].PositionPerc = Calc_Perc_Abs(val_hr_alg.Motorgroup[pGroup->Number].PositionTime, opt_app.Motorgroup[pGroup->Number].Runtime, 1000);
      }
      break;
    case omManual:
      pGroup->Delay = 0;
      break;
    case omOff:
      val_hr_alg.Motorgroup[pGroup->Number].PositionPerc = 0;
      pGroup->Delay = 0;
      break;
  }
  return (val_hr_alg.Motorgroup[pGroup->Number].PositionPerc);
}

//------------------------------------------------------------------------------
void MotorgroupSetFeedback(TMotorgroup *pGroup, int Position)
{
BACNET_APPLICATION_DATA_VALUE bacnet_value;
unsigned char EnableFeedback = 0;
TMotor *pMotor;

  // ---------------------- CANopen ----------------------
  if (opt_alg.can_backbone)
  {
    DS401_SetAnalogInput(pGroup->Number, Calc_Prop(0, 1000, 0x0000, 0x7FFF, Position));
  }
  // ---------------------- BACnet -----------------------
  else if (opt_alg.BACnet_enabled)
  {
    bacnet_value.tag       = BACNET_APPLICATION_TAG_REAL;
    bacnet_value.type.Real = Position;
    Set_Bacnet_Object_Value(pGroup->Number, POSITION_CONTROLLER_CURRENT_POSITION, &bacnet_value);
  }
  // ---------------------- Analog -----------------------
  else
  {
    if (opt_app.Motorgroup[pGroup->Number].AlarmAnaloog)
    {
      pMotor = pGroup->FirstMotor;
      while (pMotor != NULL)
      {
        if ((val_hr_alg.Motor[pMotor->Number].OperationMode == omAuto) && (val_hr_alg.Motor[pMotor->Number].Overruled == 0) && !AlarmHardGroup(pGroup->Number + 1))
        {
          EnableFeedback = 1;
          break;
        }
        pMotor = pMotor->Next;
      }
    }
    else
      EnableFeedback = 1;

    if (EnableFeedback)
    {
      if (opt_app.Motorgroup[pGroup->Number].DigitalInput && (val_hr_alg.Motorgroup[pGroup->Number].OperationMode == omAuto))
        IO_Set_Ana_Out(&opt_app.Motorgroup[pGroup->Number].AnaOutPosition, Calc_Perc_Abs(val_hr_alg.Motorgroup[pGroup->Number].PositionTime, opt_app.Motorgroup[pGroup->Number].Runtime, 1000));
      else
        IO_Set_Ana_Out(&opt_app.Motorgroup[pGroup->Number].AnaOutPosition, Position);
    }
    else
    {
      IO_Set_Ana_Out(&opt_app.Motorgroup[pGroup->Number].AnaOutPosition, 500);
    }
  }
}

//==============================================================================
//------------------------ Motorgroup - Control --------------------------------
//==============================================================================
//------------------------------------------------------------------------------
static void SetPositionAllMotors(TMotorgroup *pGroup)
{
TMotor *pMotor;

  if (pGroup->Delay == 0)
  {
    pMotor = pGroup->FirstMotor;
    while (pMotor != NULL)
    {
      val_hr_alg.Motor[pMotor->Number].PositionAuto = val_hr_alg.Motorgroup[pGroup->Number].PositionPerc;
      pMotor = pMotor->Next;
    }
  }
}


/*	
static void SetPositionAllMotors(TMotorgroup *pGroup)
{
TMotor *pMotor;

  if (pGroup->Delay == 0)
  {
    if(pGroup->OptDelay)
	{
	  int MotorCnt = 0;
      if(pGroup->DelayTimer)
        pGroup->DelayTimer--;
      else
	  {
	    int CntMotor = 0;
	    pGroup->DelayTimer = pGroup->OptDelay;

        pMotor = pGroup->FirstMotor;
        while (pMotor != NULL)
        {
	      if(MotorCnt == pGroup->Cnt)
	      {
            val_hr_alg.Motor[pMotor->Number].PositionAuto = val_hr_alg.Motorgroup[pGroup->Number].PositionPerc;
          }
          pMotor = pMotor->Next;
	   	  MotorCnt++;
        }

	    pGroup->Cnt++;
	    if(pGroup->Cnt >= MotorCnt)
	      pGroup->Cnt = 0;
	  }
	}
	else
	{
      pMotor = pGroup->FirstMotor;
      while (pMotor != NULL)
      {
        val_hr_alg.Motor[pMotor->Number].PositionAuto = val_hr_alg.Motorgroup[pGroup->Number].PositionPerc;
        pMotor = pMotor->Next;
      }
	}
  }
}

*/

//------------------------------------------------------------------------------
static void GetAveragePosition(TMotorgroup *pGroup)
{
TMotor *pMotor;
TDevice  *pDevice;
int Target;
int Actual;
int ActualValue;
int Diff = 0;
int Offset;

  // Zoek de maximale afwijking
  switch (opt_app.Motorgroup[pGroup->Number].Type)
  {
    case TYPE_RAAM:
    case TYPE_DOEK:
      Target = val_hr_alg.Motorgroup[pGroup->Number].PositionPerc;
      Actual = val_hr_alg.Motorgroup[pGroup->Number].PositionPerc;
      pMotor = pGroup->FirstMotor;
      while (pMotor)
      {
        if (val_hr_alg.Motor[pMotor->Number].OperationMode != omOff)
        {
          if (abs_int(Target - val_hr_alg.Motor[pMotor->Number].Position) > Diff)
          {
            Diff   = abs_int(Target - val_hr_alg.Motor[pMotor->Number].Position);
            Actual = val_hr_alg.Motor[pMotor->Number].Position;
          }
        }
        pMotor = pMotor->Next;
      }
      pGroup->PositionAvg = Actual;
      break;
    case TYPE_VENT:
      Target = val_hr_alg.Motorgroup[pGroup->Number].PositionPerc;
      Actual = val_hr_alg.Motorgroup[pGroup->Number].PositionPerc;
      pDevice = pGroup->FirstMotor;
      while (pDevice)
      {
        int TargetValue;
        if (val_hr_alg.Device[pDevice->Number].OperationMode != omOff)
        {
          ActualValue = Calc_Prop(0, opt_app.Motorgroup[pGroup->Number].VentAtMax, 0, 1000, val_hr_alg.Device[pDevice->Number].ActualValue); 
          if ((val_hr_alg.Device[pDevice->Number].OperationMode == omAuto) && (setp_alg.Device[pDevice->Number].Offset != 0))
          {
            TargetValue = Calc_Prop(0, opt_app.Motorgroup[pGroup->Number].VentAtMax, 0, 1000, val_hr_alg.Device[pDevice->Number].TargetValue); 
            Offset = TargetValue - Target;
            ActualValue -= Offset;
            if (ActualValue < 0)
              ActualValue = 0;
          }

          if (abs_int(Target - ActualValue) > Diff)
          {
            Diff   = abs_int(Target - ActualValue);
            Actual = ActualValue;
          }
        }
        pDevice = pDevice->Next;
      }
      pGroup->PositionAvg = Actual;
      break;
    case TYPE_KLEP:
      Target = val_hr_alg.Motorgroup[pGroup->Number].PositionPerc;
      Actual = val_hr_alg.Motorgroup[pGroup->Number].PositionPerc;
      pDevice = pGroup->FirstMotor;
      while (pDevice)
      {
        if (val_hr_alg.Device[pDevice->Number].OperationMode != omOff)
        {
          ActualValue = val_hr_alg.Device[pDevice->Number].ActualValue;

          if (abs_int(Target - (ActualValue * 10)) > Diff)
          {
            Diff   = abs_int(Target - (ActualValue * 10));
            Actual = ActualValue * 10;
          }
        }
        pDevice = pDevice->Next;
      }

      pGroup->PositionAvg = Actual;
      break;
  }
}

//------------------------------------------------------------------------------
static void ControlHiSpeed(TMotorgroup *pGroup)
{
TMotor *pMotor;
unsigned char HiSpeed = 0;
int Diff = 0;

  if (opt_app.Motorgroup[pGroup->Number].FrequencyControlled)
  {
//    #ifdef CANopen
//    if (opt_alg.can_backbone)
//      DS401_GetDigitalOutput(pGroup->Number * 2, &pGroup->HiSpeed);
//    else
//    #endif // CANopen
      pGroup->HiSpeed = IO_Get_Dig_In_Status(&opt_app.Motorgroup[pGroup->Number].DigInHiSpeed);

    pMotor = pGroup->FirstMotor;
    while (pMotor != NULL)
    {
      if (opt_alg.can_backbone || opt_alg.BACnet_enabled)
      {
        switch (val_hr_alg.Motor[pMotor->Number].OperationMode)
        {
          case omAuto:   Diff = abs_int(val_hr_alg.Motor[pMotor->Number].PositionAuto   - val_hr_alg.Motor[pMotor->Number].Position); break;
          case omManual: Diff = abs_int(val_hr_alg.Motor[pMotor->Number].PositionManual - val_hr_alg.Motor[pMotor->Number].Position); break;
          case omOff:    Diff = val_hr_alg.Motor[pMotor->Number].Position; break;
        }
        if (Diff > opt_app.Motorgroup[pGroup->Number].FrequentieVerstel)
          pMotor->Flags.HiSpeed = 1;
        else if (Diff < opt_app.Motorgroup[pGroup->Number].PositionLowSpeed)
          pMotor->Flags.HiSpeed = 0;
      }
      else
      {
        pMotor->Flags.HiSpeed = pGroup->HiSpeed;
      }

      if ((pMotor->Flags.HiSpeed) && ((opt_app.Motorgroup[pGroup->Number].Type == TYPE_DOEK) || (pMotor->RunningMode == rmClose)))
        IO_Set_Motor_Control_HiSpeed(&opt_app.Motor[pMotor->Number].IO, 1);
      else
        IO_Set_Motor_Control_HiSpeed(&opt_app.Motor[pMotor->Number].IO, 0);

      if (IO_Get_Motor_Control_HiSpeed(&opt_app.Motor[pMotor->Number].IO))
        HiSpeed = 1;
      pMotor = pMotor->Next;
    }
  }
  else
  {
    pGroup->HiSpeed = 0;
  }
}

//------------------------------------------------------------------------------
static void GetRunningModeMotorgroup(TMotorgroup *pGroup)
{
TRunningMode PreviousMode;

  if (opt_app.Motorgroup[pGroup->Number].DigitalInput)
  {
    if (pGroup->Delay > 0)
      pGroup->Delay--;

    PreviousMode = pGroup->RunningMode;
    if (IO_Get_Dig_In_Status(&opt_app.Motorgroup[pGroup->Number].DigInOpen))
    {
      pGroup->RunningMode = rmOpen;
      if (PreviousMode != rmOpen)
        pGroup->Delay = MOTOR_DELAY;
    }
    else if (IO_Get_Dig_In_Status(&opt_app.Motorgroup[pGroup->Number].DigInClose))
    {
      pGroup->RunningMode = rmClose;
      if (PreviousMode != rmClose)
        pGroup->Delay = MOTOR_DELAY;
    }
    else
      pGroup->RunningMode = rmStop;
  }
  else
  {
    pGroup->Delay = 0;
    pGroup->RunningMode = rmStop;
  }
}

//------------------------------------------------------------------------------
static void ControlVirtualMotor(TMotorgroup *pGroup)
{
  if (opt_app.Motorgroup[pGroup->Number].DigitalInput)
  {
    switch (opt_app.Motorgroup[pGroup->Number].Type)
    {
      case TYPE_RAAM:
        if (pGroup->RunningMode == rmOpen)
        {
          val_hr_alg.Motorgroup[pGroup->Number].PositionTime++;
        }
        else if (pGroup->RunningMode == rmClose)
        {
          if (pGroup->HiSpeed && (val_hr_alg.Motorgroup[pGroup->Number].PositionPerc > opt_app.Motorgroup[pGroup->Number].PositionLowSpeed))
            val_hr_alg.Motorgroup[pGroup->Number].PositionTime -= 4;
          else
            val_hr_alg.Motorgroup[pGroup->Number].PositionTime--;
        }
        break;
      case TYPE_DOEK:
        if (pGroup->RunningMode == rmOpen)
        {
          if (pGroup->HiSpeed && (val_hr_alg.Motorgroup[pGroup->Number].PositionPerc > opt_app.Motorgroup[pGroup->Number].PositionLowSpeed))
            val_hr_alg.Motorgroup[pGroup->Number].PositionTime -= 4;
          else
            val_hr_alg.Motorgroup[pGroup->Number].PositionTime--;
        }
        else if (pGroup->RunningMode == rmClose)
        {
          if (pGroup->HiSpeed && (val_hr_alg.Motorgroup[pGroup->Number].PositionPerc < (1000 - opt_app.Motorgroup[pGroup->Number].PositionLowSpeed)))
            val_hr_alg.Motorgroup[pGroup->Number].PositionTime += 4;
          else
            val_hr_alg.Motorgroup[pGroup->Number].PositionTime++;
        }
        break;
      case TYPE_VENT:
      case TYPE_KLEP:
        if (pGroup->RunningMode == rmOpen)
          val_hr_alg.Motorgroup[pGroup->Number].PositionTime++;
        else if (pGroup->RunningMode == rmClose)
          val_hr_alg.Motorgroup[pGroup->Number].PositionTime--;
        break;
    }
    if (val_hr_alg.Motorgroup[pGroup->Number].PositionTime < 0)
      val_hr_alg.Motorgroup[pGroup->Number].PositionTime = 0;
    if (val_hr_alg.Motorgroup[pGroup->Number].PositionTime > opt_app.Motorgroup[pGroup->Number].Runtime)
      val_hr_alg.Motorgroup[pGroup->Number].PositionTime = opt_app.Motorgroup[pGroup->Number].Runtime;
  }
}

//==============================================================================
//------------------------ Motorgroup - Raamsturing ----------------------------
//==============================================================================
//------------------------------------------------------------------------------
static void ControlWindow(TMotorgroup *pGroup)
{
TMotor *pMotor;

  MotorgroupGetTargetPosition(pGroup);
  MotorgroupSetFeedback(pGroup, pGroup->PositionAvg);

  // Set all group members
  SetPositionAllMotors(pGroup);

  // Check if motor has to be hold
  pMotor = pGroup->FirstMotor;
  while (pMotor != NULL)
  {
    if (pMotor->Flags.HoldOpen)
      SetMotorHold(pMotor, rmOpen);
    else
      SetMotorRelease(pMotor, rmOpen);
    
    if (pMotor->Flags.HoldClose)
      SetMotorHold(pMotor, rmClose);
    else
      SetMotorRelease(pMotor, rmClose);

    pMotor = pMotor->Next;
  }
}

//==============================================================================
//------------------------ Motorgroup - Doeksturing ----------------------------
//==============================================================================
//------------------------------------------------------------------------------
static void ControlKiersturing(TMotorgroup *pGroup)
{
TMotor *pMotor;

  pMotor = pGroup->FirstMotor;
  while (pMotor != NULL)
  {
    if ((val_hr_alg.Motor[pMotor->Number].PositionAuto == 1000) && (pMotor->Flags.LimitClose == 0))
      val_hr_alg.Motor[pMotor->Number].PositionAuto = 1000;
    else if (pGroup->Delay == 0)
      val_hr_alg.Motor[pMotor->Number].PositionAuto = val_hr_alg.Motorgroup[pGroup->Number].PositionPerc;
    pMotor = pMotor->Next;
  }
}

//------------------------------------------------------------------------------
static void ControlScreen(TMotorgroup *pGroup)
{
TMotor *pMotor;

  MotorgroupGetTargetPosition(pGroup);
  MotorgroupSetFeedback(pGroup, pGroup->PositionAvg);

  // Set all group members
  if (opt_app.Motorgroup[pGroup->Number].KierRegeling && (val_hr_alg.Motorgroup[pGroup->Number].OperationMode == omAuto))
    ControlKiersturing(pGroup);
  else
    SetPositionAllMotors(pGroup);

  // Check if motor has to be hold
  pMotor = pGroup->FirstMotor;
  while (pMotor != NULL)
  {
    if (pMotor->Flags.HoldOpen)
      SetMotorHold(pMotor, rmOpen);
    else
      SetMotorRelease(pMotor, rmOpen);
    
    if (pMotor->Flags.HoldClose)
      SetMotorHold(pMotor, rmClose);
    else
      SetMotorRelease(pMotor, rmClose);

    pMotor = pMotor->Next;
  }
}

//==============================================================================
//------------------------ Motorgroup - Ventilatorsturing ---------------------
//==============================================================================
//------------------------------------------------------------------------------
static void SetPositionAllVents(TMotorgroup *pGroup)
{
TDevice *pDevice;
int TargetValue;

  pDevice = pGroup->FirstMotor;
  while (pDevice != NULL)
  {
    TargetValue = val_hr_alg.Motorgroup[pGroup->Number].PositionPerc;
    if (TargetValue < 10L * setp_alg.Ventgroup[pGroup->Number].MinVent)
      TargetValue = 10L * setp_alg.Ventgroup[pGroup->Number].MinVent;
    if (TargetValue > 10L * setp_alg.Ventgroup[pGroup->Number].MaxVent)
      TargetValue = 10L * setp_alg.Ventgroup[pGroup->Number].MaxVent;
    SetTargetValueDevice(pDevice, Calc_Prop(0, 1000, 0, opt_app.Motorgroup[pGroup->Number].VentAtMax, TargetValue), opt_app.Ventgroup[pGroup->Number].RS485Bus, 4);
    pDevice = pDevice->Next;
  }
}

//------------------------------------------------------------------------------
static void VentSetFeedback(TMotorgroup *pGroup)
{
BACNET_APPLICATION_DATA_VALUE bacnet_value;

  // ---------------------- CANopen ----------------------
  if (opt_alg.can_backbone)
  {
    DS401_SetAnalogInput(pGroup->Number, Calc_Prop(0, 1000, 0x0000, 0x7FFF, pGroup->PositionAvg));
  }
  // ---------------------- BACnet -----------------------
  else if (opt_alg.BACnet_enabled)
  {
    bacnet_value.tag       = BACNET_APPLICATION_TAG_REAL;
    bacnet_value.type.Real = pGroup->PositionAvg;
    Set_Bacnet_Object_Value(pGroup->Number, POSITION_CONTROLLER_CURRENT_POSITION, &bacnet_value);
  }
  // ---------------------- Analog -----------------------
  else
  {
    switch (val_hr_alg.Motorgroup[pGroup->Number].OperationMode)
    {
      case omAuto:
        if (opt_app.Ventgroup[pGroup->Number].DigInOnOff.board_type && !IO_Get_Dig_In_Status(&opt_app.Ventgroup[pGroup->Number].DigInOnOff))
          IO_Set_Ana_Out(&opt_app.Motorgroup[pGroup->Number].AnaOutPosition, 0);
        else if (opt_app.Motorgroup[pGroup->Number].AnalogInput)
          IO_Set_Ana_Out(&opt_app.Motorgroup[pGroup->Number].AnaOutPosition, pGroup->PositionAvg / 10);
        else
          IO_Set_Ana_Out(&opt_app.Motorgroup[pGroup->Number].AnaOutPosition, Calc_Perc_Abs(val_hr_alg.Motorgroup[pGroup->Number].PositionTime, opt_app.Motorgroup[pGroup->Number].Runtime, 100));
        break;
      case omManual:
      case omOff:
        IO_Set_Ana_Out(&opt_app.Motorgroup[pGroup->Number].AnaOutPosition, pGroup->PositionAvg / 10);
        break;
    }
  }
}

//------------------------------------------------------------------------------
static void ControlVent(TMotorgroup *pGroup)
{
  MotorgroupGetTargetPosition(pGroup);
  VentSetFeedback(pGroup);

  switch (val_hr_alg.Motorgroup[pGroup->Number].OperationMode)
  {
    case omAuto:
      if (opt_app.Ventgroup[pGroup->Number].DigInOnOff.board_type && !IO_Get_Dig_In_Status(&opt_app.Ventgroup[pGroup->Number].DigInOnOff))
        val_hr_alg.Motorgroup[pGroup->Number].PositionPerc = 0;
      SetPositionAllVents(pGroup);
      break;
    case omManual:
      SetPositionAllVents(pGroup);
      break;
    case omOff:
      SetPositionAllVents(pGroup);
      break;
  }
}

//==============================================================================
//------------------------ Motorgroup - Klepsturing ----------------------------
//==============================================================================
//------------------------------------------------------------------------------
static void SetPositionAllFlaps(TMotorgroup *pGroup)
{
TDevice *pDevice;

  pDevice = pGroup->FirstMotor;
  while (pDevice != NULL)
  {
    SetTargetValueDevice(pDevice, (val_hr_alg.Motorgroup[pGroup->Number].PositionPerc + 5) / 10, opt_app.Ventgroup[pGroup->Number].RS485Bus, 4);
    pDevice = pDevice->Next;
  }
}

//------------------------------------------------------------------------------
static void FlapSetFeedback(TMotorgroup *pGroup)
{
BACNET_APPLICATION_DATA_VALUE bacnet_value;

  // ---------------------- CANopen ----------------------
  if (opt_alg.can_backbone)
  {
    DS401_SetAnalogInput(pGroup->Number, Calc_Prop(0, 1000, 0x0000, 0x7FFF, pGroup->PositionAvg));
  }
  // ---------------------- BACnet -----------------------
  else if (opt_alg.BACnet_enabled)
  {
    bacnet_value.tag       = BACNET_APPLICATION_TAG_REAL;
    bacnet_value.type.Real = pGroup->PositionAvg;
    Set_Bacnet_Object_Value(pGroup->Number, POSITION_CONTROLLER_CURRENT_POSITION, &bacnet_value);
  }
  // ---------------------- Analog -----------------------
  else
  {
    switch (val_hr_alg.Motorgroup[pGroup->Number].OperationMode)
    {
      case omAuto:
        if (opt_app.Motorgroup[pGroup->Number].AnalogInput)
          IO_Set_Ana_Out(&opt_app.Motorgroup[pGroup->Number].AnaOutPosition, pGroup->PositionAvg / 10);
        else
          IO_Set_Ana_Out(&opt_app.Motorgroup[pGroup->Number].AnaOutPosition, Calc_Perc_Abs(val_hr_alg.Motorgroup[pGroup->Number].PositionTime, opt_app.Motorgroup[pGroup->Number].Runtime, 100));
        break;
      case omManual:
      case omOff:
        IO_Set_Ana_Out(&opt_app.Motorgroup[pGroup->Number].AnaOutPosition, pGroup->PositionAvg / 10);
        break;
    }
  }
}

//------------------------------------------------------------------------------
static void CheckFlapExternalSensor(TMotorgroup *pGroup)
{
  switch (opt_app.Motorgroup[pGroup->Number].SensorType)
  {
    case SENSOR_TYPE_EINDSCHAKELAAR:
      if (AlarmActiveGroup(pGroup->Number + 1, KLEP_LIMITSWITCH_AL))
        IO_Set_Dig_Out(&opt_app.Motorgroup[pGroup->Number].DigOutAlarmFlap, 0);
      else
        IO_Set_Dig_Out(&opt_app.Motorgroup[pGroup->Number].DigOutAlarmFlap, 1);
      break;
    case SENSOR_TYPE_EXTERN_ALARM:
      if (AlarmActiveGroup(pGroup->Number + 1, KLEP_EXTERN_ALARM_AL))
        IO_Set_Dig_Out(&opt_app.Motorgroup[pGroup->Number].DigOutAlarmFlap, 0);
      else
        IO_Set_Dig_Out(&opt_app.Motorgroup[pGroup->Number].DigOutAlarmFlap, 1);
      break;
  }
}

//------------------------------------------------------------------------------
static void ControlKlep(TMotorgroup *pGroup)
{
  MotorgroupGetTargetPosition(pGroup);
  FlapSetFeedback(pGroup);
  SetPositionAllFlaps(pGroup);
  CheckFlapExternalSensor(pGroup);
}

//==============================================================================
//------------------------ Motorgroup -----------------------------------------
//==============================================================================
//------------------------------------------------------------------------------
OPERATION_MODE Get_Motorgroup_Operation_Mode(TMotorgroup *pGroup)
{
  OPERATION_MODE return_value = OM_NORMAL_OPERATION;
  OPERATION_MODE motor_value  = OM_NORMAL_OPERATION;
  int motor_states[7] = {0,0,0,0,0,0,0};
  TMotor * pMotor = (TMotor *)pGroup->FirstMotor;
  
  if (motor_states[OM_NOT_AVAILABLE] || !opt_app.Motorgroup[pGroup->Number].Enabled)
    return OM_NOT_AVAILABLE;

  if (val_hr_alg.Motorgroup[pGroup->Number].OperationMode == omOff)
    return OM_OUT_OF_ORDER;

  while (pMotor != NULL)
  {
    motor_value = Get_Motor_Operation_Mode(pMotor);
    motor_states[motor_value] = 1;
    pMotor = pMotor->Next;
  }
  
  if (motor_states[OM_OUT_OF_ORDER])
    return OM_OUT_OF_ORDER;
  
  if (motor_states[OM_IN_MAINTENANCE] || install_flag)
    return OM_IN_MAINTENANCE;

  if (motor_states[OM_MANUAL] || val_hr_alg.Motorgroup[pGroup->Number].OperationMode == omManual)
    return OM_MANUAL;

  if (motor_states[OM_FAULT])
    return OM_FAULT;

  return OM_NORMAL_OPERATION;
}

//------------------------------------------------------------------------------
OPERATION_STATE Get_Motorgroup_Operation_State(TMotorgroup *pGroup)
{
  OPERATION_STATE return_value = OS_AT_POSITION;
  OPERATION_STATE motor_value  = OS_AT_POSITION;
  int motor_states[9] = {0,0,0,0,0,0,0,0,0};
  TMotor * pMotor = (TMotor *)pGroup->FirstMotor;
  int motor_aantal = 0;

  while (pMotor != NULL)
  {
    motor_value = Get_Motor_Operation_State(pMotor);
    motor_states[motor_value]++;
    motor_aantal++;
    pMotor = pMotor->Next;
  }

  if (motor_states[OS_GOING_DOWN])
  {
    if (Motorgroup[pGroup->Number].TypeDoek)
      return_value = OS_GOING_UP;
    else
      return_value = OS_GOING_DOWN;
    pGroup->RunningMode = rmClose;
  }
  
  if (motor_states[OS_GOING_UP])
  {
    if (Motorgroup[pGroup->Number].TypeDoek)
      return_value = OS_GOING_DOWN;
    else
      return_value = OS_GOING_UP;
    pGroup->RunningMode = rmOpen;
  }

  if (motor_states[OS_AT_LOW_LIMIT] == motor_aantal)
  {
    if (Motorgroup[pGroup->Number].TypeDoek)
      return_value = OS_AT_HIGH_LIMIT;
    else
      return_value = OS_AT_LOW_LIMIT;
  }
  
  if (motor_states[OS_AT_HIGH_LIMIT] == motor_aantal)
  {
    if (Motorgroup[pGroup->Number].TypeDoek)
      return_value = OS_AT_LOW_LIMIT;
    else
      return_value = OS_AT_HIGH_LIMIT;
  }

  if (motor_states[OS_HOLDING])
    return_value = OS_HOLDING;

  if (motor_states[OS_RESTARTING])
    return_value = OS_RESTARTING;

  if (motor_states[OS_HELD] == motor_aantal)
    return_value = OS_HELD;
  
  return return_value;
}

//------------------------------------------------------------------------------
void ControlHoldPosition(TMotorgroup *pGroup)
{
  bool holdPosition = false;
  TMotor *pMotor = (TMotor *)pGroup->FirstMotor;

  if (Bacnet_Object_Has_Valid_Value(pGroup->Number, POSITION_CONTROLLER_HOLD_POSITION))
  {
    holdPosition = *(bool *)Get_Bacnet_Object_Value(pGroup->Number, POSITION_CONTROLLER_HOLD_POSITION);
    
    while (pMotor)
    {
  /*    if (holdPosition)
      {
        pMotor->Flags.HoldOpen = 1;
        pMotor->Flags.HoldClose = 1;
      }
      else		*/
      {
        pMotor->Flags.HoldOpen = 0;
        pMotor->Flags.HoldClose = 0;
      }
      pMotor = (TMotor*)pMotor->Next;
    }
  }
}

//------------------------------------------------------------------------------
void ControlMotorgroup(TMotorgroup *pGroup)
{
  BACNET_APPLICATION_DATA_VALUE bacnet_value;

  if (opt_app.Motorgroup[pGroup->Number].Enabled)
  {
    if (pGroup->InitFlag)
      InitMotorgroup(pGroup);
    if (TimerExpired(&pGroup->Timer_100ms))
    {
      ControlHiSpeed(pGroup);
      GetRunningModeMotorgroup(pGroup);
      ControlVirtualMotor(pGroup);

      if (TimerExpired(&pGroup->Timer_1s))
      {
        TimerReset(&pGroup->Timer_1s);
        ControlAlarmMotorgroup(pGroup);
        GetAveragePosition(pGroup);
        GetRuntimeMotorgroup(pGroup);
      }

      #ifdef TEST_MC
      Control_Test_MC(pGroup);
      #endif // TEST_MC

      switch (opt_app.Motorgroup[pGroup->Number].Type)
      {
        case TYPE_RAAM:
          if (opt_alg.BACnet_enabled)
            ControlHoldPosition(pGroup);
          switch (opt_app.Motorgroup[pGroup->Number].ControlType)
          {
            case CONTROL_NORMAL:
              ControlWindow(pGroup);
              break;
            case CONTROL_CABRIOKAS:
              ControlCabriokas(pGroup);
              break;
          }
          break;
        case TYPE_DOEK:
          if (opt_alg.BACnet_enabled)
            ControlHoldPosition(pGroup);
          switch (opt_app.Motorgroup[pGroup->Number].ControlType)
          {
            case CONTROL_NORMAL:
              ControlScreen(pGroup);
              break;
            case CONTROL_DUALSCREEN:
              ControlDualScreen(pGroup);
              break;
          }
          break;
        case TYPE_VENT:
          ControlVent(pGroup);
          break;
        case TYPE_KLEP:
          ControlKlep(pGroup);
          break;
      }

      TimerReset(&pGroup->Timer_100ms);
      pGroup->Disabled = 0;

      if (opt_alg.BACnet_enabled && (opt_app.Motorgroup[pGroup->Number].Type != TYPE_VENT))
      {
        bacnet_value.tag = BACNET_APPLICATION_TAG_UNSIGNED_INT;
        bacnet_value.type.Unsigned_Int = Get_Motorgroup_Operation_Mode(pGroup);
        Set_Bacnet_Object_Value(pGroup->Number, POSITION_CONTROLLER_OPERATION_MODE, &bacnet_value);
        
        bacnet_value.tag = BACNET_APPLICATION_TAG_UNSIGNED_INT;
        bacnet_value.type.Unsigned_Int = Get_Motorgroup_Operation_State(pGroup);
        Set_Bacnet_Object_Value(pGroup->Number, POSITION_CONTROLLER_OPERATION_STATE, &bacnet_value);
      }
    }
  }
  else if (pGroup->Disabled == 0)
  {
    ResetAlarmMotorgroup(pGroup);
    pGroup->Disabled = 1;

    if (opt_alg.BACnet_enabled && (opt_app.Motorgroup[pGroup->Number].Type != TYPE_VENT))
    {
      bacnet_value.tag = BACNET_APPLICATION_TAG_UNSIGNED_INT;
      bacnet_value.type.Unsigned_Int = Get_Motorgroup_Operation_Mode(pGroup);
      Set_Bacnet_Object_Value(pGroup->Number, POSITION_CONTROLLER_OPERATION_MODE, &bacnet_value);
      
      bacnet_value.tag = BACNET_APPLICATION_TAG_UNSIGNED_INT;
      bacnet_value.type.Unsigned_Int = Get_Motorgroup_Operation_State(pGroup);
      Set_Bacnet_Object_Value(pGroup->Number, POSITION_CONTROLLER_OPERATION_STATE, &bacnet_value);
    }
  }
}

