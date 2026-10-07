// C__IO_RS485.C

#include "ch_define.h"
#include "ch_IO_RS485.h"

//- RX ----------------------------------------------------------------------------
static void Reset_Receive_Buffer(s_value_RS485_bus* RS485Bus)
{
  RS485Bus->RxGetIndex = 0;
  RS485Bus->RxPutIndex = 0;
  RS485Bus->RxInBuffer = 0;
  RS485Bus->MessageReady = 0;
}

static void Write_Byte_Receive_Buffer(s_value_RS485_bus* RS485Bus, unsigned char data)
{
  if (RS485Bus->RxInBuffer < RS485_BUFFER_SIZE)
  {
    RS485Bus->RxBuffer[RS485Bus->RxPutIndex] = data;
    RS485Bus->RxInBuffer++;
	RS485Bus->RxPutIndex++;
	if (RS485Bus->RxPutIndex >= RS485_BUFFER_SIZE)
	  RS485Bus->RxPutIndex = 0;
  }
}

static void Write_Receive_Buffer(s_value_RS485_bus* RS485Bus, unsigned char* data, unsigned char length)
{
int i;

  for (i = 0; i < length; i++)
    Write_Byte_Receive_Buffer(RS485Bus, data[i]);
}

void IO_RS485_PDO_Receive_Transparant(s_value_RS485_bus* RS485Bus, unsigned char *data, unsigned char length, s_board_IO_on_off* IO, void (*PDO_RS485_bus_received)(s_board_IO_on_off *IO))
{
  if (RS485Bus->MessageReady) // Previous message in buffer -> reset buffer
    Reset_Receive_Buffer(RS485Bus);
  Write_Receive_Buffer(RS485Bus, &data[1], length - 1);

  // PDO event
  if ((data[0] & 0x20) == 0) // all data received
  {
    RS485Bus->MessageReady = 1;
    if (PDO_RS485_bus_received != NULL)
      PDO_RS485_bus_received(IO);
  }
}

void IO_RS485_PDO_Receive_Modbus(s_value_RS485_bus* RS485Bus, unsigned char *data, unsigned char length, s_board_IO_on_off* IO, void (*PDO_RS485_bus_received)(s_board_IO_on_off *IO))
{
unsigned char command, dataBytes, toggleBit;
length;

  command = data[0] & 0x0F;
  toggleBit = (data[0] & 0x10) >> 4;
  dataBytes = (data[0] >> 5) & 0x07;

  switch (command)
  {
      case cmdStartMsgMax7Bytes:
      case cmdStartPeriodicMsgMax7Bytes:
        Reset_Receive_Buffer(RS485Bus);
		Write_Byte_Receive_Buffer(RS485Bus, command);
		Write_Byte_Receive_Buffer(RS485Bus, dataBytes);
        RS485Bus->RxBusy = 0;
        break;
      case cmdStartMsgMore7Bytes:
      case cmdStartPeriodicMsgMore7Bytes:
        Reset_Receive_Buffer(RS485Bus);
		Write_Byte_Receive_Buffer(RS485Bus, command);
		Write_Byte_Receive_Buffer(RS485Bus, dataBytes);
        RS485Bus->RxBusy = 1;
        break;
      case cmdNextMsgBlock:
        if ((RS485Bus->RxBusy == 0) || (RS485Bus->RxToggle == toggleBit))
        {
		  Reset_Receive_Buffer(RS485Bus);
          RS485Bus->RxBusy = 0;
          return; // Error - No start command received / ToggleBit
        }
        RS485Bus->RxBusy = 1;
        RS485Bus->RxBuffer[1] += dataBytes;
        break;
      case cmdLastMsgBlock:
        if ((RS485Bus->RxBusy == 0) || (RS485Bus->RxToggle == toggleBit))
        {
		  Reset_Receive_Buffer(RS485Bus);
          RS485Bus->RxBusy = 0;
          return; // Error - No start command received / ToggleBit
        }
        RS485Bus->RxBusy = 0;
        RS485Bus->RxBuffer[1] += dataBytes;
        break;
      case cmdTimeOutPeriodicMsg:
	    Reset_Receive_Buffer(RS485Bus);
		Write_Byte_Receive_Buffer(RS485Bus, command);
		Write_Byte_Receive_Buffer(RS485Bus, dataBytes);
        RS485Bus->RxBusy = 0;
        break;
      case cmdStopPeriodicMsgMax7Bytes:
      case cmdStopPeriodicMsgMore7Bytes:
      case cmdStopAllPeriodicMsg:
      case cmdTimeOutMsg:
      default: // unknown command
        return;
  }

  Write_Receive_Buffer(RS485Bus, &data[1], dataBytes);
  RS485Bus->RxToggle = toggleBit;

  // PDO event
  if (RS485Bus->RxBusy == 0) // all data received
  {
    RS485Bus->MessageReady = 1;
    if (PDO_RS485_bus_received != NULL)
      PDO_RS485_bus_received(IO);
  }
}

//- TX ----------------------------------------------------------------------------
static unsigned char Read_Byte_Transmit_Buffer(s_value_RS485_bus* RS485Bus)
{
unsigned char data = 0;

  if (RS485Bus->TxInBuffer > 0)
  {
    data = RS485Bus->TxBuffer[RS485Bus->TxGetIndex];
    RS485Bus->TxInBuffer--;
	RS485Bus->TxGetIndex++;
	if (RS485Bus->TxGetIndex >= RS485_BUFFER_SIZE)
	  RS485Bus->TxGetIndex = 0;
  }
  return data;
}

unsigned char IO_RS485_PDO_Transmit_Transparant(s_value_RS485_bus* RS485Bus, unsigned char *data)
{
unsigned char Transmit;
int DataBytes;
int i;

  DataBytes = RS485Bus->TxInBuffer;
  if (DataBytes > 7)
  {
    DataBytes = 7;
    Transmit = 0;
  }
  else
    Transmit = 1;

  data[0] = DataBytes & 0x07;
  if (Transmit)
    data[0] |= 0x40;

  for (i = 0; i < DataBytes; i++)
  {
	if (RS485Bus->TxInBuffer > 0)
      data[i+1] = Read_Byte_Transmit_Buffer(RS485Bus);
  }
  RS485Bus->ctrl &= 0xFFFE;
  RS485Bus->ctrl |= 0x0100;
  return (DataBytes + 1);
}

unsigned char IO_RS485_PDO_Transmit_Modbus(s_value_RS485_bus* RS485Bus, unsigned char *data)
{
unsigned char command = 0;
unsigned char dataBytes = 0;
unsigned char i;

  if (RS485Bus->TxBusy == 0)
  {
    if (RS485Bus->TxInBuffer > 0)
	  command = Read_Byte_Transmit_Buffer(RS485Bus);
    if (RS485Bus->TxInBuffer > 0)
	  dataBytes = Read_Byte_Transmit_Buffer(RS485Bus);
	if (dataBytes > 7)
	{
	  dataBytes = 7;
	  RS485Bus->TxBusy = 1;
	}
  }
  else
  {
    dataBytes = RS485Bus->TxInBuffer;
	if (dataBytes > 7)
	{
	  dataBytes = 7;
	  command = cmdNextMsgBlock;
	}
	else
	{
	  command = cmdLastMsgBlock;
	  RS485Bus->TxBusy = 0;
	}
  }

  data[0]  =  command   & 0x0F;
  data[0] |= (dataBytes & 0x07) << 5;
  if (RS485Bus->TxToggle)
    data[0] |= 0x10;

  for (i = 0; i < dataBytes; i++)
  {
    if (RS485Bus->TxInBuffer > 0)
      data[i+1] = Read_Byte_Transmit_Buffer(RS485Bus);
  }
  RS485Bus->TxToggle ^= 1;
  RS485Bus->ctrl &= 0xFFFE;
  RS485Bus->ctrl |= 0x0100;
  return (dataBytes + 1);
}
