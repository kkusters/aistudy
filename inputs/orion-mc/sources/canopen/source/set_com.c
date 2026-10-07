/*
 *++ set_com - routines for setting of internal communications variables
 *-- set_com - Routinen für das Setzen interner Kommunikationsvariablen
 *
 * Copyright (c) 1997-2002 port GmbH Halle (Saale)
 *------------------------------------------------------------------
 * $Header$
 *
 *------------------------------------------------------------------
 *
 * modification history
 * --------------------
 * $Log$
 * Revision 1.0  2008-03-07 17:04:48+01  driet
 * Initial revision
 *
 * Revision 2.41  2003/10/01 13:29:25  boe
 * delete unused line parameter for setSdoCobId
 *
 * Revision 2.40  2003/07/30 08:11:44  boe
 * include time and sync header only if CONFIG_TIME/CONFIG_SYNC is set
 *
 * Revision 2.39  2003/06/16 13:20:26  boe
 * add line parameter for function setPdoTransType
 *
 * Revision 2.38  2003/06/13 14:16:06  boe
 * for mapping count from 0 and actualize mapping table
 *
 * Revision 2.37  2003/03/31 12:58:58  boe
 * correct compilerdefines only in one line
 * add CO_WATCH_DOG for reset object dir
 *
 * Revision 2.36  2003/03/20 13:11:35  boe
 * set time cob-id at resetObjDir
 *
 * Revision 2.35  2003/03/20 12:32:11  boe
 * error for resetObjDir removed (only for last version)
 *
 * Revision 2.34  2003/03/11 14:30:36  boe
 * reset objdir subindex for readonly values too
 *
 * Revision 2.33  2003/02/28 16:11:24  boe
 * correct multiline usage for heartbeat consumers
 *
 * Revision 2.32  2003/02/13 14:16:14  boe
 * correct range for reset object dictionary
 *
 * Revision 2.31  2003/02/07 10:53:35  boe
 * change parameter for srdo functions
 *
 * Revision 2.30  2003/01/27 09:35:28  boe
 * set prefefined connection for srdo
 *
 * Revision 2.29  2003/01/09 16:16:48  boe
 * add depend defines for CONFIG_SDO_COB_ID
 * correct type cast for setPdoEventTime
 *
 * Revision 2.28  2002/12/11 08:00:08  boe
 * change function parameter for setPdoEventTime() and setSdoCommPara()
 *
 * Revision 2.27  2002/11/18 09:54:19  boe
 * rework complete functionality
 * add comments/adapt on doxygen
 * add co_ to all global library variables
 * add special things for 16bit CPUs
 * use pointers for function parameter to other functions
 * move functionality outside of setCommPar
 *
 * Revision 2.26  2002/05/30 10:24:11  hae
 * documentation correction
 *
 * Revision 2.25  2002/05/21 14:21:50  boe
 * cleanup new timer usage
 *
 * Revision 2.24  2002/03/26 08:15:34  boe
 * add NEW_TIMER functionality over defines
 *
 * Revision 2.23  2002/02/26 16:26:43  boe
 * add define CONFIG_DS301_V30 for U32 nodeguarding entries
 *
 * Revision 2.22  2001/10/15 13:07:22  ro
 * include added
 * changes for the prospectively timer concept
 *
 * Revision 2.21  2001/07/10 07:20:46  boe
 * correct array index for pEv for multiline
 *
 * Revision 2.20  2001/07/09 16:24:32  boe
 * set sync cob-id flags changed
 *
 * Revision 2.19  2001/06/19 15:27:32  boe
 * added test for 11bit identifier
 * check consumer/producer bits at time and emcy
 *
 * Revision 2.18  2001/05/10 12:36:53  boe
 * structure for data mapping changed
 *
 * Revision 2.17  2001/04/05 12:27:00  boe
 * command line parameter for single line changed to void
 *
 * Revision 2.16  2001/04/05 08:45:54  boe
 * comment changed
 *
 * Revision 2.15  2001/03/29 14:35:43  boe
 * comment changed
 *
 * Revision 2.14  2001/03/28 15:50:14  boe
 * include header files for master only if CONFIG_MASTER is set
 *
 * Revision 2.13  2001/03/28 12:57:33  boe
 * source code cleaned
 *
 * Revision 2.12  2001/03/19 16:00:14  ro
 * some header only include if used - corrected
 *
 * Revision 2.11  2001/03/14 16:16:19  ro
 * setDefSdoCobId moved to sdoserv.c
 * driver access with macros
 *
 * Revision 2.10  2001/02/26 14:37:00  boe
 * documentation format changed
 * driver access functions replaced by macros
 *
 * Revision 2.9  2001/01/26 12:13:22  boe
 * split include files into function specific headers
 *
 * Revision 2.8  2001/01/17 16:21:01  boe
 * expand all implicite if tests and add type castings
 * add entries for MDPOs, emergency inhibittime
 * use PRODUCER/CONSUMER instead SERVER/CLIENT for Emergency
 *
 * Revision 2.6  2000/10/05 12:07:19  boe
 * defines for EMCY changed from CLIENT/SERVER to CONSUMER/PRODUCER
 * COM_PART_ALLOC/RELEASE modified, MPDO Modes added,
 * Inhibittime EMCY added, delete for error field changed
 *
 * Revision 2.5  2000/07/27 10:00:28  boe
 * pdo timer event counter moved from struct node_t to global variables
 *
 * Revision 2.4  2000/06/13 08:39:52  boe
 * function names (H_) changed
 *
 * Revision 2.3  2000/04/19 07:36:47  boe
 * parameter for multi line added
 *
 * Revision 2.2  2000/03/28 14:38:09  boe
 * adaption for multi-line version
 * sync producer mode changed
 *
 * Revision 2.1  2000/02/04 13:37:11  oe
 * - Changes for generating reference pages
 *
 * Revision 2.0  2000/01/21 11:04:15  boe
 * Überarbeitet und an Version 4.0 angepasst
 *
 *
 *
 *
 *------------------------------------------------------------------
 */


/****************************************************************************/
/**
*  \file set_com.c
*  \author port GmbH Halle (Saale)
*  $Revision$
*  $Date$
*
*++ This module contains functions for setting of internal communication
*++ variables of the CANopen library.
*++ Additionally it is possible to
*++ use it in applications for configuration of communication variables.
*-- Diese Modul beinhaltet Funktionen zum Setzen interner
*-- Kommunikationsvariablen.
*-- Zusätzlich it es möglich in der Initialisierungsphase
*-- von der Anwenderapplikation aus Kommunikationsvariablen
*-- zu manipulieren.
*/

/* header of standard C - libraries */

#include <stdio.h>
#include <string.h>

/* header of project specific types */

#include <cal_conf.h>
#include <co_odidx.h>
#include <co_cobid.h>
#include <co_mcpy.h>
#include <co_setcp.h>
#include <co_usr.h>
#include "access.h"
#include "nmt.h"
#include "nmt_s.h"
#include "nmterr.h"
#include "pdo.h"
#include "sdo.h"
#if defined(CONFIG_SYNC_PRODUCER) || defined(CONFIG_SYNC_CONSUMER)
# include "sync.h"
#endif /* defined(CONFIG_SYNC_PRODUCER) || defined(CONFIG_SYNC_CONSUMER) */
#include "emerg.h"
#include "cmscodec.h"
#if defined(CONFIG_TIME_PRODUCER) || defined(CONFIG_TIME_CONSUMER)
# include "time_lib.h"
#endif /* defined(CONFIG_TIME_PRODUCER) || defined(CONFIG_TIME_CONSUMER) */
#include "heartbt.h"
#include "drv.h"
#if defined(CONFIG_SRDO_CONSUMER) || defined(CONFIG_SRDO_PRODUCER)
# include "srdo.h"
#endif /* defined(CONFIG_SRDO_CONSUMER) || defined(CONFIG_SRDO_PRODUCER) */
#if defined(CONFIG_MASTER) || defined(CONFIG_SLAVE_PLUS)
# include "nmt_m.h"
#endif /* defined(CONFIG_MASTER) || defined(CONFIG_SLAVE_PLUS) */

#if defined(CONFIG_MPDO_DEST) || defined(CONFIG_MPDO_SRC)
# include "mpdo.h"
#endif /* defined(CONFIG_MPDO_DEST) || defined(CONFIG_MPDO_SRC) */

#ifdef CONFIG_FLYING_MASTER
# include "flyma.h"
#endif /* CONFIG_FLYING_MASTER */

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
RET_T setSdoCommPara(UNSIGNED16 index, UNSIGNED8 subIndex, UNSIGNED8 *pData,
    USER_T kind );
RET_T setPdoMapping(UNSIGNED16 index, UNSIGNED8 subIndex, UNSIGNED32 mapEntry
        );
RET_T setPdoCommPara(UNSIGNED16 index, UNSIGNED8 subIndex, UNSIGNED8 *pData,    
    UNSIGNED8 kind );
RET_T resetObjDirSubIndex(OBJDIR_T *pObj, UNSIGNED8 subIndex
    );

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
*++ \brief setCommPar - set the internal Communication Parameter
*-- \brief setCommPar - setzt die internen Kommunikationsparameter
*
*++ If a new communication parameter is set by users application
*++ this function have to be called e.g. after definition of a
*++ communication object a new parameter will be loaded from a
*++ non volatile memory.
*++ It is responsible for passing the communication parameter through to
*++ the internal variables.
*++ 
*++ \par example:
*
*-- Falls neue Kommunikationsparameter durch das Anwenderprogramm
*-- gesetzt werden, ist diese Funktion aufzurufen.
*-- Zum Beispiel nach der Definition eines Kommunikationsobjektes
*-- können neue Parameter aus einem nicht flüchtigen Speicher
*-- geladen werden.
*-- Die Funktion übergibt die Kommunikationsparameter an die internen
*-- Variablen.
*--
*-- \par Beispiel:
*
* \code
*  definePdo(RECEIVE_PDO, 2, CO_FALSE);           // define RPDO
*  cobId = readEeprom(pdo2cobid);                 // read new cobid from EEPROM
*  putObj(0x1401,1,&cobId,size,CO_TRUE);          // write back to Obj. Dic.
*  setCommPar(0x1401,1);                          // set value active
* \endcode
*
* \retval CO_OK
*++ success
*-- Erfolg
* \retval CO_E_NOT_EXIST
*++ internal communication object doesn't exist
*-- interne Kommunictionsvariable existiert nicht
* \retval CO_E_STATE
*++ node is in state OPERATIONAL
*-- Node ist im Zustand OPERATIONAL
* \retval CO_E_RANGE
*++ COB-ID is out of range (1..1760) for data services
*-- COB-ID ist nicht im Bereich von (1..1760) für Datendienste
* \retval CO_E_MAP
*++ dynamic mapping is not available
*-- dynamisches Mapping ist nicht verfügbar
* \retval CO_E_NO_INITIATE 
*++ service not available (not defined/compiled)
*-- Service nicht verfügbar (nicht definiert/Compiliert)
* \retval CO_E_NONEXIST_SUBINDEX
*++ subindex doesn't exist
*-- Subindex existiert nicht
* \retval CO_E_TRANS_TYPE
*++ bad transmission type
*-- falscher Transmission Type
* \retval CO_E_DATA_LENGTH
*++ to many bytes mapped
*-- Zu viele Bytes gemapped
* \retval CO_E_PARA_INCOMP
*++ parameter incompatible
*-- Parameter inkompatibel
*
*
*/

RET_T setCommPar(UNSIGNED16 index,    // index of communication object
                 UNSIGNED8  subIndex) // subindex of communication object
{
RET_T       ret = CO_OK; // return value
UNSIGNED32  size;        // size of object entry
UNSIGNED8   *pObj;       // pointer to object

  // get the address of new value
  if (getObjAddr(index, subIndex, &pObj, &size CO_COMMA_LINE_PARA) != CO_OK)
  {
    return CO_E_NOT_EXIST;
  }

  // PDO Parameter
#ifdef CONFIG_PDO_CONSUMER
  if ((index >= RPDO_PARA_BASE_INDEX) && (index <= RPDO_PARA_LAST_INDEX))
  {
    ret = setPdoCommPara(index, subIndex, pObj, RECEIVE_PDO CO_COMMA_LINE_PARA);
  }
  else if ((index >= RPDO_MAP_BASE_INDEX) && (index <= RPDO_MAP_LAST_INDEX))
  {
# ifdef CONFIG_DYN_PDO_MAPPING
    ret = setPdoMapping(index, subIndex, *((UNSIGNED32 *)pObj) CO_COMMA_LINE_PARA);
# else // CONFIG_DYN_PDO_MAPPING
    ret = CO_E_MAP;
# endif // CONFIG_DYN_PDO_MAPPING
  }
#endif // CONFIG_PDO_CONSUMER

#ifdef CONFIG_PDO_PRODUCER
  if ((index >= TPDO_PARA_BASE_INDEX) && (index <= TPDO_PARA_LAST_INDEX))
  {
    ret = setPdoCommPara(index, subIndex, pObj, TRANSMIT_PDO CO_COMMA_LINE_PARA);
  }
  else if ((index >= TPDO_MAP_BASE_INDEX) && (index <= TPDO_MAP_LAST_INDEX))
  {
# ifdef CONFIG_DYN_PDO_MAPPING
    ret = setPdoMapping(index, subIndex, *((UNSIGNED32 *)pObj) CO_COMMA_LINE_PARA);
# else // CONFIG_DYN_PDO_MAPPING
    ret = CO_E_MAP;
# endif // CONFIG_DYN_PDO_MAPPING
  }
#endif // CONFIG_PDO_PRODUCER

#if defined(CONFIG_PDO_PRODUCER) && defined(CONFIG_MPDO_SRC)
  else if ((index >= MPDO_SCANNER_LIST_INDEX) && (index <= MPDO_SCANNER_LIST_LAST))
  {
    if (testMPdoScannerEntry(index, subIndex CO_COMMA_LINE_PARA) < 0)
    {
      return(CO_E_MAP);
    }
  }
#endif // defined(CONFIG_PDO_PRODUCER) && defined(CONFIG_MPDO_SRC)

#if defined(CONFIG_PDO_CONSUMER) && defined(CONFIG_MPDO_SRC)
  else if ((index >= MPDO_DISP_LIST_INDEX) && (index <= MPDO_DISP_LIST_LAST))
  {
    if (testMPdoDispatchEntry(index, subIndex CO_COMMA_LINE_PARA) < 0)
    {
      return(CO_E_MAP);
    }
  }
#endif // defined(CONFIG_PDO_CONSUMER) && defined(CONFIG_MPDO_SRC)

#if defined(CONFIG_SDO_CLIENT) || defined(CONFIG_SDO_COB_ID)
  else if ((index >= SSDO_PARA_BASE_INDEX) && (index <= SSDO_PARA_LAST_INDEX))
  {

# ifdef CONFIG_SDO_COB_ID
# else // CONFIG_SDO_COB_ID
    // ignore first server sdo if no entry at od
    if (index != SSDO_PARA_BASE_INDEX)
# endif // CONFIG_SDO_COB_ID
    {
      ret = setSdoCommPara(index, subIndex, pObj, SERVER CO_COMMA_LINE_PARA);
    }
  }
  else if ((index >= CSDO_PARA_BASE_INDEX) && (index <= CSDO_PARA_LAST_INDEX))
  {
    ret = setSdoCommPara(index, subIndex, pObj, CLIENT CO_COMMA_LINE_PARA);
  }
#endif // defined(CONFIG_SDO_CLIENT) || defined(CONFIG_SDO_COB_ID)

#if defined(CONFIG_SRDO_CONSUMER) || defined(CONFIG_SRDO_PRODUCER)
  else if ((index >= SRDO_GFC) && (index <= SRDO_CONFIG_CHECKSUM))
  {
    ret = setSrdoData(index, subIndex, pObj CO_COMMA_LINE_PARA);
  }

#endif // defined(CONFIG_SRDO_CONSUMER) || defined(CONFIG_SRDO_PRODUCER)

  else
  {
    switch (index)
    {

#if defined(CONFIG_SYNC_PRODUCER) || defined(CONFIG_SYNC_CONSUMER)
      case SYNC_COB_ID_INDEX:
        ret = setSyncCobId((UNSIGNED32 *)pObj CO_COMMA_LINE_PARA);
        break;
#endif // defined(CONFIG_SYNC_PRODUCER) || defined(CONFIG_SYNC_CONSUMER)

#if defined(CONFIG_SYNC_PRODUCER)
      case COMM_CYCLE_INDEX:
        setSyncTimePara((UNSIGNED32 *)pObj CO_COMMA_LINE_PARA);
        break;
#endif // CONFIG_SYNC_PRODUCER

#if defined(CONFIG_TIME_CONSUMER) || defined(CONFIG_TIME_PRODUCER)
      case TIME_COB_ID_INDEX:
        ret = setTimeCobId((UNSIGNED32 *)pObj CO_COMMA_LINE_PARA);
        break;
#endif // defined(CONFIG_TIME_CONSUMER) || defined(CONFIG_TIME_PRODUCER)

#if defined(CONFIG_EMCY_PRODUCER)
      case EMCY_COB_ID_INDEX:
        ret = setEmcyCobId((UNSIGNED32 *)pObj CO_COMMA_LINE_PARA);
        break;

      case EMCY_INHIBIT_INDEX:
        co_pFirstEmcyProd->wInhibitTime = *((UNSIGNED16 *)pObj);
        break;

      case ERROR_FIELD_INDEX:
        // only 0 is allowed to write
        if (*((UNSIGNED8 *)pObj) != 0)
        {
          return(CO_E_TRANS_TYPE);
        }
        ret = eraseErr(0 CO_COMMA_LINE_PARA);
        break;
#endif // defined(CONFIG_EMCY_PRODUCER)

#if defined(CONFIG_NODE_GUARDING)
      case GUARD_TIME_INDEX:
        // we need additional the life time factor
        {
          UNSIGNED8   *pFaktor;

          if ((ret = getObjAddr(LIFE_TIME_FAC_INDEX, 0, &pFaktor, &size CO_COMMA_LINE_PARA)) == CO_OK)
          {
            ret = setLifeTime((UNSIGNED16 *)pObj, pFaktor CO_COMMA_LINE_PARA);
          }
        }
        break;

      case LIFE_TIME_FAC_INDEX:
        // we need additional the guard time
        {
          UNSIGNED8   *pGuardTime;
          if ((ret = getObjAddr(GUARD_TIME_INDEX, 0, &pGuardTime, &size CO_COMMA_LINE_PARA)) == CO_OK)
          {
            ret = setLifeTime((UNSIGNED16 *)pGuardTime, pObj CO_COMMA_LINE_PARA);
          }
        }
        break;
#endif // CONFIG_NODE_GUARDING

#ifdef CONFIG_HEARTBEAT_PRODUCER
      case HEARTBEAT_PROD_INDEX:
        setHeartBeatProducerTime(*((UNSIGNED16 *)pObj) CO_COMMA_LINE_PARA);
        break;
#endif // CONFIG_HEARTBEAT_PRODUCER

#ifdef CONFIG_HEARTBEAT_CONSUMER
      case HEARTBEAT_CON_INDEX:
        ret = setHeartBeatConsumerTime(*((UNSIGNED32 *)pObj), subIndex CO_COMMA_LINE_PARA);
        break;
#endif // CONFIG_HEARTBEAT_CONSUMER

#if defined(CONFIG_EMCY_CONSUMER)
      case EMCY_CONSUMER_INDEX:
        // abort for extended identifiers
        if ((*pObj & CAN_29_BIT_ID_FLAG) != 0)
        {
          return(CO_E_TRANS_TYPE);
        }
        break;
#endif // defined(CONFIG_EMCY_CONSUMER)

#ifdef CONFIG_FLYING_MASTER
      case FLYMANAGER_TIMEPAR:
        ret = setFlymaTimePara(subIndex, (UNSIGNED16)tmpU32 CO_COMMA_LINE_PARA);
        break;

      case FLYMANAGER_CAPDEVPAR:
        ret = setFlymaDevPara(subIndex, (UNSIGNED16)tmpU32 CO_COMMA_LINE_PARA);
        break;
#endif // CONFIG_FLYING_MASTER
    }
  }
  return ret;
}


#if defined(CONFIG_PDO_CONSUMER) || defined(CONFIG_PDO_PRODUCER)
/*******************************************************************/
/*
* setPdoCommPar - set pdo communication parameter
*
* NOMANUAL
*
* This function sets communication parameter for pdos
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
RET_T setPdoCommPara(
    UNSIGNED16  index,      /* index */
    UNSIGNED8   subIndex,   /* subindex */
    UNSIGNED8   *pData,     /* pointer to object data */
    UNSIGNED8   kind        /* kind of PDO RECEIVE/TRANSMIT */
    )
{
UNSIGNED16  nr;         /* pdo number */
PDO_T       *pPdo;          /* pointer to actual pdo */
RET_T       ret = CO_E_NOT_EXIST;   /* return value */

    /* get pdo structure */
    if (kind == RECEIVE_PDO)  {
    nr = index - RPDO_PARA_BASE_INDEX + 1;
    if ((pPdo = pdoExist(nr, RECEIVE_PDO CO_COMMA_LINE_PARA)) == NULL)  {
         return(CO_E_NOT_EXIST);
    }
    } else {
    nr = index - TPDO_PARA_BASE_INDEX + 1;
    if ((pPdo = pdoExist(nr, TRANSMIT_PDO CO_COMMA_LINE_PARA)) == NULL)  {
         return(CO_E_NOT_EXIST);
    }
    }

    switch (subIndex) {
    case 1:             /* cob-id */
        ret = setPdoCobId(pPdo, *((UNSIGNED32 *)pData));
        break;

    case 2:             /* transmission type */
        ret = setPdoTransType(pPdo, kind, *pData CO_COMMA_LINE_PARA);
        break;

    case 3:             /* inhibit time */
        ret = setPdoInhibitTime(pPdo, *((UNSIGNED16 *)pData));
        break;
    
    case 4:             /* compatibility entry */
        ret = CO_E_NONEXIST_SUBINDEX;
        break;

# ifdef CONFIG_PDO_EVENTTIMER
    case 5:             /* event timer */
        ret = setPdoEventTime(pPdo, kind, *((UNSIGNED16 *)pData)
        CO_COMMA_LINE_PARA);
        break;
# endif /* CONFIG_PDO_EVENTTIMER */
    }
    return(ret);
}
#endif /* defined(CONFIG_PDO_CONSUMER) || defined(CONFIG_PDO_PRODUCER) */


#if defined(CONFIG_SDO_CLIENT) || defined(CONFIG_SDO_SERVER)
/*******************************************************************/
/*
* setSdoCommPar - set sdo communication parameter
*
* NOMANUAL
*
* This function sets communication parameter for sdos
*
* \retval CO_OK
*++ success
*-- Erfolg
* \retval CO_E_NOT_EXIST
* internal communication object doesn't exist
* \retval CO_E_RANGE
* COB-ID is out of the range
* \retval CO_E_TRANS_TYPE
* bad transtype
*
*/
RET_T setSdoCommPara(
    UNSIGNED16  index,      /* index */
    UNSIGNED8   subIndex,   /* subindex */
    UNSIGNED8   *pData,     /* pointer to object data */
    USER_T      kind        /* kind of SDO SERVER/CLIENT */
    )
{
UNSIGNED8   nr;         /* pdo number */
SDO_T       *pSdo;          /* pointer to actual pdo */
RET_T       ret = CO_E_NOT_EXIST;   /* return value */

    /* get sdo structure */
    nr = (UNSIGNED8)(index & 0x7f) + 1;
    if ((pSdo = CMS_DomExist(nr, kind CO_COMMA_LINE_PARA)) == NULL)  {
     return(CO_E_NOT_EXIST);
    }

    switch (subIndex) {
    case 1:             /* cob-id */
    case 2:             /* cob-id */
        ret = setSdoCobId(pSdo, *((UNSIGNED32 *)pData), subIndex);
        break;
    }

    return(ret);
}
#endif /* defined(CONFIG_SDO_CLIENT) || defined(CONFIG_SDO_SERVER) */


#if defined(CONFIG_PDO_CONSUMER) || defined(CONFIG_PDO_PRODUCER)
# ifdef CONFIG_DYN_PDO_MAPPING
/*******************************************************************
*
* setPdoMapping - sets the new Mapping
*
* NOMANUAL
*
* \retval CO_OK
*++ success
*-- Erfolg
* \retval CO_E_NOT_EXIST
* internal communication object doesn't exist
* \retval CO_E_MAP
* mapping not allowed, variable has no read or write access
* \retval CO_E_DEVICE_STATE
* mapping not changable in the current state (mapping not disabled)
*
* INTERNAL
* the consistency of variables is ensured by the function setCommPar,
* which calls setPdoMapping
*/

RET_T setPdoMapping(
      UNSIGNED16 index,    /* index of mapping entry */
      UNSIGNED8  subIndex, /* subindex of mapping entry */
      UNSIGNED32 mapEntry  /* new mapping entry */
      )
{
UNSIGNED32  size;           /* size var */
UNSIGNED8   count;          /* count of valid mapping objects*/
#  if defined(CONFIG_BIG_ENDIAN) || defined(CONFIG_16BIT_CPU)
UNSIGNED32  tmpU32;    /* temporary variable */
#  endif /* defined(CONFIG_BIG_ENDIAN) || defined(CONFIG_16BIT_CPU) */

    /* subindex 0 contains number of mapped objects */
    if (subIndex == 0) {
#  if defined(CONFIG_BIG_ENDIAN) || defined(CONFIG_16BIT_CPU)
    /* subindex 0 is an 8 bit value - but it comes as 32 bit value */
    tmpU32 = mapEntry;
    mapEntry = (*((UNSIGNED8 *)&tmpU32)) & 0xff;
#  endif /* defined(CONFIG_BIG_ENDIAN) || defined(CONFIG_16BIT_CPU) */

    /* set new mapping count - check the complete mapping table
        (incl. length for all mapping entries) */
    if (checkMappingTable(index CO_COMMA_LINE_PARA) != CO_OK)  {
        return(CO_E_MAP);
    }

    return(CO_OK);
    }

    /* subindex > 0 - someone is changing a mapping entry */
    /* it is only allowed if subindex 0 = 0 */
    if (getObjEntry(index, 0, &count, &size, CO_TRUE CO_COMMA_LINE_PARA)
        != CO_OK)  {
    return(CO_E_NOT_EXIST);
    }
    if (count != 0)  {
    /* CO_E_MAP should be the best error code,
     * but isnt accepted by conformance test */
    /* return(CO_E_MAP); */
    return(CO_E_DEVICE_STATE);
    }

    /* check if the mapping entry is valid */
    if (checkMappingEntry(index, mapEntry CO_COMMA_LINE_PARA) != CO_OK)  {
    return(CO_E_MAP);
    }

    return(CO_OK);
}
# endif /* CONFIG_DYN_PDO_MAPPING */
#endif /* defined(CONFIG_PDO_CONSUMER) || defined(CONFIG_PDO_PRODUCER) */

#ifdef CONFIG_HEARTBEAT_CONSUMER
/*******************************************************************/
/*
* setHeartBeatConsumerTime - set heartbeat consumer time
*
* NOMANUAL
*
* This service change an entry from hb consumer list
* and updates the internal network structures
* All heartbeat consumers must be saved at the od
* at the function 
* createNetworkReq()
* All entries from this od list are added to the network structure
* each hb consumer entry corresponses by the network structure
*
* \retval
* RET_T
*
*/
RET_T setHeartBeatConsumerTime(
    UNSIGNED32  hbEntry,    /* entry from od hb-consumer list */
    UNSIGNED8   subIndex    /* subindex for this entry */
    )
{
UNSIGNED8   nodeId;         /* node id */
UNSIGNED8   i;          /* loop counter */
NODE_T      *pNodeInUse;        /* pointer to node structure */
UNSIGNED16  hbTime;         /* heartbeat time */
RET_T       ret = CO_OK;        /* returnvalue */

    nodeId = (UNSIGNED8)(hbEntry >> 16) & 0x7f;
    hbTime = (UNSIGNED16)(hbEntry & 0xffff);

    if (nodeId != 0)  {
    /* check, if the node id isn't set twice */
    pNodeInUse = co_pNetwork ;
    i = 0;
    while (pNodeInUse != NULL)  {
        if (pNodeInUse->bNode_ID == nodeId)  {
        i++;
        }
        pNodeInUse = pNodeInUse->pNext;
    }
    if (i > 1)  {
        return(CO_E_PARA_INCOMP);
    }
    }

    /* get the correspondenting network structure */
    /* the first network structures corresponding with hb-entries at od */
    pNodeInUse = co_pNetwork ;
    i = subIndex - 1;
    while (i != 0)  {
    pNodeInUse = pNodeInUse->pNext;
    if (pNodeInUse == NULL) {
        return CO_E_NOT_EXIST;
    }
    i--;
    }

    /* now we have the right structure */
    /* if heartbeat time = 0 or node = 0 set it to invalid */
    if ((nodeId == 0) || (hbTime == 0))  {
    /* stop the timer */
    removeTimerEvent(&pNodeInUse->timer CO_COMMA_LINE_PARA);
    /* reset node-id */
    pNodeInUse->bNode_ID = 0;
    /* remove active flag */
    pNodeInUse->mflags &= ~GUARDFLAG_HB_ACTIVE;

    return(CO_OK);
    }

    /* set the new hb monitoring time */
    pNodeInUse->timer.timerVal = (UNSIGNED32)hbTime * 10;

    /* change only the time for this node ? */
    if (nodeId == pNodeInUse->bNode_ID)  {
    return(CO_OK);
    }

    pNodeInUse->bNode_ID = nodeId;

    /* now we have to change the node-id */
    SET_COB_ID(pNodeInUse->pGuard_COB, CO_COBID_NMTERR + (UNSIGNED16)nodeId);

    return(ret);
}
#endif /* CONFIG_HEARTBEAT_CONSUMER */

/* #if defined(CONFIG_SLAVE) */
/*******************************************************************
*
* resetComStates - resets all internal communication variables
*
* NOMANUAL
*
* This service resets all internal communication variables and
* clears the TX/RX buffer.
* It is used for the reset communication service.
*
* RETURNS
* .TP
* nothing
*
*/

void resetComStates(
    void
     )
{
    /* clear TX/RX buffer */
    CLEAR_TX_BUFFER(CO_LINE_PARA);
    CLEAR_RX_BUFFER(CO_LINE_PARA);
# if defined(CONFIG_NODE_GUARDING)
    co_pNode  ->flags &= ~(GUARDFLAG_NG_ACTIVE
            + GUARDFLAG_NG_RECEIVED);
/*
GUARDFLAG_NG_LIFETIME + GUARDFLAG_HB_ACTIVE
*/
    co_pNode  ->bSuspendedGuardings = 0;
    /* toggled to 1 after first guarding */
    co_pNode  ->bGuardToggle = 0;
# endif /* CONFIG_NODE_GUARDING */
}
/* #endif */ /* defined(CONFIG_SLAVE) */


/*******************************************************************
*
*++ resetObjDir - sets the variables of the object dictionary to their defaults
*-- resetObjDir - setzt die Variables des Obj. Ver. auf ihre Standardwerte
*
* NOMANUAL
*
*++ This function sets writeable variables of the object dictionary
*++ to their defaults.
*-- Diese Funktion erlaubt das Rücksetzen von schreibbaren Objekten
*-- im Objektverzeichnis auf ihre Standardwerte.
*
*++ Range may be:
*-- Der angegebene Bereich \fIrange\fP kann sein:
*
* .TP
* RESET_ALL
*++ reset the whole object dictionary
*-- Rücksetzen des gesamten Objektverzeichnisses
* .TP
* RESET_COM
*++ reset only the communication part of the object dictionary
*-- Nur der kommunikationsspezifische Teil des Objektverzeichnisses
*-- wird rückgesetzt (Kommunikationsprofil)
* .TP
* RESET_NO_COM
*++ reset the whole object dictionary besides the communication part
*-- Das gesamte Objektverzeichnis ausschließlich des
*-- kommunikationsspezifische Teiles wird rückgesetzt
*
*++ 1) The bit_rate is a manufacturer specific extension
*++ provided by \fIport\fP GmbH
*++ for changing the CAN bit rate via SDO.
*-- 1) \fIbit_rate\fP ist eine hersteller spezifische Erweiterung
*-- von \fIport\fP GmbH und dient dem Ändern der Bustaktfrequenz
*-- über einen SDO Transfer.
*
* \retval CO_OK
*++ success
*-- Erfolg
* \retval CO_E_RANGE
*++ parameter out of valid range
*-- Parameter \fIrange\fP nicht gültig
*
*/

RET_T resetObjDir(
      UNSIGNED8 range     /* range of the object dictionary */
      )
{
UNSIGNED16  i;      /* variable for access to Object Dictionary*/
UNSIGNED8   subIndex;   /* sub index */
UNSIGNED16  min;        /* lowest valid index */
UNSIGNED16  max;        /* higest valid index */
OBJDIR_T *pObj;


    /* setting of range */
    switch(range) {
    case MEM_SEG_ALL_PARAMETERS:
        min = START_OBJ_DIC;
        max = END_OBJ_DIC;
        break;
    case MEM_SEG_COM_PARAMETERS:
        min = START_COM_PROF;
        max = END_COM_PROF;
        break;
    case MEM_SEG_APPL_PARAMETERS:
        min = START_MANU_PROF;
        max = END_OBJ_DIC;
        break;
    default:
        return(CO_E_RANGE);
    }

    /* allocate security mechanism for object dictionary consistency */
    CO_COM_PART_ALLOC(CO_LINE_PARA);
    CO_APPL_PART_ALLOC(CO_LINE_PARA);

    /* pointer to first object */
    pObj = &objDir[0];
 
    /* for all entries at object dictionary */
    i = 0;
    while (i < maxObjDicElements )  {

    /* if this index in the range */
    if ((pObj->index >= min) && (pObj->index <= max)) {
        /* yes, it is */

        /* ignore no numerical values */
        if ((pObj->pValDesc[0].attribute & CO_NUM_VAL) != 0) {

        for (subIndex = 0; subIndex < pObj->numOfElem; subIndex++) {

CO_WATCH_DOG
            /* set subindex */
            resetObjDirSubIndex(pObj, subIndex CO_COMMA_LINE_PARA);
        }
        }
    }

    pObj++;     /* next entry */
    i++;            /* incr counter */
    }

    /* release security mechanism for object dictionary consistency*/
    CO_COM_PART_RELEASE(CO_LINE_PARA);
    CO_APPL_PART_RELEASE(CO_LINE_PARA);

    return(CO_OK);
}


/*******************************************************************
*
*++ resetObjDirSubIndex - sets one entry to their defaults
*-- resetObjDirSubIndex - setzt eine Variables auf ihre Standardwerte
*
* NOMANUAL
*
*++ This function sets one variables of the object dictionary
*++ to their defaults.
*-- Diese Funktion erlaubt das Rücksetzen von einem Subindex
*-- im Objektverzeichnis auf ihre Standardwerte.
*
* RETURNS
* .TP
* \&OK
*++ success
*-- Erfolg
* .TP
* \&E_RANGE
*++ parameter out of valid range
*-- Parameter \fIrange\fP nicht gültig
*
*/
RET_T resetObjDirSubIndex(OBJDIR_T  *pObj,    // pointer to actual od entry
                          UNSIGNED8 subIndex) // subindex
{
UNSIGNED8  subIndexDesc; // subindex for object description
INTEGER8   size;         // byte size
UNSIGNED8  *pTargetAddr, // target address
           *pSourceAddr; // source address
UNSIGNED32 cobId;        // cob-id predefined connection

  // test for short arrays
  if ((pObj->pValDesc[0].attribute & CO_SHORT_ARRAY_DESC) != 0)
  {
    subIndexDesc = 1;
  }
  else
  {
    subIndexDesc = subIndex;
  }

  // only writeable and numerical variables are allowed
  if ((pObj->pValDesc[subIndexDesc].attribute & (CO_NUM_VAL)) == 0)
  {
    return(CO_E_RANGE);
  }

  // get object size
  size = pObj->pValDesc[subIndexDesc].size;
  // higest bit is sign for signed or unsigned element
  if (size < 0)
  {
    size = -size;
  }

  // get address of subindex
  pTargetAddr = getSubIndexAddr(pObj, subIndex);

  // copy default values to variables
  pSourceAddr = (UNSIGNED8 *)&(pObj->pValDesc[subIndexDesc].defaultVal);

  // set cobs for predefined communication set ----------------------------

  if (pObj->index < SYNC_COB_ID_INDEX)
  {
    // use standard values from od
  }

#if defined(CONFIG_SYNC_COB_ID) && (defined(CONFIG_SYNC_CONSUMER) || defined(CONFIG_SYNC_PRODUCER))
  // set SYNC default COB-ID
  else if (pObj->index == SYNC_COB_ID_INDEX)
  {
    pSourceAddr = (UNSIGNED8 *)&cobId;
    cobId = CO_COBID_SYNC;
# ifdef CONFIG_SYNC_PRODUCER
    // cobId = CO_COBID_SYNC | SYNC_PRODUCER_BIT;
# endif // CONFIG_SYNC_PRODUCER
# ifdef CONFIG_SYNC_CONSUMER
    // cobId = CO_COBID_SYNC | SYNC_CONSUMER_BIT;
# endif // CONFIG_SYNC_CONSUMER
  }
#endif // (defined(CONFIG_SYNC_CONSUMER) || defined(CONFIG_SYNC_PRODUCER))

  else if (pObj->index < TIME_COB_ID_INDEX)
  {
    // use standard values from od
  }

#if defined(CONFIG_TIME_COB_ID) && (defined(CONFIG_TIME_CONSUMER) || defined(CONFIG_TIME_PRODUCER))
  // set TIME stamp default COB-ID
  else if (pObj->index == TIME_COB_ID_INDEX)
  {
    pSourceAddr = (UNSIGNED8 *)&cobId;
# ifdef CONFIG_TIME_PRODUCER
    cobId = CO_COBID_TIME | TIME_PRODUCER_BIT;
# endif // CONFIG_TIME_PRODUCER
# ifdef CONFIG_TIME_CONSUMER
    cobId = CO_COBID_TIME | TIME_CONSUMER_BIT;
# endif // CONFIG_TIME_CONSUMER
  }
#endif // (defined(CONFIG_TIME_CONSUMER) || defined(CONFIG_TIME_PRODUCER))

  else if (pObj->index < EMCY_COB_ID_INDEX)
  {
    // use standard values from od
  }

#if defined(CONFIG_EMCY_PRODUCER)
  else if (pObj->index == EMCY_COB_ID_INDEX)
  {
    pSourceAddr = (UNSIGNED8 *)&cobId;
    // set EMCY default COB-ID
    cobId = CO_COBID_EMCY + (UNSIGNED32)coNodeId ;
  }
#endif // defined(CONFIG_EMCY_PRODUCER)

  else if (pObj->index < SSDO_PARA_BASE_INDEX)
  {
    // use standard values from od
  }

#ifdef CONFIG_SDO_COB_ID
  // set SDO default COB-IDs
  else if (pObj->index == SSDO_PARA_BASE_INDEX)
  {
    pSourceAddr = (UNSIGNED8 *)&cobId;
    if (subIndex == 1)
    {
      cobId = CO_COBID_CSDO + (UNSIGNED32)coNodeId;
    }
    if (subIndex == 2)
    {
      cobId = CO_COBID_SSDO + (UNSIGNED32)coNodeId;
    }
  }
#endif // CONFIG_SDO_COB_ID

  else if (pObj->index <= CSDO_PARA_LAST_INDEX)
  {
    // disable all SDO besides SSDO 1
    if ((subIndex == 1) || (subIndex == 2))
    {
      pSourceAddr = (UNSIGNED8 *)&cobId;
      cobId = SDO_NO_VALID_BIT;
    }
  }
  else if (pObj->index < SRDO_PARA_BASE_INDEX)
  {
    // use standard values from od
  }

#if defined(CONFIG_SRDO_PRODUCER) || defined(CONFIG_SRDO_CONSUMER)
  else if (pObj->index <= SRDO_PARA_LAST_INDEX)
  {
    if ((subIndex == 5) || (subIndex == 6))
    {
      pSourceAddr = (UNSIGNED8 *)&cobId;

      // only first srdo has predefined cob
      if (pObj->index == SRDO_PARA_BASE_INDEX)
      {
        if (subIndex == 5)
        {
          cobId = 0xff + 2 * (UNSIGNED32)coNodeId;
        }
        else
        {
          cobId = 0x100 + 2 * (UNSIGNED32)coNodeId;
        }
      }
      else
      {
        cobId = SRDO_NO_VALID_BIT;
      }
    }
  }
  else if (pObj->index <= SRDO_CONFIG_CHECKSUM)
  {
    ;
  }
#endif // defined(CONFIG_SRDO_PRODUCER) || defined(CONFIG_SRDO_CONSUMER)

#ifdef CONFIG_PDO_CONSUMER
  else if (pObj->index <= RPDO_PARA_LAST_INDEX)
  {
    if (subIndex == 1)
    {
      pSourceAddr = (UNSIGNED8 *)&cobId;
      // only first 4 pdo have predefined cob
      if ((pObj->index & 0x1ff) < 4)
      {
        cobId = CO_COBID_RPDO1 + (pObj->index & 0x7) * 0x100 + (UNSIGNED32)coNodeId;
      }
      else
      {
        cobId = PDO_NO_VALID_BIT;
      }
    }
  }
#endif // CONFIG_PDO_CONSUMER

  else if (pObj->index < TPDO_PARA_BASE_INDEX)
  {
    // use standard values from od
  }

#ifdef CONFIG_PDO_PRODUCER
  else if (pObj->index <= TPDO_PARA_LAST_INDEX)
  {
    if (subIndex == 1)
    {
      pSourceAddr = (UNSIGNED8 *)&cobId;
      // only first 4 pdo have predefined cob
      if ((pObj->index & 0x1ff) < 4)
      {
        cobId = CO_COBID_TPDO1 + (pObj->index & 0x1ff) * 0x100 + (UNSIGNED32)coNodeId;
      }
      else
      {
        cobId = PDO_NO_VALID_BIT;
      }
    }
  }
#endif // CONFIG_PDO_PRODUCER

  // save new value at od
#ifdef CONFIG_BIG_ENDIAN
  // defaults are saved as 4 byte values - size < 4 are special case
  CO_NUM_MEMCPY(pTargetAddr, (void *)(pSourceAddr + (4 - size)), size, CO_TRUE);
#else // CONFIG_BIG_ENDIAN
  CO_NUM_MEMCPY(pTargetAddr, pSourceAddr, size, CO_TRUE);
#endif // CONFIG_BIG_ENDIAN

  return(CO_OK);
}


/*******************************************************************
*
* setDefaultParameter - set default parameter to node-id and od entries
*
* NOMANUAL
*
* This function set the default values to 
* - node-id
* - all entries at object dictionary
*
* \retval
* nothing
*/
void setDefaultParameter(
    UNSIGNED8 range     /* range of the object dictionary */
    )
{
#ifdef CONFIG_SDO_SERVER
SDO_T       *pSdo;      /* pointer to sdo */
UNSIGNED16  cobId;      /* cob-id */
#endif /* CONFIG_SDO_SERVER */

    /* get node id except user parameter */
    if (range != MEM_SEG_APPL_PARAMETERS)  {

    /* calls user function to get the node id from e.g. DIP switch */
    coNodeId  = getNodeId(CO_LINE_PARA);

    /* set new node id internal */
    co_pNode  ->bNode_ID =
            coNodeId ;

#ifdef CONFIG_SDO_SERVER
    /* update first server sdo cob-ids */
    /* get sdo structure */
    if ((pSdo = CMS_DomExist(1, SERVER CO_COMMA_LINE_PARA)) != NULL) {
        cobId = CO_COBID_CSDO + coNodeId ;
        SET_COB_ID(pSdo->pReqInd_COB, cobId);

        cobId = CO_COBID_SSDO + coNodeId ;
        SET_COB_ID(pSdo->pResCon_COB, cobId);
        pSdo->state = SDOSTATE_READY;
    }
#endif /* CONFIG_SDO_SERVER */

    }

    /* load default communication values to od ------------------------- */
    resetObjDir(range CO_COMMA_LINE_PARA);
}

/*______________________________________________________________________EOF_*/
