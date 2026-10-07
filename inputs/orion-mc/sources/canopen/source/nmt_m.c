/*
 *++ nmt_m - Network Management for Minimum Boot Up Master (Management Control)
 *-- nmt_m - Network Management für Minimum Boot Up Master (Management Control)
 *
 * Copyright (c) 2001-2002 port GmbH Halle (Saale)
 *------------------------------------------------------------------
 * $Header$
 *------------------------------------------------------------------
 *
 *
 * modification history
 * --------------------
 * $Log$
 * Revision 1.0  2008-03-07 17:04:42+01  driet
 * Initial revision
 *
 * Revision 2.19  2003/08/11 15:07:38  boe
 * don't remove remote node structures for heartbeat entries at removeRemoteNode
 * do this at removeNetworkReq
 *
 * Revision 2.18  2003/07/22 07:20:47  boe
 * change returnvalue for getRemoteNodeState() to UNKNOWN if no node found
 *
 * Revision 2.17  2003/06/13 13:31:06  boe
 * new function getRemoteNodeState() added
 *
 * Revision 2.16  2003/03/11 13:58:02  boe
 * change some cast values
 *
 * Revision 2.15  2003/02/28 16:07:34  boe
 * correct multiline usage for heartbeat consumers
 *
 * Revision 2.14  2002/11/18 09:31:20  boe
 * get more exact cob-type to driver (don't use SPECIAL_CHANNEL)
 *
 * Revision 2.13  2002/11/15 09:57:58  boe
 * add comments/adapt on doxygen
 * add co_ to all global library variables
 * change parameter list for createNetworkReq(), addRemoteNodeReq()
 * define network struct for all hb consumers as first
 *
 * Revision 2.12  2002/05/29 13:18:28  hae
 * documentation correction
 *
 * Revision 2.11  2002/05/21 14:11:33  boe
 * cleanup new timer usage
 * add led functionality
 *
 * Revision 2.10  2002/03/26 08:12:11  boe
 * add NEW_TIMER functionality over defines
 *
 * Revision 2.9  2001/09/14 04:26:11  boe
 * use special channel object for heartbeat consumers
 *
 * Revision 2.8  2001/08/22 08:11:36  boe
 * new function changeRemoteNodeReq() added
 *
 * Revision 2.7  2001/05/10 14:03:00  boe
 * adaption for new ds307 names (change master to manager)
 *
 * Revision 2.6  2001/04/05 12:25:38  boe
 * command line parameter for single line changed to void
 *
 * Revision 2.5  2001/04/05 08:45:47  boe
 * comment changed
 *
 * Revision 2.4  2001/03/29 14:31:09  boe
 * comment changed
 *
 * Revision 2.3  2001/03/28 12:46:51  boe
 * added functionality for SLAVE_PLUS
 *
 * Revision 2.2  2001/02/26 14:10:12  boe
 * documentation format changed
 * flying master functionality added
 * driver access functions replaced by macros
 *
 * Revision 2.1  2001/01/26 10:59:47  boe
 * functions for master nmt services
 *
 *
 *
 *
 *------------------------------------------------------------------
 */

 
/****************************************************************************/
/**
*  \file nmt_m.c
*  \author port GmbH Halle (Saale)
*  $Revision$
*  $Date$
*
*++ This module contains the functions for the Network Management Control
*++ for a master device.
*++ It contains only the functions for CANopen Minimum Boot Up
*++ master devices. 
*-- Dieses Modul beinhaltet Funktionen des Netzwerk Management Control Protokolls
*-- für einen Master.
*-- Es ist beschränkt auf Funktionen, die für das CANopen Minimum Boot Up 
*-- Master benötigt werden.
* \par
*++ All defined function are only valid on a master device.
*-- Alle definierten Funktionen sind nur für Masterapplikationen gültig.
*/


/* header of standard C - libraries */

#include <string.h>
#include <stdio.h>

/* header of project specific types */

#include <cal_conf.h>
#include <co_odidx.h>
#include <co_cobid.h>
#include "nmt_m.h"
#include "access.h"
#include "nmterr.h"
#include "heartbt.h"
#include "drv.h"

#ifdef CONFIG_FLYING_MASTER
# include "flyma.h"
#endif /* CONFIG_FLYING_MASTER */


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
RET_T NMT_Node_req(UNSIGNED8 remoteNodeId, NODE_STATE_T newState
		);
/* external variables
---------------------------------------------------------------------------*/

/* global variables
---------------------------------------------------------------------------*/

/* local defined variables
---------------------------------------------------------------------------*/
#ifdef CONFIG_RCS_IDENT
static char _rcsid[] = "$Id$";
#endif /* CONFIG_RCS_IDENT */

/*---- NMT-Services ----------------------------------------------*/

/*----------------------------------------------------------------*/
/*   MODULE CONTROL	CiA/DS203-1 p.9                           */
/*----------------------------------------------------------------*/

/* MASTER or Slave Plus */
#if defined(CONFIG_MASTER) || defined(CONFIG_SLAVE_PLUS)

/****************************************************************************/
/**
*++ \brief createNetworkReq - request the local service Create Network
*-- \brief createNetworkReq - fordert den lokalen Dienst Create Network an
*
*++ The NMT master needs internal structures for the management
*++ of the remote nodes.
*++ These structures contain guarding times and the current node state.
*++ This functions serves for initialisation of these structures.
*++ At the same time the master determines
*++ which Error Control Protocol is used.
*-- Der NMT-Master benötigt zur Verwaltung der Remoteknoten
*-- interne Strukturen,
*-- in denen er Überwachungszeiten und den aktuellen Knotenstatus
*-- der Remote-Knoten ablegt.
*-- Diese Funktion dient zur Initialisierung dieser Strukturen.
*-- Dem Master wird gleichzeitig mitgeteilt,
*-- welche Knotenüberwachungsfunktionen genutzt werden sollen.

*-- Die folgenden Werte sind möglich:
*++ The following values are possible:
*++ CO_TRUE - use NMT error mechanism
*-- CO_TRUE - benutze NMT Error Mechanismus
*++ CO_FALSE - don't use NMT error mechanism
*-- CO_FALSE - benutze NMT Error Mechanismus nicht

*++ When the Heartbeat-Consumer function is used
*++ addRemoteNodeReq()
*++ is called automatically for every subindex in object 1016.
*-- Bei der Verwendung der Heartbeat-Consumer Funktionalität
*-- werden für alle Einträge in der Heartbeat-Consumer Liste (Index 1016)
*-- automatisch die Funktion
*-- addRemoteNodeReq()
*-- aufgerufen.
*++ If the entries in object 1016 already contain valid data
*++ the function
*++ addRemoteNodeReq()
*++ doesn't have to be called for this nodes a second time.
*-- Wenn zum Aufruf der Funktion schon gültige Daten in der Liste eingetragen sind,
*-- muss die Funktion
*-- nicht nochmals für diese Knoten aufgerufen werden.

*++ This function has to be called for NMT-Slaves that
*++ want to use the heartbeat consumer functionality.
*-- Diese Funktion muss auch für NMT-Slaves aufgerufen werden,
*-- wenn sie die Heartbeat Consumer Funktionen nutzen wollen.

*
* \retval CO_OK
*++ success
*-- Erfolg
* \retval CO_E_MEM
*++ not enough memory
*-- nicht genug dyn. Speicher vorhanden
* \retval CO_E_ALREADY_EXIST
*++ network was already defined
*-- Netzwerk wurde bereits definiert, oder eigener Knoten ist nicht angelegt
* \retval CO_E_NONEXIST_OBJECT
*++ object doesnt exist
*-- Objekt existiert nicht
* \retval CO_E_NONEXIST_SUBINDEX
*++ subindex doesnt exist
*-- subindex existiert nicht
* \retval CO_E_NO_READ_PERM
*++ no read permission to object
*-- Objekt nicht lesbar
* \retval CO_E_NO_WRITE_PERM
*++ no write permission
*-- Objekt nicht schreibbar
* \retval CO_E_RANGE
*++ value range exceeded
*-- wertebereich überschritten
* \retval CO_E_TRANS_TYPE
*++ bad transmission type
*-- falscher sende mode
* \retval CO_E_VALUE_TO_LOW
*++ value to low
*-- Wert zu niedrig
* \retval CO_E_VALUE_TO_HIGH
*++ value to high
*-- Wert zu hoch
*
*/

RET_T createNetworkReq(
	BOOL_T	useNodeguarding,	/**< init node-guarding */
	BOOL_T	useHeartbeat		/**< init HeartBeat */
      )
{
# ifdef CONFIG_HEARTBEAT_CONSUMER
RET_T		ret;		/* return value */
UNSIGNED32	size;		/* size */
UNSIGNED8	i, hbCnt;	/* loop variables */
UNSIGNED32	chbt;		/* heartbeat consumer entry */
#endif /* CONFIG_HEARTBEAT_CONSUMER */

    /* are nodes already added, return */
    if (co_pNetwork  != NULL) {
	return(CO_E_ALREADY_EXIST);
    }

    /* if doesn't called createNodeReq(), return */
    if (co_pNode  == NULL) {
	return(CO_E_ALREADY_EXIST);
    }

    if (useHeartbeat == CO_TRUE)  {
	/* use heartbeat */
# ifdef CONFIG_HEARTBEAT_CONSUMER
	co_pNode  ->mflags |= GUARDFLAG_HB_POSSIBLE;

	/* all hb consumer entries from od are added automatically */
	/* get number of hb consumer entries from od */
	ret = getObjEntry(HEARTBEAT_CON_INDEX, 0, &hbCnt, &size, CO_TRUE
		CO_COMMA_LINE_PARA);
	if (ret != CO_OK)  {
	    return(ret);
	}
	for (i = 1; i <= hbCnt; i++)  {
	    /* add a nmt entry */
	    /* life time factor = 0xff is internal sign for hb consumer list */
	    if ((ret = addRemoteNodeReq(0, 0, 0xff, CO_TRUE CO_COMMA_LINE_PARA))
		    != CO_OK)  {
		return(ret);
	    }
	    /* get node id and hb time */
	    ret = getObjEntry(HEARTBEAT_CON_INDEX, i, (UNSIGNED8 *)&chbt,
		&size, CO_TRUE CO_COMMA_LINE_PARA);
	    if (ret != CO_OK)  {
		return(ret);
	    }
	    /* */
	    /* change the */
	    if ((ret = setHeartBeatConsumerTime(chbt, i CO_COMMA_LINE_PARA))
			!= CO_OK)  {
		return(ret);
	    }
	}
# else /* CONFIG_HEARTBEAT_CONSUMER */
	return(CO_E_TRANS_TYPE);
# endif /* CONFIG_HEARTBEAT_CONSUMER */
    }

    if (useNodeguarding == CO_TRUE)  {
# ifdef CONFIG_NODE_GUARDING
	/* is this node NMT master ? */
	if ((co_pNode  ->mflags & GUARDFLAG_MASTER)
		== 0)  {
	    /* Guarding only for NMT master allowed */
	    return(CO_E_TRANS_TYPE);
	}
	co_pNode  ->mflags |= GUARDFLAG_NG_POSSIBLE;
# else /* CONFIG_NODE_GUARDING */
	return(CO_E_TRANS_TYPE);
# endif /* CONFIG_NODE_GUARDING */
    }

    return(CO_OK);
}


/****************************************************************************/
/**
*++ \brief deleteNetworkReq - request the local service Delete Network
*-- \brief deleteNetworkReq - fordert den lokalen Dienst Delete Network an
*
*++ The NMT-Master deletes the network object.
*++ Before this service can be used all nodes must be deleted.
*-- Der NMT-Master beseitigt das Netzwerk Objekt.
*-- Vor Aufruf dieses Dienstes müssen alle Knoten aus dem Netzwerk
*-- entfernt sein.
*
* \return
*++ nothing
*-- nichts
*
*/

void deleteNetworkReq(
	void
     )
{
NODE_T *pNodeInUse,		/* pointer to node struct */
       *pOldNodeInUse = NULL;	/* pointer to last node struct */

    pNodeInUse = co_pNetwork ;

    while (pNodeInUse != NULL) {
	/* stop timer */
	removeTimerEvent(&pNodeInUse->timer CO_COMMA_LINE_PARA);

#if 0
	/* dealloc memory */
	if (pOldNodeInUse != NULL)  {
	    pOldNodeInUse->pNext = pNodeInUse->pNext;
	} else {
	    co_pNetwork  = pNodeInUse->pNext;
	}
	CalFree(pNodeInUse);
	pOldNodeInUse = pNodeInUse;
	pNodeInUse = pNodeInUse->pNext;
#else
	pOldNodeInUse = pNodeInUse->pNext;
	CalFree(pNodeInUse);
	pNodeInUse = pOldNodeInUse;
#endif
    }
}


/****************************************************************************/
/**
*++ \brief addRemoteNodeReq - request the service Add Remote Node.
*-- \brief addRemoteNodeReq - fordert den Dienst Add Remote Node an.
*
*++ This function registers the given node at the NMT-Master
*++ with the given Error Control Protocol
*++ As current node state
*++ PRE-OPRATIONAL is used.
*++ The parameter
*++ useHeartBeat
*++ determines which Error Control Protocol to use for the given node.
*-- Mit dieser Funktion wird ein Knoten für die Verwaltung
*-- durch den NMT-Master und/oder für die Knotenüberwachung eingetragen.
*-- Als aktueller Knotenstatus wird immer 
*-- PRE_OPERATIONAL
*-- eingetragen.
*-- Der Parameter
*-- useHeartBeat
*-- legt fest,
*-- mit welchem Knotenüberwachungsdienst

*++ At the same time the guarding times can be specified.
*++ Changes of guarding times can be fulfilled with the function
*++ changeRemoteNodeReq().
*-- der jeweilige Knoten zu überwachen ist.
*-- Gleichzeitig können die Überwachungszeiten eingestellt werden.
*-- Änderungen können über die Funktion
*-- changeRemoteNodeReq()
*-- erfolgen.

*++ This function must not be called for nodes that are not
*++ in an entry of object 1016.
*++ it is done with the function createNetworkReq().
*-- Diese Funktion muss nicht für Knoten aufgerufen werden,
*-- die in der Heartbeat Consumer Liste (Index 1016)
*-- eingetragen sind.
*-- Dies erfolgt automatisch in der Funktion
*-- createNetworkReq()
*
* \retval CO_OK
*++ success
*-- Erfolg
* \retval CO_E_NO_NETWORK
*++ no network object defined
*-- kein Netzwerkobjekt vorhanden
* \retval CO_E_MEM
*++ not enough memory
*-- nicht genug dyn. Speicher vorhanden
* \retval CO_E_RANGE
*++ not allowed module number 0
*-- Modulnummer 0 wurde angegeben
* \retval CO_E_ALREADY_EXIST
*++ Remote Node Object with this number was already defined
*-- Remote Node Objekt mit dieser Nummer existiert bereits
* \retval CO_E_NONEXIST_SUBINDEX
*-- Heartbeat Eintrag nicht möglich
*++ heartbeat not possible; subindex does not exist
* \retval CO_E_TRANS_TYPE
*++ bad transmission type
*-- falscher transmission type
*/

RET_T addRemoteNodeReq(
      UNSIGNED8  bNodeId,	 /**< Node ID 1..127 (CANopen) */
      UNSIGNED16 wGuardTime,     /**< guarding/heartbeat consumer time in ms */
      UNSIGNED8  bLifeTimeFactor,/**< life time factor */
      BOOL_T	 useHeartBeat	 /**< use heartbeat for this node */
     )
{
NODE_T *pNodeInUse,		/* pointer to node struct */
       *pOldNodeInUse;		/* pointer to last node struct */
RET_T	ret = CO_OK;		/* return value */

    assert(co_pNode  != NULL);

    /* if the own node, return */
    if (bNodeId == coNodeId )  {
	return(CO_OK);
    }

    /* node id 0 isn't allowed normally */
    if (bNodeId == 0)  {
	/* exception for entries from hb-consumer list
	   all this entries are added automatically by createNetworkReq()) */
	if ((useHeartBeat != CO_TRUE) || (bLifeTimeFactor != 0xff)) {
	    return(CO_E_RANGE);
	}
    } else  {

	/* if node already exist, abort function */
	if (NMT_NodeExist(bNodeId CO_COMMA_LINE_PARA) != NULL)  {
	    /* node found, return  */
	    return(CO_E_ALREADY_EXIST);
	}
    }

    /* request monitoring by heartbeat ? */
    if (useHeartBeat == CO_TRUE)  {
	/* is heartbeat monitoring allowed ? */
	if ((co_pNode  ->mflags & GUARDFLAG_HB_POSSIBLE)
		== 0) {
	    /* not possible, return */
	    return(CO_E_TRANS_TYPE);
	}

# ifdef CONFIG_HEARTBEAT_CONSUMER
	/* if this function is called from createNodereq(),
	 * then the nodeId = 0
	 * otherwise we have to search a free entry at the hb-consumer list
	 * (index 1016) and save the new value
	 * we don't have to create a new structure
	 * because all structures for hb consumers has been created
	 * by createNodeReq()
	 */
	if (bNodeId != 0) {
	    ret = setHeartBeatTime(bNodeId, wGuardTime CO_COMMA_LINE_PARA);
	    return(ret);
	}
# endif /* CONFIG_HEARTBEAT_CONSUMER */
    } else { 	/* request nodeguarding monitoring */

	/* is nodeguarding monitoring allowed ? */
	if ((co_pNode  ->mflags & 
		(GUARDFLAG_MASTER | GUARDFLAG_NG_POSSIBLE)) == 0) {
	    /* not possible, return */
	    return(CO_E_TRANS_TYPE);
	}
    }

    /* now the requested mode is possible */

    /* find the last entry at node-list  */
    pOldNodeInUse = co_pNetwork ;
    pNodeInUse = co_pNetwork ;
    while (pNodeInUse != NULL) {
	pOldNodeInUse = pNodeInUse;
	pNodeInUse = pNodeInUse->pNext;
    }

    /* alloc memory */
    if ((pNodeInUse = CalMalloc(sizeof(NODE_T))) == NULL) {
	return(CO_E_MEM);
    }
    /* and save it at the list */
    if (pOldNodeInUse != NULL)  {
	pOldNodeInUse->pNext = pNodeInUse;
    } else  {
	co_pNetwork  = pNodeInUse;
    }

    pNodeInUse->pNext = NULL;
    pNodeInUse->bNode_ID = bNodeId;
    pNodeInUse->eState = PRE_OPERATIONAL;
    /* pNodeInUse->timer.timerVal = (UNSIGNED32)wGuardTime * 10; */

    /* save monitor mode */
    /* request heartbeat ? */
    if (useHeartBeat == CO_TRUE)  {
# ifdef CONFIG_HEARTBEAT_CONSUMER
	pNodeInUse->mflags = GUARDFLAG_HB_POSSIBLE;
# endif /* CONFIG_HEARTBEAT_CONSUMER */
    } else {
# ifdef CONFIG_NODE_GUARDING
	pNodeInUse->mflags = GUARDFLAG_NG_POSSIBLE;
	/* pNodeInUse->bLifeTimeFactor = bLifeTimeFactor; */
	/* pNodeInUse->bSuspendedGuardings = 0; */
# endif /* CONFIG_NODE_GUARDING */
    }

    /* create cob entry for monitoring */
    pNodeInUse->pGuard_COB = 
    	DEFINE_COB(CO_COB_GUARD_MASTER, 1 CO_COMMA_LINE_PARA);

    if (pNodeInUse->pGuard_COB == NULL)  {
	return(CO_E_NO_DATABASE);
    }

    /* save the monitor values */
    if (bNodeId != 0)  {
	SET_COB_ID(pNodeInUse->pGuard_COB, (UNSIGNED16)(CO_COBID_NMTERR + bNodeId));
	ret = changeRemoteNodeReq(bNodeId, bNodeId, wGuardTime, bLifeTimeFactor
		CO_COMMA_LINE_PARA);
    }

    return(ret);
}


/****************************************************************************/
/**
*++ \brief removeRemoteNodeReq - request the service Remove Remote Node.
*-- \brief removeRemoteNodeReq - fordert den Dienst Remove Remote Node an.
*
*++ The NMT Master removes the Remote Node Object with the specified
*++ number (Node Id) from the list of defined Remote Node Objects.
*-- Der NMT-Master entfernt das Remote Node Object
*-- mit der angegebenen Node-Id
*-- aus der Liste der Remote Node Objects.
*
* \retval CO_OK
*++ success
*-- Erfolg
* \retval CO_E_NOT_EXIST
*++ Remote Node Object with this number does not exist
*-- Remote Node Object mit dieser Nummer bzw. diesem Namen existiert nicht
* \retval CO_E_STATE
*-- Remote Node Object nicht im Zustand PRE_OPERAIONAL
*++ Remote Node object is not PRE_OPERATIONAL
*
*/

RET_T removeRemoteNodeReq(
      UNSIGNED8 bMod_ID	 /**< Node ID 0..127 */
     )
{
NODE_T *pNodeInUse,		/* pointer to node struct */
       *pOldNodeInUse = NULL;	/* pointer to last node struct */

    pNodeInUse = co_pNetwork ;

    while (pNodeInUse != NULL) {
	if (bMod_ID == pNodeInUse->bNode_ID) {
	    /* node found, test state  */
	    if (pNodeInUse->eState == OPERATIONAL) {
		 return(CO_E_STATE);
	    }
	    /* stop timer */
	    removeTimerEvent(&pNodeInUse->timer CO_COMMA_LINE_PARA);

# ifdef CONFIG_HEARTBEAT_CONSUMER
	    /* if heartbeat active */
	    if ((pNodeInUse->mflags & GUARDFLAG_HB_POSSIBLE) != 0) {
		/* delete entry in OV */
		UNSIGNED8 subIndex;

		if ((subIndex = findHeartBeatEntry(bMod_ID CO_COMMA_LINE_PARA))
			!= 0)  {
		    UNSIGNED32 tmpU32;

		    /* write the value into object-dictionary */
		    tmpU32 = 0;

		    putObj(HEARTBEAT_CON_INDEX, subIndex, (UNSIGNED8 *)&tmpU32,
			4, CO_TRUE CO_COMMA_LINE_PARA);
		}
		/* delete node id */
		pNodeInUse->bNode_ID = 0;

		/* reset flags */
		pNodeInUse->mflags &= ~GUARDFLAG_HB_ACTIVE;
		
	    } else 
# endif /* CONFIG_HEARTBEAT_CONSUMER */
	    {

		/* delete flags for nmt */
		/* pNodeInUse->flags = 0; */

		/* dealloc memory */
		if (pOldNodeInUse != NULL)  {
		    pOldNodeInUse->pNext = pNodeInUse->pNext;
		} else {
		    co_pNetwork  = pNodeInUse->pNext;
		}
		CalFree(pNodeInUse);
	    }
	    return(CO_OK);
	}
	pOldNodeInUse = pNodeInUse;
	pNodeInUse = pNodeInUse->pNext;
    }
    return(CO_E_NOT_EXIST);
}


/****************************************************************************/
/**
*++ \brief changeRemoteNodeReq - request the service Change Remote Node.
*-- \brief changeRemoteNodeReq - fordert den Dienst Change Remote Node an.
*
*++ This function
*-- Diese Funktion dient zum Ändern von Daten der Remote-Knoten
*-- des NMT_Masters bzw. Heartbeat Consumers.
*-- Mit dieser Funktion können nur Knoten geändert werden,
*-- die in der Netzwerk-Struktur vorhanden sind.
*-- Sie müssen also vorher mit Hilfe der Funktion
*-- addRemoteNodeReq()
*-- eingerichtet worden sein.
*-- (Ausnahme bilden die Knoten,
*-- die in der Heartbeat Consumer Liste eingetragen sind).
*-- 
*-- Der Knotenüberwachungsmechanismus, der mit der Funktion
*-- addRemoteNode() festgelegt wurde,
*-- kann nicht geändert werden.
*-- 
*-- Wenn die Knotennummer geändert wurde,
*-- befindet sich der neue Knoten im Zustand
*-- PRE_OPERATIONAL:
*-- Die Knotenüberwachung wird inaktiv gesetzt.


* \retval CO_OK
*++ success
*-- Erfolg
* \retval CO_E_RANGE
*++ not allowed module number 0
*-- Modulnummer 0 wurde angegeben
* \retval CO_E_ALREADY_EXIST
*++ Remote Node Object with this number was already defined
*-- Remote Node Objekt mit dieser Nummer existiert bereits
* \retval CO_E_NONEXIST_SUBINDEX
*-- Heartbeat Eintrag nicht möglich
*++ Heartbeat not possible; missing subindex
* \retval  CO_E_TRANS_TYPE
*++ bad transmission type
*-- falscher sende mode
* \retval  CO_E_NONEXIST_OBJECT
*++ object doesnt exist
*-- Objekt existiert nicht
* \retval  CO_E_NO_WRITE_PERM
*++ object no writeable
*--  Objekt nicht schreibbar
* \retval  CO_E_VALUE_TO_LOW
*++ value to low
*-- Wert zu niedrig
* \retval  CO_E_VALUE_TO_HIGH
*++ value to high
*-- Wert zu hoch
*
*/

RET_T changeRemoteNodeReq(
      UNSIGNED8  oldNodeId,	 /**< Node ID 1..127 */
      UNSIGNED8  newNodeId,	 /**< Node ID 1..127 */
      UNSIGNED16 wGuardTime,     /**< guarding time in ms */
      UNSIGNED8  bLifeTimeFactor /**< life time factor */
     )
{
NODE_T		*pNodeInUse;	/* pointer to node struct */
# ifdef CONFIG_HEARTBEAT_CONSUMER
UNSIGNED8	subIndex;	/* subindex */
UNSIGNED32	chbt;		/* hb channel */
RET_T		ret;		/* return value */
# endif /* CONFIG_HEARTBEAT_CONSUMER */

    /* node id 0 isn't allowed */
    if ((oldNodeId == 0)  || (newNodeId == 0))  {
	return(CO_E_RANGE);
    }

    if (newNodeId == coNodeId )  {
	return(CO_E_RANGE);
    }

    assert(co_pNode  != NULL);

    /* if node doesn't exist, abort function */
    if ((pNodeInUse = NMT_NodeExist(oldNodeId CO_COMMA_LINE_PARA)) == NULL)  {
	/* Node not found */
	return(CO_E_ALREADY_EXIST);
    }

    if (oldNodeId != newNodeId)  {
	/* check, that the new node doesn't exist */
	if (NMT_NodeExist(newNodeId CO_COMMA_LINE_PARA) != NULL)  {
	    /* Node not found */
	    return(CO_E_ALREADY_EXIST);
	}
    }

    /* Now we have found a valid old entry */
    /* stop the timer */
    removeTimerEvent(&pNodeInUse->timer CO_COMMA_LINE_PARA);

    pNodeInUse->mflags &= ~(GUARDFLAG_HB_ACTIVE | GUARDFLAG_NG_ACTIVE);

# ifdef CONFIG_HEARTBEAT_CONSUMER
    if ((pNodeInUse->mflags & GUARDFLAG_HB_POSSIBLE) != 0)  {
	/* set entry at ov */
	/* first look if an entry present */
	if ((subIndex = findHeartBeatEntry(oldNodeId CO_COMMA_LINE_PARA)) == 0)  {
	    return(CO_E_NONEXIST_SUBINDEX);
	}
	chbt = ((UNSIGNED32)newNodeId << 16) + wGuardTime;
	if ((ret = putObj(HEARTBEAT_CON_INDEX, subIndex, (UNSIGNED8 *)&chbt,
		4, CO_TRUE CO_COMMA_LINE_PARA))
		!= CO_OK)  {
	    return(ret);
	}
	if ((ret = setHeartBeatConsumerTime(chbt, subIndex CO_COMMA_LINE_PARA))
		!= CO_OK)  {
	    return(ret);
	}
    }
# endif /* CONFIG_HEARTBEAT_CONSUMER */

    pNodeInUse->bNode_ID = newNodeId;
    pNodeInUse->eState = PRE_OPERATIONAL;

# ifdef CONFIG_NODE_GUARDING
    if ((pNodeInUse->mflags & GUARDFLAG_NG_POSSIBLE) != 0)  {

	pNodeInUse->bLifeTimeFactor = bLifeTimeFactor;
	pNodeInUse->bSuspendedGuardings = 0;
	pNodeInUse->timer.timerVal = (UNSIGNED32)wGuardTime * 10;
    }
#else /* CONFIG_NODE_GUARDING */
    bLifeTimeFactor;
#endif /* CONFIG_NODE_GUARDING */

    SET_COB_ID(pNodeInUse->pGuard_COB, (UNSIGNED16)(CO_COBID_NMTERR + newNodeId));

    return(CO_OK);
}



/*******************************************************************/
/*
* NMT_NodeExist - searches for a node existing in a list
*
* NOMANUAL
*
* \retval address
* pointer to node structure
* \retval  NULL
* not found
*
*/

NODE_T *NMT_NodeExist(
	UNSIGNED8 bMod_ID   /* node id */
	)
{
NODE_T *pNodeInUse;		/* pointer to node struct */

    pNodeInUse = co_pNetwork ;

    while (pNodeInUse != NULL) {
	if (pNodeInUse->bNode_ID == bMod_ID)  {
	    return(pNodeInUse);
	}
	pNodeInUse = pNodeInUse->pNext;
    }
    return(NULL);
}

#endif /* defined(CONFIG_MASTER) || defined(CONFIG_SLAVE_PLUS) */


#if defined(CONFIG_MASTER)

/****************************************************************************/
/*
*++ \brief NMT_Node_req - request one NMT service
*-- \brief NMT_Node_req - fordert einen NMT Dienst an.
*
* NOMANUAL
*
*++ The NMT-Slave referenced by the Node-ID
*++ 1..127 (CANopen) or 0 for all Slaves
*++ will be forced to the requested state
*
*-- Der NMT-Master versetzt den durch die Node-ID
*-- 1..127 (CANopen) oder 0 für alle
*-- angegebenen NMT-Slave
*-- und das dazugehörige Remote Node Object
*-- in den geforderten Zustand
*
* \retval CO_OK
*++ success
*-- Erfolg
* \retval CO_E_NOT_EXIST
*++ Remote Node Object with specified number doesn't exist
*++ (for module nummer > 0) or
*++ no Remote Node Object doesn't exist (for module nummer = 0)
*-- Remote Node Object mit dieser Nummer existiert nicht (für Modulnummer > 0)
*-- beziehungsweise es existiert kein Remote Node Object (für Modulnummer = 0)
* \retval CO_E_STATE
*++ The Remote Node Object is not in the state
*++ PRE_OPERATIONAL for module number > 0.
*++ There is a Remote Node Object which is not in the state
*++ PRE_OPERATIONAL for module number = 0.
*-- Das Remote Node Object ist nicht im Zustand
*-- PRE_OPERATIONAL für Modulnummer > 0,
*-- bzw. ein Remote Node Object ist nicht Zustand
*-- PRE_OPERATIONAL für Modulnummer = 0.
*
* INTERNAL
* changed globals: co_pNode->eState, pNodeInUse->eState
*/

RET_T NMT_Node_req(
      UNSIGNED8		remoteNodeId,	/* Node ID 0..127 (CANopen)*/
      NODE_STATE_T	newState	/* requested state */
     )
{
NODE_T		*pNodeInUse;		/* pointer to node struct */
CAN_MSG_T	canMsg;			/* CAN Message structure */

    switch (newState)  {
	case OPERATIONAL:
	    canMsg.pData[0] = CS_START_REMOTE_NODE;
	    break;
	case PRE_OPERATIONAL:
	    canMsg.pData[0] = CS_ENTER_PRE_OP_STATE;
	    break;
	case STOPPED:
	    canMsg.pData[0] = CS_STOP_REMOTE_NODE;
	    break;
	case RESET_APPLICATION:
	    canMsg.pData[0] = CS_RESET_APPLICATION;
	    newState = PRE_OPERATIONAL;
	    break;
	case RESET_COMM:
	    canMsg.pData[0] = CS_RESET_COMM;
	    newState = PRE_OPERATIONAL;
	    break;
	default:
	    return(CO_E_STATE);
    }

    canMsg.pData[1] = remoteNodeId;

    /* selective mode - only one node ? */
    if (remoteNodeId != 0) {

	/* local node ? */
	if (co_pNode  ->bNode_ID == remoteNodeId) {
	    /* yes */
	    setNodeState(newState CO_COMMA_LINE_PARA);

#ifdef CONFIG_CO_RUN_LED
	    /* updateNMTState_led(CO_LINE_PARA); */
#endif /* CONFIG_CO_RUN_LED */

#ifdef CONFIG_FLYING_MASTER
	    if (canMsg.pData[0] == CS_RESET_COMM) {
		/* start trigger timeslot */
		flyManagerStartWaitForTrigger(CO_LINE_PARA);	
	    }
#endif /* CONFIG_FLYING_MASTER */

	} else {

	    /* if node at network list ? */
	    if ((pNodeInUse = NMT_NodeExist(remoteNodeId CO_COMMA_LINE_PARA))
		    == NULL) {
		return(CO_E_NOT_EXIST);
	    }

	    /* send to remote slave */
	    pNodeInUse->eState = newState;
	    TRANSMIT_COB(co_pNMT_COB , canMsg.pData);
	}
	return(CO_OK);
    }

    /* for all remote nodes */
    TRANSMIT_COB(co_pNMT_COB , &canMsg.pData[0]);
    /* for local node */
    setNodeState(newState CO_COMMA_LINE_PARA);

#ifdef CONFIG_FLYING_MASTER
    if (canMsg.pData[0] == CS_RESET_COMM) {
	/* start trigger timeslot */
	flyManagerStartWaitForTrigger(CO_LINE_PARA);	
    }
#endif /* CONFIG_FLYING_MASTER */

    /* starting all existing slaves */
    pNodeInUse = co_pNetwork ;
    while (pNodeInUse != NULL) {
	pNodeInUse->eState = newState;
	pNodeInUse = pNodeInUse->pNext;
    }

#ifdef CONFIG_CO_RUN_LED
    /* updateNMTState_led(CO_LINE_PARA); */
#endif /* CONFIG_CO_RUN_LED */

    return(CO_OK);
}

/****************************************************************************/
/**
*++ \brief startRemoteNodeReq - request the service Start Remote Node.
*-- \brief startRemoteNodeReq - fordert den Dienst Start Remote Node an.
*
*++ The NMT-Slave referenced by the Node-ID
*++ 1..127 (CANopen) or 0 (for all slaves)
*++ will be forced to the state OPERATIONAL.
*++ The state of the slaves have be PRE_OPERATIONAL or STOPPED.
*++ In the case of Node-ID = 0 all slaves will be forced to OPERATIONAL.
*
*-- Der NMT-Master versetzt den durch die Node-ID
*-- 1..127 (CANopen) oder 0 (für alle)
*-- angegebenen NMT-Slave
*-- und das dazugehörige Remote Node Object
*-- in den Zustand OPERATIONAL.
*-- Der NMT-Slave muß sich im Zustand PRE_OPERATIONAL
*-- befinden.
*-- Ist die Node-ID gleich 0,
*-- so werden alle Knoten,
*-- die sich im Zustand STOPPED oder
*-- PRE_OPERATIONAL befinden
*-- in den Zustand OPERATIONAL versetzt (gestartet).
*
* \retval CO_OK
*++ success
*-- Erfolg
* \retval CO_E_NO_NETWORK
*++ Network Object not defined
*-- kein Netzwerkobjekt vorhanden
* \retval CO_E_NOT_EXIST
*++ Remote Node Object with specified number doesn't exist
*++ (for module nummer > 0) or
*++ no Remote Node Object doesn't exist (for module nummer = 0)
*-- Remote Node Object mit dieser Nummer existiert nicht (für Modulnummer > 0)
*-- beziehungsweise es existiert kein Remote Node Object (für Modulnummer = 0).
* \retval CO_E_STATE
*++ The Remote Node Object is not in the state PREPARED or
*++ PRE_OPERATIONAL (only CANopen) for module number > 0.
*++ There is no Remote Node Object which is in the state PREPARED or
*++ PRE_OPERATIONAL (only CANopen) for module number = 0.
*-- Das Remote Node Object ist nicht im Zustand PREPARED
*-- oder PRE_OPERATIONAL (nur CANopen) für Modulnummer > 0,
*-- bzw. ein Remote Node Object ist nicht Zustand PREPARED
*-- oder PRE_OPERATIONAL (nur CANopen) für Modulnummer = 0.
*
* \internal
* changed globals: co_pNode->eState, pNodeInUse->eState
*/

RET_T startRemoteNodeReq(
      UNSIGNED8 bMod_ID    /**< Node ID 0..127 (CANopen)*/
     )
{
    return(NMT_Node_req(bMod_ID, OPERATIONAL CO_COMMA_LINE_PARA));
}


/****************************************************************************/
/**
*++ \brief stopRemoteNodeReq - request the service Stop Remote Node.
*-- \brief stopRemoteNodeReq - fordert den Dienst Stop Remote Node an.
*
*++ The NMT-Slave referenced by the Node-ID
*++ 1..127 (CANopen) or 0 (for all Slaves)
*++ will be forced to the state STOPPED.
*++ The state of the slaves have to be OPERATIONAL or
*++ PRE_OPERATIONAL (only CANopen).
*++ In the case of Node-ID = 0 all slaves will be forced to the state
*++ STOPPED.
*
*-- Der NMT-Master versetzt den durch die Node-ID
*-- (1..127) oder 0 (für alle)
*-- angegebenen NMT-Slave
*-- und das dazugehörige Remote Node Object
*-- in den Zustand STOPPED.
*-- Der NMT-Slave muß sich im Zustand OPERATIONAL bzw.
*-- PRE_OPERATIONAL befinden.
*-- Ist die Node-ID gleich 0,
*-- so werden alle Knoten,
*-- die sich im Zustand OPERATIONAL bzw. PRE_OPERATIONAL
*-- befinden in den Zustand STOPPED versetzt.
*
* \retval CO_OK
*++ success
*-- Erfolg
* \retval CO_E_NOT_EXIST
*++ Remote Node Object with specified number doesn't exist
*++ (for module nummer > 0) or
*++ no Remote Node Object doesn't exist (for module nummer = 0)
*-- Remote Node Object mit dieser Nummer existiert nicht (für Modulnummer > 0)
*-- beziehungsweise es existiert kein Remote Node Object (für Modulnummer = 0)
* \retval CO_E_STATE
*++ The Remote Node Object is not in the state OPERATIONAL or
*++ PRE_OPERATIONAL (only CANopen) for module number > 0.
*++ There is no Remote Node Object which is in the state OPERATIONAL or
*++ PRE_OPERATIONAL (only CANopen) for module number = 0.
*-- Das Remote Node Object ist nicht im Zustand OPERATIONAL
*-- oder PRE_OPERATIONAL (nur CANopen) für Modulnummer > 0,
*-- bzw. ein Remote Node Object ist nicht Zustand OPERATIONAL
*-- oder PRE_OPERATIONAL (nur CANopen) für Modulnummer = 0.
*
* INTERNAL
* changed globals: co_pNode->eState, pNodeInUse->eState
*/

RET_T stopRemoteNodeReq(
      UNSIGNED8 bMod_ID    /**< Node ID 0..127 */
      )
{
    return(NMT_Node_req(bMod_ID, STOPPED CO_COMMA_LINE_PARA));
}



/****************************************************************************/
/**
*++ \brief enterPreOpStateReq - set NMT-Slave in PRE_OPERATIONAL state
*-- \brief enterPreOpStateReq - setzt NMT-Slave in den Zustand PRE_OPERATIONAL
*
*++ The NMT-Master sets the NMT-Slave with ID \em nodeID (1..127 or 0 for all)
*++ in the state PRE_OPERATIONAL.
*++ The service is unconfirmed
*++ and mandatory for devices which support dynamic PDO configuration.
*++ In the state PRE_OPERATIONAL the node can communicate only via SDOs.
*-- Der NMT-Master setzt den NMT-Slave mit der ID \em nodeID
*-- (1..127 oder 0 für alle) in den Zustand PRE_OPERATIONAL.
*-- Dieser Dienst wird nicht bestätigt und ist vorgeschrieben für
*-- Geräte, die eine dynamische PDO Konfiguration unterstützen.
*-- Im Zustand PRE_OPERATIONAL können die Knoten nur über SDO
*-- kommunizieren.
*
* \retval CO_OK
*++ success
*-- Erfolg
* \retval CO_E_NOT_EXIST
*++ the node with the choosen ID doesn't exist
*-- der gewählte Knoten mit dieser ID existiert nicht
* \retval CO_E_STATE
*++ the choosen node is in the wrong state
*-- der gewählte Knoten ist im falschen Zustand
*/

RET_T enterPreOpStateReq(
      UNSIGNED8 nodeId         /**< Identification number of the node */
      )
{
    return(NMT_Node_req(nodeId, PRE_OPERATIONAL CO_COMMA_LINE_PARA));
}


/****************************************************************************/
/**
*++ \brief resetCommReq - set NMT-Slave in RESET_COMM state
*-- \brief resetCommReq - setzt den NMT-Slave in den RESET_COMM Zustand
*
*++ The NMT-Master sets the NMT-Slave with ID \em nodeID (1..127 or 0 for all)
*++ in the state RESET_COMM.
*++ In this state the parameters in the communication area of the object
*++ directory are set to their default values.
*++ The state is temporary only.
*++ That means the reached state will change automatically to
*++ PRE_OPERATIONAL afterwards.
*++ This service is unconfirmed and mandatory for all devices.
*-- Der NMT-Master setzt den NMT-Slave mit der ID \em nodeID
*-- (1..127 oder 0 für alle) in den Zustand RESET_COMM.
*-- In diesem Zustand werden die Kommunikationsparameter des Knotens
*-- zurückgesetzt.
*-- Der Zustand ist nur temporär, d.h. der Knoten geht in den Zustand
*-- PRE_OPERATIONAL automatisch über.
*-- Dieser Dienst wird nicht bestätigt und ist vorgeschrieben für
*-- alle Geräte.
*
* \retval CO_OK
*++ success
*-- Erfolg
* \retval CO_E_NOT_EXIST
*++ the node with the choosen ID doesn't exist
*-- der gewählte Knoten existiert nicht
*/

RET_T resetCommReq(
      UNSIGNED8  nodeId  /**< Identificationsnumber of the node */
      )
{
    return(NMT_Node_req(nodeId, RESET_COMM CO_COMMA_LINE_PARA));
}


/****************************************************************************/
/**
*++ \brief resetNodeReq - reset the application of the NMT-Slave
*-- \brief resetNodeReq - setzt die Applikation des NMT-Slave zurück
*
*++ The NMT-Master sets the NMT-Slave with ID \em nodeID (1..127 or 0 for all)
*++ in the state RESET_APPLICATION.
*++ In this state a reset of the node application will be performed.
*++ The state is temporary only.
*++ That means the reached state will change automatically to
*++ PRE_OPERATIONAL afterwards.
*++ The service is unconfirmed and mandatory for all devices.
*-- Der NMT-Master setzt den NMT-Slave mit der ID \em nodeId
*-- (1..127 oder 0 für alle) in den Zustand RESET_APPLICATION.
*-- In diesem Zustand wird die Applikation des Knotens zurückgesetzt.
*-- Der Zustand ist nur temporär, d.h. der Knoten geht in den Zustand
*-- PRE_OPERATIONAL automatisch über.
*-- Dieser Dienst wird nicht bestätigt und ist vorgeschrieben für
*-- alle Geräte.
*
* \retval CO_OK
*++ success
*-- Erfolg
* \retval CO_E_NOT_EXIST
*++ the node with the choosen ID doesn't exist
*-- der gewählte Knoten existiert nicht
*/

RET_T resetNodeReq(
      UNSIGNED8  nodeId  /**< Identificationsnumber of the node */
      )
{
    return(NMT_Node_req(nodeId, RESET_APPLICATION CO_COMMA_LINE_PARA));
}


/****************************************************************************/
/**
*++ \brief getRemoteNodeState - get last received node state
*-- \brief getRemoteNodeState - liefert den letzten empfangenen Knotenstatus
*
*++ This function get the node state of remote nodes
*++ from the last received heartbeat or nodegurading message.
*++ This function works only for nodes,
*++ they are monitoring from the own node.
*++ If the node isn't found the function returns 0.
*-- Diese Funktion liefert den aktuellen Knotenstatus
*-- von Remote Knoten,
*-- für die eine Knotenüberwachung auf dem lokalen Knoten eingerichtet wurde.
*-- Zurückgeliefert wird der Zustand des letzten empfangenen Heartbeats
*-- bzw. Nodeguardings.
*-- Falls der Knoten nicht in der Netzwerkstruktur gefunden wird,
*-- liefert die Funktion eine 0 zurück.
*
* \retval NODE_STATE_T
*++ success
*-- Erfolg
* \retval UNKNOWN
*++ the node with the choosen ID doesn't exist
*-- der gewählte Knoten existiert nicht
*/
NODE_STATE_T getRemoteNodeState(
	UNSIGNED8  nodeNr	/**< node number */
    )
{
NODE_T	*pNode;				/* pointer to node */

    pNode = NMT_NodeExist(nodeNr CO_COMMA_LINE_PARA);
    if (pNode == NULL)  {
	/* Node not found */
	return(UNKNOWN);
    }

    return(co_pNode  ->eState);
}

#endif  /* CONFIG_MASTER */

/*______________________________________________________________________EOF_*/
