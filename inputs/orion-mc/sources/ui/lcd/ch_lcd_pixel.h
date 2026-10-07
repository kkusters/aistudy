// CH_LCD_PIXEL.H

#ifndef _CH_LCD_PIXEL_H
#define _CH_LCD_PIXEL_H

#include "ct_disp.h"

extern s_disp_cursor const disp_cursor_56_23_8;
extern s_disp_cursor const disp_cursor_57_23_28;
extern s_disp_cursor const disp_cursor_80_23_8;
extern s_disp_cursor const disp_cursor_88_52_8;
extern s_disp_cursor const disp_cursor_88_78_8;
extern s_disp_cursor const disp_cursor_91_23_8;
extern s_disp_cursor const disp_cursor_106_61_50;
extern s_disp_cursor const disp_cursor_127_78_8;
extern s_disp_cursor const disp_cursor_135_23_8;
extern s_disp_cursor const disp_cursor_142_25_8;
extern s_disp_cursor const disp_cursor_147_23_8;
extern s_disp_cursor const disp_cursor_153_52_8;
extern s_disp_cursor const disp_cursor_153_78_8;
extern s_disp_cursor const disp_cursor_147_78_8;
extern s_disp_cursor const disp_cursor_150_52_142;
extern s_disp_cursor const disp_cursor_157_23_28;
extern s_disp_cursor const disp_cursor_157_52_8;
extern s_disp_cursor const disp_cursor_161_23_8;
extern s_disp_cursor const disp_cursor_161_78_8;
extern s_disp_cursor const disp_cursor_170_23_8;
extern s_disp_cursor const disp_cursor_171_23_8;
extern s_disp_cursor const disp_cursor_172_78_8;
extern s_disp_cursor const disp_cursor_183_23_8;
extern s_disp_cursor const disp_cursor_183_78_8;
extern s_disp_cursor const disp_cursor_186_25_150;
extern s_disp_cursor const disp_cursor_192_52_8;
extern s_disp_cursor const disp_cursor_192_78_8;
extern s_disp_cursor const disp_cursor_221_78_45;
extern s_disp_cursor const disp_cursor_193_61_8;
extern s_disp_cursor const disp_cursor_193_81_8;
extern s_disp_cursor const disp_cursor_194_23_8;
extern s_disp_cursor const disp_cursor_221_25_185;
extern s_disp_cursor const disp_cursor_207_23_8;
extern s_disp_cursor const disp_cursor_207_25_8;
extern s_disp_cursor const disp_cursor_207_27_18;
extern s_disp_cursor const disp_cursor_207_44_8;
extern s_disp_cursor const disp_cursor_207_52_8;
extern s_disp_cursor const disp_cursor_207_53_8;
extern s_disp_cursor const disp_cursor_207_63_8;
extern s_disp_cursor const disp_cursor_207_78_8;
extern s_disp_cursor const disp_cursor_207_82_8;
extern s_disp_cursor const disp_cursor_210_23_8;    
extern s_disp_cursor const disp_cursor_212_23_8;    
extern s_disp_cursor const disp_cursor_215_23_8;    
extern s_disp_cursor const disp_cursor_215_23_70;
extern s_disp_cursor const disp_cursor_220_23_8;
extern s_disp_cursor const disp_cursor_225_23_120;
extern s_disp_cursor const disp_cursor_225_23_90;
extern s_disp_cursor const disp_cursor_225_78_150;
extern s_disp_cursor const disp_cursor_225_78_180;

extern s_disp_cursor const disp_cursor_214_78_151;

extern s_disp_cursor const disp_cursor_checkbox_207_25;

extern s_disp_cursor const disp_cursor_checkbox;
extern s_disp_cursor const disp_cursor_curve_dag;
extern s_disp_cursor const disp_cursor_curve_val;
extern s_disp_cursor const disp_cursor_curve_hour;
extern s_disp_cursor const disp_cursor_curve_min; 
extern s_disp_cursor const disp_cursor_curve_aantal;
extern s_disp_cursor const disp_cursor_password;
extern s_disp_cursor const disp_cursor_password_1;
extern s_disp_cursor const disp_cursor_password_2;

extern s_disp_cursor const disp_cursor_operation;

extern bit cursor_state;

extern s_disp_loper const disp_loper;

void LCD_Invert_Block(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2);
void LCD_Draw_Bitmap(unsigned char x, unsigned char y, s_bitmap const *bitmap);
void LCD_Draw_Character(unsigned char x ,unsigned char y, s_character const *character);
void LCD_Draw_Black_Vierkant(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2);
void LCD_Draw_White_Block(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2);

void Disp_Cursor(s_disp_cursor const *ptr);
void Disp_Cursor_PC(s_disp_cursor const *ptr);

void Disp_Draw_Bitmap(void *s);
void Disp_Draw_Bitmap_Option_On(void *s);
//void Disp_Draw_Bitmap_Option_Off(void *s);
void Disp_Draw_Bitmap_Array(void *s);
void Disp_Draw_Bitmap_Array_Option_On(void *s);
//void Disp_Draw_Bitmap_Array_Option_Off(void *s);
void Disp_Draw_Bitmap_Invert(void *s);
void Disp_Draw_Bitmap_Option_On_Invert(void *);
void Disp_Draw_Bitmap_Option_On_Invert_Add(void *);
void Disp_Draw_Value(void *s);
void Disp_Draw_Value_Signed(void *s);
void Disp_Draw_Value_2_Size_No_Point(void *s);
void Disp_Draw_Value_2_Size_Option_On(void *s);
void Disp_Draw_Value_Add(void *s);
void Disp_Draw_Value_Option_On(void *s);
void Disp_Draw_Value_Option_Off(void *s);
void Disp_Draw_Value_Add_Option_On(void *s);
void Disp_Draw_Black_Block(void *s);
void Disp_Draw_Black_Vierkant(void *s);
void Disp_Draw_White_Block(void *s);
void Disp_Draw_White_Vierkant(void *s);
void Disp_Invert_Block(void *s);
void Disp_Draw_Code(void *s);
void Disp_Draw_Component(void *s);
void Disp_Draw_Component_Option_On(void *s);
void Disp_Draw_Component_Array(void *s);
void Disp_Draw_Component_Array_Option_On(void *s);
void Disp_Draw_Component_Abs(void *s);
void Disp_Draw_Component_Abs_Option_On(void *s);
void Disp_Draw_Balk(void *s);
void Disp_Draw_Time(void *s);
void Disp_Draw_Urenteller(void *s);
void Disp_Control_Func(void *s);

void Disp_Draw_Board_IO(void *s);
void Disp_Draw_Board_IO_Select(void *s);
void Disp_Draw_Board_IO_Select_Option_On(void *s);
void Disp_Draw_Board_IO_Select_Option_Off(void *s);

void Disp_Draw_Array_Select(void *s);

void Disp_Draw_Tekst_L(void *s);
void Disp_Draw_Tekst_R(void *s);
void Disp_Draw_Tekst_L_Option_On(void *s);
void Disp_Draw_Tekst_R_Option_On(void *s);
void Disp_Draw_Tekst_L_Option_Off(void *s);
//void Disp_Draw_Tekst_R_Option_Off(void *s);
void Disp_Draw_Tekst_Add_L(void *s);
void Disp_Draw_Tekst_Add_R(void *s);
void Disp_Draw_Tekst_Add_L_Option_On(void *s);
void Disp_Draw_Tekst_Add_R_Option_On(void *s);
void Disp_Draw_Tekst_Array_L(void *s);
void Disp_Draw_Tekst_Array_R(void *s);
void Disp_Draw_Tekst_Array_L_Option_On(void *s);
void Disp_Draw_Tekst_Array_R_Option_On(void *s);
//void Disp_Draw_Tekst_Array_L_Option_Off(void *s);
void Disp_Draw_Tekst_Array_R_Option_Off(void *s);
void Disp_Draw_Tekst_Array_Add_L(void *s);
void Disp_Draw_Tekst_Array_Add_R(void *s);
void Disp_Draw_Tekst_Array_Add_L_Option_On(void *s);
void Disp_Draw_Tekst_Array_Add_R_Option_On(void *s);

void Disp_Draw_Agri_Header(void *s);
void Disp_Draw_Alarm_State(void);
#endif
