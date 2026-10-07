// C__WDI.C

#include "ch_define.h"
#include "ch_wdi.h"

#define WDI_DP DP3_6
#define WDI_ODP ODP3_6

void WDI_Init(void)
{
#ifndef EMULATOR
  WDI = 1;
  WDI_DP = OUTPUT;
  WDI_ODP = PUSH_PULL;
  WDI_Trigger();
#endif // EMULATOR  
}