/*
 * co_nmt - public defines for nmt usage
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
 * Revision 1.0  2008-03-07 17:05:38+01  driet
 * Initial revision
 *
 * Revision 2.6  2002/11/18 10:56:29  boe
 * add defines for save/reload parameter range
 * change function prototypes for resetComm and ResetAppl
 *
 * Revision 2.5  2002/05/21 14:55:34  boe
 * copyright changed
 *
 * Revision 2.4  2002/04/05 13:24:27  boe
 * add new function setNodePreop
 * return value for newStateInd() changed to BOOL_T
 *
 * Revision 2.3  2001/05/10 12:45:25  boe
 * prototype for getNodeState added
 *
 * Revision 2.2  2001/03/28 13:02:45  boe
 * indication function added
 *
 * Revision 2.1  2001/01/26 12:38:40  boe
 * defines for nmt services
 *
 *
 *
 *------------------------------------------------------------------
 */

/*
DESCRIPTION

The file contains definitions of structures and data types for

*/

#ifndef __CO_NMT_H
# define __CO_NMT_H

/* # include <co_stru.h> */
# include <co_def.h>		/* include canopen definition */


#define MEM_SEG_ALL_PARAMETERS		1
#define MEM_SEG_COM_PARAMETERS		2
#define MEM_SEG_APPL_PARAMETERS		3

/* external data declarations */

/* function prototypes */

#if defined(CONFIG_MASTER) || defined(CONFIG_SLAVE_PLUS)
RET_T		createNodeReq(BOOL_T, BOOL_T, BOOL_T );
#else /*  defined(CONFIG_MASTER) || defined(CONFIG_SLAVE_PLUS) */
RET_T		createNodeReq(BOOL_T, BOOL_T );
#endif /*  defined(CONFIG_MASTER) || defined(CONFIG_SLAVE_PLUS) */
RET_T		deleteNodeReq(void);
NODE_STATE_T	getNodeState(void);
UNSIGNED8 	sGuardErrorInd(ERROR_SPEC_T );
RET_T		setNodePREOP(void);


BOOL_T		newStateInd(NODE_STATE_T );
void 		resetCommInd(void);
void 		resetApplInd(void);


#endif		/*  __CO_NMT_H */

/* end of source */

