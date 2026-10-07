// C__DISP_DIAG_SD_CARD_1.C

#include <string.h>

#include "ch_define.h"
#include "ch_asc0.h"
#include "ch_const.h"
#include "ch_disp.h"
#include "ch_disp_diagnose_0.h"
#include "ch_disp_func.h"
#include "ch_disp_password.h"
#include "ch_disp_value.h"
#include "ch_key.h"
#include "ch_lcd_7.h"
#include "ch_lcd_10.h"
#include "ch_lcd_14.h"
#include "ch_lcd_bitmap.h"
#include "ch_lcd_pixel.h"
#include "ch_sd.h"
#include "ch_string.h"
#include "ch_disp_diag_sd_card_1.h"

#ifdef SD_CARD

//static void SetFile2Info(void);
static void Arrow_Func_SD_Card_Active(void);
static void Arrow_SD_Card_Active(void);
static void Enter_SD_Card_Active(void);
static void Arrow_Func_SD_Card_Setpoints(void);
static void Arrow_SD_Card_Setpoints(void);
static void Enter_SD_Card_Setpoints(void);
static void Arrow_SD_Card_Setpoints_Bevestig(void);
static void Enter_SD_Card_Setpoints_Bevestig(void);
static void Arrow_Func_SD_Card_Options(void);
static void Arrow_SD_Card_Options(void);
static void Enter_SD_Card_Options(void);
static void Arrow_SD_Card_Options_Bevestig(void);
static void Enter_SD_Card_Options_Bevestig(void);
static void Disp_Option_Selection(void);

int selection_opt_nr = 0;
int selection_opt_nr_max;
unsigned char help_sd_card;
extern s_screen screen_diag_sd_card_1;

static s_disp_agri_header const disp_header = { Disp_Draw_Agri_Header, FN5, EMPTY, &tekst.Diag_SD_CARD_10, HK_GEEN, 0, 2};
static s_disp_func const disp_option_selection = { Disp_Control_Func, Disp_Option_Selection };
static void * const lcd_disp_header[] = { &disp_header, &disp_option_selection, 0};

static s_disp_tekst const disp_sd_card_str = { Disp_Draw_Tekst_L, 37, 20, &tekst.SD_CARD_10 };
static s_disp_bitmap const disp_orion_1 =      { Disp_Draw_Bitmap, 104, 2, &ico_orion };
s_bitmap const * const bmp_sd_card_placed_not_placed[2] = { &ico_no_sd_card, &ico_sd_card };
static s_disp_bitmap_array const disp_sd_card_placed_bmp = { Disp_Draw_Bitmap_Array,  152, 1, bmp_sd_card_placed_not_placed, UCHAR, &sd_placed_disp, 2 };
s_bitmap const * const bmp_sd_card_read[3] = { &ico_arrow_left, &ico_arrow_left, &ico_empty };
static s_disp_bitmap_array const disp_sd_card_read_bmp = { Disp_Draw_Bitmap_Array,  130, 6, bmp_sd_card_read, UCHAR, &sd_write_protect_disp, 3 };
s_bitmap const * const bmp_sd_card_write[3] = { &ico_arrow_right, &ico_empty, &ico_empty };
static s_disp_bitmap_array const disp_sd_card_write_bmp = { Disp_Draw_Bitmap_Array,  136, 6, bmp_sd_card_write, UCHAR, &sd_write_protect_disp, 3 };
static s_disp_value const disp_sd_card_free_space =  { Disp_Draw_Value, 215, 10, (SIZE_7 | RECHTS), LONG, 0, &free_disk_space };
static s_disp_value const disp_sd_card_size =        { Disp_Draw_Value, 215, 20, (SIZE_7 | RECHTS), LONG, 0, &disk_space };

static void const * const tekst_sd_active[] = { &tekst.active_10, &tekst.ask_remove_10, &tekst.remove_10, &tekst.no_card_10 };
static s_disp_tekst_array const disp_sd_card_active_str = { Disp_Draw_Tekst_Array_R, 215, 20, &tekst_sd_active, UCHAR, &setp_alg.sd_card_status, 4 };
static s_disp_tekst_array const disp_sd_card_active_change_str = { Disp_Draw_Tekst_Array_R, 215, 20, &tekst_sd_active, UCHAR, &help_sd_card, 4 };

s_file opt_file;
s_tekst_13 tekst_log_file[10] =
{
 { 13, 60, SIZE_10, "" },
 { 13, 60, SIZE_10, "" },
 { 13, 60, SIZE_10, "" },
 { 13, 60, SIZE_10, "" },
 { 13, 60, SIZE_10, "" },
 { 13, 60, SIZE_10, "" },
 { 13, 60, SIZE_10, "" },
 { 13, 60, SIZE_10, "" },
 { 13, 60, SIZE_10, "" },
 { 13, 60, SIZE_10, "" }
};
s_disp_cursor const disp_cursor_log_on =  { 195, 24,  16, 2, 20 };
static s_disp_bitmap const disp_orion_0 =      { Disp_Draw_Bitmap, 155, 2, &ico_orion };
static s_disp_bitmap const disp_sd_card_0 =     { Disp_Draw_Bitmap, 197, 1, &ico_sd_card };
static s_disp_tekst const disp_log_file_size_str = { Disp_Draw_Tekst_L, 112, 10, &tekst.SIZE_7 };
s_bitmap const * const bmp_log_off_write_read[3] = { &ico_cross_arrow_right, &ico_arrow_right, &ico_arrow_left };
static s_disp_bitmap_array const disp_log_file_0_on_bmp = { Disp_Draw_Bitmap_Array,  181, 6, bmp_log_off_write_read, UCHAR, &setp_alg.sd_card_log_on[0], 2 };
static s_disp_bitmap_array const disp_log_file_1_on_bmp = { Disp_Draw_Bitmap_Array,  181, 6, bmp_log_off_write_read, UCHAR, &setp_alg.sd_card_log_on[1], 2 };
static s_disp_bitmap_array const disp_log_file_2_on_bmp = { Disp_Draw_Bitmap_Array,  181, 6, bmp_log_off_write_read, UCHAR, &read_write_options, 3 };
static s_disp_bitmap_array const disp_log_file_2_on_change_bmp = { Disp_Draw_Bitmap_Array,  181, 6, bmp_log_off_write_read, UCHAR, &help_sd_card, 3 };
static s_disp_bitmap_array const disp_log_file_3_on_bmp = { Disp_Draw_Bitmap_Array,  181, 6, bmp_log_off_write_read, UCHAR, &read_write_setpoints, 3 };
static s_disp_bitmap_array const disp_log_file_3_on_change_bmp = { Disp_Draw_Bitmap_Array,  181, 6, bmp_log_off_write_read, UCHAR, &help_sd_card, 3 };
static s_disp_bitmap_array const disp_log_file_4_on_bmp = { Disp_Draw_Bitmap_Array,  181, 6, bmp_log_off_write_read, UCHAR, &setp_alg.sd_card_log_on[4], 2 };
static s_disp_bitmap_array const disp_log_file_5_on_bmp = { Disp_Draw_Bitmap_Array,  181, 6, bmp_log_off_write_read, UCHAR, &setp_alg.sd_card_log_on[5], 2 };
static s_disp_bitmap_array const disp_log_file_6_on_bmp = { Disp_Draw_Bitmap_Array,  181, 6, bmp_log_off_write_read, UCHAR, &setp_alg.sd_card_log_on[6], 2 };
static s_disp_bitmap_array const disp_log_file_7_on_bmp = { Disp_Draw_Bitmap_Array,  181, 6, bmp_log_off_write_read, UCHAR, &setp_alg.sd_card_log_on[7], 2 };
static s_disp_bitmap_array const disp_log_file_8_on_bmp = { Disp_Draw_Bitmap_Array,  181, 6, bmp_log_off_write_read, UCHAR, &setp_alg.sd_card_log_on[8], 2 };
//static s_disp_bitmap_array const disp_log_file_9_on_bmp = { Disp_Draw_Bitmap_Array,  181, 6, bmp_log_off_write_read, UCHAR, &setp_alg.sd_card_log_on[9], 2 };

static s_disp_tekst const     disp_log_file_0_str =   { Disp_Draw_Tekst_L, 37, 20,                      &tekst_log_file[0] };
static s_disp_value_add const disp_log_file_0_hour =  { Disp_Draw_Value_Add, (SIZE_7 | RECHTS), TIME_CHAR, 0, &log_file[0].hour };
static s_disp_value_add const disp_log_file_0_min =   { Disp_Draw_Value_Add, (SIZE_7 | RECHTS), TIME_CHAR, 0, &log_file[0].minute };
static s_disp_value_add const disp_log_file_0_sec =   { Disp_Draw_Value_Add, (SIZE_7 | RECHTS), TIME_CHAR, 0, &log_file[0].second };
static s_disp_value_add const disp_log_file_0_day =   { Disp_Draw_Value_Add, (SIZE_7 | RECHTS), TIME_CHAR, 0, &log_file[0].day };
static s_disp_value_add const disp_log_file_0_month = { Disp_Draw_Value_Add, (SIZE_7 | RECHTS), TIME_CHAR, 0, &log_file[0].month };
static s_disp_value const     disp_log_file_0_year =  { Disp_Draw_Value, 215, 20, (SIZE_7 | RECHTS), INT, 0,  &log_file[0].year };
static s_disp_value const     disp_log_file_0_size =  { Disp_Draw_Value, 215, 10, (SIZE_10| RECHTS), LONG, 0, &log_file[0].file_info.fsize };

static s_disp_tekst const     disp_log_file_1_str =   { Disp_Draw_Tekst_L, 37, 20,                      &tekst_log_file[1] };
static s_disp_value_add const disp_log_file_1_hour =  { Disp_Draw_Value_Add, (SIZE_7 | RECHTS), TIME_CHAR, 0, &log_file[1].hour };
static s_disp_value_add const disp_log_file_1_min =   { Disp_Draw_Value_Add, (SIZE_7 | RECHTS), TIME_CHAR, 0, &log_file[1].minute };
static s_disp_value_add const disp_log_file_1_sec =   { Disp_Draw_Value_Add, (SIZE_7 | RECHTS), TIME_CHAR, 0, &log_file[1].second };
static s_disp_value_add const disp_log_file_1_day =   { Disp_Draw_Value_Add, (SIZE_7 | RECHTS), TIME_CHAR, 0, &log_file[1].day };
static s_disp_value_add const disp_log_file_1_month = { Disp_Draw_Value_Add, (SIZE_7 | RECHTS), TIME_CHAR, 0, &log_file[1].month };
static s_disp_value const     disp_log_file_1_year =  { Disp_Draw_Value, 215, 20, (SIZE_7 | RECHTS), INT, 0,  &log_file[1].year };
static s_disp_value const     disp_log_file_1_size =  { Disp_Draw_Value, 215, 10, (SIZE_10| RECHTS), LONG, 0, &log_file[1].file_info.fsize };

//static s_disp_func  const     disp_log_file_2_ctrl  = { Disp_Control_Func, SetFile2Info };
//static s_disp_tekst const     disp_log_file_2_str =   { Disp_Draw_Tekst_L, 37, 20,                      &tekst_log_file[2] };
//static s_disp_value_add const disp_log_file_2_hour =  { Disp_Draw_Value_Add, (SIZE_7 | RECHTS), TIME_CHAR, 0, &opt_file.hour };
//static s_disp_value_add const disp_log_file_2_min =   { Disp_Draw_Value_Add, (SIZE_7 | RECHTS), TIME_CHAR, 0, &opt_file.minute };
//static s_disp_value_add const disp_log_file_2_sec =   { Disp_Draw_Value_Add, (SIZE_7 | RECHTS), TIME_CHAR, 0, &opt_file.second };
//static s_disp_value_add const disp_log_file_2_day =   { Disp_Draw_Value_Add, (SIZE_7 | RECHTS), TIME_CHAR, 0, &opt_file.day };
//static s_disp_value_add const disp_log_file_2_month = { Disp_Draw_Value_Add, (SIZE_7 | RECHTS), TIME_CHAR, 0, &opt_file.month };
//static s_disp_value const     disp_log_file_2_year =  { Disp_Draw_Value, 215, 20, (SIZE_7 | RECHTS), INT, 0,  &opt_file.year };
//static s_disp_value const     disp_log_file_2_size =  { Disp_Draw_Value, 215, 10, (SIZE_10| RECHTS), LONG, 0, &opt_file.file_info.fsize };
static s_disp_tekst const     disp_log_file_2_str =   { Disp_Draw_Tekst_L, 37, 20,                      &tekst_log_file[2] };
static s_disp_value_add const disp_log_file_2_hour =  { Disp_Draw_Value_Add, (SIZE_7 | RECHTS), TIME_CHAR, 0, &log_file[2].hour };
static s_disp_value_add const disp_log_file_2_min =   { Disp_Draw_Value_Add, (SIZE_7 | RECHTS), TIME_CHAR, 0, &log_file[2].minute };
static s_disp_value_add const disp_log_file_2_sec =   { Disp_Draw_Value_Add, (SIZE_7 | RECHTS), TIME_CHAR, 0, &log_file[2].second };
static s_disp_value_add const disp_log_file_2_day =   { Disp_Draw_Value_Add, (SIZE_7 | RECHTS), TIME_CHAR, 0, &log_file[2].day };
static s_disp_value_add const disp_log_file_2_month = { Disp_Draw_Value_Add, (SIZE_7 | RECHTS), TIME_CHAR, 0, &log_file[2].month };
static s_disp_value const     disp_log_file_2_year =  { Disp_Draw_Value, 215, 20, (SIZE_7 | RECHTS), INT, 0,  &log_file[2].year };
static s_disp_value const     disp_log_file_2_size =  { Disp_Draw_Value, 215, 10, (SIZE_10| RECHTS), LONG, 0, &log_file[2].file_info.fsize };

static s_disp_tekst const     disp_log_file_3_str =   { Disp_Draw_Tekst_L, 37, 20,                      &tekst_log_file[3] };
static s_disp_value_add const disp_log_file_3_hour =  { Disp_Draw_Value_Add, (SIZE_7 | RECHTS), TIME_CHAR, 0, &log_file[3].hour };
static s_disp_value_add const disp_log_file_3_min =   { Disp_Draw_Value_Add, (SIZE_7 | RECHTS), TIME_CHAR, 0, &log_file[3].minute };
static s_disp_value_add const disp_log_file_3_sec =   { Disp_Draw_Value_Add, (SIZE_7 | RECHTS), TIME_CHAR, 0, &log_file[3].second };
static s_disp_value_add const disp_log_file_3_day =   { Disp_Draw_Value_Add, (SIZE_7 | RECHTS), TIME_CHAR, 0, &log_file[3].day };
static s_disp_value_add const disp_log_file_3_month = { Disp_Draw_Value_Add, (SIZE_7 | RECHTS), TIME_CHAR, 0, &log_file[3].month };
static s_disp_value const     disp_log_file_3_year =  { Disp_Draw_Value, 215, 20, (SIZE_7 | RECHTS), INT, 0,  &log_file[3].year };
static s_disp_value const     disp_log_file_3_size =  { Disp_Draw_Value, 215, 10, (SIZE_10| RECHTS), LONG, 0, &log_file[3].file_info.fsize };

static s_disp_tekst const     disp_log_file_4_str =   { Disp_Draw_Tekst_L, 37, 20,                      &tekst_log_file[4] };
static s_disp_value_add const disp_log_file_4_hour =  { Disp_Draw_Value_Add, (SIZE_7 | RECHTS), TIME_CHAR, 0, &log_file[4].hour };
static s_disp_value_add const disp_log_file_4_min =   { Disp_Draw_Value_Add, (SIZE_7 | RECHTS), TIME_CHAR, 0, &log_file[4].minute };
static s_disp_value_add const disp_log_file_4_sec =   { Disp_Draw_Value_Add, (SIZE_7 | RECHTS), TIME_CHAR, 0, &log_file[4].second };
static s_disp_value_add const disp_log_file_4_day =   { Disp_Draw_Value_Add, (SIZE_7 | RECHTS), TIME_CHAR, 0, &log_file[4].day };
static s_disp_value_add const disp_log_file_4_month = { Disp_Draw_Value_Add, (SIZE_7 | RECHTS), TIME_CHAR, 0, &log_file[4].month };
static s_disp_value const     disp_log_file_4_year =  { Disp_Draw_Value, 215, 20, (SIZE_7 | RECHTS), INT, 0,  &log_file[4].year };
static s_disp_value const     disp_log_file_4_size =  { Disp_Draw_Value, 215, 10, (SIZE_10| RECHTS), LONG, 0, &log_file[4].file_info.fsize };

static s_disp_tekst const     disp_log_file_5_str =   { Disp_Draw_Tekst_L, 37, 20,                      &tekst_log_file[5] };
static s_disp_value_add const disp_log_file_5_hour =  { Disp_Draw_Value_Add, (SIZE_7 | RECHTS), TIME_CHAR, 0, &log_file[5].hour };
static s_disp_value_add const disp_log_file_5_min =   { Disp_Draw_Value_Add, (SIZE_7 | RECHTS), TIME_CHAR, 0, &log_file[5].minute };
static s_disp_value_add const disp_log_file_5_sec =   { Disp_Draw_Value_Add, (SIZE_7 | RECHTS), TIME_CHAR, 0, &log_file[5].second };
static s_disp_value_add const disp_log_file_5_day =   { Disp_Draw_Value_Add, (SIZE_7 | RECHTS), TIME_CHAR, 0, &log_file[5].day };
static s_disp_value_add const disp_log_file_5_month = { Disp_Draw_Value_Add, (SIZE_7 | RECHTS), TIME_CHAR, 0, &log_file[5].month };
static s_disp_value const     disp_log_file_5_year =  { Disp_Draw_Value, 215, 20, (SIZE_7 | RECHTS), INT, 0,  &log_file[5].year };
static s_disp_value const     disp_log_file_5_size =  { Disp_Draw_Value, 215, 10, (SIZE_10| RECHTS), LONG, 0, &log_file[5].file_info.fsize };

static s_disp_tekst const     disp_log_file_6_str =   { Disp_Draw_Tekst_L, 37, 20,                      &tekst_log_file[6] };
static s_disp_value_add const disp_log_file_6_hour =  { Disp_Draw_Value_Add, (SIZE_7 | RECHTS), TIME_CHAR, 0, &log_file[6].hour };
static s_disp_value_add const disp_log_file_6_min =   { Disp_Draw_Value_Add, (SIZE_7 | RECHTS), TIME_CHAR, 0, &log_file[6].minute };
static s_disp_value_add const disp_log_file_6_sec =   { Disp_Draw_Value_Add, (SIZE_7 | RECHTS), TIME_CHAR, 0, &log_file[6].second };
static s_disp_value_add const disp_log_file_6_day =   { Disp_Draw_Value_Add, (SIZE_7 | RECHTS), TIME_CHAR, 0, &log_file[6].day };
static s_disp_value_add const disp_log_file_6_month = { Disp_Draw_Value_Add, (SIZE_7 | RECHTS), TIME_CHAR, 0, &log_file[6].month };
static s_disp_value const     disp_log_file_6_year =  { Disp_Draw_Value, 215, 20, (SIZE_7 | RECHTS), INT, 0,  &log_file[6].year };
static s_disp_value const     disp_log_file_6_size =  { Disp_Draw_Value, 215, 10, (SIZE_10| RECHTS), LONG, 0, &log_file[6].file_info.fsize };

static s_disp_tekst const     disp_log_file_7_str =   { Disp_Draw_Tekst_L, 37, 20,                      &tekst_log_file[7] };
static s_disp_value_add const disp_log_file_7_hour =  { Disp_Draw_Value_Add, (SIZE_7 | RECHTS), TIME_CHAR, 0, &log_file[7].hour };
static s_disp_value_add const disp_log_file_7_min =   { Disp_Draw_Value_Add, (SIZE_7 | RECHTS), TIME_CHAR, 0, &log_file[7].minute };
static s_disp_value_add const disp_log_file_7_sec =   { Disp_Draw_Value_Add, (SIZE_7 | RECHTS), TIME_CHAR, 0, &log_file[7].second };
static s_disp_value_add const disp_log_file_7_day =   { Disp_Draw_Value_Add, (SIZE_7 | RECHTS), TIME_CHAR, 0, &log_file[7].day };
static s_disp_value_add const disp_log_file_7_month = { Disp_Draw_Value_Add, (SIZE_7 | RECHTS), TIME_CHAR, 0, &log_file[7].month };
static s_disp_value const     disp_log_file_7_year =  { Disp_Draw_Value, 215, 20, (SIZE_7 | RECHTS), INT, 0,  &log_file[7].year };
static s_disp_value const     disp_log_file_7_size =  { Disp_Draw_Value, 215, 10, (SIZE_10| RECHTS), LONG, 0, &log_file[7].file_info.fsize };

static s_disp_tekst const     disp_log_file_8_str =   { Disp_Draw_Tekst_L, 37, 20,                      &tekst_log_file[8] };
static s_disp_value_add const disp_log_file_8_hour =  { Disp_Draw_Value_Add, (SIZE_7 | RECHTS), TIME_CHAR, 0, &log_file[8].hour };
static s_disp_value_add const disp_log_file_8_min =   { Disp_Draw_Value_Add, (SIZE_7 | RECHTS), TIME_CHAR, 0, &log_file[8].minute };
static s_disp_value_add const disp_log_file_8_sec =   { Disp_Draw_Value_Add, (SIZE_7 | RECHTS), TIME_CHAR, 0, &log_file[8].second };
static s_disp_value_add const disp_log_file_8_day =   { Disp_Draw_Value_Add, (SIZE_7 | RECHTS), TIME_CHAR, 0, &log_file[8].day };
static s_disp_value_add const disp_log_file_8_month = { Disp_Draw_Value_Add, (SIZE_7 | RECHTS), TIME_CHAR, 0, &log_file[8].month };
static s_disp_value const     disp_log_file_8_year =  { Disp_Draw_Value, 215, 20, (SIZE_7 | RECHTS), INT, 0,  &log_file[8].year };
static s_disp_value const     disp_log_file_8_size =  { Disp_Draw_Value, 215, 10, (SIZE_10| RECHTS), LONG, 0, &log_file[8].file_info.fsize };

//static s_disp_tekst const     disp_log_file_9_str =   { Disp_Draw_Tekst_L, 37, 20,                      &tekst_log_file[9] };
//static s_disp_value_add const disp_log_file_9_hour =  { Disp_Draw_Value_Add, (SIZE_7 | RECHTS), TIME_CHAR, 0, &log_file[9].hour };
//static s_disp_value_add const disp_log_file_9_min =   { Disp_Draw_Value_Add, (SIZE_7 | RECHTS), TIME_CHAR, 0, &log_file[9].minute };
//static s_disp_value_add const disp_log_file_9_sec =   { Disp_Draw_Value_Add, (SIZE_7 | RECHTS), TIME_CHAR, 0, &log_file[9].second };
//static s_disp_value_add const disp_log_file_9_day =   { Disp_Draw_Value_Add, (SIZE_7 | RECHTS), TIME_CHAR, 0, &log_file[9].day };
//static s_disp_value_add const disp_log_file_9_month = { Disp_Draw_Value_Add, (SIZE_7 | RECHTS), TIME_CHAR, 0, &log_file[9].month };
//static s_disp_value const     disp_log_file_9_year =  { Disp_Draw_Value, 215, 20, (SIZE_7 | RECHTS), INT, 0,  &log_file[9].year };
//static s_disp_value const     disp_log_file_9_size =  { Disp_Draw_Value, 215, 10, (SIZE_10| RECHTS), LONG, 0, &log_file[9].file_info.fsize };



static void * const lcd_sd_card_active_disp[] = { &disp_sd_card, &disp_sd_card_str, &disp_sd_card_active_str, 0 };
static void * const lcd_sd_card_active_change_disp[] = { &disp_sd_card, &disp_sd_card_str, &disp_sd_card_active_change_str, 0 };
static void * const lcd_sd_card_state_disp[] =
{
  &disp_sd_card, &disp_sd_card_str, &disp_orion_1, &disp_sd_card_read_bmp, &disp_sd_card_write_bmp, &disp_sd_card_placed_bmp, 
  &disp_sd_card_free_space, &disp_sd_card_size, 0
};
static void * const lcd_sd_card_log_file_0_on_disp[] = 
{
           &disp_sd_card, &disp_log_file_0_str,
           &disp_orion_0, &disp_log_file_0_on_bmp, &disp_sd_card_0, 0
};                          
static void * const lcd_sd_card_log_file_1_on_disp[] = 
{
           &disp_sd_card, &disp_log_file_1_str,
           &disp_orion_0, &disp_log_file_1_on_bmp, &disp_sd_card_0, 0
};                          
static void * const lcd_sd_card_log_file_2_on_disp[] = 
{
//           &disp_log_file_2_ctrl,
           &disp_sd_card, &disp_log_file_2_str,
           &disp_orion_0, &disp_log_file_2_on_bmp, &disp_sd_card_0, 0
};                          
static void * const lcd_sd_card_log_file_2_on_change_disp[] = 
{
           &disp_sd_card, &disp_log_file_2_str,
           &disp_orion_0, &disp_log_file_2_on_change_bmp, &disp_sd_card_0, 0
};                          
static void * const lcd_sd_card_log_file_2_on_change_bevestig_disp[] = 
{
  &disp_sd_card, &disp_log_file_2_str,
  &disp_orion_0, &disp_log_file_2_on_change_bmp, &disp_sd_card_0,
  &disp_messagebox_bevestig, &disp_zeker_weten_str, &disp_bevestig_arrow_up_bmp, &disp_bevestig_is_teken, &disp_space_10_L, &disp_ja_str,  0
};                          
static void * const lcd_sd_card_log_file_3_on_disp[] = 
{
           &disp_sd_card, &disp_log_file_3_str,
           &disp_orion_0, &disp_log_file_3_on_bmp, &disp_sd_card_0, 0
};                          
static void * const lcd_sd_card_log_file_3_on_change_disp[] = 
{
           &disp_sd_card, &disp_log_file_3_str,
           &disp_orion_0, &disp_log_file_3_on_change_bmp, &disp_sd_card_0, 0
};                          
static void * const lcd_sd_card_log_file_3_on_change_bevestig_disp[] = 
{
  &disp_sd_card, &disp_log_file_3_str,
  &disp_orion_0, &disp_log_file_3_on_change_bmp, &disp_sd_card_0,
  &disp_messagebox_bevestig, &disp_zeker_weten_str, &disp_bevestig_arrow_up_bmp, &disp_bevestig_is_teken, &disp_space_10_L, &disp_ja_str,  0
};                          
static void * const lcd_sd_card_log_file_4_on_disp[] = 
{
           &disp_sd_card, &disp_log_file_4_str,
           &disp_orion_0, &disp_log_file_4_on_bmp, &disp_sd_card_0, 0
};                          
static void * const lcd_sd_card_log_file_5_on_disp[] = 
{
           &disp_sd_card, &disp_log_file_5_str,
           &disp_orion_0, &disp_log_file_5_on_bmp, &disp_sd_card_0, 0
};                          
static void * const lcd_sd_card_log_file_6_on_disp[] = 
{
           &disp_sd_card, &disp_log_file_6_str,
           &disp_orion_0, &disp_log_file_6_on_bmp, &disp_sd_card_0, 0
};                          
static void * const lcd_sd_card_log_file_7_on_disp[] = 
{
           &disp_sd_card, &disp_log_file_7_str,
           &disp_orion_0, &disp_log_file_7_on_bmp, &disp_sd_card_0, 0
};                                        
static void * const lcd_sd_card_log_file_8_on_disp[] = 
{
           &disp_sd_card, &disp_log_file_8_str,
           &disp_orion_0, &disp_log_file_8_on_bmp, &disp_sd_card_0, 0
};                          
//static void * const lcd_sd_card_log_file_9_on_disp[] = 
//{
//           &disp_sd_card, &disp_log_file_9_str,
//           &disp_orion_0, &disp_log_file_9_on_bmp, &disp_sd_card_0, 0
//};                          
static void * const lcd_sd_card_log_file_0_disp[] = 
{ 
           &disp_sd_card, &disp_log_file_0_str, 
                          &disp_log_file_0_year, &disp_min_teken_7_R,
                          &disp_log_file_0_month, &disp_min_teken_7_R,
                          &disp_log_file_0_day, &disp_space_10_R,
                          &disp_log_file_0_sec, &disp_punt_7_R,
                          &disp_log_file_0_min, &disp_dp_7_R,
                          &disp_log_file_0_hour, &disp_log_file_size_str,
                          &disp_log_file_0_size, 0 
};
static void * const lcd_sd_card_log_file_1_disp[] = 
{ 
           &disp_sd_card, &disp_log_file_1_str, 
                          &disp_log_file_1_year, &disp_min_teken_7_R,
                          &disp_log_file_1_month, &disp_min_teken_7_R,
                          &disp_log_file_1_day, &disp_space_10_R,
                          &disp_log_file_1_sec, &disp_punt_7_R,
                          &disp_log_file_1_min, &disp_dp_7_R,
                          &disp_log_file_1_hour, &disp_log_file_size_str,
                          &disp_log_file_1_size, 0 
};
static void * const lcd_sd_card_log_file_2_disp[] = 
{                                         
           &disp_sd_card, &disp_log_file_2_str, 
                          &disp_log_file_2_year, &disp_min_teken_7_R,
                          &disp_log_file_2_month, &disp_min_teken_7_R,
                          &disp_log_file_2_day, &disp_space_10_R,
                          &disp_log_file_2_sec, &disp_punt_7_R,
                          &disp_log_file_2_min, &disp_dp_7_R,
                          &disp_log_file_2_hour, &disp_log_file_size_str,
                          &disp_log_file_2_size, 0 
};
static void * const lcd_sd_card_log_file_3_disp[] = 
{ 
           &disp_sd_card, &disp_log_file_3_str, 
                          &disp_log_file_3_year, &disp_min_teken_7_R,
                          &disp_log_file_3_month, &disp_min_teken_7_R,
                          &disp_log_file_3_day, &disp_space_10_R,
                          &disp_log_file_3_sec, &disp_punt_7_R,
                          &disp_log_file_3_min, &disp_dp_7_R,
                          &disp_log_file_3_hour, &disp_log_file_size_str,
                          &disp_log_file_3_size, 0 
};
static void * const lcd_sd_card_log_file_4_disp[] = 
{                                         
           &disp_sd_card, &disp_log_file_4_str, 
                          &disp_log_file_4_year, &disp_min_teken_7_R,
                          &disp_log_file_4_month, &disp_min_teken_7_R,
                          &disp_log_file_4_day, &disp_space_10_R,
                          &disp_log_file_4_sec, &disp_punt_7_R,
                          &disp_log_file_4_min, &disp_dp_7_R,
                          &disp_log_file_4_hour, &disp_log_file_size_str,
                          &disp_log_file_4_size, 0 
};
static void * const lcd_sd_card_log_file_5_disp[] = 
{ 
           &disp_sd_card, &disp_log_file_5_str, 
                          &disp_log_file_5_year, &disp_min_teken_7_R,
                          &disp_log_file_5_month, &disp_min_teken_7_R,
                          &disp_log_file_5_day, &disp_space_10_R,
                          &disp_log_file_5_sec, &disp_punt_7_R,
                          &disp_log_file_5_min, &disp_dp_7_R,
                          &disp_log_file_5_hour, &disp_log_file_size_str,
                          &disp_log_file_5_size, 0 
};
static void * const lcd_sd_card_log_file_6_disp[] = 
{ 
           &disp_sd_card, &disp_log_file_6_str, 
                          &disp_log_file_6_year, &disp_min_teken_7_R,
                          &disp_log_file_6_month, &disp_min_teken_7_R,
                          &disp_log_file_6_day, &disp_space_10_R,
                          &disp_log_file_6_sec, &disp_punt_7_R,
                          &disp_log_file_6_min, &disp_dp_7_R,
                          &disp_log_file_6_hour, &disp_log_file_size_str,
                          &disp_log_file_6_size, 0 
};
static void * const lcd_sd_card_log_file_7_disp[] = 
{ 
           &disp_sd_card, &disp_log_file_7_str, 
                          &disp_log_file_7_year, &disp_min_teken_7_R,
                          &disp_log_file_7_month, &disp_min_teken_7_R,
                          &disp_log_file_7_day, &disp_space_10_R,
                          &disp_log_file_7_sec, &disp_punt_7_R,
                          &disp_log_file_7_min, &disp_dp_7_R,
                          &disp_log_file_7_hour, &disp_log_file_size_str,
                          &disp_log_file_7_size, 0 
};
static void * const lcd_sd_card_log_file_8_disp[] = 
{ 
           &disp_sd_card, &disp_log_file_8_str, 
                          &disp_log_file_8_year, &disp_min_teken_7_R,
                          &disp_log_file_8_month, &disp_min_teken_7_R,
                          &disp_log_file_8_day, &disp_space_10_R,
                          &disp_log_file_8_sec, &disp_punt_7_R,
                          &disp_log_file_8_min, &disp_dp_7_R,
                          &disp_log_file_8_hour, &disp_log_file_size_str,
                          &disp_log_file_8_size, 0 
};
//static void * const lcd_sd_card_log_file_9_disp[] = 
//{ 
//           &disp_sd_card, &disp_log_file_9_str, 
//                          &disp_log_file_9_year, &disp_min_teken_7_R,
//                          &disp_log_file_9_month, &disp_min_teken_7_R,
//                          &disp_log_file_9_day, &disp_space_10_R,
//                          &disp_log_file_9_sec, &disp_punt_7_R,
//                          &disp_log_file_9_min, &disp_dp_7_R,
//                          &disp_log_file_9_hour, &disp_log_file_size_str,
//                          &disp_log_file_9_size, 0 
//};

static s_key_value const key_sd_card_active_value = { UCHAR, 1, &help_sd_card, &uchar_0, &uchar_2 };
static s_key_value const key_sd_card_log_file_0_on_value = { UCHAR, 1, &setp_alg.sd_card_log_on[0], &uchar_0, &uchar_1 };
static s_key_value const key_sd_card_log_file_1_on_value = { UCHAR, 1, &setp_alg.sd_card_log_on[1], &uchar_0, &uchar_1 };
static s_key_value const key_sd_card_log_file_2_on_value = { UCHAR, 1, &help_sd_card, &uchar_0, &uchar_2 };
static s_key_value const key_sd_card_log_file_3_on_value = { UCHAR, 1, &help_sd_card, &uchar_0, &uchar_2 };
static s_key_value const key_sd_card_log_file_4_on_value = { UCHAR, 1, &setp_alg.sd_card_log_on[4], &uchar_0, &uchar_1 };
static s_key_value const key_sd_card_log_file_5_on_value = { UCHAR, 1, &setp_alg.sd_card_log_on[5], &uchar_0, &uchar_1 };
static s_key_value const key_sd_card_log_file_6_on_value = { UCHAR, 1, &setp_alg.sd_card_log_on[6], &uchar_0, &uchar_1 };
static s_key_value const key_sd_card_log_file_7_on_value = { UCHAR, 1, &setp_alg.sd_card_log_on[7], &uchar_0, &uchar_1 };
static s_key_value const key_sd_card_log_file_8_on_value = { UCHAR, 1, &setp_alg.sd_card_log_on[8], &uchar_0, &uchar_1 };
//static s_key_value const key_sd_card_log_file_9_on_value = { UCHAR, 1, &setp_alg.sd_card_log_on[9], &uchar_0, &uchar_1 };

s_key_action const diag_sd_card_1_key_action[] =
{
  { // sd_card_remove
    1,                               // nr
    0,                                // index
    (unsigned char *)&option_on,      // option
    (unsigned char *)&option_index_0,   // Optie index 
    lcd_sd_card_active_disp,          // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Func_SD_Card_Active,             // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  { 
    1,               // nr
    1,               // index
    (unsigned char *)&option_on,      // option
    (unsigned char *)&option_index_0,   // Optie index 
    lcd_sd_card_active_change_disp,          // display
    &disp_cursor_215_23_70, // cursor
    &key_sd_card_active_value,     // *value
    Dummy_Func, // void (*number)(void); 
    Arrow_SD_Card_Active,  // void (*arrow)(void); 
    Enter_SD_Card_Active,  // void (*enter)(void);
  },
  { // sd card state
    2,                                // nr
    0,                                // index
    (unsigned char *)&option_on,      // option
    (unsigned char *)&option_index_0,   // Optie index 
    lcd_sd_card_state_disp,   // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Change_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  { // log file 0 on
    10,                               // nr
    0,                                // index
    (unsigned char *)&log_file[0].used, // option
    (unsigned char *)&option_index_0,   // Optie index 
    lcd_sd_card_log_file_0_on_disp,      // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Change_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  { // log file 0 on
    10,                               // nr
    1,                                // index
    (unsigned char *)&log_file[0].used, // option
    (unsigned char *)&option_index_0,   // Optie index 
    lcd_sd_card_log_file_0_on_disp,   // display
    &disp_cursor_log_on,           // cursor
    &key_sd_card_log_file_0_on_value, // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Scroll_Value,               // void (*arrow)(void); 
    Increment_Func_Index_Enter_Value, // void (*enter)(void);
  },
  { // log file 0
    11,                               // nr
    0,                                // index
    (unsigned char *)&log_file[0].used, // option
    (unsigned char *)&option_index_0,   // Optie index 
    lcd_sd_card_log_file_0_disp,      // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Change_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  { // log file 1 on
    20,                               // nr
    0,                                // index
    (unsigned char *)&log_file[1].used, // option
    (unsigned char *)&option_index_0,   // Optie index 
    lcd_sd_card_log_file_1_on_disp,      // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Change_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  { // log file 1 on
    20,                               // nr
    1,                                // index
    (unsigned char *)&log_file[1].used, // option
    (unsigned char *)&option_index_0,   // Optie index 
    lcd_sd_card_log_file_1_on_disp,   // display
    &disp_cursor_log_on,           // cursor
    &key_sd_card_log_file_1_on_value, // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Scroll_Value,               // void (*arrow)(void); 
    Increment_Func_Index_Enter_Value, // void (*enter)(void);
  },
  { // log file 1
    21,                               // nr
    0,                                // index
    (unsigned char *)&log_file[1].used, // option
    (unsigned char *)&option_index_0,   // Optie index 
    lcd_sd_card_log_file_1_disp,      // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Change_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  { // log file 2 on
    30,                               // nr
    0,                                // index
    (unsigned char *)&log_file[2].used, // option
    (unsigned char *)&option_index_0,   // Optie index 
    lcd_sd_card_log_file_2_on_disp,      // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Func_SD_Card_Options,       // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  { // log file 2 on
    30,                               // nr
    1,                                // index
    (unsigned char *)&log_file[2].used, // option
    (unsigned char *)&option_index_0,   // Optie index 
    lcd_sd_card_log_file_2_on_change_disp,   // display
    &disp_cursor_log_on,           // cursor
    &key_sd_card_log_file_2_on_value, // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_SD_Card_Options,  // void (*arrow)(void); 
    Enter_SD_Card_Options,  // void (*enter)(void);
  },
  {
    30,                             // nr
    2,                              // index
    &log_file[2].used,              // option
    (unsigned char *)&option_index_0,   // Optie index 
    lcd_sd_card_log_file_2_on_change_bevestig_disp,// display
    0,                              // cursor
    &dummy_value,                   // *value
    Dummy_Func,                     // void (*number)(void); 
    Arrow_SD_Card_Options_Bevestig, // void (*arrow)(void); 
    Enter_SD_Card_Options_Bevestig, // void (*enter)(void);
  },
  { // log file 2
    31,                               // nr
    0,                                // index
    (unsigned char *)&log_file[2].used, // option
    (unsigned char *)&option_index_0,   // Optie index 
    lcd_sd_card_log_file_2_disp,      // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Change_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  { // log file 3 on
    40,                               // nr
    0,                                // index
    (unsigned char *)&log_file[3].used, // option
    (unsigned char *)&option_index_0,   // Optie index 
    lcd_sd_card_log_file_3_on_disp,      // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Func_SD_Card_Setpoints,     // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  { // log file 3 on
    40,                               // nr
    1,                                // index
    (unsigned char *)&log_file[3].used, // option
    (unsigned char *)&option_index_0,   // Optie index 
    lcd_sd_card_log_file_3_on_change_disp,   // display
    &disp_cursor_log_on,           // cursor
    &key_sd_card_log_file_3_on_value, // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_SD_Card_Setpoints,  // void (*arrow)(void); 
    Enter_SD_Card_Setpoints,  // void (*enter)(void);
  },
  {
    40,                             // nr
    2,                              // index
    &log_file[3].used,              // option
    (unsigned char *)&option_index_0,   // Optie index 
    lcd_sd_card_log_file_3_on_change_bevestig_disp,// display
    0,                              // cursor
    &dummy_value,                   // *value
    Dummy_Func,                     // void (*number)(void); 
    Arrow_SD_Card_Setpoints_Bevestig,// void (*arrow)(void); 
    Enter_SD_Card_Setpoints_Bevestig,// void (*enter)(void);
  },
  { // log file 3
    41,                               // nr
    0,                                // index
    (unsigned char *)&log_file[3].used, // option
    (unsigned char *)&option_index_0,   // Optie index 
    lcd_sd_card_log_file_3_disp,      // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Change_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  { // log file 4 on
    50,                               // nr
    0,                                // index
    (unsigned char *)&log_file[4].used, // option
    (unsigned char *)&option_index_0,   // Optie index 
    lcd_sd_card_log_file_4_on_disp,      // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Change_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  { // log file 4 on
    50,                               // nr
    1,                                // index
    (unsigned char *)&log_file[4].used, // option
    (unsigned char *)&option_index_0,   // Optie index 
    lcd_sd_card_log_file_4_on_disp,   // display
    &disp_cursor_log_on,           // cursor
    &key_sd_card_log_file_4_on_value, // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Scroll_Value,               // void (*arrow)(void); 
    Increment_Func_Index_Enter_Value, // void (*enter)(void);
  },
  { // log file 4
    51,                               // nr
    0,                                // index
    (unsigned char *)&log_file[4].used, // option
    (unsigned char *)&option_index_0,   // Optie index 
    lcd_sd_card_log_file_4_disp,      // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Change_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  { // log file 5 on
    60,                               // nr
    0,                                // index
    (unsigned char *)&log_file[5].used, // option
    (unsigned char *)&option_index_0,   // Optie index 
    lcd_sd_card_log_file_5_on_disp,      // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Change_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  { // log file 3 on
    60,                               // nr
    1,                                // index
    (unsigned char *)&log_file[5].used, // option
    (unsigned char *)&option_index_0,   // Optie index 
    lcd_sd_card_log_file_5_on_disp,   // display
    &disp_cursor_log_on,           // cursor
    &key_sd_card_log_file_5_on_value, // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Scroll_Value,               // void (*arrow)(void); 
    Increment_Func_Index_Enter_Value, // void (*enter)(void);
  },
  { // log file 5
    61,                               // nr
    0,                                // index
    (unsigned char *)&log_file[5].used, // option
    (unsigned char *)&option_index_0,   // Optie index 
    lcd_sd_card_log_file_5_disp,      // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Change_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  { // log file 6 on
    70,                               // nr
    0,                                // index
    (unsigned char *)&log_file[6].used, // option
    (unsigned char *)&option_index_0,   // Optie index 
    lcd_sd_card_log_file_6_on_disp,      // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Change_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  { // log file 3 on
    70,                               // nr
    1,                                // index
    (unsigned char *)&log_file[6].used, // option
    (unsigned char *)&option_index_0,   // Optie index 
    lcd_sd_card_log_file_6_on_disp,   // display
    &disp_cursor_log_on,           // cursor
    &key_sd_card_log_file_6_on_value, // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Scroll_Value,               // void (*arrow)(void); 
    Increment_Func_Index_Enter_Value, // void (*enter)(void);
  },
  { // log file 6
    71,                               // nr
    0,                                // index
    (unsigned char *)&log_file[6].used, // option
    (unsigned char *)&option_index_0,   // Optie index 
    lcd_sd_card_log_file_6_disp,      // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Change_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  { // log file 7 on
    80,                               // nr
    0,                                // index
    (unsigned char *)&log_file[7].used, // option
    (unsigned char *)&option_index_0,   // Optie index 
    lcd_sd_card_log_file_7_on_disp,      // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Change_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  { // log file 7 on
    80,                               // nr
    1,                                // index
    (unsigned char *)&log_file[7].used, // option
    (unsigned char *)&option_index_0,   // Optie index 
    lcd_sd_card_log_file_7_on_disp,   // display
    &disp_cursor_log_on,           // cursor
    &key_sd_card_log_file_7_on_value, // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Scroll_Value,               // void (*arrow)(void); 
    Increment_Func_Index_Enter_Value, // void (*enter)(void);
  },
  { // log file 7
    81,                               // nr
    0,                                // index
    (unsigned char *)&log_file[7].used, // option
    (unsigned char *)&option_index_0,   // Optie index 
    lcd_sd_card_log_file_7_disp,      // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Change_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  { // log file 8 on
    90,                               // nr
    0,                                // index
    (unsigned char *)&log_file[8].used, // option
    (unsigned char *)&option_index_0,   // Optie index 
    lcd_sd_card_log_file_8_on_disp,      // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Change_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  { // log file 8 on
    90,                               // nr
    1,                                // index
    (unsigned char *)&log_file[8].used, // option
    (unsigned char *)&option_index_0,   // Optie index 
    lcd_sd_card_log_file_8_on_disp,   // display
    &disp_cursor_log_on,           // cursor
    &key_sd_card_log_file_8_on_value, // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Scroll_Value,               // void (*arrow)(void); 
    Increment_Func_Index_Enter_Value, // void (*enter)(void);
  },
  { // log file 8
    91,                               // nr
    0,                                // index
    (unsigned char *)&log_file[8].used, // option
    (unsigned char *)&option_index_0,   // Optie index 
    lcd_sd_card_log_file_8_disp,      // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Change_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
/*
  { // log file 9 on
    100,                               // nr
    0,                                // index
    (unsigned char *)&log_file[9].used, // option
    (unsigned char *)&option_index_0,   // Optie index 
    lcd_sd_card_log_file_9_on_disp,      // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Change_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  { // log file 9 on
    100,                               // nr
    1,                                // index
    (unsigned char *)&log_file[9].used, // option
    (unsigned char *)&option_index_0,   // Optie index 
    lcd_sd_card_log_file_9_on_disp,   // display
    &disp_cursor_log_on,           // cursor
    &key_sd_card_log_file_9_on_value, // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Scroll_Value,               // void (*arrow)(void); 
    Increment_Func_Index_Enter_Value, // void (*enter)(void);
  },
  { // log file 9
    101,                              // nr
    0,                                // index
    (unsigned char *)&log_file[9].used, // option
    (unsigned char *)&option_index_0,   // Optie index 
    lcd_sd_card_log_file_9_disp,      // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Change_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
*/
};

s_screen screen_diag_sd_card_1;
s_screen const screen_diag_sd_card_1_default =
{
  1, // functie nr
  0, // index
  3, // max rel
  0, // rel actief
  0, // nummer weergegeven bin aantal ingeschakelde functie nummers
  0, // aantal ingeschakelde functie nummers
  lcd_disp_header,
  {0,0,0,0,0,0},
  0,3,19,
  0,3,19+28,
  0,3,19+28+28,
  &diag_sd_card_1_key_action[0], // first_action
  &diag_sd_card_1_key_action[sizeof(diag_sd_card_1_key_action)/sizeof(s_key_action) - 1], // last action 
  0, // change_flag
  0,
  0,
  0,
  0,
  0,
  0,
  &screen_diagnose_0
};

static void Disp_Option_Selection(void)
{
  if ((selection_opt_nr < 0) || 
      (selection_opt_nr > 3))
    selection_opt_nr = 0;  
  switch (selection_opt_nr)
  {
    default: 
    case 0: // options algemeen
      sprintf(tekst_log_file[OPT].string, "OPTA.%03i", opt_alg.adres);
      break;
    case 1: // options IO
      sprintf(tekst_log_file[OPT].string, "OPTI.%03i", opt_alg.adres);
      break;
    case 2: // option application
      sprintf(tekst_log_file[OPT].string, "OPTP.%03i", opt_alg.adres);
      break;
  }
}


void Control_Screen_Diag_SD_Card_1(void)
// zorg voor een correcte inhoud in klimaat scherm 0
{
s_tekst_13 const tekst_log_file_empty = { 13, 60, SIZE_10, "" };
int loop;

  Control_Screen(&screen_diag_sd_card_1, &screen_diag_sd_card_1_default, 1, 3);
  selection_opt_nr = 0;
  selection_opt_nr_max = 3;
  for (loop = 0; loop < 10; loop++)
  {
    tekst_log_file[loop] = tekst_log_file_empty;
    strncpy(tekst_log_file[loop].string, (char *)log_file[loop].file_name, 12); 
  }  
  Disp_Option_Selection();
}

void If_Exist_Goto_Screen_Diag_SD_Card_1(void)
{
  Control_Screen_Diag_SD_Card_1();
  if (screen_diag_sd_card_1.nr_aantal)
    Next_Screen(&screen_diag_sd_card_1);
}

static void Arrow_Func_SD_Card_Active(void)
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
      Prev_Screen();
      break;
    case RIGHT:
      if ((password_enabled & PASSWORD_ENABLED_SETP_MASK) == 0)
        Right_Password();
      else
      {
        help_sd_card = setp_alg.sd_card_status;
        Increment_Func_Index();
      }  
      break;
  }
}

static void Arrow_SD_Card_Active(void)
{
  switch (key)
  {
    case UP:
      Increment_Scroll_Value();
      switch (help_sd_card)
      {
        case 0: // sd card active
          break;
        case 1: // sd card ask remove
          if ((sd_placed_disp == 0) || (setp_alg.sd_card_status == 2))
            Increment_Scroll_Value();
          break;
        case 2: // sd card remove
          if ((setp_alg.sd_card_status == 0) || (setp_alg.sd_card_status == 1))
            Increment_Scroll_Value();
          break;
      }
//      if (((help_sd_card == 1) && (sd_card_remove == 2)) ||
//          ((help_sd_card == 2) && ((sd_card_remove == 1) || (sd_card_remove == 0))))
//        Increment_Scroll_Value();
      break;
    case DOWN:
      Decrement_Scroll_Value();
      switch (help_sd_card)
      {
        case 0: // sd card active
          break;
        case 1: // sd card ask remove
          if ((sd_placed_disp == 0) || (setp_alg.sd_card_status == 2))
            Decrement_Scroll_Value();
          break;
        case 2: // sd card remove
          if (((setp_alg.sd_card_status == 0) && (sd_placed_disp == 1)) || (setp_alg.sd_card_status == 1))
            Decrement_Scroll_Value();
          break;
      }
      break;
    case LEFT:
    case RIGHT:
      Increment_Func_Index();
      break;
  }
}

static void Enter_SD_Card_Active(void)
{
  setp_alg.sd_card_status = help_sd_card;
  Increment_Func_Index_Enter_Value();
}

//*****************************************************************************
static void Arrow_Func_SD_Card_Setpoints(void)
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
      Prev_Screen();
      break;
    case RIGHT:
      if ((setp_alg.sd_card_status == 0) && (sd_placed_disp == 1))
      {
        if ((password_enabled & PASSWORD_ENABLED_SETP_MASK) == 0)
          Right_Password();
        else if ((read_write_setpoints == 0) && (read_write_options == 0))
        {
          help_sd_card = read_write_setpoints;
          Increment_Func_Index();
        }  
      }  
      break;
  }
}

static void Arrow_SD_Card_Setpoints(void)
{
  switch (key)
  {
    case UP:
      Increment_Scroll_Value();
      break;
    case DOWN:
      Decrement_Scroll_Value();
      break;
    case LEFT:
    case RIGHT:
      Decrement_Func_Index();
      break;
  }
}

static void Enter_SD_Card_Setpoints(void)
{
  if (help_sd_card == 2)
    Increment_Func_Index();
  else
  {
    read_write_setpoints = help_sd_card;
    Decrement_Func_Index();
  }  
}

static void Arrow_SD_Card_Setpoints_Bevestig(void)
{
  switch (key)
  {
    case LEFT:
      Increment_Func_Index();
      break;
    case RIGHT:
      Increment_Func_Index();
      break;  
    case UP:
      read_write_setpoints = help_sd_card;
      Increment_Func_Index();
      break;  
    case DOWN:
      Increment_Func_Index();
      break;
  }
}

static void Enter_SD_Card_Setpoints_Bevestig(void)
{
  Increment_Func_Index();
}

//*****************************************************************************
static void Arrow_Func_SD_Card_Options(void)
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
      Prev_Screen();
      break;
    case RIGHT:
      if ((setp_alg.sd_card_status == 0) && (sd_placed_disp == 1))
      {
        if ((password_enabled & PASSWORD_ENABLED_SETP_MASK) == 0)
          Right_Password();
        else if ((read_write_setpoints == 0) && (read_write_options == 0))
        {
          help_sd_card = read_write_options;
          Increment_Func_Index();
        }  
      }  
      break;
  }
}

static void Arrow_SD_Card_Options(void)
{
  switch (key)
  {
    case UP:
      Increment_Scroll_Value();
      break;
    case DOWN:
      Decrement_Scroll_Value();
      break;
    case LEFT:
    case RIGHT:
      Decrement_Func_Index();
      break;
  }
}

static void Enter_SD_Card_Options(void)
{
  if (help_sd_card == 2)
    Increment_Func_Index();
  else
  {
    read_write_options = help_sd_card;
    Decrement_Func_Index();
  }  
}

static void Arrow_SD_Card_Options_Bevestig(void)
{
  switch (key)
  {
    case LEFT:
      Increment_Func_Index();
      break;
    case RIGHT:
      Increment_Func_Index();
      break;  
    case UP:
      read_write_options = help_sd_card;
      Increment_Func_Index();
      break;  
    case DOWN:
      Increment_Func_Index();
      break;
  }
}

static void Enter_SD_Card_Options_Bevestig(void)
{
  Increment_Func_Index();
}

#endif // SD_CARD
