/*
 * co_setcp - defines for set communication parameter
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
 * Revision 1.0  2008-03-07 17:05:42+01  driet
 * Initial revision
 *
 * Revision 2.4  2002/11/18 10:59:20  boe
 * add prototype for setDefaultParameter
 *
 * Revision 2.3  2002/05/21 14:56:43  boe
 * copyright changed
 *
 * Revision 2.2  2001/03/14 17:27:43  ro
 * prototypes moved to other header
 *
 * Revision 2.1  2001/01/26 12:41:36  boe
 * defines for set internal communication parameter
 *
 *
 *
 *------------------------------------------------------------------
 */

/*
DESCRIPTION

The file contains definitions of structures and data types for
set communication parameter
*/

#ifndef __CO_SETCP_H
# define __CO_SETCP_H

/* # include <co_stru.h> */
# include <co_def.h>		/* include canopen definition */


/* external data declarations */

/* function prototypes */

RET_T	setCommPar(UNSIGNED16, UNSIGNED8 );
RET_T	resetObjDir(UNSIGNED8 );
void 	resetComStates(void);
void	setDefaultParameter(UNSIGNED8 );

#endif		/*  __CO_SETCP_H */

/* end of source */

