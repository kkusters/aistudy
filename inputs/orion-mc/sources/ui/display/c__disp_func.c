// C__DISP_FUNC.C

#include "ch_define.h"

#include "ch_alg.h"
#include "ch_disp.h" 
#include "ch_disp_option_1.h"
#include "ch_disp_password.h"
#include "ch_disp_value.h"
#include "ch_key.h"
#include "ch_disp_func.h"

void Increment_Func_Index(void)   // Spring naar het volgende invoerveld
{
s_key_action const *ptr = screen_ptr->rel[screen_ptr->rel_actief].key_action;
unsigned char screen_option = *(unsigned char *)Get_Ptr((void *)ptr->option, *ptr->option_index);

  if (ptr == screen_ptr->last_action)
  {
    while ((ptr->index != 0) || (screen_option == FN_OFF))
    {
      ptr--;
      screen_option = *(unsigned char *)Get_Ptr((void *)ptr->option, *ptr->option_index);
    }
  }
  else
  {
    do
    {
      if (ptr == screen_ptr->last_action)
        break;
      else
      {
        ptr++;
        screen_option = *(unsigned char *)Get_Ptr((void *)ptr->option, *ptr->option_index);
      }
    }
    while ((ptr->nr == screen_ptr->nr) && (screen_option == FN_OFF));
    if ((ptr->nr != screen_ptr->nr) || ((screen_option == FN_OFF) && (ptr == screen_ptr->last_action)))
    {
      do
      {
        ptr--;
        screen_option = *(unsigned char *)Get_Ptr((void *)ptr->option, *ptr->option_index);
      }
      while ((ptr->index != 0) || (screen_option == FN_OFF));
    }
  }
  screen_ptr->rel[screen_ptr->rel_actief].key_action = ptr; 
  screen_ptr->index = ptr->index;
  Get_Value();
}

void Decrement_Func_Index(void)
{
s_key_action const *ptr = screen_ptr->rel[screen_ptr->rel_actief].key_action;
unsigned char screen_option = *(unsigned char *)Get_Ptr((void *)ptr->option, *ptr->option_index);

  if (screen_ptr->index == 0)
  {
    do 
    {
      if (ptr == screen_ptr->last_action)
        break;
      else
      {
        ptr++;
        screen_option = *(unsigned char *)Get_Ptr((void *)ptr->option, *ptr->option_index);
      }
    }
    while (ptr->nr == screen_ptr->nr);
    if (ptr->nr != screen_ptr->nr) 
    {
      do
      {
        ptr--;
        screen_option = *(unsigned char *)Get_Ptr((void *)ptr->option, *ptr->option_index);
      }
      while (screen_option == FN_OFF);
    }
  }
  else
  {
    do
    {
      ptr--;
      screen_option = *(unsigned char *)Get_Ptr((void *)ptr->option, *ptr->option_index);
    }
    while (screen_option == FN_OFF);
  }
  screen_ptr->rel[screen_ptr->rel_actief].key_action = ptr;
  screen_ptr->index = ptr->index;
  Get_Value();
}

void Decrement_Func(void)
{
s_key_action const *ptr;
unsigned char screen_option;

  if (screen_ptr->rel_actief > 0)
  {
    screen_ptr->rel_actief--;
    screen_ptr->nr_actief--;
    screen_ptr->nr = screen_ptr->rel[screen_ptr->rel_actief].key_action->nr;
  }
  else if (screen_ptr->nr_actief > 0)
  {
/* TD
    if (screen_ptr->rel_max >= 3)
      screen_ptr->rel[2].key_action = screen_ptr->rel[1].key_action;
    if (screen_ptr->rel_max >= 2)
      screen_ptr->rel[1].key_action = screen_ptr->rel[0].key_action;
*/
    if ((screen_ptr->nr_aantal >= 3) && (screen_ptr->rel_max >= 3))
      screen_ptr->rel[2].key_action = screen_ptr->rel[1].key_action;
    if ((screen_ptr->nr_aantal >= 2) && (screen_ptr->rel_max >= 2))
      screen_ptr->rel[1].key_action = screen_ptr->rel[0].key_action;
    ptr = screen_ptr->rel[0].key_action;
    while (1)
    {
      if ((unsigned long)ptr > (unsigned long)screen_ptr->first_action) // ter beveiliging scherm ptr
      {
        ptr--;
        screen_option = *(unsigned char *)Get_Ptr((void *)ptr->option, *ptr->option_index);
        if ((ptr->index == 0) && (screen_option))
        {
          screen_ptr->rel[0].key_action = ptr;
          screen_ptr->nr_actief--;
          screen_ptr->nr = screen_ptr->rel[0].key_action->nr;
          return;
        }
      }
      else 
        return;
    }
  }
}

void Increment_Func(void)
{
s_key_action const *ptr;
unsigned char screen_option;

//  if (screen_ptr->rel_actief + 1 < screen_ptr->rel_max) // TD
  if ((screen_ptr->rel_actief + 1 < screen_ptr->rel_max) && (screen_ptr->rel_actief + 1 < screen_ptr->nr_aantal))
  {
    screen_ptr->rel_actief++;
    screen_ptr->nr_actief++;
    screen_ptr->nr = screen_ptr->rel[screen_ptr->rel_actief].key_action->nr;
  }
  else 
  if (screen_ptr->nr_actief + 1 < screen_ptr->nr_aantal)
  {
    if (0 < screen_ptr->rel_actief)
      screen_ptr->rel[0].key_action = screen_ptr->rel[1].key_action;
    if (1 < screen_ptr->rel_actief)
      screen_ptr->rel[1].key_action = screen_ptr->rel[2].key_action;
    ptr = screen_ptr->rel[screen_ptr->rel_actief].key_action;
    while (1)
    {
      if ((unsigned long)ptr < (unsigned long)screen_ptr->last_action) // ter beveiliging scherm ptr
      {
        ptr++;
        screen_option = *(unsigned char *)Get_Ptr((void *)ptr->option, *ptr->option_index);
        if ((ptr->index == 0) && (screen_option))
        {
          screen_ptr->rel[screen_ptr->rel_actief].key_action = ptr;
          screen_ptr->nr_actief++;
          screen_ptr->nr = screen_ptr->rel[screen_ptr->rel_actief].key_action->nr;
          return;
        }
      }
      else
        return;
    }
  }
}

//*****************************************************************************
void Increment_Func_Index_Enter_Value(void)
{
  if (screen_ptr->change_flag)
    Enter_Value();
  Increment_Func_Index();
}
                                
void Increment_Func_Index_Enter_Option_Value(void)
{
  if (screen_ptr->change_flag)
  {
    if (Enter_Value())
      option_change_flag = 1;
  }
  Increment_Func_Index();
}
                                
//*****************************************************************************
// TD - 13-09-2007: "screen_option" naar boven gezet.
//                  "Get_Ptr" werd één keer teveel aangeroepen.
/*
char Search_Func_Index(unsigned int nr, unsigned char index)
{
s_key_action const *ptr;
unsigned char screen_option;
int cnt = 0;
int nr_actief = 0;

  ptr = screen_ptr->first_action;
  screen_option = *(unsigned char *)Get_Ptr((void *)ptr->option, *ptr->option_index);
  while ((ptr->nr != nr) || (ptr->index != index) || (screen_option == 0))
  {
    if ((ptr->index == 0) && screen_option)
      nr_actief++;
    if ((unsigned long)ptr == (unsigned long)screen_ptr->last_action)
      return (0);
    ptr++;
    screen_option = *(unsigned char *)Get_Ptr((void *)ptr->option, *ptr->option_index);
  }
  if (ptr->index)
    nr_actief--;

  screen_ptr->rel[0].key_action = ptr;
  ptr++;
  cnt++;
//  while ((cnt < screen_ptr->rel_max) && ((unsigned long)ptr <= (unsigned long)screen_ptr->last_action)) // TD
  while ((cnt < screen_ptr->rel_max) && (cnt < screen_ptr->nr_aantal) && ((unsigned long)ptr <= (unsigned long)screen_ptr->last_action))
  {
    screen_option = *(unsigned char *)Get_Ptr((void *)ptr->option, *ptr->option_index);
    if ((ptr->index == 0) && (screen_option))
    {
      screen_ptr->rel[cnt].key_action = ptr;
      cnt++;
    }
    ptr++;
  }
  while (cnt < 3)
  {
    screen_ptr->rel[cnt].key_action = 0;
    cnt++;
  }

  screen_ptr->nr_actief = nr_actief;
  screen_ptr->rel_actief = 0;
  return (1);
}
*/
char Search_Func_Index(unsigned int nr, unsigned char index)
{
s_key_action const *ptr;
unsigned char screen_option;
int cnt = 0;
int nr_actief = 0;

  ptr = screen_ptr->first_action;
  screen_option = *(unsigned char *)Get_Ptr((void *)ptr->option, *ptr->option_index);
  while ((ptr->nr != nr) || (ptr->index != index) || (screen_option == 0))
  {
    if ((ptr->index == 0) && screen_option)
      nr_actief++;
    if ((unsigned long)ptr == (unsigned long)screen_ptr->last_action)
      return (0);
    ptr++;
    screen_option = *(unsigned char *)Get_Ptr((void *)ptr->option, *ptr->option_index);
  }
  if (ptr->index)
    nr_actief--;

  if ((screen_ptr->rel_max - screen_ptr->rel_actief - 1) > (screen_ptr->nr_aantal - nr_actief - 1))
  {
    screen_ptr->rel_actief = screen_ptr->rel_max - (screen_ptr->nr_aantal - nr_actief);
  }
  if (nr_actief < screen_ptr->rel_actief)
  {
    screen_ptr->rel_actief = nr_actief;
  }

  screen_ptr->rel[screen_ptr->rel_actief].key_action = ptr;
  cnt = screen_ptr->rel_actief - 1;
  while ((cnt >= 0) && ((unsigned long)ptr >= (unsigned long)screen_ptr->first_action))
  {
    screen_option = *(unsigned char *)Get_Ptr((void *)ptr->option, *ptr->option_index);
    if ((ptr->index == 0) && screen_option && (ptr->nr != nr))
	{
	  screen_ptr->rel[cnt].key_action = ptr;
	  cnt--;
	}
	ptr--;
  }
  ptr = screen_ptr->rel[screen_ptr->rel_actief].key_action;
  ptr++;
  cnt = screen_ptr->rel_actief + 1;
  while ((cnt < screen_ptr->rel_max) && (cnt < screen_ptr->nr_aantal) && ((unsigned long)ptr <= (unsigned long)screen_ptr->last_action))
  {
    screen_option = *(unsigned char *)Get_Ptr((void *)ptr->option, *ptr->option_index);
    if ((ptr->index == 0) && screen_option)
	{
	  screen_ptr->rel[cnt].key_action = ptr;
	  cnt++;
	}
	ptr++;
  }
  while (cnt < 3)
  {
    screen_ptr->rel[cnt].key_action = 0;
    cnt++;
  }
  screen_ptr->nr_actief = nr_actief;
  return (1);
}

char Search_Func(unsigned int nr)
{
  return (Search_Func_Index(nr,0));
}

// TD - 13-09-2007: "screen_option" naar boven gezet.
//                  "Get_Ptr" werd één keer teveel aangeroepen.
void Search_Func_Or_First_True(unsigned int nr)
{
s_key_action const *ptr;
unsigned char screen_option;
int cnt = 0;
int nr_actief = 0;

  ptr = screen_ptr->first_action;
  screen_option = *(unsigned char *)Get_Ptr((void *)ptr->option, *ptr->option_index);
  while ((ptr->nr < nr) || (ptr->index != 0) || (screen_option == 0))
  {
    if ((ptr->index == 0) && screen_option)
      nr_actief++;
    if ((unsigned long)ptr == (unsigned long)screen_ptr->last_action)
      return;
    ptr++;
    screen_option = *(unsigned char *)Get_Ptr((void *)ptr->option, *ptr->option_index);
  }
  if (ptr->index)
    nr_actief--;
  screen_ptr->rel[0].key_action = ptr;
  ptr++;
  cnt++;
//  while ((cnt < screen_ptr->rel_max) && ((unsigned long)ptr <= (unsigned long)screen_ptr->last_action)) // TD
  while ((cnt < screen_ptr->rel_max) && (cnt < screen_ptr->nr_aantal) && ((unsigned long)ptr <= (unsigned long)screen_ptr->last_action))
  {
    screen_option = *(unsigned char *)Get_Ptr((void *)ptr->option, *ptr->option_index);
    if ((ptr->index == 0) && (screen_option))
    {
      screen_ptr->rel[cnt].key_action = ptr;
      cnt++;
    }
    ptr++;
  }
  while (cnt < 3)
  {
    screen_ptr->rel[cnt].key_action = 0;
    cnt++;
  }
  screen_ptr->nr_actief = nr_actief;
  screen_ptr->rel_actief = 0;
  Get_Value();
}

char Search_Func_Dont_Correct_Nr_Actief(unsigned int nr)
{
int nr_actief = screen_ptr->nr_actief;
int rel_actief = screen_ptr->rel_actief;
char help;

  help = Search_Func(nr);
  screen_ptr->nr_actief = nr_actief;
  screen_ptr->rel_actief = rel_actief;
  return (help);
}                               

//*****************************************************************************
void Next_Screen(s_screen *new_screen)
{
  new_screen->prev_screen = screen_ptr;
  screen_ptr = new_screen;
  Get_Value();
}

void First_Screen(void)
{
  while (screen_ptr->prev_screen)
    screen_ptr = screen_ptr->prev_screen;
  Get_Value();
}

void Prev_Screen(void)
{
  if (screen_ptr->prev_screen)
  {
    screen_ptr = screen_ptr->prev_screen;
    Get_Value();
  }
}

//*****************************************************************************
void Arrow_Hoofd_Func(void) // standaard functie voor hoofdschermen (bv klim_0_screen)
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
      break;
  }
}

//*****************************************************************************
void Arrow_Func(void) // standaard functie voor sub schermen (bv temp_1_screen)
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
      Prev_Screen();
      break;
    case RIGHT:
      Increment_Func_Index();
      break;
  }
}

void Arrow_Change_Func(void) // standaard functie voor sub schermen (bv temp_1_screen)
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
      Prev_Screen();
      break;
    case RIGHT:
      if ((password_enabled & PASSWORD_ENABLED_SETP_MASK) == 0)
        Right_Password();
      else
        Increment_Func_Index();
      break;
  }
}

void Arrow_Change_Syst_Func(void) // standaard functie voor sub schermen (bv temp_1_screen)
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
      Prev_Screen();
      break;
    case RIGHT:
      if ((password_enabled & PASSWORD_ENABLED_SYST_MASK) == 0)
        Right_Setpoint_Syst_Password();
      else
        Increment_Func_Index();
      break;
  }
}

void Arrow_Option_Func(void)
// zorgt er voor dat als slot aangebracht er niet naar rechts gesprongen wordt
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
      if ((password_enabled & PASSWORD_ENABLED_OPT_MASK) == 0)
        Prev_Screen();
      break;
    case RIGHT:
      if (password_enabled & PASSWORD_ENABLED_OPT_MASK)
        Increment_Func_Index();
      break;
  }
}
