// CH_RTC.H

#ifndef _CH_RTC_H
#define _CH_RTC_H

//void RTC_Init(void);
void RTC_Read(void);
void RTC_Write(void);

extern bit RTC_init_switch;
extern bit RTC_read_switch;
extern bit RTC_write_switch;

bit RTC_Set_RTC_With_Timer_Clock(void);

#endif
