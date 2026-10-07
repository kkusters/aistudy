/*
 * co_sync - public defines for sync usage
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
 * Revision 1.0  2008-03-07 17:05:44+01  driet
 * Initial revision
 *
 * Revision 2.4  2002/11/18 11:09:19  boe
 * add co_ to all global library variables
 * change prototype for syncCmd
 *
 * Revision 2.3  2002/05/21 14:56:43  boe
 * copyright changed
 *
 * Revision 2.2  2001/03/28 13:04:40  boe
 * indication function added
 *
 * Revision 2.1  2001/01/26 12:42:30  boe
 * defines for sync
 *
 *
 *
 *------------------------------------------------------------------
 */

/*
DESCRIPTION

The file contains definitions of structures and data types for
sync
*/

#ifndef __CO_SYNC_H
# define __CO_SYNC_H

/* # include <co_stru.h> */
# include <co_def.h>		/* include canopen definition */

#define SYNC_CONSUMER_BIT       0x80000000UL
#define SYNC_PRODUCER_BIT       0x40000000UL


/* external data declarations */

# ifdef CONFIG_REDUNDANCY_SUPPORT
#  define CO_COB_ID_SYNC    co_cob_id_sync
   extern UNSIGNED32 co_cob_id_sync;			    /* COB-ID sync */
# else /* CONFIG_REDUNDANCY_SUPPORT */
#  define CO_COB_ID_SYNC    co_cob_id_sync 
   extern UNSIGNED32 co_cob_id_sync ;    /* COB-ID sync */
# endif /* CONFIG_REDUNDANCY_SUPPORT */


/* function prototypes */

RET_T	defineSync(CO_USER_T );
RET_T	startSyncReq(void);
RET_T	stopSyncReq(void);

void 	syncCommand(void);
void	updateSyncActualMsg(UNSIGNED16 pdoNr, UNSIGNED8 *pData
		);

#endif		/*  __CO_SYNC_H */

/* end of source */

