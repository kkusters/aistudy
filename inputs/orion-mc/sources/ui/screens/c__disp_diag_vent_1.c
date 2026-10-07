// C__DISP_DIAG_VENT_1.C

#include <string.h>

#include "ch_define.h"

#include "ch_const.h"
#include "ch_disp.h"
#include "ch_disp_diagnose_0.h"
#include "ch_disp_diag_groep_1.h"
#include "ch_disp_func.h"
#include "ch_disp_value.h"
#include "ch_mb_device.h"
#include "ch_IO.h"
#include "ch_key.h"
#include "ch_lcd_7.h"
#include "ch_lcd_10.h"
#include "ch_lcd_14.h"
#include "ch_lcd_bitmap.h"
#include "ch_lcd_pixel.h"
#include "ch_string.h"
#include "ch_device.h"
#include "ch_disp_diag_vent_1.h"

static void Prev_Next_Func(void);
static void GetDeviceVersionNumber(void);
static void GetDeviceRunningHours(void);
static void GetDeviceEnergyConsumption(void);
static void GetDeviceTempPowerModule(void);
static void GetDeviceTempMotor(void);
static void GetDeviceTempElectronics(void);
static void GetDeviceMaxRpm(void);
static void Arrow_Maximum_Rpm_Value(void);
static void Enter_Maximum_Rpm_Value(void);

static unsigned int DeviceNr = 0;
static int DiagDeviceAddress;

static unsigned char TempPowerModule;
static unsigned char TempMotor;
static unsigned char TempElectronics;
static unsigned long Runtime;
static unsigned int  Version;
static unsigned int  MaximumRpm;
static unsigned int  Power;

//*****************************************************************************
// SCHERM OPBOUW
//*****************************************************************************
//-----------------------------------------------------------------------------
// Header
static s_disp_agri_header const disp_header       = { Disp_Draw_Agri_Header, FN5, EMPTY, &tekst_leeg, HK_GEEN, 0, 2};
static s_disp_tekst       const disp_header_str_0 = { Disp_Draw_Tekst_L, 12, 12, &tekst.Diag_10 };
static s_disp_tekst_add   const disp_header_str_1 = { Disp_Draw_Tekst_Add_L, &tekst.Ventilator_10 };
static s_disp_value_add   const disp_header_val_1 = { Disp_Draw_Value_Add, (SIZE_10 | LINKS), UINT, 0, &DeviceNr };

static void * const lcd_disp_header[] = { &disp_header_str_0, &disp_space_10_L, &disp_header_str_1, &disp_space_10_L, &disp_header_val_1, &disp_header, 0};
//-----------------------------------------------------------------------------
// Algemeen
static s_tekst_2 const tekst_h = { 2, 16, SIZE_10, "h" };
static s_tekst_2 const tekst_W = { 2, 16, SIZE_10, "W" };
//-----------------------------------------------------------------------------
// Versie nummer
static s_disp_bitmap const disp_versienummer_ico = { Disp_Draw_Bitmap,  11,  6, &fabrieksinst_ico };
static s_disp_tekst  const disp_versienummer_str = { Disp_Draw_Tekst_L, 37, 20, &tekst.Versie_10 };
static s_disp_value  const disp_versienummer_val = { Disp_Draw_Value,  207, 20, (SIZE_14 | RECHTS), UINT, 2, &Version };

static void * const lcd_diag_versienummer_disp[] = { &disp_versienummer_ico, &disp_versienummer_str, &disp_versienummer_val, 0 };
//-----------------------------------------------------------------------------
// Draaiuren
static s_disp_func   const disp_draaiuren_ctl = { Disp_Control_Func, GetDeviceRunningHours };
static s_disp_bitmap const disp_draaiuren_ico = { Disp_Draw_Bitmap,       7,  8, &ico_klok_switch_on };
static s_disp_tekst  const disp_draaiuren_str = { Disp_Draw_Tekst_L,     37, 20, &tekst.Draaiuren_10 };
static s_disp_time   const disp_draaiuren_val = { Disp_Draw_Urenteller, 219, 20, (SIZE_14 | RECHTS), &Runtime };

static void * const lcd_diag_draaiuren_disp[] = { &disp_draaiuren_ctl, &disp_draaiuren_ico, &disp_draaiuren_str, &disp_draaiuren_val, 0 };
//-----------------------------------------------------------------------------
// Vermogen
static s_disp_func   const disp_vermogen_ctl = { Disp_Control_Func, GetDeviceEnergyConsumption };
static s_disp_bitmap const disp_vermogen_ico = { Disp_Draw_Bitmap,    7,  7, &vermogen_ico };
static s_disp_tekst  const disp_vermogen_str = { Disp_Draw_Tekst_L,  37, 20, &tekst.Vermogen_10 };
static s_disp_value  const disp_vermogen_val = { Disp_Draw_Value,   207, 20, (SIZE_14 | RECHTS), UINT, 0, &Power };
static s_disp_tekst  const disp_vermogen_wtt = { Disp_Draw_Tekst_L, 210, 20, &tekst_W };

static void * const lcd_diag_vermogen_disp[] = { &disp_vermogen_ctl, &disp_vermogen_ico, &disp_vermogen_str, &disp_vermogen_val, &disp_vermogen_wtt, 0 };
//-----------------------------------------------------------------------------
// Motor temperatuur
static s_disp_func   const disp_temp_motor_ctl = { Disp_Control_Func, GetDeviceTempMotor };
static s_disp_bitmap const disp_temp_motor_ico = { Disp_Draw_Bitmap,    7,  7, &motor_temp_ico };
static s_disp_tekst  const disp_temp_motor_str = { Disp_Draw_Tekst_L,  37, 20, &tekst.Temp_motor_10 };
static s_disp_value  const disp_temp_motor_val = { Disp_Draw_Value,   207, 20, (SIZE_14 | RECHTS), UCHAR, 0, &TempMotor };

static void * const lcd_diag_temp_motor_disp[] = { &disp_temp_motor_ctl, &disp_temp_motor_ico, &disp_temp_motor_str, &disp_temp_motor_val, &disp_14_graden, 0 };
//-----------------------------------------------------------------------------
// Electronica temperatuur
static s_disp_func   const disp_temp_elec_ctl = { Disp_Control_Func, GetDeviceTempElectronics };
static s_disp_tekst  const disp_temp_elec_str = { Disp_Draw_Tekst_L,  37, 20, &tekst.Temp_electronica_10 };
static s_disp_value  const disp_temp_elec_val = { Disp_Draw_Value,   207, 20, (SIZE_14 | RECHTS), UCHAR, 0, &TempElectronics };

static void * const lcd_diag_temp_elec_disp[] = { &disp_temp_elec_ctl, &disp_temp_motor_ico, &disp_temp_elec_str, &disp_temp_elec_val, &disp_14_graden, 0 };
//-----------------------------------------------------------------------------
// Temperatuur power module
static s_disp_func   const disp_temp_power_ctl = { Disp_Control_Func, GetDeviceTempPowerModule };
static s_disp_tekst  const disp_temp_power_str = { Disp_Draw_Tekst_L,  37, 20, &tekst.Temp_power_module_10 };
static s_disp_value  const disp_temp_power_val = { Disp_Draw_Value,   207, 20, (SIZE_14 | RECHTS), UCHAR, 0, &TempPowerModule };

static void * const lcd_diag_temp_power_disp[] = { &disp_temp_power_ctl, &disp_temp_motor_ico, &disp_temp_power_str, &disp_temp_power_val, &disp_14_graden, 0 };
//-----------------------------------------------------------------------------
// Status
static s_disp_bitmap const disp_status_ico = { Disp_Draw_Bitmap,    9, 10, &ico_alarm };
static s_disp_tekst  const disp_status_str = { Disp_Draw_Tekst_L,  37, 20, &tekst.Alarm_Code_10 };
static s_disp_value  const disp_status_val = { Disp_Draw_Value,   207, 20, (SIZE_14 | RECHTS), HEX_INT, 0, &alarm_hr_alg.mbDevice[0].AlarmCode };
static s_disp_tekst  const disp_status_hex = { Disp_Draw_Tekst_L, 210, 20, &tekst_h };

static void * const lcd_diag_status_disp[] = { &disp_status_ico, &disp_status_str, &disp_status_val, &disp_status_hex, 0 };
//-----------------------------------------------------------------------------
// Maximum Rpm
static s_disp_func   const disp_maximum_rpm_ctl = { Disp_Control_Func, GetDeviceMaxRpm };
static s_disp_bitmap const disp_maximum_rpm_ico = { Disp_Draw_Bitmap,    7,  8, &max_rpm_ico };
static s_disp_tekst  const disp_maximum_rpm_str = { Disp_Draw_Tekst_L,  37, 20, &tekst.Maximum_rpm_10 };
static s_disp_value  const disp_maximum_rpm_val = { Disp_Draw_Value,   207, 20, (SIZE_14 | RECHTS), UINT, 0, &MaximumRpm };

static void * const lcd_diag_maximum_rpm_disp[] = { &disp_maximum_rpm_ctl, &disp_maximum_rpm_ico, &disp_maximum_rpm_str, &disp_maximum_rpm_val, 0 };

static s_key_value const key_maximum_rpm_val = { UINT, 4, &MaximumRpm, &uint_0, &uint_9999 };
//-----------------------------------------------------------------------------

s_key_action const diag_vent_1_key_action[] =
{
  { // Versie nummer
    1,                           // nr
    0,                           // index
    (unsigned char *)&Version,   // option
    &DiagMotorIndex,             // Optie index 
    lcd_diag_versienummer_disp,  // display
    0,                           // cursor
    &dummy_value,                // *value
    Dummy_Func,                  // void (*number)(void); 
    Arrow_Change_Func,           // void (*arrow)(void); 
    Dummy_Func,                  // void (*enter)(void);
  },
  { // Draaiuren
    2,                           // nr
    0,                           // index
    (unsigned char *)&option_on, // option
    &DiagMotorIndex,             // Optie index 
    lcd_diag_draaiuren_disp,     // display
    0,                           // cursor
    &dummy_value,                // *value
    Dummy_Func,                  // void (*number)(void); 
    Arrow_Change_Func,           // void (*arrow)(void); 
    Dummy_Func,                  // void (*enter)(void);
  },
  { // Vermogen
    3,                           // nr
    0,                           // index
    (unsigned char *)&option_on, // option
    &DiagMotorIndex,             // Optie index 
    lcd_diag_vermogen_disp,      // display
    0,                           // cursor
    &dummy_value,                // *value
    Dummy_Func,                  // void (*number)(void); 
    Arrow_Change_Func,           // void (*arrow)(void); 
    Dummy_Func,                  // void (*enter)(void);
  },
  { // Status
    4,                           // nr
    0,                           // index
    (unsigned char *)&option_on, // option
    &DiagMotorIndex,             // Optie index 
    lcd_diag_status_disp,        // display
    0,                           // cursor
    &dummy_value,                // *value
    Dummy_Func,                  // void (*number)(void); 
    Arrow_Change_Func,           // void (*arrow)(void); 
    Dummy_Func,                  // void (*enter)(void);
  },
  { // Temperatuur motor
    5,                           // nr
    0,                           // index
    (unsigned char *)&option_on, // option
    &DiagMotorIndex,             // Optie index 
    lcd_diag_temp_motor_disp,    // display
    0,                           // cursor
    &dummy_value,                // *value
    Dummy_Func,                  // void (*number)(void); 
    Arrow_Change_Func,           // void (*arrow)(void); 
    Dummy_Func,                  // void (*enter)(void);
  },
  { // Temperatuur electronica
    6,                           // nr
    0,                           // index
    (unsigned char *)&option_on, // option
    &DiagMotorIndex,             // Optie index 
    lcd_diag_temp_elec_disp,     // display
    0,                           // cursor
    &dummy_value,                // *value
    Dummy_Func,                  // void (*number)(void); 
    Arrow_Change_Func,           // void (*arrow)(void); 
    Dummy_Func,                  // void (*enter)(void);
  },
  { // Temperatuur power module
    7,                           // nr
    0,                           // index
    (unsigned char *)&option_on, // option
    &DiagMotorIndex,             // Optie index 
    lcd_diag_temp_power_disp,    // display
    0,                           // cursor
    &dummy_value,                // *value
    Dummy_Func,                  // void (*number)(void); 
    Arrow_Change_Func,           // void (*arrow)(void); 
    Dummy_Func,                  // void (*enter)(void);
  },
  { // Maximum Rpm
    8,                           // nr
    0,                           // index
    (unsigned char *)&option_on, // option
    &DiagMotorIndex,             // Optie index 
    lcd_diag_maximum_rpm_disp,   // display
    0,                           // cursor
    &dummy_value,                // *value
    Dummy_Func,                  // void (*number)(void); 
    Arrow_Change_Func,           // void (*arrow)(void); 
    Dummy_Func,                  // void (*enter)(void);
  },
  {
    8,                           // nr
    1,                           // index
    (unsigned char *)&option_on, // option
    &DiagMotorIndex,             // Optie index 
    lcd_diag_maximum_rpm_disp,   // display
    &disp_cursor_207_23_8,       // cursor
    &key_maximum_rpm_val,        // *value
    Number_Value,                // void (*number)(void); 
    Arrow_Maximum_Rpm_Value,     // void (*arrow)(void); 
    Enter_Maximum_Rpm_Value,     // void (*enter)(void);
  },
};

s_screen screen_diag_vent_1;
s_screen const screen_diag_vent_1_default =
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
  &diag_vent_1_key_action[0], // first_action
  &diag_vent_1_key_action[sizeof(diag_vent_1_key_action)/sizeof(s_key_action) - 1], // last action 
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


//================================================================================
void Control_Screen_Diag_Vent_1(void)
{
  Control_Screen(&screen_diag_vent_1, &screen_diag_vent_1_default, 1, 3);
}

void If_Exist_Goto_Screen_Diag_Vent_1(void)
{
int Group;

  DeviceNr = DiagMotorIndex + 1;
  Group = opt_app.Device[DiagMotorIndex].GroupNumber - 1;
  if ((Group < 0) || (Group >= MAX_GROUP))
	return;
  DiagDeviceAddress = (int)opt_app.Motorgroup[Group].FirstAddress + (DeviceNr - opt_app.Motorgroup[Group].FirstNumber);

  GetDeviceVersionNumber();
  Control_Screen_Diag_Vent_1();
  if (screen_diag_vent_1.nr_aantal)
    Next_Screen(&screen_diag_vent_1);
}

//================================================================================
static void Prev_Next_Func(void)
{
TDevice *pDevice;
//s_screen *screen_tmp;

  if (screen_ptr->index == 0)
  {
    switch (key_func)
    {
      case PREV:
        pDevice = Device[DiagMotorIndex].Prev;
        if (pDevice != NULL)
        {
          Prev_Screen();
          Arrow_Diag_Motor_Up();
		  If_Exist_Goto_Screen_Diag_Vent_1();
        }
        break;
      case NEXT:
        pDevice = Device[DiagMotorIndex].Next;
        if (pDevice != NULL)
        {
          Prev_Screen();
          Arrow_Diag_Motor_Down();
		  If_Exist_Goto_Screen_Diag_Vent_1();
        }
        break;
    }
  }
}

//================================================================================
static void GetDeviceVersionNumber(void)
{
  Version = mbDeviceGetVersion(DiagDeviceAddress);
}

static void GetDeviceRunningHours(void)
{
  Runtime = mbDeviceGetRunningHours(DiagDeviceAddress);
}

static void GetDeviceEnergyConsumption(void)
{
  Power = mbDeviceGetEnergyConsumption(DiagDeviceAddress);
}

static void GetDeviceTempPowerModule(void)
{
  TempPowerModule = mbDeviceGetTempPowerModule(DiagDeviceAddress);
}

static void GetDeviceTempMotor(void)
{
  TempMotor = mbDeviceGetTempMotor(DiagDeviceAddress);
}

static void GetDeviceTempElectronics(void)
{
  TempElectronics = mbDeviceGetTempElectronics(DiagDeviceAddress);
}

static void GetDeviceMaxRpm(void)
{
  if (screen_ptr->index == 0)
    MaximumRpm = mbDeviceGetMaximumRpm(DiagDeviceAddress);
}

static void Arrow_Maximum_Rpm_Value(void)
{
  Arrow_Value();
  if ((key == LEFT) || (key == RIGHT))
    mbDeviceSetMaximumRpm(DiagDeviceAddress, MaximumRpm);
}

static void Enter_Maximum_Rpm_Value(void)
{
  if (Enter_Value())
    mbDeviceSetMaximumRpm(DiagDeviceAddress, MaximumRpm);
  Increment_Func_Index();
}

//================================================================================

