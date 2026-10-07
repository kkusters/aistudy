// C__DISP_OPT_LUCHTING_2.C

#include <string.h>

#include "ch_define.h"

#include "ch_alg.h"
#include "ch_alarm.h"
#include "ch_can_backbone_appl.h"
#include "ch_const.h"
#include "ch_disp.h"
#include "ch_disp_func.h"
#include "ch_disp_opt_alg_2.h"
#include "ch_disp_opt_IO_Select.h"
#include "ch_disp_option_1.h"
#include "ch_disp_value.h" 
#include "ch_disp_password.h"
#include "ch_IO.h"
#include "ch_key.h"
#include "ch_kiersturing.h"
#include "ch_lcd_10.h"
#include "ch_lcd_14.h"
#include "ch_lcd_20.h"
#include "ch_lcd_bitmap.h"
#include "ch_lcd_hardware.h"
#include "ch_lcd_pixel.h"
#include "ch_main.h"
#include "ch_motor.h"
#include "ch_string.h" 
#include "ch_disp_opt_luchting_2.h"

#define MAX_LUCHTING 16

static void SetDisplayOptions(void);

static unsigned char DummyNotUsed(s_board_IO_on_off IO_new);
static unsigned char AnaInNotUsed(s_board_IO_on_off IO_new);
static unsigned char AnaOutNotUsed(s_board_IO_on_off IO_new);
static unsigned char DigOutNotUsed(s_board_IO_on_off IO_new);
static unsigned char MotorControlNotUsed(s_board_IO_on_off IO_new);

static void Arrow_Scroll_Luchting_Value(void);
static void Arrow_Luchting_Value(void);
static void Enter_Luchting_Value(void);

static void Disp_Adres_Func(void);
static void Arrow_Adres_Value(void);
static void Enter_Adres_Value(void);

static void Arrow_Position_Func(void);
static void Arrow_Position_Value(void);
static void Enter_Position_Value(void);
static void Number_Position_Value(void);

static void Arrow_IO_Motor_Func(void);

static void Disp_Copy_Motor_IO_1(void);
static void Disp_Copy_Motor_IO_2(void);
static void Disp_Copy_Motor_IO_3(void);
static void Disp_Copy_Motor_IO_4(void);
static void Disp_Copy_Motor_IO_5(void);
static void Disp_Copy_Motor_IO_6(void);
static void Disp_Copy_Motor_IO_7(void);
static void Disp_Copy_Motor_IO_8(void);
static void Disp_Copy_Motor_IO_9(void);
static void Disp_Copy_Motor_IO_10(void);
static void Disp_Copy_Motor_IO_11(void);
static void Disp_Copy_Motor_IO_12(void);
static void Disp_Copy_Motor_IO_13(void);
static void Disp_Copy_Motor_IO_14(void);
static void Disp_Copy_Motor_IO_15(void);
static void Disp_Copy_Motor_IO_16(void);

static void Arrow_End_Func(void);

//*****************************************************************************
// GLOBALS
//*****************************************************************************
static unsigned char const MaxAantalGroepen = MAX_LUCHTING;
static unsigned char const MaxMotor = MAX_MOTOR;
static unsigned char GroepEnabled[MAX_LUCHTING];
static unsigned char AantalGroepen;
static unsigned char GroepAdres;
static unsigned char GroepNummer;
static unsigned char TypeSturing;
static unsigned char SturingDigitaal;
static unsigned char SturingAnaloog;
static unsigned char TerugmeldingAnaloog;
static unsigned char DispGroepAdres;
static unsigned char HiSpeedDigitaal;
static unsigned char AlarmAnaloog;
static unsigned char FreqGestuurd;
static unsigned char FreqAnaloog;
static unsigned char SpeedLow;
static unsigned char SpeedHi;
static int PositionLowSpeed;
static int FrequentieVerstel;

static s_board_IO_on_off DigInOpen[MAX_LUCHTING];
static s_board_IO_on_off DigInClose[MAX_LUCHTING];
static s_board_IO_on_off AnaInPos[MAX_LUCHTING];
static s_board_IO_on_off AnaOutPos[MAX_LUCHTING];
static s_board_IO_on_off DigOutAlarm[MAX_LUCHTING];
static s_board_IO_on_off DigInHiSpeed[MAX_LUCHTING];
static s_board_IO_on_off MotorIO[16];

unsigned char CopyMotorIO = 0;
unsigned char GroupMotorIO = 0;

static int PositionManual;
static unsigned char Numeric;

static unsigned char DispMotorIO;
static unsigned char DispFrequency;

//***********************************************************************************************************
// SCHERM OPBOUW
//***********************************************************************************************************
//-----------------------------------------------------------------------------------------------------------
// Header
static s_disp_agri_header const disp_header = { Disp_Draw_Agri_Header, FN6, EMPTY, &tekst_inst.Opties_Luchting_10, HK_GEEN, 0, 3};

static void * const lcd_disp_header[] = { &disp_header, 0 };
//-----------------------------------------------------------------------------------------------------------
// Aantal luchtings groepen
static s_disp_tekst const disp_aantal_groepen_str = { Disp_Draw_Tekst_L, 37, 22, &tekst_inst.Aantal_groepen_14 };
static s_disp_value const disp_aantal_groepen_val = { Disp_Draw_Value,  207, 75, (SIZE_14 | RECHTS), UCHAR, 0, &AantalGroepen };

static void * const lcd_aantal_groepen_disp[] = { &disp_raamstand_small_ico, &disp_aantal_groepen_str, &disp_aantal_groepen_val, 0 };

static s_key_value const key_aantal_groepen_val = { UCHAR, 1, &AantalGroepen, &uchar_0, &MaxAantalGroepen };
//-----------------------------------------------------------------------------------------------------------
// Computersturing: Digitaal / Analoog / CANopen / BACnet
static s_disp_tekst       const disp_type_sturing_str = { Disp_Draw_Tekst_L,       37, 22, &tekst_inst.Type_sturing_14 };
static s_disp_tekst_array const disp_type_sturing_val = { Disp_Draw_Tekst_Array_L, 76, 75, &tekst_Digitaal_Analoog_CANopen_BACnet_14, UCHAR, &TypeSturing, 4 };

static void * const lcd_type_sturing_disp[] = { &disp_raamstand_small_ico, &disp_type_sturing_str, &disp_type_sturing_val, 0 };

static s_key_value const key_type_sturing_val = { UCHAR, 1, &TypeSturing, &uchar_0, &uchar_3 };
//-----------------------------------------------------------------------------------------------------------
// Adres in geval van CANopen of BACnet
static s_disp_tekst     const disp_adres_str  = { Disp_Draw_Tekst_L, 37, 22, &tekst_inst.Adres_14 };
static s_disp_value     const disp_adres_val  = { Disp_Draw_Value,  207, 75, (SIZE_14 | RECHTS), UCHAR, 0, &GroepAdres  };
static s_disp_value_add const disp_nummer_val = { Disp_Draw_Value_Add,       (SIZE_14 | LINKS ), UCHAR, 0, &GroepNummer };

static s_disp_func const disp_adres_func = { Disp_Control_Func, Disp_Adres_Func };

static void * const lcd_adres_disp[] = { &disp_raamstand_small_ico, &disp_adres_str, &disp_adres_val, 0 };

static s_key_value const key_adres_val = { UCHAR, 2, &GroepAdres, &uchar_0, &uchar_32 };
//-----------------------------------------------------------------------------------------------------------
// Digitale ingangen open/dicht
static s_disp_tekst const disp_open_str  = { Disp_Draw_Tekst_L, 47, 22, &tekst_inst.Open_14  };
static s_disp_tekst const disp_dicht_str = { Disp_Draw_Tekst_L, 47, 22, &tekst_inst.Dicht_14 };
static s_disp_board_IO_Selection const disp_dig_in_open_sel  = { Disp_Draw_Board_IO_Select, DigInOpen,  MAX_LUCHTING, &AantalGroepen, 0, DIGITAL_INPUT_ID, DIG_IN_RAAM, DigInNotUsed };
static s_disp_board_IO_Selection const disp_dig_in_dicht_sel = { Disp_Draw_Board_IO_Select, DigInClose, MAX_LUCHTING, &AantalGroepen, 0, DIGITAL_INPUT_ID, DIG_IN_RAAM, DigInNotUsed };

static void * const lcd_dig_in_open[]  = { &disp_raamstand_small_ico, &disp_open_str,  &disp_dig_in_open_sel,  0 };
static void * const lcd_dig_in_dicht[] = { &disp_raamstand_small_ico, &disp_dicht_str, &disp_dig_in_dicht_sel, 0 };
//-----------------------------------------------------------------------------------------------------------
// Analoge ingang positie
static s_disp_tekst const disp_position_str = { Disp_Draw_Tekst_L, 47, 22, &tekst_inst.Position_14 };
static s_disp_board_IO_Selection const disp_ana_in_pos_sel = { Disp_Draw_Board_IO_Select, AnaInPos, MAX_LUCHTING, &AantalGroepen, 0, ANALOG_INPUT_ID, ANA_IN_RAAM, AnaInNotUsed };

static void * const lcd_ana_in_position[] = { &disp_raamstand_small_ico, &disp_position_str, &disp_ana_in_pos_sel, 0 };
//-----------------------------------------------------------------------------
// Analoge uitgang terugmelding
static s_disp_tekst const disp_terugmelding_str = { Disp_Draw_Tekst_L, 47, 22, &tekst_inst.Terugmelding_14 };
static s_disp_board_IO_Selection const disp_ana_out_pos_sel = { Disp_Draw_Board_IO_Select, AnaOutPos, MAX_LUCHTING, &AantalGroepen, 0, ANALOG_OUTPUT_ID, ANA_OUT_RAAM, AnaOutNotUsed };

static void * const lcd_ana_out_terugmelding[] = { &disp_raamstand_small_ico, &disp_terugmelding_str, &disp_ana_out_pos_sel, 0 };
//-----------------------------------------------------------------------------
// Digitale uitgang alarm
static s_disp_bitmap const disp_alarm_ico = { Disp_Draw_Bitmap,   9, 10, &ico_alarm };
static s_disp_tekst  const disp_alarm_str = { Disp_Draw_Tekst_L, 47, 22, &tekst_inst.Alarm_Contact_14 };
static s_disp_board_IO_Selection const disp_dig_out_alarm_sel = { Disp_Draw_Board_IO_Select, DigOutAlarm, MAX_LUCHTING, &AantalGroepen, 0, DIGITAL_OUTPUT_ID, DIG_OUT_ALARM, DigOutNotUsed };

static void * const lcd_dig_out_alarm[] = { &disp_alarm_ico, &disp_alarm_str, &disp_dig_out_alarm_sel, 0 };
//-----------------------------------------------------------------------------
// Alarm analoge uitgang 50%
static s_disp_tekst const disp_alarm_analoog_str = { Disp_Draw_Tekst_L, 37, 22, &tekst_inst.Alarm_analoog_14 };
static s_disp_bitmap_array const disp_alarm_analoog_val = { Disp_Draw_Bitmap_Array, 190, 57, bmp_false_true, UCHAR, &AlarmAnaloog, 2 };

static void * const lcd_alarm_analoog_disp[] = { &disp_alarm_ico, &disp_alarm_analoog_str, &disp_alarm_analoog_val, 0 };

static s_key_value const key_alarm_analoog_val = { UCHAR, 1, &AlarmAnaloog, &uchar_0, &uchar_1 };
//-----------------------------------------------------------------------------
// Frequentie gestuurd
static s_disp_tekst const disp_frequentie_gestuurd_str = { Disp_Draw_Tekst_L, 37, 22, &tekst_inst.Frequentie_gestuurd_14 };
static s_disp_bitmap_array const disp_frequentie_gestuurd_val = { Disp_Draw_Bitmap_Array, 190, 57, bmp_false_true, UCHAR, &FreqGestuurd, 2 };

static void * const lcd_frequentie_gestuurd_disp[] = { &disp_raamstand_small_ico, &disp_frequentie_gestuurd_str, &disp_frequentie_gestuurd_val, 0 };

static s_key_value const key_frequentie_gestuurd_val = { UCHAR, 1, &FreqGestuurd, &uchar_0, &uchar_1 };
//-----------------------------------------------------------------------------
// Digitale ingang hoge snelheid
static s_disp_tekst const disp_hoge_snelheid_str = { Disp_Draw_Tekst_L, 47, 22, &tekst_inst.Hoge_snelheid_14 };
static s_disp_board_IO_Selection const disp_dig_in_hoge_snelheid_sel = { Disp_Draw_Board_IO_Select, DigInHiSpeed, MAX_LUCHTING, &AantalGroepen, 0, DIGITAL_INPUT_ID, DIG_IN_RAAM, DigInNotUsed };

static void * const lcd_dig_in_hoge_snelheid[] = { &disp_raamstand_small_ico, &disp_hoge_snelheid_str, &disp_dig_in_hoge_snelheid_sel, 0 };
//-----------------------------------------------------------------------------
// Uitgang snelheid analoog/digitaal
static s_disp_tekst       const disp_uitgang_snelheid_str = { Disp_Draw_Tekst_L,       37, 22, &tekst_inst.Uitgang_snelheid_14 };
static s_disp_tekst_array const disp_digitaal_analoog_str = { Disp_Draw_Tekst_Array_L, 76, 75, &tekst_digitaal_analoog_14, UCHAR, &FreqAnaloog, 2 };

static void * const lcd_uitgang_snelheid_disp[] = { &disp_raamstand_small_ico, &disp_uitgang_snelheid_str, &disp_digitaal_analoog_str, 0 };

static s_key_value const key_uitgang_snelheid_val = { UCHAR, 1, &FreqAnaloog, &uchar_0, &uchar_1 };
//-----------------------------------------------------------------------------
// Snelheid laag/hoog
static s_disp_tekst const disp_snelheid_str       = { Disp_Draw_Tekst_L,  37, 22, &tekst_inst.Snelheid_14 };
static s_disp_tekst const disp_snelheid_laag_str  = { Disp_Draw_Tekst_L,  56, 49, &tekst_inst.Laag_14 };
static s_disp_value const disp_snelheid_laag_val  = { Disp_Draw_Value,   192, 49, (SIZE_14 | RECHTS), UCHAR, 0, &SpeedLow };
static s_disp_tekst const disp_snelheid_laag_perc = { Disp_Draw_Tekst_L, 195, 42, &tekst.perc_7 };
static s_disp_tekst const disp_snelheid_hoog_str  = { Disp_Draw_Tekst_L,  56, 75, &tekst_inst.Hoog_14 };
static s_disp_value const disp_snelheid_hoog_val  = { Disp_Draw_Value,   192, 75, (SIZE_14 | RECHTS), UCHAR, 0, &SpeedHi };
static s_disp_tekst const disp_snelheid_hoog_perc = { Disp_Draw_Tekst_L, 195, 68, &tekst.perc_7 };

static void * const lcd_snelheid_disp[] =
{
  &disp_raamstand_small_ico, &disp_snelheid_str,
  &disp_snelheid_laag_str, &disp_snelheid_laag_val, &disp_snelheid_laag_perc,
  &disp_snelheid_hoog_str, &disp_snelheid_hoog_val, &disp_snelheid_hoog_perc, 0
};

static s_key_value const key_snelheid_laag_val = { UCHAR, 3, &SpeedLow, &uchar_0,  &SpeedHi   };
static s_key_value const key_snelheid_hoog_val = { UCHAR, 3, &SpeedHi,  &SpeedLow, &uchar_100 };
//-----------------------------------------------------------------------------
// Verstel hoge snelheid
static s_disp_tekst const disp_frequentie_verstel_str  = { Disp_Draw_Tekst_L,  37, 22, &tekst_inst.Frequentie_verstel_14 };
static s_disp_value const disp_frequentie_verstel_val  = { Disp_Draw_Value,   192, 75, (SIZE_14 | RECHTS), INT, 1, &FrequentieVerstel };
static s_disp_tekst const disp_frequentie_verstel_perc = { Disp_Draw_Tekst_L, 195, 68, &tekst.perc_7 };

static void * const lcd_frequentie_verstel_disp[] =
{
  &disp_raamstand_small_ico, &disp_frequentie_verstel_str,
  &disp_frequentie_verstel_val, &disp_frequentie_verstel_perc, 0
};

static s_key_value const key_frequentie_verstel_val = { INT, 4, &FrequentieVerstel, &PositionLowSpeed, &int_1000 };
//-----------------------------------------------------------------------------
// Terugschakel positie open/dicht
static s_disp_tekst const disp_terugschakel_positie_str  = { Disp_Draw_Tekst_L,  37, 22, &tekst_inst.Terugschakel_positie_14 };
static s_disp_value const disp_terugschakel_positie_val  = { Disp_Draw_Value,   192, 75, (SIZE_14 | RECHTS), INT, 1, &PositionLowSpeed };
static s_disp_tekst const disp_terugschakel_positie_perc = { Disp_Draw_Tekst_L, 195, 68, &tekst.perc_7 };

static void * const lcd_terugschakel_positie_disp[] =
{
  &disp_raamstand_small_ico, &disp_terugschakel_positie_str,
  &disp_terugschakel_positie_val, &disp_terugschakel_positie_perc, 0
};

static s_key_value const key_terugschakel_positie_val = { INT, 4, &PositionLowSpeed, &int_50, &int_950 };
//-----------------------------------------------------------------------------------------------------------
// Position
//static s_disp_tekst const disp_position_str = { Disp_Draw_Tekst_L,  37, 22, &tekst_inst.Position_14 };
static s_disp_value const disp_position_val = { Disp_Draw_Value,   207, 75, (SIZE_14 | RECHTS), INT, 1, &val_hr_alg.Motorgroup[0].PositionPerc };
static s_disp_tekst const disp_14_perc      = { Disp_Draw_Tekst_L, 210, 68, &tekst.perc_7 };

static void * const lcd_position_disp[] = { &disp_raamstand_small_ico, &disp_position_str, &disp_position_val, &disp_14_perc, 0 };

static s_key_value const key_position_val = { INT, 4, &val_hr_alg.Motorgroup[0].PositionPerc, &int_0, &int_1000 };
//-----------------------------------------------------------------------------
// Koppel motoren aan groepen
static s_disp_tekst     const disp_motoren_str = { Disp_Draw_Tekst_L, 47, 22, &tekst_inst.Motoren_14 };
static s_disp_tekst_add const disp_groep_str   = { Disp_Draw_Tekst_Add_L, &tekst_inst.Groep_14 };
static s_disp_value_add const disp_groep_1     = { Disp_Draw_Value_Add, (SIZE_14 | LINKS), UCHAR, 0, &uchar_1  };
static s_disp_value_add const disp_groep_2     = { Disp_Draw_Value_Add, (SIZE_14 | LINKS), UCHAR, 0, &uchar_2  };
static s_disp_value_add const disp_groep_3     = { Disp_Draw_Value_Add, (SIZE_14 | LINKS), UCHAR, 0, &uchar_3  };
static s_disp_value_add const disp_groep_4     = { Disp_Draw_Value_Add, (SIZE_14 | LINKS), UCHAR, 0, &uchar_4  };
static s_disp_value_add const disp_groep_5     = { Disp_Draw_Value_Add, (SIZE_14 | LINKS), UCHAR, 0, &uchar_5  };
static s_disp_value_add const disp_groep_6     = { Disp_Draw_Value_Add, (SIZE_14 | LINKS), UCHAR, 0, &uchar_6  };
static s_disp_value_add const disp_groep_7     = { Disp_Draw_Value_Add, (SIZE_14 | LINKS), UCHAR, 0, &uchar_7  };
static s_disp_value_add const disp_groep_8     = { Disp_Draw_Value_Add, (SIZE_14 | LINKS), UCHAR, 0, &uchar_8  };
static s_disp_value_add const disp_groep_9     = { Disp_Draw_Value_Add, (SIZE_14 | LINKS), UCHAR, 0, &uchar_9  };
static s_disp_value_add const disp_groep_10    = { Disp_Draw_Value_Add, (SIZE_14 | LINKS), UCHAR, 0, &uchar_10 };
static s_disp_value_add const disp_groep_11    = { Disp_Draw_Value_Add, (SIZE_14 | LINKS), UCHAR, 0, &uchar_11 };
static s_disp_value_add const disp_groep_12    = { Disp_Draw_Value_Add, (SIZE_14 | LINKS), UCHAR, 0, &uchar_12 };
static s_disp_value_add const disp_groep_13    = { Disp_Draw_Value_Add, (SIZE_14 | LINKS), UCHAR, 0, &uchar_13 };
static s_disp_value_add const disp_groep_14    = { Disp_Draw_Value_Add, (SIZE_14 | LINKS), UCHAR, 0, &uchar_14 };
static s_disp_value_add const disp_groep_15    = { Disp_Draw_Value_Add, (SIZE_14 | LINKS), UCHAR, 0, &uchar_15 };
static s_disp_value_add const disp_groep_16    = { Disp_Draw_Value_Add, (SIZE_14 | LINKS), UCHAR, 0, &uchar_16 };
static s_disp_board_IO_Selection const disp_motoren_sel = { Disp_Draw_Board_IO_Select, MotorIO, 16, (unsigned char *)&MaxMotor, 0, MOTOR_CONTROL_ID, MOTOR_CONTROL_RAAM, MotorControlNotUsed };

static s_disp_func const disp_motor_IO_1_func  = { Disp_Control_Func, Disp_Copy_Motor_IO_1  };
static s_disp_func const disp_motor_IO_2_func  = { Disp_Control_Func, Disp_Copy_Motor_IO_2  };
static s_disp_func const disp_motor_IO_3_func  = { Disp_Control_Func, Disp_Copy_Motor_IO_3  };
static s_disp_func const disp_motor_IO_4_func  = { Disp_Control_Func, Disp_Copy_Motor_IO_4  };
static s_disp_func const disp_motor_IO_5_func  = { Disp_Control_Func, Disp_Copy_Motor_IO_5  };
static s_disp_func const disp_motor_IO_6_func  = { Disp_Control_Func, Disp_Copy_Motor_IO_6  };
static s_disp_func const disp_motor_IO_7_func  = { Disp_Control_Func, Disp_Copy_Motor_IO_7  };
static s_disp_func const disp_motor_IO_8_func  = { Disp_Control_Func, Disp_Copy_Motor_IO_8  };
static s_disp_func const disp_motor_IO_9_func  = { Disp_Control_Func, Disp_Copy_Motor_IO_9  };
static s_disp_func const disp_motor_IO_10_func = { Disp_Control_Func, Disp_Copy_Motor_IO_10 };
static s_disp_func const disp_motor_IO_11_func = { Disp_Control_Func, Disp_Copy_Motor_IO_11 };
static s_disp_func const disp_motor_IO_12_func = { Disp_Control_Func, Disp_Copy_Motor_IO_12 };
static s_disp_func const disp_motor_IO_13_func = { Disp_Control_Func, Disp_Copy_Motor_IO_13 };
static s_disp_func const disp_motor_IO_14_func = { Disp_Control_Func, Disp_Copy_Motor_IO_14 };
static s_disp_func const disp_motor_IO_15_func = { Disp_Control_Func, Disp_Copy_Motor_IO_15 };
static s_disp_func const disp_motor_IO_16_func = { Disp_Control_Func, Disp_Copy_Motor_IO_16 };

static void * const lcd_motoren_groep_1[]  = { &disp_motor_IO_1_func,  &disp_motor_ico, &disp_motoren_str, &disp_space_14_L, &disp_groep_str, &disp_rechte_openings_haak_14_L, &disp_groep_1,  &disp_rechte_sluit_haak_14_L, &disp_motoren_sel, 0 };
static void * const lcd_motoren_groep_2[]  = { &disp_motor_IO_2_func,  &disp_motor_ico, &disp_motoren_str, &disp_space_14_L, &disp_groep_str, &disp_rechte_openings_haak_14_L, &disp_groep_2,  &disp_rechte_sluit_haak_14_L, &disp_motoren_sel, 0 };
static void * const lcd_motoren_groep_3[]  = { &disp_motor_IO_3_func,  &disp_motor_ico, &disp_motoren_str, &disp_space_14_L, &disp_groep_str, &disp_rechte_openings_haak_14_L, &disp_groep_3,  &disp_rechte_sluit_haak_14_L, &disp_motoren_sel, 0 };
static void * const lcd_motoren_groep_4[]  = { &disp_motor_IO_4_func,  &disp_motor_ico, &disp_motoren_str, &disp_space_14_L, &disp_groep_str, &disp_rechte_openings_haak_14_L, &disp_groep_4,  &disp_rechte_sluit_haak_14_L, &disp_motoren_sel, 0 };
static void * const lcd_motoren_groep_5[]  = { &disp_motor_IO_5_func,  &disp_motor_ico, &disp_motoren_str, &disp_space_14_L, &disp_groep_str, &disp_rechte_openings_haak_14_L, &disp_groep_5,  &disp_rechte_sluit_haak_14_L, &disp_motoren_sel, 0 };
static void * const lcd_motoren_groep_6[]  = { &disp_motor_IO_6_func,  &disp_motor_ico, &disp_motoren_str, &disp_space_14_L, &disp_groep_str, &disp_rechte_openings_haak_14_L, &disp_groep_6,  &disp_rechte_sluit_haak_14_L, &disp_motoren_sel, 0 };
static void * const lcd_motoren_groep_7[]  = { &disp_motor_IO_7_func,  &disp_motor_ico, &disp_motoren_str, &disp_space_14_L, &disp_groep_str, &disp_rechte_openings_haak_14_L, &disp_groep_7,  &disp_rechte_sluit_haak_14_L, &disp_motoren_sel, 0 };
static void * const lcd_motoren_groep_8[]  = { &disp_motor_IO_8_func,  &disp_motor_ico, &disp_motoren_str, &disp_space_14_L, &disp_groep_str, &disp_rechte_openings_haak_14_L, &disp_groep_8,  &disp_rechte_sluit_haak_14_L, &disp_motoren_sel, 0 };
static void * const lcd_motoren_groep_9[]  = { &disp_motor_IO_9_func,  &disp_motor_ico, &disp_motoren_str, &disp_space_14_L, &disp_groep_str, &disp_rechte_openings_haak_14_L, &disp_groep_9,  &disp_rechte_sluit_haak_14_L, &disp_motoren_sel, 0 };
static void * const lcd_motoren_groep_10[] = { &disp_motor_IO_10_func, &disp_motor_ico, &disp_motoren_str, &disp_space_14_L, &disp_groep_str, &disp_rechte_openings_haak_14_L, &disp_groep_10, &disp_rechte_sluit_haak_14_L, &disp_motoren_sel, 0 };
static void * const lcd_motoren_groep_11[] = { &disp_motor_IO_11_func, &disp_motor_ico, &disp_motoren_str, &disp_space_14_L, &disp_groep_str, &disp_rechte_openings_haak_14_L, &disp_groep_11, &disp_rechte_sluit_haak_14_L, &disp_motoren_sel, 0 };
static void * const lcd_motoren_groep_12[] = { &disp_motor_IO_12_func, &disp_motor_ico, &disp_motoren_str, &disp_space_14_L, &disp_groep_str, &disp_rechte_openings_haak_14_L, &disp_groep_12, &disp_rechte_sluit_haak_14_L, &disp_motoren_sel, 0 };
static void * const lcd_motoren_groep_13[] = { &disp_motor_IO_13_func, &disp_motor_ico, &disp_motoren_str, &disp_space_14_L, &disp_groep_str, &disp_rechte_openings_haak_14_L, &disp_groep_13, &disp_rechte_sluit_haak_14_L, &disp_motoren_sel, 0 };
static void * const lcd_motoren_groep_14[] = { &disp_motor_IO_14_func, &disp_motor_ico, &disp_motoren_str, &disp_space_14_L, &disp_groep_str, &disp_rechte_openings_haak_14_L, &disp_groep_14, &disp_rechte_sluit_haak_14_L, &disp_motoren_sel, 0 };
static void * const lcd_motoren_groep_15[] = { &disp_motor_IO_15_func, &disp_motor_ico, &disp_motoren_str, &disp_space_14_L, &disp_groep_str, &disp_rechte_openings_haak_14_L, &disp_groep_15, &disp_rechte_sluit_haak_14_L, &disp_motoren_sel, 0 };
static void * const lcd_motoren_groep_16[] = { &disp_motor_IO_16_func, &disp_motor_ico, &disp_motoren_str, &disp_space_14_L, &disp_groep_str, &disp_rechte_openings_haak_14_L, &disp_groep_16, &disp_rechte_sluit_haak_14_L, &disp_motoren_sel, 0 };
//-----------------------------------------------------------------------------



s_key_action const opt_luchting_2_key_action[] =
{
  { // Begin scherm
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
  //-- Groepen --------------------------------------------
  { // Aantal
    1,                                // nr
    0,                                // index
    (unsigned char *)&option_on,      // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_aantal_groepen_disp,          // display
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
    lcd_aantal_groepen_disp,          // display
    &disp_cursor_207_78_8,            // cursor
    &key_aantal_groepen_val,          // *value
    Number_Value,                     // void (*number)(void); 
    Arrow_Luchting_Value,             // void (*arrow)(void); 
    Enter_Luchting_Value,             // void (*enter)(void);
  },
  { // Type sturing digitaal/analoog/CANopen/BACnet
    2,                                // nr
    0,                                // index
    &AantalGroepen,                   // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_type_sturing_disp,            // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Option_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    2,                                // nr
    1,                                // index
    &AantalGroepen,                   // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_type_sturing_disp,            // display
    &disp_cursor_225_78_150,          // cursor
    &key_type_sturing_val,            // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Scroll_Luchting_Value,      // void (*arrow)(void); 
    Enter_Luchting_Value,             // void (*enter)(void);
  },
  { // Groep adres in geval van CANopen of BACnet
    3,                                // nr
    0,                                // index
    &DispGroepAdres,                  // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_adres_disp,                   // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Option_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    3,                                // nr
    1,                                // index
    &DispGroepAdres,                  // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_adres_disp,                   // display
    &disp_cursor_207_78_8,            // cursor
    &key_adres_val,                   // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Adres_Value,                // void (*arrow)(void); 
    Enter_Adres_Value,                // void (*enter)(void);
  },
  { // Digitale ingang open
    4,                                // nr
    0,                                // index
    &SturingDigitaal,                 // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_dig_in_open,                  // display
    &disp_cursor,                     // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_Select_Func,             // void (*arrow)(void); 
    Enter_IO_Select_Func,             // void (*enter)(void);
  },
  { // Digitale ingang dicht
    5,                                // nr
    0,                                // index
    &SturingDigitaal,                 // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_dig_in_dicht,                 // display
    &disp_cursor,                     // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_Select_Func,             // void (*arrow)(void); 
    Enter_IO_Select_Func,             // void (*enter)(void);
  },
  { // Analoge ingang positie
    6,                                // nr
    0,                                // index
    &SturingAnaloog,                  // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_ana_in_position,              // display
    &disp_cursor,                     // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_Select_Func,             // void (*arrow)(void); 
    Enter_IO_Select_Func,             // void (*enter)(void);
  },
  { // Analoge uitgang terugmelding
    7,                                // nr
    0,                                // index
    &TerugmeldingAnaloog,             // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_ana_out_terugmelding,         // display
    &disp_cursor,                     // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_Select_Func,             // void (*arrow)(void); 
    Enter_IO_Select_Func,             // void (*enter)(void);
  },
  { // Frequentie gestuurd
    8,                                // nr
    0,                                // index
    &AantalGroepen,                   // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_frequentie_gestuurd_disp,     // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Option_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    8,                                // nr
    1,                                // index
    &AantalGroepen,                   // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_frequentie_gestuurd_disp,     // display
    &disp_cursor_checkbox,            // cursor
    &key_frequentie_gestuurd_val,     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Scroll_Luchting_Value,      // void (*arrow)(void); 
    Enter_Luchting_Value,             // void (*enter)(void);
  },
  { // Digitale ingang hoge snelheid
    9,                                // nr
    0,                                // index
    &HiSpeedDigitaal,                 // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_dig_in_hoge_snelheid,         // display
    &disp_cursor,                     // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_Select_Func,             // void (*arrow)(void); 
    Enter_IO_Select_Func,             // void (*enter)(void);
  },
  { // Uitgang snelheid digitaal/analoog
    10,                               // nr
    0,                                // index
    &FreqGestuurd,                    // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_uitgang_snelheid_disp,        // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Option_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    10,                               // nr
    1,                                // index
    &FreqGestuurd,                    // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_uitgang_snelheid_disp,        // display
    &disp_cursor_225_78_150,          // cursor
    &key_uitgang_snelheid_val,        // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Scroll_Luchting_Value,      // void (*arrow)(void); 
    Enter_Luchting_Value,             // void (*enter)(void);
  },
  { // Snelheid laag/hoog
    11,                               // nr
    0,                                // index
    &FreqAnaloog,                     // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_snelheid_disp,                // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Option_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    11,                               // nr
    1,                                // index
    &FreqAnaloog,                     // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_snelheid_disp,                // display
    &disp_cursor_192_52_8,            // cursor
    &key_snelheid_laag_val,           // *value
    Number_Value,                     // void (*number)(void); 
    Arrow_Luchting_Value,             // void (*arrow)(void); 
    Enter_Luchting_Value,             // void (*enter)(void);
  },
  {
    11,                               // nr
    2,                                // index
    &FreqAnaloog,                     // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_snelheid_disp,                // display
    &disp_cursor_192_78_8,            // cursor
    &key_snelheid_hoog_val,           // *value
    Number_Value,                     // void (*number)(void); 
    Arrow_Luchting_Value,             // void (*arrow)(void); 
    Enter_Luchting_Value,             // void (*enter)(void);
  },
  { // Verstel hoge snelheid
    12,                               // nr
    0,                                // index
    &FreqGestuurd,                    // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_frequentie_verstel_disp,      // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Option_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    12,                               // nr
    1,                                // index
    &FreqGestuurd,                    // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_frequentie_verstel_disp,      // display
    &disp_cursor_192_78_8,            // cursor
    &key_frequentie_verstel_val,      // *value
    Number_Value,                     // void (*number)(void); 
    Arrow_Luchting_Value,             // void (*arrow)(void); 
    Enter_Luchting_Value,             // void (*enter)(void);
  },
  { // Terugschakel positie open/dicht
    13,                               // nr
    0,                                // index
    &FreqGestuurd,                    // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_terugschakel_positie_disp,    // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Option_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    13,                               // nr
    1,                                // index
    &FreqGestuurd,                    // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_terugschakel_positie_disp,    // display
    &disp_cursor_192_78_8,            // cursor
    &key_terugschakel_positie_val,    // *value
    Number_Value,                     // void (*number)(void); 
    Arrow_Luchting_Value,             // void (*arrow)(void); 
    Enter_Luchting_Value,             // void (*enter)(void);
  },
/*
  { // Actuele positie
    14,                               // nr
    0,                                // index
    &SturingDigitaal,                 // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_position_disp,                // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Position_Func,              // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    14,                               // nr
    1,                                // index
    &SturingDigitaal,                 // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_position_disp,                // display
    &disp_cursor_207_78_8,            // cursor
    &key_position_val,                // *value
    Number_Position_Value,            // void (*number)(void); 
    Arrow_Position_Value,             // void (*arrow)(void); 
    Enter_Position_Value,             // void (*enter)(void);
  },
*/
  { // Digitale uitgang alarm
    15,                               // nr
    0,                                // index
    &AantalGroepen,                   // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_dig_out_alarm,                // display
    &disp_cursor,                     // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_Select_Func,             // void (*arrow)(void); 
    Enter_IO_Select_Func,             // void (*enter)(void);
  },
  { // Alarm analoog
    16,                               // nr
    0,                                // index
    &TerugmeldingAnaloog,             // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_alarm_analoog_disp,           // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Option_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    16,                               // nr
    1,                                // index
    &TerugmeldingAnaloog,             // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_alarm_analoog_disp,           // display
    &disp_cursor_checkbox,            // cursor
    &key_alarm_analoog_val,           // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Scroll_Luchting_Value,      // void (*arrow)(void); 
    Enter_Luchting_Value,             // void (*enter)(void);
  },
  //-- Motoren --------------------------------------------
  { // IO motoren groep 1
    17,                               // nr
    0,                                // index
    &GroepEnabled[0],                 // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_motoren_groep_1,              // display
    &disp_cursor,                     // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_Motor_Func,              // void (*arrow)(void); 
    Enter_IO_Select_Func,             // void (*enter)(void);
  },
  { // IO motoren groep 2
    18,                               // nr
    0,                                // index
    &GroepEnabled[1],                 // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_motoren_groep_2,              // display
    &disp_cursor,                     // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_Motor_Func,              // void (*arrow)(void); 
    Enter_IO_Select_Func,             // void (*enter)(void);
  },
  { // IO motoren groep 3
    19,                               // nr
    0,                                // index
    &GroepEnabled[2],                 // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_motoren_groep_3,              // display
    &disp_cursor,                     // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_Motor_Func,              // void (*arrow)(void); 
    Enter_IO_Select_Func,             // void (*enter)(void);
  },
  { // IO motoren groep 4
    20,                               // nr
    0,                                // index
    &GroepEnabled[3],                 // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_motoren_groep_4,              // display
    &disp_cursor,                     // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_Motor_Func,              // void (*arrow)(void); 
    Enter_IO_Select_Func,             // void (*enter)(void);
  },
  { // IO motoren groep 5
    21,                               // nr
    0,                                // index
    &GroepEnabled[4],                 // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_motoren_groep_5,              // display
    &disp_cursor,                     // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_Motor_Func,              // void (*arrow)(void); 
    Enter_IO_Select_Func,             // void (*enter)(void);
  },
  { // IO motoren groep 6
    22,                               // nr
    0,                                // index
    &GroepEnabled[5],                 // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_motoren_groep_6,              // display
    &disp_cursor,                     // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_Motor_Func,              // void (*arrow)(void); 
    Enter_IO_Select_Func,             // void (*enter)(void);
  },
  { // IO motoren groep 7
    23,                               // nr
    0,                                // index
    &GroepEnabled[6],                 // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_motoren_groep_7,              // display
    &disp_cursor,                     // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_Motor_Func,              // void (*arrow)(void); 
    Enter_IO_Select_Func,             // void (*enter)(void);
  },
  { // IO motoren groep 8
    24,                               // nr
    0,                                // index
    &GroepEnabled[7],                 // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_motoren_groep_8,              // display
    &disp_cursor,                     // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_Motor_Func,              // void (*arrow)(void); 
    Enter_IO_Select_Func,             // void (*enter)(void);
  },
  { // IO motoren groep 9
    25,                               // nr
    0,                                // index
    &GroepEnabled[8],                 // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_motoren_groep_9,              // display
    &disp_cursor,                     // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_Motor_Func,              // void (*arrow)(void); 
    Enter_IO_Select_Func,             // void (*enter)(void);
  },
  { // IO motoren groep 10
    26,                               // nr
    0,                                // index
    &GroepEnabled[9],                 // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_motoren_groep_10,             // display
    &disp_cursor,                     // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_Motor_Func,              // void (*arrow)(void); 
    Enter_IO_Select_Func,             // void (*enter)(void);
  },
  { // IO motoren groep 11
    27,                               // nr
    0,                                // index
    &GroepEnabled[10],                // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_motoren_groep_11,             // display
    &disp_cursor,                     // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_Motor_Func,              // void (*arrow)(void); 
    Enter_IO_Select_Func,             // void (*enter)(void);
  },
  { // IO motoren groep 12
    28,                               // nr
    0,                                // index
    &GroepEnabled[11],                // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_motoren_groep_12,             // display
    &disp_cursor,                     // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_Motor_Func,              // void (*arrow)(void); 
    Enter_IO_Select_Func,             // void (*enter)(void);
  },
  { // IO motoren groep 13
    29,                               // nr
    0,                                // index
    &GroepEnabled[12],                // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_motoren_groep_13,             // display
    &disp_cursor,                     // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_Motor_Func,              // void (*arrow)(void); 
    Enter_IO_Select_Func,             // void (*enter)(void);
  },
  { // IO motoren groep 14
    30,                               // nr
    0,                                // index
    &GroepEnabled[13],                // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_motoren_groep_14,             // display
    &disp_cursor,                     // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_Motor_Func,              // void (*arrow)(void); 
    Enter_IO_Select_Func,             // void (*enter)(void);
  },
  { // IO motoren groep 15
    31,                               // nr
    0,                                // index
    &GroepEnabled[14],                // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_motoren_groep_15,             // display
    &disp_cursor,                     // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_Motor_Func,              // void (*arrow)(void); 
    Enter_IO_Select_Func,             // void (*enter)(void);
  },
  { // IO motoren groep 16
    32,                               // nr
    0,                                // index
    &GroepEnabled[15],                // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_motoren_groep_16,             // display
    &disp_cursor,                     // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_Motor_Func,              // void (*arrow)(void); 
    Enter_IO_Select_Func,             // void (*enter)(void);
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
s_screen screen_opt_luchting_2;
s_screen const screen_opt_luchting_2_default =
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
  &opt_luchting_2_key_action[0], // first_action
  &opt_luchting_2_key_action[sizeof(opt_luchting_2_key_action)/sizeof(s_key_action) - 1], // last action 
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

void Control_Screen_Option_Luchting_2(void)
{
  if (password_enabled & PASSWORD_ENABLED_OPT_MASK)
    start_flag = end_flag = 1;
  else
    start_flag = end_flag = 0;
  Enter_IO_Select_Func();
  
  SetDisplayOptions();

  Control_Screen(&screen_opt_luchting_2, &screen_opt_luchting_2_default, 1, 1);
}

//------------------------------------------------------------------------------
static void SetDisplayOptions(void)
{
unsigned char i;

  AantalGroepen = 0;

  for (i = 0; i < MAX_LUCHTING; i++)
    GroepEnabled[i] = 0;

  for (i = 0; i < MAX_GROUP; i++)
  {
    if (opt_app.Motorgroup[i].Enabled && (opt_app.Motorgroup[i].Type == TYPE_RAAM))
    {
      GroepEnabled[AantalGroepen] = 1;
      AantalGroepen++;
      TypeSturing       = opt_app.Motorgroup[i].AnalogInput;
	  FreqGestuurd      = opt_app.Motorgroup[i].FrequencyControlled;
	  FreqAnaloog       = opt_app.Motorgroup[i].AnalogSpeed;
	  FrequentieVerstel = opt_app.Motorgroup[i].FrequentieVerstel;
	  PositionLowSpeed  = opt_app.Motorgroup[i].PositionLowSpeed;

      switch (opt_app.Motorgroup[i].AnalogInput)
      {
        case TYPE_STURING_DIGITAAL:
          DispGroepAdres      = 0;
		  SturingDigitaal     = 1;
		  SturingAnaloog      = 0;
		  TerugmeldingAnaloog = 1;
		  HiSpeedDigitaal     = opt_app.Motorgroup[i].FrequencyControlled;
	      AlarmAnaloog        = opt_app.Motorgroup[i].AlarmAnaloog;
          break;
        case TYPE_STURING_ANALOOG:
          DispGroepAdres      = 0;
		  SturingDigitaal     = 0;
		  SturingAnaloog      = 1;
		  TerugmeldingAnaloog = 1;
		  HiSpeedDigitaal     = opt_app.Motorgroup[i].FrequencyControlled;
	      AlarmAnaloog        = opt_app.Motorgroup[i].AlarmAnaloog;
          break;
        case TYPE_STURING_CANOPEN:
          DispGroepAdres      = 1;
		  SturingDigitaal     = 0;
		  SturingAnaloog      = 0;
		  TerugmeldingAnaloog = 0;
          HiSpeedDigitaal     = 0;
	      AlarmAnaloog        = 0;
          break;
        case TYPE_STURING_BACNET:
          DispGroepAdres      = 1;
		  SturingDigitaal     = 0;
		  SturingAnaloog      = 0;
		  TerugmeldingAnaloog = 0;
          HiSpeedDigitaal     = 0;
	      AlarmAnaloog        = 0;
          break;
      }
    }
  }
  opt_app.NumberMotorgroups = AantalGroepen;
}

//------------------------------------------------------------------------------
void CheckOptionsLuchting(void)
{
int i;

  for (i = 0; i < MAX_GROUP; i++)
  {
    if (opt_app.Motorgroup[i].Enabled)
    {
      if (opt_app.Motorgroup[i].Type == TYPE_RAAM)
      {
        switch (opt_app.Motorgroup[i].AnalogInput)
        {
          case TYPE_STURING_DIGITAAL:
            opt_app.Motorgroup[i].AnaInPosition = IO_empty;
            break;
          case TYPE_STURING_ANALOOG:
            opt_app.Motorgroup[i].DigInOpen  = IO_empty;
            opt_app.Motorgroup[i].DigInClose = IO_empty;
            break;
          case TYPE_STURING_CANOPEN:
            opt_app.Motorgroup[i].DigInOpen      = IO_empty;
            opt_app.Motorgroup[i].DigInClose     = IO_empty;
            opt_app.Motorgroup[i].AnaInPosition  = IO_empty;
            opt_app.Motorgroup[i].AnaOutPosition = IO_empty;
            opt_app.Motorgroup[i].DigInHiSpeed   = IO_empty;
            break;
          case TYPE_STURING_BACNET:
            opt_app.Motorgroup[i].DigInOpen      = IO_empty;
            opt_app.Motorgroup[i].DigInClose     = IO_empty;
            opt_app.Motorgroup[i].AnaInPosition  = IO_empty;
            opt_app.Motorgroup[i].AnaOutPosition = IO_empty;
            opt_app.Motorgroup[i].DigInHiSpeed   = IO_empty;
            break;
        }

        opt_app.Motorgroup[i].PulseSystem  = 0;
        opt_app.Motorgroup[i].KierRegeling = 0;

        if (opt_app.Motorgroup[i].FrequencyControlled == 0)
        {
          opt_app.Motorgroup[i].AnalogSpeed  = 0;
          opt_app.Motorgroup[i].DigInHiSpeed = IO_empty;
        }
      }
	}
    else
    {
      opt_app.Motorgroup[i]    = default_opt_app.Motorgroup[i];
      setp_alg.Motorgroup[i]   = default_setp_alg.Motorgroup[i];
      val_hr_alg.Motorgroup[i] = default_val_hr_alg.Motorgroup[i];
    }
  }
}

//-----------------------------------------------------------------------------
/*
void CheckOptionsMotor(void)
{
int i, j;
int FirstFree;
int MotorIndex;

  for (i = 0; i < MAX_MOTOR; i++)
  {
    if (!opt_app.Motor[i].Enabled || !opt_app.Motor[i].GroupNumber || ((opt_app.Motorgroup[opt_app.Motor[i].GroupNumber-1].Type != TYPE_RAAM) && (opt_app.Motorgroup[opt_app.Motor[i].GroupNumber-1].Type != TYPE_DOEK)))
    {
      opt_app.Motor[i]    = default_opt_app.Motor[i];
      setp_alg.Motor[i]   = default_setp_alg.Motor[i];
      val_hr_alg.Motor[i] = default_val_hr_alg.Motor[i];
    }
  }
  for (i = 0; i < opt_app.NumberMotorgroups; i++)
  {
    if ((opt_app.Motorgroup[i].Type == TYPE_RAAM) || (opt_app.Motorgroup[i].Type == TYPE_DOEK))
    {
      FirstFree = MAX_MOTOR;
      MotorIndex = 0;
      for (j = 0; j < MAX_MOTOR; j++)
      {
        if (opt_app.Motor[j].Enabled)
        {
          if (opt_app.Motor[j].GroupNumber == i+1)
          {
            if (MotorIndex < opt_app.Motorgroup[i].NumberMotors)
            {
              if (opt_app.Motorgroup[i].Type == TYPE_RAAM)
                IO_Set_Motor_Control_Option(&opt_app.Motor[j].IO, 1);
              MotorIndex++;
            }
            else
            {
              opt_app.Motor[j]    = default_opt_app.Motor[j];
              setp_alg.Motor[j]   = default_setp_alg.Motor[j];
              val_hr_alg.Motor[j] = default_val_hr_alg.Motor[j];
            }
          }
        }
        else if (FirstFree == MAX_MOTOR)
          FirstFree = j;
      }
      if (MotorIndex < opt_app.Motorgroup[i].NumberMotors)
      {
        for (j = FirstFree; j < MAX_MOTOR; j++)
        {
          if (!opt_app.Motor[j].Enabled)
          {
            opt_app.Motor[j].Enabled     = 1;
            opt_app.Motor[j].GroupNumber = i+1;
            MotorIndex++;
            if (opt_app.Motorgroup[i].Type == TYPE_RAAM)
              IO_Set_Motor_Control_Option(&opt_app.Motor[j].IO, 1);
            if (MotorIndex >= opt_app.Motorgroup[i].NumberMotors)
              break;
          }
        }
      }
    }
  }
}

//-----------------------------------------------------------------------------
void CheckOptionsFrequencyControl(void)
{
int i;
int Open, Close;
TMotor *pMotor;

  // Check all motors
  for (i = 0; i < opt_app.NumberMotorgroups; i++)
  {
    // Check if frequency controlled and set settings
    if (opt_app.Motorgroup[i].FrequencyControlled)
    {
      if (opt_app.Motorgroup[i].Type == TYPE_DOEK)
      {
        Open  = 0    + opt_app.Motorgroup[i].PositionLowSpeed;
        Close = 1000 - opt_app.Motorgroup[i].PositionLowSpeed;
      }
      else
      {
        Close = 0    + opt_app.Motorgroup[i].PositionLowSpeed;
        Open  = 1000 - opt_app.Motorgroup[i].PositionLowSpeed;
      }
      pMotor = Motorgroup[i].FirstMotor;
      while (pMotor != NULL)
      {
        IO_Set_Motor_Control_Frequency(&opt_app.Motor[pMotor->Number].IO, opt_app.Motorgroup[i].SpeedLow, opt_app.Motorgroup[i].SpeedHi, Close, Open);
        pMotor = pMotor->Next;
      }
    }
  }
}
*/

//------------------------------------------------------------------------------
static unsigned char DummyNotUsed(s_board_IO_on_off IO_new)
{
  IO_new;
  return (1);
}

static unsigned char AnaInNotUsed(s_board_IO_on_off IO_new)
{
int i;

  for (i = 0; i < MAX_GROUP; i++)
  {
    if (Board_IO_Used(IO_new, &opt_app.Motorgroup[i].AnaInPosition, 1)) return (0);
  }
  return (1);
}

static unsigned char AnaOutNotUsed(s_board_IO_on_off IO_new)
{
int i;

  for (i = 0; i < MAX_GROUP; i++)
  {
    if (Board_IO_Used(IO_new, &opt_app.Motorgroup[i].AnaOutPosition, 1)) return (0);
  }
  return (1);
}

static unsigned char DigOutNotUsed(s_board_IO_on_off IO_new)
{
int i;

  if (Board_IO_Used(IO_new, &opt_app.Alarm.DigOutZacht, 1)) return (0);
  for (i = 0; i < MAX_GROUP; i++)
  {
    if (Board_IO_Used(IO_new, &opt_app.Motorgroup[i].DigOutAlarm,      1)) return (0);
    if (Board_IO_Used(IO_new, &opt_app.Motorgroup[i].DigOutAlarmFlap,  1)) return (0);
    if (Board_IO_Used(IO_new, &opt_app.Ventgroup[i].DigOutAlarmUrgent, 1)) return (0);
  }
  return (1);
}

static unsigned char MotorControlNotUsed(s_board_IO_on_off IO_new)
{
int i;

  for (i = 0; i < MAX_MOTOR; i++)
  {
    if (Board_IO_Used(IO_new, &opt_app.Motor[i].IO, 1)) return (0);
  }
  return (1);
}

//-----------------------------------------------------------------------------
static void Key_Luchting_Value(void)
{
int i, j = 0;

  if (screen_ptr->index == 0)
  {
    for (i = 0; i < MAX_GROUP; i++)
	{
	  if (opt_app.Motorgroup[i].Enabled)
	  {
	    if (opt_app.Motorgroup[i].Type == TYPE_RAAM)
		{
		  if (j < AantalGroepen)
			j++;
		  else
		    opt_app.Motorgroup[i].Enabled = 0;
		}
	  }
	  else
	  {
		if (j < AantalGroepen)
		{
		  opt_app.Motorgroup[i].Enabled = 1;
		  opt_app.Motorgroup[i].Type = TYPE_RAAM;
		  j++;
		}
	  }
	}

    for (i = 0; i < MAX_GROUP; i++)
	{
	  if (opt_app.Motorgroup[i].Enabled && (opt_app.Motorgroup[i].Type == TYPE_RAAM))
	  {
        opt_app.Motorgroup[i].AnalogInput         = TypeSturing;
        opt_app.Motorgroup[i].FrequencyControlled = FreqGestuurd;
		opt_app.Motorgroup[i].AnalogSpeed         = FreqAnaloog;
	    opt_app.Motorgroup[i].FrequentieVerstel   = FrequentieVerstel;
	    opt_app.Motorgroup[i].PositionLowSpeed    = PositionLowSpeed;
		opt_app.Motorgroup[i].AlarmAnaloog        =	AlarmAnaloog;

	  }
	}

    CheckOptionsLuchting();
    SetDisplayOptions();
    Refresh_Screen_Nr_Aantal();
  }
}

static void Arrow_Luchting_Value(void)
{
  Arrow_Option_Value();
  Key_Luchting_Value();
}

static void Arrow_Scroll_Luchting_Value(void)
{
  Arrow_Scroll_Option_Value();
  Key_Luchting_Value();
}

static void Enter_Luchting_Value(void)
{
  Increment_Func_Index_Enter_Option_Value();
  Key_Luchting_Value();
}

//================================================================================
static void Disp_Adres_Func(void)
{
  if (screen_ptr->index == 0)
  {
    GroepNummer = 1;
//    GroepAdres  = opt_app.Luchting[0].Servo.Adres;
  }
}

static unsigned char AdresOk(unsigned char Adres)
{
int i;

  if (Adres == 0)
    return (1);
  
  for (i = 0; i < MAX_GROUP; i++)
  {
    if (opt_app.Motorgroup[i].Enabled)
    {
//      if ((i != (GroepNummer - 1)) && (Adres == opt_app.Luchting[i].Servo.Adres))
        return (0);
    }
  }
  return (1);
}

static void Arrow_Adres_Value(void)
{
long old_val = screen_ptr->value;

  switch (key)
  {
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
      while (AdresOk(screen_ptr->value) == 0);
      if (old_val != screen_ptr->value)
      {
        option_change_flag = 1;
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
      while (AdresOk(screen_ptr->value) == 0);
      if (old_val != screen_ptr->value)
      {
        option_change_flag = 1;
        Put_Value();
      }
      break;
    case LEFT:
//      opt_app.Luchting[GroepNummer - 1].Servo.Adres = GroepAdres;
      if (GroepNummer > 1)
      {
        GroepNummer--;
//        GroepAdres = opt_app.Luchting[GroepNummer - 1].Servo.Adres;
        Get_Value();
      }
      else
        Arrow_Left_Value();
      break;
    case RIGHT:
//      opt_app.Luchting[GroepNummer - 1].Servo.Adres = GroepAdres;
      if (GroepNummer < AantalGroepen)
      {
        GroepNummer++;
//        GroepAdres = opt_app.Luchting[GroepNummer - 1].Servo.Adres;
        Get_Value();
      }
      else
        Increment_Func_Index();
      break;
  }
}

static void Enter_Adres_Value(void)
{
  if (screen_ptr->change_flag)
  {
    if ((screen_ptr->value >= screen_ptr->min_value) && (screen_ptr->value <= screen_ptr->max_value))
    {
      if ((GroepAdres != screen_ptr->value) && AdresOk(screen_ptr->value))
        Enter_Value();
    }
  }
//  opt_app.Luchting[GroepNummer - 1].Servo.Adres = GroepAdres;
  if (GroepNummer < AantalGroepen)
  {
    GroepNummer++;
//    GroepAdres = opt_app.Luchting[GroepNummer - 1].Servo.Adres;
    Get_Value();
  }
  else
    Increment_Func_Index();
}

//================================================================================
static void Arrow_Position_Func(void)
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
      if (password_enabled & PASSWORD_ENABLED_OPT_MASK)
      {
        Increment_Func_Index();
        PositionManual = screen_ptr->value;
      }
      break;
  }
}

static void Number_Position_Value(void)
{
  if (!Numeric)
  {
    screen_ptr->change_flag = 0;
    Numeric = 1;
  }
  Number_Value();
}

static void Arrow_Position_Value(void)
{
  switch (key)
  {
    case LEFT:  if (!Numeric)
                  screen_ptr->change_flag = 0;
                Arrow_Left_Value();
//                if (screen_ptr->index == 0)
//                  val_hr_alg.Motorgroup[GroupIndex].PositionTime = Calc_Prop(0, 1000, 0, opt_app.Motorgroup[GroupIndex].Runtime, PositionManual);
                break;
    case RIGHT: Increment_Func_Index();
//                if (screen_ptr->index == 0)
//                  val_hr_alg.Motorgroup[GroupIndex].PositionTime = Calc_Prop(0, 1000, 0, opt_app.Motorgroup[GroupIndex].Runtime, PositionManual);
                break;
    case UP:    Increment_Value_No_Enter();
                Numeric = 0;
                PositionManual = screen_ptr->value;
                break;
    case DOWN:  Decrement_Value_No_Enter();
                Numeric = 0;
                PositionManual = screen_ptr->value;
                break;
  }
}

static void Enter_Position_Value(void)
{
  if (screen_ptr->change_flag)
  {
    Correct_Decimal_Value();
    if ((screen_ptr->value >= screen_ptr->min_value) && (screen_ptr->value <= screen_ptr->max_value))
      PositionManual = screen_ptr->value;
  }
  Increment_Func_Index();
//  if (screen_ptr->index == 0)
//    val_hr_alg.Motorgroup[GroupIndex].PositionTime = Calc_Prop(0, 1000, 0, opt_app.Motorgroup[GroupIndex].Runtime, PositionManual);
}
                                
//-----------------------------------------------------------------------------
static unsigned char IO_Motor_Select_First_Free(void)
{
unsigned char nr;

  for (nr = 0; nr < MAX_MOTOR; nr++)
  {
    if (!opt_app.Motor[nr].Enabled)
      return (nr + 1);
  }
  return (0);
}

static void Arrow_Up_IO_Motor_Select(void)
{
int loop;

  if (password_enabled & PASSWORD_ENABLED_OPT_MASK)
  {
    if (board_IO_max_nr == 1)
    {
      for (loop = 0; loop < board_IO_max; loop++)
        board_IO[loop].on_off = 0;
    }
    if (!board_IO[index_array - 1].on_off)
      board_IO[index_array - 1].on_off = IO_Motor_Select_First_Free();
    Arrow_Right_IO_Select();
  }
}

static void Arrow_IO_Motor_Func(void)
{
  switch (key)
  {
    case UP:
      if (index_array == 0)
        Decrement_Func();
      else
        Arrow_Up_IO_Motor_Select();
      break;
    case DOWN:
      if (index_array == 0)
        Increment_Func();
      else
        Arrow_Down_IO_Select();
      break;
    case LEFT:
      Arrow_Left_IO_Select();
      break;
    case RIGHT:
      Arrow_Right_IO_Select();
      break;
  }
}

//-----------------------------------------------------------------------------
static void CopyMotorIOToArray(void)
{
int IO_index;
int i;

  for (i = 0; i < 16; i++)
    MotorIO[i] = IO_empty;

  IO_index = 0;
  for (i = 0; i < MAX_MOTOR; i++)
  {
    if (opt_app.Motor[i].Enabled && (opt_app.Motor[i].ControlType == TYPE_RAAM) && (opt_app.Motor[i].GroupNumber == GroupMotorIO))
    {
      MotorIO[IO_index] = opt_app.Motor[i].IO;
      IO_index++;
      if (IO_index >= 16)
        return;
    }
  }
}

static void CopyArrayToMotorIO(void)
{
int i;

  for (i = 0; i < board_IO_array_size; i++)
  {
    if ((board_IO[i].board_type != MotorIO[i].board_type) || (board_IO[i].board_nr != MotorIO[i].board_nr) || (board_IO[i].IO_nr != MotorIO[i].IO_nr) || (board_IO[i].on_off != MotorIO[i].on_off))
    {
      option_change_flag = 1;
      break;
    }
  }
  for (i = 0; i < MAX_MOTOR; i++)
  {
    if (opt_app.Motor[i].Enabled && (opt_app.Motor[i].ControlType == TYPE_RAAM) && (opt_app.Motor[i].GroupNumber == GroupMotorIO))
      opt_app.Motor[i].IO = IO_empty;
  }
  for (i = 0; i < board_IO_max; i++)
  {
    if (board_IO[i].board_type && board_IO[i].on_off)
      opt_app.Motor[board_IO[i].on_off-1].IO = board_IO[i];
  }
  for (i = 0; i < MAX_MOTOR; i++)
  {
    if (opt_app.Motor[i].Enabled && (opt_app.Motor[i].ControlType == TYPE_RAAM) && (opt_app.Motor[i].GroupNumber == GroupMotorIO) && (opt_app.Motor[i].IO.board_type == 0))
      opt_app.Motor[i] = default_opt_app.Motor[i];
  }
}

//-----------------------------------------------------------------------------
static void Disp_Copy_Motor_IO(unsigned char nr)
{
  if (index_array == 0)
  {
    if (CopyMotorIO)
    {
      Install_Copy_Board_IO_To_IO();
      CopyArrayToMotorIO();
      CopyMotorIO = 0;
    }
    else
    {
      GroupMotorIO = nr;
      CopyMotorIOToArray();
    }
  }
  else
  {
    CopyMotorIO = 1;
  }
}

static void Disp_Copy_Motor_IO_1(void)  { Disp_Copy_Motor_IO(0);  }
static void Disp_Copy_Motor_IO_2(void)  { Disp_Copy_Motor_IO(1);  }
static void Disp_Copy_Motor_IO_3(void)  { Disp_Copy_Motor_IO(2);  }
static void Disp_Copy_Motor_IO_4(void)  { Disp_Copy_Motor_IO(3);  }
static void Disp_Copy_Motor_IO_5(void)  { Disp_Copy_Motor_IO(4);  }
static void Disp_Copy_Motor_IO_6(void)  { Disp_Copy_Motor_IO(5);  }
static void Disp_Copy_Motor_IO_7(void)  { Disp_Copy_Motor_IO(6);  }
static void Disp_Copy_Motor_IO_8(void)  { Disp_Copy_Motor_IO(7);  }
static void Disp_Copy_Motor_IO_9(void)  { Disp_Copy_Motor_IO(8);  }
static void Disp_Copy_Motor_IO_10(void) { Disp_Copy_Motor_IO(9);  }
static void Disp_Copy_Motor_IO_11(void) { Disp_Copy_Motor_IO(10); }
static void Disp_Copy_Motor_IO_12(void) { Disp_Copy_Motor_IO(11); }
static void Disp_Copy_Motor_IO_13(void) { Disp_Copy_Motor_IO(12); }
static void Disp_Copy_Motor_IO_14(void) { Disp_Copy_Motor_IO(13); }
static void Disp_Copy_Motor_IO_15(void) { Disp_Copy_Motor_IO(14); }
static void Disp_Copy_Motor_IO_16(void) { Disp_Copy_Motor_IO(15); }

//*****************************************************************************
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
      CheckOptions();
      SetDisplayOptions();
      Prev_Screen();
      break;
    case RIGHT:
      break;
  }
}
