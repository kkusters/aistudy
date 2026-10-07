// C__DISP_OPT_SENSOREN_2.C

#include <string.h>

#include "ch_define.h"

#include "ch_alg.h"
#include "ch_alarm.h"
#include "ch_can_backbone_appl.h"
#include "ch_const.h"
#include "ch_disp.h"
#include "ch_disp_func.h"
#include "ch_disp_opt_alg_2.h"
#include "ch_disp_opt_IO_Select.h"
#include "ch_disp_option_1.h"
#include "ch_disp_value.h" 
#include "ch_disp_password.h"
#include "ch_mb_device.h"
#include "ch_IO.h"
#include "ch_key.h"
#include "ch_lcd_10.h"
#include "ch_lcd_14.h"
#include "ch_lcd_20.h"
#include "ch_lcd_bitmap.h"
#include "ch_lcd_hardware.h"
#include "ch_lcd_pixel.h"
#include "ch_main.h"
#include "ch_string.h" 
#include "ch_disp_opt_sensoren_2.h"

static void Arrow_Drukverschil_Value(void);
static void Enter_Drukverschil_Value(void);
static void Arrow_Range_Device_Address_Value(void);
static void Enter_Range_Device_Address_Value(void);
static void Arrow_End_Func(void);

//*****************************************************************************
// GLOBALS
//*****************************************************************************
static unsigned char const MaxSensoren = MAX_SENSOREN;

//***********************************************************************************************************
// SCHERM OPBOUW
//***********************************************************************************************************
//-----------------------------------------------------------------------------------------------------------
// Header
static s_disp_agri_header const disp_header = { Disp_Draw_Agri_Header, FN6, EMPTY, &tekst_inst.Opties_Luchtmengkast_10, HK_GEEN, 0, 3};

static void * const lcd_disp_header[] = { &disp_header, 0 };
//-----------------------------------------------------------------------------------------------------------
// Aantal drukverschil sensoren
static s_disp_tekst const disp_drukverschil_str = { Disp_Draw_Tekst_L, 37, 22, &tekst_inst.Drukverschil_14 };
static s_disp_value const disp_drukverschil_val = { Disp_Draw_Value,  207, 75, (SIZE_14 | RECHTS), UCHAR, 0, &opt_app.Sensoren.Drukverschil };

static void * const lcd_drukverschil_disp[] = { &disp_sensoren_inst, &disp_drukverschil_str, &disp_drukverschil_val, 0 };

static s_key_value const key_drukverschil_val = { UCHAR, 1, &opt_app.Sensoren.Drukverschil, &uchar_0, &MaxSensoren };
//-----------------------------------------------------------------------------------------------------------
// Adressen (RS485)
static int FirstNumber,  LastNumber;
static int FirstAddress, LastAddress;

static void AddressRangeFunc(void)
{
  LastNumber  = opt_app.Sensoren.Drukverschil;
  LastAddress = opt_app.Sensoren.FirstAddress + opt_app.Sensoren.Drukverschil - 1;
}

static s_tekst_6 const tekst_tm = { 4, 50, SIZE_14, "..." };
static s_disp_func  const disp_adressen_func      = { Disp_Control_Func, AddressRangeFunc };
static s_disp_tekst const disp_adressen_str       = { Disp_Draw_Tekst_L,  37, 22, &tekst_inst.Adressen_14 };
static s_disp_tekst const disp_nummer_str         = { Disp_Draw_Tekst_L,  14, 49, &tekst_inst.Nummer_10   };
static s_disp_tekst const disp_adres_str          = { Disp_Draw_Tekst_L,  14, 75, &tekst_inst.Adres_10    };
static s_disp_tekst const disp_tm_nummer_str      = { Disp_Draw_Tekst_L, 163, 49, &tekst_tm };
static s_disp_tekst const disp_tm_adres_str       = { Disp_Draw_Tekst_L, 163, 75, &tekst_tm };
static s_disp_value const disp_eerste_nummer_val  = { Disp_Draw_Value,   153, 49, (SIZE_14 | RECHTS), UCHAR, 0, &uchar_1                       };
static s_disp_value const disp_laatste_nummer_val = { Disp_Draw_Value,   207, 49, (SIZE_14 | RECHTS), INT,   0, &LastNumber                    };
static s_disp_value const disp_eerste_adres_val   = { Disp_Draw_Value,   153, 75, (SIZE_14 | RECHTS), UCHAR, 0, &opt_app.Sensoren.FirstAddress };
static s_disp_value const disp_laatste_adres_val  = { Disp_Draw_Value,   207, 75, (SIZE_14 | RECHTS), INT,   0, &LastAddress                   };

static void * const lcd_adressen_disp[] =
{
  &disp_sensoren_inst,  &disp_adressen_func, &disp_adressen_str, 
  &disp_nummer_str, &disp_dp_14_L, &disp_eerste_nummer_val, &disp_tm_nummer_str, &disp_laatste_nummer_val,
  &disp_adres_str,  &disp_dp_14_L, &disp_eerste_adres_val,  &disp_tm_adres_str,  &disp_laatste_adres_val,
  0
};

static s_key_value const key_eerste_adres_val  = { UCHAR, 3, &opt_app.Sensoren.FirstAddress, &uchar_1, &uchar_247 };
//-----------------------------------------------------------------------------------------------------------
// IO
static s_disp_tekst const disp_drukverschil_RS485_str = { Disp_Draw_Tekst_L, 47, 22, &tekst_inst.Drukverschil_14 };
static s_disp_board_IO_Selection const disp_RS485_sel = { Disp_Draw_Board_IO_Select, opt_app.Sensoren.RS485Bus, MAX_SENSOREN, &opt_app.Sensoren.Drukverschil, 1, RS485_BUS_ID, 0, DummyNotUsed };

static void * const lcd_RS485[] = { &disp_sensoren_inst, &disp_drukverschil_RS485_str, &disp_RS485_sel, 0 };
//-----------------------------------------------------------------------------------------------------------


//=============================================================================
s_key_action const opt_sensoren_2_key_action[] =
{
  { // Begin scherm
    0,                                // nr
    0,                                // index
    &start_flag,                      // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_start_disp,                   // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Start_Func,                 // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  { // Drukverschil
    1,                                // nr
    0,                                // index
    (unsigned char *)&option_on,      // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_drukverschil_disp,            // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Option_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    1,                                // nr
    1,                                // index
    (unsigned char *)&option_on,      // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_drukverschil_disp,            // display
    &disp_cursor_207_78_8,            // cursor
    &key_drukverschil_val,            // *value
    Number_Value,                     // void (*number)(void); 
    Arrow_Drukverschil_Value,         // void (*arrow)(void); 
    Enter_Drukverschil_Value,         // void (*enter)(void);
  },
  { // Address range
    2,                                // nr
    0,                                // index
    &opt_app.Sensoren.Drukverschil,   // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_adressen_disp,                // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Option_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    2,                                // nr
    1,                                // index
    &opt_app.Sensoren.Drukverschil,   // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_adressen_disp,                // display
    &disp_cursor_153_78_8,            // cursor
    &key_eerste_adres_val,            // *value
    Number_Value,                     // void (*number)(void); 
    Arrow_Range_Device_Address_Value, // void (*arrow)(void); 
    Enter_Range_Device_Address_Value, // void (*arrow)(void); 
  },
  { // IO (RS485)
    3,                                // nr
    0,                                // index
    &opt_app.Sensoren.Drukverschil,   // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_RS485,                        // display
    &disp_cursor,                     // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_Select_Func,             // void (*arrow)(void); 
    Enter_IO_Select_Func,             // void (*enter)(void);
  },
  {
    99,                               // nr
    0,                                // index
    &end_flag,                        // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_end_disp,                     // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_End_Func,                   // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
};

//-----------------------------------------------------------------------------
s_screen screen_opt_sensoren_2;
s_screen const screen_opt_sensoren_2_default =
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
  &opt_sensoren_2_key_action[0], // first_action
  &opt_sensoren_2_key_action[sizeof(opt_sensoren_2_key_action)/sizeof(s_key_action) - 1], // last action 
  0, // change_flag
  0, // punt in normaal display
  0, // punt tijdens invoeren
  0, // aantal digits ingevoerd (punt telt niet mee)
  0, // value
  0, // min value
  0, // max value
  &screen_option_1,  // vorige scherm
  0  // prev_next_func
};

void Control_Screen_Option_Sensoren_2(void)
{
  if (password_enabled & PASSWORD_ENABLED_OPT_MASK)
    start_flag = end_flag = 1;
  else
    start_flag = end_flag = 0;
  Enter_IO_Select_Func();
  
  Control_Screen(&screen_opt_sensoren_2, &screen_opt_sensoren_2_default, 1, 1);
}

//------------------------------------------------------------------------------
void CheckOptionsSensoren(void)
{
  if ((module.Drukverschil == 0) || (opt_app.Sensoren.Drukverschil == 0))
    opt_app.Sensoren = default_opt_app.Sensoren;
}

//------------------------------------------------------------------------------
static void Arrow_Drukverschil_Value(void)
{
  Arrow_Option_Value();
  if (screen_ptr->index == 0)
  {
    CheckOptionsSensoren();
    Refresh_Screen_Nr_Aantal();
  }
}

static void Enter_Drukverschil_Value(void)
{
  Increment_Func_Index_Enter_Option_Value();
  CheckOptionsSensoren();
  Refresh_Screen_Nr_Aantal();
}

//------------------------------------------------------------------------------
static unsigned char RangeDeviceAddressOk(int FirstAddress, int LastAddress)
{
int Last;
int i;

  for (i = 0; i < opt_app.NumberMotorgroups; i++)
  {
    Last = opt_app.Motorgroup[i].FirstAddress + opt_app.Motorgroup[i].NumberMotors - 1;
    if ((FirstAddress >= opt_app.Motorgroup[i].FirstAddress) && (FirstAddress <= Last))
	  return (0);
    if ((LastAddress >= opt_app.Motorgroup[i].FirstAddress) && (LastAddress <= Last))
	  return (0);
  }
  for (i = 0; i < MAX_LUCHTMENGKAST; i++)
  {
    if (opt_app.Luchtmengkast[i].Enabled)
    {
	  switch (opt_app.Luchtmengkast[i].Inblaasvent.TypeSturing)
	  {
        case TYPE_STURING_EBMBUS             :
        case TYPE_STURING_EBM_MODBUS         :
        case TYPE_STURING_EC_BLUE_MODBUS     :
        case TYPE_STURING_EC_BLUE_PREMIUM    :
        case TYPE_STURING_MB_ROSENBERG       :
        case TYPE_STURING_MB_CLIMAFAN        :
        case TYPE_STURING_MB_ROSENBERG_GEN3  :
        case TYPE_STURING_MB_NICOTRA_GEBHARDT:
          if ((opt_app.Luchtmengkast[i].Inblaasvent.Adres >= FirstAddress) && (opt_app.Luchtmengkast[i].Inblaasvent.Adres <= LastAddress))
            return (0);
	  }
	  switch (opt_app.Luchtmengkast[i].Afblaasvent.TypeSturing)
	  {
        case TYPE_STURING_EBMBUS             :
        case TYPE_STURING_EBM_MODBUS         :
        case TYPE_STURING_EC_BLUE_MODBUS     :
        case TYPE_STURING_EC_BLUE_PREMIUM    :
        case TYPE_STURING_MB_ROSENBERG       :
        case TYPE_STURING_MB_CLIMAFAN        :
        case TYPE_STURING_MB_ROSENBERG_GEN3  :
        case TYPE_STURING_MB_NICOTRA_GEBHARDT:
          if ((opt_app.Luchtmengkast[i].Afblaasvent.Adres >= FirstAddress) && (opt_app.Luchtmengkast[i].Afblaasvent.Adres <= LastAddress))
            return (0);
	  }
    }
  }
  return (1);
}

static void Arrow_Range_Device_Address_Value(void)
{
long old_val = screen_ptr->value;

  switch (key)
  {
    case LEFT:
      if (!screen_ptr->change_flag)
      {
        CheckOptionsSensoren();
        Refresh_Screen_Nr_Aantal();
      }
      Arrow_Left_Value();
      break;
    case RIGHT:
      CheckOptionsSensoren();
      Refresh_Screen_Nr_Aantal();
      Increment_Func_Index();
      break;
    case UP:
      do 
      {
        if (screen_ptr->value >= screen_ptr->max_value)
        {
          screen_ptr->value = old_val;
          break;
        }
        screen_ptr->value++;
      }
      while (RangeDeviceAddressOk(screen_ptr->value, screen_ptr->value + opt_app.Sensoren.Drukverschil - 1) == 0);
      if (old_val != screen_ptr->value)
      {
        Put_Value();
        option_change_flag = 1;
      }
      break;
    case DOWN:
      do 
      {
        if (screen_ptr->value <= screen_ptr->min_value)
        {
          screen_ptr->value = old_val;
          break;
        }
        screen_ptr->value--;
      }
      while (RangeDeviceAddressOk(screen_ptr->value, screen_ptr->value + opt_app.Sensoren.Drukverschil - 1) == 0);
      if (old_val != screen_ptr->value)
      {
        Put_Value();
        option_change_flag = 1;
      }
      break;
  }
}

static void Enter_Range_Device_Address_Value(void)
{
  if (screen_ptr->change_flag)
  {
    if ((screen_ptr->value >= screen_ptr->min_value) && (screen_ptr->value <= screen_ptr->max_value))
    {
      if (RangeDeviceAddressOk(screen_ptr->value, screen_ptr->value + opt_app.Sensoren.Drukverschil - 1))
      {
        Enter_Value();
        CheckOptionsSensoren();
        Refresh_Screen_Nr_Aantal();
        option_change_flag = 1;
      }
    }
  }
  Increment_Func_Index();
}

//*****************************************************************************
static void Arrow_End_Func(void)
{
  switch (key)
  {
    case UP:
      Decrement_Func();
      break;
    case DOWN:
      break;
    case LEFT:
      if (option_change_flag)
      {
        option_change_flag_alg = 1;
        option_change_flag = 0;
      }
      CheckOptions();
      Prev_Screen();
      break;
    case RIGHT:
      break;
  }
}

