// C__DISP_OPT_CABRIOKAS_2.C

#include <string.h>

#include "ch_define.h"

#include "ch_alg.h"
#include "ch_alarm.h"
#include "ch_cabriokas.h"
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
#include "ch_lcd_10.h"
#include "ch_lcd_14.h"
#include "ch_lcd_20.h"
#include "ch_lcd_bitmap.h"
#include "ch_lcd_hardware.h"
#include "ch_lcd_pixel.h"
#include "ch_main.h"
#include "ch_motor.h"
#include "ch_string.h" 
#include "ch_disp_opt_cabriokas_2.h"

static void Arrow_Aantal_Value(void);
static void Enter_Aantal_Value(void);
static void Arrow_Type_Value(void);
static void Enter_Type_Value(void);
static void Arrow_Group_Value(void);
static void Enter_Group_Value(void);
static void Arrow_MotorB_Value(void);
static void Enter_MotorB_Value(void);
static void Arrow_End_Func(void);

static s_disp_cursor const disp_cursor_group_A  = { 225, 58, 189, 2, 18 };
static s_disp_cursor const disp_cursor_group_B  = { 225, 78, 189, 2, 18 };

//-----------------------------------------------------------------------------------------------------------
static s_tekst_3 const tekst_A_10 = { 3, 15, SIZE_10, "A:" };
static s_tekst_3 const tekst_B_10 = { 3, 15, SIZE_10, "B:" };

static unsigned char Aantal;

static int MotorA;
static int MotorB;
static TMotor *pMotorA;
static TMotor *pMotorB;

static const int MaxGroup = MAX_GROUP - 1;
static const int MaxMotor = MAX_MOTOR - 1;

//*****************************************************************************
// SCHERM OPBOUW
//*****************************************************************************
//-----------------------------------------------------------------------------------------------------------
// Header
static s_disp_agri_header const disp_header = { Disp_Draw_Agri_Header, FN6, EMPTY, &tekst_inst.Opties_Cabriokas_10, HK_GEEN, 0, 3};

static void * const lcd_disp_header[] = { &disp_header, 0 };
//-----------------------------------------------------------------------------------------------------------
// Aantal cabriokassen
static s_disp_tekst const disp_aantal_str = { Disp_Draw_Tekst_L, 37, 22, &tekst_inst.Aantal_14 };
static s_disp_value const disp_aantal_val = { Disp_Draw_Value,  207, 75, (SIZE_14 | RECHTS), UCHAR, 0, &Aantal };

static void * const lcd_aantal_disp[] = { &disp_cabriokas_ico, &disp_aantal_str, &disp_aantal_val, 0 };

static s_key_value const key_aantal_val = { UCHAR, 1, &Aantal, &uchar_0, &uchar_8 };
//-----------------------------------------------------------------------------------------------------------
// Cabriokas - Tekstarrays
static void const * const tekst_cabriokas_array_14[] =
{
  &tekst_inst.Cabriokas_1_14, &tekst_inst.Cabriokas_2_14,
  &tekst_inst.Cabriokas_3_14, &tekst_inst.Cabriokas_4_14,
  &tekst_inst.Cabriokas_5_14, &tekst_inst.Cabriokas_6_14,
  &tekst_inst.Cabriokas_7_14, &tekst_inst.Cabriokas_8_14,
};
static s_disp_tekst_array const disp_cabriokas_str = { Disp_Draw_Tekst_Array_L, 37, 22, &tekst_cabriokas_array_14, UCHAR, &Cabriokas[0].Number, 8 };
//-----------------------------------------------------------------------------------------------------------
// Cabriokas - Type
static void const * const tekst_type_array_10[] = { &tekst_inst.Voor_naloop_10,  &tekst_inst.Gelijkloop_10 };
static s_disp_tekst_array const disp_cabriokas_val = { Disp_Draw_Tekst_Array_L, 37, 71, &tekst_type_array_10, UCHAR, &opt_app.Cabriokas[0].Type, 2 };

static void * const lcd_cabriokas_disp[] = { &disp_cabriokas_ico, &disp_cabriokas_str, &disp_cabriokas_val, 0 };

static s_key_value const key_cabriokas_val = { UCHAR, 1, &opt_app.Cabriokas[0].Type, &uchar_0, &uchar_1 };
//-----------------------------------------------------------------------------------------------------------
// Cabriokas - groepen
static void const * const tekst_A_array_10[] = { &tekst.V_10, &tekst_A_10 };
static void const * const tekst_B_array_10[] = { &tekst.N_10, &tekst_B_10 };
static s_disp_tekst_array const disp_A_str = { Disp_Draw_Tekst_Array_L, 17, 51, &tekst_A_array_10, UCHAR, &opt_app.Cabriokas[0].Type, 2 };
static s_disp_tekst_array const disp_B_str = { Disp_Draw_Tekst_Array_L, 17, 71, &tekst_B_array_10, UCHAR, &opt_app.Cabriokas[0].Type, 2 };
static s_disp_tekst_array const disp_groep_A_str = { Disp_Draw_Tekst_Array_L, 37, 51, &tekst_groep_10, UINT, &opt_app.Cabriokas[0].GroupA, MAX_GROUP+1 };
static s_disp_tekst_array const disp_groep_B_str = { Disp_Draw_Tekst_Array_L, 37, 71, &tekst_groep_10, UINT, &opt_app.Cabriokas[0].GroupB, MAX_GROUP+1 };

static void * const lcd_groepen_disp[] =
{
  &disp_cabriokas_ico, &disp_cabriokas_str,
  &disp_A_str, &disp_groep_A_str, 
  &disp_B_str, &disp_groep_B_str,
  0
};

static s_key_value const key_group_A_val = { INT, 2, &opt_app.Cabriokas[0].GroupA, &int_m1, &MaxGroup };
static s_key_value const key_group_B_val = { INT, 2, &opt_app.Cabriokas[0].GroupB, &int_m1, &MaxGroup };
//-----------------------------------------------------------------------------------------------------------
// Cabriokas - motoren
static s_disp_tekst_array const disp_motor_A_str = { Disp_Draw_Tekst_Array_L, 37, 51, &tekst_motor_10, UINT, &MotorA, MAX_MOTOR+1 };
static s_disp_tekst_array const disp_motor_B_str = { Disp_Draw_Tekst_Array_L, 37, 71, &tekst_motor_10, UINT, &MotorB, MAX_MOTOR+1 };

static void * const lcd_motoren_disp[] =
{
  &disp_cabriokas_ico, &disp_cabriokas_str,
  &disp_A_str, &disp_motor_A_str, 
  &disp_B_str, &disp_motor_B_str,
  0
};

static s_key_value const key_motor_A_val = { INT, 3, &MotorA, &int_m1, &MaxMotor };
static s_key_value const key_motor_B_val = { INT, 3, &MotorB, &int_m1, &MaxMotor };
//-----------------------------------------------------------------------------------------------------------
// Voorloop / Gelijkloop / Hysterese
static void const * const tekst_voorloop_array_14[] = { &tekst_inst.Voorloop_14, &tekst_inst.Gelijkloop_14 };
static s_disp_tekst_array const disp_voorloop_str = { Disp_Draw_Tekst_Array_L, 37, 22, &tekst_voorloop_array_14, UCHAR, &opt_app.Cabriokas[0].Type, 2 };
static s_disp_tekst const disp_hysterese_str = { Disp_Draw_Tekst_L,  37, 22, &tekst_inst.Hysteresis_14 };
static s_disp_value const disp_voorloop_val  = { Disp_Draw_Value,   207, 75, (SIZE_14 | RECHTS), UCHAR, 1, &opt_app.Cabriokas[0].Voorloop };
static s_disp_value const disp_hysterese_val = { Disp_Draw_Value,   207, 75, (SIZE_14 | RECHTS), UCHAR, 1, &opt_app.Cabriokas[0].Hysterese };
static s_disp_tekst const disp_14_perc       = { Disp_Draw_Tekst_L, 210, 68, &tekst.perc_7 };

static void * const lcd_voorloop_disp[]  = { &disp_cabriokas_ico, &disp_voorloop_str,  &disp_voorloop_val,  &disp_14_perc, 0 };
static void * const lcd_hysterese_disp[] = { &disp_cabriokas_ico, &disp_hysterese_str, &disp_hysterese_val, &disp_14_perc, 0 };

static s_key_value const key_voorloop_val  = { UCHAR, 3, &opt_app.Cabriokas[0].Voorloop,  &uchar_3, &uchar_200 };
static s_key_value const key_hysterese_val = { UCHAR, 3, &opt_app.Cabriokas[0].Hysterese, &uchar_3, &uchar_200 };
//-----------------------------------------------------------------------------------------------------------


s_key_action const opt_cabriokas_2_key_action[] =
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
  { // Cabriokas[0] - Type
    10,                               // nr
    0,                                // index
    &opt_app.Cabriokas[0].Enabled,    // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_cabriokas_disp,               // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Option_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*number)(void); 
  },
  { // Cabriokas[0] - Type
    10,                               // nr
    1,                                // index
    &opt_app.Cabriokas[0].Enabled,    // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_cabriokas_disp,               // display
    &disp_cursor_group_B,             // cursor
    &key_cabriokas_val,               // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Type_Value,                 // void (*arrow)(void); 
    Enter_Type_Value,                 // void (*enter)(void);
  },
  { // Cabriokas[0]
    11,                               // nr
    0,                                // index
    &opt_app.Cabriokas[0].Enabled,    // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_groepen_disp,                 // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Option_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  { // Cabriokas[0] - GroupA
    11,                               // nr
    1,                                // index
    &opt_app.Cabriokas[0].Enabled,    // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_groepen_disp,                 // display
    &disp_cursor_group_A,             // cursor
    &key_group_A_val,                 // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Group_Value,                // void (*arrow)(void); 
    Enter_Group_Value,                // void (*arrow)(void); 
  },
  { // Cabriokas[0] - GroupB
    11,                               // nr
    2,                                // index
    &opt_app.Cabriokas[0].Enabled,    // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_groepen_disp,                 // display
    &disp_cursor_group_B,             // cursor
    &key_group_B_val,                 // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Group_Value,                // void (*arrow)(void); 
    Enter_Group_Value,                // void (*arrow)(void); 
  },
  { // Cabriokas[0] - MotorB
    11,                                // nr
    3,                                 // index
    &opt_app.Cabriokas[0].Enabled,     // option
    (unsigned char *)&option_index_0,  // Optie index 
    lcd_motoren_disp,                  // display
    &disp_cursor_group_B,              // cursor
    &key_motor_B_val,                  // *value
    Dummy_Func,                        // void (*number)(void); 
    Arrow_MotorB_Value,                // void (*arrow)(void); 
    Enter_MotorB_Value,                // void (*arrow)(void); 
  },
  { // Voorloop [%]
    12,                               // nr
    0,                                // index
    &opt_app.Cabriokas[0].Enabled,    // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_voorloop_disp,                // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Option_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    12,                               // nr
    1,                                // index
    &opt_app.Cabriokas[0].Enabled,    // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_voorloop_disp,                // display
    &disp_cursor_207_78_8,            // cursor
    &key_voorloop_val,                // *value
    Number_Value,                     // void (*number)(void); 
    Arrow_Option_Value,               // void (*arrow)(void); 
    Increment_Func_Index_Enter_Option_Value, // void (*enter)(void);
  },
  { // Hysterese [%]
    13,                               // nr
    0,                                // index
    &opt_app.Cabriokas[0].Enabled,    // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_hysterese_disp,               // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Option_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    13,                               // nr
    1,                                // index
    &opt_app.Cabriokas[0].Enabled,    // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_hysterese_disp,               // display
    &disp_cursor_207_78_8,            // cursor
    &key_hysterese_val,               // *value
    Number_Value,                     // void (*number)(void); 
    Arrow_Option_Value,               // void (*arrow)(void); 
    Increment_Func_Index_Enter_Option_Value, // void (*enter)(void);
  },
  //-----------------------------
  { // Cabriokas[1] - Type
    20,                               // nr
    0,                                // index
    &opt_app.Cabriokas[1].Enabled,    // option
    (unsigned char *)&option_index_1, // Optie index 
    lcd_cabriokas_disp,               // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Option_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*number)(void); 
  },
  { // Cabriokas[1] - Type
    20,                               // nr
    1,                                // index
    &opt_app.Cabriokas[1].Enabled,    // option
    (unsigned char *)&option_index_1, // Optie index 
    lcd_cabriokas_disp,               // display
    &disp_cursor_group_B,             // cursor
    &key_cabriokas_val,               // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Type_Value,                 // void (*arrow)(void); 
    Enter_Type_Value,                 // void (*enter)(void);
  },
  { // Cabriokas[1]
    21,                               // nr
    0,                                // index
    &opt_app.Cabriokas[1].Enabled,    // option
    (unsigned char *)&option_index_1, // Optie index 
    lcd_groepen_disp,                 // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Option_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  { // Cabriokas[1] - GroupA
    21,                               // nr
    1,                                // index
    &opt_app.Cabriokas[1].Enabled,    // option
    (unsigned char *)&option_index_1, // Optie index 
    lcd_groepen_disp,                 // display
    &disp_cursor_group_A,             // cursor
    &key_group_A_val,                 // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Group_Value,                // void (*arrow)(void); 
    Enter_Group_Value,                // void (*arrow)(void); 
  },
  { // Cabriokas[1] - GroupB
    21,                               // nr
    2,                                // index
    &opt_app.Cabriokas[1].Enabled,    // option
    (unsigned char *)&option_index_1, // Optie index 
    lcd_groepen_disp,                 // display
    &disp_cursor_group_B,             // cursor
    &key_group_B_val,                 // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Group_Value,                // void (*arrow)(void); 
    Enter_Group_Value,                // void (*arrow)(void); 
  },
  { // Cabriokas[1] - MotorB
    21,                                // nr
    3,                                 // index
    &opt_app.Cabriokas[1].Enabled,     // option
    (unsigned char *)&option_index_1,  // Optie index 
    lcd_motoren_disp,                  // display
    &disp_cursor_group_B,              // cursor
    &key_motor_B_val,                  // *value
    Dummy_Func,                        // void (*number)(void); 
    Arrow_MotorB_Value,                // void (*arrow)(void); 
    Enter_MotorB_Value,                // void (*arrow)(void); 
  },
  { // Voorloop [%]
    22,                               // nr
    0,                                // index
    &opt_app.Cabriokas[1].Enabled,    // option
    (unsigned char *)&option_index_1, // Optie index 
    lcd_voorloop_disp,                // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Option_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    22,                               // nr
    1,                                // index
    &opt_app.Cabriokas[1].Enabled,    // option
    (unsigned char *)&option_index_1, // Optie index 
    lcd_voorloop_disp,                // display
    &disp_cursor_207_78_8,            // cursor
    &key_voorloop_val,                // *value
    Number_Value,                     // void (*number)(void); 
    Arrow_Option_Value,               // void (*arrow)(void); 
    Increment_Func_Index_Enter_Option_Value, // void (*enter)(void);
  },
  { // Hysterese [%]
    23,                               // nr
    0,                                // index
    &opt_app.Cabriokas[1].Enabled,    // option
    (unsigned char *)&option_index_1, // Optie index 
    lcd_hysterese_disp,               // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Option_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    23,                               // nr
    1,                                // index
    &opt_app.Cabriokas[1].Enabled,    // option
    (unsigned char *)&option_index_1, // Optie index 
    lcd_hysterese_disp,               // display
    &disp_cursor_207_78_8,            // cursor
    &key_hysterese_val,               // *value
    Number_Value,                     // void (*number)(void); 
    Arrow_Option_Value,               // void (*arrow)(void); 
    Increment_Func_Index_Enter_Option_Value, // void (*enter)(void);
  },
  //-----------------------------
  { // Cabriokas[2] - Type
    30,                               // nr
    0,                                // index
    &opt_app.Cabriokas[2].Enabled,    // option
    (unsigned char *)&option_index_2, // Optie index 
    lcd_cabriokas_disp,               // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Option_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*number)(void); 
  },
  { // Cabriokas[2] - Type
    30,                               // nr
    1,                                // index
    &opt_app.Cabriokas[2].Enabled,    // option
    (unsigned char *)&option_index_2, // Optie index 
    lcd_cabriokas_disp,               // display
    &disp_cursor_group_B,             // cursor
    &key_cabriokas_val,               // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Type_Value,                 // void (*arrow)(void); 
    Enter_Type_Value,                 // void (*enter)(void);
  },
  { // Cabriokas[2]
    31,                               // nr
    0,                                // index
    &opt_app.Cabriokas[2].Enabled,    // option
    (unsigned char *)&option_index_2, // Optie index 
    lcd_groepen_disp,                 // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Option_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  { // Cabriokas[2] - GroupA
    31,                               // nr
    1,                                // index
    &opt_app.Cabriokas[2].Enabled,    // option
    (unsigned char *)&option_index_2, // Optie index 
    lcd_groepen_disp,                 // display
    &disp_cursor_group_A,             // cursor
    &key_group_A_val,                 // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Group_Value,                // void (*arrow)(void); 
    Enter_Group_Value,                // void (*arrow)(void); 
  },
  { // Cabriokas[2] - GroupB
    31,                               // nr
    2,                                // index
    &opt_app.Cabriokas[2].Enabled,    // option
    (unsigned char *)&option_index_2, // Optie index 
    lcd_groepen_disp,                 // display
    &disp_cursor_group_B,             // cursor
    &key_group_B_val,                 // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Group_Value,                // void (*arrow)(void); 
    Enter_Group_Value,                // void (*arrow)(void); 
  },
  { // Cabriokas[2] - MotorB
    31,                                // nr
    3,                                 // index
    &opt_app.Cabriokas[2].Enabled,     // option
    (unsigned char *)&option_index_2,  // Optie index 
    lcd_motoren_disp,                  // display
    &disp_cursor_group_B,              // cursor
    &key_motor_B_val,                  // *value
    Dummy_Func,                        // void (*number)(void); 
    Arrow_MotorB_Value,                // void (*arrow)(void); 
    Enter_MotorB_Value,                // void (*arrow)(void); 
  },
  { // Voorloop [%]
    32,                               // nr
    0,                                // index
    &opt_app.Cabriokas[2].Enabled,    // option
    (unsigned char *)&option_index_2, // Optie index 
    lcd_voorloop_disp,                // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Option_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    32,                               // nr
    1,                                // index
    &opt_app.Cabriokas[2].Enabled,    // option
    (unsigned char *)&option_index_2, // Optie index 
    lcd_voorloop_disp,                // display
    &disp_cursor_207_78_8,            // cursor
    &key_voorloop_val,                // *value
    Number_Value,                     // void (*number)(void); 
    Arrow_Option_Value,               // void (*arrow)(void); 
    Increment_Func_Index_Enter_Option_Value, // void (*enter)(void);
  },
  { // Hysterese [%]
    33,                               // nr
    0,                                // index
    &opt_app.Cabriokas[2].Enabled,    // option
    (unsigned char *)&option_index_2, // Optie index 
    lcd_hysterese_disp,               // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Option_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    33,                               // nr
    1,                                // index
    &opt_app.Cabriokas[2].Enabled,    // option
    (unsigned char *)&option_index_2, // Optie index 
    lcd_hysterese_disp,               // display
    &disp_cursor_207_78_8,            // cursor
    &key_hysterese_val,               // *value
    Number_Value,                     // void (*number)(void); 
    Arrow_Option_Value,               // void (*arrow)(void); 
    Increment_Func_Index_Enter_Option_Value, // void (*enter)(void);
  },
  //-----------------------------
  { // Cabriokas[3] - Type
    40,                               // nr
    0,                                // index
    &opt_app.Cabriokas[3].Enabled,    // option
    (unsigned char *)&option_index_3, // Optie index 
    lcd_cabriokas_disp,               // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Option_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*number)(void); 
  },
  { // Cabriokas[3] - Type
    40,                               // nr
    1,                                // index
    &opt_app.Cabriokas[3].Enabled,    // option
    (unsigned char *)&option_index_3, // Optie index 
    lcd_cabriokas_disp,               // display
    &disp_cursor_group_B,             // cursor
    &key_cabriokas_val,               // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Type_Value,                 // void (*arrow)(void); 
    Enter_Type_Value,                 // void (*enter)(void);
  },
  { // Cabriokas[3]
    41,                               // nr
    0,                                // index
    &opt_app.Cabriokas[3].Enabled,    // option
    (unsigned char *)&option_index_3, // Optie index 
    lcd_groepen_disp,                 // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Option_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  { // Cabriokas[3] - GroupA
    41,                               // nr
    1,                                // index
    &opt_app.Cabriokas[3].Enabled,    // option
    (unsigned char *)&option_index_3, // Optie index 
    lcd_groepen_disp,                 // display
    &disp_cursor_group_A,             // cursor
    &key_group_A_val,                 // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Group_Value,                // void (*arrow)(void); 
    Enter_Group_Value,                // void (*arrow)(void); 
  },
  { // Cabriokas[3] - GroupB
    41,                               // nr
    2,                                // index
    &opt_app.Cabriokas[3].Enabled,    // option
    (unsigned char *)&option_index_3, // Optie index 
    lcd_groepen_disp,                 // display
    &disp_cursor_group_B,             // cursor
    &key_group_B_val,                 // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Group_Value,                // void (*arrow)(void); 
    Enter_Group_Value,                // void (*arrow)(void); 
  },
  { // Cabriokas[3] - MotorB
    41,                                // nr
    3,                                 // index
    &opt_app.Cabriokas[3].Enabled,     // option
    (unsigned char *)&option_index_4,  // Optie index 
    lcd_motoren_disp,                  // display
    &disp_cursor_group_B,              // cursor
    &key_motor_B_val,                  // *value
    Dummy_Func,                        // void (*number)(void); 
    Arrow_MotorB_Value,                // void (*arrow)(void); 
    Enter_MotorB_Value,                // void (*arrow)(void); 
  },
  { // Voorloop [%]
    42,                               // nr
    0,                                // index
    &opt_app.Cabriokas[3].Enabled,    // option
    (unsigned char *)&option_index_3, // Optie index 
    lcd_voorloop_disp,                // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Option_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    42,                               // nr
    1,                                // index
    &opt_app.Cabriokas[3].Enabled,    // option
    (unsigned char *)&option_index_3, // Optie index 
    lcd_voorloop_disp,                // display
    &disp_cursor_207_78_8,            // cursor
    &key_voorloop_val,                // *value
    Number_Value,                     // void (*number)(void); 
    Arrow_Option_Value,               // void (*arrow)(void); 
    Increment_Func_Index_Enter_Option_Value, // void (*enter)(void);
  },
  { // Hysterese [%]
    43,                               // nr
    0,                                // index
    &opt_app.Cabriokas[3].Enabled,    // option
    (unsigned char *)&option_index_3, // Optie index 
    lcd_hysterese_disp,               // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Option_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    43,                               // nr
    1,                                // index
    &opt_app.Cabriokas[3].Enabled,    // option
    (unsigned char *)&option_index_3, // Optie index 
    lcd_hysterese_disp,               // display
    &disp_cursor_207_78_8,            // cursor
    &key_hysterese_val,               // *value
    Number_Value,                     // void (*number)(void); 
    Arrow_Option_Value,               // void (*arrow)(void); 
    Increment_Func_Index_Enter_Option_Value, // void (*enter)(void);
  },
  //-----------------------------
  { // Cabriokas[4] - Type
    50,                               // nr
    0,                                // index
    &opt_app.Cabriokas[4].Enabled,    // option
    (unsigned char *)&option_index_4, // Optie index 
    lcd_cabriokas_disp,               // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Option_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*number)(void); 
  },
  { // Cabriokas[4] - Type
    50,                               // nr
    1,                                // index
    &opt_app.Cabriokas[4].Enabled,    // option
    (unsigned char *)&option_index_4, // Optie index 
    lcd_cabriokas_disp,               // display
    &disp_cursor_group_B,             // cursor
    &key_cabriokas_val,               // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Type_Value,                 // void (*arrow)(void); 
    Enter_Type_Value,                 // void (*enter)(void);
  },
  { // Cabriokas[4]
    51,                               // nr
    0,                                // index
    &opt_app.Cabriokas[4].Enabled,    // option
    (unsigned char *)&option_index_4, // Optie index 
    lcd_groepen_disp,                 // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Option_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  { // Cabriokas[4] - GroupA
    51,                               // nr
    1,                                // index
    &opt_app.Cabriokas[4].Enabled,    // option
    (unsigned char *)&option_index_4, // Optie index 
    lcd_groepen_disp,                 // display
    &disp_cursor_group_A,             // cursor
    &key_group_A_val,                 // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Group_Value,                // void (*arrow)(void); 
    Enter_Group_Value,                // void (*arrow)(void); 
  },
  { // Cabriokas[4] - GroupB
    51,                               // nr
    2,                                // index
    &opt_app.Cabriokas[4].Enabled,    // option
    (unsigned char *)&option_index_4, // Optie index 
    lcd_groepen_disp,                 // display
    &disp_cursor_group_B,             // cursor
    &key_group_B_val,                 // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Group_Value,                // void (*arrow)(void); 
    Enter_Group_Value,                // void (*arrow)(void); 
  },
  { // Cabriokas[4] - MotorB
    51,                                // nr
    3,                                 // index
    &opt_app.Cabriokas[4].Enabled,     // option
    (unsigned char *)&option_index_4,  // Optie index 
    lcd_motoren_disp,                  // display
    &disp_cursor_group_B,              // cursor
    &key_motor_B_val,                  // *value
    Dummy_Func,                        // void (*number)(void); 
    Arrow_MotorB_Value,                // void (*arrow)(void); 
    Enter_MotorB_Value,                // void (*arrow)(void); 
  },
  { // Voorloop [%]
    52,                               // nr
    0,                                // index
    &opt_app.Cabriokas[4].Enabled,    // option
    (unsigned char *)&option_index_4, // Optie index 
    lcd_voorloop_disp,                // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Option_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    52,                               // nr
    1,                                // index
    &opt_app.Cabriokas[4].Enabled,    // option
    (unsigned char *)&option_index_4, // Optie index 
    lcd_voorloop_disp,                // display
    &disp_cursor_207_78_8,            // cursor
    &key_voorloop_val,                // *value
    Number_Value,                     // void (*number)(void); 
    Arrow_Option_Value,               // void (*arrow)(void); 
    Increment_Func_Index_Enter_Option_Value, // void (*enter)(void);
  },
  { // Hysterese [%]
    53,                               // nr
    0,                                // index
    &opt_app.Cabriokas[4].Enabled,    // option
    (unsigned char *)&option_index_4, // Optie index 
    lcd_hysterese_disp,               // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Option_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    53,                               // nr
    1,                                // index
    &opt_app.Cabriokas[4].Enabled,    // option
    (unsigned char *)&option_index_4, // Optie index 
    lcd_hysterese_disp,               // display
    &disp_cursor_207_78_8,            // cursor
    &key_hysterese_val,               // *value
    Number_Value,                     // void (*number)(void); 
    Arrow_Option_Value,               // void (*arrow)(void); 
    Increment_Func_Index_Enter_Option_Value, // void (*enter)(void);
  },
  //-----------------------------
  { // Cabriokas[5] - Type
    60,                               // nr
    0,                                // index
    &opt_app.Cabriokas[5].Enabled,    // option
    (unsigned char *)&option_index_5, // Optie index 
    lcd_cabriokas_disp,               // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Option_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*number)(void); 
  },
  { // Cabriokas[5] - Type
    60,                               // nr
    1,                                // index
    &opt_app.Cabriokas[5].Enabled,    // option
    (unsigned char *)&option_index_5, // Optie index 
    lcd_cabriokas_disp,               // display
    &disp_cursor_group_B,             // cursor
    &key_cabriokas_val,               // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Type_Value,                 // void (*arrow)(void); 
    Enter_Type_Value,                 // void (*enter)(void);
  },
  { // Cabriokas[5]
    61,                               // nr
    0,                                // index
    &opt_app.Cabriokas[5].Enabled,    // option
    (unsigned char *)&option_index_5, // Optie index 
    lcd_groepen_disp,                 // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Option_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  { // Cabriokas[5] - GroupA
    61,                               // nr
    1,                                // index
    &opt_app.Cabriokas[5].Enabled,    // option
    (unsigned char *)&option_index_5, // Optie index 
    lcd_groepen_disp,                 // display
    &disp_cursor_group_A,             // cursor
    &key_group_A_val,                 // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Group_Value,                // void (*arrow)(void); 
    Enter_Group_Value,                // void (*arrow)(void); 
  },
  { // Cabriokas[5] - GroupB
    61,                               // nr
    2,                                // index
    &opt_app.Cabriokas[5].Enabled,    // option
    (unsigned char *)&option_index_5, // Optie index 
    lcd_groepen_disp,                 // display
    &disp_cursor_group_B,             // cursor
    &key_group_B_val,                 // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Group_Value,                // void (*arrow)(void); 
    Enter_Group_Value,                // void (*arrow)(void); 
  },
  { // Cabriokas[5] - MotorB
    61,                                // nr
    3,                                 // index
    &opt_app.Cabriokas[5].Enabled,     // option
    (unsigned char *)&option_index_5,  // Optie index 
    lcd_motoren_disp,                  // display
    &disp_cursor_group_B,              // cursor
    &key_motor_B_val,                  // *value
    Dummy_Func,                        // void (*number)(void); 
    Arrow_MotorB_Value,                // void (*arrow)(void); 
    Enter_MotorB_Value,                // void (*arrow)(void); 
  },
  { // Voorloop [%]
    62,                               // nr
    0,                                // index
    &opt_app.Cabriokas[5].Enabled,    // option
    (unsigned char *)&option_index_5, // Optie index 
    lcd_voorloop_disp,                // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Option_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    62,                               // nr
    1,                                // index
    &opt_app.Cabriokas[5].Enabled,    // option
    (unsigned char *)&option_index_5, // Optie index 
    lcd_voorloop_disp,                // display
    &disp_cursor_207_78_8,            // cursor
    &key_voorloop_val,                // *value
    Number_Value,                     // void (*number)(void); 
    Arrow_Option_Value,               // void (*arrow)(void); 
    Increment_Func_Index_Enter_Option_Value, // void (*enter)(void);
  },
  { // Hysterese [%]
    63,                               // nr
    0,                                // index
    &opt_app.Cabriokas[5].Enabled,    // option
    (unsigned char *)&option_index_5, // Optie index 
    lcd_hysterese_disp,               // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Option_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    63,                               // nr
    1,                                // index
    &opt_app.Cabriokas[5].Enabled,    // option
    (unsigned char *)&option_index_5, // Optie index 
    lcd_hysterese_disp,               // display
    &disp_cursor_207_78_8,            // cursor
    &key_hysterese_val,               // *value
    Number_Value,                     // void (*number)(void); 
    Arrow_Option_Value,               // void (*arrow)(void); 
    Increment_Func_Index_Enter_Option_Value, // void (*enter)(void);
  },
  //-----------------------------
  { // Cabriokas[6] - Type
    70,                               // nr
    0,                                // index
    &opt_app.Cabriokas[6].Enabled,    // option
    (unsigned char *)&option_index_6, // Optie index 
    lcd_cabriokas_disp,               // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Option_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*number)(void); 
  },
  { // Cabriokas[6] - Type
    70,                               // nr
    1,                                // index
    &opt_app.Cabriokas[6].Enabled,    // option
    (unsigned char *)&option_index_6, // Optie index 
    lcd_cabriokas_disp,               // display
    &disp_cursor_group_B,             // cursor
    &key_cabriokas_val,               // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Type_Value,                 // void (*arrow)(void); 
    Enter_Type_Value,                 // void (*enter)(void);
  },
  { // Cabriokas[6]
    71,                               // nr
    0,                                // index
    &opt_app.Cabriokas[6].Enabled,    // option
    (unsigned char *)&option_index_6, // Optie index 
    lcd_groepen_disp,                 // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Option_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  { // Cabriokas[6] - GroupA
    71,                               // nr
    1,                                // index
    &opt_app.Cabriokas[6].Enabled,    // option
    (unsigned char *)&option_index_6, // Optie index 
    lcd_groepen_disp,                 // display
    &disp_cursor_group_A,             // cursor
    &key_group_A_val,                 // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Group_Value,                // void (*arrow)(void); 
    Enter_Group_Value,                // void (*arrow)(void); 
  },
  { // Cabriokas[6] - GroupB
    71,                               // nr
    2,                                // index
    &opt_app.Cabriokas[6].Enabled,    // option
    (unsigned char *)&option_index_6, // Optie index 
    lcd_groepen_disp,                 // display
    &disp_cursor_group_B,             // cursor
    &key_group_B_val,                 // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Group_Value,                // void (*arrow)(void); 
    Enter_Group_Value,                // void (*arrow)(void); 
  },
  { // Cabriokas[6] - MotorB
    71,                                // nr
    3,                                 // index
    &opt_app.Cabriokas[6].Enabled,     // option
    (unsigned char *)&option_index_6,  // Optie index 
    lcd_motoren_disp,                  // display
    &disp_cursor_group_B,              // cursor
    &key_motor_B_val,                  // *value
    Dummy_Func,                        // void (*number)(void); 
    Arrow_MotorB_Value,                // void (*arrow)(void); 
    Enter_MotorB_Value,                // void (*arrow)(void); 
  },
  { // Voorloop [%]
    72,                               // nr
    0,                                // index
    &opt_app.Cabriokas[6].Enabled,    // option
    (unsigned char *)&option_index_6, // Optie index 
    lcd_voorloop_disp,                // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Option_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    72,                               // nr
    1,                                // index
    &opt_app.Cabriokas[6].Enabled,    // option
    (unsigned char *)&option_index_6, // Optie index 
    lcd_voorloop_disp,                // display
    &disp_cursor_207_78_8,            // cursor
    &key_voorloop_val,                // *value
    Number_Value,                     // void (*number)(void); 
    Arrow_Option_Value,               // void (*arrow)(void); 
    Increment_Func_Index_Enter_Option_Value, // void (*enter)(void);
  },
  { // Hysterese [%]
    73,                               // nr
    0,                                // index
    &opt_app.Cabriokas[6].Enabled,    // option
    (unsigned char *)&option_index_6, // Optie index 
    lcd_hysterese_disp,               // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Option_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    73,                               // nr
    1,                                // index
    &opt_app.Cabriokas[6].Enabled,    // option
    (unsigned char *)&option_index_6, // Optie index 
    lcd_hysterese_disp,               // display
    &disp_cursor_207_78_8,            // cursor
    &key_hysterese_val,               // *value
    Number_Value,                     // void (*number)(void); 
    Arrow_Option_Value,               // void (*arrow)(void); 
    Increment_Func_Index_Enter_Option_Value, // void (*enter)(void);
  },
  //-----------------------------
  { // Cabriokas[7] - Type
    80,                               // nr
    0,                                // index
    &opt_app.Cabriokas[7].Enabled,    // option
    (unsigned char *)&option_index_7, // Optie index 
    lcd_cabriokas_disp,               // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Option_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*number)(void); 
  },
  { // Cabriokas[7] - Type
    80,                               // nr
    1,                                // index
    &opt_app.Cabriokas[7].Enabled,    // option
    (unsigned char *)&option_index_7, // Optie index 
    lcd_cabriokas_disp,               // display
    &disp_cursor_group_B,             // cursor
    &key_cabriokas_val,               // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Type_Value,                 // void (*arrow)(void); 
    Enter_Type_Value,                 // void (*enter)(void);
  },
  { // Cabriokas[7]
    81,                               // nr
    0,                                // index
    &opt_app.Cabriokas[7].Enabled,    // option
    (unsigned char *)&option_index_7, // Optie index 
    lcd_groepen_disp,                 // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Option_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  { // Cabriokas[7] - GroupA
    81,                               // nr
    1,                                // index
    &opt_app.Cabriokas[7].Enabled,    // option
    (unsigned char *)&option_index_7, // Optie index 
    lcd_groepen_disp,                 // display
    &disp_cursor_group_A,             // cursor
    &key_group_A_val,                 // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Group_Value,                // void (*arrow)(void); 
    Enter_Group_Value,                // void (*arrow)(void); 
  },
  { // Cabriokas[7] - GroupB
    81,                               // nr
    2,                                // index
    &opt_app.Cabriokas[7].Enabled,    // option
    (unsigned char *)&option_index_7, // Optie index 
    lcd_groepen_disp,                 // display
    &disp_cursor_group_B,             // cursor
    &key_group_B_val,                 // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Group_Value,                // void (*arrow)(void); 
    Enter_Group_Value,                // void (*arrow)(void); 
  },
  { // Cabriokas[7] - MotorB
    81,                                // nr
    3,                                 // index
    &opt_app.Cabriokas[7].Enabled,     // option
    (unsigned char *)&option_index_7,  // Optie index 
    lcd_motoren_disp,                  // display
    &disp_cursor_group_B,              // cursor
    &key_motor_B_val,                  // *value
    Dummy_Func,                        // void (*number)(void); 
    Arrow_MotorB_Value,                // void (*arrow)(void); 
    Enter_MotorB_Value,                // void (*arrow)(void); 
  },
  { // Voorloop [%]
    82,                               // nr
    0,                                // index
    &opt_app.Cabriokas[7].Enabled,    // option
    (unsigned char *)&option_index_7, // Optie index 
    lcd_voorloop_disp,                // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Option_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    82,                               // nr
    1,                                // index
    &opt_app.Cabriokas[7].Enabled,    // option
    (unsigned char *)&option_index_7, // Optie index 
    lcd_voorloop_disp,                // display
    &disp_cursor_207_78_8,            // cursor
    &key_voorloop_val,                // *value
    Number_Value,                     // void (*number)(void); 
    Arrow_Option_Value,               // void (*arrow)(void); 
    Increment_Func_Index_Enter_Option_Value, // void (*enter)(void);
  },
  { // Hysterese [%]
    83,                               // nr
    0,                                // index
    &opt_app.Cabriokas[7].Enabled,    // option
    (unsigned char *)&option_index_7, // Optie index 
    lcd_hysterese_disp,               // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Option_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    83,                               // nr
    1,                                // index
    &opt_app.Cabriokas[7].Enabled,    // option
    (unsigned char *)&option_index_7, // Optie index 
    lcd_hysterese_disp,               // display
    &disp_cursor_207_78_8,            // cursor
    &key_hysterese_val,               // *value
    Number_Value,                     // void (*number)(void); 
    Arrow_Option_Value,               // void (*arrow)(void); 
    Increment_Func_Index_Enter_Option_Value, // void (*enter)(void);
  },
  //-----------------------------
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
s_screen screen_opt_cabriokas_2;
s_screen const screen_opt_cabriokas_2_default =
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
  &opt_cabriokas_2_key_action[0], // first_action
  &opt_cabriokas_2_key_action[sizeof(opt_cabriokas_2_key_action)/sizeof(s_key_action) - 1], // last action 
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
void Control_Screen_Option_Cabriokas_2(void)
{
int i;

  if (password_enabled & PASSWORD_ENABLED_OPT_MASK)
    start_flag = end_flag = 1;
  else
    start_flag = end_flag = 0;
  Control_Screen(&screen_opt_cabriokas_2, &screen_opt_cabriokas_2_default, 1, 1);

  Aantal = 0;
  for (i = 0; i < MAX_CABRIO; i++)
  {
    if (opt_app.Cabriokas[i].Enabled)
      Aantal++;
  }
}

//-----------------------------------------------------------------------------
static void CheckOptionsCabriokasMotors(void)
{
int i, index;
int A, B;
TMotor *pMotor, *pLink;

  // Check all motors
  for (i = 0; i < opt_app.NumberMotorgroups; i++)
  {
    if (opt_app.Motorgroup[i].ControlType == CONTROL_CABRIOKAS) //&& (opt_app.Motorgroup[i].Type == TYPE_RAAM))
	{
      index = opt_app.Motorgroup[i].ControlIndex;
      A = opt_app.Cabriokas[index].GroupA;
      B = opt_app.Cabriokas[index].GroupB;
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
        pMotor = pMotor->Next;
      }
    }
  }
}

void CheckOptionsCabriokas(void)
{
int i;
int A, B;

  // Clear all connections
  for (i = 0; i < opt_app.NumberMotorgroups; i++)
  {
    if (opt_app.Motorgroup[i].ControlType == CONTROL_CABRIOKAS) //&& (opt_app.Motorgroup[i].Type == TYPE_RAAM))
    {
      opt_app.Motorgroup[i].ControlType  = CONTROL_NORMAL;
      opt_app.Motorgroup[i].ControlIndex = 0;
    }
  }
  // Rebuild all connections
  for (i = 0; i < MAX_CABRIO; i++)
  {
    if (opt_app.Cabriokas[i].Enabled)
    {
      A = opt_app.Cabriokas[i].GroupA;
      B = opt_app.Cabriokas[i].GroupB;
      if (A != -1)
      {
        if (opt_app.Motorgroup[A].Enabled && (opt_app.Motorgroup[A].Type == TYPE_RAAM))
        {
          opt_app.Motorgroup[A].ControlType  = CONTROL_CABRIOKAS;
          opt_app.Motorgroup[A].ControlIndex = i;
        }
        else
        {
          opt_app.Cabriokas[i].GroupA = -1;
        }
      }
      if (B != -1)
      {
        if (opt_app.Motorgroup[B].Enabled && (opt_app.Motorgroup[B].Type == TYPE_RAAM))
        {
          opt_app.Motorgroup[B].ControlType  = CONTROL_CABRIOKAS;
          opt_app.Motorgroup[B].ControlIndex = i;
        }
        else
        {
          opt_app.Cabriokas[i].GroupB = -1;
        }
      }
    }
    else
    {
      opt_app.Cabriokas[i] = default_opt_app.Cabriokas[i];
    }
  }
  CheckOptionsCabriokasMotors();
}

//-----------------------------------------------------------------------------
static void Arrow_Aantal_Value(void)
{
int i;

  Arrow_Option_Value();
  if (screen_ptr->index == 0)
  {
    for (i = 0; i < Aantal; i++)
      opt_app.Cabriokas[i].Enabled = 1;
    for (i = Aantal; i < MAX_CABRIO; i++)
      opt_app.Cabriokas[i] = default_opt_app.Cabriokas[i];
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
    for (i = 0; i < Aantal; i++)
      opt_app.Cabriokas[i].Enabled = 1;
    for (i = Aantal; i < MAX_CABRIO; i++)
      opt_app.Cabriokas[i] = default_opt_app.Cabriokas[i];
    CheckOptions();
    Refresh_Screen_Nr_Aantal();
  }
}

//-----------------------------------------------------------------------------
static void Arrow_Type_Value(void)
{
  Arrow_Scroll_Option_Value();
  Refresh_Screen_Nr_Aantal();
}

static void Enter_Type_Value(void)
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
  if (opt_app.Motorgroup[nr].Type != TYPE_RAAM) // Alleen mogelijk bij doek sturing
    return (0);
  for (i = 0; i < MAX_CABRIO; i++)
  {
    if (opt_app.Cabriokas[i].Enabled)
    {
      if (nr == opt_app.Cabriokas[i].GroupA)
        return (0);
      if (nr == opt_app.Cabriokas[i].GroupB)
        return (0);
    }
  }
  return (1);
}

//-----------------------------------------------------------------------------
static void Arrow_Group_Value(void)
{
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
      if (screen_ptr->index == 2)
      {
        CheckOptions();
        if ((opt_app.Cabriokas[nr].GroupA == -1) || (opt_app.Cabriokas[nr].GroupB == -1))
        {
          Decrement_Func_Index();
          Decrement_Func_Index();
        }
        else
        {
          pMotorA = Motorgroup[opt_app.Cabriokas[nr].GroupA].FirstMotor;
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

static void Enter_Group_Value(void)
{
int nr;

  nr = (screen_ptr->nr / 10) - 1;
  if (Enter_Value())
  {
    if (screen_ptr->index == 2)
    {
      CheckOptions();
      option_change_flag = 1;
      if ((opt_app.Cabriokas[nr].GroupA == -1) || (opt_app.Cabriokas[nr].GroupB == -1))
      {
        Decrement_Func_Index();
        Decrement_Func_Index();
      }
      else
      {
        pMotorA = Motorgroup[opt_app.Cabriokas[nr].GroupA].FirstMotor;
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
  pMotor = Motorgroup[opt_app.Cabriokas[nr].GroupA].FirstMotor;
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
          pMotorB = Motorgroup[opt_app.Cabriokas[nr].GroupB].FirstMotor;
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
          pMotorB = Motorgroup[opt_app.Cabriokas[nr].GroupB].LastMotor;
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
