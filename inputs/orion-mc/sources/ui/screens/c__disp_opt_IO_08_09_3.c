// C__DISP_OPT_IO_08_09_3.C

#include "ch_define.h"

#include "ch_alg.h"
#include "ch_const.h"
#include "ch_disp.h"
#include "ch_disp_func.h"
#include "ch_disp_option_1.h"
#include "ch_disp_opt_IO_2.h"
#include "ch_disp_password.h"
#include "ch_disp_value.h"
#include "ch_IO_08_09_board.h"
#include "ch_key.h"
#include "ch_lcd_10.h"
#include "ch_lcd_14.h"
#include "ch_lcd_20.h"
#include "ch_lcd_bitmap.h"
#include "ch_lcd_pixel.h"
#include "ch_string.h"
#include "ch_disp_opt_IO_08_09_3.h"

#define INDEX_DIG_IN_ALG    0
#define INDEX_DIG_IN_WATER 10
#define INDEX_DIG_IN_VOER  20
#define INDEX_DIG_IN_EI    30
#define INDEX_DIG_IN_KWH   40

#define INDEX_ANA_OUT_ALG 0

#define INDEX_DIG_OUT_ALG 0

static unsigned char const dig_in_possible[DIG_IN_MAX] =
{                         
 1, //  0 DIG_IN_EMPTY
 0, //  1 DIG_IN_WATER
 0, //  2 DIG_IN_VOER_PULS
 0, //  3 DIG_IN_VOERWEGER
 0, //  4 DIG_IN_EI_PULS
 0, //  5 DIG_IN_EI
 0, //  6 DIG_IN_VOER
 0, //  7 DIG_IN_KLOK
 1, //  8 DIG_IN_ALARM
 1, //  9 DIG_IN_VORST
 0, // 10 DIG_IN_KWH_PULS
 0, // 11 DIG_IN_TOEREN (Altijd 0 niet mogelijk bij dit type bord)
 1, // 12 DIG_IN_RAAM
 1, // 13 DIG_IN_DOEK
 1, // 14 DIG_IN_VENT
 1, // 15 DIG_IN_LAMEL
 1, // 16 DIG_IN_VERWARMING
 1  // 17 DIG_IN_KLEP
};

static unsigned char const ana_out_possible[ANA_OUT_MAX] =
{
  1, //  0 ANA_OUT_EMPTY
  1, //  1 ANA_OUT_VENT
  1, //  2 ANA_OUT_KLEP
  1, //  3 ANA_OUT_VERWARMING
  0, //  4 ANA_OUT_LICHT
  0, //  5 ANA_OUT_UNI_REG
  0, //  6 ANA_OUT_MESTDROGING
  0, //  7 ANA_OUT_EI
  1, //  8 ANA_OUT_RAAM
  1, //  9 ANA_OUT_DOEK
  1  // 10 ANA_OUT_LAMEL
};

static unsigned char const dig_out_possible[DIG_OUT_MAX] =
{
  1, //  0 DIG_OUT_EMPTY
  1, //  1 DIG_OUT_VENT
  0, //  2 DIG_OUT_KLEP
  1, //  3 DIG_OUT_VERWARMING
  0, //  4 DIG_OUT_KOELING
  0, //  5 DIG_OUT_RV
  0, //  6 DIG_OUT_KLOK
  0, //  7 DIG_OUT_LICHT
  0, //  8 DIG_OUT_UNI_REG
  0, //  9 DIG_OUT_TUNNEL
  0, // 10 DIG_OUT_WATER
  0, // 11 DIG_OUT_VOER
  0, // 12 DIG_OUT_VOER_PULS
  0, // 13 DIG_OUT_VOERWEGER
  0, // 14 DIG_OUT_MESTDROGING
  0, // 15 DIG_OUT_EI
  0, // 16 DIG_OUT_HOPPER
  1, // 17 DIG_OUT_ALARM
  0, // 18 DIG_OUT_SILO
  1  // 19 DIG_OUT_LAMEL
};

static void Arrow_Board_Func(void);
static void Arrow_Board_Value(void);
static void Enter_Board_Value(void);
static void Arrow_Board_OK_Value(void);

static void Copy_Dig_In_To_Array(void);
static void Copy_Array_To_Dig_In(void);
static void Arrow_Dig_In_Func(void);
static void Enter_Dig_In_Func(void);
static void Arrow_Dig_In_Pulsen_Per_Eenheid_Value(void);
static void Arrow_Dig_In_Pulsen_Value(void);
static void Enter_Dig_In_Pulsen_Value(void);

static void Copy_Ana_Out_To_Array(void);
static void Copy_Array_To_Ana_Out(void);
static void Arrow_Ana_Out_Func(void);
static void Enter_Ana_Out_Func(void);
static void Arrow_Ana_Out_Sel_Value(void);
static void Enter_Ana_Out_Sel_Value(void);
static void Arrow_Ana_Out_Min_Value(void);
static void Enter_Ana_Out_Min_Value(void);
static void Arrow_Ana_Out_Max_Value(void);
static void Enter_Ana_Out_Max_Value(void);

static void Copy_Dig_Out_To_Array(void);
static void Copy_Array_To_Dig_Out(void);
static void Arrow_Dig_Out_Func(void);
static void Enter_Dig_Out_Func(void);
static void Arrow_Dig_Out_Value(void);
static void Enter_Dig_Out_Value(void);

static void Arrow_IO_08_09_Func(void);

static void Arrow_Copy_Board_Val(void);
static void Enter_Copy_Board_Val(void);
static void Arrow_Copy_Board_OK_Value(void);
static void Enter_Copy_Board_OK_Value(void);

static void Arrow_End_IO_Func(void);

static s_disp_agri_header const disp_header = { Disp_Draw_Agri_Header, FN6, EMPTY, &tekst_leeg, HK_GEEN, 0, 4};
static s_disp_tekst const disp_header_str_0  = { Disp_Draw_Tekst_L,  12, 12, &tekst_inst.Opties_bord_10 };
static s_disp_tekst_array_add const disp_header_str_1 = { Disp_Draw_Tekst_Array_Add_L, &tekst_IO_08_09_10, UCHAR, &index_IO, IO_08_09_MAX+1 };
static void * const lcd_disp_header[] = { &disp_header_str_0, &disp_space_10_L, &disp_header_str_1, &disp_header, 0 };

static s_disp_cursor const disp_cursor_219_78_63 = { 219, 78, 63, 2, 18 };

static unsigned char const index_IO_max = IO_08_09_MAX;
static unsigned char index_IO_new;
static unsigned char board_flag;
static unsigned char counter_flag;
//static unsigned char array[16];
//static unsigned char max_array_index = 12;

static unsigned char pulsen_per_eenheid;
static int pulsen;
static int dig_out_relais;
static s_option_analog_output option_analog_output_help; // voor bewaren analog_output
static unsigned int analog_output_option;
static int analog_output_voltage;
static int analog_output_min_value;
static int analog_output_max_value;
static int analog_output_max;
static int value_analog_output_help;
static int value_digital_output_test_help;

static s_disp_tekst const disp_nummer_IO_module        = { Disp_Draw_Tekst_L,  0, 22, &tekst_inst.Nummer_IO_Module_14 };
static s_disp_tekst const disp_ronde_openings_haak_str = { Disp_Draw_Tekst_L,  0, 50, &tekst_ronde_openings_haak_14 };
static s_disp_tekst_array const disp_index_IO_string   = { Disp_Draw_Tekst_Array_L, 126, 75, &tekst_IO_08_09_14, UCHAR, &index_IO,     IO_08_09_MAX+1 };
static s_disp_tekst_array const disp_index_IO_string_1 = { Disp_Draw_Tekst_Array_L, 126, 75, &tekst_IO_08_09_14, UCHAR, &index_IO_new, IO_08_09_MAX+1 };

static s_disp_tekst const disp_digitale_ingang_str  = { Disp_Draw_Tekst_L, 56, 22, &tekst_inst.Digitale_Ingang_14 };
static s_disp_tekst const disp_analoge_uitgang_str  = { Disp_Draw_Tekst_L, 56, 22, &tekst_inst.Analoge_Uitgang_14 };
static s_disp_tekst const disp_digitale_uitgang_str = { Disp_Draw_Tekst_L, 56, 22, &tekst_inst.Digitale_Uitgang_14 };

// Message screen

void const * const tekst_0_5V_0_20mA_14[] = { &tekst_leeg, &tekst_0_5V_14, &tekst_0_20mA_14 };
static s_disp_tekst_array const disp_0_5V_0_20mA_string = { Disp_Draw_Tekst_Array_L, 157, 75, &tekst_0_5V_0_20mA_14, UINT, &analog_output_option, 3 };

void const * const tekst_V_mA_14[] = { &tekst_leeg, &tekst_inst.V_14, &tekst_inst.mA_14 };
static s_disp_tekst const disp_minimum_string    = { Disp_Draw_Tekst_L,  56, 49, &tekst_inst.Minimum_14 };
static s_disp_value const disp_ana_out_min_value = { Disp_Draw_Value,   192, 49, (SIZE_14 | RECHTS), INT, 1, &analog_output_min_value };
//static s_disp_tekst const disp_min_V_str         = { Disp_Draw_Tekst_L, 195, 49, &tekst_inst.V_14 };
static s_disp_tekst const disp_maximum_string    = { Disp_Draw_Tekst_L,  56, 75, &tekst_inst.Maximum_14 };
static s_disp_value const disp_ana_out_max_value = { Disp_Draw_Value,   192, 75, (SIZE_14 | RECHTS), INT, 1, &analog_output_max_value };
//static s_disp_tekst const disp_max_V_str         = { Disp_Draw_Tekst_L, 195, 75, &tekst_inst.V_14 };
static s_disp_tekst_array const disp_min_V_mA_str = { Disp_Draw_Tekst_Array_L, 195, 49, &tekst_V_mA_14, UINT, &analog_output_option, 3 };
static s_disp_tekst_array const disp_max_V_mA_str = { Disp_Draw_Tekst_Array_L, 195, 75, &tekst_V_mA_14, UINT, &analog_output_option, 3 };

static void const * const tekst_water_per[]      = { &tekst_inst.Liters_Per_Puls_14, &tekst_inst.Pulsen_Per_Liter_14 };
static s_disp_tekst_array const disp_water_per_string = { Disp_Draw_Tekst_Array_L, 9, 50, &tekst_water_per, UCHAR, &pulsen_per_eenheid, 2 };
static s_disp_value const disp_water_pulsen_value     = { Disp_Draw_Value, 192, 75, (SIZE_14 | RECHTS), INT, 0, &pulsen };

static void const * const tekst_voer_per[]      = { &tekst_inst.Kg_Per_Puls_14, &tekst_inst.Pulsen_Per_Kg_14 };
static s_disp_tekst_array const disp_voer_per_string = { Disp_Draw_Tekst_Array_L, 9, 50, &tekst_voer_per, UCHAR, &pulsen_per_eenheid, 2 };
static s_disp_value const disp_voer_pulsen_value     = { Disp_Draw_Value, 192, 75, (SIZE_14 | RECHTS), INT, 0, &pulsen };

static void const * const tekst_ei_per[]      = { &tekst_inst.Eieren_Per_Puls_14, &tekst_inst.Pulsen_Per_Ei_14 };
static s_disp_tekst_array const disp_ei_per_string = { Disp_Draw_Tekst_Array_L, 9, 50, &tekst_ei_per, UCHAR, &pulsen_per_eenheid, 2 };
static s_disp_value const disp_ei_pulsen_value     = { Disp_Draw_Value, 192, 75, (SIZE_14 | RECHTS), INT, 0, &pulsen };

static void const * const tekst_kWh_per[]      = { &tekst_inst.kWh_Per_Puls_14, &tekst_inst.Pulsen_Per_kWh_14 };
static s_disp_tekst_array const disp_kWh_per_string = { Disp_Draw_Tekst_Array_L, 9, 50, &tekst_kWh_per, UCHAR, &pulsen_per_eenheid, 2 };
static s_disp_value const  disp_kWh_pulsen_value    = { Disp_Draw_Value, 192, 75, (SIZE_14 | RECHTS), INT, 0, &pulsen };

// Alarm contact
static void Arrow_Alarm_Contact_Func(void);
static void Arrow_Alarm_Contact_Value(void);
static unsigned char alarm_contact; // variabele invullen bij voorgaande en vorige functie
static s_disp_tekst const disp_alarm_contact_str        = { Disp_Draw_Tekst_L,       37, 22, &tekst_inst.Alarm_Contact_14 };
static s_disp_bitmap_array const disp_alarm_contact_bmp = { Disp_Draw_Bitmap_Array, 190, 57, bmp_false_true, UCHAR, &alarm_contact, 2 };
static void * const lcd_alarm_contact_disp[] = { &disp_alarm_contact_str, &disp_alarm_contact_bmp, 0 };
static s_key_value const key_alarm_contact   = { UCHAR, 1, &alarm_contact, &uchar_0, &uchar_1 };

static void * const lcd_IO_08_09_1_disp[] =
{
  &disp_nummer_IO_module,
  &disp_ronde_openings_haak_str, &disp_verwijderen_inst_str, &disp_space_14_L, 
  &disp_is_teken_14_L, &disp_space_14_L, &disp_min_teken_14_L, &disp_space_14_L, &disp_ronde_sluit_haak_14_L,
  &disp_index_IO_string, 0 
};
static void * const lcd_IO_08_09_1_disp_1[] =
{
  &disp_nummer_IO_module, &disp_index_IO_string_1, &disp_messagebox_bevestig,
  &disp_zeker_weten_inst_str, &disp_bevestig_arrow_up_bmp, &disp_bevestig_is_teken, &disp_space_10_L, &disp_ja_inst_str, 0
};
static void * const lcd_dig_in_disp[] =
{
  &disp_ingang_inst, &disp_digitaal, &disp_digitale_ingang_str, 
  &disp_box_pos1_bmp, &disp_box_pos2_bmp,  &disp_box_pos3_bmp,  &disp_box_pos4_bmp,
  &disp_box_pos5_bmp, &disp_box_pos6_bmp,  &disp_box_pos7_bmp,  &disp_box_pos8_bmp,
  &disp_block,
  &disp_IO[0], &disp_IO[1], &disp_IO[2],  &disp_IO[3],
  &disp_IO[4], &disp_IO[5], &disp_IO[6],  &disp_IO[7],
  &disp_value_IO, &disp_string_IO, &disp_unit_IO, 0
};
static void * const lcd_dig_in_water_disp[] =
{
  &disp_ingang_inst, &disp_digitaal, &disp_digitale_ingang_str, 
  &disp_water_per_string, &disp_water_pulsen_value, 0
};
static void * const lcd_dig_in_voer_disp[] =
{
  &disp_ingang_inst, &disp_digitaal, &disp_digitale_ingang_str, 
  &disp_voer_per_string, &disp_voer_pulsen_value, 0
};
static void * const lcd_dig_in_ei_disp[] =
{
  &disp_ingang_inst, &disp_digitaal, &disp_digitale_ingang_str, 
  &disp_ei_per_string, &disp_ei_pulsen_value, 0
};
static void * const lcd_dig_in_kWh_disp[] =
{
  &disp_ingang_inst, &disp_digitaal, &disp_digitale_ingang_str, 
  &disp_kWh_per_string, &disp_kWh_pulsen_value, 0
};
static void * const lcd_ana_out_disp[] =
{
  &disp_uitgang_inst, &disp_analoog, &disp_analoge_uitgang_str, 
  &disp_box_pos1_bmp, &disp_box_pos2_bmp, &disp_box_pos3_bmp, &disp_box_pos4_bmp,
  &disp_block,
  &disp_IO[0], &disp_IO[1], &disp_IO[2], &disp_IO[3],
  &disp_value_IO, &disp_unit_IO, 0
};
static void * const lcd_ana_out_sel_disp[] =
{
  &disp_uitgang_inst, &disp_analoog, &disp_analoge_uitgang_str, 
  &disp_0_5V_0_20mA_string, 0
};
static void * const lcd_ana_out_set_disp[] =
{
  &disp_uitgang_inst, &disp_analoog, &disp_analoge_uitgang_str, 
  &disp_minimum_string, &disp_ana_out_min_value, &disp_min_V_mA_str,
  &disp_maximum_string, &disp_ana_out_max_value, &disp_max_V_mA_str, 0
};
static void * const lcd_dig_out_disp[] =
{
  &disp_uitgang_inst, &disp_digitaal, &disp_digitale_uitgang_str, 
  &disp_box_pos1_bmp, &disp_box_pos2_bmp, &disp_box_pos3_bmp, &disp_box_pos4_bmp,
  &disp_block,
  &disp_IO[0], &disp_IO[1], &disp_IO[2], &disp_IO[3],
  &disp_string_IO, 0
};

static s_key_value const key_lcd_index_IO       = { UCHAR, 1, &index_IO,                &uchar_0, &index_IO_max };
static s_key_value const key_pulsen_per_eenheid = { UCHAR, 1, &pulsen_per_eenheid,      &uchar_0, &uchar_1      };
static s_key_value const key_pulsen             = { INT,   3, &pulsen,                  &int_1,   &int_999      };
static s_key_value const key_IO_dig_out         = { INT,   3, &dig_out_relais,          &int_0,   &int_1        };
static s_key_value const key_IO_ana_out_sel     = { UINT,  1, &analog_output_option,    &uint_1,  &uint_2       };
static s_key_value const key_IO_ana_out_min     = { INT,   3, &analog_output_min_value, &int_0, &analog_output_max };
static s_key_value const key_IO_ana_out_max     = { INT,   3, &analog_output_max_value, &int_0, &analog_output_max };
//---------------------------------------------------------------------------------------------------------------------------------
// Copy board
static unsigned char copy_index = 0;
static unsigned char copy_flag  = 0;

static s_disp_tekst     const disp_copy_opt_str = { Disp_Draw_Tekst_L, 37, 22, &tekst_inst.Opties_Kopieren_14 };
static s_disp_value     const disp_copy_opt_val = { Disp_Draw_Value,  207, 75, (SIZE_14 | RECHTS), UCHAR, 0, &copy_index };
static s_disp_tekst_add const disp_kopieren_str = { Disp_Draw_Tekst_Add_L, &tekst_inst.Kopieren_10 };

static void * const lcd_copy_IO_08_09_1_disp[] =   
{ 
  &disp_copy_inst, &disp_copy_opt_str, &disp_copy_opt_val, 0 
};
static void * const lcd_copy_IO_08_09_1_disp_1[] =
{ 
  &disp_copy_inst, &disp_copy_opt_str, &disp_copy_opt_val,
  &disp_messagebox_bevestig, &disp_zeker_weten_inst_str, 
  &disp_ok_str, &disp_space_10_L, &disp_is_teken_10_L, &disp_space_10_L, &disp_kopieren_str, 0 
};
//---------------------------------------------------------------------------------------------------------------------------------


s_key_action const opt_IO_08_09_3_key_action[] =
{
  {
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
  {
    1,                                // nr
    0,                                // index
    (unsigned char *)&option_on,      // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_08_09_1_disp,              // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Board_Func,                 // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    1,                                // nr
    1,                                // index
    (unsigned char *)&option_on,      // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_08_09_1_disp,              // display
    &disp_cursor_207_78_8,            // cursor
    &key_lcd_index_IO,                // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Board_Value,                // void (*arrow)(void); 
    Enter_Board_Value,                // void (*enter)(void);
  },
  {
    1,                                // nr
    2,                                // index
    (unsigned char *)&option_on,      // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_08_09_1_disp_1,            // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Board_OK_Value,             // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  { // instellen digitale ingangen
    3,                                // nr
    INDEX_DIG_IN_ALG,                 // index
    &board_flag,                      // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_dig_in_disp,                  // display
    &disp_cursor,                     // cursor
    &key_IO,                          // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Dig_In_Func,                // void (*arrow)(void); 
    Enter_Dig_In_Func,                // void (*enter)(void);
  },
  { // instellen digitale ingangen
    3,                                // nr
    INDEX_DIG_IN_WATER,               // index
    &board_flag,                      // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_dig_in_water_disp,            // display
    &disp_cursor_150_52_142,          // cursor
    &key_pulsen_per_eenheid,          // *value
    Number_Value,                     // void (*number)(void); 
    Arrow_Dig_In_Pulsen_Per_Eenheid_Value, // void (*arrow)(void); 
    Increment_Func_Index,             // void (*enter)(void);
  },
  { // instellen digitale ingangen
    3,                                // nr
    INDEX_DIG_IN_WATER+1,             // index
    &board_flag,                      // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_dig_in_water_disp,            // display
    &disp_cursor_192_78_8,            // cursor
    &key_pulsen,                      // *value
    Number_Value,                     // void (*number)(void); 
    Arrow_Dig_In_Pulsen_Value,        // void (*arrow)(void); 
    Enter_Dig_In_Pulsen_Value,        // void (*enter)(void);
  },
  { // instellen digitale ingangen
    3,                                // nr
    INDEX_DIG_IN_VOER,                // index
    &board_flag,                      // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_dig_in_voer_disp,             // display
    &disp_cursor_150_52_142,          // cursor
    &key_pulsen_per_eenheid,          // *value
    Number_Value,                     // void (*number)(void); 
    Arrow_Dig_In_Pulsen_Per_Eenheid_Value, // void (*arrow)(void); 
    Increment_Func_Index,             // void (*enter)(void);
  },
  { // instellen digitale ingangen
    3,                                // nr
    INDEX_DIG_IN_VOER+1,              // index
    &board_flag,                      // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_dig_in_voer_disp,             // display
    &disp_cursor_192_78_8,            // cursor
    &key_pulsen,                      // *value
    Number_Value,                     // void (*number)(void); 
    Arrow_Dig_In_Pulsen_Value,        // void (*arrow)(void); 
    Enter_Dig_In_Pulsen_Value,        // void (*enter)(void);
  },
  { // instellen digitale ingangen
    3,                                // nr
    INDEX_DIG_IN_EI,                  // index
    &board_flag,                      // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_dig_in_ei_disp,               // display
    &disp_cursor_150_52_142,          // cursor
    &key_pulsen_per_eenheid,          // *value
    Number_Value,                     // void (*number)(void); 
    Arrow_Dig_In_Pulsen_Per_Eenheid_Value, // void (*arrow)(void); 
    Increment_Func_Index,             // void (*enter)(void);
  },
  { // instellen digitale ingangen
    3,                                // nr
    INDEX_DIG_IN_EI+1,                // index
    &board_flag,                      // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_dig_in_ei_disp,               // display
    &disp_cursor_192_78_8,            // cursor
    &key_pulsen,                      // *value
    Number_Value,                     // void (*number)(void); 
    Arrow_Dig_In_Pulsen_Value,        // void (*arrow)(void); 
    Enter_Dig_In_Pulsen_Value,        // void (*enter)(void);
  },
  { // instellen digitale ingangen
    3,                                     // nr
    INDEX_DIG_IN_KWH,                      // index
    &board_flag,                           // option
    (unsigned char *)&option_index_0,      // Optie index 
    lcd_dig_in_kWh_disp,                   // display
    &disp_cursor_150_52_142,               // cursor
    &key_pulsen_per_eenheid,               // *value
    Number_Value,                          // void (*number)(void); 
    Arrow_Dig_In_Pulsen_Per_Eenheid_Value, // void (*arrow)(void); 
    Increment_Func_Index,                  // void (*enter)(void);
  },
  { // instellen digitale ingangen
    3,                                     // nr
    INDEX_DIG_IN_KWH+1,                    // index
    &board_flag,                           // option
    (unsigned char *)&option_index_0,      // Optie index 
    lcd_dig_in_kWh_disp,                   // display
    &disp_cursor_192_78_8,                 // cursor
    &key_pulsen,                           // *value
    Number_Value,                          // void (*number)(void); 
    Arrow_Dig_In_Pulsen_Value,             // void (*arrow)(void); 
    Enter_Dig_In_Pulsen_Value,             // void (*enter)(void);
  },
  { // instellen analoge uitgangen
    4,                                // nr
    INDEX_ANA_OUT_ALG,                // index
    &board_flag,                      // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_ana_out_disp,                 // display
    &disp_cursor,                     // cursor
    &key_IO,                          // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Ana_Out_Func,               // void (*arrow)(void); 
    Enter_Ana_Out_Func,               // void (*enter)(void);
  },
  { // instellen analoge uitgangen 0..5V / 0..20mA
    4,                                // nr
    INDEX_ANA_OUT_ALG+1,              // index
    &board_flag,                      // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_ana_out_sel_disp,             // display
    &disp_cursor_219_78_63,           // cursor
    &key_IO_ana_out_sel,              // *value
    Number_Value,                     // void (*number)(void); 
    Arrow_Ana_Out_Sel_Value,          // void (*arrow)(void); 
    Enter_Ana_Out_Sel_Value,          // void (*enter)(void);
  },
  { // instellen analoge uitgangen min waarde
    4,                                // nr
    INDEX_ANA_OUT_ALG+2,              // index
    &board_flag,                      // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_ana_out_set_disp,             // display
    &disp_cursor_192_52_8,            // cursor
    &key_IO_ana_out_min,              // *value
    Number_Value,                     // void (*number)(void); 
    Arrow_Ana_Out_Min_Value,          // void (*arrow)(void); 
    Enter_Ana_Out_Min_Value,          // void (*enter)(void);
  },
  { // instellen analoge uitgangen max waarde
    4,                                // nr
    INDEX_ANA_OUT_ALG+3,              // index
    &board_flag,                      // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_ana_out_set_disp,             // display
    &disp_cursor_192_78_8,            // cursor
    &key_IO_ana_out_max,              // *value
    Number_Value,                     // void (*number)(void); 
    Arrow_Ana_Out_Max_Value,          // void (*arrow)(void); 
    Enter_Ana_Out_Max_Value,          // void (*enter)(void);
  },
  { // instellen digitale uitgangen
    5,                                // nr
    INDEX_DIG_OUT_ALG,                // index
    &board_flag,                      // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_dig_out_disp,                 // display
    &disp_cursor,                     // cursor
    &key_IO,                          // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Dig_Out_Func,               // void (*arrow)(void); 
    Enter_Dig_Out_Func,               // void (*enter)(void);
  },
  { // instellen digitale uitgangen
    5,                                // nr
    INDEX_DIG_OUT_ALG+1,              // index
    &board_flag,                      // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_dig_out_disp,                 // display
    &disp_cursor_221_78_45,           // cursor
    &key_IO_dig_out,                  // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Dig_Out_Value,              // void (*arrow)(void); 
    Enter_Dig_Out_Value,              // void (*enter)(void);
  },
  { // alarm contact 
    6,                                // nr
    0,                                // index
    &board_flag,                      // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_alarm_contact_disp,           // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Alarm_Contact_Func,         // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    6,                                // nr
    1,                                // index
    &board_flag,                      // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_alarm_contact_disp,           // display
    &disp_cursor_checkbox,            // cursor
    &key_alarm_contact,               // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Alarm_Contact_Value,        // void (*arrow)(void); 
    Increment_Func_Index,             // void (*enter)(void);
  },
  {
    7,                                // nr
    0,                                // index
    &copy_flag,                       // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_copy_IO_08_09_1_disp,         // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_08_09_Func,              // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    7,                                // nr
    1,                                // index
    &copy_flag,                       // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_copy_IO_08_09_1_disp,         // display
    &disp_cursor_207_78_8,            // cursor
    &key_lcd_index_IO,                // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Copy_Board_Val,             // void (*arrow)(void); 
    Enter_Copy_Board_Val,             // void (*enter)(void);
  },
  {
    7,                                // nr
    2,                                // index
    &copy_flag,                       // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_copy_IO_08_09_1_disp_1,       // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Copy_Board_OK_Value,        // void (*arrow)(void); 
    Enter_Copy_Board_OK_Value,        // void (*enter)(void);  
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
    Arrow_End_IO_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
};


s_screen screen_opt_IO_08_09_3;
s_screen const screen_opt_IO_08_09_3_default =
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
  &opt_IO_08_09_3_key_action[0], // first_action
  &opt_IO_08_09_3_key_action[sizeof(opt_IO_08_09_3_key_action)/sizeof(s_key_action) - 1], // last action 
  0, // change_flag
  0, // punt in normaal display
  0, // punt tijdens invoeren
  0, // aantal digits ingevoerd (punt telt niet mee)
  0, // value
  0, // min value
  0, // max value
  &screen_opt_IO_2,  // vorige scherm
  0  // prev_next_func
};

void Control_Screen_Option_IO_08_09_3(void)
{
  index_IO = screen_ptr->nr - DISP_IO_08_09;
  if (password_enabled & PASSWORD_ENABLED_OPT_MASK)
    start_flag = end_flag = 1;
  else
    start_flag = end_flag = 0;
  board_flag = (index_IO < IO_08_09_MAX) ? 1 : 0;
  copy_flag  = (board_flag && end_flag)  ? 1 : 0;
  copy_index = 0;
  Control_Screen(&screen_opt_IO_08_09_3, &screen_opt_IO_08_09_3_default, 1, 1);
}

//-----------------------------------------------------------------------------
static void Goto_Prev_Screen(void)
{
  if (option_change_flag)
  {
    option_change_flag_alg = 1;
    option_change_flag = 0;
  }
  Control_Screen_Option_IO_2();
  Prev_Screen();
  while (screen_ptr->nr != index_IO + DISP_IO_08_09)
    Increment_Func();
}


//*****************************************************************************
static void Arrow_Board_Func(void)
{
  switch (key)
  {
    case UP:
      Decrement_Func();
      break;
    case DOWN:
      Copy_Dig_In_To_Array();     
      Increment_Func();
      break;
    case LEFT:
      if ((password_enabled & PASSWORD_ENABLED_OPT_MASK) == 0)
        Goto_Prev_Screen();
      break;
    case RIGHT:
      if (password_enabled & PASSWORD_ENABLED_OPT_MASK)
        Increment_Func_Index();
      break;
  }
}

static void Enter_Board_Value(void)
{
  if (screen_ptr->value != index_IO)
  {
    index_IO_new = screen_ptr->value;
    Increment_Func_Index();
  }
  else
    Decrement_Func_Index();
}

static void Arrow_Board_Value(void)
{
  switch (key)
  {
    case LEFT:
      Enter_Board_Value();
      break;
    case RIGHT:
      Enter_Board_Value();
      break;
    case UP:
      while (1)
      {
        screen_ptr->value++;
        if (screen_ptr->value > screen_ptr->max_value)
          screen_ptr->value = screen_ptr->min_value;
        screen_ptr->change_flag = 1;
        if (screen_ptr->value == IO_08_09_MAX)
          return;
        else if (index_IO == screen_ptr->value)
          return;
        else if ((opt_io.IO_08_09[screen_ptr->value].board_component.option & 0x000F) == 0)
          return;
      }
    case DOWN:  
      while (1)
      {
        screen_ptr->value--;
        if (screen_ptr->value < screen_ptr->min_value)
          screen_ptr->value = screen_ptr->max_value;
        screen_ptr->change_flag = 1;
        if (screen_ptr->value == IO_08_09_MAX)
          return;
        else if (index_IO == screen_ptr->value)
          return;
        else if ((opt_io.IO_08_09[screen_ptr->value].board_component.option & 0x000F) == 0)
          return;
      }
  }
}

static void Arrow_Board_OK_Value(void)
{
  switch (key)
  {
    case LEFT:
      Increment_Func_Index();
      break;
    case RIGHT:
      Increment_Func_Index();
      break;
    case UP:
      switch (index_IO)
      {
        default:
          if (index_IO_new == IO_08_09_MAX)
          {
            // wis gegevens van IO modules
            opt_io.IO_08_09[index_IO] = default_opt_io.IO_08_09[0];
            opt_io.IO_08_09[index_IO].board_component.option = 0;
            index_IO = index_IO_new;
            board_flag = 0;
			copy_flag  = (board_flag && end_flag)  ? 1 : 0;
            IO_08_09_init_switch[index_IO] = 1;
            Refresh_Screen_Nr_Aantal();
          }
          else
          {
            // copieer gegevens van IO module oud naar module nieuw en wis oude module
            opt_io.IO_08_09[index_IO_new] = opt_io.IO_08_09[index_IO];
            opt_io.IO_08_09[index_IO] = default_opt_io.IO_08_09[0];
            opt_io.IO_08_09[index_IO].board_component.option = 0;
            IO_08_09_init_switch[index_IO_new] = 1;
            IO_08_09_init_switch[index_IO] = 1;
            index_IO = index_IO_new;
          }
          break;
        case IO_08_09_MAX:
          // maak nieuw module aan
          index_IO = index_IO_new;
          opt_io.IO_08_09[index_IO].board_component.option = 1;
          board_flag = 1;
		  copy_flag  = (board_flag && end_flag)  ? 1 : 0;
          IO_08_09_init_switch[index_IO] = 1;
          Refresh_Screen_Nr_Aantal();
          break;
      }
      option_change_flag = 1;
      Increment_Func_Index();
      break;
    case DOWN:  
      Increment_Func_Index();
      break;
  }
}

//*****************************************************************************
static void Get_Dig_In_Pulsen(void)
{
  pulsen = opt_io.IO_08_09[index_IO].digital_input[index_array - 1].pulses_per_count;
  if (pulsen)
  {
    pulsen_per_eenheid = 1;
  }
  else
  {
    pulsen_per_eenheid = 0;
    pulsen = opt_io.IO_08_09[index_IO].digital_input[index_array - 1].counts_per_puls;
  }
}

static void Set_Dig_In_Pulsen(void)
{
  if (pulsen_per_eenheid)
  {
    opt_io.IO_08_09[index_IO].digital_input[index_array - 1].counts_per_puls = 0;
    opt_io.IO_08_09[index_IO].digital_input[index_array - 1].pulses_per_count = pulsen;
  }
  else
  {
    opt_io.IO_08_09[index_IO].digital_input[index_array - 1].counts_per_puls = pulsen;
    opt_io.IO_08_09[index_IO].digital_input[index_array - 1].pulses_per_count = 0;
  }
  value.IO_08_09[index_IO].digital_input[index_array - 1].ctrl |= 0x0004;
}

static void Arrow_Dig_In_Pulsen_Per_Eenheid_Value(void)
{
  switch (key)
  {
    case UP:
      Increment_Scroll_Option_Value();
      break;
    case DOWN:
      Decrement_Scroll_Option_Value();
      break;
    case LEFT:
      Set_Dig_In_Pulsen();
      Goto_Function_Index(INDEX_DIG_IN_ALG);
      break;
    case RIGHT:
      Increment_Func_Index();
      break;
  }
}

static void Enter_Dig_In_Pulsen_Value(void)
{
  if (screen_ptr->change_flag)
  {
    if (Enter_Value())
      option_change_flag = 1;
  }
  Set_Dig_In_Pulsen();
  Goto_Function_Index(INDEX_DIG_IN_ALG);
}

static void Arrow_Dig_In_Pulsen_Value(void)
{
  switch (key)
  {
    case UP:
      Increment_Option_Value();
      break;
    case DOWN:
      Decrement_Option_Value();
      break;
    case LEFT:
      if (Left_Value())
        Decrement_Func_Index();
      break;
    case RIGHT:
      Enter_Dig_In_Pulsen_Value();
      break;
  }
}

//*****************************************************************************
static void Set_Value_Unit_IO_Dig_In(void)
{
  // set unit
  Set_Unit_IO_Dig_In();
  // set value
  disp_value_IO.x = 192;
  disp_value_IO.y = 75;
  disp_value_IO.size = (SIZE_14 | RECHTS);
  disp_value_IO.type = INT;

  disp_string_IO.x = 192;
  disp_string_IO.y = 75;
  disp_string_IO.tekst_array = tekst_inst_aan_uit_14;
  disp_string_IO.type = INT;
  disp_string_IO.func = Disp_Draw_Tekst_Array_R;
  if (index_array)
  {
    switch (array[index_array - 1])
    {
      case DIG_IN_EMPTY:        
        disp_value_IO.value = &int_0;
        disp_value_IO.point = 0;
        disp_value_IO.func = Disp_Draw_Code;
        disp_string_IO.max = 3;
        disp_string_IO.index = &int_2;
        break;
      case DIG_IN_WATER:
        disp_value_IO.value = &val_hr_alg.IO_08_09[index_IO].digital_input[index_array - 1].value;
        disp_value_IO.point = 0;
        disp_value_IO.func = Disp_Draw_Value;
        disp_string_IO.max = 3;
        disp_string_IO.index = &int_2;
        break;
      case DIG_IN_VOER_PULS:
        disp_value_IO.value = &val_hr_alg.IO_08_09[index_IO].digital_input[index_array - 1].value;
        disp_value_IO.point = 0;
        disp_value_IO.func = Disp_Draw_Value;
        disp_string_IO.max = 3;
        disp_string_IO.index = &int_2;
        break;
      case DIG_IN_VOERWEGER:
        disp_value_IO.value = &int_0;
        disp_value_IO.point = 0;
        disp_value_IO.func = Disp_Draw_Code;
        disp_string_IO.max = 3;
        disp_string_IO.index = &val_hr_alg.IO_08_09[index_IO].digital_input[index_array - 1].value;
        break;
      case DIG_IN_EI_PULS:
        disp_value_IO.value = &val_hr_alg.IO_08_09[index_IO].digital_input[index_array - 1].value;
        disp_value_IO.point = 0;
        disp_value_IO.func = Disp_Draw_Value;
        disp_string_IO.max = 3;
        disp_string_IO.index = &int_2;
        break;
      case DIG_IN_EI:
        disp_value_IO.value = &int_0;
        disp_value_IO.point = 0;
        disp_value_IO.func = Disp_Draw_Code;
        disp_string_IO.max = 3;
        disp_string_IO.index = &val_hr_alg.IO_08_09[index_IO].digital_input[index_array - 1].value;
        break;
      case DIG_IN_VOER:
        disp_value_IO.value = &int_0;
        disp_value_IO.point = 0;
        disp_value_IO.func = Disp_Draw_Code;
        disp_string_IO.max = 3;
        disp_string_IO.index = &val_hr_alg.IO_08_09[index_IO].digital_input[index_array - 1].value;
        break;
      case DIG_IN_KLOK:
        disp_value_IO.value = &int_0;
        disp_value_IO.point = 0;
        disp_value_IO.func = Disp_Draw_Code;
        disp_string_IO.max = 3;
        disp_string_IO.index = &val_hr_alg.IO_08_09[index_IO].digital_input[index_array - 1].value;
        break;
      case DIG_IN_ALARM:
      case DIG_IN_VORST:
        disp_value_IO.value  = &int_0;
        disp_value_IO.point  = 0;
        disp_value_IO.func   = Disp_Draw_Code;
        disp_string_IO.max   = 3;
        disp_string_IO.index = &val_hr_alg.IO_08_09[index_IO].digital_input[index_array - 1].value;
        break;
      case DIG_IN_KWH_PULS:
        disp_value_IO.value = &val_hr_alg.IO_08_09[index_IO].digital_input[index_array - 1].value;
        disp_value_IO.point = 0;
        disp_value_IO.func = Disp_Draw_Value;
        disp_string_IO.max = 3;
        disp_string_IO.index = &int_2;
        break;
      case DIG_IN_RAAM:
      case DIG_IN_DOEK:
      case DIG_IN_VENT:
      case DIG_IN_KLEP:
      case DIG_IN_LAMEL:
      case DIG_IN_VERWARMING:
        disp_value_IO.value = &int_0;
        disp_value_IO.point = 0;
        disp_value_IO.func = Disp_Draw_Code;
        disp_string_IO.max = 3;
        disp_string_IO.index = &val_hr_alg.IO_08_09[index_IO].digital_input[index_array - 1].value;
        break;
    }
  }
  else
  {
    disp_value_IO.value = &int_0;
    disp_value_IO.point = 0;
    disp_value_IO.func = Disp_Draw_Code;
    disp_string_IO.max = 3;
    disp_string_IO.index = &int_2;
  }
}

static void Copy_Dig_In_To_Array(void)
{
int loop;
s_option_digital_input *ptr = &opt_io.IO_08_09[index_IO].digital_input[0];

  index_array = 0;
  max_array_index = IO_08_09_DIGITAL_INPUT;
  disp_cursor = disp_cursor_array[index_array];
  disp_block = disp_block_array[index_array];

  for (loop = 0; loop < IO_08_09_DIGITAL_INPUT; loop++)
  {
    array[loop] = ptr->opt_type;
    disp_IO[loop].func = Disp_Draw_Bitmap_Array;
    disp_IO[loop].x = 2 + loop * 19;
    disp_IO[loop].y = 40;
    disp_IO[loop].data_array = ico_IO_dig_in_array;
    disp_IO[loop].type = UCHAR;
    disp_IO[loop].index = &array[loop];
    disp_IO[loop].max = DIG_IN_MAX;
    ptr++;
  }
  Set_Value_Unit_IO_Dig_In();
}

static void Copy_Array_To_Dig_In(void)
{
int loop;
s_option_digital_input *ptr = &opt_io.IO_08_09[index_IO].digital_input[0];

  for (loop = 0; loop < IO_08_09_DIGITAL_INPUT; loop++)
  {
    ptr->opt_type = array[loop];
    ptr++;
  }
}

static void Set_Component_Dig_In(void)
{
  switch (array[index_array - 1])
  {
    case DIG_IN_EMPTY: 
      opt_io.IO_08_09[index_IO].digital_input[index_array - 1] = option_digital_input_empty;
      value.IO_08_09[index_IO].digital_input[index_array - 1].ctrl |= 0x0004;
      break;
    case DIG_IN_WATER: // water teller
      opt_io.IO_08_09[index_IO].digital_input[index_array - 1] = option_digital_input_water;
      value.IO_08_09[index_IO].digital_input[index_array - 1].ctrl |= 0x0004;
      break;
    case DIG_IN_VOER_PULS: // voer teller
      opt_io.IO_08_09[index_IO].digital_input[index_array - 1] = option_digital_input_voer_puls;
      value.IO_08_09[index_IO].digital_input[index_array - 1].ctrl |= 0x0004;
      break;
    case DIG_IN_VOERWEGER: // voer sensor opvangbak
      opt_io.IO_08_09[index_IO].digital_input[index_array - 1] = option_digital_input_voerweger;
      value.IO_08_09[index_IO].digital_input[index_array - 1].ctrl |= 0x0004;
      break;
    case DIG_IN_EI_PULS: // eier teller
      opt_io.IO_08_09[index_IO].digital_input[index_array - 1] = option_digital_input_ei_puls;
      value.IO_08_09[index_IO].digital_input[index_array - 1].ctrl |= 0x0004;
      break;
    case DIG_IN_EI: 
      opt_io.IO_08_09[index_IO].digital_input[index_array - 1] = option_digital_input_ei;
      value.IO_08_09[index_IO].digital_input[index_array - 1].ctrl |= 0x0004;
      break;
    case DIG_IN_VOER: // voer sensor voervraag
      opt_io.IO_08_09[index_IO].digital_input[index_array - 1] = option_digital_input_voer;
      value.IO_08_09[index_IO].digital_input[index_array - 1].ctrl |= 0x0004;
      break;
    case DIG_IN_KLOK: // bewaking nest open
      opt_io.IO_08_09[index_IO].digital_input[index_array - 1] = option_digital_input_klok;
      value.IO_08_09[index_IO].digital_input[index_array - 1].ctrl |= 0x0004;
      break;
    case DIG_IN_ALARM:
      opt_io.IO_08_09[index_IO].digital_input[index_array - 1] = option_digital_input_alarm;
      value.IO_08_09[index_IO].digital_input[index_array - 1].ctrl |= 0x0004;
      break;
    case DIG_IN_VORST:
      opt_io.IO_08_09[index_IO].digital_input[index_array - 1] = option_digital_input_vorst;
      value.IO_08_09[index_IO].digital_input[index_array - 1].ctrl |= 0x0004;
      break;
    case DIG_IN_KWH_PULS: // kWh teller
      opt_io.IO_08_09[index_IO].digital_input[index_array - 1] = option_digital_input_kWh_puls;
      value.IO_08_09[index_IO].digital_input[index_array - 1].ctrl |= 0x0004;
      break;
    case DIG_IN_RAAM:
      opt_io.IO_08_09[index_IO].digital_input[index_array - 1] = option_digital_input_raam;
      value.IO_08_09[index_IO].digital_input[index_array - 1].ctrl |= 0x0004;
      break;
    case DIG_IN_DOEK:
      opt_io.IO_08_09[index_IO].digital_input[index_array - 1] = option_digital_input_doek;
      value.IO_08_09[index_IO].digital_input[index_array - 1].ctrl |= 0x0004;
      break;
    case DIG_IN_VENT:
      opt_io.IO_08_09[index_IO].digital_input[index_array - 1] = option_digital_input_vent;
      value.IO_08_09[index_IO].digital_input[index_array - 1].ctrl |= 0x0004;
	  break;
    case DIG_IN_KLEP:
      opt_io.IO_08_09[index_IO].digital_input[index_array - 1] = option_digital_input_klep;
      value.IO_08_09[index_IO].digital_input[index_array - 1].ctrl |= 0x0004;
	  break;
    case DIG_IN_LAMEL:
      opt_io.IO_08_09[index_IO].digital_input[index_array - 1] = option_digital_input_lamel;
      value.IO_08_09[index_IO].digital_input[index_array - 1].ctrl |= 0x0004;
	  break;
    case DIG_IN_VERWARMING:
      opt_io.IO_08_09[index_IO].digital_input[index_array - 1] = option_digital_input_verwarm;
      value.IO_08_09[index_IO].digital_input[index_array - 1].ctrl |= 0x0004;
      break;
  }
}

static void Set_Key_IO_Dig_In(void)
{
  key_IO.type = UCHAR;
  key_IO.max_digits = 1;
  key_IO.value = &array[index_array - 1];
  key_IO.min_value = &key_min;
  key_IO.max_value = &key_max;
  key_min = 0;
  key_max = DIG_IN_MAX - 1;
  Get_Value();
}

static void Inc_Dig_In(void)
{
unsigned int index = screen_ptr->value;

  do
  {
    index++;
    index %= DIG_IN_MAX; 
  }
  while (dig_in_possible[index] == 0);
  screen_ptr->value = index;
}

static void Dec_Dig_In(void)
{
unsigned int index = screen_ptr->value;

  do
  {
    index += DIG_IN_MAX - 1;
    index %= DIG_IN_MAX; 
  }
  while (dig_in_possible[index] == 0);
  screen_ptr->value = index;
}

static void Arrow_Dig_In_Func(void)
{
  switch (key)
  {
    case UP:
      if (index_array == 0)
      {
        Copy_Array_To_Dig_In();
        Decrement_Func();
      }
      else
      {
        if (password_enabled & PASSWORD_ENABLED_OPT_MASK)
        {
          Inc_Dig_In();
          screen_ptr->change_flag = 0;
          array[index_array - 1] = screen_ptr->value;
          Set_Value_Unit_IO_Dig_In();
          Set_Component_Dig_In();
          option_change_flag = 1;
        }
      }
      break;
    case DOWN:
      if (index_array == 0)
      {
        Copy_Array_To_Dig_In();
        Copy_Ana_Out_To_Array();
        Increment_Func();
      }
      else
      {
        if (password_enabled & PASSWORD_ENABLED_OPT_MASK)
        {
          Dec_Dig_In();
          screen_ptr->change_flag = 0;
          array[index_array - 1] = screen_ptr->value;
          Set_Value_Unit_IO_Dig_In();
          Set_Component_Dig_In();
          option_change_flag = 1;
        }
      }
      break;
    case LEFT:
      if ((password_enabled & PASSWORD_ENABLED_OPT_MASK) == 0)
        Goto_Prev_Screen();
      else
      {
        index_array += max_array_index;
        index_array %= max_array_index + 1;
        disp_cursor = disp_cursor_array[index_array];
        disp_block = disp_block_array[index_array];
        if (index_array)
          Set_Key_IO_Dig_In();
        Set_Value_Unit_IO_Dig_In();
      }
      break;
    case RIGHT:
      index_array++;
      index_array %= max_array_index + 1;
      disp_cursor = disp_cursor_array[index_array];
      disp_block = disp_block_array[index_array];
      if (index_array)
        Set_Key_IO_Dig_In();
      Set_Value_Unit_IO_Dig_In();
      break;
  }
}

static void Enter_Dig_In_Func(void)
{
  if ((password_enabled & PASSWORD_ENABLED_OPT_MASK) && index_array)
  {
    switch (array[index_array - 1])
    {
      case DIG_IN_WATER:
        Get_Dig_In_Pulsen();
        Goto_Function_Index(INDEX_DIG_IN_WATER);
        break;
      case DIG_IN_VOER_PULS:
        Get_Dig_In_Pulsen();
        Goto_Function_Index(INDEX_DIG_IN_VOER);
        break;
      case DIG_IN_EI_PULS:
        Get_Dig_In_Pulsen();
        Goto_Function_Index(INDEX_DIG_IN_EI);
        break;
      case DIG_IN_KWH_PULS:
        Get_Dig_In_Pulsen();
        Goto_Function_Index(INDEX_DIG_IN_KWH);
        break;
      default:
        index_array = 0;
        disp_cursor = disp_cursor_array[index_array];
        disp_block = disp_block_array[index_array];
        Set_Value_Unit_IO_Dig_In();
        break;
    }
  }
  else
  {
    index_array = 0;
    disp_cursor = disp_cursor_array[index_array];
    disp_block = disp_block_array[index_array];
    Set_Value_Unit_IO_Dig_In();
  }
}

//*****************************************************************************
static void Disp_Draw_Value_Analog_Output_Voltage(void *s)
{
  if (opt_io.IO_08_09[index_IO].analog_output[index_array - 1].option == 2)
  {
    analog_output_voltage = ((long)Calc_Prop(opt_io.IO_08_09[index_IO].analog_output[index_array - 1].min_in,
                                             opt_io.IO_08_09[index_IO].analog_output[index_array - 1].max_in,
                                             opt_io.IO_08_09[index_IO].analog_output[index_array - 1].min_out,
                                             opt_io.IO_08_09[index_IO].analog_output[index_array - 1].max_out,
                                             val_hr_alg.IO_08_09[index_IO].analog_output[index_array - 1].value) * 400 + 0x3FFF) / 0x7FFF;
  }
  else
  {
    analog_output_voltage = ((long)Calc_Prop(opt_io.IO_08_09[index_IO].analog_output[index_array - 1].min_in,
                                             opt_io.IO_08_09[index_IO].analog_output[index_array - 1].max_in,
                                             opt_io.IO_08_09[index_IO].analog_output[index_array - 1].min_out,
                                             opt_io.IO_08_09[index_IO].analog_output[index_array - 1].max_out,
                                             val_hr_alg.IO_08_09[index_IO].analog_output[index_array - 1].value) * 100 + 0x3FFF) / 0x7FFF;
  }
  Disp_Draw_Value(s);
}

static void Set_Value_Unit_IO_Ana_Out(void)
{
  // set unit
//  Set_Unit_IO_Ana_Out();
  disp_unit_IO.tekst.func = Disp_Draw_Tekst_L;
  disp_unit_IO.tekst.x = 195;
  disp_unit_IO.tekst.y = 75;
  // set value
  disp_value_IO.x = 192;
  disp_value_IO.y = 75;
  disp_value_IO.size = (SIZE_14 | RECHTS);
  disp_value_IO.type = INT;
  if (index_array)
  {
    switch (array[index_array - 1])
    {
      default:
      case ANA_OUT_EMPTY:       
        disp_unit_IO.tekst.tekst = &tekst_leeg;
        disp_value_IO.value = &int_0;
        disp_value_IO.point = 0;
        disp_value_IO.func = Disp_Draw_Code;
        break;
      case ANA_OUT_VENT:
      case ANA_OUT_KLEP:
      case ANA_OUT_VERWARMING:
      case ANA_OUT_LICHT:
      case ANA_OUT_UNI_REG:   
      case ANA_OUT_MESTDROGING:
      case ANA_OUT_EI:
	  case ANA_OUT_LAMEL:
      case ANA_OUT_RAAM:
      case ANA_OUT_DOEK:
        if (opt_io.IO_08_09[index_IO].analog_output[index_array - 1].option == 2)
          disp_unit_IO.tekst.tekst = &tekst_inst.mA_14;
		else
          disp_unit_IO.tekst.tekst = &tekst_inst.V_14;
        disp_value_IO.value = &analog_output_voltage;
        disp_value_IO.point = 1;
        disp_value_IO.func = Disp_Draw_Value_Analog_Output_Voltage;
        break;
    }
  }
  else
  {
    disp_unit_IO.tekst.tekst = &tekst_leeg;
    disp_value_IO.value = &int_0;
    disp_value_IO.point = 0;
    disp_value_IO.func = Disp_Draw_Code;
  }
}

static void Copy_Ana_Out_To_Array(void)
{
int loop;
s_option_analog_output *ptr = &opt_io.IO_08_09[index_IO].analog_output[0];

  index_array = 0;
  max_array_index = IO_08_09_ANALOG_OUTPUT;
  disp_cursor = disp_cursor_array[index_array];
  disp_block = disp_block_array[index_array];

  for (loop = 0; loop < IO_08_09_ANALOG_OUTPUT; loop++)
  {
    array[loop] = ptr->opt_type;
    disp_IO[loop].func = Disp_Draw_Bitmap_Array;
    if (loop < 12)
    {
      disp_IO[loop].x = 2 + loop * 19;
      disp_IO[loop].y = 40;
    }
    else
    {
      disp_IO[loop].x = 2 - 12 * 19 + loop * 19;
      disp_IO[loop].y = 67;
    }
    disp_IO[loop].data_array = ico_IO_ana_out_array;
    disp_IO[loop].type = UCHAR;
    disp_IO[loop].index = &array[loop];
    disp_IO[loop].max = ANA_OUT_MAX;
    ptr++;
  }
  Set_Value_Unit_IO_Ana_Out();
}

static void Copy_Array_To_Ana_Out(void)
{
int loop;
s_option_analog_output *ptr = &opt_io.IO_08_09[index_IO].analog_output[0];

  for (loop = 0; loop < IO_08_09_ANALOG_OUTPUT; loop++)
  {
    ptr->opt_type = array[loop];
    ptr++;
  }
}

static void Set_Component_Ana_Out(void)
{
  switch (array[index_array - 1])
  {
    case ANA_OUT_EMPTY: 
      opt_io.IO_08_09[index_IO].analog_output[index_array - 1] = option_analog_output_empty;
      break;
    case ANA_OUT_VENT:
    case ANA_OUT_KLEP: 
    case ANA_OUT_VERWARMING: 
    case ANA_OUT_LICHT: 
    case ANA_OUT_UNI_REG: 
    case ANA_OUT_MESTDROGING:
    case ANA_OUT_EI:
    case ANA_OUT_LAMEL:
      if (opt_io.IO_08_09[index_IO].analog_output[index_array - 1].opt_type == ANA_OUT_EMPTY)
      {
        opt_io.IO_08_09[index_IO].analog_output[index_array - 1] = option_analog_output_vent;
        opt_io.IO_08_09[index_IO].analog_output[index_array - 1].max_out = 0x3FFF;
      }
      opt_io.IO_08_09[index_IO].analog_output[index_array - 1].min_in   = option_analog_output_vent.min_in;
      opt_io.IO_08_09[index_IO].analog_output[index_array - 1].max_in   = option_analog_output_vent.max_in;
      opt_io.IO_08_09[index_IO].analog_output[index_array - 1].opt_type = array[index_array - 1];
      break;
    case ANA_OUT_RAAM:
    case ANA_OUT_DOEK:
      if (opt_io.IO_08_09[index_IO].analog_output[index_array - 1].opt_type == ANA_OUT_EMPTY)
	  {
        opt_io.IO_08_09[index_IO].analog_output[index_array - 1] = option_analog_output_raam;
	  }
      opt_io.IO_08_09[index_IO].analog_output[index_array - 1].min_in   = option_analog_output_raam.min_in;
      opt_io.IO_08_09[index_IO].analog_output[index_array - 1].max_in   = option_analog_output_raam.max_in;
      opt_io.IO_08_09[index_IO].analog_output[index_array - 1].opt_type = array[index_array - 1];
      break;
  }
  value.IO_08_09[index_IO].analog_output[index_array - 1].ctrl |= 0x0004;
}

static void Set_Key_IO_Ana_Out(void)
{
  key_IO.type = UCHAR;
  key_IO.max_digits = 1;
  key_IO.value = &array[index_array - 1];
  key_IO.min_value = &key_min;
  key_IO.max_value = &key_max;
  key_min = 0;
  key_max = ANA_OUT_MAX - 1;
  Get_Value();
}

static void Inc_Ana_Out(void)
{
unsigned int index = screen_ptr->value;

  do
  {
    index++;
    index %= ANA_OUT_MAX;
  }
  while (ana_out_possible[index] == 0);
  screen_ptr->value = index;
}

static void Dec_Ana_Out(void)
{
unsigned int index = screen_ptr->value;

  do
  {
    index += ANA_OUT_MAX - 1;
    index %= ANA_OUT_MAX;
  }
  while (ana_out_possible[index] == 0);
  screen_ptr->value = index;
}

static void Arrow_Ana_Out_Func(void)
{
  switch (key)
  {
    case UP:
      if (index_array == 0)
      {
        Copy_Array_To_Ana_Out();
        Copy_Dig_In_To_Array();     
        Decrement_Func();
      }
      else
      {
        if (password_enabled & PASSWORD_ENABLED_OPT_MASK)
        {
          Inc_Ana_Out();
          screen_ptr->change_flag = 0;
          array[index_array - 1] = screen_ptr->value;
          Set_Value_Unit_IO_Ana_Out();
//          Set_Component_Ana_Out();
          option_change_flag = 1;
        }
      }
      break;
    case DOWN:
      if (index_array == 0)
      {
        Copy_Array_To_Ana_Out();
        Copy_Dig_Out_To_Array();
        Increment_Func();
      }
      else
      {
        if (password_enabled & PASSWORD_ENABLED_OPT_MASK)
        {
          Dec_Ana_Out();
          screen_ptr->change_flag = 0;
          array[index_array - 1] = screen_ptr->value;
          Set_Value_Unit_IO_Ana_Out();
//          Set_Component_Ana_Out();
          option_change_flag = 1;
        }
      }
      break;
    case LEFT:
      if ((password_enabled & PASSWORD_ENABLED_OPT_MASK) == 0)
        Goto_Prev_Screen();
      else
      {
        if (index_array)
          Set_Component_Ana_Out();
        index_array += max_array_index;
        index_array %= max_array_index + 1;
        disp_cursor = disp_cursor_array[index_array];
        disp_block = disp_block_array[index_array];
        if (index_array)
          Set_Key_IO_Ana_Out();
        Set_Value_Unit_IO_Ana_Out();
      }
      break;
    case RIGHT:
      if (index_array)
        Set_Component_Ana_Out();
      index_array++;
      index_array %= max_array_index + 1;
      disp_cursor = disp_cursor_array[index_array];
      disp_block = disp_block_array[index_array];
      if (index_array)
        Set_Key_IO_Ana_Out();
      Set_Value_Unit_IO_Ana_Out();
      break;
  }
}

static void Copy_Ana_Out_To_Min_Max(void)
{
  option_analog_output_help = opt_io.IO_08_09[index_IO].analog_output[index_array - 1];
  value_analog_output_help = val_hr_alg.IO_08_09[index_IO].analog_output[index_array - 1].value;
  opt_io.IO_08_09[index_IO].analog_output[index_array - 1] = option_analog_output_vent;
  opt_io.IO_08_09[index_IO].analog_output[index_array - 1].option = option_analog_output_help.option;
  switch (opt_io.IO_08_09[index_IO].analog_output[index_array - 1].option)
  {
    case 1:
	  analog_output_max = 50;
      analog_output_min_value = ((long)option_analog_output_help.min_out * 100  + 0x3FFF) / 0x7FFF;
      analog_output_max_value = ((long)option_analog_output_help.max_out * 100  + 0x3FFF) / 0x7FFF;
      opt_io.IO_08_09[index_IO].analog_output[index_array - 1].max_in = 100;
      break;
    case 2:
	  analog_output_max = 200;
      analog_output_min_value = ((long)option_analog_output_help.min_out * 400  + 0x3FFF) / 0x7FFF;
      analog_output_max_value = ((long)option_analog_output_help.max_out * 400  + 0x3FFF) / 0x7FFF;
      opt_io.IO_08_09[index_IO].analog_output[index_array - 1].max_in = 400;
      break;
  }
  val_hr_alg.IO_08_09[index_IO].analog_output[index_array - 1].value = analog_output_min_value;
  value.IO_08_09[index_IO].analog_output[index_array - 1].ctrl |= 0x0004;
  install_ana_out_flag = 1;
}

static void Copy_Min_Max_To_Ana_Out(void)
{
  switch (opt_io.IO_08_09[index_IO].analog_output[index_array - 1].option)
  {
    case 1:
      option_analog_output_help.min_out = ((long)analog_output_min_value * 0x7FFF) / 100;
      option_analog_output_help.max_out = ((long)analog_output_max_value * 0x7FFF) / 100;
      break;
    case 2:
      option_analog_output_help.min_out = ((long)analog_output_min_value * 0x7FFF) / 400;
      option_analog_output_help.max_out = ((long)analog_output_max_value * 0x7FFF) / 400;
      break;
  }
  opt_io.IO_08_09[index_IO].analog_output[index_array - 1] = option_analog_output_help;
  val_hr_alg.IO_08_09[index_IO].analog_output[index_array - 1].value = value_analog_output_help;
  value.IO_08_09[index_IO].analog_output[index_array - 1].ctrl |= 0x0004;
  install_ana_out_flag = 0;
}

static void Enter_Ana_Out_Func(void)
{
  if ((password_enabled & PASSWORD_ENABLED_OPT_MASK) && index_array)
  {
    Set_Component_Ana_Out();
    switch (array[index_array - 1])
    {
      case ANA_OUT_VENT:
      case ANA_OUT_KLEP:
      case ANA_OUT_VERWARMING:
      case ANA_OUT_LICHT:
      case ANA_OUT_UNI_REG:
      case ANA_OUT_MESTDROGING:
      case ANA_OUT_EI:
      case ANA_OUT_RAAM:
      case ANA_OUT_DOEK:
	  case ANA_OUT_LAMEL:
        analog_output_option = opt_io.IO_08_09[index_IO].analog_output[index_array - 1].option;
//        Copy_Ana_Out_To_Min_Max();
        Increment_Func_Index();
        break;
      default:
        index_array = 0;
        disp_cursor = disp_cursor_array[index_array];
        disp_block = disp_block_array[index_array];
        Set_Value_Unit_IO_Ana_Out();
        break;
    }
  }
  else
  {
    index_array = 0;
    disp_cursor = disp_cursor_array[index_array];
    disp_block = disp_block_array[index_array];
    Set_Value_Unit_IO_Ana_Out();
  }
}

static void Arrow_Ana_Out_Sel_Value(void)
{
  switch (key)
  {
    case UP:
      Increment_Scroll_Option_Value();
      break;
    case DOWN:
      Decrement_Scroll_Option_Value();
      break;
    case LEFT:
      opt_io.IO_08_09[index_IO].analog_output[index_array - 1].option = analog_output_option;
      Set_Value_Unit_IO_Ana_Out();
      Decrement_Func_Index();
      break;
    case RIGHT:
      opt_io.IO_08_09[index_IO].analog_output[index_array - 1].option = analog_output_option;
      Set_Value_Unit_IO_Ana_Out();
      Copy_Ana_Out_To_Min_Max();
      Increment_Func_Index();
      break;
  }
}

static void Enter_Ana_Out_Sel_Value(void)
{
  if (Enter_Value())
    option_change_flag = 1;
  opt_io.IO_08_09[index_IO].analog_output[index_array - 1].option = analog_output_option;
  Set_Value_Unit_IO_Ana_Out();
  Copy_Ana_Out_To_Min_Max();
  Increment_Func_Index();
}

static void Enter_Ana_Out_Min_Value(void)
{
  if (screen_ptr->change_flag)
  {
    if (Enter_Value())
      option_change_flag = 1;
    val_hr_alg.IO_08_09[index_IO].analog_output[index_array - 1].value = analog_output_min_value;
    value.IO_08_09[index_IO].analog_output[index_array - 1].ctrl |= 0x0001;
  }
  else
  {
    val_hr_alg.IO_08_09[index_IO].analog_output[index_array - 1].value = analog_output_max_value;
    value.IO_08_09[index_IO].analog_output[index_array - 1].ctrl |= 0x0001;
    Increment_Func_Index();
  }
}

static void Arrow_Ana_Out_Min_Value(void)
{
  switch (key)
  {
    case UP:
      Increment_Option_Value();
      val_hr_alg.IO_08_09[index_IO].analog_output[index_array - 1].value = analog_output_min_value;
      value.IO_08_09[index_IO].analog_output[index_array - 1].ctrl |= 0x0001;
      break;
    case DOWN:
      Decrement_Option_Value();
      val_hr_alg.IO_08_09[index_IO].analog_output[index_array - 1].value = analog_output_min_value;
      value.IO_08_09[index_IO].analog_output[index_array - 1].ctrl |= 0x0001;
      break;
    case LEFT:
      Copy_Min_Max_To_Ana_Out();
      Decrement_Func_Index();
      break;
    case RIGHT:
      val_hr_alg.IO_08_09[index_IO].analog_output[index_array - 1].value = analog_output_max_value;
      value.IO_08_09[index_IO].analog_output[index_array - 1].ctrl |= 0x0001;
      Increment_Func_Index();
      break;
  }
}

static void Enter_Ana_Out_Max_Value(void)
{
  if (screen_ptr->change_flag)
  {
    if (Enter_Value())
      option_change_flag = 1;
    val_hr_alg.IO_08_09[index_IO].analog_output[index_array - 1].value = analog_output_max_value;
    value.IO_08_09[index_IO].analog_output[index_array - 1].ctrl |= 0x0001;
  }
  else
  {
    Copy_Min_Max_To_Ana_Out();
    Increment_Func_Index();
  }
}

static void Arrow_Ana_Out_Max_Value(void)
{
  switch (key)
  {
    case UP:
      Increment_Option_Value();
      val_hr_alg.IO_08_09[index_IO].analog_output[index_array - 1].value = analog_output_max_value;
      value.IO_08_09[index_IO].analog_output[index_array - 1].ctrl |= 0x0001;
      break;
    case DOWN:
      Decrement_Option_Value();
      val_hr_alg.IO_08_09[index_IO].analog_output[index_array - 1].value = analog_output_max_value;
      value.IO_08_09[index_IO].analog_output[index_array - 1].ctrl |= 0x0001;
      break;
    case LEFT:
      val_hr_alg.IO_08_09[index_IO].analog_output[index_array - 1].value = analog_output_min_value;
      value.IO_08_09[index_IO].analog_output[index_array - 1].ctrl |= 0x0001;
      Decrement_Func_Index();
      break;
    case RIGHT:
      Copy_Min_Max_To_Ana_Out();
      Increment_Func_Index();
      break;
  }
}
//*****************************************************************************
static void Disp_Draw_Tekst_Option_Dig_Out(void *s)
{
  dig_out_relais = (val_hr_alg.IO_08_09[index_IO].digital_output.value & (0x0001 << (index_array - 1))) ? 1 : 0;
  Disp_Draw_Tekst_Array_L(s);
}

static void Set_Value_Unit_IO_Dig_Out(void)
{
  // set unit
  Set_Unit_IO_Dig_Out();
  // set value
  disp_string_IO.x = 185;
  disp_string_IO.y = 75;
  disp_string_IO.tekst_array = tekst_inst_aan_uit_14;
  disp_string_IO.type = INT;
  disp_string_IO.func = Disp_Draw_Tekst_Option_Dig_Out;
  if (index_array)
  {
    switch (array[index_array - 1])
    {
      default:
      case DIG_OUT_EMPTY:
        disp_string_IO.max = 3;
        disp_string_IO.index = &int_2;
        break;
      case DIG_OUT_VENT:
      case DIG_OUT_KLEP:
      case DIG_OUT_VERWARMING:
      case DIG_OUT_KOELING:
      case DIG_OUT_RV:
      case DIG_OUT_KLOK:
      case DIG_OUT_LICHT:
      case DIG_OUT_UNI_REG:
      case DIG_OUT_TUNNEL:
      case DIG_OUT_WATER:
      case DIG_OUT_VOER:
      case DIG_OUT_VOER_PULS:
      case DIG_OUT_MESTDROGING:
      case DIG_OUT_EI:
      case DIG_OUT_HOPPER:
      case DIG_OUT_ALARM:
      case DIG_OUT_SILO:
      case DIG_OUT_LAMEL:
        disp_string_IO.max = 2;
        disp_string_IO.index = &dig_out_relais;
        break;
    }
  }
  else
  {
    disp_string_IO.max = 3;
    disp_string_IO.index = &int_2;
  }
}

static void Dig_Out_Off_Class(void)
{
int loop;

  for (loop = 0; loop < IO_08_09_DIGITAL_OUTPUT; loop++)
  {
    if (opt_io.IO_08_09[index_IO].digital_output.class_nr[loop])
      val_hr_alg.IO_08_09[index_IO].digital_output.value &= ~((unsigned int)1 << loop);
  }
}

static void Copy_Dig_Out_To_Array(void)
{
int loop;
unsigned char *ptr = &opt_io.IO_08_09[index_IO].digital_output.opt_type[0];

  Dig_Out_Off_Class();
  index_array = 0;
  max_array_index = IO_08_09_DIGITAL_OUTPUT;
  disp_cursor = disp_cursor_array[index_array];
  disp_block = disp_block_array[index_array];

  for (loop = 0; loop < IO_08_09_DIGITAL_OUTPUT; loop++)
  {
    array[loop] = *ptr;
    disp_IO[loop].func = Disp_Draw_Bitmap_Array;
    if (loop < 12)
    {
      disp_IO[loop].x = 2 + loop * 19;
      disp_IO[loop].y = 40;
    }
    else
    {
      disp_IO[loop].x = 2 - 12 * 19 + loop * 19;
      disp_IO[loop].y = 67;
    }
    disp_IO[loop].data_array = ico_IO_dig_out_array;
    disp_IO[loop].type = UCHAR;
    disp_IO[loop].index = &array[loop];
    disp_IO[loop].max = DIG_OUT_MAX;
    ptr++;
  }
  Set_Value_Unit_IO_Dig_Out();
}

static void Copy_Array_To_Dig_Out(void)
{
int loop;
unsigned char *ptr = &opt_io.IO_08_09[index_IO].digital_output.opt_type[0];

  for (loop = 0; loop < IO_08_09_DIGITAL_OUTPUT; loop++)
  {
    *ptr = array[loop];
    ptr++;
  }
}

static void Set_Component_Dig_Out(void)
{
int loop;

  opt_io.IO_08_09[index_IO].digital_output.opt_type[index_array - 1] = array[index_array - 1];
  for (loop = 0; loop < IO_08_09_DIGITAL_OUTPUT; loop++)
  {
    if (opt_io.IO_08_09[index_IO].digital_output.opt_type[loop])
    {
      if (!opt_io.IO_08_09[index_IO].digital_output.option)
      {
        opt_io.IO_08_09[index_IO].digital_output.option = 1;
        value.IO_08_09[index_IO].digital_output.ctrl |= 4;
      }
      return;
    }
  }
  if (opt_io.IO_08_09[index_IO].digital_output.option)
  {
    opt_io.IO_08_09[index_IO].digital_output.option = 0;
    value.IO_08_09[index_IO].digital_output.ctrl |= 4;
  }
}

static void Set_Key_IO_Dig_Out(void)
{
  key_IO.type = UCHAR;
  key_IO.max_digits = 1;
  key_IO.value = &array[index_array - 1];
  key_IO.min_value = &key_min;
  key_IO.max_value = &key_max;
  key_min = 0;
  key_max = DIG_OUT_MAX - 1;
  Get_Value();
}

static void Inc_Dig_Out(void)
{
unsigned int index = screen_ptr->value;

  do
  {
    index++;
    index %= DIG_OUT_MAX;
  }
  while (dig_out_possible[index] == 0);
  screen_ptr->value = index; 
}

static void Dec_Dig_Out(void)
{
unsigned int index = screen_ptr->value;

  do
  {
    index += DIG_OUT_MAX - 1;
    index %= DIG_OUT_MAX;
  }
  while (dig_out_possible[index] == 0);
  screen_ptr->value = index; 
}

static void Arrow_Dig_Out_Func(void)
{
  switch (key)
  {
    case UP:
      if (index_array == 0)
      {
        Copy_Array_To_Dig_Out();
        Copy_Ana_Out_To_Array();        
        Decrement_Func();
      }
      else
      {
        if (password_enabled & PASSWORD_ENABLED_OPT_MASK)
        {
//          Increment_Scroll_Value();
          Inc_Dig_Out(); 
          screen_ptr->change_flag = 0;
          array[index_array - 1] = screen_ptr->value;
          Set_Value_Unit_IO_Dig_Out();
          Set_Component_Dig_Out();
          option_change_flag = 1;
        }
      }
      break;
    case DOWN:
      if (index_array == 0)
      {
        Copy_Array_To_Dig_Out();
        Increment_Func();
        alarm_contact = opt_io.IO_08_09[index_IO].alarm;
      }
      else
      {
        if (password_enabled & PASSWORD_ENABLED_OPT_MASK)
        {
//          Decrement_Scroll_Value();
          Dec_Dig_Out(); 
          screen_ptr->change_flag = 0;
          array[index_array - 1] = screen_ptr->value;
          Set_Value_Unit_IO_Dig_Out();
          Set_Component_Dig_Out();
          option_change_flag = 1;
        }
      }
      break;
    case LEFT:
      if ((password_enabled & PASSWORD_ENABLED_OPT_MASK) == 0)
        Goto_Prev_Screen();
      else
      {
        index_array += max_array_index;
        index_array %= max_array_index + 1;
        disp_cursor = disp_cursor_array[index_array];
        disp_block = disp_block_array[index_array];
        if (index_array)
        {
          Set_Key_IO_Dig_Out();
        }
        Set_Value_Unit_IO_Dig_Out();
      }
      break;
    case RIGHT:
      index_array++;
      index_array %= max_array_index + 1;
      disp_cursor = disp_cursor_array[index_array];
      disp_block = disp_block_array[index_array];
      if (index_array)
      {
        Set_Key_IO_Dig_Out();
      }
      Set_Value_Unit_IO_Dig_Out();
      break;
  }
}

static void Enter_Dig_Out_Func(void)
{
  if ((password_enabled & PASSWORD_ENABLED_OPT_MASK) && index_array)
  {
    switch (array[index_array - 1])
    {
      case DIG_OUT_VENT:
      case DIG_OUT_KLEP:
      case DIG_OUT_VERWARMING:
      case DIG_OUT_KOELING:
      case DIG_OUT_RV:
      case DIG_OUT_KLOK:
      case DIG_OUT_LICHT:
      case DIG_OUT_UNI_REG:
      case DIG_OUT_TUNNEL:
      case DIG_OUT_WATER:
      case DIG_OUT_VOER:
      case DIG_OUT_VOER_PULS:
      case DIG_OUT_MESTDROGING:
      case DIG_OUT_EI:
      case DIG_OUT_HOPPER:
      case DIG_OUT_ALARM:
      case DIG_OUT_SILO:
      case DIG_OUT_LAMEL:
        value_digital_output_test_help = val_hr_alg.IO_08_09[index_IO].digital_output.value;
        install_dig_out_flag = 1;
        Increment_Func_Index();
        break;
      default:
        index_array = 0;
        disp_cursor = disp_cursor_array[index_array];
        disp_block = disp_block_array[index_array];
        Set_Value_Unit_IO_Dig_Out();
        break;
    }
  }
  else
  {
    index_array = 0;
    disp_cursor = disp_cursor_array[index_array];
    disp_block = disp_block_array[index_array];
    Set_Value_Unit_IO_Dig_Out();
  }
}

static void Enter_Dig_Out_Value(void)
{
  val_hr_alg.IO_08_09[index_IO].digital_output.value = value_digital_output_test_help;
  value.IO_08_09[index_IO].digital_output.ctrl |= 0x0001;
  install_dig_out_flag = 0;
  Goto_Function_Index(INDEX_DIG_OUT_ALG);
}

static void Arrow_Dig_Out_Value(void)
{
  switch (key)
  {
    case UP:
      Increment_Scroll_Value();
      if (dig_out_relais)
        val_hr_alg.IO_08_09[index_IO].digital_output.value |= (0x0001 << (index_array - 1));
      else
        val_hr_alg.IO_08_09[index_IO].digital_output.value &= ~(0x0001 << (index_array - 1));
      value.IO_08_09[index_IO].digital_output.ctrl |= 0x0001;
      break;
    case DOWN:
      Decrement_Scroll_Value();
      if (dig_out_relais)
        val_hr_alg.IO_08_09[index_IO].digital_output.value |= (0x0001 << (index_array - 1));
      else
        val_hr_alg.IO_08_09[index_IO].digital_output.value &= ~(0x0001 << (index_array - 1));
      value.IO_08_09[index_IO].digital_output.ctrl |= 0x0001;
      break;
    case LEFT:
      install_dig_out_flag = 0;
      Goto_Function_Index(INDEX_DIG_OUT_ALG);
      break;
    case RIGHT:
      install_dig_out_flag = 0;
      Goto_Function_Index(INDEX_DIG_OUT_ALG);
      break;
  }
}

//*****************************************************************************

static void Arrow_Alarm_Contact_Func(void)
{
  switch (key)
  {
    case UP:
      Copy_Dig_Out_To_Array();    
      Decrement_Func();
      break;
    case DOWN:
      Increment_Func();
      break;
    case LEFT:
      if ((password_enabled & PASSWORD_ENABLED_OPT_MASK) == 0)
        Goto_Prev_Screen();
      break;
    case RIGHT:
      if (password_enabled & PASSWORD_ENABLED_OPT_MASK)      
        Increment_Func_Index();
      break;
  }
}

static void Arrow_Alarm_Contact_Value(void)
{
  Arrow_Scroll_Option_Value();
  opt_io.IO_08_09[index_IO].alarm = alarm_contact;
}

//*****************************************************************************
static void Arrow_IO_08_09_Func(void)
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
      if ((password_enabled & PASSWORD_ENABLED_OPT_MASK) == 0)
        Goto_Prev_Screen();
      break;
    case RIGHT:
      if (password_enabled & PASSWORD_ENABLED_OPT_MASK)      
        Increment_Func_Index();
      break;
  }
}

//*****************************************************************************
// kopieren IO_board instellingen

static unsigned char Next_IO_Board(void)
{
int index;

  if (copy_index == IO_08_09_MAX)
    return(0);
  else
    copy_index++;

  for (index = copy_index; index <= IO_08_09_MAX; index++)
  {
    if ((opt_io.IO_08_09[index-1].board_component.option & 0x000F) == 0)
      return (index);
  }
  return (0);
}

static unsigned char Prev_IO_Board(void)
{
int index;

  if (copy_index == 0)
    copy_index = IO_08_09_MAX;
  else
    copy_index--;
     
  for (index = copy_index; index > 0; index--)
  {
    if((opt_io.IO_08_09[index-1].board_component.option & 0x000F) == 0)
      return (index);
  }
  return (0);
}
  

static void Arrow_Copy_Board_Val(void)
{
  switch (key)
  {
    case UP:
      copy_index = Next_IO_Board();
      break;
    case DOWN:
      copy_index = Prev_IO_Board();
      break;
    case LEFT:
      Decrement_Func_Index();
      break;
    case RIGHT:
      Decrement_Func_Index();
      break;
  }
}

static void Enter_Copy_Board_Val(void)
{
  if (copy_index)
    Increment_Func_Index();
  else
    Decrement_Func_Index();
}

static void Arrow_Copy_Board_OK_Value(void)
{
  Increment_Func_Index();
}

static void Enter_Copy_Board_OK_Value(void)
{
  if (copy_index)
  {
    opt_io.IO_08_09[copy_index-1] = opt_io.IO_08_09[index_IO];
    index_IO = copy_index - 1;
    board_flag = 1;
	copy_flag  = (board_flag && end_flag) ? 1 : 0;
    option_change_flag = 1;
    Control_Screen(&screen_opt_IO_08_09_3, &screen_opt_IO_08_09_3_default, 1, 1);
  }
  else
  {
    Increment_Func_Index();
  }
}

//*****************************************************************************
static void Arrow_End_IO_Func(void)
{
  switch (key)
  {
    case UP:
//      Copy_Dig_Out_To_Array();      
      Decrement_Func();
      break;
    case DOWN:
      break;
    case LEFT:
      Goto_Prev_Screen();
      break;
    case RIGHT:
      break;
  }
}

