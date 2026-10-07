// CH_RS232_1.H

#ifndef _CH_RS232_1_H
#define _CH_RS232_1_H

#if (PROCESSOR==XC161CJ)

#include "ch_pc_com.h"

extern int rs232_1_main_group_blocked;
extern s_pc_com rs232_1_pc_com;
extern s_pc_com rs232_1_orion_com;

#endif // (PROCESSOR==XC161CJ)

void RS232_1_125ms_Time_Out(void);
void RS232_1_Control(void);
void RS232_1_Init(void);

#endif // _CH_RS232_1_H
