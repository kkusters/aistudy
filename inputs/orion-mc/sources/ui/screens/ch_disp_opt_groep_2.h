// CH_DISP_OPT_GROEP_2.H
					
#ifndef _CH_DISP_OPT_GROEP_2_H
#define _CH_DISP_OPT_GROEP_2_H

#include "ct_disp.h"

extern s_screen screen_opt_groep_2;

void CheckOptionsGroup(void);
void CheckOptionsMotor(void);
void CheckOptionsDevice(void);
void CheckOptionsFrequencyControl(void);
void CheckOptionsPulseSystem(void);

void Control_Screen_Option_Groep_2(unsigned char nr);

#endif