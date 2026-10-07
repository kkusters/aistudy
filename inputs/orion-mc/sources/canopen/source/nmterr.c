/*
 *++ nmterr - NMT routines for network and node error handling (Slave)
 *-- nmterr - NMT Routinen zur Netzwerk- und Knotenüberwachung (Slave)
 *
 * Copyright (c) 1995-2002 port GmbH Halle (Saale)
 *------------------------------------------------------------------
 * $Header$
 *
 *------------------------------------------------------------------
 *
 *
 * modification history
 * --------------------
 * $Log$
 * Revision 1.1  2008-03-26 17:06:02+01  driet
 * <>
 *
 * Revision 1.0  2008-03-07 17:04:43+01  driet
 * Initial revision
 *
 * Revision 2.16  2003/06/16 09:56:13  boe
 * answer to RTR request if heartbeat is used (conformance test)
 *
 * Revision 2.15  2003/03/11 14:01:11  boe
 * define tranmsit buffer variable only if FULLCAN and NODEGUARDING
 *
 * Revision 2.14  2002/11/15 10:20:47  boe
 * add comments/adapt on doxygen
 * add co_ to all global library variables
 * separate functions for heartbeat and nodeguarding
 * new functions for setTime
 *
 * Revision 2.13  2002/05/21 14:12:42  boe
 * cleanup new timer usage
 * add led functionality
 *
 * Revision 2.12  2002/03/26 08:15:02  boe
 * add NEW_TIMER functionality over defines
 *
 * Revision 2.11  2001/05/10 14:03:29  boe
 * adaption for new ds307
 *
 * Revision 2.10  2001/04/12 14:10:34  boe
 * define prototyps for single line always by void
 *
 * Revision 2.9  2001/03/28 12:53:45  boe
 * additionally include file removed
 *
 * Revision 2.8  2001/02/26 14:53:17  boe
 * driver access functions replaced by macros
 * flying master functionality added
 *
 * Revision 2.7  2001/01/26 11:01:58  boe
 * split include files into function specific headers
 *
 * Revision 2.6  2001/01/17 16:13:05  boe
 * expand all implicite if tests and add type castings
 *
 * Revision 2.5  2000/10/05 11:54:32  boe
 * ignore foreign bootup at FULLCAN-Mode
 *
 * Revision 2.4  2000/06/13 08:31:47  boe
 * function names (H_) changed
 *
 * Revision 2.3  2000/03/29 15:32:38  boe
 * extra parameter for multilien version deleted
 *
 * Revision 2.2  2000/03/28 14:28:31  boe
 * adaption for multi-line version
 * old comments removed
 *
 * Revision 2.1  2000/02/04 13:38:14  oe
 * - Changes for generating reference pages
 *
 * Revision 2.0  2000/01/21 11:02:42  boe
 * Überarbeitet und an Version 4.0 angepasst
 *
 *
 *
 *
 *--------------------------------------------------------------------------
 */

/*
*  \file nmterr.c
*  \author port GmbH Halle (Saale)
*  $Revision$
*  $Date$
*
*++ This file contains the NMT Network Error Mangement functionalities.
*++ One of the two protocols 
*++ \b "node guarding"
*++ or
*++ \b "heart beat"
*++ has to be supported by a slave.
*++ For a CANopen Minimum Capability Device
*++ the define \c CONFIG_NODE_GUARDING or \c CONFIG_HEARTBEAT_PRODUCER has to
*++ be set to enable this code for the compiler.
*-- Diese Datei enthält Routinen für die Knotenüberwachung eines Slaves.
*-- Ein Protokoll muß jeder Slave unterstützen.
*-- Zur Auswahl stehen
*-- \b Nodeguarding
*-- oder
*-- \b Heartbeat.
*-- Für CANopen Minimum Capability Device ist die Compilerdirektive
*-- \ CONFIG_NODE_GUARDING bzw. \c CONFIG_HEARTBEAT_PRODUCER zu setzen.
*
*++ All of the functions are only called from within the library
*++ and not from the library user.
*++ Therefore there are no manual entries of the functions available.
*-- Alle hier enthaltenen Funktionen werden nur innerhalb der Library
*-- aufgerufen.
*-- Daher sind keine Funktionsbeschreibungen verfügbar.
* 
*/

/* header of standard C - libraries */

#include <string.h>
#include <stdio.h>

/* header of project specific types */

#include <cal_conf.h>
#include "nmterr.h"
#include "nmt.h"
#include "drv.h"
#include "timer.h"
#ifdef CONFIG_FLYING_MASTER
# include "flyma.h"
#endif /* CONFIG_FLYING_MASTER */
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

/* external variables
---------------------------------------------------------------------------*/

/* global variables
---------------------------------------------------------------------------*/

/* local defined variables
---------------------------------------------------------------------------*/
#ifdef CONFIG_RCS_IDENT
static char _rcsid[] = "$Id$";
#endif /* CONFIG_RCS_IDENT */


#if defined(CONFIG_HEARTBEAT_PRODUCER)
/***************************************************************************
*
*++ NMT_HB_TimerPulse - function counts timer pulses for heartbeat
*-- NMT_HB_TimerPulse - Funktion zählt Timerpulse für Heartbeat
*
* NOMANUAL
*
*-- Diese Funktion sendet das Heartbeat für einen Heartbeat Producer.
*
* \returns
* nothing
*
*/

void NMT_HB_TimerPulse(
     void
    )
{
UNSIGNED8 pData[1];     /* temp. transmit buffer */

    /* if heartbeat not enabled, return */
    if ((co_pNode  ->flags & GUARDFLAG_HB_ACTIVE) == 0){
    return;
    }

    /* setup new messages */
    pData[0] = (UNSIGNED8)co_pNode  ->eState;

# ifdef CONFIG_FULLCAN
    UPDATE_COB(co_pNode  ->pGuard_COB, pData);
# endif /* CONFIG_FULLCAN */
    TRANSMIT_COB(co_pNode  ->pGuard_COB, pData);
}
#endif /* CONFIG_HEARTBEAT_PRODUCER) */


#ifdef CONFIG_SLAVE
# if defined(CONFIG_NODE_GUARDING)
/***************************************************************************
*
*++ NMT_TimerPulse - function counts timer pulses for node guarding
*-- NMT_TimerPulse - Funktion zählt Timerpulse für Nodeguarding
*
* NOMANUAL
*
*++ This function counts timer pulses for node guarding on a slave device.
*++ It must be included in the Timer ISR or in a time triggered
*++ process. The parameter is the timer interval in ms.
*-- Diese Funktion zählt die Timerpulse für das Nodeguarding auf Slave-Geräten.
*
* \returns
* nothing
*
*/

void NMT_TimerPulse(void)
{
NODE_T      *pNodeLoc;  // local pointer to node structure
#  if defined(CONFIG_FULLCAN)
UNSIGNED8   pData[1];   // temp. transmit buffer
#  endif // defined(CONFIG_HEARTBEAT_PRODUCER) || defined(CONFIG_FULLCAN)

  pNodeLoc = co_pNode;

  // if lifeguarding not active, return
  if ((pNodeLoc->flags & (GUARDFLAG_NG_ACTIVE | GUARDFLAG_NG_LIFETIME)) != (GUARDFLAG_NG_ACTIVE | GUARDFLAG_NG_LIFETIME))
  {
    return;
  }

  // no request from master was received
  pNodeLoc->bSuspendedGuardings++;
  if (pNodeLoc->bSuspendedGuardings == pNodeLoc->bLifeTimeFactor)
  {
    // lifetime is over, disable life guarding
    if (sGuardErrorInd(CO_LOST_CONNECTION CO_COMMA_LINE_PARA) == 1)
    {
#  ifdef CONFIG_CO_ERR_LED
      setCoLedState(CO_ERR_LED_NMT);
#  endif // CONFIG_CO_ERR_LED
      if (co_pNode->eState == OPERATIONAL)
      {
        setNodeState(PRE_OPERATIONAL CO_COMMA_LINE_PARA);
      }
    }

#  ifdef CONFIG_FULLCAN
    pNodeLoc->bGuardToggle = 0; // toggled to 1 after first guarding
    pData[0] = (UNSIGNED8)co_pNode->bGuardToggle | (UNSIGNED8)co_pNode->eState;
    UPDATE_COB(co_pNode->pGuard_COB, pData);
    pNodeLoc->bGuardToggle = 0x80; // toggled to 1 after first guarding
#  else // CONFIG_FULLCAN
    pNodeLoc->bGuardToggle = 0x0; // toggled to 0 before first guarding
#  endif // CONFIG_FULLCAN

    // disable guarding
    // pNodeLoc->flags &= ~(GUARDFLAG_NG_ACTIVE | GUARDFLAG_NG_LIFETIME);
    pNodeLoc->flags &= ~GUARDFLAG_NG_ACTIVE;
    removeTimerEvent(&co_pNode->timer CO_COMMA_LINE_PARA);
    return;
  }
// one suspended guarding is generated by the timer resolution only the seconded missed guarding will be showed to prevent such an error
  if (pNodeLoc->bSuspendedGuardings > 1)
  {
    if (sGuardErrorInd(CO_LOST_GUARDING_MSG CO_COMMA_LINE_PARA) == 1)
    {
#  ifdef CONFIG_CO_ERR_LED
      setCoLedState(CO_ERR_LED_NMT);
#  endif // CONFIG_CO_ERR_LED
      if (co_pNode->eState == OPERATIONAL)
      {
        setNodeState(PRE_OPERATIONAL CO_COMMA_LINE_PARA);
      }
    }
  }
}
# endif // defined(CONFIG_NODE_GUARDING)
#endif // CONFIG_SLAVE


#if (defined(CONFIG_NODE_GUARDING) && defined(CONFIG_SLAVE)) \
  || defined(CONFIG_HEARTBEAT_PRODUCER)
/***************************************************************************
*
* NMT_NodeGuardingMsg - reaction to node guarding message from master
*
* NOMANUAL
*
* transmit function was inserted because node guarding didn't work.
* REMARKS:
* .LP
* First node guard COB must have a toggle bit * set to 0.
* The default settings toke place in function
* NMT_CreateNode(). Node guarding COB has to be transferred first
* then you can change the toggle bit. The next problem is to catch
* the status before you send the guarding COB.
* I decide to solve this problem with the changing of the init data of the node.
* init value of bGuardToggle == 1 (ts).
*
* \returns
* nothing
*
*/

void NMT_NodeGuardingMsg(
    CAN_MSG_T *canMsg       /* Pointer to CAN Message */
     )
{
UNSIGNED8 pData[1];             /* transmit buffer */

# ifdef CONFIG_FULLCAN
    /* workaround for conformance test */
    /* Boot-up Message was sent ? */
    if ((canMsg->pData[0]) == 0x0) {
    /* update channel with actual state */
    pData[0] = (UNSIGNED8)co_pNode  ->eState;
    UPDATE_COB(co_pNode  ->pGuard_COB, pData);

#  ifdef CONFIG_NODE_GUARDING
    /* set toggle bit */
    co_pNode  ->bGuardToggle = 0x80;
#  endif /* defined(CONFIG_NODE_GUARDING) */
    return;
    }
# else /* CONFIG_FULLCAN */
    /* only to avoid compiler warnings */
    canMsg = canMsg;
# endif /* CONFIG_FULLCAN */

# ifdef CONFIG_NODE_GUARDING
    /* if guarding and heartbeat is active - heartbeat has priority */
    if ((co_pNode  ->flags & 
     (GUARDFLAG_NG_POSSIBLE | GUARDFLAG_HB_ACTIVE)) != GUARDFLAG_NG_POSSIBLE)  {
    /* heartbeat is active or nodeguarding is not initialized */
    return;
    }

    co_pNode  ->bSuspendedGuardings = 0;

    /* inform application about start of guarding */
    if ((co_pNode  ->flags & GUARDFLAG_NG_ACTIVE) == 0) {
    sGuardErrorInd(CO_GUARDING_STARTED CO_COMMA_LINE_PARA);
#  ifdef CONFIG_CO_ERR_LED
    resetCoLedState(CO_ERR_LED_NMT);
#  endif /* CONFIG_CO_ERR_LED */

    /* first node guarding request set guarding active */
    co_pNode  ->flags |= GUARDFLAG_NG_ACTIVE;
    }

    pData[0] = co_pNode  ->bGuardToggle
           | (UNSIGNED8)co_pNode  ->eState;
    co_pNode  ->bGuardToggle =
        co_pNode  ->bGuardToggle ^ 0x80;

#  ifdef CONFIG_FULLCAN
    UPDATE_COB(co_pNode  ->pGuard_COB, pData);
#  else /* CONFIG_FULLCAN */
    TRANSMIT_COB(co_pNode  ->pGuard_COB, pData);
#  endif /* CONFIG_FULLCAN */

    /* if lifetime is enabled, start timer to monitor the master */
    /* removeTimerEvent(&co_pNode->timer); */
    if ((co_pNode  ->flags & GUARDFLAG_NG_LIFETIME)
        != 0) {
    addTimerEvent(&co_pNode  ->timer,
        co_pNode  ->timer.timerVal,
        CO_TIMER_TYPE_NG_SLAVE | CO_TIMER_TYPE_CYCLIC
        CO_COMMA_LINE_PARA);
    }
# else /* defined(CONFIG_NODE_GUARDING) */
    /* Conformance Test uses RTR for Heartbeat nodes */
    pData[0] = (UNSIGNED8)co_pNode  ->eState;
#  ifdef CONFIG_FULLCAN
    UPDATE_COB(co_pNode  ->pGuard_COB, pData);
#  else /* CONFIG_FULLCAN */
    TRANSMIT_COB(co_pNode  ->pGuard_COB, pData);
#  endif /* CONFIG_FULLCAN */
# endif /* defined(CONFIG_NODE_GUARDING) */
}

#endif /*(defined(CONFIG_NODE_GUARDING) || defined(CONFIG_HEARTBEAT_PRODUCER)*/


#ifdef CONFIG_NODE_GUARDING
/***************************************************************************
*
* setLifeTime - set the guarding life time
*
* NOMANUAL
*
* This function set up the internal values for life time monitoring.
* It reads the value from od and starts or stops the 
* life time monitoring
*
* If the nodeguard time and life time factor != 0 
* and heartbeat is disabled,
* lifeguarding is started
* else is it disabled
*
*
* \returns CO_OK
*   successfull
* \returns CO_E_SRD_NO_RESSOURCE
*   node guardin not possible because heartbeat is active
*   or node is master - no lifetime monitoring allowed 
*
*/

RET_T setLifeTime(
    UNSIGNED16  *lifeTime,  /**< life time */
    UNSIGNED8   *factor     /**< life time factor */
    )
{

    /* if heartbeat is active, don't anything */
    if ((co_pNode  ->flags & GUARDFLAG_HB_ACTIVE) != 0){
    return(CO_E_SRD_NO_RESSOURCE);
    }

# if defined(CONFIG_MASTER) || defined(CONFIG_SLAVE_PLUS)
    /* if the node is master, no lifetime monitoring is possible */
    if ((co_pNode  ->mflags & GUARDFLAG_MASTER) != 0)  {
    return(CO_E_SRD_NO_RESSOURCE);
    }
# endif /* defined(CONFIG_MASTER) || defined(CONFIG_SLAVE_PLUS) */

    /* disable if lifetime or lifetime factor is zero */
    if ((*lifeTime == 0) || (*factor == 0)) {
    /* disable guarding */
    co_pNode  ->flags &=
        ~(GUARDFLAG_NG_ACTIVE | GUARDFLAG_NG_LIFETIME);

    /* stop timer */
    removeTimerEvent(&co_pNode  ->timer
        CO_COMMA_LINE_PARA);

    } else {
    /* lifetime and guardtime are not 0 */

    /* start node guarding, if its not active */
    co_pNode  ->flags |= GUARDFLAG_NG_LIFETIME;

    co_pNode  ->timer.timerVal = *lifeTime * 10;
    co_pNode  ->bLifeTimeFactor = *factor;
    }

    return(CO_OK);
}
#endif /* defined(CONFIG_NODE_GUARDING) */


#ifdef CONFIG_HEARTBEAT_PRODUCER
/***************************************************************************
*
* setHeartbeatProducerTime - set the heartbeat producer time
*
* NOMANUAL
*
* This function set up the internal values for heartbeat producer
* It reads the value from od and starts or stops the 
* heartbeat producer timer.
*
* If the heartbeat time is > 0 and nodeguarding is enabled,
* nodeguarding will be stopped and heartbeat is started.
*
*
* \returns
* nothing
*
*/

void setHeartBeatProducerTime(
    UNSIGNED16  hbTime  /**< heartbeat time */
    )
{
UNSIGNED32  timerVal;   /* timer value */

    timerVal = (UNSIGNED32)hbTime * 10;

    /* if time > 0 enable heartbeat */
    if (timerVal > 0)  {

    co_pNode  ->flags |= GUARDFLAG_HB_ACTIVE;

    /* disable Nodeguarding */
    co_pNode  ->flags &=
        ~(GUARDFLAG_NG_ACTIVE | GUARDFLAG_NG_LIFETIME);

    /* start heartbeat timer */
    addTimerEvent(&co_pNode  ->timer, timerVal,
        CO_TIMER_TYPE_HB_PROD | CO_TIMER_TYPE_CYCLIC
        CO_COMMA_LINE_PARA);
    } else {

    /* disable heartbeat */
    removeTimerEvent(&co_pNode  ->timer
        CO_COMMA_LINE_PARA);

    co_pNode  ->flags &= ~GUARDFLAG_HB_ACTIVE;
    }
}

#endif /* CONFIG_HEARTBEAT_PRODUCER */

/*______________________________________________________________________EOF_*/
