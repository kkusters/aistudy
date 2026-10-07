/*
 *++ nmt_s - additional network routines for slaves
 *-- nmt_s - Zusätzliche Netzwerkfunktionalität für Slaves
 *
 * Copyright (c) 2001-2002 port GmbH Halle (Saale)
 *------------------------------------------------------------------
 * $Header$
 *
 *------------------------------------------------------------------
 *
 * modification history
 * --------------------
 *
 * $Log$
 * Revision 1.0  2008-03-07 17:04:43+01  driet
 * Initial revision
 *
 * Revision 2.9  2003/03/31 12:36:38  boe
 * reset all objects for resetApplication
 *
 * Revision 2.8  2002/12/11 07:57:01  boe
 * set init value for coDefParaResetFlag
 *
 * Revision 2.7  2002/11/15 10:01:46  boe
 * add comments/adapt on doxygen
 * add co_ to all global library variables
 * add loadParameterInd() for reset commands
 * resetComm modified
 *
 * Revision 2.6  2002/05/21 14:11:52  boe
 * cleanup new timer usage
 * add led functionality
 *
 * Revision 2.5  2001/12/05 14:00:10  boe
 * send bootup message after reset communication
 *
 * Revision 2.4  2001/05/17 09:37:29  boe
 * explicite type conversion for constants to remove compiler warnings
 *
 * Revision 2.3  2001/04/05 12:26:10  boe
 * command line parameter for single line changed to void
 *
 * Revision 2.2  2001/02/26 14:52:45  boe
 * driver access functions replaced by macros
 *
 * Revision 2.1  2001/01/26 11:01:10  boe
 * functions for slave nmt services
 *
 *
 *
 *------------------------------------------------------------------
 */


/*
*  \file nmt_s.c
*  \author port GmbH Halle (Saale)
*  $Revision$
*  $Date$
*
*++ Additional network features  NMT services are defined in this
*++ module.
*++ They are necessary for the CANopen communication profile DS 301.
*++ In this module functions are defined which influence the state machine
*++ of a CANopen Device.
*++ It defines the behaviour of communication and application reset.
*-- In diesem Modul sind zusätzliche NMT Dienste definiert.
*-- Diese Dienste sind notwendig für das CANopen Communication Profile DS\ 301.
*-- Die in diesem Modul definierten Funktionen beeinflussen die Zustandsmachine
*-- der CANopen Slaves.
.LP
*/

/* header of standard C - libraries */

#include <stdio.h>
#include <string.h>

/* header of project specific types */

#include <cal_conf.h>
#include <co_odidx.h>
#include <co_setcp.h>
#include <co_usr.h>
#include "nmt.h"
#include "nmt_s.h"
#include "nmterr.h"
#include "access.h"
#include "drv.h"
#include "pdo.h"
#include "sdo.h"

#ifdef CONFIG_NON_VOLATILE_MEM
# include <co_stor.h>
#endif /*  CONFIG_NON_VOLATILE_MEM */

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
#ifdef CONFIG_NON_VOLATILE_MEM
/* flag for exception if load default parameter befor a reset
   node or communication */
UNSIGNED8   coDefParaResetFlag  = { 0 };
#endif /* CONFIG_NON_VOLATILE_MEM */

/* local defined variables
---------------------------------------------------------------------------*/
#ifdef CONFIG_RCS_IDENT
static char _rcsid[] = "$Id$";
#endif /* CONFIG_RCS_IDENT */


/* #if defined(CONFIG_SLAVE) || defined(CONFIG_MASTER_PLUS) */
/*******************************************************************
*
* resetNodeMsg - This function will be indicated at the NMT-Slave.
*
* NOMANUAL
*
* It is responsible for the reset of the slave application by calling
* the user defined routine \fIresetApplication()\fP.
* Afterwards the function sets
* the NMT Slave in the state RESET_COMM and calls resetCommInd() the
* user defined function to set the commmunication parameter to their default
* values. The last step is forcing the NMT slave to the state PRE_OPERATIONAL.
*
* \retval
* nothing
*/

void resetNodeMsg(void)
{
#ifdef CONFIG_NON_VOLATILE_MEM
  // Wenn nicht vorher 1011 war
  if (coDefParaResetFlag == 0)
  {
#endif // CONFIG_NON_VOLATILE_MEM

    // set default values
    setDefaultParameter(MEM_SEG_ALL_PARAMETERS CO_COMMA_LINE_PARA);

#ifdef CONFIG_NON_VOLATILE_MEM
    // load communication values from flash ----------------------------
    loadParameterInd(MEM_SEG_ALL_PARAMETERS, CO_RESTORE_MODE_RESETCOMM CO_COMMA_LINE_PARA);
  }
  else
  {
    // reset 1011 sign
    coDefParaResetFlag  = 0;
  }
#endif // CONFIG_NON_VOLATILE_MEM

  resetApplInd(CO_LINE_PARA);

  // automatically changing to RESET_COMM state
  co_pNode->eState = RESET_COMM;
  // reset communication
  resetCommMsg(CO_LINE_PARA);
}


/*******************************************************************
*
* resetCommMsg - This function will be indicated at the NMT-Slave.
*
* NOMANUAL
*
* This function react to a reset communication from the NMT-master.
* The reaction of this command depends on the state
* of calling 'restore default parameter' by writing to 0x1011.
* For a complete reset communication (without 0x1011 write)
* it does the following steps:
* - get node id by \fIgetNodeId()\fP call
* - reset all object dictionary entries
* - calls \fIloadParameterInd()\fP
* - calls \fIresetCommInd()\fP
* - updates the library internal communication parameter
*
* For a reset communication with 0x1011 write:
* - calls \fIresetCommInd()\fP
* - updates the library internal communication parameter
*
* Afterwards it will forcing the NMT slave to the state PRE_OPERATIONAL.
*
* \retval
* nothing
*/
void resetCommMsg(void)
{
#if defined(CONFIG_PDO_PRODUCER) || defined(CONFIG_PDO_CONSUMER) || defined(CONFIG_SDO_COB_ID) || defined(CONFIG_SDO_CLIENT)
UNSIGNED16 CO_DATA nr;  // SDO/PDO number
#endif // defined(CONFIG_PDO_PRODUCER) || defined(CONFIG_SDO_COB_ID)
#if defined(CONFIG_SDO_COB_ID) || defined(CONFIG_SDO_CLIENT)
UNSIGNED16 index;       // index
#endif // defined(CONFIG_SDO_COB_ID) || defined(CONFIG_SDO_CLIENT)
UNSIGNED8 bData;        // transmit buffer

  // reset the internal state of communication
  resetComStates(CO_LINE_PARA);

#ifdef CONFIG_NON_VOLATILE_MEM
  // Wenn nicht vorher 1011 war
  if (coDefParaResetFlag  == 0)
  {
#endif // CONFIG_NON_VOLATILE_MEM

    // set default values
    setDefaultParameter(MEM_SEG_COM_PARAMETERS CO_COMMA_LINE_PARA);

#ifdef CONFIG_NON_VOLATILE_MEM
    // load communication values from flash ----------------------------
    loadParameterInd(MEM_SEG_COM_PARAMETERS, CO_RESTORE_MODE_RESETCOMM CO_COMMA_LINE_PARA);
  }
  else
  {
    // reset 1011 sign
    coDefParaResetFlag  = 0;
  }
#endif // CONFIG_NON_VOLATILE_MEM

  // call user indication -----------------------------------------------
  resetCommInd(CO_LINE_PARA);

  // update internal variable and cob-ids --------------------------------
#if defined (CONFIG_SYNC_CONSUMER)
  setCommPar(SYNC_COB_ID_INDEX, 0 CO_COMMA_LINE_PARA);
  // cycle period, cob,
#endif // defined (CONFIG_SYNC_CONSUMER)

#if defined (CONFIG_TIME_CONSUMER)
  setCommPar(TIME_COB_ID_INDEX, 0 CO_COMMA_LINE_PARA);
#endif // defined (CONFIG_TIME_CONSUMER) || defined (CONFIG_TIME_CONSUMER)

#ifdef CONFIG_HEARTBEAT_PRODUCER
  setCommPar(HEARTBEAT_PROD_INDEX, 0 CO_COMMA_LINE_PARA);
#endif  // CONFIG_HEARTBEAT_PRODUCER

#ifdef CONFIG_EMCY_PRODUCER
  setCommPar(EMCY_COB_ID_INDEX, 0 CO_COMMA_LINE_PARA);
  // inhibit time
#endif // CONFIG_EMCY_PRODUCER

#ifdef CONFIG_PDO_CONSUMER
  // PDO Parameter
  nr = 1;
  while (nr <= 512)
  { // check for nr prevents dead lock
    if ((pdoExist(nr, RECEIVE_PDO CO_COMMA_LINE_PARA)) != NULL)
    {
      definePdo(RECEIVE_PDO, nr, CO_FALSE CO_COMMA_LINE_PARA);
    }
    nr++;
  }
#endif // CONFIG_PDO_CONSUMER
#ifdef CONFIG_PDO_PRODUCER
  nr = 1;
  while (nr <= 512)
  { // check for nr prevents dead lock
    if ((pdoExist(nr, TRANSMIT_PDO CO_COMMA_LINE_PARA)) != NULL)
    {
      definePdo(TRANSMIT_PDO, nr, CO_FALSE CO_COMMA_LINE_PARA);
    }
    nr++;
  }
#endif // CONFIG_PDO_PRODUCER

  // set SDO Para
#if defined(CONFIG_SDO_COB_ID) || defined(CONFIG_SDO_CLIENT)
  // SERVER SDO
  nr = 1;
  while (nr <= 128)
  { // check for nr prevents dead lock
    if (CMS_DomExist((UNSIGNED8)nr, SERVER CO_COMMA_LINE_PARA) != NULL)
    {
      index =  SSDO_PARA_BASE_INDEX + nr - 1;
      setCommPar(index, 1 CO_COMMA_LINE_PARA);
      setCommPar(index, 2 CO_COMMA_LINE_PARA);
    }
    nr++;
  }
  // CLIENT SDO
  nr = 1;
  while (nr <= 128)
  { // check for nr prevents dead lock
    if(CMS_DomExist((UNSIGNED8)nr, CLIENT CO_COMMA_LINE_PARA) != NULL)
    {
      index =  CSDO_PARA_BASE_INDEX + nr - 1;
      setCommPar(index, 1 CO_COMMA_LINE_PARA);
      setCommPar(index, 2 CO_COMMA_LINE_PARA);
    }
    nr++;
  }
#else // defined(CONFIG_SDO_COB_ID) || defined(CONFIG_SDO_CLIENT)
# if defined(CONFIG_SDO_SERVER)
  // set default SDO
  setDefSdoCobId(CO_LINE_PARA);
# endif // defined(CONFIG_SDO_SERVER)
#endif // defined(CONFIG_SDO_COB_ID) || defined(CONFIG_SDO_CLIENT)

  // automatically changing to PRE_OPERATIONAL state
  co_pNode->eState = PRE_OPERATIONAL;


#ifdef CONFIG_NODE_GUARDING
  setCommPar(LIFE_TIME_FAC_INDEX, 0 CO_COMMA_LINE_PARA);
  setCommPar(GUARD_TIME_INDEX, 0 CO_COMMA_LINE_PARA);
#endif // CONFIG_NODE_GUARDING

#if defined(CONFIG_NODE_GUARDING) || defined(CONFIG_HEARTBEAT_PRODUCER)
  SET_COB_ID(co_pNode->pGuard_COB, (UNSIGNED16)(CO_COBID_NMTERR + (UNSIGNED16)coNodeId ));
# ifdef CONFIG_FULLCAN
  // preset guarding channel
  UPDATE_COB(co_pNode->pGuard_COB, (UNSIGNED8 *)&(co_pNode->eState));
# endif // CONFIG_FULLCAN
#endif // defined(CONFIG_NODE_GUARDING) || defined(CONFIG_HEARTBEAT_PRODUCER)

  // send the bootup message
  bData = 0;
  TRANSMIT_COB(co_pNode  ->pGuard_COB, &bData);

#ifdef CONFIG_CO_RUN_LED
  updateNMTState_led();
#endif // CONFIG_CO_RUN_LED
}

/* #endif */ /* CONFIG_SLAVE || CONFIG_MASTER_PLUS */
/*______________________________________________________________________EOF_*/
