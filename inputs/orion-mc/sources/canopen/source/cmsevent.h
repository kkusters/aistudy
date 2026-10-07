/*
 * cmsevent - defines for event services
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
 * Revision 1.0  2008-03-07 17:05:29+01  driet
 * Initial revision
 *
 * Revision 2.8  2003/06/13 13:20:45  boe
 * define timer-data for CMS_EVENT_T only if event timer is used
 *
 * Revision 2.7  2003/03/31 12:27:28  boe
 * prototype for function getEvAddr removed
 * CMS_EVENT structure optimized if SYNC not set
 *
 * Revision 2.6  2002/11/18 10:21:18  boe
 * remove event timer entries from CMS_EVENT structure
 * change prototypr for CMS_DefEvent_req
 *
 * Revision 2.5  2002/05/21 14:02:45  boe
 * cleanup new timer usage
 *
 * Revision 2.4  2002/05/21 12:45:04  boe
 * change copyright
 *
 * Revision 2.3  2002/03/26 08:07:19  boe
 * add NEW_TIMER functionality over defines
 *
 * Revision 2.2  2001/05/10 12:29:12  boe
 * structure for data mapping changed
 *
 * Revision 2.1  2001/01/26 10:42:56  boe
 * defines for events
 *
 *
 *
 *------------------------------------------------------------------
 */

/*
DESCRIPTION

The file contains definitions of structures and complex data types
for event services

*/

#ifndef __EVENT_H
# define __EVENT_H


#include <co_stru.h>
#include "timer.h"


/* mapping object structure */
struct PDO_MAP {
	void		*pAddress;	/* pointer to data */
	struct PDO_MAP	*pNext;
	BASIC_DATA_T	eBasicType;	/* basic data type */
	UNSIGNED8	bBitSize;	/* length in bits */
};

typedef struct PDO_MAP PDO_MAP_T;


/* structure of a CMS event */
struct CMS_EVENT {
# ifdef CONFIG_PDO_EVENTTIMER
	TIMER_EVENT_T	timer;		/* timer event structure */
# endif /* CONFIG_PDO_EVENTTIMER */
	INHIBIT_EVENT_T	inhibit;	/* inhibit structure */
	struct CMS_EVENT *pNext;	/* pointer to next event */
	PDO_MAP_T 	*pMapEntries;	/* array for adresses and length of
					   mapped objects */
	COB_T		*pCOB;		/* COB for Request/Response */
	UNSIGNED16	pdoNr;		/* number of PDO 1..512 */
	UNSIGNED16  	wInhibitTime;	/* inhibit time, unit: 100us */
	UNSIGNED8	transType;	/* transmissiontype object 20,2 */
# if defined(CONFIG_SYNC_PRODUCER) || defined(CONFIG_SYNC_CONSUMER)
	UNSIGNED8	curCount;	/* is to decrement with every SYNC */
	UNSIGNED8	shadowData[8];	/* Shadow CAN Data Buffer */
# endif /* defined(CONFIG_SYNC_PRODUCER) || defined(CONFIG_SYNC_CONSUMER) */
	UNSIGNED8   	flags;		/* PDO-flags, PDO disabled, RTR, */
# if defined(CONFIG_MPDO_DEST) || defined(CONFIG_MPDO_SRC)
	UNSIGNED8	mpdoFlags;	/* flags for mpdo modes */
# endif
};

typedef struct CMS_EVENT CMS_EVENT_T;


/* external variable declarations */



/* function prototypes */

CMS_EVENT_T	*findEvAddr(CMS_EVENT_T *, UNSIGNED16);
RET_T		CMS_DefEvent_req(CMS_EVENT_T **, CMS_EVENT_T **, COB_KIND_T
			);
RET_T		CMS_TransEvent_req(CMS_EVENT_T *, PDO_MAP_T *
			);



#endif /* __EVENT_ */

/* end of source */

