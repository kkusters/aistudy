/*
 * nmt_s - defines for slave nmt services
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
 * Revision 1.0  2008-03-07 17:05:51+01  driet
 * Initial revision
 *
 * Revision 2.3  2002/11/18 10:31:42  boe
 * add co_ to all global library variables
 * change functionnames for resetCommInd to resetCommMsg
 *
 * Revision 2.2  2002/05/21 13:57:10  boe
 * change copyright
 *
 * Revision 2.1  2001/01/26 11:01:40  boe
 * functions for slave nmt services
 *
 *
 *
 *------------------------------------------------------------------
 */

/*
DESCRIPTION

The file contains definitions of structures and complex data types
for nmt slave services

*/

#ifndef __NMT_S_H
# define __NMT_S_H


/* Parameter for resetObjDir */

/* external variable declarations */
extern UNSIGNED8 	coDefParaResetFlag ;



/* function prototypes */

void 	resetCommMsg(void);
void 	resetNodeMsg(void);



#endif /* __NMT_S_H */

/* end of source */

