/*
 * co_lme - defines for lme usage
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
 * Revision 1.0  2008-03-07 17:05:36+01  driet
 * Initial revision
 *
 * Revision 2.4  2002/05/21 14:55:06  boe
 * copyright changed
 *
 * Revision 2.3  2001/04/12 14:08:15  boe
 * define prototyps for single line always by void
 *
 * Revision 2.2  2001/04/05 12:22:20  boe
 * Prototyp declaration changed
 *
 * Revision 2.1  2001/01/26 12:37:05  boe
 * defines for layer management
 *
 *
 *
 *------------------------------------------------------------------
 */

/*
DESCRIPTION

The file contains definitions of structures and data types for

*/

#ifndef __CO_LME_H
# define __CO_LME_H

#include <co_def.h>		/* include canopen definition */


/* external data declarations */


/* function prototypes */

RET_T	initCANopen(void);
void 	leaveCANopen(void);


#endif		/*  __CO_LME_H */

/* end of source */

