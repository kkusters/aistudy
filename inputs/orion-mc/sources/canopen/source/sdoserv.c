/*
 *++ sdoserv - subroutines needed for sdo server transfers
 *-- sdoserv - Unterprogramme für SDO Server Transfer
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
 * Revision 2.21  2003/10/01 13:34:32  boe
 * testSdoValue gets sdo transfer size for domains (not domain size)
 * domain transfer allowed for expitited transfer
 *
 * Revision 2.20  2003/09/26 12:30:50  boe
 * SPLIT_INDICATION usable for multi line
 *
 * Revision 2.19  2003/08/28 07:23:46  boe
 * call testSdoValue() also for domain transfers
 *
 * Revision 2.18  2003/07/22 07:24:30  boe
 * add addiditionally indication for bootloader mode
 * set correct numeric bit for all actions with 16bit cpus
 *
 * Revision 2.17  2003/06/13 14:12:33  boe
 * add includes and defines for blocktransfer
 * if size from sdo transfer less than object size, use it
 *
 * Revision 2.16  2003/02/13 09:50:21  boe
 * restore values also for split_indication
 *
 * Revision 2.15  2002/12/11 08:11:56  boe
 * change abort transfer value for non segmented transfer to SDO_OTHER
 *
 * Revision 2.14  2002/11/18 09:50:27  boe
 * add comments/adapt on doxygen
 * add co_ to all global library variables
 * get more exact cob-type to driver
 * debug messages bracketed by CONFIG_CO_DEBUG
 * replace mask by defines
 * add special things for 16bit CPUs
 *
 * Revision 2.13  2002/10/30 09:58:59  boe
 * transfer size information to getObjAddr
 * init unknown size for getObjAddr() calls
 *
 * Revision 2.12  2002/08/30 08:29:34  boe
 * change return value for testSdoValue
 *
 * Revision 2.11  2002/05/21 14:51:51  boe
 * add new functionlity to split sdoRdInd() and sdoWrInd()
 * and finish it by finishSdoRdInd() and finishSdoWrInd()
 *
 * Revision 2.10  2002/03/26 08:20:20  boe
 * add NEW_TIMER functionality over defines
 * add returnvalue for sdoWrInd()
 *
 * Revision 2.9  2001/12/05 15:46:23  boe
 * check returnvalue from sdoWrInd() and set the SDO abort code
 * return bad sdo codes with abort sdo transfer
 * only correct datatypes are allowed
 *
 * Revision 2.8  2001/09/14 04:07:55  boe
 * non volatile memory part reprocessed
 *
 * Revision 2.7  2001/05/17 09:42:06  boe
 * explicite type conversion to remove compiler warnings
 *
 * Revision 2.6  2001/04/05 12:26:59  boe
 * command line parameter for single line changed to void
 *
 * Revision 2.5  2001/03/28 12:52:27  boe
 * additionally include file removed
 *
 * Revision 2.4  2001/03/19 15:55:27  ro
 * sdomgr.h only included if CONFIG_DYN_SDO_CONNECTION_MANAGER defined
 *
 * Revision 2.3  2001/03/14 16:09:17  ro
 * header nmt.h added
 * setDefSdoCobId() for default COB-ID for first SDO added
 *
 * Revision 2.2  2001/02/26 14:41:29  boe
 * driver access functions replaced by macros
 *
 * Revision 2.1  2001/01/26 11:24:12  boe
 * functions for sdo server usage
 *
 *
 *
 *------------------------------------------------------------------
 */

/**
*  \file sdoserv.c
*  \author port GmbH Halle (Saale)
*  $Revision$
*  $Date$
*
*++ This module contains support functions for sdo server transfers.
*-- Dieses Modul enthält Hilfsfunktionen für den SDO Server Transfer
*
*/

/* header of standard C - libraries */

#include <string.h>
#include <stdio.h>

/* header of project specific types */

#include <cal_conf.h>
#include <co_debug.h>
#include <co_mcpy.h>
#include <co_setcp.h>
#include <co_odidx.h>
#include "sdo.h"
#include "cmscodec.h"
#include "access.h"
#include "nmt.h"
#include "nmt_s.h"
#include "drv.h"

#ifdef CONFIG_NON_VOLATILE_MEM
# include <co_stor.h>
#endif /* CONFIG_NON_VOLATILE_MEM */

#ifdef CONFIG_DYN_SDO_CONNECTION_MANAGER
# include <sdomgr.h>
#endif /* CONFIG_DYN_SDO_CONNECTION_MANAGER */

#ifdef CONFIG_16BIT_CPU
# include <utility.h>
#endif /* CONFIG_16BIT_CPU */

# ifdef CONFIG_SDO_BLOCKTRANSFER
# include <sdoblock.h>
# endif /* CONFIG_SDO_BLOCKTRANSFER */

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
void initDnLd_ind(SDO_T	*pCurSdo, UNSIGNED8 *canBuf );
void initUpLd_res(SDO_T	*pCurSdo);
void initDnLd_res(SDO_T	*pCurSdo);
void dnLdSeg_ind(SDO_T	*pCurSdo, UNSIGNED8 *pCanBuf );
void upLdSeg_ind(SDO_T	*pCurSdo);
void dnLdSeg_res(SDO_T	*pCurSdo);
void writeSdoValue(SDO_T *pCurSdo, UNSIGNED8 *pData );
RET_T initUpLd_ind_finish(SDO_T	*pCurSdo );
void writeSdoValue_finished(SDO_T *pCurSdo );

/* external variables
---------------------------------------------------------------------------*/

/* global variables
---------------------------------------------------------------------------*/

/* local defined variables
---------------------------------------------------------------------------*/
#ifdef CONFIG_RCS_IDENT
static char _rcsid[] = "$Id$";
#endif /* CONFIG_RCS_IDENT */


#ifdef CONFIG_SDO_SERVER

/*****************************************************************
*
* sdoServerMsgInd - sdo indication for server
*
* NOMANUAL
*
* RETURNS
* .TP
* nothing
*
*/
void sdoServerMsgInd(
	SDO_T *pSdo,		/* Pointer to actual SDO structure */
	CAN_MSG_T *canMsg	/* Pointer to CAN Message */
	)
{

# ifdef CONFIG_SDO_BLOCKTRANSFER
    if (pSdo->state == SDOSTATE_DNLD_BLK_SEG)  {
	dnLdBlk_ind(pSdo, canMsg->pData);
	return;
    }
# endif /* CONFIG_SDO_BLOCKTRANSFER */

    switch (canMsg->pData[0] & CO_SDO_SCS_MASK) {
	case CCS_INI_DN_LD_REQ : /* SERVER Initiate Download Indication */
	    if (pSdo->state != SDOSTATE_READY)  {
# ifdef CONFIG_CO_DEBUG
		BDEBUG(CO_DEBUG_SDO, "Bad State for received Message\n");
# endif /* CONFIG_CO_DEBUG */
		return;
	    }
	    pSdo->toggleBit = SDO_TOGGLE_BIT;
# ifdef CONFIG_CO_DEBUG
	    BDEBUG(CO_DEBUG_SDO, "init Download Indication\n");
# endif /* CONFIG_CO_DEBUG */
	    initDnLd_ind(pSdo, canMsg->pData CO_COMMA_LINE_PARA);
	    break;

# ifdef CONFIG_SEG_SDO
	case CCS_DN_LD_SEG_REQ : /* SERVER Download Segment Indication */
	    if (pSdo->state != SDOSTATE_DNLD_SEG)  {
#  ifdef CONFIG_CO_DEBUG
		BDEBUG(CO_DEBUG_SDO, "Bad State for received Message\n");
#  endif /* CONFIG_CO_DEBUG */
		return;
	    }
	    /* if toggle-bit not alternates, ignore message */
	    if (pSdo->toggleBit == (canMsg->pData[0] & SDO_TOGGLE_BIT)) {
		return;
	    }

#  ifdef CONFIG_CO_DEBUG
	    BDEBUG(CO_DEBUG_SDO, "Download Segment Indication\n");
#  endif /* CONFIG_CO_DEBUG */
	    pSdo->toggleBit = (canMsg->pData[0] & SDO_TOGGLE_BIT);
	    dnLdSeg_ind(pSdo, canMsg->pData CO_COMMA_LINE_PARA);
	    break;
# endif /* not CONFIG_SEG_SDO */

	case CCS_INI_UP_LD_REQ : /* SERVER Initiate Upload Indication */
	    if (pSdo->state != SDOSTATE_READY)  {
# ifdef CONFIG_CO_DEBUG
		BDEBUG(CO_DEBUG_SDO, "Bad State for received Message\n");
# endif /* CONFIG_CO_DEBUG */
		abortSdoTransf_Req(pSdo, CO_E_SDO_CMD_SPEC_INVALID);
		return;
	    }
	    pSdo->toggleBit = SDO_TOGGLE_BIT;
# ifdef CONFIG_CO_DEBUG
	    BDEBUG(CO_DEBUG_SDO, "init Upload Indication\n");
# endif /* CONFIG_CO_DEBUG */
	    if (initUpLd_ind(pSdo, canMsg->pData CO_COMMA_LINE_PARA) == CO_OK) {
		initUpLd_res(pSdo);
	    }
	    break;

# ifdef CONFIG_SEG_SDO
	case CCS_UP_LD_SEG_REQ : /* SERVER Upload Segment Indication */
	    if (pSdo->state != SDOSTATE_UPLD_INIT)  {
#  ifdef CONFIG_CO_DEBUG
		BDEBUG(CO_DEBUG_SDO, "Bad State for received Message\n");
#  endif /* CONFIG_CO_DEBUG */
		return;
	    }
	    /* if toggle not alternates ignore message */
	    if (pSdo->toggleBit == (canMsg->pData[0] & SDO_TOGGLE_BIT)) {
		abortSdoTransf_Req(pSdo, CO_E_SDO_INVALID_TOGGLEBIT);
		return;
	    }
	    pSdo->toggleBit = (canMsg->pData[0] & SDO_TOGGLE_BIT);
#  ifdef CONFIG_CO_DEBUG
	    BDEBUG(CO_DEBUG_SDO, "Upload Segment Indication\n");
#  endif /* CONFIG_CO_DEBUG */
	    upLdSeg_ind(pSdo);
	    break;
# endif /* not CONFIG_SEG_SDO */

# ifdef CONFIG_SDO_BLOCKTRANSFER
	case CO_SDOBLK_CCS_DOWN:	/* SERVER Init/End Block Download */
	    /* select init or end of block transfer */
	    if ((canMsg->pData[0] & CO_SDOBLK_SS_END) != 0) {
#  ifdef CONFIG_CO_DEBUG
		BDEBUG(CO_DEBUG_SDO, "End Download Block Indication\n");
#  endif /* CONFIG_CO_DEBUG */
		endDnLdBlk_ind(pSdo, canMsg->pData);
	    } else {
#  ifdef CONFIG_CO_DEBUG
		BDEBUG(CO_DEBUG_SDO, "Init Download Block Indication\n");
#  endif /* CONFIG_CO_DEBUG */
		initDnLdBlk_ind(pSdo, canMsg->pData CO_COMMA_LINE_PARA);
	    }
	    break;

	case CO_SDOBLK_CCS_UP:	/* SERVER Init Block Upload */
	    if ((pSdo->state != SDOSTATE_UPLD_BLK_INIT) &&
	        (pSdo->state != SDOSTATE_UPLD_BLK_SEG) &&
		(pSdo->state != SDOSTATE_UPLD_BLK_END) &&
		(pSdo->state != SDOSTATE_READY)) {
#  ifdef CONFIG_CO_DEBUG
		BDEBUG(CO_DEBUG_SDO, "Bad State for received Message\n");
#  endif /* CONFIG_CO_DEBUG */
		/* Abort Transfer */
		abortSdoTransf_Req(pSdo, CO_E_SDO_CMD_SPEC_INVALID);
		return;
	    }

#  ifdef CONFIG_CO_DEBUG
	    BDEBUG(CO_DEBUG_SDO, "Upload Block Indication\n");
#  endif /* CONFIG_CO_DEBUG */
	    initUpLdBlk_ind(pSdo, canMsg->pData CO_COMMA_LINE_PARA);
	    break;
# endif /* CONFIG_SDO_BLOCKTRANSFER */

	case CS_ABORT_TRANSFER:		/* Abort Transfer */
	    /* for CANopen application will not informed */
	    pSdo->state = SDOSTATE_READY;
	    break;

	default:	/* unknown error */
	    CMS_SdoDecode(&pSdo->odIndex, canMsg->pData);
	    abortSdoTransf_Req(pSdo, CO_E_SDO_CMD_SPEC_INVALID);
	    break;
     }
}


/*******************************************************************
*
* initDnLd_ind - Download SDO indication for CANopen
*
* NOMANUAL
*
* This function indicate a download domain request of a CANopen client.
* It puts the domain data to the objectdictionary.
* A buffer will be allocated for SDOs, which contain more than 7 Bytes.
*
* RETURNS
* .TP
* nothing
*
*/

void initDnLd_ind(
	SDO_T	*pCurSdo,	/* pointer to current sdo */
	UNSIGNED8 *canBuf	/* pointer to can buffer */
     )
{
RET_T 		commonRet;	/* common return value */
UNSIGNED8	*pAddr;		/* pointer to data address */
UNSIGNED32      objSize;	/* size of data */
UNSIGNED16 	index;		/* index */
UNSIGNED8	subIndex;	/* subindex */
UNSIGNED8	*pData;		/* pointer to SDO data */
UNSIGNED32	sdoSize = 0;	/* SDO size */
UNSIGNED8	attr;		/* flag if object is a domain */

    CMS_SdoDecode(&pCurSdo->odIndex, canBuf);

    /* detect the transfer typ */
    switch (*canBuf & CO_SDO_SIZE_TYPE_MASK) {
	/*
	 * normal transfer
	 * data set size is indicated
	 * d contains number of data bytes to be downloaded
	 */
	case 1:
	    CO_PACK_MEMCPY((UNSIGNED8 *)&sdoSize, canBuf + 4, 4, CO_NUM_VAL);
	    pData = NULL;
	    break;
	/*
	 * expedited transfer
	 * data set size is indicated
	 * d contains data
	 * n contains data number
	 */
	 /* if no size indicator size always 4 */
	case 2:
	    sdoSize = 4;
	    pData = canBuf + 4;
	    break;

	case 3:
	    sdoSize = 4 - ((*canBuf & 0x0C) >> 2);
	    pData = canBuf + 4;
	    /* pCurSdo->state = SDOSTATE_DNLD_SEG; */
	    break;

	default:
	   return;
    }

    /* get index and subIndex */
    /* for faster access */
    index = pCurSdo->odIndex.index;
    subIndex = pCurSdo->odIndex.subIndex;

    /* get target address */
#ifdef CONFIG_VIRTUAL_OBJECTS
    /* for virtual objects put trough the len information
       it will be overwritten by getObjAddr */
    objSize = sdoSize;
#endif /* CONFIG_VIRTUAL_OBJECTS */

    commonRet = getObjAddr(index, subIndex, &pAddr, &objSize
		 CO_COMMA_LINE_PARA);

    attr = getObjAttr(index, subIndex CO_COMMA_LINE_PARA);

    /* test target space size against data size and for valid address */
    if (commonRet == CO_OK) {

# ifdef CONFIG_DOMAIN_UPDNLD
	/*
	    exception for domains
	    the default value entry of the type descripion
	    contains the true size value and
	    the object entry contains only a pointer to the domain
	*/
	if ((attr & CO_UP_DN_LD_DOMAIN) != 0) {
	    if ((sdoSize > getDomainSize(index, subIndex CO_COMMA_LINE_PARA))
	     || (pAddr == NULL)) {
		commonRet = CO_E_WRONG_SIZE;
	    }
	    pAddr = getDomainAddr(index, subIndex CO_COMMA_LINE_PARA);
	    if (pAddr == NULL)  {
		commonRet = CO_E_NONEXIST_OBJECT;
	    }
#  ifdef CONFIG_16BIT_CPU
	    /* set flag for domain download */
	    pCurSdo->numeric = CO_TRUE;
#  endif /* CONFIG_16BIT_CPU */

#  ifdef CONFIG_VALUE_CHECK_FUNCTION
	    /* user function for value test */
	    if (commonRet == CO_OK)  {
		commonRet = testSdoValue(index, subIndex, (void *)pData,
		    sdoSize CO_COMMA_LINE_PARA);
	    }
#  endif /* CONFIG_VALUE_CHECK_FUNCTION */
	} else {

# endif /* CONFIG_DOMAIN_UPDNLD */

# ifdef CONFIG_16BIT_CPU
	    /* reset flag for domain download */
	    pCurSdo->numeric = CO_FALSE;
# endif /* CONFIG_16BIT_CPU */

	    if ((attr & CO_NUM_VAL) != 0)  {
		/* numeric value */
		if (sdoSize != objSize)  {
		    commonRet = CO_E_WRONG_SIZE;
		}
	    } else {
		/* not numeric value */
		if ((sdoSize > objSize) || (pAddr == NULL))  {
		    commonRet = CO_E_WRONG_SIZE;
		}
	    }
# ifdef CONFIG_DOMAIN_UPDNLD
	}
# endif /* CONFIG_DOMAIN_UPDNLD */
    }

    /* check write permission */
    if (commonRet == CO_OK) {
	if ((attr & CO_WRITE_PERM) == 0)  {
	    commonRet = CO_E_NO_WRITE_PERM;
	}
    }

    /*
      If size of indicated SDO to large, target address invalid
      or access error to Object Dictionary
      return with error (Abort Domain Transfer)
    */

    if (commonRet != CO_OK) {
	abortSdoTransf_Req(pCurSdo, commonRet);
	return;
    }

    pCurSdo->domSize = sdoSize;
    pCurSdo->restSize = sdoSize;

    if (pData != NULL) {
	/* expedited transfer */
	writeSdoValue(pCurSdo, pData CO_COMMA_LINE_PARA);
	/* dnLdSeg_res will be done in writeSdoValue */
	return;
    }

# ifdef CONFIG_SEG_SDO
    /* set target address */
    pCurSdo->pActualDomData = pAddr;
    pCurSdo->pDomData = pAddr;
    initDnLd_res(pCurSdo);
    pCurSdo->state = SDOSTATE_DNLD_SEG;
#  ifdef CONFIG_16BIT_CPU
    pCurSdo->halfWord = CO_FALSE;
#  endif /* CONFIG_16BIT_CPU */
# endif /* not CONFIG_SEG_SDO */
}


/*******************************************************************
*
* initUpLd_ind - Upload SDO indication for CANopen
*
* NOMANUAL
*
* This function indicate a upload domain request of a CANopen client.
* It gets the SDO data from the objectdictionary.
*
* \retval CO_OK
*++ success
*-- Erfolg
* else
*++ SDO abort codes
*-- SDO Abbruch Codes
*/

RET_T initUpLd_ind(
    SDO_T	*pCurSdo,	/* pointer to current sdo */
    UNSIGNED8	*canBuf		/* pointer to can buffer */
     )
{
RET_T 		commonRet;  /* common return value */
UNSIGNED8	*pData;	    /* pointer to data address */
UNSIGNED16 	index;      /* index */
UNSIGNED8	subIndex;   /* subindex */
UNSIGNED32	tmpSize = 0xffffffff;    /* size of data */
UNSIGNED8	attr;	    /* attribute */
#ifdef CONFIG_SPLIT_INDICATION
#else /* CONFIG_SPLIT_INDICATION */
# if defined(CONFIG_BIG_ENDIAN) || defined(CONFIG_16BIT_CPU)
static UNSIGNED8 tmpBuf[4]; /* convert buffer */
# endif /* defined(CONFIG_BIG_ENDIAN) || defined(CONFIG_16BIT_CPU) */
#endif /* CONFIG_SPLIT_INDICATION */

    CMS_SdoDecode(&pCurSdo->odIndex, canBuf);

    /* for faster access */
    index = pCurSdo->odIndex.index;
    subIndex = pCurSdo->odIndex.subIndex;

    /* read contents of object dictionary to SDO */
    commonRet = getObjAddr(index, subIndex, &pData,&tmpSize CO_COMMA_LINE_PARA);
    if (commonRet != CO_OK) {
	return(abortSdoTransf_Req(pCurSdo, commonRet));
    }

    /* test read permission */
    attr = getObjAttr(index, subIndex CO_COMMA_LINE_PARA);
    if ((attr & CO_READ_PERM) != CO_READ_PERM) {
	return(abortSdoTransf_Req(pCurSdo, CO_E_NO_READ_PERM));
    }

# ifdef CONFIG_NON_VOLATILE_MEM
    if ((index == STORE_PARA_INDEX) && (subIndex != 0))  {
	/* change to the default value
	 * - Device don't save parameters autinomously 
	 * - Device save parameters on command
	 */
	*((UNSIGNED32 *)pData) = STORE_PARA_ON_COMMAND;
    }
    if ((index == RESTORE_DEF_PARA_INDEX) && (subIndex != 0))  {
	/* change to the default value
	 * - Device restores parameters
	 */
	*((UNSIGNED32 *)pData) = RESTORE_PARA_ON_COMMAND;
    }
# endif /*  CONFIG_NON_VOLATILE_MEM */

    /* inform application */
    /* transmit actual value from device to object dictionary */

# ifdef CONFIG_SPLIT_INDICATION
    if ((commonRet = sdoRdInd(index, subIndex, pCurSdo->num CO_COMMA_LINE_PARA))
		!= CO_OK) {
	if (commonRet != CO_SDO_IND_BUSY) {
	    return(abortSdoTransf_Req(pCurSdo, commonRet));
	}
	/* now we wait for finish the indication */
	/* therefore the user have to call the function finishSdoRdInd() */
	pCurSdo->state = SDOSTATE_IND_BUSY;
	return(commonRet);
    }
# else /* CONFIG_SPLIT_INDICATION */
    if ((commonRet = sdoRdInd(index, subIndex CO_COMMA_LINE_PARA)) != CO_OK) {
	return(abortSdoTransf_Req(pCurSdo, commonRet));
    }
# endif /* CONFIG_SPLIT_INDICATION */


# ifdef CONFIG_SPLIT_INDICATION
    /* generate answer */
    return initUpLd_ind_finish(pCurSdo CO_COMMA_LINE_PARA);
}


/****************************************************************************/
/*
*++ \brief initUpLd_ind_finish - finish initUpLd_ind function
*-- \brief initUpLd_ind_finish - beendet die initUpLd_ind Funktion
*
* NOMANUAL
*
*-- Diese Funktion beendet die initUpLd_ind Funktion
*
* \retval
*++ success
*-- siehe initUpLd_ind
*
*/
RET_T initUpLd_ind_finish(
    SDO_T	*pCurSdo
	)
{
RET_T 		commonRet = CO_OK;  /* common return value */
UNSIGNED8	*pData;	    /* pointer to data address */
UNSIGNED16 	index;      /* index */
UNSIGNED8	subIndex;   /* subindex */
UNSIGNED32	tmpSize = 0xffffffff;    /* size of data */
UNSIGNED8	attr;	    /* attribute */
#  if defined(CONFIG_BIG_ENDIAN) || defined(CONFIG_16BIT_CPU)
static UNSIGNED8 tmpBuf[4];
#  endif /* defined(CONFIG_BIG_ENDIAN) || defined(CONFIG_16BIT_CPU) */

    /* for faster access */
    index = pCurSdo->odIndex.index;
    subIndex = pCurSdo->odIndex.subIndex;

    /* get attribute again */
    attr = getObjAttr(index, subIndex CO_COMMA_LINE_PARA);
# endif /* CONFIG_SPLIT_INDICATION */

    /* read object dictionary to SDO again
     *	(after change from user)
     * this can't get an error, therefore this was done before*/ 
    getObjAddr(index, subIndex, &pData, &tmpSize CO_COMMA_LINE_PARA);

# ifdef CONFIG_DOMAIN_UPDNLD
    /*
	exception for domains
	the default value entry of the type descripion
	contains the true size value and
	the object entry contains only a pointer to the domain
     */
    if ((attr & CO_UP_DN_LD_DOMAIN) != 0) {
	pData = getDomainAddr(index, subIndex CO_COMMA_LINE_PARA);
	if (pData == NULL)  {
	    commonRet = CO_E_NONEXIST_OBJECT;
	}

#  ifdef CONFIG_16BIT_CPU
	/* set flag for domain upload */
	pCurSdo->numeric = CO_TRUE;
	pCurSdo->halfWord = CO_FALSE;
#  endif /* CONFIG_16BIT_CPU */

    } else {

# endif /* CONFIG_DOMAIN_UPDNLD */

# ifdef CONFIG_16BIT_CPU
	/* set flag for domain upload */
	pCurSdo->numeric = CO_FALSE;
# endif /* CONFIG_16BIT_CPU */

# if defined(CONFIG_BIG_ENDIAN) || defined(CONFIG_16BIT_CPU)
	if (tmpSize < 5) {
	    CO_UNPACK_MEMCPY(tmpBuf, pData, tmpSize, attr & CO_NUM_VAL);
	    pData = tmpBuf;
	}
# endif /* defined(CONFIG_BIG_ENDIAN) || defined(CONFIG_16BIT_CPU) */

# ifdef CONFIG_DOMAIN_UPDNLD
    }
# endif /* CONFIG_DOMAIN_UPDNLD */

    if (commonRet != CO_OK) {
	return(abortSdoTransf_Req(pCurSdo, commonRet));
    }

    pCurSdo->pActualDomData = pData;
    pCurSdo->pDomData = pData;
    pCurSdo->restSize = tmpSize;
    pCurSdo->domSize = tmpSize;

    return(commonRet);
}


# ifdef CONFIG_SPLIT_INDICATION
/****************************************************************************/
/**
*++ \brief finishSdoRdInd - finish sdoRdInd
*-- \brief finishSdoRdInd - Abschluß der SdoRdInd-Funktion
*
*++ This function finishes the sdoRdInd call
*++ if the function has returned
*-- Diese Funktion beendet die SdoRdInd Funktion,
*-- wenn diese vorher mit dem Rückgabewert
*\c CO_SDO_IND_BUSY
*-- verlassen wurde.
*
*-- Normalerweise wird nach Beendigung der SdoRdInd Funktion
*-- die Antwort für die SDO Anfrage generiert
*-- und zum Server zurückgesendet.
*-- Wenn der Anwender die Indikation Funktion
*-- mit diesem speziellen Rückgabewert verläßt,
*-- wird die Programmabarbeitung der SDO Anfrage unterbrochen
*-- und erst mit dem Aufruf dieser Funktion fortgesetzt.
*-- Der Anwender hat somit die Möglichkeit,
*-- Antworten auf SDO Anfragen hinauszuzögern,
*-- um z. B. externe Ereignisse abzuwarten.
*++ Normally the response of a request is generated and
*++ sent back to the SDO-Server
*++ after sdoRdInd was called.
*++ If SdoRdInd returns with this special value
*++ processing of finishSdoRdInd is discontinued and is only finished if
*++ it is called a second time from the user application..
*++ By doing so the user application has the opportunity
*++ to delay a SDO-response in order to wait for an external event.
*
* \retval CO_OK
*++ success
*-- Erfolg
* \retval CO_E_NOT_EXIST
*++ SDO doesn´t exist
*-- SDO existiert nicht
* \retval CO_E_STATE);
*++ SDO not up to date (Client doesn´t send abort or no waiting SDO)
*-- SDO nicht mehr aktuell (Client sendet abort, oder kein wartendes SDO)
*
*/
RET_T finishSdoRdInd(
	UNSIGNED8	sdoNr,		/**< SDO number */
	RET_T		retCode		/**< returnval sdoInd */
    )
{
SDO_T	*pCurSdo;

    /* search for sdo structure */
    if ((pCurSdo = CMS_DomExist(sdoNr, SERVER CO_COMMA_LINE_PARA)) == NULL)  {
	return(CO_E_NOT_EXIST);
    }

    /* check, if the indication is waiting and the client has not aborted */
    if ((pCurSdo->state & (SDOSTATE_IND_BUSY | SDOSTATE_READY))
	!= SDOSTATE_IND_BUSY)  {
	return(CO_E_STATE);
    }

    /* if retval from indication not CO_OK, start abort */
    if (retCode != CO_OK) {
	return(abortSdoTransf_Req(pCurSdo, retCode));
    }

    /* correct the state */
    pCurSdo->state = SDOSTATE_READY;

    initUpLd_ind_finish(pCurSdo CO_COMMA_LINE_PARA);
    initUpLd_res(pCurSdo);

    return(CO_OK);
}
# endif /* CONFIG_SPLIT_INDICATION */


# ifdef CONFIG_SPLIT_INDICATION
/****************************************************************************/
/**
*++ \brief finishSdoWrInd - finish sdoWrInd
*-- \brief finishSdoWrInd - Abschluß der SdoWrInd-Funktion
*
*-- Diese Funktion beendet die SdoWrInd Funktion,
*-- wenn diese vorher mit dem Rückgabewert
*++ This function will finish the function SdoWrInd 
*++ if 
* \c CO_SDO_IND_BUSY
*-- verlassen wurde.
*++ was returned by SdoWrInd.
*
*-- Normalerweise wird nach Beendigung der SdoWrInd Funktion
*-- die Antwort für die SDO Anfrage generiert
*-- und zum Server zurückgesendet.
*-- Wenn der Anwender die Indikation Funktion
*-- mit diesem speziellen Rückgabewert verläßt,
*-- wird die Programmabarbeitung der SDO Anfrage unterbrochen
*-- und erst mit dem Aufruf dieser Funktion fortgesetzt.
*-- Der Anwender hat somit die Möglichkeit,
*-- Antworten auf SDO Anfragen hinauszuzögern,
*-- um z. B. externe Ereignisse abzuwarten.
*++ Normally the response of a request is generated
*++ and is sent back to the SDO-Server 
*++ after SdoWrInd was called.
*++ If the SdoWrInd returns with this special value
*++ processing of finishSdoRdInd is discontinued and will only be 
*++ finished if
*++ it is called a second time from the user application..
*++ By doing so the user application has the opportunity
*++ to delay a SDO-response in order to wait for an external event.
*
*-- Um diese Funktion verwenden zu können,
*-- ist das define 
* CONFIG_SPLIT_INDICATION
*-- zu setzen.
*
* \retval CO_OK
*++ success
*-- Erfolg
* \retval CO_E_NOT_EXIST
*++ SDO doesn't exist
*-- SDO existiert nicht
* \retval CO_E_STATE);
*++ SDO not up to date (Client doesn´t send abort or no waiting SDO)
*-- SDO nicht mehr aktuell (Client sendet abort, oder kein wartendes SDO)
*
*/
RET_T finishSdoWrInd(
	UNSIGNED8	sdoNr,		/**< SDO number */
	RET_T		retCode		/**< returnval sdoInd */
    )
{
SDO_T		*pCurSdo;		/* pointer to current sdo */
UNSIGNED8	*pVar;			/* pointer to variable address */
UNSIGNED32	oldSize;

    /* search for sdo structure */
    if ((pCurSdo = CMS_DomExist(sdoNr, SERVER CO_COMMA_LINE_PARA)) == NULL)  {
	return(CO_E_NOT_EXIST);
    }

    /* check, if the indication is waiting and the client has not aborted */
    if ((pCurSdo->state & (SDOSTATE_IND_BUSY | SDOSTATE_READY))
	    != SDOSTATE_IND_BUSY)  {
	return(CO_E_STATE);
    }

    /* if retval from indication not CO_OK, start abort */
    if (retCode != CO_OK) {

	/* command failed, the data would be restored */
	if (pCurSdo->saved == CO_TRUE)  {
	    if (getObjAddr(pCurSdo->odIndex.index, pCurSdo->odIndex.subIndex,
			&pVar, &oldSize CO_COMMA_LINE_PARA)
		== CO_OK)  {
		CO_NUM_MEMCPY(pVar, &pCurSdo->oldVar, oldSize,
			objAttr & CO_NUM_VAL);

		/* set communication parameter bacj to old values */
		if (pCurSdo->odIndex.index <= END_COM_PROF) {
		    setCommPar(pCurSdo->odIndex.index, pCurSdo->odIndex.subIndex			CO_COMMA_LINE_PARA);
		}
	    }
	}
	abortSdoTransf_Req(pCurSdo, retCode);
	return(retCode);
    }

    /* correct the state */
    pCurSdo->state &= ~SDOSTATE_IND_BUSY;

    writeSdoValue_finished(pCurSdo CO_COMMA_LINE_PARA);

    return(CO_OK);
}
# endif /* CONFIG_SPLIT_INDICATION */


# ifdef CONFIG_SEG_SDO
/*******************************************************************
*
* dnLdSeg_ind - download SDO segment indication for CANopen
*
* NOMANUAL
*
* This function concatenate the SDO segments and put the object to the
* dictonary of the the server.
*
* RETURNS
*	nothing
*
*/

void dnLdSeg_ind(
	SDO_T	*pCurSdo,  /* pointer to SDO */
	UNSIGNED8 *pCanBuf  /* pointer to SDO data */
     )
{
#  ifdef CONFIG_BOOT_LOADER
				/* 128 byte border */
#define NEXT_128_BORDER(val)	(((val) / 128) * 128)
				/* following 7 border */
#define NEXT_7_BORDER(val)	((val) ? ((val) + (7 - ((val) % 7))) : 0)
UNSIGNED8	overSize;	/* bytes more than 128 */
UNSIGNED32	actCnt;		/* actual size */
UNSIGNED32	lb, b128;	/* next 128 byte limit */
UNSIGNED32	g7;		/* next 7 limit after 128 byte limit */
#  endif /* CONFIG_BOOT_LOADER */

UNSIGNED8	size;		/* segmentsize */
RET_T		commonRet;	/* return value */

    size = 7 - ((*pCanBuf >> 1) & 7);
    /* copy data to target address */
    if (pCurSdo->pActualDomData == 0)  {
	return;
    }

    /* chor for to many data */
    if (pCurSdo->restSize < size)  {
	abortSdoTransf_Req(pCurSdo, CO_E_MEM);
	return;
    }

    pCurSdo->restSize -= size;

    /* don't save data if object size <= 4 */
    if (pCurSdo->domSize > 4) {
 
#  ifdef CONFIG_16BIT_CPU
	/* if domain download */
	if (pCurSdo->numeric == CO_TRUE)  {
	    pCurSdo->pActualDomData =
		pack_oddmemcpy(pCurSdo->pActualDomData, pCanBuf + 1,
		    size, &pCurSdo->halfWord);
	} else
#  endif /* CONFIG_16BIT_CPU */
	{
	    CO_MEMCPY(pCurSdo->pActualDomData, (pCanBuf + 1), size);

	    /* increment target adress pointer */
	    pCurSdo->pActualDomData += size;
	}

#  ifdef CONFIG_BOOT_LOADER
	/* set actual byte count */
	actCnt = pCurSdo->domSize - pCurSdo->restSize;
	/* look for next 128 limit */
	b128 = NEXT_128_BORDER(actCnt);
	/* get next 7 byte limit after 128 byte limit */
	g7 = NEXT_7_BORDER(b128);

	/* new 128-7 byte limit reached ? */
	if (actCnt == g7)  {
	    /* get last 128-7 limit */
	    lb = NEXT_7_BORDER(b128 - 128);

	    overSize = actCnt % 128;
	    /* call indication function */
	    commonRet = sdoBootLoadInd(g7 - lb - overSize, overSize);
	    if (commonRet != CO_OK)  {
		abortSdoTransf_Req(pCurSdo, commonRet);
		return;
	    }
	    /* reset pointer */
	    pCurSdo->pActualDomData = pCurSdo->pDomData;
	}
#  endif /* CONFIG_BOOT_LOADER */
    }

    /* last message */
    if ((*pCanBuf & CO_SDO_LAST) != 0) {

#  ifdef CONFIG_BOOT_LOADER

	/* set actual byte count */
	actCnt = pCurSdo->domSize - pCurSdo->restSize;
	/* look for next 128 limit */
	b128 = NEXT_128_BORDER(actCnt);
	/* get next 7 byte limit after 128 byte limit */
	g7 = NEXT_7_BORDER(b128);

	/* if > 128 bytes */
	if (g7 != 0)  {
	    /* next 128-7 limit not reached ? */
	    if (actCnt < g7)  {
		b128 -= 128;
		/* use last 128-7 limit */
		g7 = NEXT_7_BORDER(b128);
	    }
	}

	commonRet = sdoBootLoadInd(actCnt - g7, 0);
	if (commonRet != CO_OK)  {
	    abortSdoTransf_Req(pCurSdo, commonRet);
	    return;
	}
#  endif /* CONFIG_BOOT_LOADER */

	if (pCurSdo->domSize < 5) {
	    writeSdoValue(pCurSdo, pCanBuf + 1 CO_COMMA_LINE_PARA);
	    /* dnLdSeg_res will be done in writeSdoValue */
	    return;
	} else {

#  if defined(CONFIG_PDO_CONSUMER) && defined(CONFIG_MPDO_SRC)
	    /* today only MPDO Consumer SRC Mode receive an Unsigned64 */
	    if (pCurSdo->odIndex.index <= END_COM_PROF)  {
		if ((commonRet = setCommPar(pCurSdo->odIndex.index,
			pCurSdo->odIndex.subIndex CO_COMMA_LINE_PARA))
			!= CO_OK)  {
		    abortSdoTransf_Req(pCurSdo, commonRet);
		    return;
		}
	    }
#  endif /* defined(CONFIG_PDO_CONSUMER) && defined(CONFIG_MPDO_SRC) */

	    /* informs application about new value */
#  ifdef CONFIG_SPLIT_INDICATION
	    if ((commonRet = sdoWrInd(pCurSdo->odIndex.index,
		    pCurSdo->odIndex.subIndex, pCurSdo->num CO_COMMA_LINE_PARA))
		 != CO_OK)  {
		if (commonRet != CO_SDO_IND_BUSY) {
		    abortSdoTransf_Req(pCurSdo, commonRet CO_COMMA_LINE_PARA);
		    return;
		}
		/* now we wait for finish the indication */
		/* therefore the user has to call the function finishSdoWrInd(*/
		pCurSdo->state = SDOSTATE_IND_BUSY;
		return;
	    }
#  else /* CONFIG_SPLIT_INDICATION */
	    if ((commonRet = sdoWrInd(pCurSdo->odIndex.index,
		    pCurSdo->odIndex.subIndex CO_COMMA_LINE_PARA)) != CO_OK)  {

		abortSdoTransf_Req(pCurSdo, commonRet);
		return;
	    }
#  endif /* CONFIG_SPLIT_INDICATION */
	}

	pCurSdo->state = SDOSTATE_READY;
    }
    dnLdSeg_res(pCurSdo);
}



/*******************************************************************
*
* upLdSeg_ind - upload SDO segment indication for CANopen
*
* NOMANUAL
*
* This function segments the object from the dictonary and load it up to
* the client.
*
* RETURNS
* .TP
* nothing
*
*/

void upLdSeg_ind(
	SDO_T	*pCurSdo		/* pointer to current sdo */
     )
{
UNSIGNED8 	size;			/* size of next segment */
UNSIGNED8 	contFlag;		/* continue transfer flag */

    if (pCurSdo->restSize > 7UL) {
	contFlag = CO_SDO_MORE;
	size = 7;
    } else {
	contFlag = CO_SDO_LAST;
	size = (UNSIGNED8)pCurSdo->restSize;
	pCurSdo->state = SDOSTATE_READY;
    }

    pCurSdo->pData[0] = SCS_UP_LD_SEG_RES | pCurSdo->toggleBit
		   | ((7 - size) << 1) | contFlag;

#  ifdef CONFIG_16BIT_CPU
    /* if domain download */
    if (pCurSdo->numeric == CO_TRUE)  {
	pCurSdo->pActualDomData =
	    unpack_oddmemcpy(pCurSdo->pData + 1, pCurSdo->pActualDomData,
		    size, &pCurSdo->halfWord);
    } else
#  endif /* CONFIG_16BIT_CPU */
    {
	CO_MEMCPY(&pCurSdo->pData[1], pCurSdo->pActualDomData, size);

	/* points to next valid segment */
	pCurSdo->pActualDomData += size;
    }
    pCurSdo->restSize -= size;

    TRANSMIT_COB(pCurSdo->pResCon_COB, pCurSdo->pData);
}


/*******************************************************************
*
* dnLdSeg_res - performs the response to the service download domain segment
*
* Mit diesem Funktions\%aufruf teilt der Client dem Server
* Erfolg oder Mißerfolg des Dienstes mit.
*
* Im Byte \fIbRemRes\fP wird das Ergebis als
* \&RES_SUCC oder RES_FAIL
* spezifiziert.
* Wird RES_FAIL angegeben
* sind die Fehlercodes
* .TP
* \&E_APPLICATION_REQ
* Anwenderanforderung
* .TP
* \&E_NO_RESSOURCES
* kein Speicher für die Daten verfügbar
*
* Der Fehlercode ist Anwenderspezifiziert für \fIbErrReason\fP = 1
* und Imlementierungsspezifisch für \fIbErrReason\fP >= 128.
*
* INTERNAL
* input: remote result, error reason
*/
void dnLdSeg_res(
	SDO_T	*pCurSdo		/* pointer to current sdo */
     )
{
UNSIGNED8	pData[8];		/* transmit data buffer */

    pData[0] = SCS_DN_LD_SEG_RES | pCurSdo->toggleBit;
    memset(&pData[1], 0x0, 7);
    TRANSMIT_COB(pCurSdo->pResCon_COB, pData);
}
# endif /* CONFIG_SEG_SDO */


/*******************************************************************
*
* initUpLd_res - response to the service initiate upload domain
*
* NOMANUAL
*
* This function generates an answer for init upload domain request
* to to sdo client.
*
* \retval
*	nothing
*
*/
void initUpLd_res(
	SDO_T	*pCurSdo	/* pointer to current sdo */
     )
{
UNSIGNED8	pData[8];	/* transmit buffer */
UNSIGNED32	dSize;		/* Domain size */

    dSize = pCurSdo->domSize;

    CMS_SdoEncode(&pCurSdo->odIndex, pData);

    if ((dSize != 0) && (dSize < 5))  { /* expedited transfer */
	pData[0] = (UNSIGNED8)(SCS_INI_UP_LD_RES | ((4 - dSize) << 2) |
			    CO_SIZE_VALID | EXPED_TRANSFER);
	/* both data fields are char fields, memcpy is possible */
	CO_MEMCPY(&pData[4], pCurSdo->pActualDomData, dSize);
    } else { /* segmented transfer */
# ifdef CONFIG_SEG_SDO
	pData[0] = SCS_INI_UP_LD_RES | CO_SIZE_VALID;
	CO_UNPACK_MEMCPY(&pData[4], (UNSIGNED8 *)&dSize, 4, CO_NUM_VAL);
	pCurSdo->state = SDOSTATE_UPLD_INIT;
# else /* CONFIG_SEG_SDO */
	abortSdoTransf_Req(pCurSdo, CO_E_SDO_OTHER);
	return;
# endif /* CONFIG_SEG_SDO */
    }
    TRANSMIT_COB(pCurSdo->pResCon_COB, pData);
}


/*******************************************************************
*
* initDnLd_res - response to the service initiate download domain
*
* NOMANUAL
*
* This function generates an answer for the init download domain
* request to the sdo server.
*
* \retval
*	nothing
*
*/

void initDnLd_res(
	SDO_T	*pCurSdo	/* pointer to current sdo */
     )
{
UNSIGNED8	pData[8];	/* transmit buffer */

	CMS_SdoEncode(&pCurSdo->odIndex, pData);
	memset(&pData[4], 0x0, 4);
	pData[0] = SCS_INI_DN_LD_RES;
	TRANSMIT_COB(pCurSdo->pResCon_COB, pData);
}


/*******************************************************************
*
* writeSdoValue - write the value of a SDO to the object dictionary
*
* NOMANUAL
*
* This function writes the value of a SDO
* to the object dictionary and tests it before .
* If an error occurs it calls abortSdoTransf_Req(),
* which initiate the abort_domain_transfer,
* otherwise the initDnLd_res()
* or dnLdSeg_res() service
*
* RETURNS
* .TP
* nothing
*
*/

void writeSdoValue(
    SDO_T	*pCurSdo,  /* pointer to SDO */
    UNSIGNED8   *pData     /* pointer to SDO data */
	    )
{
RET_T 		commonRet;     /* common return value */
UNSIGNED16 	index;         /* index */
UNSIGNED8	subIndex;      /* subindex */
UNSIGNED8	*pVar;         /* pointer to variable */
UNSIGNED32  	oldSize;       /* size of former variable */
UNSIGNED8	objAttr;	/* object attributes */
#ifdef CONFIG_SPLIT_INDICATION
#else /* CONFIG_SPLIT_INDICATION */
UNSIGNED8  	oldVar[4];     /* buffer for saving former variable value */
BOOL_T		saved = CO_FALSE; /* flag signs if old value stored */
#endif /* CONFIG_SPLIT_INDICATION */

    /* for faster access */
    index = pCurSdo->odIndex.index;
    subIndex = pCurSdo->odIndex.subIndex;

    objAttr = getObjAttr(index, subIndex CO_COMMA_LINE_PARA);

# ifdef	CONFIG_VALUE_CHECK_FUNCTION
    /* user function for value test */
    if ((commonRet = testSdoValue(index, subIndex, (void *)pData,
		pCurSdo->domSize
		CO_COMMA_LINE_PARA)) != CO_OK) {
	abortSdoTransf_Req(pCurSdo, commonRet);
	return;
    }
# endif /* CONFIG_VALUE_CHECK_FUNCTION */

#ifdef CONFIG_VIRTUAL_OBJECTS
    /* for virtual objects put trough the len information
       it will be overwritten by getObjAddr */
    oldSize = pCurSdo->domSize;
#endif /* CONFIG_VIRTUAL_OBJECTS */

    commonRet = getObjAddr(index, subIndex, &pVar, &oldSize
			CO_COMMA_LINE_PARA);

    if (commonRet == CO_OK) {
	/* if size from sdo transfer less than object size, use it */
	if (pCurSdo->domSize < oldSize )  {
	    oldSize = pCurSdo->domSize;
	}

	/* save old value only for data < 5 byte */
	if (oldSize < 5)  {
#ifdef CONFIG_SPLIT_INDICATION
	    CO_NUM_MEMCPY(&pCurSdo->oldVar, pVar,oldSize, objAttr & CO_NUM_VAL);
	    pCurSdo->saved = CO_TRUE;
#else /* CONFIG_SPLIT_INDICATION */
	    CO_NUM_MEMCPY(&oldVar, pVar, oldSize, objAttr & CO_NUM_VAL);
	    saved = CO_TRUE;
#endif /* CONFIG_SPLIT_INDICATION */
	} /* oldSize < 5 */
#ifdef CONFIG_SPLIT_INDICATION
	else {
	    pCurSdo->saved = CO_FALSE;
	}
#endif /* CONFIG_SPLIT_INDICATION */
	
	/* write contents of sdo to object dictionary */
	commonRet = putObj(index, subIndex, pData, oldSize,
		CO_FALSE CO_COMMA_LINE_PARA);
    }

    /* set communication parameter */
    if ((commonRet == CO_OK) && (index <= END_COM_PROF)) {

# ifdef CONFIG_NON_VOLATILE_MEM
	if (index == STORE_PARA_INDEX) {
	    /* check signature */
	    if ((*((UNSIGNED32 *)pVar) == SAVE_SIGNATURE) && (subIndex > 0)) {
		if (saveParameterInd(subIndex CO_COMMA_LINE_PARA) == CO_FALSE) {
		    commonRet = CO_E_HARDWARE_FAULT;
		}
	    }
	    else if ((*((UNSIGNED32 *)pVar) == CLEAR_SIGNATURE) && (subIndex > 0)) {
		if (clearParameterInd(subIndex CO_COMMA_LINE_PARA) == CO_FALSE){
		    commonRet = CO_E_HARDWARE_FAULT;
		}
	    }
	    else {
		commonRet = CO_E_INVALID_TRANSMODE;
	    }
	}
	else if (index == RESTORE_DEF_PARA_INDEX) {
	    if ((*(UNSIGNED32 *)pVar == LOAD_SIGNATURE) && (subIndex > 0)) {
		/* set default values */
		setDefaultParameter(subIndex CO_COMMA_LINE_PARA);
		/* set sign for reset comm */
		coDefParaResetFlag  = 1;
		/* call user indication */
		if (loadParameterInd(subIndex, CO_RESTORE_MODE_SDO
			CO_COMMA_LINE_PARA) == CO_FALSE) {
		    commonRet = CO_E_HARDWARE_FAULT;
		    /* reset sign for reset comm */
		    coDefParaResetFlag  = 0;
		}
	    } else {
		commonRet = CO_E_INVALID_TRANSMODE;
	    }
	}
	else /* neither STORE_PARA_INDEX nor STORE_PARA_INDEX */
	{
# endif /* CONFIG_NON_VOLATILE_MEM */
	    commonRet = setCommPar(index, subIndex CO_COMMA_LINE_PARA);
# ifdef CONFIG_NON_VOLATILE_MEM
	}
# endif /* CONFIG_NON_VOLATILE_MEM */
    }

# ifdef CONFIG_NON_VOLATILE_MEM
    /* remove signature from save/restore objects */
    if ((index == STORE_PARA_INDEX) || (index == RESTORE_DEF_PARA_INDEX))  {
	putObj(index, subIndex, &oldVar[0],oldSize, CO_TRUE CO_COMMA_LINE_PARA);
    }
# endif /* CONFIG_NON_VOLATILE_MEM */

    /* control the behaviour for writing on a object */

# ifdef CONFIG_SPLIT_INDICATION
    if (commonRet == CO_OK) {
	commonRet = sdoWrInd(index, subIndex, pCurSdo->num CO_COMMA_LINE_PARA);
    }
    if (commonRet != CO_OK) {
	if (commonRet != CO_SDO_IND_BUSY) {
	    /* if command failed, the data would be restored */
	    if (pCurSdo->saved == CO_TRUE)  {
		CO_NUM_MEMCPY(pVar, &pCurSdo->oldVar, oldSize,
			objAttr & CO_NUM_VAL);

		/* set communication parameter bacj to old values */
		if (index <= END_COM_PROF) {
		    setCommPar(index, subIndex CO_COMMA_LINE_PARA);
		}
	    }
	    abortSdoTransf_Req(pCurSdo, commonRet);
	    return;
	}
	/* now we wait for finish the indication */
	/* therefore the user has to call the function finishSdoWrInd(*/
	pCurSdo->state |= SDOSTATE_IND_BUSY;
	pCurSdo->state &= ~SDOSTATE_READY;
	return;
    }
# else /* CONFIG_SPLIT_INDICATION */
    if (commonRet == CO_OK) {
	commonRet = sdoWrInd(index, subIndex CO_COMMA_LINE_PARA);
    }

    if (commonRet != CO_OK) {
	/* if command failed, the data would be restored */
	if (saved == CO_TRUE)  {
	    CO_NUM_MEMCPY(pVar, &oldVar, oldSize, objAttr & CO_NUM_VAL);

	    /* set communication parameter bacj to old values */
	    if (index <= END_COM_PROF) {
		setCommPar(index, subIndex CO_COMMA_LINE_PARA);
	    }
	}
	abortSdoTransf_Req(pCurSdo, commonRet);
	return;
    }
# endif /* CONFIG_SPLIT_INDICATION */


# ifdef CONFIG_SPLIT_INDICATION
    writeSdoValue_finished(pCurSdo CO_COMMA_LINE_PARA);
}


/*******************************************************************
*
* writeSdoValue_finished - finish write the value of a SDO to the od dictionary
*
* NOMANUAL
*
* This function finished the writeSdoValue function
* This is only a stand alone function,
* if the CONFIG_SPLIT_INDICATION is set !!
*
*
*/

void writeSdoValue_finished(
    SDO_T	*pCurSdo	/* pointer to SDO */
	    )
{
UNSIGNED16 	index;		/* index */
UNSIGNED8	subIndex;	/* subindex */
#  ifdef CONFIG_DYN_SDO_CONNECTION_MANAGER
RET_T 		commonRet;	/* common return value */
#  endif /* CONFIG_DYN_SDO_CONNECTION_MANAGER */

    /* for faster access */
    index = pCurSdo->odIndex.index;
    subIndex = pCurSdo->odIndex.subIndex;
# endif /* CONFIG_SPLIT_INDICATION */


# ifdef CONFIG_DYN_SDO_CONNECTION_MANAGER
    if (index == SRD_REQUEST_SDO_INDEX)  {
	commonRet = srdRequest(CO_LINE_PARA);
	if (commonRet != CO_OK) {
	    abortSdoTransf_Req(pCurSdo, commonRet);
	    return;
	}
    }

    if (index == SRD_RELEASE_SDO_INDEX)  {
	commonRet = srdRelease(CO_LINE_PARA);
	if (commonRet != CO_OK) {
	    abortSdoTransf_Req(pCurSdo, commonRet);
	    return;
	}
    }
# endif /* CONFIG_DYN_SDO_CONNECTION_MANAGER */

# ifdef CONFIG_SEG_SDO
    if (pCurSdo->state == SDOSTATE_DNLD_SEG) {
	dnLdSeg_res(pCurSdo);
    } else {
	initDnLd_res(pCurSdo);
    }
# else /* CONFIG_SEG_SDO */
    initDnLd_res(pCurSdo);
# endif /* CONFIG_SEG_SDO */

    pCurSdo->state = SDOSTATE_READY;
}


# if defined(CONFIG_SDO_COB_ID)
# else /* defined(CONFIG_SDO_COB_ID) */
/*******************************************************************
*
* setDefSdoCobId - sets the default COB-ID for the first SDO
*
* NOMANUAL
*
* This service sets the default COB-ID for the first SDO.
* This service is only necessary,
* if no COB-ID for the SDO in the Object Dictionary exist
* and a new node_id can be set with \fIresetCommunicaion(2)\fP.
*
* RETURNS
* .TP
* nothing
*
*/
void setDefSdoCobId(
	void
     )
{
SDO_T      *pSdoInUse;    /* pointer to SDO */

    pSdoInUse = CMS_DomExist(1, SERVER CO_COMMA_LINE_PARA);

    SET_COB_ID(pSdoInUse->pReqInd_COB,
	(UNSIGNED16)(CO_COBID_CSDO + (UNSIGNED16)coNodeId ));
    SET_COB_ID(pSdoInUse->pResCon_COB,
	(UNSIGNED16)(CO_COBID_SSDO + (UNSIGNED16)coNodeId ));
}
# endif /* !defined(CONFIG_SDO_COB_ID) */

#endif /* CONFIG_SDO_SERVER */

/*______________________________________________________________________EOF_*/
