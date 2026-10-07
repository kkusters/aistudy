/*
 *++ nmtslave - user-defined CANopen NMT functions 
 *
 * Copyright (c) 1997-2002 port GmbH Halle (Saale)
 *------------------------------------------------------------------
 * $Header$
 *
 *------------------------------------------------------------------
 *
 * modification history
 * --------------------
 *
 * $Log$
 * Revision 1.1  2011-10-26 10:44:41+02  driet
 * <>
 *
 * Revision 1.0  2011-07-18 11:49:04+02  driet
 * Initial revision
 *
 * Revision 1.3  2008-05-30 12:59:55+02  driet
 * <>
 *
 * Revision 1.2  2008-04-11 10:28:50+02  driet
 * <>
 *
 * Revision 1.1  2008-04-08 12:11:56+02  driet
 * when entering the OPERATIONAL state transmit all PDO's with transType 255
 *
 * Revision 1.0  2008-03-06 17:15:32+01  driet
 * Initial revision
 *
 * Revision 2.8  2003/01/21 09:48:16  boe
 * adapted for library v4.3
 *
 * Revision 2.7  2002/12/11 08:53:01  boe
 * adaption for library v4.3
 *
 * Revision 2.6  2002/05/30 12:39:40  hae
 * documentation correction
 *
 * Revision 2.5  2002/05/21 15:08:04  boe
 * change retval from newStateInd
 *
 * Revision 2.4  2001/04/05 08:40:30  boe
 * comment changed
 *
 * Revision 2.3  2001/03/28 13:09:23  boe
 * comments changed
 *
 * Revision 2.2  2001/03/15 08:34:52  ro
 * new Header files for Lib V4.2
 *
 * Revision 2.1  2000/06/09 14:54:11  boe
 * include file cal_help removed
 *
 * Revision 2.0  2000/01/21 11:29:13  boe
 * Überarbeitet und an Version 4.0 angepasst
 *
 *
 *
 *
 *------------------------------------------------------------------
 */

/**
*  \file nmtslave.c
*  \author port GmbH Halle (Saale)
*  $Revision$
*  $Date$
*
*++ This file contains function templates for a CANopen device.
*++ The functions have influence to the state machine behaviour of the
*++ CANopen node.
*++ The user is responsible for the contents of the functions.
*++ Before using this template you must make a copy of them.
*/


#include <cal_conf.h>
#include <co_acces.h>
#include <co_drv.h>
#include <co_pdo.h>
#include <co_usr.h>
#include <co_nmt.h>

#include "objects.h"
#include "ch_DS401.h"

/*******************************************************************/
/**
*++ \brief resetApplInd - reset the application
*
*++ This function will reset the device's application.
*++ All application parameters from the Object Dictionary are set
*++ to the default values before this function is called.
*++ The user has to ensure that his application will be reset.
*++ Additionally, it is possible application to load parameters
*++ from a non volatile memory.
*
* \return
* nothing
*/

void resetApplInd(void)
{
  // reset application states
  // users_function()
//  DS401_InitFlag = 1;
  // load certain data from non volatile memory
  // users_function()
}

/*******************************************************************/
/**
*++ \brief resetCommInd - reset communication parameters
*
*++ This function is user defined. 
*++ All communication parameters from the Object Dictionary besides the
*++ bit rate and the node ID are set 
*++ to the default values before this function is called.
*++ The default values are taken from the object dictionary's
*++ "default" value, stored as \b defaultVal in the 
*++ object dictionary's \e VALUE_DESC_T struct.
*++ All node-id depending things are reset to
*++ predefined connection set, taking care of the
*++ internal global Variable \e coNodeId,
*++ which is normally set by the library with the call \e initCanopen()
*++ which calls \e getNodeId().
*++ This is only done for the first 4 TPDs and RPDOs, all
*++ others are set to invalid.
*++
*++ In this function the CANopen node can get a new bit rate 
*++ from a DIP-Switch or non volatile memory.
*++ For the CAN controller
*++ bit rate initialisation, the user is responsible.
*++ Further communication parameters can be loaded from a non volatile
*++ memory too. 
*++ In this case the object dictionary values have to be overwritten
*++ using \e putObj() and \e setCommPar() .
*
* \return
* nothing
*/
void resetCommInd(void)
{
  // get bit rate of device (EEPROM,DIP-Switch,...)
  // users_function()
  
  // sets CAN to new bitrate (manufacturer specific)
  //
  //Stop_CAN();
  //Start_CAN();
  //
  // get COB-IDs or other communication parameter from nonvolatile memory
  // users_function()
//  DS401_InitFlag = 1;
}


/*******************************************************************/
/**
*++ \brief newStateInd -  indicate transition to a new communcation state
*
*++ This function will be indicated at the slave, if it will be forced 
*++ to an other communication state.
*++ It is called before the transition change.
*++ This user interface ensures a save behaviour of the application,
*++ before the node goes to an other state.
*++ Additionaly a change to change to OPERATIONAL can be prevent.
*
* \retval CO_TRUE
*++ State change ok
* \retval CO_FALSE
*++ don't change the state to OPERATIONAL
*/
BOOL_T newStateInd(NODE_STATE_T newState) // new state
{
  // state classification

  switch(newState)
  {
	// Standard CANopen states
	case STOPPED:
	  break;
	case PRE_OPERATIONAL:
	  break;
	case OPERATIONAL:
      // send TPDOs automatically in the main loop
      // after state transition PRE-OPERATIONAL to OPERATIONAL
//      if (p301_n1_tpdo_para.transType == 255) // TD - test CANopen Hoogendoorn
        intTPDO[1] = 1;
//      if (p301_n2_tpdo_para.transType == 255)
        intTPDO[2] = 1;
//      if (p301_n3_tpdo_para.transType == 255)
        intTPDO[3] = 1;
//      if (p301_n4_tpdo_para.transType == 255)
        intTPDO[4] = 1;
//      if (p301_n5_tpdo_para.transType == 255)
        intTPDO[5] = 1;
//      if (p301_n6_tpdo_para.transType == 255)
        intTPDO[6] = 1;
//      if (p301_n7_tpdo_para.transType == 255)
        intTPDO[7] = 1;
//      if (p301_n8_tpdo_para.transType == 255)
        intTPDO[8] = 1;
//      if (p301_n9_tpdo_para.transType == 255)
        intTPDO[9] = 1;
//      if (p301_n10_tpdo_para.transType == 255)
        intTPDO[10] = 1;
//      if (p301_n11_tpdo_para.transType == 255)
        intTPDO[11] = 1;
      break;
	default:
	  break;
  }
  return(CO_TRUE);
}
/*______________________________________________________________________EOF_*/
