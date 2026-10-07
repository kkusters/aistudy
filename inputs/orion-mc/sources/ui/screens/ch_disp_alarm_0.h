// CH_DISP_ALARM_0.H

#ifndef _CH_DISP_ALARM_0_H
#define _CH_DISP_ALARM_0_H

#include "ch_alarm.h"
#include "ct_disp.h"

#define REGEL_1 13
#define REGEL_2	30
#define REGEL_3	47
#define REGEL_4 64
#define REGEL_5 80

extern s_screen screen_alarm_0;
extern s_alarm_disp alarm_actueel;
extern s_alarm_disp laatste_alarm_actueel;
extern TAlarm const AlarmTable[];

void Alarm_Add_To_Disp(void *dest[], void * const source[]);
void Alarm_Zet_Disp(void *lcd[], int nr);

void If_Exist_Goto_Screen_Alarm_0(void);

#ifdef SD_CARD
void Alarm_Create_String_SD(char *string, s_alarm_disp *alarm);
#endif // SD_CARD
#ifdef ALARM_TEKST_NAAR_SMARTLINK
void Alarm_Create_String_PC(char *string, s_alarm_disp *alarm);
#endif // ALARM_TEKST_NAAR_SMARTLINK

#endif