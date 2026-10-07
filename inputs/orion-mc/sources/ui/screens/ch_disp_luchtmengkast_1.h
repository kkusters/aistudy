// CH_DISP_LUCHTMENGKAST_1.H

#ifndef _CH_DISP_LUCHTMENGKAST_1_H
#define _CH_DISP_LUCHTMENGKAST_1_H

#include "ct_disp.h"

extern s_screen screen_luchtmengkast_1;

extern unsigned char LuchtmengkastGroepIndex;
extern unsigned char LuchtmengkastGroepNummer;

void Prev_Next_Luchtmengkast_Func(void);

void Control_Screen_Luchtmengkast_1(void);
void If_Exist_Goto_Screen_Luchtmengkast_1(unsigned char nr);

#endif