// CH_IO_06_14.H

#ifndef _CH_IO_06_14_H
#define _CH_IO_06_14_H

#include "ct_data.h"

void IO_06_14_Alarm_All_Boards(void);
unsigned char IO_06_14_Component_PDO_Transmit(unsigned char *ptr, unsigned char card_nr, unsigned char component_id, unsigned char component_nr);
void IO_06_14_Component_PDO_Transmit_OK(unsigned char card_nr, unsigned char component_id, unsigned char coponent_nr);
void IO_06_14_Component_PDO_Receive(unsigned char *ptr, unsigned char length, unsigned char card_nr, unsigned char component_id, unsigned char component_nr);
unsigned int IO_06_14_Component_SDO_Master_Transmit(u_SDO_data *ptr, unsigned char card_nr, unsigned char component_id, unsigned char component_nr);
void IO_06_14_Component_SDO_Master_Transmit_OK(unsigned char card_nr, unsigned char component_id, unsigned char component_nr);
void IO_06_14_Component_SDO_Master_Receive(u_SDO_data *ptr, unsigned char card_nr, unsigned char component_id, unsigned char component_nr);
void IO_06_14_Init(unsigned char card_nr);
void IO_06_14_100ms(unsigned char card_nr);

unsigned char IO_06_14_CAN_Check_PDO_Component(void (*CAN_PDO_Transmit)(unsigned char priority,
                                                                         unsigned char card_id, unsigned char card_nr, 
                                                                         unsigned char component_id, unsigned char component_nr));
unsigned char IO_06_14_CAN_Check_SDO_Master_Component(void (*CAN_SDO_Master_Transmit)(unsigned char priority,
                                                                                       unsigned char card_id, unsigned char card_nr, 
                                                                                       unsigned char component_id, unsigned char component_nr),
                                                       void (*CAN_SDO_Master_Receive)(unsigned char priority,
                                                                                      unsigned char card_id, unsigned char card_nr, 
                                                                                      unsigned char component_id, unsigned char component_nr));

#endif
