// C__DISP_F3_0.C

#include "ch_define.h"

#include "ch_const.h"
#include "ch_disp.h"
#include "ch_disp_func.h"
#include "ch_key.h"
#include "ch_lcd_10.h"
#include "ch_lcd_14.h"
#include "ch_lcd_bitmap.h"
#include "ch_lcd_pixel.h"
#include "ch_string.h"                  
#include "ch_disp_F3_0.h"

static unsigned char OptionSensor[MAX_SENSOREN];

//*****************************************************************************
// SCHERM OPBOUW
//*****************************************************************************
//-----------------------------------------------------------------------------
// Header
static s_disp_agri_header const disp_header = { Disp_Draw_Agri_Header, FN3, EMPTY, &tekst.F3_10, HK_GEEN, 0, 1};
static void * const lcd_disp_header[] = { &disp_header, 0};
//-----------------------------------------------------------------------------
// Dummy screen
static unsigned char DummyScreen;
static void * const lcd_dummy_disp[] = { 0 };
//-----------------------------------------------------------------------------
// Drukverschil
static s_disp_tekst const disp_pa_7 = { Disp_Draw_Tekst_L, 210, 13, &tekst_inst.Pa_7 };

static s_disp_tekst const disp_drukverschil_1_str  = { Disp_Draw_Tekst_L, 37, 20, &tekst.Drukverschil_1_10  };
static s_disp_tekst const disp_drukverschil_2_str  = { Disp_Draw_Tekst_L, 37, 20, &tekst.Drukverschil_2_10  };
static s_disp_tekst const disp_drukverschil_3_str  = { Disp_Draw_Tekst_L, 37, 20, &tekst.Drukverschil_3_10  };
static s_disp_tekst const disp_drukverschil_4_str  = { Disp_Draw_Tekst_L, 37, 20, &tekst.Drukverschil_4_10  };
static s_disp_tekst const disp_drukverschil_5_str  = { Disp_Draw_Tekst_L, 37, 20, &tekst.Drukverschil_5_10  };
static s_disp_tekst const disp_drukverschil_6_str  = { Disp_Draw_Tekst_L, 37, 20, &tekst.Drukverschil_6_10  };
static s_disp_tekst const disp_drukverschil_7_str  = { Disp_Draw_Tekst_L, 37, 20, &tekst.Drukverschil_7_10  };
static s_disp_tekst const disp_drukverschil_8_str  = { Disp_Draw_Tekst_L, 37, 20, &tekst.Drukverschil_8_10  };
static s_disp_tekst const disp_drukverschil_9_str  = { Disp_Draw_Tekst_L, 37, 20, &tekst.Drukverschil_9_10  };
static s_disp_tekst const disp_drukverschil_10_str = { Disp_Draw_Tekst_L, 37, 20, &tekst.Drukverschil_10_10 };
static s_disp_tekst const disp_drukverschil_11_str = { Disp_Draw_Tekst_L, 37, 20, &tekst.Drukverschil_11_10 };
static s_disp_tekst const disp_drukverschil_12_str = { Disp_Draw_Tekst_L, 37, 20, &tekst.Drukverschil_12_10 };
static s_disp_tekst const disp_drukverschil_13_str = { Disp_Draw_Tekst_L, 37, 20, &tekst.Drukverschil_13_10 };
static s_disp_tekst const disp_drukverschil_14_str = { Disp_Draw_Tekst_L, 37, 20, &tekst.Drukverschil_14_10 };
static s_disp_tekst const disp_drukverschil_15_str = { Disp_Draw_Tekst_L, 37, 20, &tekst.Drukverschil_15_10 };
static s_disp_tekst const disp_drukverschil_16_str = { Disp_Draw_Tekst_L, 37, 20, &tekst.Drukverschil_16_10 };
static s_disp_value const disp_drukverschil_1_val  = { Disp_Draw_Value, 207, 20, (SIZE_14 | RECHTS), INT, 0, &val_hr_alg.Sensoren.Drukverschil[0]  };
static s_disp_value const disp_drukverschil_2_val  = { Disp_Draw_Value, 207, 20, (SIZE_14 | RECHTS), INT, 0, &val_hr_alg.Sensoren.Drukverschil[1]  };
static s_disp_value const disp_drukverschil_3_val  = { Disp_Draw_Value, 207, 20, (SIZE_14 | RECHTS), INT, 0, &val_hr_alg.Sensoren.Drukverschil[2]  };
static s_disp_value const disp_drukverschil_4_val  = { Disp_Draw_Value, 207, 20, (SIZE_14 | RECHTS), INT, 0, &val_hr_alg.Sensoren.Drukverschil[3]  };
static s_disp_value const disp_drukverschil_5_val  = { Disp_Draw_Value, 207, 20, (SIZE_14 | RECHTS), INT, 0, &val_hr_alg.Sensoren.Drukverschil[4]  };
static s_disp_value const disp_drukverschil_6_val  = { Disp_Draw_Value, 207, 20, (SIZE_14 | RECHTS), INT, 0, &val_hr_alg.Sensoren.Drukverschil[5]  };
static s_disp_value const disp_drukverschil_7_val  = { Disp_Draw_Value, 207, 20, (SIZE_14 | RECHTS), INT, 0, &val_hr_alg.Sensoren.Drukverschil[6]  };
static s_disp_value const disp_drukverschil_8_val  = { Disp_Draw_Value, 207, 20, (SIZE_14 | RECHTS), INT, 0, &val_hr_alg.Sensoren.Drukverschil[7]  };
static s_disp_value const disp_drukverschil_9_val  = { Disp_Draw_Value, 207, 20, (SIZE_14 | RECHTS), INT, 0, &val_hr_alg.Sensoren.Drukverschil[8]  };
static s_disp_value const disp_drukverschil_10_val = { Disp_Draw_Value, 207, 20, (SIZE_14 | RECHTS), INT, 0, &val_hr_alg.Sensoren.Drukverschil[9]  };
static s_disp_value const disp_drukverschil_11_val = { Disp_Draw_Value, 207, 20, (SIZE_14 | RECHTS), INT, 0, &val_hr_alg.Sensoren.Drukverschil[10] };
static s_disp_value const disp_drukverschil_12_val = { Disp_Draw_Value, 207, 20, (SIZE_14 | RECHTS), INT, 0, &val_hr_alg.Sensoren.Drukverschil[11] };
static s_disp_value const disp_drukverschil_13_val = { Disp_Draw_Value, 207, 20, (SIZE_14 | RECHTS), INT, 0, &val_hr_alg.Sensoren.Drukverschil[12] };
static s_disp_value const disp_drukverschil_14_val = { Disp_Draw_Value, 207, 20, (SIZE_14 | RECHTS), INT, 0, &val_hr_alg.Sensoren.Drukverschil[13] };
static s_disp_value const disp_drukverschil_15_val = { Disp_Draw_Value, 207, 20, (SIZE_14 | RECHTS), INT, 0, &val_hr_alg.Sensoren.Drukverschil[14] };
static s_disp_value const disp_drukverschil_16_val = { Disp_Draw_Value, 207, 20, (SIZE_14 | RECHTS), INT, 0, &val_hr_alg.Sensoren.Drukverschil[15] };

static void * const lcd_drukverschil_1_disp[]  = { &disp_sensoren_ico, &disp_drukverschil_1_str,  &disp_drukverschil_1_val,  &disp_pa_7, 0 };
static void * const lcd_drukverschil_2_disp[]  = { &disp_sensoren_ico, &disp_drukverschil_2_str,  &disp_drukverschil_2_val,  &disp_pa_7, 0 };
static void * const lcd_drukverschil_3_disp[]  = { &disp_sensoren_ico, &disp_drukverschil_3_str,  &disp_drukverschil_3_val,  &disp_pa_7, 0 };
static void * const lcd_drukverschil_4_disp[]  = { &disp_sensoren_ico, &disp_drukverschil_4_str,  &disp_drukverschil_4_val,  &disp_pa_7, 0 };
static void * const lcd_drukverschil_5_disp[]  = { &disp_sensoren_ico, &disp_drukverschil_5_str,  &disp_drukverschil_5_val,  &disp_pa_7, 0 };
static void * const lcd_drukverschil_6_disp[]  = { &disp_sensoren_ico, &disp_drukverschil_6_str,  &disp_drukverschil_6_val,  &disp_pa_7, 0 };
static void * const lcd_drukverschil_7_disp[]  = { &disp_sensoren_ico, &disp_drukverschil_7_str,  &disp_drukverschil_7_val,  &disp_pa_7, 0 };
static void * const lcd_drukverschil_8_disp[]  = { &disp_sensoren_ico, &disp_drukverschil_8_str,  &disp_drukverschil_8_val,  &disp_pa_7, 0 };
static void * const lcd_drukverschil_9_disp[]  = { &disp_sensoren_ico, &disp_drukverschil_9_str,  &disp_drukverschil_9_val,  &disp_pa_7, 0 };
static void * const lcd_drukverschil_10_disp[] = { &disp_sensoren_ico, &disp_drukverschil_10_str, &disp_drukverschil_10_val, &disp_pa_7, 0 };
static void * const lcd_drukverschil_11_disp[] = { &disp_sensoren_ico, &disp_drukverschil_11_str, &disp_drukverschil_11_val, &disp_pa_7, 0 };
static void * const lcd_drukverschil_12_disp[] = { &disp_sensoren_ico, &disp_drukverschil_12_str, &disp_drukverschil_12_val, &disp_pa_7, 0 };
static void * const lcd_drukverschil_13_disp[] = { &disp_sensoren_ico, &disp_drukverschil_13_str, &disp_drukverschil_13_val, &disp_pa_7, 0 };
static void * const lcd_drukverschil_14_disp[] = { &disp_sensoren_ico, &disp_drukverschil_14_str, &disp_drukverschil_14_val, &disp_pa_7, 0 };
static void * const lcd_drukverschil_15_disp[] = { &disp_sensoren_ico, &disp_drukverschil_15_str, &disp_drukverschil_15_val, &disp_pa_7, 0 };
static void * const lcd_drukverschil_16_disp[] = { &disp_sensoren_ico, &disp_drukverschil_16_str, &disp_drukverschil_16_val, &disp_pa_7, 0 };
//-----------------------------------------------------------------------------

s_key_action const F3_0_key_action[] =
{
  {
    0,                                // nr
    0,                                // index
    &DummyScreen,                     // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_dummy_disp,                   // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Dummy_Func,                       // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    1,                                // nr
    0,                                // index
    &OptionSensor[0],                 // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_drukverschil_1_disp,          // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Change_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    1,                                // nr
    0,                                // index
    &OptionSensor[1],                 // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_drukverschil_2_disp,          // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Change_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    1,                                // nr
    0,                                // index
    &OptionSensor[2],                 // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_drukverschil_3_disp,          // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Change_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    1,                                // nr
    0,                                // index
    &OptionSensor[3],                 // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_drukverschil_4_disp,          // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Change_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    1,                                // nr
    0,                                // index
    &OptionSensor[4],                 // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_drukverschil_5_disp,          // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Change_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    1,                                // nr
    0,                                // index
    &OptionSensor[5],                 // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_drukverschil_6_disp,          // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Change_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    1,                                // nr
    0,                                // index
    &OptionSensor[6],                 // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_drukverschil_7_disp,          // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Change_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    1,                                // nr
    0,                                // index
    &OptionSensor[7],                 // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_drukverschil_8_disp,          // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Change_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    1,                                // nr
    0,                                // index
    &OptionSensor[8],                 // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_drukverschil_9_disp,          // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Change_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    1,                                // nr
    0,                                // index
    &OptionSensor[9],                 // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_drukverschil_10_disp,         // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Change_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    1,                                // nr
    0,                                // index
    &OptionSensor[10],                // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_drukverschil_11_disp,         // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Change_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    1,                                // nr
    0,                                // index
    &OptionSensor[11],                // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_drukverschil_12_disp,         // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Change_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    1,                                // nr
    0,                                // index
    &OptionSensor[12],                // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_drukverschil_13_disp,         // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Change_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    1,                                // nr
    0,                                // index
    &OptionSensor[13],                // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_drukverschil_14_disp,         // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Change_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    1,                                // nr
    0,                                // index
    &OptionSensor[14],                // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_drukverschil_15_disp,         // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Change_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    1,                                // nr
    0,                                // index
    &OptionSensor[15],                // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_drukverschil_16_disp,         // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Change_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
};

s_screen screen_F3_0;
s_screen const screen_F3_0_default =
{
  1, // functie nr
  0, // index
  0, // max rel
  0, // rel actief
  0, // nummer weergegeven bin aantal ingeschakelde functie nummers
  0, // aantal ingeschakelde functie nummers
  lcd_disp_header,
  {0,0,0,0,0,0},
  0,3,19,
  0,3,19+28,
  0,3,19+28+28,
  &F3_0_key_action[0], // first_action
  &F3_0_key_action[sizeof(F3_0_key_action)/sizeof(s_key_action) - 1], // last action
  0, // change_flag
  0,
  0,    
  0,
  0,
  0,
  0,
  0,
  0  // prev_next_func
};

void Control_Screen_F3_0(void)
{
int i;

  for (i = 0; i < opt_app.Sensoren.Drukverschil; i++)
    OptionSensor[i] = 1;
  for (i = opt_app.Sensoren.Drukverschil; i < MAX_SENSOREN; i++)
    OptionSensor[i] = 0;

  if (opt_app.Sensoren.Drukverschil == 0)
  {
    DummyScreen = 1;
    Control_Screen(&screen_F3_0, &screen_F3_0_default, 0, 1);
  }
  else
  {
    DummyScreen = 0;
    Control_Screen(&screen_F3_0, &screen_F3_0_default, 0, 3);
  }
}
