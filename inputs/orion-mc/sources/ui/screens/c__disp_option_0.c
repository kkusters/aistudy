// C__DISP_OPTION_0.C

#include "ch_define.h"

#include "ch_alarm.h"
#include "ch_const.h"
#include "ch_data.h"
#include "ch_disp.h"
#include "ch_disp_func.h"
#include "ch_disp_option_1.h"
#include "ch_disp_password.h"
#include "ch_disp_value.h"
#include "ch_key.h"
#include "ch_lcd_10.h"
#include "ch_lcd_bitmap.h"
#include "ch_lcd_pixel.h"
#include "ch_string.h"
#include "ch_disp_option_0.h"

static void Arrow_Bekijk_Func(void);
static void Arrow_Wijzig_Func(void);
static void Orion_On_Off_Arrow(void);

unsigned char install_flag = 0;

static s_disp_agri_header const disp_header = { Disp_Draw_Agri_Header, FN6, EMPTY, &tekst_inst.Opties_10, HK_GEEN, 0, 1};
static void * const lcd_disp_header[] = { &disp_header, 0 };

static s_disp_tekst const disp_bekijk_string   = { Disp_Draw_Tekst_L, 37, 22, &tekst_inst.Bekijken_Opties_14 };
static s_disp_tekst const disp_wijzig_string   = { Disp_Draw_Tekst_L, 37, 22, &tekst_inst.Wijzigen_Opties_14 };
static void const * const tekst_on_off[2] = { &tekst_inst.Orion_Uitgeschakeld_14, &tekst_inst.Orion_Ingeschakeld_14};
static s_disp_tekst_array const disp_on_off    = { Disp_Draw_Tekst_Array_L, 37, 22, &tekst_on_off, UCHAR, &setp_alg.regelaar_on, 2 };

static void * const lcd_bekijk_disp[] = { &disp_bril, &disp_bekijk_string, 0 };
static void * const lcd_wijzig_disp[] = { &disp_pen, &disp_wijzig_string, 0 };
static void * const lcd_on_off_disp[] = { &disp_orion, &disp_on_off, 0 };

static s_key_value const key_on_off_value = { UCHAR, 2, &setp_alg.regelaar_on, &uchar_0, &uchar_1 };

s_key_action const option_0_key_action[] =
{
  {
    1,               // nr
    0,               // index
    (unsigned char *)&option_on,      // option
    (unsigned char *)&option_index_0,   // Optie index 
    lcd_bekijk_disp,     // display
    0,               // cursor
    &dummy_value,     // *value
    Dummy_Func,      // void (*number)(void); 
    Arrow_Bekijk_Func, // void (*arrow)(void); 
    Dummy_Func,      // void (*enter)(void);
  },
  {
    2,               // nr
    0,               // index
    (unsigned char *)&option_on,     // option
    (unsigned char *)&option_index_0,   // Optie index 
    lcd_wijzig_disp,     // display
    0,               // cursor
    &dummy_value,     // *value
    Dummy_Func,      // void (*number)(void); 
    Arrow_Wijzig_Func, // void (*arrow)(void); 
    Dummy_Func,      // void (*enter)(void);
  },
  {
    3,               // nr
    0,               // index
    (unsigned char *)&option_on,     // option
    (unsigned char *)&option_index_0,   // Optie index 
    lcd_on_off_disp,     // display
    0,               // cursor
    &dummy_value,     // *value
    Dummy_Func,      // void (*number)(void); 
    Arrow_Change_Func, // void (*arrow)(void); 
    Dummy_Func       // void (*enter)(void);
  },
  {
    3,               // nr
    1,               // index
    (unsigned char *)&option_on,     // option
    (unsigned char *)&option_index_0,   // Optie index 
    lcd_on_off_disp,     // display
    &disp_cursor_221_25_185,   // cursor
    &key_on_off_value,     // *value
    Dummy_Func,      // void (*number)(void); 
    Orion_On_Off_Arrow,      // void (*arrow)(void); 
    Increment_Func_Index,      // void (*enter)(void);
  }
};

s_screen screen_option_0;
s_screen const screen_option_0_default =
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
  &option_0_key_action[0], // first_action
  &option_0_key_action[sizeof(option_0_key_action)/sizeof(s_key_action) - 1], // last action 
  0, // change_flag
  0, // punt in normaal display
  0, // punt tijdens invoeren
  0, // aantal digits ingevoerd (punt telt niet mee)
  0, // value
  0, // min value
  0, // max value
  0, // vorige scherm
  0  // prev_next_func
};

void Control_Screen_Option_0(void)
// zorg voor een correcte inhoud in klimaat scherm 0
{
  Control_Screen(&screen_option_0, &screen_option_0_default, 1, 3);
}

static void Arrow_Bekijk_Func(void)
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
      break;
    case RIGHT:
      Control_Screen_Option_1();
      Next_Screen(&screen_option_1);
      #ifdef PASSWORD
	  password_enabled = password_enabled_delay = 0;
      #else // PASSWORD
      password_enabled = password_enabled_delay = password_installateur_enabled = password_gebruiker_enabled = 0;
      #endif // PASSWORD
      break;
  }
}

static void Arrow_Wijzig_Func(void) // standaard functie voor hoofdschermen (bv klim_0_screen)
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
      break;
    case RIGHT:
      Control_Screen_Option_1();
      Right_Option_0_Password();
      break;
  }
}

static void Orion_On_Off_Arrow(void)
{
  switch (key)
  {
    case UP:
      Increment_Scroll_Value();
      if (setp_alg.regelaar_on == 1)
      {
//      CreateAlarm(&alarm_hr_alg.orion_on_al);
      }
      else
	    val_hr_alg.orion_switched_off = 1;
      break;
    case DOWN:
      Decrement_Scroll_Value();
      if (setp_alg.regelaar_on == 1)
      {
//      CreateAlarm(&alarm_hr_alg.orion_on_al);
      }
      else
	    val_hr_alg.orion_switched_off = 1;
      break;
    case LEFT:
      Increment_Func_Index();
      break;
    case RIGHT:
      Increment_Func_Index();
      break;
  }
}
