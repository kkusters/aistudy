// C__DISP_BOVENKLEP_2.C  

#include <string.h>

#include "ch_define.h"

#include "ch_const.h"
#include "ch_disp.h"
#include "ch_disp_func.h"
#include "ch_disp_luchtmengkast_1.h"
#include "ch_disp_password.h"
#include "ch_disp_value.h"
#include "ch_key.h"
#include "ch_lcd_10.h"
#include "ch_lcd_14.h"
#include "ch_lcd_bitmap.h"
#include "ch_lcd_pixel.h"
#include "ch_string.h"
#include "ch_tijd.h"
#include "ch_disp_bovenklep_1.h"
#include "ch_disp_bovenklep_2.h"

static void Prev_Next_Func(void);

//*****************************************************************************
// SCHERM OPBOUW
//*****************************************************************************
//-----------------------------------------------------------------------------
// Header
static s_disp_agri_header const disp_header = { Disp_Draw_Agri_Header, FN2, HD_SYST, &tekst.Bovenklep_10, HK_RECHT, &LuchtmengkastGroepNummer, 3};

static void * const lcd_disp_header[] = { &disp_header, 0};
//-----------------------------------------------------------------------------
// Algemeen
static s_disp_tekst const disp_14_perc = { Disp_Draw_Tekst_L, 210, 13, &tekst.perc_7 };
//-----------------------------------------------------------------------------


s_key_action const bovenklep_2_key_action[] =
{
  {
    0,
    0,
    (unsigned char *)&option_off,
    (unsigned char *)&option_index_0,
    lcd_dummy_disp,
    0,
	&dummy_value,
    Dummy_Func,
    Dummy_Func,
    Dummy_Func,
  },
};

s_screen screen_bovenklep_2;
s_screen const screen_bovenklep_2_default =
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
  &bovenklep_2_key_action[0], // first_action
  &bovenklep_2_key_action[sizeof(bovenklep_2_key_action)/sizeof(s_key_action) - 1], // last action 
  0, // change_flag
  0, // punt in normaal display
  0, // punt tijdens invoeren
  0, // aantal digits ingevoerd (punt telt niet mee)
  0, // value
  0, // min value
  0, // max value
  &screen_bovenklep_1,   // vorige scherm
  Prev_Next_Func // prev_next_func
};

void Control_Screen_Bovenklep_2(void)
{
  Control_Screen(&screen_bovenklep_2, &screen_bovenklep_2_default, 1, 3);
}

void If_Exist_Goto_Screen_Bovenklep_2(void)
{
  Control_Screen_Bovenklep_2();
  if (screen_bovenklep_2.nr_aantal)
    Next_Screen(&screen_bovenklep_2);
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
    Control_Screen_Bovenklep_2();
}

