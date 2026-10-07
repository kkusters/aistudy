// CH_RS232_0.H

#ifndef _CH_RS232_0_H
#define _CH_RS232_0_H

#include "ch_pc_com.h"

extern int rs232_0_main_group_blocked;
extern s_pc_com rs232_0_pc_com;
extern s_pc_com rs232_0_orion_com;

void RS232_0_Init(void);
void RS232_0_125ms_Time_Out(void);
void RS232_0_Control(void);

#endif // _CH_RS232_0_H
