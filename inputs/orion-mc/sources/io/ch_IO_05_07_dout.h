// CH_IO_05_07_DOUT.H

#ifndef _CH_IO_05_07_DOUT_H
#define _CH_IO_05_07_DOUT_H

unsigned char IO_05_07_Digital_Output_PDO_Transmit(unsigned char *ptr, unsigned char card_nr, unsigned char component_nr);
void IO_05_07_Digital_Output_PDO_Transmit_OK(unsigned char card_nr, unsigned char component_nr);
unsigned int IO_05_07_Digital_Output_SDO_Master_Transmit(u_SDO_data *ptr, unsigned char card_nr, unsigned char component_nr);
void IO_05_07_Digital_Output_SDO_Master_Transmit_OK(unsigned char card_nr, unsigned char component_nr);
void IO_05_07_Digital_Output_SDO_Master_Receive(u_SDO_data *ptr, unsigned char card_nr, unsigned char component_nr);
void IO_05_07_Digital_Output_Init(unsigned char card_nr);
void IO_05_07_Digital_Output_100ms(unsigned char card_nr);

#endif
