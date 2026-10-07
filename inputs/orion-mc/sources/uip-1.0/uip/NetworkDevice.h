// NetworkDevice.h

#ifndef NETWORKDIVICE_H
#define NETWORKDIVICE_H

#ifdef ETHERNET

#define NETD_INIT_BEGIN 0
#define NETD_INIT_CP2200 1
#define NETD_INIT_ADRESSES 2
#define NETD_INIT_TIMERS 3
#define NETD_INIT_BACNET 4
#define NETD_INIT_READY 99

#define BUF ((struct uip_eth_hdr *)&uip_buf[0])

extern unsigned char NetworkDeviceInitState;

void Reset_NetworkDevice(void);
void Main_NetworkDevice(void);

#endif // ETHERNET

#endif // NETWORKDIVICE_H
