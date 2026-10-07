// C__DISP_F2_0.C

#include "ch_define.h"

#include "ch_const.h"
#include "ch_disp.h"
#include "ch_disp_func.h"
#include "ch_disp_luchtmengkast_1.h"
#include "ch_key.h"
#include "ch_lcd_10.h"
#include "ch_lcd_14.h"
#include "ch_lcd_bitmap.h"
#include "ch_lcd_pixel.h"
#include "ch_luchtmengkast.h"
#include "ch_string.h"
#include "ch_disp_F2_0.h"

#define DISP_LUCHTMENGKAST 1

static void Arrow_Luchtmengkast_Groep_Func(void);

static unsigned char GroepNummer;

//*****************************************************************************
// SCHERM OPBOUW
//*****************************************************************************
//-----------------------------------------------------------------------------
// Header
static s_disp_agri_header const disp_header = { Disp_Draw_Agri_Header, FN2, EMPTY, &tekst.F2_10, HK_GEEN, 0, 1};
static void * const lcd_disp_header[] = { &disp_header, 0};
//-----------------------------------------------------------------------------
// Dummy screen
static unsigned char DummyScreen;
static void * const lcd_dummy_disp[] = { 0 };
//-----------------------------------------------------------------------------
// Algemeen
static s_disp_tekst const disp_14_perc_0 = { Disp_Draw_Tekst_L, 103, 39, &tekst.perc_7 };
static s_disp_tekst const disp_14_perc_1 = { Disp_Draw_Tekst_L, 215, 36, &tekst.perc_7 };
static s_disp_tekst const disp_14_perc   = { Disp_Draw_Tekst_L, 215, 54, &tekst.perc_7 };
//-----------------------------------------------------------------------------
// Luchtmengkast groep
static void Disp_Control_Luchtmengkast(void);

static s_disp_func const disp_control_luchtmengkast = { Disp_Control_Func, Disp_Control_Luchtmengkast };

static s_disp_tekst     const disp_luchtmengkast_str = { Disp_Draw_Tekst_L,  9, 20, &tekst.Luchtmengkast_groep_14 };
static s_disp_value_add const disp_luchtmengkast_val = { Disp_Draw_Value_Add, (SIZE_14 | LINKS), UCHAR, 0, &GroepNummer };
static s_disp_bitmap    const disp_inblaasvent_ico   = { Disp_Draw_Bitmap,   9, 30, &ico_vent       };
static s_disp_bitmap    const disp_buitenklep_ico    = { Disp_Draw_Bitmap, 148, 27, &buitenklep_ico };
static s_disp_value     const disp_inblaasvent_val   = { Disp_Draw_Value,  100, 51, (SIZE_20 | RECHTS), UCHAR, 0, &val_hr_alg.LuchtmengkastGroep[0].Inblaasvent.PositionPerc };
static s_disp_value     const disp_buitenklep_val    = { Disp_Draw_Value,  212, 39, (SIZE_10 | RECHTS), UCHAR, 0, &val_hr_alg.LuchtmengkastGroep[0].Buitenklep.PositionPerc  };

static s_disp_bitmap const disp_binnenklep_ico   = { Disp_Draw_Bitmap,  153, 45, &binnenklep_ico };
static s_disp_bitmap const disp_bovenklep_ico    = { Disp_Draw_Bitmap,  153, 45, &bovenklep_ico  };
static s_disp_bitmap const disp_verwarming_ico   = { Disp_Draw_Bitmap,  149, 45, &ico_IO_verwarm };
static s_disp_bitmap const disp_ventilatie_ico   = { Disp_Draw_Bitmap,  149, 45, &ico_IO_vent    };
static s_disp_value  const disp_binnenklep_val   = { Disp_Draw_Value,   212, 57, (SIZE_10 | RECHTS), UCHAR, 0, &val_hr_alg.LuchtmengkastGroep[0].Binnenklep.PositionPerc  };
static s_disp_value  const disp_verwarming_val   = { Disp_Draw_Value,   212, 57, (SIZE_10 | RECHTS), UCHAR, 0, &val_hr_alg.LuchtmengkastGroep[0].Verwarming.PositionPerc  };
static s_disp_value  const disp_afblaasvent_val  = { Disp_Draw_Value,   212, 57, (SIZE_10 | RECHTS), UCHAR, 0, &val_hr_alg.LuchtmengkastGroep[0].Afblaasvent.PositionPerc };

static void * const lcd_luchtmengkast_groep_disp[] = 
{
  &disp_control_luchtmengkast, &disp_luchtmengkast_str, &disp_space_14_L, &disp_luchtmengkast_val,
  &disp_inblaasvent_ico, &disp_inblaasvent_val, &disp_14_perc_0,
  &disp_buitenklep_ico,  &disp_buitenklep_val,  &disp_14_perc_1,
  0
};

static void Disp_Control_Luchtmengkast(void)
{
unsigned char i;
unsigned char OffsetX = 0;
unsigned char OffsetY = 0;
s_disp_bitmap bmp;
s_disp_value  val;
s_disp_tekst  txt;

  i = screen_ptr->nr - DISP_LUCHTMENGKAST;
  GroepNummer = i + 1;

  if (opt_app.LuchtmengkastGroep[i].TypeKlep == TYPE_KLEP_BINNEN_BUITEN)
  {
    Disp_Draw_Bitmap(&disp_binnenklep_ico);
    Disp_Draw_Value(&disp_binnenklep_val);
    Disp_Draw_Tekst_L(&disp_14_perc);
	OffsetY += 18;
  }
  else if (opt_app.LuchtmengkastGroep[i].BovenklepEnabled)
  {
    Disp_Draw_Bitmap(&disp_bovenklep_ico);
    Disp_Draw_Value(&disp_binnenklep_val);
    Disp_Draw_Tekst_L(&disp_14_perc);
	OffsetY += 18;
  }
  if (opt_app.LuchtmengkastGroep[i].VerwarmingEnabled)
  {
    bmp = disp_verwarming_ico;
	val = disp_verwarming_val;
	txt = disp_14_perc;
	bmp.y += OffsetY;
	val.y += OffsetY;
	txt.y += OffsetY;
    Disp_Draw_Bitmap(&bmp);
    Disp_Draw_Value(&val);
    Disp_Draw_Tekst_L(&txt);
	OffsetY += 18;
  }
  if (opt_app.LuchtmengkastGroep[i].AfblaasventEnabled)
  {
    val = disp_afblaasvent_val;
	txt = disp_14_perc;
    if (OffsetY < 36)
	{
      bmp = disp_ventilatie_ico;
	  bmp.y += OffsetY;
	  val.y += OffsetY;
	  txt.y += OffsetY;
      Disp_Draw_Bitmap(&bmp);
	}
	else
	{
	  val.x = 100;
	  val.y = 70;
	  txt.x = 103;
	  txt.y = 67;
	}
    Disp_Draw_Value(&val);
    Disp_Draw_Tekst_L(&txt);
	OffsetY += 18;
  }
}

//-----------------------------------------------------------------------------


s_key_action const F2_0_key_action[] =
{
  {
    0,                                      // nr
    0,                                      // index
    &DummyScreen,                           // option
    (unsigned char *)&option_index_0,       // Optie index 
    lcd_dummy_disp,                         // display
    0,                                      // cursor
    &dummy_value,                           // *value
    Dummy_Func,                             // void (*number)(void); 
    Dummy_Func,                             // void (*arrow)(void); 
    Dummy_Func,                             // void (*enter)(void);
  },
  { // Luchtmengkast groep 1
    DISP_LUCHTMENGKAST,                     // nr
    0,                                      // index
    &opt_app.LuchtmengkastGroep[0].Enabled, // option
    (unsigned char *)&option_index_0,       // Optie index 
    lcd_luchtmengkast_groep_disp,           // display
    0,                                      // cursor
    &dummy_value,                           // *value
    Dummy_Func,                             // void (*number)(void); 
    Arrow_Luchtmengkast_Groep_Func,         // void (*arrow)(void); 
    Dummy_Func,                             // void (*enter)(void);
  },
  { // Luchtmengkast groep 2
    DISP_LUCHTMENGKAST + 1,                 // nr
    0,                                      // index
    &opt_app.LuchtmengkastGroep[0].Enabled, // option
    (unsigned char *)&option_index_1,       // Optie index 
    lcd_luchtmengkast_groep_disp,           // display
    0,                                      // cursor
    &dummy_value,                           // *value
    Dummy_Func,                             // void (*number)(void); 
    Arrow_Luchtmengkast_Groep_Func,         // void (*arrow)(void); 
    Dummy_Func,                             // void (*enter)(void);
  },
  { // Luchtmengkast groep 3
    DISP_LUCHTMENGKAST + 2,                 // nr
    0,                                      // index
    &opt_app.LuchtmengkastGroep[0].Enabled, // option
    (unsigned char *)&option_index_2,       // Optie index 
    lcd_luchtmengkast_groep_disp,           // display
    0,                                      // cursor
    &dummy_value,                           // *value
    Dummy_Func,                             // void (*number)(void); 
    Arrow_Luchtmengkast_Groep_Func,         // void (*arrow)(void); 
    Dummy_Func,                             // void (*enter)(void);
  },
  { // Luchtmengkast groep 4
    DISP_LUCHTMENGKAST + 3,                 // nr
    0,                                      // index
    &opt_app.LuchtmengkastGroep[0].Enabled, // option
    (unsigned char *)&option_index_3,       // Optie index 
    lcd_luchtmengkast_groep_disp,           // display
    0,                                      // cursor
    &dummy_value,                           // *value
    Dummy_Func,                             // void (*number)(void); 
    Arrow_Luchtmengkast_Groep_Func,         // void (*arrow)(void); 
    Dummy_Func,                             // void (*enter)(void);
  },
};

s_screen screen_F2_0;
s_screen const screen_F2_0_default =
{
  1, // functie nr
  0, // index
  0, // max rel
  0, // rel actief
  0, // nummer weergegeven bin aantal ingeschakelde functie nummers
  0, // aantal ingeschakelde functie nummers
  lcd_disp_header,
  {0,0,0,0,0,0},
  0,3,19,
  0,3,19+28,
  0,3,19+28+28,
  &F2_0_key_action[0], // first_action
  &F2_0_key_action[sizeof(F2_0_key_action)/sizeof(s_key_action) - 1], // last action 
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

void Control_Screen_F2_0(void)
{
  DummyScreen = 0;
  Control_Screen(&screen_F2_0, &screen_F2_0_default, 0, 1);
  if (screen_F2_0.nr_aantal == 0)
  {
    DummyScreen = 1;
    Control_Screen(&screen_F2_0, &screen_F2_0_default, 1, 1);
  }
}

//-----------------------------------------------------------------------------
static void Arrow_Luchtmengkast_Groep_Func(void)
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
      If_Exist_Goto_Screen_Luchtmengkast_1(screen_ptr->nr - DISP_LUCHTMENGKAST);
      break;
  }
}


