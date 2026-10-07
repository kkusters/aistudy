// CH_DISP_FUNC.H

#ifndef _CH_DISP_FUNC_H
#define _CH_DISP_FUNC_H

#include "ct_disp.h"

void Increment_Func_Index(void);
void Decrement_Func_Index(void);
void Increment_Func(void);
void Decrement_Func(void);

void Increment_Func_Index(void);
void Increment_Func_Index_Enter_Value(void);
void Increment_Func_Index_Enter_Option_Value(void);

char Search_Func_Index(unsigned int nr, unsigned char index);
char Search_Func(unsigned int nr);
void Search_Func_Or_First_True(unsigned int nr);
char Search_Func_Dont_Correct_Nr_Actief(unsigned int nr);

void Next_Screen(s_screen *new_screen);
void First_Screen(void);
void Prev_Screen(void);

void Arrow_Hoofd_Func(void);
void Arrow_Func(void);
void Arrow_Change_Func(void);
void Arrow_Change_Syst_Func(void);
void Arrow_Option_Func(void);

#endif