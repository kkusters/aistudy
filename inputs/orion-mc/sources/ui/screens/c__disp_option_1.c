// C__DISP_OPTION_1.C

#include "ch_define.h"

#include "ch_const.h"
#include "ch_disp.h"
#include "ch_disp_func.h"
#include "ch_disp_option_0.h"
#include "ch_disp_opt_alg_2.h"
#include "ch_disp_opt_alarm_2.h"
#include "ch_disp_opt_cabriokas_2.h"
#include "ch_disp_opt_groep_2.h"
#include "ch_disp_opt_IO_2.h"
#include "ch_disp_opt_IO_Select.h"
#include "ch_disp_opt_kiersturing_2.h"
#include "ch_disp_opt_luchtmengkast_2.h"
#include "ch_disp_opt_luchting_2.h"
#include "ch_disp_opt_motorgroepen_2.h"
#include "ch_disp_opt_sensoren_2.h"
#include "ch_disp_opt_vrijgave_vent_2.h"
#include "ch_disp_password.h"
#include "ch_disp_value.h"
#include "ch_key.h"
#include "ch_lcd_10.h"
#include "ch_lcd_14.h"
#include "ch_lcd_bitmap.h"
#include "ch_lcd_pixel.h"
#include "ch_main.h"
#include "ch_motorgroep.h"
#include "ch_pc_com.h"
#include "ch_string.h"
#include "ch_disp_option_1.h"

static void Arrow_Algemeen_Func(void);
static void Arrow_Board_Func(void);
static void Arrow_Motorgroepen_Func(void);
static void Arrow_Groep_Func(void);
static void Arrow_Vrijgave_Vent_Func(void);
static void Arrow_2_Doek_1_Bed_Func(void);
static void Arrow_Cabriokas_Func(void);
static void Arrow_Luchtmengkast_Func(void);
static void Arrow_Sensoren_Func(void);
static void Arrow_Luchting_Func(void);
static void Arrow_Alarm_Func(void);

s_disp_cursor disp_cursor = { 255, 0, 0, 0, 0};
s_disp_cursor const disp_cursor_array[17] = 
{
  { 255, 0, 0, 0, 0}, //  0 geen cursor
  {  18,53,18, 0,15}, //  1 cursor in veld  1  
  {  37,53,18, 0,15}, //  2 cursor in veld  2  
  {  56,53,18, 0,15}, //  3 cursor in veld  3  
  {  75,53,18, 0,15}, //  4 cursor in veld  4  
  {  94,53,18, 0,15}, //  5 cursor in veld  5  
  { 113,53,18, 0,15}, //  6 cursor in veld  6  
  { 132,53,18, 0,15}, //  7 cursor in veld  7  
  { 151,53,18, 0,15}, //  8 cursor in veld  8  
  { 170,53,18, 0,15}, //  9 cursor in veld  9  
  { 189,53,18, 0,15}, // 10 cursor in veld 10 
  { 208,53,18, 0,15}, // 11 cursor in veld 11 
  { 227,53,18, 0,15}, // 12 cursor in veld 12 
  {  18,80,18, 0,15}, // 13 cursor in veld 13  
  {  37,80,18, 0,15}, // 14 cursor in veld 14  
  {  56,80,18, 0,15}, // 15 cursor in veld 15  
  {  75,80,18, 0,15}, // 16 cursor in veld 16  
};
unsigned char array[16];
unsigned char index_array = 0;
unsigned char max_array_index;
unsigned char index_IO = 16;
unsigned char start_flag = 0;
unsigned char end_flag = 0;
bit install_ana_in_voerweger_flag = 0;
bit install_ana_in_dierweegschaal_flag = 0;
bit install_ana_in_silo_flag = 0;
bit install_ana_in_siloweger_flag = 0;
bit install_ana_out_flag = 0;
bit install_dig_out_flag = 0;
bit option_change_flag = 0;
unsigned char option_change_flag_alg = 0;
s_disp_block disp_block = { Disp_Invert_Block, 0, 0, 10, 10};
s_disp_block const disp_block_array[17] =
{
  { Disp_Invert_Block, 255, 0,  0, 0}, //  0 geen nummer veld geinverteerd
  { Disp_Invert_Block,   1,29, 18,38}, //  1 nummer veld  1 geinverteerd
  { Disp_Invert_Block,  20,29, 37,38}, //  2 nummer veld  2 geinverteerd
  { Disp_Invert_Block,  39,29, 56,38}, //  3 nummer veld  3 geinverteerd
  { Disp_Invert_Block,  58,29, 75,38}, //  4 nummer veld  4 geinverteerd
  { Disp_Invert_Block,  77,29, 94,38}, //  5 nummer veld  5 geinverteerd
  { Disp_Invert_Block,  96,29,113,38}, //  6 nummer veld  6 geinverteerd
  { Disp_Invert_Block, 115,29,132,38}, //  7 nummer veld  7 geinverteerd
  { Disp_Invert_Block, 134,29,151,38}, //  8 nummer veld  8 geinverteerd
  { Disp_Invert_Block, 153,29,170,38}, //  9 nummer veld  9 geinverteerd
  { Disp_Invert_Block, 172,29,189,38}, // 10 nummer veld 10 geinverteerd
  { Disp_Invert_Block, 191,29,208,38}, // 11 nummer veld 11 geinverteerd
  { Disp_Invert_Block, 210,29,227,38}, // 12 nummer veld 12 geinverteerd
  { Disp_Invert_Block,   1,56, 18,65}, // 13 nummer veld 13 geinverteerd
  { Disp_Invert_Block,  20,56, 37,65}, // 14 nummer veld 14 geinverteerd
  { Disp_Invert_Block,  39,56, 56,65}, // 15 nummer veld 15 geinverteerd
  { Disp_Invert_Block,  58,56, 75,65}, // 16 nummer veld 16 geinverteerd
};
s_bitmap const * const ico_IO_ana_in_array[ANA_IN_MAX] =
{
  &ico_empty,
  &ico_IO_temperatuur,
  &ico_IO_Pa,
  &ico_IO_RV,
  &ico_IO_CO2,
  &ico_IO_windrichting,
  &ico_IO_windsnelheid,
  &ico_IO_voerweger,
  &ico_IO_dierweegschaal,
  &ico_IO_dierweegschaal,
  &ico_IO_raam,
  &ico_IO_doek,
  &ico_IO_vent,
  &ico_IO_lamel,
  &ico_IO_verwarm,
  &ico_IO_koel,
  &ico_IO_alarm,
  &ico_IO_klep
};
s_bitmap const * const ico_IO_dig_in_array[DIG_IN_MAX] =
{
  &ico_empty,
  &ico_IO_water,
  &ico_IO_voer_puls,
  &ico_IO_voerweger,
  &ico_IO_ei_puls,
  &ico_IO_ei,
  &ico_IO_voer,
  &ico_IO_klok,
  &ico_IO_alarm,
  &ico_IO_koel,
  &ico_IO_kWh_puls,
  &ico_IO_vent,
  &ico_IO_raam,
  &ico_IO_doek,
  &ico_IO_vent,
  &ico_IO_lamel,
  &ico_IO_verwarm,
  &ico_IO_klep
};
s_bitmap const * const ico_IO_ana_out_array[ANA_OUT_MAX] =
{
  &ico_empty,
  &ico_IO_vent,
  &ico_IO_klep,
  &ico_IO_verwarm,
  &ico_IO_licht,
  &ico_IO_uni_reg,
  &ico_IO_mestdrg,
  &ico_IO_ei,
  &ico_IO_raam,
  &ico_IO_doek,
  &ico_IO_lamel
};
s_bitmap const * const ico_IO_dig_out_array[DIG_OUT_MAX] =
{
  &ico_empty,
  &ico_IO_vent,
  &ico_IO_klep,
  &ico_IO_verwarm,
  &ico_IO_koel,
  &ico_IO_RV,
  &ico_IO_klok,
  &ico_IO_licht,
  &ico_IO_uni_reg,
  &ico_IO_tunnel,
  &ico_IO_water,
  &ico_IO_voer,
  &ico_IO_voer_puls,
  &ico_IO_voerweger,
  &ico_IO_mestdrg,
  &ico_IO_ei,
  &ico_IO_hopper,
  &ico_IO_alarm,
  &ico_IO_silo,
  &ico_IO_lamel
};
s_bitmap const * const ico_IO_ana_high_in_array[ANA_HIGH_IN_MAX] =
{
  &ico_empty,
  &ico_IO_dierweegschaal,
  &ico_IO_silo,
};
s_bitmap const * const ico_IO_motor_control_array[MOTOR_CONTROL_MAX] =
{
  &ico_empty,
  &ico_IO_raam,
  &ico_IO_doek
};

s_disp_bitmap_array disp_IO[16];
s_disp_union disp_unit_IO;
//s_disp_bitmap disp_unit_IO;
s_disp_value disp_value_IO;
s_disp_tekst_array disp_string_IO;

static unsigned char option_ventilation;
static unsigned char option_screen;
static unsigned char option_window;

s_bitmap const * const bmp_boxes[16] = { &ico_box_1, &ico_box_2, &ico_box_3, &ico_box_4, &ico_box_5, &ico_box_6, &ico_box_7, &ico_box_8,
                                  &ico_box_9, &ico_box_10, &ico_box_11, &ico_box_12, &ico_box_13, &ico_box_14, &ico_box_15, &ico_box_16 };

//*****************************************************************************
// schermen voor algemeen gebruik binnen de opties
static s_disp_tekst const disp_opties_doorlopen_str = { Disp_Draw_Tekst_L, 0, 22, &tekst_inst.Opties_Doorlopen_14 };
static s_disp_tekst const disp_opties_OK_str =        { Disp_Draw_Tekst_L, 0, 22, &tekst_inst.Opties_OK_14 };

static s_disp_bitmap const disp_arrow_left = { Disp_Draw_Bitmap,  37, 61, &ico_arrow_left_14 };
static s_disp_tekst const disp_is_teken_0  = { Disp_Draw_Tekst_L,   60, 75, &tekst_is_14 };
static s_disp_tekst const disp_is_teken_1  = { Disp_Draw_Tekst_L,  160, 75, &tekst_is_14 };
static s_disp_tekst_add const disp_NEE_str = { Disp_Draw_Tekst_Add_L, &tekst_inst.NEE_14 };
s_disp_tekst_add const disp_JA_str  = { Disp_Draw_Tekst_Add_L, &tekst_inst.JA_14 };
static s_disp_bitmap const disp_arrow_down = { Disp_Draw_Bitmap,  137,61, &ico_arrow_down_14 };
static s_disp_bitmap const disp_arrow_up   = { Disp_Draw_Bitmap,  137,61, &ico_arrow_up_14 };
static s_disp_tekst_add const disp_OK_str  = { Disp_Draw_Tekst_Add_L, &tekst_ok_14};

s_disp_bitmap_array const disp_box_pos1_bmp =  { Disp_Draw_Bitmap_Array,   0, 28, bmp_boxes, UCHAR, &uchar_0,  16 };
s_disp_bitmap_array const disp_box_pos2_bmp =  { Disp_Draw_Bitmap_Array,  19, 28, bmp_boxes, UCHAR, &uchar_1,  16 };
s_disp_bitmap_array const disp_box_pos3_bmp =  { Disp_Draw_Bitmap_Array,  38, 28, bmp_boxes, UCHAR, &uchar_2,  16 };
s_disp_bitmap_array const disp_box_pos4_bmp =  { Disp_Draw_Bitmap_Array,  57, 28, bmp_boxes, UCHAR, &uchar_3,  16 };
s_disp_bitmap_array const disp_box_pos5_bmp =  { Disp_Draw_Bitmap_Array,  76, 28, bmp_boxes, UCHAR, &uchar_4,  16 };
s_disp_bitmap_array const disp_box_pos6_bmp =  { Disp_Draw_Bitmap_Array,  95, 28, bmp_boxes, UCHAR, &uchar_5,  16 };
s_disp_bitmap_array const disp_box_pos7_bmp =  { Disp_Draw_Bitmap_Array, 114, 28, bmp_boxes, UCHAR, &uchar_6,  16 };
s_disp_bitmap_array const disp_box_pos8_bmp =  { Disp_Draw_Bitmap_Array, 133, 28, bmp_boxes, UCHAR, &uchar_7,  16 };
s_disp_bitmap_array const disp_box_pos9_bmp =  { Disp_Draw_Bitmap_Array, 152, 28, bmp_boxes, UCHAR, &uchar_8,  16 };
s_disp_bitmap_array const disp_box_pos10_bmp = { Disp_Draw_Bitmap_Array, 171, 28, bmp_boxes, UCHAR, &uchar_9,  16 };
s_disp_bitmap_array const disp_box_pos11_bmp = { Disp_Draw_Bitmap_Array, 190, 28, bmp_boxes, UCHAR, &uchar_10, 16 };
s_disp_bitmap_array const disp_box_pos12_bmp = { Disp_Draw_Bitmap_Array, 209, 28, bmp_boxes, UCHAR, &uchar_11, 16 };
s_disp_bitmap_array const disp_box_pos13_bmp = { Disp_Draw_Bitmap_Array,   0, 55, bmp_boxes, UCHAR, &uchar_12, 16 };
s_disp_bitmap_array const disp_box_pos14_bmp = { Disp_Draw_Bitmap_Array,  19, 55, bmp_boxes, UCHAR, &uchar_13, 16 };
s_disp_bitmap_array const disp_box_pos15_bmp = { Disp_Draw_Bitmap_Array,  38, 55, bmp_boxes, UCHAR, &uchar_14, 16 };
s_disp_bitmap_array const disp_box_pos16_bmp = { Disp_Draw_Bitmap_Array,  57, 55, bmp_boxes, UCHAR, &uchar_15, 16 };

s_disp_tekst const disp_dig_in_str =  { Disp_Draw_Tekst_L, 37, 22, &tekst_inst.Digitale_Ingang_14 };
s_disp_tekst const disp_dig_uit_str = { Disp_Draw_Tekst_L, 47, 22, &tekst_inst.Digitale_Uitgang_14 };

void * const lcd_start_disp[] = { &disp_opties_doorlopen_str, 
                                  &disp_arrow_left, &disp_is_teken_0, &disp_space_14_L, &disp_NEE_str,
                                  &disp_arrow_down, &disp_is_teken_1, &disp_space_14_L, &disp_JA_str, 0 };
void * const lcd_end_disp[] = { &disp_opties_OK_str, 
                                &disp_arrow_left, &disp_is_teken_0, &disp_space_14_L, &disp_JA_str,
                                &disp_arrow_up, &disp_is_teken_1, &disp_space_14_L, &disp_NEE_str, 0 };

unsigned char key_val;
unsigned char key_min;
unsigned char key_max;
s_key_value key_IO = { UCHAR, 1, &key_val, &key_min, &key_max};

//*****************************************************************************


static s_disp_agri_header const disp_header = { Disp_Draw_Agri_Header, FN6, EMPTY, &tekst_inst.Opties_10, HK_GEEN, 0, 1};
static void * const lcd_disp_header[] = { &disp_header, 0 };

static s_disp_tekst const disp_algemeen_string    = { Disp_Draw_Tekst_L, 37, 22, &tekst_inst.Algemeen_14 };
static s_disp_tekst const disp_IO_string          = { Disp_Draw_Tekst_L, 37, 22, &tekst_inst.IO_14 };
static s_disp_tekst const disp_motorgroepen_str   = { Disp_Draw_Tekst_L, 37, 22, &tekst_inst.Motorgroepen_14 };
static s_disp_tekst const disp_groep_1_str        = { Disp_Draw_Tekst_L, 37, 22, &tekst_inst.Groep_1_14 };
static s_disp_tekst const disp_groep_2_str        = { Disp_Draw_Tekst_L, 37, 22, &tekst_inst.Groep_2_14 };
static s_disp_tekst const disp_groep_3_str        = { Disp_Draw_Tekst_L, 37, 22, &tekst_inst.Groep_3_14 };
static s_disp_tekst const disp_groep_4_str        = { Disp_Draw_Tekst_L, 37, 22, &tekst_inst.Groep_4_14 };
static s_disp_tekst const disp_groep_5_str        = { Disp_Draw_Tekst_L, 37, 22, &tekst_inst.Groep_5_14 };
static s_disp_tekst const disp_groep_6_str        = { Disp_Draw_Tekst_L, 37, 22, &tekst_inst.Groep_6_14 };
static s_disp_tekst const disp_groep_7_str        = { Disp_Draw_Tekst_L, 37, 22, &tekst_inst.Groep_7_14 };
static s_disp_tekst const disp_groep_8_str        = { Disp_Draw_Tekst_L, 37, 22, &tekst_inst.Groep_8_14 };
static s_disp_tekst const disp_groep_9_str        = { Disp_Draw_Tekst_L, 37, 22, &tekst_inst.Groep_9_14 };
static s_disp_tekst const disp_groep_10_str       = { Disp_Draw_Tekst_L, 37, 22, &tekst_inst.Groep_10_14 };
static s_disp_tekst const disp_groep_11_str       = { Disp_Draw_Tekst_L, 37, 22, &tekst_inst.Groep_11_14 };
static s_disp_tekst const disp_groep_12_str       = { Disp_Draw_Tekst_L, 37, 22, &tekst_inst.Groep_12_14 };
static s_disp_tekst const disp_groep_13_str       = { Disp_Draw_Tekst_L, 37, 22, &tekst_inst.Groep_13_14 };
static s_disp_tekst const disp_groep_14_str       = { Disp_Draw_Tekst_L, 37, 22, &tekst_inst.Groep_14_14 };
static s_disp_tekst const disp_groep_15_str       = { Disp_Draw_Tekst_L, 37, 22, &tekst_inst.Groep_15_14 };
static s_disp_tekst const disp_groep_16_str       = { Disp_Draw_Tekst_L, 37, 22, &tekst_inst.Groep_16_14 };
static s_disp_tekst const disp_groep_17_str       = { Disp_Draw_Tekst_L, 37, 22, &tekst_inst.Groep_17_14 };
static s_disp_tekst const disp_groep_18_str       = { Disp_Draw_Tekst_L, 37, 22, &tekst_inst.Groep_18_14 };
static s_disp_tekst const disp_groep_19_str       = { Disp_Draw_Tekst_L, 37, 22, &tekst_inst.Groep_19_14 };
static s_disp_tekst const disp_groep_20_str       = { Disp_Draw_Tekst_L, 37, 22, &tekst_inst.Groep_20_14 };
static s_disp_tekst const disp_groep_21_str       = { Disp_Draw_Tekst_L, 37, 22, &tekst_inst.Groep_21_14 };
static s_disp_tekst const disp_groep_22_str       = { Disp_Draw_Tekst_L, 37, 22, &tekst_inst.Groep_22_14 };
static s_disp_tekst const disp_groep_23_str       = { Disp_Draw_Tekst_L, 37, 22, &tekst_inst.Groep_23_14 };
static s_disp_tekst const disp_groep_24_str       = { Disp_Draw_Tekst_L, 37, 22, &tekst_inst.Groep_24_14 };
static s_disp_tekst const disp_groep_25_str       = { Disp_Draw_Tekst_L, 37, 22, &tekst_inst.Groep_25_14 };
static s_disp_tekst const disp_groep_26_str       = { Disp_Draw_Tekst_L, 37, 22, &tekst_inst.Groep_26_14 };
static s_disp_tekst const disp_groep_27_str       = { Disp_Draw_Tekst_L, 37, 22, &tekst_inst.Groep_27_14 };
static s_disp_tekst const disp_groep_28_str       = { Disp_Draw_Tekst_L, 37, 22, &tekst_inst.Groep_28_14 };
static s_disp_tekst const disp_groep_29_str       = { Disp_Draw_Tekst_L, 37, 22, &tekst_inst.Groep_29_14 };
static s_disp_tekst const disp_groep_30_str       = { Disp_Draw_Tekst_L, 37, 22, &tekst_inst.Groep_30_14 };
static s_disp_tekst const disp_groep_31_str       = { Disp_Draw_Tekst_L, 37, 22, &tekst_inst.Groep_31_14 };
static s_disp_tekst const disp_groep_32_str       = { Disp_Draw_Tekst_L, 37, 22, &tekst_inst.Groep_32_14 };
static s_disp_tekst const disp_vrijgave_vent_str  = { Disp_Draw_Tekst_L, 37, 22, &tekst_inst.Vrijgave_ventilatoren_14 };
static s_disp_tekst const disp_2_doek_1_bed_str   = { Disp_Draw_Tekst_L, 37, 22, &tekst_inst.Twee_doek_een_bed_14 };
static s_disp_tekst const disp_cabriokas_str      = { Disp_Draw_Tekst_L, 37, 22, &tekst_inst.Cabriokas_14 };
static s_disp_tekst const disp_luchtmengkast_str  = { Disp_Draw_Tekst_L, 37, 22, &tekst_inst.Luchtmengkast_14 };
static s_disp_tekst const disp_sensoren_str       = { Disp_Draw_Tekst_L, 37, 22, &tekst_inst.Sensoren_14 };
static s_disp_tekst const disp_luchting_str       = { Disp_Draw_Tekst_L, 37, 22, &tekst_inst.Luchting_14 };
static s_disp_tekst const disp_alarm_str          = { Disp_Draw_Tekst_L, 37, 22, &tekst_inst.Alarmen_14  };

//=====================================================================================================================
static void * const lcd_algemeen_disp[]      = { &disp_globel, &disp_algemeen_string, 0 };
static void * const lcd_IO_disp[]            = { &disp_io, &disp_IO_string, 0 };
static void * const lcd_motorgroepen_disp[]  = { &disp_motorgroepen_ico, &disp_motorgroepen_str, 0 };
static void * const lcd_groep_1_disp[]       = { &disp_groep_ico, &disp_groep_1_str,  0 };
static void * const lcd_groep_2_disp[]       = { &disp_groep_ico, &disp_groep_2_str,  0 };
static void * const lcd_groep_3_disp[]       = { &disp_groep_ico, &disp_groep_3_str,  0 };
static void * const lcd_groep_4_disp[]       = { &disp_groep_ico, &disp_groep_4_str,  0 };
static void * const lcd_groep_5_disp[]       = { &disp_groep_ico, &disp_groep_5_str,  0 };
static void * const lcd_groep_6_disp[]       = { &disp_groep_ico, &disp_groep_6_str,  0 };
static void * const lcd_groep_7_disp[]       = { &disp_groep_ico, &disp_groep_7_str,  0 };
static void * const lcd_groep_8_disp[]       = { &disp_groep_ico, &disp_groep_8_str,  0 };
static void * const lcd_groep_9_disp[]       = { &disp_groep_ico, &disp_groep_9_str,  0 };
static void * const lcd_groep_10_disp[]      = { &disp_groep_ico, &disp_groep_10_str, 0 };
static void * const lcd_groep_11_disp[]      = { &disp_groep_ico, &disp_groep_11_str, 0 };
static void * const lcd_groep_12_disp[]      = { &disp_groep_ico, &disp_groep_12_str, 0 };
static void * const lcd_groep_13_disp[]      = { &disp_groep_ico, &disp_groep_13_str, 0 };
static void * const lcd_groep_14_disp[]      = { &disp_groep_ico, &disp_groep_14_str, 0 };
static void * const lcd_groep_15_disp[]      = { &disp_groep_ico, &disp_groep_15_str, 0 };
static void * const lcd_groep_16_disp[]      = { &disp_groep_ico, &disp_groep_16_str, 0 };
static void * const lcd_groep_17_disp[]      = { &disp_groep_ico, &disp_groep_17_str, 0 };
static void * const lcd_groep_18_disp[]      = { &disp_groep_ico, &disp_groep_18_str, 0 };
static void * const lcd_groep_19_disp[]      = { &disp_groep_ico, &disp_groep_19_str, 0 };
static void * const lcd_groep_20_disp[]      = { &disp_groep_ico, &disp_groep_20_str, 0 };
static void * const lcd_groep_21_disp[]      = { &disp_groep_ico, &disp_groep_21_str, 0 };
static void * const lcd_groep_22_disp[]      = { &disp_groep_ico, &disp_groep_22_str, 0 };
static void * const lcd_groep_23_disp[]      = { &disp_groep_ico, &disp_groep_23_str, 0 };
static void * const lcd_groep_24_disp[]      = { &disp_groep_ico, &disp_groep_24_str, 0 };
static void * const lcd_groep_25_disp[]      = { &disp_groep_ico, &disp_groep_25_str, 0 };
static void * const lcd_groep_26_disp[]      = { &disp_groep_ico, &disp_groep_26_str, 0 };
static void * const lcd_groep_27_disp[]      = { &disp_groep_ico, &disp_groep_27_str, 0 };
static void * const lcd_groep_28_disp[]      = { &disp_groep_ico, &disp_groep_28_str, 0 };
static void * const lcd_groep_29_disp[]      = { &disp_groep_ico, &disp_groep_29_str, 0 };
static void * const lcd_groep_30_disp[]      = { &disp_groep_ico, &disp_groep_30_str, 0 };
static void * const lcd_groep_31_disp[]      = { &disp_groep_ico, &disp_groep_31_str, 0 };
static void * const lcd_groep_32_disp[]      = { &disp_groep_ico, &disp_groep_32_str, 0 };
static void * const lcd_vrijgave_vent_disp[] = { &disp_vent, &disp_vrijgave_vent_str, 0 };
static void * const lcd_2_doek_1_bed_disp[]  = { &disp_twee_doek_een_bed_inst, &disp_2_doek_1_bed_str,  0 };
static void * const lcd_cabriokas_disp[]     = { &disp_cabriokas_ico,          &disp_cabriokas_str,     0 };
static void * const lcd_luchtmengkast_disp[] = { &disp_luchtmengkast_ico,      &disp_luchtmengkast_str, 0 };
static void * const lcd_sensoren_disp[]      = { &disp_sensoren_ico,           &disp_sensoren_str,      0 };
static void * const lcd_luchting_disp[]      = { &disp_groep_ico, &disp_luchting_str, 0 };
static void * const lcd_alarm_disp[]         = { &disp_alarm_on,  &disp_alarm_str,    0 };

//=====================================================================================================================
s_key_action const option_1_key_action[] =  
{
  {
    1,                                // nr
    0,                                // index
    (unsigned char *)&option_on,      // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_algemeen_disp,                // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Algemeen_Func,              // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    2,                                // nr
    0,                                // index
    (unsigned char *)&option_on,      // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_disp,                      // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Board_Func,                 // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    3,                                // nr
    0,                                // index
    (unsigned char *)&option_on,      // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_motorgroepen_disp,            // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Motorgroepen_Func,          // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    4,                                // nr
    0,                                // index
    &opt_app.Motorgroup[0].Enabled,   // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_groep_1_disp,                 // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Groep_Func,                 // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    5,                                // nr
    0,                                // index
    &opt_app.Motorgroup[1].Enabled,   // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_groep_2_disp,                 // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Groep_Func,                 // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    6,                                // nr
    0,                                // index
    &opt_app.Motorgroup[2].Enabled,   // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_groep_3_disp,                 // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Groep_Func,                 // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    7,                                // nr
    0,                                // index
    &opt_app.Motorgroup[3].Enabled,   // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_groep_4_disp,                 // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Groep_Func,                 // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    8,                                // nr
    0,                                // index
    &opt_app.Motorgroup[4].Enabled,   // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_groep_5_disp,                 // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Groep_Func,                 // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    9,                                // nr
    0,                                // index
    &opt_app.Motorgroup[5].Enabled,   // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_groep_6_disp,                 // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Groep_Func,                 // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    10,                               // nr
    0,                                // index
    &opt_app.Motorgroup[6].Enabled,   // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_groep_7_disp,                 // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Groep_Func,                 // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    11,                               // nr
    0,                                // index
    &opt_app.Motorgroup[7].Enabled,   // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_groep_8_disp,                 // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Groep_Func,                 // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    12,                               // nr
    0,                                // index
    &opt_app.Motorgroup[8].Enabled,   // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_groep_9_disp,                 // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Groep_Func,                 // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    13,                               // nr
    0,                                // index
    &opt_app.Motorgroup[9].Enabled,   // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_groep_10_disp,                // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Groep_Func,                 // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    14,                               // nr
    0,                                // index
    &opt_app.Motorgroup[10].Enabled,  // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_groep_11_disp,                // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Groep_Func,                 // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    15,                               // nr
    0,                                // index
    &opt_app.Motorgroup[11].Enabled,  // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_groep_12_disp,                // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Groep_Func,                 // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    16,                               // nr
    0,                                // index
    &opt_app.Motorgroup[12].Enabled,  // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_groep_13_disp,                // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Groep_Func,                 // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    17,                               // nr
    0,                                // index
    &opt_app.Motorgroup[13].Enabled,  // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_groep_14_disp,                // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Groep_Func,                 // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    18,                               // nr
    0,                                // index
    &opt_app.Motorgroup[14].Enabled,  // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_groep_15_disp,                // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Groep_Func,                 // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    19,                               // nr
    0,                                // index
    &opt_app.Motorgroup[15].Enabled,  // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_groep_16_disp,                // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Groep_Func,                 // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    20,                               // nr
    0,                                // index
    &opt_app.Motorgroup[16].Enabled,  // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_groep_17_disp,                // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Groep_Func,                 // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    21,                               // nr
    0,                                // index
    &opt_app.Motorgroup[17].Enabled,  // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_groep_18_disp,                // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Groep_Func,                 // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    22,                               // nr
    0,                                // index
    &opt_app.Motorgroup[18].Enabled,  // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_groep_19_disp,                // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Groep_Func,                 // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    23,                               // nr
    0,                                // index
    &opt_app.Motorgroup[19].Enabled,  // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_groep_20_disp,                // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Groep_Func,                 // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    24,                               // nr
    0,                                // index
    &opt_app.Motorgroup[20].Enabled,  // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_groep_21_disp,                // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Groep_Func,                 // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    25,                               // nr
    0,                                // index
    &opt_app.Motorgroup[21].Enabled,  // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_groep_22_disp,                // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Groep_Func,                 // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    26,                               // nr
    0,                                // index
    &opt_app.Motorgroup[22].Enabled,  // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_groep_23_disp,                // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Groep_Func,                 // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    27,                               // nr
    0,                                // index
    &opt_app.Motorgroup[23].Enabled,  // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_groep_24_disp,                // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Groep_Func,                 // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    28,                               // nr
    0,                                // index
    &opt_app.Motorgroup[24].Enabled,  // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_groep_25_disp,                // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Groep_Func,                 // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    29,                               // nr
    0,                                // index
    &opt_app.Motorgroup[25].Enabled,  // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_groep_26_disp,                // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Groep_Func,                 // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    30,                               // nr
    0,                                // index
    &opt_app.Motorgroup[26].Enabled,  // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_groep_27_disp,                // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Groep_Func,                 // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    31,                               // nr
    0,                                // index
    &opt_app.Motorgroup[27].Enabled,  // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_groep_28_disp,                // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Groep_Func,                 // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    32,                               // nr
    0,                                // index
    &opt_app.Motorgroup[28].Enabled,  // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_groep_29_disp,                // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Groep_Func,                 // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    33,                               // nr
    0,                                // index
    &opt_app.Motorgroup[29].Enabled,  // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_groep_30_disp,                // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Groep_Func,                 // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    34,                               // nr
    0,                                // index
    &opt_app.Motorgroup[30].Enabled,  // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_groep_31_disp,                // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Groep_Func,                 // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    35,                               // nr
    0,                                // index
    &opt_app.Motorgroup[31].Enabled,  // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_groep_32_disp,                // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Groep_Func,                 // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    36,                               // nr
    0,                                // index
    &option_ventilation,              // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_vrijgave_vent_disp,           // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Vrijgave_Vent_Func,         // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    37,                               // nr
    0,                                // index
    &option_screen,                   // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_2_doek_1_bed_disp,            // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_2_Doek_1_Bed_Func,          // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    38,                               // nr
    0,                                // index
    &option_window,                   // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_cabriokas_disp,               // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Cabriokas_Func,             // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    39,                               // nr
    0,                                // index
    &module.Luchtmengkast,            // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_luchtmengkast_disp,           // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Luchtmengkast_Func,         // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    40,                               // nr
    0,                                // index
    &module.Drukverschil,             // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_sensoren_disp,                // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Sensoren_Func,              // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    41,                               // nr
    0,                                // index
    (unsigned char *)&option_off,     // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_luchting_disp,                // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Luchting_Func,              // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    42,                                                              
    0,                                                              
    (unsigned char *)&option_on,
    (unsigned char *)&option_index_0,
    lcd_alarm_disp,
    0,
    &dummy_value,
    Dummy_Func,
    Arrow_Alarm_Func,
    Dummy_Func,
  },
};

s_screen screen_option_1;
s_screen const screen_option_1_default =
{
  0, // functie nr
  0, // index
  3, // max rel
  0, // rel actief
  0, // nr actief nummer weergegeven bin aantal ingeschakelde functie nummers
  0, // aantal ingeschakelde functie nummers
  lcd_disp_header, // algemene scherm opmaak
  {0,0,0,0,0,0}, // functies behorende bij functie toetsen
  0,3,19,       // rel[0]
  0,3,19+28,    // rel[1]
  0,3,19+28+28, // rel[2]
  &option_1_key_action[0], // first_action
  &option_1_key_action[sizeof(option_1_key_action)/sizeof(s_key_action) - 1], // last action 
  0, // change_flag
  0, // punt in normaal display
  0, // punt tijdens invoeren
  0, // aantal digits ingevoerd (punt telt niet mee)
  0, // value
  0, // min value
  0, // max value
  &screen_option_0,  // vorige scherm
  0  // prev_next_func
};

//*****************************************************************************
static void SetScreenOptions(void)
{
int i;

  option_ventilation = 0;
  option_screen      = 0;
  option_window      = 0;
  for (i = 0; i < opt_app.NumberMotorgroups; i++)
  {
    switch (opt_app.Motorgroup[i].Type)
	{
	  case TYPE_RAAM:
	    option_window = 1;
	    break;
	  case TYPE_DOEK:
	    option_screen = 1;
	    break;
	  case TYPE_VENT:
	    if (module.Schakelgroepen)
	      option_ventilation = 1;
	    break;
    }
  }
}

void Control_Screen_Option_1(void)
// zorg voor een correcte inhoud in klimaat scherm 0
{
  install_flag = 1;
  ModuleCheck();
  SetScreenOptions();
  Control_Screen(&screen_option_1, &screen_option_1_default, 1, 3);
}

//*****************************************************************************
void Goto_Function_Index(unsigned char index)
{
s_key_action const *ptr = screen_ptr->rel[screen_ptr->rel_actief].key_action;

  while (ptr->index > index) ptr--;
  while (ptr->index < index) ptr++;
  screen_ptr->rel[screen_ptr->rel_actief].key_action = ptr;
  screen_ptr->index = index;
  Get_Value();
}

//*****************************************************************************
static void Check_Options_Change_Flag(void)
{
  if (option_change_flag_alg) // controleer of opties zijn gewijzigd
  {
    option_change_flag_alg = 0;
    pc_0_read_configuration_alg = 1;  
    pc_1_read_configuration_alg = 1;
    pc_2_read_configuration_alg = 1;
    rs232_0_read_configuration_alg = 1;
    rs232_1_read_configuration_alg = 1;
    #ifdef ETHERNET
    ethernet_read_configuration_alg[0] = 1;
    ethernet_read_configuration_alg[1] = 1;
    ethernet_read_configuration_alg[2] = 1;
    ethernet_read_configuration_alg[3] = 1;
    #endif // ETHERNET
    IncrementOptionsChangeCount();
  }
}

//*****************************************************************************
static void Back_To_Prev_Screen(void)
{
  // Reset regelingen bij terug keer uit installatie procedure
  if (option_change_flag_alg)
  {
//    Check_If_Configuration_Changed(1);
  }
  Check_Options_Change_Flag();
  Prev_Screen();
  install_flag = 0;
  Password_Init();
  // Reset regelingen bij terug keer uit installatie procedure
}

//-----------------------------------------------------------------------------
static void Arrow_Algemeen_Func(void)
{
  switch (key)
  {
    case UP:
      Decrement_Func();
      break;
    case DOWN:
      Increment_Func();
      break;
    case LEFT:
      Back_To_Prev_Screen();
      break;
    case RIGHT:
      Control_Screen_Option_Algemeen_2();
      Next_Screen(&screen_opt_alg_2);
      break;
  }
}

//-----------------------------------------------------------------------------
static void Arrow_Board_Func(void)
{
  switch (key)
  {
    case UP:
      Decrement_Func();
      break;
    case DOWN:
      Increment_Func();
      break;
    case LEFT:
      Back_To_Prev_Screen();
      break;
    case RIGHT:
      Control_Screen_Option_IO_2();
      Next_Screen(&screen_opt_IO_2);
      break;
  }
}

//-----------------------------------------------------------------------------
static void Arrow_Motorgroepen_Func(void)
{
  switch (key)
  {
    case UP:
      Decrement_Func();
      break;
    case DOWN:
      Increment_Func();
      break;
    case LEFT:
      Back_To_Prev_Screen();
      break;
    case RIGHT:
      Control_Screen_Option_Motorgroepen_2();
      Next_Screen(&screen_opt_motorgroepen_2);
      break;
  }
}

//-----------------------------------------------------------------------------
static void Arrow_Groep_Func(void)
{
  switch (key)
  {
    case UP:
      Decrement_Func();
      break;
    case DOWN:
      Increment_Func();
      break;
    case LEFT:
      Back_To_Prev_Screen();
      break;
    case RIGHT:
      Control_Screen_Option_Groep_2(screen_ptr->nr - 4);
      Next_Screen(&screen_opt_groep_2);
      break;
  }
}

//-----------------------------------------------------------------------------
static void Arrow_Vrijgave_Vent_Func(void)
{
  switch (key)
  {
    case UP:
      Decrement_Func();
      break;
    case DOWN:
      Increment_Func();
      break;
    case LEFT:
      Back_To_Prev_Screen();
      break;
    case RIGHT:
      Control_Screen_Option_Vrijgave_Vent_2();
      Next_Screen(&screen_opt_vrijgave_vent_2);
      break;
  }
}

//-----------------------------------------------------------------------------
static void Arrow_2_Doek_1_Bed_Func(void)
{
  switch (key)
  {
    case UP:
      Decrement_Func();
      break;
    case DOWN:
      Increment_Func();
      break;
    case LEFT:
      Back_To_Prev_Screen();
      break;
    case RIGHT:
      Control_Screen_Option_Kiersturing_2();
      Next_Screen(&screen_opt_kiersturing_2);
      break;
  }
}

//-----------------------------------------------------------------------------
static void Arrow_Cabriokas_Func(void)
{
  switch (key)
  {
    case UP:
      Decrement_Func();
      break;
    case DOWN:
      Increment_Func();
      break;
    case LEFT:
      Back_To_Prev_Screen();
      break;
    case RIGHT:
      Control_Screen_Option_Cabriokas_2();
      Next_Screen(&screen_opt_cabriokas_2);
      break;
  }
}

//-----------------------------------------------------------------------------
static void Arrow_Luchtmengkast_Func(void)
{
  switch (key)
  {
    case UP:
      Decrement_Func();
      break;
    case DOWN:
      Increment_Func();
      break;
    case LEFT:
      Back_To_Prev_Screen();
      break;
    case RIGHT:
      Control_Screen_Option_Luchtmengkast_2();
      Next_Screen(&screen_opt_luchtmengkast_2);
      break;
  }
}

//-----------------------------------------------------------------------------
static void Arrow_Sensoren_Func(void)
{
  switch (key)
  {
    case UP:
      Decrement_Func();
      break;
    case DOWN:
      Increment_Func();
      break;
    case LEFT:
      Back_To_Prev_Screen();
      break;
    case RIGHT:
      Control_Screen_Option_Sensoren_2();
      Next_Screen(&screen_opt_sensoren_2);
      break;
  }
}

//-----------------------------------------------------------------------------
static void Arrow_Luchting_Func(void)
{
  switch (key)
  {
    case UP:
      Decrement_Func();
      break;
    case DOWN:
      Increment_Func();
      break;
    case LEFT:
      Back_To_Prev_Screen();
      break;
    case RIGHT:
      Control_Screen_Option_Luchting_2();
      Next_Screen(&screen_opt_luchting_2);
      break;
  }
}

//-----------------------------------------------------------------------------
static void Arrow_Alarm_Func(void)
{
  switch (key)
  {
    case UP:
      Decrement_Func();
      break;
    case DOWN:
      Increment_Func();
      break;
    case LEFT:
      Back_To_Prev_Screen();
      break;
    case RIGHT:
      Control_Screen_Option_Alarm_2();
      Next_Screen(&screen_opt_alarm_2);
      break;
  }
}

//-----------------------------------------------------------------------------
void Arrow_Start_Func(void)
{
  switch (key)
  {
    case UP:
      break;
    case DOWN:
      Increment_Func();
      start_flag = 0;
      screen_ptr->nr_aantal--;
      screen_ptr->nr_actief--;
      break;
    case LEFT:
      Prev_Screen();
      break;
    case RIGHT:
      break;
  }
}
//-----------------------------------------------------------------------------
void Arrow_End_Func(void)
{
  switch (key)
  {
    case UP:
      Decrement_Func();
      break;
    case DOWN:
      break;
    case LEFT:
      Prev_Screen();
      break;
    case RIGHT:
      break;
  }
}

//-----------------------------------------------------------------------------
void CheckOptions(void)
{
  CheckOptionsGroup();
  CheckOptionsMotor();
  CheckOptionsDevice();
  BuildLinkedList();
  CheckOptionsFrequencyControl();
  CheckOptionsPulseSystem();
  CheckOptionsDualScreen();
  CheckOptionsCabriokas();
  CheckOptionsLuchtmengkast();
  BuildLinkedList();

  CheckOptionsVrijgave();

  SetScreenOptions();
}

//------------------------------------------------------------------------------
unsigned char DummyNotUsed(s_board_IO_on_off IO_new)
{
  IO_new;
  return (1);
}

//-----------------------------------------------------------------------------
unsigned char DigInNotUsed(s_board_IO_on_off IO_new)
{
int i;

  for (i = 0; i < MAX_LUCHTMENGKAST_GROEP; i++)
  {
    if (Board_IO_Used(IO_new, &opt_app.LuchtmengkastGroep[i].Afblaasvent.Open,  1)) return (0);
    if (Board_IO_Used(IO_new, &opt_app.LuchtmengkastGroep[i].Afblaasvent.Close, 1)) return (0);
    if (Board_IO_Used(IO_new, &opt_app.LuchtmengkastGroep[i].Inblaasvent.Open,  1)) return (0);
    if (Board_IO_Used(IO_new, &opt_app.LuchtmengkastGroep[i].Inblaasvent.Close, 1)) return (0);
  }
  for (i = 0; i < MAX_GROUP; i++)
  {
    if (Board_IO_Used(IO_new, &opt_app.Ventgroup[i].DigInOnOff,    1)) return (0);
    if (Board_IO_Used(IO_new, &opt_app.Motorgroup[i].DigInOpen,    1)) return (0);
    if (Board_IO_Used(IO_new, &opt_app.Motorgroup[i].DigInClose,   1)) return (0);
    if (Board_IO_Used(IO_new, &opt_app.Motorgroup[i].DigInHiSpeed, 1)) return (0);
  }
  if (Board_IO_Used(IO_new, opt_app.VrijgaveVent.DigInVrijgave, MAX_VRIJGAVE)) return (0);

  return (1);
}

//*****************************************************************************
void Set_Unit_IO_Ana_In(void)
{
  if (index_array)
  {                                        
    switch (array[index_array - 1])
    {
      case ANA_IN_EMPTY:
        disp_unit_IO.tekst.tekst = &tekst_leeg;
        disp_unit_IO.tekst.func = Disp_Draw_Tekst_L;
        disp_unit_IO.tekst.x = 195;
        disp_unit_IO.tekst.y = 68;
        break;
      case ANA_IN_TEMP:
        disp_unit_IO.tekst.tekst = tekst_inst_graden[temp_unit];
        disp_unit_IO.tekst.func = Disp_Draw_Tekst_L;
        disp_unit_IO.tekst.x = 195;
        disp_unit_IO.tekst.y = 68;
        break; 
      case ANA_IN_PA:
        disp_unit_IO.tekst.tekst = &tekst_inst.Pa_7;
        disp_unit_IO.tekst.func = Disp_Draw_Tekst_L;
        disp_unit_IO.tekst.x = 195;
        disp_unit_IO.tekst.y = 68;
        break;
      case ANA_IN_RV:
        disp_unit_IO.tekst.tekst = &tekst.perc_7;
        disp_unit_IO.tekst.func = Disp_Draw_Tekst_L;
        disp_unit_IO.tekst.x = 195;
        disp_unit_IO.tekst.y = 68;
        break;
      case ANA_IN_CO2:
        disp_unit_IO.tekst.tekst = &tekst_inst.ppm_7;
        disp_unit_IO.tekst.func = Disp_Draw_Tekst_L;
        disp_unit_IO.tekst.x = 195;
        disp_unit_IO.tekst.y = 66;
        break;
      case ANA_IN_WINDRICHTING:
        disp_unit_IO.tekst.tekst = &tekst_inst.graden_7; 
        disp_unit_IO.tekst.func = Disp_Draw_Tekst_L;
        disp_unit_IO.tekst.x = 195;
        disp_unit_IO.tekst.y = 68;
        break;
      case ANA_IN_WINDSNELHEID:
        disp_unit_IO.tekst.tekst = &tekst_inst.mps_7;
        disp_unit_IO.tekst.func = Disp_Draw_Tekst_L;
        disp_unit_IO.tekst.x = 195;
        disp_unit_IO.tekst.y = 66;
        break;
      case ANA_IN_VOERWEGER:
        disp_unit_IO.tekst.tekst = &tekst_inst.kg_14;
        disp_unit_IO.tekst.func = Disp_Draw_Tekst_L;
        disp_unit_IO.tekst.x = 195;
        disp_unit_IO.tekst.y = 75;
        break;
      case ANA_IN_DIERWEEGSCHAAL_TOT_25KG:
        disp_unit_IO.tekst.tekst = &tekst_inst.g_14;
        disp_unit_IO.tekst.func = Disp_Draw_Tekst_L;
        disp_unit_IO.tekst.x = 195;
        disp_unit_IO.tekst.y = 75;
        break;
      case ANA_IN_DIERWEEGSCHAAL_BOVEN_25KG:
        disp_unit_IO.tekst.tekst = &tekst_inst.kg_14;
        disp_unit_IO.tekst.func = Disp_Draw_Tekst_L;
        disp_unit_IO.tekst.x = 195;
        disp_unit_IO.tekst.y = 75;
        break;
      case ANA_IN_RAAM:
      case ANA_IN_DOEK:
      case ANA_IN_VENT:
      case ANA_IN_KLEP:
      case ANA_IN_LAMEL:
      case ANA_IN_VERWARMING:
        disp_unit_IO.tekst.tekst = &tekst_inst.perc_7;
        disp_unit_IO.tekst.func = Disp_Draw_Tekst_L;
        disp_unit_IO.tekst.x = 195;
        disp_unit_IO.tekst.y = 68;
        break;
      case ANA_IN_VORST:
      case ANA_IN_ALARM:
        disp_unit_IO.tekst.tekst = &tekst_leeg;
        disp_unit_IO.tekst.func = Disp_Draw_Tekst_L;
        disp_unit_IO.tekst.x = 195;
        disp_unit_IO.tekst.y = 68;
        break;
    }
  }
  else
  {
    disp_unit_IO.tekst.tekst = &tekst_leeg;
    disp_unit_IO.tekst.func = Disp_Draw_Tekst_L;
    disp_unit_IO.tekst.x = 195;
    disp_unit_IO.tekst.y = 68;
  }  
}

void Set_Unit_IO_Ana_In_High(void)
{
  if (index_array)
  {                                        
    switch (array[index_array - 1])
    {
      case ANA_HIGH_IN_EMPTY:
        disp_unit_IO.tekst.tekst = &tekst_leeg;
        disp_unit_IO.tekst.func = Disp_Draw_Tekst_L;
        disp_unit_IO.tekst.x = 195;
        disp_unit_IO.tekst.y = 75;
        break;
      case ANA_HIGH_IN_SILO:
        disp_unit_IO.tekst.tekst = &tekst_inst.kg_14;
        disp_unit_IO.tekst.func = Disp_Draw_Tekst_L;
        disp_unit_IO.tekst.x = 195;
        disp_unit_IO.tekst.y = 75;
        break;
    }
  }
  else
  {
    disp_unit_IO.tekst.tekst = &tekst_leeg;
    disp_unit_IO.tekst.func = Disp_Draw_Tekst_L;
    disp_unit_IO.tekst.x = 195;
    disp_unit_IO.tekst.y = 75;
  }  
}

void Set_Unit_IO_Ana_Out(void)
{
  disp_unit_IO.tekst.func = Disp_Draw_Tekst_L;
  disp_unit_IO.tekst.x = 195;
  disp_unit_IO.tekst.y = 75;
  if (index_array)
  {                                        
    switch (array[index_array - 1])
    {
      default:
      case ANA_OUT_EMPTY:       disp_unit_IO.tekst.tekst = &tekst_leeg; break;
      case ANA_OUT_VENT:
      case ANA_OUT_KLEP:
      case ANA_OUT_VERWARMING:
      case ANA_OUT_LICHT:
      case ANA_OUT_UNI_REG:
      case ANA_OUT_MESTDROGING:
      case ANA_OUT_EI:
      case ANA_OUT_RAAM:
      case ANA_OUT_DOEK:
      case ANA_OUT_LAMEL: disp_unit_IO.tekst.tekst = &tekst_inst.V_14; break;
    }
  }
  else
    disp_unit_IO.tekst.tekst = &tekst_leeg;
}

void Set_Unit_IO_Dig_In(void)
{
  disp_unit_IO.tekst.func = Disp_Draw_Tekst_L;
  disp_unit_IO.tekst.x = 195;
  disp_unit_IO.tekst.y = 75;
  disp_unit_IO.tekst.tekst = &tekst_leeg;
}

void Set_Unit_IO_Dig_Out(void)
{
  disp_unit_IO.tekst.func = Disp_Draw_Tekst_L;
  disp_unit_IO.tekst.x = 195;
  disp_unit_IO.tekst.y = 75;
  disp_unit_IO.tekst.tekst = &tekst_leeg;
}

void Set_Unit_IO_Motor_Control(void)
{
  if (index_array)
  {                                        
    switch (array[index_array - 1])
    {
      case MOTOR_CONTROL_EMPTY:
        disp_unit_IO.tekst.tekst = &tekst_leeg;
        disp_unit_IO.tekst.func = Disp_Draw_Tekst_L;
        disp_unit_IO.tekst.x = 195;
        disp_unit_IO.tekst.y = 65;
        break;
      case MOTOR_CONTROL_RAAM:
      case MOTOR_CONTROL_DOEK:
        disp_unit_IO.tekst.tekst = &tekst.perc_7;
        disp_unit_IO.tekst.func = Disp_Draw_Tekst_L;
        disp_unit_IO.tekst.x = 195;
        disp_unit_IO.tekst.y = 68;
        break;
    }
  }
  else
  {
    disp_unit_IO.tekst.tekst = &tekst_leeg;
    disp_unit_IO.tekst.func = Disp_Draw_Tekst_L;
    disp_unit_IO.tekst.x = 195;
    disp_unit_IO.tekst.y = 65;
  }  
}

