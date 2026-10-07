// CH_IO_EKU_MC.H

#ifndef _CH_IO_EKU_MC_H
#define _CH_IO_EKU_MC_H

unsigned char IO_EKU_Motor_Control_PDO_Transmit(unsigned char *ptr, unsigned char card_nr, unsigned char component_nr);
void IO_EKU_Motor_Control_PDO_Transmit_OK(unsigned char card_nr, unsigned char component_nr);
void IO_EKU_Motor_Control_PDO_Receive(unsigned char *ptr, unsigned char length, unsigned char card_nr, unsigned char component_nr);
unsigned int IO_EKU_Motor_Control_SDO_Master_Transmit(u_SDO_data *ptr, unsigned char card_nr, unsigned char component_nr);
void IO_EKU_Motor_Control_SDO_Master_Transmit_OK(unsigned char card_nr, unsigned char component_nr);
void IO_EKU_Motor_Control_SDO_Master_Receive(u_SDO_data *ptr, unsigned char card_nr, unsigned char component_nr);
void IO_EKU_Motor_Control_Init(unsigned char card_nr);
void IO_EKU_Motor_Control_100ms(unsigned char card_nr);

#endif
