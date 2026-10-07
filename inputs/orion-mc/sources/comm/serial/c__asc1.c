// C__ASC1.C

#include "ch_define.h"
#include "ch_rs232_1.h"
#include "ch_asc1.h"

// Communication only possible in half duplex mode
// Default direction is for receive data
// Then communication is switch from receive to transmitting 
// Transmitting only after receive a command
//
// See application AP165101
//
// Communication not testing on overflow of the buffers
// No control build in of communication is realy working

#define BUFFER_SIZE 1024

unsigned char asc1_receive_cnt;
unsigned char asc1_transmit_state = TRANSMIT_READY; // status uitgaangde data
unsigned char asc1_send_at = 0;
int asc1_send_at_delayed = 0;
unsigned char asc1_modem_dcd = 0;
unsigned char asc1_modem_init_state = 0;
unsigned char asc1_modem_init = 0;

static char          asc1_in_buffer[BUFFER_SIZE];
static char          asc1_out_buffer[BUFFER_SIZE];
unsigned char        asc1_init_switch = 1;
static unsigned int  asc1_in_cnt;
static unsigned int  asc1_in_get_index;
static unsigned int  asc1_in_put_index;
static unsigned int  asc1_out_cnt;
static unsigned int  asc1_out_get_index;
static unsigned int  asc1_out_put_index;
static unsigned char asc1_transmit_flag; // 0 = receive data; 1 = transmit data

unsigned char asc1_diag_reset_flag = 0;
unsigned int asc1_diag_in_cnt = 0;
unsigned int asc1_diag_in_cnt_peak = 0;
unsigned int asc1_diag_out_cnt = 0;
unsigned int asc1_diag_out_cnt_peak = 0;
unsigned int asc1_diag_in_out_cnt = 0;
unsigned int asc1_diag_in_out_cnt_peak = 0;

#define ASC1_TD P3_0
#define ASC1_TD_DP DP3_0
#define ASC1_TD_ODP ODP3_0
#define ASC1_TD_ALTSEL0 AS0P3_0
#define ASC1_RD P3_1
#define ASC1_RD_DP DP3_1
#define ASC1_RD_ODP ODP3_1
#define ASC1_RD_ALTSEL0 AS0P3_1
#define ASC1_RD_ALTSEL1 AS1P3_1

#define ASC1_DCD P6_5
#define ASC1_DCD_DP DP6_5
#define ASC1_DCD_ODP ODP6_5
#define ASC1_DCD_ALTSEL0 AS0P6_5

#define SET_FDV_9K6 0x0074
#define SET_BG_9K6 0x003A
#define SET_FDV_19K2 0x00E8
#define SET_BG_19K2 0x003A
#define SET_FDV_38K4 0x01D0
#define SET_BG_38K4 0x003A
#define SET_FDV_57K6 0x01CC
#define SET_BG_57K6 0x0026
#define SET_FDV_115K2 0x0191
#define SET_BG_115K2 0x0010

unsigned char asc1_dcd;

unsigned char asc1_TBUF_full; // 0 als transmitbuffer leeg is; 1 = transmit buffer vol      

static void ASC1_DCD_Init(void)
{
  ASC1_DCD_ALTSEL0 = 1;
  ASC1_DCD = 1;
  ASC1_DCD_DP = INPUT;
  ASC1_DCD_ODP = PUSH_PULL;

  CC1_IOC = 0x0004; // load CAPCOM1 I/O control register

  CC1_M1 = CC1_M1 & 0xFF0F | 0x0030; // load CAPCOM1 mode register 1

  CC1_CC5IC = ASC1_DCD_INT_LEVEL;     
  CC1_CC5IC_IE = 1;
  asc1_dcd = ASC1_DCD;
}

interrupt ASC1_DCD_INT_ADR using(ASC1_DCD_INT_RB) void ASC1_DCD_Interrupt(void)
{
  asc1_dcd = ASC1_DCD;
  asc1_send_at_delayed = 0;
}

static void ASC1_Init(void)
{
  asc1_in_get_index = asc1_in_put_index = 0;
  asc1_out_get_index = asc1_out_put_index = 0;
  asc1_in_cnt = 0;
  asc1_out_cnt = 0;
  asc1_TBUF_full = 0;

  switch (opt_alg.com2_bd) // straks wijzigen in optie voor com twee
  {
    default:
    case   96: ASC1_FDV = SET_FDV_9K6;   ASC1_BG = SET_BG_9K6;   break;
    case  192: ASC1_FDV = SET_FDV_19K2;  ASC1_BG = SET_BG_19K2;  break;
    case  384: ASC1_FDV = SET_FDV_38K4;  ASC1_BG = SET_BG_38K4;  break;
    case  576: ASC1_FDV = SET_FDV_57K6;  ASC1_BG = SET_BG_57K6;  break;
    case 1152: ASC1_FDV = SET_FDV_115K2; ASC1_BG = SET_BG_115K2; break;
  }
  ASC1_CON = 0x08D1; // load ASC1 control register

  ASC1_RXFCON = 0x0807; // load ASC1 receive FIFO control register (FIFO transparent mode)
  ASC1_TXFCON = 0x0102; // load ASC1 transmit FIFO control register

  ASC1_TD_ALTSEL0 = 1;
  ASC1_TD = 1;
  ASC1_TD_DP = OUTPUT;
  ASC1_TD_ODP = PUSH_PULL;
  ASC1_RD_ALTSEL0 = 0;
  ASC1_RD_ALTSEL1 =0;
  ASC1_RD = 1;
  ASC1_RD_DP = INPUT;
  ASC1_RD_ODP = PUSH_PULL;
  POCON3 = POCON3 & 0xFFF0; // high current outputs P3.0-P3.1

  ASC1_TBIC = ASC1_TRANSMITBUFFER_INT_LEVEL;     
  ASC1_RIC = ASC1_RECEIVE_INT_LEVEL;     
  ASC1_EIC = ASC1_ERROR_INT_LEVEL;     

  if (opt_alg.com2_enabled)
  {
    ASC1_TBIC_IE = 1;
    ASC1_TIC_IE = 0;
    ASC1_RIC_IE = 1;
    ASC1_EIC_IE = 1;
    ASC1_CON_R = 1;
  }
  else
  {
    ASC1_TBIC_IE = 1;
    ASC1_TIC_IE = 0;
    ASC1_RIC_IE = 1;
    ASC1_EIC_IE = 1;
    ASC1_CON_R = 1;
  }
  
  asc1_init_switch = 0;
}

interrupt ASC1_RECEIVE_INT_ADR using(ASC1_RECEIVE_INT_RB) void ASC1_Int_Receive(void)
{
static unsigned char state = 0;
char ch;

  ch = ASC1_RBUF;
  asc1_in_cnt++;
  switch (ch)
  {
    case STX:
      state = 1; // start karakter ontvangen
      asc1_in_buffer[asc1_in_put_index] = ch;
      asc1_in_put_index++;
      asc1_in_put_index %= BUFFER_SIZE;
      break;
    case ETX:
      if (state == 1)
      {
        state = 0;
        asc1_receive_cnt++;
        asc1_in_buffer[asc1_in_put_index] = ch;
        asc1_in_put_index++;
        asc1_in_put_index %= BUFFER_SIZE;
      }
      break;
    default:
      if (state == 1)
      {
        asc1_in_buffer[asc1_in_put_index] = ch;
        asc1_in_put_index++;
        asc1_in_put_index %= BUFFER_SIZE;
      }
      break;
  }
}

interrupt ASC1_TRANSMITBUFFER_INT_ADR using(ASC1_TRANSMITBUFFER_INT_RB) void ASC1_Int_Trans_Buf(void) // S0TBINT
{
  if (asc1_out_get_index != asc1_out_put_index)
  {
    asc1_out_cnt++;
    asc1_TBUF_full = 1;
    ASC1_TBUF = asc1_out_buffer[asc1_out_get_index];
    asc1_out_get_index++;
    asc1_out_get_index %= BUFFER_SIZE;
  }
  else
  {
    asc1_TBUF_full = 0;
    asc1_transmit_state = TRANSMIT_READY;
  }
}

interrupt ASC1_ERROR_INT_ADR using(ASC1_ERROR_INT_RB) void ASC1_Int_Error(void) // S0EINT
{
  if (ASC1_CON_PE) // Parity error
    ASC1_CON_PE = 0;
  if (ASC1_CON_FE) // Framing error
    ASC1_CON_FE = 0;
  if (ASC1_CON_OE) // Overrun error
    ASC1_CON_OE = 0;
  asc1_init_switch = 1;
}

void ASC1_Put_Char(char ch)
// write one character byte
{
  ASC1_TBIC_IE = 0; // JP 19-12-2006
  asc1_out_buffer[asc1_out_put_index] = ch;
  asc1_out_put_index++;
  asc1_out_put_index %= BUFFER_SIZE;
  if (!asc1_TBUF_full)
  {
    asc1_transmit_state = TRANSMIT_BUSY;
    ASC1_TBIC_IR = 1;
  }
  ASC1_TBIC_IE = 1; // JP 19-12-2006
}

void ASC1_Control(void)
{
// JP 06-10-09
static unsigned int bd_old = 0xFFFF;
static unsigned char modem_old = 0xFF;
static unsigned char enabled_old = 0xFF;

  if ((bd_old != opt_alg.com2_bd) ||
      (modem_old != opt_alg.com2_modem) ||
      (enabled_old != opt_alg.com2_enabled))
  {    
    asc1_init_switch = 1;
    bd_old = opt_alg.com2_bd;
    modem_old = opt_alg.com2_modem;
    enabled_old = opt_alg.com2_enabled;
  }    
  // end JP 06-10-09
  if (asc1_init_switch)
  {
    asc1_receive_cnt = 0;
    asc1_transmit_state = TRANSMIT_READY;
    RS232_1_Init();
    ASC1_Init();
  }  
}

char ASC1_Get_Char(void)
// get one character
{
char ch;

  if (asc1_in_get_index != asc1_in_put_index)
  {
    ch = asc1_in_buffer[asc1_in_get_index];
    asc1_in_get_index++;
    asc1_in_get_index %= BUFFER_SIZE;
    return (ch);
  }
  return (0);
}

char *ASC1_Get_String(char *string)
// read a character string who is ended on '/0'
// string is ended with '/0'
{
char *str = string;

  *str = ASC1_Get_Char();
  while (*str)
  {
    str++;
    *str = ASC1_Get_Char();
  }
  return (string);
}

char *ASC1_Get_String_Nr(char *string, unsigned int max)
// read a character string from max bytes 
// string is not ended
{
char *str = string;
unsigned int nr = 1;

  *str = ASC1_Get_Char();
  while (*str && (nr < max))
  {
    str++;
	nr++;
    *str = ASC1_Get_Char();
  }
  return (string);
}

void ASC1_Put_String(char *string)
// write a character string ended with '/0'
// don't send '/0' character
{
  while (*string)
  {
    ASC1_Put_Char(*string);
	string++;
  }
}

void ASC1_Printf(const char *format, ... )
{
va_list ap;
char data[256];

  va_start(ap, format);
  vsprintf(data, format, ap);
  va_end(ap);
  ASC1_Put_String(data); 
}

void ASC1_Timing_Control(void)
// wordt elke 100ms aangeroepen
{
static char test_at_cts = 0;
static unsigned int in_cnt[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
static unsigned int out_cnt[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
static unsigned long in_total_cnt = 0;
static unsigned long out_total_cnt = 0;
static int index = 0;

  if (opt_alg.com2_modem)
  {
    if (asc1_send_at_delayed < (PULSES_PER_SECOND*5))
    {
      asc1_send_at_delayed++;
      if (asc1_send_at_delayed == (PULSES_PER_SECOND*5))
        asc1_send_at = 1;
    }
    if (asc1_modem_init)
    {
      switch (asc1_modem_init_state)
      {
        case  0: ASC1_Put_String("AT&F\r"); break;
        case 10: ASC1_Put_String("ATE0\r"); break;
        case 20: ASC1_Put_String("ATQ1\r"); break;
        case 30: ASC1_Put_String("AT&D0\r"); break;
        case 40: ASC1_Put_String("AT&C0\r"); break; // ASC1_Put_String("AT&C1\r"); break;
        case 50: ASC1_Printf("ATS0=%i\r",opt_alg.com2_modem_answer); break;
        case 60: ASC1_Put_String("AT&K0\r"); break;
        case 70: ASC1_Put_String("AT&W\r"); break;
        case 80: asc1_modem_dcd = 1; break;
      }
      asc1_modem_init_state++;
      if (asc1_modem_init_state > 80)
      {
        asc1_modem_init_state = 0;
        asc1_modem_init = 0;
      }  
   }
    else
      asc1_modem_init_state = 0;
    if (asc1_send_at)
    {
      asc1_send_at = 0;
      ASC1_Put_String("AT\r");
    }
    else if (asc1_modem_dcd)
    {
      if (asc1_dcd == 1)
      {
        if ((test_at_cts % (PULSES_PER_SECOND*2)) == 0)
          ASC1_Put_String("AT&C0\r");
        else if ((test_at_cts % (PULSES_PER_SECOND*2)) == (PULSES_PER_SECOND*1))
          ASC1_Put_String("AT&C1\r");
        test_at_cts++;
        if (test_at_cts > (PULSES_PER_SECOND*10))
        {
          test_at_cts = 0;
          asc1_modem_dcd = 0;
        }  
      }
      else
      {
        if (test_at_cts)
        {
          test_at_cts = 0;
          ASC1_Put_String("AT&W\r");
        }
        asc1_modem_dcd = 0;
        asc1_dcd = 0;
      }
    }
  }
  else
  {
    test_at_cts = 0;
    asc1_send_at = 0;
    asc1_send_at_delayed = (PULSES_PER_SECOND*5);
    asc1_modem_dcd = 0;
    asc1_modem_init = 0;
    asc1_modem_init_state = 0;
  }  
  if (asc1_diag_reset_flag)
  {
    asc1_diag_reset_flag = 0;
    asc1_in_cnt = 0;
    asc1_out_cnt = 0;
    asc1_diag_in_cnt = 0;
    asc1_diag_in_cnt_peak = 0;
    asc1_diag_out_cnt = 0;
    asc1_diag_out_cnt_peak = 0;
    asc1_diag_in_out_cnt = 0;
    asc1_diag_in_out_cnt_peak = 0;
    in_total_cnt = out_total_cnt = 0;
    for (index = 0; index < 10; index++)
      in_cnt[index] = out_cnt[index] = 0;
    index = 0;  
  }
  else
  {
    index %= PULSES_PER_SECOND;
    in_total_cnt -= in_cnt[index];
    out_total_cnt -= out_cnt[index];
    in_cnt[index] = asc1_in_cnt;
    out_cnt[index] = asc1_out_cnt;
    asc1_in_cnt = asc1_out_cnt = 0;
    in_total_cnt += in_cnt[index];
    out_total_cnt += out_cnt[index];
    index++;
    asc1_diag_in_cnt = (in_total_cnt + (PULSES_PER_SECOND / 2)) / PULSES_PER_SECOND;
    asc1_diag_out_cnt = (out_total_cnt + (PULSES_PER_SECOND / 2)) / PULSES_PER_SECOND;
    asc1_diag_in_out_cnt = asc1_diag_in_cnt + asc1_diag_out_cnt;
    if (asc1_diag_in_cnt > asc1_diag_in_cnt_peak)
      asc1_diag_in_cnt_peak = asc1_diag_in_cnt;
    if (asc1_diag_out_cnt > asc1_diag_out_cnt_peak)
      asc1_diag_out_cnt_peak = asc1_diag_out_cnt;
    if (asc1_diag_in_out_cnt > asc1_diag_in_out_cnt_peak)
      asc1_diag_in_out_cnt_peak = asc1_diag_in_out_cnt;
  }
}
