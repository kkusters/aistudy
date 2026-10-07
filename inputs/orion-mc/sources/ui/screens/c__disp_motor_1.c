// C__DISP_MOTOR_1.C  

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
#include "ch_motor.h"
#include "ch_string.h"
#include "ch_tijd.h"
#include "ch_disp_motor_1.h"

static void Prev_Next_Func(void);

static void Arrow_Operation_Mode_Value(void);
static void Enter_Operation_Mode_Value(void);
static void Arrow_Position_Func(void);
static void Arrow_Position_Value(void);
static void Enter_Position_Value(void);
static void Number_Position_Value(void);

static s_disp_agri_header const disp_header = { Disp_Draw_Agri_Header, FN1, EMPTY, &tekst_leeg, HK_GEEN, 0, 3};
static s_disp_tekst           const disp_header_str_0  = { Disp_Draw_Tekst_L,  12, 12, &tekst.Fn_10 };
static s_disp_tekst_array_add const disp_header_str_1  = { Disp_Draw_Tekst_Array_Add_L, &tekst_motor_10, UCHAR, &MotorIndex, MAX_MOTOR };
static void * const lcd_disp_header[] = { &disp_header_str_0, &disp_space_10_L, &disp_header_str_1, &disp_header, 0};

static int PositionManual;
static unsigned char Numeric;

unsigned char MotorManual;

//*****************************************************************************
// SCHERM OPBOUW
//*****************************************************************************
//-----------------------------------------------------------------------------
/*
// basis opbouw lcd
static s_disp_tekst           const disp_header_str_0  = { Disp_Draw_Tekst_L,  12, 12, &tekst.Fn_10 };
static s_disp_tekst_array_add const disp_header_str_1  = { Disp_Draw_Tekst_Array_Add_L, &tekst_motor_10, UCHAR, &MotorIndex, MAX_MOTOR };
static s_disp_tekst           const disp_header_str_2  = { Disp_Draw_Tekst_L, 204, 12, &header_string_2 };
static s_disp_value           const disp_header_val_2  = { Disp_Draw_Value,   237, 12, (SIZE_7 | RECHTS), INT, 0, &screen_motor_1.nr };
static s_disp_block           const disp_header_invert = { Disp_Invert_Block,       0,  0, 239, 16 };
static s_disp_block           const disp_header_line_0 = { Disp_Draw_Black_Block,   0, 13,   0,104 }; // vertikaal
static s_disp_block           const disp_header_line_1 = { Disp_Draw_Black_Block, 239, 13, 239,104 }; // vertikaal
static s_disp_block           const disp_header_line_2 = { Disp_Draw_Black_Block,  44,104, 238,104 }; 

static void * const lcd_disp_header[] = { &disp_10_slot, &disp_header_str_0, &disp_space_10_L, &disp_header_str_1,
                                          &disp_header_str_2, &disp_header_val_2,
                                          &disp_bus_ok_ico,
                                          &disp_header_invert,
                                          &disp_header_line_0, &disp_header_line_1, &disp_header_line_2,
                                          &disp_loper,
                                          &disp_tab_begin,  &disp_fn_F1, 
                                          &disp_tab_2_norm, &disp_fn_F2,
                                          &disp_tab_3_norm, &disp_fn_F3,
                                          &disp_tab_4_norm, &disp_fn_alarm,
                                          &disp_tab_5_norm, &disp_fn_diagnose,
                                          &disp_tab_6_norm, &disp_fn_opties,
                                          &disp_tab_end, 0 };
*/
//-----------------------------------------------------------------------------
// Algemeen
static s_disp_tekst const disp_14_perc = { Disp_Draw_Tekst_L, 210, 13, &tekst.perc_7 };
//-----------------------------------------------------------------------------
// Status
static s_disp_tekst const disp_status_str = { Disp_Draw_Tekst_L, 37, 20, &tekst.Status_10 };
static s_disp_tekst_array_option_on const disp_status_val           = { Disp_Draw_Tekst_Array_R_Option_Off, 217, 20, &tekst_stop_open_dicht_10,     UCHAR, &Motor[0].RunningMode, 3, UCHAR, &val_hr_alg.Motor[0].Overruled };
static s_disp_tekst_array_option_on const disp_status_overruled_val = { Disp_Draw_Tekst_Array_R_Option_On,  217, 20, &tekst_ext_stop_open_dicht_10, UCHAR, &Motor[0].RunningMode, 3, UCHAR, &val_hr_alg.Motor[0].Overruled };

static void * const lcd_status_disp[] = { &disp_motor_ico, &disp_status_str, &disp_status_val, &disp_status_overruled_val, 0 };
//-----------------------------------------------------------------------------
// Bediening
static s_disp_tekst const disp_operation_str = { Disp_Draw_Tekst_L, 37, 20, &tekst.Bediening_10 };
static s_disp_tekst_array const disp_operation_val = { Disp_Draw_Tekst_Array_R, 217, 20, &tekst_auto_hand_uit_10, UCHAR, &val_hr_alg.Motor[0].OperationMode, 3 };

static void * const lcd_operation_disp[] = { &disp_motor_ico, &disp_operation_str, &disp_operation_val, 0 };

static s_key_value const key_operation_val = { UCHAR, 1, &val_hr_alg.Motor[0].OperationMode, &uchar_0, &uchar_2 };
//-----------------------------------------------------------------------------
// Positie
static s_disp_tekst const disp_position_str = { Disp_Draw_Tekst_L, 37, 20, &tekst.Positie_10 };
static s_disp_value const disp_position_val = { Disp_Draw_Value,  207, 20, (SIZE_14 | RECHTS), INT, 1, &val_hr_alg.Motor[0].PositionManual };

static void * const lcd_position_disp[] = { &disp_motor_ico, &disp_position_str, &disp_position_val, &disp_14_perc, 0 };

static s_key_value const key_position_val = { INT, 4, &val_hr_alg.Motor[0].PositionManual, &int_0, &int_1000 };
//-----------------------------------------------------------------------------


s_key_action const motor_1_key_action[] =
{
  { // Status
    1,                                // nr
    0,                                // index
    (unsigned char *)&option_on,      // option
    &MotorIndex,                      // option
    lcd_status_disp,                  // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Change_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  { // Manual/Auto/Off
    2,                                // nr
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
    2,                                // nr
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
    3,                                // nr
    0,                                // index
    &MotorManual,                     // option
    &MotorIndex,                      // option
    lcd_position_disp,                // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Position_Func,              // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    3,                                // nr
    1,                                // index
    &MotorManual,                     // option
    &MotorIndex,                      // option
    lcd_position_disp,                // display
    &disp_cursor_207_23_8,            // cursor
    &key_position_val,                // *value
    Number_Position_Value,            // void (*number)(void); 
    Arrow_Position_Value,             // void (*arrow)(void); 
    Enter_Position_Value,             // void (*enter)(void);
  },
};

s_screen screen_motor_1;
s_screen const screen_motor_1_default =
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
  &motor_1_key_action[0], // first_action
  &motor_1_key_action[sizeof(motor_1_key_action)/sizeof(s_key_action) - 1], // last action 
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

void Control_Screen_Motor_1(void)
{
  Control_Screen(&screen_motor_1, &screen_motor_1_default, 1, 3);
}

void If_Exist_Goto_Screen_Motor_1(void)
{
  if (val_hr_alg.Motor[MotorIndex].OperationMode == omManual)
    MotorManual = 1;
  else
    MotorManual = 0;
  Control_Screen_Motor_1();
  if (screen_motor_1.nr_aantal)
    Next_Screen(&screen_motor_1);
}

//================================================================================
static void Prev_Next_Func(void)
{
TMotor *pMotor;
s_screen *screen_tmp;

  if (screen_ptr->index == 0)
  {
    switch (key_func)
    {
      case PREV:
        pMotor = Motor[MotorIndex].Prev;
        if (pMotor != NULL)
        {
          screen_tmp = screen_ptr;
          screen_ptr = screen_ptr->prev_screen;
          Arrow_Motor_Up();
          screen_ptr = screen_tmp;
          if (val_hr_alg.Motor[MotorIndex].OperationMode == omManual)
            MotorManual = 1;
          else
            MotorManual = 0;
          Control_Screen_Motor_1();
        }
        break;
      case NEXT:
        pMotor = Motor[MotorIndex].Next;
        if (pMotor != NULL)
        {
          screen_tmp = screen_ptr;
          screen_ptr = screen_ptr->prev_screen;
          Arrow_Motor_Down();
          screen_ptr = screen_tmp;
          if (val_hr_alg.Motor[MotorIndex].OperationMode == omManual)
            MotorManual = 1;
          else
            MotorManual = 0;
          Control_Screen_Motor_1();
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
  if (val_hr_alg.Motor[MotorIndex].OperationMode == omManual)
    MotorManual = 1;
  else
    MotorManual = 0;
  Increment_Func_Index();
  Refresh_Screen_Nr_Aantal();
}

//================================================================================
static void Arrow_Position_Func(void)
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
      Prev_Screen();
      break;
    case RIGHT:
      if (val_hr_alg.Motor[MotorIndex].OperationMode == omManual)
      {
        if ((password_enabled & PASSWORD_ENABLED_SETP_MASK) == 0)
          Right_Password();
        else
        {
          Increment_Func_Index();
          PositionManual = screen_ptr->value;
        }
      }
      break;
  }
}

static void Number_Position_Value(void)
{
  if (!Numeric)
  {
    screen_ptr->change_flag = 0;
    Numeric = 1;
  }
  Number_Value();
}

static void Arrow_Position_Value(void)
{
  switch (key)
  {
    case LEFT:  if (!Numeric)
                  screen_ptr->change_flag = 0;
                Arrow_Left_Value();
                if (screen_ptr->index == 0)
                  val_hr_alg.Motor[MotorIndex].PositionManual = PositionManual;
                break;
    case RIGHT: Increment_Func_Index();
                if (screen_ptr->index == 0)
                  val_hr_alg.Motor[MotorIndex].PositionManual = PositionManual;
                break;
    case UP:    Increment_Value_No_Enter();
                Numeric = 0;
                PositionManual = screen_ptr->value;
                break;
    case DOWN:  Decrement_Value_No_Enter();
                Numeric = 0;
                PositionManual = screen_ptr->value;
                break;
  }
}

static void Enter_Position_Value(void)
{
  if (screen_ptr->change_flag)
  {
    Correct_Decimal_Value();
    if ((screen_ptr->value >= screen_ptr->min_value) && (screen_ptr->value <= screen_ptr->max_value))
      PositionManual = screen_ptr->value;
  }
  Increment_Func_Index();
  if (screen_ptr->index == 0)
    val_hr_alg.Motor[MotorIndex].PositionManual = PositionManual;
}
                                





