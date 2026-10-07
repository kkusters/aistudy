/*
 *++ sync - functions for the Synchronisation Object (SYNC) handling
 *-- sync - Funktionen für das Synchronisation Object (SYNC)
 *
 * Copyright (c) 1997-2002 port GmbH Halle (Saale)
 *------------------------------------------------------------------
 * $Header$
 *
 *------------------------------------------------------------------
 *
 * modification history
 * --------------------
 * $Log$
 * Revision 1.0  2008-03-07 17:04:49+01  driet
 * Initial revision
 *
 * Revision 2.15  2002/12/11 10:12:15  boe
 * add functions for activate/deactivate sync
 *
 * Revision 2.14  2002/11/18 09:58:18  boe
 * rework complete functionality
 * add comments/adapt on doxygen
 * add co_ to all global library variables
 * use functions for setting cob-id and timer values
 *
 * Revision 2.13  2002/05/30 12:14:59  hae
 * documentation correction
 *
 * Revision 2.12  2002/05/21 14:23:10  boe
 * cleanup new timer usage
 *
 * Revision 2.11  2002/03/26 08:25:46  boe
 * add NEW_TIMER functionality over defines
 *
 * Revision 2.10  2002/01/11 10:04:51  boe
 * make explicit cast for setting sync cob-id
 *
 * Revision 2.9  2001/05/23 10:13:57  boe
 * set internal sync cobid before call SET_COB_ID
 *
 * Revision 2.8  2001/04/05 12:27:01  boe
 * command line parameter for single line changed to void
 *
 * Revision 2.7  2001/04/05 08:45:56  boe
 * comment changed
 *
 * Revision 2.6  2001/03/29 14:34:26  boe
 * comment changed
 *
 * Revision 2.5  2001/02/26 14:14:51  boe
 * documentation format changed
 * driver access functions replaced by macros
 *
 * Revision 2.4  2001/01/26 12:13:59  boe
 * split include files into function specific headers
 *
 * Revision 2.3  2000/10/04 14:00:10  boe
 * defines for SYNC changed from CLIENT to CONSUMER and SERVER to PRODUCER
 *
 * Revision 2.2  2000/03/28 14:38:30  boe
 * adaption for multi-line version
 * sync producer mode changed
 *
 * Revision 2.1  2000/02/04 13:37:39  oe
 * - Changes for generating reference pages
 *
 * Revision 2.0  2000/01/21 11:04:37  boe
 * Überarbeitet und an Version 4.0 angepasst
 *
 *
 *
 *------------------------------------------------------------------
 */


/****************************************************************************/
/**
*  \file sync.c
*  \author port GmbH Halle (Saale)
*  $Revision$
*  $Date$
*
*++ This module contains the functions for SYNC Object handling for
*++ a SYNC producer or consumer.
*++ At the moment only one SYNC per
*++ physical CAN device can be defined.
*-- Diese Modul beinhaltet Funktionen für einen SYNC Producer
*-- oder Consumer.
*-- In der vorliegenden Version kann nur ein SYNC Objekt pro
*-- physischem CAN Gerät definiert werden.
*/


/* header of standard C - libraries */

#include <stdio.h>
#include <string.h>

/* project headers */

#include <cal_conf.h>
#include <co_odidx.h>
#include <co_cobid.h>
#include <co_flag.h>
#include "sync.h"
#include "access.h"
#include "nmt.h"
#include "drv.h"

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
RET_T activateSync(void);
void deActivateSync(void);

/* external variables
---------------------------------------------------------------------------*/

/* global variables
---------------------------------------------------------------------------*/
#if defined(CONFIG_SYNC_PRODUCER) || defined(CONFIG_SYNC_CONSUMER)
SYNC_T		*co_pSync ;      /* pointer to SYNC Object */
UNSIGNED32	co_cob_id_sync ;	/* COB-ID sync */
#endif /* defined(CONFIG_SYNC_PRODUCER) || defined(CONFIG_SYNC_CONSUMER) */
/* the variable coInternSyncPeriod is necessary, because the resolution of
   the internal time base (dTimerValue) is not equal
   the resolution of the communication cycle period */

/* local defined variables
---------------------------------------------------------------------------*/
#ifdef CONFIG_RCS_IDENT
static char _rcsid[] = "$Id$";
#endif /* CONFIG_RCS_IDENT */

#if defined(CONFIG_SYNC_PRODUCER) || defined(CONFIG_SYNC_CONSUMER)
/****************************************************************************/
/**
*++ \brief defineSync - define a SYNC Object
*-- \brief defineSync - definiert ein SYNC - Objekt
*
*++ This function defines a Synchronization Object (SYNC)
*++ for transmitting (PRODUCER) or receiving (CONSUMER).
*++ At the moment
*++ only one SYNC per physically CAN device is possible.
*++ The definition of a SYNC is necessary if the device
*++ should be able to be a SYNC producer or SYNC consumer.
*++ If the SYNC is only be used with the default COB-ID (128),
*++ it is not the necessary to define the object dictionary entry 0x1005
*++ (COB-ID SYNC)
*++ and the compiler define \c CONFIG_SYNC_COB_ID needs not to be set.
*-- Diese Funktion definiert ein Synchronisationsobjekt (SYNC)
*-- als Sendeobjekt oder Emfangsobjekt.
*-- Zur Zeit kann nur ein SYNC pro physischem CAN Gerät definiert werden.
*-- Die Definition eines SYNC's ist notwendig, wenn das Gerät
*-- Synchronisationssignale erzeugen oder empfangen soll.
*-- Falls das SYNC-Objekt nur mit der Standard COB-ID (128) genutzt wird, so
*-- ist der Objektverzeichniseintrag 0x1005 (COB-ID SYNC) nicht notwendig.
*-- Es darf dann die Compilerdirektive \c CONFIG_SYNC_COB_ID nicht gesetzt
*-- werden.
*
*++ \par example:
*-- \par Beispiel:
* \code
* RET_T retVal;
* 
* // consumer
* retVal = defineSync(CONSUMER);
* 
* // producer
* retVal = defineSync(PRODUCER);
* \endcode
*
* \retval CO_OK
*++ success
*-- Erfolg
* \retval CO_E_MEM
*++ memory allocation fault
*-- Speicherallozierungsfehler
* \retval CO_E_NO_ACCESS
*++ no access to object dictionary at index 0x1005
*-- kein Zugriff auf Objektverzeichnis mit Index 0x1005
*
*/

RET_T defineSync(
	CO_USER_T kindOfUse   /**< kind of using (PRODUCER/CONSUMER) */
	)
{
UNSIGNED32 	*pCobId;				/* pointer to COB-ID */
COB_KIND_T      kind;				/* kind of COB RX/TX */
RET_T		retVal;				/* return value */
UNSIGNED32      size;				/* size of object */
# ifdef CONFIG_SYNC_PRODUCER
UNSIGNED32  	*pU32;				/* temp buffer */
# endif /* CONFIG_SYNC_PRODUCER */

    if (kindOfUse == PRODUCER) {
# ifdef CONFIG_SYNC_PRODUCER
	kind = CO_COB_SYNC_PROD;
# else /* CONFIG_SYNC_PRODUCER */
	return(CO_E_TRANS_TYPE);
# endif /* CONFIG_SYNC_PRODUCER */
    } else {
	kind = CO_COB_SYNC_CONS;
    }

    if ((co_pSync  =
	    (SYNC_T *)CalMalloc(sizeof(SYNC_T))) == NULL) {
	return(CO_E_MEM);
    }

    /* define COB */
    co_pSync  ->pCobId =
	 DEFINE_COB(kind, 0 CO_COMMA_LINE_PARA);

    if (co_pSync  ->pCobId == NULL) {
	return(CO_E_NO_DATABASE);
    }

    co_pSync  ->state = CO_SYNC_STATE_INIT;

    /* sync not active */
# if defined(CONFIG_SYNC_PRODUCER)
    if (kindOfUse == PRODUCER) {
	co_pSync  ->state |= CO_SYNC_STATE_PRODUCER;

	/* get sync cycle periode */
	if (getObjAddr(COMM_CYCLE_INDEX, 0, (UNSIGNED8 **)&pU32, &size
		    CO_COMMA_LINE_PARA) != CO_OK) {
	    return CO_E_NO_ACCESS;
	}

	if ((retVal = setSyncTimePara(pU32 CO_COMMA_LINE_PARA)) != CO_OK)  {
	    return(retVal);
	}
    }
# endif /* defined(CONFIG_SYNC_PRODUCER) */

    /* get sync cobid */
    if (getObjAddr(SYNC_COB_ID_INDEX, 0, (UNSIGNED8 **)&pCobId, &size
		CO_COMMA_LINE_PARA) != CO_OK) {
	return CO_E_NO_ACCESS;
    }
    if ((retVal = setSyncCobId(pCobId CO_COMMA_LINE_PARA)) != CO_OK) {
	return(retVal);
    }

    return(CO_OK);
}


/****************************************************************************/
/*
*++ \brief setSyncCobId - set the sync cob-id for producer
*-- \brief setSyncCobId - setzt die Sync COB-Id für Producer
*
* NOMANUAL
*
*++ This function sets the sync cob-id.
*++ It checks before the allowed states for sync
*++ (Sync consumers can't produce sync...)
*
* \retval CO_OK
*++ success
*-- Erfolg
* \retval CO_E_TRANSTYPE
*++ bad transmission mode requested
*-- nicht erlaubter Transmission Mode angefordert
*
*/
RET_T setSyncCobId(
	UNSIGNED32 *pCobId		/* pointer to cob-id */
    )
{
    /* abort for extended identifiers */
    if ((*pCobId & CAN_29_BIT_ID_FLAG) != 0)  {
	return(CO_E_TRANS_TYPE);
    }

    /* check for sync producer */
    if ((*pCobId & SYNC_PRODUCER_BIT) != 0)  {
# ifdef CONFIG_SYNC_PRODUCER
	/* co_pSync  ->state |= CO_SYNC_STATE_ENABLED; */
	/* start sync */
	if (startSyncReq(CO_LINE_PARA) != CO_OK)  {
	    return(CO_E_TRANS_TYPE);
	}
# else /* CONFIG_SYNC_PRODUCER */
	return(CO_E_TRANS_TYPE);
# endif /* CONFIG_SYNC_PRODUCER */

    } else {
# ifdef CONFIG_SYNC_PRODUCER
	/* co_pSync  ->state &= ~CO_SYNC_STATE_ENABLED; */
	/* stop sync */
	if (stopSyncReq(CO_LINE_PARA) != CO_OK)  {
	    return(CO_E_TRANS_TYPE);
	}
# endif /* CONFIG_SYNC_PRODUCER */
    }

    co_cob_id_sync  = *pCobId  & CAN_11_BIT_ID_MASK;
    SET_COB_ID(co_pSync  ->pCobId, (UNSIGNED16)*pCobId);

    return CO_OK;
}

#endif /* SYNC CONSUMER + PRODUCER */


#if defined(CONFIG_SYNC_PRODUCER)

/****************************************************************************/
/**
*++ \brief startSyncReq - starts the transmission of SYNC to the consumer(s)
*-- \brief startSyncReq - startet das Senden der SYNC zu den SYNC-Consumern
*
*++ This function enables SYNC messages to be sent.
*++ The SYNC producer can be a NMT master or a NMT slave device.
*++ The start of SYNC should be done only in the
*++ state PRE_OPERATIONAL.
*-- Diese Funktion schaltet das Senden von SYNC Nachrichten ein.
*-- Der SYNC Produzent kann sowohl
*-- der NMT-Master als auch ein NMT-Slave sein.
*-- Der Start des SNYC sollte nur im
*-- Zustand PRE_OPERATIONAL ausgelöst werden.
* \par
*++ The boot up order for SYNC is:
*-- Der Boot Up Vorgang ist wie folgt:
*
* \code
* startSyncReq();              // starts SYNC message
* wait(mytime);                // wait for synchronisation
* NMT_StartRemoteNode_req(0);  // only if SYNC producer is a master
* \endcode
*
* \retval CO_OK
*++ success
*-- Erfolg
*
*/
RET_T startSyncReq(
	void
	)
{
UNSIGNED32	*pCobId;		/* pointer to cob-id */
UNSIGNED32	size;			/* temporary variable */
RET_T		retVal;			/* return value */

    assert(co_pSync  != NULL);

    /* check, if sync producer is enabled */
    if ((co_pSync  ->state & CO_SYNC_STATE_PRODUCER)
		== 0) {
	return(CO_E_RANGE);
    }

    /* check, if sync producer is active */
    if ((co_pSync  ->state & CO_SYNC_STATE_ENABLED)
		== 0)  {
	/* no, set it */
	if (getObjAddr(SYNC_COB_ID_INDEX, 0, (UNSIGNED8 **)&pCobId, &size
		CO_COMMA_LINE_PARA) != CO_OK)  {
	    return CO_E_NO_ACCESS;
	}
	/* set producer bit */
	*pCobId |= SYNC_PRODUCER_BIT;

	co_pSync  ->state |= CO_SYNC_STATE_ENABLED;
    }

    retVal = activateSync(CO_LINE_PARA);
    return(retVal);

}


/****************************************************************************/
/**
*++ \brief stopSyncReq - stop the transmission of SYNC to the consumer(s)
*-- \brief stopSyncReq - stopt das Senden des SYNC
*
*++ This function disables the transmission of SYNC messages.
*-- Diese Funktion stellt das Senden des SYNC Telegrammes ein.
*
* \retval CO_OK
*++ success
*-- Erfolg
*
*/

RET_T stopSyncReq(
	void
	)
{
UNSIGNED32	*pCobId;		/* pointer to cob-id */
UNSIGNED32	size;			/* object size */

    assert( co_pSync  !=  NULL);

    /* check, if sync producer is enabled */
    if ((co_pSync  ->state & CO_SYNC_STATE_ENABLED)
		!= 0)  {
	/* no, set it */
	if (getObjAddr(SYNC_COB_ID_INDEX, 0, (UNSIGNED8 **)&pCobId, &size
		CO_COMMA_LINE_PARA) != CO_OK)  {
	    return CO_E_NO_ACCESS;
	}
	/* set producer bit */
	*pCobId &= ~SYNC_PRODUCER_BIT;

	co_pSync  ->state &= ~CO_SYNC_STATE_ENABLED;
    }

    deActivateSync(CO_LINE_PARA);
    return(CO_OK);

}


/****************************************************************************/
/*
*++ \brief setSyncTimePara - set the sync mer paraid
*-- \brief setSyncTimePara - setzt die Sync Timer Wert
*
* NOMANUAL
*
*++ This function sets the sync timer value.
*++ if the sync is enabled, it starts it immediately.
*
* \retval CO_OK
*++ success
*-- Erfolg
* \retval CO_E_TRANSTYPE
*++ bad transmission mode requested
*-- nicht erlaubter Transmission Mode angefordert
*
*/
RET_T setSyncTimePara(
	UNSIGNED32	*pTimeVal		/* new time value */
    )
{
RET_T ret = CO_OK;

    /* save the new value */
    co_pSync  ->timer.timerVal = *pTimeVal / 100;

    /* if sync producer is enabled, set the new value and start sync */
    if ((co_pSync  ->state & CO_SYNC_STATE_ENABLED)
		!= 0)  {
	if (*pTimeVal != 0)  {
	    /* start sync */
	    /* ret = startSyncReq(CO_LINE_PARA); */
	    ret = activateSync(CO_LINE_PARA);
	} else {
	    /* stop sync */
	    /* ret = stopSyncReq(CO_LINE_PARA); */
	    deActivateSync(CO_LINE_PARA);
	}
    }
    return(ret);
}


/*******************************************************************/
/*
*++ activateSync - activate SYNC transmission
*-- activateSync - aktiviert SYNC Senden
*
* NOMANUAL
*
*++ This function activate the SYNC message transmission.
*-- Diese Funktion aktiviert das Senden der Synchronisationsnachricht (SYNC).
*
* \return 
*++ RET_T
*-- RET_T
*
*/

RET_T activateSync(
	void
	)
{
    if (addTimerEvent(&co_pSync  ->timer,
	co_pSync  ->timer.timerVal,
	CO_TIMER_TYPE_SYNC | CO_TIMER_TYPE_CYCLIC CO_COMMA_LINE_PARA)
	    != 0)  {
	return(CO_E_RANGE);
    }

    co_pSync  ->state |= CO_SYNC_STATE_ACTIVE;

    RESET_COLIB_FLAG(COFLAG_SYNC_RECEIVED);

    return(CO_OK);
}


/*******************************************************************/
/*
*++ deActivateSync - deactivate SYNC transmission
*-- deActivateSync - deaktiviert SYNC Senden
*
* NOMANUAL
*
*++ This function deactivate the SYNC message transmission.
*-- Diese Funktion deaktiviert das Senden der Synchronisationsnachricht (SYNC).
*
* \return 
*++ RET_T
*-- RET_T
*
*/

void deActivateSync(
	void
	)
{
    removeTimerEvent(&co_pSync  ->timer CO_COMMA_LINE_PARA);
    co_pSync  ->state &= ~CO_SYNC_STATE_ACTIVE;

    RESET_COLIB_FLAG(COFLAG_SYNC_RECEIVED);
}


/*******************************************************************/
/*
*++ writeSyncReq - sends a SYNC to the consumer(s)
*-- writeSyncReq - sendet ein SYNC zu dem(n) Konsumenten(n)
*
* NOMANUAL
*
*++ This function generates the SYNC message.
*-- Diese Funktion generiert eine Synchronisationsnachricht (SYNC).
*
* \return
*++ nothing
*-- nichts
*
*/

void writeSyncReq(
	void
	)
{

    /* prevents writeSync before defineSync was called */
    if (co_pSync  == NULL) {
        return;
    }

    TRANSMIT_COB(co_pSync  ->pCobId, NULL);

    /* activates local synchronous PDOs */
    SET_COLIB_FLAG(COFLAG_SYNC_RECEIVED);

}

#endif /* CONFIG_SYNC_PRODUCER */

/*______________________________________________________________________EOF_*/
