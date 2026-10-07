/*
 *++ cmsevent - functions for CAL CMS event handling
 *-- cmsevent - Funktionen für die CAL CMS Event Behandlung
 *
 * Copyright (c) 1994-2002 port GmbH Halle (Saale)
 *------------------------------------------------------------------
 * $Header$
 *
 *------------------------------------------------------------------
 *
 * modification history
 * --------------------
 * $Log$
 * Revision 1.0  2008-03-07 17:04:37+01  driet
 * Initial revision
 *
 * Revision 2.12  2003/03/31 12:24:31  boe
 * function getEvAddr removed
 *
 * Revision 2.11  2002/11/14 09:48:39  boe
 * add comments/adapt on doxygen
 * reduce parameter for CMS_DefEvent_req()
 *
 * Revision 2.10  2002/05/21 14:02:17  boe
 * cleanup new timer usage
 *
 * Revision 2.9  2002/05/21 12:44:50  boe
 * change copyright
 *
 * Revision 2.8  2002/03/26 08:07:07  boe
 * add NEW_TIMER functionality over defines
 *
 * Revision 2.7  2001/05/10 12:28:29  boe
 * commandline for TransEvReq() changed
 *
 * Revision 2.6  2001/02/26 14:47:49  boe
 * driver access functions replaced by macros
 *
 * Revision 2.5  2001/01/26 10:41:04  boe
 * split include files into function specific headers
 *
 * Revision 2.4  2001/01/17 16:06:13  boe
 * expand all implicite if tests and add type castings
 *
 * Revision 2.3  2000/10/04 14:07:45  boe
 * defines for EMCY changed from CLIENT to CONSUMER and SERVER to PRODUCER
 *
 * Revision 2.2  2000/06/13 08:28:00  boe
 * function names (H_) changed
 *
 * Revision 2.1  2000/03/28 14:21:46  boe
 * adaption for multi-line version
 *
 * Revision 2.0  2000/01/21 11:01:39  boe
 * Überarbeitet und an Version 4.0 angepasst
 *
 *
 *
 *------------------------------------------------------------------
 */

/*
*  \file cmsevent.c
*  \author port GmbH Halle (Saale)
*  $Revision$
*  $Date$
*
*++ This module contains functions for CAL CMS event handling.
*-- Dieses modul enthält Funktionen für die CAL CMS Event Behandlung.
*/


/* header of standard C - libraries */

#include <string.h>
#include <stdio.h>
#include <assert.h>

/* header of project specific types */

#include <cal_conf.h>
#include "cmsevent.h"
#include "nmt.h"
#include "drv.h"
#include "cmscodec.h"

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




/*******************************************************************
*
* findEvAddr - get the address of event entry
*
* \internal
*
* The function returns the address of an event
* with the given COB-Id
*
* \retval 
* event address if event with evNr exists
* else NULL
*
*/
CMS_EVENT_T *findEvAddr(
        CMS_EVENT_T *pFirstEv,  /* pointer to eventlist */
        UNSIGNED16  cobId       /* COB-Id */
        )
{
CMS_EVENT_T *pEvInUse;          /* pointer to event */

    pEvInUse = pFirstEv;
    while (pEvInUse != NULL) {
    if (pEvInUse->pCOB->wID == cobId) {
        return(pEvInUse);
    }
    pEvInUse = pEvInUse->pNext;
    }

    return(NULL);
}


#if defined(CONFIG_EMCY_PRODUCER) || defined(CONFIG_TIME_PRODUCER)
/*******************************************************************
*
* CMS_TransEvent_req - transmit event request
*
* \internal
*
* The function transmit an event 
* with the given event pointer and description
* It checks the correct node state before
* and generate the message.
* If an error occurres,
* the appriotate retval is returned
*
* \retval CO_OK
*++ success
* \retval CO_E_STATE
*++ bad node state
* \retval CO_E_INHIBITED
*++ inhibit time is not over
*
*/
RET_T CMS_TransEvent_req(
      CMS_EVENT_T   *pEv,     /* address of event */
      PDO_MAP_T     *pDesc    /* Pointer to description */
      )
{
UNSIGNED8   pData[8];   /* transmit buffer */

    assert(pEv != NULL);

    assert(co_pNode  != NULL);

    if ((co_pNode  ->eState != OPERATIONAL) 
     && (co_pNode  ->eState != PRE_OPERATIONAL))  {
        return(CO_E_STATE);
    }

    CMS_Encode(pDesc, (UNSIGNED8 *)pEv->pMapEntries, pData);

# ifdef CONFIG_FULLCAN
    UPDATE_COB(pEv->pCOB, pData);
# endif /* CONFIG_FULLCAN */

    if (pEv->inhibit.ticks > 0)  {
        return(CO_E_INHIBITED);
    }

    TRANSMIT_COB(pEv->pCOB, pData);
    if (pEv->wInhibitTime > 0)  {
    startInhibitTimer(&pEv->inhibit, pEv->wInhibitTime CO_COMMA_LINE_PARA);
    }
    return(CO_OK);
}
#endif /* defined(CONFIG_EMCY_PRODUCER) || defined(CONFIG_TIME_PRODUCER) */


/*******************************************************************
*
* CMS_DefEvent_req - define an event
*
* \internal
*
* The function create an event entry
* at the given event list
* and save all necessary values on it.
* It requests memory for the new event
* and create a new cob-structure.
*
* \retval RET_T
*/

RET_T CMS_DefEvent_req(CMS_EVENT_T  **ppEvFirst, // Pointer to address to event list
                       CMS_EVENT_T  **pEv,       // Pointer to actual event
                       COB_KIND_T   cobKind)     // kind of cob
{
CMS_EVENT_T *pEvInUse,            // pointer to actual event
            *pLastEvEntry = NULL; // pointer to last event

  // search end of event list
  pEvInUse = *ppEvFirst;
  if (pEvInUse != NULL)
  {
    do
    {
      pLastEvEntry = pEvInUse;
    } while ((pEvInUse = pLastEvEntry->pNext) != NULL);
  }

  if ((pEvInUse = (CMS_EVENT_T *)CalMalloc(sizeof(CMS_EVENT_T))) == NULL)
  {
    return(CO_E_MEM);
  }

  if ((pEvInUse->pCOB = DEFINE_COB(cobKind, 0 CO_COMMA_LINE_PARA)) == NULL)
  {
    return(CO_E_NO_DATABASE);
  }

  if (*ppEvFirst == NULL)
  {
    *ppEvFirst = pEvInUse;
    pEvInUse->pNext = NULL;
  }
  else
  {
    pLastEvEntry->pNext = pEvInUse;
    pEvInUse->pNext = NULL;
  }

  // return the new event address
  *pEv = pEvInUse;

  return(CO_OK);
}

/*______________________________________________________________________EOF_*/

