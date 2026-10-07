// Datalink B/IP functies voor gebruik van de uIP stack


/*####COPYRIGHTBEGIN####
 -------------------------------------------
 Copyright (C) 2005 Steve Karg

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 2
 of the License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to:
 The Free Software Foundation, Inc.
 59 Temple Place - Suite 330
 Boston, MA  02111-1307, USA.

 As a special exception, if other files instantiate templates or
 use macros or inline functions from this file, or you compile
 this file and link it with other works to produce a work based
 on this file, this file does not by itself cause the resulting
 work to be covered by the GNU General Public License. However
 the source code for this file must still be made available in
 accordance with section (3) of the GNU General Public License.

 This exception does not invalidate any other reasons why a work
 based on this file might be covered by the GNU General Public
 License.
 -------------------------------------------
####COPYRIGHTEND####*/

#include <stdint.h>     /* for standard integer types uint8_t etc. */
#include <stdbool.h>    /* for the standard bool type. */
#include <string.h>
#include "bacdcode.h"
#include "bacint.h"
#include "bip.h"
#include "net.h"        /* custom per port */
#include "ch_define.h"
#include "uip.h"
#include "ch_ethernet_app.h"
#include "ch_data.h"

/* port to use - stored in host byte order */
static uint16_t BIP_Port = 0xBAC0;

/* Broadcast Address - stored in host byte order */
static struct in_addr BIP_Broadcast_Address;

bool bip_uip_initialized = FALSE;

// Buffer voor het verzenden
uint8_t huge mtu[MAX_MPDU];


// Moet alle uip connecties sluiten
void bip_cleanup(
    void)
{
	int i;
	for (i = 0; i < UIP_UDP_CONNS; i++) 
    {
		if (uip_udp_conns[i].rport == htons(BIP_Port)) {
			uip_udp_remove(&uip_udp_conns[i]);
		}
	}

    bip_uip_initialized = FALSE;
}

/* returns host byte order */
uint32_t bip_get_addr(
    void)
{
	return (uint32_t)opt_alg.ethernet[1];
//    return BIP_Address.s_addr;
}

/* set using network byte order */
void bip_set_broadcast_addr(
    uint32_t net_address)
{
    BIP_Broadcast_Address.s_addr = net_address;
}

/* returns host byte order */
uint32_t bip_get_broadcast_addr(
    void)
{
    return BIP_Broadcast_Address.s_addr;
}

/* set using host byte order */
void bip_set_port(
    uint16_t port)
{
    BIP_Port = port;
}

/* returns host byte order */
uint16_t bip_get_port(
    void)
{
    return BIP_Port;
}

static int bip_decode_bip_address(
    uint8_t * pdu,      /* buffer to extract encoded address */
    struct in_addr *address,    /* in host format */
    uint16_t * port)
{
    int len = 0;
    uint32_t raw_address = 0;

    if (pdu) {
        (void) decode_unsigned32(&pdu[0], &raw_address);
        address->s_addr = raw_address;
        (void) decode_unsigned16(&pdu[4], port);
        len = 6;
    }

    return len;
}

/* function to send a packet out the BACnet/IP socket (Annex J) */
/* returns number of bytes sent on success, negative number on failure */
int bip_send_pdu(
    BACNET_ADDRESS * dest,      /* destination address */
    BACNET_NPDU_DATA * npdu_data,       /* network information */
    uint8_t * pdu,      /* any data to be sent - may be null */
    unsigned pdu_len)
{
    int mtu_len = 0;
	int bytes_send = 0;

    /* addr and port in host format */
    struct in_addr address;
    uint16_t port = 0;

    npdu_data; // TD - prevent warning

	if (!opt_alg.ethernet_enabled) {
		return -1;
	}

	// Determine dest address and port
    mtu[0] = BVLL_TYPE_BACNET_IP;
    if (dest->net == BACNET_BROADCAST_NETWORK) {
        address.s_addr = BIP_Broadcast_Address.s_addr;
        port = BIP_Port;
        mtu[1] = BVLC_ORIGINAL_BROADCAST_NPDU;
    } else if (dest->mac_len == 6) {
		bip_decode_bip_address(&dest->mac[0], &address, &port);
        mtu[1] = BVLC_ORIGINAL_UNICAST_NPDU;
    } else {
        /* invalid address */
        return -1;
    }

    mtu_len = 2;
    mtu_len +=
        encode_unsigned16(&mtu[mtu_len],
        (uint16_t) (pdu_len + 4 /*inclusive */ ));
    memcpy(&mtu[mtu_len], pdu, pdu_len);
    mtu_len += pdu_len;

    memcpy(uip_appdata, mtu, mtu_len);
	uip_udp_send(mtu_len);
	uip_ipaddr_copy(uip_udp_conn->sipaddr, &address.s_addr);

    return mtu_len;
}

/* Not needed! This will be done in the uIP stack */
//uint16_t bip_receive(
//    BACNET_ADDRESS * src,       /* source address */
//    uint8_t * pdu,      /* PDU data */
//    uint16_t max_pdu,   /* amount of space available in the PDU  */
//    unsigned timeout) /* number of milliseconds to wait for a packet */
//{       	
//    return 0;
//}

void bip_get_my_address(
    BACNET_ADDRESS * my_address)
{
    int i = 0;

    my_address->mac_len = 6;
    (void) encode_unsigned32(&my_address->mac[0], htonl(opt_alg.ethernet[1]));
    (void) encode_unsigned16(&my_address->mac[4], htons(BIP_Port));
    my_address->net = opt_alg.BACnet_Network_Nr;        /* local only, no routing */
    my_address->len = 0;        /* no SLEN */
    for (i = 0; i < MAX_MAC_LEN; i++) {
        /* no SADR */
        my_address->adr[i] = 0;
    }

    return;
}

void bip_get_broadcast_address(
    BACNET_ADDRESS * dest)
{       /* destination address */
    int i = 0;  /* counter */

    if (dest) {
        dest->mac_len = 6;
        (void) encode_unsigned32(&dest->mac[0],
            htonl(BIP_Broadcast_Address.s_addr));
        (void) encode_unsigned16(&dest->mac[4], htons(BIP_Port));
        dest->net = BACNET_BROADCAST_NETWORK;
        dest->len = 0;  /* no SLEN */
        for (i = 0; i < MAX_MAC_LEN; i++) {
            /* no SADR */
            dest->adr[i] = 0;
        }
    }

    return;
}

bool bip_init(
        char *ifname)
{
	struct uip_udp_conn *c;
	uint32_t ipaddr = {0};
	uint32_t netmask = {0};
	
    ifname; // TD - prevent warning

	if (bip_uip_initialized) {
		return FALSE;
	}

	// Initialiseer de ontvang connectie
	c = uip_udp_new(NULL , HTONS(BIP_Port));
	if(c != NULL) {
		uip_udp_bind(c, HTONS(BIP_Port));
	}

	// Subnet broadcast adres
	ipaddr = ipaddr | opt_alg.ethernet[0][0];
	ipaddr = ipaddr << 8 | opt_alg.ethernet[0][1];
	ipaddr = ipaddr << 8 | opt_alg.ethernet[0][2];
	ipaddr = ipaddr << 8 | opt_alg.ethernet[0][3];
	netmask = netmask | opt_alg.ethernet[1][0];
	netmask = netmask << 8 | opt_alg.ethernet[1][1];
	netmask = netmask << 8 | opt_alg.ethernet[1][2];
	netmask = netmask << 8 | opt_alg.ethernet[1][3];
	bip_set_broadcast_addr( htonl(ipaddr | ~netmask) );
	//bip_set_broadcast_addr(0xFFFFFFFF);

	bip_uip_initialized = TRUE;

	return TRUE;
}

long bip_getaddrbyname(
        const char *host_name)
{
    host_name; // TD - prevent warning
	return 0;
}

