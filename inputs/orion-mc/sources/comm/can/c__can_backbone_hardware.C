// C__CAN_BACKBONE_HARDWARE.C

#include <stdio.h>

#include "ch_define.h"
#include "ch_asc1.h"
#include "ch_can.h"
#include "ch_can_backbone_appl.h"
#include "ch_can_backbone_hardware.h"

#define M2S_WRITE_START_MAX_8_BYTES_OF_DATA       0   
#define M2S_WRITE_START_MORE_THEN_8_BYTES_OF_DATA 1   
#define M2S_WRITE_NEXT_BLOCK_OF_DATA              2
#define M2S_WRITE_LAST_BLOCK_OF_DATA              3
#define S2M_WRITE_ASK_FOR_NEXT_BLOCK_OF_DATA      4   
//#define S2M_WRITE_ASK_FOR_PREVIOUS_BLOCK_OF_DATA  5   
#define S2M_WRITE_LAST_BLOCK_CORRECT              6
#define M2S_WRITE_START_MAX_8_BYTES_OF_DATA_FAST       4   
#define M2S_WRITE_START_MORE_THEN_8_BYTES_OF_DATA_FAST 5   
#define M2S_WRITE_NEXT_BLOCK_OF_DATA_FAST              6
#define M2S_WRITE_LAST_BLOCK_OF_DATA_FAST              7
#define S2M_WRITE_ASK_FOR_NEXT_BLOCK_OF_DATA_FAST      0   
//#define S2M_WRITE_ASK_FOR_PREVIOUS_BLOCK_OF_DATA_FAST  1   
#define S2M_WRITE_LAST_BLOCK_CORRECT_FAST              2
    
#define MESSAGE_TYPE_ALARM  0
#define MESSAGE_TYPE_VALUE  1
#define MESSAGE_TYPE_SLAVE  2
#define MESSAGE_TYPE_MASTER 3

//-----------------------------------------------------------
static unsigned char cocnt_canInt;
//-----------------------------------------------------------

//flag that shows it CAN is used
static unsigned char can_backbone_hardware_used; // indicate can backbone  node and port pins   
static unsigned char can_backbone_hardware_used_extern; // say if can backbone is used external
static unsigned char can_backbone_hardware_node; // say if can backbone node (0 = node A; 1 = node B)

static s_can_backbone_transmit_alarm *can_backbone_last_transmit_alarm_ptr;
static s_can_backbone_transmit_value *can_backbone_last_transmit_value_ptr;

//NODE A FIFOs
static s_fifo can_backbone_master_transmit_fifo;
static s_fifo can_backbone_slave_transmit_fifo;

unsigned char can_backbone_hardware_busoff_reset_switch = 0;

s_can_status can_backbone_status;

//for display data in diagnose
unsigned char can_backbone_diag_reset_flag = 0;
unsigned int can_backbone_txok = 0;
unsigned int can_backbone_txok_peak = 0;
unsigned int can_backbone_rxok = 0;
unsigned int can_backbone_rxok_peak = 0;
unsigned int can_backbone_txok_rxok = 0;
unsigned int can_backbone_txok_rxok_peak = 0;


// If two can busses are used look good what to do with CANA_CR and CANB_CR
// this routine is written so that only one can bus is used and the second is free
// (This is by normal orion not possibel then one can is for backbone and one is for IO)
// can local should use interrupt 1 because backbone is using interrupt 0
// CAN Bakcbone always used can interrupt 0 fro communication
void Can_Backbone_Hardware_Init(unsigned char use_CAN_Bus/*, unsigned int P1SEL_val*/)
{
int loop;

  //if the bus is not used, set the flag value and return without initializing anything
  can_backbone_hardware_used = use_CAN_Bus;
  switch (can_backbone_hardware_used)
  {
    default:
    case CAN_BACKBONE_USE_NODE_A_NOT_EXTERN:
      can_backbone_hardware_used_extern = 0;
      can_backbone_hardware_node = 0; 
      CAN_0IC_IE = 0;
      return;
    case CAN_BACKBONE_USE_NODE_A_P4_5_P4_6:  can_backbone_hardware_used_extern = 1; can_backbone_hardware_node = 0; break;
    case CAN_BACKBONE_USE_NODE_A_P7_6_P7_7:  can_backbone_hardware_used_extern = 1; can_backbone_hardware_node = 0; break;
    case CAN_BACKBONE_USE_NODE_A_P9_2_P9_3:  can_backbone_hardware_used_extern = 1; can_backbone_hardware_node = 0; break;
    case CAN_BACKBONE_USE_NODE_B_NOT_EXTERN:
      can_backbone_hardware_used_extern = 0;
      can_backbone_hardware_node = 1;
      CAN_0IC_IE = 0;
      return;
    case CAN_BACKBONE_USE_NODE_B_P4_4_P4_7:  can_backbone_hardware_used_extern = 1; can_backbone_hardware_node = 1; break;
    case CAN_BACKBONE_USE_NODE_B_P7_4_P7_5:  can_backbone_hardware_used_extern = 1; can_backbone_hardware_node = 1; break;
    case CAN_BACKBONE_USE_NODE_B_P9_0_P9_1:  can_backbone_hardware_used_extern = 1; can_backbone_hardware_node = 1; break;
  }

  ///  - set INIT and CCE
  CAN_HWNODE[can_backbone_hardware_node].CR = 0x0041; // load global control register

  ///  -----------------------------------------------------------------------
  ///  Configuration of CAN Node A or B:
  ///  -----------------------------------------------------------------------

  ///  - transmit / receive OK interrupt node pointer: TwinCAN SRN 0
  ///  - error (BusOff and ErrorWarning) interrupt node pointer: TwinCAN SRN 0
  ///  - last error code interrupt node pointer: TwinCAN SRN 0
  CAN_HWNODE[can_backbone_hardware_node].GINP = 0x0000; // load global interrupt node pointer register

  ///  Configuration of the Node A or B Error Counter:
  ///  - the error warning threshold value (warning level) is 96
  CAN_HWNODE[can_backbone_hardware_node].ECNTH = 0x0060; // load error counter register high

  ///  Configuration of the Node A or B Baud Rate:
  ///  - required baud rate = 125.000 kbaud
  ///  - real baud rate     = 125.000 kbaud
  ///  - sample point       = 87.50 %
  ///  - there are 5 time quanta before sample point
  ///  - there are 4 time quanta after sample point
  ///  - the (re)synchronization jump width is 2 time quanta
  CAN_HWNODE[can_backbone_hardware_node].BTRL = 0x1C13; // load bit timing register low
//  CAN_HWNODE[can_backbone_hardware_node].BTRL = 0xB449; // 50kb
//  CAN_HWNODE[can_backbone_hardware_node].BTRL = 0xB458; // 20kb
  CAN_HWNODE[can_backbone_hardware_node].BTRH = 0x0000; // load bit timing register high
  CAN_HWNODE[can_backbone_hardware_node].FCRL = 0x0000; // load frame counter timing register low
  CAN_HWNODE[can_backbone_hardware_node].FCRH = 0x0000; // load frame counter timing register high
  //interrupt masks node A or B
  CAN_HWNODE[can_backbone_hardware_node].IMR4 = 0x0007;
  CAN_HWNODE[can_backbone_hardware_node].IMRL0 = 0x00FF;
  CAN_HWNODE[can_backbone_hardware_node].IMRH0 = 0x0000;
  
  ///  -----------------------------------------------------------------------
  ///  Configuration of the CAN Message Objects 0 - 31:
  ///  -----------------------------------------------------------------------

  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 0: (RECEIVE MASTER)
  ///  -----------------------------------------------------------------------
  ///  - message object 0 is valid
  ///  - enable receive interrupt; bit INTPND is set after successfull 
  ///    reception of a frame

  ///  - message object is used as receive object
  ///  - extended 29-bit identifier
  ///  - 8 valid data bytes
  ///  - this message object works with CAN node A
  ///  - remote monitoring is disabled
  ///  - receive interrupt node pointer: TwinCAN SRN 0
  CAN_HWOBJ[HW_RECEIVE_MASTER].CFGL = (can_backbone_hardware_node == 0) ? 0x0004 : 0x0006; // load message configuration register low
  CAN_HWOBJ[HW_RECEIVE_MASTER].CFGH = 0x0000; // load message configuration register high

  ///  - acceptance mask 29-bit: 0x0000
  ///  - identifier 29-bit:      0x0000
  CAN_HWOBJ[HW_RECEIVE_MASTER].AMR = 0xEC000000L; // load acceptance mask register
  CAN_HWOBJ[HW_RECEIVE_MASTER].AR = 0x08000000; // load arbitration register

  for (loop = 0; loop < 8; loop++) // load data with 0
    CAN_HWOBJ[HW_RECEIVE_MASTER].data[loop] = 0;

  ///  - functionality of standard message object
  CAN_HWOBJ[HW_RECEIVE_MASTER].FCRL = 0x0000; // load FIFO/gateway control register low
  CAN_HWOBJ[HW_RECEIVE_MASTER].FCRH = 0x0000; // load FIFO/gateway control register high
 
  CAN_HWOBJ[HW_RECEIVE_MASTER].CTRL = 0x5599; // load message control register low
  CAN_HWOBJ[HW_RECEIVE_MASTER].CTRH = 0x0000; // load message control register high
  

  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 1: (TRANSMIT MASTER)
  ///  -----------------------------------------------------------------------
  ///  - message object 1 is valid
  ///  - enable transmit interrupt; bit INTPND is set after successfull 
  ///    transmission of a frame

  ///  - message object is used as transmit object
  ///  - extended 29-bit identifier
  ///  - 8 valid data bytes
  ///  - this message object works with CAN node A
  ///  - remote monitoring is disabled
  ///  - transmit interrupt node pointer: TwinCAN SRN 0
  CAN_HWOBJ[HW_TRANSMIT_MASTER].CFGL = (can_backbone_hardware_node == 0) ? 0x000C : 0x000E; // load message configuration register low
  CAN_HWOBJ[HW_TRANSMIT_MASTER].CFGH = 0x0000; // load message configuration register high

  ///  - acceptance mask 29-bit: 0x0000
  ///  - identifier 29-bit:      0x0000
  CAN_HWOBJ[HW_TRANSMIT_MASTER].AMR = 0xFFFFFFFF; // load acceptance mask register
  CAN_HWOBJ[HW_TRANSMIT_MASTER].AR = 0x00000000; // load arbitration register

  for (loop = 0; loop < 8; loop++) // load data with 0
    CAN_HWOBJ[HW_TRANSMIT_MASTER].data[loop] = 0;

  ///  - functionality of standard message object
  CAN_HWOBJ[HW_TRANSMIT_MASTER].FCRL = 0x0000; // load FIFO/gateway control register low
  CAN_HWOBJ[HW_TRANSMIT_MASTER].FCRH = 0x0001; // load FIFO/gateway control register high

  CAN_HWOBJ[HW_TRANSMIT_MASTER].CTRL = 0x55A5; // load message control register low
  CAN_HWOBJ[HW_TRANSMIT_MASTER].CTRH = 0x0000; // load message control register high

  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 2: (RECEIVE SLAVE)
  ///  -----------------------------------------------------------------------
  ///  - message object 2 is valid
  ///  - enable receive interrupt; bit INTPND is set after successfull 
  ///    reception of a frame

  ///  - message object is used as receive object
  ///  - extended 29-bit identifier
  ///  - 0 valid data bytes
  ///  - this message object works with CAN node A
  ///  - remote monitoring is disabled
  ///  - receive interrupt node pointer: TwinCAN SRN 0
  CAN_HWOBJ[HW_RECEIVE_SLAVE].CFGL = (can_backbone_hardware_node == 0) ? 0x0004 : 0x0006; // load message configuration register low
  CAN_HWOBJ[HW_RECEIVE_SLAVE].CFGH = 0x0000; // load message configuration register high

  ///  - acceptance mask 29-bit: 0xFFFF
  ///  - identifier 29-bit:      0x0000
  CAN_HWOBJ[HW_RECEIVE_SLAVE].AMR = 0xEC000000L; // load acceptance mask register
  CAN_HWOBJ[HW_RECEIVE_SLAVE].AR = 0x0C000000; // load arbitration register

  for (loop = 0; loop < 8; loop++) // load data with 0
    CAN_HWOBJ[HW_RECEIVE_SLAVE].data[loop] = 0;

  ///  - functionality of standard message object
  CAN_HWOBJ[HW_RECEIVE_SLAVE].FCRL = 0x0000; // load FIFO/gateway control register low
  CAN_HWOBJ[HW_RECEIVE_SLAVE].FCRH = 0x0002; // load FIFO/gateway control register high

  CAN_HWOBJ[HW_RECEIVE_SLAVE].CTRL = 0x5599; // load message control register low
  CAN_HWOBJ[HW_RECEIVE_SLAVE].CTRH = 0x0000; // load message control register high

  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 3: (TRANSMIT SLAVE)
  ///  -----------------------------------------------------------------------
  ///  - message object 3 is valid
  ///  - enable transmit interrupt; bit INTPND is set after successfull 
  ///    transmission of a frame

  ///  - message object is used as transmit object
  ///  - extended 29-bit identifier
  ///  - 0 valid data bytes
  ///  - this message object works with CAN node A
  ///  - remote monitoring is disabled
  ///  - transmit interrupt node pointer: TwinCAN SRN 0
  CAN_HWOBJ[HW_TRANSMIT_SLAVE].CFGL = (can_backbone_hardware_node == 0) ? 0x000C : 0x000E; // load message configuration register low
  CAN_HWOBJ[HW_TRANSMIT_SLAVE].CFGH = 0x0000; // load message configuration register high

  ///  - acceptance mask 29-bit: 0xFFFF
  ///  - identifier 29-bit:      0x0000
  CAN_HWOBJ[HW_TRANSMIT_SLAVE].AMR = 0xFFFFFFFFL; // load acceptance mask register
  CAN_HWOBJ[HW_TRANSMIT_SLAVE].AR = 0x00000000; // load arbitration register

  for (loop = 0; loop < 8; loop++) // load data with 0
    CAN_HWOBJ[HW_TRANSMIT_SLAVE].data[loop] = 0;

  ///  - functionality of standard message object
  CAN_HWOBJ[HW_TRANSMIT_SLAVE].FCRL = 0x0000; // load FIFO/gateway control register low
  CAN_HWOBJ[HW_TRANSMIT_SLAVE].FCRH = 0x0003; // load FIFO/gateway control register high

  CAN_HWOBJ[HW_TRANSMIT_SLAVE].CTRL = 0x55A5; // load message control register low
  CAN_HWOBJ[HW_TRANSMIT_SLAVE].CTRH = 0x0000; // load message control register high

  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 4: (RECEIVE VALUE)
  ///  -----------------------------------------------------------------------
  ///  - message object 4 is valid
  ///  - enable receive interrupt; bit INTPND is set after successfull 
  ///    reception of a frame

  ///  - message object is used as receive object
  ///  - extended 29-bit identifier
  ///  - 0 valid data bytes
  ///  - this message object works with CAN node A
  ///  - remote monitoring is disabled
  ///  - receive interrupt node pointer: TwinCAN SRN 0
  CAN_HWOBJ[HW_RECEIVE_VALUE].CFGL = (can_backbone_hardware_node == 0) ? 0x0004 : 0x0006; // load message configuration register low
  CAN_HWOBJ[HW_RECEIVE_VALUE].CFGH = 0x0000; // load message configuration register high

  ///  - acceptance mask 29-bit: 0xFFFF
  ///  - identifier 29-bit:      0x0000
  CAN_HWOBJ[HW_RECEIVE_VALUE].AMR = 0xEC000000L; // load acceptance mask register
  CAN_HWOBJ[HW_RECEIVE_VALUE].AR = 0x04000000; // load arbitration register

  for (loop = 0; loop < 8; loop++) // load data with 0
    CAN_HWOBJ[HW_RECEIVE_VALUE].data[loop] = 0;

  ///  - functionality of standard message object
  CAN_HWOBJ[HW_RECEIVE_VALUE].FCRL = 0x0000; // load FIFO/gateway control register low
  CAN_HWOBJ[HW_RECEIVE_VALUE].FCRH = 0x0004; // load FIFO/gateway control register high

  CAN_HWOBJ[HW_RECEIVE_VALUE].CTRL = 0x5599; // load message control register low
  CAN_HWOBJ[HW_RECEIVE_VALUE].CTRH = 0x0000; // load message control register high

  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 5: (TRANSMIT VALUE)
  ///  -----------------------------------------------------------------------
  ///  - message object 5 is valid
  ///  - enable transmit interrupt; bit INTPND is set after successfull 
  ///    transmission of a frame

  ///  - message object is used as transmit object
  ///  - extended 29-bit identifier
  ///  - 0 valid data bytes
  ///  - this message object works with CAN node A
  ///  - remote monitoring is disabled
  ///  - transmit interrupt node pointer: TwinCAN SRN 0
  CAN_HWOBJ[HW_TRANSMIT_VALUE].CFGL = (can_backbone_hardware_node == 0) ? 0x000C : 0x000E; // load message configuration register low
  CAN_HWOBJ[HW_TRANSMIT_VALUE].CFGH = 0x0000; // load message configuration register high

  ///  - acceptance mask 29-bit: 0xFFFF
  ///  - identifier 29-bit:      0x0000
  CAN_HWOBJ[HW_TRANSMIT_VALUE].AMR = 0xFFFFFFFFL; // load acceptance mask register
  CAN_HWOBJ[HW_TRANSMIT_VALUE].AR = 0x00000000; // load arbitration register

  for (loop = 0; loop < 8; loop++) // load data with 0
    CAN_HWOBJ[HW_TRANSMIT_VALUE].data[loop] = 0;

  ///  - functionality of standard message object
  CAN_HWOBJ[HW_TRANSMIT_VALUE].FCRL = 0x0000; // load FIFO/gateway control register low
  CAN_HWOBJ[HW_TRANSMIT_VALUE].FCRH = 0x0005; // load FIFO/gateway control register high

  CAN_HWOBJ[HW_TRANSMIT_VALUE].CTRL = 0x55A5; // load message control register low
  CAN_HWOBJ[HW_TRANSMIT_VALUE].CTRH = 0x0000; // load message control register high

  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 6: (RECEIVE ALARM)
  ///  -----------------------------------------------------------------------
  ///  - message object 6 is valid
  ///  - enable receive interrupt; bit INTPND is set after successfull 
  ///    reception of a frame

  ///  - message object is used as receive object
  ///  - extended 29-bit identifier
  ///  - 0 valid data bytes
  ///  - this message object works with CAN node A
  ///  - remote monitoring is disabled
  ///  - receive interrupt node pointer: TwinCAN SRN 0
  CAN_HWOBJ[HW_RECEIVE_ALARM].CFGL = (can_backbone_hardware_node == 0) ? 0x0004 : 0x0006; // load message configuration register low
  CAN_HWOBJ[HW_RECEIVE_ALARM].CFGH = 0x0000; // load message configuration register high

  ///  - acceptance mask 29-bit: 0xFFFF
  ///  - identifier 29-bit:      0x0000
  CAN_HWOBJ[HW_RECEIVE_ALARM].AMR = 0xEC000000L; // load acceptance mask register
  CAN_HWOBJ[HW_RECEIVE_ALARM].AR = 0x00000000; // load arbitration register

  for (loop = 0; loop < 8; loop++) // load data with 0
    CAN_HWOBJ[HW_RECEIVE_ALARM].data[loop] = 0;

  ///  - functionality of standard message object
  CAN_HWOBJ[HW_RECEIVE_ALARM].FCRL = 0x0000; // load FIFO/gateway control register low
  CAN_HWOBJ[HW_RECEIVE_ALARM].FCRH = 0x0006; // load FIFO/gateway control register high

  CAN_HWOBJ[HW_RECEIVE_ALARM].CTRL = 0x5599; // load message control register low
  CAN_HWOBJ[HW_RECEIVE_ALARM].CTRH = 0x0000; // load message control register high

  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 7: (TRANSMIT ALARM)
  ///  -----------------------------------------------------------------------
  ///  - message object 7 is valid
  ///  - enable transmit interrupt; bit INTPND is set after successfull 
  ///    transmission of a frame

  ///  - message object is used as transmit object
  ///  - extended 29-bit identifier
  ///  - 0 valid data bytes
  ///  - this message object works with CAN node A
  ///  - remote monitoring is disabled
  ///  - transmit interrupt node pointer: TwinCAN SRN 0
  CAN_HWOBJ[HW_TRANSMIT_ALARM].CFGL = (can_backbone_hardware_node == 0) ? 0x000C : 0x000E; // load message configuration register low
  CAN_HWOBJ[HW_TRANSMIT_ALARM].CFGH = 0x0000; // load message configuration register high

  ///  - acceptance mask 29-bit: 0xFFFF
  ///  - identifier 29-bit:      0x0000
  CAN_HWOBJ[HW_TRANSMIT_ALARM].AMR = 0xFFFFFFFFL; // load acceptance mask register
  CAN_HWOBJ[HW_TRANSMIT_ALARM].AR = 0x00000000; // load arbitration register

  for (loop = 0; loop < 8; loop++) // load data with 0
    CAN_HWOBJ[HW_TRANSMIT_ALARM].data[loop] = 0;

  ///  - functionality of standard message object
  CAN_HWOBJ[HW_TRANSMIT_ALARM].FCRL = 0x0000; // load FIFO/gateway control register low
  CAN_HWOBJ[HW_TRANSMIT_ALARM].FCRH = 0x0007; // load FIFO/gateway control register high
  
  CAN_HWOBJ[HW_TRANSMIT_ALARM].CTRH = 0x0000; // load message control register high
  CAN_HWOBJ[HW_TRANSMIT_ALARM].CTRL = 0x55A5; // load message control register low

  ///  -----------------------------------------------------------------------
  ///  Configuration of Service Request Nodes 0 - 7:
  ///  -----------------------------------------------------------------------
  ///  SRN0 service request node configuration:
  ///  - SRN0 interrupt priority level (ILVL) = 6
  ///  - SRN0 interrupt group level (GLVL) = 0
  ///  - SRN0 group priority extension (GPX) = 0
  CAN_0IC = CAN_BACKBONE_EVENT_LEVEL | ENABLE_INT;

  // set IO pins for can backbone
  switch (can_backbone_hardware_used)
  {
    default:
    case CAN_BACKBONE_USE_NODE_A_NOT_EXTERN:
      break;
    case CAN_BACKBONE_USE_NODE_A_P4_5_P4_6:
      CAN_PISEL = (CAN_PISEL & 0xFFF8) | 0x0000;
      P4_5 = 1;
      DP4_5 = 0;
      AS0P4_6 = 1;
      P4_6 = 1;
      DP4_6 = 1;
      ODP4_6 = 1;
      break;
    case CAN_BACKBONE_USE_NODE_A_P7_6_P7_7:
      CAN_PISEL = (CAN_PISEL & 0xFFF8) | 0x0002;
      P7_6 = 1;
      DP7_6 = 0;
      AS0P7_7 = 1;
      P7_7 = 1;
      DP7_7 = 1;
      ODP7_7 = 1;
      break;
    case CAN_BACKBONE_USE_NODE_A_P9_2_P9_3:
      CAN_PISEL = (CAN_PISEL & 0xFFF8) | 0x0003;
      P9_2 = 1;
      DP9_2 = 0;
      AS0P9_3 = 1;
      P9_3 = 1;
      DP9_3 = 1;
      ODP9_3 = 1;
      break;
    case CAN_BACKBONE_USE_NODE_B_NOT_EXTERN:
      break;
    case CAN_BACKBONE_USE_NODE_B_P4_4_P4_7:
      CAN_PISEL = (CAN_PISEL & 0xFFC7) | 0x0000;
      P4_4 = 1;
      DP4_4 = 0;
      AS0P4_7 = 1;
      P4_7 = 1;
      DP4_7 = 1;
      ODP4_7 = 1;
      break;
    case CAN_BACKBONE_USE_NODE_B_P7_4_P7_5:
      CAN_PISEL = (CAN_PISEL & 0xFFC7) | 0x0010;
      P7_4 = 1;
      DP7_4 = 0;
      AS0P7_5 = 1;
      P7_5 = 1;
      DP7_5 = 1;
      ODP7_5 = 1;
      break;
    case CAN_BACKBONE_USE_NODE_B_P9_0_P9_1:
      CAN_PISEL = (CAN_PISEL & 0xFFC7) | 0x0008;
      P9_0 = 1;
      DP9_0 = 0;
      AS0P9_1 = 1;
      P9_1 = 1;
      DP9_1 = 1;
      ODP9_1 = 1;
      break;
  }

  ///  - enable interrupt generation when a message transfer is completed
  ///  - enable interrupt generation on a change of bit BOFF or EWARN
  ///  - enable interrupt generation on setting an error code in bit field LEC
  CAN_HWNODE[can_backbone_hardware_node].CR = 0x001C; // load global control register
} //  End of function CAN_vInit

//****************************************************************************
// @Function      void CAN_vGetMsgObj(unsigned char ubObjNr, s_CAN_SWObj *pstObj) 
//
//----------------------------------------------------------------------------
// @Description   This function fills the forwarded SW message object with 
//                the content of the chosen HW message object.
//                
//                The structure of the SW message object is defined in the 
//                header file CAN.H (see s_CAN_SWObj).
//
//----------------------------------------------------------------------------
// @Returnvalue   None
//
//----------------------------------------------------------------------------
// @Parameters    ubObjNr: 
//                Number of the message object to be read (0-31)
// @Parameters    *pstObj: 
//                Pointer on a message object to be filled by this function
//
//----------------------------------------------------------------------------
// @Date          4/27/2006
//
//****************************************************************************
static void CAN_vGetMsgObj(unsigned char ubObjNr, s_CAN_SWObj *pstObj)
{
unsigned char i;
unsigned char message_size = (CAN_HWOBJ[ubObjNr].CFGL & 0x00f0) >> 4;

  for (i = 0; i < message_size; i++)
    pstObj->data[i] = CAN_HWOBJ[ubObjNr].data[i];

  pstObj->ID   = CAN_HWOBJ[ubObjNr].AR;
  pstObj->mask = CAN_HWOBJ[ubObjNr].AMR;

  pstObj->CTRH = CAN_HWOBJ[ubObjNr].CTRH;
  pstObj->CFGL  = CAN_HWOBJ[ubObjNr].CFGL;
} //  End of function CAN_vGetMsgObj

//****************************************************************************
// @Function      unsigned char CAN_ubRequestMsgObj(unsigned char ubObjNr) 
//
//----------------------------------------------------------------------------
// @Description   If a TRANSMIT OBJECT is to be reconfigured it must first be 
//                accessed. The access to the transmit object is exclusive. 
//                This function checks whether the choosen message object is 
//                still executing a transmit request, or if the object can be 
//                accessed exclusively.
//                After the message object is reserved, it can be 
//                reconfigured by using the function CAN_vConfigMsgObj or 
//                CAN_vLoadData.
//                Both functions enable access to the object for the CAN 
//                controller. 
//                By calling the function CAN_vTransmit transfering of data 
//                is started.
//
//----------------------------------------------------------------------------
// @Returnvalue   0 message object is busy (a transfer is active), else 1
//
//----------------------------------------------------------------------------
// @Parameters    ubObjNr: 
//                Number of the message object (0-31)
//
//----------------------------------------------------------------------------
// @Date          4/27/2006
//
//****************************************************************************
static unsigned char CAN_ubRequestMsgObj(unsigned char ubObjNr)
{
unsigned char ubReturn = 0;

  if ((CAN_HWOBJ[ubObjNr].CTRL & 0x3000) == 0x1000)  // if reset TXRQ 
  {
    CAN_HWOBJ[ubObjNr].CTRL = 0xfbff;               // set CPUUPD 
    ubReturn = 1;
  }
  return (ubReturn);
} //  End of function CAN_ubRequestMsgObj

//****************************************************************************
// @Function      unsigned char CAN_ubNewData(unsigned char ubObjNr) 
//
//----------------------------------------------------------------------------
// @Description   This function checks whether the selected RECEIVE OBJECT 
//                has received a new message. If so the function returns the 
//                value '1'.
//
//----------------------------------------------------------------------------
// @Returnvalue   1 the message object has received a new message, else 0
//
//----------------------------------------------------------------------------
// @Parameters    ubObjNr: 
//                Number of the message object (0-31)
//
//----------------------------------------------------------------------------
// @Date          4/27/2006
//
//****************************************************************************
/*
static unsigned char CAN_ubNewData(unsigned char ubObjNr)
{
unsigned char ubReturn = 0;

  if ((CAN_HWOBJ[ubObjNr].CTRL & 0x0300) == 0x0200)  // if NEWDAT
  {
    ubReturn = 1;
  }
  return (ubReturn);
} //  End of function CAN_ubNewData
*/
//****************************************************************************
// @Function      void CAN_vTransmit(unsigned char ubObjNr) 
//
//----------------------------------------------------------------------------
// @Description   This function triggers the CAN controller to send the 
//                selected message.
//                If the selected message object is a TRANSMIT OBJECT then 
//                this function triggers the sending of a data frame. If 
//                however the selected message object is a RECEIVE OBJECT 
//                this function triggers the sending of a remote frame.
//
//----------------------------------------------------------------------------
// @Returnvalue   None
//
//----------------------------------------------------------------------------
// @Parameters    ubObjNr: 
//                Number of the message object (0-31)
//
//----------------------------------------------------------------------------
// @Date          4/27/2006
//
//****************************************************************************
_inline void CAN_vTransmit(unsigned char ubObjNr)
{
  CAN_HWOBJ[ubObjNr].CTRL = 0xe7ff;  // set TXRQ, reset CPUUPD
} //  End of function CAN_vTransmit

//****************************************************************************
// @Function      void CAN_vConfigMsgObj(unsigned char ubObjNr, s_CAN_SWObj *pstObj) 
//
//----------------------------------------------------------------------------
// @Description   This function sets up the message objects. This includes 
//                the 8 data bytes, the identifier (11- or 29-bit), the 
//                acceptance mask (11- or 29-bit), the data number (0-8 
//                bytes), the frame counter value and the XTD-bit (standard 
//                or extended identifier).  The direction bit (DIR), the NODE 
//                bit and the RMM (remote monitoring) bit can not be changed. 
//                The message is not sent; for this the function 
//                CAN_vTransmit must be called.
//                
//                The structure of the SW message object is defined in the 
//                header file CAN.H (see s_CAN_SWObj).
//
//----------------------------------------------------------------------------
// @Returnvalue   None
//
//----------------------------------------------------------------------------
// @Parameters    ubObjNr: 
//                Number of the message object to be configured (0-31)
// @Parameters    *pstObj: 
//                Pointer on a message object
//
//----------------------------------------------------------------------------
// @Date          4/27/2006
//
//****************************************************************************
static void CAN_vConfigMsgObj(unsigned char ubObjNr, s_CAN_SWObj *pstObj)
{
unsigned char i;
unsigned char message_size = (pstObj->CFGL & 0x00f0) >> 4;

  CAN_HWOBJ[ubObjNr].CTRL = 0xfb7f;     // set CPUUPD, reset MSGVAL

  CAN_HWOBJ[ubObjNr].CFGL |= 0x0004;
  CAN_HWOBJ[ubObjNr].AR   = pstObj->ID;
  CAN_HWOBJ[ubObjNr].AMR  = pstObj->mask;

  CAN_HWOBJ[ubObjNr].CTRH = pstObj->CTRH;
  CAN_HWOBJ[ubObjNr].CFGL  = (CAN_HWOBJ[ubObjNr].CFGL & 0x000f) | (pstObj->CFGL & 0x00f0);

  if (CAN_HWOBJ[ubObjNr].CFGL & 0x0008)  // if transmit direction
  {
    for (i = 0; i < message_size; i++)
      CAN_HWOBJ[ubObjNr].data[i] = pstObj->data[i];
    CAN_HWOBJ[ubObjNr].CTRL  = 0xf6bf;  // set NEWDAT, reset CPUUPD, 
  }                                         // set MSGVAL
  else                                      // if receive direction
  {
    CAN_HWOBJ[ubObjNr].CTRL  = 0xf7bf;  // reset CPUUPD, set MSGVAL
  }
} //  End of function CAN_vConfigMsgObj

//****************************************************************************
// @Function      void CAN_vLoadData(unsigned char ubObjNr, unsigned char *pubData) 
//
//----------------------------------------------------------------------------
// @Description   If a hardware TRANSMIT OBJECT has to be loaded with data 
//                but not with a new identifier, this function may be used 
//                instead of the function CAN_vConfigMsgObj. The message 
//                object should be accessed by calling the function 
//                CAN_ubRequestMsgObj before calling this function. This 
//                prevents the CAN controller from working with invalid data.
//
//----------------------------------------------------------------------------
// @Returnvalue   None
//
//----------------------------------------------------------------------------
// @Parameters    ubObjNr: 
//                Number of the message object to be configured (0-31)
// @Parameters    *pubData: 
//                Pointer on a data buffer
//
//----------------------------------------------------------------------------
// @Date          4/27/2006
//
//****************************************************************************
/*
static void CAN_vLoadData(unsigned char ubObjNr, unsigned char *pubData)
{
unsigned char i;
unsigned char message_size = (CAN_HWOBJ[ubObjNr].CFGL & 0xf0) >> 4;

  CAN_HWOBJ[ubObjNr].CTRL = 0xfaff;       // set CPUUPD and NEWDAT

  for (i = 0; i < message_size; i++)
    CAN_HWOBJ[ubObjNr].data[i] = *(pubData++);

  CAN_HWOBJ[ubObjNr].CTRL = 0xf7ff;       // reset CPUUPD
} //  End of function CAN_vLoadData
*/

//****************************************************************************
// @Function      unsigned char CAN_ubMsgLost(unsigned char ubObjNr) 
//
//----------------------------------------------------------------------------
// @Description   If a RECEIVE OBJECT receives new data before the old object 
//                has been read, the old object is lost. The CAN controller 
//                indicates this by setting the message lost bit (MSGLST). 
//                This function returns the status of this bit. 
//                
//                Note:
//                This function resets the message lost bit (MSGLST).
//
//----------------------------------------------------------------------------
// @Returnvalue   1 the message object has lost a message, else 0
//
//----------------------------------------------------------------------------
// @Parameters    ubObjNr: 
//                Number of the message object (0-31)
//
//----------------------------------------------------------------------------
// @Date          4/27/2006
//
//****************************************************************************
/*
static unsigned char CAN_ubMsgLost(unsigned char ubObjNr)
{
unsigned char ubReturn = 0;

  if ((CAN_HWOBJ[ubObjNr].CTRL & 0x0c00) == 0x0800)  // if set MSGLST 
  {
    CAN_HWOBJ[ubObjNr].CTRL = 0xf7ff;               // reset MSGLST 
    ubReturn = 1;
  }
  return (ubReturn);
} //  End of function CAN_ubMsgLost
*/
//****************************************************************************
// @Function      unsigned char CAN_ubDelMsgObj(unsigned char ubObjNr) 
//
//----------------------------------------------------------------------------
// @Description   This function marks the selected message object as not 
//                valid. This means that this object cannot be sent or 
//                receive data. If the selected object is busy (meaning the 
//                object is transmitting a message or has received a new 
//                message) this function returns the value "0" and the object 
//                is not deleted.
//
//----------------------------------------------------------------------------
// @Returnvalue   1 the message object was deleted, else 0
//
//----------------------------------------------------------------------------
// @Parameters    ubObjNr: 
//                Number of the message object (0-31)
//
//----------------------------------------------------------------------------
// @Date          4/27/2006
//
//****************************************************************************
/*
static unsigned char CAN_ubDelMsgObj(unsigned char ubObjNr)
{
unsigned char ubReturn = 0;

  if (!(CAN_HWOBJ[ubObjNr].CTRL & 0xa200)) // if set RMTPND, TXRQ or NEWDAT
  {
    CAN_HWOBJ[ubObjNr].CTRL = 0xff7f;     // reset MSGVAL
    ubReturn = 1;
  }
  return (ubReturn);

} //  End of function CAN_ubDelMsgObj
*/
//****************************************************************************
// @Function      void CAN_vReleaseObj(unsigned char ubObjNr) 
//
//----------------------------------------------------------------------------
// @Description   This function resets the NEWDAT flag of the selected 
//                RECEIVE OBJECT, so that the CAN controller have access to 
//                it. This function must be called if the function 
//                CAN_ubNewData detects, that new data are present in the 
//                message object and the actual data have been read by 
//                calling the function CAN_vGetMsgObj. 
//
//----------------------------------------------------------------------------
// @Returnvalue   None
//
//----------------------------------------------------------------------------
// @Parameters    ubObjNr: 
//                Number of the message object (0-31)
//
//----------------------------------------------------------------------------
// @Date          4/27/2006
//
//****************************************************************************
_inline void CAN_vReleaseObj(unsigned char ubObjNr)
{
  CAN_HWOBJ[ubObjNr].CTRL = 0x7DFF; // JP 15-11-2006 reset rmtpnd newdat // 0xfdff;       // reset NEWDAT
} //  End of function CAN_vReleaseObj

//****************************************************************************
// @Function      void CAN_vSetMSGVAL(unsigned char ubObjNr) 
//
//----------------------------------------------------------------------------
// @Description   This function sets the MSGVAL flag of the selected object. 
//                This is only necessary if the single data transfer mode 
//                (SDT) for the selected object is enabled. If SDT is set to 
//                '1', the CAN controller automatically resets bit MSGVAL 
//                after receiving or tranmission of a frame.
//
//----------------------------------------------------------------------------
// @Returnvalue   None
//
//----------------------------------------------------------------------------
// @Parameters    ubObjNr: 
//                Number of the message object (0-31)
//
//----------------------------------------------------------------------------
// @Date          4/27/2006
//
//****************************************************************************
/*
_inline void CAN_vSetMSGVAL(unsigned char ubObjNr)
{
  CAN_HWOBJ[ubObjNr].CTRL = 0xffbf;  // set MSGVAL
} //  End of function CAN_vSetMSGVAL
*/
/*
 ** CAN_SW_SetCommand
 *
 *  FILENAME: C:\C166_Projecten\CAN-Backbone\sources\CAN.C
 *
 *  PARAMETERS: ptr - pointer to the software message object that will be modified
        Command - code for the command; 
 *
 *  DESCRIPTION: the function sets bits 0,1,2,3,4,5 in the ID of the object to be sent 
         to the value set in parameter "Command"
 *
 *  RETURNS:
 *
 */
_inline void CAN_SW_SetCommand(s_CAN_SWObj *ptr, unsigned int Command)
{
  ptr->ID &= 0xFFFFFFE0; //clear whatever is in the command bit field of data byte 0 at the moment
  ptr->ID |= Command;
}

/*
 ** CAN_SW_GetCommand *
 *  FILENAME: C:\C166_Projecten\CAN-Backbone\sources\CAN.C
 *
 *  PARAMETERS: ptr - pointer to the software message object 
 *
 *  DESCRIPTION: the function returns the command code in the received SDO frame
 *
 *  RETURNS: 1...10 (16)
 *
 */
_inline unsigned int CAN_SW_GetCommand(s_CAN_SWObj *ptr)
{
  return (ptr->ID & 0x1F);
}

_inline void CAN_SW_SetCommand_Fast(s_CAN_SWObj *ptr, unsigned int Command)
{
// LETOP: command mag niet groter zijn dan 0x0007
// for XC161CJ
  ptr->ID &= 0xFFFFFFF8; //clear whatever is in the command bit field of data byte 0 at the moment
  ptr->ID |= Command;
}

_inline unsigned int CAN_SW_GetCommand_Fast(s_CAN_SWObj *ptr)
{
  return (ptr->ID & 0x07);
}
/*
 ** CAN_SW_SetToggleBit
 *
 *  FILENAME: C:\C166_Projecten\CAN-Backbone\sources\CAN.C
 *
 *  PARAMETERS: 
        Value - value to be set (SET or RESET)
 *
 *  DESCRIPTION: gets the value of the toggle bit form bit 6 of msg ID
 *
 *  RETURNS: 0 or 1
 *
 */
_inline void CAN_SW_SetToggleBit(s_CAN_SWObj *ptr, unsigned char Value)
{
  if (Value == SET)
    ptr->ID |= 0x20; 
  else
    ptr->ID &= 0xFFFFFFDF;
} 

/*
 ** CAN_SW_GetToggleBit
 *
 *  FILENAME: C:\C166_Projecten\CAN-Backbone\sources\CAN.C
 *
 *  PARAMETERS: ptr - pointer to sw msg obj 
 *
 *  DESCRIPTION: gets the value of the toggle bit form bit 6 of msg ID
 *
 *  RETURNS: 0 or 1
 *
 */
_inline unsigned char CAN_SW_GetToggleBit(s_CAN_SWObj *ptr)
{
  return ((ptr->ID & 0x20) >> 5);
} 

_inline void CAN_SW_Set_Count_Fast(s_CAN_SWObj *ptr, unsigned char Value)
{
// for XC161CJ
  ptr->ID &= 0xFFFFFFC7; // clear toggle bit
  ptr->ID |= (Value << 3);
} 

_inline unsigned char CAN_SW_Get_Count_Fast(s_CAN_SWObj *ptr)
{
// XC161CJ
  return ((ptr->ID >> 3) & 0x07);
} 

/*
 ** CAN_SW_SetReceiverAddress
 *
 *  FILENAME: C:\C166_Projecten\CAN-Backbone\sources\CAN.C
 *
 *  PARAMETERS: ptr - pointer to the software object that is to be changed
        RecAdr - receiver address to filled in the identifier
 *
 *  DESCRIPTION: fills in the receiver address fied of a sw object
 *
 *  RETURNS: 
 *
 */
_inline void CAN_SW_SetReceiverAddress(s_CAN_SWObj *ptr, unsigned int RecAdr)
{
  ptr->ID &= 0xFFFF003F; //clear whatever is in the receiver address field at the moment
  ptr->ID |= (RecAdr << 6);
}

/*
 ** CAN_SW_GetReceiverAddress
 *
 *  FILENAME: C:\C166_Projecten\CAN-Backbone\sources\CAN.C
 *
 *  PARAMETERS: ptr - pointer to the software message object 
 *
 *  DESCRIPTION: the function returns the "Sender Address" field
 *
 *  RETURNS: 1...999 - the address used by the sender on the CAN bus
 *
 */
_inline unsigned int CAN_SW_GetReceiverAddress(s_CAN_SWObj *ptr)
{
  return ((ptr->ID & 0x0000FFC0) >> 6);
}

/*
 ** CAN_SW_SetSenderAddress
 *
 *  FILENAME: C:\C166_Projecten\CAN-Backbone\sources\CAN.C
 *
 *  PARAMETERS: ptr - pointer to the software object that is to be changed
        SendAdr - sender address to filled in the identifier
 *
 *  DESCRIPTION: fills in the sender address fied of a sw object
 *
 *  RETURNS: 
 *
 */
_inline void CAN_SW_SetSenderAddress(s_CAN_SWObj *ptr, unsigned int SendAdr)
{
  ptr->ID &= 0xFC00FFFF; //clear whatever is in the sender address field at the moment
  ptr->ID |= ((unsigned long)SendAdr << 16);
}

/*
 ** CAN_SW_GetSenderAddress
 *
 *  FILENAME: C:\C166_Projecten\CAN-Backbone\sources\CAN.C
 *
 *  PARAMETERS: ptr - pointer to the software message object 
 *
 *  DESCRIPTION: the function returns the "Sender Address" field
 *
 *  RETURNS: 1...999 - the address used by the sender on the CAN bus
 *
 */
_inline unsigned int CAN_SW_GetSenderAddress(s_CAN_SWObj *ptr)
{
  return ((ptr->ID & 0x03FF0000) >> 16);
}

/*
 ** CAN_SW_SwapAddresses
 *
 *  FILENAME: C:\C166_Projecten\CAN-Backbone\sources\CAN.C
 *
 *  PARAMETERS: ptr - pointer to the software message object that will be modified
 *
 *  DESCRIPTION: the function swaps the content of "Sender Address" and "Receiver Address" fields
 *
 *  RETURNS:
 *
 */
_inline void CAN_SW_SwapAddresses(s_CAN_SWObj *ptr)
{
unsigned long rec, send;

  rec = (ptr->ID & 0xFFC0) << 10; // extract receiver and shift to new position
  send = (ptr->ID & 0x03FF0000) >> 10; //extract sender and shift to new position
  ptr->ID &= 0xFC00003F; //clear both sender and receiver fields
  ptr->ID |= rec | send;
}

/*
 ** CAN_SW_SetMessageType
 *
 *  FILENAME: C:\C166_Projecten\CAN-Backbone\sources\CAN.C
 *
 *  PARAMETERS: ptr - pointer to the software message object that will be modified
        MsgType - type of message : MESSAGE_TYPE_VALUE, MESSAGE_TYPE_ALARM, MESSAGE_TYPE_MASTER, MESSAGE_TYPE_SLAVE
 *
 *  DESCRIPTION: the function sets bits 4,5 in the ID of the object to be sent
         0 (00b) - MESSAGE_TYPE_ALARM
         1 (01b) - MESSAGE_TYPE_VALUE
         3 (11b) - MESSAGE_TYPE_MASTER
         2 (10b) - MESSAGE_TYPE_SLAVE
 *
 *  RETURNS:
 *
 */
_inline void CAN_SW_SetMessageType(s_CAN_SWObj *ptr, unsigned int MsgType)
{
  ptr->ID &= 0xF3FFFFFF; // clear whatever is in the TYPE bits at the moment
  ptr->ID |= ((unsigned long)MsgType << 26);
}

/*
 ** CAN_SW_GetMessageType
 *
 *  FILENAME: C:\C166_Projecten\CAN-Backbone\sources\CAN.C
 *
 *  PARAMETERS: ptr - pointer to sw msg obj
 *
 *  DESCRIPTION: returns the message type bits  
 *
 *  RETURNS:
 *
 */
_inline unsigned char CAN_SW_GetMessageType(s_CAN_SWObj *ptr)
{
  return (unsigned char)((ptr->ID & 0x0C000000) >> 26);
}

/*
 ** CAN_SW_SetPriority
 *
 *  FILENAME: C:\C166_Projecten\CAN-Backbone\sources\CAN.C
 *
 *  PARAMETERS: ptr - pointer to the software message object that will be modified
        Priority - value for the priority; 
 *
 *  DESCRIPTION: the function sets bit 29 in the ID of the object to be sent to the specified value
         A LOWER VALUE MEANS HIGHER PRIORITY !!!
 *
 *  RETURNS:
 *
 */
_inline void CAN_SW_SetPriority(s_CAN_SWObj *ptr, unsigned char Priority)
{
  if (Priority)
    ptr->ID |= 0x10000000;
  else
    ptr->ID &= 0xEFFFFFFF;  
}

/*
 ** CAN_SW_SetNrOfBytes
 *
 *  FILENAME: C:\C166_Projecten\CAN-Backbone\sources\CAN.C
 *
 *  PARAMETERS: ptr - pointer to the software message object that will be modified
        Nr - number of bytes with data in this frame; 
 *
 *  DESCRIPTION: the function sets bits 4,5,6,7 in the MSGCGF, that corespond to DLC
 *
 *  RETURNS:
 *
 */
_inline void CAN_SW_SetNrOfBytes(s_CAN_SWObj *ptr, unsigned int Nr)
{
  ptr->CFGL = ptr->CFGL & 0x0F; //clear sw DLC field
  ptr->CFGL |= Nr << 4;
}

/*
 ** CAN_SW_GetNrOfBytes
 *
 *  FILENAME: C:\C166_Projecten\CAN-Backbone\sources\CAN.C
 *
 *  PARAMETERS: ptr - pointer to the software message object 
 *
 *  DESCRIPTION: the function returns the number of bytes in the received SDO frame
 *
 *  RETURNS: 1...7
 *
 */
_inline CAN_SW_GetNrOfBytes(s_CAN_SWObj *ptr)
{
   return ((ptr->CFGL & 0xF0) >> 4);
}


/*
 ** CAN_SW_SetExtendedID
 *
 *  FILENAME: C:\C166_Projecten\CAN-Backbone\sources\CAN.C
 *
 *  PARAMETERS: ptr - pointer to sw msg obj 
 *
 *  DESCRIPTION: sets the message ID as an extended (29-bits long)
 *
 *  RETURNS:
 *
 */
_inline void CAN_SW_SetExtendedID(s_CAN_SWObj *ptr)
{
  ptr->CFGL |= 0x0004; 
} 

static void PUSH(s_CAN_SWObj *sw_msgobj_ptr, s_fifo *buffer_ptr)
{
  if (buffer_ptr->full == 0)
  {
    buffer_ptr->array[buffer_ptr->push_index] = *sw_msgobj_ptr;
    // if we don't check overflow, we may corrupt a lot!
    // when overflowing, just ignore the message!
    buffer_ptr->push_index++;

    if (buffer_ptr->push_index >= FIFO_BUFFER_SIZE)
      buffer_ptr->push_index = 0;
    
    if (buffer_ptr->push_index == buffer_ptr->pop_index)
      buffer_ptr->full = 1;
  }
}

static s_CAN_SWObj POP(s_fifo *buffer_ptr)
{
s_CAN_SWObj sw_msgobj;

  sw_msgobj = buffer_ptr->array[buffer_ptr->pop_index];
  buffer_ptr->pop_index++;
  
  // when popping, buffer is always not full!
  buffer_ptr->full = 0;

  if (buffer_ptr->pop_index >= FIFO_BUFFER_SIZE)
    buffer_ptr->pop_index = 0;

  return sw_msgobj;
}

_inline s_CAN_SWObj PEEK(s_fifo *buffer)
{
  return buffer->array[buffer->pop_index];  
}

//flushes the s_fifo and resets the pointers
_inline void FLUSH(s_fifo *buffer)
{
  buffer->pop_index = 0;
  buffer->push_index = 0;
  buffer->full = 0;
}

/*
 ** SetupTransmitValueMsgObj
 *
 *  FILENAME: C:\C166_Projecten\CAN-Backbone\sources\CAN.C
 *
 *  PARAMETERS: ptr - pointer to a transmit value struct
 *
 *  DESCRIPTION: the function takes out data from the specified transmit value struct and 
         fills a "s_CAN_SWObj" that cand be then send via the bus
 *
 *  RETURNS: s_CAN_SWObj that can be send on the bus
 *
 */
static s_CAN_SWObj SetupTransmitValueMsgObj(s_can_backbone_transmit_value *transmit_value_ptr)
{
s_CAN_SWObj sw_msgobj; 
unsigned char loop;
unsigned char message_size = transmit_value_ptr->message_size;
  
  CAN_SW_SetPriority(&sw_msgobj, transmit_value_ptr->priority);
//  CAN_SW_SetSenderAddress(&sw_msgobj, transmit_value_ptr->address_sender); 
  CAN_SW_SetSenderAddress(&sw_msgobj, transmit_value_ptr->node_ptr->address_node); 
  CAN_SW_SetReceiverAddress(&sw_msgobj, transmit_value_ptr->address_receiver); 
  CAN_SW_SetMessageType(&sw_msgobj, MESSAGE_TYPE_VALUE);
  CAN_SW_SetToggleBit(&sw_msgobj, 0);
  CAN_SW_SetCommand(&sw_msgobj, transmit_value_ptr->command);
  CAN_SW_SetExtendedID(&sw_msgobj);

  for (loop = 0; loop < message_size; loop++)
    sw_msgobj.data[loop] = transmit_value_ptr->buffer[loop];
  CAN_SW_SetNrOfBytes(&sw_msgobj, message_size);
  return (sw_msgobj);
}

/*
 ** SetupAlarmMsgObj
 *
 *  FILENAME: C:\C166_Projecten\CAN-Backbone\sources\CAN.C
 *
 *  PARAMETERS: ptr - pointer to a transmit alarm struct
 *
 *  DESCRIPTION: the function takes out data from the specified transmit alarm struct and 
         fills a "s_CAN_SWObj" that cand be then send via the bus
 *
 *  RETURNS: s_CAN_SWObj that can be send on the bus
 *
 */
static s_CAN_SWObj SetupAlarmMsgObj(s_can_backbone_transmit_alarm *transmit_alarm_ptr)
{
s_CAN_SWObj sw_msgobj; 
unsigned char loop;
unsigned char message_size = transmit_alarm_ptr->message_size;
  
  CAN_SW_SetPriority(&sw_msgobj, transmit_alarm_ptr->priority);
//  CAN_SW_SetSenderAddress(&sw_msgobj, transmit_alarm_ptr->address_sender); 
  CAN_SW_SetSenderAddress(&sw_msgobj, transmit_alarm_ptr->node_ptr->address_node); 
  CAN_SW_SetReceiverAddress(&sw_msgobj, transmit_alarm_ptr->address_receiver); 
  CAN_SW_SetMessageType(&sw_msgobj, MESSAGE_TYPE_ALARM);
  CAN_SW_SetToggleBit(&sw_msgobj, 0);
  CAN_SW_SetCommand(&sw_msgobj, 0);
  CAN_SW_SetExtendedID(&sw_msgobj);

  for (loop = 0; loop < message_size; loop++)
    sw_msgobj.data[loop] = transmit_alarm_ptr->buffer[loop];
  CAN_SW_SetNrOfBytes(&sw_msgobj, message_size);

  return sw_msgobj;
}

/*
 ** SetupMasterMsgObj
 *
 *  FILENAME: C:\C166_Projecten\CAN-Backbone\sources\CAN.C
 *
 *  PARAMETERS: ptr - pointer to a sdo master struct
 *
 *  DESCRIPTION: the function takes out data from the specified sdo struct and 
         fills a "s_CAN_SWObj" that cand be then send via the bus
         the object contains the first 8 (or less) bytes and the first 
         command that the master send to the slave
         the rest of the communication will be handeled by the interrupt
 *
 *  RETURNS: s_CAN_SWObj that can be send on the bus
 *
 */
static s_CAN_SWObj SetupMasterMsgObj(s_can_backbone_master *master_ptr)
{
s_CAN_SWObj sw_msgobj;
unsigned char i;
unsigned char message_size;

  master_ptr->buffer_index = 0;
  master_ptr->send_reset = 1;
  
  CAN_SW_SetExtendedID(&sw_msgobj);
  CAN_SW_SetMessageType(&sw_msgobj, MESSAGE_TYPE_MASTER); 
  //set receiver addres as the address of the slave
  CAN_SW_SetReceiverAddress(&sw_msgobj, master_ptr->address_receiver);
  //set the sender address as the address of the master
//  CAN_SW_SetSenderAddress(&sw_msgobj, master_ptr->address_sender);
  CAN_SW_SetSenderAddress(&sw_msgobj, master_ptr->node_ptr->address_node);
  CAN_SW_SetPriority(&sw_msgobj, master_ptr->priority);

  if (master_ptr->message_size > 8)
  {
    if (master_ptr->fast)
    {
      CAN_SW_SetCommand_Fast(&sw_msgobj, M2S_WRITE_START_MORE_THEN_8_BYTES_OF_DATA_FAST); 
      master_ptr->toggle_bit = 0;
      CAN_SW_Set_Count_Fast(&sw_msgobj, 0);
    }  
    else  
    {
      CAN_SW_SetCommand(&sw_msgobj, M2S_WRITE_START_MORE_THEN_8_BYTES_OF_DATA); 
      master_ptr->toggle_bit = SET;
      CAN_SW_SetToggleBit(&sw_msgobj, master_ptr->toggle_bit);
    }
    message_size = 8;
  }
  else
  { 
    if (master_ptr->fast)
    {
      CAN_SW_SetCommand_Fast(&sw_msgobj, M2S_WRITE_START_MAX_8_BYTES_OF_DATA_FAST);
      master_ptr->toggle_bit = 0;
      CAN_SW_Set_Count_Fast(&sw_msgobj, 0);
    }  
    else
    {
      CAN_SW_SetCommand(&sw_msgobj, M2S_WRITE_START_MAX_8_BYTES_OF_DATA);
      master_ptr->toggle_bit = SET;
      CAN_SW_SetToggleBit(&sw_msgobj, master_ptr->toggle_bit);
    }
    message_size = master_ptr->message_size;
  }
  CAN_SW_SetNrOfBytes(&sw_msgobj, message_size);
  //fill sw obj msg data bytes
  for (i = 0; i < message_size; i++)
  {
    sw_msgobj.data[i] = master_ptr->buffer_ptr[master_ptr->buffer_index];
    master_ptr->buffer_index++;
  }
  return sw_msgobj; 
}

//*****************************************************************************
void Can_Backbone_Hardware_Busoff_Reset(void)
{
s_CAN_SWObj sw_msgobj;
s_can_backbone_transmit_value *transmit_value_ptr;
s_can_backbone_transmit_alarm *transmit_alarm_ptr;
long loop = 0;

  can_backbone_hardware_busoff_reset_switch = 0;
  CAN_HWNODE[can_backbone_hardware_node].CR = 0x001C;
  while (CAN_HWNODE[can_backbone_hardware_node].SR & 0x0080)
  {
    loop++;
    if (loop >= 50000)
    {
      _nop(); _nop(); _nop(); _nop(); _nop(); _nop();
      can_backbone_appl_init_switch = 1;
      return;
    }  
  }
  CAN_0IC_IE = 1;
  transmit_alarm_ptr = can_backbone_first_transmit_alarm_ptr; 
  while (transmit_alarm_ptr != (s_can_backbone_transmit_alarm *)END_OF_CAN_LIST)   
  {
    if (transmit_alarm_ptr->send == TRUE)
    {
      //send (in this case the condition should always be true) 
      if (CAN_ubRequestMsgObj(HW_TRANSMIT_ALARM))
      {
        //setup sw_msgobj
        sw_msgobj = SetupAlarmMsgObj(transmit_alarm_ptr);
        //remember the last alarm sent
        can_backbone_last_transmit_alarm_ptr = transmit_alarm_ptr;
        CAN_vConfigMsgObj(HW_TRANSMIT_ALARM, &sw_msgobj);
        CAN_vTransmit(HW_TRANSMIT_ALARM); 
      }
      break; //break "for" loop
    }
    transmit_alarm_ptr = transmit_alarm_ptr->next_ptr;
  }
  //reinit transmission of pdos
  //check if there is anything else to send
  transmit_value_ptr = can_backbone_first_transmit_value_ptr; 
  while (transmit_value_ptr != (s_can_backbone_transmit_value *)END_OF_CAN_LIST)   
  {
    if (transmit_value_ptr->send == TRUE)
    {
      //send (in this case the condition should always be true) 
      if (CAN_ubRequestMsgObj(HW_TRANSMIT_VALUE))
      {
        //setup sw_msgobj
        sw_msgobj = SetupTransmitValueMsgObj(transmit_value_ptr);
        //remember the last value sent
        can_backbone_last_transmit_value_ptr = transmit_value_ptr;
        CAN_vConfigMsgObj(HW_TRANSMIT_VALUE, &sw_msgobj);
        CAN_vTransmit(HW_TRANSMIT_VALUE); 
      }
      break; //break "for" loop
    }
    transmit_value_ptr = transmit_value_ptr->next_ptr;
  }
  //reinit transmission of sdos
  //check if there are any
  if ((can_backbone_master_transmit_fifo.pop_index != can_backbone_master_transmit_fifo.push_index) ||
    (can_backbone_master_transmit_fifo.full == 1))
  {
    if (CAN_ubRequestMsgObj(HW_TRANSMIT_MASTER)) //in this case it should be true since the bus was just reset
    {
      sw_msgobj = POP(&can_backbone_master_transmit_fifo);
      CAN_vConfigMsgObj(HW_TRANSMIT_MASTER, &sw_msgobj);
      CAN_vTransmit(HW_TRANSMIT_MASTER); 
    }
  }
}  

//****************************************************************************
// @Function      void Can_Backbone_Interrupt(void) 
//
//----------------------------------------------------------------------------
// @Description   This is the interrupt service routine for the Service 
//                Request Node 0 of the TwinCAN module.
//
//----------------------------------------------------------------------------
// @Returnvalue   None
//
//----------------------------------------------------------------------------
// @Parameters    None
//
//----------------------------------------------------------------------------
// @Date          6/1/2006
//
//****************************************************************************
#ifndef CANopen
interrupt (CAN_BACKBONE_ADR) using(CAN_BACKBONE_RB) void Can_Backbone_Interrupt(void)
{
s_CAN_SWObj sw_msgobj;
unsigned char j;
unsigned char message_size;
unsigned int status;
unsigned int IntID;
unsigned int address_receiver_in;
unsigned int address_sender_in;
s_can_backbone_receive_value  *receive_value_ptr;
s_can_backbone_transmit_value *transmit_value_ptr;
s_can_backbone_receive_alarm  *receive_alarm_ptr;
s_can_backbone_transmit_alarm *transmit_alarm_ptr;
s_can_backbone_master *master_ptr;
s_can_backbone_slave *slave_ptr;
  
  while(CAN_HWNODE[can_backbone_hardware_node].IR)
  {
    IntID = CAN_HWNODE[can_backbone_hardware_node].IR;

    CAN_HWNODE[can_backbone_hardware_node].IR = 0;
    switch(IntID)
    {
      case 1:
        // status change interrupt of node A
        status = CAN_HWNODE[can_backbone_hardware_node].SR;
        if (status & 0x0080)  // if BOFF
        {
          // Indicates when the CAN controller is in busoff state.
          if (can_backbone_hardware_used_extern == TRUE)
          {
            can_backbone_status.BOFF++;
            can_backbone_status.EWRN = 0;

            CAN_0IC_IE = 0;
            CAN_HWNODE[can_backbone_hardware_node].CR = 0x001C;
            can_backbone_hardware_busoff_reset_switch = 1;
            FLUSH(&can_backbone_master_transmit_fifo);
            FLUSH(&can_backbone_slave_transmit_fifo);
            master_ptr = can_backbone_first_master_ptr;
            while (master_ptr != (s_can_backbone_master *)END_OF_CAN_LIST)
            {
              if (master_ptr->send == TRUE)
              { 
                sw_msgobj = SetupMasterMsgObj(master_ptr);
                PUSH(&sw_msgobj, &can_backbone_master_transmit_fifo);  
                if (master_ptr->fast)
                {
                  int j;
    
                  while ((master_ptr->fast == SDO_MAX_BLOCKS) || master_ptr->toggle_bit < 7)
                  {
                    if (master_ptr->message_size - master_ptr->buffer_index > 0)
                    {
                      master_ptr->toggle_bit++;
                      master_ptr->toggle_bit &= 0x07;
                      CAN_SW_Set_Count_Fast(&sw_msgobj, master_ptr->toggle_bit);
                      if (master_ptr->message_size - master_ptr->buffer_index <= 8)
                      {
                        message_size = master_ptr->message_size - master_ptr->buffer_index;
                        CAN_SW_SetCommand_Fast(&sw_msgobj, M2S_WRITE_LAST_BLOCK_OF_DATA_FAST);
                        for (j = 0; j < message_size; j++)
                        {
                          sw_msgobj.data[j] = master_ptr->buffer_ptr[master_ptr->buffer_index];
                          master_ptr->buffer_index++;
                        }
                        CAN_SW_SetNrOfBytes(&sw_msgobj, message_size);
                        PUSH(&sw_msgobj, &can_backbone_master_transmit_fifo);
                        break;
                      }
                      else
                      {
                        message_size = 8;
                        CAN_SW_SetCommand_Fast(&sw_msgobj, M2S_WRITE_NEXT_BLOCK_OF_DATA_FAST);
                        for (j = 0; j < message_size; j++)
                        {
                          sw_msgobj.data[j] = master_ptr->buffer_ptr[master_ptr->buffer_index];
                          master_ptr->buffer_index++;
                        }
                        CAN_SW_SetNrOfBytes(&sw_msgobj, message_size);
                        PUSH(&sw_msgobj, &can_backbone_master_transmit_fifo);
                      }
                    }
                    else
                    {
                      // no more data to send
                      break;
                    }  
                  }
                }
              }
              master_ptr = master_ptr->next_ptr;
            }
          }
        }
        if (status & 0x00040)  // if EWRN
        {
          // Indicates that the least one of the error counters in the
          // EML has reached the error warning limit of 96.
          can_backbone_status.EWRN++;
        }
        if (status & 0x0008)  // if TXOK
        {
          // Indicates that a message has been transmitted successfully
          // (error free and acknowledged by at least one other node).
          status &= 0xfff7;
          CAN_HWNODE[can_backbone_hardware_node].SR    = status;    // reset TXOK
          can_backbone_status.TXOK++;
        }
        if (status & 0x0010)  // if RXOK
        {
          // Indicates that a message has been received successfully.
          status &= 0xffef;
          CAN_HWNODE[can_backbone_hardware_node].SR    = status;    // reset RXOK
          can_backbone_status.RXOK++;
        }
        if (status & 0x0007)  // if LEC
        {
          switch (status & 0x0007)  // LECA (Last Error CodeA)
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
              if (status & 0x0080)  // if Busoff status
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
          status &= 0xfff8;
          CAN_HWOBJ[0].CTRL = 0xFFFF; // erata TWINCAN_AI.007
          CAN_HWNODE[can_backbone_hardware_node].SR = status; // reset LEC
        }
        break;
      case 2:
        //************************************************
        // message object 0 interrupt (RECEIVE MASTER)
if ((CAN_HWOBJ[HW_RECEIVE_MASTER].CTRL & 0x0003) == 0x0002) // if INTPND
{
        CAN_HWOBJ[HW_RECEIVE_MASTER].CTRL = 0xfffd;  // reset INTPND
        if ((CAN_HWOBJ[HW_RECEIVE_MASTER].CTRL & 0x0300) == 0x0200) // NEWDAT is set
        {
          CAN_vGetMsgObj(HW_RECEIVE_MASTER, &sw_msgobj); // JP 18-03-2013
          if ((CAN_HWOBJ[HW_RECEIVE_MASTER].CTRL & 0x0c00) == 0x0800) // if MSGLST is set
          {
            // Indicates that the CAN controller has stored a new 
            // message into this object, while NEWDAT was still set,
            // ie. the previously stored message is lost.
            CAN_HWOBJ[HW_RECEIVE_MASTER].CTRL = 0xf7ff;  // reset MSGLST
            CAN_HWOBJ[HW_RECEIVE_MASTER].CTRL = 0x7dff;  // reset NEWDAT
            // USER CODE BEGIN (SRN0_OBJ0,1)
          }
          else
          {
            // The CAN controller has stored a new message into this object.
            // USER CODE BEGIN (SRN0_OBJ0,2)
            //fill sw msg obj
//            CAN_vGetMsgObj(HW_RECEIVE_MASTER, &sw_msgobj);
            //release hw msg obj 
            CAN_vReleaseObj(HW_RECEIVE_MASTER); // equivalent of resteting NEWDAT
            // CAN_HWOBJ[HW_RECEIVE_MASTER].CTRL = 0xfffd;  // reset INTPND
            // CAN_HWOBJ[HW_RECEIVE_MASTER].CTRL = 0xfdff;  // reset NEWDAT
            address_sender_in = CAN_SW_GetSenderAddress(&sw_msgobj);
            address_receiver_in = CAN_SW_GetReceiverAddress(&sw_msgobj);
            master_ptr = can_backbone_first_master_ptr;
            while (master_ptr != (s_can_backbone_master *)END_OF_CAN_LIST)
            {
              if ((master_ptr->address_receiver == address_sender_in) &&
                  (master_ptr->node_ptr->address_node == address_receiver_in))
              {
                //check command
                switch(CAN_SW_GetCommand_Fast(&sw_msgobj))
                //switch(CAN_SW_GetCommand(&sw_msgobj))
                {
                  case S2M_WRITE_ASK_FOR_NEXT_BLOCK_OF_DATA_FAST:
                    #if (SDO_MAX==8)
                    if (master_ptr->toggle_bit == 7)
                    {
                      CAN_SW_SetMessageType(&sw_msgobj, MESSAGE_TYPE_MASTER);
                      CAN_SW_SwapAddresses(&sw_msgobj);
                      do
                      {
                        if (master_ptr->message_size - master_ptr->buffer_index > 0)
                        {
                          master_ptr->toggle_bit++;
                          master_ptr->toggle_bit &= 0x07;
                          CAN_SW_Set_Count_Fast(&sw_msgobj, master_ptr->toggle_bit);
                          if (master_ptr->message_size - master_ptr->buffer_index <= 8)
                          {
                            message_size = master_ptr->message_size - master_ptr->buffer_index;
                            CAN_SW_SetCommand_Fast(&sw_msgobj, M2S_WRITE_LAST_BLOCK_OF_DATA_FAST);
                            for (j = 0; j < message_size; j++)
                            {
                              sw_msgobj.data[j] = master_ptr->buffer_ptr[master_ptr->buffer_index];
                              master_ptr->buffer_index++;
                            }
                            CAN_SW_SetNrOfBytes(&sw_msgobj, message_size);
                            PUSH(&sw_msgobj, &can_backbone_master_transmit_fifo);
                            break;
                          }
                          else
                          {
                            message_size = 8;
                            CAN_SW_SetCommand_Fast(&sw_msgobj, M2S_WRITE_NEXT_BLOCK_OF_DATA_FAST);
                            for (j = 0; j < message_size; j++)
                            {
                              sw_msgobj.data[j] = master_ptr->buffer_ptr[master_ptr->buffer_index];
                              master_ptr->buffer_index++;
                            }
                            CAN_SW_SetNrOfBytes(&sw_msgobj, message_size);
                            PUSH(&sw_msgobj, &can_backbone_master_transmit_fifo);
                          }
                        }
                        else
                        {
                          // no more data to send
                          break;
                        }  
                      }
                      while (master_ptr->toggle_bit < 7);
                      if (CAN_ubRequestMsgObj(HW_TRANSMIT_MASTER))
                      {
                        if ((can_backbone_master_transmit_fifo.pop_index != can_backbone_master_transmit_fifo.push_index) ||
                            (can_backbone_master_transmit_fifo.full == 1))
                        {
                          //access the hw msg obj (in this case, condition should always be true)
                          //get the next element from the s_fifo
                          sw_msgobj = POP(&can_backbone_master_transmit_fifo);
                          CAN_vConfigMsgObj(HW_TRANSMIT_MASTER, &sw_msgobj);
                          CAN_vTransmit(HW_TRANSMIT_MASTER);  
                        }
                      }  
                      //save last send msg
                      master_ptr->prev_msg = sw_msgobj;    
                      //RESET timeout and retrys                                        
                      master_ptr->send_reset = 1;                                      
                      
                    }
                    else
                    {
                      master_ptr->buffer_index = 0;
                      master_ptr->send = FALSE;
                      master_ptr->send_reset = 1;
                    }
                    #endif // (SDO_MAX==8)
                    break;
                  case S2M_WRITE_LAST_BLOCK_CORRECT_FAST:
                    if (CAN_SW_Get_Count_Fast(&sw_msgobj) == master_ptr->toggle_bit)
                    {
                      //call "transmit" function if any
                      master_ptr->TRANSMIT_OK++;
                      if (master_ptr->transmit_func != 0)
                      {
                        (master_ptr->transmit_func)(master_ptr, TRANSMIT_MASTER_OK);
                      }
                      //all these resets must be also done in the "transmit func"
                      //they are also here in case there is no "transmit func"
                      master_ptr->buffer_index = 0;
                      master_ptr->send = FALSE;
                      master_ptr->send_reset = 1;
                      //this master has ended transmission, but other masters on the same node may be during transmission
                      //try and access the hw msg obj
                      if (CAN_ubRequestMsgObj(HW_TRANSMIT_MASTER))
                      {
                        //check the send buffer if there is some other master-to-slave msg to transmit
                        if ((can_backbone_master_transmit_fifo.pop_index != can_backbone_master_transmit_fifo.push_index) ||
                            (can_backbone_master_transmit_fifo.full == 1))
                        {
                          //get the next element from the s_fifo
                          sw_msgobj = POP(&can_backbone_master_transmit_fifo);
                          CAN_vConfigMsgObj(HW_TRANSMIT_MASTER, &sw_msgobj);
                          CAN_vTransmit(HW_TRANSMIT_MASTER);  
                        }
                      }
                    }
                    else
                    {
                      master_ptr->buffer_index = 0;
                      master_ptr->send = FALSE;
                      master_ptr->send_reset = 0;
                    }
                    break;
                  case S2M_WRITE_ASK_FOR_NEXT_BLOCK_OF_DATA:
                    if (CAN_SW_GetToggleBit(&sw_msgobj) == master_ptr->toggle_bit) // JP 17-08-07
                    {
                      //swap toggle bit
                      master_ptr->toggle_bit = !master_ptr->toggle_bit;
                      //set msg type - SDO MASTER
                      CAN_SW_SetMessageType(&sw_msgobj, MESSAGE_TYPE_MASTER);
                      //set toggle bit
                      CAN_SW_SetToggleBit(&sw_msgobj, master_ptr->toggle_bit);
                      //swap receiver and sender addresses
                      CAN_SW_SwapAddresses(&sw_msgobj);
                      //load data bytes
                      if (master_ptr->message_size - master_ptr->buffer_index <= 8)
                      {
                        message_size = master_ptr->message_size - master_ptr->buffer_index;
                        CAN_SW_SetCommand(&sw_msgobj, M2S_WRITE_LAST_BLOCK_OF_DATA); 
                      }  
                      else
                      {
                        message_size = 8;
                        CAN_SW_SetCommand(&sw_msgobj, M2S_WRITE_NEXT_BLOCK_OF_DATA);
                      }  
                      for (j = 0; j < message_size; j++) 
                      {
                        sw_msgobj.data[j] = master_ptr->buffer_ptr[master_ptr->buffer_index];
                        master_ptr->buffer_index++;
                      }
                      //set number of bytes with data in DLC 
                      CAN_SW_SetNrOfBytes(&sw_msgobj, message_size);
                      //put message on the SDO send s_fifo
                      PUSH(&sw_msgobj, &can_backbone_master_transmit_fifo);
                      if (CAN_ubRequestMsgObj(HW_TRANSMIT_MASTER))
                      {
                        if ((can_backbone_master_transmit_fifo.pop_index != can_backbone_master_transmit_fifo.push_index) ||
                            (can_backbone_master_transmit_fifo.full == 1))
                        {
                          //access the hw msg obj (in this case, condition should always be true)
                          //get the next element from the s_fifo
                          sw_msgobj = POP(&can_backbone_master_transmit_fifo);
                          CAN_vConfigMsgObj(HW_TRANSMIT_MASTER, &sw_msgobj);
                          CAN_vTransmit(HW_TRANSMIT_MASTER);  
                        }
                      }  
                      //save last send msg
                      master_ptr->prev_msg = sw_msgobj;    
                      //RESET timeout and retrys  
                      master_ptr->send_reset = 1;                                      
                    }        
                    else
                    {
                      master_ptr->buffer_index = 0;
                      master_ptr->send = FALSE;
                      master_ptr->send_reset = 1;
                    }
                    break;
                  case S2M_WRITE_LAST_BLOCK_CORRECT:
                    if (CAN_SW_GetToggleBit(&sw_msgobj) == master_ptr->toggle_bit) // JP 17-08-07
                    {
                      //call "transmit" function if any
                      master_ptr->TRANSMIT_OK++;
                      if (master_ptr->transmit_func != 0)
                      {
                        (master_ptr->transmit_func)(master_ptr, TRANSMIT_MASTER_OK);
                      }
                      //all these resets must be also done in the "transmit func"
                      //they are also here in case there is no "transmit func"
                      master_ptr->buffer_index = 0;
                      master_ptr->send = FALSE;
                      master_ptr->send_reset = 1;
                      //this master has ended transmission, but other masters on the same node may be during transmission
                      //try and access the hw msg obj
                      if (CAN_ubRequestMsgObj(HW_TRANSMIT_MASTER))
                      {
                        //check the send buffer if there is some other master-to-slave msg to transmit
                        if ((can_backbone_master_transmit_fifo.pop_index != can_backbone_master_transmit_fifo.push_index) ||
                            (can_backbone_master_transmit_fifo.full == 1))
                        {
                          //get the next element from the s_fifo
                          sw_msgobj = POP(&can_backbone_master_transmit_fifo);
                          CAN_vConfigMsgObj(HW_TRANSMIT_MASTER, &sw_msgobj);
                          CAN_vTransmit(HW_TRANSMIT_MASTER);  
                        }
                      }
                    }
                    else
                    {
                      master_ptr->buffer_index = 0;
                      master_ptr->send = FALSE;
                      master_ptr->send_reset = 1;
                    }
                    break;
                }
                //since the SDO works peer-to-peer, there cannot be any other element in
                // "first_master_ptr" that has to respond to the slave incomed messege
                break; //break the "for" loop            
              }//end of if (address check)
              master_ptr = master_ptr->next_ptr;
            }//end of while
          }// end else (MSGLST)
        }// end if NEWDAT
}
        break;
      case 3:
        //************************************************
        // message object 0 interrupt (TRANSMIT MASTER)
if ((CAN_HWOBJ[HW_TRANSMIT_MASTER].CTRL & 0x0003) == 0x0002) // if INTPND
{
        CAN_HWOBJ[HW_TRANSMIT_MASTER].CTRL = 0xfdff; // reset NEWDAT
        CAN_HWOBJ[HW_TRANSMIT_MASTER].CTRL = 0xfffd; // reset INTPND
        // The transmission of the last message object was successful.
        // so now send the next ready to go block in the master_fifo
        //check if there is something else that needs to be sent on the s_fifo
        if (CAN_ubRequestMsgObj(HW_TRANSMIT_MASTER))
        {
          if ((can_backbone_master_transmit_fifo.pop_index != can_backbone_master_transmit_fifo.push_index) ||
              (can_backbone_master_transmit_fifo.full == 1))
          {
            //get the next element from the s_fifo
            sw_msgobj = POP(&can_backbone_master_transmit_fifo);
            CAN_vConfigMsgObj(HW_TRANSMIT_MASTER, &sw_msgobj);
            CAN_vTransmit(HW_TRANSMIT_MASTER);  
          }
        }
}
        break;
      case 4:
        //*********************************************
        // message object 2 interrupt (RECEIVE SLAVE)
if ((CAN_HWOBJ[HW_RECEIVE_SLAVE].CTRL & 0x0003) == 0x0002) // if INTPND
{
        CAN_HWOBJ[HW_RECEIVE_SLAVE].CTRL = 0xfffd;  // reset INTPND
        if ((CAN_HWOBJ[HW_RECEIVE_SLAVE].CTRL & 0x0300) == 0x0200)  // NEWDAT is set
        {
          CAN_vGetMsgObj(HW_RECEIVE_SLAVE, &sw_msgobj); // JP 18-03-2013
          if ((CAN_HWOBJ[HW_RECEIVE_SLAVE].CTRL & 0x0c00) == 0x0800)  // if MSGLST is set
          {
            // Indicates that the CAN controller has stored a new 
            // message into this object, while NEWDAT was still set,
            // ie. the previously stored message is lost.
            CAN_HWOBJ[HW_RECEIVE_SLAVE].CTRL = 0xf7ff;  // reset MSGLST
            CAN_HWOBJ[HW_RECEIVE_SLAVE].CTRL = 0x7dff;  // reset NEWDAT
          }
          else
          {
            // The CAN controller has stored a new message into this object.
            //fill sw msg obj
//            CAN_vGetMsgObj(HW_RECEIVE_SLAVE, &sw_msgobj);
            //release hw msg obj 
            CAN_vReleaseObj(HW_RECEIVE_SLAVE);
            address_sender_in = CAN_SW_GetSenderAddress(&sw_msgobj);
            address_receiver_in = CAN_SW_GetReceiverAddress(&sw_msgobj);
            slave_ptr = can_backbone_first_slave_ptr;
            while (slave_ptr != (s_can_backbone_slave *)END_OF_CAN_LIST)
            {
              //check if addresses match
              //the address the slave knows as being its own must match the Receiver field from the ID
              //the address the slave knows as being for its master must match the Sender from the ID
              if ((slave_ptr->node_ptr->address_node == address_receiver_in) &&
                  (slave_ptr->address_sender == address_sender_in))
              {
                //check command
                message_size = CAN_SW_GetNrOfBytes(&sw_msgobj);
                switch(CAN_SW_GetCommand_Fast(&sw_msgobj))
                //switch(CAN_SW_GetCommand(&sw_msgobj))
                {
                  case M2S_WRITE_START_MAX_8_BYTES_OF_DATA:
                    if ((CAN_SW_GetToggleBit(&sw_msgobj) == SET) &&
                        (message_size <= slave_ptr->max_size)) // JP 17-08-07
                    {
                      //master send the slave a frame of maximum 8 bytes
                      //no check for toggle bit or status is necessary, 
                      // this way each new start of a transmission will cause the slave to reset
                      slave_ptr->message_size = 0;
                      //readout the bytes and put them in the buffer
                      for (j = 0; j < message_size; j++)
                      {
                        slave_ptr->buffer_ptr[slave_ptr->message_size] = sw_msgobj.data[j];
                        slave_ptr->message_size++;
                      }
                      //answer the master with "S2M_WRITE_LAST_BLOCK_CORRECT"
                      //first, configure sw message object
                      CAN_SW_SwapAddresses(&sw_msgobj);
                      CAN_SW_SetMessageType(&sw_msgobj, MESSAGE_TYPE_SLAVE);
                      CAN_SW_SetCommand(&sw_msgobj, S2M_WRITE_LAST_BLOCK_CORRECT);
                      CAN_SW_SetNrOfBytes(&sw_msgobj, 0);
                      //check if the hw transmit msg obj is available
                      PUSH(&sw_msgobj, &can_backbone_slave_transmit_fifo);
                      if (CAN_ubRequestMsgObj(HW_TRANSMIT_SLAVE))
                      {
                        if ((can_backbone_slave_transmit_fifo.pop_index != can_backbone_slave_transmit_fifo.push_index) ||
                            (can_backbone_slave_transmit_fifo.full == 1))
                        {
                          //get the next element from the s_fifo
                          sw_msgobj = POP(&can_backbone_slave_transmit_fifo);
                          CAN_vConfigMsgObj(HW_TRANSMIT_SLAVE, &sw_msgobj);
                          CAN_vTransmit(HW_TRANSMIT_SLAVE);
                        }  
                      }
                      //transmission has ended, the buffer contains valid data - call return function
                      slave_ptr->RECEIVE_OK++;
                      if (slave_ptr->receive_func != 0)
                        (slave_ptr->receive_func)(slave_ptr, RECEIVE_SLAVE_OK);                                               
                    }             
                    slave_ptr->toggle_bit = 0xFF; // JP 11-06-08
                    break;
                  case M2S_WRITE_START_MORE_THEN_8_BYTES_OF_DATA:
                    if ((CAN_SW_GetToggleBit(&sw_msgobj) == SET) &&
                        (message_size <= slave_ptr->max_size)) // JP 17-08-07
                    {
                      //master send the slave first 8 bytes of a larger SDO
                      //no check for toggle bit or status is necessary, 
                      // this way each new start of a transmission will cause the slave to reset
                      slave_ptr->message_size = 0;
                      //readout the bytes and put them in the buffer
                      for (j = 0; j < message_size; j++)
                      {
                        slave_ptr->buffer_ptr[slave_ptr->message_size] = sw_msgobj.data[j];
                        slave_ptr->message_size++;
                      }
                      //answer the master with "S2M_WRITE_ASK_FOR_NEXT_BLOCK_OF_DATA"
                      //first, configure sw message object
                      CAN_SW_SwapAddresses(&sw_msgobj);
                      CAN_SW_SetMessageType(&sw_msgobj, MESSAGE_TYPE_SLAVE);
                      CAN_SW_SetCommand(&sw_msgobj, S2M_WRITE_ASK_FOR_NEXT_BLOCK_OF_DATA);
                      CAN_SW_SetNrOfBytes(&sw_msgobj, 0);
                      PUSH(&sw_msgobj, &can_backbone_slave_transmit_fifo);
                      if (CAN_ubRequestMsgObj(HW_TRANSMIT_SLAVE))
                      {
                        if ((can_backbone_slave_transmit_fifo.pop_index != can_backbone_slave_transmit_fifo.push_index) ||
                            (can_backbone_slave_transmit_fifo.full == 1))
                        {
                          //get the next element from the s_fifo
                          sw_msgobj = POP(&can_backbone_slave_transmit_fifo);
                          CAN_vConfigMsgObj(HW_TRANSMIT_SLAVE, &sw_msgobj);
                          CAN_vTransmit(HW_TRANSMIT_SLAVE);
                        }  
                      }
                      //set toggle bit
                      slave_ptr->toggle_bit = SET;
                    }
                    else
                    {
                      slave_ptr->toggle_bit = 0xFF; // JP 11-06-08
                    }  
                    break;
                  case M2S_WRITE_NEXT_BLOCK_OF_DATA:
                    //check toggle bit
                    //if the toggle bit is the same as in the previous message
                    // the master is sending the previous frame again
                    // in this case, data from current message should not be stored
                    if (slave_ptr->toggle_bit == 0xFF) // error by previous message wait for restart complete command or message complete  JP 11-06-08
                      break;
                    if ((CAN_SW_GetToggleBit(&sw_msgobj) != slave_ptr->toggle_bit) &&
                        (message_size <= slave_ptr->max_size - slave_ptr->message_size)) // JP 17-08-07
                    {
                      //toggle bit ok, store data and update toggle bit
                      slave_ptr->toggle_bit = !slave_ptr->toggle_bit; 
                      for (j = 0; j < message_size; j++)
                      {
                        slave_ptr->buffer_ptr[slave_ptr->message_size] = sw_msgobj.data[j];
                        slave_ptr->message_size++;
                      }
                      //answer the master with "S2M_WRITE_ASK_FOR_NEXT_BLOCK_OF_DATA"
                      //first, configure sw message object
                      CAN_SW_SwapAddresses(&sw_msgobj);
                      CAN_SW_SetMessageType(&sw_msgobj, MESSAGE_TYPE_SLAVE);
                      CAN_SW_SetCommand(&sw_msgobj, S2M_WRITE_ASK_FOR_NEXT_BLOCK_OF_DATA);
                      CAN_SW_SetNrOfBytes(&sw_msgobj, 0);
                      //if the hw msg obj is available
                      PUSH(&sw_msgobj, &can_backbone_slave_transmit_fifo);
                      if (CAN_ubRequestMsgObj(HW_TRANSMIT_SLAVE))
                      {
                        if ((can_backbone_slave_transmit_fifo.pop_index != can_backbone_slave_transmit_fifo.push_index) ||
                            (can_backbone_slave_transmit_fifo.full == 1))
                        {
                          //get the next element from the s_fifo
                          sw_msgobj = POP(&can_backbone_slave_transmit_fifo);
                          CAN_vConfigMsgObj(HW_TRANSMIT_SLAVE, &sw_msgobj);
                          CAN_vTransmit(HW_TRANSMIT_SLAVE);
                        }  
                      }
                    }
                    else
                    {
                      slave_ptr->toggle_bit = 0xFF; // JP 11-06-08
                    } 
                    break;                    
                  case M2S_WRITE_LAST_BLOCK_OF_DATA:
                    //NO NEED TO CHECK THE TOGGLE BIT
                    //readout the bytes and put them in the buffer
                    if (slave_ptr->toggle_bit == 0xFF) // error by previous message wait for restart complete command or message complete JP 11-06-08
                      break;
                    if ((CAN_SW_GetToggleBit(&sw_msgobj) != slave_ptr->toggle_bit) &&
                        (message_size <= slave_ptr->max_size - slave_ptr->message_size)) // JP 17-08-07
                    {
                      for (j = 0; j < message_size; j++)
                      {
                        slave_ptr->buffer_ptr[slave_ptr->message_size] = sw_msgobj.data[j];
                        slave_ptr->message_size++;
                      }
                      // JP 22-11-2006 what to doo if toggle bit not correct
                      //answer the master with "S2M_WRITE_LAST_BLOCK_CORRECT"
                      //first, configure sw message object
                      CAN_SW_SwapAddresses(&sw_msgobj);
                      CAN_SW_SetMessageType(&sw_msgobj, MESSAGE_TYPE_SLAVE);
                      CAN_SW_SetCommand(&sw_msgobj, S2M_WRITE_LAST_BLOCK_CORRECT);
                      CAN_SW_SetNrOfBytes(&sw_msgobj, 0);
                      PUSH(&sw_msgobj, &can_backbone_slave_transmit_fifo);
                      if (CAN_ubRequestMsgObj(HW_TRANSMIT_SLAVE))
                      {
                        if ((can_backbone_slave_transmit_fifo.pop_index != can_backbone_slave_transmit_fifo.push_index) ||
                            (can_backbone_slave_transmit_fifo.full == 1))
                        {
                          //get the next element from the s_fifo
                          sw_msgobj = POP(&can_backbone_slave_transmit_fifo);
                          CAN_vConfigMsgObj(HW_TRANSMIT_SLAVE, &sw_msgobj);
                          CAN_vTransmit(HW_TRANSMIT_SLAVE);
                        }  
                      }
                      //transmission has ended, the buffer contains valid data - call return function
                      slave_ptr->RECEIVE_OK++;
                      if (slave_ptr->receive_func != 0)
                        (slave_ptr->receive_func)(slave_ptr, RECEIVE_SLAVE_OK);
                    }  
                    slave_ptr->toggle_bit = 0xFF; // JP 11-06-08
                    break;
                  case M2S_WRITE_START_MAX_8_BYTES_OF_DATA_FAST:
                    if ((CAN_SW_Get_Count_Fast(&sw_msgobj) == 0) &&
                        (message_size <= slave_ptr->max_size)) // JP 17-08-07
                    {
                      //master send the slave a frame of maximum 8 bytes
                      //no check for toggle bit or status is necessary, 
                      // this way each new start of a transmission will cause the slave to reset
                      slave_ptr->message_size = 0;
                      slave_ptr->toggle_bit = 0; // JP 12-11-07
                      //readout the bytes and put them in the buffer
                      for (j = 0; j < message_size; j++)
                      {
                        slave_ptr->buffer_ptr[slave_ptr->message_size] = sw_msgobj.data[j];
                        slave_ptr->message_size++;
                      }
                      //answer the master with "S2M_WRITE_LAST_BLOCK_CORRECT"
                      //first, configure sw message object
                      CAN_SW_SwapAddresses(&sw_msgobj);
                      CAN_SW_SetMessageType(&sw_msgobj, MESSAGE_TYPE_SLAVE);
                      CAN_SW_SetCommand_Fast(&sw_msgobj, S2M_WRITE_LAST_BLOCK_CORRECT_FAST);
                      CAN_SW_SetNrOfBytes(&sw_msgobj, 0);
                      //check if the hw transmit msg obj is available
                      PUSH(&sw_msgobj, &can_backbone_slave_transmit_fifo);
                      if (CAN_ubRequestMsgObj(HW_TRANSMIT_SLAVE))
                      {
                        if ((can_backbone_slave_transmit_fifo.pop_index != can_backbone_slave_transmit_fifo.push_index) ||
                            (can_backbone_slave_transmit_fifo.full == 1))
                        {
                          //get the next element from the s_fifo
                          sw_msgobj = POP(&can_backbone_slave_transmit_fifo);
                          CAN_vConfigMsgObj(HW_TRANSMIT_SLAVE, &sw_msgobj);
                          CAN_vTransmit(HW_TRANSMIT_SLAVE);
                        }  
                      }
                      //transmission has ended, the buffer contains valid data - call return function
                      slave_ptr->RECEIVE_OK++;
                      if (slave_ptr->receive_func != 0)
                        (slave_ptr->receive_func)(slave_ptr, RECEIVE_SLAVE_OK);                                               
                    }             
                    slave_ptr->toggle_bit = 0xFF; // JP 11-06-08
                    break;
                  case M2S_WRITE_START_MORE_THEN_8_BYTES_OF_DATA_FAST:
                    if ((CAN_SW_Get_Count_Fast(&sw_msgobj) == 0) &&
                        (message_size <= slave_ptr->max_size)) // JP 17-08-07
                    {
                      //master send the slave first 8 bytes of a larger SDO
                      //no check for toggle bit or status is necessary, 
                      // this way each new start of a transmission will cause the slave to reset
                      slave_ptr->message_size = 0;
                      slave_ptr->toggle_bit = 0; // JP 12-11-07
                      //readout the bytes and put them in the buffer
                      for (j = 0; j < message_size; j++)
                      {
                        slave_ptr->buffer_ptr[slave_ptr->message_size] = sw_msgobj.data[j];
                        slave_ptr->message_size++;
                      }
                    }  
                    else
                    {
                      slave_ptr->toggle_bit = 0xFF; // JP 11-06-08
                    }  
                    break;
                  case M2S_WRITE_NEXT_BLOCK_OF_DATA_FAST:
                    //check toggle bit
                    //if the toggle bit is the same as in the previous message
                    // the master is sending the previous frame again
                    // in this case, data from current message should not be stored
                    if (slave_ptr->toggle_bit == 0xFF) // error by previous message wiat for restart complete command or message complete
                      break;
                    if ((CAN_SW_Get_Count_Fast(&sw_msgobj) == ((slave_ptr->toggle_bit + 1) & 0x07)) && 
                        (message_size <= slave_ptr->max_size - slave_ptr->message_size)) // JP 17-08-07
                    {
                      //toggle bit ok, store data and update toggle bit
                      slave_ptr->toggle_bit = CAN_SW_Get_Count_Fast(&sw_msgobj);
                      for (j = 0; j < message_size; j++)
                      {
                        slave_ptr->buffer_ptr[slave_ptr->message_size] = sw_msgobj.data[j];
                        slave_ptr->message_size++;
                      }
#if (SDO_MAX==8)
                      if (slave_ptr->toggle_bit == 7)
                      {
                        // 8 messages are received ask for next messages
                        CAN_SW_SwapAddresses(&sw_msgobj);
                        CAN_SW_SetMessageType(&sw_msgobj, MESSAGE_TYPE_SLAVE);
                        CAN_SW_SetCommand_Fast(&sw_msgobj, S2M_WRITE_ASK_FOR_NEXT_BLOCK_OF_DATA_FAST);
                        CAN_SW_SetNrOfBytes(&sw_msgobj, 0);
                        PUSH(&sw_msgobj, &can_backbone_slave_transmit_fifo);
                        if (CAN_ubRequestMsgObj(HW_TRANSMIT_SLAVE))
                        {
                          if ((can_backbone_slave_transmit_fifo.pop_index != can_backbone_slave_transmit_fifo.push_index) ||
                              (can_backbone_slave_transmit_fifo.full == 1))
                          {
                            //get the next element from the s_fifo
                            sw_msgobj = POP(&can_backbone_slave_transmit_fifo);
                            CAN_vConfigMsgObj(HW_TRANSMIT_SLAVE, &sw_msgobj);
                            CAN_vTransmit(HW_TRANSMIT_SLAVE);
                          }  
                        }
                      }
#endif // (SDO_MAX==8)
                    }
                    else
                      slave_ptr->toggle_bit = 0xFF;
                    break;
                  case M2S_WRITE_LAST_BLOCK_OF_DATA_FAST:
                    //NO NEED TO CHECK THE TOGGLE BIT
                    //readout the bytes and put them in the buffer
                    if (slave_ptr->toggle_bit == 0xFF) // error by previous message wiat for restart complete command or message complete
                      break;
                    if ((CAN_SW_Get_Count_Fast(&sw_msgobj) == ((slave_ptr->toggle_bit + 1) & 0x07)) && //(CAN_SW_GetToggleBit(&sw_msgobj) != slave_ptr->toggle_bit) &&
                        (message_size <= slave_ptr->max_size - slave_ptr->message_size)) // JP 17-08-07
                    {
                      for (j = 0; j < message_size; j++)
                      {
                        slave_ptr->buffer_ptr[slave_ptr->message_size] = sw_msgobj.data[j];
                        slave_ptr->message_size++;
                      }
                      // JP 22-11-2006 what to doo if toggle bit not correct
                      //answer the master with "S2M_WRITE_LAST_BLOCK_CORRECT"
                      //first, configure sw message object
                      CAN_SW_SwapAddresses(&sw_msgobj);
                      CAN_SW_SetMessageType(&sw_msgobj, MESSAGE_TYPE_SLAVE);
                      CAN_SW_SetCommand_Fast(&sw_msgobj, S2M_WRITE_LAST_BLOCK_CORRECT_FAST);
                      CAN_SW_SetNrOfBytes(&sw_msgobj, 0);
                      PUSH(&sw_msgobj, &can_backbone_slave_transmit_fifo);
                      if (CAN_ubRequestMsgObj(HW_TRANSMIT_SLAVE))
                      {
                        if ((can_backbone_slave_transmit_fifo.pop_index != can_backbone_slave_transmit_fifo.push_index) ||
                            (can_backbone_slave_transmit_fifo.full == 1))
                        {
                          //get the next element from the s_fifo
                          sw_msgobj = POP(&can_backbone_slave_transmit_fifo);
                          CAN_vConfigMsgObj(HW_TRANSMIT_SLAVE, &sw_msgobj);
                          CAN_vTransmit(HW_TRANSMIT_SLAVE);
                        }  
                      }
                      //transmission has ended, the buffer contains valid data - call return function
                      slave_ptr->RECEIVE_OK++;
                      if (slave_ptr->receive_func != 0)
                        (slave_ptr->receive_func)(slave_ptr, RECEIVE_SLAVE_OK);
                    }  
                    slave_ptr->toggle_bit = 0xFF;
                    break;
                }//end of "switch( CAN_SW_GetCommand(&sw_msgobj))"
                //since the SDO works peer-to-peer, there cannot be any other element in
                // "first_slave_ptr" that has to respond to the master's incomed messege
                break; //break "for" loop
              }//end of if (address check)
              slave_ptr = slave_ptr->next_ptr;
            }//end of while
          }//end else (MSGLST)
        }  // End of RXIPND2
}
        break;
      case 5:
        //**********************************************
        // message object 1 interrupt (TRANSMIT SLAVE)
        // The transmission of the last message object was successful.
if ((CAN_HWOBJ[HW_TRANSMIT_SLAVE].CTRL & 0x0003) == 0x0002) // if INTPND
{
        CAN_HWOBJ[HW_TRANSMIT_SLAVE].CTRL = 0xfdff;      // reset NEWDAT
        CAN_HWOBJ[HW_TRANSMIT_SLAVE].CTRL = 0xfffd;   // reset INTPND
        //check if there is something else that needs to be sent on the s_fifo
//        if (can_backbone_slave_transmit_fifo.pop_index != can_backbone_slave_transmit_fifo.push_index)
        //access the hw msg obj (in this case, condition should always be true)
        if (CAN_ubRequestMsgObj(HW_TRANSMIT_SLAVE))
        {
          if ((can_backbone_slave_transmit_fifo.pop_index != can_backbone_slave_transmit_fifo.push_index) ||
              (can_backbone_slave_transmit_fifo.full == 1))
          {
            //get the next element from the s_fifo
            sw_msgobj = POP(&can_backbone_slave_transmit_fifo);
            CAN_vConfigMsgObj(HW_TRANSMIT_SLAVE, &sw_msgobj);
            CAN_vTransmit(HW_TRANSMIT_SLAVE); 
          }
        }
}
        break;
      case 6:
        //*******************************************
        // message object 4 interrupt (RECEIVE VALUE)
if ((CAN_HWOBJ[HW_RECEIVE_VALUE].CTRL & 0x0003) == 0x0002) // if INTPND
{
        CAN_HWOBJ[HW_RECEIVE_VALUE].CTRL = 0xfffd;  // reset INTPND
        if ((CAN_HWOBJ[HW_RECEIVE_VALUE].CTRL & 0x0300) == 0x0200)  // if NEWDAT is set
        {
          CAN_vGetMsgObj(HW_RECEIVE_VALUE, &sw_msgobj); // JP 18-03-2013
          if ((CAN_HWOBJ[HW_RECEIVE_VALUE].CTRL & 0x0c00) == 0x0800)  // if MSGLST is set
          {
            // Indicates that the CAN controller has stored a new 
            // message into this object, while NEWDAT was still set,
            // ie. the previously stored message is lost.
            CAN_HWOBJ[HW_RECEIVE_VALUE].CTRL = 0xf7ff;  // reset MSGLST
            CAN_HWOBJ[HW_RECEIVE_VALUE].CTRL = 0x7dff;  // reset NEWDAT
          }
          else
          {
            // The CAN controller has stored a new message into this object.
            // USER CODE BEGIN (SRN0_OBJ4,2)
            //fill sw msg obj
//            CAN_vGetMsgObj(HW_RECEIVE_VALUE, &sw_msgobj);
            //release hw msg obj 
            CAN_vReleaseObj(HW_RECEIVE_VALUE);
            // CAN_HWOBJ[4].CTRL = 0xfdff;  // reset NEWDAT
            // CAN_HWOBJ[4].CTRL = 0xfffd;  // reset INTPND
            address_receiver_in = CAN_SW_GetReceiverAddress(&sw_msgobj);
            address_sender_in = CAN_SW_GetSenderAddress(&sw_msgobj);
            switch (CAN_SW_GetCommand(&sw_msgobj))
            {
              default:
              case NO_COMMAND:
                receive_value_ptr = can_backbone_first_receive_value_ptr;
                while (receive_value_ptr != (s_can_backbone_receive_value *)END_OF_CAN_LIST)
                {
                  if (receive_value_ptr->command == NO_COMMAND)
                  {
                    if ((receive_value_ptr->address_sender == BROADCAST) &&
                        (address_receiver_in == BROADCAST))
                    {
                      receive_value_ptr->time_reset = 1;
                      receive_value_ptr->address_sender_in = address_sender_in;
                      message_size = receive_value_ptr->message_size = CAN_SW_GetNrOfBytes(&sw_msgobj);
                      //readout the bytes and put them in the buffer
                      for (j = 0; j < message_size; j++)
                      {
                        receive_value_ptr->buffer[j] = sw_msgobj.data[j];
                      }
          
                      //call "receive function" if there is any
                      if (receive_value_ptr->receive_func != 0)
                      {
                        (receive_value_ptr->receive_func)(receive_value_ptr, RECEIVE_VALUE_OK);
                      }
                    }//end if (index check)
                    else if ((receive_value_ptr->address_sender == address_sender_in) &&
                             (receive_value_ptr->node_ptr->address_node == address_receiver_in))
                    {
                      receive_value_ptr->time_reset = 1;
                      receive_value_ptr->address_sender_in = address_sender_in;
                      message_size = receive_value_ptr->message_size = CAN_SW_GetNrOfBytes(&sw_msgobj);
                      //readout the bytes and put them in the buffer
                      for (j = 0; j < message_size; j++)
                      {
                        receive_value_ptr->buffer[j] = sw_msgobj.data[j];
                      }
            
                      //call "receive function" if there is any
                      if (receive_value_ptr->receive_func != 0)
                      {
                        (receive_value_ptr->receive_func)(receive_value_ptr, RECEIVE_VALUE_OK);
                      }
                    }         
                  }  
                  receive_value_ptr = receive_value_ptr->next_ptr;
                } //end while
                break;
              case REGISTER_COMMAND:
                receive_value_ptr = can_backbone_first_receive_value_ptr;
                while (receive_value_ptr != (s_can_backbone_receive_value *)END_OF_CAN_LIST)
                {
                  if (receive_value_ptr->command == REGISTER_COMMAND)
                  {
                    receive_value_ptr->time_reset = 1;
                    receive_value_ptr->address_sender_in = address_sender_in;
                    receive_value_ptr->buffer[0] = ((unsigned char *)&address_sender_in)[0];
                    receive_value_ptr->buffer[1] = ((unsigned char *)&address_sender_in)[1];
                  
                    //call "receive function" if there is any
                    if (receive_value_ptr->receive_func != 0)
                    {
                      (receive_value_ptr->receive_func)(receive_value_ptr, RECEIVE_VALUE_OK);
                    }
                  }//end if (index check)
                  receive_value_ptr = receive_value_ptr->next_ptr;
                } //end while
                break;
            }
          }// end else (MSGLST)
        }  // End of RXIPND4
}
        break;
      case 7:
        //**********************************************
        // message object 5 interrupt (TRANSMIT VALUE)
        // The transmission of the last message object was successful.
if ((CAN_HWOBJ[HW_TRANSMIT_VALUE].CTRL & 0x0003) == 0x0002) // if INTPND
{
        CAN_HWOBJ[HW_TRANSMIT_VALUE].CTRL = 0xfdff;  // reset NEWDAT
        CAN_HWOBJ[HW_TRANSMIT_VALUE].CTRL = 0xfffd;  // reset INTPND
        //call "transmit func" for the last value, reset the "send" flag
        can_backbone_last_transmit_value_ptr->TRANSMIT_OK++;
        if ((can_backbone_last_transmit_value_ptr->transmit_func != 0) &&
            (can_backbone_last_transmit_value_ptr->send == TRUE))
        {
          (can_backbone_last_transmit_value_ptr->transmit_func)(can_backbone_last_transmit_value_ptr, TRANSMIT_VALUE_OK);
        }
        can_backbone_last_transmit_value_ptr->time_reset = 1;
        //reset the "send" flag
        can_backbone_last_transmit_value_ptr->send = FALSE;
        can_backbone_last_transmit_value_ptr->send_reset = 1;
        //check if there is anything else to send
        transmit_value_ptr = can_backbone_first_transmit_value_ptr; 
        while (transmit_value_ptr != (s_can_backbone_transmit_value *)END_OF_CAN_LIST)    
        {
          if (transmit_value_ptr->send == TRUE)
          {
            //send (in this case the condition should always be true) 
            if (CAN_ubRequestMsgObj(HW_TRANSMIT_VALUE))
            {
              //setup sw_msgobj
              sw_msgobj = SetupTransmitValueMsgObj(transmit_value_ptr);
              //remember the last value sent
              can_backbone_last_transmit_value_ptr = transmit_value_ptr;
              CAN_vConfigMsgObj(HW_TRANSMIT_VALUE, &sw_msgobj);
              CAN_vTransmit(HW_TRANSMIT_VALUE); 
            }
            break; //break "for" loop
          }
          transmit_value_ptr = transmit_value_ptr->next_ptr;
        }
}
        break;
      case 8:
        //**********************************************
        // message object 6 interrupt (RECEIVE ALARM)
if ((CAN_HWOBJ[HW_RECEIVE_ALARM].CTRL & 0x0003) == 0x0002) // if INTPND
{
        CAN_HWOBJ[HW_RECEIVE_ALARM].CTRL = 0xfffd;  // reset INTPND
        if ((CAN_HWOBJ[HW_RECEIVE_ALARM].CTRL & 0x0300) == 0x0200)    // if NEWDAT is set
        {
          CAN_vGetMsgObj(HW_RECEIVE_ALARM, &sw_msgobj); // JP 18-03-2013
          if ((CAN_HWOBJ[HW_RECEIVE_ALARM].CTRL & 0x0c00) == 0x0800)  // if MSGLST is set
          {
            // Indicates that the CAN controller has stored a new 
            // message into this object, while NEWDAT was still set,
            // ie. the previously stored message is lost.
            CAN_HWOBJ[HW_RECEIVE_ALARM].CTRL = 0xf7ff;  // reset MSGLST
            CAN_HWOBJ[HW_RECEIVE_ALARM].CTRL = 0x7dff;  // reset NEWDAT
          }
          else
          {
            // The CAN controller has stored a new message into this object.
            //fill sw msg obj
//            CAN_vGetMsgObj(HW_RECEIVE_ALARM, &sw_msgobj);
            //release hw msg obj 
            CAN_vReleaseObj(HW_RECEIVE_ALARM);
            address_receiver_in = CAN_SW_GetReceiverAddress(&sw_msgobj);
            address_sender_in = CAN_SW_GetSenderAddress(&sw_msgobj);
            receive_alarm_ptr = can_backbone_first_receive_alarm_ptr;
            while (receive_alarm_ptr != (s_can_backbone_receive_alarm *)END_OF_CAN_LIST)
            {
              // do not accept youre own message
              if (receive_alarm_ptr->node_ptr->address_node != address_sender_in)
              {
                //if "address_receiver" field is 0 or "address_receiver" and "address_sender"
                //match the sender and receiver from the ID of the incoming message, then               
                //store values for the incoming message
                message_size = receive_alarm_ptr->message_size = CAN_SW_GetNrOfBytes(&sw_msgobj);
                //readout the bytes and put them in the buffer
                for (j = 0; j < message_size; j++)
                {
                  receive_alarm_ptr->buffer[j] = sw_msgobj.data[j];
                }
                receive_alarm_ptr->RECEIVE_OK++;
                receive_alarm_ptr->time_reset = 1;
                receive_alarm_ptr->address_sender_in = address_sender_in;
                //at last call "receiver function" if any
                if (receive_alarm_ptr->receive_func != 0)
                {
                  (receive_alarm_ptr->receive_func)(receive_alarm_ptr, RECEIVE_ALARM_OK);
                }
              }
              receive_alarm_ptr = receive_alarm_ptr->next_ptr;
            }// end while
            // don't forget to reset time out counter from the sdo slave of this node!
            //find the sdo slave that holds the watchdog timer and reset the timer and the flag
            slave_ptr = can_backbone_first_slave_ptr;
            while (slave_ptr != (s_can_backbone_slave *)END_OF_CAN_LIST)
            {
              //slave address must match address sender
              if (slave_ptr->address_sender == address_sender_in)
              {
                slave_ptr->time_reset = 1; 
                slave_ptr->watchdog_flag = SET;
              }
              slave_ptr = slave_ptr->next_ptr;
            }
          }// end else (MSGLST)
        }  // End of RXIPND6
}
        break;
      case 9:
        //**********************************************
        // message object 7 interrupt (TRANSMIT ALARM)
        // The transmission of the last message object was successful.
if ((CAN_HWOBJ[HW_TRANSMIT_ALARM].CTRL & 0x0003) == 0x0002) // if INTPND
{
        CAN_HWOBJ[HW_TRANSMIT_ALARM].CTRL = 0xfdff;      // reset NEWDAT
        CAN_HWOBJ[HW_TRANSMIT_ALARM].CTRL = 0xfffd;        // reset INTPND
        //call "transmit func" for the last alarm reset the "send" flag
        // only call the function if send flag still pending!!
        if ((can_backbone_last_transmit_alarm_ptr->transmit_func != 0) &&
            (can_backbone_last_transmit_alarm_ptr->send==TRUE)) 
        {
          (can_backbone_last_transmit_alarm_ptr->transmit_func)(can_backbone_last_transmit_alarm_ptr, TRANSMIT_ALARM_OK);
        }
        can_backbone_last_transmit_alarm_ptr->TRANSMIT_OK++;
        //reset the "send" flag
        can_backbone_last_transmit_alarm_ptr->time_reset = 1;
        can_backbone_last_transmit_alarm_ptr->send = FALSE;
        can_backbone_last_transmit_alarm_ptr->send_reset = 1;
        //check if there is anything else to send
        transmit_alarm_ptr = can_backbone_first_transmit_alarm_ptr;
        while (transmit_alarm_ptr != (s_can_backbone_transmit_alarm *)END_OF_CAN_LIST)
        {
          if (transmit_alarm_ptr->send == TRUE)
          {
            //send (in this case the condition should always be true) 
            if (CAN_ubRequestMsgObj(HW_TRANSMIT_ALARM))
            {
              //setup sw_msgobj
              sw_msgobj = SetupAlarmMsgObj(transmit_alarm_ptr);
              //remember the last alarm sent
              can_backbone_last_transmit_alarm_ptr = transmit_alarm_ptr;
              CAN_vConfigMsgObj(HW_TRANSMIT_ALARM, &sw_msgobj);
              CAN_vTransmit(HW_TRANSMIT_ALARM); 
            }
            break; //break "while" loop
          }
          transmit_alarm_ptr = transmit_alarm_ptr->next_ptr;
        }
}
        break;
    } // end "switch(IntID)"
/*
    CAN_HWNODE[can_backbone_hardware_node].IR = 0;  // JP 19-06-08 voorkomt dat elk bericht 2 keer wordt afgehandeld
    _nop();                                         // JP 19-06-08
    _nop();                                         // JP 19-06-08
    _nop();                                         // JP 19-06-08
    _nop();                                         // JP 19-06-08
    _nop();                                         // JP 19-06-08
    _nop();                                         // JP 19-06-08
    _nop();                                         // JP 19-06-08
*/
  }  // End of while()
} //  End of function Can_Backbone_Interrupt
#endif // CANopen

// Enable_CAN_Interrupts - enables the CAN-Controller interrupt
// Enables the interrupt for the CAN-Controller.
// The disable/enable counter is reset.
void Enable_CAN_Interrupts(void) 
{ 
  cocnt_canInt = 0;
  // internal CAN Controller
  CAN_0IC_IE = 1;
}

// Disable_CAN_Interrupts - disables the CAN-Controller interrupt
// Disables the interrupt for the CAN-Controller.
// Additionally the counter for disabling calls is incremented.
void Disable_CAN_Interrupts(void)
{
  // internal CAN controller 
  CAN_0IC_IE = 0;
  cocnt_canInt++;
}

// Restore_CAN_Interrupts - restore the CAN-Controller interrupt
// Decrements the disable/enable counter.
// If this counter is 0 the CAN interrupt is enabled.
void Restore_CAN_Interrupts(void)
{
  if(cocnt_canInt > 1) 
    cocnt_canInt--;
  else
    Enable_CAN_Interrupts();
}

void Enable_Can_Backbone_Int(void)
{
  CAN_0IC_IE = 1;
}

void Disable_Can_Backbone_Int(void)
{
  CAN_0IC_IE = 0;
}

//--------------------------------------------------------------------------------------

/*
 ** Send_Value
 *
 *  FILENAME: C:\C166_Projecten\CAN-Backbone\sources\CAN.C
 *
 *  PARAMETERS: pointer to the value to send
        node - node identifier
 *
 *  DESCRIPTION: the function sends a value to the receivers
        
         if the receiver is local, the message is copied in the receiver's field
          and the message is not put on the bus
         if there were no locals to accept it, the message is put on the bus
         if the transmission is a broadcast, data is copied to all the locals
          and the message is put on the bus as well

 *        if there is some other value that is pending for transmission in the list, 
          the current value will only get his flag set, actual transmission will
          be handled within the interrupt
        if there are no other pdos marked for transmissio, it means that the hw
          message object (can register) is free to be used and it will be loaded
          with data from the current value and then send on the bus
        
        !!! DO NOT SET THE "send" FIELD IN THE VALUE PRIOR TO CALLING THIS FUNCTION !!!
 *  RETURNS:
 *
 */
void Send_Value(s_can_backbone_transmit_value *transmit_value_ptr)
{
s_CAN_SWObj sw_msgobj;
s_can_backbone_receive_value *receive_value_ptr;
unsigned char local;
unsigned char loop;
unsigned char message_size;

  //if there have passed less then "first_transmit_value_ptr->time_delay_max" seconds since last value transmission
  // set the "send" flag to "PENDING" and the control function will recall "Send_Value" next second
  if (transmit_value_ptr->time_cnt < transmit_value_ptr->time_delay_max)
  {
    transmit_value_ptr->send = PENDING;
    return;   
  }
  local = FALSE;
  //send to all locals, and let them decide if they accept it
  switch (transmit_value_ptr->command)
  {
    default:
    case NO_COMMAND:
      receive_value_ptr = can_backbone_first_receive_value_ptr;
      while (receive_value_ptr != (s_can_backbone_receive_value *)END_OF_CAN_LIST)
      { 
//        if ((receive_value_ptr->node_ptr->address_node != transmit_value_ptr->address_sender) &&
        if ((receive_value_ptr->node_ptr->address_node != transmit_value_ptr->node_ptr->address_node) &&
            (receive_value_ptr->command == NO_COMMAND))
        {
          if ((receive_value_ptr->address_sender == BROADCAST) &&
              (transmit_value_ptr->address_receiver == BROADCAST))
          {
            //copy value
            receive_value_ptr->time_reset = 1; 
            message_size = transmit_value_ptr->message_size;
            for (loop = 0; loop < message_size; loop++)
              receive_value_ptr->buffer[loop] = transmit_value_ptr->buffer[loop];
            receive_value_ptr->message_size = message_size;
            //set "address_in" field
//            receive_value_ptr->address_sender_in = transmit_value_ptr->address_sender;
            receive_value_ptr->address_sender_in = transmit_value_ptr->node_ptr->address_node;
            //the transmit function will be called by the interrupt that handles the CAN comunication
            //if the CAN is not used, or the value was a broadcast, the transmit func will be called lower in the code
            if (receive_value_ptr->receive_func != 0)
            {
              (receive_value_ptr->receive_func)(receive_value_ptr, RECEIVE_VALUE_OK);          
            }
          }
//          else if ((receive_value_ptr->address_sender == transmit_value_ptr->address_sender) &&
          else if ((receive_value_ptr->address_sender == transmit_value_ptr->node_ptr->address_node) &&
//                   (receive_value_ptr->address_receiver == transmit_value_ptr->address_receiver))
                   (receive_value_ptr->node_ptr->address_node == transmit_value_ptr->address_receiver))
          {
            //copy value
            receive_value_ptr->time_reset = 1; 
            message_size = transmit_value_ptr->message_size;
            for (loop = 0; loop < message_size; loop++)
              receive_value_ptr->buffer[loop] = transmit_value_ptr->buffer[loop];
            receive_value_ptr->message_size = message_size;
            //set "address_in" field
//            receive_value_ptr->address_sender_in = transmit_value_ptr->address_sender;
            receive_value_ptr->address_sender_in = transmit_value_ptr->node_ptr->address_node;
            //the transmit function will be called by the interrupt that handles the CAN comunication
            //if the CAN is not used, or the value was a broadcast, the transmit func will be called lower in the code
            if (receive_value_ptr->receive_func != 0)
            {
              (receive_value_ptr->receive_func)(receive_value_ptr, RECEIVE_VALUE_OK);          
            }
            local = TRUE;  
            break;                                                                                 
          }
        }
        receive_value_ptr = receive_value_ptr->next_ptr;
      }//end "while"
      break;
    case REGISTER_COMMAND:
      receive_value_ptr = can_backbone_first_receive_value_ptr;
      while (receive_value_ptr != (s_can_backbone_receive_value *)END_OF_CAN_LIST)
      { 
//        if ((receive_value_ptr->node_ptr->address_node != transmit_value_ptr->address_sender) &&
        if ((receive_value_ptr->node_ptr->address_node != transmit_value_ptr->node_ptr->address_node) &&
            (receive_value_ptr->command == REGISTER_COMMAND))
        {
          receive_value_ptr->time_reset = 1; 
//          receive_value_ptr->address_sender_in = transmit_value_ptr->address_sender;
          receive_value_ptr->address_sender_in = transmit_value_ptr->node_ptr->address_node;
//          receive_value_ptr->buffer[0] = ((unsigned char *)&(transmit_value_ptr->address_sender))[0];
          receive_value_ptr->buffer[0] = ((unsigned char *)&(transmit_value_ptr->node_ptr->address_node))[0];
//          receive_value_ptr->buffer[1] = ((unsigned char *)&(transmit_value_ptr->address_sender))[1];
          receive_value_ptr->buffer[1] = ((unsigned char *)&(transmit_value_ptr->node_ptr->address_node))[1];
          //call "receive function" if there is any
          if (receive_value_ptr->receive_func != 0)
          {
            (receive_value_ptr->receive_func)(receive_value_ptr, RECEIVE_VALUE_OK);
          }
        }  
        receive_value_ptr = receive_value_ptr->next_ptr;
      }//end "while"
      break;
  }
  //if the CAN is not used, end function here
  if (can_backbone_hardware_used_extern == FALSE)
  {
    if (local == TRUE)
    {
      //if there is a transmit function and there was at least one local receiver
      if (transmit_value_ptr->transmit_func != 0)
      {
        (transmit_value_ptr->transmit_func)(transmit_value_ptr, TRANSMIT_VALUE_OK);
      }
      transmit_value_ptr->TRANSMIT_OK++;
    }
    else
    {
      //no local receiver was found
      if (transmit_value_ptr->transmit_func != 0)
      {
        (transmit_value_ptr->transmit_func)(transmit_value_ptr, TRANSMIT_VALUE_ERROR);
      }
      transmit_value_ptr->TRANSMIT_BAD++;
    }
    transmit_value_ptr->time_reset = 1;
    //reset the "send" flag
    transmit_value_ptr->send = FALSE;
    transmit_value_ptr->send_reset = 1;
    return;
  }
  //if the receiver is remote or broadcast, then put it on the bus
// Rdb 05-10-06  We also need to send value's to the bus if receiver address is != BROADCAST!
//  if ((local == FALSE) || (transmit_value_ptr->address_receiver == BROADCAST))
  if(local == FALSE)
  {
    //set flag
    Disable_Can_Backbone_Int();
    transmit_value_ptr->send = TRUE; 
    //access message object (condition should always be true)
    if (CAN_ubRequestMsgObj(HW_TRANSMIT_VALUE))
    {
      //configure sw msg obj using data from the value
      sw_msgobj = SetupTransmitValueMsgObj(transmit_value_ptr);
      can_backbone_last_transmit_value_ptr = transmit_value_ptr;
      CAN_vConfigMsgObj(HW_TRANSMIT_VALUE, &sw_msgobj);
      CAN_vTransmit(HW_TRANSMIT_VALUE);
    }
    Enable_Can_Backbone_Int();
  }
  else
  {
    //there was at least one local receiver, but the value is not a broadcast
    //call transmitter function if any
    if (transmit_value_ptr->transmit_func != 0)
    {
      (transmit_value_ptr->transmit_func)(transmit_value_ptr, TRANSMIT_VALUE_OK);
    }
    transmit_value_ptr->TRANSMIT_OK++;
    transmit_value_ptr->time_reset = 1;
    //reset the "send" flag
    transmit_value_ptr->send = FALSE;
    transmit_value_ptr->send_reset = 1;
  }
}

/*
 ** Send_Alarm
 *
 *  FILENAME: C:\C166_Projecten\CAN-Backbone\sources\CAN.C
 *
 *  PARAMETERS:
 *
 *  DESCRIPTION: see description for Send_Value
 *
 *  RETURNS:
 *
 */
void Send_Alarm(s_can_backbone_transmit_alarm *transmit_alarm_ptr)
{
s_CAN_SWObj sw_msgobj;
s_can_backbone_receive_alarm *receive_alarm_ptr;
s_can_backbone_slave *slave_ptr;
unsigned char local;
unsigned char loop;
unsigned char message_size;

  //if there have passed less then "first_transmit_value_ptr->time_delay_max" seconds since last alarm transmission
  // set the "send" flag to "PENDING" and the control function will recall "Send_Alarm" next second
  if (transmit_alarm_ptr->time_cnt < transmit_alarm_ptr->time_delay_max)
  {
    transmit_alarm_ptr->send = PENDING;
    return;   
  }
  local = FALSE;
  //send to all locals, and let them decide if they accept it
  receive_alarm_ptr = can_backbone_first_receive_alarm_ptr;
  while (receive_alarm_ptr != (s_can_backbone_receive_alarm *)END_OF_CAN_LIST)
  { 
    // do not accept youre own message
    if (receive_alarm_ptr->node_ptr->address_node != transmit_alarm_ptr->node_ptr->address_node)
    {
      receive_alarm_ptr->RECEIVE_OK++;
      receive_alarm_ptr->time_reset = 1;
      //copy value
      message_size = transmit_alarm_ptr->message_size;
      for (loop = 0; loop < message_size; loop++)
        receive_alarm_ptr->buffer[loop] = transmit_alarm_ptr->buffer[loop];
      receive_alarm_ptr->message_size = message_size;
      //set "address_in" field
//      receive_alarm_ptr->address_sender_in = transmit_alarm_ptr->address_sender;
      receive_alarm_ptr->address_sender_in = transmit_alarm_ptr->node_ptr->address_node;
      // at last call callback function
      if (receive_alarm_ptr->receive_func != 0)
      {
        (receive_alarm_ptr->receive_func)(receive_alarm_ptr, RECEIVE_ALARM_OK);          
      }
      local = TRUE;
    }
    receive_alarm_ptr = receive_alarm_ptr->next_ptr;
  }//end while
  // don't forget to reset time out counter from the sdo slave of this node!
  //find the sdo slave that holds the watchdog timer and reset the timer and the flag
  slave_ptr = can_backbone_first_slave_ptr;
  while (slave_ptr != (s_can_backbone_slave *)END_OF_CAN_LIST)
  {
    if (slave_ptr->address_sender == transmit_alarm_ptr->node_ptr->address_node)
    {
      slave_ptr->time_reset = 1; 
      slave_ptr->watchdog_flag = SET;
    }
    slave_ptr = slave_ptr->next_ptr;
  }

  //if the CAN is not used, end function here
  if (can_backbone_hardware_used_extern == FALSE)
  {
    if (local == TRUE)
    {
      //if there is a transmit function and there was at least one local receiver
      if (transmit_alarm_ptr->transmit_func != 0)
      {
        (transmit_alarm_ptr->transmit_func)(transmit_alarm_ptr, TRANSMIT_ALARM_OK);
      }
      transmit_alarm_ptr->TRANSMIT_OK++;
    }
    else
    {
      //no local receiver was found
      if (transmit_alarm_ptr->transmit_func != 0)
      {
        (transmit_alarm_ptr->transmit_func)(transmit_alarm_ptr, TRANSMIT_ALARM_ERROR);
      }
      transmit_alarm_ptr->TRANSMIT_BAD++;
    }
    transmit_alarm_ptr->time_reset = 1;
    transmit_alarm_ptr->send = FALSE;
    transmit_alarm_ptr->send_reset = 1;
    return;
  }
  //if the address is remote or broadcast, then put it on the bus
  if ((local == FALSE) || (transmit_alarm_ptr->address_receiver == BROADCAST))
  {
    Disable_Can_Backbone_Int();
    //set flag
    transmit_alarm_ptr->send = TRUE; 
    //access message object (condition should always be true)
    if (CAN_ubRequestMsgObj(HW_TRANSMIT_ALARM))
    {
      //config sw msg obj with data from the alarm struct
      sw_msgobj = SetupAlarmMsgObj(transmit_alarm_ptr);
      can_backbone_last_transmit_alarm_ptr = transmit_alarm_ptr;
      CAN_vConfigMsgObj(HW_TRANSMIT_ALARM, &sw_msgobj);
      CAN_vTransmit(HW_TRANSMIT_ALARM);
    }
    Enable_Can_Backbone_Int();
  }
  else
  {
    //there was at least one local receiver, but the alarm is not a broadcast
    //call transmiter function if any
    if (transmit_alarm_ptr->transmit_func != 0)
    {
      (transmit_alarm_ptr->transmit_func)(transmit_alarm_ptr, TRANSMIT_ALARM_OK);
    }
    transmit_alarm_ptr->TRANSMIT_OK++;
    transmit_alarm_ptr->time_reset = 1;
    transmit_alarm_ptr->send = FALSE;
    transmit_alarm_ptr->send_reset = 1;
  }
}

/*
 ** Send_Master
 *
 *  FILENAME: C:\C166_Projecten\CAN-Backbone\sources\CAN.C
 *
 *  PARAMETERS:
 *
 *  DESCRIPTION: 
 *
 *  RETURNS:
 *
 */
void Send_Master(s_can_backbone_master *master_ptr)
{
s_CAN_SWObj sw_msgobj;
s_can_backbone_slave *slave_ptr;
unsigned int j;
unsigned int message_size;

  //search the slave among locals
  slave_ptr = can_backbone_first_slave_ptr;
  while (slave_ptr != (s_can_backbone_slave *)END_OF_CAN_LIST)
  { 
//    if((slave_ptr->address_receiver == master_ptr->address_receiver) &&
    if((slave_ptr->node_ptr->address_node == master_ptr->address_receiver) &&
//       (slave_ptr->address_sender == master_ptr->address_sender))
       (slave_ptr->address_sender == master_ptr->node_ptr->address_node))
    {
      //the slave is local
      //copy data to slave
      message_size = master_ptr->message_size;
      for (j = 0; j < message_size; j++)
        (slave_ptr->buffer_ptr)[j] = (master_ptr->buffer_ptr)[j];
      //copy data size
      slave_ptr->message_size = message_size;
      //call funcs
      slave_ptr->RECEIVE_OK++;
      if (slave_ptr->receive_func != 0)
      {
        (slave_ptr->receive_func)(slave_ptr, RECEIVE_SLAVE_OK);
      }
      master_ptr->TRANSMIT_OK++;
      if (master_ptr->transmit_func != 0)
      {
        (master_ptr->transmit_func)(master_ptr, TRANSMIT_MASTER_OK);
      }
      master_ptr->buffer_index = 0;
      master_ptr->send = FALSE;
      master_ptr->send_reset = 1;
      //the slave was local and was found, no need to continue
      return;
    }
    slave_ptr = slave_ptr->next_ptr;
  }
  //if the CAN is not used, end function here
  if (can_backbone_hardware_used_extern == FALSE)
    return;
  // if the address is remote, use the bus
  master_ptr->send = TRUE;
  sw_msgobj = SetupMasterMsgObj(master_ptr);
  Disable_Can_Backbone_Int();
  PUSH(&sw_msgobj, &can_backbone_master_transmit_fifo);
  Enable_Can_Backbone_Int();
  if (master_ptr->fast)
  {
    int j;
    
    while ((master_ptr->fast == SDO_MAX_BLOCKS) || master_ptr->toggle_bit < 7)
    {
      if (master_ptr->message_size - master_ptr->buffer_index > 0)
      {
        master_ptr->toggle_bit++;
        master_ptr->toggle_bit &= 0x07;
        CAN_SW_Set_Count_Fast(&sw_msgobj, master_ptr->toggle_bit);
        if (master_ptr->message_size - master_ptr->buffer_index <= 8)
        {
          message_size = master_ptr->message_size - master_ptr->buffer_index;
          CAN_SW_SetCommand_Fast(&sw_msgobj, M2S_WRITE_LAST_BLOCK_OF_DATA_FAST);
          for (j = 0; j < message_size; j++)
          {
            sw_msgobj.data[j] = master_ptr->buffer_ptr[master_ptr->buffer_index];
            master_ptr->buffer_index++;
          }
          CAN_SW_SetNrOfBytes(&sw_msgobj, message_size);
          Disable_Can_Backbone_Int();
          PUSH(&sw_msgobj, &can_backbone_master_transmit_fifo);
          Enable_Can_Backbone_Int();
          break;
        }
        else
        {
          message_size = 8;
          CAN_SW_SetCommand_Fast(&sw_msgobj, M2S_WRITE_NEXT_BLOCK_OF_DATA_FAST);
          for (j = 0; j < message_size; j++)
          {
            sw_msgobj.data[j] = master_ptr->buffer_ptr[master_ptr->buffer_index];
            master_ptr->buffer_index++;
          }
          CAN_SW_SetNrOfBytes(&sw_msgobj, message_size);
          Disable_Can_Backbone_Int();
          PUSH(&sw_msgobj, &can_backbone_master_transmit_fifo);
          Enable_Can_Backbone_Int();
        }
      }
      else
      {
        // no more data to send
        break;
      }  
    }
  }
  Disable_Can_Backbone_Int();
  if (CAN_ubRequestMsgObj(HW_TRANSMIT_MASTER))
  {
    if ((can_backbone_master_transmit_fifo.pop_index != can_backbone_master_transmit_fifo.push_index) ||
        (can_backbone_master_transmit_fifo.full == 1))
    {
      //access the hw msg obj (in this case, condition should always be true)
      //get the next element from the s_fifo
      sw_msgobj = POP(&can_backbone_master_transmit_fifo);
      CAN_vConfigMsgObj(HW_TRANSMIT_MASTER, &sw_msgobj);
      CAN_vTransmit(HW_TRANSMIT_MASTER);  
    }
  }  
  master_ptr->prev_msg = sw_msgobj;    
  Enable_Can_Backbone_Int();
}

//*****************************************************************************
//* Can backbone Alarm
//*****************************************************************************

static void Can_Backbone_Check_Timeout_Transmit_Alarm(void)
// check to see if the last send Alarm is still waiting from a response from the bus
{
s_CAN_SWObj sw_msgobj;

  if (can_backbone_last_transmit_alarm_ptr != 0)
  {
    if (can_backbone_last_transmit_alarm_ptr->send_reset)
    {
      Disable_Can_Backbone_Int();
      can_backbone_last_transmit_alarm_ptr->send_timeout = CAN_ALARM_TIMEOUT;
      can_backbone_last_transmit_alarm_ptr->send_retry_counter = CAN_RETRY;
      can_backbone_last_transmit_alarm_ptr->send_reset = 0;
      Enable_Can_Backbone_Int();
    }
    if (can_backbone_last_transmit_alarm_ptr->send == TRUE)
    {
      if (can_backbone_last_transmit_alarm_ptr->send_timeout != 0)
      { 
        //not yet a timeout
        can_backbone_last_transmit_alarm_ptr->send_timeout--;
      }
      else
      {
        //time to wait has run out
        Disable_Can_Backbone_Int();
        if (can_backbone_last_transmit_alarm_ptr->send_retry_counter != 0)
        { 
          //there are some retries left, try and resend
          can_backbone_last_transmit_alarm_ptr->send_retry_counter--;
          //reset timer
          can_backbone_last_transmit_alarm_ptr->send_timeout = CAN_ALARM_TIMEOUT;
       
          //send message on the bus
          sw_msgobj = SetupAlarmMsgObj(can_backbone_last_transmit_alarm_ptr);
          CAN_vConfigMsgObj(HW_TRANSMIT_ALARM, &sw_msgobj);
          CAN_vTransmit(HW_TRANSMIT_ALARM);
        }
        else
        {
          //out of time and no more retries
          if (can_backbone_last_transmit_alarm_ptr->transmit_func != 0)
          {
            (can_backbone_last_transmit_alarm_ptr->transmit_func)(can_backbone_last_transmit_alarm_ptr, TRANSMIT_ALARM_ERROR);
          }

          can_backbone_last_transmit_alarm_ptr->TRANSMIT_BAD++;

          can_backbone_last_transmit_alarm_ptr->time_reset = 1;
          can_backbone_last_transmit_alarm_ptr->send = FALSE;
          can_backbone_last_transmit_alarm_ptr->send_timeout = CAN_ALARM_TIMEOUT;
          can_backbone_last_transmit_alarm_ptr->send_retry_counter = CAN_RETRY;
        }           
        Enable_Can_Backbone_Int();
      }
    }
  }  
}

//*****************************************************************************
//* Can backbone Value
//*****************************************************************************

static void Can_Backbone_Check_Timeout_Transmit_Value(void)
// check to see if the last send value is still waiting from a response from the bus
{
s_CAN_SWObj sw_msgobj;

  if (can_backbone_last_transmit_value_ptr != 0)
  {
    if (can_backbone_last_transmit_value_ptr->send_reset)
    {
      Disable_Can_Backbone_Int();
      can_backbone_last_transmit_value_ptr->send_timeout = CAN_VALUE_TIMEOUT;
      can_backbone_last_transmit_value_ptr->send_retry_counter = CAN_RETRY;
      can_backbone_last_transmit_value_ptr->send_reset = 0;
      Enable_Can_Backbone_Int();
    }
    if (can_backbone_last_transmit_value_ptr->send == TRUE)
    {
      if (can_backbone_last_transmit_value_ptr->send_timeout != 0)
      { 
        //not yet a timeout
        can_backbone_last_transmit_value_ptr->send_timeout--;
      }
      else
      {
        //time to wait has run out
        Disable_Can_Backbone_Int();
        if (can_backbone_last_transmit_value_ptr->send_retry_counter != 0)
        { 
          //there are some retries left, try and resend
          can_backbone_last_transmit_value_ptr->send_retry_counter--;
          //reset timer
          can_backbone_last_transmit_value_ptr->send_timeout = CAN_VALUE_TIMEOUT;
          //send message on the bus
          sw_msgobj = SetupTransmitValueMsgObj(can_backbone_last_transmit_value_ptr);
          CAN_vConfigMsgObj(HW_TRANSMIT_VALUE, &sw_msgobj);
          CAN_vTransmit(HW_TRANSMIT_VALUE);
        }
        else
        {
          //out of time and no more retrys
          if (can_backbone_last_transmit_value_ptr->transmit_func != 0)
          {
            (can_backbone_last_transmit_value_ptr->transmit_func)(can_backbone_last_transmit_value_ptr, TRANSMIT_VALUE_ERROR);
          }
          can_backbone_last_transmit_value_ptr->TRANSMIT_BAD++;
          can_backbone_last_transmit_value_ptr->time_reset = 1;
          //reset the "send" flag
          can_backbone_last_transmit_value_ptr->send = FALSE;
          can_backbone_last_transmit_value_ptr->send_timeout = CAN_VALUE_TIMEOUT;
          can_backbone_last_transmit_value_ptr->send_retry_counter = CAN_RETRY;
        }           
        Enable_Can_Backbone_Int();
      }
    }
  }  
}

//*****************************************************************************
//* Can backbone Master & Slave
//*****************************************************************************

static void Can_Backbone_Check_Timeout_Master(void)
// check to see if the last send master message is still waiting until ready
{
s_can_backbone_master *master_ptr;

  //*** SDO ***   
  master_ptr = can_backbone_first_master_ptr;
  while (master_ptr != (s_can_backbone_master *)END_OF_CAN_LIST)
  {
    if (master_ptr->send_reset)
    {
      Disable_Can_Backbone_Int();
      master_ptr->send_timeout = CAN_MASTER_TIMEOUT;
      master_ptr->send_retry_counter = CAN_RETRY;
      master_ptr->send_reset = 0;
      Enable_Can_Backbone_Int();
    }
    if (master_ptr->send == TRUE)
    //get first master that is during a transmission
    {
      if (master_ptr->send_timeout != 0)
        //there still is some time
        master_ptr->send_timeout--;
      else
      {
        Disable_Can_Backbone_Int();
        if (master_ptr->send_retry_counter != 0)
        {
          //still got some retrys left
          //force the last send msg of this master in the hardware and send it
          //send message on the bus
          CAN_vConfigMsgObj(HW_TRANSMIT_MASTER, &(master_ptr->prev_msg));
          CAN_vTransmit(HW_TRANSMIT_MASTER);
          //decrement retries
          master_ptr->send_retry_counter--;
          //reset counter
          master_ptr->send_timeout = CAN_MASTER_TIMEOUT;
          break; //break for loop
        }
        else
        {
          //out of time and no more retrys
          if (master_ptr->transmit_func != 0)
          {
            (master_ptr->transmit_func)(master_ptr, TRANSMIT_MASTER_ERROR);
          }
          master_ptr->TRANSMIT_BAD++;
          master_ptr->buffer_index = 0;
          master_ptr->send = FALSE;
          master_ptr->send_timeout = CAN_MASTER_TIMEOUT;
          master_ptr->send_retry_counter = CAN_RETRY;
        }
        Enable_Can_Backbone_Int();
      }  
    }
    master_ptr = master_ptr->next_ptr;
  }
}

void Can_Backbone_Diagnose_Msg(void)
{
static unsigned int txok[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, };
static unsigned int rxok[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, };
static int index = 0;

  if (can_backbone_diag_reset_flag)
  {
    s_can_status can_backbone_status_empty = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

    can_backbone_diag_reset_flag = 0;
    can_backbone_status = can_backbone_status_empty;
    can_backbone_txok = 0;
    can_backbone_txok_peak = 0;
    can_backbone_rxok = 0;
    can_backbone_rxok_peak = 0;
    can_backbone_txok_rxok = 0;
    can_backbone_txok_rxok_peak = 0;
    for (index = 0; index < 10; index++)
      txok[index] = rxok[index] = 0;
    index = 0;  
  }
  else
  {
    index %= PULSES_PER_SECOND;
    can_backbone_txok -= txok[index];
    can_backbone_rxok -= rxok[index];
    txok[index] = can_backbone_status.TXOK;
    rxok[index] = can_backbone_status.RXOK;
    can_backbone_status.RXOK = can_backbone_status.TXOK = 0;
    can_backbone_txok += txok[index];
    can_backbone_rxok += rxok[index];
    index++;
    can_backbone_txok_rxok = can_backbone_txok + can_backbone_rxok;
    if(can_backbone_txok > can_backbone_txok_peak)
      can_backbone_txok_peak = can_backbone_txok;
    if(can_backbone_rxok > can_backbone_rxok_peak)
      can_backbone_rxok_peak = can_backbone_rxok;
    if(can_backbone_txok_rxok > can_backbone_txok_rxok_peak)
      can_backbone_txok_rxok_peak = can_backbone_txok_rxok;
  }    
}

void Can_Backbone_Hardware_Timing_Control(void)
{
  Can_Backbone_Diagnose_Msg();
  Can_Backbone_Check_Timeout_Transmit_Alarm();
  Can_Backbone_Check_Timeout_Transmit_Value();
//  Can_Backbone_Check_Timeout_Master();
}

