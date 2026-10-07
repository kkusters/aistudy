// C__CONST.C

#include "ch_define.h"
#include "ch_alarm.h"
#include "ch_const.h"

// ****************************************************************************
s_option_analog_input const option_analog_input_empty                       = { ANA_INPUT_GEEN,          0,     0,      0, 0,    0,    0, 0, 0,  0,            0, ANA_IN_EMPTY                     };
s_option_analog_input const option_analog_input_CO2                         = { ANA_INPUT_0_5V_NO_LIMIT, 0xCCD, 0x3FFF, 0, 5000, 0x0C, 0, 0, 1,  REFRESH_TIME_100MS_BASE, 0, ANA_IN_CO2                       };
s_option_analog_input const option_analog_input_Pa                          = { ANA_INPUT_0_5V,          0,     0x3FFF, 0, 100,  0x0C, 0, 0, 1,  REFRESH_TIME_100MS_BASE, 0, ANA_IN_PA                        };
s_option_analog_input const option_analog_input_RV                          = { ANA_INPUT_0_5V,          0,     0x3FFF, 0, 100,  0x0C, 0, 0, 1,  REFRESH_TIME_100MS_BASE, 0, ANA_IN_RV                        };
s_option_analog_input const option_analog_input_temp_celsius                = { ANA_INPUT_CELSIUS,       0,     0,      0, 0,    0x0C, 0, 0, 1,  REFRESH_TIME_100MS_BASE, 0, ANA_IN_TEMP                      };
s_option_analog_input const option_analog_input_temp_fahrenheid             = { ANA_INPUT_FAHRENHEID,    0,     0,      0, 0,    0x0C, 0, 0, 1,  REFRESH_TIME_100MS_BASE, 0, ANA_IN_TEMP                      };
s_option_analog_input const option_analog_input_windrichting                = { ANA_INPUT_0_5V,          0,     0x3FFF, 0, 360,  0x0C, 0, 0, 1,  REFRESH_TIME_100MS_BASE, 0, ANA_IN_WINDRICHTING              };
s_option_analog_input const option_analog_input_windsnelheid                = { ANA_INPUT_0_5V,          0,     0x3FFF, 0, 300,  0x0C, 0, 0, 1,  REFRESH_TIME_100MS_BASE, 0, ANA_IN_WINDSNELHEID              };
s_option_analog_input const option_analog_input_voerweger                   = { ANA_INPUT_0_5V_NO_LIMIT, 0x2F2, 0x3D2E, 0, 4000, 0x0C, 0, 0, 10, REFRESH_TIME_100MS_BASE, 0, ANA_IN_VOERWEGER                 };
s_option_analog_input const option_analog_input_voerweger_V                 = { ANA_INPUT_0_5V_NO_LIMIT, 0,     0x3FFF, 0, 500,  0x0C, 0, 0, 1,  REFRESH_TIME_100MS_BASE, 0, ANA_IN_VOERWEGER                 };
s_option_analog_input const option_analog_input_dierweegschaal_tot_25kg     = { ANA_INPUT_0_5V_NO_LIMIT, 0x2F2, 0x3D2E, 0, 5000, 0x0C, 0, 0, 1,  REFRESH_TIME_100MS_BASE, 0, ANA_IN_DIERWEEGSCHAAL_TOT_25KG   };
s_option_analog_input const option_analog_input_dierweegschaal_tot_25kg_V   = { ANA_INPUT_0_5V_NO_LIMIT, 0,     0x3FFF, 0, 500,  0x0C, 0, 0, 1,  REFRESH_TIME_100MS_BASE, 0, ANA_IN_DIERWEEGSCHAAL_TOT_25KG   };
s_option_analog_input const option_analog_input_dierweegschaal_boven_25kg   = { ANA_INPUT_0_5V_NO_LIMIT, 0x2F2, 0x3D2E, 0, 5000, 0x0C, 0, 0, 1,  REFRESH_TIME_100MS_BASE, 0, ANA_IN_DIERWEEGSCHAAL_BOVEN_25KG };
s_option_analog_input const option_analog_input_dierweegschaal_boven_25kg_V = { ANA_INPUT_0_5V_NO_LIMIT, 0,     0x3FFF, 0, 500,  0x0C, 0, 0, 1,  REFRESH_TIME_100MS_BASE, 0, ANA_IN_DIERWEEGSCHAAL_BOVEN_25KG };
s_option_analog_input const option_analog_input_raam                        = { ANA_INPUT_0_5V,          0,     0x3FFF, 0, 1000, 0x0C, 0, 0, 1,  REFRESH_TIME_100MS_BASE, 0, ANA_IN_RAAM                      };
s_option_analog_input const option_analog_input_doek                        = { ANA_INPUT_0_5V,          0,     0x3FFF, 0, 1000, 0x0C, 0, 0, 1,  REFRESH_TIME_100MS_BASE, 0, ANA_IN_DOEK                      };
s_option_analog_input const option_analog_input_vent                        = { ANA_INPUT_0_5V,          0,     0x3FFF, 0, 100,  0x0C, 0, 0, 1,  REFRESH_TIME_100MS_BASE, 0, ANA_IN_VENT                      };
s_option_analog_input const option_analog_input_klep                        = { ANA_INPUT_0_5V,          0,     0x3FFF, 0, 100,  0x0C, 0, 0, 1,  REFRESH_TIME_100MS_BASE, 0, ANA_IN_KLEP                      };
s_option_analog_input const option_analog_input_lamel                       = { ANA_INPUT_0_5V,          0,     0x3FFF, 0, 100,  0x0C, 0, 0, 1,  REFRESH_TIME_100MS_BASE, 0, ANA_IN_LAMEL                     };
s_option_analog_input const option_analog_input_verwarm                     = { ANA_INPUT_0_5V,          0,     0x3FFF, 0, 100,  0x0C, 0, 0, 1,  REFRESH_TIME_100MS_BASE, 0, ANA_IN_VERWARMING                };
s_option_analog_input const option_analog_input_vorst                       = { ANA_INPUT_0_5V,          0,     0x3FFF, 0, 2,    0x0C, 0, 0, 1,  REFRESH_TIME_100MS_BASE, 0, ANA_IN_VORST                     };
s_option_analog_input const option_analog_input_alarm                       = { ANA_INPUT_0_5V,          0,     0x3FFF, 0, 2,    0x0C, 0, 0, 1,  REFRESH_TIME_100MS_BASE, 0, ANA_IN_ALARM                     };
s_option_analog_input const option_analog_input_V                           = { ANA_INPUT_0_5V_NO_LIMIT, 0,     0x3FFF, 0, 500,  0x0C, 0, 0, 1,  REFRESH_TIME_100MS_BASE, 0, ANA_IN_RAAM                      };
s_option_analog_input const option_analog_input_mA                          = { ANA_INPUT_0_5V_NO_LIMIT | 0x0400, 0, 0x3FFF, 0, 200, 0x0C, 0, 0, 1, REFRESH_TIME_100MS_BASE, 0, ANA_IN_RAAM                   };
// ****************************************************************************
s_option_digital_input const option_digital_input_empty     = { 0x00F0,0,0,0xC,0,0,1,REFRESH_TIME_100MS_BASE,0,DIG_IN_EMPTY };
s_option_digital_input const option_digital_input_water     = { 0x02F2,0,1,0xC,0,0,1,REFRESH_TIME_100MS_BASE,0,DIG_IN_WATER };
s_option_digital_input const option_digital_input_voer_puls = { 0x02F2,0,1,0xC,0,0,1,REFRESH_TIME_100MS_BASE,0,DIG_IN_VOER_PULS };
s_option_digital_input const option_digital_input_voerweger = { 0x00F1,0,0,0xC,0,0,1,REFRESH_TIME_100MS_BASE,0,DIG_IN_VOERWEGER };
s_option_digital_input const option_digital_input_ei_puls   = { 0x02F2,0,1,0xC,0,0,1,REFRESH_TIME_100MS_BASE,0,DIG_IN_EI_PULS };
s_option_digital_input const option_digital_input_ei        = { 0x00F1,0,0,0xC,0,0,1,REFRESH_TIME_100MS_BASE,0,DIG_IN_EI };
s_option_digital_input const option_digital_input_voer      = { 0x00F1,0,0,0xC,0,0,1,REFRESH_TIME_100MS_BASE,0,DIG_IN_VOER }; // sensor voervraag
s_option_digital_input const option_digital_input_klok      = { 0x00F1,0,0,0xC,0,0,1,REFRESH_TIME_100MS_BASE,0,DIG_IN_KLOK }; // bewaking nest open
s_option_digital_input const option_digital_input_alarm     = { 0x00F1,0,0,0xC,0,0,1,REFRESH_TIME_100MS_BASE,0,DIG_IN_ALARM };
s_option_digital_input const option_digital_input_vorst     = { 0x00F1,0,0,0xC,0,0,1,REFRESH_TIME_100MS_BASE,0,DIG_IN_VORST };
s_option_digital_input const option_digital_input_kWh_puls  = { 0x02F2,0,1,0xC,0,0,1,REFRESH_TIME_100MS_BASE,0,DIG_IN_KWH_PULS };
s_option_digital_input const option_digital_input_toeren    = { 0x0003,0,1,0xC,0,0,1,REFRESH_TIME_100MS_BASE,0,DIG_IN_TOEREN };
s_option_digital_input const option_digital_input_raam      = { 0x00F1,0,0,0xC,0,0,1,REFRESH_TIME_100MS_BASE,0,DIG_IN_RAAM  };
s_option_digital_input const option_digital_input_doek      = { 0x00F1,0,0,0xC,0,0,1,REFRESH_TIME_100MS_BASE,0,DIG_IN_DOEK  };
s_option_digital_input const option_digital_input_vent      = { 0x00F1,0,0,0xC,0,0,1,REFRESH_TIME_100MS_BASE,0,DIG_IN_VENT  };
s_option_digital_input const option_digital_input_klep      = { 0x00F1,0,0,0xC,0,0,1,REFRESH_TIME_100MS_BASE,0,DIG_IN_KLEP  };
s_option_digital_input const option_digital_input_lamel     = { 0x00F1,0,0,0xC,0,0,1,REFRESH_TIME_100MS_BASE,0,DIG_IN_LAMEL };
s_option_digital_input const option_digital_input_verwarm   = { 0x00F1,0,0,0xC,0,0,1,REFRESH_TIME_100MS_BASE,0,DIG_IN_VERWARMING };
// ****************************************************************************
s_option_counter_input const option_counter_input_empty   = { 0x0000,1,0xC,1,REFRESH_TIME_100MS_BASE,0,DIG_IN_EMPTY };
s_option_counter_input const option_counter_input_ei_puls = { 0x0002,1,0xC,5,REFRESH_TIME_100MS_BASE,0,DIG_IN_EI_PULS };
// ****************************************************************************
s_option_analog_output const option_analog_output_empty        = { 0,0,   0,0,0x3FFF,0,ANA_OUT_EMPTY };
s_option_analog_output const option_analog_output_vent         = { 1,0, 100,0,0x7FFF,0,ANA_OUT_VENT };
s_option_analog_output const option_analog_output_klep         = { 1,0, 100,0,0x7FFF,0,ANA_OUT_KLEP };
s_option_analog_output const option_analog_output_verwarming   = { 1,0, 100,0,0x7FFF,0,ANA_OUT_VERWARMING };
s_option_analog_output const option_analog_output_licht        = { 1,0, 100,0,0x7FFF,0,ANA_OUT_LICHT };
s_option_analog_output const option_analog_output_uni_regeling = { 1,0, 100,0,0x7FFF,0,ANA_OUT_UNI_REG };
s_option_analog_output const option_analog_output_mestdroging  = { 1,0, 100,0,0x7FFF,0,ANA_OUT_MESTDROGING };
s_option_analog_output const option_analog_output_raam         = { 1,0,1000,0,0x3FFF,0,ANA_OUT_RAAM  };
s_option_analog_output const option_analog_output_doek         = { 1,0,1000,0,0x3FFF,0,ANA_OUT_DOEK  };
s_option_analog_output const option_analog_output_lamel        = { 1,0, 100,0,0x3FFF,0,ANA_OUT_LAMEL };
// ****************************************************************************
s_option_analog_high_input const option_analog_high_input_empty       = { ANA_HIGH_INPUT_GEEN,0,0,0,0,0,0,0,0,0,0,ANA_HIGH_IN_EMPTY,0 };
s_option_analog_high_input const option_analog_high_input_silo_FWS    = { ANA_HIGH_INPUT_WEGING | ANA_HIGH_INPUT_M10_10MV,0x00000000,0x5FFFE,0,20000,0,150000,0x0C,1,/*10*/REFRESH_TIME_100MS_BASE,0,ANA_HIGH_IN_SILO_FWS,30000 };
s_option_analog_high_input const option_analog_high_input_silo_FWS_mV = { ANA_HIGH_INPUT_WEGING | ANA_HIGH_INPUT_M10_10MV,0x00000000,0x5FFFE,0,100000,0,150000,0x0C,1,/*10*/REFRESH_TIME_100MS_BASE,0,ANA_HIGH_IN_SILO_FWS,30000 };
s_option_analog_high_input const option_analog_high_input_silo        = { ANA_HIGH_INPUT_WEGING | ANA_HIGH_INPUT_M20_20MV,0x00000000,0x5FFFE,0,20000,0,150000,0x0C,1,/*10*/REFRESH_TIME_100MS_BASE,0,ANA_HIGH_IN_SILO,30000 };
s_option_analog_high_input const option_analog_high_input_silo_mV     = { ANA_HIGH_INPUT_WEGING | ANA_HIGH_INPUT_M20_20MV,0x00000000,0x5FFFE,0,100000,0,150000,0x0C,1,/*10*/REFRESH_TIME_100MS_BASE,0,ANA_HIGH_IN_SILO,30000 };
//s_option_analog_high_input const option_analog_high_input_silo_binair = { ANA_HIGH_INPUT_BINAIR,0xFFFC0000,0x3FFFF,0,500,0,500,0x0C,1,REFRESH_TIME_100MS_BASE,0,ANA_HIGH_IN_SILO_FWS,0 };
//s_option_analog_high_input const option_analog_high_input_dierweging = { ANA_HIGH_INPUT_WEGING,0xCCD,0x3FFF,0,5000,0,500,0x0C,1,REFRESH_TIME_100MS_BASE,0,ANA_HIGH_IN_DIERWEGING,0 };
// ****************************************************************************
s_option_motor_control const option_motor_control_empty = { 0,0,1000,0,1000,1800,10,50,100,100,900,0,0,10,REFRESH_TIME_100MS_BASE,0,MOTOR_CONTROL_EMPTY };
s_option_motor_control const option_motor_control_raam  = { 1,0,1000,0,1000,1800,10,50,100,100,900,0,0, 5,REFRESH_TIME_100MS_BASE,0,MOTOR_CONTROL_RAAM  };
s_option_motor_control const option_motor_control_doek  = { 1,1000,0,1000,0,1800,10,50,100,900,100,0,0, 1,REFRESH_TIME_100MS_BASE,0,MOTOR_CONTROL_DOEK  };
// ****************************************************************************
s_option_RS485_bus const option_RS485_bus_empty   = { 0,br9600,0,0,0,0,1500,0,0 };
s_option_RS485_bus const option_RS485_bus_enabled = { 1,br9600,0,0,0,0,1500,0,0 };
// ****************************************************************************
s_board_IO_on_off const IO_empty = {0,0,0,0};
// ****************************************************************************

unsigned char const first_char = 0x20;
unsigned char const last_char = 0x6E;

char const char_m50 = -50;
char const char_m10 = -10;
char const char_m2 = -2;
char const char_10 = 10;
char const char_50 = 50;

unsigned char const uchar_0 = 0;
unsigned char const uchar_1 = 1;
unsigned char const uchar_2 = 2;
unsigned char const uchar_3 = 3;
unsigned char const uchar_4 = 4;
unsigned char const uchar_5 = 5;
unsigned char const uchar_6 = 6;
unsigned char const uchar_7 = 7;
unsigned char const uchar_8 = 8;
unsigned char const uchar_9 = 9;
unsigned char const uchar_10 = 10;
unsigned char const uchar_11 = 11;
unsigned char const uchar_12 = 12;
unsigned char const uchar_13 = 13;
unsigned char const uchar_14 = 14;
unsigned char const uchar_15 = 15;
unsigned char const uchar_16 = 16;
unsigned char const uchar_20 = 20;
unsigned char const uchar_23 = 23;
unsigned char const uchar_24 = 24;
unsigned char const uchar_32 = 32;
unsigned char const uchar_50 = 50;
unsigned char const uchar_59 = 59;
unsigned char const uchar_60 = 60;
unsigned char const uchar_99 = 99;
unsigned char const uchar_100 = 100;
unsigned char const uchar_120 = 120;
unsigned char const uchar_128 = 128;
unsigned char const uchar_200 = 200;
unsigned char const uchar_240 = 240;
unsigned char const uchar_247 = 247;
unsigned char const uchar_250 = 250;
unsigned char const uchar_255 = 255;
unsigned int const uint_0 = 0;
unsigned int const uint_1 = 1;
unsigned int const uint_2 = 2;
unsigned int const uint_10 = 10;
unsigned int const uint_999 = 999; 
unsigned int const uint_9999 = 9999;
unsigned int const uint_xFFFF = 0xFFFF;
int const int_m1 = -1;
int const int_0 = 0;
int const int_1 = 1;
int const int_2 = 2;
int const int_3 = 3;
int const int_4 = 4;
int const int_5 = 5;
int const int_6 = 6;
int const int_7 = 7;
int const int_8 = 8;
int const int_9 = 9;
int const int_ = 3;
int const int_10 = 10;
int const int_12 = 12;
int const int_20 = 20;
int const int_23 = 23;
int const int_31 = 31;
int const int_50 = 50;
int const int_59 = 59;
int const int_60 = 60;
int const int_63 = 63;
int const int_64 = 64;
int const int_96 = 96;
int const int_99 = 99;
int const int_100 = 100;
int const int_m100 = -100;
int const int_120 = 120;
int const int_m120 = -120;
int const int_126 = 126;
int const int_127 = 127;
int const int_200 = 200;
int const int_300 = 300;
int const int_m99 = -99;
int const int_359 = 359;
int const int_384 = 384;
int const int_500 = 500;
int const int_750 = 750;
int const int_900 = 900;
int const int_950 = 950;
int const int_980 = 980;
int const int_999 = 999;
int const int_m999 = -999;
int const int_1000 = 1000;
int const int_1100 = 1100;
int const int_1152 = 1152;
int const int_1250 = 1250;
int const int_1970 = 1970;
int const int_2099 = 2099;
int const int_2500 = 2500;
int const int_3500 = 3500;
int const int_5000 = 5000;
int const int_6000 = 6000;
int const int_9999 = 9999;
int const int_m9999 = -9999;
int const int_25000 = 25000;
int const int_32000 = 32000;
long const long_0 = 0;
long const long_10 = 10;
long const long_80000 = 80000;
long const long_99999 = 99999;
long const long_m999999 = -999999;
long const long_999999 = 999999;
long const long_99999999 = 99999999;
long const long_999999999 = 999999999;

//*****************************************************************************
unsigned char far dummy_far;
unsigned char huge dummy_huge;
unsigned char shuge dummy_shuge;

s_alarm_disp const alarm_geen = { GEEN_AL,0,0,0,0,0,0,0,0,0 };

int dummy = 0;
s_key_value const dummy_value =
{
  INT,
  0,
  &dummy,
  &int_0,
  &int_0,
};
s_time const time_00_00 = {0,0};
s_time const time_24_00 = {24,0};

void * const lcd_dummy_disp[] = { 0 };

unsigned char const option_on = FN_CURVE_ON;
unsigned char const option_off = 0;
unsigned char const option_index_0 = 0;
unsigned char const option_index_1 = 1;
unsigned char const option_index_2 = 2;
unsigned char const option_index_3 = 3;
unsigned char const option_index_4 = 4;
unsigned char const option_index_5 = 5;
unsigned char const option_index_6 = 6;
unsigned char const option_index_7 = 7;
unsigned char const option_index_8 = 8;
unsigned char const option_index_9 = 9;
unsigned char const option_index_10 = 10;
unsigned char const option_index_11 = 11;
unsigned char const option_index_12 = 12;
unsigned char const option_index_13 = 13;
unsigned char const option_index_14 = 14;
unsigned char const option_index_15 = 15;
unsigned char const option_index_16 = 16;
unsigned char const option_index_17 = 17;
unsigned char const option_index_18 = 18;
unsigned char const option_index_19 = 19;
unsigned char const option_index_20 = 20;
unsigned char const option_index_21 = 21;
unsigned char const option_index_22 = 22;
unsigned char const option_index_23 = 23;
unsigned char const option_index_24 = 24;
unsigned char const option_index_25 = 25;
unsigned char const option_index_26 = 26;
unsigned char const option_index_27 = 27;
unsigned char const option_index_28 = 28;
unsigned char const option_index_29 = 29;
unsigned char const option_index_30 = 30;
unsigned char const option_index_31 = 31;
unsigned char log_off = 0;

void Dummy_Func(void) 
{
}
