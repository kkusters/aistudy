// CH_IO_07_07_DAC.H

#ifndef _CH_IO_07_07_DAC_H
#define _CH_IO_07_07_DAC_H

unsigned char IO_07_07_Analog_Output_PDO_Transmit(unsigned char *ptr, unsigned char card_nr, unsigned char component_nr);
void IO_07_07_Analog_Output_PDO_Transmit_OK(unsigned char card_nr, unsigned char component_nr);
unsigned int IO_07_07_Analog_Output_SDO_Master_Transmit(u_SDO_data *ptr, unsigned char card_nr, unsigned char component_nr);
void IO_07_07_Analog_Output_SDO_Master_Transmit_OK(unsigned char card_nr, unsigned char component_nr);
void IO_07_07_Analog_Output_SDO_Master_Receive(u_SDO_data *ptr, unsigned char card_nr, unsigned char component_nr);
void IO_07_07_Analog_Output_Init(unsigned char card_nr);
void IO_07_07_Analog_Output_100ms(unsigned char card_nr);

#endif
