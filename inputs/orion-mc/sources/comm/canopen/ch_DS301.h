// CH_DS301.H

#ifndef _CH_DS301_H
#define _CH_DS301_H

#include "ch_define.h"
#include <co_type.h>

#ifdef CANopen

RET_T CANopenEmcyReq(UNSIGNED16 errCode, UNSIGNED16 manu1Err, UNSIGNED16 manu2Err, UNSIGNED8  manu3Err);
RET_T CANopenEmcyErase(UNSIGNED16 errCode, UNSIGNED16 manu1Err, UNSIGNED16 manu2Err, UNSIGNED8  manu3Err);

void CANopenCreateEmcy(int al_code, int al_index, int al_value);
void CANopenClearEmcy(int al_code, int al_index);

#endif // CANopen

#endif // CH_D301_H