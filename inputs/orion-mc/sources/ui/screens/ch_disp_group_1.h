// CH_DISP_GROUP_1.H

#ifndef _CH_DISP_GROUP_1_H
#define _CH_DISP_GROUP_1_H

#include "ct_disp.h"

extern s_screen screen_group_1;

extern unsigned char GroupIndex;
extern unsigned char MotorIndex;
extern int DeviceNr;

void Control_Screen_Group_1(void);
void If_Exist_Goto_Screen_Group_1(unsigned char nr);

void SetMotorScreen(void);

void Arrow_Motor_Up(void);
void Arrow_Motor_Down(void);

#endif