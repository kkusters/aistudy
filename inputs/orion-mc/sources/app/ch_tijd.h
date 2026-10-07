// CH_TIJD.H

#ifndef _CH_TIJD_H
#define _CH_TIJD_T

#include <time.h>

/* deze structur komt uit time.h
struct tm
{
  int   tm_sec;         // seconds after the minute - [0, 59]   
  int   tm_min;         // minutes after the hour - [0, 59]     
  int   tm_hour;        // hours since midnight - [0, 23]       
  int   tm_mday;        // day of the month - [1, 31]           
  int   tm_mon;         // months since January - [0, 11]       
  int   tm_year;        // year since 1900                      
  int   tm_wday;        // days since Sunday - [0, 6]           
  int   tm_yday;        // days since January 1 - [0, 365]      
  int   tm_isdst;       // Daylight Saving Time flag            
};
*/

extern bit tijd_init_switch;
extern bit uur_switch;
extern bit dag_switch;
extern bit einde_dag_switch;
extern bit tijd_sync_switch;
extern struct tm tijd;
extern time_t clock_in_sec_last_startup;
extern time_t clock_in_sec;

extern bit flag_1s;
//extern time_t sec_after_last_startup;

void Tijd_PC_Set(time_t new_time);
void Tijd_Master_Sync(void);
bit Tijd_Check_Flag_1s(void);
bit Tijd_Check_Flag_1min(void);
bit Tijd_Check_Flag_1hour(void);
bit Tijd_Check_Flag_1day(void);
void Einde_Dag_Control(void);
void Uur_Control(void);
void Dag_Control(void);

#endif
