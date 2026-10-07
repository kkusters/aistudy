// C__DISP_F1_0.C

#include "ch_define.h"

#include "ch_const.h"
#include "ch_disp.h"
#include "ch_disp_func.h"
#include "ch_disp_group_1.h"
#include "ch_disp_kiersturing_1.h"
#include "ch_disp_tijd_1.h"
#include "ch_key.h"
#include "ch_kiersturing.h"
#include "ch_lcd_10.h"
#include "ch_lcd_bitmap.h"
#include "ch_lcd_hardware.h"
#include "ch_lcd_pixel.h"
#include "ch_string.h"
#include "ch_tijd.h"
#include "ch_disp_F1_0.h"

static void Arrow_Group_Func(void);
static void Arrow_Kiersturing_Func(void);
static void Disp_Control_DualScreen(void);
static void Arrow_Tijd_Func(void);

static unsigned char DispInput;

static s_disp_agri_header const disp_header = { Disp_Draw_Agri_Header, FN1, EMPTY, &tekst.Motorgroepen_10, HK_GEEN, 0, 1};
static void * const lcd_disp_header[] = { &disp_header, 0};

static s_tekst_3 const tekst_A_10 = { 3, 15, SIZE_10, "A:" };
static s_tekst_3 const tekst_B_10 = { 3, 15, SIZE_10, "B:" };

static s_tekst_2 const frame_A_7 = { 2, 10, SIZE_7, "A" };
static s_tekst_2 const frame_B_7 = { 2, 10, SIZE_7, "B" };

//*****************************************************************************
// STRING ARRAY'S 
//*****************************************************************************
static void const * const tekst_dagweek[7] =
{
  &tekst.Zondag_10, 
  &tekst.Maandag_10, 
  &tekst.Dinsdag_10, 
  &tekst.Woensdag_10, 
  &tekst.Donderdag_10, 
  &tekst.Vrijdag_10, 
  &tekst.Zaterdag_10, 
};

//*****************************************************************************
// SCHERM OPBOUW
//*****************************************************************************
// Algemeen
static s_disp_tekst const disp_14_perc_0 = { Disp_Draw_Tekst_L, 103, 39, &tekst.perc_7 };
static s_disp_tekst const disp_14_perc_1 = { Disp_Draw_Tekst_L, 215, 36, &tekst.perc_7 };
static s_disp_tekst const disp_14_perc_A = { Disp_Draw_Tekst_L, 212, 44, &tekst.perc_7 };
static s_disp_tekst const disp_14_perc_B = { Disp_Draw_Tekst_L, 212, 64, &tekst.perc_7 };
//-----------------------------------------------------------------------------
// Motorgroep
static void Disp_Control_Group(void);
static void Disp_Control_Ico(void);

static s_disp_func const disp_control     = { Disp_Control_Func, Disp_Control_Group };
static s_disp_func const disp_control_ico = { Disp_Control_Func, Disp_Control_Ico };
static s_bitmap const * const ico_raamstand[] = { &raamstand_00_ico, &raamstand_01_ico, &raamstand_02_ico, &raamstand_03_ico, &raamstand_04_ico, &raamstand_05_ico };
static s_bitmap const * const ico_doekstand[] = { &doekstand_00_ico, &doekstand_01_ico, &doekstand_02_ico, &doekstand_03_ico, &doekstand_04_ico, &doekstand_05_ico };

static s_disp_tekst_array const disp_group_str = { Disp_Draw_Tekst_Array_L, 9, 20, &tekst_groep_14, UCHAR, &opt_app.Motorgroup[0].Number, MAX_GROUP };
static s_disp_value const disp_group_avg_position_val = { Disp_Draw_Value,  100, 51, (SIZE_20 | RECHTS), INT, 1, &Motorgroup[0].PositionAvg };
static s_disp_value const disp_group_str_position_val = { Disp_Draw_Value,  212, 39, (SIZE_10 | RECHTS), INT, 1, &val_hr_alg.Motorgroup[0].PositionPerc };
static s_disp_tekst_array           const disp_group_output_val = { Disp_Draw_Tekst_Array_R,           220, 58, &tekst_auto_hand_uit_10,   UCHAR, &val_hr_alg.Motorgroup[0].OperationMode, 3 };
static s_disp_tekst_array_option_on const disp_group_input_val  = { Disp_Draw_Tekst_Array_R_Option_On, 220, 77, &tekst_stop_open_dicht_10, UCHAR, &Motorgroup[0].RunningMode, 3, UCHAR, &DispInput };
static s_disp_bitmap           const disp_streef_ico = { Disp_Draw_Bitmap,           140, 30, &ico_streef };
static s_disp_bitmap           const disp_output_ico = { Disp_Draw_Bitmap,           140, 46, &output_ico };
static s_disp_bitmap_option_on const disp_input_ico  = { Disp_Draw_Bitmap_Option_On, 140, 65, &input_ico, UCHAR, &DispInput };

static void * const lcd_group_disp[] = 
{
  &disp_control, &disp_control_ico,
  &disp_group_str, &disp_group_avg_position_val, &disp_14_perc_0,
  &disp_streef_ico, &disp_group_str_position_val, &disp_14_perc_1,
  &disp_output_ico, &disp_group_output_val,
  &disp_input_ico,  &disp_group_input_val,
  0
};

static void Disp_Control_Group(void)
{
  if (!opt_alg.can_backbone && (opt_app.Motorgroup[lcd_rel_disp_index].Type != TYPE_VENT))
    DispInput = 1;
  else
    DispInput = 0;
}

static void Disp_Control_Ico(void)
{
unsigned char Bitmap;

  switch (opt_app.Motorgroup[screen_ptr->nr - 1].Type)
  {
    case TYPE_RAAM:
      if (opt_app.Motorgroup[screen_ptr->nr - 1].ControlType == CONTROL_CABRIOKAS)
	  {
	    if (Motorgroup[screen_ptr->nr - 1].PositionAvg > 900)
          LCD_Draw_Bitmap(9, 25, &cabriostand_05_ico);
	    else if (Motorgroup[screen_ptr->nr - 1].PositionAvg > 700)
          LCD_Draw_Bitmap(9, 25, &cabriostand_04_ico);
	    else if (Motorgroup[screen_ptr->nr - 1].PositionAvg > 500)
          LCD_Draw_Bitmap(9, 25, &cabriostand_03_ico);
	    else if (Motorgroup[screen_ptr->nr - 1].PositionAvg > 200)
          LCD_Draw_Bitmap(9, 25, &cabriostand_02_ico);
	    else if (Motorgroup[screen_ptr->nr - 1].PositionAvg > 10)
          LCD_Draw_Bitmap(9, 25, &cabriostand_01_ico);
		else
          LCD_Draw_Bitmap(9, 25, &cabriostand_00_ico);
	  }
      else
      {
	    if (Motorgroup[screen_ptr->nr - 1].PositionAvg < 0)
		  Bitmap = 0;
		else if (Motorgroup[screen_ptr->nr - 1].PositionAvg > 1000)
		  Bitmap = 5;
        else
          Bitmap = ((5 * Motorgroup[screen_ptr->nr - 1].PositionAvg) + 500) / 1000;
        LCD_Draw_Bitmap(9, 25, ico_raamstand[Bitmap]);
      }
      break;
    case TYPE_DOEK:
	  if (Motorgroup[screen_ptr->nr - 1].PositionAvg < 0)
	    Bitmap = 0;
	  else if (Motorgroup[screen_ptr->nr - 1].PositionAvg > 1000)
	    Bitmap = 5;
      else
        Bitmap = ((5 * Motorgroup[screen_ptr->nr - 1].PositionAvg) + 500) / 1000;
      LCD_Draw_Bitmap(9, 25, ico_doekstand[Bitmap]);
      break;
    case TYPE_VENT:
      LCD_Draw_Bitmap(9, 30, &ico_vent);
      break;
    case TYPE_KLEP:
      LCD_Draw_Bitmap(9, 30, &ico_klep_links);
      break;
  }
}

//-----------------------------------------------------------------------------
// 2 Doeken op 1 draadbed
static s_disp_tekst disp_frame_A;
static s_disp_tekst disp_frame_B;
static s_disp_tekst const disp_frame_A_const = { Disp_Draw_Tekst_L, 9, 17, &frame_A_7 };
static s_disp_tekst const disp_frame_B_const = { Disp_Draw_Tekst_L, 9, 17, &frame_B_7 };
static s_disp_tekst const disp_A_str = { Disp_Draw_Tekst_L, 9, 51, &tekst_A_10 };
static s_disp_tekst const disp_B_str = { Disp_Draw_Tekst_L, 9, 71, &tekst_B_10 };
static s_disp_tekst_array const disp_doek_groep_A_str = { Disp_Draw_Tekst_Array_L, 27, 51, &tekst_groep_10, UINT, &opt_app.DualScreen[0].GroupA, MAX_GROUP+1 };
static s_disp_tekst_array const disp_doek_groep_B_str = { Disp_Draw_Tekst_Array_L, 27, 71, &tekst_groep_10, UINT, &opt_app.DualScreen[0].GroupB, MAX_GROUP+1 };
static s_disp_value const disp_doek_position_A_val = { Disp_Draw_Value, 209, 51, (SIZE_14 | RECHTS), INT, 1, &DualScreen[0].PositionA };
static s_disp_value const disp_doek_position_B_val = { Disp_Draw_Value, 209, 71, (SIZE_14 | RECHTS), INT, 1, &DualScreen[0].PositionB };

static s_bitmap const * const ico_doek_array[] =
{
  &doek_00_ico, &doek_01_ico, &doek_02_ico, &doek_03_ico, &doek_04_ico, &doek_05_ico, &doek_06_ico, &doek_07_ico, &doek_08_ico,
  &doek_09_ico, &doek_10_ico, &doek_11_ico, &doek_12_ico, &doek_13_ico, &doek_14_ico, &doek_15_ico, &doek_16_ico, &doek_17_ico,
  &doek_18_ico, &doek_19_ico, &doek_20_ico, &doek_21_ico, &doek_22_ico, &doek_23_ico, &doek_24_ico,
};
static s_disp_bitmap_array const disp_dualscreen_A_ico = { Disp_Draw_Bitmap_Array, 9, 20, ico_doek_array, UCHAR, &DualScreen[0].BitmapA, 25 };
static s_disp_bitmap_array const disp_dualscreen_B_ico = { Disp_Draw_Bitmap_Array, 9, 20, ico_doek_array, UCHAR, &DualScreen[0].BitmapB, 25 };
static s_disp_bitmap_array disp_doek_A_ico;
static s_disp_bitmap_array disp_doek_B_ico;
static s_disp_func const disp_control_dualscreen = { Disp_Control_Func, Disp_Control_DualScreen };

static void * const lcd_2_doek_1_bed_disp[] = 
{
  &disp_control_dualscreen,
  &disp_dualscreen_frame_ico, &disp_frame_A, &disp_frame_B, &disp_doek_A_ico, &disp_doek_B_ico,
  &disp_A_str, &disp_doek_groep_A_str, &disp_doek_position_A_val, &disp_14_perc_A,
  &disp_B_str, &disp_doek_groep_B_str, &disp_doek_position_B_val, &disp_14_perc_B,
  0
};
//-----------------------------------------------------------------------------
// Tijd + Datum
static s_disp_bitmap const disp_tijd0_0        = { Disp_Draw_Bitmap,   9, 29, &ico_tijd };
static s_disp_tekst  const disp_tijd_datum_str = { Disp_Draw_Tekst_L,  9, 20, &tekst.Tijd_en_Datum_14 };
static s_disp_value  const disp_uur_value      = { Disp_Draw_Value,   (unsigned char)82,            51, (SIZE_20 | RECHTS), INT, 0, &tijd.tm_hour };
static s_disp_tekst  const disp_dp_string_0    = { Disp_Draw_Tekst_L, (unsigned char)82+3,          49, &tekst_dp_14 };
static s_disp_value  const disp_min_value      = { Disp_Draw_Value,   (unsigned char)82+3+26,       51, (SIZE_20 | RECHTS), TIME_INT, 0, &tijd.tm_min };
static s_disp_value  const disp_sec_value      = { Disp_Draw_Value,   (unsigned char)82+3+25+15, 51-12, (SIZE_7  | RECHTS), TIME_INT, 0, &tijd.tm_sec };
static s_disp_value     const disp_jaar_value  = { Disp_Draw_Value, 207, 39, (SIZE_10 | RECHTS), INT, 0, &tijd.tm_year };
static s_disp_value_add const disp_maand_value = { Disp_Draw_Value_Add, (SIZE_10 | RECHTS), TIME_INT, 0, &tijd.tm_mon };
static s_disp_value_add const disp_dag_value   = { Disp_Draw_Value_Add, (SIZE_10 | RECHTS), INT, 0, &tijd.tm_mday };
static s_disp_tekst_array const disp_str_dagweek = { Disp_Draw_Tekst_Array_L, 137, 58, &tekst_dagweek, UCHAR, &tijd.tm_wday , 7 };

static void * const lcd_tijd_disp[] = 
{
  &disp_tijd0_0, &disp_tijd_datum_str,
  &disp_uur_value, &disp_dp_string_0, &disp_min_value, &disp_sec_value,
  &disp_str_dagweek,
  &disp_jaar_value, &disp_min_teken_10_R, &disp_maand_value, &disp_min_teken_10_R, &disp_dag_value, 
  0
};
//-----------------------------------------------------------------------------


s_key_action const F1_0_key_action[] =
{
  { // Group 1
    1,                                // nr
    0,                                // index
    &opt_app.Motorgroup[0].Enabled,   // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_group_disp,                   // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Group_Func,                 // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  { // Group 2
    2,                                // nr
    0,                                // index
    &opt_app.Motorgroup[0].Enabled,   // option
    (unsigned char *)&option_index_1, // Optie index 
    lcd_group_disp,                   // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Group_Func,                 // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  { // Group 3
    3,                                // nr
    0,                                // index
    &opt_app.Motorgroup[0].Enabled,   // option
    (unsigned char *)&option_index_2, // Optie index 
    lcd_group_disp,                   // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Group_Func,                 // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  { // Group 4
    4,                                // nr
    0,                                // index
    &opt_app.Motorgroup[0].Enabled,   // option
    (unsigned char *)&option_index_3, // Optie index 
    lcd_group_disp,                   // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Group_Func,                 // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  { // Group 5
    5,                                // nr
    0,                                // index
    &opt_app.Motorgroup[0].Enabled,   // option
    (unsigned char *)&option_index_4, // Optie index 
    lcd_group_disp,                   // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Group_Func,                 // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  { // Group 6
    6,                                // nr
    0,                                // index
    &opt_app.Motorgroup[0].Enabled,   // option
    (unsigned char *)&option_index_5, // Optie index 
    lcd_group_disp,                   // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Group_Func,                 // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  { // Group 7
    7,                                // nr
    0,                                // index
    &opt_app.Motorgroup[0].Enabled,   // option
    (unsigned char *)&option_index_6, // Optie index 
    lcd_group_disp,                   // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Group_Func,                 // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  { // Group 8
    8,                                // nr
    0,                                // index
    &opt_app.Motorgroup[0].Enabled,   // option
    (unsigned char *)&option_index_7, // Optie index 
    lcd_group_disp,                   // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Group_Func,                 // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  { // Group 9
    9,                                // nr
    0,                                // index
    &opt_app.Motorgroup[0].Enabled,   // option
    (unsigned char *)&option_index_8, // Optie index 
    lcd_group_disp,                   // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Group_Func,                 // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  { // Group 10
    10,                               // nr
    0,                                // index
    &opt_app.Motorgroup[0].Enabled,   // option
    (unsigned char *)&option_index_9, // Optie index 
    lcd_group_disp,                   // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Group_Func,                 // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  { // Group 11
    11,                                // nr
    0,                                 // index
    &opt_app.Motorgroup[0].Enabled,    // option
    (unsigned char *)&option_index_10, // Optie index 
    lcd_group_disp,                    // display
    0,                                 // cursor
    &dummy_value,                      // *value
    Dummy_Func,                        // void (*number)(void); 
    Arrow_Group_Func,                  // void (*arrow)(void); 
    Dummy_Func,                        // void (*enter)(void);
  },
  { // Group 12
    12,                                // nr
    0,                                 // index
    &opt_app.Motorgroup[0].Enabled,    // option
    (unsigned char *)&option_index_11, // Optie index 
    lcd_group_disp,                    // display
    0,                                 // cursor
    &dummy_value,                      // *value
    Dummy_Func,                        // void (*number)(void); 
    Arrow_Group_Func,                  // void (*arrow)(void); 
    Dummy_Func,                        // void (*enter)(void);
  },
  { // Group 13
    13,                                // nr
    0,                                 // index
    &opt_app.Motorgroup[0].Enabled,    // option
    (unsigned char *)&option_index_12, // Optie index 
    lcd_group_disp,                    // display
    0,                                 // cursor
    &dummy_value,                      // *value
    Dummy_Func,                        // void (*number)(void); 
    Arrow_Group_Func,                  // void (*arrow)(void); 
    Dummy_Func,                        // void (*enter)(void);
  },
  { // Group 14
    14,                                // nr
    0,                                 // index
    &opt_app.Motorgroup[0].Enabled,    // option
    (unsigned char *)&option_index_13, // Optie index 
    lcd_group_disp,                    // display
    0,                                 // cursor
    &dummy_value,                      // *value
    Dummy_Func,                        // void (*number)(void); 
    Arrow_Group_Func,                  // void (*arrow)(void); 
    Dummy_Func,                        // void (*enter)(void);
  },
  { // Group 15
    15,                                // nr
    0,                                 // index
    &opt_app.Motorgroup[0].Enabled,    // option
    (unsigned char *)&option_index_14, // Optie index 
    lcd_group_disp,                    // display
    0,                                 // cursor
    &dummy_value,                      // *value
    Dummy_Func,                        // void (*number)(void); 
    Arrow_Group_Func,                  // void (*arrow)(void); 
    Dummy_Func,                        // void (*enter)(void);
  },
  { // Group 16
    16,                                // nr
    0,                                 // index
    &opt_app.Motorgroup[0].Enabled,    // option
    (unsigned char *)&option_index_15, // Optie index 
    lcd_group_disp,                    // display
    0,                                 // cursor
    &dummy_value,                      // *value
    Dummy_Func,                        // void (*number)(void); 
    Arrow_Group_Func,                  // void (*arrow)(void); 
    Dummy_Func,                        // void (*enter)(void);
  },
  { // Group 17
    17,                                // nr
    0,                                 // index
    &opt_app.Motorgroup[0].Enabled,    // option
    (unsigned char *)&option_index_16, // Optie index 
    lcd_group_disp,                    // display
    0,                                 // cursor
    &dummy_value,                      // *value
    Dummy_Func,                        // void (*number)(void); 
    Arrow_Group_Func,                  // void (*arrow)(void); 
    Dummy_Func,                        // void (*enter)(void);
  },
  { // Group 18
    18,                                // nr
    0,                                 // index
    &opt_app.Motorgroup[0].Enabled,    // option
    (unsigned char *)&option_index_17, // Optie index 
    lcd_group_disp,                    // display
    0,                                 // cursor
    &dummy_value,                      // *value
    Dummy_Func,                        // void (*number)(void); 
    Arrow_Group_Func,                  // void (*arrow)(void); 
    Dummy_Func,                        // void (*enter)(void);
  },
  { // Group 19
    19,                                // nr
    0,                                 // index
    &opt_app.Motorgroup[0].Enabled,    // option
    (unsigned char *)&option_index_18, // Optie index 
    lcd_group_disp,                    // display
    0,                                 // cursor
    &dummy_value,                      // *value
    Dummy_Func,                        // void (*number)(void); 
    Arrow_Group_Func,                  // void (*arrow)(void); 
    Dummy_Func,                        // void (*enter)(void);
  },
  { // Group 20
    20,                                // nr
    0,                                 // index
    &opt_app.Motorgroup[0].Enabled,    // option
    (unsigned char *)&option_index_19, // Optie index 
    lcd_group_disp,                    // display
    0,                                 // cursor
    &dummy_value,                      // *value
    Dummy_Func,                        // void (*number)(void); 
    Arrow_Group_Func,                  // void (*arrow)(void); 
    Dummy_Func,                        // void (*enter)(void);
  },
  { // Group 21
    21,                                // nr
    0,                                 // index
    &opt_app.Motorgroup[0].Enabled,    // option
    (unsigned char *)&option_index_20, // Optie index 
    lcd_group_disp,                    // display
    0,                                 // cursor
    &dummy_value,                      // *value
    Dummy_Func,                        // void (*number)(void); 
    Arrow_Group_Func,                  // void (*arrow)(void); 
    Dummy_Func,                        // void (*enter)(void);
  },
  { // Group 22
    22,                                // nr
    0,                                 // index
    &opt_app.Motorgroup[0].Enabled,    // option
    (unsigned char *)&option_index_21, // Optie index 
    lcd_group_disp,                    // display
    0,                                 // cursor
    &dummy_value,                      // *value
    Dummy_Func,                        // void (*number)(void); 
    Arrow_Group_Func,                  // void (*arrow)(void); 
    Dummy_Func,                        // void (*enter)(void);
  },
  { // Group 23
    23,                                // nr
    0,                                 // index
    &opt_app.Motorgroup[0].Enabled,    // option
    (unsigned char *)&option_index_22, // Optie index 
    lcd_group_disp,                    // display
    0,                                 // cursor
    &dummy_value,                      // *value
    Dummy_Func,                        // void (*number)(void); 
    Arrow_Group_Func,                  // void (*arrow)(void); 
    Dummy_Func,                        // void (*enter)(void);
  },
  { // Group 24
    24,                                // nr
    0,                                 // index
    &opt_app.Motorgroup[0].Enabled,    // option
    (unsigned char *)&option_index_23, // Optie index 
    lcd_group_disp,                    // display
    0,                                 // cursor
    &dummy_value,                      // *value
    Dummy_Func,                        // void (*number)(void); 
    Arrow_Group_Func,                  // void (*arrow)(void); 
    Dummy_Func,                        // void (*enter)(void);
  },
  { // Group 25
    25,                                // nr
    0,                                 // index
    &opt_app.Motorgroup[0].Enabled,    // option
    (unsigned char *)&option_index_24, // Optie index 
    lcd_group_disp,                    // display
    0,                                 // cursor
    &dummy_value,                      // *value
    Dummy_Func,                        // void (*number)(void); 
    Arrow_Group_Func,                  // void (*arrow)(void); 
    Dummy_Func,                        // void (*enter)(void);
  },
  { // Group 26
    26,                                // nr
    0,                                 // index
    &opt_app.Motorgroup[0].Enabled,    // option
    (unsigned char *)&option_index_25, // Optie index 
    lcd_group_disp,                    // display
    0,                                 // cursor
    &dummy_value,                      // *value
    Dummy_Func,                        // void (*number)(void); 
    Arrow_Group_Func,                  // void (*arrow)(void); 
    Dummy_Func,                        // void (*enter)(void);
  },
  { // Group 27
    27,                                // nr
    0,                                 // index
    &opt_app.Motorgroup[0].Enabled,    // option
    (unsigned char *)&option_index_26, // Optie index 
    lcd_group_disp,                    // display
    0,                                 // cursor
    &dummy_value,                      // *value
    Dummy_Func,                        // void (*number)(void); 
    Arrow_Group_Func,                  // void (*arrow)(void); 
    Dummy_Func,                        // void (*enter)(void);
  },
  { // Group 28
    28,                                // nr
    0,                                 // index
    &opt_app.Motorgroup[0].Enabled,    // option
    (unsigned char *)&option_index_27, // Optie index 
    lcd_group_disp,                    // display
    0,                                 // cursor
    &dummy_value,                      // *value
    Dummy_Func,                        // void (*number)(void); 
    Arrow_Group_Func,                  // void (*arrow)(void); 
    Dummy_Func,                        // void (*enter)(void);
  },
  { // Group 29
    29,                                // nr
    0,                                 // index
    &opt_app.Motorgroup[0].Enabled,    // option
    (unsigned char *)&option_index_28, // Optie index 
    lcd_group_disp,                    // display
    0,                                 // cursor
    &dummy_value,                      // *value
    Dummy_Func,                        // void (*number)(void); 
    Arrow_Group_Func,                  // void (*arrow)(void); 
    Dummy_Func,                        // void (*enter)(void);
  },
  { // Group 30
    30,                                // nr
    0,                                 // index
    &opt_app.Motorgroup[0].Enabled,    // option
    (unsigned char *)&option_index_29, // Optie index 
    lcd_group_disp,                    // display
    0,                                 // cursor
    &dummy_value,                      // *value
    Dummy_Func,                        // void (*number)(void); 
    Arrow_Group_Func,                  // void (*arrow)(void); 
    Dummy_Func,                        // void (*enter)(void);
  },
  { // Group 31
    31,                                // nr
    0,                                 // index
    &opt_app.Motorgroup[0].Enabled,    // option
    (unsigned char *)&option_index_30, // Optie index 
    lcd_group_disp,                    // display
    0,                                 // cursor
    &dummy_value,                      // *value
    Dummy_Func,                        // void (*number)(void); 
    Arrow_Group_Func,                  // void (*arrow)(void); 
    Dummy_Func,                        // void (*enter)(void);
  },
  { // Group 32
    32,                                // nr
    0,                                 // index
    &opt_app.Motorgroup[0].Enabled,    // option
    (unsigned char *)&option_index_31, // Optie index 
    lcd_group_disp,                    // display
    0,                                 // cursor
    &dummy_value,                      // *value
    Dummy_Func,                        // void (*number)(void); 
    Arrow_Group_Func,                  // void (*arrow)(void); 
    Dummy_Func,                        // void (*enter)(void);
  },
  { // 2 Doeken op 1 draadbed - 1
    41,                               // nr
    0,                                // index
    &opt_app.DualScreen[0].Enabled,   // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_2_doek_1_bed_disp,            // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Kiersturing_Func,           // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  { // 2 Doeken op 1 draadbed - 2
    42,                               // nr
    0,                                // index
    &opt_app.DualScreen[0].Enabled,   // option
    (unsigned char *)&option_index_1, // Optie index 
    lcd_2_doek_1_bed_disp,            // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Kiersturing_Func,           // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  { // 2 Doeken op 1 draadbed - 3
    43,                               // nr
    0,                                // index
    &opt_app.DualScreen[0].Enabled,   // option
    (unsigned char *)&option_index_2, // Optie index 
    lcd_2_doek_1_bed_disp,            // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Kiersturing_Func,           // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  { // 2 Doeken op 1 draadbed - 4
    44,                               // nr
    0,                                // index
    &opt_app.DualScreen[0].Enabled,   // option
    (unsigned char *)&option_index_3, // Optie index 
    lcd_2_doek_1_bed_disp,            // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Kiersturing_Func,           // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  { // 2 Doeken op 1 draadbed - 5
    45,                               // nr
    0,                                // index
    &opt_app.DualScreen[0].Enabled,   // option
    (unsigned char *)&option_index_4, // Optie index 
    lcd_2_doek_1_bed_disp,            // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Kiersturing_Func,           // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  { // 2 Doeken op 1 draadbed - 6
    46,                               // nr
    0,                                // index
    &opt_app.DualScreen[0].Enabled,   // option
    (unsigned char *)&option_index_5, // Optie index 
    lcd_2_doek_1_bed_disp,            // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Kiersturing_Func,           // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  { // 2 Doeken op 1 draadbed - 7
    47,                               // nr
    0,                                // index
    &opt_app.DualScreen[0].Enabled,   // option
    (unsigned char *)&option_index_6, // Optie index 
    lcd_2_doek_1_bed_disp,            // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Kiersturing_Func,           // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  { // 2 Doeken op 1 draadbed - 8
    48,                               // nr
    0,                                // index
    &opt_app.DualScreen[0].Enabled,   // option
    (unsigned char *)&option_index_7, // Optie index 
    lcd_2_doek_1_bed_disp,            // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Kiersturing_Func,           // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  { // Tijd + Datum
    300,                              // nr
    0,                                // index
    (unsigned char *)&option_on,      // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_tijd_disp,                    // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Tijd_Func,                  // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
};

s_screen screen_F1_0;
s_screen const screen_F1_0_default =
{
  1, // functie nr
  0, // index
  1, // max rel
  0, // rel actief
  0, // nr actief nummer weergegeven bin aantal ingeschakelde functie nummers
  0, // aantal ingeschakelde functie nummers
  lcd_disp_header, // algemene scherm opmaak
  {0,0,0,0,0,0}, // functies behorende bij functie toetsen
  0,3,19,       // rel[0]
  0,3,19+28,    // rel[1]
  0,3,19+28+28, // rel[2]
  &F1_0_key_action[0], // first_action
  &F1_0_key_action[sizeof(F1_0_key_action)/sizeof(s_key_action) - 1], // last action 
  0, // change_flag
  0, // punt in normaal display
  0, // punt tijdens invoeren
  0, // aantal digits ingevoerd (punt telt niet mee)
  0, // value
  0, // min value
  0, // max value
  0, // vorige scherm
  0  // prev_next_func
};

void Control_Screen_F1_0(void)
{
  Control_Screen(&screen_F1_0, &screen_F1_0_default, 0, 1);
}

//-----------------------------------------------------------------------------
static void Arrow_Group_Func(void)
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
      break;
    case RIGHT:
      If_Exist_Goto_Screen_Group_1(screen_ptr->nr - 1);
      break;
  }
}

//-----------------------------------------------------------------------------
static void Arrow_Kiersturing_Func(void)
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
      break;
    case RIGHT:
      If_Exist_Goto_Screen_Kiersturing_1(screen_ptr->nr - 41);
      break;
  }
}

//-----------------------------------------------------------------------------
static void Disp_Control_DualScreen(void)
{
  disp_frame_A = disp_frame_A_const;
  disp_frame_B = disp_frame_B_const;
  if (opt_app.DualScreen[screen_ptr->nr - 41].CombiMatic)
  {
    disp_doek_A_ico = disp_dualscreen_A_ico;
    disp_doek_B_ico = disp_dualscreen_B_ico;
    disp_doek_A_ico.x += disp_doek_B_ico.data_array[DualScreen[screen_ptr->nr - 41].BitmapB]->width + 1;
    disp_frame_A.x = disp_doek_A_ico.x + (disp_doek_A_ico.data_array[DualScreen[screen_ptr->nr - 41].BitmapA]->width / 2) - 2;
    disp_frame_B.x = disp_doek_B_ico.x + (disp_doek_B_ico.data_array[DualScreen[screen_ptr->nr - 41].BitmapB]->width / 2) - 2;
    if ((DualScreen[screen_ptr->nr - 41].BitmapA <= 1) && (DualScreen[screen_ptr->nr - 41].BitmapB <= 1))
    {
      disp_frame_A.x += 1;
      disp_frame_B.x -= 1;
    }
  }
  else
  {
    disp_doek_A_ico = disp_dualscreen_A_ico;
    disp_doek_B_ico = disp_dualscreen_B_ico;
    disp_doek_B_ico.x += (disp_dualscreen_frame_ico.data->width - disp_doek_B_ico.data_array[DualScreen[screen_ptr->nr - 41].BitmapB]->width);
    disp_frame_A.x = disp_doek_A_ico.x + (disp_doek_A_ico.data_array[DualScreen[screen_ptr->nr - 41].BitmapA]->width / 2) - 2;
    disp_frame_B.x = disp_doek_B_ico.x + (disp_doek_B_ico.data_array[DualScreen[screen_ptr->nr - 41].BitmapB]->width / 2) - 2;
  }
}

//-----------------------------------------------------------------------------
static void Arrow_Tijd_Func(void)
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
      break;
    case RIGHT:
      If_Exist_Goto_Screen_Tijd_1();
      break;
  }
}

