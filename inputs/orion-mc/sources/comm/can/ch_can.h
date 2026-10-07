// CH_CAN.H

#ifndef _CH_CAN_H
#define _CH_CAN_H

#include "ch_define.h"

#define HW_RECEIVE_MASTER  0
#define HW_TRANSMIT_MASTER 1
#define HW_RECEIVE_SLAVE   2
#define HW_TRANSMIT_SLAVE  3
#define HW_RECEIVE_VALUE   4
#define HW_TRANSMIT_VALUE  5
#define HW_RECEIVE_ALARM   6
#define HW_TRANSMIT_ALARM  7

typedef struct
{
  unsigned char data[8]; // Data 0..7
  unsigned long AR;      // Arbitration Register
  unsigned long AMR;     // Acceptance Mask Register
  unsigned int  CTRL;    // Control Register Low
  unsigned int  CTRH;    // Control Register High (Frame Counter)
  unsigned int  CFGL;    // Configuration Register Low
  unsigned int  CFGH;    // Configuretion Register High
  unsigned int  FCRL;    // FIFO/Gateway Control Register Low
  unsigned int  FCRH;    // FIFO/Gateway Control Register High
  unsigned long dummy;   // Reserved
} s_CanObj;

typedef struct
{
  unsigned int CR;        // 0x00 Control Register
  unsigned int dummy_0;   // 0x02
  unsigned int SR;        // 0x04 Status Register
  unsigned int dummy_1;   // 0x06
  unsigned int IR;        // 0x08 Interrupt Pending Register
  unsigned int dummy_2;   // 0x0A
  unsigned int BTRL;      // 0x0C Bit Timing Register Low
  unsigned int BTRH;      // 0x0E Bit Timing Register High
  unsigned int GINP;      // 0x10 Global Int. Node Pointer Register
  unsigned int dummy_3;   // 0x12
  unsigned int FCRL;      // 0x14 Frame Counter Register Low
  unsigned int FCRH;      // 0x16 Frame Counter Register High
  unsigned int IMRL0;     // 0x18 INTID Mask Register 0 Low
  unsigned int IMRH0;     // 0x1A INTID Mask Register 0 High
  unsigned int IMR4;      // 0x1C INTID Mask Register 4
  unsigned int dummy_4;   // 0x1E
  unsigned int ECNTL;     // 0x20 Error Counter Register Low
  unsigned int ECNTH;     // 0x22 Error Counter Register High
  unsigned int dummy[14]; // 0x24, 0x26, 0x28, 0x2A, 0x2C, 0x2E, 0x30, 0x32, 0x34, 0x36, 0x38, 0x3A, 0x3C, 0x3E
} s_Can_Node_Obj;

typedef struct
{
  unsigned int  CFGL;    // Message Configuration Register
  unsigned long ID;      // standard (11-bit)/extended (29-bit) identifier
  unsigned long mask;    // standard (11-bit)/extended (29-bit) mask
  unsigned char data[8]; // data Byte 0..7
  unsigned int  CTRH;    // control register high (Frame Counter)
} s_CAN_SWObj;

#define CAN_HWNODE ((s_Can_Node_Obj volatile *)0x200200)
// CAN_HWNODE[0] = can node a
// CAN_HWNODE[1] = can node b
#define CAN_HWOBJ ((s_CanObj volatile *)0x200300)
// CAN_HWOBJ[0] = can message 0
// CAN_HWOBJ[1] = can message 1
// CAN_HWOBJ[2] = can message 2
// ......

#define HW_IO_TRANSMIT_MASTER 16
#define HW_IO_RECEIVE_MASTER 17
#define HW_IO_TRANSMIT_VALUE 18
#define HW_IO_RECEIVE_VALUE 19

#endif // CH_CAN_H