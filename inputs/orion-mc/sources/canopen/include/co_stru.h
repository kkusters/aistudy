/*
 * co_stru - defines common structures for CANopen
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
 * Revision 1.0  2008-03-07 17:05:43+01  driet
 * Initial revision
 *
 * Revision 2.5  2003/03/31 13:08:10  boe
 * don't add member pNext for COB_T is CONFIG_COB_ARRAY is set
 *
 * Revision 2.4  2002/05/21 14:56:43  boe
 * copyright changed
 *
 * Revision 2.3  2002/04/05 13:28:45  boe
 * add receive time to CAN msg structure for SRDO consumer
 *
 * Revision 2.2  2001/03/14 17:28:34  ro
 * Redundancy Support added
 *
 * Revision 2.1  2001/01/26 12:42:19  boe
 * structure defines
 *
 *
 *
 *------------------------------------------------------------------
 */

/*
DESCRIPTION

The file contains definitions of structures and complex data types
for the CANopen library
*/

#ifndef __CO_STRU_H
# define __CO_STRU_H

# include <co_def.h>		/* include canopen definition */
# include <co_type.h>


/* CAN buffer structures */
struct CAN_MSG
{
#ifdef CONFIG_SRDO_CONSUMER
    UNSIGNED16 recTime;			/* receive time */
#endif /* CONFIG_SRDO_CONSUMER */
    UNSIGNED16 wCOB_ID;			/* COB Id */
    UNSIGNED8  pData[8];		/* data */
    UNSIGNED8  length;			/* if bit CO_RTR_REQ is set -> RTR */
};

typedef struct 	CAN_MSG		CAN_MSG_T;


/* struct COB */
struct CO_COB
{
#ifdef CONFIG_COB_ARRAY
#else /* CONFIG_COB_ARRAY */
struct	CO_COB	 	 *pNext;
#endif /* CONFIG_COB_ARRAY */
	UNSIGNED16	 wID;
	UNSIGNED8	 bLength;
	UNSIGNED8	 bChannel;
	COB_KIND_T       eType;
#ifdef CONFIG_REDUNDANCY_SUPPORT
	struct CO_COB	*pNextLine;
#endif /* CONFIG_REDUNDANCY_SUPPORT */
};

typedef struct	CO_COB	COB_T;


/* Identity object DS 301 V 4.0 */
typedef struct {
	UNSIGNED8	numOfEntries;          /* number of entries in record */
	UNSIGNED32	vendorId;
	UNSIGNED32	productCode;
	UNSIGNED32	revisionNumber;
	UNSIGNED32	serialNumber;
} IDENTITY_T;


#endif		/*  __CO_STRU_H */

/* end of source */

