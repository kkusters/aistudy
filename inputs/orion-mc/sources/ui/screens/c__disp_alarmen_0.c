// C__DISP_ALARMEN_0.C

#include "ch_define.h"

#include "ch_alarm.h"
#include "ch_const.h"
#include "ch_disp.h"
#include "ch_disp_alarmen_actief_1.h"
#include "ch_disp_alarmen_historie_1.h"
#include "ch_disp_func.h"
#include "ch_lcd_10.h"
#include "ch_lcd_bitmap.h"
#include "ch_lcd_pixel.h"
#include "ch_string.h"
#include "ch_disp_alarmen_0.h"

static void Enter_Reset_Func(void);

static s_disp_agri_header const disp_header = { Disp_Draw_Agri_Header, FN4, EMPTY, &tekst.Alarmen_10, HK_GEEN, 0, 1};

static s_disp_tekst const disp_alarmen_actief =   { Disp_Draw_Tekst_L, 37, 20, &tekst.Alarmen_Actief_10 };
static s_disp_tekst const disp_alarmen_historie = { Disp_Draw_Tekst_L, 37, 20, &tekst.Alarmen_Historie_10 };
static s_disp_tekst const disp_alarmen_wissen =   { Disp_Draw_Tekst_L, 37, 20, &tekst.Alarmen_Wissen_10 };

static void * const lcd_disp_header[] = { &disp_header, 0 };
static void * const lcd_alarmen_actief[] =   { &disp_alarm_on, &disp_alarmen_actief, 0 };
static void * const lcd_alarmen_historie[] = { &disp_kalender, &disp_alarmen_historie, 0 };
static void * const lcd_alarmen_wissen[] =   { &disp_afvalemmer, &disp_alarmen_wissen, 0 };
static void * const lcd_alarmen_wissen_1[] = 
{ 
  &disp_afvalemmer, &disp_alarmen_wissen, &disp_messagebox_bevestig, 
  &disp_zeker_weten_str, 
  &disp_ok_str, &disp_space_10_L, &disp_is_teken_10_L, &disp_space_10_L, &disp_wissen_str, 0
};


s_key_action const alarmen_0_key_action[] =
{
  {
    1,    // nr
    0,               // index
    (unsigned char *)&option_on,      // option
    (unsigned char *)&option_index_0,   // Optie index 
    lcd_alarmen_actief,	 // display
    0,               // cursor
    &dummy_value,    // *value
    Dummy_Func,    // void (*number)(void); 
    Arrow_Goto_Alarm_Actief,    // void (*arrow)(void); 
    Dummy_Func,    // void (*enter)(void);
  },
  {
    2,    // nr
    0,               // index
    (unsigned char *)&option_on,      // option
    (unsigned char *)&option_index_0,   // Optie index 
    lcd_alarmen_historie,	 // display
    0,               // cursor
    &dummy_value,    // *value
    Dummy_Func,    // void (*number)(void); 
    Arrow_Goto_Alarm_Historie, // void (*arrow)(void); 
    Dummy_Func,    // void (*enter)(void);
  },
  {
    3,    // nr
    0,               // index
    (unsigned char *)&option_on,      // option
    (unsigned char *)&option_index_0,   // Optie index 
    lcd_alarmen_wissen,	 // display
    0,               // cursor
    &dummy_value,    // *value
    Dummy_Func,    // void (*number)(void); 
    Arrow_Change_Func,    // void (*arrow)(void); 
    Dummy_Func,    // void (*enter)(void);
  },
  {
    3,    // nr
    1,               // index
    (unsigned char *)&option_on,      // option
    (unsigned char *)&option_index_0,   // Optie index 
    lcd_alarmen_wissen_1,	 // display
    0,               // cursor
    &dummy_value,    // *value
    Increment_Func_Index,    // void (*number)(void); 
    Increment_Func_Index,    // void (*arrow)(void); 
    Enter_Reset_Func,    // void (*enter)(void);
  },
};

s_screen screen_alarmen_0;
s_screen const screen_alarmen_0_default =
{
  1, // functie nr
  0, // index
  3, // max rel
  0, // rel actief
  0, // nummer weergegeven bin aantal ingeschakelde functie nummers
  0, // aantal ingeschakelde functie nummers
  lcd_disp_header,
  {0,0,0,0,0,0},
  0,3,19,       // rel[0]
  0,3,19+28,    // rel[1]
  0,3,19+28+28, // rel[2]
  &alarmen_0_key_action[0], // first_action
  &alarmen_0_key_action[sizeof(alarmen_0_key_action)/sizeof(s_key_action) - 1], // last action 
  0, // change_flag
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0  // prev_next_func
};

void Control_Screen_Alarmen_0(void)
// zorg voor een correcte inhoud in klimaat scherm 0
{
  Control_Screen(&screen_alarmen_0, &screen_alarmen_0_default, 0, 3);
}

static void Enter_Reset_Func(void)
{
  Alarm_Reset_Data_Algemeen();
  val_hr_alg.orion_switched_off = 0;
  Increment_Func_Index();
}
