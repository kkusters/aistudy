/*
 * nmt_m - defines for nmt master services
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
 * Revision 1.0  2008-03-07 17:05:50+01  driet
 * Initial revision
 *
 * Revision 2.4  2002/11/18 10:30:22  boe
 * add co_ to all global library variables
 *
 * Revision 2.3  2002/05/21 13:56:48  boe
 * change copyright
 *
 * Revision 2.2  2001/02/26 14:52:18  boe
 * variable and prototype declaration changed
 *
 * Revision 2.1  2001/01/26 11:00:25  boe
 * defines for master nmt services
 *
 *
 *
 *------------------------------------------------------------------
 */

/*
DESCRIPTION

The file contains definitions of structures and complex data types
for nmt master services

*/

#ifndef __NMT_M_H
# define __NMT_M_H


#include "nmt.h"
#include <co_nmt_m.h>

/* structure of the network object */
typedef	struct	NODE		NETWORK_T;


/* external variable declarations */

extern NETWORK_T *co_pNetwork          ;


/* function prototypes */

extern NODE_T	*NMT_NodeExist(UNSIGNED8 bMod_ID );


#endif /* __NMT_M_H */

/* end of source */

