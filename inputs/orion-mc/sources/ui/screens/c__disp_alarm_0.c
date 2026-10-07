// C__DISP_ALARM_0.C

#include <stdio.h> 
#include <string.h> 

#include "ch_define.h"
#include "ch_alarm.h"
#include "ch_alg.h"
#include "ch_const.h"
#include "ch_disp.h"
#include "ch_disp_func.h"
#include "ch_lcd_10.h"
#include "ch_lcd_bitmap.h"
#include "ch_lcd_hardware.h"
#include "ch_lcd_pixel.h"
#include "ch_string.h"
#include "ch_disp_alarm_0.h"

static void Disp_Control_Alarm_Actueel(void);
                              
s_alarm_disp alarm_actueel;
s_alarm_disp laatste_alarm_actueel = { GEEN_AL,0,0,0,0,0,0,0,0,0 }; // wordt alleen voor actueel alarm gebruikt

static s_disp_agri_header const disp_header = { Disp_Draw_Agri_Header, FN4, EMPTY, &tekst.Alarm_10, HK_GEEN, 0, 1};
static s_disp_func   const disp_control =       { Disp_Control_Func, Disp_Control_Alarm_Actueel };
static s_disp_bitmap_option_on const disp_time_on_alarm = { Disp_Draw_Bitmap_Option_On, 37+3, REGEL_4+10, &ico_alarm, LONG, &alarm_actueel.on};     // i.v.m. COMPUTER_AL
static s_disp_bitmap_option_on const disp_time_on_bmp =   { Disp_Draw_Bitmap_Option_On, 58+3, REGEL_4+10, &ico_switch_on, LONG, &alarm_actueel.on};   // i.v.m. COMPUTER_AL
static s_disp_time             const disp_time_on =       { Disp_Draw_Time,             75+3, REGEL_4+19, SIZE_10, &alarm_actueel.on };

//*****************************************************************************
static s_disp_bitmap const disp_alarm = { Disp_Draw_Bitmap, 12, 22, &ico_alarm };

//*****************************************************************************
static s_disp_tekst const disp_alarm_str                = { Disp_Draw_Tekst_L, 37, REGEL_1, &tekst.Alarm_10 };
static s_disp_tekst const disp_waarschuwing_str         = { Disp_Draw_Tekst_L, 37, REGEL_1, &tekst.Waarschuwing_10 };
static s_disp_tekst const disp_alarm_systeem_str        = { Disp_Draw_Tekst_L, 37, REGEL_1, &tekst.Alarm_Systeem_10 };
static s_disp_tekst const disp_waarschuwing_systeem_str = { Disp_Draw_Tekst_L, 37, REGEL_1, &tekst.Waarschuwing_Syst_10 };
static s_disp_value_add const disp_add_nr_val =           { Disp_Draw_Value_Add, (SIZE_10 | LINKS), INT, 0, &alarm_actueel.value };

//*****************************************************************************
static s_disp_bitmap const disp_systeem = { Disp_Draw_Bitmap, 3, 3, &ico_systeem };
static s_disp_bitmap const disp_motor_communication_al_ico  = { Disp_Draw_Bitmap, 3, 3, &motor_communication_al_ico  };
static s_disp_bitmap const disp_vent_communication_al_ico   = { Disp_Draw_Bitmap, 3, 3, &vent_communication_al_ico   };
static s_disp_bitmap const disp_klep_communication_al_ico   = { Disp_Draw_Bitmap, 3, 3, &klep_communication_al_ico   };
static s_disp_bitmap const disp_sensor_communication_al_ico = { Disp_Draw_Bitmap, 3, 3, &sensor_communication_al_ico };

static s_disp_tekst const disp_geen_alarm_str                 = { Disp_Draw_Tekst_L, 37, REGEL_2, &tekst.Geen_Actief_Alarm_10 };
static s_disp_tekst const disp_onbekend_alarm_str             = { Disp_Draw_Tekst_L, 37, REGEL_2, &tekst.Onbekend_Alarm_10 };
static s_disp_tekst const disp_opties_str                     = { Disp_Draw_Tekst_L, 37, REGEL_2, &tekst.Opties_10 };
static s_disp_tekst const disp_gewist_str                     = { Disp_Draw_Tekst_L, 37, REGEL_3, &tekst.Gewist_10 };
static s_disp_tekst const disp_instellingen_str               = { Disp_Draw_Tekst_L, 37, REGEL_2, &tekst.Instellingen_10 };
static s_disp_tekst const disp_minimum_maximum_str            = { Disp_Draw_Tekst_L, 37, REGEL_2, &tekst.Minimum_Maximum_10 };
static s_disp_tekst const disp_syst_al_I2C0_str               = { Disp_Draw_Tekst_L, 37, REGEL_2, &tekst.I2C_10 };
static s_disp_tekst const disp_syst_al_EEP_str                = { Disp_Draw_Tekst_L, 37, REGEL_2, &tekst.EEPROM_10 };
static s_disp_tekst const disp_syst_al_EEP_taal_str           = { Disp_Draw_Tekst_L, 37, REGEL_2, &tekst.EEPROM_taal_10 };
static s_disp_tekst const disp_syst_al_geen_alarm_contact_str = { Disp_Draw_Tekst_L, 37, REGEL_2, &tekst.Geen_Alarm_Contact_10 };
static s_disp_tekst const disp_syst_al_RTC_str                = { Disp_Draw_Tekst_L, 37, REGEL_2, &tekst.RTC_10 };
static s_disp_tekst const disp_syst_al_TIMER_1ms_str          = { Disp_Draw_Tekst_L, 37, REGEL_2, &tekst.TIMER_10 };
static s_disp_tekst const disp_syst_al_HTRAP_str              = { Disp_Draw_Tekst_L, 37, REGEL_2, &tekst.HTRAP_10 };
static s_disp_tekst const disp_syst_al_PLL_str                = { Disp_Draw_Tekst_L, 37, REGEL_2, &tekst.PLL_10 };
static s_disp_tekst const disp_alarmen_str                    = { Disp_Draw_Tekst_L, 37, REGEL_2, &tekst.Alarmen_10 };
static s_disp_tekst const disp_syst_al_opstart_str            = { Disp_Draw_Tekst_L, 37, REGEL_2, &tekst.Opnieuw_Gestart_10 };
static s_disp_tekst const disp_print_str                      = { Disp_Draw_Tekst_L, 37, REGEL_2, &tekst.Print_10 }; 

static s_disp_tekst_array_add const disp_IO_06_14_str  = { Disp_Draw_Tekst_Array_Add_L, &tekst_IO_06_14_10,  INT, &alarm_actueel.index, IO_06_14_MAX  };
static s_disp_tekst_array_add const disp_IO_12_06_str  = { Disp_Draw_Tekst_Array_Add_L, &tekst_IO_12_06_10,  INT, &alarm_actueel.index, IO_12_06_MAX  };
static s_disp_tekst_array_add const disp_IO_08_09_str  = { Disp_Draw_Tekst_Array_Add_L, &tekst_IO_08_09_10,  INT, &alarm_actueel.index, IO_08_09_MAX  };
static s_disp_tekst_array_add const disp_IO_EKU_str    = { Disp_Draw_Tekst_Array_Add_L, &tekst_IO_EKU_10,    INT, &alarm_actueel.index, IO_EKU_MAX    };
static s_disp_tekst_array_add const disp_IO_H2MC_str   = { Disp_Draw_Tekst_Array_Add_L, &tekst_IO_H2MC_10,   INT, &alarm_actueel.index, IO_H2MC_MAX   };
static s_disp_tekst_array_add const disp_IO_H1MC_str   = { Disp_Draw_Tekst_Array_Add_L, &tekst_IO_H1MC_10,   INT, &alarm_actueel.index, IO_H1MC_MAX   };
static s_disp_tekst_array_add const disp_IO_05_07_str  = { Disp_Draw_Tekst_Array_Add_L, &tekst_IO_05_07_10,  INT, &alarm_actueel.index, IO_05_07_MAX  };
static s_disp_tekst_array_add const disp_IO_07_07_str  = { Disp_Draw_Tekst_Array_Add_L, &tekst_IO_07_07_10,  INT, &alarm_actueel.index, IO_07_07_MAX  };

static s_disp_tekst const disp_niet_gevonden_str   = { Disp_Draw_Tekst_L, 37, REGEL_3, &tekst.Niet_Gevonden_10 }; 
static s_disp_tekst const disp_adc_str             = { Disp_Draw_Tekst_L, 37, REGEL_3, &tekst.ADC_10 }; 
static s_disp_tekst const disp_analoge_ingang_str  = { Disp_Draw_Tekst_L, 37, REGEL_3, &tekst.Analoge_Ingang_10 };
static s_disp_tekst const disp_externe_24v_str     = { Disp_Draw_Tekst_L, 37, REGEL_3, &tekst.Externe_24V_10 }; 
static s_disp_tekst const disp_version_error_str =          { Disp_Draw_Tekst_L, 37, REGEL_3, &tekst.Version_Error_10 };

static s_disp_tekst const disp_orion_uit_str              = { Disp_Draw_Tekst_L, 37, REGEL_2, &tekst.Orion_Uitgeschakeld_10 };
static s_disp_tekst const disp_orion_on_str               = { Disp_Draw_Tekst_L, 37, REGEL_2, &tekst.Geen_Minimum_Alarm_10 };
static s_disp_tekst const disp_opties_en_instellingen_str = { Disp_Draw_Tekst_L, 37, REGEL_2, &tekst.Opties_En_Instellingen_10 };
static s_disp_tekst const disp_niet_terug_gezet           = { Disp_Draw_Tekst_L, 37, REGEL_3, &tekst.Niet_Terug_Gezet_10 };
static s_disp_tekst const disp_terug_gezet_str            = { Disp_Draw_Tekst_L, 37, REGEL_3, &tekst.Terug_Gezet_10 };

static s_disp_tekst const disp_computer_str        = { Disp_Draw_Tekst_L, 37, REGEL_2, &tekst.Computer_10 };
#ifdef CAN_BACKBONE_PC_WARNING
static s_disp_tekst const disp_can_pc_1_offline_str = { Disp_Draw_Tekst_L, 37, REGEL_2, &tekst.CAN_PC_1_Offline_10 };
static s_disp_tekst const disp_can_pc_2_offline_str = { Disp_Draw_Tekst_L, 37, REGEL_2, &tekst.CAN_PC_2_Offline_10 };
static s_disp_tekst const disp_can_pc_3_offline_str = { Disp_Draw_Tekst_L, 37, REGEL_2, &tekst.CAN_PC_3_Offline_10 };
#else // CAN_BACKBONE_PC_WARNING
static s_disp_tekst const disp_can_pc_str          = { Disp_Draw_Tekst_L, 37, REGEL_2, &tekst.CAN_RS232_10 };
#endif // CAN_BACKBONE_PC_WARNING
static s_disp_value_add const disp_computer_nr_val = { Disp_Draw_Value_Add, (SIZE_10 | LINKS), INT, 0, &alarm_actueel.index };
static s_disp_tekst_add const disp_add_alarm_str   = { Disp_Draw_Tekst_Add_L, &tekst.Alarm_Computer_10 };
static s_disp_tekst_add const disp_add_offline_str = { Disp_Draw_Tekst_Add_L, &tekst.Offline_10 };

//-------------------------------------------------------------------------------------------------------------------------------
static s_disp_tekst_array const disp_groep_str              = { Disp_Draw_Tekst_Array_L, 37, REGEL_2, &tekst_groep_10, UCHAR, &alarm_actueel.index, MAX_GROUP };
static s_disp_tekst_array const disp_motor_1_str            = { Disp_Draw_Tekst_Array_L, 37, REGEL_1, &tekst_motor_10, UCHAR, &alarm_actueel.index, MAX_MOTOR };
static s_disp_tekst_array const disp_motor_2_str            = { Disp_Draw_Tekst_Array_L, 37, REGEL_2, &tekst_motor_10, UCHAR, &alarm_actueel.index, MAX_MOTOR };
static s_disp_tekst       const disp_vent_str               = { Disp_Draw_Tekst_L,       37, REGEL_2, &tekst.Ventilator_10          };
static s_disp_tekst       const disp_klep_str               = { Disp_Draw_Tekst_L,       37, REGEL_2, &tekst.Klep_10                };
static s_disp_tekst       const disp_sensor_str             = { Disp_Draw_Tekst_L,       37, REGEL_2, &tekst.Sensor_10              };
static s_disp_tekst       const disp_luchtmengkastgroep_str = { Disp_Draw_Tekst_L,       37, REGEL_2, &tekst.Luchtmengkast_groep_10 };
static s_disp_tekst       const disp_luchtmengkast_1_str    = { Disp_Draw_Tekst_L,       37, REGEL_1, &tekst.Luchtmengkast_10       };
static s_disp_tekst       const disp_luchtmengkast_2_str    = { Disp_Draw_Tekst_L,       37, REGEL_2, &tekst.Luchtmengkast_10       };

static s_disp_bitmap_array const disp_raam_doek_al = { Disp_Draw_Bitmap_Array, 5, 5, ico_type_array, UCHAR, &opt_app.Motorgroup[0].Type, 4 };

static s_disp_bitmap const disp_motor_al         = { Disp_Draw_Bitmap, 5, 5, &motor_ico         };
static s_disp_bitmap const disp_vent_al          = { Disp_Draw_Bitmap, 5, 5, &vent_ico_17x17    };
static s_disp_bitmap const disp_klep_al          = { Disp_Draw_Bitmap, 5, 5, &ico_IO_klep       };
static s_disp_bitmap const disp_luchtmengkast_al = { Disp_Draw_Bitmap, 5, 3, &luchtmengkast_ico };

static s_disp_tekst_option_on     const disp_slave_str    = { Disp_Draw_Tekst_L_Option_On, 37, REGEL_2, &tekst.Slave_10, UCHAR, &alarm_actueel.value };
static s_disp_value_add_option_on const disp_slave_nr_val = { Disp_Draw_Value_Add_Option_On, (SIZE_10 | LINKS), UCHAR, 0, &alarm_actueel.value, UCHAR, &alarm_actueel.value };

static s_disp_tekst const disp_handbediening_str = { Disp_Draw_Tekst_L, 37, REGEL_3, &tekst.Handbediening_10 };

static s_disp_tekst_option_on const disp_emergency_switch_2_str        = { Disp_Draw_Tekst_L_Option_Off, 37, REGEL_2, &tekst.Emergency_switch_10,        UCHAR, &alarm_actueel.value };
static s_disp_tekst_option_on const disp_emergency_switch_3_str        = { Disp_Draw_Tekst_L_Option_On,  37, REGEL_3, &tekst.Emergency_switch_10,        UCHAR, &alarm_actueel.value };
static s_disp_tekst_option_on const disp_thermal_failure_close_2_str   = { Disp_Draw_Tekst_L_Option_Off, 37, REGEL_2, &tekst.Thermal_failure_close_10,   UCHAR, &alarm_actueel.value };
static s_disp_tekst_option_on const disp_thermal_failure_close_3_str   = { Disp_Draw_Tekst_L_Option_On,  37, REGEL_3, &tekst.Thermal_failure_close_10,   UCHAR, &alarm_actueel.value };
static s_disp_tekst_option_on const disp_thermal_failure_open_2_str    = { Disp_Draw_Tekst_L_Option_Off, 37, REGEL_2, &tekst.Thermal_failure_open_10,    UCHAR, &alarm_actueel.value };
static s_disp_tekst_option_on const disp_thermal_failure_open_3_str    = { Disp_Draw_Tekst_L_Option_On,  37, REGEL_3, &tekst.Thermal_failure_open_10,    UCHAR, &alarm_actueel.value };
static s_disp_tekst_option_on const disp_break_input_2_str             = { Disp_Draw_Tekst_L_Option_Off, 37, REGEL_2, &tekst.Break_input_10,             UCHAR, &alarm_actueel.value };
static s_disp_tekst_option_on const disp_break_input_3_str             = { Disp_Draw_Tekst_L_Option_On,  37, REGEL_3, &tekst.Break_input_10,             UCHAR, &alarm_actueel.value };
static s_disp_tekst_option_on const disp_speed_to_low_2_str            = { Disp_Draw_Tekst_L_Option_Off, 37, REGEL_2, &tekst.Speed_to_low_10,            UCHAR, &alarm_actueel.value };
static s_disp_tekst_option_on const disp_speed_to_low_3_str            = { Disp_Draw_Tekst_L_Option_On,  37, REGEL_3, &tekst.Speed_to_low_10,            UCHAR, &alarm_actueel.value };
static s_disp_tekst_option_on const disp_encoder_failure_2_str         = { Disp_Draw_Tekst_L_Option_Off, 37, REGEL_2, &tekst.Encoder_failure_10,         UCHAR, &alarm_actueel.value };
static s_disp_tekst_option_on const disp_encoder_failure_3_str         = { Disp_Draw_Tekst_L_Option_On,  37, REGEL_3, &tekst.Encoder_failure_10,         UCHAR, &alarm_actueel.value };
static s_disp_tekst_option_on const disp_encoder_failure_A_2_str       = { Disp_Draw_Tekst_L_Option_Off, 37, REGEL_2, &tekst.Encoder_failure_A_10,       UCHAR, &alarm_actueel.value };
static s_disp_tekst_option_on const disp_encoder_failure_A_3_str       = { Disp_Draw_Tekst_L_Option_On,  37, REGEL_3, &tekst.Encoder_failure_A_10,       UCHAR, &alarm_actueel.value };
static s_disp_tekst_option_on const disp_encoder_failure_B_2_str       = { Disp_Draw_Tekst_L_Option_Off, 37, REGEL_2, &tekst.Encoder_failure_B_10,       UCHAR, &alarm_actueel.value };
static s_disp_tekst_option_on const disp_encoder_failure_B_3_str       = { Disp_Draw_Tekst_L_Option_On,  37, REGEL_3, &tekst.Encoder_failure_B_10,       UCHAR, &alarm_actueel.value };
static s_disp_tekst_option_on const disp_no_feedback_2_str             = { Disp_Draw_Tekst_L_Option_Off, 37, REGEL_2, &tekst.No_feedback_10,             UCHAR, &alarm_actueel.value };
static s_disp_tekst_option_on const disp_no_feedback_3_str             = { Disp_Draw_Tekst_L_Option_On,  37, REGEL_3, &tekst.No_feedback_10,             UCHAR, &alarm_actueel.value };
static s_disp_tekst_option_on const disp_encoder_interference_2_str    = { Disp_Draw_Tekst_L_Option_Off, 37, REGEL_2, &tekst.Encoder_interference_10,    UCHAR, &alarm_actueel.value };
static s_disp_tekst_option_on const disp_encoder_interference_3_str    = { Disp_Draw_Tekst_L_Option_On,  37, REGEL_3, &tekst.Encoder_interference_10,    UCHAR, &alarm_actueel.value };
static s_disp_tekst_option_on const disp_gelijkloop_beveiliging_2_str  = { Disp_Draw_Tekst_L_Option_Off, 37, REGEL_2, &tekst.Gelijkloop_beveiliging_10,  UCHAR, &alarm_actueel.value };
static s_disp_tekst_option_on const disp_gelijkloop_beveiliging_3_str  = { Disp_Draw_Tekst_L_Option_On,  37, REGEL_3, &tekst.Gelijkloop_beveiliging_10,  UCHAR, &alarm_actueel.value };
static s_disp_tekst_option_on const disp_limitswitch_not_reached_2_str = { Disp_Draw_Tekst_L_Option_Off, 37, REGEL_2, &tekst.Limitswitch_not_reached_10, UCHAR, &alarm_actueel.value };
static s_disp_tekst_option_on const disp_limitswitch_not_reached_3_str = { Disp_Draw_Tekst_L_Option_On,  37, REGEL_3, &tekst.Limitswitch_not_reached_10, UCHAR, &alarm_actueel.value };
static s_disp_tekst_option_on const disp_not_enough_pulses_2_str       = { Disp_Draw_Tekst_L_Option_Off, 37, REGEL_2, &tekst.Not_enough_pulses_10,       UCHAR, &alarm_actueel.value };
static s_disp_tekst_option_on const disp_not_enough_pulses_3_str       = { Disp_Draw_Tekst_L_Option_On,  37, REGEL_3, &tekst.Not_enough_pulses_10,       UCHAR, &alarm_actueel.value };
static s_disp_tekst_option_on const disp_pulses_to_fast_2_str          = { Disp_Draw_Tekst_L_Option_Off, 37, REGEL_2, &tekst.Pulses_to_fast_10,          UCHAR, &alarm_actueel.value };
static s_disp_tekst_option_on const disp_pulses_to_fast_3_str          = { Disp_Draw_Tekst_L_Option_On,  37, REGEL_3, &tekst.Pulses_to_fast_10,          UCHAR, &alarm_actueel.value };
static s_disp_tekst_option_on const disp_directions_not_defined_2_str  = { Disp_Draw_Tekst_L_Option_Off, 37, REGEL_2, &tekst.Direction_not_defined_10,   UCHAR, &alarm_actueel.value };
static s_disp_tekst_option_on const disp_directions_not_defined_3_str  = { Disp_Draw_Tekst_L_Option_On,  37, REGEL_3, &tekst.Direction_not_defined_10,   UCHAR, &alarm_actueel.value };
static s_disp_tekst_option_on const disp_onbekend_alarm_2_str          = { Disp_Draw_Tekst_L_Option_Off, 37, REGEL_2, &tekst.Onbekend_Alarm_10,          UCHAR, &alarm_actueel.value };
static s_disp_tekst_option_on const disp_onbekend_alarm_3_str          = { Disp_Draw_Tekst_L_Option_On,  37, REGEL_3, &tekst.Onbekend_Alarm_10,          UCHAR, &alarm_actueel.value };
static s_disp_tekst_option_on const disp_motor_not_running_2_str       = { Disp_Draw_Tekst_L_Option_Off, 37, REGEL_2, &tekst.Motor_not_running_10,       UCHAR, &alarm_actueel.value };
static s_disp_tekst_option_on const disp_motor_not_running_3_str       = { Disp_Draw_Tekst_L_Option_On,  37, REGEL_3, &tekst.Motor_not_running_10,       UCHAR, &alarm_actueel.value };

static s_disp_tekst const disp_general_error_str = { Disp_Draw_Tekst_L, 37, REGEL_3, &tekst.General_Error_10 };
static s_disp_tekst const disp_motor_fault_str = { Disp_Draw_Tekst_L, 37, REGEL_3, &tekst.Motor_Fault_10 };
static s_disp_tekst const disp_motor_blocked_str = { Disp_Draw_Tekst_L, 37, REGEL_3, &tekst.Motor_Blocked_10 };
static s_disp_tekst const disp_heat_sink_temperature_str = { Disp_Draw_Tekst_L, 37, REGEL_3, &tekst.Heat_Sink_Temperature_10 };
static s_disp_tekst const disp_ground_fault_str = { Disp_Draw_Tekst_L, 37, REGEL_3, &tekst.Ground_Fault_10 };
static s_disp_tekst const disp_hall_ic_fault_str = { Disp_Draw_Tekst_L, 37, REGEL_3, &tekst.Hall_IC_Fault_10 };
static s_disp_tekst const disp_overcurrent_str = { Disp_Draw_Tekst_L, 37, REGEL_3, &tekst.Overcurrent_10 };
static s_disp_tekst const disp_line_fault_str = { Disp_Draw_Tekst_L, 37, REGEL_3, &tekst.Line_Fault_10 };
static s_disp_tekst const disp_int_heat_sink_sensor_str = { Disp_Draw_Tekst_L, 37, REGEL_3, &tekst.Int_Heat_Sink_Sensor_10 };
static s_disp_tekst const disp_dc_res_voltage_to_high_str = { Disp_Draw_Tekst_L, 37, REGEL_3, &tekst.DC_Res_Voltage_To_High_10 };
static s_disp_tekst const disp_temperature_lowering_str = { Disp_Draw_Tekst_L, 37, REGEL_3, &tekst.Temperature_Lowering_10 };
static s_disp_tekst const disp_wrong_connection_str = { Disp_Draw_Tekst_L, 37, REGEL_3, &tekst.Wrong_Connection_10 };
static s_disp_tekst const disp_external_fault_str = { Disp_Draw_Tekst_L, 37, REGEL_3, &tekst.External_Fault_10 };
static s_disp_tekst const disp_factory_settings_str = { Disp_Draw_Tekst_L, 37, REGEL_3, &tekst.Factory_Settings_10 };
static s_disp_tekst const disp_eep_error_str = { Disp_Draw_Tekst_L, 37, REGEL_3, &tekst.EEP_Error_10 };
static s_disp_tekst const disp_rtc_general_fault_str = { Disp_Draw_Tekst_L, 37, REGEL_3, &tekst.RTC_General_Fault_10 };
static s_disp_tekst const disp_rtc_voltage_fault_str = { Disp_Draw_Tekst_L, 37, REGEL_3, &tekst.RTC_Voltage_Fault_10 };
static s_disp_tekst const disp_filter_contamination_str = { Disp_Draw_Tekst_L, 37, REGEL_3, &tekst.Filter_Contamination_10 };
static s_disp_tekst const disp_transfer_error_str = { Disp_Draw_Tekst_L, 37, REGEL_3, &tekst.Transfer_Error_10 };
static s_disp_tekst const disp_data_connection_line_str = { Disp_Draw_Tekst_L, 37, REGEL_3, &tekst.Data_Connetion_Line_10 };
static s_disp_tekst const disp_data_connection_checksum_str = { Disp_Draw_Tekst_L, 37, REGEL_3, &tekst.Data_Connection_Checksum_10 };
static s_disp_tekst const disp_sensor_fault_input_1_str = { Disp_Draw_Tekst_L, 37, REGEL_3, &tekst.Sensor_Fault_Input_1_10 };
static s_disp_tekst const disp_sensor_fault_input_2_str = { Disp_Draw_Tekst_L, 37, REGEL_3, &tekst.Sensor_Fault_Input_2_10 };
static s_disp_tekst const disp_sensor_fault_input_3_str = { Disp_Draw_Tekst_L, 37, REGEL_3, &tekst.Sensor_Fault_Input_3_10 };

static s_disp_tekst const disp_not_installed_str           = { Disp_Draw_Tekst_L, 37, REGEL_3, &tekst.Not_installed_10           };
static s_disp_tekst const disp_dualscreen_not_possible_str = { Disp_Draw_Tekst_L, 37, REGEL_3, &tekst.Dualscreen_not_possible_10 };
static s_disp_tekst const disp_install_mode_str            = { Disp_Draw_Tekst_L, 37, REGEL_3, &tekst.Installation_mode_10       };
static s_disp_tekst const disp_sensor_hi_speed_str         = { Disp_Draw_Tekst_L, 37, REGEL_3, &tekst.Sensor_hi_speed_10         };
static s_disp_tekst const disp_deviation_position_str      = { Disp_Draw_Tekst_L, 37, REGEL_3, &tekst.Deviation_position_10      };
static s_disp_tekst const disp_limitswitches_not_equal_str = { Disp_Draw_Tekst_L, 37, REGEL_3, &tekst.Limitswitches_not_equal_10 };
static s_disp_tekst const disp_speed_not_equal_str         = { Disp_Draw_Tekst_L, 37, REGEL_3, &tekst.Speed_not_equal_10         };
static s_disp_tekst const disp_multiple_master_str         = { Disp_Draw_Tekst_L, 37, REGEL_3, &tekst.Multiple_master_10         };
static s_disp_tekst const disp_frequency_controller_str    = { Disp_Draw_Tekst_L, 37, REGEL_3, &tekst.Frequency_controller_10    };
static s_disp_tekst const disp_wrong_direction_str         = { Disp_Draw_Tekst_L, 37, REGEL_3, &tekst.Wrong_direction_10         };
static s_disp_tekst const disp_link_unknown_str            = { Disp_Draw_Tekst_L, 37, REGEL_3, &tekst.Link_unknown_10            };
static s_disp_tekst const disp_position_not_reached_str    = { Disp_Draw_Tekst_L, 37, REGEL_3, &tekst.Position_not_reached_10    };
static s_disp_tekst const disp_limitswitch_str             = { Disp_Draw_Tekst_L, 37, REGEL_3, &tekst.Limitswitch_not_reached_10 };
static s_disp_tekst const disp_extern_alarm_str            = { Disp_Draw_Tekst_L, 37, REGEL_3, &tekst.Extern_alarm_10            };

static s_disp_tekst const disp_locked_motor_str              = { Disp_Draw_Tekst_L, 37, REGEL_3, &tekst.Locked_motor_10              };
static s_disp_tekst const disp_hall_failure_str              = { Disp_Draw_Tekst_L, 37, REGEL_3, &tekst.Hall_failure_10              };
static s_disp_tekst const disp_thermal_motor_str             = { Disp_Draw_Tekst_L, 37, REGEL_3, &tekst.Thermal_motor_10             };
static s_disp_tekst const disp_comm_error_master_slave_str   = { Disp_Draw_Tekst_L, 37, REGEL_3, &tekst.Comm_error_master_slave_10   };
static s_disp_tekst const disp_thermal_power_module_str      = { Disp_Draw_Tekst_L, 37, REGEL_3, &tekst.Thermal_power_module_10      };
static s_disp_tekst const disp_comm_error_remote_unit_str    = { Disp_Draw_Tekst_L, 37, REGEL_3, &tekst.Comm_error_remote_unit_10    };
static s_disp_tekst const disp_phase_failure_str             = { Disp_Draw_Tekst_L, 37, REGEL_3, &tekst.Phase_failure_10             };
static s_disp_tekst const disp_brake_str                     = { Disp_Draw_Tekst_L, 37, REGEL_3, &tekst.Break_10                     };
static s_disp_tekst const disp_high_line_voltage_str         = { Disp_Draw_Tekst_L, 37, REGEL_3, &tekst.High_line_voltage_10         };
static s_disp_tekst const disp_low_line_voltage_str          = { Disp_Draw_Tekst_L, 37, REGEL_3, &tekst.Low_line_voltage_10          };
static s_disp_tekst const disp_low_dc_link_voltage_str       = { Disp_Draw_Tekst_L, 37, REGEL_3, &tekst.Low_DC_link_voltage_10       };
static s_disp_tekst const disp_high_dc_link_voltage_str      = { Disp_Draw_Tekst_L, 37, REGEL_3, &tekst.High_DC_link_voltage_10      };
static s_disp_tekst const disp_driver_problem_str            = { Disp_Draw_Tekst_L, 37, REGEL_3, &tekst.Driver_problem_10            };
static s_disp_tekst const disp_electronic_box_over_heat_str  = { Disp_Draw_Tekst_L, 37, REGEL_3, &tekst.Electronic_box_over_heat_10  };
static s_disp_tekst const disp_excessive_dc_link_current_str = { Disp_Draw_Tekst_L, 37, REGEL_3, &tekst.Excessive_DC_link_current_10 };
static s_disp_tekst const disp_i_limit_str                   = { Disp_Draw_Tekst_L, 37, REGEL_3, &tekst.i_limit_10                   };
static s_disp_tekst const disp_p_limit_str                   = { Disp_Draw_Tekst_L, 37, REGEL_3, &tekst.p_limit_10                   };
static s_disp_tekst const disp_te_high_str                   = { Disp_Draw_Tekst_L, 37, REGEL_3, &tekst.te_high_10                   };
static s_disp_tekst const disp_tm_high_str                   = { Disp_Draw_Tekst_L, 37, REGEL_3, &tekst.tm_high_10                   };
static s_disp_tekst const disp_tei_high_str                  = { Disp_Draw_Tekst_L, 37, REGEL_3, &tekst.tei_high_10                  };
static s_disp_tekst const disp_uz_low_str                    = { Disp_Draw_Tekst_L, 37, REGEL_3, &tekst.uz_low_10                    };
static s_disp_tekst const disp_n_low_str                     = { Disp_Draw_Tekst_L, 37, REGEL_3, &tekst.n_low_10                     };
static s_disp_tekst const disp_igbt_fault_str                = { Disp_Draw_Tekst_L, 37, REGEL_3, &tekst.igbt_fault_10                };
static s_disp_tekst const disp_uzk_hi_str                    = { Disp_Draw_Tekst_L, 37, REGEL_3, &tekst.uzk_hi_10                    };
static s_disp_tekst const disp_uzk_lo_str                    = { Disp_Draw_Tekst_L, 37, REGEL_3, &tekst.uzk_lo_10                    };
static s_disp_tekst const disp_uin_hi_str                    = { Disp_Draw_Tekst_L, 37, REGEL_3, &tekst.uin_hi_10                    };
static s_disp_tekst const disp_uin_lo_str                    = { Disp_Draw_Tekst_L, 37, REGEL_3, &tekst.uin_lo_10                    };

static s_disp_tekst const disp_failure_power_section_str     = { Disp_Draw_Tekst_L, 37, REGEL_3, &tekst.failure_power_section_10     };
static s_disp_tekst const disp_umax_str                      = { Disp_Draw_Tekst_L, 37, REGEL_3, &tekst.umax_10                      };
static s_disp_tekst const disp_umin_str                      = { Disp_Draw_Tekst_L, 37, REGEL_3, &tekst.umin_10                      };
static s_disp_tekst const disp_overspeed_str                 = { Disp_Draw_Tekst_L, 37, REGEL_3, &tekst.overspeed_10                 };
static s_disp_tekst const disp_locked_rotor_str              = { Disp_Draw_Tekst_L, 37, REGEL_3, &tekst.locked_rotor_10              };
static s_disp_tekst const disp_underspeed_str                = { Disp_Draw_Tekst_L, 37, REGEL_3, &tekst.underspeed_10                };
static s_disp_tekst const disp_24V_supply_overloaded_str     = { Disp_Draw_Tekst_L, 37, REGEL_3, &tekst._24V_supply_overload_10      };
static s_disp_tekst const disp_input_phase_error_str         = { Disp_Draw_Tekst_L, 37, REGEL_3, &tekst.input_phase_error_10         };
static s_disp_tekst const disp_motor_phase_error_str         = { Disp_Draw_Tekst_L, 37, REGEL_3, &tekst.motor_phase_error_10         };																								   
static s_disp_tekst const disp_memory_error_str              = { Disp_Draw_Tekst_L, 37, REGEL_3, &tekst.memory_error_10              };
static s_disp_tekst const disp_short_circuit_str             = { Disp_Draw_Tekst_L, 37, REGEL_3, &tekst.short_circuit_10             };
static s_disp_tekst const disp_loss_of_synchronism_str       = { Disp_Draw_Tekst_L, 37, REGEL_3, &tekst.loss_of_synchronism_10       };
static s_disp_tekst const disp_input_voltage_error_str       = { Disp_Draw_Tekst_L, 37, REGEL_3, &tekst.input_voltage_error_10       };
static s_disp_tekst const disp_input_relay_not_closed_str    = { Disp_Draw_Tekst_L, 37, REGEL_3, &tekst.input_relay_not_closed_10    };
static s_disp_tekst const disp_high_starting_current_str     = { Disp_Draw_Tekst_L, 37, REGEL_3, &tekst.high_starting_current_10     };

static s_disp_tekst const disp_buitenklep_str            = { Disp_Draw_Tekst_L, 37, REGEL_2, &tekst.Buitenklep_10            };
static s_disp_tekst const disp_binnenklep_str            = { Disp_Draw_Tekst_L, 37, REGEL_2, &tekst.Binnenklep_10            };
static s_disp_tekst const disp_bovenklep_str             = { Disp_Draw_Tekst_L, 37, REGEL_2, &tekst.Bovenklep_10             };
static s_disp_tekst const disp_recirculatieklep_str      = { Disp_Draw_Tekst_L, 37, REGEL_2, &tekst.Recirculatieklep_10      };
static s_disp_tekst const disp_inblaasvent_str           = { Disp_Draw_Tekst_L, 37, REGEL_2, &tekst.Inblaasvent_10           };
static s_disp_tekst const disp_afblaasvent_str           = { Disp_Draw_Tekst_L, 37, REGEL_2, &tekst.Afblaasvent_10           };
static s_disp_tekst const disp_verwarming_str            = { Disp_Draw_Tekst_L, 37, REGEL_2, &tekst.Verwarming_10            };
static s_disp_tekst const disp_vorstbeveiliging_str      = { Disp_Draw_Tekst_L, 37, REGEL_3, &tekst.Vorstbeveiliging_10      };
static s_disp_tekst const disp_algemeen_extern_alarm_str = { Disp_Draw_Tekst_L, 37, REGEL_3, &tekst.Algemeen_extern_alarm_10 };
static s_disp_tekst const disp_drukverschil_bewaking_str = { Disp_Draw_Tekst_L, 37, REGEL_3, &tekst.Drukverschil_bewaking_10 };

static s_disp_tekst const disp_syst_al_geheugen_256k_str = { Disp_Draw_Tekst_L, 37, REGEL_2, &tekst.Geheugen_256k_10 };   
static s_disp_tekst const disp_syst_al_eeprom_256k_str   = { Disp_Draw_Tekst_L, 37, REGEL_2, &tekst.EEPROM_256k_10 };
static s_disp_tekst const disp_OK_is_opties_wissen_str   = { Disp_Draw_Tekst_L, 37, REGEL_3, &tekst.OK_is_opties_wissen_10 };   
//*****************************************************************************
static void * const lcd_disp_header[] = { &disp_control, &disp_header, &disp_time_on_alarm, &disp_time_on_bmp, &disp_time_on, 0};
static void *lcd_alarm_actueel[20];

//*****************************************************************************
static void * const lcd_geen_al[] =
{
                        // icoon
                        // regel 1
  &disp_geen_alarm_str, // regel 2
  0                     // regel 3
};
static void * const lcd_onbekend_al[] =
{
                            // icoon
                            // regel 1 
  &disp_onbekend_alarm_str, // regel 2
  0                         // regel 3
};
static void * const lcd_systeem_al_option[] =
{ 
  &disp_systeem,           // icoon
  &disp_alarm_systeem_str, // regel 1
  &disp_opties_str,        // regel 2
  &disp_gewist_str, 0      // regel 3
};
static void * const lcd_systeem_al_setpoint[] =
{ 
  &disp_systeem,           // icoon
  &disp_alarm_systeem_str, // regel 1
  &disp_instellingen_str,  // regel 2
  &disp_gewist_str, 0      // regel 3
};
static void * const lcd_systeem_al_value_hr[] =
{
  &disp_systeem,                  // icoon
  &disp_waarschuwing_systeem_str, // regel 1
  &disp_minimum_maximum_str,      // regel 2
  &disp_gewist_str, 0             // regel 3
};
static void * const lcd_systeem_al_I2C0[] =
{
  &disp_systeem,           // icoon
  &disp_alarm_systeem_str, // regel 1
  &disp_syst_al_I2C0_str,  // regel 2
  0                        // regel 3
};
static void * const lcd_systeem_al_EEP[] =
{
  &disp_systeem,           // icoon
  &disp_alarm_systeem_str, // regel 1
  &disp_syst_al_EEP_str,   // regel 2
  0                        // regel 3
};
static void * const lcd_systeem_al_EEP_taal[] =
{
  &disp_systeem,              // icoon
  &disp_alarm_systeem_str,    // regel 1
  &disp_syst_al_EEP_taal_str, // regel 2
  0                           // regel 3
};
static void * const lcd_systeem_al_geen_alarm_contact[] =
{
  &disp_systeem,                        // icoon
  &disp_alarm_systeem_str,              // regel 1
  &disp_syst_al_geen_alarm_contact_str, // regel 2
  0                                     // regel 3
};
static void * const lcd_systeem_al_RTC[] =
{
  &disp_systeem,                  // icoon
  &disp_waarschuwing_systeem_str, // regel 1
  &disp_syst_al_RTC_str,          // regel 2
  0                               // regel 3
};
static void * const lcd_systeem_al_TIMER_1ms[] =
{ 
  &disp_systeem,                  // icoon
  &disp_waarschuwing_systeem_str, // regel 1
  &disp_syst_al_TIMER_1ms_str,    // regel 2
  0                               // regel 3
};
static void * const lcd_systeem_al_HTRAP[] =
{
  &disp_systeem,                  // icoon
  &disp_waarschuwing_systeem_str, // regel 1
  &disp_syst_al_HTRAP_str, &disp_space_10_L, &disp_add_nr_val, // regel 2
  0                               // regel 3
};
static void * const lcd_systeem_al_PLL[] =
{
  &disp_systeem,                  // icoon
  &disp_waarschuwing_systeem_str, // regel 1
  &disp_syst_al_PLL_str,          // regel 2
  0                               // regel 3
};
static void * const lcd_systeem_al_opstart[] =
{
  &disp_systeem,                  // icoon
  &disp_waarschuwing_systeem_str, // regel 1
  &disp_syst_al_opstart_str,      // regel 2
  0                               // regel 3
};
static void * const lcd_systeem_al_alarmen_gewist[] =
{
  &disp_systeem,                  // icoon
  &disp_waarschuwing_systeem_str, // regel 1
  &disp_alarmen_str,              // regel 2
  &disp_gewist_str, 0             // regel 3
};
static void * const lcd_IO_06_14_board_al[] =
{
  &disp_systeem,                                         // icoon
  &disp_alarm_systeem_str,                               // regel 1
  &disp_print_str, &disp_space_10_L, &disp_IO_06_14_str, // regel 2 
  &disp_niet_gevonden_str, 0                             // regel 3
};
static void * const lcd_IO_06_14_onbekend_al[] =
{
  &disp_systeem,                                                 // icoon
  &disp_alarm_systeem_str,                                       // regel 1
  &disp_print_str, &disp_space_10_L, &disp_IO_06_14_str,         // regel 2
  &disp_onbekend_alarm_3_str, &disp_space_10_L, &disp_add_nr_val, 0 // regel 3
};
static void * const lcd_IO_12_06_board_al[] =
{
  &disp_systeem,                                         // icoon
  &disp_alarm_systeem_str,                               // regel 1
  &disp_print_str, &disp_space_10_L, &disp_IO_12_06_str, // regel 2 
  &disp_niet_gevonden_str, 0                             // regel 3
};
static void * const lcd_IO_12_06_onbekend_al[] =
{
  &disp_systeem,                                                 // icoon
  &disp_alarm_systeem_str,                                       // regel 1
  &disp_print_str, &disp_space_10_L, &disp_IO_12_06_str,         // regel 2
  &disp_onbekend_alarm_3_str, &disp_space_10_L, &disp_add_nr_val, 0 // regel 3
};
static void * const lcd_IO_08_09_board_al[] =
{
  &disp_systeem,                                         // icoon
  &disp_alarm_systeem_str,                               // regel 1
  &disp_print_str, &disp_space_10_L, &disp_IO_08_09_str, // regel 2 
  &disp_niet_gevonden_str, 0                             // regel 3
};
static void * const lcd_IO_08_09_onbekend_al[] =
{
  &disp_systeem,                                                 // icoon
  &disp_alarm_systeem_str,                                       // regel 1
  &disp_print_str, &disp_space_10_L, &disp_IO_08_09_str,         // regel 2
  &disp_onbekend_alarm_3_str, &disp_space_10_L, &disp_add_nr_val, 0 // regel 3
};
static void * const lcd_IO_EKU_board_al[] =
{
  &disp_systeem,                                        // icoon
  &disp_alarm_systeem_str,                              // regel 1
  &disp_print_str, &disp_space_10_L, &disp_IO_EKU_str,  // regel 2 
  &disp_niet_gevonden_str, 0                            // regel 3
};
static void * const lcd_IO_EKU_onbekend_al[] =
{
  &disp_systeem,                                                 // icoon
  &disp_alarm_systeem_str,                                       // regel 1
  &disp_print_str, &disp_space_10_L, &disp_IO_EKU_str,           // regel 2
  &disp_onbekend_alarm_3_str, &disp_space_10_L, &disp_add_nr_val, 0 // regel 3
};
static void * const lcd_IO_H2MC_board_al[] =
{
  &disp_systeem,                                        // icoon
  &disp_alarm_systeem_str,                              // regel 1
  &disp_print_str, &disp_space_10_L, &disp_IO_H2MC_str, // regel 2 
  &disp_niet_gevonden_str, 0                            // regel 3
};
static void * const lcd_IO_H2MC_onbekend_al[] =
{
  &disp_systeem,                                                 // icoon
  &disp_alarm_systeem_str,                                       // regel 1
  &disp_print_str, &disp_space_10_L, &disp_IO_H2MC_str,          // regel 2
  &disp_onbekend_alarm_3_str, &disp_space_10_L, &disp_add_nr_val, 0 // regel 3
};
static void * const lcd_IO_H1MC_board_al[] =
{
  &disp_systeem,                                         // icoon
  &disp_alarm_systeem_str,                               // regel 1
  &disp_print_str, &disp_space_10_L, &disp_IO_H1MC_str,  // regel 2 
  &disp_niet_gevonden_str, 0                             // regel 3
};
static void * const lcd_IO_H1MC_onbekend_al[] =
{
  &disp_systeem,                                                 // icoon
  &disp_alarm_systeem_str,                                       // regel 1
  &disp_print_str, &disp_space_10_L, &disp_IO_H1MC_str,          // regel 2
  &disp_onbekend_alarm_3_str, &disp_space_10_L, &disp_add_nr_val, 0 // regel 3
};
static void * const lcd_IO_05_07_board_al[] =
{
  &disp_systeem,                                         // icoon
  &disp_alarm_systeem_str,                               // regel 1
  &disp_print_str, &disp_space_10_L, &disp_IO_05_07_str, // regel 2 
  &disp_niet_gevonden_str, 0                             // regel 3
};
static void * const lcd_IO_05_07_adc_al[] =
{
  &disp_systeem,                                         // icoon
  &disp_alarm_systeem_str,                               // regel 1
  &disp_print_str, &disp_space_10_L, &disp_IO_05_07_str, // regel 2
  &disp_adc_str, 0                                       // regel 3
};
static void * const lcd_IO_05_07_externe_24v_al[] =
{
  &disp_systeem,                                          // icoon
  &disp_alarm_systeem_str,                                // regel 1
  &disp_print_str, &disp_space_10_L, &disp_IO_05_07_str,  // regel 2
  &disp_externe_24v_str, 0                                // regel 3
};
static void * const lcd_IO_05_07_adc_nr_al[] =
{
  &disp_systeem,                                                  // icoon
  &disp_alarm_systeem_str,                                        // regel 1
  &disp_print_str, &disp_space_10_L, &disp_IO_05_07_str,          // regel 2
  &disp_analoge_ingang_str, &disp_space_10_L, &disp_add_nr_val, 0 // regel 3
};
static void * const lcd_IO_05_07_onbekend_al[] =
{
  &disp_systeem,                                                 // icoon
  &disp_alarm_systeem_str,                                       // regel 1
  &disp_print_str, &disp_space_10_L, &disp_IO_05_07_str,         // regel 2
  &disp_onbekend_alarm_3_str, &disp_space_10_L, &disp_add_nr_val, 0 // regel 3
};
static void * const lcd_IO_05_07_versie_al[] =
{ 
  &disp_systeem,                                            // icoon 
  &disp_alarm_systeem_str,                                  // regel 1
  &disp_print_str, &disp_space_10_L, &disp_IO_05_07_str,    // regel 2
  &disp_version_error_str, 0                                // regel 3
};

static void * const lcd_IO_07_07_board_al[] =
{
  &disp_systeem,                                         // icoon
  &disp_alarm_systeem_str,                               // regel 1
  &disp_print_str, &disp_space_10_L, &disp_IO_07_07_str, // regel 2 
  &disp_niet_gevonden_str, 0                             // regel 3
};
static void * const lcd_IO_07_07_adc_al[] =
{
  &disp_systeem,                                         // icoon
  &disp_alarm_systeem_str,                               // regel 1
  &disp_print_str, &disp_space_10_L, &disp_IO_07_07_str, // regel 2
  &disp_adc_str, 0                                       // regel 3
};
static void * const lcd_IO_07_07_externe_24v_al[] =
{
  &disp_systeem,                                          // icoon
  &disp_alarm_systeem_str,                                // regel 1
  &disp_print_str, &disp_space_10_L, &disp_IO_07_07_str,  // regel 2
  &disp_externe_24v_str, 0                                // regel 3
};
static void * const lcd_IO_07_07_adc_nr_al[] =
{
  &disp_systeem,                                                  // icoon
  &disp_alarm_systeem_str,                                        // regel 1
  &disp_print_str, &disp_space_10_L, &disp_IO_07_07_str,          // regel 2
  &disp_analoge_ingang_str, &disp_space_10_L, &disp_add_nr_val, 0 // regel 3
};
static void * const lcd_IO_07_07_onbekend_al[] =
{
  &disp_systeem,                                                 // icoon
  &disp_alarm_systeem_str,                                       // regel 1
  &disp_print_str, &disp_space_10_L, &disp_IO_07_07_str,         // regel 2
  &disp_onbekend_alarm_3_str, &disp_space_10_L, &disp_add_nr_val, 0 // regel 3
};
static void * const lcd_IO_07_07_versie_al[] =
{ 
  &disp_systeem,                                            // icoon 
  &disp_alarm_systeem_str,                                  // regel 1
  &disp_print_str, &disp_space_10_L, &disp_IO_07_07_str,    // regel 2
  &disp_version_error_str, 0                                // regel 3
};

static void * const lcd_orion_off_al[] =
{
  &disp_systeem,                  // icoon
  &disp_waarschuwing_systeem_str, // regel 1
  &disp_orion_uit_str,            // regel 2
  0                               // regel 3
};
static void * const lcd_orion_on_al[] =
{
  &disp_systeem,                  // icoon
  &disp_waarschuwing_systeem_str, // regel 1
  &disp_orion_on_str,             // regel 2
  0                               // regel 3
};
static void * const lcd_computer_al[] =
{
  &disp_systeem,                                                                                      // icoon
  &disp_waarschuwing_systeem_str,                                                                     // regel 1
  &disp_computer_str, &disp_space_10_L, &disp_computer_nr_val, &disp_space_10_L, &disp_add_alarm_str, // regel 2
  0                                                                                                   // regel 3
};
#ifdef CAN_BACKBONE_PC_WARNING
static void * const lcd_can_pc_1_al[] =
{
  &disp_systeem,               // icoon
  &disp_waarschuwing_systeem_str, // regel 1
  &disp_can_pc_1_offline_str,  // regel 2
  0                            // regel 3
};
static void * const lcd_can_pc_2_al[] =
{
  &disp_systeem,               // icoon
  &disp_waarschuwing_systeem_str, // regel 1
  &disp_can_pc_2_offline_str,  // regel 2
  0                            // regel 3
};
static void * const lcd_can_pc_3_al[] =
{
  &disp_systeem,               // icoon
  &disp_waarschuwing_systeem_str, // regel 1
  &disp_can_pc_3_offline_str,  // regel 2
  0                            // regel 3
};
#else // CAN_BACKBONE_PC_WARNING
static void * const lcd_can_pc_al[] =
{
  &disp_systeem,                                                 // icoon
  &disp_alarm_systeem_str,       // regel 1
  &disp_can_pc_str, &disp_space_10_L, &disp_add_offline_str, // regel 2
  0                                                              // regel 3
};
#endif // CAN_BACKBONE_PC_WARNING
static void * const lcd_can_computer_al[] =
{
  &disp_systeem,                                                                                        // icoon
  &disp_alarm_systeem_str,                                                                              // regel 1
  &disp_computer_str, &disp_space_10_L, &disp_computer_nr_val, &disp_space_10_L, &disp_add_offline_str, // regel 2
  0                                                                                                     // regel 3
};
static void * const lcd_systeem_al_restore_option_setpoint_failed[] =
{ 
  &disp_systeem,                    // icoon
  &disp_alarm_systeem_str,          // regel 1
  &disp_opties_en_instellingen_str, // regel 2
  &disp_niet_terug_gezet, 0         // regel 3
};
static void * const lcd_systeem_al_new_option_setpoint[] =
{ 
  &disp_systeem,                    // icoon
  &disp_waarschuwing_systeem_str,   // regel 1
  &disp_opties_en_instellingen_str, // regel 2
  &disp_terug_gezet_str, 0          // regel 3
};
static void * const lcd_systeem_al_new_option[] =
{ 
  &disp_systeem,                  // icoon
  &disp_waarschuwing_systeem_str, // regel 1
  &disp_opties_str,               // regel 2
  &disp_terug_gezet_str, 0        // regel 3
};
static void * const lcd_systeem_al_new_setpoint[] =
{ 
  &disp_systeem,                  // icoon
  &disp_waarschuwing_systeem_str, // regel 1
  &disp_instellingen_str,         // regel 2
  &disp_terug_gezet_str, 0        // regel 3
};
//-----------------------------------------------------------------------------------
static void * const lcd_motorgroup_manual_al[] =
{
  &disp_raam_doek_al, &disp_alarm, // icoon
  &disp_waarschuwing_str,          // regel 1
  &disp_groep_str,                 // regel 2
  &disp_handbediening_str, 0       // regel 3
};
//-----------------------------------------------------------------------------------
static void * const lcd_motor_emergencyswitch_al[] =
{
  &disp_motor_al, &disp_alarm,                           // icoon
//  &disp_alarm_str,                                     // regel 1
  &disp_motor_1_str,                                     // regel 1
  &disp_slave_str, &disp_space_10_L, &disp_slave_nr_val, // regel 2
  &disp_emergency_switch_2_str,                          // regel 2
  &disp_emergency_switch_3_str, 0                        // regel 3
};
static void * const lcd_motor_thermal_close_al[] =
{
  &disp_motor_al, &disp_alarm,                           // icoon
//  &disp_alarm_str,                                     // regel 1
  &disp_motor_1_str,                                     // regel 1
  &disp_slave_str, &disp_space_10_L, &disp_slave_nr_val, // regel 2
  &disp_thermal_failure_close_2_str,                     // regel 2
  &disp_thermal_failure_close_3_str, 0                   // regel 3
};
static void * const lcd_motor_thermal_open_al[] =
{
  &disp_motor_al, &disp_alarm,                           // icoon
//  &disp_alarm_str,                                     // regel 1
  &disp_motor_1_str,                                     // regel 1
  &disp_slave_str, &disp_space_10_L, &disp_slave_nr_val, // regel 2
  &disp_thermal_failure_open_2_str,                      // regel 2
  &disp_thermal_failure_open_3_str, 0                    // regel 3
};
static void * const lcd_motor_frequency_controller_al[] =
{
  &disp_motor_al, &disp_alarm,       // icoon
  &disp_alarm_str,                   // regel 1
  &disp_motor_2_str,                 // regel 2
  &disp_frequency_controller_str, 0  // regel 3
};
static void * const lcd_motor_break_input_al[] =
{
  &disp_motor_al, &disp_alarm,                           // icoon
//  &disp_alarm_str,                                     // regel 1
  &disp_motor_1_str,                                     // regel 1
  &disp_slave_str, &disp_space_10_L, &disp_slave_nr_val, // regel 2
  &disp_break_input_2_str,                               // regel 2
  &disp_break_input_3_str, 0                             // regel 3
};
static void * const lcd_motor_speed_to_low_al[] =
{
  &disp_motor_al, &disp_alarm,                           // icoon
//  &disp_alarm_str,                                     // regel 1
  &disp_motor_1_str,                                     // regel 1
  &disp_slave_str, &disp_space_10_L, &disp_slave_nr_val, // regel 2
  &disp_speed_to_low_2_str,                              // regel 2
  &disp_speed_to_low_3_str, 0                            // regel 3
};
static void * const lcd_motor_encoder_failure_al[] =
{
  &disp_motor_al, &disp_alarm,                           // icoon
//  &disp_alarm_str,                                     // regel 1
  &disp_motor_1_str,                                     // regel 1
  &disp_slave_str, &disp_space_10_L, &disp_slave_nr_val, // regel 2
  &disp_encoder_failure_2_str,                           // regel 2
  &disp_encoder_failure_3_str, 0                         // regel 3
};
static void * const lcd_motor_encoder_failure_A_al[] =
{
  &disp_motor_al, &disp_alarm,                           // icoon
//  &disp_alarm_str,                                     // regel 1
  &disp_motor_1_str,                                     // regel 1
  &disp_slave_str, &disp_space_10_L, &disp_slave_nr_val, // regel 2
  &disp_encoder_failure_A_2_str,                         // regel 2
  &disp_encoder_failure_A_3_str, 0                       // regel 3
};
static void * const lcd_motor_encoder_failure_B_al[] =
{
  &disp_motor_al, &disp_alarm,                           // icoon
//  &disp_alarm_str,                                     // regel 1
  &disp_motor_1_str,                                     // regel 1
  &disp_slave_str, &disp_space_10_L, &disp_slave_nr_val, // regel 2
  &disp_encoder_failure_B_2_str,                         // regel 2
  &disp_encoder_failure_B_3_str, 0                       // regel 3
};
static void * const lcd_motor_no_feedback_al[] =
{
  &disp_motor_al, &disp_alarm,                           // icoon
//  &disp_alarm_str,                                     // regel 1
  &disp_motor_1_str,                                     // regel 1
  &disp_slave_str, &disp_space_10_L, &disp_slave_nr_val, // regel 2
  &disp_no_feedback_2_str,                               // regel 2
  &disp_no_feedback_3_str, 0                             // regel 3
};
static void * const lcd_motor_not_enough_pulses_al[] =
{
  &disp_motor_al, &disp_alarm,                           // icoon
//  &disp_waarschuwing_str,                              // regel 1
  &disp_motor_1_str,                                     // regel 1
  &disp_slave_str, &disp_space_10_L, &disp_slave_nr_val, // regel 2
  &disp_not_enough_pulses_2_str,                         // regel 2
  &disp_not_enough_pulses_3_str, 0                       // regel 3
};
static void * const lcd_motor_pulses_to_fast_al[] =
{
  &disp_motor_al, &disp_alarm,                           // icoon
//  &disp_waarschuwing_str,                              // regel 1
  &disp_motor_1_str,                                     // regel 1
  &disp_slave_str, &disp_space_10_L, &disp_slave_nr_val, // regel 2
  &disp_pulses_to_fast_2_str,                            // regel 2
  &disp_pulses_to_fast_3_str, 0                          // regel 3
};
static void * const lcd_motor_not_installed_al[] =
{
  &disp_motor_al, &disp_alarm, // icoon
  &disp_waarschuwing_str,      // regel 1
  &disp_motor_2_str,           // regel 2
  &disp_not_installed_str, 0   // regel 3
};
static void * const lcd_motor_encoder_interference_al[] =
{
  &disp_motor_al, &disp_alarm,                           // icoon
//  &disp_waarschuwing_str,                              // regel 1
  &disp_motor_1_str,                                     // regel 1
  &disp_slave_str, &disp_space_10_L, &disp_slave_nr_val, // regel 2
  &disp_encoder_interference_2_str,                      // regel 2
  &disp_encoder_interference_3_str, 0                    // regel 3
};
static void * const lcd_motor_dualscreen_not_possible_al[] =
{
  &disp_motor_al, &disp_alarm,         // icoon
  &disp_waarschuwing_str,              // regel 1
  &disp_motor_2_str,                   // regel 2
  &disp_dualscreen_not_possible_str, 0 // regel 3
};
static void * const lcd_motor_install_mode_al[] =
{
  &disp_motor_al, &disp_alarm, // icoon
  &disp_waarschuwing_str,      // regel 1
  &disp_motor_2_str,           // regel 2
  &disp_install_mode_str, 0    // regel 3
};
static void * const lcd_motor_directions_not_defined_al[] =
{
  &disp_motor_al, &disp_alarm,                           // icoon
//  &disp_waarschuwing_str,                              // regel 1
  &disp_motor_1_str,                                     // regel 1
  &disp_slave_str, &disp_space_10_L, &disp_slave_nr_val, // regel 2
  &disp_directions_not_defined_2_str,                    // regel 2
  &disp_directions_not_defined_3_str, 0                  // regel 3
};
static void * const lcd_motor_limit_switch_safety_speed_al[] =
{
  &disp_motor_al, &disp_alarm, // icoon
  &disp_waarschuwing_str,      // regel 1
  &disp_motor_2_str,           // regel 2
  &disp_sensor_hi_speed_str, 0 // regel 3
};
static void * const lcd_motor_warning_deviation_position_al[] =
{
  &disp_motor_al, &disp_alarm,    // icoon
  &disp_waarschuwing_str,         // regel 1
  &disp_motor_2_str,              // regel 2
  &disp_deviation_position_str, 0 // regel 3
};
static void * const lcd_motor_error_deviation_position_al[] =
{
  &disp_motor_al, &disp_alarm,    // icoon
  &disp_alarm_str,                // regel 1
  &disp_motor_2_str,              // regel 2
  &disp_deviation_position_str, 0 // regel 3
};
static void * const lcd_motor_communication_al[] =
{
  &disp_motor_communication_al_ico,                      // icoon
//  &disp_alarm_str,                                     // regel 1
  &disp_motor_1_str,                                     // regel 1
  &disp_slave_str, &disp_space_10_L, &disp_slave_nr_val, // regel 2
  &disp_niet_gevonden_str, 0                             // regel 3
};
static void * const lcd_motor_limitswitches_not_equal_al[] =
{
  &disp_motor_al, &disp_alarm,         // icoon
  &disp_waarschuwing_str,              // regel 1
  &disp_motor_2_str,                   // regel 2
  &disp_limitswitches_not_equal_str, 0 // regel 3
};
static void * const lcd_motor_speed_not_equal_al[] =
{
  &disp_motor_al, &disp_alarm,                           // icoon
//  &disp_alarm_str,                                     // regel 1
  &disp_motor_1_str,                                     // regel 1
  &disp_slave_str, &disp_space_10_L, &disp_slave_nr_val, // regel 2
  &disp_speed_not_equal_str, 0                           // regel 3
};
static void * const lcd_motor_multiple_master_al[] =
{
  &disp_motor_al, &disp_alarm, // icoon
  &disp_alarm_str,             // regel 1
  &disp_motor_2_str,           // regel 2
  &disp_multiple_master_str, 0 // regel 3
};
static void * const lcd_motor_slave_not_init_al[] =
{
  &disp_motor_al, &disp_alarm,                           // icoon
//  &disp_alarm_str,                                     // regel 1
  &disp_motor_1_str,                                     // regel 1
  &disp_slave_str, &disp_space_10_L, &disp_slave_nr_val, // regel 2
  &disp_not_installed_str, 0                             // regel 3
};
static void * const lcd_motor_sync_guard_active_al[] =
{
  &disp_motor_al, &disp_alarm,                           // icoon
//  &disp_waarschuwing_str,                              // regel 1
  &disp_motor_1_str,                                     // regel 1
  &disp_slave_str, &disp_space_10_L, &disp_slave_nr_val, // regel 2
  &disp_gelijkloop_beveiliging_2_str,                    // regel 2
  &disp_gelijkloop_beveiliging_3_str, 0                  // regel 3
};
static void * const lcd_motor_limitswitch_not_reached_al[] =
{
  &disp_motor_al, &disp_alarm,                           // icoon
//  &disp_waarschuwing_str,                              // regel 1
  &disp_motor_1_str,                                     // regel 1
  &disp_slave_str, &disp_space_10_L, &disp_slave_nr_val, // regel 2
  &disp_limitswitch_not_reached_2_str,                   // regel 2
  &disp_limitswitch_not_reached_3_str, 0                 // regel 3
};
static void * const lcd_motor_wrong_direction_al[] =
{
  &disp_motor_al, &disp_alarm,                           // icoon
//  &disp_waarschuwing_str,                              // regel 1
  &disp_motor_1_str,                                     // regel 1
  &disp_slave_str, &disp_space_10_L, &disp_slave_nr_val, // regel 2
  &disp_wrong_direction_str, 0                           // regel 3
};
static void * const lcd_motor_link_unknown_al[] =
{
  &disp_motor_al, &disp_alarm, // icoon
  &disp_waarschuwing_str,      // regel 1
  &disp_motor_2_str,           // regel 2
  &disp_link_unknown_str, 0    // regel 3
};
static void * const lcd_motor_not_running_al[] =
{
  &disp_motor_al, &disp_alarm,                           // icoon
//  &disp_waarschuwing_str,                              // regel 1
  &disp_motor_1_str,                                     // regel 1
  &disp_slave_str, &disp_space_10_L, &disp_slave_nr_val, // regel 2
  &disp_motor_not_running_2_str,                         // regel 2
  &disp_motor_not_running_3_str, 0                       // regel 3
};
static void * const lcd_motor_manual_al[] =
{
  &disp_motor_al, &disp_alarm, // icoon
  &disp_waarschuwing_str,      // regel 1
  &disp_motor_2_str,           // regel 2
  &disp_handbediening_str, 0   // regel 3
};
static void * const lcd_motor_unknown_al[] =
{
  &disp_motor_al, &disp_alarm,                           // icoon
//  &disp_waarschuwing_str,                              // regel 1
  &disp_motor_1_str,                                     // regel 1
  &disp_slave_str, &disp_space_10_L, &disp_slave_nr_val, // regel 2
  &disp_onbekend_alarm_2_str,                            // regel 2
  &disp_onbekend_alarm_3_str, 0                          // regel 3
};
static void * const lcd_motor_position_not_reached_al[] =
{
  &disp_motor_al, &disp_alarm,      // icoon
  &disp_alarm_str,                  // regel 1
  &disp_motor_2_str,                // regel 2
  &disp_position_not_reached_str, 0 // regel 3
};

static void * const lcd_vent_manual_al[] =
{
  &disp_vent_al, &disp_alarm,                         // icoon
  &disp_waarschuwing_str,                             // regel 1
  &disp_vent_str, &disp_space_10_L, &disp_add_nr_val, // regel 2
  &disp_handbediening_str, 0                          // regel 3
};
static void * const lcd_vent_target_not_reached_al[] =
{
  &disp_vent_al, &disp_alarm,                         // icoon
  &disp_alarm_str,                                    // regel 1
  &disp_vent_str, &disp_space_10_L, &disp_add_nr_val, // regel 2
  &disp_position_not_reached_str, 0                   // regel 3
};
static void * const lcd_vent_communication_al[] =
{
  &disp_vent_communication_al_ico,                    // icoon
  &disp_alarm_str,                                    // regel 1
  &disp_vent_str, &disp_space_10_L, &disp_add_nr_val, // regel 2
  &disp_niet_gevonden_str, 0                          // regel 3
};

static void * const lcd_klep_manual_al[] =
{
  &disp_klep_al, &disp_alarm,                         // icoon
  &disp_waarschuwing_str,                             // regel 1
  &disp_klep_str, &disp_space_10_L, &disp_add_nr_val, // regel 2
  &disp_handbediening_str, 0                          // regel 3
};
static void * const lcd_klep_target_not_reached_al[] =
{
  &disp_klep_al, &disp_alarm,                         // icoon
  &disp_alarm_str,                                    // regel 1
  &disp_klep_str, &disp_space_10_L, &disp_add_nr_val, // regel 2
  &disp_position_not_reached_str, 0                   // regel 3
};
static void * const lcd_klep_limitswitch_al[] =
{
  &disp_klep_al, &disp_alarm,                         // icoon
  &disp_alarm_str,                                    // regel 1
  &disp_klep_str, &disp_space_10_L, &disp_add_nr_val, // regel 2
  &disp_limitswitch_str, 0                            // regel 3
};
static void * const lcd_klep_extern_alarm_al[] =
{
  &disp_klep_al, &disp_alarm,                         // icoon
  &disp_alarm_str,                                    // regel 1
  &disp_klep_str, &disp_space_10_L, &disp_add_nr_val, // regel 2
  &disp_extern_alarm_str, 0                           // regel 3
};
static void * const lcd_klep_communication_al[] =
{
  &disp_klep_communication_al_ico,                    // icoon
  &disp_alarm_str,                                    // regel 1
  &disp_klep_str, &disp_space_10_L, &disp_add_nr_val, // regel 2
  &disp_niet_gevonden_str, 0                          // regel 3
};

static void * const lcd_drukverschil_communication_al[] =
{
  &disp_sensor_communication_al_ico,                    // icoon
  &disp_alarm_str,                                      // regel 1
  &disp_sensor_str, &disp_space_10_L, &disp_add_nr_val, // regel 2
  &disp_niet_gevonden_str, 0                            // regel 3
};

static void * const lcd_vent_locked_motor_al[] =
{
  &disp_vent_al, &disp_alarm,                         // icoon
  &disp_alarm_str,                                    // regel 1
  &disp_vent_str, &disp_space_10_L, &disp_add_nr_val, // regel 2
  &disp_locked_motor_str, 0                           // regel 3
};
static void * const lcd_vent_hall_failure_al[] =
{
  &disp_vent_al, &disp_alarm,                         // icoon
  &disp_alarm_str,                                    // regel 1
  &disp_vent_str, &disp_space_10_L, &disp_add_nr_val, // regel 2
  &disp_hall_failure_str, 0                           // regel 3
};
static void * const lcd_vent_thermal_motor_al[] =
{
  &disp_vent_al, &disp_alarm,                         // icoon
  &disp_alarm_str,                                    // regel 1
  &disp_vent_str, &disp_space_10_L, &disp_add_nr_val, // regel 2
  &disp_thermal_motor_str, 0                          // regel 3
};
static void * const lcd_vent_comm_error_master_slave_al[] =
{
  &disp_vent_al, &disp_alarm,                         // icoon
  &disp_alarm_str,                                    // regel 1
  &disp_vent_str, &disp_space_10_L, &disp_add_nr_val, // regel 2
  &disp_comm_error_master_slave_str, 0                // regel 3
};
static void * const lcd_vent_thermal_power_module_al[] =
{
  &disp_vent_al, &disp_alarm,                         // icoon
  &disp_alarm_str,                                    // regel 1
  &disp_vent_str, &disp_space_10_L, &disp_add_nr_val, // regel 2
  &disp_thermal_power_module_str, 0                   // regel 3
};
static void * const lcd_vent_comm_error_remote_unit_al[] =
{
  &disp_vent_al, &disp_alarm,                         // icoon
  &disp_alarm_str,                                    // regel 1
  &disp_vent_str, &disp_space_10_L, &disp_add_nr_val, // regel 2
  &disp_comm_error_remote_unit_str, 0                 // regel 3
};
static void * const lcd_vent_phase_failure_al[] =
{
  &disp_vent_al, &disp_alarm,                         // icoon
  &disp_alarm_str,                                    // regel 1
  &disp_vent_str, &disp_space_10_L, &disp_add_nr_val, // regel 2
  &disp_phase_failure_str, 0                          // regel 3
};
static void * const lcd_vent_brake_al[] =
{
  &disp_vent_al, &disp_alarm,                         // icoon
  &disp_alarm_str,                                    // regel 1
  &disp_vent_str, &disp_space_10_L, &disp_add_nr_val, // regel 2
  &disp_brake_str, 0                                  // regel 3
};
static void * const lcd_vent_high_line_voltage_al[] =
{
  &disp_vent_al, &disp_alarm,                         // icoon
  &disp_alarm_str,                                    // regel 1
  &disp_vent_str, &disp_space_10_L, &disp_add_nr_val, // regel 2
  &disp_high_line_voltage_str, 0                      // regel 3
};
static void * const lcd_vent_low_line_voltage_al[] =
{
  &disp_vent_al, &disp_alarm,                         // icoon
  &disp_alarm_str,                                    // regel 1
  &disp_vent_str, &disp_space_10_L, &disp_add_nr_val, // regel 2
  &disp_low_line_voltage_str, 0                       // regel 3
};
static void * const lcd_vent_low_dc_link_voltage_al[] =
{
  &disp_vent_al, &disp_alarm,                         // icoon
  &disp_alarm_str,                                    // regel 1
  &disp_vent_str, &disp_space_10_L, &disp_add_nr_val, // regel 2
  &disp_low_dc_link_voltage_str, 0                    // regel 3
};
static void * const lcd_vent_high_dc_link_voltage_al[] =
{
  &disp_vent_al, &disp_alarm,                         // icoon
  &disp_alarm_str,                                    // regel 1
  &disp_vent_str, &disp_space_10_L, &disp_add_nr_val, // regel 2
  &disp_high_dc_link_voltage_str, 0                   // regel 3
};
static void * const lcd_vent_driver_problem_al[] =
{
  &disp_vent_al, &disp_alarm,                         // icoon
  &disp_alarm_str,                                    // regel 1
  &disp_vent_str, &disp_space_10_L, &disp_add_nr_val, // regel 2
  &disp_driver_problem_str, 0                         // regel 3
};
static void * const lcd_vent_electronic_box_over_heat_al[] =
{
  &disp_vent_al, &disp_alarm,                         // icoon
  &disp_alarm_str,                                    // regel 1
  &disp_vent_str, &disp_space_10_L, &disp_add_nr_val, // regel 2
  &disp_electronic_box_over_heat_str, 0               // regel 3
};
static void * const lcd_vent_excessive_dc_link_current_al[] =
{
  &disp_vent_al, &disp_alarm,                         // icoon
  &disp_alarm_str,                                    // regel 1
  &disp_vent_str, &disp_space_10_L, &disp_add_nr_val, // regel 2
  &disp_excessive_dc_link_current_str, 0              // regel 3
};
static void * const lcd_vent_unknown_al[] =
{
  &disp_vent_al, &disp_alarm,                         // icoon
  &disp_alarm_str,                                    // regel 1
  &disp_vent_str, &disp_space_10_L, &disp_add_nr_val, // regel 2
  &disp_onbekend_alarm_3_str, 0                       // regel 3
};
static void * const lcd_vent_i_limit_al[] =
{
  &disp_vent_al, &disp_alarm,                         // icoon
  &disp_alarm_str,                                    // regel 1
  &disp_vent_str, &disp_space_10_L, &disp_add_nr_val, // regel 2
  &disp_i_limit_str, 0                                // regel 3
};
static void * const lcd_vent_p_limit_al[] =
{
  &disp_vent_al, &disp_alarm,                         // icoon
  &disp_alarm_str,                                    // regel 1
  &disp_vent_str, &disp_space_10_L, &disp_add_nr_val, // regel 2
  &disp_p_limit_str, 0                                // regel 3
};
static void * const lcd_vent_te_high_al[] =
{
  &disp_vent_al, &disp_alarm,                         // icoon
  &disp_alarm_str,                                    // regel 1
  &disp_vent_str, &disp_space_10_L, &disp_add_nr_val, // regel 2
  &disp_te_high_str, 0                                // regel 3
};
static void * const lcd_vent_tm_high_al[] =
{
  &disp_vent_al, &disp_alarm,                         // icoon
  &disp_alarm_str,                                    // regel 1
  &disp_vent_str, &disp_space_10_L, &disp_add_nr_val, // regel 2
  &disp_tm_high_str, 0                                // regel 3
};
static void * const lcd_vent_tei_high_al[] =
{
  &disp_vent_al, &disp_alarm,                         // icoon
  &disp_alarm_str,                                    // regel 1
  &disp_vent_str, &disp_space_10_L, &disp_add_nr_val, // regel 2
  &disp_tei_high_str, 0                               // regel 3
};
static void * const lcd_vent_uz_low_al[] =
{
  &disp_vent_al, &disp_alarm,                         // icoon
  &disp_alarm_str,                                    // regel 1
  &disp_vent_str, &disp_space_10_L, &disp_add_nr_val, // regel 2
  &disp_uz_low_str, 0                                 // regel 3
};
static void * const lcd_vent_n_low_al[] =
{
  &disp_vent_al, &disp_alarm,                         // icoon
  &disp_alarm_str,                                    // regel 1
  &disp_vent_str, &disp_space_10_L, &disp_add_nr_val, // regel 2
  &disp_n_low_str, 0                                  // regel 3
};
static void * const lcd_vent_igbt_fault_al[] =
{
  &disp_vent_al, &disp_alarm,                         // icoon
  &disp_alarm_str,                                    // regel 1
  &disp_vent_str, &disp_space_10_L, &disp_add_nr_val, // regel 2
  &disp_igbt_fault_str, 0                             // regel 3
};
static void * const lcd_vent_uzk_hi_al[] =
{
  &disp_vent_al, &disp_alarm,                         // icoon
  &disp_alarm_str,                                    // regel 1
  &disp_vent_str, &disp_space_10_L, &disp_add_nr_val, // regel 2
  &disp_uzk_hi_str, 0                                 // regel 3
};
static void * const lcd_vent_uzk_lo_al[] =
{
  &disp_vent_al, &disp_alarm,                         // icoon
  &disp_alarm_str,                                    // regel 1
  &disp_vent_str, &disp_space_10_L, &disp_add_nr_val, // regel 2
  &disp_uzk_lo_str, 0                                 // regel 3
};
static void * const lcd_vent_uin_hi_al[] =
{
  &disp_vent_al, &disp_alarm,                         // icoon
  &disp_alarm_str,                                    // regel 1
  &disp_vent_str, &disp_space_10_L, &disp_add_nr_val, // regel 2
  &disp_uin_hi_str, 0                                 // regel 3
};
static void * const lcd_vent_uin_lo_al[] =
{
  &disp_vent_al, &disp_alarm,                         // icoon
  &disp_alarm_str,                                    // regel 1
  &disp_vent_str, &disp_space_10_L, &disp_add_nr_val, // regel 2
  &disp_uin_lo_str, 0                                 // regel 3
};
static void * const lcd_vent_failure_power_section_al[] =
{
  &disp_vent_al, &disp_alarm,                         // icoon
  &disp_alarm_str,                                    // regel 1
  &disp_vent_str, &disp_space_10_L, &disp_add_nr_val, // regel 2
  &disp_failure_power_section_str, 0                  // regel 3
};
static void * const lcd_vent_umax_al[] =
{
  &disp_vent_al, &disp_alarm,                         // icoon
  &disp_alarm_str,                                    // regel 1
  &disp_vent_str, &disp_space_10_L, &disp_add_nr_val, // regel 2
  &disp_umax_str, 0                                   // regel 3
};
static void * const lcd_vent_umin_al[] =
{
  &disp_vent_al, &disp_alarm,                         // icoon
  &disp_alarm_str,                                    // regel 1
  &disp_vent_str, &disp_space_10_L, &disp_add_nr_val, // regel 2
  &disp_umin_str, 0                                   // regel 3
};
static void * const lcd_vent_locked_rotor_al[] =
{
  &disp_vent_al, &disp_alarm,                         // icoon
  &disp_alarm_str,                                    // regel 1
  &disp_vent_str, &disp_space_10_L, &disp_add_nr_val, // regel 2
  &disp_locked_rotor_str, 0                           // regel 3
};
static void * const lcd_vent_overspeed_al[] =
{
  &disp_vent_al, &disp_alarm,                         // icoon
  &disp_alarm_str,                                    // regel 1
  &disp_vent_str, &disp_space_10_L, &disp_add_nr_val, // regel 2
  &disp_overspeed_str, 0                              // regel 3
};
static void * const lcd_vent_underspeed_al[] =
{
  &disp_vent_al, &disp_alarm,                         // icoon
  &disp_alarm_str,                                    // regel 1
  &disp_vent_str, &disp_space_10_L, &disp_add_nr_val, // regel 2
  &disp_underspeed_str, 0                             // regel 3
};
static void * const lcd_vent_24V_supply_overloaded_al[] =
{
  &disp_vent_al, &disp_alarm,                         // icoon
  &disp_alarm_str,                                    // regel 1
  &disp_vent_str, &disp_space_10_L, &disp_add_nr_val, // regel 2
  &disp_24V_supply_overloaded_str, 0                  // regel 3
};
static void * const lcd_vent_input_phase_error_al[] =
{
  &disp_vent_al, &disp_alarm,                         // icoon
  &disp_alarm_str,                                    // regel 1
  &disp_vent_str, &disp_space_10_L, &disp_add_nr_val, // regel 2
  &disp_input_phase_error_str, 0                      // regel 3
};
static void * const lcd_vent_motor_phase_error_al[] =
{
  &disp_vent_al, &disp_alarm,                         // icoon
  &disp_alarm_str,                                    // regel 1
  &disp_vent_str, &disp_space_10_L, &disp_add_nr_val, // regel 2
  &disp_motor_phase_error_str, 0                      // regel 3
};
static void * const lcd_vent_memory_error_al[] =
{
  &disp_vent_al, &disp_alarm,                         // icoon
  &disp_alarm_str,                                    // regel 1
  &disp_vent_str, &disp_space_10_L, &disp_add_nr_val, // regel 2
  &disp_memory_error_str, 0                           // regel 3
};
static void * const lcd_vent_short_circuit_al[] =
{
  &disp_vent_al, &disp_alarm,                         // icoon
  &disp_alarm_str,                                    // regel 1
  &disp_vent_str, &disp_space_10_L, &disp_add_nr_val, // regel 2
  &disp_short_circuit_str, 0                          // regel 3
};
 static void * const lcd_vent_loss_of_synchronism_al[] =
{
  &disp_vent_al, &disp_alarm,                         // icoon
  &disp_alarm_str,                                    // regel 1
  &disp_vent_str, &disp_space_10_L, &disp_add_nr_val, // regel 2
  &disp_loss_of_synchronism_str, 0                    // regel 3
};
static void * const lcd_vent_input_voltage_error_al[] =
{
  &disp_vent_al, &disp_alarm,                         // icoon
  &disp_alarm_str,                                    // regel 1
  &disp_vent_str, &disp_space_10_L, &disp_add_nr_val, // regel 2
  &disp_input_voltage_error_str, 0                    // regel 3
};
static void * const lcd_vent_input_relay_not_closed_al[] =
{
  &disp_vent_al, &disp_alarm,                         // icoon
  &disp_alarm_str,                                    // regel 1
  &disp_vent_str, &disp_space_10_L, &disp_add_nr_val, // regel 2
  &disp_input_relay_not_closed_str, 0                 // regel 3
};
static void * const lcd_vent_high_starting_current_al[] =
{
  &disp_vent_al, &disp_alarm,                         // icoon
  &disp_alarm_str,                                    // regel 1
  &disp_vent_str, &disp_space_10_L, &disp_add_nr_val, // regel 2
  &disp_high_starting_current_str, 0                  // regel 3
};
static void * const lcd_vent_general_error_al[] =
{
  &disp_vent_al, &disp_alarm,                         // icoon
  &disp_alarm_str,                                    // regel 1
  &disp_vent_str, &disp_space_10_L, &disp_add_nr_val, // regel 2
  &disp_general_error_str, 0                          // regel 3
};
static void * const lcd_vent_motor_fault_al[] =
{
  &disp_vent_al, &disp_alarm,                         // icoon
  &disp_alarm_str,                                    // regel 1
  &disp_vent_str, &disp_space_10_L, &disp_add_nr_val, // regel 2
  &disp_motor_fault_str, 0                            // regel 3
};
static void * const lcd_vent_motor_blocked_al[] =
{
  &disp_vent_al, &disp_alarm,                         // icoon
  &disp_alarm_str,                                    // regel 1
  &disp_vent_str, &disp_space_10_L, &disp_add_nr_val, // regel 2
  &disp_motor_blocked_str, 0                          // regel 3
};
static void * const lcd_vent_heat_sink_temperature_al[] =
{
  &disp_vent_al, &disp_alarm,                         // icoon
  &disp_alarm_str,                                    // regel 1
  &disp_vent_str, &disp_space_10_L, &disp_add_nr_val, // regel 2
  &disp_heat_sink_temperature_str, 0                  // regel 3
};
static void * const lcd_vent_ground_fault_al[] =
{
  &disp_vent_al, &disp_alarm,                         // icoon
  &disp_alarm_str,                                    // regel 1
  &disp_vent_str, &disp_space_10_L, &disp_add_nr_val, // regel 2
  &disp_ground_fault_str, 0                           // regel 3
};
static void * const lcd_vent_hall_ic_fault_al[] =
{
  &disp_vent_al, &disp_alarm,                         // icoon
  &disp_alarm_str,                                    // regel 1
  &disp_vent_str, &disp_space_10_L, &disp_add_nr_val, // regel 2
  &disp_hall_ic_fault_str, 0                          // regel 3
};
static void * const lcd_vent_overcurrent_al[] =
{
  &disp_vent_al, &disp_alarm,                         // icoon
  &disp_alarm_str,                                    // regel 1
  &disp_vent_str, &disp_space_10_L, &disp_add_nr_val, // regel 2
  &disp_overcurrent_str, 0                            // regel 3
};
static void * const lcd_vent_line_fault_al[] =
{
  &disp_vent_al, &disp_alarm,                         // icoon
  &disp_alarm_str,                                    // regel 1
  &disp_vent_str, &disp_space_10_L, &disp_add_nr_val, // regel 2
  &disp_line_fault_str, 0                             // regel 3
};
static void * const lcd_vent_line_int_heat_sink_sensor_al[] =
{
  &disp_vent_al, &disp_alarm,                         // icoon
  &disp_alarm_str,                                    // regel 1
  &disp_vent_str, &disp_space_10_L, &disp_add_nr_val, // regel 2
  &disp_int_heat_sink_sensor_str, 0                   // regel 3
};
static void * const lcd_vent_dc_res_voltage_to_high_al[] =
{
  &disp_vent_al, &disp_alarm,                         // icoon
  &disp_alarm_str,                                    // regel 1
  &disp_vent_str, &disp_space_10_L, &disp_add_nr_val, // regel 2
  &disp_dc_res_voltage_to_high_str, 0                 // regel 3
};
static void * const lcd_vent_wrong_direction_al[] =
{
  &disp_vent_al, &disp_alarm,                         // icoon
  &disp_alarm_str,                                    // regel 1
  &disp_vent_str, &disp_space_10_L, &disp_add_nr_val, // regel 2
  &disp_wrong_direction_str, 0                        // regel 3
};
static void * const lcd_vent_temperature_lowering_al[] =
{
  &disp_vent_al, &disp_alarm,                         // icoon
  &disp_alarm_str,                                    // regel 1
  &disp_vent_str, &disp_space_10_L, &disp_add_nr_val, // regel 2
  &disp_temperature_lowering_str, 0                   // regel 3
};
static void * const lcd_vent_wrong_connection_al[] =
{
  &disp_vent_al, &disp_alarm,                         // icoon
  &disp_alarm_str,                                    // regel 1
  &disp_vent_str, &disp_space_10_L, &disp_add_nr_val, // regel 2
  &disp_wrong_connection_str, 0                       // regel 3
};
static void * const lcd_vent_external_fault_al[] =
{
  &disp_vent_al, &disp_alarm,                         // icoon
  &disp_alarm_str,                                    // regel 1
  &disp_vent_str, &disp_space_10_L, &disp_add_nr_val, // regel 2
  &disp_external_fault_str, 0                         // regel 3
};
static void * const lcd_vent_factory_settings_al[] =
{
  &disp_vent_al, &disp_alarm,                         // icoon
  &disp_alarm_str,                                    // regel 1
  &disp_vent_str, &disp_space_10_L, &disp_add_nr_val, // regel 2
  &disp_factory_settings_str, 0                       // regel 3
};
static void * const lcd_vent_eep_error_al[] =
{
  &disp_vent_al, &disp_alarm,                         // icoon
  &disp_alarm_str,                                    // regel 1
  &disp_vent_str, &disp_space_10_L, &disp_add_nr_val, // regel 2
  &disp_eep_error_str, 0                              // regel 3
};
static void * const lcd_vent_rtc_general_fault_al[] =
{
  &disp_vent_al, &disp_alarm,                         // icoon
  &disp_alarm_str,                                    // regel 1
  &disp_vent_str, &disp_space_10_L, &disp_add_nr_val, // regel 2
  &disp_rtc_general_fault_str, 0                      // regel 3
};
static void * const lcd_vent_rtc_voltage_fault_al[] =
{
  &disp_vent_al, &disp_alarm,                         // icoon
  &disp_alarm_str,                                    // regel 1
  &disp_vent_str, &disp_space_10_L, &disp_add_nr_val, // regel 2
  &disp_rtc_voltage_fault_str, 0                      // regel 3
};
static void * const lcd_vent_filter_contamination_al[] =
{
  &disp_vent_al, &disp_alarm,                         // icoon
  &disp_alarm_str,                                    // regel 1
  &disp_vent_str, &disp_space_10_L, &disp_add_nr_val, // regel 2
  &disp_filter_contamination_str, 0                   // regel 3
};
static void * const lcd_vent_transfer_error_al[] =
{
  &disp_vent_al, &disp_alarm,                         // icoon
  &disp_alarm_str,                                    // regel 1
  &disp_vent_str, &disp_space_10_L, &disp_add_nr_val, // regel 2
  &disp_transfer_error_str, 0                         // regel 3
};
static void * const lcd_vent_data_connection_line_al[] =
{
  &disp_vent_al, &disp_alarm,                         // icoon
  &disp_alarm_str,                                    // regel 1
  &disp_vent_str, &disp_space_10_L, &disp_add_nr_val, // regel 2
  &disp_data_connection_line_str, 0                   // regel 3
};
static void * const lcd_vent_data_connection_checksum_al[] =
{
  &disp_vent_al, &disp_alarm,                         // icoon
  &disp_alarm_str,                                    // regel 1
  &disp_vent_str, &disp_space_10_L, &disp_add_nr_val, // regel 2
  &disp_data_connection_checksum_str, 0               // regel 3
};
static void * const lcd_vent_sensor_fault_input_1_al[] =
{
  &disp_vent_al, &disp_alarm,                         // icoon
  &disp_alarm_str,                                    // regel 1
  &disp_vent_str, &disp_space_10_L, &disp_add_nr_val, // regel 2
  &disp_sensor_fault_input_1_str, 0                   // regel 3
};
static void * const lcd_vent_sensor_fault_input_2_al[] =
{
  &disp_vent_al, &disp_alarm,                         // icoon
  &disp_alarm_str,                                    // regel 1
  &disp_vent_str, &disp_space_10_L, &disp_add_nr_val, // regel 2
  &disp_sensor_fault_input_2_str, 0                   // regel 3
};
static void * const lcd_vent_sensor_fault_input_3_al[] =
{
  &disp_vent_al, &disp_alarm,                         // icoon
  &disp_alarm_str,                                    // regel 1
  &disp_vent_str, &disp_space_10_L, &disp_add_nr_val, // regel 2
  &disp_sensor_fault_input_3_str, 0                   // regel 3
};









//-----------------------------------------------------------------------------------
static void * const lcd_luchtmengkastgroep_manual_al[] =
{
  &disp_luchtmengkast_al, &disp_alarm,                              // icoon
  &disp_waarschuwing_str,                                           // regel 1
  &disp_luchtmengkastgroep_str, &disp_space_10_L, &disp_add_nr_val, // regel 2
  &disp_handbediening_str, 0                                        // regel 3
};
static void * const lcd_luchtmengkast_manual_al[] =
{
  &disp_luchtmengkast_al, &disp_alarm,                                                                        // icoon
  &disp_waarschuwing_str,                                                                                     // regel 1
  &disp_luchtmengkast_2_str, &disp_rechte_openings_haak_10_L, &disp_add_nr_val, &disp_rechte_sluit_haak_10_L, // regel 2
  &disp_handbediening_str, 0                                                                                  // regel 3
};
static void * const lcd_luchtmengkast_vorst_al[] =
{
  &disp_luchtmengkast_al, &disp_alarm,                                                                        // icoon
  &disp_alarm_str,                                                                                            // regel 1
  &disp_luchtmengkast_2_str, &disp_rechte_openings_haak_10_L, &disp_add_nr_val, &disp_rechte_sluit_haak_10_L, // regel 2
  &disp_vorstbeveiliging_str, 0                                                                               // regel 3
};
static void * const lcd_luchtmengkast_extern_al[] =
{
  &disp_luchtmengkast_al, &disp_alarm,                                                                        // icoon
  &disp_alarm_str,                                                                                            // regel 1
  &disp_luchtmengkast_2_str, &disp_rechte_openings_haak_10_L, &disp_add_nr_val, &disp_rechte_sluit_haak_10_L, // regel 2
  &disp_algemeen_extern_alarm_str, 0                                                                          // regel 3
};
static void * const lcd_luchtmengkast_drukverschil_al[] =
{
  &disp_luchtmengkast_al, &disp_alarm,                                                                        // icoon
  &disp_alarm_str,                                                                                            // regel 1
  &disp_luchtmengkast_2_str, &disp_rechte_openings_haak_10_L, &disp_add_nr_val, &disp_rechte_sluit_haak_10_L, // regel 2
  &disp_drukverschil_bewaking_str, 0                                                                          // regel 3
};
static void * const lcd_luchtmengkast_recircklep_target_not_reached_al[] =
{
  &disp_luchtmengkast_al, &disp_alarm,                                                                        // icoon
  &disp_luchtmengkast_1_str, &disp_rechte_openings_haak_10_L, &disp_add_nr_val, &disp_rechte_sluit_haak_10_L, // regel 1
  &disp_recirculatieklep_str,                                                                                 // regel 2
  &disp_position_not_reached_str, 0                                                                           // regel 3
};
static void * const lcd_luchtmengkast_buitenklep_target_not_reached_al[] =
{
  &disp_luchtmengkast_al, &disp_alarm,                                                                        // icoon
  &disp_luchtmengkast_1_str, &disp_rechte_openings_haak_10_L, &disp_add_nr_val, &disp_rechte_sluit_haak_10_L, // regel 1
  &disp_buitenklep_str,                                                                                       // regel 2
  &disp_position_not_reached_str, 0                                                                           // regel 3
};
static void * const lcd_luchtmengkast_binnenklep_target_not_reached_al[] =
{
  &disp_luchtmengkast_al, &disp_alarm,                                                                        // icoon
  &disp_luchtmengkast_1_str, &disp_rechte_openings_haak_10_L, &disp_add_nr_val, &disp_rechte_sluit_haak_10_L, // regel 1
  &disp_binnenklep_str,                                                                                       // regel 2
  &disp_position_not_reached_str, 0                                                                           // regel 3
};
static void * const lcd_luchtmengkast_bovenklep_target_not_reached_al[] =
{
  &disp_luchtmengkast_al, &disp_alarm,                                                                        // icoon
  &disp_luchtmengkast_1_str, &disp_rechte_openings_haak_10_L, &disp_add_nr_val, &disp_rechte_sluit_haak_10_L, // regel 1
  &disp_bovenklep_str,                                                                                        // regel 2
  &disp_position_not_reached_str, 0                                                                           // regel 3
};
static void * const lcd_luchtmengkast_inblaasvent_target_not_reached_al[] =
{
  &disp_luchtmengkast_al, &disp_alarm,                                                                        // icoon
  &disp_luchtmengkast_1_str, &disp_rechte_openings_haak_10_L, &disp_add_nr_val, &disp_rechte_sluit_haak_10_L, // regel 1
  &disp_inblaasvent_str,                                                                                     // regel 2
  &disp_position_not_reached_str, 0                                                                           // regel 3
};
static void * const lcd_luchtmengkast_afblaasvent_target_not_reached_al[] =
{
  &disp_luchtmengkast_al, &disp_alarm,                                                                        // icoon
  &disp_luchtmengkast_1_str, &disp_rechte_openings_haak_10_L, &disp_add_nr_val, &disp_rechte_sluit_haak_10_L, // regel 1
  &disp_afblaasvent_str,                                                                                       // regel 2
  &disp_position_not_reached_str, 0                                                                           // regel 3
};
static void * const lcd_luchtmengkast_verwarming_target_not_reached_al[] =
{
  &disp_luchtmengkast_al, &disp_alarm,                                                                        // icoon
  &disp_luchtmengkast_1_str, &disp_rechte_openings_haak_10_L, &disp_add_nr_val, &disp_rechte_sluit_haak_10_L, // regel 1
  &disp_verwarming_str,                                                                                       // regel 2
  &disp_position_not_reached_str, 0                                                                           // regel 3
};

static void * const lcd_luchtmengkast_inblaasvent_communication_al[] =
{
  &disp_luchtmengkast_al, &disp_alarm,                                                                        // icoon
  &disp_luchtmengkast_1_str, &disp_rechte_openings_haak_10_L, &disp_add_nr_val, &disp_rechte_sluit_haak_10_L, // regel 1
  &disp_inblaasvent_str,                                                                                      // regel 2
  &disp_niet_gevonden_str, 0                                                                                  // regel 3
};
static void * const lcd_luchtmengkast_inblaasvent_locked_motor_al[] =
{
  &disp_luchtmengkast_al, &disp_alarm,                                                                        // icoon
  &disp_luchtmengkast_1_str, &disp_rechte_openings_haak_10_L, &disp_add_nr_val, &disp_rechte_sluit_haak_10_L, // regel 1
  &disp_inblaasvent_str,                                                                                      // regel 2
  &disp_locked_motor_str, 0                                                                                   // regel 3
};
static void * const lcd_luchtmengkast_inblaasvent_hall_failure_al[] =
{
  &disp_luchtmengkast_al, &disp_alarm,                                                                        // icoon
  &disp_luchtmengkast_1_str, &disp_rechte_openings_haak_10_L, &disp_add_nr_val, &disp_rechte_sluit_haak_10_L, // regel 1
  &disp_inblaasvent_str,                                                                                      // regel 2
  &disp_hall_failure_str, 0                                                                                   // regel 3
};
static void * const lcd_luchtmengkast_inblaasvent_thermal_motor_al[] =
{
  &disp_luchtmengkast_al, &disp_alarm,                                                                        // icoon
  &disp_luchtmengkast_1_str, &disp_rechte_openings_haak_10_L, &disp_add_nr_val, &disp_rechte_sluit_haak_10_L, // regel 1
  &disp_inblaasvent_str,                                                                                      // regel 2
  &disp_thermal_motor_str, 0                                                                                  // regel 3
};
static void * const lcd_luchtmengkast_inblaasvent_comm_error_master_slave_al[] =
{
  &disp_luchtmengkast_al, &disp_alarm,                                                                        // icoon
  &disp_luchtmengkast_1_str, &disp_rechte_openings_haak_10_L, &disp_add_nr_val, &disp_rechte_sluit_haak_10_L, // regel 1
  &disp_inblaasvent_str,                                                                                      // regel 2
  &disp_comm_error_master_slave_str, 0                                                                        // regel 3
};
static void * const lcd_luchtmengkast_inblaasvent_thermal_power_module_al[] =
{
  &disp_luchtmengkast_al, &disp_alarm,                                                                        // icoon
  &disp_luchtmengkast_1_str, &disp_rechte_openings_haak_10_L, &disp_add_nr_val, &disp_rechte_sluit_haak_10_L, // regel 1
  &disp_inblaasvent_str,                                                                                      // regel 2
  &disp_thermal_power_module_str, 0                                                                           // regel 3
};
static void * const lcd_luchtmengkast_inblaasvent_comm_error_remote_unit_al[] =
{
  &disp_luchtmengkast_al, &disp_alarm,                                                                        // icoon
  &disp_luchtmengkast_1_str, &disp_rechte_openings_haak_10_L, &disp_add_nr_val, &disp_rechte_sluit_haak_10_L, // regel 1
  &disp_inblaasvent_str,                                                                                      // regel 2
  &disp_comm_error_remote_unit_str, 0                                                                         // regel 3
};
static void * const lcd_luchtmengkast_inblaasvent_phase_failure_al[] =
{
  &disp_luchtmengkast_al, &disp_alarm,                                                                        // icoon
  &disp_luchtmengkast_1_str, &disp_rechte_openings_haak_10_L, &disp_add_nr_val, &disp_rechte_sluit_haak_10_L, // regel 1
  &disp_inblaasvent_str,                                                                                      // regel 2
  &disp_phase_failure_str, 0                                                                                  // regel 3
};
static void * const lcd_luchtmengkast_inblaasvent_brake_al[] =
{
  &disp_luchtmengkast_al, &disp_alarm,                                                                        // icoon
  &disp_luchtmengkast_1_str, &disp_rechte_openings_haak_10_L, &disp_add_nr_val, &disp_rechte_sluit_haak_10_L, // regel 1
  &disp_inblaasvent_str,                                                                                      // regel 2
  &disp_brake_str, 0                                                                                          // regel 3
};
static void * const lcd_luchtmengkast_inblaasvent_high_line_voltage_al[] =
{
  &disp_luchtmengkast_al, &disp_alarm,                                                                        // icoon
  &disp_luchtmengkast_1_str, &disp_rechte_openings_haak_10_L, &disp_add_nr_val, &disp_rechte_sluit_haak_10_L, // regel 1
  &disp_inblaasvent_str,                                                                                      // regel 2
  &disp_high_line_voltage_str, 0                                                                              // regel 3
};
static void * const lcd_luchtmengkast_inblaasvent_low_line_voltage_al[] =
{
  &disp_luchtmengkast_al, &disp_alarm,                                                                        // icoon
  &disp_luchtmengkast_1_str, &disp_rechte_openings_haak_10_L, &disp_add_nr_val, &disp_rechte_sluit_haak_10_L, // regel 1
  &disp_inblaasvent_str,                                                                                      // regel 2
  &disp_low_line_voltage_str, 0                                                                               // regel 3
};
static void * const lcd_luchtmengkast_inblaasvent_low_dc_link_voltage_al[] =
{
  &disp_luchtmengkast_al, &disp_alarm,                                                                        // icoon
  &disp_luchtmengkast_1_str, &disp_rechte_openings_haak_10_L, &disp_add_nr_val, &disp_rechte_sluit_haak_10_L, // regel 1
  &disp_inblaasvent_str,                                                                                      // regel 2
  &disp_low_dc_link_voltage_str, 0                                                                            // regel 3
};
static void * const lcd_luchtmengkast_inblaasvent_high_dc_link_voltage_al[] =
{
  &disp_luchtmengkast_al, &disp_alarm,                                                                        // icoon
  &disp_luchtmengkast_1_str, &disp_rechte_openings_haak_10_L, &disp_add_nr_val, &disp_rechte_sluit_haak_10_L, // regel 1
  &disp_inblaasvent_str,                                                                                      // regel 2
  &disp_high_dc_link_voltage_str, 0                                                                           // regel 3
};
static void * const lcd_luchtmengkast_inblaasvent_driver_problem_al[] =
{
  &disp_luchtmengkast_al, &disp_alarm,                                                                        // icoon
  &disp_luchtmengkast_1_str, &disp_rechte_openings_haak_10_L, &disp_add_nr_val, &disp_rechte_sluit_haak_10_L, // regel 1
  &disp_inblaasvent_str,                                                                                      // regel 2
  &disp_driver_problem_str, 0                                                                                 // regel 3
};
static void * const lcd_luchtmengkast_inblaasvent_electronic_box_over_heat_al[] =
{
  &disp_luchtmengkast_al, &disp_alarm,                                                                        // icoon
  &disp_luchtmengkast_1_str, &disp_rechte_openings_haak_10_L, &disp_add_nr_val, &disp_rechte_sluit_haak_10_L, // regel 1
  &disp_inblaasvent_str,                                                                                      // regel 2
  &disp_electronic_box_over_heat_str, 0                                                                       // regel 3
};
static void * const lcd_luchtmengkast_inblaasvent_excessive_dc_link_current_al[] =
{
  &disp_luchtmengkast_al, &disp_alarm,                                                                        // icoon
  &disp_luchtmengkast_1_str, &disp_rechte_openings_haak_10_L, &disp_add_nr_val, &disp_rechte_sluit_haak_10_L, // regel 1
  &disp_inblaasvent_str,                                                                                      // regel 2
  &disp_excessive_dc_link_current_str, 0                                                                      // regel 3
};
static void * const lcd_luchtmengkast_inblaasvent_unknown_al[] =
{
  &disp_luchtmengkast_al, &disp_alarm,                                                                        // icoon
  &disp_luchtmengkast_1_str, &disp_rechte_openings_haak_10_L, &disp_add_nr_val, &disp_rechte_sluit_haak_10_L, // regel 1
  &disp_inblaasvent_str,                                                                                      // regel 2
  &disp_onbekend_alarm_3_str, 0                                                                               // regel 3
};
static void * const lcd_luchtmengkast_inblaasvent_i_limit_al[] =
{
  &disp_luchtmengkast_al, &disp_alarm,                                                                        // icoon
  &disp_luchtmengkast_1_str, &disp_rechte_openings_haak_10_L, &disp_add_nr_val, &disp_rechte_sluit_haak_10_L, // regel 1
  &disp_inblaasvent_str,                                                                                      // regel 2
  &disp_i_limit_str, 0                                // regel 3
};
static void * const lcd_luchtmengkast_inblaasvent_p_limit_al[] =
{
  &disp_luchtmengkast_al, &disp_alarm,                                                                        // icoon
  &disp_luchtmengkast_1_str, &disp_rechte_openings_haak_10_L, &disp_add_nr_val, &disp_rechte_sluit_haak_10_L, // regel 1
  &disp_inblaasvent_str,                                                                                      // regel 2
  &disp_p_limit_str, 0                                // regel 3
};
static void * const lcd_luchtmengkast_inblaasvent_te_high_al[] =
{
  &disp_luchtmengkast_al, &disp_alarm,                                                                        // icoon
  &disp_luchtmengkast_1_str, &disp_rechte_openings_haak_10_L, &disp_add_nr_val, &disp_rechte_sluit_haak_10_L, // regel 1
  &disp_inblaasvent_str,                                                                                      // regel 2
  &disp_te_high_str, 0                                // regel 3
};
static void * const lcd_luchtmengkast_inblaasvent_tm_high_al[] =
{
  &disp_luchtmengkast_al, &disp_alarm,                                                                        // icoon
  &disp_luchtmengkast_1_str, &disp_rechte_openings_haak_10_L, &disp_add_nr_val, &disp_rechte_sluit_haak_10_L, // regel 1
  &disp_inblaasvent_str,                                                                                      // regel 2
  &disp_tm_high_str, 0                                // regel 3
};
static void * const lcd_luchtmengkast_inblaasvent_tei_high_al[] =
{
  &disp_luchtmengkast_al, &disp_alarm,                                                                        // icoon
  &disp_luchtmengkast_1_str, &disp_rechte_openings_haak_10_L, &disp_add_nr_val, &disp_rechte_sluit_haak_10_L, // regel 1
  &disp_inblaasvent_str,                                                                                      // regel 2
  &disp_tei_high_str, 0                               // regel 3
};
static void * const lcd_luchtmengkast_inblaasvent_uz_low_al[] =
{
  &disp_luchtmengkast_al, &disp_alarm,                                                                        // icoon
  &disp_luchtmengkast_1_str, &disp_rechte_openings_haak_10_L, &disp_add_nr_val, &disp_rechte_sluit_haak_10_L, // regel 1
  &disp_inblaasvent_str,                                                                                      // regel 2
  &disp_uz_low_str, 0                                 // regel 3
};
static void * const lcd_luchtmengkast_inblaasvent_n_low_al[] =
{
  &disp_luchtmengkast_al, &disp_alarm,                                                                        // icoon
  &disp_luchtmengkast_1_str, &disp_rechte_openings_haak_10_L, &disp_add_nr_val, &disp_rechte_sluit_haak_10_L, // regel 1
  &disp_inblaasvent_str,                                                                                      // regel 2
  &disp_n_low_str, 0                                  // regel 3
};
static void * const lcd_luchtmengkast_inblaasvent_igbt_fault_al[] =
{
  &disp_luchtmengkast_al, &disp_alarm,                                                                        // icoon
  &disp_luchtmengkast_1_str, &disp_rechte_openings_haak_10_L, &disp_add_nr_val, &disp_rechte_sluit_haak_10_L, // regel 1
  &disp_inblaasvent_str,                                                                                      // regel 2
  &disp_igbt_fault_str, 0                                                                                     // regel 3
};
static void * const lcd_luchtmengkast_inblaasvent_uzk_hi_al[] =
{
  &disp_luchtmengkast_al, &disp_alarm,                                                                        // icoon
  &disp_luchtmengkast_1_str, &disp_rechte_openings_haak_10_L, &disp_add_nr_val, &disp_rechte_sluit_haak_10_L, // regel 1
  &disp_inblaasvent_str,                                                                                      // regel 2
  &disp_uzk_hi_str, 0                                                                                         // regel 3
};
static void * const lcd_luchtmengkast_inblaasvent_uzk_lo_al[] =
{
  &disp_luchtmengkast_al, &disp_alarm,                                                                        // icoon
  &disp_luchtmengkast_1_str, &disp_rechte_openings_haak_10_L, &disp_add_nr_val, &disp_rechte_sluit_haak_10_L, // regel 1
  &disp_inblaasvent_str,                                                                                      // regel 2
  &disp_uzk_lo_str, 0                                                                                         // regel 3
};
static void * const lcd_luchtmengkast_inblaasvent_uin_hi_al[] =
{
  &disp_luchtmengkast_al, &disp_alarm,                                                                        // icoon
  &disp_luchtmengkast_1_str, &disp_rechte_openings_haak_10_L, &disp_add_nr_val, &disp_rechte_sluit_haak_10_L, // regel 1
  &disp_inblaasvent_str,                                                                                      // regel 2
  &disp_uin_hi_str, 0                                                                                         // regel 3
};
static void * const lcd_luchtmengkast_inblaasvent_uin_lo_al[] =
{
  &disp_luchtmengkast_al, &disp_alarm,                                                                        // icoon
  &disp_luchtmengkast_1_str, &disp_rechte_openings_haak_10_L, &disp_add_nr_val, &disp_rechte_sluit_haak_10_L, // regel 1
  &disp_inblaasvent_str,                                                                                      // regel 2
  &disp_uin_lo_str, 0                                                                                         // regel 3
};
static void * const lcd_luchtmengkast_inblaasvent_failure_power_section_al[] =
{
  &disp_luchtmengkast_al, &disp_alarm,                                                                        // icoon
  &disp_luchtmengkast_1_str, &disp_rechte_openings_haak_10_L, &disp_add_nr_val, &disp_rechte_sluit_haak_10_L, // regel 1
  &disp_inblaasvent_str,                                                                                      // regel 2
  &disp_failure_power_section_str, 0                                                                          // regel 3
};
static void * const lcd_luchtmengkast_inblaasvent_umax_al[] =
{
  &disp_luchtmengkast_al, &disp_alarm,                                                                        // icoon
  &disp_luchtmengkast_1_str, &disp_rechte_openings_haak_10_L, &disp_add_nr_val, &disp_rechte_sluit_haak_10_L, // regel 1
  &disp_inblaasvent_str,                                                                                      // regel 2
  &disp_umax_str, 0                                                                                           // regel 3
};
static void * const lcd_luchtmengkast_inblaasvent_umin_al[] =
{
  &disp_luchtmengkast_al, &disp_alarm,                                                                        // icoon
  &disp_luchtmengkast_1_str, &disp_rechte_openings_haak_10_L, &disp_add_nr_val, &disp_rechte_sluit_haak_10_L, // regel 1
  &disp_inblaasvent_str,                                                                                      // regel 2
  &disp_umin_str, 0                                                                                           // regel 3
};
static void * const lcd_luchtmengkast_inblaasvent_overspeed_al[] =
{
  &disp_luchtmengkast_al, &disp_alarm,                                                                        // icoon
  &disp_luchtmengkast_1_str, &disp_rechte_openings_haak_10_L, &disp_add_nr_val, &disp_rechte_sluit_haak_10_L, // regel 1
  &disp_inblaasvent_str,                                                                                      // regel 2
  &disp_overspeed_str, 0                                                                                      // regel 3
};
static void * const lcd_luchtmengkast_inblaasvent_locked_rotor_al[] =
{
  &disp_luchtmengkast_al, &disp_alarm,                                                                        // icoon
  &disp_luchtmengkast_1_str, &disp_rechte_openings_haak_10_L, &disp_add_nr_val, &disp_rechte_sluit_haak_10_L, // regel 1
  &disp_inblaasvent_str,                                                                                      // regel 2
  &disp_locked_rotor_str, 0                                                                                   // regel 3
};
static void * const lcd_luchtmengkast_inblaasvent_underspeed_al[] =
{
  &disp_luchtmengkast_al, &disp_alarm,                                                                        // icoon
  &disp_luchtmengkast_1_str, &disp_rechte_openings_haak_10_L, &disp_add_nr_val, &disp_rechte_sluit_haak_10_L, // regel 1
  &disp_inblaasvent_str,                                                                                      // regel 2
  &disp_underspeed_str, 0                                                                                     // regel 3
};
static void * const lcd_luchtmengkast_inblaasvent_24V_supply_overloaded_al[] =
{
  &disp_luchtmengkast_al, &disp_alarm,                                                                        // icoon
  &disp_luchtmengkast_1_str, &disp_rechte_openings_haak_10_L, &disp_add_nr_val, &disp_rechte_sluit_haak_10_L, // regel 1
  &disp_inblaasvent_str,                                                                                      // regel 2
  &disp_24V_supply_overloaded_str, 0                                                                          // regel 3
};
static void * const lcd_luchtmengkast_inblaasvent_input_phase_error_al[] =
{
  &disp_luchtmengkast_al, &disp_alarm,                                                                        // icoon
  &disp_luchtmengkast_1_str, &disp_rechte_openings_haak_10_L, &disp_add_nr_val, &disp_rechte_sluit_haak_10_L, // regel 1
  &disp_inblaasvent_str,                                                                                      // regel 2
  &disp_input_phase_error_str, 0                                                                              // regel 3
};
static void * const lcd_luchtmengkast_inblaasvent_motor_phase_error_al[] =
{
  &disp_luchtmengkast_al, &disp_alarm,                                                                        // icoon
  &disp_luchtmengkast_1_str, &disp_rechte_openings_haak_10_L, &disp_add_nr_val, &disp_rechte_sluit_haak_10_L, // regel 1
  &disp_inblaasvent_str,                                                                                      // regel 2
  &disp_motor_phase_error_str, 0                                                                              // regel 3
};									
static void * const lcd_luchtmengkast_inblaasvent_memory_error_al[] =
{
  &disp_luchtmengkast_al, &disp_alarm,                                                                        // icoon
  &disp_luchtmengkast_1_str, &disp_rechte_openings_haak_10_L, &disp_add_nr_val, &disp_rechte_sluit_haak_10_L, // regel 1
  &disp_inblaasvent_str,                                                                                      // regel 2
  &disp_memory_error_str, 0                                                                                   // regel 3
};
static void * const lcd_luchtmengkast_inblaasvent_short_circuit_al[] =
{
  &disp_luchtmengkast_al, &disp_alarm,                                                                        // icoon
  &disp_luchtmengkast_1_str, &disp_rechte_openings_haak_10_L, &disp_add_nr_val, &disp_rechte_sluit_haak_10_L, // regel 1
  &disp_inblaasvent_str,                                                                                      // regel 2
  &disp_short_circuit_str, 0                                                                                  // regel 3
};
static void * const lcd_luchtmengkast_inblaasvent_loss_of_synchronism_al[] =
{
  &disp_luchtmengkast_al, &disp_alarm,                                                                        // icoon
  &disp_luchtmengkast_1_str, &disp_rechte_openings_haak_10_L, &disp_add_nr_val, &disp_rechte_sluit_haak_10_L, // regel 1
  &disp_inblaasvent_str,                                                                                      // regel 2
  &disp_loss_of_synchronism_str, 0                                                                            // regel 3
};
static void * const lcd_luchtmengkast_inblaasvent_input_voltage_error_al[] =
{
  &disp_luchtmengkast_al, &disp_alarm,                                                                        // icoon
  &disp_luchtmengkast_1_str, &disp_rechte_openings_haak_10_L, &disp_add_nr_val, &disp_rechte_sluit_haak_10_L, // regel 1
  &disp_inblaasvent_str,                                                                                      // regel 2
  &disp_input_voltage_error_str, 0                                                                            // regel 3
};
static void * const lcd_luchtmengkast_inblaasvent_input_relay_not_closed_al[] =
{
  &disp_luchtmengkast_al, &disp_alarm,                                                                        // icoon
  &disp_luchtmengkast_1_str, &disp_rechte_openings_haak_10_L, &disp_add_nr_val, &disp_rechte_sluit_haak_10_L, // regel 1
  &disp_inblaasvent_str,                                                                                      // regel 2
  &disp_input_relay_not_closed_str, 0                                                                         // regel 3
};
static void * const lcd_luchtmengkast_inblaasvent_high_starting_current_al[] =
{
  &disp_luchtmengkast_al, &disp_alarm,                                                                        // icoon
  &disp_luchtmengkast_1_str, &disp_rechte_openings_haak_10_L, &disp_add_nr_val, &disp_rechte_sluit_haak_10_L, // regel 1
  &disp_inblaasvent_str,                                                                                      // regel 2
  &disp_high_starting_current_str, 0                                                                          // regel 3
};
static void * const lcd_luchtmengkast_inblaasvent_general_error_al[] =
{
  &disp_luchtmengkast_al, &disp_alarm,                                                                        // icoon
  &disp_luchtmengkast_1_str, &disp_rechte_openings_haak_10_L, &disp_add_nr_val, &disp_rechte_sluit_haak_10_L, // regel 1
  &disp_inblaasvent_str,                                                                                      // regel 2
  &disp_general_error_str, 0                                                                               // regel 3
};
static void * const lcd_luchtmengkast_inblaasvent_motor_fault_al[] =
{
  &disp_luchtmengkast_al, &disp_alarm,                                                                        // icoon
  &disp_luchtmengkast_1_str, &disp_rechte_openings_haak_10_L, &disp_add_nr_val, &disp_rechte_sluit_haak_10_L, // regel 1
  &disp_inblaasvent_str,                                                                                      // regel 2
  &disp_motor_fault_str, 0                                                                               // regel 3
};
static void * const lcd_luchtmengkast_inblaasvent_motor_blocked_al[] =
{
  &disp_luchtmengkast_al, &disp_alarm,                                                                        // icoon
  &disp_luchtmengkast_1_str, &disp_rechte_openings_haak_10_L, &disp_add_nr_val, &disp_rechte_sluit_haak_10_L, // regel 1
  &disp_inblaasvent_str,                                                                                      // regel 2
  &disp_motor_blocked_str, 0                                                                               // regel 3
};
static void * const lcd_luchtmengkast_inblaasvent_heat_sink_temperature_al[] =
{
  &disp_luchtmengkast_al, &disp_alarm,                                                                        // icoon
  &disp_luchtmengkast_1_str, &disp_rechte_openings_haak_10_L, &disp_add_nr_val, &disp_rechte_sluit_haak_10_L, // regel 1
  &disp_inblaasvent_str,                                                                                      // regel 2
  &disp_heat_sink_temperature_str, 0                                                                               // regel 3
};
static void * const lcd_luchtmengkast_inblaasvent_ground_fault_al[] =
{
  &disp_luchtmengkast_al, &disp_alarm,                                                                        // icoon
  &disp_luchtmengkast_1_str, &disp_rechte_openings_haak_10_L, &disp_add_nr_val, &disp_rechte_sluit_haak_10_L, // regel 1
  &disp_inblaasvent_str,                                                                                      // regel 2
  &disp_ground_fault_str, 0                                                                               // regel 3
};
static void * const lcd_luchtmengkast_inblaasvent_hall_ic_fault_al[] =
{
  &disp_luchtmengkast_al, &disp_alarm,                                                                        // icoon
  &disp_luchtmengkast_1_str, &disp_rechte_openings_haak_10_L, &disp_add_nr_val, &disp_rechte_sluit_haak_10_L, // regel 1
  &disp_inblaasvent_str,                                                                                      // regel 2
  &disp_hall_ic_fault_str, 0                                                                               // regel 3
};
static void * const lcd_luchtmengkast_inblaasvent_overcurrent_al[] =
{
  &disp_luchtmengkast_al, &disp_alarm,                                                                        // icoon
  &disp_luchtmengkast_1_str, &disp_rechte_openings_haak_10_L, &disp_add_nr_val, &disp_rechte_sluit_haak_10_L, // regel 1
  &disp_inblaasvent_str,                                                                                      // regel 2
  &disp_overcurrent_str, 0                                                                                    // regel 3
};
static void * const lcd_luchtmengkast_inblaasvent_line_fault_al[] =         
{
  &disp_luchtmengkast_al, &disp_alarm,                                                                        // icoon
  &disp_luchtmengkast_1_str, &disp_rechte_openings_haak_10_L, &disp_add_nr_val, &disp_rechte_sluit_haak_10_L, // regel 1
  &disp_inblaasvent_str,                                                                                      // regel 2
  &disp_line_fault_str, 0                                                                               // regel 3
};
static void * const lcd_luchtmengkast_inblaasvent_line_int_heat_sink_sensor_al[] =
{
  &disp_luchtmengkast_al, &disp_alarm,                                                                        // icoon
  &disp_luchtmengkast_1_str, &disp_rechte_openings_haak_10_L, &disp_add_nr_val, &disp_rechte_sluit_haak_10_L, // regel 1
  &disp_inblaasvent_str,                                                                                      // regel 2
  &disp_int_heat_sink_sensor_str, 0                                                                               // regel 3
};
static void * const lcd_luchtmengkast_inblaasvent_dc_res_voltage_to_high_al[] =
{
  &disp_luchtmengkast_al, &disp_alarm,                                                                        // icoon
  &disp_luchtmengkast_1_str, &disp_rechte_openings_haak_10_L, &disp_add_nr_val, &disp_rechte_sluit_haak_10_L, // regel 1
  &disp_inblaasvent_str,                                                                                      // regel 2
  &disp_dc_res_voltage_to_high_str, 0                                                                               // regel 3
};
static void * const lcd_luchtmengkast_inblaasvent_wrong_direction_al[] =
{
  &disp_luchtmengkast_al, &disp_alarm,                                                                        // icoon
  &disp_luchtmengkast_1_str, &disp_rechte_openings_haak_10_L, &disp_add_nr_val, &disp_rechte_sluit_haak_10_L, // regel 1
  &disp_inblaasvent_str,                                                                                      // regel 2
  &disp_wrong_direction_str, 0                                                                               // regel 3
};
static void * const lcd_luchtmengkast_inblaasvent_temperature_lowering_al[] =
{
  &disp_luchtmengkast_al, &disp_alarm,                                                                        // icoon
  &disp_luchtmengkast_1_str, &disp_rechte_openings_haak_10_L, &disp_add_nr_val, &disp_rechte_sluit_haak_10_L, // regel 1
  &disp_inblaasvent_str,                                                                                      // regel 2
  &disp_temperature_lowering_str, 0                                                                               // regel 3
};
static void * const lcd_luchtmengkast_inblaasvent_wrong_connection_al[] =
{
  &disp_luchtmengkast_al, &disp_alarm,                                                                        // icoon
  &disp_luchtmengkast_1_str, &disp_rechte_openings_haak_10_L, &disp_add_nr_val, &disp_rechte_sluit_haak_10_L, // regel 1
  &disp_inblaasvent_str,                                                                                      // regel 2
  &disp_wrong_connection_str, 0                                                                               // regel 3
};
static void * const lcd_luchtmengkast_inblaasvent_external_fault_al[] =
{
  &disp_luchtmengkast_al, &disp_alarm,                                                                        // icoon
  &disp_luchtmengkast_1_str, &disp_rechte_openings_haak_10_L, &disp_add_nr_val, &disp_rechte_sluit_haak_10_L, // regel 1
  &disp_inblaasvent_str,                                                                                      // regel 2
  &disp_external_fault_str, 0                                                                               // regel 3
};
static void * const lcd_luchtmengkast_inblaasvent_factory_settings_al[] =
{
  &disp_luchtmengkast_al, &disp_alarm,                                                                        // icoon
  &disp_luchtmengkast_1_str, &disp_rechte_openings_haak_10_L, &disp_add_nr_val, &disp_rechte_sluit_haak_10_L, // regel 1
  &disp_inblaasvent_str,                                                                                      // regel 2
  &disp_factory_settings_str, 0                                                                               // regel 3
};
static void * const lcd_luchtmengkast_inblaasvent_eep_error_al[] =
{
  &disp_luchtmengkast_al, &disp_alarm,                                                                        // icoon
  &disp_luchtmengkast_1_str, &disp_rechte_openings_haak_10_L, &disp_add_nr_val, &disp_rechte_sluit_haak_10_L, // regel 1
  &disp_inblaasvent_str,                                                                                      // regel 2
  &disp_eep_error_str, 0                                                                               // regel 3
};
static void * const lcd_luchtmengkast_inblaasvent_rtc_general_fault_al[] =
{
  &disp_luchtmengkast_al, &disp_alarm,                                                                        // icoon
  &disp_luchtmengkast_1_str, &disp_rechte_openings_haak_10_L, &disp_add_nr_val, &disp_rechte_sluit_haak_10_L, // regel 1
  &disp_inblaasvent_str,                                                                                      // regel 2
  &disp_rtc_general_fault_str, 0                                                                               // regel 3
};
static void * const lcd_luchtmengkast_inblaasvent_rtc_voltage_fault_al[] =
{
  &disp_luchtmengkast_al, &disp_alarm,                                                                        // icoon
  &disp_luchtmengkast_1_str, &disp_rechte_openings_haak_10_L, &disp_add_nr_val, &disp_rechte_sluit_haak_10_L, // regel 1
  &disp_inblaasvent_str,                                                                                      // regel 2
  &disp_rtc_voltage_fault_str, 0                                                                               // regel 3
};
static void * const lcd_luchtmengkast_inblaasvent_filter_contamination_al[] =
{
  &disp_luchtmengkast_al, &disp_alarm,                                                                        // icoon
  &disp_luchtmengkast_1_str, &disp_rechte_openings_haak_10_L, &disp_add_nr_val, &disp_rechte_sluit_haak_10_L, // regel 1
  &disp_inblaasvent_str,                                                                                      // regel 2
  &disp_filter_contamination_str, 0                                                                               // regel 3
};
static void * const lcd_luchtmengkast_inblaasvent_transfer_error_al[] =
{
  &disp_luchtmengkast_al, &disp_alarm,                                                                        // icoon
  &disp_luchtmengkast_1_str, &disp_rechte_openings_haak_10_L, &disp_add_nr_val, &disp_rechte_sluit_haak_10_L, // regel 1
  &disp_inblaasvent_str,                                                                                      // regel 2
  &disp_transfer_error_str, 0                                                                               // regel 3
};
static void * const lcd_luchtmengkast_inblaasvent_data_connection_line_al[] =
{
  &disp_luchtmengkast_al, &disp_alarm,                                                                        // icoon
  &disp_luchtmengkast_1_str, &disp_rechte_openings_haak_10_L, &disp_add_nr_val, &disp_rechte_sluit_haak_10_L, // regel 1
  &disp_inblaasvent_str,                                                                                      // regel 2
  &disp_data_connection_line_str, 0                                                                               // regel 3
};
static void * const lcd_luchtmengkast_inblaasvent_data_connection_checksum_al[] =
{
  &disp_luchtmengkast_al, &disp_alarm,                                                                        // icoon
  &disp_luchtmengkast_1_str, &disp_rechte_openings_haak_10_L, &disp_add_nr_val, &disp_rechte_sluit_haak_10_L, // regel 1
  &disp_inblaasvent_str,                                                                                      // regel 2
  &disp_data_connection_checksum_str, 0                                                                               // regel 3
};
static void * const lcd_luchtmengkast_inblaasvent_sensor_fault_input_1_al[] =
{
  &disp_luchtmengkast_al, &disp_alarm,                                                                        // icoon
  &disp_luchtmengkast_1_str, &disp_rechte_openings_haak_10_L, &disp_add_nr_val, &disp_rechte_sluit_haak_10_L, // regel 1
  &disp_inblaasvent_str,                                                                                      // regel 2
  &disp_sensor_fault_input_1_str, 0                                                                               // regel 3
};
static void * const lcd_luchtmengkast_inblaasvent_sensor_fault_input_2_al[] =
{
  &disp_luchtmengkast_al, &disp_alarm,                                                                        // icoon
  &disp_luchtmengkast_1_str, &disp_rechte_openings_haak_10_L, &disp_add_nr_val, &disp_rechte_sluit_haak_10_L, // regel 1
  &disp_inblaasvent_str,                                                                                      // regel 2
  &disp_sensor_fault_input_2_str, 0                                                                               // regel 3
};
static void * const lcd_luchtmengkast_inblaasvent_sensor_fault_input_3_al[] =
{
  &disp_luchtmengkast_al, &disp_alarm,                                                                        // icoon
  &disp_luchtmengkast_1_str, &disp_rechte_openings_haak_10_L, &disp_add_nr_val, &disp_rechte_sluit_haak_10_L, // regel 1
  &disp_inblaasvent_str,                                                                                      // regel 2
  &disp_sensor_fault_input_3_str, 0                                                                               // regel 3
};





static void * const lcd_luchtmengkast_afblaasvent_communication_al[] =
{
  &disp_luchtmengkast_al, &disp_alarm,                                                                        // icoon
  &disp_luchtmengkast_1_str, &disp_rechte_openings_haak_10_L, &disp_add_nr_val, &disp_rechte_sluit_haak_10_L, // regel 1
  &disp_afblaasvent_str,                                                                                       // regel 2
  &disp_niet_gevonden_str, 0                                                                                  // regel 3
};
static void * const lcd_luchtmengkast_afblaasvent_locked_motor_al[] =
{
  &disp_luchtmengkast_al, &disp_alarm,                                                                        // icoon
  &disp_luchtmengkast_1_str, &disp_rechte_openings_haak_10_L, &disp_add_nr_val, &disp_rechte_sluit_haak_10_L, // regel 1
  &disp_afblaasvent_str,                                                                                       // regel 2
  &disp_locked_motor_str, 0                                                                                   // regel 3
};
static void * const lcd_luchtmengkast_afblaasvent_hall_failure_al[] =
{
  &disp_luchtmengkast_al, &disp_alarm,                                                                        // icoon
  &disp_luchtmengkast_1_str, &disp_rechte_openings_haak_10_L, &disp_add_nr_val, &disp_rechte_sluit_haak_10_L, // regel 1
  &disp_afblaasvent_str,                                                                                       // regel 2
  &disp_hall_failure_str, 0                                                                                   // regel 3
};
static void * const lcd_luchtmengkast_afblaasvent_thermal_motor_al[] =
{
  &disp_luchtmengkast_al, &disp_alarm,                                                                        // icoon
  &disp_luchtmengkast_1_str, &disp_rechte_openings_haak_10_L, &disp_add_nr_val, &disp_rechte_sluit_haak_10_L, // regel 1
  &disp_afblaasvent_str,                                                                                       // regel 2
  &disp_thermal_motor_str, 0                                                                                  // regel 3
};
static void * const lcd_luchtmengkast_afblaasvent_comm_error_master_slave_al[] =
{
  &disp_luchtmengkast_al, &disp_alarm,                                                                        // icoon
  &disp_luchtmengkast_1_str, &disp_rechte_openings_haak_10_L, &disp_add_nr_val, &disp_rechte_sluit_haak_10_L, // regel 1
  &disp_afblaasvent_str,                                                                                       // regel 2
  &disp_comm_error_master_slave_str, 0                                                                        // regel 3
};
static void * const lcd_luchtmengkast_afblaasvent_thermal_power_module_al[] =
{
  &disp_luchtmengkast_al, &disp_alarm,                                                                        // icoon
  &disp_luchtmengkast_1_str, &disp_rechte_openings_haak_10_L, &disp_add_nr_val, &disp_rechte_sluit_haak_10_L, // regel 1
  &disp_afblaasvent_str,                                                                                       // regel 2
  &disp_thermal_power_module_str, 0                                                                           // regel 3
};
static void * const lcd_luchtmengkast_afblaasvent_comm_error_remote_unit_al[] =
{
  &disp_luchtmengkast_al, &disp_alarm,                                                                        // icoon
  &disp_luchtmengkast_1_str, &disp_rechte_openings_haak_10_L, &disp_add_nr_val, &disp_rechte_sluit_haak_10_L, // regel 1
  &disp_afblaasvent_str,                                                                                       // regel 2
  &disp_comm_error_remote_unit_str, 0                                                                         // regel 3
};
static void * const lcd_luchtmengkast_afblaasvent_phase_failure_al[] =
{
  &disp_luchtmengkast_al, &disp_alarm,                                                                        // icoon
  &disp_luchtmengkast_1_str, &disp_rechte_openings_haak_10_L, &disp_add_nr_val, &disp_rechte_sluit_haak_10_L, // regel 1
  &disp_afblaasvent_str,                                                                                       // regel 2
  &disp_phase_failure_str, 0                                                                                  // regel 3
};
static void * const lcd_luchtmengkast_afblaasvent_brake_al[] =
{
  &disp_luchtmengkast_al, &disp_alarm,                                                                        // icoon
  &disp_luchtmengkast_1_str, &disp_rechte_openings_haak_10_L, &disp_add_nr_val, &disp_rechte_sluit_haak_10_L, // regel 1
  &disp_afblaasvent_str,                                                                                      // regel 2
  &disp_brake_str, 0                                                                                          // regel 3
};
static void * const lcd_luchtmengkast_afblaasvent_high_line_voltage_al[] =
{
  &disp_luchtmengkast_al, &disp_alarm,                                                                        // icoon
  &disp_luchtmengkast_1_str, &disp_rechte_openings_haak_10_L, &disp_add_nr_val, &disp_rechte_sluit_haak_10_L, // regel 1
  &disp_afblaasvent_str,                                                                                      // regel 2
  &disp_high_line_voltage_str, 0                                                                              // regel 3
};
static void * const lcd_luchtmengkast_afblaasvent_low_line_voltage_al[] =
{
  &disp_luchtmengkast_al, &disp_alarm,                                                                        // icoon
  &disp_luchtmengkast_1_str, &disp_rechte_openings_haak_10_L, &disp_add_nr_val, &disp_rechte_sluit_haak_10_L, // regel 1
  &disp_afblaasvent_str,                                                                                      // regel 2
  &disp_low_line_voltage_str, 0                                                                               // regel 3
};
static void * const lcd_luchtmengkast_afblaasvent_low_dc_link_voltage_al[] =
{
  &disp_luchtmengkast_al, &disp_alarm,                                                                        // icoon
  &disp_luchtmengkast_1_str, &disp_rechte_openings_haak_10_L, &disp_add_nr_val, &disp_rechte_sluit_haak_10_L, // regel 1
  &disp_afblaasvent_str,                                                                                      // regel 2
  &disp_low_dc_link_voltage_str, 0                                                                            // regel 3
};
static void * const lcd_luchtmengkast_afblaasvent_high_dc_link_voltage_al[] =
{
  &disp_luchtmengkast_al, &disp_alarm,                                                                        // icoon
  &disp_luchtmengkast_1_str, &disp_rechte_openings_haak_10_L, &disp_add_nr_val, &disp_rechte_sluit_haak_10_L, // regel 1
  &disp_afblaasvent_str,                                                                                      // regel 2
  &disp_high_dc_link_voltage_str, 0                                                                           // regel 3
};
static void * const lcd_luchtmengkast_afblaasvent_driver_problem_al[] =
{
  &disp_luchtmengkast_al, &disp_alarm,                                                                        // icoon
  &disp_luchtmengkast_1_str, &disp_rechte_openings_haak_10_L, &disp_add_nr_val, &disp_rechte_sluit_haak_10_L, // regel 1
  &disp_afblaasvent_str,                                                                                      // regel 2
  &disp_driver_problem_str, 0                                                                                 // regel 3
};
static void * const lcd_luchtmengkast_afblaasvent_electronic_box_over_heat_al[] =
{
  &disp_luchtmengkast_al, &disp_alarm,                                                                        // icoon
  &disp_luchtmengkast_1_str, &disp_rechte_openings_haak_10_L, &disp_add_nr_val, &disp_rechte_sluit_haak_10_L, // regel 1
  &disp_afblaasvent_str,                                                                                       // regel 2
  &disp_electronic_box_over_heat_str, 0                                                                       // regel 3
};
static void * const lcd_luchtmengkast_afblaasvent_excessive_dc_link_current_al[] =
{
  &disp_luchtmengkast_al, &disp_alarm,                                                                        // icoon
  &disp_luchtmengkast_1_str, &disp_rechte_openings_haak_10_L, &disp_add_nr_val, &disp_rechte_sluit_haak_10_L, // regel 1
  &disp_afblaasvent_str,                                                                                       // regel 2
  &disp_excessive_dc_link_current_str, 0                                                                      // regel 3
};
static void * const lcd_luchtmengkast_afblaasvent_unknown_al[] =
{
  &disp_luchtmengkast_al, &disp_alarm,                                                                        // icoon
  &disp_luchtmengkast_1_str, &disp_rechte_openings_haak_10_L, &disp_add_nr_val, &disp_rechte_sluit_haak_10_L, // regel 1
  &disp_afblaasvent_str,                                                                                       // regel 2
  &disp_onbekend_alarm_3_str, 0                                                                               // regel 3
};
static void * const lcd_luchtmengkast_afblaasvent_i_limit_al[] =
{
  &disp_luchtmengkast_al, &disp_alarm,                         // icoon
  &disp_luchtmengkast_1_str, &disp_rechte_openings_haak_10_L, &disp_add_nr_val, &disp_rechte_sluit_haak_10_L, // regel 1
  &disp_afblaasvent_str,                                                                                       // regel 2
  &disp_i_limit_str, 0                                // regel 3
};
static void * const lcd_luchtmengkast_afblaasvent_p_limit_al[] =
{
  &disp_luchtmengkast_al, &disp_alarm,                         // icoon
  &disp_luchtmengkast_1_str, &disp_rechte_openings_haak_10_L, &disp_add_nr_val, &disp_rechte_sluit_haak_10_L, // regel 1
  &disp_afblaasvent_str,                                                                                       // regel 2
  &disp_p_limit_str, 0                                // regel 3
};
static void * const lcd_luchtmengkast_afblaasvent_te_high_al[] =
{
  &disp_luchtmengkast_al, &disp_alarm,                         // icoon
  &disp_luchtmengkast_1_str, &disp_rechte_openings_haak_10_L, &disp_add_nr_val, &disp_rechte_sluit_haak_10_L, // regel 1
  &disp_afblaasvent_str,                                                                                       // regel 2
  &disp_te_high_str, 0                                // regel 3
};
static void * const lcd_luchtmengkast_afblaasvent_tm_high_al[] =
{
  &disp_luchtmengkast_al, &disp_alarm,                         // icoon
  &disp_luchtmengkast_1_str, &disp_rechte_openings_haak_10_L, &disp_add_nr_val, &disp_rechte_sluit_haak_10_L, // regel 1
  &disp_afblaasvent_str,                                                                                       // regel 2
  &disp_tm_high_str, 0                                // regel 3
};
static void * const lcd_luchtmengkast_afblaasvent_tei_high_al[] =
{
  &disp_luchtmengkast_al, &disp_alarm,                         // icoon
  &disp_luchtmengkast_1_str, &disp_rechte_openings_haak_10_L, &disp_add_nr_val, &disp_rechte_sluit_haak_10_L, // regel 1
  &disp_afblaasvent_str,                                                                                       // regel 2
  &disp_tei_high_str, 0                               // regel 3
};
static void * const lcd_luchtmengkast_afblaasvent_uz_low_al[] =
{
  &disp_luchtmengkast_al, &disp_alarm,                         // icoon
  &disp_luchtmengkast_1_str, &disp_rechte_openings_haak_10_L, &disp_add_nr_val, &disp_rechte_sluit_haak_10_L, // regel 1
  &disp_afblaasvent_str,                                                                                       // regel 2
  &disp_uz_low_str, 0                                 // regel 3
};
static void * const lcd_luchtmengkast_afblaasvent_n_low_al[] =
{
  &disp_luchtmengkast_al, &disp_alarm,                         // icoon
  &disp_luchtmengkast_1_str, &disp_rechte_openings_haak_10_L, &disp_add_nr_val, &disp_rechte_sluit_haak_10_L, // regel 1
  &disp_afblaasvent_str,                                                                                       // regel 2
  &disp_n_low_str, 0                                  // regel 3
};
static void * const lcd_luchtmengkast_afblaasvent_igbt_fault_al[] =
{
  &disp_luchtmengkast_al, &disp_alarm,                                                                        // icoon
  &disp_luchtmengkast_1_str, &disp_rechte_openings_haak_10_L, &disp_add_nr_val, &disp_rechte_sluit_haak_10_L, // regel 1
  &disp_afblaasvent_str,                                                                                      // regel 2
  &disp_igbt_fault_str, 0                                                                                     // regel 3
};
static void * const lcd_luchtmengkast_afblaasvent_uzk_hi_al[] =
{
  &disp_luchtmengkast_al, &disp_alarm,                                                                        // icoon
  &disp_luchtmengkast_1_str, &disp_rechte_openings_haak_10_L, &disp_add_nr_val, &disp_rechte_sluit_haak_10_L, // regel 1
  &disp_afblaasvent_str,                                                                                      // regel 2
  &disp_uzk_hi_str, 0                                                                                         // regel 3
};
static void * const lcd_luchtmengkast_afblaasvent_uzk_lo_al[] =
{
  &disp_luchtmengkast_al, &disp_alarm,                                                                        // icoon
  &disp_luchtmengkast_1_str, &disp_rechte_openings_haak_10_L, &disp_add_nr_val, &disp_rechte_sluit_haak_10_L, // regel 1
  &disp_afblaasvent_str,                                                                                      // regel 2
  &disp_uzk_lo_str, 0                                                                                         // regel 3
};
static void * const lcd_luchtmengkast_afblaasvent_uin_hi_al[] =
{
  &disp_luchtmengkast_al, &disp_alarm,                                                                        // icoon
  &disp_luchtmengkast_1_str, &disp_rechte_openings_haak_10_L, &disp_add_nr_val, &disp_rechte_sluit_haak_10_L, // regel 1
  &disp_afblaasvent_str,                                                                                      // regel 2
  &disp_uin_hi_str, 0                                                                                         // regel 3
};
static void * const lcd_luchtmengkast_afblaasvent_uin_lo_al[] =
{
  &disp_luchtmengkast_al, &disp_alarm,                                                                        // icoon
  &disp_luchtmengkast_1_str, &disp_rechte_openings_haak_10_L, &disp_add_nr_val, &disp_rechte_sluit_haak_10_L, // regel 1
  &disp_afblaasvent_str,                                                                                      // regel 2
  &disp_uin_lo_str, 0                                                                                         // regel 3
};
static void * const lcd_luchtmengkast_afblaasvent_failure_power_section_al[] =
{
  &disp_luchtmengkast_al, &disp_alarm,                                                                        // icoon
  &disp_luchtmengkast_1_str, &disp_rechte_openings_haak_10_L, &disp_add_nr_val, &disp_rechte_sluit_haak_10_L, // regel 1
  &disp_afblaasvent_str,                                                                                      // regel 2
  &disp_failure_power_section_str, 0                                                                          // regel 3
};
static void * const lcd_luchtmengkast_afblaasvent_umax_al[] =
{
  &disp_luchtmengkast_al, &disp_alarm,                                                                        // icoon
  &disp_luchtmengkast_1_str, &disp_rechte_openings_haak_10_L, &disp_add_nr_val, &disp_rechte_sluit_haak_10_L, // regel 1
  &disp_afblaasvent_str,                                                                                      // regel 2
  &disp_umax_str, 0                                                                                           // regel 3
};
static void * const lcd_luchtmengkast_afblaasvent_umin_al[] =
{
  &disp_luchtmengkast_al, &disp_alarm,                                                                        // icoon
  &disp_luchtmengkast_1_str, &disp_rechte_openings_haak_10_L, &disp_add_nr_val, &disp_rechte_sluit_haak_10_L, // regel 1
  &disp_afblaasvent_str,                                                                                      // regel 2
  &disp_umin_str, 0                                                                                           // regel 3
};
static void * const lcd_luchtmengkast_afblaasvent_overspeed_al[] =
{
  &disp_luchtmengkast_al, &disp_alarm,                                                                        // icoon
  &disp_luchtmengkast_1_str, &disp_rechte_openings_haak_10_L, &disp_add_nr_val, &disp_rechte_sluit_haak_10_L, // regel 1
  &disp_afblaasvent_str,                                                                                      // regel 2
  &disp_overspeed_str, 0                                                                                      // regel 3
};
static void * const lcd_luchtmengkast_afblaasvent_locked_rotor_al[] =
{
  &disp_luchtmengkast_al, &disp_alarm,                                                                        // icoon
  &disp_luchtmengkast_1_str, &disp_rechte_openings_haak_10_L, &disp_add_nr_val, &disp_rechte_sluit_haak_10_L, // regel 1
  &disp_afblaasvent_str,                                                                                      // regel 2
  &disp_locked_rotor_str, 0                                                                                   // regel 3
};
static void * const lcd_luchtmengkast_afblaasvent_underspeed_al[] =
{
  &disp_luchtmengkast_al, &disp_alarm,                                                                        // icoon
  &disp_luchtmengkast_1_str, &disp_rechte_openings_haak_10_L, &disp_add_nr_val, &disp_rechte_sluit_haak_10_L, // regel 1
  &disp_afblaasvent_str,                                                                                      // regel 2
  &disp_underspeed_str, 0                                                                                     // regel 3
};
static void * const lcd_luchtmengkast_afblaasvent_24V_supply_overloaded_al[] =
{
  &disp_luchtmengkast_al, &disp_alarm,                                                                        // icoon
  &disp_luchtmengkast_1_str, &disp_rechte_openings_haak_10_L, &disp_add_nr_val, &disp_rechte_sluit_haak_10_L, // regel 1
  &disp_afblaasvent_str,                                                                                      // regel 2
  &disp_24V_supply_overloaded_str, 0                                                                          // regel 3
};
static void * const lcd_luchtmengkast_afblaasvent_input_phase_error_al[] =
{
  &disp_luchtmengkast_al, &disp_alarm,                                                                        // icoon
  &disp_luchtmengkast_1_str, &disp_rechte_openings_haak_10_L, &disp_add_nr_val, &disp_rechte_sluit_haak_10_L, // regel 1
  &disp_afblaasvent_str,                                                                                      // regel 2
  &disp_input_phase_error_str, 0                                                                              // regel 3
};
static void * const lcd_luchtmengkast_afblaasvent_motor_phase_error_al[] =
{
  &disp_luchtmengkast_al, &disp_alarm,                                                                        // icoon
  &disp_luchtmengkast_1_str, &disp_rechte_openings_haak_10_L, &disp_add_nr_val, &disp_rechte_sluit_haak_10_L, // regel 1
  &disp_afblaasvent_str,                                                                                      // regel 2
  &disp_motor_phase_error_str, 0                                                                              // regel 3
};
static void * const lcd_luchtmengkast_afblaasvent_memory_error_al[] =
{
  &disp_luchtmengkast_al, &disp_alarm,                                                                        // icoon
  &disp_luchtmengkast_1_str, &disp_rechte_openings_haak_10_L, &disp_add_nr_val, &disp_rechte_sluit_haak_10_L, // regel 1
  &disp_afblaasvent_str,                                                                                      // regel 2
  &disp_memory_error_str, 0                                                                                   // regel 3
};
static void * const lcd_luchtmengkast_afblaasvent_short_circuit_al[] =
{
  &disp_luchtmengkast_al, &disp_alarm,                                                                        // icoon
  &disp_luchtmengkast_1_str, &disp_rechte_openings_haak_10_L, &disp_add_nr_val, &disp_rechte_sluit_haak_10_L, // regel 1
  &disp_afblaasvent_str,                                                                                      // regel 2
  &disp_short_circuit_str, 0                                                                                  // regel 3
};
static void * const lcd_luchtmengkast_afblaasvent_loss_of_synchronism_al[] =
{
  &disp_luchtmengkast_al, &disp_alarm,                                                                        // icoon
  &disp_luchtmengkast_1_str, &disp_rechte_openings_haak_10_L, &disp_add_nr_val, &disp_rechte_sluit_haak_10_L, // regel 1
  &disp_afblaasvent_str,                                                                                      // regel 2
  &disp_loss_of_synchronism_str, 0                                                                            // regel 3
};
static void * const lcd_luchtmengkast_afblaasvent_input_voltage_error_al[] =
{
  &disp_luchtmengkast_al, &disp_alarm,                                                                        // icoon
  &disp_luchtmengkast_1_str, &disp_rechte_openings_haak_10_L, &disp_add_nr_val, &disp_rechte_sluit_haak_10_L, // regel 1
  &disp_afblaasvent_str,                                                                                      // regel 2
  &disp_input_voltage_error_str, 0                                                                            // regel 3
};
static void * const lcd_luchtmengkast_afblaasvent_input_relay_not_closed_al[] =
{
  &disp_luchtmengkast_al, &disp_alarm,                                                                        // icoon
  &disp_luchtmengkast_1_str, &disp_rechte_openings_haak_10_L, &disp_add_nr_val, &disp_rechte_sluit_haak_10_L, // regel 1
  &disp_afblaasvent_str,                                                                                      // regel 2
  &disp_input_relay_not_closed_str, 0                                                                         // regel 3
};
static void * const lcd_luchtmengkast_afblaasvent_high_starting_current_al[] =
{
  &disp_luchtmengkast_al, &disp_alarm,                                                                        // icoon
  &disp_luchtmengkast_1_str, &disp_rechte_openings_haak_10_L, &disp_add_nr_val, &disp_rechte_sluit_haak_10_L, // regel 1
  &disp_afblaasvent_str,                                                                                      // regel 2
  &disp_high_starting_current_str, 0                                                                          // regel 3
};
static void * const lcd_luchtmengkast_afblaasvent_general_error_al[] =
{
  &disp_luchtmengkast_al, &disp_alarm,                                                                        // icoon
  &disp_luchtmengkast_1_str, &disp_rechte_openings_haak_10_L, &disp_add_nr_val, &disp_rechte_sluit_haak_10_L, // regel 1
  &disp_afblaasvent_str,                                                                                      // regel 2
  &disp_general_error_str, 0                                                                                  // regel 3
};
static void * const lcd_luchtmengkast_afblaasvent_motor_fault_al[] =
{
  &disp_luchtmengkast_al, &disp_alarm,                                                                        // icoon
  &disp_luchtmengkast_1_str, &disp_rechte_openings_haak_10_L, &disp_add_nr_val, &disp_rechte_sluit_haak_10_L, // regel 1
  &disp_afblaasvent_str,                                                                                      // regel 2
  &disp_motor_fault_str, 0                                                                                    // regel 3
};
static void * const lcd_luchtmengkast_afblaasvent_motor_blocked_al[] =
{
  &disp_luchtmengkast_al, &disp_alarm,                                                                        // icoon
  &disp_luchtmengkast_1_str, &disp_rechte_openings_haak_10_L, &disp_add_nr_val, &disp_rechte_sluit_haak_10_L, // regel 1
  &disp_afblaasvent_str,                                                                                      // regel 2
  &disp_motor_blocked_str, 0                                                                                  // regel 3
};
static void * const lcd_luchtmengkast_afblaasvent_heat_sink_temperature_al[] =
{
  &disp_luchtmengkast_al, &disp_alarm,                                                                        // icoon
  &disp_luchtmengkast_1_str, &disp_rechte_openings_haak_10_L, &disp_add_nr_val, &disp_rechte_sluit_haak_10_L, // regel 1
  &disp_afblaasvent_str,                                                                                      // regel 2
  &disp_heat_sink_temperature_str, 0                                                                          // regel 3
};
static void * const lcd_luchtmengkast_afblaasvent_ground_fault_al[] =
{
  &disp_luchtmengkast_al, &disp_alarm,                                                                        // icoon
  &disp_luchtmengkast_1_str, &disp_rechte_openings_haak_10_L, &disp_add_nr_val, &disp_rechte_sluit_haak_10_L, // regel 1
  &disp_afblaasvent_str,                                                                                      // regel 2
  &disp_ground_fault_str, 0                                                                                   // regel 3
};
static void * const lcd_luchtmengkast_afblaasvent_hall_ic_fault_al[] =
{
  &disp_luchtmengkast_al, &disp_alarm,                                                                        // icoon
  &disp_luchtmengkast_1_str, &disp_rechte_openings_haak_10_L, &disp_add_nr_val, &disp_rechte_sluit_haak_10_L, // regel 1
  &disp_afblaasvent_str,                                                                                      // regel 2
  &disp_hall_ic_fault_str, 0                                                                                  // regel 3
};
static void * const lcd_luchtmengkast_afblaasvent_overcurrent_al[] =
{
  &disp_luchtmengkast_al, &disp_alarm,                                                                        // icoon
  &disp_luchtmengkast_1_str, &disp_rechte_openings_haak_10_L, &disp_add_nr_val, &disp_rechte_sluit_haak_10_L, // regel 1
  &disp_afblaasvent_str,                                                                                      // regel 2
  &disp_overcurrent_str, 0                                                                                    // regel 3
};
static void * const lcd_luchtmengkast_afblaasvent_line_fault_al[] =         
{
  &disp_luchtmengkast_al, &disp_alarm,                                                                        // icoon
  &disp_luchtmengkast_1_str, &disp_rechte_openings_haak_10_L, &disp_add_nr_val, &disp_rechte_sluit_haak_10_L, // regel 1
  &disp_afblaasvent_str,                                                                                      // regel 2
  &disp_line_fault_str, 0                                                                                     // regel 3
};
static void * const lcd_luchtmengkast_afblaasvent_line_int_heat_sink_sensor_al[] =
{
  &disp_luchtmengkast_al, &disp_alarm,                                                                        // icoon
  &disp_luchtmengkast_1_str, &disp_rechte_openings_haak_10_L, &disp_add_nr_val, &disp_rechte_sluit_haak_10_L, // regel 1
  &disp_afblaasvent_str,                                                                                      // regel 2
  &disp_int_heat_sink_sensor_str, 0                                                                           // regel 3
};
static void * const lcd_luchtmengkast_afblaasvent_dc_res_voltage_to_high_al[] =
{
  &disp_luchtmengkast_al, &disp_alarm,                                                                        // icoon
  &disp_luchtmengkast_1_str, &disp_rechte_openings_haak_10_L, &disp_add_nr_val, &disp_rechte_sluit_haak_10_L, // regel 1
  &disp_afblaasvent_str,                                                                                      // regel 2
  &disp_dc_res_voltage_to_high_str, 0                                                                         // regel 3
};
static void * const lcd_luchtmengkast_afblaasvent_wrong_direction_al[] =
{
  &disp_luchtmengkast_al, &disp_alarm,                                                                        // icoon
  &disp_luchtmengkast_1_str, &disp_rechte_openings_haak_10_L, &disp_add_nr_val, &disp_rechte_sluit_haak_10_L, // regel 1
  &disp_afblaasvent_str,                                                                                      // regel 2
  &disp_wrong_direction_str, 0                                                                                // regel 3
};
static void * const lcd_luchtmengkast_afblaasvent_temperature_lowering_al[] =
{
  &disp_luchtmengkast_al, &disp_alarm,                                                                        // icoon
  &disp_luchtmengkast_1_str, &disp_rechte_openings_haak_10_L, &disp_add_nr_val, &disp_rechte_sluit_haak_10_L, // regel 1
  &disp_afblaasvent_str,                                                                                      // regel 2
  &disp_temperature_lowering_str, 0                                                                           // regel 3
};
static void * const lcd_luchtmengkast_afblaasvent_wrong_connection_al[] =
{
  &disp_luchtmengkast_al, &disp_alarm,                                                                        // icoon
  &disp_luchtmengkast_1_str, &disp_rechte_openings_haak_10_L, &disp_add_nr_val, &disp_rechte_sluit_haak_10_L, // regel 1
  &disp_afblaasvent_str,                                                                                      // regel 2
  &disp_wrong_connection_str, 0                                                                               // regel 3
};
static void * const lcd_luchtmengkast_afblaasvent_external_fault_al[] =
{
  &disp_luchtmengkast_al, &disp_alarm,                                                                        // icoon
  &disp_luchtmengkast_1_str, &disp_rechte_openings_haak_10_L, &disp_add_nr_val, &disp_rechte_sluit_haak_10_L, // regel 1
  &disp_afblaasvent_str,                                                                                      // regel 2
  &disp_external_fault_str, 0                                                                               // regel 3
};
static void * const lcd_luchtmengkast_afblaasvent_factory_settings_al[] =
{
  &disp_luchtmengkast_al, &disp_alarm,                                                                        // icoon
  &disp_luchtmengkast_1_str, &disp_rechte_openings_haak_10_L, &disp_add_nr_val, &disp_rechte_sluit_haak_10_L, // regel 1
  &disp_afblaasvent_str,                                                                                      // regel 2
  &disp_factory_settings_str, 0                                                                               // regel 3
};
static void * const lcd_luchtmengkast_afblaasvent_eep_error_al[] =
{
  &disp_luchtmengkast_al, &disp_alarm,                                                                        // icoon
  &disp_luchtmengkast_1_str, &disp_rechte_openings_haak_10_L, &disp_add_nr_val, &disp_rechte_sluit_haak_10_L, // regel 1
  &disp_afblaasvent_str,                                                                                      // regel 2
  &disp_eep_error_str, 0                                                                                      // regel 3
};
static void * const lcd_luchtmengkast_afblaasvent_rtc_general_fault_al[] =
{
  &disp_luchtmengkast_al, &disp_alarm,                                                                        // icoon
  &disp_luchtmengkast_1_str, &disp_rechte_openings_haak_10_L, &disp_add_nr_val, &disp_rechte_sluit_haak_10_L, // regel 1
  &disp_afblaasvent_str,                                                                                      // regel 2
  &disp_rtc_general_fault_str, 0                                                                              // regel 3
};
static void * const lcd_luchtmengkast_afblaasvent_rtc_voltage_fault_al[] =
{
  &disp_luchtmengkast_al, &disp_alarm,                                                                        // icoon
  &disp_luchtmengkast_1_str, &disp_rechte_openings_haak_10_L, &disp_add_nr_val, &disp_rechte_sluit_haak_10_L, // regel 1
  &disp_afblaasvent_str,                                                                                      // regel 2
  &disp_rtc_voltage_fault_str, 0                                                                              // regel 3
};
static void * const lcd_luchtmengkast_afblaasvent_filter_contamination_al[] =
{
  &disp_luchtmengkast_al, &disp_alarm,                                                                        // icoon
  &disp_luchtmengkast_1_str, &disp_rechte_openings_haak_10_L, &disp_add_nr_val, &disp_rechte_sluit_haak_10_L, // regel 1
  &disp_afblaasvent_str,                                                                                      // regel 2
  &disp_filter_contamination_str, 0                                                                           // regel 3
};
static void * const lcd_luchtmengkast_afblaasvent_transfer_error_al[] =
{
  &disp_luchtmengkast_al, &disp_alarm,                                                                        // icoon
  &disp_luchtmengkast_1_str, &disp_rechte_openings_haak_10_L, &disp_add_nr_val, &disp_rechte_sluit_haak_10_L, // regel 1
  &disp_afblaasvent_str,                                                                                      // regel 2
  &disp_transfer_error_str, 0                                                                                 // regel 3
};
static void * const lcd_luchtmengkast_afblaasvent_data_connection_line_al[] =
{
  &disp_luchtmengkast_al, &disp_alarm,                                                                        // icoon
  &disp_luchtmengkast_1_str, &disp_rechte_openings_haak_10_L, &disp_add_nr_val, &disp_rechte_sluit_haak_10_L, // regel 1
  &disp_afblaasvent_str,                                                                                      // regel 2
  &disp_data_connection_line_str, 0                                                                           // regel 3
};
static void * const lcd_luchtmengkast_afblaasvent_data_connection_checksum_al[] =
{
  &disp_luchtmengkast_al, &disp_alarm,                                                                        // icoon
  &disp_luchtmengkast_1_str, &disp_rechte_openings_haak_10_L, &disp_add_nr_val, &disp_rechte_sluit_haak_10_L, // regel 1
  &disp_afblaasvent_str,                                                                                      // regel 2
  &disp_data_connection_checksum_str, 0                                                                       // regel 3
};
static void * const lcd_luchtmengkast_afblaasvent_sensor_fault_input_1_al[] =
{
  &disp_luchtmengkast_al, &disp_alarm,                                                                        // icoon
  &disp_luchtmengkast_1_str, &disp_rechte_openings_haak_10_L, &disp_add_nr_val, &disp_rechte_sluit_haak_10_L, // regel 1
  &disp_afblaasvent_str,                                                                                      // regel 2
  &disp_sensor_fault_input_1_str, 0                                                                           // regel 3
};
static void * const lcd_luchtmengkast_afblaasvent_sensor_fault_input_2_al[] =
{
  &disp_luchtmengkast_al, &disp_alarm,                                                                        // icoon
  &disp_luchtmengkast_1_str, &disp_rechte_openings_haak_10_L, &disp_add_nr_val, &disp_rechte_sluit_haak_10_L, // regel 1
  &disp_afblaasvent_str,                                                                                      // regel 2
  &disp_sensor_fault_input_2_str, 0                                                                           // regel 3
};
static void * const lcd_luchtmengkast_afblaasvent_sensor_fault_input_3_al[] =
{
  &disp_luchtmengkast_al, &disp_alarm,                                                                        // icoon
  &disp_luchtmengkast_1_str, &disp_rechte_openings_haak_10_L, &disp_add_nr_val, &disp_rechte_sluit_haak_10_L, // regel 1
  &disp_afblaasvent_str,                                                                                      // regel 2
  &disp_sensor_fault_input_3_str, 0                                                                           // regel 3
};


//-----------------------------------------------------------------------------------
static void * const lcd_systeem_al_geheugen_256k[] =
{
  &disp_systeem,                    // icoon
  &disp_waarschuwing_systeem_str,   // regel 1
  &disp_syst_al_geheugen_256k_str,  // regel 2
  0                                 // regel 3
};

static void * const lcd_systeem_al_eeprom_256k[] =
{
  &disp_systeem,                    // icoon
  &disp_alarm_systeem_str,          // regel 1
  &disp_syst_al_eeprom_256k_str,    // regel 2
  0                                 // regel 3
};

s_key_action alarm_0_key_action;
s_key_action const const_alarm_0_key_action =
{
  0,                                // nr
  0,                                // index
  (unsigned char *)&option_on,      // option
  (unsigned char *)&option_index_0, // Optie index 
  lcd_alarm_actueel,                // display
  0,                                // cursor
  &dummy_value,                     // *value
  Alarm_Return,                     // void (*number)(void); 
  Alarm_Return,                     // void (*arrow)(void); 
  Alarm_Return,                     // void (*enter)(void);
};

s_screen screen_alarm_0;
s_screen const screen_alarm_0_default =
{
  0, // functie nr
  0, // index
  1, // max rel
  0, // rel actief
  0, // nr actief nummer weergegeven bin aantal ingeschakelde functie nummers
  0, // aantal ingeschakelde functie nummers
  lcd_disp_header, // algemene scherm opmaak
  {0,0,0,0,0,0}, // functies behorende bij functie toetsen
  0,3,19,       // rel[0]
  0,3,19+28,    // rel[1]
  0,3,19+28+28, // rel[2]
  &alarm_0_key_action, // first_action
  &alarm_0_key_action, // last action 
  0, // change_flag
  0, // punt in normaal display
  0, // punt tijdens invoeren
  0, // aantal digits ingevoerd (punt telt niet mee)
  0, // value
  0, // min value
  0, // max value
  0, // vorige scherm
  0  // prev_next_func
};

void Alarm_Copy_Disp(void *dest[], void * const source[])
{
  do
  {
    *dest = *source;
    dest++;
    source++;
  }
  while (*source);
  *dest = 0;
}

void Alarm_Add_To_Disp(void *dest[], void * const source[])
{
  while (*dest)
  {
    dest++;
  }
  do
  {
    *dest = *source;
    dest++;
    source++;
  }
  while (*source);
  *dest = 0;
}

void Alarm_Zet_Disp(void *lcd[], int nr)
{
TAlarm Alarm;

  GetAlarm(nr, &Alarm);
  if (Alarm.Lcd == NULL)
    Alarm_Copy_Disp(lcd, lcd_onbekend_al);
  else                                                                                                                                                                              
    Alarm_Copy_Disp(lcd, Alarm.Lcd);                                                                                                                                                
}                                                                                                                                                                                   
                                                                                                                                                                                    
/*  JP dit gaat fout (alarm_actueel wordt neit gevuld
static void Disp_Control_Alarm_Actueel(void)
{
  if(laatste_alarm_actueel.code != alarm_disp_act.code)
  {
    alarm_actueel = laatste_alarm_actueel = alarm_disp_act;
    alarm_0_key_action.nr = screen_alarm_0.nr = alarm_disp_act.code;
    Alarm_Zet_Disp(lcd_alarm_actueel, screen_alarm_0.nr);
    lcd_refresh_fast_switch = 1;
  }
  else
  {
    alarm_actueel = laatste_alarm_actueel = alarm_disp_act;
  }
}
*/
static void Disp_Control_Alarm_Actueel(void)
{
  if(laatste_alarm_actueel.code != alarm_disp_act.code)
  {
    alarm_actueel = alarm_disp_act;
    laatste_alarm_actueel = alarm_disp_act;
    alarm_0_key_action.nr = screen_alarm_0.nr = alarm_disp_act.code;
    Alarm_Zet_Disp(lcd_alarm_actueel, screen_alarm_0.nr);
    lcd_refresh_fast_switch = 1;
  }
  else
  {
    alarm_actueel = alarm_disp_act;
    laatste_alarm_actueel = alarm_disp_act;
  }
}

void Control_Screen_Alarm_0(void)
{
  alarm_0_key_action = const_alarm_0_key_action;
  Control_Screen(&screen_alarm_0, &screen_alarm_0_default, 1, 1);
}

void If_Exist_Goto_Screen_Alarm_0(void)
{
  Control_Screen_Alarm_0();
  if (screen_alarm_0.nr_aantal)
    Next_Screen(&screen_alarm_0);
}     

//==============================================================================
//------------------------ Alarm - Table Handling ------------------------------
//==============================================================================
TAlarm const AlarmTable[] =
{
  // Harde alarmen
  { SYSTEEM_GEHEUGEN_256K_AL,                    0, MASK_AL_HARD      | MASK_ALG      , lcd_systeem_al_geheugen_256k                  , ALARM_VAL_0 },
  {	SYSTEEM_EEPROM_256K_AL,						 0, MASK_AL_HARD      | MASK_ALG      , lcd_systeem_al_eeprom_256k                    , ALARM_VAL_0 },

  { SYSTEEM_AL_OPT,                              1, MASK_HA_ZA_ON_WIS | MASK_ALG_DISP , lcd_systeem_al_option                         , ALARM_VAL_0 },
  { ORION_OFF_AL,                                2, MASK_HA_ZA_ON_WIS | MASK_ALG_DISP , lcd_orion_off_al                              , ALARM_VAL_0 },
  { SYSTEEM_AL_RESTORE_OPTION_SETPOINT_FAILED,   3, MASK_AL_HARD_WIS  | MASK_ALG      , lcd_systeem_al_restore_option_setpoint_failed , ALARM_VAL_0 },
  { SYSTEEM_AL_SETP,                             4, MASK_AL_HARD_WIS  | MASK_ALG      , lcd_systeem_al_setpoint                       , ALARM_VAL_0 },
  { SYSTEEM_AL_I2C0,                             5, MASK_AL_HARD_WIS  | MASK_ALG      , lcd_systeem_al_I2C0                           , ALARM_VAL_0 },
  { SYSTEEM_AL_EEP,                              6, MASK_AL_HARD_WIS  | MASK_ALG      , lcd_systeem_al_EEP                            , ALARM_VAL_0 },

  { IO_06_14_BOARD_AL,                          10, MASK_HA_ZA_ON     | MASK_ALG      , lcd_IO_06_14_board_al                         , ALARM_INDEX },
  { IO_06_14_ONBEKEND_AL,                       13, MASK_HA_ZA_ON     | MASK_ALG      , lcd_IO_06_14_onbekend_al                      , ALARM_INDEX },

  { IO_12_06_BOARD_AL,                          10, MASK_HA_ZA_ON     | MASK_ALG      , lcd_IO_12_06_board_al                         , ALARM_INDEX },
  { IO_12_06_ONBEKEND_AL,                       13, MASK_HA_ZA_ON     | MASK_ALG      , lcd_IO_12_06_onbekend_al                      , ALARM_INDEX },

  { IO_08_09_BOARD_AL,                          10, MASK_HA_ZA_ON     | MASK_ALG      , lcd_IO_08_09_board_al                         , ALARM_INDEX },
  { IO_08_09_ONBEKEND_AL,                       13, MASK_HA_ZA_ON     | MASK_ALG      , lcd_IO_08_09_onbekend_al                      , ALARM_INDEX },

  { IO_EKU_BOARD_AL,                            10, MASK_HA_ZA_ON     | MASK_ALG      , lcd_IO_EKU_board_al                           , ALARM_INDEX },
  { IO_EKU_ONBEKEND_AL,                         13, MASK_HA_ZA_ON     | MASK_ALG      , lcd_IO_EKU_onbekend_al                        , ALARM_INDEX },

  { IO_H2MC_BOARD_AL,                           10, MASK_HA_ZA_ON     | MASK_ALG      , lcd_IO_H2MC_board_al                          , ALARM_INDEX },
  { IO_H2MC_ONBEKEND_AL,                        13, MASK_HA_ZA_ON     | MASK_ALG      , lcd_IO_H2MC_onbekend_al                       , ALARM_INDEX },

  { IO_H1MC_BOARD_AL,                           10, MASK_HA_ZA_ON     | MASK_ALG      , lcd_IO_H1MC_board_al                          , ALARM_INDEX },
  { IO_H1MC_ONBEKEND_AL,                        13, MASK_HA_ZA_ON     | MASK_ALG      , lcd_IO_H1MC_onbekend_al                       , ALARM_INDEX },

  { IO_05_07_BOARD_AL,                          10, MASK_HA_ZA_ON     | MASK_ALG      , lcd_IO_05_07_board_al                         , ALARM_INDEX },
  { IO_05_07_ADC_AL,                            11, MASK_HA_ZA_ON     | MASK_ALG      , lcd_IO_05_07_adc_al                           , ALARM_INDEX },
  { IO_05_07_EXTERNE_24V_AL,                    11, MASK_HA_ZA_ON     | MASK_ALG      , lcd_IO_05_07_externe_24v_al                   , ALARM_INDEX },
  { IO_05_07_ANA_IN_1_AL,                       12, MASK_HA_ZA_ON     | MASK_ALG      , lcd_IO_05_07_adc_nr_al                        , ALARM_INDEX },
  { IO_05_07_ANA_IN_2_AL,                       12, MASK_HA_ZA_ON     | MASK_ALG      , lcd_IO_05_07_adc_nr_al                        , ALARM_INDEX },
  { IO_05_07_ANA_IN_3_AL,                       12, MASK_HA_ZA_ON     | MASK_ALG      , lcd_IO_05_07_adc_nr_al                        , ALARM_INDEX },
  { IO_05_07_ANA_IN_4_AL,                       12, MASK_HA_ZA_ON     | MASK_ALG      , lcd_IO_05_07_adc_nr_al                        , ALARM_INDEX },
  { IO_05_07_ANA_IN_5_AL,                       12, MASK_HA_ZA_ON     | MASK_ALG      , lcd_IO_05_07_adc_nr_al                        , ALARM_INDEX },
  { IO_05_07_ONBEKEND_AL,                       13, MASK_HA_ZA_ON     | MASK_ALG      , lcd_IO_05_07_onbekend_al                      , ALARM_INDEX },
  { IO_05_07_VERSIE_AL,                         12, MASK_AL_ZA_ON     | MASK_ALG      , lcd_IO_05_07_versie_al                        , ALARM_INDEX },

  { IO_07_07_BOARD_AL,                          10, MASK_HA_ZA_ON     | MASK_ALG      , lcd_IO_07_07_board_al                         , ALARM_INDEX },
  { IO_07_07_ADC_AL,                            11, MASK_HA_ZA_ON     | MASK_ALG      , lcd_IO_07_07_adc_al                           , ALARM_INDEX },
  { IO_07_07_EXTERNE_24V_AL,                    11, MASK_HA_ZA_ON     | MASK_ALG      , lcd_IO_07_07_externe_24v_al                   , ALARM_INDEX },
  { IO_07_07_ANA_IN_1_AL,                       12, MASK_HA_ZA_ON     | MASK_ALG      , lcd_IO_07_07_adc_nr_al                        , ALARM_INDEX },
  { IO_07_07_ANA_IN_2_AL,                       12, MASK_HA_ZA_ON     | MASK_ALG      , lcd_IO_07_07_adc_nr_al                        , ALARM_INDEX },
  { IO_07_07_ANA_IN_3_AL,                       12, MASK_HA_ZA_ON     | MASK_ALG      , lcd_IO_07_07_adc_nr_al                        , ALARM_INDEX },
  { IO_07_07_ANA_IN_4_AL,                       12, MASK_HA_ZA_ON     | MASK_ALG      , lcd_IO_07_07_adc_nr_al                        , ALARM_INDEX },
  { IO_07_07_ONBEKEND_AL,                       13, MASK_HA_ZA_ON     | MASK_ALG      , lcd_IO_07_07_onbekend_al                      , ALARM_INDEX },
  { IO_07_07_VERSIE_AL,                         12, MASK_AL_ZA_ON     | MASK_ALG      , lcd_IO_07_07_versie_al                        , ALARM_INDEX },

  { MOTOR_MANUAL_AL,                            15, MASK_HA_ZA_ON     | MASK_ALG      , lcd_motor_manual_al                           , ALARM_INDEX },
  { MOTOR_EMERGENCYSWITCH_AL,                   13, MASK_HA_ZA_ON     | MASK_ALG      , lcd_motor_emergencyswitch_al                  , ALARM_INDEX },
  { MOTOR_THERMAL_CLOSE_AL,                     13, MASK_HA_ZA_ON     | MASK_ALG      , lcd_motor_thermal_close_al                    , ALARM_INDEX },
  { MOTOR_THERMAL_OPEN_AL,                      13, MASK_HA_ZA_ON     | MASK_ALG      , lcd_motor_thermal_open_al                     , ALARM_INDEX },
  { MOTOR_BREAK_INPUT_AL,                       13, MASK_HA_ZA_ON     | MASK_ALG      , lcd_motor_break_input_al                      , ALARM_INDEX },
  { MOTOR_SPEED_TO_LOW_AL,                      13, MASK_HA_ZA_ON     | MASK_ALG      , lcd_motor_speed_to_low_al                     , ALARM_INDEX },
  { MOTOR_ENCODER_FAILURE_AL,                   13, MASK_HA_ZA_ON     | MASK_ALG      , lcd_motor_encoder_failure_al                  , ALARM_INDEX },
  { MOTOR_NO_FEEDBACK_AL,                       13, MASK_HA_ZA_ON     | MASK_ALG      , lcd_motor_no_feedback_al                      , ALARM_INDEX },
  { MOTOR_NOT_ENOUGH_PULSES_AL,                 13, MASK_HA_ZA_ON     | MASK_ALG      , lcd_motor_not_enough_pulses_al                , ALARM_INDEX },
  { MOTOR_PULSES_TO_FAST_AL,                    13, MASK_HA_ZA_ON     | MASK_ALG      , lcd_motor_pulses_to_fast_al                   , ALARM_INDEX },
  { MOTOR_NOT_INSTALLED_AL,                     13, MASK_HA_ZA_ON     | MASK_ALG      , lcd_motor_not_installed_al                    , ALARM_INDEX },
  { MOTOR_ENCODER_INTERFERENCE_AL,              13, MASK_HA_ZA_ON     | MASK_ALG      , lcd_motor_encoder_interference_al             , ALARM_INDEX },
  { MOTOR_INSTALL_MODE_AL,                      13, MASK_HA_ZA_ON     | MASK_ALG      , lcd_motor_install_mode_al                     , ALARM_INDEX },
  { MOTOR_ERROR_DEVIATION_POSITION_AL,          14, MASK_HA_ZA_ON     | MASK_ALG      , lcd_motor_error_deviation_position_al         , ALARM_INDEX },
  { MOTOR_WARNING_DEVIATION_POSITION_AL,        14, MASK_HA_ZA_ON     | MASK_ALG      , lcd_motor_warning_deviation_position_al       , ALARM_INDEX },
  { MOTOR_LIMITSWITCH_SAFETY_SPEED_AL,          13, MASK_HA_ZA_ON     | MASK_ALG      , lcd_motor_limit_switch_safety_speed_al        , ALARM_INDEX },
  { MOTOR_DIRECTIONS_NOT_DEFINED_AL,            13, MASK_HA_ZA_ON     | MASK_ALG      , lcd_motor_directions_not_defined_al           , ALARM_INDEX },
  { MOTOR_DUALSCREEN_NOT_POSSIBLE_AL,           13, MASK_HA_ZA_ON     | MASK_ALG      , lcd_motor_dualscreen_not_possible_al          , ALARM_INDEX },
  { MOTOR_COMMUNICATION_AL,                     13, MASK_HA_ZA_ON     | MASK_ALG      , lcd_motor_communication_al                    , ALARM_INDEX },
  { MOTOR_LIMITSWITCHES_NOT_EQUAL_AL,           13, MASK_HA_ZA_ON     | MASK_ALG      , lcd_motor_limitswitches_not_equal_al          , ALARM_INDEX },
  { MOTOR_SPEED_NOT_EQUAL_AL,                   13, MASK_HA_ZA_ON     | MASK_ALG      , lcd_motor_speed_not_equal_al                  , ALARM_INDEX },
  { MOTOR_MULTIPLE_MASTER_AL,                   12, MASK_HA_ZA_ON     | MASK_ALG      , lcd_motor_multiple_master_al                  , ALARM_INDEX },
  { MOTOR_SLAVE_NOT_INIT_AL,                    12, MASK_HA_ZA_ON     | MASK_ALG      , lcd_motor_slave_not_init_al                   , ALARM_INDEX },
  { MOTOR_FREQUENCY_CONTROLLER_AL,              13, MASK_HA_ZA_ON     | MASK_ALG      , lcd_motor_frequency_controller_al             , ALARM_INDEX },
  { MOTOR_NOT_SYNCHRONOUS_AL,                   13, MASK_HA_ZA_ON     | MASK_ALG      , lcd_motor_sync_guard_active_al                , ALARM_INDEX },
  { MOTOR_LIMITSWITCH_NOT_REACHED_AL,           13, MASK_HA_ZA_ON     | MASK_ALG      , lcd_motor_limitswitch_not_reached_al          , ALARM_INDEX },
  { MOTOR_WRONG_DIRECTION_AL,                   13, MASK_HA_ZA_ON     | MASK_ALG      , lcd_motor_wrong_direction_al                  , ALARM_INDEX },
  { MOTOR_LINK_UNKNOWN_AL,                      13, MASK_HA_ZA_ON     | MASK_ALG      , lcd_motor_link_unknown_al                     , ALARM_INDEX },
  { MOTOR_NOT_RUNNING_AL,                       13, MASK_HA_ZA_ON     | MASK_ALG      , lcd_motor_not_running_al                      , ALARM_INDEX },
  { MOTOR_ENCODER_FAILURE_A_AL,                 14, MASK_HA_ZA_ON     | MASK_ALG      , lcd_motor_encoder_failure_A_al                , ALARM_INDEX },
  { MOTOR_ENCODER_FAILURE_B_AL,                 14, MASK_HA_ZA_ON     | MASK_ALG      , lcd_motor_encoder_failure_B_al                , ALARM_INDEX },
  { MOTOR_POSITION_NOT_REACHED_AL,              13, MASK_HA_ZA_ON     | MASK_ALG      , lcd_motor_position_not_reached_al             , ALARM_INDEX },


  { VENT_MANUAL_AL,                             15, MASK_HA_ZA_ON     | MASK_ALG      , lcd_vent_manual_al                            , ALARM_INDEX },
  { VENT_TARGET_NOT_REACHED_AL,                 14, MASK_HA_ZA_ON_WIS | MASK_ALG      , lcd_vent_target_not_reached_al                , ALARM_INDEX },

  { KLEP_MANUAL_AL,                             15, MASK_HA_ZA_ON     | MASK_ALG      , lcd_klep_manual_al                            , ALARM_INDEX },
  { KLEP_TARGET_NOT_REACHED_AL,                 14, MASK_HA_ZA_ON_WIS | MASK_ALG      , lcd_klep_target_not_reached_al                , ALARM_INDEX },
  { KLEP_LIMITSWITCH_AL,                        14, MASK_HA_ZA_ON_WIS | MASK_ALG      , lcd_klep_limitswitch_al                       , ALARM_INDEX },
  { KLEP_EXTERN_ALARM_AL,                       14, MASK_HA_ZA_ON_WIS | MASK_ALG      , lcd_klep_extern_alarm_al                      , ALARM_INDEX },

  { VENT_MB_COMMUNICATION_AL,             12, MASK_HA_ZA_ON_WIS | MASK_ALG      , lcd_vent_communication_al                     , ALARM_INDEX },
  { VENT_MB_LOCKED_MOTOR_AL,              13, MASK_HA_ZA_ON_WIS | MASK_ALG      , lcd_vent_locked_motor_al                      , ALARM_INDEX },
  { VENT_MB_HALL_FAILURE_AL,              13, MASK_HA_ZA_ON_WIS | MASK_ALG      , lcd_vent_hall_failure_al                      , ALARM_INDEX },
  { VENT_MB_THERMAL_MOTOR_AL,             13, MASK_HA_ZA_ON_WIS | MASK_ALG      , lcd_vent_thermal_motor_al                     , ALARM_INDEX },
  { VENT_MB_COMM_ERROR_MASTER_SLAVE_AL,   13, MASK_HA_ZA_ON_WIS | MASK_ALG      , lcd_vent_comm_error_master_slave_al           , ALARM_INDEX },
  { VENT_MB_THERMAL_POWER_MODULE_AL,      13, MASK_HA_ZA_ON_WIS | MASK_ALG      , lcd_vent_thermal_power_module_al              , ALARM_INDEX },
  { VENT_MB_COMM_ERROR_REMOTE_UNIT_AL,    13, MASK_HA_ZA_ON_WIS | MASK_ALG      , lcd_vent_comm_error_remote_unit_al            , ALARM_INDEX },
  { VENT_MB_PHASE_FAILURE_AL,             13, MASK_HA_ZA_ON_WIS | MASK_ALG      , lcd_vent_phase_failure_al                     , ALARM_INDEX },
  { VENT_MB_BRAKE_AL,                     13, MASK_HA_ZA_ON_WIS | MASK_ALG      , lcd_vent_brake_al                             , ALARM_INDEX },
  { VENT_MB_HIGH_LINE_VOLTAGE_AL,         13, MASK_HA_ZA_ON_WIS | MASK_ALG      , lcd_vent_high_line_voltage_al                 , ALARM_INDEX },
  { VENT_MB_LOW_LINE_VOLTAGE_AL,          13, MASK_HA_ZA_ON_WIS | MASK_ALG      , lcd_vent_low_line_voltage_al                  , ALARM_INDEX },
  { VENT_MB_LOW_DC_LINK_VOLTAGE_AL,       13, MASK_HA_ZA_ON_WIS | MASK_ALG      , lcd_vent_low_dc_link_voltage_al               , ALARM_INDEX },
  { VENT_MB_HIGH_DC_LINK_VOLTAGE_AL,      13, MASK_HA_ZA_ON_WIS | MASK_ALG      , lcd_vent_high_dc_link_voltage_al              , ALARM_INDEX },
  { VENT_MB_DRIVER_PROBLEM_AL,            13, MASK_HA_ZA_ON_WIS | MASK_ALG      , lcd_vent_driver_problem_al                    , ALARM_INDEX },
  { VENT_MB_ELECTRONIC_BOX_OVER_HEAT_AL,  13, MASK_HA_ZA_ON_WIS | MASK_ALG      , lcd_vent_electronic_box_over_heat_al          , ALARM_INDEX },
  { VENT_MB_EXCESSIVE_DC_LINK_CURRENT_AL, 13, MASK_HA_ZA_ON     | MASK_ALG      , lcd_vent_excessive_dc_link_current_al         , ALARM_INDEX },
  { VENT_MB_UNKNOWN_AL,                   15, MASK_HA_ZA_ON     | MASK_ALG      , lcd_vent_unknown_al                           , ALARM_INDEX },

  { VENT_MB_GENERAL_ERROR_AL,             13, MASK_HA_ZA_ON_WIS | MASK_ALG      , lcd_vent_general_error_al                     , ALARM_INDEX },
  { VENT_MB_MOTOR_FAULT_AL,               13, MASK_HA_ZA_ON_WIS | MASK_ALG      , lcd_vent_motor_fault_al                       , ALARM_INDEX }, 
  { VENT_MB_MOTOR_BLOCKED_AL,             13, MASK_HA_ZA_ON_WIS | MASK_ALG      , lcd_vent_motor_blocked_al                     , ALARM_INDEX },
  { VENT_MB_HEAT_SINK_TEMPERATURE_AL,     13, MASK_HA_ZA_ON_WIS | MASK_ALG      , lcd_vent_heat_sink_temperature_al             , ALARM_INDEX },
  { VENT_MB_GROUND_FAULT_AL,              13, MASK_HA_ZA_ON_WIS | MASK_ALG      , lcd_vent_ground_fault_al                      , ALARM_INDEX },
  { VENT_MB_HALL_IC_FAULT_AL,             13, MASK_HA_ZA_ON_WIS | MASK_ALG      , lcd_vent_hall_ic_fault_al                     , ALARM_INDEX },
  { VENT_MB_OVERCURRENT_AL,               13, MASK_HA_ZA_ON_WIS | MASK_ALG      , lcd_vent_overcurrent_al                       , ALARM_INDEX },
  { VENT_MB_LINE_FAULT_AL,                13, MASK_HA_ZA_ON_WIS | MASK_ALG      , lcd_vent_line_fault_al                        , ALARM_INDEX },
  { VENT_MB_LINE_INT_HEAT_SINK_SENSOR_AL, 13, MASK_HA_ZA_ON_WIS | MASK_ALG      , lcd_vent_line_int_heat_sink_sensor_al         , ALARM_INDEX },
  { VENT_MB_DC_RES_VOLTAGE_TO_HIGH_AL,    13, MASK_HA_ZA_ON_WIS | MASK_ALG      , lcd_vent_dc_res_voltage_to_high_al            , ALARM_INDEX },
  { VENT_MB_WRONG_DIRECTION_AL,           13, MASK_HA_ZA_ON_WIS | MASK_ALG      , lcd_vent_wrong_direction_al                   , ALARM_INDEX },
  { VENT_MB_TEMPERATURE_LOWERING_AL,      13, MASK_HA_ZA_ON_WIS | MASK_ALG      , lcd_vent_temperature_lowering_al              , ALARM_INDEX },
  { VENT_MB_WRONG_CONNECTION_AL,          13, MASK_HA_ZA_ON_WIS | MASK_ALG      , lcd_vent_wrong_connection_al                  , ALARM_INDEX },
  { VENT_MB_EXTERNAL_FAULT_AL,            13, MASK_HA_ZA_ON_WIS | MASK_ALG      , lcd_vent_external_fault_al                    , ALARM_INDEX },
  { VENT_MB_FACTORY_SETTINGS_AL,          13, MASK_HA_ZA_ON_WIS | MASK_ALG      , lcd_vent_factory_settings_al                  , ALARM_INDEX },
  { VENT_MB_EEP_ERROR_AL,                 13, MASK_HA_ZA_ON_WIS | MASK_ALG      , lcd_vent_eep_error_al                         , ALARM_INDEX },
  { VENT_MB_RTC_GENERAL_FAULT_AL,         13, MASK_HA_ZA_ON_WIS | MASK_ALG      , lcd_vent_rtc_general_fault_al                 , ALARM_INDEX },
  { VENT_MB_RTC_VOLTAGE_FAULT_AL,         13, MASK_HA_ZA_ON_WIS | MASK_ALG      , lcd_vent_rtc_voltage_fault_al                 , ALARM_INDEX },
  { VENT_MB_FILTER_CONTAMINATION_AL,      13, MASK_HA_ZA_ON_WIS | MASK_ALG      , lcd_vent_filter_contamination_al              , ALARM_INDEX },
  { VENT_MB_TRANSFER_ERROR_AL,            13, MASK_HA_ZA_ON_WIS | MASK_ALG      , lcd_vent_transfer_error_al                    , ALARM_INDEX },
  { VENT_MB_DATA_CONNECTION_LINE_AL,      13, MASK_HA_ZA_ON_WIS | MASK_ALG      , lcd_vent_data_connection_line_al              , ALARM_INDEX },
  { VENT_MB_DATA_CONNECTION_CHECKSUM_AL,  13, MASK_HA_ZA_ON_WIS | MASK_ALG      , lcd_vent_data_connection_checksum_al          , ALARM_INDEX },
  { VENT_MB_SENSOR_FAULT_INPUT_1_AL,      13, MASK_HA_ZA_ON_WIS | MASK_ALG      , lcd_vent_sensor_fault_input_1_al              , ALARM_INDEX },
  { VENT_MB_SENSOR_FAULT_INPUT_2_AL,      13, MASK_HA_ZA_ON_WIS | MASK_ALG      , lcd_vent_sensor_fault_input_2_al              , ALARM_INDEX },
  { VENT_MB_SENSOR_FAULT_INPUT_3_AL,      13, MASK_HA_ZA_ON_WIS | MASK_ALG      , lcd_vent_sensor_fault_input_3_al              , ALARM_INDEX },
  { VENT_MB_I_LIMIT_AL,                   13, MASK_HA_ZA_ON_WIS | MASK_ALG      , lcd_vent_i_limit_al                           , ALARM_INDEX },
  { VENT_MB_P_LIMIT_AL,                   13, MASK_HA_ZA_ON_WIS | MASK_ALG      , lcd_vent_p_limit_al                           , ALARM_INDEX },
  { VENT_MB_TE_HIGH_AL,                   13, MASK_HA_ZA_ON_WIS | MASK_ALG      , lcd_vent_te_high_al                           , ALARM_INDEX },
  { VENT_MB_TM_HIGH_AL,                   13, MASK_HA_ZA_ON_WIS | MASK_ALG      , lcd_vent_tm_high_al                           , ALARM_INDEX },
  { VENT_MB_TEI_HIGH_AL,                  13, MASK_HA_ZA_ON_WIS | MASK_ALG      , lcd_vent_tei_high_al                          , ALARM_INDEX },
  { VENT_MB_UZ_LOW_AL,                    13, MASK_HA_ZA_ON_WIS | MASK_ALG      , lcd_vent_uz_low_al                            , ALARM_INDEX },
  { VENT_MB_N_LOW_AL,                     13, MASK_HA_ZA_ON_WIS | MASK_ALG      , lcd_vent_n_low_al                             , ALARM_INDEX },
  { VENT_MB_IGBT_FAULT_AL,                13, MASK_HA_ZA_ON_WIS | MASK_ALG      , lcd_vent_igbt_fault_al                        , ALARM_INDEX },
  { VENT_MB_UZK_HI_AL,                    13, MASK_HA_ZA_ON_WIS | MASK_ALG      , lcd_vent_uzk_hi_al                            , ALARM_INDEX },
  { VENT_MB_UZK_LO_AL,                    13, MASK_HA_ZA_ON_WIS | MASK_ALG      , lcd_vent_uzk_lo_al                            , ALARM_INDEX },
  { VENT_MB_UIN_HI_AL,                    13, MASK_HA_ZA_ON_WIS | MASK_ALG      , lcd_vent_uin_hi_al                            , ALARM_INDEX },
  { VENT_MB_UIN_LO_AL,                    13, MASK_HA_ZA_ON_WIS | MASK_ALG      , lcd_vent_uin_lo_al                            , ALARM_INDEX },
  { VENT_MB_FAILURE_POWER_SECTION_AL,     13, MASK_HA_ZA_ON_WIS | MASK_ALG      , lcd_vent_failure_power_section_al             , ALARM_INDEX },
  { VENT_MB_UMAX_AL,                      13, MASK_HA_ZA_ON_WIS | MASK_ALG      , lcd_vent_umax_al                              , ALARM_INDEX },
  { VENT_MB_UMIN_AL,                      13, MASK_HA_ZA_ON_WIS | MASK_ALG      , lcd_vent_umin_al                              , ALARM_INDEX },
  { VENT_MB_OVERSPEED_AL,                 13, MASK_HA_ZA_ON_WIS | MASK_ALG      , lcd_vent_overspeed_al                         , ALARM_INDEX },
  { VENT_MB_LOCKED_ROTOR_AL,              13, MASK_HA_ZA_ON_WIS | MASK_ALG      , lcd_vent_locked_rotor_al                      , ALARM_INDEX },
  { VENT_MB_UNDERSPEED_AL,                13, MASK_HA_ZA_ON_WIS | MASK_ALG      , lcd_vent_underspeed_al                        , ALARM_INDEX },
  { VENT_MB_24V_SUPPLY_OVERLOADED_AL,     13, MASK_HA_ZA_ON_WIS | MASK_ALG      , lcd_vent_24V_supply_overloaded_al             , ALARM_INDEX },
  { VENT_MB_INPUT_PHASE_ERROR_AL,         13, MASK_HA_ZA_ON_WIS | MASK_ALG      , lcd_vent_input_phase_error_al                 , ALARM_INDEX },
  { VENT_MB_MOTOR_PHASE_ERROR_AL,         13, MASK_HA_ZA_ON_WIS | MASK_ALG      , lcd_vent_motor_phase_error_al                 , ALARM_INDEX },
  { VENT_MB_MEMORY_ERROR_AL,              13, MASK_HA_ZA_ON_WIS | MASK_ALG      , lcd_vent_memory_error_al                      , ALARM_INDEX },
  { VENT_MB_SHORT_CIRCUIT_AL,             13, MASK_HA_ZA_ON_WIS | MASK_ALG      , lcd_vent_short_circuit_al                     , ALARM_INDEX },
  { VENT_MB_LOSS_OF_SYNCHRONISM_AL,       13, MASK_HA_ZA_ON_WIS | MASK_ALG      , lcd_vent_loss_of_synchronism_al               , ALARM_INDEX },
  { VENT_MB_INPUT_VOLTAGE_ERROR_AL,       13, MASK_HA_ZA_ON_WIS | MASK_ALG      , lcd_vent_input_voltage_error_al               , ALARM_INDEX },
  { VENT_MB_INPUT_RELAY_NOT_CLOSED_AL,    13, MASK_HA_ZA_ON_WIS | MASK_ALG      , lcd_vent_input_relay_not_closed_al            , ALARM_INDEX },
  { VENT_MB_HIGH_STARTING_CURRENT_AL,     13, MASK_HA_ZA_ON_WIS | MASK_ALG      , lcd_vent_high_starting_current_al             , ALARM_INDEX },

  { KLEP_MB_COMMUNICATION_AL,             12, MASK_HA_ZA_ON_WIS | MASK_ALG      , lcd_klep_communication_al                     , ALARM_INDEX },

  { DRUKVERSCHIL_MB_COMMUNICATION_AL,     12, MASK_HA_ZA_ON_WIS | MASK_ALG      , lcd_drukverschil_communication_al             , ALARM_INDEX },

  { MOTOR_UNKNOWN_AL,                     23, MASK_HA_ZA_ON     | MASK_ALG      , lcd_motor_unknown_al                          , ALARM_INDEX },

  { MOTORGROUP_MANUAL_AL,                 18, MASK_HA_ZA_ON     | MASK_ALG      , lcd_motorgroup_manual_al                      , ALARM_INDEX },
  { LUCHTMENGKASTGROEP_MANUAL_AL,         18, MASK_HA_ZA_ON     | MASK_ALG      , lcd_luchtmengkastgroep_manual_al              , ALARM_INDEX },
  { LUCHTMENGKAST_MANUAL_AL,              18, MASK_HA_ZA_ON     | MASK_ALG      , lcd_luchtmengkast_manual_al                   , ALARM_INDEX },

  { LUCHTMENGKAST_VORST_AL,                                 12, MASK_HA_ZA_ON_WIS | MASK_ALG, lcd_luchtmengkast_vorst_al                         , ALARM_INDEX },
  { LUCHTMENGKAST_EXTERN_AL,                                13, MASK_HA_ZA_ON     | MASK_ALG, lcd_luchtmengkast_extern_al                        , ALARM_INDEX },
  { LUCHTMENGKAST_DRUKVERSCHIL_AL,                          15, MASK_AL_ZA_ON     | MASK_ALG, lcd_luchtmengkast_drukverschil_al                  , ALARM_INDEX },
  { LUCHTMENGKAST_RECIRCKLEP_TARGET_NOT_REACHED_AL,         14, MASK_HA_ZA_ON     | MASK_ALG, lcd_luchtmengkast_recircklep_target_not_reached_al , ALARM_INDEX },
  { LUCHTMENGKAST_BUITENKLEP_TARGET_NOT_REACHED_AL,         14, MASK_HA_ZA_ON     | MASK_ALG, lcd_luchtmengkast_buitenklep_target_not_reached_al , ALARM_INDEX },
  { LUCHTMENGKAST_BINNENKLEP_TARGET_NOT_REACHED_AL,         14, MASK_HA_ZA_ON     | MASK_ALG, lcd_luchtmengkast_binnenklep_target_not_reached_al , ALARM_INDEX },
  { LUCHTMENGKAST_BOVENKLEP_TARGET_NOT_REACHED_AL,          14, MASK_HA_ZA_ON     | MASK_ALG, lcd_luchtmengkast_bovenklep_target_not_reached_al  , ALARM_INDEX },
  { LUCHTMENGKAST_INBLAASVENT_TARGET_NOT_REACHED_AL,        14, MASK_HA_ZA_ON     | MASK_ALG, lcd_luchtmengkast_inblaasvent_target_not_reached_al, ALARM_INDEX },
  { LUCHTMENGKAST_AFBLAASVENT_TARGET_NOT_REACHED_AL,        14, MASK_HA_ZA_ON     | MASK_ALG, lcd_luchtmengkast_afblaasvent_target_not_reached_al, ALARM_INDEX },
  { LUCHTMENGKAST_VERWARMING_TARGET_NOT_REACHED_AL,         14, MASK_HA_ZA_ON     | MASK_ALG, lcd_luchtmengkast_verwarming_target_not_reached_al , ALARM_INDEX },

  { LUCHTMENGKAST_INBLAASVENT_MB_COMMUNICATION_AL,             12, MASK_HA_ZA_ON     | MASK_ALG, lcd_luchtmengkast_inblaasvent_communication_al            , ALARM_INDEX },
  { LUCHTMENGKAST_INBLAASVENT_MB_LOCKED_MOTOR_AL,              13, MASK_HA_ZA_ON_WIS | MASK_ALG, lcd_luchtmengkast_inblaasvent_locked_motor_al             , ALARM_INDEX },
  { LUCHTMENGKAST_INBLAASVENT_MB_HALL_FAILURE_AL,              13, MASK_HA_ZA_ON_WIS | MASK_ALG, lcd_luchtmengkast_inblaasvent_hall_failure_al             , ALARM_INDEX },
  { LUCHTMENGKAST_INBLAASVENT_MB_THERMAL_MOTOR_AL,             13, MASK_HA_ZA_ON_WIS | MASK_ALG, lcd_luchtmengkast_inblaasvent_thermal_motor_al            , ALARM_INDEX },
  { LUCHTMENGKAST_INBLAASVENT_MB_COMM_ERROR_MASTER_SLAVE_AL,   13, MASK_HA_ZA_ON_WIS | MASK_ALG, lcd_luchtmengkast_inblaasvent_comm_error_master_slave_al  , ALARM_INDEX },
  { LUCHTMENGKAST_INBLAASVENT_MB_THERMAL_POWER_MODULE_AL,      13, MASK_HA_ZA_ON_WIS | MASK_ALG, lcd_luchtmengkast_inblaasvent_thermal_power_module_al     , ALARM_INDEX },
  { LUCHTMENGKAST_INBLAASVENT_MB_COMM_ERROR_REMOTE_UNIT_AL,    13, MASK_HA_ZA_ON_WIS | MASK_ALG, lcd_luchtmengkast_inblaasvent_comm_error_remote_unit_al   , ALARM_INDEX },
  { LUCHTMENGKAST_INBLAASVENT_MB_PHASE_FAILURE_AL,             13, MASK_HA_ZA_ON_WIS | MASK_ALG, lcd_luchtmengkast_inblaasvent_phase_failure_al            , ALARM_INDEX },
  { LUCHTMENGKAST_INBLAASVENT_MB_BRAKE_AL,                     13, MASK_HA_ZA_ON_WIS | MASK_ALG, lcd_luchtmengkast_inblaasvent_brake_al                    , ALARM_INDEX },
  { LUCHTMENGKAST_INBLAASVENT_MB_HIGH_LINE_VOLTAGE_AL,         13, MASK_HA_ZA_ON_WIS | MASK_ALG, lcd_luchtmengkast_inblaasvent_high_line_voltage_al        , ALARM_INDEX },
  { LUCHTMENGKAST_INBLAASVENT_MB_LOW_LINE_VOLTAGE_AL,          13, MASK_HA_ZA_ON_WIS | MASK_ALG, lcd_luchtmengkast_inblaasvent_low_line_voltage_al         , ALARM_INDEX },
  { LUCHTMENGKAST_INBLAASVENT_MB_LOW_DC_LINK_VOLTAGE_AL,       13, MASK_HA_ZA_ON_WIS | MASK_ALG, lcd_luchtmengkast_inblaasvent_low_dc_link_voltage_al      , ALARM_INDEX },
  { LUCHTMENGKAST_INBLAASVENT_MB_HIGH_DC_LINK_VOLTAGE_AL,      13, MASK_HA_ZA_ON_WIS | MASK_ALG, lcd_luchtmengkast_inblaasvent_high_dc_link_voltage_al     , ALARM_INDEX },
  { LUCHTMENGKAST_INBLAASVENT_MB_DRIVER_PROBLEM_AL,            13, MASK_HA_ZA_ON_WIS | MASK_ALG, lcd_luchtmengkast_inblaasvent_driver_problem_al           , ALARM_INDEX },
  { LUCHTMENGKAST_INBLAASVENT_MB_ELECTRONIC_BOX_OVER_HEAT_AL,  13, MASK_HA_ZA_ON_WIS | MASK_ALG, lcd_luchtmengkast_inblaasvent_electronic_box_over_heat_al , ALARM_INDEX },
  { LUCHTMENGKAST_INBLAASVENT_MB_EXCESSIVE_DC_LINK_CURRENT_AL, 13, MASK_HA_ZA_ON_WIS | MASK_ALG, lcd_luchtmengkast_inblaasvent_excessive_dc_link_current_al, ALARM_INDEX },
  { LUCHTMENGKAST_INBLAASVENT_MB_UNKNOWN_AL,                   15, MASK_HA_ZA_ON_WIS | MASK_ALG, lcd_luchtmengkast_inblaasvent_unknown_al                  , ALARM_INDEX },

  { LUCHTMENGKAST_INBLAASVENT_MB_GENERAL_ERROR_AL,             13, MASK_HA_ZA_ON_WIS | MASK_ALG, lcd_luchtmengkast_inblaasvent_general_error_al            , ALARM_INDEX },
  { LUCHTMENGKAST_INBLAASVENT_MB_MOTOR_FAULT_AL,               13, MASK_HA_ZA_ON_WIS | MASK_ALG, lcd_luchtmengkast_inblaasvent_motor_fault_al              , ALARM_INDEX }, 
  { LUCHTMENGKAST_INBLAASVENT_MB_MOTOR_BLOCKED_AL,             13, MASK_HA_ZA_ON_WIS | MASK_ALG, lcd_luchtmengkast_inblaasvent_motor_blocked_al            , ALARM_INDEX },
  { LUCHTMENGKAST_INBLAASVENT_MB_HEAT_SINK_TEMPERATURE_AL,     13, MASK_HA_ZA_ON_WIS | MASK_ALG, lcd_luchtmengkast_inblaasvent_heat_sink_temperature_al    , ALARM_INDEX },
  { LUCHTMENGKAST_INBLAASVENT_MB_GROUND_FAULT_AL,              13, MASK_HA_ZA_ON_WIS | MASK_ALG, lcd_luchtmengkast_inblaasvent_ground_fault_al             , ALARM_INDEX },
  { LUCHTMENGKAST_INBLAASVENT_MB_HALL_IC_FAULT_AL,             13, MASK_HA_ZA_ON_WIS | MASK_ALG, lcd_luchtmengkast_inblaasvent_hall_ic_fault_al            , ALARM_INDEX },
  { LUCHTMENGKAST_INBLAASVENT_MB_OVERCURRENT_AL,               13, MASK_HA_ZA_ON_WIS | MASK_ALG, lcd_luchtmengkast_inblaasvent_overcurrent_al              , ALARM_INDEX },
  { LUCHTMENGKAST_INBLAASVENT_MB_LINE_FAULT_AL,                13, MASK_HA_ZA_ON_WIS | MASK_ALG, lcd_luchtmengkast_inblaasvent_line_fault_al               , ALARM_INDEX },
  { LUCHTMENGKAST_INBLAASVENT_MB_LINE_INT_HEAT_SINK_SENSOR_AL, 13, MASK_HA_ZA_ON_WIS | MASK_ALG, lcd_luchtmengkast_inblaasvent_line_int_heat_sink_sensor_al, ALARM_INDEX },
  { LUCHTMENGKAST_INBLAASVENT_MB_DC_RES_VOLTAGE_TO_HIGH_AL,    13, MASK_HA_ZA_ON_WIS | MASK_ALG, lcd_luchtmengkast_inblaasvent_dc_res_voltage_to_high_al   , ALARM_INDEX },
  { LUCHTMENGKAST_INBLAASVENT_MB_WRONG_DIRECTION_AL,           13, MASK_HA_ZA_ON_WIS | MASK_ALG, lcd_luchtmengkast_inblaasvent_wrong_direction_al          , ALARM_INDEX },
  { LUCHTMENGKAST_INBLAASVENT_MB_TEMPERATURE_LOWERING_AL,      13, MASK_HA_ZA_ON_WIS | MASK_ALG, lcd_luchtmengkast_inblaasvent_temperature_lowering_al     , ALARM_INDEX },
  { LUCHTMENGKAST_INBLAASVENT_MB_WRONG_CONNECTION_AL,          13, MASK_HA_ZA_ON_WIS | MASK_ALG, lcd_luchtmengkast_inblaasvent_wrong_connection_al         , ALARM_INDEX },
  { LUCHTMENGKAST_INBLAASVENT_MB_EXTERNAL_FAULT_AL,            13, MASK_HA_ZA_ON_WIS | MASK_ALG, lcd_luchtmengkast_inblaasvent_external_fault_al           , ALARM_INDEX },
  { LUCHTMENGKAST_INBLAASVENT_MB_FACTORY_SETTINGS_AL,          13, MASK_HA_ZA_ON_WIS | MASK_ALG, lcd_luchtmengkast_inblaasvent_factory_settings_al         , ALARM_INDEX },
  { LUCHTMENGKAST_INBLAASVENT_MB_EEP_ERROR_AL,                 13, MASK_HA_ZA_ON_WIS | MASK_ALG, lcd_luchtmengkast_inblaasvent_eep_error_al                , ALARM_INDEX },
  { LUCHTMENGKAST_INBLAASVENT_MB_RTC_GENERAL_FAULT_AL,         13, MASK_HA_ZA_ON_WIS | MASK_ALG, lcd_luchtmengkast_inblaasvent_rtc_general_fault_al        , ALARM_INDEX },
  { LUCHTMENGKAST_INBLAASVENT_MB_RTC_VOLTAGE_FAULT_AL,         13, MASK_HA_ZA_ON_WIS | MASK_ALG, lcd_luchtmengkast_inblaasvent_rtc_voltage_fault_al        , ALARM_INDEX },
  { LUCHTMENGKAST_INBLAASVENT_MB_FILTER_CONTAMINATION_AL,      13, MASK_HA_ZA_ON_WIS | MASK_ALG, lcd_luchtmengkast_inblaasvent_filter_contamination_al     , ALARM_INDEX },
  { LUCHTMENGKAST_INBLAASVENT_MB_TRANSFER_ERROR_AL,            13, MASK_HA_ZA_ON_WIS | MASK_ALG, lcd_luchtmengkast_inblaasvent_transfer_error_al           , ALARM_INDEX },
  { LUCHTMENGKAST_INBLAASVENT_MB_DATA_CONNECTION_LINE_AL,      13, MASK_HA_ZA_ON_WIS | MASK_ALG, lcd_luchtmengkast_inblaasvent_data_connection_line_al     , ALARM_INDEX },
  { LUCHTMENGKAST_INBLAASVENT_MB_DATA_CONNECTION_CHECKSUM_AL,  13, MASK_HA_ZA_ON_WIS | MASK_ALG, lcd_luchtmengkast_inblaasvent_data_connection_checksum_al , ALARM_INDEX },
  { LUCHTMENGKAST_INBLAASVENT_MB_SENSOR_FAULT_INPUT_1_AL,      13, MASK_HA_ZA_ON_WIS | MASK_ALG, lcd_luchtmengkast_inblaasvent_sensor_fault_input_1_al     , ALARM_INDEX },
  { LUCHTMENGKAST_INBLAASVENT_MB_SENSOR_FAULT_INPUT_2_AL,      13, MASK_HA_ZA_ON_WIS | MASK_ALG, lcd_luchtmengkast_inblaasvent_sensor_fault_input_2_al     , ALARM_INDEX },
  { LUCHTMENGKAST_INBLAASVENT_MB_SENSOR_FAULT_INPUT_3_AL,      13, MASK_HA_ZA_ON_WIS | MASK_ALG, lcd_luchtmengkast_inblaasvent_sensor_fault_input_3_al     , ALARM_INDEX },
  { LUCHTMENGKAST_INBLAASVENT_MB_I_LIMIT_AL,                   13, MASK_HA_ZA_ON_WIS | MASK_ALG, lcd_luchtmengkast_inblaasvent_i_limit_al                  , ALARM_INDEX },
  { LUCHTMENGKAST_INBLAASVENT_MB_P_LIMIT_AL,                   13, MASK_HA_ZA_ON_WIS | MASK_ALG, lcd_luchtmengkast_inblaasvent_p_limit_al                  , ALARM_INDEX },
  { LUCHTMENGKAST_INBLAASVENT_MB_TE_HIGH_AL,                   13, MASK_HA_ZA_ON_WIS | MASK_ALG, lcd_luchtmengkast_inblaasvent_te_high_al                  , ALARM_INDEX },
  { LUCHTMENGKAST_INBLAASVENT_MB_TM_HIGH_AL,                   13, MASK_HA_ZA_ON_WIS | MASK_ALG, lcd_luchtmengkast_inblaasvent_tm_high_al                  , ALARM_INDEX },
  { LUCHTMENGKAST_INBLAASVENT_MB_TEI_HIGH_AL,                  13, MASK_HA_ZA_ON_WIS | MASK_ALG, lcd_luchtmengkast_inblaasvent_tei_high_al                 , ALARM_INDEX },
  { LUCHTMENGKAST_INBLAASVENT_MB_UZ_LOW_AL,                    13, MASK_HA_ZA_ON_WIS | MASK_ALG, lcd_luchtmengkast_inblaasvent_uz_low_al                   , ALARM_INDEX },
  { LUCHTMENGKAST_INBLAASVENT_MB_N_LOW_AL,                     13, MASK_HA_ZA_ON_WIS | MASK_ALG, lcd_luchtmengkast_inblaasvent_n_low_al                    , ALARM_INDEX },
  { LUCHTMENGKAST_INBLAASVENT_MB_IGBT_FAULT_AL,                13, MASK_HA_ZA_ON_WIS | MASK_ALG, lcd_luchtmengkast_inblaasvent_igbt_fault_al               , ALARM_INDEX },
  { LUCHTMENGKAST_INBLAASVENT_MB_UZK_HI_AL,                    13, MASK_HA_ZA_ON_WIS | MASK_ALG, lcd_luchtmengkast_inblaasvent_uzk_hi_al                   , ALARM_INDEX },
  { LUCHTMENGKAST_INBLAASVENT_MB_UZK_LO_AL,                    13, MASK_HA_ZA_ON_WIS | MASK_ALG, lcd_luchtmengkast_inblaasvent_uzk_lo_al                   , ALARM_INDEX },
  { LUCHTMENGKAST_INBLAASVENT_MB_UIN_HI_AL,                    13, MASK_HA_ZA_ON_WIS | MASK_ALG, lcd_luchtmengkast_inblaasvent_uin_hi_al                   , ALARM_INDEX },
  { LUCHTMENGKAST_INBLAASVENT_MB_UIN_LO_AL,                    13, MASK_HA_ZA_ON_WIS | MASK_ALG, lcd_luchtmengkast_inblaasvent_uin_lo_al                   , ALARM_INDEX },
  { LUCHTMENGKAST_INBLAASVENT_MB_FAILURE_POWER_SECTION_AL,     13, MASK_HA_ZA_ON_WIS | MASK_ALG, lcd_luchtmengkast_inblaasvent_failure_power_section_al    , ALARM_INDEX },
  { LUCHTMENGKAST_INBLAASVENT_MB_UMAX_AL,                      13, MASK_HA_ZA_ON_WIS | MASK_ALG, lcd_luchtmengkast_inblaasvent_umax_al                     , ALARM_INDEX },
  { LUCHTMENGKAST_INBLAASVENT_MB_UMIN_AL,                      13, MASK_HA_ZA_ON_WIS | MASK_ALG, lcd_luchtmengkast_inblaasvent_umin_al                     , ALARM_INDEX },
  { LUCHTMENGKAST_INBLAASVENT_MB_OVERSPEED_AL,                 13, MASK_HA_ZA_ON_WIS | MASK_ALG, lcd_luchtmengkast_inblaasvent_overspeed_al                , ALARM_INDEX },
  { LUCHTMENGKAST_INBLAASVENT_MB_LOCKED_ROTOR_AL,              13, MASK_HA_ZA_ON_WIS | MASK_ALG, lcd_luchtmengkast_inblaasvent_locked_rotor_al             , ALARM_INDEX },
  { LUCHTMENGKAST_INBLAASVENT_MB_UNDERSPEED_AL,                13, MASK_HA_ZA_ON_WIS | MASK_ALG, lcd_luchtmengkast_inblaasvent_underspeed_al               , ALARM_INDEX },
  { LUCHTMENGKAST_INBLAASVENT_MB_24V_SUPPLY_OVERLOADED_AL,     13, MASK_HA_ZA_ON_WIS | MASK_ALG, lcd_luchtmengkast_inblaasvent_24V_supply_overloaded_al    , ALARM_INDEX },
  { LUCHTMENGKAST_INBLAASVENT_MB_INPUT_PHASE_ERROR_AL,         13, MASK_HA_ZA_ON_WIS | MASK_ALG, lcd_luchtmengkast_inblaasvent_input_phase_error_al        , ALARM_INDEX },
  { LUCHTMENGKAST_INBLAASVENT_MB_MOTOR_PHASE_ERROR_AL,         13, MASK_HA_ZA_ON_WIS | MASK_ALG, lcd_luchtmengkast_inblaasvent_motor_phase_error_al        , ALARM_INDEX },
  { LUCHTMENGKAST_INBLAASVENT_MB_MEMORY_ERROR_AL,              13, MASK_HA_ZA_ON_WIS | MASK_ALG, lcd_luchtmengkast_inblaasvent_memory_error_al             , ALARM_INDEX },
  { LUCHTMENGKAST_INBLAASVENT_MB_SHORT_CIRCUIT_AL,             13, MASK_HA_ZA_ON_WIS | MASK_ALG, lcd_luchtmengkast_inblaasvent_short_circuit_al            , ALARM_INDEX },
  { LUCHTMENGKAST_INBLAASVENT_MB_LOSS_OF_SYNCHRONISM_AL,       13, MASK_HA_ZA_ON_WIS | MASK_ALG, lcd_luchtmengkast_inblaasvent_loss_of_synchronism_al      , ALARM_INDEX },
  { LUCHTMENGKAST_INBLAASVENT_MB_INPUT_VOLTAGE_ERROR_AL,       13, MASK_HA_ZA_ON_WIS | MASK_ALG, lcd_luchtmengkast_inblaasvent_input_voltage_error_al      , ALARM_INDEX },
  { LUCHTMENGKAST_INBLAASVENT_MB_INPUT_RELAY_NOT_CLOSED_AL,    13, MASK_HA_ZA_ON_WIS | MASK_ALG, lcd_luchtmengkast_inblaasvent_input_relay_not_closed_al   , ALARM_INDEX },
  { LUCHTMENGKAST_INBLAASVENT_MB_HIGH_STARTING_CURRENT_AL,     13, MASK_HA_ZA_ON_WIS | MASK_ALG, lcd_luchtmengkast_inblaasvent_high_starting_current_al    , ALARM_INDEX },

  { LUCHTMENGKAST_AFBLAASVENT_MB_COMMUNICATION_AL,             12, MASK_HA_ZA_ON     | MASK_ALG, lcd_luchtmengkast_afblaasvent_communication_al            , ALARM_INDEX },
  { LUCHTMENGKAST_AFBLAASVENT_MB_LOCKED_MOTOR_AL,              13, MASK_HA_ZA_ON_WIS | MASK_ALG, lcd_luchtmengkast_afblaasvent_locked_motor_al             , ALARM_INDEX },
  { LUCHTMENGKAST_AFBLAASVENT_MB_HALL_FAILURE_AL,              13, MASK_HA_ZA_ON_WIS | MASK_ALG, lcd_luchtmengkast_afblaasvent_hall_failure_al             , ALARM_INDEX },
  { LUCHTMENGKAST_AFBLAASVENT_MB_THERMAL_MOTOR_AL,             13, MASK_HA_ZA_ON_WIS | MASK_ALG, lcd_luchtmengkast_afblaasvent_thermal_motor_al            , ALARM_INDEX },
  { LUCHTMENGKAST_AFBLAASVENT_MB_COMM_ERROR_MASTER_SLAVE_AL,   13, MASK_HA_ZA_ON_WIS | MASK_ALG, lcd_luchtmengkast_afblaasvent_comm_error_master_slave_al  , ALARM_INDEX },
  { LUCHTMENGKAST_AFBLAASVENT_MB_THERMAL_POWER_MODULE_AL,      13, MASK_HA_ZA_ON_WIS | MASK_ALG, lcd_luchtmengkast_afblaasvent_thermal_power_module_al     , ALARM_INDEX },
  { LUCHTMENGKAST_AFBLAASVENT_MB_COMM_ERROR_REMOTE_UNIT_AL,    13, MASK_HA_ZA_ON_WIS | MASK_ALG, lcd_luchtmengkast_afblaasvent_comm_error_remote_unit_al   , ALARM_INDEX },
  { LUCHTMENGKAST_AFBLAASVENT_MB_PHASE_FAILURE_AL,             13, MASK_HA_ZA_ON_WIS | MASK_ALG, lcd_luchtmengkast_afblaasvent_phase_failure_al            , ALARM_INDEX },
  { LUCHTMENGKAST_AFBLAASVENT_MB_BRAKE_AL,                     13, MASK_HA_ZA_ON_WIS | MASK_ALG, lcd_luchtmengkast_afblaasvent_brake_al                    , ALARM_INDEX },
  { LUCHTMENGKAST_AFBLAASVENT_MB_HIGH_LINE_VOLTAGE_AL,         13, MASK_HA_ZA_ON_WIS | MASK_ALG, lcd_luchtmengkast_afblaasvent_high_line_voltage_al        , ALARM_INDEX },
  { LUCHTMENGKAST_AFBLAASVENT_MB_LOW_LINE_VOLTAGE_AL,          13, MASK_HA_ZA_ON_WIS | MASK_ALG, lcd_luchtmengkast_afblaasvent_low_line_voltage_al         , ALARM_INDEX },
  { LUCHTMENGKAST_AFBLAASVENT_MB_LOW_DC_LINK_VOLTAGE_AL,       13, MASK_HA_ZA_ON_WIS | MASK_ALG, lcd_luchtmengkast_afblaasvent_low_dc_link_voltage_al      , ALARM_INDEX },
  { LUCHTMENGKAST_AFBLAASVENT_MB_HIGH_DC_LINK_VOLTAGE_AL,      13, MASK_HA_ZA_ON_WIS | MASK_ALG, lcd_luchtmengkast_afblaasvent_high_dc_link_voltage_al     , ALARM_INDEX },
  { LUCHTMENGKAST_AFBLAASVENT_MB_DRIVER_PROBLEM_AL,            13, MASK_HA_ZA_ON_WIS | MASK_ALG, lcd_luchtmengkast_afblaasvent_driver_problem_al           , ALARM_INDEX },
  { LUCHTMENGKAST_AFBLAASVENT_MB_ELECTRONIC_BOX_OVER_HEAT_AL,  13, MASK_HA_ZA_ON_WIS | MASK_ALG, lcd_luchtmengkast_afblaasvent_electronic_box_over_heat_al , ALARM_INDEX },
  { LUCHTMENGKAST_AFBLAASVENT_MB_EXCESSIVE_DC_LINK_CURRENT_AL, 13, MASK_HA_ZA_ON_WIS | MASK_ALG, lcd_luchtmengkast_afblaasvent_excessive_dc_link_current_al, ALARM_INDEX },
  { LUCHTMENGKAST_AFBLAASVENT_MB_UNKNOWN_AL,                   15, MASK_HA_ZA_ON_WIS | MASK_ALG, lcd_luchtmengkast_afblaasvent_unknown_al                  , ALARM_INDEX },

  { LUCHTMENGKAST_AFBLAASVENT_MB_GENERAL_ERROR_AL,             13, MASK_HA_ZA_ON_WIS | MASK_ALG, lcd_luchtmengkast_afblaasvent_general_error_al            , ALARM_INDEX },
  { LUCHTMENGKAST_AFBLAASVENT_MB_MOTOR_FAULT_AL,               13, MASK_HA_ZA_ON_WIS | MASK_ALG, lcd_luchtmengkast_afblaasvent_motor_fault_al              , ALARM_INDEX }, 
  { LUCHTMENGKAST_AFBLAASVENT_MB_MOTOR_BLOCKED_AL,             13, MASK_HA_ZA_ON_WIS | MASK_ALG, lcd_luchtmengkast_afblaasvent_motor_blocked_al            , ALARM_INDEX },
  { LUCHTMENGKAST_AFBLAASVENT_MB_HEAT_SINK_TEMPERATURE_AL,     13, MASK_HA_ZA_ON_WIS | MASK_ALG, lcd_luchtmengkast_afblaasvent_heat_sink_temperature_al    , ALARM_INDEX },
  { LUCHTMENGKAST_AFBLAASVENT_MB_GROUND_FAULT_AL,              13, MASK_HA_ZA_ON_WIS | MASK_ALG, lcd_luchtmengkast_afblaasvent_ground_fault_al             , ALARM_INDEX },
  { LUCHTMENGKAST_AFBLAASVENT_MB_HALL_IC_FAULT_AL,             13, MASK_HA_ZA_ON_WIS | MASK_ALG, lcd_luchtmengkast_afblaasvent_hall_ic_fault_al            , ALARM_INDEX },
  { LUCHTMENGKAST_AFBLAASVENT_MB_OVERCURRENT_AL,               13, MASK_HA_ZA_ON_WIS | MASK_ALG, lcd_luchtmengkast_afblaasvent_overcurrent_al              , ALARM_INDEX },
  { LUCHTMENGKAST_AFBLAASVENT_MB_LINE_FAULT_AL,                13, MASK_HA_ZA_ON_WIS | MASK_ALG, lcd_luchtmengkast_afblaasvent_line_fault_al               , ALARM_INDEX },
  { LUCHTMENGKAST_AFBLAASVENT_MB_LINE_INT_HEAT_SINK_SENSOR_AL, 13, MASK_HA_ZA_ON_WIS | MASK_ALG, lcd_luchtmengkast_afblaasvent_line_int_heat_sink_sensor_al, ALARM_INDEX },
  { LUCHTMENGKAST_AFBLAASVENT_MB_DC_RES_VOLTAGE_TO_HIGH_AL,    13, MASK_HA_ZA_ON_WIS | MASK_ALG, lcd_luchtmengkast_afblaasvent_dc_res_voltage_to_high_al   , ALARM_INDEX },
  { LUCHTMENGKAST_AFBLAASVENT_MB_WRONG_DIRECTION_AL,           13, MASK_HA_ZA_ON_WIS | MASK_ALG, lcd_luchtmengkast_afblaasvent_wrong_direction_al          , ALARM_INDEX },
  { LUCHTMENGKAST_AFBLAASVENT_MB_TEMPERATURE_LOWERING_AL,      13, MASK_HA_ZA_ON_WIS | MASK_ALG, lcd_luchtmengkast_afblaasvent_temperature_lowering_al     , ALARM_INDEX },
  { LUCHTMENGKAST_AFBLAASVENT_MB_WRONG_CONNECTION_AL,          13, MASK_HA_ZA_ON_WIS | MASK_ALG, lcd_luchtmengkast_afblaasvent_wrong_connection_al         , ALARM_INDEX },
  { LUCHTMENGKAST_AFBLAASVENT_MB_EXTERNAL_FAULT_AL,            13, MASK_HA_ZA_ON_WIS | MASK_ALG, lcd_luchtmengkast_afblaasvent_external_fault_al           , ALARM_INDEX },
  { LUCHTMENGKAST_AFBLAASVENT_MB_FACTORY_SETTINGS_AL,          13, MASK_HA_ZA_ON_WIS | MASK_ALG, lcd_luchtmengkast_afblaasvent_factory_settings_al         , ALARM_INDEX },
  { LUCHTMENGKAST_AFBLAASVENT_MB_EEP_ERROR_AL,                 13, MASK_HA_ZA_ON_WIS | MASK_ALG, lcd_luchtmengkast_afblaasvent_eep_error_al                , ALARM_INDEX },
  { LUCHTMENGKAST_AFBLAASVENT_MB_RTC_GENERAL_FAULT_AL,         13, MASK_HA_ZA_ON_WIS | MASK_ALG, lcd_luchtmengkast_afblaasvent_rtc_general_fault_al        , ALARM_INDEX },
  { LUCHTMENGKAST_AFBLAASVENT_MB_RTC_VOLTAGE_FAULT_AL,         13, MASK_HA_ZA_ON_WIS | MASK_ALG, lcd_luchtmengkast_afblaasvent_rtc_voltage_fault_al        , ALARM_INDEX },
  { LUCHTMENGKAST_AFBLAASVENT_MB_FILTER_CONTAMINATION_AL,      13, MASK_HA_ZA_ON_WIS | MASK_ALG, lcd_luchtmengkast_afblaasvent_filter_contamination_al     , ALARM_INDEX },
  { LUCHTMENGKAST_AFBLAASVENT_MB_TRANSFER_ERROR_AL,            13, MASK_HA_ZA_ON_WIS | MASK_ALG, lcd_luchtmengkast_afblaasvent_transfer_error_al           , ALARM_INDEX },
  { LUCHTMENGKAST_AFBLAASVENT_MB_DATA_CONNECTION_LINE_AL,      13, MASK_HA_ZA_ON_WIS | MASK_ALG, lcd_luchtmengkast_afblaasvent_data_connection_line_al     , ALARM_INDEX },
  { LUCHTMENGKAST_AFBLAASVENT_MB_DATA_CONNECTION_CHECKSUM_AL,  13, MASK_HA_ZA_ON_WIS | MASK_ALG, lcd_luchtmengkast_afblaasvent_data_connection_checksum_al , ALARM_INDEX },
  { LUCHTMENGKAST_AFBLAASVENT_MB_SENSOR_FAULT_INPUT_1_AL,      13, MASK_HA_ZA_ON_WIS | MASK_ALG, lcd_luchtmengkast_afblaasvent_sensor_fault_input_1_al     , ALARM_INDEX },
  { LUCHTMENGKAST_AFBLAASVENT_MB_SENSOR_FAULT_INPUT_2_AL,      13, MASK_HA_ZA_ON_WIS | MASK_ALG, lcd_luchtmengkast_afblaasvent_sensor_fault_input_2_al     , ALARM_INDEX },
  { LUCHTMENGKAST_AFBLAASVENT_MB_SENSOR_FAULT_INPUT_3_AL,      13, MASK_HA_ZA_ON_WIS | MASK_ALG, lcd_luchtmengkast_afblaasvent_sensor_fault_input_3_al     , ALARM_INDEX },
  { LUCHTMENGKAST_AFBLAASVENT_MB_I_LIMIT_AL,                   13, MASK_HA_ZA_ON_WIS | MASK_ALG, lcd_luchtmengkast_afblaasvent_i_limit_al                  , ALARM_INDEX },
  { LUCHTMENGKAST_AFBLAASVENT_MB_P_LIMIT_AL,                   13, MASK_HA_ZA_ON_WIS | MASK_ALG, lcd_luchtmengkast_afblaasvent_p_limit_al                  , ALARM_INDEX },
  { LUCHTMENGKAST_AFBLAASVENT_MB_TE_HIGH_AL,                   13, MASK_HA_ZA_ON_WIS | MASK_ALG, lcd_luchtmengkast_afblaasvent_te_high_al                  , ALARM_INDEX },
  { LUCHTMENGKAST_AFBLAASVENT_MB_TM_HIGH_AL,                   13, MASK_HA_ZA_ON_WIS | MASK_ALG, lcd_luchtmengkast_afblaasvent_tm_high_al                  , ALARM_INDEX },
  { LUCHTMENGKAST_AFBLAASVENT_MB_TEI_HIGH_AL,                  13, MASK_HA_ZA_ON_WIS | MASK_ALG, lcd_luchtmengkast_afblaasvent_tei_high_al                 , ALARM_INDEX },
  { LUCHTMENGKAST_AFBLAASVENT_MB_UZ_LOW_AL,                    13, MASK_HA_ZA_ON_WIS | MASK_ALG, lcd_luchtmengkast_afblaasvent_uz_low_al                   , ALARM_INDEX },
  { LUCHTMENGKAST_AFBLAASVENT_MB_N_LOW_AL,                     13, MASK_HA_ZA_ON_WIS | MASK_ALG, lcd_luchtmengkast_afblaasvent_n_low_al                    , ALARM_INDEX },
  { LUCHTMENGKAST_AFBLAASVENT_MB_IGBT_FAULT_AL,                13, MASK_HA_ZA_ON_WIS | MASK_ALG, lcd_luchtmengkast_afblaasvent_igbt_fault_al               , ALARM_INDEX },
  { LUCHTMENGKAST_AFBLAASVENT_MB_UZK_HI_AL,                    13, MASK_HA_ZA_ON_WIS | MASK_ALG, lcd_luchtmengkast_afblaasvent_uzk_hi_al                   , ALARM_INDEX },
  { LUCHTMENGKAST_AFBLAASVENT_MB_UZK_LO_AL,                    13, MASK_HA_ZA_ON_WIS | MASK_ALG, lcd_luchtmengkast_afblaasvent_uzk_lo_al                   , ALARM_INDEX },
  { LUCHTMENGKAST_AFBLAASVENT_MB_UIN_HI_AL,                    13, MASK_HA_ZA_ON_WIS | MASK_ALG, lcd_luchtmengkast_afblaasvent_uin_hi_al                   , ALARM_INDEX },
  { LUCHTMENGKAST_AFBLAASVENT_MB_UIN_LO_AL,                    13, MASK_HA_ZA_ON_WIS | MASK_ALG, lcd_luchtmengkast_afblaasvent_uin_lo_al                   , ALARM_INDEX },
  { LUCHTMENGKAST_AFBLAASVENT_MB_FAILURE_POWER_SECTION_AL,     13, MASK_HA_ZA_ON_WIS | MASK_ALG, lcd_luchtmengkast_afblaasvent_failure_power_section_al    , ALARM_INDEX },
  { LUCHTMENGKAST_AFBLAASVENT_MB_UMAX_AL,                      13, MASK_HA_ZA_ON_WIS | MASK_ALG, lcd_luchtmengkast_afblaasvent_umax_al                     , ALARM_INDEX },
  { LUCHTMENGKAST_AFBLAASVENT_MB_UMIN_AL,                      13, MASK_HA_ZA_ON_WIS | MASK_ALG, lcd_luchtmengkast_afblaasvent_umin_al                     , ALARM_INDEX },
  { LUCHTMENGKAST_AFBLAASVENT_MB_OVERSPEED_AL,                 13, MASK_HA_ZA_ON_WIS | MASK_ALG, lcd_luchtmengkast_afblaasvent_overspeed_al                , ALARM_INDEX },
  { LUCHTMENGKAST_AFBLAASVENT_MB_LOCKED_ROTOR_AL,              13, MASK_HA_ZA_ON_WIS | MASK_ALG, lcd_luchtmengkast_afblaasvent_locked_rotor_al             , ALARM_INDEX },
  { LUCHTMENGKAST_AFBLAASVENT_MB_UNDERSPEED_AL,                13, MASK_HA_ZA_ON_WIS | MASK_ALG, lcd_luchtmengkast_afblaasvent_underspeed_al               , ALARM_INDEX },
  { LUCHTMENGKAST_AFBLAASVENT_MB_24V_SUPPLY_OVERLOADED_AL,     13, MASK_HA_ZA_ON_WIS | MASK_ALG, lcd_luchtmengkast_afblaasvent_24V_supply_overloaded_al    , ALARM_INDEX },
  { LUCHTMENGKAST_AFBLAASVENT_MB_INPUT_PHASE_ERROR_AL,         13, MASK_HA_ZA_ON_WIS | MASK_ALG, lcd_luchtmengkast_afblaasvent_input_phase_error_al        , ALARM_INDEX },
  { LUCHTMENGKAST_AFBLAASVENT_MB_MOTOR_PHASE_ERROR_AL,         13, MASK_HA_ZA_ON_WIS | MASK_ALG, lcd_luchtmengkast_afblaasvent_motor_phase_error_al        , ALARM_INDEX },
  { LUCHTMENGKAST_AFBLAASVENT_MB_MEMORY_ERROR_AL,              13, MASK_HA_ZA_ON_WIS | MASK_ALG, lcd_luchtmengkast_afblaasvent_memory_error_al             , ALARM_INDEX },
  { LUCHTMENGKAST_AFBLAASVENT_MB_SHORT_CIRCUIT_AL,             13, MASK_HA_ZA_ON_WIS | MASK_ALG, lcd_luchtmengkast_afblaasvent_short_circuit_al            , ALARM_INDEX },
  { LUCHTMENGKAST_AFBLAASVENT_MB_LOSS_OF_SYNCHRONISM_AL,       13, MASK_HA_ZA_ON_WIS | MASK_ALG, lcd_luchtmengkast_afblaasvent_loss_of_synchronism_al      , ALARM_INDEX },
  { LUCHTMENGKAST_AFBLAASVENT_MB_INPUT_VOLTAGE_ERROR_AL,       13, MASK_HA_ZA_ON_WIS | MASK_ALG, lcd_luchtmengkast_afblaasvent_input_voltage_error_al      , ALARM_INDEX },
  { LUCHTMENGKAST_AFBLAASVENT_MB_INPUT_RELAY_NOT_CLOSED_AL,    13, MASK_HA_ZA_ON_WIS | MASK_ALG, lcd_luchtmengkast_afblaasvent_input_relay_not_closed_al   , ALARM_INDEX },
  { LUCHTMENGKAST_AFBLAASVENT_MB_HIGH_STARTING_CURRENT_AL,     13, MASK_HA_ZA_ON_WIS | MASK_ALG, lcd_luchtmengkast_afblaasvent_high_starting_current_al    , ALARM_INDEX },

	 
  // Zachte alarmen
  { ORION_ON_AL,                                24, MASK_AL_ZA_ON_WIS | MASK_ALG      , lcd_orion_on_al                               , ALARM_VAL_0 }, // niet nodig geen temperaturen
  { SYSTEEM_AL_VAL_HR,                           5, MASK_AL_ZACHT_WIS | MASK_ALG      , lcd_systeem_al_value_hr                       , ALARM_VAL_0 },
  { SYSTEEM_AL_RTC,                              6, MASK_AL_ZACHT_WIS | MASK_ALG      , lcd_systeem_al_RTC                            , ALARM_VAL_0 },
  { SYSTEEM_AL_TIMER_1ms,                        7, MASK_AL_ZACHT_WIS | MASK_ALG      , lcd_systeem_al_TIMER_1ms                      , ALARM_VAL_0 },
  { SYSTEEM_AL_HTRAP,                            8, MASK_AL_ZACHT_WIS | MASK_ALG      , lcd_systeem_al_HTRAP                          , ALARM_VAL_0 },
  { SYSTEEM_AL_PLL,                              9, MASK_AL_ZACHT_WIS | MASK_ALG      , lcd_systeem_al_PLL                            , ALARM_VAL_0 },
  { SYSTEEM_AL_NEW_OPTION_SETPOINT,             19, MASK_AL_ZACHT_WIS | MASK_ALG_DISP , lcd_systeem_al_new_option_setpoint            , ALARM_VAL_0 },
  { SYSTEEM_AL_NEW_OPTION,                      20, MASK_AL_ZACHT_WIS | MASK_ALG_DISP , lcd_systeem_al_new_option                     , ALARM_VAL_0 },
  { SYSTEEM_AL_NEW_SETPOINT,                    21, MASK_AL_ZACHT_WIS | MASK_ALG_DISP , lcd_systeem_al_new_setpoint                   , ALARM_VAL_0 },
  { SYSTEEM_AL_ALARM_GERESET,                  255, MASK_AL_ZACHT     | MASK_ALG      , lcd_systeem_al_alarmen_gewist                 , ALARM_VAL_0 },
  { SYSTEEM_AL_OPSTART,                        255, MASK_AL_ZACHT     | MASK_ALG      , lcd_systeem_al_opstart                        , ALARM_VAL_0 },
  { SYSTEEM_AL_EEP_TAAL,                        27, MASK_AL_ZACHT_WIS | MASK_ALG      , lcd_systeem_al_EEP_taal                       , ALARM_VAL_0 },
  { SYSTEEM_AL_GEEN_ALARM_CONTACT,              22, MASK_AL_ZACHT     | MASK_ALG      , lcd_systeem_al_geen_alarm_contact             , ALARM_VAL_0 },
  #ifdef CAN_BACKBONE_PC_WARNING
  { CAN_PC_1_MASTER_AL,                        500, MASK_AL_ZA_ON     | MASK_ALG      , lcd_can_pc_1_al                               , ALARM_VAL_0 },
  { CAN_PC_2_MASTER_AL,                        500, MASK_AL_ZA_ON     | MASK_ALG      , lcd_can_pc_2_al                               , ALARM_VAL_0 },
  { CAN_PC_3_MASTER_AL,                        500, MASK_AL_ZA_ON     | MASK_ALG      , lcd_can_pc_3_al                               , ALARM_VAL_0 },
  #else // CAN_BACKBONE_PC_WARNING                                                   
  { CAN_PC_MASTER_AL,                           25, MASK_AL_ZA_ON     | MASK_ALG      , lcd_can_pc_al                                 , ALARM_VAL_0 },
  #endif // CAN_BACKBONE_PC_WARNING

  { CAN_AL,                                     22, MASK_HA_ZA_ON     | MASK_ALG      , lcd_can_computer_al                           , ALARM_VAL_0 },

  { COMPUTER_AL,                                26, MASK_AL_ZA_ON     | MASK_ALG_DISP , lcd_computer_al                               , ALARM_VAL_0 },
  { GEEN_AL,                                   255, 0                 | MASK_ALG_DISP , lcd_geen_al                                   , ALARM_VAL_0 },
  { ONBEKEND_AL,                               255, MASK_HA_ZA_ON     | MASK_ALG_DISP , lcd_onbekend_al                               , ALARM_VAL_0 }
};
                                                                                                                                                                            
void String_Draw_Value_Func(char *string, unsigned char punt, e_type type, void *value)                                                                                    
{
unsigned char change_value = 0;
unsigned char min;
unsigned char point;
unsigned char zero = 1;
unsigned char nr_digits = 0;
char val_str[15] = "";
char *ch_ptr;
long val;

  val = Return_Value(type, value);
  if ((long)value == (long)screen_ptr->rel[screen_ptr->rel_actief].key_action->value->value) 
  {
    change_value = 1;
    if (screen_ptr->change_flag)
      val = screen_ptr->value;
    else
      screen_ptr->norm_point = screen_ptr->point = punt;
    point = screen_ptr->point;
  }
  else
    point = punt;

  val_str[14] = 0;
  ch_ptr = &val_str[14];
  switch (type)
  {
    default:
    case CHAR:
    case UCHAR:
    case INT:
    case UINT:
    case LONG:
      if (val < 0)
      {
        min = 1;
        val = -val;
      }
      else
        min = 0;
      do
      {
        ch_ptr--;
        *ch_ptr = (val % 10) + '0';
        nr_digits++;
        val /= 10;
        if (point)
        {
          point--;
          if (point == 0)
          {
            ch_ptr--;
            *ch_ptr = '.';
            if (val == 0)
            {
              ch_ptr--;
              *ch_ptr = '0';
              nr_digits++;
            }
          }
        }
      }
      while (val || point);
      if (min)
      {
        ch_ptr--;
        *ch_ptr = '-';
        nr_digits++;
      }
      break;
    case TIME_CHAR:
    case TIME_INT:
      ch_ptr--;
      *ch_ptr = (val % 10) + '0';
      ch_ptr--;
      *ch_ptr = (val / 10) + '0';
      nr_digits = (val / 10) ? 2 : 1;
      break;
    case HEX_CHAR:
      do
      {
        ch_ptr--;
        *ch_ptr = Hex_To_Asc(val & 0x0F);
        nr_digits++;
        val >>= 4;
      }
      while (val || (nr_digits < 2));
      break;
    case HEX_INT:
      do
      {
        ch_ptr--;
        *ch_ptr = Hex_To_Asc(val & 0x000F);
        nr_digits++;
        val >>= 4;
      }
      while (val || (nr_digits < 4));
      break;
    case HEX_LONG:
      do
      {
        ch_ptr--;
        *ch_ptr = Hex_To_Asc(val & 0x0000000F);
        nr_digits++;
        val >>= 4;
      }
      while (val || (nr_digits < 8));
      break;
  }
  if (change_value)
    screen_ptr->nr_digits = nr_digits;
  while (*ch_ptr)
  {
    *string = *ch_ptr;
    string++;
    ch_ptr++;
  }  
  *string = 0;
}

#define STRING_SD 0
#define STRING_PC 1

#ifdef ALARM_TEKST_NAAR_SMARTLINK
void Alarm_String_Type_Correction(unsigned char type, unsigned char font_type, char *string, unsigned char *prev_y, unsigned char new_y)
{
int length;
  if (type == STRING_PC) // JP 25-03-10
  {
    font_type >>= 4;
    font_type |= 0x30;
    if (string[0] != 0)
    {
      if (*prev_y != new_y)
      {
        strcat(string, "\n");
        length = strlen(string);
        string[length] = font_type;
        string[length+1] = 0;
      }  
      else
        strcat(string, " ");
    }
    else
    {
      length = strlen(string);
      string[length] = font_type;
      string[length+1] = 0;
    }  
    *prev_y = new_y;
  }
  else
    strcat(string, " ");
}    
#endif // ALARM_TEKST_NAAR_SMARTLINK


void Alarm_Create_String(char *string, s_alarm_disp *alarm, unsigned char type)
{
unsigned char y = 0;
TAlarm Alarm;
static void **lcd;
s_alarm_disp actueel = alarm_actueel;    // alarm voor scherm bewaren

  alarm_actueel = *alarm;
  GetAlarm(alarm_actueel.code, &Alarm);
  string[0] = '\0';
  if (type == STRING_SD) // JP 25-03-10
    sprintf(string,"%i", alarm_actueel.code);
  lcd = Alarm.Lcd;
  while (*lcd)
  {
    if      (((*(long *)*lcd) == (unsigned long)Disp_Draw_Tekst_L) ||
             ((*(long *)*lcd) == (unsigned long)Disp_Draw_Tekst_R))
    {
      #ifdef ALARM_TEKST_NAAR_SMARTLINK
      Alarm_String_Type_Correction(type, ((s_tekst_50 *)((s_disp_tekst *)*lcd)->tekst)->font_type, string, &y, ((s_disp_tekst *)*lcd)->y);
      #else // ALARM_TEKST_NAAR_SMARTLINK
      strcat(string, " ");
      #endif // ALARM_TEKST_NAAR_SMARTLINK
      _hstrcat(string, (char *)((s_tekst_50 *)((s_disp_tekst *)*lcd)->tekst)->string);
    }  
    else if (((*(long *)*lcd) == (unsigned long)Disp_Draw_Tekst_L_Option_On) ||
             ((*(long *)*lcd) == (unsigned long)Disp_Draw_Tekst_R_Option_On))
    {
      if (Return_Value(((s_disp_tekst_option_on *)*lcd)->option_type, ((s_disp_tekst_option_on *)*lcd)->option))
      {
        #ifdef ALARM_TEKST_NAAR_SMARTLINK
        Alarm_String_Type_Correction(type, ((s_tekst_50 *)((s_disp_tekst_option_on *)*lcd)->tekst)->font_type, string, &y, ((s_disp_tekst_option_on *)*lcd)->y);
        #else // ALARM_TEKST_NAAR_SMARTLINK
        strcat(string, " ");
        #endif // ALARM_TEKST_NAAR_SMARTLINK
        _hstrcat(string, (char *)((s_tekst_50 *)((s_disp_tekst_option_on *)*lcd)->tekst)->string);
      }  
    }  
    else if ((*(long *)*lcd) == (unsigned long)Disp_Draw_Tekst_L_Option_Off) // JP niet getest
    {
      if (Return_Value(((s_disp_tekst_option_on *)*lcd)->option_type, ((s_disp_tekst_option_on *)*lcd)->option) == 0)
      {
        strcat(string, " ");
        _hstrcat(string, (char *)((s_tekst_50 *)((s_disp_tekst_option_on *)*lcd)->tekst)->string);
      }  
    }  
    else if (((*(long *)*lcd) == (unsigned long)Disp_Draw_Tekst_Add_L) ||
             ((*(long *)*lcd) == (unsigned long)Disp_Draw_Tekst_Add_R))
    {
      _hstrcat(string, (char *)((s_tekst_50 *)((s_disp_tekst_add *)*lcd)->tekst)->string);
    }  
    else if (((*(long *)*lcd) == (unsigned long)Disp_Draw_Tekst_Add_L_Option_On) ||
             ((*(long *)*lcd) == (unsigned long)Disp_Draw_Tekst_Add_R_Option_On))
    {
      if (Return_Value(((s_disp_tekst_add_option_on *)*lcd)->option_type, ((s_disp_tekst_add_option_on *)*lcd)->option))
      {
        _hstrcat(string, (char *)((s_tekst_50 *)((s_disp_tekst_add_option_on *)*lcd)->tekst)->string);
      }  
    }  
    else if (((*(long *)*lcd) == (unsigned long)Disp_Draw_Tekst_Array_L) ||
             ((*(long *)*lcd) == (unsigned long)Disp_Draw_Tekst_Array_R))
    {
      long index;
      
      index = Return_Value(((s_disp_tekst_array *)*lcd)->type,((s_disp_tekst_array *)*lcd)->index);
      if (index > ((s_disp_tekst_array *)*lcd)->max)
        index = ((s_disp_tekst_array *)*lcd)->max;
      #ifdef ALARM_TEKST_NAAR_SMARTLINK
      Alarm_String_Type_Correction(type, (((s_tekst_50 **)(((s_disp_tekst_array *)*lcd)->tekst_array))[index])->font_type, string, &y, ((s_disp_tekst_array *)*lcd)->y);
      #else // ALARM_TEKST_NAAR_SMARTLINK
      strcat(string, " ");
      #endif // ALARM_TEKST_NAAR_SMARTLINK
      _hstrcat(string, (char *)(((s_tekst_50 **)(((s_disp_tekst_array *)*lcd)->tekst_array))[index])->string);
    }  
    else if (((*(long *)*lcd) == (unsigned long)Disp_Draw_Tekst_Array_L_Option_On) ||
             ((*(long *)*lcd) == (unsigned long)Disp_Draw_Tekst_Array_R_Option_On))
    {
      if (Return_Value(((s_disp_tekst_array_option_on *)*lcd)->option_type, ((s_disp_tekst_array_option_on *)*lcd)->option))
      {
        long index;
      
        index = Return_Value(((s_disp_tekst_array_option_on *)*lcd)->type,((s_disp_tekst_array_option_on *)*lcd)->index);
        if (index > ((s_disp_tekst_array_option_on *)*lcd)->max)
          index = ((s_disp_tekst_array_option_on *)*lcd)->max;
        #ifdef ALARM_TEKST_NAAR_SMARTLINK
        Alarm_String_Type_Correction(type, (((s_tekst_50 **)(((s_disp_tekst_array_option_on *)*lcd)->tekst_array))[index])->font_type, string, &y, ((s_disp_tekst_array_option_on *)*lcd)->y);
        #else // ALARM_TEKST_NAAR_SMARTLINK
        strcat(string, " ");
        #endif // ALARM_TEKST_NAAR_SMARTLINK
        _hstrcat(string, (char *)(((s_tekst_50 **)(((s_disp_tekst_array_option_on *)*lcd)->tekst_array))[index])->string);
      }  
    }  
    else if ((*(long *)*lcd) == (unsigned long)Disp_Draw_Tekst_Array_R_Option_Off) // JP niet getest
    {
      if (Return_Value(((s_disp_tekst_array_option_on *)*lcd)->option_type, ((s_disp_tekst_array_option_on *)*lcd)->option) == 0)
      {
        long index;
      
        index = Return_Value(((s_disp_tekst_array_option_on *)*lcd)->type,((s_disp_tekst_array_option_on *)*lcd)->index);
        if (index > ((s_disp_tekst_array_option_on *)*lcd)->max)
          index = ((s_disp_tekst_array_option_on *)*lcd)->max;
        #ifdef ALARM_TEKST_NAAR_SMARTLINK
        Alarm_String_Type_Correction(type, (((s_tekst_50 **)(((s_disp_tekst_array_option_on *)*lcd)->tekst_array))[index])->font_type, string, &y, ((s_disp_tekst_array_option_on *)*lcd)->y);
        #else // ALARM_TEKST_NAAR_SMARTLINK
        strcat(string, " ");
        #endif // ALARM_TEKST_NAAR_SMARTLINK
        _hstrcat(string, (char *)(((s_tekst_50 **)(((s_disp_tekst_array_option_on *)*lcd)->tekst_array))[index])->string);
      }  
    }  


    else if (((*(long *)*lcd) == (unsigned long)Disp_Draw_Tekst_Array_Add_L) ||
             ((*(long *)*lcd) == (unsigned long)Disp_Draw_Tekst_Array_Add_R))
    {
      long index;
      
      index = Return_Value(((s_disp_tekst_array_add *)*lcd)->type,((s_disp_tekst_array_add *)*lcd)->index);
      if (index > ((s_disp_tekst_array_add *)*lcd)->max)
        index = ((s_disp_tekst_array_add *)*lcd)->max;
      _hstrcat(string, (char *)(((s_tekst_50 **)(((s_disp_tekst_array_add *)*lcd)->tekst_array))[index])->string);
    }  
    else if (((*(long *)*lcd) == (unsigned long)Disp_Draw_Tekst_Array_Add_L_Option_On) ||
             ((*(long *)*lcd) == (unsigned long)Disp_Draw_Tekst_Array_Add_R_Option_On))
    {
      if (Return_Value(((s_disp_tekst_array_add_option_on *)*lcd)->option_type, ((s_disp_tekst_array_add_option_on *)*lcd)->option))
      {
        long index;
      
        index = Return_Value(((s_disp_tekst_array_add_option_on *)*lcd)->type,((s_disp_tekst_array_add_option_on *)*lcd)->index);
        if (index > ((s_disp_tekst_array_add_option_on *)*lcd)->max)
          index = ((s_disp_tekst_array_add_option_on *)*lcd)->max;
        _hstrcat(string, (char *)(((s_tekst_50 **)(((s_disp_tekst_array_add_option_on *)*lcd)->tekst_array))[index])->string);
      }  
    }  
    else if ((*(long *)*lcd) == (unsigned long)Disp_Draw_Value)
    {
      char help[50];
      
      #ifdef ALARM_TEKST_NAAR_SMARTLINK
      Alarm_String_Type_Correction(type, ((s_disp_value *)*lcd)->size, string, &y, ((s_disp_value *)*lcd)->y);
      #else // ALARM_TEKST_NAAR_SMARTLINK
      strcat(string, " ");
      #endif // ALARM_TEKST_NAAR_SMARTLINK
      String_Draw_Value_Func(help,((s_disp_value *)*lcd)->point,((s_disp_value *)*lcd)->type,((s_disp_value *)*lcd)->value);
      strcat(string, help);
    }
    else if ((*(long *)*lcd) == (unsigned long)Disp_Draw_Value_2_Size_No_Point)
    {
      char help[50];
      
      #ifdef ALARM_TEKST_NAAR_SMARTLINK
      Alarm_String_Type_Correction(type, ((s_disp_value_2_size_no_point *)*lcd)->size_1, string, &y, ((s_disp_value_2_size_no_point *)*lcd)->y);
      #else // ALARM_TEKST_NAAR_SMARTLINK
      strcat(string, " ");
      #endif // ALARM_TEKST_NAAR_SMARTLINK
      String_Draw_Value_Func(help,((s_disp_value_2_size_no_point *)*lcd)->point,((s_disp_value_2_size_no_point *)*lcd)->type,((s_disp_value_2_size_no_point *)*lcd)->value);
      strcat(string, help);
    }
    else if ((*(long *)*lcd) == (unsigned long)Disp_Draw_Value_2_Size_Option_On)
    {
      if (Return_Value(((s_disp_value_2_size_option_on *)*lcd)->option_type, ((s_disp_value_2_size_option_on *)*lcd)->option))
      {
        char help[50];
      
        #ifdef ALARM_TEKST_NAAR_SMARTLINK
        Alarm_String_Type_Correction(type, ((s_disp_value_2_size_option_on *)*lcd)->size_1, string, &y, ((s_disp_value_2_size_option_on *)*lcd)->y);
        #else // ALARM_TEKST_NAAR_SMARTLINK
        strcat(string, " ");
        #endif // ALARM_TEKST_NAAR_SMARTLINK
        String_Draw_Value_Func(help,((s_disp_value_2_size_option_on *)*lcd)->point,((s_disp_value_2_size_option_on *)*lcd)->type,((s_disp_value_2_size_option_on *)*lcd)->value);
        strcat(string, help);
      }  
    }       
    else if ((*(long *)*lcd) == (unsigned long)Disp_Draw_Value_Add)
    {
      char help[50];
      
      String_Draw_Value_Func(help,((s_disp_value_add *)*lcd)->point,((s_disp_value_add *)*lcd)->type,((s_disp_value_add *)*lcd)->value);
      strcat(string, help);
    }
    else if ((*(long *)*lcd) == (unsigned long)Disp_Draw_Value_Option_On)
    {
      if (Return_Value(((s_disp_value_option_on *)*lcd)->option_type, ((s_disp_value_option_on *)*lcd)->option))
      {
        char help[50];
      
        #ifdef ALARM_TEKST_NAAR_SMARTLINK
        Alarm_String_Type_Correction(type, ((s_disp_value_option_on *)*lcd)->size, string, &y, ((s_disp_value_option_on *)*lcd)->y);
        #else // ALARM_TEKST_NAAR_SMARTLINK
        strcat(string, " ");
        #endif // ALARM_TEKST_NAAR_SMARTLINK
        String_Draw_Value_Func(help,((s_disp_value_option_on *)*lcd)->point,((s_disp_value_option_on *)*lcd)->type,((s_disp_value_option_on *)*lcd)->value);
        strcat(string, help);
      }  
    }
    else if ((*(long *)*lcd) == (unsigned long)Disp_Draw_Value_Option_Off)
    {
      if (!Return_Value(((s_disp_value_option_on *)*lcd)->option_type, ((s_disp_value_option_on *)*lcd)->option))
      {
        char help[50];
      
        #ifdef ALARM_TEKST_NAAR_SMARTLINK
        Alarm_String_Type_Correction(type, ((s_disp_value_option_on *)*lcd)->size, string, &y, ((s_disp_value_option_on *)*lcd)->y);
        #else // ALARM_TEKST_NAAR_SMARTLINK
        strcat(string, " ");
        #endif // ALARM_TEKST_NAAR_SMARTLINK
        String_Draw_Value_Func(help,((s_disp_value_option_on *)*lcd)->point,((s_disp_value_option_on *)*lcd)->type,((s_disp_value_option_on *)*lcd)->value);
        strcat(string, help);
      }  
    }
    else if ((*(long *)*lcd) == (unsigned long)Disp_Draw_Value_Add_Option_On)
    {
      if (Return_Value(((s_disp_value_add_option_on *)*lcd)->option_type, ((s_disp_value_add_option_on *)*lcd)->option))
      {
        char help[50];
      
        String_Draw_Value_Func(help,((s_disp_value_add_option_on *)*lcd)->point,((s_disp_value_add_option_on *)*lcd)->type,((s_disp_value_add_option_on *)*lcd)->value);
        strcat(string, " ");
        strcat(string, help);
      }  
    }
    //else if ((*(long *)*lcd) == (unsigned long)Disp_Draw_Value_Array) // JP 09-09-09 deze functie is niet getest
    //{
    //  char help[50];
    //  void **value;
    //  long index;
    //  
    //  index = Return_Value(((s_disp_value_array *)*lcd)->type,((s_disp_value_array *)*lcd)->index);
    //  if (index > ((s_disp_value_array *)*lcd)->max)
    //    index = ((s_disp_value_array *)*lcd)->max;
    //  value = ((s_disp_value_array *)*lcd)->value_array;
    //  String_Draw_Value_Func(help,((s_disp_value_array *)*lcd)->point,((s_disp_value_array *)*lcd)->type,value[index]);
    //  strcat(string, " ");
    //  strcat(string, help);
    //}
    else if ((*(long *)*lcd) == (unsigned long)Disp_Draw_Time)
    {
      if (*((s_disp_time *)*lcd)->time != 0)
      {
        char help[20];
        struct tm *tm_ptr;
        int val;
        
        tm_ptr = gmtime(((s_disp_time *)*lcd)->time);
        val = tm_ptr->tm_hour;
        help[0] = (val / 10) + '0';
        help[1] = (val % 10) + '0'; 
        help[2] = ':';
        val = tm_ptr->tm_min;
        help[3] = (val / 10) + '0';
        help[4] = (val % 10) + '0'; 
        val = tm_ptr->tm_sec;
        help[5] = ' ';
        help[6] = (val / 10) + '0';
        help[7] = (val % 10) + '0'; 
        help[8] = ' ';
        val = tm_ptr->tm_mday;
        help[9] = (val / 10) + '0';
        help[10] = (val % 10) + '0'; 
        help[11] = '-';
        val = tm_ptr->tm_mon+1;
        help[12] = (val / 10) + '0';
        help[13] = (val % 10) + '0'; 
        help[14] = '-';
        val = tm_ptr->tm_year+1900;
        help[15] = (val / 1000) + '0';
        val %= 1000;
        help[16] = (val / 100) + '0';
        val %= 100;
        help[17] = (val / 10) + '0';
        help[18] = (val % 10) + '0';
        help[19] = 0;
        #ifdef ALARM_TEKST_NAAR_SMARTLINK
        Alarm_String_Type_Correction(type, ((s_disp_time *)*lcd)->size, string, &y, ((s_disp_time *)*lcd)->y);
        #else // ALARM_TEKST_NAAR_SMARTLINK
        strcat(string, " ");
        #endif // ALARM_TEKST_NAAR_SMARTLINK
        strcat(string, help);
      }  
    }
    else if ((*(long *)*lcd) == (unsigned long)Disp_Draw_Urenteller)
    {
      unsigned long time;
      char loop;
      char cnt = 0;
      char help_reverse[15];
      char help[15];
      
      time = *((s_disp_time *)*lcd)->time;
      help_reverse[0] = (time % 10) + '0'; // seconden
      time /= 10;
      help_reverse[1] = (time % 6) + '0';
      time /= 6;
      help_reverse[2] = ' ';
      help_reverse[3] = (time % 10) + '0'; // seconden
      time /= 10;
      help_reverse[4] = (time % 6) + '0';
      time /= 6;
      help_reverse[5] = ':';
      cnt = 6;
      do 
      {
        help_reverse[cnt] = (time % 10) + '0';
        time /= 10;
        cnt++;
      }
      while (time != 0);
      loop = 0;
      while (cnt)
      {
        cnt--;
        help[loop] = help_reverse[cnt];
        loop++;
      }
      #ifdef ALARM_TEKST_NAAR_SMARTLINK
      Alarm_String_Type_Correction(type, ((s_disp_time *)*lcd)->size, string, &y, ((s_disp_time *)*lcd)->y);
      #else // ALARM_TEKST_NAAR_SMARTLINK
      strcat(string, " ");
      #endif // ALARM_TEKST_NAAR_SMARTLINK
      strcat(string, help);
    }
    //else if ((*(long *)*lcd) == (unsigned long)Disp_Draw_Day_Time)                    { } // niet geimplementeerd want wordt in alarm schermen niet gebruikt
    else if ((*(long *)*lcd) == (unsigned long)Disp_Draw_Bitmap)                      { }
    else if ((*(long *)*lcd) == (unsigned long)Disp_Draw_Bitmap_Option_On)            { }
    else if ((*(long *)*lcd) == (unsigned long)Disp_Draw_Bitmap_Array)                { }
    else if ((*(long *)*lcd) == (unsigned long)Disp_Draw_Bitmap_Array_Option_On)      { }
    else if ((*(long *)*lcd) == (unsigned long)Disp_Draw_Bitmap_Invert)               { }
    else if ((*(long *)*lcd) == (unsigned long)Disp_Draw_Bitmap_Option_On_Invert)     { }
    else if ((*(long *)*lcd) == (unsigned long)Disp_Draw_Bitmap_Option_On_Invert_Add) { }
    else if ((*(long *)*lcd) == (unsigned long)Disp_Draw_Black_Block)                 { }
    else if ((*(long *)*lcd) == (unsigned long)Disp_Draw_Black_Vierkant)              { }
    else if ((*(long *)*lcd) == (unsigned long)Disp_Draw_White_Block)                 { }
    else if ((*(long *)*lcd) == (unsigned long)Disp_Draw_White_Vierkant)              { }
    else if ((*(long *)*lcd) == (unsigned long)Disp_Invert_Block)                     { }
    else if ((*(long *)*lcd) == (unsigned long)Disp_Draw_Code)                        { }
    else if ((*(long *)*lcd) == (unsigned long)Disp_Draw_Component)                   { }
    else if ((*(long *)*lcd) == (unsigned long)Disp_Draw_Balk)                        { }
    else if ((*(long *)*lcd) == (unsigned long)Disp_Control_Func)                     { }
    else if ((*(long *)*lcd) == (unsigned long)Disp_Draw_Board_IO)                    { }
    else if ((*(long *)*lcd) == (unsigned long)Disp_Draw_Board_IO_Select)             { }
    else if ((*(long *)*lcd) == (unsigned long)Disp_Draw_Board_IO_Select_Option_On)   { }
    else if ((*(long *)*lcd) == (unsigned long)Disp_Draw_Board_IO_Select_Option_Off)  { }
    else if ((*(long *)*lcd) == (unsigned long)Disp_Draw_Agri_Header)                 { }
    lcd++;
  }
  alarm_actueel = actueel; // terugzetten alarm voor scherm
}

#ifdef SD_CARD
void Alarm_Create_String_SD(char *string, s_alarm_disp *alarm)
{
  Alarm_Create_String(string, alarm, STRING_SD);
}
#endif // SD_CARD

#ifdef ALARM_TEKST_NAAR_SMARTLINK
void Alarm_Create_String_PC(char *string, s_alarm_disp *alarm)
{
  Alarm_Create_String(string, alarm, STRING_PC);
}
#endif // ALARM_TEKST_NAAR_SMARTLINK
