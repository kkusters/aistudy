/*
 *++ heartbt - additional heartbeat routines for a CANopen master
 *-- heartbt - Zusätzliche Heartbeat Routinen für einen CANopen Master
 *
 * Copyright (c) 2001-2002 port GmbH Halle (Saale)
 *------------------------------------------------------------------
 * $Header$
 *
 *------------------------------------------------------------------
 *
 * modification history
 * --------------------
 *
 * $Log$
 * Revision 1.1  2008-03-26 17:06:01+01  driet
 * <>
 *
 * Revision 1.0  2008-03-07 17:04:40+01  driet
 * Initial revision
 *
 * Revision 2.12  2003/06/13 13:26:35  boe
 * add include nmt_m.h
 *
 * Revision 2.11  2003/02/28 16:05:26  boe
 * correct multiline parameter for some function calls
 *
 * Revision 2.10  2002/11/15 09:42:34  boe
 * add comments/adapt on doxygen
 * save consumer settings in mFlags
 *
 * Revision 2.9  2002/05/29 11:42:43  hae
 * documentation correction
 *
 * Revision 2.8  2002/05/21 14:06:38  boe
 * cleanup new timer usage
 *
 * Revision 2.7  2002/03/26 08:11:26  boe
 * add NEW_TIMER functionality over defines
 *
 * Revision 2.6  2001/04/05 08:45:43  boe
 * comment changed
 *
 * Revision 2.5  2001/03/29 14:25:43  boe
 * comment changed
 *
 * Revision 2.4  2001/03/28 15:50:11  boe
 * include header files for master only if CONFIG_MASTER is set
 *
 * Revision 2.3  2001/03/28 12:45:25  boe
 * added functionality for SLAVE_PLUS
 *
 * Revision 2.2  2001/02/26 14:05:47  boe
 * documentation format changed
 *
 * Revision 2.1  2001/01/26 10:52:34  boe
 * functions for heartbeat usage
 *
 *
 *
 *
 *------------------------------------------------------------------
 */


/****************************************************************************/
/**
*  \file heartbt.c
*  \author port GmbH Halle (Saale)
*  $Revision$
*  $Date$
*
*++ Additional network  NMT services
*++ are defined in this module.
*++ They are necessary for the CANopen Communication Profile DS\ 301.
*++ Additionally it contains functions for
*++ parametrization, start and stop of Heartbeat for a Minimum Capability
*++ Master Device.
*++ \par
*++ All functions of this module are only valid for master applications.
*-- In diesem Modul sind zusätzliche NMT Dienste definiert.
*-- Diese Dienste sind notwendig für das CANopen Communication Profile DS\ 301.
*-- Weiterhin beinhaltet es Funktionen zur Parametrierung, Start und Stopp
*-- des HeartBeat für ein Minimum Capability Master Device.
*-- \par
*-- Alle Funktionen dieses Moduls sind nur für Masterapplikationen gültig.
*/

/* header of standard C - libraries */

#include <stdio.h>
#include <string.h>

/* header of project specific types */
#include <cal_conf.h>
#include <co_odidx.h>
#include "heartbt.h"
#include "nmt.h"
#include "nmterr.h"
#include "access.h"
#if defined(CONFIG_MASTER) || defined(CONFIG_SLAVE_PLUS)
# include "nmt_m.h"
#endif /* defined(CONFIG_MASTER) || defined(CONFIG_SLAVE_PLUS) */
#ifdef CONFIG_HEARTBEAT_CONSUMER
#include "nmt_m.h"
#endif /*CONFIG_HEARTBEAT_CONSUMER */

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

/* external variables
---------------------------------------------------------------------------*/

/* global variables
---------------------------------------------------------------------------*/

/* local defined variables
---------------------------------------------------------------------------*/
#ifdef CONFIG_RCS_IDENT
static char _rcsid[] = "$Id$";
#endif /* CONFIG_RCS_IDENT */


#ifdef CONFIG_HEARTBEAT_CONSUMER
/****************************************************************************/
/**
*
*++ \brief setHeartBeatTime -  set the heartbeat time for the master
*-- \brief setHeartBeatTime -  setzt das HeartBeat Zeitintervall beim Master
*
*++ This function sets the heartbeat time
*++ at a minimum boot up master,
*++ for a guarded slave device.
*++ This time is the basic time period which
*++ the master checks the connection to the device.
*-- Diese Funktion setzt das Zeitintervall für die HeartBeat Überwachung
*-- auf einem Master, der das Minimum Boot Up nutzt.
*-- Diese Zeit ist die Spanne in der der Master den jeweiligen
*-- Slave überwacht
*-- Gleichzeitig wird auch der entsprechende Eintrag im OV aktualisiert
*
* \retval CO_OK
*++ success
*-- Erfolg
* \retval CO_E_NOT_EXIST
*++ node or consumer communication object doesn't exist
*-- der gewählte Knoten existiert nicht oder kein Consumer Eintrag frei
* \retval CO_E_NONEXIST_OBJECT
*++ object doesnt exist
*-- Objekt existiert nicht
* \retval CO_E_NONEXIST_SUBINDEX
*++ subindex existiert nicht
*-- Subindex existiert nicht
* \retval CO_E_NO_WRITE_PERM
*++ object not writable
*-- Objekt nicht schreibbar
* \retval CO_E_VALUE_TO_LOW
*++ value to low
*-- Wert zu niedrig
* \retval CO_E_VALUE_TO_HIGH
*++ value to high
*-- Wert zu hoch
*
*/

RET_T setHeartBeatTime(
      UNSIGNED8  nodeId,    /**< node ID of heartbeat slave */
      UNSIGNED16 hbTime     /**< heartbeat time in ms */
      )
{
UNSIGNED32  tmpU32;     /* temp u32 val */
RET_T       retVal;     /* return value */
UNSIGNED8   subIndex;   /* subindex */

    /* exist a subentry for this node at ov */
    subIndex = findHeartBeatEntry(nodeId CO_COMMA_LINE_PARA);

    /* returns, if not subindex available */
    if (subIndex == 0)  {
    return(CO_E_NOT_EXIST);
    }

    /* write the value into object-dictionary */
    tmpU32 = ((UNSIGNED32)nodeId << 16) | hbTime;

    if ((retVal = putObj(HEARTBEAT_CON_INDEX, subIndex, (UNSIGNED8 *)&tmpU32,
        4, CO_TRUE CO_COMMA_LINE_PARA)) != CO_OK)  {
    return(retVal);
    }

    return(setHeartBeatConsumerTime(tmpU32, subIndex CO_COMMA_LINE_PARA));
}


/****************************************************************************/
/**
*
*++ \brief startHeartBeatReq -  start the heartbeat monitoring
*-- \brief startHeartBeatReq -  startet die HeartBeat Überwachung
*
*++ This function starts the Heartbeat monitoring,
*++ which is a minimum boot up master,
*++ for the specified Heartbeat slave.
*++ If \em nodeId is zero
*++ heartbeat to all configured producers in the network is started.
*-- Diese Funktion startet die HeartBeat Überwachung für den
*-- übergebenen Knoten.
*-- Das HeartBeat wird für alle Producer gestartet, wenn der
*-- Parameter \em nodeId Null ist.
*
* \retval CO_OK
*++ success
*-- Erfolg
* \retval CO_E_NOT_EXIST
*++ node doesn't exist
*-- der gewählte Knoten existiert nicht
*
*/

RET_T startHeartBeatReq(UNSIGNED8 nodeId) // node ID of Heart Beat producers
{
NODE_T     *pNode;   // pointer to node struct
UNSIGNED8  subIndex; // sub index

  // exist a NMT-entry for this node
  if ((pNode = NMT_NodeExist(nodeId CO_COMMA_LINE_PARA)) == NULL)
  {
    return(CO_E_NOT_EXIST);
  }

  // exist a subentry for this node at ov
  subIndex = findHeartBeatEntry(nodeId CO_COMMA_LINE_PARA);

  // returns, if not subindex available
  if (subIndex == 0)
  {
    return(CO_E_NOT_EXIST);
  }

  if (addTimerEvent(&pNode->timer, pNode->timer.timerVal, CO_TIMER_TYPE_HB_CONS CO_COMMA_LINE_PARA) != 0)
  {
    return(CO_E_RANGE);
  }

  pNode->mflags |= GUARDFLAG_HB_ACTIVE;
  pNode->mflags &= ~GUARDFLAG_NG_RECEIVED;

  return(CO_OK);
}


/****************************************************************************/
/**
*
*++ \brief stopHeartBeatReq -  stop the heartbeat monitoring
*-- \brief stopHeartBeatReq -  beendet die HeartBeat Überwachung
*
*++ This function stops the heartbeat monitoring,
*++ for the specified heartbeat slave.
*++ If \em nodeId is zero
*++ heartbeat monitoring of all configured producers in the network
*++ is stopped.
*-- Diese Funktion beendet die HeartBeat Überwachung für den
*-- übergebenen Knoten.
*-- Die HeartBeat-Überwachung
*-- wird für alle Producer beendet, wenn der
*-- Parameter \em nodeId Null ist.
*
* \retval CO_OK
*++ success
*-- Erfolg
* \retval CO_E_NOT_EXIST
*++ node doesn't exist
*-- der gewählte Knoten existiert nicht
*
*/

RET_T stopHeartBeatReq(
      UNSIGNED8 nodeId    /**< node ID of hearbeat producers */
      )
{
NODE_T      *pNode;     /* pointer to node struct */
UNSIGNED8   subIndex;   /* sub index */

    /* exist a NMT-entry for this node */
    if ((pNode = NMT_NodeExist(nodeId CO_COMMA_LINE_PARA)) == NULL)  {
    return(CO_E_NOT_EXIST);
    }

    /* exist a subentry for this node at ov */
    subIndex = findHeartBeatEntry(nodeId CO_COMMA_LINE_PARA);

    /* returns, if not subindex available */
    if (subIndex == 0)  {
    return(CO_E_NOT_EXIST);
    }

    pNode->mflags &= ~GUARDFLAG_HB_ACTIVE;

    removeTimerEvent(&pNode->timer CO_COMMA_LINE_PARA);

    return(CO_OK);
}

# if defined(CONFIG_MASTER) || defined(CONFIG_MASTER_PLUS) || defined(CONFIG_SLAVE_PLUS)

/*******************************************************************
*
* findHeartBeatEntry - searches for heartbeat entry at ov
*
* NOMANUAL
*
* This function checks the hb-consumer list at od (index 1016)
* and returns the index for the given node
* or the next free entry
*
* RETURNS
* \retval subIndex
*   subindex for th given node or next free
* \retval 0
*   no entry found and no more entries free
*
*/

UNSIGNED8 findHeartBeatEntry(
    UNSIGNED8 bNodeId   /* node id */
    )
{
UNSIGNED8   hbCnt;      /* subIndix count */
UNSIGNED32  size;       /* bytesize of objectentry */
UNSIGNED8   i;      /* loop variable */
UNSIGNED32  chbt;       /* consumer heartbeat entry */
UNSIGNED8   empty = 0;  /* flag variable */

    /* get max. count from od */
    if (getObjEntry(HEARTBEAT_CON_INDEX, 0, &hbCnt, &size, CO_TRUE
        CO_COMMA_LINE_PARA) != CO_OK)  {
        return(0);
    }

    /* get max. number of mapping objects */
    /* numOfElemnts are mapping + subindex 0 */
    hbCnt = getNumOfElem(HEARTBEAT_CON_INDEX CO_COMMA_LINE_PARA);

    for (i = 1; i < hbCnt; i++)  {
    /* get the entry */
    if (getObjEntry(HEARTBEAT_CON_INDEX, i, (UNSIGNED8 *)&chbt, &size,
        CO_TRUE CO_COMMA_LINE_PARA) != CO_OK)  {
        return(0);
    }
    /* test for valid node id entry */
    if (((UNSIGNED8)(chbt >> 16)) == bNodeId)  {
        return(i);
    }
    if (((chbt >> 16) == 0) && (empty == 0)) {
        empty = i;
    }
    }
    return(empty);
}
# endif  /* defined(CONFIG_MASTER) || defined(CONFIG_MASTER_PLUS) || defined(CONFIG_SLAVE_PLUS) */

#endif /* CONFIG_HEARTBEAT_CONSUMER */

/*______________________________________________________________________EOF_*/

