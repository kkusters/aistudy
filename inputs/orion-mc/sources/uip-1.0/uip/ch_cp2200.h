// CH_CP2200.H

#ifndef _CH_CP2200_H
#define _CH_CP2200_H

#ifdef ETHERNET

#include "uip_arp.h"
#include "uip-conf.h"

#define EBUF ((struct uip_eth_hdr *)&uip_buf[0])


//#define UIP_CONF_IPV6 0

// ATENTION: EMCMOD0 disable ALE
#define CP2200_RST P20_4
#define CP2200_RST_DP DP20_4

// CP2200 interrupt
#define CP2200_INT P9_5
#define CP2200_INT_DP DP9_5
#define CP2200_INT_ODP ODP9_5
#define CP2200_INT_ALTSEL0 AS0P9_5
#define CP2200_INT_ALTSEL1 AS1P9_5

// CPINFOH
#define CPINFOH_RXVALID 0x80
#define CPINFOH_RXVLAN 0x40
#define CPINFOH_RXUCF 0x20
#define CPINFOH_RXPCF 0x10
#define CPINFOH_RXCF 0x08
#define CPINFOH_RXADATA 0x04
#define CPINFOH_BCAST 0x02
#define CPINFOH_MCAST 0x01
// CPINFOL
#define CPINFOL_RXOK 0x80
#define CPINFOL_LENGHT 0x40
#define CPINFOL_LENERR 0x20
#define CPINFOL_CRCERR 0x10
#define CPINFOL_RXLEN 0x02
#define CPINFOL_RXDROP 0x01
// FLASHSTA
#define FLASHSTA_FLBUSY 0x08
#define FLASHSTA_FLWRITE 0x02
#define FLASHSTA_FLERASE 0x01
// INT0
#define INT0_EOPINT 0x80         // The last byte of a packet has been read.
#define INT0_RXEINT 0x40         // The receive FIFO is empty.
#define INT0_SELFINT 0x20        // Self Initialization has completed.
#define INT0_OSCINT 0x10         // Oscillator Initialization has completed.
#define INT0_FLWEINT 0x08        // A Flash write or erase operation has completed.
#define INT0_TXINT 0x04          // A packet has been transmitted.
#define INT0_RXFINT 0x02         // The receive FIFO is full.
#define INT0_RXINT 0x01          // A packet has been added to the receive buffer.
// INT0EN
#define INT0EN_EEOPINT 0x80      // Enable End of Packet Interrupt.
#define INT0EN_ERXEINT 0x40      // Enable Receive FIFO Empty Interrupt.
#define INT0EN_ESELFINT 0x20     // Enable Self Initialization Complete Interrupt.
#define INT0EN_EOSCINT 0x10      // Enable Oscillator Initialization Complete Interrupt.
#define INT0EN_EFLWEINT 0x08     // Enable Flash Write/Erase Operation Complete Interrupt.
#define INT0EN_ETXINT 0x04       // Enable Packet Transmitted Interrupt.
#define INT0EN_ERXFINT 0x02      // Enable Receive FIFO Full Interrupt.
#define INT0EN_ERXINT 0x01       // Enable Packet Received Interrupt.
// INT0RD
#define INT0RD_EOPINTR 0x80      // The last byte of a packet has been read.
#define INT0RD_RXEINTR 0x40      // The receive FIFO is empty.
#define INT0RD_SELFINTR 0x20     // Self Initialization has completed.
#define INT0RD_OSCINTR 0x10      // Oscillator Initialization has completed.
#define INT0RD_FLWEINTR 0x08     // A Flash write or erase operation has completed.
#define INT0RD_TXINTR 0x04       // A packet has been transmitted.
#define INT0RD_RXFINTR 0x02      // The receive FIFO is full.
#define INT0RD_RXINTR 0x01       // A packet has been added to the receive buffer.
// INT1
#define INT1_WAKEINT 0x20        // The device has been connected to a network since the last time WAKEINT was cleared.
#define INT1_LINKINT 0x10        // The link status has changed (device has been connected or removed from a network).
#define INT1_JABINT 0x08         // A jabber condition has been detected.
#define INT1_ANFINT 0x04         // Auto-Negotiation has failed.
#define INT1_RFINT  0x02         // A remote fault has been detected.
#define INT1_ANCINT 0x01         // Auto-Negotiation has completed.
// INT1EN
#define INT1EN_EWAKEINT 0x20     // Enable “Wake-on-Lan” Interrupt.
#define INT1EN_ELINKINT 0x10     // Enable Link Status Changed Interrupt.
#define INT1EN_EJABINT 0x08      // Enable Jabber Detected Interrupt.
#define INT1EN_EANFINT 0x04      // Enable Auto-Negotiation Failed Interrupt.
#define INT1EN_ERFINT  0x02      // Enable Remote Fault Interrupt.
#define INT1EN_EANCINT 0x01      // Enable Auto-Negotiation Complete Interrupt.
// INT1RD
#define INT1RD_WAKEINTR 0x20     // The device has been connected to a network since the last time WAKEINT was cleared.
#define INT1RD_LINKINTR 0x10     // The link status has changed (device has been connected or removed from a network).
#define INT1RD_JABINTR 0x08      // A jabber condition has been detected.
#define INT1RD_ANFINTR 0x04      // Auto-Negotiation has failed.
#define INT1RD_RFINTR  0x02      // A remote fault has been detected.
#define INT1RD_ANCINTR 0x01      // Auto-Negotiation has completed.
// IOPWR
#define IOPWR_ACTEN 0x08         // Activity LED enabled.
#define IOPWR_LINKEN 0x04        // Link (Link/Activity) LED enabled.
#define IOPWR_WEAKD 0x02         // Weak pull-ups are disabled.
// OSCPWR
#define OSCPWR_OSCOE 0x01
// PHYCF
#define PHYCF_SMSQ 0x80          // Receiver Smart Squelch is enabled.
#define PHYCF_LINKINTG 0x40      // Link integrity function is enabled.
#define PHYCF_JABBER 0x20        // Jabber protection function is enabled.
#define PHYCF_AUTONEG 0x10       // Auto-Negotiation function is enabled.
#define PHYCF_ADRFAULT  0x08     // Advertise (during auto-negotiation) that the CP2200/01 has the ability to detect remote faults.
#define PHYCF_ADPAUSE 0x04       // Indicates (during auto-negotiation) that the CP2200/01 does have pause packet capability.
#define PHYCF_AUTOPOL 0x02       // Automatic receiver polarity correction is enabled.
#define PHYCF_REVPOL 0x01        // The receiver polarity is reversed.
// PHYCN
#define PHYCN_PHYEN 0x80
#define PHYCN_TXEN 0x40
#define PHYCN_RXEN 0x20
#define PHYCN_DPLXMD 0x10
#define PHYCN_LBMD 0x08
#define PHYCN_LPRFAULT 0x04
#define PHYCN_POLREV 0x02
#define PHYCN_LINKSTA 0x01
// RSTEN
#define RSTEN_ESWRST 0x04
#define RSTEN_EPFRST 0x02
// RSTSTA
#define RSTSTA_SWRSI 0x04
#define RSTSTA_PORSI 0x02
#define RSTSTA_PINRSI 0x01
// RXCN
#define RXCN_RXINH 0x08
#define RXCN_RXCLRV 0x04
#define RXCN_RXSKIP 0x02
#define RXCN_RXCLEAR 0x01
// RXFILT
#define RXFILTS_IGNRUNT 0x08
#define RXFILTS_IGNERR 0x04
#define RXFILTS_IGNBCST 0x02
#define RXFILTS_IGNMCST 0x01
// RXSTA
#define RXSTA_CPEND 0x02
#define RXSTA_RXBUSY 0x01
// SWRST
#define SWRST_RESET 0x04
// TXBUSY
#define TXBUSYFLAG 0x01
// TXCN
#define TXCN_OVRRIDE  0x80    // Settings for bits 5, 4, 3, 2, and 1 in TXCN will be applied. MAC settings will be overridden.
#define TXCN_CRCENOV  0x20    // Enable CRC append on transmission.
#define TXCN_PADENOV  0x10    // Enable padding of short frames.
#define TXCN_TXPPKT   0x08    // Transmit a PAUSE control packet
#define TXCN_BCKPRES  0x04    // Back pressure will be applied on transmission (only valid in half duplex mode).
#define TXCN_FDPLXOV  0x02    // Transmit interface operates in full duplex mode.
#define TXCN_TXGO     0x01    // Set this bit to ‘1’ to begin transmission of a packet.
// TXPWR
#define TXPWR_PSAVED 0x80
// TXSTA2
#define TXSTA2_TXOK 0x80
#define TXSTA2_TXTYPE 0x40
#define TXSTA2_TXCLERR 0x20
#define TXSTA2_TXCRCER 0x10
#define TXSTA2_TXCOL3 0x08
#define TXSTA2_TXCOL2 0x04
#define TXSTA2_TXCOL1 0x02
#define TXSTA2_TXCOL0 0x01
// TXSTA3
#define TXSTA3_TXURUN 0x80        // Packet aborted due to data under-run condition.
#define TXSTA3_TXJUMBO 0x40
#define TXSTA3_TXLTCL 0x20        // Collision detected after the 51.2 ms collision window.
#define TXSTA3_TXEXCL 0x10        // Packet aborted due to detection of 16 or more collisions.
#define TXSTA3_TXEXDE 0x08        // Packet was transmitted with an excessive delay (greater than 2.42 ms).
#define TXSTA3_TXDE 0x04          // Packet was transmitted with a non-excessive delay (less than 2.42 ms).
#define TXSTA3_TXBCAST 0x02       // Transmitted packet had a broadcast destination address.
#define TXSTA3_TXMCAST 0x01       // Transmit packet had a multicast destination address.
// TXSTA6
#define TXSTA6_TXVLAN 0x08
#define TXSTA6_BCKPRES 0x04
#define TXSTA6_TXPF 0x02
#define TXSTA6_TXCF 0x01
// VDMCN
#define VDMCN_VDMEN 0x80         //  VDD Monitor Enable
#define VDMCN_VDDSTAT 0x40       //  VDD voltage is above the VDD Monitor threshold

// Indirect MAC registers
#define MACCN  0x00 // MAC Control. Used to enable reception and other options.
#define MACCF 0x01 // MAC Configuration. Used to configure padding options and other settings.
#define IPGT 0x02 // Back-to-Back Interpacket Delay. Sets the Back-to-Back Interpacket Delay.
#define IPGR 0x03 // Non-Back-to-Back Interpacket Delay. Sets the Non-Back-to-Back Interpacket Delay.
#define CWMAXR 0x04 // Collision Window and Maximum Retransmit. Sets the collision window size and the maximum number of retransmits allowed.
#define MAXLEN 0x05 // Maximum Frame Length. Sets the maximum receive frame length.
#define MACAD0 0x10 // MAC Address 
#define MACAD1 0x11 // MAC Address 
#define MACAD2 0x12 // MAC Address 

typedef struct  
{                           
  unsigned char dummy_0x00;
  unsigned char RXAUTORD;    // 0x01 *
  unsigned char RAMRXDATA;   // 0x02 *     RXFIFO RAM Data Register page 24
  unsigned char TXAUTOWR;    // 0x03 *     Transmit Data AutoWrite page 53
  unsigned char RAMTXDATA;   // 0x04 *     TXBUFF RAM Data Register page 24
  unsigned char FLASHAUTORD; // 0x05 *     Flash AutoRead w/ increment page 77
  unsigned char FLASHDATA;   // 0x06 *     Flash Read/Write Data Register page 77
  unsigned char dummy_0x07;
  unsigned char RAMADDRH;    // 0x08 *     RAM Address Pointer High Byte page 24
  unsigned char RAMADDRL;    // 0x09 *     RAM Address Pointer Low Byte page 24
  unsigned char MACADDR;     // 0x0A *     MAC Address Pointer page 79
  unsigned char MACDATAH;    // 0x0B *     MAC Data Register High Byte page 79
  unsigned char MACDATAL;    // 0x0C *     MAC Data Register Low Byte page 79
  unsigned char MACRW;       // 0x0D *     MAC Read/Write Initiate page 79
  unsigned char RXHASHH;     // 0x0E *     Receive Hash Table High Byte page 62
  unsigned char RXHASHL;     // 0x0F *     Receive Hash Table Low Byte page 63
  unsigned char RXFILT;      // 0x10 *     Receive Filter Configuration
  unsigned char RXCN;        // 0x11 *     Receive Control page 61
  unsigned char RXSTA;       // 0x12 *     Receive Status page
  unsigned char VDMCN;       // 0x13 *     VDD Monitor Control Register page 39
  unsigned char dummy_0x14;
  unsigned char RXFIFOTAILH; // 0x15 *     Receive Buffer Tail Pointer High Byte page 71
  unsigned char RXFIFOTAILL; // 0x16 *     Receive Buffer Tail Pointer Low Byte page 71
  unsigned char RXFIFOHEADH; // 0x17 *     Receive Buffer Head Pointer High Byte page 71
  unsigned char RXFIFOHEADL; // 0x18 *     Receive Buffer Head Pointer Low Byte page 71
  unsigned char dummy_0x19;
  unsigned char CPTLB;       // 0x1A *     Current RX Packet TLB Number page 67
  unsigned char dummy_0x1B;
  unsigned char TLBVALID;    // 0x1C *     TLB Valid Indicators page 68
  unsigned char CPINFOH;     // 0x1D *     Current RX Packet Information High Byte page 63
  unsigned char CPINFOL;     // 0x1E *     Current RX Packet Information Low Byte page 64
  unsigned char CPLENH;      // 0x1F *     Current RX Packet Length High Byte page 64
  unsigned char CPLENL;      // 0x20 *     Current RX Packet Length Low Byte page 64
  unsigned char CPADDRH;     // 0x21 *     Current RX Packet Address High Byte page 65
  unsigned char CPADDRL;     // 0x22 *     Current RX Packet Address Low Byte page 65
  unsigned char TLB0INFOH;   // 0x23 *     TLB0 Information High Byte page 68
  unsigned char TLB0INFOL;   // 0x24 *     TLB0 Information Low Byte page 69
  unsigned char TLB0LENH;    // 0x25 *     TLB0 Length High Byte page 69
  unsigned char TLB0LENL;    // 0x26 *     TLB0 Length Low Byte page 70
  unsigned char TLB0ADDRH;   // 0x27 *     TLB0 Address High Byte page 70
  unsigned char TLB0ADDRL;   // 0x28 *     TLB0 Address Low Byte page 70
  unsigned char TLB1INFOH;   // 0x29 *     TLB1 Information High Byte page 68
  unsigned char TLB1INFOL;   // 0x2A *     TLB1 Information Low Byte page 69
  unsigned char TLB1LENH;    // 0x2B *     TLB1 Length High Byte page 69
  unsigned char TLB1LENL;    // 0x2C *     TLB1 Length Low Byte page 70
  unsigned char TLB1ADDRH;   // 0x2D *     TLB1 Address High Byte page 70
  unsigned char TLB1ADDRL;   // 0x2E *     TLB1 Address Low Byte page 70
  unsigned char TLB2INFOH;   // 0x2F *     TLB2 Information High Byte page 68
  unsigned char TLB2INFOL;   // 0x30 *     TLB2 Information Low Byte page 69
  unsigned char TLB2LENH;    // 0x31 *     TLB2 Length High Byte page 69
  unsigned char TLB2LENL;    // 0x32 *     TLB2 Length Low Byte page 70
  unsigned char TLB2ADDRH;   // 0x33 *     TLB2 Address High Byte page 70
  unsigned char TLB2ADDRL;   // 0x34 *     TLB2 Address Low Byte page 70
  unsigned char TLB3INFOH;   // 0x35 *     TLB3 Information High Byte page 68
  unsigned char TLB3INFOL;   // 0x36 *     TLB3 Information Low Byte page 69
  unsigned char TLB3LENH;    // 0x37 *     TLB3 Length High Byte page 69
  unsigned char TLB3LENL;    // 0x38 *     TLB3 Length Low Byte page 70
  unsigned char TLB3ADDRH;   // 0x39 *     TLB3 Address High Byte page 70
  unsigned char TLB3ADDRL;   // 0x3A *     TLB3 Address Low Byte page 70
  unsigned char TLB4INFOH;   // 0x3B *     TLB4 Information High Byte page 68
  unsigned char TLB4INFOL;   // 0x3C *     TLB4 Information Low Byte page 69
  unsigned char TLB4LENH;    // 0x3D *     TLB4 Length High Byte page 69
  unsigned char TLB4LENL;    // 0x3E *     TLB4 Length Low Byte page 70
  unsigned char TLB4ADDRH;   // 0x3F *     TLB4 Address High Byte
  unsigned char TLB4ADDRL;   // 0x40 *     TLB4 Address Low Byte page 70
  unsigned char TLB5INFOH;   // 0x41 *     TLB5 Information High Byte page 68
  unsigned char TLB5INFOL;   // 0x42 *     TLB5 Information Low Byte page 69
  unsigned char TLB5LENH;    // 0x43 *     TLB5 Length High Byte page 69
  unsigned char TLB5LENL;    // 0x44 *     TLB5 Length Low Byte page 70
  unsigned char TLB5ADDRH;   // 0x45 *     TLB5 Address High Byte page 70
  unsigned char TLB5ADDRL;   // 0x46 *     TLB5 Address Low Byte page 70
  unsigned char TLB6INFOH;   // 0x47 *     TLB6 Information High Byte page 68
  unsigned char TLB6INFOL;   // 0x48 *     TLB6 Information Low Byte page 69
  unsigned char TLB6LENH;    // 0x49 *     TLB6 Length High Byte page 69
  unsigned char TLB6LENL;    // 0x4A *     TLB6 Length Low Byte page 70
  unsigned char TLB6ADDRH;   // 0x4B *     Address High Byte page 70
  unsigned char TLB6ADDRL;   // 0x4C *     Address Low Byte page 70
  unsigned char TLB7INFOH;   // 0x4D *     TLB7 Information High Byte page 68
  unsigned char TLB7INFOL;   // 0x4E *     TLB7 Information Low Byte page 69
  unsigned char TLB7LENH;    // 0x4F *     TLB7 Length High Byte page 69
  unsigned char TLB7LENL;    // 0x50 *     TLB7 Length Low Byte page 70
  unsigned char TLB7ADDRH;   // 0x51 *     TLB7 Address High Byte page 70
  unsigned char TLB7ADDRL;   // 0x52 *     TLB7 Address Low Byte page 70
  unsigned char TXCN;        // 0x53 *     Transmit Control page 51
  unsigned char TXBUSY;      // 0x54 *     Transmit Busy Indicator page 51
  unsigned char TXPAUSEH;    // 0x55 *     Transmit Pause High Byte page 52
  unsigned char TXPAUSEL;    // 0x56 *     Transmit Pause Low Byte page 52
  unsigned char TXENDH;      // 0x57 *     Transmit Data Ending Address High Byte page 53
  unsigned char TXENDL;      // 0x58 *     Transmit Data Ending Address Low Byte
  unsigned char TXSTARTH;    // 0x59 *     Transmit Data Starting Address High Byte page 52
  unsigned char TXSTARTL;    // 0x5A *     Transmit Data Starting Address Low Byte page 52
  unsigned char RXFIFOSTA;   // 0x5B *     Receive Buffer Status page 72
  unsigned char TXSTA6;      // 0x5C *     Transmit Status Vector 6 page 54
  unsigned char TXSTA5;      // 0x5D *     Transmit Status Vector 5 page 54
  unsigned char TXSTA4;      // 0x5E *     Transmit Status Vector 4 page 55
  unsigned char TXSTA3;      // 0x5F *     Transmit Status Vector 3 page 55
  unsigned char TXSTA2;      // 0x60 *     Transmit Status Vector 2 page 56
  unsigned char TXSTA1;      // 0x61 *     Transmit Status Vector 1 page 56
  unsigned char TXSTA0;      // 0x62 *     Transmit Status Vector 0 page 57
  unsigned char INT0;        // 0x63 *     Interrupt Status Register 0 (Self-Clearing) page 31
  unsigned char INT0EN;      // 0x64 *     Interrupt Enable Register 0 page 33
  unsigned char dummy_0x65;
  unsigned char dummy_0x66;
  unsigned char FLASHKEY;    // 0x67 *     Flash Lock and Key page 76
  unsigned char FLASHADDRL;  // 0x68 *     Flash Address Pointer Low Byte page 76
  unsigned char FLASHADDRH;  // 0x69 *     Flash Address Pointer High Byte page 76
  unsigned char FLASHERASE;  // 0x6A *     Flash Erase page 77
  unsigned char dummy_0x6B;
  unsigned char dummy_0x6C;
  unsigned char dummy_0x6D;
  unsigned char dummy_0x6E;
  unsigned char dummy_0x6F;
  unsigned char IOPWR;       // 0x70 *     Port Input/Output Power page 45
  unsigned char dummy_0x71;
  unsigned char RSTEN;       // 0x72 *     Reset Enable Register page 42
  unsigned char RSTSTA;      // 0x73 *     Reset Source Status Register page 41
  unsigned char dummy_0x74;
  unsigned char SWRST;       // 0x75 *     Software Reset Register page
  unsigned char INT0RD;      // 0x76 *     Interrupt Status Register 0 (Read-Only) page 32
  unsigned char dummy_0x77;
  unsigned char PHYCN;       // 0x78 *     Physical Layer Control page 91
  unsigned char PHYCF;       // 0x79 *     Physical Layer Configuration page 92
  unsigned char TXPWR;       // 0x7A *     Transmitter Power page 46
  unsigned char FLASHSTA;    // 0x7B *     Flash Status page 75
  unsigned char OSCPWR;      // 0x7C *     Oscillator Power page 46
  unsigned char INT1EN;      // 0x7D *     Interrupt Enable Register 1 page 36
  unsigned char INT1RD;      // 0x7E *     Interrupt Status Register 1 (Read-Only) page 35
  unsigned char INT1;        // 0x7F *     Interrupt Status Register 1 (Self-Clearing) page 34
  unsigned char PHYSTA;      // 0x80 *     Physical Layer Status page 93
} s_cp2200;

extern unsigned char cp2200_InitState;
extern volatile s_cp2200 huge cp2200;

void Reset_CP2200(void);
unsigned char CP2200_Init(void);
unsigned char CP2200_Transmit_Reset(void); // JP 24-01-08
//void CP2200_Transmit_Reset(void); // JP 24-01-08
void CP2200_transmit(void);
u16_t CP2200_receive(void);

typedef struct // struct mag maximaal 512 bytes zijn is 1 flash pagina
{
  unsigned char control;  // 0xAA
  unsigned char lcd_color;     // green display = 0; white display is 1
} s_CP2200_flash;

extern s_CP2200_flash cp2200_flash;

void CP2200_Read_Display_Color(void);
void CP2200_Write_Display_Color(void);

#endif // ETHERNET

#endif // _CH_CP2200_H
