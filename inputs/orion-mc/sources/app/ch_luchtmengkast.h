// CH_LUCHTMENGKAST.H

#ifndef _CH_LUCHTMENGKAST_H
#define _CH_LUCHTMENGKAST_H

#include <time.h>

//==============================================================================
//------------------------ Luchtmengkast - Typedefs ----------------------------
//==============================================================================
#define TYPE_KLEP_BINNEN_BUITEN 0
#define TYPE_KLEP_RECIRCULATIE  1

#define AFBLAASVENT_OP_ONDERDRUK       1
#define AFBLAASVENT_GEKOPPELD_AAN_KLEP 2

#define BOVENKLEP_OP_ONDERDRUK       1
#define BOVENKLEP_GEKOPPELD_AAN_KLEP 2

//==============================================================================
//------------------------ Luchtmengkast - Globals -----------------------------
//==============================================================================
typedef struct
{
  unsigned char AlarmTimer;
  unsigned char OldValue;
  unsigned char Correction;
  TTimer Timer_1min;
  TTimer Timer_5s;
} TLuchtmengkastSectie;

typedef struct
{
  TLuchtmengkastSectie Buitenklep;
  TLuchtmengkastSectie Binnenklep;
  TLuchtmengkastSectie Verwarming;
  TLuchtmengkastSectie Afblaasvent;
  TLuchtmengkastSectie Inblaasvent;

  TTimer Timer_1s;
  unsigned char CyclusTimerVerwarming;
} TLuchtmengkast;

typedef struct
{
  TTimer AfblaasventTimer_1s;
  TTimer BovenklepTimer_1s;
  unsigned char CyclusTimerAfblaasvent;
  unsigned char CyclusTimerBovenklep;
} TLuchtmengkastGroep;

extern TLuchtmengkast Luchtmengkast[MAX_LUCHTMENGKAST];

//==============================================================================
//------------------------ Luchtmengkast - Initialisation ----------------------
//==============================================================================
void LuchtmengkastInit(void);

//==============================================================================
//------------------------ Luchtmengkast - Main --------------------------------
//==============================================================================
void LuchtmengkastMain(void);

#endif
