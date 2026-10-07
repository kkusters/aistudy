// CT_STRING.H

#ifndef _CT_STRING_H
#define _CT_STRING_H

#define LENGTH 20

typedef struct { unsigned char max_char; unsigned char max_bits; unsigned char font_type; char string[1]; } s_tekst_1;
typedef struct { unsigned char max_char; unsigned char max_bits; unsigned char font_type; char string[2]; } s_tekst_2;
typedef struct { unsigned char max_char; unsigned char max_bits; unsigned char font_type; char string[3]; } s_tekst_3;
typedef struct { unsigned char max_char; unsigned char max_bits; unsigned char font_type; char string[4]; } s_tekst_4;
typedef struct { unsigned char max_char; unsigned char max_bits; unsigned char font_type; char string[5]; } s_tekst_5;
typedef struct { unsigned char max_char; unsigned char max_bits; unsigned char font_type; char string[6]; } s_tekst_6;
typedef struct { unsigned char max_char; unsigned char max_bits; unsigned char font_type; char string[7]; } s_tekst_7;
typedef struct { unsigned char max_char; unsigned char max_bits; unsigned char font_type; char string[8]; } s_tekst_8;
typedef struct { unsigned char max_char; unsigned char max_bits; unsigned char font_type; char string[9]; } s_tekst_9;
typedef struct { unsigned char max_char; unsigned char max_bits; unsigned char font_type; char string[10]; } s_tekst_10;
typedef struct { unsigned char max_char; unsigned char max_bits; unsigned char font_type; char string[11]; } s_tekst_11;
typedef struct { unsigned char max_char; unsigned char max_bits; unsigned char font_type; char string[12]; } s_tekst_12;
typedef struct { unsigned char max_char; unsigned char max_bits; unsigned char font_type; char string[13]; } s_tekst_13;
typedef struct { unsigned char max_char; unsigned char max_bits; unsigned char font_type; char string[14]; } s_tekst_14;
typedef struct { unsigned char max_char; unsigned char max_bits; unsigned char font_type; char string[15]; } s_tekst_15;
typedef struct { unsigned char max_char; unsigned char max_bits; unsigned char font_type; char string[16]; } s_tekst_16;
typedef struct { unsigned char max_char; unsigned char max_bits; unsigned char font_type; char string[17]; } s_tekst_17;
typedef struct { unsigned char max_char; unsigned char max_bits; unsigned char font_type; char string[18]; } s_tekst_18;
typedef struct { unsigned char max_char; unsigned char max_bits; unsigned char font_type; char string[19]; } s_tekst_19;
typedef struct { unsigned char max_char; unsigned char max_bits; unsigned char font_type; char string[20]; } s_tekst_20;
typedef struct { unsigned char max_char; unsigned char max_bits; unsigned char font_type; char string[21]; } s_tekst_21;
typedef struct { unsigned char max_char; unsigned char max_bits; unsigned char font_type; char string[22]; } s_tekst_22;
typedef struct { unsigned char max_char; unsigned char max_bits; unsigned char font_type; char string[23]; } s_tekst_23;
typedef struct { unsigned char max_char; unsigned char max_bits; unsigned char font_type; char string[24]; } s_tekst_24;
typedef struct { unsigned char max_char; unsigned char max_bits; unsigned char font_type; char string[25]; } s_tekst_25;
typedef struct { unsigned char max_char; unsigned char max_bits; unsigned char font_type; char string[26]; } s_tekst_26;
typedef struct { unsigned char max_char; unsigned char max_bits; unsigned char font_type; char string[27]; } s_tekst_27;
typedef struct { unsigned char max_char; unsigned char max_bits; unsigned char font_type; char string[28]; } s_tekst_28;
typedef struct { unsigned char max_char; unsigned char max_bits; unsigned char font_type; char string[29]; } s_tekst_29;
typedef struct { unsigned char max_char; unsigned char max_bits; unsigned char font_type; char string[30]; } s_tekst_30;
typedef struct { unsigned char max_char; unsigned char max_bits; unsigned char font_type; char string[35]; } s_tekst_35;
typedef struct { unsigned char max_char; unsigned char max_bits; unsigned char font_type; char string[50]; } s_tekst_50;

typedef struct
{
  unsigned int area_size;
  unsigned int computer;
  unsigned int soort;
  unsigned int versie_tekst;

  s_tekst_15 Gekozen_Taal_14;

  s_tekst_25 Motorgroepen_10;

  s_tekst_18 Groep_1_14;
  s_tekst_18 Groep_2_14;
  s_tekst_18 Groep_3_14;
  s_tekst_18 Groep_4_14;
  s_tekst_18 Groep_5_14;
  s_tekst_18 Groep_6_14;
  s_tekst_18 Groep_7_14;
  s_tekst_18 Groep_8_14;
  s_tekst_18 Groep_9_14;
  s_tekst_18 Groep_10_14;
  s_tekst_18 Groep_11_14;
  s_tekst_18 Groep_12_14;
  s_tekst_18 Groep_13_14;
  s_tekst_18 Groep_14_14;
  s_tekst_18 Groep_15_14;
  s_tekst_18 Groep_16_14;
  s_tekst_18 Groep_17_14;
  s_tekst_18 Groep_18_14;
  s_tekst_18 Groep_19_14;
  s_tekst_18 Groep_20_14;
  s_tekst_18 Groep_21_14;
  s_tekst_18 Groep_22_14;
  s_tekst_18 Groep_23_14;
  s_tekst_18 Groep_24_14;
  s_tekst_18 Groep_25_14;
  s_tekst_18 Groep_26_14;
  s_tekst_18 Groep_27_14;
  s_tekst_18 Groep_28_14;
  s_tekst_18 Groep_29_14;
  s_tekst_18 Groep_30_14;
  s_tekst_18 Groep_31_14;
  s_tekst_18 Groep_32_14;

  s_tekst_12 EXT_OPEN_10;
  s_tekst_12 EXT_DICHT_10;
  s_tekst_12 EXT_STOP_10;
  s_tekst_7  OPEN_10;
  s_tekst_7  DICHT_10;
  s_tekst_7  STOP_10;
  s_tekst_7  HAND_10;
  s_tekst_7  AUTO_10;
  s_tekst_7  UIT_10;
  s_tekst_7  HAND_7;
  s_tekst_7  AUTO_7;
  s_tekst_7  UIT_7;

  s_tekst_15 Zondag_10;
  s_tekst_15 Maandag_10;
  s_tekst_15 Dinsdag_10;
  s_tekst_15 Woensdag_10;
  s_tekst_15 Donderdag_10;
  s_tekst_15 Vrijdag_10;
  s_tekst_15 Zaterdag_10;
  s_tekst_25 Tijd_en_Datum_14;

  s_tekst_7  Fn_10;
  s_tekst_7  Syst_10;
  s_tekst_7  Diag_10;

  s_tekst_18 Groep_1_10;
  s_tekst_18 Groep_2_10;
  s_tekst_18 Groep_3_10;
  s_tekst_18 Groep_4_10;
  s_tekst_18 Groep_5_10;
  s_tekst_18 Groep_6_10;
  s_tekst_18 Groep_7_10;
  s_tekst_18 Groep_8_10;
  s_tekst_18 Groep_9_10;
  s_tekst_18 Groep_10_10;
  s_tekst_18 Groep_11_10;
  s_tekst_18 Groep_12_10;
  s_tekst_18 Groep_13_10;
  s_tekst_18 Groep_14_10;
  s_tekst_18 Groep_15_10;
  s_tekst_18 Groep_16_10;
  s_tekst_18 Groep_17_10;
  s_tekst_18 Groep_18_10;
  s_tekst_18 Groep_19_10;
  s_tekst_18 Groep_20_10;
  s_tekst_18 Groep_21_10;
  s_tekst_18 Groep_22_10;
  s_tekst_18 Groep_23_10;
  s_tekst_18 Groep_24_10;
  s_tekst_18 Groep_25_10;
  s_tekst_18 Groep_26_10;
  s_tekst_18 Groep_27_10;
  s_tekst_18 Groep_28_10;
  s_tekst_18 Groep_29_10;
  s_tekst_18 Groep_30_10;
  s_tekst_18 Groep_31_10;
  s_tekst_18 Groep_32_10;

  s_tekst_18 Motor_1_10;
  s_tekst_18 Motor_2_10;
  s_tekst_18 Motor_3_10;
  s_tekst_18 Motor_4_10;
  s_tekst_18 Motor_5_10;
  s_tekst_18 Motor_6_10;
  s_tekst_18 Motor_7_10;
  s_tekst_18 Motor_8_10;
  s_tekst_18 Motor_9_10;
  s_tekst_18 Motor_10_10;
  s_tekst_18 Motor_11_10;
  s_tekst_18 Motor_12_10;
  s_tekst_18 Motor_13_10;
  s_tekst_18 Motor_14_10;
  s_tekst_18 Motor_15_10;
  s_tekst_18 Motor_16_10;
  s_tekst_18 Motor_17_10;
  s_tekst_18 Motor_18_10;
  s_tekst_18 Motor_19_10;
  s_tekst_18 Motor_20_10;
  s_tekst_18 Motor_21_10;
  s_tekst_18 Motor_22_10;
  s_tekst_18 Motor_23_10;
  s_tekst_18 Motor_24_10;
  s_tekst_18 Motor_25_10;
  s_tekst_18 Motor_26_10;
  s_tekst_18 Motor_27_10;
  s_tekst_18 Motor_28_10;
  s_tekst_18 Motor_29_10;
  s_tekst_18 Motor_30_10;
  s_tekst_18 Motor_31_10;
  s_tekst_18 Motor_32_10;
  s_tekst_18 Motor_33_10;
  s_tekst_18 Motor_34_10;
  s_tekst_18 Motor_35_10;
  s_tekst_18 Motor_36_10;
  s_tekst_18 Motor_37_10;
  s_tekst_18 Motor_38_10;
  s_tekst_18 Motor_39_10;
  s_tekst_18 Motor_40_10;
  s_tekst_18 Motor_41_10;
  s_tekst_18 Motor_42_10;
  s_tekst_18 Motor_43_10;
  s_tekst_18 Motor_44_10;
  s_tekst_18 Motor_45_10;
  s_tekst_18 Motor_46_10;
  s_tekst_18 Motor_47_10;
  s_tekst_18 Motor_48_10;
  s_tekst_18 Motor_49_10;
  s_tekst_18 Motor_50_10;
  s_tekst_18 Motor_51_10;
  s_tekst_18 Motor_52_10;
  s_tekst_18 Motor_53_10;
  s_tekst_18 Motor_54_10;
  s_tekst_18 Motor_55_10;
  s_tekst_18 Motor_56_10;
  s_tekst_18 Motor_57_10;
  s_tekst_18 Motor_58_10;
  s_tekst_18 Motor_59_10;
  s_tekst_18 Motor_60_10;
  s_tekst_18 Motor_61_10;
  s_tekst_18 Motor_62_10;
  s_tekst_18 Motor_63_10;
  s_tekst_18 Motor_64_10;

  s_tekst_18 Status_10;
  s_tekst_18 Bediening_10;
  s_tekst_18 Positie_10;

  s_tekst_18 Alarm_HAND_10;
  s_tekst_18 Alarm_afwijking_10;
  s_tekst_18 Alarm_urgent_10;
  s_tekst_18 Pulse_zone_10;
  s_tekst_18 Pulse_width_10;
  s_tekst_18 Cycletime_10;
  s_tekst_18 Hysterese_10;
  s_tekst_18 Stapgrootte_10;
  s_tekst_18 Bandbreedte_10;

  s_tekst_23 Twee_doeken_een_bed_1_10;
  s_tekst_23 Twee_doeken_een_bed_2_10;
  s_tekst_23 Twee_doeken_een_bed_3_10;
  s_tekst_23 Twee_doeken_een_bed_4_10;
  s_tekst_23 Twee_doeken_een_bed_5_10;
  s_tekst_23 Twee_doeken_een_bed_6_10;
  s_tekst_23 Twee_doeken_een_bed_7_10;
  s_tekst_23 Twee_doeken_een_bed_8_10;

  s_tekst_18 Kier_10;
  s_tekst_18 Hysteresis_10;
  s_tekst_18 Standby_10;
  s_tekst_18 Voorloop_10;

  s_tekst_18 Time_10;
  s_tekst_18 Tijd_10;
  s_tekst_12 Datum_10;
  s_tekst_21 Sync_Tijd_10;
  s_tekst_21 Dagenteller_10;

  s_tekst_25 F2_10;

  s_tekst_25 F3_10;

  s_tekst_25 Alarmen_10;
  s_tekst_25 Alarmen_Actief_10;
  s_tekst_25 Alarmen_Historie_10;
  s_tekst_25 Alarmen_Wissen_10;
  s_tekst_25 Alarm_10;
  s_tekst_25 Waarschuwing_10;
  s_tekst_25 Alarm_Systeem_10; 
  s_tekst_25 Waarschuwing_Syst_10; 
  s_tekst_10 Alarm_Computer_10;
  s_tekst_25 Geen_Actief_Alarm_10;
  s_tekst_25 Onbekend_Alarm_10;
  s_tekst_25 Opties_10;
  s_tekst_25 Instellingen_10;
  s_tekst_25 Opties_En_Instellingen_10;
  s_tekst_25 Minimum_Maximum_10;
  s_tekst_25 Gewist_10;
  s_tekst_25 Terug_Gezet_10;
  s_tekst_25 Niet_Terug_Gezet_10;
  s_tekst_25 I2C_10;
  s_tekst_25 EEPROM_10;
  s_tekst_25 EEPROM_taal_10;
  s_tekst_25 RTC_10;
  s_tekst_25 TIMER_10;
  s_tekst_25 HTRAP_10;
  s_tekst_25 PLL_10;
  s_tekst_25 Opnieuw_Gestart_10;
  s_tekst_10 Print_10;
  s_tekst_25 Niet_Gevonden_10;
  s_tekst_25 ADC_10;
  s_tekst_20 Analoge_Ingang_10;
  s_tekst_20 Externe_24V_10;
  s_tekst_25 Orion_Uitgeschakeld_10;
  s_tekst_25 Geen_Minimum_Alarm_10;
  s_tekst_10 Offline_10; 
  s_tekst_10 Slave_10;

  s_tekst_18 Diagnose_10;

  s_tekst_21 Info_10;
  s_tekst_15 Serie_Nr_14;
  s_tekst_15 Computer_10;
  s_tekst_15 Computer_14;

  s_tekst_18 Draaiuren_10;
  s_tekst_18 Schakelingen_10;
  s_tekst_18 Storingen_10;
  s_tekst_18 Looptijd_10;
  s_tekst_18 Alarm_Code_10;

  s_tekst_25 Bewerken_Tekst_10;

  s_tekst_3  kg_10; // eenheid kilogram
  s_tekst_3  kg_14; // eenheid kilogram
  s_tekst_3  l_10; // eenheid liter
  s_tekst_3  l_14; // eenheid liter
  s_tekst_3  g_10; // eenheid gram
  s_tekst_3  ml_10; // eenheid mili liter
  s_tekst_3  m_10; // eenheid minuten
  s_tekst_3  s_10;
  s_tekst_6  AAN_20;
  s_tekst_6  UIT_20;
  s_tekst_6  Ana_10;
  s_tekst_6  Dig_10;
  s_tekst_6  MC_10;
  s_tekst_25 Geen_IO_Toegewezen_10;

  s_tekst_13 Password_10;
  s_tekst_13 Password_14;
  s_tekst_20 Zeker_weten_7;
  s_tekst_9  Wissen_10;

  s_tekst_25 CAN_LOCAL_14;
  s_tekst_25 Diag_CAN_LOCAL_10;
  s_tekst_12 TXOK_10;
  s_tekst_12 RXOK_10;
  s_tekst_12 TXOK_RXOK_10;
  s_tekst_23 BOFF_10;
  s_tekst_23 EWRN_10;
  s_tekst_23 Stuff_error_10;
  s_tekst_23 Form_error_10;
  s_tekst_23 Ack_error_10;
  s_tekst_23 Bit1_error_10;
  s_tekst_23 Bit0_busoff_error_10;
  s_tekst_23 Bit0_normal_error_10;
  s_tekst_23 Crc_error_10;
  s_tekst_3 perc_7;
  s_tekst_3 Pa_7;
  s_tekst_3 g_14;
  s_tekst_3 ml_14;
  s_tekst_3 graden_celsius_7;
  s_tekst_3 graden_fahrenheid_7;
  s_tekst_3 graden_7;
  s_tekst_4 minuut_7;

  s_tekst_25 Geen_Alarm_Contact_10;

  s_tekst_21 Handbediening_10;
  s_tekst_21 Emergency_switch_10;
  s_tekst_25 Thermal_failure_close_10;
  s_tekst_25 Thermal_failure_open_10;
  s_tekst_21 Break_input_10;
  s_tekst_21 Speed_to_low_10;
  s_tekst_21 Encoder_failure_10;
  s_tekst_26 Encoder_failure_A_10;
  s_tekst_26 Encoder_failure_B_10;
  s_tekst_21 No_feedback_10;
  s_tekst_21 Not_enough_pulses_10;
  s_tekst_21 Pulses_to_fast_10;
  s_tekst_21 Not_installed_10;
  s_tekst_23 Encoder_interference_10;
  s_tekst_25 Dualscreen_not_possible_10;
  s_tekst_21 Installation_mode_10;
  s_tekst_21 Deviation_position_10;
  s_tekst_23 Sensor_hi_speed_10;
  s_tekst_23 Direction_not_defined_10;
  s_tekst_28 Limitswitches_not_equal_10;
  s_tekst_21 Speed_not_equal_10;
  s_tekst_21 Multiple_master_10;
  s_tekst_21 Frequency_controller_10;
  s_tekst_23 Gelijkloop_beveiliging_10;
  s_tekst_28 Limitswitch_not_reached_10;
  s_tekst_23 Wrong_direction_10;
  s_tekst_25 Link_unknown_10;
  s_tekst_21 Motor_not_running_10;
  s_tekst_25 Position_not_reached_10;

  s_tekst_25 Locked_motor_10;
  s_tekst_25 Hall_failure_10;
  s_tekst_25 Thermal_motor_10;
  s_tekst_25 Comm_error_master_slave_10;
  s_tekst_25 Thermal_power_module_10;
  s_tekst_25 Comm_error_remote_unit_10;
  s_tekst_25 Phase_failure_10;
  s_tekst_25 Break_10;
  s_tekst_25 Low_line_voltage_10;
  s_tekst_25 Low_DC_link_voltage_10;
  s_tekst_25 High_DC_link_voltage_10;
  s_tekst_25 Driver_problem_10;
  s_tekst_25 Electronic_box_over_heat_10;
  s_tekst_26 Excessive_DC_link_current_10;

  s_tekst_21 Vorstbeveiliging_10;
  s_tekst_23 Algemeen_extern_alarm_10;
  s_tekst_23 Drukverschil_bewaking_10;

  s_tekst_6  JA_10;
  s_tekst_6  NEE_10;

  s_tekst_21 CAN_RS232_10;

  s_tekst_28 CAN_BACKBONE_14;
  s_tekst_28 Diag_CAN_BACKBONE_10;

  s_tekst_21 Wijzig_computer_type_14;
  s_tekst_21 Activeer_module_7;
  s_tekst_21 Deactiveer_module_7;
  s_tekst_18 Ongeldige_code_7;

  s_tekst_10 SD_CARD_10;
  s_tekst_28 SD_CARD_14;
  s_tekst_28 Diag_SD_CARD_10;
  s_tekst_12 no_card_10;
  s_tekst_12 placed_10;
  s_tekst_12 read_write_10;
  s_tekst_12 read_only_10;
  s_tekst_12 active_10;
  s_tekst_12 ask_remove_10;
  s_tekst_12 remove_10;
  s_tekst_12 SIZE_7;

  s_tekst_28 Diag_COM1_USB_10;
  s_tekst_28 COM1_USB_10;
  s_tekst_4  kBd_14;
  s_tekst_12 TX_10;
  s_tekst_12 RX_10;
  s_tekst_12 TX_RX_10;
  s_tekst_12 AT_10;
  s_tekst_12 Modem_Init_10;

  s_tekst_28 Diag_COM2_10;
  s_tekst_28 COM2_10;
  s_tekst_12 Modem_DCD_10;

  s_tekst_28 Diag_Ethernet_10;
  s_tekst_9  IP_10;
  s_tekst_9  Mask_10;
  s_tekst_9  Gate_10;
  s_tekst_9  MAC_10;
  s_tekst_9  PC_10;
  s_tekst_16 NO_CONNECTION_14;
  s_tekst_12 RTX_10;
  s_tekst_12 NO_ACK_10;
  s_tekst_12 CONNECT_10;
  s_tekst_23 Reset_Ethernet_10;
  s_tekst_9  Port_10;

  s_tekst_28 COM1_USB_14;
  s_tekst_28 COM2_14;
  s_tekst_28 Ethernet_14;

  s_tekst_18 Ventilator_10;
  s_tekst_18 Minimum_10;
  s_tekst_18 Maximum_10;
  s_tekst_18 Offset_10;

  s_tekst_18 Maximum_rpm_10;
  s_tekst_18 Vermogen_10;
  s_tekst_18 Temp_motor_10;
  s_tekst_18 Temp_electronica_10;
  s_tekst_18 Temp_power_module_10;
  s_tekst_23 Fabrieksinst_terugz_10;
  s_tekst_12 Versie_10;

  s_tekst_18 Reset_all_10;

  s_tekst_3  V_10;
  s_tekst_3  N_10;
  s_tekst_25 Geheugen_256k_10;	  
  s_tekst_21 Ongeldige_update_10;
  s_tekst_21 Plaats_512k_EEPROM_10;
  s_tekst_21 OK_is_opties_wissen_10;

  s_tekst_21 Luchtmengkast_14;
  s_tekst_21 Luchtmengkast_groep_14;
  s_tekst_21 Luchtmengkast_10;
  s_tekst_21 Luchtmengkast_groep_10;
  s_tekst_21 Diag_Luchtmengkast_10;
  s_tekst_21 Fn_Luchtmengkast_10;
  s_tekst_25 Syst_Luchtmengkast_10;

  s_tekst_21 Binnenklep_10;
  s_tekst_21 Buitenklep_10;
  s_tekst_21 Recirculatieklep_10;
  s_tekst_21 Bovenklep_10;
  s_tekst_21 Verwarming_10;
  s_tekst_21 Inblaasvent_10;
  s_tekst_21 Afblaasvent_10;
  s_tekst_10 ebm_10;

  s_tekst_18 Afblaasvent_aan_10;
  s_tekst_18 Afblaasvent_uit_10;
  s_tekst_18 Bovenklep_open_10;
  s_tekst_18 Bovenklep_dicht_10;

  //#ifdef CAN_BACKBONE_PC_WARNING
  s_tekst_21 CAN_PC_1_Offline_10;
  s_tekst_21 CAN_PC_2_Offline_10;
  s_tekst_21 CAN_PC_3_Offline_10;
  //#endif // CAN_BACKBONE_PC_WARNING
  s_tekst_15 Display_14;
  s_tekst_10 Groen_14;
  s_tekst_10 Wit_14;

  s_tekst_10 Modules_14;

  s_tekst_25 General_Error_10;
  s_tekst_25 Motor_Fault_10;
  s_tekst_25 Motor_Blocked_10;
  s_tekst_25 Heat_Sink_Temperature_10;
  s_tekst_25 Ground_Fault_10;
  s_tekst_25 Hall_IC_Fault_10;
  s_tekst_25 Overcurrent_10;
  s_tekst_25 Line_Fault_10;
  s_tekst_25 Int_Heat_Sink_Sensor_10;
  s_tekst_25 DC_Res_Voltage_To_High_10;
  s_tekst_25 Temperature_Lowering_10;
  s_tekst_25 Wrong_Connection_10;
  s_tekst_25 External_Fault_10;
  s_tekst_25 Factory_Settings_10;
  s_tekst_25 EEP_Error_10;
  s_tekst_25 RTC_General_Fault_10;
  s_tekst_25 RTC_Voltage_Fault_10;
  s_tekst_25 Filter_Contamination_10;
  s_tekst_25 Transfer_Error_10;
  s_tekst_25 Data_Connetion_Line_10;
  s_tekst_25 Data_Connection_Checksum_10;
  s_tekst_25 Sensor_Fault_Input_1_10;
  s_tekst_25 Sensor_Fault_Input_2_10;
  s_tekst_25 Sensor_Fault_Input_3_10;
  s_tekst_25 High_line_voltage_10;
  s_tekst_28 i_limit_10;
  s_tekst_28 p_limit_10;
  s_tekst_50 te_high_10;
  s_tekst_25 tm_high_10;
  s_tekst_50 tei_high_10;
  s_tekst_25 uz_low_10;
  s_tekst_50 n_low_10;

  s_tekst_25 igbt_fault_10;
  s_tekst_25 uzk_hi_10;
  s_tekst_25 uzk_lo_10;
  s_tekst_25 uin_hi_10;
  s_tekst_25 uin_lo_10;

  s_tekst_18 Klep_10;
  s_tekst_18 Sensor_10;
  s_tekst_18 Drukverschil_1_10;
  s_tekst_18 Drukverschil_2_10;
  s_tekst_18 Drukverschil_3_10;
  s_tekst_18 Drukverschil_4_10;
  s_tekst_18 Drukverschil_5_10;
  s_tekst_18 Drukverschil_6_10;
  s_tekst_18 Drukverschil_7_10;
  s_tekst_18 Drukverschil_8_10;
  s_tekst_18 Drukverschil_9_10;
  s_tekst_18 Drukverschil_10_10;
  s_tekst_18 Drukverschil_11_10;
  s_tekst_18 Drukverschil_12_10;
  s_tekst_18 Drukverschil_13_10;
  s_tekst_18 Drukverschil_14_10;
  s_tekst_18 Drukverschil_15_10;
  s_tekst_18 Drukverschil_16_10;

  s_tekst_18 Extern_alarm_10;

  s_tekst_25 failure_power_section_10;
  s_tekst_18 umax_10;
  s_tekst_18 umin_10;
  s_tekst_18 overspeed_10;
  s_tekst_18 locked_rotor_10;

  s_tekst_25 EEPROM_256k_10;
  s_tekst_25 Version_Error_10;

  s_tekst_18 underspeed_10;
  s_tekst_25 _24V_supply_overload_10;
  s_tekst_25 input_phase_error_10;
  s_tekst_25 motor_phase_error_10;
  s_tekst_18 memory_error_10;
  s_tekst_18 short_circuit_10;  
  s_tekst_25 loss_of_synchronism_10;
  s_tekst_25 input_voltage_error_10;
  s_tekst_25 input_relay_not_closed_10; 
  s_tekst_25 high_starting_current_10; 

} s_tekst;

typedef struct
{
  unsigned int area_size;
  unsigned int computer;
  unsigned int soort;
  unsigned int versie_tekst_inst;

  s_tekst_15 Gekozen_Taal_14;

  s_tekst_15 Opties_10;
  s_tekst_25 Bekijken_Opties_14;
  s_tekst_25 Wijzigen_Opties_14;
  s_tekst_25 Orion_Ingeschakeld_14;
  s_tekst_25 Orion_Uitgeschakeld_14;

  s_tekst_25 Algemeen_14;
  s_tekst_25 IO_14;
  s_tekst_25 Motorgroepen_14;
  s_tekst_18 Groep_1_14;
  s_tekst_18 Groep_2_14;
  s_tekst_18 Groep_3_14;
  s_tekst_18 Groep_4_14;
  s_tekst_18 Groep_5_14;
  s_tekst_18 Groep_6_14;
  s_tekst_18 Groep_7_14;
  s_tekst_18 Groep_8_14;
  s_tekst_18 Groep_9_14;
  s_tekst_18 Groep_10_14;
  s_tekst_18 Groep_11_14;
  s_tekst_18 Groep_12_14;
  s_tekst_18 Groep_13_14;
  s_tekst_18 Groep_14_14;
  s_tekst_18 Groep_15_14;
  s_tekst_18 Groep_16_14;
  s_tekst_18 Groep_17_14;
  s_tekst_18 Groep_18_14;
  s_tekst_18 Groep_19_14;
  s_tekst_18 Groep_20_14;
  s_tekst_18 Groep_21_14;
  s_tekst_18 Groep_22_14;
  s_tekst_18 Groep_23_14;
  s_tekst_18 Groep_24_14;
  s_tekst_18 Groep_25_14;
  s_tekst_18 Groep_26_14;
  s_tekst_18 Groep_27_14;
  s_tekst_18 Groep_28_14;
  s_tekst_18 Groep_29_14;
  s_tekst_18 Groep_30_14;
  s_tekst_18 Groep_31_14;
  s_tekst_18 Groep_32_14;
  s_tekst_25 Twee_doek_een_bed_14;
  s_tekst_25 Cabriokas_14;

  s_tekst_25 Opties_Doorlopen_14;
  s_tekst_25 Opties_OK_14;

  s_tekst_25 Opties_algemeen_10;
  s_tekst_21 Taal_14;
  s_tekst_21 Opties_Wissen_14;
  s_tekst_25 Opties_gewist_14;
  s_tekst_21 Setpoints_Wissen_14;
  s_tekst_25 Setpoints_gewist_14;
  s_tekst_21 Helderheid_14;
  s_tekst_21 LCD_dimmen_14;
  s_tekst_21 Computer_14;
  s_tekst_18 Nummer_14;
  s_tekst_18 Adres_14;
  s_tekst_18 CAN_BACKBONE_14;
  s_tekst_18 CAN_RS232_14;
  s_tekst_18 Waarschuwing_14;
  s_tekst_21 RS232_14;
  s_tekst_4  kBd_14;
  s_tekst_21 Installateurs_14;
  s_tekst_21 Gebruikers_14;
  s_tekst_21 PC_14; 
  s_tekst_15 Wachtwoord_14;
  s_tekst_15 Herhaal_14;
  s_tekst_21 Wachtwoord_ongelijk_7;

  s_tekst_27 Opties_IO_10;
  s_tekst_11 Versie_7;

  s_tekst_11 Opties_bord_10; // (hierachter wordt IO bordt weergegeven)
  s_tekst_25 Nummer_IO_Module_14;
  s_tekst_15 Verwijderen_14;
  s_tekst_20 Analoge_Ingang_14;
  s_tekst_20 Digitale_Ingang_14;
  s_tekst_20 Analoge_Uitgang_14;
  s_tekst_20 Digitale_Uitgang_14;
  s_tekst_20 Motor_Control_14;
  s_tekst_20 Instellen_sensor_14;
  s_tekst_20 IJken_sensor_14;
  s_tekst_20 IJken_Minimum_14;
  s_tekst_20 IJken_Maximum_14;
  s_tekst_15 Instellen_14;
  s_tekst_15 IJken_14;
  s_tekst_2  V_14; // voor unit voltage
  s_tekst_3  mA_14; // voor unit amperes
  s_tekst_10 Overnemen_10;
  s_tekst_25 Windrichting_14; 
  s_tekst_27 Windsnelheid_bij_5V_14; 
  s_tekst_3  kg_14; // eenheid kilogram
  s_tekst_3  g_14; // eenheid gram
  s_tekst_25 Pulsen_Per_Liter_14;
  s_tekst_25 Liters_Per_Puls_14;
  s_tekst_25 Pulsen_Per_Kg_14;
  s_tekst_25 Kg_Per_Puls_14;
  s_tekst_25 Pulsen_Per_Ei_14;
  s_tekst_25 Eieren_Per_Puls_14;
  s_tekst_25 Pulsen_Per_kWh_14;
  s_tekst_25 kWh_Per_Puls_14;
  s_tekst_15 Minimum_14;
  s_tekst_15 Maximum_14;
  s_tekst_20 Aantal_Tellers_14;
  s_tekst_11 Teller_14;
  s_tekst_21 Fout_Licht_Niveau_14;
  s_tekst_21 Fout_Communicatie_14;
  s_tekst_21 Fout_Onbekend_14;
  s_tekst_20 Niet_Toegewezen_14;

  s_tekst_25 Opties_motorgroepen_10;
  s_tekst_21 Aantal_motorgroepen_14;

  s_tekst_21 Opties_groep_1_10;
  s_tekst_21 Opties_groep_2_10;
  s_tekst_21 Opties_groep_3_10;
  s_tekst_21 Opties_groep_4_10;
  s_tekst_21 Opties_groep_5_10;
  s_tekst_21 Opties_groep_6_10;
  s_tekst_21 Opties_groep_7_10;
  s_tekst_21 Opties_groep_8_10;
  s_tekst_21 Opties_groep_9_10;
  s_tekst_21 Opties_groep_10_10;
  s_tekst_21 Opties_groep_11_10;
  s_tekst_21 Opties_groep_12_10;
  s_tekst_21 Opties_groep_13_10;
  s_tekst_21 Opties_groep_14_10;
  s_tekst_21 Opties_groep_15_10;
  s_tekst_21 Opties_groep_16_10;
  s_tekst_21 Opties_groep_17_10;
  s_tekst_21 Opties_groep_18_10;
  s_tekst_21 Opties_groep_19_10;
  s_tekst_21 Opties_groep_20_10;
  s_tekst_21 Opties_groep_21_10;
  s_tekst_21 Opties_groep_22_10;
  s_tekst_21 Opties_groep_23_10;
  s_tekst_21 Opties_groep_24_10;
  s_tekst_21 Opties_groep_25_10;
  s_tekst_21 Opties_groep_26_10;
  s_tekst_21 Opties_groep_27_10;
  s_tekst_21 Opties_groep_28_10;
  s_tekst_21 Opties_groep_29_10;
  s_tekst_21 Opties_groep_30_10;
  s_tekst_21 Opties_groep_31_10;
  s_tekst_21 Opties_groep_32_10;

  s_tekst_30 Opties_2_doek_1_bed_10;
  s_tekst_21 Aantal_14;
  s_tekst_23 Twee_doek_een_bed_1_14;
  s_tekst_23 Twee_doek_een_bed_2_14;
  s_tekst_23 Twee_doek_een_bed_3_14;
  s_tekst_23 Twee_doek_een_bed_4_14;
  s_tekst_23 Twee_doek_een_bed_5_14;
  s_tekst_23 Twee_doek_een_bed_6_14;
  s_tekst_23 Twee_doek_een_bed_7_14;
  s_tekst_23 Twee_doek_een_bed_8_14;
  s_tekst_21 Master_14;
  s_tekst_21 Standby_regeling_14;
  s_tekst_15 Geen_10;
  s_tekst_21 Tegenover_elkaar_10;
  s_tekst_21 Achter_elkaar_10;

  s_tekst_25 Opties_Cabriokas_10;
  s_tekst_21 Cabriokas_1_14;
  s_tekst_21 Cabriokas_2_14;
  s_tekst_21 Cabriokas_3_14;
  s_tekst_21 Cabriokas_4_14;
  s_tekst_21 Cabriokas_5_14;
  s_tekst_21 Cabriokas_6_14;
  s_tekst_21 Cabriokas_7_14;
  s_tekst_21 Cabriokas_8_14;
  s_tekst_18 Voor_naloop_10;
  s_tekst_18 Gelijkloop_10;
  s_tekst_18 Voorloop_14;
  s_tekst_18 Gelijkloop_14;
  s_tekst_18 Hysteresis_14;

  s_tekst_23 Opties_Luchtmengkast_10;
  s_tekst_18 Luchtmengkast_14;
  s_tekst_18 Aantal_groepen_14;
  s_tekst_18 Aantal_units_14;
  s_tekst_18 Type_klep_14;
  s_tekst_18 Binnen_buiten_14;
  s_tekst_18 Recirculatie_14;
  s_tekst_18 Binnenklep_14;
  s_tekst_18 Buitenklep_14;
  s_tekst_18 Recirculatieklep_14;
  s_tekst_18 Bovenklep_14;
  s_tekst_18 Verwarming_14;
  s_tekst_18 Inblaasvent_14;
  s_tekst_18 Afblaasvent_14;
  s_tekst_21 Inblaastemp_14;
  s_tekst_21 Mengtemp_14;
  s_tekst_21 Streeftemp_14;
  s_tekst_18 Vorstbewaking_14;
  s_tekst_18 Alarm_ingang_14;
  s_tekst_18 Drukverschil_14;
  s_tekst_10 Units_14;
  s_tekst_10 Groep_14;
  s_tekst_18 Naregelen_14;

  s_tekst_21 Gekoppeld_aan_klep_14;

  s_tekst_15 Raam_14;
  s_tekst_15 Doek_14;
  s_tekst_15 Ventilatie_14;
  s_tekst_18 Type_sturing_14;
  s_tekst_15 Open_14;
  s_tekst_15 Dicht_14;
  s_tekst_15 Hoger_14;
  s_tekst_15 Lager_14;
  s_tekst_21 Position_14;
  s_tekst_21 Terugmelding_14;
  s_tekst_21 Aantal_motoren_14;
  s_tekst_21 Motoren_14;
  s_tekst_21 Master_nummer_14;
  s_tekst_21 Frequentie_gestuurd_14;
  s_tekst_15 Digitaal_14;
  s_tekst_15 Analoog_14;
  s_tekst_21 Hoge_snelheid_14;
  s_tekst_21 Uitgang_snelheid_14;
  s_tekst_21 Snelheid_14;
  s_tekst_15 Laag_14;
  s_tekst_15 Hoog_14;
  s_tekst_21 Terugschakel_positie_14;
  s_tekst_21 Pulse_system_14;
  s_tekst_21 Kier_regeling_14;
  s_tekst_21 Alarm_analoog_14;

  s_tekst_6  JA_10;
  s_tekst_6  JA_14;
  s_tekst_6  NEE_14;
  s_tekst_6  AAN_14;
  s_tekst_6  UIT_14;
  s_tekst_20 Zeker_weten_7;
  s_tekst_9  Wissen_10;
  s_tekst_9  Kopieren_10;
  s_tekst_3  Pa_7;
  s_tekst_4  ppm_7;
  s_tekst_3  perc_7;
  s_tekst_4  mps_7;
  s_tekst_6  mpmin_7;
  s_tekst_3  graden_celsius_7;
  s_tekst_3  graden_fahrenheid_7;
  s_tekst_3  graden_7;
  s_tekst_3  meter_7;

  s_tekst_21 Alarm_Contact_14;
  s_tekst_21 Alarm_zacht_contact_14;
  s_tekst_21 Alarm_urgent_14;
  s_tekst_23 Opties_Kopieren_14;

  s_tekst_21 COM1_USB_14;
  s_tekst_21 COM2_14;
  s_tekst_21 Modem_14;
  s_tekst_21 Answer_14;
  s_tekst_21 ETHERNET_14;
  s_tekst_9  IP_10;
  s_tekst_9  Mask_10;
  s_tekst_9  Gate_10;
  s_tekst_9  Port_10;

  s_tekst_9  RS485_14;
  s_tekst_18 RS485_baudrate_14;
  s_tekst_18 RS485_pariteit_14;
  s_tekst_9  Geen_14;
  s_tekst_9  Even_14;
  s_tekst_9  Oneven_14;

  s_tekst_18 Looptijd_14;
  s_tekst_18 Aan_uit_14;

  s_tekst_21 Absoluut_14;
  s_tekst_21 Relatief_14;

  s_tekst_9  Type_14;
  s_tekst_18 Adressen_14;
  s_tekst_21 Nummer_7;
  s_tekst_21 niet_beschikbaar_7;
  s_tekst_18 Nummer_10;
  s_tekst_18 Adres_10;
  s_tekst_21 Wijzig_adres_14;
  s_tekst_12 Wijzigen_10;
  s_tekst_21 gewijzigd_10;
  s_tekst_21 communicatie_fout_10;

  s_tekst_21 Opties_Luchting_10;
  s_tekst_18 Luchting_14;

  s_tekst_25 Alarmen_14;
  s_tekst_25 Opties_Alarmen_10;

  s_tekst_23 Frequentie_verstel_14;
  //#ifdef CAN_BACKBONE_PC_WARNING
  s_tekst_5 PC_1_14;
  s_tekst_5 PC_2_14;
  s_tekst_5 PC_3_14;
  s_tekst_15 Waarschuwing_7;
  //#endif // CAN_BACKBONE_PC_WARNING
  //#ifdef PASSWORD
  s_tekst_21 Beheerder_14;
  s_tekst_21 Gebruiker_1_14;
  s_tekst_21 Gebruiker_2_14;
  s_tekst_21 Gebruiker_3_14;
  s_tekst_21 Gebruiker_4_14;
  s_tekst_10 Level_14;
  s_tekst_15 Opties_7;
  s_tekst_15 Setpoints_7;
  s_tekst_15 Setpoints_Syst_7;
  //#endif // PASSWORD

  s_tekst_11 BACnet_Device_ID_10;
  s_tekst_11 BACnet_Network_Nr_10;  

  s_tekst_21 Operation_Mode_14;
  s_tekst_15 Closed_Loop_14;
  s_tekst_15 Open_Loop_14;

  s_tekst_25 Vrijgave_ventilatoren_14;
  s_tekst_35 Opties_Vrijgave_ventilatoren_10;
  s_tekst_21 Vrijgave_14;
  s_tekst_21 Ventilatoren_14;

  s_tekst_15 PowerFactor_14;
  s_tekst_15 RampUpDown_14;
  s_tekst_15 RampUp_14;
  s_tekst_15 RampDown_14;

  s_tekst_15 Klep_14;
  s_tekst_15 SensorType_14;
  s_tekst_18 Eindschakelaar_14;

  s_tekst_18 Sensoren_14;

  s_tekst_21 Alarm_eindschakelaar_14;

  s_tekst_18 Extern_alarm_14;
  s_tekst_25 Vent_At_Max_14;

  s_tekst_21 Watchdog_Mode_14;
  s_tekst_21 Watchdog_Position_14;

  s_tekst_25 Data_Wissen_14;  
  s_tekst_25 Selecteer_Data_14;
  s_tekst_25 Druk_OK_Om_Te_Wissen_14;
  s_tekst_25 Data_Gewist_14; 
  s_tekst_25 Setpoints_10;    
  s_tekst_25 Werkgeheugen_10; 
  s_tekst_25 Restart_10;  

} s_tekst_inst;

#endif
