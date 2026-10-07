// CH_IO_08_09_DAC.H

#ifndef _CH_IO_08_09_DAC_H
#define _CH_IO_08_09_DAC_H

unsigned char IO_08_09_Analog_Output_PDO_Transmit(unsigned char *ptr, unsigned char card_nr, unsigned char component_nr);
void IO_08_09_Analog_Output_PDO_Transmit_OK(unsigned char card_nr, unsigned char component_nr);
unsigned int IO_08_09_Analog_Output_SDO_Master_Transmit(u_SDO_data *ptr, unsigned char card_nr, unsigned char component_nr);
void IO_08_09_Analog_Output_SDO_Master_Transmit_OK(unsigned char card_nr, unsigned char component_nr);
void IO_08_09_Analog_Output_SDO_Master_Receive(u_SDO_data *ptr, unsigned char card_nr, unsigned char component_nr);
void IO_08_09_Analog_Output_Init(unsigned char card_nr);
void IO_08_09_Analog_Output_100ms(unsigned char card_nr);

#endif
