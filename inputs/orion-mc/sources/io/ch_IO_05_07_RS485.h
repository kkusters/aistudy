// CH_IO_05_07_RS485.H

#ifndef _CH_IO_05_07_RS485_H
#define _CH_IO_05_07_RS485_H

void IO_05_07_RS485_Bus_PDO_Receive(unsigned char *ptr, unsigned char length, unsigned char card_nr, unsigned char component_nr);
unsigned char IO_05_07_RS485_Bus_PDO_Transmit(unsigned char *ptr, unsigned char card_nr, unsigned char component_nr);
void IO_05_07_RS485_Bus_PDO_Transmit_OK(unsigned char card_nr, unsigned char component_nr);
unsigned int IO_05_07_RS485_Bus_SDO_Master_Transmit(u_SDO_data *ptr, unsigned char card_nr, unsigned char component_nr);
void IO_05_07_RS485_Bus_SDO_Master_Transmit_OK(unsigned char card_nr, unsigned char component_nr);
void IO_05_07_RS485_Bus_SDO_Master_Receive(u_SDO_data *ptr, unsigned char card_nr, unsigned char component_nr);
void IO_05_07_RS485_Bus_Init(unsigned char card_nr);

#endif
