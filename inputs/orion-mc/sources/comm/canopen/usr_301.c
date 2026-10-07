/*
 *++ usr_301 - modul for user interfaces 
 *
 * Copyright (c) 1997-2002 port GmbH Halle (Saale)
 *------------------------------------------------------------------
 * $Header$
 *
 *------------------------------------------------------------------
 *
 * modification history
 * --------------------
 * $Log$
 * Revision 1.1  2011-10-10 11:33:17+02  driet
 * <>
 *
 * Revision 1.0  2011-07-18 11:48:58+02  driet
 * Initial revision
 *
 * Revision 1.14  2010-04-02 11:40:31+02  poelj
 * <>
 *
 * Revision 1.13  2009-11-26 09:21:34+01  driet
 * <>
 *
 * Revision 1.12  2009-09-21 13:09:09+02  poelj
 * <>
 *
 * Revision 1.11  2009-09-14 14:14:20+02  poelj
 * <>
 *
 * Revision 1.10  2009-06-16 16:23:11+02  poelj
 * <>
 *
 * Revision 1.9  2008-10-24 16:43:39+02  driet
 * <>
 *
 * Revision 1.8  2008-09-22 15:21:52+02  driet
 * <>
 *
 * Revision 1.7  2008-09-17 17:06:15+02  driet
 * <>
 *
 * Revision 1.6  2008-06-23 17:29:24+02  driet
 * <>
 *
 * Revision 1.5  2008-04-11 10:28:51+02  driet
 * <>
 *
 * Revision 1.4  2008-03-31 13:09:58+02  driet
 * <>
 *
 * Revision 1.3  2008-03-31 13:02:18+02  driet
 * <>
 *
 * Revision 1.2  2008-03-26 17:06:05+01  driet
 * <>
 *
 * Revision 1.1  2008-03-12 16:32:13+01  driet
 * <>
 *
 * Revision 1.0  2008-03-06 17:15:40+01  driet
 * Initial revision
 *
 * Revision 2.17  2003/02/03 16:02:22  ro
 * sdoRdCon() and sdoWrCon() corrected
 *
 * Revision 2.16  2003/01/21 09:47:08  boe
 * lss master indication modified
 *
 * Revision 2.15  2002/12/11 08:52:28  boe
 * adaption for library v4.3
 *
 * Revision 2.14  2002/08/30 10:08:46  boe
 * Returnvalue for testSdoVal changed to RET_T
 *
 * Revision 2.13  2002/05/30 13:48:48  hae
 * documentation correction
 *
 * Revision 2.12  2002/05/29 12:06:05  ro
 * return value of sdoRdInd() changed to RET_T
 *
 * Revision 2.11  2002/05/21 15:07:18  boe
 * change retval from sdo indications
 * add led and lss indication functions
 *
 * Revision 2.9  2001/07/09 16:22:39  boe
 * SDO abortcodes adapted
 *
 * Revision 2.8  2001/04/05 08:40:31  boe
 * comment changed
 *
 * Revision 2.7  2001/03/28 13:09:41  boe
 * comments changed
 *
 * Revision 2.6  2001/03/15 08:48:09  ro
 * new Header files for Lib V4.2
 * Server/Client moved to Producer/Consumer
 *
 * Revision 2.5  2000/08/31 13:31:58  boe
 * New indication for bootup and heartbeat started added at function mGuardInd()
 *
 * Revision 2.4  2000/07/27 10:03:03  boe
 * function pdoTimerInd() added for readPdo timer events
 *
 * Revision 2.3  2000/06/23 10:02:55  oe
 * Rework for Reference Manual
 *
 * Revision 2.2  2000/06/21 09:08:46  boe
 * more comments added
 *
 * Revision 2.1  2000/06/09 14:56:06  boe
 * parameter for testSdoValue changed
 *
 * Revision 2.0  2000/01/21 11:29:47  boe
 * Überarbeitet und an Version 4.0 angepasst
 *
 *
 *
 *
 *------------------------------------------------------------------
 */


/**
*  \file usr_301.c
*  \author port GmbH Halle (Saale)
*  $Revision$
*  $Date$
*
*++ This modul contains functions for the error handling
*++ (CAN controller errors, node-guarding errors, abort domain transfer)
*++ by the application.
*++ Furthermore interface routines for receiving SDOs and PDOs are defined here.
*++ The functions are called by the CANopen communication services.
*++ The user is responsible for the content of all these functions.
*
*/

/* header of standard C - libraries */

#include <stdio.h>

/* header of project specific types */

#include "ch_define.h"

#ifdef CANopen

#include "ch_alarm.h"
#include "ch_DS301.h"

#include <cal_conf.h>
#include <co_sdo.h>
#include <co_pdo.h>
#include <co_emcy.h>
#include <co_flag.h>
#include <co_odidx.h>
#include <co_usr.h>
#include <co_nmt.h>

#include <pdo.h>
#include "ch_ds401.h"

#if defined(CONFIG_MASTER) || defined(CONFIG_SLAVE_PLUS)
#include <co_nmt_m.h>
#endif /* defined(CONFIG_MASTER) || defined(CONFIG_SLAVE_PLUS) */

#if defined(CONFIG_LSS_MASTER) || defined(CONFIG_LSS_SLAVE)
# include <co_lss.h>
#endif /* defined(CONFIG_LSS_MASTER) || defined(CONFIG_LSS_SLAVE) */

#include <co_acces.h>
#include <co_stru.h>

#define DEF_OBJ_DIC // used to make the variable definitions
#include "objects.h"

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

/* local defined variables
---------------------------------------------------------------------------*/
#ifdef CONFIG_RCS_IDENT
static char _rcsid[] = "$Id$";
#endif

/*******************************************************************/
/**
*
*++ \brief getNodeId - get the node ID of the device
*
*++ This function has to be filled by the user.
*++ It returns the node ID of the device from e.g. a DIP switch
*++ or nonvolatile memory to the CANopen layer.
*
*++ It is called from the CANopen layer
*++ to initialize node ID dependent COB-IDs.
*
* \return node-id
*++ node ID in the range of 1..127
*/

UNSIGNED8 getNodeId(void)
{
  return (opt_alg.adres);
}

#ifdef CONFIG_EMCY_CONSUMER
/*******************************************************************/
/**
*++ \brief emcyInd - indicate the occurence of an emergency object
*
*++ In this function the user has to define his application specific
*++ error handling.
*++ The function should send a message to the server in
*++ order to repair the error.
*
* \return
*++ nothing
*/
void emcyInd(UNSIGNED8 emcyNr) // emergency number
{
EMERGENCY_T *pEmcy;

  pEmcy = readEmcy(emcyNr);
  switch (pEmcy->errCode & 0xFF00)
  {
    case 0x4000:
      break;
    case 0x5000:
      break;
    case 0x6000:
      break;
    default:
      break;
  }
}
#endif // CONFIG_EMCY_CONSUMER

#ifdef CONFIG_TIME_CONSUMER
/*******************************************************************/
/**
*
*++ \brief timeInd - indicate the occurence of a Time Stamp Object
*
*++ In this function the user has to define his application specific
*++ itime stamp handling.
*++ The \c TIME_OF_DAY_T structure, referenced by \em address, contains
*++ the time in ms after midnight and the number of day since January 1, 1984.
*
* \return
*++ nothing
*
*/

void timeInd(
     TIME_OF_DAY_T *address   /**< Time Stamp Object */
     )
{
    /* printf("days %u time %lu",address->days,address->time); */
}
#endif /* CONFIG_TIME_CONSUMER */

#ifdef CONFIG_PDO_CONSUMER
/*******************************************************************/
/**
*
*++ \brief pdoInd - indicate the occurence of a PDO
*
*++ In this function the user has to define his application specific
*++ handling for PDOs.
*
* \return
*++ nothing
*
*/

void pdoInd(
     UNSIGNED16 pdoNr    /**< nr of PDO */
     )
{
UNSIGNED16 index;       /* object index */
UNSIGNED16 mapIndex;    /* mapping index */
UNSIGNED8  mapSubIndex; /* mapping subindex */
UNSIGNED32 dummy, size; /* dummy U32 var */
UNSIGNED8  entries;     /* nr entries in pdo */
UNSIGNED8  mapNr;

  index = RPDO_MAP_BASE_INDEX + pdoNr - 1;

  /* test count of mapping entries for variable mapping */
  if (getObjEntry(index, 0, &entries, &size, CO_TRUE CO_COMMA_LINE_PARA) != CO_OK)
    return;

  for (mapNr = 1; mapNr <= entries; mapNr++)
  {
    /* get mapping data */
    if (getObjEntry(index, mapNr, (UNSIGNED8 *)&dummy, &size, CO_TRUE CO_COMMA_LINE_PARA) != CO_OK)
    {
      return;
    }
    mapIndex    = (UNSIGNED16)((dummy  >> MAP_INDEX_SHIFT)    & MAP_INDEX_MASK);
    mapSubIndex = (UNSIGNED8) ((dummy  >> MAP_SUBINDEX_SHIFT) & MAP_SUBINDEX_MASK);

    switch (mapIndex)
    {
      case I_DS401_WRITE_STATE_8:
        DigOutValid[mapSubIndex] = 1;
        break;
      case I_DS401_WRITE_AN_16:
        AnaOutValid[mapSubIndex] = 1;
        break;
    }
  }
}

#endif /* CONFIG_PDO_CONSUMER */


#if defined(CONFIG_PDO_CONSUMER) && defined(CONFIG_PDO_EVENTTIMER)
/*******************************************************************/
/**
*
*++ \brief pdoTimerInd - indicate the occurence of a PDO timer event
*
*++ In this function the user has to define his application specific
*++ handling for receive PDO timer events.
*++ For PDO remote requests (called with the function readPdoReq())
*++ the optional timer event time is used to watch the
*++ occurence of the requested PDO.
*++ If the PDO doesn't arrive in this period,
*++ this function will be called.
*
* \return
*++ nothing
*
*/

void pdoTimerInd(
     UNSIGNED16 pdoNr    /**< nr of PDO */
     )
{
  pdoNr;
}

#endif /* defined(CONFIG_PDO_CONSUMER) && defined(CONFIG_PDO_EVENTTIMER) */


#ifdef CONFIG_SDO_SERVER
/********************************************************************/
/**
*
*++ \brief sdoWrInd - indicate the occurence of a SDO write access
*
*++ This function is called if an SDO write request reaches the CANopen
*++ SDO server.
*++ Parameters of the function are the Index and Sub Index
*++ of the entry in the local object dictionary where the data
*++ should be written to.
*
*++ If numerical data with size up to 4 byte should be written,
*++ the library stores the prevoius value in in temporary buffer.
*++ The new value is put into the local object dictionary.
*
*++ If the application does not accept this new value,
*++ i.e. the function returns with a value > 0,
*++ the old value is restored from the temporary buffer to the
*++ object dictionary and the SDO write request will be answered
*++ with a "\b Abort \b Domain \b Transfer"
*++ by the library.
*++ The abort code can be specified by the \b return -value.
*
* \return
*++ The return value, which has to be specified by the application,
*++ selects the possible protocol answer of the write request
*++ to the SDO server.
* \retval CO_OK
*++ success
* \retval RET_T
*++ One of the valid, SDO related, values can be returned.
*++ This value is transferred  to  \em abortSdoTransf_Req() .
*++ Possible are:
* \li \c CO_E_NONEXIST_OBJECT
* \li \c CO_E_NONEXIST_SUBINDEX
* \li \c CO_E_NO_READ_PERM
* \li \c CO_E_NO_WRITE_PERM
* \li \c CO_E_MAP
* \li \c CO_E_DATA_LENGTH
* \li \c CO_E_TRANS_TYPE
* \li \c CO_E_VALUE_TO_HIGH
* \li \c CO_E_VALUE_TO_LOW:
* \li \c CO_E_WRONG_SIZE
* \li \c CO_E_PARA_INCOMP
* \li \c CO_E_HARDWARE_FAULT
* \li \c CO_E_SRD_NO_RESSOURCE
* \li \c CO_E_SDO_CMD_SPEC_INVALID
* \li \c CO_E_MEM
* \li \c CO_E_SDO_INVALID_BLKSIZE
* \li \c CO_E_SDO_INVALID_BLKCRC
* \li \c CO_E_SDO_TIMEOUT
* \li \c CO_E_INVALID_TRANSMODE
* \li \c CO_E_SDO_OTHER
* \li \c CO_E_DEVICE_STATE
*
*++ all other return values are defaulting to E_SDO_OTHER. 
*
*/

RET_T sdoWrInd(
      UNSIGNED16 index, /**< index to object */
      UNSIGNED8  subIndex   /**< subindex to object */
      )
{
UNSIGNED16 mapIndex;    /* mapping index */
UNSIGNED8  mapSubIndex; /* mapping subindex */
UNSIGNED32 dummy, size; /* dummy U32 var */
UNSIGNED16 i, nr = 0;
RET_T retval = CO_OK;

  // Check PDO mapping
  if (((index >= RPDO_MAP_BASE_INDEX) && (index <= RPDO_MAP_LAST_INDEX)) || ((index >= TPDO_MAP_BASE_INDEX) && (index <= TPDO_MAP_LAST_INDEX)))
  {
    if (subIndex != 0)
    {
      retval = getObjEntry(index, subIndex, (UNSIGNED8 *)&dummy, &size, CO_TRUE CO_COMMA_LINE_PARA);
      if (retval != CO_OK)
        return (retval);

      mapIndex    = (UNSIGNED16)((dummy  >> MAP_INDEX_SHIFT)    & MAP_INDEX_MASK);
      mapSubIndex = (UNSIGNED8) ((dummy  >> MAP_SUBINDEX_SHIFT) & MAP_SUBINDEX_MASK);
      switch (mapIndex)
      {
        case I_DS401_READ_STATE_8:
		  if (opt_app.LuchtmengkastGroep[0].Enabled && (opt_app.LuchtmengkastGroep[0].Buitenklep.TypeSturing == TYPE_STURING_CANOPEN))
		  {
            // LBK
		    for (i = 0; i < MAX_LUCHTMENGKAST_GROEP; i++)
		    {
		      if (opt_app.LuchtmengkastGroep[i].Enabled)
			    nr = i + 1;
  		    }
		    if (mapSubIndex > ((nr + 1) / 2))
		      return (CO_E_MAP);
		  }
		  else
		  {
		    // Motorgroup
            nr = (opt_app.NumberMotorgroups * 5) / 8;
            if ((opt_app.NumberMotorgroups * 5) % 8)
              nr += 1;
            if (mapSubIndex > nr)
		      return (CO_E_MAP);
		  }
          break;
        case I_DS401_WRITE_STATE_8:
		  if (opt_app.LuchtmengkastGroep[0].Enabled && (opt_app.LuchtmengkastGroep[0].Buitenklep.TypeSturing == TYPE_STURING_CANOPEN))
		  {
            // LBK
			return (CO_E_MAP);
		  }
		  else
		  {
		    // Motorgroup
            nr = (opt_app.NumberMotorgroups * 2) / 8;
            if ((opt_app.NumberMotorgroups * 2) % 8)
              nr += 1;
            if (mapSubIndex > nr)
              return (CO_E_MAP);
		  }
          break;
        case I_DS401_READ_AN_16:
        case I_DS401_WRITE_AN_16:
		  if (opt_app.LuchtmengkastGroep[0].Enabled && (opt_app.LuchtmengkastGroep[0].Buitenklep.TypeSturing == TYPE_STURING_CANOPEN))
		  {
            // LBK
		    for (i = 0; i < MAX_LUCHTMENGKAST_GROEP; i++)
		    {
		      if (opt_app.LuchtmengkastGroep[i].Enabled)
			    nr = i + 1;
  		    }
            if (mapSubIndex > (nr * 4))
		      return (CO_E_MAP);
		  }
		  else
		  {
		    // Motorgroup
            if (mapSubIndex > opt_app.NumberMotorgroups)
              return (CO_E_MAP);
		  }
          break;
      }
    }
  }
  else //
  {
    switch (index)
    {
      case I_DS401_WRITE_STATE_8:
        DigOutValid[subIndex] = 1;
        break;
      case I_DS401_WRITE_AN_16:
        AnaOutValid[subIndex] = 1;
        break;
    }
  }
  return(CO_OK);
}


/********************************************************************/
/**
*++ \brief sdoRdInd - indicate the occurence of a SDO read access
*
*++ This function determines a reaction to the device for
*++ an index - subindex read access via SDO.
*++ With this function the object dictionary has to made up to date
*++ e.g. by reading of digital inputs.
*++ If an error occurs an \b abort \b domain \b transfer will be started.
*++ This function is called
*++ only for objects with an index greater 0x1FFF.
*
* \retval CO_OK
*++ success
* \retval RET_T
*++ One of the valid, SDO related, values can be returned.
*++ This value is transferred  to  \em abortSdoTransf_Req() .
*++ Possible are:
* \li \c CO_E_NONEXIST_OBJECT
* \li \c CO_E_NONEXIST_SUBINDEX
* \li \c CO_E_NO_READ_PERM
* \li \c CO_E_NO_WRITE_PERM
* \li \c CO_E_MAP
* \li \c CO_E_DATA_LENGTH
* \li \c CO_E_TRANS_TYPE
* \li \c CO_E_VALUE_TO_HIGH
* \li \c CO_E_VALUE_TO_LOW:
* \li \c CO_E_WRONG_SIZE
* \li \c CO_E_PARA_INCOMP
* \li \c CO_E_HARDWARE_FAULT
* \li \c CO_E_SRD_NO_RESSOURCE
* \li \c CO_E_SDO_CMD_SPEC_INVALID
* \li \c CO_E_MEM
* \li \c CO_E_SDO_INVALID_BLKSIZE
* \li \c CO_E_SDO_INVALID_BLKCRC
* \li \c CO_E_SDO_TIMEOUT
* \li \c CO_E_INVALID_TRANSMODE
* \li \c CO_E_SDO_OTHER
* \li \c CO_E_DEVICE_STATE
*
*++ all other return values are defaulting to E_SDO_OTHER. 
*
*/

RET_T sdoRdInd(UNSIGNED16 index,    // index to object
               UNSIGNED8  subIndex) // subindex to object
{
UNSIGNED8  *pData;
UNSIGNED32 Size;
RET_T ret;

  switch (index)
  {
    case 0x100A: // Software Version
      if ((ret = getObjAddr(index, subIndex, &pData, &Size)) != CO_OK)
        return (ret);
      pData[0] = '0' + ( rom.versie_programma / 100);
      pData[1] = '.';
      pData[2] = '0' + ((rom.versie_programma % 100) / 10);
      pData[3] = '0' + ( rom.versie_programma % 10);
      pData[4] = NULL;
      break;
  }
  return(CO_OK);
}


# ifdef CONFIG_VALUE_CHECK_FUNCTION
/********************************************************************/
/**
*
*++ \brief testSdoValue - check the value of a SDO before writing to OD
*
*++ In this function the user can check the value of a SDO, which should
*++ be written into the object dictionary. The user is responsible for
*++ the pointer conversion of \em pData.
*
* \retval CO_OK
*++ success
* \retval RET_T
*++ sdo abort error reason
*
*/

RET_T testSdoValue(
       UNSIGNED16 index,     /**< index of object */
      UNSIGNED8  subIndex,  /**< subindex of object */
       void       *pData,    /**< pointer to new data */
       UNSIGNED32 size       /**< data size */
       )
{

#ifdef CONFIG_BIG_ENDIAN
UNSIGNED8       convBuffer[4];  /* swapping buffer */
UNSIGNED8       i;              /* loop counter */

    /*
       converts only numerical values with 2 and 4 bytes
       the user is responsible that only numerical values
       will be converted
    */

    if(size <= 4)
    {
    for(i = 0; i < size; i++)
    {
        convBuffer[i] = ((UNSIGNED8 *)pData)[size-1-i];
    }
    pData = &convBuffer;
    }
#endif

/*
 checks contents of SDO

UNSIGNED8 tmpU8;

    memcpy(&tmpU8,pData,1);
    switch(index) {
    case 0x0000:
        if(subindex == ) {
        if(tmpU8 > )
            return CO_E_SDO_OTHER;
        }
        break;
    default:
    }
*/
    return CO_OK;
}
# endif /* CONFIG_VALUE_CHECK_FUNCTION */
#endif  /* CONFIG_SDO_SERVER */

#ifdef CONFIG_SDO_CLIENT
/*******************************************************************/
/**
*
*++ \brief sdoWrCon - confirmation function for SDO write access
*
*++ This function signs that the message sent by \em writeSdoReq()
*++ was confirmed by the SDO server.
*++ It handles errors and is useful for program synchronization.
*++ If the \em errorFlag isn't zero, the last SDO transfer
*++ was terminated by an \b abort \b domain \b transfer.
*++ The reason for the termination contains the \em errorFlag.
*++ The \em errorFlag can be combinations of the following constants:
*
* \retval E_SDO_NO_ERROR
* \retval E_SDO_SERVICE
* \retval E_SDO_INCONS_PARA
* \retval E_SDO_ILLEG_PARA
* \retval E_SDO_ACCESS
* \retval E_SDO_UNSUPP_ACCESS
* \retval E_SDO_NONEXIST_OBJECT
* \retval E_SDO_INVALID_ADDRESS
* \retval E_SDO_HARDWARE_FAULT
* \retval E_SDO_TYPE_CONFLICT
* \retval E_SDO_INCONS_OBJ_ATTR
* \retval E_SDO_OTHER
*
*/
void sdoWrCon(
     UNSIGNED8  sdoNr,       /**< number of SDO */
     UNSIGNED32 errorFlag    /**< errorflag, if zero sucess */
)
{
    switch (errorFlag & 0xFF000000)  {
    /* successful confirmation */
    case E_SDO_NO_ERROR:
        break;
    /* service error */
    case E_SDO_SERVICE:
        switch (errorFlag & 0x00FF0000) {
        /* inconsistent parameter */
        case E_SDO_INCONS_PARA:
            break;
        /* wrong communication parameter or
           internal sdo object not exist */
        case E_SDO_ILLEG_PARA:
            break;
        }
        break;
    /* access error */
    case E_SDO_ACCESS:
        switch (errorFlag & 0x00FF0000) {
        /* unsupported access */
        case E_SDO_UNSUPP_ACCESS:
            switch (errorFlag & 0x00000000FFUL) {
            /* No write permission */
            case E_SDO_A_NO_WRITE_PERM:
                break;
            /* read or write permission */
            default:
                break;
            }
            break;
        /* index doesn't exist */
        case E_SDO_NONEXIST_OBJECT:
            break;
        /* mapping fault */
        case E_PDO_MAPPING:
            break;
        /* harware fault not implement yet e.g EEPROM */
        case E_SDO_HARDWARE_FAULT:
            break;
        /* size of SDO value is not equal the defined size */
        case E_SDO_TYPE_CONFLICT:
            break;
        /* inconsistent attribut */
        case E_SDO_INCONS_OBJ_ATTR:
            switch (errorFlag & 0x00000000FFUL) {
            /* value higher than maximum */
            case E_SDO_A_NONEXIST_SUBINDEX:
                break;
            /* value higher than maximum */
            case E_SDO_A_VALUE_TO_HIGH:
                break;
            /* value lesser than minimum */
            case E_SDO_A_VALUE_TO_LOW:
                break;
            /* invalid value (testSdoValue) */
            case E_SDO_A_INVALID_VAL:
                 break;
            /* object attribute inconsistent */
            case E_SDO_A_VALUE_RANGE_EXCEED:
                break;
            default:
                break;
            }
        }
        break;
    case E_SDO_OTHER:
        break;
    default:
        break;
    }
}

/*******************************************************************/
/**
*
*++ \brief sdoRdCon - confirmation function for SDO read access
*
*++ This functions signs that the message sent by \em readSdoReq()
*++ was confirmed by the SDO Server.
*++ It handles errors and is useful for program synchronization.
*++ If the \em errorFlag isn't zero, the last SDO transfer
*++ was terminated by an \b abort \b domain \b transfer.
*++ The \em errorFlag contains the reason for the termination.
*++ In case of success the parameter \em pObj from \em readSdoReq()
*++ points to the read value.
*++ The \em errorFlag can be combinations of the following constants:
* \arg
*  E_SDO_NO_ERROR
* \arg
*  E_SDO_SERVICE
* \arg
*  E_SDO_INCONS_PARA
* \arg
*  E_SDO_ILLEG_PARA
* \arg
*  E_SDO_ACCESS
* \arg
*  E_SDO_UNSUPP_ACCESS
* \arg
*  E_SDO_NONEXIST_OBJECT
* \arg
*  E_SDO_INVALID_ADDRESS
* \arg
*  E_SDO_HARDWARE_FAULT
* \arg
*  E_SDO_TYPE_CONFLICT
* \arg
*  E_SDO_INCONS_OBJ_ATTR
* \arg
*  E_SDO_OTHER
*
* \return
* nothing
*
*/
void sdoRdCon(
     UNSIGNED8  sdoNr,       /**< number of SDO */
     UNSIGNED32 errorFlag    /**< errorflag, if zero sucess */
)
{
    switch (errorFlag & 0xFF000000)  {
    /* successful confirmation */
    case E_SDO_NO_ERROR:
        break;
    /* service error */
    case E_SDO_SERVICE:
        switch (errorFlag & 0x00FF0000) {
        /* inconsistent parameter */
        case E_SDO_INCONS_PARA:
            break;
        /* wrong communication parameter or
           internal sdo object not exist */
        case E_SDO_ILLEG_PARA:
            break;
        }
        break;
    /* access error */
    case E_SDO_ACCESS:
        switch (errorFlag & 0x00FF0000) {
        /* unsupported access */
        case E_SDO_UNSUPP_ACCESS:
            switch (errorFlag & 0x00000000FFUL) {
            /* No read permission */
            case E_SDO_A_NO_READ_PERM:
                break;
            /* read or write permission */
            default:
                break;
            }
            break;
        /* index doesn't exist */
        case E_SDO_NONEXIST_OBJECT:
            break;
        /* harware fault not implement yet e.g EEPROM */
        case E_SDO_HARDWARE_FAULT:
            break;
        /* size of SDO value is not equal the defined size */
        case E_SDO_TYPE_CONFLICT:
            break;
        /* inconsistent attribut */
        case E_SDO_INCONS_OBJ_ATTR:
            switch (errorFlag & 0x00000000FFUL) {
            /* value higher than maximum */
            case E_SDO_A_NONEXIST_SUBINDEX:
                break;
            /* object attribute inconsistent */
            case E_SDO_A_VALUE_RANGE_EXCEED:
                break;
            default:
                break;
            }
        }
        break;
    case E_SDO_OTHER:
        break;
    default:
        break;
    }
}
#endif /* CONFIG_SDO_CLIENT */

/* functions for Nodeguarding/Heartbeat */

#if defined(CONFIG_HEARTBEAT_CONSUMER) || defined(CONFIG_NODE_GUARDING)

# if defined(CONFIG_MASTER) || defined(CONFIG_SLAVE_PLUS)
/********************************************************************/
/**
*
*++ \brief mGuardErrorInd - indicate the occurence of a nodeguarding/heartbeat error
*
*++ This function defines the reaction for node guading error
*++ or a heartbeat event (start heartbeat, bootup, ...)
*++ which will be indicated to the master.
*
*++ Meaning of the parameter:
*
* \li  CO_LOST_GUARDING_MSG
* \par
*++ Guarding time has elapsed or toggle bit has not altered.
*
* \li  CO_LOST_CONNECTION
* \par
*++ The lifetime (lifetime factor * guarding time) is elapsed.
*
* \li  CO_NODE_STATE
* \par
*++ The guarding node has not the expected state.
*
* \li  CO_BOOT_UP
* \par
*++ Bootup Message was received
*
* \li  CO_HB_STARTED
* \par
*++ First Heartbeat message was received
*
* \li  CO_LOST_HEARTBEAT
* \par
*++ Heartbeat missing (heartbeat will be disabled)
*
* \return
*++ nothing
*
*/

void mGuardErrorInd(
     UNSIGNED8 nodeId, // node ID of error source
     ERROR_SPEC_T kind // kind of error
     )
{
  switch(kind)
  {
    case CO_LOST_HEARTBEAT:
      // Enter Pre-operational
      setNodePREOP();
      if (CreateAlarm(&alarm_hr_alg.can_al, CAN_AL, nodeId, 0, 0, ZACHT_ALARM))
      {
        alarm_hr_alg.computer_al_nr = nodeId;
        CANopenEmcyReq(0x8130, EMCY_LOST_HEARTBEAT | nodeId, 0, 0);
      }
      break;
    case CO_HB_STARTED:
      if (ClearAlarm(&alarm_hr_alg.can_al, CAN_AL, nodeId))
      {
        CANopenEmcyErase(0x8130, EMCY_LOST_HEARTBEAT | nodeId, 0, 0);
      }
      break;
    default:
      break;
  }
}

# endif /* CONFIG_MASTER */

/********************************************************************/
/**
*
*++ \brief sGuardErrorInd - indicate the occurence of a nodeguarding error
*
*++ This function defines the reaction for node guarding errors
*++ which will be indicated to the local slave.
*++ One suspended guarding is generated by the timer resolution,
*++ therefore only the second missed guarding will be showed.
*
*++ Meaning of the parameter:
*
* \li CO_GUARDING_STARTED
* \par
*++ shows that guarding was (re) started
*
* \li  CO_LOST_GUARDING_MSG
* \par
*++ Guarding time is elapsed at least the second time.
*
* \li  CO_LOST_CONNECTION
* \par
*++ The lifetime (lifetime factor * guarding time) is elapsed.
*
* \retval 0
*++ node should keep in the current state
* \retval 1
*++ node should be forced to the state PRE_OPERATIONAL
*/

UNSIGNED8 sGuardErrorInd(
      ERROR_SPEC_T kind  /**< kind of error */
      )
{
    switch(kind) {
    case CO_GUARDING_STARTED:
        return 0;
    case CO_LOST_GUARDING_MSG:
        return 0;
    case CO_LOST_CONNECTION:
        return 1;
    default:
        return 0;
    }
}

#endif /* CONFIG_NODE_GUARDING */

/* interface functions to no volatile memory */

#ifdef CONFIG_NON_VOLATILE_MEM
/********************************************************************/
/**
*
*++ \brief saveParameterInd - indicate a store to non volatile memory command
*
*++ This function indicates a "store parameters to non volatile memory" 
*++ command via SDO.
*++ This command is a SDO write access to object 0x1010 with the signature
*++ "save".
*++ In this function the user has to implement his target specific
*++ save functions.
*++ If this function returns an error an \b abort \b domain \b transfer will
*++ be initiate with the error code "hardware fault".
*++ The parameter segment corresponds by subindex the subindex from 1010.
*
* \retval CO_TRUE
*++ success
* \retval CO_FALSE
*++ error
*
*/

BOOL_T saveParameterInd(
       UNSIGNED8 segment   /**< subindex which specifies the memory segment */
       )
{
    switch(segment)
    {
    /* all parameters */
    case MEM_SEG_ALL_PARAMETERS:
        break;
    /* communication parameter */
    case MEM_SEG_COM_PARAMETERS:
        break;
    /* application parameter */
    case MEM_SEG_APPL_PARAMETERS:
        break;
    /* segment 4 - 127 manufacturer specific */
    default:
        return CO_FALSE;
    }
    return CO_TRUE;
}


/********************************************************************/
/**
*
*++ \brief clearParameterInd - indicate a clear non volatile memory command
*
*++ This function indicates a "clear to non volatile memory"
*++ command via SDO.
*++ This command is a SDO write access to object 0x1010 with the signature
*++ "\b kill".
*++ This signature is an extension of the library by \em port.
*++ In this function the user has to implement his target specific
*++ clear functions.
*++ If this function returns an error an \b abort \b domain \b transfer will
*++ be initiate with the error code "hardware fault".
*
* \retval
* CO_TRUE
*++ success
* \retval
* CO_FALSE
*++ error
*
*/

BOOL_T clearParameterInd(
       UNSIGNED8 segment   /**< subindex which specifies the memory segment */
       )
{
    switch(segment)
    {
    /* all parameters */
    case MEM_SEG_ALL_PARAMETERS:
        break;
    /* communication parameter */
    case MEM_SEG_COM_PARAMETERS:
            break;
    /* application parameter */
    case MEM_SEG_APPL_PARAMETERS:
        break;
    /* segment 4 - 127 manufacturer specific */
    default:
        return CO_FALSE;
    }

    return CO_TRUE;
}

/********************************************************************/
/**
*
*++ \brief loadParameterInd - indicate a restore from non volatile memory command
*
*
*++ This function is called
*++ to load the object dictionary 
*++ with default values or saved values from Flash or EEPROM.
*
*++ It is called from the library
*++ at bootup, resetCommunication and after write at object 0x1011,
*++ after the object dictionary was written by default values.
*
*++ The parameter segment describes,
*++ which part of the object dictionary
*++ should be actualized.
*
*++ The parameter mode says,
*++ which restore mode should be used:
* BOOTUP
*++ (Only called at bootup)
*++ The object dictionary should be overwritten
*++ by configured data from nonvolatile memory.
* RESET_COMM
*++ (It's called at reset communication, if there wasn't a write access 
*++ at object 0x1011 before)
*++ The object dictionary should be overwritten
*++ by configured data from nonvolatile memory.
* SDO
*++ (Only called at write access to object 0x1011)
*++ The object dictionary was restored by the library
*++ by the default values from EPROM.
*++ If other values are necessary,
*++ they can be modified here.
*++ Please Note:
*++ the default Parameter are only valid after a reset communication !
*
*++ If this function returns an error an \b abort \b domain \b transfer will
*++ be initiate with the error code "hardware fault".
*
* \retval
* CO_TRUE
*++ success
* \retval
* CO_FALSE
*++ error
*
*/

BOOL_T loadParameterInd(
    UNSIGNED8 segment,  /**< specifies the segment */
    UNSIGNED8 mode      /**< restore mode */
       )
{
  switch (mode)
  {
    default:
      break;
  }
  switch (segment)
  {
    case MEM_SEG_ALL_PARAMETERS: // all parameters
      break;
    case MEM_SEG_COM_PARAMETERS: // communication parameter
      break;
    case MEM_SEG_APPL_PARAMETERS: // application parameter
      break;
    default: // segment 4 - 127 manufacturer specific
      return CO_FALSE;
  }
  return CO_TRUE;
}

#endif /* CONFIG_NON_VOLATILE_MEM */


/* error handling for CAN controller and receive and transmit buffer */

#ifdef CONFIG_CAN_ERROR_HANDLING
/********************************************************************/
/**
*
*++ \brief canErrorInd - indicate the occurence of an error on the CAN driver
*-- \brief canErrorInd - zeigt das Aufreten eines CAN Treiberfehlers an
*
*++ This function indicates one of the following errors:
*-- Diese Funktion zeigt das Auftreten eines der folgenden Fehler an:
*
* \retval
* COFLAG_CAN_BUSOFF
*++ CAN-controller error
*-- Fehler vom CAN Controller
* \retval
* COFLAG_CAN_PASSIVE
*++ CAN-controller error
*-- Fehler vom CAN Controller
* \retval
* COFLAG_CAN_OVERFLOW
*++ CAN-controller overrun error
*-- Overrun Fehler vom CAN Controller
* \retval
* COFLAG_BUFFER_OVERFLOW
*++ receive/transmit buffer overflow
*-- Empfangs/Sendepuffer übergelaufen
*
* \retval
* CO_TRUE
*++ CAN controller has to stay in the current state
*-- CAN Controller soll im aktuellen Zustand bleiben
* \retval
* CO_FALSE
*++ CAN controller has to go to BUS ON again
*-- CAN Controller soll wieder nach BUS ON gehen
*
*/

BOOL_T canErrorInd(
    UNSIGNED8 errorFlags
    )
{
    printf("canErrorInd: ");
    if ((TEST_COLIB_FLAG(errorFlags & COFLAG_CAN_BUSOFF)) != 0)  {
    printf("BUS_OFF\n");
    }
    if ((TEST_COLIB_FLAG(errorFlags & COFLAG_CAN_OVERFLOW)) != 0)  {
    printf("OVERRUN\n");
    }
    if ((TEST_COLIB_FLAG(errorFlags & COFLAG_CAN_PASSIVE)) != 0)  {
    printf("ERROR_PASSIVE\n");
    }
    if ((TEST_COLIB_FLAG(errorFlags & COFLAG_BUFFER_OVERFLOW)) != 0)  {
    printf("ERROR_BUFFER_OVERFLOW\n");
    }

    return(CO_TRUE);
}
       
#endif /* CONFIG_CAN_ERROR_HANDLING */

#ifdef CONFIG_SYNC_CMD
/*******************************************************************/
/**
*
*++ \brief syncCommand - call application specific commands after SYNC
*-- \brief syncCommand - Aufruf von applikationsspezifischen Kommandos nach SYNC
*
*++ This function will be called after the SYNC is received.
*++ The user can define own reactions for the SYNC telegram.
*-- Diese Funktion wird nach dem Empfang eines SYNC Telegramms aufgerufen.
*-- Der Anwender kann mit dieser Funktion eigene Reaktionen
*-- auf den Erhalt des SYNC Impuls definieren.
*
*
* \return
*++ nothing
*-- nichts
*
*/

void syncCommand(
    void
    )
{

}

#endif /* CONFIG_SYNC_CMD */


#ifdef CONFIG_UPDATE_SYNC_ACTUAL_MSG
/*******************************************************************/
/**
*
*++ \brief updateSyncActualMsg - update the content of sync. PDOs
*-- \brief updateSyncActualMsg - aktualisert den Inhalt sync. PDOs
*
*++ This function updates the content of synchronous PDOs at the
*++ occurence of a SYNC.
*++ The application has to pass the data in encoded form.
*++ The simplest way is to call \em writePdoReq(pdoNr).
*++ That function updates the buffer for synchronous messages.
*++ The user has to save \em writePdoReq(pdoNr) for interruptions,
*++ because this function isn't reentrant.
*-- Diese Funktion aktualisert den Inhalt synchroner PDOs beim Eintreffen
*-- der SYNC Nachricht.
*-- Die Daten sind von der Applikation in kodierter Form breitzustellen.
*-- Im einfachsten Fall ist \em writePdoReq(pdoNr) aufzurufen, die
*-- die Puffer aktualisiert.
*-- Es ist aber zu beachten, daß \em writePdoReq(pdoNr) gegen Unterbrechung
*-- zu schützen ist, da sie nicht reentrant ist.
*
* \return
*++ nothing
*-- nichts
*
*/

void updateSyncActualMsg(
     UNSIGNED16 pdoNr,         /**< number of RPDO */
     UNSIGNED8 CO_FAR *pData   /**< pointer to encoded data buffer */
     ) 
{
    /* user specific code here */

}
#endif /* CONFIG_UPDATE_SYNC_ACTUAL_MSG */


#ifdef CONFIG_LSS_MASTER
/*******************************************************************/
/**
*
*++ \brief lssMasterCon - master confirmation
*-- \brief lssMasterCon - Master Confirmation
*
*-- Diese Funktion wird bei einer Master Confirmation aufgerufen.
*-- Verschiedene LSS Nachrichten erfordern eine Antwort vom LSS Slave.
*-- Sobald diese Antwort eingetroffen ist,
*-- oder die Timeout Zeit abgelaufen ist,
*-- wird diese Funktion aufgerufen.
*++ This function is called by a master confirmation.
*++ LSS messages require a response of a LSS slave.
*++ This function is called as soon as the response was received
*++ or a timeout was occured.
*-- Der Parameter mode signalisiert,
*-- ob ein timeout vorliegt
*-- oder eine Antwort vom Slave eingetroffen ist.
*-- Fehler oder Rückgabewerte des Slaves
*-- werden in den übergebenen Parametern 1 und 2 übergeben.
*-- Die Bedeutung der Parameter 1 und 2 ist abhängig von dem 
*-- gesendeten Kommando und hat folgende Bedeutung:
*++ The parameter mode indicates
*++ if a timeout is occured
*++ or an answer was received.
*++ Error or other return values
*++ are provide by the parameter 1 and 2.
*++ The meaning of parameter 1 and 2 depends on the last command
*++ and have following meaning:
*
* mode          para1       para2   Bedeutung
* LSS_CON_TIMEOUT   -       -   no answer received
* LSS_CON_ANSWER_OK -       -   answer received (no error)
* LSS_CON_ANSWER_DATA   u32 data value  -   answer and data received
* LSS_CON_ANSWER_NODEID u8 node id  -   answer and node id received
* LSS_CON_ANSWER_ERROR  ErrorCode   ManCode answer with error received
*
*
* \return
*++ nothing
*-- nichts
*
*/

void lssMasterCon(
    UNSIGNED8 mode,     /**< lss confirmation mode */
    UNSIGNED8 *par1,    /**< pointer to additional data 1 */
    UNSIGNED8 *par2     /**< pointer to additional data 2 */
     ) 
{
    switch (mode)  {
    case LSS_CON_TIMEOUT:   /* no answer from lss slave */
        break;

    case LSS_CON_ANSWER_OK: /* answer from lss slave ok */
        break;

    case LSS_CON_ANSWER_DATA:   /* answer from lss with data */
        break;

    case LSS_CON_ANSWER_NODEID: /* answer inquire node id */
        break;

    case LSS_CON_ANSWER_ERROR:  /* LSS confirmation answer error */
        break;
    }
}

#endif /* CONFIG_LSS_MASTER */

#ifdef CONFIG_LSS_SLAVE
/********************************************************************/
/**
*
*++ \brief lssSlaveInd - indicate a LSS command
*-- \brief lssSlaveInd - zeigt LSS slave Kommando an
*
*-- Diese Funktion wird beim Eintreffen eines LSS Kommandos aufgerufen.
*-- Der Parameter cmd spezifiziert das erhaltene Kommando vom LSS Master.
*-- Die folgenden 2 Parameter sind abhängig vom gesetzten Kommando.
*-- Mögliche Kommandos sind:
*++ This function is called when a LSS command was received.
*++ The parameter cmd specifies the received command.
*++ The following two parameter are depending on cmd.
*++ Possible commands:
* \code
*-- Kommando        Para1   Para2
*++ command
* LSS_IND_NODEID    new-id          new node id
* LSS_IND_STORE                 store parameter
* LSS_IND_BITRATE   table   index       new bitrate received
* LSS_IND_BITRATE_SET               new bitrate will be set
* LSS_IND_BITRATE_ACTIVE            new bitrate has been activated
* \endcode
*
*-- Das Kommando BITRATE enthält die neue Bitrate.
*-- Der Anwender muss sicherstellen,
*-- dass die gewünschte Bitrate auch einstellbar ist.
*-- Anderenfalls ist eine negativer Rückgabewert zu generieren.
*-- Die neue Bitrate darf zu diesem Zeitpunkt nicht eingestellt werden !
*-- Erst nach einem Reset Communication
*-- oder einem entsprechenden LSS Kommando
*-- darf auf die neue Bitrate umgeschalten werden.
*++ The command LSS_IND_BITRATE contains the new bit rate.
*++ The user has to asure that the desired bit rate can be configured.
*++ Otherwise a negative return value has to be generated.
*++ The new bit rate has to be set only after a reset communication or
*++ an equivalent LSS command.
* \par
*-- Der Umschaltzeitpunkt über ein LSS Kommando
*-- wird über diese Indikationfunktion und dem mode LSS_IND_BITRATE_SET
*-- signalisiert. Vor dem Aufruf wird die vom LSS vorgegebene Delay-Zeit
*-- abgewartet.
*-- Der Anwender kann nun die Bitrate entsprechend den Vorgaben einstellen.
*++ The command LSS_IND_BITRATE_SET signals that
*++ the device should switch to the new bit rate.
*++ Before switching to the new bit rate 
*++ the device must wait a pedefined time.
*++ After this time the user can switch to the new bit rate.
* \par
*-- Anschließend muß nochmals die Delay-Zeit abgewartet werden,
*-- bevor neue Messages gesendet werden dürfen.
*-- Dies erfolgt automatisch in der Library.
*-- Das Ende der Delay-Zeit wird mit dieser Indikationfunktion und dem Mode
*-- LSS_IND_BITRATE_ACTIVE
*-- angezeigt.
*++ Then again the device has to wait for a predefined time
*++ before it is allowed to send messages again.
*++ The end of this second delay time is indicated with the command
*++ LSS_IND_BITRATE_ACTIVE.
*
*-- Wenn bei der Bearbeitung Fehler aufgetreten sind,
*-- kann das dem LSS Master durch einen Rückgabewert != 0 signalisiert werden.
*++ If an error occured during proccessing the command
*++ a return value != 0 will signal that to the LSS master.
* \par
*-- Beim Kommando LSS_IND_NODEID muss die Funktion ggf. alle COB-Ids
*-- für die initialisierten Dienste anpassen,
*-- die direkt von der Knotennummer abhängen.
*-- Die Library aktualisiert automatisch die COB-Ids von
*-- \li ersten Server SDO (0x580/0x600 + node-id)
*-- \li Emergency (0x80 + node-id)
*-- \li NMT-Err (Heartbeat,Node-Guarding) (0x700 + node-id)
*-- unter der Voraussetzung, das die Funktion ohne Fehler zurückkehrt.
*++ The command LSS_IND_NODEID requires to adjust all COB-IDs for the
*++ initialized services that are depending on the node id.
*++ The CANopen library adjusts the COB-IDs of
*++ \li first server SDO (0x580/0x600 + node id)
*++ \li emergency object (0x80 + node id)
*++ \li NMT-Err (Heartbeat, Node-Guarding) (0x700 + node id)
*
* \retval
* 0
*++ success
*--Erfolg
* \retval
* != 0
*++ errorcode
*-- Fehlercode
* \retval
* 1
*-- Nicht unterstützte Tabelle
*
*/

UNSIGNED8 lssSlaveInd(
    UNSIGNED8 art,      /**< lss indication art */
    UNSIGNED8 para1,    /**< parameter 1 */
    UNSIGNED8 para2     /**< parameter 1 */
       )
{
    switch (art) {
    case LSS_IND_NODEID:        /* new node id is set */
        break;

    case LSS_IND_BITRATE:       /* new bitrate */
        /* allow only the canopen table */
        if (para1 != 0)  {
        return(1);      /* only canopen stand. table supported*/
        }
        if (para2 > 8)  {
        return(2);      /* only 9 bitrates supported*/
        }
        break;

    case LSS_IND_BITRATE_SET:   /* set the new bitrate */
        break;

    case LSS_IND_BITRATE_ACTIVE:    /* new bitrate is activeted */
        break;

    case LSS_IND_STORE:     /* save the new data */
        break;
    }
    return 0;
}

#endif /* CONFIG_LSS_SLAVE */


#ifdef CONFIG_CO_LED

/********************************************************************/
/**
*
*-- \brief ledInd - schaltet die CANopen LED an bzw. aus
*++ \brief ledInd - switch CANopen indicator on or off
*
*-- Diese Funktion dient zum Setzen (Ein/Ausschalten) der CANopen LEDs.
*-- Sie wird von der Bibliothek gemäss dem
*-- NMT-Zustand bzw. Fehlerzustand aufgerufen.
*-- Der Parameter led zeigt an, welche der beiden LEDs
*--\code
*--     CO_ERR_LED  Fehler LED
*--     CO_RUN_LED  Status LED
*--\endcode
*-- betätigt werden soll.
*
*-- Der einzustellende Zustand für diese LED
*--\code
*--     CO_LED_ON   LED einschalten
*--     CO_LED_OFF  LED ausschalten
 --\endcode
*-- wird über den Parameter action übergeben.
*
*
*++ This function switches the CANopen LEDs on or off.
*++ It is called from the library according to the
*++ NMT-state and error state, respectively.
*++ The parameter led determines which of the two LEDs to act on.
*++\code
*++     CO_ERR_LED  Error LED
*++     CO_RUN_LED  NMT LED
 ++\endcode
*
*++ The state for this LED is handed over
*++\code
*++     CO_LED_ON   turn LED on
*++     CO_LED_OFF  turn LED off
*++\endcode
*++ within the parameter action.

* \return 
*-- nichts
*++ nothing
*/

void ledInd (
    UNSIGNED8 led,      /**< which CANopen LED */
    UNSIGNED8 action    /**< turn LED on or off */
    )
{
    if (led == CO_ERR_LED) {
    if (action == CO_LED_ON) {
        /* switch Error LED on */
    } else {
        /* switch Error LED off */
    }
    }
    if (led == CO_RUN_LED) {
    if (action == CO_LED_ON) {
        /* switch Status LED on */
    } else {
        /* switch Status LED off */
    }
    }
}
#endif /* CONFIG_CO_LED */


#ifdef CONFIG_USER_TIMER_EVENT
/********************************************************************/
/**
*
*++ userTimerEvent - signals a user timer event
*-- userTimerEvent - signallisiert ein Anwender Timer Ereignis
*
* \return
*++ nothing
*-- nichts
*
*/

void userTimerEvent(
    TIMER_EVENT_T *pTimer
    )
{
    /* if this the invalid data timer - for invalid data from MCB */
    if (pTimer == &invalidTime)  {
    }
}


#endif /* CONFIG_USER_TIMER_EVENT */


#ifdef CONFIG_VIRTUAL_OBJECTS
/****************************************************************************/
/**
*
*++ \brief getVirtualObjAddr - get address of a virtual object
*-- \brief getVirtualObjAddr - liefert Adresse eines virtuellen Objektes
*
*++ This function supports the handling of \em virtual objects
*++ in the manufacturer specific part of the object dictionary
*++ and the acces from the CANopen network by uing SDOs.
*++ The function delivers the address of an \em virtual object
*++ referenced by \em index and \em subIndex.
*-- Die Funktion ermöglicht die Nutzung von virtuellen Objekten
*-- im herstellerspezifischen Bereich des Objektverzeichnis
*-- für den Zugriff per SDO.
*
*++ The CANopen layer calls this user provided funtion
*++ every time it finds no real object dictionary entry.
*++ The application programmer has to provide a pointer to some data
*++ and a length information for this data.
*++ Allowed is only a maximum of up to four bytes.
*++ Is this referenced \em virtual object not available
*++ the correct SDO abort code has to be provided as 
*++ an valid \c RET_T 
*-- Immer wenn kein lokales Objekt ermittelt werden konnte,
*-- wird diese Funktion aufgerufen.
*-- Der Anwender muss für das
*-- über den Index und Subindex übergebene virtuelle Objekt
*-- einen Zeiger auf die Daten
*-- und die Länge des virtuellen Objekts 
*-- zurückliefern.
*-- Wenn das virtuelle Objekt nicht bereitgestellt werden kann,
*-- muss ein SDO-Abort Kode entsprechend dem \c RET_T Typ
*-- als Rückgabewert zurückgegeben werden.
*
*
* \retval CO_OK
*++ success
*-- Erfolg
* \retval RET_T
*++ if the referenced \em virtual object is not available 
*-- Wenn das geforderte \em virtuelle Objekt nicht geliefert werden kann
*
*/

RET_T getVirtualObjAddr(
      UNSIGNED16 index,     /**< main-index */
      UNSIGNED8  subIndex,  /**< sub-index */
      UNSIGNED8  **ppData,  /**< destination for data address*/
      UNSIGNED32 *pSize     /**< destination for data size */
      )
{
RET_T   retval = CO_E_NONEXIST_OBJECT;

    printf("getVirtualObjAddr: index %x subIndex %d", index, subIndex);

    /* virtual read object */
    if (index == VIRT_RD_OBJ)  {
    switch (subIndex)  {
        case 0:
        *ppData = (UNSIGNED8 *)&vobj_u8;
        *pSize = 1;
        break;
        default:
        *ppData = (UNSIGNED8 *)&vobj_u16;
        *pSize = 2;
    }
    retval = CO_OK;
    }

    /* virtual write object */
    if (index == VIRT_WR_OBJ)  {
    *pSize = 4;
    switch (subIndex)  {
        case 0:
        *ppData = (UNSIGNED8 *)&vobj_u8;
        *pSize = 1;
        retval = CO_OK;
        break;
        case 1:
        case 2:
        case 3:
        *ppData = (UNSIGNED8 *)&vobj_u32;
        retval = CO_OK;
        break;
        default:
        retval = CO_E_NONEXIST_SUBINDEX;
    }
    }

    printf(" returns %d\n", (UNSIGNED8)retval);
    return(retval);
}
#endif /* CONFIG_VIRTUAL_OBJECTS */
/*______________________________________________________________________EOF_*/

#endif // CANopen
