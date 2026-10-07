// C__DISP_KIERSTURING_1.C  

#include <string.h>

#include "ch_define.h"

#include "ch_alg.h"
#include "ch_const.h"
#include "ch_disp.h"
#include "ch_disp_F1_0.h"
#include "ch_disp_func.h"
#include "ch_disp_password.h"
#include "ch_disp_value.h"
#include "ch_key.h"
#include "ch_lcd_10.h"
#include "ch_lcd_14.h"
#include "ch_lcd_bitmap.h"
#include "ch_lcd_pixel.h"
#include "ch_string.h"
#include "ch_tijd.h"
#include "ch_disp_kiersturing_1.h"

static void Prev_Next_Func(void);

static void const * const tekst_kiersturing_10[] =
{
  &tekst.Twee_doeken_een_bed_1_10, &tekst.Twee_doeken_een_bed_2_10, &tekst.Twee_doeken_een_bed_3_10, &tekst.Twee_doeken_een_bed_4_10,
  &tekst.Twee_doeken_een_bed_5_10, &tekst.Twee_doeken_een_bed_6_10, &tekst.Twee_doeken_een_bed_7_10, &tekst.Twee_doeken_een_bed_8_10
};

static s_disp_agri_header const disp_header = { Disp_Draw_Agri_Header, FN1, EMPTY, &tekst_leeg, HK_GEEN, 0, 2};
static s_disp_tekst           const disp_header_str_0  = { Disp_Draw_Tekst_L,  12, 12, &tekst.Fn_10 };
static s_disp_tekst_array_add const disp_header_str_1  = { Disp_Draw_Tekst_Array_Add_L, &tekst_kiersturing_10, UCHAR, &DoekIndex, MAX_SCREEN };
static void * const lcd_disp_header[] = { &disp_header_str_0, &disp_space_10_L, &disp_header_str_1, &disp_header, 0};

unsigned char DoekIndex;

//*****************************************************************************
// STRING ARRAY'S 
//*****************************************************************************
static void const * const tekst_kier_voorloop_10[] =
{
  &tekst.Kier_10, &tekst.Voorloop_10
};

//*****************************************************************************
// SCHERM OPBOUW
//*****************************************************************************
//-----------------------------------------------------------------------------
// basis opbouw lcd
/*
static s_disp_tekst           const disp_header_str_0  = { Disp_Draw_Tekst_L,  12, 12, &tekst.Fn_10 };
static s_disp_tekst_array_add const disp_header_str_1  = { Disp_Draw_Tekst_Array_Add_L, &tekst_kiersturing_10, UCHAR, &DoekIndex, MAX_SCREEN };
static s_disp_tekst           const disp_header_str_2  = { Disp_Draw_Tekst_L, 204, 12, &header_string_2 };
static s_disp_value           const disp_header_val_2  = { Disp_Draw_Value,   237, 12, (SIZE_7 | RECHTS), INT, 0, &screen_kiersturing_1.nr };
static s_disp_block           const disp_header_invert = { Disp_Invert_Block,       0,  0, 239, 16 };
static s_disp_block           const disp_header_line_0 = { Disp_Draw_Black_Block,   0, 13,   0,104 }; // vertikaal
static s_disp_block           const disp_header_line_1 = { Disp_Draw_Black_Block, 239, 13, 239,104 }; // vertikaal
static s_disp_block           const disp_header_line_2 = { Disp_Draw_Black_Block,  44,104, 238,104 }; 

static void * const lcd_disp_header[] = { &disp_10_slot, &disp_header_str_0, &disp_space_10_L, &disp_header_str_1,
                                          &disp_header_str_2, &disp_header_val_2,
                                          &disp_bus_ok_ico,
                                          &disp_header_invert,
                                          &disp_header_line_0, &disp_header_line_1, &disp_header_line_2,
                                          &disp_loper,
                                          &disp_tab_begin,  &disp_fn_F1, 
                                          &disp_tab_2_norm, &disp_fn_F2,
                                          &disp_tab_3_norm, &disp_fn_F3,
                                          &disp_tab_4_norm, &disp_fn_alarm,
                                          &disp_tab_5_norm, &disp_fn_diagnose,
                                          &disp_tab_6_norm, &disp_fn_opties,
                                          &disp_tab_end, 0 };
*/
//-----------------------------------------------------------------------------
// Algemeen
static s_disp_tekst const disp_14_perc = { Disp_Draw_Tekst_L, 210, 13, &tekst.perc_7 };
//-----------------------------------------------------------------------------
// Kier/Voorloop
static s_disp_tekst_array const disp_kier_str = { Disp_Draw_Tekst_Array_L, 37, 20, &tekst_kier_voorloop_10, UCHAR, &opt_app.DualScreen[0].CombiMatic, 2 };
static s_disp_value       const disp_kier_val = { Disp_Draw_Value,        207, 20, (SIZE_14 | RECHTS), INT, 1, &setp_alg.DualScreen[0].Opening };

static void * const lcd_kier_disp[] = { &disp_twee_doek_een_bed_ico, &disp_kier_str, &disp_kier_val, &disp_14_perc, 0 };

static s_key_value const key_kier_val = { INT, 3, &setp_alg.DualScreen[0].Opening, &int_5, &int_1000 };
//-----------------------------------------------------------------------------
// Hysteresis
static s_disp_tekst const disp_hysteresis_str = { Disp_Draw_Tekst_L, 37, 20, &tekst.Hysteresis_10 };
static s_disp_value const disp_hysteresis_val = { Disp_Draw_Value,  207, 20, (SIZE_14 | RECHTS), INT, 1, &setp_alg.DualScreen[0].Hysteresis };

static void * const lcd_hysteresis_disp[] = { &disp_hysterese, &disp_hysteresis_str, &disp_hysteresis_val, &disp_14_perc, 0 };

static s_key_value const key_hysteresis_val = { INT, 3, &setp_alg.DualScreen[0].Hysteresis, &int_2, &setp_alg.DualScreen[0].Opening };
//-----------------------------------------------------------------------------
// Standby time
static s_disp_tekst const disp_standby_str = { Disp_Draw_Tekst_L,  37, 20, &tekst.Standby_10 };
static s_disp_value const disp_standby_val = { Disp_Draw_Value,   207, 20, (SIZE_14 | RECHTS), INT, 0, &setp_alg.DualScreen[0].StandbyTime };
static s_disp_tekst const disp_minuut_str  = { Disp_Draw_Tekst_L, 210, 20, &tekst.minuut_7 };

static void * const lcd_standby_disp[] = { &disp_klok_15x15_ico, &disp_standby_str, &disp_standby_val, &disp_minuut_str, 0 };

static s_key_value const key_standby_val = { INT, 2, &setp_alg.DualScreen[0].StandbyTime, &int_1, &int_999 };
//-----------------------------------------------------------------------------

s_key_action const kiersturing_1_key_action[] =
{
  { // Kier
    1,                                // nr
    0,                                // index
    (unsigned char *)&option_on,      // option
    &DoekIndex,                       // option
    lcd_kier_disp,                    // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Change_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    1,                                // nr
    1,                                // index
    (unsigned char *)&option_on,      // option
    &DoekIndex,                       // option
    lcd_kier_disp,                    // display
    &disp_cursor_207_23_8,            // cursor
    &key_kier_val,                    // *value
    Number_Value,                     // void (*number)(void); 
    Arrow_Value,                      // void (*arrow)(void); 
    Increment_Func_Index_Enter_Value, // void (*enter)(void);
  },
  { // Hysteresis
    2,                                // nr
    0,                                // index
    (unsigned char *)&option_on,      // option
    &DoekIndex,                       // option
    lcd_hysteresis_disp,              // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Change_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    2,                                // nr
    1,                                // index
    (unsigned char *)&option_on,      // option
    &DoekIndex,                       // option
    lcd_hysteresis_disp,              // display
    &disp_cursor_207_23_8,            // cursor
    &key_hysteresis_val,              // *value
    Number_Value,                     // void (*number)(void); 
    Arrow_Value,                      // void (*arrow)(void); 
    Increment_Func_Index_Enter_Value, // void (*enter)(void);
  },
  { // Standby time
    3,                                // nr
    0,                                // index
    &opt_app.DualScreen[0].Standby,   // option
    &DoekIndex,                       // option
    lcd_standby_disp,                 // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Change_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    3,                                // nr
    1,                                // index
    &opt_app.DualScreen[0].Standby,   // option
    &DoekIndex,                       // option
    lcd_standby_disp,                 // display
    &disp_cursor_207_23_8,            // cursor
    &key_standby_val,                 // *value
    Number_Value,                     // void (*number)(void); 
    Arrow_Value,                      // void (*arrow)(void); 
    Increment_Func_Index_Enter_Value, // void (*enter)(void);
  },
};

s_screen screen_kiersturing_1;
s_screen const screen_kiersturing_1_default =
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
  &kiersturing_1_key_action[0], // first_action
  &kiersturing_1_key_action[sizeof(kiersturing_1_key_action)/sizeof(s_key_action) - 1], // last action 
  0, // change_flag
  0, // punt in normaal display
  0, // punt tijdens invoeren
  0, // aantal digits ingevoerd (punt telt niet mee)
  0, // value
  0, // min value
  0, // max value
  &screen_F1_0,   // vorige scherm
  &Prev_Next_Func // prev_next_func
};

void Control_Screen_Kiersturing_1(void)
{
  Control_Screen(&screen_kiersturing_1, &screen_kiersturing_1_default, 1, 3);
}

void If_Exist_Goto_Screen_Kiersturing_1(unsigned char nr)
{
  DoekIndex = nr;
  Control_Screen_Kiersturing_1();
  if (screen_kiersturing_1.nr_aantal)
    Next_Screen(&screen_kiersturing_1);
}

//================================================================================
static void Prev_Next_Func(void)
{
s_screen *screen_tmp;

  if (screen_ptr->index == 0)
  {
    switch (key_func)
    {
      case PREV:
        if (DoekIndex > 0)
        {
          DoekIndex--;
          Control_Screen_Kiersturing_1();
          screen_tmp = screen_ptr;
          screen_ptr = screen_ptr->prev_screen;
          Decrement_Func();
          screen_ptr = screen_tmp;
        }
        break;
      case NEXT:
        if (((DoekIndex + 1) < MAX_SCREEN) && (opt_app.DualScreen[DoekIndex + 1].Enabled))
        {
          DoekIndex++;
          Control_Screen_Kiersturing_1();
          screen_tmp = screen_ptr;
          screen_ptr = screen_ptr->prev_screen;
          Increment_Func();
          screen_ptr = screen_tmp;
        }
        break;
    }
  }
}




