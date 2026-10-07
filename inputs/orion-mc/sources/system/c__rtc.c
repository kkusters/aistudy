// C__RTC.C

#include <time.h>

#include "ch_define.h"
#include "ch_alg.h"
#include "ch_i2c.h"
#include "ch_tijd.h"
#include "ch_timer.h"
#include "ch_rtc.h"

// CLKOUT is on
#define CLKOUT_CTRL 0x82
#define IIC_ADDRESS_RTC ((unsigned char)0xA2)

// structur for registers in RTC-IC
typedef struct
{
  unsigned char Ctrl_Stat_1;
  unsigned char Ctrl_Stat_2;
  unsigned char VL_Seconds;
  unsigned char Minutes;
  unsigned char Hours;
  unsigned char Days;
  unsigned char Weekdays;
  unsigned char Cent_Months;
  unsigned char Years;
  unsigned char Minute_Alarm;
  unsigned char Hour_Alarm;
  unsigned char Day_Alarm;
  unsigned char Weekday_Alarm;
  unsigned char CLKout_Ctrl;
  unsigned char Timer_Ctrl;
  unsigned char Timer;
} s_RTC;

struct tm RTC_time_not_corrected;
time_t RTC_time_in_sec;
s_RTC RTC; // last correct readed RTC data if rtc.timer != 0xFF not readed since startup  
s_RTC rtc_receive; // new not checked RTC data 
bit RTC_init_switch = 1;
bit RTC_read_switch = 1;
bit RTC_write_switch = 0;

const s_RTC RTC_init_code =
{
  0x00, // Ctrl_Stat_1
  0x00, // Ctrl_Stat_1
  0x00, // VL_Seconds
  0x00, // Minutes
  0x00, // Hours
  0x01, // Days
  0x00, // Weekdays
  0x01, // Cent_Months
  0x00, // Years
  0x80, // Minute_alarm
  0x80, // Hour_alarm
  0x80, // Day_alarm
  0x80, // Weekday_alarm
  CLKOUT_CTRL, // CLKout_Ctrl
  0x03, // Time_Ctrl
  0xAA  // Timer 
};

const s_RTC RTC_mask =
{
  0xA8, // Ctrl_Stat_1
  0x1F, // Ctrl_Stat_1
  0xFF, // VL_Seconds
  0x7F, // Minutes
  0x3F, // Hours
  0x3F, // Days
  0x07, // Weekdays
  0x9F, // Cent_Months
  0xFF, // Years
  0xFF, // Minute_alarm
  0xBF, // Hour_alarm
  0xBF, // Day_alarm
  0x83, // Weekday_alarm
  0x83, // CLKout_Ctrl
  0x83, // Time_Ctrl
  0xFF  // Timer 
};

static _near void RTC_Error(unsigned char error) // error = 1 by fault and 0 if ok
{
static int error_cnt = 0;

  if (error)
  {
    error_cnt++;
    if (!RTC_init_switch &&
        (error_cnt >= 10))
    {
      error_cnt = 10;
	  // alarm RTC zetten
      RTC_init_switch = 1;
	  while (1);
    }
  }
  else
  {
    error_cnt = 0;
  } 
}

static bit RTC_Send_Data(unsigned char *RTC_ptr)
{
  if (IIC_Write(IIC_ADDRESS_RTC, 0, 0, IIC_ONE_SUB_ADDRESS, RTC_ptr,16))
  {
    RTC_Error(1);
    return (1);
  }  
  RTC_Error(0);
  return (0);
}

static bit RTC_Receive_Data(void)
{
unsigned char loop;
//unsigned char ch;
unsigned char data[20];
unsigned char *ch_ptr = (unsigned char *)&rtc_receive;
unsigned char *mask_ptr = (unsigned char *)&RTC_mask;

  if (IIC_Read(IIC_ADDRESS_RTC, 0, 0, IIC_ONE_SUB_ADDRESS, (unsigned char *)&data, 16)) // read rtc data 
  {
    // error receiving data
    RTC_Error(1);
    return (1);
  }
  for (loop = 0; loop < 16; loop++)
  {
    *ch_ptr = data[loop] & *mask_ptr++;
    ch_ptr++;
  }
  RTC_Error(0);
  return (0);
}

bit RTC_Set_RTC_With_Timer_Clock(void)
{
s_RTC RTC_send = RTC_init_code;

  RTC_send.VL_Seconds  = Dec_Byte_To_Bcd(tijd.tm_sec);
  RTC_send.Minutes     = Dec_Byte_To_Bcd(tijd.tm_min);
  RTC_send.Hours       = Dec_Byte_To_Bcd(tijd.tm_hour);
  RTC_send.Days        = Dec_Byte_To_Bcd(tijd.tm_mday);
  RTC_send.Cent_Months = Dec_Byte_To_Bcd(tijd.tm_mon);
  if (tijd.tm_year < 2000)
    RTC_send.Cent_Months = RTC_send.Cent_Months | 0x80;
  RTC_send.Years      = Dec_Byte_To_Bcd(tijd.tm_year % 100);
  RTC_send.Weekdays   = Dec_Byte_To_Bcd(tijd.tm_wday);
  return (RTC_Send_Data((unsigned char *)&RTC_send));
}

void RTC_Set_Timer_Clock_With_RTC(void)
{
  RTC_time_not_corrected.tm_sec  = Bcd_Byte_To_Dec(RTC.VL_Seconds & 0x007F);
  RTC_time_not_corrected.tm_min  = Bcd_Byte_To_Dec(RTC.Minutes);
  RTC_time_not_corrected.tm_hour = Bcd_Byte_To_Dec(RTC.Hours);
  RTC_time_not_corrected.tm_mday = Bcd_Byte_To_Dec(RTC.Days);
  RTC_time_not_corrected.tm_mon  = (unsigned int)(Bcd_Byte_To_Dec(RTC.Cent_Months & 0x1F) - 1);
  RTC_time_not_corrected.tm_year = Bcd_Byte_To_Dec(RTC.Years);
  RTC_time_not_corrected.tm_year += (RTC.Cent_Months & 0x80) ? 0 : 100;
  RTC_time_in_sec = mktime((struct tm *)&RTC_time_not_corrected);
  if (RTC_time_in_sec == -1)
  {
    // error time in RTC not correct
	RTC_Set_RTC_With_Timer_Clock();
//	alarm_hr.RTC_al |= AL_ON;
  }
  else
    _stime(&RTC_time_in_sec);
}

bit RTC_Send_Init_Code(void)
{
  return (RTC_Send_Data((unsigned char *)&RTC_init_code));
}

static bit RTC_Check(void)
{
  if (rtc_receive.VL_Seconds & 0x80) // klok spanning is weg geweest
  {
//    RTC_voltage_low = 1; // wordt gebruikt bij controle data setpoints en options JP 09-08-07
//	alarm_hr.RTC_al |= AL_ON; JP 09-08-07
    RTC_init_switch = 1;
	RTC_Send_Init_Code();
	// alarm RTC zetten
    return (1); // RTC not correct
  }
  else if ((rtc_receive.Ctrl_Stat_1 != 0) ||
           (rtc_receive.Ctrl_Stat_2 != 0) ||
           (rtc_receive.Minute_Alarm != 0x80) ||
           (rtc_receive.Hour_Alarm != 0x80) ||
           (rtc_receive.Day_Alarm != 0x80) ||
           (rtc_receive.Weekday_Alarm != 0x80) ||
           (rtc_receive.CLKout_Ctrl != CLKOUT_CTRL) ||
           (rtc_receive.Timer_Ctrl != 0x03) ||
           (rtc_receive.Timer != 0xAA))
  {
//	alarm_hr.RTC_al |= AL_ON; JP 09-08-07
    RTC_init_switch = 1;
    RTC_Set_RTC_With_Timer_Clock();
	// alarm RTC zetten
    return (1); // RTC not correct
  }
  else
    return (0); // RTC correct
}

void RTC_Read(void)
// Write data out of RTC-tijd in to CPU-tijd
{
  if (RTC_Receive_Data() ||
      RTC_Check())
    return;
  RTC = rtc_receive;   
  RTC_Set_Timer_Clock_With_RTC();
  RTC_read_switch = 0;
}

void RTC_Write(void)
// write data out of CPU-tijd in to RTC-tijd
{
  if (RTC_Set_RTC_With_Timer_Clock())
// as possible build in RTC_Receive_Data and RTC_check data to check if data is write correctly into RTC
    return;  
  RTC_write_switch = 0;
}
