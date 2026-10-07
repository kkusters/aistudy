// C__ALG.C

#include "ch_define.h"

#include "ch_cabriokas.h"
#include "ch_kiersturing.h"
#include "ch_motor.h"
#include "ch_motorgroep.h"
#include "ch_device.h"
#include "ch_alg.h"

//------------------------------------------------------------------------------
unsigned char * Copy_N_Bytes(unsigned char *destination, unsigned char *source, unsigned int nr)
{
unsigned char *start = destination;

  while (nr--)
  {
    *destination++ = *source++;
  }
  return (start);
}
//-----------------------------------------------------------------------------
unsigned char Bcd_Byte_To_Dec(unsigned char bcd)
// converts a byte BCD value to a byte DEC value
{
unsigned char help;

  help = (bcd >> 4) * 10;
  bcd &= 0x0f;
  return (help + bcd);  
}
//-----------------------------------------------------------------------------
unsigned char Dec_Byte_To_Bcd(unsigned char dec)
{
// converts abyte DEC value to a byte BCD value
// value with a value of 100 or more cannot be converted
unsigned char help;

  help = (dec / 10) << 4;
  dec %= 10;
  return (help + dec);
}
//-----------------------------------------------------------------------------
unsigned char Asc_To_Hex(unsigned char ch)
{
  if (ch >= '0')
  {
    if (ch <= '9')
	  return (ch - '0');
	else if (ch >= 'A')
	{
	  if (ch <= 'F')
	    return (ch - 'A' + 10);
	}
  }
  return (0);
}

unsigned char Hex_To_Asc(unsigned value)
{
char const hex_table[16] = { '0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

  return (hex_table[value & 0x0F]);
}
//-----------------------------------------------------------------------------
int Avg(long value, int number)
{
  if (number != 0)
  {
    if (value >= 0)
      return ((value + (number / 2))/ number);
    else
      return ((value - (number / 2))/ number);
  } 
  else 
  {
    return 0; // Delen door 0
  }
}

//-----------------------------------------------------------------------------
long Avg_long(long value, int number)
{
  if (number != 0)
  {
    if (value >= 0)
      return ((value + (number / 2))/ number);
    else
      return ((value - (number / 2))/ number);
  } 
  else 
  {
    return 0; // Delen door 0
  }
}

//-----------------------------------------------------------------------------
//  y_max  -------                                   -------
//                \                                 /
//                 \                               /
//                  \              OF             /
//                   \                           /
//                    \                         /
//  y_min              ---                   ---
//
//           x_max  x_min                   x_min  x_max

int Calc_Prop(int x_min ,int x_max ,int y_min ,int y_max ,int x_meet)
{
  if (y_min == y_max)
    return (y_min);
  else if (x_min == x_max)
  {
    if (x_meet < x_min)
      return (y_min);
    else
      return (y_max);  
  }
  else
  {
    if (x_min < x_max)
    {
      if (x_meet <= x_min)
        return (y_min);
      else if (x_meet >= x_max)
        return (y_max);
    }
    else
    {
      if (x_meet >= x_min)
        return (y_min);
      else if (x_meet <= x_max)
        return (y_max);
    }
	if (y_min < y_max)
      return (y_min + (((long)(x_meet - x_min) * (y_max - y_min)) + ((x_max - x_min) / 2)) / (x_max - x_min));
	else
      return (y_min + (((long)(x_meet - x_min) * (y_max - y_min)) - ((x_max - x_min) / 2)) / (x_max - x_min));
  }
}

long Calc_Prop_Long(long x_min ,long x_max ,long y_min ,long y_max ,long x_meet)
{
float val;

  if (y_min == y_max)
    return (y_min);
  else if (x_min == x_max)
  {
    if (x_meet < x_min)
      return (y_min);
    else
      return (y_max);  
  }
  else
  {
    if (x_min < x_max)
    {
      if (x_meet <= x_min)
        return (y_min);
      else if (x_meet >= x_max)
        return (y_max);
    }
    else
    {
      if (x_meet >= x_min)
        return (y_min);
      else if (x_meet <= x_max)
        return (y_max);
    }
	val = (float)y_min + ((((float)x_meet - x_min) * ((float)y_max - y_min))  / ((float)x_max - x_min));
	return ((val < 0) ? val - 0.5 : val + 0.5);  
  }
}

//-----------------------------------------------------------------------------
// Het zelfde als Calc_Prop maar dan zonder min. en max. grenzen

int Calc_Prop_NoLimit(int x_min ,int x_max ,int y_min ,int y_max ,int x_meet)
{
  if (y_min == y_max)
    return (y_min);
  if (x_min == x_max)
  {
    if (x_meet < x_min)
      return (y_min);
    else
      return (y_max);
  }
  if (y_min < y_max)
    return (y_min + (((long)(x_meet - x_min) * (y_max - y_min)) + ((x_max - x_min) / 2)) / (x_max - x_min));
  else
    return (y_min + (((long)(x_meet - x_min) * (y_max - y_min)) - ((x_max - x_min) / 2)) / (x_max - x_min));
}

//-----------------------------------------------------------------------------
// Bandbreedte regeling met cylustijd
void Calc_Integrated_Pos(int act, int streef, int *pos, int min_pos, int max_pos, unsigned char bandbreedte, unsigned char max_stap, unsigned char hysterese, unsigned char cyclus_tijd, unsigned char *timer)
{
  if (*timer >= cyclus_tijd)
  {
    if (act <= (streef - hysterese))
      *pos += Calc_Prop(streef - hysterese, streef - hysterese - bandbreedte, 1, max_stap, act);
    else if (act >= (streef + hysterese))
      *pos -= Calc_Prop(streef + hysterese, streef + hysterese + bandbreedte, 1, max_stap, act);
    *timer = 0;
  }
  else 
  {
    (*timer)++;
    if ((act > streef - hysterese) && (act < streef + hysterese))
      *timer = 0;
  }

  if (*pos < min_pos)
    *pos = min_pos;
  else if (*pos > max_pos)
    *pos = max_pos;
}

void Calc_Integrated_Neg(int act, int streef, int *pos, int min_pos, int max_pos, unsigned char bandbreedte, unsigned char max_stap, unsigned char hysterese, unsigned char cyclus_tijd, unsigned char *timer)
{
  if (*timer >= cyclus_tijd)
  {
    if (act <= (streef - hysterese))
      *pos -= Calc_Prop(streef - hysterese, streef - hysterese - bandbreedte, 1, max_stap, act);
    else if (act >= (streef + hysterese))
      *pos += Calc_Prop(streef + hysterese, streef + hysterese + bandbreedte, 1, max_stap, act);
    *timer = 0;
  }
  else 
  {
    (*timer)++;
    if ((act > streef - hysterese) && (act < streef + hysterese))
      *timer = 0;
  }

  if (*pos < min_pos)
    *pos = min_pos;
  else if (*pos > max_pos)
    *pos = max_pos;
}

//-----------------------------------------------------------------------------
long Calc_Perc(long deel, long totaal, long perc)
// perc = 100 voor 100%
// perc = 1000 voor 100.0%
// perc = 10000 voor 100.00%
// deel is waarde waarvan percentage berekend moet worden
{
  if (totaal)
    return ((((float)deel * perc) + (totaal / 2)) / totaal);
  else
    return (0);
}

long Calc_Perc_Abs(long deel, long totaal, long perc)
// perc = 100 voor 100%
// perc = 1000 voor 100.0%
// perc = 10000 voor 100.00%
// deel is waarde waarvan percentage berekend moet worden
// waarde wordt altijd naar beneden afgerond
{
  if (totaal)
	return (((float)deel * perc) / totaal);
  else
    return (0);
}

//-----------------------------------------------------------------------------
// return long value uit een void ptr
long Return_Value(e_type type, void *ptr)
{
  switch (type)
  {
    default:
    case CHAR:      return (*(char *)ptr);
    case UCHAR:     return (*(unsigned char *)ptr);
    case INT:	    return (*(int *)ptr);
    case UINT:      return (*(unsigned int *)ptr);
    case LONG:      return (*(long *)ptr);
	case TIME_CHAR: return (*(char *)ptr);
    case TIME_INT:  return (*(int *)ptr);
    case HEX_CHAR:  return (*(unsigned char *)ptr);
    case HEX_INT:   return (*(unsigned int *)ptr);
    case HEX_LONG:  return (*(unsigned long *)ptr);
  }
}

//-----------------------------------------------------------------------------
//returns absolute waarde van int
int abs_int( int arg )
{
	return( arg<0 ? -arg : arg );
}

//------------------------------------------------------------------------------
long abs_long(long arg)
{
  return (arg < 0 ? -arg : arg);
}

//-----------------------------------------------------------------------------
//returns sin(hoek in graden), waarde <0..100>, hoek <0..360>
int Sin_Angle_Degrees(int angle) //angle waarde <0..360> //Ramon
{
  static int table[91] = {0,2,3,5,7,9,10,12,14,16,17,19,21,22,24,26,28,29,31,33,34,36,37,39,41,42,44,45,47,48,50,52,53,54,56,57,59,60,62,
						  63,64,66,67,68,69,71,72,73,74,75,77,78,79,80,81,82,83,84,85,86,87,87,88,89,90,91,91,92,93,93,94,95,95,96,96,97,
						  97,97,98,98,98,99,99,99,99,100,100,100,100,100,100}; //100*sin(hoek in graden) (afgerond heel getal) voor hoeken 0..90

  if (angle < 0) return(0); //angle < 0, ongeldige invoer
  if (angle <= 90)  return(table[angle]);		 //angle <0..90>
  if (angle <= 180) return(table[180 - angle]);  //angle <91..180>
  if (angle <= 270) return(-table[angle - 180]); //angle <181..270>
  if (angle <= 360) return(-table[360 - angle]); //angle <271..360>
  return(0); //angle > 360, ongeldige invoer
}

//*****************************************************************************
void *Get_Ptr(void *val, unsigned char nr)
{
  if (nr < MAX_GROUP)
  {
    if (((unsigned long)val >= (unsigned long)&val_hr_alg.Motorgroup[0]) && ((unsigned long)val < (unsigned long)&val_hr_alg.Motorgroup[1]))
	  return ((void *)((unsigned long)val + (nr * ((unsigned long)&val_hr_alg.Motorgroup[1] - (unsigned long)&val_hr_alg.Motorgroup[0]))));
    else if (((unsigned long)val >= (unsigned long)&setp_alg.Motorgroup[0]) && ((unsigned long)val < (unsigned long)&setp_alg.Motorgroup[1]))
	  return ((void *)((unsigned long)val + (nr * ((unsigned long)&setp_alg.Motorgroup[1] - (unsigned long)&setp_alg.Motorgroup[0]))));
    else if (((unsigned long)val >= (unsigned long)&opt_app.Motorgroup[0]) && ((unsigned long)val < (unsigned long)&opt_app.Motorgroup[1]))
	  return ((void *)((unsigned long)val + (nr * ((unsigned long)&opt_app.Motorgroup[1] - (unsigned long)&opt_app.Motorgroup[0]))));
    else if (((unsigned long)val >= (unsigned long)&Motorgroup[0]) && ((unsigned long)val < (unsigned long)&Motorgroup[1]))
	  return ((void *)((unsigned long)val + (nr * ((unsigned long)&Motorgroup[1] - (unsigned long)&Motorgroup[0]))));
    else if (((unsigned long)val >= (unsigned long)&alarm_hr_alg.Motorgroup[0]) && ((unsigned long)val < (unsigned long)&alarm_hr_alg.Motorgroup[1]))
	  return ((void *)((unsigned long)val + (nr * ((unsigned long)&alarm_hr_alg.Motorgroup[1] - (unsigned long)&alarm_hr_alg.Motorgroup[0]))));
    else if (((unsigned long)val >= (unsigned long)&opt_app.Ventgroup[0]) && ((unsigned long)val < (unsigned long)&opt_app.Ventgroup[1]))
	  return ((void *)((unsigned long)val + (nr * ((unsigned long)&opt_app.Ventgroup[1] - (unsigned long)&opt_app.Ventgroup[0]))));
    else if (((unsigned long)val >= (unsigned long)&setp_alg.Ventgroup[0]) && ((unsigned long)val < (unsigned long)&setp_alg.Ventgroup[1]))
	  return ((void *)((unsigned long)val + (nr * ((unsigned long)&setp_alg.Ventgroup[1] - (unsigned long)&setp_alg.Ventgroup[0]))));
  }
  if (nr < MAX_MOTOR)
  {
    if (((unsigned long)val >= (unsigned long)&val_hr_alg.Motor[0]) && ((unsigned long)val < (unsigned long)&val_hr_alg.Motor[1]))
	  return ((void *)((unsigned long)val + (nr * ((unsigned long)&val_hr_alg.Motor[1] - (unsigned long)&val_hr_alg.Motor[0]))));
    else if (((unsigned long)val >= (unsigned long)&opt_app.Motor[0]) && ((unsigned long)val < (unsigned long)&opt_app.Motor[1]))
	  return ((void *)((unsigned long)val + (nr * ((unsigned long)&opt_app.Motor[1] - (unsigned long)&opt_app.Motor[0]))));
    else if (((unsigned long)val >= (unsigned long)&Motor[0]) && ((unsigned long)val < (unsigned long)&Motor[1]))
	  return ((void *)((unsigned long)val + (nr * ((unsigned long)&Motor[1] - (unsigned long)&Motor[0]))));
    else if (((unsigned long)val >= (unsigned long)&alarm_hr_alg.Motor[0]) && ((unsigned long)val < (unsigned long)&alarm_hr_alg.Motor[1]))
	  return ((void *)((unsigned long)val + (nr * ((unsigned long)&alarm_hr_alg.Motor[1] - (unsigned long)&alarm_hr_alg.Motor[0]))));
  }
  if (nr < MAX_SCREEN)
  {
    if (((unsigned long)val >= (unsigned long)&val_hr_alg.DualScreen[0]) && ((unsigned long)val < (unsigned long)&val_hr_alg.DualScreen[1]))
	  return ((void *)((unsigned long)val + (nr * ((unsigned long)&val_hr_alg.DualScreen[1] - (unsigned long)&val_hr_alg.DualScreen[0]))));
    else if (((unsigned long)val >= (unsigned long)&setp_alg.DualScreen[0]) && ((unsigned long)val < (unsigned long)&setp_alg.DualScreen[1]))
	  return ((void *)((unsigned long)val + (nr * ((unsigned long)&setp_alg.DualScreen[1] - (unsigned long)&setp_alg.DualScreen[0]))));
    else if (((unsigned long)val >= (unsigned long)&opt_app.DualScreen[0]) && ((unsigned long)val < (unsigned long)&opt_app.DualScreen[1]))
	  return ((void *)((unsigned long)val + (nr * ((unsigned long)&opt_app.DualScreen[1] - (unsigned long)&opt_app.DualScreen[0]))));
    else if (((unsigned long)val >= (unsigned long)&DualScreen[0]) && ((unsigned long)val < (unsigned long)&DualScreen[1]))
	  return ((void *)((unsigned long)val + (nr * ((unsigned long)&DualScreen[1] - (unsigned long)&DualScreen[0]))));
  }
  if (nr < MAX_DEVICE)
  {
    if (((unsigned long)val >= (unsigned long)&val_hr_alg.Device[0]) && ((unsigned long)val < (unsigned long)&val_hr_alg.Device[1]))
	  return ((void *)((unsigned long)val + (nr * ((unsigned long)&val_hr_alg.Device[1] - (unsigned long)&val_hr_alg.Device[0]))));
    else if (((unsigned long)val >= (unsigned long)&setp_alg.Device[0]) && ((unsigned long)val < (unsigned long)&setp_alg.Device[1]))
	  return ((void *)((unsigned long)val + (nr * ((unsigned long)&setp_alg.Device[1] - (unsigned long)&setp_alg.Device[0]))));
    else if (((unsigned long)val >= (unsigned long)&opt_app.Device[0]) && ((unsigned long)val < (unsigned long)&opt_app.Device[1]))
	  return ((void *)((unsigned long)val + (nr * ((unsigned long)&opt_app.Device[1] - (unsigned long)&opt_app.Device[0]))));
    else if (((unsigned long)val >= (unsigned long)&Device[0]) && ((unsigned long)val < (unsigned long)&Device[1]))
	  return ((void *)((unsigned long)val + (nr * ((unsigned long)&Device[1] - (unsigned long)&Device[0]))));
    else if (((unsigned long)val >= (unsigned long)&alarm_hr_alg.Device[0]) && ((unsigned long)val < (unsigned long)&alarm_hr_alg.Device[1]))
	  return ((void *)((unsigned long)val + (nr * ((unsigned long)&alarm_hr_alg.Device[1] - (unsigned long)&alarm_hr_alg.Device[0]))));
  }
  if (nr < MAX_MB_DEVICE)
  {
    if (((unsigned long)val >= (unsigned long)&alarm_hr_alg.mbDevice[0]) && ((unsigned long)val < (unsigned long)&alarm_hr_alg.mbDevice[1]))
	  return ((void *)((unsigned long)val + (nr * ((unsigned long)&alarm_hr_alg.mbDevice[1] - (unsigned long)&alarm_hr_alg.mbDevice[0]))));
  }
  if (nr < MAX_CABRIO)
  {
    if (((unsigned long)val >= (unsigned long)&opt_app.Cabriokas[0]) && ((unsigned long)val < (unsigned long)&opt_app.Cabriokas[1]))
	  return ((void *)((unsigned long)val + (nr * ((unsigned long)&opt_app.Cabriokas[1] - (unsigned long)&opt_app.Cabriokas[0]))));
    else if (((unsigned long)val >= (unsigned long)&Cabriokas[0]) && ((unsigned long)val < (unsigned long)&Cabriokas[1]))
	  return ((void *)((unsigned long)val + (nr * ((unsigned long)&Cabriokas[1] - (unsigned long)&Cabriokas[0]))));
  }
  if (nr < MAX_LUCHTMENGKAST_GROEP)
  {
    if (((unsigned long)val >= (unsigned long)&val_hr_alg.LuchtmengkastGroep[0]) && ((unsigned long)val < (unsigned long)&val_hr_alg.LuchtmengkastGroep[1]))
	  return ((void *)((unsigned long)val + (nr * ((unsigned long)&val_hr_alg.LuchtmengkastGroep[1] - (unsigned long)&val_hr_alg.LuchtmengkastGroep[0]))));
    else if (((unsigned long)val >= (unsigned long)&setp_alg.LuchtmengkastGroep[0]) && ((unsigned long)val < (unsigned long)&setp_alg.LuchtmengkastGroep[1]))
	  return ((void *)((unsigned long)val + (nr * ((unsigned long)&setp_alg.LuchtmengkastGroep[1] - (unsigned long)&setp_alg.LuchtmengkastGroep[0]))));
    else if (((unsigned long)val >= (unsigned long)&opt_app.LuchtmengkastGroep[0]) && ((unsigned long)val < (unsigned long)&opt_app.LuchtmengkastGroep[1]))
	  return ((void *)((unsigned long)val + (nr * ((unsigned long)&opt_app.LuchtmengkastGroep[1] - (unsigned long)&opt_app.LuchtmengkastGroep[0]))));
    else if (((unsigned long)val >= (unsigned long)&alarm_hr_alg.LuchtmengkastGroep[0]) && ((unsigned long)val < (unsigned long)&alarm_hr_alg.LuchtmengkastGroep[1]))
	  return ((void *)((unsigned long)val + (nr * ((unsigned long)&alarm_hr_alg.LuchtmengkastGroep[1] - (unsigned long)&alarm_hr_alg.LuchtmengkastGroep[0]))));
  }
  if (nr < MAX_LUCHTMENGKAST)
  {
    if (((unsigned long)val >= (unsigned long)&val_hr_alg.Luchtmengkast[0]) && ((unsigned long)val < (unsigned long)&val_hr_alg.Luchtmengkast[1]))
	  return ((void *)((unsigned long)val + (nr * ((unsigned long)&val_hr_alg.Luchtmengkast[1] - (unsigned long)&val_hr_alg.Luchtmengkast[0]))));
//    else if (((unsigned long)val >= (unsigned long)&setp_alg.Luchtmengkast[0]) && ((unsigned long)val < (unsigned long)&setp_alg.Luchtmengkast[1]))
//	  return ((void *)((unsigned long)val + (nr * ((unsigned long)&setp_alg.Luchtmengkast[1] - (unsigned long)&setp_alg.Luchtmengkast[0]))));
    else if (((unsigned long)val >= (unsigned long)&opt_app.Luchtmengkast[0]) && ((unsigned long)val < (unsigned long)&opt_app.Luchtmengkast[1]))
	  return ((void *)((unsigned long)val + (nr * ((unsigned long)&opt_app.Luchtmengkast[1] - (unsigned long)&opt_app.Luchtmengkast[0]))));
    else if (((unsigned long)val >= (unsigned long)&alarm_hr_alg.Luchtmengkast[0]) && ((unsigned long)val < (unsigned long)&alarm_hr_alg.Luchtmengkast[1]))
	  return ((void *)((unsigned long)val + (nr * ((unsigned long)&alarm_hr_alg.Luchtmengkast[1] - (unsigned long)&alarm_hr_alg.Luchtmengkast[0]))));
  }
  return (val);
}