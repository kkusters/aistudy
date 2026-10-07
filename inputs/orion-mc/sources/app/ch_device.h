// CH_VENT.H

#ifndef _CH_VENT_H
#define _CH_VENT_H

#include <time.h>

#include "ct_data.h"
#include "ch_timer.h"

//==============================================================================
//------------------------ Device - Defines ------------------------------------
//==============================================================================
#define VENT_EMCY_BIT_1 ((unsigned char)0x01)
#define VENT_EMCY_BIT_2 ((unsigned char)0x02)
#define VENT_EMCY_BIT_3 ((unsigned char)0x04)
#define VENT_EMCY_BIT_4 ((unsigned char)0x08)
#define VENT_EMCY_BIT_5 ((unsigned char)0x10)

//==============================================================================
//------------------------ Device - Typedefs -----------------------------------
//==============================================================================
typedef struct
{
  unsigned char Number;
  unsigned char TimerPositionAlarm;
  TTimer Timer_1min;
  TTimer TimerAlarmDelay;
  void *Next;
  void *Prev;
} TDevice;

//==============================================================================
//------------------------ Device - Globals ------------------------------------
//==============================================================================
extern TDevice Device[MAX_DEVICE];

//==============================================================================
//------------------------ Device - Initialisation -----------------------------
//==============================================================================
void CreateDevice(TDevice *pDevice, int Number);
void InitDevice(TDevice *pDevice);

//==============================================================================
//------------------------ Device - Alarm --------------------------------------
//==============================================================================
//------------------------------------------------------------------------------
void ResetAlarmDevice(TDevice *pDevice);

//==============================================================================
//------------------------ Device - Control ------------------------------------
//==============================================================================
unsigned char DeviceGetEmcyBits(TDevice *pDevice);
unsigned char ControlAlarmDevice(TDevice *pDevice);

void SetTargetValueDevice(TDevice *pDevice, unsigned char Value, s_board_IO_on_off *IO, unsigned char IO_size);

int GetFlapSensorValue(TDevice *pDevice);

#endif
