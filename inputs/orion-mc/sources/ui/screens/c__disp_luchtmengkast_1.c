// C__DISP_LUCHTMENGKAST_1.C  

#include <string.h>

#include "ch_define.h"

#include "ch_const.h"
#include "ch_disp.h"
#include "ch_disp_func.h"
#include "ch_disp_F2_0.h"
#include "ch_disp_afblaasvent_1.h"
#include "ch_disp_inblaasvent_1.h"
#include "ch_disp_bovenklep_1.h"
#include "ch_disp_verwarming_1.h"
#include "ch_disp_password.h"
#include "ch_disp_value.h"
#include "ch_key.h"
#include "ch_lcd_10.h"
#include "ch_lcd_14.h"
#include "ch_lcd_bitmap.h"
#include "ch_lcd_pixel.h"
#include "ch_luchtmengkast.h"
#include "ch_string.h"
#include "ch_tijd.h"
#include "ch_disp_luchtmengkast_1.h"

static void Arrow_Operation_Mode_Value(void);
static void Arrow_Position_Func(void);
static void Arrow_Inblaasvent_Func(void);
static void Arrow_Afblaasvent_Func(void);
static void Arrow_Bovenklep_Func(void);
static void Arrow_Verwarming_Func(void);

unsigned char LuchtmengkastGroepIndex;
unsigned char LuchtmengkastGroepNummer;

static unsigned char DispBinnenklep;

//*****************************************************************************
// SCHERM OPBOUW
//*****************************************************************************
//-----------------------------------------------------------------------------
// Header
static s_disp_agri_header const disp_header     = { Disp_Draw_Agri_Header, FN2, EMPTY, &tekst_leeg, HK_GEEN, 0, 2};
static s_disp_tekst       const disp_header_str = { Disp_Draw_Tekst_L, 12, 12, &tekst.Fn_Luchtmengkast_10 };
static s_disp_value_add   const disp_header_val = { Disp_Draw_Value_Add, (SIZE_10 | LINKS), UCHAR, 0, &LuchtmengkastGroepNummer };

static void * const lcd_disp_header[] = { &disp_header_str, &disp_rechte_openings_haak_10_L, &disp_header_val, &disp_rechte_sluit_haak_10_L, &disp_header, 0};
//-----------------------------------------------------------------------------
// Algemeen
static s_disp_tekst const disp_14_perc = { Disp_Draw_Tekst_L, 210, 13, &tekst.perc_7 };
//-----------------------------------------------------------------------------
// Bediening
static s_disp_tekst       const disp_operation_str = { Disp_Draw_Tekst_L,        37, 20, &tekst.Bediening_10 };
static s_disp_tekst_array const disp_operation_val = { Disp_Draw_Tekst_Array_R, 217, 20, &tekst_auto_hand_uit_10, UCHAR, &val_hr_alg.LuchtmengkastGroep[0].OperationMode, 3 };

static void * const lcd_operation_disp[] = { &disp_luchtmengkast_ico, &disp_operation_str, &disp_operation_val, 0 };

static s_key_value const key_operation_val = { UCHAR, 1, &val_hr_alg.LuchtmengkastGroep[0].OperationMode, &uchar_0, &uchar_2 };
//-----------------------------------------------------------------------------
// Buitenklep / Binnenklep / Recirculatieklep / Bovenklep
static void const * const tekst_buitenklep_recirculatieklep_10[] = { &tekst.Buitenklep_10, &tekst.Recirculatieklep_10 };
static void const * const tekst_binnenklep_bovenklep_10[]        = { &tekst.Binnenklep_10, &tekst.Bovenklep_10        };
static s_disp_tekst_array const disp_buitenklep_str = { Disp_Draw_Tekst_Array_L, 37, 20, &tekst_buitenklep_recirculatieklep_10, UCHAR, &opt_app.LuchtmengkastGroep[0].TypeKlep, 2 };
static s_disp_tekst_array const disp_binnenklep_str = { Disp_Draw_Tekst_Array_L, 37, 20, &tekst_binnenklep_bovenklep_10,        UCHAR, &opt_app.LuchtmengkastGroep[0].TypeKlep, 2 };
static s_disp_value const disp_buitenklep_val = { Disp_Draw_Value, 207, 20, (SIZE_14 | RECHTS), UCHAR, 0, &val_hr_alg.LuchtmengkastGroep[0].Buitenklep.PositionPerc };
static s_disp_value const disp_binnenklep_val = { Disp_Draw_Value, 207, 20, (SIZE_14 | RECHTS), UCHAR, 0, &val_hr_alg.LuchtmengkastGroep[0].Binnenklep.PositionPerc };

static void * const lcd_buitenklep_disp[] = { &disp_luchtmengkast_ico, &disp_buitenklep_str, &disp_buitenklep_val, &disp_14_perc, 0 };
static void * const lcd_binnenklep_disp[] = { &disp_luchtmengkast_ico, &disp_binnenklep_str, &disp_binnenklep_val, &disp_14_perc, 0 };

static s_key_value const key_buitenklep_val = { UCHAR, 3, &val_hr_alg.LuchtmengkastGroep[0].Buitenklep.PositionPerc, &uchar_0, &uchar_100 };
static s_key_value const key_binnenklep_val = { UCHAR, 3, &val_hr_alg.LuchtmengkastGroep[0].Binnenklep.PositionPerc, &uchar_0, &uchar_100 };
//-----------------------------------------------------------------------------
// Invoerventilatie / Afblaasventilatie
static s_disp_tekst const disp_inblaasvent_str = { Disp_Draw_Tekst_L, 37, 20, &tekst.Inblaasvent_10 };
static s_disp_tekst const disp_afblaasvent_str = { Disp_Draw_Tekst_L, 37, 20, &tekst.Afblaasvent_10 };
static s_disp_value const disp_inblaasvent_val = { Disp_Draw_Value,  207, 20, (SIZE_14 | RECHTS), UCHAR, 0, &val_hr_alg.LuchtmengkastGroep[0].Inblaasvent.PositionPerc };
static s_disp_value const disp_afblaasvent_val = { Disp_Draw_Value,  207, 20, (SIZE_14 | RECHTS), UCHAR, 0, &val_hr_alg.LuchtmengkastGroep[0].Afblaasvent.PositionPerc };

static void * const lcd_inblaasvent_disp[] = { &disp_vent, &disp_inblaasvent_str, &disp_inblaasvent_val, &disp_14_perc, 0 };
static void * const lcd_afblaasvent_disp[] = { &disp_vent, &disp_afblaasvent_str, &disp_afblaasvent_val, &disp_14_perc, 0 };

static s_key_value const key_inblaasvent_val = { UCHAR, 3, &val_hr_alg.LuchtmengkastGroep[0].Inblaasvent.PositionPerc, &uchar_0, &uchar_100 };
static s_key_value const key_afblaasvent_val = { UCHAR, 3, &val_hr_alg.LuchtmengkastGroep[0].Afblaasvent.PositionPerc, &uchar_0, &uchar_100 };
//-----------------------------------------------------------------------------
// Verwarming
static s_disp_tekst const disp_verwarming_str = { Disp_Draw_Tekst_L, 37, 20, &tekst.Verwarming_10  };
static s_disp_value const disp_verwarming_val = { Disp_Draw_Value,  207, 20, (SIZE_14 | RECHTS), UCHAR, 0, &val_hr_alg.LuchtmengkastGroep[0].Verwarming.PositionPerc  };

static void * const lcd_verwarming_disp[] = { &disp_verwarm, &disp_verwarming_str, &disp_verwarming_val, &disp_14_perc, 0 };

static s_key_value const key_verwarming_val = { UCHAR, 3, &val_hr_alg.LuchtmengkastGroep[0].Verwarming.PositionPerc, &uchar_0, &uchar_100 };
//-----------------------------------------------------------------------------


s_key_action const luchtmengkast_1_key_action[] =
{
  { // Manual/Auto/Off
    1,                                // nr
    0,                                // index
    (unsigned char *)&option_on,      // option
    &LuchtmengkastGroepIndex,         // option
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
    &LuchtmengkastGroepIndex,         // option
    lcd_operation_disp,               // display
    &disp_cursor_operation,           // cursor
    &key_operation_val,               // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Operation_Mode_Value,       // void (*arrow)(void); 
    Increment_Func_Index_Enter_Value, // void (*enter)(void);
  },
  { // Buitenklep / Recirculatieklep
    2,                                // nr
    0,                                // index
    (unsigned char *)&option_on,      // option
    &LuchtmengkastGroepIndex,         // option
    lcd_buitenklep_disp,              // display
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
    &LuchtmengkastGroepIndex,         // option
    lcd_buitenklep_disp,              // display
    &disp_cursor_207_23_8,            // cursor
    &key_buitenklep_val,              // *value
    Number_Value,                     // void (*number)(void); 
    Arrow_Value,                      // void (*arrow)(void); 
    Increment_Func_Index_Enter_Value, // void (*enter)(void);
  },
  { // Binnenklep / Bovenklep
    3,                                // nr
    0,                                // index
    &DispBinnenklep,                  // option
    &LuchtmengkastGroepIndex,         // option
    lcd_binnenklep_disp,              // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Bovenklep_Func,             // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    3,                                // nr
    1,                                // index
    &DispBinnenklep,                  // option
    &LuchtmengkastGroepIndex,         // option
    lcd_binnenklep_disp,              // display
    &disp_cursor_207_23_8,            // cursor
    &key_binnenklep_val,              // *value
    Number_Value,                     // void (*number)(void); 
    Arrow_Value,                      // void (*arrow)(void); 
    Increment_Func_Index_Enter_Value, // void (*enter)(void);
  },
  { // Inblaasventilatie
    4,                                // nr
    0,                                // index
    (unsigned char *)&option_on,      // option
    &LuchtmengkastGroepIndex,         // option
    lcd_inblaasvent_disp,             // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Inblaasvent_Func,           // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    4,                                // nr
    1,                                // index
    (unsigned char *)&option_on,      // option
    &LuchtmengkastGroepIndex,         // option
    lcd_inblaasvent_disp,             // display
    &disp_cursor_207_23_8,            // cursor
    &key_inblaasvent_val,             // *value
    Number_Value,                     // void (*number)(void); 
    Arrow_Value,                      // void (*arrow)(void); 
    Increment_Func_Index_Enter_Value, // void (*enter)(void);
  },
  { // Afblaasventilatie
    5,                                                 // nr
    0,                                                 // index
    &opt_app.LuchtmengkastGroep[0].AfblaasventEnabled, // option
    &LuchtmengkastGroepIndex,                          // option
    lcd_afblaasvent_disp,                              // display
    0,                                                 // cursor
    &dummy_value,                                      // *value
    Dummy_Func,                                        // void (*number)(void); 
    Arrow_Afblaasvent_Func,                            // void (*arrow)(void); 
    Dummy_Func,                                        // void (*enter)(void);
  },
  {
    5,                                                 // nr
    1,                                                 // index
    &opt_app.LuchtmengkastGroep[0].AfblaasventEnabled, // option
    &LuchtmengkastGroepIndex,                          // option
    lcd_afblaasvent_disp,                              // display
    &disp_cursor_207_23_8,                             // cursor
    &key_afblaasvent_val,                              // *value
    Number_Value,                                      // void (*number)(void); 
    Arrow_Value,                                       // void (*arrow)(void); 
    Increment_Func_Index_Enter_Value,                  // void (*enter)(void);
  },
  { // Verwarming
    6,                                                // nr
    0,                                                // index
    &opt_app.LuchtmengkastGroep[0].VerwarmingEnabled, // option
    &LuchtmengkastGroepIndex,                         // option
    lcd_verwarming_disp,                              // display
    0,                                                // cursor
    &dummy_value,                                     // *value
    Dummy_Func,                                       // void (*number)(void); 
    Arrow_Verwarming_Func,                            // void (*arrow)(void); 
    Dummy_Func,                                       // void (*enter)(void);
  },
  {
    6,                                                // nr
    1,                                                // index
    &opt_app.LuchtmengkastGroep[0].VerwarmingEnabled, // option
    &LuchtmengkastGroepIndex,                         // option
    lcd_verwarming_disp,                              // display
    &disp_cursor_207_23_8,                            // cursor
    &key_verwarming_val,                              // *value
    Number_Value,                                     // void (*number)(void); 
    Arrow_Value,                                      // void (*arrow)(void); 
    Increment_Func_Index_Enter_Value,                 // void (*enter)(void);
  },
};

s_screen screen_luchtmengkast_1;
s_screen const screen_luchtmengkast_1_default =
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
  &luchtmengkast_1_key_action[0], // first_action
  &luchtmengkast_1_key_action[sizeof(luchtmengkast_1_key_action)/sizeof(s_key_action) - 1], // last action 
  0, // change_flag
  0, // punt in normaal display
  0, // punt tijdens invoeren
  0, // aantal digits ingevoerd (punt telt niet mee)
  0, // value
  0, // min value
  0, // max value
  &screen_F2_0,   // vorige scherm
  &Prev_Next_Luchtmengkast_Func // prev_next_func
};

static void SetDisplayOptions(void)
{
  LuchtmengkastGroepNummer = LuchtmengkastGroepIndex + 1;

  if (opt_app.LuchtmengkastGroep[LuchtmengkastGroepIndex].BovenklepEnabled || (opt_app.LuchtmengkastGroep[LuchtmengkastGroepIndex].TypeKlep == TYPE_KLEP_BINNEN_BUITEN))
    DispBinnenklep = 1;
  else
    DispBinnenklep = 0;
}

void Control_Screen_Luchtmengkast_1(void)
{
  Control_Screen(&screen_luchtmengkast_1, &screen_luchtmengkast_1_default, 1, 3);
}

void If_Exist_Goto_Screen_Luchtmengkast_1(unsigned char nr)
{
  LuchtmengkastGroepIndex = nr;
  SetDisplayOptions();

  Control_Screen_Luchtmengkast_1();
  if (screen_luchtmengkast_1.nr_aantal)
    Next_Screen(&screen_luchtmengkast_1);
}

//================================================================================
void Prev_Next_Luchtmengkast_Func(void)
{
s_screen *screen_tmp;

  if (screen_ptr->index == 0)
  {
    switch (key_func)
    {
      case PREV:
        if (LuchtmengkastGroepIndex > 0)
        {
          LuchtmengkastGroepIndex--;
          SetDisplayOptions();
          Control_Screen_Luchtmengkast_1();
          screen_tmp = screen_ptr;
          screen_ptr = screen_ptr->prev_screen;
          Decrement_Func();
          screen_ptr = screen_tmp;
        }
        break;
      case NEXT:
        if (((LuchtmengkastGroepIndex + 1) < MAX_LUCHTMENGKAST_GROEP) && opt_app.LuchtmengkastGroep[LuchtmengkastGroepIndex + 1].Enabled)
        {
          LuchtmengkastGroepIndex++;
          SetDisplayOptions();
          Control_Screen_Luchtmengkast_1();
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
      Increment_Scroll_Value_No_Enter();
      break;
    case DOWN:
      Decrement_Scroll_Value_No_Enter();
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
      if (val_hr_alg.LuchtmengkastGroep[LuchtmengkastGroepIndex].OperationMode == omManual)
      {
        if ((password_enabled & PASSWORD_ENABLED_SETP_MASK) == 0)
          Right_Password();
        else
          Increment_Func_Index();
      }
      break;
  }
}

//================================================================================
static void Arrow_Inblaasvent_Func(void)
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
      if (val_hr_alg.LuchtmengkastGroep[LuchtmengkastGroepIndex].OperationMode == omManual)
      {
        if ((password_enabled & PASSWORD_ENABLED_SETP_MASK) == 0)
          Right_Password();
        else
          Increment_Func_Index();
      }
	  else
	  {
	    If_Exist_Goto_Screen_Inblaasvent_1();
	  }
      break;
  }
}

//================================================================================
static void Arrow_Afblaasvent_Func(void)
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
      if (val_hr_alg.LuchtmengkastGroep[LuchtmengkastGroepIndex].OperationMode == omManual)
      {
        if ((password_enabled & PASSWORD_ENABLED_SETP_MASK) == 0)
          Right_Password();
        else
          Increment_Func_Index();
      }
	  else
	  {
	    If_Exist_Goto_Screen_Afblaasvent_1();
	  }
      break;
  }
}

//================================================================================
static void Arrow_Bovenklep_Func(void)
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
      if (val_hr_alg.LuchtmengkastGroep[LuchtmengkastGroepIndex].OperationMode == omManual)
      {
        if ((password_enabled & PASSWORD_ENABLED_SETP_MASK) == 0)
          Right_Password();
        else
          Increment_Func_Index();
      }
	  else
	  {
	    If_Exist_Goto_Screen_Bovenklep_1();
	  }
      break;
  }
}

//================================================================================
static void Arrow_Verwarming_Func(void)
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
      if (val_hr_alg.LuchtmengkastGroep[LuchtmengkastGroepIndex].OperationMode == omManual)
      {
        if ((password_enabled & PASSWORD_ENABLED_SETP_MASK) == 0)
          Right_Password();
        else
          Increment_Func_Index();
      }
	  else
	  {
	    If_Exist_Goto_Screen_Verwarming_1();
	  }
      break;
  }
}

//================================================================================

