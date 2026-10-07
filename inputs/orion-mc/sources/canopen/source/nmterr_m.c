/*
 *++ nmterr_m - NMT routines for network and node error handling (Master)
 *-- nmterr_m - NMT Routinen zur Netzwerk- und Knotenüberwachung (Master)
 *
 * Copyright (c) 1997-2002 port GmbH Halle (Saale)
 *------------------------------------------------------------------
 * $Header$
 *
 *------------------------------------------------------------------
 *
 *
 * modification history
 * --------------------
 * $Log$
 * Revision 1.1  2008-03-26 17:06:03+01  driet
 * <>
 *
 * Revision 1.0  2008-03-07 17:04:44+01  driet
 * Initial revision
 *
 * Revision 2.21  2003/03/11 14:02:25  boe
 * change cast value
 *
 * Revision 2.20  2003/02/28 16:08:03  boe
 * correct multiline usage for heartbeat consumers
 *
 * Revision 2.19  2003/01/09 16:13:52  boe
 * add typecast for NODE_STATE_T
 *
 * Revision 2.18  2002/11/18 09:30:12  boe
 * add comments/adapt on doxygen
 * add co_ to all global library variables
 * save monitoring properties at mflags
 * get more exact cob-type to driver (don't use SPECIAL_CHANNEL)
 *
 * Revision 2.17  2002/05/21 14:17:39  boe
 * cleanup new timer usage
 * add led functionality
 *
 * Revision 2.16  2002/03/26 08:15:02  boe
 * add NEW_TIMER functionality over defines
 *
 * Revision 2.15  2001/05/10 14:04:05  boe
 * adaption for new ds307
 *
 * Revision 2.14  2001/04/12 14:13:42  boe
 * define prototyps for single line always by void
 *
 * Revision 2.13  2001/04/05 08:45:48  boe
 * comment changed
 *
 * Revision 2.12  2001/03/28 12:47:26  boe
 * added functionality for SLAVE_PLUS
 *
 * Revision 2.11  2001/03/19 15:54:52  ro
 * sdomgr.h only included if CONFIG_DYN_SDO_CONNECTION_MANAGER defined
 *
 * Revision 2.10  2001/03/14 15:54:54  ro
 * Flying Master:
 * Heartbeat check for more then one master detection only if a master is detect
 *
 * Revision 2.9  2001/02/26 14:11:15  boe
 * documentation format changed
 * flying master functionality added
 * driver access functions replaced by macros
 *
 * Revision 2.8  2001/01/26 11:03:06  boe
 * split include files into function specific headers
 * use can buffer by function parameter (no acces more to variable CAN_Msg)
 *
 * Revision 2.7  2000/10/05 11:55:36  boe
 * CAN buffer for Multiline used as array
 *
 * Revision 2.6  2000/08/31 13:48:55  boe
 * call mGuardInd() for bootup and heartbeat started
 *
 * Revision 2.5  2000/06/13 08:32:30  boe
 * function names (H_) changed
 * hearbeat failure changed
 *
 * Revision 2.4  2000/05/11 13:41:20  boe
 * Heartbeat Consumer Mode modified
 *
 * Revision 2.3  2000/04/19 07:33:30  boe
 * multi line mode changed
 *
 * Revision 2.2  2000/03/28 14:29:21  boe
 * adaption for multi-line version
 *
 * Revision 2.1  2000/02/04 13:38:15  oe
 * - Changes for generating reference pages
 *
 * Revision 2.0  2000/01/21 11:02:55  boe
 * Überarbeitet und an Version 4.0 angepasst
 *
 *
 *
 *--------------------------------------------------------------------------
 */

/****************************************************************************/
/*
*  \file nmterr_m.c
*  \author port GmbH Halle (Saale)
*  $Revision$
*  $Date$
*
*++ This file contains the NMT Network Error Mangement (Nodeguarding/Heartbeat)
*++ for a master device.
*++ For a CANopen Minimum Capability Device
*++ the define CONFIG_NODE_GUARDING or CONFIG_HEART_BEAT has to be set
*++ to enable this code for the compiler.
*-- Diese Datei enthält Routinen für die Knotenüberwachung der Slaves
*-- durch einem Master.
*-- Jeder Slave muss einen Knotenüberwachungsdienst bereitstellen
*-- (Nodeguarding oder Heartbeat).
*-- Für CANopen Minimum Capability Device ist die Compilerdirektive
*-- CONFIG_NODE_GUARDING oder CONFIG_HEARTBEAT_CONSUMER zu setzen.
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
#include "nmt_m.h"
#include "drv.h"
#include "nmterr.h"
#include "nmterr_m.h"

#ifdef CONFIG_FLYING_MASTER
# include "flyma.h"
#endif /* CONFIG_FLYING_MASTER */

#ifdef CONFIG_DYN_SDO_CONNECTION_MANAGER
# include "sdomgr.h"
#endif /* CONFIG_DYN_SDO_CONNECTION_MANAGER */

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

#if defined(CONFIG_MASTER) || defined(CONFIG_SLAVE_PLUS)

/***************************************************************************
*
* NMT_M_NodeGuardingMsg - function analyses the guarding message
*
* NOMANUAL
*
* This function analyses the guarding message answer from any slave on the
*
* RETURNS
* .TP
* nothing
*
*/

void NMT_M_NodeGuardingMsg(CAN_MSG_T *canMsg) // Pointer to CAN Message
{
NODE_T *pNodeInUse;     /* pointer to node struct */

  if ((pNodeInUse = NMT_NodeExist((UNSIGNED8)(canMsg->wCOB_ID & 0x7f) CO_COMMA_LINE_PARA)) == NULL)
  {
    // if not found
    return;
  }

  // test for bootup message
  if ((canMsg->pData[0]) == 0)
  {
    // yes, it's bootup message
    mGuardErrorInd(pNodeInUse->bNode_ID, CO_BOOT_UP CO_COMMA_LINE_PARA);
  }
  else
  {
# ifdef CONFIG_SLAVE_PLUS
# else // CONFIG_SLAVE_PLUS
    // test for wrong state
    if ((canMsg->pData[0] & 0x7f) != (UNSIGNED8)(pNodeInUse->eState))
    {
      mGuardErrorInd(pNodeInUse->bNode_ID, CO_NODE_STATE CO_COMMA_LINE_PARA);
      pNodeInUse->eState = (NODE_STATE_T)(canMsg->pData[0] & 0x7f);
    }
# endif // CONFIG_SLAVE_PLUS

# ifdef CONFIG_NODE_GUARDING
    if ((pNodeInUse->mflags & GUARDFLAG_NG_ACTIVE) != 0)
    {
      // test of toggle bit
      if ((canMsg->pData[0] & 0x80) != pNodeInUse->bGuardToggle)
      {
        pNodeInUse->bGuardToggle = canMsg->pData[0] & 0x80;
        pNodeInUse->bSuspendedGuardings = 0;
        pNodeInUse->mflags |= GUARDFLAG_NG_RECEIVED;
      }
    }
# endif // CONFIG_NODE_GUARDING

# ifdef CONFIG_HEARTBEAT_CONSUMER
    if ((pNodeInUse->mflags & (GUARDFLAG_HB_POSSIBLE)) != 0)
    {
      pNodeInUse->mflags |= GUARDFLAG_NG_RECEIVED;
      // user indication for the first occurence of the heartbeat
      if ((pNodeInUse->mflags & GUARDFLAG_HB_ACTIVE) == 0)
      {
        mGuardErrorInd(pNodeInUse->bNode_ID, CO_HB_STARTED CO_COMMA_LINE_PARA);
        pNodeInUse->mflags |= GUARDFLAG_HB_ACTIVE;
      }
      // start the timer (again)
      addTimerEvent(&pNodeInUse->timer, pNodeInUse->timer.timerVal, CO_TIMER_TYPE_HB_CONS CO_COMMA_LINE_PARA);
    }
# endif // CONFIG_HEARTBEAT_CONSUMER
  }

# ifdef CONFIG_CO_ERR_LED
  resetCoLedState(CO_ERR_LED_NMT);
# endif // CONFIG_CO_ERR_LED
}


/***************************************************************************
*
*++ NMT_M_TimerPulse_ind - function counts timer pulses for node guarding
*-- NMT_M_TimerPulse_ind - Funktion zählt Timerpulse für Nodeguarding
*
* NOMANUAL
*
*++ This function counts timer pulses for node guarding on a master device.
*++ It must be included in the Timer ISR or in a time triggered
*++ process. The parameter is the timer interval in ms.
*-- Diese Funktion zählt die Timerpulse für das Nodeguarding auf Master-Geräten.
*
* RETURNS
* .TP
* nothing
*
*/

void NMT_M_TimerPulse(TIMER_EVENT_T *pTimer) // pointer to timer event structure
{
NODE_T *pNodeInUse; // pointer to current node structure

  // the timer event structure is the first entry at the node structure 
  // therefore the pointer is equal to the start of the node structure 
  pNodeInUse = (NODE_T *)pTimer;

# ifdef CONFIG_NODE_GUARDING
  if ((pNodeInUse->mflags & (GUARDFLAG_NG_POSSIBLE | GUARDFLAG_NG_ACTIVE)) == (GUARDFLAG_NG_POSSIBLE | GUARDFLAG_NG_ACTIVE))
  {
    // Nodeguarding active

    // GuardTime elapsed but no Guarding Answer received
    if ((pNodeInUse->mflags & GUARDFLAG_NG_RECEIVED) == 0)
    {
      pNodeInUse->bSuspendedGuardings++;

      // life time for this module is elapsed
      if (pNodeInUse->bSuspendedGuardings == pNodeInUse->bLifeTimeFactor)
      {
        // disable guarding
        pNodeInUse->mflags &= ~GUARDFLAG_NG_ACTIVE;

#  ifdef CONFIG_DYN_SDO_CONNECTION_MANAGER
        lostConnection(pNodeInUse->bNode_ID CO_COMMA_LINE_PARA);
#  endif // CONFIG_DYN_SDO_CONNECTION_MANAGER

        mGuardErrorInd(pNodeInUse->bNode_ID, CO_LOST_CONNECTION CO_COMMA_LINE_PARA);
#  ifdef CONFIG_CO_ERR_LED
        setCoLedState(CO_ERR_LED_NMT);
#  endif // CONFIG_CO_ERR_LED

        // remove the timer event at the incdication function its better we only remove the flag
        // removeTimerEvent(&pNodeInUse->timer); */
        pNodeInUse->timer.timerType &= ~CO_TIMER_TYPE_CYCLIC;
      }
      else
      {
        // life time isn't elapsed
        mGuardErrorInd(pNodeInUse->bNode_ID, CO_LOST_GUARDING_MSG CO_COMMA_LINE_PARA);
        TRANSMIT_COB(pNodeInUse->pGuard_COB, NULL);
#  ifdef CONFIG_CO_ERR_LED
        setCoLedState(CO_ERR_LED_NMT);
#  endif // CONFIG_CO_ERR_LED
      }
    }
    else
    {
      // guarding was received
      pNodeInUse->mflags &= ~GUARDFLAG_NG_RECEIVED;
      TRANSMIT_COB(pNodeInUse->pGuard_COB, NULL);
    }
  }
# endif // CONFIG_NODE_GUARDING

# ifdef CONFIG_HEARTBEAT_CONSUMER
  if ((pNodeInUse->mflags & (GUARDFLAG_HB_POSSIBLE + GUARDFLAG_HB_ACTIVE)) == (GUARDFLAG_HB_POSSIBLE + GUARDFLAG_HB_ACTIVE))
  {
    // Heartbeat is active
    pNodeInUse->mflags &= ~GUARDFLAG_HB_ACTIVE;

#  ifdef CONFIG_DYN_SDO_CONNECTION_MANAGER
    lostConnection(pNodeInUse->bNode_ID CO_COMMA_LINE_PARA);
#  endif // CONFIG_DYN_SDO_CONNECTION_MANAGER

    mGuardErrorInd(pNodeInUse->bNode_ID, CO_LOST_HEARTBEAT CO_COMMA_LINE_PARA);

#ifdef CONFIG_CO_ERR_LED
    setCoLedState(CO_ERR_LED_NMT);
#endif // CONFIG_CO_ERR_LED

#  ifdef CONFIG_FLYING_MASTER
    // if this the actual master node, start new master nego.
    if (activeManager == pNodeInUse->bNode_ID)
    {
      resetCommReq(0 CO_COMMA_LINE_PARA);
    }
#  endif // CONFIG_FLYING_MASTER
  }
# endif // CONFIG_HEARTBEAT_CONSUMER
}
# endif // CONFIG_MASTER || CONFIG_SLAVE_PLUS

/*______________________________________________________________________EOF_*/
