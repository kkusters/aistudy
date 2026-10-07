// CH_TIMER.H

#ifndef _CH_TIMER_H
#define _CH_TIMER_H

#include "ch_define.h"

bit Timer_125ms_Check_Flag(void);
bit Timer_500ms_Check_Flag(void);

/********************************************************************************************************/
#define SYSTEM_TIMER_BASE 5 // SystemTimer in ms
#define TIMER_25MS  ((unsigned int)  25 / SYSTEM_TIMER_BASE)
#define TIMER_50MS  ((unsigned int)  50 / SYSTEM_TIMER_BASE)
#define TIMER_100MS ((unsigned int) 100 / SYSTEM_TIMER_BASE)
#define TIMER_200MS ((unsigned int) 200 / SYSTEM_TIMER_BASE)
#define TIMER_500MS ((unsigned int) 500 / SYSTEM_TIMER_BASE)
#define TIMER_1SEC  ((unsigned int)1000 / SYSTEM_TIMER_BASE)
#define TIMER_1MIN  ((unsigned int)60 * TIMER_1SEC)
#define TIMER_5MIN  ((unsigned int) 5 * TIMER_1MIN)

typedef struct
{
  unsigned int Starttime;
  unsigned int Interval;
} TTimer;

void TimerSet(TTimer *Timer, unsigned int Interval);
void TimerReset(TTimer *Timer);
void TimerRestart(TTimer *Timer);
int  TimerExpired(TTimer *Timer);

/********************************************************************************************************/
extern unsigned char timer_init_switch;
extern volatile unsigned int timer_1_milli_seconde;
extern volatile unsigned int timer_31_25_milli_seconde;
extern volatile unsigned int timer_1_seconde;

#define TIME_BASE_1_MSEC &timer_1_milli_seconde
#define TIME_BASE_31_25_MS &timer_31_25_milli_seconde
#define TIME_BASE_1_SEC &timer_1_seconde

typedef struct
{
  unsigned int start;
  unsigned int interval;
  volatile unsigned int *ptr_timer;
} s_timer;

void Timer_Set(s_timer *timer, unsigned int interval, volatile unsigned int *ptr_timer);
void Timer_Reset(s_timer *timer);
void Timer_Restart(s_timer *timer);
unsigned char Timer_Expired(s_timer *timer);
unsigned int Timer_Delay(s_timer *timer);
unsigned int Timer_Rest(s_timer *timer);

/********************************************************************************************************/
void Timer_Control(void);

#endif // _CH_TIMER_H
