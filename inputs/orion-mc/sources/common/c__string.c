// C__STRING.C                            

#include "ch_define.h"

#include "ch_disp.h"       
#include "ch_eep_taal.h"   
#include "ch_lcd_pixel.h"
#include "ch_string.h"

s_tekst const tekst_engels =
{
  sizeof(s_tekst), // unsinged int area_size;
  ORION,           // unsigned int computer;
  MULTI_CONNECT,   // unsigned int soort;
  VERSIE_TEKST,    // unsigned int versie_tekst;

  { sizeof(tekst.Gekozen_Taal_14.string),           150, SIZE_14, "English" },

  { sizeof(tekst.Motorgroepen_10.string),           174, SIZE_10, "Motor groups" },

  { sizeof(tekst.Groep_1_14.string),                185, SIZE_14, "Group 1" },
  { sizeof(tekst.Groep_2_14.string),                185, SIZE_14, "Group 2" },
  { sizeof(tekst.Groep_3_14.string),                185, SIZE_14, "Group 3" },
  { sizeof(tekst.Groep_4_14.string),                185, SIZE_14, "Group 4" },
  { sizeof(tekst.Groep_5_14.string),                185, SIZE_14, "Group 5" },
  { sizeof(tekst.Groep_6_14.string),                185, SIZE_14, "Group 6" },
  { sizeof(tekst.Groep_7_14.string),                185, SIZE_14, "Group 7" },
  { sizeof(tekst.Groep_8_14.string),                185, SIZE_14, "Group 8" },
  { sizeof(tekst.Groep_9_14.string),                185, SIZE_14, "Group 9" },
  { sizeof(tekst.Groep_10_14.string),               185, SIZE_14, "Group 10" },
  { sizeof(tekst.Groep_11_14.string),               185, SIZE_14, "Group 11" },
  { sizeof(tekst.Groep_12_14.string),               185, SIZE_14, "Group 12" },
  { sizeof(tekst.Groep_13_14.string),               185, SIZE_14, "Group 13" },
  { sizeof(tekst.Groep_14_14.string),               185, SIZE_14, "Group 14" },
  { sizeof(tekst.Groep_15_14.string),               185, SIZE_14, "Group 15" },
  { sizeof(tekst.Groep_16_14.string),               185, SIZE_14, "Group 16" },
  { sizeof(tekst.Groep_17_14.string),               185, SIZE_14, "Group 17" },
  { sizeof(tekst.Groep_18_14.string),               185, SIZE_14, "Group 18" },
  { sizeof(tekst.Groep_19_14.string),               185, SIZE_14, "Group 19" },
  { sizeof(tekst.Groep_20_14.string),               185, SIZE_14, "Group 20" },
  { sizeof(tekst.Groep_21_14.string),               185, SIZE_14, "Group 21" },
  { sizeof(tekst.Groep_22_14.string),               185, SIZE_14, "Group 22" },
  { sizeof(tekst.Groep_23_14.string),               185, SIZE_14, "Group 23" },
  { sizeof(tekst.Groep_24_14.string),               185, SIZE_14, "Group 24" },
  { sizeof(tekst.Groep_25_14.string),               185, SIZE_14, "Group 25" },
  { sizeof(tekst.Groep_26_14.string),               185, SIZE_14, "Group 26" },
  { sizeof(tekst.Groep_27_14.string),               185, SIZE_14, "Group 27" },
  { sizeof(tekst.Groep_28_14.string),               185, SIZE_14, "Group 28" },
  { sizeof(tekst.Groep_29_14.string),               185, SIZE_14, "Group 29" },
  { sizeof(tekst.Groep_30_14.string),               185, SIZE_14, "Group 30" },
  { sizeof(tekst.Groep_31_14.string),               185, SIZE_14, "Group 31" },
  { sizeof(tekst.Groep_32_14.string),               185, SIZE_14, "Group 32" },

  { sizeof(tekst.EXT_OPEN_10.string),                76, SIZE_10, "EXT. OPEN" },
  { sizeof(tekst.EXT_DICHT_10.string),               76, SIZE_10, "EXT. CLOSE" },
  { sizeof(tekst.EXT_STOP_10.string),                76, SIZE_10, "EXT. STOP" },
  { sizeof(tekst.OPEN_10.string),                    60, SIZE_10, "OPEN" },
  { sizeof(tekst.DICHT_10.string),                   60, SIZE_10, "CLOSE" },
  { sizeof(tekst.STOP_10.string),                    60, SIZE_10, "STOP" },
  { sizeof(tekst.HAND_10.string),                    60, SIZE_10, "MANUAL" },
  { sizeof(tekst.AUTO_10.string),                    60, SIZE_10, "AUTO" },
  { sizeof(tekst.UIT_10.string),                     60, SIZE_10, "OFF" },
  { sizeof(tekst.HAND_7.string),                     60, SIZE_7,  "MANUAL" },
  { sizeof(tekst.AUTO_7.string),                     60, SIZE_7,  "AUTO" },
  { sizeof(tekst.UIT_7.string),                      60, SIZE_7,  "OFF" },

  { sizeof(tekst.Zondag_10.string),                  80, SIZE_10, "Sunday" },
  { sizeof(tekst.Maandag_10.string),                 80, SIZE_10, "Monday" },
  { sizeof(tekst.Dinsdag_10.string),                 80, SIZE_10, "Tuesday" },
  { sizeof(tekst.Woensdag_10.string),                80, SIZE_10, "Wednesday" },
  { sizeof(tekst.Donderdag_10.string),               80, SIZE_10, "Thursday" },
  { sizeof(tekst.Vrijdag_10.string),                 80, SIZE_10, "Friday" },
  { sizeof(tekst.Zaterdag_10.string),                80, SIZE_10, "Saturday" },
  { sizeof(tekst.Tijd_en_Datum_14.string),          200, SIZE_14, "Time + Date" },

  { sizeof(tekst.Fn_10.string),                      40, SIZE_10, "Fn.:" },
  { sizeof(tekst.Syst_10.string),                    40, SIZE_10, "Syst.:" },
  { sizeof(tekst.Diag_10.string),                    40, SIZE_10, "Diag.:" },

  { sizeof(tekst.Groep_1_10.string),                129, SIZE_10, "Group 1" },
  { sizeof(tekst.Groep_2_10.string),                129, SIZE_10, "Group 2" },
  { sizeof(tekst.Groep_3_10.string),                129, SIZE_10, "Group 3" },
  { sizeof(tekst.Groep_4_10.string),                129, SIZE_10, "Group 4" },
  { sizeof(tekst.Groep_5_10.string),                129, SIZE_10, "Group 5" },
  { sizeof(tekst.Groep_6_10.string),                129, SIZE_10, "Group 6" },
  { sizeof(tekst.Groep_7_10.string),                129, SIZE_10, "Group 7" },
  { sizeof(tekst.Groep_8_10.string),                129, SIZE_10, "Group 8" },
  { sizeof(tekst.Groep_9_10.string),                129, SIZE_10, "Group 9" },
  { sizeof(tekst.Groep_10_10.string),               129, SIZE_10, "Group 10" },
  { sizeof(tekst.Groep_11_10.string),               129, SIZE_10, "Group 11" },
  { sizeof(tekst.Groep_12_10.string),               129, SIZE_10, "Group 12" },
  { sizeof(tekst.Groep_13_10.string),               129, SIZE_10, "Group 13" },
  { sizeof(tekst.Groep_14_10.string),               129, SIZE_10, "Group 14" },
  { sizeof(tekst.Groep_15_10.string),               129, SIZE_10, "Group 15" },
  { sizeof(tekst.Groep_16_10.string),               129, SIZE_10, "Group 16" },
  { sizeof(tekst.Groep_17_10.string),               129, SIZE_10, "Group 17" },
  { sizeof(tekst.Groep_18_10.string),               129, SIZE_10, "Group 18" },
  { sizeof(tekst.Groep_19_10.string),               129, SIZE_10, "Group 19" },
  { sizeof(tekst.Groep_20_10.string),               129, SIZE_10, "Group 20" },
  { sizeof(tekst.Groep_21_10.string),               129, SIZE_10, "Group 21" },
  { sizeof(tekst.Groep_22_10.string),               129, SIZE_10, "Group 22" },
  { sizeof(tekst.Groep_23_10.string),               129, SIZE_10, "Group 23" },
  { sizeof(tekst.Groep_24_10.string),               129, SIZE_10, "Group 24" },
  { sizeof(tekst.Groep_25_10.string),               129, SIZE_10, "Group 25" },
  { sizeof(tekst.Groep_26_10.string),               129, SIZE_10, "Group 26" },
  { sizeof(tekst.Groep_27_10.string),               129, SIZE_10, "Group 27" },
  { sizeof(tekst.Groep_28_10.string),               129, SIZE_10, "Group 28" },
  { sizeof(tekst.Groep_29_10.string),               129, SIZE_10, "Group 29" },
  { sizeof(tekst.Groep_30_10.string),               129, SIZE_10, "Group 30" },
  { sizeof(tekst.Groep_31_10.string),               129, SIZE_10, "Group 31" },
  { sizeof(tekst.Groep_32_10.string),               129, SIZE_10, "Group 32" },

  { sizeof(tekst.Motor_1_10.string),                110, SIZE_10, "Motor 1" },
  { sizeof(tekst.Motor_2_10.string),                110, SIZE_10, "Motor 2" },
  { sizeof(tekst.Motor_3_10.string),                110, SIZE_10, "Motor 3" },
  { sizeof(tekst.Motor_4_10.string),                110, SIZE_10, "Motor 4" },
  { sizeof(tekst.Motor_5_10.string),                110, SIZE_10, "Motor 5" },
  { sizeof(tekst.Motor_6_10.string),                110, SIZE_10, "Motor 6" },
  { sizeof(tekst.Motor_7_10.string),                110, SIZE_10, "Motor 7" },
  { sizeof(tekst.Motor_8_10.string),                110, SIZE_10, "Motor 8" },
  { sizeof(tekst.Motor_9_10.string),                110, SIZE_10, "Motor 9" },
  { sizeof(tekst.Motor_10_10.string),               110, SIZE_10, "Motor 10" },
  { sizeof(tekst.Motor_11_10.string),               110, SIZE_10, "Motor 11" },
  { sizeof(tekst.Motor_12_10.string),               110, SIZE_10, "Motor 12" },
  { sizeof(tekst.Motor_13_10.string),               110, SIZE_10, "Motor 13" },
  { sizeof(tekst.Motor_14_10.string),               110, SIZE_10, "Motor 14" },
  { sizeof(tekst.Motor_15_10.string),               110, SIZE_10, "Motor 15" },
  { sizeof(tekst.Motor_16_10.string),               110, SIZE_10, "Motor 16" },
  { sizeof(tekst.Motor_17_10.string),               110, SIZE_10, "Motor 17" },
  { sizeof(tekst.Motor_18_10.string),               110, SIZE_10, "Motor 18" },
  { sizeof(tekst.Motor_19_10.string),               110, SIZE_10, "Motor 19" },
  { sizeof(tekst.Motor_20_10.string),               110, SIZE_10, "Motor 20" },
  { sizeof(tekst.Motor_21_10.string),               110, SIZE_10, "Motor 21" },
  { sizeof(tekst.Motor_22_10.string),               110, SIZE_10, "Motor 22" },
  { sizeof(tekst.Motor_23_10.string),               110, SIZE_10, "Motor 23" },
  { sizeof(tekst.Motor_24_10.string),               110, SIZE_10, "Motor 24" },
  { sizeof(tekst.Motor_25_10.string),               110, SIZE_10, "Motor 25" },
  { sizeof(tekst.Motor_26_10.string),               110, SIZE_10, "Motor 26" },
  { sizeof(tekst.Motor_27_10.string),               110, SIZE_10, "Motor 27" },
  { sizeof(tekst.Motor_28_10.string),               110, SIZE_10, "Motor 28" },
  { sizeof(tekst.Motor_29_10.string),               110, SIZE_10, "Motor 29" },
  { sizeof(tekst.Motor_30_10.string),               110, SIZE_10, "Motor 30" },
  { sizeof(tekst.Motor_31_10.string),               110, SIZE_10, "Motor 31" },
  { sizeof(tekst.Motor_32_10.string),               110, SIZE_10, "Motor 32" },
  { sizeof(tekst.Motor_33_10.string),               110, SIZE_10, "Motor 33" },
  { sizeof(tekst.Motor_34_10.string),               110, SIZE_10, "Motor 34" },
  { sizeof(tekst.Motor_35_10.string),               110, SIZE_10, "Motor 35" },
  { sizeof(tekst.Motor_36_10.string),               110, SIZE_10, "Motor 36" },
  { sizeof(tekst.Motor_37_10.string),               110, SIZE_10, "Motor 37" },
  { sizeof(tekst.Motor_38_10.string),               110, SIZE_10, "Motor 38" },
  { sizeof(tekst.Motor_39_10.string),               110, SIZE_10, "Motor 39" },
  { sizeof(tekst.Motor_40_10.string),               110, SIZE_10, "Motor 40" },
  { sizeof(tekst.Motor_41_10.string),               110, SIZE_10, "Motor 41" },
  { sizeof(tekst.Motor_42_10.string),               110, SIZE_10, "Motor 42" },
  { sizeof(tekst.Motor_43_10.string),               110, SIZE_10, "Motor 43" },
  { sizeof(tekst.Motor_44_10.string),               110, SIZE_10, "Motor 44" },
  { sizeof(tekst.Motor_45_10.string),               110, SIZE_10, "Motor 45" },
  { sizeof(tekst.Motor_46_10.string),               110, SIZE_10, "Motor 46" },
  { sizeof(tekst.Motor_47_10.string),               110, SIZE_10, "Motor 47" },
  { sizeof(tekst.Motor_48_10.string),               110, SIZE_10, "Motor 48" },
  { sizeof(tekst.Motor_49_10.string),               110, SIZE_10, "Motor 49" },
  { sizeof(tekst.Motor_50_10.string),               110, SIZE_10, "Motor 50" },
  { sizeof(tekst.Motor_51_10.string),               110, SIZE_10, "Motor 51" },
  { sizeof(tekst.Motor_52_10.string),               110, SIZE_10, "Motor 52" },
  { sizeof(tekst.Motor_53_10.string),               110, SIZE_10, "Motor 53" },
  { sizeof(tekst.Motor_54_10.string),               110, SIZE_10, "Motor 54" },
  { sizeof(tekst.Motor_55_10.string),               110, SIZE_10, "Motor 55" },
  { sizeof(tekst.Motor_56_10.string),               110, SIZE_10, "Motor 56" },
  { sizeof(tekst.Motor_57_10.string),               110, SIZE_10, "Motor 57" },
  { sizeof(tekst.Motor_58_10.string),               110, SIZE_10, "Motor 58" },
  { sizeof(tekst.Motor_59_10.string),               110, SIZE_10, "Motor 59" },
  { sizeof(tekst.Motor_60_10.string),               110, SIZE_10, "Motor 60" },
  { sizeof(tekst.Motor_61_10.string),               110, SIZE_10, "Motor 61" },
  { sizeof(tekst.Motor_62_10.string),               110, SIZE_10, "Motor 62" },
  { sizeof(tekst.Motor_63_10.string),               110, SIZE_10, "Motor 63" },
  { sizeof(tekst.Motor_64_10.string),               110, SIZE_10, "Motor 64" },

  { sizeof(tekst.Status_10.string),                 100, SIZE_10, "Status" },
  { sizeof(tekst.Bediening_10.string),              110, SIZE_10, "Operation" },
  { sizeof(tekst.Positie_10.string),                110, SIZE_10, "Position" },

  { sizeof(tekst.Alarm_HAND_10.string),             110, SIZE_10, "Alarm MANUAL" },
  { sizeof(tekst.Alarm_afwijking_10.string),        110, SIZE_10, "Alarm variance" },
  { sizeof(tekst.Alarm_urgent_10.string),           110, SIZE_10, "Alarm urgent" },
  { sizeof(tekst.Pulse_zone_10.string),             110, SIZE_10, "Pulse zone" },
  { sizeof(tekst.Pulse_width_10.string),            110, SIZE_10, "Pulse width" },
  { sizeof(tekst.Cycletime_10.string),              110, SIZE_10, "Cycletime" },
  { sizeof(tekst.Hysterese_10.string),              130, SIZE_10, "Hysteresis" },                                        
  { sizeof(tekst.Stapgrootte_10.string),            130, SIZE_10, "Stepsize" },                               
  { sizeof(tekst.Bandbreedte_10.string),            136, SIZE_10, "Bandwidth" },                                      

  { sizeof(tekst.Twee_doeken_een_bed_1_10.string),  145, SIZE_10, "Dual screens [1]" },
  { sizeof(tekst.Twee_doeken_een_bed_2_10.string),  145, SIZE_10, "Dual screens [2]" },
  { sizeof(tekst.Twee_doeken_een_bed_3_10.string),  145, SIZE_10, "Dual screens [3]" },
  { sizeof(tekst.Twee_doeken_een_bed_4_10.string),  145, SIZE_10, "Dual screens [4]" },
  { sizeof(tekst.Twee_doeken_een_bed_5_10.string),  145, SIZE_10, "Dual screens [5]" },
  { sizeof(tekst.Twee_doeken_een_bed_6_10.string),  145, SIZE_10, "Dual screens [6]" },
  { sizeof(tekst.Twee_doeken_een_bed_7_10.string),  145, SIZE_10, "Dual screens [7]" },
  { sizeof(tekst.Twee_doeken_een_bed_8_10.string),  145, SIZE_10, "Dual screens [8]" },

  { sizeof(tekst.Kier_10.string),                   110, SIZE_10, "Gap" },
  { sizeof(tekst.Hysteresis_10.string),             110, SIZE_10, "Hysteresis" },
  { sizeof(tekst.Standby_10.string),                110, SIZE_10, "Standby" },
  { sizeof(tekst.Voorloop_10.string),               110, SIZE_10, "Leading" },

  { sizeof(tekst.Time_10.string),                   129, SIZE_10, "Time" },
  { sizeof(tekst.Tijd_10.string),                   110, SIZE_10, "Time" },
  { sizeof(tekst.Datum_10.string),                   75, SIZE_10, "Date" },
  { sizeof(tekst.Sync_Tijd_10.string),              150, SIZE_10, "Synchronise time" },
  { sizeof(tekst.Dagenteller_10.string),            140, SIZE_10, "Day counter" },

  { sizeof(tekst.F2_10.string),                     174, SIZE_10, "Ventilation" },

  { sizeof(tekst.F3_10.string),                     174, SIZE_10, "Sensors" },

  { sizeof(tekst.Alarmen_10.string),                174, SIZE_10, "Alarms" },
  { sizeof(tekst.Alarmen_Actief_10.string),         174, SIZE_10, "Current alarms" },
  { sizeof(tekst.Alarmen_Historie_10.string),       174, SIZE_10, "Alarm log" },
  { sizeof(tekst.Alarmen_Wissen_10.string),         174, SIZE_10, "Delete alarm log" },
  { sizeof(tekst.Alarm_10.string),                  174, SIZE_10, "Alarm" },
  { sizeof(tekst.Waarschuwing_10.string),           185, SIZE_10, "Warning" },
  { sizeof(tekst.Alarm_Systeem_10.string),          185, SIZE_10, "Alarm (System)" },
  { sizeof(tekst.Waarschuwing_Syst_10.string),      185, SIZE_10, "Warning (Syst)" },
  { sizeof(tekst.Alarm_Computer_10.string),          40, SIZE_10, "Alarm" },
  { sizeof(tekst.Geen_Actief_Alarm_10.string),      185, SIZE_10, "No current alarm" },
  { sizeof(tekst.Onbekend_Alarm_10.string),         185, SIZE_10, "Unknown alarm" },
  { sizeof(tekst.Opties_10.string),                 174, SIZE_10, "Options" },
  { sizeof(tekst.Instellingen_10.string),           185, SIZE_10, "Settings" },
  { sizeof(tekst.Opties_En_Instellingen_10.string), 185, SIZE_10, "Options + settings" },
  { sizeof(tekst.Minimum_Maximum_10.string),        185, SIZE_10, "Minimum/Maximum" },
  { sizeof(tekst.Gewist_10.string),                 185, SIZE_10, "Deleted" },
  { sizeof(tekst.Terug_Gezet_10.string),            185, SIZE_10, "Restored" },
  { sizeof(tekst.Niet_Terug_Gezet_10.string),       185, SIZE_10, "Not restored" },
  { sizeof(tekst.I2C_10.string),                    185, SIZE_10, "I2C (EEPROM/RTC)" },
  { sizeof(tekst.EEPROM_10.string),                 185, SIZE_10, "EEPROM" },
  { sizeof(tekst.EEPROM_taal_10.string),            185, SIZE_10, "EEPROM (language)" },
  { sizeof(tekst.RTC_10.string),                    185, SIZE_10, "RTC" },
  { sizeof(tekst.TIMER_10.string),                  185, SIZE_10, "TIMER" },
  { sizeof(tekst.HTRAP_10.string),                  185, SIZE_10, "HTRAP" },
  { sizeof(tekst.PLL_10.string),                    185, SIZE_10, "PLL" },
  { sizeof(tekst.Opnieuw_Gestart_10.string),        185, SIZE_10, "Restart computer" },
  { sizeof(tekst.Print_10.string),                   50, SIZE_10, "Board" },
  { sizeof(tekst.Niet_Gevonden_10.string),          185, SIZE_10, "Not found" },
  { sizeof(tekst.ADC_10.string),                    185, SIZE_10, "ADC" },
  { sizeof(tekst.Analoge_Ingang_10.string),         150, SIZE_10, "Analogue input" },
  { sizeof(tekst.Externe_24V_10.string),            185, SIZE_10, "External 24V" },                
  { sizeof(tekst.Orion_Uitgeschakeld_10.string),    185, SIZE_10, "Orion switched-off" },
  { sizeof(tekst.Geen_Minimum_Alarm_10.string),     185, SIZE_10, "No Minimum Alarm" },
  { sizeof(tekst.Offline_10.string),                 80, SIZE_10, "offline" },
  { sizeof(tekst.Slave_10.string),                   80, SIZE_10, "Slave" },

  { sizeof(tekst.Diagnose_10.string),               174, SIZE_10, "Diagnostics" },

  { sizeof(tekst.Info_10.string),                   132, SIZE_10, "Info" },
  { sizeof(tekst.Serie_Nr_14.string),                90, SIZE_14, "Serial No:" },
  { sizeof(tekst.Computer_10.string),                80, SIZE_10, "Computer:" },
  { sizeof(tekst.Computer_14.string),               100, SIZE_14, "Computer:" },

  { sizeof(tekst.Draaiuren_10.string),              110, SIZE_10, "Running hours" },
  { sizeof(tekst.Schakelingen_10.string),           110, SIZE_10, "Switches" },
  { sizeof(tekst.Storingen_10.string),              110, SIZE_10, "Failures" },
  { sizeof(tekst.Looptijd_10.string),               110, SIZE_10, "Running time" },
  { sizeof(tekst.Alarm_Code_10.string),             110, SIZE_10, "Alarm Code" },

  { sizeof(tekst.Bewerken_Tekst_10.string),         185, SIZE_10, "Edit text" },

  { sizeof(tekst.kg_10.string),                      16, SIZE_10, "kg" },
  { sizeof(tekst.kg_14.string),                      19, SIZE_14, "kg" },
  { sizeof(tekst.l_10.string),                       16, SIZE_10, "L" },
  { sizeof(tekst.l_14.string),                       19, SIZE_14, "L" },
  { sizeof(tekst.g_10.string),                       16, SIZE_10, "g" },
  { sizeof(tekst.ml_10.string),                      16, SIZE_10, "ml" },
  { sizeof(tekst.m_10.string),                       16, SIZE_10, "m" },
  { sizeof(tekst.s_10.string),                       16, SIZE_10, "s" },
  { sizeof(tekst.AAN_20.string),                     45, SIZE_20, "ON" },
  { sizeof(tekst.UIT_20.string),                     45, SIZE_20, "OFF" },
  { sizeof(tekst.Ana_10.string),                     36, SIZE_10, "Ana" },
  { sizeof(tekst.Dig_10.string),                     36, SIZE_10, "Dig" },
  { sizeof(tekst.MC_10.string),                      36, SIZE_10, "-" },
  { sizeof(tekst.Geen_IO_Toegewezen_10.string),     185, SIZE_10, "No I/O assigned" },

  { sizeof(tekst.Password_10.string),               111, SIZE_10, "Password" },
  { sizeof(tekst.Password_14.string),               111, SIZE_14, "Password:" },
  { sizeof(tekst.Zeker_weten_7.string),             105, SIZE_7,  "Are you sure?" },
  { sizeof(tekst.Wissen_10.string),                  65, SIZE_10, "delete" },

  { sizeof(tekst.CAN_LOCAL_14.string),              185, SIZE_14, "CAN LOCAL" },
  { sizeof(tekst.Diag_CAN_LOCAL_10.string),         174, SIZE_10, "Diag.: CAN LOCAL" },
  { sizeof(tekst.TXOK_10.string),                    90, SIZE_10, "TXOK" },
  { sizeof(tekst.RXOK_10.string),                    90, SIZE_10, "RXOK" },
  { sizeof(tekst.TXOK_RXOK_10.string),               90, SIZE_10, "TXOK+RXOK" },
  { sizeof(tekst.BOFF_10.string),                   130, SIZE_10, "BOFF" },
  { sizeof(tekst.EWRN_10.string),                   130, SIZE_10, "EWRN" },
  { sizeof(tekst.Stuff_error_10.string),            130, SIZE_10, "Stuff error" },
  { sizeof(tekst.Form_error_10.string),             130, SIZE_10, "Form error" },
  { sizeof(tekst.Ack_error_10.string),              130, SIZE_10, "Ack error" },
  { sizeof(tekst.Bit1_error_10.string),             130, SIZE_10, "Bit1 error" },
  { sizeof(tekst.Bit0_busoff_error_10.string),      130, SIZE_10, "Bit0 busoff error" },
  { sizeof(tekst.Bit0_normal_error_10.string),      130, SIZE_10, "Bit0 normal error" },
  { sizeof(tekst.Crc_error_10.string),              130, SIZE_10, "Crc error" },
  { sizeof(tekst.perc_7.string),                      8, SIZE_7,  "%" },
  { sizeof(tekst.Pa_7.string),                       15, SIZE_7,  "Pa" },
  { sizeof(tekst.g_14.string),                       15, SIZE_14, "g" },
  { sizeof(tekst.ml_14.string),                      15, SIZE_14, "ml" },
  { sizeof(tekst.graden_celsius_7.string),           10, SIZE_7,  "°C" },
  { sizeof(tekst.graden_fahrenheid_7.string),        10, SIZE_7,  "°F" },
  { sizeof(tekst.graden_7.string),                    8, SIZE_7,  "°" },
  { sizeof(tekst.minuut_7.string),                    8, SIZE_7,  "min" },

  { sizeof(tekst.Geen_Alarm_Contact_10.string),     185, SIZE_10, "No alarm contact" },

  { sizeof(tekst.Handbediening_10.string),           185, SIZE_10, "Manual control" },
  { sizeof(tekst.Emergency_switch_10.string),        185, SIZE_10, "Emergency switch" },
  { sizeof(tekst.Thermal_failure_close_10.string),   185, SIZE_10, "Thermal failure close" },
  { sizeof(tekst.Thermal_failure_open_10.string),    185, SIZE_10, "Thermal failure open" },
  { sizeof(tekst.Break_input_10.string),             185, SIZE_10, "Break input" },
  { sizeof(tekst.Speed_to_low_10.string),            185, SIZE_10, "Speed to low" },
  { sizeof(tekst.Encoder_failure_10.string),         185, SIZE_10, "Encoder failure" },
  { sizeof(tekst.Encoder_failure_A_10.string),       185, SIZE_10, "Encoder failure A-signal" },
  { sizeof(tekst.Encoder_failure_B_10.string),       185, SIZE_10, "Encoder failure B-signal" },
  { sizeof(tekst.No_feedback_10.string),             185, SIZE_10, "No feedback" },
  { sizeof(tekst.Not_enough_pulses_10.string),       185, SIZE_10, "Not enough pulses" },
  { sizeof(tekst.Pulses_to_fast_10.string),          185, SIZE_10, "Pulses to fast" },
  { sizeof(tekst.Not_installed_10.string),           185, SIZE_10, "Not installed" },
  { sizeof(tekst.Encoder_interference_10.string),    185, SIZE_10, "Encoder interference" },
  { sizeof(tekst.Dualscreen_not_possible_10.string), 185, SIZE_10, "Dualscreen not possible" },
  { sizeof(tekst.Installation_mode_10.string),       185, SIZE_10, "Installation mode" },
  { sizeof(tekst.Deviation_position_10.string),      185, SIZE_10, "Position deviates" },
  { sizeof(tekst.Sensor_hi_speed_10.string),         185, SIZE_10, "Sensor hi-speed" },
  { sizeof(tekst.Direction_not_defined_10.string),   185, SIZE_10, "Direction not defined" },
  { sizeof(tekst.Limitswitches_not_equal_10.string), 185, SIZE_10, "Limitswitches not equal" },
  { sizeof(tekst.Speed_not_equal_10.string),         185, SIZE_10, "Speed not equal" },
  { sizeof(tekst.Multiple_master_10.string),         185, SIZE_10, "Multiple masters" },
  { sizeof(tekst.Frequency_controller_10.string),    185, SIZE_10, "Frequency controller" },
  { sizeof(tekst.Gelijkloop_beveiliging_10.string),  185, SIZE_10, "Sync guard active" },
  { sizeof(tekst.Limitswitch_not_reached_10.string), 185, SIZE_10, "Limitswitch not reached" },
  { sizeof(tekst.Wrong_direction_10.string),         185, SIZE_10, "Wrong direction" },
  { sizeof(tekst.Link_unknown_10.string),            185, SIZE_10, "Link unknown" },
  { sizeof(tekst.Motor_not_running_10.string),       185, SIZE_10, "No pulses" },
  { sizeof(tekst.Position_not_reached_10.string),    185, SIZE_10, "Position not reached" },

  { sizeof(tekst.Locked_motor_10.string),              185, SIZE_10, "Locked motor" },
  { sizeof(tekst.Hall_failure_10.string),              185, SIZE_10, "Hall failure" },
  { sizeof(tekst.Thermal_motor_10.string),             185, SIZE_10, "Thermal overload motor" },
  { sizeof(tekst.Comm_error_master_slave_10.string),   185, SIZE_10, "Comm master/slave PIC" },
  { sizeof(tekst.Thermal_power_module_10.string),      185, SIZE_10, "Thermal power module" },
  { sizeof(tekst.Comm_error_remote_unit_10.string),    185, SIZE_10, "Comm error remote unit" },
  { sizeof(tekst.Phase_failure_10.string),             185, SIZE_10, "Phase failure" },
  { sizeof(tekst.Break_10.string),                     185, SIZE_10, "Break" },
  { sizeof(tekst.Low_line_voltage_10.string),          185, SIZE_10, "Low line voltage" },
  { sizeof(tekst.Low_DC_link_voltage_10.string),       185, SIZE_10, "Low DC-link voltage" },
  { sizeof(tekst.High_DC_link_voltage_10.string),      185, SIZE_10, "High DC-link voltage" },
  { sizeof(tekst.Driver_problem_10.string),            185, SIZE_10, "Driver problem" },
  { sizeof(tekst.Electronic_box_over_heat_10.string),  185, SIZE_10, "Electronic box over heat" },
  { sizeof(tekst.Excessive_DC_link_current_10.string), 185, SIZE_10, "Excessive DC-link current" },

  { sizeof(tekst.Vorstbeveiliging_10.string),          185, SIZE_10, "Frost protection" },
  { sizeof(tekst.Algemeen_extern_alarm_10.string),     185, SIZE_10, "External alarm" },
  { sizeof(tekst.Drukverschil_bewaking_10.string),     185, SIZE_10, "Drukverschil bewaking" },

  { sizeof(tekst.JA_10.string),                      45, SIZE_10, "YES" },
  { sizeof(tekst.NEE_10.string),                     45, SIZE_10, "NO" },

  { sizeof(tekst.CAN_RS232_10.string),               80, SIZE_10, "CAN-RS232" },

  { sizeof(tekst.CAN_BACKBONE_14.string),           185, SIZE_14, "CAN BACKBONE" },
  { sizeof(tekst.Diag_CAN_BACKBONE_10.string),      174, SIZE_10, "Diag.: CAN BACKBONE" },

  { sizeof(tekst.Wijzig_computer_type_14.string),   180, SIZE_14, "Module"/*"Change computer type"*/ },
  { sizeof(tekst.Activeer_module_7.string),         105, SIZE_7,  "Activate module" },
  { sizeof(tekst.Deactiveer_module_7.string),       105, SIZE_7,  "Deactivate module" },
  { sizeof(tekst.Ongeldige_code_7.string),          105, SIZE_7,  "Not a valid code!" },

  { sizeof(tekst.SD_CARD_10.string),                 90, SIZE_10, "SD-CARD" },
  { sizeof(tekst.SD_CARD_14.string),                185, SIZE_14, "SD-CARD" },
  { sizeof(tekst.Diag_SD_CARD_10.string),           174, SIZE_10, "Diag.: SD-CARD" },
  { sizeof(tekst.no_card_10.string),                 90, SIZE_10, "no card" },
  { sizeof(tekst.placed_10.string),                  90, SIZE_10, "placed" },
  { sizeof(tekst.read_write_10.string),              90, SIZE_10, "read/write" },
  { sizeof(tekst.read_only_10.string),               90, SIZE_10, "read only" },
  { sizeof(tekst.active_10.string),                  90, SIZE_10, "active" },
  { sizeof(tekst.ask_remove_10.string),              90, SIZE_10, "ask remove" },
  { sizeof(tekst.remove_10.string),                  90, SIZE_10, "remove" },
  { sizeof(tekst.SIZE_7.string),                     40, SIZE_7,  "SIZE" },

  { sizeof(tekst.Diag_COM1_USB_10.string),          185, SIZE_10, "Diag.: COM1/USB" },     // Diag_COM1_USB_10
  { sizeof(tekst.COM1_USB_10.string),                75, SIZE_10, "COM1/USB" },            // COM1_USB_10
  { sizeof(tekst.kBd_14.string),                     30, SIZE_14, "kBd" },                 // kBd_14
  { sizeof(tekst.TX_10.string),                      60, SIZE_10, "TX" },                  // TX_10
  { sizeof(tekst.RX_10.string),                      60, SIZE_10, "RX" },                  // RX_10
  { sizeof(tekst.TX_RX_10.string),                   60, SIZE_10, "TX+RX" },               // TX_RX_10
  { sizeof(tekst.AT_10.string),                      90, SIZE_10, "AT" },                  // AT_10
  { sizeof(tekst.Modem_Init_10.string),              90, SIZE_10, "Modem Init" },          // Modem_Init_10

  { sizeof(tekst.Diag_COM2_10.string),              185, SIZE_10, "Diag.: COM2" },         // Diag_COM2_10
  { sizeof(tekst.COM2_10.string),                    75, SIZE_10, "COM2" },                // COM2_10
  { sizeof(tekst.Modem_DCD_10.string),               90, SIZE_10, "Modem DCD" },           // Modem_DCD_10

  { sizeof(tekst.Diag_Ethernet_10.string),          185, SIZE_10, "Diag.: Ethernet" },     // Diag_Ethernet_10
  { sizeof(tekst.IP_10.string),                      50, SIZE_10, "IP" },                  // IP_10
  { sizeof(tekst.Mask_10.string),                    50, SIZE_10, "Mask" },                // Mask_10
  { sizeof(tekst.Gate_10.string),                    50, SIZE_10, "Gate" },                // Gate_10
  { sizeof(tekst.MAC_10.string),                     50, SIZE_10, "MAC" },                 // MAC_10
  { sizeof(tekst.PC_10.string),                      50, SIZE_10, "PC" },                  // PC_10
  { sizeof(tekst.NO_CONNECTION_14.string),          150, SIZE_14, "NO CONNECTION" },       // NO_CONNECTION_14
  { sizeof(tekst.RTX_10.string),                     60, SIZE_10, "RTX" },                 // RTX_10
  { sizeof(tekst.NO_ACK_10.string),                  60, SIZE_10, "NO_ACK" },              // NO_ACK_10
  { sizeof(tekst.CONNECT_10.string),                 60, SIZE_10, "CONNECT" },             // CONNECT_10
  { sizeof(tekst.Reset_Ethernet_10.string),         130, SIZE_10, "Reset Ethernet" },      // Reset_Ethernet_10
  { sizeof(tekst.Port_10.string),                    50, SIZE_10, "Port" },                // Port_10

  { sizeof(tekst.COM1_USB_14.string),               185, SIZE_14, "COM1/USB" },            // COM1_USB_14
  { sizeof(tekst.COM2_14.string),                   185, SIZE_14, "COM2" },                // COM2_14
  { sizeof(tekst.Ethernet_14.string),               185, SIZE_14, "Ethernet" },            // Ethernet_14

  { sizeof(tekst.Ventilator_10.string),             185, SIZE_10, "Fan" },
  { sizeof(tekst.Minimum_10.string),                185, SIZE_10, "Minimum" },
  { sizeof(tekst.Maximum_10.string),                185, SIZE_10, "Maximum" },
  { sizeof(tekst.Offset_10.string),                 185, SIZE_10, "Offset" },

  { sizeof(tekst.Maximum_rpm_10.string),            110, SIZE_10, "Maximum rpm" },
  { sizeof(tekst.Vermogen_10.string),               110, SIZE_10, "Power consumption" },
  { sizeof(tekst.Temp_motor_10.string),             110, SIZE_10, "Temp motor" },
  { sizeof(tekst.Temp_electronica_10.string),       110, SIZE_10, "Temp electronics" },
  { sizeof(tekst.Temp_power_module_10.string),      110, SIZE_10, "Temp power module" },
  { sizeof(tekst.Fabrieksinst_terugz_10.string),    110, SIZE_10, "Fabrieksinst. terugz." },
  { sizeof(tekst.Versie_10.string),                 110, SIZE_10, "Version" },

  { sizeof(tekst.Reset_all_10.string),              110, SIZE_10, "Reset all" },

  { sizeof(tekst.V_10.string),                       60, SIZE_10, "L:" },
  { sizeof(tekst.N_10.string),                       60, SIZE_10, "T:" },
  { sizeof(tekst.Geheugen_256k_10.string),          185, SIZE_10, "ONLY 256k RAM" },      	  
  { sizeof(tekst.Ongeldige_update_10.string),       185, SIZE_10, "Unvalid update" },
  { sizeof(tekst.Plaats_512k_EEPROM_10.string),     185, SIZE_10, "Insert 512k EEPROM" },
  { sizeof(tekst.OK_is_opties_wissen_10.string),    185, SIZE_10, "OK = erase options" },

  { sizeof(tekst.Luchtmengkast_14.string),          185, SIZE_14, "Luchtmengkast" },
  { sizeof(tekst.Luchtmengkast_groep_14.string),    185, SIZE_14, "Luchtmengkast groep" },
  { sizeof(tekst.Luchtmengkast_10.string),          185, SIZE_10, "Luchtmengkast" },
  { sizeof(tekst.Luchtmengkast_groep_10.string),    185, SIZE_10, "Luchtmengkast groep" },
  { sizeof(tekst.Diag_Luchtmengkast_10.string),     185, SIZE_10, "Diag.: Luchtmengkast" },
  { sizeof(tekst.Fn_Luchtmengkast_10.string),       185, SIZE_10, "Fn.: Luchtmengkast" },
  { sizeof(tekst.Syst_Luchtmengkast_10.string),     185, SIZE_10, "Syst.: Luchtmengkast" },

  { sizeof(tekst.Binnenklep_10.string),             185, SIZE_10, "Binnenklep" },
  { sizeof(tekst.Buitenklep_10.string),             185, SIZE_10, "Buitenklep" },
  { sizeof(tekst.Recirculatieklep_10.string),       185, SIZE_10, "Recirculatieklep" },
  { sizeof(tekst.Bovenklep_10.string),              185, SIZE_10, "Bovenklep" },
  { sizeof(tekst.Verwarming_10.string),             185, SIZE_10, "Verwarming" },
  { sizeof(tekst.Inblaasvent_10.string),            185, SIZE_10, "Inblaasventilator" },
  { sizeof(tekst.Afblaasvent_10.string),            185, SIZE_10, "Afblaasventilator" },
  { sizeof(tekst.ebm_10.string),                    110, SIZE_10, "ebm" },

  { sizeof(tekst.Afblaasvent_aan_10.string),        185, SIZE_10, "Afblaasvent. aan" },
  { sizeof(tekst.Afblaasvent_uit_10.string),        185, SIZE_10, "Afblaasvent. uit" },
  { sizeof(tekst.Bovenklep_open_10.string),         185, SIZE_10, "Bovenklep open" },
  { sizeof(tekst.Bovenklep_dicht_10.string),        185, SIZE_10, "Bovenklep dicht" },

  //#ifdef CAN_BACKBONE_PC_WARNING
  { sizeof(tekst.CAN_PC_1_Offline_10.string),       185, SIZE_10, "CAN PC 1 offline" },   // CAN_PC_1_Offline_10
  { sizeof(tekst.CAN_PC_2_Offline_10.string),       185, SIZE_10, "CAN PC 2 offline" },   // CAN_PC_2_Offline_10
  { sizeof(tekst.CAN_PC_3_Offline_10.string),       185, SIZE_10, "CAN PC 3 offline" },   // CAN_PC_3_Offline_10
  //#endif // CAN_BACKBONE_PC_WARNING

  { sizeof(tekst.Display_14.string),                 90, SIZE_14, "Display" },             // Display_14
  { sizeof(tekst.Groen_14.string),                   90, SIZE_14, "Green" },               // Groen_14
  { sizeof(tekst.Wit_14.string),                     90, SIZE_14, "White" },               // Wit_14

  { sizeof(tekst.Modules_14.string),                180, SIZE_14, "Modules" }, // Modules_14

  { sizeof(tekst.General_Error_10.string),             185, SIZE_10, "General error" },
  { sizeof(tekst.Motor_Fault_10.string),               185, SIZE_10, "Motor fault" },
  { sizeof(tekst.Motor_Blocked_10.string),             185, SIZE_10, "Motor blocked" },
  { sizeof(tekst.Heat_Sink_Temperature_10.string),     185, SIZE_10, "Heat sink temperature" },
  { sizeof(tekst.Ground_Fault_10.string),              185, SIZE_10, "Ground fault" },
  { sizeof(tekst.Hall_IC_Fault_10.string),             185, SIZE_10, "Hall IC fault" },
  { sizeof(tekst.Overcurrent_10.string),               185, SIZE_10, "Overcurrent" },
  { sizeof(tekst.Line_Fault_10.string),                185, SIZE_10, "Line fault" },
  { sizeof(tekst.Int_Heat_Sink_Sensor_10.string),      185, SIZE_10, "Int heat sink sensor" },
  { sizeof(tekst.DC_Res_Voltage_To_High_10.string),    185, SIZE_10, "DC res voltage to high" },
  { sizeof(tekst.Temperature_Lowering_10.string),      185, SIZE_10, "Temperature lowering" },
  { sizeof(tekst.Wrong_Connection_10.string),          185, SIZE_10, "Wrong connection" },
  { sizeof(tekst.External_Fault_10.string),            185, SIZE_10, "External fault" },
  { sizeof(tekst.Factory_Settings_10.string),          185, SIZE_10, "Factory settings" },
  { sizeof(tekst.EEP_Error_10.string),                 185, SIZE_10, "EEP error" },
  { sizeof(tekst.RTC_General_Fault_10.string),         185, SIZE_10, "RTC general fault" },
  { sizeof(tekst.RTC_Voltage_Fault_10.string),         185, SIZE_10, "RTC voltage fault" },
  { sizeof(tekst.Filter_Contamination_10.string),      185, SIZE_10, "Filter contamination" },
  { sizeof(tekst.Transfer_Error_10.string),            185, SIZE_10, "Transfer error" },
  { sizeof(tekst.Data_Connetion_Line_10.string),       185, SIZE_10, "Data connection line" },
  { sizeof(tekst.Data_Connection_Checksum_10.string),  185, SIZE_10, "Data connection checksum" },
  { sizeof(tekst.Sensor_Fault_Input_1_10.string),      185, SIZE_10, "Sensor fault input 1" },
  { sizeof(tekst.Sensor_Fault_Input_2_10.string),      185, SIZE_10, "Sensor fault input 2" },
  { sizeof(tekst.Sensor_Fault_Input_3_10.string),      185, SIZE_10, "Sensor fault input 3" },
  { sizeof(tekst.High_line_voltage_10.string),         185, SIZE_10, "High line voltage" },
  { sizeof(tekst.i_limit_10.string),                   185, SIZE_10, "Current limitation in mesh" },
  { sizeof(tekst.p_limit_10.string),                   185, SIZE_10, "Power limitation in mesh" },
  { sizeof(tekst.te_high_10.string),                   185, SIZE_10, "Output stage temperature high" },
  { sizeof(tekst.tm_high_10.string),                   185, SIZE_10, "Motor temperature high" },
  { sizeof(tekst.tei_high_10.string),                  185, SIZE_10, "Electronics interion temperature high" },
  { sizeof(tekst.uz_low_10.string),                    185, SIZE_10, "DC-Link voltage low" },
  { sizeof(tekst.n_low_10.string),                     185, SIZE_10, "Actual speed less then limit speed" },

  { sizeof(tekst.igbt_fault_10.string),                185, SIZE_10, "IGBT fault" },
  { sizeof(tekst.uzk_hi_10.string),                    185, SIZE_10, "Uzk hi" },
  { sizeof(tekst.uzk_lo_10.string),                    185, SIZE_10, "Uzk lo" },
  { sizeof(tekst.uin_hi_10.string),                    185, SIZE_10, "Uin hi" },
  { sizeof(tekst.uin_lo_10.string),                    185, SIZE_10, "Uin lo" },

  { sizeof(tekst.Klep_10.string),                      185, SIZE_10, "Flap" },
  { sizeof(tekst.Sensor_10.string),                    185, SIZE_10, "Sensor" },
  { sizeof(tekst.Drukverschil_1_10.string),            185, SIZE_10, "Drukverschil 1"  },
  { sizeof(tekst.Drukverschil_2_10.string),            185, SIZE_10, "Drukverschil 2"  },
  { sizeof(tekst.Drukverschil_3_10.string),            185, SIZE_10, "Drukverschil 3"  },
  { sizeof(tekst.Drukverschil_4_10.string),            185, SIZE_10, "Drukverschil 4"  },
  { sizeof(tekst.Drukverschil_5_10.string),            185, SIZE_10, "Drukverschil 5"  },
  { sizeof(tekst.Drukverschil_6_10.string),            185, SIZE_10, "Drukverschil 6"  },
  { sizeof(tekst.Drukverschil_7_10.string),            185, SIZE_10, "Drukverschil 7"  },
  { sizeof(tekst.Drukverschil_8_10.string),            185, SIZE_10, "Drukverschil 8"  },
  { sizeof(tekst.Drukverschil_9_10.string),            185, SIZE_10, "Drukverschil 9"  },
  { sizeof(tekst.Drukverschil_10_10.string),           185, SIZE_10, "Drukverschil 10" },
  { sizeof(tekst.Drukverschil_11_10.string),           185, SIZE_10, "Drukverschil 11" },
  { sizeof(tekst.Drukverschil_12_10.string),           185, SIZE_10, "Drukverschil 12" },
  { sizeof(tekst.Drukverschil_13_10.string),           185, SIZE_10, "Drukverschil 13" },
  { sizeof(tekst.Drukverschil_14_10.string),           185, SIZE_10, "Drukverschil 14" },
  { sizeof(tekst.Drukverschil_15_10.string),           185, SIZE_10, "Drukverschil 15" },
  { sizeof(tekst.Drukverschil_16_10.string),           185, SIZE_10, "Drukverschil 16" },

  { sizeof(tekst.Extern_alarm_10.string),              185, SIZE_10, "External alarm" },

  { sizeof(tekst.failure_power_section_10.string),     185, SIZE_10, "Failure power section" },
  { sizeof(tekst.umax_10.string),                      185, SIZE_10, "U > Umax" },
  { sizeof(tekst.umin_10.string),                      185, SIZE_10, "U < Umin" },
  { sizeof(tekst.overspeed_10.string),                 185, SIZE_10, "Overspeed" },
  { sizeof(tekst.locked_rotor_10.string),              185, SIZE_10, "Locked rotor" },

  { sizeof(tekst.EEPROM_256k_10.string),               185, SIZE_10, "ONLY 256k EEPROM" },    
  { sizeof(tekst.Version_Error_10.string),             185, SIZE_10, "Version error" },  
  
  { sizeof(tekst.underspeed_10.string),                185, SIZE_10, "Underspeed" },
  { sizeof(tekst._24V_supply_overload_10.string),      185, SIZE_10, "24V supply overload" },   
  { sizeof(tekst.input_phase_error_10.string),         185, SIZE_10, "Input phase error" }, 
  { sizeof(tekst.motor_phase_error_10.string),         185, SIZE_10, "Motor phase error" },    
  { sizeof(tekst.memory_error_10.string),              185, SIZE_10, "Memory error" }, 
  { sizeof(tekst.short_circuit_10.string),             185, SIZE_10, "Short circuit" }, 
  { sizeof(tekst.loss_of_synchronism_10.string),       185, SIZE_10, "Loss of synchronism" }, 
  { sizeof(tekst.input_voltage_error_10.string),       185, SIZE_10, "Input voltage error" },
  { sizeof(tekst.input_relay_not_closed_10.string),    185, SIZE_10, "Input relay not closed" }, 
  { sizeof(tekst.high_starting_current_10.string),     185, SIZE_10, "High starting current" }, 
  
               
};

s_tekst const tekst_nederlands =
{
  sizeof(s_tekst), // unsinged int area_size;
  ORION,           // unsigned int computer;
  MULTI_CONNECT,   // unsigned int soort;
  VERSIE_TEKST,    // unsigned int versie_tekst;

  { sizeof(tekst.Gekozen_Taal_14.string),           150, SIZE_14, "Nederlands" },

  { sizeof(tekst.Motorgroepen_10.string),           174, SIZE_10, "Motorgroepen" },

  { sizeof(tekst.Groep_1_14.string),                185, SIZE_14, "Groep 1" },
  { sizeof(tekst.Groep_2_14.string),                185, SIZE_14, "Groep 2" },
  { sizeof(tekst.Groep_3_14.string),                185, SIZE_14, "Groep 3" },
  { sizeof(tekst.Groep_4_14.string),                185, SIZE_14, "Groep 4" },
  { sizeof(tekst.Groep_5_14.string),                185, SIZE_14, "Groep 5" },
  { sizeof(tekst.Groep_6_14.string),                185, SIZE_14, "Groep 6" },
  { sizeof(tekst.Groep_7_14.string),                185, SIZE_14, "Groep 7" },
  { sizeof(tekst.Groep_8_14.string),                185, SIZE_14, "Groep 8" },
  { sizeof(tekst.Groep_9_14.string),                185, SIZE_14, "Groep 9" },
  { sizeof(tekst.Groep_10_14.string),               185, SIZE_14, "Groep 10" },
  { sizeof(tekst.Groep_11_14.string),               185, SIZE_14, "Groep 11" },
  { sizeof(tekst.Groep_12_14.string),               185, SIZE_14, "Groep 12" },
  { sizeof(tekst.Groep_13_14.string),               185, SIZE_14, "Groep 13" },
  { sizeof(tekst.Groep_14_14.string),               185, SIZE_14, "Groep 14" },
  { sizeof(tekst.Groep_15_14.string),               185, SIZE_14, "Groep 15" },
  { sizeof(tekst.Groep_16_14.string),               185, SIZE_14, "Groep 16" },
  { sizeof(tekst.Groep_17_14.string),               185, SIZE_14, "Groep 17" },
  { sizeof(tekst.Groep_18_14.string),               185, SIZE_14, "Groep 18" },
  { sizeof(tekst.Groep_19_14.string),               185, SIZE_14, "Groep 19" },
  { sizeof(tekst.Groep_20_14.string),               185, SIZE_14, "Groep 20" },
  { sizeof(tekst.Groep_21_14.string),               185, SIZE_14, "Groep 21" },
  { sizeof(tekst.Groep_22_14.string),               185, SIZE_14, "Groep 22" },
  { sizeof(tekst.Groep_23_14.string),               185, SIZE_14, "Groep 23" },
  { sizeof(tekst.Groep_24_14.string),               185, SIZE_14, "Groep 24" },
  { sizeof(tekst.Groep_25_14.string),               185, SIZE_14, "Groep 25" },
  { sizeof(tekst.Groep_26_14.string),               185, SIZE_14, "Groep 26" },
  { sizeof(tekst.Groep_27_14.string),               185, SIZE_14, "Groep 27" },
  { sizeof(tekst.Groep_28_14.string),               185, SIZE_14, "Groep 28" },
  { sizeof(tekst.Groep_29_14.string),               185, SIZE_14, "Groep 29" },
  { sizeof(tekst.Groep_30_14.string),               185, SIZE_14, "Groep 30" },
  { sizeof(tekst.Groep_31_14.string),               185, SIZE_14, "Groep 31" },
  { sizeof(tekst.Groep_32_14.string),               185, SIZE_14, "Groep 32" },

  { sizeof(tekst.EXT_OPEN_10.string),                76, SIZE_10, "EXT. OPEN" },
  { sizeof(tekst.EXT_DICHT_10.string),               76, SIZE_10, "EXT. DICHT" },
  { sizeof(tekst.EXT_STOP_10.string),                76, SIZE_10, "EXT. STOP" },
  { sizeof(tekst.OPEN_10.string),                    60, SIZE_10, "OPEN" },
  { sizeof(tekst.DICHT_10.string),                   60, SIZE_10, "DICHT" },
  { sizeof(tekst.STOP_10.string),                    60, SIZE_10, "STOP" },
  { sizeof(tekst.HAND_10.string),                    60, SIZE_10, "HAND" },
  { sizeof(tekst.AUTO_10.string),                    60, SIZE_10, "AUTO" },
  { sizeof(tekst.UIT_10.string),                     60, SIZE_10, "UIT" },
  { sizeof(tekst.HAND_7.string),                     60, SIZE_7,  "HAND" },
  { sizeof(tekst.AUTO_7.string),                     60, SIZE_7,  "AUTO" },
  { sizeof(tekst.UIT_7.string),                      60, SIZE_7,  "UIT" },

  { sizeof(tekst.Zondag_10.string),                  80, SIZE_10, "Zondag" },
  { sizeof(tekst.Maandag_10.string),                 80, SIZE_10, "Maandag" },
  { sizeof(tekst.Dinsdag_10.string),                 80, SIZE_10, "Dinsdag" },
  { sizeof(tekst.Woensdag_10.string),                80, SIZE_10, "Woensdag" },
  { sizeof(tekst.Donderdag_10.string),               80, SIZE_10, "Donderdag" },
  { sizeof(tekst.Vrijdag_10.string),                 80, SIZE_10, "Vrijdag" },
  { sizeof(tekst.Zaterdag_10.string),                80, SIZE_10, "Zaterdag" },
  { sizeof(tekst.Tijd_en_Datum_14.string),          200, SIZE_14, "Tijd + Datum" },

  { sizeof(tekst.Fn_10.string),                      40, SIZE_10, "Fn.:" },
  { sizeof(tekst.Syst_10.string),                    40, SIZE_10, "Syst.:" },
  { sizeof(tekst.Diag_10.string),                    40, SIZE_10, "Diag.:" },

  { sizeof(tekst.Groep_1_10.string),                129, SIZE_10, "Groep 1" },
  { sizeof(tekst.Groep_2_10.string),                129, SIZE_10, "Groep 2" },
  { sizeof(tekst.Groep_3_10.string),                129, SIZE_10, "Groep 3" },
  { sizeof(tekst.Groep_4_10.string),                129, SIZE_10, "Groep 4" },
  { sizeof(tekst.Groep_5_10.string),                129, SIZE_10, "Groep 5" },
  { sizeof(tekst.Groep_6_10.string),                129, SIZE_10, "Groep 6" },
  { sizeof(tekst.Groep_7_10.string),                129, SIZE_10, "Groep 7" },
  { sizeof(tekst.Groep_8_10.string),                129, SIZE_10, "Groep 8" },
  { sizeof(tekst.Groep_9_10.string),                129, SIZE_10, "Groep 9" },
  { sizeof(tekst.Groep_10_10.string),               129, SIZE_10, "Groep 10" },
  { sizeof(tekst.Groep_11_10.string),               129, SIZE_10, "Groep 11" },
  { sizeof(tekst.Groep_12_10.string),               129, SIZE_10, "Groep 12" },
  { sizeof(tekst.Groep_13_10.string),               129, SIZE_10, "Groep 13" },
  { sizeof(tekst.Groep_14_10.string),               129, SIZE_10, "Groep 14" },
  { sizeof(tekst.Groep_15_10.string),               129, SIZE_10, "Groep 15" },
  { sizeof(tekst.Groep_16_10.string),               129, SIZE_10, "Groep 16" },
  { sizeof(tekst.Groep_17_10.string),               129, SIZE_10, "Groep 17" },
  { sizeof(tekst.Groep_18_10.string),               129, SIZE_10, "Groep 18" },
  { sizeof(tekst.Groep_19_10.string),               129, SIZE_10, "Groep 19" },
  { sizeof(tekst.Groep_20_10.string),               129, SIZE_10, "Groep 20" },
  { sizeof(tekst.Groep_21_10.string),               129, SIZE_10, "Groep 21" },
  { sizeof(tekst.Groep_22_10.string),               129, SIZE_10, "Groep 22" },
  { sizeof(tekst.Groep_23_10.string),               129, SIZE_10, "Groep 23" },
  { sizeof(tekst.Groep_24_10.string),               129, SIZE_10, "Groep 24" },
  { sizeof(tekst.Groep_25_10.string),               129, SIZE_10, "Groep 25" },
  { sizeof(tekst.Groep_26_10.string),               129, SIZE_10, "Groep 26" },
  { sizeof(tekst.Groep_27_10.string),               129, SIZE_10, "Groep 27" },
  { sizeof(tekst.Groep_28_10.string),               129, SIZE_10, "Groep 28" },
  { sizeof(tekst.Groep_29_10.string),               129, SIZE_10, "Groep 29" },
  { sizeof(tekst.Groep_30_10.string),               129, SIZE_10, "Groep 30" },
  { sizeof(tekst.Groep_31_10.string),               129, SIZE_10, "Groep 31" },
  { sizeof(tekst.Groep_32_10.string),               129, SIZE_10, "Groep 32" },

  { sizeof(tekst.Motor_1_10.string),                110, SIZE_10, "Motor 1" },
  { sizeof(tekst.Motor_2_10.string),                110, SIZE_10, "Motor 2" },
  { sizeof(tekst.Motor_3_10.string),                110, SIZE_10, "Motor 3" },
  { sizeof(tekst.Motor_4_10.string),                110, SIZE_10, "Motor 4" },
  { sizeof(tekst.Motor_5_10.string),                110, SIZE_10, "Motor 5" },
  { sizeof(tekst.Motor_6_10.string),                110, SIZE_10, "Motor 6" },
  { sizeof(tekst.Motor_7_10.string),                110, SIZE_10, "Motor 7" },
  { sizeof(tekst.Motor_8_10.string),                110, SIZE_10, "Motor 8" },
  { sizeof(tekst.Motor_9_10.string),                110, SIZE_10, "Motor 9" },
  { sizeof(tekst.Motor_10_10.string),               110, SIZE_10, "Motor 10" },
  { sizeof(tekst.Motor_11_10.string),               110, SIZE_10, "Motor 11" },
  { sizeof(tekst.Motor_12_10.string),               110, SIZE_10, "Motor 12" },
  { sizeof(tekst.Motor_13_10.string),               110, SIZE_10, "Motor 13" },
  { sizeof(tekst.Motor_14_10.string),               110, SIZE_10, "Motor 14" },
  { sizeof(tekst.Motor_15_10.string),               110, SIZE_10, "Motor 15" },
  { sizeof(tekst.Motor_16_10.string),               110, SIZE_10, "Motor 16" },
  { sizeof(tekst.Motor_17_10.string),               110, SIZE_10, "Motor 17" },
  { sizeof(tekst.Motor_18_10.string),               110, SIZE_10, "Motor 18" },
  { sizeof(tekst.Motor_19_10.string),               110, SIZE_10, "Motor 19" },
  { sizeof(tekst.Motor_20_10.string),               110, SIZE_10, "Motor 20" },
  { sizeof(tekst.Motor_21_10.string),               110, SIZE_10, "Motor 21" },
  { sizeof(tekst.Motor_22_10.string),               110, SIZE_10, "Motor 22" },
  { sizeof(tekst.Motor_23_10.string),               110, SIZE_10, "Motor 23" },
  { sizeof(tekst.Motor_24_10.string),               110, SIZE_10, "Motor 24" },
  { sizeof(tekst.Motor_25_10.string),               110, SIZE_10, "Motor 25" },
  { sizeof(tekst.Motor_26_10.string),               110, SIZE_10, "Motor 26" },
  { sizeof(tekst.Motor_27_10.string),               110, SIZE_10, "Motor 27" },
  { sizeof(tekst.Motor_28_10.string),               110, SIZE_10, "Motor 28" },
  { sizeof(tekst.Motor_29_10.string),               110, SIZE_10, "Motor 29" },
  { sizeof(tekst.Motor_30_10.string),               110, SIZE_10, "Motor 30" },
  { sizeof(tekst.Motor_31_10.string),               110, SIZE_10, "Motor 31" },
  { sizeof(tekst.Motor_32_10.string),               110, SIZE_10, "Motor 32" },
  { sizeof(tekst.Motor_33_10.string),               110, SIZE_10, "Motor 33" },
  { sizeof(tekst.Motor_34_10.string),               110, SIZE_10, "Motor 34" },
  { sizeof(tekst.Motor_35_10.string),               110, SIZE_10, "Motor 35" },
  { sizeof(tekst.Motor_36_10.string),               110, SIZE_10, "Motor 36" },
  { sizeof(tekst.Motor_37_10.string),               110, SIZE_10, "Motor 37" },
  { sizeof(tekst.Motor_38_10.string),               110, SIZE_10, "Motor 38" },
  { sizeof(tekst.Motor_39_10.string),               110, SIZE_10, "Motor 39" },
  { sizeof(tekst.Motor_40_10.string),               110, SIZE_10, "Motor 40" },
  { sizeof(tekst.Motor_41_10.string),               110, SIZE_10, "Motor 41" },
  { sizeof(tekst.Motor_42_10.string),               110, SIZE_10, "Motor 42" },
  { sizeof(tekst.Motor_43_10.string),               110, SIZE_10, "Motor 43" },
  { sizeof(tekst.Motor_44_10.string),               110, SIZE_10, "Motor 44" },
  { sizeof(tekst.Motor_45_10.string),               110, SIZE_10, "Motor 45" },
  { sizeof(tekst.Motor_46_10.string),               110, SIZE_10, "Motor 46" },
  { sizeof(tekst.Motor_47_10.string),               110, SIZE_10, "Motor 47" },
  { sizeof(tekst.Motor_48_10.string),               110, SIZE_10, "Motor 48" },
  { sizeof(tekst.Motor_49_10.string),               110, SIZE_10, "Motor 49" },
  { sizeof(tekst.Motor_50_10.string),               110, SIZE_10, "Motor 50" },
  { sizeof(tekst.Motor_51_10.string),               110, SIZE_10, "Motor 51" },
  { sizeof(tekst.Motor_52_10.string),               110, SIZE_10, "Motor 52" },
  { sizeof(tekst.Motor_53_10.string),               110, SIZE_10, "Motor 53" },
  { sizeof(tekst.Motor_54_10.string),               110, SIZE_10, "Motor 54" },
  { sizeof(tekst.Motor_55_10.string),               110, SIZE_10, "Motor 55" },
  { sizeof(tekst.Motor_56_10.string),               110, SIZE_10, "Motor 56" },
  { sizeof(tekst.Motor_57_10.string),               110, SIZE_10, "Motor 57" },
  { sizeof(tekst.Motor_58_10.string),               110, SIZE_10, "Motor 58" },
  { sizeof(tekst.Motor_59_10.string),               110, SIZE_10, "Motor 59" },
  { sizeof(tekst.Motor_60_10.string),               110, SIZE_10, "Motor 60" },
  { sizeof(tekst.Motor_61_10.string),               110, SIZE_10, "Motor 61" },
  { sizeof(tekst.Motor_62_10.string),               110, SIZE_10, "Motor 62" },
  { sizeof(tekst.Motor_63_10.string),               110, SIZE_10, "Motor 63" },
  { sizeof(tekst.Motor_64_10.string),               110, SIZE_10, "Motor 64" },

  { sizeof(tekst.Status_10.string),                 100, SIZE_10, "Status" },
  { sizeof(tekst.Bediening_10.string),              110, SIZE_10, "Bediening" },
  { sizeof(tekst.Positie_10.string),                110, SIZE_10, "Positie" },

  { sizeof(tekst.Alarm_HAND_10.string),             110, SIZE_10, "Alarm HAND" },
  { sizeof(tekst.Alarm_afwijking_10.string),        110, SIZE_10, "Alarm afwijking" },
  { sizeof(tekst.Alarm_urgent_10.string),           110, SIZE_10, "Alarm urgent" },
  { sizeof(tekst.Pulse_zone_10.string),             110, SIZE_10, "Puls zone" },
  { sizeof(tekst.Pulse_width_10.string),            110, SIZE_10, "Puls breedte" },
  { sizeof(tekst.Cycletime_10.string),              110, SIZE_10, "Cyclustijd" },
  { sizeof(tekst.Hysterese_10.string),              130, SIZE_10, "Hysterese" },
  { sizeof(tekst.Stapgrootte_10.string),            130, SIZE_10, "Stapgrootte" },                               
  { sizeof(tekst.Bandbreedte_10.string),            136, SIZE_10, "Bandbreedte" },                                      

  { sizeof(tekst.Twee_doeken_een_bed_1_10.string),  145, SIZE_10, "2 doeken op 1 bed [1]" },
  { sizeof(tekst.Twee_doeken_een_bed_2_10.string),  145, SIZE_10, "2 doeken op 1 bed [2]" },
  { sizeof(tekst.Twee_doeken_een_bed_3_10.string),  145, SIZE_10, "2 doeken op 1 bed [3]" },
  { sizeof(tekst.Twee_doeken_een_bed_4_10.string),  145, SIZE_10, "2 doeken op 1 bed [4]" },
  { sizeof(tekst.Twee_doeken_een_bed_5_10.string),  145, SIZE_10, "2 doeken op 1 bed [5]" },
  { sizeof(tekst.Twee_doeken_een_bed_6_10.string),  145, SIZE_10, "2 doeken op 1 bed [6]" },
  { sizeof(tekst.Twee_doeken_een_bed_7_10.string),  145, SIZE_10, "2 doeken op 1 bed [7]" },
  { sizeof(tekst.Twee_doeken_een_bed_8_10.string),  145, SIZE_10, "2 doeken op 1 bed [8]" },

  { sizeof(tekst.Kier_10.string),                   110, SIZE_10, "Kier" },
  { sizeof(tekst.Hysteresis_10.string),             110, SIZE_10, "Hysterese" },
  { sizeof(tekst.Standby_10.string),                110, SIZE_10, "Standby" },
  { sizeof(tekst.Voorloop_10.string),               110, SIZE_10, "Voorloop" },

  { sizeof(tekst.Time_10.string),                   129, SIZE_10, "Tijd" },
  { sizeof(tekst.Tijd_10.string),                   110, SIZE_10, "Tijd" },
  { sizeof(tekst.Datum_10.string),                   75, SIZE_10, "Datum" },
  { sizeof(tekst.Sync_Tijd_10.string),              150, SIZE_10, "Synchroniseer tijd" },
  { sizeof(tekst.Dagenteller_10.string),            140, SIZE_10, "Dagenteller" },

  { sizeof(tekst.F2_10.string),                     174, SIZE_10, "Ventilatie" },

  { sizeof(tekst.F3_10.string),                     174, SIZE_10, "Sensoren" },

  { sizeof(tekst.Alarmen_10.string),                174, SIZE_10, "Alarmen" },
  { sizeof(tekst.Alarmen_Actief_10.string),         174, SIZE_10, "Alarmen Actief" },
  { sizeof(tekst.Alarmen_Historie_10.string),       174, SIZE_10, "Alarmen Historie" },
  { sizeof(tekst.Alarmen_Wissen_10.string),         174, SIZE_10, "Alarmen Wissen" },
  { sizeof(tekst.Alarm_10.string),                  174, SIZE_10, "Alarm" },
  { sizeof(tekst.Waarschuwing_10.string),           185, SIZE_10, "Waarschuwing" },
  { sizeof(tekst.Alarm_Systeem_10.string),          185, SIZE_10, "Alarm (Systeem)" },
  { sizeof(tekst.Waarschuwing_Syst_10.string),      185, SIZE_10, "Waarschuwing (Syst)" },
  { sizeof(tekst.Alarm_Computer_10.string),          40, SIZE_10, "Alarm" },
  { sizeof(tekst.Geen_Actief_Alarm_10.string),      185, SIZE_10, "Geen Actief Alarm" },
  { sizeof(tekst.Onbekend_Alarm_10.string),         185, SIZE_10, "Onbekend Alarm" },
  { sizeof(tekst.Opties_10.string),                 174, SIZE_10, "Opties" },
  { sizeof(tekst.Instellingen_10.string),           185, SIZE_10, "Instellingen" },
  { sizeof(tekst.Opties_En_Instellingen_10.string), 185, SIZE_10, "Opties + Instellingen" },
  { sizeof(tekst.Minimum_Maximum_10.string),        185, SIZE_10, "Minimum/Maximum" },
  { sizeof(tekst.Gewist_10.string),                 185, SIZE_10, "Gewist" },
  { sizeof(tekst.Terug_Gezet_10.string),            185, SIZE_10, "Terug Gezet" },
  { sizeof(tekst.Niet_Terug_Gezet_10.string),       185, SIZE_10, "Niet Terug Gezet" },
  { sizeof(tekst.I2C_10.string),                    185, SIZE_10, "I2C (EEPROM/RTC)" },
  { sizeof(tekst.EEPROM_10.string),                 185, SIZE_10, "EEPROM" },
  { sizeof(tekst.EEPROM_taal_10.string),            185, SIZE_10, "EEPROM (Taal)" },
  { sizeof(tekst.RTC_10.string),                    185, SIZE_10, "RTC" },
  { sizeof(tekst.TIMER_10.string),                  185, SIZE_10, "TIMER" },
  { sizeof(tekst.HTRAP_10.string),                  185, SIZE_10, "HTRAP" },
  { sizeof(tekst.PLL_10.string),                    185, SIZE_10, "PLL" },
  { sizeof(tekst.Opnieuw_Gestart_10.string),        185, SIZE_10, "Opnieuw Gestart" },
  { sizeof(tekst.Print_10.string),                   50, SIZE_10, "Print" },
  { sizeof(tekst.Niet_Gevonden_10.string),          185, SIZE_10, "Niet Gevonden" },
  { sizeof(tekst.ADC_10.string),                    185, SIZE_10, "ADC" },
  { sizeof(tekst.Analoge_Ingang_10.string),         150, SIZE_10, "Analoge Ingang" },
  { sizeof(tekst.Externe_24V_10.string),            185, SIZE_10, "Externe 24V" },                
  { sizeof(tekst.Orion_Uitgeschakeld_10.string),    185, SIZE_10, "Orion Uitgeschakeld" },
  { sizeof(tekst.Geen_Minimum_Alarm_10.string),     185, SIZE_10, "Geen Minimum Alarm" },
  { sizeof(tekst.Offline_10.string),                 80, SIZE_10, "offline" },
  { sizeof(tekst.Slave_10.string),                   80, SIZE_10, "Slave" },

  { sizeof(tekst.Diagnose_10.string),               174, SIZE_10, "Diagnose" },

  { sizeof(tekst.Info_10.string),                   132, SIZE_10, "Info" },
  { sizeof(tekst.Serie_Nr_14.string),                90, SIZE_14, "Serie Nr:" },
  { sizeof(tekst.Computer_10.string),                80, SIZE_10, "Computer:" },
  { sizeof(tekst.Computer_14.string),               100, SIZE_14, "Computer:" },

  { sizeof(tekst.Draaiuren_10.string),              110, SIZE_10, "Draaiuren" },
  { sizeof(tekst.Schakelingen_10.string),           110, SIZE_10, "Schakelingen" },
  { sizeof(tekst.Storingen_10.string),              110, SIZE_10, "Storingen" },
  { sizeof(tekst.Looptijd_10.string),               110, SIZE_10, "Looptijd" },
  { sizeof(tekst.Alarm_Code_10.string),             110, SIZE_10, "Alarm Code" },

  { sizeof(tekst.Bewerken_Tekst_10.string),         185, SIZE_10, "Bewerken Tekst" },

  { sizeof(tekst.kg_10.string),                      16, SIZE_10, "kg" },
  { sizeof(tekst.kg_14.string),                      19, SIZE_14, "kg" },
  { sizeof(tekst.l_10.string),                       16, SIZE_10, "L" },
  { sizeof(tekst.l_14.string),                       19, SIZE_14, "L" },
  { sizeof(tekst.g_10.string),                       16, SIZE_10, "g" },
  { sizeof(tekst.ml_10.string),                      16, SIZE_10, "ml" },
  { sizeof(tekst.m_10.string),                       16, SIZE_10, "m" },
  { sizeof(tekst.s_10.string),                       16, SIZE_10, "s" },
  { sizeof(tekst.AAN_20.string),                     45, SIZE_20, "AAN" },
  { sizeof(tekst.UIT_20.string),                     45, SIZE_20, "UIT" },
  { sizeof(tekst.Ana_10.string),                     36, SIZE_10, "Ana" },
  { sizeof(tekst.Dig_10.string),                     36, SIZE_10, "Dig" },
  { sizeof(tekst.MC_10.string),                      36, SIZE_10, "-" },
  { sizeof(tekst.Geen_IO_Toegewezen_10.string),     185, SIZE_10, "Geen IO Toegewezen" },

  { sizeof(tekst.Password_10.string),               111, SIZE_10, "Password" },
  { sizeof(tekst.Password_14.string),               111, SIZE_14, "Password:" },
  { sizeof(tekst.Zeker_weten_7.string),             105, SIZE_7,  "Zeker weten?" },
  { sizeof(tekst.Wissen_10.string),                  65, SIZE_10, "Wissen!" },

  { sizeof(tekst.CAN_LOCAL_14.string),              185, SIZE_14, "CAN LOCAL" },
  { sizeof(tekst.Diag_CAN_LOCAL_10.string),         174, SIZE_10, "Diag.: CAN LOCAL" },
  { sizeof(tekst.TXOK_10.string),                    90, SIZE_10, "TXOK" },
  { sizeof(tekst.RXOK_10.string),                    90, SIZE_10, "RXOK" },
  { sizeof(tekst.TXOK_RXOK_10.string),               90, SIZE_10, "TXOK+RXOK" },
  { sizeof(tekst.BOFF_10.string),                   130, SIZE_10, "BOFF" },
  { sizeof(tekst.EWRN_10.string),                   130, SIZE_10, "EWRN" },
  { sizeof(tekst.Stuff_error_10.string),            130, SIZE_10, "Stuff error" },
  { sizeof(tekst.Form_error_10.string),             130, SIZE_10, "Form error" },
  { sizeof(tekst.Ack_error_10.string),              130, SIZE_10, "Ack error" },
  { sizeof(tekst.Bit1_error_10.string),             130, SIZE_10, "Bit1 error" },
  { sizeof(tekst.Bit0_busoff_error_10.string),      130, SIZE_10, "Bit0 busoff error" },
  { sizeof(tekst.Bit0_normal_error_10.string),      130, SIZE_10, "Bit0 normal error" },
  { sizeof(tekst.Crc_error_10.string),              130, SIZE_10, "Crc error" },
  { sizeof(tekst.perc_7.string),                      8, SIZE_7,  "%" },
  { sizeof(tekst.Pa_7.string),                       15, SIZE_7,  "Pa" },
  { sizeof(tekst.g_14.string),                       15, SIZE_14, "g" },
  { sizeof(tekst.ml_14.string),                      15, SIZE_14, "ml" },
  { sizeof(tekst.graden_celsius_7.string),           10, SIZE_7,  "°C" },
  { sizeof(tekst.graden_fahrenheid_7.string),        10, SIZE_7,  "°F" },
  { sizeof(tekst.graden_7.string),                    8, SIZE_7,  "°" },
  { sizeof(tekst.minuut_7.string),                    8, SIZE_7,  "min" },

  { sizeof(tekst.Geen_Alarm_Contact_10.string),     185, SIZE_10, "Geen Alarmcontact" },

  { sizeof(tekst.Handbediening_10.string),           185, SIZE_10, "Handbediening" },
  { sizeof(tekst.Emergency_switch_10.string),        185, SIZE_10, "Nood eindschakelaar" },
  { sizeof(tekst.Thermal_failure_close_10.string),   185, SIZE_10, "Thermische storing dicht" },
  { sizeof(tekst.Thermal_failure_open_10.string),    185, SIZE_10, "Thermische storing open" },
  { sizeof(tekst.Break_input_10.string),             185, SIZE_10, "Break input" },
  { sizeof(tekst.Speed_to_low_10.string),            185, SIZE_10, "Snelheid te laag" },
  { sizeof(tekst.Encoder_failure_10.string),         185, SIZE_10, "Fout signaal encoder" },
  { sizeof(tekst.Encoder_failure_A_10.string),       185, SIZE_10, "Fout encoder A-signaal" },
  { sizeof(tekst.Encoder_failure_B_10.string),       185, SIZE_10, "Fout encoder B-signaal" },
  { sizeof(tekst.No_feedback_10.string),             185, SIZE_10, "Geen terugmelding" },
  { sizeof(tekst.Not_enough_pulses_10.string),       185, SIZE_10, "Te weinig pulsen" },
  { sizeof(tekst.Pulses_to_fast_10.string),          185, SIZE_10, "Pulsen te snel" },
  { sizeof(tekst.Not_installed_10.string),           185, SIZE_10, "Niet ingeregeld" },
  { sizeof(tekst.Encoder_interference_10.string),    185, SIZE_10, "Encoder interferentie" },
  { sizeof(tekst.Dualscreen_not_possible_10.string), 185, SIZE_10, "Dualscreen niet mogelijk" },
  { sizeof(tekst.Installation_mode_10.string),       185, SIZE_10, "Installatie mode" },
  { sizeof(tekst.Deviation_position_10.string),      185, SIZE_10, "Positie wijkt af" },
  { sizeof(tekst.Sensor_hi_speed_10.string),         185, SIZE_10, "Sensor hoge snelheid" },
  { sizeof(tekst.Direction_not_defined_10.string),   185, SIZE_10, "Looprichting onbekend" },
  { sizeof(tekst.Limitswitches_not_equal_10.string), 185, SIZE_10, "Eindschakelaars niet gelijk" },
  { sizeof(tekst.Speed_not_equal_10.string),         185, SIZE_10, "Snelheid niet gelijk" },
  { sizeof(tekst.Multiple_master_10.string),         185, SIZE_10, "Meerdere masters" },
  { sizeof(tekst.Frequency_controller_10.string),    185, SIZE_10, "Frequentie regelaar" },
  { sizeof(tekst.Gelijkloop_beveiliging_10.string),  185, SIZE_10, "Gelijkloop beveiliging" },
  { sizeof(tekst.Limitswitch_not_reached_10.string), 185, SIZE_10, "Eindschakelaar niet bereikt" },
  { sizeof(tekst.Wrong_direction_10.string),         185, SIZE_10, "Draairichting verkeerd" },
  { sizeof(tekst.Link_unknown_10.string),            185, SIZE_10, "Koppeling niet ingesteld" },
  { sizeof(tekst.Motor_not_running_10.string),       185, SIZE_10, "Geen pulsen" },
  { sizeof(tekst.Position_not_reached_10.string),    185, SIZE_10, "Positie niet bereikt" },

  { sizeof(tekst.Locked_motor_10.string),              185, SIZE_10, "Locked motor" },
  { sizeof(tekst.Hall_failure_10.string),              185, SIZE_10, "Hall failure" },
  { sizeof(tekst.Thermal_motor_10.string),             185, SIZE_10, "Thermal overload motor" },
  { sizeof(tekst.Comm_error_master_slave_10.string),   185, SIZE_10, "Comm master/slave PIC" },
  { sizeof(tekst.Thermal_power_module_10.string),      185, SIZE_10, "Thermal power module" },
  { sizeof(tekst.Comm_error_remote_unit_10.string),    185, SIZE_10, "Comm error remote unit" },
  { sizeof(tekst.Phase_failure_10.string),             185, SIZE_10, "Phase failure" },
  { sizeof(tekst.Break_10.string),                     185, SIZE_10, "Break" },
  { sizeof(tekst.Low_line_voltage_10.string),          185, SIZE_10, "Low line voltage" },
  { sizeof(tekst.Low_DC_link_voltage_10.string),       185, SIZE_10, "Low DC-link voltage" },
  { sizeof(tekst.High_DC_link_voltage_10.string),      185, SIZE_10, "High DC-link voltage" },
  { sizeof(tekst.Driver_problem_10.string),            185, SIZE_10, "Driver problem" },
  { sizeof(tekst.Electronic_box_over_heat_10.string),  185, SIZE_10, "Electronic box over heat" },
  { sizeof(tekst.Excessive_DC_link_current_10.string), 185, SIZE_10, "Excessive DC-link current" },

  { sizeof(tekst.Vorstbeveiliging_10.string),          185, SIZE_10, "Vorstbeveiliging" },
  { sizeof(tekst.Algemeen_extern_alarm_10.string),     185, SIZE_10, "Algemeen extern alarm" },
  { sizeof(tekst.Drukverschil_bewaking_10.string),     185, SIZE_10, "Drukverschil bewaking" },

  { sizeof(tekst.JA_10.string),                      45, SIZE_10, "JA" },
  { sizeof(tekst.NEE_10.string),                     45, SIZE_10, "NEE" },

  { sizeof(tekst.CAN_RS232_10.string),               80, SIZE_10, "CAN-RS232" },

  { sizeof(tekst.CAN_BACKBONE_14.string),           185, SIZE_14, "CAN BACKBONE" },
  { sizeof(tekst.Diag_CAN_BACKBONE_10.string),      174, SIZE_10, "Diag.: CAN BACKBONE" },

  { sizeof(tekst.Wijzig_computer_type_14.string),   180, SIZE_14, "Module"/*"Wijzig computer type"*/ },
  { sizeof(tekst.Activeer_module_7.string),         105, SIZE_7,  "Activeer module" },
  { sizeof(tekst.Deactiveer_module_7.string),       105, SIZE_7,  "Deactiveer module" },
  { sizeof(tekst.Ongeldige_code_7.string),          105, SIZE_7,  "Ongeldige code!" },

  { sizeof(tekst.SD_CARD_10.string),                 90, SIZE_10, "SD-CARD" },
  { sizeof(tekst.SD_CARD_14.string),                185, SIZE_14, "SD-CARD" },
  { sizeof(tekst.Diag_SD_CARD_10.string),           174, SIZE_10, "Diag.: SD-CARD" },
  { sizeof(tekst.no_card_10.string),                 90, SIZE_10, "no card" },
  { sizeof(tekst.placed_10.string),                  90, SIZE_10, "placed" },
  { sizeof(tekst.read_write_10.string),              90, SIZE_10, "read/write" },
  { sizeof(tekst.read_only_10.string),               90, SIZE_10, "read only" },
  { sizeof(tekst.active_10.string),                  90, SIZE_10, "active" },
  { sizeof(tekst.ask_remove_10.string),              90, SIZE_10, "ask remove" },
  { sizeof(tekst.remove_10.string),                  90, SIZE_10, "remove" },
  { sizeof(tekst.SIZE_7.string),                     40, SIZE_7,  "SIZE" },

  { sizeof(tekst.Diag_COM1_USB_10.string),          185, SIZE_10, "Diag.: COM1/USB" },     // Diag_COM1_USB_10
  { sizeof(tekst.COM1_USB_10.string),                75, SIZE_10, "COM1/USB" },            // COM1_USB_10
  { sizeof(tekst.kBd_14.string),                     30, SIZE_14, "kBd" },                 // kBd_14
  { sizeof(tekst.TX_10.string),                      60, SIZE_10, "TX" },                  // TX_10
  { sizeof(tekst.RX_10.string),                      60, SIZE_10, "RX" },                  // RX_10
  { sizeof(tekst.TX_RX_10.string),                   60, SIZE_10, "TX+RX" },               // TX_RX_10
  { sizeof(tekst.AT_10.string),                      90, SIZE_10, "AT" },                  // AT_10
  { sizeof(tekst.Modem_Init_10.string),              90, SIZE_10, "Modem Init" },          // Modem_Init_10

  { sizeof(tekst.Diag_COM2_10.string),              185, SIZE_10, "Diag.: COM2" },         // Diag_COM2_10
  { sizeof(tekst.COM2_10.string),                    75, SIZE_10, "COM2" },                // COM2_10
  { sizeof(tekst.Modem_DCD_10.string),               90, SIZE_10, "Modem DCD" },           // Modem_DCD_10

  { sizeof(tekst.Diag_Ethernet_10.string),          185, SIZE_10, "Diag.: Ethernet" },     // Diag_Ethernet_10
  { sizeof(tekst.IP_10.string),                      50, SIZE_10, "IP" },                  // IP_10
  { sizeof(tekst.Mask_10.string),                    50, SIZE_10, "Mask" },                // Mask_10
  { sizeof(tekst.Gate_10.string),                    50, SIZE_10, "Gate" },                // Gate_10
  { sizeof(tekst.MAC_10.string),                     50, SIZE_10, "MAC" },                 // MAC_10
  { sizeof(tekst.PC_10.string),                      50, SIZE_10, "PC" },                  // PC_10
  { sizeof(tekst.NO_CONNECTION_14.string),          150, SIZE_14, "NO CONNECTION" },       // NO_CONNECTION_14
  { sizeof(tekst.RTX_10.string),                     60, SIZE_10, "RTX" },                 // RTX_10
  { sizeof(tekst.NO_ACK_10.string),                  60, SIZE_10, "NO_ACK" },              // NO_ACK_10
  { sizeof(tekst.CONNECT_10.string),                 60, SIZE_10, "CONNECT" },             // CONNECT_10
  { sizeof(tekst.Reset_Ethernet_10.string),         130, SIZE_10, "Reset Ethernet" },      // Reset_Ethernet_10
  { sizeof(tekst.Port_10.string),                    50, SIZE_10, "Port" },                // Port_10

  { sizeof(tekst.COM1_USB_14.string),               185, SIZE_14, "COM1/USB" },            // COM1_USB_14
  { sizeof(tekst.COM2_14.string),                   185, SIZE_14, "COM2" },                // COM2_14
  { sizeof(tekst.Ethernet_14.string),               185, SIZE_14, "Ethernet" },            // Ethernet_14

  { sizeof(tekst.Ventilator_10.string),             185, SIZE_10, "Ventilator" },
  { sizeof(tekst.Minimum_10.string),                185, SIZE_10, "Minimum" },
  { sizeof(tekst.Maximum_10.string),                185, SIZE_10, "Maximum" },
  { sizeof(tekst.Offset_10.string),                 185, SIZE_10, "Offset" },

  { sizeof(tekst.Maximum_rpm_10.string),            110, SIZE_10, "Maximum rpm" },
  { sizeof(tekst.Vermogen_10.string),               110, SIZE_10, "Vermogen" },
  { sizeof(tekst.Temp_motor_10.string),             110, SIZE_10, "Temp motor" },
  { sizeof(tekst.Temp_electronica_10.string),       110, SIZE_10, "Temp electronica" },
  { sizeof(tekst.Temp_power_module_10.string),      110, SIZE_10, "Temp power module" },
  { sizeof(tekst.Fabrieksinst_terugz_10.string),    110, SIZE_10, "Fabrieksinst. terugz." },
  { sizeof(tekst.Versie_10.string),                 110, SIZE_10, "Versie" },

  { sizeof(tekst.Reset_all_10.string),              110, SIZE_10, "Reset all" },

  { sizeof(tekst.V_10.string),                       60, SIZE_10, "V:" },
  { sizeof(tekst.N_10.string),                       60, SIZE_10, "N:" },
  { sizeof(tekst.Geheugen_256k_10.string),          185, SIZE_10, "ONLY 256k RAM" },      	  
  { sizeof(tekst.Ongeldige_update_10.string),       185, SIZE_10, "Ongeldige update" },
  { sizeof(tekst.Plaats_512k_EEPROM_10.string),     185, SIZE_10, "Plaats 512k EEPROM" },
  { sizeof(tekst.OK_is_opties_wissen_10.string),    185, SIZE_10, "OK = opties wissen" },

  { sizeof(tekst.Luchtmengkast_14.string),          185, SIZE_14, "Luchtmengkast" },
  { sizeof(tekst.Luchtmengkast_groep_14.string),    185, SIZE_14, "Luchtmengkast groep" },
  { sizeof(tekst.Luchtmengkast_10.string),          185, SIZE_10, "Luchtmengkast" },
  { sizeof(tekst.Luchtmengkast_groep_10.string),    185, SIZE_10, "Luchtmengkast groep" },
  { sizeof(tekst.Diag_Luchtmengkast_10.string),     185, SIZE_10, "Diag.: Luchtmengkast" },
  { sizeof(tekst.Fn_Luchtmengkast_10.string),       185, SIZE_10, "Fn.: Luchtmengkast" },
  { sizeof(tekst.Syst_Luchtmengkast_10.string),     185, SIZE_10, "Syst.: Luchtmengkast" },

  { sizeof(tekst.Binnenklep_10.string),             185, SIZE_10, "Binnenklep" },
  { sizeof(tekst.Buitenklep_10.string),             185, SIZE_10, "Buitenklep" },
  { sizeof(tekst.Recirculatieklep_10.string),       185, SIZE_10, "Recirculatieklep" },
  { sizeof(tekst.Bovenklep_10.string),              185, SIZE_10, "Bovenklep" },
  { sizeof(tekst.Verwarming_10.string),             185, SIZE_10, "Verwarming" },
  { sizeof(tekst.Inblaasvent_10.string),            185, SIZE_10, "Inblaasventilator" },
  { sizeof(tekst.Afblaasvent_10.string),            185, SIZE_10, "Afblaasventilator" },
  { sizeof(tekst.ebm_10.string),                    110, SIZE_10, "ebm" },

  { sizeof(tekst.Afblaasvent_aan_10.string),        185, SIZE_10, "Afblaasvent. aan" },
  { sizeof(tekst.Afblaasvent_uit_10.string),        185, SIZE_10, "Afblaasvent. uit" },
  { sizeof(tekst.Bovenklep_open_10.string),         185, SIZE_10, "Bovenklep open" },
  { sizeof(tekst.Bovenklep_dicht_10.string),        185, SIZE_10, "Bovenklep dicht" },

  //#ifdef CAN_BACKBONE_PC_WARNING
  { sizeof(tekst.CAN_PC_1_Offline_10.string),        85, SIZE_10, "CAN PC 1 offline" },   // CAN_PC_1_Offline_10
  { sizeof(tekst.CAN_PC_2_Offline_10.string),        85, SIZE_10, "CAN PC 2 offline" },   // CAN_PC_2_Offline_10
  { sizeof(tekst.CAN_PC_3_Offline_10.string),        85, SIZE_10, "CAN PC 3 offline" },   // CAN_PC_3_Offline_10
  //#endif // CAN_BACKBONE_PC_WARNING

  { sizeof(tekst.Display_14.string),                 90, SIZE_14, "Display" },             // Display_14
  { sizeof(tekst.Groen_14.string),                   90, SIZE_14, "Groen" },               // Groen_14
  { sizeof(tekst.Wit_14.string),                     90, SIZE_14, "Wit" },                 // Wit_14

  { sizeof(tekst.Modules_14.string),                180, SIZE_14, "Modules" }, // Modules_14

  { sizeof(tekst.General_Error_10.string),             185, SIZE_10, "General error" },
  { sizeof(tekst.Motor_Fault_10.string),               185, SIZE_10, "Motor fault" },
  { sizeof(tekst.Motor_Blocked_10.string),             185, SIZE_10, "Motor blocked" },
  { sizeof(tekst.Heat_Sink_Temperature_10.string),     185, SIZE_10, "Heat sink temperature" },
  { sizeof(tekst.Ground_Fault_10.string),              185, SIZE_10, "Ground fault" },
  { sizeof(tekst.Hall_IC_Fault_10.string),             185, SIZE_10, "Hall IC fault" },
  { sizeof(tekst.Overcurrent_10.string),               185, SIZE_10, "Overcurrent" },
  { sizeof(tekst.Line_Fault_10.string),                185, SIZE_10, "Line fault" },
  { sizeof(tekst.Int_Heat_Sink_Sensor_10.string),      185, SIZE_10, "Int heat sink sensor" },
  { sizeof(tekst.DC_Res_Voltage_To_High_10.string),    185, SIZE_10, "DC res voltage to high" },
  { sizeof(tekst.Temperature_Lowering_10.string),      185, SIZE_10, "Temperature lowering" },
  { sizeof(tekst.Wrong_Connection_10.string),          185, SIZE_10, "Wrong connection" },
  { sizeof(tekst.External_Fault_10.string),            185, SIZE_10, "External fault" },
  { sizeof(tekst.Factory_Settings_10.string),          185, SIZE_10, "Factory settings" },
  { sizeof(tekst.EEP_Error_10.string),                 185, SIZE_10, "EEP error" },
  { sizeof(tekst.RTC_General_Fault_10.string),         185, SIZE_10, "RTC general fault" },
  { sizeof(tekst.RTC_Voltage_Fault_10.string),         185, SIZE_10, "RTC voltage fault" },
  { sizeof(tekst.Filter_Contamination_10.string),      185, SIZE_10, "Filter contamination" },
  { sizeof(tekst.Transfer_Error_10.string),            185, SIZE_10, "Transfer error" },
  { sizeof(tekst.Data_Connetion_Line_10.string),       185, SIZE_10, "Data connection line" },
  { sizeof(tekst.Data_Connection_Checksum_10.string),  185, SIZE_10, "Data connection checksum" },
  { sizeof(tekst.Sensor_Fault_Input_1_10.string),      185, SIZE_10, "Sensor fault input 1" },
  { sizeof(tekst.Sensor_Fault_Input_2_10.string),      185, SIZE_10, "Sensor fault input 2" },
  { sizeof(tekst.Sensor_Fault_Input_3_10.string),      185, SIZE_10, "Sensor fault input 3" },
  { sizeof(tekst.High_line_voltage_10.string),         185, SIZE_10, "High line voltage" },
  { sizeof(tekst.i_limit_10.string),                   185, SIZE_10, "Current limitation in mesh" },
  { sizeof(tekst.p_limit_10.string),                   185, SIZE_10, "Power limitation in mesh" },
  { sizeof(tekst.te_high_10.string),                   185, SIZE_10, "Output stage temperature high" },
  { sizeof(tekst.tm_high_10.string),                   185, SIZE_10, "Motor temperature high" },
  { sizeof(tekst.tei_high_10.string),                  185, SIZE_10, "Electronics interion temperature high" },
  { sizeof(tekst.uz_low_10.string),                    185, SIZE_10, "DC-Link voltage low" },
  { sizeof(tekst.n_low_10.string),                     185, SIZE_10, "Actual speed less then limit speed" },

  { sizeof(tekst.igbt_fault_10.string),                185, SIZE_10, "IGBT fault" },
  { sizeof(tekst.uzk_hi_10.string),                    185, SIZE_10, "Uzk hi" },
  { sizeof(tekst.uzk_lo_10.string),                    185, SIZE_10, "Uzk lo" },
  { sizeof(tekst.uin_hi_10.string),                    185, SIZE_10, "Uin hi" },
  { sizeof(tekst.uin_lo_10.string),                    185, SIZE_10, "Uin lo" },

  { sizeof(tekst.Klep_10.string),                      185, SIZE_10, "Klep" },
  { sizeof(tekst.Sensor_10.string),                    185, SIZE_10, "Sensor" },
  { sizeof(tekst.Drukverschil_1_10.string),            185, SIZE_10, "Drukverschil 1"  },
  { sizeof(tekst.Drukverschil_2_10.string),            185, SIZE_10, "Drukverschil 2"  },
  { sizeof(tekst.Drukverschil_3_10.string),            185, SIZE_10, "Drukverschil 3"  },
  { sizeof(tekst.Drukverschil_4_10.string),            185, SIZE_10, "Drukverschil 4"  },
  { sizeof(tekst.Drukverschil_5_10.string),            185, SIZE_10, "Drukverschil 5"  },
  { sizeof(tekst.Drukverschil_6_10.string),            185, SIZE_10, "Drukverschil 6"  },
  { sizeof(tekst.Drukverschil_7_10.string),            185, SIZE_10, "Drukverschil 7"  },
  { sizeof(tekst.Drukverschil_8_10.string),            185, SIZE_10, "Drukverschil 8"  },
  { sizeof(tekst.Drukverschil_9_10.string),            185, SIZE_10, "Drukverschil 9"  },
  { sizeof(tekst.Drukverschil_10_10.string),           185, SIZE_10, "Drukverschil 10" },
  { sizeof(tekst.Drukverschil_11_10.string),           185, SIZE_10, "Drukverschil 11" },
  { sizeof(tekst.Drukverschil_12_10.string),           185, SIZE_10, "Drukverschil 12" },
  { sizeof(tekst.Drukverschil_13_10.string),           185, SIZE_10, "Drukverschil 13" },
  { sizeof(tekst.Drukverschil_14_10.string),           185, SIZE_10, "Drukverschil 14" },
  { sizeof(tekst.Drukverschil_15_10.string),           185, SIZE_10, "Drukverschil 15" },
  { sizeof(tekst.Drukverschil_16_10.string),           185, SIZE_10, "Drukverschil 16" },

  { sizeof(tekst.Extern_alarm_10.string),              185, SIZE_10, "Extern alarm" },

  { sizeof(tekst.failure_power_section_10.string),     185, SIZE_10, "Failure power section" },
  { sizeof(tekst.umax_10.string),                      185, SIZE_10, "U > Umax" },
  { sizeof(tekst.umin_10.string),                      185, SIZE_10, "U < Umin" },
  { sizeof(tekst.overspeed_10.string),                 185, SIZE_10, "Overspeed" },
  { sizeof(tekst.locked_rotor_10.string),              185, SIZE_10, "Locked rotor" },

  { sizeof(tekst.EEPROM_256k_10.string),               185, SIZE_10, "SLECHTS 256k EEPROM" },    
  { sizeof(tekst.Version_Error_10.string),             185, SIZE_10, "Versiefout" },

  { sizeof(tekst.underspeed_10.string),                185, SIZE_10, "Underspeed" },  
  { sizeof(tekst._24V_supply_overload_10.string),      185, SIZE_10, "24V supply overload" },
  { sizeof(tekst.input_phase_error_10.string),         185, SIZE_10, "Input phase error" }, 
  { sizeof(tekst.motor_phase_error_10.string),         185, SIZE_10, "Motor phase error" },
  { sizeof(tekst.memory_error_10.string),              185, SIZE_10, "Memory error" }, 
  { sizeof(tekst.short_circuit_10.string),             185, SIZE_10, "Short circuit" }, 
  { sizeof(tekst.loss_of_synchronism_10.string),       185, SIZE_10, "Loss of synchronism" }, 
  { sizeof(tekst.input_voltage_error_10.string),       185, SIZE_10, "Input voltage error" },
  { sizeof(tekst.input_relay_not_closed_10.string),    185, SIZE_10, "Input relay not closed" }, 
  { sizeof(tekst.high_starting_current_10.string),     185, SIZE_10, "High starting current" }, 
};

s_tekst const tekst_duits =
{
  sizeof(s_tekst), // unsinged int area_size;
  ORION,           // unsigned int computer;
  MULTI_CONNECT,   // unsigned int soort;
  VERSIE_TEKST,    // unsigned int versie_tekst;

  { sizeof(tekst.Gekozen_Taal_14.string),           150, SIZE_14, "Deutsch" },

  { sizeof(tekst.Motorgroepen_10.string),           174, SIZE_10, "Motorgruppen" },

  { sizeof(tekst.Groep_1_14.string),                185, SIZE_14, "Gruppe 1" },
  { sizeof(tekst.Groep_2_14.string),                185, SIZE_14, "Gruppe 2" },
  { sizeof(tekst.Groep_3_14.string),                185, SIZE_14, "Gruppe 3" },
  { sizeof(tekst.Groep_4_14.string),                185, SIZE_14, "Gruppe 4" },
  { sizeof(tekst.Groep_5_14.string),                185, SIZE_14, "Gruppe 5" },
  { sizeof(tekst.Groep_6_14.string),                185, SIZE_14, "Gruppe 6" },
  { sizeof(tekst.Groep_7_14.string),                185, SIZE_14, "Gruppe 7" },
  { sizeof(tekst.Groep_8_14.string),                185, SIZE_14, "Gruppe 8" },
  { sizeof(tekst.Groep_9_14.string),                185, SIZE_14, "Gruppe 9" },
  { sizeof(tekst.Groep_10_14.string),               185, SIZE_14, "Gruppe 10" },
  { sizeof(tekst.Groep_11_14.string),               185, SIZE_14, "Gruppe 11" },
  { sizeof(tekst.Groep_12_14.string),               185, SIZE_14, "Gruppe 12" },
  { sizeof(tekst.Groep_13_14.string),               185, SIZE_14, "Gruppe 13" },
  { sizeof(tekst.Groep_14_14.string),               185, SIZE_14, "Gruppe 14" },
  { sizeof(tekst.Groep_15_14.string),               185, SIZE_14, "Gruppe 15" },
  { sizeof(tekst.Groep_16_14.string),               185, SIZE_14, "Gruppe 16" },
  { sizeof(tekst.Groep_17_14.string),               185, SIZE_14, "Gruppe 17" },
  { sizeof(tekst.Groep_18_14.string),               185, SIZE_14, "Gruppe 18" },
  { sizeof(tekst.Groep_19_14.string),               185, SIZE_14, "Gruppe 19" },
  { sizeof(tekst.Groep_20_14.string),               185, SIZE_14, "Gruppe 20" },
  { sizeof(tekst.Groep_21_14.string),               185, SIZE_14, "Gruppe 21" },
  { sizeof(tekst.Groep_22_14.string),               185, SIZE_14, "Gruppe 22" },
  { sizeof(tekst.Groep_23_14.string),               185, SIZE_14, "Gruppe 23" },
  { sizeof(tekst.Groep_24_14.string),               185, SIZE_14, "Gruppe 24" },
  { sizeof(tekst.Groep_25_14.string),               185, SIZE_14, "Gruppe 25" },
  { sizeof(tekst.Groep_26_14.string),               185, SIZE_14, "Gruppe 26" },
  { sizeof(tekst.Groep_27_14.string),               185, SIZE_14, "Gruppe 27" },
  { sizeof(tekst.Groep_28_14.string),               185, SIZE_14, "Gruppe 28" },
  { sizeof(tekst.Groep_29_14.string),               185, SIZE_14, "Gruppe 29" },
  { sizeof(tekst.Groep_30_14.string),               185, SIZE_14, "Gruppe 30" },
  { sizeof(tekst.Groep_31_14.string),               185, SIZE_14, "Gruppe 31" },
  { sizeof(tekst.Groep_32_14.string),               185, SIZE_14, "Gruppe 32" },

  { sizeof(tekst.EXT_OPEN_10.string),                76, SIZE_10, "EXT. OPEN" },
  { sizeof(tekst.EXT_DICHT_10.string),               76, SIZE_10, "EXT. CLOSE" },
  { sizeof(tekst.EXT_STOP_10.string),                76, SIZE_10, "EXT. STOP" },
  { sizeof(tekst.OPEN_10.string),                    60, SIZE_10, "OPEN" },
  { sizeof(tekst.DICHT_10.string),                   60, SIZE_10, "CLOSE" },
  { sizeof(tekst.STOP_10.string),                    60, SIZE_10, "STOP" },
  { sizeof(tekst.HAND_10.string),                    60, SIZE_10, "HAND" },
  { sizeof(tekst.AUTO_10.string),                    60, SIZE_10, "AUTO" },
  { sizeof(tekst.UIT_10.string),                     60, SIZE_10, "AUS" },
  { sizeof(tekst.HAND_7.string),                     60, SIZE_7,  "HAND" },
  { sizeof(tekst.AUTO_7.string),                     60, SIZE_7,  "AUTO" },
  { sizeof(tekst.UIT_7.string),                      60, SIZE_7,  "AUS" },

  { sizeof(tekst.Zondag_10.string),                  80, SIZE_10, "Sonntag" },
  { sizeof(tekst.Maandag_10.string),                 80, SIZE_10, "Montag" },
  { sizeof(tekst.Dinsdag_10.string),                 80, SIZE_10, "Dienstag" },
  { sizeof(tekst.Woensdag_10.string),                80, SIZE_10, "Mittwoch" },
  { sizeof(tekst.Donderdag_10.string),               80, SIZE_10, "Donnerstag" },
  { sizeof(tekst.Vrijdag_10.string),                 80, SIZE_10, "Freitag" },
  { sizeof(tekst.Zaterdag_10.string),                80, SIZE_10, "Samstag" },
  { sizeof(tekst.Tijd_en_Datum_14.string),          200, SIZE_14, "Uhrzeit + Datum" },

  { sizeof(tekst.Fn_10.string),                      40, SIZE_10, "Fn.:" },
  { sizeof(tekst.Syst_10.string),                    40, SIZE_10, "Syst.:" },
  { sizeof(tekst.Diag_10.string),                    40, SIZE_10, "Diag.:" },

  { sizeof(tekst.Groep_1_10.string),                129, SIZE_10, "Gruppe 1" },
  { sizeof(tekst.Groep_2_10.string),                129, SIZE_10, "Gruppe 2" },
  { sizeof(tekst.Groep_3_10.string),                129, SIZE_10, "Gruppe 3" },
  { sizeof(tekst.Groep_4_10.string),                129, SIZE_10, "Gruppe 4" },
  { sizeof(tekst.Groep_5_10.string),                129, SIZE_10, "Gruppe 5" },
  { sizeof(tekst.Groep_6_10.string),                129, SIZE_10, "Gruppe 6" },
  { sizeof(tekst.Groep_7_10.string),                129, SIZE_10, "Gruppe 7" },
  { sizeof(tekst.Groep_8_10.string),                129, SIZE_10, "Gruppe 8" },
  { sizeof(tekst.Groep_9_10.string),                129, SIZE_10, "Gruppe 9" },
  { sizeof(tekst.Groep_10_10.string),               129, SIZE_10, "Gruppe 10" },
  { sizeof(tekst.Groep_11_10.string),               129, SIZE_10, "Gruppe 11" },
  { sizeof(tekst.Groep_12_10.string),               129, SIZE_10, "Gruppe 12" },
  { sizeof(tekst.Groep_13_10.string),               129, SIZE_10, "Gruppe 13" },
  { sizeof(tekst.Groep_14_10.string),               129, SIZE_10, "Gruppe 14" },
  { sizeof(tekst.Groep_15_10.string),               129, SIZE_10, "Gruppe 15" },
  { sizeof(tekst.Groep_16_10.string),               129, SIZE_10, "Gruppe 16" },
  { sizeof(tekst.Groep_17_10.string),               129, SIZE_10, "Gruppe 17" },
  { sizeof(tekst.Groep_18_10.string),               129, SIZE_10, "Gruppe 18" },
  { sizeof(tekst.Groep_19_10.string),               129, SIZE_10, "Gruppe 19" },
  { sizeof(tekst.Groep_20_10.string),               129, SIZE_10, "Gruppe 20" },
  { sizeof(tekst.Groep_21_10.string),               129, SIZE_10, "Gruppe 21" },
  { sizeof(tekst.Groep_22_10.string),               129, SIZE_10, "Gruppe 22" },
  { sizeof(tekst.Groep_23_10.string),               129, SIZE_10, "Gruppe 23" },
  { sizeof(tekst.Groep_24_10.string),               129, SIZE_10, "Gruppe 24" },
  { sizeof(tekst.Groep_25_10.string),               129, SIZE_10, "Gruppe 25" },
  { sizeof(tekst.Groep_26_10.string),               129, SIZE_10, "Gruppe 26" },
  { sizeof(tekst.Groep_27_10.string),               129, SIZE_10, "Gruppe 27" },
  { sizeof(tekst.Groep_28_10.string),               129, SIZE_10, "Gruppe 28" },
  { sizeof(tekst.Groep_29_10.string),               129, SIZE_10, "Gruppe 29" },
  { sizeof(tekst.Groep_30_10.string),               129, SIZE_10, "Gruppe 30" },
  { sizeof(tekst.Groep_31_10.string),               129, SIZE_10, "Gruppe 31" },
  { sizeof(tekst.Groep_32_10.string),               129, SIZE_10, "Gruppe 32" },

  { sizeof(tekst.Motor_1_10.string),                110, SIZE_10, "Motor 1" },
  { sizeof(tekst.Motor_2_10.string),                110, SIZE_10, "Motor 2" },
  { sizeof(tekst.Motor_3_10.string),                110, SIZE_10, "Motor 3" },
  { sizeof(tekst.Motor_4_10.string),                110, SIZE_10, "Motor 4" },
  { sizeof(tekst.Motor_5_10.string),                110, SIZE_10, "Motor 5" },
  { sizeof(tekst.Motor_6_10.string),                110, SIZE_10, "Motor 6" },
  { sizeof(tekst.Motor_7_10.string),                110, SIZE_10, "Motor 7" },
  { sizeof(tekst.Motor_8_10.string),                110, SIZE_10, "Motor 8" },
  { sizeof(tekst.Motor_9_10.string),                110, SIZE_10, "Motor 9" },
  { sizeof(tekst.Motor_10_10.string),               110, SIZE_10, "Motor 10" },
  { sizeof(tekst.Motor_11_10.string),               110, SIZE_10, "Motor 11" },
  { sizeof(tekst.Motor_12_10.string),               110, SIZE_10, "Motor 12" },
  { sizeof(tekst.Motor_13_10.string),               110, SIZE_10, "Motor 13" },
  { sizeof(tekst.Motor_14_10.string),               110, SIZE_10, "Motor 14" },
  { sizeof(tekst.Motor_15_10.string),               110, SIZE_10, "Motor 15" },
  { sizeof(tekst.Motor_16_10.string),               110, SIZE_10, "Motor 16" },
  { sizeof(tekst.Motor_17_10.string),               110, SIZE_10, "Motor 17" },
  { sizeof(tekst.Motor_18_10.string),               110, SIZE_10, "Motor 18" },
  { sizeof(tekst.Motor_19_10.string),               110, SIZE_10, "Motor 19" },
  { sizeof(tekst.Motor_20_10.string),               110, SIZE_10, "Motor 20" },
  { sizeof(tekst.Motor_21_10.string),               110, SIZE_10, "Motor 21" },
  { sizeof(tekst.Motor_22_10.string),               110, SIZE_10, "Motor 22" },
  { sizeof(tekst.Motor_23_10.string),               110, SIZE_10, "Motor 23" },
  { sizeof(tekst.Motor_24_10.string),               110, SIZE_10, "Motor 24" },
  { sizeof(tekst.Motor_25_10.string),               110, SIZE_10, "Motor 25" },
  { sizeof(tekst.Motor_26_10.string),               110, SIZE_10, "Motor 26" },
  { sizeof(tekst.Motor_27_10.string),               110, SIZE_10, "Motor 27" },
  { sizeof(tekst.Motor_28_10.string),               110, SIZE_10, "Motor 28" },
  { sizeof(tekst.Motor_29_10.string),               110, SIZE_10, "Motor 29" },
  { sizeof(tekst.Motor_30_10.string),               110, SIZE_10, "Motor 30" },
  { sizeof(tekst.Motor_31_10.string),               110, SIZE_10, "Motor 31" },
  { sizeof(tekst.Motor_32_10.string),               110, SIZE_10, "Motor 32" },
  { sizeof(tekst.Motor_33_10.string),               110, SIZE_10, "Motor 33" },
  { sizeof(tekst.Motor_34_10.string),               110, SIZE_10, "Motor 34" },
  { sizeof(tekst.Motor_35_10.string),               110, SIZE_10, "Motor 35" },
  { sizeof(tekst.Motor_36_10.string),               110, SIZE_10, "Motor 36" },
  { sizeof(tekst.Motor_37_10.string),               110, SIZE_10, "Motor 37" },
  { sizeof(tekst.Motor_38_10.string),               110, SIZE_10, "Motor 38" },
  { sizeof(tekst.Motor_39_10.string),               110, SIZE_10, "Motor 39" },
  { sizeof(tekst.Motor_40_10.string),               110, SIZE_10, "Motor 40" },
  { sizeof(tekst.Motor_41_10.string),               110, SIZE_10, "Motor 41" },
  { sizeof(tekst.Motor_42_10.string),               110, SIZE_10, "Motor 42" },
  { sizeof(tekst.Motor_43_10.string),               110, SIZE_10, "Motor 43" },
  { sizeof(tekst.Motor_44_10.string),               110, SIZE_10, "Motor 44" },
  { sizeof(tekst.Motor_45_10.string),               110, SIZE_10, "Motor 45" },
  { sizeof(tekst.Motor_46_10.string),               110, SIZE_10, "Motor 46" },
  { sizeof(tekst.Motor_47_10.string),               110, SIZE_10, "Motor 47" },
  { sizeof(tekst.Motor_48_10.string),               110, SIZE_10, "Motor 48" },
  { sizeof(tekst.Motor_49_10.string),               110, SIZE_10, "Motor 49" },
  { sizeof(tekst.Motor_50_10.string),               110, SIZE_10, "Motor 50" },
  { sizeof(tekst.Motor_51_10.string),               110, SIZE_10, "Motor 51" },
  { sizeof(tekst.Motor_52_10.string),               110, SIZE_10, "Motor 52" },
  { sizeof(tekst.Motor_53_10.string),               110, SIZE_10, "Motor 53" },
  { sizeof(tekst.Motor_54_10.string),               110, SIZE_10, "Motor 54" },
  { sizeof(tekst.Motor_55_10.string),               110, SIZE_10, "Motor 55" },
  { sizeof(tekst.Motor_56_10.string),               110, SIZE_10, "Motor 56" },
  { sizeof(tekst.Motor_57_10.string),               110, SIZE_10, "Motor 57" },
  { sizeof(tekst.Motor_58_10.string),               110, SIZE_10, "Motor 58" },
  { sizeof(tekst.Motor_59_10.string),               110, SIZE_10, "Motor 59" },
  { sizeof(tekst.Motor_60_10.string),               110, SIZE_10, "Motor 60" },
  { sizeof(tekst.Motor_61_10.string),               110, SIZE_10, "Motor 61" },
  { sizeof(tekst.Motor_62_10.string),               110, SIZE_10, "Motor 62" },
  { sizeof(tekst.Motor_63_10.string),               110, SIZE_10, "Motor 63" },
  { sizeof(tekst.Motor_64_10.string),               110, SIZE_10, "Motor 64" },

  { sizeof(tekst.Status_10.string),                 100, SIZE_10, "Status" },
  { sizeof(tekst.Bediening_10.string),              110, SIZE_10, "Bedienung" },
  { sizeof(tekst.Positie_10.string),                110, SIZE_10, "Position" },

  { sizeof(tekst.Alarm_HAND_10.string),             110, SIZE_10, "Alarm HAND" },
  { sizeof(tekst.Alarm_afwijking_10.string),        110, SIZE_10, "Alarm Abweichung" },
  { sizeof(tekst.Alarm_urgent_10.string),           110, SIZE_10, "Alarm dringend" },
  { sizeof(tekst.Pulse_zone_10.string),             110, SIZE_10, "Pulszone" },
  { sizeof(tekst.Pulse_width_10.string),            110, SIZE_10, "Pulsbreite" },
  { sizeof(tekst.Cycletime_10.string),              110, SIZE_10, "Zykluszeit" },
  { sizeof(tekst.Hysterese_10.string),              130, SIZE_10, "Hysterese" },                                        
  { sizeof(tekst.Stapgrootte_10.string),            130, SIZE_10, "Schrittweite" },                               
  { sizeof(tekst.Bandbreedte_10.string),            136, SIZE_10, "Bandbreite" },                                      

  { sizeof(tekst.Twee_doeken_een_bed_1_10.string),  145, SIZE_10, "2 Schirme 1 Anlage [1]" },
  { sizeof(tekst.Twee_doeken_een_bed_2_10.string),  145, SIZE_10, "2 Schirme 1 Anlage [2]" },
  { sizeof(tekst.Twee_doeken_een_bed_3_10.string),  145, SIZE_10, "2 Schirme 1 Anlage [3]" },
  { sizeof(tekst.Twee_doeken_een_bed_4_10.string),  145, SIZE_10, "2 Schirme 1 Anlage [4]" },
  { sizeof(tekst.Twee_doeken_een_bed_5_10.string),  145, SIZE_10, "2 Schirme 1 Anlage [5]" },
  { sizeof(tekst.Twee_doeken_een_bed_6_10.string),  145, SIZE_10, "2 Schirme 1 Anlage [6]" },
  { sizeof(tekst.Twee_doeken_een_bed_7_10.string),  145, SIZE_10, "2 Schirme 1 Anlage [7]" },
  { sizeof(tekst.Twee_doeken_een_bed_8_10.string),  145, SIZE_10, "2 Schirme 1 Anlage [8]" },

  { sizeof(tekst.Kier_10.string),                   110, SIZE_10, "Abstand" },
  { sizeof(tekst.Hysteresis_10.string),             110, SIZE_10, "Hysterese" },
  { sizeof(tekst.Standby_10.string),                110, SIZE_10, "Standby" },
  { sizeof(tekst.Voorloop_10.string),               110, SIZE_10, "Vorlauf" },

  { sizeof(tekst.Time_10.string),                   129, SIZE_10, "Uhrzeit" },
  { sizeof(tekst.Tijd_10.string),                   110, SIZE_10, "Uhrzeit" },
  { sizeof(tekst.Datum_10.string),                   75, SIZE_10, "Datum" },
  { sizeof(tekst.Sync_Tijd_10.string),              150, SIZE_10, "Synchronisier Zeit" },
  { sizeof(tekst.Dagenteller_10.string),            140, SIZE_10, "Tageszähler" },

  { sizeof(tekst.F2_10.string),                     174, SIZE_10, "Ventilation" },

  { sizeof(tekst.F3_10.string),                     174, SIZE_10, "Sensors" },

  { sizeof(tekst.Alarmen_10.string),                174, SIZE_10, "Alarme" },
  { sizeof(tekst.Alarmen_Actief_10.string),         174, SIZE_10, "Alarme aktiv" },
  { sizeof(tekst.Alarmen_Historie_10.string),       174, SIZE_10, "Alarmverlauf" },
  { sizeof(tekst.Alarmen_Wissen_10.string),         174, SIZE_10, "Alarme löschen" },
  { sizeof(tekst.Alarm_10.string),                  174, SIZE_10, "Alarm" },
  { sizeof(tekst.Waarschuwing_10.string),           185, SIZE_10, "Warnung" },
  { sizeof(tekst.Alarm_Systeem_10.string),          185, SIZE_10, "Alarm (System)" },
  { sizeof(tekst.Waarschuwing_Syst_10.string),      185, SIZE_10, "Warnung (Syst)" },
  { sizeof(tekst.Alarm_Computer_10.string),          40, SIZE_10, "Alarm" },
  { sizeof(tekst.Geen_Actief_Alarm_10.string),      185, SIZE_10, "Kein Alarm aktiv" },
  { sizeof(tekst.Onbekend_Alarm_10.string),         185, SIZE_10, "Unbekannter Alarm" },
  { sizeof(tekst.Opties_10.string),                 174, SIZE_10, "Optionen" },
  { sizeof(tekst.Instellingen_10.string),           185, SIZE_10, "Einstellungen" },
  { sizeof(tekst.Opties_En_Instellingen_10.string), 185, SIZE_10, "Optionen + Einstellungen" },
  { sizeof(tekst.Minimum_Maximum_10.string),        185, SIZE_10, "Minimum/Maximum" },
  { sizeof(tekst.Gewist_10.string),                 185, SIZE_10, "gelöscht" },
  { sizeof(tekst.Terug_Gezet_10.string),            185, SIZE_10, "zurückgesetzt" },
  { sizeof(tekst.Niet_Terug_Gezet_10.string),       185, SIZE_10, "nicht zurückgesetzt" },
  { sizeof(tekst.I2C_10.string),                    185, SIZE_10, "I2C (EEPROM/RTC)" },
  { sizeof(tekst.EEPROM_10.string),                 185, SIZE_10, "EEPROM" },
  { sizeof(tekst.EEPROM_taal_10.string),            185, SIZE_10, "EEPROM (Sprache)" },
  { sizeof(tekst.RTC_10.string),                    185, SIZE_10, "RTC" },
  { sizeof(tekst.TIMER_10.string),                  185, SIZE_10, "TIMER" },
  { sizeof(tekst.HTRAP_10.string),                  185, SIZE_10, "HTRAP" },
  { sizeof(tekst.PLL_10.string),                    185, SIZE_10, "PLL" },
  { sizeof(tekst.Opnieuw_Gestart_10.string),        185, SIZE_10, "Neu gestartet" },
  { sizeof(tekst.Print_10.string),                   50, SIZE_10, "Platine" },
  { sizeof(tekst.Niet_Gevonden_10.string),          185, SIZE_10, "Nicht gefunden" },
  { sizeof(tekst.ADC_10.string),                    185, SIZE_10, "ADC" },
  { sizeof(tekst.Analoge_Ingang_10.string),         150, SIZE_10, "Analoger Eingang" },
  { sizeof(tekst.Externe_24V_10.string),            185, SIZE_10, "24V externe" },
  { sizeof(tekst.Orion_Uitgeschakeld_10.string),    185, SIZE_10, "Orion ausgeschaltet" },
  { sizeof(tekst.Geen_Minimum_Alarm_10.string),     185, SIZE_10, "Kein Min.-Alarm" },
  { sizeof(tekst.Offline_10.string),                 80, SIZE_10, "offline" },
  { sizeof(tekst.Slave_10.string),                   80, SIZE_10, "Slave" },

  { sizeof(tekst.Diagnose_10.string),               174, SIZE_10, "Diagnose" },

  { sizeof(tekst.Info_10.string),                   132, SIZE_10, "Info" },
  { sizeof(tekst.Serie_Nr_14.string),                90, SIZE_14, "Seriennr:" },
  { sizeof(tekst.Computer_10.string),                80, SIZE_10, "Computer:" },
  { sizeof(tekst.Computer_14.string),               100, SIZE_14, "Computer:" },

  { sizeof(tekst.Draaiuren_10.string),              110, SIZE_10, "Betriebsstunden" },
  { sizeof(tekst.Schakelingen_10.string),           110, SIZE_10, "Schaltvorgänge" },
  { sizeof(tekst.Storingen_10.string),              110, SIZE_10, "Störungen" },
  { sizeof(tekst.Looptijd_10.string),               110, SIZE_10, "Laufzeit" },
  { sizeof(tekst.Alarm_Code_10.string),             110, SIZE_10, "Alarmcode" },

  { sizeof(tekst.Bewerken_Tekst_10.string),         185, SIZE_10, "Text bearbeiten" },

  { sizeof(tekst.kg_10.string),                      16, SIZE_10, "kg" },
  { sizeof(tekst.kg_14.string),                      19, SIZE_14, "kg" },
  { sizeof(tekst.l_10.string),                       16, SIZE_10, "L" },
  { sizeof(tekst.l_14.string),                       19, SIZE_14, "L" },
  { sizeof(tekst.g_10.string),                       16, SIZE_10, "g" },
  { sizeof(tekst.ml_10.string),                      16, SIZE_10, "ml" },
  { sizeof(tekst.m_10.string),                       16, SIZE_10, "m" },
  { sizeof(tekst.s_10.string),                       16, SIZE_10, "s" },
  { sizeof(tekst.AAN_20.string),                     45, SIZE_20, "EIN" },
  { sizeof(tekst.UIT_20.string),                     45, SIZE_20, "AUS" },
  { sizeof(tekst.Ana_10.string),                     36, SIZE_10, "Ana" },
  { sizeof(tekst.Dig_10.string),                     36, SIZE_10, "Dig" },
  { sizeof(tekst.MC_10.string),                      36, SIZE_10, "-" },
  { sizeof(tekst.Geen_IO_Toegewezen_10.string),     185, SIZE_10, "Kein IO zugewiesen" },

  { sizeof(tekst.Password_10.string),               111, SIZE_10, "Zugangscode" },
  { sizeof(tekst.Password_14.string),               111, SIZE_14, "Zugangscode:" },
  { sizeof(tekst.Zeker_weten_7.string),             105, SIZE_7,  "Sind Sie sicher?" },
  { sizeof(tekst.Wissen_10.string),                  65, SIZE_10, "Löschen!" },

  { sizeof(tekst.CAN_LOCAL_14.string),              185, SIZE_14, "CAN LOCAL" },
  { sizeof(tekst.Diag_CAN_LOCAL_10.string),         174, SIZE_10, "Diag.: CAN LOCAL" },
  { sizeof(tekst.TXOK_10.string),                    90, SIZE_10, "TXOK" },
  { sizeof(tekst.RXOK_10.string),                    90, SIZE_10, "RXOK" },
  { sizeof(tekst.TXOK_RXOK_10.string),               90, SIZE_10, "TXOK+RXOK" },
  { sizeof(tekst.BOFF_10.string),                   130, SIZE_10, "BOFF" },
  { sizeof(tekst.EWRN_10.string),                   130, SIZE_10, "EWRN" },
  { sizeof(tekst.Stuff_error_10.string),            130, SIZE_10, "Stuff error" },
  { sizeof(tekst.Form_error_10.string),             130, SIZE_10, "Form error" },
  { sizeof(tekst.Ack_error_10.string),              130, SIZE_10, "Ack error" },
  { sizeof(tekst.Bit1_error_10.string),             130, SIZE_10, "Bit1 error" },
  { sizeof(tekst.Bit0_busoff_error_10.string),      130, SIZE_10, "Bit0 busoff error" },
  { sizeof(tekst.Bit0_normal_error_10.string),      130, SIZE_10, "Bit0 normal error" },
  { sizeof(tekst.Crc_error_10.string),              130, SIZE_10, "Crc error" },
  { sizeof(tekst.perc_7.string),                      8, SIZE_7,  "%" },
  { sizeof(tekst.Pa_7.string),                       15, SIZE_7,  "Pa" },
  { sizeof(tekst.g_14.string),                       15, SIZE_14, "g" },
  { sizeof(tekst.ml_14.string),                      15, SIZE_14, "ml" },
  { sizeof(tekst.graden_celsius_7.string),           10, SIZE_7,  "°C" },
  { sizeof(tekst.graden_fahrenheid_7.string),        10, SIZE_7,  "°F" },
  { sizeof(tekst.graden_7.string),                    8, SIZE_7,  "°" },
  { sizeof(tekst.minuut_7.string),                    8, SIZE_7,  "min" },

  { sizeof(tekst.Geen_Alarm_Contact_10.string),     185, SIZE_10, "Kein Alarmkontakt" },

  { sizeof(tekst.Handbediening_10.string),           185, SIZE_10, "Handbedienung" },
  { sizeof(tekst.Emergency_switch_10.string),        185, SIZE_10, "Not-Endschalter" },
  { sizeof(tekst.Thermal_failure_close_10.string),   185, SIZE_10, "Therm Störung Schließen" },
  { sizeof(tekst.Thermal_failure_open_10.string),    185, SIZE_10, "Therm Störung Öffnen" },
  { sizeof(tekst.Break_input_10.string),             185, SIZE_10, "Break input" },
  { sizeof(tekst.Speed_to_low_10.string),            185, SIZE_10, "Geschwind zu gering" },
  { sizeof(tekst.Encoder_failure_10.string),         185, SIZE_10, "Fehlersignal Encoder" },
  { sizeof(tekst.Encoder_failure_A_10.string),       185, SIZE_10, "Fehler Encoder A-signal" },
  { sizeof(tekst.Encoder_failure_B_10.string),       185, SIZE_10, "Fehler Encoder B-signal" },
  { sizeof(tekst.No_feedback_10.string),             185, SIZE_10, "Keine Rückmeldung" },
  { sizeof(tekst.Not_enough_pulses_10.string),       185, SIZE_10, "Zu wenige Pulse" },
  { sizeof(tekst.Pulses_to_fast_10.string),          185, SIZE_10, "Pulse zu schnell" },
  { sizeof(tekst.Not_installed_10.string),           185, SIZE_10, "Nicht justiert" },
  { sizeof(tekst.Encoder_interference_10.string),    185, SIZE_10, "Encoderstörung" },
  { sizeof(tekst.Dualscreen_not_possible_10.string), 185, SIZE_10, "2 Schirme nicht möglich" },
  { sizeof(tekst.Installation_mode_10.string),       185, SIZE_10, "Einrichtungsmodus" },
  { sizeof(tekst.Deviation_position_10.string),      185, SIZE_10, "Position weicht ab" },
  { sizeof(tekst.Sensor_hi_speed_10.string),         185, SIZE_10, "Sensor hohe Geschwind" },
  { sizeof(tekst.Direction_not_defined_10.string),   185, SIZE_10, "Laufrichtung unbekannt" },
  { sizeof(tekst.Limitswitches_not_equal_10.string), 185, SIZE_10, "Endschalter nicht gleich" },
  { sizeof(tekst.Speed_not_equal_10.string),         185, SIZE_10, "Differentz Geschwind" },
  { sizeof(tekst.Multiple_master_10.string),         185, SIZE_10, "Mehrere Masters" },
  { sizeof(tekst.Frequency_controller_10.string),    185, SIZE_10, "Frequenzregler" },
  { sizeof(tekst.Gelijkloop_beveiliging_10.string),  185, SIZE_10, "Gleichlaufsicherung" },
  { sizeof(tekst.Limitswitch_not_reached_10.string), 185, SIZE_10, "Endschalter nicht erreicht" },
  { sizeof(tekst.Wrong_direction_10.string),         185, SIZE_10, "Falsche Drehrichtung" },
  { sizeof(tekst.Link_unknown_10.string),            185, SIZE_10, "Kopplung nicht konfig" },
  { sizeof(tekst.Motor_not_running_10.string),       185, SIZE_10, "Keine Pulse" },
  { sizeof(tekst.Position_not_reached_10.string),    185, SIZE_10, "Position nicht erreicht" },

  { sizeof(tekst.Locked_motor_10.string),              185, SIZE_10, "Motor blockiert" },
  { sizeof(tekst.Hall_failure_10.string),              185, SIZE_10, "Hallsensorfehler" },
  { sizeof(tekst.Thermal_motor_10.string),             185, SIZE_10, "Motor überhitzt" },
  { sizeof(tekst.Comm_error_master_slave_10.string),   185, SIZE_10, "Comm Master/Slave PIC" },
  { sizeof(tekst.Thermal_power_module_10.string),      185, SIZE_10, "Endstufe überhitzt" },
  { sizeof(tekst.Comm_error_remote_unit_10.string),    185, SIZE_10, "Comm error remote unit" },
  { sizeof(tekst.Phase_failure_10.string),             185, SIZE_10, "Phasenausfall" },
  { sizeof(tekst.Break_10.string),                     185, SIZE_10, "Bremsbetrieb" },
  { sizeof(tekst.Low_line_voltage_10.string),          185, SIZE_10, "Zwischenkreisunterspann" },
  { sizeof(tekst.Low_DC_link_voltage_10.string),       185, SIZE_10, "Low DC-link voltage" },
  { sizeof(tekst.High_DC_link_voltage_10.string),      185, SIZE_10, "High DC-link voltage" },
  { sizeof(tekst.Driver_problem_10.string),            185, SIZE_10, "Driver problem" },
  { sizeof(tekst.Electronic_box_over_heat_10.string),  185, SIZE_10, "Elektronikraum überhitzt" },
  { sizeof(tekst.Excessive_DC_link_current_10.string), 185, SIZE_10, "Excessive DC-link current" },

  { sizeof(tekst.Vorstbeveiliging_10.string),          185, SIZE_10, "Frostschutz" },
  { sizeof(tekst.Algemeen_extern_alarm_10.string),     185, SIZE_10, "Allgemeiner ext Alarm" },
  { sizeof(tekst.Drukverschil_bewaking_10.string),     185, SIZE_10, "Drukverschil bewaking" },

  { sizeof(tekst.JA_10.string),                      45, SIZE_10, "JA" },
  { sizeof(tekst.NEE_10.string),                     45, SIZE_10, "NEIN" },

  { sizeof(tekst.CAN_RS232_10.string),               80, SIZE_10, "CAN-RS232" },

  { sizeof(tekst.CAN_BACKBONE_14.string),           185, SIZE_14, "CAN BACKBONE" },
  { sizeof(tekst.Diag_CAN_BACKBONE_10.string),      174, SIZE_10, "Diag.: CAN BACKBONE" },

  { sizeof(tekst.Wijzig_computer_type_14.string),   180, SIZE_14, "Modul"/*"Wijzig computer type"*/ },
  { sizeof(tekst.Activeer_module_7.string),         105, SIZE_7,  "Modul aktivieren" },
  { sizeof(tekst.Deactiveer_module_7.string),       105, SIZE_7,  "Modul deaktivieren" },
  { sizeof(tekst.Ongeldige_code_7.string),          105, SIZE_7,  "Üngültiger Code!" },

  { sizeof(tekst.SD_CARD_10.string),                 90, SIZE_10, "SD-CARD" },
  { sizeof(tekst.SD_CARD_14.string),                185, SIZE_14, "SD-CARD" },
  { sizeof(tekst.Diag_SD_CARD_10.string),           174, SIZE_10, "Diag.: SD-CARD" },
  { sizeof(tekst.no_card_10.string),                 90, SIZE_10, "no card" },
  { sizeof(tekst.placed_10.string),                  90, SIZE_10, "placed" },
  { sizeof(tekst.read_write_10.string),              90, SIZE_10, "read/write" },
  { sizeof(tekst.read_only_10.string),               90, SIZE_10, "read only" },
  { sizeof(tekst.active_10.string),                  90, SIZE_10, "activ" },
  { sizeof(tekst.ask_remove_10.string),              90, SIZE_10, "ask remove" },
  { sizeof(tekst.remove_10.string),                  90, SIZE_10, "remove" },
  { sizeof(tekst.SIZE_7.string),                     40, SIZE_7,  "SIZE" },

  { sizeof(tekst.Diag_COM1_USB_10.string),          185, SIZE_10, "Diag.: COM1/USB" },     // Diag_COM1_USB_10
  { sizeof(tekst.COM1_USB_10.string),                75, SIZE_10, "COM1/USB" },            // COM1_USB_10
  { sizeof(tekst.kBd_14.string),                     30, SIZE_14, "kBd" },                 // kBd_14
  { sizeof(tekst.TX_10.string),                      60, SIZE_10, "TX" },                  // TX_10
  { sizeof(tekst.RX_10.string),                      60, SIZE_10, "RX" },                  // RX_10
  { sizeof(tekst.TX_RX_10.string),                   60, SIZE_10, "TX+RX" },               // TX_RX_10
  { sizeof(tekst.AT_10.string),                      90, SIZE_10, "AT" },                  // AT_10
  { sizeof(tekst.Modem_Init_10.string),              90, SIZE_10, "Modem Init" },          // Modem_Init_10

  { sizeof(tekst.Diag_COM2_10.string),              185, SIZE_10, "Diag.: COM2" },         // Diag_COM2_10
  { sizeof(tekst.COM2_10.string),                    75, SIZE_10, "COM2" },                // COM2_10
  { sizeof(tekst.Modem_DCD_10.string),               90, SIZE_10, "Modem DCD" },           // Modem_DCD_10

  { sizeof(tekst.Diag_Ethernet_10.string),          185, SIZE_10, "Diag.: Ethernet" },     // Diag_Ethernet_10
  { sizeof(tekst.IP_10.string),                      50, SIZE_10, "IP" },                  // IP_10
  { sizeof(tekst.Mask_10.string),                    50, SIZE_10, "Mask" },                // Mask_10
  { sizeof(tekst.Gate_10.string),                    50, SIZE_10, "Gate" },                // Gate_10
  { sizeof(tekst.MAC_10.string),                     50, SIZE_10, "MAC" },                 // MAC_10
  { sizeof(tekst.PC_10.string),                      50, SIZE_10, "PC" },                  // PC_10
  { sizeof(tekst.NO_CONNECTION_14.string),          150, SIZE_14, "NO CONNECTION" },       // NO_CONNECTION_14
  { sizeof(tekst.RTX_10.string),                     60, SIZE_10, "RTX" },                 // RTX_10
  { sizeof(tekst.NO_ACK_10.string),                  60, SIZE_10, "NO_ACK" },              // NO_ACK_10
  { sizeof(tekst.CONNECT_10.string),                 60, SIZE_10, "CONNECT" },             // CONNECT_10
  { sizeof(tekst.Reset_Ethernet_10.string),         130, SIZE_10, "Reset Ethernet" },      // Reset_Ethernet_10
  { sizeof(tekst.Port_10.string),                    50, SIZE_10, "Port" },                // Port_10

  { sizeof(tekst.COM1_USB_14.string),               185, SIZE_14, "COM1/USB" },            // COM1_USB_14
  { sizeof(tekst.COM2_14.string),                   185, SIZE_14, "COM2" },                // COM2_14
  { sizeof(tekst.Ethernet_14.string),               185, SIZE_14, "Ethernet" },            // Ethernet_14

  { sizeof(tekst.Ventilator_10.string),             185, SIZE_10, "Ventilator" },
  { sizeof(tekst.Minimum_10.string),                185, SIZE_10, "Minimum" },
  { sizeof(tekst.Maximum_10.string),                185, SIZE_10, "Maximum" },
  { sizeof(tekst.Offset_10.string),                 185, SIZE_10, "Offset" },

  { sizeof(tekst.Maximum_rpm_10.string),            110, SIZE_10, "Maximale Drehzahl" },
  { sizeof(tekst.Vermogen_10.string),               110, SIZE_10, "Leistung" },
  { sizeof(tekst.Temp_motor_10.string),             110, SIZE_10, "Motortemperatur" },
  { sizeof(tekst.Temp_electronica_10.string),       110, SIZE_10, "Temp Elektronik" },
  { sizeof(tekst.Temp_power_module_10.string),      110, SIZE_10, "Modultemperatur" },
  { sizeof(tekst.Fabrieksinst_terugz_10.string),    110, SIZE_10, "Auf Werkseinst zurücks" },
  { sizeof(tekst.Versie_10.string),                 110, SIZE_10, "Version" },

  { sizeof(tekst.Reset_all_10.string),              110, SIZE_10, "Alle aufheben" },

  { sizeof(tekst.V_10.string),                       60, SIZE_10, "V:" },
  { sizeof(tekst.N_10.string),                       60, SIZE_10, "N:" },
  { sizeof(tekst.Geheugen_256k_10.string),          185, SIZE_10, "NUR 256k RAM" },      	  
  { sizeof(tekst.Ongeldige_update_10.string),       185, SIZE_10, "Ongeldige update" },
  { sizeof(tekst.Plaats_512k_EEPROM_10.string),     185, SIZE_10, "Plaats 512k EEPROM" },
  { sizeof(tekst.OK_is_opties_wissen_10.string),    185, SIZE_10, "OK = opties wissen" },

  { sizeof(tekst.Luchtmengkast_14.string),          185, SIZE_14, "Luftmischer" },
  { sizeof(tekst.Luchtmengkast_groep_14.string),    185, SIZE_14, "Luftmischergruppe" },
  { sizeof(tekst.Luchtmengkast_10.string),          185, SIZE_10, "Luftmischer" },
  { sizeof(tekst.Luchtmengkast_groep_10.string),    185, SIZE_10, "Luftmischergruppe" },
  { sizeof(tekst.Diag_Luchtmengkast_10.string),     185, SIZE_10, "Diag.: Luftmischer" },
  { sizeof(tekst.Fn_Luchtmengkast_10.string),       185, SIZE_10, "Fn.: Luftmischer" },
  { sizeof(tekst.Syst_Luchtmengkast_10.string),     185, SIZE_10, "Syst.: Luftmischer" },

  { sizeof(tekst.Binnenklep_10.string),             185, SIZE_10, "Innenklappe" },
  { sizeof(tekst.Buitenklep_10.string),             185, SIZE_10, "Außenklappe" },
  { sizeof(tekst.Recirculatieklep_10.string),       185, SIZE_10, "Rezirkulationsklappe" },
  { sizeof(tekst.Bovenklep_10.string),              185, SIZE_10, "Obere Klappe" },
  { sizeof(tekst.Verwarming_10.string),             185, SIZE_10, "Heizung" },
  { sizeof(tekst.Inblaasvent_10.string),            185, SIZE_10, "Einblasventilator" },
  { sizeof(tekst.Afblaasvent_10.string),            185, SIZE_10, "Ausblasventilator" },
  { sizeof(tekst.ebm_10.string),                    110, SIZE_10, "ebm" },

  { sizeof(tekst.Afblaasvent_aan_10.string),        185, SIZE_10, "Ausblasv. EIN" },
  { sizeof(tekst.Afblaasvent_uit_10.string),        185, SIZE_10, "Ausblasv. AUS" },
  { sizeof(tekst.Bovenklep_open_10.string),         185, SIZE_10, "Bovenklep open" },
  { sizeof(tekst.Bovenklep_dicht_10.string),        185, SIZE_10, "Bovenklep dicht" },

  //#ifdef CAN_BACKBONE_PC_WARNING
  { sizeof(tekst.CAN_PC_1_Offline_10.string),       185, SIZE_10, "CAN PC 1 offline" },   // CAN_PC_1_Offline_10
  { sizeof(tekst.CAN_PC_2_Offline_10.string),       185, SIZE_10, "CAN PC 2 offline" },   // CAN_PC_2_Offline_10
  { sizeof(tekst.CAN_PC_3_Offline_10.string),       185, SIZE_10, "CAN PC 3 offline" },   // CAN_PC_3_Offline_10
  //#endif // CAN_BACKBONE_PC_WARNING

  { sizeof(tekst.Display_14.string),                 90, SIZE_14, "Display" },             // Display_14
  { sizeof(tekst.Groen_14.string),                   90, SIZE_14, "Grün" },                // Groen_14
  { sizeof(tekst.Wit_14.string),                     90, SIZE_14, "Weiß" },               // Wit_14

  { sizeof(tekst.Modules_14.string),                180, SIZE_14, "Module" }, // Modules_14

  { sizeof(tekst.General_Error_10.string),             185, SIZE_10, "General error" },
  { sizeof(tekst.Motor_Fault_10.string),               185, SIZE_10, "Motor fault" },
  { sizeof(tekst.Motor_Blocked_10.string),             185, SIZE_10, "Motor blocked" },
  { sizeof(tekst.Heat_Sink_Temperature_10.string),     185, SIZE_10, "Heat sink temperature" },
  { sizeof(tekst.Ground_Fault_10.string),              185, SIZE_10, "Ground fault" },
  { sizeof(tekst.Hall_IC_Fault_10.string),             185, SIZE_10, "Hall IC fault" },
  { sizeof(tekst.Overcurrent_10.string),               185, SIZE_10, "Overcurrent" },
  { sizeof(tekst.Line_Fault_10.string),                185, SIZE_10, "Line fault" },
  { sizeof(tekst.Int_Heat_Sink_Sensor_10.string),      185, SIZE_10, "Int heat sink sensor" },
  { sizeof(tekst.DC_Res_Voltage_To_High_10.string),    185, SIZE_10, "DC res voltage to high" },
  { sizeof(tekst.Temperature_Lowering_10.string),      185, SIZE_10, "Temperature lowering" },
  { sizeof(tekst.Wrong_Connection_10.string),          185, SIZE_10, "Wrong connection" },
  { sizeof(tekst.External_Fault_10.string),            185, SIZE_10, "External fault" },
  { sizeof(tekst.Factory_Settings_10.string),          185, SIZE_10, "Factory settings" },
  { sizeof(tekst.EEP_Error_10.string),                 185, SIZE_10, "EEP error" },
  { sizeof(tekst.RTC_General_Fault_10.string),         185, SIZE_10, "RTC general fault" },
  { sizeof(tekst.RTC_Voltage_Fault_10.string),         185, SIZE_10, "RTC voltage fault" },
  { sizeof(tekst.Filter_Contamination_10.string),      185, SIZE_10, "Filter contamination" },
  { sizeof(tekst.Transfer_Error_10.string),            185, SIZE_10, "Transfer error" },
  { sizeof(tekst.Data_Connetion_Line_10.string),       185, SIZE_10, "Data connection line" },
  { sizeof(tekst.Data_Connection_Checksum_10.string),  185, SIZE_10, "Data connection checksum" },
  { sizeof(tekst.Sensor_Fault_Input_1_10.string),      185, SIZE_10, "Sensor fault input 1" },
  { sizeof(tekst.Sensor_Fault_Input_2_10.string),      185, SIZE_10, "Sensor fault input 2" },
  { sizeof(tekst.Sensor_Fault_Input_3_10.string),      185, SIZE_10, "Sensor fault input 3" },
  { sizeof(tekst.High_line_voltage_10.string),         185, SIZE_10, "Zwischenkreisüberspann" },
  { sizeof(tekst.i_limit_10.string),                   185, SIZE_10, "Current limitation in mesh" },
  { sizeof(tekst.p_limit_10.string),                   185, SIZE_10, "Power limitation in mesh" },
  { sizeof(tekst.te_high_10.string),                   185, SIZE_10, "Output stage temperature high" },
  { sizeof(tekst.tm_high_10.string),                   185, SIZE_10, "Motor temperature high" },
  { sizeof(tekst.tei_high_10.string),                  185, SIZE_10, "Electronics interion temperature high" },
  { sizeof(tekst.uz_low_10.string),                    185, SIZE_10, "DC-Link voltage low" },
  { sizeof(tekst.n_low_10.string),                     185, SIZE_10, "Actual speed less then limit speed" },

  { sizeof(tekst.igbt_fault_10.string),                185, SIZE_10, "IGBT fault" },
  { sizeof(tekst.uzk_hi_10.string),                    185, SIZE_10, "Uzk hi" },
  { sizeof(tekst.uzk_lo_10.string),                    185, SIZE_10, "Uzk lo" },
  { sizeof(tekst.uin_hi_10.string),                    185, SIZE_10, "Uin hi" },
  { sizeof(tekst.uin_lo_10.string),                    185, SIZE_10, "Uin lo" },

  { sizeof(tekst.Klep_10.string),                      185, SIZE_10, "Klep" },
  { sizeof(tekst.Sensor_10.string),                    185, SIZE_10, "Sensor" },
  { sizeof(tekst.Drukverschil_1_10.string),            185, SIZE_10, "Drukverschil 1"  },
  { sizeof(tekst.Drukverschil_2_10.string),            185, SIZE_10, "Drukverschil 2"  },
  { sizeof(tekst.Drukverschil_3_10.string),            185, SIZE_10, "Drukverschil 3"  },
  { sizeof(tekst.Drukverschil_4_10.string),            185, SIZE_10, "Drukverschil 4"  },
  { sizeof(tekst.Drukverschil_5_10.string),            185, SIZE_10, "Drukverschil 5"  },
  { sizeof(tekst.Drukverschil_6_10.string),            185, SIZE_10, "Drukverschil 6"  },
  { sizeof(tekst.Drukverschil_7_10.string),            185, SIZE_10, "Drukverschil 7"  },
  { sizeof(tekst.Drukverschil_8_10.string),            185, SIZE_10, "Drukverschil 8"  },
  { sizeof(tekst.Drukverschil_9_10.string),            185, SIZE_10, "Drukverschil 9"  },
  { sizeof(tekst.Drukverschil_10_10.string),           185, SIZE_10, "Drukverschil 10" },
  { sizeof(tekst.Drukverschil_11_10.string),           185, SIZE_10, "Drukverschil 11" },
  { sizeof(tekst.Drukverschil_12_10.string),           185, SIZE_10, "Drukverschil 12" },
  { sizeof(tekst.Drukverschil_13_10.string),           185, SIZE_10, "Drukverschil 13" },
  { sizeof(tekst.Drukverschil_14_10.string),           185, SIZE_10, "Drukverschil 14" },
  { sizeof(tekst.Drukverschil_15_10.string),           185, SIZE_10, "Drukverschil 15" },
  { sizeof(tekst.Drukverschil_16_10.string),           185, SIZE_10, "Drukverschil 16" },

  { sizeof(tekst.Extern_alarm_10.string),              185, SIZE_10, "External alarm" },

  { sizeof(tekst.failure_power_section_10.string),     185, SIZE_10, "Failure power section" },
  { sizeof(tekst.umax_10.string),                      185, SIZE_10, "U > Umax" },
  { sizeof(tekst.umin_10.string),                      185, SIZE_10, "U < Umin" },
  { sizeof(tekst.overspeed_10.string),                 185, SIZE_10, "Overspeed" },
  { sizeof(tekst.locked_rotor_10.string),              185, SIZE_10, "Locked rotor" },

  { sizeof(tekst.EEPROM_256k_10.string),               185, SIZE_10, "NUR 256k EEPROM" },
  { sizeof(tekst.Version_Error_10.string),             185, SIZE_10, "Versionsfehler" },

  { sizeof(tekst.underspeed_10.string),                185, SIZE_10, "Underspeed" },  
  { sizeof(tekst._24V_supply_overload_10.string),      185, SIZE_10, "24V supply overload" },
  { sizeof(tekst.input_phase_error_10.string),         185, SIZE_10, "Input phase error" }, 
  { sizeof(tekst.motor_phase_error_10.string),         185, SIZE_10, "Motor phase error" },
  { sizeof(tekst.memory_error_10.string),              185, SIZE_10, "Memory error" }, 
  { sizeof(tekst.short_circuit_10.string),             185, SIZE_10, "Short circuit" }, 
  { sizeof(tekst.loss_of_synchronism_10.string),       185, SIZE_10, "Loss of synchronism" }, 
  { sizeof(tekst.input_voltage_error_10.string),       185, SIZE_10, "Input voltage error" },
  { sizeof(tekst.input_relay_not_closed_10.string),    185, SIZE_10, "Input relay not closed" }, 
  { sizeof(tekst.high_starting_current_10.string),     185, SIZE_10, "High starting current" }, 
};

s_tekst const tekst_spaans =
{
  sizeof(s_tekst), // unsinged int area_size;
  ORION,           // unsigned int computer;
  MULTI_CONNECT,   // unsigned int soort;
  VERSIE_TEKST,    // unsigned int versie_tekst;

  { sizeof(tekst.Gekozen_Taal_14.string),           150, SIZE_14, "Español" },

  { sizeof(tekst.Motorgroepen_10.string),           174, SIZE_10, "Motorgroepen" },

  { sizeof(tekst.Groep_1_14.string),                185, SIZE_14, "Groep 1" },
  { sizeof(tekst.Groep_2_14.string),                185, SIZE_14, "Groep 2" },
  { sizeof(tekst.Groep_3_14.string),                185, SIZE_14, "Groep 3" },
  { sizeof(tekst.Groep_4_14.string),                185, SIZE_14, "Groep 4" },
  { sizeof(tekst.Groep_5_14.string),                185, SIZE_14, "Groep 5" },
  { sizeof(tekst.Groep_6_14.string),                185, SIZE_14, "Groep 6" },
  { sizeof(tekst.Groep_7_14.string),                185, SIZE_14, "Groep 7" },
  { sizeof(tekst.Groep_8_14.string),                185, SIZE_14, "Groep 8" },
  { sizeof(tekst.Groep_9_14.string),                185, SIZE_14, "Groep 9" },
  { sizeof(tekst.Groep_10_14.string),               185, SIZE_14, "Groep 10" },
  { sizeof(tekst.Groep_11_14.string),               185, SIZE_14, "Groep 11" },
  { sizeof(tekst.Groep_12_14.string),               185, SIZE_14, "Groep 12" },
  { sizeof(tekst.Groep_13_14.string),               185, SIZE_14, "Groep 13" },
  { sizeof(tekst.Groep_14_14.string),               185, SIZE_14, "Groep 14" },
  { sizeof(tekst.Groep_15_14.string),               185, SIZE_14, "Groep 15" },
  { sizeof(tekst.Groep_16_14.string),               185, SIZE_14, "Groep 16" },
  { sizeof(tekst.Groep_17_14.string),               185, SIZE_14, "Groep 17" },
  { sizeof(tekst.Groep_18_14.string),               185, SIZE_14, "Groep 18" },
  { sizeof(tekst.Groep_19_14.string),               185, SIZE_14, "Groep 19" },
  { sizeof(tekst.Groep_20_14.string),               185, SIZE_14, "Groep 20" },
  { sizeof(tekst.Groep_21_14.string),               185, SIZE_14, "Groep 21" },
  { sizeof(tekst.Groep_22_14.string),               185, SIZE_14, "Groep 22" },
  { sizeof(tekst.Groep_23_14.string),               185, SIZE_14, "Groep 23" },
  { sizeof(tekst.Groep_24_14.string),               185, SIZE_14, "Groep 24" },
  { sizeof(tekst.Groep_25_14.string),               185, SIZE_14, "Groep 25" },
  { sizeof(tekst.Groep_26_14.string),               185, SIZE_14, "Groep 26" },
  { sizeof(tekst.Groep_27_14.string),               185, SIZE_14, "Groep 27" },
  { sizeof(tekst.Groep_28_14.string),               185, SIZE_14, "Groep 28" },
  { sizeof(tekst.Groep_29_14.string),               185, SIZE_14, "Groep 29" },
  { sizeof(tekst.Groep_30_14.string),               185, SIZE_14, "Groep 30" },
  { sizeof(tekst.Groep_31_14.string),               185, SIZE_14, "Groep 31" },
  { sizeof(tekst.Groep_32_14.string),               185, SIZE_14, "Groep 32" },

  { sizeof(tekst.EXT_OPEN_10.string),                76, SIZE_10, "EXT. OPEN" },
  { sizeof(tekst.EXT_DICHT_10.string),               76, SIZE_10, "EXT. DICHT" },
  { sizeof(tekst.EXT_STOP_10.string),                76, SIZE_10, "EXT. STOP" },
  { sizeof(tekst.OPEN_10.string),                    60, SIZE_10, "OPEN" },
  { sizeof(tekst.DICHT_10.string),                   60, SIZE_10, "DICHT" },
  { sizeof(tekst.STOP_10.string),                    60, SIZE_10, "STOP" },
  { sizeof(tekst.HAND_10.string),                    60, SIZE_10, "HAND" },
  { sizeof(tekst.AUTO_10.string),                    60, SIZE_10, "AUTO" },
  { sizeof(tekst.UIT_10.string),                     60, SIZE_10, "UIT" },
  { sizeof(tekst.HAND_7.string),                     60, SIZE_7,  "HAND" },
  { sizeof(tekst.AUTO_7.string),                     60, SIZE_7,  "AUTO" },
  { sizeof(tekst.UIT_7.string),                      60, SIZE_7,  "UIT" },

  { sizeof(tekst.Zondag_10.string),                  80, SIZE_10, "Domingo" },
  { sizeof(tekst.Maandag_10.string),                 80, SIZE_10, "Lunes" },
  { sizeof(tekst.Dinsdag_10.string),                 80, SIZE_10, "Martes" },
  { sizeof(tekst.Woensdag_10.string),                80, SIZE_10, "Miércoles" },
  { sizeof(tekst.Donderdag_10.string),               80, SIZE_10, "Jueves" },
  { sizeof(tekst.Vrijdag_10.string),                 80, SIZE_10, "Viernes" },
  { sizeof(tekst.Zaterdag_10.string),                80, SIZE_10, "Sábado" },
  { sizeof(tekst.Tijd_en_Datum_14.string),          200, SIZE_14, "Hora + Fecha" },

  { sizeof(tekst.Fn_10.string),                      40, SIZE_10, "Fn.:" },
  { sizeof(tekst.Syst_10.string),                    40, SIZE_10, "Syst.:" },
  { sizeof(tekst.Diag_10.string),                    40, SIZE_10, "Diag.:" },

  { sizeof(tekst.Groep_1_10.string),                129, SIZE_10, "Group 1" },
  { sizeof(tekst.Groep_2_10.string),                129, SIZE_10, "Group 2" },
  { sizeof(tekst.Groep_3_10.string),                129, SIZE_10, "Group 3" },
  { sizeof(tekst.Groep_4_10.string),                129, SIZE_10, "Group 4" },
  { sizeof(tekst.Groep_5_10.string),                129, SIZE_10, "Group 5" },
  { sizeof(tekst.Groep_6_10.string),                129, SIZE_10, "Group 6" },
  { sizeof(tekst.Groep_7_10.string),                129, SIZE_10, "Group 7" },
  { sizeof(tekst.Groep_8_10.string),                129, SIZE_10, "Group 8" },
  { sizeof(tekst.Groep_9_10.string),                129, SIZE_10, "Group 9" },
  { sizeof(tekst.Groep_10_10.string),               129, SIZE_10, "Group 10" },
  { sizeof(tekst.Groep_11_10.string),               129, SIZE_10, "Group 11" },
  { sizeof(tekst.Groep_12_10.string),               129, SIZE_10, "Group 12" },
  { sizeof(tekst.Groep_13_10.string),               129, SIZE_10, "Group 13" },
  { sizeof(tekst.Groep_14_10.string),               129, SIZE_10, "Group 14" },
  { sizeof(tekst.Groep_15_10.string),               129, SIZE_10, "Group 15" },
  { sizeof(tekst.Groep_16_10.string),               129, SIZE_10, "Group 16" },
  { sizeof(tekst.Groep_17_10.string),               129, SIZE_10, "Group 17" },
  { sizeof(tekst.Groep_18_10.string),               129, SIZE_10, "Group 18" },
  { sizeof(tekst.Groep_19_10.string),               129, SIZE_10, "Group 19" },
  { sizeof(tekst.Groep_20_10.string),               129, SIZE_10, "Group 20" },
  { sizeof(tekst.Groep_21_10.string),               129, SIZE_10, "Group 21" },
  { sizeof(tekst.Groep_22_10.string),               129, SIZE_10, "Group 22" },
  { sizeof(tekst.Groep_23_10.string),               129, SIZE_10, "Group 23" },
  { sizeof(tekst.Groep_24_10.string),               129, SIZE_10, "Group 24" },
  { sizeof(tekst.Groep_25_10.string),               129, SIZE_10, "Group 25" },
  { sizeof(tekst.Groep_26_10.string),               129, SIZE_10, "Group 26" },
  { sizeof(tekst.Groep_27_10.string),               129, SIZE_10, "Group 27" },
  { sizeof(tekst.Groep_28_10.string),               129, SIZE_10, "Group 28" },
  { sizeof(tekst.Groep_29_10.string),               129, SIZE_10, "Group 29" },
  { sizeof(tekst.Groep_30_10.string),               129, SIZE_10, "Group 30" },
  { sizeof(tekst.Groep_31_10.string),               129, SIZE_10, "Group 31" },
  { sizeof(tekst.Groep_32_10.string),               129, SIZE_10, "Group 32" },

  { sizeof(tekst.Motor_1_10.string),                110, SIZE_10, "Motor 1" },
  { sizeof(tekst.Motor_2_10.string),                110, SIZE_10, "Motor 2" },
  { sizeof(tekst.Motor_3_10.string),                110, SIZE_10, "Motor 3" },
  { sizeof(tekst.Motor_4_10.string),                110, SIZE_10, "Motor 4" },
  { sizeof(tekst.Motor_5_10.string),                110, SIZE_10, "Motor 5" },
  { sizeof(tekst.Motor_6_10.string),                110, SIZE_10, "Motor 6" },
  { sizeof(tekst.Motor_7_10.string),                110, SIZE_10, "Motor 7" },
  { sizeof(tekst.Motor_8_10.string),                110, SIZE_10, "Motor 8" },
  { sizeof(tekst.Motor_9_10.string),                110, SIZE_10, "Motor 9" },
  { sizeof(tekst.Motor_10_10.string),               110, SIZE_10, "Motor 10" },
  { sizeof(tekst.Motor_11_10.string),               110, SIZE_10, "Motor 11" },
  { sizeof(tekst.Motor_12_10.string),               110, SIZE_10, "Motor 12" },
  { sizeof(tekst.Motor_13_10.string),               110, SIZE_10, "Motor 13" },
  { sizeof(tekst.Motor_14_10.string),               110, SIZE_10, "Motor 14" },
  { sizeof(tekst.Motor_15_10.string),               110, SIZE_10, "Motor 15" },
  { sizeof(tekst.Motor_16_10.string),               110, SIZE_10, "Motor 16" },
  { sizeof(tekst.Motor_17_10.string),               110, SIZE_10, "Motor 17" },
  { sizeof(tekst.Motor_18_10.string),               110, SIZE_10, "Motor 18" },
  { sizeof(tekst.Motor_19_10.string),               110, SIZE_10, "Motor 19" },
  { sizeof(tekst.Motor_20_10.string),               110, SIZE_10, "Motor 20" },
  { sizeof(tekst.Motor_21_10.string),               110, SIZE_10, "Motor 21" },
  { sizeof(tekst.Motor_22_10.string),               110, SIZE_10, "Motor 22" },
  { sizeof(tekst.Motor_23_10.string),               110, SIZE_10, "Motor 23" },
  { sizeof(tekst.Motor_24_10.string),               110, SIZE_10, "Motor 24" },
  { sizeof(tekst.Motor_25_10.string),               110, SIZE_10, "Motor 25" },
  { sizeof(tekst.Motor_26_10.string),               110, SIZE_10, "Motor 26" },
  { sizeof(tekst.Motor_27_10.string),               110, SIZE_10, "Motor 27" },
  { sizeof(tekst.Motor_28_10.string),               110, SIZE_10, "Motor 28" },
  { sizeof(tekst.Motor_29_10.string),               110, SIZE_10, "Motor 29" },
  { sizeof(tekst.Motor_30_10.string),               110, SIZE_10, "Motor 30" },
  { sizeof(tekst.Motor_31_10.string),               110, SIZE_10, "Motor 31" },
  { sizeof(tekst.Motor_32_10.string),               110, SIZE_10, "Motor 32" },
  { sizeof(tekst.Motor_33_10.string),               110, SIZE_10, "Motor 33" },
  { sizeof(tekst.Motor_34_10.string),               110, SIZE_10, "Motor 34" },
  { sizeof(tekst.Motor_35_10.string),               110, SIZE_10, "Motor 35" },
  { sizeof(tekst.Motor_36_10.string),               110, SIZE_10, "Motor 36" },
  { sizeof(tekst.Motor_37_10.string),               110, SIZE_10, "Motor 37" },
  { sizeof(tekst.Motor_38_10.string),               110, SIZE_10, "Motor 38" },
  { sizeof(tekst.Motor_39_10.string),               110, SIZE_10, "Motor 39" },
  { sizeof(tekst.Motor_40_10.string),               110, SIZE_10, "Motor 40" },
  { sizeof(tekst.Motor_41_10.string),               110, SIZE_10, "Motor 41" },
  { sizeof(tekst.Motor_42_10.string),               110, SIZE_10, "Motor 42" },
  { sizeof(tekst.Motor_43_10.string),               110, SIZE_10, "Motor 43" },
  { sizeof(tekst.Motor_44_10.string),               110, SIZE_10, "Motor 44" },
  { sizeof(tekst.Motor_45_10.string),               110, SIZE_10, "Motor 45" },
  { sizeof(tekst.Motor_46_10.string),               110, SIZE_10, "Motor 46" },
  { sizeof(tekst.Motor_47_10.string),               110, SIZE_10, "Motor 47" },
  { sizeof(tekst.Motor_48_10.string),               110, SIZE_10, "Motor 48" },
  { sizeof(tekst.Motor_49_10.string),               110, SIZE_10, "Motor 49" },
  { sizeof(tekst.Motor_50_10.string),               110, SIZE_10, "Motor 50" },
  { sizeof(tekst.Motor_51_10.string),               110, SIZE_10, "Motor 51" },
  { sizeof(tekst.Motor_52_10.string),               110, SIZE_10, "Motor 52" },
  { sizeof(tekst.Motor_53_10.string),               110, SIZE_10, "Motor 53" },
  { sizeof(tekst.Motor_54_10.string),               110, SIZE_10, "Motor 54" },
  { sizeof(tekst.Motor_55_10.string),               110, SIZE_10, "Motor 55" },
  { sizeof(tekst.Motor_56_10.string),               110, SIZE_10, "Motor 56" },
  { sizeof(tekst.Motor_57_10.string),               110, SIZE_10, "Motor 57" },
  { sizeof(tekst.Motor_58_10.string),               110, SIZE_10, "Motor 58" },
  { sizeof(tekst.Motor_59_10.string),               110, SIZE_10, "Motor 59" },
  { sizeof(tekst.Motor_60_10.string),               110, SIZE_10, "Motor 60" },
  { sizeof(tekst.Motor_61_10.string),               110, SIZE_10, "Motor 61" },
  { sizeof(tekst.Motor_62_10.string),               110, SIZE_10, "Motor 62" },
  { sizeof(tekst.Motor_63_10.string),               110, SIZE_10, "Motor 63" },
  { sizeof(tekst.Motor_64_10.string),               110, SIZE_10, "Motor 64" },

  { sizeof(tekst.Status_10.string),                 100, SIZE_10, "Status" },
  { sizeof(tekst.Bediening_10.string),              110, SIZE_10, "Bediening" },
  { sizeof(tekst.Positie_10.string),                110, SIZE_10, "Positie" },

  { sizeof(tekst.Alarm_HAND_10.string),             110, SIZE_10, "Alarm HAND" },
  { sizeof(tekst.Alarm_afwijking_10.string),        110, SIZE_10, "Alarm afwijking" },
  { sizeof(tekst.Alarm_urgent_10.string),           110, SIZE_10, "Alarm urgent" },
  { sizeof(tekst.Pulse_zone_10.string),             110, SIZE_10, "Pulse zone" },
  { sizeof(tekst.Pulse_width_10.string),            110, SIZE_10, "Pulse width" },
  { sizeof(tekst.Cycletime_10.string),              110, SIZE_10, "Cycletime" },
  { sizeof(tekst.Hysterese_10.string),              130, SIZE_10, "Hysteresis" },                                        
  { sizeof(tekst.Stapgrootte_10.string),            130, SIZE_10, "Stapgrootte" },                               
  { sizeof(tekst.Bandbreedte_10.string),            136, SIZE_10, "Bandbreedte" },                                      

  { sizeof(tekst.Twee_doeken_een_bed_1_10.string),  145, SIZE_10, "2 doeken op 1 bed [1]" },
  { sizeof(tekst.Twee_doeken_een_bed_2_10.string),  145, SIZE_10, "2 doeken op 1 bed [2]" },
  { sizeof(tekst.Twee_doeken_een_bed_3_10.string),  145, SIZE_10, "2 doeken op 1 bed [3]" },
  { sizeof(tekst.Twee_doeken_een_bed_4_10.string),  145, SIZE_10, "2 doeken op 1 bed [4]" },
  { sizeof(tekst.Twee_doeken_een_bed_5_10.string),  145, SIZE_10, "2 doeken op 1 bed [5]" },
  { sizeof(tekst.Twee_doeken_een_bed_6_10.string),  145, SIZE_10, "2 doeken op 1 bed [6]" },
  { sizeof(tekst.Twee_doeken_een_bed_7_10.string),  145, SIZE_10, "2 doeken op 1 bed [7]" },
  { sizeof(tekst.Twee_doeken_een_bed_8_10.string),  145, SIZE_10, "2 doeken op 1 bed [8]" },

  { sizeof(tekst.Kier_10.string),                   110, SIZE_10, "Kier" },
  { sizeof(tekst.Hysteresis_10.string),             110, SIZE_10, "Hysteresis" },
  { sizeof(tekst.Standby_10.string),                110, SIZE_10, "Standby" },
  { sizeof(tekst.Voorloop_10.string),               110, SIZE_10, "Voorloop" },

  { sizeof(tekst.Time_10.string),                   129, SIZE_10, "Hora" },
  { sizeof(tekst.Tijd_10.string),                   110, SIZE_10, "Hora" },
  { sizeof(tekst.Datum_10.string),                   75, SIZE_10, "Fecha" },
  { sizeof(tekst.Sync_Tijd_10.string),              150, SIZE_10, "Sincronice tiempo" },
  { sizeof(tekst.Dagenteller_10.string),            140, SIZE_10, "Número del día" },

  { sizeof(tekst.F2_10.string),                     174, SIZE_10, "Ventilatie" },

  { sizeof(tekst.F3_10.string),                     174, SIZE_10, "Sensors" },

  { sizeof(tekst.Alarmen_10.string),                174, SIZE_10, "Alarmas" },
  { sizeof(tekst.Alarmen_Actief_10.string),         174, SIZE_10, "Alarmar actuales" },
  { sizeof(tekst.Alarmen_Historie_10.string),       174, SIZE_10, "Registro de alarmar" },
  { sizeof(tekst.Alarmen_Wissen_10.string),         174, SIZE_10, "Borrado alarma" },
  { sizeof(tekst.Alarm_10.string),                  174, SIZE_10, "Alarma" },
  { sizeof(tekst.Waarschuwing_10.string),           185, SIZE_10, "Advertencia" },
  { sizeof(tekst.Alarm_Systeem_10.string),          185, SIZE_10, "Alarma (Sistema)" },
  { sizeof(tekst.Waarschuwing_Syst_10.string),      185, SIZE_10, "Advertencia (Sist)" },
  { sizeof(tekst.Alarm_Computer_10.string),          40, SIZE_10, "Alarma" },
  { sizeof(tekst.Geen_Actief_Alarm_10.string),      185, SIZE_10, "No alarma actual" },
  { sizeof(tekst.Onbekend_Alarm_10.string),         185, SIZE_10, "Alarma desconocida" },
  { sizeof(tekst.Opties_10.string),                 174, SIZE_10, "Opciones Borrado" },
  { sizeof(tekst.Instellingen_10.string),           185, SIZE_10, "Ajustes Borrado" },
  { sizeof(tekst.Opties_En_Instellingen_10.string), 185, SIZE_10, "Opciones + ajustes" },
  { sizeof(tekst.Minimum_Maximum_10.string),        185, SIZE_10, "Minima/Maxima" },
  { sizeof(tekst.Gewist_10.string),                 185, SIZE_10, "Borrado" },
  { sizeof(tekst.Terug_Gezet_10.string),            185, SIZE_10, "Substituido" },
  { sizeof(tekst.Niet_Terug_Gezet_10.string),       185, SIZE_10, "No substituido" },
  { sizeof(tekst.I2C_10.string),                    185, SIZE_10, "I2C (EEPROM/RTC)" },
  { sizeof(tekst.EEPROM_10.string),                 185, SIZE_10, "EEPROM" },
  { sizeof(tekst.EEPROM_taal_10.string),            185, SIZE_10, "EEPROM (language)" },
  { sizeof(tekst.RTC_10.string),                    185, SIZE_10, "RTC" },
  { sizeof(tekst.TIMER_10.string),                  185, SIZE_10, "MARCADOR" },
  { sizeof(tekst.HTRAP_10.string),                  185, SIZE_10, "HTRAP" },
  { sizeof(tekst.PLL_10.string),                    185, SIZE_10, "PLL" },
  { sizeof(tekst.Opnieuw_Gestart_10.string),        185, SIZE_10, "Comienzo Ordenador" },
  { sizeof(tekst.Print_10.string),                   50, SIZE_10, "Placa" },
  { sizeof(tekst.Niet_Gevonden_10.string),          185, SIZE_10, "No encontrado" },
  { sizeof(tekst.ADC_10.string),                    185, SIZE_10, "ADC" },
  { sizeof(tekst.Analoge_Ingang_10.string),         150, SIZE_10, "Salidas analógicas" },
  { sizeof(tekst.Externe_24V_10.string),            185, SIZE_10, "External 24V" },                
  { sizeof(tekst.Orion_Uitgeschakeld_10.string),    185, SIZE_10, "Desconectar Orion" },
  { sizeof(tekst.Geen_Minimum_Alarm_10.string),     185, SIZE_10, "No Alarma Minima" },
  { sizeof(tekst.Offline_10.string),                 80, SIZE_10, "Off-line" },
  { sizeof(tekst.Slave_10.string),                   80, SIZE_10, "Slave" },

  { sizeof(tekst.Diagnose_10.string),               174, SIZE_10, "Diagnostico" },

  { sizeof(tekst.Info_10.string),                   132, SIZE_10, "Info" },
  { sizeof(tekst.Serie_Nr_14.string),                90, SIZE_14, "Serie no.:" },
  { sizeof(tekst.Computer_10.string),                80, SIZE_10, "Ordenador:" },
  { sizeof(tekst.Computer_14.string),               100, SIZE_14, "Ordenador:" },

  { sizeof(tekst.Draaiuren_10.string),              110, SIZE_10, "Draaiuren" },
  { sizeof(tekst.Schakelingen_10.string),           110, SIZE_10, "Schakelingen" },
  { sizeof(tekst.Storingen_10.string),              110, SIZE_10, "Storingen" },
  { sizeof(tekst.Looptijd_10.string),               110, SIZE_10, "Looptijd" },
  { sizeof(tekst.Alarm_Code_10.string),             110, SIZE_10, "Alarm Code" },

  { sizeof(tekst.Bewerken_Tekst_10.string),         185, SIZE_10, "Modifique el texto" },

  { sizeof(tekst.kg_10.string),                      16, SIZE_10, "kg" },
  { sizeof(tekst.kg_14.string),                      19, SIZE_14, "kg" },
  { sizeof(tekst.l_10.string),                       16, SIZE_10, "L" },
  { sizeof(tekst.l_14.string),                       19, SIZE_14, "L" },
  { sizeof(tekst.g_10.string),                       16, SIZE_10, "g" },
  { sizeof(tekst.ml_10.string),                      16, SIZE_10, "ml" },
  { sizeof(tekst.m_10.string),                       16, SIZE_10, "m" },
  { sizeof(tekst.s_10.string),                       16, SIZE_10, "s" },
  { sizeof(tekst.AAN_20.string),                     45, SIZE_20, "ON" },
  { sizeof(tekst.UIT_20.string),                     45, SIZE_20, "OFF" },
  { sizeof(tekst.Ana_10.string),                     36, SIZE_10, "Ana" },
  { sizeof(tekst.Dig_10.string),                     36, SIZE_10, "Dig" },
  { sizeof(tekst.MC_10.string),                      36, SIZE_10, "-" },
  { sizeof(tekst.Geen_IO_Toegewezen_10.string),     185, SIZE_10, "Ningún I/O asignado" },

  { sizeof(tekst.Password_10.string),               111, SIZE_10, "Contraseña" },
  { sizeof(tekst.Password_14.string),               111, SIZE_14, "Contraseña:" },
  { sizeof(tekst.Zeker_weten_7.string),             105, SIZE_7,  "¿Está usted seguro?" },
  { sizeof(tekst.Wissen_10.string),                  65, SIZE_10, "Borrar" },

  { sizeof(tekst.CAN_LOCAL_14.string),              185, SIZE_14, "CAN LOCAL" },
  { sizeof(tekst.Diag_CAN_LOCAL_10.string),         174, SIZE_10, "Diag.: CAN LOCAL" },
  { sizeof(tekst.TXOK_10.string),                    90, SIZE_10, "TXOK" },
  { sizeof(tekst.RXOK_10.string),                    90, SIZE_10, "RXOK" },
  { sizeof(tekst.TXOK_RXOK_10.string),               90, SIZE_10, "TXOK+RXOK" },
  { sizeof(tekst.BOFF_10.string),                   130, SIZE_10, "BOFF" },
  { sizeof(tekst.EWRN_10.string),                   130, SIZE_10, "EWRN" },
  { sizeof(tekst.Stuff_error_10.string),            130, SIZE_10, "Stuff error" },
  { sizeof(tekst.Form_error_10.string),             130, SIZE_10, "Form error" },
  { sizeof(tekst.Ack_error_10.string),              130, SIZE_10, "Ack error" },
  { sizeof(tekst.Bit1_error_10.string),             130, SIZE_10, "Bit1 error" },
  { sizeof(tekst.Bit0_busoff_error_10.string),      130, SIZE_10, "Bit0 busoff error" },
  { sizeof(tekst.Bit0_normal_error_10.string),      130, SIZE_10, "Bit0 normal error" },
  { sizeof(tekst.Crc_error_10.string),              130, SIZE_10, "Crc error" },
  { sizeof(tekst.perc_7.string),                      8, SIZE_7,  "%" },
  { sizeof(tekst.Pa_7.string),                       15, SIZE_7,  "Pa" },
  { sizeof(tekst.g_14.string),                       15, SIZE_14, "g" },
  { sizeof(tekst.ml_14.string),                      15, SIZE_14, "ml" },
  { sizeof(tekst.graden_celsius_7.string),           10, SIZE_7,  "°C" },
  { sizeof(tekst.graden_fahrenheid_7.string),        10, SIZE_7,  "°F" },
  { sizeof(tekst.graden_7.string),                    8, SIZE_7,  "°" },
  { sizeof(tekst.minuut_7.string),                    8, SIZE_7,  "min" },

  { sizeof(tekst.Geen_Alarm_Contact_10.string),     185, SIZE_10, "Sin contacto de alarma" },

  { sizeof(tekst.Handbediening_10.string),           185, SIZE_10, "Control manual" },
  { sizeof(tekst.Emergency_switch_10.string),        185, SIZE_10, "Emergency switch" },
  { sizeof(tekst.Thermal_failure_close_10.string),   185, SIZE_10, "Thermal failure close" },
  { sizeof(tekst.Thermal_failure_open_10.string),    185, SIZE_10, "Thermal failure open" },
  { sizeof(tekst.Break_input_10.string),             185, SIZE_10, "Break input" },
  { sizeof(tekst.Speed_to_low_10.string),            185, SIZE_10, "Speed to low" },
  { sizeof(tekst.Encoder_failure_10.string),         185, SIZE_10, "Encoder failure" },
  { sizeof(tekst.Encoder_failure_A_10.string),       185, SIZE_10, "Encoder failure A-signal" },
  { sizeof(tekst.Encoder_failure_B_10.string),       185, SIZE_10, "Encoder failure B-signal" },
  { sizeof(tekst.No_feedback_10.string),             185, SIZE_10, "No feedback" },
  { sizeof(tekst.Not_enough_pulses_10.string),       185, SIZE_10, "Not enough pulses" },
  { sizeof(tekst.Pulses_to_fast_10.string),          185, SIZE_10, "Pulses to fast" },
  { sizeof(tekst.Not_installed_10.string),           185, SIZE_10, "Not installed" },
  { sizeof(tekst.Encoder_interference_10.string),    185, SIZE_10, "Encoder interference" },
  { sizeof(tekst.Dualscreen_not_possible_10.string), 185, SIZE_10, "Dualscreen not possible" },
  { sizeof(tekst.Installation_mode_10.string),       185, SIZE_10, "Installation mode" },
  { sizeof(tekst.Deviation_position_10.string),      185, SIZE_10, "Position deviates" },
  { sizeof(tekst.Sensor_hi_speed_10.string),         185, SIZE_10, "Sensor hi-speed" },
  { sizeof(tekst.Direction_not_defined_10.string),   185, SIZE_10, "Direction not defined" },
  { sizeof(tekst.Limitswitches_not_equal_10.string), 185, SIZE_10, "Eindschakelaars niet gelijk" },
  { sizeof(tekst.Speed_not_equal_10.string),         185, SIZE_10, "Snelheid niet gelijk" },
  { sizeof(tekst.Multiple_master_10.string),         185, SIZE_10, "Multiple masters" },
  { sizeof(tekst.Frequency_controller_10.string),    185, SIZE_10, "Frequency controller" },
  { sizeof(tekst.Gelijkloop_beveiliging_10.string),  185, SIZE_10, "Gelijkloop beveiliging" },
  { sizeof(tekst.Limitswitch_not_reached_10.string), 185, SIZE_10, "Eindschakelaar niet bereikt" },
  { sizeof(tekst.Wrong_direction_10.string),         185, SIZE_10, "Draairichting verkeerd" },
  { sizeof(tekst.Link_unknown_10.string),            185, SIZE_10, "Koppeling niet ingesteld" },
  { sizeof(tekst.Motor_not_running_10.string),       185, SIZE_10, "Geen pulsen" },
  { sizeof(tekst.Position_not_reached_10.string),    185, SIZE_10, "Position not reached" },

  { sizeof(tekst.Locked_motor_10.string),              185, SIZE_10, "Locked motor" },
  { sizeof(tekst.Hall_failure_10.string),              185, SIZE_10, "Hall failure" },
  { sizeof(tekst.Thermal_motor_10.string),             185, SIZE_10, "Thermal overload motor" },
  { sizeof(tekst.Comm_error_master_slave_10.string),   185, SIZE_10, "Comm master/slave PIC" },
  { sizeof(tekst.Thermal_power_module_10.string),      185, SIZE_10, "Thermal power module" },
  { sizeof(tekst.Comm_error_remote_unit_10.string),    185, SIZE_10, "Comm error remote unit" },
  { sizeof(tekst.Phase_failure_10.string),             185, SIZE_10, "Phase failure" },
  { sizeof(tekst.Break_10.string),                     185, SIZE_10, "Break" },
  { sizeof(tekst.Low_line_voltage_10.string),          185, SIZE_10, "Low line voltage" },
  { sizeof(tekst.Low_DC_link_voltage_10.string),       185, SIZE_10, "Low DC-link voltage" },
  { sizeof(tekst.High_DC_link_voltage_10.string),      185, SIZE_10, "High DC-link voltage" },
  { sizeof(tekst.Driver_problem_10.string),            185, SIZE_10, "Driver problem" },
  { sizeof(tekst.Electronic_box_over_heat_10.string),  185, SIZE_10, "Electronic box over heat" },
  { sizeof(tekst.Excessive_DC_link_current_10.string), 185, SIZE_10, "Excessive DC-link current" },

  { sizeof(tekst.Vorstbeveiliging_10.string),          185, SIZE_10, "Vorstbeveiliging" },
  { sizeof(tekst.Algemeen_extern_alarm_10.string),     185, SIZE_10, "Algemeen extern alarm" },
  { sizeof(tekst.Drukverschil_bewaking_10.string),     185, SIZE_10, "Drukverschil bewaking" },

  { sizeof(tekst.JA_10.string),                      45, SIZE_10, "SI" },
  { sizeof(tekst.NEE_10.string),                     45, SIZE_10, "NO" },

  { sizeof(tekst.CAN_RS232_10.string),               80, SIZE_10, "CAN-RS232" },

  { sizeof(tekst.CAN_BACKBONE_14.string),           185, SIZE_14, "CAN BACKBONE" },
  { sizeof(tekst.Diag_CAN_BACKBONE_10.string),      174, SIZE_10, "Diag.: CAN BACKBONE" },

  { sizeof(tekst.Wijzig_computer_type_14.string),   180, SIZE_14, "Module"/*"Wijzig computer type"*/ },
  { sizeof(tekst.Activeer_module_7.string),         105, SIZE_7,  "Active module" },
  { sizeof(tekst.Deactiveer_module_7.string),       105, SIZE_7,  "Desactive module" },
  { sizeof(tekst.Ongeldige_code_7.string),          105, SIZE_7,  "Ongeldige code!" },

  { sizeof(tekst.SD_CARD_10.string),                 90, SIZE_10, "SD-CARD" },
  { sizeof(tekst.SD_CARD_14.string),                185, SIZE_14, "SD-CARD" },
  { sizeof(tekst.Diag_SD_CARD_10.string),           174, SIZE_10, "Diag.: SD-CARD" },
  { sizeof(tekst.no_card_10.string),                 90, SIZE_10, "no card" },
  { sizeof(tekst.placed_10.string),                  90, SIZE_10, "placed" },
  { sizeof(tekst.read_write_10.string),              90, SIZE_10, "read/write" },
  { sizeof(tekst.read_only_10.string),               90, SIZE_10, "read only" },
  { sizeof(tekst.active_10.string),                  90, SIZE_10, "active" },
  { sizeof(tekst.ask_remove_10.string),              90, SIZE_10, "ask remove" },
  { sizeof(tekst.remove_10.string),                  90, SIZE_10, "remove" },
  { sizeof(tekst.SIZE_7.string),                     40, SIZE_7,  "SIZE" },

  { sizeof(tekst.Diag_COM1_USB_10.string),          185, SIZE_10, "Diag.: COM1/USB" },     // Diag_COM1_USB_10
  { sizeof(tekst.COM1_USB_10.string),                75, SIZE_10, "COM1/USB" },            // COM1_USB_10
  { sizeof(tekst.kBd_14.string),                     30, SIZE_14, "kBd" },                 // kBd_14
  { sizeof(tekst.TX_10.string),                      60, SIZE_10, "TX" },                  // TX_10
  { sizeof(tekst.RX_10.string),                      60, SIZE_10, "RX" },                  // RX_10
  { sizeof(tekst.TX_RX_10.string),                   60, SIZE_10, "TX+RX" },               // TX_RX_10
  { sizeof(tekst.AT_10.string),                      90, SIZE_10, "AT" },                  // AT_10
  { sizeof(tekst.Modem_Init_10.string),              90, SIZE_10, "Modem Init" },          // Modem_Init_10

  { sizeof(tekst.Diag_COM2_10.string),              185, SIZE_10, "Diag.: COM2" },         // Diag_COM2_10
  { sizeof(tekst.COM2_10.string),                    75, SIZE_10, "COM2" },                // COM2_10
  { sizeof(tekst.Modem_DCD_10.string),               90, SIZE_10, "Modem DCD" },           // Modem_DCD_10

  { sizeof(tekst.Diag_Ethernet_10.string),          185, SIZE_10, "Diag.: Ethernet" },     // Diag_Ethernet_10
  { sizeof(tekst.IP_10.string),                      50, SIZE_10, "IP" },                  // IP_10
  { sizeof(tekst.Mask_10.string),                    50, SIZE_10, "Mask" },                // Mask_10
  { sizeof(tekst.Gate_10.string),                    50, SIZE_10, "Gate" },                // Gate_10
  { sizeof(tekst.MAC_10.string),                     50, SIZE_10, "MAC" },                 // MAC_10
  { sizeof(tekst.PC_10.string),                      50, SIZE_10, "PC" },                  // PC_10
  { sizeof(tekst.NO_CONNECTION_14.string),          150, SIZE_14, "NO CONNECTION" },       // NO_CONNECTION_14
  { sizeof(tekst.RTX_10.string),                     60, SIZE_10, "RTX" },                 // RTX_10
  { sizeof(tekst.NO_ACK_10.string),                  60, SIZE_10, "NO_ACK" },              // NO_ACK_10
  { sizeof(tekst.CONNECT_10.string),                 60, SIZE_10, "CONNECT" },             // CONNECT_10
  { sizeof(tekst.Reset_Ethernet_10.string),         130, SIZE_10, "Reset Ethernet" },      // Reset_Ethernet_10
  { sizeof(tekst.Port_10.string),                    50, SIZE_10, "Port" },                // Port_10

  { sizeof(tekst.COM1_USB_14.string),               185, SIZE_14, "COM1/USB" },            // COM1_USB_14
  { sizeof(tekst.COM2_14.string),                   185, SIZE_14, "COM2" },                // COM2_14
  { sizeof(tekst.Ethernet_14.string),               185, SIZE_14, "Ethernet" },            // Ethernet_14

  { sizeof(tekst.Ventilator_10.string),             185, SIZE_10, "Ventilator" },
  { sizeof(tekst.Minimum_10.string),                185, SIZE_10, "Minimum" },
  { sizeof(tekst.Maximum_10.string),                185, SIZE_10, "Maximum" },
  { sizeof(tekst.Offset_10.string),                 185, SIZE_10, "Offset" },

  { sizeof(tekst.Maximum_rpm_10.string),            110, SIZE_10, "Maximum rpm" },
  { sizeof(tekst.Vermogen_10.string),               110, SIZE_10, "Vermogen" },
  { sizeof(tekst.Temp_motor_10.string),             110, SIZE_10, "Temp motor" },
  { sizeof(tekst.Temp_electronica_10.string),       110, SIZE_10, "Temp electronica" },
  { sizeof(tekst.Temp_power_module_10.string),      110, SIZE_10, "Temp power module" },
  { sizeof(tekst.Fabrieksinst_terugz_10.string),    110, SIZE_10, "Fabrieksinst. terugz." },
  { sizeof(tekst.Versie_10.string),                 110, SIZE_10, "Versie" },

  { sizeof(tekst.Reset_all_10.string),              110, SIZE_10, "Reset all" },

  { sizeof(tekst.V_10.string),                       60, SIZE_10, "V:" },
  { sizeof(tekst.N_10.string),                       60, SIZE_10, "N:" },
  { sizeof(tekst.Geheugen_256k_10.string),          185, SIZE_10, "ONLY 256k RAM" },      	  
  { sizeof(tekst.Ongeldige_update_10.string),       185, SIZE_10, "Ongeldige update" },
  { sizeof(tekst.Plaats_512k_EEPROM_10.string),     185, SIZE_10, "Plaats 512k EEPROM" },
  { sizeof(tekst.OK_is_opties_wissen_10.string),    185, SIZE_10, "OK = opties wissen" },

  { sizeof(tekst.Luchtmengkast_14.string),          185, SIZE_14, "Luchtmengkast" },
  { sizeof(tekst.Luchtmengkast_groep_14.string),    185, SIZE_14, "Luchtmengkast groep" },
  { sizeof(tekst.Luchtmengkast_10.string),          185, SIZE_10, "Luchtmengkast" },
  { sizeof(tekst.Luchtmengkast_groep_10.string),    185, SIZE_10, "Luchtmengkast groep" },
  { sizeof(tekst.Diag_Luchtmengkast_10.string),     185, SIZE_10, "Diag.: Luchtmengkast" },
  { sizeof(tekst.Fn_Luchtmengkast_10.string),       185, SIZE_10, "Fn.: Luchtmengkast" },
  { sizeof(tekst.Syst_Luchtmengkast_10.string),     185, SIZE_10, "Syst.: Luchtmengkast" },

  { sizeof(tekst.Binnenklep_10.string),             185, SIZE_10, "Binnenklep" },
  { sizeof(tekst.Buitenklep_10.string),             185, SIZE_10, "Buitenklep" },
  { sizeof(tekst.Recirculatieklep_10.string),       185, SIZE_10, "Recirculatieklep" },
  { sizeof(tekst.Bovenklep_10.string),              185, SIZE_10, "Bovenklep" },
  { sizeof(tekst.Verwarming_10.string),             185, SIZE_10, "Verwarming" },
  { sizeof(tekst.Inblaasvent_10.string),            185, SIZE_10, "Inblaasventilator" },
  { sizeof(tekst.Afblaasvent_10.string),            185, SIZE_10, "Afblaasventilator" },
  { sizeof(tekst.ebm_10.string),                    110, SIZE_10, "ebm" },

  { sizeof(tekst.Afblaasvent_aan_10.string),        185, SIZE_10, "Afblaasvent. aan" },
  { sizeof(tekst.Afblaasvent_uit_10.string),        185, SIZE_10, "Afblaasvent. uit" },
  { sizeof(tekst.Bovenklep_open_10.string),         185, SIZE_10, "Bovenklep open" },
  { sizeof(tekst.Bovenklep_dicht_10.string),        185, SIZE_10, "Bovenklep dicht" },

  //#ifdef CAN_BACKBONE_PC_WARNING
  { sizeof(tekst.CAN_PC_1_Offline_10.string),              185, SIZE_10, "CAN PC 1 offline" },   // CAN_PC_1_Offline_10
  { sizeof(tekst.CAN_PC_2_Offline_10.string),              185, SIZE_10, "CAN PC 2 offline" },   // CAN_PC_2_Offline_10
  { sizeof(tekst.CAN_PC_3_Offline_10.string),              185, SIZE_10, "CAN PC 3 offline" },   // CAN_PC_3_Offline_10
  //#endif // CAN_BACKBONE_PC_WARNING

  { sizeof(tekst.Display_14.string),                        90, SIZE_14, "Display" },             // Display_14
  { sizeof(tekst.Groen_14.string),                          90, SIZE_14, "Green" },               // Groen_14
  { sizeof(tekst.Wit_14.string),                            90, SIZE_14, "White" },               // Wit_14

  { sizeof(tekst.Modules_14.string),                180, SIZE_14, "Modules" }, // Modules_14

  { sizeof(tekst.General_Error_10.string),             185, SIZE_10, "General error" },
  { sizeof(tekst.Motor_Fault_10.string),               185, SIZE_10, "Motor fault" },
  { sizeof(tekst.Motor_Blocked_10.string),             185, SIZE_10, "Motor blocked" },
  { sizeof(tekst.Heat_Sink_Temperature_10.string),     185, SIZE_10, "Heat sink temperature" },
  { sizeof(tekst.Ground_Fault_10.string),              185, SIZE_10, "Ground fault" },
  { sizeof(tekst.Hall_IC_Fault_10.string),             185, SIZE_10, "Hall IC fault" },
  { sizeof(tekst.Overcurrent_10.string),               185, SIZE_10, "Overcurrent" },
  { sizeof(tekst.Line_Fault_10.string),                185, SIZE_10, "Line fault" },
  { sizeof(tekst.Int_Heat_Sink_Sensor_10.string),      185, SIZE_10, "Int heat sink sensor" },
  { sizeof(tekst.DC_Res_Voltage_To_High_10.string),    185, SIZE_10, "DC res voltage to high" },
  { sizeof(tekst.Temperature_Lowering_10.string),      185, SIZE_10, "Temperature lowering" },
  { sizeof(tekst.Wrong_Connection_10.string),          185, SIZE_10, "Wrong connection" },
  { sizeof(tekst.External_Fault_10.string),            185, SIZE_10, "External fault" },
  { sizeof(tekst.Factory_Settings_10.string),          185, SIZE_10, "Factory settings" },
  { sizeof(tekst.EEP_Error_10.string),                 185, SIZE_10, "EEP error" },
  { sizeof(tekst.RTC_General_Fault_10.string),         185, SIZE_10, "RTC general fault" },
  { sizeof(tekst.RTC_Voltage_Fault_10.string),         185, SIZE_10, "RTC voltage fault" },
  { sizeof(tekst.Filter_Contamination_10.string),      185, SIZE_10, "Filter contamination" },
  { sizeof(tekst.Transfer_Error_10.string),            185, SIZE_10, "Transfer error" },
  { sizeof(tekst.Data_Connetion_Line_10.string),       185, SIZE_10, "Data connection line" },
  { sizeof(tekst.Data_Connection_Checksum_10.string),  185, SIZE_10, "Data connection checksum" },
  { sizeof(tekst.Sensor_Fault_Input_1_10.string),      185, SIZE_10, "Sensor fault input 1" },
  { sizeof(tekst.Sensor_Fault_Input_2_10.string),      185, SIZE_10, "Sensor fault input 2" },
  { sizeof(tekst.Sensor_Fault_Input_3_10.string),      185, SIZE_10, "Sensor fault input 3" },
  { sizeof(tekst.High_line_voltage_10.string),         185, SIZE_10, "High line voltage" },
  { sizeof(tekst.i_limit_10.string),                   185, SIZE_10, "Current limitation in mesh" },
  { sizeof(tekst.p_limit_10.string),                   185, SIZE_10, "Power limitation in mesh" },
  { sizeof(tekst.te_high_10.string),                   185, SIZE_10, "Output stage temperature high" },
  { sizeof(tekst.tm_high_10.string),                   185, SIZE_10, "Motor temperature high" },
  { sizeof(tekst.tei_high_10.string),                  185, SIZE_10, "Electronics interion temperature high" },
  { sizeof(tekst.uz_low_10.string),                    185, SIZE_10, "DC-Link voltage low" },
  { sizeof(tekst.n_low_10.string),                     185, SIZE_10, "Actual speed less then limit speed" },

  { sizeof(tekst.igbt_fault_10.string),                185, SIZE_10, "IGBT fault" },
  { sizeof(tekst.uzk_hi_10.string),                    185, SIZE_10, "Uzk hi" },
  { sizeof(tekst.uzk_lo_10.string),                    185, SIZE_10, "Uzk lo" },
  { sizeof(tekst.uin_hi_10.string),                    185, SIZE_10, "Uin hi" },
  { sizeof(tekst.uin_lo_10.string),                    185, SIZE_10, "Uin lo" },

  { sizeof(tekst.Klep_10.string),                      185, SIZE_10, "Klep" },
  { sizeof(tekst.Sensor_10.string),                    185, SIZE_10, "Sensor" },
  { sizeof(tekst.Drukverschil_1_10.string),            185, SIZE_10, "Drukverschil 1"  },
  { sizeof(tekst.Drukverschil_2_10.string),            185, SIZE_10, "Drukverschil 2"  },
  { sizeof(tekst.Drukverschil_3_10.string),            185, SIZE_10, "Drukverschil 3"  },
  { sizeof(tekst.Drukverschil_4_10.string),            185, SIZE_10, "Drukverschil 4"  },
  { sizeof(tekst.Drukverschil_5_10.string),            185, SIZE_10, "Drukverschil 5"  },
  { sizeof(tekst.Drukverschil_6_10.string),            185, SIZE_10, "Drukverschil 6"  },
  { sizeof(tekst.Drukverschil_7_10.string),            185, SIZE_10, "Drukverschil 7"  },
  { sizeof(tekst.Drukverschil_8_10.string),            185, SIZE_10, "Drukverschil 8"  },
  { sizeof(tekst.Drukverschil_9_10.string),            185, SIZE_10, "Drukverschil 9"  },
  { sizeof(tekst.Drukverschil_10_10.string),           185, SIZE_10, "Drukverschil 10" },
  { sizeof(tekst.Drukverschil_11_10.string),           185, SIZE_10, "Drukverschil 11" },
  { sizeof(tekst.Drukverschil_12_10.string),           185, SIZE_10, "Drukverschil 12" },
  { sizeof(tekst.Drukverschil_13_10.string),           185, SIZE_10, "Drukverschil 13" },
  { sizeof(tekst.Drukverschil_14_10.string),           185, SIZE_10, "Drukverschil 14" },
  { sizeof(tekst.Drukverschil_15_10.string),           185, SIZE_10, "Drukverschil 15" },
  { sizeof(tekst.Drukverschil_16_10.string),           185, SIZE_10, "Drukverschil 16" },

  { sizeof(tekst.Extern_alarm_10.string),              185, SIZE_10, "External alarm" },

  { sizeof(tekst.failure_power_section_10.string),     185, SIZE_10, "Failure power section" },
  { sizeof(tekst.umax_10.string),                      185, SIZE_10, "U > Umax" },
  { sizeof(tekst.umin_10.string),                      185, SIZE_10, "U < Umin" },
  { sizeof(tekst.overspeed_10.string),                 185, SIZE_10, "Overspeed" },
  { sizeof(tekst.locked_rotor_10.string),              185, SIZE_10, "Locked rotor" },

  { sizeof(tekst.EEPROM_256k_10.string),               185, SIZE_10, "SÓLO 256k EEPROM" },    
  { sizeof(tekst.Version_Error_10.string),             185, SIZE_10, "Error de versión" },

  { sizeof(tekst.underspeed_10.string),                185, SIZE_10, "Underspeed" }, 
  { sizeof(tekst._24V_supply_overload_10.string),      185, SIZE_10, "24V supply overload" },
  { sizeof(tekst.input_phase_error_10.string),         185, SIZE_10, "Input phase error" }, 
  { sizeof(tekst.motor_phase_error_10.string),         185, SIZE_10, "Motor phase error" },
  { sizeof(tekst.memory_error_10.string),              185, SIZE_10, "Memory error" }, 
  { sizeof(tekst.short_circuit_10.string),             185, SIZE_10, "Short circuit" }, 
  { sizeof(tekst.loss_of_synchronism_10.string),       185, SIZE_10, "Loss of synchronism" }, 
  { sizeof(tekst.input_voltage_error_10.string),       185, SIZE_10, "Input voltage error" },
  { sizeof(tekst.input_relay_not_closed_10.string),    185, SIZE_10, "Input relay not closed" }, 
  { sizeof(tekst.high_starting_current_10.string),     185, SIZE_10, "High starting current" }, 
};

//-----------------------------------------------------------------------------

s_tekst_inst const tekst_inst_engels =
{
  sizeof(s_tekst_inst), // unsinged int area_size;
  ORION,                // unsigned int computer;
  MULTI_CONNECT,        // unsigned int soort;
  VERSIE_TEKST_INST,    // unsigned int versie_tekst_inst;

  { sizeof(tekst_inst.Gekozen_Taal_14.string),         150, SIZE_14, "English" },

  { sizeof(tekst_inst.Opties_10.string),               174, SIZE_10, "Options" },
  { sizeof(tekst_inst.Bekijken_Opties_14.string),      185, SIZE_14, "View options" },
  { sizeof(tekst_inst.Wijzigen_Opties_14.string),      185, SIZE_14, "Modify options" },
  { sizeof(tekst_inst.Orion_Ingeschakeld_14.string),   185, SIZE_14, "Orion switched-on" },
  { sizeof(tekst_inst.Orion_Uitgeschakeld_14.string),  185, SIZE_14, "Orion switched-off" },

  { sizeof(tekst_inst.Algemeen_14.string),             185, SIZE_14, "General" },
  { sizeof(tekst_inst.IO_14.string),                   185, SIZE_14, "Inputs/outputs" },
  { sizeof(tekst_inst.Motorgroepen_14.string),         185, SIZE_14, "Motorgroups" },
  { sizeof(tekst_inst.Groep_1_14.string),              185, SIZE_14, "Group 1" },
  { sizeof(tekst_inst.Groep_2_14.string),              185, SIZE_14, "Group 2" },
  { sizeof(tekst_inst.Groep_3_14.string),              185, SIZE_14, "Group 3" },
  { sizeof(tekst_inst.Groep_4_14.string),              185, SIZE_14, "Group 4" },
  { sizeof(tekst_inst.Groep_5_14.string),              185, SIZE_14, "Group 5" },
  { sizeof(tekst_inst.Groep_6_14.string),              185, SIZE_14, "Group 6" },
  { sizeof(tekst_inst.Groep_7_14.string),              185, SIZE_14, "Group 7" },
  { sizeof(tekst_inst.Groep_8_14.string),              185, SIZE_14, "Group 8" },
  { sizeof(tekst_inst.Groep_9_14.string),              185, SIZE_14, "Group 9" },
  { sizeof(tekst_inst.Groep_10_14.string),             185, SIZE_14, "Group 10" },
  { sizeof(tekst_inst.Groep_11_14.string),             185, SIZE_14, "Group 11" },
  { sizeof(tekst_inst.Groep_12_14.string),             185, SIZE_14, "Group 12" },
  { sizeof(tekst_inst.Groep_13_14.string),             185, SIZE_14, "Group 13" },
  { sizeof(tekst_inst.Groep_14_14.string),             185, SIZE_14, "Group 14" },
  { sizeof(tekst_inst.Groep_15_14.string),             185, SIZE_14, "Group 15" },
  { sizeof(tekst_inst.Groep_16_14.string),             185, SIZE_14, "Group 16" },
  { sizeof(tekst_inst.Groep_17_14.string),             185, SIZE_14, "Group 17" },
  { sizeof(tekst_inst.Groep_18_14.string),             185, SIZE_14, "Group 18" },
  { sizeof(tekst_inst.Groep_19_14.string),             185, SIZE_14, "Group 19" },
  { sizeof(tekst_inst.Groep_20_14.string),             185, SIZE_14, "Group 20" },
  { sizeof(tekst_inst.Groep_21_14.string),             185, SIZE_14, "Group 21" },
  { sizeof(tekst_inst.Groep_22_14.string),             185, SIZE_14, "Group 22" },
  { sizeof(tekst_inst.Groep_23_14.string),             185, SIZE_14, "Group 23" },
  { sizeof(tekst_inst.Groep_24_14.string),             185, SIZE_14, "Group 24" },
  { sizeof(tekst_inst.Groep_25_14.string),             185, SIZE_14, "Group 25" },
  { sizeof(tekst_inst.Groep_26_14.string),             185, SIZE_14, "Group 26" },
  { sizeof(tekst_inst.Groep_27_14.string),             185, SIZE_14, "Group 27" },
  { sizeof(tekst_inst.Groep_28_14.string),             185, SIZE_14, "Group 28" },
  { sizeof(tekst_inst.Groep_29_14.string),             185, SIZE_14, "Group 29" },
  { sizeof(tekst_inst.Groep_30_14.string),             185, SIZE_14, "Group 30" },
  { sizeof(tekst_inst.Groep_31_14.string),             185, SIZE_14, "Group 31" },
  { sizeof(tekst_inst.Groep_32_14.string),             185, SIZE_14, "Group 32" },
  { sizeof(tekst_inst.Twee_doek_een_bed_14.string),    185, SIZE_14, "Dualscreen" },
  { sizeof(tekst_inst.Cabriokas_14.string),            185, SIZE_14, "Open roof" },

  { sizeof(tekst_inst.Opties_Doorlopen_14.string),     220, SIZE_14, "View options?" },
  { sizeof(tekst_inst.Opties_OK_14.string),            220, SIZE_14, "Options OK?" },

  { sizeof(tekst_inst.Opties_algemeen_10.string),      185, SIZE_10, "Options: General" },
  { sizeof(tekst_inst.Taal_14.string),                 185, SIZE_14, "Language" },
  { sizeof(tekst_inst.Opties_Wissen_14.string),        185, SIZE_14, "Delete options" },
  { sizeof(tekst_inst.Opties_gewist_14.string),        210, SIZE_14, "Options deleted!" },
  { sizeof(tekst_inst.Setpoints_Wissen_14.string),     185, SIZE_14, "Delete setpoints" },
  { sizeof(tekst_inst.Setpoints_gewist_14.string),     210, SIZE_14, "Setpoints deleted!" },
  { sizeof(tekst_inst.Helderheid_14.string),           185, SIZE_14, "Brightness" },
  { sizeof(tekst_inst.LCD_dimmen_14.string),           185, SIZE_14, "Dim backlighting" },
  { sizeof(tekst_inst.Computer_14.string),             185, SIZE_14, "Computer" },
  { sizeof(tekst_inst.Nummer_14.string),               135, SIZE_14, "Number" },
  { sizeof(tekst_inst.Adres_14.string),                135, SIZE_14, "Adres" },
  { sizeof(tekst_inst.CAN_BACKBONE_14.string),         140, SIZE_14, "CAN BACKBONE" },        // CAN_BACKBONE_14
  { sizeof(tekst_inst.CAN_RS232_14.string),            140, SIZE_14, "CAN-RS232" },           // CAN_RS232_14
  { sizeof(tekst_inst.Waarschuwing_14.string),         140, SIZE_14, "Warning" },             // Waarschuwing_14
  { sizeof(tekst_inst.RS232_14.string),                185, SIZE_14, "RS232" },
  { sizeof(tekst_inst.kBd_14.string),                   30, SIZE_14, "kBd" },
  { sizeof(tekst_inst.Installateurs_14.string),        185, SIZE_14, "Installer" },
  { sizeof(tekst_inst.Gebruikers_14.string),           185, SIZE_14, "Customer" },
  { sizeof(tekst_inst.PC_14.string),                   185, SIZE_14, "PC" },
  { sizeof(tekst_inst.Wachtwoord_14.string),           110, SIZE_14, "password" },
  { sizeof(tekst_inst.Herhaal_14.string),              110, SIZE_14, "repeat" },
  { sizeof(tekst_inst.Wachtwoord_ongelijk_7.string),   100, SIZE_7,  "Passwords differs" },

  { sizeof(tekst_inst.Opties_IO_10.string),            185, SIZE_10, "Options: Inputs/outputs" },
  { sizeof(tekst_inst.Versie_7.string),                 40, SIZE_7,  "Version:" },

  { sizeof(tekst_inst.Opties_bord_10.string),           66, SIZE_10, "Options:" },
  { sizeof(tekst_inst.Nummer_IO_Module_14.string),     220, SIZE_14, "Number I/O Module" },
  { sizeof(tekst_inst.Verwijderen_14.string),          150, SIZE_14, "remove" },
  { sizeof(tekst_inst.Analoge_Ingang_14.string),       150, SIZE_14, "Analogue input" },
  { sizeof(tekst_inst.Digitale_Ingang_14.string),      150, SIZE_14, "Digital input" },
  { sizeof(tekst_inst.Analoge_Uitgang_14.string),      150, SIZE_14, "Analogue output" },
  { sizeof(tekst_inst.Digitale_Uitgang_14.string),     150, SIZE_14, "Digital output" },
  { sizeof(tekst_inst.Motor_Control_14.string),        150, SIZE_14, "Motor control" },
  { sizeof(tekst_inst.Instellen_sensor_14.string),     130, SIZE_14, "Adjust sensor" },
  { sizeof(tekst_inst.IJken_sensor_14.string),         130, SIZE_14, "Calibrate sensor" },
  { sizeof(tekst_inst.IJken_Minimum_14.string),        130, SIZE_14, "Calibr minimum" },   
  { sizeof(tekst_inst.IJken_Maximum_14.string),        130, SIZE_14, "Calibr maximum" },   
  { sizeof(tekst_inst.Instellen_14.string),            130, SIZE_14, "Adjust" },
  { sizeof(tekst_inst.IJken_14.string),                130, SIZE_14, "Calibrate" },
  { sizeof(tekst_inst.V_14.string),                     12, SIZE_14, "V" },
  { sizeof(tekst_inst.mA_14.string),                    12, SIZE_14, "mA" },
  { sizeof(tekst_inst.Overnemen_10.string),             77, SIZE_10, "Take over" },
  { sizeof(tekst_inst.Windrichting_14.string),         210, SIZE_14, "Wind direction" },
  { sizeof(tekst_inst.Windsnelheid_bij_5V_14.string),  210, SIZE_14, "Wind speed at 5.0V" },
  { sizeof(tekst_inst.kg_14.string),                    19, SIZE_14, "kg" },
  { sizeof(tekst_inst.g_14.string),                     19, SIZE_14, "g" },
  { sizeof(tekst_inst.Pulsen_Per_Liter_14.string),     210, SIZE_14, "Pulses per litre" },
  { sizeof(tekst_inst.Liters_Per_Puls_14.string),      210, SIZE_14, "Litres per pulse" },
  { sizeof(tekst_inst.Pulsen_Per_Kg_14.string),        210, SIZE_14, "Pulses per kg" },
  { sizeof(tekst_inst.Kg_Per_Puls_14.string),          210, SIZE_14, "Kg per pulse" },
  { sizeof(tekst_inst.Pulsen_Per_Ei_14.string),        210, SIZE_14, "Pulses per egg" },
  { sizeof(tekst_inst.Eieren_Per_Puls_14.string),      210, SIZE_14, "Eggs per pulse" },
  { sizeof(tekst_inst.Pulsen_Per_kWh_14.string),       210, SIZE_14, "Pulses per kWh" },
  { sizeof(tekst_inst.kWh_Per_Puls_14.string),         210, SIZE_14, "kWh per pulse" },
  { sizeof(tekst_inst.Minimum_14.string),              100, SIZE_14, "Minimum:" },
  { sizeof(tekst_inst.Maximum_14.string),              100, SIZE_14, "Maximum:" },
  { sizeof(tekst_inst.Aantal_Tellers_14.string),       135, SIZE_14, "Counters" },
  { sizeof(tekst_inst.Teller_14.string),               100, SIZE_14, "Counter" },
  { sizeof(tekst_inst.Fout_Licht_Niveau_14.string),    180, SIZE_14, "Error light level" },
  { sizeof(tekst_inst.Fout_Communicatie_14.string),    180, SIZE_14, "Error communication" },
  { sizeof(tekst_inst.Fout_Onbekend_14.string),        180, SIZE_14, "Error unknown" },
  { sizeof(tekst_inst.Niet_Toegewezen_14.string),      150, SIZE_14, "Not assigned" },

  { sizeof(tekst_inst.Opties_motorgroepen_10.string),  185, SIZE_10, "Options: Motorgroups" },
  { sizeof(tekst_inst.Aantal_motorgroepen_14.string),  180, SIZE_14, "Motorgroups" },

  { sizeof(tekst_inst.Opties_groep_1_10.string),       185, SIZE_10, "Options: Group 1" },
  { sizeof(tekst_inst.Opties_groep_2_10.string),       185, SIZE_10, "Options: Group 2" },
  { sizeof(tekst_inst.Opties_groep_3_10.string),       185, SIZE_10, "Options: Group 3" },
  { sizeof(tekst_inst.Opties_groep_4_10.string),       185, SIZE_10, "Options: Group 4" },
  { sizeof(tekst_inst.Opties_groep_5_10.string),       185, SIZE_10, "Options: Group 5" },
  { sizeof(tekst_inst.Opties_groep_6_10.string),       185, SIZE_10, "Options: Group 6" },
  { sizeof(tekst_inst.Opties_groep_7_10.string),       185, SIZE_10, "Options: Group 7" },
  { sizeof(tekst_inst.Opties_groep_8_10.string),       185, SIZE_10, "Options: Group 8" },
  { sizeof(tekst_inst.Opties_groep_9_10.string),       185, SIZE_10, "Options: Group 9" },
  { sizeof(tekst_inst.Opties_groep_10_10.string),      185, SIZE_10, "Options: Group 10" },
  { sizeof(tekst_inst.Opties_groep_11_10.string),      185, SIZE_10, "Options: Group 11" },
  { sizeof(tekst_inst.Opties_groep_12_10.string),      185, SIZE_10, "Options: Group 12" },
  { sizeof(tekst_inst.Opties_groep_13_10.string),      185, SIZE_10, "Options: Group 13" },
  { sizeof(tekst_inst.Opties_groep_14_10.string),      185, SIZE_10, "Options: Group 14" },
  { sizeof(tekst_inst.Opties_groep_15_10.string),      185, SIZE_10, "Options: Group 15" },
  { sizeof(tekst_inst.Opties_groep_16_10.string),      185, SIZE_10, "Options: Group 16" },
  { sizeof(tekst_inst.Opties_groep_17_10.string),      185, SIZE_10, "Options: Group 17" },
  { sizeof(tekst_inst.Opties_groep_18_10.string),      185, SIZE_10, "Options: Group 18" },
  { sizeof(tekst_inst.Opties_groep_19_10.string),      185, SIZE_10, "Options: Group 19" },
  { sizeof(tekst_inst.Opties_groep_20_10.string),      185, SIZE_10, "Options: Group 20" },
  { sizeof(tekst_inst.Opties_groep_21_10.string),      185, SIZE_10, "Options: Group 21" },
  { sizeof(tekst_inst.Opties_groep_22_10.string),      185, SIZE_10, "Options: Group 22" },
  { sizeof(tekst_inst.Opties_groep_23_10.string),      185, SIZE_10, "Options: Group 23" },
  { sizeof(tekst_inst.Opties_groep_24_10.string),      185, SIZE_10, "Options: Group 24" },
  { sizeof(tekst_inst.Opties_groep_25_10.string),      185, SIZE_10, "Options: Group 25" },
  { sizeof(tekst_inst.Opties_groep_26_10.string),      185, SIZE_10, "Options: Group 26" },
  { sizeof(tekst_inst.Opties_groep_27_10.string),      185, SIZE_10, "Options: Group 27" },
  { sizeof(tekst_inst.Opties_groep_28_10.string),      185, SIZE_10, "Options: Group 28" },
  { sizeof(tekst_inst.Opties_groep_29_10.string),      185, SIZE_10, "Options: Group 29" },
  { sizeof(tekst_inst.Opties_groep_30_10.string),      185, SIZE_10, "Options: Group 30" },
  { sizeof(tekst_inst.Opties_groep_31_10.string),      185, SIZE_10, "Options: Group 31" },
  { sizeof(tekst_inst.Opties_groep_32_10.string),      185, SIZE_10, "Options: Group 32" },

  { sizeof(tekst_inst.Opties_2_doek_1_bed_10.string),  185, SIZE_10, "Options: Dualscreen" },
  { sizeof(tekst_inst.Aantal_14.string),               180, SIZE_14, "Number" },
  { sizeof(tekst_inst.Twee_doek_een_bed_1_14.string),  180, SIZE_14, "Dualscreen [1]" },
  { sizeof(tekst_inst.Twee_doek_een_bed_2_14.string),  180, SIZE_14, "Dualscreen [2]" },
  { sizeof(tekst_inst.Twee_doek_een_bed_3_14.string),  180, SIZE_14, "Dualscreen [3]" },
  { sizeof(tekst_inst.Twee_doek_een_bed_4_14.string),  180, SIZE_14, "Dualscreen [4]" },
  { sizeof(tekst_inst.Twee_doek_een_bed_5_14.string),  180, SIZE_14, "Dualscreen [5]" },
  { sizeof(tekst_inst.Twee_doek_een_bed_6_14.string),  180, SIZE_14, "Dualscreen [6]" },
  { sizeof(tekst_inst.Twee_doek_een_bed_7_14.string),  180, SIZE_14, "Dualscreen [7]" },
  { sizeof(tekst_inst.Twee_doek_een_bed_8_14.string),  180, SIZE_14, "Dualscreen [8]" },
  { sizeof(tekst_inst.Master_14.string),               180, SIZE_14, "Master" },
  { sizeof(tekst_inst.Standby_regeling_14.string),     180, SIZE_14, "Standby control" },
  { sizeof(tekst_inst.Geen_10.string),                 180, SIZE_10, "None" },
  { sizeof(tekst_inst.Tegenover_elkaar_10.string),     180, SIZE_10, "Opposite screens" },
  { sizeof(tekst_inst.Achter_elkaar_10.string),        180, SIZE_10, "Following screens" },

  { sizeof(tekst_inst.Opties_Cabriokas_10.string),     185, SIZE_10, "Options: Open roof" },
  { sizeof(tekst_inst.Cabriokas_1_14.string),          180, SIZE_14, "Open roof [1]" },
  { sizeof(tekst_inst.Cabriokas_2_14.string),          180, SIZE_14, "Open roof [2]" },
  { sizeof(tekst_inst.Cabriokas_3_14.string),          180, SIZE_14, "Open roof [3]" },
  { sizeof(tekst_inst.Cabriokas_4_14.string),          180, SIZE_14, "Open roof [4]" },
  { sizeof(tekst_inst.Cabriokas_5_14.string),          180, SIZE_14, "Open roof [5]" },
  { sizeof(tekst_inst.Cabriokas_6_14.string),          180, SIZE_14, "Open roof [6]" },
  { sizeof(tekst_inst.Cabriokas_7_14.string),          180, SIZE_14, "Open roof [7]" },
  { sizeof(tekst_inst.Cabriokas_8_14.string),          180, SIZE_14, "Open roof [8]" },
  { sizeof(tekst_inst.Voor_naloop_10.string),          180, SIZE_10, "Lead/Trail" },
  { sizeof(tekst_inst.Gelijkloop_10.string),           180, SIZE_10, "Synchronised" },
  { sizeof(tekst_inst.Voorloop_14.string),             180, SIZE_14, "Leading" },
  { sizeof(tekst_inst.Gelijkloop_14.string),           180, SIZE_14, "Synchronised" },
  { sizeof(tekst_inst.Hysteresis_14.string),           180, SIZE_14, "Hysteresis" },

  { sizeof(tekst_inst.Opties_Luchtmengkast_10.string), 185, SIZE_10, "Options: Luchtmengkast" }, // TD - nog vertalen
  { sizeof(tekst_inst.Luchtmengkast_14.string),        185, SIZE_14, "Luchtmengkast" },
  { sizeof(tekst_inst.Aantal_groepen_14.string),       180, SIZE_14, "Aantal groepen" },
  { sizeof(tekst_inst.Aantal_units_14.string),         180, SIZE_14, "Aantal units" },
  { sizeof(tekst_inst.Type_klep_14.string),            180, SIZE_14, "Type klep" },
  { sizeof(tekst_inst.Binnen_buiten_14.string),        180, SIZE_14, "Binnen/buiten" },
  { sizeof(tekst_inst.Recirculatie_14.string),         180, SIZE_14, "Recirculatie" },
  { sizeof(tekst_inst.Binnenklep_14.string),           180, SIZE_14, "Binnenklep" },
  { sizeof(tekst_inst.Buitenklep_14.string),           180, SIZE_14, "Buitenklep" },
  { sizeof(tekst_inst.Recirculatieklep_14.string),     180, SIZE_14, "Recirculatieklep" },
  { sizeof(tekst_inst.Bovenklep_14.string),            180, SIZE_14, "Bovenklep" },
  { sizeof(tekst_inst.Verwarming_14.string),           180, SIZE_14, "Verwarming" },
  { sizeof(tekst_inst.Inblaasvent_14.string),          180, SIZE_14, "Inblaasvent" },
  { sizeof(tekst_inst.Afblaasvent_14.string),          180, SIZE_14, "Afblaasvent" },
  { sizeof(tekst_inst.Inblaastemp_14.string),          180, SIZE_14, "Inblaastemperatuur" },
  { sizeof(tekst_inst.Mengtemp_14.string),             180, SIZE_14, "Mengtemperatuur" },
  { sizeof(tekst_inst.Streeftemp_14.string),           180, SIZE_14, "Streeftemperatuur" },
  { sizeof(tekst_inst.Vorstbewaking_14.string),        180, SIZE_14, "Vorstbewaking" },
  { sizeof(tekst_inst.Alarm_ingang_14.string),         180, SIZE_14, "Alarm ingang" },
  { sizeof(tekst_inst.Drukverschil_14.string),         180, SIZE_14, "Drukverschil" },
  { sizeof(tekst_inst.Units_14.string),                 80, SIZE_14, "Units" },
  { sizeof(tekst_inst.Groep_14.string),                 80, SIZE_14, "Groep" },
  { sizeof(tekst_inst.Naregelen_14.string),            180, SIZE_14, "Naregelen" },

  { sizeof(tekst_inst.Gekoppeld_aan_klep_14.string),   180, SIZE_14, "Gekoppeld aan klep" },

  { sizeof(tekst_inst.Raam_14.string),                 150, SIZE_14, "Window" },
  { sizeof(tekst_inst.Doek_14.string),                 150, SIZE_14, "Screen" },
  { sizeof(tekst_inst.Ventilatie_14.string),           150, SIZE_14, "Ventilation" },
  { sizeof(tekst_inst.Type_sturing_14.string),         180, SIZE_14, "Type control" },
  { sizeof(tekst_inst.Open_14.string),                 180, SIZE_14, "Open" },
  { sizeof(tekst_inst.Dicht_14.string),                180, SIZE_14, "Close" },
  { sizeof(tekst_inst.Hoger_14.string),                180, SIZE_14, "Higher" },
  { sizeof(tekst_inst.Lager_14.string),                180, SIZE_14, "Lower" },
  { sizeof(tekst_inst.Position_14.string),             180, SIZE_14, "Position" },
  { sizeof(tekst_inst.Terugmelding_14.string),         180, SIZE_14, "Feedback" },
  { sizeof(tekst_inst.Aantal_motoren_14.string),       180, SIZE_14, "Number motors" },
  { sizeof(tekst_inst.Motoren_14.string),              180, SIZE_14, "Motors" },
  { sizeof(tekst_inst.Master_nummer_14.string),        180, SIZE_14, "Master number" },
  { sizeof(tekst_inst.Frequentie_gestuurd_14.string),  180, SIZE_14, "Frequency controlled" },
  { sizeof(tekst_inst.Digitaal_14.string),             150, SIZE_14, "Digital" },
  { sizeof(tekst_inst.Analoog_14.string),              150, SIZE_14, "Analog" },
  { sizeof(tekst_inst.Hoge_snelheid_14.string),        180, SIZE_14, "High speed" },
  { sizeof(tekst_inst.Uitgang_snelheid_14.string),     180, SIZE_14, "Output speed" },
  { sizeof(tekst_inst.Snelheid_14.string),             180, SIZE_14, "Speed" },
  { sizeof(tekst_inst.Laag_14.string),                 150, SIZE_14, "Low" },
  { sizeof(tekst_inst.Hoog_14.string),                 150, SIZE_14, "High" },
  { sizeof(tekst_inst.Terugschakel_positie_14.string), 180, SIZE_14, "Position low speed" },
  { sizeof(tekst_inst.Pulse_system_14.string),         180, SIZE_14, "Pulse system" },
  { sizeof(tekst_inst.Kier_regeling_14.string),        180, SIZE_14, "Gap control" },
  { sizeof(tekst_inst.Alarm_analoog_14.string),        180, SIZE_14, "Alarm analogue" },

  { sizeof(tekst_inst.JA_10.string),                   45, SIZE_10, "YES" },
  { sizeof(tekst_inst.JA_14.string),                   45, SIZE_14, "YES" },
  { sizeof(tekst_inst.NEE_14.string),                  45, SIZE_14, "NO" },
  { sizeof(tekst_inst.AAN_14.string),                  45, SIZE_14, "ON" },
  { sizeof(tekst_inst.UIT_14.string),                  45, SIZE_14, "OFF" },
  { sizeof(tekst_inst.Zeker_weten_7.string),          100, SIZE_7,  "Are you sure?" },
  { sizeof(tekst_inst.Wissen_10.string),               65, SIZE_10, "delete" },
  { sizeof(tekst_inst.Kopieren_10.string),             65, SIZE_10, "Copy" },
  { sizeof(tekst_inst.Pa_7.string),                    13, SIZE_7,  "Pa" },
  { sizeof(tekst_inst.ppm_7.string),                   20, SIZE_7,  "ppm" },
  { sizeof(tekst_inst.perc_7.string),                   8, SIZE_7,  "%" },
  { sizeof(tekst_inst.mps_7.string),                   18, SIZE_7,  "m/s" },
  { sizeof(tekst_inst.mpmin_7.string),                 30, SIZE_7,  "m/min" },
  { sizeof(tekst_inst.graden_celsius_7.string),        10, SIZE_7,  "°C" },
  { sizeof(tekst_inst.graden_fahrenheid_7.string),     10, SIZE_7,  "°F" },
  { sizeof(tekst_inst.graden_7.string),                 8, SIZE_7,  "°" },
  { sizeof(tekst_inst.meter_7.string),                  8, SIZE_7,  "m" },

  { sizeof(tekst_inst.Alarm_Contact_14.string),       180, SIZE_14, "Alarm contact" },
  { sizeof(tekst_inst.Alarm_zacht_contact_14.string), 180, SIZE_14, "Warning contact" },
  { sizeof(tekst_inst.Alarm_urgent_14.string),        180, SIZE_14, "Alarm urgent" },
  { sizeof(tekst_inst.Opties_Kopieren_14.string),     185, SIZE_14, "Copy Options" },

  { sizeof(tekst_inst.COM1_USB_14.string),            185, SIZE_14, "COM1/USB" },            // COM1_USB_14
  { sizeof(tekst_inst.COM2_14.string),                185, SIZE_14, "COM2" },                // COM2_14
  { sizeof(tekst_inst.Modem_14.string),               185, SIZE_14, "Modem" },               // Modem_14
  { sizeof(tekst_inst.Answer_14.string),              150, SIZE_14, "Answer (ATS0=.)" },     // Answer_14
  { sizeof(tekst_inst.ETHERNET_14.string),            185, SIZE_14, "ETHERNET" },            // ETHERNET_14
  { sizeof(tekst_inst.IP_10.string),                   50, SIZE_10, "IP" },                  // IP_10
  { sizeof(tekst_inst.Mask_10.string),                 50, SIZE_10, "Mask" },                // Mask_10
  { sizeof(tekst_inst.Gate_10.string),                 50, SIZE_10, "Gate" },                // Gate_10
  { sizeof(tekst_inst.Port_10.string),                 50, SIZE_10, "Port" },                // Port_10

  { sizeof(tekst_inst.RS485_14.string),               150, SIZE_14, "RS485" },
  { sizeof(tekst_inst.RS485_baudrate_14.string),      150, SIZE_14, "RS485 baudrate" },
  { sizeof(tekst_inst.RS485_pariteit_14.string),      150, SIZE_14, "RS485 parity" },
  { sizeof(tekst_inst.Geen_14.string),                110, SIZE_14, "None" },
  { sizeof(tekst_inst.Even_14.string),                110, SIZE_14, "Even" },
  { sizeof(tekst_inst.Oneven_14.string),              110, SIZE_14, "Odd" },

  { sizeof(tekst_inst.Looptijd_14.string),            110, SIZE_14, "Running time" },
  { sizeof(tekst_inst.Aan_uit_14.string),             110, SIZE_14, "On/off" },

  { sizeof(tekst_inst.Absoluut_14.string),            180, SIZE_14, "Absolute" },                                            
  { sizeof(tekst_inst.Relatief_14.string),            180, SIZE_14, "Relative" },

  { sizeof(tekst_inst.Type_14.string),                180, SIZE_14, "Type" },
  { sizeof(tekst_inst.Adressen_14.string),            180, SIZE_14, "Addresses" },
  { sizeof(tekst_inst.Nummer_7.string),               180, SIZE_7,  "Number" },
  { sizeof(tekst_inst.niet_beschikbaar_7.string),     180, SIZE_7,  "not available" },
  { sizeof(tekst_inst.Nummer_10.string),              180, SIZE_10, "Number" },
  { sizeof(tekst_inst.Adres_10.string),               180, SIZE_10, "Address" },
  { sizeof(tekst_inst.Wijzig_adres_14.string),        180, SIZE_14, "Change address" },
  { sizeof(tekst_inst.Wijzigen_10.string),             65, SIZE_10, "change" },
  { sizeof(tekst_inst.gewijzigd_10.string),           180, SIZE_10, "changed" },
  { sizeof(tekst_inst.communicatie_fout_10.string),   180, SIZE_10, "communication error" },

  { sizeof(tekst_inst.Opties_Luchting_10.string),     185, SIZE_10, "Opties: Luchting" },
  { sizeof(tekst_inst.Luchting_14.string),            185, SIZE_14, "Luchting" },

  { sizeof(tekst_inst.Alarmen_14.string),             185, SIZE_14, "Alarms" },                                              
  { sizeof(tekst_inst.Opties_Alarmen_10.string),      185, SIZE_10, "Options: Alarms" },                                     

  { sizeof(tekst_inst.Frequentie_verstel_14.string),  185, SIZE_14, "Verstel hoge snelheid" },
  //#ifdef CAN_BACKBONE_PC_WARNING
  { sizeof(tekst_inst.PC_1_14.string),                      50, SIZE_14, "PC 1" },                // PC_1_14
  { sizeof(tekst_inst.PC_2_14.string),                      50, SIZE_14, "PC 2" },                // PC_2_14
  { sizeof(tekst_inst.PC_3_14.string),                      50, SIZE_14, "PC 3" },                // PC_3_14
  { sizeof(tekst_inst.Waarschuwing_7.string),               50, SIZE_7,  "Warning" },             // Waarschuwing_7
  //#endif // CAN_BACKBONE_PC_WARNING
  //#ifdef PASSWORD
  { sizeof(tekst_inst.Beheerder_14.string),                185, SIZE_14, "Beheerder" },           // Beheerder_14
  { sizeof(tekst_inst.Gebruiker_1_14.string),              185, SIZE_14, "Customer 1" },          // Gebruikers_1_14
  { sizeof(tekst_inst.Gebruiker_2_14.string),              185, SIZE_14, "Customer 2" },          // Gebruikers_2_14
  { sizeof(tekst_inst.Gebruiker_3_14.string),              185, SIZE_14, "Customer 3" },          // Gebruikers_3_14
  { sizeof(tekst_inst.Gebruiker_4_14.string),              185, SIZE_14, "Customer 4" },          // Gebruikers_4_14
  { sizeof(tekst_inst.Level_14.string),                     85, SIZE_14, "level:" },              // Level_14
  { sizeof(tekst_inst.Opties_7.string),                     80, SIZE_7,  "Options" },             // Opties_7
  { sizeof(tekst_inst.Setpoints_7.string),                  80, SIZE_7,  "Setpoints" },           // Setpoints_7
  { sizeof(tekst_inst.Setpoints_Syst_7.string),             80, SIZE_7,  "Setpoints Syst" },      // Setpoints_Syst_7
  //#endif // PASSWORD

  { sizeof(tekst_inst.BACnet_Device_ID_10.string),          80, SIZE_10, "Device ID" },
  { sizeof(tekst_inst.BACnet_Network_Nr_10.string),         80, SIZE_10, "Network Nr" },

  { sizeof(tekst_inst.Operation_Mode_14.string),           185, SIZE_14, "Operation Mode" },
  { sizeof(tekst_inst.Closed_Loop_14.string),              180, SIZE_14, "Closed Loop" },                                            
  { sizeof(tekst_inst.Open_Loop_14.string),                180, SIZE_14, "Open Loop" },

  { sizeof(tekst_inst.Vrijgave_ventilatoren_14.string),        180, SIZE_14, "Release inputs fans" },
  { sizeof(tekst_inst.Opties_Vrijgave_ventilatoren_10.string), 185, SIZE_10, "Opt: Release input fans" },
  { sizeof(tekst_inst.Vrijgave_14.string),                     180, SIZE_14, "Release inputs" },
  { sizeof(tekst_inst.Ventilatoren_14.string),                 180, SIZE_14, "Fans" },

  { sizeof(tekst_inst.PowerFactor_14.string),              185, SIZE_14, "Power factor" },
  { sizeof(tekst_inst.RampUpDown_14.string),               185, SIZE_14, "Ramp-Up/Down" },
  { sizeof(tekst_inst.RampUp_14.string),                   185, SIZE_14, "Ramp-Up" },
  { sizeof(tekst_inst.RampDown_14.string),                 185, SIZE_14, "Ramp-Down" },

  { sizeof(tekst_inst.Klep_14.string),                     150, SIZE_14, "Flap" },
  { sizeof(tekst_inst.SensorType_14.string),               185, SIZE_14, "Sensor type" },
  { sizeof(tekst_inst.Eindschakelaar_14.string),           110, SIZE_14, "Limitswitch" },

  { sizeof(tekst_inst.Sensoren_14.string),                 185, SIZE_14, "Sensors" },

  { sizeof(tekst_inst.Alarm_eindschakelaar_14.string),     180, SIZE_14, "Alarm limitswitch" },

  { sizeof(tekst_inst.Extern_alarm_14.string),             110, SIZE_14, "External alarm" },
  { sizeof(tekst_inst.Vent_At_Max_14.string),              185, SIZE_14, "Vent at 100%" },

  { sizeof(tekst_inst.Watchdog_Mode_14.string),            185, SIZE_14, "Watchdog Mode" },
  { sizeof(tekst_inst.Watchdog_Position_14.string),        185, SIZE_14, "Watchdog Position" },

  { sizeof(tekst_inst.Data_Wissen_14.string),              185, SIZE_14, "Delete data" },
  { sizeof(tekst_inst.Selecteer_Data_14.string),           185, SIZE_14, "Select data" },
  { sizeof(tekst_inst.Druk_OK_Om_Te_Wissen_14.string),     185, SIZE_14, "OK = Delete" },
  { sizeof(tekst_inst.Data_Gewist_14.string),              185, SIZE_14, "Data deleted!" },
  { sizeof(tekst_inst.Setpoints_10.string),                185, SIZE_10, "Setpoints" },
  { sizeof(tekst_inst.Werkgeheugen_10.string),             185, SIZE_10, "Memory" },
  { sizeof(tekst_inst.Restart_10.string),                  185, SIZE_10, "Restart" },
};

s_tekst_inst const tekst_inst_nederlands = 
{
  sizeof(s_tekst_inst), // unsinged int area_size;
  ORION,                // unsigned int computer;
  MULTI_CONNECT,        // unsigned int soort;
  VERSIE_TEKST_INST,    // unsigned int versie_tekst_inst;

  { sizeof(tekst_inst.Gekozen_Taal_14.string),         150, SIZE_14, "Nederlands" },

  { sizeof(tekst_inst.Opties_10.string),               174, SIZE_10, "Opties" },
  { sizeof(tekst_inst.Bekijken_Opties_14.string),      185, SIZE_14, "Bekijken Opties" },
  { sizeof(tekst_inst.Wijzigen_Opties_14.string),      185, SIZE_14, "Wijzigen Opties" },
  { sizeof(tekst_inst.Orion_Ingeschakeld_14.string),   185, SIZE_14, "Orion Ingeschakeld" },
  { sizeof(tekst_inst.Orion_Uitgeschakeld_14.string),  185, SIZE_14, "Orion Uitgeschakeld" },

  { sizeof(tekst_inst.Algemeen_14.string),             185, SIZE_14, "Algemeen" },
  { sizeof(tekst_inst.IO_14.string),                   185, SIZE_14, "IO" },
  { sizeof(tekst_inst.Motorgroepen_14.string),         185, SIZE_14, "Motorgroepen" },
  { sizeof(tekst_inst.Groep_1_14.string),              185, SIZE_14, "Groep 1" },
  { sizeof(tekst_inst.Groep_2_14.string),              185, SIZE_14, "Groep 2" },
  { sizeof(tekst_inst.Groep_3_14.string),              185, SIZE_14, "Groep 3" },
  { sizeof(tekst_inst.Groep_4_14.string),              185, SIZE_14, "Groep 4" },
  { sizeof(tekst_inst.Groep_5_14.string),              185, SIZE_14, "Groep 5" },
  { sizeof(tekst_inst.Groep_6_14.string),              185, SIZE_14, "Groep 6" },
  { sizeof(tekst_inst.Groep_7_14.string),              185, SIZE_14, "Groep 7" },
  { sizeof(tekst_inst.Groep_8_14.string),              185, SIZE_14, "Groep 8" },
  { sizeof(tekst_inst.Groep_9_14.string),              185, SIZE_14, "Groep 9" },
  { sizeof(tekst_inst.Groep_10_14.string),             185, SIZE_14, "Groep 10" },
  { sizeof(tekst_inst.Groep_11_14.string),             185, SIZE_14, "Groep 11" },
  { sizeof(tekst_inst.Groep_12_14.string),             185, SIZE_14, "Groep 12" },
  { sizeof(tekst_inst.Groep_13_14.string),             185, SIZE_14, "Groep 13" },
  { sizeof(tekst_inst.Groep_14_14.string),             185, SIZE_14, "Groep 14" },
  { sizeof(tekst_inst.Groep_15_14.string),             185, SIZE_14, "Groep 15" },
  { sizeof(tekst_inst.Groep_16_14.string),             185, SIZE_14, "Groep 16" },
  { sizeof(tekst_inst.Groep_17_14.string),             185, SIZE_14, "Groep 17" },
  { sizeof(tekst_inst.Groep_18_14.string),             185, SIZE_14, "Groep 18" },
  { sizeof(tekst_inst.Groep_19_14.string),             185, SIZE_14, "Groep 19" },
  { sizeof(tekst_inst.Groep_20_14.string),             185, SIZE_14, "Groep 20" },
  { sizeof(tekst_inst.Groep_21_14.string),             185, SIZE_14, "Groep 21" },
  { sizeof(tekst_inst.Groep_22_14.string),             185, SIZE_14, "Groep 22" },
  { sizeof(tekst_inst.Groep_23_14.string),             185, SIZE_14, "Groep 23" },
  { sizeof(tekst_inst.Groep_24_14.string),             185, SIZE_14, "Groep 24" },
  { sizeof(tekst_inst.Groep_25_14.string),             185, SIZE_14, "Groep 25" },
  { sizeof(tekst_inst.Groep_26_14.string),             185, SIZE_14, "Groep 26" },
  { sizeof(tekst_inst.Groep_27_14.string),             185, SIZE_14, "Groep 27" },
  { sizeof(tekst_inst.Groep_28_14.string),             185, SIZE_14, "Groep 28" },
  { sizeof(tekst_inst.Groep_29_14.string),             185, SIZE_14, "Groep 29" },
  { sizeof(tekst_inst.Groep_30_14.string),             185, SIZE_14, "Groep 30" },
  { sizeof(tekst_inst.Groep_31_14.string),             185, SIZE_14, "Groep 31" },
  { sizeof(tekst_inst.Groep_32_14.string),             185, SIZE_14, "Groep 32" },
  { sizeof(tekst_inst.Twee_doek_een_bed_14.string),    185, SIZE_14, "2 doeken op 1 bed" },
  { sizeof(tekst_inst.Cabriokas_14.string),            185, SIZE_14, "Cabriokas" },

  { sizeof(tekst_inst.Opties_Doorlopen_14.string),     220, SIZE_14, "Opties doorlopen?" },
  { sizeof(tekst_inst.Opties_OK_14.string),            220, SIZE_14, "Opties OK?" },

  { sizeof(tekst_inst.Opties_algemeen_10.string),      185, SIZE_10, "Opties: Algemeen" },
  { sizeof(tekst_inst.Taal_14.string),                 185, SIZE_14, "Taal" },
  { sizeof(tekst_inst.Opties_Wissen_14.string),        185, SIZE_14, "Opties Wissen" },
  { sizeof(tekst_inst.Opties_gewist_14.string),        210, SIZE_14, "Opties gewist!" },
  { sizeof(tekst_inst.Setpoints_Wissen_14.string),     185, SIZE_14, "Setpoints Wissen" },
  { sizeof(tekst_inst.Setpoints_gewist_14.string),     210, SIZE_14, "Setpoints gewist!" },
  { sizeof(tekst_inst.Helderheid_14.string),           185, SIZE_14, "Helderheid" },
  { sizeof(tekst_inst.LCD_dimmen_14.string),           185, SIZE_14, "LCD dimmen" },
  { sizeof(tekst_inst.Computer_14.string),             185, SIZE_14, "Computer" },
  { sizeof(tekst_inst.Nummer_14.string),               135, SIZE_14, "Nummer" },
  { sizeof(tekst_inst.Adres_14.string),                135, SIZE_14, "Adres" },
  { sizeof(tekst_inst.CAN_BACKBONE_14.string),         140, SIZE_14, "CAN BACKBONE" },        // CAN_BACKBONE_14
  { sizeof(tekst_inst.CAN_RS232_14.string),            140, SIZE_14, "CAN-RS232" },           // CAN_RS232_14
  { sizeof(tekst_inst.Waarschuwing_14.string),         140, SIZE_14, "Waarschuwing" },        // Waarschuwing_14
  { sizeof(tekst_inst.RS232_14.string),                185, SIZE_14, "RS232" },
  { sizeof(tekst_inst.kBd_14.string),                   30, SIZE_14, "kBd" },
  { sizeof(tekst_inst.Installateurs_14.string),        185, SIZE_14, "Installateurs" },
  { sizeof(tekst_inst.Gebruikers_14.string),           185, SIZE_14, "Gebruikers" },
  { sizeof(tekst_inst.PC_14.string),                   185, SIZE_14, "PC" },
  { sizeof(tekst_inst.Wachtwoord_14.string),           110, SIZE_14, "code:" },
  { sizeof(tekst_inst.Herhaal_14.string),              110, SIZE_14, "herhaal:" },
  { sizeof(tekst_inst.Wachtwoord_ongelijk_7.string),   100, SIZE_7,  "passwords ongelijk!" },

  { sizeof(tekst_inst.Opties_IO_10.string),            185, SIZE_10, "Opties: IO" },
  { sizeof(tekst_inst.Versie_7.string),                 40, SIZE_7,  "Versie:" },

  { sizeof(tekst_inst.Opties_bord_10.string),           66, SIZE_10, "Opties:" },
  { sizeof(tekst_inst.Nummer_IO_Module_14.string),     220, SIZE_14, "Nummer IO Module" },
  { sizeof(tekst_inst.Verwijderen_14.string),          150, SIZE_14, "Verwijderen" },
  { sizeof(tekst_inst.Analoge_Ingang_14.string),       150, SIZE_14, "Analoge Ingang" },
  { sizeof(tekst_inst.Digitale_Ingang_14.string),      150, SIZE_14, "Digitale Ingang" },
  { sizeof(tekst_inst.Analoge_Uitgang_14.string),      150, SIZE_14, "Analoge Uitgang" },
  { sizeof(tekst_inst.Digitale_Uitgang_14.string),     150, SIZE_14, "Digitale Uitgang" },
  { sizeof(tekst_inst.Motor_Control_14.string),        150, SIZE_14, "Motor control" },
  { sizeof(tekst_inst.Instellen_sensor_14.string),     130, SIZE_14, "Instellen sensor" },
  { sizeof(tekst_inst.IJken_sensor_14.string),         130, SIZE_14, "IJken sensor" },
  { sizeof(tekst_inst.IJken_Minimum_14.string),        130, SIZE_14, "IJken Minimum" },                                      
  { sizeof(tekst_inst.IJken_Maximum_14.string),        130, SIZE_14, "IJken Maximum" },                                      
  { sizeof(tekst_inst.Instellen_14.string),            130, SIZE_14, "Instellen" },
  { sizeof(tekst_inst.IJken_14.string),                130, SIZE_14, "IJken" },
  { sizeof(tekst_inst.V_14.string),                     12, SIZE_14, "V" },
  { sizeof(tekst_inst.mA_14.string),                    12, SIZE_14, "mA" },
  { sizeof(tekst_inst.Overnemen_10.string),             77, SIZE_10, "Overnemen" },
  { sizeof(tekst_inst.Windrichting_14.string),         210, SIZE_14, "Windrichting" },
  { sizeof(tekst_inst.Windsnelheid_bij_5V_14.string),  210, SIZE_14, "Windsnelheid bij 5.0V" },
  { sizeof(tekst_inst.kg_14.string),                    19, SIZE_14, "kg" },
  { sizeof(tekst_inst.g_14.string),                     19, SIZE_14, "g" },
  { sizeof(tekst_inst.Pulsen_Per_Liter_14.string),     210, SIZE_14, "Pulsen Per Liter" },
  { sizeof(tekst_inst.Liters_Per_Puls_14.string),      210, SIZE_14, "Liters Per Puls" },
  { sizeof(tekst_inst.Pulsen_Per_Kg_14.string),        210, SIZE_14, "Pulsen Per Kg" },
  { sizeof(tekst_inst.Kg_Per_Puls_14.string),          210, SIZE_14, "Kg Per Puls" },
  { sizeof(tekst_inst.Pulsen_Per_Ei_14.string),        210, SIZE_14, "Pulsen Per Ei" },
  { sizeof(tekst_inst.Eieren_Per_Puls_14.string),      210, SIZE_14, "Eieren Per Puls" },
  { sizeof(tekst_inst.Pulsen_Per_kWh_14.string),       210, SIZE_14, "Pulsen Per kWh" },
  { sizeof(tekst_inst.kWh_Per_Puls_14.string),         210, SIZE_14, "kWh Per Puls" },
  { sizeof(tekst_inst.Minimum_14.string),              100, SIZE_14, "Minimum:" },
  { sizeof(tekst_inst.Maximum_14.string),              100, SIZE_14, "Maximum:" },
  { sizeof(tekst_inst.Aantal_Tellers_14.string),       135, SIZE_14, "Aantal tellers" },
  { sizeof(tekst_inst.Teller_14.string),               100, SIZE_14, "teller" },
  { sizeof(tekst_inst.Fout_Licht_Niveau_14.string),    180, SIZE_14, "Fout Licht Niveau" },
  { sizeof(tekst_inst.Fout_Communicatie_14.string),    180, SIZE_14, "Fout Communicatie" },
  { sizeof(tekst_inst.Fout_Onbekend_14.string),        180, SIZE_14, "Fout Onbekend" },
  { sizeof(tekst_inst.Niet_Toegewezen_14.string),      150, SIZE_14, "Niet Toegewezen" },

  { sizeof(tekst_inst.Opties_motorgroepen_10.string),  185, SIZE_10, "Opties: Motorgroepen" },
  { sizeof(tekst_inst.Aantal_motorgroepen_14.string),  180, SIZE_14, "Motorgroepen" },

  { sizeof(tekst_inst.Opties_groep_1_10.string),       185, SIZE_10, "Opties: Groep 1" },
  { sizeof(tekst_inst.Opties_groep_2_10.string),       185, SIZE_10, "Opties: Groep 2" },
  { sizeof(tekst_inst.Opties_groep_3_10.string),       185, SIZE_10, "Opties: Groep 3" },
  { sizeof(tekst_inst.Opties_groep_4_10.string),       185, SIZE_10, "Opties: Groep 4" },
  { sizeof(tekst_inst.Opties_groep_5_10.string),       185, SIZE_10, "Opties: Groep 5" },
  { sizeof(tekst_inst.Opties_groep_6_10.string),       185, SIZE_10, "Opties: Groep 6" },
  { sizeof(tekst_inst.Opties_groep_7_10.string),       185, SIZE_10, "Opties: Groep 7" },
  { sizeof(tekst_inst.Opties_groep_8_10.string),       185, SIZE_10, "Opties: Groep 8" },
  { sizeof(tekst_inst.Opties_groep_9_10.string),       185, SIZE_10, "Opties: Groep 9" },
  { sizeof(tekst_inst.Opties_groep_10_10.string),      185, SIZE_10, "Opties: Groep 10" },
  { sizeof(tekst_inst.Opties_groep_11_10.string),      185, SIZE_10, "Opties: Groep 11" },
  { sizeof(tekst_inst.Opties_groep_12_10.string),      185, SIZE_10, "Opties: Groep 12" },
  { sizeof(tekst_inst.Opties_groep_13_10.string),      185, SIZE_10, "Opties: Groep 13" },
  { sizeof(tekst_inst.Opties_groep_14_10.string),      185, SIZE_10, "Opties: Groep 14" },
  { sizeof(tekst_inst.Opties_groep_15_10.string),      185, SIZE_10, "Opties: Groep 15" },
  { sizeof(tekst_inst.Opties_groep_16_10.string),      185, SIZE_10, "Opties: Groep 16" },
  { sizeof(tekst_inst.Opties_groep_17_10.string),      185, SIZE_10, "Opties: Groep 17" },
  { sizeof(tekst_inst.Opties_groep_18_10.string),      185, SIZE_10, "Opties: Groep 18" },
  { sizeof(tekst_inst.Opties_groep_19_10.string),      185, SIZE_10, "Opties: Groep 19" },
  { sizeof(tekst_inst.Opties_groep_20_10.string),      185, SIZE_10, "Opties: Groep 20" },
  { sizeof(tekst_inst.Opties_groep_21_10.string),      185, SIZE_10, "Opties: Groep 21" },
  { sizeof(tekst_inst.Opties_groep_22_10.string),      185, SIZE_10, "Opties: Groep 22" },
  { sizeof(tekst_inst.Opties_groep_23_10.string),      185, SIZE_10, "Opties: Groep 23" },
  { sizeof(tekst_inst.Opties_groep_24_10.string),      185, SIZE_10, "Opties: Groep 24" },
  { sizeof(tekst_inst.Opties_groep_25_10.string),      185, SIZE_10, "Opties: Groep 25" },
  { sizeof(tekst_inst.Opties_groep_26_10.string),      185, SIZE_10, "Opties: Groep 26" },
  { sizeof(tekst_inst.Opties_groep_27_10.string),      185, SIZE_10, "Opties: Groep 27" },
  { sizeof(tekst_inst.Opties_groep_28_10.string),      185, SIZE_10, "Opties: Groep 28" },
  { sizeof(tekst_inst.Opties_groep_29_10.string),      185, SIZE_10, "Opties: Groep 29" },
  { sizeof(tekst_inst.Opties_groep_30_10.string),      185, SIZE_10, "Opties: Groep 30" },
  { sizeof(tekst_inst.Opties_groep_31_10.string),      185, SIZE_10, "Opties: Groep 31" },
  { sizeof(tekst_inst.Opties_groep_32_10.string),      185, SIZE_10, "Opties: Groep 32" },

  { sizeof(tekst_inst.Opties_2_doek_1_bed_10.string),  185, SIZE_10, "Opties: 2 doeken op 1 bed" },
  { sizeof(tekst_inst.Aantal_14.string),               180, SIZE_14, "Aantal" },
  { sizeof(tekst_inst.Twee_doek_een_bed_1_14.string),  180, SIZE_14, "2 doeken op 1 bed [1]" },
  { sizeof(tekst_inst.Twee_doek_een_bed_2_14.string),  180, SIZE_14, "2 doeken op 1 bed [2]" },
  { sizeof(tekst_inst.Twee_doek_een_bed_3_14.string),  180, SIZE_14, "2 doeken op 1 bed [3]" },
  { sizeof(tekst_inst.Twee_doek_een_bed_4_14.string),  180, SIZE_14, "2 doeken op 1 bed [4]" },
  { sizeof(tekst_inst.Twee_doek_een_bed_5_14.string),  180, SIZE_14, "2 doeken op 1 bed [5]" },
  { sizeof(tekst_inst.Twee_doek_een_bed_6_14.string),  180, SIZE_14, "2 doeken op 1 bed [6]" },
  { sizeof(tekst_inst.Twee_doek_een_bed_7_14.string),  180, SIZE_14, "2 doeken op 1 bed [7]" },
  { sizeof(tekst_inst.Twee_doek_een_bed_8_14.string),  180, SIZE_14, "2 doeken op 1 bed [8]" },
  { sizeof(tekst_inst.Master_14.string),               180, SIZE_14, "Master" },
  { sizeof(tekst_inst.Standby_regeling_14.string),     180, SIZE_14, "Slaapstand" },
  { sizeof(tekst_inst.Geen_10.string),                 180, SIZE_10, "Geen" },
  { sizeof(tekst_inst.Tegenover_elkaar_10.string),     180, SIZE_10, "Tegenover elkaar" },
  { sizeof(tekst_inst.Achter_elkaar_10.string),        180, SIZE_10, "Achter elkaar" },

  { sizeof(tekst_inst.Opties_Cabriokas_10.string),     185, SIZE_10, "Opties: Cabriokas" },
  { sizeof(tekst_inst.Cabriokas_1_14.string),          180, SIZE_14, "Cabriokas [1]" },
  { sizeof(tekst_inst.Cabriokas_2_14.string),          180, SIZE_14, "Cabriokas [2]" },
  { sizeof(tekst_inst.Cabriokas_3_14.string),          180, SIZE_14, "Cabriokas [3]" },
  { sizeof(tekst_inst.Cabriokas_4_14.string),          180, SIZE_14, "Cabriokas [4]" },
  { sizeof(tekst_inst.Cabriokas_5_14.string),          180, SIZE_14, "Cabriokas [5]" },
  { sizeof(tekst_inst.Cabriokas_6_14.string),          180, SIZE_14, "Cabriokas [6]" },
  { sizeof(tekst_inst.Cabriokas_7_14.string),          180, SIZE_14, "Cabriokas [7]" },
  { sizeof(tekst_inst.Cabriokas_8_14.string),          180, SIZE_14, "Cabriokas [8]" },
  { sizeof(tekst_inst.Voor_naloop_10.string),          180, SIZE_10, "Voor-/Naloop" },
  { sizeof(tekst_inst.Gelijkloop_10.string),           180, SIZE_10, "Gelijkloop" },
  { sizeof(tekst_inst.Voorloop_14.string),             180, SIZE_14, "Voorloop" },
  { sizeof(tekst_inst.Gelijkloop_14.string),           180, SIZE_14, "Gelijkloop" },
  { sizeof(tekst_inst.Hysteresis_14.string),           180, SIZE_14, "Hysterese" },

  { sizeof(tekst_inst.Opties_Luchtmengkast_10.string), 185, SIZE_10, "Opties: Luchtmengkast" }, // TD - nog vertalen
  { sizeof(tekst_inst.Luchtmengkast_14.string),        185, SIZE_14, "Luchtmengkast" },
  { sizeof(tekst_inst.Aantal_groepen_14.string),       180, SIZE_14, "Aantal groepen" },
  { sizeof(tekst_inst.Aantal_units_14.string),         180, SIZE_14, "Aantal units" },
  { sizeof(tekst_inst.Type_klep_14.string),            180, SIZE_14, "Type klep" },
  { sizeof(tekst_inst.Binnen_buiten_14.string),        180, SIZE_14, "Binnen/buiten" },
  { sizeof(tekst_inst.Recirculatie_14.string),         180, SIZE_14, "Recirculatie" },
  { sizeof(tekst_inst.Binnenklep_14.string),           180, SIZE_14, "Binnenklep" },
  { sizeof(tekst_inst.Buitenklep_14.string),           180, SIZE_14, "Buitenklep" },
  { sizeof(tekst_inst.Recirculatieklep_14.string),     180, SIZE_14, "Recirculatieklep" },
  { sizeof(tekst_inst.Bovenklep_14.string),            180, SIZE_14, "Bovenklep" },
  { sizeof(tekst_inst.Verwarming_14.string),           180, SIZE_14, "Verwarming" },
  { sizeof(tekst_inst.Inblaasvent_14.string),          180, SIZE_14, "Inblaasvent" },
  { sizeof(tekst_inst.Afblaasvent_14.string),          180, SIZE_14, "Afblaasvent" },
  { sizeof(tekst_inst.Inblaastemp_14.string),          180, SIZE_14, "Inblaastemperatuur" },
  { sizeof(tekst_inst.Mengtemp_14.string),             180, SIZE_14, "Mengtemperatuur" },
  { sizeof(tekst_inst.Streeftemp_14.string),           180, SIZE_14, "Streeftemperatuur" },
  { sizeof(tekst_inst.Vorstbewaking_14.string),        180, SIZE_14, "Vorstbewaking" },
  { sizeof(tekst_inst.Alarm_ingang_14.string),         180, SIZE_14, "Alarm ingang" },
  { sizeof(tekst_inst.Drukverschil_14.string),         180, SIZE_14, "Drukverschil" },
  { sizeof(tekst_inst.Units_14.string),                 80, SIZE_14, "Units" },
  { sizeof(tekst_inst.Groep_14.string),                 80, SIZE_14, "Groep" },
  { sizeof(tekst_inst.Naregelen_14.string),            180, SIZE_14, "Naregelen" },

  { sizeof(tekst_inst.Gekoppeld_aan_klep_14.string),   180, SIZE_14, "Gekoppeld aan klep" },

  { sizeof(tekst_inst.Raam_14.string),                 150, SIZE_14, "Raam" },
  { sizeof(tekst_inst.Doek_14.string),                 150, SIZE_14, "Doek" },
  { sizeof(tekst_inst.Ventilatie_14.string),           150, SIZE_14, "Ventilatie" },
  { sizeof(tekst_inst.Type_sturing_14.string),         180, SIZE_14, "Type sturing" },
  { sizeof(tekst_inst.Open_14.string),                 180, SIZE_14, "Open" },
  { sizeof(tekst_inst.Dicht_14.string),                180, SIZE_14, "Dicht" },
  { sizeof(tekst_inst.Hoger_14.string),                180, SIZE_14, "Hoger" },
  { sizeof(tekst_inst.Lager_14.string),                180, SIZE_14, "Lager" },
  { sizeof(tekst_inst.Position_14.string),             180, SIZE_14, "Positie" },
  { sizeof(tekst_inst.Terugmelding_14.string),         180, SIZE_14, "Terugmelding" },
  { sizeof(tekst_inst.Aantal_motoren_14.string),       180, SIZE_14, "Aantal motoren" },
  { sizeof(tekst_inst.Motoren_14.string),              180, SIZE_14, "Motoren" },
  { sizeof(tekst_inst.Master_nummer_14.string),        180, SIZE_14, "Master nummer" },
  { sizeof(tekst_inst.Frequentie_gestuurd_14.string),  180, SIZE_14, "Frequentie gestuurd" },
  { sizeof(tekst_inst.Digitaal_14.string),             150, SIZE_14, "Digitaal" },
  { sizeof(tekst_inst.Analoog_14.string),              150, SIZE_14, "Analoog" },
  { sizeof(tekst_inst.Hoge_snelheid_14.string),        180, SIZE_14, "Hoge snelheid" },
  { sizeof(tekst_inst.Uitgang_snelheid_14.string),     180, SIZE_14, "Uitgang snelheid" },
  { sizeof(tekst_inst.Snelheid_14.string),             180, SIZE_14, "Snelheid" },
  { sizeof(tekst_inst.Laag_14.string),                 150, SIZE_14, "Laag" },
  { sizeof(tekst_inst.Hoog_14.string),                 150, SIZE_14, "Hoog" },
  { sizeof(tekst_inst.Terugschakel_positie_14.string), 180, SIZE_14, "Terugschakel positie" },
  { sizeof(tekst_inst.Pulse_system_14.string),         180, SIZE_14, "Puls systeem" },
  { sizeof(tekst_inst.Kier_regeling_14.string),        180, SIZE_14, "Kier regeling" },
  { sizeof(tekst_inst.Alarm_analoog_14.string),        180, SIZE_14, "Alarm analoog" },

  { sizeof(tekst_inst.JA_10.string),                   45, SIZE_10, "JA" },
  { sizeof(tekst_inst.JA_14.string),                   45, SIZE_14, "JA" },
  { sizeof(tekst_inst.NEE_14.string),                  45, SIZE_14, "NEE" },
  { sizeof(tekst_inst.AAN_14.string),                  45, SIZE_14, "AAN" },
  { sizeof(tekst_inst.UIT_14.string),                  45, SIZE_14, "UIT" },
  { sizeof(tekst_inst.Zeker_weten_7.string),          100, SIZE_7,  "Zeker weten?" },
  { sizeof(tekst_inst.Wissen_10.string),               65, SIZE_10, "Wissen!" },
  { sizeof(tekst_inst.Kopieren_10.string),             65, SIZE_10, "Kopiëren" },
  { sizeof(tekst_inst.Pa_7.string),                    13, SIZE_7,  "Pa" },
  { sizeof(tekst_inst.ppm_7.string),                   20, SIZE_7,  "ppm" },
  { sizeof(tekst_inst.perc_7.string),                   8, SIZE_7,  "%" },
  { sizeof(tekst_inst.mps_7.string),                   18, SIZE_7,  "m/s" },
  { sizeof(tekst_inst.mpmin_7.string),                 30, SIZE_7,  "m/min" },
  { sizeof(tekst_inst.graden_celsius_7.string),        10, SIZE_7,  "°C" },
  { sizeof(tekst_inst.graden_fahrenheid_7.string),     10, SIZE_7,  "°F" },
  { sizeof(tekst_inst.graden_7.string),                 8, SIZE_7,  "°" },
  { sizeof(tekst_inst.meter_7.string),                  8, SIZE_7,  "m" },

  { sizeof(tekst_inst.Alarm_Contact_14.string),       180, SIZE_14, "Alarm contact" },
  { sizeof(tekst_inst.Alarm_zacht_contact_14.string), 180, SIZE_14, "Alarm zacht contact" },
  { sizeof(tekst_inst.Alarm_urgent_14.string),        180, SIZE_14, "Alarm urgent" },
  { sizeof(tekst_inst.Opties_Kopieren_14.string),     185, SIZE_14, "Opties Kopiëren" },

  { sizeof(tekst_inst.COM1_USB_14.string),            185, SIZE_14, "COM1/USB" },            // COM1_USB_14
  { sizeof(tekst_inst.COM2_14.string),                185, SIZE_14, "COM2" },                // COM2_14
  { sizeof(tekst_inst.Modem_14.string),               185, SIZE_14, "Modem" },               // Modem_14
  { sizeof(tekst_inst.Answer_14.string),              150, SIZE_14, "Answer (ATS0=.)" },     // Answer_14
  { sizeof(tekst_inst.ETHERNET_14.string),            185, SIZE_14, "ETHERNET" },            // ETHERNET_14
  { sizeof(tekst_inst.IP_10.string),                   50, SIZE_10, "IP" },                  // IP_10
  { sizeof(tekst_inst.Mask_10.string),                 50, SIZE_10, "Mask" },                // Mask_10
  { sizeof(tekst_inst.Gate_10.string),                 50, SIZE_10, "Gate" },                // Gate_10
  { sizeof(tekst_inst.Port_10.string),                 50, SIZE_10, "Port" },                // Port_10

  { sizeof(tekst_inst.RS485_14.string),               150, SIZE_14, "RS485" },
  { sizeof(tekst_inst.RS485_baudrate_14.string),      150, SIZE_14, "RS485 baudrate" },
  { sizeof(tekst_inst.RS485_pariteit_14.string),      150, SIZE_14, "RS485 pariteit" },
  { sizeof(tekst_inst.Geen_14.string),                110, SIZE_14, "Geen" },
  { sizeof(tekst_inst.Even_14.string),                110, SIZE_14, "Even" },
  { sizeof(tekst_inst.Oneven_14.string),              110, SIZE_14, "Oneven" },

  { sizeof(tekst_inst.Looptijd_14.string),            110, SIZE_14, "Looptijd" },
  { sizeof(tekst_inst.Aan_uit_14.string),             110, SIZE_14, "Aan/uit" },

  { sizeof(tekst_inst.Absoluut_14.string),            180, SIZE_14, "Absoluut" },            
  { sizeof(tekst_inst.Relatief_14.string),            180, SIZE_14, "Relatief" },            

  { sizeof(tekst_inst.Type_14.string),                180, SIZE_14, "Type" },
  { sizeof(tekst_inst.Adressen_14.string),            180, SIZE_14, "Adressen" },
  { sizeof(tekst_inst.Nummer_7.string),               180, SIZE_7,  "Nummer" },
  { sizeof(tekst_inst.niet_beschikbaar_7.string),     180, SIZE_7,  "niet beschikbaar" },
  { sizeof(tekst_inst.Nummer_10.string),              180, SIZE_10, "Nummer" },
  { sizeof(tekst_inst.Adres_10.string),               180, SIZE_10, "Adres" },
  { sizeof(tekst_inst.Wijzig_adres_14.string),        180, SIZE_14, "Wijzig adres" },
  { sizeof(tekst_inst.Wijzigen_10.string),             65, SIZE_10, "wijzigen" },
  { sizeof(tekst_inst.gewijzigd_10.string),           180, SIZE_10, "gewijzigd" },
  { sizeof(tekst_inst.communicatie_fout_10.string),   180, SIZE_10, "communicatie fout" },

  { sizeof(tekst_inst.Opties_Luchting_10.string),     185, SIZE_10, "Opties: Luchting" },
  { sizeof(tekst_inst.Luchting_14.string),            185, SIZE_14, "Luchting" },

  { sizeof(tekst_inst.Alarmen_14.string),             185, SIZE_14, "Alarmen" },                                             
  { sizeof(tekst_inst.Opties_Alarmen_10.string),      185, SIZE_10, "Opties: Alarmen" },                                     

  { sizeof(tekst_inst.Frequentie_verstel_14.string),  185, SIZE_14, "Verstel hoge snelheid" },
  //#ifdef CAN_BACKBONE_PC_WARNING
  { sizeof(tekst_inst.PC_1_14.string),                      50, SIZE_14, "PC 1" },                // PC_1_14
  { sizeof(tekst_inst.PC_2_14.string),                      50, SIZE_14, "PC 2" },                // PC_2_14
  { sizeof(tekst_inst.PC_3_14.string),                      50, SIZE_14, "PC 3" },                // PC_3_14
  { sizeof(tekst_inst.Waarschuwing_7.string),               50, SIZE_7,  "Waarschuwing" },        // Waarschuwing_7
  //#endif // CAN_BACKBONE_PC_WARNING
  //#ifdef PASSWORD
  { sizeof(tekst_inst.Beheerder_14.string),                185, SIZE_14, "Beheerder" },           // Beheerder_14
  { sizeof(tekst_inst.Gebruiker_1_14.string),              185, SIZE_14, "Gebruiker 1" },         // Gebruiker_1_14
  { sizeof(tekst_inst.Gebruiker_2_14.string),              185, SIZE_14, "Gebruiker 2" },         // Gebruiker_2_14
  { sizeof(tekst_inst.Gebruiker_3_14.string),              185, SIZE_14, "Gebruiker 3" },         // Gebruiker_3_14
  { sizeof(tekst_inst.Gebruiker_4_14.string),              185, SIZE_14, "Gebruiker 4" },         // Gebruiker_4_14
  { sizeof(tekst_inst.Level_14.string),                     85, SIZE_14, "level:" },              // Level_14
  { sizeof(tekst_inst.Opties_7.string),                     80, SIZE_7,  "Options" },             // Opties_7
  { sizeof(tekst_inst.Setpoints_7.string),                  80, SIZE_7,  "Setpoints" },           // Setpoints_7
  { sizeof(tekst_inst.Setpoints_Syst_7.string),             80, SIZE_7,  "Setpoints Syst" },      // Setpoints_Syst_7
  //#endif // PASSWORD

  { sizeof(tekst_inst.BACnet_Device_ID_10.string),          80, SIZE_10,  "Device ID" },
  { sizeof(tekst_inst.BACnet_Network_Nr_10.string),         80, SIZE_10,  "Network Nr" },

  { sizeof(tekst_inst.Operation_Mode_14.string),           185, SIZE_14, "Operation Mode" },
  { sizeof(tekst_inst.Closed_Loop_14.string),              180, SIZE_14, "Closed Loop" },                                            
  { sizeof(tekst_inst.Open_Loop_14.string),                180, SIZE_14, "Open Loop" },

  { sizeof(tekst_inst.Vrijgave_ventilatoren_14.string),        180, SIZE_14, "Vrijgave ventilatoren" },
  { sizeof(tekst_inst.Opties_Vrijgave_ventilatoren_10.string), 185, SIZE_10, "Opt: Vrijgave ventilatoren" },
  { sizeof(tekst_inst.Vrijgave_14.string),                     180, SIZE_14, "Vrijgave" },
  { sizeof(tekst_inst.Ventilatoren_14.string),                 180, SIZE_14, "Ventilatoren" },

  { sizeof(tekst_inst.PowerFactor_14.string),              185, SIZE_14, "Power factor" },
  { sizeof(tekst_inst.RampUpDown_14.string),               185, SIZE_14, "Ramp-Up/Down" },
  { sizeof(tekst_inst.RampUp_14.string),                   185, SIZE_14, "Ramp-Up" },
  { sizeof(tekst_inst.RampDown_14.string),                 185, SIZE_14, "Ramp-Down" },

  { sizeof(tekst_inst.Klep_14.string),                     150, SIZE_14, "Klep" },
  { sizeof(tekst_inst.SensorType_14.string),               185, SIZE_14, "Sensor type" },
  { sizeof(tekst_inst.Eindschakelaar_14.string),           110, SIZE_14, "Eindschakelaar" },

  { sizeof(tekst_inst.Sensoren_14.string),                 185, SIZE_14, "Sensoren" },

  { sizeof(tekst_inst.Alarm_eindschakelaar_14.string),     180, SIZE_14, "Alarm eindschakelaar" },

  { sizeof(tekst_inst.Extern_alarm_14.string),             110, SIZE_14, "Extern alarm" },
  { sizeof(tekst_inst.Vent_At_Max_14.string),              185, SIZE_14, "Vent bij 100%" },

  { sizeof(tekst_inst.Watchdog_Mode_14.string),            185, SIZE_14, "Watchdog Mode" },
  { sizeof(tekst_inst.Watchdog_Position_14.string),        185, SIZE_14, "Watchdog Position" },

  { sizeof(tekst_inst.Data_Wissen_14.string),              185, SIZE_14, "Data wissen" },
  { sizeof(tekst_inst.Selecteer_Data_14.string),           185, SIZE_14, "Data selecteren" },
  { sizeof(tekst_inst.Druk_OK_Om_Te_Wissen_14.string),     185, SIZE_14, "OK = Wissen" },
  { sizeof(tekst_inst.Data_Gewist_14.string),              185, SIZE_14, "Data gewist!" },
  { sizeof(tekst_inst.Setpoints_10.string),                185, SIZE_10, "Instellingen" },
  { sizeof(tekst_inst.Werkgeheugen_10.string),             185, SIZE_10, "Werkgeheugen" },
  { sizeof(tekst_inst.Restart_10.string),                  185, SIZE_10, "Opnieuw opstarten" },

};

s_tekst_inst const tekst_inst_duits =
{
  sizeof(s_tekst_inst), // unsinged int area_size;
  ORION,                // unsigned int computer;
  MULTI_CONNECT,        // unsigned int soort;
  VERSIE_TEKST_INST,    // unsigned int versie_tekst_inst;

  { sizeof(tekst_inst.Gekozen_Taal_14.string),         150, SIZE_14, "Deutsch" },

  { sizeof(tekst_inst.Opties_10.string),               174, SIZE_10, "Optionen" },
  { sizeof(tekst_inst.Bekijken_Opties_14.string),      185, SIZE_14, "Optionen ANZEIGEN" },
  { sizeof(tekst_inst.Wijzigen_Opties_14.string),      185, SIZE_14, "Optionen ÄNDERN" },
  { sizeof(tekst_inst.Orion_Ingeschakeld_14.string),   185, SIZE_14, "Orion eingeschaltet" },
  { sizeof(tekst_inst.Orion_Uitgeschakeld_14.string),  185, SIZE_14, "Orion ausgeschaltet" },

  { sizeof(tekst_inst.Algemeen_14.string),             185, SIZE_14, "Allgemeines" },
  { sizeof(tekst_inst.IO_14.string),                   185, SIZE_14, "Ein / Ausgänge" },
  { sizeof(tekst_inst.Motorgroepen_14.string),         185, SIZE_14, "Motorgruppen" },
  { sizeof(tekst_inst.Groep_1_14.string),              185, SIZE_14, "Gruppe 1" },
  { sizeof(tekst_inst.Groep_2_14.string),              185, SIZE_14, "Gruppe 2" },
  { sizeof(tekst_inst.Groep_3_14.string),              185, SIZE_14, "Gruppe 3" },
  { sizeof(tekst_inst.Groep_4_14.string),              185, SIZE_14, "Gruppe 4" },
  { sizeof(tekst_inst.Groep_5_14.string),              185, SIZE_14, "Gruppe 5" },
  { sizeof(tekst_inst.Groep_6_14.string),              185, SIZE_14, "Gruppe 6" },
  { sizeof(tekst_inst.Groep_7_14.string),              185, SIZE_14, "Gruppe 7" },
  { sizeof(tekst_inst.Groep_8_14.string),              185, SIZE_14, "Gruppe 8" },
  { sizeof(tekst_inst.Groep_9_14.string),              185, SIZE_14, "Gruppe 9" },
  { sizeof(tekst_inst.Groep_10_14.string),             185, SIZE_14, "Gruppe 10" },
  { sizeof(tekst_inst.Groep_11_14.string),             185, SIZE_14, "Gruppe 11" },
  { sizeof(tekst_inst.Groep_12_14.string),             185, SIZE_14, "Gruppe 12" },
  { sizeof(tekst_inst.Groep_13_14.string),             185, SIZE_14, "Gruppe 13" },
  { sizeof(tekst_inst.Groep_14_14.string),             185, SIZE_14, "Gruppe 14" },
  { sizeof(tekst_inst.Groep_15_14.string),             185, SIZE_14, "Gruppe 15" },
  { sizeof(tekst_inst.Groep_16_14.string),             185, SIZE_14, "Gruppe 16" },
  { sizeof(tekst_inst.Groep_17_14.string),             185, SIZE_14, "Gruppe 17" },
  { sizeof(tekst_inst.Groep_18_14.string),             185, SIZE_14, "Gruppe 18" },
  { sizeof(tekst_inst.Groep_19_14.string),             185, SIZE_14, "Gruppe 19" },
  { sizeof(tekst_inst.Groep_20_14.string),             185, SIZE_14, "Gruppe 20" },
  { sizeof(tekst_inst.Groep_21_14.string),             185, SIZE_14, "Gruppe 21" },
  { sizeof(tekst_inst.Groep_22_14.string),             185, SIZE_14, "Gruppe 22" },
  { sizeof(tekst_inst.Groep_23_14.string),             185, SIZE_14, "Gruppe 23" },
  { sizeof(tekst_inst.Groep_24_14.string),             185, SIZE_14, "Gruppe 24" },
  { sizeof(tekst_inst.Groep_25_14.string),             185, SIZE_14, "Gruppe 25" },
  { sizeof(tekst_inst.Groep_26_14.string),             185, SIZE_14, "Gruppe 26" },
  { sizeof(tekst_inst.Groep_27_14.string),             185, SIZE_14, "Gruppe 27" },
  { sizeof(tekst_inst.Groep_28_14.string),             185, SIZE_14, "Gruppe 28" },
  { sizeof(tekst_inst.Groep_29_14.string),             185, SIZE_14, "Gruppe 29" },
  { sizeof(tekst_inst.Groep_30_14.string),             185, SIZE_14, "Gruppe 30" },
  { sizeof(tekst_inst.Groep_31_14.string),             185, SIZE_14, "Gruppe 31" },
  { sizeof(tekst_inst.Groep_32_14.string),             185, SIZE_14, "Gruppe 32" },
  { sizeof(tekst_inst.Twee_doek_een_bed_14.string),    185, SIZE_14, "2 Schirme an 1 Anlage" },
  { sizeof(tekst_inst.Cabriokas_14.string),            185, SIZE_14, "Cabrio-Gewächshaus" },

  { sizeof(tekst_inst.Opties_Doorlopen_14.string),     220, SIZE_14, "Durchlauf Optionen?" },
  { sizeof(tekst_inst.Opties_OK_14.string),            220, SIZE_14, "Optionen OK?" },

  { sizeof(tekst_inst.Opties_algemeen_10.string),      185, SIZE_10, "Opt: Allgemeines" },
  { sizeof(tekst_inst.Taal_14.string),                 185, SIZE_14, "Sprache" },
  { sizeof(tekst_inst.Opties_Wissen_14.string),        185, SIZE_14, "Optionen löschen" },
  { sizeof(tekst_inst.Opties_gewist_14.string),        210, SIZE_14, "Optionen gelöscht!" },
  { sizeof(tekst_inst.Setpoints_Wissen_14.string),     185, SIZE_14, "Sollwerte Löschen" },
  { sizeof(tekst_inst.Setpoints_gewist_14.string),     210, SIZE_14, "Sollwerte gelöscht!" },
  { sizeof(tekst_inst.Helderheid_14.string),           185, SIZE_14, "Helligkeit" },
  { sizeof(tekst_inst.LCD_dimmen_14.string),           185, SIZE_14, "LCD abblenden" },
  { sizeof(tekst_inst.Computer_14.string),             185, SIZE_14, "Computer" },
  { sizeof(tekst_inst.Nummer_14.string),               135, SIZE_14, "Nummer" },
  { sizeof(tekst_inst.Adres_14.string),                135, SIZE_14, "Adresse" },
  { sizeof(tekst_inst.CAN_BACKBONE_14.string),         140, SIZE_14, "CAN BACKBONE" },        // CAN_BACKBONE_14
  { sizeof(tekst_inst.CAN_RS232_14.string),            140, SIZE_14, "CAN-RS232" },           // CAN_RS232_14
  { sizeof(tekst_inst.Waarschuwing_14.string),         140, SIZE_10, "Warnung" },             // Waarschuwing_14
  { sizeof(tekst_inst.RS232_14.string),                185, SIZE_14, "RS232" },
  { sizeof(tekst_inst.kBd_14.string),                   30, SIZE_14, "kBd" },
  { sizeof(tekst_inst.Installateurs_14.string),        185, SIZE_14, "Installateure" },
  { sizeof(tekst_inst.Gebruikers_14.string),           185, SIZE_14, "Nutzer" },
  { sizeof(tekst_inst.PC_14.string),                   185, SIZE_14, "PC" },
  { sizeof(tekst_inst.Wachtwoord_14.string),           110, SIZE_14, "Code" },
  { sizeof(tekst_inst.Herhaal_14.string),              110, SIZE_14, "Wiederhole" },
  { sizeof(tekst_inst.Wachtwoord_ongelijk_7.string),   100, SIZE_7,  "Codes ungleich!" },

  { sizeof(tekst_inst.Opties_IO_10.string),            185, SIZE_10, "Opt: Ein / Ausgänge" },
  { sizeof(tekst_inst.Versie_7.string),                 40, SIZE_7,  "Version:" },

  { sizeof(tekst_inst.Opties_bord_10.string),           66, SIZE_10, "Optionen:" },
  { sizeof(tekst_inst.Nummer_IO_Module_14.string),     220, SIZE_14, "Nummer IO-Modul" },
  { sizeof(tekst_inst.Verwijderen_14.string),          150, SIZE_14, "Löschen" },
  { sizeof(tekst_inst.Analoge_Ingang_14.string),       150, SIZE_14, "Analoger Eingänge" },
  { sizeof(tekst_inst.Digitale_Ingang_14.string),      150, SIZE_14, "Digitaler Eingänge" },
  { sizeof(tekst_inst.Analoge_Uitgang_14.string),      150, SIZE_14, "Analoger Ausgänge" },
  { sizeof(tekst_inst.Digitale_Uitgang_14.string),     150, SIZE_14, "Digitaler Ausgänge" },
  { sizeof(tekst_inst.Motor_Control_14.string),        150, SIZE_14, "Motorsteuerung" },
  { sizeof(tekst_inst.Instellen_sensor_14.string),     130, SIZE_14, "Sensor einstellen" },
  { sizeof(tekst_inst.IJken_sensor_14.string),         130, SIZE_14, "Sensor eichen" },
  { sizeof(tekst_inst.IJken_Minimum_14.string),        130, SIZE_14, "Eichen Minimum" },  
  { sizeof(tekst_inst.IJken_Maximum_14.string),        130, SIZE_14, "Eichen Maximum" },  
  { sizeof(tekst_inst.Instellen_14.string),            130, SIZE_14, "einstellen" },
  { sizeof(tekst_inst.IJken_14.string),                130, SIZE_14, "eichen" },
  { sizeof(tekst_inst.V_14.string),                     12, SIZE_14, "V" },
  { sizeof(tekst_inst.mA_14.string),                    12, SIZE_14, "mA" },
  { sizeof(tekst_inst.Overnemen_10.string),             77, SIZE_10, "Übernehmen" },
  { sizeof(tekst_inst.Windrichting_14.string),         210, SIZE_14, "Windrichtung" },
  { sizeof(tekst_inst.Windsnelheid_bij_5V_14.string),  210, SIZE_14, "Windgeschwindigkeit bei 5V" },
  { sizeof(tekst_inst.kg_14.string),                    19, SIZE_14, "kg" },
  { sizeof(tekst_inst.g_14.string),                     19, SIZE_14, "g" },
  { sizeof(tekst_inst.Pulsen_Per_Liter_14.string),     210, SIZE_14, "Pulse pro Liter" },
  { sizeof(tekst_inst.Liters_Per_Puls_14.string),      210, SIZE_14, "Liter pro Puls" },
  { sizeof(tekst_inst.Pulsen_Per_Kg_14.string),        210, SIZE_14, "Pulse pro Kg" },
  { sizeof(tekst_inst.Kg_Per_Puls_14.string),          210, SIZE_14, "Kg pro Puls" },
  { sizeof(tekst_inst.Pulsen_Per_Ei_14.string),        210, SIZE_14, "Pulse pro Ei" },
  { sizeof(tekst_inst.Eieren_Per_Puls_14.string),      210, SIZE_14, "Eier pro Puls" },
  { sizeof(tekst_inst.Pulsen_Per_kWh_14.string),       210, SIZE_14, "Pulse pro kWh" },
  { sizeof(tekst_inst.kWh_Per_Puls_14.string),         210, SIZE_14, "kWh pro Puls" },
  { sizeof(tekst_inst.Minimum_14.string),              100, SIZE_14, "Minimum:" },
  { sizeof(tekst_inst.Maximum_14.string),              100, SIZE_14, "Maximum:" },
  { sizeof(tekst_inst.Aantal_Tellers_14.string),       135, SIZE_14, "Anzahl Zähler" },
  { sizeof(tekst_inst.Teller_14.string),               100, SIZE_14, "Zähler" },
  { sizeof(tekst_inst.Fout_Licht_Niveau_14.string),    180, SIZE_14, "Falsche Lichtstufe" },
  { sizeof(tekst_inst.Fout_Communicatie_14.string),    180, SIZE_14, "Kommunikationsfehler" },
  { sizeof(tekst_inst.Fout_Onbekend_14.string),        180, SIZE_14, "Unbekannter Fehler" },
  { sizeof(tekst_inst.Niet_Toegewezen_14.string),      150, SIZE_14, "Nicht zugewiesen" },

  { sizeof(tekst_inst.Opties_motorgroepen_10.string),  185, SIZE_10, "Opt: Motorgruppen" },
  { sizeof(tekst_inst.Aantal_motorgroepen_14.string),  180, SIZE_14, "Motorgruppen" },

  { sizeof(tekst_inst.Opties_groep_1_10.string),       185, SIZE_10, "Opt: Gruppe 1" },
  { sizeof(tekst_inst.Opties_groep_2_10.string),       185, SIZE_10, "Opt: Gruppe 2" },
  { sizeof(tekst_inst.Opties_groep_3_10.string),       185, SIZE_10, "Opt: Gruppe 3" },
  { sizeof(tekst_inst.Opties_groep_4_10.string),       185, SIZE_10, "Opt: Gruppe 4" },
  { sizeof(tekst_inst.Opties_groep_5_10.string),       185, SIZE_10, "Opt: Gruppe 5" },
  { sizeof(tekst_inst.Opties_groep_6_10.string),       185, SIZE_10, "Opt: Gruppe 6" },
  { sizeof(tekst_inst.Opties_groep_7_10.string),       185, SIZE_10, "Opt: Gruppe 7" },
  { sizeof(tekst_inst.Opties_groep_8_10.string),       185, SIZE_10, "Opt: Gruppe 8" },
  { sizeof(tekst_inst.Opties_groep_9_10.string),       185, SIZE_10, "Opt: Gruppe 9" },
  { sizeof(tekst_inst.Opties_groep_10_10.string),      185, SIZE_10, "Opt: Gruppe 10" },
  { sizeof(tekst_inst.Opties_groep_11_10.string),      185, SIZE_10, "Opt: Gruppe 11" },
  { sizeof(tekst_inst.Opties_groep_12_10.string),      185, SIZE_10, "Opt: Gruppe 12" },
  { sizeof(tekst_inst.Opties_groep_13_10.string),      185, SIZE_10, "Opt: Gruppe 13" },
  { sizeof(tekst_inst.Opties_groep_14_10.string),      185, SIZE_10, "Opt: Gruppe 14" },
  { sizeof(tekst_inst.Opties_groep_15_10.string),      185, SIZE_10, "Opt: Gruppe 15" },
  { sizeof(tekst_inst.Opties_groep_16_10.string),      185, SIZE_10, "Opt: Gruppe 16" },
  { sizeof(tekst_inst.Opties_groep_17_10.string),      185, SIZE_10, "Opt: Gruppe 17" },
  { sizeof(tekst_inst.Opties_groep_18_10.string),      185, SIZE_10, "Opt: Gruppe 18" },
  { sizeof(tekst_inst.Opties_groep_19_10.string),      185, SIZE_10, "Opt: Gruppe 19" },
  { sizeof(tekst_inst.Opties_groep_20_10.string),      185, SIZE_10, "Opt: Gruppe 20" },
  { sizeof(tekst_inst.Opties_groep_21_10.string),      185, SIZE_10, "Opt: Gruppe 21" },
  { sizeof(tekst_inst.Opties_groep_22_10.string),      185, SIZE_10, "Opt: Gruppe 22" },
  { sizeof(tekst_inst.Opties_groep_23_10.string),      185, SIZE_10, "Opt: Gruppe 23" },
  { sizeof(tekst_inst.Opties_groep_24_10.string),      185, SIZE_10, "Opt: Gruppe 24" },
  { sizeof(tekst_inst.Opties_groep_25_10.string),      185, SIZE_10, "Opt: Gruppe 25" },
  { sizeof(tekst_inst.Opties_groep_26_10.string),      185, SIZE_10, "Opt: Gruppe 26" },
  { sizeof(tekst_inst.Opties_groep_27_10.string),      185, SIZE_10, "Opt: Gruppe 27" },
  { sizeof(tekst_inst.Opties_groep_28_10.string),      185, SIZE_10, "Opt: Gruppe 28" },
  { sizeof(tekst_inst.Opties_groep_29_10.string),      185, SIZE_10, "Opt: Gruppe 29" },
  { sizeof(tekst_inst.Opties_groep_30_10.string),      185, SIZE_10, "Opt: Gruppe 30" },
  { sizeof(tekst_inst.Opties_groep_31_10.string),      185, SIZE_10, "Opt: Gruppe 31" },
  { sizeof(tekst_inst.Opties_groep_32_10.string),      185, SIZE_10, "Opt: Gruppe 32" },

  { sizeof(tekst_inst.Opties_2_doek_1_bed_10.string),  185, SIZE_10, "Opt: 2 Schirme an 1 Anlage" },
  { sizeof(tekst_inst.Aantal_14.string),               180, SIZE_14, "Anzahl" },
  { sizeof(tekst_inst.Twee_doek_een_bed_1_14.string),  180, SIZE_14, "2 Schirme 1 Anlage [1]" },
  { sizeof(tekst_inst.Twee_doek_een_bed_2_14.string),  180, SIZE_14, "2 Schirme 1 Anlage [2]" },
  { sizeof(tekst_inst.Twee_doek_een_bed_3_14.string),  180, SIZE_14, "2 Schirme 1 Anlage [3]" },
  { sizeof(tekst_inst.Twee_doek_een_bed_4_14.string),  180, SIZE_14, "2 Schirme 1 Anlage [4]" },
  { sizeof(tekst_inst.Twee_doek_een_bed_5_14.string),  180, SIZE_14, "2 Schirme 1 Anlage [5]" },
  { sizeof(tekst_inst.Twee_doek_een_bed_6_14.string),  180, SIZE_14, "2 Schirme 1 Anlage [6]" },
  { sizeof(tekst_inst.Twee_doek_een_bed_7_14.string),  180, SIZE_14, "2 Schirme 1 Anlage [7]" },
  { sizeof(tekst_inst.Twee_doek_een_bed_8_14.string),  180, SIZE_14, "2 Schirme 1 Anlage [8]" },
  { sizeof(tekst_inst.Master_14.string),               180, SIZE_14, "Master" },
  { sizeof(tekst_inst.Standby_regeling_14.string),     180, SIZE_14, "Standby" },
  { sizeof(tekst_inst.Geen_10.string),                 180, SIZE_10, "Kein" },
  { sizeof(tekst_inst.Tegenover_elkaar_10.string),     180, SIZE_10, "Gegenüberliegend" },
  { sizeof(tekst_inst.Achter_elkaar_10.string),        180, SIZE_10, "Nebeneinander" },

  { sizeof(tekst_inst.Opties_Cabriokas_10.string),     185, SIZE_10, "Opt: Cabrio-Gewächshaus" },
  { sizeof(tekst_inst.Cabriokas_1_14.string),          180, SIZE_14, "Cabrio-Gewächshaus 1" },
  { sizeof(tekst_inst.Cabriokas_2_14.string),          180, SIZE_14, "Cabrio-Gewächshaus 2" },
  { sizeof(tekst_inst.Cabriokas_3_14.string),          180, SIZE_14, "Cabrio-Gewächshaus 3" },
  { sizeof(tekst_inst.Cabriokas_4_14.string),          180, SIZE_14, "Cabrio-Gewächshaus 4" },
  { sizeof(tekst_inst.Cabriokas_5_14.string),          180, SIZE_14, "Cabrio-Gewächshaus 5" },
  { sizeof(tekst_inst.Cabriokas_6_14.string),          180, SIZE_14, "Cabrio-Gewächshaus 6" },
  { sizeof(tekst_inst.Cabriokas_7_14.string),          180, SIZE_14, "Cabrio-Gewächshaus 7" },
  { sizeof(tekst_inst.Cabriokas_8_14.string),          180, SIZE_14, "Cabrio-Gewächshaus 8" },
  { sizeof(tekst_inst.Voor_naloop_10.string),          180, SIZE_10, "Vor-/Nachlauf" },
  { sizeof(tekst_inst.Gelijkloop_10.string),           180, SIZE_10, "Gleichlauf" },
  { sizeof(tekst_inst.Voorloop_14.string),             180, SIZE_14, "Vorlauf" },
  { sizeof(tekst_inst.Gelijkloop_14.string),           180, SIZE_14, "Gleichlauf" },
  { sizeof(tekst_inst.Hysteresis_14.string),           180, SIZE_14, "Hysterese" },

  { sizeof(tekst_inst.Opties_Luchtmengkast_10.string), 185, SIZE_10, "Opt: Luftmischer" },
  { sizeof(tekst_inst.Luchtmengkast_14.string),        185, SIZE_14, "Luftmischer" },
  { sizeof(tekst_inst.Aantal_groepen_14.string),       180, SIZE_14, "Anzahl Gruppen" },
  { sizeof(tekst_inst.Aantal_units_14.string),         180, SIZE_14, "Anzahl Einheiten" },
  { sizeof(tekst_inst.Type_klep_14.string),            180, SIZE_14, "Klappentyp" },
  { sizeof(tekst_inst.Binnen_buiten_14.string),        180, SIZE_14, "Innen/Außen" },
  { sizeof(tekst_inst.Recirculatie_14.string),         180, SIZE_14, "Rezirkulation" },
  { sizeof(tekst_inst.Binnenklep_14.string),           180, SIZE_14, "Innenklappe" },
  { sizeof(tekst_inst.Buitenklep_14.string),           180, SIZE_14, "Außenklappe" },
  { sizeof(tekst_inst.Recirculatieklep_14.string),     180, SIZE_14, "Rezirk-klappe" },
  { sizeof(tekst_inst.Bovenklep_14.string),            180, SIZE_14, "Obere Klappe" },
  { sizeof(tekst_inst.Verwarming_14.string),           180, SIZE_14, "Heizung" },
  { sizeof(tekst_inst.Inblaasvent_14.string),          180, SIZE_14, "Einblasv" },
  { sizeof(tekst_inst.Afblaasvent_14.string),          180, SIZE_14, "Ausblasv" },
  { sizeof(tekst_inst.Inblaastemp_14.string),          180, SIZE_14, "Einblastemperatur" },
  { sizeof(tekst_inst.Mengtemp_14.string),             180, SIZE_14, "Mischtemperatur" },
  { sizeof(tekst_inst.Streeftemp_14.string),           180, SIZE_14, "Solltemperatuur" },
  { sizeof(tekst_inst.Vorstbewaking_14.string),        180, SIZE_14, "Frostüberwachung" },
  { sizeof(tekst_inst.Alarm_ingang_14.string),         180, SIZE_14, "Alarmeingang" },
  { sizeof(tekst_inst.Drukverschil_14.string),         180, SIZE_14, "Drukverschil" },
  { sizeof(tekst_inst.Units_14.string),                 80, SIZE_14, "Einheiten" },
  { sizeof(tekst_inst.Groep_14.string),                 80, SIZE_14, "Gruppe" },
  { sizeof(tekst_inst.Naregelen_14.string),            180, SIZE_14, "Nachregeln" },

  { sizeof(tekst_inst.Gekoppeld_aan_klep_14.string),   180, SIZE_14, "Gekoppelt mit Klappe" },

  { sizeof(tekst_inst.Raam_14.string),                 150, SIZE_14, "Fenster" },
  { sizeof(tekst_inst.Doek_14.string),                 150, SIZE_14, "Schirm" },
  { sizeof(tekst_inst.Ventilatie_14.string),           150, SIZE_14, "Ventilation" },
  { sizeof(tekst_inst.Type_sturing_14.string),         180, SIZE_14, "Steuerungstyp" },
  { sizeof(tekst_inst.Open_14.string),                 180, SIZE_14, "Öffnen" },
  { sizeof(tekst_inst.Dicht_14.string),                180, SIZE_14, "Schließen" },
  { sizeof(tekst_inst.Hoger_14.string),                180, SIZE_14, "Erhöhen" },
  { sizeof(tekst_inst.Lager_14.string),                180, SIZE_14, "Herabsetzen" },
  { sizeof(tekst_inst.Position_14.string),             180, SIZE_14, "Position" },
  { sizeof(tekst_inst.Terugmelding_14.string),         180, SIZE_14, "Rückmeldung" },
  { sizeof(tekst_inst.Aantal_motoren_14.string),       180, SIZE_14, "Anzahl Motoren" },
  { sizeof(tekst_inst.Motoren_14.string),              180, SIZE_14, "Motoren" },
  { sizeof(tekst_inst.Master_nummer_14.string),        180, SIZE_14, "Nummer Master" },
  { sizeof(tekst_inst.Frequentie_gestuurd_14.string),  180, SIZE_14, "Frequenzgesteuert" },
  { sizeof(tekst_inst.Digitaal_14.string),             150, SIZE_14, "Digital" },
  { sizeof(tekst_inst.Analoog_14.string),              150, SIZE_14, "Analog" },
  { sizeof(tekst_inst.Hoge_snelheid_14.string),        180, SIZE_14, "Hohe Geschwindigkeit" },
  { sizeof(tekst_inst.Uitgang_snelheid_14.string),     180, SIZE_14, "Ausgang Geschwind" },
  { sizeof(tekst_inst.Snelheid_14.string),             180, SIZE_14, "Geschwindigkeit" },
  { sizeof(tekst_inst.Laag_14.string),                 150, SIZE_14, "Gering" },
  { sizeof(tekst_inst.Hoog_14.string),                 150, SIZE_14, "Hoch" },
  { sizeof(tekst_inst.Terugschakel_positie_14.string), 180, SIZE_14, "Zurückschaltposition" },
  { sizeof(tekst_inst.Pulse_system_14.string),         180, SIZE_14, "Pulssystem" },
  { sizeof(tekst_inst.Kier_regeling_14.string),        180, SIZE_14, "Abstandregelung" },
  { sizeof(tekst_inst.Alarm_analoog_14.string),        180, SIZE_14, "Analoger Alarm" },

  { sizeof(tekst_inst.JA_10.string),                   45, SIZE_10, "JA" },
  { sizeof(tekst_inst.JA_14.string),                   45, SIZE_14, "JA" },
  { sizeof(tekst_inst.NEE_14.string),                  45, SIZE_14, "NEIN" },
  { sizeof(tekst_inst.AAN_14.string),                  45, SIZE_14, "EIN" },
  { sizeof(tekst_inst.UIT_14.string),                  45, SIZE_14, "AUS" },
  { sizeof(tekst_inst.Zeker_weten_7.string),          100, SIZE_7,  "Sind Sie sicher?" },
  { sizeof(tekst_inst.Wissen_10.string),               65, SIZE_10, "Löschen!" },
  { sizeof(tekst_inst.Kopieren_10.string),             65, SIZE_10, "Kopieren" },
  { sizeof(tekst_inst.Pa_7.string),                    13, SIZE_7,  "Pa" },
  { sizeof(tekst_inst.ppm_7.string),                   20, SIZE_7,  "ppm" },
  { sizeof(tekst_inst.perc_7.string),                   8, SIZE_7,  "%" },
  { sizeof(tekst_inst.mps_7.string),                   18, SIZE_7,  "m/s" },
  { sizeof(tekst_inst.mpmin_7.string),                 30, SIZE_7,  "m/min" },
  { sizeof(tekst_inst.graden_celsius_7.string),        10, SIZE_7,  "°C" },
  { sizeof(tekst_inst.graden_fahrenheid_7.string),     10, SIZE_7,  "°F" },
  { sizeof(tekst_inst.graden_7.string),                 8, SIZE_7,  "°" },
  { sizeof(tekst_inst.meter_7.string),                  8, SIZE_7,  "m" },

  { sizeof(tekst_inst.Alarm_Contact_14.string),       180, SIZE_14, "Alarmkontakt" },
  { sizeof(tekst_inst.Alarm_zacht_contact_14.string), 180, SIZE_14, "Kontakt leiser Alarm" },
  { sizeof(tekst_inst.Alarm_urgent_14.string),        180, SIZE_14, "Alarm dringend" },
  { sizeof(tekst_inst.Opties_Kopieren_14.string),     185, SIZE_14, "Optionen kopieren" },

  { sizeof(tekst_inst.COM1_USB_14.string),            185, SIZE_14, "COM1/USB" },            // COM1_USB_14
  { sizeof(tekst_inst.COM2_14.string),                185, SIZE_14, "COM2" },                // COM2_14
  { sizeof(tekst_inst.Modem_14.string),               185, SIZE_14, "Modem" },               // Modem_14
  { sizeof(tekst_inst.Answer_14.string),              150, SIZE_14, "Answer (ATS0=.)" },     // Answer_14
  { sizeof(tekst_inst.ETHERNET_14.string),            185, SIZE_14, "ETHERNET" },            // ETHERNET_14
  { sizeof(tekst_inst.IP_10.string),                   50, SIZE_10, "IP" },                  // IP_10
  { sizeof(tekst_inst.Mask_10.string),                 50, SIZE_10, "Mask" },                // Mask_10
  { sizeof(tekst_inst.Gate_10.string),                 50, SIZE_10, "Gate" },                // Gate_10
  { sizeof(tekst_inst.Port_10.string),                 50, SIZE_10, "Port" },                // Port_10

  { sizeof(tekst_inst.RS485_14.string),               150, SIZE_14, "RS485" },
  { sizeof(tekst_inst.RS485_baudrate_14.string),      150, SIZE_14, "RS485-Baudrate" },
  { sizeof(tekst_inst.RS485_pariteit_14.string),      150, SIZE_14, "RS485-Parität" },
  { sizeof(tekst_inst.Geen_14.string),                110, SIZE_14, "Keine" },
  { sizeof(tekst_inst.Even_14.string),                110, SIZE_14, "Gerade" },
  { sizeof(tekst_inst.Oneven_14.string),              110, SIZE_14, "Ungerade" },

  { sizeof(tekst_inst.Looptijd_14.string),            110, SIZE_14, "Laufzeit" },
  { sizeof(tekst_inst.Aan_uit_14.string),             110, SIZE_14, "Ein/Aus" },

  { sizeof(tekst_inst.Absoluut_14.string),            180, SIZE_14, "Absolut" },                                             
  { sizeof(tekst_inst.Relatief_14.string),            180, SIZE_14, "Relativ" },                                             

  { sizeof(tekst_inst.Type_14.string),                180, SIZE_14, "Type" },
  { sizeof(tekst_inst.Adressen_14.string),            180, SIZE_14, "Adresse" },
  { sizeof(tekst_inst.Nummer_7.string),               180, SIZE_7,  "Nummer" },
  { sizeof(tekst_inst.niet_beschikbaar_7.string),     180, SIZE_7,  "niet beschikbaar" },
  { sizeof(tekst_inst.Nummer_10.string),              180, SIZE_10, "Nummer" },
  { sizeof(tekst_inst.Adres_10.string),               180, SIZE_10, "Adres" },
  { sizeof(tekst_inst.Wijzig_adres_14.string),        180, SIZE_14, "Adresse ändern" },
  { sizeof(tekst_inst.Wijzigen_10.string),             65, SIZE_10, "ändern" },
  { sizeof(tekst_inst.gewijzigd_10.string),           180, SIZE_10, "geändert" },
  { sizeof(tekst_inst.communicatie_fout_10.string),   180, SIZE_10, "Kommunikationsfehler" },

  { sizeof(tekst_inst.Opties_Luchting_10.string),     185, SIZE_10, "Opt: Lüftung" },
  { sizeof(tekst_inst.Luchting_14.string),            185, SIZE_14, "Lüftung" },

  { sizeof(tekst_inst.Alarmen_14.string),             185, SIZE_14, "Alarme" },                                           
  { sizeof(tekst_inst.Opties_Alarmen_10.string),      185, SIZE_10, "Opt: Alarme" },                                  

  { sizeof(tekst_inst.Frequentie_verstel_14.string),  185, SIZE_14, "Diff Hohe Geschw" },
  //#ifdef CAN_BACKBONE_PC_WARNING
  { sizeof(tekst_inst.PC_1_14.string),                 50, SIZE_14, "PC 1" },
  { sizeof(tekst_inst.PC_2_14.string),                 50, SIZE_14, "PC 2" },
  { sizeof(tekst_inst.PC_3_14.string),                 50, SIZE_14, "PC 3" },
  { sizeof(tekst_inst.Waarschuwing_7.string),          50, SIZE_7,  "Warnung" },
  //#endif // CAN_BACKBONE_PC_WARNING
  //#ifdef PASSWORD
  { sizeof(tekst_inst.Beheerder_14.string),           185, SIZE_14, "Supervisor" },
  { sizeof(tekst_inst.Gebruiker_1_14.string),         185, SIZE_14, "Benutzer 1" },
  { sizeof(tekst_inst.Gebruiker_2_14.string),         185, SIZE_14, "Benutzer 2" },
  { sizeof(tekst_inst.Gebruiker_3_14.string),         185, SIZE_14, "Benutzer 3" },
  { sizeof(tekst_inst.Gebruiker_4_14.string),         185, SIZE_14, "Benutzer 4" },
  { sizeof(tekst_inst.Level_14.string),                85, SIZE_14, "Level:" },
  { sizeof(tekst_inst.Opties_7.string),                80, SIZE_7,  "Optionen" },
  { sizeof(tekst_inst.Setpoints_7.string),             80, SIZE_7,  "Sollwerte" },
  { sizeof(tekst_inst.Setpoints_Syst_7.string),        80, SIZE_7,  "Sollwerte Syst" },
  //#endif // PASSWORD

  { sizeof(tekst_inst.BACnet_Device_ID_10.string),     80, SIZE_10, "Device ID" },
  { sizeof(tekst_inst.BACnet_Network_Nr_10.string),    80, SIZE_10, "Network Nr" },

  { sizeof(tekst_inst.Operation_Mode_14.string),           185, SIZE_14, "Operation Mode" },
  { sizeof(tekst_inst.Closed_Loop_14.string),              180, SIZE_14, "Closed Loop" },                                            
  { sizeof(tekst_inst.Open_Loop_14.string),                180, SIZE_14, "Open Loop" },

  { sizeof(tekst_inst.Vrijgave_ventilatoren_14.string),        180, SIZE_14, "Freigabe Lüfter" },
  { sizeof(tekst_inst.Opties_Vrijgave_ventilatoren_10.string), 185, SIZE_10, "Opt: Freigabe Lüfter" },
  { sizeof(tekst_inst.Vrijgave_14.string),                     180, SIZE_14, "Freigabe" },
  { sizeof(tekst_inst.Ventilatoren_14.string),                 180, SIZE_14, "Lüfter" },

  { sizeof(tekst_inst.PowerFactor_14.string),              185, SIZE_14, "Power factor" },
  { sizeof(tekst_inst.RampUpDown_14.string),               185, SIZE_14, "Ramp-Up/Down" },
  { sizeof(tekst_inst.RampUp_14.string),                   185, SIZE_14, "Ramp-Up" },
  { sizeof(tekst_inst.RampDown_14.string),                 185, SIZE_14, "Ramp-Down" },

  { sizeof(tekst_inst.Klep_14.string),                     150, SIZE_14, "Klappe" },
  { sizeof(tekst_inst.SensorType_14.string),               185, SIZE_14, "Sensor type" },
  { sizeof(tekst_inst.Eindschakelaar_14.string),           110, SIZE_14, "Eindschakelaar" },

  { sizeof(tekst_inst.Sensoren_14.string),                 185, SIZE_14, "Sensoren" },

  { sizeof(tekst_inst.Alarm_eindschakelaar_14.string),     180, SIZE_14, "Alarm eindschakelaar" },

  { sizeof(tekst_inst.Extern_alarm_14.string),             110, SIZE_14, "Extern alarm" },
  { sizeof(tekst_inst.Vent_At_Max_14.string),              185, SIZE_14, "Vent bei 100%" },

  { sizeof(tekst_inst.Watchdog_Mode_14.string),            185, SIZE_14, "Watchdog Mode" },
  { sizeof(tekst_inst.Watchdog_Position_14.string),        185, SIZE_14, "Watchdog Position" },

  { sizeof(tekst_inst.Data_Wissen_14.string),              185, SIZE_14, "Daten löschen" },
  { sizeof(tekst_inst.Selecteer_Data_14.string),           185, SIZE_14, "Daten auswählen" },
  { sizeof(tekst_inst.Druk_OK_Om_Te_Wissen_14.string),     185, SIZE_14, "OK = Löschen" },
  { sizeof(tekst_inst.Data_Gewist_14.string),              185, SIZE_14, "Daten gelöscht!" },
  { sizeof(tekst_inst.Setpoints_10.string),                185, SIZE_10, "Einstellungen" },
  { sizeof(tekst_inst.Werkgeheugen_10.string),             185, SIZE_10, "Arbeitsspeicher" },
  { sizeof(tekst_inst.Restart_10.string),                  185, SIZE_10, "Neustart" },

};

s_tekst_inst const tekst_inst_spaans =
{
  sizeof(s_tekst_inst), // unsinged int area_size;
  ORION,                // unsigned int computer;
  MULTI_CONNECT,        // unsigned int soort;
  VERSIE_TEKST_INST,    // unsigned int versie_tekst_inst;

  { sizeof(tekst_inst.Gekozen_Taal_14.string),         150, SIZE_14, "Español" },

  { sizeof(tekst_inst.Opties_10.string),               174, SIZE_10, "Opciones" },
  { sizeof(tekst_inst.Bekijken_Opties_14.string),      185, SIZE_14, "Ver Opciones" },
  { sizeof(tekst_inst.Wijzigen_Opties_14.string),      185, SIZE_14, "Modificar Opciones" },
  { sizeof(tekst_inst.Orion_Ingeschakeld_14.string),   185, SIZE_14, "Orion conectado" },
  { sizeof(tekst_inst.Orion_Uitgeschakeld_14.string),  185, SIZE_14, "Orion desconectado" },

  { sizeof(tekst_inst.Algemeen_14.string),             185, SIZE_14, "Generales" },
  { sizeof(tekst_inst.IO_14.string),                   185, SIZE_14, "Entradas/Salidas" },
  { sizeof(tekst_inst.Motorgroepen_14.string),         185, SIZE_14, "Motorgroepen" },
  { sizeof(tekst_inst.Groep_1_14.string),              185, SIZE_14, "Groep 1" },
  { sizeof(tekst_inst.Groep_2_14.string),              185, SIZE_14, "Groep 2" },
  { sizeof(tekst_inst.Groep_3_14.string),              185, SIZE_14, "Groep 3" },
  { sizeof(tekst_inst.Groep_4_14.string),              185, SIZE_14, "Groep 4" },
  { sizeof(tekst_inst.Groep_5_14.string),              185, SIZE_14, "Groep 5" },
  { sizeof(tekst_inst.Groep_6_14.string),              185, SIZE_14, "Groep 6" },
  { sizeof(tekst_inst.Groep_7_14.string),              185, SIZE_14, "Groep 7" },
  { sizeof(tekst_inst.Groep_8_14.string),              185, SIZE_14, "Groep 8" },
  { sizeof(tekst_inst.Groep_9_14.string),              185, SIZE_14, "Groep 9" },
  { sizeof(tekst_inst.Groep_10_14.string),             185, SIZE_14, "Groep 10" },
  { sizeof(tekst_inst.Groep_11_14.string),             185, SIZE_14, "Groep 11" },
  { sizeof(tekst_inst.Groep_12_14.string),             185, SIZE_14, "Groep 12" },
  { sizeof(tekst_inst.Groep_13_14.string),             185, SIZE_14, "Groep 13" },
  { sizeof(tekst_inst.Groep_14_14.string),             185, SIZE_14, "Groep 14" },
  { sizeof(tekst_inst.Groep_15_14.string),             185, SIZE_14, "Groep 15" },
  { sizeof(tekst_inst.Groep_16_14.string),             185, SIZE_14, "Groep 16" },
  { sizeof(tekst_inst.Groep_17_14.string),             185, SIZE_14, "Groep 17" },
  { sizeof(tekst_inst.Groep_18_14.string),             185, SIZE_14, "Groep 18" },
  { sizeof(tekst_inst.Groep_19_14.string),             185, SIZE_14, "Groep 19" },
  { sizeof(tekst_inst.Groep_20_14.string),             185, SIZE_14, "Groep 20" },
  { sizeof(tekst_inst.Groep_21_14.string),             185, SIZE_14, "Groep 21" },
  { sizeof(tekst_inst.Groep_22_14.string),             185, SIZE_14, "Groep 22" },
  { sizeof(tekst_inst.Groep_23_14.string),             185, SIZE_14, "Groep 23" },
  { sizeof(tekst_inst.Groep_24_14.string),             185, SIZE_14, "Groep 24" },
  { sizeof(tekst_inst.Groep_25_14.string),             185, SIZE_14, "Groep 25" },
  { sizeof(tekst_inst.Groep_26_14.string),             185, SIZE_14, "Groep 26" },
  { sizeof(tekst_inst.Groep_27_14.string),             185, SIZE_14, "Groep 27" },
  { sizeof(tekst_inst.Groep_28_14.string),             185, SIZE_14, "Groep 28" },
  { sizeof(tekst_inst.Groep_29_14.string),             185, SIZE_14, "Groep 29" },
  { sizeof(tekst_inst.Groep_30_14.string),             185, SIZE_14, "Groep 30" },
  { sizeof(tekst_inst.Groep_31_14.string),             185, SIZE_14, "Groep 31" },
  { sizeof(tekst_inst.Groep_32_14.string),             185, SIZE_14, "Groep 32" },
  { sizeof(tekst_inst.Twee_doek_een_bed_14.string),    185, SIZE_14, "2 doeken op 1 bed" },
  { sizeof(tekst_inst.Cabriokas_14.string),            185, SIZE_14, "Cabriokas" },

  { sizeof(tekst_inst.Opties_Doorlopen_14.string),     220, SIZE_14, "Ver opciones?" },
  { sizeof(tekst_inst.Opties_OK_14.string),            220, SIZE_14, "Opciones OK?" },

  { sizeof(tekst_inst.Opties_algemeen_10.string),      185, SIZE_10, "Opciones: Generales" },
  { sizeof(tekst_inst.Taal_14.string),                 185, SIZE_14, "Lengua" },
  { sizeof(tekst_inst.Opties_Wissen_14.string),        185, SIZE_14, "Borrar opciones" },
  { sizeof(tekst_inst.Opties_gewist_14.string),        210, SIZE_14, "Opciones borradas!" },
  { sizeof(tekst_inst.Setpoints_Wissen_14.string),     185, SIZE_14, "Borrar ajustes" },
  { sizeof(tekst_inst.Setpoints_gewist_14.string),     210, SIZE_14, "Adjustes borrados!" },
  { sizeof(tekst_inst.Helderheid_14.string),           185, SIZE_14, "Brillo" },
  { sizeof(tekst_inst.LCD_dimmen_14.string),           185, SIZE_14, "Dim backlighting" },
  { sizeof(tekst_inst.Computer_14.string),             185, SIZE_14, "Ordenador" },
  { sizeof(tekst_inst.Nummer_14.string),               135, SIZE_14, "Número" },
  { sizeof(tekst_inst.Adres_14.string),                135, SIZE_14, "Adres" },
  { sizeof(tekst_inst.CAN_BACKBONE_14.string),         140, SIZE_14, "CAN BACKBONE" },        // CAN_BACKBONE_14
  { sizeof(tekst_inst.CAN_RS232_14.string),            140, SIZE_14, "CAN-RS232" },           // CAN_RS232_14
  { sizeof(tekst_inst.Waarschuwing_14.string),         140, SIZE_10, "Advertencia" },         // Waarschuwing_14
  { sizeof(tekst_inst.RS232_14.string),                185, SIZE_14, "RS232" },
  { sizeof(tekst_inst.kBd_14.string),                   30, SIZE_14, "kBd" },
  { sizeof(tekst_inst.Installateurs_14.string),        185, SIZE_14, "Instalador" },
  { sizeof(tekst_inst.Gebruikers_14.string),           185, SIZE_14, "Usuario" },
  { sizeof(tekst_inst.PC_14.string),                   185, SIZE_14, "PC" },
  { sizeof(tekst_inst.Wachtwoord_14.string),           110, SIZE_14, "contraseña" },
  { sizeof(tekst_inst.Herhaal_14.string),              110, SIZE_14, "repetición" },
  { sizeof(tekst_inst.Wachtwoord_ongelijk_7.string),   100, SIZE_7,  "Contraseña diferent" },

  { sizeof(tekst_inst.Opties_IO_10.string),            185, SIZE_10, "Opciones: Entradas/Salidas" },
  { sizeof(tekst_inst.Versie_7.string),                 40, SIZE_7,  "Versión:" },

  { sizeof(tekst_inst.Opties_bord_10.string),           66, SIZE_10, "Opciones:" },
  { sizeof(tekst_inst.Nummer_IO_Module_14.string),     220, SIZE_14, "Número IO Módulo" },
  { sizeof(tekst_inst.Verwijderen_14.string),          150, SIZE_14, "eliminar" },
  { sizeof(tekst_inst.Analoge_Ingang_14.string),       150, SIZE_14, "Entrada analógica" },
  { sizeof(tekst_inst.Digitale_Ingang_14.string),      150, SIZE_14, "Entrada digital" },
  { sizeof(tekst_inst.Analoge_Uitgang_14.string),      150, SIZE_14, "Salida analógica" },
  { sizeof(tekst_inst.Digitale_Uitgang_14.string),     150, SIZE_14, "Salida digital" },
  { sizeof(tekst_inst.Motor_Control_14.string),        150, SIZE_14, "Motor control" },
  { sizeof(tekst_inst.Instellen_sensor_14.string),     130, SIZE_14, "Ajuste sensor" },
  { sizeof(tekst_inst.IJken_sensor_14.string),         130, SIZE_14, "Calibre sensor" },
  { sizeof(tekst_inst.IJken_Minimum_14.string),        130, SIZE_14, "Calibre minima" },                                     
  { sizeof(tekst_inst.IJken_Maximum_14.string),        130, SIZE_14, "Calibre maxima" },                                     
  { sizeof(tekst_inst.Instellen_14.string),            130, SIZE_14, "Ajuste" },
  { sizeof(tekst_inst.IJken_14.string),                130, SIZE_14, "Calibre" },
  { sizeof(tekst_inst.V_14.string),                     12, SIZE_14, "V" },
  { sizeof(tekst_inst.mA_14.string),                    12, SIZE_14, "mA" },
  { sizeof(tekst_inst.Overnemen_10.string),             77, SIZE_14, "Asumir" },
  { sizeof(tekst_inst.Windrichting_14.string),         210, SIZE_14, "Dirección de viento" },
  { sizeof(tekst_inst.Windsnelheid_bij_5V_14.string),  210, SIZE_14, "Velocidad de viento en 5V" },
  { sizeof(tekst_inst.kg_14.string),                    19, SIZE_14, "kg" },
  { sizeof(tekst_inst.g_14.string),                     19, SIZE_14, "g" },
  { sizeof(tekst_inst.Pulsen_Per_Liter_14.string),     210, SIZE_14, "Pulsos por litro" },
  { sizeof(tekst_inst.Liters_Per_Puls_14.string),      210, SIZE_14, "Litros por pulso" },
  { sizeof(tekst_inst.Pulsen_Per_Kg_14.string),        210, SIZE_14, "Pulsos por kg" },
  { sizeof(tekst_inst.Kg_Per_Puls_14.string),          210, SIZE_14, "Kg por pulso" },
  { sizeof(tekst_inst.Pulsen_Per_Ei_14.string),        210, SIZE_14, "Pulsos por huevo" },
  { sizeof(tekst_inst.Eieren_Per_Puls_14.string),      210, SIZE_14, "Huevos por pulso" },
  { sizeof(tekst_inst.Pulsen_Per_kWh_14.string),       210, SIZE_14, "Pulsos por kWh" },
  { sizeof(tekst_inst.kWh_Per_Puls_14.string),         210, SIZE_14, "kWh por pulso" },
  { sizeof(tekst_inst.Minimum_14.string),              100, SIZE_14, "Minima:" },
  { sizeof(tekst_inst.Maximum_14.string),              100, SIZE_14, "Maxima:" },
  { sizeof(tekst_inst.Aantal_Tellers_14.string),       135, SIZE_14, "Contadores" },
  { sizeof(tekst_inst.Teller_14.string),               100, SIZE_14, "Contador" },
  { sizeof(tekst_inst.Fout_Licht_Niveau_14.string),    180, SIZE_14, "Error nivel ilumin" },
  { sizeof(tekst_inst.Fout_Communicatie_14.string),    180, SIZE_14, "Error comunicación" },
  { sizeof(tekst_inst.Fout_Onbekend_14.string),        180, SIZE_14, "Error desconocido" },
  { sizeof(tekst_inst.Niet_Toegewezen_14.string),      150, SIZE_14, "No asignado" },

  { sizeof(tekst_inst.Opties_motorgroepen_10.string),  185, SIZE_10, "Opciones: Motorgroepen" },
  { sizeof(tekst_inst.Aantal_motorgroepen_14.string),  180, SIZE_14, "Motorgroepen" },

  { sizeof(tekst_inst.Opties_groep_1_10.string),       185, SIZE_10, "Opciones: Groep 1" },
  { sizeof(tekst_inst.Opties_groep_2_10.string),       185, SIZE_10, "Opciones: Groep 2" },
  { sizeof(tekst_inst.Opties_groep_3_10.string),       185, SIZE_10, "Opciones: Groep 3" },
  { sizeof(tekst_inst.Opties_groep_4_10.string),       185, SIZE_10, "Opciones: Groep 4" },
  { sizeof(tekst_inst.Opties_groep_5_10.string),       185, SIZE_10, "Opciones: Groep 5" },
  { sizeof(tekst_inst.Opties_groep_6_10.string),       185, SIZE_10, "Opciones: Groep 6" },
  { sizeof(tekst_inst.Opties_groep_7_10.string),       185, SIZE_10, "Opciones: Groep 7" },
  { sizeof(tekst_inst.Opties_groep_8_10.string),       185, SIZE_10, "Opciones: Groep 8" },
  { sizeof(tekst_inst.Opties_groep_9_10.string),       185, SIZE_10, "Opciones: Groep 9" },
  { sizeof(tekst_inst.Opties_groep_10_10.string),      185, SIZE_10, "Opciones: Groep 10" },
  { sizeof(tekst_inst.Opties_groep_11_10.string),      185, SIZE_10, "Opciones: Groep 11" },
  { sizeof(tekst_inst.Opties_groep_12_10.string),      185, SIZE_10, "Opciones: Groep 12" },
  { sizeof(tekst_inst.Opties_groep_13_10.string),      185, SIZE_10, "Opciones: Groep 13" },
  { sizeof(tekst_inst.Opties_groep_14_10.string),      185, SIZE_10, "Opciones: Groep 14" },
  { sizeof(tekst_inst.Opties_groep_15_10.string),      185, SIZE_10, "Opciones: Groep 15" },
  { sizeof(tekst_inst.Opties_groep_16_10.string),      185, SIZE_10, "Opciones: Groep 16" },
  { sizeof(tekst_inst.Opties_groep_17_10.string),      185, SIZE_10, "Opciones: Groep 17" },
  { sizeof(tekst_inst.Opties_groep_18_10.string),      185, SIZE_10, "Opciones: Groep 18" },
  { sizeof(tekst_inst.Opties_groep_19_10.string),      185, SIZE_10, "Opciones: Groep 19" },
  { sizeof(tekst_inst.Opties_groep_20_10.string),      185, SIZE_10, "Opciones: Groep 20" },
  { sizeof(tekst_inst.Opties_groep_21_10.string),      185, SIZE_10, "Opciones: Groep 21" },
  { sizeof(tekst_inst.Opties_groep_22_10.string),      185, SIZE_10, "Opciones: Groep 22" },
  { sizeof(tekst_inst.Opties_groep_23_10.string),      185, SIZE_10, "Opciones: Groep 23" },
  { sizeof(tekst_inst.Opties_groep_24_10.string),      185, SIZE_10, "Opciones: Groep 24" },
  { sizeof(tekst_inst.Opties_groep_25_10.string),      185, SIZE_10, "Opciones: Groep 25" },
  { sizeof(tekst_inst.Opties_groep_26_10.string),      185, SIZE_10, "Opciones: Groep 26" },
  { sizeof(tekst_inst.Opties_groep_27_10.string),      185, SIZE_10, "Opciones: Groep 27" },
  { sizeof(tekst_inst.Opties_groep_28_10.string),      185, SIZE_10, "Opciones: Groep 28" },
  { sizeof(tekst_inst.Opties_groep_29_10.string),      185, SIZE_10, "Opciones: Groep 29" },
  { sizeof(tekst_inst.Opties_groep_30_10.string),      185, SIZE_10, "Opciones: Groep 30" },
  { sizeof(tekst_inst.Opties_groep_31_10.string),      185, SIZE_10, "Opciones: Groep 31" },
  { sizeof(tekst_inst.Opties_groep_32_10.string),      185, SIZE_10, "Opciones: Groep 32" },

  { sizeof(tekst_inst.Opties_2_doek_1_bed_10.string),  185, SIZE_10, "Optiones: 2 doeken op 1 bed" },
  { sizeof(tekst_inst.Aantal_14.string),               180, SIZE_14, "Aantal" },
  { sizeof(tekst_inst.Twee_doek_een_bed_1_14.string),  180, SIZE_14, "2 doeken op 1 bed [1]" },
  { sizeof(tekst_inst.Twee_doek_een_bed_2_14.string),  180, SIZE_14, "2 doeken op 1 bed [2]" },
  { sizeof(tekst_inst.Twee_doek_een_bed_3_14.string),  180, SIZE_14, "2 doeken op 1 bed [3]" },
  { sizeof(tekst_inst.Twee_doek_een_bed_4_14.string),  180, SIZE_14, "2 doeken op 1 bed [4]" },
  { sizeof(tekst_inst.Twee_doek_een_bed_5_14.string),  180, SIZE_14, "2 doeken op 1 bed [5]" },
  { sizeof(tekst_inst.Twee_doek_een_bed_6_14.string),  180, SIZE_14, "2 doeken op 1 bed [6]" },
  { sizeof(tekst_inst.Twee_doek_een_bed_7_14.string),  180, SIZE_14, "2 doeken op 1 bed [7]" },
  { sizeof(tekst_inst.Twee_doek_een_bed_8_14.string),  180, SIZE_14, "2 doeken op 1 bed [8]" },
  { sizeof(tekst_inst.Master_14.string),               180, SIZE_14, "Master" },
  { sizeof(tekst_inst.Standby_regeling_14.string),     180, SIZE_14, "Standby control" },
  { sizeof(tekst_inst.Geen_10.string),                 180, SIZE_10, "Geen" },
  { sizeof(tekst_inst.Tegenover_elkaar_10.string),     180, SIZE_10, "Tegenover elkaar" },
  { sizeof(tekst_inst.Achter_elkaar_10.string),        180, SIZE_10, "Achter elkaar" },

  { sizeof(tekst_inst.Opties_Cabriokas_10.string),     185, SIZE_10, "Options: Cabriokas" },
  { sizeof(tekst_inst.Cabriokas_1_14.string),          180, SIZE_14, "Cabriokas [1]" },
  { sizeof(tekst_inst.Cabriokas_2_14.string),          180, SIZE_14, "Cabriokas [2]" },
  { sizeof(tekst_inst.Cabriokas_3_14.string),          180, SIZE_14, "Cabriokas [3]" },
  { sizeof(tekst_inst.Cabriokas_4_14.string),          180, SIZE_14, "Cabriokas [4]" },
  { sizeof(tekst_inst.Cabriokas_5_14.string),          180, SIZE_14, "Cabriokas [5]" },
  { sizeof(tekst_inst.Cabriokas_6_14.string),          180, SIZE_14, "Cabriokas [6]" },
  { sizeof(tekst_inst.Cabriokas_7_14.string),          180, SIZE_14, "Cabriokas [7]" },
  { sizeof(tekst_inst.Cabriokas_8_14.string),          180, SIZE_14, "Cabriokas [8]" },
  { sizeof(tekst_inst.Voor_naloop_10.string),          180, SIZE_10, "Voor-/Naloop" },
  { sizeof(tekst_inst.Gelijkloop_10.string),           180, SIZE_10, "Gelijkloop" },
  { sizeof(tekst_inst.Voorloop_14.string),             180, SIZE_14, "Voorloop" },
  { sizeof(tekst_inst.Gelijkloop_14.string),           180, SIZE_14, "Gelijkloop" },
  { sizeof(tekst_inst.Hysteresis_14.string),           180, SIZE_14, "Hysterese" },

  { sizeof(tekst_inst.Opties_Luchtmengkast_10.string), 185, SIZE_10, "Options: Luchtmengkast" }, // TD - nog vertalen
  { sizeof(tekst_inst.Luchtmengkast_14.string),        185, SIZE_14, "Luchtmengkast" },
  { sizeof(tekst_inst.Aantal_groepen_14.string),       180, SIZE_14, "Aantal groepen" },
  { sizeof(tekst_inst.Aantal_units_14.string),         180, SIZE_14, "Aantal units" },
  { sizeof(tekst_inst.Type_klep_14.string),            180, SIZE_14, "Type klep" },
  { sizeof(tekst_inst.Binnen_buiten_14.string),        180, SIZE_14, "Binnen/buiten" },
  { sizeof(tekst_inst.Recirculatie_14.string),         180, SIZE_14, "Recirculatie" },
  { sizeof(tekst_inst.Binnenklep_14.string),           180, SIZE_14, "Binnenklep" },
  { sizeof(tekst_inst.Buitenklep_14.string),           180, SIZE_14, "Buitenklep" },
  { sizeof(tekst_inst.Recirculatieklep_14.string),     180, SIZE_14, "Recirculatieklep" },
  { sizeof(tekst_inst.Bovenklep_14.string),            180, SIZE_14, "Bovenklep" },
  { sizeof(tekst_inst.Verwarming_14.string),           180, SIZE_14, "Verwarming" },
  { sizeof(tekst_inst.Inblaasvent_14.string),          180, SIZE_14, "Inblaasvent" },
  { sizeof(tekst_inst.Afblaasvent_14.string),          180, SIZE_14, "Afblaasvent" },
  { sizeof(tekst_inst.Inblaastemp_14.string),          180, SIZE_14, "Inblaastemperatuur" },
  { sizeof(tekst_inst.Mengtemp_14.string),             180, SIZE_14, "Mengtemperatuur" },
  { sizeof(tekst_inst.Streeftemp_14.string),           180, SIZE_14, "Streeftemperatuur" },
  { sizeof(tekst_inst.Vorstbewaking_14.string),        180, SIZE_14, "Vorstbewaking" },
  { sizeof(tekst_inst.Alarm_ingang_14.string),         180, SIZE_14, "Alarm ingang" },
  { sizeof(tekst_inst.Drukverschil_14.string),         180, SIZE_14, "Drukverschil" },
  { sizeof(tekst_inst.Units_14.string),                 80, SIZE_14, "Units" },
  { sizeof(tekst_inst.Groep_14.string),                 80, SIZE_14, "Groep" },
  { sizeof(tekst_inst.Naregelen_14.string),            180, SIZE_14, "Naregelen" },

  { sizeof(tekst_inst.Gekoppeld_aan_klep_14.string),   180, SIZE_14, "Gekoppeld aan klep" },

  { sizeof(tekst_inst.Raam_14.string),                 150, SIZE_14, "Raam" },
  { sizeof(tekst_inst.Doek_14.string),                 150, SIZE_14, "Doek" },
  { sizeof(tekst_inst.Ventilatie_14.string),           150, SIZE_14, "Ventilatie" },
  { sizeof(tekst_inst.Type_sturing_14.string),         180, SIZE_14, "Type sturing" },
  { sizeof(tekst_inst.Open_14.string),                 180, SIZE_14, "Open" },
  { sizeof(tekst_inst.Dicht_14.string),                180, SIZE_14, "Close" },
  { sizeof(tekst_inst.Hoger_14.string),                180, SIZE_14, "Hoger" },
  { sizeof(tekst_inst.Lager_14.string),                180, SIZE_14, "Lager" },
  { sizeof(tekst_inst.Position_14.string),             180, SIZE_14, "Position" },
  { sizeof(tekst_inst.Terugmelding_14.string),         180, SIZE_14, "Feedback" },
  { sizeof(tekst_inst.Aantal_motoren_14.string),       180, SIZE_14, "Aantal motoren" },
  { sizeof(tekst_inst.Motoren_14.string),              180, SIZE_14, "Motoren" },
  { sizeof(tekst_inst.Master_nummer_14.string),        180, SIZE_14, "Master number" },
  { sizeof(tekst_inst.Frequentie_gestuurd_14.string),  180, SIZE_14, "Frequentie gestuurd" },
  { sizeof(tekst_inst.Digitaal_14.string),             150, SIZE_14, "Digitaal" },
  { sizeof(tekst_inst.Analoog_14.string),              150, SIZE_14, "Analoog" },
  { sizeof(tekst_inst.Hoge_snelheid_14.string),        180, SIZE_14, "Hoge snelheid" },
  { sizeof(tekst_inst.Uitgang_snelheid_14.string),     180, SIZE_14, "Uitgang snelheid" },
  { sizeof(tekst_inst.Snelheid_14.string),             180, SIZE_14, "Snelheid" },
  { sizeof(tekst_inst.Laag_14.string),                 150, SIZE_14, "Laag" },
  { sizeof(tekst_inst.Hoog_14.string),                 150, SIZE_14, "Hoog" },
  { sizeof(tekst_inst.Terugschakel_positie_14.string), 180, SIZE_14, "Terugschakel positie" },
  { sizeof(tekst_inst.Pulse_system_14.string),         180, SIZE_14, "Pulse system" },
  { sizeof(tekst_inst.Kier_regeling_14.string),        180, SIZE_14, "Kier regeling" },
  { sizeof(tekst_inst.Alarm_analoog_14.string),        180, SIZE_14, "Alarm analoog" },

  { sizeof(tekst_inst.JA_10.string),                   45, SIZE_10, "SI" },
  { sizeof(tekst_inst.JA_14.string),                   45, SIZE_14, "SI" },
  { sizeof(tekst_inst.NEE_14.string),                  45, SIZE_14, "NO" },
  { sizeof(tekst_inst.AAN_14.string),                  45, SIZE_14, "ON" },
  { sizeof(tekst_inst.UIT_14.string),                  45, SIZE_14, "OFF" },
  { sizeof(tekst_inst.Zeker_weten_7.string),          100, SIZE_7,  "¿Está usted seguro?" },
  { sizeof(tekst_inst.Wissen_10.string),               65, SIZE_10, "Borrar" },
  { sizeof(tekst_inst.Kopieren_10.string),             65, SIZE_10, "Copy" },
  { sizeof(tekst_inst.Pa_7.string),                    13, SIZE_7,  "Pa" },
  { sizeof(tekst_inst.ppm_7.string),                   20, SIZE_7,  "ppm" },
  { sizeof(tekst_inst.perc_7.string),                   8, SIZE_7,  "%" },
  { sizeof(tekst_inst.mps_7.string),                   18, SIZE_7,  "m/s" },
  { sizeof(tekst_inst.mpmin_7.string),                 30, SIZE_7,  "m/min" },
  { sizeof(tekst_inst.graden_celsius_7.string),        10, SIZE_7,  "°C" },
  { sizeof(tekst_inst.graden_fahrenheid_7.string),     10, SIZE_7,  "°F" },
  { sizeof(tekst_inst.graden_7.string),                 8, SIZE_7,  "°" },
  { sizeof(tekst_inst.meter_7.string),                  8, SIZE_7,  "m" },

  { sizeof(tekst_inst.Alarm_Contact_14.string),       180, SIZE_14, "Alarm contact" },
  { sizeof(tekst_inst.Alarm_zacht_contact_14.string), 180, SIZE_14, "Alarm zacht contact" },
  { sizeof(tekst_inst.Alarm_urgent_14.string),        180, SIZE_14, "Alarm urgent" },
  { sizeof(tekst_inst.Opties_Kopieren_14.string),     185, SIZE_14, "Copy Options" },

  { sizeof(tekst_inst.COM1_USB_14.string),            185, SIZE_14, "COM1/USB" },            // COM1_USB_14
  { sizeof(tekst_inst.COM2_14.string),                185, SIZE_14, "COM2" },                // COM2_14
  { sizeof(tekst_inst.Modem_14.string),               185, SIZE_14, "Modem" },               // Modem_14
  { sizeof(tekst_inst.Answer_14.string),              150, SIZE_14, "Answer (ATS0=.)" },     // Answer_14
  { sizeof(tekst_inst.ETHERNET_14.string),            185, SIZE_14, "ETHERNET" },            // ETHERNET_14
  { sizeof(tekst_inst.IP_10.string),                   50, SIZE_10, "IP" },                  // IP_10
  { sizeof(tekst_inst.Mask_10.string),                 50, SIZE_10, "Mask" },                // Mask_10
  { sizeof(tekst_inst.Gate_10.string),                 50, SIZE_10, "Gate" },                // Gate_10
  { sizeof(tekst_inst.Port_10.string),                 50, SIZE_10, "Port" },                // Port_10

  { sizeof(tekst_inst.RS485_14.string),               150, SIZE_14, "RS485" },
  { sizeof(tekst_inst.RS485_baudrate_14.string),      150, SIZE_14, "RS485 baudrate" },
  { sizeof(tekst_inst.RS485_pariteit_14.string),      150, SIZE_14, "RS485 pariteit" },
  { sizeof(tekst_inst.Geen_14.string),                110, SIZE_14, "Geen" },
  { sizeof(tekst_inst.Even_14.string),                110, SIZE_14, "Even" },
  { sizeof(tekst_inst.Oneven_14.string),              110, SIZE_14, "Oneven" },

  { sizeof(tekst_inst.Looptijd_14.string),            110, SIZE_14, "Looptijd" },
  { sizeof(tekst_inst.Aan_uit_14.string),             110, SIZE_14, "Aan/uit" },

  { sizeof(tekst_inst.Absoluut_14.string),            180, SIZE_14, "Absoluto" },                                            
  { sizeof(tekst_inst.Relatief_14.string),            180, SIZE_14, "Relativo" },                                            

  { sizeof(tekst_inst.Type_14.string),                180, SIZE_14, "Type" },
  { sizeof(tekst_inst.Adressen_14.string),            180, SIZE_14, "Addresses" },
  { sizeof(tekst_inst.Nummer_7.string),               180, SIZE_7,  "Nummer" },
  { sizeof(tekst_inst.niet_beschikbaar_7.string),     180, SIZE_7,  "niet beschikbaar" },
  { sizeof(tekst_inst.Nummer_10.string),              180, SIZE_10, "Nummer" },
  { sizeof(tekst_inst.Adres_10.string),               180, SIZE_10, "Adres" },
  { sizeof(tekst_inst.Wijzig_adres_14.string),        180, SIZE_14, "Wijzig adres" },
  { sizeof(tekst_inst.Wijzigen_10.string),             65, SIZE_10, "change" },
  { sizeof(tekst_inst.gewijzigd_10.string),           180, SIZE_10, "gewijzigd" },
  { sizeof(tekst_inst.communicatie_fout_10.string),   180, SIZE_10, "communicatie fout" },

  { sizeof(tekst_inst.Opties_Luchting_10.string),     185, SIZE_10, "Opties: Luchting" },
  { sizeof(tekst_inst.Luchting_14.string),            185, SIZE_14, "Luchting" },

  { sizeof(tekst_inst.Alarmen_14.string),             185, SIZE_14, "Alarmas" },                                             
  { sizeof(tekst_inst.Opties_Alarmen_10.string),      185, SIZE_10, "Optiones: Alarmas" },                                   

  { sizeof(tekst_inst.Frequentie_verstel_14.string),  185, SIZE_14, "Verstel hoge snelheid" },
  //#ifdef CAN_BACKBONE_PC_WARNING
  { sizeof(tekst_inst.PC_1_14.string),                      50, SIZE_14, "PC 1" },                // PC_1_14
  { sizeof(tekst_inst.PC_2_14.string),                      50, SIZE_14, "PC 2" },                // PC_2_14
  { sizeof(tekst_inst.PC_3_14.string),                      50, SIZE_14, "PC 3" },                // PC_3_14
  { sizeof(tekst_inst.Waarschuwing_7.string),               50, SIZE_7,  "Advertencia" },         // Waarschuwing_7
  //#endif // CAN_BACKBONE_PC_WARNING
  //#ifdef PASSWORD
  { sizeof(tekst_inst.Beheerder_14.string),                185, SIZE_14, "Beheerder" },           // Beheerder_14
  { sizeof(tekst_inst.Gebruiker_1_14.string),              185, SIZE_14, "Usuario 1" },           // Gebruiker_1_14
  { sizeof(tekst_inst.Gebruiker_2_14.string),              185, SIZE_14, "Usuario 2" },           // Gebruiker_2_14
  { sizeof(tekst_inst.Gebruiker_3_14.string),              185, SIZE_14, "Usuario 3" },           // Gebruiker_3_14
  { sizeof(tekst_inst.Gebruiker_4_14.string),              185, SIZE_14, "Usuario 4" },           // Gebruiker_4_14
  { sizeof(tekst_inst.Level_14.string),                     85, SIZE_14, "level:" },              // Level_14
  { sizeof(tekst_inst.Opties_7.string),                     80, SIZE_7,  "Options" },             // Opties_7
  { sizeof(tekst_inst.Setpoints_7.string),                  80, SIZE_7,  "Setpoints" },           // Setpoints_7
  { sizeof(tekst_inst.Setpoints_Syst_7.string),             80, SIZE_7,  "Setpoints Syst" },      // Setpoints_Syst_7
  //#endif // PASSWORD

  { sizeof(tekst_inst.BACnet_Device_ID_10.string),          80, SIZE_10,  "Device ID" },
  { sizeof(tekst_inst.BACnet_Network_Nr_10.string),         80, SIZE_10,  "Network Nr" },

  { sizeof(tekst_inst.Operation_Mode_14.string),           185, SIZE_14, "Operation Mode" },
  { sizeof(tekst_inst.Closed_Loop_14.string),              180, SIZE_14, "Closed Loop" },                                            
  { sizeof(tekst_inst.Open_Loop_14.string),                180, SIZE_14, "Open Loop" },

  { sizeof(tekst_inst.Vrijgave_ventilatoren_14.string),        180, SIZE_14, "Vent. del Lanzamiento" },
  { sizeof(tekst_inst.Opties_Vrijgave_ventilatoren_10.string), 185, SIZE_10, "Opt: Vent. del Lanzamiento" },
  { sizeof(tekst_inst.Vrijgave_14.string),                     180, SIZE_14, "del Lanzamiento" },
  { sizeof(tekst_inst.Ventilatoren_14.string),                 180, SIZE_14, "Ventiladores" },

  { sizeof(tekst_inst.PowerFactor_14.string),              185, SIZE_14, "Power factor" },
  { sizeof(tekst_inst.RampUpDown_14.string),               185, SIZE_14, "Ramp-Up/Down" },
  { sizeof(tekst_inst.RampUp_14.string),                   185, SIZE_14, "Ramp-Up" },
  { sizeof(tekst_inst.RampDown_14.string),                 185, SIZE_14, "Ramp-Down" },

  { sizeof(tekst_inst.Klep_14.string),                     150, SIZE_14, "Válvula" },
  { sizeof(tekst_inst.SensorType_14.string),               185, SIZE_14, "Sensor type" },
  { sizeof(tekst_inst.Eindschakelaar_14.string),           110, SIZE_14, "Eindschakelaar" },

  { sizeof(tekst_inst.Sensoren_14.string),                 185, SIZE_14, "Sensoren" },

  { sizeof(tekst_inst.Alarm_eindschakelaar_14.string),     180, SIZE_14, "Alarm eindschakelaar" },

  { sizeof(tekst_inst.Extern_alarm_14.string),             110, SIZE_14, "Extern alarm" },
  { sizeof(tekst_inst.Vent_At_Max_14.string),              185, SIZE_14, "Vent at 100%" },

  { sizeof(tekst_inst.Watchdog_Mode_14.string),            185, SIZE_14, "Watchdog Mode" },
  { sizeof(tekst_inst.Watchdog_Position_14.string),        185, SIZE_14, "Watchdog Position" },

  { sizeof(tekst_inst.Data_Wissen_14.string),              185, SIZE_14, "Borrar datos" },
  { sizeof(tekst_inst.Selecteer_Data_14.string),           185, SIZE_14, "Seleccionar datos" },
  { sizeof(tekst_inst.Druk_OK_Om_Te_Wissen_14.string),     185, SIZE_14, "OK = Eliminar" },
  { sizeof(tekst_inst.Data_Gewist_14.string),              185, SIZE_14, "Datos eliminados!" },
  { sizeof(tekst_inst.Setpoints_10.string),                185, SIZE_10, "Ajustes" },
  { sizeof(tekst_inst.Werkgeheugen_10.string),             185, SIZE_10, "Memoria" },
  { sizeof(tekst_inst.Restart_10.string),                  185, SIZE_10, "Reiniciar" },

};

s_tekst_8 const tekst_CANopen_14 = { 8, 70, SIZE_14, "CANopen" };
s_tekst_7 const tekst_BACnet_14  = { 7, 70, SIZE_14, "BACnet" };
s_tekst_12 const tekst_Hoogendoorn_14 = { 12, 110, SIZE_14, "TCP Link" };

s_tekst_7  const tekst_ebmBus_14           = { 7, 70, SIZE_14, "ebmBus" };
s_tekst_11 const tekst_ebm_Modbus_14       = { 7, 70, SIZE_14, "ebm Modbus" };
s_tekst_14 const tekst_ECblue_Modbus_14    = { 7, 70, SIZE_14, "ECblue Modbus" };
s_tekst_15 const tekst_ECblue_Premium_14   = { 7, 70, SIZE_14, "ECblue Premium" };
s_tekst_10 const tekst_Rosenberg_14        = { 7, 70, SIZE_14, "Rosenberg" };
s_tekst_9  const tekst_Climafan_14         = { 7, 70, SIZE_14, "Climafan" };
s_tekst_20 const tekst_Rosenberg_Gen3_14   = { 7, 70, SIZE_14, "Rosenberg Gen3" };
s_tekst_21 const tekst_Nicotra_Gebhardt_14 = { 7, 70, SIZE_14, "Nicotra Gebhardt PFP" };

s_tekst_5 const tekst_Volt_14   = { 5, 70, SIZE_14, "Volt" };
s_tekst_5 const tekst_mA_14     = { 5, 70, SIZE_14, "mA" };
s_tekst_8 const tekst_0_5V_14   = { 8, 70, SIZE_14, "0..5V" };
s_tekst_8 const tekst_0_20mA_14 = { 8, 70, SIZE_14, "0..20mA" };
s_tekst_2 const tekst_dp_7 = { 2, 3, SIZE_7, ":" };
s_tekst_2 const tekst_dp_10 = { 2, 4, SIZE_10, ":" };
s_tekst_2 const tekst_dp_14 = { 2, 4, SIZE_14, ":" };
s_tekst_2 const tekst_punt_7 = { 2, 3, SIZE_7, "." };
s_tekst_2 const tekst_punt_10 = { 2, 4, SIZE_10, "." };
s_tekst_2 const tekst_punt_14 = { 2, 4, SIZE_14, "." };
s_tekst_2 const tekst_is_7  = { 2, 6, SIZE_7,  "=" };
s_tekst_2 const tekst_is_10 = { 2, 8, SIZE_10, "=" };
s_tekst_2 const tekst_is_14 = { 2, 9, SIZE_14, "=" };
s_tekst_3 const tekst_ok_7  = { 3, 14, SIZE_7,  "OK" };
s_tekst_3 const tekst_ok_10 = { 3, 19, SIZE_10, "OK" };
s_tekst_3 const tekst_ok_14 = { 3, 24, SIZE_14, "OK" };
s_tekst_7 const tekst_USER_14 = { 7, 88, SIZE_14, "(user)" };
s_tekst_1 const tekst_leeg = { 1, 0, SIZE_14, "" };
s_tekst_2 const tekst_space_7 = { 2, 3, SIZE_7, " " };
s_tekst_2 const tekst_space_10 = { 2, 5, SIZE_10, " " };
s_tekst_2 const tekst_space_14 = { 2, 5, SIZE_14, " " };
s_tekst_2 const tekst_min_7 = { 2, 5, SIZE_7, "-" };
s_tekst_2 const tekst_min_10 = { 2, 5, SIZE_10, "-" };
s_tekst_2 const tekst_min_14 = { 2, 9, SIZE_14, "-" };
s_tekst_2 const tekst_ronde_openings_haak_7 = { 2, 8, SIZE_7, "(" };
s_tekst_2 const tekst_ronde_sluit_haak_7 = { 2, 8, SIZE_7, ")" };
s_tekst_2 const tekst_ronde_openings_haak_10 = { 2, 8, SIZE_10, "(" };
s_tekst_2 const tekst_ronde_sluit_haak_10 = { 2, 8, SIZE_10, ")" };
s_tekst_2 const tekst_ronde_openings_haak_14 = { 2, 8, SIZE_14, "(" };
s_tekst_2 const tekst_ronde_sluit_haak_14 = { 2, 8, SIZE_14, ")" };
s_tekst_2 const tekst_rechte_openings_haak_10 = { 2, 8, SIZE_10, "[" };
s_tekst_2 const tekst_rechte_sluit_haak_10 = { 2, 8, SIZE_10, "]" };
s_tekst_2 const tekst_rechte_openings_haak_14 = { 2, 8, SIZE_14, "[" };
s_tekst_2 const tekst_rechte_sluit_haak_14 = { 2, 8, SIZE_14, "]" };
s_tekst_10 const tekst_computer_14 = { 10, 50, SIZE_14, "ORION" };
s_tekst_10 const tekst_soort_14 = { 10, 40, SIZE_14, "-FWS" };
s_tekst_1 const tekst_BASIS_14[] = { 1, 0, SIZE_14, "" };
void const * const tekst_type_14[1] = { &tekst_BASIS_14 };
void const * const tekst_graden[3] = { &tekst.graden_celsius_7, &tekst.graden_fahrenheid_7, &tekst_leeg };
void const * const tekst_inst_graden[3] = { &tekst_inst.graden_celsius_7, &tekst_inst.graden_fahrenheid_7, &tekst_leeg };
s_disp_tekst_array const disp_14_graden = { Disp_Draw_Tekst_Array_L, 210, 13, tekst_graden, UCHAR, &temp_unit, 3 };
s_disp_tekst_array const disp_14_curve_temp_graden = { Disp_Draw_Tekst_Array_L, 94, 13, tekst_graden, UCHAR, &temp_unit, 3 };

s_tekst_12 const tekst_IO_06_14_1_10 = { 12, 90, SIZE_10, "IO-06-14[1]" };
s_tekst_12 const tekst_IO_06_14_1_14 = { 12, 90, SIZE_14, "IO-06-14[1]" };
s_tekst_12 const tekst_IO_06_14_2_10 = { 12, 90, SIZE_10, "IO-06-14[2]" };
s_tekst_12 const tekst_IO_06_14_2_14 = { 12, 90, SIZE_14, "IO-06-14[2]" };
s_tekst_12 const tekst_IO_06_14_3_10 = { 12, 90, SIZE_10, "IO-06-14[3]" };
s_tekst_12 const tekst_IO_06_14_3_14 = { 12, 90, SIZE_14, "IO-06-14[3]" };
s_tekst_12 const tekst_IO_06_14_4_10 = { 12, 90, SIZE_10, "IO-06-14[4]" };
s_tekst_12 const tekst_IO_06_14_4_14 = { 12, 90, SIZE_14, "IO-06-14[4]" };
s_tekst_12 const tekst_IO_06_14_e_10 = { 12, 89, SIZE_10, "IO-06-14[-]" };
s_tekst_12 const tekst_IO_06_14_e_14 = { 12, 89, SIZE_14, "IO-06-14[-]" };

s_tekst_12 const tekst_IO_12_06_1_10 = { 12, 90, SIZE_10, "IO-12-06[1]" };
s_tekst_12 const tekst_IO_12_06_1_14 = { 12, 90, SIZE_14, "IO-12-06[1]" };
s_tekst_12 const tekst_IO_12_06_2_10 = { 12, 90, SIZE_10, "IO-12-06[2]" };
s_tekst_12 const tekst_IO_12_06_2_14 = { 12, 90, SIZE_14, "IO-12-06[2]" };
s_tekst_12 const tekst_IO_12_06_3_10 = { 12, 90, SIZE_10, "IO-12-06[3]" };
s_tekst_12 const tekst_IO_12_06_3_14 = { 12, 90, SIZE_14, "IO-12-06[3]" };
s_tekst_12 const tekst_IO_12_06_4_10 = { 12, 90, SIZE_10, "IO-12-06[4]" };
s_tekst_12 const tekst_IO_12_06_4_14 = { 12, 90, SIZE_14, "IO-12-06[4]" };
s_tekst_12 const tekst_IO_12_06_e_10 = { 12, 89, SIZE_10, "IO-12-06[-]" };
s_tekst_12 const tekst_IO_12_06_e_14 = { 12, 89, SIZE_14, "IO-12-06[-]" };

s_tekst_12 const tekst_IO_08_09_1_10 = { 12, 90, SIZE_10, "IO-08-09[1]" };
s_tekst_12 const tekst_IO_08_09_1_14 = { 12, 90, SIZE_14, "IO-08-09[1]" };
s_tekst_12 const tekst_IO_08_09_2_10 = { 12, 90, SIZE_10, "IO-08-09[2]" };
s_tekst_12 const tekst_IO_08_09_2_14 = { 12, 90, SIZE_14, "IO-08-09[2]" };
s_tekst_12 const tekst_IO_08_09_3_10 = { 12, 90, SIZE_10, "IO-08-09[3]" };
s_tekst_12 const tekst_IO_08_09_3_14 = { 12, 90, SIZE_14, "IO-08-09[3]" };
s_tekst_12 const tekst_IO_08_09_4_10 = { 12, 90, SIZE_10, "IO-08-09[4]" };
s_tekst_12 const tekst_IO_08_09_4_14 = { 12, 90, SIZE_14, "IO-08-09[4]" };
s_tekst_12 const tekst_IO_08_09_5_10 = { 12, 90, SIZE_10, "IO-08-09[5]" };
s_tekst_12 const tekst_IO_08_09_5_14 = { 12, 90, SIZE_14, "IO-08-09[5]" };
s_tekst_12 const tekst_IO_08_09_6_10 = { 12, 90, SIZE_10, "IO-08-09[6]" };
s_tekst_12 const tekst_IO_08_09_6_14 = { 12, 90, SIZE_14, "IO-08-09[6]" };
s_tekst_12 const tekst_IO_08_09_7_10 = { 12, 90, SIZE_10, "IO-08-09[7]" };
s_tekst_12 const tekst_IO_08_09_7_14 = { 12, 90, SIZE_14, "IO-08-09[7]" };
s_tekst_12 const tekst_IO_08_09_8_10 = { 12, 90, SIZE_10, "IO-08-09[8]" };
s_tekst_12 const tekst_IO_08_09_8_14 = { 12, 90, SIZE_14, "IO-08-09[8]" };
s_tekst_12 const tekst_IO_08_09_e_10 = { 12, 89, SIZE_10, "IO-08-09[-]" };
s_tekst_12 const tekst_IO_08_09_e_14 = { 12, 89, SIZE_14, "IO-08-09[-]" };

s_tekst_12 const tekst_IO_05_07_1_10  = { 12, 90, SIZE_10, "IO-05-07[1]"  };
s_tekst_12 const tekst_IO_05_07_1_14  = { 12, 90, SIZE_14, "IO-05-07[1]"  };
s_tekst_12 const tekst_IO_05_07_2_10  = { 12, 90, SIZE_10, "IO-05-07[2]"  };
s_tekst_12 const tekst_IO_05_07_2_14  = { 12, 90, SIZE_14, "IO-05-07[2]"  };
s_tekst_12 const tekst_IO_05_07_3_10  = { 12, 90, SIZE_10, "IO-05-07[3]"  };
s_tekst_12 const tekst_IO_05_07_3_14  = { 12, 90, SIZE_14, "IO-05-07[3]"  };
s_tekst_12 const tekst_IO_05_07_4_10  = { 12, 90, SIZE_10, "IO-05-07[4]"  };
s_tekst_12 const tekst_IO_05_07_4_14  = { 12, 90, SIZE_14, "IO-05-07[4]"  };
s_tekst_12 const tekst_IO_05_07_5_10  = { 12, 90, SIZE_10, "IO-05-07[5]"  };
s_tekst_12 const tekst_IO_05_07_5_14  = { 12, 90, SIZE_14, "IO-05-07[5]"  };
s_tekst_12 const tekst_IO_05_07_6_10  = { 12, 90, SIZE_10, "IO-05-07[6]"  };
s_tekst_12 const tekst_IO_05_07_6_14  = { 12, 90, SIZE_14, "IO-05-07[6]"  };
s_tekst_12 const tekst_IO_05_07_7_10  = { 12, 90, SIZE_10, "IO-05-07[7]"  };
s_tekst_12 const tekst_IO_05_07_7_14  = { 12, 90, SIZE_14, "IO-05-07[7]"  };
s_tekst_12 const tekst_IO_05_07_8_10  = { 12, 90, SIZE_10, "IO-05-07[8]"  };
s_tekst_12 const tekst_IO_05_07_8_14  = { 12, 90, SIZE_14, "IO-05-07[8]"  };
s_tekst_12 const tekst_IO_05_07_9_10  = { 12, 90, SIZE_10, "IO-05-07[9]"  };
s_tekst_12 const tekst_IO_05_07_9_14  = { 12, 90, SIZE_14, "IO-05-07[9]"  };
s_tekst_12 const tekst_IO_05_07_10_10 = { 12, 90, SIZE_10, "IO-05-07[10]" };
s_tekst_12 const tekst_IO_05_07_10_14 = { 12, 90, SIZE_14, "IO-05-07[10]" };
s_tekst_12 const tekst_IO_05_07_11_10 = { 12, 90, SIZE_10, "IO-05-07[11]" };
s_tekst_12 const tekst_IO_05_07_11_14 = { 12, 90, SIZE_14, "IO-05-07[11]" };
s_tekst_12 const tekst_IO_05_07_12_10 = { 12, 90, SIZE_10, "IO-05-07[12]" };
s_tekst_12 const tekst_IO_05_07_12_14 = { 12, 90, SIZE_14, "IO-05-07[12]" };
s_tekst_12 const tekst_IO_05_07_13_10 = { 12, 90, SIZE_10, "IO-05-07[13]" };
s_tekst_12 const tekst_IO_05_07_13_14 = { 12, 90, SIZE_14, "IO-05-07[13]" };
s_tekst_12 const tekst_IO_05_07_14_10 = { 12, 90, SIZE_10, "IO-05-07[14]" };
s_tekst_12 const tekst_IO_05_07_14_14 = { 12, 90, SIZE_14, "IO-05-07[14]" };
s_tekst_12 const tekst_IO_05_07_15_10 = { 12, 90, SIZE_10, "IO-05-07[15]" };
s_tekst_12 const tekst_IO_05_07_15_14 = { 12, 90, SIZE_14, "IO-05-07[15]" };
s_tekst_12 const tekst_IO_05_07_16_10 = { 12, 90, SIZE_10, "IO-05-07[16]" };
s_tekst_12 const tekst_IO_05_07_16_14 = { 12, 90, SIZE_14, "IO-05-07[16]" };
s_tekst_12 const tekst_IO_05_07_e_10  = { 12, 89, SIZE_10, "IO-05-07[-]"  };
s_tekst_12 const tekst_IO_05_07_e_14  = { 12, 89, SIZE_14, "IO-05-07[-]"  };

s_tekst_12 const tekst_IO_07_07_1_10  = { 12, 90, SIZE_10, "IO-07-07[1]"  };
s_tekst_12 const tekst_IO_07_07_1_14  = { 12, 90, SIZE_14, "IO-07-07[1]"  };
s_tekst_12 const tekst_IO_07_07_2_10  = { 12, 90, SIZE_10, "IO-07-07[2]"  };
s_tekst_12 const tekst_IO_07_07_2_14  = { 12, 90, SIZE_14, "IO-07-07[2]"  };
s_tekst_12 const tekst_IO_07_07_3_10  = { 12, 90, SIZE_10, "IO-07-07[3]"  };
s_tekst_12 const tekst_IO_07_07_3_14  = { 12, 90, SIZE_14, "IO-07-07[3]"  };
s_tekst_12 const tekst_IO_07_07_4_10  = { 12, 90, SIZE_10, "IO-07-07[4]"  };
s_tekst_12 const tekst_IO_07_07_4_14  = { 12, 90, SIZE_14, "IO-07-07[4]"  };
s_tekst_12 const tekst_IO_07_07_5_10  = { 12, 90, SIZE_10, "IO-07-07[5]"  };
s_tekst_12 const tekst_IO_07_07_5_14  = { 12, 90, SIZE_14, "IO-07-07[5]"  };
s_tekst_12 const tekst_IO_07_07_6_10  = { 12, 90, SIZE_10, "IO-07-07[6]"  };
s_tekst_12 const tekst_IO_07_07_6_14  = { 12, 90, SIZE_14, "IO-07-07[6]"  };
s_tekst_12 const tekst_IO_07_07_7_10  = { 12, 90, SIZE_10, "IO-07-07[7]"  };
s_tekst_12 const tekst_IO_07_07_7_14  = { 12, 90, SIZE_14, "IO-07-07[7]"  };
s_tekst_12 const tekst_IO_07_07_8_10  = { 12, 90, SIZE_10, "IO-07-07[8]"  };
s_tekst_12 const tekst_IO_07_07_8_14  = { 12, 90, SIZE_14, "IO-07-07[8]"  };
s_tekst_12 const tekst_IO_07_07_9_10  = { 12, 90, SIZE_10, "IO-07-07[9]"  };
s_tekst_12 const tekst_IO_07_07_9_14  = { 12, 90, SIZE_14, "IO-07-07[9]"  };
s_tekst_12 const tekst_IO_07_07_10_10 = { 12, 90, SIZE_10, "IO-07-07[10]" };
s_tekst_12 const tekst_IO_07_07_10_14 = { 12, 90, SIZE_14, "IO-07-07[10]" };
s_tekst_12 const tekst_IO_07_07_11_10 = { 12, 90, SIZE_10, "IO-07-07[11]" };
s_tekst_12 const tekst_IO_07_07_11_14 = { 12, 90, SIZE_14, "IO-07-07[11]" };
s_tekst_12 const tekst_IO_07_07_12_10 = { 12, 90, SIZE_10, "IO-07-07[12]" };
s_tekst_12 const tekst_IO_07_07_12_14 = { 12, 90, SIZE_14, "IO-07-07[12]" };
s_tekst_12 const tekst_IO_07_07_13_10 = { 12, 90, SIZE_10, "IO-07-07[13]" };
s_tekst_12 const tekst_IO_07_07_13_14 = { 12, 90, SIZE_14, "IO-07-07[13]" };
s_tekst_12 const tekst_IO_07_07_14_10 = { 12, 90, SIZE_10, "IO-07-07[14]" };
s_tekst_12 const tekst_IO_07_07_14_14 = { 12, 90, SIZE_14, "IO-07-07[14]" };
s_tekst_12 const tekst_IO_07_07_15_10 = { 12, 90, SIZE_10, "IO-07-07[15]" };
s_tekst_12 const tekst_IO_07_07_15_14 = { 12, 90, SIZE_14, "IO-07-07[15]" };
s_tekst_12 const tekst_IO_07_07_16_10 = { 12, 90, SIZE_10, "IO-07-07[16]" };
s_tekst_12 const tekst_IO_07_07_16_14 = { 12, 90, SIZE_14, "IO-07-07[16]" };
s_tekst_12 const tekst_IO_07_07_17_10 = { 12, 90, SIZE_10, "IO-07-07[17]" };
s_tekst_12 const tekst_IO_07_07_17_14 = { 12, 90, SIZE_14, "IO-07-07[17]" };
s_tekst_12 const tekst_IO_07_07_18_10 = { 12, 90, SIZE_10, "IO-07-07[18]" };
s_tekst_12 const tekst_IO_07_07_18_14 = { 12, 90, SIZE_14, "IO-07-07[18]" };
s_tekst_12 const tekst_IO_07_07_19_10 = { 12, 90, SIZE_10, "IO-07-07[19]" };
s_tekst_12 const tekst_IO_07_07_19_14 = { 12, 90, SIZE_14, "IO-07-07[19]" };
s_tekst_12 const tekst_IO_07_07_20_10 = { 12, 90, SIZE_10, "IO-07-07[20]" };
s_tekst_12 const tekst_IO_07_07_20_14 = { 12, 90, SIZE_14, "IO-07-07[20]" };
s_tekst_12 const tekst_IO_07_07_21_10 = { 12, 90, SIZE_10, "IO-07-07[21]" };
s_tekst_12 const tekst_IO_07_07_21_14 = { 12, 90, SIZE_14, "IO-07-07[21]" };
s_tekst_12 const tekst_IO_07_07_22_10 = { 12, 90, SIZE_10, "IO-07-07[22]" };
s_tekst_12 const tekst_IO_07_07_22_14 = { 12, 90, SIZE_14, "IO-07-07[22]" };
s_tekst_12 const tekst_IO_07_07_23_10 = { 12, 90, SIZE_10, "IO-07-07[23]" };
s_tekst_12 const tekst_IO_07_07_23_14 = { 12, 90, SIZE_14, "IO-07-07[23]" };
s_tekst_12 const tekst_IO_07_07_24_10 = { 12, 90, SIZE_10, "IO-07-07[24]" };
s_tekst_12 const tekst_IO_07_07_24_14 = { 12, 90, SIZE_14, "IO-07-07[24]" };
s_tekst_12 const tekst_IO_07_07_25_10 = { 12, 90, SIZE_10, "IO-07-07[25]" };
s_tekst_12 const tekst_IO_07_07_25_14 = { 12, 90, SIZE_14, "IO-07-07[25]" };
s_tekst_12 const tekst_IO_07_07_26_10 = { 12, 90, SIZE_10, "IO-07-07[26]" };
s_tekst_12 const tekst_IO_07_07_26_14 = { 12, 90, SIZE_14, "IO-07-07[26]" };
s_tekst_12 const tekst_IO_07_07_27_10 = { 12, 90, SIZE_10, "IO-07-07[27]" };
s_tekst_12 const tekst_IO_07_07_27_14 = { 12, 90, SIZE_14, "IO-07-07[27]" };
s_tekst_12 const tekst_IO_07_07_28_10 = { 12, 90, SIZE_10, "IO-07-07[28]" };
s_tekst_12 const tekst_IO_07_07_28_14 = { 12, 90, SIZE_14, "IO-07-07[28]" };
s_tekst_12 const tekst_IO_07_07_29_10 = { 12, 90, SIZE_10, "IO-07-07[29]" };
s_tekst_12 const tekst_IO_07_07_29_14 = { 12, 90, SIZE_14, "IO-07-07[29]" };
s_tekst_12 const tekst_IO_07_07_30_10 = { 12, 90, SIZE_10, "IO-07-07[30]" };
s_tekst_12 const tekst_IO_07_07_30_14 = { 12, 90, SIZE_14, "IO-07-07[30]" };
s_tekst_12 const tekst_IO_07_07_31_10 = { 12, 90, SIZE_10, "IO-07-07[31]" };
s_tekst_12 const tekst_IO_07_07_31_14 = { 12, 90, SIZE_14, "IO-07-07[31]" };
s_tekst_12 const tekst_IO_07_07_32_10 = { 12, 90, SIZE_10, "IO-07-07[32]" };
s_tekst_12 const tekst_IO_07_07_32_14 = { 12, 90, SIZE_14, "IO-07-07[32]" };
s_tekst_12 const tekst_IO_07_07_e_10  = { 12, 89, SIZE_10, "IO-07-07[-]"  };
s_tekst_12 const tekst_IO_07_07_e_14  = { 12, 89, SIZE_14, "IO-07-07[-]"  };

s_tekst_12 const tekst_IO_EKU_1_10  = { 12, 90, SIZE_10, "IO-EKU[1]"  };
s_tekst_12 const tekst_IO_EKU_1_14  = { 12, 90, SIZE_14, "IO-EKU[1]"  };
s_tekst_12 const tekst_IO_EKU_2_10  = { 12, 90, SIZE_10, "IO-EKU[2]"  };
s_tekst_12 const tekst_IO_EKU_2_14  = { 12, 90, SIZE_14, "IO-EKU[2]"  };
s_tekst_12 const tekst_IO_EKU_3_10  = { 12, 90, SIZE_10, "IO-EKU[3]"  };
s_tekst_12 const tekst_IO_EKU_3_14  = { 12, 90, SIZE_14, "IO-EKU[3]"  };
s_tekst_12 const tekst_IO_EKU_4_10  = { 12, 90, SIZE_10, "IO-EKU[4]"  };
s_tekst_12 const tekst_IO_EKU_4_14  = { 12, 90, SIZE_14, "IO-EKU[4]"  };
s_tekst_12 const tekst_IO_EKU_5_10  = { 12, 90, SIZE_10, "IO-EKU[5]"  };
s_tekst_12 const tekst_IO_EKU_5_14  = { 12, 90, SIZE_14, "IO-EKU[5]"  };
s_tekst_12 const tekst_IO_EKU_6_10  = { 12, 90, SIZE_10, "IO-EKU[6]"  };
s_tekst_12 const tekst_IO_EKU_6_14  = { 12, 90, SIZE_14, "IO-EKU[6]"  };
s_tekst_12 const tekst_IO_EKU_7_10  = { 12, 90, SIZE_10, "IO-EKU[7]"  };
s_tekst_12 const tekst_IO_EKU_7_14  = { 12, 90, SIZE_14, "IO-EKU[7]"  };
s_tekst_12 const tekst_IO_EKU_8_10  = { 12, 90, SIZE_10, "IO-EKU[8]"  };
s_tekst_12 const tekst_IO_EKU_8_14  = { 12, 90, SIZE_14, "IO-EKU[8]"  };
s_tekst_12 const tekst_IO_EKU_9_10  = { 12, 90, SIZE_10, "IO-EKU[9]"  };
s_tekst_12 const tekst_IO_EKU_9_14  = { 12, 90, SIZE_14, "IO-EKU[9]"  };
s_tekst_12 const tekst_IO_EKU_10_10 = { 12, 90, SIZE_10, "IO-EKU[10]" };
s_tekst_12 const tekst_IO_EKU_10_14 = { 12, 90, SIZE_14, "IO-EKU[10]" };
s_tekst_12 const tekst_IO_EKU_11_10 = { 12, 90, SIZE_10, "IO-EKU[11]" };
s_tekst_12 const tekst_IO_EKU_11_14 = { 12, 90, SIZE_14, "IO-EKU[11]" };
s_tekst_12 const tekst_IO_EKU_12_10 = { 12, 90, SIZE_10, "IO-EKU[12]" };
s_tekst_12 const tekst_IO_EKU_12_14 = { 12, 90, SIZE_14, "IO-EKU[12]" };
s_tekst_12 const tekst_IO_EKU_13_10 = { 12, 90, SIZE_10, "IO-EKU[13]" };
s_tekst_12 const tekst_IO_EKU_13_14 = { 12, 90, SIZE_14, "IO-EKU[13]" };
s_tekst_12 const tekst_IO_EKU_14_10 = { 12, 90, SIZE_10, "IO-EKU[14]" };
s_tekst_12 const tekst_IO_EKU_14_14 = { 12, 90, SIZE_14, "IO-EKU[14]" };
s_tekst_12 const tekst_IO_EKU_15_10 = { 12, 90, SIZE_10, "IO-EKU[15]" };
s_tekst_12 const tekst_IO_EKU_15_14 = { 12, 90, SIZE_14, "IO-EKU[15]" };
s_tekst_12 const tekst_IO_EKU_16_10 = { 12, 90, SIZE_10, "IO-EKU[16]" };
s_tekst_12 const tekst_IO_EKU_16_14 = { 12, 90, SIZE_14, "IO-EKU[16]" };
s_tekst_12 const tekst_IO_EKU_e_10  = { 12, 89, SIZE_10, "IO-EKU[-]"  };
s_tekst_12 const tekst_IO_EKU_e_14  = { 12, 89, SIZE_14, "IO-EKU[-]"  };

s_tekst_12 const tekst_IO_H2MC_1_10  = { 12, 90, SIZE_10, "IO-H2MC[1]"  };
s_tekst_12 const tekst_IO_H2MC_1_14  = { 12, 90, SIZE_14, "IO-H2MC[1]"  };
s_tekst_12 const tekst_IO_H2MC_2_10  = { 12, 90, SIZE_10, "IO-H2MC[2]"  };
s_tekst_12 const tekst_IO_H2MC_2_14  = { 12, 90, SIZE_14, "IO-H2MC[2]"  };
s_tekst_12 const tekst_IO_H2MC_3_10  = { 12, 90, SIZE_10, "IO-H2MC[3]"  };
s_tekst_12 const tekst_IO_H2MC_3_14  = { 12, 90, SIZE_14, "IO-H2MC[3]"  };
s_tekst_12 const tekst_IO_H2MC_4_10  = { 12, 90, SIZE_10, "IO-H2MC[4]"  };
s_tekst_12 const tekst_IO_H2MC_4_14  = { 12, 90, SIZE_14, "IO-H2MC[4]"  };
s_tekst_12 const tekst_IO_H2MC_5_10  = { 12, 90, SIZE_10, "IO-H2MC[5]"  };
s_tekst_12 const tekst_IO_H2MC_5_14  = { 12, 90, SIZE_14, "IO-H2MC[5]"  };
s_tekst_12 const tekst_IO_H2MC_6_10  = { 12, 90, SIZE_10, "IO-H2MC[6]"  };
s_tekst_12 const tekst_IO_H2MC_6_14  = { 12, 90, SIZE_14, "IO-H2MC[6]"  };
s_tekst_12 const tekst_IO_H2MC_7_10  = { 12, 90, SIZE_10, "IO-H2MC[7]"  };
s_tekst_12 const tekst_IO_H2MC_7_14  = { 12, 90, SIZE_14, "IO-H2MC[7]"  };
s_tekst_12 const tekst_IO_H2MC_8_10  = { 12, 90, SIZE_10, "IO-H2MC[8]"  };
s_tekst_12 const tekst_IO_H2MC_8_14  = { 12, 90, SIZE_14, "IO-H2MC[8]"  };
s_tekst_12 const tekst_IO_H2MC_9_10  = { 12, 90, SIZE_10, "IO-H2MC[9]"  };
s_tekst_12 const tekst_IO_H2MC_9_14  = { 12, 90, SIZE_14, "IO-H2MC[9]"  };
s_tekst_12 const tekst_IO_H2MC_10_10 = { 12, 90, SIZE_10, "IO-H2MC[10]" };
s_tekst_12 const tekst_IO_H2MC_10_14 = { 12, 90, SIZE_14, "IO-H2MC[10]" };
s_tekst_12 const tekst_IO_H2MC_11_10 = { 12, 90, SIZE_10, "IO-H2MC[11]" };
s_tekst_12 const tekst_IO_H2MC_11_14 = { 12, 90, SIZE_14, "IO-H2MC[11]" };
s_tekst_12 const tekst_IO_H2MC_12_10 = { 12, 90, SIZE_10, "IO-H2MC[12]" };
s_tekst_12 const tekst_IO_H2MC_12_14 = { 12, 90, SIZE_14, "IO-H2MC[12]" };
s_tekst_12 const tekst_IO_H2MC_13_10 = { 12, 90, SIZE_10, "IO-H2MC[13]" };
s_tekst_12 const tekst_IO_H2MC_13_14 = { 12, 90, SIZE_14, "IO-H2MC[13]" };
s_tekst_12 const tekst_IO_H2MC_14_10 = { 12, 90, SIZE_10, "IO-H2MC[14]" };
s_tekst_12 const tekst_IO_H2MC_14_14 = { 12, 90, SIZE_14, "IO-H2MC[14]" };
s_tekst_12 const tekst_IO_H2MC_15_10 = { 12, 90, SIZE_10, "IO-H2MC[15]" };
s_tekst_12 const tekst_IO_H2MC_15_14 = { 12, 90, SIZE_14, "IO-H2MC[15]" };
s_tekst_12 const tekst_IO_H2MC_16_10 = { 12, 90, SIZE_10, "IO-H2MC[16]" };
s_tekst_12 const tekst_IO_H2MC_16_14 = { 12, 90, SIZE_14, "IO-H2MC[16]" };
s_tekst_12 const tekst_IO_H2MC_17_10 = { 12, 90, SIZE_10, "IO-H2MC[17]" };
s_tekst_12 const tekst_IO_H2MC_17_14 = { 12, 90, SIZE_14, "IO-H2MC[17]" };
s_tekst_12 const tekst_IO_H2MC_18_10 = { 12, 90, SIZE_10, "IO-H2MC[18]" };
s_tekst_12 const tekst_IO_H2MC_18_14 = { 12, 90, SIZE_14, "IO-H2MC[18]" };
s_tekst_12 const tekst_IO_H2MC_19_10 = { 12, 90, SIZE_10, "IO-H2MC[19]" };
s_tekst_12 const tekst_IO_H2MC_19_14 = { 12, 90, SIZE_14, "IO-H2MC[19]" };
s_tekst_12 const tekst_IO_H2MC_20_10 = { 12, 90, SIZE_10, "IO-H2MC[20]" };
s_tekst_12 const tekst_IO_H2MC_20_14 = { 12, 90, SIZE_14, "IO-H2MC[20]" };
s_tekst_12 const tekst_IO_H2MC_21_10 = { 12, 90, SIZE_10, "IO-H2MC[21]" };
s_tekst_12 const tekst_IO_H2MC_21_14 = { 12, 90, SIZE_14, "IO-H2MC[21]" };
s_tekst_12 const tekst_IO_H2MC_22_10 = { 12, 90, SIZE_10, "IO-H2MC[22]" };
s_tekst_12 const tekst_IO_H2MC_22_14 = { 12, 90, SIZE_14, "IO-H2MC[22]" };
s_tekst_12 const tekst_IO_H2MC_23_10 = { 12, 90, SIZE_10, "IO-H2MC[23]" };
s_tekst_12 const tekst_IO_H2MC_23_14 = { 12, 90, SIZE_14, "IO-H2MC[23]" };
s_tekst_12 const tekst_IO_H2MC_24_10 = { 12, 90, SIZE_10, "IO-H2MC[24]" };
s_tekst_12 const tekst_IO_H2MC_24_14 = { 12, 90, SIZE_14, "IO-H2MC[24]" };
s_tekst_12 const tekst_IO_H2MC_25_10 = { 12, 90, SIZE_10, "IO-H2MC[25]" };
s_tekst_12 const tekst_IO_H2MC_25_14 = { 12, 90, SIZE_14, "IO-H2MC[25]" };
s_tekst_12 const tekst_IO_H2MC_26_10 = { 12, 90, SIZE_10, "IO-H2MC[26]" };
s_tekst_12 const tekst_IO_H2MC_26_14 = { 12, 90, SIZE_14, "IO-H2MC[26]" };
s_tekst_12 const tekst_IO_H2MC_27_10 = { 12, 90, SIZE_10, "IO-H2MC[27]" };
s_tekst_12 const tekst_IO_H2MC_27_14 = { 12, 90, SIZE_14, "IO-H2MC[27]" };
s_tekst_12 const tekst_IO_H2MC_28_10 = { 12, 90, SIZE_10, "IO-H2MC[28]" };
s_tekst_12 const tekst_IO_H2MC_28_14 = { 12, 90, SIZE_14, "IO-H2MC[28]" };
s_tekst_12 const tekst_IO_H2MC_29_10 = { 12, 90, SIZE_10, "IO-H2MC[29]" };
s_tekst_12 const tekst_IO_H2MC_29_14 = { 12, 90, SIZE_14, "IO-H2MC[29]" };
s_tekst_12 const tekst_IO_H2MC_30_10 = { 12, 90, SIZE_10, "IO-H2MC[30]" };
s_tekst_12 const tekst_IO_H2MC_30_14 = { 12, 90, SIZE_14, "IO-H2MC[30]" };
s_tekst_12 const tekst_IO_H2MC_31_10 = { 12, 90, SIZE_10, "IO-H2MC[31]" };
s_tekst_12 const tekst_IO_H2MC_31_14 = { 12, 90, SIZE_14, "IO-H2MC[31]" };
s_tekst_12 const tekst_IO_H2MC_32_10 = { 12, 90, SIZE_10, "IO-H2MC[32]" };
s_tekst_12 const tekst_IO_H2MC_32_14 = { 12, 90, SIZE_14, "IO-H2MC[32]" };
s_tekst_12 const tekst_IO_H2MC_e_10  = { 12, 89, SIZE_10, "IO-H2MC[-]"  };
s_tekst_12 const tekst_IO_H2MC_e_14  = { 12, 89, SIZE_14, "IO-H2MC[-]"  };

s_tekst_12 const tekst_IO_H1MC_1_10  = { 12, 90, SIZE_10, "IO-H1MC[1]"  };
s_tekst_12 const tekst_IO_H1MC_1_14  = { 12, 90, SIZE_14, "IO-H1MC[1]"  };
s_tekst_12 const tekst_IO_H1MC_2_10  = { 12, 90, SIZE_10, "IO-H1MC[2]"  };
s_tekst_12 const tekst_IO_H1MC_2_14  = { 12, 90, SIZE_14, "IO-H1MC[2]"  };
s_tekst_12 const tekst_IO_H1MC_3_10  = { 12, 90, SIZE_10, "IO-H1MC[3]"  };
s_tekst_12 const tekst_IO_H1MC_3_14  = { 12, 90, SIZE_14, "IO-H1MC[3]"  };
s_tekst_12 const tekst_IO_H1MC_4_10  = { 12, 90, SIZE_10, "IO-H1MC[4]"  };
s_tekst_12 const tekst_IO_H1MC_4_14  = { 12, 90, SIZE_14, "IO-H1MC[4]"  };
s_tekst_12 const tekst_IO_H1MC_5_10  = { 12, 90, SIZE_10, "IO-H1MC[5]"  };
s_tekst_12 const tekst_IO_H1MC_5_14  = { 12, 90, SIZE_14, "IO-H1MC[5]"  };
s_tekst_12 const tekst_IO_H1MC_6_10  = { 12, 90, SIZE_10, "IO-H1MC[6]"  };
s_tekst_12 const tekst_IO_H1MC_6_14  = { 12, 90, SIZE_14, "IO-H1MC[6]"  };
s_tekst_12 const tekst_IO_H1MC_7_10  = { 12, 90, SIZE_10, "IO-H1MC[7]"  };
s_tekst_12 const tekst_IO_H1MC_7_14  = { 12, 90, SIZE_14, "IO-H1MC[7]"  };
s_tekst_12 const tekst_IO_H1MC_8_10  = { 12, 90, SIZE_10, "IO-H1MC[8]"  };
s_tekst_12 const tekst_IO_H1MC_8_14  = { 12, 90, SIZE_14, "IO-H1MC[8]"  };
s_tekst_12 const tekst_IO_H1MC_9_10  = { 12, 90, SIZE_10, "IO-H1MC[9]"  };
s_tekst_12 const tekst_IO_H1MC_9_14  = { 12, 90, SIZE_14, "IO-H1MC[9]"  };
s_tekst_12 const tekst_IO_H1MC_10_10 = { 12, 90, SIZE_10, "IO-H1MC[10]" };
s_tekst_12 const tekst_IO_H1MC_10_14 = { 12, 90, SIZE_14, "IO-H1MC[10]" };
s_tekst_12 const tekst_IO_H1MC_11_10 = { 12, 90, SIZE_10, "IO-H1MC[11]" };
s_tekst_12 const tekst_IO_H1MC_11_14 = { 12, 90, SIZE_14, "IO-H1MC[11]" };
s_tekst_12 const tekst_IO_H1MC_12_10 = { 12, 90, SIZE_10, "IO-H1MC[12]" };
s_tekst_12 const tekst_IO_H1MC_12_14 = { 12, 90, SIZE_14, "IO-H1MC[12]" };
s_tekst_12 const tekst_IO_H1MC_13_10 = { 12, 90, SIZE_10, "IO-H1MC[13]" };
s_tekst_12 const tekst_IO_H1MC_13_14 = { 12, 90, SIZE_14, "IO-H1MC[13]" };
s_tekst_12 const tekst_IO_H1MC_14_10 = { 12, 90, SIZE_10, "IO-H1MC[14]" };
s_tekst_12 const tekst_IO_H1MC_14_14 = { 12, 90, SIZE_14, "IO-H1MC[14]" };
s_tekst_12 const tekst_IO_H1MC_15_10 = { 12, 90, SIZE_10, "IO-H1MC[15]" };
s_tekst_12 const tekst_IO_H1MC_15_14 = { 12, 90, SIZE_14, "IO-H1MC[15]" };
s_tekst_12 const tekst_IO_H1MC_16_10 = { 12, 90, SIZE_10, "IO-H1MC[16]" };
s_tekst_12 const tekst_IO_H1MC_16_14 = { 12, 90, SIZE_14, "IO-H1MC[16]" };
s_tekst_12 const tekst_IO_H1MC_17_10 = { 12, 90, SIZE_10, "IO-H1MC[17]" };
s_tekst_12 const tekst_IO_H1MC_17_14 = { 12, 90, SIZE_14, "IO-H1MC[17]" };
s_tekst_12 const tekst_IO_H1MC_18_10 = { 12, 90, SIZE_10, "IO-H1MC[18]" };
s_tekst_12 const tekst_IO_H1MC_18_14 = { 12, 90, SIZE_14, "IO-H1MC[18]" };
s_tekst_12 const tekst_IO_H1MC_19_10 = { 12, 90, SIZE_10, "IO-H1MC[19]" };
s_tekst_12 const tekst_IO_H1MC_19_14 = { 12, 90, SIZE_14, "IO-H1MC[19]" };
s_tekst_12 const tekst_IO_H1MC_20_10 = { 12, 90, SIZE_10, "IO-H1MC[20]" };
s_tekst_12 const tekst_IO_H1MC_20_14 = { 12, 90, SIZE_14, "IO-H1MC[20]" };
s_tekst_12 const tekst_IO_H1MC_21_10 = { 12, 90, SIZE_10, "IO-H1MC[21]" };
s_tekst_12 const tekst_IO_H1MC_21_14 = { 12, 90, SIZE_14, "IO-H1MC[21]" };
s_tekst_12 const tekst_IO_H1MC_22_10 = { 12, 90, SIZE_10, "IO-H1MC[22]" };
s_tekst_12 const tekst_IO_H1MC_22_14 = { 12, 90, SIZE_14, "IO-H1MC[22]" };
s_tekst_12 const tekst_IO_H1MC_23_10 = { 12, 90, SIZE_10, "IO-H1MC[23]" };
s_tekst_12 const tekst_IO_H1MC_23_14 = { 12, 90, SIZE_14, "IO-H1MC[23]" };
s_tekst_12 const tekst_IO_H1MC_24_10 = { 12, 90, SIZE_10, "IO-H1MC[24]" };
s_tekst_12 const tekst_IO_H1MC_24_14 = { 12, 90, SIZE_14, "IO-H1MC[24]" };
s_tekst_12 const tekst_IO_H1MC_25_10 = { 12, 90, SIZE_10, "IO-H1MC[25]" };
s_tekst_12 const tekst_IO_H1MC_25_14 = { 12, 90, SIZE_14, "IO-H1MC[25]" };
s_tekst_12 const tekst_IO_H1MC_26_10 = { 12, 90, SIZE_10, "IO-H1MC[26]" };
s_tekst_12 const tekst_IO_H1MC_26_14 = { 12, 90, SIZE_14, "IO-H1MC[26]" };
s_tekst_12 const tekst_IO_H1MC_27_10 = { 12, 90, SIZE_10, "IO-H1MC[27]" };
s_tekst_12 const tekst_IO_H1MC_27_14 = { 12, 90, SIZE_14, "IO-H1MC[27]" };
s_tekst_12 const tekst_IO_H1MC_28_10 = { 12, 90, SIZE_10, "IO-H1MC[28]" };
s_tekst_12 const tekst_IO_H1MC_28_14 = { 12, 90, SIZE_14, "IO-H1MC[28]" };
s_tekst_12 const tekst_IO_H1MC_29_10 = { 12, 90, SIZE_10, "IO-H1MC[29]" };
s_tekst_12 const tekst_IO_H1MC_29_14 = { 12, 90, SIZE_14, "IO-H1MC[29]" };
s_tekst_12 const tekst_IO_H1MC_30_10 = { 12, 90, SIZE_10, "IO-H1MC[30]" };
s_tekst_12 const tekst_IO_H1MC_30_14 = { 12, 90, SIZE_14, "IO-H1MC[30]" };
s_tekst_12 const tekst_IO_H1MC_31_10 = { 12, 90, SIZE_10, "IO-H1MC[31]" };
s_tekst_12 const tekst_IO_H1MC_31_14 = { 12, 90, SIZE_14, "IO-H1MC[31]" };
s_tekst_12 const tekst_IO_H1MC_32_10 = { 12, 90, SIZE_10, "IO-H1MC[32]" };
s_tekst_12 const tekst_IO_H1MC_32_14 = { 12, 90, SIZE_14, "IO-H1MC[32]" };
s_tekst_12 const tekst_IO_H1MC_e_10  = { 12, 89, SIZE_10, "IO-H1MC[-]"  };
s_tekst_12 const tekst_IO_H1MC_e_14  = { 12, 89, SIZE_14, "IO-H1MC[-]"  };

s_tekst_15 const tekst_no_language =  { 15, 150, SIZE_14, "No language" };

void const * const tekst_IO_06_14_10[] = 
{
  &tekst_IO_06_14_1_10, &tekst_IO_06_14_2_10, &tekst_IO_06_14_3_10, &tekst_IO_06_14_4_10, &tekst_IO_06_14_e_10
};
void const * const tekst_IO_06_14_14[] =
{          
  &tekst_IO_06_14_1_14, &tekst_IO_06_14_2_14, &tekst_IO_06_14_3_14, &tekst_IO_06_14_4_14, &tekst_IO_06_14_e_14
};
void const * const tekst_IO_12_06_10[] =
{
  &tekst_IO_12_06_1_10, &tekst_IO_12_06_2_10, &tekst_IO_12_06_3_10, &tekst_IO_12_06_4_10, &tekst_IO_12_06_e_10
};
void const * const tekst_IO_12_06_14[] =
{
  &tekst_IO_12_06_1_14, &tekst_IO_12_06_2_14, &tekst_IO_12_06_3_14, &tekst_IO_12_06_4_14, &tekst_IO_12_06_e_14
};
void const * const tekst_IO_08_09_10[] =
{          
  &tekst_IO_08_09_1_10, &tekst_IO_08_09_2_10, &tekst_IO_08_09_3_10, &tekst_IO_08_09_4_10, 
  &tekst_IO_08_09_5_10, &tekst_IO_08_09_6_10, &tekst_IO_08_09_7_10, &tekst_IO_08_09_8_10, &tekst_IO_08_09_e_10
};
void const * const tekst_IO_08_09_14[] =
{
  &tekst_IO_08_09_1_14, &tekst_IO_08_09_2_14, &tekst_IO_08_09_3_14, &tekst_IO_08_09_4_14,
  &tekst_IO_08_09_5_14, &tekst_IO_08_09_6_14, &tekst_IO_08_09_7_14, &tekst_IO_08_09_8_14, &tekst_IO_08_09_e_14
};
void const * const tekst_IO_05_07_10[] =
{
  &tekst_IO_05_07_1_10,  &tekst_IO_05_07_2_10,  &tekst_IO_05_07_3_10,  &tekst_IO_05_07_4_10, 
  &tekst_IO_05_07_5_10,  &tekst_IO_05_07_6_10,  &tekst_IO_05_07_7_10,  &tekst_IO_05_07_8_10,
  &tekst_IO_05_07_9_10,  &tekst_IO_05_07_10_10, &tekst_IO_05_07_11_10, &tekst_IO_05_07_12_10, 
  &tekst_IO_05_07_13_10, &tekst_IO_05_07_14_10, &tekst_IO_05_07_15_10, &tekst_IO_05_07_16_10, &tekst_IO_05_07_e_10
};
void const * const tekst_IO_05_07_14[] =
{
  &tekst_IO_05_07_1_14,  &tekst_IO_05_07_2_14,  &tekst_IO_05_07_3_14,  &tekst_IO_05_07_4_14,
  &tekst_IO_05_07_5_14,  &tekst_IO_05_07_6_14,  &tekst_IO_05_07_7_14,  &tekst_IO_05_07_8_14,
  &tekst_IO_05_07_9_14,  &tekst_IO_05_07_10_14, &tekst_IO_05_07_11_14, &tekst_IO_05_07_12_14,
  &tekst_IO_05_07_13_14, &tekst_IO_05_07_14_14, &tekst_IO_05_07_15_14, &tekst_IO_05_07_16_14, &tekst_IO_05_07_e_14
};
void const * const tekst_IO_07_07_10[] =
{
  &tekst_IO_07_07_1_10,  &tekst_IO_07_07_2_10,  &tekst_IO_07_07_3_10,  &tekst_IO_07_07_4_10, 
  &tekst_IO_07_07_5_10,  &tekst_IO_07_07_6_10,  &tekst_IO_07_07_7_10,  &tekst_IO_07_07_8_10,
  &tekst_IO_07_07_9_10,  &tekst_IO_07_07_10_10, &tekst_IO_07_07_11_10, &tekst_IO_07_07_12_10, 
  &tekst_IO_07_07_13_10, &tekst_IO_07_07_14_10, &tekst_IO_07_07_15_10, &tekst_IO_07_07_16_10,
  &tekst_IO_07_07_17_10, &tekst_IO_07_07_18_10, &tekst_IO_07_07_19_10, &tekst_IO_07_07_20_10, 
  &tekst_IO_07_07_21_10, &tekst_IO_07_07_22_10, &tekst_IO_07_07_23_10, &tekst_IO_07_07_24_10,
  &tekst_IO_07_07_25_10, &tekst_IO_07_07_26_10, &tekst_IO_07_07_27_10, &tekst_IO_07_07_28_10, 
  &tekst_IO_07_07_29_10, &tekst_IO_07_07_30_10, &tekst_IO_07_07_31_10, &tekst_IO_07_07_32_10,
  &tekst_IO_07_07_e_10
};
void const * const tekst_IO_07_07_14[] =
{
  &tekst_IO_07_07_1_14,  &tekst_IO_07_07_2_14,  &tekst_IO_07_07_3_14,  &tekst_IO_07_07_4_14,
  &tekst_IO_07_07_5_14,  &tekst_IO_07_07_6_14,  &tekst_IO_07_07_7_14,  &tekst_IO_07_07_8_14,
  &tekst_IO_07_07_9_14,  &tekst_IO_07_07_10_14, &tekst_IO_07_07_11_14, &tekst_IO_07_07_12_14,
  &tekst_IO_07_07_13_14, &tekst_IO_07_07_14_14, &tekst_IO_07_07_15_14, &tekst_IO_07_07_16_14,
  &tekst_IO_07_07_17_14, &tekst_IO_07_07_18_14, &tekst_IO_07_07_19_14, &tekst_IO_07_07_20_14, 
  &tekst_IO_07_07_21_14, &tekst_IO_07_07_22_14, &tekst_IO_07_07_23_14, &tekst_IO_07_07_24_14,
  &tekst_IO_07_07_25_14, &tekst_IO_07_07_26_14, &tekst_IO_07_07_27_14, &tekst_IO_07_07_28_14, 
  &tekst_IO_07_07_29_14, &tekst_IO_07_07_30_14, &tekst_IO_07_07_31_14, &tekst_IO_07_07_32_14,
  &tekst_IO_07_07_e_14
};
void const * const tekst_IO_EKU_10[] = 
{
  &tekst_IO_EKU_1_10,  &tekst_IO_EKU_2_10,  &tekst_IO_EKU_3_10,  &tekst_IO_EKU_4_10,  &tekst_IO_EKU_5_10,
  &tekst_IO_EKU_6_10,  &tekst_IO_EKU_7_10,  &tekst_IO_EKU_8_10,  &tekst_IO_EKU_9_10,  &tekst_IO_EKU_10_10,
  &tekst_IO_EKU_11_10, &tekst_IO_EKU_12_10, &tekst_IO_EKU_13_10, &tekst_IO_EKU_14_10, &tekst_IO_EKU_15_10, 
  &tekst_IO_EKU_16_10, &tekst_IO_EKU_e_10
};
void const * const tekst_IO_EKU_14[] =
{ 
  &tekst_IO_EKU_1_14,  &tekst_IO_EKU_2_14,  &tekst_IO_EKU_3_14,  &tekst_IO_EKU_4_14,  &tekst_IO_EKU_5_14,
  &tekst_IO_EKU_6_14,  &tekst_IO_EKU_7_14,  &tekst_IO_EKU_8_14,  &tekst_IO_EKU_9_14,  &tekst_IO_EKU_10_14,
  &tekst_IO_EKU_11_14, &tekst_IO_EKU_12_14, &tekst_IO_EKU_13_14, &tekst_IO_EKU_14_14, &tekst_IO_EKU_15_14, 
  &tekst_IO_EKU_16_14, &tekst_IO_EKU_e_14
};
void const * const tekst_IO_H2MC_10[] = 
{
  &tekst_IO_H2MC_1_10,  &tekst_IO_H2MC_2_10,  &tekst_IO_H2MC_3_10,  &tekst_IO_H2MC_4_10,  &tekst_IO_H2MC_5_10,
  &tekst_IO_H2MC_6_10,  &tekst_IO_H2MC_7_10,  &tekst_IO_H2MC_8_10,  &tekst_IO_H2MC_9_10,  &tekst_IO_H2MC_10_10,
  &tekst_IO_H2MC_11_10, &tekst_IO_H2MC_12_10, &tekst_IO_H2MC_13_10, &tekst_IO_H2MC_14_10, &tekst_IO_H2MC_15_10, 
  &tekst_IO_H2MC_16_10, &tekst_IO_H2MC_17_10, &tekst_IO_H2MC_18_10, &tekst_IO_H2MC_19_10, &tekst_IO_H2MC_20_10, 
  &tekst_IO_H2MC_21_10, &tekst_IO_H2MC_22_10, &tekst_IO_H2MC_23_10, &tekst_IO_H2MC_24_10, &tekst_IO_H2MC_25_10, 
  &tekst_IO_H2MC_26_10, &tekst_IO_H2MC_27_10, &tekst_IO_H2MC_28_10, &tekst_IO_H2MC_29_10, &tekst_IO_H2MC_30_10, 
  &tekst_IO_H2MC_31_10, &tekst_IO_H2MC_32_10, &tekst_IO_H2MC_e_10
};
void const * const tekst_IO_H2MC_14[] =
{ 
  &tekst_IO_H2MC_1_14,  &tekst_IO_H2MC_2_14,  &tekst_IO_H2MC_3_14,  &tekst_IO_H2MC_4_14,  &tekst_IO_H2MC_5_14,
  &tekst_IO_H2MC_6_14,  &tekst_IO_H2MC_7_14,  &tekst_IO_H2MC_8_14,  &tekst_IO_H2MC_9_14,  &tekst_IO_H2MC_10_14,
  &tekst_IO_H2MC_11_14, &tekst_IO_H2MC_12_14, &tekst_IO_H2MC_13_14, &tekst_IO_H2MC_14_14, &tekst_IO_H2MC_15_14, 
  &tekst_IO_H2MC_16_14, &tekst_IO_H2MC_17_14, &tekst_IO_H2MC_18_14, &tekst_IO_H2MC_19_14, &tekst_IO_H2MC_20_14, 
  &tekst_IO_H2MC_21_14, &tekst_IO_H2MC_22_14, &tekst_IO_H2MC_23_14, &tekst_IO_H2MC_24_14, &tekst_IO_H2MC_25_14, 
  &tekst_IO_H2MC_26_14, &tekst_IO_H2MC_27_14, &tekst_IO_H2MC_28_14, &tekst_IO_H2MC_29_14, &tekst_IO_H2MC_30_14, 
  &tekst_IO_H2MC_31_14, &tekst_IO_H2MC_32_14, &tekst_IO_H2MC_e_14
};
void const * const tekst_IO_H1MC_10[] = 
{
  &tekst_IO_H1MC_1_10,  &tekst_IO_H1MC_2_10,  &tekst_IO_H1MC_3_10,  &tekst_IO_H1MC_4_10,  &tekst_IO_H1MC_5_10,
  &tekst_IO_H1MC_6_10,  &tekst_IO_H1MC_7_10,  &tekst_IO_H1MC_8_10,  &tekst_IO_H1MC_9_10,  &tekst_IO_H1MC_10_10,
  &tekst_IO_H1MC_11_10, &tekst_IO_H1MC_12_10, &tekst_IO_H1MC_13_10, &tekst_IO_H1MC_14_10, &tekst_IO_H1MC_15_10, 
  &tekst_IO_H1MC_16_10, &tekst_IO_H1MC_17_10, &tekst_IO_H1MC_18_10, &tekst_IO_H1MC_19_10, &tekst_IO_H1MC_20_10, 
  &tekst_IO_H1MC_21_10, &tekst_IO_H1MC_22_10, &tekst_IO_H1MC_23_10, &tekst_IO_H1MC_24_10, &tekst_IO_H1MC_25_10, 
  &tekst_IO_H1MC_26_10, &tekst_IO_H1MC_27_10, &tekst_IO_H1MC_28_10, &tekst_IO_H1MC_29_10, &tekst_IO_H1MC_30_10, 
  &tekst_IO_H1MC_31_10, &tekst_IO_H1MC_32_10, &tekst_IO_H1MC_e_10
};
void const * const tekst_IO_H1MC_14[] =
{ 
  &tekst_IO_H1MC_1_14,  &tekst_IO_H1MC_2_14,  &tekst_IO_H1MC_3_14,  &tekst_IO_H1MC_4_14,  &tekst_IO_H1MC_5_14,
  &tekst_IO_H1MC_6_14,  &tekst_IO_H1MC_7_14,  &tekst_IO_H1MC_8_14,  &tekst_IO_H1MC_9_14,  &tekst_IO_H1MC_10_14,
  &tekst_IO_H1MC_11_14, &tekst_IO_H1MC_12_14, &tekst_IO_H1MC_13_14, &tekst_IO_H1MC_14_14, &tekst_IO_H1MC_15_14, 
  &tekst_IO_H1MC_16_14, &tekst_IO_H1MC_17_14, &tekst_IO_H1MC_18_14, &tekst_IO_H1MC_19_14, &tekst_IO_H1MC_20_14, 
  &tekst_IO_H1MC_21_14, &tekst_IO_H1MC_22_14, &tekst_IO_H1MC_23_14, &tekst_IO_H1MC_24_14, &tekst_IO_H1MC_25_14, 
  &tekst_IO_H1MC_26_14, &tekst_IO_H1MC_27_14, &tekst_IO_H1MC_28_14, &tekst_IO_H1MC_29_14, &tekst_IO_H1MC_30_14, 
  &tekst_IO_H1MC_31_14, &tekst_IO_H1MC_32_14, &tekst_IO_H1MC_e_14
};

void const * const tekst_groep_10[] =
{
  &tekst.Groep_1_10,  &tekst.Groep_2_10,  &tekst.Groep_3_10,  &tekst.Groep_4_10,  &tekst.Groep_5_10,
  &tekst.Groep_6_10,  &tekst.Groep_7_10,  &tekst.Groep_8_10,  &tekst.Groep_9_10,  &tekst.Groep_10_10,
  &tekst.Groep_11_10, &tekst.Groep_12_10, &tekst.Groep_13_10, &tekst.Groep_14_10, &tekst.Groep_15_10,
  &tekst.Groep_16_10, &tekst.Groep_17_10, &tekst.Groep_18_10, &tekst.Groep_19_10, &tekst.Groep_20_10,
  &tekst.Groep_21_10, &tekst.Groep_22_10, &tekst.Groep_23_10, &tekst.Groep_24_10, &tekst.Groep_25_10,
  &tekst.Groep_26_10, &tekst.Groep_27_10, &tekst.Groep_28_10, &tekst.Groep_29_10, &tekst.Groep_30_10,
  &tekst.Groep_31_10, &tekst.Groep_32_10, &tekst_leeg
};

void const * const tekst_motor_10[] =
{
  &tekst.Motor_1_10,   &tekst.Motor_2_10,   &tekst.Motor_3_10,   &tekst.Motor_4_10,   &tekst.Motor_5_10,
  &tekst.Motor_6_10,   &tekst.Motor_7_10,   &tekst.Motor_8_10,   &tekst.Motor_9_10,   &tekst.Motor_10_10,
  &tekst.Motor_11_10,  &tekst.Motor_12_10,  &tekst.Motor_13_10,  &tekst.Motor_14_10,  &tekst.Motor_15_10,
  &tekst.Motor_16_10,  &tekst.Motor_17_10,  &tekst.Motor_18_10,  &tekst.Motor_19_10,  &tekst.Motor_20_10,
  &tekst.Motor_21_10,  &tekst.Motor_22_10,  &tekst.Motor_23_10,  &tekst.Motor_24_10,  &tekst.Motor_25_10,
  &tekst.Motor_26_10,  &tekst.Motor_27_10,  &tekst.Motor_28_10,  &tekst.Motor_29_10,  &tekst.Motor_30_10,
  &tekst.Motor_31_10,  &tekst.Motor_32_10,  &tekst.Motor_33_10,  &tekst.Motor_34_10,  &tekst.Motor_35_10,
  &tekst.Motor_36_10,  &tekst.Motor_37_10,  &tekst.Motor_38_10,  &tekst.Motor_39_10,  &tekst.Motor_40_10,
  &tekst.Motor_41_10,  &tekst.Motor_42_10,  &tekst.Motor_43_10,  &tekst.Motor_44_10,  &tekst.Motor_45_10,
  &tekst.Motor_46_10,  &tekst.Motor_47_10,  &tekst.Motor_48_10,  &tekst.Motor_49_10,  &tekst.Motor_50_10,
  &tekst.Motor_51_10,  &tekst.Motor_52_10,  &tekst.Motor_53_10,  &tekst.Motor_54_10,  &tekst.Motor_55_10,
  &tekst.Motor_56_10,  &tekst.Motor_57_10,  &tekst.Motor_58_10,  &tekst.Motor_59_10,  &tekst.Motor_60_10,
  &tekst.Motor_61_10,  &tekst.Motor_62_10,  &tekst.Motor_63_10,  &tekst.Motor_64_10,  &tekst_leeg
};

void const * const tekst_groep_14[] =
{
  &tekst.Groep_1_14,  &tekst.Groep_2_14,  &tekst.Groep_3_14,  &tekst.Groep_4_14,  &tekst.Groep_5_14,
  &tekst.Groep_6_14,  &tekst.Groep_7_14,  &tekst.Groep_8_14,  &tekst.Groep_9_14,  &tekst.Groep_10_14,
  &tekst.Groep_11_14, &tekst.Groep_12_14, &tekst.Groep_13_14, &tekst.Groep_14_14, &tekst.Groep_15_14,
  &tekst.Groep_16_14, &tekst.Groep_17_14, &tekst.Groep_18_14, &tekst.Groep_19_14, &tekst.Groep_20_14,
  &tekst.Groep_21_14, &tekst.Groep_22_14, &tekst.Groep_23_14, &tekst.Groep_24_14, &tekst.Groep_25_14,
  &tekst.Groep_26_14, &tekst.Groep_27_14, &tekst.Groep_28_14, &tekst.Groep_29_14, &tekst.Groep_30_14,
  &tekst.Groep_31_14, &tekst.Groep_32_14, &tekst_leeg
};
void const * const tekst_inst_groep_14[] =
{
  &tekst_inst.Groep_1_14,  &tekst_inst.Groep_2_14,  &tekst_inst.Groep_3_14,  &tekst_inst.Groep_4_14,  &tekst_inst.Groep_5_14,
  &tekst_inst.Groep_6_14,  &tekst_inst.Groep_7_14,  &tekst_inst.Groep_8_14,  &tekst_inst.Groep_9_14,  &tekst_inst.Groep_10_14,
  &tekst_inst.Groep_11_14, &tekst_inst.Groep_12_14, &tekst_inst.Groep_13_14, &tekst_inst.Groep_14_14, &tekst_inst.Groep_15_14,
  &tekst_inst.Groep_16_14, &tekst_inst.Groep_17_14, &tekst_inst.Groep_18_14, &tekst_inst.Groep_19_14, &tekst_inst.Groep_20_14,
  &tekst_inst.Groep_21_14, &tekst_inst.Groep_22_14, &tekst_inst.Groep_23_14, &tekst_inst.Groep_24_14, &tekst_inst.Groep_25_14,
  &tekst_inst.Groep_26_14, &tekst_inst.Groep_27_14, &tekst_inst.Groep_28_14, &tekst_inst.Groep_29_14, &tekst_inst.Groep_30_14,
  &tekst_inst.Groep_31_14, &tekst_inst.Groep_32_14, &tekst_leeg
};

void const * const tekst_ext_stop_open_dicht_10[3] = { &tekst.EXT_STOP_10, &tekst.EXT_OPEN_10, &tekst.EXT_DICHT_10 };
void const * const tekst_stop_open_dicht_10[3] = { &tekst.STOP_10, &tekst.OPEN_10, &tekst.DICHT_10 };
void const * const tekst_auto_hand_uit_10[3]   = { &tekst.AUTO_10, &tekst.HAND_10, &tekst.UIT_10   };
void const * const tekst_auto_hand_uit_7[3]    = { &tekst.AUTO_7,  &tekst.HAND_7,  &tekst.UIT_7    };

s_tekst_6 const tekst9600   = { 6, 110, SIZE_14, "9.6" };
s_tekst_6 const tekst19200  = { 6, 110, SIZE_14, "19.2" };
s_tekst_6 const tekst38400  = { 6, 110, SIZE_14, "38.4" };
s_tekst_6 const tekst57600  = { 6, 110, SIZE_14, "57.6" };
s_tekst_6 const tekst115200 = { 6, 110, SIZE_14, "115.2" };

void const * const tekst_aan_uit_20[2]               = { &tekst.UIT_20, &tekst.AAN_20 };
void const * const tekst_inst_aan_uit_14[3]          = { &tekst_inst.UIT_14, &tekst_inst.AAN_14, &tekst_leeg };
void const * const tekst_leeg_dp_10[2]               = { &tekst_leeg, &tekst_dp_10 };
void const * const tekst_leeg_dp_14[2]               = { &tekst_leeg, &tekst_dp_14 };
void const * const tekst_inst_rel_abs_14[2]          = { &tekst_inst.Relatief_14, &tekst_inst.Absoluut_14 };
void const * const tekst_inst_geen_even_oneven_14[3] = { &tekst_inst.Geen_14, &tekst_inst.Even_14, &tekst_inst.Oneven_14 };
void const * const tekst_inst_baudrate_14[5]         = { &tekst9600, &tekst19200, &tekst38400, &tekst57600, &tekst115200 };

void const * const tekst_digitaal_analoog_14[]                = { &tekst_inst.Digitaal_14, &tekst_inst.Analoog_14 };
void const * const tekst_digitaal_analoog_ebmBus_modBus_14[]  = { &tekst_inst.Digitaal_14, &tekst_inst.Analoog_14, &tekst_ebmBus_14,  &tekst_ECblue_Premium_14, &tekst_ebm_Modbus_14, &tekst_ECblue_Modbus_14, &tekst_Rosenberg_14, &tekst_Climafan_14, &tekst_Rosenberg_Gen3_14, &tekst_Nicotra_Gebhardt_14 };
void const * const tekst_Digitaal_Analoog_CANopen_BACnet_14[] = { &tekst_inst.Digitaal_14, &tekst_inst.Analoog_14, &tekst_CANopen_14, &tekst_BACnet_14 };

s_disp_tekst const disp_ok_str = { Disp_Draw_Tekst_L, 85+3, 65+19, &tekst_ok_10 };
s_disp_tekst_add const disp_wissen_str = { Disp_Draw_Tekst_Add_L, &tekst.Wissen_10 };
s_disp_tekst_add const disp_wijzigen_str = { Disp_Draw_Tekst_Add_L, &tekst_inst.Wijzigen_10 };
s_disp_tekst const disp_zeker_weten_str =      { Disp_Draw_Tekst_L, 85+3, 40+19, &tekst.Zeker_weten_7 };
s_disp_tekst const disp_zeker_weten_inst_str = { Disp_Draw_Tekst_L, 85+3, 40+19, &tekst_inst.Zeker_weten_7 };
s_disp_tekst_add const disp_verwijderen_inst_str = { Disp_Draw_Tekst_Add_L, &tekst_inst.Verwijderen_14 };
s_disp_tekst_add const disp_instellen_sensor_inst_str = { Disp_Draw_Tekst_Add_L, &tekst_inst.Instellen_sensor_14 };
s_disp_tekst_add const disp_ijken_sensor_inst_str     = { Disp_Draw_Tekst_Add_L, &tekst_inst.IJken_sensor_14 };
s_disp_tekst_add const disp_ijken_minimum_inst_str    = { Disp_Draw_Tekst_Add_L, &tekst_inst.IJken_Minimum_14 };
s_disp_tekst_add const disp_ijken_maximum_inst_str    = { Disp_Draw_Tekst_Add_L, &tekst_inst.IJken_Maximum_14 };
s_disp_tekst_add const disp_instellen_inst_str        = { Disp_Draw_Tekst_Add_L, &tekst_inst.Instellen_14 };
s_disp_tekst_add const disp_ijken_inst_str            = { Disp_Draw_Tekst_Add_L, &tekst_inst.IJken_14 };
s_disp_tekst_add const disp_overnemen_inst_str        = { Disp_Draw_Tekst_Add_L, &tekst_inst.Overnemen_10 };

s_disp_tekst_add const disp_dp_7_L =  { Disp_Draw_Tekst_Add_L,  &tekst_dp_7 };
s_disp_tekst_add const disp_dp_10_L = { Disp_Draw_Tekst_Add_L, &tekst_dp_10 };
s_disp_tekst_add const disp_dp_14_L = { Disp_Draw_Tekst_Add_L, &tekst_dp_14 };
s_disp_tekst_add const disp_is_teken_7_L  = { Disp_Draw_Tekst_Add_L, &tekst_is_7 };
s_disp_tekst_add const disp_ja_str      = { Disp_Draw_Tekst_Add_L, &tekst.JA_10 };
s_disp_tekst_add const disp_ja_inst_str = { Disp_Draw_Tekst_Add_L, &tekst_inst.JA_10 };
s_disp_tekst  const disp_bevestig_is_teken = { Disp_Draw_Tekst_L, 103+3, 67+19, &tekst_is_10 };
s_disp_tekst_add const disp_is_teken_10_L = { Disp_Draw_Tekst_Add_L, &tekst_is_10 };
s_disp_tekst_add const disp_is_teken_14_L = { Disp_Draw_Tekst_Add_L, &tekst_is_14 };
s_disp_tekst_add const disp_space_7_L =  { Disp_Draw_Tekst_Add_L, &tekst_space_7 };
s_disp_tekst_add const disp_space_10_L = { Disp_Draw_Tekst_Add_L, &tekst_space_10 };
s_disp_tekst_add const disp_space_14_L = { Disp_Draw_Tekst_Add_L, &tekst_space_14 };
s_disp_tekst_add const disp_min_teken_10_R = { Disp_Draw_Tekst_Add_R, &tekst_min_10 };
s_disp_tekst_add const disp_min_teken_14_R = { Disp_Draw_Tekst_Add_R, &tekst_min_14 };
s_disp_tekst_add const disp_min_teken_14_L = { Disp_Draw_Tekst_Add_L, &tekst_min_14 };
s_disp_tekst_add const disp_ronde_openings_haak_7_L   = { Disp_Draw_Tekst_Add_L, &tekst_ronde_openings_haak_7   };
s_disp_tekst_add const disp_ronde_sluit_haak_7_L      = { Disp_Draw_Tekst_Add_L, &tekst_ronde_sluit_haak_7      };
s_disp_tekst_add const disp_ronde_openings_haak_10_L  = { Disp_Draw_Tekst_Add_L, &tekst_ronde_openings_haak_10  };
s_disp_tekst_add const disp_ronde_sluit_haak_10_L     = { Disp_Draw_Tekst_Add_L, &tekst_ronde_sluit_haak_10     };
s_disp_tekst_add const disp_ronde_openings_haak_14_L  = { Disp_Draw_Tekst_Add_L, &tekst_ronde_openings_haak_14  };
s_disp_tekst_add const disp_ronde_sluit_haak_14_L     = { Disp_Draw_Tekst_Add_L, &tekst_ronde_sluit_haak_14     };
s_disp_tekst_add const disp_rechte_openings_haak_10_L = { Disp_Draw_Tekst_Add_L, &tekst_rechte_openings_haak_10 };
s_disp_tekst_add const disp_rechte_sluit_haak_10_L    = { Disp_Draw_Tekst_Add_L, &tekst_rechte_sluit_haak_10    };
s_disp_tekst_add const disp_rechte_openings_haak_14_L = { Disp_Draw_Tekst_Add_L, &tekst_rechte_openings_haak_14 };
s_disp_tekst_add const disp_rechte_sluit_haak_14_L    = { Disp_Draw_Tekst_Add_L, &tekst_rechte_sluit_haak_14    };

s_disp_tekst_add const disp_dp_7_R = { Disp_Draw_Tekst_Add_R, &tekst_dp_7 };
s_disp_tekst_add const disp_punt_7_R = { Disp_Draw_Tekst_Add_R, &tekst_punt_7 };
s_disp_tekst_add const disp_min_teken_7_R = { Disp_Draw_Tekst_Add_R, &tekst_min_7 };
s_disp_tekst_add const disp_space_10_R = { Disp_Draw_Tekst_Add_R, &tekst_space_10 };


//*****************************************************************************
void Default_Strings(void)
{
  tekst      = tekst_engels;
  tekst_inst = tekst_inst_engels;
}

void Copy_Strings(void)
{
  switch (opt_alg.taalkeuze)
  {
    default:
    case ENGELS:     tekst = tekst_engels;     tekst_inst = tekst_inst_engels;     break;
    case NEDERLANDS: tekst = tekst_nederlands; tekst_inst = tekst_inst_nederlands; break;
    case DUITS:      tekst = tekst_duits;      tekst_inst = tekst_inst_duits;      break;
    case SPAANS:     tekst = tekst_spaans;     tekst_inst = tekst_inst_spaans;     break;
    case USER:       tekst = tekst_engels;
                     tekst_inst = tekst_inst_engels;
                     EEP_taal_read_switch = 1;
                     break; // terug lezen eigen taal uit eeprom
  }
//  Copy_Voerteksten();
}