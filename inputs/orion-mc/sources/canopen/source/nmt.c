/*
 *++ nmt - Network Management for Minimum Boot Up (Module Control)
 *-- nmt - Network Management für Minimum Boot Up (Module Control)
 *
 * Copyright (c) 2001-2002 port GmbH Halle (Saale)
 *------------------------------------------------------------------
 * $Header$*
 *------------------------------------------------------------------
 *
 *
 * modification history
 * --------------------
 * $Log$
 * Revision 1.3  2008-03-31 09:30:22+02  driet
 * <>
 *
 * Revision 1.2  2008-03-28 10:31:34+01  driet
 * <>
 *
 * Revision 1.1  2008-03-26 17:06:02+01  driet
 * <>
 *
 * Revision 1.0  2008-03-07 17:04:41+01  driet
 * Initial revision
 *
 * Revision 2.21  2003/07/08 13:29:47  boe
 * don't send bootup and don't change node state, if nodeid=255 and lss is used
 *
 * Revision 2.20  2003/06/13 13:28:59  boe
 * correct toggle bit handling for nodeguarding and fullcan
 *
 * Revision 2.19  2003/01/27 09:32:16  boe
 * srdo (safety functions) extended for multiline
 *
 * Revision 2.18  2002/12/11 07:55:40  boe
 * correct setting of lifetime factor value
 *
 * Revision 2.17  2002/11/15 09:48:56  boe
 * add comments/adapt on doxygen
 * add co_ to all global library variables
 * add NMT COB definittion to createNodeReq()
 * new function setNodeState()
 *
 * Revision 2.16  2002/05/30 14:15:53  hae
 * documentation correction
 *
 * Revision 2.15  2002/05/29 12:47:14  hae
 * documentation correction
 *
 * Revision 2.14  2002/05/21 14:10:39  boe
 * cleanup new timer usage
 * add led functionality
 *
 * Revision 2.13  2002/03/26 08:19:02  boe
 * add NEW_TIMER functionality over defines
 * add lss services
 * add new function setNodePreop()
 *
 * Revision 2.12  2002/02/26 16:26:17  boe
 * add define CONFIG_DS301_V30 for U32 nodeguarding entries
 *
 * Revision 2.11  2001/05/17 09:22:42  boe
 * explicite type conversion for constants to remove compiler warnings
 *
 * Revision 2.10  2001/05/10 14:02:43  boe
 * adaption for new ds307 names (change master to manager)
 *
 * Revision 2.9  2001/04/05 12:25:03  boe
 * command line parameter for single line changed to void
 *
 * Revision 2.8  2001/04/05 08:45:46  boe
 * comment changed
 *
 * Revision 2.7  2001/03/29 14:29:36  boe
 * comment changed
 *
 * Revision 2.6  2001/03/29 06:10:59  boe
 * use master capabilities only if CONFIG_MASTER is set
 *
 * Revision 2.3  2001/03/28 12:55:40  boe
 * define NMT COB only for master as RTR
 *
 * Revision 2.2  2001/02/26 14:08:51  boe
 * documentation format changed
 * flying master functionality added
 * driver access functions replaced by macros
 *
 * Revision 2.1  2001/01/26 10:59:02  boe
 * functions for nmt services
 *
 *
 *
 *------------------------------------------------------------------
 */


/****************************************************************************/
/**
*  \file nmt.c
*  \author port GmbH Halle (Saale)
*  $Revision$
*  $Date$
*
*++ This module contains the functions for the Network Management Control protocol.
*++ It contains only the functions for CANopen Minimum Boot Up
*++ slave devices. 
*-- Dieses Modul beinhaltet Funktionen des Network Management Control Protokolls.
*-- Es ist beschränkt auf Funktionen,
*-- die für das CANopen Minimum Boot Up 
*-- für Slaves benötigt werden.
*
*-- Folgende Kombinationen sind möglich
*++ The following combinations are possible.
*
*\code
*       Slave   Master
* NMT_Master        x
* Nodeguarding      o
* Lifeguarding  o
* HB Producer   o   o
* HB Consumer   o   o
*
*-- x - zwingend
*++ x - mandatory
* o - optional
*\endcode
*
*/


/* header of standard C - libraries */

#include <string.h>
#include <stdio.h>

/* header of project specific types */

#include <cal_conf.h>
#include <co_odidx.h>
#include <co_cobid.h>
#include "nmt.h"
#include "nmt_s.h"
#include "nmterr.h"
#include "access.h"
#include "drv.h"
#if defined(CONFIG_MASTER) || defined(CONFIG_SLAVE_PLUS)
# include "nmt_m.h"
#endif /* defined(CONFIG_MASTER) || defined(CONFIG_SLAVE_PLUS) */

#ifdef CONFIG_FLYING_MASTER
#include "flyma.h"
#endif /* CONFIG_FLYING_MASTER */

#if defined(CONFIG_SRDO_CONSUMER) || defined(CONFIG_SRDO_PRODUCER)
#include "srdo.h"
#endif /* defined(CONFIG_SRDO_CONSUMER) || defined(CONFIG_SRDO_PRODUCER) */

#ifdef CONFIG_LSS_SLAVE
# include "lss.h"
#endif /* CONFIG_LSS_SLAVE */

#ifdef CONFIG_CO_LED
# include "led.h"
#endif /* CONFIG_CO_RUN_LED */

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
/* pointer to local node structure */
NODE_T      *co_pNode   ;
COB_T       *co_pNMT_COB    ;

/* pointer to local network structure */
#if defined(CONFIG_MASTER) || defined(CONFIG_SLAVE_PLUS) || defined(CONFIG_HEARTBEAT_CONSUMER)
NETWORK_T   *co_pNetwork    ;
#endif /* defined(CONFIG_MASTER) || defined(CONFIG_SLAVE_PLUS) */

/* local defined variables
---------------------------------------------------------------------------*/
#ifdef CONFIG_RCS_IDENT
static char _rcsid[] = "$Id$";
#endif /* CONFIG_RCS_IDENT */



/*******************************************************************
*
* H_NMT_NodeStartStopMsg - eval the NMT messages at the node
*
* NOMANUAL
*
* nothing
*
*/
void NMT_NodeStartStopMsg(CAN_MSG_T *canMsg) // Pointer to CAN Message
{
NODE_STATE_T newState; // temporary state

  // NMT message for all nodes or only for this node ?
  if ((canMsg->pData[1] != co_pNode->bNode_ID) && (canMsg->pData[1] != 0))
  {
    return;
  }

#ifdef CONFIG_LSS_SLAVE
  setLssState(0 CO_COMMA_LINE_PARA);
#endif // CONFIG_LSS_SLAVE

  switch (canMsg->pData[0])
  {
    case CS_START_REMOTE_NODE :
      newState = OPERATIONAL;
      break;

    case CS_ENTER_PRE_OP_STATE :
      newState = PRE_OPERATIONAL;
      break;

    case CS_RESET_APPLICATION :
      newState = RESET_APPLICATION;
      break;

    case CS_RESET_COMM :
      newState = RESET_COMM;
      break;

    case STOPPED:
    default:
      newState = STOPPED;
      break;
  }
  setNodeState(newState CO_COMMA_LINE_PARA);
}


/*******************************************************************
*
* setNodeState - set the requested NMT state for this node
*
* NOMANUAL
*
* This function sets the requested NMT state for this node.
* and calls the necessary functions and the user indication.
*
* \retval
*   nothing
*
*/
void setNodeState(NODE_STATE_T newState) // new NMT state
{
#if defined(CONFIG_FULLCAN)
UNSIGNED8 pData[1]; // temporary transmit buffer
#endif // CONFIG_FULLCAN

#ifdef CONFIG_LSS_SLAVE
  // don't change the state, if we don't have a valid node-id
  if (coNodeId  == 255)
  {
    return;
  }
#endif // CONFIG_LSS_SLAVE

  if (newState == RESET_APPLICATION)
  {
    co_pNode->eState = RESET_APPLICATION;
    resetNodeMsg(CO_LINE_PARA);
    newState = co_pNode->eState;
  }

  if (newState == RESET_COMM)
  {
#ifdef CONFIG_FLYING_MASTER
    // start trigger timeslot
    flyManagerStartWaitForTrigger(CO_LINE_PARA);
#endif // CONFIG_FLYING_MASTER
    co_pNode->eState = RESET_COMM;
    resetCommMsg(CO_LINE_PARA);
    newState = co_pNode->eState;
  }

  // if no new state
  if (newState != co_pNode->eState)
  {
#if defined(CONFIG_SRDO_CONSUMER) || defined(CONFIG_SRDO_PRODUCER)
    if (newState == OPERATIONAL)
    {
      if (srdoGoOperational(CO_LINE_PARA) == CO_OK)
      {
        if (CO_TRUE == newStateInd(newState CO_COMMA_LINE_PARA))
        {
          co_pNode->eState = newState;
        }
      }
    }
    else
    {
      // not OPERATIONAL - delete all srdo timer events
      srdoGoPreop(CO_LINE_PARA);
      newStateInd(newState CO_COMMA_LINE_PARA);
      co_pNode->eState = newState;
    }
#else // defined(CONFIG_SRDO_CONSUMER) || defined(CONFIG_SRDO_PRODUCER)
    if (newStateInd(newState CO_COMMA_LINE_PARA) == CO_FALSE)
    {
      // don't change to OPERATIONAL
      if (newState != OPERATIONAL)
      {
        co_pNode->eState = newState;
      }
    }
    else
    {
      co_pNode->eState = newState;
    }
#endif // defined(CONFIG_SRDO_CONSUMER) || defined(CONFIG_SRDO_PRODUCER)
  }

#if defined(CONFIG_FULLCAN)
  pData[0] = (UNSIGNED8)co_pNode->eState
# if defined(CONFIG_NODE_GUARDING)
   // toggle bit is already set to next state, invert it here
   | ((UNSIGNED8)(co_pNode->bGuardToggle) ^ 0x80)
# endif // defined(CONFIG_NODE_GUARDING)
   ;
  UPDATE_COB(co_pNode->pGuard_COB, pData);
#endif // CONFIG_FULLCAN

#ifdef CONFIG_CO_RUN_LED
  updateNMTState_led(CO_LINE_PARA);
#endif /* CONFIG_CO_RUN_LED */
}


/****************************************************************************/
/**
*++ \brief createNodeReq - request the service Create Node.
*-- \brief createNodeReq - fordert den Dienst Service Create Node an.
*
*++ This function creates the internal structures for a CANopen node
*++ with the demanded error control services.
*++ At minimum 1 service is mandatory.
*-- Diese Funktion legt die internen Strukturen für einen CANopen-Knoten
*-- mit den geforderten Überwachungsdiensten an.
*-- Mindestens 1 Überwachungsdienst muss eingerichtet werden.
*
*-- Weiterhin wird das NMT object mit der COB-ID 0 angelegt.
*-- Ob der Knoten als NMT-Master oder NMT-Slave arbeiten soll, 
*-- kann über den Parameter
*++ Furthermore the NMT object with node-id 0 is created.
*++ The operating mode (master or slave) is setup by the parameter
* master
*-- festgelegt werden.
*++ Depending on this parameter the NMT object is created as
*++ receive or transmit object.
*-- Dementsprechend wird das NMT-Objekt auch als Sende- oder Empfangsobjekt
*-- angelegt.
*-- Der Parameter ist nur für MASTER, SLAVE_PLUS oder Multi-Line
*-- Knoten notwendig.
*++ This parameter is only necessary for MASTER, SLAVE_PLUS or Multi-Line nodes.
*-- Falls als CAN_Hardware ein Full-CAN Controller genutzt wird,
*-- wird von das NMT-Objekt auch ein Hardwarekanal belegt.
*++ This function call creates the NMT object with COB-ID 0.
*++ In Full CAN controllers also the hardware object for NMT
*++ is initialized.
*
*-- Am Ende dieser Funktion befindet sich der Knoten im Zustand
*-- PRE_OPERATIONAL.
*++ The Node Object's state is PRE_OPERATIONAL after calling this function.
*
*\code
*   NMT-Error Object
*   NG  HB-Prod Master  RTR_Tx  Tx
*   0   1   0       x
*   0   1   1       x
*   1   0   0   x
*   1   0   1   
*   1   1   0   x   (x)
*   1   1   1       x
*
*\endcode
* \retval CO_OK
*++ success
*-- Erfolg
* \retval CO_E_MEM
*++ not enough memory
*-- nicht genug dyn. Speicher vorhanden
* \retval CO_E_ALREADY_EXIST
*++ Remote Node already exists
*-- Remote Node existiert bereits
* \retval CO_E_NO_ACCESS
*++ no access to the Object Dictionary (node ID and node-guarding parameters)
*-- kein Zugriff aud das Objektverzeichnis (node ID und Nodeguardingparameter)
* \retval CO_E_NO_INITIATE
*++ no error control service requested (node-guarding or heartbeat is mandatory)
*-- kein Error Control Service angefordert (Nodeguarding oder Heartbeat
*-- ist erforderlich)
*
*/

RET_T createNodeReq(
    BOOL_T  defNodeguarding,    /**< init node-guarding */
    BOOL_T  defHeartbeat        /**< init Heart-Beat */
#if defined(CONFIG_MASTER) || defined(CONFIG_SLAVE_PLUS)
    ,BOOL_T master          /**< is master */
#endif /* defined(CONFIG_MASTER) || defined(CONFIG_SLAVE_PLUS) */
    )
{
UNSIGNED16  tmpU16;     /* temp u16 val */
UNSIGNED32  size;       /* size of object */
UNSIGNED8   pData[1];   /* temporary variable */
#ifdef CONFIG_NODE_GUARDING
UNSIGNED8   tmpU8;      /* temp u8 val */
#endif /* CONFIG_NODE_GUARDING */
#ifdef CONFIG_DS301_V30
UNSIGNED32  tmpU32;     /* temp u32 variable */
#endif /* CONFIG_DS301_V30 */

    /* node already exists ? */
    if (co_pNode  != NULL)  {
    return(CO_E_ALREADY_EXIST);
    }

    /* test if at least one error service is requested */
    if ((defNodeguarding == CO_FALSE) && (defHeartbeat == CO_FALSE))  {
    return(CO_E_NO_INITIATE);
    }

    if ((co_pNode  = (NODE_T*)CalMalloc(sizeof(NODE_T)))
        == NULL) {
    return(CO_E_MEM);
    }

    co_pNode  ->bNode_ID =
    coNodeId ;
    co_pNode  ->flags = 0;

    /*---- initialize cobs for nmt -----------------------------------*/
#if defined(CONFIG_MASTER) || defined(CONFIG_SLAVE_PLUS)
    if (master == CO_TRUE) {
    co_pNMT_COB  =
        DEFINE_COB(CO_COB_NMT_MASTER, 2 CO_COMMA_LINE_PARA);
    /* save master flag */
    co_pNode  ->mflags |= GUARDFLAG_MASTER;
    } else 
#endif /* defined(CONFIG_MASTER) || defined(CONFIG_SLAVE_PLUS) */
    {
    co_pNMT_COB  =
            DEFINE_COB(CO_COB_NMT_SLAVE, 2 CO_COMMA_LINE_PARA);
    }

    if (co_pNMT_COB  == NULL)  {
    return(CO_E_NO_DATABASE);
    }

    SET_COB_ID(co_pNMT_COB , CO_COBID_NMT);

    /*---- initialize cobs for nmterr -----------------------------------*/
    co_pNode  ->pGuard_COB =
#ifdef CONFIG_NODE_GUARDING
        DEFINE_COB(CO_COB_GUARD_SLAVE, 1 CO_COMMA_LINE_PARA);
#else /* CONFIG_NODE_GUARDING */
        DEFINE_COB(CO_COB_HB_PROD, 1 CO_COMMA_LINE_PARA);
#endif /* CONFIG_NODE_GUARDING */
    if (co_pNode  ->pGuard_COB == NULL)  {
    return(CO_E_NO_DATABASE);
    }
    SET_COB_ID(co_pNode  ->pGuard_COB,
        (UNSIGNED16)(CO_COBID_NMTERR
            + (UNSIGNED16)coNodeId ));

    if (defNodeguarding == CO_TRUE)  {

#ifdef CONFIG_NODE_GUARDING
# if defined(CONFIG_MASTER) || defined(CONFIG_SLAVE_PLUS)
    /* define cob only if we are not the nodeguarding master */
    if (master != CO_TRUE)
# endif /* defined(CONFIG_MASTER) || defined(CONFIG_SLAVE_PLUS) */
    {
        /* lifeguarding is possible */
        co_pNode  ->flags |= GUARDFLAG_NG_POSSIBLE;

        /* request nodeguarding slave */

# ifdef CONFIG_FULLCAN
        /* preset guarding channel */
        pData[0] = (UNSIGNED8) co_pNode  ->eState;
        UPDATE_COB(co_pNode  ->pGuard_COB,
            &pData[0]);
# endif /* CONFIG_FULLCAN */

        /* toggled to 1 after first guarding */
        co_pNode  ->bGuardToggle = 0;
        co_pNode  ->bSuspendedGuardings = 0;

        /* get Guarding Time from od */
        /* get value from object dictionary */
# ifdef CONFIG_DS301_V30
        /* use 32bit entries from DS301 V30 */
        if (getObjEntry(GUARD_TIME_INDEX, 0,
        (UNSIGNED8 *)&tmpU32, &size, CO_TRUE CO_COMMA_LINE_PARA)
            != CO_OK) {
        return CO_E_NO_ACCESS;
        }
        tmpU16 = (UNSIGNED16)tmpU32;
# else /* CONFIG_DS301_V30 */
        if (getObjEntry(GUARD_TIME_INDEX, 0, (UNSIGNED8 *)&tmpU16, &size,
            CO_TRUE CO_COMMA_LINE_PARA) != CO_OK) {
        return CO_E_NO_ACCESS;
        }
# endif /* CONFIG_DS301_V30 */

        /* get Lifetime Factor from od */
# ifdef CONFIG_DS301_V30
        /* use 32bit entries from DS301 V30 */
        if (getObjEntry(LIFE_TIME_FAC_INDEX, 0,
        (UNSIGNED8 *)&tmpU32, &size, CO_TRUE CO_COMMA_LINE_PARA)
            != CO_OK) {
        return CO_E_NO_ACCESS;
        }
        tmpU8 = (UNSIGNED8)tmpU32;
# else /* CONFIG_DS301_V30 */
        if (getObjEntry(LIFE_TIME_FAC_INDEX, 0,
        &co_pNode  ->bLifeTimeFactor, &size,
            CO_TRUE CO_COMMA_LINE_PARA) != CO_OK) {
        return CO_E_NO_ACCESS;
        }
        tmpU8 = co_pNode  ->bLifeTimeFactor;
# endif /* CONFIG_DS301_V30 */

        setLifeTime(&tmpU16, &tmpU8 CO_COMMA_LINE_PARA);
    }
#else /* CONFIG_NODE_GUARDING */
    /* Nodeguarding requested but not compiled */
    return(CO_E_NO_INITIATE);
#endif /* defined(CONFIG_NODE_GUARDING) */
    }

    if (defHeartbeat == CO_TRUE)  {

#if defined(CONFIG_HEARTBEAT_PRODUCER)
    co_pNode  ->flags |= GUARDFLAG_HB_POSSIBLE;

    /* get Heartbeat Time from dictionary */
    if (getObjEntry(HEARTBEAT_PROD_INDEX, 0, (UNSIGNED8 *)&tmpU16,
         &size, CO_TRUE CO_COMMA_LINE_PARA) != CO_OK) {
        return CO_E_NO_ACCESS;
    }
    /* set heartbeat time */
    setHeartBeatProducerTime(tmpU16 CO_COMMA_LINE_PARA);

#else /* defined(CONFIG_HEARTBEAT_PRODUCER) */
    /* Heartbeat requested but not compiled */
    return(CO_E_NO_INITIATE);
#endif /* defined(CONFIG_HEARTBEAT_PRODUCER) || defined(HEARTBEAT_CONSUMER) */
    }

#ifdef CONFIG_LSS_SLAVE
    /* if the node unconfigured */
    if (coNodeId  == 255)  {
    co_pNode  ->eState = INITIALISING;
    } else
#endif /* CONFIG_LSS_SLAVE */
    {
    co_pNode  ->eState = PRE_OPERATIONAL;

    pData[0] = 0;
    TRANSMIT_COB(co_pNode  ->pGuard_COB, pData);
    }

#ifdef CONFIG_CO_RUN_LED
    updateNMTState_led();
#endif /* CONFIG_CO_RUN_LED */

    return(CO_OK);
}


/****************************************************************************/
/**
*++ \brief deleteNodeReq - request the service Delete Node.
*-- \brief deleteNodeReq - fordert den Dienst Delete Node an.
*
*++ The NMT-Slave deletes its own Node Object.
*++ Allocated ressources are freed.
*
*-- Der NMT-Slave löscht das bei ihm vorhandene
*-- Node Object.
*-- Belegte Ressourcen werden freigegeben.
*
* \retval CO_OK
*++ success
*-- Erfolg
* \retval CO_E_STATE
*++ The Node Object isn't in the state DISCONNECTED or
*++ PRE_OPERATIONAL or PREPARED for
*++ Minimum Capability Device (Minimum Boot Up).
*-- Das Node Object ist nicht im Zustand DISCONNECTED bzw.
*-- PRE_OPERATIONAL oder PREPARED für
*-- Minimum Capability Device (Minimum Boot Up).
*
*/

RET_T deleteNodeReq(
    void
    )
{
    assert(co_pNode  != NULL);

    if (co_pNode  ->eState == OPERATIONAL ) {
    return(CO_E_STATE);
    }
    CalFree(co_pNode );
    co_pNode  = NULL;
    return(CO_OK);
}


#if defined(CONFIG_SLAVE) || defined(CONFIG_MASTER_PLUS)
/****************************************************************************/
/**
*++ \brief getNodeState - provide the local node state
*-- \brief getNodeState - liefert den Zustand des lokalen Netzknotens
*
*++ This function provides the communication state of the local node.
*-- Diese Funktion liefert den Kommunikationszustand
*-- des lokalen Knotens zurück.
*
* \retval 0
*++ error, node doesn't exist
*-- Fehler, der gewählte Knoten existiert nicht
* \retval STOPPED
* \retval OPERATIONAL
* \retval PRE_OPERATIONAL
* \retval RESET_APPLICATION
* \retval RESET_COMM
*
*/

NODE_STATE_T getNodeState(void)
{
  if (co_pNode == NULL)
  {
    return((NODE_STATE_T)0);
  }
  else
  {
    return(co_pNode->eState);
  }
}


/****************************************************************************/
/**
*++ \brief setNodePREOP - set the local node state to PRE-OPERATIONAL
*-- \brief setNodePREOP - setzt den Zustand des lokalen Netzknotens
*
*++ This function changes the communication state of the local node.
*++ It is only allowed to call this function at OPERATIONAL
*-- Diese Funktion setzt den Kommunikationszustand
*-- des lokalen Knotens zu PRE-OPERATIONAL.
*-- Sie darf aber nur im Zustand OPERATIONAL aufgerufen werden.
*
* \retval CO_OK
*++ ok
*-- ok
* \retval CO_E_DEVICE_STATE
*++ error, bad device state
*-- Fehler, falscher Knotenstatus (ungleich OPERATIONAL)
*
*/

RET_T setNodePREOP(void)
{
  assert(co_pNode  != NULL);

  if (co_pNode  ->eState != OPERATIONAL)
  {
    return(CO_E_DEVICE_STATE);
  }

  setNodeState(PRE_OPERATIONAL CO_COMMA_LINE_PARA);
  return(CO_OK);
}
#endif /* CONFIG_SLAVE || CONFIG_MASTER_PLUS */


#ifdef CONFIG_CO_RUN_LED
/*******************************************************************
*
* updateNMTState_led - update run led with actual nmt state
*
* NOMANUAL
*
* nothing
*
*/
void updateNMTState_led(
     )
{
    if (co_pNode  ->eState == OPERATIONAL)  {
    setCoLedState(CO_RUN_LED_OPERATIONAL);
    }
    if (co_pNode  ->eState == PRE_OPERATIONAL)  {
    setCoLedState(CO_RUN_LED_PREOP);
    }
    if (co_pNode  ->eState == STOPPED)  {
    setCoLedState(CO_RUN_LED_STOPPED);
    }
}
#endif /* CONFIG_CO_RUN_LED */

/*______________________________________________________________________EOF_*/

