// C__DISP_ALARMEN_ACTIEF_1.C

#include "ch_define.h"

#include "ch_alarm.h"
#include "ch_const.h"
#include "ch_disp.h"
#include "ch_disp_alarm_0.h"
#include "ch_disp_func.h"
#include "ch_key.h"
#include "ch_lcd_10.h"
#include "ch_lcd_bitmap.h"
#include "ch_lcd_pixel.h"
#include "ch_string.h"
#include "ch_disp_alarmen_actief_1.h"

int change_actief_alarm = 0;

static void Disp_Control_Alarm_Actief(void);
static void Arrow_Alarmen_Actief(void);
static void Enter_Alarmen_Actief(void);

static int alarm_index;

static s_disp_agri_header      const disp_header =        { Disp_Draw_Agri_Header, FN4, EMPTY, &tekst.Alarmen_Actief_10, HK_GEEN, 0, 1};
static s_disp_func             const disp_control =       { Disp_Control_Func, Disp_Control_Alarm_Actief };
static s_disp_bitmap_option_on const disp_time_on_alarm = { Disp_Draw_Bitmap_Option_On, 37+3, REGEL_4+10, &ico_alarm, LONG, &alarm_actueel.on };
static s_disp_bitmap_option_on const disp_time_on_bmp =   { Disp_Draw_Bitmap_Option_On, 58+3, REGEL_4+10, &ico_switch_on, LONG, &alarm_actueel.on }; 
static s_disp_time             const disp_time_on =       { Disp_Draw_Time,             75+3, REGEL_4+19, SIZE_10, &alarm_actueel.on };

static void * const lcd_disp_header[] = { &disp_control, &disp_header, &disp_time_on_alarm, &disp_time_on_bmp, &disp_time_on, 0 };
static void *lcd_alarmen_actief[20];

s_key_action alarmen_actief_1_key_action;

s_key_action const const_alarmen_actief_1_key_action =
{
  0,                                // nr
  0,                                // index
  (unsigned char *)&option_on,      // option
  (unsigned char *)&option_index_0, // Optie index 
  lcd_alarmen_actief,               // display
  0,                                // cursor
  &dummy_value,                     // *value
  Dummy_Func,                       // void (*number)(void); 
  Arrow_Alarmen_Actief,             // void (*arrow)(void); 
  Enter_Alarmen_Actief,             // void (*enter)(void);
};

s_screen screen_alarmen_actief_1;
s_screen const screen_alarmen_actief_1_default =
{
  0, // functie nr
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
  &alarmen_actief_1_key_action, // first_action
  &alarmen_actief_1_key_action, // last action 
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

int Count_Nr_Actief_Alarm(void)
{
int loop;
int index = alarm_hr_alg.alarmen.index;
int cnt = 0;

  for (loop = 0; loop < ALARM_DISP_VALUES; loop++)
  {
    if ((alarm_hr_alg.alarmen.al[index].code) &&
        (alarm_hr_alg.alarmen.al[index].mask & MASK_ALG_DISP) &&
        (alarm_hr_alg.alarmen.al[index].off == 0))
    {
      cnt++;
    }
    if (index <= 0)
      index = ALARM_DISP_VALUES - 1;
    else
      index--;
  }
  return (cnt);
}

static int Get_Nr_Actief_Alarm(int index_actief)
{
int loop;
int index = alarm_hr_alg.alarmen.index;
int cnt = 0;

  if (index_actief == -1)
    return (0);

  for (loop = 0; loop < ALARM_DISP_VALUES; loop++)
  {
    if(alarm_hr_alg.alarmen.al[index].code &&
      (alarm_hr_alg.alarmen.al[index].mask & MASK_ALG_DISP) &&
      (alarm_hr_alg.alarmen.al[index].off == 0))
    {
      if (index == index_actief)
        return (cnt);
      cnt++;
    }
    if (index <= 0)
      index = ALARM_DISP_VALUES - 1;
    else
      index--;
  }
  return (0);
}

static int Get_Actief_Alarm(void)
{
int loop;
int index = alarm_hr_alg.alarmen.index;

  for (loop = 0; loop < ALARM_DISP_VALUES; loop++)
  {
    if(alarm_hr_alg.alarmen.al[index].code &&
      (alarm_hr_alg.alarmen.al[index].mask & MASK_ALG_DISP) &&
      (alarm_hr_alg.alarmen.al[index].off == 0))
    {
      return (index);
    }
    if (index <= 0)
      index = ALARM_DISP_VALUES - 1;
    else
      index--;
  }
  return (-1); 
}

int Get_Prev_Actief_Alarm(int index)
{
int old_index = index;
int loop;

  if (index == -1)
    return (-1);

  for (loop = 0; loop < ALARM_DISP_VALUES; loop++)
  {
    if (index == alarm_hr_alg.alarmen.index)
      return (old_index);
    else if (index >= ALARM_DISP_VALUES - 1)
      index = 0;
    else
      index++;
    if(alarm_hr_alg.alarmen.al[index].code &&
           (alarm_hr_alg.alarmen.al[index].mask & MASK_ALG_DISP) &&
           (alarm_hr_alg.alarmen.al[index].off == 0))
      return (index);
  }
  return (-1); 
}

int Get_Next_Actief_Alarm(int index)
{
int old_index = index;
int loop;

  if (index == -1)
    return (-1);

  for (loop = 0; loop < ALARM_DISP_VALUES; loop++)
  {
    if (index <= 0)
      index = ALARM_DISP_VALUES - 1;
    else
      index--;
    if (index == alarm_hr_alg.alarmen.index)
      return (old_index);
    else if(alarm_hr_alg.alarmen.al[index].code &&
           (alarm_hr_alg.alarmen.al[index].mask & MASK_ALG_DISP) &&
           (alarm_hr_alg.alarmen.al[index].off == 0))
      return (index);
  }
  return (-1); 
}

static int Refresh_Actief_Alarm(int nr_actief)
{
int loop;
int index = alarm_hr_alg.alarmen.index;
int last_index = -1;
int cnt = 0;

  for (loop = 0; loop < ALARM_DISP_VALUES; loop++)
  {
    if(alarm_hr_alg.alarmen.al[index].code &&
       (alarm_hr_alg.alarmen.al[index].mask & MASK_ALG_DISP) &&
       (alarm_hr_alg.alarmen.al[index].off == 0))
    {
      if (cnt == nr_actief)
        return (index);
      last_index = index;
      cnt++;
    }
    if (index <= 0)
      index = ALARM_DISP_VALUES - 1;
    else
      index--;
  }
  return (last_index);
}

static s_disp_func const disp_alarm_state = { Disp_Control_Func, Disp_Draw_Alarm_State };
static void * const add_alarm_state[] =     { &disp_alarm_state, 0 };

static void Disp_Alarm_Actief(void)
{
  alarm_actueel = (alarm_index != -1) ? alarm_hr_alg.alarmen.al[alarm_index] : alarm_geen;
  alarmen_actief_1_key_action.nr = screen_alarmen_actief_1.nr = alarm_actueel.code;
  Alarm_Zet_Disp(lcd_alarmen_actief, screen_alarmen_actief_1.nr);
  Alarm_Add_To_Disp(lcd_alarmen_actief, add_alarm_state);
  screen_alarmen_actief_1.nr_aantal = Count_Nr_Actief_Alarm();
  screen_alarmen_actief_1.nr_actief = Get_Nr_Actief_Alarm(alarm_index);
}

void Control_Screen_Alarmen_Actief_1(void)
{
  alarmen_actief_1_key_action = const_alarmen_actief_1_key_action;
  Control_Screen(&screen_alarmen_actief_1, &screen_alarmen_actief_1_default, 1, 1);
  alarm_index = Get_Actief_Alarm();
  change_actief_alarm = 0;
}

void If_Exist_Goto_Screen_Alarmen_Actief_1(void)
{
  Control_Screen_Alarmen_Actief_1();
  Next_Screen(&screen_alarmen_actief_1);
}

void Arrow_Goto_Alarm_Actief(void)
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
      If_Exist_Goto_Screen_Alarmen_Actief_1();
      break;
  }
}

static void Disp_Control_Alarm_Actief(void)
{
  if(alarm_index != -1)
  {
    if(alarm_hr_alg.alarmen.al[alarm_index].off != 0)
    {
      alarm_index = Refresh_Actief_Alarm(screen_alarmen_actief_1.nr_actief);
      change_actief_alarm = 0;
    }
  }
  else
  {
    alarm_index = Get_Actief_Alarm();
    change_actief_alarm = 0;
  }
  Disp_Alarm_Actief();
}

static void Arrow_Alarmen_Actief(void)
{
unsigned char next_state = 0;

  if(change_actief_alarm)
  {
    switch (key)
    {
      case UP:    
        if(alarm_hr_alg.alarmen.al[alarm_index].mask & MASK_AL_HARD)
        {
          alarm_hr_alg.alarmen.al[alarm_index].state = AL_HARD;
          alarm_hr_alg.alarmen.al[alarm_index].hard = HARD_ALARM;
        }
        break;
      case RIGHT:    
        if(alarm_hr_alg.alarmen.al[alarm_index].mask & MASK_AL_ZACHT)
        {
          alarm_hr_alg.alarmen.al[alarm_index].state = AL_ZACHT;
          alarm_hr_alg.alarmen.al[alarm_index].hard = ZACHT_ALARM;
        }
        break;
      case DOWN:    
        if(alarm_hr_alg.alarmen.al[alarm_index].mask & MASK_AL_ONDERD)
          alarm_hr_alg.alarmen.al[alarm_index].state = AL_ONDERDRUKT;
        break;
    }
    change_actief_alarm = 0;    
  }
  else
  {
    switch (key)
    {
      case UP:   alarm_index = Get_Prev_Actief_Alarm(alarm_index);  break;  
      case DOWN: alarm_index = Get_Next_Actief_Alarm(alarm_index);  break;  
      case LEFT: Prev_Screen(); break;
      case RIGHT: break;
    }
  }
}

static void Enter_Alarmen_Actief(void)
{
  if (alarm_index != -1)
  {
    if (change_actief_alarm)
    {
      if(alarm_hr_alg.alarmen.al[alarm_index].mask & MASK_AL_WISSEN)
      {
        Alarm_Reset(&alarm_hr_alg.alarmen.al[alarm_index]);
        alarm_index = Get_Next_Actief_Alarm(alarm_index);   
      }
      change_actief_alarm = 0;
    }
    else
    { // controleren of alarm of wijzigen mogelijk is
      switch (alarm_hr_alg.alarmen.al[alarm_index].mask & ( MASK_AL_HARD | MASK_AL_ZACHT | MASK_AL_ONDERD | MASK_AL_WISSEN))
      {
        case 0x0:                         break;
        case MASK_AL_HARD:                break;
        case MASK_AL_ZACHT:               break;
        case MASK_AL_ONDERD:              break;
        default: change_actief_alarm = 1; break;
      }      
    }
  }
}
