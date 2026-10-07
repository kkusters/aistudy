/*
 *++ emerg - functions for Emergency Object (EMCY) handling
 *-- emerg - Funktionen für das Emergency Object (EMCY) 
 *
 * Copyright (c) 1996-2002 port GmbH Halle (Saale)
 *------------------------------------------------------------------
 * $Header$
 *
 *------------------------------------------------------------------
 *
 * modification history
 * --------------------
 * $Log$
 * Revision 1.4  2008-10-24 16:43:33+02  driet
 * <>
 *
 * Revision 1.3  2008-09-17 17:06:14+02  driet
 * <>
 *
 * Revision 1.2  2008-04-02 09:14:27+02  driet
 * <>
 *
 * Revision 1.1  2008-03-12 16:32:12+01  driet
 * <>
 *
 * Revision 1.0  2008-03-07 17:04:39+01  driet
 * Initial revision
 *
 * Revision 2.29  2003/06/13 13:24:34  boe
 * set generic bit alltimes and save it at object dictionary
 *
 * Revision 2.28  2003/03/31 13:14:00  boe
 * emcy consumer usage optimezed
 * (define own functions and structures)
 *
 * Revision 2.27  2002/11/14 10:05:24  boe
 * add comments/adapt on doxygen
 * add co_ to all global library variables
 * get more exact cob-type to driver
 * new function setEmcyCobId() (for EMCY producer)
 *
 * Revision 2.26  2002/05/29 10:22:10  hae
 * documentation correction
 *
 * Revision 2.25  2002/05/21 13:22:00  boe
 * change copyright
 *
 * Revision 2.24  2002/03/26 08:09:12  boe
 * add NEW_TIMER functionality over defines
 *
 * Revision 2.23  2001/12/05 13:57:30  boe
 * save emergency mapping structure at ROM
 *
 * Revision 2.22  2001/08/20 13:55:57  boe
 * extra code for moving the error array field for 16bit cpu
 *
 * Revision 2.21  2001/07/10 07:18:14  boe
 * correct array index for pEv at defineEmcy()
 *
 * Revision 2.20  2001/06/19 15:21:26  boe
 * added test for EMCYFLAG_ENABLED enabled
 *
 * Revision 2.19  2001/05/17 09:18:30  boe
 * explicite typconvertion to remove compiler warnings
 *
 * Revision 2.18  2001/05/10 12:33:37  boe
 * structure for data mapping changed
 *
 * Revision 2.17  2001/04/05 08:45:41  boe
 * comment changed
 *
 * Revision 2.16  2001/03/29 14:24:49  boe
 * comment changed
 *
 * Revision 2.15  2001/03/28 12:51:10  boe
 * additionally include file removed
 *
 * Revision 2.14  2001/03/12 10:22:52  boe
 * setClientEmcyCobId added
 *
 * Revision 2.13  2001/02/26 14:04:36  boe
 * documentation format changed
 *
 * Revision 2.12  2001/01/26 10:50:01  boe
 * split include files into function specific headers
 * use can buffer by function parameter (no acces more to variable CAN_Msg)
 *
 * Revision 2.11  2001/01/17 16:10:03  boe
 * expand all implicite if tests and add type castings
 * use PRODUCER/CONSUMER instead SERVER/CLIENT
 * add inhibittime usage
 *
 * Revision 2.10  2000/11/03 10:16:39  boe
 * emcy number for producer(server) set always to one
 *
 * Revision 2.9  2000/10/17 10:39:53  boe
 * typecasting added
 *
 * Revision 2.7  2000/06/23 09:43:07  oe
 * Reworking for generating the Regerence Manual
 *
 * Revision 2.6  2000/06/21 15:06:15  boe
 * usage of subindex 0 for emergency entries changed
 *
 * Revision 2.5  2000/06/21 08:51:42  boe
 * sunindex 0 for multiple sibindices returns always size 1
 *
 * Revision 2.4  2000/06/13 08:28:45  boe
 * function names (H_) changed
 *
 * Revision 2.3  2000/04/19 07:32:41  boe
 * variable optimization for defineemcy
 *
 * Revision 2.2  2000/03/28 14:25:39  boe
 * adaption for multi-line version
 * set error bit for emergency messages added
 *
 * Revision 2.1  2000/02/04 13:38:12  oe
 * - Changes for generating reference pages
 *
 * Revision 2.0  2000/01/21 11:02:01  boe
 * Überarbeitet und an Version 4.0 angepasst
 *
 *
 *
 *
 *------------------------------------------------------------------
 */


/****************************************************************************/
/**
*  \file emerg.c
*  \author port GmbH Halle (Saale)
*  $Revision$
*  $Date$
*
*
*++ This module contains functions for the handling and transmission
*++ of CANopen Emergency Objects (EMCY).
*++ Emergency messages are triggered by the occurence of a device internal
*++ error situation
*++ and are transmitted from the concerned application device to other devices
*++ with high priority.
*++ This makes them suitable for interrupt type error alerts.
*
*++ If an internal device error occures then the application
*++ transmit an error message with a predefined errorcode.
*
*
*-- Dieses Modul enthält Funktionen zur Manipulation und zum Senden
*-- von CANopen Emergency Objekten (EMCY).
*-- Emergency-Nachrichten werden bei schweren Fehlern in einem Gerät 
*-- ausgelöst und mit hoher Priorität übertragen.
*
*-- Falls ein interner schweren Fehler im Gerät auftritt, dann
*-- wird eine Emergency-Nachricht mit einem vordefinierten Fehlerkode
*-- gesendet.
*
*/


/* header of standard C - libraries */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* header of project specific types */

#include <cal_conf.h>
#include <co_drv.h>
#include <co_odidx.h>
#include <co_mcpy.h>
#include "emerg.h"
#include "nmt.h"
#include "drv.h"
#include "access.h"
#include "cmsevent.h"
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
EMCY_CONS_T *getEmcyConsAddr(UNSIGNED8 );
EMCY_CONS_T *findEmcyConsAddr(UNSIGNED16 cobId );
RET_T       defEmcyConsEvent(EMCY_CONS_T **pEv );

/* external variables
---------------------------------------------------------------------------*/

/* global variables
---------------------------------------------------------------------------*/
#if defined(CONFIG_EMCY_PRODUCER) || defined(CONFIG_EMCY_CONSUMER)
CO_CONST PDO_MAP_T coEmcyTypeDesc[5] = {
        {NULL, (PDO_MAP_T*)&coEmcyTypeDesc[1], CO_UNSIGNED, 16 },/* error code */
        {NULL, (PDO_MAP_T*)&coEmcyTypeDesc[2], CO_UNSIGNED,  8 },/* error register*/
        {NULL, (PDO_MAP_T*)&coEmcyTypeDesc[3], CO_UNSIGNED, 16 },/* manu error 1 */
        {NULL, (PDO_MAP_T*)&coEmcyTypeDesc[4], CO_UNSIGNED, 16 },/* manu error 2 */
        {NULL, NULL,           CO_UNSIGNED,  8}  /* manu error 3 */
    };
#endif /* defined(CONFIG_EMCY_PRODUCER) || defined(CONFIG_EMCY_CONSUMER) */
#ifdef CONFIG_EMCY_PRODUCER
EMCY_T  *co_pFirstEmcyProd ;
#endif /* CONFIG_EMCY_PRODUCER */
#ifdef CONFIG_EMCY_CONSUMER
EMCY_CONS_T *co_pFirstEmcyCons ;
UNSIGNED8   lastEmcyCons ;
#endif /* CONFIG_EMCY_CONSUMER */

/* local defined variables
---------------------------------------------------------------------------*/
#ifdef CONFIG_RCS_IDENT
static char _rcsid[] = "$Id$";
#endif /* CONFIG_RCS_IDENT */

#ifdef CONFIG_EMCY_CONSUMER
static EMERGENCY_T coEmcyConsumer ;
#endif /* CONFIG_EMCY_CONSUMER */


#if defined(CONFIG_EMCY_PRODUCER) || defined(CONFIG_EMCY_CONSUMER)
/****************************************************************************/
/**
*
*++ \brief defineEmcy - defines an Emergency Object (EMCY)
*-- \brief defineEmcy - definiert ein Emergency Object (EMCY)
*
*++ This function defines an Emergency Object.
*++ It must be called by both emergency producers and emergency consumers.
*++ For consumers it is necessary to define the COB-ID
*++ all the emergency objects have a number for reference
*++ The producer uses only one Emergency Object and it uses the default COB-ID
*++ after initialisation.
*-- Diese Funktion definiert ein Emergency Object.
*-- Diese Funktion ist sowohl vom Emergency Producer
*-- als auch Emergency Consumer aufzurufen.
*-- Für die Consumer Emergency Objekte ist die COB-ID zu übergeben.
*-- Producer hingegen nutzen nur ein Emergency Objekt und nutzen die
*-- Standard COB-ID nach der Initialisierung.
*
*++Example:
*--Beispiel:
* 
* \code
* RET retVal;
* 
* // producer
* retVal = defineEmcy(PRODUCER, NULL, 0);
* 
* // consumer
* // first EMCY
* retVal = defineEmcy(CONSUMER, 1, 129);
* // second EMCY
* retVal = defineEmcy(CONSUMER, 2, 130);
* \endcode
* 
* \par REMARK
*++ From former definition:
*-- Von früheren Definitionen:
* \code
* Client - Consumer
* Server - Producer
* \endcode
*
* \retval CO_OK
*++ success
*-- Erfolg
* \retval CO_E_MEM
*++ memory allocation fault
*-- Fehler bei der dynamischen Speicheranforderung
* \retval CO_E_RANGE
*++ COB-ID is outside of the valid limits (1 - 1760)
*-- COB-ID ist außerhalb der gültigen Grenzen (1 - 1760)
* \retval CO_E_NO_ACCESS
*++ no access to object dictionary (COB-ID EMCY, Node ID)
*-- kein Zugriff auf das Objektverzeichnis möglich (COB-ID EMCY, Node ID)
* \retval CO_E_TRANS_TYPE
*++ type of service not available (not compiled)
*-- Dienst nicht möglich (nicht übersetzt)
* 
*/

RET_T defineEmcy(
      CO_USER_T   kindOfUse, /**< kind of using (CONSUMER/PRODUCER) */
      UNSIGNED8   emcyNr,    /**< emergency number for client */
      UNSIGNED16  cobId      /**< COB-ID for Client - not important for server*/
      )
{
RET_T       ret;        /* return value */
#ifdef CONFIG_EMCY_PRODUCER
UNSIGNED32  *pCobIdReq; /* COB-ID for Request/Indication*/
EMCY_T      *pEv;       /* pointer to event */
UNSIGNED32  size;       /* size of object */
#endif /* CONFIG_EMCY_PRODUCER */
#ifdef CONFIG_EMCY_CONSUMER
EMCY_CONS_T *pEmcyCons; /* pointer to emcy consumer */
#endif /* CONFIG_EMCY_CONSUMER */

  if (kindOfUse == PRODUCER)
  {
#ifdef CONFIG_EMCY_PRODUCER
    /* get cob-id from od */
    if (getObjAddr(EMCY_COB_ID_INDEX, 0, (UNSIGNED8 **)&pCobIdReq, &size CO_COMMA_LINE_PARA) != CO_OK)
    {
      return CO_E_NO_ACCESS;
    }
    ret = CMS_DefEvent_req(&co_pFirstEmcyProd, &pEv, CO_COB_EMCY_PROD CO_COMMA_LINE_PARA);
    if (ret != CO_OK)
    {
      return(ret);
    }
#else /* CONFIG_EMCY_PRODUCER */
    return(CO_E_TRANS_TYPE);
#endif /* CONFIG_EMCY_PRODUCER */
  }
  else
  {
#ifdef CONFIG_EMCY_CONSUMER
    /* cobIdReq = (UNSIGNED32)cobId; */
    ret = defEmcyConsEvent(&pEmcyCons CO_COMMA_LINE_PARA);
    if (ret != CO_OK)
    {
      return(ret);
    }
    pEmcyCons->emcyNr = emcyNr;
#else /* CONFIG_EMCY_CONSUMER */
    /* only for compiler warnings, ignore this */
    cobId = cobId;
    emcyNr = emcyNr;
    return(CO_E_TRANS_TYPE);
#endif /* CONFIG_EMCY_CONSUMER */
  }

  /*
     built typedesc list for EMCY
     "STRUCTURE OF UNSIGNED(16),UNSIGNED(8),UNSIGNED(16),"
     "UNSIGNED(16),UNSIGNED(8)"
   */

#ifdef CONFIG_EMCY_PRODUCER
  if (kindOfUse == PRODUCER)
  {
    pEv->pCOB->bLength = 8;
    pEv->pdoNr = 1;

    /* setup the inhibit time if entry available */
    if (getObjEntry(EMCY_INHIBIT_INDEX, 0, (UNSIGNED8 *)&pEv->wInhibitTime, &size, CO_TRUE CO_COMMA_LINE_PARA) != CO_OK)
    {
      /* no entry at ov found, reset the value */
      pEv->wInhibitTime = 0;
    }

    /* set cob id */
    ret = setEmcyCobId(pCobIdReq CO_COMMA_LINE_PARA);
  }
#endif /* CONFIG_EMCY_PRODUCER */

#ifdef CONFIG_EMCY_CONSUMER
  if (kindOfUse == CONSUMER)
  {
    pEmcyCons->pCOB->bLength = 8;
    /* set cob id */
    SET_COB_ID(pEmcyCons->pCOB, cobId);

    /* set emergency valid */
    pEmcyCons->flags = EMCYFLAG_ENABLED;
  }
#endif /* CONFIG_EMCY_CONSUMER */

  return(ret);
}

#endif  /* defined( CONFIG_EMCY_PRODUCER || CONFIG_EMCY_CONSUMER ) */


#ifdef CONFIG_EMCY_PRODUCER

/****************************************************************************/
/**
*
*++ \brief writeEmcyReq - transmit an Emergency Object to the client(s)
*-- \brief writeEmcyReq - sendet ein Emergency Objekt zu dem(n) Client(s)
*
*++ This function stores a serious device error in the local pre-defined
*++ error field (0x1003) and transmits an Emergency Object to the
*++ emergency client(s).
*++ This service is available in the node states PRE_OPERATIONAL and
*++ OPERATIONAL.
*-- Diese Funktion speichert schwere Gerätefehler im lokalen pre-defined
*-- error field (0x1003) und sendet ein Emergency Objekt zu dem(n)
*-- Emergency Client(s).
*-- Dieser Dienst ist in den Zuständen PRE_OPERATIONAL und
*-- OPERATIONAL verfügbar.
*
*++ \em manu1Err is the additional information entry at object 0x1003
*-- \em manu1Err ist die zusätzliche Fehlerinformation im Objekt 0x1003
*
*
* \retval CO_OK
*++ success
*-- Erfolg
* \retval CO_E_NOT_EXIST
*++ Emergency Object or error register (0x1001) doesn't exist
*-- Emergency Objekt oder Error Register (0x1001) existieren nicht
* \retval CO_E_STATE
*++ node isn't in state OPERATIONAL or PRE_OPERATIONAL
*-- Knoten ist nicht im Zustand OPERATIONAL oder PRE_OPERATIONAL
* \retval CO_E_DISABLED
*++ emergency service disabled
*-- Emergency Dienst disabled
* \retval CO_E_INHIBITED
*++ inhibit time not over
*-- Inhibit Zeit noch nicht angelaufen
*
*/

RET_T writeEmcyReq(UNSIGNED16 errCode,  // error code
                   UNSIGNED16 manu1Err, // manufacturer spec. error code (Byte 3-4)
                   UNSIGNED16 manu2Err, // manufacturer spec. error code (Byte 5-6)
                   UNSIGNED8  manu3Err) // manufacturer spec. error code (Byte 7)
{
EMERGENCY_T coStdEmcy;  // emergency object
RET_T       ret;        // return value
UNSIGNED32  size;       // object size
UNSIGNED8   errCnt;     // error counter
UNSIGNED32  err;        // error code
UNSIGNED8   *pData;     // pointer to data
#ifdef CONFIG_BIG_ENDIAN
UNSIGNED8   *pDat2;     // second pointer to data
#endif // CONFIG_BIG_ENDIAN

  // if producer initialised
  if (co_pFirstEmcyProd == NULL)
  {
    // return
    return(CO_E_DISABLED);
  }

  if ((co_pFirstEmcyProd->flags & EMCYFLAG_ENABLED) == 0)
  {
    // return
    return(CO_E_DISABLED);
  }

  // get error register
  if (getObjAddr(0x1001, 0, &pData, &size CO_COMMA_LINE_PARA) != CO_OK)
  {
    return(CO_E_NOT_EXIST);
  }
  if (errCode > 0x00FF)
  { // 0 - ff no error/reset error message
    *pData |= 0x01; // set Bit 0 - generic error
  }

  coStdEmcy.errReg = *pData;

  err = ((UNSIGNED32)manu1Err << 16) + ((UNSIGNED32)errCode & 0xffff);
  coStdEmcy.errCode = errCode;
  coStdEmcy.manu1 = manu1Err;
  coStdEmcy.manu2 = manu2Err;
  coStdEmcy.manu3 = manu3Err;

  co_pFirstEmcyProd->pMapEntries = (PDO_MAP_T *)&coStdEmcy;
  if ((ret = CMS_TransEvent_req(co_pFirstEmcyProd, (PDO_MAP_T *)&coEmcyTypeDesc[0] CO_COMMA_LINE_PARA)) != CO_OK)
  {
    return(ret);
  }

  // internal error handling
  if (errCode > 0x00FF)
  { // 0 - ff no error/reset error message
    // read the counter of errorcodes
    ret = getObjAddr(ERROR_FIELD_INDEX, 0, &pData, &size CO_COMMA_LINE_PARA);
    if (ret != CO_OK)
    {
      // no entries, return
      return(CO_OK);
    }

    // actually error count
    errCnt = *pData;

    // allocate security mechanism for object dictionary consistency
    CO_COM_PART_ALLOC(CO_LINE_PARA);
    // deletes the oldest entry, if counter == array size

    if (errCnt == (getNumOfElem(ERROR_FIELD_INDEX CO_COMMA_LINE_PARA) - 1))
    {
      errCnt--;
    }

    // move error elements to pos+1 - (internal all index are saved as 32 bit val)
# ifdef CONFIG_16BIT_CPU
    CO_NUM_MEMMOVE((void*)(pData+4),(void*)(pData+2),(size_t)(errCnt * 4), CO_NUM_VAL);
# else  // CONFIG_16BIT_CPU
#  ifdef CONFIG_BIG_ENDIAN
    if (getObjAddr(ERROR_FIELD_INDEX, 1, &pDat2, &size CO_COMMA_LINE_PARA) == CO_OK)
    {
      CO_NUM_MEMMOVE((void*)(pDat2+4),(void*)(pDat2),(size_t)(errCnt * 4), CO_NUM_VAL);
    }
#  else // CONFIG_BIG_ENDIAN
    CO_NUM_MEMMOVE((void*)(pData+8),(void*)(pData+4),(size_t)(errCnt * 4), CO_NUM_VAL);
#  endif // CONFIG_BIG_ENDIAN
# endif // CONFIG_16BIT_CPU
    errCnt++; // increments the counter

    *pData = errCnt;

    // release security mechanism for object dictionary consistency
    CO_COM_PART_RELEASE(CO_LINE_PARA);

    // the error code is filled in at the begin of the array
    if (putObj(ERROR_FIELD_INDEX, 1, (UNSIGNED8 *)&err, (UNSIGNED32)4, CO_TRUE CO_COMMA_LINE_PARA) != CO_OK)
    {
      return(CO_E_NOT_EXIST);
    }
  }
  return(CO_OK);
}
#endif // CONFIG_EMCY_PRODUCER


#ifdef CONFIG_EMCY_CONSUMER

/****************************************************************************/
/**
*
*++ \brief readEmcy - read the error code of the Emergency Object
*-- \brief readEmcy - liest den Fehlerkode des Emergency Objekt
*
*++ This function returns the last received error code of an Emergency Object.
*-- Diese Funktion gibt den letzten empfangenen Fehlerkode
*-- des übergebenen Emergency Objektes zurück.
*
* \retval address
*++ address for Emergency Object
*-- Adresse zum Emergency Objekt
* \retval NULL
*++ Emergency Object does'nt exist
*-- Emergency Objekt existiert nicht
*
*/

EMERGENCY_T *readEmcy(
      UNSIGNED8 emcyNr      /**< emergency number */
      )
{
  if (emcyNr != lastEmcyCons)
  {
    return(NULL);
  }
  return(&coEmcyConsumer);
}

#endif /* CONFIG_EMCY_CONSUMER */


#ifdef CONFIG_EMCY_PRODUCER

/****************************************************************************/
/**
*
*++ \brief eraseErr - erase error entries from the Predefined Errorfield
*-- \brief eraseErr - löscht Fehlereinträge aus dem Predefined Errorfield
*
*++ If a device error was repared then this function has to be called by the
*++ application.
*++ It does erase the error(s) from the
*++ CANopen error-array object at index 0x1003 and decrements the error
*++ counter at subindex 0.
*++ If the current error is the last error then it will
*++ send an emergency message with error code \c NO_ERROR (0).
*++ If the parameter \em addErrCode equal zero
*++ the whole list is erased.
*++ For all other cases all entries are erased from the array, 
*++ which have the same additional error code
*++ like the parameter \em addErrCode.
*-- Wenn ein Gerätefehler behoben wurde, ist diese Funktion durch die
*-- Applikation aufzurufen.
*-- Sie löscht Fehler vom CANopen Fehlerfeld auf Index 0x1003 und 
*-- dekrementiert den Fehlerzähler auf Subindex 0.
*-- Ist der letzte Fehlereintrag gelöscht, so sendet diese Funktion
*-- eine Emergency Nachricht mit dem Fehlerkode \c NO_ERROR (0).
*-- Wenn der Parameter \em addErrCode Null ist,
*-- wird die gesamte Fehlerliste gelöscht.
*-- Anderenfalls werden alle Fehlereinträge, die die Zusatzfehlerinformation
*-- \em addErrCode enthalten, gelöscht.
*
* \retval CO_OK
*++ success
*-- Erfolg
* \retval CO_E_NOT_EXIST
*++ Emergency Object or error register doesn't exists
*-- Emergency Objekt oder Error Register (0x1001) existieren nicht
* \retval CO_E_STATE
*++ node isn't in state PRE_OPERATIONAL or OPERATIONAL
*-- Knoten ist nicht im Zustand OPERATIONAL oder PRE_OPERATIONAL
* \retval CO_E_DISABLED
*++ emergency service disabled
*-- Emergency Dienst disabled
* \retval CO_E_INHIBITED
*++ inhibit time not over
*-- Inhibit Zeit noch nicht angelaufen
*
*/
RET_T eraseErr(UNSIGNED16 addErrCode) // additional error code of 0x1003
{
RET_T      ret=CO_OK;  // return value
UNSIGNED8  *ppData[1]; // reference to predefined error field
UNSIGNED32 size;       // dummy entry
UNSIGNED8  i;          // loop counter
UNSIGNED8  max;        // maximum of array entries
UNSIGNED8  *pCount;    // number of current entries
UNSIGNED32 *pErr;      // error field entry

  // read the counter of errorcodes
  if (getObjAddr(ERROR_FIELD_INDEX, 0, &pCount, &size CO_COMMA_LINE_PARA) != CO_OK)
  {
    return(CO_E_NOT_EXIST);
  }

  // allocate security mechanism for object dictionary consistency
  CO_COM_PART_ALLOC(CO_LINE_PARA);

  // default case
  if (addErrCode == 0)
  {
    // get address of first entry
    getObjAddr(ERROR_FIELD_INDEX, 1, (UNSIGNED8 **)&pErr, &size CO_COMMA_LINE_PARA);
    if (*pErr > 0)
    {
      *pErr = 0;   // erase all entries
      *pCount = 0;
    }
    else
    {
      // error was already deleted
      // release security mechanism for object dictionary consistency
      CO_COM_PART_RELEASE(CO_LINE_PARA);
      return(CO_OK);  
    }
  }
  else
  { // erases selective
    max = *pCount;
    for (i = 0; i < max; i++)
    {
      getObjAddr(ERROR_FIELD_INDEX, i+1, ppData, &size CO_COMMA_LINE_PARA);
      pErr = (UNSIGNED32 *)*ppData;

      if (((UNSIGNED16)((*pErr) >> 16)) == addErrCode)
      {
        (*pCount)--;
#ifdef CONFIG_16BIT_CPU
        CO_NUM_MEMMOVE(*ppData,(*ppData)+2,(max-i-1)*4, CO_NUM_VAL);
#else // CONFIG_16BIT_CPU
        CO_NUM_MEMMOVE(*ppData,(*ppData)+4,(max-i-1)*4, CO_NUM_VAL);
#endif // CONFIG_16BIT_CPU
      }
    }
  }

/*****************************************************************************************
 TD - When there are no errors in the array does not mean that there are no errors left!
      It's possible that there are active errors which are not in the array.
      This can be after the array is cleared by writing a zero to it, after a boot up,
      or when there are more errors than the size of the array.
	  User must send the ErrorReset/NoError in application when there are no errors left.

-- code port --
  // no more errors in the array
  if (*pCount == 0)
  {
    ret = writeEmcyReq(0, 0, 0, 0 CO_COMMA_LINE_PARA);
  }
-- end code port --

*****************************************************************************************/

  // release security mechanism for object dictionary consistency
  CO_COM_PART_RELEASE(CO_LINE_PARA);

  return(ret);
}

/****************************************************************************/
/*
*++ \brief setEmcyCobId - set the COB-ID for a EMCY producer
*-- \brief setEmcyCobId - setzt die COB-ID eines EMCY Producer
*
* NOMANUAL
*
*++ This service sets a new local COB-ID for a EMCY producer.
*-- Mit dieser Funktion kann eine neue COB-ID beim EMCY Producer
*-- eingestellt werden.
*
* \retval CO_OK
*++ success
*-- Erfolg
    return(CO_E_NOT_EXIST);
service not initialized
    return(CO_E_TRANS_TYPE);
29 bit identifier are not allowed
*/

RET_T setEmcyCobId(
    UNSIGNED32 *pCobid  /* pointer to new cob-id */
    )
{
    if (co_pFirstEmcyProd  == NULL)  {
    return(CO_E_NOT_EXIST);
    }

    /* abort for extended identifiers */
    if ((*pCobid & CAN_29_BIT_ID_FLAG) != 0)  {
    return(CO_E_TRANS_TYPE);
    }

    if ((*pCobid & EMCY_NOT_VALID_BIT) != 0)  {
    /* disable emcy */
    co_pFirstEmcyProd  ->flags &= ~EMCYFLAG_ENABLED;
    } else {
    /* enable emcy */
    SET_COB_ID(co_pFirstEmcyProd  ->pCOB,
        (UNSIGNED16)(*pCobid & CAN_11_BIT_ID_MASK));
    co_pFirstEmcyProd  ->flags |= EMCYFLAG_ENABLED;
    }

    return(CO_OK);
}
#endif /* CONFIG_EMCY_PRODUCER */

#ifdef CONFIG_EMCY_CONSUMER
/****************************************************************************/
/*
* \internal
*
*++ \brief emcyMsgReceived - Emergency message Received
*-- \brief emcyMsgReceived - Fehlermeldung empfangen
*
* NOMANUAL
*
* \return
* nothing
*
*/
void emcyMsgReceived(
    CAN_MSG_T *canMsg       /* Pointer to CAN Message */
    )
{
EMCY_CONS_T *pEv;       /* pointer to event */

    if ((pEv = findEmcyConsAddr(canMsg->wCOB_ID CO_COMMA_LINE_PARA)) != NULL)  {
    CMS_Decode((PDO_MAP_T *)&coEmcyTypeDesc[0],
        (UNSIGNED8 *)&coEmcyConsumer ,
        &canMsg->pData[0]);
    lastEmcyCons  = pEv->emcyNr;
    emcyInd((UNSIGNED8)pEv->emcyNr CO_COMMA_LINE_PARA);
    }
}


/****************************************************************************/
/**
*++ \brief setClientEmcyCobId - set the COB-ID for a EMCY consumer
*-- \brief setClientEmcyCobId - setzt die COB-ID eines EMCY Consumer Objektes
*
*++ This service sets a new local COB-ID for a EMCY consumer.
*++ It is necessary to call this service
*++ if it is neccessary to change the COB-ID of an EMCY after
*++ the definition.
*
*-- Mit dieser Funktion kann eine neue COB-ID beim EMCY Consumer
*-- eingestellt werden.
*-- Sie ist notwendig, falls der bei der
*-- Definition des EMCY festgelegte COB-ID geändert werden muß.
*
* \retval CO_OK
*++ success
*-- Erfolg
* \retval CO_E_NOT_EXIST
*++ internal EMCY object doesn't exist
*-- internes EMCY Objekt existiert nicht
*/

RET_T  setClientEmcyCobId(
    UNSIGNED8 emcyNr,   /**< Emergency number */
    UNSIGNED16  cobId   /**< new COB_ID */
    )
{
EMCY_CONS_T *pEmcyInUse;    /* pointer to emcy structure */

    pEmcyInUse = getEmcyConsAddr(emcyNr CO_COMMA_LINE_PARA);
    if (pEmcyInUse == NULL) {
    return CO_E_NOT_EXIST;
    }
    SET_COB_ID(pEmcyInUse->pCOB, cobId);
    return CO_OK;
}


/*******************************************************************
*
* getEmcyConsAddr - get emergency consumer address
*
* \internal
*
* The function returns the address of an emergency consumer
* with the given number
*
* \retval 
* address if event with evNr exists
* else NULL
*
*/
EMCY_CONS_T *getEmcyConsAddr(
    UNSIGNED8   evNr    /* event number */
    )
{
EMCY_CONS_T *pEvInUse;      /* pointer to event */

    pEvInUse = co_pFirstEmcyCons ;
    while (pEvInUse != NULL) {
    if (pEvInUse->emcyNr == evNr) {
        return(pEvInUse);
    }
    pEvInUse = pEvInUse->pNext;
    }

    return(NULL);
}


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
EMCY_CONS_T *findEmcyConsAddr(
    UNSIGNED16  cobId       /* COB-Id */
    )
{
EMCY_CONS_T *pEvInUse;      /* pointer to event */

    pEvInUse = co_pFirstEmcyCons ;
    while (pEvInUse != NULL) {
    if (pEvInUse->pCOB->wID == cobId) {
        return(pEvInUse);
    }
    pEvInUse = pEvInUse->pNext;
    }

    return(NULL);
}


/*******************************************************************
*
* defEmcyConsEvent - define an emergency consumer event
*
* \internal
*
* The function create an emergency consumer event
* at the consuer list
* and save all necessary values on it.
* It requests memory for the new event
* and create a new cob-structure.
*
* \retval RET_T
*/

RET_T defEmcyConsEvent(
    EMCY_CONS_T **pEv       /* Pointer to actual event */
     )
{
EMCY_CONS_T *pEvInUse,      /* pointer to actual event */
        *pLastEvEntry = NULL;   /* pointer to last event */

    /* search end of event list */
    pEvInUse = co_pFirstEmcyCons ;
    if (pEvInUse != NULL)  {
    do {
        pLastEvEntry = pEvInUse;
    } while ((pEvInUse = pLastEvEntry->pNext) != NULL);
    }

    if ((pEvInUse = (EMCY_CONS_T *)CalMalloc(sizeof(EMCY_CONS_T))) == NULL) {
    return(CO_E_MEM);
    }


    if ((pEvInUse->pCOB = DEFINE_COB(CO_COB_EMCY_CONS, 0 CO_COMMA_LINE_PARA))
        == NULL)  {
        return(CO_E_NO_DATABASE);
    }

    if (co_pFirstEmcyCons  == NULL)  {
    co_pFirstEmcyCons  = pEvInUse;
    pEvInUse->pNext = NULL;
    }
    else {
    pLastEvEntry->pNext = pEvInUse;
    pEvInUse->pNext = NULL;
    }

    /* return the new event address */
    *pEv = pEvInUse;

    return(CO_OK);
}


/******************************************************************
*
*-- leaveEmcyConsEvent - leave emergency consumer event
*++ leaveEmcyConsEvents - leave emergency consumer event
*
* NOMANUAL
*
*-- Mit dieser Funktion sollten
*-- nach Beendigung des Anwenderprogramms
*-- alle Initialisierungen für Emergency Consumer Events wieder
*-- zurückgenommen werden.
*++ The application should use this function to
*++ reset all library inizializations,
*++ possibly after ending the application programm for events
*/

void leaveEmcyCons(
    )
{
EMCY_CONS_T *pEvEntry, *pEv;    /* pointer to event structures */

    pEvEntry = co_pFirstEmcyCons ;

    while (pEvEntry != NULL) {
    pEv = pEvEntry->pNext;
    CalFree(pEvEntry);
    pEvEntry = pEv;
    }
}
#endif /* defined(CONFIG_EMCY_CONSUMER) */

/*______________________________________________________________________EOF_*/
