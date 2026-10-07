// C__DISP_TIJD_2.C

#include "ch_define.h"

#include "ch_const.h"
#include "ch_can_backbone_appl.h"
#include "ch_disp.h"
#include "ch_disp_func.h"
#include "ch_disp_password.h"
#include "ch_disp_value.h"
#include "ch_key.h"
#include "ch_lcd_10.h"
#include "ch_lcd_14.h"
#include "ch_lcd_bitmap.h"
#include "ch_lcd_pixel.h"
#include "ch_string.h"
#include "ch_disp_tijd_2.h"

static void Arrow_Tijd_Sync(void);
static void Enter_Tijd_Sync(void);

static s_disp_agri_header const disp_header = { Disp_Draw_Agri_Header, FN1, HD_SYST, &tekst.Time_10, HK_GEEN, 0, 3};
static void * const lcd_disp_header[] = { &disp_header, 0};

//*****************************************************************************
// SCHERM OPBOUW
//*****************************************************************************
//-----------------------------------------------------------------------------
// basis opbouw lcd
/*
static s_disp_tekst     const disp_header_str_0  = { Disp_Draw_Tekst_L,  12, 12, &tekst.Syst_10 };
static s_disp_tekst_add const disp_header_str_1  = { Disp_Draw_Tekst_Add_L, &tekst.Time_10 };
static s_disp_tekst     const disp_header_str_2  = { Disp_Draw_Tekst_L, 204, 12, &header_string_2 };
static s_disp_value     const disp_header_val_2  = { Disp_Draw_Value,   237, 12, (SIZE_7 | RECHTS), INT, 0, &screen_tijd_2.nr };
static s_disp_block     const disp_header_invert = { Disp_Invert_Block,       0,  0, 239, 16 };
static s_disp_block     const disp_header_line_0 = { Disp_Draw_Black_Block,   0, 13,   0,104 }; // vertikaal
static s_disp_block     const disp_header_line_1 = { Disp_Draw_Black_Block, 239, 13, 239,104 }; // vertikaal
static s_disp_block     const disp_header_line_2 = { Disp_Draw_Black_Block,  44,104, 238,104 }; 

static void * const lcd_disp_header[] = { &disp_10_slot, &disp_header_str_0, &disp_space_10_L, &disp_header_str_1,
                                          &disp_header_str_2, &disp_header_val_2,
										  &disp_bus_ok_ico,
                                          &disp_header_invert,
                                          &disp_header_line_0, &disp_header_line_1, &disp_header_line_2,
                                          &disp_loper,
                                          &disp_tab_begin,  &disp_fn_F1, 
                                          &disp_tab_2_norm, &disp_fn_F2,
                                          &disp_tab_3_norm, &disp_fn_F3,
                                          &disp_tab_4_norm, &disp_fn_alarm,
                                          &disp_tab_5_norm, &disp_fn_diagnose,
                                          &disp_tab_6_norm, &disp_fn_opties,
                                          &disp_tab_end, 0};
*/
//-----------------------------------------------------------------------------
// Sync tijd
static s_disp_cursor       const disp_cursor_klok_sync = { 219, 25, 30, 2, 22 };
static s_disp_tekst        const disp_sync_tijd_str = { Disp_Draw_Tekst_L,       37, 20, &tekst.Sync_Tijd_10 };
static s_disp_bitmap_array const disp_sync_tijd_bmp = { Disp_Draw_Bitmap_Array, 190,  5, ico_klok_niet_ontvangen_zenden, UCHAR, &setp_alg.tijd_sync, 3 };

static void * const lcd_sync_tijd_disp[] = { &disp_tijd, &disp_sync_tijd_str, &disp_sync_tijd_bmp, 0 };

static s_key_value const key_sync_tijd_value = { UCHAR, 3, &setp_alg.tijd_sync, &uchar_0, &uchar_2 };
//-----------------------------------------------------------------------------

s_key_action const tijd_2_key_action[] =
{
  {
    1,               // nr
    0,               // index
    &opt_alg.can_backbone,      // option
    (unsigned char *)&option_index_0,   // Optie index 
    lcd_sync_tijd_disp,  // display
    0,               // cursor
      &dummy_value,     // *value
    Dummy_Func,      // void (*number)(void); 
    Arrow_Change_Syst_Func, // void (*arrow)(void); 
    Dummy_Func,      // void (*enter)(void);
  },
  {
    1,               // nr
    1,               // index
    &opt_alg.can_backbone,      // option
    (unsigned char *)&option_index_0,   // Optie index 
    lcd_sync_tijd_disp,  // display
    &disp_cursor_klok_sync, // cursor
    &key_sync_tijd_value,     // *value
    Dummy_Func,      // void (*number)(void); 
    Arrow_Tijd_Sync,      // void (*arrow)(void); 
    Enter_Tijd_Sync,      // void (*enter)(void);
  },
};

s_screen screen_tijd_2;
s_screen const screen_tijd_2_default =
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
  &tijd_2_key_action[0], // first_action
  &tijd_2_key_action[sizeof(tijd_2_key_action)/sizeof(s_key_action) - 1], // last action 
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

void Control_Screen_Tijd_2(void)
{
  Control_Screen(&screen_tijd_2, &screen_tijd_2_default, 1, 3);
}

void If_Exist_Goto_Screen_Tijd_2(void)
{
  Control_Screen_Tijd_2();
  if (screen_tijd_2.nr_aantal)
    Next_Screen(&screen_tijd_2);
}


static void Arrow_Tijd_Sync(void)
{
  switch (key)
  {
    case LEFT:  
    case RIGHT:
      Increment_Func_Index();
      break;
    case UP:
      Can_Backbone_Free_Transmit_Value(&can_backbone_transmit_clock_in_sec);
      Increment_Scroll_Value();
      Can_Backbone_Transmit_Value_Init(&can_backbone_appl_node_alg.node, 
                                       BROADCAST, 
                                       NO_COMMAND,
                                       Can_Backbone_Transmit_Clock_In_Sec_Func,
                                       &can_backbone_transmit_clock_in_sec,
                                       0);
      break;
    case DOWN:
      Can_Backbone_Free_Transmit_Value(&can_backbone_transmit_clock_in_sec);
      Decrement_Scroll_Value();
      Can_Backbone_Transmit_Value_Init(&can_backbone_appl_node_alg.node, 
                                       BROADCAST, 
                                       NO_COMMAND,
                                       Can_Backbone_Transmit_Clock_In_Sec_Func,
                                       &can_backbone_transmit_clock_in_sec,
                                       0);
      break;
  }
}

static void Enter_Tijd_Sync(void)
{
  Increment_Func_Index_Enter_Value();
}
