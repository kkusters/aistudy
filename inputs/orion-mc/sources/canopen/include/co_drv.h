/*
 * co_drv - declarations for public driver interface
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
 * Revision 1.0  2008-03-07 17:05:33+01  driet
 * Initial revision
 *
 * Revision 2.8  2002/11/18 10:52:53  boe
 * add prototype for iniDevice
 *
 * Revision 2.7  2002/05/21 14:53:49  boe
 * copyright changed
 *
 * Revision 2.6  2002/04/05 13:23:21  boe
 * add external declaration for bittiming table
 *
 * Revision 2.5  2001/12/05 15:09:51  boe
 * don't include files for multi can controllers in standard mode
 *
 * Revision 2.4  2001/03/14 17:21:33  ro
 * MultiCanController prototypes moved to a special header
 * Redundancy Support added
 *
 * Revision 2.3  2001/02/26 14:59:18  boe
 * driver access functions replaced by macros
 *
 * Revision 2.2  2001/01/31 12:20:33  ro
 * Define for Set_Baudrate in MULTILINE Mode corrected
 *
 * Revision 2.1  2001/01/26 12:32:49  boe
 * defines for driver interface
 *
 *
 *
 *------------------------------------------------------------------
 */

/*
DESCRIPTION

The file contains definitions of structures and complex data types
for the public driver interface

*/

#ifndef __CO_DRV_H
# define __CO_DRV_H

#include <co_def.h>		/* include canopen definition */
#include <co_stru.h>

/* if RTX51 not defined */
#ifndef CONFIG_OS_RTX51
# ifndef RTX51_MODIFIER
#  define RTX51_MODIFIER
# endif
#endif


/* returns for Init_CAN */

# define CO_INIT_CAN_OK			0
# define CO_E_INIT_HARD_RES_ACTIVE 	1
# define CO_E_INIT_CLK_FREQ		2
# define CO_E_INIT_BAUD			3
# define CO_E_INIT_PROP			4
# define CO_E_INIT_WRONG_ADDRESS        5
# define CO_E_INIT_UNSPEC_ERROR         255


/* table for CAN-Timings */
/* for every baudrate must exist a entry in the table 	*/
/* for 125kbit/s rate must be 125                       */
/* the last entry must be 0,0,0				*/
/* e.g. 	
 * const BTR_TAB_T can_btr_tab[] = { {rate1, 1_BTR0, 1_BTR1},
 *				     {rate2, 2_BTR0, 2_BTR1},
 *				     {    0,      0,      0} ***last Entry***
 *				   }
 */
typedef struct {
    UNSIGNED16 rate;
    UNSIGNED8  btr0;
    UNSIGNED8  btr1;
} BTR_TAB_T;    


/* set defaults for CAN driver buffers */

# ifndef CONFIG_RX_BUFFER_SIZE       /* if not defined in cal_conf.h */
#  define CONFIG_RX_BUFFER_SIZE 10
# endif

# ifndef CONFIG_TX_BUFFER_SIZE       /* if not defined in cal_conf.h */
#  define CONFIG_TX_BUFFER_SIZE 10
# endif

# ifndef CONFIG_BIT_RATE_INDEX       /* is necessary for switching bitrate on devices, */
#  define CONFIG_BIT_RATE_INDEX 0    /* which have no non volatile memory */
# endif


/* external variable declarations */

extern volatile UNSIGNED32	dTimerValue;
extern UNSIGNED16 CO_CONST	wTimerPulse;
extern UNSIGNED16 CO_CONST	coTimerPulse;

extern CO_CONST UNSIGNED16 co_bittiming_table[];


/* include special defines for redundancy support */
# ifdef CONFIG_REDUNDANCY_SUPPORT
#  include <co_drvry.h>
# endif /* CONFIG_REDUNDANCY_SUPPORT */


/* are the macros not defined use the standard functions */
# ifndef CLEAR_BUSOFF
#   define CLEAR_BUSOFF		Clear_busoff
# endif /* CLEAR_BUSOFF */

# ifndef FLUSH_MBOX
#   define FLUSH_MBOX		FlushMbox
# endif /* FLUSH_MBOX */

# ifndef START_CAN
#   define START_CAN		Start_CAN
# endif /* START_CAN */

# ifndef STOP_CAN
#   define STOP_CAN		Stop_CAN
# endif /* STOP_CAN */

# ifndef INIT_CAN
#  define INIT_CAN		initCan
# endif /* INIT_CAN */

# ifndef SET_BAUDRATE
#  define SET_BAUDRATE		Set_Baudrate
# endif /* SET_BAUDRATE */

# ifndef CLEAR_BUSOFF
#  define CLEAR_BUSOFF		Clear_busoff
# endif /* CLEAR_BUSOFF */

# ifndef SETINTMASK
#  define SETINTMASK		SetIntMask
# endif /* SETINTMASK */

# ifndef RESETINTMASK
#  define RESETINTMASK		ResetIntMask
# endif /* RESETINTMASK */


/* function prototypes */

UNSIGNED16	InitCalMalloc(UNSIGNED16 );
void		*CalMalloc(size_t x);
void		CalFree(void *);
INTEGER32	compareTime(UNSIGNED32, UNSIGNED32);

void		FlushMbox(void);
UNSIGNED8	initTimer(void);
void		releaseTimer(void);
void		clearTxBuffer(void);
void		clearRxBuffer(void);
BOOL_T		checkTxBuffer(void);
UNSIGNED8	getNumberOfTxMessages(void);
UNSIGNED8	getNumberOfRxMessages(void);

UNSIGNED8	iniDevice(void);
UNSIGNED8	initCan(UNSIGNED16 );
UNSIGNED8	Set_Baudrate(UNSIGNED16, BTR_TAB_T * );
void		Start_CAN(void);
void		Stop_CAN(void);
void		Clear_busoff(void);
void		SetIntMask(void);
void		ResetIntMask(void);


#endif /* __CO_DRV_H */

/* end of source */

