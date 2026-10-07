// C__DISP_VENT_1.C  

#include <string.h>

#include "ch_define.h"

#include "ch_alg.h"
#include "ch_const.h"
#include "ch_disp.h"
#include "ch_disp_func.h"
#include "ch_disp_group_1.h"
#include "ch_disp_password.h"
#include "ch_disp_value.h"
#include "ch_key.h"
#include "ch_lcd_10.h"
#include "ch_lcd_14.h"
#include "ch_lcd_bitmap.h"
#include "ch_lcd_pixel.h"
#include "ch_string.h"
#include "ch_tijd.h"
#include "ch_device.h"
#include "ch_disp_vent_1.h"

static void Prev_Next_Func(void);

static void Arrow_Operation_Mode_Value(void);
static void Enter_Operation_Mode_Value(void);

static unsigned char VentManual;

//*****************************************************************************
// SCHERM OPBOUW
//*****************************************************************************
//-----------------------------------------------------------------------------
// basis opbouw lcd
static s_disp_agri_header const disp_header       = { Disp_Draw_Agri_Header, FN1, EMPTY, &tekst_leeg, HK_GEEN, 0, 3};
static s_disp_tekst       const disp_header_str_0 = { Disp_Draw_Tekst_L,  12, 12, &tekst.Fn_10 };
static s_disp_tekst_add   const disp_header_str_1 = { Disp_Draw_Tekst_Add_L, &tekst.Ventilator_10 };
static s_disp_value_add   const disp_header_val_1 = { Disp_Draw_Value_Add, (SIZE_10 | LINKS), INT, 0, &DeviceNr };

static void * const lcd_disp_header[] = { &disp_header_str_0, &disp_space_10_L, &disp_header_str_1, &disp_space_10_L, &disp_header_val_1, &disp_header, 0};
//-----------------------------------------------------------------------------
// Algemeen
static s_disp_tekst const disp_14_perc = { Disp_Draw_Tekst_L, 210, 13, &tekst.perc_7 };
//-----------------------------------------------------------------------------
// Bediening
static s_disp_tekst       const disp_operation_str = { Disp_Draw_Tekst_L, 37, 20, &tekst.Bediening_10 };
static s_disp_tekst_array const disp_operation_val = { Disp_Draw_Tekst_Array_R, 217, 20, &tekst_auto_hand_uit_10, UCHAR, &val_hr_alg.Device[0].OperationMode, 3 };

static void * const lcd_operation_disp[] = { &disp_motor_ico, &disp_operation_str, &disp_operation_val, 0 };

static s_key_value const key_operation_val = { UCHAR, 1, &val_hr_alg.Device[0].OperationMode, &uchar_0, &uchar_2 };
//-----------------------------------------------------------------------------
// Positie
static s_disp_tekst const disp_position_str = { Disp_Draw_Tekst_L, 37, 20, &tekst.Positie_10 };
static s_disp_value const disp_position_val = { Disp_Draw_Value,  207, 20, (SIZE_14 | RECHTS), UCHAR, 0, &val_hr_alg.Device[0].TargetValue };

static void * const lcd_position_disp[] = { &disp_motor_ico, &disp_position_str, &disp_position_val, &disp_14_perc, 0 };

static s_key_value const key_position_val = { UCHAR, 3, &val_hr_alg.Device[0].TargetValue, &uchar_0, &uchar_100 };
//-----------------------------------------------------------------------------
// Offset
static s_disp_tekst const disp_offset_str = { Disp_Draw_Tekst_L, 37, 20, &tekst.Offset_10 };
static s_disp_value const disp_offset_val = { Disp_Draw_Value,  207, 20, (SIZE_14 | RECHTS), CHAR, 0, &setp_alg.Device[0].Offset };

static void * const lcd_offset_disp[] = { &disp_offset_ico, &disp_offset_str, &disp_offset_val, &disp_14_perc, 0 };

static s_key_value const key_offset_val = { CHAR, 3, &setp_alg.Device[0].Offset, &char_m50, &char_50 };
//-----------------------------------------------------------------------------


s_key_action const vent_1_key_action[] =
{
  { // Manual/Auto/Off
    1,                                // nr
    0,                                // index
    (unsigned char *)&option_on,      // option
    &MotorIndex,                      // option
    lcd_operation_disp,               // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Change_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    1,                                // nr
    1,                                // index
    (unsigned char *)&option_on,      // option
    &MotorIndex,                      // option
    lcd_operation_disp,               // display
    &disp_cursor_operation,           // cursor
    &key_operation_val,               // *value
    Number_Value,                     // void (*number)(void); 
    Arrow_Operation_Mode_Value,       // void (*arrow)(void); 
    Enter_Operation_Mode_Value,       // void (*enter)(void);
  },
  { // Position
    2,                                // nr
    0,                                // index
    (unsigned char *)&option_on,      // option
    &MotorIndex,                      // option
    lcd_position_disp,                // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Change_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    2,                                // nr
    1,                                // index
    &VentManual,                      // option
    &MotorIndex,                      // option
    lcd_position_disp,                // display
    &disp_cursor_207_23_8,            // cursor
    &key_position_val,                // *value
    Number_Value,                     // void (*number)(void); 
    Arrow_Value,                      // void (*arrow)(void); 
    Increment_Func_Index_Enter_Value, // void (*enter)(void);
  },
  { // Offset
    5,                                // nr
    0,                                // index
    (unsigned char *)&option_on,      // option
    &MotorIndex,                      // option
    lcd_offset_disp,                  // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Change_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    5,                                // nr
    1,                                // index
    (unsigned char *)&option_on,      // option
    &MotorIndex,                      // option
    lcd_offset_disp,                  // display
    &disp_cursor_207_23_8,            // cursor
    &key_offset_val,                  // *value
    Number_Value,                     // void (*number)(void); 
    Arrow_Value,                      // void (*arrow)(void); 
    Increment_Func_Index_Enter_Value, // void (*enter)(void);
  },
};

s_screen screen_vent_1;
s_screen const screen_vent_1_default =
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
  &vent_1_key_action[0], // first_action
  &vent_1_key_action[sizeof(vent_1_key_action)/sizeof(s_key_action) - 1], // last action 
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

void Control_Screen_Vent_1(void)
{
  if (val_hr_alg.Device[MotorIndex].OperationMode == omManual)
    VentManual = 1;
  else
    VentManual = 0;

  Control_Screen(&screen_vent_1, &screen_vent_1_default, 1, 3);
}

void If_Exist_Goto_Screen_Vent_1(void)
{
  Control_Screen_Vent_1();
  if (screen_vent_1.nr_aantal)
    Next_Screen(&screen_vent_1);
}

//================================================================================
static void Prev_Next_Func(void)
{
TDevice *pDevice;
s_screen *screen_tmp;

  if (screen_ptr->index == 0)
  {
    switch (key_func)
    {
      case PREV:
        pDevice = Device[MotorIndex].Prev;
        if (pDevice != NULL)
        {
          screen_tmp = screen_ptr;
          screen_ptr = screen_ptr->prev_screen;
          Arrow_Motor_Up();
          screen_ptr = screen_tmp;
          Control_Screen_Vent_1();
        }
        break;
      case NEXT:
        pDevice = Device[MotorIndex].Next;
        if (pDevice != NULL)
        {
          screen_tmp = screen_ptr;
          screen_ptr = screen_ptr->prev_screen;
          Arrow_Motor_Down();
          screen_ptr = screen_tmp;
          Control_Screen_Vent_1();
        }
        break;
    }
  }
}

//================================================================================
static void Arrow_Operation_Mode_Value(void)
{
  switch (key)
  {
    case LEFT:
      Enter_Operation_Mode_Value();
      break;
    case RIGHT:
      Enter_Operation_Mode_Value();
      break;
    case UP:
      Increment_Scroll_Value_No_Enter();
      break;
    case DOWN:
      Decrement_Scroll_Value_No_Enter();
      break;
  }
}

static void Enter_Operation_Mode_Value(void)
{
  if (screen_ptr->change_flag)
    Enter_Value();
  if (val_hr_alg.Device[MotorIndex].OperationMode == omManual)
    VentManual = 1;
  else
    VentManual = 0;
  Increment_Func_Index();
  Refresh_Screen_Nr_Aantal();
}

//================================================================================
                                





