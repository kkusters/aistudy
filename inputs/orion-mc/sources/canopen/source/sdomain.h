/*
 * sdomain - defines for internal sdo usage
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
 * Revision 2.2  2002/05/21 13:58:06  boe
 * change copyright
 *
 * Revision 2.1  2001/01/26 11:22:59  boe
 * defines for sdo main
 *
 *
 *
 *------------------------------------------------------------------
 */

/*
DESCRIPTION

The file contains definitions of structures and data types for sdo usage

*/

#ifndef __SDOMAIN_H
# define __SDOMAIN_H


#include "sdo.h"


/* function prototypes */
void	sdoMsgReceived(CAN_MSG_T *canMsg );
void	printSdoState(char *fctName, SDO_T *pSdo);


#endif		/*  __SDOMAIN_H */

/* end of source */

