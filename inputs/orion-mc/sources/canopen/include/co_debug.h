/****************************************************************************
 *
 * co_debug -	 Makros for library debugging
 *
 * Copyright (c) 2000-2002 port GmbH Halle
 *------------------------------------------------------------------
 * $Header$
 *
 *------------------------------------------------------------------
 *
 * modification history
 * --------------------
 * $Log$
 * Revision 1.0  2008-06-06 15:18:05+02  driet
 * Initial revision
 *
 * Revision 2.10  2002/05/21 14:58:09  boe
 * add debug for lss
 *
 * Revision 2.9  2001/10/15 12:44:31  ro
 * user define BDEBUG allow, if don't define CONFIG_CO_DEBUG
 *
 * Revision 2.8  2001/03/14 17:15:00  ro
 * comment formatted
 *
 * Revision 2.7  2000/10/19 09:40:11  ro
 * PRINTF corrected
 *
 * Revision 2.6  2000/10/13 11:45:14  ro
 * TARGET_FUJITSU_90540 added
 *
 * Revision 2.5  2000/10/05 11:14:04  boe
 * macros definition depends on TARGET
 *
 * Revision 2.4  2000/07/21 06:39:48  ro
 * BDEBUG wegen Problemen mit BorlandC geaendert
 *
 * Revision 2.3  2000/06/21 09:18:08  boe
 * change definition for BDEBUG operating system specific
 *
 * Revision 2.2  2000/06/13 06:36:07  boe
 * error messages at co_debug now
 *
 * Revision 2.1  2000/06/09 07:02:07  boe
 * debug functions for library
 *
 *
 *
 *------------------------------------------------------------------
 */

/*
DESCRIPTION

This file contains macros for debug the CANopen library.
Debug messages can be enabled/disabled at runtime
over the variable co_debug.
This variable is bitcoded.

*/

#ifndef __CO_DEBUG_H
# define __CO_DEBUG_H

# ifdef CONFIG_RCS_IDENT
static char _rcs_debug_h[] = "$Id$";
# endif

extern int co_debug;


# ifndef _STDIO_H_
#  include <stdio.h>
# endif

# ifndef PRINTF
#  ifdef TARGET_LINUX
#   define PRINTF fprintf(stderr,
#  else /* TARGET_LINUX */
#   define PRINTF printf
#  endif /* else TARGET_LINUX */
# endif /* ifndef PRINTF */


# ifdef CONFIG_CO_DEBUG
#  ifndef BDEBUG
#   define BDEBUG		debugprint
#  endif /* ifndef BDEBUG */
# else /* CONFIG_CO_DEBUG */
#  ifndef BDEBUG
#   if defined(TARGET_LINUX) || defined(TARGET_APC)
#     define BDEBUG		#undef
#   elif defined(TARGET_FUJITSU_90540)
#     define BDEBUG		//
#   else /* TARGET_LINUX */
#     define BDEBUG		/##/
#   endif /* else TARGET_LINUX */
#  endif /*  BDEBUG */
# endif /* else CONFIG_CO_DEBUG */

/* Debuglevels */
#define CO_DEBUG_CPU		(1 << 0)
#define CO_DEBUG_CAN		(1 << 1)
#define CO_DEBUG_SDO		(1 << 2)
#define CO_DEBUG_SDOBLOCK	(1 << 3)
#define CO_DEBUG_SDOMANAGER	(1 << 4)
#define CO_DEBUG_PDO		(1 << 5)
#define CO_DEBUG_LSS		(1 << 6)

/* Funktionen zur Meldungsausgabe aus co_debug.c */
void debugprint(int level, char *fmt, ...);

#endif /* __CO_DEBUG_H */

