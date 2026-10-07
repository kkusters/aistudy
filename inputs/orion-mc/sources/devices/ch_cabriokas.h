// CH_CABRIOKAS.H

#ifndef _CH_CABRIOKAS_H
#define _CH_CABRIOKAS_H

#include <time.h>

#include "ct_data.h"
#include "ch_motorgroep.h"
#include "ch_timer.h"

//==============================================================================
//------------------------ Cabriokas - Typedefs --------------------------------
//==============================================================================
typedef struct
{
  unsigned char Number;
  unsigned char InitFlag;
  unsigned char BitmapA;
  unsigned char BitmapB;
  int PositionA;
  int PositionB;
  TTimer Timer_1min;
  void *GroupA;
  void *GroupB;
} TCabriokas;

extern TCabriokas Cabriokas[MAX_CABRIO];

//==============================================================================
//------------------------ Cabriokas - Initialisation --------------------------
//==============================================================================
void CreateCabriokas(TCabriokas *pCabrio, unsigned char Number);
void InitCabriokas(TCabriokas *pCabrio);

//==============================================================================
//------------------------ Cabriokas - Control ---------------------------------
//==============================================================================
void ControlCabriokas(TMotorgroup *pGroupA);

#endif
