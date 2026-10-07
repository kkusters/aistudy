// C__DISP_DIAG_MOTOR_1.C

#include <string.h>

#include "ch_define.h"

#include "ch_const.h"
#include "ch_disp.h"
#include "ch_disp_diagnose_0.h"
#include "ch_disp_diag_groep_1.h"
#include "ch_disp_func.h"
#include "ch_disp_value.h"
#include "ch_motor.h"
#include "ch_IO.h"
#include "ch_key.h"
#include "ch_lcd_7.h"
#include "ch_lcd_10.h"
#include "ch_lcd_14.h"
#include "ch_lcd_bitmap.h"
#include "ch_lcd_pixel.h"
#include "ch_string.h"
#include "ch_disp_diag_motor_1.h"

static void Prev_Next_Func(void);
static void GetMotorManagement(void);
static void GetMotorRuntime(void);
static void GetMotorMode(void);

static s_disp_agri_header const disp_header = { Disp_Draw_Agri_Header, FN5, EMPTY, &tekst_leeg, HK_GEEN, 0, 2};
static s_disp_tekst           const disp_header_str_0  = { Disp_Draw_Tekst_L,  12, 12, &tekst.Diag_10 };
static s_disp_tekst_array_add const disp_header_str_1  = { Disp_Draw_Tekst_Array_Add_L, &tekst_motor_10, UCHAR, &DiagMotorIndex, MAX_MOTOR };
static void * const lcd_disp_header[] = { &disp_header_str_0, &disp_space_10_L, &disp_header_str_1, &disp_header, 0};

static int Runtime = 0;
static int MotorMode = 0;

//*****************************************************************************
// SCHERM OPBOUW
//*****************************************************************************
//-----------------------------------------------------------------------------
// Draaiuren
static s_disp_bitmap const disp_draaiuren_ico = { Disp_Draw_Bitmap,       7,  8, &ico_klok_switch_on };
static s_disp_tekst  const disp_draaiuren_str = { Disp_Draw_Tekst_L,     37, 20, &tekst.Draaiuren_10 };
#ifdef TEST_H2MC
static s_disp_value  const disp_draaiuren_val = { Disp_Draw_Value, 207, 20, (SIZE_7 | RECHTS), LONG, 0, &Motor[0].Management.Runtime }; // TD - TEST GER
#else
static s_disp_time   const disp_draaiuren_val = { Disp_Draw_Urenteller, 219, 20, (SIZE_14 | RECHTS), &Motor[0].Management.Runtime };
#endif
static s_disp_func   const disp_management_control = { Disp_Control_Func, GetMotorManagement };

static void * const lcd_diag_draaiuren_disp[] = { &disp_draaiuren_ico, &disp_draaiuren_str, &disp_draaiuren_val, 0 };
//-----------------------------------------------------------------------------
// Aantal schakelingen
static s_disp_tekst const disp_schakelingen_str = { Disp_Draw_Tekst_L, 37, 20, &tekst.Schakelingen_10 };
#ifdef TEST_H2MC
static s_disp_value const disp_schakelingen_val = { Disp_Draw_Value,  207, 20, (SIZE_7 | RECHTS), LONG, 0, &Motor[0].Management.Switches }; // TD - TEST GER
#else
static s_disp_value const disp_schakelingen_val = { Disp_Draw_Value,  207, 20, (SIZE_14 | RECHTS), LONG, 0, &Motor[0].Management.Switches };
#endif

static void * const lcd_diag_schakelingen_disp[] = { &disp_schakelingen_ico, &disp_schakelingen_str, &disp_schakelingen_val, 0 };
//-----------------------------------------------------------------------------
// Aantal storingen
static s_disp_bitmap const disp_storingen_ico = { Disp_Draw_Bitmap,   9, 10, &ico_alarm };
static s_disp_tekst  const disp_storingen_str = { Disp_Draw_Tekst_L, 37, 20, &tekst.Storingen_10 };
static s_disp_value  const disp_storingen_val = { Disp_Draw_Value,  207, 20, (SIZE_14 | RECHTS), INT, 0, &Motor[0].Management.Failures };

static void * const lcd_diag_storingen_disp[] = { &disp_management_control, &disp_storingen_ico, &disp_storingen_str, &disp_storingen_val, 0 };
//-----------------------------------------------------------------------------
// IO
static s_disp_func     const disp_mode_control = { Disp_Control_Func, GetMotorMode };
static s_disp_value    const disp_mode_val     = { Disp_Draw_Value, 207, 20, (SIZE_14 | RECHTS), INT, 0, &MotorMode };
static s_disp_board_IO const disp_board_IO     = { Disp_Draw_Board_IO, MOTOR_CONTROL, &opt_app.Motor[0].IO };

static void * const lcd_diag_board_IO_disp[] = { &disp_mode_control, &disp_motor_ico, &disp_board_IO, &disp_mode_val, 0 };
//-----------------------------------------------------------------------------
// Looptijd
static s_disp_tekst const disp_looptijd_str = { Disp_Draw_Tekst_L,  37, 20, &tekst.Looptijd_10 };
static s_disp_value const disp_looptijd_val = { Disp_Draw_Value,   207, 20, (SIZE_14 | RECHTS), INT, 0, &Runtime };
static s_disp_tekst const disp_looptijd_sec = { Disp_Draw_Tekst_L, 210, 20, &tekst.s_10 };
static s_disp_func  const disp_looptijd_control = { Disp_Control_Func, GetMotorRuntime };

static void * const lcd_diag_looptijd_disp[] = { &disp_looptijd_control, &disp_looptijd_ico, &disp_looptijd_str, &disp_looptijd_val, &disp_looptijd_sec, 0 };
//-----------------------------------------------------------------------------
// Alarm Code
static s_disp_tekst  const disp_alarm_code_str = { Disp_Draw_Tekst_L,  37, 20, &tekst.Alarm_Code_10 };
static s_disp_value  const disp_alarm_code_val = { Disp_Draw_Value,   207, 20, (SIZE_14 | RECHTS), UCHAR, 0, &alarm_hr_alg.Motor[0].AlarmCode };
static s_disp_bitmap const disp_alarm_code_ico = { Disp_Draw_Bitmap,    9, 10, &ico_alarm };

static void * const lcd_diag_alarm_code_disp[] = { &disp_alarm_code_ico, &disp_alarm_code_str, &disp_alarm_code_val, 0 };
//-----------------------------------------------------------------------------

s_key_action const diag_motor_1_key_action[] =
{
  { // Draaiuren
    1,                           // nr
    0,                           // index
    (unsigned char *)&option_on, // option
    &DiagMotorIndex,             // Optie index 
    lcd_diag_draaiuren_disp,     // display
    0,                           // cursor
    &dummy_value,                // *value
    Dummy_Func,                  // void (*number)(void); 
    Arrow_Func,                  // void (*arrow)(void); 
    Dummy_Func,                  // void (*enter)(void);
  },
  { // Aantal schakelingen
    2,                           // nr
    0,                           // index
    (unsigned char *)&option_on, // option
    &DiagMotorIndex,             // Optie index 
    lcd_diag_schakelingen_disp,  // display
    0,                           // cursor
    &dummy_value,                // *value
    Dummy_Func,                  // void (*number)(void); 
    Arrow_Func,                  // void (*arrow)(void); 
    Dummy_Func,                  // void (*enter)(void);
  },
  { // Aantal storingen
    3,                           // nr
    0,                           // index
    (unsigned char *)&option_on, // option
    &DiagMotorIndex,             // Optie index 
    lcd_diag_storingen_disp,     // display
    0,                           // cursor
    &dummy_value,                // *value
    Dummy_Func,                  // void (*number)(void); 
    Arrow_Func,                  // void (*arrow)(void); 
    Dummy_Func,                  // void (*enter)(void);
  },
  { // IO
    4,                           // nr
    0,                           // index
    (unsigned char *)&option_on, // option
    &DiagMotorIndex,             // Optie index 
    lcd_diag_board_IO_disp,      // display
    0,                           // cursor
    &dummy_value,                // *value
    Dummy_Func,                  // void (*number)(void); 
    Arrow_Func,                  // void (*arrow)(void); 
    Dummy_Func,                  // void (*enter)(void);
  },
  { // Looptijd
    5,                           // nr
    0,                           // index
    (unsigned char *)&option_on, // option
    &DiagMotorIndex,             // Optie index 
    lcd_diag_looptijd_disp,      // display
    0,                           // cursor
    &dummy_value,                // *value
    Dummy_Func,                  // void (*number)(void); 
    Arrow_Func,                  // void (*arrow)(void); 
    Dummy_Func,                  // void (*enter)(void);
  },
  { // Alarm Code
    6,                           // nr
    0,                           // index
    (unsigned char *)&option_on, // option
    &DiagMotorIndex,             // Optie index 
    lcd_diag_alarm_code_disp,    // display
    0,                           // cursor
    &dummy_value,                // *value
    Dummy_Func,                  // void (*number)(void); 
    Arrow_Func,                  // void (*arrow)(void); 
    Dummy_Func,                  // void (*enter)(void);
  },
};

s_screen screen_diag_motor_1;
s_screen const screen_diag_motor_1_default =
{
  1, // functie nr
  0, // index
  3, // max rel
  0, // rel actief
  0, // nummer weergegeven bin aantal ingeschakelde functie nummers
  0, // aantal ingeschakelde functie nummers
  lcd_disp_header,
  {0,0,0,0,0,0},
  0,3,19,
  0,3,19+28,
  0,3,19+28+28,
  &diag_motor_1_key_action[0], // first_action
  &diag_motor_1_key_action[sizeof(diag_motor_1_key_action)/sizeof(s_key_action) - 1], // last action 
  0, // change_flag
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  &Prev_Next_Func // prev_next_func
};


void Control_Screen_Diag_Motor_1(void)
{
  Control_Screen(&screen_diag_motor_1, &screen_diag_motor_1_default, 1, 3);
}

void If_Exist_Goto_Screen_Diag_Motor_1(void)
{
  Control_Screen_Diag_Motor_1();
  if (screen_diag_motor_1.nr_aantal)
    Next_Screen(&screen_diag_motor_1);
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
        pMotor = Motor[DiagMotorIndex].Prev;
        if (pMotor != NULL)
        {
          screen_tmp = screen_ptr;
          screen_ptr = screen_ptr->prev_screen;
          Arrow_Diag_Motor_Up();
          screen_ptr = screen_tmp;
        }
        break;
      case NEXT:
        pMotor = Motor[DiagMotorIndex].Next;
        if (pMotor != NULL)
        {
          screen_tmp = screen_ptr;
          screen_ptr = screen_ptr->prev_screen;
          Arrow_Diag_Motor_Down();
          screen_ptr = screen_tmp;
        }
        break;
    }
  }
}

//================================================================================
static void GetMotorManagement(void)
{
  Motor[DiagMotorIndex].CtrlMotorManagement |= MOTOR_MANAGEMENT_REQUEST_FLAG;
}

static void GetMotorRuntime(void)
{
  Runtime = (opt_app.Motor[DiagMotorIndex].Runtime + 5) / 10;
}

static void GetMotorMode(void)
{
  MotorMode = IO_Get_Motor_Control_Option(&opt_app.Motor[DiagMotorIndex].IO);
}


