// CH_DS401.H

#ifndef _CH_DS401_H
#define _CH_DS401_H

#include "ch_define.h"

#include <co_type.h>

#ifdef CANopen

#define I_DS401_READ_STATE_8  0x6000
#define I_DS401_WRITE_STATE_8 0x6200
#define I_DS401_READ_AN_16	  0x6401
#define I_DS401_WRITE_AN_16	  0x6411

extern UNSIGNED8 AnaOutValid[33];
extern UNSIGNED8 DigOutValid[9];

extern UNSIGNED8 intTPDO[12];

void servePdos(void);

RET_T DS401_SetDigitalInput (UNSIGNED8 nr, UNSIGNED8  val);
RET_T DS401_GetDigitalOutput(UNSIGNED8 nr, UNSIGNED8 *val);

RET_T DS401_SetAnalogInput (UNSIGNED8 nr, INTEGER16  val);
RET_T DS401_GetAnalogOutput(UNSIGNED8 nr, INTEGER16 *val);

#endif // CANopen

#endif // CH_DS401_H