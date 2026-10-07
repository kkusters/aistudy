/*
 *++ timer - contains timer functionality
 *-- timer - beinhaltet die Timer Funktionalität
 *
 * Copyright (c) 2001-2002 port GmbH Halle (Saale)
 *------------------------------------------------------------------
 * $Header$
 *
 *------------------------------------------------------------------
 *
 * modification history
 * --------------------
 * $Log$
 * Revision 1.0  2008-03-07 17:04:50+01  driet
 * Initial revision
 *
 * Revision 2.18  2003/07/30 08:14:30  boe
 * include time and sync header only if CONFIG_TIME/CONFIG_SYNC is set
 *
 * Revision 2.17  2003/07/09 14:43:05  boe
 * if time less than timertick use timertick (addTimerEvent)
 *
 * Revision 2.16  2003/03/31 15:43:48  boe
 * add multiline parameter for event timer functions
 *
 * Revision 2.15  2003/03/31 13:50:15  boe
 * rework for inhibit timers
 * call usrTimerEvents with line para
 * check eva-runtime directly
 * delete resttime for new timers
 *
 * Revision 2.14  2003/02/28 16:13:55  boe
 * correct multiline usage for heartbeat consumers
 * save timerTicks before usage
 *
 * Revision 2.13  2003/01/27 09:37:51  boe
 * expand all functions for multiline
 *
 * Revision 2.12  2003/01/21 08:59:22  boe
 * defines for LSS changed
 *
 * Revision 2.11  2003/01/20 15:17:12  boe
 * add multiline parameter for lss functions
 *
 * Revision 2.10  2003/01/09 16:18:04  boe
 * add type casts for calculated times
 *
 * Revision 2.9  2002/12/11 08:46:27  boe
 * check for timer values less than timer cycle
 *
 * Revision 2.8  2002/11/18 10:00:10  boe
 * add comments/adapt on doxygen
 * add co_ to all global library variables
 * enlarge for multiline usage
 *
 * Revision 2.7  2002/07/23 09:09:54  hae
 * disable range checking for large timer when adding a new timer
 *
 * Revision 2.6  2002/05/30 14:26:06  hae
 * corrected includes
 *
 * Revision 2.5  2002/05/28 14:25:00  hae
 * Added/ completed english documentation
 *
 * Revision 2.4  2002/05/21 14:26:20  boe
 * cleanup module
 *
 * Revision 2.3  2002/04/05 13:09:00  boe
 * timer check routine modified
 *
 * Revision 2.2  2002/03/26 08:36:09  boe
 * add new function stopInhibitTimer()
 * provide lss time services
 *
 * Revision 2.1  2001/12/20 13:57:23  boe
 * timer routines
 *
 *
 *
 *------------------------------------------------------------------
 */


/****************************************************************************/
/**
*  \file timer.c
*  \author port GmbH Halle (Saale)
*  $Revision$
*  $Date$
*
*++ This module contains all functions for usage of timer driven events
*++ of the CANopen Library.
*-- Dieses Modul enthält alle Funktionen zur Benutzung von Timerdiensten
*-- in der Bibliothek.
* \par
*++ Some of the functions are for internal usage only.
*++ They have no manual entries.
*-- Einige dieser Funktionen werden nur intern benutzt.
*-- Sie haben keine Handbuch Einträge.
*
*/

/* header of standard C - libraries */
#include <stdio.h>

/* header of common types */
#include <cal_conf.h>
#include <co_drv.h>
#include <co_emcy.h>
#if defined(CONFIG_SYNC_PRODUCER) || defined(CONFIG_SYNC_CONSUMER)
# include "sync.h"
#endif /* defined(CONFIG_SYNC_PRODUCER) || defined(CONFIG_SYNC_CONSUMER) */
#include "pdo.h"
#include "nmterr.h"
#if defined(CONFIG_MASTER) || defined(CONFIG_SLAVE_PLUS)
#include "nmterr_m.h"
#endif /* defined(CONFIG_MASTER) || defined(CONFIG_SLAVE_PLUS) */
#ifdef CONFIG_FLYING_MASTER
# include "flyma.h"
#endif /* CONFIG_FLYING_MASTER */
#if defined(CONFIG_SRDO_PRODUCER) || defined(CONFIG_SRDO_CONSUMER)
# include "srdo.h"
#endif /* CONFIG_SRDO_PRODUCER */
#if defined(CONFIG_LSS_SLAVE) || defined(CONFIG_LSS_MASTER)
# include "lss.h"
#endif /* defined(CONFIG_LSS_SLAVE) || defined(CONFIG_LSS_MASTER) */
#ifdef CONFIG_CO_LED
# include "led.h"
#endif /* CONFIG_CO_LED */

/* constant definitions
---------------------------------------------------------------------------*/

/* local defined data types
---------------------------------------------------------------------------*/

/* list of external used functions, if not in headers
---------------------------------------------------------------------------*/

/* list of global defined functions
---------------------------------------------------------------------------*/

/* list of local defined functions
---------------------------------------------------------------------------*/
int printTimerType(UNSIGNED8 timerType);

/* external variables
---------------------------------------------------------------------------*/
extern volatile UNSIGNED8    coTimerTicks ;

/* global variables
---------------------------------------------------------------------------*/
TIMER_EVENT_T	*co_timerList  = { NULL };
INHIBIT_EVENT_T	*co_inhibitList  = { NULL };
#ifdef CONFIG_EVA_VERSION
TIMER_EVENT_T	evaTimer;
#endif /* CONFIG_EVA_VERSION */

/* local defined variables
---------------------------------------------------------------------------*/
#ifdef CONFIG_RCS_IDENT
static char _rcsid[] = "$Id$";
#endif /* CONFIG_RCS_IDENT */


/*******************************************************************/
/*
*++ \brief checkTimerEvent - check the timer list for next event
*-- \brief checkTimerEvent - prüft die Timerliste für das nächste Event
*
* NOMANUAL
*
*++ This function checks if a event timed out.
*++ If yes, then the apropriate function is called,
*++ and the time is adjusted for the next event.
*-- Diese Funktion testet, ob die Zeit für das nächste Timerereignis 
*-- abgelaufen ist.
*-- Wenn ja, wird die entsprechende Funktion aufgerufen
*-- und die Zeit für das nächste Timerereignis gesetzt
*
* \retval CO_OK
*	nothing
*
*/

void checkTimerEvent(
    void
       )
{
TIMER_EVENT_T	*pTimer;	/* pointer to timer structure */
INHIBIT_EVENT_T	*pInhibit,	/* pointer to inhibit structure */
		*pLastInhibit;	/* pointer to last inhibit structure */
UNSIGNED8	timerTicks;	/* temporary coTimerTicks */

    /* coTimerTicks can be changed by the timer interrupt
     * therefore we save it at a temporary variable */
    timerTicks = coTimerTicks ;
    pTimer = co_timerList ;

    /* first, check the inhibit timers */
    if (co_inhibitList  != NULL)  {
	pInhibit = co_inhibitList ;
	pLastInhibit = NULL;
	/* for each entry */
	while (pInhibit != NULL)  {
#ifdef TIMER_DEBUG
printf("ptr = %x, ticks: %d, next = %x\n", (int)pInhibit, (int)pInhibit->ticks, (int)pInhibit->pNext);
#endif /* TIMER_DEBUG */
	    if (pInhibit->ticks < timerTicks)  {
		pInhibit->ticks = 0;
	    } else {
		pInhibit->ticks -= timerTicks;
	    }
	    if (pInhibit->ticks == 0)  {
#ifdef TIMER_DEBUG
printf("checkTimerEvent: dTimerValue (in ms): %ld,  ", dTimerValue / 10);
printf("inhibitTime reached, nextPtr %x\n", (int)pInhibit->pNext);
#endif /* TIMER_DEBUG */
		/* remove it from the timer list */
		if (pLastInhibit != NULL)  {
		    pLastInhibit->pNext = pInhibit->pNext;
		    pInhibit->pNext = NULL;
		    pInhibit = pLastInhibit->pNext;
		} else {
		    co_inhibitList  = pInhibit->pNext;
		    pInhibit->pNext = NULL;
		    pInhibit = co_inhibitList ;
		}
	    } else {
		pLastInhibit = pInhibit;
		pInhibit = pInhibit->pNext;
	    }
	}
    } else {
	/* no inhibit timer active */

	/* valid timer entries available ? */
	if (pTimer == NULL)  {
	    /* no timer event defined, then delete actual ticks */
	    /* only allowed, if no inhibit timers are active */
	    coTimerTicks  = 0;
	    return;
	}

	/* check, if the first timer event is reached */
	/* as a precaution test it only if no event timer is active */
	if ((timerTicks < pTimer->endTime)
	 && (timerTicks < 10)) {
	    /* no, return */
	    return;
	}
    }

    /* we wait waited, until the first time is reached or timerticks > 10 */
    coTimerTicks  -= timerTicks;

    /* for all timer events */
    while (pTimer != NULL)  {

#ifdef TIMER_DEBUG
printf("ptr2 ");
printTimerType(pTimer->timerType);
printf(" = %x, ticks: %d, next = %x\n", (int)pTimer, (int)pTimer->endTime, (int)pTimer->pNext);
#endif /* TIMER_DEBUG */

	if (pTimer->endTime < timerTicks)  {
	    pTimer->endTime = 0;
	} else {
	    pTimer->endTime -= timerTicks;
	}
	pTimer = pTimer->pNext;
    }

    /* remove the zero entries and restart cyclic timers */
    pTimer = co_timerList ;
    while (pTimer != NULL)  {
	/* if the time is up */
	if (pTimer->endTime == 0)  {
#ifdef TIMER_DEBUG
printf("\ncheckTimerEvent: dTimerValue (in ms): %ld,  ", dTimerValue / 10);
printf("timerticks: %d, endTime: %d\n", coTicks, pTimer->endTime);
#endif /* TIMER_DEBUG */

#ifdef CONFIG_EVA_VERSION
	    /* check for eva-version timer */
	    if (pTimer == &evaTimer)  {
		((void (CO_CODE *) (void))0x0)(); /* reset */
	    }
#endif /* CONFIG_EVA_VERSION */

	    /* call the timer event */
	    switch (pTimer->timerType & ~CO_TIMER_TYPE_CYCLIC)  {

#ifdef CONFIG_SYNC_PRODUCER
		case CO_TIMER_TYPE_SYNC:
		    writeSyncReq(CO_LINE_PARA);
		    break;
#endif /* CONFIG_SYNC_PRODUCER */

#if defined(CONFIG_HEARTBEAT_PRODUCER)
		case CO_TIMER_TYPE_HB_PROD:
		    NMT_HB_TimerPulse(CO_LINE_PARA);
		    break;
#endif /* CONFIG_HEARTBEAT_PRODUCER */

#if defined(CONFIG_PDO_CONSUMER) && defined(CONFIG_PDO_EVENTTIMER)
		case CO_TIMER_TYPE_EVENTRPDO:
		    /* printf("* Event Rec PDO\n"); */
		    eventRecPdo(pTimer CO_COMMA_LINE_PARA);
		    break;
#endif /* defined(CONFIG_PDO_CONSUMER) && defined(CONFIG_PDO_EVENTTIMER) */

#if defined(CONFIG_PDO_PRODUCER) && defined(CONFIG_PDO_EVENTTIMER)
		case CO_TIMER_TYPE_EVENTTPDO:
		    /* printf("* Event Trans PDO\n"); */
		    eventTransPdo(pTimer CO_COMMA_LINE_PARA);
		    break;
#endif /* defined(CONFIG_PDO_PRODUCER) && defined(CONFIG_PDO_EVENTTIMER) */

#if defined(CONFIG_NODE_GUARDING) && defined(CONFIG_MASTER)
		case CO_TIMER_TYPE_NG_MSTR:
		    /* printf("* NodeGuarding Master Event\n"); */
		    NMT_M_TimerPulse(pTimer CO_COMMA_LINE_PARA);
		    break;
#endif /* defined(CONFIG_NODE_GUARDING) && defined(CONFIG_MASTER) */

#if defined(CONFIG_NODE_GUARDING) && defined(CONFIG_SLAVE)
		case CO_TIMER_TYPE_NG_SLAVE:
		    /* printf("* NodeGuarding Slave Event\n"); */
		    NMT_TimerPulse(CO_LINE_PARA);
		    break;
#endif /* defined(CONFIG_NODE_GUARDING) && defined(CONFIG_SLAVE) */

#ifdef CONFIG_HEARTBEAT_CONSUMER
		case CO_TIMER_TYPE_HB_CONS:
		    /* printf("* Heartbeat Consumer Event\n"); */
		    NMT_M_TimerPulse(pTimer CO_COMMA_LINE_PARA);
		    break;
#endif /* CONFIG_HEARTBEAT_CONSUMER */

#ifdef CONFIG_FLYING_MASTER
		case CO_TIMER_TYPE_FLYMA_DETECTM:
		    /* printf("* TimeOut Flying Manager: Detect Manager\n"); */
		    flyMaTout_DetectM();
		    break;

		case CO_TIMER_TYPE_FLYMA_ACTIVEM:
		    /* printf("* TimeOut Flying Manager: Active Manager\n"); */
		    flyMaTout_ActiveM();
		    break;

		case CO_TIMER_TYPE_FLYMA_TRIGTSLOT:
		    /* printf("* TimeOut Flying Manager: Trigger Timeslot\n"); */
		    flyMa_TriggerTimeSlot();
		    break;

		case CO_TIMER_TYPE_FLYMA_SENDMID:
		    /* printf("* TimeOut Flying Manager: send mid\n"); */
		    activeManager_Resp();
		    break;

		case CO_TIMER_TYPE_FLYMA_CYC_CHECK:
		    /* printf("* TimeOut Flying Manager: cyclic check\n"); */
		    activeManager_Req();
		    break;
#endif /* CONFIG_FLYING_MASTER */

#ifdef CONFIG_SRDO_PRODUCER
		case CO_TIMER_TYPE_SRDO_PROD:
		    writeSrdo(pTimer CO_COMMA_LINE_PARA);
		    break;
#endif /* CONFIG_SRDO_PRODUCER */

#ifdef CONFIG_SRDO_CONSUMER
		case CO_TIMER_TYPE_SRDO_CON:
# ifdef CO_TIMER_DEBUG
		    printf("* SRDO Consumer\n");
# endif /* CO_TIMER_DEBUG */
		    srdoTimeOut(pTimer CO_COMMA_LINE_PARA);
		    break;
#endif /* CONFIG_SRDO_CONSUMER*/

#ifdef CONFIG_LSS_MASTER
		case CO_TIMER_TYPE_LSS_MSTR:
		    lssTimeOut(CO_LINE_PARA);
		    break;
#endif /* CONFIG_LSS_MASTER */

#ifdef CONFIG_LSS_SLAVE
		case CO_TIMER_TYPE_LSS_SL:
		    lssSwitchTimeEvent(CO_LINE_PARA);
		    break;
#endif /* CONFIG_LSS_SLAVE */

#ifdef CONFIG_CO_LED
		case CO_TIMER_TYPE_LED:
		    setCoLed(pTimer);
#endif /*  CONFIG_CO_LED */

#ifdef CONFIG_USER_TIMER_EVENT
		case CO_TIMER_TYPE_USERSPEC:
		    userTimerEvent(pTimer CO_COMMA_LINE_PARA);
		    break;
#endif /* CONFIG_USER_TIMER_EVENT */

		default:
# ifdef CO_TIMER_DEBUG
		    printf("*** Unknown Timervent\n");
# endif /* CO_TIMER_DEBUG */
		    break;
	    }

	    /* disable/restart timer */
	    removeTimerEvent(pTimer CO_COMMA_LINE_PARA);
	    /* if a cyclic timer, start it again */
	    if ((pTimer->timerType &
			    (CO_TIMER_TYPE_CYCLIC | CO_TIMER_TYPE_AGAIN))
		    != 0) {
		pTimer->timerType &= ~CO_TIMER_TYPE_AGAIN;
		addTimerEvent(pTimer, pTimer->timerVal,
			pTimer->timerType | CO_TIMER_TYPE_REMAIN
			CO_COMMA_LINE_PARA);
	    }
	    /* pTimer = pTimer->pNext; */
	    pTimer = co_timerList ;
	} else {
	    break;
	}
    }

#ifdef TIMER_DEBUG
pTimer = co_timerList;
if(pTimer != NULL) {
printf("ptr4 ");
printTimerType(pTimer->timerType);
printf(" = %x, ticks: %d, next = %x\n", (int)pTimer, (int)pTimer->endTime, (int)pTimer->pNext);
} else {
printf("ptr4: pTimer==NULL\n");
}
#endif /* TIMER_DEBUG */


}


/*******************************************************************/
/**
*++ \brief addTimerEvent - add a timerevent to the timer list
*-- \brief addTimerEvent - fügt ein Timerevent zur Timerliste hinzu
*
*++ This function adds a new timer event to the timer list.
*++ At first it is checked
*++ if this event already exists.
*++ It is then deleted before 
*++ it is added to the timer list.
*++ The timer list is a sortet linked list.
*-- Diese Funktion fügt ein neues Timerereignis in die Timerliste ein.
*-- Dabei wird zuerst geprüft,
*-- ob schon ein Eintrag von diesem Ereignis vorhanden ist
*-- und ggf. gelöscht.
*-- Anschliessend wird das Timerereignis an der richtigen Stelle
*-- in der verketten Timerliste eingehängt.
*
*++ If the timer value can not be stored in timerticks, then
*++ the function returns with an error.
*++ In order to use large timer values
*++ the define CONFIG_LARGE_TIMER can be set.
*++ Timerticks then are stored as a U32 value.
*++ This is then valid for all timer variables !!
*-- Wenn der Timerwert nicht in Timerticks gespeichert werden kann,
*-- kehrt die Funktion mit einem Fehler zurück.
*-- Als Abhilfe kann das define CONFIG_LARGE_TIMER
*-- gesetzt werden. Dabei wird die timerTick Variable als U32 gespeichert
*-- Diese Variablengröße gilt aber dann für alle Timervariablen !!
*-- Wenn die angegeben Zeit kleiner als die Zeitauflösung (1 Tick) ist,
*-- wird die Zeit auf 1 Tick aufgerundet.
*
* \retval 0
*++	success
*--	Erfolg
* \retval 1
*--	bad time - timerVal = 0
*++	Zeitwert timerVal = 0
* \retval 2
*--	timerVal to large or coTimerTicks to small
*++	timerVal zu groß oder coTimerTicks zu klein gewählt
*/

UNSIGNED8 addTimerEvent(
    TIMER_EVENT_T	*pTimer, /* pointer to event structure */
    UNSIGNED32		timerVal,/* timervalue in 1/10 of msec */
    UNSIGNED8		timerType /* timer type, acyclic/cyclic */
	)
{
TIMER_EVENT_T	*nextTimer,	/* pointer to timer structure */
		*postTimer;	/* pointer to timer structure */
UNSIGNED32	tVal;		/* timer value */

    if (timerVal == 0)  {
# ifdef CO_TIMER_DEBUG
printf("*** addTimerEvent: bad time: 0, ");
printf("type: %d,\n ", timerType);
# endif /* CO_TIMER_DEBUG */
	return(1);
    }

    removeTimerEvent(pTimer CO_COMMA_LINE_PARA);

    /* save type at the timer struct */
    pTimer->timerType = timerType & ~CO_TIMER_TYPE_REMAIN;
    pTimer->timerVal = timerVal;

    /* if isn't a cyclic call, ignore resttime */
    if ((timerType & CO_TIMER_TYPE_REMAIN) == 0)  {
	pTimer->restTime = 0;
    }

#ifdef TIMER_DEBUG
printf("addTimer: %x, ", (int)pTimer);
printTimerType(timerType);
printf("\n");
#endif /* TIMER_DEBUG */
    /* calculate the endtime in ticks */

#ifdef CONFIG_LARGE_TIMER
    /* no range checking for large timer */
#else /* CONFIG_LARGE_TIMER */
    if (timerVal > ((UNSIGNED32)coTimerPulse << 15))  {
	return(2);
    }
#endif /* CONFIG_LARGE_TIMER */

    /* correct the timer period with the resttime from last call */

    /* check for timervalue < timer cycle */
    if ((timerVal + pTimer->restTime) <= coTimerPulse)  {
	pTimer->endTime = 1;
	pTimer->restTime = 0;
    } else {
	tVal = timerVal + pTimer->restTime;

#ifdef CONFIG_LARGE_TIMER
	pTimer->endTime = tVal / coTimerPulse;
#else /* CONFIG_LARGE_TIMER */
	pTimer->endTime = (UNSIGNED16)(tVal / coTimerPulse);
#endif /* CONFIG_LARGE_TIMER */

	pTimer->restTime = (UNSIGNED16)(tVal % coTimerPulse);
    }

    /* for not cyclic timers round up */
    if ((pTimer->restTime != 0) && ((timerType & CO_TIMER_TYPE_CYCLIC) == 0)) {
	pTimer->endTime ++;
    }

    /* add occured and not worked coTimerTicks */
    pTimer->endTime += coTimerTicks ;

#ifdef TIMER_DEBUG
printf("addTimerEvent: dTimerValue (in ms): %ld,  ", dTimerValue / 10);
printf("type: %d, ", timerType);
printf("time: %ld, ticks: %d, rest %d\n", timerVal, pTimer->endTime, pTimer->restTime);
#endif /* TIMER_DEBUG */

    /* set start of the list */
    postTimer = NULL;
    nextTimer = co_timerList ;

#ifdef TIMER_DEBUG
printf("addTimer: co_timerList: %x", co_timerList);
#endif /* TIMER_DEBUG */
    while (nextTimer != NULL)  {
	/* if the new time less, save it before */
	if (pTimer->endTime < nextTimer->endTime)  {
	     break;
	}
	postTimer = nextTimer;
	nextTimer = nextTimer->pNext;
    }

#ifdef TIMER_DEBUG
printf(" postTimer: %x", postTimer);
printf(" nextTimer: %x", nextTimer);
#endif /* TIMER_DEBUG */
    /* setup the new timer */
    pTimer->pNext = nextTimer;

    /* if is the first entry at the list ? */
    if (postTimer == NULL)  {
	co_timerList  = pTimer;
    } else {
	postTimer->pNext = pTimer;
    }
#ifdef TIMER_DEBUG
/* printf(" co_timerList: %x, pNext %x", co_timerList, co_timerList->pNext); */
#endif /* TIMER_DEBUG */
    
    return(0);
}


/*******************************************************************/
/**
*++ \brief removeTimerEvent - delete a timerevent from the timer list
*-- \brief removeTimerEvent - löscht ein Timerevent aus der Timerliste
*
*
*++ This function removes a timer event of the linked list.
*++ If this function is called from inside of a indication function
*++ then this timer is not deleted but marked for deletion
*++ and finally deleted at the end of the function timercheck().
*-- Diese Funktion löscht ein Timerereignis aus der verketten Timerliste.
*-- Wird diese Funktion während einer Timer Indication Funktion aufgerufen,
*-- wird der Timer nur zum Löschen vorgemerkt
*-- und erst am Ende der Timer-Check Funktion gelöscht
*
* \retval none
*/

void removeTimerEvent(
    TIMER_EVENT_T	*pTimer /* pointer to event structure */
	)
{
TIMER_EVENT_T	*pT,		/* pointer to timer structure */
		*pLast;		/* pointer to timer structure */

#ifdef TIMER_DEBUG
printf("removeTimer: %x ", (int)pTimer);
printTimerType(pTimer->timerType);
printf("next: %x\n", pTimer->pNext);
#endif /* TIMER_DEBUG */

    /* search Vorgänger */
    pT = co_timerList ;
    pLast = NULL;

    /* for all timer events */
    while (pT != NULL)  {
	if (pT == pTimer)  {
	    break;
	}
	pLast = pT;
	pT = pT->pNext;
    }
    /* timer wasn't found */
    if (pT == NULL)  {
	return;
    }

#ifdef TIMER_DEBUG
printf("removeTimerEvent: dTimerValue (in ms): %ld,  ", dTimerValue / 10);
printf("type: %d, ", pTimer->timerType);
printf("time: %ld\n", pTimer->timerVal);
printf("next: %x\n", pTimer->pNext);
#endif /* TIMER_DEBUG */

    /* set Nachfolger */
    if (pLast == NULL)  {
	co_timerList  = pTimer->pNext;
    } else {
	pLast->pNext = pTimer->pNext;
    }

    /* delete restTime for acyclic timers */
    if ((pTimer->timerType & CO_TIMER_TYPE_CYCLIC) == 0) {
	pTimer->restTime = 0;
    }
}


/*******************************************************************/
/**
*++ \brief checkActiveTimer - check, if timer is active
*-- \brief checkActiveTimer - testet, ob der Timer aktiv ist
*
*++ This function checks if the timer is active.
*-- Diese Funktion prüft, ob der Timer aktiv ist
*
* \retval CO_TRUE
*++ timer active
*-- Timer aktiv
* \retval CO_FALSE
*++ timer inactive
*-- Timer nicht aktiv
*
*/

BOOL_T checkActiveTimer(
    TIMER_EVENT_T	*pTimer /* pointer to event structure */
	)
{
TIMER_EVENT_T	*pT;		/* pointer to timer structure */

#ifdef TIMER_DEBUG
printf("checkActiveTimer\n");
#endif /* TIMER_DEBUG */

    pT = co_timerList ;

    /* for all timer events */
    while (pT != NULL)  {
	if (pT == pTimer)  {
	    return(CO_TRUE);
	}
	pT = pT->pNext;
    }
    return(CO_FALSE);
}


/*******************************************************************
*
*++ \brief startInhibitTimer - starts a inhibit timer
*-- \brief startInhibitTimer - startet einen Inhibit Timer
*
* NOMANUAL
*
*++ This function starts a new inhibit timer.
*++ The evaluation is done in checkTimeEvent().
*++ Every new inhibit timer is inserted
*++ at the start of the linked timer list.
*++ If the pointer to nextPtr is != 0, then
*++ the inhibit timer is already in the timer list
*++ and it isn´t inserted.
*-- Diese Funktion startet einen neuen Inhibit Timer
*-- Die Auswertung erfolgt in checktimerEvent()
*-- Jeder neue Inhibit Timer wird am Anfang der Liste eingefügt.
*-- Wenn der Zeiger auf nextPtr != 0 ist,
*-- steht der Inhibit Timer schon in der Liste
*-- und muss nicht noch einmaleingetragen werden
*
* RETURNS
* .TP
*/

void startInhibitTimer(
    INHIBIT_EVENT_T	*pInhibit,/* pointer to inhibit structure */
    UNSIGNED16		timerVal/* timervalue in 1/10 of msec */
	)
{
UNSIGNED16 ticks;		/* timer ticks */

    /* calculate the endtime in ticks */
    ticks = timerVal / coTimerPulse;

    if ((timerVal * coTimerPulse) < ticks)  {
	ticks ++;
    }

    /* the inhibit time is a minimum time -
     * the first tick can occure immediately after this function
     * therefore we add one tick 
     */
    ticks ++;

    /* check, if the inhibittimer is active */
    if (pInhibit->ticks != 0)  {
	pInhibit->ticks = ticks;
	return;
    }

    pInhibit->ticks = ticks;

#ifdef TIMER_DEBUG
printf("startInhibitTimer: dTimerValue (in ms): %ld,  ", dTimerValue / 10);
printf("time: %d, ticks: %d\n", timerVal, (int)pInhibit->ticks);
#endif /* TIMER_DEBUG */

    /* save it at start of the list */
    pInhibit->pNext = co_inhibitList ;
    co_inhibitList  = pInhibit;
}


/*******************************************************************
*
*++ \brief stopInhibitTimer - stops a inhibit timer
*-- \brief stopInhibitTimer - stoppt einen Inhibit Timer
*
* NOMANUAL
*
*++ This function removes an inhibit timer from the linked list.
*-- Diese Funktion entfernt einen Inhibit Timer aus der verketteten Liste
*
* \retval
*	nothing
*/

void stopInhibitTimer(
    INHIBIT_EVENT_T	*pInhibit/* pointer to inhibit structure */
	)
{
INHIBIT_EVENT_T	*pTmp;		/* pointer to inhibit structure */
INHIBIT_EVENT_T	*pLastInhibit;	/* pointer to inhibit structure */

    /* delete timer val */
    pInhibit->ticks = 0;

    if (co_inhibitList  == NULL)  {
	return;
    }

    pTmp = co_inhibitList ;
    pLastInhibit = NULL;

    /* for each entry */
    while (pTmp != NULL)  {
	/* is the right entry ? */
	if (pTmp == pInhibit)  {
	    /* yes, remove it from the timer list */
	    if (pLastInhibit == NULL)  {
		co_inhibitList  = pInhibit->pNext;
	    } else {
		pLastInhibit->pNext = pInhibit->pNext;
	    }
	    pInhibit->pNext = NULL;
	    pTmp = NULL;
	} else {
	    pTmp = pTmp->pNext;
	}
    }
}





#ifdef TIMER_DEBUG
int printTimerType(
    UNSIGNED8 timerType
    )
{
struct {
    UNSIGNED8	typ;
    char	*strg;
} static type[3] = {
    {CO_TIMER_TYPE_CYCLIC,	"cyclic timers"},
    {CO_TIMER_TYPE_AGAIN,	"start timer again"},
};
#define TXT_CNT 18
struct {
    UNSIGNED8	typ;
    char	*strg;
} static txt[TXT_CNT] = {
    {CO_TIMER_TYPE_SYNC,	"sync transmit"},
    {CO_TIMER_TYPE_HB_PROD,	"heartbeat producer transmit"},
    {CO_TIMER_TYPE_HB_CONS,	"heartbeat consumer"},
    {CO_TIMER_TYPE_NG_MSTR,	"Nodeguarding master"},
    {CO_TIMER_TYPE_NG_SLAVE,	"Nodeguarding slave"},
    {CO_TIMER_TYPE_EVENTRPDO,	"event rec pdos"},
    {CO_TIMER_TYPE_EVENTTPDO,	"event trans pdos"},
    {CO_TIMER_TYPE_FLYMA_DETECTM, "flyma detect manager"},
    {CO_TIMER_TYPE_FLYMA_ACTIVEM, "flyma detect active manager"},
    {CO_TIMER_TYPE_FLYMA_TRIGTSLOT, "flyma start trigger timeslot"},
    {CO_TIMER_TYPE_FLYMA_SENDMID, "flyma send mid"},
    {CO_TIMER_TYPE_FLYMA_CYC_CHECK, "flyma cyclic check"},
    {CO_TIMER_TYPE_SRDO_PROD,	"srdo producer"},
    {CO_TIMER_TYPE_SRDO_CON,	"srdo consumer"},
    {CO_TIMER_TYPE_LSS1,	"lss timer 1"},
    {CO_TIMER_TYPE_LSS2,	"lss timer 2"},
    {CO_TIMER_TYPE_LSS3,	"lss timer 3"},
    {CO_TIMER_TYPE_USERSPEC,	"user specific timers"}
};
int i;

    for (i = 0; i < 3; i++)  {
	if ((timerType & type[i].typ) != 0)  {
	    printf("%s - ", type[i].strg);
	}
    }
    for (i = 0; i < TXT_CNT; i++)  {
	if ((timerType & 0xf) == txt[i].typ)  {
	    printf("%s - ", txt[i].strg);
	}
    }
}
#endif /* TIMER_DEBUG */


/*______________________________________________________________________EOF_*/
