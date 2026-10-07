// CH_DISP_OPTION_1.H

#ifndef _CH_DISP_OPTION_1_H
#define _CH_DISP_OPTION_1_H

#include "ch_const.h"
#include "ct_disp.h"

extern bit install_ana_in_voerweger_flag;
extern bit install_ana_in_dierweegschaal_flag;
extern bit install_ana_in_silo_flag;
extern bit install_ana_in_siloweger_flag;
extern bit install_ana_out_flag;
extern bit install_dig_out_flag;
extern bit option_change_flag;
extern unsigned char option_change_flag_alg;

extern s_disp_cursor disp_cursor;
extern s_disp_cursor const disp_cursor_array[17];
extern s_disp_block disp_block;
extern s_disp_block const disp_block_array[17];
extern s_bitmap const * const ico_IO_ana_in_array[ANA_IN_MAX];
extern s_bitmap const * const ico_IO_dig_in_array[DIG_IN_MAX];
extern s_bitmap const * const ico_IO_ana_out_array[ANA_OUT_MAX];
extern s_bitmap const * const ico_IO_dig_out_array[DIG_OUT_MAX];
extern s_bitmap const * const ico_IO_ana_high_in_array[ANA_HIGH_IN_MAX];
extern s_bitmap const * const ico_IO_motor_control_array[MOTOR_CONTROL_MAX];

extern s_disp_bitmap_array disp_IO[16];
extern s_disp_union disp_unit_IO;
//extern s_disp_bitmap disp_unit_IO;
extern s_disp_value disp_value_IO;
extern s_disp_tekst_array disp_string_IO;

extern s_disp_bitmap_array const disp_box_pos1_bmp;
extern s_disp_bitmap_array const disp_box_pos2_bmp;
extern s_disp_bitmap_array const disp_box_pos3_bmp;
extern s_disp_bitmap_array const disp_box_pos4_bmp;
extern s_disp_bitmap_array const disp_box_pos5_bmp;
extern s_disp_bitmap_array const disp_box_pos6_bmp;
extern s_disp_bitmap_array const disp_box_pos7_bmp;
extern s_disp_bitmap_array const disp_box_pos8_bmp;
extern s_disp_bitmap_array const disp_box_pos9_bmp;
extern s_disp_bitmap_array const disp_box_pos10_bmp;
extern s_disp_bitmap_array const disp_box_pos11_bmp;
extern s_disp_bitmap_array const disp_box_pos11_bmp;
extern s_disp_bitmap_array const disp_box_pos12_bmp;
extern s_disp_bitmap_array const disp_box_pos13_bmp;
extern s_disp_bitmap_array const disp_box_pos14_bmp;
extern s_disp_bitmap_array const disp_box_pos15_bmp;
extern s_disp_bitmap_array const disp_box_pos16_bmp;

extern s_disp_tekst const disp_dig_in_str;
extern s_disp_tekst const disp_dig_uit_str;
extern s_disp_tekst_add const disp_JA_str;

extern void * const lcd_start_disp[];
extern void * const lcd_end_disp[];

extern unsigned char key_val;
extern unsigned char key_min;
extern unsigned char key_max;
extern s_key_value key_IO;

extern unsigned char array[16];
extern unsigned char index_array;
extern unsigned char max_array_index;
extern unsigned char index_IO;
extern unsigned char start_flag;
extern unsigned char end_flag;

//*****************************************************************************

extern s_screen screen_option_1;

void Control_Screen_Option_1(void);
void Goto_Function_Index(unsigned char index);
void Arrow_Start_Func(void);
void Arrow_End_Func(void);

void CheckOptions(void);

unsigned char DummyNotUsed(s_board_IO_on_off IO_new);
unsigned char DigInNotUsed(s_board_IO_on_off IO_new);

void Set_Unit_IO_Ana_In(void);
void Set_Unit_IO_Ana_In_High(void);
void Set_Unit_IO_Ana_Out(void);
void Set_Unit_IO_Dig_In(void);
void Set_Unit_IO_Dig_Out(void);
void Set_Unit_IO_Motor_Control(void);

#endif