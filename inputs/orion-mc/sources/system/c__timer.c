// C__TIMER.C

#include "ch_define.h"

#include "co_flag.h"

//#include "ch_can1.h"
//#include "ch_test.h"
#include "ch_rtc.h"
#include "ch_test.h"
#include "ch_tijd.h"
#include "ch_sd.h"
#include "ch_timer.h"

#define TIMER_ERROR_CNT_MAX 100
#define TIMER_WAIT_CNT_MAX 5000

#define SET_T78CON 0x0006
// set T7 for 1 ms
#define SET_T7 0xFD8F
// set T7 for 25 ms
// #define SET_T7 0xC2F7

// IIC_RTC_CLOCKOUT is the clockout line from the RTC
#define IIC_RTC_CLOCKOUT P9_4
#define IIC_RTC_CLOCKOUT_DP DP9_4
#define IIC_RTC_CLOCKOUT_ODP ODP9_4
#define IIC_RTC_CLOCKOUT_ALTSEL0 AS0P9_4
#define IIC_RTC_CLOCKOUT_ALTSEL1 AS1P9_4

#define IIC_RTC_MIN_MSEC ((unsigned int)40-4)
#define IIC_RTC_MAX_MSEC ((unsigned int)40+4)

unsigned char clock_error = 0; // no rtc pulses
unsigned char clock_error_sec = 0; // no rtc pulses
unsigned int clock_check_timer = 0; // timer to check if rtc is still working


bit flag_125ms = 0;
bit flag_500ms = 0;
unsigned char timer_init_switch = 1;
static unsigned int timer_error_cnt = 0;
static unsigned int timer_wait_cnt = TIMER_WAIT_CNT_MAX;
unsigned int SystemTimer; // Timer in 5ms
volatile unsigned char coTimerTicks; // CANopen timer ticks
unsigned int const coTimerPulse = CONFIG_TIMER_INC; // length of 1 tick

//-----------------------------------------------------------------------------

bit Timer_125ms_Check_Flag(void)
{
  if (flag_125ms)
  {
    flag_125ms = 0;
    return (1);
  }
  else
    return (0);
}

bit Timer_500ms_Check_Flag(void)
{
  if (flag_500ms)
  {
    flag_500ms = 0;
    return (1);
  }
  else
    return (0);
}

void Timer_Init(void)
{
  CC2_T7IC_IE = 0;
  CC2_T78CON = (CC2_T78CON & 0xFF00) | SET_T78CON; // timer 8 timer mode
  CC2_T7 = SET_T7;
  CC2_T7REL = SET_T7;
  CC2_T7IC = TIMER_CHECK_EVENT_LEVEL;
  CC2_T7IC_IE = 1;
  CC2_T78CON_T7R = 1;
}

void RTC_Clock_Out_Init(void)
// set port and interrupt for rtc form RTC_IIC
{
  IIC_RTC_CLOCKOUT_ALTSEL0 = 0;
  IIC_RTC_CLOCKOUT_ALTSEL1 = 0;
  IIC_RTC_CLOCKOUT = 1;
  IIC_RTC_CLOCKOUT_DP = INPUT;
  IIC_RTC_CLOCKOUT_ODP = OPEN_DRAIN;

  // set interrupt for input from rtc ic
  CC2_IOC =  0x0004; // load CAPCOM2 I/O control register
  // non staggered mode 
  // compare output signals effect the associated port output pin

  CC2_M5 = CC2_M5 & 0xFFF0 | 0x0001; // Generate interrupt on negative transition
  CC2_CC20IC = IIC_RTC_CLOCKOUT_INT_LEVEL;     
  CC2_CC20IC_IE = 1;
  // read RTC to see if settings are correct (if not then settings are corrected)
  RTC_read_switch = 1;
}

void Timer_Control(void) // vervangt Timer_1ms_Init en Timer_1ms_Check
{
  // check if control interrupt for rtc is working correct
  // if not the init this interrupt again
  if (timer_init_switch)
  {
    timer_init_switch = 0;
    Timer_Init(); 
    RTC_Clock_Out_Init();
  }
  else
  {
    if (timer_wait_cnt < TIMER_WAIT_CNT_MAX)
      timer_wait_cnt++;
    else
    {
      // error timer not working
      if (timer_error_cnt < TIMER_ERROR_CNT_MAX)
      {
        timer_error_cnt++;
        timer_wait_cnt = 0; 
        Timer_Init(); 
      }  
      else // normal intitialisation is not working restart compleet system by watchdog timeout
        while (1);
    }  
    if (clock_error_sec == 1) // init at first time error set
    {
      clock_error_sec++;
      RTC_Clock_Out_Init();
    }
    if (clock_error_sec >= 17) // try to init every 15 second
    {
      clock_error_sec = 2;
      RTC_Clock_Out_Init();
    }  
  }
}

interrupt TIMER_INT_ADR using(TIMER_INT_RB) void Timer_Interrupt(void)
{
static char cnt_31_25_ms_max = 30;
static char cnt_31_25_ms_correct = 0;
static char cnt_31_25_ms = 0;
static char cnt_5ms = 0;
static char cnt_10ms = 0;
static char cnt_25ms = 0;
static int cnt_125ms = 0;
static int cnt_500ms = 0;
static int cnt_1s = 0;

  #ifdef TIMING_TEST
  test_cnt++;
  #endif
  timer_1_milli_seconde++;
  if (cnt_5ms < 5-1)
    cnt_5ms++;
  else
  {
    cnt_5ms = 0;
    SystemTimer++; // Timebase = 5ms
    #ifdef SD_CARD
    if (cnt_10ms < 2-1)
      cnt_10ms++;
    else
    {
      cnt_10ms = 0;
//      disk_timerproc();
    }
    #endif // SD_CARD
    if (cnt_25ms < 5-1)
      cnt_25ms++;
    else
    {
      cnt_25ms = 0;

      // called every 25 ms to check if rtc clock is correct working
      #ifdef CANopen
      coTimerTicks++;
	  SET_COLIB_FLAG(COFLAG_TIMER_PULSED);
      #endif // CANopen
      timer_wait_cnt = 0;
      timer_error_cnt = 0;
      // if XC161CJ this interrupt only needed to check if interrupt from RTC is correct
      if (clock_check_timer < IIC_RTC_MAX_MSEC+10)
        clock_check_timer++;
      else
        clock_error = 1;  
      if (clock_error == 0) // rtc is correct reset value to simulate rtc
      {
        cnt_1s = 0;
        cnt_500ms = 0;
        cnt_125ms = 0;
        clock_error_sec = 0;
      }
      else // simulate rtc
      {
        if (cnt_125ms < 4)
          cnt_125ms++;
        else
        {
          cnt_125ms = 0;
          flag_125ms = 1;
          if (cnt_500ms < 3)
            cnt_500ms++;
          else
          {
            cnt_500ms = 0;
            flag_500ms = 1;
            if (cnt_1s < 1)
              cnt_1s++;
            else
            {
              cnt_1s = 0;
              timer_1_seconde++;
              clock_in_sec_last_startup++;
              flag_1s = 1;
              clock_error_sec++;
            }
          }    
        }  
      }
    }    
  }  
  if (clock_error)
  {
    if (cnt_31_25_ms < cnt_31_25_ms_max)
      cnt_31_25_ms++;
    else
    {
      cnt_31_25_ms = 0;
      timer_31_25_milli_seconde++;
      if (cnt_31_25_ms_correct < 3)
      {
        cnt_31_25_ms_max = 30;
        cnt_31_25_ms_correct++;
      }
      else
      {
        // 125 milli seconde
        cnt_31_25_ms_max = 31;
        cnt_31_25_ms_correct = 0;
      }
    }
  }
}

static interrupt IIC_RTC_CLOCKOUT_INT_ADR using(IIC_RTC_CLOCKOUT_INT_RB) void IIC_CLkout_Interrupt(void)
{
static unsigned char startup = 1;
static int cnt_125ms = 0;
static int cnt_500ms = 0;
static int cnt_1s = 0;
static int clock_ok_cnt = 0;

  if (clock_error)
  {
    if (cnt_1s < 31)
      cnt_1s++;
    else  
    {
      cnt_1s = 0;
      if ((clock_check_timer > IIC_RTC_MIN_MSEC) &&
          (clock_check_timer < IIC_RTC_MAX_MSEC))
      {    
        if (clock_ok_cnt < 10)
          clock_ok_cnt++;
        else  
        {
          clock_error = 0;   
          clock_ok_cnt = 0;
        }
      } 
      else 
        clock_ok_cnt = 0; 
      clock_check_timer = 0;  
    }  
    cnt_125ms = 0;
  }
  else
  {
    timer_31_25_milli_seconde++;
    if (cnt_125ms < 3)
      cnt_125ms++;
    else
    {
      cnt_125ms = 0;
      flag_125ms = 1;
      if (cnt_500ms < 3)
        cnt_500ms++;
      else
      {
        cnt_500ms = 0;
        flag_500ms = 1;
        if (cnt_1s < 1)
          cnt_1s++;
        else
        {
          cnt_1s = 0;
          flag_1s = 1;
          timer_1_seconde++;
          if ((clock_check_timer < IIC_RTC_MIN_MSEC) ||
              (clock_check_timer > IIC_RTC_MAX_MSEC))
          {    
            if (startup)
              startup = 0;
            else
            {
              clock_ok_cnt = 0;
              clock_error = 1; 
            }  
          }  
          clock_check_timer = 0;  
          clock_in_sec_last_startup++;
        }
      }    
    }
  }  
}

/********************************************************************************************************/
// Set a timer. Function TimerExpired will return true after the timer has expired.
// Input: TTimer *Timer -> pointer to the timer.
// Input: int Interval  -> interval before timer expires (x125ms).
void TimerSet(TTimer *Timer, unsigned int Interval)
{
  Timer->Interval  = Interval;
  Timer->Starttime = SystemTimer;
}

// Resets the timer with the same interval that was set in TimerSet. The starttime is
// the exact time the timer last expired. This way the timer will expire every interval.
// In : TTimer *Timer -> pointer to the timer.
void TimerReset(TTimer *Timer)
{
  Timer->Starttime += Timer->Interval;
}

// Restarts the timer with the same interval that was set in TimerSet. The timer will start
// at the current time.
// In : TTimer *Timer -> pointer to the timer.
void TimerRestart(TTimer *Timer)
{
  Timer->Starttime = SystemTimer;
}

// Check if timer has expired. The function returns 1 when the timer has expired.
// In : TTimer *Timer -> pointer to the timer.
int TimerExpired(TTimer *Timer)
{
  if ((SystemTimer - Timer->Starttime) >= Timer->Interval)
    return (1);
  else
    return (0);
}

//-----------------------------------------------------------------------------
volatile unsigned int timer_1_milli_seconde;
volatile unsigned int timer_31_25_milli_seconde;
volatile unsigned int timer_1_seconde;

void Timer_Set(s_timer *timer, unsigned int interval, volatile unsigned int *ptr_timer)
{
  timer->ptr_timer = ptr_timer;
  timer->interval = interval;
  timer->start = *timer->ptr_timer;
}

void Timer_Reset(s_timer *timer)
{
  timer->start += timer->interval;
}

void Timer_Restart(s_timer *timer)
{
  timer->start = *timer->ptr_timer;
}

unsigned char Timer_Expired(s_timer *timer)
{
  if ((*timer->ptr_timer - timer->start) >= timer->interval)
  {
    return (1);
  }  
  else
    return (0);
}

unsigned int Timer_Delay(s_timer *timer)
{
  return (*timer->ptr_timer - timer->start);
}

unsigned int Timer_Rest(s_timer *timer)
{
  if (timer->ptr_timer == NULL)
    return (0);
  return (timer->interval - (*timer->ptr_timer - timer->start));
}
