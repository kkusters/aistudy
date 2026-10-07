// C__DISP_INBLAASVENT_1.C  

#include <string.h>

#include "ch_define.h"

#include "ch_const.h"
#include "ch_disp.h"
#include "ch_disp_func.h"
#include "ch_disp_password.h"
#include "ch_disp_value.h"
#include "ch_disp_luchtmengkast_1.h"
#include "ch_key.h"
#include "ch_lcd_10.h"
#include "ch_lcd_14.h"
#include "ch_lcd_bitmap.h"
#include "ch_lcd_pixel.h"
#include "ch_string.h"
#include "ch_tijd.h"
#include "ch_disp_inblaasvent_1.h"

static void Prev_Next_Func(void);

//*****************************************************************************
// SCHERM OPBOUW
//*****************************************************************************
//-----------------------------------------------------------------------------
// Header
static s_disp_agri_header const disp_header = { Disp_Draw_Agri_Header, FN2, HD_FN, &tekst.Inblaasvent_10, HK_RECHT, &LuchtmengkastGroepNummer, 2};

static void * const lcd_disp_header[] = { &disp_header, 0};
//-----------------------------------------------------------------------------
// Algemeen
static s_disp_tekst const disp_14_perc = { Disp_Draw_Tekst_L, 210, 13, &tekst.perc_7 };
//-----------------------------------------------------------------------------
// Inblaasventilator minimum
static s_disp_tekst const disp_inblaasvent_minimum_str = { Disp_Draw_Tekst_L, 37, 20, &tekst.Minimum_10 };
static s_disp_value const disp_inblaasvent_minimum_val = { Disp_Draw_Value,  207, 20, (SIZE_14 | RECHTS), UCHAR, 0, &setp_alg.LuchtmengkastGroep[0].InblaasventMinimum };

static void * const lcd_inblaasvent_minimum_disp[] = { &disp_vent, &disp_inblaasvent_minimum_str, &disp_inblaasvent_minimum_val, &disp_14_perc, 0 };

static s_key_value const key_inblaasvent_minimum_val = { UCHAR, 3, &setp_alg.LuchtmengkastGroep[0].InblaasventMinimum, &uchar_0, &uchar_100 };
//-----------------------------------------------------------------------------
// Inblaasventilator maximum
static s_disp_tekst const disp_inblaasvent_maximum_str = { Disp_Draw_Tekst_L, 37, 20, &tekst.Maximum_10 };
static s_disp_value const disp_inblaasvent_maximum_val = { Disp_Draw_Value,  207, 20, (SIZE_14 | RECHTS), UCHAR, 0, &setp_alg.LuchtmengkastGroep[0].InblaasventMaximum };

static void * const lcd_inblaasvent_maximum_disp[] = { &disp_vent, &disp_inblaasvent_maximum_str, &disp_inblaasvent_maximum_val, &disp_14_perc, 0 };

static s_key_value const key_inblaasvent_maximum_val = { UCHAR, 3, &setp_alg.LuchtmengkastGroep[0].InblaasventMaximum, &uchar_0, &uchar_100 };
//-----------------------------------------------------------------------------


s_key_action const inblaasvent_1_key_action[] =
{
  {	// Inblaasventilator minimum
    1,
    0,
    (unsigned char *)&option_on,
    &LuchtmengkastGroepIndex,
    lcd_inblaasvent_minimum_disp,
    0,
	&dummy_value,
    Dummy_Func,
    Arrow_Change_Syst_Func,
    Dummy_Func,
  },
  {
    1,
    1,
    (unsigned char *)&option_on,
    &LuchtmengkastGroepIndex,
    lcd_inblaasvent_minimum_disp,
    &disp_cursor_207_23_8,
	&key_inblaasvent_minimum_val,
    Number_Value,
    Arrow_Value,
    Increment_Func_Index_Enter_Value,
  },
  {	// Inblaasventilator maximum
    2,
    0,
    (unsigned char *)&option_on,
    &LuchtmengkastGroepIndex,
    lcd_inblaasvent_maximum_disp,
    0,
	&dummy_value,
    Dummy_Func,
    Arrow_Change_Syst_Func,
    Dummy_Func,
  },
  {
    2,
    1,
    (unsigned char *)&option_on,
    &LuchtmengkastGroepIndex,
    lcd_inblaasvent_maximum_disp,
    &disp_cursor_207_23_8,
	&key_inblaasvent_maximum_val,
    Number_Value,
    Arrow_Value,
    Increment_Func_Index_Enter_Value,
  },
};

s_screen screen_inblaasvent_1;
s_screen const screen_inblaasvent_1_default =
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
  &inblaasvent_1_key_action[0], // first_action
  &inblaasvent_1_key_action[sizeof(inblaasvent_1_key_action)/sizeof(s_key_action) - 1], // last action 
  0, // change_flag
  0, // punt in normaal display
  0, // punt tijdens invoeren
  0, // aantal digits ingevoerd (punt telt niet mee)
  0, // value
  0, // min value
  0, // max value
  &screen_luchtmengkast_1,   // vorige scherm
  Prev_Next_Func // prev_next_func
};

void Control_Screen_Inblaasvent_1(void)
{
  Control_Screen(&screen_inblaasvent_1, &screen_inblaasvent_1_default, 1, 3);
}

void If_Exist_Goto_Screen_Inblaasvent_1(void)
{
  Control_Screen_Inblaasvent_1();
  if (screen_inblaasvent_1.nr_aantal)
    Next_Screen(&screen_inblaasvent_1);
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
    Control_Screen_Inblaasvent_1();
}

                                
