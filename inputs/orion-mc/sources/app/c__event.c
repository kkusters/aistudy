// C__EVENT.C  

#include "ch_define.h"

#include "ch_IO.h"
#include "ch_event.h"

//=============================================================================
//                     Event list
//=============================================================================
s_event_list event_list;

//=============================================================================
//                     Event initialisation
//=============================================================================
void Event_Init(void)
{
unsigned char *ptr;
int i;

  ptr = (unsigned char *)&event_list;
  for (i = 0; i < sizeof(event_list); i++)
    ptr[i] = 0;

  // Init H2MC events
  for (i = 0; i < IO_H2MC_MAX; i++)
  {
    event_list.IO_H2MC[i].PDO_received = Event_PDO_Received;
    event_list.IO_H2MC[i].SDO_received = Event_SDO_Received;
  }
  // Init H1MC events
  for (i = 0; i < IO_H1MC_MAX; i++)
  {
    event_list.IO_H1MC[i].PDO_received = Event_PDO_Received;
    event_list.IO_H1MC[i].SDO_received = Event_SDO_Received;
  }
  // Init EKU events
  for (i = 0; i < IO_EKU_MAX; i++)
  {
    event_list.IO_EKU[i].PDO_received = Event_PDO_Received;
    event_list.IO_EKU[i].SDO_received = Event_SDO_Received;
  }
}

//=============================================================================
//                     Set Event
//=============================================================================
void SetPdoReceivedEvent(s_board_IO_on_off *IO, void (*PdoReceivedEvent)(s_board_IO_on_off *IO))
{
  switch (IO->board_type)
  {
    case IO_05_07_ID:
	  event_list.IO_05_07[IO->board_nr].PDO_RS485_bus_received = PdoReceivedEvent;
	  break;
    case IO_07_07_ID:
	  event_list.IO_07_07[IO->board_nr].PDO_RS485_bus_received = PdoReceivedEvent;
	  break;
  }
}

void SetPdoTransmittedEvent(s_board_IO_on_off *IO, void (*PdoTransmittedEvent)(s_board_IO_on_off *IO))
{
  switch (IO->board_type)
  {
    case IO_05_07_ID:
	  event_list.IO_05_07[0].PDO_RS485_bus_transmitted = PdoTransmittedEvent;
	  break;
    case IO_07_07_ID:
	  event_list.IO_07_07[0].PDO_RS485_bus_transmitted = PdoTransmittedEvent;
	  break;
  }
}

//=============================================================================
//                     Event handler
//=============================================================================
void Event_PDO_Received(s_board_IO_on_off *IO)
{
  IO;
}

void Event_SDO_Received(s_board_IO_on_off *IO)
{
int i;

  for (i = 0; i < MAX_MOTOR; i++)
  {
    if ((opt_app.Motor[i].IO.board_type == IO->board_type) &&
	    (opt_app.Motor[i].IO.board_nr == IO->board_nr) &&
		(opt_app.Motor[i].IO.IO_nr == IO->IO_nr))
	{
	  Motor[i].CtrlMotorManagement &= (~MOTOR_MANAGEMENT_REQUEST_FLAG);
	  Motor[i].CtrlMotorManagement |= MOTOR_MANAGEMENT_SEND_PC_FLAG;
	  IO_Update_Motor_Control_MotorManagement(IO, &Motor[i].Management);
	  return;
	}
  }
}