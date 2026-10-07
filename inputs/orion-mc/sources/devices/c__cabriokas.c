// C__CABRIOKAS.C  

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
#include "ch_cabriokas.h"

static void SetGroupVoorNaloop(TMotorgroup *pGroup);
static void SetGroupGelijkloop(TMotorgroup *pGroup);
static void SetMotorsVoorNaloop(TMotorgroup *pGroup);
static void SetMotorsGelijkloop(TMotorgroup *pGroup);

TCabriokas Cabriokas[MAX_CABRIO];

//==============================================================================
//------------------------ Cabriokas - Initialisation --------------------------
//==============================================================================
//------------------------------------------------------------------------------
void CreateCabriokas(TCabriokas *pCabrio, unsigned char Number)
{
  pCabrio->Number = Number;
  InitCabriokas(pCabrio);
}

void InitCabriokas(TCabriokas *pCabrio)
{
  TimerSet(&pCabrio->Timer_1min, TIMER_1MIN);
  pCabrio->InitFlag = 0;
}

//==============================================================================
//------------------------ Cabriokas - Control ---------------------------------
//==============================================================================
//------------------------------------------------------------------------------
void ControlCabriokas(TMotorgroup *pGroup)
{
int index = opt_app.Motorgroup[pGroup->Number].ControlIndex;

  //---------------------------------------------------------
  // Get new value for screen
  //---------------------------------------------------------
  MotorgroupGetTargetPosition(pGroup);

  //---------------------------------------------------------
  // Control groups
  //---------------------------------------------------------
  if (opt_app.Cabriokas[index].Type)
    SetGroupGelijkloop(pGroup);
  else
    SetGroupVoorNaloop(pGroup);

  //---------------------------------------------------------
  // Control motors
  //---------------------------------------------------------
  if (opt_app.Cabriokas[index].Type)
    SetMotorsGelijkloop(pGroup);
  else
    SetMotorsVoorNaloop(pGroup);

  //---------------------------------------------------------
  // Set feedback
  //---------------------------------------------------------
  MotorgroupSetFeedback(pGroup, pGroup->PositionAvg);
}

//==============================================================================
//------------------------ Voor-/Naloop ----------------------------------------
//==============================================================================
//------------------------------------------------------------------------------
// De ramen worden gesloten door een kleine overkapping op één raam welke over
// het andere raam heen valt bij sluiten. Hierbij is het van belang dat het ene
// raam al dicht is voordat het andere (met de overkapping) sluit. Bij het openen
// geldt dit natuurlijk andersom. Het raam met overkapping moet eerst openen
// voordat het andere mag openen.
//------------------------------------------------------------------------------
//
//                 /               ---\
//               /                      \
//             /                          \
//           /   Naloop                     \    Voorloop
//         /                                  \
//       /                                      \
//     /||                                      ||\
//      ||                                      ||
//      ||......................................||
static void SetGroupVoorNaloop(TMotorgroup *pGroup)
{
int VirtualPositionV, VirtualPositionN; // V = Voorloop; N = Naloop
int DesiredPositionV, DesiredPositionN; // V = Voorloop; N = Naloop
int index = opt_app.Motorgroup[pGroup->Number].ControlIndex;

  //---------------------------------------------------------
  // Define voorloop and naloop
  //---------------------------------------------------------
  if (pGroup->Number == opt_app.Cabriokas[index].GroupA)
  {
    VirtualPositionV = val_hr_alg.Motorgroup[pGroup->Number].PositionPerc;
    if (pGroup->Link == NULL)
      VirtualPositionN = 0;
    else
      VirtualPositionN = val_hr_alg.Motorgroup[pGroup->Link->Number].PositionPerc;
  }
  else
  {
    if (pGroup->Link == NULL)
      VirtualPositionV = 0;
    else
      VirtualPositionV = val_hr_alg.Motorgroup[pGroup->Link->Number].PositionPerc;
    VirtualPositionN = val_hr_alg.Motorgroup[pGroup->Number].PositionPerc;
  }

  //---------------------------------------------------------
  // Adjust positions
  //---------------------------------------------------------
  if (VirtualPositionV < (opt_app.Cabriokas[index].Voorloop + opt_app.Cabriokas[index].Hysterese))
  {
    if ((VirtualPositionV + VirtualPositionN) < (opt_app.Cabriokas[index].Voorloop + opt_app.Cabriokas[index].Hysterese))
    {
      DesiredPositionV = VirtualPositionV + VirtualPositionN;
      DesiredPositionN = 0;
    }
    else
    {
      DesiredPositionV = opt_app.Cabriokas[index].Voorloop + opt_app.Cabriokas[index].Hysterese;
      DesiredPositionN = (VirtualPositionV + VirtualPositionN) - (opt_app.Cabriokas[index].Voorloop + opt_app.Cabriokas[index].Hysterese);
    }
  }
  else
  {
    DesiredPositionV = VirtualPositionV;
    DesiredPositionN = VirtualPositionN;
  }

  //---------------------------------------------------------
  // Write new positions to Cabriokas
  //---------------------------------------------------------
  Cabriokas[index].PositionA = DesiredPositionV;
  Cabriokas[index].PositionB = DesiredPositionN;
}

//------------------------------------------------------------------------------
static void SetMotorsVoorNaloop(TMotorgroup *pGroup)
{
unsigned char Voorloop;
unsigned char Naloop;
TMotor *pMotor;
int DesiredPosition;
int MotorPosition;
int index = opt_app.Motorgroup[pGroup->Number].ControlIndex;

  //---------------------------------------------------------
  // Define voorloop and naloop
  //---------------------------------------------------------
  if (pGroup->Number == opt_app.Cabriokas[index].GroupA)
  {
    Voorloop = 1;
    Naloop   = 0;
    DesiredPosition = Cabriokas[index].PositionA;
  }
  else
  {
    Voorloop = 0;
    Naloop   = 1;
    DesiredPosition = Cabriokas[index].PositionB;
  }
  
  //---------------------------------------------------------
  // Set motor position
  //---------------------------------------------------------
  pMotor = pGroup->FirstMotor;
  while (pMotor != NULL)
  {
    if (pMotor->Link != NULL)
    {
      MotorPosition = DesiredPosition;
      if (Voorloop)
      {
        // Check if linked motor overruled
        if (val_hr_alg.Motor[pMotor->Link->Number].Overruled)
        {
          if (DesiredPosition < (opt_app.Cabriokas[index].Voorloop + opt_app.Cabriokas[index].Hysterese))
            MotorPosition = opt_app.Cabriokas[index].Voorloop + opt_app.Cabriokas[index].Hysterese;
        }
        else if (val_hr_alg.Motor[pMotor->Link->Number].OperationMode == omManual)
        {
          if ((val_hr_alg.Motor[pMotor->Link->Number].PositionManual > 0) && (DesiredPosition < (opt_app.Cabriokas[index].Voorloop + opt_app.Cabriokas[index].Hysterese)))
            MotorPosition = opt_app.Cabriokas[index].Voorloop + opt_app.Cabriokas[index].Hysterese;
        }
      }

      // Check to hold or release motor
      if (Voorloop)
      {
        if (pMotor->Flags.HoldOpen)
          SetMotorHold(pMotor, rmOpen);
        else //
          SetMotorRelease(pMotor, rmOpen); // Open always allowed

        if (pMotor->Flags.HoldClose)
          SetMotorHold(pMotor, rmClose);
        else if (pMotor->Link->Flags.LimitClose || (val_hr_alg.Motor[pMotor->Number].Position > (opt_app.Cabriokas[index].Voorloop + opt_app.Cabriokas[index].Hysterese)))
          SetMotorRelease(pMotor, rmClose); // Release motor if closing
        else
          SetMotorHold(pMotor, rmClose); // Hold motor if closing
      }
      else
      {
        if (pMotor->Flags.HoldClose)
          SetMotorHold(pMotor, rmClose);
        else if (val_hr_alg.Motor[pMotor->Link->Number].Position >= opt_app.Cabriokas[index].Voorloop)
          SetMotorRelease(pMotor, rmClose);
        else if (val_hr_alg.Motor[pMotor->Number].Position > (opt_app.Cabriokas[index].Voorloop + opt_app.Cabriokas[index].Hysterese))
          SetMotorRelease(pMotor, rmClose);
        else
          SetMotorHold(pMotor, rmClose);
        
        if (pMotor->Flags.HoldOpen)
          SetMotorHold(pMotor, rmOpen);
        else if (val_hr_alg.Motor[pMotor->Link->Number].Position >= opt_app.Cabriokas[index].Voorloop)
          SetMotorRelease(pMotor, rmOpen); // Release motor if opening
        else if (val_hr_alg.Motor[pMotor->Number].Position > opt_app.Cabriokas[index].Voorloop)
          SetMotorRelease(pMotor, rmOpen);
        else
          SetMotorHold(pMotor, rmOpen); // Hold motor if opening
      }
      ClearAlarm(&alarm_hr_alg.Motor[pMotor->Number].LinkUnknown, MOTOR_LINK_UNKNOWN_AL, pMotor->Number);
    }
    else
    {
      MotorPosition = val_hr_alg.Motorgroup[pGroup->Number].PositionPerc;
      SetMotorHold(pMotor, rmOpen);
      SetMotorHold(pMotor, rmClose); // Hold both directions when opposite screen is unknown
      CreateAlarm(&alarm_hr_alg.Motor[pMotor->Number].LinkUnknown, MOTOR_LINK_UNKNOWN_AL, pMotor->Number, 0, opt_app.Motor[pMotor->Number].GroupNumber, HARD_ALARM);
    }
    if ((pGroup->Delay == 0) && (pGroup->Link->Delay == 0))
      val_hr_alg.Motor[pMotor->Number].PositionAuto = MotorPosition;
    pMotor = pMotor->Next;
  }
}

//==============================================================================
//------------------------ Gelijkloop ------------------------------------------
//==============================================================================
//------------------------------------------------------------------------------
// De ramen worden gesloten door de rubbers van beide ramen gelijktijdig tegen
// elkaar te drukken. Hierbij is het van belang dat beide ramen gelijktijdig
// sluiten. Bij het openen geldt dit natuurlijk andersom. Beide ramen moeten
// gelijktijdig openen.
//------------------------------------------------------------------------------
//
//                 /                  \
//               /                      \
//             /                          \
//           /                              \
//         /                                  \
//       /                                      \
//     /||                                      ||\
//      ||                                      ||
//      ||......................................||
static void SetGroupGelijkloop(TMotorgroup *pGroup)
{
int VirtualPositionA, VirtualPositionB;
int DesiredPositionA, DesiredPositionB;
int index = opt_app.Motorgroup[pGroup->Number].ControlIndex;

  //---------------------------------------------------------
  // Define A and B
  //---------------------------------------------------------
  if (pGroup->Number == opt_app.Cabriokas[index].GroupA)
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

  //---------------------------------------------------------
  // Adjust positions
  //---------------------------------------------------------
  if (VirtualPositionA < (opt_app.Cabriokas[index].Voorloop + opt_app.Cabriokas[index].Hysterese))
  {
    if ((VirtualPositionA + VirtualPositionB) < (2 * (opt_app.Cabriokas[index].Voorloop + opt_app.Cabriokas[index].Hysterese)))
    {
      DesiredPositionA = (VirtualPositionA + VirtualPositionB) / 2;
      DesiredPositionB = (VirtualPositionA + VirtualPositionB) / 2;
    }
    else
    {
      DesiredPositionA = opt_app.Cabriokas[index].Voorloop + opt_app.Cabriokas[index].Hysterese;
      DesiredPositionB = VirtualPositionB - DesiredPositionA;
    }
  }
  else if (VirtualPositionB < (opt_app.Cabriokas[index].Voorloop + opt_app.Cabriokas[index].Hysterese))
  {
    if ((VirtualPositionA + VirtualPositionB) < (2 * (opt_app.Cabriokas[index].Voorloop + opt_app.Cabriokas[index].Hysterese)))
    {
      DesiredPositionA = (VirtualPositionA + VirtualPositionB) / 2;
      DesiredPositionB = (VirtualPositionA + VirtualPositionB) / 2;
    }
    else
    {
      DesiredPositionB = opt_app.Cabriokas[index].Voorloop + opt_app.Cabriokas[index].Hysterese;
      DesiredPositionA = VirtualPositionA - DesiredPositionB;
    }
  }
  else
  {
    DesiredPositionA = VirtualPositionA;
    DesiredPositionB = VirtualPositionB;
  }

  //---------------------------------------------------------
  // Write new positions to Cabriokas
  //---------------------------------------------------------
  Cabriokas[index].PositionA = DesiredPositionA;
  Cabriokas[index].PositionB = DesiredPositionB;
}

//------------------------------------------------------------------------------
static void SetMotorsGelijkloop(TMotorgroup *pGroup)
{
TMotor *pMotor;
int DesiredPosition;
int MotorPosition;
int VoorloopHysterese;
int index = opt_app.Motorgroup[pGroup->Number].ControlIndex;
unsigned char HoldOpen, HoldClose;

  //---------------------------------------------------------
  // Define A and B
  //---------------------------------------------------------
  if (pGroup->Number == opt_app.Cabriokas[index].GroupA)
    DesiredPosition = Cabriokas[index].PositionA;
  else
    DesiredPosition = Cabriokas[index].PositionB;
  VoorloopHysterese = opt_app.Cabriokas[index].Voorloop + opt_app.Cabriokas[index].Hysterese;
  
  //---------------------------------------------------------
  // Set motor position
  //---------------------------------------------------------
  pMotor = pGroup->FirstMotor;
  while (pMotor != NULL)
  {
    if (pMotor->Link != NULL)
    {
      HoldOpen = HoldClose = 0;
      //HoldOpen  = pMotor->Flags.HoldOpen;
      //HoldClose = pMotor->Flags.HoldClose;
      MotorPosition = DesiredPosition;

      // Check if linked motor overruled
      if ((val_hr_alg.Motor[pMotor->Number].OperationMode == omAuto) && (MotorPosition < VoorloopHysterese))
      {
        if (val_hr_alg.Motor[pMotor->Link->Number].Overruled)
        {
          switch (pMotor->Link->RunningMode)
          {
            case rmStop :
              HoldOpen  = 1;
              HoldClose = 1;
              MotorPosition = val_hr_alg.Motor[pMotor->Number].Position;
              break;
            case rmOpen :
              MotorPosition = VoorloopHysterese;
              break;
            case rmClose:
              if (val_hr_alg.Motor[pMotor->Number].Position < opt_app.Cabriokas[index].Voorloop)
                MotorPosition = 0;
              else
                MotorPosition = VoorloopHysterese;
              break;
          }
        }
        else if (val_hr_alg.Motor[pMotor->Link->Number].OperationMode == omManual)
        {
          if (val_hr_alg.Motor[pMotor->Link->Number].PositionManual < VoorloopHysterese)
            MotorPosition = val_hr_alg.Motor[pMotor->Link->Number].PositionManual;
          else
            MotorPosition = VoorloopHysterese;
        }
      }

      // Prevent motor running faster than virtual motor
      if (!val_hr_alg.Motor[pMotor->Number].Overruled && (MotorPosition < VoorloopHysterese))
      {
        if (((val_hr_alg.Motorgroup[pGroup->Number].PositionPerc > 0) && (Motorgroup[pGroup->Number].RunningMode != rmStop)) ||
            ((val_hr_alg.Motorgroup[pGroup->Link->Number].PositionPerc > 0) && (Motorgroup[pGroup->Link->Number].RunningMode != rmStop)))
        {
          HoldOpen  = 1;
          HoldClose = 1;
        }
      }

      // Synchronise
      if (val_hr_alg.Motor[pMotor->Number].Overruled)
      {
        pMotor->Flags.Sync       = 1;
        pMotor->Link->Flags.Sync = 1;
      }
      if (pMotor->Flags.Sync)
      {
        if (MotorPosition > VoorloopHysterese)
        {
          pMotor->Flags.Sync       = 0;
          pMotor->Link->Flags.Sync = 0;
        }
        else
        {
          MotorPosition = VoorloopHysterese;
          if (((val_hr_alg.Motor[pMotor->Number].Position >= opt_app.Cabriokas[index].Voorloop) && (pMotor->RunningMode == rmStop)) &&
              ((val_hr_alg.Motor[pMotor->Link->Number].Position >= opt_app.Cabriokas[index].Voorloop) && (pMotor->Link->RunningMode == rmStop)))
          {
            pMotor->Flags.Sync       = 0;
            pMotor->Link->Flags.Sync = 0;
          }
        }
      }

      // Check to hold or release motor
      if (pMotor->Flags.HoldOpen || HoldOpen)
      {
        SetMotorHold(pMotor, rmOpen);
      }
      else if ((!val_hr_alg.Motor[pMotor->Number].Overruled) && (val_hr_alg.Motor[pMotor->Number].Position < VoorloopHysterese))
      { 
        if ((val_hr_alg.Motor[pMotor->Number].Position - val_hr_alg.Motor[pMotor->Link->Number].Position) >= opt_app.Cabriokas[index].Hysterese)
          SetMotorHold(pMotor, rmOpen);
        else if ((val_hr_alg.Motor[pMotor->Number].Position - val_hr_alg.Motor[pMotor->Link->Number].Position) <= 0)
          SetMotorRelease(pMotor, rmOpen);
      }
      else
      {
        SetMotorRelease(pMotor, rmOpen);
      }

      if (pMotor->Flags.HoldClose || HoldClose)
      {
        SetMotorHold(pMotor, rmClose);
      }
      else if ((!val_hr_alg.Motor[pMotor->Number].Overruled) && (val_hr_alg.Motor[pMotor->Number].Position < VoorloopHysterese))
      { 
        if ((val_hr_alg.Motor[pMotor->Link->Number].Position - val_hr_alg.Motor[pMotor->Number].Position) >= opt_app.Cabriokas[index].Hysterese)
          SetMotorHold(pMotor, rmClose);
        else if ((val_hr_alg.Motor[pMotor->Link->Number].Position - val_hr_alg.Motor[pMotor->Number].Position) <= 0)
          SetMotorRelease(pMotor, rmClose);
      }
      else
      {
        SetMotorRelease(pMotor, rmClose);
      }

      ClearAlarm(&alarm_hr_alg.Motor[pMotor->Number].LinkUnknown, MOTOR_LINK_UNKNOWN_AL, pMotor->Number);
    }
    else
    {
      MotorPosition = val_hr_alg.Motorgroup[pGroup->Number].PositionPerc;
      SetMotorHold(pMotor, rmOpen);
      SetMotorHold(pMotor, rmClose); // Hold both directions when opposite screen is unknown
      CreateAlarm(&alarm_hr_alg.Motor[pMotor->Number].LinkUnknown, MOTOR_LINK_UNKNOWN_AL, pMotor->Number, 0, opt_app.Motor[pMotor->Number].GroupNumber, HARD_ALARM);
    }
    if ((pGroup->Delay == 0) && (pGroup->Link->Delay == 0))
      val_hr_alg.Motor[pMotor->Number].PositionAuto = MotorPosition;
    pMotor = pMotor->Next;
  }
}
