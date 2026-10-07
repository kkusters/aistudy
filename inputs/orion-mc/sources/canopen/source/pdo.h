/*
 * pdo - defines for pdo usage
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
 * Revision 1.0  2008-03-07 17:05:53+01  driet
 * Initial revision
 *
 * Revision 2.8  2003/06/16 13:19:13  boe
 * add line parameter for function setPdoTransType
 *
 * Revision 2.7  2003/03/31 12:38:55  boe
 * add multiline parameter for some function calls (pdo event timer functions)
 *
 * Revision 2.6  2002/12/11 08:00:57  boe
 * change function parameter for prototype of setPdoEventTime()
 *
 * Revision 2.5  2002/11/18 10:36:03  boe
 * add new prototypes
 * add co_ to all global library variables
 *
 * Revision 2.4  2002/05/21 14:19:34  boe
 * cleanup new timer usage
 *
 * Revision 2.3  2002/03/26 08:15:02  boe
 * add NEW_TIMER functionality over defines
 *
 * Revision 2.2  2001/02/26 14:55:46  boe
 * cmsevent.h local included
 *
 * Revision 2.1  2001/01/26 12:26:59  boe
 * defines for pdos
 *
 *
 *
 *------------------------------------------------------------------
 */

/*
DESCRIPTION

The file contains definitions of structures and data types for pdo usage

*/

#ifndef __PDO_H
# define __PDO_H

/* # include <co_stru.h> */
# include <co_pdo.h>
# include "cmsevent.h"
# include "timer.h"


typedef struct CMS_EVENT PDO_T;

/* defines for PDO flags */
#define PDOFLAG_DISABLED      ((UNSIGNED8)1)
#define PDOFLAG_SYNC          ((UNSIGNED8)2)
#define PDOFLAG_CYCLIC        ((UNSIGNED8)4)
#define PDOFLAG_SYNC_POSSIBLE ((UNSIGNED8)8)
#define PDOFLAG_RTR           ((UNSIGNED8)0x10)
#define PDOFLAG_TOTRANSMIT    ((UNSIGNED8)0x20)
#define PDOFLAG_TOUPDATE      ((UNSIGNED8)0x40)
#define PDOFLAG_OUTSTANDING   ((UNSIGNED8)0x80)


#define MAP_INDEX_SHIFT         16
#define MAP_INDEX_MASK          0x0000FFFFUL
#define MAP_SUBINDEX_SHIFT      8
#define MAP_SUBINDEX_MASK       0x000000FFUL
#define MAP_LENGTH_MASK     0x000000FFUL


#define ERRCODE_BAD_PDOPARA 0x8210


/* external data declarations */
extern PDO_T    *co_pFirstTrPdo ;
extern PDO_T    *co_pFirstRecPdo ;

/* function prototypes */

void    pdoMsgReceived(CAN_MSG_T *canMsg );
void    pdoRtrMsgReceived(CAN_MSG_T *canMsg );
RET_T   prepareTransPdo(PDO_T *, UNSIGNED8 *);
PDO_T   *pdoExist(UNSIGNED16 , UNSIGNED8 );
void    transSyncPdo(void);
void    updateSyncRpdo(void);
RET_T   updateSyncTpdo(void);
void    eventTransPdo(TIMER_EVENT_T *pTimer );
void    eventRecPdo(TIMER_EVENT_T *pTimer );
RET_T   checkMappingTable(UNSIGNED16 index );
RET_T   checkMappingEntry(UNSIGNED16 index, UNSIGNED32 newMapEntry );
RET_T   setPdoCobId(PDO_T *pPdo, UNSIGNED32 cobId);
RET_T   setPdoTransType(PDO_T *pPdo, UNSIGNED8 kind, UNSIGNED8 transType );
RET_T   setPdoInhibitTime(PDO_T *pPdo, UNSIGNED16 inhibitTime);
RET_T   setPdoEventTime(PDO_T *, UNSIGNED8, UNSIGNED16 eventTime );

#endif      /*  __PDO_H */

/* end of source */

