// C__DISP_TIJD_1.C

#include "ch_define.h"

#include "ch_const.h"
#include "ch_disp.h"
#include "ch_disp_func.h"
#include "ch_disp_F1_0.h"
#include "ch_disp_value.h"
#include "ch_key.h"
#include "ch_lcd_10.h"
#include "ch_lcd_14.h"
#include "ch_lcd_bitmap.h"
#include "ch_lcd_pixel.h"
#include "ch_pc_com.h"
#include "ch_rtc.h"
#include "ch_string.h"
#include "ch_tijd.h"
#include "ch_disp_tijd_1.h"

static void Arrow_Hour(void);
static void Enter_Hour(void);
static void Arrow_Min(void);
static void Enter_Min(void);
static void Arrow_Dag_Maand_Jaar(void);
static void Enter_Dag_Maand_Jaar(void);
static void Arrow_Dagenteller(void);
static void Enter_Dagenteller(void);

static unsigned char reset_management_flag = 0;
static bit sync_flag = 0; // geeft aan dat tijd tijdens invoer veranderd is en zorgt bij verlaten van veld voor synchroniseren tijd

static s_disp_agri_header const disp_header = { Disp_Draw_Agri_Header, FN1, HD_FN, &tekst.Time_10, HK_GEEN, 0, 2};
static void * const lcd_disp_header[] = { &disp_header, 0};

static char vraagteken[] = "?";

//*****************************************************************************
// SCHERM OPBOUW
//*****************************************************************************
//-----------------------------------------------------------------------------
// basis opbouw lcd
/*
static s_disp_tekst     const disp_header_str_0  = { Disp_Draw_Tekst_L,  12, 12, &tekst.Fn_10 };
static s_disp_tekst_add const disp_header_str_1  = { Disp_Draw_Tekst_Add_L, &tekst.Time_10 };
static s_disp_tekst     const disp_header_str_2  = { Disp_Draw_Tekst_L, 204, 12, &header_string_2 };
static s_disp_value     const disp_header_val_2  = { Disp_Draw_Value,   237, 12, (SIZE_7 | RECHTS), INT, 0, &screen_tijd_1.nr };
static s_disp_block     const disp_header_invert = { Disp_Invert_Block,   0,  0, 239, 16 };
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
// Tijd
static s_disp_tekst const disp_tijd_str = { Disp_Draw_Tekst_L, 37, 20, &tekst.Tijd_10 };
static s_disp_value const disp_uur_val  = { Disp_Draw_Value,   (unsigned char)170,         20,   (SIZE_14 | RECHTS), INT, 0, &tijd.tm_hour };
static s_disp_tekst const disp_dp_str_0 = { Disp_Draw_Tekst_L, (unsigned char)170+3,       18,   &tekst_dp_14 };
static s_disp_value const disp_min_val  = { Disp_Draw_Value,   (unsigned char)170+3+21,    20,   (SIZE_14 | RECHTS), TIME_INT, 0, &tijd.tm_min };
static s_disp_value const disp_sec_val  = { Disp_Draw_Value,   (unsigned char)170+3+21+13, 20-7, (SIZE_7 | RECHTS),  TIME_INT, 0, &tijd.tm_sec };

static void * const lcd_tijd_disp[] = { &disp_tijd, &disp_tijd_str, &disp_uur_val, &disp_dp_str_0, &disp_min_val, &disp_sec_val, 0 };

static s_key_value const key_hour_value = { INT, 2, &tijd.tm_hour, &int_0, &int_23 };
static s_key_value const key_min_value  = { INT, 2, &tijd.tm_min,  &int_0, &int_59 };
//-----------------------------------------------------------------------------
// Datum
static s_disp_tekst     const disp_datum_str = { Disp_Draw_Tekst_L, 37, 20, &tekst.Datum_10 };
static s_disp_value     const disp_jaar_val  = { Disp_Draw_Value,  207, 20, (SIZE_14 | RECHTS), INT,      0, &tijd.tm_year };
static s_disp_value_add const disp_maand_val = { Disp_Draw_Value_Add,       (SIZE_14 | RECHTS), TIME_INT, 0, &tijd.tm_mon };
static s_disp_value_add const disp_dag_val   = { Disp_Draw_Value_Add,       (SIZE_14 | RECHTS), INT,      0, &tijd.tm_mday };

static void * const lcd_datum_disp[] = { &disp_kalender, &disp_datum_str, &disp_jaar_val, &disp_min_teken_14_R, &disp_maand_val, &disp_min_teken_14_R, &disp_dag_val, 0 };

static s_key_value const key_dag_value   = { INT, 2, &tijd.tm_mday, &int_1,    &int_31 };
static s_key_value const key_maand_value = { INT, 2, &tijd.tm_mon,  &int_1,    &int_12 };
static s_key_value const key_jaar_value  = { INT, 4, &tijd.tm_year, &int_1970, &int_2099 };
//-----------------------------------------------------------------------------
// Dagenteller
static s_disp_tekst const disp_dagenteller_str = { Disp_Draw_Tekst_L, 37, 20, &tekst.Dagenteller_10 };
static s_disp_value const disp_dagenteller_val = { Disp_Draw_Value,  207, 20, (SIZE_14 | RECHTS), INT, 0, &setp_alg.dagenteller };

static void * const lcd_dagenteller_disp[] = { &disp_dag, &disp_dagenteller_str, &disp_dagenteller_val, 0 };

static s_key_value const key_dagenteller_value = { INT, 3, &setp_alg.dagenteller, &int_m99, &int_999 };
//-----------------------------------------------------------------------------

s_key_action const tijd_1_key_action[] =
{
  {
    1,               // nr
    0,               // index
    (unsigned char *)&option_on,      // option
    (unsigned char *)&option_index_0,   // Optie index 
    lcd_tijd_disp,	 // display
    0,               // cursor
	&dummy_value,     // *value
    Dummy_Func,      // void (*number)(void); 
    Arrow_Change_Func, // void (*arrow)(void); 
    Dummy_Func,      // void (*enter)(void);
  },
  {
    1,               // nr
    1,               // index
    (unsigned char *)&option_on,      // option
    (unsigned char *)&option_index_0,   // Optie index 
    lcd_tijd_disp,	 // display
    &disp_cursor_170_23_8, // cursor
	&key_hour_value, // *value
    Number_Value,    // void (*number)(void); 
    Arrow_Hour,      // void (*arrow)(void); 
    Enter_Hour,      // void (*enter)(void);
  },
  {
    1,               // nr
    2,               // index
    (unsigned char *)&option_on,      // option
    (unsigned char *)&option_index_0,   // Optie index 
    lcd_tijd_disp,	 // display
    &disp_cursor_194_23_8, // cursor
	&key_min_value,     // *value
    Number_Value,      // void (*number)(void); 
    Arrow_Min,      // void (*arrow)(void); 
    Enter_Min,      // void (*enter)(void);
  },
  {
    2,               // nr
    0,               // index
    (unsigned char *)&option_on,      // option
    (unsigned char *)&option_index_0,   // Optie index 
    lcd_datum_disp,	 // display
    0,               // cursor
	&dummy_value,     // *value
    Dummy_Func,      // void (*number)(void); 
    Arrow_Change_Func, // void (*arrow)(void); 
    Dummy_Func,      // void (*enter)(void);
  },
  {
    2,               // nr
    1,               // index
    (unsigned char *)&option_on,      // option
    (unsigned char *)&option_index_0,   // Optie index 
    lcd_datum_disp,	 // display
    &disp_cursor_135_23_8, // cursor
	&key_dag_value,     // *value
    Number_Value,      // void (*number)(void); 
    Arrow_Dag_Maand_Jaar, // void (*arrow)(void); 
    Enter_Dag_Maand_Jaar,      // void (*enter)(void);
  },
  {
    2,               // nr
    2,               // index
    (unsigned char *)&option_on,      // option
    (unsigned char *)&option_index_0,   // Optie index 
    lcd_datum_disp,	 // display
    &disp_cursor_161_23_8, // cursor
	&key_maand_value,     // *value
    Number_Value,      // void (*number)(void); 
    Arrow_Dag_Maand_Jaar, // void (*arrow)(void); 
    Enter_Dag_Maand_Jaar,      // void (*enter)(void);
  },
  {
    2,               // nr
    3,               // index
    (unsigned char *)&option_on,      // option
    (unsigned char *)&option_index_0,   // Optie index 
    lcd_datum_disp,	 // display
    &disp_cursor_207_23_8, // cursor
	&key_jaar_value,     // *value
    Number_Value,      // void (*number)(void); 
    Arrow_Dag_Maand_Jaar, // void (*arrow)(void); 
    Enter_Dag_Maand_Jaar,      // void (*enter)(void);
  },
  {
    3,               // nr
    0,               // index
    (unsigned char *)&option_off,      // option
    (unsigned char *)&option_index_0,   // Optie index 
    lcd_dagenteller_disp,	 // display
    0,               // cursor
	&dummy_value,     // *value
    Dummy_Func,      // void (*number)(void); 
    Arrow_Change_Func, // void (*arrow)(void); 
    Dummy_Func,      // void (*enter)(void);
  },
  {
    3,               // nr
    1,               // index
    (unsigned char *)&option_off,      // option
    (unsigned char *)&option_index_0,   // Optie index 
    lcd_dagenteller_disp,	 // display
    &disp_cursor_207_23_8, // cursor
	&key_dagenteller_value,     // *value
    Number_Value,      // void (*number)(void); 
    Arrow_Dagenteller, // void (*arrow)(void); 
    Enter_Dagenteller,      // void (*enter)(void);
  },
};

s_screen screen_tijd_1;
s_screen const screen_tijd_1_default =
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
  &tijd_1_key_action[0], // first_action
  &tijd_1_key_action[sizeof(tijd_1_key_action)/sizeof(s_key_action) - 1], // last action 
  0, // change_flag
  0, // punt in normaal display
  0, // punt tijdens invoeren
  0, // aantal digits ingevoerd (punt telt niet mee)
  0, // value
  0, // min value
  0, // max value
  &screen_F1_0,  // vorige scherm
  0  // prev_next_func
};

void Control_Screen_Tijd_1(void)
// zorg voor een correcte inhoud in klimaat scherm 0
{
  Control_Screen(&screen_tijd_1, &screen_tijd_1_default, 1, 3);
}

void If_Exist_Goto_Screen_Tijd_1(void)
{
  Control_Screen_Tijd_1();
  if (screen_tijd_1.nr_aantal)
    Next_Screen(&screen_tijd_1);
}


static void Arrow_Hour(void)
{
  switch (key)
  {
    case LEFT:
      Arrow_Left_Value();
      if (sync_flag &&
          opt_alg.can_backbone &&
          (setp_alg.tijd_sync == 2))
      {
        sync_flag = 0;
        tijd_sync_switch = 1;    
      }
      break;
	case RIGHT: 
	  Increment_Func_Index(); 
      if (sync_flag &&
          opt_alg.can_backbone &&
          (setp_alg.tijd_sync == 2))
      {
        sync_flag = 0;
        tijd_sync_switch = 1;    
      }
	  break;
	case UP:
	  Increment_Value();
	  sync_flag = 1;
      RTC_Write();
      RTC_Read();
	  break;
	case DOWN:
	  Decrement_Value();
	  sync_flag = 1;
      RTC_Write();
      RTC_Read();
	  break;
  }
}

static void Enter_Hour(void)
{
  if (Enter_Value())
    sync_flag = 1;
  RTC_Write();
  RTC_Read();
  if (sync_flag &&
      opt_alg.can_backbone &&
      (setp_alg.tijd_sync == 2))
  {
    sync_flag = 0;
    tijd_sync_switch = 1;    
  }
  Increment_Func_Index();
}

static void Arrow_Min(void)
{
  switch (key)
  {
    case LEFT:
      Arrow_Left_Value();
      if (sync_flag &&
          opt_alg.can_backbone &&
          (setp_alg.tijd_sync == 2))
      {
        sync_flag = 0;
        tijd_sync_switch = 1;    
      }
      break;
	case RIGHT:
	  Increment_Func_Index();
      if (sync_flag &&
          opt_alg.can_backbone &&
          (setp_alg.tijd_sync == 2))
      {
        sync_flag = 0;
        tijd_sync_switch = 1;    
      }
	  break;
	case UP:
	  Increment_Value();
	  sync_flag = 1;
      tijd.tm_sec = 0;
      RTC_Write();
      RTC_Read();
	  break;
	case DOWN:
	  Decrement_Value();
	  sync_flag = 1;
      tijd.tm_sec = 0;
      RTC_Write();
      RTC_Read();
	  break;
  }
}

static void Enter_Min(void)
{
  if (Enter_Value())
    sync_flag = 1;
  tijd.tm_sec = 0;
  RTC_Write();
  RTC_Read();
  Increment_Func_Index();
  if (sync_flag &&
      opt_alg.can_backbone &&
      (setp_alg.tijd_sync == 2))
  {
    sync_flag = 0;
    tijd_sync_switch = 1;    
  }
}

static void Arrow_Dag_Maand_Jaar(void)
{
  switch (key)
  {
    case LEFT:
      Arrow_Left_Value();
      if (sync_flag &&
          opt_alg.can_backbone &&
          (setp_alg.tijd_sync == 2))
      {
        sync_flag = 0;
        tijd_sync_switch = 1;    
      }
      break;
	case RIGHT:
	  Increment_Func_Index();
      if (sync_flag &&
          opt_alg.can_backbone &&
          (setp_alg.tijd_sync == 2))
      {
        sync_flag = 0;
        tijd_sync_switch = 1;    
      }
	  break;
	case UP:
	  Increment_Value();
	  sync_flag = 1;
      RTC_Write();
      RTC_Read();
	  break;
	case DOWN:
	  Decrement_Value();
	  sync_flag = 1;
      RTC_Write();
      RTC_Read();
	  break;
  }
}

static void Enter_Dag_Maand_Jaar(void)
{
  if (Enter_Value())
	sync_flag = 1;
  RTC_Write();
  RTC_Read();
  Increment_Func_Index();
  if (sync_flag &&
      opt_alg.can_backbone &&
      (setp_alg.tijd_sync == 2))
  {
    sync_flag = 0;
    tijd_sync_switch = 1;    
  }
}

static void Arrow_Dagenteller(void)
{
  switch (key)
  {
    case LEFT:
      if (setp_alg.dagenteller == 0)
	  {
		reset_management_flag = 1;
		Increment_Func_Index();
	  }
	  else
	  {
	    reset_management_flag = 0;
	    Arrow_Left_Value();
	  }
      break;
	case RIGHT: 
	  if (setp_alg.dagenteller == 0)
	    reset_management_flag = 1;
	  else
	    reset_management_flag = 0;
	  Increment_Func_Index();
      break;
	case UP:
	  Increment_Value();
      if (setp_alg.dagenteller <= 0)
      {
        // value temp no min alarm = 1;
      }
	  break;
	case DOWN:
	  Decrement_Value();
      if (setp_alg.dagenteller <= 0)
      {
        // value temp no min alarm = 1;
      }
	  break;
  }
}

static void Enter_Dagenteller(void)
{
  if (Enter_Value())
  {
  }
  if (setp_alg.dagenteller <= 0)
  {
    // value temp no min alarm = 1;
  }
  if (setp_alg.dagenteller == 0)
    reset_management_flag = 1;
  else
    reset_management_flag = 0;
  Increment_Func_Index();
}

