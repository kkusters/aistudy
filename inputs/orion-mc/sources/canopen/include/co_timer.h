/*
 * co_timer - public defines for timer usage
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
 * Revision 1.1  2008-03-26 17:06:09+01  driet
 * <>
 *
 * Revision 1.0  2008-03-07 17:05:44+01  driet
 * Initial revision
 *
 * Revision 2.5  2003/03/31 13:08:53  boe
 * define a new bit for timer remain
 *
 * Revision 2.4  2003/01/21 08:57:40  boe
 * defines for LSS changed
 *
 * Revision 2.3  2002/11/18 11:10:33  boe
 * add support for multiline
 *
 * Revision 2.2  2002/08/30 08:50:54  boe
 * add includefile co_def.h
 *
 * Revision 2.1  2002/05/21 14:57:15  boe
 * public defines for timer usage
 *
 *
 *
 *
 *------------------------------------------------------------------
 */

/*
DESCRIPTION

The file contains definitions of structures and data types for timer usage

*/

#ifndef __CO_TIMER_H
# define __CO_TIMER_H

#include <co_type.h>
#include <co_def.h>

/* structure of a timer */

struct TIMER_EVENT
{
  struct TIMER_EVENT  *pNext;
  UNSIGNED32      timerVal;   // timerVal in 1/10 of msec
#ifdef CONFIG_LARGE_TIMER
  UNSIGNED32      endTime;    // endtime in timerticks
#else // CONFIG_LARGE_TIMER
  UNSIGNED16      endTime;    // endtime in timerticks
#endif // CONFIG_LARGE_TIMER
  UNSIGNED16      restTime;   // restTime in 1/10 of msec
  UNSIGNED8       timerType;  // type of timer event
};

typedef struct TIMER_EVENT TIMER_EVENT_T;


/* timer types for execution */
#define CO_TIMER_TYPE_CYCLIC          ((UNSIGNED8)0x80) /* for cyclic timers */
#define CO_TIMER_TYPE_AGAIN           ((UNSIGNED8)0x40) /* for start timer again */
#define CO_TIMER_TYPE_REMAIN          ((UNSIGNED8)0x20) /* use remain time */
#define CO_TIMER_TYPE_SYNC            ((UNSIGNED8)1)    /* for sync transmit */
#define CO_TIMER_TYPE_HB_PROD         ((UNSIGNED8)2)    /* for heartbeat producer transmit */
#define CO_TIMER_TYPE_HB_CONS         ((UNSIGNED8)3)    /* for heartbeat consumer */
#define CO_TIMER_TYPE_NG_MSTR         ((UNSIGNED8)4)    /* for Nodeguarding master */
#define CO_TIMER_TYPE_NG_SLAVE        ((UNSIGNED8)5)    /* for Nodeguarding slave */
#define CO_TIMER_TYPE_EVENTRPDO       ((UNSIGNED8)6)    /* for event rec pdos */
#define CO_TIMER_TYPE_EVENTTPDO       ((UNSIGNED8)7)    /* for event trans pdos */
#define CO_TIMER_TYPE_FLYMA_DETECTM   ((UNSIGNED8)8)    /* for flyma detect manager */
#define CO_TIMER_TYPE_FLYMA_ACTIVEM   ((UNSIGNED8)9)    /* for flyma detect active manager */
#define CO_TIMER_TYPE_FLYMA_TRIGTSLOT ((UNSIGNED8)10)   /* for flyma start trigger timeslot*/
#define CO_TIMER_TYPE_FLYMA_SENDMID   ((UNSIGNED8)11)   /* for flyma send mid */
#define CO_TIMER_TYPE_FLYMA_CYC_CHECK ((UNSIGNED8)12)   /* for flyma cyclic check */
#define CO_TIMER_TYPE_SRDO_PROD       ((UNSIGNED8)13)   /* for srdo producer */
#define CO_TIMER_TYPE_SRDO_CON        ((UNSIGNED8)14)   /* for srdo consumer */
#define CO_TIMER_TYPE_LSS_SL          ((UNSIGNED8)15)   /* for lss slave timer */
#define CO_TIMER_TYPE_LSS_MSTR        ((UNSIGNED8)16)   /* for lss ,master timer */
#define CO_TIMER_TYPE_LED             ((UNSIGNED8)17)   /* for led timer */
#define CO_TIMER_TYPE_USERSPEC        ((UNSIGNED8)20)   /* for user specific timers */

/* external data declarations */

/* function prototypes */

void removeTimerEvent(TIMER_EVENT_T *pTimer );
UNSIGNED8 addTimerEvent(TIMER_EVENT_T *pTimer, UNSIGNED32 timerVal, UNSIGNED8 timerType );
void userTimerEvent(TIMER_EVENT_T * );
BOOL_T checkActiveTimer(TIMER_EVENT_T   *pTimer );

#endif      /*  __CO_TIMER_H */

/* end of source */
