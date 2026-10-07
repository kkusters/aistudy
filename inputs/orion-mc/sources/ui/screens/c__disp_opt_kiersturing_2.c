// C__DISP_OPT_KIERSTURING_2.C

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
#include "ch_IO.h"
#include "ch_key.h"
#include "ch_kiersturing.h"
#include "ch_lcd_10.h"
#include "ch_lcd_14.h"
#include "ch_lcd_20.h"
#include "ch_lcd_bitmap.h"
#include "ch_lcd_hardware.h"
#include "ch_lcd_pixel.h"
#include "ch_main.h"
#include "ch_motor.h"
#include "ch_string.h" 
#include "ch_disp_opt_kiersturing_2.h"

static void Arrow_Aantal_Value(void);
static void Enter_Aantal_Value(void);
static void Arrow_CombiMatic_Value(void);
static void Enter_CombiMatic_Value(void);
static void Arrow_DualScreen_Value(void);
static void Enter_DualScreen_Value(void);
static void Arrow_MotorB_Value(void);
static void Enter_MotorB_Value(void);
static void Disp_Control_Master(void);
static void Arrow_End_Func(void);

static s_disp_cursor const disp_cursor_group_A  = { 225, 58, 189, 2, 18 };
static s_disp_cursor const disp_cursor_group_B  = { 225, 78, 189, 2, 18 };

static s_disp_agri_header const disp_header = { Disp_Draw_Agri_Header, FN6, EMPTY, &tekst_inst.Opties_2_doek_1_bed_10, HK_GEEN, 0, 3};
static void * const lcd_disp_header[] = { &disp_header, 0 };


static s_tekst_3 const tekst_A_10 = { 3, 15, SIZE_10, "A:" };
static s_tekst_3 const tekst_B_10 = { 3, 15, SIZE_10, "B:" };

static s_tekst_21 master_string;

static unsigned char NrDualScreens;

static int MotorA;
static int MotorB;
static TMotor *pMotorA;
static TMotor *pMotorB;

static const int MaxGroup = MAX_GROUP - 1;
static const int MaxMotor = MAX_MOTOR - 1;

//*****************************************************************************
// SCHERM OPBOUW
//*****************************************************************************
// Twee doeken op één draadbed - aantal
static s_disp_tekst const disp_aantal_str = { Disp_Draw_Tekst_L, 37, 22, &tekst_inst.Aantal_14 };
static s_disp_value const disp_aantal_val = { Disp_Draw_Value,  207, 75, (SIZE_14 | RECHTS), UCHAR, 0, &NrDualScreens };

static void * const lcd_aantal_disp[] = { &disp_twee_doek_een_bed_inst, &disp_aantal_str, &disp_aantal_val, 0 };

static s_key_value const key_aantal_val = { UCHAR, 1, &NrDualScreens, &uchar_0, &uchar_8 };
//-----------------------------------------------------------------------------------------------------------
// Twee doeken op één draadbed - Algemeen
static void const * const tekst_twee_doek_een_bed_array_14[] =
{
  &tekst_inst.Twee_doek_een_bed_1_14, &tekst_inst.Twee_doek_een_bed_2_14,
  &tekst_inst.Twee_doek_een_bed_3_14, &tekst_inst.Twee_doek_een_bed_4_14,
  &tekst_inst.Twee_doek_een_bed_5_14, &tekst_inst.Twee_doek_een_bed_6_14,
  &tekst_inst.Twee_doek_een_bed_7_14, &tekst_inst.Twee_doek_een_bed_8_14,
};
static s_disp_tekst_array const disp_twee_doek_een_bed_str = { Disp_Draw_Tekst_Array_L, 37, 22, &tekst_twee_doek_een_bed_array_14, UCHAR, &opt_app.DualScreen[0].Number, 8 };
//-----------------------------------------------------------------------------------------------------------
// Twee doeken op één draadbed - CombiMatic
static void const * const tekst_tegenover_achter_elkaar_10[] = { &tekst_inst.Tegenover_elkaar_10,  &tekst_inst.Achter_elkaar_10 };
static s_disp_tekst_array const disp_combimatic_val = { Disp_Draw_Tekst_Array_L, 37, 71, &tekst_tegenover_achter_elkaar_10, UCHAR, &opt_app.DualScreen[0].CombiMatic, 2 };

static void * const lcd_combimatic_disp[] = { &disp_twee_doek_een_bed_ico, &disp_twee_doek_een_bed_str, &disp_combimatic_val, 0 };

static s_key_value const key_combimatic_val = { UCHAR, 1, &opt_app.DualScreen[0].CombiMatic, &uchar_0, &uchar_1 };
//-----------------------------------------------------------------------------------------------------------
// Twee doeken op één draadbed - groepen
static s_disp_tekst const disp_A_str = { Disp_Draw_Tekst_L, 17, 51, &tekst_A_10 };
static s_disp_tekst const disp_B_str = { Disp_Draw_Tekst_L, 17, 71, &tekst_B_10 };
static s_disp_tekst_array const disp_groep_A_str = { Disp_Draw_Tekst_Array_L, 37, 51, &tekst_groep_10, UINT, &opt_app.DualScreen[0].GroupA, MAX_GROUP+1 };
static s_disp_tekst_array const disp_groep_B_str = { Disp_Draw_Tekst_Array_L, 37, 71, &tekst_groep_10, UINT, &opt_app.DualScreen[0].GroupB, MAX_GROUP+1 };

static void * const lcd_groepen_disp[] =
{
  &disp_twee_doek_een_bed_ico, &disp_twee_doek_een_bed_str, &disp_combimatic_a_b_ico,
  &disp_A_str, &disp_groep_A_str, 
  &disp_B_str, &disp_groep_B_str,
  0
};

static s_key_value const key_group_A_val = { INT, 2, &opt_app.DualScreen[0].GroupA, &int_m1, &MaxGroup };
static s_key_value const key_group_B_val = { INT, 2, &opt_app.DualScreen[0].GroupB, &int_m1, &MaxGroup };
//-----------------------------------------------------------------------------------------------------------
// Twee doeken op één draadbed - motoren
static s_disp_tekst_array const disp_motor_A_str = { Disp_Draw_Tekst_Array_L, 37, 51, &tekst_motor_10, UINT, &MotorA, MAX_MOTOR+1 };
static s_disp_tekst_array const disp_motor_B_str = { Disp_Draw_Tekst_Array_L, 37, 71, &tekst_motor_10, UINT, &MotorB, MAX_MOTOR+1 };

static void * const lcd_motoren_disp[] =
{
  &disp_twee_doek_een_bed_ico, &disp_twee_doek_een_bed_str, &disp_combimatic_a_b_ico,
  &disp_A_str, &disp_motor_A_str, 
  &disp_B_str, &disp_motor_B_str,
  0
};

static s_key_value const key_motor_A_val = { INT, 3, &MotorA, &int_m1, &MaxMotor };
static s_key_value const key_motor_B_val = { INT, 3, &MotorB, &int_m1, &MaxMotor };
//-----------------------------------------------------------------------------------------------------------
// Twee doeken op één draadbed - master
static s_disp_tekst const disp_master_str        = { Disp_Draw_Tekst_L, 37, 41, &tekst_inst.Master_14 };
static s_disp_tekst const disp_master_select_str = { Disp_Draw_Tekst_L, 37, 71, &master_string        };
static s_disp_func  const disp_master_func       = { Disp_Control_Func, Disp_Control_Master };

static void * const lcd_master_disp[] =
{
  &disp_twee_doek_een_bed_ico, &disp_twee_doek_een_bed_str,
  &disp_master_str, &disp_master_func, &disp_master_select_str,
  0
};

static s_key_value const key_master_val = { UCHAR, 1, &opt_app.DualScreen[0].Master, &uchar_0, &uchar_2 };
//-----------------------------------------------------------------------------------------------------------
// Relatief / Absoluut
static s_disp_tekst_array const disp_rel_abs_str = { Disp_Draw_Tekst_Array_L, 46, 75, &tekst_inst_rel_abs_14, UCHAR, &opt_app.DualScreen[0].Absolute, 2 };

static void * const lcd_rel_abs_disp[] = { &disp_twee_doek_een_bed_ico, &disp_twee_doek_een_bed_str, &disp_rel_abs_str, 0};

static s_key_value const key_rel_abs_val = { UCHAR, 2, &opt_app.DualScreen[0].Absolute, &uchar_0, &uchar_1 };
//-----------------------------------------------------------------------------------------------------------
// Standby regeling
static s_disp_tekst const disp_standby_regeling_str = { Disp_Draw_Tekst_L, 37, 41, &tekst_inst.Standby_regeling_14 };
static s_disp_bitmap_array const disp_standby_regeling_val = { Disp_Draw_Bitmap_Array, 190, 57, bmp_false_true, UCHAR, &opt_app.DualScreen[0].Standby, 2 };

static void * const lcd_standby_regeling_disp[] =
{
  &disp_twee_doek_een_bed_ico, &disp_twee_doek_een_bed_str,
  &disp_standby_regeling_str, &disp_standby_regeling_val,
  0
};

static s_key_value const key_standby_regeling_val = { UCHAR, 1, &opt_app.DualScreen[0].Standby, &uchar_0, &uchar_1 };
//-----------------------------------------------------------------------------

s_key_action const opt_kiersturing_2_key_action[] =
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
  //-----------------------------
  { // Aantal
    1,                                // nr
    0,                                // index
    (unsigned char *)&option_on,      // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_aantal_disp,                  // display
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
    lcd_aantal_disp,                  // display
    &disp_cursor_207_78_8,            // cursor
    &key_aantal_val,                  // *value
    Number_Value,                     // void (*number)(void); 
    Arrow_Aantal_Value,               // void (*arrow)(void); 
    Enter_Aantal_Value,               // void (*enter)(void);
  },
  //-----------------------------
  { // DualScreen[0] - CombiMatic
    10,                               // nr
    0,                                // index
    &opt_app.DualScreen[0].Enabled,   // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_combimatic_disp,              // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Option_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*number)(void); 
  },
  { // DualScreen[0] - CombiMatic
    10,                               // nr
    1,                                // index
    &opt_app.DualScreen[0].Enabled,   // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_combimatic_disp,              // display
    &disp_cursor_group_B,             // cursor
    &key_combimatic_val,              // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_CombiMatic_Value,           // void (*arrow)(void); 
    Enter_CombiMatic_Value,           // void (*enter)(void);
  },
  { // DualScreen[0]
    11,                               // nr
    0,                                // index
    &opt_app.DualScreen[0].Enabled,   // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_groepen_disp,                 // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Option_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  { // DualScreen[0] - GroupA
    11,                               // nr
    1,                                // index
    &opt_app.DualScreen[0].Enabled,   // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_groepen_disp,                 // display
    &disp_cursor_group_A,             // cursor
    &key_group_A_val,                 // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_DualScreen_Value,           // void (*arrow)(void); 
    Enter_DualScreen_Value,           // void (*arrow)(void); 
  },
  { // DualScreen[0] - GroupB
    11,                               // nr
    2,                                // index
    &opt_app.DualScreen[0].Enabled,   // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_groepen_disp,                 // display
    &disp_cursor_group_B,             // cursor
    &key_group_B_val,                 // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_DualScreen_Value,           // void (*arrow)(void); 
    Enter_DualScreen_Value,           // void (*arrow)(void); 
  },
  { // DualScreen[0] - MotorB
    11,                                // nr
    3,                                 // index
    &opt_app.DualScreen[0].Enabled,    // option
    (unsigned char *)&option_index_0,  // Optie index 
    lcd_motoren_disp,                  // display
    &disp_cursor_group_B,              // cursor
    &key_motor_B_val,                  // *value
    Dummy_Func,                        // void (*number)(void); 
    Arrow_MotorB_Value,                // void (*arrow)(void); 
    Enter_MotorB_Value,                // void (*arrow)(void); 
  },
  { // DualScreen[0] - Master
    12,                               // nr
    0,                                // index
    &opt_app.DualScreen[0].Enabled,   // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_master_disp,                  // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Option_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*number)(void); 
  },
  { // DualScreen[0] - Master
    12,                               // nr
    1,                                // index
    &opt_app.DualScreen[0].Enabled,   // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_master_disp,                  // display
    &disp_cursor_group_B,             // cursor
    &key_master_val,                  // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Scroll_Option_Value,        // void (*arrow)(void); 
    Increment_Func_Index_Enter_Value, // void (*enter)(void);
  },
  { // DualScreen[0] - Relatief / Absoluut
    13,                               // nr
    0,                                // index
    &opt_app.DualScreen[0].CombiMatic,// option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_rel_abs_disp,                 // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Option_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    13,                               // nr
    1,                                // index
    &opt_app.DualScreen[0].CombiMatic,// option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_rel_abs_disp,                 // display
    &disp_cursor_225_78_180,          // cursor
    &key_rel_abs_val,                 // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Scroll_Option_Value,        // void (*arrow)(void); 
    Increment_Func_Index_Enter_Value, // void (*enter)(void);
  },
  { // DualScreen[0] - Standby
    14,                               // nr
    0,                                // index
    &opt_app.DualScreen[0].Enabled,   // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_standby_regeling_disp,        // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Option_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*number)(void); 
  },
  { // DualScreen[0] - Standby
    14,                               // nr
    1,                                // index
    &opt_app.DualScreen[0].Enabled,   // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_standby_regeling_disp,        // display
    &disp_cursor_checkbox,            // cursor
    &key_standby_regeling_val,        // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Option_Value,               // void (*arrow)(void); 
    Increment_Func_Index_Enter_Value, // void (*enter)(void);
  },
  //-----------------------------
  { // DualScreen[1] - CombiMatic
    20,                               // nr
    0,                                // index
    &opt_app.DualScreen[1].Enabled,   // option
    (unsigned char *)&option_index_1, // Optie index 
    lcd_combimatic_disp,              // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Option_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*number)(void); 
  },
  { // DualScreen[1] - CombiMatic
    20,                               // nr
    1,                                // index
    &opt_app.DualScreen[1].Enabled,   // option
    (unsigned char *)&option_index_1, // Optie index 
    lcd_combimatic_disp,              // display
    &disp_cursor_group_B,             // cursor
    &key_combimatic_val,              // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_CombiMatic_Value,           // void (*arrow)(void); 
    Enter_CombiMatic_Value,           // void (*enter)(void);
  },
  { // DualScreen[1]
    21,                               // nr
    0,                                // index
    &opt_app.DualScreen[1].Enabled,   // option
    (unsigned char *)&option_index_1, // Optie index 
    lcd_groepen_disp,                 // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Option_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  { // DualScreen[1] - GroupA
    21,                               // nr
    1,                                // index
    &opt_app.DualScreen[1].Enabled,   // option
    (unsigned char *)&option_index_1, // Optie index 
    lcd_groepen_disp,                 // display
    &disp_cursor_group_A,             // cursor
    &key_group_A_val,                 // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_DualScreen_Value,           // void (*arrow)(void); 
    Enter_DualScreen_Value,           // void (*arrow)(void); 
  },
  { // DualScreen[1] - GroupB
    21,                               // nr
    2,                                // index
    &opt_app.DualScreen[1].Enabled,   // option
    (unsigned char *)&option_index_1, // Optie index 
    lcd_groepen_disp,                 // display
    &disp_cursor_group_B,             // cursor
    &key_group_B_val,                 // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_DualScreen_Value,           // void (*arrow)(void); 
    Enter_DualScreen_Value,           // void (*arrow)(void); 
  },
  { // DualScreen[1] - MotorB
    21,                                // nr
    3,                                 // index
    &opt_app.DualScreen[1].Enabled,    // option
    (unsigned char *)&option_index_1,  // Optie index 
    lcd_motoren_disp,                  // display
    &disp_cursor_group_B,              // cursor
    &key_motor_B_val,                  // *value
    Dummy_Func,                        // void (*number)(void); 
    Arrow_MotorB_Value,                // void (*arrow)(void); 
    Enter_MotorB_Value,                // void (*arrow)(void); 
  },
  { // DualScreen[1] - Master
    22,                               // nr
    0,                                // index
    &opt_app.DualScreen[1].Enabled,   // option
    (unsigned char *)&option_index_1, // Optie index 
    lcd_master_disp,                  // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Option_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*number)(void); 
  },
  { // DualScreen[1] - Master
    22,                               // nr
    1,                                // index
    &opt_app.DualScreen[1].Enabled,   // option
    (unsigned char *)&option_index_1, // Optie index 
    lcd_master_disp,                  // display
    &disp_cursor_group_B,             // cursor
    &key_master_val,                  // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Scroll_Option_Value,        // void (*arrow)(void); 
    Increment_Func_Index_Enter_Value, // void (*enter)(void);
  },
  { // DualScreen[1] - Relatief / Absoluut
    23,                               // nr
    0,                                // index
    &opt_app.DualScreen[1].CombiMatic,// option
    (unsigned char *)&option_index_1, // Optie index 
    lcd_rel_abs_disp,                 // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Option_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    23,                               // nr
    1,                                // index
    &opt_app.DualScreen[1].CombiMatic,// option
    (unsigned char *)&option_index_1, // Optie index 
    lcd_rel_abs_disp,                 // display
    &disp_cursor_225_78_180,          // cursor
    &key_rel_abs_val,                 // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Scroll_Option_Value,        // void (*arrow)(void); 
    Increment_Func_Index_Enter_Value, // void (*enter)(void);
  },
  { // DualScreen[1] - Standby
    24,                               // nr
    0,                                // index
    &opt_app.DualScreen[1].Enabled,   // option
    (unsigned char *)&option_index_1, // Optie index 
    lcd_standby_regeling_disp,        // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Option_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*number)(void); 
  },
  { // DualScreen[1] - Standby
    24,                               // nr
    1,                                // index
    &opt_app.DualScreen[1].Enabled,   // option
    (unsigned char *)&option_index_1, // Optie index 
    lcd_standby_regeling_disp,        // display
    &disp_cursor_checkbox,            // cursor
    &key_standby_regeling_val,        // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Option_Value,               // void (*arrow)(void); 
    Increment_Func_Index_Enter_Value, // void (*enter)(void);
  },
  //-----------------------------
  { // DualScreen[2] - CombiMatic
    30,                               // nr
    0,                                // index
    &opt_app.DualScreen[2].Enabled,   // option
    (unsigned char *)&option_index_2, // Optie index 
    lcd_combimatic_disp,              // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Option_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*number)(void); 
  },
  { // DualScreen[2] - CombiMatic
    30,                               // nr
    1,                                // index
    &opt_app.DualScreen[2].Enabled,   // option
    (unsigned char *)&option_index_2, // Optie index 
    lcd_combimatic_disp,              // display
    &disp_cursor_group_B,             // cursor
    &key_combimatic_val,              // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_CombiMatic_Value,           // void (*arrow)(void); 
    Enter_CombiMatic_Value,           // void (*enter)(void);
  },
  { // DualScreen[2]
    31,                               // nr
    0,                                // index
    &opt_app.DualScreen[2].Enabled,   // option
    (unsigned char *)&option_index_2, // Optie index 
    lcd_groepen_disp,                 // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Option_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  { // DualScreen[2] - GroupA
    31,                               // nr
    1,                                // index
    &opt_app.DualScreen[2].Enabled,   // option
    (unsigned char *)&option_index_2, // Optie index 
    lcd_groepen_disp,                 // display
    &disp_cursor_group_A,             // cursor
    &key_group_A_val,                 // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_DualScreen_Value,           // void (*arrow)(void); 
    Enter_DualScreen_Value,           // void (*arrow)(void); 
  },
  { // DualScreen[2] - GroupB
    31,                               // nr
    2,                                // index
    &opt_app.DualScreen[2].Enabled,   // option
    (unsigned char *)&option_index_2, // Optie index 
    lcd_groepen_disp,                 // display
    &disp_cursor_group_B,             // cursor
    &key_group_B_val,                 // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_DualScreen_Value,           // void (*arrow)(void); 
    Enter_DualScreen_Value,           // void (*arrow)(void); 
  },
  { // DualScreen[2] - MotorB
    31,                                // nr
    3,                                 // index
    &opt_app.DualScreen[2].Enabled,    // option
    (unsigned char *)&option_index_2,  // Optie index 
    lcd_motoren_disp,                  // display
    &disp_cursor_group_B,              // cursor
    &key_motor_B_val,                  // *value
    Dummy_Func,                        // void (*number)(void); 
    Arrow_MotorB_Value,                // void (*arrow)(void); 
    Enter_MotorB_Value,                // void (*arrow)(void); 
  },
  { // DualScreen[2] - Master
    32,                               // nr
    0,                                // index
    &opt_app.DualScreen[2].Enabled,   // option
    (unsigned char *)&option_index_2, // Optie index 
    lcd_master_disp,                  // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Option_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*number)(void); 
  },
  { // DualScreen[2] - Master
    32,                               // nr
    1,                                // index
    &opt_app.DualScreen[2].Enabled,   // option
    (unsigned char *)&option_index_2, // Optie index 
    lcd_master_disp,                  // display
    &disp_cursor_group_B,             // cursor
    &key_master_val,                  // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Scroll_Option_Value,        // void (*arrow)(void); 
    Increment_Func_Index_Enter_Value, // void (*enter)(void);
  },
  { // DualScreen[2] - Relatief / Absoluut
    33,                               // nr
    0,                                // index
    &opt_app.DualScreen[2].CombiMatic,// option
    (unsigned char *)&option_index_2, // Optie index 
    lcd_rel_abs_disp,                 // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Option_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    33,                               // nr
    1,                                // index
    &opt_app.DualScreen[2].CombiMatic,// option
    (unsigned char *)&option_index_2, // Optie index 
    lcd_rel_abs_disp,                 // display
    &disp_cursor_225_78_180,          // cursor
    &key_rel_abs_val,                 // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Scroll_Option_Value,        // void (*arrow)(void); 
    Increment_Func_Index_Enter_Value, // void (*enter)(void);
  },
  { // DualScreen[2] - Standby
    34,                               // nr
    0,                                // index
    &opt_app.DualScreen[2].Enabled,   // option
    (unsigned char *)&option_index_2, // Optie index 
    lcd_standby_regeling_disp,        // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Option_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*number)(void); 
  },
  { // DualScreen[2] - Standby
    34,                               // nr
    1,                                // index
    &opt_app.DualScreen[2].Enabled,   // option
    (unsigned char *)&option_index_2, // Optie index 
    lcd_standby_regeling_disp,        // display
    &disp_cursor_checkbox,            // cursor
    &key_standby_regeling_val,        // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Option_Value,               // void (*arrow)(void); 
    Increment_Func_Index_Enter_Value, // void (*enter)(void);
  },
  //-----------------------------
  { // DualScreen[3] - CombiMatic
    40,                               // nr
    0,                                // index
    &opt_app.DualScreen[3].Enabled,   // option
    (unsigned char *)&option_index_3, // Optie index 
    lcd_combimatic_disp,              // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Option_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*number)(void); 
  },
  { // DualScreen[3] - CombiMatic
    40,                               // nr
    1,                                // index
    &opt_app.DualScreen[3].Enabled,   // option
    (unsigned char *)&option_index_3, // Optie index 
    lcd_combimatic_disp,              // display
    &disp_cursor_group_B,             // cursor
    &key_combimatic_val,              // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_CombiMatic_Value,           // void (*arrow)(void); 
    Enter_CombiMatic_Value,           // void (*enter)(void);
  },
  { // DualScreen[3]
    41,                               // nr
    0,                                // index
    &opt_app.DualScreen[3].Enabled,   // option
    (unsigned char *)&option_index_3, // Optie index 
    lcd_groepen_disp,                 // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Option_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  { // DualScreen[3] - GroupA
    41,                               // nr
    1,                                // index
    &opt_app.DualScreen[3].Enabled,   // option
    (unsigned char *)&option_index_3, // Optie index 
    lcd_groepen_disp,                 // display
    &disp_cursor_group_A,             // cursor
    &key_group_A_val,                 // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_DualScreen_Value,           // void (*arrow)(void); 
    Enter_DualScreen_Value,           // void (*arrow)(void); 
  },
  { // DualScreen[3] - GroupB
    41,                               // nr
    2,                                // index
    &opt_app.DualScreen[3].Enabled,   // option
    (unsigned char *)&option_index_3, // Optie index 
    lcd_groepen_disp,                 // display
    &disp_cursor_group_B,             // cursor
    &key_group_B_val,                 // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_DualScreen_Value,           // void (*arrow)(void); 
    Enter_DualScreen_Value,           // void (*arrow)(void); 
  },
  { // DualScreen[3] - MotorB
    41,                                // nr
    3,                                 // index
    &opt_app.DualScreen[3].Enabled,    // option
    (unsigned char *)&option_index_4,  // Optie index 
    lcd_motoren_disp,                  // display
    &disp_cursor_group_B,              // cursor
    &key_motor_B_val,                  // *value
    Dummy_Func,                        // void (*number)(void); 
    Arrow_MotorB_Value,                // void (*arrow)(void); 
    Enter_MotorB_Value,                // void (*arrow)(void); 
  },
  { // DualScreen[3] - Master
    42,                               // nr
    0,                                // index
    &opt_app.DualScreen[3].Enabled,   // option
    (unsigned char *)&option_index_3, // Optie index 
    lcd_master_disp,                  // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Option_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*number)(void); 
  },
  { // DualScreen[3] - Master
    42,                               // nr
    1,                                // index
    &opt_app.DualScreen[3].Enabled,   // option
    (unsigned char *)&option_index_3, // Optie index 
    lcd_master_disp,                  // display
    &disp_cursor_group_B,             // cursor
    &key_master_val,                  // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Scroll_Option_Value,        // void (*arrow)(void); 
    Increment_Func_Index_Enter_Value, // void (*enter)(void);
  },
  { // DualScreen[3] - Relatief / Absoluut
    43,                               // nr
    0,                                // index
    &opt_app.DualScreen[3].CombiMatic,// option
    (unsigned char *)&option_index_3, // Optie index 
    lcd_rel_abs_disp,                 // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Option_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    43,                               // nr
    1,                                // index
    &opt_app.DualScreen[3].CombiMatic,// option
    (unsigned char *)&option_index_3, // Optie index 
    lcd_rel_abs_disp,                 // display
    &disp_cursor_225_78_180,          // cursor
    &key_rel_abs_val,                 // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Scroll_Option_Value,        // void (*arrow)(void); 
    Increment_Func_Index_Enter_Value, // void (*enter)(void);
  },
  { // DualScreen[3] - Standby
    44,                               // nr
    0,                                // index
    &opt_app.DualScreen[3].Enabled,   // option
    (unsigned char *)&option_index_3, // Optie index 
    lcd_standby_regeling_disp,        // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Option_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*number)(void); 
  },
  { // DualScreen[3] - Standby
    44,                               // nr
    1,                                // index
    &opt_app.DualScreen[3].Enabled,   // option
    (unsigned char *)&option_index_3, // Optie index 
    lcd_standby_regeling_disp,        // display
    &disp_cursor_checkbox,            // cursor
    &key_standby_regeling_val,        // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Option_Value,               // void (*arrow)(void); 
    Increment_Func_Index_Enter_Value, // void (*enter)(void);
  },
  //-----------------------------
  { // DualScreen[4] - CombiMatic
    50,                               // nr
    0,                                // index
    &opt_app.DualScreen[4].Enabled,   // option
    (unsigned char *)&option_index_4, // Optie index 
    lcd_combimatic_disp,              // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Option_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*number)(void); 
  },
  { // DualScreen[4] - CombiMatic
    50,                               // nr
    1,                                // index
    &opt_app.DualScreen[4].Enabled,   // option
    (unsigned char *)&option_index_4, // Optie index 
    lcd_combimatic_disp,              // display
    &disp_cursor_group_B,             // cursor
    &key_combimatic_val,              // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_CombiMatic_Value,           // void (*arrow)(void); 
    Enter_CombiMatic_Value,           // void (*enter)(void);
  },
  { // DualScreen[4]
    51,                               // nr
    0,                                // index
    &opt_app.DualScreen[4].Enabled,   // option
    (unsigned char *)&option_index_4, // Optie index 
    lcd_groepen_disp,                 // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Option_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  { // DualScreen[4] - GroupA
    51,                               // nr
    1,                                // index
    &opt_app.DualScreen[4].Enabled,   // option
    (unsigned char *)&option_index_4, // Optie index 
    lcd_groepen_disp,                 // display
    &disp_cursor_group_A,             // cursor
    &key_group_A_val,                 // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_DualScreen_Value,           // void (*arrow)(void); 
    Enter_DualScreen_Value,           // void (*arrow)(void); 
  },
  { // DualScreen[4] - GroupB
    51,                               // nr
    2,                                // index
    &opt_app.DualScreen[4].Enabled,   // option
    (unsigned char *)&option_index_4, // Optie index 
    lcd_groepen_disp,                 // display
    &disp_cursor_group_B,             // cursor
    &key_group_B_val,                 // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_DualScreen_Value,           // void (*arrow)(void); 
    Enter_DualScreen_Value,           // void (*arrow)(void); 
  },
  { // DualScreen[4] - MotorB
    51,                                // nr
    3,                                 // index
    &opt_app.DualScreen[4].Enabled,    // option
    (unsigned char *)&option_index_4,  // Optie index 
    lcd_motoren_disp,                  // display
    &disp_cursor_group_B,              // cursor
    &key_motor_B_val,                  // *value
    Dummy_Func,                        // void (*number)(void); 
    Arrow_MotorB_Value,                // void (*arrow)(void); 
    Enter_MotorB_Value,                // void (*arrow)(void); 
  },
  { // DualScreen[4] - Master
    52,                               // nr
    0,                                // index
    &opt_app.DualScreen[4].Enabled,   // option
    (unsigned char *)&option_index_4, // Optie index 
    lcd_master_disp,                  // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Option_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*number)(void); 
  },
  { // DualScreen[4] - Master
    52,                               // nr
    1,                                // index
    &opt_app.DualScreen[4].Enabled,   // option
    (unsigned char *)&option_index_4, // Optie index 
    lcd_master_disp,                  // display
    &disp_cursor_group_B,             // cursor
    &key_master_val,                  // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Scroll_Option_Value,        // void (*arrow)(void); 
    Increment_Func_Index_Enter_Value, // void (*enter)(void);
  },
  { // DualScreen[4] - Relatief / Absoluut
    53,                               // nr
    0,                                // index
    &opt_app.DualScreen[4].CombiMatic,// option
    (unsigned char *)&option_index_4, // Optie index 
    lcd_rel_abs_disp,                 // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Option_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    53,                               // nr
    1,                                // index
    &opt_app.DualScreen[4].CombiMatic,// option
    (unsigned char *)&option_index_4, // Optie index 
    lcd_rel_abs_disp,                 // display
    &disp_cursor_225_78_180,          // cursor
    &key_rel_abs_val,                 // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Scroll_Option_Value,        // void (*arrow)(void); 
    Increment_Func_Index_Enter_Value, // void (*enter)(void);
  },
  { // DualScreen[4] - Standby
    54,                               // nr
    0,                                // index
    &opt_app.DualScreen[4].Enabled,   // option
    (unsigned char *)&option_index_4, // Optie index 
    lcd_standby_regeling_disp,        // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Option_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*number)(void); 
  },
  { // DualScreen[4] - Standby
    54,                               // nr
    1,                                // index
    &opt_app.DualScreen[4].Enabled,   // option
    (unsigned char *)&option_index_4, // Optie index 
    lcd_standby_regeling_disp,        // display
    &disp_cursor_checkbox,            // cursor
    &key_standby_regeling_val,        // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Option_Value,               // void (*arrow)(void); 
    Increment_Func_Index_Enter_Value, // void (*enter)(void);
  },
  //-----------------------------
  { // DualScreen[5] - CombiMatic
    60,                               // nr
    0,                                // index
    &opt_app.DualScreen[5].Enabled,   // option
    (unsigned char *)&option_index_5, // Optie index 
    lcd_combimatic_disp,              // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Option_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*number)(void); 
  },
  { // DualScreen[5] - CombiMatic
    60,                               // nr
    1,                                // index
    &opt_app.DualScreen[5].Enabled,   // option
    (unsigned char *)&option_index_5, // Optie index 
    lcd_combimatic_disp,              // display
    &disp_cursor_group_B,             // cursor
    &key_combimatic_val,              // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_CombiMatic_Value,           // void (*arrow)(void); 
    Enter_CombiMatic_Value,           // void (*enter)(void);
  },
  { // DualScreen[5]
    61,                               // nr
    0,                                // index
    &opt_app.DualScreen[5].Enabled,   // option
    (unsigned char *)&option_index_5, // Optie index 
    lcd_groepen_disp,                 // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Option_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  { // DualScreen[5] - GroupA
    61,                               // nr
    1,                                // index
    &opt_app.DualScreen[5].Enabled,   // option
    (unsigned char *)&option_index_5, // Optie index 
    lcd_groepen_disp,                 // display
    &disp_cursor_group_A,             // cursor
    &key_group_A_val,                 // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_DualScreen_Value,           // void (*arrow)(void); 
    Enter_DualScreen_Value,           // void (*arrow)(void); 
  },
  { // DualScreen[5] - GroupB
    61,                               // nr
    2,                                // index
    &opt_app.DualScreen[5].Enabled,   // option
    (unsigned char *)&option_index_5, // Optie index 
    lcd_groepen_disp,                 // display
    &disp_cursor_group_B,             // cursor
    &key_group_B_val,                 // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_DualScreen_Value,           // void (*arrow)(void); 
    Enter_DualScreen_Value,           // void (*arrow)(void); 
  },
  { // DualScreen[5] - MotorB
    61,                                // nr
    3,                                 // index
    &opt_app.DualScreen[5].Enabled,    // option
    (unsigned char *)&option_index_5,  // Optie index 
    lcd_motoren_disp,                  // display
    &disp_cursor_group_B,              // cursor
    &key_motor_B_val,                  // *value
    Dummy_Func,                        // void (*number)(void); 
    Arrow_MotorB_Value,                // void (*arrow)(void); 
    Enter_MotorB_Value,                // void (*arrow)(void); 
  },
  { // DualScreen[5] - Master
    62,                               // nr
    0,                                // index
    &opt_app.DualScreen[5].Enabled,   // option
    (unsigned char *)&option_index_5, // Optie index 
    lcd_master_disp,                  // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Option_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*number)(void); 
  },
  { // DualScreen[5] - Master
    62,                               // nr
    1,                                // index
    &opt_app.DualScreen[5].Enabled,   // option
    (unsigned char *)&option_index_5, // Optie index 
    lcd_master_disp,                  // display
    &disp_cursor_group_B,             // cursor
    &key_master_val,                  // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Scroll_Option_Value,        // void (*arrow)(void); 
    Increment_Func_Index_Enter_Value, // void (*enter)(void);
  },
  { // DualScreen[5] - Relatief / Absoluut
    63,                               // nr
    0,                                // index
    &opt_app.DualScreen[5].CombiMatic,// option
    (unsigned char *)&option_index_5, // Optie index 
    lcd_rel_abs_disp,                 // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Option_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    63,                               // nr
    1,                                // index
    &opt_app.DualScreen[5].CombiMatic,// option
    (unsigned char *)&option_index_5, // Optie index 
    lcd_rel_abs_disp,                 // display
    &disp_cursor_225_78_180,          // cursor
    &key_rel_abs_val,                 // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Scroll_Option_Value,        // void (*arrow)(void); 
    Increment_Func_Index_Enter_Value, // void (*enter)(void);
  },
  { // DualScreen[5] - Standby
    64,                               // nr
    0,                                // index
    &opt_app.DualScreen[5].Enabled,   // option
    (unsigned char *)&option_index_5, // Optie index 
    lcd_standby_regeling_disp,        // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Option_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*number)(void); 
  },
  { // DualScreen[5] - Standby
    64,                               // nr
    1,                                // index
    &opt_app.DualScreen[5].Enabled,   // option
    (unsigned char *)&option_index_5, // Optie index 
    lcd_standby_regeling_disp,        // display
    &disp_cursor_checkbox,            // cursor
    &key_standby_regeling_val,        // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Option_Value,               // void (*arrow)(void); 
    Increment_Func_Index_Enter_Value, // void (*enter)(void);
  },
  //-----------------------------
  { // DualScreen[6] - CombiMatic
    70,                               // nr
    0,                                // index
    &opt_app.DualScreen[6].Enabled,   // option
    (unsigned char *)&option_index_6, // Optie index 
    lcd_combimatic_disp,              // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Option_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*number)(void); 
  },
  { // DualScreen[6] - CombiMatic
    70,                               // nr
    1,                                // index
    &opt_app.DualScreen[6].Enabled,   // option
    (unsigned char *)&option_index_6, // Optie index 
    lcd_combimatic_disp,              // display
    &disp_cursor_group_B,             // cursor
    &key_combimatic_val,              // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_CombiMatic_Value,           // void (*arrow)(void); 
    Enter_CombiMatic_Value,           // void (*enter)(void);
  },
  { // DualScreen[6]
    71,                               // nr
    0,                                // index
    &opt_app.DualScreen[6].Enabled,   // option
    (unsigned char *)&option_index_6, // Optie index 
    lcd_groepen_disp,                 // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Option_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  { // DualScreen[6] - GroupA
    71,                               // nr
    1,                                // index
    &opt_app.DualScreen[6].Enabled,   // option
    (unsigned char *)&option_index_6, // Optie index 
    lcd_groepen_disp,                 // display
    &disp_cursor_group_A,             // cursor
    &key_group_A_val,                 // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_DualScreen_Value,           // void (*arrow)(void); 
    Enter_DualScreen_Value,           // void (*arrow)(void); 
  },
  { // DualScreen[6] - GroupB
    71,                               // nr
    2,                                // index
    &opt_app.DualScreen[6].Enabled,   // option
    (unsigned char *)&option_index_6, // Optie index 
    lcd_groepen_disp,                 // display
    &disp_cursor_group_B,             // cursor
    &key_group_B_val,                 // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_DualScreen_Value,           // void (*arrow)(void); 
    Enter_DualScreen_Value,           // void (*arrow)(void); 
  },
  { // DualScreen[6] - MotorB
    71,                                // nr
    3,                                 // index
    &opt_app.DualScreen[6].Enabled,    // option
    (unsigned char *)&option_index_6,  // Optie index 
    lcd_motoren_disp,                  // display
    &disp_cursor_group_B,              // cursor
    &key_motor_B_val,                  // *value
    Dummy_Func,                        // void (*number)(void); 
    Arrow_MotorB_Value,                // void (*arrow)(void); 
    Enter_MotorB_Value,                // void (*arrow)(void); 
  },
  { // DualScreen[6] - Master
    72,                               // nr
    0,                                // index
    &opt_app.DualScreen[6].Enabled,   // option
    (unsigned char *)&option_index_6, // Optie index 
    lcd_master_disp,                  // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Option_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*number)(void); 
  },
  { // DualScreen[6] - Master
    72,                               // nr
    1,                                // index
    &opt_app.DualScreen[6].Enabled,   // option
    (unsigned char *)&option_index_6, // Optie index 
    lcd_master_disp,                  // display
    &disp_cursor_group_B,             // cursor
    &key_master_val,                  // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Scroll_Option_Value,        // void (*arrow)(void); 
    Increment_Func_Index_Enter_Value, // void (*enter)(void);
  },
  { // DualScreen[6] - Relatief / Absoluut
    73,                               // nr
    0,                                // index
    &opt_app.DualScreen[6].CombiMatic,// option
    (unsigned char *)&option_index_6, // Optie index 
    lcd_rel_abs_disp,                 // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Option_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    73,                               // nr
    1,                                // index
    &opt_app.DualScreen[6].CombiMatic,// option
    (unsigned char *)&option_index_6, // Optie index 
    lcd_rel_abs_disp,                 // display
    &disp_cursor_225_78_180,          // cursor
    &key_rel_abs_val,                 // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Scroll_Option_Value,        // void (*arrow)(void); 
    Increment_Func_Index_Enter_Value, // void (*enter)(void);
  },
  { // DualScreen[6] - Standby
    74,                               // nr
    0,                                // index
    &opt_app.DualScreen[6].Enabled,   // option
    (unsigned char *)&option_index_6, // Optie index 
    lcd_standby_regeling_disp,        // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Option_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*number)(void); 
  },
  { // DualScreen[6] - Standby
    74,                               // nr
    1,                                // index
    &opt_app.DualScreen[6].Enabled,   // option
    (unsigned char *)&option_index_6, // Optie index 
    lcd_standby_regeling_disp,        // display
    &disp_cursor_checkbox,            // cursor
    &key_standby_regeling_val,        // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Option_Value,               // void (*arrow)(void); 
    Increment_Func_Index_Enter_Value, // void (*enter)(void);
  },
  //-----------------------------
  { // DualScreen[7] - CombiMatic
    80,                               // nr
    0,                                // index
    &opt_app.DualScreen[7].Enabled,   // option
    (unsigned char *)&option_index_7, // Optie index 
    lcd_combimatic_disp,              // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Option_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*number)(void); 
  },
  { // DualScreen[7] - CombiMatic
    80,                               // nr
    1,                                // index
    &opt_app.DualScreen[7].Enabled,   // option
    (unsigned char *)&option_index_7, // Optie index 
    lcd_combimatic_disp,              // display
    &disp_cursor_group_B,             // cursor
    &key_combimatic_val,              // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_CombiMatic_Value,           // void (*arrow)(void); 
    Enter_CombiMatic_Value,           // void (*enter)(void);
  },
  { // DualScreen[7]
    81,                               // nr
    0,                                // index
    &opt_app.DualScreen[7].Enabled,   // option
    (unsigned char *)&option_index_7, // Optie index 
    lcd_groepen_disp,                 // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Option_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  { // DualScreen[7] - GroupA
    81,                               // nr
    1,                                // index
    &opt_app.DualScreen[7].Enabled,   // option
    (unsigned char *)&option_index_7, // Optie index 
    lcd_groepen_disp,                 // display
    &disp_cursor_group_A,             // cursor
    &key_group_A_val,                 // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_DualScreen_Value,           // void (*arrow)(void); 
    Enter_DualScreen_Value,           // void (*arrow)(void); 
  },
  { // DualScreen[7] - GroupB
    81,                               // nr
    2,                                // index
    &opt_app.DualScreen[7].Enabled,   // option
    (unsigned char *)&option_index_7, // Optie index 
    lcd_groepen_disp,                 // display
    &disp_cursor_group_B,             // cursor
    &key_group_B_val,                 // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_DualScreen_Value,           // void (*arrow)(void); 
    Enter_DualScreen_Value,           // void (*arrow)(void); 
  },
  { // DualScreen[7] - MotorB
    81,                                // nr
    3,                                 // index
    &opt_app.DualScreen[7].Enabled,    // option
    (unsigned char *)&option_index_7,  // Optie index 
    lcd_motoren_disp,                  // display
    &disp_cursor_group_B,              // cursor
    &key_motor_B_val,                  // *value
    Dummy_Func,                        // void (*number)(void); 
    Arrow_MotorB_Value,                // void (*arrow)(void); 
    Enter_MotorB_Value,                // void (*arrow)(void); 
  },
  { // DualScreen[7] - Master
    82,                               // nr
    0,                                // index
    &opt_app.DualScreen[7].Enabled,   // option
    (unsigned char *)&option_index_7, // Optie index 
    lcd_master_disp,                  // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Option_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*number)(void); 
  },
  { // DualScreen[7] - Master
    82,                               // nr
    1,                                // index
    &opt_app.DualScreen[7].Enabled,   // option
    (unsigned char *)&option_index_7, // Optie index 
    lcd_master_disp,                  // display
    &disp_cursor_group_B,             // cursor
    &key_master_val,                  // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Scroll_Option_Value,        // void (*arrow)(void); 
    Increment_Func_Index_Enter_Value, // void (*enter)(void);
  },
  { // DualScreen[7] - Relatief / Absoluut
    83,                               // nr
    0,                                // index
    &opt_app.DualScreen[7].CombiMatic,// option
    (unsigned char *)&option_index_7, // Optie index 
    lcd_rel_abs_disp,                 // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Option_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    83,                               // nr
    1,                                // index
    &opt_app.DualScreen[7].CombiMatic,// option
    (unsigned char *)&option_index_7, // Optie index 
    lcd_rel_abs_disp,                 // display
    &disp_cursor_225_78_180,          // cursor
    &key_rel_abs_val,                 // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Scroll_Option_Value,        // void (*arrow)(void); 
    Increment_Func_Index_Enter_Value, // void (*enter)(void);
  },
  { // DualScreen[7] - Standby
    84,                               // nr
    0,                                // index
    &opt_app.DualScreen[7].Enabled,   // option
    (unsigned char *)&option_index_7, // Optie index 
    lcd_standby_regeling_disp,        // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Option_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*number)(void); 
  },
  { // DualScreen[7] - Standby
    84,                               // nr
    1,                                // index
    &opt_app.DualScreen[7].Enabled,   // option
    (unsigned char *)&option_index_7, // Optie index 
    lcd_standby_regeling_disp,        // display
    &disp_cursor_checkbox,            // cursor
    &key_standby_regeling_val,        // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Option_Value,               // void (*arrow)(void); 
    Increment_Func_Index_Enter_Value, // void (*enter)(void);
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
s_screen screen_opt_kiersturing_2;
s_screen const screen_opt_kiersturing_2_default =
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
  &opt_kiersturing_2_key_action[0], // first_action
  &opt_kiersturing_2_key_action[sizeof(opt_kiersturing_2_key_action)/sizeof(s_key_action) - 1], // last action 
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

//*****************************************************************************
void Control_Screen_Option_Kiersturing_2(void)
{
int i;

  if (password_enabled & PASSWORD_ENABLED_OPT_MASK)
    start_flag = end_flag = 1;
  else
    start_flag = end_flag = 0;
  Control_Screen(&screen_opt_kiersturing_2, &screen_opt_kiersturing_2_default, 1, 1);

  NrDualScreens = 0;
  for (i = 0; i < MAX_SCREEN; i++)
  {
    if (opt_app.DualScreen[i].Enabled)
      NrDualScreens++;
  }
}

//-----------------------------------------------------------------------------
static int DualScreenLinkPossible(TMotor *pMotorA, TMotor *pMotorB)
{
  if ((pMotorA == NULL) || (pMotorB == NULL))
    return (0);
  if ((opt_app.Motor[pMotorA->Number].Enabled == 0) || (opt_app.Motor[pMotorB->Number].Enabled == 0))
    return (0);
  if ((opt_app.Motor[pMotorA->Number].IO.board_type != 0) && (opt_app.Motor[pMotorA->Number].IO.board_type == opt_app.Motor[pMotorB->Number].IO.board_type) && (opt_app.Motor[pMotorA->Number].IO.board_nr == opt_app.Motor[pMotorB->Number].IO.board_nr))
    return (1);
  else
    return (0);
}

static void CheckOptionsDualScreenMotors(void)
{
int i, index;
int A, B;
TMotor *pMotor, *pLink;

  // Check all motors
  for (i = 0; i < opt_app.NumberMotorgroups; i++)
  {
    if (opt_app.Motorgroup[i].ControlType == CONTROL_DUALSCREEN)
    {
      index = opt_app.Motorgroup[i].ControlIndex;
      A = opt_app.DualScreen[index].GroupA;
      B = opt_app.DualScreen[index].GroupB;
      pMotor = Motorgroup[i].FirstMotor;
      while (pMotor != NULL)
      {
        if (opt_app.Motor[pMotor->Number].Link != -1)
        {
          pLink = &Motor[opt_app.Motor[pMotor->Number].Link];
          if (opt_app.Motor[pLink->Number].Enabled == 0)
            opt_app.Motor[pMotor->Number].Link = -1;
          else if (((A != opt_app.Motor[pMotor->Number].GroupNumber - 1) || (B != opt_app.Motor[pLink->Number].GroupNumber - 1)) && ((A != opt_app.Motor[pLink->Number].GroupNumber - 1) || (B != opt_app.Motor[pMotor->Number].GroupNumber - 1)))
            opt_app.Motor[pMotor->Number].Link = -1;
        }
		else
		  pLink = NULL;

        if (opt_app.DualScreen[index].CombiMatic)
        {
          IO_Set_Motor_Control_Option(&opt_app.Motor[pMotor->Number].IO, OPTION_POSITION);
        }
        else
        {
          if (DualScreenLinkPossible(pMotor, pLink))
            IO_Set_Motor_Control_Option(&opt_app.Motor[pMotor->Number].IO, OPTION_DUALSCREEN);
		  else
            IO_Set_Motor_Control_Option(&opt_app.Motor[pMotor->Number].IO, OPTION_POSITION);
        }
        pMotor = pMotor->Next;
      }
    }
  }
}

void CheckOptionsDualScreen(void)
{
int i;
int A, B;

  // Clear all connections
  for (i = 0; i < opt_app.NumberMotorgroups; i++)
  {
    if (opt_app.Motorgroup[i].ControlType == CONTROL_DUALSCREEN) // && (opt_app.Motorgroup[i].Type == TYPE_DOEK))
    {
      opt_app.Motorgroup[i].ControlType  = CONTROL_NORMAL;
      opt_app.Motorgroup[i].ControlIndex = 0;
    }
  }
  // Rebuild all connections
  for (i = 0; i < MAX_SCREEN; i++)
  {
    if (opt_app.DualScreen[i].Enabled)
    {
      A = opt_app.DualScreen[i].GroupA;
      B = opt_app.DualScreen[i].GroupB;
      if (A != -1)
      {
        if (opt_app.Motorgroup[A].Enabled && (opt_app.Motorgroup[A].Type == TYPE_DOEK))
        {
          opt_app.Motorgroup[A].ControlType  = CONTROL_DUALSCREEN;
          opt_app.Motorgroup[A].ControlIndex = i;
        }
        else
        {
          opt_app.DualScreen[i].GroupA = -1;
          if (opt_app.DualScreen[i].Master == 1)
            opt_app.DualScreen[i].Master = 0;
        }
      }
      if (B != -1)
      {
        if (opt_app.Motorgroup[B].Enabled && (opt_app.Motorgroup[B].Type == TYPE_DOEK))
        {
          opt_app.Motorgroup[B].ControlType  = CONTROL_DUALSCREEN;
          opt_app.Motorgroup[B].ControlIndex = i;
        }
        else
        {
          opt_app.DualScreen[i].GroupB = -1;
          if (opt_app.DualScreen[i].Master == 2)
            opt_app.DualScreen[i].Master = 0;
        }
      }
    }
    else
    {
      opt_app.DualScreen[i]    = default_opt_app.DualScreen[i];
      setp_alg.DualScreen[i]   = default_setp_alg.DualScreen[i];
      val_hr_alg.DualScreen[i] = default_val_hr_alg.DualScreen[i];
    }
  }
  CheckOptionsDualScreenMotors();
}

//-----------------------------------------------------------------------------
static void Arrow_Aantal_Value(void)
{
int i;

  Arrow_Option_Value();
  if (screen_ptr->index == 0)
  {
    for (i = 0; i < NrDualScreens; i++)
      opt_app.DualScreen[i].Enabled = 1;
    for (i = NrDualScreens; i < MAX_SCREEN; i++)
      opt_app.DualScreen[i] = default_opt_app.DualScreen[i];
    CheckOptions();
    Refresh_Screen_Nr_Aantal();
  }
}

static void Enter_Aantal_Value(void)
{
int i;

  Increment_Func_Index_Enter_Option_Value();
  if (screen_ptr->index == 0)
  {
    for (i = 0; i < NrDualScreens; i++)
      opt_app.DualScreen[i].Enabled = 1;
    for (i = NrDualScreens; i < MAX_SCREEN; i++)
      opt_app.DualScreen[i] = default_opt_app.DualScreen[i];
    CheckOptions();
    Refresh_Screen_Nr_Aantal();
  }
}

//-----------------------------------------------------------------------------
static void Arrow_CombiMatic_Value(void)
{
  Arrow_Scroll_Option_Value();
  Refresh_Screen_Nr_Aantal();
}

static void Enter_CombiMatic_Value(void)
{
  Increment_Func_Index_Enter_Option_Value();
  Refresh_Screen_Nr_Aantal();
}

//-----------------------------------------------------------------------------
static int GroupOK(int nr)
{
int i;

  if (nr == -1)
    return (1);
  if (opt_app.Motorgroup[nr].Type != TYPE_DOEK) // Alleen mogelijk bij doek sturing
    return (0);
  for (i = 0; i < MAX_SCREEN; i++)
  {
    if (opt_app.DualScreen[i].Enabled)
    {
      if (nr == opt_app.DualScreen[i].GroupA)
        return (0);
      if (nr == opt_app.DualScreen[i].GroupB)
        return (0);
    }
  }
  return (1);
}

//-----------------------------------------------------------------------------
static void Arrow_DualScreen_Value(void)
{
int screen_index = screen_ptr->index;
int nr;

  nr = (screen_ptr->nr / 10) - 1;
  switch (key)
  {
    case UP:
      do
      {
        if (screen_ptr->value < (opt_app.NumberMotorgroups - 1))
          screen_ptr->value++;
        else
          screen_ptr->value = -1;
      } while (!GroupOK(screen_ptr->value));
      Put_Value();
      option_change_flag = 1;
      break;
    case DOWN:
      do
      {
        if (screen_ptr->value > -1)
          screen_ptr->value--;
        else
          screen_ptr->value = opt_app.NumberMotorgroups - 1;
      } while (!GroupOK(screen_ptr->value));
      Put_Value();
      option_change_flag = 1;
      break;
    case LEFT:
      Arrow_Left_Value();
      if (screen_ptr->index == 0)
        CheckOptions();
      break;
    case RIGHT:
      if (screen_index == 2)
      {
        CheckOptions();
        if ((opt_app.DualScreen[nr].GroupA == -1) || (opt_app.DualScreen[nr].GroupB == -1))
        {
          Decrement_Func_Index();
          Decrement_Func_Index();
        }
        else
        {
          pMotorA = Motorgroup[opt_app.DualScreen[nr].GroupA].FirstMotor;
          if (pMotorA != NULL)
          {
            MotorA  = pMotorA->Number;
            pMotorB = pMotorA->Link;
            if (pMotorB != NULL)
              MotorB = pMotorB->Number;
            else
              MotorB = -1;
            Increment_Func_Index();
          }
          else
          {
            Decrement_Func_Index();
            Decrement_Func_Index();
          }
        }
      }
      else
        Increment_Func_Index();
      break;
  }
}

static void Enter_DualScreen_Value(void)
{
int screen_index = screen_ptr->index;
int nr;

  nr = (screen_ptr->nr / 10) - 1;
  if (Enter_Value())
  {
    if (screen_index == 2)
    {
      CheckOptions();
      option_change_flag = 1;
      if ((opt_app.DualScreen[nr].GroupA == -1) || (opt_app.DualScreen[nr].GroupB == -1))
      {
        Decrement_Func_Index();
        Decrement_Func_Index();
      }
      else
      {
        pMotorA = Motorgroup[opt_app.DualScreen[nr].GroupA].FirstMotor;
        if (pMotorA != NULL)
        {
          MotorA  = pMotorA->Number;
          pMotorB = pMotorA->Link;
          if (pMotorB != NULL)
            MotorB = pMotorB->Number;
          else
            MotorB = -1;
          Increment_Func_Index();
        }
        else
        {
          Decrement_Func_Index();
          Decrement_Func_Index();
        }
      }
    }
    else
      Increment_Func_Index();
  }
}

//-----------------------------------------------------------------------------
static unsigned char MotorOK(void)
{
TMotor *pMotor;
int nr;

  if (pMotorB == NULL)
    return (1);
  
  nr = (screen_ptr->nr / 10) - 1;
  pMotor = Motorgroup[opt_app.DualScreen[nr].GroupA].FirstMotor;
  while ((pMotor != pMotorA) && (pMotor != NULL))
  {
    if (pMotorB == pMotor->Link)
      return (0);
    pMotor = pMotor->Next;
  }
  return (1);
}

//-----------------------------------------------------------------------------
static void CheckForDoubleLink(void)
{
TMotor *pMotor;
int nr;

  nr = (screen_ptr->nr / 10) - 1;
  pMotor = pMotorA->Next;
  while (pMotor != NULL)
  {
    if (pMotor->Link == pMotorA->Link)
    {
      opt_app.Motor[pMotor->Number].Link = -1;
      pMotor->Link = NULL;
    }
    pMotor = pMotor->Next;
  }
}

//-----------------------------------------------------------------------------
static void Arrow_MotorB_Value(void)
{
int nr;

  nr = (screen_ptr->nr / 10) - 1;
  switch (key)
  {
    case UP:
      do
      {
        if (pMotorB == NULL)
          pMotorB = Motorgroup[opt_app.DualScreen[nr].GroupB].FirstMotor;
        else
          pMotorB = pMotorB->Next;
      } while (!MotorOK());
      if (pMotorB != NULL)
        MotorB = pMotorB->Number;
      else
        MotorB = -1;
      option_change_flag = 1;
      break;
    case DOWN:
      do
      {
        if (pMotorB == NULL)
          pMotorB = Motorgroup[opt_app.DualScreen[nr].GroupB].LastMotor;
        else
          pMotorB = pMotorB->Prev;
      } while (!MotorOK());
      if (pMotorB != NULL)
        MotorB = pMotorB->Number;
      else
        MotorB = -1;
      option_change_flag = 1;
      break;
    case LEFT:
      if (pMotorB != NULL)
      {
        opt_app.Motor[pMotorA->Number].Link = MotorB;
        opt_app.Motor[pMotorB->Number].Link = MotorA;
        pMotorA->Link = pMotorB;
        pMotorB->Link = pMotorA;
        CheckForDoubleLink();
      }
      else
      {
        opt_app.Motor[pMotorA->Number].Link = -1;
        pMotorA->Link = NULL;
      }
      pMotorA = pMotorA->Prev;
      if (pMotorA == NULL)
      {
        CheckOptions();
        Arrow_Left_Value();
      }
      else
      {
        MotorA  = pMotorA->Number;
        pMotorB = pMotorA->Link;
        if (pMotorB != NULL)
          MotorB = pMotorB->Number;
        else
          MotorB = -1;
      }
      break;
    case RIGHT:
      if (pMotorB != NULL)
      {
        opt_app.Motor[pMotorA->Number].Link = MotorB;
        opt_app.Motor[pMotorB->Number].Link = MotorA;
        pMotorA->Link = pMotorB;
        pMotorB->Link = pMotorA;
        CheckForDoubleLink();
      }
      else
      {
        opt_app.Motor[pMotorA->Number].Link = -1;
        pMotorA->Link = NULL;
      }
      pMotorA = pMotorA->Next;
      if (pMotorA == NULL)
      {
        CheckOptions();
        Increment_Func_Index();
      }
      else
      {
        MotorA  = pMotorA->Number;
        pMotorB = pMotorA->Link;
        if (pMotorB != NULL)
          MotorB = pMotorB->Number;
        else
          MotorB = -1;
      }
      break;
  }
}

static void Enter_MotorB_Value(void)
{
int nr;

  nr = (screen_ptr->nr / 10) - 1;
  if (pMotorB != NULL)
  {
    opt_app.Motor[pMotorA->Number].Link = MotorB;
    opt_app.Motor[pMotorB->Number].Link = MotorA;
    pMotorA->Link = pMotorB;
    pMotorB->Link = pMotorA;
    CheckForDoubleLink();
  }
  else
  {
    opt_app.Motor[pMotorA->Number].Link = -1;
    pMotorA->Link = NULL;
  }
  pMotorA = pMotorA->Next;
  if (pMotorA == NULL)
  {
    CheckOptions();
    Increment_Func_Index();
  }
  else
  {
    MotorA  = pMotorA->Number;
    pMotorB = pMotorA->Link;
    if (pMotorB != NULL)
      MotorB = pMotorB->Number;
    else
      MotorB = -1;
  }
}

//-----------------------------------------------------------------------------
static void Disp_Control_Master(void)
{
int nr;

  nr = (screen_ptr->nr / 10) - 1;
  switch (opt_app.DualScreen[nr].Master)
  {
    case 1: // GroupA is master
      if ((opt_app.DualScreen[nr].GroupA >= 0) && (opt_app.DualScreen[nr].GroupA < MAX_GROUP))
        master_string = *((s_tekst_21 *)tekst_groep_10[opt_app.DualScreen[nr].GroupA]);
      else
      {
        master_string = *((s_tekst_21 *)&tekst_inst.Geen_10);
        opt_app.DualScreen[nr].Master = 0;
      }
      break;
    case 2: // GroupB is master
      if ((opt_app.DualScreen[nr].GroupB >= 0) && (opt_app.DualScreen[nr].GroupB < MAX_GROUP))
        master_string = *((s_tekst_21 *)tekst_groep_10[opt_app.DualScreen[nr].GroupB]);
      else
      {
        master_string = *((s_tekst_21 *)&tekst_inst.Geen_10);
        opt_app.DualScreen[nr].Master = 0;
      }
      break;
    default: // Geen master
      master_string = *((s_tekst_21 *)&tekst_inst.Geen_10);
      break;
  }
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
