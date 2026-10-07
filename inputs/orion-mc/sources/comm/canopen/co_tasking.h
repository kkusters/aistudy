/*
 * co_tasking.h - special defines for the Tasking Compiler
 *
 * Copyright (c) 2002-2003 port GmbH Halle/Saale
 *------------------------------------------------------------------
 *
 * $Header$
 *
 *------------------------------------------------------------------
 * $Log$
 * Revision 1.0  2011-07-18 11:49:02+02  driet
 * Initial revision
 *
 * Revision 1.0  2008-03-06 17:16:55+01  driet
 * Initial revision
 *
 * Revision 1.1  2003/06/05 08:16:34  ro
 * first adaptation for Tasking C166 Compiler
 *
 *
 * Compiler dependend header
 *
 *
 *
 *------------------------------------------------------------------
 */

/**
* \file co_tasking.h
* \author port GmbH
* $Revision$
* $Date$
*
*
*++ This header file supports the Tasking Compiler
*++ C166.
*-- Diese Header-Datei enthält Defines zur Verwendung
*-- mit dem Tasking C166-Compiler.
*
*/

#ifndef __CO_TASKING_H
#define __CO_TASKING_H

# include <stddef.h>

/* --------------------------------------------------------------- */
# ifdef CONFIG_CPU_FAMILY_C166
/* --------------------------------------------------------------- */
#  ifndef CO_CONST
#    define CO_CONST	const
#  endif
#  ifndef CO_DATA
#    define CO_DATA
#  endif
#  ifndef CO_CODE
#    define CO_CODE 	
#  endif
#  ifndef FAR
#    define FAR 	far
#  endif
#  ifndef XDATA
#    define XDATA
#  endif

#  ifndef VOLATILE
#    define VOLATILE 	volatile
#  endif

/* --------------------------------------------------------------- */
# endif /* CONFIG_CPU_FAMILY_C166 */
/* --------------------------------------------------------------- */

/* --------------------------------------------------------------- */
# ifdef CONFIG_CPU_FAMILY_8051
/* --------------------------------------------------------------- */
#  ifndef CO_CONST
#    define CO_CONST	
#  endif
#  ifndef CO_DATA
#    define CO_DATA 	
#  endif
#  ifndef IDATA
#    define IDATA 	
#  endif
#  ifndef CO_CODE
#    define CO_CODE 	
#  endif
#  ifndef FAR
#    define FAR 
#  endif
#  ifndef XDATA
#    define XDATA 	
#  endif

#  ifndef VOLATILE
#    define VOLATILE 	
#  endif

/* --------------------------------------------------------------- */
# endif /* CONFIG_CPU_FAMILY_8051 */
/* --------------------------------------------------------------- */

# ifndef INTERRUPT
#  define INTERRUPT
# endif

# ifdef CONFIG_DRIVER_TEST
#  include <stdio.h>
#  ifndef PRINTF
#   define PRINTF printf
#  endif /* PRINTF */
# endif /* CONFIG_DRIVER_TEST */

#endif /* __CO_TASKING_H */
