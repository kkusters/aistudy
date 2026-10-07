// CH_LCD_10.H

#ifndef _CH_LCD_10_H
#define _CH_LCD_10_H

#include "ct_disp.h"

extern s_bitmap const * const ico_10_graden[];
extern s_bitmap const ico_10_slot_open;
extern s_bitmap const ico_10_slot_dicht;
extern s_disp_bitmap_array const disp_10_slot;

void LCD_Draw_ASCII_Multi_10_L(char c);
void LCD_Draw_ASCII_Multi_10_R(char c);
void LCD_Draw_ASCII_Rus_10_L(char c);
void LCD_Draw_ASCII_Rus_10_R(char c);
void LCD_Draw_ASCII_EE_10_L(char c);
void LCD_Draw_ASCII_EE_10_R(char c);

#endif
