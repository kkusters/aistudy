/*
 * co_cobid - cob id define for CANopen
 *
 * Copyright (c) 2002 port GmbH Halle/Saale
 *------------------------------------------------------------------
 * $Header$
 *
 *------------------------------------------------------------------
 *
 * modification history
 * --------------------
 * $Log$
 * Revision 1.0  2008-03-07 17:05:31+01  driet
 * Initial revision
 *
 * Revision 2.1  2002/11/18 10:50:39  boe
 * cob-id defines for CANopen
 *
 *
 *
 *
 *------------------------------------------------------------------
 */

/*
DESCRIPTION

The file contains definitions of predifined COB-Ids from DS301

*/

#ifndef __CO_COBID_H
# define __CO_COBID_H

/* predefined connection set from ds 301 */

/* NMT */
#define CO_COBID_NMT		0

/* SYNC */
#define CO_COBID_SYNC		0x80

/* EMCY */
#define CO_COBID_EMCY		0x80
#define CO_COBID_EMCY_FIRST	(CO_COBID_EMCY + 1)
#define CO_COBID_EMCY_LAST	(CO_COBID_EMCY + 0x7f)

/* TIME */
#define CO_COBID_TIME		0x100

/* SRDO */
#define CO_COBID_SRDO		0x100
#define CO_COBID_SRDO_FIRST	(CO_COBID_SRDO + 1)
#define CO_COBID_SRDO_LAST	(CO_COBID_SRDO + 0xff)

/* PDOs */
#define CO_COBID_PDO		0x180
#define CO_COBID_PDO_FIRST	(CO_COBID_PDO + 1)
#define CO_COBID_PDO_LAST	(CO_COBID_PDO + 0x3ff)

#define CO_COBID_TPDO1		(CO_COBID_PDO )
#define CO_COBID_TPDO2		(CO_COBID_PDO + 0x100)
#define CO_COBID_TPDO3		(CO_COBID_PDO + 0x200)
#define CO_COBID_TPDO4		(CO_COBID_PDO + 0x300)

#define CO_COBID_RPDO1		(CO_COBID_PDO + 0x080)
#define CO_COBID_RPDO2		(CO_COBID_PDO + 0x180)
#define CO_COBID_RPDO3		(CO_COBID_PDO + 0x280)
#define CO_COBID_RPDO4		(CO_COBID_PDO + 0x380)

/* SDO */
#define CO_COBID_SDO		0x580
#define CO_COBID_SSDO		CO_COBID_SDO
#define CO_COBID_CSDO		(CO_COBID_SDO + 0x80)
#define CO_COBID_SDO_FIRST	(CO_COBID_SDO + 1)
#define CO_COBID_SDO_LAST	(CO_COBID_SDO + 0xff)

#define CO_COBID_SSDO_FIRST	(CO_COBID_SSDO + 1)
#define CO_COBID_CSDO_FIRST	(CO_COBID_CSDO + 1)

/* NMTERR */
#define CO_COBID_NMTERR		0x700
#define CO_COBID_NMTERR_FIRST	(CO_COBID_NMTERR + 1)
#define CO_COBID_NMTERR_LAST	(CO_COBID_NMTERR + 0x7f)

#define CO_COBID_SDOREQ		1760

#define CO_COBID_LSS_REQ	2021	/* Request COB Id for LSS */
#define CO_COBID_LSS_CON	2020	/* Confirmation COB Id for LSS */

/* #define CO_COBID_FLYMA		2040 */

#endif		/*  __CO_COBID_H */
/* end of source */

