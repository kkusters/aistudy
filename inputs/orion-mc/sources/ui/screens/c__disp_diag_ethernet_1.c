// C__DISP_DIAG_ETHERNET_1.C

#include <stdio.h> 

#include "ch_define.h"

#ifdef ETHERNET

#include "ch_asc0.h"
#include "ch_const.h"
#include "ch_disp.h"
#include "ch_disp_diagnose_0.h"
#include "ch_disp_func.h"
#include "ch_disp_value.h"
#include "ch_ethernet_app.h"
#include "ch_key.h"
#include "ch_lcd_7.h"
#include "ch_lcd_10.h"
#include "ch_lcd_14.h"
#include "ch_lcd_bitmap.h"
#include "ch_lcd_pixel.h"
#include "ch_string.h"
#include "networkdevice.h"
#include "uip_arp.h"
#include "uip-conf.h"
#include "ch_disp_diag_ethernet_1.h"

extern s_screen screen_diag_ethernet_1;

static void Disp_Ethernet_Control(void);
static void Enter_Reset_Diag_Ethernet(void);
static void Arrow_Reset_Ethernet(void);
static void Enter_Reset_Ethernet(void);

unsigned char ethernet_reset = 0;

static s_tekst_18 tekst_mac_address;
s_tekst_16 tekst_ip_adres[3];

static s_disp_agri_header const disp_header =              { Disp_Draw_Agri_Header, FN5, EMPTY, &tekst.Diag_CAN_LOCAL_10, HK_GEEN, 0, 2};
static s_disp_func const disp_ehternet_control = { Disp_Control_Func, Disp_Ethernet_Control };
static void * const lcd_disp_header[] = { &disp_header, &disp_ehternet_control, 0};

static s_disp_tekst const disp_port_str         = { Disp_Draw_Tekst_L,  37, 20, &tekst.Port_10 };
static s_disp_value const disp_port_val         = { Disp_Draw_Value,   217, 20, (SIZE_14 | RECHTS), UINT, 0, &opt_alg.ethernet_port };
static s_disp_tekst const disp_IP_str           = { Disp_Draw_Tekst_L,  37, 20, &tekst.IP_10 };
//static s_disp_tekst const disp_punt_0_str     = { Disp_Draw_Tekst_L, 118, 20, &tekst_punt_14 };
//static s_disp_tekst const disp_punt_1_str     = { Disp_Draw_Tekst_L, 152, 20, &tekst_punt_14 };
//static s_disp_tekst const disp_punt_2_str     = { Disp_Draw_Tekst_L, 186, 20, &tekst_punt_14 };
static s_disp_tekst_add const disp_punt_str     = { Disp_Draw_Tekst_Add_R, &tekst_punt_14 };
//static s_disp_value const disp_IP_0_val       = { Disp_Draw_Value,   115, 20, (SIZE_14 | RECHTS), UCHAR, 0, &opt_alg.ethernet[0][0] };
//static s_disp_value const disp_IP_1_val       = { Disp_Draw_Value,   149, 20, (SIZE_14 | RECHTS), UCHAR, 0, &opt_alg.ethernet[0][1] };
//static s_disp_value const disp_IP_2_val       = { Disp_Draw_Value,   183, 20, (SIZE_14 | RECHTS), UCHAR, 0, &opt_alg.ethernet[0][2] };
static s_disp_value const disp_IP_3_val         = { Disp_Draw_Value,   217, 20, (SIZE_14 | RECHTS), UCHAR, 0, &opt_alg.ethernet[0][3] };
static s_disp_value_add const disp_IP_2_val     = { Disp_Draw_Value_Add, (SIZE_14 | RECHTS), UCHAR, 0, &opt_alg.ethernet[0][2] };
static s_disp_value_add const disp_IP_1_val     = { Disp_Draw_Value_Add, (SIZE_14 | RECHTS), UCHAR, 0, &opt_alg.ethernet[0][1] };
static s_disp_value_add const disp_IP_0_val     = { Disp_Draw_Value_Add, (SIZE_14 | RECHTS), UCHAR, 0, &opt_alg.ethernet[0][0] };
static s_disp_tekst const disp_Mask_str         = { Disp_Draw_Tekst_L,  37, 20, &tekst.Mask_10 };
//static s_disp_value const disp_Mask_0_val     = { Disp_Draw_Value,   115, 20, (SIZE_14 | RECHTS), UCHAR, 0, &opt_alg.ethernet[1][0] };
//static s_disp_value const disp_Mask_1_val     = { Disp_Draw_Value,   149, 20, (SIZE_14 | RECHTS), UCHAR, 0, &opt_alg.ethernet[1][1] };
//static s_disp_value const disp_Mask_2_val     = { Disp_Draw_Value,   183, 20, (SIZE_14 | RECHTS), UCHAR, 0, &opt_alg.ethernet[1][2] };
static s_disp_value const disp_Mask_3_val       = { Disp_Draw_Value,   217, 20, (SIZE_14 | RECHTS), UCHAR, 0, &opt_alg.ethernet[1][3] };
static s_disp_value_add  const disp_Mask_2_val  = { Disp_Draw_Value_Add, (SIZE_14 | RECHTS), UCHAR, 0, &opt_alg.ethernet[1][2] };
static s_disp_value_add  const disp_Mask_1_val  = { Disp_Draw_Value_Add, (SIZE_14 | RECHTS), UCHAR, 0, &opt_alg.ethernet[1][1] };
static s_disp_value_add  const disp_Mask_0_val  = { Disp_Draw_Value_Add, (SIZE_14 | RECHTS), UCHAR, 0, &opt_alg.ethernet[1][0] };
static s_disp_tekst const disp_Gate_str         = { Disp_Draw_Tekst_L,  37, 20, &tekst.Gate_10 };
static s_disp_value const disp_Gate_3_val       = { Disp_Draw_Value,   217, 20, (SIZE_14 | RECHTS), UCHAR, 0, &opt_alg.ethernet[2][3] };
static s_disp_value_add  const disp_Gate_2_val  = { Disp_Draw_Value_Add, (SIZE_14 | RECHTS), UCHAR, 0, &opt_alg.ethernet[2][2] };
static s_disp_value_add  const disp_Gate_1_val  = { Disp_Draw_Value_Add, (SIZE_14 | RECHTS), UCHAR, 0, &opt_alg.ethernet[2][1] };
static s_disp_value_add  const disp_Gate_0_val  = { Disp_Draw_Value_Add, (SIZE_14 | RECHTS), UCHAR, 0, &opt_alg.ethernet[2][0] };
static s_disp_tekst const disp_MAC_str          = { Disp_Draw_Tekst_L,  37, 20, &tekst.MAC_10 };
static s_disp_tekst const disp_MAC_address_str  = { Disp_Draw_Tekst_R, 217, 20, &tekst_mac_address };

static s_disp_tekst const disp_address_str       = { Disp_Draw_Tekst_L,  37, 20, &tekst.PC_10 };
static s_disp_value_add const disp_address_0_val = { Disp_Draw_Value_Add, (SIZE_10 | LINKS), UCHAR, 0, &uchar_1 };
static s_disp_value_add const disp_address_1_val = { Disp_Draw_Value_Add, (SIZE_10 | LINKS), UCHAR, 0, &uchar_2 };
static s_disp_value_add const disp_address_2_val = { Disp_Draw_Value_Add, (SIZE_10 | LINKS), UCHAR, 0, &uchar_3 };
static s_disp_tekst const disp_address_0_str     = { Disp_Draw_Tekst_R, 217, 20, &tekst_ip_adres[0] };
static s_disp_tekst const disp_address_1_str     = { Disp_Draw_Tekst_R, 217, 20, &tekst_ip_adres[1] };
static s_disp_tekst const disp_address_2_str     = { Disp_Draw_Tekst_R, 217, 20, &tekst_ip_adres[2] };

static s_disp_tekst const disp_out_cnt_str      = { Disp_Draw_Tekst_L, 37, 20, &tekst.TX_10 };
static s_disp_value const disp_out_cnt_val      = { Disp_Draw_Value,  147, 20, (SIZE_14 | RECHTS), UINT, 0, &ethernet_diag_out_cnt };
static s_disp_value const disp_out_cnt_peak_val = { Disp_Draw_Value,  207, 20, (SIZE_14 | RECHTS), UINT, 0, &ethernet_diag_out_cnt_peak };
static s_disp_tekst const disp_in_cnt_str       = { Disp_Draw_Tekst_L, 37, 20, &tekst.RX_10 };
static s_disp_value const disp_in_cnt_val       = { Disp_Draw_Value,  147, 20, (SIZE_14 | RECHTS), UINT, 0, &ethernet_diag_in_cnt };
static s_disp_value const disp_in_cnt_peak_val  = { Disp_Draw_Value,  207, 20, (SIZE_14 | RECHTS), UINT, 0, &ethernet_diag_in_cnt_peak };

static s_disp_tekst const disp_rtx_str          = { Disp_Draw_Tekst_L, 37, 20, &tekst.RTX_10 };
static s_disp_value const disp_rtx_val          = { Disp_Draw_Value,  207, 20, (SIZE_14 | RECHTS), UINT, 0, &eth_status.retransmit_cnt };
static s_disp_tekst const disp_no_ack_str       = { Disp_Draw_Tekst_L, 37, 20, &tekst.NO_ACK_10 };
static s_disp_value const disp_no_ack_val       = { Disp_Draw_Value,  207, 20, (SIZE_14 | RECHTS), UINT, 0, &eth_status.no_ack_send_cnt };
static s_disp_tekst const disp_connect_str      = { Disp_Draw_Tekst_L, 37, 20, &tekst.CONNECT_10 };
static s_disp_value const disp_connect_val      = { Disp_Draw_Value,  147, 20, (SIZE_14 | RECHTS), UINT, 0, &eth_status.connect_cnt };
static s_disp_value const disp_disconnect_val   = { Disp_Draw_Value,  207, 20, (SIZE_14 | RECHTS), UINT, 0, &eth_status.disconnect_cnt };

static s_disp_tekst const disp_reset_ethernet_str    = { Disp_Draw_Tekst_L, 12, 20, &tekst.Reset_Ethernet_10 };
static s_disp_bitmap_array const disp_reset_ethernet_bmp = { Disp_Draw_Bitmap_Array,  190, 6, bmp_false_true, UCHAR, &ethernet_reset, 2 };

static void * const lcd_port_disp[]      = { &disp_globe, &disp_port_str, &disp_port_val, 0 };
static void * const lcd_IP_disp[]        = { &disp_globe, &disp_IP_str,   &disp_IP_3_val,   &disp_punt_str, &disp_IP_2_val,   &disp_punt_str, &disp_IP_1_val,   &disp_punt_str, &disp_IP_0_val,   0 };
static void * const lcd_mask_disp[]      = { &disp_globe, &disp_Mask_str, &disp_Mask_3_val, &disp_punt_str, &disp_Mask_2_val, &disp_punt_str, &disp_Mask_1_val, &disp_punt_str, &disp_Mask_0_val, 0 };
static void * const lcd_gate_disp[]      = { &disp_globe, &disp_Gate_str, &disp_Gate_3_val, &disp_punt_str, &disp_Gate_2_val, &disp_punt_str, &disp_Gate_1_val, &disp_punt_str, &disp_Gate_0_val, 0 };
static void * const lcd_mac_disp[]       = { &disp_globe, &disp_MAC_str, &disp_MAC_address_str, 0 };
static void * const lcd_address_0_disp[] = { &disp_globe, &disp_address_str, &disp_space_10_L, &disp_address_0_val, &disp_address_0_str, 0 };
static void * const lcd_address_1_disp[] = { &disp_globe, &disp_address_str, &disp_space_10_L, &disp_address_1_val, &disp_address_1_str, 0 };
static void * const lcd_address_2_disp[] = { &disp_globe, &disp_address_str, &disp_space_10_L, &disp_address_2_val, &disp_address_2_str, 0 };
static void * const lcd_in_cnt_disp[]    = { &disp_globe, &disp_in_cnt_str, &disp_in_cnt_val, &disp_in_cnt_peak_val, 0 };
static void * const lcd_out_cnt_disp[]   = { &disp_globe, &disp_out_cnt_str, &disp_out_cnt_val, &disp_out_cnt_peak_val, 0 };
static void * const lcd_rtx_disp[]       = { &disp_globe, &disp_rtx_str, &disp_rtx_val, 0 };
static void * const lcd_no_ack_disp[]    = { &disp_globe, &disp_no_ack_str, &disp_no_ack_val, 0 };
static void * const lcd_connect_disp[]   = { &disp_globe, &disp_connect_str, &disp_connect_val, &disp_disconnect_val, 0 };
static void * const lcd_reset_ethernet_disp[] = { &disp_reset_ethernet_str, &disp_reset_ethernet_bmp, 0 };

static s_key_value const key_reset_ethernet_value = { UCHAR, 1, &ethernet_reset, &uchar_0, &uchar_1 };

s_key_action const diag_ethernet_1_key_action[] =
{
  { // Port
    1,                              // nr
    0,                              // index
    (unsigned char *)&option_on,    // option
    (unsigned char *)&option_index_0,// Optie index 
    lcd_port_disp,                  // display
    0,                              // cursor
    &dummy_value,                   // *value
    Dummy_Func,                     // void (*number)(void); 
    Arrow_Change_Func,              // void (*arrow)(void); 
    Dummy_Func,                     // void (*number)(void); 
  },
  { // IP
    2,                              // nr
    0,                              // index
    (unsigned char *)&option_on,    // option
    (unsigned char *)&option_index_0,// Optie index 
    lcd_IP_disp,                    // display
    0,                              // cursor
    &dummy_value,                   // *value
    Dummy_Func,                     // void (*number)(void); 
    Arrow_Change_Func,              // void (*arrow)(void); 
    Dummy_Func,                     // void (*number)(void); 
  },
  { // Mask
    3,                              // nr
    0,                              // index
    (unsigned char *)&option_on,    // option
    (unsigned char *)&option_index_0,// Optie index 
    lcd_mask_disp,                  // display
    0,                              // cursor
    &dummy_value,                   // *value
    Dummy_Func,                     // void (*number)(void); 
    Arrow_Change_Func,              // void (*arrow)(void); 
    Dummy_Func,                     // void (*number)(void); 
  },
  { // Gate
    4,                              // nr
    0,                              // index
    (unsigned char *)&option_on,    // option
    (unsigned char *)&option_index_0,// Optie index 
    lcd_gate_disp,                  // display
    0,                              // cursor
    &dummy_value,                   // *value
    Dummy_Func,                     // void (*number)(void); 
    Arrow_Change_Func,              // void (*arrow)(void); 
    Dummy_Func,                     // void (*number)(void); 
  },
  { // Mac_Address
    5,                              // nr
    0,                              // index
    (unsigned char *)&option_on,    // option
    (unsigned char *)&option_index_0,// Optie index 
    lcd_mac_disp,                   // display
    0,                              // cursor
    &dummy_value,                   // *value
    Dummy_Func,                     // void (*number)(void); 
    Arrow_Change_Func,              // void (*arrow)(void); 
    Dummy_Func,                     // void (*number)(void); 
  },
  { // eerste ethernet adres
    6,                              // nr
    0,                              // index
    (unsigned char *)&option_on,    // option
    (unsigned char *)&option_index_0,// Optie index 
    lcd_address_0_disp,             // display
    0,                              // cursor
    &dummy_value,                   // *value
    Dummy_Func,                     // void (*number)(void); 
    Arrow_Change_Func,              // void (*arrow)(void); 
    Dummy_Func,                     // void (*number)(void); 
  },
  { // tweede ethernet adres
    7,                              // nr
    0,                              // index
    (unsigned char *)&option_on,    // option
    (unsigned char *)&option_index_0,// Optie index 
    lcd_address_1_disp,             // display
    0,                              // cursor
    &dummy_value,                   // *value
    Dummy_Func,                     // void (*number)(void); 
    Arrow_Change_Func,              // void (*arrow)(void); 
    Dummy_Func,                     // void (*number)(void); 
  },
  { // derde ethernet adres
    8,                              // nr
    0,                              // index
    (unsigned char *)&option_on,    // option
    (unsigned char *)&option_index_0,// Optie index 
    lcd_address_2_disp,             // display
    0,                              // cursor
    &dummy_value,                   // *value
    Dummy_Func,                     // void (*number)(void); 
    Arrow_Change_Func,              // void (*arrow)(void); 
    Dummy_Func,                     // void (*number)(void); 
  },
  { // TXOK
    9,                              // nr
    0,                              // index
    (unsigned char *)&option_on,    // option
    (unsigned char *)&option_index_0,// Optie index 
    lcd_out_cnt_disp,               // display
    0,                              // cursor
    &dummy_value,                   // *value
    Dummy_Func,                     // void (*number)(void); 
    Arrow_Change_Func,              // void (*arrow)(void); 
    Enter_Reset_Diag_Ethernet,      // void (*enter)(void);
  },
  { // RXOK
    10,                             // nr
    0,                              // index
    (unsigned char *)&option_on,    // option
    (unsigned char *)&option_index_0,// Optie index 
    lcd_in_cnt_disp,                // display
    0,                              // cursor
    &dummy_value,                   // *value
    Dummy_Func,                     // void (*number)(void); 
    Arrow_Change_Func,              // void (*arrow)(void); 
    Enter_Reset_Diag_Ethernet,      // void (*enter)(void);
  },
  { // RTX
    11,                             // nr
    0,                              // index
    (unsigned char *)&option_on,    // option
    (unsigned char *)&option_index_0,// Optie index 
    lcd_rtx_disp,                   // display
    0,                              // cursor
    &dummy_value,                   // *value
    Dummy_Func,                     // void (*number)(void); 
    Arrow_Change_Func,              // void (*arrow)(void); 
    Enter_Reset_Diag_Ethernet,      // void (*enter)(void);
  },
  { // NO_ACK
    12,                             // nr
    0,                              // index
    (unsigned char *)&option_on,    // option
    (unsigned char *)&option_index_0,// Optie index 
    lcd_no_ack_disp,                // display
    0,                              // cursor
    &dummy_value,                   // *value
    Dummy_Func,                     // void (*number)(void); 
    Arrow_Change_Func,              // void (*arrow)(void); 
    Enter_Reset_Diag_Ethernet,      // void (*enter)(void);
  },
  { // connect
    13,                             // nr
    0,                              // index
    (unsigned char *)&option_on,    // option
    (unsigned char *)&option_index_0,// Optie index 
    lcd_connect_disp,               // display
    0,                              // cursor
    &dummy_value,                   // *value
    Dummy_Func,                     // void (*number)(void); 
    Arrow_Change_Func,              // void (*arrow)(void); 
    Enter_Reset_Diag_Ethernet,      // void (*enter)(void);
  },
  { // Reset Backbone
    14,                             // nr
    0,                              // index
    (unsigned char *)&option_on,    // option
    (unsigned char *)&option_index_0,// Optie index 
    lcd_reset_ethernet_disp,        // display
    0,                              // cursor
    &dummy_value,                   // *value
    Dummy_Func,                     // void (*number)(void); 
    Arrow_Change_Func,              // void (*arrow)(void); 
    Enter_Reset_Diag_Ethernet,      // void (*enter)(void);
  },
  {	
    14,                             // nr
    1,                              // index
    (unsigned char *)&option_on,    // option
    (unsigned char *)&option_index_0,// Optie index 
    lcd_reset_ethernet_disp,        // display
    &disp_cursor_207_27_18,         // cursor
	&key_reset_ethernet_value,      // *value
    Dummy_Func,                     // void (*number)(void); 
    Arrow_Reset_Ethernet,           // void (*arrow)(void); 
	Enter_Reset_Ethernet,           // void (*enter)(void);
  },
};

s_screen screen_diag_ethernet_1;
s_screen const screen_diag_ethernet_1_default =
{
  1, // functie nr
  0, // index
  3, // max rel
  0, // rel actief
  0, // nummer weergegeven bin aantal ingeschakelde functie nummers
  0, // aantal ingeschakelde functie nummers
  lcd_disp_header,
  {0,0,0,0,0,0},
  0,3,19,
  0,3,19+28,
  0,3,19+28+28,
  &diag_ethernet_1_key_action[0], // first_action
  &diag_ethernet_1_key_action[sizeof(diag_ethernet_1_key_action)/sizeof(s_key_action) - 1], // last action 
  0, // change_flag
  0,
  0,
  0,
  0,
  0,
  0,
  &screen_diagnose_0,
  0 // function for prev and next key
};


void Control_Screen_Diag_Ethernet_1(void)
// zorg voor een correcte inhoud in klimaat scherm 0
{
  Control_Screen(&screen_diag_ethernet_1, &screen_diag_ethernet_1_default, 1, 3);
}

static void Disp_Ethernet_Control(void)
{
int loop;
int nr = 0;

  tekst_mac_address.max_char = 18;
  tekst_mac_address.max_bits = 150;
  tekst_mac_address.font_type = SIZE_14;
  sprintf(tekst_mac_address.string,"%02X:%02X:%02X:%02X:%02X:%02X", uip_ethaddr.addr[0],
                                                                    uip_ethaddr.addr[1],
                                                                    uip_ethaddr.addr[2],
                                                                    uip_ethaddr.addr[3],
                                                                    uip_ethaddr.addr[4],
                                                                    uip_ethaddr.addr[5]);
  for (loop = 0; loop < 4; loop++)
  {
    if (uip_conns[loop].appstate.toegewezen)
    {
      tekst_ip_adres[nr].max_char = 16;
      tekst_ip_adres[nr].max_bits = 150;
      tekst_ip_adres[nr].font_type = SIZE_14;
      sprintf(tekst_ip_adres[nr].string, "%i.%i.%i.%i", uip_conns[loop].ripaddr[0] & 0x00FF,
                                                        (uip_conns[loop].ripaddr[0] >> 8) & 0x00FF,
                                                        uip_conns[loop].ripaddr[1] & 0x00FF,
                                                        (uip_conns[loop].ripaddr[1] >> 8) & 0x00FF);
      nr++;  
      if (nr == 3)                                                              
        break;                                                
    }
  }
  while (nr < 3)
  {
    tekst_ip_adres[nr] = tekst.NO_CONNECTION_14;
    nr++;
  }  
}

void If_Exist_Goto_Screen_Diag_Ethernet_1(void)
{
  Control_Screen_Diag_Ethernet_1();
  if (screen_diag_ethernet_1.nr_aantal)
  {                              
    Disp_Ethernet_Control();          
    Next_Screen(&screen_diag_ethernet_1);
  }  
}

static void Enter_Reset_Diag_Ethernet(void)
{
  ethernet_diag_reset_flag = 1;
}

static void Arrow_Reset_Ethernet(void)
{
  switch (key)
  {
    case UP:
      Increment_Value();
      break;
    case DOWN:
      Decrement_Value();
      break;
    case LEFT:
      if (ethernet_reset == 1)
      {
        ethernet_reset = 0;
        Reset_NetworkDevice();
      }  
      Increment_Func_Index();
      break;
    case RIGHT:
      if (ethernet_reset == 1)
      {
        ethernet_reset = 0;
        Reset_NetworkDevice();
      }  
      Increment_Func_Index();
      break;
  }
}

static void Enter_Reset_Ethernet(void)
{
  if (ethernet_reset == 1)
  {
    ethernet_reset = 0;
    Reset_NetworkDevice();
  }  
  Increment_Func_Index_Enter_Value();
}

#endif // ETHERNET