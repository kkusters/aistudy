/*
 * co_type - defines basic types for CANopen
 *
 * Copyright (c) 2001-2002 port GmbH Halle/Saale
 *------------------------------------------------------------------
 * $Header$
 *
 *------------------------------------------------------------------
 *
 * modification history
 * --------------------
 * $Log$
 * Revision 1.3  2008-04-11 10:29:15+02  driet
 * <>
 *
 * Revision 1.2  2008-03-31 13:02:54+02  driet
 * <>
 *
 * Revision 1.1  2008-03-28 10:31:38+01  driet
 * <>
 *
 * Revision 1.0  2008-03-07 17:05:45+01  driet
 * Initial revision
 *
 * Revision 2.12  2003/07/22 07:29:43  boe
 * add node state UNKNOWN
 *
 * Revision 2.11  2003/06/13 14:30:39  boe
 * add COB for debugging
 *
 * Revision 2.10  2002/11/18 11:12:41  boe
 * remove  structure for domain > 8
 * add defines for COB-ID Types
 *
 * Revision 2.9  2002/05/21 14:59:14  boe
 * add types for split sdo-indication
 *
 * Revision 2.8  2001/12/05 15:12:21  boe
 * added new RET_T values
 *
 * Revision 2.7  2001/08/09 11:45:18  boe
 * define CO_CONST only if it's not defined yet
 *
 * Revision 2.6  2001/06/19 15:32:00  boe
 * added additionally return values for sdo abort
 *
 * Revision 2.5  2001/04/05 08:43:21  boe
 * comment changed
 *
 * Revision 2.4  2001/03/29 14:37:30  boe
 * comment changed
 *
 * Revision 2.3  2001/03/28 13:06:30  boe
 * comment added
 *
 * Revision 2.2  2001/02/26 15:00:53  boe
 * documentation format changed
 *
 * Revision 2.1  2001/01/26 12:43:04  boe
 * canopen types
 *
 *
 *
 *------------------------------------------------------------------
 */



/**
* \file co_type.h
* \author port GmbH, Halle Saale
*
*++ This file contains atomic types for the CANopen library.
*-- Dieses File enthält die Basic Daten Typen für die CANopen Library
*/

#ifndef __CO_TYPE_H
# define __CO_TYPE_H

/* CANopen Basic Types */

# ifdef BOOLEAN
typedef BOOLEAN BOOL_T;
# else
typedef enum { CO_FALSE, CO_TRUE } BOOL_T;
# endif

//# ifdef SDCC
  typedef unsigned int UNSIGNED16;
  typedef int INTEGER16;
//# else /* SDCC */
//  typedef unsigned short int UNSIGNED16;
//  typedef short int INTEGER16;
//# endif /* SDCC */

typedef unsigned char UNSIGNED8;
typedef unsigned long int UNSIGNED32;
typedef signed char INTEGER8;
typedef long int INTEGER32;
typedef float REAL32;

/* Compiler and architecture dependent memory types */
# ifdef CO_CONST
# else /* CO_CONST */
#  ifdef __C51__
#    define CO_CONST	code
#  else /* __C51__ */
#    define CO_CONST const
#  endif /* __C51__ */
# endif /* CO_CONST */

# ifdef CO_DATA
# else /* CO_DATA */
#  ifdef __C51__
#    define CO_DATA	data
#  else /* __C51__ */
#    define CO_DATA
#  endif /* __C51__ */
# endif /* CO_DATA */

# ifdef __C51__
   typedef unsigned char LOOPCNT_U8;	/* loop counter < 256 */
   typedef int LOOPCNT_U16;		/* loop counter < 0x8000 */
# else
   typedef int LOOPCNT_U8;		/* loop counter < 256 */
   typedef int LOOPCNT_U16;		/* loop counter < 0x8000 */
# endif

/* visual string */
typedef unsigned char 	VIS_STRING_T;

/* octet string */
typedef unsigned char 	OCT_STRING_T;

/* bit string */
typedef unsigned char 	BIT_STRING_T;

/* domain */
typedef void *		UPDNLD_DOMAIN_T;

/* download program structure DS 302 */
/*-----------------------------------*/

typedef struct
{
    UNSIGNED8        numOfEntries;  /* number of entries in record */
    UPDNLD_DOMAIN_T  targetAdr[1];  /* domain target address field */
} DOMAIN_FIELD1_T;

typedef struct
{
    UNSIGNED8        numOfEntries;  /* number of entries in record */
    UPDNLD_DOMAIN_T  targetAdr[2];  /* domain target address field */
} DOMAIN_FIELD2_T;

typedef struct
{
    UNSIGNED8        numOfEntries;  /* number of entries in record */
    UPDNLD_DOMAIN_T  targetAdr[3];  /* domain target address field */
} DOMAIN_FIELD3_T;

typedef struct
{
    UNSIGNED8        numOfEntries;  /* number of entries in record */
    UPDNLD_DOMAIN_T  targetAdr[4];  /* domain target address field */
} DOMAIN_FIELD4_T;

typedef struct
{
    UNSIGNED8        numOfEntries;  /* number of entries in record */
    UPDNLD_DOMAIN_T  targetAdr[5];  /* domain target address field */
} DOMAIN_FIELD5_T;

typedef struct
{
    UNSIGNED8        numOfEntries;  /* number of entries in record */
    UPDNLD_DOMAIN_T  targetAdr[6];  /* domain target address field */
} DOMAIN_FIELD6_T;

typedef struct
{
    UNSIGNED8        numOfEntries;  /* number of entries in record */
    UPDNLD_DOMAIN_T  targetAdr[7];  /* domain target address field */
} DOMAIN_FIELD7_T;

typedef struct
{
    UNSIGNED8        numOfEntries;  /* number of entries in record */
    UPDNLD_DOMAIN_T  targetAdr[8];  /* domain target address field */
} DOMAIN_FIELD8_T;


/* kind of CAN object */
#ifdef CONFIG_COMPAT_COB_DRIVER
 typedef enum { RX_COB, TX_COB, RTR_RX_COB, RTR_TX_COB} COB_KIND_T;

# define CO_COB_NMT_MASTER	TX_COB
# define CO_COB_NMT_SLAVE	RX_COB
# define CO_COB_GUARD_MASTER	RTR_RX_COB
# define CO_COB_GUARD_SLAVE	RTR_TX_COB
# define CO_COB_HB_PROD		TX_COB
# define CO_COB_HB_CONS		RX_COB
# define CO_COB_SYNC_PROD	TX_COB
# define CO_COB_SYNC_CONS	RX_COB
# define CO_COB_TIME_PROD	TX_COB
# define CO_COB_TIME_CONS	RX_COB
# define CO_COB_PDO_PROD	TX_COB
# define CO_COB_PDO_PROD_RTR	RTR_TX_COB
# define CO_COB_PDO_CONS	RX_COB
# define CO_COB_PDO_CONS_RTR	RTR_RX_COB
# define CO_COB_SDO_TX		TX_COB
# define CO_COB_SDO_RX		RX_COB
# define CO_COB_EMCY_PROD	TX_COB
# define CO_COB_EMCY_CONS	RX_COB
# define CO_COB_SRDO_PROD	TX_COB
# define CO_COB_SRDO_CONS	RX_COB
# define CO_COB_FLYMA_TX	TX_COB
# define CO_COB_FLYMA_RX	RX_COB
# define CO_COB_LSS_TX		TX_COB
# define CO_COB_LSS_RX		RX_COB
# define CO_COB_SDOMGR_TX	TX_COB
# define CO_COB_SDOMGR_RX	RX_COB
# define CO_COB_SDOREQ_TX	TX_COB
# define CO_COB_DEBUG		TX_COB

#else /* CONFIG_COMPAT_COB_DRIVER */

typedef enum {
	CO_COB_RTR =		0x40,
	CO_COB_DIR_MASK =		0x80,	/* direction mask */
	CO_COB_TX =		0x80,			/* tx cob */
	CO_COB_RX =		0x00,			/* rx cob */
	CO_COB_TX_RTR =		(CO_COB_TX | CO_COB_RTR), /* rtr tx cob */
	CO_COB_RX_RTR =		(CO_COB_RX | CO_COB_RTR), /* rtr rx cob */
	CO_COB_NMT_MASTER =	(CO_COB_TX + 1),
	CO_COB_NMT_SLAVE =	(CO_COB_RX + 2),
	CO_COB_GUARD_MASTER =	((CO_COB_RX | CO_COB_RTR) + 3),
	CO_COB_GUARD_SLAVE =	((CO_COB_TX | CO_COB_RTR) + 4),
	CO_COB_HB_PROD =	(CO_COB_TX + 5),
	CO_COB_HB_CONS =	(CO_COB_RX + 6),
	CO_COB_SYNC_PROD =	(CO_COB_TX + 7),
	CO_COB_SYNC_CONS =	(CO_COB_RX + 8),
	CO_COB_TIME_PROD =	(CO_COB_TX + 9),
	CO_COB_TIME_CONS =	(CO_COB_RX + 10),
	CO_COB_PDO_PROD =	(CO_COB_TX + 11),
	CO_COB_PDO_PROD_RTR =	((CO_COB_TX | CO_COB_RTR) + 12),
	CO_COB_PDO_CONS =	(CO_COB_RX + 13),
	CO_COB_PDO_CONS_RTR =	((CO_COB_RX | CO_COB_RTR) + 14),
	CO_COB_SDO_TX =		(CO_COB_TX + 15),
	CO_COB_SDO_RX =		(CO_COB_RX + 16),
	CO_COB_EMCY_PROD =	(CO_COB_TX + 17),
	CO_COB_EMCY_CONS =	(CO_COB_RX + 18),
	CO_COB_SRDO_PROD =	(CO_COB_TX + 19),
	CO_COB_SRDO_CONS =	(CO_COB_RX + 20),
	CO_COB_FLYMA_TX =	(CO_COB_TX + 21),
	CO_COB_FLYMA_RX =	(CO_COB_RX + 22),
	CO_COB_LSS_TX =		(CO_COB_TX + 23),
	CO_COB_LSS_RX =		(CO_COB_RX + 24),
	CO_COB_SDOMGR_TX =	(CO_COB_TX + 25),
	CO_COB_SDOMGR_RX =	(CO_COB_RX + 26),
	CO_COB_SDOREQ_TX =	(CO_COB_TX + 27),
	CO_COB_DEBUG =		(CO_COB_TX + 28)

    } COB_KIND_T;

#endif /* CONFIG_COMPAT_COB_DRIVER */

typedef enum { CO_BOOLEAN, CO_INTEGER, CO_UNSIGNED, CO_NIL,
	       CO_DUMMY_SPACE, CO_INVALID, CO_STRING} BASIC_DATA_T;
typedef enum { UNKNOWN, INITIALISING, STOPPED=4, OPERATIONAL=5,
		  PRE_OPERATIONAL=127, RESET_APPLICATION, RESET_COMM
		} NODE_STATE_T;
typedef enum { CLIENT, SERVER } USER_T;	    /* for variable and domains */
typedef enum { CONSUMER, PRODUCER } CO_USER_T;

/*!
*++ return values for service requests
*-- Rückgabewerte von Dienstfunktionen
* \par
*-- Achtung: in der Dokumentation angegeben Werte dienen nur der Referenz,
*-- sie können sich ändern
*++ values are subject of change, use the enum definition. 
*/
typedef enum
{
  CO_OK,                  /*!<  0 request successful */
  CO_E_MEM,               /*!<  1 not enough memory */
  CO_E_NOT_EXIST,         /*!<  2 object doesn't exist */
  CO_E_ALREADY_EXIST,     /*!<  3 object already exist */
  CO_E_STATE,             /*!<  4 operation not allowed in this state */
  CO_E_TYPE,              /*!<  5 no matching type */
  CO_E_INHIBITED,         /*!<  6 inhibit time active */
  CO_E_NO_INITIATE,       /*!<  7 no initiate service executed */
  CO_E_BUSY,              /*!<  8 service is already running */
  CO_E_DATA_LENGTH,       /*!<  9 datatype doesn't fit in telegram */
  CO_E_ERROR_LENGTH,      /*!< 10 errortype doesn't fit in telegram */
  CO_E_NO_NETWORK,        /*!< 11 no network object */
  CO_E_RANGE,             /*!< 12 invalid range */
  CO_E_NAME_LENGTH,       /*!< 13 name length not correct */
  CO_E_NAME_SYNTAX,       /*!< 14 name syntax not correct */
  CO_E_NO_DATABASE,       /*!< 15 no COB database available */
  CO_E_DISABLED,          /*!< 16 object disabled */
  CO_E_SYNTAX,            /*!< 17 syntax error in data- or errortype description (internal)*/
  CO_E_SYNTAX_D_TYPE,     /*!< 18 syntax error in datatype description */
  CO_E_SYNTAX_E_TYPE,     /*!< 19 syntax error in errortype description */
  CO_E_MAP,               /*!< 20 mapping error */
  CO_E_NO_ACCESS,         /*!< 21 no access to object dictionary */
  CO_E_NONEXIST_OBJECT,   /*!< 22 object doesn't exist */
  CO_E_NONEXIST_SUBINDEX, /*!< 23 subindex doesn't exist */
  CO_E_NO_READ_PERM,      /*!< 24 no read permission */
  CO_E_NO_WRITE_PERM,     /*!< 25 no write permission */
  CO_E_VALUE_TO_HIGH,     /*!< 26 value greater upper limit */
  CO_E_VALUE_TO_LOW,      /*!< 27 value smaller lower limit */
  CO_E_WRONG_SIZE,        /*!< 28 object has wrong size */
  CO_E_TRANS_TYPE, 	  /*!< 29 wrong trans type */
  CO_E_HARDWARE_FAULT,	  /*!< 30 hardware fault */
  CO_E_PARA_INCOMP,	  /*!< 31 parameter incompatible */
  CO_E_SDO_OTHER,	  /*!< 32 unknown sdo error */
  CO_E_SDO_CMD_SPEC_INVALID,/*!< 33 sdo command specifier invalid */
  CO_E_SDO_INVALID_BLKSIZE, /*!< 34 invalid sdo block size */
  CO_E_SDO_INVALID_BLKCRC,/*!< 35 invalid sdo block crc sum */
  CO_E_SRD_NO_RESSOURCE,  /*!< 36 no resources available for sdo connection */
  CO_E_BAD_ERROR_CTRL,	  /*!< 37 bad requested error control mechanism */
  CO_E_SDO_TIMEOUT,	  /*!< 38 sdo timed out */
  CO_E_SDO_INVALID_TOGGLEBIT,/*!< 39 sdo invalid togglebit */
  CO_E_INVALID_TRANSMODE,  /*!< 40 sdo invalid transmode */
  CO_E_DEVICE_STATE,       /*!< 41 bad device state */
  CO_E_BAD_CRC,		   /*!< 42 bad CRC */
  CO_SDO_IND_BUSY=99	   /*!< 99 SDO indication busy */
} RET_T;

/*!
*++ CAN/CANopen error conditions
*-- CAN/CANopen Fehler-Bedingungen
*/
typedef enum {
   CO_BUS_OFF,           /*!< can-controller error */
   CO_ERROR_PASSIVE,     /*!< can-controller error */
   CO_OVERRUN,           /*!< can-controller error */
   CO_RX_BUFFER_OVERFLOW,/*!< receive buffer overflow */
   CO_TX_BUFFER_OVERFLOW,/*!< transmit buffer overflow */
   CO_DRIVER_ERROR,      /*!< couldn't connect to driver */
   CO_LOST_GUARDING_MSG, /*!< slave misses guarding message */
   CO_NODE_STATE,        /*!< changed state recognized by guarding */
   CO_LOST_CONNECTION,   /*!< life time elapsed for node */
   CO_INVALID_COB,       /*!< protocol error (domain protocol) */
   CO_ERROR_ACTIVE,      /*!< CAN controller is active again */
   CO_GUARDING_STARTED,  /*!< first guarding message received */
   CO_LOST_HEARTBEAT,	 /*!< lost heartbeat message */
   CO_BOOT_UP,		 /*!< Bootup Message received */
   CO_HB_STARTED	 /*!< first Heartbeat received */
} ERROR_SPEC_T;


typedef enum { CO_DISABLED, CO_ENABLED } STATE_T;


#define CAN_29_BIT_ID_FLAG  0x20000000UL
#define CAN_11_BIT_ID_MASK  0x7ffUL
#define CAN_29_BIT_ID_MASK  0x1fffffffUL

#endif		/* __CO_TYPE_H */
