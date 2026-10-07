// CH_ETHERNET.H

#ifndef __CH_ETHERNET_H
#define __CH_ETHERNET_H

#ifdef ETHERNET

#include "ch_pc_com.h" 

extern int ethernet_main_group_blocked[4];
extern s_pc_com ethernet_pc_com[4];
extern s_pc_com ethernet_orion_com[4];

void Ethernet_125ms_Time_Out(void);
void Ethernet_Control_Port(unsigned char conn_nr);
void Ethernet_Control(void);

#endif // ETHERNET

#endif //  __CH_RS232_0_H

