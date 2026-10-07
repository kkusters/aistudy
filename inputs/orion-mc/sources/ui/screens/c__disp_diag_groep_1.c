// C__DISP_DIAG_GROEP_1.C

#include <string.h>

#include "ch_define.h"

#include "ch_const.h"
#include "ch_disp.h"
#include "ch_disp_diagnose_0.h"
#include "ch_disp_diag_motor_1.h"
#include "ch_disp_diag_vent_1.h"
#include "ch_disp_func.h"
#include "ch_disp_value.h"
#include "ch_key.h"
#include "ch_lcd_7.h"
#include "ch_lcd_10.h"
#include "ch_lcd_14.h"
#include "ch_lcd_bitmap.h"
#include "ch_lcd_pixel.h"
#include "ch_motor.h"
#include "ch_motorgroep.h"
#include "ch_string.h"
#include "ch_device.h"
#include "ch_disp_diag_groep_1.h"

static void Prev_Next_Func(void);
static void SetDiagMotorScreen(void);
static void Arrow_Diag_Motor_Func(void);
static void Reset_All_Func(void);
static void GetMotorManagement(void);
static void GetRuntimeMotorgroup(void);

static unsigned char DispDummy;
static unsigned char DispRuntime;

unsigned char DiagGroupIndex;
unsigned char DiagMotorIndex;
static unsigned char DispDiagMotorIndex[3];
static unsigned char DispDiagMotorOption[3];
static unsigned char DispDiagDeviceOption[3];
static unsigned char DiagDeviceType;
static int DispDiagDeviceNr[3];
int DiagDeviceNr;

static int Runtime = 0;

//*****************************************************************************
// SCHERM OPBOUW
//*****************************************************************************
//-----------------------------------------------------------------------------
// Header
//-----------------------------------------------------------------------------
static s_disp_agri_header const disp_header = { Disp_Draw_Agri_Header, FN5, EMPTY, &tekst_leeg, HK_GEEN, 0, 2};
static s_disp_tekst const disp_header_str_0  = { Disp_Draw_Tekst_L,  12, 12, &tekst.Diag_10 };
static s_disp_tekst_array_add const disp_header_str_1  = { Disp_Draw_Tekst_Array_Add_L, &tekst_groep_10, UCHAR, &DiagGroupIndex, MAX_GROUP };
static void * const lcd_disp_header[] = { &disp_header_str_0, &disp_space_10_L, &disp_header_str_1, &disp_header, 0};

//-----------------------------------------------------------------------------
// basis opbouw lcd
//-----------------------------------------------------------------------------
// Algemeen
static s_disp_tekst const disp_14_perc = { Disp_Draw_Tekst_L, 210, 13, &tekst.perc_7 };
static s_bitmap const * const ico_looptijd_raam_doek[] = { &looptijd_raam_ico, &looptijd_doek_ico };
static s_disp_bitmap_array const disp_looptijd_raam_doek_ico = { Disp_Draw_Bitmap_Array, 10, 6, ico_looptijd_raam_doek, UCHAR, &opt_app.Motorgroup[0].Type, 2 };
//-----------------------------------------------------------------------------
// Looptijd
static s_disp_tekst const disp_looptijd_str = { Disp_Draw_Tekst_L,  37, 20, &tekst.Looptijd_10 };
static s_disp_value const disp_looptijd_val = { Disp_Draw_Value,   207, 20, (SIZE_14 | RECHTS), INT, 0, &Runtime };
static s_disp_tekst const disp_looptijd_sec = { Disp_Draw_Tekst_L, 210, 20, &tekst.s_10 };
static s_disp_func  const disp_looptijd_control = { Disp_Control_Func, GetRuntimeMotorgroup };

static void * const lcd_diag_looptijd_disp[] = { &disp_looptijd_control, &disp_looptijd_raam_doek_ico, &disp_looptijd_str, &disp_looptijd_val, &disp_looptijd_sec, 0 };
//-----------------------------------------------------------------------------
// Reset all
static s_disp_tekst     const disp_reset_all_str = { Disp_Draw_Tekst_L, 37, 20, &tekst.Reset_all_10 };

static void * const lcd_diag_reset_all_disp[]    = { &disp_afvalemmer, &disp_reset_all_str, 0 };
static void * const lcd_diag_reset_all_ok_disp[] = { &disp_afvalemmer, &disp_reset_all_str, &disp_messagebox_bevestig,
                                                     &disp_zeker_weten_inst_str, &disp_ok_str, &disp_space_10_L, &disp_is_teken_10_L, &disp_space_10_L, &disp_ja_str, 0 };
//-----------------------------------------------------------------------------
// Motor
static s_disp_tekst_array const disp_motor_1_str = { Disp_Draw_Tekst_Array_L, 37, 20, &tekst_motor_10, UCHAR, &DispDiagMotorIndex[0], MAX_MOTOR };
static s_disp_tekst_array const disp_motor_2_str = { Disp_Draw_Tekst_Array_L, 37, 20, &tekst_motor_10, UCHAR, &DispDiagMotorIndex[1], MAX_MOTOR };
static s_disp_tekst_array const disp_motor_3_str = { Disp_Draw_Tekst_Array_L, 37, 20, &tekst_motor_10, UCHAR, &DispDiagMotorIndex[2], MAX_MOTOR };
static s_disp_value       const disp_motor_val   = { Disp_Draw_Value,        207, 20, (SIZE_14 | RECHTS), INT, 1, &val_hr_alg.Motor[0].Position };
static s_disp_func        const disp_control     = { Disp_Control_Func, GetMotorManagement };

static void * const lcd_diag_motor_1_disp[] = { &disp_control, &disp_motor_ico, &disp_motor_1_str, &disp_motor_val, &disp_14_perc, 0 };
static void * const lcd_diag_motor_2_disp[] = { &disp_motor_ico, &disp_motor_2_str, &disp_motor_val, &disp_14_perc, 0 };
static void * const lcd_diag_motor_3_disp[] = { &disp_motor_ico, &disp_motor_3_str, &disp_motor_val, &disp_14_perc, 0 };
//-----------------------------------------------------------------------------
// Vent/Klep
static void const * const tekst_device_10[] = { &tekst.Motor_1_10, &tekst.Motor_1_10, &tekst.Ventilator_10, &tekst.Klep_10 };

static s_disp_tekst_array const disp_device_str  = { Disp_Draw_Tekst_Array_L, 37, 20, &tekst_device_10, UCHAR, &DiagDeviceType, 4  };
static s_disp_value_add   const disp_device_1_nr = { Disp_Draw_Value_Add, (SIZE_10 | LINKS), INT, 0, &DispDiagDeviceNr[0] };
static s_disp_value_add   const disp_device_2_nr = { Disp_Draw_Value_Add, (SIZE_10 | LINKS), INT, 0, &DispDiagDeviceNr[1] };
static s_disp_value_add   const disp_device_3_nr = { Disp_Draw_Value_Add, (SIZE_10 | LINKS), INT, 0, &DispDiagDeviceNr[2] };
static s_disp_value       const disp_device_val  = { Disp_Draw_Value, 207, 20, (SIZE_14 | RECHTS), UCHAR, 0, &val_hr_alg.Device[0].ActualValue };

static void * const lcd_diag_device_1_disp[] = { &disp_motor_ico, &disp_device_str, &disp_space_10_L, &disp_device_1_nr, &disp_device_val, &disp_14_perc, 0 };
static void * const lcd_diag_device_2_disp[] = { &disp_motor_ico, &disp_device_str, &disp_space_10_L, &disp_device_2_nr, &disp_device_val, &disp_14_perc, 0 };
static void * const lcd_diag_device_3_disp[] = { &disp_motor_ico, &disp_device_str, &disp_space_10_L, &disp_device_3_nr, &disp_device_val, &disp_14_perc, 0 };
//-----------------------------------------------------------------------------

s_key_action const diag_group_1_key_action[] =
{
  { // Dummy
    0,                                // nr
    0,                                // index
    &DispDummy,                       // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_dummy_disp,                   // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Func,                       // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  { // Looptijd
    1,                                // nr
    0,                                // index
    &DispRuntime,                     // option
    &DiagGroupIndex,                  // Optie index 
    lcd_diag_looptijd_disp,           // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Func,                       // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  { // Reset all
    2,                                // nr
    0,                                // index
    &Motorgroup[0].TypeVent,          // option
    &DiagGroupIndex,                  // Optie index 
    lcd_diag_reset_all_disp,          // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Func,                       // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    2,                                // nr
    1,                                // index
    &Motorgroup[0].TypeVent,          // option
    &DiagGroupIndex,                  // Optie index 
    lcd_diag_reset_all_ok_disp,       // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Func,                       // void (*arrow)(void); 
    Reset_All_Func,                   // void (*enter)(void);
  },
  { // Motor 1
    3,                                // nr
    0,                                // index
    &DispDiagMotorOption[0],          // option
    &DispDiagMotorIndex[0],           // option
    lcd_diag_motor_1_disp,            // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Diag_Motor_Func,            // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  { // Vent/Klep 1
    3,                                // nr
    0,                                // index
    &DispDiagDeviceOption[0],         // option
    &DispDiagMotorIndex[0],           // option
    lcd_diag_device_1_disp,           // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Diag_Motor_Func,            // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  { // Motor 2
    4,                                // nr
    0,                                // index
    &DispDiagMotorOption[1],          // option
    &DispDiagMotorIndex[1],           // option
    lcd_diag_motor_2_disp,            // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Diag_Motor_Func,            // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  { // Vent/Klep 2
    4,                                // nr
    0,                                // index
    &DispDiagDeviceOption[1],         // option
    &DispDiagMotorIndex[1],           // option
    lcd_diag_device_2_disp,           // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Diag_Motor_Func,            // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  { // Motor 3
    5,                                // nr
    0,                                // index
    &DispDiagMotorOption[2],          // option
    &DispDiagMotorIndex[2],           // option
    lcd_diag_motor_3_disp,            // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Diag_Motor_Func,            // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  { // Vent/Klep 3
    5,                                // nr
    0,                                // index
    &DispDiagDeviceOption[2],         // option
    &DispDiagMotorIndex[2],           // option
    lcd_diag_device_3_disp,           // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Diag_Motor_Func,            // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
};

s_screen screen_diag_group_1;
s_screen const screen_diag_group_1_default =
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
  &diag_group_1_key_action[0], // first_action
  &diag_group_1_key_action[sizeof(diag_group_1_key_action)/sizeof(s_key_action) - 1], // last action 
  0, // change_flag
  0,
  0,
  0,
  0,
  0,
  0,
  &screen_diagnose_0,
  &Prev_Next_Func // prev_next_func
};


void Control_Screen_Diag_Group_1(void)
{
  Control_Screen(&screen_diag_group_1, &screen_diag_group_1_default, 1, 3);
}

void If_Exist_Goto_Screen_Diag_Group_1(unsigned char nr)
{
  DispDummy = 0;
  DiagGroupIndex = nr;
  SetDiagMotorScreen();

  Control_Screen_Diag_Group_1();
  if (opt_app.Motorgroup[DiagGroupIndex].NumberMotors > 3)
    screen_diag_group_1.nr_aantal += opt_app.Motorgroup[DiagGroupIndex].NumberMotors - 3;
  if (screen_diag_group_1.nr_aantal)
    Next_Screen(&screen_diag_group_1);
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
        if (DiagGroupIndex > 0)
        {
          DiagGroupIndex--;
          SetDiagMotorScreen();
          if ((DispRuntime == 0) && (opt_app.Motorgroup[DiagGroupIndex].NumberMotors == 0))
            DispDummy = 1;
          else
            DispDummy = 0;
          Control_Screen_Diag_Group_1();
          if (opt_app.Motorgroup[DiagGroupIndex].NumberMotors > 3)
            screen_diag_group_1.nr_aantal += opt_app.Motorgroup[DiagGroupIndex].NumberMotors - 3;
          screen_tmp = screen_ptr;
          screen_ptr = screen_ptr->prev_screen;
          Decrement_Func();
          screen_ptr = screen_tmp;
        }
        break;
      case NEXT:
        if ((DiagGroupIndex + 1) < opt_app.NumberMotorgroups)
        {
          DiagGroupIndex++;
          SetDiagMotorScreen();
          if ((DispRuntime == 0) && (opt_app.Motorgroup[DiagGroupIndex].NumberMotors == 0))
            DispDummy = 1;
          else
            DispDummy = 0;
          Control_Screen_Diag_Group_1();
          if (opt_app.Motorgroup[DiagGroupIndex].NumberMotors > 3)
            screen_diag_group_1.nr_aantal += opt_app.Motorgroup[DiagGroupIndex].NumberMotors - 3;
          screen_tmp = screen_ptr;
          screen_ptr = screen_ptr->prev_screen;
          Increment_Func();
          screen_ptr = screen_tmp;
        }
        break;
    }
  }
}

//================================================================================
static void SetDiagMotorScreen(void)
{
TMotor *pMotor;
TDevice  *pDevice;
int i;

  switch (opt_app.Motorgroup[DiagGroupIndex].Type)
  {
    case TYPE_RAAM:
	case TYPE_DOEK:
	  if (opt_app.Motorgroup[DiagGroupIndex].DigitalInput)
        DispRuntime = 1;
      else
        DispRuntime = 0;
	  break;
	case TYPE_VENT:
	case TYPE_KLEP:
	  DispRuntime = 0;
	  break;
  }

  for (i = 0; i < 3; i++)
  {
    DispDiagMotorOption[i]  = 0;
	DispDiagDeviceOption[i] = 0;
  }

  DiagDeviceType = opt_app.Motorgroup[DiagGroupIndex].Type;
  switch (opt_app.Motorgroup[DiagGroupIndex].Type)
  {
    case TYPE_RAAM:
    case TYPE_DOEK:
      pMotor = Motorgroup[DiagGroupIndex].FirstMotor;
      for (i = 0; i < 3; i++)
      {
        if (pMotor != NULL)
        {
          DispDiagMotorOption[i] = 1;
          DispDiagMotorIndex[i]  = pMotor->Number;
          pMotor = pMotor->Next;
        }
        else
          break;
      }
      DiagMotorIndex = DispDiagMotorIndex[0];
      break;
    case TYPE_VENT:
    case TYPE_KLEP:
      pDevice = Motorgroup[DiagGroupIndex].FirstMotor;
      for (i = 0; i < 3; i++)
      {
        if (pDevice != NULL)
        {
          DispDiagDeviceOption[i] = 1;
		  DispDiagMotorIndex[i]   = pDevice->Number;
          DispDiagDeviceNr[i]     = pDevice->Number + 1;
          pDevice = pDevice->Next;
        }
        else
          break;
      }
      DiagMotorIndex = DispDiagMotorIndex[0];
      DiagDeviceNr   = DispDiagDeviceNr[0];
      break;
  }
}

//================================================================================
void Arrow_Diag_Motor_Up(void)
{
unsigned int screen_nr = screen_ptr->nr;
unsigned char next = 0;
TMotor *pMotor;
TDevice  *pDevice;

  switch (opt_app.Motorgroup[DiagGroupIndex].Type)
  {
    case TYPE_RAAM:
	case TYPE_DOEK:
      pMotor = Motor[DiagMotorIndex].Prev;
	  if (pMotor == NULL)
	  {
	    Decrement_Func();
		return;
	  }
	  else
	    next = pMotor->Number;
	  break;
	case TYPE_VENT:
	case TYPE_KLEP:
      pDevice = Device[DiagMotorIndex].Prev;
	  if (pDevice == NULL)
	  {
	    Decrement_Func();
		return;
	  }
	  else
	    next = pDevice->Number;
	  break;
  }
  if (DiagMotorIndex == DispDiagMotorIndex[0])
  {
    DispDiagMotorIndex[2] = DispDiagMotorIndex[1];
    DispDiagMotorIndex[1] = DispDiagMotorIndex[0];
    DispDiagMotorIndex[0] = next;
	DispDiagDeviceNr[0] = DispDiagMotorIndex[0] + 1;
	DispDiagDeviceNr[1] = DispDiagMotorIndex[1] + 1;
	DispDiagDeviceNr[2] = DispDiagMotorIndex[2] + 1;
    DiagMotorIndex = next;
    DiagDeviceNr = next + 1;
    screen_ptr->nr_actief--;
    screen_ptr->nr--;
  }
  else
  {
    Decrement_Func();
    DiagMotorIndex = next;
    DiagDeviceNr = next + 1;
    screen_ptr->nr = screen_nr - 1;
  }
}

void Arrow_Diag_Motor_Down(void)
{
unsigned int screen_nr = screen_ptr->nr;
unsigned char ScrollScreen = 0;
unsigned char next = 0;
TMotor *pMotor;
TDevice  *pDevice;

  switch (opt_app.Motorgroup[DiagGroupIndex].Type)
  {
    case TYPE_RAAM:
	case TYPE_DOEK:
	  ScrollScreen = DispDiagMotorOption[2];
      pMotor = Motor[DiagMotorIndex].Next;
	  if (pMotor == NULL)
	  {
	    Increment_Func();
		return;
	  }
	  else
	    next = pMotor->Number;
	  break;
	case TYPE_VENT:
	case TYPE_KLEP:
	  ScrollScreen = DispDiagDeviceOption[2];
      pDevice = Device[DiagMotorIndex].Next;
	  if (pDevice == NULL)
	  {
	    Increment_Func();
		return;
	  }
	  else
	    next = pDevice->Number;
	  break;
  }
  if (ScrollScreen && (DiagMotorIndex == DispDiagMotorIndex[2]))
  {
    DispDiagMotorIndex[0] = DispDiagMotorIndex[1];
    DispDiagMotorIndex[1] = DispDiagMotorIndex[2];
    DispDiagMotorIndex[2] = next;
	DispDiagDeviceNr[0] = DispDiagMotorIndex[0] + 1;
	DispDiagDeviceNr[1] = DispDiagMotorIndex[1] + 1;
	DispDiagDeviceNr[2] = DispDiagMotorIndex[2] + 1;
    DiagMotorIndex = next;
    DiagDeviceNr   = next + 1;
    screen_ptr->nr_actief++;
    screen_ptr->nr++;
  }
  else
  {
    Increment_Func();
    DiagMotorIndex = next;
    DiagDeviceNr   = next + 1;
    screen_ptr->nr = screen_nr + 1;
  }
}

static void Arrow_Diag_Motor_Func(void)
{
  switch (key)
  {
    case UP:
      Arrow_Diag_Motor_Up();
      break;
    case DOWN:
      Arrow_Diag_Motor_Down();
      break;
    case LEFT:
      Prev_Screen();
      break;
    case RIGHT:
      switch (opt_app.Motorgroup[DiagGroupIndex].Type)
      {
        case TYPE_RAAM:
	    case TYPE_DOEK:
          If_Exist_Goto_Screen_Diag_Motor_1();
          break;
		case TYPE_VENT:
          If_Exist_Goto_Screen_Diag_Vent_1();
          break;
		case TYPE_KLEP:
          break;
	  }
      break;
  }
}

//================================================================================
static void Reset_All_Func(void)
{
TDevice *pDevice;

  pDevice = Motorgroup[DiagGroupIndex].FirstMotor;
  while (pDevice != NULL)
  {
    ResetAlarmDevice(pDevice);
	pDevice = pDevice->Next;
  }
  Increment_Func_Index();
}

//================================================================================
static void GetMotorManagement(void)
{
  Motor[DispDiagMotorIndex[screen_ptr->rel_actief]].CtrlMotorManagement |= MOTOR_MANAGEMENT_REQUEST_FLAG;
}

//================================================================================
static void GetRuntimeMotorgroup(void)
{
  Runtime = (opt_app.Motorgroup[DiagGroupIndex].Runtime + 5) / 10;
}

//================================================================================


