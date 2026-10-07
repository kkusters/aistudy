/*
 * TwinCan - definitions for TwinCan like CAN controllers
 *
 *
 *--------------------------------------------------------------------------
 */

#ifndef __TWINCAN_H
#define __TWINCAN_H

// message object configuration register
# define CAN_MSG_Dir        0x08
# define CAN_MSG_Xtd        0x04

// Definitions for Data direction
# define CAN_Dir_TRANSMIT   1
# define CAN_Dir_RECEIVE    0

// message object control register
# define CAN_MSG_VAL_TEST     0x0080
# define CAN_MSG_TXIE_TEST    0x0020
# define CAN_MSG_RXIE_TEST    0x0008
# define CAN_MSG_IntPnd_TEST  0x0002

# define CAN_MSG_VAL_SET      0xFFBF
# define CAN_MSG_VAL_RESET    0xFF7F
# define CAN_MSG_TXIE_SET     0xFFEF
# define CAN_MSG_TXIE_RESET   0xFFDF
# define CAN_MSG_RXIE_SET     0xFFFB
# define CAN_MSG_RXIE_RESET   0xFFF7
# define CAN_MSG_IntPnd_SET   0xFFFE
# define CAN_MSG_IntPnd_RESET 0xFFFD

# define CAN_MSG_RmtPnd_TEST  0x8000
# define CAN_MSG_TxRqst_TEST  0x2000
# define CAN_MSG_MsgLst_TEST  0x0800    /* only receive         */
# define CAN_MSG_CPUUpd_TEST  0x0800    /* only transmit        */
# define CAN_MSG_NewDat_TEST  0x0200
 
# define CAN_MSG_RmtPnd_SET   0xBFFF
# define CAN_MSG_RmtPnd_RESET 0x7FFF
# define CAN_MSG_TxRqst_SET   0xEFFF
# define CAN_MSG_TxRqst_RESET 0xDFFF
# define CAN_MSG_MsgLst_SET   0xFBFF
# define CAN_MSG_MsgLst_RESET 0xF7FF
# define CAN_MSG_CPUUpd_SET   CAN_MSG_MsgLst_SET  
# define CAN_MSG_CPUUpd_RESET CAN_MSG_MsgLst_RESET
# define CAN_MSG_NewDat_SET   0xFEFF
# define CAN_MSG_NewDat_RESET 0xFDFF

// Contents of Interrupt Register
# define CAN_INTERRUPT_IDLE     0x00
# define CAN_STATUS_CHANGED_INT 0x01
# define CAN_MESSAGE15_INT      0x02

// Masks for Status Register
# define CAN_STAT_BUSOFF               0x80
# define CAN_STAT_ERROR_WARNING_STATUS 0x40
# define CAN_STAT_RXOK                 0x10
# define CAN_STAT_TXOK                 0x08
# define CAN_STAT_LASTERRORCODE        0x07 // LEC - MASK

// Allocate certain CAN channels for the different modi.
//----------------------------------------------------------
#ifdef CONFIG_FULLCAN
//----------------------------------------------------------
// Full-CAN mode with only one transmit channel
# if defined(CONFIG_ONLY_ONE_TRANSMIT_CHANNEL)
  // transmit channel number - first channel
# define CAN_TRANSMIT_OBJ   0
  // nodeguarding channel number - second channel
# define CAN_NODEGUARD_OBJ  (CAN_TRANSMIT_OBJ + 1)
# endif // CONFIG_ONLY_ONE_TRANSMIT_CHANNEL
  // last usable channel
# define CAN_LAST_OBJ       13
  // special channel - last object
# define CAN_GROUP_OBJ      14
//----------------------------------------------------------
#else // CONFIG_FULLCAN
//----------------------------------------------------------
// Basic-CAN mode
  // transmit channel number
# define CAN_TRANSMIT_OBJ    0
# define CAN_NODEGUARD_OBJ   (CAN_TRANSMIT_OBJ + 1)
  // receive channel number for all messages
# define CAN_ALL_RECEIVE_OBJ 14
//----------------------------------------------------------
# endif  // CONFIG_FULLCAN
//----------------------------------------------------------

UNSIGNED8 Init_CAN(UNSIGNED8 use_CAN_bus, UNSIGNED16 wBaudRate);

#endif //__TWINCAN_H
