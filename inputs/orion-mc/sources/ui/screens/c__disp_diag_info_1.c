// C__DISP_DIAG_INFO_1.C

#include <string.h>

#include "ch_define.h"
#include "ch_computer.h"
#include "ch_const.h"
#include "ch_disp.h"
#include "ch_disp_func.h"
#include "ch_disp_value.h"
#include "ch_key.h"
#include "ch_lcd_10.h"
#include "ch_lcd_14.h"
#include "ch_lcd_bitmap.h"
#include "ch_lcd_pixel.h"
#include "ch_main.h"
#include "ch_pc_com.h"
#include "ch_string.h"
#include "ch_disp_diag_info_1.h"

static void Arrow_Info_Func(void);
static void Arrow_Module_Value(void);
static void Enter_Module_Value(void);
static void Arrow_Bevestig_Module(void);
static void Enter_Bevestig_Module(void);

static long ModuleCode;
static long NewModule;

extern s_screen screen_diag_info_1;

static s_disp_agri_header const disp_header = { Disp_Draw_Agri_Header, FN5, HD_DIAG, &tekst.Info_10, HK_GEEN, 0, 2};
static void * const lcd_disp_header[] = { &disp_header, 0};

static s_tekst_21 tekst_activate_module;
static s_tekst_15 tekst_module_type[6] = {{ 16, 120, SIZE_7, "" },{ 16, 120, SIZE_7, "" },{ 16, 120, SIZE_7, "" },{ 16, 120, SIZE_7, "" },{ 16, 120, SIZE_7, "" },{ 16, 120, SIZE_7, "" }};

//----------------------------------------------------------------------------------------------------------
// versie
static s_disp_tekst const disp_computer_soort_type_str = { Disp_Draw_Tekst_L, 12, 20, &tekst_computer_soort_type };

static s_disp_tekst const disp_serie_nr_str = { Disp_Draw_Tekst_L, 12,48, &tekst.Serie_Nr_14 };
static s_disp_value_add  const disp_serienummer_value = { Disp_Draw_Value_Add, (SIZE_14 | LINKS), LONG, 0, &module.serie_number };
static s_disp_tekst_add const disp_serie_nummer_extensie_str = { Disp_Draw_Tekst_Add_L, &tekst_serie_nummer_extensie };

static s_disp_tekst const disp_computer_str = { Disp_Draw_Tekst_L, 12, 76, &tekst.Computer_14 };
static s_disp_value_add const disp_computer_nr_val = { Disp_Draw_Value_Add, (SIZE_14 | LINKS), INT, 0, &opt_alg.adres };

static void * const lcd_versie_disp[] = 
{
  &disp_computer_soort_type_control, &disp_computer_soort_type_str, &disp_space_14_L, &disp_add_versie_val, &disp_add_sub_versie_punt, &disp_add_sub_versie_nr, 
  &disp_serie_nr_str, &disp_space_14_L, &disp_serienummer_value, &disp_serie_nummer_extensie_str,
  &disp_computer_str, &disp_space_14_L, &disp_computer_nr_val, 0
};
//----------------------------------------------------------------------------------------------------------
// Module code
static s_disp_tekst const disp_wijzig_computer_type_str = { Disp_Draw_Tekst_L, 37, 22, &tekst.Wijzig_computer_type_14 };
static s_disp_value const disp_computer_type_val = { Disp_Draw_Value,  127, 75, (SIZE_14 | RECHTS), LONG, 0, &ModuleCode };

static void * const lcd_wijzig_computer_type_disp[] = { &disp_sleutel, &disp_wijzig_computer_type_str, &disp_computer_type_val, &disp_password_help, 0 };

static s_key_value const key_computer_type_val = { LONG, 8, &ModuleCode, &long_0, &long_99999999 };
//----------------------------------------------------------------------------------------------------------
// Bevestig module
static s_disp_tekst const disp_activeer_module_str = { Disp_Draw_Tekst_L, 85+3, 40+19, &tekst_activate_module };
static s_disp_tekst const disp_module_type_1_str   = { Disp_Draw_Tekst_L, 85+3, 50+19, &tekst_module_type[0] };
static s_disp_tekst const disp_module_type_2_str   = { Disp_Draw_Tekst_L, 85+3, 59+19, &tekst_module_type[1] };
static s_disp_tekst const disp_module_type_3_str   = { Disp_Draw_Tekst_L, 85+3, 68+19, &tekst_module_type[2] };

static void * const lcd_bevestig_type_disp[] = { &disp_sleutel, &disp_wijzig_computer_type_str, &disp_computer_type_val, &disp_password_help,
                                                 &disp_messagebox_bevestig, &disp_activeer_module_str,
                                                 &disp_module_type_1_str, &disp_module_type_2_str, &disp_module_type_3_str, 0 };
//----------------------------------------------------------------------------------------------------------
// Ongeldige code
static s_disp_tekst const disp_ongeldige_code_str = { Disp_Draw_Tekst_L, 85+3, 52+19, &tekst.Ongeldige_code_7 };

static void * const lcd_ongeldige_code_disp[] = { &disp_sleutel, &disp_wijzig_computer_type_str, &disp_computer_type_val, &disp_password_help,
                                                  &disp_messagebox_error, &disp_ongeldige_code_str, 0 };
//----------------------------------------------------------------------------------------------------------
// Aangeschakelde modules
static s_tekst_15 tekst_modules[10] = {{ 16, 120, SIZE_10, "" }, { 16, 120, SIZE_10, "" }, { 16, 120, SIZE_10, "" }, { 16, 120, SIZE_10, "" },
                                            { 16, 120, SIZE_10, "" }, { 16, 120, SIZE_10, "" }, { 16, 120, SIZE_10, "" }, { 16, 120, SIZE_10, "" }};

static s_disp_tekst const disp_modules_str = { Disp_Draw_Tekst_L, 12, 20, &tekst.Modules_14 };
static s_disp_tekst const disp_module_1_str   = { Disp_Draw_Tekst_L, 12, 37, &tekst_modules[0] };
static s_disp_tekst const disp_module_2_str   = { Disp_Draw_Tekst_L, 12, 51, &tekst_modules[1] };
static s_disp_tekst const disp_module_3_str   = { Disp_Draw_Tekst_L, 12, 65, &tekst_modules[2] };
static s_disp_tekst const disp_module_4_str   = { Disp_Draw_Tekst_L, 12, 79, &tekst_modules[3] };
static s_disp_tekst const disp_module_5_str   = { Disp_Draw_Tekst_L, 120, 37, &tekst_modules[4] };
static s_disp_tekst const disp_module_6_str   = { Disp_Draw_Tekst_L, 120, 51, &tekst_modules[5] };
static s_disp_tekst const disp_module_7_str   = { Disp_Draw_Tekst_L, 120, 65, &tekst_modules[6] };
static s_disp_tekst const disp_module_8_str   = { Disp_Draw_Tekst_L, 120, 79, &tekst_modules[7] };

static void * const lcd_modules_disp[] = { &disp_modules_str, &disp_module_1_str, &disp_module_2_str, &disp_module_3_str, &disp_module_4_str,
                                           &disp_module_5_str, &disp_module_6_str, &disp_module_7_str, &disp_module_8_str, 0 };
//---------------------------------------------------------------------------------------------------------


s_key_action const diag_info_1_key_action[] =
{
  { // empty
    1,                                // nr
    0,                                // index
    (unsigned char *)&option_on,      // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_versie_disp,                  // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Info_Func,                  // void (*arrow)(void); 
    Prev_Screen,                      // void (*enter)(void);
  },
  { // Wijzigen module
    1,                                // nr
    1,                                // index
    (unsigned char *)&option_on,      // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_wijzig_computer_type_disp,    // display
    &disp_cursor_127_78_8,            // cursor
    &key_computer_type_val,           // *value
    Number_Code,                      // void (*number)(void); 
    Arrow_Module_Value,               // void (*arrow)(void); 
    Enter_Module_Value,               // void (*enter)(void);
  },
  { // Bevestig module
    1,                                // nr
    2,                                // index
    (unsigned char *)&option_on,      // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_bevestig_type_disp,           // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Bevestig_Module,            // void (*arrow)(void); 
    Enter_Bevestig_Module,            // void (*enter)(void);
  },
  { // Ongeldige code
    1,                                // nr
    3,                                // index
    (unsigned char *)&option_on,      // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_ongeldige_code_disp,          // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Increment_Func_Index,             // void (*arrow)(void); 
    Increment_Func_Index,             // void (*enter)(void);
  },
  { // aangeschakelde modules
    2,                                // nr
    0,                                // index
    (unsigned char *)&option_on,      // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_modules_disp,                  // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Info_Func,                  // void (*arrow)(void); 
    Prev_Screen,                      // void (*enter)(void);
  },
};

s_screen screen_diag_info_1;
s_screen const screen_diag_info_1_default =
{
  1, // functie nr
  0, // index
  1, // max rel
  0, // rel actief
  0, // nummer weergegeven bin aantal ingeschakelde functie nummers
  0, // aantal ingeschakelde functie nummers
  lcd_disp_header,
  {0,0,0,0,0,0},
  0,3,19,
  0,3,19+28,
  0,3,19+28+28,
  &diag_info_1_key_action[0], // first_action
  &diag_info_1_key_action[sizeof(diag_info_1_key_action)/sizeof(s_key_action) - 1], // last action 
  0, // change_flag
  0,
  0,
  0,
  0,
  0,
  0,
  0
};

static void Init_Modules_Teksten(void)
{
  int i;

  for (i = 0; i < 8; i++)
    strcpy(tekst_modules[i].string, "");

  i = 0;
  if (module.CanOpen)
  {
    strcat(tekst_modules[i].string, "CANopen");
    i++;
  }
  if (module.Ventilatie)
  {
    strcat(tekst_modules[i].string, "Ventilatie");
    i++;
  }
  if (module.Luchtmengkast)
  {
    strcat(tekst_modules[i].string, "Luchtmengkast");
    i++;
  }
  if (module.BACnet)
  {
    strcat(tekst_modules[i].string, "BACnet");
    i++;
  }
  if (module.Schakelgroepen)
  {
    strcat(tekst_modules[i].string, "Schakelgroepen");
    i++;
  }
  if (module.Klep)
  {
    strcat(tekst_modules[i].string, "Klep");
    i++;
  }
  if (module.Drukverschil)
  {
    strcat(tekst_modules[i].string, "Drukverschil");
    i++;
  }
  if (module.WatchdogMode)
  {
    strcat(tekst_modules[i].string, "WatchdogMode");
    i++;
  }
  if (module.Hoogendoorn)
  {
    strcat(tekst_modules[i].string, "TCP link");
    i++;
  }

  if (i == 0)
    strcat(tekst_module_type[0].string, "Basis");
}

void Control_Screen_Diag_Info_1(void)
// zorg voor een correcte inhoud in klimaat scherm 0
{
  Init_Modules_Teksten();
  Control_Screen(&screen_diag_info_1, &screen_diag_info_1_default, 1, 1);
}

void If_Exist_Goto_Screen_Diag_Info_1(void)
{
  Control_Screen_Diag_Info_1();
  if (screen_diag_info_1.nr_aantal)
    Next_Screen(&screen_diag_info_1);
}

//========================================================================================
static void Arrow_Info_Func(void)
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
      ModuleCode = 0;
      Increment_Func_Index();
      break;
  }
}

//----------------------------------------------------------------------------------------
static long FlipValue(long Value)
{
long NewValue = 0;

  while (Value > 0)
  {
    NewValue *= 10;
    NewValue += (Value % 10);
    Value /= 10;
  }
  while ((NewValue / 100000) < 1)
    NewValue *= 10;

  return (NewValue);
}

static long CheckModuleCode(long Code)
{
unsigned long Value;
unsigned long Rest;

  if (module.serie_number != 0)
  {
    Value = module.serie_number * 2;
    Rest = Value / 1000000L;
    Value %= 1000000L;
    Value += Rest;
    Value |= 1;
    Value = FlipValue(Value);
    Value = Code - Value;
    Value ^= 19675;
    Value /= 1357L;
    Value -= 1;
    return (Value);
  }
  else
    return (-1);
} 

static void Arrow_Module_Value(void)
{
  switch (key)
  {
    case LEFT:  Arrow_Left_Value(); break;
    case RIGHT: Decrement_Func_Index(); break;
    case UP:    Increment_Value(); break;
    case DOWN:  Decrement_Value(); break;
  }
}

static void Enter_Module_Value(void)
{
unsigned long AcceptMask;
int i;

  if (Enter_Value())
  {
    AcceptMask = MODULE_CANOPEN | MODULE_VENTILATIE | MODULE_LUCHTMENGKAST | MODULE_BACNET | MODULE_SCHAKELGROEPEN | MODULE_KLEP | MODULE_DRUKVERSCHIL | MODULE_WATCHDOGMODE | MODULE_HOOGENDOORN;
    AcceptMask ^= 0xFFFFFFFF;

    NewModule = CheckModuleCode(ModuleCode);

    if (NewModule & AcceptMask)
    {
      Increment_Func_Index();
    }
    else
    {
      for (i = 0; i < 4; i++)
        strcpy(tekst_module_type[i].string, "");
      tekst_activate_module = tekst.Activeer_module_7;
      if (NewModule == MODULE_BASIS)
        strcat(tekst_module_type[1].string, "- Basis");
      else
      {
        i = 0;
        if (NewModule & MODULE_CANOPEN)
		{
          strcat(tekst_module_type[i].string, "- CANopen");
		  i++;
		}
        if (NewModule & MODULE_VENTILATIE)
		{
          strcat(tekst_module_type[i].string, "- Ventilatie");
		  i++;
		}
        if (NewModule & MODULE_LUCHTMENGKAST)
		{
          strcat(tekst_module_type[i].string, "- Luchtmengkast");
		  i++;
		}
        if (NewModule & MODULE_BACNET)
		{
          strcat(tekst_module_type[i].string, "- BACnet");
		  i++;
		}
        if (NewModule & MODULE_SCHAKELGROEPEN)
		{
          strcat(tekst_module_type[i].string, "- Schakelgroepen");
		  i++;
		}
        if (NewModule & MODULE_KLEP)
		{
          strcat(tekst_module_type[i].string, "- Klep");
		  i++;
		}
        if (NewModule & MODULE_DRUKVERSCHIL)
		{
          strcat(tekst_module_type[i].string, "- Drukverschil");
		  i++;
		}
        if (NewModule & MODULE_WATCHDOGMODE)
		{
          strcat(tekst_module_type[i].string, "- WatchdogMode");
		  i++;
		}
        if (NewModule & MODULE_HOOGENDOORN)
		{
          strcat(tekst_module_type[i].string, "- TCP link");
		  i++;
		}
      }
    }
    Increment_Func_Index();
  }
}

static void Arrow_Bevestig_Module(void)
{
  switch (key)
  {
    case LEFT:
    case RIGHT:
    case UP:
    case DOWN:
      Increment_Func_Index();
      Increment_Func_Index();
      break;
  }
}

static void Enter_Bevestig_Module(void)
{
  if (module.type != NewModule)
  {
    opt_alg.type = setp_alg.type = module.type = NewModule;
    IncrementOptionsChangeCount();
    pc_0_read_configuration_alg = 1;
    pc_1_read_configuration_alg = 1;
    pc_2_read_configuration_alg = 1;
    rs232_0_read_configuration_alg = 1;
    rs232_1_read_configuration_alg = 1;
    #ifdef ETHERNET
    ethernet_read_configuration_alg[0] = 1;
    ethernet_read_configuration_alg[1] = 1;
    ethernet_read_configuration_alg[2] = 1;
    ethernet_read_configuration_alg[3] = 1;
    #endif // ETHERNET
  }
  vlag_module_write = 1;
  comp_ram_eep_switch = 1; 
  ModuleCheck();
  Init_All_Screen();
  return;
}


