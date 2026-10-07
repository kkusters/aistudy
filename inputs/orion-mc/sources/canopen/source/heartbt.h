/*
 * heartbt - defines for heartbeat services
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
 * Revision 1.0  2008-03-07 17:05:49+01  driet
 * Initial revision
 *
 * Revision 2.4  2003/02/28 16:06:29  boe
 * correct multiline parameter for some function calls
 *
 * Revision 2.3  2002/11/18 10:27:36  boe
 * add prototype for setHeartBeatConsumerTime
 *
 * Revision 2.2  2002/05/21 13:55:56  boe
 * change copyright
 *
 * Revision 2.1  2001/01/26 10:52:51  boe
 * define for heartbeat service
 *
 *
 *
 *------------------------------------------------------------------
 */

/*
DESCRIPTION

The file contains definitions of structures and complex data types
for heartbeat services

*/

#ifndef __HEARTBT_H
# define __HEARTBT_H

#include <co_def.h>
#include <co_hb.h>

/* external variable declarations */


/* function prototypes */

UNSIGNED8 findHeartBeatEntry(UNSIGNED8 bNodeId );
RET_T setHeartBeatTime(UNSIGNED8  nodeId, UNSIGNED16 );
RET_T setHeartBeatConsumerTime(UNSIGNED32 hbEntry, UNSIGNED8 subIndex ); 


#endif /* __HEARTBT_H */

/* end of source */

