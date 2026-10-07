/*
 * timer - defines for timer usage
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
 * Revision 1.0  2008-03-07 17:05:56+01  driet
 * Initial revision
 *
 * Revision 2.5  2002/11/18 10:45:07  boe
 * add multiline parameter
 *
 * Revision 2.4  2002/05/29 12:01:22  ro
 * parameter list of checkTimerEvent() changed to void
 *
 * Revision 2.3  2002/05/21 14:25:29  boe
 * moved public defines to co_timer.h
 *
 * Revision 2.2  2002/03/26 08:36:27  boe
 * add new function stopInhibitTimer()
 * provide lss time services
 *
 * Revision 2.1  2001/12/20 13:57:34  boe
 * defines for timer routines
 *
 *
 *
 *------------------------------------------------------------------
 */

/*
DESCRIPTION

The file contains definitions of structures and data types for timer usage

*/

#ifndef __TIMER_H
# define __TIMER_H

#include <co_type.h>
#include <co_timer.h>


struct INHIBIT_EVENT {
	struct INHIBIT_EVENT	*pNext;
	UNSIGNED16		ticks;		/* timer ticks */
};

typedef struct INHIBIT_EVENT INHIBIT_EVENT_T;

/* external data declarations */

extern TIMER_EVENT_T	*co_timerList ;

/* function prototypes */

void checkTimerEvent(void);
void startInhibitTimer(INHIBIT_EVENT_T * , UNSIGNED16 );
void stopInhibitTimer(INHIBIT_EVENT_T * );

#endif		/*  __TIMER_H */

/* end of source */
