/*
 * co_flag - defines for library flag usage
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
 * Revision 1.0  2008-03-07 17:05:35+01  driet
 * Initial revision
 *
 * Revision 2.3  2002/05/21 14:54:30  boe
 * copyright changed
 *
 * Revision 2.2  2001/03/14 17:24:36  ro
 * Redundancy Support added
 *
 * Revision 2.1  2001/01/26 12:33:30  boe
 * defines for flags
 *
 *
 *
 *------------------------------------------------------------------
 */

/*
DESCRIPTION

The file contains definitions of structures and data types for
flag usage

*/

#ifndef __CO_FLAG_H
# define __CO_FLAG_H

# include <co_def.h>        /* include canopen definition */
# include <co_type.h>
# ifdef CONFIG_REDUNDANCY_SUPPORT
#  include <co_redcy.h>
# endif /* CONFIG_REDUNDANCY_SUPPORT */


/* CO Library Flag Definition */
#ifndef TEST_COLIB_FLAG 
extern UNSIGNED8    coFlags ;
/* # define SET_COLIB_FLAG(FLAG CO_COMMA_LINE_PARA) \ */
        /* coFlags  |= FLAG; */
#  define SET_COLIB_FLAG(FLAG)    coFlags  |= (FLAG)
#  define RESET_COLIB_FLAG(FLAG)  coFlags  &= ~(FLAG)
#  define TEST_COLIB_FLAG(FLAG)   coFlags  & (FLAG)
#endif

#define COFLAG_ALL             ((UNSIGNED8)0xFF)
#define COFLAG_CAN_ERROR       (COFLAG_CAN_BUSOFF + COFLAG_CAN_OVERFLOW + COFLAG_CAN_PASSIVE + COFLAG_BUFFER_OVERFLOW)
#define COFLAG_SYNC_RECEIVED   ((UNSIGNED8)0x01) /* sync received */
#define COFLAG_TIMER_PULSED    ((UNSIGNED8)0x02) /* timer has pulsed */
#define COFLAG_SDO_BLOCKTRANS  ((UNSIGNED8)0x04) /* outstanding block transfers */
#define COFLAG_SDO_MANAGER     ((UNSIGNED8)0x08) /* outstanding sdo manager transfers */
#define COFLAG_CAN_BUSOFF      ((UNSIGNED8)0x10)
#define COFLAG_CAN_PASSIVE     ((UNSIGNED8)0x20)
#define COFLAG_CAN_OVERFLOW    ((UNSIGNED8)0x40)
#define COFLAG_BUFFER_OVERFLOW ((UNSIGNED8)0x80) /* Buffer overflow */


/* external data declarations */

/* function prototypes */


#endif      /*  __CO_FLAG_H */

/* end of source */

