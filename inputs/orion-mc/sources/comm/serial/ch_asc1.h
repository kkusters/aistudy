// CH_ASC1.H

#ifndef _CH_ASC1_H
#define _CH_ASC1_H

#include <stdarg.h>
#include <stdio.h> 

//#define ASC1_CAN_AL 1
//#define ASC1_DIERWEGING 1
//#define ASC1_HEARTBEAT 1
//#define ASC1_CAN_IO 1
//#define ASC1_CAN_FWS 1
//#define ASC1_CAN_INTERFACE 1
//#define ASC1_KLEP 1
//#define ASC1_HOPPER 1
//#define ASC1_TIME 1
//#define ASC1_LOG_VOER 1
// voer computer
//#define ASC1_FWS 1
//#define ASC1_SILO 1

extern unsigned char asc1_receive_cnt;
extern unsigned char asc1_transmit_state;
extern unsigned char asc1_init_switch;
extern unsigned char asc1_send_at;
extern unsigned char asc1_modem_dcd;
extern unsigned char asc1_dcd;
extern unsigned char asc1_modem_init;
extern unsigned char asc1_modem_init_state;

extern unsigned char asc1_diag_reset_flag;
extern unsigned int asc1_diag_in_cnt;
extern unsigned int asc1_diag_in_cnt_peak;
extern unsigned int asc1_diag_out_cnt;
extern unsigned int asc1_diag_out_cnt_peak;
extern unsigned int asc1_diag_in_out_cnt;
extern unsigned int asc1_diag_in_out_cnt_peak;

void ASC1_Control(void);
void ASC1_Timing_Control(void);
char ASC1_Get_Char(void);
char *ASC1_Get_String(char *string);
char *ASC1_Get_String_Nr(char *string, unsigned int max);
void ASC1_Put_Char(char ch);
void ASC1_Put_String(char *string);
void ASC1_Printf(const char *format, ... );

#endif
