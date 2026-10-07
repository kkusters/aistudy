/*
 * access - defines for access to object dictionary
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
 * Revision 1.0  2008-03-07 08:56:16+01  driet
 * Initial revision
 *
 * Revision 2.5  2003/06/13 13:12:49  boe
 * define MAX_DATA_SIZE changed to CO_MAX_NUMDATA_SIZE
 * and default value is set to 8
 *
 * Revision 2.4  2002/05/21 12:44:04  boe
 * change copyright
 *
 * Revision 2.3  2001/03/29 14:22:51  boe
 * defines for VALUE_DESC moved to public header co_access.h
 *
 * Revision 2.2  2001/02/26 14:44:23  boe
 * MAX_DATA_SIZE defined for SDO transfers
 *
 * Revision 2.1  2001/01/26 10:42:00  boe
 * defines for od access
 *
 *
 *
 *------------------------------------------------------------------
 */

/*
DESCRIPTION

The file contains definitions of structures and complex data types
for access to object dictionary

*/

#ifndef __ACCESS_H
# define __ACCESS_H


#include <co_acces.h>		/* include public definition */



/* external variable declarations */
/* number of objects */
extern UNSIGNED16	maxObjDicElements ;
/* object dictionary */
extern OBJDIR_T objDir[];	/* od for single line */


/* defines max. Datasize for convert buffer */
#ifndef CO_MAX_NUMDATA_SIZE
# define CO_MAX_NUMDATA_SIZE 8
#else /* check size */
# if (CO_MAX_NUMDATA_SIZE < 5)
#  define CO_MAX_NUMDATA_SIZE 4
# endif
#endif


/* function prototypes */

OBJDIR_T 	*searchObj(UNSIGNED16 );
void   	    *getSubIndexAddr( OBJDIR_T *, UNSIGNED8);


#endif /* __ACCESS_H */

/* end of source */

