/*
 * co_def - defines constants and enumerations
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
 * Revision 1.0  2008-03-07 17:05:32+01  driet
 * Initial revision
 *
 * Revision 2.5  2003/03/31 13:06:36  boe
 * add empty define for CO_WATCH_DOG
 *
 * Revision 2.4  2002/05/21 14:53:39  boe
 * copyright changed
 *
 * Revision 2.3  2001/04/05 09:03:57  boe
 * test for alignment added
 *
 * Revision 2.2  2001/03/14 17:17:47  ro
 * some lines formatted
 *
 * Revision 2.1  2001/01/26 12:32:24  boe
 * defines for canopen
 *
 *
 *
 */

/*
DESCRIPTION

This file contains constants and enumerations for the CANopen library.

*/

#ifndef __CO_DEF_H
# define __CO_DEF_H

#include <assert.h>		/* include assert definition */
#include <co_type.h>		/* include data type definition */


/* macros for single/multiple line substitution */
# ifdef CONFIG_MULT_LINES
#  define CO_COMMA_LINE_PARA_DECL  ,UNSIGNED8
#  define CO_LINE_PARA_DECL        UNSIGNED8
#  define CO_COMMA_LINE_PARA       ,canLine
#  define CO_LINE_PARA             canLine
#  define CO_LINE_PARA_ARRAY_DEF   [CONFIG_MULT_LINES]
#  define CO_LINE_PARA_ARRAY_INDEX [canLine]
# else
#  define CO_COMMA_LINE_PARA_DECL
#  define CO_LINE_PARA_DECL		void
#  define CO_COMMA_LINE_PARA
#  define CO_LINE_PARA
#  define CO_LINE_PARA_ARRAY_DEF
#  define CO_LINE_PARA_ARRAY_INDEX
# endif


/* Alignment should be set at makefile or cal_conf.h */
# if !defined(CONFIG_ALIGNMENT)
#  error No Alignmet set - Please define it at the Makefile or in cal_conf.h
# endif

/* if segmented SDO transfer is not allowed,
   no domain (program) up- and download is possible  */
# if !defined(CONFIG_SEG_SDO) && defined(CONFIG_DOMAIN_UPDNLD)
#  error Not possible configuration: CONFIG_DOMAIN_UPDNLD is set and CONFIG_SEG_SDO is not set
# endif

/* Heartbeat or Nodeguarding is mandatory */
# if !(defined(CONFIG_NODE_GUARDING) || defined(CONFIG_HEARTBEAT_PRODUCER))
#  error Not possible configuration: NODEGUARDING or HEARTBEAT is required
# endif


/* for compatibility */
# if defined(CONFIG_EMCY_SERVER)
#  define CONFIG_EMCY_PRODUCER
# endif

# if defined(CONFIG_EMCY_CLIENT)
#  define CONFIG_EMCY_CONSUMER
# endif

# if defined(CONFIG_PDO_SERVER)
#  define CONFIG_PDO_PRODUCER
# endif

# if defined(CONFIG_PDO_CLIENT)
#  define CONFIG_PDO_CONSUMER
# endif

/* define security mechanism, if not defined in cal_conf.h */

/* BC 4.5 32bit compiler has problem with empty parameters within macros
   at WIN32 console applications */

# if defined (__WIN32__) && defined(__BORLANDC__)
#  if __BORLANDC__ > 0x400 && __BORLANDC__ < 0x500
#    ifndef CONFIG_MULT_LINES
#	define CO_MACRO_DEF_EXCEPTION 1
#    endif
#  else
#   undef CO_MACRO_DEF_EXCEPTION
#  endif
# endif

# ifdef CO_MACRO_DEF_EXCEPTION

#  ifndef CO_COM_PART_ALLOC
#   define CO_COM_PART_ALLOC()
#  endif

#  ifndef CO_COM_PART_RELEASE
#   define CO_COM_PART_RELEASE()
#  endif

#  ifndef CO_APPL_PART_ALLOC
#   define CO_APPL_PART_ALLOC()
#  endif

#  ifndef CO_APPL_PART_RELEASE
#   define CO_APPL_PART_RELEASE()
#  endif

#  ifndef CO_NEW_RX_MSG
#   define CO_NEW_RX_MSG()
#  endif

# else /* CO_MACRO_DEF_EXCEPTION */

#  ifndef CO_COM_PART_ALLOC
#   define CO_COM_PART_ALLOC(CO_LINE_PARA)
#  endif

#  ifndef CO_COM_PART_RELEASE
#   define CO_COM_PART_RELEASE(CO_LINE_PARA)
#  endif

#  ifndef CO_APPL_PART_ALLOC
#   define CO_APPL_PART_ALLOC(CO_LINE_PARA)
#  endif

#  ifndef CO_APPL_PART_RELEASE
#   define CO_APPL_PART_RELEASE(CO_LINE_PARA)
#  endif

#  ifndef CO_NEW_RX_MSG
#   define CO_NEW_RX_MSG(CO_LINE_PARA)
#  endif

# endif /* CO_MACRO_DEF_EXCEPTION */

/* watchdog call */
# ifndef CO_WATCH_DOG
#  define CO_WATCH_DOG
# endif

# ifdef CONFIG_MASTER_PLUS
/* prevent warnings */
#  undef  CONFIG_MASTER
#  undef  CONFIG_SLAVE
/* set both configurations */
#  define CONFIG_MASTER
#  define CONFIG_SLAVE
# endif

# ifdef CONFIG_SLAVE_PLUS
/* prevent warnings if already exist */
#  ifndef  CONFIG_SLAVE
   /* set slave configurations */
#   define CONFIG_SLAVE
#  endif
# endif

# define CO_RTR_REQ	0x10	/* RTR sign at cantelegram length
				 * here use the bitdefinition from sja1000 */


#endif    /* __CO_DEF_H */

/*______________________________________________________________________EOF_*/
