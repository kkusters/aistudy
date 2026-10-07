// C__DISP_OPT_VRIJGAVE_VENT_2.C

#include <string.h>

#include "ch_define.h"

#include "ch_alg.h"
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
#include "ch_motorgroep.h"
#include "ch_string.h" 
#include "ch_disp_opt_vrijgave_vent_2.h"

static void SetTabelProperties(void);

static void Arrow_Aantal_Value(void);
static void Enter_Aantal_Value(void);
static void Arrow_Tabel_Func(void);
static void Arrow_Tabel_Value(void);
static void Enter_Tabel_Value(void);
static void Arrow_End_Func(void);

static int FanNr[6];
static int NrFans;
static int Rij, Kolom;
static int Rijen, Kolommen;

static s_disp_cursor disp_tabel_cursor = { 255, 0, 0, 0, 0};

//*****************************************************************************
// SCHERM OPBOUW
//*****************************************************************************
//-----------------------------------------------------------------------------------------------------------
// Header
static s_disp_agri_header const disp_header = { Disp_Draw_Agri_Header, FN6, EMPTY, &tekst_inst.Opties_Vrijgave_ventilatoren_10, HK_GEEN, 0, 3};

static void * const lcd_disp_header[] = { &disp_header, 0 };
//-----------------------------------------------------------------------------------------------------------
// Aantal groepen
static s_disp_tekst const disp_aantal_str = { Disp_Draw_Tekst_L, 37, 22, &tekst_inst.Aantal_14 };
static s_disp_value const disp_aantal_val = { Disp_Draw_Value,  207, 75, (SIZE_14 | RECHTS), UCHAR, 0, &opt_app.VrijgaveVent.Aantal };

static void * const lcd_aantal_disp[] = { &disp_vent_inst, &disp_aantal_str, &disp_aantal_val, 0 };

static s_key_value const key_aantal_val = { UCHAR, 2, &opt_app.VrijgaveVent.Aantal, &uchar_0, &uchar_12 };
//-----------------------------------------------------------------------------
// Digitale ingangen vrijgave
static s_disp_tekst const disp_vrijgave_str = { Disp_Draw_Tekst_L, 47, 22, &tekst_inst.Vrijgave_14 };
static s_disp_board_IO_Selection const disp_dig_in_vrijgave_sel = { Disp_Draw_Board_IO_Select, opt_app.VrijgaveVent.DigInVrijgave, MAX_VRIJGAVE, &opt_app.VrijgaveVent.Aantal, 0, DIGITAL_INPUT_ID, DIG_IN_VENT, DigInNotUsed };

static void * const lcd_dig_in_vrijgave[] = { &disp_vent_inst, &disp_vrijgave_str, &disp_dig_in_vrijgave_sel, 0 };
//-----------------------------------------------------------------------------------------------------------
// Invoer tabel
static s_disp_value const disp_const_val = { Disp_Draw_Value, 31, 49, (SIZE_7 | RECHTS), INT, 0, &FanNr[0] };
static s_disp_value disp_vent_nr;

static void DispVrijgaveTabelFunc(void)
{
int i, j, x1, y1, x2, y2;
int input;
int Mask;

  LCD_Set_Offset(0,0);
  for (i = 0; i < Kolommen; i++)
  {
    x1 = 34 + (i * 12);
	y1 = 30;
    LCD_Draw_Bitmap(x1,y1,input_nr_ico[i]);
    x1 = 34 + (i * 12);
	y1 = 40;
	x2 = 46 + (i * 12);
	y2 = 40 + (Rijen * 10);
    LCD_Draw_Black_Vierkant(x1, y1, x2, y2);
  }
  for (i = 0; i < Rijen; i++)
  {
    x1 = 13;
	y1 = 40 + (i * 10);
	x2 = 34 + (Kolommen * 12);
	y2 = 50 + (i * 10);
    LCD_Draw_Black_Vierkant(x1, y1, x2, y2);
    disp_vent_nr = disp_const_val;
	disp_vent_nr.y += (i * 10);
	disp_vent_nr.value = &FanNr[i];
	Disp_Draw_Value(&disp_vent_nr);
    Mask = 0x0001;
	for (j = 0; j < Kolommen; j++)
	{
      if (opt_app.VrijgaveVent.Ventilator[FanNr[i] - 1] & Mask)
	  {
        x1 = 38 + (j * 12);
    	y1 = 43 + (i * 10);
        LCD_Draw_Bitmap(x1,y1,&bolletje_ico);
	  }
	  Mask <<= 1;
	}
  }
  if (screen_ptr->index > 0)
  {
    LCD_Draw_Bitmap(97,19,&input_small_ico);
    LCD_Draw_Bitmap(53,17,&vent_small_ico);
    disp_vent_nr = disp_const_val;
	disp_vent_nr.x = 86;
	disp_vent_nr.y = 27;
	disp_vent_nr.value = &FanNr[Rij];
	Disp_Draw_Value(&disp_vent_nr);
	input = Kolom + 1;
	disp_vent_nr.x = 121;
	disp_vent_nr.y = 27;
	disp_vent_nr.value = &input;
	Disp_Draw_Value(&disp_vent_nr);
  }
}

static s_disp_func const disp_vrijgave_tabel_ctl = { Disp_Control_Func, DispVrijgaveTabelFunc };

static void * const lcd_vrijgave_tabel[] = { &disp_vrijgave_tabel_ctl, 0 };
//-----------------------------------------------------------------------------
// IO Ventilatoren
static s_disp_tekst const disp_ventilatoren_str = { Disp_Draw_Tekst_L, 47, 22, &tekst_inst.Ventilatoren_14 };
static s_disp_board_IO_Selection const disp_vent_485_sel = { Disp_Draw_Board_IO_Select, opt_app.VrijgaveVent.RS485Bus, MAX_VRIJGAVE, (unsigned char *)&uchar_12, 1, RS485_BUS_ID, 0, DummyNotUsed };

static void * const lcd_ventilatoren[] = { &disp_vent_inst, &disp_ventilatoren_str, &disp_vent_485_sel, 0 };
//-----------------------------------------------------------------------------


s_key_action const opt_vrijgave_vent_2_key_action[] =
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
  { // Digitale ingang vrijgave
    2,                                // nr
    0,                                // index
    &opt_app.VrijgaveVent.Aantal,     // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_dig_in_vrijgave,              // display
    &disp_cursor,                     // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_Select_Func,             // void (*arrow)(void); 
    Enter_IO_Select_Func,             // void (*enter)(void);
  },
  //-----------------------------
  { // Vrijgave tabel
    3,                                // nr
    0,                                // index
    &opt_app.VrijgaveVent.Aantal,     // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_vrijgave_tabel,               // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Tabel_Func,                 // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  { // Vrijgave tabel
    3,                                // nr
    1,                                // index
    &opt_app.VrijgaveVent.Aantal,     // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_vrijgave_tabel,               // display
    &disp_tabel_cursor,               // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Tabel_Value,                // void (*arrow)(void); 
    Enter_Tabel_Value,                // void (*enter)(void);
  },
  //-----------------------------
  { // RS485Bus ventilatoren
    4,                                // nr
    0,                                // index
    &opt_app.VrijgaveVent.Aantal,     // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_ventilatoren,                 // display
    &disp_cursor,                     // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_Select_Func,             // void (*arrow)(void); 
    Enter_IO_Select_Func,             // void (*enter)(void);
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
s_screen screen_opt_vrijgave_vent_2;
s_screen const screen_opt_vrijgave_vent_2_default =
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
  &opt_vrijgave_vent_2_key_action[0], // first_action
  &opt_vrijgave_vent_2_key_action[sizeof(opt_vrijgave_vent_2_key_action)/sizeof(s_key_action) - 1], // last action 
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
void Control_Screen_Option_Vrijgave_Vent_2(void)
{
  if (password_enabled & PASSWORD_ENABLED_OPT_MASK)
    start_flag = end_flag = 1;
  else
    start_flag = end_flag = 0;
  Control_Screen(&screen_opt_vrijgave_vent_2, &screen_opt_vrijgave_vent_2_default, 1, 1);

  SetTabelProperties();
}

//-----------------------------------------------------------------------------
void CheckOptionsVrijgave(void)
{
unsigned char Enabled;
unsigned int Mask;
int i;

  Enabled = 0;
  for (i = 0; i < opt_app.NumberMotorgroups; i++)
  {
    if (opt_app.Motorgroup[i].Type == TYPE_VENT)
	{
	  Enabled = 1;
	  break;
	}
  }
  if (Enabled == 0)
    opt_app.VrijgaveVent.Aantal = 0;

  if (opt_app.VrijgaveVent.Aantal == 0)
  {
    opt_app.VrijgaveVent = default_opt_app.VrijgaveVent;
  }
  else
  {
    for (i = opt_app.VrijgaveVent.Aantal; i < MAX_VRIJGAVE; i++)
    {
      opt_app.VrijgaveVent.DigInVrijgave[i] = IO_empty;
    }
    Mask = ((long)0x0001 << opt_app.VrijgaveVent.Aantal) - 1;
    for (i = 0; i < MAX_DEVICE; i++)
    {
      opt_app.VrijgaveVent.Ventilator[i] &= Mask;
	}
  }
}

//-----------------------------------------------------------------------------
static void GetNrFans(void)
{
int i;
int Group;

  NrFans = 0;
  for (i = 0; i < MAX_DEVICE; i++)
  {
    if (opt_app.Device[i].Enabled)
	{
	  Group = (int)opt_app.Device[i].GroupNumber - 1;
	  if ((Group >= 0) && (opt_app.Motorgroup[Group].Type == TYPE_VENT))
        NrFans++;
    }
  }
}

//-----------------------------------------------------------------------------
static void GetRijenRolommen(void)
{
  if (NrFans > 6)
    Rijen = 6;
  else
    Rijen = NrFans;
  Kolommen = opt_app.VrijgaveVent.Aantal;
}

//-----------------------------------------------------------------------------
static void SetFirstFanNr(void)
{
int i;
int Group;
int Aantal = 0;

  for (i = 0; i < MAX_DEVICE; i++)
  {
    if (opt_app.Device[i].Enabled)
	{
	  Group = (int)opt_app.Device[i].GroupNumber - 1;
	  if ((Group >= 0) && (opt_app.Motorgroup[Group].Type == TYPE_VENT))
      {
	    FanNr[Aantal] = i + 1;
	    Aantal++;
	    if (Aantal >= 6)
	      return;
	  }
	}
  }
}

static void SetLastFanNr(void)
{
int i;
int Group;
int Aantal = 5;

  for (i = MAX_DEVICE - 1; i >= 0; i--)
  {
    if (opt_app.Device[i].Enabled)
	{
	  Group = (int)opt_app.Device[i].GroupNumber - 1;
	  if ((Group >= 0) && (opt_app.Motorgroup[Group].Type == TYPE_VENT))
      {
  	    FanNr[Aantal] = i + 1;
    	Aantal--;
	    if (Aantal < 0)
	      return;
	  }
	}
  }
}

static unsigned char SetNextFanNr(void)
{
int i, j;
int Group;

  for (i = FanNr[5]; i < MAX_DEVICE; i++)
  {
    if (opt_app.Device[i].Enabled)
	{
	  Group = (int)opt_app.Device[i].GroupNumber - 1;
	  if ((Group >= 0) && (opt_app.Motorgroup[Group].Type == TYPE_VENT))
      {
	    for (j = 0; j < 5; j++)
	      FanNr[j] = FanNr[j+1];
	    FanNr[5] = i + 1;
        return (1);
	  }
	}
  }
  return(0);
}

static unsigned char SetPrevFanNr(void)
{
int i, j;
int Group;

  for (i = FanNr[0] - 2; i >= 0; i--)
  {
    if (opt_app.Device[i].Enabled)
	{
	  Group = (int)opt_app.Device[i].GroupNumber - 1;
	  if ((Group >= 0) && (opt_app.Motorgroup[Group].Type == TYPE_VENT))
      {
	    for (j = 5; j > 0; j--)
	      FanNr[j] = FanNr[j-1];
	    FanNr[0] = i + 1;
        return (1);
	  }
	}
  }
  return(0);
}

//-----------------------------------------------------------------------------
static void SetTabelProperties(void)
{
  GetNrFans();
  GetRijenRolommen();
  SetFirstFanNr();
}

//-----------------------------------------------------------------------------
static void SetTabelCursor(void)
{
  block_display_cursor_block = 0;
  disp_tabel_cursor.dx  = 11;
  disp_tabel_cursor.dy1 =  1;
  disp_tabel_cursor.dy2 =  9;
  disp_tabel_cursor.x   = 42 + (Kolom * 12);
  disp_tabel_cursor.y   = 30 + (Rij * 10);
}

//-----------------------------------------------------------------------------
static void Arrow_Aantal_Value(void)
{
  Arrow_Option_Value();
  switch (key)
  {
    case LEFT:
    case RIGHT:
	  CheckOptionsVrijgave();
	  SetTabelProperties();
      Refresh_Screen_Nr_Aantal();
      break;
    case UP:
    case DOWN:
      break;
  }
}

static void Enter_Aantal_Value(void)
{
  Increment_Func_Index_Enter_Option_Value();
  CheckOptionsVrijgave();
  SetTabelProperties();
  Refresh_Screen_Nr_Aantal();
}

//-----------------------------------------------------------------------------
static void Arrow_Tabel_Func(void)
{
  switch (key)
  {
    case RIGHT:
	  if (NrFans == 0)
	    return;
	  else
	  {
	    Rij = Kolom = 0;
     	SetTabelCursor();
	  }
      break;
    case LEFT:
    case UP:
    case DOWN:
	  SetFirstFanNr();
      break;
  }
  Arrow_Option_Func();
}

static void Arrow_Tabel_Left(void)
{
  if (Kolom > 0)
    Kolom--;
  else
	Arrow_Left_Value();
}

static void Arrow_Tabel_Right(void)
{
  if (Kolom < Kolommen - 1)
    Kolom++;
  else if (Rij < Rijen - 1)
  {
    Rij++;
    Kolom = 0;
  }
  else if (NrFans <= 6)
  {
    Increment_Func_Index();
  }
  else if (SetNextFanNr() == 0)
  {
    SetFirstFanNr();
    Increment_Func_Index();
  }
  else
  {
    Kolom = 0;
  }
}

static void Arrow_Tabel_Up(void)
{
  if (Rij > 0)
    Rij--;
  else if (NrFans <= 6)
    Rij = NrFans - 1;
  else if (SetPrevFanNr() == 0)
  {
    SetLastFanNr();
    Rij = 5;
  }
}

static void Arrow_Tabel_Down(void)
{
  if (Rij < Rijen - 1)
    Rij++;
  else if (NrFans <= 6)
    Rij = 0;
  else if (SetNextFanNr() == 0)
  {
    SetFirstFanNr();
    Rij = 0;
  }
}

static void Arrow_Tabel_Value(void)
{
  switch (key)
  {
	case LEFT : Arrow_Tabel_Left();  break;
	case RIGHT: Arrow_Tabel_Right(); break;
    case UP   : Arrow_Tabel_Up();    break;
	case DOWN : Arrow_Tabel_Down();  break;
  }
  SetTabelCursor();
}

static void Enter_Tabel_Value(void)
{
unsigned int Mask;
int index;

  Mask = 0x0001 << Kolom;
  index = FanNr[Rij] - 1;
  opt_app.VrijgaveVent.Ventilator[index] ^= Mask;
  Arrow_Tabel_Right();
  SetTabelCursor();
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
