// C__DISP_BOVENKLEP_1.C  

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
#include "ch_disp_bovenklep_1.h"

static void Prev_Next_Func(void);

static void Arrow_Open_Value(void);
static void Enter_Open_Value(void);

//*****************************************************************************
// SCHERM OPBOUW
//*****************************************************************************
//-----------------------------------------------------------------------------
// Header
static s_disp_agri_header const disp_header = { Disp_Draw_Agri_Header, FN2, HD_FN, &tekst.Bovenklep_10, HK_RECHT, &LuchtmengkastGroepNummer, 2};

static void * const lcd_disp_header[] = { &disp_header, 0};
//-----------------------------------------------------------------------------
// Algemeen
static s_disp_tekst const disp_14_perc = { Disp_Draw_Tekst_L, 210, 13, &tekst.perc_7 };
//-----------------------------------------------------------------------------
// Bovenklep open (gekoppeld aan klep)
static s_disp_tekst const disp_bovenklep_open_str = { Disp_Draw_Tekst_L, 37, 20, &tekst.Bovenklep_open_10 };
static s_disp_value const disp_bovenklep_open_val = { Disp_Draw_Value,  207, 20, (SIZE_14 | RECHTS), UCHAR, 0, &setp_alg.LuchtmengkastGroep[0].KlepstandAan };

static void * const lcd_bovenklep_open_disp[] = { &disp_luchtmengkast_ico, &disp_bovenklep_open_str, &disp_bovenklep_open_val, &disp_14_perc, 0 };

static s_key_value const key_bovenklep_open_val = { UCHAR, 3, &setp_alg.LuchtmengkastGroep[0].KlepstandAan, &uchar_0, &uchar_100 };
//-----------------------------------------------------------------------------
// Bovenklep dicht (gekoppeld aan klep)
static s_disp_tekst const disp_bovenklep_dicht_str = { Disp_Draw_Tekst_L, 37, 20, &tekst.Bovenklep_dicht_10 };
static s_disp_value const disp_bovenklep_dicht_val = { Disp_Draw_Value,  207, 20, (SIZE_14 | RECHTS), UCHAR, 0, &setp_alg.LuchtmengkastGroep[0].KlepstandUit };

static void * const lcd_bovenklep_dicht_disp[] = { &disp_luchtmengkast_ico, &disp_bovenklep_dicht_str, &disp_bovenklep_dicht_val, &disp_14_perc, 0 };

static s_key_value const key_bovenklep_dicht_val = { UCHAR, 3, &setp_alg.LuchtmengkastGroep[0].KlepstandUit, &uchar_0, &setp_alg.LuchtmengkastGroep[0].KlepstandAan };
//-----------------------------------------------------------------------------


s_key_action const bovenklep_1_key_action[] =
{
  {	// Bovenklep open
    1,
    0,
    &opt_app.LuchtmengkastGroep[0].BovenklepGekoppeldAanKlep,
    &LuchtmengkastGroepIndex,
    lcd_bovenklep_open_disp,
    0,
	&dummy_value,
    Dummy_Func,
    Arrow_Change_Syst_Func,
    Dummy_Func,
  },
  {
    1,
    1,
    &opt_app.LuchtmengkastGroep[0].BovenklepGekoppeldAanKlep,
    &LuchtmengkastGroepIndex,
    lcd_bovenklep_open_disp,
    &disp_cursor_207_23_8,
	&key_bovenklep_open_val,
    Number_Value,
    Arrow_Open_Value,
    Enter_Open_Value,
  },
  {	// Bovenklep dicht
    2,
    0,
    &opt_app.LuchtmengkastGroep[0].BovenklepGekoppeldAanKlep,
    &LuchtmengkastGroepIndex,
    lcd_bovenklep_dicht_disp,
    0,
	&dummy_value,
    Dummy_Func,
    Arrow_Change_Syst_Func,
    Dummy_Func,
  },
  {
    2,
    1,
    &opt_app.LuchtmengkastGroep[0].BovenklepGekoppeldAanKlep,
    &LuchtmengkastGroepIndex,
    lcd_bovenklep_dicht_disp,
    &disp_cursor_207_23_8,
	&key_bovenklep_dicht_val,
    Number_Value,
    Arrow_Value,
    Increment_Func_Index_Enter_Value,
  },
};

s_screen screen_bovenklep_1;
s_screen const screen_bovenklep_1_default =
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
  &bovenklep_1_key_action[0], // first_action
  &bovenklep_1_key_action[sizeof(bovenklep_1_key_action)/sizeof(s_key_action) - 1], // last action 
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

void Control_Screen_Bovenklep_1(void)
{
  Control_Screen(&screen_bovenklep_1, &screen_bovenklep_1_default, 1, 3);
}

void If_Exist_Goto_Screen_Bovenklep_1(void)
{
  Control_Screen_Bovenklep_1();
  if (screen_bovenklep_1.nr_aantal)
    Next_Screen(&screen_bovenklep_1);
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
    Control_Screen_Bovenklep_1();
}

//================================================================================
static void Arrow_Open_Value(void)
{
unsigned char Diff;
int Dicht;

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
	  Dicht = setp_alg.LuchtmengkastGroep[LuchtmengkastGroepIndex].KlepstandAan - Diff;
	  if (Dicht < 0)
	    Dicht = 0;
	  setp_alg.LuchtmengkastGroep[LuchtmengkastGroepIndex].KlepstandUit = Dicht;
      break;
	case DOWN:
	  Diff = setp_alg.LuchtmengkastGroep[LuchtmengkastGroepIndex].KlepstandAan - setp_alg.LuchtmengkastGroep[LuchtmengkastGroepIndex].KlepstandUit;
	  Decrement_Value();
	  Dicht = setp_alg.LuchtmengkastGroep[LuchtmengkastGroepIndex].KlepstandAan - Diff;
	  if (Dicht < 0)
	    Dicht = 0;
	  setp_alg.LuchtmengkastGroep[LuchtmengkastGroepIndex].KlepstandUit = Dicht;
	  break;
  }
}

static void Enter_Open_Value(void)
{
unsigned char Diff;
int Dicht;

  if (screen_ptr->change_flag)
  {
    Diff = setp_alg.LuchtmengkastGroep[LuchtmengkastGroepIndex].KlepstandAan - setp_alg.LuchtmengkastGroep[LuchtmengkastGroepIndex].KlepstandUit;
    Enter_Value();
	Dicht = setp_alg.LuchtmengkastGroep[LuchtmengkastGroepIndex].KlepstandAan - Diff;
	if (Dicht < 0)
	  Dicht = 0;
	setp_alg.LuchtmengkastGroep[LuchtmengkastGroepIndex].KlepstandUit = Dicht;
  }
  Increment_Func_Index();
}
                                
