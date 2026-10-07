// C__DISP.C

#include <stdarg.h>
//#include <time.h> // voor weergeven tijd

#include "ch_define.h"

#include "ch_alarm.h"
#include "ch_alg.h"
#include "ch_disp_alarm_0.h"
#include "ch_disp_alarmen_0.h"
#include "ch_disp_diagnose_0.h"
#include "ch_disp_func.h"
#include "ch_disp_F1_0.h"
#include "ch_disp_F2_0.h"
#include "ch_disp_F3_0.h"
#include "ch_disp_afblaasvent_1.h"
#include "ch_disp_afblaasvent_2.h"
#include "ch_disp_bovenklep_1.h"
#include "ch_disp_bovenklep_2.h"
#include "ch_disp_verwarming_1.h"
#include "ch_disp_verwarming_2.h"
#include "ch_disp_group_1.h"
#include "ch_disp_group_2.h"
#include "ch_disp_luchtmengkast_1.h"
#include "ch_disp_luchtmengkast_2.h"
#include "ch_disp_option_0.h"
#include "ch_disp_password.h"
#include "ch_disp_start.h"
#include "ch_disp_tijd_1.h"
#include "ch_disp_tijd_2.h"
#include "ch_disp_value.h"
#include "ch_key.h"
#include "ch_main.h"
#include "ch_lcd_hardware.h"
#include "ch_disp.h"

s_screen *screen_ptr = &screen_start;
s_screen *screen_F1_ptr = &screen_F1_0;
s_screen *screen_F2_ptr = &screen_F2_0;
s_screen *screen_F3_ptr = &screen_F3_0;
s_screen *screen_diagnose_ptr = &screen_diagnose_0;
s_screen *screen_alarmen_ptr = &screen_alarmen_0;
s_screen *screen_option_ptr = &screen_option_0;

unsigned char block_display_cursor_block = 0; // blokkeer weergeven blok cursor voor 3 seconden
static unsigned char last_key_func = 0;

// controleer of scherm in orde is en zoniet dan vernieuwen
// vernieuwd wordt ook als header_disp 0 wordt gemaakt
// TD - 13-09-2007: "do while" veranderd in "while" en "screen_option" naar boven gezet.
//                  "Get_Ptr" werd één keer teveel aangeroepen.
void Control_Screen(s_screen *scr_ptr, s_screen const *def_scr_ptr, 
                    unsigned char opnieuw, unsigned char rel_aantal)
{
s_screen *help_ptr;
s_key_action const *ptr;
unsigned char screen_option;
unsigned char loop;
unsigned char cnt = 0;
       
  if (!scr_ptr->header_disp)
    opnieuw = 1;
  if (!opnieuw) // controleer settings
  {
    for (loop = 0; loop < scr_ptr->rel_max; loop++)
    {
      ptr = scr_ptr->rel[loop].key_action;
      screen_option = *(unsigned char *)Get_Ptr((void *)ptr->option, *ptr->option_index);
      if ((ptr) && (screen_option == FN_OFF))
      {
        opnieuw = 1;
        break;
      }
    }
  }
  if (opnieuw)
  {
    *scr_ptr = *def_scr_ptr;
    ptr = scr_ptr->first_action;
    while ((unsigned long)ptr <= (unsigned long)scr_ptr->last_action)
    {
      screen_option = *(unsigned char *)Get_Ptr((void *)ptr->option, *ptr->option_index);
      if ((ptr->index == 0) && (screen_option))
      {
   	    scr_ptr->nr_aantal++;
        if (cnt < rel_aantal) 
        {
          // nieuw relatief scherm gevonden
          scr_ptr->rel[cnt].key_action = ptr;
          cnt++;
        }
      }
      ptr++;  
    }
//    scr_ptr->rel_max = cnt; // TD
    scr_ptr->rel_max = rel_aantal;
    scr_ptr->nr = scr_ptr->rel[0].key_action->nr;
    scr_ptr->index = scr_ptr->rel[0].key_action->index;
    help_ptr = screen_ptr;
    screen_ptr = scr_ptr;
    if (Search_Func(def_scr_ptr->nr) == 0)
	{
	  screen_ptr->index = 0;
	}
    screen_ptr = help_ptr;
  }
}

void Init_All_Screen(void)
{
  screen_F1_0.header_disp = 0;
  Control_Screen_F1_0();
  screen_F1_ptr = &screen_F1_0;

  screen_F2_0.header_disp = 0;
  Control_Screen_F2_0();
  screen_F2_ptr = &screen_F2_0;

  screen_F3_0.header_disp = 0;
  Control_Screen_F3_0();
  screen_F3_ptr = &screen_F3_0;

  screen_diagnose_0.header_disp = 0;
  Control_Screen_Diagnose_0();
  screen_diagnose_ptr = &screen_diagnose_0;

  screen_alarmen_0.header_disp = 0;
  Control_Screen_Alarmen_0();
  screen_alarmen_ptr = &screen_alarmen_0;

  screen_option_0.header_disp = 0;
  Control_Screen_Option_0();
  screen_option_ptr = &screen_option_0;

  screen_ptr = screen_F1_ptr;
  last_key_func = F1;
  Next_Screen(&screen_start);
}

void Refresh_Screen_Nr_Aantal(void)
// vaststellen maximaal aantal functienummers van scherm en
// functie weer terugzetten op goede positie func nummer en index
// TD - 13-09-2007: "do while" veranderd in "while" en "screen_option" naar boven gezet.
//                  "Get_Ptr" werd één keer teveel aangeroepen.
{
s_key_action const *ptr;
unsigned char screen_option;
       
  ptr = screen_ptr->first_action;
  screen_ptr->nr_aantal = 0;
  // opnieuw toewijzen functie toetsen
  while ((unsigned long)ptr <= (unsigned long)screen_ptr->last_action)
  {
    screen_option = *(unsigned char *)Get_Ptr((void *)ptr->option, *ptr->option_index);
    if ((ptr->index == 0) && (screen_option))
      screen_ptr->nr_aantal++;
    ptr++;
  }
  Search_Func_Index(screen_ptr->nr, screen_ptr->index);
}

//*****************************************************************************
static void Display_Save_Last_Screen(void)
{
  if (last_key_func != key_func)
  {  
    switch (last_key_func)
    {
      case F1:
        screen_F1_ptr = screen_ptr; 
        break;
      case F2:
        screen_F2_ptr = screen_ptr; 
        break;
      case F3:
        screen_F3_ptr = screen_ptr; 
        break;
      case F4: // alarmen
        screen_alarmen_ptr = screen_ptr; 
        break;
      case F5: // diganose
        screen_diagnose_ptr = screen_ptr; 
        break;
      case F6: // installatie scherm
        if (1) // nog in vullen variabele die gezet wordt als opties gewijzigd zijn
        {
          screen_F1_0.header_disp = 0; screen_F1_ptr = &screen_F1_0; // vernieuwen screen_F1_0
          screen_F2_0.header_disp = 0; screen_F2_ptr = &screen_F2_0; // vernieuwen screen_F2_0
          screen_F3_0.header_disp = 0; screen_F3_ptr = &screen_F3_0; // vernieuwen screen_F3_0
          screen_diagnose_0.header_disp = 0; screen_diagnose_ptr = &screen_diagnose_0; // vernieuwen screen_diagnose_0
          screen_alarmen_0.header_disp = 0;  screen_alarmen_ptr = &screen_alarmen_0; // vernieuwen screen_alarmen_0
        }
        // alle schermen opnieuw controleren als er opties gewijzigd zijn
        break;
    }
    last_key_func = key_func;
  }
}

unsigned char Control_If_Not_Curve_Screen(void) // return 1 als geen curve scherm  (voor blokkeren F1 tm F6)
{
/*
  if ((unsigned long)screen_ptr == (unsigned long)&screen_message)
    return (0);
  else
*/
    return (1);
}

void Display_Key_Proc(void)
{
  if ((unsigned long)(screen_ptr->rel[screen_ptr->rel_actief].key_action->number) == (unsigned long)Number_Code)
  {
	switch(key_func)
	{
      case F1: key = '1';  key_func = 0; break;
      case F2: key = '2';  key_func = 0; break;
      case F3: key = '3';  key_func = 0; break;
      case F4: key = '4';  key_func = 0; break;
      case F5: key = '5';  key_func = 0; break;
      case F6: key = '6';  key_func = 0; break;
	}
	switch(key)
	{
      case UP:    key = '7';  break;
      case LEFT:  key = '8';  break;
      case RIGHT: key = '9';  break;
      case DOWN:  key = '0';  break;
	}
  }
  
  if (key_func)
  {
    lcd_dimmen_cnt = 2;
    alarm_disp_blocked = ALARM_DISP_BLOCKED;
    if ((unsigned long)screen_ptr == (unsigned long)&screen_start)
      Prev_Screen();
    if (!install_flag && ((unsigned long)screen_ptr != (unsigned long)&screen_password))
    {
      if (password_enabled)
        password_enabled_delay = PASSWORD_DELAY;
      if ((unsigned long)screen_ptr == (unsigned long)&screen_alarm_0)
        Alarm_Return();
      if (last_key_func != key_func)
      {
  	    switch (key_func)
        {
          case F1: // klimaat
            if (Control_If_Not_Curve_Screen() && (screen_ptr->index == 0))
            {
              Display_Save_Last_Screen();
              Control_Screen_F1_0();
              screen_ptr = screen_F1_ptr; 
              last_key_func = key_func;
              lcd_refresh_fast_switch = 1;
            }
            break;
          case F2: // voer water licht
            if (Control_If_Not_Curve_Screen() && (screen_ptr->index == 0))
            {
              Display_Save_Last_Screen();
              Control_Screen_F2_0();
              screen_ptr = screen_F2_ptr; 
              last_key_func = key_func;
              lcd_refresh_fast_switch = 1;
            }
            break;
          case F3: // management
            if (Control_If_Not_Curve_Screen() && (screen_ptr->index == 0))
            {
              Display_Save_Last_Screen();
              Control_Screen_F3_0();
              screen_ptr = screen_F3_ptr; 
  	          last_key_func = key_func;
              lcd_refresh_fast_switch = 1;
            }
            break;
          case F4: // alarmen
            if (Control_If_Not_Curve_Screen() && (screen_ptr->index == 0))
            {
              Display_Save_Last_Screen();
              Control_Screen_Alarmen_0();
              screen_ptr = screen_alarmen_ptr; 
  	          last_key_func = key_func;
              lcd_refresh_fast_switch = 1;
            }
            break;
          case F5: // diagnose
            if (Control_If_Not_Curve_Screen() && (screen_ptr->index == 0))
            {
              Display_Save_Last_Screen();
              Control_Screen_Diagnose_0();
              screen_ptr = screen_diagnose_ptr; 
  	          last_key_func = key_func;
              lcd_refresh_fast_switch = 1;
            }
            break;
          case F6: // install scherm / systeem functies
            if (Control_If_Not_Curve_Screen() && (screen_ptr->index == 0))
            {
              if (((unsigned long)screen_ptr == (unsigned long)&screen_F1_0) ||
                  ((unsigned long)screen_ptr == (unsigned long)&screen_F2_0) ||
                  ((unsigned long)screen_ptr == (unsigned long)&screen_F3_0) ||
                  ((unsigned long)screen_ptr == (unsigned long)&screen_diagnose_0) ||
                  ((unsigned long)screen_ptr == (unsigned long)&screen_alarmen_0))
              {
                Display_Save_Last_Screen();
                Password_Init();
                Control_Screen_Option_0();
                screen_ptr = &screen_option_0; 
  	            last_key_func = key_func;
                lcd_refresh_fast_switch = 1;
              }
              else if (((unsigned long)screen_ptr == (unsigned long)&screen_tijd_1) && (screen_ptr->index == 0))
              {
                If_Exist_Goto_Screen_Tijd_2();
                lcd_refresh_fast_switch = 1;
              }
              else if (((unsigned long)screen_ptr == (unsigned long)&screen_group_1) && (screen_ptr->index == 0))
              {
                If_Exist_Goto_Screen_Group_2();
                lcd_refresh_fast_switch = 1;
              }
              else if (((unsigned long)screen_ptr == (unsigned long)&screen_luchtmengkast_1) && (screen_ptr->index == 0))
              {
                If_Exist_Goto_Screen_Luchtmengkast_2();
                lcd_refresh_fast_switch = 1;
              }
              else if (((unsigned long)screen_ptr == (unsigned long)&screen_afblaasvent_1) && (screen_ptr->index == 0))
              {
                If_Exist_Goto_Screen_Afblaasvent_2();
                lcd_refresh_fast_switch = 1;
              }
              else if (((unsigned long)screen_ptr == (unsigned long)&screen_bovenklep_1) && (screen_ptr->index == 0))
              {
                If_Exist_Goto_Screen_Bovenklep_2();
                lcd_refresh_fast_switch = 1;
              }
              else if (((unsigned long)screen_ptr == (unsigned long)&screen_verwarming_1) && (screen_ptr->index == 0))
              {
                If_Exist_Goto_Screen_Verwarming_2();
                lcd_refresh_fast_switch = 1;
              }
              else if ((screen_ptr->index == 0) &&
                       ((unsigned long)screen_ptr == (unsigned long)&screen_tijd_2) ||
                       ((unsigned long)screen_ptr == (unsigned long)&screen_group_2) ||
					   ((unsigned long)screen_ptr == (unsigned long)&screen_luchtmengkast_2) ||
					   ((unsigned long)screen_ptr == (unsigned long)&screen_bovenklep_2) ||
					   ((unsigned long)screen_ptr == (unsigned long)&screen_afblaasvent_2) ||
					   ((unsigned long)screen_ptr == (unsigned long)&screen_verwarming_2))
              {
                Prev_Screen();
                lcd_refresh_fast_switch = 1;
              }
            }
            break;
          case PREV:
          case NEXT:  
            if (screen_ptr->prev_next_func != 0)
              (screen_ptr->prev_next_func)();
            lcd_refresh_fast_switch = 1;
            break;
        }    
      }
    }
    key_func = 0;
  }
  if (key)
  {
    lcd_dimmen_cnt = 2;
    alarm_disp_blocked = ALARM_DISP_BLOCKED;
    if ((unsigned long)screen_ptr == (unsigned long)&screen_start)
      Prev_Screen();
    if (password_enabled)
      password_enabled_delay = PASSWORD_DELAY;
    if (screen_ptr->rel_max > 0)
//    if ((screen_ptr->nr_aantal > 0) && (screen_ptr->rel_max > 0)) // TD
    {
      if (((key >= '0') && (key <= '9')) ||
          (key == PLUS_MIN) ||
          (key == '.'))
        (*screen_ptr->rel[screen_ptr->rel_actief].key_action->number)();
      else if ((key == UP) || (key == DOWN)) // UP, DOWN
      {
        block_display_cursor_block = 3;
        (*screen_ptr->rel[screen_ptr->rel_actief].key_action->arrow)();
      }
      else if ((key == LEFT) || (key == RIGHT)) // LEFT, RIGHT
      {
        block_display_cursor_block = 0;
        (*screen_ptr->rel[screen_ptr->rel_actief].key_action->arrow)();
      }
      else if (key == OK)
      {
        block_display_cursor_block = 0;
        (*screen_ptr->rel[screen_ptr->rel_actief].key_action->enter)();
      }
    }
    lcd_refresh_fast_switch = 1;
    key = 0;
  }
}
