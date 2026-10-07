// C__CP2200.C

#include <stdlib.h>   // malloc
#include <string.h>
#include "ch_define.h"
#ifdef ETHERNET
#include "ch_asc1.h"
#include "clock.h"
#include "timer.h"
#include "uip_timer.h"
#include "uip-conf.h"
#include "uip.h"
#include "uip_arp.h" 
#include "NetworkDevice.h"
#include "httpd.h"
//#include "ch_iic_rtc.h"
#include "ch_timer.h"
#include "ch_cp2200.h"

#pragma class HB=MEM_CP2200
volatile s_cp2200 huge cp2200;
#pragma default_attributes

// globale Variablen
static char CP2200_status=0;

unsigned char cp2200_InitState = 0;
struct timer InitTimeOut;

static char WaitCnt = 0;

struct uip_eth_addr eaddr;   // tijdelijk test uit lezen cp2200

//---------------------------------------------------------------------------------
// Static variables for CP2200
//---------------------------------------------------------------------------------

// Physical size of packet must be completely read!

bit TX_EventPending;    // the CP2200 hardware receive event
bit ARP_EventPending;   // trigger the arp timer event

bit CP2200_Osc_Ready;   // Ocsillator ready bits            !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
bit CP2200_Active;      // Result of ocsillator and reset bits          !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
bit CP2200_AutoNeg;     // Autonegotiation status                     !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
bit Link_Connected;     // Contains the status of the ethernet link   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

static void cpWriteMac(char adr,int value)
{
  cp2200.MACADDR = adr;
  cp2200.MACDATAH = (unsigned char)(value >> 8);
  cp2200.MACDATAL = (unsigned char) value;
  cp2200.MACRW = 1;
}

void CP2200_Interrupt(void)
{
  volatile u8_t intstat0;
  volatile u8_t intstat1;   
  volatile u8_t valid_bits;
  volatile u8_t pkt_count;
  volatile u8_t Tmp_u8_t;

  do
  {
    // Read Interrupt status bytes - these can be used to determine network conditions
    // as well as receive data interrupt status bits.
    intstat0 = (cp2200.INT0 & cp2200.INT0EN);
    intstat1 = (cp2200.INT1 & cp2200.INT1EN);
   
    // Test if a packet received interrupt bit has been set
    if (intstat0 & INT0_RXINT)      // Bit 0
    {
      // Count the number of packets in the receive buffer         
      // This is equal to the number of bits set to 1 in TLBVALID
      valid_bits = cp2200.TLBVALID;
      // Loop accumulates the number of bits set in Valid_Bits and 
      // stores this value in i. Uses the Brian Kernighan method 
      // for counting bits
      for (pkt_count = 0; valid_bits; pkt_count++)      
      {                                 
        valid_bits &= valid_bits - 1; 
      }
   
      // If the receive buffer has 7 packets, then disable reception.
      if ( pkt_count >= 7) 
      {
        // Inhibit New Packet Reception
        cp2200.RXCN = RXCN_RXINH;
      }
    }

    // Receive FIFO Full - do this first to stop packets
    // The receive buffer is full or the maximum number of packets is received.
    // Decode the RXFIFOSTA status register to determine the receive buffer status.
    if(intstat0 & INT0_RXFINT)      // Bit 1
    {
      // Turn on congestion control?
      cp2200.RXCN |= RXCN_RXINH;
      cp2200.INT0EN |= INT0EN_ERXEINT;
    }

    // Packet Transmitted
    // The transmit interface has transmitted a packet.
    if(intstat0 & INT0_TXINT)       // Bit 2
    {
      // Clear transmit OK bit
      cp2200.TXSTA2 &= ~TXSTA2_TXOK;
    }

    // Flash Write/Erase Complete
    // A Flash write or erase operation has completed.
    //  if (intstat0 & FLWEINT)     // Bit 3
    //  {
    //
    //  }

    // Oscillator Initialization Complete
    // The external oscillator has stabilized.
    if(intstat0 & INT0_OSCINT)      // Bit 4
    {
      CP2200_Osc_Ready = TRUE;  // Ocsillator ready bits
    }

    // Self Initialization Complete
    // The device is ready for Reset Initialization.
    if(intstat0 & INT0_SELFINT)     // Bit 5
    {
      CP2200_Active = TRUE;
    }

    // Receive FIFO Empty
    // The last packet in the receive buffer has been unloaded or discarded.
    if(intstat0 & INT0_RXEINT)      // Bit 6
    {
      // Turn off congestion control
      Tmp_u8_t = RXCN_RXINH;
      cp2200.RXCN &= ~Tmp_u8_t;
      Tmp_u8_t = INT0EN_ERXEINT;
      cp2200.INT0EN &= ~Tmp_u8_t;
    }                           

    // End of Packet
    // The last byte of a packet has been read from the
    // receive buffer using the AutoRead interface.
    //  if (intstat0 & EOPINT)      // Bit 7
    //  {
    //      if (cp2200.RXSTA & CPEND)
    //      {
    //          cp2200.RXCN |= RXCLRV;
    //      }
    //  }

    // Auto-Negotiation Complete
    // An auto-negotiation attempt has completed. This interrupt
    // only indicates completion, and not success. Occasionally,
    // Auto-Negotiation attempts will not complete and/or fail;
    // therefore, a 3 to 4 second timeout should be
    // implemented. A successful auto-negotiation attempt is
    // one that completes without failure.
    if(intstat1 & INT1_ANCINT)      // Bit 0
    {
      if (cp2200.PHYSTA == 0x00)
      {
        CP2200_AutoNeg = TRUE;
      }
    }

    // Remote Fault Notification
    // A remote fault (in the cable or link partner) has been detected.
    // See IEEE 802.3 for more information about remote fault detection.
    if (intstat1 & INT1_RFINT)      // Bit 1
    {
      Link_Connected = FALSE;       // Reset the status of the ethernet link
    }

    // Auto-Negotiation Failed
    // An auto-negotiation attempt has failed. Software should
    // check for a valid link and re-try auto-negotiation.
    if (intstat1 & INT1_ANFINT)     // Bit 2
    {
      CP2200_AutoNeg = FALSE;  // Reset the status of the ethernet link
      Link_Connected = FALSE;
    }

    // Jabber Detected
    // The transmit interface has detected and responded to a
    // jabber condition. See IEEE 802.3 for more information
    // about jabber conditions.
    if(intstat1 & INT1_JABINT)      // Bit 3
    {
      Tmp_u8_t = INT1_JABINT;
      intstat1 &= ~Tmp_u8_t;
    }

    // Link Status Changed
    // The device has been connected or disconnected from the network.
    if (intstat1 & INT1_LINKINT)        // Bit 4
    {
      Link_Connected = (cp2200.PHYCN & PHYCN_LINKSTA);      // Reset the status of the ethernet link
    }

    // “Wake-on-LAN” Wakeup Event
    // The device has been connected to a network.
    if (intstat1 & INT1_WAKEINT)        // Bit 5
    {
      Tmp_u8_t = INT1_WAKEINT;
      cp2200.INT1EN &= ~Tmp_u8_t;   
      Link_Connected = (cp2200.PHYCN & PHYCN_LINKSTA);      // Reset the status of the ethernet link
    }
  }
  while (intstat0 || intstat1);
}


interrupt CP2200_INT_ADR using(CP2200_INT_RB) void CP2200_ExtI2C_Interrupt(void)
{
  CP2200_Interrupt();
}

void Reset_CP2200(void)
{
// disable CP2200
  CC2_CC21IC_IE = 0;  // Enable interupt shared cp2200 and Ext-I2C
}

unsigned char CP2200_Init(void)
{
static s_timer timer_31_25_ms;
u8_t tmp;
static u8_t AutoNegTryed;

  switch (cp2200_InitState)
  {
    case 0:
      // switch interrupt CP2200 on
      CP2200_INT_ALTSEL0 = 0;
      CP2200_INT_ALTSEL1 = 0;
      CP2200_INT = 1;
      CP2200_INT_DP = INPUT;
      CP2200_INT_ODP = PUSH_PULL;

      CC2_IOC =  0x0004; // load CAPCOM2 I/O control register (reeds ingevuld bij IIC_RTC_Control();)
      // non staggered mode 
      // compare output signals effect the associated port output pin

      CC2_M5 = CC2_M5 & 0xFF0F | 0x0020; // Generate interrupt on negative transition
      CC2_CC21IC = CP2200_INT_LEVEL;     

      CP2200_Osc_Ready = FALSE;     // Ocsillator ready bits
      CP2200_Active = FALSE;        // Clear the result of ocsillator and reset bits
      CP2200_AutoNeg = FALSE;       // Autonegotiation status
      Link_Connected = FALSE;       // Reset the status of the ethernet link
      AutoNegTryed = FALSE;

      // create a reset
      CP2200_RST = 0;                  // Step 0, Hardware reset
      CP2200_RST_DP = 1;               //   set reset pin as output
      cp2200_InitState++;
      WaitCnt = 2;
	  timer_set(&InitTimeOut, (int)8 * CLOCK_SECOND );
      Timer_Set(&timer_31_25_ms, 1, TIME_BASE_31_25_MS); // controleerd of main loop binnen 100 ms blijft
      break;
    case 1:
      if (Timer_Expired(&timer_31_25_ms))
      {
        Timer_Reset(&timer_31_25_ms);
        WaitCnt--;
        if (WaitCnt <= 0)
        {
          CP2200_RST_DP = 0;               //   set reset pin as input  
          CP2200_RST = 1;
          cp2200_InitState++;
        }
      }
      break;
    case 2:
      if (CP2200_RST == 1)             // Step 1, check if reset is high
      {
        cp2200.SWRST |= SWRST_RESET;
        cp2200_InitState++;
      }
      break;  
    case 3:
      cp2200.VDMCN |= VDMCN_VDMEN;     // enabel VDD monitoring 
      cp2200_InitState++;
      break;
    case 4:
      if (cp2200.VDMCN & VDMCN_VDDSTAT) // check VDD
      {  // Clear Interrupt masks
        cp2200.INT0EN = 0x00;
        cp2200.INT1EN = 0x00;
        cp2200_InitState++;
      }
      break;    
    case 5:
      if(!(cp2200.FLASHSTA & 0x08)) 
      {
        cp2200.INT0EN = (INT0EN_EOSCINT | INT0EN_ESELFINT);
        cp2200_InitState++;
      }
      break;
    case 6:
      // Step 2: Wait for Oscillator Initialization to complete. The host processor will
      //         receive notification through the interrupt request signal once the
      //         oscillator has stabilized. Use INT0RD to avoid clearing self-initialisation bit

      if (cp2200.INT0RD & INT0RD_OSCINTR) // check if oscillator init is complete
        cp2200_InitState++;
      break;
    case 7:       // Disable staat in de handleiding (step4) ????????
      cp2200.INT0EN = INT0EN_ETXINT | INT0EN_ERXINT; // Stap 4, enabel interrupts used
      cp2200.INT1EN = 0;
      cp2200.RSTEN = 0;   
      // cp2200.RSTEN = RSTEN_ESWRST | RSTEN_EPFRST;    // enable reset by software ande by power failure
      cp2200_InitState++;

      cp2200.FLASHADDRH = 0x1F;
      cp2200.FLASHADDRL = 0xFA;

      for (tmp = 0; tmp < 6; tmp++)
      {
        uip_ethaddr.addr[tmp] = cp2200.FLASHAUTORD;
      }   
      break;

    //--------------------------------------------------------------------------
    // Physical Layer Initialization (Section 15.7 of CP220x Datasheet)
    //--------------------------------------------------------------------------
      case 8:    //PHY INIT
      cp2200.PHYCN = 0;                           // Phy init Stap 2
      cp2200.PHYCF = PHYCF_AUTONEG;               // Phy init Stap 3
      cp2200.PHYCF |= PHYCF_SMSQ | PHYCF_LINKINTG | PHYCF_JABBER | PHYCF_ADPAUSE | PHYCF_AUTOPOL;
      tmp = cp2200.INT1RD;                        // Interrupt Status Register 1 (Self-Clearing) page 34
      cp2200.PHYCN |= PHYCN_PHYEN;                // Phy init Stap 4.1 
      cp2200_InitState++;
      WaitCnt = 2;                                // Phy init Stap 4.2 wait
      Timer_Restart(&timer_31_25_ms);
      break;
    case 9:
      if (Timer_Expired(&timer_31_25_ms))
      {
        Timer_Reset(&timer_31_25_ms);
        WaitCnt--;
        if (WaitCnt <= 0)
        {
          cp2200_InitState++;
          cp2200.PHYCN |= PHYCN_TXEN | PHYCN_RXEN;  // Phy init Stap 4.3 Enable the transmitter and receiver
        }
      }
      break;
    case 10:
      if (cp2200.INT1RD & INT1RD_WAKEINTR)          // ok wait 250ms
        WaitCnt = 4*2+1;
      else                                          // wait 1,5 seconds  
        WaitCnt = 4*12+1;
      cp2200_InitState++;
      Timer_Restart(&timer_31_25_ms);
      break;
    case 11:
      if (Timer_Expired(&timer_31_25_ms))
      {
        Timer_Reset(&timer_31_25_ms);
        WaitCnt--;
        if (WaitCnt <= 0)
          cp2200_InitState++;
      }                                              
      break;
    case 12:
      if (cp2200.PHYSTA == 0x00)  
        cp2200_InitState++;                        // Normal operation
      break;
    case 13:
      if (cp2200.PHYCN & PHYCN_LINKSTA)
        cp2200_InitState++;   // Link is good
      break;  
    case 14:    
      cp2200.IOPWR = IOPWR_ACTEN | IOPWR_LINKEN;  // Phy init Stap 6 enable activety led and link led
      cp2200_InitState++;
      break;

    case 15:  
    //--------------------------------------------------------------------------
    // Mac Initialization (Section 14. of CP220x Datasheet)
    //--------------------------------------------------------------------------

    // Step 7: Initialize the media access controller (MAC). See “14.1. Initializing the MAC”
    //         on page 77 for a detailed MAC initialization procedure.
    //         Most MAC indirect registers can be left at their reset values.

    // Step 7a: Write 0x3F0F to CWMAXR.
      cp2200.MACADDR = CWMAXR;
      cp2200.MACDATAH = 0x3f;
      cp2200.MACDATAL = 0x0f;
      cp2200.MACRW = 0;

      // Step 7a: Determine if the physical layer is set to full-duplex or half-duplex.
      //          The MAC must be set to the same duplex mode as the physical layer
      //          before sending or receiving any packets.

      if(cp2200.PHYCN & PHYCN_DPLXMD)
      {
      // Step 7b: Write 0x00B3 (full-duplex) or 0x00B2 (half-duplex) to MACCF.
      //          The appropriate bits in this register may also be set or cleared
      //          to change padding options or MAC behavior.
        cp2200.MACADDR = MACCF;
        cp2200.MACDATAH = 0x40;
        cp2200.MACDATAL = 0xb3;
        cp2200.MACRW = 0;
    
      // Step 7c: Write 0x0015 (full-duplex) or 0x0012 (half-duplex) to IPGT.
        cp2200.MACADDR = IPGT;
        cp2200.MACDATAH = 0x00;
        cp2200.MACDATAL = 0x15;
        cp2200.MACRW = 0;
      }
      else
      {
      // Step 7b: Write 0x00B3 (full-duplex) or 0x00B2 (half-duplex) to MACCF.
      // The appropriate bits in this register may also be set or cleared
      //  to change padding options or MAC behavior.
        cp2200.MACADDR = MACCF;
        cp2200.MACDATAH = 0x40;
        cp2200.MACDATAL = 0xb2;
        cp2200.MACRW = 0;
    
      // Step 7c: Write 0x0015 (full-duplex) or 0x0012 (half-duplex) to IPGT.
        cp2200.MACADDR = IPGT;
        cp2200.MACDATAH = 0x00;
        cp2200.MACDATAL = 0x12;
        cp2200.MACRW = 0;
      }

      // Step 7d: Write 0x0C12 to IPGR.
      cp2200.MACADDR = IPGR;
      cp2200.MACDATAH = 0x0C;
      cp2200.MACDATAL = 0x12;
      cp2200.MACRW = 0;

      // Configure the MAXLEN register to 1518 bytes
      cp2200.MACADDR = MAXLEN;
      cp2200.MACDATAH = (UIP_BUFSIZE >> 8);
      cp2200.MACDATAL = (UIP_BUFSIZE & 0xff);
      //  cp2200.MACDATAH = 0x05;    // 1518 octets
      //  cp2200.MACDATAL = 0xEE;
      //  cp2200.MACDATAH = 0x06;    // 1536 octets
      //  cp2200.MACDATAL = 0x00;
      cp2200.MACRW = 0;

      // Step 7e: Program the 48-bit Ethernet MAC Address by writing to MACAD0:MACAD1:MACAD2.
      //          Set Node address 6 bytes from MCU FLASH (if valid and different from
      //          MAC address in CP2200). Otherwise read MAC from CP2200.

      // Set CP2200 Flash address regs to last 6 bytes containing factory set MAC address

      // Perform octet reversal for correct byte order in H/W mac register
      cp2200.MACADDR = MACAD0;
      cp2200.MACDATAH = uip_ethaddr.addr[5];
      cp2200.MACDATAL = uip_ethaddr.addr[4];
      cp2200.MACRW = 0;
      cp2200.MACADDR = MACAD1;
      cp2200.MACDATAH = uip_ethaddr.addr[3];
      cp2200.MACDATAL = uip_ethaddr.addr[2];
      cp2200.MACRW = 0;
      cp2200.MACADDR = MACAD2;
      cp2200.MACDATAH = uip_ethaddr.addr[1];
      cp2200.MACDATAL = uip_ethaddr.addr[0];
      cp2200.MACRW = 0;
  
      // Step 7f: Write 0x0001 to MACCN to enable reception.
      //          If loopback mode or flow control is desired, set the appropriate bits
      //          to enable these functions.

      cp2200.MACADDR = MACCN;
      cp2200.MACDATAH = 0x00;
      cp2200.MACDATAL = 0x01;
      cp2200.MACRW = 0;

      // Step 8: Configure the receive filter. See “12.3. Initializing the Receive Buffer,
      //         Filter and Hash Table” on page 57 for a detailed initialization procedure.
      //         Normally no additional initialisation is normally necessary

      //    CP2200regs[RXFILT] = (IGNRUNT | IGNERR | IGNBCST | IGNMCST);
      //    CP2200regs[RXHASHH] = 0xFF;
      //    CP2200regs[RXHASHL] = 0xFF;

      // Other housekeeping arrangements
      // Bit values in INT0EN
      //#define EEOPINT     0x80    // Enable End of Packet Interrupt.
      //#define ERXEINT     0x40    // Enable Receive FIFO Empty Interrupt.
      //#define ESELFINT        0x20    // Enable Self Initialization Complete Interrupt.
      //#define EOSCINT     0x10    // Enable Oscillator Initialization Complete Interrupt.
      //#define EFLWEINT        0x08    // Enable Flash Write/Erase Operation Complete Interrupt.
      //#define ETXINT      0x04    // Enable Packet Transmitted Interrupt.
      //#define ERXFINT     0x02    // Enable Receive FIFO Full Interrupt.
      //#define ERXINT      0x01    // Enable Packet Received Interrupt.
      // Bit values in INT1EN
      //#define EWAKEINT        0x20    // Enable “Wake-on-Lan” Interrupt.
      //#define ELINKINT        0x10    // Enable Link Status Changed Interrupt.
      //#define EJABINT     0x08    // Enable Jabber Detected Interrupt.
      //#define EANFINT     0x04    // Enable Auto-Negotiation Failed Interrupt.
      //#define ERFINT      0x02    // Enable Remote Fault Interrupt.
      //#define EANCINT     0x01    // Enable Auto-Negotiation Complete Interrupt.
      
      // Enable interupts
      cp2200.INT0EN = (INT0EN_ETXINT | INT0EN_ERXFINT | INT0EN_ERXINT);
      cp2200.INT1EN = (INT1EN_ELINKINT | INT1EN_EJABINT );
      
      // Clear any interrupts pending
      tmp = cp2200.INT0;
      tmp = cp2200.INT1;

      CC2_CC21IC_IE = 1;  // Enable interupt shared cp2200 and Ext-I2C

      return (1);
  }
  if(timer_expired(&InitTimeOut))
    cp2200_InitState = 0;
  return (0);  
}



// ---------------------------------------------
unsigned char CP2200_ReadTXBuffer(unsigned int addr)
{
  cp2200.RAMADDRL = addr & 0xFF; 
  cp2200.RAMADDRH = addr >> 8; 
  return (cp2200.RAMTXDATA);

}

// ---------------------------------------------
void CP2200_WriteTXBuffer(unsigned int addr, unsigned char value)
{
  cp2200.RAMADDRL = addr & 0xFF; 
  cp2200.RAMADDRH = addr >> 8; 
  cp2200.RAMTXDATA = value; 
}

// ---------------------------------------------
unsigned char CP2200_ReadRXBuffer(unsigned int addr)
{
  cp2200.RAMADDRL = (char) (addr & 0xFF); 
  cp2200.RAMADDRH = (char) (addr >> 8); 
  return (cp2200.RAMRXDATA);
}

// ---------------------------------------------
void CP2200_WriteRXBuffer(unsigned int addr, unsigned char value)
{
  cp2200.RAMADDRL = addr & 0xFF; 
  cp2200.RAMADDRH = addr >> 8; 
  cp2200.RAMRXDATA = value; 
}

// ---------------------------------------------------------------------------
//                          CP2200_receive()
// This function will service interrupt bits (polled) and
// read an entire IP packet into the uip_buf.
// ---------------------------------------------------------------------------
u16_t CP2200_receive(void)
{    
  volatile int i;
  u16_t RX_Length = 0;

  // Actual receive data routine (polled)
  // Step 1: Read RXVALID (CPINFOH.7) to check if the current packet was received correctly.
  if((cp2200.CPINFOH & CPINFOH_RXVALID) && (cp2200.CPINFOL & CPINFOL_RXOK))
  { // Step 2: If RXOK and RXVALID are True, read the length of the current packet from CPLENH:CPLENL.
      RX_Length = (cp2200.CPLENL + (cp2200.CPLENH << 8));

    // Step 3: Read the entire packet, one byte at a time, by reading RXAUTORD.
    // If packet will fit in the buffer

    if (UIP_BUFSIZE >= RX_Length)
    {
      for (i = 0; i < RX_Length; i++) 
      {
        *(uip_buf + i) = cp2200.RXAUTORD;
      }
      // Step 4: If the entire packet was read, write a ‘1’ to RXCLRV (RXCN.2).
      if (cp2200.RXSTA & RXSTA_CPEND)
      {
        cp2200.RXCN |= RXCN_RXCLRV;                    // Clear the valid bit only
      }
      else
      {
        cp2200.RXCN |= RXCN_RXSKIP;
      }
    }               
    else
    { // Skip packet as it was invalid or large for the buffer
      cp2200.RXCN |= RXCN_RXSKIP;
    }

    // If there are no more packets in the receive buffer, enable reception
    if (cp2200.TLBVALID == 0x00)
    {
      cp2200.RXCN = 0x00;   
    }
  }
  else
  {
    cp2200.RXCN |= RXCN_RXSKIP;
  }

  // Finally return the buffer length which, if non zero, indicates if a packet has been received
  return RX_Length;
}

//-----------------------------------------------------------------------------
//                  CP2200_transmit(void)
//
// Send the data packet in the uip_buf and uip_appdata buffers
// to the CP2200 ethernet controller. 
//
// The uip_buf array is used to hold incoming and outgoing packets.
// The device driver should place incoming data into this buffer.
// When sending data, the device driver should read the
// link level headers and the TCP/IP headers from this buffer.
// The size of the link level headers is configured by the
// UIP_LLH_LEN define.
//
//-----------------------------------------------------------------------------
unsigned char CP2200_Transmit_Reset(void) // JP 24-01-08
{
long cnt = 100000;

  while (cp2200.TXBUSY & TXBUSYFLAG && cnt)
    cnt--;
  if (cnt == 0)
    return (0);  
  cp2200.TXCN = 0x00;     // e. Set TXCN to 0x00.
  return (1);
}
/*
void CP2200_Transmit_Reset(void) // JP 24-01-08
{
  while (cp2200.TXBUSY & TXBUSYFLAG);
  cp2200.TXCN = 0x00;     // e. Set TXCN to 0x00.
}
*/

void CP2200_transmit(void)
{
  u8_t *ptr; // general pointer for data
  static u16_t i;
  static u16_t j;

  // Once reset initialization is complete (See Section 6.2 on page 17), the CP2200/1
  // is ready to transmit Ethernet packets.
  // The following procedure can be used to transmit a packet:

  retransmit:
  // Step 1: Poll TXBUSY until it becomes 0x00.
  while (cp2200.TXBUSY & TXBUSYFLAG);

  // Step 2: Check to ensure that the last packet transmitted was not aborted.
  //         The last packet was aborted if any of bits (3–7) in the TXSTA3 register
  //         are set. If the previous packet was aborted, the following steps
  //         may be used to clear the abort condition.
  //         Note that the abort condition must be cleared prior to
  //         sending any additional packets.
  //         Aborted packets only occur in half-duplex mode and are typically caused by
  //         excessive network traffic.
  if ((cp2200.TXSTA3 & (TXSTA3_TXURUN | TXSTA3_TXLTCL | TXSTA3_TXEXCL | TXSTA3_TXEXDE | TXSTA3_TXDE)) != 0)
  {
    // Set TXSTARTH:TXSTARTL to 0x0000.
    cp2200.TXSTARTH = 0x00;
    cp2200.TXSTARTL = 0x00;
    // Set TXENDH:TXENDL to 0x0040.
    cp2200.TXENDH = 0x00;
    cp2200.TXENDL = 0x40;
    // Write 0x81 to TXCN.
    cp2200.TXCN = (TXCN_OVRRIDE | TXCN_TXGO);
    // Re-check for an aborted packet and repeat this procedure as necessary.
    goto retransmit;
  }


  cp2200.TXCN = 0x00;     // e. Set TXCN to 0x00.
  // Step 3:
  // Set up packet header for passing to CP2200 TX FIFO 
  // 54 octets - header, etc. info
  ptr = &uip_buf[0];

  // Modified to allow for the TCP options field length if supplied
  // uip_buf[UIP_LLH_LEN + 9] is the header protocol octet
  // uip_buf[UIP_LLH_LEN + 32] is the TCP offset octet which determines where the TCP header ends and data begins

  if (EBUF->type == HTONS(UIP_ETHTYPE_ARP))
  {
    j = uip_len;
  }
  else
  {
    if (uip_buf[UIP_LLH_LEN + 9] == UIP_PROTO_TCP) 
      j = (UIP_LLH_LEN + UIP_TCPIP_HLEN + (((uip_buf[UIP_LLH_LEN + 32] >> 4) - 5) << 2));
    else
      j = (UIP_LLH_LEN + UIP_TCPIP_HLEN);
  }

  // Step 3a: Set the TXSTARTH:TXSTARTL transmit buffer pointer to 0x0000.
  cp2200.TXSTARTH = 0x00;
  cp2200.TXSTARTL = 0x00;

  // Step 4: Load data into transmit buffer by writing it to TXAUTOWR one byte at time.
  for (i = 0; i < j; i++) 
    cp2200.TXAUTOWR = *ptr++; // write header data to transmit SRAM

  // reached the end of header, switch pointer to uip_appdata to be sent.
  // Write appdata to CP2200 - should include any offset
  if (i < uip_len) 
  {
    for (ptr = uip_appdata; (i < uip_len) ; i++) 
    {
      cp2200.TXAUTOWR = *ptr++; // write data to transmit SRAM
    }
 }

  // Step 5: Set the TXSTARTH:TXSTARTL transmit buffer pointer back to 0x0000.
  cp2200.TXSTARTH = 0x00;
  cp2200.TXSTARTL = 0x00;

  // Step 6: Write a ‘1’ to the TXGO bit (TXCN.0) to begin transmission.
  cp2200.TXCN = TXCN_TXGO;
}

//*****************************************************************************
//*****************************************************************************
// for reading writing erasing flash off CP2200
void CP2200_Init_Flash_Data(void)
// JP 21-02-2014 aangepast omdat detectie display kleur niet altijd goed werkt als SD-card geplaatst
{
static s_timer cp2200_flash_timer;
int loop = 0;

    CC2_CC21IC_IE = 0;  // Disable interupt shared cp2200 and Ext-I2C
    CP2200_RST = 0;                  // Step 0, Hardware reset
    CP2200_RST_DP = 1;               //   set reset pin as output
    Timer_Set(&cp2200_flash_timer, 5, TIME_BASE_1_MSEC);
    while (!Timer_Expired(&cp2200_flash_timer));
    CP2200_RST = 1;
    CP2200_RST_DP = 0;               //   set reset pin as input  
    Timer_Set(&cp2200_flash_timer, 10, TIME_BASE_1_MSEC);
    while (!Timer_Expired(&cp2200_flash_timer));
    Timer_Set(&cp2200_flash_timer, 10, TIME_BASE_1_MSEC);
    while ((CP2200_RST == 0) && (loop < 1000))
      loop++;
    CC2_CC21IC_IE = 1;  // Enable interupt shared cp2200 and Ext-I2C
    while (!Timer_Expired(&cp2200_flash_timer));
}
/*
void CP2200_Init_Flash_Data(void)
{
static s_timer cp2200_flash_timer;
int loop = 0;

    CC2_CC21IC_IE = 0;  // Disable interupt shared cp2200 and Ext-I2C
    CP2200_RST = 0;                  // Step 0, Hardware reset
    CP2200_RST_DP = 1;               //   set reset pin as output
    Timer_Set(&cp2200_flash_timer, 6, TIME_BASE_1_MSEC);
    for (loop = 0; loop < 100; loop++)
      _nop();
    CP2200_RST = 1;
    CP2200_RST_DP = 0;               //   set reset pin as input  
    while ((CP2200_RST == 0) && (loop < 1000))
      loop++;
    CC2_CC21IC_IE = 1;  // Enable interupt shared cp2200 and Ext-I2C
    while (!Timer_Expired(&cp2200_flash_timer));
}
*/

void CP2200_Read_Flash_Data(s_CP2200_flash *flash)
// addr = adres van waar af data uit flash gelezen wordt
// buffer is pointer naar buffer waarin data
{
unsigned char *data = (unsigned char *)flash;
int loop;

  CC2_CC21IC_IE = 0;  // Disable interupt shared cp2200 and Ext-I2C
  cp2200.FLASHADDRH = 0;
  cp2200.FLASHADDRL = 0;
  for (loop = 0; loop < sizeof(s_CP2200_flash); loop++)
  {
    *data = cp2200.FLASHAUTORD;
    data++;
  }   
  CC2_CC21IC_IE = 1;  // Enable interupt shared cp2200 and Ext-I2C
}

void CP2200_Erase_Flash_Data(void)
{
static s_timer cp2200_flash_timer;

  // laatste page beveiligine want hier staat mac address
  CC2_CC21IC_IE = 0;  // Enable interupt shared cp2200 and Ext-I2C
  cp2200.FLASHKEY = 0xA5;
  cp2200.FLASHKEY = 0xF1;
  cp2200.FLASHADDRH = 0;
  cp2200.FLASHADDRL = 0;
  cp2200.FLASHERASE = 0x01;
  Timer_Set(&cp2200_flash_timer, 15, TIME_BASE_1_MSEC);
  while ((cp2200.FLASHSTA & 0x08) && !Timer_Expired(&cp2200_flash_timer))
  {
  }
  if (Timer_Expired(&cp2200_flash_timer))
    cp2200_flash.lcd_color = 0; // bij geen flash altijd groen display
  CC2_CC21IC_IE = 1;  // Enable interupt shared cp2200 and Ext-I2C
}

void CP2200_Write_Flash_Byte(int addr, unsigned char byte)
{
int loop = 0;

  if (byte != 0xFF)
  {
    CC2_CC21IC_IE = 0;  // Enable interupt shared cp2200 and Ext-I2C
    cp2200.FLASHKEY = 0xA5;
    cp2200.FLASHKEY = 0xF1;
    cp2200.FLASHADDRH = addr >> 8;
    cp2200.FLASHADDRL = addr & 0xFF;
    cp2200.FLASHDATA = byte;
    while ((cp2200.FLASHSTA & 0x08) && (loop < 1000))
      loop++;
    CC2_CC21IC_IE = 1;  // Enable interupt shared cp2200 and Ext-I2C
  }  
}

void CP2200_Write_Flash_Data(void)
// addr = adres van waar af data uit flash gelezen wordt
// buffer is pointer naar buffer waarin data
{
s_CP2200_flash temp_cp2200_flash;
unsigned char *ptr = (unsigned char *)&cp2200_flash;
unsigned char *temp_ptr = (unsigned char *)&temp_cp2200_flash;
unsigned char ok = 1;
int loop;

  CP2200_Read_Flash_Data(&temp_cp2200_flash); // lees 1 byte
  for (loop = 0; loop < sizeof(s_CP2200_flash); loop++)
  {
    if (ptr[loop] != temp_ptr[loop])
    {
      ok = 0;
      break;
    }  
  }
  if (ok == 0)
  {
    // te schrijven data ongelijk aan data in flash
    CP2200_Erase_Flash_Data();
    for (loop = 0; loop < sizeof(s_CP2200_flash); loop++)
    {
      CP2200_Write_Flash_Byte(loop, ptr[loop]);
    }
  }  
}

//*****************************************************************************
s_CP2200_flash cp2200_flash = { 0, 0 };
s_CP2200_flash const cp2200_flash_default = 
{
  0xAA, // unsigned char control;  // 0xAA
  0x00, // unsigned char color;     // green display = 0; white display is 1 
};

void CP2200_Init_Flash_If_Needed(void)
{
  if (opt_alg.ethernet_enabled == 0)  // geen ethernet dan reset cp2200
    CP2200_Init_Flash_Data();
  else if (NetworkDeviceInitState != NETD_INIT_READY) // ethernet niet ready dan init cp2200 en reset ethernet
  {
    CP2200_Init_Flash_Data();
    Reset_NetworkDevice();
  }
}

void CP2200_Read_Display_Color(void)
// JP 21-02-2014 aangepast omdat detectie display kleur niet altijd goed werkt als SD-card geplaatst
// 5x proberen in plaats van 1 x
{
int count = 5;

  while (count)
  {
    CP2200_Init_Flash_If_Needed();
    CP2200_Read_Flash_Data(&cp2200_flash);
    if (cp2200_flash.control != 0xAA)
      count--; // foutief
    else
      count = 0; // ok
  }
  if (cp2200_flash.control != 0xAA)
  {
    cp2200_flash = cp2200_flash_default;
    CP2200_Write_Flash_Data();
  }
}
/*
void CP2200_Read_Display_Color(void)
{
  CP2200_Init_Flash_If_Needed();
  CP2200_Read_Flash_Data(&cp2200_flash);
  if (cp2200_flash.control != 0xAA)
  {
    cp2200_flash = cp2200_flash_default;
    CP2200_Write_Flash_Data();
  }
}
*/

void CP2200_Write_Display_Color(void)
{
s_CP2200_flash temp_cp2200_flash;

  CP2200_Init_Flash_If_Needed();
  CP2200_Read_Flash_Data(&temp_cp2200_flash);
  if (cp2200_flash.lcd_color != temp_cp2200_flash.lcd_color)
  {
    CP2200_Write_Flash_Data();
  }  
}

#else // ETHERNET

#pragma class HB=MEM_CP2200
char huge dummy_cp2200;
#pragma default_attributes

#endif // ETHERNET



