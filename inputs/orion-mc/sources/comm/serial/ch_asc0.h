// CH_ASC0.H

#ifndef _CH_ASC0_H
#define _CH_ASC0_H

#include <stdarg.h>
#include <stdio.h>

extern unsigned char asc0_receive_cnt;
extern unsigned char asc0_transmit_state;
extern unsigned char asc0_init_switch;
extern unsigned char asc0_send_at;
extern unsigned char asc0_modem_init;
extern unsigned char asc0_modem_init_state;

extern unsigned char asc0_diag_reset_flag;
extern unsigned int asc0_diag_in_cnt;
extern unsigned int asc0_diag_in_cnt_peak;
extern unsigned int asc0_diag_out_cnt;
extern unsigned int asc0_diag_out_cnt_peak;
extern unsigned int asc0_diag_in_out_cnt;
extern unsigned int asc0_diag_in_out_cnt_peak;

void ASC0_Control(void);
void ASC0_Timing_Control(void);
char ASC0_Get_Char(void);
char *ASC0_Get_String(char *string);
char *ASC0_Get_String_Nr(char *string, unsigned int max);
void ASC0_Put_Char(char ch);
void ASC0_Put_String(char *string);
void ASC0_Printf(const char *format, ... );

#endif
