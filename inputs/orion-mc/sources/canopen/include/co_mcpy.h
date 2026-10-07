/*
 * comcpy - macros for memcpy
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
 * Revision 1.0  2008-03-07 17:05:37+01  driet
 * Initial revision
 *
 * Revision 2.4  2002/05/21 14:55:14  boe
 * copyright changed
 *
 * Revision 2.3  2001/03/28 13:08:30  boe
 * Prototype definition changed
 *
 * Revision 2.2  2001/02/26 14:59:51  boe
 * co_util.h included
 *
 * Revision 2.1  2001/01/26 12:37:51  boe
 * defines for memcpy
 *
 *
 *
 *------------------------------------------------------------------
 */

/*
DESCRIPTION

The file contains definitions for memcpy routines

*/

#ifndef __CO_MCPY_H
# define __CO_MCPY_H

# include <co_def.h>		/* include canopen definition */
# include <co_type.h>
# include <co_util.h>

/*
	memcpy for little,big endian machines real 16bit cpus
*/
/* default memcpy */
#ifndef CO_MEMCPY
# define CO_MEMCPY(dest, src, size)	memcpy(dest, src, (size_t)(size))
#endif

/* memcpy for numeric values */
#ifndef CO_NUM_MEMCPY
# if defined(CONFIG_16BIT_CPU)
#   define CO_NUM_MEMCPY(dest, src, size, num)			\
	if (num) 						\
		memcpy(dest, src, (size_t)((size + 1) >> 1));	\
	else							\
		memcpy(dest, src, (size_t)(size));
# else /* CONFIG_16BIT_CPU */
#  define CO_NUM_MEMCPY(dest, src, size, num)  memcpy(dest, src, (size_t)(size))
# endif /* else CONFIG_16BIT_CPU */
#endif /* ifndef CO_NUM_MEMCPY */

/* memcpy for data packing (can-buf into internal variable) */
#ifndef CO_PACK_MEMCPY
# if defined(CONFIG_16BIT_CPU)
#   define CO_PACK_MEMCPY	pack_memcpy
# elif defined(CONFIG_BIG_ENDIAN)
#   define CO_PACK_MEMCPY	CO_UNPACK_MEMCPY
# else /* CONFIG_BIG_ENDIAN */
#   define CO_PACK_MEMCPY(dest, src, size, num)	memcpy(dest, src,(size_t)(size))
# endif /* CONFIG_BIG_ENDIAN */
#endif /* ifndef CO_PACK_MEMCPY */

/* memcpy for data unpacking (internal variable into can-buf) */
#ifndef CO_UNPACK_MEMCPY
# if defined(CONFIG_16BIT_CPU)
#   define CO_UNPACK_MEMCPY	unpack_memcpy
void unpack_memory(UNSIGNED8 *, UNSIGNED8 *, UNSIGNED32, UNSIGNED8);
# elif defined(CONFIG_BIG_ENDIAN)
#   define CO_UNPACK_MEMCPY(dest, src, size, num)	{	\
	if (num)  {					\
	    LOOPCNT_U8	cnt = (UNSIGNED8)(size) - 1;	\
	    do  {					\
		(dest)[cnt] = (src)[(size) - 1 - cnt];	\
	    } while (cnt--);				\
	} else {					\
	    memcpy(dest, src, (size_t)(size));		\
	}						\
    }
# else /* CONFIG_BIG_ENDIAN */
#   define CO_UNPACK_MEMCPY(dest, src, size, num)  memcpy(dest, src, (size_t)(size))
# endif /* else CONFIG_BIG_ENDIAN */
#endif /* ifndef CO_UNPACK_MEMCPY */

/* memmove for numeric values */
#ifndef CO_NUM_MEMMOVE
# if defined(CONFIG_16BIT_CPU)
#   define CO_NUM_MEMMOVE(dest, src, size, num)		\
	if (num) 					\
		memmove(dest, src, (size + 1) >> 1);	\
	else						\
		memmove(dest, src, size);
# else /* CONFIG_16BIT_CPU */
#  define CO_NUM_MEMMOVE(dest, src, size, num)	memmove(dest, src, size)
# endif /* else CONFIG_16BIT_CPU */
#endif /* ifndef CO_NUM_MEMMOVE */


#endif		/*  __CO_MCPY_H */

/* end of source */

