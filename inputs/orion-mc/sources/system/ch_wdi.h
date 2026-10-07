// CH_WDI.H

#ifndef _CH_WDI_H
#define _CH_WDI_H

#include "ch_define.h"
#include "ch_test.h"

#define WDI P3_6

_inline void WDI_Trigger(void)
{
#ifndef EMULATOR
  WDI = ~WDI;
#endif // EMULATOR 
}

void WDI_Init(void);

#endif // _CH_WDI_H