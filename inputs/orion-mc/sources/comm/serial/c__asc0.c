// C__ASC0.C

#include "ch_define.h"

#include "ch_alg.h"
#include "ch_rs232_0.h"
#include "ch_ASC0.h"

#define BUFFER_SIZE 1024

// Communication not testing on overflow of the buffers
// No control build in of communication is realy working

unsigned char asc0_receive_cnt;
unsigned char asc0_transmit_state;
unsigned char asc0_send_at = 0;
int           asc0_send_at_delayed = 0;
unsigned char asc0_modem_init_state = 0;
unsigned char asc0_modem_init = 0;

static char asc0_in_buffer[BUFFER_SIZE];
static char asc0_out_buffer[BUFFER_SIZE];
unsigned char        asc0_init_switch = 1;
static unsigned int  asc0_in_cnt;
static unsigned int  asc0_in_get_index;
static unsigned int  asc0_in_put_index;
static unsigned int  asc0_out_cnt;
static unsigned int  asc0_out_get_index;
static unsigned int  asc0_out_put_index;
static unsigned char asc0_S0TBUF_full; // 0 als transmitbuffer leeg is; 1 = transmit buffer vol      

unsigned char asc0_diag_reset_flag = 0;
unsigned int  asc0_diag_in_cnt = 0;
unsigned int  asc0_diag_in_cnt_peak = 0;
unsigned int  asc0_diag_out_cnt = 0;
unsigned int  asc0_diag_out_cnt_peak = 0;
unsigned int  asc0_diag_in_out_cnt = 0;
unsigned int  asc0_diag_in_out_cnt_peak = 0;

#define ASC0_TD P3_10
#define ASC0_TD_DP DP3_10
#define ASC0_TD_ODP ODP3_10
#define ASC0_TD_ALTSEL0 AS0P3_10
#define ASC0_RD P3_11
#define ASC0_RD_DP DP3_11
#define ASC0_RD_ODP ODP3_11
#define ASC0_RD_ALTSEL0 AS0P3_11

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

void ASC0_Init(void)
{
  asc0_in_get_index = asc0_in_put_index = 0;
  asc0_out_get_index = asc0_out_put_index = 0;
  asc0_in_cnt = 0;
  asc0_out_cnt = 0;
  asc0_S0TBUF_full = 0;

  switch (opt_alg.com1_bd)
  {
    default:
    case   96: ASC0_FDV = SET_FDV_9K6;   ASC0_BG = SET_BG_9K6;   break;
    case  192: ASC0_FDV = SET_FDV_19K2;  ASC0_BG = SET_BG_19K2;  break;
    case  384: ASC0_FDV = SET_FDV_38K4;  ASC0_BG = SET_BG_38K4;  break;
    case  576: ASC0_FDV = SET_FDV_57K6;  ASC0_BG = SET_BG_57K6;  break;
    case 1152: ASC0_FDV = SET_FDV_115K2; ASC0_BG = SET_BG_115K2; break;
  }
  // 8-bit data aysnchronous operation (bit 2=0; 1=0; 0=1)
  // one stop bit (bit 3=1)
  // Receiver enabled (bit 4=1)
  // Parity disabled (bit 5=0)
  // Frame check disabled (bit 6=1)
  // Overrun check disabled (bit 7=1)
  // Clear parity error flag (bit 8=0)
  // Clear Framing error flag (bit 9=0)
  // Clear Overrun error flag (bit 10=0)
  // Parity selection (bit 12=x)
  // Baudrate Selection S0BRS (bit 13=0)
  // Loopback (bit 14=0)
  // Baudrate generator (bit 15=0) Communication is off
  ASC0_CON = 0x08D1; // load ASC0 control register

  ASC0_RXFCON = 0x0807; // load ASC1 receive FIFO control register (FIFO transparent mode)
//  ASC0_RXFCON = 0x0102; // load ASC0 receive FIFO control register
  ASC0_TXFCON = 0x0102; // load ASC0 transmit FIFO control register

  ASC0_TD_ALTSEL0 = 1;
  ASC0_TD = 1;
  ASC0_TD_DP = OUTPUT;
  ASC0_TD_ODP = PUSH_PULL;
  ASC0_RD_ALTSEL0 = 0;
  ASC0_RD = 1;
  ASC0_RD_DP = INPUT;
  ASC0_RD_ODP = PUSH_PULL;
  POCON3 = POCON3 & 0xF0FF; // high current outputs P3.8-P3.11

  // Interrupt level transmit interrupt
//  ASC0_TIC = ASC0_TRANSMIT_INT_LEVEL; // was commentaar 11-01-07
  // Interrupt level transmit buffer interrupt
  S0TBIC = ASC0_TRANSMITBUFFER_INT_LEVEL;     
  // Interrupt level receive interrupt
  S0RIC = ASC0_RECEIVE_INT_LEVEL;     
  // Interrupt level error interrupt
  S0EIC = ASC0_ERROR_INT_LEVEL;     

//  ASC0_TIC_IE = 1; // was commentaar 11-01-07
  if (opt_alg.com1_enabled)
  {
    S0TBIE = 1;
    S0TIE = 0;
    S0RIE = 1;
    S0EIE = 1;
    S0R = 1;
  }
  else
  {
    S0TBIE = 0;
    S0TIE = 0;
    S0RIE = 0;
    S0EIE = 0;
    S0R = 0;
  }
  if (opt_alg.com1_modem)
    asc0_send_at = 1;
  asc0_init_switch = 0;
}

interrupt ASC0_RECEIVE_INT_ADR using(ASC0_RECEIVE_INT_RB) void ASC0_Int_Receive(void) // S0RINT
{
static unsigned char state = 0;
char ch;

  ch = S0RBUF;
  asc0_in_cnt++;
  switch (ch)
  {
    case STX:
      state = 1; // start karakter ontvangen
      asc0_in_buffer[asc0_in_put_index] = ch;
      asc0_in_put_index++;
      asc0_in_put_index %= BUFFER_SIZE;
      break;
    case ETX:
      if (state == 1)
      {
        state = 0;
        asc0_receive_cnt++;
        asc0_in_buffer[asc0_in_put_index] = ch;
        asc0_in_put_index++;
        asc0_in_put_index %= BUFFER_SIZE;
      }
      break;
    default:
      if (state == 1)
      {
        asc0_in_buffer[asc0_in_put_index] = ch;
        asc0_in_put_index++;
        asc0_in_put_index %= BUFFER_SIZE;
      }
      break;
  }
}

void ASC0_Control(void)
{
// JP 06-10-09
static unsigned int bd_old = 0xFFFF;
static unsigned char modem_old = 0xFF;
static unsigned char enabled_old = 0xFF;

  if ((bd_old != opt_alg.com1_bd) ||
      (modem_old != opt_alg.com1_modem) ||
      (enabled_old != opt_alg.com1_enabled))
  {    
    asc0_init_switch = 1;
    bd_old = opt_alg.com1_bd;
    modem_old = opt_alg.com1_modem;
    enabled_old = opt_alg.com1_enabled;
  }    
  // end JP 06-10-09
  if (asc0_init_switch)
  {
    asc0_receive_cnt = 0;
    asc0_transmit_state = TRANSMIT_READY;
    RS232_0_Init();
    ASC0_Init();
  }  
}

interrupt ASC0_TRANSMITBUFFER_INT_ADR using(ASC0_TRANSMITBUFFER_INT_RB) void ASC0_Int_Trans_Buf(void) // S0TBINT
{
  if (asc0_out_get_index != asc0_out_put_index)
  {
    asc0_out_cnt++;
    asc0_S0TBUF_full = 1;
    S0TBUF = asc0_out_buffer[asc0_out_get_index];
    asc0_out_get_index++;
    asc0_out_get_index %= BUFFER_SIZE;
  }
  else
  {
    asc0_S0TBUF_full = 0;
    asc0_transmit_state = TRANSMIT_READY;
  }
}

interrupt ASC0_ERROR_INT_ADR using(ASC0_ERROR_INT_RB) void ASC0_Int_Error(void) // S0EINT
{
  if (S0PE) // Parity error
    S0PE = 0;
  if (S0FE) // Framing error
    S0FE = 0;
  if (S0OE) // Overrun error
    S0OE = 0;
  asc0_init_switch = 1;
}

char ASC0_Get_Char(void)
// read one character
{
char ch;

  if (asc0_in_get_index != asc0_in_put_index)
  {
    ch = asc0_in_buffer[asc0_in_get_index];
    asc0_in_get_index++;
    asc0_in_get_index %= BUFFER_SIZE;
    return (ch);
  }
  return (0);
}

char *ASC0_Get_String(char *string)
// read a character string who is ended on '/0'
// string is ended with '/0'
{
char *str = string;

  *str = ASC0_Get_Char();
  while (*str)
  {
    str++;
    *str = ASC0_Get_Char();
  }
  return (string);
}

char *ASC0_Get_String_Nr(char *string, unsigned int max)
// read a character string from max bytes 
// string is not ended
{
char *str = string;
unsigned int nr = 1;

  *str = ASC0_Get_Char();
  while (*str && (nr < max))
  {
    str++;
    nr++;
    *str = ASC0_Get_Char();
  }
  return (string);
}

void ASC0_Put_Char(char ch)
// write one character byte
{
  S0TBIE = 0; // JP 22-01-2007
  asc0_out_buffer[asc0_out_put_index] = ch;
  asc0_out_put_index++;
  asc0_out_put_index %= BUFFER_SIZE;
  if (!asc0_S0TBUF_full)
  {
    asc0_transmit_state = TRANSMIT_BUSY;
    S0TBIR = 1;
  }
  S0TBIE = 1; // JP 22-01-2007
}

void ASC0_Put_String(char *string)
// write a character string ended with '/0'
// don't send '/0' character
{
  while (*string)
  {
    ASC0_Put_Char(*string);
    string++;
  }
}

void ASC0_Printf(const char *format, ... )
{
va_list ap;
char data[256];

  va_start(ap, format);
  vsprintf(data, format, ap);
  va_end(ap);
  ASC0_Put_String(data); 
}

void ASC0_Timing_Control(void)
// wordt elke 100ms aangeroepen
{
static unsigned int in_cnt[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
static unsigned int out_cnt[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
static unsigned long in_total_cnt = 0;
static unsigned long out_total_cnt = 0;
static int index = 0;

  if (opt_alg.com1_modem)
  {
    if (asc0_send_at_delayed < (PULSES_PER_SECOND*5))
    {
      asc0_send_at_delayed++;
      if (asc0_send_at_delayed == (PULSES_PER_SECOND*5))
        asc0_send_at = 1;
    }
    if (asc0_modem_init)
    {
      switch (asc0_modem_init_state)
      {
        case  0: ASC0_Put_String("AT&F\r"); break;
        case 10: ASC0_Put_String("ATE0\r"); break;
        case 20: ASC0_Put_String("ATQ1\r"); break;
        case 30: ASC0_Put_String("AT&D0\r"); break;
        case 40: ASC0_Put_String("AT&C1\r"); break;
        case 50: ASC0_Printf("ATS0=%i\r",opt_alg.com1_modem_answer); break;
        case 60: ASC0_Put_String("AT&K0\r"); break;
        case 70: ASC0_Put_String("AT&W\r"); break;
      }
      asc0_modem_init_state++;
      if (asc0_modem_init_state > 80)
      {
        asc0_modem_init_state = 0;
        asc0_modem_init = 0;
      }  
    }
    else
      asc0_modem_init_state = 0;
    if (asc0_send_at)
    {
      asc0_send_at = 0;
      ASC0_Put_String("AT\r");
    }
  }
  else
  {
    asc0_modem_init = 0;
    asc0_modem_init_state = 0;
    asc0_send_at = 0;
    asc0_send_at_delayed = (PULSES_PER_SECOND*5);
  }  
  if (asc0_diag_reset_flag)
  {
    asc0_diag_reset_flag = 0;
    asc0_in_cnt = 0;
    asc0_out_cnt = 0;
    asc0_diag_in_cnt = 0;
    asc0_diag_in_cnt_peak = 0;
    asc0_diag_out_cnt = 0;
    asc0_diag_out_cnt_peak = 0;
    asc0_diag_in_out_cnt = 0;
    asc0_diag_in_out_cnt_peak = 0;
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
    in_cnt[index] = asc0_in_cnt;
    out_cnt[index] = asc0_out_cnt;
    asc0_in_cnt = asc0_out_cnt = 0;
    in_total_cnt += in_cnt[index];
    out_total_cnt += out_cnt[index];
    index++;
    asc0_diag_in_cnt = (in_total_cnt + (PULSES_PER_SECOND / 2)) / PULSES_PER_SECOND;
    asc0_diag_out_cnt = (out_total_cnt + (PULSES_PER_SECOND / 2)) / PULSES_PER_SECOND;
    asc0_diag_in_out_cnt = asc0_diag_in_cnt + asc0_diag_out_cnt;
    if (asc0_diag_in_cnt > asc0_diag_in_cnt_peak)
      asc0_diag_in_cnt_peak = asc0_diag_in_cnt;
    if (asc0_diag_out_cnt > asc0_diag_out_cnt_peak)
      asc0_diag_out_cnt_peak = asc0_diag_out_cnt;
    if (asc0_diag_in_out_cnt > asc0_diag_in_out_cnt_peak)
      asc0_diag_in_out_cnt_peak = asc0_diag_in_out_cnt;
  }
}