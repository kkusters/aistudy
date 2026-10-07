/*
 * nmterr_m - defines for heartbeat/nodeguarding as master usage
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
 * Revision 2.4  2002/11/18 10:34:04  boe
 * change functionname from NMT_M_TimerPulse_ind to NMT_M_TimerPulse
 *
 * Revision 2.3  2002/05/21 14:18:05  boe
 * cleanup new timer usage
 *
 * Revision 2.2  2002/03/26 08:15:02  boe
 * add NEW_TIMER functionality over defines
 *
 * Revision 2.1  2001/01/26 11:04:13  boe
 * defines for master nmt error mechanism
 *
 *
 *
 *------------------------------------------------------------------
 */

/*
DESCRIPTION

The file contains definitions of structures and data types for
heartbeat/nodeguarding as master
*/

#ifndef __NMTERR_M_H
# define __NMTERR_M_H

# include "timer.h"


/* external data declarations */

/* function prototypes */
void	NMT_M_TimerPulse    (TIMER_EVENT_T * );
void	NMT_M_NodeGuardingMsg   (CAN_MSG_T *canMsg );


#endif		/*  __NMTERR_M_H */

/* end of source */

