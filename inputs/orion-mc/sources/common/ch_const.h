// CH_CONST.H

#ifndef _CH_CONST_H
#define _CH_CONST_H

#include "ct_data.h"
#include "ct_disp.h"

//#define REFRESH_TIME (PULSES_PER_SECOND * 10)
#define REFRESH_TIME_125MS_BASE (PULSES_PER_SECOND * 10)
#define REFRESH_TIME_100MS_BASE (10 * 10)

#define ANA_IN 0
#define DIG_IN 1
#define ANA_OUT	2
#define DIG_OUT	3
#define ANA_HIGH_IN 3
#define MOTOR_CONTROL 4
//-----------------------------------
// ALS INGANG WORDT TOEGEVOEGD DAN BIJ ELK TYPE IO BORD Inc_Ana_In en Dec_Ana_In aanpassen
#define ANA_IN_EMPTY                     0
#define ANA_IN_TEMP                      1
#define ANA_IN_PA                        2
#define ANA_IN_RV                        3
#define ANA_IN_CO2                       4
#define ANA_IN_WINDRICHTING              5
#define ANA_IN_WINDSNELHEID              6
#define ANA_IN_VOERWEGER                 7
#define ANA_IN_DIERWEEGSCHAAL_TOT_25KG   8
#define ANA_IN_DIERWEEGSCHAAL_BOVEN_25KG 9
#define ANA_IN_RAAM                     10
#define ANA_IN_DOEK                     11
#define ANA_IN_VENT                     12
#define ANA_IN_LAMEL                    13
#define ANA_IN_VERWARMING               14
#define ANA_IN_VORST                    15
#define ANA_IN_ALARM                    16
#define ANA_IN_KLEP                     17

#define ANA_IN_MAX 18

//-----------------------------------
#define DIG_IN_EMPTY       0
#define DIG_IN_WATER       1
#define DIG_IN_VOER_PULS   2
#define DIG_IN_VOERWEGER   3
#define DIG_IN_EI_PULS     4            
#define DIG_IN_EI          5
#define DIG_IN_VOER        6
#define DIG_IN_KLOK        7
#define DIG_IN_ALARM       8
#define DIG_IN_VORST       9
#define DIG_IN_KWH_PULS   10
#define DIG_IN_TOEREN     11
#define DIG_IN_RAAM       12
#define DIG_IN_DOEK       13
#define DIG_IN_VENT       14
#define DIG_IN_LAMEL      15
#define DIG_IN_VERWARMING 16
#define DIG_IN_KLEP       17
//#define DIG_IN_WINDSNELHEID 1
//#define DIG_IN_TELLER 2
//#define DIG_IN_TOESTAND 3

#define DIG_IN_MAX 18

//-----------------------------------
// ALS UITGANG WORDT TOEGEVOEGD DAN BIJ ELK TYPE IO BORD Inc_Ana_Out en Dec_Ana_Out aanpassen
#define ANA_OUT_EMPTY       0
#define ANA_OUT_VENT        1
#define ANA_OUT_KLEP        2
#define ANA_OUT_VERWARMING  3
#define ANA_OUT_LICHT       4
#define ANA_OUT_UNI_REG     5
#define ANA_OUT_MESTDROGING 6
#define ANA_OUT_EI          7
#define ANA_OUT_RAAM        8
#define ANA_OUT_DOEK        9
#define ANA_OUT_LAMEL      10

#define ANA_OUT_MAX 11

//-----------------------------------
#define DIG_OUT_EMPTY        0
#define DIG_OUT_VENT         1
#define DIG_OUT_KLEP         2
#define DIG_OUT_VERWARMING   3
#define DIG_OUT_KOELING      4
#define DIG_OUT_RV           5
#define DIG_OUT_KLOK         6
#define DIG_OUT_LICHT        7
#define DIG_OUT_UNI_REG      8
#define DIG_OUT_TUNNEL       9
#define DIG_OUT_WATER       10
#define DIG_OUT_VOER        11
#define DIG_OUT_VOER_PULS   12
#define DIG_OUT_VOERWEGER   13
#define DIG_OUT_MESTDROGING 14
#define DIG_OUT_EI          15
#define DIG_OUT_HOPPER      16
#define DIG_OUT_ALARM       17
#define DIG_OUT_SILO        18
#define DIG_OUT_LAMEL       19

#define DIG_OUT_MAX 20
//-----------------------------------
// ALS INGANG WORDT TOEGEVOEGD DAN BIJ ELK TYPE IO BORD Inc_Ana_In en Dec_Ana_In aanpassen
#define ANA_HIGH_IN_EMPTY 0
#define ANA_HIGH_IN_SILO_FWS 1
#define ANA_HIGH_IN_SILO 2
//#define ANA_HIGH_IN_DIERWEGING 2

#define ANA_HIGH_IN_MAX 3
//-----------------------------------
#define MOTOR_CONTROL_EMPTY 0
#define MOTOR_CONTROL_RAAM  1
#define MOTOR_CONTROL_DOEK  2

#define MOTOR_CONTROL_MAX 3
//-----------------------------------

extern s_option_analog_input const option_analog_input_empty;
extern s_option_analog_input const option_analog_input_CO2;
extern s_option_analog_input const option_analog_input_Pa;
extern s_option_analog_input const option_analog_input_RV;
extern s_option_analog_input const option_analog_input_windsnelheid;
extern s_option_analog_input const option_analog_input_temp_celsius;
extern s_option_analog_input const option_analog_input_temp_fahrenheid;
extern s_option_analog_input const option_analog_input_windrichting;
extern s_option_analog_input const option_analog_input_voerweger;
extern s_option_analog_input const option_analog_input_voerweger_V;
extern s_option_analog_input const option_analog_input_dierweegschaal_tot_25kg;
extern s_option_analog_input const option_analog_input_dierweegschaal_tot_25kg_V;
extern s_option_analog_input const option_analog_input_dierweegschaal_boven_25kg;
extern s_option_analog_input const option_analog_input_dierweegschaal_boven_25kg_V;
extern s_option_analog_input const option_analog_input_raam;
extern s_option_analog_input const option_analog_input_doek;
extern s_option_analog_input const option_analog_input_vent;
extern s_option_analog_input const option_analog_input_klep;
extern s_option_analog_input const option_analog_input_lamel;
extern s_option_analog_input const option_analog_input_verwarm;
extern s_option_analog_input const option_analog_input_vorst;
extern s_option_analog_input const option_analog_input_alarm;
extern s_option_analog_input const option_analog_input_V;
extern s_option_analog_input const option_analog_input_mA;

extern s_option_digital_input const option_digital_input_empty;
extern s_option_digital_input const option_digital_input_water;
extern s_option_digital_input const option_digital_input_voer_puls;
extern s_option_digital_input const option_digital_input_voerweger;
extern s_option_digital_input const option_digital_input_ei_puls;
extern s_option_digital_input const option_digital_input_ei;
extern s_option_digital_input const option_digital_input_voer;
extern s_option_digital_input const option_digital_input_klok;
extern s_option_digital_input const option_digital_input_alarm;
extern s_option_digital_input const option_digital_input_vorst;
extern s_option_digital_input const option_digital_input_kWh_puls;
extern s_option_digital_input const option_digital_input_toeren;
extern s_option_digital_input const option_digital_input_raam;
extern s_option_digital_input const option_digital_input_doek;
extern s_option_digital_input const option_digital_input_vent;
extern s_option_digital_input const option_digital_input_klep;
extern s_option_digital_input const option_digital_input_lamel;
extern s_option_digital_input const option_digital_input_verwarm;

extern s_option_counter_input const option_counter_input_empty;
extern s_option_counter_input const option_counter_input_ei_puls;

extern s_option_analog_output const option_analog_output_empty;
extern s_option_analog_output const option_analog_output_vent;
extern s_option_analog_output const option_analog_output_klep;
extern s_option_analog_output const option_analog_output_verwarming;
extern s_option_analog_output const option_analog_output_licht;
extern s_option_analog_output const option_analog_output_uni_regeling;
extern s_option_analog_output const option_analog_output_raam;
extern s_option_analog_output const option_analog_output_doek;
extern s_option_analog_output const option_analog_output_lamel;

extern s_option_analog_high_input const option_analog_high_input_empty;
extern s_option_analog_high_input const option_analog_high_input_silo_FWS;
extern s_option_analog_high_input const option_analog_high_input_silo_FWS_mV;
extern s_option_analog_high_input const option_analog_high_input_silo;
extern s_option_analog_high_input const option_analog_high_input_silo_mV;
//extern s_option_analog_high_input const option_analog_high_input_silo_binair;
//extern s_option_analog_high_input const option_analog_high_input_dierweging;

extern s_option_motor_control const option_motor_control_empty;
extern s_option_motor_control const option_motor_control_raam;
extern s_option_motor_control const option_motor_control_doek;

extern s_option_RS485_bus const option_RS485_bus_empty;
extern s_option_RS485_bus const option_RS485_bus_enabled;

extern s_board_IO_on_off const IO_empty;

extern s_key_value const dummy_value;
extern void * const lcd_dummy_disp[];

extern unsigned char const option_on;
extern unsigned char const option_off;
extern unsigned char const option_index_0;
extern unsigned char const option_index_1;
extern unsigned char const option_index_2;
extern unsigned char const option_index_3;
extern unsigned char const option_index_4;
extern unsigned char const option_index_5;
extern unsigned char const option_index_6;
extern unsigned char const option_index_7;
extern unsigned char const option_index_8;
extern unsigned char const option_index_9;
extern unsigned char const option_index_10;
extern unsigned char const option_index_11;
extern unsigned char const option_index_12;
extern unsigned char const option_index_13;
extern unsigned char const option_index_14;
extern unsigned char const option_index_15;
extern unsigned char const option_index_16;
extern unsigned char const option_index_17;
extern unsigned char const option_index_18;
extern unsigned char const option_index_19;
extern unsigned char const option_index_20;
extern unsigned char const option_index_21;
extern unsigned char const option_index_22;
extern unsigned char const option_index_23;
extern unsigned char const option_index_24;
extern unsigned char const option_index_25;
extern unsigned char const option_index_26;
extern unsigned char const option_index_27;
extern unsigned char const option_index_28;
extern unsigned char const option_index_29;
extern unsigned char const option_index_30;
extern unsigned char const option_index_31;

extern unsigned char const first_char;
extern unsigned char const last_char;

extern char const char_m50;
extern char const char_m10;
extern char const char_m2;
extern char const char_10;
extern char const char_50;

extern unsigned char const uchar_0;
extern unsigned char const uchar_1;
extern unsigned char const uchar_2;
extern unsigned char const uchar_3;
extern unsigned char const uchar_4;
extern unsigned char const uchar_5;
extern unsigned char const uchar_6;
extern unsigned char const uchar_7;
extern unsigned char const uchar_8;
extern unsigned char const uchar_9;
extern unsigned char const uchar_10;
extern unsigned char const uchar_11;
extern unsigned char const uchar_12;
extern unsigned char const uchar_13;
extern unsigned char const uchar_14;
extern unsigned char const uchar_15;
extern unsigned char const uchar_16;
extern unsigned char const uchar_20;
extern unsigned char const uchar_23;
extern unsigned char const uchar_24;
extern unsigned char const uchar_32;
extern unsigned char const uchar_50;
extern unsigned char const uchar_59;
extern unsigned char const uchar_60;
extern unsigned char const uchar_99;
extern unsigned char const uchar_100;
extern unsigned char const uchar_120;
extern unsigned char const uchar_128;
extern unsigned char const uchar_200;
extern unsigned char const uchar_240;
extern unsigned char const uchar_247;
extern unsigned char const uchar_250;
extern unsigned char const uchar_255;
extern unsigned int const uint_0;
extern unsigned int const uint_1;
extern unsigned int const uint_2;
extern unsigned int const uint_10;
extern unsigned int const uint_999;
extern unsigned int const uint_9999;
extern unsigned int const uint_xFFFF;
extern int const int_m1;
extern int const int_0;
extern int const int_1;
extern int const int_2;
extern int const int_3;
extern int const int_4;
extern int const int_5;
extern int const int_6;
extern int const int_7;
extern int const int_8;
extern int const int_9;
extern int const int_10;
extern int const int_12;
extern int const int_20;
extern int const int_23;
extern int const int_31;
extern int const int_50;
extern int const int_59;
extern int const int_60;
extern int const int_63;
extern int const int_64;
extern int const int_96;
extern int const int_99;
extern int const int_100;
extern int const int_m100;
extern int const int_120;
extern int const int_m120;
extern int const int_126;
extern int const int_127;
extern int const int_200;
extern int const int_300;
extern int const int_m99;
extern int const int_359;
extern int const int_384;
extern int const int_500;
extern int const int_750;
extern int const int_900;
extern int const int_950;
extern int const int_980;
extern int const int_999;
extern int const int_m999;
extern int const int_1000;
extern int const int_1100;
extern int const int_1152;
extern int const int_1250;
extern int const int_1970;
extern int const int_2099;
extern int const int_2500;
extern int const int_3500;
extern int const int_5000;
extern int const int_6000;
extern int const int_9999;
extern int const int_m9999;
extern int const int_25000;
extern int const int_32000;
extern long const long_0;
extern long const long_10;
extern long const long_80000;
extern long const long_99999;
extern long const long_m999999;
extern long const long_999999;
extern long const long_1000000;
extern long const long_99999999;
extern long const long_999999999;

extern s_time const time_00_00;
extern s_time const time_24_00;

extern s_alarm_disp const alarm_geen;
extern unsigned char log_off;

void Dummy_Func(void);

#endif
