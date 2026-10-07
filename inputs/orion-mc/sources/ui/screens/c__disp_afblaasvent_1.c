// C__DISP_AFBLAASVENT_1.C  

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
#include "ch_disp_afblaasvent_1.h"

static void Prev_Next_Func(void);

static void Arrow_Aan_Value(void);
static void Enter_Aan_Value(void);

//*****************************************************************************
// SCHERM OPBOUW
//*****************************************************************************
//-----------------------------------------------------------------------------
// Header
static s_disp_agri_header const disp_header = { Disp_Draw_Agri_Header, FN2, HD_FN, &tekst.Afblaasvent_10, HK_RECHT, &LuchtmengkastGroepNummer, 2};

static void * const lcd_disp_header[] = { &disp_header, 0};
//-----------------------------------------------------------------------------
// Algemeen
static s_disp_tekst const disp_14_perc = { Disp_Draw_Tekst_L, 210, 13, &tekst.perc_7 };
//-----------------------------------------------------------------------------
// Afblaasventilator aan (gekoppeld aan klep)
static s_disp_tekst const disp_afblaasvent_aan_str = { Disp_Draw_Tekst_L, 37, 20, &tekst.Afblaasvent_aan_10 };
static s_disp_value const disp_afblaasvent_aan_val = { Disp_Draw_Value,  207, 20, (SIZE_14 | RECHTS), UCHAR, 0, &setp_alg.LuchtmengkastGroep[0].KlepstandAan };

static void * const lcd_afblaasvent_aan_disp[] = { &disp_luchtmengkast_ico, &disp_afblaasvent_aan_str, &disp_afblaasvent_aan_val, &disp_14_perc, 0 };

static s_key_value const key_afblaasvent_aan_val = { UCHAR, 3, &setp_alg.LuchtmengkastGroep[0].KlepstandAan, &uchar_0, &uchar_100 };
//-----------------------------------------------------------------------------
// Afblaasventilator uit (gekoppeld aan klep)
static s_disp_tekst const disp_afblaasvent_uit_str = { Disp_Draw_Tekst_L, 37, 20, &tekst.Afblaasvent_uit_10 };
static s_disp_value const disp_afblaasvent_uit_val = { Disp_Draw_Value,  207, 20, (SIZE_14 | RECHTS), UCHAR, 0, &setp_alg.LuchtmengkastGroep[0].KlepstandUit };

static void * const lcd_afblaasvent_uit_disp[] = { &disp_luchtmengkast_ico, &disp_afblaasvent_uit_str, &disp_afblaasvent_uit_val, &disp_14_perc, 0 };

static s_key_value const key_afblaasvent_uit_val = { UCHAR, 3, &setp_alg.LuchtmengkastGroep[0].KlepstandUit, &uchar_0, &setp_alg.LuchtmengkastGroep[0].KlepstandAan };
//-----------------------------------------------------------------------------
// Afblaasventilator minimum
static s_disp_tekst const disp_afblaasvent_minimum_str = { Disp_Draw_Tekst_L, 37, 20, &tekst.Minimum_10 };
static s_disp_value const disp_afblaasvent_minimum_val = { Disp_Draw_Value,  207, 20, (SIZE_14 | RECHTS), UCHAR, 0, &setp_alg.LuchtmengkastGroep[0].AfblaasventMinimum };

static void * const lcd_afblaasvent_minimum_disp[] = { &disp_vent, &disp_afblaasvent_minimum_str, &disp_afblaasvent_minimum_val, &disp_14_perc, 0 };

static s_key_value const key_afblaasvent_minimum_val = { UCHAR, 3, &setp_alg.LuchtmengkastGroep[0].AfblaasventMinimum, &uchar_0, &uchar_100 };
//-----------------------------------------------------------------------------
// Afblaasventilator maximum
static s_disp_tekst const disp_afblaasvent_maximum_str = { Disp_Draw_Tekst_L, 37, 20, &tekst.Maximum_10 };
static s_disp_value const disp_afblaasvent_maximum_val = { Disp_Draw_Value,  207, 20, (SIZE_14 | RECHTS), UCHAR, 0, &setp_alg.LuchtmengkastGroep[0].AfblaasventMaximum };

static void * const lcd_afblaasvent_maximum_disp[] = { &disp_vent, &disp_afblaasvent_maximum_str, &disp_afblaasvent_maximum_val, &disp_14_perc, 0 };

static s_key_value const key_afblaasvent_maximum_val = { UCHAR, 3, &setp_alg.LuchtmengkastGroep[0].AfblaasventMaximum, &uchar_0, &uchar_100 };
//-----------------------------------------------------------------------------


s_key_action const afblaasvent_1_key_action[] =
{
  {	// Afblaasventilator aan
    1,
    0,
    &opt_app.LuchtmengkastGroep[0].AfblaasventGekoppeldAanKlep,
    &LuchtmengkastGroepIndex,
    lcd_afblaasvent_aan_disp,
    0,
	&dummy_value,
    Dummy_Func,
    Arrow_Change_Syst_Func,
    Dummy_Func,
  },
  {
    1,
    1,
    &opt_app.LuchtmengkastGroep[0].AfblaasventGekoppeldAanKlep,
    &LuchtmengkastGroepIndex,
    lcd_afblaasvent_aan_disp,
    &disp_cursor_207_23_8,
	&key_afblaasvent_aan_val,
    Number_Value,
    Arrow_Aan_Value,
    Enter_Aan_Value,
  },
  {	// Afblaasventilator uit
    2,
    0,
    &opt_app.LuchtmengkastGroep[0].AfblaasventGekoppeldAanKlep,
    &LuchtmengkastGroepIndex,
    lcd_afblaasvent_uit_disp,
    0,
	&dummy_value,
    Dummy_Func,
    Arrow_Change_Syst_Func,
    Dummy_Func,
  },
  {
    2,
    1,
    &opt_app.LuchtmengkastGroep[0].AfblaasventGekoppeldAanKlep,
    &LuchtmengkastGroepIndex,
    lcd_afblaasvent_uit_disp,
    &disp_cursor_207_23_8,
	&key_afblaasvent_uit_val,
    Number_Value,
    Arrow_Value,
    Increment_Func_Index_Enter_Value,
  },
  {	// Afblaasventilator minimum
    3,
    0,
    (unsigned char *)&option_on,
    &LuchtmengkastGroepIndex,
    lcd_afblaasvent_minimum_disp,
    0,
	&dummy_value,
    Dummy_Func,
    Arrow_Change_Syst_Func,
    Dummy_Func,
  },
  {
    3,
    1,
    (unsigned char *)&option_on,
    &LuchtmengkastGroepIndex,
    lcd_afblaasvent_minimum_disp,
    &disp_cursor_207_23_8,
	&key_afblaasvent_minimum_val,
    Number_Value,
    Arrow_Value,
    Increment_Func_Index_Enter_Value,
  },
  {	// Afblaasventilator maximum
    4,
    0,
    (unsigned char *)&option_on,
    &LuchtmengkastGroepIndex,
    lcd_afblaasvent_maximum_disp,
    0,
	&dummy_value,
    Dummy_Func,
    Arrow_Change_Syst_Func,
    Dummy_Func,
  },
  {
    4,
    1,
    (unsigned char *)&option_on,
    &LuchtmengkastGroepIndex,
    lcd_afblaasvent_maximum_disp,
    &disp_cursor_207_23_8,
	&key_afblaasvent_maximum_val,
    Number_Value,
    Arrow_Value,
    Increment_Func_Index_Enter_Value,
  },
};

s_screen screen_afblaasvent_1;
s_screen const screen_afblaasvent_1_default =
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
  &afblaasvent_1_key_action[0], // first_action
  &afblaasvent_1_key_action[sizeof(afblaasvent_1_key_action)/sizeof(s_key_action) - 1], // last action 
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

void Control_Screen_Afblaasvent_1(void)
{
  Control_Screen(&screen_afblaasvent_1, &screen_afblaasvent_1_default, 1, 3);
}

void If_Exist_Goto_Screen_Afblaasvent_1(void)
{
  Control_Screen_Afblaasvent_1();
  if (screen_afblaasvent_1.nr_aantal)
    Next_Screen(&screen_afblaasvent_1);
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
    Control_Screen_Afblaasvent_1();
}

//================================================================================
static void Arrow_Aan_Value(void)
{
unsigned char Diff;
int Uit;

  switch (key)
  {
	case LEFT:
	  Arrow_Left_Value();
	  break;
	case RIGHT:
	  Increment_Func_Index();
	  break;
    case UP:
	  Diff = setp_alg.LuchtmengkastGroep[LuchtmengkastGroepIndex].KlepstandAan - setp_alg.LuchtmengkastGroep[LuchtmengkastGroepIndex].KlepstandUit;
      Increment_Value();
	  Uit = setp_alg.LuchtmengkastGroep[LuchtmengkastGroepIndex].KlepstandAan - Diff;
	  if (Uit < 0)
	    Uit = 0;
	  setp_alg.LuchtmengkastGroep[LuchtmengkastGroepIndex].KlepstandUit = Uit;
      break;
	case DOWN:
	  Diff = setp_alg.LuchtmengkastGroep[LuchtmengkastGroepIndex].KlepstandAan - setp_alg.LuchtmengkastGroep[LuchtmengkastGroepIndex].KlepstandUit;
	  Decrement_Value();
	  Uit = setp_alg.LuchtmengkastGroep[LuchtmengkastGroepIndex].KlepstandAan - Diff;
	  if (Uit < 0)
	    Uit = 0;
	  setp_alg.LuchtmengkastGroep[LuchtmengkastGroepIndex].KlepstandUit = Uit;
	  break;
  }
}

static void Enter_Aan_Value(void)
{
unsigned char Diff;
int Uit;

  if (screen_ptr->change_flag)
  {
    Diff = setp_alg.LuchtmengkastGroep[LuchtmengkastGroepIndex].KlepstandAan - setp_alg.LuchtmengkastGroep[LuchtmengkastGroepIndex].KlepstandUit;
    Enter_Value();
	Uit = setp_alg.LuchtmengkastGroep[LuchtmengkastGroepIndex].KlepstandAan - Diff;
	if (Uit < 0)
	  Uit = 0;
	setp_alg.LuchtmengkastGroep[LuchtmengkastGroepIndex].KlepstandUit = Uit;
  }
  Increment_Func_Index();
}
                                
