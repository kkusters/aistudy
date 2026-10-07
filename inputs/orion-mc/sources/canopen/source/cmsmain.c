/*
 *++ cmsmain - contains functions for all CMS services of CAL
 *-- cmsmain - beinhaltet Funktionen für alle CMS Dienste von CAL
 *
 * Copyright (c) 1994-2002 port GmbH Halle (Saale)
 *------------------------------------------------------------------
 * $Header$
 *
 *--------------------------------------------------------------------------
 *
 * modification history
 * --------------------
 * $Log$
 * Revision 1.1  2008-03-19 17:04:50+01  driet
 * <>
 *
 * Revision 1.0  2008-03-07 17:04:38+01  driet
 * Initial revision
 *
 * Revision 2.22  2003/07/30 08:08:40  boe
 * include time header only if CONFIG_TIME is set
 *
 * Revision 2.21  2003/01/20 15:15:57  boe
 * correct LSS COB-Id entries
 *
 * Revision 2.20  2002/11/14 09:59:45  boe
 * add comments/adapt on doxygen
 * replace cob-id's by defines
 * call NMT_NodeGuardingMsg() also for bootup
 * move code for RTR PDOs to pdo.c
 *
 * Revision 2.19  2002/06/11 13:00:33  hae
 * documentation review
 *
 * Revision 2.18  2002/05/21 14:03:15  boe
 * cleanup new timer usage
 *
 * Revision 2.17  2002/05/21 12:45:16  boe
 * change copyright
 *
 * Revision 2.16  2002/03/26 08:17:47  boe
 * add NEW_TIMER functionality over defines
 *
 * Revision 2.15  2001/12/05 13:53:37  boe
 * answer for RTR request only for PODs or NMT services
 *
 * Revision 2.14  2001/05/10 14:02:06  boe
 * adaption for new ds307 names (change master to manager)
 *
 * Revision 2.13  2001/03/29 06:11:59  boe
 * use master capabilities only if CONFIG_MASTER is set
 *
 * Revision 2.11  2001/03/28 12:44:43  boe
 * added functionality for SLAVE_PLUS
 *
 * Revision 2.10  2001/03/19 15:54:00  ro
 * sdomgr.h only included if CONFIG_DYN_SDO_CONNECTION_MANAGER defined
 *
 * Revision 2.9  2001/02/26 14:02:48  boe
 * documentation format chenged
 * flying master functionality added
 *
 * Revision 2.8  2001/01/26 10:48:04  boe
 * split include files into function specific headers
 * use can buffer by function parameter (delete global variable CAN_Msg)
 *
 * Revision 2.7  2001/01/17 16:06:58  boe
 * expand all implicite if tests and add type castings
 * use extra can buffer for each can line
 *
 * Revision 2.6  2000/10/05 11:51:13  boe
 * defines for EMCY,PDO,TIME changed from CLIENT/SERVER to CONSUMER/PRODUCER
 * CAN buffer for Multiline used as array
 *
 * Revision 2.5  2000/07/27 09:59:07  boe
 * save the last transmit time for rtr pdos and reload the event timer
 *
 * Revision 2.4  2000/06/13 08:28:26  boe
 * function names (H_) changed
 *
 * Revision 2.3  2000/05/11 13:40:32  boe
 * conditional define for Heartbeat Consumer Mode modified
 *
 * Revision 2.2  2000/04/18 08:33:06  boe
 * special datatype DATA replaced with CO_DATA
 *
 * Revision 2.1  2000/03/28 14:22:56  boe
 * adaption for multi-line version
 * rtr time requests removed
 *
 * Revision 2.0  2000/01/21 11:01:51  boe
 * Überarbeitet und an Version 4.0 angepasst
 *
 *
 *
 *------------------------------------------------------------------
 */

/**
*  \file cmsmain.c
*  \author port GmbH Halle (Saale)
*  $Revision$
*  $Date$
*
*++ This file contains functions for all CMS services.
*-- Diese Datei enthält Funktionen für alle CMS-Dienste.
*/

/* header of standard C - libraries */

#include <string.h>
#include <stdio.h>
#include <assert.h>

/* header of project specific types */

#include <cal_conf.h>
#include <co_drvif.h>
#include <co_cobid.h>
#include "cmsmain.h"
#include "nmt.h"
#include "nmterr.h"
#include "pdo.h"
#include "sdomain.h"
#include "emerg.h"
#include "drv.h"
#if defined(CONFIG_TIME_PRODUCER) || defined(CONFIG_TIME_CONSUMER)
# include "time_lib.h"
#endif /* defined(CONFIG_TIME_PRODUCER) || defined(CONFIG_TIME_CONSUMER) */
#if defined(CONFIG_MASTER) || defined(CONFIG_SLAVE_PLUS)
# include "nmterr_m.h"
# include "nmt_m.h"
#endif /* defined(CONFIG_MASTER) || defined(CONFIG_SLAVE_PLUS) */

#ifdef CONFIG_DYN_SDO_CONNECTION_MANAGER
# include "sdomgr.h"
#endif /* CONFIG_DYN_SDO_CONNECTION_MANAGER */

#ifdef CONFIG_FLYING_MASTER
# include "flyma.h"
#endif /* CONFIG_FLYING_MASTER */

#ifdef CONFIG_SRDO_CONSUMER
# include "srdo.h"
#endif /* CONFIG_SRDO_CONSUMER */

#if defined(CONFIG_LSS_SLAVE) || defined(CONFIG_LSS_MASTER)
# include "lss.h"
#endif /* defined(CONFIG_LSS_SLAVE) || defined(CONFIG_LSS_MASTER) */


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
void rtrRequestReceived(CAN_MSG_T *canMsg );

/* external variables
---------------------------------------------------------------------------*/

/* global variables
---------------------------------------------------------------------------*/

/* local defined variables
---------------------------------------------------------------------------*/
#ifdef CONFIG_RCS_IDENT
static char _rcsid[] = "$Id$";
#endif /* CONFIG_RCS_IDENT */


/****************************************************************************/
/**
*
*++ \brief msgIdentification - Entrypoint for CANopen Library
*-- \brief msgIdentification - Einsprungspunkt für die CANopen Bibliothek
*
*-- Diese funktion wird von \a FlushMbox() aufgerufen.
*-- Sie führt die Erkennung der CAN-Telegramme durch.
*++ This function is called from inside \a FlushMbox().
*++ It performs the identification if a CAN message.
*
* \return
*++ nothing
*-- nichts
*
*/

void msgIdentification(CAN_MSG_T *canMsg) // Pointer to CAN Message
{
UNSIGNED16 CO_DATA wCOB; // cob data

  assert(co_pNode  != NULL);

  // if rtr request
  if ((canMsg->length & CO_RTR_REQ) != 0)
  {
    // yes, rtr request
#if (defined(CONFIG_NODE_GUARDING) && defined(CONFIG_SLAVE)) || defined(CONFIG_HEARTBEAT_PRODUCER) || defined(CONFIG_PDO_PRODUCER)
    rtrRequestReceived(canMsg CO_COMMA_LINE_PARA);
#endif // (defined(CONFIG_NODE_GUARDING) && defined(CONFIG_SLAVE)) || defined(CONFIG_PDO_PRODUCER)
    return;
  }

  // for faster access
  wCOB = canMsg->wCOB_ID;

  // NMT Event
  if (wCOB == CO_COBID_NMT)
  {
    // NMT Event
#ifdef CONFIG_SLAVE
    NMT_NodeStartStopMsg(canMsg CO_COMMA_LINE_PARA);
#endif // CONFIG SLAVE
  }
  else

#ifdef CONFIG_TIME_CONSUMER
  // Time Stamp
  if ((co_pFirstTime != NULL) && (wCOB == co_pFirstTime->pCOB->wID))
  {
    timeReceived(canMsg CO_COMMA_LINE_PARA);
  }
  else
#endif // CONFIG_TIME_CONSUMER

  if (wCOB < CO_COBID_SYNC)
  {
    return;
  }
  else

  // SYNC telegrams are processed by the driver
#ifdef CONFIG_EMCY_CONSUMER
  if (wCOB <= CO_COBID_EMCY_LAST)
  {
    // Emergency
    emcyMsgReceived(canMsg CO_COMMA_LINE_PARA); 
  }
  else
#endif // CONFIG_EMCY_CONSUMER

#ifdef CONFIG_SRDO_CONSUMER
  if (wCOB <= CO_COBID_SRDO_LAST)
  {
    srdoMsgReceived(canMsg CO_COMMA_LINE_PARA);
  }
#endif // CONFIG_SRDO_CONSUMER

#ifdef CONFIG_PDO_CONSUMER
  if (wCOB <= CO_COBID_PDO_LAST)
  {
    // PDO
    pdoMsgReceived(canMsg CO_COMMA_LINE_PARA);
  }
  else
#endif // CONFIG_PDO_CONSUMER

  if (wCOB <= CO_COBID_SDO_LAST)
  {
    // SDO
    sdoMsgReceived(canMsg CO_COMMA_LINE_PARA);
  }
  else if (wCOB < CO_COBID_NMTERR_FIRST)
  {
    // ????
#ifdef CONFIG_DYN_SDO_CONNECTION_MANAGER
    if (wCOB == CO_COBID_SDOREQ)
    {
      dynSdoRegistration(CO_LINE_PARA);
    }
#endif // CONFIG_DYN_SDO_CONNECTION_MANAGER
  }
  else
  // NMT ERROR control
  if (wCOB <= CO_COBID_NMTERR_LAST)
  {
#if (defined(CONFIG_MASTER) && (defined(CONFIG_NODE_GUARDING) || defined(CONFIG_HEARTBEAT_CONSUMER))) || (defined(CONFIG_SLAVE_PLUS) && defined(CONFIG_HEARTBEAT_CONSUMER))
    // CAN line is master
    if (co_pNetwork  != NULL)
    {
      NMT_M_NodeGuardingMsg(canMsg CO_COMMA_LINE_PARA);
    }
#endif // (defined(CONFIG_MASTER) && (defined(CONFIG_NODE_GUARDING) ||
  }

#ifdef CONFIG_LSS_MASTER
  else if (wCOB == CO_COBID_LSS_CON)
  {
    lssMsgReceived(canMsg CO_COMMA_LINE_PARA);
  }
#endif // CONFIG_LSS_MASTER

#ifdef CONFIG_LSS_SLAVE
  else if (wCOB == CO_COBID_LSS_REQ)
  {
    lssMsgReceived(canMsg CO_COMMA_LINE_PARA);
  }
#endif // CONFIG_LSS_SLAVE

#ifdef CONFIG_FLYING_MASTER
  else if (wCOB > 2040)
  {
    flyManagerMsg(canMsg CO_COMMA_LINE_PARA);
  }
#endif // CONFIG_FLYING_MASTER
}


#if (defined(CONFIG_NODE_GUARDING) && defined(CONFIG_SLAVE)) \
  || defined(CONFIG_HEARTBEAT_PRODUCER) \
  || defined(CONFIG_PDO_PRODUCER)
/*******************************************************************
*
* rtrRequestReceived
*
* NOMANUAL
*
* If a message is identified as an rtr messages then
* this function will be called
* Here the appropriate function for this event is called
*
* \return
*++ nothing
*-- nichts
*
*/
void rtrRequestReceived(
    CAN_MSG_T *canMsg       /* Pointer to CAN Message */
    )
{

#if (defined(CONFIG_NODE_GUARDING) && defined(CONFIG_SLAVE)) \
  || defined(CONFIG_HEARTBEAT_PRODUCER)
    /* CAN line is slave - it is the nodeguarding cob ?*/
    if (canMsg->wCOB_ID == co_pNode  ->pGuard_COB->wID){

    NMT_NodeGuardingMsg(canMsg CO_COMMA_LINE_PARA);
    return;
    }
#endif /* CONFIG_SLAVE && CONFIG_NODE_GUARDING */

#ifdef CONFIG_PDO_PRODUCER
    if ((canMsg->wCOB_ID >= CO_COBID_PDO_FIRST)
     && (canMsg->wCOB_ID <= CO_COBID_PDO_LAST)) {

    pdoRtrMsgReceived(canMsg CO_COMMA_LINE_PARA);
    return;
    }
#endif /* CONFIG_PDO_PRODUCER */
}

#endif /* (defined(CONFIG_NODE_GUARDING) && defined(CONFIG_SLAVE)) || defined(CONFIG_PDO_PRODUCER) */

/*______________________________________________________________________EOF_*/
