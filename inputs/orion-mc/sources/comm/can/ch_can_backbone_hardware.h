// CH_CAN_BACKBONE_HARDWARE.H

#ifndef __CH_CAN_BACKBONE_HARDWARE_H
#define __CH_CAN_BACKBONE_HARDWARE_H

#include "ch_define.h"
#include "ch_can.h"
#include "ch_can_backbone.h"

//array size defs
#define FIFO_BUFFER_SIZE 100

// command fields for value's
#define NO_COMMAND		 0x00
#define REGISTER_COMMAND 0x1F

//simbol definitions
#define BROADCAST     0x0000

#define END_OF_CAN_LIST -1

#define FALSE   0
#define TRUE    1
#define PENDING 2     // starts a alarm, value the next 1 second scan

#define RESET            0
#define SET              1

#define PRIORITY_HIGH 0
#define PRIORITY_LOW  1

//defines for the timer
#define CAN_ALARM_TIMEOUT (PULSES_PER_SECOND*1)
#define CAN_VALUE_TIMEOUT (PULSES_PER_SECOND*1)
#define CAN_MASTER_TIMEOUT (PULSES_PER_SECOND*1)
#define CAN_RETRY       3

#define TRANSMIT_MASTER_ERROR 0
#define TRANSMIT_MASTER_OK 1

#define TRANSMIT_VALUE_ERROR 0
#define TRANSMIT_VALUE_OK 1

#define TRANSMIT_ALARM_ERROR 0
#define TRANSMIT_ALARM_OK 1

#define RECEIVE_SLAVE_ERROR 0
#define RECEIVE_SLAVE_OK 1

// use one of this defines as parameter for Can_Backbone_Hardware_Init
// Make no failure to take de wrong define
// if node A is used for lokal CAN then select only node B for CAN backbone
// if node B is used for lokal CAN then select only node A for CAN backbone
#define CAN_BACKBONE_USE_NODE_A_NOT_EXTERN 0
// P4_5 P4_6 not tested
#define CAN_BACKBONE_USE_NODE_A_P4_5_P4_6 1
// P7_6 P7_76 not tested
#define CAN_BACKBONE_USE_NODE_A_P7_6_P7_7 2
// P9_2 P9_3 not tested
#define CAN_BACKBONE_USE_NODE_A_P9_2_P9_3 3

#define CAN_BACKBONE_USE_NODE_B_NOT_EXTERN 4
// P4_4 P4_7 not tested
#define CAN_BACKBONE_USE_NODE_B_P4_4_P4_7 5
#define CAN_BACKBONE_USE_NODE_B_P7_4_P7_5 6
// P9_0 P9_1 not tested
#define CAN_BACKBONE_USE_NODE_B_P9_0_P9_1 7

#define CAN_BACKBONE_NOT_EXTERN CAN_BACKBONE_USE_NODE_A_NOT_EXTERN
#define CAN_BACKBONE_EXTERN CAN_BACKBONE_USE_NODE_B_P7_4_P7_5

// The following data type serves as a software message object. Each access to
// a hardware message object has to be made by forward a pointer to a software
// message object (s_CAN_SWObj). The data type has the following fields:
//
// CFGL:
// this byte has the same structure as the message configuration register of a
// hardware message object. It contains the "Data Lenght Code" (DLC), the 
// "Extended Identifier" (XTD), the "Message Direction" (DIR), the "Node
// Select" and the "Remote Monitoring Mode".
//
//
//         7     6     5      4    3     2     1     0
//      |------------------------------------------------|
//      |        DLC            | DIR | XTD | NODE | RMM |
//      |------------------------------------------------|
//
// ID: 
// this field is four bytes long and contains either the 11-bit identifier 
// or the 29-bit identifier
//
// data[8]:
// 8 bytes containing the data of a frame
//
/*
typedef struct
{
  unsigned int  MCFG;    // Message Configuration Register
  unsigned int  UID;     // upper word (extended (29-bit) identifier)
  unsigned int  LID;     // lower word (extended (29-bit) identifier)
  unsigned char data[8]; // data Byte 0..7
} s_CAN_SWObj;
*/ 
//*******  FIFOs  ******    
typedef struct
{
  s_CAN_SWObj array[FIFO_BUFFER_SIZE];   //holds the sw_msgobj
  unsigned int push_index;
  unsigned int pop_index;
  unsigned char full;
} s_fifo;

typedef struct 
{
  unsigned int    RXOK;
  unsigned int    TXOK;
  unsigned int    BOFF;
  unsigned int    EWRN;
  unsigned int    ERROR_STUFF;
  unsigned int    ERROR_FORM;
  unsigned int    ERROR_ACK;
  unsigned int    ERROR_BIT1;
  unsigned int    ERROR_BIT0_BUSOFF;
  unsigned int    ERROR_BIT0_NORMAL;
  unsigned int    ERROR_CRC;
  unsigned long    ERROR_OTHER;
} s_can_status;

extern unsigned char can_backbone_hardware_busoff_reset_switch;
extern unsigned int can_backbone_txok;
extern unsigned int can_backbone_txok_peak;
extern unsigned int can_backbone_rxok;
extern unsigned int can_backbone_rxok_peak;
extern unsigned int can_backbone_txok_rxok;
extern unsigned int can_backbone_txok_rxok_peak;
extern s_can_status can_backbone_status;
extern unsigned char can_backbone_diag_reset_flag;

void Enable_CAN_Interrupts(void);
void Disable_CAN_Interrupts(void);
void Restore_CAN_Interrupts(void);

void Enable_Can_Backbone_Int(void);
void Disable_Can_Backbone_Int(void);

void Can_Backbone_Hardware_Busoff_Reset(void);
void Can_Backbone_Hardware_Timing_Control(void);
void Can_Backbone_Hardware_Init(unsigned char use_CAN_Bus);

void Can_Backbone_Diagnose_Msg(void);

#endif // __CH_CAN_BACKBONE_HARDWARE_H