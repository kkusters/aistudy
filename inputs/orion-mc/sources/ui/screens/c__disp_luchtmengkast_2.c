// C__DISP_LUCHTMENGKAST_2.C  

#include "ch_define.h"

#include "ch_const.h"
#include "ch_disp.h"
#include "ch_disp_func.h"
#include "ch_disp_luchtmengkast_1.h"
#include "ch_disp_value.h"
#include "ch_key.h"
#include "ch_lcd_10.h"
#include "ch_lcd_14.h"
#include "ch_lcd_bitmap.h"
#include "ch_lcd_pixel.h"
#include "ch_luchtmengkast.h"
#include "ch_string.h"
#include "ch_tijd.h"
#include "ch_disp_luchtmengkast_2.h"

static void Prev_Next_Func(void);

//*****************************************************************************
// SCHERM OPBOUW
//*****************************************************************************
//-----------------------------------------------------------------------------
// Header
static s_disp_agri_header const disp_header     = { Disp_Draw_Agri_Header, FN2, EMPTY, &tekst_leeg, HK_GEEN, 0, 3};
static s_disp_tekst       const disp_header_str = { Disp_Draw_Tekst_L, 12, 12, &tekst.Syst_Luchtmengkast_10 };
static s_disp_value_add   const disp_header_val = { Disp_Draw_Value_Add, (SIZE_10 | LINKS), UCHAR, 0, &LuchtmengkastGroepNummer };

static void * const lcd_disp_header[] = { &disp_header_str, &disp_rechte_openings_haak_10_L, &disp_header_val, &disp_rechte_sluit_haak_10_L, &disp_header, 0};
//-----------------------------------------------------------------------------
// Algemeen
static s_disp_tekst const disp_14_perc = { Disp_Draw_Tekst_L, 210, 13, &tekst.perc_7 };
//-----------------------------------------------------------------------------
// Wachttijd alarm handbediening
static s_disp_tekst const disp_alarm_manual_str = { Disp_Draw_Tekst_L,  37, 20, &tekst.Alarm_HAND_10 };
static s_disp_value const disp_alarm_manual_val = { Disp_Draw_Value,   207, 20, (SIZE_14 | RECHTS), UCHAR, 0, &setp_alg.LuchtmengkastGroep[0].DelayAlarmManual };
static s_disp_tekst const disp_minuut_str       = { Disp_Draw_Tekst_L, 210, 20, &tekst.minuut_7 };

static void * const lcd_delay_alarm_manual_disp[] = { &disp_alarm_delay_ico, &disp_alarm_manual_str, &disp_alarm_manual_val, &disp_minuut_str, 0 };

static s_key_value const key_alarm_manual_val = { UCHAR, 2, &setp_alg.LuchtmengkastGroep[0].DelayAlarmManual, &uchar_0, &uchar_60 };
//-----------------------------------------------------------------------------
// Alarm afwijking positie
static s_disp_tekst const disp_alarm_position_str = { Disp_Draw_Tekst_L,  37, 20, &tekst.Alarm_afwijking_10 };
static s_disp_value const disp_alarm_position_val = { Disp_Draw_Value,   207, 20, (SIZE_14 | RECHTS), UCHAR, 0, &setp_alg.LuchtmengkastGroep[0].DiffPositionAlarm };

static void * const lcd_alarm_position_disp[] = { &disp_alarm_position_ico, &disp_alarm_position_str, &disp_alarm_position_val, &disp_14_perc, 0 };

static s_key_value const key_alarm_position_val = { UCHAR, 3, &setp_alg.LuchtmengkastGroep[0].DiffPositionAlarm, &uchar_2, &uchar_250 };
//-----------------------------------------------------------------------------
// Alarm afwijking positie - tijd
static s_disp_tekst const disp_alarm_position_time_str = { Disp_Draw_Tekst_L,  37, 20, &tekst.Alarm_afwijking_10 };
static s_disp_value const disp_alarm_position_time_val = { Disp_Draw_Value,   207, 20, (SIZE_14 | RECHTS), UCHAR, 0, &setp_alg.LuchtmengkastGroep[0].TimePositionAlarm };

static void * const lcd_alarm_position_time_disp[] = { &disp_alarm_position_ico, &disp_alarm_position_time_str, &disp_alarm_position_time_val, &disp_minuut_str, 0 };

static s_key_value const key_alarm_position_time_val = { UCHAR, 3, &setp_alg.LuchtmengkastGroep[0].TimePositionAlarm, &uchar_1, &uchar_120 };
//-----------------------------------------------------------------------------


s_key_action const luchtmengkast_2_key_action[] =
{
  { // Wachttijd alarm handbediening
    1,                                      // nr
    0,                                      // index
    &opt_app.LuchtmengkastGroep[0].Enabled, // option
    &LuchtmengkastGroepIndex,               // option
    lcd_delay_alarm_manual_disp,            // display
    0,                                      // cursor
    &dummy_value,                           // *value
    Dummy_Func,                             // void (*number)(void); 
    Arrow_Change_Syst_Func,                 // void (*arrow)(void); 
    Dummy_Func,                             // void (*enter)(void);
  },
  {
    1,                                      // nr
    1,                                      // index
    &opt_app.LuchtmengkastGroep[0].Enabled, // option
    &LuchtmengkastGroepIndex,               // option
    lcd_delay_alarm_manual_disp,            // display
    &disp_cursor_207_23_8,                  // cursor
    &key_alarm_manual_val,                  // *value
    Number_Value,                           // void (*number)(void); 
    Arrow_Value,                            // void (*arrow)(void); 
    Increment_Func_Index_Enter_Value,       // void (*enter)(void);
  },
  { // Alarm afwijking positie
    2,                                      // nr
    0,                                      // index
    &opt_app.LuchtmengkastGroep[0].Enabled, // option
    &LuchtmengkastGroepIndex,               // option
    lcd_alarm_position_disp,                // display
    0,                                      // cursor
    &dummy_value,                           // *value
    Dummy_Func,                             // void (*number)(void); 
    Arrow_Change_Syst_Func,                 // void (*arrow)(void); 
    Dummy_Func,                             // void (*enter)(void);
  },
  {
    2,                                      // nr
    1,                                      // index
    &opt_app.LuchtmengkastGroep[0].Enabled, // option
    &LuchtmengkastGroepIndex,               // option
    lcd_alarm_position_disp,                // display
    &disp_cursor_207_23_8,                  // cursor
    &key_alarm_position_val,                // *value
    Number_Value,                           // void (*number)(void); 
    Arrow_Value,                            // void (*arrow)(void); 
    Increment_Func_Index_Enter_Value,       // void (*enter)(void);
  },
  { // Alarm afwijking positie - tijd
    3,                                      // nr
    0,                                      // index
    &opt_app.LuchtmengkastGroep[0].Enabled, // option
    &LuchtmengkastGroepIndex,               // option
    lcd_alarm_position_time_disp,           // display
    0,                                      // cursor
    &dummy_value,                           // *value
    Dummy_Func,                             // void (*number)(void); 
    Arrow_Change_Syst_Func,                 // void (*arrow)(void); 
    Dummy_Func,                             // void (*enter)(void);
  },
  {
    3,                                      // nr
    1,                                      // index
    &opt_app.LuchtmengkastGroep[0].Enabled, // option
    &LuchtmengkastGroepIndex,               // option
    lcd_alarm_position_time_disp,           // display
    &disp_cursor_207_23_8,                  // cursor
    &key_alarm_position_time_val,           // *value
    Number_Value,                           // void (*number)(void); 
    Arrow_Value,                            // void (*arrow)(void); 
    Increment_Func_Index_Enter_Value,       // void (*enter)(void);
  },
};

s_screen screen_luchtmengkast_2;
s_screen const screen_luchtmengkast_2_default =
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
  &luchtmengkast_2_key_action[0], // first_action
  &luchtmengkast_2_key_action[sizeof(luchtmengkast_2_key_action)/sizeof(s_key_action) - 1], // last action 
  0, // change_flag
  0, // punt in normaal display
  0, // punt tijdens invoeren
  0, // aantal digits ingevoerd (punt telt niet mee)
  0, // value
  0, // min value
  0, // max value
  &screen_luchtmengkast_1, // vorige scherm
  &Prev_Next_Func  // prev_next_func
};

void Control_Screen_Luchtmengkast_2(void)
{
  Control_Screen(&screen_luchtmengkast_2, &screen_luchtmengkast_2_default, 1, 3);
}

void If_Exist_Goto_Screen_Luchtmengkast_2(void)
{
  Control_Screen_Luchtmengkast_2();
  if (screen_luchtmengkast_2.nr_aantal)
    Next_Screen(&screen_luchtmengkast_2);
}

//================================================================================
static void Prev_Next_Func(void)
{
unsigned char OldIndex;
s_screen *screen_tmp;

  OldIndex = LuchtmengkastGroepIndex;
  screen_tmp = screen_ptr;
  screen_ptr = screen_ptr->prev_screen;
  Prev_Next_Luchtmengkast_Func();
  screen_ptr = screen_tmp;
  if (OldIndex != LuchtmengkastGroepIndex)
    Control_Screen_Luchtmengkast_2();
}




