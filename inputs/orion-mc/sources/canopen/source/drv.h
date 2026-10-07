/*
 * drv - defines for driver interface
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
 * Revision 1.0  2008-03-07 17:05:47+01  driet
 * Initial revision
 *
 * Revision 2.6  2002/11/18 10:24:49  boe
 * remove declaration of external variables
 * remove defines for standard functions
 *
 * Revision 2.5  2002/05/21 12:45:44  boe
 * change copyright
 *
 * Revision 2.4  2001/09/13 15:06:58  boe
 * add prototypes for special channel usage
 *
 * Revision 2.3  2001/03/14 15:40:49  ro
 * prototypes for Redundancy Support moved to special header
 * prototypes for MULT_CANCONTROL_TYPE moved to special header
 *
 * Revision 2.2  2001/02/26 14:48:54  boe
 * prototype declaration for multiline changed
 *
 * Revision 2.1  2001/01/26 10:49:11  boe
 * defines for driver interface
 *
 *
 *
 *------------------------------------------------------------------
 */

/*
DESCRIPTION

The file contains definitions of structures and complex data types
for driver interface

*/

#ifndef __DRV_H
# define __DRV_H

#include <co_def.h>
#include <co_drv.h>
#include <co_drvif.h>

# ifdef CONFIG_REDUNDANCY_SUPPORT
#  include "drv_rdcy.h"
# endif /* CONFIG_REDUNDANCY_SUPPORT */



/* external variable declarations */



/* are the macros not defined use the standard functions */
# ifndef DEFINE_COB
#   define DEFINE_COB		Define_COB
# endif /* DEFINE_COB */

# ifndef SET_COB_ID
#   define SET_COB_ID		Set_COB_ID
# endif /* SET_COB_ID */

# ifndef TRANSMIT_COB
#   define TRANSMIT_COB		Transmit_COB
# endif /* TRANSMIT_COB */

# ifndef UPDATE_COB
#   define UPDATE_COB		Update_COB
# endif /* UPDATE_COB */

# ifndef CLEAR_RX_BUFFER
#   define CLEAR_RX_BUFFER	clearRxBuffer
# endif /* CLEAR_RX_BUFFER */

# ifndef CLEAR_TX_BUFFER
#   define CLEAR_TX_BUFFER	clearTxBuffer
# endif /* CLEAR_TX_BUFFER */



/* function prototypes */

#endif /* __DRV_H */

/* end of source */
