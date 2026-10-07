// C__DISP_DIAG_CAN_BACKBONE_1.C

#include "ch_define.h"

#include "ch_can_backbone_hardware.h"
#include "ch_const.h"
#include "ch_disp.h"
#include "ch_disp_diagnose_0.h"
#include "ch_disp_func.h"
#include "ch_disp_value.h"
#include "ch_key.h"
#include "ch_lcd_7.h"
#include "ch_lcd_10.h"
#include "ch_lcd_14.h"
#include "ch_lcd_bitmap.h"
#include "ch_lcd_pixel.h"
#include "ch_string.h"
#include "ch_disp_diag_can_backbone_1.h"

#include <co_type.h>
#include <co_nmt.h>

extern s_screen screen_diag_can_backbone_1;

static void Enter_Reset_Diag_Can_Backbone(void);
static void Disp_CANopen_State(void);


static s_disp_agri_header const disp_header =              { Disp_Draw_Agri_Header, FN5, EMPTY, &tekst.Diag_CAN_BACKBONE_10, HK_GEEN, 0, 2};
static void * const lcd_disp_header[] = { &disp_header, 0};

static s_tekst_18 CANopen_state;
static s_tekst_18 const unknown_state_str     = { 18, 80, SIZE_10, "UNKNOWN STATE" };
static s_tekst_18 const initialising_str      = { 18, 80, SIZE_10, "INITIALISING" };
static s_tekst_18 const stopped_str           = { 18, 80, SIZE_10, "STOPPED" };
static s_tekst_18 const operational_str       = { 18, 80, SIZE_10, "OPERATIONAL" };
static s_tekst_18 const pre_operational_str   = { 18, 80, SIZE_10, "PRE-OPERATIONAL" };
static s_tekst_18 const reset_application_str = { 18, 80, SIZE_10, "RESET APPLICATION" };
static s_tekst_18 const reset_comm_str        = { 18, 80, SIZE_10, "RESET COMM" };

static s_disp_func  const disp_control               = { Disp_Control_Func, Disp_CANopen_State };
static s_disp_tekst const disp_CANopen_state_str     = { Disp_Draw_Tekst_L, 12, 20, &CANopen_state };
static s_disp_tekst const disp_txok_str              = { Disp_Draw_Tekst_L, 12, 20, &tekst.TXOK_10 };
static s_disp_value const disp_txok_val              = { Disp_Draw_Value,  147, 20, (SIZE_14 | RECHTS), UINT, 0, &can_backbone_txok };
static s_disp_value const disp_txok_peak_val         = { Disp_Draw_Value,  207, 20, (SIZE_14 | RECHTS), UINT, 0, &can_backbone_txok_peak };
static s_disp_tekst const disp_rxok_str              = { Disp_Draw_Tekst_L, 12, 20, &tekst.RXOK_10 };
static s_disp_value const disp_rxok_val              = { Disp_Draw_Value,  147, 20, (SIZE_14 | RECHTS), UINT, 0, &can_backbone_rxok };
static s_disp_value const disp_rxok_peak_val         = { Disp_Draw_Value,  207, 20, (SIZE_14 | RECHTS), UINT, 0, &can_backbone_rxok_peak };
static s_disp_tekst const disp_txok_rxok_str         = { Disp_Draw_Tekst_L, 12, 20, &tekst.TXOK_RXOK_10 };
static s_disp_value const disp_txok_rxok_val         = { Disp_Draw_Value,  147, 20, (SIZE_14 | RECHTS), UINT, 0, &can_backbone_txok_rxok };
static s_disp_value const disp_txok_rxok_peak_val    = { Disp_Draw_Value,  207, 20, (SIZE_14 | RECHTS), UINT, 0, &can_backbone_txok_rxok_peak };
static s_disp_tekst const disp_boff_str              = { Disp_Draw_Tekst_L, 12, 20, &tekst.BOFF_10 };
static s_disp_value const disp_boff_val              = { Disp_Draw_Value,  207, 20, (SIZE_14 | RECHTS), UINT, 0, &can_backbone_status.BOFF };
static s_disp_tekst const disp_ewrn_str              = { Disp_Draw_Tekst_L, 12, 20, &tekst.EWRN_10 };
static s_disp_value const disp_ewrn_val              = { Disp_Draw_Value,  207, 20, (SIZE_14 | RECHTS), UINT, 0, &can_backbone_status.EWRN };
static s_disp_tekst const disp_stuff_error_str       = { Disp_Draw_Tekst_L, 12, 20, &tekst.Stuff_error_10 };
static s_disp_value const disp_stuff_error_val       = { Disp_Draw_Value,  207, 20, (SIZE_14 | RECHTS), UINT, 0, &can_backbone_status.ERROR_STUFF };
static s_disp_tekst const disp_form_error_str        = { Disp_Draw_Tekst_L, 12, 20, &tekst.Form_error_10 };
static s_disp_value const disp_form_error_val        = { Disp_Draw_Value,  207, 20, (SIZE_14 | RECHTS), UINT, 0, &can_backbone_status.ERROR_FORM };
static s_disp_tekst const disp_ack_error_str         = { Disp_Draw_Tekst_L, 12, 20, &tekst.Ack_error_10 };
static s_disp_value const disp_ack_error_val         = { Disp_Draw_Value,  207, 20, (SIZE_14 | RECHTS), UINT, 0, &can_backbone_status.ERROR_ACK };
static s_disp_tekst const disp_bit1_error_str        = { Disp_Draw_Tekst_L, 12, 20, &tekst.Bit1_error_10 };
static s_disp_value const disp_bit1_error_val        = { Disp_Draw_Value,  207, 20, (SIZE_14 | RECHTS), UINT, 0, &can_backbone_status.ERROR_BIT1 };
static s_disp_tekst const disp_bit0_busoff_error_str = { Disp_Draw_Tekst_L, 12, 20, &tekst.Bit0_busoff_error_10 };
static s_disp_value const disp_bit0_busoff_error_val = { Disp_Draw_Value,  207, 20, (SIZE_14 | RECHTS), UINT, 0, &can_backbone_status.ERROR_BIT0_BUSOFF };
static s_disp_tekst const disp_bit0_normal_error_str = { Disp_Draw_Tekst_L, 12, 20, &tekst.Bit0_normal_error_10 };
static s_disp_value const disp_bit0_normal_error_val = { Disp_Draw_Value,  207, 20, (SIZE_14 | RECHTS), UINT, 0, &can_backbone_status.ERROR_BIT0_NORMAL };
static s_disp_tekst const disp_crc_error_str         = { Disp_Draw_Tekst_L, 12, 20, &tekst.Crc_error_10 };
static s_disp_value const disp_crc_error_val         = { Disp_Draw_Value,  207, 20, (SIZE_14 | RECHTS), UINT, 0, &can_backbone_status.ERROR_CRC };


static void * const lcd_CANopen_state_disp[]     = { &disp_control, &disp_CANopen_state_str, 0 };
static void * const lcd_txok_disp[]              = { &disp_txok_str, &disp_txok_val, &disp_txok_peak_val, 0 };
static void * const lcd_rxok_disp[]              = { &disp_rxok_str, &disp_rxok_val, &disp_rxok_peak_val, 0 };
static void * const lcd_txok_rxok_disp[]         = { &disp_txok_rxok_str, &disp_txok_rxok_val, &disp_txok_rxok_peak_val, 0 };
static void * const lcd_boff_disp[]              = { &disp_boff_str, &disp_boff_val, 0 };
static void * const lcd_ewrn_disp[]              = { &disp_ewrn_str, &disp_ewrn_val, 0 };
static void * const lcd_stuff_error_disp[]       = { &disp_stuff_error_str, &disp_stuff_error_val, 0 };
static void * const lcd_form_error_disp[]        = { &disp_form_error_str, &disp_form_error_val, 0 };
static void * const lcd_ack_error_disp[]         = { &disp_ack_error_str, &disp_ack_error_val, 0 };
static void * const lcd_bit1_error_disp[]        = { &disp_bit1_error_str, &disp_bit1_error_val, 0 };
static void * const lcd_bit0_busoff_error_disp[] = { &disp_bit0_busoff_error_str, &disp_bit0_busoff_error_val, 0 };
static void * const lcd_bit0_normal_error_disp[] = { &disp_bit0_normal_error_str, &disp_bit0_normal_error_val, 0 };
static void * const lcd_crc_error_disp[]         = { &disp_crc_error_str, &disp_crc_error_val, 0 };

s_key_action const diag_can_backbone_1_key_action[] =
{
#ifdef CANopen
  { // CANopen state
    0,                                // nr
    0,                                // index
    (unsigned char *)&option_on,      // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_CANopen_state_disp,           // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Change_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
#endif // CANopen
  { // TXOK
    1,                                // nr
    0,                                // index
    (unsigned char *)&option_on,      // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_txok_disp,                    // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Change_Func,                // void (*arrow)(void); 
    Enter_Reset_Diag_Can_Backbone,            // void (*enter)(void);
  },
  { // RXOK
    2,                                // nr
    0,                                // index
    (unsigned char *)&option_on,      // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_rxok_disp,                    // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Change_Func,                // void (*arrow)(void); 
    Enter_Reset_Diag_Can_Backbone,            // void (*enter)(void);
  },
  { // TXOK + RXOK
    3,                                // nr
    0,                                // index
    (unsigned char *)&option_on,      // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_txok_rxok_disp,               // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Change_Func,                // void (*arrow)(void); 
    Enter_Reset_Diag_Can_Backbone,            // void (*enter)(void);
  },
  { // BOFF
    4,                                // nr
    0,                                // index
    (unsigned char *)&option_on,      // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_boff_disp,                    // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Change_Func,                // void (*arrow)(void); 
    Enter_Reset_Diag_Can_Backbone,            // void (*enter)(void);
  },
  { // EWRN
    5,                                // nr
    0,                                // index
    (unsigned char *)&option_on,      // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_ewrn_disp,                    // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Change_Func,                // void (*arrow)(void); 
    Enter_Reset_Diag_Can_Backbone,            // void (*enter)(void);
  },
  { // Stuff error
    6,                                // nr
    0,                                // index
    (unsigned char *)&option_on,      // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_stuff_error_disp,             // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Change_Func,                // void (*arrow)(void); 
    Enter_Reset_Diag_Can_Backbone,            // void (*enter)(void);
  },
  { // Form error
    7,                                // nr
    0,                                // index
    (unsigned char *)&option_on,      // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_form_error_disp,              // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Change_Func,                // void (*arrow)(void); 
    Enter_Reset_Diag_Can_Backbone,            // void (*enter)(void);
  },
  { // Ack error
    8,                                // nr
    0,                                // index
    (unsigned char *)&option_on,      // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_ack_error_disp,               // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Change_Func,                // void (*arrow)(void); 
    Enter_Reset_Diag_Can_Backbone,            // void (*enter)(void);
  },
  { // Bit1 error
    9,                                // nr
    0,                                // index
    (unsigned char *)&option_on,      // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_bit1_error_disp,             // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Change_Func,                // void (*arrow)(void); 
    Enter_Reset_Diag_Can_Backbone,            // void (*enter)(void);
  },
  { // Bit0 busoff error
    10,                                // nr
    0,                                // index
    (unsigned char *)&option_on,      // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_bit0_busoff_error_disp,       // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Change_Func,                // void (*arrow)(void); 
    Enter_Reset_Diag_Can_Backbone,            // void (*enter)(void);
  },
  { // Bit0 normal error
    11,                               // nr
    0,                                // index
    (unsigned char *)&option_on,      // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_bit0_normal_error_disp,       // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Change_Func,                // void (*arrow)(void); 
    Enter_Reset_Diag_Can_Backbone,            // void (*enter)(void);
  },
  { // Crc error
    12,                               // nr
    0,                                // index
    (unsigned char *)&option_on,      // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_crc_error_disp,               // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Change_Func,                // void (*arrow)(void); 
    Enter_Reset_Diag_Can_Backbone,            // void (*enter)(void);
  },
};

s_screen screen_diag_can_backbone_1;
s_screen const screen_diag_can_backbone_1_default =
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
  &diag_can_backbone_1_key_action[0], // first_action
  &diag_can_backbone_1_key_action[sizeof(diag_can_backbone_1_key_action)/sizeof(s_key_action) - 1], // last action 
  0, // change_flag
  0,
  0,
  0,
  0,
  0,
  0,
  &screen_diagnose_0
};


void Control_Screen_Diag_Can_Backbone_1(void)
// zorg voor een correcte inhoud in klimaat scherm 0
{
  Control_Screen(&screen_diag_can_backbone_1, &screen_diag_can_backbone_1_default, 1, 3);
}

void If_Exist_Goto_Screen_Diag_Can_Backbone_1(void)
{
  Control_Screen_Diag_Can_Backbone_1();
  if (screen_diag_can_backbone_1.nr_aantal)
    Next_Screen(&screen_diag_can_backbone_1);
}

static void Enter_Reset_Diag_Can_Backbone(void)
{
  can_backbone_diag_reset_flag = 1;
}

static void Disp_CANopen_State(void)
{
NODE_STATE_T state;

  state = getNodeState();
  switch (state)
  {
    case INITIALISING:
	  CANopen_state = initialising_str;
	  break;
    case STOPPED:
	  CANopen_state = stopped_str;
	  break;
    case OPERATIONAL:
	  CANopen_state = operational_str;
	  break;
    case PRE_OPERATIONAL:
	  CANopen_state = pre_operational_str;
	  break;
    case RESET_APPLICATION:
	  CANopen_state = reset_application_str;
	  break;
    case RESET_COMM:
	  CANopen_state = reset_comm_str;
	  break;
	default:
	  CANopen_state = unknown_state_str;
	  break;
  }
}
