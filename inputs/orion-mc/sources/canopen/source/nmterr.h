/*
 * nmterr - defines for nmt error control usage
 *
 * Copyright (c) 2001-2002 port GmbH Halle/Saale
 *------------------------------------------------------------------
 * $Header$
 *
 *------------------------------------------------------------------
 *
 * modification history
 * --------------------
 * $Log$
 * Revision 1.0  2008-03-07 17:05:52+01  driet
 * Initial revision
 *
 * Revision 2.6  2002/11/18 10:32:56  boe
 * add co_ to all global library variables
 * rework flags definition
 * add new prototypes
 *
 * Revision 2.5  2002/05/21 14:18:23  boe
 * cleanup new timer usage
 *
 * Revision 2.4  2002/03/26 08:15:02  boe
 * add NEW_TIMER functionality over defines
 *
 * Revision 2.3  2001/05/10 14:03:45  boe
 * adaption for new ds307
 *
 * Revision 2.2  2001/02/26 14:54:01  boe
 * co_struct included
 * flying master functionality added
 *
 * Revision 2.1  2001/01/26 11:02:36  boe
 * defines for nmt error control
 *
 *
 *
 *------------------------------------------------------------------
 */

/*
DESCRIPTION

The file contains definitions of structures and data types for
 nmt error control usage

*/

#ifndef __NMTERROR_H
# define __NMTERROR_H

#include <co_stru.h>
#include "timer.h"


/* flags for mflags and flags at structure NODE_T */
/* flag             set     reset
 * GUARDFLAG_NG_LIFETIME    setLifeTime setLifeTime
 *                      setHeartBeatProducerTime
 *                      resetComState
 * GUARDFLAG_NG_RECEIVED    setGuardingPara startHeartBeatReq
 *              NMT_M_NodeGuardingMsg   NMT_M_NodeGuardingMsg   
 *                      resetComState
 * GUARDFLAG_NG_ACTIVE      setGuardingPara setGuardingPara 
 *              NMT_NodeGuardingMsg NMT_TimerPulse_ind
 *                      setLifeTime
 *                      setHeartBeatProducerTime
 *                      NMT_M_TimerPulse_ind
 *                      resetComState
 * GUARDFLAG_NG_POSSIBLE
 * GUARDFLAG_HB_ACTIVE
 * GUARDFLAG_HB_POSSIBLE
 *
 *
 * flags
 * Lifetime possible        GUARDFLAG_NG_POSSIBLE
 * Lifetime configured      GUARDFLAG_NG_POSSIBLE
 *              GUARDFLAG_NG_LIFETIME
 * Lifetime active      GUARDFLAG_NG_POSSIBLE
 *              GUARDFLAG_NG_LIFETIME
 *              GUARDFLAG_NG_ACTIVE
 * Heartbeat Prod possible  GUARDFLAG_HB_POSSIBLE
 * Heartbeat Prod active    GUARDFLAG_HB_POSSIBLE
 *              GUARDFLAG_HB_ACTIVE
 *
 * mflags
 *              GUARDFLAG_MASTER
 * Nodeguarding possible    GUARDFLAG_NG_POSSIBLE
 * Nodeguarding active      GUARDFLAG_NG_POSSIBLE
 *              GUARDFLAG_NG_ACTIVE
 * Heartbeat Cons possible  GUARDFLAG_HB_POSSIBLE
 * Heartbeat Cons active    GUARDFLAG_HB_POSSIBLE
 *              GUARDFLAG_HB_ACTIVE
 *
 */
#define GUARDFLAG_NG_RECEIVED ((UNSIGNED8)1)    /* Guarding Telegramm received */
#define GUARDFLAG_NG_ACTIVE   ((UNSIGNED8)2)    /* Guarding is active */
#define GUARDFLAG_NG_LIFETIME ((UNSIGNED8)4)    /* lifetime is enabled */
#define GUARDFLAG_NG_POSSIBLE ((UNSIGNED8)8)    /* Guarding possible */
#define GUARDFLAG_HB_ACTIVE   ((UNSIGNED8)0x10) /* Heartbeat active */
#define GUARDFLAG_HB_POSSIBLE ((UNSIGNED8)0x20) /* Heartbeat possible */

#define GUARDFLAG_MASTER      ((UNSIGNED8)0x80) /* node works as master */


/* external data declarations */

/* function prototypes */

void    NMT_NodeGuardingMsg (CAN_MSG_T *canMsg );
void    NMT_HB_TimerPulse (void);
void    NMT_TimerPulse (void);
RET_T   setLifeTime(UNSIGNED16 *lifeTime, UNSIGNED8 *faktor );
void    setHeartBeatProducerTime(UNSIGNED16 hbTime );


#endif      /*  __NMTERROR_H */

/* end of source */

