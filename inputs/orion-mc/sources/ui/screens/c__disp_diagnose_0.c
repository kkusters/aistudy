// C__DISP_DIAGNOSE_0.C

#include "ch_define.h"

#include "ch_const.h"
#include "ch_computer.h"
#include "ch_cp2200.h"
#include "ch_disp.h"
#include "ch_disp_diag_asc0_1.h"
#include "ch_disp_diag_asc1_1.h"
#include "ch_disp_diag_can_backbone_1.h"
#include "ch_disp_diag_can_io_1.h"
#include "ch_disp_diag_ethernet_1.h"
#include "ch_disp_diag_info_1.h"
#include "ch_disp_diag_groep_1.h"
#include "ch_disp_diag_luchtmengkast_1.h"
#include "ch_disp_diag_SD_Card_1.h"
#include "ch_disp_func.h"
#include "ch_disp_value.h"
#include "ch_key.h"
#include "ch_lcd_10.h"
#include "ch_lcd_14.h"
#include "ch_lcd_bitmap.h"
#include "ch_lcd_pixel.h"
#include "ch_string.h"
#include "ch_test.h"
#include "ch_disp_diagnose_0.h"

#define DISP_INFO          1
#define DISP_GROEP         (DISP_INFO + 1)
#define DISP_LUCHTMENGKAST (DISP_GROEP + MAX_GROUP)
#define DISP_COM1          (DISP_LUCHTMENGKAST + 1)
#define DISP_COM2          (DISP_COM1 + 1)
#define DISP_ETHERNET      (DISP_COM2 + 1)
#define DISP_CAN_LOCAL     (DISP_ETHERNET + 1)
#define DISP_CAN_BACKBONE  (DISP_CAN_LOCAL + 1)
#define DISP_SD_CARD       (DISP_CAN_BACKBONE + 1)
#define DISP_DISPLAY_COLOR (DISP_SD_CARD + 1)

static void Arrow_Diag_Info_1_Func(void);
static void Arrow_Diag_Groep_1_Func(void);
static void Arrow_Diag_Luchtmengkast_1_Func(void);
static void Arrow_Diag_Com1_Usb_1_Func(void);
static void Arrow_Diag_Com2_1_Func(void);
#ifdef ETHERNET
static void Arrow_Diag_Ethernet_1_Func(void);
static void Arrow_Display_Color_Value(void); 
static void Enter_Display_Color_Value(void);
#endif // ETHERNET
static void Arrow_Diag_Can_Local_1_Func(void);
static void Arrow_Diag_Can_Backbone_1_Func(void);
#ifdef SD_CARD
static void Arrow_Diag_SD_Card_1_Func(void);
#endif // SD_CARD

static void Number_Reset_Test(void);
static void Enter_Reset_Test(void);
static void Arrow_Reset_Test(void);

static s_disp_agri_header const disp_header = { Disp_Draw_Agri_Header, FN5, EMPTY, &tekst.Diagnose_10, HK_GEEN, 0, 1};
static void * const lcd_disp_header[] = { &disp_header, 0};
//-----------------------------------------------------------------------------
// Versie
static s_disp_tekst const disp_computer_soort_type_str = { Disp_Draw_Tekst_L, 43, 20, &tekst_computer_soort_type };

static void * const lcd_info_disp[] = { &disp_globel, &disp_computer_soort_type_control, &disp_computer_soort_type_str, &disp_space_14_L, &disp_add_versie_val, &disp_add_sub_versie_punt, &disp_add_sub_versie_nr, 0 };
//-----------------------------------------------------------------------------
// Motorgroepen
static s_disp_tekst_array const disp_groep_str = { Disp_Draw_Tekst_Array_L, 43, 20, &tekst_groep_14, UCHAR, &opt_app.Motorgroup[0].Number, MAX_GROUP };

static void * const lcd_groep_disp[] = { &disp_groep_ico, &disp_groep_str, 0 };
//-----------------------------------------------------------------------------
// Luchtmengkast
static s_disp_tekst const disp_luchtmengkast_str = { Disp_Draw_Tekst_L, 43, 20, &tekst.Luchtmengkast_14 };

static void * const lcd_luchtmengkast_disp[] = { &disp_luchtmengkast_ico, &disp_luchtmengkast_str, 0 };
//-----------------------------------------------------------------------------
// com1/usb
static s_disp_bitmap_array const disp_com1_bmp = { Disp_Draw_Bitmap_Array, 9, 0, ico_pc_modem_big_array, UCHAR, &opt_alg.com1_modem, 2 };
static s_disp_tekst const disp_com1_usb_str = { Disp_Draw_Tekst_L, 43, 20, &tekst.COM1_USB_14 };
static void * const lcd_com1_usb_disp[] = { &disp_com1_bmp, &disp_com1_usb_str, 0 };
static s_disp_bitmap_array const disp_com2_bmp = { Disp_Draw_Bitmap_Array, 9, 0, ico_pc_modem_big_array, UCHAR, &opt_alg.com2_modem, 2 };
static s_disp_tekst const disp_com2_str = { Disp_Draw_Tekst_L, 43, 20, &tekst.COM2_14 };
static void * const lcd_com2_disp[] = { &disp_com2_bmp, &disp_com2_str, 0 };
#ifdef ETHERNET
// ethernet
static s_disp_tekst const disp_ethernet_str = { Disp_Draw_Tekst_L, 43, 20, &tekst.Ethernet_14 };
static void * const lcd_ethernet_disp[] = { &disp_globe, &disp_ethernet_str, 0 };
static void const * const tekst_groen_wit[]    = { &tekst.Groen_14, &tekst.Wit_14 };
static s_disp_tekst_array const disp_color_val = { Disp_Draw_Tekst_Array_R, 200, 20, &tekst_groen_wit, UCHAR, &cp2200_flash.lcd_color, 2 };
static s_disp_tekst const disp_display_str     = { Disp_Draw_Tekst_L, 43, 20, &tekst.Display_14 };
static void * const lcd_display_color_disp[]  = { &disp_globe, &disp_display_str, &disp_color_val, 0 };
static s_key_value const key_display_color_value = { UCHAR, 1, &cp2200_flash.lcd_color, &uchar_0, &uchar_1 };
s_disp_cursor const disp_cursor_display_color = { 200, 25, 50, 2, 20 };
#endif // ETHERNET
//-----------------------------------------------------------------------------
// CAN LOCAL
static s_disp_tekst const disp_can_local_str    = { Disp_Draw_Tekst_L, 43, 20, &tekst.CAN_LOCAL_14 };
static s_disp_tekst const disp_can_backbone_str = { Disp_Draw_Tekst_L, 43, 20, &tekst.CAN_BACKBONE_14 };

static void * const lcd_can_local_disp[]    = { &disp_can_local, &disp_can_local_str,    0 };
static void * const lcd_can_backbone_disp[] = { &disp_can_local, &disp_can_backbone_str, 0 };
//-----------------------------------------------------------------------------
#ifdef SD_CARD
static s_disp_tekst const disp_sd_card_str = { Disp_Draw_Tekst_L, 43, 20, &tekst.SD_CARD_14 };
static void * const lcd_sd_card_disp[] = { &disp_sd_card, &disp_sd_card_str, 0 };
#endif // SD_CARD

#ifdef TIMING_TEST
static s_disp_value  const disp_test_1_0 = { Disp_Draw_Value,  100,  9, (SIZE_7 | RECHTS), INT, 0, &test_normal };
static s_disp_value  const disp_test_1_1 = { Disp_Draw_Value,  100, 18, (SIZE_7 | RECHTS), INT, 0, &test_middel };
static s_disp_value  const disp_test_1_2 = { Disp_Draw_Value,  100, 27, (SIZE_7 | RECHTS), INT, 0, &test_max };
static void * const lcd_test_disp_0[] =
{
  &disp_test_1_0, &disp_test_1_1, &disp_test_1_2,
 0
};
#endif

#ifdef TEST_H2MC
static s_disp_value  const disp_test_2_0 = { Disp_Draw_Value,  100,  9, (SIZE_7 | RECHTS), INT, 1, &val_hr_alg.IO_H2MC[0].motor_control[0].Value };
static s_disp_value  const disp_test_2_1 = { Disp_Draw_Value,  100, 18, (SIZE_7 | RECHTS), INT, 1, &val_hr_alg.IO_20_33P[0].analog_input[0].value };
static s_disp_value  const disp_test_2_2 = { Disp_Draw_Value,  180,  9, (SIZE_7 | RECHTS), INT, 1, &val_hr_alg.IO_H2MC[0].motor_control[1].Value };
static s_disp_value  const disp_test_2_3 = { Disp_Draw_Value,  180, 18, (SIZE_7 | RECHTS), INT, 1, &val_hr_alg.IO_20_33P[0].analog_input[1].value };
static void * const lcd_test_disp_1[] =
{
  &disp_test_2_0, &disp_test_2_1,
  &disp_test_2_2, &disp_test_2_3,
 0
};
#endif


s_key_action const diagnose_0_key_action[] =
{
  { // Info
    DISP_INFO,                         // nr
    0,                                 // index
    (unsigned char *)&option_on,       // option
    (unsigned char *)&option_index_0,  // Optie index 
    lcd_info_disp,                     // display
    0,                                 // cursor
    &dummy_value,                      // *value
    Dummy_Func,                        // void (*number)(void); 
    Arrow_Diag_Info_1_Func,            // void (*arrow)(void); 
    Dummy_Func,                        // void (*enter)(void);
  },
  { // Groep 1
    DISP_GROEP + 0,                    // nr
    0,                                 // index
    &opt_app.Motorgroup[0].Enabled,    // option
    (unsigned char *)&option_index_0,  // Optie index 
    lcd_groep_disp,                    // display
    0,                                 // cursor
    &dummy_value,                      // *value
    Dummy_Func,                        // void (*number)(void); 
    Arrow_Diag_Groep_1_Func,           // void (*arrow)(void); 
    Dummy_Func,                        // void (*enter)(void);
  },
  { // Groep 2
    DISP_GROEP + 1,                    // nr
    0,                                 // index
    &opt_app.Motorgroup[0].Enabled,    // option
    (unsigned char *)&option_index_1,  // Optie index 
    lcd_groep_disp,                    // display
    0,                                 // cursor
    &dummy_value,                      // *value
    Dummy_Func,                        // void (*number)(void); 
    Arrow_Diag_Groep_1_Func,           // void (*arrow)(void); 
    Dummy_Func,                        // void (*enter)(void);
  },
  { // Groep 3
    DISP_GROEP + 2,                    // nr
    0,                                 // index
    &opt_app.Motorgroup[0].Enabled,    // option
    (unsigned char *)&option_index_2,  // Optie index 
    lcd_groep_disp,                    // display
    0,                                 // cursor
    &dummy_value,                      // *value
    Dummy_Func,                        // void (*number)(void); 
    Arrow_Diag_Groep_1_Func,           // void (*arrow)(void); 
    Dummy_Func,                        // void (*enter)(void);
  },
  { // Groep 4
    DISP_GROEP + 3,                    // nr
    0,                                 // index
    &opt_app.Motorgroup[0].Enabled,    // option
    (unsigned char *)&option_index_3,  // Optie index 
    lcd_groep_disp,                    // display
    0,                                 // cursor
    &dummy_value,                      // *value
    Dummy_Func,                        // void (*number)(void); 
    Arrow_Diag_Groep_1_Func,           // void (*arrow)(void); 
    Dummy_Func,                        // void (*enter)(void);
  },
  { // Groep 5
    DISP_GROEP + 4,                    // nr
    0,                                 // index
    &opt_app.Motorgroup[0].Enabled,    // option
    (unsigned char *)&option_index_4,  // Optie index 
    lcd_groep_disp,                    // display
    0,                                 // cursor
    &dummy_value,                      // *value
    Dummy_Func,                        // void (*number)(void); 
    Arrow_Diag_Groep_1_Func,           // void (*arrow)(void); 
    Dummy_Func,                        // void (*enter)(void);
  },
  { // Groep 6
    DISP_GROEP + 5,                    // nr
    0,                                 // index
    &opt_app.Motorgroup[0].Enabled,    // option
    (unsigned char *)&option_index_5,  // Optie index 
    lcd_groep_disp,                    // display
    0,                                 // cursor
    &dummy_value,                      // *value
    Dummy_Func,                        // void (*number)(void); 
    Arrow_Diag_Groep_1_Func,           // void (*arrow)(void); 
    Dummy_Func,                        // void (*enter)(void);
  },
  { // Groep 7
    DISP_GROEP + 6,                    // nr
    0,                                 // index
    &opt_app.Motorgroup[0].Enabled,    // option
    (unsigned char *)&option_index_6,  // Optie index 
    lcd_groep_disp,                    // display
    0,                                 // cursor
    &dummy_value,                      // *value
    Dummy_Func,                        // void (*number)(void); 
    Arrow_Diag_Groep_1_Func,           // void (*arrow)(void); 
    Dummy_Func,                        // void (*enter)(void);
  },
  { // Groep 8
    DISP_GROEP + 7,                    // nr
    0,                                 // index
    &opt_app.Motorgroup[0].Enabled,    // option
    (unsigned char *)&option_index_7,  // Optie index 
    lcd_groep_disp,                    // display
    0,                                 // cursor
    &dummy_value,                      // *value
    Dummy_Func,                        // void (*number)(void); 
    Arrow_Diag_Groep_1_Func,           // void (*arrow)(void); 
    Dummy_Func,                        // void (*enter)(void);
  },
  { // Groep 9
    DISP_GROEP + 8,                    // nr
    0,                                 // index
    &opt_app.Motorgroup[0].Enabled,    // option
    (unsigned char *)&option_index_8,  // Optie index 
    lcd_groep_disp,                    // display
    0,                                 // cursor
    &dummy_value,                      // *value
    Dummy_Func,                        // void (*number)(void); 
    Arrow_Diag_Groep_1_Func,           // void (*arrow)(void); 
    Dummy_Func,                        // void (*enter)(void);
  },
  { // Groep 10
    DISP_GROEP + 9,                    // nr
    0,                                 // index
    &opt_app.Motorgroup[0].Enabled,    // option
    (unsigned char *)&option_index_9,  // Optie index 
    lcd_groep_disp,                    // display
    0,                                 // cursor
    &dummy_value,                      // *value
    Dummy_Func,                        // void (*number)(void); 
    Arrow_Diag_Groep_1_Func,           // void (*arrow)(void); 
    Dummy_Func,                        // void (*enter)(void);
  },
  { // Groep 11
    DISP_GROEP + 10,                   // nr
    0,                                 // index
    &opt_app.Motorgroup[0].Enabled,    // option
    (unsigned char *)&option_index_10, // Optie index 
    lcd_groep_disp,                    // display
    0,                                 // cursor
    &dummy_value,                      // *value
    Dummy_Func,                        // void (*number)(void); 
    Arrow_Diag_Groep_1_Func,           // void (*arrow)(void); 
    Dummy_Func,                        // void (*enter)(void);
  },
  { // Groep 12
    DISP_GROEP + 11,                   // nr
    0,                                 // index
    &opt_app.Motorgroup[0].Enabled,    // option
    (unsigned char *)&option_index_11, // Optie index 
    lcd_groep_disp,                    // display
    0,                                 // cursor
    &dummy_value,                      // *value
    Dummy_Func,                        // void (*number)(void); 
    Arrow_Diag_Groep_1_Func,           // void (*arrow)(void); 
    Dummy_Func,                        // void (*enter)(void);
  },
  { // Groep 13
    DISP_GROEP + 12,                   // nr
    0,                                 // index
    &opt_app.Motorgroup[0].Enabled,    // option
    (unsigned char *)&option_index_12, // Optie index 
    lcd_groep_disp,                    // display
    0,                                 // cursor
    &dummy_value,                      // *value
    Dummy_Func,                        // void (*number)(void); 
    Arrow_Diag_Groep_1_Func,           // void (*arrow)(void); 
    Dummy_Func,                        // void (*enter)(void);
  },
  { // Groep 14
    DISP_GROEP + 13,                   // nr
    0,                                 // index
    &opt_app.Motorgroup[0].Enabled,    // option
    (unsigned char *)&option_index_13, // Optie index 
    lcd_groep_disp,                    // display
    0,                                 // cursor
    &dummy_value,                      // *value
    Dummy_Func,                        // void (*number)(void); 
    Arrow_Diag_Groep_1_Func,           // void (*arrow)(void); 
    Dummy_Func,                        // void (*enter)(void);
  },
  { // Groep 15
    DISP_GROEP + 14,                   // nr
    0,                                 // index
    &opt_app.Motorgroup[0].Enabled,    // option
    (unsigned char *)&option_index_14, // Optie index 
    lcd_groep_disp,                    // display
    0,                                 // cursor
    &dummy_value,                      // *value
    Dummy_Func,                        // void (*number)(void); 
    Arrow_Diag_Groep_1_Func,           // void (*arrow)(void); 
    Dummy_Func,                        // void (*enter)(void);
  },
  { // Groep 16
    DISP_GROEP + 15,                   // nr
    0,                                 // index
    &opt_app.Motorgroup[0].Enabled,    // option
    (unsigned char *)&option_index_15, // Optie index 
    lcd_groep_disp,                    // display
    0,                                 // cursor
    &dummy_value,                      // *value
    Dummy_Func,                        // void (*number)(void); 
    Arrow_Diag_Groep_1_Func,           // void (*arrow)(void); 
    Dummy_Func,                        // void (*enter)(void);
  },
  { // Groep 17
    DISP_GROEP + 16,                   // nr
    0,                                 // index
    &opt_app.Motorgroup[0].Enabled,    // option
    (unsigned char *)&option_index_16, // Optie index 
    lcd_groep_disp,                    // display
    0,                                 // cursor
    &dummy_value,                      // *value
    Dummy_Func,                        // void (*number)(void); 
    Arrow_Diag_Groep_1_Func,           // void (*arrow)(void); 
    Dummy_Func,                        // void (*enter)(void);
  },
  { // Groep 18
    DISP_GROEP + 17,                   // nr
    0,                                 // index
    &opt_app.Motorgroup[0].Enabled,    // option
    (unsigned char *)&option_index_17, // Optie index 
    lcd_groep_disp,                    // display
    0,                                 // cursor
    &dummy_value,                      // *value
    Dummy_Func,                        // void (*number)(void); 
    Arrow_Diag_Groep_1_Func,           // void (*arrow)(void); 
    Dummy_Func,                        // void (*enter)(void);
  },
  { // Groep 19
    DISP_GROEP + 18,                   // nr
    0,                                 // index
    &opt_app.Motorgroup[0].Enabled,    // option
    (unsigned char *)&option_index_18, // Optie index 
    lcd_groep_disp,                    // display
    0,                                 // cursor
    &dummy_value,                      // *value
    Dummy_Func,                        // void (*number)(void); 
    Arrow_Diag_Groep_1_Func,           // void (*arrow)(void); 
    Dummy_Func,                        // void (*enter)(void);
  },
  { // Groep 20
    DISP_GROEP + 19,                   // nr
    0,                                 // index
    &opt_app.Motorgroup[0].Enabled,    // option
    (unsigned char *)&option_index_19, // Optie index 
    lcd_groep_disp,                    // display
    0,                                 // cursor
    &dummy_value,                      // *value
    Dummy_Func,                        // void (*number)(void); 
    Arrow_Diag_Groep_1_Func,           // void (*arrow)(void); 
    Dummy_Func,                        // void (*enter)(void);
  },
  { // Groep 21
    DISP_GROEP + 20,                   // nr
    0,                                 // index
    &opt_app.Motorgroup[0].Enabled,    // option
    (unsigned char *)&option_index_20, // Optie index 
    lcd_groep_disp,                    // display
    0,                                 // cursor
    &dummy_value,                      // *value
    Dummy_Func,                        // void (*number)(void); 
    Arrow_Diag_Groep_1_Func,           // void (*arrow)(void); 
    Dummy_Func,                        // void (*enter)(void);
  },
  { // Groep 22
    DISP_GROEP + 21,                   // nr
    0,                                 // index
    &opt_app.Motorgroup[0].Enabled,    // option
    (unsigned char *)&option_index_21, // Optie index 
    lcd_groep_disp,                    // display
    0,                                 // cursor
    &dummy_value,                      // *value
    Dummy_Func,                        // void (*number)(void); 
    Arrow_Diag_Groep_1_Func,           // void (*arrow)(void); 
    Dummy_Func,                        // void (*enter)(void);
  },
  { // Groep 23
    DISP_GROEP + 22,                   // nr
    0,                                 // index
    &opt_app.Motorgroup[0].Enabled,    // option
    (unsigned char *)&option_index_22, // Optie index 
    lcd_groep_disp,                    // display
    0,                                 // cursor
    &dummy_value,                      // *value
    Dummy_Func,                        // void (*number)(void); 
    Arrow_Diag_Groep_1_Func,           // void (*arrow)(void); 
    Dummy_Func,                        // void (*enter)(void);
  },
  { // Groep 24
    DISP_GROEP + 23,                   // nr
    0,                                 // index
    &opt_app.Motorgroup[0].Enabled,    // option
    (unsigned char *)&option_index_23, // Optie index 
    lcd_groep_disp,                    // display
    0,                                 // cursor
    &dummy_value,                      // *value
    Dummy_Func,                        // void (*number)(void); 
    Arrow_Diag_Groep_1_Func,           // void (*arrow)(void); 
    Dummy_Func,                        // void (*enter)(void);
  },
  { // Groep 25
    DISP_GROEP + 24,                   // nr
    0,                                 // index
    &opt_app.Motorgroup[0].Enabled,    // option
    (unsigned char *)&option_index_24, // Optie index 
    lcd_groep_disp,                    // display
    0,                                 // cursor
    &dummy_value,                      // *value
    Dummy_Func,                        // void (*number)(void); 
    Arrow_Diag_Groep_1_Func,           // void (*arrow)(void); 
    Dummy_Func,                        // void (*enter)(void);
  },
  { // Groep 26
    DISP_GROEP + 25,                   // nr
    0,                                 // index
    &opt_app.Motorgroup[0].Enabled,    // option
    (unsigned char *)&option_index_25, // Optie index 
    lcd_groep_disp,                    // display
    0,                                 // cursor
    &dummy_value,                      // *value
    Dummy_Func,                        // void (*number)(void); 
    Arrow_Diag_Groep_1_Func,           // void (*arrow)(void); 
    Dummy_Func,                        // void (*enter)(void);
  },
  { // Groep 27
    DISP_GROEP + 26,                   // nr
    0,                                 // index
    &opt_app.Motorgroup[0].Enabled,    // option
    (unsigned char *)&option_index_26, // Optie index 
    lcd_groep_disp,                    // display
    0,                                 // cursor
    &dummy_value,                      // *value
    Dummy_Func,                        // void (*number)(void); 
    Arrow_Diag_Groep_1_Func,           // void (*arrow)(void); 
    Dummy_Func,                        // void (*enter)(void);
  },
  { // Groep 28
    DISP_GROEP + 27,                   // nr
    0,                                 // index
    &opt_app.Motorgroup[0].Enabled,    // option
    (unsigned char *)&option_index_27, // Optie index 
    lcd_groep_disp,                    // display
    0,                                 // cursor
    &dummy_value,                      // *value
    Dummy_Func,                        // void (*number)(void); 
    Arrow_Diag_Groep_1_Func,           // void (*arrow)(void); 
    Dummy_Func,                        // void (*enter)(void);
  },
  { // Groep 29
    DISP_GROEP + 28,                   // nr
    0,                                 // index
    &opt_app.Motorgroup[0].Enabled,    // option
    (unsigned char *)&option_index_28, // Optie index 
    lcd_groep_disp,                    // display
    0,                                 // cursor
    &dummy_value,                      // *value
    Dummy_Func,                        // void (*number)(void); 
    Arrow_Diag_Groep_1_Func,           // void (*arrow)(void); 
    Dummy_Func,                        // void (*enter)(void);
  },
  { // Groep 30
    DISP_GROEP + 29,                   // nr
    0,                                 // index
    &opt_app.Motorgroup[0].Enabled,    // option
    (unsigned char *)&option_index_29, // Optie index 
    lcd_groep_disp,                    // display
    0,                                 // cursor
    &dummy_value,                      // *value
    Dummy_Func,                        // void (*number)(void); 
    Arrow_Diag_Groep_1_Func,           // void (*arrow)(void); 
    Dummy_Func,                        // void (*enter)(void);
  },
  { // Groep 31
    DISP_GROEP + 30,                   // nr
    0,                                 // index
    &opt_app.Motorgroup[0].Enabled,    // option
    (unsigned char *)&option_index_30, // Optie index 
    lcd_groep_disp,                    // display
    0,                                 // cursor
    &dummy_value,                      // *value
    Dummy_Func,                        // void (*number)(void); 
    Arrow_Diag_Groep_1_Func,           // void (*arrow)(void); 
    Dummy_Func,                        // void (*enter)(void);
  },
  { // Groep 32
    DISP_GROEP + 31,                   // nr
    0,                                 // index
    &opt_app.Motorgroup[0].Enabled,    // option
    (unsigned char *)&option_index_31, // Optie index 
    lcd_groep_disp,                    // display
    0,                                 // cursor
    &dummy_value,                      // *value
    Dummy_Func,                        // void (*number)(void); 
    Arrow_Diag_Groep_1_Func,           // void (*arrow)(void); 
    Dummy_Func,                        // void (*enter)(void);
  },
  { // Luchtmengkast
    DISP_LUCHTMENGKAST,                // nr
    0,                                 // index
    &opt_app.Luchtmengkast[0].Enabled, // option
    (unsigned char *)&option_index_0,  // Optie index 
    lcd_luchtmengkast_disp,            // display
    0,                                 // cursor
    &dummy_value,                      // *value
    Dummy_Func,                        // void (*number)(void); 
    Arrow_Diag_Luchtmengkast_1_Func,   // void (*arrow)(void); 
    Dummy_Func,                        // void (*enter)(void);
  },
  {	// com1
    DISP_COM1,                         // nr
    0,                                 // index
    &opt_alg.com1_enabled,             // option
    (unsigned char *)&option_index_0,  // Optie index 
    lcd_com1_usb_disp,                 // display
    0,                                 // cursor
	&dummy_value,                      // *value
    Dummy_Func,                        // void (*number)(void); 
    Arrow_Diag_Com1_Usb_1_Func,        // void (*arrow)(void); 
    Dummy_Func,                        // void (*enter)(void);
  },
  {	// com2
    DISP_COM2,                         // nr
    0,                                 // index
    &opt_alg.com2_enabled,             // option
    (unsigned char *)&option_index_0,  // Optie index 
    lcd_com2_disp,                     // display
    0,                                 // cursor
	&dummy_value,                      // *value
    Dummy_Func,                        // void (*number)(void); 
    Arrow_Diag_Com2_1_Func,            // void (*arrow)(void); 
    Dummy_Func,                        // void (*enter)(void);
  },
  #ifdef ETHERNET
  {	// ethernet
    DISP_ETHERNET,                     // nr
    0,                                 // index
    &opt_alg.ethernet_enabled,         // option
    (unsigned char *)&option_index_0,  // Optie index 
    lcd_ethernet_disp,                 // display
    0,                                 // cursor
	&dummy_value,                      // *value
    Dummy_Func,                        // void (*number)(void); 
    Arrow_Diag_Ethernet_1_Func,        // void (*arrow)(void); 
    Dummy_Func,                        // void (*enter)(void);
  },
  #endif // ETHERNET
  { // CAN LOCAL
    DISP_CAN_LOCAL,                    // nr
    0,                                 // index
    (unsigned char *)&option_on,       // option
    (unsigned char *)&option_index_0,  // Optie index 
    lcd_can_local_disp,                // display
    0,                                 // cursor
    &dummy_value,                      // *value
    Dummy_Func,                        // void (*number)(void); 
    Arrow_Diag_Can_Local_1_Func,       // void (*arrow)(void); 
    Dummy_Func,                        // void (*enter)(void);
  },
  { // CAN BACKBONE
    DISP_CAN_BACKBONE,                 // nr
    0,                                 // index
    &opt_alg.can_backbone,             // option
    (unsigned char *)&option_index_0,  // Optie index 
    lcd_can_backbone_disp,             // display
    0,                                 // cursor
    &dummy_value,                      // *value
    Dummy_Func,                        // void (*number)(void); 
    Arrow_Diag_Can_Backbone_1_Func,    // void (*arrow)(void); 
    Dummy_Func,                        // void (*enter)(void);
  },
  #ifdef SD_CARD
  {	// empty
    DISP_SD_CARD,                      // nr
    0,                                 // index
    (unsigned char *)&option_on,       // option
    (unsigned char *)&option_index_0,  // Optie index 
    lcd_sd_card_disp,                  // display
    0,                                 // cursor
	&dummy_value,                      // *value
    Dummy_Func,                        // void (*number)(void); 
    Arrow_Diag_SD_Card_1_Func,         // void (*arrow)(void); 
    Dummy_Func,                        // void (*enter)(void);
  },
  #endif // SD_CARD
#ifdef ETHERNET
  {	// empty
    DISP_DISPLAY_COLOR,               // nr
    0,               // index
    (unsigned char *)&option_on,      // option
    (unsigned char *)&option_index_0,   // Optie index 
    lcd_display_color_disp, // display
    0,               // cursor
	&dummy_value,     // *value
    Dummy_Func,      // void (*number)(void); 
    Arrow_Change_Func, // void (*arrow)(void); 
    Dummy_Func,      // void (*enter)(void);
  },
  {	// empty
    DISP_DISPLAY_COLOR,               // nr
    1,               // index
    (unsigned char *)&option_on,      // option
    (unsigned char *)&option_index_0,   // Optie index 
    lcd_display_color_disp, // display
    &disp_cursor_display_color,  // cursor
	&key_display_color_value,     // *value
    Dummy_Func,      // void (*number)(void); 
    Arrow_Display_Color_Value, // void (*arrow)(void); 
    Enter_Display_Color_Value,      // void (*enter)(void);
  },
#endif // ETHERNET
  #ifdef TIMING_TEST
  {
    100,                              // nr
    0,                                // index
    (unsigned char *)&option_on,      // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_test_disp_0,                  // display
    0,                                // cursor
    &dummy_value,                     // *value
    Number_Reset_Test,                // void (*number)(void); 
    Arrow_Reset_Test,                 // void (*arrow)(void); 
    Enter_Reset_Test,                 // void (*enter)(void);
  },
  #endif // TIMING_TEST
  #ifdef TEST_H2MC
  {
    200,                              // nr
    0,                                // index
    (unsigned char *)&option_on,      // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_test_disp_1,                  // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Func,                       // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  #endif
};

s_screen screen_diagnose_0;
s_screen const screen_diagnose_0_default =
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
  &diagnose_0_key_action[0], // first_action
  &diagnose_0_key_action[sizeof(diagnose_0_key_action)/sizeof(s_key_action) - 1], // last action 
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


void Control_Screen_Diagnose_0(void)
{
  Control_Screen(&screen_diagnose_0, &screen_diagnose_0_default, 0, 3);
}

//-----------------------------------------------------------------------------
static void Arrow_Diag_Info_1_Func(void)
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
      If_Exist_Goto_Screen_Diag_Info_1();
      break;
  }
}

static void Arrow_Diag_Groep_1_Func(void)
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
	  If_Exist_Goto_Screen_Diag_Group_1(screen_ptr->nr - 2);
      break;
  }
}

static void Arrow_Diag_Luchtmengkast_1_Func(void)
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
	  If_Exist_Goto_Screen_Diag_Luchtmengkast_1();
      break;
  }
}

static void Arrow_Diag_Com1_Usb_1_Func(void) // standaard functie voor hoofdschermen (bv klim_0_screen)
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
      If_Exist_Goto_Screen_Diag_Asc0_1();
      break;
  }
}

static void Arrow_Diag_Com2_1_Func(void) // standaard functie voor hoofdschermen (bv klim_0_screen)
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
      If_Exist_Goto_Screen_Diag_Asc1_1();
      break;
  }
}

#ifdef ETHERNET
static void Arrow_Diag_Ethernet_1_Func(void) // standaard functie voor hoofdschermen (bv klim_0_screen)
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
      If_Exist_Goto_Screen_Diag_Ethernet_1();
      break;
  }
}
#endif // ETHERNET

static void Arrow_Diag_Can_Local_1_Func(void)
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
      If_Exist_Goto_Screen_Diag_Can_IO_1();
      break;
  }
}

static void Arrow_Diag_Can_Backbone_1_Func(void)
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
      If_Exist_Goto_Screen_Diag_Can_Backbone_1();
      break;
  }
}

#ifdef SD_CARD
static void Arrow_Diag_SD_Card_1_Func(void) // standaard functie voor hoofdschermen (bv klim_0_screen)
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
      If_Exist_Goto_Screen_Diag_SD_Card_1();
      break;
  }
}
#endif // SD_CARD


//*****************************************************************************
#ifdef ETHERNET
static void Enter_Display_Color_Value(void)
{
  CP2200_Write_Display_Color();
  Increment_Func_Index();
}

static void Arrow_Display_Color_Value(void)
{
  switch (key)
  {
    case UP:
    case DOWN:
	  Increment_Scroll_Value();
      break;
    case LEFT:
    case RIGHT:
      Enter_Display_Color_Value();
      break;
  }
}
#endif // ETHERNET

//*****************************************************************************

static void Number_Reset_Test(void)
{
  switch (key)
  {
    case '0':
      break;
    case '1': 
      break;
    case '2': 
      break;
  }
}

static void Enter_Reset_Test(void)
{
  TEST_RESET();
}

static void Arrow_Reset_Test(void)
{
  switch (key)
  {
    case UP:    Decrement_Func(); break;
    case DOWN:  Increment_Func(); break;
    case LEFT:
      break;
    case RIGHT: 
      break;
  }
}
