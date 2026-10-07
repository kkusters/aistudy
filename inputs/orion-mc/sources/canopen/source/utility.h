/*
 * utility - defines for utility
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
 * Revision 1.0  2008-03-07 17:05:57+01  driet
 * Initial revision
 *
 * Revision 2.4  2002/11/18 10:44:41  boe
 * add special prototypes for 16 bit CPUs
 * remove prototyp for flagIdentification
 *
 * Revision 2.3  2002/05/21 13:51:35  boe
 * change copyright
 *
 * Revision 2.2  2001/03/14 16:40:52  ro
 * Redundancy Support functionality:
 *     special prototype for flagIdentification()
 *
 * Revision 2.1  2001/01/26 12:16:36  boe
 * defines for utilities
 *
 *
 *
 *------------------------------------------------------------------
 */

/*
DESCRIPTION

The file contains definitions of structures and data types for

*/

#ifndef __UTILITY_H
# define __UTILITY_H

# include <co_util.h>


/* external data declarations */

/* function prototypes */

#ifdef CONFIG_16BIT_CPU
UNSIGNED8 *unpack_oddmemcpy(UNSIGNED8 *dest, UNSIGNED8 *src, UNSIGNED32 size, BOOL_T *odd);
UNSIGNED8 *pack_oddmemcpy(UNSIGNED8 *dest, UNSIGNED8 *src, UNSIGNED32 size, BOOL_T *odd);
#endif /* CONFIG_16BIT_CPU */

#endif		/*  __UTILITY_H */

/* end of source */

