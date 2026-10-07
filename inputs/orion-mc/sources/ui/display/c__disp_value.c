// C__DISP_VALUE.C

#include "ch_define.h"

#include "ch_alg.h"
#include "ch_disp.h"
#include "ch_disp_func.h"
#include "ch_disp_option_1.h"
#include "ch_disp_value.h" 
#include "ch_key.h"
#include "ch_main.h"
#include "ch_disp_value.h"

void Get_Value(void)
{
s_key_action const *key_action_ptr = screen_ptr->rel[screen_ptr->rel_actief].key_action;
s_key_value const *key_value_ptr = key_action_ptr->value;
e_type type = key_value_ptr->type;
                   
//  if (screen_ptr->rel_max > 0) // TD
  if ((screen_ptr->nr_aantal > 0) && (screen_ptr->rel_max > 0))
  {
    screen_ptr->nr = key_action_ptr->nr;
    screen_ptr->index = key_action_ptr->index;
    screen_ptr->value = Return_Value(type, Get_Ptr(key_value_ptr->value, *key_action_ptr->option_index));
    screen_ptr->min_value = Return_Value(type, Get_Ptr(key_value_ptr->min_value, *key_action_ptr->option_index));
    screen_ptr->max_value = Return_Value(type, Get_Ptr(key_value_ptr->max_value, *key_action_ptr->option_index));
    screen_ptr->change_flag = 0;
  }
}

void Put_Value(void)
{
s_key_action const *key_action_ptr = screen_ptr->rel[screen_ptr->rel_actief].key_action;
void *ptr = Get_Ptr(key_action_ptr->value->value, *key_action_ptr->option_index);

  switch (key_action_ptr->value->type)
  {
    case CHAR:  *(char *)ptr = screen_ptr->value; break;
    case UCHAR: *(unsigned char *)ptr = screen_ptr->value; break;
    case INT:   *(int *)ptr = screen_ptr->value; break;
    case UINT:  *(unsigned int *)ptr = screen_ptr->value; break;
    case LONG:  *(long *)ptr = screen_ptr->value; break;
  }
  screen_ptr->change_flag = 0;
}

//*****************************************************************************

void Number_Value(void)
{
unsigned char help;
static unsigned char old_point;

  if (screen_ptr->point == 0)
    old_point = 0;
  if (key >= '0' && key <= '9')
  {
    help = key - '0';
    if (screen_ptr->norm_point)
    {
      if (!screen_ptr->change_flag)
      {
        screen_ptr->value = help;
        screen_ptr->change_flag = 1;
        screen_ptr->point = 0;
      }
      else
      {
        if (((screen_ptr->point != 0) && (old_point || (screen_ptr->nr_digits < screen_ptr->rel[screen_ptr->rel_actief].key_action->value->max_digits))) ||
            ((screen_ptr->point == 0) && ((screen_ptr->value == 0) || (screen_ptr->nr_digits + screen_ptr->norm_point < screen_ptr->rel[screen_ptr->rel_actief].key_action->value->max_digits))))
        {
          if (old_point == 1)
          {
            screen_ptr->value += (screen_ptr->value < 0) ? -help : help;
            old_point = 0;
          }
          else
          {
            if (screen_ptr->point < screen_ptr->norm_point)
            {
              screen_ptr->value *= 10;
              screen_ptr->value += (screen_ptr->value < 0) ? -help : help;
              if (screen_ptr->point)
                screen_ptr->point++;
            }
          }
        }
      }
    }
    else
    {
      if (!screen_ptr->change_flag)
      {
        screen_ptr->value = help;
        screen_ptr->change_flag = 1;
      }
      else
      {
        if (screen_ptr->nr_digits < screen_ptr->rel[screen_ptr->rel_actief].key_action->value->max_digits)
        {
          screen_ptr->value *= 10;
          screen_ptr->value += (screen_ptr->value < 0) ? -help : help;
        }
        else if ((screen_ptr->rel[screen_ptr->rel_actief].key_action->value->max_digits) &&
                 (screen_ptr->value == 0))
        {
          screen_ptr->value = help;
        }
      }
    }
  }  
  else if (key == PLUS_MIN)
  {
    if ((screen_ptr->min_value < 0) &&
        (screen_ptr->change_flag) &&
        ((screen_ptr->value < (long)0) ||
         (screen_ptr->nr_digits < screen_ptr->rel[screen_ptr->rel_actief].key_action->value->max_digits)))
      screen_ptr->value = -screen_ptr->value;
  }
  else if (key == '.')
  {
// nieuw zodat ook met . begonnen kan worden 29-08-2003
    if (screen_ptr->norm_point)
    {
      if (!screen_ptr->change_flag)
      {
        screen_ptr->change_flag = 1;
        screen_ptr->value = 0;
        screen_ptr->point = 1;
        old_point = 1;
      }
      else if (!screen_ptr->point)
      {
        screen_ptr->value *= 10;
        screen_ptr->point = 1;
        old_point = 1;
      }
    }
/* oud kan niet met punt begonnen worden
    if ((screen_ptr->change_flag) && 
        (screen_ptr->norm_point) &&
        (!screen_ptr->point))
    {
      screen_ptr->value *= 10;
      screen_ptr->point = 1;
      old_point = 1;
    }
*/
  }
}

void Correct_Decimal_Value(void)
{
//unsigned char help;

  if (screen_ptr->point > screen_ptr->norm_point)
  {
    while (screen_ptr->point > screen_ptr->norm_point)
    {
      screen_ptr->value /= 10;
      screen_ptr->point--;
    }
  }
  else if (screen_ptr->point < screen_ptr->norm_point)
  {
    while (screen_ptr->point < screen_ptr->norm_point)
    {
      screen_ptr->value *= 10;
      screen_ptr->point++;
    }
  }
}

void Increment_Value(void)
{
  Correct_Decimal_Value();
//  screen_ptr->value++;
  screen_ptr->value += key_inc;
  if (screen_ptr->value < screen_ptr->min_value)
    screen_ptr->value = screen_ptr->min_value;
  else if (screen_ptr->value > screen_ptr->max_value)
    screen_ptr->value = screen_ptr->max_value;
  screen_ptr->change_flag = 1;
  Enter_Value();
}

void Increment_Value_No_Enter(void)
{
  Correct_Decimal_Value();
//  screen_ptr->value++;
  screen_ptr->value += key_inc;
  if (screen_ptr->value < screen_ptr->min_value)
    screen_ptr->value = screen_ptr->min_value;
  else if (screen_ptr->value > screen_ptr->max_value)
    screen_ptr->value = screen_ptr->max_value;
  screen_ptr->change_flag = 1;
}

void Increment_Option_Value(void)
{
  Increment_Value();
  option_change_flag = 1;
}

void Increment_Scroll_Value(void)
{
  Correct_Decimal_Value();
  screen_ptr->value++;
  if (screen_ptr->value > screen_ptr->max_value)
    screen_ptr->value = screen_ptr->min_value;
  screen_ptr->change_flag = 1;
  Enter_Value();
}

void Increment_Scroll_Value_No_Enter(void)
{
  Correct_Decimal_Value();
  screen_ptr->value++;
  if (screen_ptr->value > screen_ptr->max_value)
    screen_ptr->value = screen_ptr->min_value;
  screen_ptr->change_flag = 1;
}

void Increment_Scroll_Option_Value(void)
{
  Increment_Scroll_Value();
  option_change_flag = 1;
}

void Increment_Scroll_Option_Value_No_Enter(void)
{
  Increment_Scroll_Value_No_Enter();
  option_change_flag = 1;
}

void Decrement_Value(void)
{
  Correct_Decimal_Value();
//  screen_ptr->value--;
  screen_ptr->value -= key_inc;
  if (screen_ptr->value < screen_ptr->min_value)
    screen_ptr->value = screen_ptr->min_value;
  else if (screen_ptr->value > screen_ptr->max_value)
    screen_ptr->value = screen_ptr->max_value;
  screen_ptr->change_flag = 1;
  Enter_Value();
}

void Decrement_Value_No_Enter(void)
{
  Correct_Decimal_Value();
//  screen_ptr->value--;
  screen_ptr->value -= key_inc;
  if (screen_ptr->value < screen_ptr->min_value)
    screen_ptr->value = screen_ptr->min_value;
  else if (screen_ptr->value > screen_ptr->max_value)
    screen_ptr->value = screen_ptr->max_value;
  screen_ptr->change_flag = 1;
}

void Decrement_Option_Value(void)
{
  Decrement_Value();
  option_change_flag = 1;
}

void Decrement_Scroll_Value(void)
{
  Correct_Decimal_Value();
  screen_ptr->value--;
  if (screen_ptr->value < screen_ptr->min_value)
    screen_ptr->value = screen_ptr->max_value;
  screen_ptr->change_flag = 1;
  Enter_Value();
}

void Decrement_Scroll_Value_No_Enter(void)
{
  Correct_Decimal_Value();
  screen_ptr->value--;
  if (screen_ptr->value < screen_ptr->min_value)
    screen_ptr->value = screen_ptr->max_value;
  screen_ptr->change_flag = 1;
}

void Decrement_Scroll_Option_Value(void)
{
  Decrement_Scroll_Value();
  option_change_flag = 1;
}

void Decrement_Scroll_Option_Value_No_Enter(void)
{
  Decrement_Scroll_Value_No_Enter();
  option_change_flag = 1;
}

unsigned char Enter_Value(void)
{
  Correct_Decimal_Value();
  if ((screen_ptr->value >= screen_ptr->min_value) &&
      (screen_ptr->value <= screen_ptr->max_value))
  {
    Put_Value();
    return (1);
  }
  else
    return (0);
}

//*****************************************************************************
unsigned char Left_Value(void)
{
  if (screen_ptr->change_flag)
  {
    if (screen_ptr->point)
      screen_ptr->point--;
    screen_ptr->value /= 10;
    if((screen_ptr->value == 0) && (screen_ptr->point == 0))
      screen_ptr->change_flag = 0;
    return(0);
  }
  return(1);
}

void Arrow_Left_Value(void)
{
  if (Left_Value())
    Decrement_Func_Index();
}

void Arrow_Value(void)
{
  switch (key)
  {
    case LEFT:  Arrow_Left_Value(); break;
    case RIGHT: Increment_Func_Index(); break;
    case UP:    Increment_Value(); break;
    case DOWN:  Decrement_Value(); break;
  }
}

void Arrow_Option_Value(void)
{
  switch (key)
  {
    case LEFT:  Arrow_Left_Value(); break;
    case RIGHT: Increment_Func_Index(); break;
    case UP:    Increment_Option_Value(); break;
    case DOWN:  Decrement_Option_Value(); break;
  }
}

void Arrow_Scroll_Value(void)
{
  switch (key)
  {
    case LEFT:  Arrow_Left_Value(); break;
    case RIGHT: Increment_Func_Index(); break;
    case UP:    Increment_Scroll_Value(); break;
    case DOWN:  Decrement_Scroll_Value(); break;
  }
}

void Arrow_Scroll_Option_Value(void)
{
  switch (key)
  {
    case LEFT:  Arrow_Left_Value(); break;
    case RIGHT: Increment_Func_Index(); break;
    case UP:    Increment_Scroll_Option_Value(); break;
    case DOWN:  Decrement_Scroll_Option_Value(); break;
  }
}

void Arrow_Scroll_Option_Value_Refresh_ScreenNr(void)
{
  switch (key)
  {
    case LEFT:  Arrow_Left_Value(); break;
    case RIGHT: Increment_Func_Index(); break;
    case UP:    Increment_Scroll_Option_Value();
                Refresh_Screen_Nr_Aantal();
                break;
    case DOWN:  Decrement_Scroll_Option_Value();
                Refresh_Screen_Nr_Aantal();
                break;
  }
}

//*****************************************************************************

void Number_Code(void)
{
unsigned char help;

  if (key >= '0' && key <= '9')
  {
    help = key - '0';
    if (!screen_ptr->change_flag)
    {
      screen_ptr->value = help;
      screen_ptr->change_flag = 1;
    }
    else
    {
      if (screen_ptr->nr_digits < screen_ptr->rel[screen_ptr->rel_actief].key_action->value->max_digits)
      {
        screen_ptr->value *= 10;
        screen_ptr->value += (screen_ptr->value < 0) ? -help : help;
      }
      else if ((screen_ptr->rel[screen_ptr->rel_actief].key_action->value->max_digits) &&
               (screen_ptr->value == 0))
      {
        screen_ptr->value = help;
      }
    }
  }  
}
