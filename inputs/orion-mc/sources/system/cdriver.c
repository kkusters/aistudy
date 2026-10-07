/*
 * cdriver - common CAN driver functions
 *
 * Copyright (c) 1997-2002 port GmbH Halle (Saale)
 *------------------------------------------------------------------
 * $Id$
 *
 * modification history
 * --------------------
 * $Log$
 * Revision 1.0  2011-07-18 11:49:00+02  driet
 * Initial revision
 *
 * Revision 1.1  2008-03-19 17:04:47+01  driet
 * <>
 *
 * Revision 1.0  2008-03-06 17:15:15+01  driet
 * Initial revision
 *
 * Revision 2.40  2003/08/20 11:51:21  ro
 * undef putchar - before define the function putchar()
 *
 * Revision 2.39  2003/08/04 13:19:32  ro
 * simple putchar() from Keil Compiler as an empty function added
 *
 * Revision 2.38  2003/07/30 11:47:24  ro
 * ENABLE_CPU_INTERRUPTS() changed to RESTORE_CPU_INTERRUPTS()
 *
 * Revision 2.37  2003/07/22 15:18:55  ro
 * empty stdio-functions for mode CONFIG_NO_PRINTF added
 * disable interrupts during BUFFER_ENTRY_INCR()
 *
 * Revision 2.36  2003/05/07 16:18:00  ro
 * some backslashes added after linefeeds within macro calls
 *
 * Revision 2.35  2003/03/10 08:13:54  ro
 * Time Test support added
 *
 * Revision 2.34  2003/02/11 15:15:27  ro
 * comment corrected
 *
 * Revision 2.33  2003/01/20 16:19:50  ro
 * a bracket added to the CO_MEMCPY() call - no functionality changed
 *
 * Revision 2.32  2002/12/18 09:07:27  ro
 * validCobId() corrected
 *
 * Revision 2.31  2002/12/11 14:36:14  ro
 * driver concept changed,
 * adapt to library version 4.3
 *
 * now using RESTORE_xxx_INTERRUPTS()
 * compareTime() removed - new timer concept
 *
 * Revision 2.30  2002/02/21 16:37:39  ro
 * COB List as Array
 * COB ID Index List for Valid ID checking
 * Binary Search in validCobId()
 *
 * Revision 2.29  2002/01/25 16:13:06  ro
 * break sending Messages in move_TxBuffer()
 *
 * Revision 2.28  2001/10/09 09:18:35  ro
 * recalcTimerEvent() for NEW_TIMER added
 * correct first databyte of NMT Error Control object after bootup message
 *
 * Revision 2.27  2001/05/18 13:44:53  boe
 * volatile replaced by macro VOLATILE
 *
 * Revision 2.26  2001/05/15 14:47:22  ro
 * blank parameter list changed to void
 *
 * Revision 2.25  2001/04/05 08:53:32  boe
 * comment changed
 *
 * Revision 2.24  2001/03/28 13:46:36  ro
 * comment format changed
 *
 * Revision 2.23  2001/03/13 14:56:17  ro
 * standard bufferhandling only if CONFIG_STANDARD_BUFFER is set
 * getNumberOfRxMessages added
 * getNumberOfTxMessages corrected
 * move_TxBuffer for Redundancy Support added
 * function calls replaced with macro calls
 *
 * Revision 2.22  2001/01/31 10:51:22  ro
 * #include's added
 * msgIdentification call changed
 *
 * Revision 2.21  2001/01/15 09:58:45  ro
 * Message-ID calculation corrected
 *
 * Revision 2.20  2000/11/10 10:21:14  ro
 * new marcos for communication buffer access
 * BasicCAN mode readjusted
 *
 * Revision 2.18  2000/10/18 09:53:31  ro
 * CAN_Msg for Multiline enhanced
 *
 * Revision 2.17  2000/07/31 10:09:46  boe
 * memcpy replaced with macro CO_MEMCPY
 * function flushMbox only declared if CONFIG_STANDARD_FLUSHMBOX is set
 *
 * Revision 2.16  2000/07/21 06:48:51  ro
 * PEAK Dongle Anpassung
 *
 * Revision 2.15  2000/07/11 16:14:09  boe
 * buffer handling modified
 *
 * Revision 2.14  2000/07/04 15:10:27  boe
 * define insert_tx_request without RTX51_modifier and without NOAREGS
 * compare_time function changed
 *
 * Revision 2.13  2000/06/23 10:39:58  oe
 * Rework for Reference Manual
 *
 * Revision 2.12  2000/06/21 10:27:19  boe
 * function getNumberOfTxMessages() changed
 *
 * Revision 2.11  2000/06/06 09:02:21  ro
 * Ergänzung der deutschen Handbuchkommentare
 *
 * Revision 2.10  2000/05/31 10:23:46  boe
 * external declarations moved to cdriver.h
 *
 * Revision 2.9  2000/05/31 10:16:34  boe
 * compiler warning removed
 *
 * Revision 2.8  2000/05/09 09:22:09  oe
 * - for KEIL-C --> use some more "data" variable
 * - checkTxBuffer() and getNumberOfTxMessages() added
 *
 * Revision 2.6  2000/04/18 08:58:40  boe
 * special datatype DATA removed, FlushMbox for MSC removed
 *
 * Revision 2.5  2000/04/04 08:49:01  boe
 * comment removed
 *
 * Revision 2.4  2000/03/28 14:53:31  boe
 * adaption for multi-line version
 *
 * Revision 2.3  2000/03/15 16:31:13  boe
 * Multilineversion überarbeitet
 *
 * Revision 2.2  2000/02/16 14:03:53  boe
 * undefine for FlushMbox for layer 2 hardware
 *
 * Revision 2.1  2000/01/26 15:32:14  boe
 * Anpassung an Netporty
 *
 * Revision 2.0  2000/01/21 13:15:33  boe
 * Überarbeitet und an Version 4.0 angepasst
 *
 *
 *------------------------------------------------------------------
 */

/**
*  \file cdriver.c
*  \author port GmbH Halle (Saale)
*  $Revision$
*  $Date$
*
*++ This module provides some functions used by all
*++ drivers for CAN controllers.
*-- Dieses Modul stellt Funktionen, welche von allen
*-- Treibern für CAN Controller benutzt werden,
*-- bereit.
*
*++ This modul defines also all global variables used within the driver.
*-- Dieses Module definiert auch alle globalen Variablen, welche in
*-- den Treibern benutzt werden.
*
*++ Mostly these are functions for manipulating the send
*++ and receive queue.
*-- Die meisten dieser Funktionen manipulieren den Sende- und
*-- Empfangspuffer.
*
*
*/
/**
* \def CONFIG_COLIB_BUFFER
*++ Our default Bufferhandling is used.
*-- Unser Standard Bufferhandling wird benutzt.
*
* \def CONFIG_COLIB_FLUSHMBOX
*++ enable our standard FlushMbox() function
*-- aktiviert unsere Standard FlushMbox() Funktion
*
* \def CONFIG_TEST_VALID_COB
*++ enable the validCobId() function
*-- aktiviert die validCobId() Funktion
*/



#ifdef CONFIG_NO_PRINTF
# include <stdio.h>
#endif /* CONFIG_NO_PRINTF */
#include <string.h>

#include <cal_conf.h>


#include <co_stru.h>
#include <co_flag.h>
#include <co_drv.h>
#include <co_drvif.h>
#include <co_debug.h>
#include <co_timer.h>

#include <cdriver.h>

#ifdef CONFIG_SYNC_CONSUMER
# ifdef CONFIG_FULLCAN
# else /* CONFIG_FULLCAN */
#  include <co_sync.h>
# endif /* CONFIG_FULLCAN */
#endif /* CONFIG_SYNC_CONSUMER */

#include "ch_can_backbone_hardware.h"

#ifdef CONFIG_REDUNDANCY_SUPPORT
#  ifndef DISABLE_TX_MESSAGES
#    define DISABLE_TX_MESSAGES(line)
#  endif /* DISABLE_TX_MESSAGES */
#endif /* CONFIG_REDUNDANCY_SUPPORT */

/*---- const -------------------------------------------------*/
/** \var co_bittiming_table
*++ Table of supported CAN bitrates as defined in CANopen.
*++ The order follows the specification DSP 305.
*-- Tabelle aller in CANopen verfügbaren CAN-Bitraten.
*-- Die Reihenfolge entspricht dem DSP 305.
*/
CO_CONST UNSIGNED16 co_bittiming_table[9] = {
    1000, 800, 500, 250, 125, 100, 50, 20, 10
};

/*---- externals -------------------------------------------------*/

/*---- global variables ------------------------------------------*/
/** start pointer for COB list */
COB_T     *co_pFirst_COB_Entry CO_LINE_PARA_ARRAY_DEF;
/** buffer for CAN message */
CAN_MSG_T CAN_Msg CO_LINE_PARA_ARRAY_DEF;

#ifdef CONFIG_COLIB_BUFFER

VOLATILE BUFFER_ENTRY_T pTX_Buffer[CONFIG_TX_BUFFER_SIZE] CO_LINE_PARA_ARRAY_DEF;
VOLATILE BUFFER_ENTRY_T pRX_Buffer[CONFIG_RX_BUFFER_SIZE] CO_LINE_PARA_ARRAY_DEF;


VOLATILE UNSIGNED8 CO_DATA
            bRX_WriteIndex CO_LINE_PARA_ARRAY_DEF,
            bRX_ReadIndex  CO_LINE_PARA_ARRAY_DEF,
                bTX_WriteIndex CO_LINE_PARA_ARRAY_DEF,
                bTX_ReadIndex  CO_LINE_PARA_ARRAY_DEF;

#endif /* CONFIG_STANDARD_BUFFER */

#ifdef CONFIG_COB_ARRAY
#  ifdef CONFIG_COB_NUMBERS
#  else /* CONFIG_COB_NUMBERS */
#    error "Number of CAN Objects is absent (CONFIG_COB_NUMBERS)!"
#  endif /* CONFIG_COB_NUMBERS */
COB_T cob_list[CONFIG_COB_NUMBERS];
UNSIGNED8 cob_index_list[CONFIG_COB_NUMBERS];
UNSIGNED8 cob_list_next_entry;
#endif /* CONFIG_COB_ARRAY */

#ifdef CONFIG_COLIB_BUFFER
/*******************************************************************/
/**
*
*++ \brief clearTxBuffer - clears the CAN transmit buffer
*-- \brief clearTxBuffer - löscht den CAN Sendepuffer
*
* \returns
*++ nothing
*-- nichts
*
*/
void clearTxBuffer(
#ifdef CONFIG_MULT_LINES
     UNSIGNED8 canLine /**< number of CAN line 0..CONFIG_MULT_LINES-1 */
# else
    CO_LINE_PARA_DECL
#endif
     )
{
UNSIGNED8 i;

    Disable_CAN_Interrupts(CO_LINE_PARA);

    bTX_WriteIndex CO_LINE_PARA_ARRAY_INDEX =
    bTX_ReadIndex CO_LINE_PARA_ARRAY_INDEX = 0;
    
    for (i = 0; i < CONFIG_TX_BUFFER_SIZE; i++)
    {
      pTX_Buffer[i] CO_LINE_PARA_ARRAY_INDEX .eStat = EMPTY;
    }   
    
    Restore_CAN_Interrupts(CO_LINE_PARA);
}


/*******************************************************************/
/**
*
*++ \brief clearRxBuffer - clears the CAN receive buffer
*-- \brief clearRxBuffer - löscht den CAN Emfangspuffer
*
*
*
* \returns
*++ nothing
*-- nichts
*
*/
void clearRxBuffer(
#ifdef CONFIG_MULT_LINES
     UNSIGNED8 canLine /**< number of CAN line 0..CONFIG_MULT_LINES-1 */
# else
    CO_LINE_PARA_DECL
#endif
     )
{
UNSIGNED8 i;

    Disable_CAN_Interrupts(CO_LINE_PARA);
    
    bRX_WriteIndex CO_LINE_PARA_ARRAY_INDEX = bRX_ReadIndex CO_LINE_PARA_ARRAY_INDEX = 0;
        
    for (i = 0; i < CONFIG_RX_BUFFER_SIZE; i++)
    {
      pRX_Buffer[i] CO_LINE_PARA_ARRAY_INDEX .eStat = EMPTY;
    }
    
    Restore_CAN_Interrupts(CO_LINE_PARA);
}


/*******************************************************************/
/**
*
*++ \brief checkTxBuffer - returns the transmit buffer status
*-- \brief checkTxBuffer - gibt den Status des Sendepuffers zurück
*
*
*
* \retval
*++ CO_TRUE - transmit buffer empty
*-- CO_TRUE - Sendepuffer frei
* \retval
*++ CO_FALSE - transmit buffer full
*-- CO_FALSE - Sendepuffer voll
*
*/
BOOL_T checkTxBuffer(
#ifdef CONFIG_MULT_LINES
     UNSIGNED8 canLine /**< number of CAN line 0..CONFIG_MULT_LINES-1 */
# else
    CO_LINE_PARA_DECL
#endif
     )
{
    if (pTX_Buffer[bTX_WriteIndex CO_LINE_PARA_ARRAY_INDEX ]
        CO_LINE_PARA_ARRAY_INDEX .eStat == FULL) 
    {
    return(CO_FALSE);
    } else  {
    return(CO_TRUE);
    }
}


/*******************************************************************/
/**
*
*++ \brief getNumberOfTxMessages - delivers the number of messages in TX queue
*-- \brief getNumberOfTxMessages - liefert die Anzahl von Nachrichten in der Sendequeue
*
* \returns
*++ number of messages in TX queue
*-- Anzahl der Nachrichten in der Sende-Queue
*
*/
UNSIGNED8 getNumberOfTxMessages(
#ifdef CONFIG_MULT_LINES
     UNSIGNED8 canLine /**< number of CAN line 0..CONFIG_MULT_LINES-1 */
# else
    CO_LINE_PARA_DECL
#endif
     )
{
UNSIGNED8   cnt;

    Disable_CAN_Interrupts(CO_LINE_PARA);
    
    if(    bTX_WriteIndex CO_LINE_PARA_ARRAY_INDEX 
        >= bTX_ReadIndex CO_LINE_PARA_ARRAY_INDEX) 
    {

    cnt = bTX_WriteIndex CO_LINE_PARA_ARRAY_INDEX
                - bTX_ReadIndex CO_LINE_PARA_ARRAY_INDEX;
    } else {
    cnt = CONFIG_TX_BUFFER_SIZE
            - (  bTX_ReadIndex CO_LINE_PARA_ARRAY_INDEX
                   - bTX_WriteIndex CO_LINE_PARA_ARRAY_INDEX);
    }
    
    /* WriteIndex == ReadIndex : Is Buffer empty or full ? */
    if ( (cnt == 0) && (pTX_Buffer[ bTX_WriteIndex CO_LINE_PARA_ARRAY_INDEX ] 
                        CO_LINE_PARA_ARRAY_INDEX .eStat == FULL) ) 
    { 
    cnt = CONFIG_TX_BUFFER_SIZE;
    }    
    
    Restore_CAN_Interrupts(CO_LINE_PARA);
    return(cnt);
}


/*******************************************************************/
/**
*
*++ \brief getNumberOfRxMessages - delivers the number of messages in RX queue
*-- \brief getNumberOfRxMessages - liefert die Anzahl der Nachrichten in der Empfangsqueue
*
* \returns
*++ number of messages in RX queue
*-- Anzahl der Nachrichten in der Empfangs-Queue
*
*/
UNSIGNED8 getNumberOfRxMessages(
#ifdef CONFIG_MULT_LINES
     UNSIGNED8 canLine /* number of CAN line 0..CONFIG_MULT_LINES-1 */
# else
    CO_LINE_PARA_DECL
#endif
     )
{
UNSIGNED8   cnt;

    Disable_CAN_Interrupts(CO_LINE_PARA);
    
    if(    bRX_WriteIndex CO_LINE_PARA_ARRAY_INDEX 
        >= bRX_ReadIndex CO_LINE_PARA_ARRAY_INDEX) 
    {

    cnt = bRX_WriteIndex CO_LINE_PARA_ARRAY_INDEX
                - bRX_ReadIndex CO_LINE_PARA_ARRAY_INDEX;
    } else {
    cnt = CONFIG_RX_BUFFER_SIZE
            - (  bRX_ReadIndex CO_LINE_PARA_ARRAY_INDEX
                   - bRX_WriteIndex CO_LINE_PARA_ARRAY_INDEX);
    }
    
    /* WriteIndex == ReadIndex : Is Buffer empty or full ? */
    if ( (cnt == 0) && (pRX_Buffer[ bRX_WriteIndex CO_LINE_PARA_ARRAY_INDEX ] 
                        CO_LINE_PARA_ARRAY_INDEX .eStat == FULL) ) 
    { 
        cnt = CONFIG_RX_BUFFER_SIZE;
    }    
    
    Restore_CAN_Interrupts(CO_LINE_PARA);
    return(cnt);
}


/*******************************************************************/
/**
*++ \brief Insert_TX_Request - inserts the next transm. request into queue
*-- \brief Insert_TX_Request - fügt einen Transmission Request in den Puffer ein.
*
* \returns
*++ nothing
*-- nichts
*
*/

void Insert_TX_Request(COB_T      *pCOB,  // pointer to COB to be transmitted
                       UNSIGNED8  *pData) // pointer to message data
{
#ifdef CONFIG_MULT_LINES
UNSIGNED8 canLine = pCOB->canLine;
#endif

  CHECK_BUFFER_WRITE( TX, COFLAG_BUFFER_OVERFLOW )
  {
    Disable_CAN_Interrupts(CO_LINE_PARA);

    BUFFER_WRITE( TX, bChannel, pCOB->bChannel);

#if defined (CONFIG_FULLCAN) && !defined(CONFIG_ONLY_ONE_TRANSMIT_CHANNEL)
#else
    BUFFER_WRITE( TX, wCOB_ID, pCOB->wID);
#endif

    BUFFER_WRITE( TX, bLength, pCOB->bLength);

    BUFFER_WRITE( TX, eType, pCOB->eType);

    CO_MEMCPY((void*)&pTX_Buffer[bTX_WriteIndex CO_LINE_PARA_ARRAY_INDEX ] CO_LINE_PARA_ARRAY_INDEX .pData[0], (const void*)pData, pCOB->bLength);
                                 
    // Do this changes atomic.
    BUFFER_ENTRY_INCR( TX, Write, FULL);

    Restore_CAN_Interrupts(CO_LINE_PARA);
  }
}

# ifdef CONFIG_REDUNDANCY_SUPPORT
/*******************************************************************/
/**
*++ \brief move_TXBuffer - move all Messages from a line to a other line
*-- \brief move_TxBuffer - verschiebt alle Messages von einer in eine andere Linie 
*
*
* \returns 
*++ nothing
*-- nichts
*/

void move_TxBuffer( 
    UNSIGNED8 targetLine, /**< target CAN line */
    UNSIGNED8 sourceLine  /**< source CAN line */
)
{
UNSIGNED8 canLine = sourceLine;
COB_T cob;

    DISABLE_TX_MESSAGES(sourceLine); /* cancel sending of Messages */
    
    DISABLE_CAN_INTERRUPTS(sourceLine);
    while(1) {
        CHECK_BUFFER_READ( TX )
        {
        cob.wID = BUFFER_READ( TX, wCOB_ID ); /* only, if isn't C505C / C515C */ 
        cob.bLength = BUFFER_READ( TX, bLength );
        cob.bChannel = BUFFER_READ( TX, bChannel ); 
        cob.canLine = targetLine;
        cob.eType = BUFFER_READ( TX, eType );
        cob.pNext = NULL;
        cob.pNextLine = NULL;

        /* !!! don't use Transmit_COB_Redundancy !!! */
        /* Transmit_COB enable CAN-Interrupts !!! */
        Transmit_COB( &cob, &pTX_Buffer[ bTX_ReadIndex[sourceLine] ] 
                                [sourceLine].pData[0]); 

        DISABLE_CPU_INTERRUPTS();
        /* Do this changes atomic. */
            BUFFER_ENTRY_INCR( TX , Read , EMPTY);
        RESTORE_CPU_INTERRUPTS();

        } else {
            break;
        }    
    }    
    RESTORE_CAN_INTERRUPTS(sourceLine);
}
# endif /* CONFIG_REDUNDANCY_SUPPORT */
#endif /* CONFIG_STANDARD_BUFFER */

#ifdef CONFIG_TEST_VALID_COB
/*******************************************************************/
/**
*++ validCobId() - checks whether the COB-ID of a received message is valid
*-- validCobId() - prueft die Gültigkeit der COB-IDs emfangener Nachrichten
*
*++ This function checks whether the COB-ID of a received message is valid
*++ for the local node.
*-- Diese Funktion prueft die Gültigkeit der COB-IDs emfangener Nachrichten
*-- für den localen Netzwerkknoten.
*
* \retval CO_TRUE
*++COB-ID is valid
*--COB-ID ist gueltig
*
* \retval CO_FALSE
*++COB-ID isn't valid
*--COB-ID ist ungueltig
*
*/

BOOL_T validCobId(
       UNSIGNED16 cobId,    /**< COB-ID */
       UNSIGNED8  rtrFlag   /**< RTR-BIT value [0 or !=0] */
# ifdef CONFIG_MULT_LINES
       ,UNSIGNED8 canLine   /**< number of CAN line 0..CONFIG_MULT_LINES-1 */
# endif
       )
{
# ifdef CONFIG_COB_ARRAY
BOOL_T found = CO_FALSE;
UNSIGNED8 start = 0;
UNSIGNED8 end = cob_list_next_entry;
UNSIGNED8 middle;
UNSIGNED16 wID;
COB_KIND_T eType;
COB_T   * pCOB;

    while( start != end ) {
        middle = ((end - start) >> 1) + start;

    /* wID = cob_list[ *(cob_index_list + middle) ].wID; */
    pCOB =(COB_T*)((UNSIGNED8*)&cob_list[0] + \
            ((*(cob_index_list + middle)) * sizeof(COB_T))); 
    wID = pCOB->wID;

        if(cobId < wID) {
            end = middle;
        } else {
        if(cobId == wID) {
            found = CO_TRUE;
            break;
        }
            /* check for end = start + 1 */
        if (start == middle)  {
        break;
        }

        start = middle;
    }
    }   

    if( found == CO_FALSE ) {
        return CO_FALSE;
    }    

    /* index middle is the found COB-ID */
    /* cob_list[cob_index_list[middle]].eType */
    /* eType = cob_list[*(cob_index_list + middle)].eType; */
    if(rtrFlag != 0) 
    {
    eType = pCOB->eType;
        if ((eType & CO_COB_RTR) == 0)
    {
        return CO_FALSE;
    } else {
        return CO_TRUE;
    }
    }        
    return CO_TRUE;

#else /* CONFIG_COB_ARRAY */
COB_T * pCur;

    pCur = co_pFirst_COB_Entry CO_LINE_PARA_ARRAY_INDEX;
    while(pCur != NULL)
    {
    if( pCur->wID == cobId )
    {
        /* if RTR - TX Object test for RTR bit */
        if( (rtrFlag != 0)
            && ((pCur->eType & CO_COB_RTR) == 0))
        {   
        return CO_FALSE;
        } else {
        return CO_TRUE;
        }   
    }
    pCur = pCur->pNext;
    }
    return CO_FALSE;
#endif    /* CONFIG_COB_ARRAY */
}
#endif /* CONFIG_TEST_VALID_COB */


#ifdef CONFIG_COLIB_FLUSHMBOX
/*******************************************************************/
/**
*
*++ \brief FlushMbox - read can messages from buffer
*-- \brief FlushMbox - Auslesen des Message Emfangspuffers
*
*++ This function should be called cyclically or whenever
*++ the user is expecting a response to any request.
*++ The function flushes the message buffer filled by
*++ the CAN interrupt routine and starts
*++ processing of the messages.
*-- Diese Funktion sollte zyklisch aufgerufen werden
*-- oder wann immer die Applikation die Antwort auf einen
*-- CANopen-Request
*-- erwartet.
*-- Der von der Empfangs Interruptservice Routine gefüllte Puffer
*-- wird geleert und nacheinender immer eine Message
*-- zur Auswertung an eine interne CANopen Routine übergeben.
* 
*++ CAN interrupts are disabled while correcting buffer pointers.
*-- Die CAN Interrupte werden während der Korrektur der Empfangspuffer
*-- Zeiger gesperrt.
*
* \returns
*++ nothing
*-- nichts
*
*/
void FlushMbox(
# ifdef CONFIG_MULT_LINES
     UNSIGNED8 canLine // number of CAN line 0..CONFIG_MULT_LINES-1
# else
    CO_LINE_PARA_DECL
# endif
     )
{
  // first test the library flags and work it
  if (TEST_COLIB_FLAG(COFLAG_ALL))
  {
    FLAG_IDENTIFICATION(CO_LINE_PARA);
  }

  // now look at the buffer for new can data
  while ( BUFFER_READ( RX, eStat) == FULL)
  { 
    Disable_CAN_Interrupts(CO_LINE_PARA);
# if defined(CONFIG_CPU_FAMILY_8051) && defined(CONFIG_CAN_FAMILY_82527)
    /* ID shifting from internal registers to normal int
     * is not done within ISR but here.
     * (It's time consuming with an 8051)
     */
    CAN_Msg CO_LINE_PARA_ARRAY_INDEX .wCOB_ID = BUFFER_READ( RX, wCOB_ID) >> 5;
# else // defined(CONFIG_CPU_FAMILY_8051) && defined(CONFIG_CAN_FAMILY_82527)
    CAN_Msg CO_LINE_PARA_ARRAY_INDEX .wCOB_ID = BUFFER_READ( RX, wCOB_ID);
# endif // defined(CONFIG_CPU_FAMILY_8051) && defined(CONFIG_CAN_FAMILY_82527)

    CAN_Msg CO_LINE_PARA_ARRAY_INDEX .length = BUFFER_READ( RX, bLength);

# ifdef CONFIG_DRIVER_TEST
    PRINTF("%x:%d:", (int)CAN_Msg CO_LINE_PARA_ARRAY_INDEX .wCOB_ID, (int)CAN_Msg CO_LINE_PARA_ARRAY_INDEX .length  );
# endif // CONFIG_DRIVER_TEST

    CO_MEMCPY((void*)&(CAN_Msg CO_LINE_PARA_ARRAY_INDEX .pData[0]), (const void*)&(pRX_Buffer[bRX_ReadIndex CO_LINE_PARA_ARRAY_INDEX] CO_LINE_PARA_ARRAY_INDEX .pData[0]), 8);

    // Do this changes atomic.
    BUFFER_ENTRY_INCR( RX, Read, EMPTY);

    Restore_CAN_Interrupts(CO_LINE_PARA);

# ifdef CONFIG_FULLCAN
# else // CONFIG_FULLCAN
#  ifdef CONFIG_SYNC_CONSUMER
    if ((CO_COB_ID_SYNC != 0) && (CAN_Msg CO_LINE_PARA_ARRAY_INDEX .wCOB_ID == (CO_COB_ID_SYNC & CAN_11_BIT_ID_MASK))) 
    {
      SET_COLIB_FLAG(COFLAG_SYNC_RECEIVED);
      FLAG_IDENTIFICATION(CO_LINE_PARA);
    }
    else 
#  endif // CONFIG_SYNC_CONSUMER
# endif // CONFIG_FULLCAN
    {
      MSG_IDENTIFICATION(&CAN_Msg CO_LINE_PARA_ARRAY_INDEX);
    } // sync
  } // while Buffer full
}
#endif // CONFIG_COLIB_FLUSHMBOX

#ifdef CONFIG_NO_PRINTF
/*******************************************************************/
/*
 * empty stdio functions - only needed if you use our example without printf()
 * support.
 * For your application you should disable the define CONFIG_NO_PRINTF
 * and should remove all references to this functions.
 *
 */
int fprintf(FILE *stream, const char *format, ...)
{
    return 0;
}

int printf(const char *format, ...) 
{
    return 0;
}

# ifdef putchar
#  undef putchar
# endif

# ifdef PUTCHAR_CHAR
char putchar( char c)
# else
int putchar( int c )
# endif
{
    return c;
}

int fputc( int c, FILE * stream)
{
    return c;
}

int fflush( FILE * stream)
{
    return 0;
}

/*******************************************************************/
#endif /* CONFIG_NO_PRINTF */

/*______________________________________________________________________EOF_*/
