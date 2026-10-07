// C__DISP_ALARMEN_HISTORIE_1.C

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
#include "ch_disp_alarmen_historie_1.h"

static void Disp_Control_Alarm_Historie(void);
static void Arrow_Alarmen_Historie(void);

static int alarm_index;

static s_disp_agri_header const disp_header =              { Disp_Draw_Agri_Header, FN4, EMPTY, &tekst.Alarmen_Historie_10, HK_GEEN, 0, 1};
static s_disp_func const disp_control =                    { Disp_Control_Func, Disp_Control_Alarm_Historie };
static s_disp_bitmap_option_on const disp_time_on_alarm =  { Disp_Draw_Bitmap_Option_On, 40, REGEL_4+10, &ico_alarm, LONG, &alarm_actueel.on };
static s_disp_bitmap_option_on const disp_time_on_bmp =    { Disp_Draw_Bitmap_Option_On, 61, REGEL_4+10, &ico_switch_on, LONG, &alarm_actueel.on }; 
static s_disp_time const disp_time_on =                    { Disp_Draw_Time,             78, REGEL_4+19, SIZE_10, &alarm_actueel.on };
static s_disp_bitmap_option_on const disp_time_off_alarm = { Disp_Draw_Bitmap_Option_On, 40, REGEL_5+10, &ico_alarm, LONG, &alarm_actueel.off };
static s_disp_bitmap_option_on const disp_time_off_bmp =   { Disp_Draw_Bitmap_Option_On, 61, REGEL_5+10, &ico_switch_off, LONG, &alarm_actueel.off }; 
static s_disp_time const disp_time_off =                   { Disp_Draw_Time,             78, REGEL_5+19, SIZE_10, &alarm_actueel.off };

static void * const lcd_disp_header[] = 
{ 
  &disp_control, &disp_header, 
  &disp_time_on_alarm,  &disp_time_on_bmp,  &disp_time_on, 
  &disp_time_off_alarm, &disp_time_off_bmp, &disp_time_off, 0
};   
static void *lcd_alarmen_historie[20];

s_key_action alarmen_historie_1_key_action;

s_key_action const const_alarmen_historie_1_key_action =
{
  0,    
  0,              
  (unsigned char *)&option_on,      
  (unsigned char *)&option_index_0,   
  lcd_alarmen_historie,  
  0,            
  &dummy_value, 
  Dummy_Func,   
  Arrow_Alarmen_Historie,
  Dummy_Func,    
};

s_screen screen_alarmen_historie_1;

s_screen const screen_alarmen_historie_1_default =
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
  &alarmen_historie_1_key_action, // first_action
  &alarmen_historie_1_key_action, // last action 
  0, // change_flag
  0, // punt in normaal display
  0, // punt tijdens invoeren
  0, // aantal digits ingevoerd (punt telt niet mee)
  0, // value
  0, // min value
  0, // max value
  0  // vorige scherm
};

static int Count_Nr_Alarm(void)
{
int loop;
int index = alarm_hr_alg.alarmen.index;
int cnt = 0;

  for (loop = 0; loop < ALARM_DISP_VALUES; loop++)
  {
    if ((alarm_hr_alg.alarmen.al[index].code) &&
        (alarm_hr_alg.alarmen.al[index].mask & MASK_ALG_DISP))
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

static int Get_Nr_Alarm(int index_actief)
{
int loop;
int index = alarm_hr_alg.alarmen.index;
int cnt = 0;

  if (index_actief == -1)
    return (0);

  for (loop = 0; loop < ALARM_DISP_VALUES; loop++)
  {
    if ((alarm_hr_alg.alarmen.al[index].code) &&
        (alarm_hr_alg.alarmen.al[index].mask & MASK_ALG_DISP))
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

static int Get_Alarm(void)
{
int loop;
int index = alarm_hr_alg.alarmen.index;

  for (loop = 0; loop < ALARM_DISP_VALUES; loop++)
  {
    if ((alarm_hr_alg.alarmen.al[index].code) &&
        (alarm_hr_alg.alarmen.al[index].mask & MASK_ALG_DISP))
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

static int Get_Prev_Alarm(int index)
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
    if ((alarm_hr_alg.alarmen.al[index].code) &&
        (alarm_hr_alg.alarmen.al[index].mask & MASK_ALG_DISP))
      return (index);
  }
  return (-1); 
}

static int Get_Next_Alarm(int index)
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
    else if ((alarm_hr_alg.alarmen.al[index].code) &&
             (alarm_hr_alg.alarmen.al[index].mask & MASK_ALG_DISP))
      return (index);
  }
  return (-1); 
}

static void Disp_Alarm(void)
{
  alarm_actueel = (alarm_index != -1) ? alarm_hr_alg.alarmen.al[alarm_index] : alarm_geen;
  alarmen_historie_1_key_action.nr = screen_alarmen_historie_1.nr = alarm_actueel.code;
  Alarm_Zet_Disp(lcd_alarmen_historie, screen_alarmen_historie_1.nr);
  screen_alarmen_historie_1.nr_aantal = Count_Nr_Alarm();
  screen_alarmen_historie_1.nr_actief = Get_Nr_Alarm(alarm_index);
}

void Control_Screen_Alarmen_Historie_1(void)
{
  alarmen_historie_1_key_action = const_alarmen_historie_1_key_action;
  Control_Screen(&screen_alarmen_historie_1, &screen_alarmen_historie_1_default, 1, 1);
  alarm_index = Get_Alarm();
  Disp_Alarm();
}

void If_Exist_Goto_Screen_Alarmen_Historie_1(void)
{
  Control_Screen_Alarmen_Historie_1();
  if (screen_alarmen_historie_1.nr_aantal)
    Next_Screen(&screen_alarmen_historie_1);
}

void Arrow_Goto_Alarm_Historie(void)
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
      If_Exist_Goto_Screen_Alarmen_Historie_1();
      break;
  }
}

static void Disp_Control_Alarm_Historie(void)
{
  Disp_Alarm();
}

static void Arrow_Alarmen_Historie(void)
{
  switch (key)
  {
    case UP:
      alarm_index = Get_Prev_Alarm(alarm_index);
      Disp_Alarm();
      break;
    case DOWN:
      alarm_index = Get_Next_Alarm(alarm_index);
      Disp_Alarm();
      break;
    case LEFT:
      Prev_Screen();
      break;
    case RIGHT:
      break;
  }
}
