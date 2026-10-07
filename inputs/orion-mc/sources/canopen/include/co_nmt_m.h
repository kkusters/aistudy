/*
 * co_nmt_m - public defines for nmt master usage
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
 * Revision 1.0  2008-03-07 17:05:39+01  driet
 * Initial revision
 *
 * Revision 2.6  2003/07/22 07:27:13  boe
 * add prototype for getRemoteNodeState()
 *
 * Revision 2.5  2002/11/18 10:57:07  boe
 * change function prototypes for addRemotenOde and createRemoteNode
 *
 * Revision 2.4  2002/05/21 14:55:34  boe
 * copyright changed
 *
 * Revision 2.3  2001/08/22 08:21:49  boe
 * prototyp for new function changeRemoteNodeReq() added
 *
 * Revision 2.2  2001/03/28 13:03:00  boe
 * indication function added
 *
 * Revision 2.1  2001/01/26 12:39:04  boe
 * defines for nmt services master
 *
 *
 *
 *------------------------------------------------------------------
 */

/*
DESCRIPTION

The file contains definitions of structures and data types for
nmt master usage
*/

#ifndef __CO_NMT_M_H
# define __CO_NMT_M_H

/* # include <co_stru.h> */
# include <co_def.h>		/* include canopen definition */


/* external data declarations */

/* function prototypes */

RET_T	createNetworkReq (BOOL_T, BOOL_T );
void 	deleteNetworkReq (void);
RET_T	addRemoteNodeReq   (UNSIGNED8, UNSIGNED16, UNSIGNED8, BOOL_T
		);
RET_T	changeRemoteNodeReq(UNSIGNED8, UNSIGNED8, UNSIGNED16, UNSIGNED8
		);
RET_T	removeRemoteNodeReq(UNSIGNED8 );
RET_T	startRemoteNodeReq (UNSIGNED8 );
RET_T	stopRemoteNodeReq  (UNSIGNED8 );
RET_T 	enterPreOpStateReq(UNSIGNED8 );
RET_T 	resetNodeReq(UNSIGNED8 );
RET_T 	resetCommReq(UNSIGNED8 );
NODE_STATE_T getRemoteNodeState(UNSIGNED8  nodeNr );

void 	mGuardErrorInd(UNSIGNED8,ERROR_SPEC_T );

#endif		/*  __CO_NMT_M_H */

/* end of source */

