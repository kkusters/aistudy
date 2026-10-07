// C__MOTORGROEP.C  

#include <string.h>

#include "ch_define.h"

#include "ch_alarm.h"
#include "ch_alg.h"
#include "ch_DS301.h"
#include "ch_DS401.h"
#include "ch_IO.h"
#include "ch_motor.h"
#include "ch_motorgroep.h"
#include "bacapp.h"
#include "ch_bacnet_object.h"
#include "ch_kiersturing.h"

static void SetGroupCombiMatic(TMotorgroup *pGroup);
static void SetGroupDualScreen(TMotorgroup *pGroup);
static void SetMotorsCombiMatic(TMotorgroup *pGroup);
static void SetMotorsDualScreen(TMotorgroup *pGroup);

TDualScreen DualScreen[MAX_SCREEN];

//==============================================================================
//------------------------ Motorgroup - Initialisation -------------------------
//==============================================================================
//------------------------------------------------------------------------------
void CreateDualScreen(TDualScreen *pScreen, unsigned char Number)
{
  pScreen->Number = Number;
  InitDualScreen(pScreen);
}

void InitDualScreen(TDualScreen *pScreen)
{
  TimerSet(&pScreen->Timer_1min, TIMER_1MIN);
  pScreen->InitFlag = 0;
}

//==============================================================================
//------------------------ DualScreen - Visualisation --------------------------
//==============================================================================
//------------------------------------------------------------------------------
static void ControlDisplayDualScreen(int nr)
{
  if (opt_app.DualScreen[nr].CombiMatic)
  {
    if (opt_app.DualScreen[nr].GroupB != -1)
      DualScreen[nr].PositionB = Motorgroup[opt_app.DualScreen[nr].GroupB].PositionAvg;
    else
      DualScreen[nr].PositionB = 0;

    DualScreen[nr].BitmapB = ((24 * DualScreen[nr].PositionB) + 500) / 1000;

    if (opt_app.DualScreen[nr].GroupA != -1)
    {
      if (opt_app.DualScreen[nr].Absolute)
	  {
        DualScreen[nr].PositionA = Motorgroup[opt_app.DualScreen[nr].GroupA].PositionAvg;
        DualScreen[nr].BitmapA = ((24 * (DualScreen[nr].PositionA - DualScreen[nr].PositionB)) + 500) / 1000;
	  }
      else
	  {
        DualScreen[nr].PositionA = Motorgroup[opt_app.DualScreen[nr].GroupA].PositionAvg - DualScreen[nr].PositionB;
        DualScreen[nr].BitmapA = ((24 * DualScreen[nr].PositionA) + 500) / 1000;
	  }
    }
    else
	{
      DualScreen[nr].PositionA = 0;
	  DualScreen[nr].BitmapA   = 0;
	}
  }
  else
  {
    if (opt_app.DualScreen[nr].GroupA != -1)
      DualScreen[nr].PositionA = Motorgroup[opt_app.DualScreen[nr].GroupA].PositionAvg;
    else
      DualScreen[nr].PositionA = 0;
    if (opt_app.DualScreen[nr].GroupB != -1)
      DualScreen[nr].PositionB = Motorgroup[opt_app.DualScreen[nr].GroupB].PositionAvg;
    else
      DualScreen[nr].PositionB = 0;

    DualScreen[nr].BitmapA = ((24 * DualScreen[nr].PositionA) + 500) / 1000;
    DualScreen[nr].BitmapB = ((24 * DualScreen[nr].PositionB) + 500) / 1000;
  }

  if ((DualScreen[nr].BitmapA + DualScreen[nr].BitmapB) > 24)
    DualScreen[nr].BitmapB = 24 - DualScreen[nr].BitmapA;
}

//==============================================================================
//------------------------ DualScreen - Feedback -------------------------------
//==============================================================================
//------------------------------------------------------------------------------
static void ControlFeedbackDualScreen(TMotorgroup *pGroup)
{
  MotorgroupSetFeedback(pGroup, pGroup->PositionAvg);
}

static void ControlFeedbackCombiMatic(TMotorgroup *pGroup)
{
int index;

  if (!pGroup)
    return;

  index = opt_app.Motorgroup[pGroup->Number].ControlIndex;
  if (pGroup->Number == opt_app.DualScreen[index].GroupA)
  {
    MotorgroupSetFeedback(pGroup, DualScreen[index].PositionA);
  }
  else
  {
    MotorgroupSetFeedback(pGroup, DualScreen[index].PositionB);
  }
}

//==============================================================================
//------------------------ DualScreen - Control --------------------------------
//==============================================================================
//------------------------------------------------------------------------------
static void ResetStandby(int nr)
{
  val_hr_alg.DualScreen[nr].TimerStandby = 0;
  DualScreen[nr].Standby = 0;
  TimerSet(&DualScreen[nr].Timer_1min, TIMER_1MIN);
}

//------------------------------------------------------------------------------
static void ControlStandby(int nr)
{
  if (opt_app.DualScreen[nr].Standby)
  {
    if (!DualScreen[nr].Standby)
    {
      if (TimerExpired(&DualScreen[nr].Timer_1min))
      {
        val_hr_alg.DualScreen[nr].TimerStandby++;
        TimerReset(&DualScreen[nr].Timer_1min);
      }
      if (val_hr_alg.DualScreen[nr].TimerStandby >= setp_alg.DualScreen[nr].StandbyTime)
        DualScreen[nr].Standby = 1;
    }
  }
  else
    DualScreen[nr].Standby = 0;
}

//------------------------------------------------------------------------------
static unsigned char CheckIfGroupMaster(TMotorgroup *pGroup, int OldPosition)
{
int index = opt_app.Motorgroup[pGroup->Number].ControlIndex;

  if (pGroup->Link == NULL)
    return (1);

  if (pGroup->Number == opt_app.DualScreen[index].GroupA)
  {
    switch (opt_app.DualScreen[index].Master)
    {
      case 1: return (1);
      case 2: return (0);
    }
  }
  else
  {
    switch (opt_app.DualScreen[index].Master)
    {
      case 1: return (0);
      case 2: return (1);
    }
  }

  switch (val_hr_alg.Motorgroup[pGroup->Number].OperationMode)
  {
    case omAuto:
      switch (val_hr_alg.Motorgroup[pGroup->Link->Number].OperationMode)
      {
        case omAuto:
		  if (opt_app.DualScreen[index].CombiMatic && opt_app.DualScreen[index].Absolute)
		  {
            if (pGroup->Number == opt_app.DualScreen[index].GroupA)
			{
              if ((pGroup->RunningMode == rmOpen) && (pGroup->Link->RunningMode != rmClose))
                return (1);
			}
			else
			{
              if ((pGroup->RunningMode == rmClose) && (pGroup->Link->RunningMode != rmOpen))
                return (1);
			}
		  }
		  else
		  {
            if (opt_alg.can_backbone)
            {
              if (val_hr_alg.Motorgroup[pGroup->Number].PositionPerc > OldPosition)
                return (1);
            }
            else
            {
              if ((pGroup->RunningMode == rmClose) && (pGroup->Link->RunningMode != rmClose))
                return (1);
            }
		  }
          return (0);
        case omManual:
          return (0);
        case omOff:
          return (1);
      }
      break;
    case omManual:
      switch (val_hr_alg.Motorgroup[pGroup->Link->Number].OperationMode)
      {
        case omAuto:
          return (1);
        case omManual:
          return (0);
        case omOff:
          return (1);
      }
      break;
    case omOff:
      return (0);
  }
  return (0);     
}

//------------------------------------------------------------------------------
void ControlDualScreen(TMotorgroup *pGroup)
{
int OldPosition;
int index = opt_app.Motorgroup[pGroup->Number].ControlIndex;

  //---------------------------------------------------------
  // Get new value for screen
  //---------------------------------------------------------
  OldPosition = val_hr_alg.Motorgroup[pGroup->Number].PositionPerc;
  MotorgroupGetTargetPosition(pGroup);

  //---------------------------------------------------------
  // Check standby time
  //---------------------------------------------------------
  if ((val_hr_alg.Motorgroup[pGroup->Number].OperationMode == omAuto) && (val_hr_alg.Motorgroup[pGroup->Number].PositionPerc == OldPosition))
    ControlStandby(index);
  else
    ResetStandby(index);

  //---------------------------------------------------------
  // Check master
  //---------------------------------------------------------
  if (CheckIfGroupMaster(pGroup, OldPosition))
    val_hr_alg.DualScreen[index].Master = pGroup->Number;
  if ((val_hr_alg.DualScreen[index].Master != pGroup->Number) && (val_hr_alg.DualScreen[index].Master != pGroup->Link->Number))
    val_hr_alg.DualScreen[index].Master = pGroup->Number;

  //---------------------------------------------------------
  // Control groups
  //---------------------------------------------------------
  if (opt_app.DualScreen[index].CombiMatic)
    SetGroupCombiMatic(pGroup);
  else
    SetGroupDualScreen(pGroup);

  //---------------------------------------------------------
  // Set display variables - also used for feedback
  //---------------------------------------------------------
  ControlDisplayDualScreen(index);

  //---------------------------------------------------------
  // Set feedback
  //---------------------------------------------------------
  if (opt_app.DualScreen[index].CombiMatic)
    ControlFeedbackCombiMatic(pGroup);
  else
    ControlFeedbackDualScreen(pGroup);
  
  //---------------------------------------------------------
  // Control motors
  //---------------------------------------------------------
  if (opt_app.DualScreen[index].CombiMatic)
    SetMotorsCombiMatic(pGroup);
  else
    SetMotorsDualScreen(pGroup);
}

//==============================================================================
//------------------------ CombiMatic ------------------------------------------
//==============================================================================
//------------------------------------------------------------------------------
// || 2nd screen              1st screen                                      ||
// ||^-^-^-^-^-^-^-^-^-^-^-|^-^-^-^-^-^-^-^-^-^-^-^-|                         ||
// ||.........................................................................||
static void SetGroupCombiMatic(TMotorgroup *pGroup)
{
int CurrentPositionA, CurrentPositionB; // A = first screen; B = second screen
int VirtualPositionA, VirtualPositionB; // A = first screen; B = second screen
int DesiredPositionA, DesiredPositionB; // A = first screen; B = second screen
int MinimumGap;
unsigned char MasterA;
int index = opt_app.Motorgroup[pGroup->Number].ControlIndex;

  //---------------------------------------------------------
  // Define first and second screen
  //---------------------------------------------------------
  if (pGroup->Number == opt_app.DualScreen[index].GroupA)
  {
    VirtualPositionA = val_hr_alg.Motorgroup[pGroup->Number].PositionPerc;
    if (pGroup->Link == NULL)
      VirtualPositionB = 0;
    else
      VirtualPositionB = val_hr_alg.Motorgroup[pGroup->Link->Number].PositionPerc;
    MasterA = (pGroup->Number == val_hr_alg.DualScreen[index].Master) ? 1 : 0;
  }
  else
  {
    if (pGroup->Link == NULL)
      VirtualPositionA = 0;
    else
      VirtualPositionA = val_hr_alg.Motorgroup[pGroup->Link->Number].PositionPerc;
    VirtualPositionB = val_hr_alg.Motorgroup[pGroup->Number].PositionPerc;
    MasterA = (pGroup->Number == val_hr_alg.DualScreen[index].Master) ? 0 : 1;
  }
  CurrentPositionA = val_hr_alg.DualScreen[index].PositionGroupA;
  CurrentPositionB = val_hr_alg.DualScreen[index].PositionGroupB;

  //---------------------------------------------------------
  // Define gap
  //---------------------------------------------------------
  MinimumGap = CurrentPositionA - VirtualPositionB;
  if (DualScreen[index].Standby || (VirtualPositionB == 0) || (VirtualPositionA == 1000))
    MinimumGap = 0;
  else if (VirtualPositionB > CurrentPositionB)
    MinimumGap = setp_alg.DualScreen[index].Opening;
  else if (MinimumGap > setp_alg.DualScreen[index].Opening)
    MinimumGap = setp_alg.DualScreen[index].Opening;
  else if (MinimumGap < 0)
    MinimumGap = 0;

  //---------------------------------------------------------
  // Adjust positions
  //---------------------------------------------------------
  if (opt_app.DualScreen[index].Absolute)
  {
    if (MasterA)
    {
      DesiredPositionA = VirtualPositionA;
      DesiredPositionB = DesiredPositionA - MinimumGap;
      if (DesiredPositionB > VirtualPositionB)
        DesiredPositionB = VirtualPositionB;
      else if (DesiredPositionB < 0)
        DesiredPositionB = 0;
    }
    else
    {
      DesiredPositionB = VirtualPositionB;
      DesiredPositionA = DesiredPositionB + MinimumGap;
      if (DesiredPositionA < VirtualPositionA)
        DesiredPositionA = VirtualPositionA;
      else if (DesiredPositionA > 1000)
        DesiredPositionA = 1000;
    }
  }
  else
  {
    if (VirtualPositionA > MinimumGap)
      DesiredPositionA = VirtualPositionB + VirtualPositionA;
    else
      DesiredPositionA = VirtualPositionB + MinimumGap;
    DesiredPositionB = VirtualPositionB;
    if (DesiredPositionA > 1000)
    {
      DesiredPositionA = 1000;
      if (MasterA)
        DesiredPositionB = 1000 - VirtualPositionA;
      else
        DesiredPositionB = VirtualPositionB;
    }
  }

  //---------------------------------------------------------
  // Write new positions to DualScreen
  //---------------------------------------------------------
  val_hr_alg.DualScreen[index].PositionGroupA = DesiredPositionA;
  val_hr_alg.DualScreen[index].PositionGroupB = DesiredPositionB;
}

//------------------------------------------------------------------------------
static void SetMotorsCombiMatic(TMotorgroup *pGroup)
{
TMotor *pMotor;
int Gap, MinGap, MaxGap;
int GapOverruled;
int DesiredPosition;
int index = opt_app.Motorgroup[pGroup->Number].ControlIndex;

  //---------------------------------------------------------
  // Define minimum and maximum gap between screens
  //---------------------------------------------------------
  if (DualScreen[index].Standby)
  {
    Gap    = 0;
    MinGap = 0;
    MaxGap = 0;
    GapOverruled = 0;
  }
  else
  {
    Gap    = setp_alg.DualScreen[index].Opening;
    MinGap = setp_alg.DualScreen[index].Opening - setp_alg.DualScreen[index].Hysteresis;
    MaxGap = setp_alg.DualScreen[index].Opening + setp_alg.DualScreen[index].Hysteresis;
    GapOverruled = setp_alg.DualScreen[index].Opening + (2 * setp_alg.DualScreen[index].Hysteresis);
  }

  pMotor = pGroup->FirstMotor;
  while (pMotor != NULL)
  {
    // Check if there is some kind of action, then reset the standby timer
    if (val_hr_alg.Motor[pMotor->Number].Overruled || (val_hr_alg.Motor[pMotor->Number].OperationMode != omAuto) || ((pMotor->RunningMode != rmStop) && !DualScreen[index].Standby))
      ResetStandby(index);

    // Define the desired position
    if (pGroup->Number == opt_app.DualScreen[index].GroupA)
      DesiredPosition = val_hr_alg.DualScreen[index].PositionGroupA;
    else
      DesiredPosition = val_hr_alg.DualScreen[index].PositionGroupB;

    if (pMotor->Link != NULL)
    {
      // check for exceptions
      if (pGroup->Number == opt_app.DualScreen[index].GroupA) // First screen
      {
        // Check if linked motor overruled
        if (val_hr_alg.Motor[pMotor->Link->Number].Overruled)
        {
          switch (pMotor->Link->RunningMode)
          {
            case rmStop:
			  if (opt_app.DualScreen[index].Absolute)
			  {
			    if (val_hr_alg.Motorgroup[pGroup->Number].PositionPerc < (val_hr_alg.Motor[pMotor->Link->Number].Position + GapOverruled))
				  DesiredPosition = val_hr_alg.Motor[pMotor->Link->Number].Position + GapOverruled;
				else
			      DesiredPosition = val_hr_alg.Motorgroup[pGroup->Number].PositionPerc;
			  }
			  else
			  {
                if (val_hr_alg.Motorgroup[pGroup->Number].PositionPerc > GapOverruled)
                  DesiredPosition = val_hr_alg.Motor[pMotor->Link->Number].Position + val_hr_alg.Motorgroup[pGroup->Number].PositionPerc;
                else
                  DesiredPosition = val_hr_alg.Motor[pMotor->Link->Number].Position + GapOverruled;
			  }
              if (DesiredPosition > 1000)
                DesiredPosition = 1000;
              break;
            case rmOpen:
              DesiredPosition = val_hr_alg.Motorgroup[pGroup->Number].PositionPerc;
              break;
            case rmClose:
			  if (opt_app.DualScreen[index].Absolute)
			  {
			    if (val_hr_alg.Motorgroup[pGroup->Number].PositionPerc > (val_hr_alg.Motor[pMotor->Link->Number].Position + GapOverruled))
				  DesiredPosition = val_hr_alg.Motorgroup[pGroup->Number].PositionPerc;
				else
			      DesiredPosition = 1000;
			  }
			  else
			  {
                DesiredPosition = 1000;
			  }
              break;
          }
        }
        // Check if opposite motor off
        else if (val_hr_alg.Motor[pMotor->Link->Number].OperationMode == omOff)
        {
          DesiredPosition = val_hr_alg.Motorgroup[pGroup->Number].PositionPerc;
        }
        // Check if opposite motor in manual
        else if (val_hr_alg.Motor[pMotor->Link->Number].OperationMode == omManual)
        {
		  if (opt_app.DualScreen[index].Absolute)
		  {
		    if (val_hr_alg.Motorgroup[pGroup->Number].PositionPerc > val_hr_alg.Motor[pMotor->Link->Number].PositionManual + Gap)
              DesiredPosition = val_hr_alg.Motorgroup[pGroup->Number].PositionPerc;
            else
              DesiredPosition = val_hr_alg.Motor[pMotor->Link->Number].PositionManual + Gap;
		  }
		  else
		  {
		    if (val_hr_alg.Motorgroup[pGroup->Number].PositionPerc > Gap)
              DesiredPosition = val_hr_alg.Motor[pMotor->Link->Number].PositionManual + val_hr_alg.Motorgroup[pGroup->Number].PositionPerc;
            else
              DesiredPosition = val_hr_alg.Motor[pMotor->Link->Number].PositionManual + Gap;
		  }
          if (DesiredPosition > 1000)
            DesiredPosition = 1000;
        }
      }
      else // SecondScreen
      {
        // Check if linked motor overruled
        if (val_hr_alg.Motor[pMotor->Link->Number].Overruled)
        {
          switch (pMotor->Link->RunningMode)
          {
            case rmStop:
              if (val_hr_alg.Motorgroup[pGroup->Number].PositionPerc > (val_hr_alg.Motor[pMotor->Link->Number].Position - GapOverruled))
                DesiredPosition = val_hr_alg.Motor[pMotor->Link->Number].Position - GapOverruled;
			  else
                DesiredPosition = val_hr_alg.Motorgroup[pGroup->Number].PositionPerc;
              if (DesiredPosition < 0)
                DesiredPosition = 0;
              break;
            case rmOpen:
			  if (opt_app.DualScreen[index].Absolute)
			  {
			    if (val_hr_alg.Motorgroup[pGroup->Number].PositionPerc < (val_hr_alg.Motor[pMotor->Link->Number].Position - GapOverruled))
				  DesiredPosition = val_hr_alg.Motorgroup[pGroup->Number].PositionPerc;
				else
			      DesiredPosition = 0;
			  }
			  else
			  {
                DesiredPosition = 0;
			  }
              break;
            case rmClose:
              DesiredPosition = val_hr_alg.Motorgroup[pGroup->Number].PositionPerc;
              break;
          }
        }
        // Check if opposite motor off
        else if (val_hr_alg.Motor[pMotor->Link->Number].OperationMode == omOff)
        {
          DesiredPosition = 0;
        }
        // Check if opposite motor in manual
        else if (val_hr_alg.Motor[pMotor->Link->Number].OperationMode == omManual)
        {
          if (val_hr_alg.Motorgroup[pGroup->Number].PositionPerc > (val_hr_alg.Motor[pMotor->Link->Number].PositionManual - Gap))
            DesiredPosition = val_hr_alg.Motor[pMotor->Link->Number].PositionManual - Gap;
		  else
		    DesiredPosition = val_hr_alg.Motorgroup[pGroup->Number].PositionPerc;
          if (DesiredPosition < 0)
            DesiredPosition = 0;
        }
      }

      // Check if opening too small
      if (pGroup->Number == opt_app.DualScreen[index].GroupA)
      {
	    if (pMotor->Flags.HoldClose)
          SetMotorHold(pMotor, rmClose);
		else
          SetMotorRelease(pMotor, rmClose);

		if (pMotor->Flags.HoldOpen)
		  SetMotorHold(pMotor, rmOpen);
        else if (pMotor->Link->Flags.LimitOpen && (val_hr_alg.Motor[pMotor->Link->Number].Position < 20))
          SetMotorRelease(pMotor, rmOpen); // Linked motor is open
        else if (val_hr_alg.Motor[pMotor->Number].Position <= (val_hr_alg.Motor[pMotor->Link->Number].Position + MinGap))
          SetMotorHold(pMotor, rmOpen); // Hold motor if opening
        else if (val_hr_alg.Motor[pMotor->Number].Position >= (val_hr_alg.Motor[pMotor->Link->Number].Position + Gap))
          SetMotorRelease(pMotor, rmOpen); // Gap ok, release motor
      }
      else
      {
	    if (pMotor->Flags.HoldOpen)
          SetMotorHold(pMotor, rmOpen);
		else
          SetMotorRelease(pMotor, rmOpen);

		if (pMotor->Flags.HoldClose)
		   SetMotorHold(pMotor, rmClose);
        else if (pMotor->Link->Flags.LimitClose && (val_hr_alg.Motor[pMotor->Link->Number].Position > 980))
          SetMotorRelease(pMotor, rmClose); // Linked motor is closed
        else if (val_hr_alg.Motor[pMotor->Number].Position >= (val_hr_alg.Motor[pMotor->Link->Number].Position - MinGap))
          SetMotorHold(pMotor, rmClose); // Hold motor if closing
        else if (val_hr_alg.Motor[pMotor->Number].Position <= (val_hr_alg.Motor[pMotor->Link->Number].Position - Gap))
          SetMotorRelease(pMotor, rmClose); // Gap ok, release motor
      }
      ClearAlarm(&alarm_hr_alg.Motor[pMotor->Number].LinkUnknown, MOTOR_LINK_UNKNOWN_AL, pMotor->Number);
    }
    else
    {
      DesiredPosition = val_hr_alg.Motorgroup[pGroup->Number].PositionPerc;
      SetMotorHold(pMotor, rmOpen);
      SetMotorHold(pMotor, rmClose); // Hold both directions when opposite screen is unknown
      CreateAlarm(&alarm_hr_alg.Motor[pMotor->Number].LinkUnknown, MOTOR_LINK_UNKNOWN_AL, pMotor->Number, 0, opt_app.Motor[pMotor->Number].GroupNumber, HARD_ALARM);
    }

    if (opt_app.Motorgroup[pGroup->Number].KierRegeling && 
        (val_hr_alg.Motorgroup[pGroup->Number].OperationMode == omAuto) &&
        ((val_hr_alg.Motor[pMotor->Number].PositionAuto == 1000) && (pMotor->Flags.LimitClose == 0)))
      val_hr_alg.Motor[pMotor->Number].PositionAuto = 1000;
	else if ((pGroup->Delay == 0) && (pGroup->Link->Delay == 0))
      val_hr_alg.Motor[pMotor->Number].PositionAuto = DesiredPosition;
    pMotor = pMotor->Next;
  }
}

//==============================================================================
//------------------------ DualScreen ------------------------------------------
//==============================================================================
//------------------------------------------------------------------------------
// || 2nd screen                                       1st screen             ||
// ||^-^-^-^-^-^-^-^-^-^-^-|                         |^-^-^-^-^-^-^-^-^-^-^-^-||
// ||.........................................................................||
static void SetGroupDualScreen(TMotorgroup *pGroup)
{
int CurrentPositionA, CurrentPositionB; // A = master; B = slave
int VirtualPositionA, VirtualPositionB; // A = master; B = slave
int DesiredPositionA, DesiredPositionB; // A = master; B = slave
int MinimumGap;
int index = opt_app.Motorgroup[pGroup->Number].ControlIndex;

  //---------------------------------------------------------
  // Define master and slave screen
  //---------------------------------------------------------
  if (pGroup->Number == val_hr_alg.DualScreen[index].Master)
  {
    VirtualPositionA = val_hr_alg.Motorgroup[pGroup->Number].PositionPerc;
    if (pGroup->Link == NULL)
      VirtualPositionB = 0;
    else
      VirtualPositionB = val_hr_alg.Motorgroup[pGroup->Link->Number].PositionPerc;
  }
  else
  {
    if (pGroup->Link == NULL)
      VirtualPositionA = 0;
    else
      VirtualPositionA = val_hr_alg.Motorgroup[pGroup->Link->Number].PositionPerc;
    VirtualPositionB = val_hr_alg.Motorgroup[pGroup->Number].PositionPerc;
  }
  if (val_hr_alg.DualScreen[index].Master == opt_app.DualScreen[index].GroupA)
  {
    CurrentPositionA = val_hr_alg.DualScreen[index].PositionGroupA;
    CurrentPositionB = val_hr_alg.DualScreen[index].PositionGroupB;
  }
  else
  {
    CurrentPositionA = val_hr_alg.DualScreen[index].PositionGroupB;
    CurrentPositionB = val_hr_alg.DualScreen[index].PositionGroupA;
  }

  //---------------------------------------------------------
  // Define gap
  //---------------------------------------------------------
  MinimumGap = 1000 - VirtualPositionA - CurrentPositionB;
  if (DualScreen[index].Standby || (VirtualPositionA == 0) || (VirtualPositionB == 0))
    MinimumGap = 0;
  else if (VirtualPositionA > CurrentPositionA)
    MinimumGap = setp_alg.DualScreen[index].Opening;
  else if (MinimumGap > setp_alg.DualScreen[index].Opening)
    MinimumGap = setp_alg.DualScreen[index].Opening;
  else if (MinimumGap < 0)
    MinimumGap = 0;

  //---------------------------------------------------------
  // Adjust positions
  //---------------------------------------------------------
  DesiredPositionA = VirtualPositionA;
  DesiredPositionB = 1000 - VirtualPositionA - MinimumGap;
  if (DesiredPositionB < 0)
    DesiredPositionB = 0;
  if (DesiredPositionB > VirtualPositionB)
    DesiredPositionB = VirtualPositionB;

  //---------------------------------------------------------
  // Write new positions to DualScreen
  //---------------------------------------------------------
  if (val_hr_alg.DualScreen[index].Master == opt_app.DualScreen[index].GroupA)
  {
    val_hr_alg.DualScreen[index].PositionGroupA = DesiredPositionA;
    val_hr_alg.DualScreen[index].PositionGroupB = DesiredPositionB;
  }
  else
  {
    val_hr_alg.DualScreen[index].PositionGroupB = DesiredPositionA;
    val_hr_alg.DualScreen[index].PositionGroupA = DesiredPositionB;
  }
}

//------------------------------------------------------------------------------
static void SetMotorsDualScreen(TMotorgroup *pGroup)
{
TMotor *pMotor;
int GapGroup, GapMotor;
int MinGapOpen, MinGapClose;
int MaxGapOpen, MaxGapClose;
int DesiredPosition;
int index = opt_app.Motorgroup[pGroup->Number].ControlIndex;
unsigned char LinkedFlagLimitOpen;

  //---------------------------------------------------------
  // Define minimum and maximum gap between screens
  //---------------------------------------------------------
  if (pGroup->Link == NULL)
    GapGroup = 1000 - val_hr_alg.Motorgroup[pGroup->Number].PositionPerc;
  else
    GapGroup = 1000 - val_hr_alg.Motorgroup[pGroup->Number].PositionPerc - val_hr_alg.Motorgroup[pGroup->Link->Number].PositionPerc;
  if (GapGroup < setp_alg.DualScreen[index].Opening)
  {
    if (DualScreen[index].Standby)
    {
      MaxGapClose = 0;
      MinGapClose = 0;
      MinGapOpen  = 0;
      MaxGapOpen  = 0;
    }
    else
    {
      MaxGapClose = setp_alg.DualScreen[index].Opening;
      MinGapClose = MaxGapClose - setp_alg.DualScreen[index].Hysteresis;
      MinGapOpen = setp_alg.DualScreen[index].Opening;
      MaxGapOpen = MinGapOpen + setp_alg.DualScreen[index].Hysteresis;
    }
  }
  else
  {
    MaxGapClose = setp_alg.DualScreen[index].Opening;
    MinGapClose = MaxGapClose - setp_alg.DualScreen[index].Hysteresis;
    MinGapOpen = GapGroup;
    MaxGapOpen = MinGapOpen + setp_alg.DualScreen[index].Hysteresis;
  }

  pMotor = pGroup->FirstMotor;
  while (pMotor != NULL)
  {
    // Check if there is some kind of action, then reset the standby timer
    if (val_hr_alg.Motor[pMotor->Number].Overruled || (val_hr_alg.Motor[pMotor->Number].OperationMode != omAuto) || ((pMotor->RunningMode != rmStop) && !DualScreen[index].Standby))
      ResetStandby(index);

    // Define the desired position
    if (pGroup->Number == opt_app.DualScreen[index].GroupA)
      DesiredPosition = val_hr_alg.DualScreen[index].PositionGroupA;
    else
      DesiredPosition = val_hr_alg.DualScreen[index].PositionGroupB;

    if (pMotor->Link != NULL)
    {
	  LinkedFlagLimitOpen = Motor[pMotor->Link->Number].Flags.LimitOpen; 
      GapMotor = 1000 - val_hr_alg.Motor[pMotor->Number].Position - val_hr_alg.Motor[pMotor->Link->Number].Position;
      // Check if opposite motor overruled
      if (val_hr_alg.Motor[pMotor->Link->Number].Overruled)
      {
        switch (pMotor->Link->RunningMode)
        {
          case rmStop:
            DesiredPosition = 1000 - val_hr_alg.Motor[pMotor->Link->Number].Position - MaxGapClose;
            if (DesiredPosition > val_hr_alg.Motorgroup[pGroup->Number].PositionPerc)
              DesiredPosition = val_hr_alg.Motorgroup[pGroup->Number].PositionPerc;
            else if (DesiredPosition < 0)
              DesiredPosition = 0;
            break;
          case rmOpen:
            DesiredPosition = val_hr_alg.Motorgroup[pGroup->Number].PositionPerc;
            break;
          case rmClose:
            if ((1000 - val_hr_alg.Motor[pMotor->Link->Number].Position - MaxGapClose) < val_hr_alg.Motorgroup[pGroup->Number].PositionPerc)
              DesiredPosition = 0;
            else
              DesiredPosition = val_hr_alg.Motorgroup[pGroup->Number].PositionPerc;
            break;
        }
      }
      // Check if opposite motor off
      else if (val_hr_alg.Motor[pMotor->Link->Number].OperationMode == omOff)
      {
        DesiredPosition = val_hr_alg.Motorgroup[pGroup->Number].PositionPerc;
      }
      // Check if opposite motor in manual
      else if (val_hr_alg.Motor[pMotor->Link->Number].OperationMode == omManual)
      {
        DesiredPosition = 1000 - val_hr_alg.Motor[pMotor->Link->Number].PositionManual - MaxGapClose;
        if (DesiredPosition > val_hr_alg.Motorgroup[pGroup->Number].PositionPerc)
          DesiredPosition = val_hr_alg.Motorgroup[pGroup->Number].PositionPerc;
        if (DesiredPosition < 0)
          DesiredPosition = 0;
      }

      // Check if opening too small
	  if (pMotor->Flags.HoldClose)
	    SetMotorHold(pMotor, rmClose);
      else if (pMotor->Link->Flags.LimitOpen && (val_hr_alg.Motor[pMotor->Link->Number].Position < 20)) // Linked motor is open
        SetMotorRelease(pMotor, rmClose);
      else if (GapMotor < MinGapClose)
        SetMotorHold(pMotor, rmClose);
      else if (GapMotor > MaxGapClose)
        SetMotorRelease(pMotor, rmClose);

      // Check if opening too large
	  if (pMotor->Flags.HoldOpen)
	    SetMotorHold(pMotor, rmOpen);
      else if ((val_hr_alg.Motor[pMotor->Number].OperationMode != omAuto) || (val_hr_alg.Motor[pMotor->Number].Overruled) || (alarm_hr_alg.Motor[pMotor->Link->Number].AlarmCode != 0))
        SetMotorRelease(pMotor, rmOpen); // Manual or alarm, release motor
      else if (GapMotor > MaxGapOpen)
        SetMotorHold(pMotor, rmOpen); // Hold motor if opening
      else if (GapMotor < MinGapOpen)
        SetMotorRelease(pMotor, rmOpen); // Opening ok, release motor
      ClearAlarm(&alarm_hr_alg.Motor[pMotor->Number].LinkUnknown, MOTOR_LINK_UNKNOWN_AL, pMotor->Number);
    }
    else
    {
      LinkedFlagLimitOpen = 0;
      DesiredPosition = val_hr_alg.Motorgroup[pGroup->Number].PositionPerc;
      SetMotorHold(pMotor, rmOpen);
      SetMotorHold(pMotor, rmClose); // Hold both directions when opposite screen is unknown
      CreateAlarm(&alarm_hr_alg.Motor[pMotor->Number].LinkUnknown, MOTOR_LINK_UNKNOWN_AL, pMotor->Number, 0, opt_app.Motor[pMotor->Number].GroupNumber, HARD_ALARM);
    }
    if (opt_app.Motorgroup[pGroup->Number].KierRegeling && 
        (val_hr_alg.Motorgroup[pGroup->Number].OperationMode == omAuto) &&
        ((val_hr_alg.Motor[pMotor->Number].PositionAuto == 1000) && (pMotor->Flags.LimitClose == 0) && LinkedFlagLimitOpen))
      val_hr_alg.Motor[pMotor->Number].PositionAuto = 1000;
	else if ((pGroup->Delay == 0) && (pGroup->Link->Delay == 0))
      val_hr_alg.Motor[pMotor->Number].PositionAuto = DesiredPosition;
    pMotor = pMotor->Next;
  }
}
