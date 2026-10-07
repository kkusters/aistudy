/*
 * co_splus - public defines for slave plus
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
 * Revision 1.0  2008-06-06 15:24:23+02  driet
 * Initial revision
 *
 * Revision 2.3  2002/05/21 14:56:43  boe
 * copyright changed
 *
 * Revision 2.2  2001/05/23 10:18:08  boe
 * declaration error for multiline corrected
 *
 * Revision 2.1  2001/03/28 13:07:31  boe
 * slave plus defines
 *
 *
 *
 *------------------------------------------------------------------
 */

/*
DESCRIPTION

The file contains definitions of structures and data types for
slaves with nmt master capabilities and heartbeat consumer properties
*/

#ifndef __CO_SLAVE_PLUS_H
# define __CO_SLAVE_PLUS_H

#include <co_def.h>		/* include canopen definition */

/* external data declarations */

extern BOOL_T nodeIsMaster ;

/* function prototypes */

void	changeSlaveNetOption(BOOL_T masterFlag );
RET_T	newRemoteStateReq(NODE_STATE_T newState );

#endif		/*  __CO_SLAVE_PLUS_H */

/*______________________________________________________________________EOF_*/
