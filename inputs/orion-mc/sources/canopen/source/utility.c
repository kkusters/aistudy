/*
 *++ utility - contains utility functions
 *-- utility - beinhaltet Utility Funktionen
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
 * Revision 1.0  2008-03-07 17:04:51+01  driet
 * Initial revision
 *
 * Revision 2.30  2003/07/30 08:13:13  boe
 * include time and sync header only if CONFIG_TIME/CONFIG_SYNC is set
 *
 * Revision 2.29  2003/06/13 14:21:31  boe
 * new define CONFIG_NO_MAP_SYNC_PDO - don't map after sync
 * and before send PDOs
 *
 * Revision 2.28  2003/03/31 13:47:05  boe
 * reset timer flag before calling checkTimerEvent
 * time secure for eva-version deleted
 *
 * Revision 2.27  2003/01/21 09:06:16  boe
 * init timer structure for coWait()
 *
 * Revision 2.26  2002/11/18 10:01:28  boe
 * add comments/adapt on doxygen
 * add co_ to all global library variables
 * add special functionality for 16 bit CPUs
 *
 * Revision 2.25  2002/05/30 12:31:55  hae
 * documentation correction
 *
 * Revision 2.24  2002/05/21 14:24:23  boe
 * cleanup new timer usage
 * add led usage
 * add crc8 calculation
 *
 * Revision 2.23  2002/03/26 08:14:43  boe
 * add NEW_TIMER functionality over defines
 *
 * Revision 2.22  2001/12/20 13:58:46  boe
 * add 8bit crc calculation
 *
 * Revision 2.21  2001/10/15 13:09:50  ro
 * include added
 * changes for the prospectively timer concept
 *
 * Revision 2.20  2001/08/21 14:05:30  boe
 * crc calculation adapted to correctness standard ds301
 *
 * Revision 2.19  2001/05/10 14:04:16  boe
 * adaption for new ds307
 *
 * Revision 2.18  2001/04/12 14:02:08  boe
 * start blocktransfer for server and client
 *
 * Revision 2.17  2001/04/05 12:27:03  boe
 * command line parameter for single line changed to void
 *
 * Revision 2.16  2001/04/05 08:45:58  boe
 * comment changed
 *
 * Revision 2.15  2001/03/29 14:34:33  boe
 * comment changed
 *
 * Revision 2.14  2001/03/28 15:50:15  boe
 * include header files for master only if CONFIG_MASTER is set
 *
 * Revision 2.13  2001/03/28 12:49:37  boe
 * added functionality for SLAVE_PLUS
 * call user sync function
 *
 * Revision 2.12  2001/03/19 15:57:26  ro
 * some header only include if used - corrected
 *
 * Revision 2.11  2001/03/14 15:05:39  ro
 * driver access functions replaced by macros
 *
 * Revision 2.10  2001/02/26 14:12:29  boe
 * documentation format changed
 * flying master functionality added
 *
 * Revision 2.9  2001/01/26 12:16:02  boe
 * split include files into function specific headers
 * move getMapObjAddr() and waitForSdoRes() to other files
 *
 * Revision 2.8  2001/01/17 16:21:25  boe
 * expand all implicite if tests and add type castings
 *
 * Revision 2.7  2000/10/23 13:04:02  ro
 * merge with old Version
 *
 * Revision 2.6  2000/10/23 12:56:11  ro
 * Timer for evalution version
 *
 * Revision 2.5  2000/10/04 13:57:45  boe
 * defines for PDO changed from CLIENT to CONSUMER and SERVER to PRODUCER
 *
 * Revision 2.4.2.2  2000/10/23 12:51:34  ro
 * Clear_busoff
 *
 * Revision 2.4.2.1  2000/10/23 08:00:05  ro
 * dTimerValue removed
 *
 * Revision 2.4  2000/06/23 09:43:07  oe
 * Reworking for generating the Regerence Manual
 *
 * Revision 2.3  2000/06/13 08:41:57  boe
 * function names (H_) changed, blocktransfer added,
 * CRC functions added
 *
 * Revision 2.2  2000/03/28 14:40:57  boe
 * adaption for multi-line version
 * sync producer mode changed
 * function getMapObjAddr added
 *
 * Revision 2.1  2000/02/04 13:37:41  oe
 * - Changes for generating reference pages
 *
 * Revision 2.0  2000/01/21 11:05:03  boe
 * Überarbeitet und an Version 4.0 angepasst
 *
 *
 *
 *------------------------------------------------------------------
 */


/****************************************************************************/
/**
*  \file utility.c
*  \author port GmbH Halle (Saale)
*  $Revision$
*  $Date$
*
*++ This module contains some useful functions for a convenient usage of
*++ the CANopen Library.
*-- Dieses Modul enthält nützliche Funktionen zur Benutzung der CANopen
*-- Bibliothek.
* \par
*++ Some of the functions are for internal usage only.
*++ They have no manual entries.
*++ For example CRC calculation used by the SDO block-transfer.
*-- Einige dieser Funktionen werden nur intern benutzt.
*-- Sie haben keine Handbuch Einträge.
*-- Zum Beispiel eine CRC-Summenberechnung,
*-- welche vom SDO Blocktransfer genutzt wird.
*
*/

/* header of standard C - libraries */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

/* header of common types */

#include <cal_conf.h>
#include <co_flag.h>
#include <co_usr.h>
#include <co_mcpy.h>
#include <co_drvif.h>
#include "utility.h"
#include "drv.h"
#include "nmterr.h"
#include "pdo.h"

#if defined(CONFIG_SYNC_PRODUCER) || defined(CONFIG_SYNC_CONSUMER)
#include "sync.h"
#endif /* defined(CONFIG_SYNC_PRODUCER) || defined(CONFIG_SYNC_CONSUMER) */

#if defined(CONFIG_TIME_PRODUCER) || defined(CONFIG_TIME_CONSUMER)
#include "timer.h"
#endif /* defined(CONFIG_TIME_PRODUCER) || defined(CONFIG_TIME_CONSUMER) */

#if defined(CONFIG_MASTER) || defined(CONFIG_SLAVE_PLUS)
# include "nmterr_m.h"
#endif /* defined(CONFIG_MASTER) || defined(CONFIG_SLAVE_PLUS) */

#ifdef CONFIG_FLYING_MASTER
# include "flyma.h"
#endif /* CONFIG_FLYING_MASTER */

#ifdef CONFIG_SDO_BLOCKTRANSFER
# include "sdoblock.h"
#endif /* CONFIG_SDO_BLOCKTRANSFER */

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


/*******************************************************************
*
*++ flagIdentification - indicate library flags
*-- flagIdentification - indentifiziert library flags
*
* NOMANUAL
*
*++ This function evaluates the CANopen-library flags
*++ and calls the apropriate routines.
*++ To minimize the interrupt service routines
*++ there are flags for all necessary services.
*++ These flags are set at the interruptroutines and
*++ evaluate in this function
*-- Diese Funktion wertet die CANopen Library Flags
*-- aus und ruft die entsprechende Behandlungsroutine
*-- Um die Interruptroutinen möglichst kurz zu halten,
*-- werden für die notwendigen Aufgaben in den Interruptroutinen
*-- nur Flags gesetzt.
*-- Diese Flags werden in dieser Funktion ausgewertet.
*
* \retval
*	nothing
*/

void flagIdentification(
	void
       )
{
#ifdef CONFIG_CAN_ERROR_HANDLING
UNSIGNED8	tmpFlags;	/* temporary coFlags */
#endif /* CONFIG_CAN_ERROR_HANDLING */

#ifdef CONFIG_SYNC_CONSUMER

    if (TEST_COLIB_FLAG(COFLAG_SYNC_RECEIVED))  {
# ifdef CONFIG_PDO_PRODUCER
#  ifdef CONFIG_NO_MAP_SYNC_PDO
#  else /* CONFIG_MAP_SYNC_PDO_AFTER_SEND */
	updateSyncTpdo(CO_LINE_PARA);
#  endif /* CONFIG_NO_MAP_SYNC_PDO */

	/* transmit synch. TPDOs */
	transSyncPdo(CO_LINE_PARA);
# endif /* CONFIG_PDO_PRODUCER */
# ifdef CONFIG_PDO_CONSUMER
	updateSyncRpdo(CO_LINE_PARA);
# endif /* CONFIG_PDO_CONSUMER */

# ifdef CONFIG_SYNC_CMD
	syncCommand(CO_LINE_PARA);
# endif /* CONFIG_SYNC_CMD */

	RESET_COLIB_FLAG(COFLAG_SYNC_RECEIVED);
    }
#endif  /* defined(CONFIG_SYNC_CONSUMER) */


    /* timer pulse */
    if ((TEST_COLIB_FLAG(COFLAG_TIMER_PULSED)) != 0) {

	RESET_COLIB_FLAG(COFLAG_TIMER_PULSED);

	checkTimerEvent(CO_LINE_PARA);

#ifdef CONFIG_SYNC_PRODUCER
	/* sync producer has to send own sync pdos */
	if (TEST_COLIB_FLAG(COFLAG_SYNC_RECEIVED))  {
# ifdef CONFIG_PDO_PRODUCER
#  ifdef CONFIG_NO_MAP_SYNC_PDO
#  else /* CONFIG_MAP_SYNC_PDO_AFTER_SEND */
	    updateSyncTpdo(CO_LINE_PARA);
#  endif /* CONFIG_NO_MAP_SYNC_PDO */

	    /* transmit synch. TPDOs */
	    transSyncPdo(CO_LINE_PARA);
# endif /* CONFIG_PDO_PRODUCER */
# ifdef CONFIG_PDO_CONSUMER
	    updateSyncRpdo(CO_LINE_PARA);
# endif /* CONFIG_PDO_CONSUMER */

# ifdef CONFIG_SYNC_CMD
	    syncCommand(CO_LINE_PARA);
# endif /* CONFIG_SYNC_CMD */

	    RESET_COLIB_FLAG(COFLAG_SYNC_RECEIVED);
	}
#endif /* CONFIG_SYNC_PRODUCER */

    }

#ifdef CONFIG_CAN_ERROR_HANDLING

# ifdef CONFIG_CO_ERR_LED
    if (TEST_COLIB_FLAG(COFLAG_CAN_PASSIVE)) {
	setCoLedState(CO_ERR_LED_CAN_WARN);
    }
    if (TEST_COLIB_FLAG(COFLAG_CAN_BUSOFF)) {
	setCoLedState(CO_ERR_LED_BUSOFF);
    }
# endif /* CONFIG_CO_ERR_LED */

    /* there are more than one flag for CAN errors,
     * therefore we have to reset only the given flags */
    tmpFlags = TEST_COLIB_FLAG(COFLAG_CAN_ERROR);
    if (tmpFlags != 0)  {
	if (canErrorInd(tmpFlags CO_COMMA_LINE_PARA) == CO_FALSE) {
	    /* try to go BUS ON again */
	    CLEAR_BUSOFF(CO_LINE_PARA);
	}
	RESET_COLIB_FLAG(tmpFlags);
    }
#endif /* CONFIG_CAN_ERROR_HANDLING */


#if defined(CONFIG_SDO_BLOCKTRANSFER)
    /* there are SDO to send for block down/up-load */
    if (TEST_COLIB_FLAG(COFLAG_SDO_BLOCKTRANS))  {

	RESET_COLIB_FLAG(COFLAG_SDO_BLOCKTRANS);

	sdoContBlockTrans(CO_LINE_PARA);
    }
#endif /* defined(CONFIG_SDO_BLOCKTRANSFER) */

#ifdef CONFIG_DYN_SDO_CONNECTION_MANAGER
    /* there are SDO manager function to execute */
    if (TEST_COLIB_FLAG(COFLAG_SDO_MANAGER))  {

	RESET_COLIB_FLAG(COFLAG_SDO_MANAGER);

	dynSdoManager(CO_LINE_PARA);
    }
#endif /* CONFIG_DYN_SDO_CONNECTION_MANAGER */

}


#ifdef CONFIG_CO_WAIT
/****************************************************************************/
/**
*++ \brief coWait - wait for certain time
*-- \brief coWait - wartet für eine bestimmte Zeit
*
*++ This function is useful for single tasking systems in order to get
*++ a synchronous program flow.
*++ It uses the CANopen Timer for realizing of the delay time.
*-- Diese Funktion ist nützlich beim Einsatz von Single Tasking Systemen,
*-- um einem synchronen Programmablauf zu erreichen.
*-- Die Funktion benutzt den CANopen Timer zur Realisierung
*-- der Wartezeit.
* \par
*++ While waiting(sleeping) the CANopen receive queue is tested
*++ and emptied (call CANopen services) if messages do arrive.
*-- In der Wartephase wird die Empfangsqueue überwacht
*-- und CANopen Funktionen gerufen, falls Nachrichten da sind.
*
* \return
*++ nothing
*-- nichts
*
*/

void coWait(
	UNSIGNED32 waitingTime	/**< time to wait 1/10 ms */
	)
{
TIMER_EVENT_T	waitTimer;	/* timer structure */

    /* clear timer structure */
    memset(&waitTimer, 0, sizeof(TIMER_EVENT_T));

    if (addTimerEvent(&waitTimer, waitingTime, 0 CO_COMMA_LINE_PARA) != CO_OK) {
	return;
    }

    while (checkActiveTimer(&waitTimer CO_COMMA_LINE_PARA) == CO_TRUE)  {
    /* endTime = dTimerValue + waitingTime; */
    /* do { */
	    FLUSH_MBOX( CO_LINE_PARA );
    }
    /* while (dTimerValue < endTime); */
}
#endif /* CONFIG_CO_WAIT */


#if defined(CONFIG_BLOCK_CRC) || defined(CONFIG_SRDO_PRODUCER) || defined(CONFIG_SRDO_CONSUMER)
/****************************************************************************/
/**
*++ \brief CRC calculation algorithm to verify SDO block transfers 
*-- \brief CRC Summen Berechnung für SDO Block Transfers
*
*++ This function calculates the checksum for the SDO block transfer.
*++ The check polynom has the formula x^16 + x^12 + x^5 + 1.
*++ The are two variants to calculate the checksum:
*++ \li 1.
*++    using a crc_table
*++    this will costs 512 bytes for the crc table but the algorythm
*++    is very fast.
*++ \li 2.
*++    calculate the crc itself
*++    the necessary code size is equal to the 1st solution, but the runtime is
*++    up to four time slower than the table variant
*-- Diese Funktion berechnet die Checksumme für den SDO Block Transfer.
*-- Die Prüfsumme wird nach dem Polynom x^16 + x^12 + x^5 + 1
*-- berechnet.
*-- Zur Berechnung stehen 2 Varianten zur Verfügung
*-- \li 1.
*--    Nutzung einer CRC Tabelle.
*--    Diese Tabelle benötigt zusätzlich 512 Bytes für die Tabelle.
*--    Der Algorithmus ist aber sehr schnell.
*-- \li 2.
*--    CRC Berechnung ohne Tabelle.
*--    Der benötigte Algorithmus ist vom Code nicht viel länger,
*--    benötigt aber bis 4 mal so lange an Ausführungszeit.
*
* \return
*++ 16 bit CRC sum
*-- 16 Bit CRC Summe
*/

# ifdef CONFIG_BLOCK_CRC_TABLE

static UNSIGNED16 CO_CONST crc_table[] = {
    0x0000, 0x17CE, 0x0FDF, 0x1811, 0x1FBE, 0x0870, 0x1061, 0x07AF, 
    0x1F3F, 0x08F1, 0x10E0, 0x072E, 0x0081, 0x174F, 0x0F5E, 0x1890, 
    0x1E3D, 0x09F3, 0x11E2, 0x062C, 0x0183, 0x164D, 0x0E5C, 0x1992, 
    0x0102, 0x16CC, 0x0EDD, 0x1913, 0x1EBC, 0x0972, 0x1163, 0x06AD, 
    0x1C39, 0x0BF7, 0x13E6, 0x0428, 0x0387, 0x1449, 0x0C58, 0x1B96, 
    0x0306, 0x14C8, 0x0CD9, 0x1B17, 0x1CB8, 0x0B76, 0x1367, 0x04A9, 
    0x0204, 0x15CA, 0x0DDB, 0x1A15, 0x1DBA, 0x0A74, 0x1265, 0x05AB, 
    0x1D3B, 0x0AF5, 0x12E4, 0x052A, 0x0285, 0x154B, 0x0D5A, 0x1A94, 
    0x1831, 0x0FFF, 0x17EE, 0x0020, 0x078F, 0x1041, 0x0850, 0x1F9E, 
    0x070E, 0x10C0, 0x08D1, 0x1F1F, 0x18B0, 0x0F7E, 0x176F, 0x00A1, 
    0x060C, 0x11C2, 0x09D3, 0x1E1D, 0x19B2, 0x0E7C, 0x166D, 0x01A3, 
    0x1933, 0x0EFD, 0x16EC, 0x0122, 0x068D, 0x1143, 0x0952, 0x1E9C, 
    0x0408, 0x13C6, 0x0BD7, 0x1C19, 0x1BB6, 0x0C78, 0x1469, 0x03A7, 
    0x1B37, 0x0CF9, 0x14E8, 0x0326, 0x0489, 0x1347, 0x0B56, 0x1C98, 
    0x1A35, 0x0DFB, 0x15EA, 0x0224, 0x058B, 0x1245, 0x0A54, 0x1D9A, 
    0x050A, 0x12C4, 0x0AD5, 0x1D1B, 0x1AB4, 0x0D7A, 0x156B, 0x02A5, 
    0x1021, 0x07EF, 0x1FFE, 0x0830, 0x0F9F, 0x1851, 0x0040, 0x178E, 
    0x0F1E, 0x18D0, 0x00C1, 0x170F, 0x10A0, 0x076E, 0x1F7F, 0x08B1, 
    0x0E1C, 0x19D2, 0x01C3, 0x160D, 0x11A2, 0x066C, 0x1E7D, 0x09B3, 
    0x1123, 0x06ED, 0x1EFC, 0x0932, 0x0E9D, 0x1953, 0x0142, 0x168C, 
    0x0C18, 0x1BD6, 0x03C7, 0x1409, 0x13A6, 0x0468, 0x1C79, 0x0BB7, 
    0x1327, 0x04E9, 0x1CF8, 0x0B36, 0x0C99, 0x1B57, 0x0346, 0x1488, 
    0x1225, 0x05EB, 0x1DFA, 0x0A34, 0x0D9B, 0x1A55, 0x0244, 0x158A, 
    0x0D1A, 0x1AD4, 0x02C5, 0x150B, 0x12A4, 0x056A, 0x1D7B, 0x0AB5, 
    0x0810, 0x1FDE, 0x07CF, 0x1001, 0x17AE, 0x0060, 0x1871, 0x0FBF, 
    0x172F, 0x00E1, 0x18F0, 0x0F3E, 0x0891, 0x1F5F, 0x074E, 0x1080, 
    0x162D, 0x01E3, 0x19F2, 0x0E3C, 0x0993, 0x1E5D, 0x064C, 0x1182, 
    0x0912, 0x1EDC, 0x06CD, 0x1103, 0x16AC, 0x0162, 0x1973, 0x0EBD, 
    0x1429, 0x03E7, 0x1BF6, 0x0C38, 0x0B97, 0x1C59, 0x0448, 0x1386, 
    0x0B16, 0x1CD8, 0x04C9, 0x1307, 0x14A8, 0x0366, 0x1B77, 0x0CB9, 
    0x0A14, 0x1DDA, 0x05CB, 0x1205, 0x15AA, 0x0264, 0x1A75, 0x0DBB, 
    0x152B, 0x02E5, 0x1AF4, 0x0D3A, 0x0A95, 0x1D5B, 0x054A, 0x1284, 
  };

/**
*
* crc16Calc - calculate 16 bit CRC
* with table
*
* NOMANUAL
*
* \retval
*++ calculated CRC
*-- berechnete CRC
*
*/
UNSIGNED16 crc16Calc(
	UNSIGNED8 *buf,		/**< Start adress of data set in memory */
	UNSIGNED16 crc,		/**< value to begin with CRC calculation */
	UNSIGNED32 lng		/**< number of bytes to include in calculation*/
#  ifdef CONFIG_16BIT_CPU
	,BOOL_T	numeric		/**< data at buffer are numeric */
#  endif /* CONFIG_16BIT_CPU */
	)
{
UNSIGNED16	tcrc;		/* temporary U16 value */
#  ifdef CONFIG_16BIT_CPU
UNSIGNED8	val;		/* temporary U8 value */
UNSIGNED8	odd = 0;  	/* odd value */
#  endif /* CONFIG_16BIT_CPU */

    tcrc = crc;
    while (lng--) {
#  ifdef CONFIG_16BIT_CPU
	/* if numeric data field */
	if (numeric == CO_TRUE)  {
	    /* if odd address use high part */
	    if (odd != 0)  {
		val = (*(UNSIGNED16 *)buf >> 8) & 0xff;
		buf++;
		odd = 0;
	    } else {
		/* if even address use low part */
		val = *buf & 0xff;
		/* address is only incremented after odd addresses */
		odd ++;
	    }
	} else {
	    val = *buf & 0xff;
	    buf++;
	}
	tcrc = ((tcrc >> 8) & 0xff) ^ crc_table[(tcrc ^ val) & 0xff];
#  else /* CONFIG_16BIT_CPU */
	tcrc = ((tcrc >> 8) & 0xff) ^ crc_table[(tcrc ^ *buf++) & 0xff];
#  endif /* CONFIG_16BIT_CPU */
    }
    return tcrc;
}

# else /* CONFIG_BLOCK_CRC_TABLE */

/***************************************************************************
*
* crc16Calc - calculate 16 bit CRC
* without table
*
* NOMANUAL
*/

UNSIGNED16  crc16Calc(
	UNSIGNED8 *buf,		/**< Start adress of data set in memory */
	UNSIGNED16 crc,		/**< value to begin with CRC calculation */
	UNSIGNED32 lng		/**< number of bytes to include in calculation*/
#  ifdef CONFIG_16BIT_CPU
	,BOOL_T	numeric		/**< data at buffer are numeric */
#  endif /* CONFIG_16BIT_CPU */
	)
{
#define M16	0xA001		/* crc-16 mask (x^16 + x^15 +x^2 + 1) */
#define MTT	0x1021		/* crc-ccitt mask (x^16 + x^12 + x^5 + 1) */
UNSIGNED8	carry, b;
UNSIGNED8	cnt;
#  ifdef CONFIG_16BIT_CPU
UNSIGNED8	val;
UNSIGNED8	odd = 0;  
#  endif /* CONFIG_16BIT_CPU */

    while (lng)
    {
#  ifdef CONFIG_16BIT_CPU
	/* if numeric data field */
	if (numeric == CO_TRUE)  {
	    /* if odd address use high part */
	    if (odd != 0)  {
		b = (*(UNSIGNED16 *)buf >> 8) & 0xff;
		buf++;
		odd = 0;
	    } else {
		/* if even address use low part */
		b = *buf & 0xff;
		/* address is only incremented after odd addresses */
		odd ++;
	    }
	} else
#  endif /* CONFIG_16BIT_CPU */
	{
	    b = *buf & 0xff;
	    buf++;
	}
	/* b = (UNSIGNED8) *buf++; */
	crc ^= b;
	cnt = 0;
	while (cnt < 8)
	{
	    carry =(UNSIGNED8) crc & 0x01;
	    crc >>= 1;
	    if (carry) {
		crc ^= MTT;
	    }
	    cnt++;
	}
	lng--;
    }
    return (crc);
}
# endif /* CONFIG_BLOCK_CRC_TABLE */
#endif /* (CONFIG_BLOCK_CRC) || defined(CONFIG_SRDO_PRODUCER) || defined(CONFIG_SRDO_CONSUMER */


#ifdef CONFIG_8BIT_CRC
/****************************************************************************/
/**
*++ \brief CRC calculation algorithm
*-- \brief CRC Summen Berechnung
*
*++ This function calculates the checksum
*++ The check polynom has the formula
*-- Diese Funktion berechnet die Checksumme
*-- Die Prüfsumme wird nach dem Polynom
* x^8 + x^3 + x^2 + x + 1
*-- berechnet.
*
*++ \internal
*++    The calculation is performed in little-endian format.
*-- \internal
*--    Die Berechnung wird im little-endian Format durchgeführt.
*
* \return
*++ 8 bit CRC sum
*-- 8 Bit CRC Summe
*/
UNSIGNED8 crc8Calc(
	UNSIGNED8 *buf,		/**< Start adress of data set in memory */
	UNSIGNED8 crc,		/**< value to begin with CRC calculation */
	UNSIGNED32 lng		/**< number of bytes to include in calculation*/
	)
{
#define CRC8_POL 0x4F
UNSIGNED8 carry, b;
UNSIGNED8 cnt;

    while (lng > 0) {

	b = (UNSIGNED8) *buf++;
	crc ^= b;
	cnt = 0;
	while (cnt < 8)
	{
	    carry =(UNSIGNED8) crc & 0x80;
	    crc <<= 1;
	    if (carry) {
	    	crc ^= CRC8_POL;
	    }
	    cnt++;
	}
	lng--;
    }
    return (crc);
}
#endif /* CONFIG_8BIT_CRC */


#ifdef CONFIG_16BIT_CPU
/***************************************************************************
*
* unpack_memcpy - special memcpy from packed values to unpacked values
*
*
* NOMANUAL
*
* Only valid for 16bit CPUs
* Copy wordwise to bytewise if numeric is true
* (p.e. from CAN-buffer to internal variables)
*
* \retval
*	nothing
*/
void unpack_memcpy(
	UNSIGNED8 *dest,	/**< destination area (bytewise) */
	UNSIGNED8 *src,		/**< source area (wordwise) */
	UNSIGNED32 size,	/**< size in bytes */
	UNSIGNED8 numeric	/**< numeric flag */
	)
{ 
UNSIGNED32	i;		/* loop counter */ 

    if (numeric != 0)  {
	for (i = 0; i < size; i++)  {		
	    if ((i & 1) != 0) {
		dest[i] = ((UNSIGNED16*)src)[i>>1] >> 8;	
	    }  else  {
		dest[i] = ((UNSIGNED16*)src)[i>>1] & 0xFF;	
	    }
	}
    } else  {
	memcpy((void *)dest, (void *)src, size);
    }
}


/***************************************************************************
*
* pack_memcpy - special memcpy from unpacked values to packed values
*
* 
* NOMANUAL
*
* Only valid for 16bit CPUs
* Copy bytewise to wordwise if numeric is true
* (p.e. from CAN-buffer to internal variables)
*
* \retval
*	nothing
*/

void pack_memcpy(
	UNSIGNED8 *dest,	/**< destination area (wordwise) */
	UNSIGNED8 *src,		/**< source area (bytewise) */
	UNSIGNED32 size,	/**< size in bytes */
	UNSIGNED8 numeric	/**< numeric flag */
	)
{ 
UNSIGNED32	i;		/* loop counter */

    if (numeric != 0)  {
	for (i = 0; i < size; i++)  {		
	    if ((i & 1) != 0) {
		((UNSIGNED16 *)dest)[i >> 1] &= 0xFF;
		((UNSIGNED16 *)dest)[i >> 1] |= src[i] << 8;	
	    } else  {
		((UNSIGNED16 *)dest)[i >> 1] &= 0xFF00;
		((UNSIGNED16 *)dest)[i >> 1] |= src[i] & 0xFF;	
	    }
	}
    } else  {
	memcpy((void *)dest, (void *)src, size);
    }
}


# ifdef CONFIG_SEG_SDO
/***************************************************************************
*
* pack_oddmemcpy - special memcpy from unpacked values to packed values
*			for odd adresses or sizes
*
* 
* NOMANUAL
*
* Only valid for 16bit CPUs
* Copy bytewise to wordwise with odd byte counts
* If odd is set, copy first byte of src-address to upper byte of dest-address
*
* \retval
*	actual dest pointer as return value
*	odd
*/

UNSIGNED8 *pack_oddmemcpy(
	UNSIGNED8 *dest,	/**< destination area (wordwise) */
	UNSIGNED8 *src,		/**< source area (bytewise) */
	UNSIGNED32 size,	/**< size in bytes */
	BOOL_T	*odd		/**< start at odd address */
	)
{ 
    /* start at odd address ? */
    if (*odd == CO_TRUE)  {
	/* copy first byte of src-address to upper byte of dest-address */
	(*(UNSIGNED16 *)dest) &= 0x00FF;
	(*(UNSIGNED16 *)dest) |= ((UNSIGNED16)*src) << 8;	
	dest ++;
	src ++;
	size --;
    }
    /* now copy all even bytes */
    CO_PACK_MEMCPY(dest, src, size, CO_TRUE);
    dest += (size >> 1);

    /* copy last odd byte */
    if ((size & 1) != 0)  {
	*odd = CO_TRUE;
	(*(UNSIGNED16 *)dest) &= 0xFF00;
	(*(UNSIGNED16 *)dest) |= *(src + size - 1);
    } else {
	*odd = CO_FALSE;
    }

    return(dest);
}


/***************************************************************************
*
* unpack_oddmemcpy - special memcpy from packed values to unpacked values
*			for odd adresses or sizes
*
* 
* NOMANUAL
*
* Only valid for 16bit CPUs
* Copy wordwise to bytewise with odd byte counts
* If odd is set, copy lower byte of src-address to first dest address
*
* \retval
*	actual src pointer as return value
*	odd
*/

UNSIGNED8 *unpack_oddmemcpy(
	UNSIGNED8 *dest,	/**< destination area (bytewise) */
	UNSIGNED8 *src,		/**< source area (wordwise) */
	UNSIGNED32 size,	/**< size in bytes */
	BOOL_T	  *odd		/**< start at odd address */
	)
{ 
    /* start at odd address ? */
    if (*odd == CO_TRUE)  {
	/* copy first byte of src-address to upper byte of dest-address */
	*dest = *((UNSIGNED16 *)src) >> 8;	
	src ++;
	dest ++;
	size --;
    }
    /* now copy all even bytes */
    CO_UNPACK_MEMCPY(dest, src, size, CO_TRUE);
    src += (size >> 1);

    /* copy last odd byte */
    if ((size & 1) != 0)  {
	*(dest + size - 1) = (*(UNSIGNED16 *)src) & 0xff;
	*odd = CO_TRUE;
    } else {
	*odd = CO_FALSE;
    }

    return(src);
}

# endif /* CONFIG_SEG_SDO */
#endif /* CONFIG_16BIT_CPU */

/*______________________________________________________________________EOF_*/
