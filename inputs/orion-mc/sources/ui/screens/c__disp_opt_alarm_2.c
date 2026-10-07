// C__DISP_OPT_ALARM_2.C

#include <string.h>
#include "ch_define.h"
#include "ch_const.h"
#include "ch_disp.h"
#include "ch_disp_func.h"
#include "ch_disp_option_1.h"
#include "ch_disp_opt_IO_Select.h"
#include "ch_disp_password.h"
#include "ch_disp_value.h"
#include "ch_key.h"
#include "ch_lcd_bitmap.h"
#include "ch_lcd_pixel.h"
#include "ch_string.h"
#include "ch_disp_opt_alarm_2.h"

static unsigned char DigOutAlarmNotUsed(s_board_IO_on_off IO_new);

//*****************************************************************************
// SCHERM OPBOUW
//*****************************************************************************
// Header
static s_disp_agri_header const disp_header = { Disp_Draw_Agri_Header, FN6, EMPTY, &tekst_inst.Opties_Alarmen_10, HK_GEEN, 0, 3};
static void * const lcd_disp_header[] = { &disp_header, 0};	  
//-----------------------------------------------------------------------------------------------------------
// Digitale uitgang zacht alarm
static s_disp_bitmap const disp_zacht_alarm_ico = { Disp_Draw_Bitmap,   9, 10, &ico_alarm };
static s_disp_tekst  const disp_zacht_alarm_str = { Disp_Draw_Tekst_L, 47, 22, &tekst_inst.Alarm_zacht_contact_14 };
static s_disp_board_IO_Selection const disp_dig_out_zacht_alarm_sel = { Disp_Draw_Board_IO_Select, &opt_app.Alarm.DigOutZacht, 1, (unsigned char *)&uchar_1, 1, DIGITAL_OUTPUT_ID, DIG_OUT_ALARM, DigOutAlarmNotUsed };

static void * const lcd_dig_out_zacht_alarm[] = { &disp_zacht_alarm_ico, &disp_zacht_alarm_str, &disp_dig_out_zacht_alarm_sel, 0 };
//-----------------------------------------------------------------------------

s_key_action const opt_alarm_2_key_action[] =
{
  {	//*** Begin scherm
    0,
    0,
    &start_flag,
    (unsigned char *)&option_index_0,
    lcd_start_disp,
    0,
	&dummy_value,
    Dummy_Func,
    Arrow_Start_Func,
    Dummy_Func,
  },
  { // Digitale uitgang zacht alarm
    1,                                // nr
    0,                                // index
    (unsigned char *)&option_on,      // option
    (unsigned char *)&option_index_0, // option
    lcd_dig_out_zacht_alarm,          // display
    &disp_cursor,                     // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_Select_Func,             // void (*arrow)(void); 
    Enter_IO_Select_Func,             // void (*enter)(void);
  },
  {
    99,
    0,
    &end_flag,
    (unsigned char *)&option_index_0,
    lcd_end_disp,
    0,
	&dummy_value,
    Dummy_Func,
    Arrow_End_Func,
    Dummy_Func,
  },
};

s_screen screen_opt_alarm_2;
s_screen const screen_opt_alarm_2_default =
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
  &opt_alarm_2_key_action[0], // first_action
  &opt_alarm_2_key_action[sizeof(opt_alarm_2_key_action)/sizeof(s_key_action) - 1], // last action 
  0, // change_flag
  0, // punt in normaal display
  0, // punt tijdens invoeren
  0, // aantal digits ingevoerd (punt telt niet mee)
  0, // value
  0, // min value
  0, // max value
  &screen_option_1  // vorige scherm
};

void Control_Screen_Option_Alarm_2(void)
// zorg voor een correcte inhoud in klimaat scherm 0
{
  if (password_enabled & PASSWORD_ENABLED_OPT_MASK)
    start_flag = end_flag = 1;
  else
    start_flag = end_flag = 0;
  Control_Screen(&screen_opt_alarm_2, &screen_opt_alarm_2_default, 1, 1);
}

//-----------------------------------------------------------------------------
static unsigned char DigOutAlarmNotUsed(s_board_IO_on_off IO_new)
{
int i;

  for (i = 0; i < MAX_LUCHTMENGKAST_GROEP; i++)
  {
    if (Board_IO_Used(IO_new, &opt_app.LuchtmengkastGroep[i].DigOutAlarm, 1)) return (0);
  }
  for (i = 0; i < MAX_GROUP; i++)
  {
    if (Board_IO_Used(IO_new, &opt_app.Motorgroup[i].DigOutAlarm,      1)) return (0);
    if (Board_IO_Used(IO_new, &opt_app.Motorgroup[i].DigOutAlarmFlap,  1)) return (0);
    if (Board_IO_Used(IO_new, &opt_app.Ventgroup[i].DigOutAlarmUrgent, 1)) return (0);
  }
  return (1);
}

//-----------------------------------------------------------------------------



