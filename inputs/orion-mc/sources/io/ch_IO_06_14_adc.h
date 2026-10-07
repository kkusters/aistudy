// CH_IO_06_14_ADC.H

#ifndef _CH_IO_06_14_ADC_H
#define _CH_IO_06_14_ADC_H

void IO_06_14_Analog_Input_PDO_Receive(unsigned char *ptr, unsigned char length, unsigned char card_nr, unsigned char component_nr);
unsigned int IO_06_14_Analog_Input_SDO_Master_Transmit(u_SDO_data *ptr, unsigned char card_nr, unsigned char component_nr);
void IO_06_14_Analog_Input_SDO_Master_Transmit_OK(unsigned char card_nr, unsigned char component_nr);
void IO_06_14_Analog_Input_SDO_Master_Receive(u_SDO_data *ptr, unsigned char card_nr, unsigned char component_nr);
void IO_06_14_Analog_Input_Init(unsigned char card_nr);

#endif
