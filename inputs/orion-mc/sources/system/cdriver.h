/*
 * cdriver - includes for common CAN driver functions
 *
 * Copyright (c) 2000-2002 port GmbH Halle (Saale)
 *------------------------------------------------------------------
 * $Id$
 *
 * modification history
 * --------------------
 * $Log$
 * Revision 1.0  2011-07-18 11:49:01+02  driet
 * Initial revision
 *
 * Revision 1.1  2009-09-21 13:09:23+02  poelj
 * <>
 *
 * Revision 1.0  2008-03-06 17:16:40+01  driet
 * Initial revision
 *
 * Revision 1.19  2003/07/11 11:32:05  ro
 * add 'do{}while(0)' to larger macros
 *
 * Revision 1.18  2003/07/02 09:23:05  ro
 * first steps to support small memory model
 * (for-loop and direct transfer work with FAR pointers)
 *
 * Revision 1.17  2003/01/20 16:24:43  ro
 * CONFIG_CAN_USE_DIRECTTRANSFER corrected
 * BUFFER_ENTRY_T disabled if CONFIG_COLIB_BUFFER isn't set
 *
 * Revision 1.16  2002/12/18 09:59:53  ro
 * preprocessor spaces reworked
 *
 * Revision 1.15  2002/12/11 14:21:42  ro
 * driver concept changed,
 * adapt to library version 4.3
 *
 * CONFIG_DRIVER_TRACE_ENABLE removed
 * BUFFER_ADDR added
 * a lot of defines renamed
 *
 * Revision 1.14  2001/11/19 15:33:24  ro
 * default ADDR_T changed to (UNSIGNED8 FAR *)
 *
 * Revision 1.13  2001/04/04 07:23:26  ro
 * CONFIG_DRIVER_USE_DIRECTTRANSFER - BUFFER_READ_CPY and BUFFER_WRITE_CPY corrected
 *
 * Revision 1.12  2001/03/28 13:24:17  ro
 * macros renamed (CAN_OBJ_READ -> CAN_READ_OBJ)
 * cast's added
 * BUFFER_WRITE_CPY - CONFIG_DRIVER_USE_DIRECTTRANSFER corrected
 *
 * Revision 1.11  2001/03/13 14:33:25  ro
 * move_TxBuffer for Redundancy Support added
 *
 * Revision 1.10  2001/02/28 08:36:10  ro
 * CANopenLib declaration removed - use CANopenLib header files!
 *
 * Revision 1.9  2001/01/15 10:42:13  ro
 * Debug - cal_**_ints for interrupts count - declaration corrected
 *
 * Revision 1.8  2000/11/21 16:15:20  ro
 * memory copy macros only if you use CONFIG_STANDARD_FLUSHMBOX
 * interrupt count vars unified
 *
 * Revision 1.7  2000/11/10 10:00:15  ro
 * new macros for CAN communication buffer access added
 *
 * Revision 1.6  2000/10/19 16:22:23  ro
 * return to v1.4
 *
 * Revision 1.4  2000/10/19 08:48:32  ro
 * standard CO_MEMCPY define added
 *
 * Revision 1.3  2000/07/31 10:10:28  boe
 * dTimerValue as volatile declared
 *
 * Revision 1.2  2000/06/21 10:29:41  boe
 * default value for RTX51_MODIFIER added
 *
 * Revision 1.1  2000/05/31 10:12:51  boe
 * defines for cdriver module diposited
 *
 *
 *
 *------------------------------------------------------------------
 */

/**
*  \file cdriver.h
*  \author port GmbH Halle (Saale)
*  $Revision$
*  $Date$
*
*++ This module provides
*++ the defines and declarations for the cdriver modul.
*-- Diese Modul enthält defines und Deklarationen
*-- für das cdriver Modul.
*
*/

#ifndef __CDRIVER_H
# define __CDRIVER_H

/*---- includes --------------------------------------------------*/
/* are specified in hardware/driver.c, the file which includes this */
# ifndef CAN_ISR_REGISTERBANK
#  define CAN_ISR_REGISTERBANK
# endif /* CAN_ISR_REGISTERBANK */

# ifndef CAN_ISRSUB_REGISTERBANK
#  define CAN_ISRSUB_REGISTERBANK
# endif /* CAN_ISRSUB_REGISTERBANK */

# ifndef CAN_ISR_NUMBER
#  define CAN_ISR_NUMBER
# endif /* CAN_ISR_NUMBER */

# ifndef VOLATILE
#  define VOLATILE volatile
# endif /* VOLATILE */

# ifndef CO_DATA
#  define CO_DATA
# endif /* CO_DATA */

# ifndef XDATA
#  define XDATA
# endif /* XDATA */

# ifndef FAR
#  define FAR
# endif /* FAR */

# ifndef CONFIG_CAN_SLOW_DOWN_IO
#  define CONFIG_CAN_SLOW_DOWN_IO
# endif

/*---- externals -------------------------------------------------*/
# ifdef CONFIG_MULT_LINES
extern void * canAddrTab[CONFIG_MULT_LINES]; /* table for CAN addresses */
# endif

/*---- struct definition ---------------------------------------*/

# ifdef CONFIG_COLIB_BUFFER
/* common transmit and receive buffer */
typedef enum  { EMPTY, FULL }   MEM_STAT_T;

struct BUFFER_ENTRY
{
    COB_KIND_T  eType;
    UNSIGNED16  wCOB_ID;
    UNSIGNED8   bLength;
    UNSIGNED8   pData[8];
    UNSIGNED8   bChannel;
    MEM_STAT_T  eStat;
};

typedef struct BUFFER_ENTRY BUFFER_ENTRY_T;


/*---- function prototypes ---------------------------------------*/

/*---- global variables ------------------------------------------*/
extern    VOLATILE BUFFER_ENTRY_T XDATA pTX_Buffer[] CO_LINE_PARA_ARRAY_DEF;
extern    VOLATILE BUFFER_ENTRY_T XDATA pRX_Buffer[] CO_LINE_PARA_ARRAY_DEF;

extern VOLATILE UNSIGNED8 CO_DATA
            bRX_WriteIndex CO_LINE_PARA_ARRAY_DEF,
            bRX_ReadIndex  CO_LINE_PARA_ARRAY_DEF,
                bTX_WriteIndex CO_LINE_PARA_ARRAY_DEF,
                bTX_ReadIndex  CO_LINE_PARA_ARRAY_DEF;

# ifndef CO_MEMCPY
#  define CO_MEMCPY(dest,src,size) memcpy((dest),(src),(size))
# endif

# ifndef CO_CAN_MEMCPY
#  define CO_CAN_MEMCPY(dest,src,size) CO_MEMCPY((dest),(src),(size))
# endif

/*  Buffer access
-------------------------------------------------------------*/

/* buffer access check
 *
 *  CHECK_BUFFER_WRITE(direction, error)
 *          direction   = RX | TX
 *      error       = for example COFLAG_BUFFER_OVERFLOW
 *                if no write permit
------------------------------------------------------------------*/
# ifdef CONFIG_CAN_ERROR_HANDLING
#  define CHECK_BUFFER_WRITE(direction,error) if (p##direction##_Buffer[  \
                  b##direction##_WriteIndex CO_LINE_PARA_ARRAY_INDEX ] \
                  CO_LINE_PARA_ARRAY_INDEX .eStat == FULL ) { \
                      SET_COLIB_FLAG(error); \
                  } else   
# else
#  define CHECK_BUFFER_WRITE(direction,error) if (p##direction##_Buffer[  \
                  b##direction##_WriteIndex CO_LINE_PARA_ARRAY_INDEX ] \
                  CO_LINE_PARA_ARRAY_INDEX .eStat == EMPTY )
# endif              

/*   CHECK_BUFFER_READ(direction)
 *          direction   = RX | TX
-----------------------------------------------------------------*/ 
# define CHECK_BUFFER_READ(direction)  if (p##direction##_Buffer[ \
                  b##direction##_ReadIndex CO_LINE_PARA_ARRAY_INDEX ] \
                   CO_LINE_PARA_ARRAY_INDEX .eStat == FULL ) 

/*  buffer access
-----------------------------------------------------------------------*/

/*  BUFFER_WRITE(direction, destination, source)
 *               direction  = TX | RX
 *               destination    = item from p??_Buffer[index]
 *               source     = data to write
-----------------------------------------------------------------------*/
# define BUFFER_WRITE(direction,destination,source) p##direction##_Buffer[\
         b##direction##_WriteIndex \
                 CO_LINE_PARA_ARRAY_INDEX ] \
                       CO_LINE_PARA_ARRAY_INDEX .destination = (source)

/*  ret BUFFER_READ(direction, source)
 *               ret        = read data
 *               direction  = TX | RX
 *               source     = item from p??_Buffer[index]
-----------------------------------------------------------------------*/
# define BUFFER_READ(direction,source) p##direction##_Buffer[ \
                  b##direction##_ReadIndex CO_LINE_PARA_ARRAY_INDEX] \
           CO_LINE_PARA_ARRAY_INDEX .source 
           
/*  BUFFER_ENTRY_T BUFFER_ADDR(direction)
 *               direction  = TX | RX
 *       action     = Write | Read
-----------------------------------------------------------------------*/
# define BUFFER_ADDR(direction,action) (&(p##direction##_Buffer[ \
                  b##direction##_##action##Index CO_LINE_PARA_ARRAY_INDEX] \
           CO_LINE_PARA_ARRAY_INDEX))

/* after buffer read/write the index must increment
 *    BUFFER_ENTRY_INCR(direction, action, status)
 *              direction   = TX | RX
 *      action      = Write | Read
 *      status      = new FULL | EMPTY
 *
----------------------------------------------------------------------*/ 
# define BUFFER_ENTRY_INCR(direction,action,status) do{\
        p##direction##_Buffer[          \
         b##direction##_##action##Index CO_LINE_PARA_ARRAY_INDEX ] \
             CO_LINE_PARA_ARRAY_INDEX .eStat = status; \
             if (++b##direction##_##action##Index CO_LINE_PARA_ARRAY_INDEX \
                  == CONFIG_##direction##_BUFFER_SIZE) { \
                 b##direction##_##action##Index CO_LINE_PARA_ARRAY_INDEX = 0; \
             }    \
    }while(0)
# ifdef CONFIG_COLIB_FLUSHMBOX
/* Macro's for buffer access with direct access to CAN controller */

//------------------------------------------------------------------------
// Msg-Data copy from controller to buffer
// BUFFER_WRITE_CPY( length )
// length  = 0..8 Byte
//-----------------------------------------------------------------------
#define BUFFER_WRITE_CPY(length) CO_CAN_MEMCPY(         \
          (void *)(&(BUFFER_ADDR(RX,Write)->pData[0])), \
          (void *)CAN_HWOBJ[bChannel].data,         \
          (length))
//------------------------------------------------------------------------
// copy message data from buffer to controller
// BUFFER_READ_CPY( length )
// length = count of bytes
//------------------------------------------------------------------------
#define BUFFER_READ_CPY(length) CO_CAN_MEMCPY(         \
          (void *)CAN_HWOBJ[bChannel].data,        \
          (void *)(&(BUFFER_ADDR(TX,Read)->pData[0])), \
          (length))
//------------------------------------------------------------------------

# endif /* CONFIG_STANDARD_FLUSHMBOX */

# ifdef  CONFIG_CAN_DEBUG_VARS
extern VOLATILE UNSIGNED32 cal_rx_ints CO_LINE_PARA_ARRAY_DEF;
extern VOLATILE UNSIGNED32 cal_tx_ints CO_LINE_PARA_ARRAY_DEF;
extern VOLATILE UNSIGNED32 cal_ch_ints CO_LINE_PARA_ARRAY_DEF;
# endif /* CONFIG_CAN_DEBUG_VARS */


/* function prototypes */
void Insert_TX_Request( COB_T * , UNSIGNED8 * );

# endif /*CONFIG_COLIB_BUFFER*/

# ifdef CONFIG_REDUNDANCY_SUPPORT
void move_TxBuffer( UNSIGNED8 , UNSIGNED8 );
# endif
# ifdef CONFIG_TEST_VALID_COB
BOOL_T validCobId( UNSIGNED16, UNSIGNED8 CO_COMMA_LINE_PARA_DECL);
# endif

#endif /* __CDRIVER_H */

/*______________________________________________________________________EOF_*/
