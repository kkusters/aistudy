/*
 * nmt - defines for nmt services
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
 * Revision 1.1  2008-03-26 17:06:10+01  driet
 * <>
 *
 * Revision 1.0  2008-03-07 17:05:50+01  driet
 * Initial revision
 *
 * Revision 2.5  2002/11/18 10:28:53  boe
 * add prototype for setNodeState
 *
 * Revision 2.4  2002/05/21 14:11:02  boe
 * cleanup new timer usage
 * add led functionality
 *
 * Revision 2.3  2002/03/26 08:12:11  boe
 * add NEW_TIMER functionality over defines
 *
 * Revision 2.2  2001/03/28 12:46:21  boe
 * added functionality for SLAVE_PLUS
 *
 * Revision 2.1  2001/01/26 10:59:20  boe
 * defines for nmt services
 *
 *
 *
 *------------------------------------------------------------------
 */

/*
DESCRIPTION

The file contains definitions of structures and complex data types
for nmt services

*/

#ifndef __NMT_H
# define __NMT_H


#include <co_stru.h>
#include <co_nmt.h>
#include "cmsevent.h"
#ifdef CONFIG_CO_LED
# include "led.h"
#endif /* CONFIG_CO_LED */

#include "timer.h"

/* structure of a node object */

struct NODE
{
  TIMER_EVENT_T timer;  // timer structure (must be at first position)
#if defined(CONFIG_MASTER) || defined(CONFIG_MASTER_PLUS) || defined(CONFIG_SLAVE_PLUS)
  struct NODE *pNext;   // pointer to next node
                        // only for Remote Node Obj.
#endif
  COB_T *pGuard_COB;    // pointer to guarding COB

#if defined(CONFIG_NODE_GUARDING)
  UNSIGNED8 bGuardToggle;        // toggle bit for Node Guarding
  UNSIGNED8 bLifeTimeFactor;     // wGuardTime*bLifeTimeFactor = LifeTime
  UNSIGNED8 bSuspendedGuardings; // suspended Life Guardings
#endif
  UNSIGNED8 flags;      // flag (see nmterr.h)
#if defined(CONFIG_MASTER) || defined(CONFIG_SLAVE_PLUS)
  UNSIGNED8   mflags;   // flags only for master (see nmterr.h)
#endif
  UNSIGNED8   bNode_ID; // Node-ID (1..127)
  NODE_STATE_T  eState; // node state
};

typedef struct NODE NODE_T;


/* NMT Codes */
#define CS_START_REMOTE_NODE       1
#define CS_STOP_REMOTE_NODE    2
#define CS_ENTER_PRE_OP_STATE    128
#define CS_RESET_APPLICATION     129
#define CS_RESET_COMM            130

/* external variable declarations */

extern NODE_T       *co_pNode ;
extern UNSIGNED8    coNodeId ;/* CANopen Node Id */
extern COB_T        *co_pNMT_COB ;


/* function prototypes */

void    NMT_NodeStartStopMsg(CAN_MSG_T *canMsg );
void    setNodeState(NODE_STATE_T newState );

#ifdef CONFIG_CO_RUN_LED
void updateNMTState_led();
#endif /* CONFIG_CO_RUN_LED */


#endif /* __NMT_H */

/* end of source */

