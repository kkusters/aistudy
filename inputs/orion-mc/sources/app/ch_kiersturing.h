// CH_KIERSTURING.H

#ifndef _CH_KIERSTURING_H
#define _CH_KIERSTURING_H

#include <time.h>

#include "ct_data.h"
#include "ch_motorgroep.h"
#include "ch_timer.h"

//==============================================================================
//------------------------ DualScreen - Typedefs -------------------------------
//==============================================================================
typedef struct
{
  unsigned char Number;
  unsigned char InitFlag;
  unsigned char Standby;
  unsigned char BitmapA;
  unsigned char BitmapB;
  int PositionA;
  int PositionB;
  TTimer Timer_1min;
  void *GroupA;
  void *GroupB;
} TDualScreen;

extern TDualScreen DualScreen[MAX_SCREEN];

//==============================================================================
//------------------------ Motorgroup - Initialisation -------------------------
//==============================================================================
void CreateDualScreen(TDualScreen *pScreen, unsigned char Number);
void InitDualScreen(TDualScreen *pScreen);

//==============================================================================
//------------------------ DualScreen - Control --------------------------------
//==============================================================================
void ControlDualScreen(TMotorgroup *pGroupA);

#endif
