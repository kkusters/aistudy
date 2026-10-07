// CH_CAN_IO.H

#ifndef _CH_CAN_IO_H
#define _CH_CAN_IO_H

#include "ch_define.h"
#include "ch_asc1.h"
#include "ch_can.h"

typedef union
{
  unsigned int i[2];
  unsigned long l;
} u_ulong;

typedef struct
{
  unsigned int intid; // counts how many times interrupt is entered
  unsigned int BOFF;
  unsigned int EWRN;
  unsigned int TXOK;
  unsigned int RXOK;
  unsigned int stuff_error;
  unsigned int form_error;
  unsigned int ack_error;
  unsigned int bit1_error;
  unsigned int bit0_busoff_error;
  unsigned int bit0_normal_error;
  unsigned int crc_error;
} s_can_state_cnt;

extern bit can_io_alarm;
extern bit can_io_init_switch;
extern s_can_state_cnt can_IO_state;

extern unsigned char can_io_diag_reset_flag;
extern unsigned int can_io_txok;
extern unsigned int can_io_txok_peak;
extern unsigned int can_io_rxok;
extern unsigned int can_io_rxok_peak;
extern unsigned int can_io_txok_rxok;
extern unsigned int can_io_txok_rxok_peak;

_inline void Enable_Can_IO_Int(void)
{
  CAN_1IC_IE = 1;
}

_inline void Disable_Can_IO_Int(void)
{
  CAN_1IC_IE = 0;
}

/****************************************************************************************************/

void CAN_IO_Init(void);

void CAN_IO_Init_All_Boards(void);
void CAN_IO_Board_Init(void);	// init alle borden aangesloten op CAN1
void CAN_IO_100ms(void);
void CAN_IO_Proc(void);
#ifdef ASC1_CAN_IO
void CAN_IO_Test(void);
#endif

#endif
