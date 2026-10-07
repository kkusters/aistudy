/*
 * co_usr - declarations for public functions at usr_301.c
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
 * Revision 1.1  2008-10-24 16:44:12+02  driet
 * <>
 *
 * Revision 1.0  2008-03-07 17:05:46+01  driet
 * Initial revision
 *
 * Revision 2.5  2003/07/30 08:05:02  boe
 * delete not necessary include co_time.h
 *
 * Revision 2.4  2002/05/21 14:57:47  boe
 * copyright changed
 *
 * Revision 2.3  2001/03/28 13:06:04  boe
 * indication function moved to the services files
 *
 * Revision 2.2  2001/03/14 17:29:16  ro
 * prototypes for Redundancy Support added
 *
 * Revision 2.1  2001/01/26 12:43:28  boe
 * prototypen user interface
 *
 *
 *
 *------------------------------------------------------------------
 */

/*
DESCRIPTION

The file contains definitions of structures and complex data types
for the public indication functions

*/

#ifndef __CO_USR
# define __CO_USR


#include <co_def.h>		/* include canopen definition */


/* external variable declarations */



/* function prototypes */

UNSIGNED8 getNodeId(void);
BOOL_T 	  canErrorInd(UNSIGNED8 );



#endif /* __CO_USR */

/* end of source */

