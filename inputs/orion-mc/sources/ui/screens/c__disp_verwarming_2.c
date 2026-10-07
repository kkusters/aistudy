// C__DISP_VERWARMING_2.C  

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
#include "ch_disp_verwarming_1.h"
#include "ch_disp_verwarming_2.h"

static void Prev_Next_Func(void);

//*****************************************************************************
// SCHERM OPBOUW
//*****************************************************************************
//-----------------------------------------------------------------------------
// Header
static s_disp_agri_header const disp_header = { Disp_Draw_Agri_Header, FN2, HD_SYST, &tekst.Verwarming_10, HK_RECHT, &LuchtmengkastGroepNummer, 2};

static void * const lcd_disp_header[] = { &disp_header, 0};
//-----------------------------------------------------------------------------
// Algemeen
static s_disp_tekst const disp_14_perc = { Disp_Draw_Tekst_L, 210, 13, &tekst.perc_7 };
//-----------------------------------------------------------------------------
// Stap verwarming naregelen
static s_disp_tekst const disp_stapgrootte_str = { Disp_Draw_Tekst_L, 37, 20, &tekst.Stapgrootte_10 };
static s_disp_value const disp_stapgrootte_val = { Disp_Draw_Value,  207, 20, (SIZE_14 | RECHTS), UCHAR, 0, &setp_alg.LuchtmengkastGroep[0].VerwarmingStap };

static void * const lcd_stapgrootte_disp[] = { &disp_verwarm, &disp_stapgrootte_str, &disp_stapgrootte_val, &disp_14_perc, 0 };

static s_key_value const key_stapgrootte_val = { UCHAR, 3, &setp_alg.LuchtmengkastGroep[0].VerwarmingStap, &uchar_0, &uchar_100 };
//-----------------------------------------------------------------------------
// Cyclustijd verwarming naregelen
static s_disp_tekst const disp_cyclustijd_str = { Disp_Draw_Tekst_L,  37, 20, &tekst.Cycletime_10 };
static s_disp_value const disp_cyclustijd_val = { Disp_Draw_Value,   207, 20, (SIZE_14 | RECHTS), UCHAR, 0, &setp_alg.LuchtmengkastGroep[0].VerwarmingCyclustijd };
static s_disp_tekst const disp_cyclustijd_sec = { Disp_Draw_Tekst_L, 210, 20, &tekst.s_10 };

static void * const lcd_cyclustijd_disp[] = { &disp_cyclustijd, &disp_cyclustijd_str, &disp_cyclustijd_val, &disp_cyclustijd_sec, 0 };

static s_key_value const key_cyclustijd_val = { UCHAR, 3, &setp_alg.LuchtmengkastGroep[0].VerwarmingCyclustijd, &uchar_0, &uchar_240 };
//-----------------------------------------------------------------------------
// Hysterese
static s_disp_tekst const disp_hysterese_str = { Disp_Draw_Tekst_L, 37, 20, &tekst.Hysterese_10 };
static s_disp_value const disp_hysterese_val = { Disp_Draw_Value,  207, 20, (SIZE_14 | RECHTS), UCHAR, 1, &setp_alg.LuchtmengkastGroep[0].VerwarmingHysterese };

static void * const lcd_hysterese_disp[] = { &disp_hysterese, &disp_hysterese_str, &disp_hysterese_val, &disp_14_graden, 0 };

static s_key_value const key_hysterese_val = { UCHAR, 3, &setp_alg.LuchtmengkastGroep[0].VerwarmingHysterese, &uchar_0, &uchar_200 };
//-----------------------------------------------------------------------------
// Bandbreedte
static s_disp_tekst const disp_bandbreedte_str = { Disp_Draw_Tekst_L, 37, 20, &tekst.Bandbreedte_10 };
static s_disp_value const disp_bandbreedte_val = { Disp_Draw_Value,  207, 20, (SIZE_14 | RECHTS), UCHAR, 1, &setp_alg.LuchtmengkastGroep[0].VerwarmingBandbreedte };

static void * const lcd_bandbreedte_disp[] = { &disp_bandbreedte_ico, &disp_bandbreedte_str, &disp_bandbreedte_val, &disp_14_graden, 0 };

static s_key_value const key_bandbreedte_val = { UCHAR, 3, &setp_alg.LuchtmengkastGroep[0].VerwarmingBandbreedte, &uchar_0, &uchar_200 };
//-----------------------------------------------------------------------------


s_key_action const verwarming_2_key_action[] =
{
  {	// Stapgrootte verwarming naregelen
    1,
    0,
    &opt_app.LuchtmengkastGroep[0].Naregelen,
    &LuchtmengkastGroepIndex,
    lcd_stapgrootte_disp,
    0,
	&dummy_value,
    Dummy_Func,
    Arrow_Change_Syst_Func,           // void (*arrow)(void); 
    Dummy_Func,
  },
  {
    1,
    1,
    &opt_app.LuchtmengkastGroep[0].Naregelen,
    &LuchtmengkastGroepIndex,
    lcd_stapgrootte_disp,
    &disp_cursor_207_23_8,
	&key_stapgrootte_val,
    Number_Value,
    Arrow_Value,
    Increment_Func_Index_Enter_Value,
  },
  {	// Cyclustijd verwarming naregelen
    2,
    0,
    &opt_app.LuchtmengkastGroep[0].Naregelen,
    &LuchtmengkastGroepIndex,
    lcd_cyclustijd_disp,
    0,
	&dummy_value,
    Dummy_Func,
    Arrow_Change_Syst_Func,           // void (*arrow)(void); 
    Dummy_Func,
  },
  {
    2,
    1,
    &opt_app.LuchtmengkastGroep[0].Naregelen,
    &LuchtmengkastGroepIndex,
    lcd_cyclustijd_disp,
    &disp_cursor_207_23_8,
	&key_cyclustijd_val,
    Number_Value,
    Arrow_Value,
    Increment_Func_Index_Enter_Value,
  },
  {	// Hysterese verwarming naregelen
    3,
    0,
    &opt_app.LuchtmengkastGroep[0].Naregelen,
    &LuchtmengkastGroepIndex,
    lcd_hysterese_disp,
    0,
	&dummy_value,
    Dummy_Func,
    Arrow_Change_Syst_Func,           // void (*arrow)(void); 
    Dummy_Func,
  },
  {
    3,
    1,
    &opt_app.LuchtmengkastGroep[0].Naregelen,
    &LuchtmengkastGroepIndex,
    lcd_hysterese_disp,
    &disp_cursor_207_23_8,
	&key_hysterese_val,
    Number_Value,
    Arrow_Value,
    Increment_Func_Index_Enter_Value,
  },
  {	// Bandbreedte verwarming naregelen
    4,
    0,
    &opt_app.LuchtmengkastGroep[0].Naregelen,
    &LuchtmengkastGroepIndex,
    lcd_bandbreedte_disp,
    0,
	&dummy_value,
    Dummy_Func,
    Arrow_Change_Syst_Func,           // void (*arrow)(void); 
    Dummy_Func,
  },
  {
    4,
    1,
    &opt_app.LuchtmengkastGroep[0].Naregelen,
    &LuchtmengkastGroepIndex,
    lcd_bandbreedte_disp,
    &disp_cursor_207_23_8,
	&key_bandbreedte_val,
    Number_Value,
    Arrow_Value,
    Increment_Func_Index_Enter_Value,
  },
};

s_screen screen_verwarming_2;
s_screen const screen_verwarming_2_default =
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
  &verwarming_2_key_action[0], // first_action
  &verwarming_2_key_action[sizeof(verwarming_2_key_action)/sizeof(s_key_action) - 1], // last action 
  0, // change_flag
  0, // punt in normaal display
  0, // punt tijdens invoeren
  0, // aantal digits ingevoerd (punt telt niet mee)
  0, // value
  0, // min value
  0, // max value
  &screen_verwarming_1, // vorige scherm
  &Prev_Next_Func  // prev_next_func
};

void Control_Screen_Verwarming_2(void)
{
  Control_Screen(&screen_verwarming_2, &screen_verwarming_2_default, 1, 3);
}

void If_Exist_Goto_Screen_Verwarming_2(void)
{
  Control_Screen_Verwarming_2();
  if (screen_verwarming_2.nr_aantal)
    Next_Screen(&screen_verwarming_2);
}

//================================================================================
static void Prev_Next_Func(void)
{
unsigned char OldIndex;
s_screen *screen_tmp;

  OldIndex = LuchtmengkastGroepIndex;
  screen_tmp = screen_ptr;
  screen_ptr = screen_ptr->prev_screen;
  screen_ptr = screen_ptr->prev_screen;
  Prev_Next_Luchtmengkast_Func();
  screen_ptr = screen_tmp;
  if (OldIndex != LuchtmengkastGroepIndex)
    Control_Screen_Verwarming_2();
}




