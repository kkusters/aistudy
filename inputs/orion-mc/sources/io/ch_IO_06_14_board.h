// CH_IO_06_14_BOARD.C

#ifndef _CH_IO_06_14_BOARD_H
#define _CH_IO_06_14_BOARD_H

extern unsigned char IO_06_14_init_switch[IO_06_14_MAX];

void IO_06_14_Alarm_Check(unsigned char alarm);
unsigned char IO_06_14_Board_Component_PDO_Transmit(unsigned char *ptr, unsigned char card_nr);
void IO_06_14_Board_Component_PDO_Transmit_OK(unsigned char card_nr);
void IO_06_14_Board_Component_PDO_Receive(unsigned char *ptr, unsigned char length, unsigned char card_nr);
unsigned int IO_06_14_Board_Component_SDO_Master_Transmit(u_SDO_data *ptr, unsigned char card_nr);
void IO_06_14_Board_Component_SDO_Master_Transmit_OK(unsigned char card_nr);
void IO_06_14_Board_Component_SDO_Master_Receive(u_SDO_data *ptr, unsigned char card_nr);
void IO_06_14_Board_Component_Init(unsigned char card_nr);
unsigned char IO_06_14_Board_Component_100ms(unsigned char card_nr);

#endif