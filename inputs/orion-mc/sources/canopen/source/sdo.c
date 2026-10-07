/*
 *++ sdo - contains SDOs service functions
 *-- sdo - beinhaltet Servicefunktionen für SDOs 
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
 * Revision 1.0  2008-03-07 17:04:46+01  driet
 * Initial revision
 *
 * Revision 2.23  2003/10/01 13:27:03  boe
 * delete unused line parameter for setSdoCobId
 *
 * Revision 2.22  2003/07/22 07:22:01  boe
 * change comments
 *
 * Revision 2.21  2003/06/13 14:03:51  boe
 * add include for block transfer
 *
 * Revision 2.20  2002/12/11 08:09:58  boe
 * use as default at defineSdo cob entries from od
 *
 * Revision 2.19  2002/11/18 09:36:15  boe
 * add comments/adapt on doxygen
 * add co_ to all global library variables
 * get more exact cob-type to driver
 *
 * Revision 2.18  2002/05/30 09:00:09  hae
 * documentation correction
 *
 * Revision 2.17  2002/05/21 14:27:46  boe
 * copyright changed
 *
 * Revision 2.16  2002/03/26 08:16:04  boe
 * add NEW_TIMER functionality over defines
 *
 * Revision 2.15  2001/12/05 15:42:55  boe
 * add define to enable debug messages
 * add abort sdo transfer for read and write permission
 *
 * Revision 2.14  2001/06/19 15:22:57  boe
 * add additionally abort codes
 *
 * Revision 2.13  2001/05/17 09:39:01  boe
 * explicite type conversion to remove compiler warnings
 *
 * Revision 2.12  2001/04/05 08:45:50  boe
 * comment changed
 *
 * Revision 2.11  2001/03/29 14:32:15  boe
 * comment changed
 *
 * Revision 2.10  2001/03/28 12:56:59  boe
 * unused variable removed for BIG_ENDIAN
 *
 * Revision 2.9  2001/02/26 14:56:20  boe
 * driver access functions replaced by macros
 * documentation format changed
 *
 * Revision 2.8  2001/01/26 11:16:56  boe
 * split include files into function specific headers
 * move function abortSdoTransf_Req() to this file
 *
 * Revision 2.7  2001/01/17 16:18:34  boe
 * expand all implicite if tests and add type castings
 * allow only one server sdo without entry at the od
 *
 * Revision 2.6  2000/10/05 12:03:38  boe
 * memcpy replaced by CO_MEMCPY
 * COM_PART_ALLOC/RELEASE modified
 *
 * Revision 2.5  2000/08/02 07:15:54  boe
 * define swapped data buffer at writeSdoReq() as static
 *
 * Revision 2.4  2000/06/21 15:07:11  boe
 * writeSdoReq for big endian changed
 *
 * Revision 2.3  2000/06/13 08:38:08  boe
 * function names (H_) changed
 * adaption for blocktransfer modes
 *
 * Revision 2.2  2000/03/28 14:36:54  boe
 * adaption for multi-line version
 * defineSdo changed
 *
 * Revision 2.1  2000/02/04 13:33:08  oe
 * - Changes for generating reference pages
 *
 * Revision 2.0  2000/01/21 11:04:02  boe
 * Überarbeitet und an Version 4.0 angepasst
 *
 *
 *
 *------------------------------------------------------------------
 */


/****************************************************************************/
/**
*  \file sdo.c
*  \author port GmbH Halle (Saale)
*  $Revision$
*  $Date$
*
*++ This module contains the functions for handling the
*++ Service Data Objects (SDO).
*++ Each SDO has to be defined by the
*++ \em defineSdo ()
*++ service.
*++ The handling of server SDOs is hidden in the library.
*++ For client SDO usage the functions
*++ \em writeSdoReq()
*++ and
*++ \em readSdoReq()
*++ are defined in this module.
*-- Dieses Modul enthält Funktionen für Service Daten Objekte (SDO).
*-- Jedes SDO ist mit der Funktion
*-- \em defineSdo() anzulegen.
*-- Die Abarbeitung der Dienste für Server-SDOs ist transparent
*-- für den Anwender.
*-- Zur Nutzung von Client-SDOs stehen die Dienste
*-- \em writeSdoReq()
*-- und
*-- \em readSdoReq()
*-- zur Verfügung.
* \par
*++ The CANopen Library supports up to 128 client
*++ and 128 server SDOs.
*-- Von der CANopen Library werden bis zu 128 Client-
*-- und 128 Server-SDOs unterstützt.
* \par
*++ The CANopen Library by \em port supports the down- and upload
*++ of domains e.g. programs up to a size of 4 294 967 295 bytes.
*-- Die CANopen Library von \em port unterstützt das Herunter- und Hinaufladen
*-- von Domains z.B Programmen bis zu einer Größe von 4 294 967 295 Bytes.
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
#include <co_mcpy.h>
#include <co_debug.h>
#include "sdo.h"
#include "nmt.h"
#include "access.h"
#include "cmscodec.h"
#include "drv.h"

#ifdef CONFIG_SDO_BLOCKTRANSFER
# include "sdoblock.h"
#endif /* CONFIG_SDO_BLOCKTRANSFER */

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
CMS_DOMAIN_T       *co_pFirstDomEntry    ;

/* local defined variables
---------------------------------------------------------------------------*/
#ifdef CONFIG_RCS_IDENT
static char _rcsid[] = "$Id$";
#endif /* CONFIG_RCS_IDENT */



#if defined(CONFIG_SDO_SERVER) || defined(CONFIG_SDO_CLIENT)
/****************************************************************************/
/**
*++ \brief defineSdo - defines a SDO
*-- \brief defineSdo - definiert ein SDO
*
*++ This function defines a Service Data Objetc SDO.
*++ The user has to ensure that the number of SDO is unique
*++ between 1...128.
*-- Diese Funktion definiert ein SDO Objekt:
*-- Der Anwender sorgt für eine eindeutige Nummernvergabe von 1..128.
*
*++ \em kindOfUse can be \c CLIENT or \c SERVER
*-- \em kindOfUse kann \c CLIENT oder \c SERVER sein
*
*++ For the RSDO/TSDO pair of the first Server-SDO the resulting COB-IDs will be
*++ computed from the Node ID according to DS301.
*-- Die COB-IDs für das RSDO/TSDO Paar der ersten Server-SDO werden über die
*-- Node-ID gemäß DS301 berechnet.
* \code
* 1st  Receive  SDO     1536 + node ID
* 1st  Transmit SDO     1408 + node ID
* \endcode
*++ All other SDO COB-IDs are set to ( 0x80000000 | 1760) = 0x80006E0 ).
*++ After 'defining' they are disabled.
*++ To change their COB-ID the object dictionary must be modified
*++ and
*++ \em setCommPar()
*++ has to be called in order to set the internal values.
*-- Alle anderen SDOs erhalten eine COB-ID von
*-- (0x80000000 | 1760) = 0x80006E0.
*-- Nach der 'Definition' sind sie zunächst deaktiviert.
*-- Um Ihre COB-ID zu ändern muß der Objektverzeichniseintrag gändert
*-- werden.
*-- Durch Aufruf von
*-- \em setCommPar()
*-- werden die internen Werte gültig gesetzt.
*
* \code
* defineSdo(2, CLIENT);                // define SDO 2 as client SDO
* cobId = 1200;                        // new COB-ID
* putObj(0x1281, 1, &cobId, 4, CO_TRUE);  // set new Value to OD
* cobId = 1201;                        // new COB-ID
* putObj(0x1281, 2, &cobId, 4, CO_TRUE);  // set new Value to OD
* setCommPar(0x1281, 1);               // set internal values
* setCommPar(0x1281, 2);               // set internal values
* \endcode
*
*++ \par Notice:
*-- \par Hinweis:
*
*++ If only one Server-SDO should be available at the device, it is not
*++ necessary to have an object dictionary entry for the SDOs.
*++ In such a case don't set the compiler directive CONFIG_SDO_COB_ID.
*-- Falls nur eine Server-SDO auf dem Gerät implementiert werden soll, ist
*-- es nicht notwendig Einträge für die SDOs im Objektverzeichnis zu haben.
*-- In diesem Falle ist die Compilerdirektive CONFIG_SDO_COB_ID nicht zu setzen.
*
* \retval CO_OK
*++ success
*-- Erfolg
* \retval CO_E_MEM
*++ memory allocation fault
*-- Speicherallozierungsfehler
* \retval CO_E_NO_ACCESS
*++ no access to object dictionary (COB-ID SDO or Node-ID)
*-- Zugriffsfehler auf das Objektverzeichnis (COB-ID SDO or Node-ID)
* \retval CO_E_TRANS_TYPE
*++ bad transmission type requested
*-- nicht compilierter Transmission Type angefordert
*
*/

RET_T defineSdo(
      UNSIGNED8 sdoNr,    /**< number of SDO	 */
      USER_T kindOfUse    /**< kind of using the SDO */
      )
{
UNSIGNED32	cobIdReq;	/* COB-ID for Request/Indication*/
UNSIGNED32	cobIdRes;	/* COB-ID for Response/Confirm */
COB_KIND_T	C_COB_kind,	/* cob-id structure client */
		S_COB_kind;	/* cob-id structure server */
# if defined(CONFIG_SDO_COB_ID) || defined(CONFIG_SDO_CLIENT)
UNSIGNED16    baseIndex = 0;   /* base for client or server SDO index */
CMS_DOMAIN_T	*pLastDomEntry = NULL;	/* pointer to sdo structure entry */
UNSIGNED32	size;		/* size of object */
# endif /* defined(CONFIG_SDO_COB_ID) || defined(CONFIG_SDO_CLIENT) */
CMS_DOMAIN_T	*pDomInUse;	/* pointer to actual sdo structure */

    /* set default values for first server sdo */
    if ((sdoNr == 1) && (kindOfUse == SERVER)) {
	/* default COB-ID */
	cobIdReq = CO_COBID_CSDO + (UNSIGNED32)coNodeId ;
	/* default COB-ID */
	cobIdRes = CO_COBID_SSDO + (UNSIGNED32)coNodeId ;
	C_COB_kind = CO_COB_SDO_RX;
	S_COB_kind = CO_COB_SDO_TX;
# ifdef CONFIG_SDO_COB_ID
	baseIndex = SSDO_PARA_BASE_INDEX;
# endif /* CONFIG_SDO_COB_ID */
    } else  {

# if defined(CONFIG_SDO_COB_ID) || defined(CONFIG_SDO_CLIENT)
	if (kindOfUse == SERVER ) {
#  ifdef CONFIG_SDO_COB_ID
	    baseIndex = SSDO_PARA_BASE_INDEX;
	    C_COB_kind  = CO_COB_SDO_RX;
	    S_COB_kind  = CO_COB_SDO_TX;
#  else /* CONFIG_SDO_COB_ID */
	    return(CO_E_TRANS_TYPE);
#  endif /* CONFIG_SDO_COB_ID */
	} else {
	    baseIndex = CSDO_PARA_BASE_INDEX;
	    C_COB_kind  = CO_COB_SDO_TX;
	    S_COB_kind  = CO_COB_SDO_RX;
	}

	/* read COB-ID from object dictionary */
	if (getObjEntry(baseIndex + (UNSIGNED16)sdoNr - 1, 1,
	    (UNSIGNED8 *)&cobIdReq, &size, CO_TRUE  CO_COMMA_LINE_PARA) != CO_OK) {
	    return CO_E_NO_ACCESS;
	}
	if (getObjEntry(baseIndex + (UNSIGNED16)sdoNr - 1, 2,
	    (UNSIGNED8 *)&cobIdRes, &size , CO_TRUE CO_COMMA_LINE_PARA) != CO_OK) {
	    return CO_E_NO_ACCESS;
	}

# else /* defined(CONFIG_SDO_COB_ID) || defined(CONFIG_SDO_CLIENT) */
	return(CO_E_TRANS_TYPE);
# endif /* defined(CONFIG_SDO_COB_ID) || defined(CONFIG_SDO_CLIENT) */
    }

# if defined(CONFIG_SDO_COB_ID) || defined(CONFIG_SDO_CLIENT)
    pDomInUse = co_pFirstDomEntry ;
    while (pDomInUse != NULL)  {
	pLastDomEntry = pDomInUse;
	pDomInUse = pDomInUse->pNext;
    }
# endif /* defined(CONFIG_SDO_COB_ID) || defined(CONFIG_SDO_CLIENT) */

    if ((pDomInUse = (CMS_DOMAIN_T *)CalMalloc(sizeof(CMS_DOMAIN_T))) == NULL) {
	return(CO_E_MEM);
    }

# if defined(CONFIG_SDO_COB_ID) || defined(CONFIG_SDO_CLIENT)
    if (co_pFirstDomEntry  == NULL) {
# endif /* defined(CONFIG_SDO_COB_ID) || defined(CONFIG_SDO_CLIENT) */
	co_pFirstDomEntry  = pDomInUse;
# if defined(CONFIG_SDO_COB_ID) || defined(CONFIG_SDO_CLIENT)
    } else {
	pLastDomEntry->pNext = pDomInUse;
    }
# endif /* defined(CONFIG_SDO_COB_ID) || defined(CONFIG_SDO_CLIENT) */
    pDomInUse->pNext = NULL;

    /* fill in domain attributes */
    pDomInUse->num	= sdoNr;
    pDomInUse->userType	= kindOfUse;
    pDomInUse->state = SDOSTATE_READY;

# if defined(CONFIG_SDO_COB_ID) || defined(CONFIG_SDO_CLIENT)
    if ((cobIdRes & SDO_NO_VALID_BIT) != 0) {
	pDomInUse->state = SDOSTATE_DISABLED;
    }
# endif /* defined(CONFIG_SDO_COB_ID) || defined(CONFIG_SDO_CLIENT) */

    if ((pDomInUse->pReqInd_COB =
	DEFINE_COB(C_COB_kind, 8 CO_COMMA_LINE_PARA)) == NULL) {
	return(CO_E_NO_DATABASE);
    }

    if ((pDomInUse->pResCon_COB =
	 DEFINE_COB(S_COB_kind, 8 CO_COMMA_LINE_PARA)) == NULL) {
	return(CO_E_NO_DATABASE);
    }

    SET_COB_ID(pDomInUse->pReqInd_COB, (UNSIGNED16)cobIdReq);
    SET_COB_ID(pDomInUse->pResCon_COB, (UNSIGNED16)cobIdRes);

    return(CO_OK);
}
#endif /* defined(CONFIG_SDO_SERVER) || defined(CONFIG_SDO_CLIENT) */


/* only Client functions */

#ifdef CONFIG_SDO_CLIENT

/****************************************************************************/
/**
*++ \brief writeSdoReq - write a dataobject to the server's object dictionary
*-- \brief writeSdoReq - schreibt ein Datenobjekt zum Serverobjektverzeichnis
*
*++ The function writes data objects, which are referenced by the index and
*++ subindex to the servers object dictionary.
*++ After a successful confirmation by \em sdoWrCon() the value is valid
*++ on the remote device.
*++ Errors on transmission have to be checked within
*++ \em sdoWrCon() .
*++ The user is responsible for the program synchronization 
*++ between the write request
*++ and the write confirmation call.
*++ For single tasking systems the function \em waitForSdoRes()
*++ is available in module \b utility.c.
*-- Diese Funktion schreibt Datenobjekte, die über die Angabe von Index
*-- und Subindex addressiert werden zum Objektverzeichnis des SDO Servers.
*-- Nach erfolgreicher Bestätigung durch die Funktion \em sdoWrCon() 
*-- ist der geschriebene Wert in dem angesprochenen Gerät gültig. 
*-- Treten Fehler beim Schreibzugriff auf, so sind diese innerhalb von
*-- \em sdoWrCon() auszuwerten.
*-- Die Programmsynchronisation zwischen
*-- Schreibanforderung und Schreibbestätigung ist durch den Benutzer 
*-- sicherzustellen.
*-- Für Einprozeßsysteme kann dazu die Funktion
*-- \em waitForSdoRes() aus dem Modul \b utility.c genutzt werden.
*++ There is an exception for big endian devices (CONFIG_BIG_ENDIAN is set)
*++ and real 16-bit CPUs (CONFIG_16BIT_CPU is set).
*++ For these the bit 7 of the parameter
*++ \b sdo
*++ has to be set, if numeric values
*++ should be transmited e.g. \c sdo \c = \c sdo \c | \c 0x80.
*++ It is recommended to use the constant \c CO_NUM_SDO in order to
*++ ensure the portability of the application. 
*
*-- Für Big-Endian-Geräte (\c CONFIG_BIG_ENDIAN ist gesetzt)
*-- und real 16-bit CPUs (CONFIG_16BIT_CPU is set).
*-- gibt es eine Ausnahme.
*-- Sollen numerische Werte übertragen werden, so ist das Bit 7 
*-- des Parameters sdo zu setzen z.B. sdo = sdo | 0x80.
*-- Soll Portabilität der Programme gesichert werden, so wird empfohlen
*-- die Konstante \c CO_NUM_SDO zur SDO-Nummer zu addieren.
*
*++ The following example uses the
*++ \em writeSdoReq()
*++ to set the COB-ID of the first receive PDO of another node.
*-- Das folgende Beispiel zeigt,
*-- wie man den
*-- \em writeSdoReq()
*-- benutzt, um die COB-ID des ersten Empfangs PDO eines anderen Knoten
*-- umzukonfigurieren.
*
* \code
* UNSIGNED32 cobId;
* cobId = 200;
* ret = writeSdoReq(CO_NUM_SDO + 1, 0x1400, 1, &cobId, 4);
* \endcode
*
* \retval CO_OK
*++ success
*-- Erfolg
* \retval CO_E_NOT_EXIST
*++ SDO with current code number doesn't exist
*-- SDO mit aktueller Nummer existiert nicht
* \retval CO_E_STATE
*++ node isn't in state OPERATIONAL or PRE_OPERATIONAL
*-- Knoten ist nicht im Zustand OPERATIONAL oder PRE_OPERATIONAL
* \retval CO_E_TYPE
*++ SDO usertype isn't CLIENT
*-- SDO Nutzertyp ist nicht CLIENT
* \retval CO_E_BUSY
*++ an automatic domain transfer is still operating for this SDO
*-- ein automatischer Domaintransfer für diese SDO ist noch aktiv
* \retval CO_E_DISABLED
*++ selected SDO is disabled
*-- das gewählte SDO ist inaktiv
*/

RET_T writeSdoReq(
      UNSIGNED8   sdo,	   /**< number of SDO */
      UNSIGNED16  index,   /**< index of object dictionary */
      UNSIGNED8   subIndex,/**< subindex of object dictionary */
      UNSIGNED8   *pData,  /**< sdo data address */
      UNSIGNED32  length   /**< length of sdo data */
      )
{
SDO_T	*pCurSdo;		/* pointer to current sdo */

    /* the setting of the global multiplexor is for more
       convenience and reliability */
    pCurSdo = CMS_DomExist(sdo & CO_NONUM_SDO, CLIENT CO_COMMA_LINE_PARA);
    if (pCurSdo == NULL) {
	return(CO_E_NOT_EXIST);
    }

# if defined(CONFIG_BIG_ENDIAN) || defined(CONFIG_16BIT_CPU)
    if ((sdo & CO_NUM_SDO) != 0)  {
	pCurSdo->numeric = CO_TRUE;
    } else {
	pCurSdo->numeric = CO_FALSE;
    }
# endif /* defined(CONFIG_BIG_ENDIAN) || defined(CONFIG_16BIT_CPU) */

    pCurSdo->odIndex.index = index;
    pCurSdo->odIndex.subIndex = subIndex;

# ifdef CONFIG_16BIT_CPU
    pCurSdo->halfWord = CO_FALSE;
# endif /* CONFIG_16BIT_CPU */

    return(initUpDnLd_req(pCurSdo, pData, length,
# ifdef CONFIG_SDO_BLOCKTRANSFER
		CO_SDOBLK_CCS_DOWN
# else /* CONFIG_SDO_BLOCKTRANSFER */
		CCS_INI_DN_LD_REQ
# endif /* CONFIG_SDO_BLOCKTRANSFER */
		CO_COMMA_LINE_PARA));
}


/****************************************************************************/
/**
*++ \brief readSdoReq - read a data object from the server's object dictionary
*-- \brief readSdoReq - liest ein Datenobjekt vom Serverobjektverzeichnis
*
*++ The function reads data objects,
*++ which are referenced by the index and subindex
*++ from the servers object dictionary.
*++ After a successful confirmation by \em sdoRdCon() the value was received
*++ from the remote device.
*++ Errors on transmission have to be checked within
*++ \em sdoRdCon().
*++ The user is responsible for the program synchronization
*++ between the read request
*++ and the read confirmation call.
*++ For single tasking systems the function \em waitForSdoRes()
*++ is available in module \b utility.c .
*-- Diese Funktion liest Datenobjekte, die über die Angabe von Index
*-- und Subindex adressiert werden, vom Objektverzeichnis des SDO Servers.
*-- Nach erfolgreicher Bestätigung durch die Funktion \em sdoRdCon()
*-- ist der gelesene Wert auf dem lokalen Gerät gültig. 
*-- Treten Fehler beim Lesezugriff auf, so sind diese innerhalb von
*-- \em sdoRdCon() auszuwerten.
*-- Die Programmsynchronisation zwischen
*-- Leseanforderung und Lesebestätigung ist durch den Benutzer 
*-- sicherzustellen.
*-- Für Einprozeßsysteme kann dazu die Funktion
*-- \em waitForSdoRes() aus dem Modul \b utility.c genutzt werden.
*
*++ There is an exception for big endian devices (CONFIG_BIG_ENDIAN is set)
*++ and real 16-bit CPUs (CONFIG_16BIT_CPU is set).
*++ For these the bit 7 of parameter sdo has to be set, when numeric values
*++ should be received e.g.
*++ \code 
*++ sdo = sdo | 0x80;
*++ sdo = sdo | CO_NUM_SDO;
*++ \endcode
*++ It is recommended to use the constant \c CO_NUM_SDO in order to
*++ ensure the portability of the application. 
*-- Für Big-Endian-Geräte (CONFIG_BIG_ENDIAN ist gesetzt)
*-- und real 16-bit CPUs (CONFIG_16BIT_CPU is set)
*-- gibt es eine Ausnahme.
*-- Sollen numerische Werte empfangen werden, so ist das Bit 7 
*-- des Parameters 'sdo' zu setzen z.B.
*-- \code 
*-- sdo = sdo | 0x80;
*-- sdo = sdo | CO_NUM_SDO;
*-- \endcode
*-- Soll Portabilität der Programme gesichert werden, so wird empfohlen
*-- die Konstante \c CO_NUM_SDO zur SDO-Nummer zu addieren.
*
*++ The following example uses the
*++ \em readSdoReq()
*++ to read a value from a remote device.
*-- Das folgende Beispiel zeigt,
*-- wie man den
*-- \em readSdoReq()
*-- zum Lesen von Werten von einem anderen Gerät verwendet.
*
* \code
* UNSIGNED32 value;
* ret = readSdoReq(CO_NUM_SDO + 1, 0x2000, 0, &value, 4);
* \endcode
*
* \retval CO_OK
*++ success
*-- Erfolg
* \retval CO_E_NOT_EXIST
*++ SDO with current codenumber doesn't exist
*-- SDO mit aktueller Nummer existiert nicht
* \retval CO_E_STATE
*++ node isn't in state OPERATIONAL or PRE_OPERATIONAL
*-- Knoten ist nicht im Zustand OPERATIONAL oder PRE_OPERATIONAL
* \retval CO_E_TYPE
*++ SDO usertype isn't CLIENT
*-- SDO Nutzertyp ist nicht CLIENT
* \retval CO_E_BUSY
*++ an automatic domain transfer is still operating for this SDO
*-- ein automatischer Domaintransfer für diese SDO ist noch aktiv
* \retval CO_E_DISABLED
*++ selected SDO is disabled
*-- die gewählte SDO ist deaktiviert
*
*/

RET_T readSdoReq(
      UNSIGNED8   sdo, 	    /**< Codenumber of SDO */
      UNSIGNED16  index,    /**< index of object dictionary */
      UNSIGNED8   subIndex, /**< subindex of object dictionary */
      UNSIGNED8   *pObj,    /**< pointer to destination address for object*/
      UNSIGNED32  size      /**< size of object */
      )
{
SDO_T	*pCurSdo;		/* pointer to current sdo */

    /* the setting of the global multiplexor is for more
       convenience and reliability */
    pCurSdo = CMS_DomExist(sdo & CO_NONUM_SDO, CLIENT CO_COMMA_LINE_PARA);

    if (pCurSdo == NULL)  {
	return(CO_E_NOT_EXIST);
    }

# if defined(CONFIG_BIG_ENDIAN) || defined(CONFIG_16BIT_CPU)
    if ((sdo & CO_NUM_SDO) != 0)  {
	pCurSdo->numeric = CO_TRUE;
    } else {
	pCurSdo->numeric = CO_FALSE;
    }
# endif /* defined(CONFIG_BIG_ENDIAN) || defined(CONFIG_16BIT_CPU) */

    pCurSdo->odIndex.index = index;
    pCurSdo->odIndex.subIndex = subIndex;

# ifdef CONFIG_16BIT_CPU
    pCurSdo->halfWord = CO_FALSE;
# endif /* CONFIG_16BIT_CPU */

    return(initUpDnLd_req(pCurSdo, pObj, size,
# ifdef CONFIG_SDO_BLOCKTRANSFER
		CO_SDOBLK_CCS_UP
# else /* CONFIG_SDO_BLOCKTRANSFER */
		CCS_INI_UP_LD_REQ
# endif /* CONFIG_SDO_BLOCKTRANSFER */
		CO_COMMA_LINE_PARA));
}

#endif /* CONFIG_SDO_CLIENT */


/****************************************************************************/
/**
*++ \brief getSdoSize - get the size of data read by SDO
*-- \brief getSdoSize - ermittelt die Größe von per SDO gelesenen Daten
*
*++ This function gets the size of data read by SDO.
*++ This function is
*++ useful for data types with no fixed length e.g. Octet-Strings.
*++ For Visual-Strings this function is not necessary, because the
*++ CANopen Library appends an End of String character at the string
*++ if estimated size > real size.
*-- Diese Funktion ermittelt die reale Größe der per SDO gelesenen Daten.
*-- Sie wird genutzt für Datentypen ohne feste Länge z.B. Octet-Strings.
*-- Für Visual-Strings ist sie nicht notwendig, da die CANopen Bibliothek
*-- ein End of String - Zeichen anhängt, falls die geschätzte Größe größer 
*-- als die reale Größe ist.
*
* \code
* // for SDO Clients
* UNSIGNED8 value[127];
* size = 127;                                      // max. allowed size
* ret = readSdoReq(1, 0x2000, 0, &value[0], size);
* if (waitForSdoRes(1,1000) == CO_TRUE)            // wait for response
*     size = getSdoSize(1, CLIENT);                // get real size of data
* // for SDO Server
* sdoWrInd(UNSIGNED16 index, UNSIGNED8 subIndex)
* {
*     sdo = getActualSdo(index, subIndex);         // get number of active SDO
*     size = getSdoSize(sdo, SERVER);              // size of written data
* }
* \endcode
*
* \retval size
*++ size of data, at success of read SDO
*-- Datengröße, bei Erfolg von read SDO
* \retval X
*++ undefined value, when read SDO not succesful
*-- undefiniert Wert, wenn read SDO nicht erfolgreich
*
*/

UNSIGNED32 getSdoSize(
	   UNSIGNED8   sdo, 	  /**< Codenumber of SDO */
	   USER_T      kindOfUse  /**< CLIENT/SERVER */
	   )
{
SDO_T	*pCurSdo;		/* pointer to actual sdo */

    pCurSdo = CMS_DomExist((UNSIGNED8)(sdo & CO_NONUM_SDO), kindOfUse CO_COMMA_LINE_PARA);
    assert(pCurSdo != NULL);
    return(pCurSdo->domSize);
}


/****************************************************************************/
/**
*++ \brief getActualSdo - get the number of the current Server SDO
*-- \brief getActualSdo - ermittelt die Nummer der aktuellen Server-SDO
*
*++ This function returns the number of the current Server SDO.
*++ This function is useful to get the sdo number within sdoWrInd().
*-- Diese Funktion ermittelt die Nummer der aktuellen Server-SDO.
*-- Sie wird genutzt innerhalb von sdoWrInd, um die Nummer der aktuellen
*-- SDO zu ermitteln.
*
* \code
* sdoWrInd(UNSIGNED16 index, UNSIGNED8 subIndex)
* {
*     sdo = getActualSdo(index, subIndex);     // get number of active SDO
*     size = getSdoSize(sdo, SERVER);          // size of written data
* }
* \endcode
* \retval number
*++ number of Server SDO (1-128)
*-- Server-SDO Nummer (1-128)
* \retval 0
*++ failure
*-- Fehler
*
*/

UNSIGNED8 getActualSdo(
	  UNSIGNED16 index,       /**< index of transfered object */
	  UNSIGNED8  subIndex     /**< sub index of transfered object */
	  )
{
SDO_T *pSdoInUse;		/* pointer to actual sdo */

    if ((pSdoInUse = co_pFirstDomEntry  ) == NULL) {
	return(0);
    }

    while(pSdoInUse != NULL) {
       if (pSdoInUse->userType == SERVER) {
	   if (  (pSdoInUse->odIndex.index == index)
	      && (pSdoInUse->odIndex.subIndex == subIndex))
	   return(pSdoInUse->num);
       }
       pSdoInUse = pSdoInUse->pNext;
    }
    return(0);
}


/*******************************************************************
*
* setSdoCobId - sets the COB-ID of SDO
*
* NOMANUAL
*
* This function sets cob-ids for SDO
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

RET_T setSdoCobId(
	SDO_T	*pSdo,		/* pointer to sdo structure */
	UNSIGNED32 cobId,	/* new COB-ID */
	UNSIGNED8  subIndex	/* subindex */
      )
{

    /* Extended IDs are not allowed */
    if ((cobId & CAN_29_BIT_ID_FLAG) != 0)  {
	return(CO_E_TRANS_TYPE);
    }

    /* set internal valid bit */
    if ((cobId & SDO_NO_VALID_BIT) != 0)  {
	pSdo->state = SDOSTATE_DISABLED;
    } else {

	/* library only accepts 11 bit identifier */
	cobId &= CAN_11_BIT_ID_MASK;

	/* test for valid COB-Ids */
	if (((cobId & CAN_11_BIT_ID_MASK) < CO_COBID_SDO_FIRST)
	 || ((cobId & CAN_11_BIT_ID_MASK) > CO_COBID_SDO_LAST)) {
	    return(CO_E_RANGE);
	}

	pSdo->state = SDOSTATE_READY;

	if (subIndex == 1) {
	    SET_COB_ID(pSdo->pReqInd_COB, (UNSIGNED16)cobId);
	} else {
	    SET_COB_ID(pSdo->pResCon_COB, (UNSIGNED16)cobId);
	}
    }
    return(CO_OK);
}


/****************************************************************************/
/**
*++ \brief abortSdoTransf_Req - request the remote service abort domain transfer
*-- \brief abortSdoTransf_Req - Anforderung eines Abort Domain Transfers
*
*-- Der Client
*-- oder der Server der Domain
*-- versuchen den Domaintransfer wegen eines Fehlers abzubrechen.
*-- Dieser Dienst ist unbestätigt.
*-- Der Fehlercode \em errReason kann folgende Werte annehmen:
*++ Client or server of a domain try to interrupt the transmission
*++ due do a error condition.
*++ This service is un-confirmed.
*++ The error code \em errReason can have the following values:
*
* \arg \c 0
*-- nicht spezifizierter Fehler
*++ not specified error
* \arg \c 1
*-- Application Request
*++ interrupted on application request
* \arg \c 2
*-- keine Resourcen verfügbar
*++ no more internal resources
* \arg \c 3..127
*-- reserviert
*++ reserved
* \arg \c 128..255
*-- implementierungsspezifischer Fehler
*++ implementation specific error
* \par
*-- ist der Fehlercode = 1 oder größer 127
*-- gibt \em dApplErr die genauere Abbruchursache an.
*++ for error codes 1 or > 127
*++ the value of \em dApplErr gives the reason for the abort.
* 
* \retval CO_OK
*-- Erfolg
*++ Success
* \retval CO_E_NOT_EXIST
*-- Domain mit dieser Codenummer existiert nicht
*++ domain with this number does not exist
* \retval CO_E_STATE
*-- Knoten nicht im Zustand OPERATIONAL
*++ node not in state OPERATIONAL
* \internal
* input: handle, error reason
*/
RET_T abortSdoTransf_Req(
	SDO_T	*pCurSdo,
	RET_T	commonRet	/**< return error code */
      )
{
UNSIGNED8	pData[8];	/* transmit buffer  */
UNSIGNED32	errReason;	/* error reason coded */

#ifdef CONFIG_CO_DEBUG
    BDEBUG(CO_DEBUG_SDO, "*** Abort Sdo Transfer %x\n", (UNSIGNED8)commonRet);
#endif /* CONFIG_CO_DEBUG */

    /* detect the error reason */
    switch (commonRet) {
	case CO_E_NONEXIST_OBJECT:
	    errReason = E_SDO_ACCESS | E_SDO_NONEXIST_OBJECT;
	    break;

	case CO_E_NONEXIST_SUBINDEX:
	    errReason = E_SDO_ACCESS | E_SDO_INCONS_OBJ_ATTR
		| E_SDO_A_NONEXIST_SUBINDEX;
	    break;

	case CO_E_NO_READ_PERM:
	    errReason = E_SDO_ACCESS | E_SDO_UNSUPP_ACCESS
		| E_SDO_A_NO_READ_PERM;
	    break;

	case CO_E_NO_WRITE_PERM:
	    errReason = E_SDO_ACCESS | E_SDO_UNSUPP_ACCESS
		| E_SDO_A_NO_WRITE_PERM;
	    break;

	case CO_E_MAP:
	    errReason = E_SDO_ACCESS | E_PDO_MAPPING | E_SDO_A_NO_MAPPING;
	    break;

	case CO_E_DATA_LENGTH:
	    errReason = E_SDO_ACCESS | E_PDO_MAPPING
		| E_SDO_A_PDO_LENGTH_EXCEED;
	    break;

	case CO_E_TRANS_TYPE:
	    errReason = E_SDO_ACCESS | E_SDO_INCONS_OBJ_ATTR |
			E_SDO_A_VALUE_RANGE_EXCEED;
	    break;

	case CO_E_VALUE_TO_HIGH:
	    errReason = E_SDO_ACCESS | E_SDO_INCONS_OBJ_ATTR
			| E_SDO_A_VALUE_TO_HIGH;
	    break;

	case CO_E_VALUE_TO_LOW:
	    errReason = E_SDO_ACCESS | E_SDO_INCONS_OBJ_ATTR
			| E_SDO_A_VALUE_TO_LOW;
	    break;

	case CO_E_WRONG_SIZE:
	    errReason = E_SDO_ACCESS | E_SDO_TYPE_CONFLICT
			| E_SDO_A_INVALID_VAL;
	    break;

	case CO_E_PARA_INCOMP:
	    errReason = E_SDO_ACCESS | E_SDO_ILLEG_PARA
			| E_SDO_A_GENERAL_PARA_INCOMP;
	    break;

	case CO_E_HARDWARE_FAULT:
	    errReason = E_SDO_ACCESS | E_SDO_HARDWARE_FAULT;
	    break;

	case CO_E_SRD_NO_RESSOURCE:
	    errReason = E_SDO_ACCESS | E_SDO_RES_NOT_AVAIL
		| E_SDO_A_SDO_CONN;
	    break;
	
	case CO_E_SDO_CMD_SPEC_INVALID:
	    errReason = E_SDO_SERVICE | E_SDO_ILLEG_PARA
		| E_SDO_A_CMD_SPEC_INVALID;
	    break;

	case CO_E_MEM:
	    errReason = E_SDO_SERVICE | E_SDO_ILLEG_PARA | E_SDO_A_OUT_OF_MEM;;
	    break;

	case CO_E_SDO_INVALID_BLKSIZE:
	    errReason = E_SDO_SERVICE | E_SDO_ILLEG_PARA
		| E_SDO_A_SIZE_INVALID;
	    break;

	case CO_E_SDO_INVALID_BLKCRC:
	    errReason = E_SDO_SERVICE | E_SDO_ILLEG_PARA
		| E_SDO_A_CRC_INVALID;
	    break;

	case CO_E_SDO_TIMEOUT:
	    errReason = E_SDO_SERVICE | E_SDO_ILLEG_PARA;
	    break;

	case CO_E_INVALID_TRANSMODE:
	    errReason = E_SDO_OTHER | E_SDO_A_INVALID_TRANSMODE;
	    break;

	case CO_E_SDO_OTHER:
	    errReason = E_SDO_OTHER;
	    break;

	case CO_E_DEVICE_STATE:
	    errReason = E_SDO_OTHER | E_SDO_A_WRONG_STATE;
	    break;

	default:
	    errReason = E_SDO_OTHER;
    }

    CMS_SdoEncode(&pCurSdo->odIndex, pData);
    pData[0] = CS_ABORT_TRANSFER;

    /* fill in the reason bytes */
    CO_UNPACK_MEMCPY(&pData[4], (UNSIGNED8*)&errReason, 4, CO_NUM_VAL);

    if (pCurSdo->userType == CLIENT) {
	TRANSMIT_COB(pCurSdo->pReqInd_COB, pData);
    } else {
	TRANSMIT_COB(pCurSdo->pResCon_COB, pData);
    }

    pCurSdo->state = SDOSTATE_READY;
    return(commonRet);
}

/*______________________________________________________________________EOF_*/

