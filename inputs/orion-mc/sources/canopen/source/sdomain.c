/*
 *++ sdomain - subroutines needed for domain transfers
 *-- sdomain - Unterprogramme für Domain Transfer
 *
 * Copyright (c) 2001-2002 port GmbH Halle (Saale)
 *------------------------------------------------------------------
 *
 * $Id$
 *
 *------------------------------------------------------------------
 *
 * modification history
 * --------------------
 * $Log$
 * Revision 1.0  2008-03-07 17:04:47+01  driet
 * Initial revision
 *
 * Revision 2.4  2002/11/18 09:47:54  boe
 * add comments/adapt on doxygen
 * add co_ to all global library variables
 *
 * Revision 2.3  2002/05/21 14:28:16  boe
 * copyright changed
 *
 * Revision 2.2  2001/10/15 13:04:34  ro
 * include added
 *
 * Revision 2.1  2001/01/26 11:22:36  boe
 * functions for sdo main usage
 *
 *
 *
 *------------------------------------------------------------------
 */

/*
*  \file sdomain.c
*  \author port GmbH Halle (Saale)
*  $Revision$
*  $Date$
*
*++ This module contains support functions for domain transfer.
*-- Dieses Modul enthält Hilfsfunktionen für den Domaintransfer.   
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
#include <co_debug.h>
#include "sdomain.h"
#include "sdo.h"
#include "nmt.h"

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
* sdoMsgReceived - sdo receive function
*
* NOMANUAL
*
* this function calls the corresponding coding or
* encoding functions to transfer a multiplexed domain.
* it calls the response or confirmation primitives too.
*
* \retval
*	nothing
*/

void sdoMsgReceived(
    CAN_MSG_T *canMsg		/* Pointer to CAN Message */
	)
{
SDO_T *pIndicatedSdo;		/* pointer to actual sdo */

    assert(co_pNode  != NULL);
    if ( (co_pNode  ->eState != OPERATIONAL)
      && (co_pNode  ->eState != PRE_OPERATIONAL)) {
	return;
    }

    pIndicatedSdo = co_pFirstDomEntry ;
    while (pIndicatedSdo != NULL) {
	if ((pIndicatedSdo->pReqInd_COB->wID == canMsg->wCOB_ID)
	 || (pIndicatedSdo->pResCon_COB->wID == canMsg->wCOB_ID)) {
	    break;
	}
	pIndicatedSdo = pIndicatedSdo->pNext;
    }

    if (pIndicatedSdo == NULL) {
	return;
    }

    /* SDO disabled */
    if (pIndicatedSdo->state == SDOSTATE_DISABLED)  {
	return;
    }

    /* DOMAIN SERVER */
    if (pIndicatedSdo->userType == SERVER) {
#ifdef CONFIG_SDO_SERVER
	sdoServerMsgInd(pIndicatedSdo, canMsg);
#endif 	/* defined(SDO_SERVER) */
    }

#if defined(CONFIG_SDO_CLIENT)
    else   {/* DOMAIN CLIENT */
	sdoClientMsgCon(pIndicatedSdo, canMsg);
    } /* end if */
#endif /* defined(SDO_CLIENT) */
}


/*****************************************************************
*
* CMS_DomExist - get pointer to domain structure
*
* NOMANUAL
*
* RETURNS
* .TP
* pointer to domain structure
*
*/

SDO_T *CMS_DomExist(
	UNSIGNED8  num,		/* internal number of domain */
	USER_T     kindOfUse	/* CLIENT/SERVER */
     )
{
SDO_T *pDomInUse;		/* pointer to actual sdo */

    if ((pDomInUse = co_pFirstDomEntry  ) == NULL)  {
	return(NULL);
    }
    while (pDomInUse != NULL) {
	if ((pDomInUse->num == num) && (pDomInUse->userType == kindOfUse))  {
	   return(pDomInUse);
	}
	pDomInUse = pDomInUse->pNext;
    }
    return(NULL);
}


#ifdef CONFIG_CO_DEBUG
/*****************************************************************
*
* printSdoState - debug print actual sdo state
*
* NOMANUAL
*
* RETURNS
* .TP
* nothing
*
*/
void printSdoState(char *fctName, SDO_T *pSdo)
{
    /* BDEBUG(CO_DEBUG_SDO, "%s: state = ", fctName); */
    BDEBUG(CO_DEBUG_SDO, "state = ");
    switch (pSdo->state)  {
	case SDOSTATE_DISABLED:
	    BDEBUG(CO_DEBUG_SDO, "disabled");
	    break;
	case SDOSTATE_READY:
	    BDEBUG(CO_DEBUG_SDO, "ready");
	    break;
	case SDOSTATE_DNLD_INIT:
	    BDEBUG(CO_DEBUG_SDO, "init download");
	    break;
	case SDOSTATE_DNLD_SEG:
	    BDEBUG(CO_DEBUG_SDO, "download segment");
	    break;
	case SDOSTATE_DNLD_BLK_INIT:
	    BDEBUG(CO_DEBUG_SDO, "init block download");
	    break;
	case SDOSTATE_DNLD_BLK_SEG:
	    BDEBUG(CO_DEBUG_SDO, "download block segment");
	    break;
	case SDOSTATE_DNLD_BLK_END:
	    BDEBUG(CO_DEBUG_SDO, "download block end");
	    break;
	case SDOSTATE_UPLD_BLK_INIT:
	    BDEBUG(CO_DEBUG_SDO, "init block upload");
	    break;
	case SDOSTATE_UPLD_BLK_SEG:
	    BDEBUG(CO_DEBUG_SDO, "upload block segment");
	    break;
	case SDOSTATE_UPLD_BLK_END:
	    BDEBUG(CO_DEBUG_SDO, "upload block end");
	    break;
	default:
	    BDEBUG(CO_DEBUG_SDO, "unknwon state 0x%x", pSdo->state);
    }
    BDEBUG(CO_DEBUG_SDO, "\n");
}
#endif /* CONFIG_CO_DEBUG */
/*______________________________________________________________________EOF_*/
