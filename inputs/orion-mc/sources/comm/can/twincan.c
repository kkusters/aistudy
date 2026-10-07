//-------------------------------------------------------------------
// TwinCan - general CANopen driver for TwinCan compatible controller
//
// Programmer: Twan Driessen
//
//-------------------------------------------------------------------

/**
*  \file TwinCan.c
*
*++ This module contains
*++ all functions needed for a CANopen CAN driver
*++ which are specific to the TwinCan architecture.
*
*++ In this version for using with CANopen, it uses only
*++ standard identifier format.
*
*++ In BASIC CAN mode (\c CONFIG_FULLCAN is not defined)
*++ the driver uses only two message objects
*++ The 'Last Message Object' to receive all messages
*++ and a pre-defined message object ,\c CAN_TRANSMIT_OBJ, to transmit messages.
*++ If Nodeguarding is enabled also the third message object 
*++ \c CAN_NODEGUARD_OBJ is used.
*
*++ If \c CONFIG_FULLCAN is defined, all message objects of the CAN controller
*++ are used.
*++ Message objects are allocated in the order they are requested.
*++ If in this mode also \c CONFIG_ONLY_ONE_TRANSMIT_CHANNEL
*++ is defined, then two predifined transmit channels are used.
*++ One for common transmit requests and one for the Node Guarding channel.
*++ All other channels from 2...13 are available for receiving
*++ and are allocated using
*++ \em defineCOB()
*++ requests for receive objects.
*
*++ In the FullCAN mode it's usable to group messages on one channel.
*++ \c CONFIG_GROUP_CHANNEL had to be set.
*/

/**
* \def CONFIG_DONT_USE_ISR
*++ use your own interrupt function
*/

#include <string.h>

#include <cal_conf.h>

# include <co_type.h>
# include <co_def.h>
# include <co_drv.h>
# include <co_drvif.h>
# include <co_flag.h>

# include <cdriver.h>
#include "ch_can_backbone_hardware.h"
#include "twincan.h"

# if defined(CONFIG_GROUP_CHANNEL) && !defined(CONFIG_FULLCAN)
#  error "CONFIG_GROUP_CHANNEL only with CONFIG_FULLCAN usefull!"
# endif

# if defined(CONFIG_NODE_GUARDING) && defined(CONFIG_REDUNDANCY_SUPPORT)
#  error "Nodeguarding doesn't support with Redundancy support!"
# endif /* defined(CONFIG_NODE_GUARDING) && defined(CONFIG_REDUNDANCY_SUPPORT)*/

// external variables
//---------------------------------------------------------------------------
# ifdef CONFIG_REDUNDANCY_SUPPORT
extern VOLATILE UNSIGNED32  reduncySendTime; // last send time
# endif // CONFIG_REDUNDANCY_SUPPORT

// local defined variables
//---------------------------------------------------------------------------
# ifdef CONFIG_MULT_LINES
// transmission in action
static VOLATILE BOOL_T eSending   CO_LINE_PARA_ARRAY_DEF;
static VOLATILE BOOL_T eReceiving CO_LINE_PARA_ARRAY_DEF;
# else // CONFIG_MULT_LINES
// transmission in action
static bit eSending;
static bit eReceiving;
# endif // CONFIG_MULT_LINES

# ifdef CONFIG_GROUP_CHANNEL
// Vars for mask building
static UNSIGNED16 u16OldId   CO_LINE_PARA_ARRAY_DEF;
static UNSIGNED16 u16OldMask CO_LINE_PARA_ARRAY_DEF;
# endif // CONFIG_GROUP_CHANNEL

/*
 * The following two variables are used to determine the Channel
 * or COB-ID usage within ISR. 
 * They are set in Define_COB() and Set_COB_ID().
 */
# ifdef CONFIG_SYNC_CONSUMER
static UNSIGNED8 CO_DATA coSyncChannel CO_LINE_PARA_ARRAY_DEF;
# endif // CONFIG_SYNC_CONSUMER
static UNSIGNED8 CO_DATA coNmtErrChannel CO_LINE_PARA_ARRAY_DEF;

// CAN controller channel count
static UNSIGNED8 coSavedChannel CO_LINE_PARA_ARRAY_DEF;

# ifdef CONFIG_COB_ARRAY
#  ifdef CONFIG_COB_NUMBERS
#  else /* CONFIG_COB_NUMBERS */
#    error "Number of CAN Objects is not set(CONFIG_COB_NUMBERS)!"
#  endif /* CONFIG_COB_NUMBERS */
COB_T cob_list[CONFIG_COB_NUMBERS];
UNSIGNED8 cob_index_list[CONFIG_COB_NUMBERS];
UNSIGNED8 cob_list_next_entry;
# endif /* CONFIG_COB_ARRAY */

void CAN_int_RX14( CO_LINE_PARA_DECL );
void GetNext_TX_Request(CO_LINE_PARA_DECL ) RTX51_MODIFIER;

//-------------------------------------------------------------------
// \brief Init_CAN - initialize the CAN-Controller
//
// This function resets the global message buffer.
// It sets the bit rate for the CAN controller.
// The CAN controller is not started by this function.
//
// With \c CONFIG_FULLCAN not set,
// the object 14 is preconfigured to receive all CAN messages
// like a Basic CAN controller
// and the object 0 is preconfigured as a transmit object.
// In this mode for Nodeguarding the channel 1 is used.
//
// \retval CO_INIT_CAN_OK
// success
// \retval CO_E_INIT_HARD_RES_ACTIVE
// reset is aktive, init failed
// \retval CO_E_INIT_BAUD
// could'nt adjust baudrate, init failed

UNSIGNED8 Init_CAN(UNSIGNED8  canLine,   // nr of CAN-controller
                   UNSIGNED16 wBaudRate) // bitrate (e.g. 50 = 50kBaud)
{
  Disable_CAN_Interrupts();

  eSending   CO_LINE_PARA_ARRAY_INDEX =
  eReceiving CO_LINE_PARA_ARRAY_INDEX = CO_FALSE;

  // reset TX/RX buffer
  clearRxBuffer(CO_LINE_PARA);
  clearTxBuffer(CO_LINE_PARA);

  ///  - set INIT and CCE
  CAN_HWNODE[canLine].CR = 0x0041; // load global control register

  ///  -----------------------------------------------------------------------
  ///  Configuration of CAN Node A or B:
  ///  -----------------------------------------------------------------------

  ///  - transmit / receive OK interrupt node pointer: TwinCAN SRN 0
  ///  - error (BusOff and ErrorWarning) interrupt node pointer: TwinCAN SRN 0
  ///  - last error code interrupt node pointer: TwinCAN SRN 0
  CAN_HWNODE[canLine].GINP = 0x0000; // load global interrupt node pointer register

  ///  Configuration of the Node A or B Error Counter:
  ///  - the error warning threshold value (warning level) is 96
  CAN_HWNODE[canLine].ECNTH = 0x0060; // load error counter register high

  ///  Configuration of the Node A or B Baud Rate:
  ///  - required baud rate = 125.000 kbaud
  ///  - real baud rate     = 125.000 kbaud
  ///  - sample point       = 87.50 %
  ///  - there are 5 time quanta before sample point
  ///  - there are 4 time quanta after sample point
  ///  - the (re)synchronization jump width is 2 time quanta
//  CAN_HWNODE[canLine].BTRL = 0x1C13; // load bit timing register low
  switch (wBaudRate)
  {
    case  20: CAN_HWNODE[canLine].BTRL = 0x9618; break; //  20kb / 80.00%
	case  50: CAN_HWNODE[canLine].BTRL = 0x1C31; break; //  50kb / 87.50%
	case 125: CAN_HWNODE[canLine].BTRL = 0x1C13; break; // 125kb / 87.50%
	case 250: CAN_HWNODE[canLine].BTRL = 0x1C09; break; // 250kb / 87.50%
	case 500: CAN_HWNODE[canLine].BTRL = 0x1C04; break; // 500kb / 87.50%
	default:  CAN_HWNODE[canLine].BTRL = 0x1C31; break; //  50kb / 87.50%
  }
//  CAN_HWNODE[canLine].BTRL = 0x1C31; // 50kb
//  CAN_HWNODE[canLine].BTRL = 0xB458; // 20kb
  CAN_HWNODE[canLine].BTRH = 0x0000; // load bit timing register high
  CAN_HWNODE[canLine].FCRL = 0x0000; // load frame counter timing register low
  CAN_HWNODE[canLine].FCRH = 0x0000; // load frame counter timing register high
  //interrupt masks node A or B
  CAN_HWNODE[canLine].IMR4 = 0x0007;
  CAN_HWNODE[canLine].IMRL0 = 0xFFFF;
  CAN_HWNODE[canLine].IMRH0 = 0x0000;
  
  // predefined objects in some cases
  /*-----------------------------------------------------------------*/
  /* FullCAN                                                         */
  /*-----------------------------------------------------------------*/     

  /* calculate first free channel without predefined functionality */
  coSavedChannel CO_LINE_PARA_ARRAY_INDEX = 0;
    

  ///  -----------------------------------------------------------------------
  ///  Configuration of the CAN Message Objects 0 - 31:
  ///  -----------------------------------------------------------------------

  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 0: (TRANSMIT MESSAGES)
  ///  -----------------------------------------------------------------------
  ///  - message object 0 is valid
  ///  - enable receive interrupt; bit INTPND is set after successfull 
  ///    reception of a frame

  ///  - message object is used as transmit object
  ///  - 11-bit identifier
  ///  - 8 valid data bytes
  ///  - this message object works with CAN node A
  ///  - remote monitoring is disabled
  ///  - transmit interrupt node pointer: TwinCAN SRN 0

// Full-CAN mode with only one transmit channel
#if !defined (CONFIG_FULLCAN) || defined(CONFIG_ONLY_ONE_TRANSMIT_CHANNEL)
  {
    UNSIGNED16 loop;

    CAN_HWOBJ[CAN_TRANSMIT_OBJ].CFGL = (canLine == 0) ? 0x0008 : 0x000A; // load message configuration register low
    CAN_HWOBJ[CAN_TRANSMIT_OBJ].CFGH = 0x0000; // load message configuration register high

    ///  - acceptance mask 29-bit: 0x0000
    ///  - identifier 29-bit:      0x0000
    CAN_HWOBJ[CAN_TRANSMIT_OBJ].AMR = 0xEC000000L; // load acceptance mask register
    CAN_HWOBJ[CAN_TRANSMIT_OBJ].AR = 0x08000000; // load arbitration register

    for (loop = 0; loop < 8; loop++) // load data with 0
      CAN_HWOBJ[CAN_TRANSMIT_OBJ].data[loop] = 0;

    ///  - functionality of standard message object
    CAN_HWOBJ[CAN_TRANSMIT_OBJ].FCRL = 0x0000; // load FIFO/gateway control register low
    CAN_HWOBJ[CAN_TRANSMIT_OBJ].FCRH = 0x0000; // load FIFO/gateway control register high
 
    CAN_HWOBJ[CAN_TRANSMIT_OBJ].CTRL = 0x55A5; // load message control register low
    CAN_HWOBJ[CAN_TRANSMIT_OBJ].CTRH = 0x0000; // load message control register high

    coSavedChannel CO_LINE_PARA_ARRAY_INDEX ++;
  }
#endif // !defined (CONFIG_FULLCAN) || defined(CONFIG_ONLY_ONE_TRANSMIT_CHANNEL)

  // special handling for some services
#  ifdef CONFIG_SYNC_CONSUMER
  coSyncChannel CO_LINE_PARA_ARRAY_INDEX = 255;
#  endif // CONFIG_SYNC_CONSUMER
  coNmtErrChannel CO_LINE_PARA_ARRAY_INDEX = 255;

  ///  -----------------------------------------------------------------------
  ///  Configuration of Service Request Nodes 0 - 7:
  ///  -----------------------------------------------------------------------
  ///  SRN0 service request node configuration:
  ///  - SRN0 interrupt priority level (ILVL) = 6
  ///  - SRN0 interrupt group level (GLVL) = 0
  ///  - SRN0 group priority extension (GPX) = 0
  CAN_0IC = CAN_BACKBONE_EVENT_LEVEL | ENABLE_INT;

  // set IO pins for can backbone
  switch (canLine)
  {
    case 0:
      CAN_PISEL = (CAN_PISEL & 0xFFF8) | 0x0002;
      P7_6 = 1;
      DP7_6 = 0;
      AS0P7_7 = 1;
      P7_7 = 1;
      DP7_7 = 1;
      ODP7_7 = 1;
      break;
    case 1:
      CAN_PISEL = (CAN_PISEL & 0xFFC7) | 0x0010;
      P7_4 = 1;
      DP7_4 = 0;
      AS0P7_5 = 1;
      P7_5 = 1;
      DP7_5 = 1;
      ODP7_5 = 1;
      break;
  }

  ///  - enable interrupt generation when a message transfer is completed
  ///  - enable interrupt generation on a change of bit BOFF or EWARN
  ///  - enable interrupt generation on setting an error code in bit field LEC
  CAN_HWNODE[canLine].CR = 0x001C; // load global control register

  Restore_CAN_Interrupts();

  return(CO_INIT_CAN_OK);
}

//-------------------------------------------------------------------
// \brief Start_CAN - starts the CAN-Controller
// Starts the CAN-Controller by resetting the init bit.
//
// \returns
// nothing
void Start_CAN(
# ifdef CONFIG_MULT_LINES
     UNSIGNED8 canLine // number of CAN line 0..CONFIG_MULT_LINES-1
# else // CONFIG_MULT_LINES
    CO_LINE_PARA_DECL
# endif // CONFIG_MULT_LINES
     )
{
  eSending   CO_LINE_PARA_ARRAY_INDEX =
  eReceiving CO_LINE_PARA_ARRAY_INDEX = CO_FALSE;
  
# ifdef CONFIG_MULT_LINES
  CAN_HWNODE[canLine].CR = 0x001C; // load global control register
# else // CONFIG_MULT_LINES
  CAN_HWNODE[1].CR = 0x001C; // load global control register
# endif // CONFIG_MULT_LINES

  Enable_CAN_Interrupts();
}


//------------------------------------------------------------------
// \brief Stop_CAN - stops the CAN-Controller
// Stops the CAN controller by setting the init bit.
// All error at the CAN controller are reset.
//
// \returns
// nothing
void Stop_CAN(
# ifdef CONFIG_MULT_LINES
     UNSIGNED8 canLine // number of CAN line 0..CONFIG_MULT_LINES-1
# else // CONFIG_MULT_LINES
    CO_LINE_PARA_DECL
# endif // CONFIG_MULT_LINES
     )
{
# ifdef CONFIG_MULT_LINES
  CAN_HWNODE[canLine].CR |= 0x0001; // load global control register
# else // CONFIG_MULT_LINES
  CAN_HWNODE[1].CR |= 0x0001; // load global control register
# endif // CONFIG_MULT_LINES
}

//-------------------------------------------------------------------
// \brief Clear_busoff_82527 - starts the CAN-Controller after bus-off
// In case of a Bus-OFF the controller is in init-reset mode.
// We only have to clear the init bit to go Bus-ON again.
// If this call is used more times successively,
// a waiting time between the calls should be used.
//
// \returns
// nothing
void Clear_busoff(
# ifdef CONFIG_MULT_LINES
     UNSIGNED8 canLine /**< number of CAN line 0..CONFIG_MULT_LINES-1 */
# else /* CONFIG_MULT_LINES */
    CO_LINE_PARA_DECL
# endif /* CONFIG_MULT_LINES */
     )
{
# ifdef CONFIG_MULT_LINES
  CAN_HWNODE[canLine].CR = 0x001C; // load global control register
# else // CONFIG_MULT_LINES
  CAN_HWNODE[1].CR = 0x001C; // load global control register
# endif // CONFIG_MULT_LINES
}

//-------------------------------------------------------------------
// \brief Define_COB - creates a COB in the COB-list with attributes
//
// Creates a COB in the COB-list with attributes given as parameter.
// If it is the first call the list is created otherwise
// the COB entry is appended to the list.
// In both cases memory is allocated with a call to
// \em CalMalloc (3) .
//
// With Full-CAN controllers also object channels in the controllers hardware
// are occupied.
// The Channel is configured according to the type of the COB
// as transmit or receive object.
// The COB ID would be assigned later, with a call to Set_COB_ID().
//
// If \c CONFIG_ONLY_ONE_TRANSMIT_CHANNEL
// is set, only one channel is used for all transmit objects
// plus one additional transmit channel for Node-Guarding
// (for RTR requests to node-guarding slaves).
// If this is enabled, transmit channels can !!! not !!! handle RTR Requests.
//
// \returns 
// pointer to COB
//
// \retval  not NULL
// success
//
// \retval NULL
// definition failed, e.g. no more channels within Full-CAN controller
// or no memory.
COB_T *Define_COB(
      COB_KIND_T eType,   // COB type
      UNSIGNED8 bLength   // COB length
# ifdef CONFIG_MULT_LINES
      ,UNSIGNED8 canLine  // number of CAN line 0..CONFIG_MULT_LINES-1
# endif // CONFIG_MULT_LINES
      )
{
COB_T *pCOB_InUse,
      *pOld_COB_InUse;
BOOL_T    fReturn; // return, if CO_TRUE
UNSIGNED8 bChannel;
bChannel = coSavedChannel CO_LINE_PARA_ARRAY_INDEX;

//---------------------------------------------------------------------
//  check for free channel and suitable COB type
//---------------------------------------------------------------------
  if (bChannel == (CAN_LAST_OBJ + 1))
  {
    // No more channels available
    return (NULL);
  }

  if ( 
#  ifdef CONFIG_NODE_GUARDING
        // NG use a own channel
        (eType != CO_COB_GUARD_SLAVE) &&
#  endif // CONFIG_NODE_GUARDING
    // no handling for RTR's in BasicCAN mode
        ((eType & (CO_COB_DIR_MASK | CO_COB_RTR)) == CO_COB_TX_RTR) ) 
  {
    return( NULL );
  }

//---------------------------------------------------------------------
//  check end
//---------------------------------------------------------------------

//---------------------------------------------------------------------
//  create a new COB
//---------------------------------------------------------------------
# ifdef CONFIG_COB_ARRAY
  if ( co_pFirst_COB_Entry CO_LINE_PARA_ARRAY_INDEX == NULL )
  {
    co_pFirst_COB_Entry CO_LINE_PARA_ARRAY_INDEX = &cob_list[0];
    cob_list_next_entry = 0;
  }
    
  if( cob_list_next_entry == CONFIG_COB_NUMBERS )
  {
    return(NULL);
  }
    
  pCOB_InUse = &cob_list[cob_list_next_entry];
    
# else // CONFIG_COB_ARRAY

  pCOB_InUse = CalMalloc(sizeof(COB_T));
  if (pCOB_InUse == NULL)
  {
    return(NULL); // not enough memory
  }

  pOld_COB_InUse = co_pFirst_COB_Entry CO_LINE_PARA_ARRAY_INDEX;
  if (pOld_COB_InUse == NULL)
  {
    co_pFirst_COB_Entry CO_LINE_PARA_ARRAY_INDEX = pCOB_InUse;
  }
  else
  {
    while (pOld_COB_InUse->pNext != NULL)
    {
      pOld_COB_InUse = pOld_COB_InUse->pNext;
    }
    pOld_COB_InUse->pNext = pCOB_InUse;
  }
# endif // CONFIG_COB_ARRAY
//---------------------------------------------------------------------
//  create end
//---------------------------------------------------------------------

  pCOB_InUse->pNext    = NULL;
  pCOB_InUse->bLength  = bLength;
  pCOB_InUse->eType    = eType;
  pCOB_InUse->wID      = 0xffff; // disable ID
# ifdef CONFIG_MULT_LINES
  pCOB_InUse->canLine  = canLine;
# endif // CONFIG_MULT_LINES

  fReturn = CO_FALSE;

/*---------------------------------------------------------------------*/
/* special handling */
/*---------------------------------------------------------------------*/
# ifdef CONFIG_ONLY_ONE_TRANSMIT_CHANNEL
  if((eType & CO_COB_DIR_MASK) == CO_COB_TX) 
  {
    bChannel = CAN_TRANSMIT_OBJ;
    fReturn = CO_TRUE;
  }
#  ifdef CONFIG_NODE_GUARDING
  if( eType == CO_COB_GUARD_SLAVE )
  {
    bChannel = CAN_NODEGUARD_OBJ;
    fReturn = CO_TRUE;
  }
#  endif // CONFIG_NODE_GUARDING
# endif // CONFIG_ONLY_ONE_TRANSMIT_CHANNEL

/*---------------------------------------------------------------------*/
/* calculate used channel and write data to the CAN controller */
/*---------------------------------------------------------------------*/
/*---------------------------------------------------------------------*/
# ifdef CONFIG_GROUP_CHANNEL
/*---------------------------------------------------------------------*/
  if( eType == CO_COB_HB_CONS )
  {
    bChannel = CAN_GROUP_OBJ;
    fReturn = CO_TRUE;
  }
/*---------------------------------------------------------------------*/
# endif /* CONFIG_GROUP_CHANNEL */
/*---------------------------------------------------------------------*/

/*---------------------------------------------------------------------*/
/* special handling */
/*---------------------------------------------------------------------*/
/* at the moment compile it every time
 * not possible for TRANSMIT_COB channel objects (1TX mode)
 */
/*---------------------------------------------------------------------*/
  switch( eType )
  {
    case CO_COB_GUARD_SLAVE: 
      coNmtErrChannel CO_LINE_PARA_ARRAY_INDEX = bChannel;
      break;

#  ifdef CONFIG_ONLY_ONE_TRANSMIT_CHANNEL
#  else /* CONFIG_ONLY_ONE_TRANSMIT_CHANNEL */
    case CO_COB_HB_PROD:
      /* == Bootup Channel */
      coNmtErrChannel CO_LINE_PARA_ARRAY_INDEX = bChannel;
      break;
#  endif /* CONFIG_ONLY_ONE_TRANSMIT_CHANNEL */

#  ifdef CONFIG_SYNC_CONSUMER
    case CO_COB_SYNC_CONS:
      coSyncChannel CO_LINE_PARA_ARRAY_INDEX = bChannel;
      break;
#  endif /* CONFIG_SYNC_CONSUMER */
  }
/*---------------------------------------------------------------------*/

  /* save the used transmit channel */    
  pCOB_InUse->bChannel = bChannel;

  if (fReturn == CO_TRUE)
  {
    /* return without change the hardware */
    return(pCOB_InUse);
  }

  Disable_CAN_Interrupts(CO_LINE_PARA);

  /* The Channel must be configured according to the Type of the COB
   * The COB ID would be assigned later, with a call to Set_COB_ID().
   */
  if ((eType & CO_COB_DIR_MASK) == CO_COB_RX)
  {
    CAN_HWOBJ[bChannel].CFGL = 0x0002 | ((UNSIGNED8)(bLength << 4) | (UNSIGNED8)(CAN_Dir_RECEIVE << 3));
    CAN_HWOBJ[bChannel].CTRL = CAN_MSG_RXIE_SET;
    if ((eType & CO_COB_RTR) != 0)
    {
      CAN_HWOBJ[bChannel].CTRL = CAN_MSG_TXIE_SET;
    }
  }
  else
  { // (eType == CO_COB_RX)
#  ifdef CONFIG_ONLY_ONE_TRANSMIT_CHANNEL
    // all transmit objects are assigned to one channel,
    // the CAN_TRANSMIT_OBJ channel. It is already configured in
    // Init_CAN(). CAN_NODEGUARDING_OBJ it's same.
#  else // CONFIG_ONLY_ONE_TRANSMIT_CHANNEL
    CAN_HWOBJ[bChannel].CFGL = 0x0002 | ((UNSIGNED8)(bLength << 4) | (UNSIGNED8)(CAN_Dir_TRANSMIT << 3));
    CAN_HWOBJ[bChannel].CFGH = 0x0000;
    CAN_HWOBJ[bChannel].CTRL = CAN_MSG_TXIE_SET;
    CAN_HWOBJ[bChannel].CTRH = 0x0000;
    CAN_HWOBJ[bChannel].FCRL = 0x0000;
    CAN_HWOBJ[bChannel].FCRH = 0x0000;
#  endif /* CONFIG_ONLY_ONE_TRANSMIT_CHANNEL */
  } // else (eType == RX_COB || eType == RTR_RX_COB)

  CAN_HWOBJ[bChannel].CTRL = CAN_MSG_VAL_RESET & CAN_MSG_IntPnd_RESET && CAN_MSG_RmtPnd_RESET & CAN_MSG_TxRqst_RESET & CAN_MSG_CPUUpd_RESET & CAN_MSG_NewDat_RESET;

  Restore_CAN_Interrupts(CO_LINE_PARA);

  /*
   *increment message object/channel counter at the end,
   * because we want to start with index 0 in CAN_OBJ[]
   */
  bChannel++;
/*---------------------------------------------------------------------*/
/* write to CAN controller end */
/*---------------------------------------------------------------------*/

/*---------------------------------------------------------------------*/
/* save actual state */
/*---------------------------------------------------------------------*/
  /* save channel number of actual line */
  coSavedChannel CO_LINE_PARA_ARRAY_INDEX = bChannel;

# ifdef CONFIG_COB_ARRAY
  cob_list_next_entry++;
# endif /* CONFIG_COB_ARRAY */

  return(pCOB_InUse);
}


//-------------------------------------------------------------------
// \brief Set_COB_ID - assigns an identifier to a specific COB
// Change the COB-ID in software and in the CAN controller channel.
//
// \returns
// nothing
void Set_COB_ID(COB_T      *pCOB, // pointer to COB in list
                UNSIGNED16 wID)   // identifier
{
# ifdef CONFIG_MULT_LINES
UNSIGNED8   canLine;
# endif // CONFIG_MULT_LINES
UNSIGNED8 bChannel; // COB-IDs channel number

  if (pCOB == NULL)
  {
    return;
  }

#  ifdef CONFIG_MULT_LINES
  canLine = pCOB->canLine;
#  endif // CONFIG_MULT_LINES

  pCOB->wID = wID;
    
  Disable_CAN_Interrupts(CO_LINE_PARA);

  bChannel = pCOB->bChannel;
    
/*---------------------------------------------------------------------*/
/* write new COB-ID to the CAN hardware */
/*---------------------------------------------------------------------*/
#  ifdef CONFIG_ONLY_ONE_TRANSMIT_CHANNEL
  // write new Message ID
  if (bChannel != CAN_TRANSMIT_OBJ)
  { 
#  endif // CONFIG_ONLY_ONE_TRANSMIT_CHANNEL

    // Set Channel to the new Message ID
        
    // Reset Message Valid, 1 byte
    CAN_HWOBJ[bChannel].CTRL = CAN_MSG_VAL_RESET;

#  ifdef CONFIG_GROUP_CHANNEL
    /* The concretely COB-Id must be one of Identifier for this
     * channel. The Mask allowed all other defined Identifier
     * for this channel. For building the mask the first Identifier
     * are used and all unequal bits with the new Identifier
     * are masked.
     * Mask: 1..Bit must be equal with ID
     */
    if( bChannel == CAN_GROUP_OBJ )
    {
      if( u16OldId CO_LINE_PARA_ARRAY_INDEX == 0xFFFF )
      {
        // first ID for this channel
        u16OldId CO_LINE_PARA_ARRAY_INDEX = wID;
        // set Message ID
        CAN_WRITE_OID(bChannel, wID);
      }
      else
      {
        u16OldMask CO_LINE_PARA_ARRAY_INDEX &= ~(u16OldId CO_LINE_PARA_ARRAY_INDEX ^ wID);
        // set new Mask
        CAN_WRITEW(CAN_Msg15MaskLowReg_W, u16OldMask CO_LINE_PARA_ARRAY_INDEX << 5 );
      }
    }
    else
    {
#  endif // CONFIG_GROUP_CHANNEL

      // set Message ID
      CAN_HWOBJ[bChannel].AR = (long)wID << 18;

# ifdef CONFIG_GROUP_CHANNEL
    }
# endif // CONFIG_GROUP_CHANNEL
        
    // set Message Valid, 1 byte
    CAN_HWOBJ[bChannel].CTRL = CAN_MSG_VAL_SET;
        
#  ifdef CONFIG_ONLY_ONE_TRANSMIT_CHANNEL
  }
#  endif /* CONFIG_ONLY_ONE_TRANSMIT_CHANNEL */

  Restore_CAN_Interrupts(CO_LINE_PARA);

# ifdef CONFIG_COB_ARRAY
  // resort the index array
  {
    CO_DATA UNSIGNED8 x,i;
    UNSIGNED16 tmpId;
    
    cob_index_list[0] = 0;
    for(i = 1; i < cob_list_next_entry; i++ )
    {
      x = 0;
      tmpId = cob_list[i].wID;
        
      while( (x < i) && (cob_list[*(cob_index_list + x)].wID < tmpId))
      {
        x++;
      }
      if( x < i )
      {
        memmove( cob_index_list + x + 1, cob_index_list + x, i-x);
      } 
      *(cob_index_list + x) = i;
    } /* for */
  }
# endif /* CONFIG_COB_ARRAY */
}


//-------------------------------------------------------------------
// \brief Update_COB - updates data of transmit channel on the CAN controller
//
// Updates the data contents of a COB in a full CAN controller
// with the data
// \em pMsg
// points to.
// This function is only useful on a Full CAN-Controller.
//
// \returns
// nothing
void Update_COB(COB_T      *pCOB,   // pointer to COB in list
                UNSIGNED8  *pMsg)   // pointer to data
{
UNSIGNED8 bChannel; // current Channel number from 0...14
UNSIGNED8 i;
# ifdef CONFIG_MULT_LINES
UNSIGNED8 canLine;
# endif // CONFIG_MULT_LINES

  if (pCOB == NULL)
  {
    return;
  }

  bChannel = pCOB->bChannel;

# ifdef CONFIG_GROUP_CHANNEL
  if( bChannel == CAN_GROUP_OBJ )
  {
    // no update for group channel, because this channel is a receive only channel
    return;
  }
# endif // CONFIG_GROUP_CHANNEL

# ifdef CONFIG_ONLY_ONE_TRANSMIT_CHANNEL
  if(bChannel == CAN_TRANSMIT_OBJ)
  {
    // no update for transmit only channel no RTR support for this channel
    return;
  }
# endif // CONFIG_ONLY_ONE_TRANSMIT_CHANNEL

# ifdef CONFIG_MULT_LINES
  canLine = pCOB->canLine;
# endif // CONFIG_MULT_LINES

  Disable_CAN_Interrupts(CO_LINE_PARA);

  // set Bit CPUUPD
  CAN_HWOBJ[bChannel].CTRL = CAN_MSG_CPUUpd_SET;

  for(i = 0; i < pCOB->bLength; i++)
  {
    CAN_HWOBJ[bChannel].data[i] = *(pMsg++);
  }

  // Update data length code
  CAN_HWOBJ[bChannel].CFGL &= 0x000F;
  CAN_HWOBJ[bChannel].CFGL |= (UNSIGNED8)(pCOB->bLength << 4);
             
  // Reset CPU Update Bit
  CAN_HWOBJ[bChannel].CTRL = CAN_MSG_CPUUpd_RESET;
    
  Restore_CAN_Interrupts(CO_LINE_PARA);
}

void CkeckStatusCanBus(UNSIGNED8 bStat)
{
  if (bStat & CAN_STAT_BUSOFF)
  {
    can_backbone_status.BOFF++;
    can_backbone_status.EWRN = 0;
  }
  if (bStat & CAN_STAT_ERROR_WARNING_STATUS)
  {
    can_backbone_status.EWRN++;
  }
  if (bStat & CAN_STAT_RXOK)
  {
    can_backbone_status.RXOK++;
  }
  if (bStat & CAN_STAT_TXOK)
  {
    can_backbone_status.TXOK++;
  }
  if (bStat & CAN_STAT_LASTERRORCODE)
  {
    switch (bStat & 0x0007)  // LECA (Last Error CodeA)
    {
      case 1: // Stuff Error
        // More than 5 equal bits in a sequence have occurred
        // in a part of a received message where this is not allowed.
        can_backbone_status.ERROR_STUFF++;
        break;
      case 2: // Form Error
        // A fixed format part of a received frame has the wrong format.
        can_backbone_status.ERROR_FORM++;
        break;
      case 3: // Ack Error
        // The message this CAN controller transmitted was not acknowledged 
        // by another node.
        can_backbone_status.ERROR_ACK++;
        break;
      case 4: // Bit1 Error
        // During the transmission of a message (with the
        // exeption of the arbitration field), the device
        // wanted to send a recessive level ("1"), but the
        // monitored bus value was dominant.
        can_backbone_status.ERROR_BIT1++;
        break;
      case 5: // Bit0 Error
        // During the transmission of a message (or acknow-
        // ledge bit, active error flag, or overload flag),
        // the device wanted to send a dominant level ("0"),
        // but the monitored bus value was recessive. During
        // busoff recovery this staus is set each time a
        // sequence of 11 recessive bits has been monitored.
        // This enables the CPU to monitor the proceeding of
        // the busoff recovery sequence (indicating the bus
        // is not stuck at dominant or continously disturbed).
        if (bStat & 0x0080)  // if Busoff status
        {
          // USER CODE BEGIN (SRN0_NODEA,9)
          can_backbone_status.ERROR_BIT0_BUSOFF++;
        }
        else
        {
          // USER CODE BEGIN (SRN0_NODEA,10)
          can_backbone_status.ERROR_BIT0_NORMAL++;
        }
        break;
      case 6: // CRC Error
        // The CRC check sum was incorrect in the message received.
        can_backbone_status.ERROR_CRC++;       
        break;
      default:
        // USER CODE BEGIN (SRN0_NODEA,12)
        break;
    }
  }
}

# if defined(CONFIG_MULT_LINES) && !defined(CONFIG_MULT_CANCONTROL_TYPE)
/*************************************************************************/
/**
*
*++ CAN_int_82527_lineX - CAN interrupt for line number x
*-- CAN_int_82527_lineX - CAN-Interrupt für CAN Linie x
*
*++ This is the CAN-interrupt routine for the multi line version
*++ It calls the function CAN_int with the line parameter x
*-- Das ist die CAN Interruptroutine für die Multiline Version.
*-- Diese Funktion ruft die CAN_int Routine mit dem Linienparameter x
*-- auf.
*
* \returns
*++ nothing
*-- nichts
*
*/

void CAN_int_82527_line0(
    void
     ) CONFIG_CAN_ISR_NUMBER_LINE0 CONFIG_CAN_ISR_REGISTERBANK_LINE0
{
    CAN_int_82527(0);
}

void CAN_int_82527_line1(
    void
     ) CONFIG_CAN_ISR_NUMBER_LINE1 CONFIG_CAN_ISR_REGISTERBANK_LINE1
{
    CAN_int_82527(1);
}
# endif /* CONFIG_MULT_LINES */

# ifdef  CONFIG_DONT_USE_ISR
/* use your own interrupt function */
# else /* CONFIG_DONT_USE_ISR */
/*************************************************************************/
/**
*++ \brief CAN_int_82527 - CAN interrupt service routine
*
*++ The ISR subroutine contains actually many different implementation
*++ variants.
*++ All are selectable with compiler constants.
*
*++ First there is a general decision to use the
*++ 82527 as a Basic CAN or Full CAN controller.
*++ The Full CAN implementation supports all types of CAN frames
*++ but is limited in CANopen to handle only up to 14 message objects.
*++ This is always enough for simple devices.
*
*++ The following can be taken as an example.
*
*++ - Network NMT Message (mandatory)
*++ - one SDO Message pair (mandatory)
*++ - one PDO pair
*++ - one emergency message
*++ - one node guarding message
* 
*++ which results in 
*++ 7 message objects
*
*++ In 8 bit applications the ISR consumes more time then
*++ the repetition rate of more than two consecutive interrupts with no data
*++ and the maximum bit rate of 1 Mbit/s.
*++ This is one of the reasons to put such things like bit shifting for
*++ the message ID, outside of the ISR.
*++ If the \c CONFIG_CPU_FAMILY_8051 is set, this is done in FlushMbox(), 
*++ not in the ISR.
*++ Reading and writing of the message ID object register is therefore
*++ put in a Macro, called \c CAN_READ_OID(obj).
*
* \returns
*++ nothing
*
*++ The following subroutine are used within ISR and from the library:
* \code
* GetNext_TX_Request()          NOAREG
* \endcode
*
*++ To improve execution speed, one can
*++ double the code from
*++ GetNext_TX_Request() into GetNext_TX_Request_ISR()
*++ and specify these functions with 'using x'
*++ Change the function calls in CAN_int() to function 
*++ GetNext_TX_Request() into GetNext_TX_Request_ISR().
*/
#ifdef CANopen
interrupt (CAN_BACKBONE_ADR) using(CAN_BACKBONE_RB) void CANopen_Interrupt(void)
{
UNSIGNED8 CO_DATA bStat;      /* holds the contents of the status register */
UNSIGNED8 CO_DATA bChannel;   /* holds the contents of the interrupt register */
//UNSIGNED8 CO_DATA i;          /* Byte counter in data for-loop */
UNSIGNED8 CO_DATA wakeupFlag; /* not 0 .. wakeup CAN Task */
VOLATILE BUFFER_ENTRY_T * CO_DATA pBuf;

  Disable_CAN_Interrupts(CO_LINE_PARA);
    
  wakeupFlag = 0;

/* --------------------------------------------------------------------------*/
/* --------------- CONFIG_FULLCAN -------------------------------------------*/
/*===========================================================================*/
  bChannel = CAN_HWNODE[1].IR;
  CAN_HWNODE[1].IR = 0;//JP

  // Label for 2nd reading of interrupt
  if(bChannel == 0) goto Leave;   // C505C Error, see 7/7/98

Again:

  // Test Interrupt Source, it could be a Status or Channel Interrupt
  if(bChannel == CAN_STATUS_CHANGED_INT)
  {
/* --------------------------------------------------------------------------*/
/* Status INT ---------------------------------------------------------------*/
/* --------------------------------------------------------------------------*/

    // status register has to be read to clear an pending Status Int
    bStat = CAN_HWNODE[1].SR;
    
    CkeckStatusCanBus(bStat);
# ifdef CONFIG_CAN_ERROR_HANDLING
#  ifdef CONFIG_REDUNDANCY_SUPPORT
    if( reduncyTransmitLine() == canLine )
    {
      // active transmit line
      if (bStat & CAN_STAT_BUSOFF)
      {
        SET_COLIB_FLAG(COFLAG_CAN_BUSOFF);
        wakeupFlag = 1;
      }
      if (bStat & CAN_STAT_ERROR_WARNING_STATUS)
      {
        SET_COLIB_FLAG(COFLAG_CAN_PASSIVE);
        wakeupFlag = 1;
      }
    }
    else
    {
      // inactive line
      if (bStat & CAN_STAT_BUSOFF)
      {
        // auto buson
        CAN_RESET_BIT(CAN_ControlReg, CAN_CNTL_INIT + CAN_CNTL_CONFIG_CHANGE_ENABLE);
      }
    }
#  else // CONFIG_REDUNDANCY_SUPPORT
    if (bStat & CAN_STAT_BUSOFF)
    {
      SET_COLIB_FLAG(COFLAG_CAN_BUSOFF);
      wakeupFlag = 1;
    }
    if (bStat & CAN_STAT_ERROR_WARNING_STATUS)
    {
      SET_COLIB_FLAG(COFLAG_CAN_PASSIVE);
      wakeupFlag = 1;
    }
#  endif // CONFIG_REDUNDANCY_SUPPORT
# endif // CONFIG_CAN_ERROR_HANDLING
CAN_HWNODE[1].SR = 0; // jp
  }
  else
  {

// Message INT -------------------------------------------------------------
# ifdef CONFIG_GROUP_CHANNEL
    if (bChannel == CAN_MESSAGE15_INT)
    {
      bChannel = 14;
      CAN_int_RX14( CO_LINE_PARA );
    }
    else
# endif // CONFIG_GROUP_CHANNEL
    {
      bChannel -= 2;

      bStat = CAN_HWOBJ[bChannel].CFGL;

      if (((bStat & CAN_MSG_Dir) == CAN_Dir_RECEIVE) && ((CAN_HWOBJ[bChannel].CTRL & 0x0300) == CAN_MSG_NewDat_TEST))      
      {

/* --------------------------------------------------------------------------*/
/* Receive INT -------------------------------------------------------------*/
/* --------------------------------------------------------------------------*/

        /*
         * First handle all Messages which must not be sent to
         * the upper layer, but are handled within the ISR:
         * SYNC - is a RX_COB
         *
         */
# ifdef CONFIG_SYNC_CONSUMER
        if (bChannel == coSyncChannel CO_LINE_PARA_ARRAY_INDEX )
        {
          // reenable receive buffer
          CAN_HWOBJ[bChannel].CTRL = CAN_MSG_NewDat_RESET;
          // SYNC message received
          SET_COLIB_FLAG(COFLAG_SYNC_RECEIVED);
          wakeupFlag = 1;
        }
        else 
# endif // CONFIG_SYNC_CONSUMER
        {
          /*
           * Second: all Messages which has to be put
           * in the receive queue.
           */
          CHECK_BUFFER_WRITE( RX , COFLAG_BUFFER_OVERFLOW )
          {
            pBuf = BUFFER_ADDR(RX, Write);
            
            /* receive queue is free 
             * In FULLCAN Mode then Message ID for channel 0..13
             * is constant.
             */
            pBuf->wCOB_ID = CAN_HWOBJ[bChannel].AR >> 18;

            /*
             * Transfer receive data into RX_Buffer
             *
             * while transfering data new messages can arrive.
             * Therefore it is necessary to test this condition.
             * If new data has arrived, then use the new one.
             */

            do
            {
              CAN_HWOBJ[bChannel].CTRL = CAN_MSG_NewDat_RESET;

              bStat = CAN_HWOBJ[bChannel].CFGL;
              // test for RTR
              if (bStat & 8)
              {
                // RTR is set
                pBuf->bLength = CO_RTR_REQ; 
              }
              else
              {
                bStat >>= 4;  // get length from DLC
                if (bStat > 8)
                {
                  bStat = 8;
                }
                pBuf->bLength = bStat;

                /* copy message data from controller to RX buffer  
                 * If memcpy used the volatile attribut removed
                 * at the cast (void*) for memcpy parameters.
                 * Ignore the warning.
                 */
                BUFFER_WRITE_CPY(bStat);
//              for (i = 0; i < bStat; i++)
//                pRX_Buffer[bRX_WriteIndex].pData[i] = CAN_HWOBJ[bChannel].data[i];

              } // if (bStat & 8 )

              // last Channel - check shadow buffer
              if ((CAN_HWOBJ[bChannel].CTRL & CAN_MSG_NewDat_TEST) != 0)
              {
                SET_COLIB_FLAG( COFLAG_CAN_OVERFLOW );
              }
              else
              {
                // quit the while loop
                break;
              }
            } while( CO_TRUE );
                
            BUFFER_ENTRY_INCR(RX, Write, FULL);
          } // check buffer free

          wakeupFlag = 1;

          // reset NewDat also if the buffer full
          CAN_HWOBJ[bChannel].CTRL = CAN_MSG_NewDat_RESET;
        } // SYNC check

# ifdef CONFIG_CAN_ERROR_HANDLING
        // control, if Msg lost
        if ((CAN_HWOBJ[bChannel].CTRL & CAN_MSG_MsgLst_TEST) != 0)
        {
          SET_COLIB_FLAG( COFLAG_CAN_OVERFLOW );
          CAN_HWOBJ[bChannel].CTRL = CAN_MSG_MsgLst_RESET;
          wakeupFlag = 1;
        }
# endif // CONFIG_CAN_ERROR_HANDLING

        // reset INTPND in Message Control Register
        CAN_HWOBJ[bChannel].CTRL = CAN_MSG_IntPnd_RESET;
      }
      else
      {
/* --------------------------------------------------------------------------*/
/* Transmit INT -------------------------------------------------------------*/
/* --------------------------------------------------------------------------*/

        // Reset TX Status Bit
        CAN_HWNODE[1].SR = 0;
        
        // Reset INTPND
        CAN_HWOBJ[bChannel].CTRL = CAN_MSG_IntPnd_RESET;

# if defined(CONFIG_NODE_GUARDING) || defined(CONFIG_HEARTBEAT_PRODUCER)
        if ( bChannel == coNmtErrChannel CO_LINE_PARA_ARRAY_INDEX ) 
        {
          // CAN has answerded of the Nodeguarding RTR Message
            
          CHECK_BUFFER_WRITE( RX , COFLAG_BUFFER_OVERFLOW ) 
          {
            UNSIGNED8 u8Tmp;

            u8Tmp = CAN_HWOBJ[bChannel].data[0];
            
#  ifdef CONFIG_HEARTBEAT_PRODUCER
#   ifdef CONFIG_NODE_GUARDING
            /* if nodeguarding and heartbeat enabled,
             * all messages send to the CANopen Library */
#   else // CONFIG_NODE_GUARDING
            /* Bootup Ack for the Library */
            if( u8Tmp == 0)
#   endif // CONFIG_NODE_GUARDING
#  endif // CONFIG_HEARTBEAT_PRODUCER
            {
              BUFFER_WRITE( RX , pData[0] , u8Tmp);
              BUFFER_WRITE( RX , wCOB_ID, (CAN_HWOBJ[bChannel].AR >> 18));
              // set only the RTR Bit
              BUFFER_WRITE( RX , bLength , CO_RTR_REQ );
                
              BUFFER_ENTRY_INCR( RX , Write , FULL );
            }   
          }
          wakeupFlag = 1;
        }  
        /* Heartbeat send of this channel 
         * For this, we had to call GetNext_TX_Request, too.
         */
# endif // CONFIG_NODE_GUARDING || CONFIG_HEARTBEAT_PRODUCER

        eSending CO_LINE_PARA_ARRAY_INDEX = CO_FALSE;
        // get next transmission request
        GetNext_TX_Request(CO_LINE_PARA); 

      } // Transmit Int
    } // Last Channel check
  } // Message Int


// ------ Code for all Interrupt Sources ------------------------------------
  // Reset RXOK, TXOK and LEC(Last Error Code) in CAN-SR
  CAN_HWNODE[1].SR = 0;

  /*
   * Before leaving the ISR the Interrupt Register has to be checked again
   * to see if there are a new Int is pending
   */
  bChannel = CAN_HWNODE[1].IR;
  CAN_HWNODE[1].IR = 0;//JP
//  if((bChannel = CAN_HWNODE[1].IR) !=  0) // TD
  if(bChannel !=  0)
  {
    goto Again;
  }

// ------------ Both Full CAN and Basic CAN ---------------------------------
Leave:

  if (wakeupFlag != 0 )
  {
    /* CAN-Task wake up after reading all messages 
     * from the CAN Controller
     *
     */
    CO_NEW_RX_MSG(CO_LINE_PARA); // message received
  }

  Enable_CAN_Interrupts(CO_LINE_PARA);    /* enable CAN interrupt */
}
#endif // CANopen

# if defined(CONFIG_GROUP_CHANNEL) || !defined(CONFIG_FULLCAN)
/******************************************************************
* CAN_int_RX14 - receive messages from the last channel
*
* This function is part of the CAN interrupt function.
*
*******************************************************************/
void CAN_int_RX14(
#  ifdef CONFIG_MULT_LINES
    UNSIGNED8 canLine /* number of CAN line 0..CONFIG_MULT_LINES-1 */
#  else /* CONFIG_MULT_LINES */
    CO_LINE_PARA_DECL
#  endif /* CONFIG_MULT_LINES */
    ) 
{
#  define CHANNEL14 14
UNSIGNED8 CO_DATA bStat;
UNSIGNED8 CO_DATA wakeupFlag; /* not 0 .. wakeup CAN Task */
BUFFER_ENTRY_T * CO_DATA pBuf;
UNSIGNED8 CO_DATA bChannel = CHANNEL14; /* for access with macros */
UNSIGNED16 wID; /* COB-ID */
UNSIGNED8  rtrFlag; /* RTR Flag set? */

  wakeupFlag = 0;

  /*
   * Transfer receive data into RX_Buffer
   *
   * while transfering data new messages can arrive.
   * Therefore it is necessary to test this condition.
   * If new data has arrived, then use the new one.
   */
  while ((CAN_HWOBJ[CHANNEL14].CTRL & CAN_MSG_NewDat_TEST) != 0)
  {
    wID = CAN_HWOBJ[CHANNEL14].AR >> 18;

    // test for RTR
    bStat = CAN_HWOBJ[CHANNEL14].CFGL;
    rtrFlag = 0;
    if (bStat & 8)
    {
      // RTR is set
      rtrFlag = CO_RTR_REQ; 
    }

#   ifdef CONFIG_TEST_VALID_COB
    if (validCobId( wID, rtrFlag CO_COMMA_LINE_PARA) == CO_FALSE ) 
    {
      // ignore message
      // go to the next channel
    }
    else
#   endif // CONFIG_TEST_VALID_COB
    {
      // on Channel 14 we could only receive messages
      CHECK_BUFFER_WRITE( RX , COFLAG_BUFFER_OVERFLOW )
      {
        pBuf = (BUFFER_ENTRY_T *)BUFFER_ADDR(RX, Write);

        pBuf->wCOB_ID = CAN_HWOBJ[CHANNEL14].AR >> 18;
        
        bStat = CAN_HWOBJ[CHANNEL14].CFGL;
        // test for RTR
        if (bStat & 8)
        {
          // RTR is set
          pBuf->bLength = CO_RTR_REQ;
        }
        else
        {
          bStat >>= 4;  // get length from DLC
          if (bStat > 8)
          {
            bStat = 8;
          }
          pBuf->bLength = bStat;

          /* copy message data from controller to RX buffer  
           * If memcpy used the volatile attribut removed
           * at the cast (void*) for memcpy parameters.
           * Ignore the warning.
           */
          BUFFER_WRITE_CPY( bStat );
        } // if (bStat & 8 )

        BUFFER_ENTRY_INCR( RX , Write, FULL );
        // now read the shadow buffer
      }
      wakeupFlag = 1;
    }

#   ifdef CONFIG_CAN_ERROR_HANDLING
    // control, if Msg lost
    if ((CAN_HWOBJ[CHANNEL14].CTRL & CAN_MSG_MsgLst_TEST) != 0)
    {
      SET_COLIB_FLAG( COFLAG_CAN_OVERFLOW );
      wakeupFlag = 1;
      CAN_HWOBJ[CHANNEL14].CTRL = CAN_MSG_MsgLst_RESET;
    }
#   endif // CONFIG_CAN_ERROR_HANDLING

    // reset INTPND, RMTPND a. NEWDAT in Msg Ctrl Register
    CAN_HWOBJ[CHANNEL14].CTRL = CAN_MSG_IntPnd_RESET & CAN_MSG_RmtPnd_RESET & CAN_MSG_NewDat_RESET;
        
    // last Channel - check shadow buffer
  }
        
  if (wakeupFlag != 0)
  {
    /* CAN-Task wake up after reading all messages 
     * from the CAN Controller
     *
     */
    CO_NEW_RX_MSG(CO_LINE_PARA); // message received
  }
#  undef CHANNEL14
}
#  endif // defined(CONFIG_GROUP_CHANNEL) || !defined(CONFIG_FULLCAN)
# endif // CONFIG_DONT_USE_ISR

//-------------------------------------------------------------------
// \brief GetNext_TX_Request - transmits message from the transmission queue
//
// \attention
// Do NOT call from user code !
//
// The function is called within the CAN ISR and directly in
// Transmit_COB();
// A Full CAN controller in Basic CAN mode
// uses object CAN_TRANSMIT_OBJ to Transmit a COB.
// In Full CAN mode the object specified in bChannel
// of the COB structure is used.
//
// \returns
// nothing
void GetNext_TX_Request(
# ifdef CONFIG_MULT_LINES
     UNSIGNED8 canLine // number of CAN line 0..CONFIG_MULT_LINES-1
# else // CONFIG_MULT_LINES
     CO_LINE_PARA_DECL
# endif // CONFIG_MULT_LINES
     ) RTX51_MODIFIER
{
UNSIGNED8 bChannel;

  CHECK_BUFFER_READ( TX )
  {
    if (eSending CO_LINE_PARA_ARRAY_INDEX == CO_TRUE)
    {
#  ifdef CONFIG_REDUNDANCY_SUPPORT
      redundancyFlags |= REDCYFLAG_TRANSMIT;
#  endif // CONFIG_REDUNDANCY_SUPPORT
      return;
    }

    bChannel = BUFFER_READ( TX , bChannel );

    // if CAN controller is busy return
    if ((CAN_HWOBJ[bChannel].CTRL & 0x3000) == CAN_MSG_TxRqst_TEST)
    {    
      // CAN controller busy
#  ifdef CONFIG_REDUNDANCY_SUPPORT
      redundancyFlags |= REDCYFLAG_TRANSMIT;
#  endif /* CONFIG_REDUNDANCY_SUPPORT */
      return;
    }

    // Transmit COB available
    // Bit CPUUPD setzen
    CAN_HWOBJ[bChannel].CTRL = CAN_MSG_CPUUpd_SET;

    // set COB ID
    CAN_HWOBJ[bChannel].AR = ((long)BUFFER_READ( TX , wCOB_ID)) << 18;

    // data frame
    if ((BUFFER_READ( TX , eType ) & CO_COB_DIR_MASK) == CO_COB_TX)
    {
      // assuming the object IS a transmit object and valid
      // Set data length and direction in configuration register
      CAN_HWOBJ[bChannel].CFGL = 0x0002 | ((VOLATILE UNSIGNED8)(BUFFER_READ(TX , bLength) << 4) | (UNSIGNED8)(CAN_Dir_TRANSMIT << 3));

      /* writing data from transmit buffer to CAN controller 
       * If memcpy used the volatile attribut removed
       * at the cast (void*) for memcpy parameters.
       * Ignore the warning.
       */
      BUFFER_READ_CPY( BUFFER_READ( TX, bLength ) );
    }
    
    if ( (BUFFER_READ( TX , eType ) & (CO_COB_RTR | CO_COB_DIR_MASK)) == CO_COB_RX_RTR ) 
    {
      // assuming the object is a receive object, now send a RTR
      // Set data length and direction in configuration register
      CAN_HWOBJ[bChannel].CFGL = 0x0002 | ((VOLATILE UNSIGNED8)(BUFFER_READ(TX , bLength) << 4) | (UNSIGNED8)(CAN_Dir_RECEIVE << 3));
    }
    else
    {
      eSending CO_LINE_PARA_ARRAY_INDEX = CO_TRUE;
    }

    /* the next statements should be atomar.
     * that means that incrementing the pointer and setting the flag
     * must be done with sending the object.
     */
    BUFFER_ENTRY_INCR( TX , Read , EMPTY);
        
    // activate message transmission
    // reset CPUUPD and set Bits MSGVAL and TXRQ
    CAN_HWOBJ[bChannel].CTRL = CAN_MSG_VAL_SET & CAN_MSG_TxRqst_SET & CAN_MSG_CPUUpd_RESET;

# ifdef CONFIG_REDUNDANCY_SUPPORT

    redundancyFlags |= REDCYFLAG_TRANSMIT;
    
    reduncySendTime = dTimerValue; // last send time
  }
  else
  {
    // nothing to send
    redundancyFlags &= ~REDCYFLAG_TRANSMIT;
        
# endif // CONFIG_REDUNDANCY_SUPPORT
  }
}

//-------------------------------------------------------------------
// \brief Transmit_COB - transmits a COB
//
// \em Transmit_COB()
// transmits a CAN message with the attribute of \em pCOB
// and the data of \em pMsg.
//
// \returns
// nothing
void Transmit_COB(COB_T     *pCOB, // pointer to COB in list
                  UNSIGNED8 *pMsg) // pointer to data
{
  // insert message to queue
  Insert_TX_Request(pCOB, pMsg);
  // get next transmission request from queue
  Disable_CAN_Interrupts(CO_LINE_PARA);
  GetNext_TX_Request(CO_LINE_PARA);
  Restore_CAN_Interrupts(CO_LINE_PARA);
}

//-------------------------------------------------------------------
// \brief disableTxMessages - stop sending of Messages
//
// \returns
// nothing
void disableTxMessages(
# ifdef CONFIG_MULT_LINES
     UNSIGNED8 canLine // number of CAN line 0..CONFIG_MULT_LINES-1
# else // CONFIG_MULT_LINES
    CO_LINE_PARA_DECL
# endif // CONFIG_MULT_LINES
)
{
UNSIGNED8 bChannel;

  for (bChannel = 0; bChannel < 14; bChannel++)
  {
    // set Bit CPUUPD - disable the tranmission
    CAN_HWOBJ[bChannel].CTRL = CAN_MSG_CPUUpd_SET & CAN_MSG_TxRqst_RESET & CAN_MSG_CPUUpd_RESET;
  }
}

//______________________________________________________________________EOF_
