/**
 * \addtogroup helloworld
 * @{
 */

/**
 * \file
 *         An example of how to write uIP applications
 *         with protosockets.
 * \author
 *         Adam Dunkels <adam@sics.se>
 */

/*
 * This is a short example of how to write uIP applications using
 * protosockets.
 */

/*
 * We define the application state (struct ethernet_state) in the
 * hello-world.h file, so we need to include it here. We also include
 * uip.h (since this cannot be included in hello-world.h) and
 * <string.h>, since we use the memcpy() function in the code.
 */
#include <string.h>
#include "ch_define.h"
#ifdef ETHERNET

#include "clock.h"
#include "uip.h"
#include "ch_test.h"
#include "NetworkDevice.h"
#include "ch_cp2200.h"
#include "uip_timer.h"
#include "datalink.h"
#include "ch_ethernet_app.h"
#include "npdu.h"
#include "handlers.h"
#include "bacdef.h"
#include "bacint.h"
#include "net.h"
#include "tsm.h"

#pragma noclear
s_eth_status eth_status;
#pragma clear
#pragma default_attributes
unsigned char ethernet_diag_reset_flag = 0;
unsigned int ethernet_in_cnt = 0;
unsigned int ethernet_diag_in_cnt = 0;
unsigned int ethernet_diag_in_cnt_peak = 0;
unsigned int ethernet_out_cnt = 0;
unsigned int ethernet_diag_out_cnt = 0;
unsigned int ethernet_diag_out_cnt_peak = 0;
//uip_tcp_appstate_t eth;
struct timer no_ack_send_timer[UIP_CONNS];

static time_t last_seconds_1 = 0;
static time_t last_seconds_2 = 0;

static unsigned char ethernet_reset_eth(uip_tcp_appstate_t *s)
{
/*
  // weet niet of onderstaande if gedeelte noodzakelijk is. else gedeelte wel
  if ((uip_conns[0].appstate.connected == 0) &&
      (uip_conns[1].appstate.connected == 0))
  {    
    uip_conns[0].appstate.in_get_index = 0;
    uip_conns[0].appstate.in_put_index = 0;
    uip_conns[0].appstate.buffer_in[0] = 0;
    uip_conns[0].appstate.buffer_out[0] = 0;
    uip_conns[0].appstate.receive_cnt = 0;
    uip_conns[0].appstate.receive_flag = 0;
    uip_conns[0].appstate.send_state = ETH_SEND_NOT_BUSY;
    uip_conns[0].appstate.state = 0;
    uip_conns[1].appstate.in_get_index = 0;
    uip_conns[1].appstate.in_put_index = 0;
    uip_conns[1].appstate.buffer_in[0] = 0;
    uip_conns[1].appstate.buffer_out[0] = 0;
    uip_conns[1].appstate.receive_cnt = 0;
    uip_conns[1].appstate.receive_flag = 0;
    uip_conns[1].appstate.send_state = ETH_SEND_NOT_BUSY;
    uip_conns[1].appstate.state = 0;
    return (CP2200_Transmit_Reset()); // JP 05-06-08
  }
  else
*/
  {  
    s->in_get_index = 0;
    s->in_put_index = 0;
    s->buffer_in[0] = 0;
    s->buffer_out[0] = 0;
    s->receive_cnt = 0;
    s->receive_flag = 0;
    s->send_state = ETH_SEND_NOT_BUSY;
    s->state = 0;
    return (1);
  }  
}
/*
static void ethernet_reset_eth(uip_tcp_appstate_t *s)
{
  // weet niet of onderstaande if gedeelte noodzakelijk is. else gedeelte wel
  if ((uip_conns[0].appstate.connected == 0) &&
      (uip_conns[1].appstate.connected == 0))
  {    
    uip_conns[0].appstate.in_get_index = 0;
    uip_conns[0].appstate.in_put_index = 0;
    uip_conns[0].appstate.buffer_in[0] = 0;
    uip_conns[0].appstate.buffer_out[0] = 0;
    uip_conns[0].appstate.receive_cnt = 0;
    uip_conns[0].appstate.receive_flag = 0;
    uip_conns[0].appstate.send_state = ETH_SEND_NOT_BUSY;
    uip_conns[0].appstate.state = 0;
    uip_conns[1].appstate.in_get_index = 0;
    uip_conns[1].appstate.in_put_index = 0;
    uip_conns[1].appstate.buffer_in[0] = 0;
    uip_conns[1].appstate.buffer_out[0] = 0;
    uip_conns[1].appstate.receive_cnt = 0;
    uip_conns[1].appstate.receive_flag = 0;
    uip_conns[1].appstate.send_state = ETH_SEND_NOT_BUSY;
    uip_conns[1].appstate.state = 0;
    CP2200_Transmit_Reset(); // JP 05-06-08
  }
  else
  {  
    s->in_get_index = 0;
    s->in_put_index = 0;
    s->buffer_in[0] = 0;
    s->buffer_out[0] = 0;
    s->receive_cnt = 0;
    s->receive_flag = 0;
    s->send_state = ETH_SEND_NOT_BUSY;
    s->state = 0;
  }  
}
*/

static unsigned char ethernet_reset_eth_all(void)
{
unsigned char loop;

  for (loop = 0; loop < UIP_CONNS;loop++)
  {
    if (ethernet_reset_eth(&uip_conns[loop].appstate) == 0)
      return (0);
  }
  return (CP2200_Transmit_Reset()); // JP 24-01-08
}
/*
static void ethernet_reset_eth_all(void)
{
unsigned char loop;

  for (loop = 0; loop < UIP_CONNS;loop++)
  {
    ethernet_reset_eth(&uip_conns[loop].appstate);
  }
  CP2200_Transmit_Reset(); // JP 24-01-08
}
*/

unsigned char ethernet_init(void)
{
int loop;

  /* We start to listen for connections on TCP port 1000. */
  uip_listen(HTONS(opt_alg.ethernet_port));
  if (ethernet_reset_eth_all() == 0)
    return (0);
  for (loop = 0; loop < UIP_CONNS; loop++)  
    timer_set(&no_ack_send_timer[loop], CLOCK_SECOND * 15);  
  return (1);
}

static void Copy_Data(char *to, char *from, int len)
{
  while (len)
  {
    *to = *from;
    to++;
    from++;
    len--;
  }
  *to = 0;
}

void Ethernet_0_Put_String(char *string)
// write a character string ended with '/0'
// don't send '/0' character
{
char *ptr = uip_conns[0].appstate.buffer_out;

  if (*string != 0) // JP 04-02-09 allen iets versturen als er ook data in string staat
  {
    while (*string)
    {
      *ptr = *string;
      ptr++;
      string++;
    }
    *ptr = 0;
    uip_conns[0].appstate.send_state = ETH_SEND_ASK;
  }  
}

void Ethernet_1_Put_String(char *string)
// write a character string ended with '/0'
// don't send '/0' character
{
char *ptr = uip_conns[1].appstate.buffer_out;

  if (*string != 0) // JP 04-02-09 allen iets versturen als er ook data in string staat
  {
    while (*string)
    {
      *ptr = *string;
      ptr++;
      string++;
    }
    *ptr = 0;
    uip_conns[1].appstate.send_state = ETH_SEND_ASK;
  }  
}

void Ethernet_2_Put_String(char *string)
// write a character string ended with '/0'
// don't send '/0' character
{
char *ptr = uip_conns[2].appstate.buffer_out;

  if (*string != 0) // JP 04-02-09 allen iets versturen als er ook data in string staat
  {
    while (*string)
    {
      *ptr = *string;
      ptr++;
      string++;
    }
    *ptr = 0;
    uip_conns[2].appstate.send_state = ETH_SEND_ASK;
  }  
}

void Ethernet_3_Put_String(char *string)
// write a character string ended with '/0'
// don't send '/0' character
{
char *ptr = uip_conns[3].appstate.buffer_out;

  if (*string != 0) // JP 04-02-09 allen iets versturen als er ook data in string staat
  {
    while (*string)
    {
      *ptr = *string;
      ptr++;
      string++;
    }
    *ptr = 0;
    uip_conns[3].appstate.send_state = ETH_SEND_ASK;
  }  
}

void Ethernet_Put_String(unsigned char conn_nr, char *string)
// write a character string ended with '/0'
// don't send '/0' character
{
char *ptr = uip_conns[conn_nr].appstate.buffer_out;

  if (*string != 0) // JP 04-02-09 allen iets versturen als er ook data in string staat
  {
    while (*string)
    {
      *ptr = *string;
      ptr++;
      string++;
    }
    *ptr = 0;
    uip_conns[conn_nr].appstate.send_state = ETH_SEND_ASK;
  }  
}

#ifndef TEST_ASC_STANDAARD
static void Receive_Data(uip_tcp_appstate_t *s)
{
int length;
char *ptr;
char ch;

  ptr = uip_appdata;
  length = uip_datalen();
  while (length)
  {
    ch = *ptr;
    switch (ch)
    {
      case STX:
        s->state = 1;
        s->buffer_in[s->in_put_index] = ch;
        s->in_put_index++;
        s->in_put_index %= ETH_BUFFER;
        break;
      case ETX:
        if (s->state == 1)
        {
          s->state = 0;
          s->receive_cnt++;
          s->buffer_in[s->in_put_index] = ch;
          s->in_put_index++;
          s->in_put_index %= ETH_BUFFER;
        }
        break;
      default:
        if (s->state == 1)
        {
          s->buffer_in[s->in_put_index] = ch;
          s->in_put_index++;
          s->in_put_index %= ETH_BUFFER;
        }
        break;
    }
    ptr++;
    length--;
  }
  s->receive_flag = 1;
  eth_status.receive_cnt++;
  ethernet_in_cnt++;
}
#else // TEST_ASC_STANDAARD
static void Receive_Data(uip_tcp_appstate_t *s)
{
int length;
char *ptr;

  ptr = uip_appdata;
  length = uip_datalen();
  while (length)
  {
    s->buffer_in[s->in_put_index] = *ptr;
    s->in_put_index++;
    s->in_put_index %= ETH_BUFFER;
    ptr++;
    length--;
  }
  *ptr = 0;
  s->receive_flag = 1;
  eth_status.receive_cnt++;
  ethernet_in_cnt++;
}
#endif // TEST_ASC_STANDAARD

static void Send_Data(uip_tcp_appstate_t *s)
{
  Copy_Data(uip_appdata, s->buffer_out, strlen(s->buffer_out));
  s->send_state = ETH_SEND_BUSY;
  uip_send(uip_appdata, strlen(s->buffer_out));
  ethernet_out_cnt++;
}

void ethernet_appcall(void)
{
  uip_tcp_appstate_t *s = (uip_tcp_appstate_t *)&(uip_conn->appstate);

  if (s->close_connection == 1)
  {
    s->close_connection = 0;
    s->close_connection_timeout = 0;
    uip_close();
  }
  if (uip_connected())
  {
    s->connected = 1;
    s->send_state = ETH_SEND_NOT_BUSY;
    eth_status.connect_cnt++;
  }
  if (uip_closed())
  {
    s->connected = 0;
    ethernet_reset_eth(s);
    eth_status.closed_cnt++;
    eth_status.disconnect_cnt++;
  }
  if (uip_aborted())
  {
    s->connected = 0;
    ethernet_reset_eth(s);
    eth_status.aborted_cnt++;
    eth_status.disconnect_cnt++;
  }
  if (uip_timedout())
  {
    s->connected = 0;
    ethernet_reset_eth(s);
    eth_status.timedout_cnt++;
    eth_status.disconnect_cnt++;
  }
  if (s->connected)
  { 
    if (uip_acked())
    {
      eth_status.transmit_ack_cnt++;
      s->send_state = ETH_SEND_NOT_BUSY;
    }  
    if (uip_newdata())
      Receive_Data(s);
    if (uip_rexmit())
    {
      eth_status.retransmit_cnt++;
      if (s->retransmit_retry_cnt < 10)
        s->retransmit_retry_cnt++;
      if (s->send_state == ETH_SEND_BUSY)
        Send_Data(s); // JP test om te kijken of dit noodzakelijk is
    }  
    else if (uip_poll())
    {
      eth_status.poll_cnt++;
      if (s->send_state == ETH_SEND_ASK)
      {
        eth_status.transmit_cnt++;
        Send_Data(s); // JP test om te kijken of dit noodzakelijk is
      }  
    }
  }  
}

void udp_appcall(void)
{
  struct uip_udpip_hdr * udp_hdr = (struct uip_udpip_hdr *)&uip_buf[UIP_LLH_LEN];
  BACNET_ADDRESS src = { 0 };  /* address where message came from */
  time_t current_seconds = 0;
  uint32_t elapsed_seconds_1 = 0;
  uint32_t elapsed_seconds_2 = 0;

  if (uip_newdata()) {
	if (uip_udp_conn->rport == HTONS(0xBAC0) && uip_len) {
	  src.mac_len = 6;
	  src.mac[0] = udp_hdr->srcipaddr[1] >> 8;
	  src.mac[1] = udp_hdr->srcipaddr[1];
	  src.mac[2] = udp_hdr->srcipaddr[0] >> 8;
	  src.mac[3] = udp_hdr->srcipaddr[0];
	  src.mac[4] = udp_hdr->srcport;
	  src.mac[5] = udp_hdr->srcport >> 8;
	  npdu_handler(&src, &uip_buf[UIP_LLH_LEN + UIP_IPH_LEN + UIP_UDPH_LEN + 4], (uint16_t)(uip_len - 4));
	}
  }
  else if (uip_poll())
  {
    current_seconds = time(NULL);
    elapsed_seconds_1 = current_seconds - last_seconds_1;
    elapsed_seconds_2 = current_seconds - last_seconds_2;
    if (elapsed_seconds_1) {
      last_seconds_1 = current_seconds;
    }
	if (!tsm_timer_milliseconds(elapsed_seconds_1 * 1000))
	{
      if (elapsed_seconds_2) {
        last_seconds_2 = current_seconds;
      }
      handler_cov_task(elapsed_seconds_2);
	}
  }
}

void Ethernet_Timing_Control(void)
// wordt elke 100ms aangeroepen
{
static unsigned int in_cnt[10] = {0,0,0,0,0,0,0,0,0,0};
static unsigned int out_cnt[10] = {0,0,0,0,0,0,0,0,0,0};
static unsigned long in_total_cnt = 0;
static unsigned long out_total_cnt = 0;
static int index = 0;
static s_eth_status const eth_status_empty = { 0 };
unsigned char loop;

  if (opt_alg.ethernet_enabled)
  {
    for (loop = 0; loop < UIP_CONNS; loop++)
    {
      if (uip_conns[loop].appstate.connected)
      {
        if (uip_conns[loop].appstate.send_state == ETH_SEND_BUSY)
        {
          if(timer_expired(&no_ack_send_timer[loop])) 
          {
            timer_restart(&no_ack_send_timer[loop]);
            uip_conns[loop].appstate.send_state = ETH_SEND_NOT_BUSY;
            eth_status.no_ack_send_cnt++;
          }  
        }
        else
          timer_restart(&no_ack_send_timer[loop]);
      }    
    }
    if (ethernet_diag_reset_flag)
    {
      eth_status = eth_status_empty;
      ethernet_diag_reset_flag = 0;
      ethernet_diag_in_cnt = 0;
      ethernet_diag_in_cnt_peak = 0;
      ethernet_diag_out_cnt = 0;
      ethernet_diag_out_cnt_peak = 0;
      in_total_cnt = out_total_cnt = 0;
      for (index = 0; index < 8; index++)
        in_cnt[index] = out_cnt[index] = 0;
      index = 0;
    }
    else
    {
      index %= PULSES_PER_SECOND;
      in_total_cnt -= in_cnt[index];
      out_total_cnt -= out_cnt[index];
      in_cnt[index] = ethernet_in_cnt;
      out_cnt[index] = ethernet_out_cnt;
      ethernet_in_cnt = ethernet_out_cnt = 0;
      in_total_cnt += in_cnt[index];
      out_total_cnt += out_cnt[index];
      index++;
      ethernet_diag_in_cnt = in_total_cnt;
      ethernet_diag_out_cnt = out_total_cnt;
      if (ethernet_diag_in_cnt > ethernet_diag_in_cnt_peak)
        ethernet_diag_in_cnt_peak = ethernet_diag_in_cnt;
      if (ethernet_diag_out_cnt > ethernet_diag_out_cnt_peak)
        ethernet_diag_out_cnt_peak = ethernet_diag_out_cnt;
    }  
  }
  else
  {
    for (loop = 0; loop < UIP_CONNS; loop++)
      timer_restart(&no_ack_send_timer[loop]);
  }  
}
#endif // ETHERNET
