// C__HTRAP.C
// Hardware trap routines
// NMI interrupt is triggered by MAX690A PFO (power failure output)

#include "ch_define.h"

#include "ch_alarm.h"
#include "ch_wdi.h"
#include "ch_htrap.h"

#define SYSSTAT_OSCLOCK 0x8000
#define SYSSTAT_PLLLOCK 0x4000
#define SYSSTAT_CLKHIX 0x2000
#define SYSSTAT_CLKLOX 0x1000
#define SYSSTAT_PLLEM 0x0400
#define SYSSTAT_HWR 0x0004
#define SYSSTAT_SWR 0x0002
#define SYSSTAT_WDTR 0x0001

#pragma noclear
s_htrap_error_cnt htrap_error_cnt;
#pragma clear
#pragma default_attributes

void Hardware_Trap_Init(void)
{
  htrap_error_cnt.reset++;
  if (htrap_error_cnt.check != 0x5A5A)
  {													    
    htrap_error_cnt.reset = htrap_error_cnt.pfo = 
    				        htrap_error_cnt.stkof = 
    					    htrap_error_cnt.stkuf =
    					    htrap_error_cnt.pacer =
    					    htrap_error_cnt.illopa =
    					    htrap_error_cnt.prtflt =
    					    htrap_error_cnt.undopc = 0;
    htrap_error_cnt.check = 0x5A5A;
  }
  // check if oscillator clock is correct
  // and set interrupt so that processor generates a interrupt by not a correct clock
  if (!(SYSSTAT & SYSSTAT_PLLLOCK))
    while (1);
  PLLIC = PLL_OWD_INT_LEVEL;
  PLLIC_IR = 0;
  PLLIC_IE = 1;
}
											  
interrupt PFO_INT_ADR void PFO_Int(void) // NMI
// NMI interrupt is triggered by MAX690A PFO (power failure output)
{
  NMI = 0;
  htrap_error_cnt.pfo++;
}

interrupt STACK_OVERFLOW_INT_ADR void Stack_Overflow_int(void)
{
  STKOF = 0;
  htrap_error_cnt.stkof++;
  CreateAlarm(&alarm_hr_alg.htrap_al, SYSTEEM_AL_HTRAP, 0, 1, 0, ZACHT_ALARM);
  while (1); // lets restart the program
}

interrupt STACK_UNDERFLOW_INT_ADR void Stack_Underflow_int(void)
{
  STKUF = 0;
  htrap_error_cnt.stkuf++;
  CreateAlarm(&alarm_hr_alg.htrap_al, SYSTEEM_AL_HTRAP, 0, 2, 0, ZACHT_ALARM);
  while (1); // lets restart the program
}

interrupt CLASS_B_TRAP_INT_ADR void Class_B_Hardware_Trap_int(void)
{
int help = 20;

  if (ILLOPA) // Illegal word operand access flag
  {
    help = 21;
	ILLOPA = 0;
    htrap_error_cnt.illopa++;
  }
  if (PRTFLT) // Protection fault flag
  {
    help = 22;
	PRTFLT = 0;
    htrap_error_cnt.prtflt++;
  }
  if (PACER) // Program memory access error
  {
    help = 23;
    PACER = 0;
    htrap_error_cnt.pacer++;
  }
  if (UNDOPC) // Undefined opcode flag
  {
    help = 24;
	UNDOPC = 0;
    htrap_error_cnt.undopc++;
  }
  CreateAlarm(&alarm_hr_alg.htrap_al, SYSTEEM_AL_HTRAP, 0, help, 0, ZACHT_ALARM);
  while (1); // lets restart the program
}

interrupt PLL_OWD_INT_ADR using(PLL_OWD_INT_RB) void PLL_OWD_RTC_Int(void)
{
  CreateAlarm(&alarm_hr_alg.PLL_al, SYSTEEM_AL_PLL, 0, 0, 0, ZACHT_ALARM);
  while (1);
}
