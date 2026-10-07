// NetworkDevice.C

#include "stdlib.h"   // malloc
#include "string.h"
#include "ch_define.h"
#ifdef ETHERNET

#include "ch_asc1.h"
#include "ch_define.h"
#include "clock.h"
#include "ch_ethernet.h" 
#include "uip_timer.h"
#include "uip-conf.h"
#include "uip.h"
#include "uip_arp.h" 
#include "httpd.h"
#include "ch_cp2200.h"
#include "ch_timer.h"
#include "NetworkDevice.h"
#include "datalink.h"

unsigned char NetworkDeviceInitState = 0;

//static char WaitCnt = 0;
clock_time_t  sec_counter;

struct timer periodic_timer, udp_periodic_timer, arp_timer, TestTimer;

//  struct uip_eth_addr eaddr;   // tijdelijk test uit lezen cp2200
//  unsigned int TestMac[3];  //!!!!!!!!!!!!!!!!!!!!!!!!!
//  unsigned int TestMacChar[6];  //!!!!!!!!!!!!!!!!!!!!!!!!!
//  unsigned char TstH, TstL;

//int CntScan = 0;

void CountSeconds(void)
{
  sec_counter++;
}

clock_time_t clock_time(void)
{
    return(sec_counter);
}

void NetworkDeviceInit(void)
{
  uip_ipaddr_t ipaddr;

  switch (NetworkDeviceInitState)
  {
    case NETD_INIT_BEGIN:
      // Initialise the uIP TCP/IP stack.
      uip_init();

      // Initialise the ARP cache
      uip_arp_init();

      // Initialise UDP applications if necessary
      #if UIP_UDP
        // DHCP
        #if UIP_CONF_DHCP
        // Initialise the DHCP CLIENT app.
        if (tmp_security_params.valid_flash == FLASH_VALID) 
        {
          dhcpc_init();
        }
        #endif // UIP_CONF_DHCP

        // DNS resolution
        #if UIP_CONF_RESOLV
        // Initialise the DNS resolver app.
          resolv_init();
        #endif // UIP_CONF_RESOLV

        // SNTP
        #if UIP_CONF_SNTP
        // Sets up the SNTP application in uIP
          sntp_init();  
        #endif // UIP_CONF_SNTP

      #endif // UIP_UDP

      // Initialise the SRCPD app.
      // srcpd_init();

      // Initialise the webserver app.
      // httpd_init();

      // Initialise the telnet application
      // telnetd_init();

      // Initialise the hello world application
      if (ethernet_init() == 0)
        return;
      // ethernet_init();

      cp2200_InitState = 0;
      NetworkDeviceInitState = NETD_INIT_CP2200;
      break;
    case NETD_INIT_CP2200:
      if (CP2200_Init() == 1) // cp2200 is geinititaliseerd
        NetworkDeviceInitState++;
      break;
    case NETD_INIT_ADRESSES:
      // uip_ipaddr(ipaddr, (u16_t)255,(u16_t)255,(u16_t)255,(u16_t)0);
      uip_ipaddr(ipaddr, (u16_t)opt_alg.ethernet[1][0],(u16_t)opt_alg.ethernet[1][1],(u16_t)opt_alg.ethernet[1][2],(u16_t)opt_alg.ethernet[1][3]);
      uip_setnetmask(ipaddr);
    
      // uip_ipaddr(ipaddr, (u16_t)192, (u16_t)168, (u16_t)1, (u16_t)40);
      uip_ipaddr(ipaddr, (u16_t)opt_alg.ethernet[0][0],(u16_t)opt_alg.ethernet[0][1],(u16_t)opt_alg.ethernet[0][2],(u16_t)opt_alg.ethernet[0][3]);
      uip_sethostaddr(ipaddr);

      // uip_ipaddr(ipaddr, (u16_t)192, (u16_t)168, (u16_t)1, (u16_t)254);
      uip_ipaddr(ipaddr, (u16_t)opt_alg.ethernet[2][0],(u16_t)opt_alg.ethernet[2][1],(u16_t)opt_alg.ethernet[2][2],(u16_t)opt_alg.ethernet[2][3]);
      uip_setdraddr(ipaddr);

      NetworkDeviceInitState = NETD_INIT_TIMERS;

      break;
    case NETD_INIT_TIMERS:
      timer_set(&periodic_timer, CLOCK_SECOND / 2);
      timer_set(&udp_periodic_timer, CLOCK_SECOND / 10);
      timer_set(&arp_timer, CLOCK_SECOND * 10);
      timer_set(&TestTimer, CLOCK_SECOND );      // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
      NetworkDeviceInitState = NETD_INIT_BACNET;
      break;
    case NETD_INIT_BACNET:
      if (opt_alg.BACnet_enabled) {
        datalink_init(NULL);
      }
      NetworkDeviceInitState = NETD_INIT_READY;		
      break;
  }
}


void Reset_NetworkDevice(void)
{
  if (NetworkDeviceInitState != NETD_INIT_BEGIN)
  {
    // switch off network device
    NetworkDeviceInitState = NETD_INIT_BEGIN;
    Reset_CP2200();
    if (opt_alg.BACnet_enabled)
      datalink_cleanup();
  }  
}

//-----------------------------------------------------------------------------
//
// Main Network Divice
//
//-----------------------------------------------------------------------------
void Main_NetworkDevice(void)
{
static startup = 1;
static s_timer timer_31_25_ms;
static int index = 0;
int i = 0;
static CountTiks = 0;

  if (startup)
  {
    Timer_Set(&timer_31_25_ms, 1, TIME_BASE_31_25_MS); // controleerd of main loop binnen 100 ms blijft
    startup = 0;
  }
  if (Timer_Expired(&timer_31_25_ms))
  {
    Timer_Reset(&timer_31_25_ms);
    CountSeconds();  
  }  
  // JP 31-03-09 begin
  // zorgt er voor dat als door een storing het cp2200 IC gereset is hier dit IC opnieuw geinitialiseerd wordt
  if ((cp2200.RSTSTA & 0x03) && 
      (NetworkDeviceInitState == NETD_INIT_READY))
  {
    cp2200_InitState = 0;
    NetworkDeviceInitState = NETD_INIT_BEGIN;
  }    
  // JP 31-03-09 end
  if (NetworkDeviceInitState != NETD_INIT_READY)
    NetworkDeviceInit();
  else
  {
    uip_len = CP2200_receive();
    if (uip_len > 0)
    {
      if (BUF->type == htons(UIP_ETHTYPE_IP)) 
      {
        uip_arp_ipin();
        uip_input();
        // If the above function invocation resulted in data that
        // should be sent out on the network, the global variable
        // uip_len is set to a value > 0.
        if (uip_len > 0)
        {
          uip_arp_out();
          CP2200_transmit();
        }
      }
      else if (BUF->type == htons(UIP_ETHTYPE_ARP)) 
      {
        uip_arp_arpin();
        // If the above function invocation resulted in data that
        // should be sent out on the network, the global variable
        // uip_len is set to a value > 0.
        if (uip_len > 0) 
        {
          CP2200_transmit();
        }
      }
    } // end of (uip_len != 0)

    // MV 14-11-2011 Om te zorgen dat poll en periodic ook aan de beurt komen als er veel berichten binnen komen
    //else if (timer_expired(&periodic_timer)) // || uip_conns[0].appstate.send_state == ETH_SEND_ASK)
    if (timer_expired(&periodic_timer)) // || uip_conns[0].appstate.send_state == ETH_SEND_ASK)
    {
      timer_reset(&periodic_timer);
      for (i = 0; i < UIP_CONNS; i++) 
      {
        uip_periodic(i);
        // If the above function invocation resulted in data that
        // should be sent out on the network, the global variable
        // uip_len is set to a value > 0. 
        if (uip_len > 0) 
        {
          eth_status.periodic_cnt++;
          uip_arp_out();
          CP2200_transmit();
        }   
      }

      // Call the ARP timer function every 10 seconds.
      if (timer_expired(&arp_timer)) 
      {
        timer_reset(&arp_timer);
        uip_arp_timer();
      }
    }
    else if (timer_expired(&udp_periodic_timer)) // || uip_conns[0].appstate.send_state == ETH_SEND_ASK)
    {
      timer_reset(&udp_periodic_timer);

      for (i = 0; i < UIP_UDP_CONNS; i++) 
      {
        uip_udp_periodic(i);
        // If the above function invocation resulted in data that
        // should be sent out on the network, the global variable
        // uip_len is set to a value > 0. 
        if (uip_len > 0) 
        {
          eth_status.periodic_cnt++;
          uip_arp_out();
          CP2200_transmit();
        }   
      }
    }
    else
    {
      for (i = 0; i < UIP_CONNS; i++)
      {
        index++;
        index %= UIP_CONNS;
        if (uip_conns[index].appstate.send_state == ETH_SEND_ASK)
        {
          uip_poll_conn(&uip_conns[index]);
          if(uip_len > 0)
          {
            eth_status.poll_con_cnt++;
            uip_arp_out();
            CP2200_transmit();
          }
          break;
        }
      }
    }
/*
    else if (uip_conns[0].appstate.send_state == ETH_SEND_ASK)
    {
      uip_poll_conn(&uip_conns[0]);
      if(uip_len > 0) 
      {
        eth_status.poll_con_cnt++;
        uip_arp_out();
        CP2200_transmit();
      }   
    }
    else if (uip_conns[1].appstate.send_state == ETH_SEND_ASK)
    {
      uip_poll_conn(&uip_conns[1]);
      if (uip_len > 0) 
      {
        eth_status.poll_con_cnt++;
        uip_arp_out();
        CP2200_transmit();
      }   
    }
*/
  }
}
#endif // ETRHERNET

