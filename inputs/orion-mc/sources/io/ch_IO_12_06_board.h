// CH_IO_12_06_BOARD.C

#ifndef _CH_IO_12_06_BOARD_H
#define _CH_IO_12_06_BOARD_H

extern unsigned char IO_12_06_init_switch[IO_12_06_MAX];

void IO_12_06_Alarm_Check(unsigned char alarm);
unsigned char IO_12_06_Board_Component_PDO_Transmit(unsigned char *ptr, unsigned char card_nr);
void IO_12_06_Board_Component_PDO_Transmit_OK(unsigned char card_nr);
void IO_12_06_Board_Component_PDO_Receive(unsigned char *ptr, unsigned char length, unsigned char card_nr);
unsigned int IO_12_06_Board_Component_SDO_Master_Transmit(u_SDO_data *ptr, unsigned char card_nr);
void IO_12_06_Board_Component_SDO_Master_Transmit_OK(unsigned char card_nr);
void IO_12_06_Board_Component_SDO_Master_Receive(u_SDO_data *ptr, unsigned char card_nr);
void IO_12_06_Board_Component_Init(unsigned char card_nr);
unsigned char IO_12_06_Board_Component_100ms(unsigned char card_nr);

#endif