// CH_EVENT.H

#ifndef _CH_EVENT_H
#define _CH_EVENT_H

#include <time.h>

#include "ct_data.h"

//=============================================================================
//                     Event list
//=============================================================================
typedef struct
{
  void (*PDO_received)(s_board_IO_on_off *IO);
  void (*SDO_received)(s_board_IO_on_off *IO);
} s_event_board_IO;

typedef struct
{
  void (*PDO_RS485_bus_transmitted)(s_board_IO_on_off *IO);
  void (*PDO_RS485_bus_received)(s_board_IO_on_off *IO);
} s_event_board_IO_05_07;

typedef struct
{
  void (*PDO_RS485_bus_transmitted)(s_board_IO_on_off *IO);
  void (*PDO_RS485_bus_received)(s_board_IO_on_off *IO);
} s_event_board_IO_07_07;

typedef struct
{
  s_event_board_IO IO_06_14[IO_06_14_MAX];
  s_event_board_IO IO_12_06[IO_12_06_MAX];
  s_event_board_IO IO_08_09[IO_08_09_MAX];
  s_event_board_IO IO_H2MC[IO_H2MC_MAX];
  s_event_board_IO IO_EKU[IO_EKU_MAX];
  s_event_board_IO IO_H1MC[IO_H1MC_MAX];
  s_event_board_IO_05_07 IO_05_07[IO_05_07_MAX];
  s_event_board_IO_07_07 IO_07_07[IO_07_07_MAX];
} s_event_list;

extern s_event_list event_list;

//=============================================================================
//                     Set Event
//=============================================================================
void SetPdoReceivedEvent(s_board_IO_on_off *IO, void (*PdoReceivedEvent)(s_board_IO_on_off *IO));
void SetPdoTransmittedEvent(s_board_IO_on_off *IO, void (*PdoTransmittedEvent)(s_board_IO_on_off *IO));

//=============================================================================
//                     Event initialisation
//=============================================================================
void Event_Init(void);

//=============================================================================
//                     Event handler
//=============================================================================
void Event_PDO_Received(s_board_IO_on_off *IO);
void Event_SDO_Received(s_board_IO_on_off *IO);

#endif
