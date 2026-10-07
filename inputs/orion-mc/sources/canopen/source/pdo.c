/*
 *++ pdo - contains PDO service routines
 *-- pdo - beinhaltet Funktionen für PDO Dienste
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
 * Revision 1.4  2008-04-08 12:15:15+02  driet
 * changed function getMapObjAddr
 * now it works for RPDO's and TPDO's
 *
 * Revision 1.3  2008-03-26 17:06:04+01  driet
 * <>
 *
 * Revision 1.2  2008-03-20 15:57:09+01  driet
 * <>
 *
 * Revision 1.1  2008-03-19 17:05:33+01  driet
 * Controle op RTR bij schrijven COB-ID geblokkeerd
 *
 * Revision 1.0  2008-03-07 17:04:45+01  driet
 * Initial revision
 *
 * Revision 2.30  2003/07/22 07:21:27  boe
 * change comments
 *
 * Revision 2.29  2003/06/16 13:20:02  boe
 * add line parameter for function setPdoTransType
 *
 * Revision 2.28  2003/06/13 14:01:53  boe
 * new function updatePdoReq()
 * disable event timer for switching to sync mode
 *
 * Revision 2.27  2003/03/31 12:38:18  boe
 * add multiline parameter for some function calls (pdo event timer functions)
 *
 * Revision 2.26  2003/02/13 09:46:17  boe
 * don't use shadow buffer outside of sync usage
 *
 * Revision 2.25  2002/12/11 07:58:39  boe
 * change function parameter for setPdoEventTime()
 *
 * Revision 2.24  2002/11/18 09:34:25  boe
 * add comments/adapt on doxygen
 * add co_ to all global library variables
 * get more exact cob-type to driver
 * don't set cob-id to default value at definePdo()
 * use new functions for setTransType and SetMapping
 *
 * Revision 2.23  2002/05/29 15:20:16  hae
 * documentation correction
 *
 * Revision 2.22  2002/05/21 14:19:11  boe
 * cleanup new timer usage
 *
 * Revision 2.21  2002/03/26 08:15:02  boe
 * add NEW_TIMER functionality over defines
 *
 * Revision 2.20  2001/05/17 09:38:37  boe
 * explicite type conversion to remove compiler warnings
 *
 * Revision 2.19  2001/05/10 12:36:23  boe
 * structure for data mapping changed
 *
 * Revision 2.18  2001/04/05 12:26:55  boe
 * command line parameter for single line changed to void
 *
 * Revision 2.17  2001/04/05 08:45:49  boe
 * comment changed
 *
 * Revision 2.16  2001/03/29 14:31:41  boe
 * comment changed
 *
 * Revision 2.15  2001/03/28 12:56:22  boe
 * indication call to updateSyncCmdMsg() removed
 *
 * Revision 2.14  2001/03/19 15:58:03  ro
 * some header only include if used - corrected
 *
 * Revision 2.13  2001/03/14 16:50:05  ro
 * pdoMsgReceived: MPDO - remove MSB of first byte (addressing mode)
 *
 * Revision 2.12  2001/02/26 14:55:11  boe
 * driver access functions replaced by macros
 * documentation format changed
 *
 * Revision 2.11  2001/01/26 11:06:06  boe
 * split include files into function specific headers
 * use can buffer by function parameter (no acces more to variable CAN_Msg)
 * move function getMapObjAddr() to this file
 *
 * Revision 2.10  2001/01/17 16:17:08  boe
 * expand all implicite if tests and add type castings
 * set the COB-Id depending on the define CONFIG_ONLY_ONE_TRANSMIT_CHANNEL
 *
 * Revision 2.9  2000/10/05 12:01:48  boe
 * PDO_SERVER and PDO_CLIENT renamed as PDO_CONSUMER/PRODUCER
 * MPDO modes added, CAN buffer for Multiline used as array
 * COM_PART_ALLOC/RELEASE modified
 *
 * Revision 2.8  2000/07/28 06:40:52  boe
 * pdo timer event counter moved from struct node_t to global variables
 * call the pdoTimerInd() for failed readPdo requests
 *
 * Revision 2.7  2000/06/21 08:54:07  boe
 * update sync messages inserted, comments expanded
 *
 * Revision 2.6  2000/06/13 08:37:21  boe
 * function names (H_) changed
 *
 * Revision 2.5  2000/04/19 07:36:02  boe
 * parameter optimization
 *
 * Revision 2.4  2000/04/18 08:44:59  boe
 * optimation for variablen access
 *
 * Revision 2.3  2000/03/29 15:35:51  boe
 * extra parameter for multilien version deleted
 *
 * Revision 2.2  2000/03/28 14:34:13  boe
 * adaption for multi-line version
 * old comments removed
 *
 * Revision 2.1  2000/02/04 13:31:11  oe
 * - Changes for generating reference pages
 *
 * Revision 2.0  2000/01/21 11:03:50  boe
 * Überarbeitet und an Version 4.0 angepasst
 *
 *
 *
 *
 *------------------------------------------------------------------
 */


/****************************************************************************/
/**
*  \file pdo.c
*  \author port GmbH Halle (Saale)
*  $Revision$
*  $Date$
*
*++ This module contains the functions for handling the
*-- Diese Modul beinhaltet Funktionen für
* Process Data Objects (PDO).
*
*++ The CANopen library supports asynchronous, synchrounous,
*++ cyclic and acyclic PDOs.
*++ All Transmit-PDO's can be requested via the CAN-RTR request.
*-- Die CANopen Library unterstützt asynchrone, synchrone
*-- zyklische und azyklische PDOs.
*-- Alle Transmit-PDO's können über ein RTR angefordert werden.
* \par
*++ In the states OPERATIONAL and PRE_OPERATIONAL, the node's
*++ PDO parameter set can be changed via SDO transfer or
*++ the node's local application program.
*++ But changing of transmission type is only possible in the state
*++ PRE_OPERATIONAL.
*-- In den Zuständen OPERATIONAL und PRE_OPERATIONAL
*-- lassen sich die PDO-Parameter über SDO
*-- oder durch das lokale Applikationsprogramm ändern.
*-- Eine Ausnahme ist der Transmission Type,
*-- der nur im Zustand PRE_OPERATIONAL geändert werden darf.
* \par
*++ The CANopen Library by \em port support a bitwise mapping.
*++ That means up to 64 variables (1 bit) can be mapped into one PDO.
*++ Futher a so called dummy entry mapping
*++ (with index entries 1-7) is possible.
*++ For that kind of mapping the corresponding data in the PDO is
*++ not evaluated by the device.
*++ This feature is useful for transmitting data to several devices
*++ by using one PDO.
*++ Each device is only utilizing a certain part of the PDO.
*-- Die CANopen Library von \em port unterstützt wahlweise bitweises Mapping.
*-- Das bedeuted, daß bis zu 64 Variablen (1 bit) auf eine PDO gemappt
*-- werden können.
*-- Weiterhin ist das sogenannte dummy entry mapping
*-- (mit den Indexnumern 1-7) möglich.
*-- Mit diesem Mapping werden die korrespondierenden Daten vom
*-- Gerät nicht übernommen.
*-- Diese Methode ist nützlich für das Versenden von Daten an verschiedene
*-- Geräte mit einem PDO.
*-- Jedes Gerät übernimmt nur den Teil der PDO Daten, die für ihn bestimmt sind.
* \par
*++ Dynamically PDO mapping is possible.
*-- Dynamisches PDO Mapping wird unterstützt.
* \par
*++ This modul is very scalable through compiler defines.
*++ Please see the appendix in the CANopen User Manual.
*-- Durch Compiler Direktiven ist dieses Modul in großem Umfang
*-- in der Kodegröße skalierbar
*-- (siehe im Anhang des CANopen Library UserManual).
*/

/* header of standard C - libraries */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* header of project specific types */

#include <cal_conf.h>
#include <co_odidx.h>
#include <co_emcy.h>
#include <co_mcpy.h>
#include "pdo.h"
#include "access.h"
#include "cmsevent.h"
#include "cmscodec.h"
#include "nmt.h"
#include "drv.h"

#if defined(CONFIG_MPDO_DEST) || defined(CONFIG_MPDO_SRC)
# include "mpdo.h"
#endif /* defined(CONFIG_MPDO_DEST) || defined(CONFIG_MPDO_SRC) */

#if defined(CONFIG_SYNC_PRODUCER) || defined(CONFIG_SYNC_CONSUMER)
# include "sync.h"
#endif /* defined(SYNC_PRODUCER) || defined SYNC_CONSUMER) */

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
#ifdef CONFIG_PDO_PRODUCER
PDO_T     *co_pFirstTrPdo ;
#endif /* CONFIG_PDO_PRODUCER */
#ifdef CONFIG_PDO_CONSUMER
PDO_T     *co_pFirstRecPdo ;
#endif /* CONFIG_PDO_CONSUMER */

/* local defined variables
---------------------------------------------------------------------------*/
#ifdef CONFIG_RCS_IDENT
static char _rcsid[] = "$Id$";
#endif /* CONFIG_RCS_IDENT */


#if defined(CONFIG_PDO_CONSUMER) || defined(CONFIG_PDO_PRODUCER)
/****************************************************************************/
/**
*++ \brief definePdo - define PDO properties
*-- \brief definePdo - definiert PDO Eigenschaften
*
*++ This function defines a Process Data Object (PDO) with its usable
*++ properties.
*-- Diese Funktion definiert ein Process Data Object (PDO) mit bestimmten
*-- Eigenschaften.
 \par
*++ A shadow variable list for temporary storage of data will be created for
*++ synchronous RPDO (besides RTR).
*-- Es wird eine Schattenvariablenliste für die temporäre Speicherung der
*-- Daten von synchronen RPDO (außer RTR) angelegt.
*++ The data of
*++ the shadow memory will be copied to the original storage place if a
*++ valid SYNC occurs.
*-- Die Daten des Schattenspeichers werden an die richtigen Speicherstellen
*-- kopiert beim nächsten gültigen SYNC.
 \par
*++ All RTR PDO have the behaviour of common asynchronous
*++ PDOs besides the updating of data.
*-- Alle RTR PDO haben das Verhalten normaler asynchroner PDOs, außer
*-- beim Aktualisieren der Daten.
 \par
*++ If variable PDO mapping should be
*++ allowed the user has to set the \b dynMap flag.
*++ Additionally the
*++ defines CONFIG_DYN_PDO_MAPPING and CONFIG_MAX_DYN_MAP_ENTRIES have to be set.
*-- Wenn dynamisches PDO Mapping unterstützt werden soll, so ist der Parameter
*-- \b dynMap zu setzen.
*-- Zusätzlich sind die Compilerdefines CONFIG_DYN_PDO_MAPPING und
*-- CONFIG_MAX_DYN_MAP_ENTRIES zu verwenden.
 \par
*++ All necessary parameters for creating a PDO besides the COB-ID
*++ are read from the
*++ corresponding object dictionary entries (0x1400 - 0x1BFF).
*++ For the first four RPDO/TPDO pairs the resulting COB-IDs will be
*++ computed from the Node ID according to DS301.
*-- Alle notwendigen Parameter für das Erzeugen einer PDO, außer der COB-ID
*-- werden aus den korrespondierenden Objektverzeichniseinträgen
*-- (0x1400 - 0x1BFF) gelesen.
*-- Für die ersten vier TPDOs und RPDOs werden die Standard COB-IDs
*-- nach CiA DS 301 berechnet.
*
* \code
* 1st  RPDO      0x200 + node ID
* 2nd  RPDO      0x300 + node ID
* 3rd  RPDO      0x400 + node ID
* 4th  RPDO      0x500 + node ID
* 1st  TPDO      0x180 + node ID
* 2nd  TPDO      0x280 + node ID
* 3rd  TPDO      0x380 + node ID
* 4th  TPDO      0x480 + node ID
* \endcode
*++ All other PDO COB-IDs are set to
*-- Die COB-IDs aller weiteren PDOs werden zu
( 0x80000000 | 1760) = 0x80006E0 )
*-- gesetzt und sind damit nicht aktiv.
*++ After 'defining' they are disabled.
*++ To change their COB-ID the object dictionary must be modified and
*-- Um die COB-IDs zu ändern ist die Funktion
* \em setCommPar(2)
*++ has to be called in order to set the internal values.
*-- aufzurufen, um die internen Bibliotheksvariablen zu setzen.
*
* \code
* definePDO(RECEIVE_PDO, 6, ...);      // define PDO
* cobId = 600;                         // new COB-ID
* putObj(0x1405, 1, &cobId, 4, CO_TRUE);  // set new Value to OD
* setCommPar(0x1405, 1);               // set internal values
* nr = 0;                              // mapping count
* putObj(0x1605, 0, &nr, 1, CO_TRUE);  // disable mapping
* setCommPar(0x1605, 0);               // set internal values
* mapping = 0x20000120;                // new Mapping 
*                                      // (Index 2000, Subindex 0, Length 32 bit) 
* putObj(0x1605, 1, &mapping, 4, CO_TRUE);  // set new Mapping to OD
* nr = 1;                              // new number of mappings
* putObj(0x1605, 0, &nr, 1, CO_TRUE);  // set new Value to OD
* setCommPar(0x1605, 0);               // set internal values
* \endcode
*
*++ Please note:
*++ if your CAN controller doesn't support RTR-Request
*++ or the define ONLY_ONE_TRANSMIT_CHANNEL has been set,
*++ the bit PDO_NO_RTR_ALLOWED_BIT has to be set for each COB-Id.
*++ Otherwise the function returns error!
*-- Bitte beachten:
*-- Wenn der CAN-Controller kein RTR unterstützt 
*-- oder das define ONLY_ONE_TRANSMIT_CHANNEL gesetzt ist,
*-- muss das Bit PDO_NO_RTR_ALLOWED_BIT bei jeder COB-Id gesetzt werden,
*-- ansonsten kehrt die Funktion mit einer Fehlermeldung zurück.
*
* \retval CO_OK
*++ success
*-- Erfolg
* \retval CO_E_MEM
*++ memory allocation fault
*-- Speicherzuweisungsfehler
* \retval CO_E_NO_ACCESS
*++ no access to object dictionary (PDO parameter, Node Id)
*-- Kein Zugriff auf Objektverzeichnis (PDO parameter, Node Id) möglich
* \retval CO_E_RANGE
*++ COB_ID is outside of the valid limit (1 - 1760)
*-- COB_ID ist außer des gültigen Bereiches (1 - 1760)
* \retval CO_E_MAP
*++ mapping error, e.g. mapping not possible
*-- Mappingfehler, z.B. Mapping nicht möglich
* \retval CO_E_TRANS_TYPE
*++ bad transmission type
*-- eingestellter Transmission Typ nicht möglich
*/

RET_T definePdo(
      UNSIGNED8  kind,    // kind of PDO RECEIVE_PDO/TRANSMIT_PDO
      UNSIGNED16 pdoNr,   // number of PDO
      BOOL_T     dynMap   // permission flag for dynamically PDO mapping
      )
{
UNSIGNED8  actMapCnt;             // max. count of mapping objects (subindex 0)
UNSIGNED8  maxMapValue;           // max. possible count of mapping objects
UNSIGNED32 size;                  // size of object
UNSIGNED16 pdoOffs;               // pdo index address offset
UNSIGNED16 pdoMapBase;            // base index of PDO mapping
UNSIGNED16 pdoParaBase;           // base index of PDO parameter
UNSIGNED16 inhibitTime = 0;       // inhibittime
RET_T      retVal;                // return value
PDO_MAP_T  *pMapEntry;            // pointer to current mapping variable
PDO_MAP_T  *pPrevMapEntry = NULL; // pointer to current mapping variable
UNSIGNED8  count;                 // count of subindicies of PDO parameter
UNSIGNED8  i;                     // help var
UNSIGNED32 cobId;                 // COB_ID of PDO
PDO_T     *pPdo;                  // pointer to actual pdo structure
PDO_T     **ppFirstPdo;           // pointer to address of first pdo
COB_KIND_T cobKind;               // cob-id kind (transmit/receive)


  if (kind == TRANSMIT_PDO)
  {
#ifdef CONFIG_PDO_PRODUCER
    pdoMapBase = TPDO_MAP_BASE_INDEX;
    pdoParaBase = TPDO_PARA_BASE_INDEX;
    // cobId = CO_COBID_TPDO1;
    cobKind = CO_COB_PDO_PROD_RTR;
    ppFirstPdo = &co_pFirstTrPdo;
#else // CONFIG_PDO_PRODUCER
    return(CO_E_TRANS_TYPE);
#endif // CONFIG_PDO_PRODUCER
  }
  else
  {
#ifdef CONFIG_PDO_CONSUMER
    pdoMapBase = RPDO_MAP_BASE_INDEX;
    pdoParaBase = RPDO_PARA_BASE_INDEX;
    // cobId = CO_COBID_RPDO1;
    cobKind = CO_COB_PDO_CONS_RTR;
    ppFirstPdo = &co_pFirstRecPdo ;
#else // CONFIG_PDO_CONSUMER
    return(CO_E_TRANS_TYPE);
#endif // CONFIG_PDO_CONSUMER
  }

  // for faster access
  pdoOffs = pdoNr - 1;

  // if the pdo exist, set startaddress
  pPdo = pdoExist(pdoNr, kind CO_COMMA_LINE_PARA);

  // read COB-ID parameter
  if (getObjEntry((UNSIGNED16)(pdoParaBase + pdoOffs), 1, (UNSIGNED8 *)&cobId, &size, CO_TRUE CO_COMMA_LINE_PARA) != CO_OK)
  {
    return CO_E_NO_ACCESS;
  }

#ifdef CONFIG_ONLY_ONE_TRANSMIT_CHANNEL
  cobId |= PDO_NO_RTR_ALLOWED_BIT;

  // write back COB-ID parameter
  if (putObj((UNSIGNED16)(pdoParaBase + pdoOffs), 1,(UNSIGNED8 *)&cobId, 4UL, CO_TRUE CO_COMMA_LINE_PARA) != CO_OK)
  {
    return CO_E_NO_ACCESS;
  }
#endif // CONFIG_ONLY_ONE_TRANSMIT_CHANNEL

  // define Event and allocate memory if it not exist
  if (pPdo == NULL)
  {
    if ((cobId & PDO_NO_RTR_ALLOWED_BIT) != 0)
    {
      if (kind == TRANSMIT_PDO)
      {
        cobKind = CO_COB_PDO_PROD;
      }
      else
      {
        cobKind = CO_COB_PDO_CONS;
      }
    }

    if ((retVal = CMS_DefEvent_req(ppFirstPdo, &pPdo, cobKind CO_COMMA_LINE_PARA)) != CO_OK)
    {
      return(retVal);
    }
    pPdo->pdoNr = pdoNr;
    pPdo->pMapEntries = NULL;
  }

  // reset pdo flags
  pPdo->flags = 0;

#if defined(CONFIG_SYNC_CONSUMER) || defined(CONFIG_SYNC_PRODUCER)
  pPdo->flags |= PDOFLAG_SYNC_POSSIBLE;
#endif // defined(CONFIG_SYNC_CONSUMER) || defined(CONFIG_SYNC_PRODUCER)

#if defined(CONFIG_MPDO_DEST) || defined(CONFIG_MPDO_SRC)
  pPdo->mpdoFlags = 0;
#endif // defined(CONFIG_MPDO_DEST) || defined(CONFIG_MPDO_SRC)

  // set cob-id
  if ((retVal = setPdoCobId(pPdo, cobId)) != CO_OK)
  {
    return(retVal);
  }

  // get transmission type
  if (getObjEntry((UNSIGNED16)(pdoParaBase + pdoOffs), 2, &pPdo->transType, &size, CO_TRUE CO_COMMA_LINE_PARA) != CO_OK)
  {
    return CO_E_NO_ACCESS;
  }

  // set transmission type
  retVal = setPdoTransType(pPdo, kind, pPdo->transType CO_COMMA_LINE_PARA);
  if (retVal != CO_OK)
  {
    return(retVal);
  }

  // get number of sub indicies of PDO parameter
  if (getObjEntry((UNSIGNED16)(pdoParaBase + pdoOffs), 0, &count, &size, CO_TRUE CO_COMMA_LINE_PARA) != CO_OK)
  {
    return CO_E_NO_ACCESS;
  }

  // get optional entry inhibit time
  if (count >= 3)
  {
    if (getObjEntry((UNSIGNED16)(pdoParaBase + pdoOffs), 3, (UNSIGNED8 *)&inhibitTime, &size, CO_TRUE CO_COMMA_LINE_PARA) != CO_OK)
    {
      return CO_E_NO_ACCESS;
    }
  }
  if ((retVal = setPdoInhibitTime(pPdo, inhibitTime)) != CO_OK)
  {
    return(retVal);
  }
  pPdo->inhibit.ticks = 0;

#ifdef CONFIG_PDO_EVENTTIMER
  // get optional entry event timer
  if (count > 4)
//  if ((count > 4) && ((pPdo->flags & PDOFLAG_SYNC) == 0)) // TD - 4-3-08
  {
    // entry for event timer exist
    UNSIGNED16  tmpU16;

    if (getObjEntry((UNSIGNED16)pdoParaBase + pdoOffs, 5, (UNSIGNED8 *)&tmpU16, &size, CO_TRUE CO_COMMA_LINE_PARA) != CO_OK)
    {
      return CO_E_NO_ACCESS;
    }

    retVal = setPdoEventTime(pPdo, kind, tmpU16 CO_COMMA_LINE_PARA);
    if (retVal != CO_OK)
    {
      return(retVal);
    }
  }
#endif // CONFIG_PDO_EVENTTIMER

  // get number of mapping objects
  if (getObjEntry((UNSIGNED16)(pdoMapBase + pdoOffs), 0, &actMapCnt, &size, CO_TRUE CO_COMMA_LINE_PARA) != CO_OK)
  {
    return CO_E_MAP;
  }

#if defined(CONFIG_MPDO_DEST)
  // MPDO destination mode is signed as 255
  if (actMapCnt == 255)
  {
    // MPDO dest. mode
    if (kind == TRANSMIT_PDO)
    {
      pPdo->mpdoFlags = MPDOFLAG_DEST_PRODUCER;
      actMapCnt = 1;
    }
    else
    {
      pPdo->mpdoFlags = MPDOFLAG_DEST_CONSUMER;
      actMapCnt = 0;
    }
  }
#endif // CONFIG_MPDO_DEST_PRODUCER

#if defined(CONFIG_MPDO_SRC)
  // MPDO source mode is signed as 254
  if (actMapCnt == 254)
  {
    // test the transmission type - only 254 or 255 ar allowed
    if ((pPdo->transType != 254) && (pPdo->transType != 255))
    {
      return CO_E_TRANS_TYPE;
    }

    // MPDO src mode
# if defined(CONFIG_PDO_PRODUCER)
    if (kind == TRANSMIT_PDO)
    {
      pPdo->mpdoFlags = MPDOFLAG_SRC_PRODUCER;
      // test the mapping entries at the object scanner list
      if ((retVal = testMPdoScannerList(CO_LINE_PARA)) != CO_OK)
      {
        return(retVal);
      }
    }
    else
    {
# endif // defined(CONFIG_PDO_PRODUCER)
# if defined(CONFIG_PDO_CONSUMER)
      pPdo->mpdoFlags = MPDOFLAG_SRC_CONSUMER;
      // test the mapping entries at the dispatcher list
      if ((retVal = testMPdoDispatcherList(CO_LINE_PARA)) != CO_OK)
      {
        return(retVal);
      }
# endif // defined(CONFIG_PDO_CONSUMER)
# if defined(CONFIG_PDO_PRODUCER)
    }
# endif // defined(CONFIG_PDO_PRODUCER)

    actMapCnt = 0;
  }
#endif // CONFIG_MPDO_SRC

  maxMapValue = actMapCnt;

  // if first call, alloc memory for mapping entries
  if (pPdo->pMapEntries == NULL)
  {
    if (dynMap == CO_TRUE)
    {
#ifdef CONFIG_DYN_PDO_MAPPING
      if (actMapCnt > CONFIG_MAX_DYN_MAP_ENTRIES)
      {
        return CO_E_MAP;
      }
      else
      {
        // get max. number of mapping objects
        // numOfElemnts are mapping + subindex 0
        maxMapValue = getNumOfElem(pdoMapBase + pdoOffs CO_COMMA_LINE_PARA);
        if (maxMapValue > CONFIG_MAX_DYN_MAP_ENTRIES)
        {
          maxMapValue = CONFIG_MAX_DYN_MAP_ENTRIES;
        }
      }
    }
    else
    { // no dynamic mapping
      // set objectattribute from subindex 0 to readonly
      i = getObjAttr(pdoMapBase + pdoOffs, 0 CO_COMMA_LINE_PARA);
      i &= ~CO_WRITE_PERM;
      if (setObjAttr(pdoMapBase + pdoOffs, 0, i CO_COMMA_LINE_PARA) != CO_TRUE)
      {
        return(CO_E_NO_ACCESS);
      }
#else // CONFIG_DYN_PDO_MAPPING
      return(CO_E_MAP); // ! CONFIG_DYN_PDO_MAPPING
#endif // CONFIG_DYN_PDO_MAPPING
    }

    // for all mapping entries
    for (i = 0; i < maxMapValue; i++)
    {
      // get memory for mapping data information
      pMapEntry = (PDO_MAP_T *)CalMalloc(sizeof(PDO_MAP_T));
      // if memory not available
      if (pMapEntry == NULL)
      {
        return(CO_E_MEM);
      }

      // set end of list for typedesc
      pMapEntry->pNext = NULL;

      // set the previews pointer
      if (pPrevMapEntry == NULL)
      {
        // set pointer to first object
        pPdo->pMapEntries = pMapEntry;
      }
      else
      {
        // set pointer to the prev. structure
        pPrevMapEntry->pNext = pMapEntry;
      }
      pPrevMapEntry = pMapEntry;
    } // end for loop
  }

  // check Mapping
  retVal = checkMappingTable(pdoMapBase + pdoOffs CO_COMMA_LINE_PARA);

  return(retVal);
}
#endif // defined(CONFIG_PDO_CONSUMER) || defined(CONFIG_PDO_PRODUCER)


#if defined(CONFIG_PDO_CONSUMER) || defined(CONFIG_PDO_PRODUCER)
/*******************************************************************
*
* pdoExist - searchs for PDO in event list
*
* NOMANUAL
*
* This function tests whether a PDO exists or not.
* This is doing by comparing the PDO number and the direction flag.
*
* \retval address
* pointer to event structure - success
* \retval NULL
* error
*
*/

PDO_T *pdoExist(
      UNSIGNED16 num,     /* number of the PDO */
      UNSIGNED8  pdoType  /* RECEIVE/TRANSMIT PDO */
      )
{
PDO_T   *pPdoInUse;     /* pointer to actual pdo */

    /* set the event list */
    if (pdoType == TRANSMIT_PDO)  {
# ifdef CONFIG_PDO_PRODUCER
    pPdoInUse = co_pFirstTrPdo ;
# else /* CONFIG_PDO_PRODUCER */
    return(NULL);
# endif /* CONFIG_PDO_PRODUCER */
    } else  {
# ifdef CONFIG_PDO_CONSUMER
    pPdoInUse = co_pFirstRecPdo ;
# else /* CONFIG_PDO_CONSUMER */
    return(NULL);
# endif /* CONFIG_PDO_CONSUMER */
    }

    while (pPdoInUse != NULL) {
    if (pPdoInUse->pdoNr == num) {
        return(pPdoInUse);
    }
    pPdoInUse = pPdoInUse->pNext;
    }
    return(NULL);
}
#endif /* defined(CONFIG_PDO_CONSUMER) || defined(CONFIG_PDO_PRODUCER) */


#ifdef CONFIG_PDO_PRODUCER
/*******************************************************************
*
*++ prepareTransPdo - prepared a PDO for transmitting to the client(s)
*-- prepareTransPdo - bereitet ein PDO zur Sendung a die Client(s) vor
*
* \internal
*
*++ The function writes a PDO to the given transmit buffer.
*++ With synchronous PDOs the PDOs sync-buffer values will be updated
*++ and they are transmitted with the following SYNC object.
*-- Diese Funktion schreibt ein PDO in den übergebenen Sendepuffer.
*-- Bei synchronen PDOs werden nur die Daten des Schattenpuffers aktualisiert,
*-- die mit dem nächsten gültigen SYNC gesendet werden.
*
*++ This service is only available in the node state OPERATIONAL.
*-- Dieser Dienst ist nur im Zustand OPERATIONAL verfügbar.
*
* \retval CO_OK
*++ success
*-- Erfolg
* \retval CO_E_STATE
*++ node isn't in state OPERATIONAL
*-- Knoten ist nicht im Zustand OPERATIONAL
* \retval CO_E_DISABLED
*++ PDO is disabled
*-- PDO ist disabled
* \retval CO_E_MAP
*++ mapping incorrect
*-- Mapping Fehler
*
*/

RET_T prepareTransPdo(PDO_T     *pPdo, // Pointer to PDO Data
                      UNSIGNED8 *pBuf) // Send Buffer
{
  // ensures that node exist
  assert(co_pNode  != NULL);

  if (co_pNode->eState != OPERATIONAL)
  {
    return(CO_E_STATE);
  }

  // if asynchronous PDO disabled do nothing
  if ((pPdo->flags & PDOFLAG_DISABLED) != 0)
  {
    return(CO_E_DISABLED);
  }

# if (defined(CONFIG_SYNC_CONSUMER) || defined(CONFIG_SYNC_PRODUCER)) && defined(CONFIG_PDO_PRODUCER)
  // for synchronous PDO set the shadow buffer
  if ((pPdo->flags & PDOFLAG_SYNC) != 0)
  {
    pBuf = pPdo->shadowData;
  }
# endif // (defined(CONFIG_SYNC_CONSUMER) || defined(CONFIG_SYNC_PRODUCER)) && defined(CONFIG_PDO_PRODUCER)

  if (CMS_MapEncode(pPdo->pMapEntries, pBuf) > 8)
  {
    return (CO_E_MAP);
  }

# ifdef CONFIG_FULLCAN
#  ifdef CONFIG_ONLY_ONE_TRANSMIT_CHANNEL
#  else // CONFIG_ONLY_ONE_TRANSMIT_CHANNEL
  UPDATE_COB(pPdo->pCOB, pBuf);
#  endif // CONFIG_ONLY_ONE_TRANSMIT_CHANNEL
# endif // CONFIG_FULLCAN

  return(CO_OK);
}
#endif // CONFIG_PDO_PRODUCER

#if (defined(CONFIG_SYNC_CONSUMER) || defined(CONFIG_SYNC_PRODUCER)) && defined(CONFIG_PDO_PRODUCER)
/*******************************************************************
*
*++ updateSyncTpdo - update all synchronous pdos after received the sync
*-- updateSyncTpdo - update alle synchronen pdos nach dem sync Empfang
*
*++ The function updates the new PDO data to the transmit buffer.
*++ they will be transmitted with the following SYNC object.
*-- Diese Funktion aktualisiert die Sendepuffer der zyklischen PDOs
*-- Diese werden mit dem nächsten SYNC gesendet.
*
*++ This service is only available in the node state OPERATIONAL.
*-- Dieser Dienst ist nur im Zustand OPERATIONAL verfügbar.
*
* \retval CO_OK
*++ success
*-- Erfolg
* \retval CO_E_STATE
*++ node isn't in state OPERATIONAL
*-- Knoten ist nicht im Zustand OPERATIONAL
*
*/

RET_T updateSyncTpdo(void)
{
PDO_T *pPdo; // pointer to actual pdo

  // ensures that node exist
  assert(co_pNode != NULL);

  if (co_pNode->eState != OPERATIONAL)
  {
    return(CO_E_STATE);
  }

  pPdo = co_pFirstTrPdo;
  while (pPdo != NULL)
  {
    // PDO enabled and synchronous and cyclic
    if ((pPdo->flags & (PDOFLAG_SYNC | PDOFLAG_DISABLED | PDOFLAG_CYCLIC)) == (PDOFLAG_SYNC | PDOFLAG_CYCLIC))
    {
      pPdo->curCount--;
      if (pPdo->curCount == 0)
      {
        pPdo->curCount = pPdo->transType;

        // update the buffer - test for valid count was tested at mapset
        prepareTransPdo(pPdo, pPdo->shadowData);

        pPdo->flags |= PDOFLAG_TOTRANSMIT;
      }
    }
    pPdo = pPdo->pNext;
  }

  return(CO_OK);
}
#endif /* (defined(CONFIG_SYNC_CONSUMER) || defined(CONFIG_SYNC_PRODUCER)) && defined(CONFIG_PDO_PRODUCER) */


#ifdef CONFIG_PDO_PRODUCER
/****************************************************************************/
/**
*++ \brief writePdoReq - transmit a PDO to the client(s)
*-- \brief writePdoReq - sendet eine PDO zu den Client(s)
*
*++ The function writes a PDO to a transmit buffer.
*++ Asynchronous PDOs are transmitted immediately.
*++ With synchronous PDOs the PDOs sync-buffer values will be updated
*++ and they are transmitted with the following SYNC object.
*-- Diese Funktion schreibt eine PDO in den Sendepuffer.
*-- Asynchrone PDOs werden sofort gesendet.
*-- Bei synchronen PDOs werden nur die Daten des Schattenpuffers aktualisiert,
*-- die mit dem nächsten gültigen SYNC gesendet werden.
* 
*++ This service is only available in the node state OPERATIONAL.
*-- Dieser Dienst ist nur im Zustand OPERATIONAL verfügbar.
* 
*++ The paramter is only the PDO number to send.
*++ The function is looking in the mapping list to determine
*++ which entries of the object dictionary it has
*++ copy into the transmit COB.
*-- Der einzige Parameter dieser Funktion ist die PDO Nummer.
*-- Die Funktion durchsucht die Mapping-Liste um diejenigen
*-- Objektverzeichniseinträge zu ermitteln, welche in das Sende-COB
*-- kopiert werden.
*
* \retval CO_OK
*++ success
*-- Erfolg
* \retval CO_E_NOT_EXIST
*++ PDO doesn't exists
*-- PDO existiert nicht
* \retval CO_E_STATE
*++ node isn't in state OPERATIONAL
*-- Knoten ist nicht im Zustand OPERATIONAL
* \retval CO_E_INHIBIT
*++ inhibit time is still valid
*-- Sperrzeit ist noch gültig
* \retval CO_E_DISABLED
*++ PDO is disabled
*-- PDO ist disabled
* \retval CO_E_TYPE
*++ bad transmission type (MPDO usage)
*-- Falscher Transmission Type (bei MPDO)
* \retval CO_E_MAP
*++ invalid mapping
*-- Mapping ungültig
*
*/

RET_T writePdoReq
(
  UNSIGNED16 pdoNr    // number of Transmit PDO
)
{
UNSIGNED8  pData[8]; // mapping transmit buffer
PDO_T     *pPdo;     // pointer to actual pdo structure
RET_T      ret;      // return value

  // ensures that node exist
  assert(co_pNode  != NULL);

  if ((pPdo = pdoExist(pdoNr, TRANSMIT_PDO CO_COMMA_LINE_PARA)) == NULL)
  {
    return(CO_E_NOT_EXIST);
  }

  if ((pPdo->flags & PDOFLAG_RTR) != 0)
  {
    return(CO_E_NOT_EXIST);
  }

# ifdef CONFIG_MPDO_DEST
  if ((pPdo->mpdoFlags & MPDOFLAG_DEST_PRODUCER) != 0)
  {
    // mpdo has to send by writeMPdoReq()
    return(CO_E_TYPE);
  }
# endif // CONFIG_MPDO_DEST_PRODUCER
# ifdef CONFIG_MPDO_SRC
  if ((pPdo->mpdoFlags & MPDOFLAG_SRC_PRODUCER) != 0)
  {
    // mpdo has to send by writeMPdoReq()
    return(CO_E_TYPE);
  }
# endif // CONFIG_MPDO_DEST_PRODUCER

  // only asynchronous PDOs are to transmit immediately
  // synchronous acyclic have to be marked for sending
# if defined(CONFIG_SYNC_CONSUMER) || defined(CONFIG_SYNC_PRODUCER)
  if ((pPdo->flags & PDOFLAG_SYNC) != 0)
  {
    if ((ret = prepareTransPdo(pPdo, pPdo->shadowData)) != CO_OK)
    {
      return(ret);
    }
    if ((pPdo->flags & PDOFLAG_CYCLIC) == 0)
    {
      pPdo->flags |= PDOFLAG_TOTRANSMIT;
    }
  }
  else
# endif // defined(CONFIG_SYNC_CONSUMER) || defined(CONFIG_SYNC_PRODUCER)
  {
    if ((ret = prepareTransPdo(pPdo, pData)) != CO_OK)
    {
      return(ret);
    }
    // if inhibit timer is running
    if (pPdo->inhibit.ticks > 0)
    {
      return(CO_E_INHIBITED);
    }
    TRANSMIT_COB(pPdo->pCOB, pData);
    if (pPdo->wInhibitTime > 0)
    {
      startInhibitTimer(&pPdo->inhibit, pPdo->wInhibitTime CO_COMMA_LINE_PARA);
    }

# ifdef CONFIG_PDO_EVENTTIMER
    // start event timer
    if (pPdo->timer.timerVal != 0)
    {
      // removeTimerEvent(&pPdo->timer);
      addTimerEvent(&pPdo->timer, pPdo->timer.timerVal, CO_TIMER_TYPE_EVENTTPDO | CO_TIMER_TYPE_CYCLIC CO_COMMA_LINE_PARA);
    }
# endif // CONFIG_PDO_EVENTTIMER
  }
  return(CO_OK);
}


/****************************************************************************/
/**
*++ \brief updatePdoReq - update a PDO at CAn controller
*-- \brief updatePdoReq - aktualisiseren eines PDO im CAN Controller
*
*++ The function updates only a PDO at the CAN transmit buffer.
*++ It is used for RTR only PDOs to update the CAN controller.
*-- Diese Funktion aktualisiert die Daten eines PDO im CAN-Controller.
*-- Sie wird für RTR only PDOs verwendet,
*-- um den CAN Controller zu aktualisieren
* 
*++ This service is only available in the node state OPERATIONAL.
*-- Dieser Dienst ist nur im Zustand OPERATIONAL verfügbar.
* 
*++ The paramter is only the PDO number to send.
*++ The function is looking in the mapping list to determine
*++ which entries of the object dictionary it has
*++ copy into the transmit COB.
*-- Der einzige Parameter dieser Funktion ist die PDO Nummer.
*-- Die Funktion durchsucht die Mapping-Liste um diejenigen
*-- Objektverzeichniseinträge zu ermitteln, welche in das Sende-COB
*-- kopiert werden.
*
* \retval CO_OK
*++ success
*-- Erfolg
* \retval CO_E_NOT_EXIST
*++ PDO doesn't exists
*-- PDO existiert nicht
* \retval CO_E_STATE
*++ node isn't in state OPERATIONAL
*-- Knoten ist nicht im Zustand OPERATIONAL
* \retval CO_E_DISABLED
*++ PDO is disabled
*-- PDO ist disabled
* \retval CO_E_MAP
*++ invalid mapping
*-- Mapping ungültig
*
*/

RET_T updatePdoReq(
      UNSIGNED16 pdoNr    /**< number of Transmit PDO */
      )
{
UNSIGNED8  pData[8];        /* mapping transmit buffer */
PDO_T     *pPdo;        /* pointer to actual pdo structure */
RET_T      ret;         /* return value */

    /* ensures that node exist */
    assert(co_pNode  != NULL);

    if ((pPdo = pdoExist(pdoNr, TRANSMIT_PDO CO_COMMA_LINE_PARA)) == NULL) {
    return(CO_E_NOT_EXIST);
    }

    if ((ret = prepareTransPdo(pPdo, pData)) != CO_OK)  {
    return(ret);
    }

# ifdef CONFIG_FULLCAN
    UPDATE_COB(pPdo->pCOB, pData);
# endif /* CONFIG_FULLCAN */

# if defined(CONFIG_SYNC_CONSUMER) || defined(CONFIG_SYNC_PRODUCER)
    if ((pPdo->flags & PDOFLAG_SYNC) != 0) {
    prepareTransPdo(pPdo, pPdo->shadowData);
    }
# endif /* defined(CONFIG_SYNC_CONSUMER) || defined(CONFIG_SYNC_PRODUCER) */

    return(CO_OK);
}
#endif /* CONFIG_PDO_PRODUCER */


#ifdef CONFIG_PDO_CONSUMER
/****************************************************************************/
/**
*++ \brief readPdoReq - request a remote transmission for a PDO
*-- \brief readPdoReq - fordert eine PDO über RTR an
*
*++ This function requests a remote transmission for a PDO.
*++ The service is available for all PDOs but
*++ can only be used in the node state OPERATIONAL.
*-- Diese Funktion fordert ein PDO über einen RTR-Frame an.
*-- Der Dienst ist verfügbar für alle PDOs, kann aber nur
*-- im Zustand OPERATIONAL genutzt werden.
* 
*++ A special kind are the 'only RTR PDOs' (type 252 and 253).
*++ The difference between
*++ both kind of RTR is the updating of the variables.
*++ Type 252 (sychronous RTR PDO) will be updated with the SYNC.
*++ With the other type (asynchonous RTR PDO)
*++ the values will be updated if an RTR occurs.
*-- Einen Spezialfall sind die 'only RTR PDOs' (type 252 and 253).
*-- Der Unterschied zwischen beiden Typen ist die Aktualisierung der Daten.
*-- Die sychrone RTR PDO (252) aktualisiert mit dem SYNC, die asynchrone PDO
*-- mit dem RTR Signal.
* 
*++ If a Full CAN controller is used
*++ such an updating must be ensured by the application because the controllers
*++ hardware handles the RTR on layer2 in the CAN chip.
*-- Wenn ein Full CAN Controller eingesetzt wird, muß die Datenaktualisierung
*-- durch die Applikation sichergestellt werden, da die Hardware den RTR
*-- direkt auf dem Chip in der Schicht 2 auswertet.
*
* \retval CO_OK
*++ success
*-- Erfolg
* \retval CO_E_NOT_EXIST
*++ PDO doesn't exists or isn't RTR
*-- PDO existiert nicht oder unterstützt kein RTR
* \retval CO_E_STATE
*++ node isn't in state OPERATIONAL
*-- Knoten ist nicht im Zustand OPERATIONAL
* \retval CO_E_INHIBIT
*++ inhibit time is still valid
*-- Sperrzeit ist noch gültig
* \retval CO_E_DISABLED
*++ PDO is disabled
*-- PDO ist disabled
*
*/

RET_T readPdoReq(
      UNSIGNED16 pdoNr    /**< number of Receive PDO */
      )
{
PDO_T  *pPdo;  /* current used PDO */

    /* ensures that node exist */
    assert( co_pNode  != NULL);

    if (co_pNode  ->eState != OPERATIONAL)  {
    return(CO_E_STATE);
    }

    if ((pPdo = pdoExist(pdoNr, RECEIVE_PDO CO_COMMA_LINE_PARA)) == NULL)  {
    return(CO_E_NOT_EXIST);
    }

    /* if not RTR or PDO disabled do nothing */
    if ((pPdo->flags & PDOFLAG_DISABLED) != 0) {
    return(CO_E_DISABLED);
    }

    TRANSMIT_COB(pPdo->pCOB, NULL);

# ifdef CONFIG_PDO_EVENTTIMER
    if (pPdo->timer.timerVal != 0)  {
    /* removeTimerEvent(&pPdo->timer); */
    addTimerEvent(&pPdo->timer, pPdo->timer.timerVal,
        CO_TIMER_TYPE_EVENTRPDO | CO_TIMER_TYPE_CYCLIC
        CO_COMMA_LINE_PARA);
    }
# endif /* CONFIG_PDO_EVENTTIMER */
    pPdo->flags |= PDOFLAG_OUTSTANDING;

    return(CO_OK);
}
#endif /* CONFIG_PDO_CONSUMER */


#ifdef CONFIG_PDO_CONSUMER
/*******************************************************************
*
*++ pdoMsgReceived - pdo message received
*-- pdoMsgReceived - PDO message erhalten
*
* NOMANUAL
*
*++ This function edit the received pdos
*-- Diese Funktion bearbeitet die Empfangs-PDOs
*
* RETURNS
*   nothing
*
*/

void pdoMsgReceived(CAN_MSG_T *canMsg) // Pointer to CAN Message
{
PDO_T       *pPdo;      // pointer to actual pdo
# if defined(CONFIG_MPDO_DEST)
UNSIGNED8   *pData;     // pointer to object address
UNSIGNED32  size;       // object size
# endif // defined(CONFIG_MPDO_DEST)
# if defined(CONFIG_MPDO_DEST) || defined(CONFIG_MPDO_SRC)
UNSIGNED8   tmpU8;      // temporary U8 variable
UNSIGNED16  tmpU16;     // temporary U16 variable
# endif // defined(CONFIG_MPDO_DEST) || defined(CONFIG_MPDO_SRC)

  assert(co_pNode != NULL);

  if (co_pNode->eState != OPERATIONAL)
  {
    return;
  }

  if ((pPdo = findEvAddr(co_pFirstRecPdo, canMsg->wCOB_ID)) == NULL)
  {
    return;
  }

  // pdo found, test is it enabled
  if ((pPdo->flags & PDOFLAG_DISABLED) != 0)
  {
    return;
  }

  // test for valid data count
  if (canMsg->length < pPdo->pCOB->bLength)
  {
# ifdef CONFIG_EMCY_PRODUCER
    // send an emergency
    writeEmcyReq(ERRCODE_BAD_PDOPARA, 0, 0, 0 CO_COMMA_LINE_PARA);
# endif // CONFIG_EMCY_PRODUCER
    return;
  }

# if defined(CONFIG_SYNC_CONSUMER) || defined(CONFIG_SYNC_PRODUCER)
  // synchronous PDO
  if ((pPdo->flags & PDOFLAG_SYNC) != 0)
  {
    // toUpdate
    pPdo->flags |= PDOFLAG_TOUPDATE;
    pPdo->curCount = pPdo->transType;
    CO_MEMCPY(pPdo->shadowData, &canMsg->pData[0], 8);
  }
  else // asynchronous PDO
# endif // defined(CONFIG_SYNC_CONSUMER) || defined(CONFIG_SYNC_PRODUCER)
  {
# if defined(CONFIG_MPDO_DEST)
    if ((pPdo->mpdoFlags & MPDOFLAG_DEST_CONSUMER) != 0)
    {
      // src node id
      tmpU8 = canMsg->pData[0] & 0x7f;
      // test for valid node id
      if ((tmpU8 != 0) && (tmpU8 != coNodeId ))
      {
        return;
      }
      // src index
      tmpU16 = (((UNSIGNED16)canMsg->pData[2]) << 8) + canMsg->pData[1];
      // src subIndex
      tmpU8 = canMsg->pData[3];
      if (getObjAddr(tmpU16, tmpU8, &pData, &size CO_COMMA_LINE_PARA) != CO_OK)
      {
        return;
      }
      putObj(tmpU16, tmpU8, &canMsg->pData[4], size, CO_FALSE CO_COMMA_LINE_PARA);
      mpdoInd(pPdo->pdoNr, tmpU16, tmpU8 CO_COMMA_LINE_PARA);
    }
    else 
# endif // defined(CONFIG_MPDO_DEST)
# if defined(CONFIG_MPDO_SRC)
    if ((pPdo->mpdoFlags & MPDOFLAG_SRC_CONSUMER) != 0)
    {
      if (mpdoSrcModeReceived(canMsg->pData CO_COMMA_LINE_PARA) == CO_OK)
      {
        // save pdo was ok
        // src index
        tmpU16 = (((UNSIGNED16)canMsg->pData[2]) << 8) + canMsg->pData[1];
        // src subIndex
        tmpU8 = canMsg->pData[3];
        mpdoInd(pPdo->pdoNr, tmpU16, tmpU8 CO_COMMA_LINE_PARA);
      }
    }
    else 
# endif // defined(CONFIG_MPDO_SRC)
    {
      CMS_MapDecode(pPdo->pMapEntries, &canMsg->pData[0]);
      pdoInd(pPdo->pdoNr CO_COMMA_LINE_PARA);
    }
  }

  // outstanding rtr
  if ((pPdo->flags & PDOFLAG_OUTSTANDING) != 0)
  {
    pPdo->flags &= ~PDOFLAG_OUTSTANDING;
  }

# ifdef CONFIG_PDO_EVENTTIMER
  // restart timer pdo timer
  if (pPdo->timer.timerVal != 0)
  {
    // removeTimerEvent(&pPdo->timer);
    addTimerEvent(&pPdo->timer, pPdo->timer.timerVal, CO_TIMER_TYPE_EVENTRPDO | CO_TIMER_TYPE_CYCLIC CO_COMMA_LINE_PARA);
  }
# endif // CONFIG_PDO_EVENTTIMER
}
#endif // CONFIG_PDO_CONSUMER

#ifdef CONFIG_PDO_PRODUCER
/*******************************************************************
*
*++ pdoRtrMsgReceived - pdo RTR message received
*-- pdoRtrMsgReceived - PDO RTR message erhalten
*
* NOMANUAL
*
*++ This function edit the received RTR for transmit pdos
*-- Diese Funktion bearbeitet die RTR für Transmit-PDOs
*
* RETURNS
*   nothing
*
*/

void pdoRtrMsgReceived(
    CAN_MSG_T *canMsg       /* Pointer to CAN Message */
    )
{
PDO_T    *pPdo;         /* pointer to actual pdo */
UNSIGNED8   pData[8];   /* transmit buffer */

    if ((pPdo = findEvAddr(co_pFirstTrPdo , 
        canMsg->wCOB_ID)) != NULL)  {

    /* PDO found */
    if (prepareTransPdo(pPdo, pData) == CO_OK)  {
        /* asynchron PDO has to be transmitted immediately */
        /* synchron PDO has to transmit at the next SYNC */
        if ((pPdo->flags & PDOFLAG_SYNC) != 0) {
        pPdo->flags |= PDOFLAG_TOTRANSMIT;
# ifdef CONFIG_FULLCAN
# else /* CONFIG_FULLCAN */
        } else  {
        TRANSMIT_COB(pPdo->pCOB, pData);
        /* if inhibittime is set, start timer */
        if (pPdo->wInhibitTime > 0)  {
            startInhibitTimer(&pPdo->inhibit, pPdo->wInhibitTime
            CO_COMMA_LINE_PARA);
        }
#  ifdef CONFIG_PDO_EVENTTIMER
        /* reload the event time */
        if (pPdo->timer.timerVal != 0)  {
            /* removeTimerEvent(&pEv->timer); */
            addTimerEvent(&pPdo->timer, pPdo->timer.timerVal,
            CO_TIMER_TYPE_EVENTTPDO | CO_TIMER_TYPE_CYCLIC
            CO_COMMA_LINE_PARA);
        }
#  endif /* CONFIG_PDO_EVENTTIMER */
# endif /* CONFIG_FULLCAN */
        }
    }
    }
}
#endif /* CONFIG_PDO_PRODUCER */


#if (defined(CONFIG_SYNC_CONSUMER) || defined(CONFIG_SYNC_PRODUCER)) && defined(CONFIG_PDO_PRODUCER)
/*******************************************************************
*
* transSyncPdo - start transmission of synchronous PDO
*
* NOMANUAL
*
* This function inserts all transmission requests for synchronous PDOs
* into transmission buffer.
*
* \return
* nothing
*
*/

void transSyncPdo(void)
{
PDO_T *pPdo; // pointer to actual pdo

  assert(co_pNode  != NULL);

  if (co_pNode->eState != OPERATIONAL)
  {
    return;
  }

  pPdo = co_pFirstTrPdo;
  while (pPdo != NULL)
  {
    // PDO enabled and toTransmit
    if ((pPdo->flags & (PDOFLAG_SYNC | PDOFLAG_DISABLED | PDOFLAG_TOTRANSMIT)) == (PDOFLAG_SYNC | PDOFLAG_TOTRANSMIT))
    {
      /* here can be included a user function
         for update the objectdictionary parts
         which have influence to the TPDOs or
         synchronous RTR PDOs */
# ifdef CONFIG_UPDATE_SYNC_ACTUAL_MSG
      updateSyncActualMsg(pPdo->pdoNr,pPdo->shadowData CO_COMMA_LINE_PARA);
# endif // CONFIG_UPDATE_SYNC_ACTUAL_MSG
      // copy to transmit buffer and send
      TRANSMIT_COB(pPdo->pCOB, pPdo->shadowData);
      // for acyclic PDO reset transmit request
      pPdo->flags &= ~PDOFLAG_TOTRANSMIT;
    }
    pPdo = pPdo->pNext;
  }
}
#endif // (defined(CONFIG_SYNC_CONSUMER) || defined(CONFIG_SYNC_PRODUCER)) && defined(CONFIG_PDO_PRODUCER)




#if (defined(CONFIG_SYNC_CONSUMER) || defined(CONFIG_SYNC_PRODUCER)) && defined(CONFIG_PDO_CONSUMER)
/*******************************************************************
*
* updateSyncRpdo - updates the values of synchronous RPDOs
*
* NOMANUAL
*
* This function will be called if a SYNC occurs.
* Its task is to copy the values to the object dictionary.
* This procedure will be done only
* for acyclic PDOs and cyclic PDOs which SYNC counter
* was decremented to zero and additionally the flag toUpdate is 1.
*
* \return
* nothing
*
*/

void updateSyncRpdo(
    void
     )
{
PDO_T   *pPdo;      /* pointer to current sync. RPDO buffer */
BOOL_T  update;     /* help variable - should update be started */

    assert(co_pNode  != NULL);

    if(co_pNode  ->eState != OPERATIONAL)  {
    return;
    }

    pPdo = co_pFirstRecPdo ;
    while (pPdo != NULL) {
    /* PDO disabled */
    if ((pPdo->flags & (PDOFLAG_DISABLED | PDOFLAG_SYNC | PDOFLAG_TOUPDATE))
        == (PDOFLAG_SYNC | PDOFLAG_TOUPDATE )) {

        /* is this a cyclic pdo ? */
        if ((pPdo->flags & PDOFLAG_CYCLIC) != 0) {
        pPdo->curCount--;
        if (pPdo->curCount == 0)  {
            update = CO_TRUE;
            pPdo->curCount = pPdo->transType;
        }  else  {
            update = CO_FALSE;
        }
        } else  {
        /* acyclic */
        update = CO_TRUE;
        }
    
        if (update == CO_TRUE)  {
        pPdo->flags &= ~PDOFLAG_TOUPDATE;

        CMS_MapDecode(pPdo->pMapEntries, pPdo->shadowData);

        pdoInd(pPdo->pdoNr CO_COMMA_LINE_PARA);
        }
    }
    pPdo = pPdo->pNext;
    }
}
#endif /* (defined(CONFIG_SYNC_CONSUMER) || defined(CONFIG_SYNC_PRODUCER)) && defined(CONFIG_PDO_CONSUMER) */



#if (defined(CONFIG_PDO_CONSUMER) || defined(CONFIG_PDO_PRODUCER))
# ifdef CONFIG_PDO_EVENTTIMER
#  ifdef CONFIG_PDO_PRODUCER
/*******************************************************************
*
*++ eventTransPdo - process event Transmit PDOs 
*-- eventTransPdo - event Transmit PDOs bearbeiten
*
* NOMANUAL
*
*++ This function tests all event TPDOs
*++ and transmits it if the event-time is over.
*++ All RTR requested RPDOs are supervised according there TimeOut value.
*-- Diese Funktion testet alle TPDOs und weist die Sendung an
*-- wenn die Event-Zeit abgelaufen ist.
*-- Gleichzeitig werden alle per RTR angeforderten RPDOs
*-- auf ihr TimeOut überwacht.
*
* \return
*   nothing
*
*/

void eventTransPdo(TIMER_EVENT_T *pTimer) // pointer to timer event structure
{
PDO_T     *pPdo;     // pointer to actual pdo
UNSIGNED8 buffer[8]; // transmit buffer

  // timer structure is the first structure at pdo structure,
  // therefore the pointer are the same
  pPdo = (PDO_T *)pTimer;

  if (co_pNode->eState != OPERATIONAL)
  {
    return;
  }

  if (prepareTransPdo(pPdo, buffer) == CO_OK)
  {
    TRANSMIT_COB(pPdo->pCOB, buffer);
    // reload for the timer event is not necessary (it's a cyclic timer)
  }
}
#  endif /* CONFIG_PDO_PRODUCER */


#  ifdef CONFIG_PDO_CONSUMER
/*******************************************************************
*
*++ eventRecPdo - process event Receive PDOs 
*-- eventRecPdo - event Receive PDOs bearbeiten
*
* NOMANUAL
*
*++ This function tests all event RPDOs
*++ and checks it if the event-time is over.
*-- Diese Funktion testet alle RPDOs und checkt
*-- ob die Event-Zeit abgelaufen ist.
*
* \return
*   nothing
*
*/
void eventRecPdo(
    TIMER_EVENT_T   *pTimer     /* pointer to timer event structure */
    )
{
PDO_T       *pPdo;          /* pointer to actual pdo */

    /* timer structure is the first structure at pdo structure,
     * therefore the pointer are the same */
    pPdo = (PDO_T *)pTimer;

    if (co_pNode  ->eState != OPERATIONAL)  {
    return;
    }

    /* if ((pPdo->flags & PDOFLAG_OUTSTANDING) != 0)  { */
    /* call user function */
    pdoTimerInd(pPdo->pdoNr CO_COMMA_LINE_PARA);
    pPdo->flags &= ~PDOFLAG_OUTSTANDING;
    /* } */
}
#  endif /* CONFIG_PDO_CONSUMER */
# endif /* CONFIG_PDO_EVENTTIMER */
#endif /* (defined(CONFIG_PDO_CONSUMER) || defined(CONFIG_PDO_PRODUCER)) */


#if defined(CONFIG_PDO_CONSUMER) || defined(CONFIG_PDO_PRODUCER)
/*******************************************************************
*
*++ checkMappingEntry - check one mapping entry
*-- checkMappingEntry - prüft ein Mapping Eintrag
*
* NOMANUAL
*
*++ This function checks one mapping entry
*-- Diese Funktion testet einen einen Mapping Eintrag.
*-- Dazu werden für normale Mappingeinträge die Attribute getestet.
*-- Dummy Mapping Einträge sind nur für Receive PDOs erlaubt.
*-- Weiterhin wird die Objektgröße mit dem Mapping Eintrag überprüft
*
*
* \return
*   RET_T
*/

RET_T checkMappingEntry(
    UNSIGNED16 index,       // pdo Mapping table index
    UNSIGNED32 newMapEntry  // new mapping entry
   )
{
UNSIGNED16  mapIndex;    // Mapping index
UNSIGNED8   mapSubIndex; // Mapping subIndex
UNSIGNED8   mapLength;   // Mapping len
UNSIGNED8   attr, kind;  // object attribute
UNSIGNED8   *addr;       // pointer to address
UNSIGNED32  size;        // object size

  mapIndex    = (UNSIGNED16) ((newMapEntry  >> MAP_INDEX_SHIFT)    & MAP_INDEX_MASK);
  mapSubIndex = (UNSIGNED8)  ((newMapEntry  >> MAP_SUBINDEX_SHIFT) & MAP_SUBINDEX_MASK);
  mapLength   = (UNSIGNED8)   (newMapEntry                         & MAP_LENGTH_MASK);

  // get attribute of the mapping index
  attr = getObjAttr(mapIndex, mapSubIndex CO_COMMA_LINE_PARA);

  // PDO Mapping Receive PDO
  if ((index >= RPDO_MAP_BASE_INDEX) && (index <= RPDO_MAP_LAST_INDEX))
  {
    // for objects 1 -7 (basic datatypes) mapping is always possible (dummy mapping)
    // dummy or real mapping ?
    if (mapIndex > 0x7)
    {
      // real mapping - variable needs mapping and write permission
      if ((attr & (CO_MAP_PERM | CO_WRITE_PERM)) != (CO_MAP_PERM | CO_WRITE_PERM))
      {
        return(CO_E_MAP);
      }
    }
    kind = RECEIVE_PDO;
#if 0
    pdoNr = index - RPDO_MAP_BASE_INDEX + 1;
#endif
  }
  // PDO Mapping Transmit PDO
  else if ((index >= TPDO_MAP_BASE_INDEX) && (index <= TPDO_MAP_LAST_INDEX))
  {
    // check for dummy mapping
    if (mapIndex > 0x7)
    {
      // no dummy mapping - var needs read and mapping permission
      if ((attr & (CO_MAP_PERM | CO_READ_PERM)) != (CO_MAP_PERM | CO_READ_PERM))
      {
        return(CO_E_MAP);
      }
    }
    else
    {
      // dummy mapping for transmit PDOs is not allowed
      // exception for delete this mapping entry
      if (newMapEntry != 0)
      {
        return(CO_E_MAP);
      }
    }
    kind = TRANSMIT_PDO;
#if 0
    pdoNr = index - TPDO_MAP_BASE_INDEX + 1;
#endif
  }
  else
  {
    // no valid index given
    return(CO_E_NOT_EXIST);
  }

  // search for pdo entry
  if ((pdoExist((index & 0x1ff) + 1, kind CO_COMMA_LINE_PARA)) == NULL)
  {
    // pdo is not initialized
    return(CO_E_NOT_EXIST);
  }

  // check for correct mapping length
  // dummy mapping ?
  if (mapIndex > 0x7)
  {
    // no, get size and check for correct data len
    if (getObjAddr(mapIndex, mapSubIndex, (UNSIGNED8 **)&addr, &size CO_COMMA_LINE_PARA) != CO_OK)
    {
      return(CO_E_NOT_EXIST);
    }
    // calculate the length in bits
    size = size << 3;
  }
  else
  {
    // dummy mapping
    size = getOvDataTypeLen(mapIndex);
  }

# ifdef CONFIG_BIT_ENCODING
  // At the moment, we can't get bit variables, therefore we test only for to big mapping size
  if (mapLength > size)
  {
# else // CONFIG_BIT_ENCODING
    if (mapLength != size)
    {
# endif // CONFIG_BIT_ENCODING
      return(CO_E_MAP);
    }
  // zero mapping isn't allowed
  if (mapLength == 0)
  {
    return(CO_E_MAP);
  }
  
  return(CO_OK);
}


/*******************************************************************
*
*++ checkMappingTable - check the complete mapping table
*-- checkMappingTable - prüft die gesamte Mapping Tabelle
*
* NOMANUAL
*
*++ This function checks the whole mapping table.
*++ Each mapping entry is tested by the function
*-- Diese Funktion testet die komplette Mapping Tabelle.
*-- Jeder Mappingeintrag wird mit Hilfe der Funktion
* checkMappingEntry()
*++ If it is ok,
*++ an internal mapping table is created.
*-- auf Gültigkeit geprüft.
*-- Anschliessend wird die interne Mappingstruktur aufgebaut.
*
*
* \return
*   RET_T
*
*/

RET_T checkMappingTable(UNSIGNED16 index) // pdo Mapping index
{
UNSIGNED16  mapIndex;      // Mapping index
UNSIGNED8   mapSubIndex;   // Mapping subIndex
UNSIGNED8   mapLength;     // Mapping length
UNSIGNED32  *mapEntry;     // mapping entry
UNSIGNED8   kind;          // type of pdo
UNSIGNED16  pdoNr;         // pdo number
PDO_T       *pPdo;         // pointer to pdo structure
UNSIGNED8   i, mappingCnt; // number of mappings
UNSIGNED32  size;          // object size
PDO_MAP_T   *pMap;         // pointer to mapping data
UNSIGNED8   mapBuf[8];     // mapping buffer

  // search for pdo entry - there are max 512 PDOs possible
  pdoNr = (index & 0x1ff) + 1;

  // if transmit PDO ?
  // if (kind == TRANSMIT_PDO)  {
  if (index > RPDO_MAP_LAST_INDEX)
  {
    kind = TRANSMIT_PDO;
  }
  else
  {
    kind = RECEIVE_PDO;
  }
  if ((pPdo = pdoExist(pdoNr, kind CO_COMMA_LINE_PARA)) == NULL)
  {
    // pdo is not initialized
    return(CO_E_NOT_EXIST);
  }

  // get mapping count
  if (getObjEntry(index, 0, (UNSIGNED8 *)&mappingCnt, &size, CO_TRUE CO_COMMA_LINE_PARA) != CO_OK)
  {
    return(CO_E_NOT_EXIST);
  }

# if defined(CONFIG_MPDO_DEST) || defined(CONFIG_MPDO_SRC)
  // reset mpdo modes
  pPdo->mpdoFlags = 0;
# endif // defined(CONFIG_MPDO_DEST) || defined(CONFIG_MPDO_SRC)

# if defined(CONFIG_MPDO_DEST)
  // MPDO destination mode is signed as 255
  if (mappingCnt == 255)
  {
    // MPDO dest. mode
    // if transmit PDO ?
    // if (kind == TRANSMIT_PDO)  {
    if (index > RPDO_MAP_LAST_INDEX)
    {
      // MPDO Producer
      pPdo->mpdoFlags = MPDOFLAG_DEST_PRODUCER;
      mappingCnt = 1;
    }
    else
    {
      // MPDO Consumer
      pPdo->mpdoFlags = MPDOFLAG_DEST_CONSUMER;
      mappingCnt = 0;
    }
  }
# endif // defined(CONFIG_MPDO_DEST)

# if defined(CONFIG_MPDO_SRC)
  // MPDO source mode is signed as 254
  if (mappingCnt == 254)
  {
    // MPDO src mode
    mappingCnt = 0;
    // if transmit PDO ?
    // if (kind == TRANSMIT_PDO)  {
    if (index > RPDO_MAP_LAST_INDEX)
    {
      // MPDO Producer
      pPdo->mpdoFlags = MPDOFLAG_SRC_PRODUCER;
      if (testMPdoScannerList(CO_LINE_PARA) != CO_OK)
      {
        return(CO_E_MAP);
      }
    }
    else
    {
      // MPDO Consumer
      pPdo->mpdoFlags = MPDOFLAG_SRC_CONSUMER;
      // test the mapping entries at the dispatcher list
      if (testMPdoDispatcherList(CO_LINE_PARA) != CO_OK)
      {
        return(CO_E_MAP);
      }
    }
  }
# endif // defined(CONFIG_MPDO_SRC)

  // get start of mapping table
  pMap = pPdo->pMapEntries;

  // Now check each mapping entry
  for (i = 1; i <= mappingCnt; i++)
  {
    // check for valid mapping structure
    if (pMap == NULL)
    {
      return(CO_E_MAP);
    }
    
    // get mapping entry
    if (getObjAddr(index, i, (UNSIGNED8 **)&mapEntry, &size CO_COMMA_LINE_PARA) != CO_OK)
    {
      return(CO_E_NOT_EXIST);
    }

    // first check the mapped value
    if (checkMappingEntry(index, *mapEntry CO_COMMA_LINE_PARA) != CO_OK)
    {
      return(CO_E_MAP);
    }

    // set temporary variables for easier access
    mapIndex    = (UNSIGNED16)((*mapEntry >> MAP_INDEX_SHIFT) & MAP_INDEX_MASK);
    mapSubIndex = (UNSIGNED8) ((*mapEntry >> MAP_SUBINDEX_SHIFT) & MAP_SUBINDEX_MASK);
    mapLength   = (UNSIGNED8)  (*mapEntry & MAP_LENGTH_MASK);

    // if no dummy mapping, get address of mapped object
    if (mapIndex > 0x7)
    {
      // get real mapping entry
      if (getObjAddr(mapIndex, mapSubIndex, (UNSIGNED8 **)&pMap->pAddress, &size CO_COMMA_LINE_PARA) != CO_OK)
      {
        return(CO_E_NOT_EXIST);
      }
      // set object type
      // is this a numeric value
      if ((getObjAttr(mapIndex, mapSubIndex CO_COMMA_LINE_PARA) & CO_NUM_VAL) == CO_NUM_VAL)
      {
        pMap->eBasicType = CO_UNSIGNED;
      }
      else
      {
        pMap->eBasicType = CO_STRING;
      }
    }
    else
    { // dummy mapping objects 1 - 7
      // dummy mapping only for receive PDOs
      pMap->eBasicType = CO_DUMMY_SPACE;
      pMap->pAddress = NULL;
    }

    pMap->bBitSize = mapLength;

    // increment pointer
    pMap = pMap->pNext;
  }
    
  // sign last entry for not full list
  if (pMap != NULL)
  {
    pMap->eBasicType = CO_INVALID;
  }

  // check for valid data count
  if (mappingCnt != 0)
  {
# ifdef CONFIG_PDO_PRODUCER
    pPdo->pCOB->bLength = CMS_MapEncode(pPdo->pMapEntries, mapBuf);
# else // CONFIG_PDO_PRODUCER
    pPdo->pCOB->bLength = CMS_MapDecode(pPdo->pMapEntries, mapBuf);
# endif // CONFIG_PDO_PRODUCER
  }
  else
  {
    pPdo->pCOB->bLength = 0;
  }

  // check for valid mapping length
  if (pPdo->pCOB->bLength > 8)
  {
    return(CO_E_MAP);
  }

# if defined(CONFIG_MPDO_DEST) || defined(CONFIG_MPDO_SRC)
  if ((pPdo->mpdoFlags & MPDOFLAG_DEST) || (pPdo->mpdoFlags & MPDOFLAG_SRC))
  {
    // MPDO has always length 8
    pPdo->pCOB->bLength = 8;
  }
# endif // defined(CONFIG_MPDO_DEST) && defined(CONFIG_PDO_PRODUCER)

# ifdef CONFIG_FULLCAN
  UPDATE_COB(pPdo->pCOB, mapBuf);
# endif /* CONFIG_FULLCAN */

  return(CO_OK);
}

#endif /* defined(CONFIG_PDO_CONSUMER) || defined(CONFIG_PDO_PRODUCER) */

#if defined(CONFIG_PDO_CONSUMER) || defined(CONFIG_PDO_PRODUCER)
/*******************************************************************/
/*
*
* setPdoTransType - sets the new PDO Transmission Type
*
* NOMANUAL
*
* This service sets the Transmission Type of PDOs.
*
* \retval CO_OK
* success
* \retval CO_TRANSTYPE
* bad transmission type 
*
*/

RET_T setPdoTransType(PDO_T       *pPdo,     // pointer to actual pdo
                      UNSIGNED8   kind,      // kind of pdo
                      UNSIGNED8   transType) // new Transmission Type
{
  // sync pdo ?
  if (transType < 241)
  {
    // sync PDO
# if (defined(CONFIG_SYNC_PRODUCER) || defined(CONFIG_SYNC_CONSUMER))

    // bit must be set for SYNC PDOs
    if ((pPdo->flags & PDOFLAG_SYNC_POSSIBLE) == 0)
    {
      return(CO_E_TRANS_TYPE);
    }

#  ifdef CONFIG_PDO_EVENTTIMER
    // disable event timer
    {
      UNSIGNED16 tmpU16 = 0;
      setPdoEventTime(pPdo, kind, 0 CO_COMMA_LINE_PARA);
      if (kind == RECEIVE_PDO)
      {
        putObj((UNSIGNED16)(RPDO_PARA_BASE_INDEX + pPdo->pdoNr - 1), 5, (UNSIGNED8 *)&tmpU16, 2, CO_TRUE CO_COMMA_LINE_PARA);
      }
      else
      {
        putObj((UNSIGNED16)(TPDO_PARA_BASE_INDEX + pPdo->pdoNr - 1), 5, (UNSIGNED8 *)&tmpU16, 2, CO_TRUE CO_COMMA_LINE_PARA);
      }
    }
#  endif // CONFIG_PDO_EVENTTIMER

    // set sync sign
    pPdo->flags |= PDOFLAG_SYNC;
    if (transType > 0)
    {
      // cyclic pdo
      pPdo->flags |= PDOFLAG_CYCLIC;
    }
    else
    {
      // acyclic pdo
      pPdo->flags &= ~PDOFLAG_CYCLIC;
    }

    pPdo->curCount = transType;
# else // (defined(CONFIG_SYNC_PRODUCER) || defined(CONFIG_SYNC_CONSUMER))
    return(CO_E_TRANS_TYPE);
# endif // (defined(CONFIG_SYNC_PRODUCER) || defined(CONFIG_SYNC_CONSUMER))

  }
  else if (transType < 252)
  {
    // reserved transmission type
    return(CO_E_TRANS_TYPE);
  }
  else if (transType < 254)
  {
    // only RTR pdos

    // RTR for Receive PDOs are not allowed
    if (kind == RECEIVE_PDO)
    {
      return(CO_E_TRANS_TYPE);
    }

# ifdef CONFIG_ONLY_ONE_TRANSMIT_CHANNEL
    // RTR are not allowed
    return(CO_E_TRANS_TYPE);
#else // CONFIG_ONLY_ONE_TRANSMIT_CHANNEL

    // sync ?
    if (transType == 252)
    {
      // synchron RTR only

# if (defined(CONFIG_SYNC_PRODUCER) || defined(CONFIG_SYNC_CONSUMER))
      pPdo->flags &= ~PDOFLAG_CYCLIC;
      pPdo->flags |= PDOFLAG_SYNC;
# else // (defined(CONFIG_SYNC_PRODUCER) || defined(CONFIG_SYNC_CONSUMER))
      return(CO_E_TRANS_TYPE);
# endif // (defined(CONFIG_SYNC_PRODUCER) || defined(CONFIG_SYNC_CONSUMER))
    }
    else
    {
      pPdo->flags &= ~(PDOFLAG_SYNC | PDOFLAG_CYCLIC);
    }
    pPdo->flags |= PDOFLAG_RTR;
# endif // CONFIG_ONLY_ONE_TRANSMIT_CHANNEL
  }
  else
  {
    pPdo->flags &= ~(PDOFLAG_SYNC | PDOFLAG_CYCLIC | PDOFLAG_RTR);
  }
  pPdo->transType = transType;
  return(CO_OK);
}
#endif // defined(CONFIG_PDO_CONSUMER) || defined(CONFIG_PDO_PRODUCER)

#if defined(CONFIG_PDO_CONSUMER) || defined(CONFIG_PDO_PRODUCER)
# ifdef CONFIG_PDO_EVENTTIMER
/*******************************************************************/
/*
*
* setPdoEventTime - sets the new PDO Event Time
*
* NOMANUAL
*
* This service sets the Event Time of PDOs.
* For the other CANopen Communication Objects
* there is no inhibit time entry in Object Dictionary.
*
* \retval CO_OK
*++ success
*-- Erfolg
* \retval CO_E_NOT_EXIST
* internal communication object doesn't exist
*
*/

RET_T setPdoEventTime(
    PDO_T       *pPdo,      // pointer to actual pdo
    UNSIGNED8   kind,       // kind of pdo
    UNSIGNED16  eventTime   // new Event Time, unit 1ms
      )
{
  pPdo->timer.timerVal = (UNSIGNED32)eventTime * 10;

  // remove old event
  removeTimerEvent(&pPdo->timer CO_COMMA_LINE_PARA);

//  if ((pPdo->flags & PDOFLAG_SYNC) != 0) // TD - test Hoogendoorn
//  {
//    return(CO_E_TRANS_TYPE);
//  }

  if (eventTime != 0)
  {
    if (kind == TRANSMIT_PDO)
    {
      addTimerEvent(&pPdo->timer, pPdo->timer.timerVal, CO_TIMER_TYPE_EVENTTPDO | CO_TIMER_TYPE_CYCLIC CO_COMMA_LINE_PARA);
    }
    else
    {
      addTimerEvent(&pPdo->timer, pPdo->timer.timerVal, CO_TIMER_TYPE_EVENTRPDO | CO_TIMER_TYPE_CYCLIC CO_COMMA_LINE_PARA);
    }
  }
  return(CO_OK);
}
# endif /* CONFIG_PDO_EVENTTIMER */
#endif /* defined(CONFIG_PDO_CONSUMER) || defined(CONFIG_PDO_PRODUCER) */

#if defined(CONFIG_PDO_CONSUMER) || defined(CONFIG_PDO_PRODUCER)
/*******************************************************************/
/*
*
* setPdoInhibitTime - sets the new Inhibit Time
*
* NOMANUAL
*
* This service sets the Inhibit Time of PDOs.
* For the other CANopen Communication Objects
* there is no inhibit time entry in Object Dictionary.
*
* \retval CO_OK
*++ success
*-- Erfolg
* \retval CO_E_NOT_EXIST
* internal communication object doesn't exist
*
*/

RET_T setPdoInhibitTime(
      PDO_T  *pPdo,       /* pointer to actual PDO */
      UNSIGNED16 inhibitTime  /* new Inhibit Time, unit 100us */
      )
{
    pPdo->wInhibitTime = inhibitTime;

    return(CO_OK);
}
#endif /* defined(CONFIG_PDO_CONSUMER) || defined(CONFIG_PDO_PRODUCER) */


#if defined(CONFIG_PDO_CONSUMER) || defined(CONFIG_PDO_PRODUCER)
/*******************************************************************/
/*
*
* setPdoCobId - sets the COB-ID of a PDO
*
* NOMANUAL
*
* This function sets cob-id for PDO
*
* \retval CO_OK
*++ success
*-- Erfolg
* \retval CO_E_NOT_EXIST
* internal communication object doesn't exist
* \retval CO_E_RANGE
* COB-ID is out of the range (1..1760)
* \retval CO_E_TRANS_TYPE
* bad tranatype
*
*/

RET_T setPdoCobId(PDO_T      *pPdo, // pointer to pdo
                  UNSIGNED32 cobId) // new COB-ID
{
  // Extended IDs are not allowed
  if ((cobId & CAN_29_BIT_ID_FLAG) != 0)
  {
    return(CO_E_TRANS_TYPE);
  }

# ifdef CONFIG_ONLY_ONE_TRANSMIT_CHANNEL
  // RTR are not allowed
  cobId |= PDO_NO_RTR_ALLOWED_BIT; // TD - test Hoogendoorn
  if ((cobId & PDO_NO_RTR_ALLOWED_BIT) == 0)
  {
    return(CO_E_TRANS_TYPE);
  }
# endif // CONFIG_ONLY_ONE_TRANSMIT_CHANNEL

  // set internal valid bit
  if ((cobId & PDO_NO_VALID_BIT) != 0)
  {
    pPdo->flags |= PDOFLAG_DISABLED;
  }
  else
  {
    // test for valid COB-Ids
    if (((cobId & CAN_11_BIT_ID_MASK) < CO_COBID_PDO_FIRST) || ((cobId & CAN_11_BIT_ID_MASK) > CO_COBID_PDO_LAST))
    {
      return(CO_E_RANGE);
    }
    pPdo->flags &= ~PDOFLAG_DISABLED;
  }

  // cobId &= CAN_11_BIT_ID_MASK;
  SET_COB_ID(pPdo->pCOB,(UNSIGNED16)cobId);

  return(CO_OK);
}
#endif // defined(CONFIG_PDO_CONSUMER) || defined(CONFIG_PDO_PRODUCER)

#if defined(CONFIG_PDO_CONSUMER) || defined(CONFIG_PDO_PRODUCER)
/****************************************************************************/
/**
*++ \brief getMapObjAddr - return the address of the mapping entry
*-- \brief getMapObjAddr - liefert die Adresse des Mappingeintrages
*
*++ This function looks for the address of the mapping entry.
*++ If the entry is not available or not readable
*++ the function returns the NULL pointer.
*-- Diese Funktion ermittelt die Adresse des gesuchten Mappingeintrages.
*-- Falls kein Mappingeintrag im Objektverzeichnis vorhanden ist
*-- oder der Eintrag nicht lesbar ist,
*-- wird der NULL Zeiger zurückgeliefert.
*
*++ \retval address of mapping entry
*-- \retval Adresse des Mapping Eintrages
*++ \retval NULL
*++ if error occures
*-- bei Fehler
*/

void *getMapObjAddr(
      UNSIGNED16 pdoMapBase,
      UNSIGNED16 pdoNr,  /**< number of RPDO */
      UNSIGNED8  mapNr   /**< number of mapping object (sub index)*/
      )
{
UNSIGNED16 index;       /* object index */
UNSIGNED16 mapIndex;    /* mapping index */
UNSIGNED8  mapSubIndex; /* mapping subindex */
UNSIGNED8  *pData;      /* reference to object dictionary */
UNSIGNED32 dummy, size; /* dummy U32 var */
UNSIGNED8  dummy_u8;    /* dummy U8 var */

  index = pdoMapBase + pdoNr - 1;

  /* test count of mapping entries for variable mapping */
  if (getObjEntry(index, 0, &dummy_u8, &size, CO_TRUE CO_COMMA_LINE_PARA) != CO_OK)
  {
    return NULL;
  }
  if (mapNr > dummy_u8)
  {
    return NULL;
  }
  /* get mapping data */
  if (getObjEntry(index, mapNr, (UNSIGNED8 *)&dummy, &size, CO_TRUE CO_COMMA_LINE_PARA) != 0)
  {
    return NULL;
  }
  mapIndex    = (UNSIGNED16)((dummy  >> MAP_INDEX_SHIFT)    & MAP_INDEX_MASK);
  mapSubIndex = (UNSIGNED8) ((dummy  >> MAP_SUBINDEX_SHIFT) & MAP_SUBINDEX_MASK);

  if (getObjAddr(mapIndex, mapSubIndex, &pData, &dummy CO_COMMA_LINE_PARA) != 0)
  {
    return NULL;
  }

  return((void *)pData);
}
#endif /* defined(CONFIG_PDO_CONSUMER) || defined(CONFIG_PDO_PRODUCER) */
/*______________________________________________________________________EOF_*/
