// CH_DISP.H

#ifndef _CH_DISP_H
#define _CH_DISP_H

#include "ct_disp.h"

extern s_screen *screen_ptr;

extern unsigned char block_display_cursor_block; // blokkeer weergeven blok cursor voor 3 seconden

void Control_Screen(s_screen *screen_ptr, s_screen const *default_screen_ptr, 
                    unsigned char opnieuw, unsigned char rel_aantal);

void Init_All_Screen(void);
void Refresh_Screen_Nr_Aantal(void);

void Display_Key_Proc(void);


#endif