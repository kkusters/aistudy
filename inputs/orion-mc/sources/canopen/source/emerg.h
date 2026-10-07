/*
 * emerg - defines for emergency services
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
 * Revision 1.0  2008-03-07 17:05:48+01  driet
 * Initial revision
 *
 * Revision 2.5  2003/03/31 13:15:06  boe
 * emcy consumer usage optimezed
 * (define own functions and structures)
 *
 * Revision 2.4  2002/11/18 10:25:26  boe
 * add prototype for setEmcyCobId
 *
 * Revision 2.3  2002/05/21 13:55:04  boe
 * change copyright
 *
 * Revision 2.2  2001/06/19 15:20:35  boe
 * add define for EMCYFLAG_ENABLED
 *
 * Revision 2.1  2001/01/26 10:50:38  boe
 * defines for emergency usage
 *
 *
 *
 *------------------------------------------------------------------
 */

/*
DESCRIPTION

The file contains definitions of structures and complex data types
for emergency services

*/

#ifndef __EMERG_H
# define __EMERG_H


#include <co_stru.h>
#include <co_emcy.h>


struct EMCY_CONS {
    struct EMCY_CONS *pNext;    /* pointer to next event */
    COB_T       *pCOB;      /* COB for Request/Response */
    UNSIGNED8   emcyNr;     /* number of emergency  1..128 */
    UNSIGNED8       flags;      /* PDO-flags, PDO disabled, RTR, */
};

typedef struct EMCY_CONS EMCY_CONS_T;

typedef struct CMS_EVENT EMCY_T;


#define EMCYFLAG_ENABLED ((UNSIGNED8)1)


/* external variable declarations */
extern EMCY_T       *co_pFirstEmcyProd ;
extern EMCY_CONS_T  *co_pFirstEmcyCons ;


/* function prototypes */

void    emcyMsgReceived(CAN_MSG_T *canMsg );
RET_T   setEmcyCobId(UNSIGNED32 *pCobid );
void    leaveEmcyCons(void);




#endif /* __EMERG_H */

/* end of source */

