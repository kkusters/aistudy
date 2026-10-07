/*
 * co_emcy - public defines for emergnecy usage
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
 * Revision 1.3  2008-10-24 16:44:10+02  driet
 * <>
 *
 * Revision 1.2  2008-09-17 17:06:55+02  driet
 * <>
 *
 * Revision 1.1  2008-03-12 16:32:37+01  driet
 * <>
 *
 * Revision 1.0  2008-03-07 17:05:34+01  driet
 * Initial revision
 *
 * Revision 2.6  2002/11/18 10:54:28  boe
 * moved cobid to co_cobid.h
 *
 * Revision 2.5  2002/05/21 14:54:20  boe
 * copyright changed
 *
 * Revision 2.4  2001/06/19 15:29:38  boe
 * added define EMCY_NOT_VALID_BIT
 *
 * Revision 2.3  2001/03/28 13:01:59  boe
 * indication function added
 *
 * Revision 2.2  2001/03/14 17:23:53  ro
 * setClientEmcyCobId() added
 *
 * Revision 2.1  2001/01/26 12:33:13  boe
 * defines for emergency
 *
 *
 *
 *------------------------------------------------------------------
 */

/*
DESCRIPTION

The file contains public definitions of structures and data types for
emergnecy usage

*/

#ifndef __CO_EMCY_H
# define __CO_EMCY_H

# include <co_def.h>		/* include canopen definition */
# include <co_type.h>
# include <co_cobid.h>

/* Emergency object DS 301 Version 4.0 */

typedef struct
{
    UNSIGNED16 errCode;
    UNSIGNED8  errReg;
    UNSIGNED16 manu1;
    UNSIGNED16 manu2;
    UNSIGNED8  manu3;
} EMERGENCY_T;


/* Bitmask for enable/disable emcy */
#define EMCY_NOT_VALID_BIT	0x80000000

/* external data declarations */

/* function prototypes */

RET_T 		defineEmcy(CO_USER_T, UNSIGNED8, UNSIGNED16);
EMERGENCY_T *readEmcy(UNSIGNED8 );
RET_T 		writeEmcyReq(UNSIGNED16, UNSIGNED16, UNSIGNED16, UNSIGNED8);
RET_T 		eraseErr(UNSIGNED16 );
RET_T		setClientEmcyCobId(UNSIGNED8, UNSIGNED16 );

void 		emcyInd(UNSIGNED8 );

#endif		/*  __CO_EMCY_H */

/* end of source */

