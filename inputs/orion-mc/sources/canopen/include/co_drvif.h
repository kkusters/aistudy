/*
 * co_drvif - declarations for public driver interface between lib and driver
 *
 * Copyright (c) 2002 port GmbH Halle/Saale
 *------------------------------------------------------------------
 * $Header$
 *
 *------------------------------------------------------------------
 *
 * modification history
 * --------------------
 * $Log$
 * Revision 1.0  2008-03-07 17:05:34+01  driet
 * Initial revision
 *
 * Revision 2.1  2002/11/18 10:53:59  boe
 * driver interface defines
 *
 *
 *
 *------------------------------------------------------------------
 */

/*
DESCRIPTION

The file contains definitions of structures and complex data types
for the driver interface between library and driver
It is normally not designed for user applications
and shouldn't use by them.

*/

#ifndef __CO_DRVIF_H
# define __CO_DRVIF_H

#include <co_def.h>		/* include canopen definition */
#include <co_stru.h>
#include <co_drv.h>

# ifndef FLAG_IDENTIFICATION
#   define FLAG_IDENTIFICATION	flagIdentification
# endif /* FLAG_IDENTIFICATION */

# ifndef MSG_IDENTIFICATION
#    define MSG_IDENTIFICATION(par1)	msgIdentification(par1)
# endif /* MSG_IDENTIFICATION */


/* external variable declarations */

extern volatile UNSIGNED8	coTimerTicks ;	/* CANopen timer ticks */

# ifdef CONFIG_REDUNDANCY_SUPPORT
extern COB_T     *co_pFirst_COB_Entry [];
# else /* CONFIG_REDUNDANCY_SUPPORT */
extern COB_T     *co_pFirst_COB_Entry ;
# endif /* CONFIG_REDUNDANCY_SUPPORT */


/* function prototypes */

#if defined(CONFIG_REDUNDANCY_SUPPORT)
void	msgIdentification(CAN_MSG_T *canMsg);
#else /* CONFIG_REDUNDANCY_SUPPORT */
void	msgIdentification(CAN_MSG_T *canMsg );
#endif	/*CONFIG_REDUNDANCY_SUPPORT */

#ifdef CONFIG_REDUNDANCY_SUPPORT
void	flagIdentification(void);
#else
void	flagIdentification(void);
#endif

COB_T	*Define_COB(COB_KIND_T, UNSIGNED8 );
void	GetNext_TX_Request(void) RTX51_MODIFIER;
void	Set_COB_ID(COB_T *, UNSIGNED16);
void	Transmit_COB(COB_T *, UNSIGNED8 *);
void	Update_COB(COB_T *, UNSIGNED8 *);

#endif /* __CO_DRVIF_H */

/* end of source */
