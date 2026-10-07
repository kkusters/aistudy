// C__DISP_OPT_MOTORGROEPEN_2.C

#include "ch_define.h"

#include "ch_alg.h"
#include "ch_asc0.h"
#include "ch_const.h"
#include "ch_disp.h"
#include "ch_disp_func.h"
#include "ch_disp_opt_IO_Select.h"
#include "ch_disp_option_1.h"
#include "ch_disp_value.h" 
#include "ch_disp_password.h"
#include "ch_DS401.h"
#include "ch_key.h"
#include "ch_lcd_10.h"
#include "ch_lcd_14.h"
#include "ch_lcd_20.h"
#include "ch_lcd_bitmap.h"
#include "ch_lcd_pixel.h"
#include "ch_string.h" 
#include "ch_disp_opt_motorgroepen_2.h"

static void Arrow_Motorgroepen_Value(void);
static void Enter_Motorgroepen_Value(void);
static void Arrow_End_Func(void);

static s_disp_agri_header const disp_header = { Disp_Draw_Agri_Header, FN6, EMPTY, &tekst_inst.Opties_motorgroepen_10, HK_GEEN, 0, 3};
static void * const lcd_disp_header[] = { &disp_header, 0 };

static unsigned char MaxGroup = MAX_GROUP;
                                            
//*****************************************************************************
// SCHERM OPBOUW
//*****************************************************************************
// Aantal motorgroepen
static s_disp_tekst const disp_aantal_motorgroepen_str = { Disp_Draw_Tekst_L, 37, 22, &tekst_inst.Aantal_motorgroepen_14 };
static s_disp_value const disp_aantal_motorgroepen_val = { Disp_Draw_Value,  207, 75, (SIZE_14 | RECHTS), UCHAR, 0, &opt_app.NumberMotorgroups };

static void * const lcd_aantal_motorgroepen_disp[] = { &disp_motorgroepen_ico, &disp_aantal_motorgroepen_str, &disp_aantal_motorgroepen_val, 0 };

static s_key_value const key_aantal_motorgroepen_val = { UCHAR, 2, &opt_app.NumberMotorgroups, &uchar_0, &MaxGroup };
//-----------------------------------------------------------------------------


s_key_action const opt_motorgroepen_2_key_action[] =
{
  { // Begin scherm
    0,                                // nr
    0,                                // index
    &start_flag,                      // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_start_disp,                   // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Start_Func,                 // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  { // Aantal motorgroepen
    1,                                // nr
    0,                                // index
    (unsigned char *)&option_on,      // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_aantal_motorgroepen_disp,     // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Option_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    1,                                // nr
    1,                                // index
    (unsigned char *)&option_on,      // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_aantal_motorgroepen_disp,     // display
    &disp_cursor_207_78_8,            // cursor
    &key_aantal_motorgroepen_val,     // *value
    Number_Value,                     // void (*number)(void); 
    Arrow_Motorgroepen_Value,         // void (*arrow)(void); 
    Enter_Motorgroepen_Value,         // void (*enter)(void);
  },
  {
    99,                               // nr
    0,                                // index
    &end_flag,                        // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_end_disp,                     // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_End_Func,                   // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
};

//-----------------------------------------------------------------------------
s_screen screen_opt_motorgroepen_2;
s_screen const screen_opt_motorgroepen_2_default =
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
  &opt_motorgroepen_2_key_action[0], // first_action
  &opt_motorgroepen_2_key_action[sizeof(opt_motorgroepen_2_key_action)/sizeof(s_key_action) - 1], // last action 
  0, // change_flag
  0, // punt in normaal display
  0, // punt tijdens invoeren
  0, // aantal digits ingevoerd (punt telt niet mee)
  0, // value
  0, // min value
  0, // max value
  &screen_option_1,  // vorige scherm
  0  // prev_next_func
};

void Control_Screen_Option_Motorgroepen_2(void)
{
  if (password_enabled & PASSWORD_ENABLED_OPT_MASK)
    start_flag = end_flag = 1;
  else
    start_flag = end_flag = 0;
  Control_Screen(&screen_opt_motorgroepen_2, &screen_opt_motorgroepen_2_default, 1, 1);
}

//-----------------------------------------------------------------------------
static void OptionCheckMotorgroups(void)
{
unsigned char nr;

  for (nr = 0; nr < opt_app.NumberMotorgroups; nr++)
    opt_app.Motorgroup[nr].Enabled = 1;
  for (nr = opt_app.NumberMotorgroups; nr < MAX_GROUP; nr++)
    opt_app.Motorgroup[nr].Enabled = 0;
  CheckOptions();
}

static void Arrow_Motorgroepen_Value(void)
{
  Arrow_Option_Value();
  if (screen_ptr->index == 0)
    OptionCheckMotorgroups();
}

static void Enter_Motorgroepen_Value(void)
{
  Increment_Func_Index_Enter_Option_Value();
  if (screen_ptr->index == 0)
    OptionCheckMotorgroups();
}

//-----------------------------------------------------------------------------
static void Arrow_End_Func(void)
{
  switch (key)
  {
    case UP:
      Decrement_Func();
      break;
    case DOWN:
      break;
    case LEFT:
      if (option_change_flag)
      {
        option_change_flag_alg = 1;
        option_change_flag = 0;
      }
	  CheckOptions();
      Prev_Screen();
      Refresh_Screen_Nr_Aantal();
      break;
    case RIGHT:
      break;
  }
}
