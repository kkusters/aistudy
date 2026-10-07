// CH_IO_07_07_DIGIN.H

#ifndef _CH_IO_07_07_DIGIN_H
#define _CH_IO_07_07_DIGIN_H

void IO_07_07_Digital_Input_PDO_Receive(unsigned char *ptr, unsigned char length, unsigned char card_nr, unsigned char component_nr);
unsigned int IO_07_07_Digital_Input_SDO_Master_Transmit(u_SDO_data *ptr, unsigned char card_nr, unsigned char component_nr);
void IO_07_07_Digital_Input_SDO_Master_Transmit_OK(unsigned char card_nr, unsigned char component_nr);
void IO_07_07_Digital_Input_SDO_Master_Receive(u_SDO_data *ptr, unsigned char card_nr, unsigned char component_nr);
void IO_07_07_Digital_Input_Init(unsigned char card_nr);

#endif
