// C__DISP_OPT_ALG_2.C

#include "ch_define.h"

#include "ch_alg.h"
#include "ch_alarm.h"
#include "ch_asc0.h"
#include "ch_can_backbone_appl.h"
#include "ch_can_io.h"
#include "ch_const.h"
#include "ch_disp.h"
#include "ch_disp_func.h"
#include "ch_disp_option_1.h"
#include "ch_disp_value.h" 
#include "ch_disp_password.h"
#include "ch_eep_taal.h"
#include "ch_key.h"
#include "ch_lcd_10.h"
#include "ch_lcd_14.h"
#include "ch_lcd_20.h"
#include "ch_lcd_bitmap.h"
#include "ch_lcd_pixel.h"
#include "ch_string.h" 
#include "NetworkDevice.h"
#include "ch_disp_opt_alg_2.h"
#include "ch_disp_opt_alg_wis_2.h"
#include "ch_bacnet_server.h"

static unsigned int eerstecode = 0;
static unsigned int tweedecode = 0;
#ifdef PASSWORD
unsigned char password_disp[5] = {0,0,0,0,0};
unsigned char password_level_disp[5] = {0,0,0,0,0};
unsigned char disp_setpoint;
unsigned char disp_setpoint_system;
unsigned char disp_option;
#endif // PASSWORD

static void Arrow_Lcd_Angle_value(void);   // void (*enter)(void);
static void Arrow_Data_Wissen(void);       // void (*enter)(void);
static void ArrowTaal(void);
static void EnterTaal(void);               // void (*enter)(void);
void Arrow_Computer_Nr(void);
void Enter_Computer_Nr(void);
static void Arrow_Can_Backbone(void);
static void Enter_Can_Backbone(void);
static void Arrow_Can_Baudrate(void);
static void Enter_Can_Baudrate(void);
#ifdef CAN_BACKBONE_PC_WARNING
static void Arrow_Can_PC_Enable_Value(void);
static void Enter_Can_PC_Enable_Value(void);
#else // CAN_BACKBONE_PC_WARNING
static void Arrow_Can_RS232(void);
static void Enter_Can_RS232(void);
#endif // CAN_BACKBONE_PC_WARNING

static void Arrow_Com1_Enable_Value(void);
static void Arrow_Com1_Baudrate_Value(void);
static void Arrow_Com1_Modem_Value(void);
static void Arrow_Com1_Modem_Answer_Value(void);
static void Enter_Com1_Modem_Answer_Value(void);
static void Arrow_Com2_Enable_Value(void);
static void Arrow_Com2_Baudrate_Value(void);
static void Arrow_Com2_Modem_Value(void);
static void Arrow_Com2_Modem_Answer_Value(void);
static void Enter_Com2_Modem_Answer_Value(void);
static void Arrow_Option_Alg_Func(void);
#ifdef ETHERNET
unsigned char ethernet_port[4];
unsigned char ethernet[3][4][3]; // Host, mask, gate
unsigned char ethernet_number_changed;
static void Arrow_Ethernet_Enable_Value(void);
static void Enter_Ethernet_Enable_Value(void);
static void Arrow_Ethernet_Func(void);
static void Arrow_Ethernet_Port_Value(void);
static void Enter_Ethernet_Port_Value(void);
static void Arrow_Ethernet_Value(void);
static void Enter_Ethernet_Value(void);
void CheckOptionsHoogendoorn(void);
static void Arrow_Hoogendoorn_Enable_Value(void);
static void Enter_Hoogendoorn_Enable_Value(void);
static void Arrow_Bacnet_Enable_Value(void);
static void Enter_Bacnet_Enable_Value(void);
static void Arrow_Bacnet_Func(void);
static void Arrow_Bacnet_Device_Id_Value(void);
static void Enter_Bacnet_Device_Id_Value(void);
static void Arrow_Bacnet_Network_Nr_Value(void);
static void Enter_Bacnet_Network_Nr_Value(void);
unsigned char bacnet_device_id[5];
unsigned char bacnet_network_nr[5];
#endif // ETHERNET
//static void RS232_Arrow(void);

static void Arrow_Code(void);
static void Enter_Code(void);
static void Arrow_Herhaal(void);
#ifdef PASSWORD
static void Enter_Herhaal_Beheerder(void);
static void Enter_Herhaal_Gebruiker_1(void);
static void Enter_Herhaal_Gebruiker_2(void);
static void Enter_Herhaal_Gebruiker_3(void);
static void Enter_Herhaal_Gebruiker_4(void);
static void Disp_Control_Gebruiker_1_Level(void);
static void Disp_Control_Gebruiker_2_Level(void);
static void Disp_Control_Gebruiker_3_Level(void);
static void Disp_Control_Gebruiker_4_Level(void);
#else // PASSWORD
static void Enter_Herhaal_Instal(void);
static void Enter_Herhaal_User(void);
#endif // PASSWORD
static void Enter_Herhaal_Pc(void);

static unsigned char flag_opties_gewist = 0;
static unsigned char flag_setpoints_gewist = 0;
static unsigned char option_adres_changed = 0;

void const * const tekst_leeg_opties_gewist_14[2] = { &tekst_leeg, &tekst_inst.Opties_gewist_14 };
void const * const tekst_leeg_setpoints_gewist_14[2] = { &tekst_leeg, &tekst_inst.Setpoints_gewist_14 };

void * tekst_taalkeuze[5];
unsigned char aantal_talen;

static s_disp_agri_header const disp_header = { Disp_Draw_Agri_Header, FN6, EMPTY, &tekst_inst.Opties_algemeen_10, HK_GEEN, 0, 3};
static void * const lcd_disp_header[] = { &disp_header, 0 };

// Taal keuze
static s_disp_tekst const disp_taalkeuze_str_0 = { Disp_Draw_Tekst_L, 37, 22, &tekst_inst.Taal_14 };
static s_disp_tekst_array const disp_str_taalkeuze = { Disp_Draw_Tekst_Array_L,  76, 75, &tekst_taalkeuze, UCHAR, &opt_alg.taalkeuze, 5 };
void const * const tekst_eigen_taal_array_14[] = 
{
  &tekst_leeg, &tekst_leeg, &tekst_leeg, &tekst_leeg, &tekst_USER_14
};
static s_disp_tekst_array const disp_eigen_taal_str = { Disp_Draw_Tekst_Array_L, 37, 41, &tekst_eigen_taal_array_14, UCHAR, &opt_alg.taalkeuze, 5 };
// Opties
static s_disp_tekst_add const disp_ja_14_inst_str = { Disp_Draw_Tekst_Add_L, &tekst_inst.JA_14 };
static s_disp_tekst const disp_optie_str0 =             { Disp_Draw_Tekst_L, 37, 22, &tekst_inst.Data_Wissen_14 };
static s_disp_bitmap const disp_optie_bmp0 =            { Disp_Draw_Bitmap,  137,61, &ico_arrow_right_14 };
static s_disp_tekst const disp_is_teken =               { Disp_Draw_Tekst_L,   160, 75, &tekst_is_14 };
static s_disp_tekst const disp_ok_str =                 { Disp_Draw_Tekst_L, 85+3, 65+19, &tekst_ok_10 };            
static s_disp_tekst_add const disp_wissen_str =         { Disp_Draw_Tekst_Add_L, &tekst_inst.Wissen_10 };
// LCD angle
static s_disp_tekst const disp_ldc_angle_str_0 = { Disp_Draw_Tekst_L, 37, 22, &tekst_inst.Helderheid_14 };
static s_disp_value  const disp_lcd_angle_value = { Disp_Draw_Value,  207, 75, (SIZE_14 | RECHTS), CHAR, 0, &opt_alg.lcd_angle };
static s_disp_balk   const disp_lcd_angle_balk =  { Disp_Draw_Balk, 50, 35, 170, 50, HORIZONTAAL_LINKS_RECHTS, CHAR, &opt_alg.lcd_angle, &char_m10, &char_10 };
// LCD Dimmen
static s_disp_tekst const disp_lcd_dimmen_str_0 = { Disp_Draw_Tekst_L, 37, 22, &tekst_inst.LCD_dimmen_14 };
static s_disp_bitmap_array const disp_dimmen_bmp = { Disp_Draw_Bitmap_Array,  190, 57, bmp_false_true, UCHAR, &opt_alg.lcd_dimmen , 2 };
// Computer Nummer
static s_disp_tekst const disp_computer_str =         { Disp_Draw_Tekst_L, 37, 22, &tekst_inst.Computer_14 };
static s_disp_tekst const disp_adres_str =         { Disp_Draw_Tekst_L, 37, 41, &tekst_inst.Nummer_14 };
static s_disp_value const disp_adres_val =         { Disp_Draw_Value,  207, 75, (SIZE_14 | RECHTS), INT, 0, &opt_alg.adres };
// Can backbone communicatie
static s_disp_tekst const disp_can_backbone_str =        { Disp_Draw_Tekst_L, 37, 22, &tekst_inst.CAN_BACKBONE_14 };
static s_disp_bitmap_array const disp_can_backbone_bmp = { Disp_Draw_Bitmap_Array,  190, 57, bmp_false_true, UCHAR, &opt_alg.can_backbone, 2 };
// Can backbone baudrate
static s_disp_value const disp_can_baudrate_val = { Disp_Draw_Value, 192, 75, (SIZE_14 | RECHTS), INT, 0, &opt_alg.CanBaudrate };
#ifdef CAN_BACKBONE_PC_WARNING
s_disp_cursor const disp_pc_1_cursor_checkbox =         { 207, 41,  18, 2, 20 };
s_disp_cursor const disp_pc_2_cursor_checkbox =         { 207, 61,  18, 2, 20 };
s_disp_cursor const disp_pc_3_cursor_checkbox =         { 207, 81,  18, 2, 20 };
static s_disp_tekst const disp_pc_1_str =               { Disp_Draw_Tekst_L, 37, 40, &tekst_inst.PC_1_14 };
static s_disp_tekst const disp_pc_2_str =               { Disp_Draw_Tekst_L, 37, 60, &tekst_inst.PC_2_14 };
static s_disp_tekst const disp_pc_3_str =               { Disp_Draw_Tekst_L, 37, 80, &tekst_inst.PC_3_14 };
static s_disp_tekst_add const disp_warning_str =        { Disp_Draw_Tekst_Add_L, &tekst_inst.Waarschuwing_7 };
static s_disp_bitmap_array const disp_pc_1_bmp =        { Disp_Draw_Bitmap_Array,  190, 22, bmp_false_true, UCHAR, &opt_alg.can_backbone_pc_warning[0], 2 };
static s_disp_bitmap_array const disp_pc_2_bmp =        { Disp_Draw_Bitmap_Array,  190, 42, bmp_false_true, UCHAR, &opt_alg.can_backbone_pc_warning[1], 2 };
static s_disp_bitmap_array const disp_pc_3_bmp =        { Disp_Draw_Bitmap_Array,  190, 62, bmp_false_true, UCHAR, &opt_alg.can_backbone_pc_warning[2], 2 };
#else // CAN_BACKBONE_PC_WARNING
// CAN_RS232 (blackbox) communicatie rainbow
static s_disp_tekst const disp_can_rs232_str =              { Disp_Draw_Tekst_L, 37, 22, &tekst_inst.CAN_RS232_14 };
static s_disp_tekst const disp_can_rs232_waarschuwing_str = { Disp_Draw_Tekst_L, 37, 41, &tekst_inst.Waarschuwing_14 };
static s_disp_bitmap_array const disp_can_rs232_bmp = { Disp_Draw_Bitmap_Array,  190, 57, bmp_false_true, UCHAR, &opt_alg.can_rs232, 2 };
#endif // CAN_BACKBONE_PC_WARNING

static s_disp_bitmap_array const disp_com1_bmp =        { Disp_Draw_Bitmap_Array, 9, 0, ico_pc_modem_big_array, UCHAR, &opt_alg.com1_modem, 2 };
static s_disp_bitmap_array const disp_com1_enable_bmp = { Disp_Draw_Bitmap_Array,  190, 57, bmp_false_true, UCHAR, &opt_alg.com1_enabled, 2 };
static s_disp_value  const disp_com1_baudrate_val =     { Disp_Draw_Value,  192, 75, (SIZE_14 | RECHTS), INT, 1, &opt_alg.com1_bd };
static s_disp_bitmap_array const disp_com1_modem_bmp =  { Disp_Draw_Bitmap_Array,  190, 57, bmp_false_true, UCHAR, &opt_alg.com1_modem, 2 };
static s_disp_value  const disp_com1_modem_answer_val = { Disp_Draw_Value,  207, 75, (SIZE_14 | RECHTS), UCHAR, 0, &opt_alg.com1_modem_answer };
static s_disp_tekst const disp_modem_str =              { Disp_Draw_Tekst_L,  37, 42, &tekst_inst.Modem_14 };
static s_disp_tekst const disp_modem_answer_str =       { Disp_Draw_Tekst_L,  37, 62, &tekst_inst.Answer_14 };
static s_disp_tekst const disp_baudrate_str =           { Disp_Draw_Tekst_L, 195, 75, &tekst_inst.kBd_14 };
// RS232 baudrate
static s_disp_tekst const disp_com1_str =               { Disp_Draw_Tekst_L,  37, 22, &tekst_inst.COM1_USB_14 };
static s_disp_bitmap_array const disp_com2_bmp =        { Disp_Draw_Bitmap_Array, 9, 0, ico_pc_modem_big_array, UCHAR, &opt_alg.com2_modem, 2 };
static s_disp_bitmap_array const disp_com2_enable_bmp = { Disp_Draw_Bitmap_Array,  190, 57, bmp_false_true, UCHAR, &opt_alg.com2_enabled, 2 };
static s_disp_tekst const disp_com2_str =               { Disp_Draw_Tekst_L,  37, 22, &tekst_inst.COM2_14 };
static s_disp_value  const disp_com2_baudrate_val =     { Disp_Draw_Value,  192, 75, (SIZE_14 | RECHTS), INT, 1, &opt_alg.com2_bd };
static s_disp_bitmap_array const disp_com2_modem_bmp =  { Disp_Draw_Bitmap_Array,  190, 57, bmp_false_true, UCHAR, &opt_alg.com2_modem, 2 };
static s_disp_value  const disp_com2_modem_answer_val = { Disp_Draw_Value,  207, 75, (SIZE_14 | RECHTS), UCHAR, 0, &opt_alg.com2_modem_answer };
#ifdef ETHERNET
// Ethernet
static s_disp_tekst const disp_ethernet_str =        { Disp_Draw_Tekst_L,  37, 20, &tekst_inst.ETHERNET_14 };
static s_disp_tekst const disp_port_str =            { Disp_Draw_Tekst_R,  98, 20, &tekst_inst.Port_10 };
//static s_disp_value const disp_port_val =            { Disp_Draw_Value,   135, 20, (SIZE_10 | RECHTS), INT, 0, &opt_alg.ethernet_port };
static s_disp_value const disp_port_3_val =          { Disp_Draw_Value,   107, 20, (SIZE_10 | RECHTS), UCHAR, 0, &ethernet_port[3] };
static s_disp_value const disp_port_2_val =          { Disp_Draw_Value,   115, 20, (SIZE_10 | RECHTS), UCHAR, 0, &ethernet_port[2] };
static s_disp_value const disp_port_1_val =          { Disp_Draw_Value,   123, 20, (SIZE_10 | RECHTS), UCHAR, 0, &ethernet_port[1] };
static s_disp_value const disp_port_0_val =          { Disp_Draw_Value,   131, 20, (SIZE_10 | RECHTS), UCHAR, 0, &ethernet_port[0] };
static s_tekst_15 tekst_5843 = { 9, 50, SIZE_10, "(def.5843)" };
static s_disp_tekst const disp_5843_str =            { Disp_Draw_Tekst_R, 210, 20, &tekst_5843 };
static s_disp_tekst const disp_host_str =            { Disp_Draw_Tekst_R,  98, 40, &tekst_inst.IP_10 };
static s_disp_tekst const disp_mask_str =            { Disp_Draw_Tekst_R,  98, 60, &tekst_inst.Mask_10 };
static s_disp_tekst const disp_gate_str =            { Disp_Draw_Tekst_R,  98, 80, &tekst_inst.Gate_10 };
static s_disp_bitmap_array const disp_ethernet_enable_bmp = { Disp_Draw_Bitmap_Array,  190, 57, bmp_false_true, UCHAR, &opt_alg.ethernet_enabled, 2 };
static s_disp_tekst const disp_host_punt_0_str =     { Disp_Draw_Tekst_L, 126, 40, &tekst_punt_10 };
static s_disp_tekst const disp_host_punt_1_str =     { Disp_Draw_Tekst_L, 154, 40, &tekst_punt_10 };
static s_disp_tekst const disp_host_punt_2_str =     { Disp_Draw_Tekst_L, 182, 40, &tekst_punt_10 };
static s_disp_tekst const disp_mask_punt_0_str =     { Disp_Draw_Tekst_L, 126, 60, &tekst_punt_10 };
static s_disp_tekst const disp_mask_punt_1_str =     { Disp_Draw_Tekst_L, 154, 60, &tekst_punt_10 };
static s_disp_tekst const disp_mask_punt_2_str =     { Disp_Draw_Tekst_L, 182, 60, &tekst_punt_10 };
static s_disp_tekst const disp_gate_punt_0_str =     { Disp_Draw_Tekst_L, 126, 80, &tekst_punt_10 };
static s_disp_tekst const disp_gate_punt_1_str =     { Disp_Draw_Tekst_L, 154, 80, &tekst_punt_10 };
static s_disp_tekst const disp_gate_punt_2_str =     { Disp_Draw_Tekst_L, 182, 80, &tekst_punt_10 };
static s_disp_value  const disp_host_0_2_val = { Disp_Draw_Value, 107,  40, (SIZE_10 | RECHTS), UCHAR, 0, &ethernet[0][0][2] };
static s_disp_value  const disp_host_0_1_val = { Disp_Draw_Value, 115,  40, (SIZE_10 | RECHTS), UCHAR, 0, &ethernet[0][0][1] };
static s_disp_value  const disp_host_0_0_val = { Disp_Draw_Value, 123,  40, (SIZE_10 | RECHTS), UCHAR, 0, &ethernet[0][0][0] };
static s_disp_value  const disp_host_1_2_val = { Disp_Draw_Value, 135,  40, (SIZE_10 | RECHTS), UCHAR, 0, &ethernet[0][1][2] };
static s_disp_value  const disp_host_1_1_val = { Disp_Draw_Value, 143,  40, (SIZE_10 | RECHTS), UCHAR, 0, &ethernet[0][1][1] };
static s_disp_value  const disp_host_1_0_val = { Disp_Draw_Value, 151,  40, (SIZE_10 | RECHTS), UCHAR, 0, &ethernet[0][1][0] };
static s_disp_value  const disp_host_2_2_val = { Disp_Draw_Value, 163,  40, (SIZE_10 | RECHTS), UCHAR, 0, &ethernet[0][2][2] };
static s_disp_value  const disp_host_2_1_val = { Disp_Draw_Value, 171,  40, (SIZE_10 | RECHTS), UCHAR, 0, &ethernet[0][2][1] };
static s_disp_value  const disp_host_2_0_val = { Disp_Draw_Value, 179,  40, (SIZE_10 | RECHTS), UCHAR, 0, &ethernet[0][2][0] };
static s_disp_value  const disp_host_3_2_val = { Disp_Draw_Value, 191,  40, (SIZE_10 | RECHTS), UCHAR, 0, &ethernet[0][3][2] };
static s_disp_value  const disp_host_3_1_val = { Disp_Draw_Value, 199,  40, (SIZE_10 | RECHTS), UCHAR, 0, &ethernet[0][3][1] };
static s_disp_value  const disp_host_3_0_val = { Disp_Draw_Value, 207,  40, (SIZE_10 | RECHTS), UCHAR, 0, &ethernet[0][3][0] };
static s_disp_value  const disp_mask_0_2_val = { Disp_Draw_Value, 107,  60, (SIZE_10 | RECHTS), UCHAR, 0, &ethernet[1][0][2] };
static s_disp_value  const disp_mask_0_1_val = { Disp_Draw_Value, 115,  60, (SIZE_10 | RECHTS), UCHAR, 0, &ethernet[1][0][1] };
static s_disp_value  const disp_mask_0_0_val = { Disp_Draw_Value, 123,  60, (SIZE_10 | RECHTS), UCHAR, 0, &ethernet[1][0][0] };
static s_disp_value  const disp_mask_1_2_val = { Disp_Draw_Value, 135,  60, (SIZE_10 | RECHTS), UCHAR, 0, &ethernet[1][1][2] };
static s_disp_value  const disp_mask_1_1_val = { Disp_Draw_Value, 143,  60, (SIZE_10 | RECHTS), UCHAR, 0, &ethernet[1][1][1] };
static s_disp_value  const disp_mask_1_0_val = { Disp_Draw_Value, 151,  60, (SIZE_10 | RECHTS), UCHAR, 0, &ethernet[1][1][0] };
static s_disp_value  const disp_mask_2_2_val = { Disp_Draw_Value, 163,  60, (SIZE_10 | RECHTS), UCHAR, 0, &ethernet[1][2][2] };
static s_disp_value  const disp_mask_2_1_val = { Disp_Draw_Value, 171,  60, (SIZE_10 | RECHTS), UCHAR, 0, &ethernet[1][2][1] };
static s_disp_value  const disp_mask_2_0_val = { Disp_Draw_Value, 179,  60, (SIZE_10 | RECHTS), UCHAR, 0, &ethernet[1][2][0] };
static s_disp_value  const disp_mask_3_2_val = { Disp_Draw_Value, 191,  60, (SIZE_10 | RECHTS), UCHAR, 0, &ethernet[1][3][2] };
static s_disp_value  const disp_mask_3_1_val = { Disp_Draw_Value, 199,  60, (SIZE_10 | RECHTS), UCHAR, 0, &ethernet[1][3][1] };
static s_disp_value  const disp_mask_3_0_val = { Disp_Draw_Value, 207,  60, (SIZE_10 | RECHTS), UCHAR, 0, &ethernet[1][3][0] };
static s_disp_value  const disp_gate_0_2_val = { Disp_Draw_Value, 107,  80, (SIZE_10 | RECHTS), UCHAR, 0, &ethernet[2][0][2] };
static s_disp_value  const disp_gate_0_1_val = { Disp_Draw_Value, 115,  80, (SIZE_10 | RECHTS), UCHAR, 0, &ethernet[2][0][1] };
static s_disp_value  const disp_gate_0_0_val = { Disp_Draw_Value, 123,  80, (SIZE_10 | RECHTS), UCHAR, 0, &ethernet[2][0][0] };
static s_disp_value  const disp_gate_1_2_val = { Disp_Draw_Value, 135,  80, (SIZE_10 | RECHTS), UCHAR, 0, &ethernet[2][1][2] };
static s_disp_value  const disp_gate_1_1_val = { Disp_Draw_Value, 143,  80, (SIZE_10 | RECHTS), UCHAR, 0, &ethernet[2][1][1] };
static s_disp_value  const disp_gate_1_0_val = { Disp_Draw_Value, 151,  80, (SIZE_10 | RECHTS), UCHAR, 0, &ethernet[2][1][0] };
static s_disp_value  const disp_gate_2_2_val = { Disp_Draw_Value, 163,  80, (SIZE_10 | RECHTS), UCHAR, 0, &ethernet[2][2][2] };
static s_disp_value  const disp_gate_2_1_val = { Disp_Draw_Value, 171,  80, (SIZE_10 | RECHTS), UCHAR, 0, &ethernet[2][2][1] };
static s_disp_value  const disp_gate_2_0_val = { Disp_Draw_Value, 179,  80, (SIZE_10 | RECHTS), UCHAR, 0, &ethernet[2][2][0] };
static s_disp_value  const disp_gate_3_2_val = { Disp_Draw_Value, 191,  80, (SIZE_10 | RECHTS), UCHAR, 0, &ethernet[2][3][2] };
static s_disp_value  const disp_gate_3_1_val = { Disp_Draw_Value, 199,  80, (SIZE_10 | RECHTS), UCHAR, 0, &ethernet[2][3][1] };
static s_disp_value  const disp_gate_3_0_val = { Disp_Draw_Value, 207,  80, (SIZE_10 | RECHTS), UCHAR, 0, &ethernet[2][3][0] };
//s_disp_cursor const disp_cursor_port =     { 135, 22,   6, 2, 13 };
s_disp_cursor const disp_cursor_port_3 =   { 107, 22,   6, 2, 13 };
s_disp_cursor const disp_cursor_port_2 =   { 115, 22,   6, 2, 13 };
s_disp_cursor const disp_cursor_port_1 =   { 123, 22,   6, 2, 13 };
s_disp_cursor const disp_cursor_port_0 =   { 131, 22,   6, 2, 13 };
s_disp_cursor const disp_cursor_host_0_2 = { 107, 42,   6, 2, 13 };
s_disp_cursor const disp_cursor_host_0_1 = { 115, 42,   6, 2, 13 };
s_disp_cursor const disp_cursor_host_0_0 = { 123, 42,   6, 2, 13 };
s_disp_cursor const disp_cursor_host_1_2 = { 135, 42,   6, 2, 13 };
s_disp_cursor const disp_cursor_host_1_1 = { 143, 42,   6, 2, 13 };
s_disp_cursor const disp_cursor_host_1_0 = { 151, 42,   6, 2, 13 };
s_disp_cursor const disp_cursor_host_2_2 = { 163, 42,   6, 2, 13 };
s_disp_cursor const disp_cursor_host_2_1 = { 171, 42,   6, 2, 13 };
s_disp_cursor const disp_cursor_host_2_0 = { 179, 42,   6, 2, 13 };
s_disp_cursor const disp_cursor_host_3_2 = { 191, 42,   6, 2, 13 };
s_disp_cursor const disp_cursor_host_3_1 = { 199, 42,   6, 2, 13 };
s_disp_cursor const disp_cursor_host_3_0 = { 207, 42,   6, 2, 13 };
s_disp_cursor const disp_cursor_mask_0_2 = { 107, 62,   6, 2, 13 };
s_disp_cursor const disp_cursor_mask_0_1 = { 115, 62,   6, 2, 13 };
s_disp_cursor const disp_cursor_mask_0_0 = { 123, 62,   6, 2, 13 };
s_disp_cursor const disp_cursor_mask_1_2 = { 135, 62,   6, 2, 13 };
s_disp_cursor const disp_cursor_mask_1_1 = { 143, 62,   6, 2, 13 };
s_disp_cursor const disp_cursor_mask_1_0 = { 151, 62,   6, 2, 13 };
s_disp_cursor const disp_cursor_mask_2_2 = { 163, 62,   6, 2, 13 };
s_disp_cursor const disp_cursor_mask_2_1 = { 171, 62,   6, 2, 13 };
s_disp_cursor const disp_cursor_mask_2_0 = { 179, 62,   6, 2, 13 };
s_disp_cursor const disp_cursor_mask_3_2 = { 191, 62,   6, 2, 13 };
s_disp_cursor const disp_cursor_mask_3_1 = { 199, 62,   6, 2, 13 };
s_disp_cursor const disp_cursor_mask_3_0 = { 207, 62,   6, 2, 13 };
s_disp_cursor const disp_cursor_gate_0_2 = { 107, 82,   6, 2, 13 };
s_disp_cursor const disp_cursor_gate_0_1 = { 115, 82,   6, 2, 13 };
s_disp_cursor const disp_cursor_gate_0_0 = { 123, 82,   6, 2, 13 };
s_disp_cursor const disp_cursor_gate_1_2 = { 135, 82,   6, 2, 13 };
s_disp_cursor const disp_cursor_gate_1_1 = { 143, 82,   6, 2, 13 };
s_disp_cursor const disp_cursor_gate_1_0 = { 151, 82,   6, 2, 13 };
s_disp_cursor const disp_cursor_gate_2_2 = { 163, 82,   6, 2, 13 };
s_disp_cursor const disp_cursor_gate_2_1 = { 171, 82,   6, 2, 13 };
s_disp_cursor const disp_cursor_gate_2_0 = { 179, 82,   6, 2, 13 };
s_disp_cursor const disp_cursor_gate_3_2 = { 191, 82,   6, 2, 13 };
s_disp_cursor const disp_cursor_gate_3_1 = { 199, 82,   6, 2, 13 };
s_disp_cursor const disp_cursor_gate_3_0 = { 207, 82,   6, 2, 13 };
// BACnet
static s_disp_tekst const disp_bacnet_str = { Disp_Draw_Tekst_L,  37, 20, &tekst_BACnet_14 };
static s_disp_bitmap_array const disp_bacnet_enable_bmp = { Disp_Draw_Bitmap_Array,  190, 57, bmp_false_true, UCHAR, &opt_alg.BACnet_enabled, 2 };

static s_disp_tekst const disp_bacnet_device_id_str =            { Disp_Draw_Tekst_R,  138, 40, &tekst_inst.BACnet_Device_ID_10 };
static s_disp_tekst const disp_bacnet_network_nr_str =           { Disp_Draw_Tekst_R,  138, 60, &tekst_inst.BACnet_Network_Nr_10 };
static s_disp_value const disp_bacnet_device_id_4_val =          { Disp_Draw_Value,   167, 40, (SIZE_10 | RECHTS), UCHAR, 0, &bacnet_device_id[4] };
static s_disp_value const disp_bacnet_device_id_3_val =          { Disp_Draw_Value,   175, 40, (SIZE_10 | RECHTS), UCHAR, 0, &bacnet_device_id[3] };
static s_disp_value const disp_bacnet_device_id_2_val =          { Disp_Draw_Value,   183, 40, (SIZE_10 | RECHTS), UCHAR, 0, &bacnet_device_id[2] };
static s_disp_value const disp_bacnet_device_id_1_val =          { Disp_Draw_Value,   191, 40, (SIZE_10 | RECHTS), UCHAR, 0, &bacnet_device_id[1] };
static s_disp_value const disp_bacnet_device_id_0_val =          { Disp_Draw_Value,   199, 40, (SIZE_10 | RECHTS), UCHAR, 0, &bacnet_device_id[0] };
static s_disp_value const disp_bacnet_network_nr_4_val =         { Disp_Draw_Value,   167, 60, (SIZE_10 | RECHTS), UCHAR, 0, &bacnet_network_nr[4] };
static s_disp_value const disp_bacnet_network_nr_3_val =         { Disp_Draw_Value,   175, 60, (SIZE_10 | RECHTS), UCHAR, 0, &bacnet_network_nr[3] };
static s_disp_value const disp_bacnet_network_nr_2_val =         { Disp_Draw_Value,   183, 60, (SIZE_10 | RECHTS), UCHAR, 0, &bacnet_network_nr[2] };
static s_disp_value const disp_bacnet_network_nr_1_val =         { Disp_Draw_Value,   191, 60, (SIZE_10 | RECHTS), UCHAR, 0, &bacnet_network_nr[1] };
static s_disp_value const disp_bacnet_network_nr_0_val =         { Disp_Draw_Value,   199, 60, (SIZE_10 | RECHTS), UCHAR, 0, &bacnet_network_nr[0] };
s_disp_cursor const disp_cursor_bacnet_device_id_4 =   { 167, 42,   6, 2, 13 };
s_disp_cursor const disp_cursor_bacnet_device_id_3 =   { 175, 42,   6, 2, 13 };
s_disp_cursor const disp_cursor_bacnet_device_id_2 =   { 183, 42,   6, 2, 13 };
s_disp_cursor const disp_cursor_bacnet_device_id_1 =   { 191, 42,   6, 2, 13 };
s_disp_cursor const disp_cursor_bacnet_device_id_0 =   { 199, 42,   6, 2, 13 };
s_disp_cursor const disp_cursor_bacnet_network_nr_4 =  { 167, 62,   6, 2, 13 };
s_disp_cursor const disp_cursor_bacnet_network_nr_3 =  { 175, 62,   6, 2, 13 };
s_disp_cursor const disp_cursor_bacnet_network_nr_2 =  { 183, 62,   6, 2, 13 };
s_disp_cursor const disp_cursor_bacnet_network_nr_1 =  { 191, 62,   6, 2, 13 };
s_disp_cursor const disp_cursor_bacnet_network_nr_0 =  { 199, 62,   6, 2, 13 };
#endif // ETHERNET

// RS232 baudrate
//static s_disp_tekst const disp_RS232_str =      { Disp_Draw_Tekst_L,  37, 22, &tekst_inst.RS232_14 };
//static s_disp_value const disp_baudrate_value = { Disp_Draw_Value, 192, 75, (SIZE_14 | RECHTS), INT, 1, &opt_alg.rs232 };
//static s_disp_tekst const disp_baudrate_str =   { Disp_Draw_Tekst_L, 195, 75, &tekst_inst.kBd_14 };

// Installateurs code
static s_bitmap const * const bmp_geen_code[] = { &ico_empty, &ico_14_code };
#ifdef PASSWORD
static s_disp_cursor const disp_cursor_level =   { 128, 53,   8, 2, 18 };
static s_disp_tekst const disp_code_str_1 = { Disp_Draw_Tekst_R, 86, 40, &tekst_inst.Wachtwoord_14 };
static s_disp_value const disp_code_1 =     { Disp_Draw_Code,  91, 26, (SIZE_14 | LINKS), UINT, 0, &eerstecode };
static s_disp_tekst const disp_code_str_2 = { Disp_Draw_Tekst_R, 86, 65, &tekst_inst.Herhaal_14 };
static s_disp_value const disp_code_2 =     { Disp_Draw_Code,  91, 51, (SIZE_14 | LINKS), UINT, 0, &tweedecode };
static s_disp_tekst const disp_code_str_3 = { Disp_Draw_Tekst_L,  85+3, 40+19, &tekst_inst.Wachtwoord_ongelijk_7 };
static s_disp_tekst const disp_code_str_4 = { Disp_Draw_Tekst_R, 170+3, 65+19, &tekst_ok_14 };
static s_disp_tekst const disp_beheerder_code_str_0 =        { Disp_Draw_Tekst_L,  37, 22, &tekst_inst.Beheerder_14 };
static s_disp_bitmap_option_on const disp_beheerder_code =   { Disp_Draw_Bitmap_Option_On, 91, 26, &ico_14_code, UINT, &opt_alg.password_nummer[0] };
static s_disp_tekst const disp_gebruiker_1_code_str_0 =      { Disp_Draw_Tekst_L, 37, 22, &tekst_inst.Gebruiker_1_14 };
static s_disp_bitmap_option_on const disp_gebruiker_1_code = { Disp_Draw_Bitmap_Option_On, 91, 26, &ico_14_code, UINT, &opt_alg.password_nummer[1] };
static s_disp_tekst const disp_gebruiker_2_code_str_0 =          { Disp_Draw_Tekst_L, 37, 22, &tekst_inst.Gebruiker_2_14 };
static s_disp_bitmap_option_on const disp_gebruiker_2_code =     { Disp_Draw_Bitmap_Option_On, 91, 26, &ico_14_code, UINT, &opt_alg.password_nummer[2] };
static s_disp_tekst const disp_gebruiker_3_code_str_0 =          { Disp_Draw_Tekst_L, 37, 22, &tekst_inst.Gebruiker_3_14 };
static s_disp_bitmap_option_on const disp_gebruiker_3_code =     { Disp_Draw_Bitmap_Option_On, 91, 26, &ico_14_code, UINT, &opt_alg.password_nummer[3] };
static s_disp_tekst const disp_gebruiker_4_code_str_0 =          { Disp_Draw_Tekst_L, 37, 22, &tekst_inst.Gebruiker_4_14 };
static s_disp_bitmap_option_on const disp_gebruiker_4_code =     { Disp_Draw_Bitmap_Option_On, 91, 26, &ico_14_code, UINT, &opt_alg.password_nummer[4] };
static s_disp_tekst const disp_gebruiker_1_level_str = { Disp_Draw_Tekst_R, 115, 50, &tekst_inst.Level_14 };
static s_disp_value const disp_gebruiker_1_level =     { Disp_Draw_Value,   120, 50, (SIZE_14 | LINKS), UCHAR, 0, &opt_alg.password_level[1] };
s_disp_func const disp_gebruiker_1_level_control = { Disp_Control_Func, Disp_Control_Gebruiker_1_Level };
s_disp_func const disp_gebruiker_2_level_control = { Disp_Control_Func, Disp_Control_Gebruiker_2_Level };
s_disp_func const disp_gebruiker_3_level_control = { Disp_Control_Func, Disp_Control_Gebruiker_3_Level };
s_disp_func const disp_gebruiker_4_level_control = { Disp_Control_Func, Disp_Control_Gebruiker_4_Level };
static s_disp_tekst_option_on const disp_gebruiker_setp_str = { Disp_Draw_Tekst_L_Option_On, 135, 50, &tekst_inst.Setpoints_7, UCHAR, &disp_setpoint };
static s_disp_tekst_option_on const disp_gebruiker_syst_str = { Disp_Draw_Tekst_L_Option_On, 135, 60, &tekst_inst.Setpoints_Syst_7, UCHAR, &disp_setpoint_system };
static s_disp_tekst_option_on const disp_gebruiker_opt_str =  { Disp_Draw_Tekst_L_Option_On, 135, 70, &tekst_inst.Opties_7, UCHAR, &disp_option };
static s_disp_tekst const disp_gebruiker_2_level_str = { Disp_Draw_Tekst_R, 115, 50, &tekst_inst.Level_14 };
static s_disp_value const disp_gebruiker_2_level =     { Disp_Draw_Value,   120, 50, (SIZE_14 | LINKS), UCHAR, 0, &opt_alg.password_level[2] };
static s_disp_tekst const disp_gebruiker_3_level_str = { Disp_Draw_Tekst_R, 115, 50, &tekst_inst.Level_14 };
static s_disp_value const disp_gebruiker_3_level =     { Disp_Draw_Value,   120, 50, (SIZE_14 | LINKS), UCHAR, 0, &opt_alg.password_level[3] };
static s_disp_tekst const disp_gebruiker_4_level_str = { Disp_Draw_Tekst_R, 115, 50, &tekst_inst.Level_14 };
static s_disp_value const disp_gebruiker_4_level =     { Disp_Draw_Value,   120, 50, (SIZE_14 | LINKS), UCHAR, 0, &opt_alg.password_level[4] };
#else // PASSWORD
static s_disp_tekst const disp_instal_code_str_0 = { Disp_Draw_Tekst_L,  37, 22, &tekst_inst.Installateurs_14 };
//static s_disp_bitmap_array const disp_instal_code = { Disp_Draw_Bitmap_Array, 120, 36, bmp_geen_code, UINT, &opt_alg.password_installateur, 2 };
//static s_disp_tekst const disp_instal_code_str_1 = { Disp_Draw_Tekst_R, 115, 50, &tekst_inst.Wachtwoord_14 };
//static s_disp_tekst const disp_instal_code_str_2 = { Disp_Draw_Tekst_R, 115, 75, &tekst_inst.Herhaal_14 };
//static s_disp_value const disp_code =              { Disp_Draw_Code,  120, 36, (SIZE_14 | LINKS), UINT, 0, &eerstecode };
//static s_disp_value const disp_Herhaal =           { Disp_Draw_Code,  120, 61, (SIZE_14 | LINKS), UINT, 0, &tweedecode };
static s_disp_bitmap_option_on const disp_instal_code = { Disp_Draw_Bitmap_Option_On, 91, 26, &ico_14_code, UINT, &opt_alg.password_installateur };
static s_disp_tekst const disp_instal_code_str_1 = { Disp_Draw_Tekst_R, 86, 40, &tekst_inst.Wachtwoord_14 };
static s_disp_tekst const disp_instal_code_str_2 = { Disp_Draw_Tekst_R, 86, 65, &tekst_inst.Herhaal_14 };
static s_disp_value const disp_code =              { Disp_Draw_Code,  91, 26, (SIZE_14 | LINKS), UINT, 0, &eerstecode };
static s_disp_value const disp_Herhaal =           { Disp_Draw_Code,  91, 51, (SIZE_14 | LINKS), UINT, 0, &tweedecode };
static s_disp_tekst const disp_instal_code_str_3 = { Disp_Draw_Tekst_L,  85+3, 40+19, &tekst_inst.Wachtwoord_ongelijk_7 };
static s_disp_tekst const disp_instal_code_str_4 = { Disp_Draw_Tekst_R, 170+3, 65+19, &tekst_ok_14 };
// Gebruikers code
static s_disp_tekst const disp_user_code_str_0 = { Disp_Draw_Tekst_L, 37, 22, &tekst_inst.Gebruikers_14 };
//static s_disp_bitmap_array const disp_user_code = { Disp_Draw_Bitmap_Array, 120, 36, bmp_geen_code, UINT, &opt_alg.password_gebruiker, 2 };
static s_disp_bitmap_option_on const disp_user_code = { Disp_Draw_Bitmap_Option_On, 91, 26, &ico_14_code, UINT, &opt_alg.password_gebruiker };
#endif // PASWORD
// PC code
static s_disp_tekst const disp_pc_code_str_0 = { Disp_Draw_Tekst_L, 37, 22, &tekst_inst.PC_14 };
//static s_disp_bitmap_array const disp_pc_code = { Disp_Draw_Bitmap_Array, 120, 36, bmp_geen_code, UINT, &opt_alg.password_pc, 2 };
static s_disp_bitmap_option_on const disp_pc_code = { Disp_Draw_Bitmap_Option_On,  91, 26, &ico_14_code, UINT, &opt_alg.password_pc };

//*****************************************************************************
//*** Schermen afdrukken
//*****************************************************************************
static void * const lcd_taalkeuze_disp[] = { &disp_flag, &disp_taalkeuze_str_0, &disp_eigen_taal_str, &disp_str_taalkeuze,  0 };
static void * const lcd_data_wissen_disp[] = { &disp_afvalemmer, &disp_optie_str0, &disp_optie_bmp0, &disp_is_teken, &disp_space_14_L, &disp_ja_14_inst_str, 0 };
static void * const lcd_angle_disp[] =  { &disp_helderheid, &disp_ldc_angle_str_0, &disp_lcd_angle_balk, &disp_lcd_angle_value, 0 };
static void * const lcd_dimmen_disp[] = { &disp_lcd_dimmen, &disp_lcd_dimmen_str_0, &disp_dimmen_bmp, 0 };
static void * const lcd_computer_nr_disp[] = { &disp_comp_nr, &disp_computer_str, &disp_adres_str, &disp_adres_val, 0 };
static void * const lcd_can_backbone_disp[] = { &disp_comp_nr, &disp_can_backbone_str, &disp_can_backbone_bmp, 0 };
static void * const lcd_can_baudrate_disp[] = { &disp_comp_nr, &disp_can_backbone_str, &disp_can_baudrate_val, &disp_baudrate_str, 0 };
#ifdef CAN_BACKBONE_PC_WARNING
static void * const lcd_can_backbone_pc_waarschuwing_disp[] =
{
  &disp_comp_nr, &disp_can_backbone_str,
  &disp_pc_1_str, &disp_space_10_L, &disp_warning_str, &disp_pc_1_bmp,
  &disp_pc_2_str, &disp_space_10_L, &disp_warning_str, &disp_pc_2_bmp,
  &disp_pc_3_str, &disp_space_10_L, &disp_warning_str, &disp_pc_3_bmp, 0
};
#else // CAN_BACKBONE_PC_WARNING
static void * const lcd_can_rs232_disp[] = { &disp_comp_nr, &disp_can_rs232_str, &disp_can_rs232_waarschuwing_str, &disp_can_rs232_bmp, 0 };
#endif // CAN_BACKBONE_PC_WARNING

static void * const lcd_com1_enable_disp[] =       { &disp_com1_bmp, &disp_com1_str, &disp_com1_enable_bmp, 0 };
static void * const lcd_com1_baudrate_disp[] =     { &disp_com1_bmp, &disp_com1_str, &disp_com1_baudrate_val, &disp_baudrate_str, 0 };
static void * const lcd_com1_modem_disp[] =        { &disp_com1_bmp, &disp_com1_str, &disp_modem_str, &disp_com1_modem_bmp, 0 };
static void * const lcd_com1_modem_answer_disp[] = { &disp_com1_bmp, &disp_com1_str, &disp_modem_str, &disp_modem_answer_str, &disp_com1_modem_answer_val, 0 };
static void * const lcd_com2_enable_disp[] =       { &disp_com2_bmp, &disp_com2_str, &disp_com2_enable_bmp, 0 };
static void * const lcd_com2_baudrate_disp[] =     { &disp_com2_bmp, &disp_com2_str, &disp_com2_baudrate_val, &disp_baudrate_str, 0 };
static void * const lcd_com2_modem_disp[] =        { &disp_com2_bmp, &disp_com2_str, &disp_modem_str, &disp_com2_modem_bmp, 0 };
static void * const lcd_com2_modem_answer_disp[] = { &disp_com2_bmp, &disp_com2_str, &disp_modem_str, &disp_modem_answer_str, &disp_com2_modem_answer_val, 0 };
#ifdef ETHERNET
static void * const lcd_ethernet_enable_disp[] =   { &disp_globe, &disp_ethernet_str, &disp_ethernet_enable_bmp, 0 };
static void * const lcd_ethernet_disp[] =
{
  &disp_globe,   //&disp_ethernet_str,
  &disp_port_str, &disp_port_3_val,   &disp_port_2_val,   &disp_port_1_val,   &disp_port_0_val,      &disp_5843_str,
  &disp_host_str, &disp_host_0_2_val, &disp_host_0_1_val, &disp_host_0_0_val, &disp_host_punt_0_str,
                  &disp_host_1_2_val, &disp_host_1_1_val, &disp_host_1_0_val, &disp_host_punt_1_str,
                  &disp_host_2_2_val, &disp_host_2_1_val, &disp_host_2_0_val, &disp_host_punt_2_str,
                  &disp_host_3_2_val, &disp_host_3_1_val, &disp_host_3_0_val,
  &disp_mask_str, &disp_mask_0_2_val, &disp_mask_0_1_val, &disp_mask_0_0_val, &disp_mask_punt_0_str,
                  &disp_mask_1_2_val, &disp_mask_1_1_val, &disp_mask_1_0_val, &disp_mask_punt_1_str,
                  &disp_mask_2_2_val, &disp_mask_2_1_val, &disp_mask_2_0_val, &disp_mask_punt_2_str,
                  &disp_mask_3_2_val, &disp_mask_3_1_val, &disp_mask_3_0_val,
  &disp_gate_str, &disp_gate_0_2_val, &disp_gate_0_1_val, &disp_gate_0_0_val, &disp_gate_punt_0_str,
                  &disp_gate_1_2_val, &disp_gate_1_1_val, &disp_gate_1_0_val, &disp_gate_punt_1_str,
                  &disp_gate_2_2_val, &disp_gate_2_1_val, &disp_gate_2_0_val, &disp_gate_punt_2_str,
                  &disp_gate_3_2_val, &disp_gate_3_1_val, &disp_gate_3_0_val,
  0
};
static s_disp_tekst const disp_Hoogendoorn_str =        { Disp_Draw_Tekst_L,  37, 20, &tekst_Hoogendoorn_14 };
static s_disp_bitmap_array const disp_Hoogendoorn_enable_bmp = { Disp_Draw_Bitmap_Array,  190, 57, bmp_false_true, UCHAR, &opt_alg.hoogendoorn_enabled, 2 };
static void * const lcd_Hoogendoorn_enable_disp[] =   { &disp_globe, &disp_Hoogendoorn_str, &disp_Hoogendoorn_enable_bmp, 0 };
static s_key_value const key_Hoogendoorn_enable_value = { UCHAR, 3, &opt_alg.hoogendoorn_enabled, &uchar_0, &uchar_1 };

static void * const lcd_bacnet_enable_disp[] =   { &disp_globe, &disp_bacnet_str, &disp_bacnet_enable_bmp, 0 };
static void * const lcd_bacnet_disp[] =
{
  &disp_globe, &disp_bacnet_str,
  &disp_bacnet_device_id_str, &disp_bacnet_device_id_4_val,   &disp_bacnet_device_id_3_val,   &disp_bacnet_device_id_2_val,   &disp_bacnet_device_id_1_val, &disp_bacnet_device_id_0_val,
  &disp_bacnet_network_nr_str, &disp_bacnet_network_nr_4_val,   &disp_bacnet_network_nr_3_val,   &disp_bacnet_network_nr_2_val,   &disp_bacnet_network_nr_1_val, &disp_bacnet_network_nr_0_val,
  0
};
#endif // ETHERNET
//static void * const lcd_RS232_baudrate_disp[] = { &disp_comp_nr, &disp_RS232_str, &disp_baudrate_value, &disp_baudrate_str, 0 };

#ifdef PASSWORD
static void * const lcd_beheerder_code_disp[] =     { &disp_sleutel, &disp_beheerder_code_str_0, &disp_code_str_1, &disp_beheerder_code, &disp_password_help, 0 };
static void * const lcd_beheerder_code_1_disp[] =   { &disp_sleutel, &disp_beheerder_code_str_0, &disp_code_str_1, &disp_code_1, &disp_password_help, 0  };
static void * const lcd_beheerder_code_2_disp[] =   { &disp_sleutel, &disp_beheerder_code_str_0, &disp_code_str_1, &disp_code_str_2, &disp_code_1, &disp_code_2, &disp_password_help, 0 };
static void * const lcd_beheerder_code_3_disp[] =   { &disp_sleutel, &disp_beheerder_code_str_0, &disp_code_str_1, &disp_code_str_2, &disp_code_1, &disp_code_2, &disp_password_help,
                                                      &disp_messagebox_error, &disp_code_str_3, &disp_code_str_4, 0 };
static void * const lcd_gebruiker_1_code_disp[] =   { &disp_sleutel, &disp_gebruiker_1_code_str_0, &disp_code_str_1, &disp_gebruiker_1_code, &disp_password_help, 0 };
static void * const lcd_gebruiker_1_code_1_disp[] = { &disp_sleutel, &disp_gebruiker_1_code_str_0, &disp_code_str_1, &disp_code_1, &disp_password_help, 0 };
static void * const lcd_gebruiker_1_code_2_disp[] = { &disp_sleutel, &disp_gebruiker_1_code_str_0, &disp_code_str_1, &disp_code_str_2, &disp_code_1, &disp_code_2, &disp_password_help, 0 };
static void * const lcd_gebruiker_1_code_3_disp[] = { &disp_sleutel, &disp_gebruiker_1_code_str_0, &disp_code_str_1, &disp_code_str_2, &disp_code_1, &disp_code_2, &disp_password_help,
                                                      &disp_messagebox_error, &disp_code_str_3, &disp_code_str_4, 0 };
static void * const lcd_gebruiker_1_level_disp[] =  { &disp_gebruiker_1_level_control, &disp_sleutel, &disp_gebruiker_1_code_str_0, &disp_gebruiker_1_level_str, &disp_gebruiker_1_level, &disp_gebruiker_setp_str, &disp_gebruiker_syst_str, &disp_gebruiker_opt_str, 0 };
static void * const lcd_gebruiker_2_code_disp[] =   { &disp_sleutel, &disp_gebruiker_2_code_str_0, &disp_code_str_1, &disp_gebruiker_2_code, &disp_password_help, 0 };
static void * const lcd_gebruiker_2_code_1_disp[] = { &disp_sleutel, &disp_gebruiker_2_code_str_0, &disp_code_str_1, &disp_code_1, &disp_password_help, 0 };
static void * const lcd_gebruiker_2_code_2_disp[] = { &disp_sleutel, &disp_gebruiker_2_code_str_0, &disp_code_str_1, &disp_code_str_2, &disp_code_1, &disp_code_2, &disp_password_help, 0 };
static void * const lcd_gebruiker_2_code_3_disp[] = { &disp_sleutel, &disp_gebruiker_2_code_str_0, &disp_code_str_1, &disp_code_str_2, &disp_code_1, &disp_code_2, &disp_password_help, 
                                                      &disp_messagebox_error, &disp_code_str_3, &disp_code_str_4, 0 };
static void * const lcd_gebruiker_2_level_disp[] =  { &disp_gebruiker_2_level_control, &disp_sleutel, &disp_gebruiker_2_code_str_0, &disp_gebruiker_2_level_str, &disp_gebruiker_2_level, &disp_gebruiker_setp_str, &disp_gebruiker_syst_str, &disp_gebruiker_opt_str, 0 };
static void * const lcd_gebruiker_3_code_disp[] =   { &disp_sleutel, &disp_gebruiker_3_code_str_0, &disp_code_str_1, &disp_gebruiker_3_code, &disp_password_help, 0 };
static void * const lcd_gebruiker_3_code_1_disp[] = { &disp_sleutel, &disp_gebruiker_3_code_str_0, &disp_code_str_1, &disp_code_1, &disp_password_help, 0 };
static void * const lcd_gebruiker_3_code_2_disp[] = { &disp_sleutel, &disp_gebruiker_3_code_str_0, &disp_code_str_1, &disp_code_str_2, &disp_code_1, &disp_code_2, &disp_password_help, 0 };
static void * const lcd_gebruiker_3_code_3_disp[] = { &disp_sleutel, &disp_gebruiker_3_code_str_0, &disp_code_str_1, &disp_code_str_2, &disp_code_1, &disp_code_2, &disp_password_help,
                                                      &disp_messagebox_error, &disp_code_str_3, &disp_code_str_4, 0 };
static void * const lcd_gebruiker_3_level_disp[] =  { &disp_gebruiker_3_level_control, &disp_sleutel, &disp_gebruiker_3_code_str_0, &disp_gebruiker_3_level_str, &disp_gebruiker_3_level, &disp_gebruiker_setp_str, &disp_gebruiker_syst_str, &disp_gebruiker_opt_str, 0 };
static void * const lcd_gebruiker_4_code_disp[] =   { &disp_sleutel, &disp_gebruiker_4_code_str_0, &disp_code_str_1, &disp_gebruiker_4_code, &disp_password_help, 0 };
static void * const lcd_gebruiker_4_code_1_disp[] = { &disp_sleutel, &disp_gebruiker_4_code_str_0, &disp_code_str_1, &disp_code_1, &disp_password_help, 0 };
static void * const lcd_gebruiker_4_code_2_disp[] = { &disp_sleutel, &disp_gebruiker_4_code_str_0, &disp_code_str_1, &disp_code_str_2, &disp_code_1, &disp_code_2, &disp_password_help, 0 };
static void * const lcd_gebruiker_4_code_3_disp[] = { &disp_sleutel, &disp_gebruiker_4_code_str_0, &disp_code_str_1, &disp_code_str_2, &disp_code_1, &disp_code_2, &disp_password_help,
                                                      &disp_messagebox_error, &disp_code_str_3, &disp_code_str_4, 0 };
static void * const lcd_gebruiker_4_level_disp[] =  { &disp_gebruiker_4_level_control, &disp_sleutel, &disp_gebruiker_4_code_str_0, &disp_gebruiker_4_level_str, &disp_gebruiker_4_level, &disp_gebruiker_setp_str, &disp_gebruiker_syst_str, &disp_gebruiker_opt_str, 0 };
static void * const lcd_pc_code_disp[] =   { &disp_sleutel, &disp_pc_code_str_0, &disp_code_str_1, &disp_pc_code, &disp_password_help, 0 };
static void * const lcd_pc_code_1_disp[] = { &disp_sleutel, &disp_pc_code_str_0, &disp_code_str_1, &disp_code_1, &disp_password_help, 0 };
static void * const lcd_pc_code_2_disp[] = { &disp_sleutel, &disp_pc_code_str_0, &disp_code_str_1, &disp_code_str_2, &disp_code_1, &disp_code_2, &disp_password_help, 0 };
static void * const lcd_pc_code_3_disp[] = { &disp_sleutel, &disp_pc_code_str_0, &disp_code_str_1, &disp_code_str_2, &disp_code_1, &disp_code_2, &disp_password_help,
                                             &disp_messagebox_error, &disp_code_str_3, &disp_code_str_4, 0 };
#else // PASSWORD
static void * const lcd_instal_code_disp[] =   { &disp_sleutel, &disp_instal_code_str_0, &disp_instal_code_str_1, &disp_instal_code, &disp_password_help, 0 };
static void * const lcd_instal_code_1_disp[] = { &disp_sleutel, &disp_instal_code_str_0, &disp_instal_code_str_1, &disp_code, &disp_password_help, 0 };
static void * const lcd_instal_code_2_disp[] = { &disp_sleutel, &disp_instal_code_str_0, &disp_instal_code_str_1, &disp_instal_code_str_2, &disp_code, &disp_Herhaal, &disp_password_help, 0 };
static void * const lcd_instal_code_3_disp[] = { &disp_sleutel, &disp_instal_code_str_0, &disp_instal_code_str_1, &disp_instal_code_str_2, &disp_code, &disp_password_help, 
                                                 &disp_Herhaal, &disp_messagebox_error, &disp_instal_code_str_3, &disp_instal_code_str_4, 0 };

static void * const lcd_user_code_disp[] =   { &disp_sleutel, &disp_user_code_str_0, &disp_instal_code_str_1, &disp_user_code, &disp_password_help, 0 };
static void * const lcd_user_code_1_disp[] = { &disp_sleutel, &disp_user_code_str_0, &disp_instal_code_str_1, &disp_code, &disp_password_help, 0 };
static void * const lcd_user_code_2_disp[] = { &disp_sleutel, &disp_user_code_str_0, &disp_instal_code_str_1, &disp_instal_code_str_2, &disp_code, &disp_Herhaal, &disp_password_help, 0 };
static void * const lcd_user_code_3_disp[] = { &disp_sleutel, &disp_user_code_str_0, &disp_instal_code_str_1, &disp_instal_code_str_2, &disp_code, &disp_password_help, 
                                               &disp_Herhaal, &disp_messagebox_error, &disp_instal_code_str_3, &disp_instal_code_str_4, 0 };

static void * const lcd_pc_code_disp[] =   { &disp_sleutel, &disp_pc_code_str_0, &disp_instal_code_str_1, &disp_pc_code, &disp_password_help, 0 };
static void * const lcd_pc_code_1_disp[] = { &disp_sleutel, &disp_pc_code_str_0, &disp_instal_code_str_1, &disp_code, &disp_password_help, 0 };
static void * const lcd_pc_code_2_disp[] = { &disp_sleutel, &disp_pc_code_str_0, &disp_instal_code_str_1, &disp_instal_code_str_2, &disp_code, &disp_Herhaal, &disp_password_help, 0 };
static void * const lcd_pc_code_3_disp[] = { &disp_sleutel, &disp_pc_code_str_0, &disp_instal_code_str_1, &disp_instal_code_str_2, &disp_code, &disp_password_help, 
                                             &disp_Herhaal, &disp_messagebox_error, &disp_instal_code_str_3, &disp_instal_code_str_4, 0 };
#endif // PASSWORD

static s_key_value const key_lcd_angle_value       = { CHAR, 2, &opt_alg.lcd_angle  , &char_m10, &char_10 };
static s_key_value const key_adres_val =         { INT, 3, &opt_alg.adres, &int_1, &int_63 };
static s_key_value const key_can_backbone_val = { CHAR, 3, &opt_alg.can_backbone, &uchar_0, &uchar_1 };
static s_key_value const key_can_baudrate_val = { INT,  3, &opt_alg.CanBaudrate,  &int_20,  &int_500 };
#ifdef CAN_BACKBONE_PC_WARNING
static s_key_value const key_pc_1_enable_value = { UCHAR, 3, &opt_alg.can_backbone_pc_warning[0], &uchar_0, &uchar_1 };
static s_key_value const key_pc_2_enable_value = { UCHAR, 3, &opt_alg.can_backbone_pc_warning[1], &uchar_0, &uchar_1 };
static s_key_value const key_pc_3_enable_value = { UCHAR, 3, &opt_alg.can_backbone_pc_warning[2], &uchar_0, &uchar_1 };
#else // CAN_BACKBONE_PC_WARNING
static s_key_value const key_can_rs232_val = { CHAR, 3, &opt_alg.can_rs232, &uchar_0, &uchar_1 };
#endif // CAN_BACKBONE_PC_WARNING

static s_key_value const key_com1_enable_value =       { UCHAR, 3, &opt_alg.com1_enabled, &uchar_0, &uchar_1 };
static s_key_value const key_com1_modem_value =        { UCHAR, 3, &opt_alg.com1_modem, &uchar_0, &uchar_1 };
static s_key_value const key_com1_modem_answer_value = { UCHAR, 3, &opt_alg.com1_modem_answer, &uchar_1, &uchar_10 };
static s_key_value const key_com1_baudrate_value =     { INT,   3, &opt_alg.com1_bd, &int_96, &int_1152 };
static s_key_value const key_com2_enable_value =       { UCHAR, 3, &opt_alg.com2_enabled, &uchar_0, &uchar_1 };
static s_key_value const key_com2_baudrate_value =     { INT,   3, &opt_alg.com2_bd, &int_96, &int_1152 };
static s_key_value const key_com2_modem_value =        { UCHAR, 3, &opt_alg.com2_modem, &uchar_0, &uchar_1 };
static s_key_value const key_com2_modem_answer_value = { UCHAR, 3, &opt_alg.com2_modem_answer, &uchar_1, &uchar_10 };
#ifdef ETHERNET
static s_key_value const key_ethernet_enable_value =   { UCHAR, 3, &opt_alg.ethernet_enabled, &uchar_0, &uchar_1 };
static s_key_value const key_port_3_value =            { UCHAR, 1, &ethernet_port[3],  &uchar_0, &uchar_9 };
static s_key_value const key_port_2_value =            { UCHAR, 1, &ethernet_port[2],  &uchar_0, &uchar_9 };
static s_key_value const key_port_1_value =            { UCHAR, 1, &ethernet_port[1],  &uchar_0, &uchar_9 };
static s_key_value const key_port_0_value =            { UCHAR, 1, &ethernet_port[0],  &uchar_0, &uchar_9 };
static s_key_value const key_host_0_2_value =          { UCHAR, 1, &ethernet[0][0][2], &uchar_0, &uchar_9 };
static s_key_value const key_host_0_1_value =          { UCHAR, 1, &ethernet[0][0][1], &uchar_0, &uchar_9 };
static s_key_value const key_host_0_0_value =          { UCHAR, 1, &ethernet[0][0][0], &uchar_0, &uchar_9 };
static s_key_value const key_host_1_2_value =          { UCHAR, 1, &ethernet[0][1][2], &uchar_0, &uchar_9 };
static s_key_value const key_host_1_1_value =          { UCHAR, 1, &ethernet[0][1][1], &uchar_0, &uchar_9 };
static s_key_value const key_host_1_0_value =          { UCHAR, 1, &ethernet[0][1][0], &uchar_0, &uchar_9 };
static s_key_value const key_host_2_2_value =          { UCHAR, 1, &ethernet[0][2][2], &uchar_0, &uchar_9 };
static s_key_value const key_host_2_1_value =          { UCHAR, 1, &ethernet[0][2][1], &uchar_0, &uchar_9 };
static s_key_value const key_host_2_0_value =          { UCHAR, 1, &ethernet[0][2][0], &uchar_0, &uchar_9 };
static s_key_value const key_host_3_2_value =          { UCHAR, 1, &ethernet[0][3][2], &uchar_0, &uchar_9 };
static s_key_value const key_host_3_1_value =          { UCHAR, 1, &ethernet[0][3][1], &uchar_0, &uchar_9 };
static s_key_value const key_host_3_0_value =          { UCHAR, 1, &ethernet[0][3][0], &uchar_0, &uchar_9 };
static s_key_value const key_mask_0_2_value =          { UCHAR, 1, &ethernet[1][0][2], &uchar_0, &uchar_9 };
static s_key_value const key_mask_0_1_value =          { UCHAR, 1, &ethernet[1][0][1], &uchar_0, &uchar_9 };
static s_key_value const key_mask_0_0_value =          { UCHAR, 1, &ethernet[1][0][0], &uchar_0, &uchar_9 };
static s_key_value const key_mask_1_2_value =          { UCHAR, 1, &ethernet[1][1][2], &uchar_0, &uchar_9 };
static s_key_value const key_mask_1_1_value =          { UCHAR, 1, &ethernet[1][1][1], &uchar_0, &uchar_9 };
static s_key_value const key_mask_1_0_value =          { UCHAR, 1, &ethernet[1][1][0], &uchar_0, &uchar_9 };
static s_key_value const key_mask_2_2_value =          { UCHAR, 1, &ethernet[1][2][2], &uchar_0, &uchar_9 };
static s_key_value const key_mask_2_1_value =          { UCHAR, 1, &ethernet[1][2][1], &uchar_0, &uchar_9 };
static s_key_value const key_mask_2_0_value =          { UCHAR, 1, &ethernet[1][2][0], &uchar_0, &uchar_9 };
static s_key_value const key_mask_3_2_value =          { UCHAR, 1, &ethernet[1][3][2], &uchar_0, &uchar_9 };
static s_key_value const key_mask_3_1_value =          { UCHAR, 1, &ethernet[1][3][1], &uchar_0, &uchar_9 };
static s_key_value const key_mask_3_0_value =          { UCHAR, 1, &ethernet[1][3][0], &uchar_0, &uchar_9 };
static s_key_value const key_gate_0_2_value =          { UCHAR, 1, &ethernet[2][0][2], &uchar_0, &uchar_9 };
static s_key_value const key_gate_0_1_value =          { UCHAR, 1, &ethernet[2][0][1], &uchar_0, &uchar_9 };
static s_key_value const key_gate_0_0_value =          { UCHAR, 1, &ethernet[2][0][0], &uchar_0, &uchar_9 };
static s_key_value const key_gate_1_2_value =          { UCHAR, 1, &ethernet[2][1][2], &uchar_0, &uchar_9 };
static s_key_value const key_gate_1_1_value =          { UCHAR, 1, &ethernet[2][1][1], &uchar_0, &uchar_9 };
static s_key_value const key_gate_1_0_value =          { UCHAR, 1, &ethernet[2][1][0], &uchar_0, &uchar_9 };
static s_key_value const key_gate_2_2_value =          { UCHAR, 1, &ethernet[2][2][2], &uchar_0, &uchar_9 };
static s_key_value const key_gate_2_1_value =          { UCHAR, 1, &ethernet[2][2][1], &uchar_0, &uchar_9 };
static s_key_value const key_gate_2_0_value =          { UCHAR, 1, &ethernet[2][2][0], &uchar_0, &uchar_9 };
static s_key_value const key_gate_3_2_value =          { UCHAR, 1, &ethernet[2][3][2], &uchar_0, &uchar_9 };
static s_key_value const key_gate_3_1_value =          { UCHAR, 1, &ethernet[2][3][1], &uchar_0, &uchar_9 };
static s_key_value const key_gate_3_0_value =          { UCHAR, 1, &ethernet[2][3][0], &uchar_0, &uchar_9 };
static s_key_value const key_bacnet_device_id_4_value =  { UCHAR, 1, &bacnet_device_id[4],  &uchar_0, &uchar_9 };
static s_key_value const key_bacnet_device_id_3_value =  { UCHAR, 1, &bacnet_device_id[3],  &uchar_0, &uchar_9 };
static s_key_value const key_bacnet_device_id_2_value =  { UCHAR, 1, &bacnet_device_id[2],  &uchar_0, &uchar_9 };
static s_key_value const key_bacnet_device_id_1_value =  { UCHAR, 1, &bacnet_device_id[1],  &uchar_0, &uchar_9 };
static s_key_value const key_bacnet_device_id_0_value =  { UCHAR, 1, &bacnet_device_id[0],  &uchar_0, &uchar_9 };
static s_key_value const key_bacnet_network_nr_4_value = { UCHAR, 1, &bacnet_network_nr[4],  &uchar_0, &uchar_9 };
static s_key_value const key_bacnet_network_nr_3_value = { UCHAR, 1, &bacnet_network_nr[3],  &uchar_0, &uchar_9 };
static s_key_value const key_bacnet_network_nr_2_value = { UCHAR, 1, &bacnet_network_nr[2],  &uchar_0, &uchar_9 };
static s_key_value const key_bacnet_network_nr_1_value = { UCHAR, 1, &bacnet_network_nr[1],  &uchar_0, &uchar_9 };
static s_key_value const key_bacnet_network_nr_0_value = { UCHAR, 1, &bacnet_network_nr[0],  &uchar_0, &uchar_9 };
// BACnet
static s_key_value const key_bacnet_enable_value =   { UCHAR, 3, &opt_alg.BACnet_enabled, &uchar_0, &uchar_1 };
#endif // ETHERNET
//static s_key_value const key_RS232_baudrate_value =  { INT, 3, &opt_alg.com1_bd, &int_96, &int_384 };

static s_key_value const key_taalkeuze_value = { UCHAR, 2, &opt_alg.taalkeuze  , &uchar_0, &aantal_talen };
static s_key_value const key_taalkeuze_inst_value = { UCHAR, 2, &opt_alg.taalkeuze_inst  , &uchar_0, &uchar_2 };
static s_key_value const key_lcd_dimmen_value = { UCHAR, 2, &opt_alg.lcd_dimmen  , &uchar_0, &uchar_1 };
static s_key_value const key_code_value = { UINT, 4, &eerstecode, &int_0, &int_9999 };
static s_key_value const key_bevestig_value = { UINT, 4, &tweedecode, &int_0, &int_9999 };
#ifdef PASSWORD
static s_key_value const key_gebruiker_1_level_value =  { UCHAR,  1, &opt_alg.password_level[1], &uchar_1, &uchar_3 };
static s_key_value const key_gebruiker_2_level_value =  { UCHAR,  1, &opt_alg.password_level[2], &uchar_1, &uchar_3 };
static s_key_value const key_gebruiker_3_level_value =  { UCHAR,  1, &opt_alg.password_level[3], &uchar_1, &uchar_3 };
static s_key_value const key_gebruiker_4_level_value =  { UCHAR,  1, &opt_alg.password_level[4], &uchar_1, &uchar_3 };
#endif // PASSWORD


s_key_action const opt_alg_2_key_action[] =
{
  { //*** Begin scherm
    0,                                // nr
    0,                                // index
    &start_flag,                      // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_start_disp,                   // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Start_Func,                 // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  { //*** Taalkeuze 
    1,                                // nr
    0,                                // index
    (unsigned char *)&option_on,      // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_taalkeuze_disp,               // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Option_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    1,                                // nr
    1,                                // index
    (unsigned char *)&option_on,      // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_taalkeuze_disp,               // display
    &disp_cursor_225_78_150,          // cursor
    &key_taalkeuze_value,             // *value
    Dummy_Func,                       // void (*number)(void); 
    ArrowTaal,                        // void (*arrow)(void); 
    EnterTaal,                        // void (*enter)(void);
  },
  { //*** Opties wissen
    2,               
    0,               
    (unsigned char *)&option_on,      
    (unsigned char *)&option_index_0, 
    lcd_data_wissen_disp,   
    0,               
    &dummy_value,    
    Dummy_Func,      
    Arrow_Data_Wissen,
    Dummy_Func,     
  },
  { //*** Helderheid
    4,                                // nr
    0,                                // index
    (unsigned char *)&option_on,      // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_angle_disp,                   // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Option_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    4,                                // nr
    1,                                // index
    (unsigned char *)&option_on,      // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_angle_disp,                   // display
    &disp_cursor_207_78_8,            // cursor
    &key_lcd_angle_value,             // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Lcd_Angle_value,            // void (*arrow)(void); 
    Increment_Func_Index,             // void (*enter)(void);
  },
  { //*** Display dimmen 
    5,                                // nr
    0,                                // index
    (unsigned char *)&option_on,      // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_dimmen_disp,                  // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Option_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    5,                                // nr
    1,                                // index
    (unsigned char *)&option_on,      // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_dimmen_disp,                  // display
    &disp_cursor_checkbox,            // cursor
    &key_lcd_dimmen_value,            // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Scroll_Option_Value,        // void (*arrow)(void); 
    Increment_Func_Index,             // void (*enter)(void);
  },
  { // Computer Nummer
    7,                                // nr
    0,                                // index
    (unsigned char *)&option_on,      // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_computer_nr_disp,             // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Option_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    7,                                // nr
    1,                                // index
    (unsigned char *)&option_on,      // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_computer_nr_disp,             // display
    &disp_cursor_207_78_8,   // cursor
    &key_adres_val,                   // *value
    Number_Value,                     // void (*number)(void); 
    Arrow_Computer_Nr,                // void (*arrow)(void); 
    Enter_Computer_Nr,                // void (*enter)(void);
  },
  { // Can backbone
    8,                                // nr
    0,                                // index
    &opt_alg.CANopenPossible,         // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_can_backbone_disp,            // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Option_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    8,                                // nr
    1,                                // index
    &opt_alg.CANopenPossible,         // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_can_backbone_disp,            // display
    &disp_cursor_checkbox,            // cursor
    &key_can_backbone_val,            // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Can_Backbone,               // void (*arrow)(void); 
    Enter_Can_Backbone,               // void (*enter)(void);
  },
  { // Baudrate CAN Backbone
    9,                                // nr
    0,                                // index
    &opt_alg.can_backbone,            // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_can_baudrate_disp,            // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Option_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    9,                                // nr
    1,                                // index
    &opt_alg.can_backbone,            // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_can_baudrate_disp,            // display
    &disp_cursor_192_78_8,            // cursor
    &key_can_baudrate_val,            // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Can_Baudrate,               // void (*arrow)(void); 
    Enter_Can_Baudrate,               // void (*enter)(void);
  },
  #ifdef CAN_BACKBONE_PC_WARNING
  { // CAN Backbone PC waarschuwing
    10,                             // nr
    0,                              // index
//    &opt_alg.can_backbone,      // option
    (unsigned char *)&option_off,
    (unsigned char *)&option_index_0, // Optie index 
    lcd_can_backbone_pc_waarschuwing_disp,// display
    0,                              // cursor
    &dummy_value,                   // *value
    Dummy_Func,                     // void (*number)(void); 
    Arrow_Option_Func,              // void (*arrow)(void); 
    Dummy_Func,                     // void (*enter)(void);
  },
  {
    10,                             // nr
    1,                              // index
//    &opt_alg.can_backbone,      // option
    (unsigned char *)&option_off,
    (unsigned char *)&option_index_0, // Optie index 
    lcd_can_backbone_pc_waarschuwing_disp,// display
    &disp_pc_1_cursor_checkbox,     // cursor
    &key_pc_1_enable_value,         // *value
    Dummy_Func,                     // void (*number)(void); 
    Arrow_Can_PC_Enable_Value,      // void (*arrow)(void); 
    Enter_Can_PC_Enable_Value,      // void (*enter)(void);
  },
  {
    10,                             // nr
    2,                              // index
//    &opt_alg.can_backbone,      // option
    (unsigned char *)&option_off,
    (unsigned char *)&option_index_0, // Optie index 
    lcd_can_backbone_pc_waarschuwing_disp,// display
    &disp_pc_2_cursor_checkbox,     // cursor
    &key_pc_2_enable_value,         // *value
    Dummy_Func,                     // void (*number)(void); 
    Arrow_Can_PC_Enable_Value,      // void (*arrow)(void); 
    Enter_Can_PC_Enable_Value,      // void (*enter)(void);
  },
  {
    10,                             // nr
    3,                              // index
//    &opt_alg.can_backbone,      // option
    (unsigned char *)&option_off,
    (unsigned char *)&option_index_0, // Optie index 
    lcd_can_backbone_pc_waarschuwing_disp,// display
    &disp_pc_3_cursor_checkbox,     // cursor
    &key_pc_3_enable_value,         // *value
    Dummy_Func,                     // void (*number)(void); 
    Arrow_Can_PC_Enable_Value,      // void (*arrow)(void); 
    Enter_Can_PC_Enable_Value,      // void (*enter)(void);
  },
  #else // CAN_BACKBONE_PC_WARNING
  { // Can RS232
    10,              // nr
    0,               // index
//    &opt_alg.can_backbone,      // option
    (unsigned char *)&option_off,
    (unsigned char *)&option_index_0,   // Optie index 
    lcd_can_rs232_disp,  // display
    0,               // cursor
    &dummy_value,     // *value
    Dummy_Func,      // void (*number)(void); 
    Arrow_Option_Func, // void (*arrow)(void); 
    Dummy_Func,      // void (*enter)(void);
  },
  {
    10,              // nr
    1,               // index
//    &opt_alg.can_backbone,      // option
    (unsigned char *)&option_off,
    (unsigned char *)&option_index_0,   // Optie index 
    lcd_can_rs232_disp,  // display
    &disp_cursor_checkbox,   // cursor
    &key_can_rs232_val,     // *value
    Dummy_Func,      // void (*number)(void); 
    Arrow_Can_RS232, // void (*arrow)(void); 
    Enter_Can_RS232,      // void (*enter)(void);
  },
  #endif // CAN_BACKBONE_PC_WARNING


  { // Com1 Enable
    11,                             // nr
    0,                              // index
    (unsigned char *)&option_on,    // option
    (unsigned char *)&option_index_0,// Optie index 
    lcd_com1_enable_disp,           // display
    0,                              // cursor
    &dummy_value,                   // *value
    Dummy_Func,                     // void (*number)(void); 
    Arrow_Option_Func,              // void (*arrow)(void); 
    Dummy_Func,                     // void (*enter)(void);
  },
  {
    11,                             // nr
    1,                              // index
    (unsigned char *)&option_on,    // option
    (unsigned char *)&option_index_0,// Optie index 
    lcd_com1_enable_disp,           // display
    &disp_cursor_checkbox,          // cursor
    &key_com1_enable_value,         // *value
    Dummy_Func,                     // void (*number)(void); 
    Arrow_Com1_Enable_Value,        // void (*arrow)(void); 
    Increment_Func_Index,           // void (*enter)(void);
  },
  { // Com1 Baudrate RS232
    12,                             // nr
    0,                              // index
    &opt_alg.com1_enabled,          // option
    (unsigned char *)&option_index_0,// Optie index 
    lcd_com1_baudrate_disp,         // display
    0,                              // cursor
    &dummy_value,                   // *value
    Dummy_Func,                     // void (*number)(void); 
    Arrow_Option_Func,              // void (*arrow)(void); 
    Dummy_Func,                     // void (*enter)(void);
  },
  {
    12,                             // nr
    1,                              // index
    &opt_alg.com1_enabled,          // option
    (unsigned char *)&option_index_0,// Optie index 
    lcd_com1_baudrate_disp,         // display
    &disp_cursor_192_78_8,          // cursor
    &key_com1_baudrate_value,       // *value
    Dummy_Func,                     // void (*number)(void); 
    Arrow_Com1_Baudrate_Value,      // void (*arrow)(void); 
    Increment_Func_Index,           // void (*enter)(void);
  },
  { // Com1 modem
    13,                             // nr
    0,                              // index
    &opt_alg.com1_enabled,          // option
    (unsigned char *)&option_index_0,// Optie index 
    lcd_com1_modem_disp,            // display
    0,                              // cursor
    &dummy_value,                   // *value
    Dummy_Func,                     // void (*number)(void); 
    Arrow_Option_Func,              // void (*arrow)(void); 
    Dummy_Func,                     // void (*enter)(void);
  },
  {
    13,                             // nr
    1,                              // index
    &opt_alg.com1_enabled,          // option
    (unsigned char *)&option_index_0,// Optie index 
    lcd_com1_modem_disp,            // display
    &disp_cursor_checkbox,          // cursor
    &key_com1_modem_value,          // *value
    Dummy_Func,                     // void (*number)(void); 
    Arrow_Com1_Modem_Value,         // void (*arrow)(void); 
    Increment_Func_Index,           // void (*enter)(void);
  },
  { // Com1 modem answer
    14,                             // nr
    0,                              // index
    &opt_alg.com1_modem,            // option
    (unsigned char *)&option_index_0,// Optie index 
    lcd_com1_modem_answer_disp,     // display
    0,                              // cursor
    &dummy_value,                   // *value
    Dummy_Func,                     // void (*number)(void); 
    Arrow_Option_Func,              // void (*arrow)(void); 
    Dummy_Func,                     // void (*enter)(void);
  },
  {
    14,                             // nr
    1,                              // index
    &opt_alg.com1_modem,            // option
    (unsigned char *)&option_index_0,// Optie index 
    lcd_com1_modem_answer_disp,     // display
    &disp_cursor_207_78_8,          // cursor
    &key_com1_modem_answer_value,   // *value
    Number_Value,                   // void (*number)(void); 
    Arrow_Com1_Modem_Answer_Value,  // void (*arrow)(void); 
    Enter_Com1_Modem_Answer_Value,  // void (*enter)(void);
  },
  { // Com2 Enable
    15,                             // nr
    0,                              // index
    (unsigned char *)&option_on,    // option
    (unsigned char *)&option_index_0,// Optie index 
    lcd_com2_enable_disp,           // display
    0,                              // cursor
    &dummy_value,                   // *value
    Dummy_Func,                     // void (*number)(void); 
    Arrow_Option_Func,              // void (*arrow)(void); 
    Dummy_Func,                     // void (*enter)(void);
  },
  {
    15,                             // nr
    1,                              // index
    (unsigned char *)&option_on,    // option
    (unsigned char *)&option_index_0,// Optie index 
    lcd_com2_enable_disp,           // display
    &disp_cursor_checkbox,          // cursor
    &key_com2_enable_value,         // *value
    Dummy_Func,                     // void (*number)(void); 
    Arrow_Com2_Enable_Value,        // void (*arrow)(void); 
    Increment_Func_Index,           // void (*enter)(void);
  },
  { // Com2 Baudrate RS232
    16,                             // nr
    0,                              // index
    &opt_alg.com2_enabled,          // option
    (unsigned char *)&option_index_0,// Optie index 
    lcd_com2_baudrate_disp,         // display
    0,                              // cursor
    &dummy_value,                   // *value
    Dummy_Func,                     // void (*number)(void); 
    Arrow_Option_Func,              // void (*arrow)(void); 
    Dummy_Func,                     // void (*enter)(void);
  },
  {
    16,                             // nr
    1,                              // index
    &opt_alg.com2_enabled,          // option
    (unsigned char *)&option_index_0,// Optie index 
    lcd_com2_baudrate_disp,         // display
    &disp_cursor_192_78_8,          // cursor
    &key_com2_baudrate_value,       // *value
    Dummy_Func,                     // void (*number)(void); 
    Arrow_Com2_Baudrate_Value,      // void (*arrow)(void); 
    Increment_Func_Index,           // void (*enter)(void);
  },
  { // Com2 modem
    17,                             // nr
    0,                              // index
    &opt_alg.com2_enabled,          // option
    (unsigned char *)&option_index_0,// Optie index 
    lcd_com2_modem_disp,            // display
    0,                              // cursor
    &dummy_value,                   // *value
    Dummy_Func,                     // void (*number)(void); 
    Arrow_Option_Func,              // void (*arrow)(void); 
    Dummy_Func,                     // void (*enter)(void);
  },
  {
    17,                             // nr
    1,                              // index
    &opt_alg.com2_enabled,          // option
    (unsigned char *)&option_index_0,// Optie index 
    lcd_com2_modem_disp,            // display
    &disp_cursor_checkbox,          // cursor
    &key_com2_modem_value,          // *value
    Dummy_Func,                     // void (*number)(void); 
    Arrow_Com2_Modem_Value,         // void (*arrow)(void); 
    Increment_Func_Index,           // void (*enter)(void);
  },
  { // Com2 modem answer
    18,                             // nr
    0,                              // index
    &opt_alg.com2_modem,            // option
    (unsigned char *)&option_index_0,// Optie index 
    lcd_com2_modem_answer_disp,     // display
    0,                              // cursor
    &dummy_value,                   // *value
    Dummy_Func,                     // void (*number)(void); 
    Arrow_Option_Func,              // void (*arrow)(void); 
    Dummy_Func,                     // void (*enter)(void);
  },
  {
    18,                             // nr
    1,                              // index
    &opt_alg.com2_modem,            // option
    (unsigned char *)&option_index_0,// Optie index 
    lcd_com2_modem_answer_disp,     // display
    &disp_cursor_207_78_8,          // cursor
    &key_com2_modem_answer_value,   // *value
    Number_Value,                   // void (*number)(void); 
    Arrow_Com2_Modem_Answer_Value,  // void (*arrow)(void); 
    Enter_Com2_Modem_Answer_Value,  // void (*enter)(void);
  },
#ifdef ETHERNET
  { // Ethernet Enable
    19,                // nr
    0,                // index
    (unsigned char *)&option_on,       // option
    (unsigned char *)&option_index_0,// Optie index 
    lcd_ethernet_enable_disp,  // display
    0,                // cursor
    &dummy_value,     // *value
    Dummy_Func,       // void (*number)(void); 
    Arrow_Option_Alg_Func, // void (*arrow)(void); 
    Dummy_Func,       // void (*enter)(void);
  },
  {
    19,                // nr
    1,                // index
    (unsigned char *)&option_on,       // option
    (unsigned char *)&option_index_0,// Optie index 
    lcd_ethernet_enable_disp,  // display
    &disp_cursor_checkbox,   // cursor
    &key_ethernet_enable_value,     // *value
    Dummy_Func,       // void (*number)(void); 
    Arrow_Ethernet_Enable_Value,      // void (*arrow)(void); 
    Enter_Ethernet_Enable_Value,      // void (*enter)(void);
  },
  { // Ethernet 
    20,                // nr
    0,                // index
    (unsigned char *)&opt_alg.ethernet_enabled,       // option
    (unsigned char *)&option_index_0,// Optie index 
    lcd_ethernet_disp,  // display
    0,                // cursor
    &dummy_value,     // *value
    Dummy_Func,       // void (*number)(void); 
    Arrow_Ethernet_Func, // void (*arrow)(void); 
    Dummy_Func,       // void (*enter)(void);
  },
  { // Ethernet 
    20,                // nr
    1,                // index
    (unsigned char *)&opt_alg.ethernet_enabled,       // option
    (unsigned char *)&option_index_0,// Optie index 
    lcd_ethernet_disp,  // display
    &disp_cursor_port_3,                // cursor
    &key_port_3_value,     // *value
    Dummy_Func,       // void (*number)(void); 
    Arrow_Ethernet_Port_Value, // void (*arrow)(void); 
    Enter_Ethernet_Port_Value, // void (*enter)(void);
  },
  { // Ethernet 
    20,                // nr
    2,                // index
    (unsigned char *)&opt_alg.ethernet_enabled,       // option
    (unsigned char *)&option_index_0,// Optie index 
    lcd_ethernet_disp,  // display
    &disp_cursor_port_2,                // cursor
    &key_port_2_value,     // *value
    Dummy_Func,       // void (*number)(void); 
    Arrow_Ethernet_Port_Value, // void (*arrow)(void); 
    Enter_Ethernet_Port_Value, // void (*enter)(void);
  },
  { // Ethernet 
    20,                // nr
    3,                // index
    (unsigned char *)&opt_alg.ethernet_enabled,       // option
    (unsigned char *)&option_index_0,// Optie index 
    lcd_ethernet_disp,  // display
    &disp_cursor_port_1,                // cursor
    &key_port_1_value,     // *value
    Dummy_Func,       // void (*number)(void); 
    Arrow_Ethernet_Port_Value, // void (*arrow)(void); 
    Enter_Ethernet_Port_Value, // void (*enter)(void);
  },
  { // Ethernet 
    20,                // nr
    4,                // index
    (unsigned char *)&opt_alg.ethernet_enabled,       // option
    (unsigned char *)&option_index_0,// Optie index 
    lcd_ethernet_disp,  // display
    &disp_cursor_port_0,                // cursor
    &key_port_0_value,     // *value
    Dummy_Func,       // void (*number)(void); 
    Arrow_Ethernet_Port_Value, // void (*arrow)(void); 
    Enter_Ethernet_Port_Value, // void (*enter)(void);
  },
  { // Ethernet 
    20,                // nr
    5,                // index
    (unsigned char *)&opt_alg.ethernet_enabled,       // option
    (unsigned char *)&option_index_0,// Optie index 
    lcd_ethernet_disp,  // display
    &disp_cursor_host_0_2,                // cursor
    &key_host_0_2_value,     // *value
    Dummy_Func,       // void (*number)(void); 
    Arrow_Ethernet_Value, // void (*arrow)(void); 
    Enter_Ethernet_Value, // void (*enter)(void);
  },
  { // Ethernet 
    20,                // nr
    6,                // index
    (unsigned char *)&opt_alg.ethernet_enabled,       // option
    (unsigned char *)&option_index_0,// Optie index 
    lcd_ethernet_disp,  // display
    &disp_cursor_host_0_1,                // cursor
    &key_host_0_1_value,     // *value
    Dummy_Func,       // void (*number)(void); 
    Arrow_Ethernet_Value, // void (*arrow)(void); 
    Enter_Ethernet_Value, // void (*enter)(void);
  },
  { // Ethernet 
    20,                // nr
    7,                // index
    (unsigned char *)&opt_alg.ethernet_enabled,       // option
    (unsigned char *)&option_index_0,// Optie index 
    lcd_ethernet_disp,  // display
    &disp_cursor_host_0_0,                // cursor
    &key_host_0_0_value,     // *value
    Dummy_Func,       // void (*number)(void); 
    Arrow_Ethernet_Value, // void (*arrow)(void); 
    Enter_Ethernet_Value, // void (*enter)(void);
  },
  { // Ethernet 
    20,                // nr
    8,                // index
    (unsigned char *)&opt_alg.ethernet_enabled,       // option
    (unsigned char *)&option_index_0,// Optie index 
    lcd_ethernet_disp,  // display
    &disp_cursor_host_1_2,                // cursor
    &key_host_1_2_value,     // *value
    Dummy_Func,       // void (*number)(void); 
    Arrow_Ethernet_Value, // void (*arrow)(void); 
    Enter_Ethernet_Value, // void (*enter)(void);
  },
  { // Ethernet 
    20,                // nr
    9,                // index
    (unsigned char *)&opt_alg.ethernet_enabled,       // option
    (unsigned char *)&option_index_0,// Optie index 
    lcd_ethernet_disp,  // display
    &disp_cursor_host_1_1,                // cursor
    &key_host_1_1_value,     // *value
    Dummy_Func,       // void (*number)(void); 
    Arrow_Ethernet_Value, // void (*arrow)(void); 
    Enter_Ethernet_Value, // void (*enter)(void);
  },
  { // Ethernet 
    20,                // nr
    10,                // index
    (unsigned char *)&opt_alg.ethernet_enabled,       // option
    (unsigned char *)&option_index_0,// Optie index 
    lcd_ethernet_disp,  // display
    &disp_cursor_host_1_0,                // cursor
    &key_host_1_0_value,     // *value
    Dummy_Func,       // void (*number)(void); 
    Arrow_Ethernet_Value, // void (*arrow)(void); 
    Enter_Ethernet_Value, // void (*enter)(void);
  },
  { // Ethernet 
    20,                // nr
    11,                // index
    (unsigned char *)&opt_alg.ethernet_enabled,       // option
    (unsigned char *)&option_index_0,// Optie index 
    lcd_ethernet_disp,  // display
    &disp_cursor_host_2_2,                // cursor
    &key_host_2_2_value,     // *value
    Dummy_Func,       // void (*number)(void); 
    Arrow_Ethernet_Value, // void (*arrow)(void); 
    Enter_Ethernet_Value, // void (*enter)(void);
  },
  { // Ethernet 
    20,                // nr
    12,                // index
    (unsigned char *)&opt_alg.ethernet_enabled,       // option
    (unsigned char *)&option_index_0,// Optie index 
    lcd_ethernet_disp,  // display
    &disp_cursor_host_2_1,                // cursor
    &key_host_2_1_value,     // *value
    Dummy_Func,       // void (*number)(void); 
    Arrow_Ethernet_Value, // void (*arrow)(void); 
    Enter_Ethernet_Value, // void (*enter)(void);
  },
  { // Ethernet 
    20,                // nr
    13,                // index
    (unsigned char *)&opt_alg.ethernet_enabled,       // option
    (unsigned char *)&option_index_0,// Optie index 
    lcd_ethernet_disp,  // display
    &disp_cursor_host_2_0,                // cursor
    &key_host_2_0_value,     // *value
    Dummy_Func,       // void (*number)(void); 
    Arrow_Ethernet_Value, // void (*arrow)(void); 
    Enter_Ethernet_Value, // void (*enter)(void);
  },
  { // Ethernet 
    20,                // nr
    14,                // index
    (unsigned char *)&opt_alg.ethernet_enabled,       // option
    (unsigned char *)&option_index_0,// Optie index 
    lcd_ethernet_disp,  // display
    &disp_cursor_host_3_2,                // cursor
    &key_host_3_2_value,     // *value
    Dummy_Func,       // void (*number)(void); 
    Arrow_Ethernet_Value, // void (*arrow)(void); 
    Enter_Ethernet_Value, // void (*enter)(void);
  },
  { // Ethernet 
    20,                // nr
    15,                // index
    (unsigned char *)&opt_alg.ethernet_enabled,       // option
    (unsigned char *)&option_index_0,// Optie index 
    lcd_ethernet_disp,  // display
    &disp_cursor_host_3_1,                // cursor
    &key_host_3_1_value,     // *value
    Dummy_Func,       // void (*number)(void); 
    Arrow_Ethernet_Value, // void (*arrow)(void); 
    Enter_Ethernet_Value, // void (*enter)(void);
  },
  { // Ethernet 
    20,                // nr
    16,                // index
    (unsigned char *)&opt_alg.ethernet_enabled,       // option
    (unsigned char *)&option_index_0,// Optie index 
    lcd_ethernet_disp,  // display
    &disp_cursor_host_3_0,                // cursor
    &key_host_3_0_value,     // *value
    Dummy_Func,       // void (*number)(void); 
    Arrow_Ethernet_Value, // void (*arrow)(void); 
    Enter_Ethernet_Value, // void (*enter)(void);
  },

  { // Ethernet 
    20,                // nr
    17,                // index
    (unsigned char *)&opt_alg.ethernet_enabled,       // option
    (unsigned char *)&option_index_0,// Optie index 
    lcd_ethernet_disp,  // display
    &disp_cursor_mask_0_2,                // cursor
    &key_mask_0_2_value,     // *value
    Dummy_Func,       // void (*number)(void); 
    Arrow_Ethernet_Value, // void (*arrow)(void); 
    Enter_Ethernet_Value, // void (*enter)(void);
  },
  { // Ethernet 
    20,                // nr
    18,                // index
    (unsigned char *)&opt_alg.ethernet_enabled,       // option
    (unsigned char *)&option_index_0,// Optie index 
    lcd_ethernet_disp,  // display
    &disp_cursor_mask_0_1,                // cursor
    &key_mask_0_1_value,     // *value
    Dummy_Func,       // void (*number)(void); 
    Arrow_Ethernet_Value, // void (*arrow)(void); 
    Enter_Ethernet_Value, // void (*enter)(void);
  },
  { // Ethernet 
    20,                // nr
    19,                // index
    (unsigned char *)&opt_alg.ethernet_enabled,       // option
    (unsigned char *)&option_index_0,// Optie index 
    lcd_ethernet_disp,  // display
    &disp_cursor_mask_0_0,                // cursor
    &key_mask_0_0_value,     // *value
    Dummy_Func,       // void (*number)(void); 
    Arrow_Ethernet_Value, // void (*arrow)(void); 
    Enter_Ethernet_Value, // void (*enter)(void);
  },
  { // Ethernet 
    20,                // nr
    20,                // index
    (unsigned char *)&opt_alg.ethernet_enabled,       // option
    (unsigned char *)&option_index_0,// Optie index 
    lcd_ethernet_disp,  // display
    &disp_cursor_mask_1_2,                // cursor
    &key_mask_1_2_value,     // *value
    Dummy_Func,       // void (*number)(void); 
    Arrow_Ethernet_Value, // void (*arrow)(void); 
    Enter_Ethernet_Value, // void (*enter)(void);
  },
  { // Ethernet 
    20,                // nr
    21,                // index
    (unsigned char *)&opt_alg.ethernet_enabled,       // option
    (unsigned char *)&option_index_0,// Optie index 
    lcd_ethernet_disp,  // display
    &disp_cursor_mask_1_1,                // cursor
    &key_mask_1_1_value,     // *value
    Dummy_Func,       // void (*number)(void); 
    Arrow_Ethernet_Value, // void (*arrow)(void); 
    Enter_Ethernet_Value, // void (*enter)(void);
  },
  { // Ethernet 
    20,                // nr
    22,                // index
    (unsigned char *)&opt_alg.ethernet_enabled,       // option
    (unsigned char *)&option_index_0,// Optie index 
    lcd_ethernet_disp,  // display
    &disp_cursor_mask_1_0,                // cursor
    &key_mask_1_0_value,     // *value
    Dummy_Func,       // void (*number)(void); 
    Arrow_Ethernet_Value, // void (*arrow)(void); 
    Enter_Ethernet_Value, // void (*enter)(void);
  },
  { // Ethernet 
    20,                // nr
    23,                // index
    (unsigned char *)&opt_alg.ethernet_enabled,       // option
    (unsigned char *)&option_index_0,// Optie index 
    lcd_ethernet_disp,  // display
    &disp_cursor_mask_2_2,                // cursor
    &key_mask_2_2_value,     // *value
    Dummy_Func,       // void (*number)(void); 
    Arrow_Ethernet_Value, // void (*arrow)(void); 
    Enter_Ethernet_Value, // void (*enter)(void);
  },
  { // Ethernet 
    20,                // nr
    24,                // index
    (unsigned char *)&opt_alg.ethernet_enabled,       // option
    (unsigned char *)&option_index_0,// Optie index 
    lcd_ethernet_disp,  // display
    &disp_cursor_mask_2_1,                // cursor
    &key_mask_2_1_value,     // *value
    Dummy_Func,       // void (*number)(void); 
    Arrow_Ethernet_Value, // void (*arrow)(void); 
    Enter_Ethernet_Value, // void (*enter)(void);
  },
  { // Ethernet 
    20,                // nr
    25,                // index
    (unsigned char *)&opt_alg.ethernet_enabled,       // option
    (unsigned char *)&option_index_0,// Optie index 
    lcd_ethernet_disp,  // display
    &disp_cursor_mask_2_0,                // cursor
    &key_mask_2_0_value,     // *value
    Dummy_Func,       // void (*number)(void); 
    Arrow_Ethernet_Value, // void (*arrow)(void); 
    Enter_Ethernet_Value, // void (*enter)(void);
  },
  { // Ethernet 
    20,                // nr
    26,                // index
    (unsigned char *)&opt_alg.ethernet_enabled,       // option
    (unsigned char *)&option_index_0,// Optie index 
    lcd_ethernet_disp,  // display
    &disp_cursor_mask_3_2,                // cursor
    &key_mask_3_2_value,     // *value
    Dummy_Func,       // void (*number)(void); 
    Arrow_Ethernet_Value, // void (*arrow)(void); 
    Enter_Ethernet_Value, // void (*enter)(void);
  },
  { // Ethernet 
    20,                // nr
    27,                // index
    (unsigned char *)&opt_alg.ethernet_enabled,       // option
    (unsigned char *)&option_index_0,// Optie index 
    lcd_ethernet_disp,  // display
    &disp_cursor_mask_3_1,                // cursor
    &key_mask_3_1_value,     // *value
    Dummy_Func,       // void (*number)(void); 
    Arrow_Ethernet_Value, // void (*arrow)(void); 
    Enter_Ethernet_Value, // void (*enter)(void);
  },
  { // Ethernet 
    20,                // nr
    28,                // index
    (unsigned char *)&opt_alg.ethernet_enabled,       // option
    (unsigned char *)&option_index_0,// Optie index 
    lcd_ethernet_disp,  // display
    &disp_cursor_mask_3_0,                // cursor
    &key_mask_3_0_value,     // *value
    Dummy_Func,       // void (*number)(void); 
    Arrow_Ethernet_Value, // void (*arrow)(void); 
    Enter_Ethernet_Value, // void (*enter)(void);
  },

  { // Ethernet 
    20,                // nr
    29,                // index
    (unsigned char *)&opt_alg.ethernet_enabled,       // option
    (unsigned char *)&option_index_0,// Optie index 
    lcd_ethernet_disp,  // display
    &disp_cursor_gate_0_2,                // cursor
    &key_gate_0_2_value,     // *value
    Dummy_Func,       // void (*number)(void); 
    Arrow_Ethernet_Value, // void (*arrow)(void); 
    Enter_Ethernet_Value, // void (*enter)(void);
  },
  { // Ethernet 
    20,                // nr
    30,                // index
    (unsigned char *)&opt_alg.ethernet_enabled,       // option
    (unsigned char *)&option_index_0,// Optie index 
    lcd_ethernet_disp,  // display
    &disp_cursor_gate_0_1,                // cursor
    &key_gate_0_1_value,     // *value
    Dummy_Func,       // void (*number)(void); 
    Arrow_Ethernet_Value, // void (*arrow)(void); 
    Enter_Ethernet_Value, // void (*enter)(void);
  },
  { // Ethernet 
    20,                // nr
    31,                // index
    (unsigned char *)&opt_alg.ethernet_enabled,       // option
    (unsigned char *)&option_index_0,// Optie index 
    lcd_ethernet_disp,  // display
    &disp_cursor_gate_0_0,                // cursor
    &key_gate_0_0_value,     // *value
    Dummy_Func,       // void (*number)(void); 
    Arrow_Ethernet_Value, // void (*arrow)(void); 
    Enter_Ethernet_Value, // void (*enter)(void);
  },
  { // Ethernet 
    20,                // nr
    32,                // index
    (unsigned char *)&opt_alg.ethernet_enabled,       // option
    (unsigned char *)&option_index_0,// Optie index 
    lcd_ethernet_disp,  // display
    &disp_cursor_gate_1_2,                // cursor
    &key_gate_1_2_value,     // *value
    Dummy_Func,       // void (*number)(void); 
    Arrow_Ethernet_Value, // void (*arrow)(void); 
    Enter_Ethernet_Value, // void (*enter)(void);
  },
  { // Ethernet 
    20,                // nr
    33,                // index
    (unsigned char *)&opt_alg.ethernet_enabled,       // option
    (unsigned char *)&option_index_0,// Optie index 
    lcd_ethernet_disp,  // display
    &disp_cursor_gate_1_1,                // cursor
    &key_gate_1_1_value,     // *value
    Dummy_Func,       // void (*number)(void); 
    Arrow_Ethernet_Value, // void (*arrow)(void); 
    Enter_Ethernet_Value, // void (*enter)(void);
  },
  { // Ethernet 
    20,                // nr
    34,                // index
    (unsigned char *)&opt_alg.ethernet_enabled,       // option
    (unsigned char *)&option_index_0,// Optie index 
    lcd_ethernet_disp,  // display
    &disp_cursor_gate_1_0,                // cursor
    &key_gate_1_0_value,     // *value
    Dummy_Func,       // void (*number)(void); 
    Arrow_Ethernet_Value, // void (*arrow)(void); 
    Enter_Ethernet_Value, // void (*enter)(void);
  },
  { // Ethernet 
    20,                // nr
    35,                // index
    (unsigned char *)&opt_alg.ethernet_enabled,       // option
    (unsigned char *)&option_index_0,// Optie index 
    lcd_ethernet_disp,  // display
    &disp_cursor_gate_2_2,                // cursor
    &key_gate_2_2_value,     // *value
    Dummy_Func,       // void (*number)(void); 
    Arrow_Ethernet_Value, // void (*arrow)(void); 
    Enter_Ethernet_Value, // void (*enter)(void);
  },
  { // Ethernet 
    20,                // nr
    36,                // index
    (unsigned char *)&opt_alg.ethernet_enabled,       // option
    (unsigned char *)&option_index_0,// Optie index 
    lcd_ethernet_disp,  // display
    &disp_cursor_gate_2_1,                // cursor
    &key_gate_2_1_value,     // *value
    Dummy_Func,       // void (*number)(void); 
    Arrow_Ethernet_Value, // void (*arrow)(void); 
    Enter_Ethernet_Value, // void (*enter)(void);
  },
  { // Ethernet 
    20,                // nr
    37,                // index
    (unsigned char *)&opt_alg.ethernet_enabled,       // option
    (unsigned char *)&option_index_0,// Optie index 
    lcd_ethernet_disp,  // display
    &disp_cursor_gate_2_0,                // cursor
    &key_gate_2_0_value,     // *value
    Dummy_Func,       // void (*number)(void); 
    Arrow_Ethernet_Value, // void (*arrow)(void); 
    Enter_Ethernet_Value, // void (*enter)(void);
  },
  { // Ethernet 
    20,                // nr
    38,                // index
    (unsigned char *)&opt_alg.ethernet_enabled,       // option
    (unsigned char *)&option_index_0,// Optie index 
    lcd_ethernet_disp,  // display
    &disp_cursor_gate_3_2,                // cursor
    &key_gate_3_2_value,     // *value
    Dummy_Func,       // void (*number)(void); 
    Arrow_Ethernet_Value, // void (*arrow)(void); 
    Enter_Ethernet_Value, // void (*enter)(void);
  },
  { // Ethernet 
    20,                // nr
    39,                // index
    (unsigned char *)&opt_alg.ethernet_enabled,       // option
    (unsigned char *)&option_index_0,// Optie index 
    lcd_ethernet_disp,  // display
    &disp_cursor_gate_3_1,                // cursor
    &key_gate_3_1_value,     // *value
    Dummy_Func,       // void (*number)(void); 
    Arrow_Ethernet_Value, // void (*arrow)(void); 
    Enter_Ethernet_Value, // void (*enter)(void);
  },
  { // Ethernet 
    20,                // nr
    40,                // index
    (unsigned char *)&opt_alg.ethernet_enabled,       // option
    (unsigned char *)&option_index_0,// Optie index 
    lcd_ethernet_disp,  // display
    &disp_cursor_gate_3_0,                // cursor
    &key_gate_3_0_value,     // *value
    Dummy_Func,       // void (*number)(void); 
    Arrow_Ethernet_Value, // void (*arrow)(void); 
    Enter_Ethernet_Value, // void (*enter)(void);
  },
  { // Hoogendoorn Enable
    21,                // nr
    0,                // index
    (unsigned char *)&opt_alg.hoogendoorn_possible,       // option
    (unsigned char *)&option_index_0,// Optie index 
    lcd_Hoogendoorn_enable_disp,  // display
    0,                // cursor
    &dummy_value,     // *value
    Dummy_Func,       // void (*number)(void); 
    Arrow_Option_Alg_Func, // void (*arrow)(void); 
    Dummy_Func,       // void (*enter)(void);
  },
  {
    21,                // nr
    1,                // index
    (unsigned char *)&opt_alg.hoogendoorn_possible,       // option
    (unsigned char *)&option_index_0,// Optie index 
    lcd_Hoogendoorn_enable_disp,  // display
    &disp_cursor_checkbox,   // cursor
    &key_Hoogendoorn_enable_value,     // *value
    Dummy_Func,       // void (*number)(void); 
    Arrow_Hoogendoorn_Enable_Value,// void (*arrow)(void); 
    Enter_Hoogendoorn_Enable_Value, // void (*enter)(void);
  },

  { // BACnet Enable
    22,                // nr
    0,                // index
    (unsigned char *)&opt_alg.BACnet_possible,       // option
    (unsigned char *)&option_index_0,// Optie index 
    lcd_bacnet_enable_disp,  // display
    0,                // cursor
    &dummy_value,     // *value
    Dummy_Func,       // void (*number)(void); 
    Arrow_Option_Alg_Func, // void (*arrow)(void); 
    Dummy_Func,       // void (*enter)(void);
  },
  { // BACnet Enable
    22,                // nr
    1,                // index
    (unsigned char *)&opt_alg.BACnet_possible,       // option
    (unsigned char *)&option_index_0,// Optie index 
    lcd_bacnet_enable_disp,  // display
    &disp_cursor_checkbox,   // cursor
    &key_bacnet_enable_value,     // *value
    Dummy_Func,       // void (*number)(void); 
    Arrow_Bacnet_Enable_Value,      // void (*arrow)(void); 
    Enter_Bacnet_Enable_Value,      // void (*enter)(void);
  },

  { // BACnet
    23,                // nr
    0,                // index
    (unsigned char *)&opt_alg.BACnet_enabled,       // option
    (unsigned char *)&option_index_0,// Optie index 
    lcd_bacnet_disp,  // display
    0,                // cursor
    &dummy_value,     // *value
    Dummy_Func,       // void (*number)(void); 
    Arrow_Bacnet_Func, // void (*arrow)(void); 
    Dummy_Func,       // void (*enter)(void);
  },
  { // BACnet
    23,                // nr
    1,                // index
    (unsigned char *)&opt_alg.BACnet_enabled,       // option
    (unsigned char *)&option_index_0,// Optie index 
    lcd_bacnet_disp,  // display
    &disp_cursor_bacnet_device_id_4,                // cursor
    &key_bacnet_device_id_4_value,     // *value
    Dummy_Func,       // void (*number)(void); 
    Arrow_Bacnet_Device_Id_Value, // void (*arrow)(void); 
    Enter_Bacnet_Device_Id_Value, // void (*enter)(void);
  },
  { // BACnet
    23,                // nr
    2,                // index
    (unsigned char *)&opt_alg.BACnet_enabled,       // option
    (unsigned char *)&option_index_0,// Optie index 
    lcd_bacnet_disp,  // display
    &disp_cursor_bacnet_device_id_3,                // cursor
    &key_bacnet_device_id_3_value,     // *value
    Dummy_Func,       // void (*number)(void); 
    Arrow_Bacnet_Device_Id_Value, // void (*arrow)(void); 
    Enter_Bacnet_Device_Id_Value, // void (*enter)(void);
  },
  { // BACnet
    23,                // nr
    3,                // index
    (unsigned char *)&opt_alg.BACnet_enabled,       // option
    (unsigned char *)&option_index_0,// Optie index 
    lcd_bacnet_disp,  // display
    &disp_cursor_bacnet_device_id_2,                // cursor
    &key_bacnet_device_id_2_value,     // *value
    Dummy_Func,       // void (*number)(void); 
    Arrow_Bacnet_Device_Id_Value, // void (*arrow)(void); 
    Enter_Bacnet_Device_Id_Value, // void (*enter)(void);
  },
  { // BACnet
    23,                // nr
    4,                // index
    (unsigned char *)&opt_alg.BACnet_enabled,       // option
    (unsigned char *)&option_index_0,// Optie index 
    lcd_bacnet_disp,  // display
    &disp_cursor_bacnet_device_id_1,                // cursor
    &key_bacnet_device_id_1_value,     // *value
    Dummy_Func,       // void (*number)(void); 
    Arrow_Bacnet_Device_Id_Value, // void (*arrow)(void); 
    Enter_Bacnet_Device_Id_Value, // void (*enter)(void);
  },
  { // BACnet
    23,                // nr
    5,                // index
    (unsigned char *)&opt_alg.BACnet_enabled,       // option
    (unsigned char *)&option_index_0,// Optie index 
    lcd_bacnet_disp,  // display
    &disp_cursor_bacnet_device_id_0,                // cursor
    &key_bacnet_device_id_0_value,     // *value
    Dummy_Func,       // void (*number)(void); 
    Arrow_Bacnet_Device_Id_Value, // void (*arrow)(void); 
    Enter_Bacnet_Device_Id_Value, // void (*enter)(void);
  },  { // BACnet
    23,                // nr
    6,                // index
    (unsigned char *)&opt_alg.BACnet_enabled,       // option
    (unsigned char *)&option_index_0,// Optie index 
    lcd_bacnet_disp,  // display
    &disp_cursor_bacnet_network_nr_4,                // cursor
    &key_bacnet_network_nr_4_value,     // *value
    Dummy_Func,       // void (*number)(void); 
    Arrow_Bacnet_Network_Nr_Value, // void (*arrow)(void); 
    Enter_Bacnet_Network_Nr_Value, // void (*enter)(void);
  },
  { // BACnet
    23,                // nr
    7,                // index
    (unsigned char *)&opt_alg.BACnet_enabled,       // option
    (unsigned char *)&option_index_0,// Optie index 
    lcd_bacnet_disp,  // display
    &disp_cursor_bacnet_network_nr_3,                // cursor
    &key_bacnet_network_nr_3_value,     // *value
    Dummy_Func,       // void (*number)(void); 
    Arrow_Bacnet_Network_Nr_Value, // void (*arrow)(void); 
    Enter_Bacnet_Network_Nr_Value, // void (*enter)(void);
  },
  { // BACnet
    23,                // nr
    8,                // index
    (unsigned char *)&opt_alg.BACnet_enabled,       // option
    (unsigned char *)&option_index_0,// Optie index 
    lcd_bacnet_disp,  // display
    &disp_cursor_bacnet_network_nr_2,                // cursor
    &key_bacnet_network_nr_2_value,     // *value
    Dummy_Func,       // void (*number)(void); 
    Arrow_Bacnet_Network_Nr_Value, // void (*arrow)(void); 
    Enter_Bacnet_Network_Nr_Value, // void (*enter)(void);
  },
  { // BACnet
    23,                // nr
    9,                // index
    (unsigned char *)&opt_alg.BACnet_enabled,       // option
    (unsigned char *)&option_index_0,// Optie index 
    lcd_bacnet_disp,  // display
    &disp_cursor_bacnet_network_nr_1,                // cursor
    &key_bacnet_network_nr_1_value,     // *value
    Dummy_Func,       // void (*number)(void); 
    Arrow_Bacnet_Network_Nr_Value, // void (*arrow)(void); 
    Enter_Bacnet_Network_Nr_Value, // void (*enter)(void);
  },
  { // BACnet
    23,                // nr
    10,                // index
    (unsigned char *)&opt_alg.BACnet_enabled,       // option
    (unsigned char *)&option_index_0,// Optie index 
    lcd_bacnet_disp,  // display
    &disp_cursor_bacnet_network_nr_0,                // cursor
    &key_bacnet_network_nr_0_value,     // *value
    Dummy_Func,       // void (*number)(void); 
    Arrow_Bacnet_Network_Nr_Value, // void (*arrow)(void); 
    Enter_Bacnet_Network_Nr_Value, // void (*enter)(void);
  },

#endif // ETHERNET
//  { // Baudrate RS232
//    10,              // nr
//    0,               // index
//    (unsigned char *)&option_on,      // option
//    (unsigned char *)&option_index_0,   // Optie index 
//    lcd_RS232_baudrate_disp,  // display
//    0,               // cursor
//    &dummy_value,     // *value
//    Dummy_Func,      // void (*number)(void); 
//    Arrow_Option_Func, // void (*arrow)(void); 
//    Dummy_Func,      // void (*enter)(void);
//  },
//  {
//    10,              // nr
//    1,               // index
//    (unsigned char *)&option_on, // &opt_alg.s[0].vent_temp_curve,     // option
//    (unsigned char *)&option_index_0,   // Optie index 
//    lcd_RS232_baudrate_disp,     // display
//    &disp_cursor_192_78_8,   // cursor
//    &key_RS232_baudrate_value,     // *value
//    Dummy_Func,      // void (*number)(void); 
//    RS232_Arrow, // void (*arrow)(void); 
//    Increment_Func_Index,      // void (*enter)(void);
//  },
/*
/*
  { // Celsius / Farenheid
    9,                                // nr
    0,                                // index
    (unsigned char *)&option_on,      // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_cel_far_disp,                 // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Option_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    9,                                // nr
    1,                                // index
    (unsigned char *)&option_on,      // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_cel_far_disp,                 // display
    &disp_cursor_207_78_8,            // cursor
    &key_cel_far_value,               // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Cel_Far_Value,              // void (*arrow)(void); 
    Increment_Func_Index,             // void (*enter)(void);
  },
*/
  #ifdef PASSWORD
  { // Instalateurs code
    24,                             // nr
    0,                              // index
    &password_disp[0],              // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_beheerder_code_disp,        // display
    0,                              // cursor
    &dummy_value,                   // *value
    Dummy_Func,                     // void (*number)(void); 
    Arrow_Option_Func,              // void (*arrow)(void); 
    Dummy_Func,                     // void (*enter)(void);
  },
  {
    24,                             // nr
    1,                              // index
    &password_disp[0],              // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_beheerder_code_1_disp,      // display
    &disp_cursor_password_1,        // cursor
    &key_code_value,                // *value
    Number_Code,                    // void (*number)(void); 
    Arrow_Code,                     // void (*arrow)(void); 
    Enter_Code,                     // void (*enter)(void);
  },
  {
    24,                             // nr
    2,                              // index
    &password_disp[0],              // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_beheerder_code_2_disp,      // display
    &disp_cursor_password_2,        // cursor
    &key_bevestig_value,            // *value
    Number_Code,                    // void (*number)(void); 
    Arrow_Herhaal,                  // void (*arrow)(void); 
    Enter_Herhaal_Beheerder,        // void (*enter)(void);
  },
  {
    24,                             // nr
    3,                              // index
    &password_disp[0],              // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_beheerder_code_3_disp,      // display
    0,                              // cursor
    &dummy_value,                   // *value
    Dummy_Func,                     // void (*number)(void); 
    Increment_Func_Index,           // void (*arrow)(void); 
    Increment_Func_Index,           // void (*enter)(void);
  },
  { // Gerbruikers code
    25,                             // nr
    0,                              // index
    &password_disp[1],              // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_gebruiker_1_code_disp,      // display
    0,                              // cursor
    &dummy_value,                   // *value
    Dummy_Func,                     // void (*number)(void); 
    Arrow_Option_Func,              // void (*arrow)(void); 
    Dummy_Func,                     // void (*enter)(void);
  },
  {
    25,                             // nr
    1,                              // index
    &password_disp[1],              // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_gebruiker_1_code_1_disp,    // display
    &disp_cursor_password_1,        // cursor
    &key_code_value,                // *value
    Number_Code,                    // void (*number)(void); 
    Arrow_Code,                     // void (*arrow)(void); 
    Enter_Code,                     // void (*enter)(void);
  },
  {
    25,                             // nr
    2,                              // index
    &password_disp[1],              // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_gebruiker_1_code_2_disp,    // display
    &disp_cursor_password_2,        // cursor
    &key_bevestig_value,            // *value
    Number_Code,                    // void (*number)(void); 
    Arrow_Herhaal,                  // void (*arrow)(void); 
    Enter_Herhaal_Gebruiker_1,      // void (*enter)(void);
  },
  {
    25,                             // nr
    3,                              // index
    &password_disp[1],              // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_gebruiker_1_code_3_disp,    // display
    0,                              // cursor
    &dummy_value,                   // *value
    Dummy_Func,                     // void (*number)(void); 
    Increment_Func_Index,           // void (*arrow)(void); 
    Increment_Func_Index,           // void (*enter)(void);
  },


  { // Gerbruiker 1 Level
    26,                             // nr
    0,                              // index
    &password_level_disp[1],        // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_gebruiker_1_level_disp,     // display
    0,                              // cursor
    &dummy_value,                   // *value
    Dummy_Func,                     // void (*number)(void); 
    Arrow_Option_Func,              // void (*arrow)(void); 
    Dummy_Func,                     // void (*enter)(void);
  },
  { // Gerbruiker 1 Level
    26,                             // nr
    1,                              // index
    &password_level_disp[1],        // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_gebruiker_1_level_disp,     // display
    &disp_cursor_level,             // cursor
    &key_gebruiker_1_level_value,   // *value
    Number_Value,                   // void (*number)(void); 
    Arrow_Scroll_Option_Value,      // void (*arrow)(void); 
    Increment_Func_Index,           // void (*enter)(void);
  },

  { // Gerbruikers code
    27,                             // nr
    0,                              // index
    &password_disp[2],              // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_gebruiker_2_code_disp,      // display
    0,                              // cursor
    &dummy_value,                   // *value
    Dummy_Func,                     // void (*number)(void); 
    Arrow_Option_Func,              // void (*arrow)(void); 
    Dummy_Func,                     // void (*enter)(void);
  },
  {
    27,                             // nr
    1,                              // index
    &password_disp[2],              // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_gebruiker_2_code_1_disp,    // display
    &disp_cursor_password_1,        // cursor
    &key_code_value,                // *value
    Number_Code,                    // void (*number)(void); 
    Arrow_Code,                     // void (*arrow)(void); 
    Enter_Code,                     // void (*enter)(void);
  },
  {
    27,                             // nr
    2,                              // index
    &password_disp[2],              // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_gebruiker_2_code_2_disp,    // display
    &disp_cursor_password_2,        // cursor
    &key_bevestig_value,            // *value
    Number_Code,                    // void (*number)(void); 
    Arrow_Herhaal,                  // void (*arrow)(void); 
    Enter_Herhaal_Gebruiker_2,      // void (*enter)(void);
  },
  {
    27,                             // nr
    3,                              // index
    &password_disp[2],              // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_gebruiker_2_code_3_disp,    // display
    0,                              // cursor
    &dummy_value,                   // *value
    Dummy_Func,                     // void (*number)(void); 
    Increment_Func_Index,           // void (*arrow)(void); 
    Increment_Func_Index,           // void (*enter)(void);
  },

  { // Gerbruiker 2 Level
    28,                             // nr
    0,                              // index
    &password_level_disp[2],        // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_gebruiker_2_level_disp,     // display
    0,                              // cursor
    &dummy_value,                   // *value
    Dummy_Func,                     // void (*number)(void); 
    Arrow_Option_Func,              // void (*arrow)(void); 
    Dummy_Func,                     // void (*enter)(void);
  },
  { // Gerbruiker 2 Level
    28,                             // nr
    1,                              // index
    &password_level_disp[2],        // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_gebruiker_2_level_disp,     // display
    &disp_cursor_level,             // cursor
    &key_gebruiker_2_level_value,   // *value
    Number_Value,                   // void (*number)(void); 
    Arrow_Scroll_Option_Value,      // void (*arrow)(void); 
    Increment_Func_Index,           // void (*enter)(void);
  },

  { // Gerbruikers code
    29,                             // nr
    0,                              // index
    &password_disp[3],              // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_gebruiker_3_code_disp,      // display
    0,                              // cursor
    &dummy_value,                   // *value
    Dummy_Func,                     // void (*number)(void); 
    Arrow_Option_Func,              // void (*arrow)(void); 
    Dummy_Func,                     // void (*enter)(void);
  },
  {
    29,                             // nr
    1,                              // index
    &password_disp[3],              // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_gebruiker_3_code_1_disp,    // display
    &disp_cursor_password_1,        // cursor
    &key_code_value,                // *value
    Number_Code,                    // void (*number)(void); 
    Arrow_Code,                     // void (*arrow)(void); 
    Enter_Code,                     // void (*enter)(void);
  },
  {
    29,                             // nr
    2,                              // index
    &password_disp[3],              // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_gebruiker_3_code_2_disp,    // display
    &disp_cursor_password_2,        // cursor
    &key_bevestig_value,            // *value
    Number_Code,                    // void (*number)(void); 
    Arrow_Herhaal,                  // void (*arrow)(void); 
    Enter_Herhaal_Gebruiker_3,      // void (*enter)(void);
  },
  {
    29,                             // nr
    3,                              // index
    &password_disp[3],              // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_gebruiker_3_code_3_disp,    // display
    0,                              // cursor
    &dummy_value,                   // *value
    Dummy_Func,                     // void (*number)(void); 
    Increment_Func_Index,           // void (*arrow)(void); 
    Increment_Func_Index,           // void (*enter)(void);
  },

  { // Gerbruiker 3 Level
    30,                             // nr
    0,                              // index
    &password_level_disp[3],        // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_gebruiker_3_level_disp,     // display
    0,                              // cursor
    &dummy_value,                   // *value
    Dummy_Func,                     // void (*number)(void); 
    Arrow_Option_Func,              // void (*arrow)(void); 
    Dummy_Func,                     // void (*enter)(void);
  },
  { // Gerbruiker 3 Level
    30,                             // nr
    1,                              // index
    &password_level_disp[3],        // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_gebruiker_3_level_disp,     // display
    &disp_cursor_level,             // cursor
    &key_gebruiker_3_level_value,   // *value
    Number_Value,                   // void (*number)(void); 
    Arrow_Scroll_Option_Value,      // void (*arrow)(void); 
    Increment_Func_Index,           // void (*enter)(void);
  },

  { // Gerbruikers code
    31,                             // nr
    0,                              // index
    &password_disp[4],              // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_gebruiker_4_code_disp,      // display
    0,                              // cursor
    &dummy_value,                   // *value
    Dummy_Func,                     // void (*number)(void); 
    Arrow_Option_Func,              // void (*arrow)(void); 
    Dummy_Func,                     // void (*enter)(void);
  },
  {
    31,                             // nr
    1,                              // index
    &password_disp[4],              // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_gebruiker_4_code_1_disp,    // display
    &disp_cursor_password_1,        // cursor
    &key_code_value,                // *value
    Number_Code,                    // void (*number)(void); 
    Arrow_Code,                     // void (*arrow)(void); 
    Enter_Code,                     // void (*enter)(void);
  },
  {
    31,                             // nr
    2,                              // index
    &password_disp[4],              // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_gebruiker_4_code_2_disp,    // display
    &disp_cursor_password_2,        // cursor
    &key_bevestig_value,            // *value
    Number_Code,                    // void (*number)(void); 
    Arrow_Herhaal,                  // void (*arrow)(void); 
    Enter_Herhaal_Gebruiker_4,      // void (*enter)(void);
  },
  {
    31,                             // nr
    3,                              // index
    &password_disp[4],              // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_gebruiker_4_code_3_disp,    // display
    0,                              // cursor
    &dummy_value,                   // *value
    Dummy_Func,                     // void (*number)(void); 
    Increment_Func_Index,           // void (*arrow)(void); 
    Increment_Func_Index,           // void (*enter)(void);
  },

  { // Gerbruiker 4 Level
    32,                             // nr
    0,                              // index
    &password_level_disp[4],        // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_gebruiker_4_level_disp,     // display
    0,                              // cursor
    &dummy_value,                   // *value
    Dummy_Func,                     // void (*number)(void); 
    Arrow_Option_Func,              // void (*arrow)(void); 
    Dummy_Func,                     // void (*enter)(void);
  },
  { // Gerbruiker 4 Level
    32,                             // nr
    1,                              // index
    &password_level_disp[4],        // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_gebruiker_4_level_disp,     // display
    &disp_cursor_level,             // cursor
    &key_gebruiker_4_level_value,   // *value
    Number_Value,                   // void (*number)(void); 
    Arrow_Scroll_Option_Value,      // void (*arrow)(void); 
    Increment_Func_Index,           // void (*enter)(void);
  },
  #else // PASSWORD
  { // Gebruikers code
    24,                               // nr
    0,                                // index
    &password_enabled,                // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_user_code_disp,               // display
    0,                                // cursor
    &key_code_value,                  // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Option_Alg_Func,            // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    24,                               // nr
    1,                                // index
    &password_enabled,                // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_user_code_1_disp,             // display
    &disp_cursor_password_1,          // cursor
    &key_code_value,                  // *value
    Number_Code,                      // void (*number)(void); 
    Arrow_Code,                       // void (*arrow)(void); 
    Enter_Code,                       // void (*enter)(void);
  },
  {
    24,                               // nr
    2,                                // index
    &password_enabled,                // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_user_code_2_disp,             // display
    &disp_cursor_password_2,          // cursor
    &key_bevestig_value,              // *value
    Number_Code,                      // void (*number)(void); 
    Arrow_Herhaal,                    // void (*arrow)(void); 
    Enter_Herhaal_User,               // void (*enter)(void);
  },
  {
    24,                               // nr
    3,                                // index
    &password_enabled,                // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_user_code_3_disp,             // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Increment_Func_Index,             // void (*arrow)(void); 
    Increment_Func_Index,             // void (*enter)(void);
  },
  { // Instalateurs code
    25,                               // nr
    0,                                // index
    &password_installateur_enabled,   // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_instal_code_disp,             // display
    0,                                // cursor
    &key_code_value,                  // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Option_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    25,                               // nr
    1,                                // index
    &password_installateur_enabled,   // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_instal_code_1_disp,           // display
    &disp_cursor_password_1,          // cursor
    &key_code_value,                  // *value
    Number_Code,                      // void (*number)(void); 
    Arrow_Code,                       // void (*arrow)(void); 
    Enter_Code,                       // void (*enter)(void);
  },
  {
    25,                               // nr
    2,                                // index
    &password_installateur_enabled,   // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_instal_code_2_disp,           // display
    &disp_cursor_password_2,          // cursor
    &key_bevestig_value,              // *value
    Number_Code,                      // void (*number)(void); 
    Arrow_Herhaal,                    // void (*arrow)(void); 
    Enter_Herhaal_Instal,             // void (*enter)(void);
  },
  {
    25,                               // nr
    3,                                // index
    &password_installateur_enabled,   // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_instal_code_3_disp,           // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Increment_Func_Index,             // void (*arrow)(void); 
    Increment_Func_Index,             // void (*enter)(void);
  },
  #endif // PASSWORD
  { // PC code
    33,                               // nr
    0,                                // index
    &password_enabled,                // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_pc_code_disp,                 // display
    0,                                // cursor
    &key_code_value,                  // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Option_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    33,                               // nr
    1,                                // index
    &password_enabled,                // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_pc_code_1_disp,               // display
    &disp_cursor_password_1,          // cursor
    &key_code_value,                  // *value
    Number_Code,                      // void (*number)(void); 
    Arrow_Code,                       // void (*arrow)(void); 
    Enter_Code,                       // void (*enter)(void);
  },
  {
    33,                               // nr
    2,                                // index
    &password_enabled,                // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_pc_code_2_disp,               // display
    &disp_cursor_password_2,          // cursor
    &key_bevestig_value,              // *value
    Number_Code,                      // void (*number)(void); 
    Arrow_Herhaal,                    // void (*arrow)(void); 
    Enter_Herhaal_Pc,                 // void (*enter)(void);
  },
  {
    33,                               // nr
    3,                                // index
    &password_enabled,                // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_pc_code_3_disp,               // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Increment_Func_Index,             // void (*arrow)(void); 
    Increment_Func_Index,             // void (*enter)(void);
  },
  {
    99,                               // nr
    0,                                // index
    &end_flag,                        // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_end_disp,                     // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_End_Func,                   // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
};

//-----------------------------------------------------------------------------
s_screen screen_opt_alg_2;
s_screen const screen_opt_alg_2_default =
{
  0, // functie nr
  0, // index
  3, // max rel
  0, // rel actief
  0, // nr actief nummer weergegeven bin aantal ingeschakelde functie nummers
  0, // aantal ingeschakelde functie nummers
  lcd_disp_header, // algemene scherm opmaak
  {0,0,0,0,0,0}, // functies behorende bij functie toetsen
  0,3,19,       // rel[0]
  0,3,19+28,    // rel[1]
  0,3,19+28+28, // rel[2]
  &opt_alg_2_key_action[0], // first_action
  &opt_alg_2_key_action[sizeof(opt_alg_2_key_action)/sizeof(s_key_action) - 1], // last action 
  0, // change_flag
  0, // punt in normaal display
  0, // punt tijdens invoeren
  0, // aantal digits ingevoerd (punt telt niet mee)
  0, // value
  0, // min value
  0, // max value
  &screen_option_1,  // vorige scherm
  0  // prev_next_func
};

void Control_Options_Password(void)
{
#ifdef PASSWORD
int loop;

  for (loop = 0; loop < 5; loop++)
    password_disp[loop] = password_level_disp[loop] = 0;
  if (password_enabled & PASSWORD_ENABLED_OPT_MASK)
  {
    if (opt_alg.password_nummer[0])
    {
      if (((password_enabled & PASSWORD_GEBRUIKER_MASK) == PASSWORD_BEHEERDER) ||
          ((password_enabled & PASSWORD_GEBRUIKER_MASK) == PASSWORD_HOTRACO_ADM))
      {
        for (loop = 0; loop < 5; loop++)
        {
          password_disp[loop] = 1;
          if (opt_alg.password_nummer[loop])
            password_level_disp[loop] = 1;
        }   
      }  
      else if ((password_enabled & PASSWORD_GEBRUIKER_MASK) == PASSWORD_GEBRUIKER_1)
        password_disp[1] = 1;
      else if ((password_enabled & PASSWORD_GEBRUIKER_MASK) == PASSWORD_GEBRUIKER_2)
        password_disp[2] = 1;
      else if ((password_enabled & PASSWORD_GEBRUIKER_MASK) == PASSWORD_GEBRUIKER_3)
        password_disp[3] = 1;
      else if ((password_enabled & PASSWORD_GEBRUIKER_MASK) == PASSWORD_GEBRUIKER_4)
        password_disp[4] = 1;
    }
    else
      password_disp[0] = 1;  
  }
#endif // PASSWORD
}

void Control_Screen_Option_Algemeen_2(void)
// zorg voor een correcte inhoud in klimaat scherm 0
{
  if (password_enabled & PASSWORD_ENABLED_OPT_MASK)
    start_flag = end_flag = 1;
  else
    start_flag = end_flag = 0;
  tekst_taalkeuze[0] = &tekst_engels.Gekozen_Taal_14;
  tekst_taalkeuze[1] = &tekst_nederlands.Gekozen_Taal_14;
  tekst_taalkeuze[2] = &tekst_duits.Gekozen_Taal_14;
  tekst_taalkeuze[3] = &tekst_spaans.Gekozen_Taal_14;
  tekst_taalkeuze[4] = &eeprom_taal.Gekozen_Taal_14; // &tekst.Gekozen_Taal_14;
  aantal_talen = (EEP_taal_eprom_aanwezig && EEP_taal_tekst_aanwezig && EEP_taal_tekst_inst_aanwezig) ? 4 : 3;
  Control_Options_Password();
  Control_Screen(&screen_opt_alg_2, &screen_opt_alg_2_default, 1, 1);
  flag_opties_gewist = 0;
  flag_setpoints_gewist = 0;
}

//-----------------------------------------------------------------------------
static void Arrow_Data_Wissen(void)
{
  switch (key)
  {
    case UP:
      Decrement_Func();
      break;
    case DOWN:
      Increment_Func();
      break;
    case LEFT:
      if ((password_enabled & PASSWORD_ENABLED_OPT_MASK) == 0)
        Prev_Screen();
      break;
    case RIGHT:
      If_Exist_Goto_Screen_Opt_Alg_Wis_2();
      break;
  }
}
//-----------------------------------------------------------------------------
static void Arrow_Lcd_Angle_value(void) // void (*enter)(void);
{
  switch (key)
  {
    case LEFT:  Arrow_Left_Value(); break;
    case RIGHT: Increment_Func_Index(); break;
    case UP:    Increment_Option_Value(); break;
    case DOWN:  Decrement_Option_Value(); break;
  }
}
//-----------------------------------------------------------------------------
static void ArrowTaal(void)
{
  switch (key)
  {
    case LEFT:  break;
    case RIGHT: Increment_Func_Index(); Copy_Strings(); break;
    case UP:    Increment_Scroll_Option_Value(); break;
    case DOWN:  Decrement_Scroll_Option_Value(); break;
  }
}

static void EnterTaal(void)   // void (*enter)(void);
{
  Increment_Func_Index();
  Copy_Strings();
}

//-----------------------------------------------------------------------------
// niet aanroepen als oude nummer gelijk is aan nieuw nummer
static unsigned char Check_Computer_Nr(int nr)
{
  if (nr == 0)
    return(1);
  if (nr == opt_alg.adres)
    return (0);
  return(1);
} 

void Arrow_Computer_Nr(void)
{
long old_val = screen_ptr->value;

  switch (key)
  {
    case LEFT:  
      if (option_adres_changed)
      {
        option_adres_changed = 0;
        option_change_flag = 1;
        can_backbone_appl_init_switch = 1;

        if (opt_alg.BACnet_enabled)
          Bacnet_Reinit();
      }
      Decrement_Func_Index(); 
      break;
    case RIGHT:
      if (option_adres_changed)
      {
        option_adres_changed = 0;
        option_change_flag = 1;
        can_backbone_appl_init_switch = 1;

        if (opt_alg.BACnet_enabled)
          Bacnet_Reinit();
      }
      Increment_Func_Index(); 
      break;
    case UP:  
      do 
      {
        if (screen_ptr->value >= screen_ptr->max_value)
        {
          screen_ptr->value = old_val;
          break;
        }
        screen_ptr->value++;
      }
      while (Check_Computer_Nr(screen_ptr->value) == 0);
      if (old_val != screen_ptr->value)
      {
        option_adres_changed = 1;
        Put_Value();
      }
      break;
    case DOWN:
      do 
      {
        if (screen_ptr->value <= screen_ptr->min_value)
        {
          screen_ptr->value = old_val;
          break;
        }
        screen_ptr->value--;
      }
      while (Check_Computer_Nr(screen_ptr->value) == 0);
      if (old_val != screen_ptr->value)
      {
        option_adres_changed = 1;
        Put_Value();
      }
      break;
  }
}

void Enter_Computer_Nr(void)
{
int *int_ptr;
s_key_action const *key_action_ptr = screen_ptr->rel[screen_ptr->rel_actief].key_action;
s_key_value const *key_value_ptr = key_action_ptr->value;
e_type type = key_value_ptr->type;

  if (screen_ptr->change_flag)
  {
    if ((screen_ptr->value >= screen_ptr->min_value) && 
        (screen_ptr->value <= screen_ptr->max_value))
    {
      int_ptr = Get_Ptr(key_value_ptr->value, *key_action_ptr->option_index);
      if ((Return_Value(type, int_ptr /*Get_Ptr(key_value_ptr->value, *key_action_ptr->option_index)*/) != screen_ptr->value) && // nummer niet gewijzigd
          Check_Computer_Nr(screen_ptr->value))
      {
        // nieuw nummer goed gekeurd
        *int_ptr = screen_ptr->value;
        option_adres_changed = 1;
        Put_Value();
      }
    }
//  screen_ptr->change_flag = 0;
  }
  if (option_adres_changed)
  {
    option_adres_changed = 0;
    option_change_flag = 1;
    can_backbone_appl_init_switch = 1;

    if (opt_alg.BACnet_enabled)
      Bacnet_Reinit();
  }
  Increment_Func_Index();
}

//-----------------------------------------------------------------------------
static void Arrow_Can_Backbone(void)
{
  switch (key)
  {
    case LEFT:  
      if (option_adres_changed)
      {
        option_adres_changed = 0;
        if (opt_alg.can_backbone == 0)
        {
          #ifdef CAN_BACKBONE_PC_WARNING
          opt_alg.can_backbone_pc_warning[0] = 0;
          opt_alg.can_backbone_pc_warning[1] = 0;
          opt_alg.can_backbone_pc_warning[2] = 0;
          #else // CAN_BACKBONE_PC_WARNING
          opt_alg.can_rs232 = 0;
          #endif // CAN_BACKBONE_PC_WARNING
		  CheckOptions();
        }  
        option_change_flag = 1;
        can_backbone_appl_init_switch = 1;
      }
      Decrement_Func_Index(); 
      Refresh_Screen_Nr_Aantal();
      break;
    case RIGHT:
      if (option_adres_changed)
      {
        option_adres_changed = 0;
        if (opt_alg.can_backbone == 0)
        {
          #ifdef CAN_BACKBONE_PC_WARNING
          opt_alg.can_backbone_pc_warning[0] = 0;
          opt_alg.can_backbone_pc_warning[1] = 0;
          opt_alg.can_backbone_pc_warning[2] = 0;
          #else // CAN_BACKBONE_PC_WARNING
          opt_alg.can_rs232 = 0;
          #endif // CAN_BACKBONE_PC_WARNING
		  CheckOptions();
        }  
        option_change_flag = 1;
        can_backbone_appl_init_switch = 1;
      }
      Increment_Func_Index(); 
      Refresh_Screen_Nr_Aantal();
      break;
    case UP:    
    case DOWN:
      Increment_Scroll_Value();
      option_adres_changed = 1;
      break;
  }
}

static void Enter_Can_Backbone(void)
{
  if (option_adres_changed)
  {
    option_adres_changed = 0;
    if (opt_alg.can_backbone == 0)
    {
      #ifdef CAN_BACKBONE_PC_WARNING
      opt_alg.can_backbone_pc_warning[0] = 0;
      opt_alg.can_backbone_pc_warning[1] = 0;
      opt_alg.can_backbone_pc_warning[2] = 0;
      #else // CAN_BACKBONE_PC_WARNING
      opt_alg.can_rs232 = 0;
      #endif // CAN_BACKBONE_PC_WARNING
      CheckOptions();
    }  
    option_change_flag = 1;
    can_backbone_appl_init_switch = 1;
  }
  Increment_Func_Index();
  Refresh_Screen_Nr_Aantal();
}

//-----------------------------------------------------------------------------
static void Can_Baudrate_Up_Down(int up)
{
  if (screen_ptr->value == 20)
    screen_ptr->value = (up) ? 50 : 500;
  else if (screen_ptr->value == 50)
    screen_ptr->value = (up) ? 125 : 20;
  else if (screen_ptr->value == 125)
    screen_ptr->value = (up) ? 250 : 50;
  else if (screen_ptr->value == 250)
    screen_ptr->value = (up) ? 500 : 125;
  else if (screen_ptr->value == 500)
    screen_ptr->value = (up) ? 20 : 250;
  else
    screen_ptr->value = 50;
  opt_alg.CanBaudrate = screen_ptr->value;
  screen_ptr->change_flag = 1;
  option_change_flag = 1;
}

static void Arrow_Can_Baudrate(void)
{
  switch (key)
  {
    case UP:
      Can_Baudrate_Up_Down(1);
      break;
    case DOWN:
      Can_Baudrate_Up_Down(0);
      break;
    case RIGHT:
    case LEFT:
      can_backbone_appl_init_switch = 1;
      Increment_Func_Index();
      break;      
  }
}

static void Enter_Can_Baudrate(void)
{
  can_backbone_appl_init_switch = 1;
  Increment_Func_Index();
}

//-----------------------------------------------------------------------------
#ifdef CAN_BACKBONE_PC_WARNING
static void Enter_Can_PC_Enable_Value(void)
{
  if (screen_ptr->index == 3)
  {
    if (option_adres_changed)
    {
      option_adres_changed = 0;
      can_backbone_appl_init_switch = 1;
    }
  }
  Increment_Func_Index();
}

static void Arrow_Can_PC_Enable_Value(void)
{
  switch (key)
  {
    case UP:
    case DOWN:
      Increment_Scroll_Option_Value();
      option_adres_changed = 1;
      break;
    case RIGHT:
      Enter_Can_PC_Enable_Value();
      break;
    case LEFT:
      if (screen_ptr->index == 1)
      {
        if (option_adres_changed)
        {
          option_adres_changed = 0;
          can_backbone_appl_init_switch = 1;
        }
      }
      Decrement_Func_Index();
      break;      
  }
}
#else // CAN_BACKBONE_PC_WARNING

static void Arrow_Can_RS232(void)    
{
  Arrow_Can_Backbone();
}

static void Enter_Can_RS232(void)
{
  Enter_Can_Backbone();
}
#endif // CAN_BACKBONE_PC_WARNING

//-----------------------------------------------------------------------------
static void COM_Up_Down(int up)
{
  switch (screen_ptr->value)
  {
    default: screen_ptr->value = 96; break;
    case   96: screen_ptr->value = (up) ?  192 : 1152; break;
    case  192: screen_ptr->value = (up) ?  384 :   96; break;
    case  384: screen_ptr->value = (up) ?  576 :  192; break;
    case  576: screen_ptr->value = (up) ? 1152 :  384; break;
    case 1152: screen_ptr->value = (up) ?   96 :  576; break;
  }
}

static void Arrow_Com1_Baudrate_Value(void)
{
  switch (key)
  {
    case UP:
      COM_Up_Down(1);
      opt_alg.com1_bd = screen_ptr->value;
      asc0_init_switch = 1;
      screen_ptr->change_flag = 1;
      option_change_flag = 1;
      break;
    case DOWN:
      COM_Up_Down(0);
      opt_alg.com1_bd = screen_ptr->value;
      asc0_init_switch = 1;
      screen_ptr->change_flag = 1;
      option_change_flag = 1;
      break;
    case RIGHT:
    case LEFT:
      Increment_Func_Index();
      break;      
  }
}

static void Arrow_Com1_Enable_Value(void)
{
  switch (key)
  {
    case UP:
    case DOWN:
      Increment_Scroll_Option_Value();
      if (opt_alg.com1_enabled == 0)
      {
        opt_alg.com1_modem = 0;
      }
      Refresh_Screen_Nr_Aantal();
//      Control_Options();
      Refresh_Screen_Nr_Aantal();
      asc0_init_switch = 1;
      break;
    case RIGHT:
    case LEFT:
      Increment_Func_Index();
      break;      
  }
}

//-----------------------------------------------------------------------------
static void Arrow_Com1_Modem_Value(void)
{
  switch (key)
  {
    case UP:
    case DOWN:
      Increment_Scroll_Option_Value();
      Refresh_Screen_Nr_Aantal();
      asc0_init_switch = 1;
      break;
    case RIGHT:
    case LEFT:
      Increment_Func_Index();
      break;      
  }
}

static void Arrow_Com1_Modem_Answer_Value(void)
{
  switch (key)
  {
    case LEFT:  Arrow_Left_Value(); break;
    case RIGHT: Increment_Func_Index(); break;
    case UP:    
      Increment_Option_Value();
      asc0_modem_init_state = 0;
      asc0_modem_init = 1;
      break;
    case DOWN:
      Decrement_Option_Value();
      asc0_modem_init_state = 0;
      asc0_modem_init = 1;
      break;
  }
}

static void Enter_Com1_Modem_Answer_Value(void)
{
  if (screen_ptr->change_flag)
  {
    if (Enter_Value())
    {
      option_change_flag = 1;
      asc0_modem_init_state = 0;
      asc0_modem_init = 1;
    }  
  }
  Increment_Func_Index();
}

static void Arrow_Com2_Baudrate_Value(void)
{
  switch (key)
  {
    case UP:
      COM_Up_Down(1);
      opt_alg.com2_bd = screen_ptr->value;
      asc1_init_switch = 1;
      screen_ptr->change_flag = 1;
      option_change_flag = 1;
      break;
    case DOWN:
      COM_Up_Down(0);
      opt_alg.com2_bd = screen_ptr->value;
      asc1_init_switch = 1;
      screen_ptr->change_flag = 1;
      option_change_flag = 1;
      break;
    case RIGHT:
    case LEFT:
      Increment_Func_Index();
      break;      
  }
}

static void Arrow_Com2_Enable_Value(void)
{
  switch (key)
  {
    case UP:
    case DOWN:
      Increment_Scroll_Option_Value();
      if (opt_alg.com2_enabled == 0)
      {
        opt_alg.com2_modem = 0;
      }
      Refresh_Screen_Nr_Aantal();
//      Control_Options();
      Refresh_Screen_Nr_Aantal();
      asc1_init_switch = 1;
      break;
    case RIGHT:
    case LEFT:
      Increment_Func_Index();
      break;      
  }
}

static void Arrow_Com2_Modem_Value(void)
{
  switch (key)
  {
    case UP:
    case DOWN:
      Increment_Scroll_Option_Value();
      Refresh_Screen_Nr_Aantal();
      asc1_init_switch = 1;
      break;
    case RIGHT:
    case LEFT:
      Increment_Func_Index();
      break;      
  }
}

static void Arrow_Com2_Modem_Answer_Value(void)
{
  switch (key)
  {
    case LEFT:  Arrow_Left_Value(); break;
    case RIGHT: Increment_Func_Index(); break;
    case UP:    
      Increment_Option_Value();
      asc1_modem_init_state = 0;
      asc1_modem_init = 1;
      break;
    case DOWN:
      Decrement_Option_Value();
      asc1_modem_init_state = 0;
      asc1_modem_init = 1;
      break;
  }
}

static void Enter_Com2_Modem_Answer_Value(void)
{
  if (screen_ptr->change_flag)
  {
    if (Enter_Value())
    {
      option_change_flag = 1;
      asc1_modem_init_state = 0;
      asc1_modem_init = 1;
    }  
  }
  Increment_Func_Index();
}

#ifndef ETHERNET
static void Arrow_Option_Alg_Func(void)
{
  Arrow_Option_Func();
}
#endif // ETHERNET
#ifdef ETHERNET
void Fill_Ethernet_With_Opt_Alg_Ethernet(void)
{
unsigned char index_0, index_1, uc_help;
unsigned int ui_help;

  for (index_0 = 0; index_0 < 3; index_0++)
  {
    for (index_1 = 0; index_1 < 4; index_1++)
    {
      uc_help = opt_alg.ethernet[index_0][index_1];
      ethernet[index_0][index_1][0] = uc_help % 10;
      uc_help /= 10;
      ethernet[index_0][index_1][1] = uc_help % 10;
      uc_help /= 10;
      ethernet[index_0][index_1][2] = uc_help;
    }
  }
  ui_help = opt_alg.ethernet_port;
  ethernet_port[0] = ui_help % 10;
  ui_help /= 10;
  ethernet_port[1] = ui_help % 10;
  ui_help /= 10;
  ethernet_port[2] = ui_help % 10;
  ui_help /= 10;
  ethernet_port[3] = ui_help;
}

void Fill_Bacnet_With_Opt_Alg_Bacnet(void)
{
  unsigned long ui_help;

  ui_help = opt_alg.BACnet_Device_Id;
  bacnet_device_id[0] = ui_help % 10;
  ui_help /= 10;
  bacnet_device_id[1] = ui_help % 10;
  ui_help /= 10;
  bacnet_device_id[2] = ui_help % 10;
  ui_help /= 10;
  bacnet_device_id[3] = ui_help % 10;
  ui_help /= 10;
  bacnet_device_id[4] = ui_help;

  ui_help = opt_alg.BACnet_Network_Nr;
  bacnet_network_nr[0] = ui_help % 10;
  ui_help /= 10;
  bacnet_network_nr[1] = ui_help % 10;
  ui_help /= 10;
  bacnet_network_nr[2] = ui_help % 10;
  ui_help /= 10;
  bacnet_network_nr[3] = ui_help % 10;
  ui_help /= 10;
  bacnet_network_nr[4] = ui_help;
}

static void Arrow_Option_Alg_Func(void)
{
  Fill_Ethernet_With_Opt_Alg_Ethernet();
  Fill_Bacnet_With_Opt_Alg_Bacnet();
  Arrow_Option_Func();
}

static void Arrow_Ethernet_Enable_Value(void)
{
  switch (key)
  {
    case UP:
    case DOWN:
      Increment_Scroll_Option_Value();
//      NetworkDeviceInitState = 0; // init ethernet
      break;
    case RIGHT:
    case LEFT:
      Enter_Ethernet_Enable_Value();
      break;      
  }
}

static void Enter_Ethernet_Enable_Value(void)
{
  if (opt_alg.ethernet_enabled)
  {
    if (module.BACnet)
      opt_alg.BACnet_possible = 1;
  }
  else
  {
    Bacnet_Cleanup();
    opt_alg.BACnet_enabled = 0;
    opt_alg.BACnet_possible = 0;
  }
  CheckOptionsHoogendoorn();			 
  Increment_Func_Index();
  Refresh_Screen_Nr_Aantal();
}

static void Arrow_Ethernet_Func(void)
{
  ethernet_number_changed = 0;
  Arrow_Option_Func(); 
}

void Check_And_Fill_Ethernet_Port_Value(void)
// 0 if not correct new value
// 1 if correct new value
{
unsigned int help;

  ethernet_number_changed = 0;
  help = ethernet_port[3];
  help *= 10;
  help += ethernet_port[2];
  help *= 10;
  help += ethernet_port[1];
  help *= 10;
  help += ethernet_port[0];
  opt_alg.ethernet_port = help;
}  

static void Arrow_Ethernet_Port_Value(void)
{
unsigned char index = 3 - (screen_ptr->index - 1);

  switch (key)
  {
    case LEFT:
      if (screen_ptr->index == 1)
        Reset_NetworkDevice(); 
      if ((index == 3) && (ethernet_number_changed))
        Check_And_Fill_Ethernet_Port_Value();
      Decrement_Func_Index();
      break;
    case RIGHT: 
      if ((index == 0) && (ethernet_number_changed))
        Check_And_Fill_Ethernet_Port_Value();
      Increment_Func_Index();
      break;
    case UP:
      Increment_Scroll_Value();
      ethernet_number_changed = 1;
      break;
    case DOWN:
      Decrement_Scroll_Value();
      ethernet_number_changed = 1;
      break;
  }
}

static void Enter_Ethernet_Port_Value(void)
{
unsigned char index = 3 - (screen_ptr->index - 1);

  if (ethernet_number_changed == 0)
  {
    switch (index)
    {
      case 3: Increment_Func_Index();
      case 2: Increment_Func_Index();
      case 1: Increment_Func_Index();
      case 0: Increment_Func_Index();
    }
  }
  else
  {
    if (index == 0)
      Check_And_Fill_Ethernet_Port_Value();
    Increment_Func_Index();
  }  
}

unsigned char Check_And_Fill_Ethernet_Value(unsigned char index_0, unsigned char index_1)
// 0 if not correct new value
// 1 if correct new value
{
unsigned int help;

  ethernet_number_changed = 0;
  help = ethernet[index_0][index_1][2];
  help *= 10;
  help += ethernet[index_0][index_1][1];
  help *= 10;
  help += ethernet[index_0][index_1][0];
  if (help & 0xFF00) // getal groter dan 255
  {
    help = opt_alg.ethernet[index_0][index_1];
    ethernet[index_0][index_1][0] = help % 10;
    help /= 10;
    ethernet[index_0][index_1][1] = help % 10;
    help /= 10;
    ethernet[index_0][index_1][2] = help;
    return (0);
  }
  else
  {
    opt_alg.ethernet[index_0][index_1] = help;
    return (1);
  }  
}  

static void Arrow_Ethernet_Value(void)
{
unsigned char index_0 = (screen_ptr->index - 5) / 12; // Host = 0; Mask = 1; Gate = 2
unsigned char index_1 = ((screen_ptr->index - 5) % 12) / 3; //  
unsigned char index_2 = 2 - ((screen_ptr->index - 5) % 3);
//unsigned int help;

  switch (key)
  {
    case LEFT:
      if ((index_2 == 2) && 
          (ethernet_number_changed))
      {
        if (Check_And_Fill_Ethernet_Value(index_0, index_1))
          Decrement_Func_Index();
      }
      else
      {
        Decrement_Func_Index();
      }  
      break;
    case RIGHT: 
      if ((index_2 == 0) && 
          (index_1 == 3) &&
          (index_0 == 2))
        Reset_NetworkDevice(); 
      if ((index_2 == 0) &&
          (ethernet_number_changed))
      {
        if (Check_And_Fill_Ethernet_Value(index_0, index_1))
          Increment_Func_Index();
      }
      else
      {
        Increment_Func_Index();
      }  
      break;
    case UP:
      Increment_Scroll_Value();
      ethernet_number_changed = 1;
      break;
    case DOWN:
      Decrement_Scroll_Value();
      ethernet_number_changed = 1;
      break;
  }
}

static void Enter_Ethernet_Value(void)
{
unsigned char index_0 = (screen_ptr->index - 5) / 12; // Host = 0; Mask = 1; Gate = 2
unsigned char index_1 = ((screen_ptr->index - 5) % 12) / 3; //  
unsigned char index_2 = 2 - ((screen_ptr->index - 5) % 3);
//unsigned int help;

  if (ethernet_number_changed == 0)
  {
    if ((index_1 == 3) &&
        (index_0 == 2))
      Reset_NetworkDevice(); 
    switch (index_2)
    {
      case 2: Increment_Func_Index();
      case 1: Increment_Func_Index();
      case 0: Increment_Func_Index();
    }
  }
  else
  {
    if (index_2 == 0)
    {
      if ((index_1 == 3) &&
          (index_0 == 2))
        Reset_NetworkDevice(); 
      if (Check_And_Fill_Ethernet_Value(index_0, index_1))
        Increment_Func_Index();
    }
    else
    {
      Increment_Func_Index();
    }  
  }  
}

static void Arrow_Bacnet_Enable_Value(void)
{
  switch (key)
  {
    case UP:
    case DOWN:
      Increment_Scroll_Option_Value();
      Refresh_Screen_Nr_Aantal();
      break;
    case RIGHT:
    case LEFT:
      Increment_Func_Index();
      break;      
  }
}

static void Enter_Bacnet_Enable_Value(void)
{
  Bacnet_Reinit();
  Increment_Func_Index(); 
}

static void Arrow_Bacnet_Func(void)
{
  ethernet_number_changed = 0;
  Arrow_Option_Func(); 
}

void Check_And_Fill_Bacnet_Device_Id_Value(void)
{
  unsigned long help;
  help = bacnet_device_id[4];
  help *= 10;
  help += bacnet_device_id[3];
  help *= 10;
  help += bacnet_device_id[2];
  help *= 10;
  help += bacnet_device_id[1];
  help *= 10;
  help += bacnet_device_id[0];
  opt_alg.BACnet_Device_Id = help;
  Bacnet_Reinit();
}  

static void Arrow_Bacnet_Device_Id_Value(void)
{
  unsigned char index = 4 - ((screen_ptr->index - 1) % 5);

  switch (key)
  {
    case LEFT:
      if (screen_ptr->index == 1)
        Bacnet_Reinit(); 
      if (index == 4)
        Check_And_Fill_Bacnet_Device_Id_Value();
      Decrement_Func_Index();
      break;
    case RIGHT:
      if (index == 0)
        Check_And_Fill_Bacnet_Device_Id_Value();
      Increment_Func_Index();
      break;
    case UP:
      Increment_Scroll_Value();
      break;
    case DOWN:
      Decrement_Scroll_Value();
      break;
  }
}

static void Enter_Bacnet_Device_Id_Value(void)
{
  unsigned char index = 4 - ((screen_ptr->index - 1) % 5);
  switch (index)
  {
    case 4:
      Increment_Func_Index();
    case 3:
      Increment_Func_Index();
    case 2:
      Increment_Func_Index();
    case 1:
      Increment_Func_Index();
    case 0:
	  Check_And_Fill_Bacnet_Device_Id_Value();
      Increment_Func_Index();
  }
}

void Check_And_Fill_Bacnet_Network_Nr_Value(void)
{
  unsigned int help;
  help = bacnet_network_nr[4];
  help *= 10;
  help += bacnet_network_nr[3];
  help *= 10;
  help += bacnet_network_nr[2];
  help *= 10;
  help += bacnet_network_nr[1];
  help *= 10;
  help += bacnet_network_nr[0];
  if (help > 65534)
  {
    help = 65534;
	bacnet_network_nr[0] = '6';
	bacnet_network_nr[1] = '5';
	bacnet_network_nr[2] = '5';
	bacnet_network_nr[3] = '3';
	bacnet_network_nr[4] = '4';
  }
  opt_alg.BACnet_Network_Nr = help;
  Bacnet_Reinit();
}

static void Arrow_Bacnet_Network_Nr_Value(void)
{
  unsigned char index = 4 - ((screen_ptr->index - 1) % 5);
  switch (key)
  {
    case LEFT:
      if (screen_ptr->index == 1)
        Bacnet_Reinit(); 
      if (index == 4)
        Check_And_Fill_Bacnet_Network_Nr_Value();
      Decrement_Func_Index();
      break;
    case RIGHT: 
      if (index == 0)
        Check_And_Fill_Bacnet_Network_Nr_Value();
      Increment_Func_Index();
      break;
    case UP:
      Increment_Scroll_Value();
      break;
    case DOWN:
      Decrement_Scroll_Value();
      break;
  }
}

static void Enter_Bacnet_Network_Nr_Value(void)
{
  unsigned char index = 4 - ((screen_ptr->index - 1) % 5);
  switch (index)
  {
    case 4:
      Increment_Func_Index();
    case 3:
      Increment_Func_Index();
    case 2:
      Increment_Func_Index();
    case 1:
      Increment_Func_Index();
    case 0:
	  Check_And_Fill_Bacnet_Network_Nr_Value();
      Increment_Func_Index();
  }
}

#endif // ETHERNET
/*
static void RS232_Up_Down(int up)
{
  if (screen_ptr->value == 96)
    screen_ptr->value = (up) ? 192 : 384;
  else if (screen_ptr->value == 192)
    screen_ptr->value = (up) ? 384 :  96;
  else if (screen_ptr->value == 384)
    screen_ptr->value = (up) ? 96 : 192;
  else
    screen_ptr->value = 96;
  opt_alg.rs232 = screen_ptr->value;
  asc0_init_switch = 1;
  screen_ptr->change_flag = 1;
  option_change_flag = 1;
}

static void RS232_Arrow(void)
{
  switch (key)
  {
    case UP:
      RS232_Up_Down(1);
      break;
    case DOWN:
      RS232_Up_Down(0);
      break;
    case RIGHT:
    case LEFT:
      Increment_Func_Index();
      break;      
  }
}
*/
//-----------------------------------------------------------------------------
static void Arrow_Code(void)
{
  switch (key)
  {
    case LEFT:  
      if (Left_Value())
      {
        Decrement_Func_Index();
        eerstecode = 0;
      }
      break;
    case RIGHT:
      Decrement_Func_Index();
      eerstecode = 0;
      break;
    case UP:    break;
    case DOWN:  break;
  }
}
//-----------------------------------------------------------------------------
static void Enter_Code(void)
{
  if (screen_ptr->change_flag)
  {
    Increment_Func_Index_Enter_Value();
  }
}
//-----------------------------------------------------------------------------
static void Arrow_Herhaal(void)
{
  switch (key)
  {
    case LEFT:  
      if (Left_Value())
      {
        Decrement_Func_Index();
        eerstecode = 0;
      }
      break;
    case RIGHT:
      Increment_Func_Index();
      Increment_Func_Index();
      eerstecode = 0;
      break;
    case UP:    break;
    case DOWN:  break;
  }
}
//-----------------------------------------------------------------------------
#ifdef PASSWORD
static void Enter_Herhaal_Beheerder(void)
{
  Increment_Func_Index_Enter_Value();
  if (eerstecode == tweedecode)
  {
    opt_alg.password_nummer[0] = tweedecode;
    option_change_flag_alg = 1;
    if (opt_alg.password_nummer[0] == 0) 
    {
      password_disp[1] = password_disp[2] = password_disp[3] = password_disp[4] = 0;
      opt_alg.password_nummer[1] = opt_alg.password_nummer[2] = opt_alg.password_nummer[3] = opt_alg.password_nummer[4] = 0; 
      opt_alg.password_level[0] = opt_alg.password_level[1] = opt_alg.password_level[2] = opt_alg.password_level[3] = opt_alg.password_level[4] = 0; 
      password_level_disp[1] = password_level_disp[2] = password_level_disp[3] = password_level_disp[4] = 0;
    }
    else  
    {
      opt_alg.password_level[0] = PASSWORD_LEVEL_SETP_SYST_OPT;
      password_disp[1] = password_disp[2] = password_disp[3] = password_disp[4] = 1;
    }  
    Increment_Func_Index();
    Refresh_Screen_Nr_Aantal();
  }
  eerstecode = 0;
  tweedecode = 0;
}
//-----------------------------------------------------------------------------
static void Enter_Herhaal_Gebruiker(unsigned char index)
{
  Increment_Func_Index_Enter_Value();
  if (eerstecode == tweedecode)
  {
    opt_alg.password_nummer[index] = tweedecode;
    if (opt_alg.password_nummer[index])
    {
      if (opt_alg.password_level[index] == PASSWORD_LEVEL_GEEN)
        opt_alg.password_level[index] = PASSWORD_LEVEL_SETP;
    }
    else 
      opt_alg.password_level[index] = PASSWORD_LEVEL_GEEN;
    if (password_disp[0])
      password_level_disp[index] = opt_alg.password_nummer[index] ? 1 : 0;
    option_change_flag_alg = 1;
    Increment_Func_Index();
    Refresh_Screen_Nr_Aantal();
  }
  eerstecode = 0;
  tweedecode = 0;
}

static void Enter_Herhaal_Gebruiker_1(void)
{
  Enter_Herhaal_Gebruiker(1);
}
//-----------------------------------------------------------------------------
static void Enter_Herhaal_Gebruiker_2(void)
{
  Enter_Herhaal_Gebruiker(2);
}
//-----------------------------------------------------------------------------
static void Enter_Herhaal_Gebruiker_3(void)
{
  Enter_Herhaal_Gebruiker(3);
}
//-----------------------------------------------------------------------------
static void Enter_Herhaal_Gebruiker_4(void)
{
  Enter_Herhaal_Gebruiker(4);
}

static void Disp_Control_Gebruiker_Level(unsigned char level)
{
  switch (level)
  {
    default:
    case PASSWORD_LEVEL_GEEN:          disp_setpoint = 0; disp_setpoint_system = 0; disp_option = 0; break;
    case PASSWORD_LEVEL_SETP:          disp_setpoint = 1; disp_setpoint_system = 0; disp_option = 0; break; 
    case PASSWORD_LEVEL_SETP_SYST:     disp_setpoint = 1; disp_setpoint_system = 1; disp_option = 0; break;
    case PASSWORD_LEVEL_SETP_SYST_OPT: disp_setpoint = 1; disp_setpoint_system = 1; disp_option = 1; break;
  }
}
static void Disp_Control_Gebruiker_1_Level(void)
{
  Disp_Control_Gebruiker_Level(opt_alg.password_level[1]);
}

static void Disp_Control_Gebruiker_2_Level(void)
{
  Disp_Control_Gebruiker_Level(opt_alg.password_level[2]);
}

static void Disp_Control_Gebruiker_3_Level(void)
{
  Disp_Control_Gebruiker_Level(opt_alg.password_level[3]);
}

static void Disp_Control_Gebruiker_4_Level(void)
{
  Disp_Control_Gebruiker_Level(opt_alg.password_level[4]);
}

#else // PASSWORD
static void Enter_Herhaal_Instal(void)
{
  Increment_Func_Index_Enter_Value();
  if (eerstecode == tweedecode)
  {
    opt_alg.password_installateur = tweedecode;
    option_change_flag = 1;
    Increment_Func_Index();
  }
  eerstecode = 0;
  tweedecode = 0;
}
//-----------------------------------------------------------------------------
static void Enter_Herhaal_User(void)
{
  Increment_Func_Index_Enter_Value();
  if (eerstecode == tweedecode)
  {
    opt_alg.password_gebruiker = tweedecode;
    option_change_flag = 1;
    Increment_Func_Index();
  }
  eerstecode = 0;
  tweedecode = 0;
}
#endif // PASSWORD
//-----------------------------------------------------------------------------
static void Enter_Herhaal_Pc(void)
{
  Increment_Func_Index_Enter_Value();
  if (eerstecode == tweedecode)
  {
    opt_alg.password_pc = tweedecode;
    option_change_flag = 1;
    Increment_Func_Index();
  }
  eerstecode = 0;
  tweedecode = 0;
}

//-----------------------------------------------------------------------------
static void Arrow_End_Func(void)
{
  switch (key)
  {
    case UP:
      Decrement_Func();
      break;
    case DOWN:
      break;
    case LEFT:
      if (option_change_flag)
      {
        option_change_flag_alg = 1;
        option_change_flag = 0;
      }
      Can_Backbone_Address_Check();
      Prev_Screen();
      Refresh_Screen_Nr_Aantal();
      break;
    case RIGHT:
      break;
  }
}

void CheckOptionsHoogendoorn(void)
{
  opt_alg.hoogendoorn_possible = (module.Hoogendoorn && opt_alg.ethernet_enabled) ? 1 : 0;
  if (opt_alg.hoogendoorn_possible == 0)
	opt_alg.hoogendoorn_enabled = 0;	  
}

static void Arrow_Hoogendoorn_Enable_Value(void)
{
  switch (key)
  {
    case UP:
    case DOWN:
      Increment_Scroll_Option_Value();
      Refresh_Screen_Nr_Aantal();
      break;
    case RIGHT:
    case LEFT:
      Increment_Func_Index();
      break;      
  }
}

static void Enter_Hoogendoorn_Enable_Value(void)
{
  Increment_Func_Index(); 
}
