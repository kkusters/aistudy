/*
 * co_hb - public defines for heart beat
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
 * Revision 1.0  2008-03-07 17:05:36+01  driet
 * Initial revision
 *
 * Revision 2.2  2002/05/21 14:54:57  boe
 * copyright changed
 *
 * Revision 2.1  2001/01/26 12:36:40  boe
 * defines for heartbeat
 *
 *
 *
 *------------------------------------------------------------------
 */

/*
DESCRIPTION

The file contains definitions of structures and data types for
 heart beat usage

*/

#ifndef __CO_HB_H
# define __CO_HB_H


#include <co_def.h>		/* include canopen definition */


/* external data declarations */

/* function prototypes */

RET_T	setHeartBeatTime(UNSIGNED8, UNSIGNED16 );
RET_T	startHeartBeatReq(UNSIGNED8 );
RET_T	stopHeartBeatReq(UNSIGNED8 );



#endif		/*  __CO_HB_H */

/* end of source */

