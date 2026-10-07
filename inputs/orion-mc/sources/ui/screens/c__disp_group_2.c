// C__DISP_GROUP_2.C  

#include "ch_define.h"

#include "ch_const.h"
#include "ch_disp.h"
#include "ch_disp_func.h"
#include "ch_disp_group_1.h"
#include "ch_disp_value.h"
#include "ch_key.h"
#include "ch_lcd_10.h"
#include "ch_lcd_14.h"
#include "ch_lcd_bitmap.h"
#include "ch_lcd_pixel.h"
#include "ch_motorgroep.h"
#include "ch_string.h"
#include "ch_tijd.h"
#include "ch_device.h"
#include "ch_disp_group_2.h"

static void Prev_Next_Func(void);
static void Arrow_Min_Max_Value(void);
static void Enter_Min_Max_Value(void);

static s_disp_agri_header const disp_header = { Disp_Draw_Agri_Header, FN1, EMPTY, &tekst_leeg, HK_GEEN, 0, 3};
static s_disp_tekst           const disp_header_str_0  = { Disp_Draw_Tekst_L,  12, 12, &tekst.Syst_10 };
static s_disp_tekst_array_add const disp_header_str_1  = { Disp_Draw_Tekst_Array_Add_L, &tekst_groep_10, UCHAR, &GroupIndex, MAX_GROUP };
static void * const lcd_disp_header[] = { &disp_header_str_0, &disp_space_10_L, &disp_header_str_1, &disp_header, 0};

//*****************************************************************************
// SCHERM OPBOUW
//*****************************************************************************
//-----------------------------------------------------------------------------
// Algemeen
static s_tekst_2    const tekst_x = { 2, 16, SIZE_10, "x" };
static s_disp_tekst const disp_14_perc = { Disp_Draw_Tekst_L, 210, 13, &tekst.perc_7 };
//-----------------------------------------------------------------------------
// Wachttijd alarm handbediening
static s_disp_tekst const disp_alarm_manual_str = { Disp_Draw_Tekst_L,  37, 20, &tekst.Alarm_HAND_10 };
static s_disp_value const disp_alarm_manual_val = { Disp_Draw_Value,   207, 20, (SIZE_14 | RECHTS), INT, 0, &setp_alg.Motorgroup[0].DelayAlarmManual };
static s_disp_tekst const disp_minuut_str       = { Disp_Draw_Tekst_L, 210, 20, &tekst.minuut_7 };

static void * const lcd_delay_alarm_manual_disp[] = { &disp_alarm_delay_ico, &disp_alarm_manual_str, &disp_alarm_manual_val, &disp_minuut_str, 0 };

static s_key_value const key_alarm_manual_val = { INT, 2, &setp_alg.Motorgroup[0].DelayAlarmManual, &int_0, &int_60 };
//-----------------------------------------------------------------------------
// Alarm afwijking positie
static s_disp_tekst const disp_alarm_position_str = { Disp_Draw_Tekst_L,  37, 20, &tekst.Alarm_afwijking_10 };
static s_disp_value const disp_alarm_position_val = { Disp_Draw_Value,   207, 20, (SIZE_14 | RECHTS), UCHAR, 1, &setp_alg.Motorgroup[0].DiffPositionAlarm };

static void * const lcd_alarm_position_disp[] = { &disp_alarm_position_ico, &disp_alarm_position_str, &disp_alarm_position_val, &disp_14_perc, 0 };

static s_key_value const key_alarm_position_val = { UCHAR, 3, &setp_alg.Motorgroup[0].DiffPositionAlarm, &uchar_5, &uchar_250 };
//-----------------------------------------------------------------------------
// Alarm afwijking positie - tijd
static s_disp_tekst const disp_alarm_position_time_str = { Disp_Draw_Tekst_L,  37, 20, &tekst.Alarm_afwijking_10 };
static s_disp_value const disp_alarm_position_time_val = { Disp_Draw_Value,   207, 20, (SIZE_14 | RECHTS), UCHAR, 0, &setp_alg.Motorgroup[0].TimePositionAlarm };

static void * const lcd_alarm_position_time_disp[] = { &disp_alarm_position_ico, &disp_alarm_position_time_str, &disp_alarm_position_time_val, &disp_minuut_str, 0 };

static s_key_value const key_alarm_position_time_val = { UCHAR, 3, &setp_alg.Motorgroup[0].TimePositionAlarm, &uchar_1, &uchar_120 };
//-----------------------------------------------------------------------------
// Alarm urgent
static s_disp_tekst const disp_alarm_urgent_str = { Disp_Draw_Tekst_L,  37, 20, &tekst.Alarm_urgent_10 };
static s_disp_value const disp_alarm_urgent_val = { Disp_Draw_Value,   207, 20, (SIZE_14 | RECHTS), UCHAR, 0, &setp_alg.Ventgroup[0].AlarmUrgent };
static s_disp_tekst const disp_alarm_urgent_x   = { Disp_Draw_Tekst_L, 210, 20, &tekst_x };

static void * const lcd_alarm_urgent_disp[] = { &disp_alarm_urgent_ico, &disp_alarm_urgent_str, &disp_alarm_urgent_val, &disp_alarm_urgent_x, 0 };

static s_key_value const key_alarm_urgent_val = { UCHAR, 3, &setp_alg.Ventgroup[0].AlarmUrgent, &uchar_1, &uchar_255 };
//-----------------------------------------------------------------------------
// Pulse system - Pulse zone
static s_disp_tekst const disp_pulse_zone_str = { Disp_Draw_Tekst_L,  37, 20, &tekst.Pulse_zone_10 };
static s_disp_value const disp_pulse_zone_val = { Disp_Draw_Value,   207, 20, (SIZE_14 | RECHTS), UCHAR, 1, &setp_alg.Motorgroup[0].PulseSystem.PulseZone };

static void * const lcd_pulse_zone_disp[] = { &disp_pulse_system_ico, &disp_pulse_zone_str, &disp_pulse_zone_val, &disp_14_perc, 0 };

static s_key_value const key_pulse_zone_val = { UCHAR, 3, &setp_alg.Motorgroup[0].PulseSystem.PulseZone, &uchar_0, &uchar_200 };
//-----------------------------------------------------------------------------
// Pulse system - Pulse width
static s_disp_tekst const disp_pulse_width_str = { Disp_Draw_Tekst_L,  37, 20, &tekst.Pulse_width_10 };
static s_disp_value const disp_pulse_width_val = { Disp_Draw_Value,   207, 20, (SIZE_14 | RECHTS), UCHAR, 1, &setp_alg.Motorgroup[0].PulseSystem.PulseWidth };

static void * const lcd_pulse_width_disp[] = { &disp_pulsetime_ico, &disp_pulse_width_str, &disp_pulse_width_val, &disp_14_perc, 0 };

static s_key_value const key_pulse_width_val = { UCHAR, 3, &setp_alg.Motorgroup[0].PulseSystem.PulseWidth, &uchar_0, &uchar_200 };
//-----------------------------------------------------------------------------
// Pulse system - Cycletime
static s_disp_tekst const disp_pulse_cycletime_str = { Disp_Draw_Tekst_L,  37, 20, &tekst.Cycletime_10 };
static s_disp_value const disp_pulse_cycletime_val = { Disp_Draw_Value,   207, 20, (SIZE_14 | RECHTS), UCHAR, 0, &setp_alg.Motorgroup[0].PulseSystem.CycleTime };

static void * const lcd_pulse_cycletime_disp[] = { &disp_cycletime_ico, &disp_pulse_cycletime_str, &disp_pulse_cycletime_val, &disp_minuut_str, 0 };

static s_key_value const key_pulse_cycletime_val = { UCHAR, 3, &setp_alg.Motorgroup[0].PulseSystem.CycleTime, &uchar_0, &uchar_240 };
//-----------------------------------------------------------------------------
// Minimum ventilatie
static s_disp_tekst const disp_minimum_str = { Disp_Draw_Tekst_L, 37, 20, &tekst.Minimum_10 };
static s_disp_value const disp_minimum_val = { Disp_Draw_Value,  207, 20, (SIZE_14 | RECHTS), UCHAR, 0, &setp_alg.Ventgroup[0].MinVent };

static void * const lcd_minimum_disp[] = { &disp_vent_min_val, &disp_minimum_str, &disp_minimum_val, &disp_14_perc, 0 };

static s_key_value const key_minimum_val = { UCHAR, 3, &setp_alg.Ventgroup[0].MinVent, &uchar_0, &uchar_100 };
//-----------------------------------------------------------------------------
// Maximum ventilatie
static s_disp_tekst const disp_maximum_str = { Disp_Draw_Tekst_L, 37, 20, &tekst.Maximum_10 };
static s_disp_value const disp_maximum_val = { Disp_Draw_Value,  207, 20, (SIZE_14 | RECHTS), UCHAR, 0, &setp_alg.Ventgroup[0].MaxVent };

static void * const lcd_maximum_disp[] = { &disp_vent_max_val, &disp_maximum_str, &disp_maximum_val, &disp_14_perc, 0 };

static s_key_value const key_maximum_val = { UCHAR, 3, &setp_alg.Ventgroup[0].MaxVent, &uchar_0, &uchar_100 };
//-----------------------------------------------------------------------------

s_key_action const group_2_key_action[] =
{
  { // Wachttijd alarm handbediening
    1,                                // nr
    0,                                // index
    (unsigned char *)&option_on,      // option
    &GroupIndex,                      // option
    lcd_delay_alarm_manual_disp,      // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Change_Syst_Func,           // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    1,                                // nr
    1,                                // index
    (unsigned char *)&option_on,      // option
    &GroupIndex,                      // option
    lcd_delay_alarm_manual_disp,      // display
    &disp_cursor_207_23_8,            // cursor
    &key_alarm_manual_val,            // *value
    Number_Value,                     // void (*number)(void); 
    Arrow_Value,                      // void (*arrow)(void); 
    Increment_Func_Index_Enter_Value, // void (*enter)(void);
  },
  { // Alarm afwijking positie
    2,                                // nr
    0,                                // index
    &Motorgroup[0].AlarmAfw,          // option
    &GroupIndex,                      // option
    lcd_alarm_position_disp,          // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Change_Syst_Func,           // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    2,                                // nr
    1,                                // index
    &Motorgroup[0].AlarmAfw,          // option
    &GroupIndex,                      // option
    lcd_alarm_position_disp,          // display
    &disp_cursor_207_23_8,            // cursor
    &key_alarm_position_val,          // *value
    Number_Value,                     // void (*number)(void); 
    Arrow_Value,                      // void (*arrow)(void); 
    Increment_Func_Index_Enter_Value, // void (*enter)(void);
  },
  { // Alarm afwijking positie - tijd
    3,                                // nr
    0,                                // index
    &Motorgroup[0].AlarmAfw,          // option
    &GroupIndex,                      // option
    lcd_alarm_position_time_disp,     // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Change_Syst_Func,           // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    3,                                // nr
    1,                                // index
    &Motorgroup[0].AlarmAfw,          // option
    &GroupIndex,                      // option
    lcd_alarm_position_time_disp,     // display
    &disp_cursor_207_23_8,            // cursor
    &key_alarm_position_time_val,     // *value
    Number_Value,                     // void (*number)(void); 
    Arrow_Value,                      // void (*arrow)(void); 
    Increment_Func_Index_Enter_Value, // void (*enter)(void);
  },
  { // Alarm urgent
    4,                                // nr
    0,                                // index
    &Motorgroup[0].TypeVent,          // option
    &GroupIndex,                      // option
    lcd_alarm_urgent_disp,            // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Change_Syst_Func,           // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    4,                                // nr
    1,                                // index
    &Motorgroup[0].TypeVent,          // option
    &GroupIndex,                      // option
    lcd_alarm_urgent_disp,            // display
    &disp_cursor_207_23_8,            // cursor
    &key_alarm_urgent_val,            // *value
    Number_Value,                     // void (*number)(void); 
    Arrow_Value,                      // void (*arrow)(void); 
    Increment_Func_Index_Enter_Value, // void (*enter)(void);
  },
  { // Pulse system - pulse zone
    5,                                  // nr
    0,                                  // index
    &opt_app.Motorgroup[0].PulseSystem, // option
    &GroupIndex,                        // option
    lcd_pulse_zone_disp,                // display
    0,                                  // cursor
    &dummy_value,                       // *value
    Dummy_Func,                         // void (*number)(void); 
    Arrow_Change_Syst_Func,           // void (*arrow)(void); 
    Dummy_Func,                         // void (*enter)(void);
  },
  {
    5,                                  // nr
    1,                                  // index
    &opt_app.Motorgroup[0].PulseSystem, // option
    &GroupIndex,                        // option
    lcd_pulse_zone_disp,                // display
    &disp_cursor_207_23_8,              // cursor
    &key_pulse_zone_val,                // *value
    Number_Value,                       // void (*number)(void); 
    Arrow_Value,                        // void (*arrow)(void); 
    Increment_Func_Index_Enter_Value,   // void (*enter)(void);
  },
  { // Pulse system - pulse width
    6,                                  // nr
    0,                                  // index
    &opt_app.Motorgroup[0].PulseSystem, // option
    &GroupIndex,                        // option
    lcd_pulse_width_disp,               // display
    0,                                  // cursor
    &dummy_value,                       // *value
    Dummy_Func,                         // void (*number)(void); 
    Arrow_Change_Syst_Func,           // void (*arrow)(void); 
    Dummy_Func,                         // void (*enter)(void);
  },
  {
    6,                                  // nr
    1,                                  // index
    &opt_app.Motorgroup[0].PulseSystem, // option
    &GroupIndex,                        // option
    lcd_pulse_width_disp,               // display
    &disp_cursor_207_23_8,              // cursor
    &key_pulse_width_val,               // *value
    Number_Value,                       // void (*number)(void); 
    Arrow_Value,                        // void (*arrow)(void); 
    Increment_Func_Index_Enter_Value,   // void (*enter)(void);
  },
  { // Pulse system - cycletime
    7,                                  // nr
    0,                                  // index
    &opt_app.Motorgroup[0].PulseSystem, // option
    &GroupIndex,                        // option
    lcd_pulse_cycletime_disp,           // display
    0,                                  // cursor
    &dummy_value,                       // *value
    Dummy_Func,                         // void (*number)(void); 
    Arrow_Change_Syst_Func,           // void (*arrow)(void); 
    Dummy_Func,                         // void (*enter)(void);
  },
  {
    7,                                  // nr
    1,                                  // index
    &opt_app.Motorgroup[0].PulseSystem, // option
    &GroupIndex,                        // option
    lcd_pulse_cycletime_disp,           // display
    &disp_cursor_207_23_8,              // cursor
    &key_pulse_cycletime_val,           // *value
    Number_Value,                       // void (*number)(void); 
    Arrow_Value,                        // void (*arrow)(void); 
    Increment_Func_Index_Enter_Value,   // void (*enter)(void);
  },
  { // Minimum
    8,                                // nr
    0,                                // index
    &Motorgroup[0].TypeVent,          // option
    &GroupIndex,                      // option
    lcd_minimum_disp,                 // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Change_Syst_Func,           // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    8,                                // nr
    1,                                // index
    &Motorgroup[0].TypeVent,          // option
    &GroupIndex,                      // option
    lcd_minimum_disp,                 // display
    &disp_cursor_207_23_8,            // cursor
    &key_minimum_val,                 // *value
    Number_Value,                     // void (*number)(void); 
    Arrow_Min_Max_Value,              // void (*arrow)(void); 
    Enter_Min_Max_Value,              // void (*enter)(void);
  },
  { // Maximum
    9,                                // nr
    0,                                // index
    &Motorgroup[0].TypeVent,          // option
    &GroupIndex,                      // option
    lcd_maximum_disp,                 // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Change_Syst_Func,           // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    9,                                // nr
    1,                                // index
    &Motorgroup[0].TypeVent,          // option
    &GroupIndex,                      // option
    lcd_maximum_disp,                 // display
    &disp_cursor_207_23_8,            // cursor
    &key_maximum_val,                 // *value
    Number_Value,                     // void (*number)(void); 
    Arrow_Min_Max_Value,              // void (*arrow)(void); 
    Enter_Min_Max_Value,              // void (*enter)(void);
  },
};

s_screen screen_group_2;
s_screen const screen_group_2_default =
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
  &group_2_key_action[0], // first_action
  &group_2_key_action[sizeof(group_2_key_action)/sizeof(s_key_action) - 1], // last action 
  0, // change_flag
  0, // punt in normaal display
  0, // punt tijdens invoeren
  0, // aantal digits ingevoerd (punt telt niet mee)
  0, // value
  0, // min value
  0, // max value
  &screen_group_1, // vorige scherm
  &Prev_Next_Func  // prev_next_func
};

void Control_Screen_Group_2(void)
{
  Control_Screen(&screen_group_2, &screen_group_2_default, 1, 3);
}

void If_Exist_Goto_Screen_Group_2(void)
{
  Control_Screen_Group_2();
  if (screen_group_2.nr_aantal)
    Next_Screen(&screen_group_2);
}

//================================================================================
static void Prev_Next_Func(void)
{
s_screen *screen_tmp;

  if (screen_ptr->index == 0)
  {
    switch (key_func)
    {
      case PREV:
        if (GroupIndex > 0)
        {
          GroupIndex--;
          SetMotorScreen();
          Control_Screen_Group_1();
          if (opt_app.Motorgroup[GroupIndex].NumberMotors > 3)
            screen_group_1.nr_aantal += opt_app.Motorgroup[GroupIndex].NumberMotors - 3;
          screen_tmp = screen_ptr;
          screen_ptr = screen_ptr->prev_screen->prev_screen;
          Decrement_Func();
          screen_ptr = screen_tmp;
          Control_Screen_Group_2();
        }
        break;
      case NEXT:
        if ((GroupIndex + 1) < opt_app.NumberMotorgroups)
        {
          GroupIndex++;
          SetMotorScreen();
          Control_Screen_Group_1();
          if (opt_app.Motorgroup[GroupIndex].NumberMotors > 3)
            screen_group_1.nr_aantal += opt_app.Motorgroup[GroupIndex].NumberMotors - 3;
          screen_tmp = screen_ptr;
          screen_ptr = screen_ptr->prev_screen->prev_screen;
          Increment_Func();
          screen_ptr = screen_tmp;
          Control_Screen_Group_2();
        }
        break;
    }
  }
}

//================================================================================
static void AdjustMinMax(void)
{
int Min, Max;

  Min = setp_alg.Ventgroup[GroupIndex].MinVent * 10;
  Max = setp_alg.Ventgroup[GroupIndex].MaxVent * 10;
  if (val_hr_alg.Motorgroup[GroupIndex].OperationMode == omManual)
  {
    if (val_hr_alg.Motorgroup[GroupIndex].PositionPerc < Min)
      val_hr_alg.Motorgroup[GroupIndex].PositionPerc = Min;
    if (val_hr_alg.Motorgroup[GroupIndex].PositionPerc > Max)
      val_hr_alg.Motorgroup[GroupIndex].PositionPerc = Max;
  }
}

static void Arrow_Min_Max_Value(void)
{
  Arrow_Value();
  if (screen_ptr->index == 0)
    AdjustMinMax();
}

static void Enter_Min_Max_Value(void)
{
  Increment_Func_Index_Enter_Value();
  AdjustMinMax();
}





