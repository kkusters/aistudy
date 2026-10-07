// CH_DISP_DIAG_GROEP_1.H

#ifndef _CH_DISP_DIAG_GROEP_1_H
#define _CH_DISP_DIAG_GROEP_1_H

#include "ct_disp.h"

extern s_screen screen_diag_group_1;

extern unsigned char DiagMotorIndex;

void If_Exist_Goto_Screen_Diag_Group_1(unsigned char nr);

void Arrow_Diag_Motor_Up(void);
void Arrow_Diag_Motor_Down(void);

#endif