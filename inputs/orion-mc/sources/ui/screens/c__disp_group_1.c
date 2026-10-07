// C__DISP_GROUP_1.C  

#include <string.h> 

#include "ch_define.h"

#include "ch_const.h"
#include "ch_disp.h"
#include "ch_disp_func.h"
#include "ch_disp_F1_0.h"
#include "ch_disp_motor_1.h"
#include "ch_disp_klep_1.h"
#include "ch_disp_vent_1.h"
#include "ch_disp_password.h"
#include "ch_disp_value.h"
#include "ch_key.h"
#include "ch_lcd_10.h"
#include "ch_lcd_14.h"
#include "ch_lcd_bitmap.h"
#include "ch_lcd_pixel.h"
#include "ch_motor.h"
#include "ch_motorgroep.h"
#include "ch_string.h"
#include "ch_tijd.h"
#include "ch_device.h"
#include "ch_disp_group_1.h"

static void Prev_Next_Func(void);
static void Arrow_Motor_Func(void);
static void Arrow_Operation_Mode_Value(void);
static void Arrow_Position_Func(void);
static void Arrow_Position_Value(void);
static void Enter_Position_Value(void);
static void Number_Position_Value(void);

static s_disp_agri_header const disp_header = { Disp_Draw_Agri_Header, FN1, EMPTY, &tekst_leeg, HK_GEEN, 0, 2};
static s_disp_tekst           const disp_header_str_0  = { Disp_Draw_Tekst_L,  12, 12, &tekst.Fn_10 };
static s_disp_tekst_array_add const disp_header_str_1  = { Disp_Draw_Tekst_Array_Add_L, &tekst_groep_10, UCHAR, &GroupIndex, MAX_GROUP };
static void * const lcd_disp_header[] = { &disp_header_str_0, &disp_space_10_L, &disp_header_str_1, &disp_header, 0};

static int PositionManual;
static unsigned char Numeric;

unsigned char GroupIndex;
unsigned char MotorIndex;
static unsigned char DispMotorIndex[3];
static unsigned char DispMotorOption[3];
static unsigned char DispDeviceOption[3];
static unsigned char DeviceType;
static int DispDeviceNr[3];
static int MinPosition;
static int MaxPosition;
int DeviceNr;

//*****************************************************************************
// SCHERM OPBOUW
//*****************************************************************************
//-----------------------------------------------------------------------------
// Algemeen
static s_disp_tekst const disp_14_perc = { Disp_Draw_Tekst_L, 210, 13, &tekst.perc_7 };
static s_disp_bitmap_array const disp_raam_doek_ico = { Disp_Draw_Bitmap_Array, 10, 6, ico_type_array, UCHAR, &opt_app.Motorgroup[0].Type, 4 };
//-----------------------------------------------------------------------------
// Bediening
static s_disp_tekst const disp_operation_str = { Disp_Draw_Tekst_L, 37, 20, &tekst.Bediening_10 };
static s_disp_tekst_array const disp_operation_val = { Disp_Draw_Tekst_Array_R, 217, 20, &tekst_auto_hand_uit_10, UCHAR, &val_hr_alg.Motorgroup[0].OperationMode, 3 };

static void * const lcd_operation_disp[] = { &disp_raam_doek_ico, &disp_operation_str, &disp_operation_val, 0 };

static s_key_value const key_operation_val = { UCHAR, 1, &val_hr_alg.Motorgroup[0].OperationMode, &uchar_0, &uchar_2 };
//-----------------------------------------------------------------------------
// Positie
static void Disp_Min_Max_Func(void)
{
  if (opt_app.Motorgroup[GroupIndex].Type == TYPE_VENT)
  {
    MinPosition = setp_alg.Ventgroup[GroupIndex].MinVent * 10;
    MaxPosition = setp_alg.Ventgroup[GroupIndex].MaxVent * 10;
  }
  else
  {
    MinPosition = 0;
    MaxPosition = 1000;
  }
}

static s_disp_func  const disp_min_max_func = { Disp_Control_Func, Disp_Min_Max_Func };
static s_disp_tekst const disp_position_str = { Disp_Draw_Tekst_L, 37, 20, &tekst.Positie_10 };
static s_disp_value const disp_position_val = { Disp_Draw_Value,  207, 20, (SIZE_14 | RECHTS), INT, 1, &val_hr_alg.Motorgroup[0].PositionPerc };

static void * const lcd_position_disp[] = { &disp_min_max_func, &disp_raam_doek_ico, &disp_position_str, &disp_position_val, &disp_14_perc, 0 };

static s_key_value const key_position_val = { INT, 4, &val_hr_alg.Motorgroup[0].PositionPerc, &MinPosition, &MaxPosition };
//-----------------------------------------------------------------------------
// Motor
static s_disp_tekst_array const disp_motor_1_str = { Disp_Draw_Tekst_Array_L, 37, 20, &tekst_motor_10, UCHAR, &DispMotorIndex[0], MAX_MOTOR };
static s_disp_tekst_array const disp_motor_2_str = { Disp_Draw_Tekst_Array_L, 37, 20, &tekst_motor_10, UCHAR, &DispMotorIndex[1], MAX_MOTOR };
static s_disp_tekst_array const disp_motor_3_str = { Disp_Draw_Tekst_Array_L, 37, 20, &tekst_motor_10, UCHAR, &DispMotorIndex[2], MAX_MOTOR };
static s_disp_value       const disp_motor_val   = { Disp_Draw_Value,        207, 20, (SIZE_14 | RECHTS), INT, 1, &val_hr_alg.Motor[0].Position };

static void * const lcd_motor_1_disp[] = { &disp_motor_ico, &disp_motor_1_str, &disp_motor_val, &disp_14_perc, 0 };
static void * const lcd_motor_2_disp[] = { &disp_motor_ico, &disp_motor_2_str, &disp_motor_val, &disp_14_perc, 0 };
static void * const lcd_motor_3_disp[] = { &disp_motor_ico, &disp_motor_3_str, &disp_motor_val, &disp_14_perc, 0 };
//-----------------------------------------------------------------------------
// Vent/Klep
static void const * const tekst_device_10[] = { &tekst.Motor_1_10, &tekst.Motor_1_10, &tekst.Ventilator_10, &tekst.Klep_10 };

static s_disp_tekst_array const disp_device_str  = { Disp_Draw_Tekst_Array_L, 37, 20, &tekst_device_10, UCHAR, &DeviceType, 4  };
static s_disp_value_add   const disp_device_1_nr = { Disp_Draw_Value_Add, (SIZE_10 | LINKS), INT, 0, &DispDeviceNr[0] };
static s_disp_value_add   const disp_device_2_nr = { Disp_Draw_Value_Add, (SIZE_10 | LINKS), INT, 0, &DispDeviceNr[1] };
static s_disp_value_add   const disp_device_3_nr = { Disp_Draw_Value_Add, (SIZE_10 | LINKS), INT, 0, &DispDeviceNr[2] };
static s_disp_value       const disp_device_val  = { Disp_Draw_Value, 207, 20, (SIZE_14 | RECHTS), UCHAR, 0, &val_hr_alg.Device[0].ActualValue };

static void * const lcd_device_1_disp[] = { &disp_motor_ico, &disp_device_str, &disp_space_10_L, &disp_device_1_nr, &disp_device_val, &disp_14_perc, 0 };
static void * const lcd_device_2_disp[] = { &disp_motor_ico, &disp_device_str, &disp_space_10_L, &disp_device_2_nr, &disp_device_val, &disp_14_perc, 0 };
static void * const lcd_device_3_disp[] = { &disp_motor_ico, &disp_device_str, &disp_space_10_L, &disp_device_3_nr, &disp_device_val, &disp_14_perc, 0 };
//-----------------------------------------------------------------------------



s_key_action const group_1_key_action[] =
{
  { // Manual/Auto/Off
    1,                                // nr
    0,                                // index
    (unsigned char *)&option_on,      // option
    &GroupIndex,                      // option
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
    &GroupIndex,                      // option
    lcd_operation_disp,               // display
    &disp_cursor_operation,           // cursor
    &key_operation_val,               // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Operation_Mode_Value,       // void (*arrow)(void); 
    Increment_Func_Index_Enter_Value, // void (*enter)(void);
  },
  { // Position
    2,                                // nr
    0,                                // index
    (unsigned char *)&option_on,      // option
    &GroupIndex,                      // option
    lcd_position_disp,                // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Position_Func,              // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    2,                                // nr
    1,                                // index
    (unsigned char *)&option_on,      // option
    &GroupIndex,                      // option
    lcd_position_disp,                // display
    &disp_cursor_207_23_8,            // cursor
    &key_position_val,                // *value
    Number_Position_Value,            // void (*number)(void); 
    Arrow_Position_Value,             // void (*arrow)(void); 
    Enter_Position_Value,             // void (*enter)(void);
  },
  { // Motor 1
    3,                                // nr
    0,                                // index
    &DispMotorOption[0],              // option
    &DispMotorIndex[0],               // option
    lcd_motor_1_disp,                 // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Motor_Func,                 // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  { // Vent/Klep 1
    3,                                // nr
    0,                                // index
    &DispDeviceOption[0],             // option
    &DispMotorIndex[0],               // option
    lcd_device_1_disp,                // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Motor_Func,                 // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  { // Motor 2
    4,                                // nr
    0,                                // index
    &DispMotorOption[1],              // option
    &DispMotorIndex[1],               // option
    lcd_motor_2_disp,                 // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Motor_Func,                 // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  { // Vent/Klep 2
    4,                                // nr
    0,                                // index
    &DispDeviceOption[1],             // option
    &DispMotorIndex[1],               // option
    lcd_device_2_disp,                // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Motor_Func,                 // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  { // Motor 3
    5,                                // nr
    0,                                // index
    &DispMotorOption[2],              // option
    &DispMotorIndex[2],               // option
    lcd_motor_3_disp,                 // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Motor_Func,                 // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  { // Vent/Klep 3
    5,                                // nr
    0,                                // index
    &DispDeviceOption[2],             // option
    &DispMotorIndex[2],               // option
    lcd_device_3_disp,                // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Motor_Func,                 // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
};

s_screen screen_group_1;
s_screen const screen_group_1_default =
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
  &group_1_key_action[0], // first_action
  &group_1_key_action[sizeof(group_1_key_action)/sizeof(s_key_action) - 1], // last action 
  0, // change_flag
  0, // punt in normaal display
  0, // punt tijdens invoeren
  0, // aantal digits ingevoerd (punt telt niet mee)
  0, // value
  0, // min value
  0, // max value
  &screen_F1_0,   // vorige scherm
  &Prev_Next_Func // prev_next_func
};

void Control_Screen_Group_1(void)
{
  Control_Screen(&screen_group_1, &screen_group_1_default, 1, 3);
}

void If_Exist_Goto_Screen_Group_1(unsigned char nr)
{
  GroupIndex = nr;
  SetMotorScreen();

  Control_Screen_Group_1();
  if (opt_app.Motorgroup[GroupIndex].NumberMotors > 3)
    screen_group_1.nr_aantal += opt_app.Motorgroup[GroupIndex].NumberMotors - 3;
  if (screen_group_1.nr_aantal)
    Next_Screen(&screen_group_1);
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
          screen_ptr = screen_ptr->prev_screen;
          Decrement_Func();
          screen_ptr = screen_tmp;
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
          screen_ptr = screen_ptr->prev_screen;
          Increment_Func();
          screen_ptr = screen_tmp;
        }
        break;
    }
  }
}

//================================================================================
void SetMotorScreen(void)
{
TMotor *pMotor;
TDevice  *pDevice;
int i;

  for (i = 0; i < 3; i++)
  {
    DispMotorOption[i]  = 0;
	DispDeviceOption[i] = 0;
  }

  DeviceType = opt_app.Motorgroup[GroupIndex].Type;
  switch (opt_app.Motorgroup[GroupIndex].Type)
  {
    case TYPE_RAAM:
    case TYPE_DOEK:
      pMotor = Motorgroup[GroupIndex].FirstMotor;
      for (i = 0; i < 3; i++)
      {
        if (pMotor != NULL)
        {
          DispMotorOption[i] = 1;
          DispMotorIndex[i]  = pMotor->Number;
          pMotor = pMotor->Next;
        }
        else
          break;
      }
      MotorIndex = DispMotorIndex[0];
      break;
    case TYPE_VENT:
    case TYPE_KLEP:
      pDevice = Motorgroup[GroupIndex].FirstMotor;
      for (i = 0; i < 3; i++)
      {
        if (pDevice != NULL)
        {
          DispDeviceOption[i] = 1;
		  DispMotorIndex[i]   = pDevice->Number;
          DispDeviceNr[i]     = pDevice->Number + 1;
          pDevice = pDevice->Next;
        }
        else
          break;
      }
      MotorIndex = DispMotorIndex[0];
      DeviceNr   = DispDeviceNr[0];
      break;
  }
}

//================================================================================
static void Arrow_Operation_Mode_Value(void)
{
  switch (key)
  {
    case LEFT:
      Enter_Value();
      Decrement_Func_Index();
      break;
    case RIGHT:
      Enter_Value();
      Increment_Func_Index();
      break;
    case UP:
      screen_ptr->value++;
      if (screen_ptr->value > screen_ptr->max_value)
        screen_ptr->value = screen_ptr->min_value;
      screen_ptr->change_flag = 1;
      break;
    case DOWN:
      screen_ptr->value--;
      if (screen_ptr->value < screen_ptr->min_value)
        screen_ptr->value = screen_ptr->max_value;
      screen_ptr->change_flag = 1;
      break;
  }
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
      if (val_hr_alg.Motorgroup[GroupIndex].OperationMode == omManual)
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
                  val_hr_alg.Motorgroup[GroupIndex].PositionPerc = PositionManual;
                break;
    case RIGHT: Increment_Func_Index();
                if (screen_ptr->index == 0)
                  val_hr_alg.Motorgroup[GroupIndex].PositionPerc = PositionManual;
                break;
    case UP:    Correct_Decimal_Value();
                screen_ptr->value += key_inc;
                if (screen_ptr->value < screen_ptr->min_value)
                  screen_ptr->value = screen_ptr->min_value;
                else if (screen_ptr->value > screen_ptr->max_value)
                  screen_ptr->value = screen_ptr->max_value;
                screen_ptr->change_flag = 1;
                Numeric = 0;
                PositionManual = screen_ptr->value;
                break;
    case DOWN:  Correct_Decimal_Value();
                screen_ptr->value -= key_inc;
                if (screen_ptr->value < screen_ptr->min_value)
                  screen_ptr->value = screen_ptr->min_value;
                else if (screen_ptr->value > screen_ptr->max_value)
                  screen_ptr->value = screen_ptr->max_value;
                screen_ptr->change_flag = 1;
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
    val_hr_alg.Motorgroup[GroupIndex].PositionPerc = PositionManual;
}
                                
//================================================================================
void Arrow_Motor_Up(void)
{
unsigned int screen_nr = screen_ptr->nr;
unsigned char next = 0;
TMotor *pMotor;
TDevice  *pDevice;

  switch (opt_app.Motorgroup[GroupIndex].Type)
  {
    case TYPE_RAAM:
	case TYPE_DOEK:
      pMotor = Motor[MotorIndex].Prev;
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
      pDevice = Device[MotorIndex].Prev;
	  if (pDevice == NULL)
	  {
	    Decrement_Func();
		return;
	  }
	  else
	    next = pDevice->Number;
	  break;
  }
  if (MotorIndex == DispMotorIndex[0])
  {
    DispMotorIndex[2] = DispMotorIndex[1];
    DispMotorIndex[1] = DispMotorIndex[0];
    DispMotorIndex[0] = next;
	DispDeviceNr[0] = DispMotorIndex[0] + 1;
	DispDeviceNr[1] = DispMotorIndex[1] + 1;
	DispDeviceNr[2] = DispMotorIndex[2] + 1;
    MotorIndex = next;
    DeviceNr = next + 1;
    screen_ptr->nr_actief--;
    screen_ptr->nr--;
  }
  else
  {
    Decrement_Func();
    MotorIndex = next;
    DeviceNr = next + 1;
    screen_ptr->nr = screen_nr - 1;
  }
}

void Arrow_Motor_Down(void)
{
unsigned int screen_nr = screen_ptr->nr;
unsigned char ScrollScreen = 0;
unsigned char next = 0;
TMotor *pMotor;
TDevice  *pDevice;

  switch (opt_app.Motorgroup[GroupIndex].Type)
  {
    case TYPE_RAAM:
	case TYPE_DOEK:
	  ScrollScreen = DispMotorOption[2];
      pMotor = Motor[MotorIndex].Next;
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
	  ScrollScreen = DispDeviceOption[2];
      pDevice = Device[MotorIndex].Next;
	  if (pDevice == NULL)
	  {
	    Increment_Func();
		return;
	  }
	  else
	    next = pDevice->Number;
	  break;
  }
  if (ScrollScreen && (MotorIndex == DispMotorIndex[2]))
  {
    DispMotorIndex[0] = DispMotorIndex[1];
    DispMotorIndex[1] = DispMotorIndex[2];
    DispMotorIndex[2] = next;
	DispDeviceNr[0] = DispMotorIndex[0] + 1;
	DispDeviceNr[1] = DispMotorIndex[1] + 1;
	DispDeviceNr[2] = DispMotorIndex[2] + 1;
    MotorIndex = next;
    DeviceNr   = next + 1;
    screen_ptr->nr_actief++;
    screen_ptr->nr++;
  }
  else
  {
    Increment_Func();
    MotorIndex = next;
    DeviceNr   = next + 1;
    screen_ptr->nr = screen_nr + 1;
  }
}

static void Arrow_Motor_Func(void)
{
  switch (key)
  {
    case UP:
      Arrow_Motor_Up();
      break;
    case DOWN:
      Arrow_Motor_Down();
      break;
    case LEFT:
      Prev_Screen();
      break;
    case RIGHT:
      switch (opt_app.Motorgroup[GroupIndex].Type)
      {
        case TYPE_RAAM:
	    case TYPE_DOEK:
          If_Exist_Goto_Screen_Motor_1();
          break;
		case TYPE_VENT:
          If_Exist_Goto_Screen_Vent_1();
          break;
		case TYPE_KLEP:
          If_Exist_Goto_Screen_Klep_1();
          break;
	  }
  }
}

//================================================================================

