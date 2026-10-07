// C__TIJD.C

#include "ch_define.h"

#include "ch_asc1.h"
#include "ch_can_backbone_appl.h"
#include "ch_rtc.h"
#include "ch_sd.h"
#include "ch_sd_log.h"
#include "ch_tijd.h"

bit flag_1s = 0;
bit flag_1min = 0;
bit flag_1hour = 0;			   
bit flag_1day = 0;			   
bit tijd_init_switch = 0;
bit uur_switch = 0;
bit dag_switch = 0;
bit einde_dag_switch = 0;
bit tijd_sync_switch = 0;

time_t clock_in_sec_last_startup = 0;
#pragma noclear
struct tm tijd;
time_t clock_in_sec;
#pragma clear
#pragma default_attributes

// function for us with standard C routines from time.h
// time.h CLOCKS_PER_SEC changed from 1000 to 1 
// library c:\c166\lib\ext
// source time.c new compiled and linked in c166l.lib
// compile:
// go to directory from time.c
// c:\c166\bin\c166 time.c -e -x -Ml -Ofl -RclPR=CLIBRARY
// c:\c166\bin\a166 time.src EX NOM166 NOCHECKCPU1R006 NOCHECKCPU16 NOCHECKSTBUS1 NOCHECKBUS18 DEBUG NOPA NOLOCALS NOPRINT
// go to directory lib and copy time.obj in thsi directory
// c:\c166\bin\ar166 crv c166l.lib time.obj

time_t _time(time_t *pt)
{
time_t help;

  _atomic(2);
  help = clock_in_sec_last_startup;
  if (pt)
    *pt = help;
  return (help);
}

void Tijd_Master_Sync(void)
// stuurt 1 maal per dag om 00:20:30 via CAN de tijd door ter synchronistatie alle computers
{
  if ((tijd_sync_switch || 
      ((tijd.tm_hour == 1) && (tijd.tm_min == 20))) &&
      (tijd.tm_sec == 30) &&
      (opt_alg.can_backbone) && (setp_alg.tijd_sync == 2))
  {
    // Zend tijd
//    Can_Backbone_Set_Clock_In_Sec_And_Send(&can_backbone_transmit_clock_in_sec, time(0));  JP 13-08-07
	tijd_sync_switch = 0;
  }
}

void Tijd_PC_Set(time_t new_time)
// tijd wordt via PC ingesteld
{
struct tm *tm_ptr;

  _stime(&new_time);
  clock_in_sec = time(0);
  tm_ptr = gmtime(&clock_in_sec);
  tijd = *tm_ptr;
  tijd.tm_mon++;
  tijd.tm_year += 1900;
  RTC_read_switch = 0;
  RTC_write_switch = 1;
}

bit Tijd_Check_Flag_1s(void)
{
//static time_t help;

  if (flag_1s)
  {
    flag_1s = 0;
    clock_in_sec = time(0);
    tijd = *gmtime(&clock_in_sec);
    tijd.tm_mon++;
    tijd.tm_year += 1900;
    Tijd_Master_Sync();
    if (tijd.tm_sec == 0)
    {
      flag_1min = 1;
      if (tijd.tm_min == 0)
      {
        flag_1hour = 1;
        if (tijd.tm_hour == 0)
          flag_1day = 1;
      }
    }  
    return (1);
  }
  else
    return (0);
}

bit Tijd_Check_Flag_1min(void)
{
  if (flag_1min)
  {
    flag_1min = 0;
    return (1);
  }
  else
    return (0);  
}

bit Tijd_Check_Flag_1hour(void)
{
  if (flag_1hour)
  {
    flag_1hour = 0;
    return (1);
  }
  else
    return (0);  
}

bit Tijd_Check_Flag_1day(void)
{
  if (flag_1day)
  {
    flag_1day = 0;
    return (1);
  }
  else
    return (0);  
}

//*****************************************************************************
void Einde_Dag_Control(void)
{
#ifdef SD_CARD
  File_Rename(LOG_FILE);
  File_Rename(MOTOR_FILE);

  SD_Log_Einde_Dag();
#endif // SD_CARD
  einde_dag_switch = 0;
}

void Uur_Control(void)
// wordt elk uur aangeroepen
{
  if (tijd.tm_hour == 0)
    einde_dag_switch = 1;
  uur_switch = 0;
}

void Dag_Control(void)
{
  if (setp_alg.dagenteller < 999)
  {
    setp_alg.dagenteller++;
  }
  dag_switch = 0;
}
