/*
 * co_sdo - public defines for sdo usage
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
 * Revision 1.0  2008-03-07 17:05:41+01  driet
 * Initial revision
 *
 * Revision 2.8  2003/07/22 07:28:54  boe
 * add prototype for bootloader indication
 *
 * Revision 2.7  2002/11/18 10:58:35  boe
 * moved cob-ids to co_cobid.h
 * change prototypes
 *
 * Revision 2.6  2002/08/30 08:50:22  boe
 * change returnvalue for testSdoValue
 *
 * Revision 2.5  2002/05/21 14:58:49  boe
 * add function for split sdo-indication
 *
 * Revision 2.4  2002/04/05 13:27:52  boe
 * change return value for sdoWrInd
 *
 * Revision 2.3  2001/06/19 15:31:15  boe
 * added additionally abort codes
 *
 * Revision 2.2  2001/03/28 13:04:02  boe
 * indication functions added
 *
 * Revision 2.1  2001/01/26 12:40:19  boe
 * defines for sdo usage
 *
 *
 *
 *------------------------------------------------------------------
 */

/*
DESCRIPTION

The file contains definitions of structures and data types for sdo usage

*/

#ifndef __CO_SDO_H
# define __CO_SDO_H

/* # include <co_stru.h> */
# include <co_def.h>		/* include canopen definition */
# include <co_cobid.h>		/* include cobid definition */

typedef struct {
	UNSIGNED8   numOfEntries;          /* number of entries in record */
	UNSIGNED32  cobIdReqInd;	   /* COB ID request or indication */
	UNSIGNED32  cobIdResCon;	   /* COB ID response or confirmation */
	UNSIGNED8   nodeId;		   /* write permission */
} SDO_COMM_PAR_T;


#define SDO_NO_VALID_BIT        0x80000000UL

/* numeric bit for SDO type length on big endian machines */
# define CO_NUM_SDO		0x80
# define CO_NONUM_SDO		0x7F


/* SDO Error defines DS 301 V 4.0 */

/* error class */
#define E_SDO_NO_ERROR         	0x00000000UL
#define E_SDO_SERVICE          	0x05000000UL
#define E_SDO_ACCESS           	0x06000000UL
#define E_SDO_OTHER		0x08000000UL
/* error code */
#define E_SDO_UNSUPP_ACCESS    	0x00010000UL
#define E_SDO_NONEXIST_OBJECT  	0x00020000UL
#define E_SDO_INCONS_PARA      	0x00030000UL
#define E_SDO_ILLEG_PARA       	0x00040000UL
#define E_PDO_MAPPING	  	0x00040000UL
#define E_SDO_HARDWARE_FAULT   	0x00060000UL
#define E_SDO_TYPE_CONFLICT    	0x00070000UL
#define E_SDO_INCONS_OBJ_ATTR  	0x00090000UL
#define E_SDO_RES_NOT_AVAIL 	0x000a0000UL

/* additional Error Codes for SDO */
#define E_SDO_A_NO_DETAILS		0
#define E_SDO_A_CMD_SPEC_INVALID	0x01
#define E_SDO_A_NO_READ_PERM		0x01
#define E_SDO_A_SIZE_INVALID		0x02
#define E_SDO_A_NO_WRITE_PERM		0x02
#define E_SDO_A_CRC_INVALID		0x04
#define E_SDO_A_OUT_OF_MEM		0x05
#define E_SDO_A_INVALID_VAL		0x10
#define E_SDO_A_NONEXIST_SUBINDEX	0x11
#define E_SDO_A_LENGTH_TO_HIGH		0x12
#define E_SDO_A_LENGTH_TO_LOW		0x13
#define E_SDO_A_NO_EXECUTION		0x20
#define E_SDO_A_INVALID_TRANSMODE	0x20
#define E_SDO_A_UNDER_LOCAL_CONTROL	0x21
#define E_SDO_A_WRONG_STATE		0x22
#define E_SDO_A_SDO_CONN		0x23
#define E_SDO_A_VALUE_RANGE_EXCEED	0x30
#define E_SDO_A_VALUE_TO_HIGH		0x31
#define E_SDO_A_VALUE_TO_LOW		0x32
#define E_SDO_A_MAX_LESS_MIN		0x36
#define E_SDO_A_INCOMP			0x40
#define E_SDO_A_NO_MAPPING		0x41
#define E_SDO_A_PDO_LENGTH_EXCEED	0x42
#define E_SDO_A_GENERAL_PARA_INCOMP	0x43
#define E_SDO_A_GENERAL_INTERNAL_INCOMP	0x47

/* error class */
#define E_SDO_INTERNAL		0xFF000000UL
/* error code */
#define E_SDO_NO_RESSOURCES    	0x00FF0000UL /* no or not enough memory for upload   */
#define E_SDO_ZERO_ERROR        0x00FE0000UL /* abortDomainTransfer has error code 0 */

/* external data declarations */


/* function prototypes */

RET_T	 	defineSdo(UNSIGNED8, USER_T );
RET_T	 	writeSdoReq(UNSIGNED8, UNSIGNED16, UNSIGNED8, UNSIGNED8 *,
				UNSIGNED32 );
RET_T	 	readSdoReq(UNSIGNED8, UNSIGNED16, UNSIGNED8, UNSIGNED8 *,
				UNSIGNED32 );
UNSIGNED32      getSdoSize(UNSIGNED8, USER_T );
UNSIGNED8       getActualSdo(UNSIGNED16, UNSIGNED8 );
BOOL_T		waitForSdoRes(UNSIGNED8  , UNSIGNED32 );

#ifdef CONFIG_SPLIT_INDICATION
RET_T	 	sdoRdInd(UNSIGNED16, UNSIGNED8, UNSIGNED8 );
RET_T	 	sdoWrInd(UNSIGNED16, UNSIGNED8, UNSIGNED8 );
#else /* CONFIG_SPLIT_INDICATION */
RET_T	 	sdoRdInd(UNSIGNED16, UNSIGNED8 );
RET_T	 	sdoWrInd(UNSIGNED16, UNSIGNED8 );
#endif /* CONFIG_SPLIT_INDICATION */
void            sdoRdCon(UNSIGNED8, UNSIGNED32 );
void            sdoWrCon(UNSIGNED8, UNSIGNED32 );
RET_T		testSdoValue(UNSIGNED16, UNSIGNED8, void *, UNSIGNED32
			);
RET_T		sdoBootLoadInd(UNSIGNED8 actSize, UNSIGNED8 overSize);

RET_T finishSdoWrInd(UNSIGNED8 sdoNr, RET_T retCode );
RET_T finishSdoRdInd(UNSIGNED8 sdoNr, RET_T retCode );

#endif		/*  __CO_SDO_H */

/* end of source */

