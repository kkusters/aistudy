// C__DISP_DIAG_ASC1_1.C

#include "ch_define.h"

#if (PROCESSOR==XC161CJ)

#include "ch_asc1.h"
#include "ch_const.h"
#include "ch_disp.h"
#include "ch_disp_diagnose_0.h"
#include "ch_disp_func.h"
#include "ch_disp_value.h"
#include "ch_key.h"
#include "ch_lcd_7.h"
#include "ch_lcd_10.h"
#include "ch_lcd_14.h"
#include "ch_lcd_bitmap.h"
#include "ch_lcd_pixel.h"
#include "ch_string.h"
#include "ch_disp_diag_asc1_1.h"

extern s_screen screen_diag_asc1_1;

static void Enter_Reset_Diag_Asc1(void);

static s_disp_agri_header const disp_header =              { Disp_Draw_Agri_Header, FN5, EMPTY, &tekst.Diag_COM2_10, HK_GEEN, 0, 2};
static void * const lcd_disp_header[] = { &disp_header, 0};

static s_disp_bitmap_array const disp_com2_bmp       = { Disp_Draw_Bitmap_Array, 9, 0, ico_pc_modem_big_array, UCHAR, &opt_alg.com2_modem, 2 };
static s_disp_tekst const disp_com2_str              = { Disp_Draw_Tekst_L, 37, 20, &tekst.COM2_10 };
static s_disp_value  const disp_baudrate_val         = { Disp_Draw_Value,  192, 20, (SIZE_14 | RECHTS), INT, 1, &opt_alg.com2_bd };
static s_disp_tekst const disp_baudrate_unit         = { Disp_Draw_Tekst_L, 195, 20, &tekst.kBd_14 };
static s_disp_tekst const disp_out_cnt_str           = { Disp_Draw_Tekst_L, 37, 20, &tekst.TX_10 };
static s_disp_value const disp_out_cnt_val           = { Disp_Draw_Value,  147, 20, (SIZE_14 | RECHTS), UINT, 0, &asc1_diag_out_cnt };
static s_disp_value const disp_out_cnt_peak_val      = { Disp_Draw_Value,  207, 20, (SIZE_14 | RECHTS), UINT, 0, &asc1_diag_out_cnt_peak };
static s_disp_tekst const disp_in_cnt_str            = { Disp_Draw_Tekst_L, 37, 20, &tekst.RX_10 };
static s_disp_value const disp_in_cnt_val            = { Disp_Draw_Value,  147, 20, (SIZE_14 | RECHTS), UINT, 0, &asc1_diag_in_cnt };
static s_disp_value const disp_in_cnt_peak_val       = { Disp_Draw_Value,  207, 20, (SIZE_14 | RECHTS), UINT, 0, &asc1_diag_in_cnt_peak };
static s_disp_tekst const disp_in_out_cnt_str        = { Disp_Draw_Tekst_L, 37, 20, &tekst.TX_RX_10 };
static s_disp_value const disp_in_out_cnt_val        = { Disp_Draw_Value,  147, 20, (SIZE_14 | RECHTS), UINT, 0, &asc1_diag_in_out_cnt };
static s_disp_value const disp_in_out_cnt_peak_val   = { Disp_Draw_Value,  207, 20, (SIZE_14 | RECHTS), UINT, 0, &asc1_diag_in_out_cnt_peak };
static s_disp_tekst const disp_at_str                = { Disp_Draw_Tekst_L, 37, 20, &tekst.AT_10 };
static s_disp_bitmap_array const disp_at_bmp         = { Disp_Draw_Bitmap_Array,  190, 6, bmp_false_true, UCHAR, &asc1_send_at, 2 };
static s_disp_tekst const disp_modem_init_str        = { Disp_Draw_Tekst_L, 37, 20, &tekst.Modem_Init_10 };
static s_disp_bitmap_array const disp_modem_init_bmp = { Disp_Draw_Bitmap_Array,  190, 6, bmp_false_true, UCHAR, &asc1_modem_init, 2 };
static s_disp_tekst const disp_modem_dcd_str         = { Disp_Draw_Tekst_L, 37, 20, &tekst.Modem_DCD_10 };
static void const * const tekst_ok_not[]        = { &tekst_ok_10, &tekst_leeg };
static s_disp_tekst_array const disp_dcd_val         = { Disp_Draw_Tekst_Array_L, 150, 20, &tekst_ok_not, UCHAR, &asc1_dcd, 2 };

static s_disp_bitmap_array const disp_set_dcd_bmp    = { Disp_Draw_Bitmap_Array,  190, 6, bmp_false_true, UCHAR, &asc1_modem_dcd, 2 };

static void * const lcd_baudrate_disp[]   = { &disp_com2_bmp, &disp_com2_str, &disp_baudrate_val, &disp_baudrate_unit, 0 };
static void * const lcd_in_cnt_disp[]     = { &disp_com2_bmp, &disp_in_cnt_str, &disp_in_cnt_val, &disp_in_cnt_peak_val, 0 };
static void * const lcd_out_cnt_disp[]    = { &disp_com2_bmp, &disp_out_cnt_str, &disp_out_cnt_val, &disp_out_cnt_peak_val, 0 };
static void * const lcd_in_out_cnt_disp[] = { &disp_com2_bmp, &disp_in_out_cnt_str, &disp_in_out_cnt_val, &disp_in_out_cnt_peak_val, 0 };
static void * const lcd_at_disp[]         = { &disp_com2_bmp, &disp_at_str, &disp_at_bmp, 0 };
static void * const lcd_modem_dcd_disp[]  = { &disp_com2_bmp, &disp_modem_dcd_str, &disp_dcd_val, &disp_set_dcd_bmp, 0 };
static void * const lcd_modem_init_disp[] = { &disp_com2_bmp, &disp_modem_init_str, &disp_modem_init_bmp, 0 };

static s_key_value const key_at_value = { UCHAR, 1, &asc1_send_at, &uchar_0, &uchar_1 };
static s_key_value const key_modem_dcd_value = { UCHAR, 1, &asc1_modem_dcd, &uchar_0, &uchar_1 };
static s_key_value const key_modem_init_value = { UCHAR, 1, &asc1_modem_init, &uchar_0, &uchar_1 };

s_key_action const diag_asc1_1_key_action[] =
{
  { // Baudrate
    1,                              // nr
    0,                              // index
    (unsigned char *)&option_on,    // option
    (unsigned char *)&option_index_0,// Optie index 
    lcd_baudrate_disp,              // display
    0,                              // cursor
    &dummy_value,                   // *value
    Dummy_Func,                     // void (*number)(void); 
    Arrow_Change_Func,              // void (*arrow)(void); 
    Enter_Reset_Diag_Asc1,          // void (*enter)(void);
  },
  { // TXOK
    2,                              // nr
    0,                              // index
    (unsigned char *)&option_on,    // option
    (unsigned char *)&option_index_0,// Optie index 
    lcd_out_cnt_disp,               // display
    0,                              // cursor
    &dummy_value,                   // *value
    Dummy_Func,                     // void (*number)(void); 
    Arrow_Change_Func,              // void (*arrow)(void); 
    Enter_Reset_Diag_Asc1,          // void (*enter)(void);
  },
  { // RXOK
    3,                              // nr
    0,                              // index
    (unsigned char *)&option_on,    // option
    (unsigned char *)&option_index_0,// Optie index 
    lcd_in_cnt_disp,                // display
    0,                              // cursor
    &dummy_value,                   // *value
    Dummy_Func,                     // void (*number)(void); 
    Arrow_Change_Func,              // void (*arrow)(void); 
    Enter_Reset_Diag_Asc1,          // void (*enter)(void);
  },
  { // TXOK + RXOK
    4,                              // nr
    0,                              // index
    (unsigned char *)&option_on,    // option
    (unsigned char *)&option_index_0,// Optie index 
    lcd_in_out_cnt_disp,            // display
    0,                              // cursor
    &dummy_value,                   // *value
    Dummy_Func,                     // void (*number)(void); 
    Arrow_Change_Func,              // void (*arrow)(void); 
    Enter_Reset_Diag_Asc1,          // void (*enter)(void);
  },
  { // Send AT
    5,                              // nr
    0,                              // index
    &opt_alg.com2_modem,            // option
    (unsigned char *)&option_index_0,// Optie index 
    lcd_at_disp,                    // display
    0,                              // cursor
    &dummy_value,                   // *value
    Dummy_Func,                     // void (*number)(void); 
    Arrow_Change_Func,              // void (*arrow)(void); 
    Enter_Reset_Diag_Asc1,          // void (*enter)(void);
  },
  {	
    5,                              // nr
    1,                              // index
    &opt_alg.com2_modem,            // option
    (unsigned char *)&option_index_0,// Optie index 
    lcd_at_disp,                    // display
    &disp_cursor_207_27_18,         // cursor
	&key_at_value,                  // *value
    Dummy_Func,                     // void (*number)(void); 
    Arrow_Value,                    // void (*arrow)(void); 
	Increment_Func_Index_Enter_Value,// void (*enter)(void);
  },
  { // Set modem DCD
    6,                              // nr
    0,                              // index
    &opt_alg.com2_modem,            // option
    (unsigned char *)&option_index_0,// Optie index 
    lcd_modem_dcd_disp,             // display
    0,                              // cursor
    &dummy_value,                   // *value
    Dummy_Func,                     // void (*number)(void); 
    Arrow_Change_Func,              // void (*arrow)(void); 
    Enter_Reset_Diag_Asc1,          // void (*enter)(void);
  },
  {	
    6,                              // nr
    1,                              // index
    &opt_alg.com2_modem,            // option
    (unsigned char *)&option_index_0,// Optie index 
    lcd_modem_dcd_disp,             // display
    &disp_cursor_207_27_18,         // cursor
	&key_modem_dcd_value,           // *value
    Dummy_Func,                     // void (*number)(void); 
    Arrow_Value,                    // void (*arrow)(void); 
	Increment_Func_Index_Enter_Value,// void (*enter)(void);
  },
  { // modem init
    7,                              // nr
    0,                              // index
    &opt_alg.com2_modem,            // option
    (unsigned char *)&option_index_0,// Optie index 
    lcd_modem_init_disp,            // display
    0,                              // cursor
    &dummy_value,                   // *value
    Dummy_Func,                     // void (*number)(void); 
    Arrow_Change_Func,              // void (*arrow)(void); 
    Enter_Reset_Diag_Asc1,          // void (*enter)(void);
  },
  {	
    7,                              // nr
    1,                              // index
    &opt_alg.com2_modem,            // option
    (unsigned char *)&option_index_0,// Optie index 
    lcd_modem_init_disp,            // display
    &disp_cursor_207_27_18,         // cursor
	&key_modem_init_value,          // *value
    Dummy_Func,                     // void (*number)(void); 
    Arrow_Value,                    // void (*arrow)(void); 
	Increment_Func_Index_Enter_Value,// void (*enter)(void);
  },
};

s_screen screen_diag_asc1_1;
s_screen const screen_diag_asc1_1_default =
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
  &diag_asc1_1_key_action[0], // first_action
  &diag_asc1_1_key_action[sizeof(diag_asc1_1_key_action)/sizeof(s_key_action) - 1], // last action 
  0, // change_flag
  0,
  0,
  0,
  0,
  0,
  0,
  &screen_diagnose_0,
  0 // function for prev and next key
};


void Control_Screen_Diag_Asc1_1(void)
// zorg voor een correcte inhoud in klimaat scherm 0
{
  Control_Screen(&screen_diag_asc1_1, &screen_diag_asc1_1_default, 1, 3);
}

void If_Exist_Goto_Screen_Diag_Asc1_1(void)
{
  Control_Screen_Diag_Asc1_1();
  if (screen_diag_asc1_1.nr_aantal)
    Next_Screen(&screen_diag_asc1_1);
}

static void Enter_Reset_Diag_Asc1(void)
{
  asc1_diag_reset_flag = 1;
}

#endif // (PROCESSOR==XC161CJ)
