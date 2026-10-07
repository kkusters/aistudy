// C__CAN_IO.C									

#include "ch_define.h"
#include "ch_alg.h"
#include "ch_asc1.h"
#include "ch_can.h"
#include "ch_IO_05_07.h"
#include "ch_IO_06_14.h"
#include "ch_IO_07_07.h"
#include "ch_IO_08_09.h"
#include "ch_IO_12_06.h"
#include "ch_IO_EKU.h"
#include "ch_IO_H1MC.h"
#include "ch_IO_H2MC.h"
#include "ch_IO_05_07_board.h"
#include "ch_IO_06_14_board.h"
#include "ch_IO_07_07_board.h"
#include "ch_IO_08_09_board.h"
#include "ch_IO_12_06_board.h"
#include "ch_IO_EKU_board.h"
#include "ch_IO_H1MC_board.h"
#include "ch_IO_H2MC_board.h"
//#include "ch_disp.h" // voor test display_update_switch
#include "ch_can_IO.h"

#define CAN_IO_TIMEOUT 10

bit can_io_alarm = 1;
bit can_io_init_switch = 1;
bit can_io_PDO_busy = 0;
bit can_io_SDO_master_busy = 0;
bit can_io_SDO_master_toggle_bit = 0;
u_SDO_data can_io_SDO_master_data;
unsigned int can_io_PDO_timeout = CAN_IO_TIMEOUT;
unsigned int can_io_SDO_master_timeout = CAN_IO_TIMEOUT;
unsigned int can_io_SDO_master_length;
unsigned char *can_io_SDO_master_ptr;

s_can_state_cnt can_IO_state = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

unsigned char can_io_diag_reset_flag = 0;
unsigned int can_io_txok = 0;
unsigned int can_io_txok_peak = 0;
unsigned int can_io_rxok = 0;
unsigned int can_io_rxok_peak = 0;
unsigned int can_io_txok_rxok = 0;
unsigned int can_io_txok_rxok_peak = 0;

void Transmit_IO_SDO_Master_Component_OK(unsigned char card_id, unsigned char card_nr, unsigned char component_nr, unsigned char component_id)
{
  switch (card_id)
  {
    case BROADCOAST_ID:
	  break;
	case IO_06_14_ID:
	  IO_06_14_Component_SDO_Master_Transmit_OK(card_nr, component_id, component_nr);
	  break;
	case IO_12_06_ID:
	  IO_12_06_Component_SDO_Master_Transmit_OK(card_nr, component_id, component_nr);
	  break;
	case IO_08_09_ID:
	  IO_08_09_Component_SDO_Master_Transmit_OK(card_nr, component_id, component_nr);
	  break;
	case IO_EKU_ID:
	  IO_EKU_Component_SDO_Master_Transmit_OK(card_nr, component_id, component_nr);
	  break;
	case IO_H2MC_ID:
	case IO_H2MC_ID + 1:
	  card_nr += (((int)card_id - IO_H2MC_ID) * 32);
	  IO_H2MC_Component_SDO_Master_Transmit_OK(card_nr, component_id, component_nr);
	  break;
	case IO_H1MC_ID:
	  IO_H1MC_Component_SDO_Master_Transmit_OK(card_nr, component_id, component_nr);
	  break;
	case IO_05_07_ID:
	  IO_05_07_Component_SDO_Master_Transmit_OK(card_nr, component_id, component_nr);
	  break;
	case IO_07_07_ID:
	  IO_07_07_Component_SDO_Master_Transmit_OK(card_nr, component_id, component_nr);
	  break;
  }
}

void Receive_IO_SDO_Master_Component(u_SDO_data *ptr,unsigned char card_id,unsigned char card_nr,unsigned char component_id,unsigned char component_nr)
{
  switch (card_id)
  {
    case BROADCOAST_ID:
	  break;
	case IO_06_14_ID:
	  IO_06_14_Component_SDO_Master_Receive(ptr,card_nr,component_id,component_nr);
	  break;
	case IO_12_06_ID:
	  IO_12_06_Component_SDO_Master_Receive(ptr,card_nr,component_id,component_nr);
	  break;
	case IO_08_09_ID:
	  IO_08_09_Component_SDO_Master_Receive(ptr,card_nr,component_id,component_nr);
	  break;
	case IO_EKU_ID:
	  IO_EKU_Component_SDO_Master_Receive(ptr,card_nr,component_id,component_nr);
	  break;
	case IO_H2MC_ID:
	case IO_H2MC_ID + 1:
	  card_nr += (((int)card_id - IO_H2MC_ID) * 32);
	  IO_H2MC_Component_SDO_Master_Receive(ptr,card_nr,component_id,component_nr);
	  break;
	case IO_H1MC_ID:
	  IO_H1MC_Component_SDO_Master_Receive(ptr,card_nr,component_id,component_nr);
	  break;
	case IO_05_07_ID:
	  IO_05_07_Component_SDO_Master_Receive(ptr,card_nr,component_id,component_nr);
	  break;
	case IO_07_07_ID:
	  IO_07_07_Component_SDO_Master_Receive(ptr,card_nr,component_id,component_nr);
	  break;
  }
}

void Transmit_IO_PDO_Component_OK(unsigned char card_id, unsigned char card_nr,unsigned char component_id, unsigned char component_nr)
{
  switch (card_id)
  {
    case BROADCOAST_ID:
	  break;
	case IO_06_14_ID:
	  IO_06_14_Component_PDO_Transmit_OK(card_nr,component_id,component_nr);
	  break;
	case IO_12_06_ID:
	  IO_12_06_Component_PDO_Transmit_OK(card_nr,component_id,component_nr);
	  break;
	case IO_08_09_ID:
	  IO_08_09_Component_PDO_Transmit_OK(card_nr,component_id,component_nr);
	  break;
	case IO_EKU_ID:
	  IO_EKU_Component_PDO_Transmit_OK(card_nr,component_id,component_nr);
	  break;
	case IO_H2MC_ID:
	case IO_H2MC_ID + 1:
	  card_nr += (((int)card_id - IO_H2MC_ID) * 32);
	  IO_H2MC_Component_PDO_Transmit_OK(card_nr,component_id,component_nr);
	  break;
	case IO_H1MC_ID:
	  IO_H1MC_Component_PDO_Transmit_OK(card_nr,component_id,component_nr);
	  break;
	case IO_05_07_ID:
	  IO_05_07_Component_PDO_Transmit_OK(card_nr,component_id,component_nr);
	  break;
	case IO_07_07_ID:
	  IO_07_07_Component_PDO_Transmit_OK(card_nr,component_id,component_nr);
	  break;
  }
}

void Receive_IO_PDO_Component(unsigned char *ptr, unsigned char length,
                           unsigned char card_id, unsigned char card_nr, unsigned char component_id, unsigned char component_nr)
{
  switch (card_id)
  {
    case BROADCOAST_ID: // broadcaost message received
	  break;
	case IO_06_14_ID:
	  IO_06_14_Component_PDO_Receive(ptr, length, card_nr, component_id, component_nr);
	  break;
	case IO_12_06_ID:
	  IO_12_06_Component_PDO_Receive(ptr, length, card_nr, component_id, component_nr);
	  break;
	case IO_08_09_ID:
	  IO_08_09_Component_PDO_Receive(ptr, length, card_nr, component_id, component_nr);
	  break;
	case IO_EKU_ID:
	  IO_EKU_Component_PDO_Receive(ptr, length, card_nr, component_id, component_nr);
	  break;
	case IO_H2MC_ID:
	case IO_H2MC_ID + 1:
	  card_nr += (((int)card_id - IO_H2MC_ID) * 32);
	  IO_H2MC_Component_PDO_Receive(ptr, length, card_nr, component_id, component_nr);
	  break;
	case IO_H1MC_ID:
	  IO_H1MC_Component_PDO_Receive(ptr, length, card_nr, component_id, component_nr);
	  break;
	case IO_05_07_ID:
	  IO_05_07_Component_PDO_Receive(ptr, length, card_nr, component_id, component_nr);
	  break;
	case IO_07_07_ID:
	  IO_07_07_Component_PDO_Receive(ptr, length, card_nr, component_id, component_nr);
	  break;
  }
}

#define CAN_IO 0
#define CAN_IO_RXD P7_6
#define DCAN_IO_RXD DP7_6
#define ODCAN_IO_RXD ODP7_6
#define CAN_IO_TXT P7_7
#define DCAN_IO_TXT DP7_7
#define ODCAN_IO_TXT ODP7_7
#define AS0DCAN_IO_TXT AS0P7_7

/****************************************************************************************************/

void CAN_IO_Init(void)
{
int loop;

  Disable_Can_IO_Int(); // disable interrupt
  CAN_HWNODE[CAN_IO].CR = 0x0041;
  
  CAN_HWNODE[CAN_IO].GINP = 0x1111;
  CAN_HWNODE[CAN_IO].ECNTH = 0x0060;
  CAN_HWNODE[CAN_IO].BTRL = 0x1C18; // 0x3440 | ((CLKFREQ / (10 * CAN_IO_BAUDRATE)) - 1);
  CAN_HWNODE[CAN_IO].BTRH = 0x0000;
  CAN_HWNODE[CAN_IO].FCRL = 0x0000;
  CAN_HWNODE[CAN_IO].FCRH = 0x0000;
  CAN_HWNODE[CAN_IO].IMR4 = 0x0007;
  CAN_HWNODE[CAN_IO].IMRL0 = 0x0000;
  CAN_HWNODE[CAN_IO].IMRH0 = 0x000F;

  CAN_HWOBJ[HW_IO_TRANSMIT_MASTER].CFGL = 0x000C;
  CAN_HWOBJ[HW_IO_TRANSMIT_MASTER].CFGH = 0x0011;
  CAN_HWOBJ[HW_IO_TRANSMIT_MASTER].AMR = 0xFFFFFFFFL; // acceptance maks
  CAN_HWOBJ[HW_IO_TRANSMIT_MASTER].AR =  0x08000000L; // arbitration reg
  for (loop = 0; loop < 8; loop++)
    CAN_HWOBJ[HW_IO_TRANSMIT_MASTER].data[loop] = 0;
  CAN_HWOBJ[HW_IO_TRANSMIT_MASTER].FCRL = 0x0000;
  CAN_HWOBJ[HW_IO_TRANSMIT_MASTER].FCRH = 0x0001;
  CAN_HWOBJ[HW_IO_TRANSMIT_MASTER].CTRL = 0x55A5;
  CAN_HWOBJ[HW_IO_TRANSMIT_MASTER].CTRH = 0x0000;
  
  CAN_HWOBJ[HW_IO_RECEIVE_MASTER].CFGL = 0x0004;
  CAN_HWOBJ[HW_IO_RECEIVE_MASTER].CFGH = 0x0011;
  CAN_HWOBJ[HW_IO_RECEIVE_MASTER].AMR = 0xE3800000L; // acceptance maks
  CAN_HWOBJ[HW_IO_RECEIVE_MASTER].AR =  0x00800000L; // arbitration reg
  for (loop = 0; loop < 8; loop++)
    CAN_HWOBJ[HW_IO_RECEIVE_MASTER].data[loop] = 0;
  CAN_HWOBJ[HW_IO_RECEIVE_MASTER].FCRL = 0x0000;
  CAN_HWOBJ[HW_IO_RECEIVE_MASTER].FCRH = 0x0000;
  CAN_HWOBJ[HW_IO_RECEIVE_MASTER].CTRL = 0x5599;
  CAN_HWOBJ[HW_IO_RECEIVE_MASTER].CTRH = 0x0000;

  CAN_HWOBJ[HW_IO_TRANSMIT_VALUE].CFGL = 0x000C;
  CAN_HWOBJ[HW_IO_TRANSMIT_VALUE].CFGH = 0x0011;
  CAN_HWOBJ[HW_IO_TRANSMIT_VALUE].AMR = 0xFFFFFFFFL; // acceptance maks
  CAN_HWOBJ[HW_IO_TRANSMIT_VALUE].AR =  0x00000000L; // arbitration reg
  for (loop = 0; loop < 8; loop++)
    CAN_HWOBJ[HW_IO_TRANSMIT_VALUE].data[loop] = 0;
  CAN_HWOBJ[HW_IO_TRANSMIT_VALUE].FCRL = 0x0000;
  CAN_HWOBJ[HW_IO_TRANSMIT_VALUE].FCRH = 0x0003;
  CAN_HWOBJ[HW_IO_TRANSMIT_VALUE].CTRL = 0x55A5;
  CAN_HWOBJ[HW_IO_TRANSMIT_VALUE].CTRH = 0x0000;
  
  CAN_HWOBJ[HW_IO_RECEIVE_VALUE].CFGL = 0x0004;
  CAN_HWOBJ[HW_IO_RECEIVE_VALUE].CFGH = 0x0011;
  CAN_HWOBJ[HW_IO_RECEIVE_VALUE].AMR = 0xE3800000L; // acceptance maks
  CAN_HWOBJ[HW_IO_RECEIVE_VALUE].AR =  0x00000000L; // arbitration reg
  for (loop = 0; loop < 8; loop++)
    CAN_HWOBJ[HW_IO_RECEIVE_VALUE].data[loop] = 0;
  CAN_HWOBJ[HW_IO_RECEIVE_VALUE].FCRL = 0x0000;
  CAN_HWOBJ[HW_IO_RECEIVE_VALUE].FCRH = 0x0002;
  CAN_HWOBJ[HW_IO_RECEIVE_VALUE].CTRL = 0x5599;
  CAN_HWOBJ[HW_IO_RECEIVE_VALUE].CTRH = 0x0000;
  
  CAN_PISEL = (CAN_PISEL & 0xFFF8) | 0x0002;
  CAN_IO_RXD = 1;
  DCAN_IO_RXD = 0;
  ODCAN_IO_RXD = 0;
  AS0DCAN_IO_TXT = 1;
  CAN_IO_TXT = 1;
  DCAN_IO_TXT = 1;
  ODCAN_IO_TXT = 1;
  
  CAN_1IC = CAN_IO_EVENT_LEVEL;
  CAN_HWNODE[CAN_IO].CR = 0x001C;
  Enable_Can_IO_Int(); // enable interrupt

  can_io_PDO_busy = 0;
  can_io_PDO_timeout = CAN_IO_TIMEOUT;

  can_io_SDO_master_busy = 0;
  can_io_SDO_master_toggle_bit = 0;
  can_io_SDO_master_timeout = CAN_IO_TIMEOUT;

  can_io_init_switch = 0;
}

_inline bit CAN_IO_Read_W_Bit(s_CanObj volatile *ptr)
{
  return (ptr->AR & 0x00400000L);
}

_inline bit CAN_IO_Read_Toggle_Bit(s_CanObj volatile *ptr)
{
  return (ptr->data[0] & 0x80);
}

_inline void CAN_IO_Transmit(s_CanObj volatile *ptr)
{
  ptr->CTRL = 0xE7FF; // set TXRQ, reset CPUUPD
}

_inline unsigned char CAN_IO_Read_Card_Nr(s_CanObj volatile *ptr)
{
  return (ptr->AR & 0x0000001FL);
}

_inline unsigned char CAN_IO_Read_Card_Id(s_CanObj volatile *ptr)
{
  return ((ptr->AR >> 5) & 0x0000007FL);
}

_inline unsigned char CAN_IO_Read_Component_Nr(s_CanObj volatile *ptr)
{
  return ((ptr->AR >> 12) & 0x0000001FL);
}

_inline unsigned char CAN_IO_Read_Component_Id(s_CanObj volatile *ptr)
{
  return ((ptr->AR >> 17) & 0x0000001FL);
}

interrupt CAN_IO_ADR using(CAN_IO_RB) void CAN_IO_Int(void)
{
unsigned int status;
unsigned int IntID;
unsigned char length;
unsigned char card_nr;
unsigned char card_id;
unsigned char component_nr;
unsigned char component_id;
s_CanObj can_obj;

  while(CAN_HWNODE[CAN_IO].IR)
  {
    IntID = CAN_HWNODE[CAN_IO].IR;

    CAN_HWNODE[CAN_IO].IR = 0; // JP 20-06-08
    switch(IntID)
    {
      case 1:
        status = CAN_HWNODE[CAN_IO].SR;
        if (status & 0x0080)  // if BOFF
        {
          Disable_Can_IO_Int();
          can_io_init_switch = 1;
        }
        if (status & 0x00040)  // if EWRN
        {
          // Indicates that the least one of the error counters in the
          // EML has reached the error warning limit of 96.
          can_IO_state.EWRN++;
        }
        if (status & 0x0008)  // if TXOK
        {
          // Indicates that a message has been transmitted successfully
          // (error free and acknowledged by at least one other node).
          status &= 0xfff7;
          CAN_HWNODE[CAN_IO].SR = status;    // reset TXOK
          can_IO_state.TXOK++;
          can_io_alarm = 0;
        }
        if (status & 0x0010)  // if RXOK
        {
          // Indicates that a message has been received successfully.
          status &= 0xffef;
          CAN_HWNODE[CAN_IO].SR = status;    // reset RXOK
          can_IO_state.RXOK++;
          can_io_alarm = 0;
        }
        if (status & 0x0007)  // if LEC
        {
          switch (status & 0x0007)  // LECA (Last Error CodeA)
          {
            case 1: // Stuff Error
              // More than 5 equal bits in a sequence have occurred
              // in a part of a received message where this is not allowed.
              can_IO_state.stuff_error++;
              break;
            case 2: // Form Error
              // A fixed format part of a received frame has the wrong format.
              can_IO_state.form_error++;
              break;
            case 3: // Ack Error
              // The message this CAN controller transmitted was not acknowledged 
              // by another node.
              can_IO_state.ack_error++;
              break;
            case 4: // Bit1 Error
              // During the transmission of a message (with the
              // exeption of the arbitration field), the device
              // wanted to send a recessive level ("1"), but the
              // monitored bus value was dominant.
              can_IO_state.bit1_error++;
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
                can_IO_state.bit0_busoff_error++;
              }
              else
              {
                // USER CODE BEGIN (SRN0_NODEA,10)
                can_IO_state.bit0_normal_error++;
              }
              break;
            case 6: // CRC Error
              // The CRC check sum was incorrect in the message received.
              can_IO_state.crc_error++;       
              break;
            default:
              // USER CODE BEGIN (SRN0_NODEA,12)
              break;
          }
          status &= 0xfff8;
          CAN_HWOBJ[0].CTRL = 0xFFFF; // erata TWINCAN_AI.007
          CAN_HWNODE[CAN_IO].SR = status; // reset LEC
        }
        break;
      case 18: // HW_IO_TRANSMIT_MASTER
if ((CAN_HWOBJ[HW_IO_TRANSMIT_MASTER].CTRL & 0x0003) == 0x0002) // if INTPND
{
        CAN_HWOBJ[HW_IO_TRANSMIT_MASTER].CTRL = 0xfdff; // reset NEWDAT
        CAN_HWOBJ[HW_IO_TRANSMIT_MASTER].CTRL = 0xfffd; // reset INTPND
        switch (CAN_HWOBJ[HW_IO_TRANSMIT_MASTER].data[0] & 0x0F)
        {
          case 0x0A: // error transmit
            can_io_SDO_master_busy = 0;
            break;
        }
}
        break;
      case 19: // HW_IO_RECEIVE_MASTER
if ((CAN_HWOBJ[HW_IO_RECEIVE_MASTER].CTRL & 0x0003) == 0x0002) // if INTPND
{
        CAN_HWOBJ[HW_IO_RECEIVE_MASTER].CTRL = 0xfffd;  // reset INTPND
        if ((CAN_HWOBJ[HW_IO_RECEIVE_MASTER].CTRL & 0x0300) == 0x0200) // NEWDAT is set
        {
          can_obj = CAN_HWOBJ[HW_IO_RECEIVE_MASTER];
          if ((CAN_HWOBJ[HW_IO_RECEIVE_MASTER].CTRL & 0x0c00) == 0x0800) // if MSGLST is set
          {
            CAN_HWOBJ[HW_IO_RECEIVE_MASTER].CTRL = 0xf7ff;  // reset MSGLST
            CAN_HWOBJ[HW_IO_RECEIVE_MASTER].CTRL = 0x7dff;  // reset NEWDAT
            can_io_SDO_master_busy = 0;
          }
          else
          {
            CAN_HWOBJ[HW_IO_RECEIVE_MASTER].CTRL = 0x7DFF;
            switch (can_obj.data[0] & 0x0F)
            {
              case 0x04: // SDO Write ask for next block
                if ((can_obj.AR |  0xFC400000L) != 
                    (CAN_HWOBJ[HW_IO_TRANSMIT_MASTER].AR | 0xFC000000L))
                {
                  can_io_SDO_master_busy = 0;
                }    
                else if (CAN_IO_Read_Toggle_Bit(&can_obj) ^ can_io_SDO_master_toggle_bit)
                {
                  can_io_SDO_master_busy = 0;
                }
                else if (!can_io_SDO_master_busy)
                {
                  can_io_SDO_master_busy = 0;
                }
                else
                {
                  can_io_SDO_master_timeout = CAN_IO_TIMEOUT;
                  can_io_SDO_master_toggle_bit = !can_io_SDO_master_toggle_bit;
                  can_io_SDO_master_ptr += 7;
                  can_io_SDO_master_length -= 7;
                  if (can_io_SDO_master_length > 7) // check if 7 characters or more
                  {
                    length = 7;
                    CAN_HWOBJ[HW_IO_TRANSMIT_MASTER].data[0] = 0x02;
                  }
                  else
                  {
                    length = can_io_SDO_master_length;
                    CAN_HWOBJ[HW_IO_TRANSMIT_MASTER].data[0] = 0x03;
                  }
                  Copy_N_Bytes((unsigned char *)&CAN_HWOBJ[HW_IO_TRANSMIT_MASTER].data[1], can_io_SDO_master_ptr, length);
                  length <<= 4;
                  CAN_HWOBJ[HW_IO_TRANSMIT_MASTER].data[0] += length;
                  CAN_HWOBJ[HW_IO_TRANSMIT_MASTER].CFGL = 0x1C + length;
                  if (can_io_SDO_master_toggle_bit)
                    CAN_HWOBJ[HW_IO_TRANSMIT_MASTER].data[0] |= 0x80;
                  CAN_HWOBJ[HW_IO_TRANSMIT_MASTER].CTRL = 0xF6BF;  
                  CAN_IO_Transmit(&CAN_HWOBJ[HW_IO_TRANSMIT_MASTER]);
                }
                break;
              case 0x05: // SDO Write last block correct received
                if ((can_obj.AR |  0xFC400000L) != 
                    (CAN_HWOBJ[HW_IO_TRANSMIT_MASTER].AR | 0xFC000000L))
                {
                  can_io_SDO_master_busy = 0;
                }    
                else if (CAN_IO_Read_Toggle_Bit(&can_obj) ^ can_io_SDO_master_toggle_bit)
                {
                  can_io_SDO_master_busy = 0;
                }
                else if (!can_io_SDO_master_busy)
                {
                  can_io_SDO_master_busy = 0;
                }
                else
                {
                  card_nr = CAN_IO_Read_Card_Nr(&can_obj);
                  card_id = CAN_IO_Read_Card_Id(&can_obj);
                  component_nr = CAN_IO_Read_Component_Nr(&can_obj);
                  component_id = CAN_IO_Read_Component_Id(&can_obj);
                  Transmit_IO_SDO_Master_Component_OK(card_id, card_nr, component_nr, component_id);
                  can_io_SDO_master_busy = 0;
                }
                break;  
              case 0x08: // SDO Read send block with data
                if ((can_obj.AR |  0xFC400000L) != 
                    (CAN_HWOBJ[HW_IO_TRANSMIT_MASTER].AR | 0xFC000000L))
                {
                  can_io_SDO_master_busy = 0;
                }    
                else if (CAN_IO_Read_Toggle_Bit(&can_obj) ^ can_io_SDO_master_toggle_bit)
                {
                  can_io_SDO_master_busy = 0;
                }
                else if (!can_io_SDO_master_busy)
                {
                  can_io_SDO_master_busy = 0;
                }
                else
                {
                  can_io_SDO_master_timeout = CAN_IO_TIMEOUT;
                  can_io_SDO_master_toggle_bit = !can_io_SDO_master_toggle_bit;
                  Copy_N_Bytes(can_io_SDO_master_ptr, (unsigned char *)&can_obj.data[1], 7);
                  can_io_SDO_master_ptr += 7;
                  CAN_HWOBJ[HW_IO_TRANSMIT_MASTER].data[0] = 0x17;
                  if (can_io_SDO_master_toggle_bit)
                    CAN_HWOBJ[HW_IO_TRANSMIT_MASTER].data[0] |= 0x80;
                  CAN_HWOBJ[HW_IO_TRANSMIT_MASTER].CTRL = 0xF6BF;  
                  CAN_IO_Transmit(&CAN_HWOBJ[HW_IO_TRANSMIT_MASTER]);
                }
                break;
              case 0x09: // SDO Read send last block with data
                if ((can_obj.AR |  0xFC400000L) != 
                    (CAN_HWOBJ[HW_IO_TRANSMIT_MASTER].AR | 0xFC000000L))
                {
                  can_io_SDO_master_busy = 0;
                }    
                else if (CAN_IO_Read_Toggle_Bit(&can_obj) ^ can_io_SDO_master_toggle_bit)
                {
                  can_io_SDO_master_busy = 0;
                }
                else if (!can_io_SDO_master_busy)
                {
                  can_io_SDO_master_busy = 0;
                }
                else
                {
                  Copy_N_Bytes(can_io_SDO_master_ptr, (unsigned char *)&can_obj.data[1], 
                               (can_obj.data[0] >> 4) & 0x07);
                  card_nr = CAN_IO_Read_Card_Nr(&can_obj);
                  card_id = CAN_IO_Read_Card_Id(&can_obj);
                  component_nr = CAN_IO_Read_Component_Nr(&can_obj);
                  component_id = CAN_IO_Read_Component_Id(&can_obj);
                  Receive_IO_SDO_Master_Component(&can_io_SDO_master_data, card_id, card_nr, component_id, component_nr);
                  can_io_SDO_master_busy = 0;
                }
                break;  
              case 0x0A: // SDO error
                can_io_SDO_master_busy = 0;
                break;
            }
          }
        }  
}
        break;
      case 20: // HW_IO_TRANSMIT_VALUE
if ((CAN_HWOBJ[HW_IO_TRANSMIT_VALUE].CTRL & 0x0003) == 0x0002) // if INTPND
{
        CAN_HWOBJ[HW_IO_TRANSMIT_VALUE].CTRL = 0xfdff;      // reset NEWDAT
        CAN_HWOBJ[HW_IO_TRANSMIT_VALUE].CTRL = 0xfffd;   // reset INTPND
        card_nr = CAN_IO_Read_Card_Nr(&CAN_HWOBJ[HW_IO_TRANSMIT_VALUE]);
        card_id = CAN_IO_Read_Card_Id(&CAN_HWOBJ[HW_IO_TRANSMIT_VALUE]);
        component_nr = CAN_IO_Read_Component_Nr(&CAN_HWOBJ[HW_IO_TRANSMIT_VALUE]);
        component_id = CAN_IO_Read_Component_Id(&CAN_HWOBJ[HW_IO_TRANSMIT_VALUE]);
        Transmit_IO_PDO_Component_OK(card_id, card_nr, component_id, component_nr);
        can_io_PDO_busy = 0;
}
        break;
      case 21: // HW_IO_RECEIVE_VALUE
if ((CAN_HWOBJ[HW_IO_RECEIVE_VALUE].CTRL & 0x0003) == 0x0002) // if INTPND
{
        CAN_HWOBJ[HW_IO_RECEIVE_VALUE].CTRL = 0xfffd;  // reset INTPND
        if ((CAN_HWOBJ[HW_IO_RECEIVE_VALUE].CTRL & 0x0300) == 0x0200) // NEWDAT is set
        {
          can_obj = CAN_HWOBJ[HW_IO_RECEIVE_VALUE];
          if ((CAN_HWOBJ[HW_IO_RECEIVE_VALUE].CTRL & 0x0c00) == 0x0800) // if MSGLST is set
          {
            CAN_HWOBJ[HW_IO_RECEIVE_VALUE].CTRL = 0xf7ff;  // reset MSGLST
            CAN_HWOBJ[HW_IO_RECEIVE_VALUE].CTRL = 0x7dff;  // reset NEWDAT
          }
          else
          {
            CAN_HWOBJ[HW_IO_RECEIVE_VALUE].CTRL = 0x7DFF;
            card_nr = CAN_IO_Read_Card_Nr(&can_obj);
            card_id = CAN_IO_Read_Card_Id(&can_obj);
            component_nr = CAN_IO_Read_Component_Nr(&can_obj);
            component_id = CAN_IO_Read_Component_Id(&can_obj);
            length = (can_obj.CFGL >> 4) & 0x000F;
            Receive_IO_PDO_Component((unsigned char *)&can_obj.data[0], length, card_id, card_nr, component_id, component_nr);
          }
        }  
}
        break;
    }
/*
    CAN_HWNODE[CAN_IO].IR = 0; // JP 20-06-08
    _nop();                    // JP 20-06-08
    _nop();                    // JP 20-06-08
    _nop();                    // JP 20-06-08
    _nop();                    // JP 20-06-08
    _nop();                    // JP 20-06-08
    _nop();                    // JP 20-06-08
    _nop();                    // JP 20-06-08
*/
  }      
}

static void CAN_IO_PDO_Transmit(unsigned char priority,
                                unsigned char card_id, unsigned char card_nr, 
                                unsigned char component_id, unsigned char component_nr)
{
unsigned char length;

  if (can_io_PDO_busy) // this if should be placed in the procedure call
    return;

  switch (card_id)
  {
    case IO_06_14_ID:
	  length = IO_06_14_Component_PDO_Transmit((unsigned char *)&(CAN_HWOBJ[HW_IO_TRANSMIT_VALUE].data[0]), card_nr, component_id, component_nr);
	  break;
    case IO_12_06_ID:
	  length = IO_12_06_Component_PDO_Transmit((unsigned char *)&(CAN_HWOBJ[HW_IO_TRANSMIT_VALUE].data[0]), card_nr, component_id, component_nr);
	  break;
    case IO_08_09_ID:
	  length = IO_08_09_Component_PDO_Transmit((unsigned char *)&(CAN_HWOBJ[HW_IO_TRANSMIT_VALUE].data[0]), card_nr, component_id, component_nr);
	  break;
    case IO_EKU_ID:
	  length = IO_EKU_Component_PDO_Transmit((unsigned char *)&(CAN_HWOBJ[HW_IO_TRANSMIT_VALUE].data[0]), card_nr, component_id, component_nr);
	  break;
    case IO_H2MC_ID:
    case IO_H2MC_ID + 1:
	  length = IO_H2MC_Component_PDO_Transmit((unsigned char *)&(CAN_HWOBJ[HW_IO_TRANSMIT_VALUE].data[0]), card_nr, component_id, component_nr);
	  card_id += card_nr / 32;
	  card_nr %= 32;
	  break;
    case IO_H1MC_ID:
	  length = IO_H1MC_Component_PDO_Transmit((unsigned char *)&(CAN_HWOBJ[HW_IO_TRANSMIT_VALUE].data[0]), card_nr, component_id, component_nr);
	  break;
    case IO_05_07_ID:
	  length = IO_05_07_Component_PDO_Transmit((unsigned char *)&(CAN_HWOBJ[HW_IO_TRANSMIT_VALUE].data[0]), card_nr, component_id, component_nr);
	  break;
    case IO_07_07_ID:
	  length = IO_07_07_Component_PDO_Transmit((unsigned char *)&(CAN_HWOBJ[HW_IO_TRANSMIT_VALUE].data[0]), card_nr, component_id, component_nr);
	  break;
	default:
	  length = 0;
	  break;
  }
  if (length)
  {
    CAN_HWOBJ[HW_IO_TRANSMIT_VALUE].AR = 0x00400000 + 
                                         ((unsigned long)priority << 26) + 
                                         ((unsigned long)component_id << 17) + ((unsigned long)component_nr << 12) +
                                         ((unsigned long)card_id << 5) + card_nr;
    CAN_HWOBJ[HW_IO_TRANSMIT_VALUE].CFGL = 0x0C + (length << 4);
    can_io_PDO_busy = 1;
    can_io_PDO_timeout = CAN_IO_TIMEOUT;
    CAN_HWOBJ[HW_IO_TRANSMIT_VALUE].CTRL = 0xF6BF; // reset MSGLST, set NEWDAT, set MSGVAL
    CAN_IO_Transmit(&CAN_HWOBJ[HW_IO_TRANSMIT_VALUE]);
  }
}

static void CAN_IO_SDO_Master_Transmit(unsigned char priority,
                                       unsigned char card_id, unsigned char card_nr, 
                                       unsigned char component_id, unsigned char component_nr)
{
unsigned char length;
// unsigned long id;

  if (can_io_SDO_master_busy) // this if should be placed in the procedure call
    return;
  switch (card_id)
  {
	case BROADCOAST_ID:
	  can_io_SDO_master_length = 0;
	  break;
    case IO_06_14_ID:
	  can_io_SDO_master_length = IO_06_14_Component_SDO_Master_Transmit(&can_io_SDO_master_data, card_nr, component_id, component_nr);
	  break;
    case IO_12_06_ID:
	  can_io_SDO_master_length = IO_12_06_Component_SDO_Master_Transmit(&can_io_SDO_master_data, card_nr, component_id, component_nr);
	  break;
    case IO_08_09_ID:
	  can_io_SDO_master_length = IO_08_09_Component_SDO_Master_Transmit(&can_io_SDO_master_data, card_nr, component_id, component_nr);
	  break;
    case IO_EKU_ID:
	  can_io_SDO_master_length = IO_EKU_Component_SDO_Master_Transmit(&can_io_SDO_master_data, card_nr, component_id, component_nr);
	  break;
    case IO_H2MC_ID:
    case IO_H2MC_ID + 1:
	  can_io_SDO_master_length = IO_H2MC_Component_SDO_Master_Transmit(&can_io_SDO_master_data, card_nr, component_id, component_nr);
	  card_id += card_nr / 32;
	  card_nr %= 32;
	  break;
    case IO_H1MC_ID:
	  can_io_SDO_master_length = IO_H1MC_Component_SDO_Master_Transmit(&can_io_SDO_master_data, card_nr, component_id, component_nr);
	  break;
    case IO_05_07_ID:
	  can_io_SDO_master_length = IO_05_07_Component_SDO_Master_Transmit(&can_io_SDO_master_data, card_nr, component_id, component_nr);
	  break;
    case IO_07_07_ID:
	  can_io_SDO_master_length = IO_07_07_Component_SDO_Master_Transmit(&can_io_SDO_master_data, card_nr, component_id, component_nr);
	  break;
	default:
	  can_io_SDO_master_length = 0;
	  break;
  }
  if (can_io_SDO_master_length == 0)
    return;

  can_io_SDO_master_busy = 1;
  can_io_SDO_master_timeout = CAN_IO_TIMEOUT;
  can_io_SDO_master_toggle_bit = 0;
  CAN_HWOBJ[HW_IO_TRANSMIT_MASTER].AR = 0x00C00000 + 
                                        ((unsigned long)priority << 26) + 
                                        ((unsigned long)component_id << 17) + ((unsigned long)component_nr << 12) +
                                        ((unsigned long)card_id << 5) + card_nr;
  if (can_io_SDO_master_length > 7) // check if 7 characters or more
  {
    length = 7;
    CAN_HWOBJ[HW_IO_TRANSMIT_MASTER].data[0] = 0x01; // SDO Write start send more then 7 bytes data
  }
  else
  {
	length = can_io_SDO_master_length;
    CAN_HWOBJ[HW_IO_TRANSMIT_MASTER].data[0] = 0x00; // SDO Write start send maximum 7 bytes with data
  }
  can_io_SDO_master_ptr = (unsigned char *)&can_io_SDO_master_data;
  Copy_N_Bytes((unsigned char *)&CAN_HWOBJ[HW_IO_TRANSMIT_MASTER].data[1],can_io_SDO_master_ptr,length);
  length <<= 4;			  
  CAN_HWOBJ[HW_IO_TRANSMIT_MASTER].data[0] += length;
  CAN_HWOBJ[HW_IO_TRANSMIT_MASTER].CFGL = 0x1C + length;
  CAN_HWOBJ[HW_IO_TRANSMIT_MASTER].CTRL = 0xF6BF; // reset MSGLST, set NEWDAT, set MSGVAL
  CAN_IO_Transmit(&CAN_HWOBJ[HW_IO_TRANSMIT_MASTER]);
}

static void CAN_IO_SDO_Master_Receive(unsigned char priority,
                                      unsigned char card_id, unsigned char card_nr, 
                                      unsigned char component_id, unsigned char component_nr)
{
  if (can_io_SDO_master_busy) // this if should be placed in the procedure call
    return;
  CAN_HWOBJ[HW_IO_TRANSMIT_MASTER].AR = 0x00C00000 + 
                                       ((unsigned long)priority << 26) + 
                                       ((unsigned long)component_id << 17) + ((unsigned long)component_nr << 12) +
                                       ((unsigned long)card_id << 5) + card_nr;
  can_io_SDO_master_busy = 1;
  can_io_SDO_master_timeout = CAN_IO_TIMEOUT;
  can_io_SDO_master_toggle_bit = 0;
  can_io_SDO_master_ptr = (unsigned char *)&can_io_SDO_master_data;
  CAN_HWOBJ[HW_IO_TRANSMIT_MASTER].data[0] = 0x06; // SDO Write start send more then 7 bytes data
  CAN_HWOBJ[HW_IO_TRANSMIT_MASTER].CFGL = 0x1C;
  CAN_HWOBJ[HW_IO_TRANSMIT_MASTER].CTRL = 0xF6BF; // reset MSGLST, set NEWDAT, set MSGVAL
  CAN_IO_Transmit(&CAN_HWOBJ[HW_IO_TRANSMIT_MASTER]);
}

void CAN_IO_Check_Communication(void)
{
static unsigned int can1_SDO_master_busy_cnt = 0;
static unsigned int can1_SDO_slave_busy_cnt = 0;

  if (can_io_SDO_master_busy)
  {
    if (can_io_SDO_master_timeout == 0) 
    {
      can1_SDO_master_busy_cnt++;
      can_io_SDO_master_busy = 0;
	  if (can1_SDO_master_busy_cnt > 3)
	  {
	    // delete message that not can be sent and set error flag
		// errror melding creeren
        can_io_init_switch = 1;
	    can1_SDO_master_busy_cnt = 0;
	  }
	}
  }
  else if (can_io_SDO_master_timeout != 0)
    can1_SDO_master_busy_cnt = 0;

  if (can_io_PDO_busy && (can_io_PDO_timeout == 0))
	can_io_init_switch = 1;
}

/****************************************************************************************************/

unsigned char CAN_IO_Check_PDO_Component(void)
{
static unsigned char nr = 0;
unsigned char old_nr = nr;

   if (can_io_PDO_busy)
     return (1);
   do
   {
     switch (nr)
     {
	   case 0: // IO_06_14
	     nr = 1;
	     if (IO_06_14_CAN_Check_PDO_Component(CAN_IO_PDO_Transmit))
		   return (1);
	     break;
	   case 1: // IO_12_06
	     nr = 2;
	     if (IO_12_06_CAN_Check_PDO_Component(CAN_IO_PDO_Transmit))
		   return (1);
	     break;
	   case 2: // IO_08_09
	     nr = 3;
	     if (IO_08_09_CAN_Check_PDO_Component(CAN_IO_PDO_Transmit))
		   return (1);
	     break;
	   case 3: // IO_EKU
	     nr = 4;
	     if (IO_EKU_CAN_Check_PDO_Component(CAN_IO_PDO_Transmit))
		   return (1);
	     break;
	   case 4: // IO_H2MC
	     nr = 5;
	     if (IO_H2MC_CAN_Check_PDO_Component(CAN_IO_PDO_Transmit))
		   return (1);
	     break;
	   case 5: // IO_H1MC
	     nr = 6;
	     if (IO_H1MC_CAN_Check_PDO_Component(CAN_IO_PDO_Transmit))
		   return (1);
	     break;
	   case 6: // IO_05_07
	     nr = 7;
	     if (IO_05_07_CAN_Check_PDO_Component(CAN_IO_PDO_Transmit))
		   return (1);
	     break;
	   case 7: // IO_07_07
	     nr = 0;
	     if (IO_07_07_CAN_Check_PDO_Component(CAN_IO_PDO_Transmit))
		   return (1);
	     break;
     }
   }
   while (old_nr != nr);
   return (0);

}
/****************************************************************************************************/


unsigned char CAN_IO_Check_SDO_Master_Component(void)
{
static unsigned char nr = 0;
unsigned char old_nr = nr;

   if (can_io_SDO_master_busy)
     return (1);

   do
   {
     switch (nr)
     {
	   case 0: 
	     nr = 1;
	     if (IO_06_14_CAN_Check_SDO_Master_Component(CAN_IO_SDO_Master_Transmit, CAN_IO_SDO_Master_Receive))
		   return (1);
	     break;
	   case 1: 
	     nr = 2;
	     if (IO_12_06_CAN_Check_SDO_Master_Component(CAN_IO_SDO_Master_Transmit, CAN_IO_SDO_Master_Receive))
		   return (1);
	     break;
	   case 2: 
	     nr = 3;
	     if (IO_08_09_CAN_Check_SDO_Master_Component(CAN_IO_SDO_Master_Transmit, CAN_IO_SDO_Master_Receive))
		   return (1);
	     break;
	   case 3: 
	     nr = 4;
	     if (IO_EKU_CAN_Check_SDO_Master_Component(CAN_IO_SDO_Master_Transmit, CAN_IO_SDO_Master_Receive))
		   return (1);
	     break;
	   case 4: 
	     nr = 5;
	     if (IO_H2MC_CAN_Check_SDO_Master_Component(CAN_IO_SDO_Master_Transmit, CAN_IO_SDO_Master_Receive))
		   return (1);
	     break;
	   case 5: 
	     nr = 6;
	     if (IO_H1MC_CAN_Check_SDO_Master_Component(CAN_IO_SDO_Master_Transmit, CAN_IO_SDO_Master_Receive))
		   return (1);
	     break;
	   case 6: 
	     nr = 7;
	     if (IO_05_07_CAN_Check_SDO_Master_Component(CAN_IO_SDO_Master_Transmit, CAN_IO_SDO_Master_Receive))
		   return (1);
	     break;
	   case 7: 
	     nr = 0;
	     if (IO_07_07_CAN_Check_SDO_Master_Component(CAN_IO_SDO_Master_Transmit, CAN_IO_SDO_Master_Receive))
		   return (1);
	     break;
     }
   }
   while (old_nr != nr);
   return (0);
}

/****************************************************************************************************/
void CAN_IO_Init_All_Boards(void)
{
unsigned char loop;

  for (loop = 0; loop < IO_06_14_MAX; loop++)
  {
    if (opt_io.IO_06_14[loop].board_component.option & 0x000F)
      IO_06_14_init_switch[loop] = 1;
  }
  for (loop = 0; loop < IO_12_06_MAX; loop++)
  {
    if (opt_io.IO_12_06[loop].board_component.option & 0x000F)
      IO_12_06_init_switch[loop] = 1;
  }
  for (loop = 0; loop < IO_08_09_MAX; loop++)
  {
    if (opt_io.IO_08_09[loop].board_component.option & 0x000F)
      IO_08_09_init_switch[loop] = 1;
  }
  for (loop = 0; loop < IO_EKU_MAX; loop++)
  {
    if (opt_io.IO_EKU[loop].board_component.option & 0x000F)
      IO_EKU_init_switch[loop] = 1;
  }
  for (loop = 0; loop < IO_H2MC_MAX; loop++)
  {
    if (opt_io.IO_H2MC[loop].board_component.option & 0x000F)
      IO_H2MC_init_switch[loop] = 1;
  }
  for (loop = 0; loop < IO_H1MC_MAX; loop++)
  {
    if (opt_io.IO_H1MC[loop].board_component.option & 0x000F)
      IO_H1MC_init_switch[loop] = 1;
  }
  for (loop = 0; loop < IO_05_07_MAX; loop++)
  {
    if (opt_io.IO_05_07[loop].board_component.option & 0x000F)
      IO_05_07_init_switch[loop] = 1;
  }
  for (loop = 0; loop < IO_07_07_MAX; loop++)
  {
    if (opt_io.IO_07_07[loop].board_component.option & 0x000F)
      IO_07_07_init_switch[loop] = 1;
  }
}


void CAN_IO_Board_Init(void)
// initialiseer alle borden die op CAN 1 zijn aangesloten
{
unsigned char board_present = 0;
unsigned char loop;

  for (loop = 0; loop < IO_06_14_MAX; loop++)
  {
    if (opt_io.IO_06_14[loop].board_component.option & 0x000F)
    {
      board_present = 1;
      if (IO_06_14_init_switch[loop])
        IO_06_14_Init(loop);
    }
  }
  for (loop = 0; loop < IO_12_06_MAX; loop++)
  {
    if (opt_io.IO_12_06[loop].board_component.option & 0x000F)
    {
      board_present = 1;
      if (IO_12_06_init_switch[loop])
        IO_12_06_Init(loop);
    }
  }
  for (loop = 0; loop < IO_08_09_MAX; loop++)
  {
    if (opt_io.IO_08_09[loop].board_component.option & 0x000F)
    {
      board_present = 1;
      if (IO_08_09_init_switch[loop])
        IO_08_09_Init(loop);
    }
  }
  for (loop = 0; loop < IO_EKU_MAX; loop++)
  {
    if (opt_io.IO_EKU[loop].board_component.option & 0x000F)
    {
      board_present = 1;
      if (IO_EKU_init_switch[loop])
        IO_EKU_Init(loop);
    }
  }
  for (loop = 0; loop < IO_H2MC_MAX; loop++)
  {
    if (opt_io.IO_H2MC[loop].board_component.option & 0x000F)
    {
      board_present = 1;
      if (IO_H2MC_init_switch[loop])
        IO_H2MC_Init(loop);
    }
  }
  for (loop = 0; loop < IO_H1MC_MAX; loop++)
  {
    if (opt_io.IO_H1MC[loop].board_component.option & 0x000F)
    {
      board_present = 1;
      if (IO_H1MC_init_switch[loop])
        IO_H1MC_Init(loop);
    }
  }
  for (loop = 0; loop < IO_05_07_MAX; loop++)
  {
    if (opt_io.IO_05_07[loop].board_component.option & 0x000F)
    {
      board_present = 1;
      if (IO_05_07_init_switch[loop])
        IO_05_07_Init(loop);
    }
  }
  for (loop = 0; loop < IO_07_07_MAX; loop++)
  {
    if (opt_io.IO_07_07[loop].board_component.option & 0x000F)
    {
      board_present = 1;
      if (IO_07_07_init_switch[loop])
        IO_07_07_Init(loop);
    }
  }
  if (!board_present)
    can_io_alarm = 0;
}

/****************************************************************************************************/

void CAN_IO_Diagnose_Msg(void)
{
static unsigned int txok[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, };
static unsigned int rxok[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, };
static int index = 0;

  if (can_io_diag_reset_flag)
  {
    s_can_state_cnt can_IO_state_empty = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    can_io_diag_reset_flag = 0;
    can_IO_state = can_IO_state_empty;
    can_io_txok = 0;
    can_io_txok_peak = 0;
    can_io_rxok = 0;
    can_io_rxok_peak = 0;
    can_io_txok_rxok = 0;
    can_io_txok_rxok_peak = 0;
    for (index = 0; index < 10; index++)
      txok[index] = rxok[index] = 0;
    index = 0;  
  }
  else
  {
    index %= 10;
    can_io_txok -= txok[index];
    can_io_rxok -= rxok[index];
    txok[index] = can_IO_state.TXOK;
    rxok[index] = can_IO_state.RXOK;
    can_IO_state.RXOK = can_IO_state.TXOK = 0;
    can_io_txok += txok[index];
    can_io_rxok += rxok[index];
    index++;
    can_io_txok_rxok = can_io_txok + can_io_rxok;
    if(can_io_txok > can_io_txok_peak)
      can_io_txok_peak = can_io_txok;
    if(can_io_rxok > can_io_rxok_peak)
      can_io_rxok_peak = can_io_rxok;
    if(can_io_txok_rxok > can_io_txok_rxok_peak)
      can_io_txok_rxok_peak = can_io_txok_rxok;
  }    
}

void CAN_IO_100ms(void)
{
unsigned char loop;

  if (can_io_SDO_master_busy && can_io_SDO_master_timeout)
    can_io_SDO_master_timeout--;
  if (can_io_PDO_busy && can_io_PDO_timeout)
    can_io_PDO_timeout--;
  for (loop = 0; loop < IO_06_14_MAX; loop++)
    IO_06_14_100ms(loop);
  for (loop = 0; loop < IO_12_06_MAX; loop++)
    IO_12_06_100ms(loop);
  for (loop = 0; loop < IO_08_09_MAX; loop++)
    IO_08_09_100ms(loop);
  for (loop = 0; loop < IO_EKU_MAX; loop++)
    IO_EKU_100ms(loop);
  for (loop = 0; loop < IO_H2MC_MAX; loop++)
    IO_H2MC_100ms(loop);
  for (loop = 0; loop < IO_H1MC_MAX; loop++)
    IO_H1MC_100ms(loop);
  for (loop = 0; loop < IO_05_07_MAX; loop++)
    IO_05_07_100ms(loop);
  for (loop = 0; loop < IO_07_07_MAX; loop++)
    IO_07_07_100ms(loop);
  CAN_IO_Diagnose_Msg();    
}

#ifdef ASC1_CAN_IO
void CAN_IO_Test(void)
{
static unsigned int msg_hw_lost[15] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
static s_can_state_cnt state = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int loop;

  if (state.ack_error != can_IO_state.ack_error)
  {
	ASC1_Printf("CAN IO ack error %i\r", can_IO_state.ack_error); 
	state.ack_error = can_IO_state.ack_error;
  }
  else if (state.bit0_busoff_error != can_IO_state.bit0_busoff_error)
  {
	ASC1_Printf("CAN IO bit0 busoff error %i\r", can_IO_state.bit0_busoff_error); 
	state.bit0_busoff_error = can_IO_state.bit0_busoff_error;
  }
  else if (state.bit0_normal_error != can_IO_state.bit0_normal_error)
  {
	ASC1_Printf("CAN IO bit0 normal error %i\r", can_IO_state.bit0_normal_error); 
	state.bit0_busoff_error = can_IO_state.bit0_busoff_error;
  }
  else if (state.bit1_error != can_IO_state.bit1_error)
  {
	ASC1_Printf("CAN IO bit1 error %i\r", can_IO_state.bit1_error); 
	state.bit1_error = can_IO_state.bit1_error;
  }
  else if (state.bit1_error != can_IO_state.bit1_error)
  {
	ASC1_Printf("CAN IO bit1 error %i\r", can_IO_state.bit1_error); 
	state.bit1_error = can_IO_state.bit1_error;
  }
  else if (state.BOFF != can_IO_state.BOFF)
  {
	ASC1_Printf("CAN IO BOFF %i\r", can_IO_state.BOFF); 
	state.BOFF = can_IO_state.BOFF;
  }
  else if (state.crc_error != can_IO_state.crc_error)
  {
	ASC1_Printf("CAN IO crc error %i\r", can_IO_state.crc_error); 
	state.crc_error = can_IO_state.crc_error;
  }
  else if (state.EWRN != can_IO_state.EWRN)
  {
	ASC1_Printf("CAN IO EWRN %i\r", can_IO_state.EWRN); 
	state.EWRN = can_IO_state.EWRN;
  }
  else if (state.form_error != can_IO_state.form_error)
  {
	ASC1_Printf("CAN IO form error %i\r", can_IO_state.form_error); 
	state.form_error = can_IO_state.form_error;
  }
  else if (state.stuff_error != can_IO_state.stuff_error)
  {
	ASC1_Printf("CAN IO stuff error %i\r", can_IO_state.stuff_error); 
	state.stuff_error = can_IO_state.stuff_error;
  }
  else
  {
    for (loop = 0; loop < 15; loop++)
    {
      if (msg_hw_lost[loop] != can_io_msg_hw_lost[loop])
	  {
	    ASC1_Printf("CAN IO message lost %i %i\r",loop, can_io_msg_hw_lost[loop]); 
	    msg_hw_lost[loop] = can_io_msg_hw_lost[loop];
		break;
	  }
	}
  }
}
#endif

void CAN_IO_Proc(void)
{
  CAN_IO_Board_Init();
  CAN_IO_Check_PDO_Component();
  CAN_IO_Check_SDO_Master_Component();
  CAN_IO_Check_Communication();
}