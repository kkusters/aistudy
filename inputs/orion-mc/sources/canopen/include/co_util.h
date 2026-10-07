/*
 * utility - public defines for utility
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
 * Revision 1.0  2008-06-06 15:21:40+02  driet
 * Initial revision
 *
 * Revision 2.4  2002/11/18 11:11:31  boe
 * add/change prototypes for 16bit CPUs
 *
 * Revision 2.3  2002/05/21 14:59:43  boe
 * add prototype for crc8 function
 *
 * Revision 2.2  2001/02/26 15:02:57  boe
 * co_type.h included
 *
 * Revision 2.1  2001/01/26 12:43:49  boe
 * utilities prototypen
 *
 *
 *
 *------------------------------------------------------------------
 */

/*
DESCRIPTION

The file contains definitions of structures and data types for utilities
*/

#ifndef __CO_UTIL_H
# define __CO_UTIL_H

# include <co_type.h>


/* external data declarations */

/* function prototypes */

void		coWait(UNSIGNED32 waitingTime);
UNSIGNED8	crc8Calc(UNSIGNED8 *, UNSIGNED8, UNSIGNED32);

#ifdef CONFIG_16BIT_CPU
void		pack_memcpy(UNSIGNED8 *dest, UNSIGNED8 *src, UNSIGNED32 size,
			UNSIGNED8);
void		unpack_memcpy(UNSIGNED8 *dest, UNSIGNED8 *src, UNSIGNED32 size,
			UNSIGNED8);
UNSIGNED16	crc16Calc(UNSIGNED8 *, UNSIGNED16, UNSIGNED32, BOOL_T);
#else /* CONFIG_16BIT_CPU */
UNSIGNED16	crc16Calc(UNSIGNED8 *, UNSIGNED16, UNSIGNED32);
#endif /* CONFIG_16BIT_CPU */


#endif		/*  __CO_UTIL_H */

/* end of source */
