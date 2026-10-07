/*
 * sync - defines for sync usage
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
 * Revision 1.0  2008-03-07 17:05:55+01  driet
 * Initial revision
 *
 * Revision 2.6  2002/12/11 10:10:22  boe
 * add further sync state defines
 *
 * Revision 2.5  2002/11/18 10:47:33  boe
 * add syncflag defines
 * add co_ to all global library variables
 * add/change prototypes
 *
 * Revision 2.4  2002/05/21 14:23:34  boe
 * cleanup new timer usage
 *
 * Revision 2.3  2002/03/26 08:14:43  boe
 * add NEW_TIMER functionality over defines
 *
 * Revision 2.2  2001/03/14 16:30:08  ro
 * macro for Sync COB-ID access added
 *
 * Revision 2.1  2001/01/26 11:24:24  boe
 * defines for sync
 *
 *
 *
 *------------------------------------------------------------------
 */

/*
DESCRIPTION

The file contains definitions of structures and data types for sync

*/

#ifndef __SYNC_H
# define __SYNC_H

# include <co_stru.h>
# include <co_sync.h>

# include "timer.h"


/* defines for sync state */

#define CO_SYNC_STATE_INIT	1	/* sync is initialized */
#define CO_SYNC_STATE_PRODUCER	2	/* sync producer */
#define CO_SYNC_STATE_ENABLED	4	/* sync producer is enabled,not active*/
#define CO_SYNC_STATE_ACTIVE	8	/* sync producer is active */


/* structure of a SYNC */

struct SYNC_OBJ {
	TIMER_EVENT_T	timer;		/* tdimer structure */
	COB_T		*pCobId;	/* COB structure */
	UNSIGNED8	state;		/* sync state */
};

typedef struct SYNC_OBJ SYNC_T;


/* external data declarations */

extern SYNC_T	*co_pSync ;      /* pointer to SYNC object */
extern UNSIGNED32 coInternSyncPeriod ;  /* internal time for sync */
extern UNSIGNED32 coLastSyncTime ;  /* last sync time */

/* function prototypes */

void 	writeSyncReq(void);
RET_T	setSyncCobId(UNSIGNED32 *pCobId );
RET_T	setSyncTimePara(UNSIGNED32 *pTimeVal );

#endif		/*  __SYNC_H */

/* end of source */

