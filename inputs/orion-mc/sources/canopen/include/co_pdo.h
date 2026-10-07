/*
 * pdo - public defines for pdo usage
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
 * Revision 1.1  2008-04-08 12:19:45+02  driet
 * <>
 *
 * Revision 1.0  2008-03-07 17:05:40+01  driet
 * Initial revision
 *
 * Revision 2.5  2003/06/13 14:28:18  boe
 * add new function prototype updatePdoReq
 *
 * Revision 2.4  2002/11/18 10:58:00  boe
 * moved cob-ids to co_cobid.h
 * remove PDO_MAPPING structs > 8
 *
 * Revision 2.3  2002/05/21 14:55:56  boe
 * copyright changed
 *
 * Revision 2.2  2001/03/28 13:03:31  boe
 * indication function added
 *
 * Revision 2.1  2001/01/26 12:39:58  boe
 * defines for pdo usage
 *
 *
 *
 *------------------------------------------------------------------
 */

/*
DESCRIPTION

The file contains definitions of structures and data types for pdo usage

*/

#ifndef __CO_PDO_H
# define __CO_PDO_H

#include <co_def.h>		/* include canopen definition */
#include <co_cobid.h>		/* include cobid definition */

#define TRANSMIT_PDO 		0
#define RECEIVE_PDO  		1

#define PDO_NO_RTR_ALLOWED_BIT  0x40000000UL
#define PDO_NO_VALID_BIT        0x80000000UL


/* variants of PDO Mapping structure in order to save RAM
   max 64 entries possible */

typedef struct{
    UNSIGNED8   numOfEntries;  /* number of entries in record */
    UNSIGNED32  map[1];        /* mapping entry */
} PDO_MAPPING1_T;

typedef struct{
    UNSIGNED8   numOfEntries;  /* number of entries in record */
    UNSIGNED32  map[2];        /* mapping entries */
} PDO_MAPPING2_T;

typedef struct{
    UNSIGNED8   numOfEntries;  /* number of entries in record */
    UNSIGNED32  map[3];        /* mapping entries*/
} PDO_MAPPING3_T;

typedef struct{
    UNSIGNED8   numOfEntries;  /* number of entries in record */
    UNSIGNED32  map[4];        /* mapping entries */
} PDO_MAPPING4_T;

typedef struct{
    UNSIGNED8   numOfEntries;  /* number of entries in record */
    UNSIGNED32  map[5];        /* mapping entries */
} PDO_MAPPING5_T;

typedef struct{
    UNSIGNED8   numOfEntries;  /* number of entries in record */
    UNSIGNED32  map[6];        /* mapping entries */
} PDO_MAPPING6_T;

typedef struct{
    UNSIGNED8   numOfEntries;  /* number of entries in record */
    UNSIGNED32  map[7];        /* mapping entries */
} PDO_MAPPING7_T;

typedef struct{
    UNSIGNED8   numOfEntries;  /* number of entries in record */
    UNSIGNED32  map[8];        /* mapping entries */
} PDO_MAPPING8_T;

/* variants of PDO parameter structure in order to save RAM */

typedef struct {
    UNSIGNED8   numOfEntries;          /* number of entries in record */
    UNSIGNED32  cobId;                 /* COB-ID */
    UNSIGNED8   transType;             /* transmission type */
} PDO_COMM_PAR2_T;

typedef struct {
	UNSIGNED8   numOfEntries;          /* number of entries in record */
	UNSIGNED32  cobId;                 /* COB-ID */
	UNSIGNED8   transType;             /* transmission type */
	UNSIGNED16  inhibitTime;           /* inhibit time */
} PDO_COMM_PAR3_T;

typedef struct {
	UNSIGNED8   numOfEntries;          /* number of entries in record */
	UNSIGNED32  cobId;                 /* COB-ID */
	UNSIGNED8   transType;             /* transmission type */
	UNSIGNED16  inhibitTime;           /* inhibit time */
	UNSIGNED8   cmsPriorityGroup;      /* Compatibility Entry */
} PDO_COMM_PAR4_T;

typedef struct {
	UNSIGNED8   numOfEntries;          /* number of entries in record */
	UNSIGNED32  cobId;                 /* COB-ID */
	UNSIGNED8   transType;             /* transmission type */
	UNSIGNED16  inhibitTime;           /* inhibit time */
	UNSIGNED8   cmsPriorityGroup;      /* Compatibility Entry */
	UNSIGNED16  eventTimer;		   /* Event Timer */
} PDO_COMM_PAR5_T;
/* external data declarations */

/* function prototypes */

RET_T 	definePdo(UNSIGNED8, UNSIGNED16, BOOL_T );
RET_T 	writePdoReq(UNSIGNED16 );
RET_T	updatePdoReq(UNSIGNED16 );
RET_T 	readPdoReq(UNSIGNED16 );
void 	*getMapObjAddr(UNSIGNED16, UNSIGNED16, UNSIGNED8 );

void 	pdoInd(UNSIGNED16 );
void 	pdoTimerInd(UNSIGNED16 );

#endif		/*  __CO_PDO_H */

/* end of source */

