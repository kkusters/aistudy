// CH_DISP_OPT_IO_SELECT.H
					
#ifndef _CH_DISP_OPT_IO_SELECT_H
#define _CH_DISP_OPT_IO_SELECT_H

#include "ct_disp.h"

extern int board_IO_max;
extern int board_IO_max_nr;
extern int board_IO_array_size;
extern s_board_IO_on_off board_IO[16];

extern s_disp_bitmap_array const disp_box_IO_pos1_bmp;
extern s_disp_bitmap_array const disp_box_IO_pos2_bmp;
extern s_disp_bitmap_array const disp_box_IO_pos3_bmp;
extern s_disp_bitmap_array const disp_box_IO_pos4_bmp;
extern s_disp_bitmap_array const disp_box_IO_pos5_bmp;
extern s_disp_bitmap_array const disp_box_IO_pos6_bmp;
extern s_disp_bitmap_array const disp_box_IO_pos7_bmp;
extern s_disp_bitmap_array const disp_box_IO_pos8_bmp;
extern s_disp_bitmap_array const disp_box_IO_pos9_bmp;
extern s_disp_bitmap_array const disp_box_IO_pos10_bmp;
extern s_disp_bitmap_array const disp_box_IO_pos11_bmp;
extern s_disp_bitmap_array const disp_box_IO_pos12_bmp;
extern s_disp_bitmap_array const disp_box_IO_pos13_bmp;
extern s_disp_bitmap_array const disp_box_IO_pos14_bmp;
extern s_disp_bitmap_array const disp_box_IO_pos15_bmp;
extern s_disp_bitmap_array const disp_box_IO_pos16_bmp;

extern s_disp_bitmap_array const disp_IO_vink_pos1_bmp;
extern s_disp_bitmap_array const disp_IO_vink_pos2_bmp;
extern s_disp_bitmap_array const disp_IO_vink_pos3_bmp;
extern s_disp_bitmap_array const disp_IO_vink_pos4_bmp;
extern s_disp_bitmap_array const disp_IO_vink_pos5_bmp;
extern s_disp_bitmap_array const disp_IO_vink_pos6_bmp;
extern s_disp_bitmap_array const disp_IO_vink_pos7_bmp;
extern s_disp_bitmap_array const disp_IO_vink_pos8_bmp;
extern s_disp_bitmap_array const disp_IO_vink_pos9_bmp;
extern s_disp_bitmap_array const disp_IO_vink_pos10_bmp;
extern s_disp_bitmap_array const disp_IO_vink_pos11_bmp;
extern s_disp_bitmap_array const disp_IO_vink_pos12_bmp;
extern s_disp_bitmap_array const disp_IO_vink_pos13_bmp;
extern s_disp_bitmap_array const disp_IO_vink_pos14_bmp;
extern s_disp_bitmap_array const disp_IO_vink_pos15_bmp;
extern s_disp_bitmap_array const disp_IO_vink_pos16_bmp;
/*
extern s_disp_value_option_on const disp_IO_nr_pos1;
extern s_disp_value_option_on const disp_IO_nr_pos2;
extern s_disp_value_option_on const disp_IO_nr_pos3;
extern s_disp_value_option_on const disp_IO_nr_pos4;
extern s_disp_value_option_on const disp_IO_nr_pos5;
extern s_disp_value_option_on const disp_IO_nr_pos6;
extern s_disp_value_option_on const disp_IO_nr_pos7;
extern s_disp_value_option_on const disp_IO_nr_pos8;
extern s_disp_value_option_on const disp_IO_nr_pos9;
extern s_disp_value_option_on const disp_IO_nr_pos10;
extern s_disp_value_option_on const disp_IO_nr_pos11;
extern s_disp_value_option_on const disp_IO_nr_pos12;
extern s_disp_value_option_on const disp_IO_nr_pos13;
extern s_disp_value_option_on const disp_IO_nr_pos14;
extern s_disp_value_option_on const disp_IO_nr_pos15;
extern s_disp_value_option_on const disp_IO_nr_pos16;
*/
extern s_disp_value_2_size_option_on const disp_IO_nr_pos1;
extern s_disp_value_2_size_option_on const disp_IO_nr_pos2;
extern s_disp_value_2_size_option_on const disp_IO_nr_pos3;
extern s_disp_value_2_size_option_on const disp_IO_nr_pos4;
extern s_disp_value_2_size_option_on const disp_IO_nr_pos5;
extern s_disp_value_2_size_option_on const disp_IO_nr_pos6;
extern s_disp_value_2_size_option_on const disp_IO_nr_pos7;
extern s_disp_value_2_size_option_on const disp_IO_nr_pos8;
extern s_disp_value_2_size_option_on const disp_IO_nr_pos9;
extern s_disp_value_2_size_option_on const disp_IO_nr_pos10;
extern s_disp_value_2_size_option_on const disp_IO_nr_pos11;
extern s_disp_value_2_size_option_on const disp_IO_nr_pos12;
extern s_disp_value_2_size_option_on const disp_IO_nr_pos13;
extern s_disp_value_2_size_option_on const disp_IO_nr_pos14;
extern s_disp_value_2_size_option_on const disp_IO_nr_pos15;
extern s_disp_value_2_size_option_on const disp_IO_nr_pos16;

extern s_disp_tekst const disp_board_str;
extern s_disp_tekst_array const disp_NO_IO;

void Install_Copy_IO_To_Board_IO(s_board_IO_on_off *IO,
                                 unsigned char max,
                                 unsigned char max_nr,
								 unsigned char on_off,
                                 unsigned char IO_type,
                                 unsigned char IO_type_sel,
                                 unsigned char (*NotUsedFunc)(s_board_IO_on_off IO_new));
void Install_Copy_IO_To_Board_IO_Range(s_board_IO_on_off *IO,
                                       unsigned char max,
                                       unsigned char max_nr,
								       unsigned char on_off,
                                       unsigned char IO_type,
                                       unsigned char IO_type_sel_first,
                                       unsigned char IO_type_sel_last,
                                       unsigned char (*NotUsedFunc)(s_board_IO_on_off IO_new));

void Install_Copy_Board_IO_To_IO(void);
void Install_Control_All_Board_IO(void);
void Install_Control_Board_IO(s_board_IO_on_off *IO,
                              unsigned char max,
                              unsigned char max_nr,
                              unsigned char on_off,
                              unsigned char IO_type,
                              unsigned char IO_type_sel);

unsigned char Board_IO_Used(s_board_IO_on_off IO_new, s_board_IO_on_off *IO_array, unsigned char array_size);
unsigned char Board_IO_On_Off(s_board_IO_on_off IO_new, s_board_IO_on_off *IO_array, unsigned char array_size);

void Arrow_Left_IO_Select(void);
void Arrow_Right_IO_Select(void);
void Arrow_Up_IO_Select(void);
void Arrow_Down_IO_Select(void);
void Arrow_IO_Select_Func(void);
void Enter_IO_Select_Func(void);
void Arrow_IO_Select_No_Shift_Func(void);

void Install_Copy_Option_To_Board_IO(unsigned char *opt, unsigned char max, unsigned char max_nr);
void Install_Copy_Board_IO_To_Option(void);

#endif