// C__DISP_OPT_IO_H2MC_3.C

#include "ch_define.h"

#include "ch_alg.h"
#include "ch_const.h"
#include "ch_disp.h"
#include "ch_disp_func.h"
#include "ch_disp_option_1.h"
#include "ch_disp_opt_IO_2.h"
#include "ch_disp_password.h"
#include "ch_disp_value.h"
#include "ch_IO_H2MC_board.h"
#include "ch_key.h"
#include "ch_lcd_10.h"
#include "ch_lcd_14.h"
#include "ch_lcd_20.h"
#include "ch_lcd_bitmap.h"
#include "ch_lcd_pixel.h"
#include "ch_string.h"
#include "ch_disp_opt_IO_H2MC_3.h"

#define INDEX_MOTOR_CONTROL_ALG 0

static unsigned char const motor_control_possible[MOTOR_CONTROL_MAX] =
{  
// 0 is niet te selecteren; 1 is te selecteren
// MOTOR_CONTROL_EMPTY moet altijd 1 zijn
// er moet altijd minimaal een 1 aanwezig zijn
  1, // MOTOR_CONTROL_EMPTY
  1, // MOTOR_CONTROL_RAAM
  1  // MOTOR_CONTROL_DOEK
};

static void Arrow_Board_Func(void);
static void Arrow_Board_Value(void);
static void Enter_Board_Value(void);
static void Arrow_Board_OK_Value(void);

static void Arrow_Motor_Control_Func(void);
static void Enter_Motor_Control_Func(void);

static void Copy_Motor_Control_To_Array(void);
static void Copy_Array_To_Motor_Control(void);

static void Arrow_End_IO_Func(void);

static s_disp_agri_header const disp_header = { Disp_Draw_Agri_Header, FN6, EMPTY, &tekst_leeg, HK_GEEN, 0, 4};
static s_disp_tekst           const disp_header_string_0 = { Disp_Draw_Tekst_L,  12, 12, &tekst_inst.Opties_bord_10 };
static s_disp_tekst_array_add const disp_header_string_1 = { Disp_Draw_Tekst_Array_Add_L, &tekst_IO_H2MC_10, UCHAR, &index_IO, IO_H2MC_MAX+1 };
static void * const lcd_disp_header[] = { &disp_header_string_0, &disp_space_10_L, &disp_header_string_1, &disp_header, 0 };

static unsigned char const index_IO_max = IO_H2MC_MAX;
static unsigned char index_IO_new;
static unsigned char board_flag;
//---------------------------------------------------------------------------------------
// Nummer IO module
static s_disp_tekst       const disp_nummer_IO_module        = { Disp_Draw_Tekst_L,         0, 22, &tekst_inst.Nummer_IO_Module_14 };
static s_disp_tekst       const disp_ronde_openings_haak_str = { Disp_Draw_Tekst_L,         0, 50, &tekst_ronde_openings_haak_14 };
static s_disp_tekst_array const disp_index_IO_string         = { Disp_Draw_Tekst_Array_L, 126, 75, &tekst_IO_H2MC_14, UCHAR, &index_IO, IO_H2MC_MAX+1 };
static s_disp_tekst_array const disp_index_IO_string_1       = { Disp_Draw_Tekst_Array_L, 126, 75, &tekst_IO_H2MC_14, UCHAR, &index_IO_new, IO_H2MC_MAX+1 };

static void * const lcd_IO_H2MC_1_disp[] =   
{ 
  &disp_nummer_IO_module,
  &disp_ronde_openings_haak_str, &disp_verwijderen_inst_str, &disp_space_14_L, 
  &disp_is_teken_14_L, &disp_space_14_L, &disp_min_teken_14_L, &disp_space_14_L, &disp_ronde_sluit_haak_14_L,
  &disp_index_IO_string, 0
};

static s_key_value const key_lcd_index_IO =  { UCHAR, 1, &index_IO, &uchar_0, &index_IO_max };
//---------------------------------------------------------------------------------------
// Message screen

static void * const lcd_IO_H2MC_1_disp_1[] =
{
  &disp_nummer_IO_module, &disp_index_IO_string_1, &disp_messagebox_bevestig,
  &disp_zeker_weten_inst_str, &disp_bevestig_arrow_up_bmp, &disp_bevestig_is_teken, &disp_space_10_L, &disp_ja_inst_str, 0
};
//---------------------------------------------------------------------------------------
// Motor control
static s_disp_tekst const disp_motor_control_str = { Disp_Draw_Tekst_L, 56, 22, &tekst_inst.Motor_Control_14 };

static void * const lcd_motor_control_disp[] =
{
  &disp_uitgang_inst, &disp_motor_inst, &disp_motor_control_str, 
  &disp_box_pos1_bmp, &disp_box_pos2_bmp, 
  &disp_block,
  &disp_IO[0], &disp_IO[1],
  &disp_value_IO, &disp_unit_IO, 0
};
//---------------------------------------------------------------------------------------
// Alarm contact
static void Arrow_Alarm_Contact_Func(void);
static void Arrow_Alarm_Contact_Value(void);

static unsigned char alarm_contact; // variabele invullen bij voorgaande en vorige functie

static s_disp_tekst        const disp_alarm_contact_str = { Disp_Draw_Tekst_L,       37, 22, &tekst_inst.Alarm_Contact_14 };
static s_disp_bitmap_array const disp_alarm_contact_bmp = { Disp_Draw_Bitmap_Array, 190, 57, bmp_false_true, UCHAR, &alarm_contact, 2 };

static void * const lcd_alarm_contact_disp[] = { &disp_alarm_contact_str, &disp_alarm_contact_bmp, 0 };

static s_key_value const key_alarm_contact = { UCHAR, 1, &alarm_contact, &uchar_0, &uchar_1 };
//---------------------------------------------------------------------------------------


s_key_action const opt_IO_H2MC_3_key_action[] =
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
    lcd_IO_H2MC_1_disp,               // display
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
    lcd_IO_H2MC_1_disp,               // display
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
    lcd_IO_H2MC_1_disp_1,             // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Board_OK_Value,             // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  { // instellen motor control
    2,                                // nr
    INDEX_MOTOR_CONTROL_ALG,          // index
    &board_flag,                      // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_motor_control_disp,           // display
    &disp_cursor,                     // cursor
    &key_IO,                          // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Motor_Control_Func,         // void (*arrow)(void); 
    Enter_Motor_Control_Func,         // void (*enter)(void);
  },
  { // alarm contact 
    3,                                // nr
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
    3,                                // nr
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

s_screen screen_opt_IO_H2MC_3;
s_screen const screen_opt_IO_H2MC_3_default =
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
  &opt_IO_H2MC_3_key_action[0], // first_action
  &opt_IO_H2MC_3_key_action[sizeof(opt_IO_H2MC_3_key_action)/sizeof(s_key_action) - 1], // last action 
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

void Control_Screen_Option_IO_H2MC_3(void)
{
  index_IO = screen_ptr->nr - DISP_IO_H2MC;  
  if (password_enabled & PASSWORD_ENABLED_OPT_MASK)
    start_flag = end_flag = 1;
  else
    start_flag = end_flag = 0;
  board_flag = (index_IO < IO_H2MC_MAX) ? 1 : 0;
  Control_Screen(&screen_opt_IO_H2MC_3, &screen_opt_IO_H2MC_3_default, 1, 1);
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
  while (screen_ptr->nr != index_IO + DISP_IO_H2MC)
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
      Copy_Motor_Control_To_Array();
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
        if (screen_ptr->value == IO_H2MC_MAX)
         return;
        else if (index_IO == screen_ptr->value)
          return;
        else if ((opt_io.IO_H2MC[screen_ptr->value].board_component.option & 0x000F) == 0)
          return;
      }
    case DOWN:  
      while (1)
      {
        screen_ptr->value--;
        if (screen_ptr->value < screen_ptr->min_value)
          screen_ptr->value = screen_ptr->max_value;
        screen_ptr->change_flag = 1;
        if (screen_ptr->value == IO_H2MC_MAX)
          return;
        else if (index_IO == screen_ptr->value)
          return;
        else if ((opt_io.IO_H2MC[screen_ptr->value].board_component.option & 0x000F) == 0)
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
          if (index_IO_new == IO_H2MC_MAX)
          {
            // wis gegevens van IO modules
            opt_io.IO_H2MC[index_IO] = default_opt_io.IO_H2MC[0];
            index_IO = index_IO_new;
            board_flag = 0;
            IO_H2MC_init_switch[index_IO] = 1;
            Refresh_Screen_Nr_Aantal();
          }
          else
          {
            // copieer gegevens van IO module oud naar module nieuw en wis oude module
            opt_io.IO_H2MC[index_IO_new] = opt_io.IO_H2MC[index_IO];
            opt_io.IO_H2MC[index_IO] = default_opt_io.IO_H2MC[0];
            IO_H2MC_init_switch[index_IO_new] = 1;
            IO_H2MC_init_switch[index_IO] = 1;
            index_IO = index_IO_new;
          }
          break;
        case IO_H2MC_MAX:
          // maak nieuw module aan
          index_IO = index_IO_new;
          opt_io.IO_H2MC[index_IO].board_component.option = 1;
          board_flag = 1;
          IO_H2MC_init_switch[index_IO] = 1;
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
static void Set_Value_Unit_IO_Motor_Control(void)
{
  // set unit
  Set_Unit_IO_Motor_Control();
  // set value
  disp_value_IO.x = 192;
  disp_value_IO.y = 75;
  disp_value_IO.size = (SIZE_14 | RECHTS);
  disp_value_IO.type = INT;
  if (index_array)
  {
    switch (array[index_array - 1])
    {
      case MOTOR_CONTROL_EMPTY:   
        disp_value_IO.value = &long_0;
        disp_value_IO.point = 0;
        disp_value_IO.func = Disp_Draw_Code;
        break;
      case MOTOR_CONTROL_RAAM:
      case MOTOR_CONTROL_DOEK:
        disp_value_IO.value = &val_hr_alg.IO_H2MC[index_IO].motor_control[index_array - 1].Value;
        disp_value_IO.point = 1;
        disp_value_IO.func = Disp_Draw_Value;
        break;
    }
  }
  else
  {
    disp_value_IO.value = &long_0;
    disp_value_IO.point = 0;
    disp_value_IO.func = Disp_Draw_Code;
  }
}

static void Copy_Motor_Control_To_Array(void)
{
int loop;
s_option_motor_control *ptr = &opt_io.IO_H2MC[index_IO].motor_control[0];

  index_array = 0;
  max_array_index = IO_H2MC_MOTOR_CONTROL;
  disp_cursor = disp_cursor_array[index_array];
  disp_block = disp_block_array[index_array];

  for (loop = 0; loop < IO_H2MC_MOTOR_CONTROL; loop++)
  {
    array[loop] = ptr->OptType;
    disp_IO[loop].func = Disp_Draw_Bitmap_Array;
    disp_IO[loop].x = 2 + loop * 19;
    disp_IO[loop].y = 40;
    disp_IO[loop].data_array = ico_IO_motor_control_array;
    disp_IO[loop].type = UCHAR;
    disp_IO[loop].index = &array[loop];
    disp_IO[loop].max = MOTOR_CONTROL_MAX;
    ptr++;
  }
  Set_Value_Unit_IO_Motor_Control();
}

static void Copy_Array_To_Motor_Control(void)
{
int loop;
s_option_motor_control *ptr = &opt_io.IO_H2MC[index_IO].motor_control[0];

  for (loop = 0; loop < IO_H2MC_MOTOR_CONTROL; loop++)
  {
    ptr->OptType = array[loop];
    ptr++;
  }
}

static void Set_Component_Motor_Control(void)
{
  switch (array[index_array - 1])
  {
    case MOTOR_CONTROL_EMPTY: 
      opt_io.IO_H2MC[index_IO].motor_control[index_array - 1] = option_motor_control_empty;
      value.IO_H2MC[index_IO].motor_control[index_array - 1].ctrl |= 0x0004;
      break;
    case MOTOR_CONTROL_RAAM:
      opt_io.IO_H2MC[index_IO].motor_control[index_array - 1] = option_motor_control_raam;
      value.IO_H2MC[index_IO].motor_control[index_array - 1].ctrl |= 0x0004;
	  value.IO_H2MC[index_IO].motor_control[index_array - 1].ctrl |= 0x0008;
      break;
    case MOTOR_CONTROL_DOEK:
      opt_io.IO_H2MC[index_IO].motor_control[index_array - 1] = option_motor_control_doek;
      value.IO_H2MC[index_IO].motor_control[index_array - 1].ctrl |= 0x0004;
	  value.IO_H2MC[index_IO].motor_control[index_array - 1].ctrl |= 0x0008;
      break;
  }
}

static void Set_Key_IO_Motor_Control(void)
{
  key_IO.type = UCHAR;
  key_IO.max_digits = 1;
  key_IO.value = &array[index_array - 1];
  key_IO.min_value = &key_min;
  key_IO.max_value = &key_max;
  key_min = 0;
  key_max = MOTOR_CONTROL_MAX - 1;
  Get_Value();
}

static void Inc_Motor_Control(void)
{
unsigned int index = screen_ptr->value;
  
  do 
  {
    index++;
    index %= MOTOR_CONTROL_MAX;
  }
  while (motor_control_possible[index] == 0);
  screen_ptr->value = index;
}

static void Dec_Motor_Control(void)
{
unsigned int index = screen_ptr->value;
  
  do 
  {
    index += MOTOR_CONTROL_MAX - 1;
    index %= MOTOR_CONTROL_MAX;
  }
  while (motor_control_possible[index] == 0);
  screen_ptr->value = index;
}

static void Arrow_Motor_Control_Func(void)
{
  switch (key)
  {
    case UP:
      if (index_array == 0)
      {
        Copy_Array_To_Motor_Control();      
        Decrement_Func();
      }
      else
      {
        if (password_enabled & PASSWORD_ENABLED_OPT_MASK)
        {
          Inc_Motor_Control();
          option_change_flag = 1;
          screen_ptr->change_flag = 0;
          array[index_array - 1] = screen_ptr->value;
          Set_Value_Unit_IO_Motor_Control();
          Set_Component_Motor_Control();
        }
      }
      break;
    case DOWN:
      if (index_array == 0)
      {
        Copy_Array_To_Motor_Control();      
        Increment_Func();
        alarm_contact = opt_io.IO_H2MC[index_IO].alarm;
      }
      else
      {
        if (password_enabled & PASSWORD_ENABLED_OPT_MASK)
        {
          Dec_Motor_Control();
          option_change_flag = 1;
          screen_ptr->change_flag = 0;
          array[index_array - 1] = screen_ptr->value;
          Set_Value_Unit_IO_Motor_Control();
          Set_Component_Motor_Control();
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
          Set_Key_IO_Motor_Control();
        Set_Value_Unit_IO_Motor_Control();
      }
      break;
    case RIGHT:
      index_array++;
      index_array %= max_array_index + 1;
      disp_cursor = disp_cursor_array[index_array];
      disp_block = disp_block_array[index_array];
      if (index_array)
        Set_Key_IO_Motor_Control();
      Set_Value_Unit_IO_Motor_Control();
      break;
  }
}

static void Enter_Motor_Control_Func(void)
{
  index_array = 0;
  disp_cursor = disp_cursor_array[index_array];
  disp_block = disp_block_array[index_array];
  Set_Value_Unit_IO_Motor_Control();
}

//*****************************************************************************
static void Arrow_Alarm_Contact_Func(void)
{
  switch (key)
  {
    case UP:
      Copy_Motor_Control_To_Array();    
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
  opt_io.IO_H2MC[index_IO].alarm = alarm_contact;
}

//*****************************************************************************
static void Arrow_End_IO_Func(void)
{
  switch (key)
  {
    case UP:
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

