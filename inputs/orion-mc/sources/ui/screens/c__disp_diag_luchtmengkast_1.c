// C__DISP_DIAG_LUCHTMENGKAST_1.C

#include "ch_define.h"
#include "ch_const.h"
#include "ch_disp.h"
#include "ch_disp_diag_ebm_1.h"
#include "ch_disp_func.h"
#include "ch_disp_value.h"
#include "ch_key.h"
#include "ch_lcd_bitmap.h"
#include "ch_lcd_pixel.h"
#include "ch_luchtmengkast.h"
#include "ch_string.h"
#include "ch_disp_diag_luchtmengkast_1.h"

#define OFFSET_X (unsigned char)28
#define OFFSET_Y (unsigned char)6

#define MIN_OFFSET -50
#define MAX_OFFSET  50

#define MIN_SETPOINT   0
#define MAX_SETPOINT 100

#define INDEX_OPERATION   0
#define INDEX_BUITENKLEP  1
#define INDEX_INBLAASVENT 2
#define INDEX_VERWARMING  3
#define INDEX_BINNENKLEP  4
#define INDEX_BOVENKLEP   5
#define INDEX_AFBLAASVENT 6

static void Control_Disp_Luchtmengkast(void);
static void Arrow_Diag_Luchtmengkast_Value(void);
static void Enter_Diag_Luchtmengkast_Value(void);

static unsigned char disp_binnenklep;

static unsigned char buitenklep_ico_nr;
static unsigned char binnenklep_ico_nr;
static unsigned char buitenklep_val;
static unsigned char binnenklep_val;
static unsigned char bovenklep_val;
static unsigned char verwarming_val;
static unsigned char inblaasvent_val;
static unsigned char afblaasvent_val;

static unsigned char EditIndex;

//***********************************************************************************************************
// SCHERM OPBOUW
//***********************************************************************************************************
//-----------------------------------------------------------------------------------------------------------
// Edit values
static s_key_value key_diag_luchtmengkast;
static s_disp_cursor cursor_diag_luchtmengkast;
static s_disp_cursor const disp_cursor_operation   = { 42, 30, 37, 2, 11 };
static s_disp_cursor const disp_cursor_buitenklep  = { OFFSET_X +  48, OFFSET_Y + 17, 5, 2, 10 };
static s_disp_cursor const disp_cursor_binnenklep  = { OFFSET_X +  92, OFFSET_Y + 66, 5, 2, 10 };
static s_disp_cursor const disp_cursor_bovenklep   = { OFFSET_X + 184, OFFSET_Y + 40, 5, 2, 10 };
static s_disp_cursor const disp_cursor_verwarming  = { OFFSET_X +  80, OFFSET_Y + 36, 5, 2, 10 };
static s_disp_cursor const disp_cursor_inblaasvent = { OFFSET_X +  97, OFFSET_Y + 17, 5, 2, 10 };
static s_disp_cursor const disp_cursor_afblaasvent = { OFFSET_X + 184, OFFSET_Y + 58, 5, 2, 10 };
//-----------------------------------------------------------------------------------------------------------
// Header
static s_disp_agri_header const disp_header = { Disp_Draw_Agri_Header, FN5, EMPTY, &tekst.Diag_Luchtmengkast_10, HK_GEEN, 0, 3 };

static void * const lcd_disp_header[] = { &disp_header, 0 };
//-----------------------------------------------------------------------------------------------------------
// Frame
static s_disp_bitmap const disp_luchtmengkast_boven_ico  = { Disp_Draw_Bitmap, OFFSET_X +  26, OFFSET_Y +  0, &luchtmengkast_bovenkant_ico };
static s_disp_bitmap const disp_luchtmengkast_onder_ico  = { Disp_Draw_Bitmap, OFFSET_X +  26, OFFSET_Y + 61, &luchtmengkast_onderkant_ico };
static s_disp_bitmap const disp_luchtmengkast_kanaal_ico = { Disp_Draw_Bitmap, OFFSET_X + 105, OFFSET_Y +  0, &luchtmengkast_kanaal_ico    };
static s_disp_func   const disp_luchtmengkast_control    = { Disp_Control_Func, Control_Disp_Luchtmengkast };
//-----------------------------------------------------------------------------------------------------------
// Nummer luchtmengkast
static unsigned char luchtmengkast_nummer;
static unsigned char luchtmengkast_groep;
static s_disp_tekst     const disp_luchtmengkast_haak   = { Disp_Draw_Tekst_L, 20, 14, &tekst_ronde_openings_haak_7 };
static s_disp_value     const disp_luchtmengkast_nummer = { Disp_Draw_Value,   14, 14, (SIZE_10 | RECHTS), UCHAR, 0, &luchtmengkast_nummer };
static s_disp_value_add const disp_luchtmengkast_groep  = { Disp_Draw_Value_Add,       (SIZE_7  | LINKS),  UCHAR, 0, &luchtmengkast_groep  };
//-----------------------------------------------------------------------------
// Bediening
static s_disp_tekst_array const disp_operation_val = { Disp_Draw_Tekst_Array_L, 6, 28, &tekst_auto_hand_uit_7, UCHAR, &val_hr_alg.Luchtmengkast[0].OperationMode, 3 };
static s_key_value        const key_operation_val  = { UCHAR, 1, &val_hr_alg.Luchtmengkast[0].OperationMode, &uchar_0, &uchar_2 };
//-----------------------------------------------------------------------------------------------------------
// Klep
static s_disp_bitmap_array const disp_buitenklep_1_ico = { Disp_Draw_Bitmap_Array, OFFSET_X +  20, OFFSET_Y +  6, bmp_buitenklep_pos, UCHAR, &buitenklep_ico_nr, 8 };
static s_disp_bitmap_array const disp_buitenklep_2_ico = { Disp_Draw_Bitmap_Array, OFFSET_X +  20, OFFSET_Y + 17, bmp_buitenklep_pos, UCHAR, &buitenklep_ico_nr, 8 };
static s_disp_bitmap_array const disp_buitenklep_3_ico = { Disp_Draw_Bitmap_Array, OFFSET_X +  20, OFFSET_Y + 28, bmp_buitenklep_pos, UCHAR, &buitenklep_ico_nr, 8 };
static s_disp_bitmap_array const disp_buitenklep_4_ico = { Disp_Draw_Bitmap_Array, OFFSET_X +  20, OFFSET_Y + 39, bmp_buitenklep_pos, UCHAR, &buitenklep_ico_nr, 8 };
static s_disp_bitmap_array const disp_buitenklep_5_ico = { Disp_Draw_Bitmap_Array, OFFSET_X +  20, OFFSET_Y + 50, bmp_buitenklep_pos, UCHAR, &buitenklep_ico_nr, 8 };
static s_disp_bitmap_array const disp_binnenklep_1_ico = { Disp_Draw_Bitmap_Array, OFFSET_X + 102, OFFSET_Y + 23, bmp_binnenklep_pos, UCHAR, &binnenklep_ico_nr, 8 };
static s_disp_bitmap_array const disp_binnenklep_2_ico = { Disp_Draw_Bitmap_Array, OFFSET_X + 102, OFFSET_Y + 34, bmp_binnenklep_pos, UCHAR, &binnenklep_ico_nr, 8 };
static s_disp_bitmap_array const disp_binnenklep_3_ico = { Disp_Draw_Bitmap_Array, OFFSET_X + 102, OFFSET_Y + 45, bmp_binnenklep_pos, UCHAR, &binnenklep_ico_nr, 8 };
static s_disp_bitmap_array const disp_binnenklep_4_ico = { Disp_Draw_Bitmap_Array, OFFSET_X + 102, OFFSET_Y + 56, bmp_binnenklep_pos, UCHAR, &binnenklep_ico_nr, 8 };
//-----------------------------------------------------------------------------------------------------------
// Buitenklep
static s_disp_value const disp_buitenklep_val  = { Disp_Draw_Value,   OFFSET_X + 48, OFFSET_Y + 15, (SIZE_7 | RECHTS), UCHAR, 0, &buitenklep_val };
static s_disp_tekst const disp_buitenklep_unit = { Disp_Draw_Tekst_L, OFFSET_X + 50, OFFSET_Y + 15, &tekst.perc_7 };
//-----------------------------------------------------------------------------------------------------------
// Binnenklep
static s_disp_value_option_on const disp_binnenklep_val  = { Disp_Draw_Value_Option_On,   OFFSET_X + 92, OFFSET_Y + 64, (SIZE_7 | RECHTS), UCHAR, 0, &binnenklep_val, UCHAR, &disp_binnenklep };
static s_disp_tekst_option_on const disp_binnenklep_unit = { Disp_Draw_Tekst_L_Option_On, OFFSET_X + 94, OFFSET_Y + 64, &tekst.perc_7, UCHAR, &disp_binnenklep };
//-----------------------------------------------------------------------------------------------------------
// Bovenklep
static s_disp_value_option_on  const disp_bovenklep_val  = { Disp_Draw_Value_Option_On,   OFFSET_X + 184, OFFSET_Y + 38, (SIZE_7 | RECHTS), UCHAR, 0, &bovenklep_val, UCHAR, &opt_app.Luchtmengkast[0].BovenklepEnabled };
static s_disp_tekst_option_on  const disp_bovenklep_unit = { Disp_Draw_Tekst_L_Option_On, OFFSET_X + 186, OFFSET_Y + 38, &tekst.perc_7,  UCHAR, &opt_app.Luchtmengkast[0].BovenklepEnabled };
static s_disp_bitmap_option_on const disp_bovenklep_ico  = { Disp_Draw_Bitmap_Option_On,  OFFSET_X + 154, OFFSET_Y + 28, &bovenklep_ico, UCHAR, &opt_app.Luchtmengkast[0].BovenklepEnabled };
//-----------------------------------------------------------------------------------------------------------
// Verwarming
static s_disp_value_option_on  const disp_verwarming_val  = { Disp_Draw_Value_Option_On,   OFFSET_X + 80, OFFSET_Y + 34, (SIZE_7 | RECHTS), UCHAR, 0, &verwarming_val, UCHAR, &opt_app.Luchtmengkast[0].VerwarmingEnabled };
static s_disp_tekst_option_on  const disp_verwarming_unit = { Disp_Draw_Tekst_L_Option_On, OFFSET_X + 82, OFFSET_Y + 34, &tekst.perc_7,     UCHAR, &opt_app.Luchtmengkast[0].VerwarmingEnabled };
static s_disp_bitmap_option_on const disp_verwarming_ico  = { Disp_Draw_Bitmap_Option_On,  OFFSET_X + 50, OFFSET_Y + 25, &verwarm_9x10_ico, UCHAR, &opt_app.Luchtmengkast[0].VerwarmingEnabled };
//-----------------------------------------------------------------------------------------------------------
// Meng temperatuur
static s_disp_value_option_on       const disp_mengtemp_val  = { Disp_Draw_Value_Option_On,         OFFSET_X + 80, OFFSET_Y + 48, (SIZE_7 | RECHTS), INT, 1, &val_hr_alg.Luchtmengkast[0].Mengtemp, UCHAR, &opt_app.Luchtmengkast[0].AnaInMengtemp.board_type };
static s_disp_tekst_array_option_on const disp_mengtemp_unit = { Disp_Draw_Tekst_Array_L_Option_On, OFFSET_X + 82, OFFSET_Y + 48, tekst_graden, UCHAR, &temp_unit, 3, UCHAR, &opt_app.Luchtmengkast[0].AnaInMengtemp.board_type };
static s_disp_bitmap_option_on      const disp_mengtemp_ico  = { Disp_Draw_Bitmap_Option_On,        OFFSET_X + 52, OFFSET_Y + 39, &temp_5x10_ico, UCHAR, &opt_app.Luchtmengkast[0].AnaInMengtemp.board_type };
//-----------------------------------------------------------------------------------------------------------
// Inblaas temperatuur
static s_disp_value_option_on       const disp_inblaastemp_val  = { Disp_Draw_Value_Option_On,         OFFSET_X + 136, OFFSET_Y + 15, (SIZE_7 | RECHTS), INT, 1, &val_hr_alg.Luchtmengkast[0].Inblaastemp, UCHAR, &opt_app.Luchtmengkast[0].AnaInInblaastemp.board_type };
static s_disp_tekst_array_option_on const disp_inblaastemp_unit = { Disp_Draw_Tekst_Array_L_Option_On, OFFSET_X + 138, OFFSET_Y + 15, tekst_graden, UCHAR, &temp_unit, 3, UCHAR, &opt_app.Luchtmengkast[0].AnaInInblaastemp.board_type };
//-----------------------------------------------------------------------------------------------------------
// Buitentemperatuur
//static s_disp_value_option_on        const disp_lk_buiten_val    = { Disp_Draw_Value_Option_On,         OFFSET_X +  5, OFFSET_Y + 45, (SIZE_7 | RECHTS), INT, 1, &value_hr.buitentemp_act, UCHAR, &option.buitentemp };
//static s_disp_tekst_array_option_on  const disp_lk_buiten_unit   = { Disp_Draw_Tekst_Array_L_Option_On, OFFSET_X +  7, OFFSET_Y + 45, tekst_graden,  UCHAR, &option.temp_unit, 3, UCHAR, &option.buitentemp };
//static s_disp_bitmap_array_option_on const disp_lk_dag_nacht_ico = { Disp_Draw_Bitmap_Array_Option_On,  OFFSET_X - 15, OFFSET_Y + 22, dag_nacht_ico, UCHAR, &value_hr.mestdrogingen.klok_dag_nacht.status, 2, UCHAR, &option.mestdroging.dag_nacht };
//-----------------------------------------------------------------------------------------------------------
// Inblaas ventilator
static s_disp_value const disp_inblaasvent_val    = { Disp_Draw_Value,        OFFSET_X + 97, OFFSET_Y + 15, (SIZE_7 | RECHTS), UCHAR, 0, &inblaasvent_val };
static s_disp_value const disp_inblaasvent_offset = { Disp_Draw_Value_Signed, OFFSET_X + 97, OFFSET_Y + 26, (SIZE_7 | RECHTS),  CHAR, 0, &val_hr_alg.Luchtmengkast[0].Inblaasvent.Offset };
static s_disp_tekst const disp_inblaasvent_unit   = { Disp_Draw_Tekst_L,      OFFSET_X + 99, OFFSET_Y + 15, &tekst.perc_7 };
//-----------------------------------------------------------------------------------------------------------
// Afblaas ventilator
static s_disp_value_option_on  const disp_afblaasvent_val    = { Disp_Draw_Value_Option_On,   OFFSET_X + 184, OFFSET_Y + 56, (SIZE_7 | RECHTS), UCHAR, 0, &afblaasvent_val, UCHAR, &opt_app.Luchtmengkast[0].AfblaasventEnabled };
static s_disp_value            const disp_afblaasvent_offset = { Disp_Draw_Value_Signed,      OFFSET_X + 184, OFFSET_Y + 67, (SIZE_7 | RECHTS),  CHAR, 0, &val_hr_alg.Luchtmengkast[0].Afblaasvent.Offset };
static s_disp_tekst_option_on  const disp_afblaasvent_unit   = { Disp_Draw_Tekst_L_Option_On, OFFSET_X + 186, OFFSET_Y + 56, &tekst.perc_7,   UCHAR, &opt_app.Luchtmengkast[0].AfblaasventEnabled };
static s_disp_bitmap_option_on const disp_afblaasvent_ico    = { Disp_Draw_Bitmap_Option_On,  OFFSET_X + 153, OFFSET_Y + 46, &vent_13x13_ico, UCHAR, &opt_app.Luchtmengkast[0].AfblaasventEnabled };
//-----------------------------------------------------------------------------------------------------------
// Vorst beveiliging
static s_disp_bitmap_option_on const disp_vorst_ico = { Disp_Draw_Bitmap_Option_On, OFFSET_X + 0, OFFSET_Y + 30, &ico_IO_koel, UCHAR, &alarm_hr_alg.Luchtmengkast[0].Vorst };
//-----------------------------------------------------------------------------------------------------------
// Extern algemeen alarm
static s_disp_bitmap_option_on const disp_extern_alarm_ico = { Disp_Draw_Bitmap_Option_On, OFFSET_X + 152, OFFSET_Y + 62, &ico_IO_alarm, UCHAR, &alarm_hr_alg.Luchtmengkast[0].Extern };
//-----------------------------------------------------------------------------------------------------------
// Luchtmengkast
static void * const lcd_luchtmengkast_disp[] =
{
  &disp_luchtmengkast_boven_ico, &disp_luchtmengkast_onder_ico, &disp_luchtmengkast_kanaal_ico,
  &disp_luchtmengkast_control,
  &disp_luchtmengkast_nummer, &disp_luchtmengkast_haak, &disp_luchtmengkast_groep, &disp_ronde_sluit_haak_7_L,
  &disp_operation_val,
  &disp_buitenklep_1_ico, &disp_buitenklep_2_ico, &disp_buitenklep_3_ico, &disp_buitenklep_4_ico, &disp_buitenklep_5_ico,
  &disp_binnenklep_1_ico, &disp_binnenklep_2_ico, &disp_binnenklep_3_ico, &disp_binnenklep_4_ico,
  &disp_inblaastemp_val,  &disp_inblaastemp_unit,
  &disp_mengtemp_val,     &disp_mengtemp_unit, &disp_mengtemp_ico,
  &disp_inblaasvent_val,  &disp_inblaasvent_unit,
  &disp_buitenklep_val,   &disp_buitenklep_unit,
  &disp_binnenklep_val,   &disp_binnenklep_unit,
  &disp_verwarming_ico,   &disp_verwarming_val,  &disp_verwarming_unit,
  &disp_bovenklep_ico,    &disp_bovenklep_val,   &disp_bovenklep_unit,
  &disp_afblaasvent_ico,  &disp_afblaasvent_val, &disp_afblaasvent_unit,
  &disp_vorst_ico, &disp_extern_alarm_ico,
//  &disp_buitenentemp_val, &disp_buitentemp_unit,
  0
};


//=============================================================================
s_key_action const diag_luchtmengkast_1_key_action[] =
{
  { // Luchtmengkast 1
    1,                                 // nr
    0,                                 // index
    &opt_app.Luchtmengkast[0].Enabled, // option
    (unsigned char *)&option_index_0,  // Optie index 
    lcd_luchtmengkast_disp,            // display
    0,                                 // cursor
    &dummy_value,                      // *value
    Dummy_Func,                        // void (*number)(void); 
    Arrow_Change_Func,                 // void (*arrow)(void);
    Dummy_Func,                        // void (*enter)(void);
  },
  {
    1,                                 // nr
    1,                                 // index
    &opt_app.Luchtmengkast[0].Enabled, // option
    (unsigned char *)&option_index_0,  // Optie index 
    lcd_luchtmengkast_disp,            // display
    &cursor_diag_luchtmengkast,        // cursor
    &key_diag_luchtmengkast,           // *value
    Dummy_Func,                        // void (*number)(void); 
    Arrow_Diag_Luchtmengkast_Value,    // void (*arrow)(void);
    Enter_Diag_Luchtmengkast_Value,    // void (*enter)(void);
  },
  { // Luchtmengkast 2
    2,                                 // nr
    0,                                 // index
    &opt_app.Luchtmengkast[0].Enabled, // option
    (unsigned char *)&option_index_1,  // Optie index 
    lcd_luchtmengkast_disp,            // display
    0,                                 // cursor
    &dummy_value,                      // *value
    Dummy_Func,                        // void (*number)(void); 
    Arrow_Change_Func,                 // void (*arrow)(void);
    Dummy_Func,                        // void (*enter)(void);
  },
  {
    2,                                 // nr
    1,                                 // index
    &opt_app.Luchtmengkast[0].Enabled, // option
    (unsigned char *)&option_index_1,  // Optie index 
    lcd_luchtmengkast_disp,            // display
    &cursor_diag_luchtmengkast,        // cursor
    &key_diag_luchtmengkast,           // *value
    Dummy_Func,                        // void (*number)(void); 
    Arrow_Diag_Luchtmengkast_Value,    // void (*arrow)(void);
    Enter_Diag_Luchtmengkast_Value,    // void (*enter)(void);
  },
  { // Luchtmengkast 3
    3,                                 // nr
    0,                                 // index
    &opt_app.Luchtmengkast[0].Enabled, // option
    (unsigned char *)&option_index_2,  // Optie index 
    lcd_luchtmengkast_disp,            // display
    0,                                 // cursor
    &dummy_value,                      // *value
    Dummy_Func,                        // void (*number)(void); 
    Arrow_Change_Func,                 // void (*arrow)(void);
    Dummy_Func,                        // void (*enter)(void);
  },
  {
    3,                                 // nr
    1,                                 // index
    &opt_app.Luchtmengkast[0].Enabled, // option
    (unsigned char *)&option_index_2,  // Optie index 
    lcd_luchtmengkast_disp,            // display
    &cursor_diag_luchtmengkast,        // cursor
    &key_diag_luchtmengkast,           // *value
    Dummy_Func,                        // void (*number)(void); 
    Arrow_Diag_Luchtmengkast_Value,    // void (*arrow)(void);
    Enter_Diag_Luchtmengkast_Value,    // void (*enter)(void);
  },
  { // Luchtmengkast 4
    4,                                 // nr
    0,                                 // index
    &opt_app.Luchtmengkast[0].Enabled, // option
    (unsigned char *)&option_index_3,  // Optie index 
    lcd_luchtmengkast_disp,            // display
    0,                                 // cursor
    &dummy_value,                      // *value
    Dummy_Func,                        // void (*number)(void); 
    Arrow_Change_Func,                 // void (*arrow)(void);
    Dummy_Func,                        // void (*enter)(void);
  },
  {
    4,                                 // nr
    1,                                 // index
    &opt_app.Luchtmengkast[0].Enabled, // option
    (unsigned char *)&option_index_3,  // Optie index 
    lcd_luchtmengkast_disp,            // display
    &cursor_diag_luchtmengkast,        // cursor
    &key_diag_luchtmengkast,           // *value
    Dummy_Func,                        // void (*number)(void); 
    Arrow_Diag_Luchtmengkast_Value,    // void (*arrow)(void);
    Enter_Diag_Luchtmengkast_Value,    // void (*enter)(void);
  },
  { // Luchtmengkast 5
    5,                                 // nr
    0,                                 // index
    &opt_app.Luchtmengkast[0].Enabled, // option
    (unsigned char *)&option_index_4,  // Optie index 
    lcd_luchtmengkast_disp,            // display
    0,                                 // cursor
    &dummy_value,                      // *value
    Dummy_Func,                        // void (*number)(void); 
    Arrow_Change_Func,                 // void (*arrow)(void);
    Dummy_Func,                        // void (*enter)(void);
  },
  {
    5,                                 // nr
    1,                                 // index
    &opt_app.Luchtmengkast[0].Enabled, // option
    (unsigned char *)&option_index_4,  // Optie index 
    lcd_luchtmengkast_disp,            // display
    &cursor_diag_luchtmengkast,        // cursor
    &key_diag_luchtmengkast,           // *value
    Dummy_Func,                        // void (*number)(void); 
    Arrow_Diag_Luchtmengkast_Value,    // void (*arrow)(void);
    Enter_Diag_Luchtmengkast_Value,    // void (*enter)(void);
  },
  { // Luchtmengkast 6
    6,                                 // nr
    0,                                 // index
    &opt_app.Luchtmengkast[0].Enabled, // option
    (unsigned char *)&option_index_5,  // Optie index 
    lcd_luchtmengkast_disp,            // display
    0,                                 // cursor
    &dummy_value,                      // *value
    Dummy_Func,                        // void (*number)(void); 
    Arrow_Change_Func,                 // void (*arrow)(void);
    Dummy_Func,                        // void (*enter)(void);
  },
  {
    6,                                 // nr
    1,                                 // index
    &opt_app.Luchtmengkast[0].Enabled, // option
    (unsigned char *)&option_index_5,  // Optie index 
    lcd_luchtmengkast_disp,            // display
    &cursor_diag_luchtmengkast,        // cursor
    &key_diag_luchtmengkast,           // *value
    Dummy_Func,                        // void (*number)(void); 
    Arrow_Diag_Luchtmengkast_Value,    // void (*arrow)(void);
    Enter_Diag_Luchtmengkast_Value,    // void (*enter)(void);
  },
  { // Luchtmengkast 7
    7,                                 // nr
    0,                                 // index
    &opt_app.Luchtmengkast[0].Enabled, // option
    (unsigned char *)&option_index_6,  // Optie index 
    lcd_luchtmengkast_disp,            // display
    0,                                 // cursor
    &dummy_value,                      // *value
    Dummy_Func,                        // void (*number)(void); 
    Arrow_Change_Func,                 // void (*arrow)(void);
    Dummy_Func,                        // void (*enter)(void);
  },
  {
    7,                                 // nr
    1,                                 // index
    &opt_app.Luchtmengkast[0].Enabled, // option
    (unsigned char *)&option_index_6,  // Optie index 
    lcd_luchtmengkast_disp,            // display
    &cursor_diag_luchtmengkast,        // cursor
    &key_diag_luchtmengkast,           // *value
    Dummy_Func,                        // void (*number)(void); 
    Arrow_Diag_Luchtmengkast_Value,    // void (*arrow)(void);
    Enter_Diag_Luchtmengkast_Value,    // void (*enter)(void);
  },
  { // Luchtmengkast 8
    8,                                 // nr
    0,                                 // index
    &opt_app.Luchtmengkast[0].Enabled, // option
    (unsigned char *)&option_index_7,  // Optie index 
    lcd_luchtmengkast_disp,            // display
    0,                                 // cursor
    &dummy_value,                      // *value
    Dummy_Func,                        // void (*number)(void); 
    Arrow_Change_Func,                 // void (*arrow)(void);
    Dummy_Func,                        // void (*enter)(void);
  },
  {
    8,                                 // nr
    1,                                 // index
    &opt_app.Luchtmengkast[0].Enabled, // option
    (unsigned char *)&option_index_7,  // Optie index 
    lcd_luchtmengkast_disp,            // display
    &cursor_diag_luchtmengkast,        // cursor
    &key_diag_luchtmengkast,           // *value
    Dummy_Func,                        // void (*number)(void); 
    Arrow_Diag_Luchtmengkast_Value,    // void (*arrow)(void);
    Enter_Diag_Luchtmengkast_Value,    // void (*enter)(void);
  },
  { // Luchtmengkast 9
    9,                                 // nr
    0,                                 // index
    &opt_app.Luchtmengkast[0].Enabled, // option
    (unsigned char *)&option_index_8,  // Optie index 
    lcd_luchtmengkast_disp,            // display
    0,                                 // cursor
    &dummy_value,                      // *value
    Dummy_Func,                        // void (*number)(void); 
    Arrow_Change_Func,                 // void (*arrow)(void);
    Dummy_Func,                        // void (*enter)(void);
  },
  {
    9,                                 // nr
    1,                                 // index
    &opt_app.Luchtmengkast[0].Enabled, // option
    (unsigned char *)&option_index_8,  // Optie index 
    lcd_luchtmengkast_disp,            // display
    &cursor_diag_luchtmengkast,        // cursor
    &key_diag_luchtmengkast,           // *value
    Dummy_Func,                        // void (*number)(void); 
    Arrow_Diag_Luchtmengkast_Value,    // void (*arrow)(void);
    Enter_Diag_Luchtmengkast_Value,    // void (*enter)(void);
  },
  { // Luchtmengkast 10
    10,                                // nr
    0,                                 // index
    &opt_app.Luchtmengkast[0].Enabled, // option
    (unsigned char *)&option_index_9,  // Optie index 
    lcd_luchtmengkast_disp,            // display
    0,                                 // cursor
    &dummy_value,                      // *value
    Dummy_Func,                        // void (*number)(void); 
    Arrow_Change_Func,                 // void (*arrow)(void);
    Dummy_Func,                        // void (*enter)(void);
  },
  {
    10,                                // nr
    1,                                 // index
    &opt_app.Luchtmengkast[0].Enabled, // option
    (unsigned char *)&option_index_9,  // Optie index 
    lcd_luchtmengkast_disp,            // display
    &cursor_diag_luchtmengkast,        // cursor
    &key_diag_luchtmengkast,           // *value
    Dummy_Func,                        // void (*number)(void); 
    Arrow_Diag_Luchtmengkast_Value,    // void (*arrow)(void);
    Enter_Diag_Luchtmengkast_Value,    // void (*enter)(void);
  },
  { // Luchtmengkast 11
    11,                                // nr
    0,                                 // index
    &opt_app.Luchtmengkast[0].Enabled, // option
    (unsigned char *)&option_index_10, // Optie index 
    lcd_luchtmengkast_disp,            // display
    0,                                 // cursor
    &dummy_value,                      // *value
    Dummy_Func,                        // void (*number)(void); 
    Arrow_Change_Func,                 // void (*arrow)(void);
    Dummy_Func,                        // void (*enter)(void);
  },
  {
    11,                                // nr
    1,                                 // index
    &opt_app.Luchtmengkast[0].Enabled, // option
    (unsigned char *)&option_index_10, // Optie index 
    lcd_luchtmengkast_disp,            // display
    &cursor_diag_luchtmengkast,        // cursor
    &key_diag_luchtmengkast,           // *value
    Dummy_Func,                        // void (*number)(void); 
    Arrow_Diag_Luchtmengkast_Value,    // void (*arrow)(void);
    Enter_Diag_Luchtmengkast_Value,    // void (*enter)(void);
  },
  { // Luchtmengkast 12
    12,                                // nr
    0,                                 // index
    &opt_app.Luchtmengkast[0].Enabled, // option
    (unsigned char *)&option_index_11, // Optie index 
    lcd_luchtmengkast_disp,            // display
    0,                                 // cursor
    &dummy_value,                      // *value
    Dummy_Func,                        // void (*number)(void); 
    Arrow_Change_Func,                 // void (*arrow)(void);
    Dummy_Func,                        // void (*enter)(void);
  },
  {
    12,                                // nr
    1,                                 // index
    &opt_app.Luchtmengkast[0].Enabled, // option
    (unsigned char *)&option_index_11, // Optie index 
    lcd_luchtmengkast_disp,            // display
    &cursor_diag_luchtmengkast,        // cursor
    &key_diag_luchtmengkast,           // *value
    Dummy_Func,                        // void (*number)(void); 
    Arrow_Diag_Luchtmengkast_Value,    // void (*arrow)(void);
    Enter_Diag_Luchtmengkast_Value,    // void (*enter)(void);
  },
  { // Luchtmengkast 13
    13,                                // nr
    0,                                 // index
    &opt_app.Luchtmengkast[0].Enabled, // option
    (unsigned char *)&option_index_12, // Optie index 
    lcd_luchtmengkast_disp,            // display
    0,                                 // cursor
    &dummy_value,                      // *value
    Dummy_Func,                        // void (*number)(void); 
    Arrow_Change_Func,                 // void (*arrow)(void);
    Dummy_Func,                        // void (*enter)(void);
  },
  {
    13,                                // nr
    1,                                 // index
    &opt_app.Luchtmengkast[0].Enabled, // option
    (unsigned char *)&option_index_12, // Optie index 
    lcd_luchtmengkast_disp,            // display
    &cursor_diag_luchtmengkast,        // cursor
    &key_diag_luchtmengkast,           // *value
    Dummy_Func,                        // void (*number)(void); 
    Arrow_Diag_Luchtmengkast_Value,    // void (*arrow)(void);
    Enter_Diag_Luchtmengkast_Value,    // void (*enter)(void);
  },
  { // Luchtmengkast 14
    14,                                // nr
    0,                                 // index
    &opt_app.Luchtmengkast[0].Enabled, // option
    (unsigned char *)&option_index_13, // Optie index 
    lcd_luchtmengkast_disp,            // display
    0,                                 // cursor
    &dummy_value,                      // *value
    Dummy_Func,                        // void (*number)(void); 
    Arrow_Change_Func,                 // void (*arrow)(void);
    Dummy_Func,                        // void (*enter)(void);
  },
  {
    14,                                // nr
    1,                                 // index
    &opt_app.Luchtmengkast[0].Enabled, // option
    (unsigned char *)&option_index_13, // Optie index 
    lcd_luchtmengkast_disp,            // display
    &cursor_diag_luchtmengkast,        // cursor
    &key_diag_luchtmengkast,           // *value
    Dummy_Func,                        // void (*number)(void); 
    Arrow_Diag_Luchtmengkast_Value,    // void (*arrow)(void);
    Enter_Diag_Luchtmengkast_Value,    // void (*enter)(void);
  },
  { // Luchtmengkast 15
    15,                                // nr
    0,                                 // index
    &opt_app.Luchtmengkast[0].Enabled, // option
    (unsigned char *)&option_index_14, // Optie index 
    lcd_luchtmengkast_disp,            // display
    0,                                 // cursor
    &dummy_value,                      // *value
    Dummy_Func,                        // void (*number)(void); 
    Arrow_Change_Func,                 // void (*arrow)(void);
    Dummy_Func,                        // void (*enter)(void);
  },
  {
    15,                                // nr
    1,                                 // index
    &opt_app.Luchtmengkast[0].Enabled, // option
    (unsigned char *)&option_index_14, // Optie index 
    lcd_luchtmengkast_disp,            // display
    &cursor_diag_luchtmengkast,        // cursor
    &key_diag_luchtmengkast,           // *value
    Dummy_Func,                        // void (*number)(void); 
    Arrow_Diag_Luchtmengkast_Value,    // void (*arrow)(void);
    Enter_Diag_Luchtmengkast_Value,    // void (*enter)(void);
  },
  { // Luchtmengkast 16
    16,                                // nr
    0,                                 // index
    &opt_app.Luchtmengkast[0].Enabled, // option
    (unsigned char *)&option_index_15, // Optie index 
    lcd_luchtmengkast_disp,            // display
    0,                                 // cursor
    &dummy_value,                      // *value
    Dummy_Func,                        // void (*number)(void); 
    Arrow_Change_Func,                 // void (*arrow)(void);
    Dummy_Func,                        // void (*enter)(void);
  },
  {
    16,                                // nr
    1,                                 // index
    &opt_app.Luchtmengkast[0].Enabled, // option
    (unsigned char *)&option_index_15, // Optie index 
    lcd_luchtmengkast_disp,            // display
    &cursor_diag_luchtmengkast,        // cursor
    &key_diag_luchtmengkast,           // *value
    Dummy_Func,                        // void (*number)(void); 
    Arrow_Diag_Luchtmengkast_Value,    // void (*arrow)(void);
    Enter_Diag_Luchtmengkast_Value,    // void (*enter)(void);
  },
};

s_screen screen_diag_luchtmengkast_1;
s_screen const screen_diag_luchtmengkast_1_default =
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
  &diag_luchtmengkast_1_key_action[0], // first_action
  &diag_luchtmengkast_1_key_action[sizeof(diag_luchtmengkast_1_key_action)/sizeof(s_key_action) - 1], // last action 
  0, // change_flag
  0,
  0,
  0,
  0,
  0,
  0,
  0
};

//=============================================================================
void Control_Screen_Diag_Luchtmengkast_1(void)
{
  Control_Screen(&screen_diag_luchtmengkast_1, &screen_diag_luchtmengkast_1_default, 1, 1);
}

void If_Exist_Goto_Screen_Diag_Luchtmengkast_1(void)
{
  Control_Screen_Diag_Luchtmengkast_1();
  if (screen_diag_luchtmengkast_1.nr_aantal)
  {
    Next_Screen(&screen_diag_luchtmengkast_1);
    EditIndex = INDEX_OPERATION;
    Control_Disp_Luchtmengkast();
  }
}

//=============================================================================
static void Control_Disp_Luchtmengkast(void)
{
unsigned char index;

  index = screen_ptr->nr - 1;
  luchtmengkast_nummer = index + 1;
  luchtmengkast_groep  = opt_app.Luchtmengkast[index].Groep + 1;

  // operation mode
  if (EditIndex == INDEX_OPERATION)
  {
    cursor_diag_luchtmengkast = disp_cursor_operation;
    key_diag_luchtmengkast    = key_operation_val;
  }
  else
  {
    key_diag_luchtmengkast = dummy_value;
  }

  // buitenklep
  if (EditIndex == INDEX_BUITENKLEP)
  {
    buitenklep_val = val_hr_alg.Luchtmengkast[index].Buitenklep.Setpoint;
    cursor_diag_luchtmengkast = disp_cursor_buitenklep;
  }
  else
  {
    buitenklep_val = val_hr_alg.Luchtmengkast[index].Buitenklep.Actual;
  }

  // binnenklep
  if (EditIndex == INDEX_BINNENKLEP)
  {
    binnenklep_val = val_hr_alg.Luchtmengkast[index].Binnenklep.Setpoint;
    cursor_diag_luchtmengkast = disp_cursor_binnenklep;
  }
  else
  {
    binnenklep_val = val_hr_alg.Luchtmengkast[index].Binnenklep.Actual;
  }

  // bovenklep
  if (EditIndex == INDEX_BOVENKLEP)
  {
    bovenklep_val = val_hr_alg.Luchtmengkast[index].Binnenklep.Setpoint;
    cursor_diag_luchtmengkast = disp_cursor_bovenklep;
  }
  else
  {
    bovenklep_val = val_hr_alg.Luchtmengkast[index].Binnenklep.Actual;
  }

  // inblaasvent
  if (EditIndex == INDEX_INBLAASVENT)
  {
	inblaasvent_val = val_hr_alg.Luchtmengkast[index].Inblaasvent.Setpoint;
    cursor_diag_luchtmengkast = disp_cursor_inblaasvent;
    // offset
    if (val_hr_alg.Luchtmengkast[index].OperationMode == omAuto)
      Disp_Draw_Value_Signed(&disp_inblaasvent_offset);
  }
  else
  {
	inblaasvent_val = val_hr_alg.Luchtmengkast[index].Inblaasvent.Actual;
  }

  // afblaasvent
  if (EditIndex == INDEX_AFBLAASVENT)
  {
    afblaasvent_val = val_hr_alg.Luchtmengkast[index].Afblaasvent.Setpoint;
    cursor_diag_luchtmengkast = disp_cursor_afblaasvent;
    // offset
    if (val_hr_alg.Luchtmengkast[index].OperationMode == omAuto)
      Disp_Draw_Value_Signed(&disp_afblaasvent_offset);
  }
  else
  {
    if (opt_app.Luchtmengkast[index].Afblaasvent.AnaIn.board_type)
      afblaasvent_val = val_hr_alg.Luchtmengkast[index].Afblaasvent.Actual;
    else
      afblaasvent_val = val_hr_alg.Luchtmengkast[index].Afblaasvent.Setpoint;
  }

  // verwarming
  if (EditIndex == INDEX_VERWARMING)
  {
    verwarming_val = val_hr_alg.Luchtmengkast[index].Verwarming.Setpoint;
    cursor_diag_luchtmengkast = disp_cursor_verwarming;
  }
  else
  {
    if (opt_app.Luchtmengkast[index].Verwarming.AnaIn.board_type)
      verwarming_val = val_hr_alg.Luchtmengkast[index].Verwarming.Actual;
    else
      verwarming_val = val_hr_alg.Luchtmengkast[index].Verwarming.Setpoint;
  }

  // frame buitenklep
  if (val_hr_alg.Luchtmengkast[index].Buitenklep.Actual == 100)
    buitenklep_ico_nr = 7;
  else if (val_hr_alg.Luchtmengkast[index].Buitenklep.Actual >= 84)
    buitenklep_ico_nr = 6;
  else if (val_hr_alg.Luchtmengkast[index].Buitenklep.Actual >= 68)
    buitenklep_ico_nr = 5;
  else if (val_hr_alg.Luchtmengkast[index].Buitenklep.Actual >= 51)
    buitenklep_ico_nr = 4;
  else if (val_hr_alg.Luchtmengkast[index].Buitenklep.Actual >= 34)
    buitenklep_ico_nr = 3;
  else if (val_hr_alg.Luchtmengkast[index].Buitenklep.Actual >= 18)
    buitenklep_ico_nr = 2;
  else if (val_hr_alg.Luchtmengkast[index].Buitenklep.Actual >= 1)
    buitenklep_ico_nr = 1;
  else
    buitenklep_ico_nr = 0;

  // frame binnenklep
  if (opt_app.Luchtmengkast[index].TypeKlep == TYPE_KLEP_RECIRCULATIE) // recirculatieklep
  {
    disp_binnenklep = 0;
    binnenklep_ico_nr = 7 - buitenklep_ico_nr;
  }
  else // binnen-/buitenklep
  {
    disp_binnenklep = 1;
    if (val_hr_alg.Luchtmengkast[index].Binnenklep.Actual == 100)
      binnenklep_ico_nr = 7;
    else if (val_hr_alg.Luchtmengkast[index].Binnenklep.Actual >= 84)
      binnenklep_ico_nr = 6;
    else if (val_hr_alg.Luchtmengkast[index].Binnenklep.Actual >= 68)
      binnenklep_ico_nr = 5;
    else if (val_hr_alg.Luchtmengkast[index].Binnenklep.Actual >= 51)
      binnenklep_ico_nr = 4;
    else if (val_hr_alg.Luchtmengkast[index].Binnenklep.Actual >= 34)
      binnenklep_ico_nr = 3;
    else if (val_hr_alg.Luchtmengkast[index].Binnenklep.Actual >= 18)
      binnenklep_ico_nr = 2;
    else if (val_hr_alg.Luchtmengkast[index].Binnenklep.Actual >= 1)
      binnenklep_ico_nr = 1;
    else
      binnenklep_ico_nr = 0;
  }
}

//================================================================================
static void Arrow_Diag_Luchtmengkast_Left(void)
{
unsigned char index = screen_ptr->nr - 1;

  switch (EditIndex)
  {
    case INDEX_OPERATION:
	  Enter_Value();
      Decrement_Func_Index();
      break;
    case INDEX_BUITENKLEP:
      EditIndex = INDEX_OPERATION;
      break;
    case INDEX_INBLAASVENT:
      EditIndex = INDEX_BUITENKLEP;
      break;
    case INDEX_VERWARMING:
      EditIndex = INDEX_INBLAASVENT;
      break;
    case INDEX_BINNENKLEP:
      if (opt_app.Luchtmengkast[index].VerwarmingEnabled)
        EditIndex = INDEX_VERWARMING;
      else
        EditIndex = INDEX_INBLAASVENT;
      break;
    case INDEX_BOVENKLEP:
      if (disp_binnenklep)
        EditIndex = INDEX_BINNENKLEP;
      else if (opt_app.Luchtmengkast[index].VerwarmingEnabled)
        EditIndex = INDEX_VERWARMING;
      else
        EditIndex = INDEX_INBLAASVENT;
      break;
    case INDEX_AFBLAASVENT:
      if (opt_app.Luchtmengkast[index].BovenklepEnabled)
        EditIndex = INDEX_BOVENKLEP;
      else if (disp_binnenklep)
        EditIndex = INDEX_BINNENKLEP;
      else if (opt_app.Luchtmengkast[index].VerwarmingEnabled)
        EditIndex = INDEX_VERWARMING;
      else
        EditIndex = INDEX_INBLAASVENT;
      break;
  }
}

static void Arrow_Diag_Luchtmengkast_Right(void)
{
unsigned char index = screen_ptr->nr - 1;

  switch (EditIndex)
  {
    case INDEX_OPERATION:
      Enter_Value();
      EditIndex = INDEX_BUITENKLEP;
      break;
    case INDEX_BUITENKLEP:
      EditIndex = INDEX_INBLAASVENT;
      break;
    case INDEX_INBLAASVENT:
      if (opt_app.Luchtmengkast[index].VerwarmingEnabled)
        EditIndex = INDEX_VERWARMING;
      else if (disp_binnenklep)
        EditIndex = INDEX_BINNENKLEP;
      else if (opt_app.Luchtmengkast[index].BovenklepEnabled)
        EditIndex = INDEX_BOVENKLEP;
      else if (opt_app.Luchtmengkast[index].AfblaasventEnabled)
        EditIndex = INDEX_AFBLAASVENT;
      else
      {
        EditIndex = INDEX_OPERATION;
        Increment_Func_Index();
      }
      break;
    case INDEX_VERWARMING:
      if (disp_binnenklep)
        EditIndex = INDEX_BINNENKLEP;
      else if (opt_app.Luchtmengkast[index].BovenklepEnabled)
        EditIndex = INDEX_BOVENKLEP;
      else if (opt_app.Luchtmengkast[index].AfblaasventEnabled)
        EditIndex = INDEX_AFBLAASVENT;
      else
      {
        EditIndex = INDEX_OPERATION;
        Increment_Func_Index();
      }
      break;
    case INDEX_BINNENKLEP:
      if (opt_app.Luchtmengkast[index].BovenklepEnabled)
        EditIndex = INDEX_BOVENKLEP;
      else if (opt_app.Luchtmengkast[index].AfblaasventEnabled)
        EditIndex = INDEX_AFBLAASVENT;
      else
        Increment_Func_Index();
      break;
    case INDEX_BOVENKLEP:
      if (opt_app.Luchtmengkast[index].AfblaasventEnabled)
        EditIndex = INDEX_AFBLAASVENT;
      else
      {
        EditIndex = INDEX_OPERATION;
        Increment_Func_Index();
      }
      break;
    case INDEX_AFBLAASVENT:
      EditIndex = INDEX_OPERATION;
      Increment_Func_Index();
      break;
  }
}

static void Arrow_Diag_Luchtmengkast_Up(void)
{
unsigned char index = screen_ptr->nr - 1;

  switch (EditIndex)
  {
    case INDEX_OPERATION: // AUTO/MANUAL/OFF
      Increment_Scroll_Value_No_Enter();
      break;
    case INDEX_BUITENKLEP:
      if (val_hr_alg.Luchtmengkast[index].OperationMode == omManual)
      {
		if (val_hr_alg.Luchtmengkast[index].Buitenklep.Setpoint < MAX_SETPOINT)
	      val_hr_alg.Luchtmengkast[index].Buitenklep.Setpoint++;
      }
      break;
    case INDEX_INBLAASVENT:
	  if (val_hr_alg.Luchtmengkast[index].OperationMode == omManual)
	  {
		if (val_hr_alg.Luchtmengkast[index].Inblaasvent.Setpoint < MAX_SETPOINT)
		  val_hr_alg.Luchtmengkast[index].Inblaasvent.Setpoint++;
	  }
	  else if (val_hr_alg.Luchtmengkast[index].OperationMode == omAuto)
	  {
		if (val_hr_alg.Luchtmengkast[index].Inblaasvent.Offset < MAX_OFFSET)
		  val_hr_alg.Luchtmengkast[index].Inblaasvent.Offset++;
	  }
      break;
    case INDEX_VERWARMING:
      if (val_hr_alg.Luchtmengkast[index].OperationMode == omManual)
      {
		if (val_hr_alg.Luchtmengkast[index].Verwarming.Setpoint < MAX_SETPOINT)
		  val_hr_alg.Luchtmengkast[index].Verwarming.Setpoint++;
      }
      break;
    case INDEX_BINNENKLEP:
      if (val_hr_alg.Luchtmengkast[index].OperationMode == omManual)
      {
		if (val_hr_alg.Luchtmengkast[index].Binnenklep.Setpoint < MAX_SETPOINT)
		  val_hr_alg.Luchtmengkast[index].Binnenklep.Setpoint++;
      }
      break;
    case INDEX_BOVENKLEP:
      if (val_hr_alg.Luchtmengkast[index].OperationMode == omManual)
      {
		if (val_hr_alg.Luchtmengkast[index].Binnenklep.Setpoint < MAX_SETPOINT)
		  val_hr_alg.Luchtmengkast[index].Binnenklep.Setpoint++;
      }
      break;
    case INDEX_AFBLAASVENT:
      if (val_hr_alg.Luchtmengkast[index].OperationMode == omManual)
      {
		if (val_hr_alg.Luchtmengkast[index].Afblaasvent.Setpoint < MAX_SETPOINT)
		  val_hr_alg.Luchtmengkast[index].Afblaasvent.Setpoint++;
      }
	  else if (val_hr_alg.Luchtmengkast[index].OperationMode == omAuto)
	  {
		if (val_hr_alg.Luchtmengkast[index].Afblaasvent.Offset < MAX_OFFSET)
		  val_hr_alg.Luchtmengkast[index].Afblaasvent.Offset++;
	  }
      break;
  }
}

static void Arrow_Diag_Luchtmengkast_Down(void)
{
unsigned char index = screen_ptr->nr - 1;

  switch (EditIndex)
  {
    case INDEX_OPERATION: // AUTO/MANUAL/OFF
      Decrement_Scroll_Value_No_Enter();
      break;
    case INDEX_BUITENKLEP:
      if (val_hr_alg.Luchtmengkast[index].OperationMode == omManual)
      {
		if (val_hr_alg.Luchtmengkast[index].Buitenklep.Setpoint > MIN_SETPOINT)
		  val_hr_alg.Luchtmengkast[index].Buitenklep.Setpoint--;
      }
      break;
    case INDEX_INBLAASVENT:
      if (val_hr_alg.Luchtmengkast[index].OperationMode == omManual)
	  {
		if (val_hr_alg.Luchtmengkast[index].Inblaasvent.Setpoint > MIN_SETPOINT)
		  val_hr_alg.Luchtmengkast[index].Inblaasvent.Setpoint--;
	  }
	  else if (val_hr_alg.Luchtmengkast[index].OperationMode == omAuto)
      {
	    if (val_hr_alg.Luchtmengkast[index].Inblaasvent.Offset > MIN_OFFSET)
		  val_hr_alg.Luchtmengkast[index].Inblaasvent.Offset--;
	  }
      break;
    case INDEX_VERWARMING:
      if (val_hr_alg.Luchtmengkast[index].OperationMode == omManual)
      {
		if (val_hr_alg.Luchtmengkast[index].Verwarming.Setpoint > MIN_SETPOINT)
		  val_hr_alg.Luchtmengkast[index].Verwarming.Setpoint--;
      }
      break;
    case INDEX_BINNENKLEP:
      if (val_hr_alg.Luchtmengkast[index].OperationMode == omManual)
      {
		if (val_hr_alg.Luchtmengkast[index].Binnenklep.Setpoint > MIN_SETPOINT)
		  val_hr_alg.Luchtmengkast[index].Binnenklep.Setpoint--;
      }
      break;
    case INDEX_BOVENKLEP:
      if (val_hr_alg.Luchtmengkast[index].OperationMode == omManual)
      {
		if (val_hr_alg.Luchtmengkast[index].Binnenklep.Setpoint > MIN_SETPOINT)
		  val_hr_alg.Luchtmengkast[index].Binnenklep.Setpoint--;
      }
      break;
    case INDEX_AFBLAASVENT:
      if (val_hr_alg.Luchtmengkast[index].OperationMode == omManual)
      {
		if (val_hr_alg.Luchtmengkast[index].Afblaasvent.Setpoint > MIN_SETPOINT)
		  val_hr_alg.Luchtmengkast[index].Afblaasvent.Setpoint--;
      }
	  else if (val_hr_alg.Luchtmengkast[index].OperationMode == omAuto)
      {
	    if (val_hr_alg.Luchtmengkast[index].Afblaasvent.Offset > MIN_OFFSET)
		  val_hr_alg.Luchtmengkast[index].Afblaasvent.Offset--;
	  }
      break;
  }
}

static void Arrow_Diag_Luchtmengkast_Value(void)
{
unsigned char OldEditIndex = EditIndex;

  switch (key)
  {
    case LEFT : Arrow_Diag_Luchtmengkast_Left();  break;
    case RIGHT: Arrow_Diag_Luchtmengkast_Right(); break;
    case UP   : Arrow_Diag_Luchtmengkast_Up();    break;
    case DOWN : Arrow_Diag_Luchtmengkast_Down();  break;
  }

  if (OldEditIndex != EditIndex)
    Control_Disp_Luchtmengkast();
}

static void Enter_Diag_Luchtmengkast_Value(void)
{
unsigned char OldEditIndex = EditIndex;
unsigned char index;

  index = screen_ptr->nr - 1;

  switch (EditIndex)
  {
    case INDEX_OPERATION:
      Enter_Value();
      EditIndex = INDEX_BUITENKLEP;
	  break;
    case INDEX_BUITENKLEP:
      break;
    case INDEX_INBLAASVENT:
      If_Exist_Goto_Screen_Diag_Ebm_1(opt_app.Luchtmengkast[index].Inblaasvent.Adres);
      break;
    case INDEX_VERWARMING:
    case INDEX_BINNENKLEP:
    case INDEX_BOVENKLEP:
      break;
    case INDEX_AFBLAASVENT:
      If_Exist_Goto_Screen_Diag_Ebm_1(opt_app.Luchtmengkast[index].Afblaasvent.Adres);
      break;
  }

  if (OldEditIndex != EditIndex)
    Control_Disp_Luchtmengkast();
}





