// CH_IO_RS485.H

#ifndef _CH_IO_RS485_H
#define _CH_IO_RS485_H

typedef enum
{
  cmdStartMsgMax7Bytes          = 0,
  cmdStartMsgMore7Bytes         = 1,
  cmdStartPeriodicMsgMax7Bytes  = 2,
  cmdStartPeriodicMsgMore7Bytes = 3,
  cmdStopPeriodicMsgMax7Bytes   = 4,
  cmdStopPeriodicMsgMore7Bytes  = 5,
  cmdNextMsgBlock               = 6,
  cmdLastMsgBlock               = 7,
  cmdStopAllPeriodicMsg         = 8,
  cmdTimeOutMsg                 = 9,
  cmdTimeOutPeriodicMsg         = 10
} TRS485ModbusCommand;


void IO_RS485_PDO_Receive_Transparant(s_value_RS485_bus* RS485Bus, unsigned char *data, unsigned char length, s_board_IO_on_off* IO, void (*PDO_RS485_bus_received)(s_board_IO_on_off *IO));
void IO_RS485_PDO_Receive_Modbus(s_value_RS485_bus* RS485Bus, unsigned char *data, unsigned char length, s_board_IO_on_off* IO, void (*PDO_RS485_bus_received)(s_board_IO_on_off *IO));

unsigned char IO_RS485_PDO_Transmit_Transparant(s_value_RS485_bus* RS485Bus, unsigned char *data);
unsigned char IO_RS485_PDO_Transmit_Modbus(s_value_RS485_bus* RS485Bus, unsigned char *data);

#endif
