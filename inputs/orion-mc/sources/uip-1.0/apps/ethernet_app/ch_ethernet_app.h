/**
 * \addtogroup apps
 * @{
 */

/**
 * \defgroup helloworld Hello, world
 * @{
 *
 * A small example showing how to write applications with
 * \ref psock "protosockets".
 */

/**
 * \file
 *         Header file for an example of how to write uIP applications
 *         with protosockets.
 * \author
 *         Adam Dunkels <adam@sics.se>
 */

#ifndef __ETHERNET_APP_H__
#define __ETHERNET_APP_H__

#ifdef ETHERNET

#include "uipopt.h"

//#define ETH_BUFFER (512+20)
#define ETH_BUFFER (1024+22)

#define ETH_SEND_NOT_BUSY 0
#define ETH_SEND_ASK 1
#define ETH_SEND_BUSY 2

typedef struct
{
  int transmit_cnt;            // number of messages transmitted
  int receive_cnt;             // number of messages comming in
  int retransmit_cnt;          // message was retransmitted
  int transmit_ack_cnt;            // number of messages transmitted
  int poll_cnt;                // 
  int poll_con_cnt;
  int periodic_cnt;
  int connect_cnt;             // number of connections
  int disconnect_cnt;
  int closed_cnt;
  int aborted_cnt;
  int timedout_cnt;
  int no_ack_send_cnt;         // time out 1 second no ack after send received
//  int no_complete_message_cnt; // a incomplete message in the input string
//  int correct_message_cnt;     // a correct message received
//  int more_messages_cnt;        // more the one message in the intput string
} s_eth_status;

typedef struct ethernet_state
{
  char buffer_in[ETH_BUFFER];
  char buffer_out[ETH_BUFFER];

  unsigned int in_get_index;
  unsigned int in_put_index;
//  unsigned int out_get_index;
//  unsigned int out_put_index;
  char receive_cnt;
  
  char connected;
  char receive_flag;
  char send_state;
  char retransmit_retry_cnt;    //
  unsigned char state;
  unsigned char toegewezen; 
  unsigned char close_connection; // sluit verbinding af omdat 45 seconden geen communicatie
  int close_connection_timeout;
} uip_tcp_appstate_t;

typedef struct udp_state {
	char dummy;
};

typedef struct udp_state uip_udp_appstate_t;

extern s_eth_status eth_status;
//extern uip_tcp_appstate_t eth;
extern unsigned char ethernet_diag_reset_flag;
extern unsigned int ethernet_diag_in_cnt;
extern unsigned int ethernet_diag_in_cnt_peak;
extern unsigned int ethernet_diag_out_cnt;
extern unsigned int ethernet_diag_out_cnt_peak;

void Ethernet_0_Put_String(char *string);
void Ethernet_1_Put_String(char *string);
void Ethernet_2_Put_String(char *string);
void Ethernet_3_Put_String(char *string);
void Ethernet_Put_String(unsigned char conn_nr, char *string);

void ethernet_appcall(void);
#ifndef UIP_APPCALL
#define UIP_APPCALL ethernet_appcall
#endif /* UIP_APPCALL */

void udp_appcall(void);
#ifndef UIP_UDP_APPCALL
#define UIP_UDP_APPCALL udp_appcall
#endif /* UIP_UDP_APPCALL */

unsigned char ethernet_init(void);
//void ethernet_init(void);
void Ethernet_Timing_Control(void);

#endif // ETHERNET

#endif /* __ETHERNET_APP_H__ */
/** @} */
/** @} */
