// C__IO_07_07_RS485.C

#include "ch_define.h"

#include "ch_event.h"
#include "ch_IO.h"
#include "ch_IO_07_07_RS485.h"
#include "ch_IO_RS485.h"

//-----------------------------------------------------------------------------
void IO_07_07_RS485_Bus_PDO_Receive(unsigned char *ptr, unsigned char length, unsigned char card_nr, unsigned char component_nr)
{
  s_board_IO_on_off IO;
  if ((component_nr < IO_07_07_RS485_BUS) && (length >= 1) && (length <= 8))
  {
    IO.board_type = IO_07_07_ID;
    IO.board_nr   = card_nr;
    IO.IO_nr      = component_nr;
    switch (opt_io.IO_07_07[card_nr].RS485_bus.Option & 0x000F)
	{
      case RS485ModeTransparant: IO_RS485_PDO_Receive_Transparant(&value.IO_07_07[card_nr].RS485_bus, ptr, length, &IO, event_list.IO_07_07[card_nr].PDO_RS485_bus_received); break;
      case RS485ModeModbus: IO_RS485_PDO_Receive_Modbus(&value.IO_07_07[card_nr].RS485_bus, ptr, length, &IO, event_list.IO_07_07[card_nr].PDO_RS485_bus_received); break;
	}
  }
}

//-----------------------------------------------------------------------------
unsigned char IO_07_07_RS485_Bus_PDO_Transmit(unsigned char *ptr, unsigned char card_nr, unsigned char component_nr)
{
  if (component_nr < IO_07_07_RS485_BUS)
  {
    switch (opt_io.IO_07_07[card_nr].RS485_bus.Option & 0x000F)
	{
	  case RS485ModeTransparant: return IO_RS485_PDO_Transmit_Transparant(&value.IO_07_07[card_nr].RS485_bus, ptr);
	  case RS485ModeModbus: return IO_RS485_PDO_Transmit_Modbus(&value.IO_07_07[card_nr].RS485_bus, ptr);
	}
  }
  return (0);
}

//-----------------------------------------------------------------------------
void IO_07_07_RS485_Bus_PDO_Transmit_OK(unsigned char card_nr, unsigned char component_nr)
{
  if ((component_nr < IO_07_07_RS485_BUS) && (opt_io.IO_07_07[card_nr].RS485_bus.Option & 0x000F))
  {
    if (value.IO_07_07[card_nr].RS485_bus.TxInBuffer == 0)
	  value.IO_07_07[card_nr].RS485_bus.ctrl &= 0xFEFF;

    // PDO event
    if (event_list.IO_07_07[card_nr].PDO_RS485_bus_transmitted != NULL)
	{
	  s_board_IO_on_off IO;

	  IO.board_type = IO_07_07_ID;
	  IO.board_nr   = card_nr;
	  IO.IO_nr      = component_nr;
	  event_list.IO_07_07[card_nr].PDO_RS485_bus_transmitted(&IO);
	}
  }
}

unsigned int IO_07_07_RS485_Bus_SDO_Master_Transmit(u_SDO_data *ptr, unsigned char card_nr, unsigned char component_nr)
{
  if (component_nr < IO_07_07_RS485_BUS) 
  {
    ptr->RS485_bus.Command      = val_hr_alg.IO_07_07[card_nr].RS485_bus.Command;
	ptr->RS485_bus.Option       = opt_io.IO_07_07[card_nr].RS485_bus.Option;
	ptr->RS485_bus.CommSettings = (opt_io.IO_07_07[card_nr].RS485_bus.BaudRate & 0x000F) | ((opt_io.IO_07_07[card_nr].RS485_bus.Parity & 0x03) << 4);
	ptr->RS485_bus.Difference   = opt_io.IO_07_07[card_nr].RS485_bus.Difference;
	ptr->RS485_bus.EndOfFrame   = opt_io.IO_07_07[card_nr].RS485_bus.EndOfFrame;
	ptr->RS485_bus.IntervalTime = opt_io.IO_07_07[card_nr].RS485_bus.IntervalTime;
	ptr->RS485_bus.IdleTime     = opt_io.IO_07_07[card_nr].RS485_bus.IdleTime;
	ptr->RS485_bus.InhibitTime  = opt_io.IO_07_07[card_nr].RS485_bus.InhibitTime;
	ptr->RS485_bus.ClassNr      = opt_io.IO_07_07[card_nr].RS485_bus.ClassNr;
	return (sizeof(s_RS485_bus));
  }
  return (0);
}

void IO_07_07_RS485_Bus_SDO_Master_Transmit_OK(unsigned char card_nr, unsigned char component_nr)
{
  if (component_nr < IO_07_07_RS485_BUS) 
  {
	value.IO_07_07[card_nr].RS485_bus.ctrl &= 0xFFFB;
  }
}

void IO_07_07_RS485_Bus_SDO_Master_Receive(u_SDO_data *ptr, unsigned char card_nr, unsigned char component_nr)
// wordt momenteel niet gebruikt
{
  ptr; // deze regel is er alleen om een waarschuwing te voorkomen
  if (component_nr < IO_07_07_RS485_BUS) 
  {
    value.IO_07_07[card_nr].RS485_bus.ctrl &= 0xFFF7;
  }
}

void IO_07_07_RS485_Bus_Init(unsigned char card_nr)
// wordt aan geroepen als can opnieuw geinitialiseerd wordt
{
unsigned char comp_nr;

  for (comp_nr = 0; comp_nr < IO_07_07_RS485_BUS; comp_nr++)
  {
    value.IO_07_07[card_nr].RS485_bus.ctrl = 0x0000;
    value.IO_07_07[card_nr].RS485_bus.TxPutIndex = value.IO_07_07[card_nr].RS485_bus.TxGetIndex = value.IO_07_07[card_nr].RS485_bus.TxInBuffer = 0;
    value.IO_07_07[card_nr].RS485_bus.RxPutIndex = value.IO_07_07[card_nr].RS485_bus.RxGetIndex = value.IO_07_07[card_nr].RS485_bus.RxInBuffer = 0;
  }
}

