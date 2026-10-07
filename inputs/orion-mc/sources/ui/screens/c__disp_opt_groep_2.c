// C__DISP_OPT_GROEP_2.C

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
#include "ch_mb_device.h"
#include "ch_motor.h"
#include "ch_device.h"
#include "ch_string.h" 
#include "ch_disp_opt_groep_2.h"

static void SetDisplayOptions(void);

static unsigned char DummyNotUsed(s_board_IO_on_off IO_new);
static unsigned char AnaInNotUsed(s_board_IO_on_off IO_new);
static unsigned char DigInHiSpeedNotUsed(s_board_IO_on_off IO_new);
static unsigned char AnaOutNotUsed(s_board_IO_on_off IO_new);
static unsigned char DigOutNotUsed(s_board_IO_on_off IO_new);
static unsigned char MotorControlNotUsed(s_board_IO_on_off IO_new);

static void Arrow_Type_Value(void);
static void Enter_Type_Value(void);

static void Arrow_Value_Refresh_Screen(void);
static void Enter_Value_Refresh_Screen(void);
static void Arrow_Scroll_Value_Refresh_Screen(void);

static void Arrow_Type_Device_Value(void);
static void Enter_Type_Device_Value(void);

static void Arrow_Range_Device_Number_Value(void);
static void Enter_Range_Device_Number_Value(void);
static void Arrow_Range_Device_Address_Value(void);
static void Enter_Range_Device_Address_Value(void);

static void Arrow_Wijzig_Adres_Func(void);
static void Arrow_Wijzig_Adres_Value(void);
static void Enter_Wijzig_Adres_Value(void);
static void Enter_Wijzig_Adres_Func(void);

static void Arrow_Position_Func(void);
static void Arrow_Position_Value(void);
static void Enter_Position_Value(void);
static void Number_Position_Value(void);

static void GetMaxMotor(void);

static void Arrow_IO_Device_Select_Func(void);
static void Enter_IO_Device_Select_Func(void);
static void Arrow_IO_Motor_Func(void);

static void Disp_Draw_Board_Motor_IO_Select(void *s);
static void Disp_Draw_Board_Motor_IO_Select_Option_On(void *s);
static void Disp_Draw_Board_Motor_IO_Select_Option_Off(void *s);

static void Arrow_End_Func(void);

//*****************************************************************************
// STRING ARRAY'S 
//*****************************************************************************
static void const * const tekst_opties_groep_10[] =
{
  &tekst_inst.Opties_groep_1_10,  &tekst_inst.Opties_groep_2_10,  &tekst_inst.Opties_groep_3_10,  &tekst_inst.Opties_groep_4_10,  &tekst_inst.Opties_groep_5_10,
  &tekst_inst.Opties_groep_6_10,  &tekst_inst.Opties_groep_7_10,  &tekst_inst.Opties_groep_8_10,  &tekst_inst.Opties_groep_9_10,  &tekst_inst.Opties_groep_10_10,
  &tekst_inst.Opties_groep_11_10, &tekst_inst.Opties_groep_12_10, &tekst_inst.Opties_groep_13_10, &tekst_inst.Opties_groep_14_10, &tekst_inst.Opties_groep_15_10,
  &tekst_inst.Opties_groep_16_10, &tekst_inst.Opties_groep_17_10, &tekst_inst.Opties_groep_18_10, &tekst_inst.Opties_groep_19_10, &tekst_inst.Opties_groep_20_10,
  &tekst_inst.Opties_groep_21_10, &tekst_inst.Opties_groep_22_10, &tekst_inst.Opties_groep_23_10, &tekst_inst.Opties_groep_24_10, &tekst_inst.Opties_groep_25_10,
  &tekst_inst.Opties_groep_26_10, &tekst_inst.Opties_groep_27_10, &tekst_inst.Opties_groep_28_10, &tekst_inst.Opties_groep_29_10, &tekst_inst.Opties_groep_30_10,
  &tekst_inst.Opties_groep_31_10, &tekst_inst.Opties_groep_32_10
};
static unsigned char GroupIndex;
                                            
static s_disp_agri_header const disp_header = { Disp_Draw_Agri_Header, FN6, EMPTY, &tekst_leeg, HK_GEEN, 0, 3};
static s_disp_tekst_array const disp_header_str_0 = { Disp_Draw_Tekst_Array_L, 12, 12, &tekst_opties_groep_10, UCHAR, &GroupIndex, MAX_GROUP };
static void * const lcd_disp_header[] = { &disp_header_str_0, &disp_header, 0 };

static s_board_IO_on_off MotorIO[16];

static int PositionManual;
static unsigned char Numeric;

static unsigned char MaxMotor;

static unsigned char DispMotorIO;
static unsigned char DispModbusIO;
static unsigned char DispLooptijd;
static unsigned char DispFrequency;
static unsigned char DispFrequencyVerstel;
static unsigned char DispAlarmAnaloog;
static unsigned char DispClosedLoopOpenLoop;
static unsigned char DispDeviceNumberNotAvailable;
static unsigned char DispWatchdogMode;

static unsigned int OudAdres;
static unsigned int NieuwAdres;
static unsigned int const MaxAdres = MAX_MB_DEVICE;
static unsigned char AdresWijzigenOk;
static unsigned char AdresWijzigenMislukt;
static unsigned char AdresWijzigenStatus;

//*****************************************************************************
// SCHERM OPBOUW
//*****************************************************************************
// Type: Raam/Scherm/Ventilatie/Klep
static void const * const tekst_type_14[] = { &tekst_inst.Raam_14, &tekst_inst.Doek_14, &tekst_inst.Ventilatie_14, &tekst_inst.Klep_14 };
static s_disp_tekst_array  const disp_group_str = { Disp_Draw_Tekst_Array_L, 37, 22, &tekst_inst_groep_14, UCHAR, &GroupIndex, MAX_GROUP };
static s_disp_tekst_array  const disp_type_str  = { Disp_Draw_Tekst_Array_L, 76, 75, &tekst_type_14, UCHAR, &opt_app.Motorgroup[0].Type, 4 };
static s_disp_bitmap_array const disp_group_ico = { Disp_Draw_Bitmap_Array, 10, 6, ico_type_array, UCHAR, &opt_app.Motorgroup[0].Type, 4 };

static void * const lcd_type_disp[] = { &disp_group_ico, &disp_group_str, &disp_type_str, 0 };

static s_key_value const key_type_val = { UCHAR, 1, &opt_app.Motorgroup[0].Type, &uchar_0, &uchar_3 };
//-----------------------------------------------------------------------------------------------------------
// Digitaal / Analoog
static s_disp_tekst       const disp_type_sturing_str = { Disp_Draw_Tekst_L,       37, 22, &tekst_inst.Type_sturing_14 };
static s_disp_tekst_array const disp_type_sturing_val = { Disp_Draw_Tekst_Array_L, 76, 75, &tekst_digitaal_analoog_14, UCHAR, &opt_app.Motorgroup[0].AnalogInput, 2 };

static void * const lcd_type_sturing_disp[] = { &disp_group_ico, &disp_type_sturing_str, &disp_type_sturing_val, 0 };

static s_key_value const key_type_sturing_val = { UCHAR, 1, &opt_app.Motorgroup[0].AnalogInput, &uchar_0, &uchar_1 };
//-----------------------------------------------------------------------------------------------------------
// Digitale ingang aan/uit
static s_disp_tekst const disp_aan_uit_str = { Disp_Draw_Tekst_L, 47, 22, &tekst_inst.Aan_uit_14 };
static s_disp_board_IO_Selection const disp_dig_in_vent_aan_uit_sel = { Disp_Draw_Board_IO_Select, &opt_app.Ventgroup[0].DigInOnOff, 1, (unsigned char *)&uchar_1, 1, DIGITAL_INPUT_ID, DIG_IN_VENT, DigInNotUsed };

static void * const lcd_dig_in_aan_uit[] = { &disp_group_ico, &disp_aan_uit_str, &disp_dig_in_vent_aan_uit_sel, 0 };
//-----------------------------------------------------------------------------------------------------------
// Digitale ingang open
static void const * const tekst_open_14[] = { &tekst_inst.Open_14, &tekst_inst.Open_14, &tekst_inst.Hoger_14 };
static s_disp_tekst_array const disp_open_str  = { Disp_Draw_Tekst_Array_L, 47, 22, &tekst_open_14, UCHAR, &opt_app.Motorgroup[0].Type, 3 };
static s_disp_board_IO_Selection_option_on const disp_dig_in_raam_open_sel = { Disp_Draw_Board_IO_Select_Option_On, &opt_app.Motorgroup[0].DigInOpen, 1, (unsigned char *)&uchar_1, 1, DIGITAL_INPUT_ID, DIG_IN_RAAM, DigInNotUsed, UCHAR, &Motorgroup[0].TypeRaam };
static s_disp_board_IO_Selection_option_on const disp_dig_in_doek_open_sel = { Disp_Draw_Board_IO_Select_Option_On, &opt_app.Motorgroup[0].DigInOpen, 1, (unsigned char *)&uchar_1, 1, DIGITAL_INPUT_ID, DIG_IN_DOEK, DigInNotUsed, UCHAR, &Motorgroup[0].TypeDoek };
static s_disp_board_IO_Selection_option_on const disp_dig_in_vent_open_sel = { Disp_Draw_Board_IO_Select_Option_On, &opt_app.Motorgroup[0].DigInOpen, 1, (unsigned char *)&uchar_1, 1, DIGITAL_INPUT_ID, DIG_IN_VENT, DigInNotUsed, UCHAR, &Motorgroup[0].TypeVent };
static s_disp_board_IO_Selection_option_on const disp_dig_in_klep_open_sel = { Disp_Draw_Board_IO_Select_Option_On, &opt_app.Motorgroup[0].DigInOpen, 1, (unsigned char *)&uchar_1, 1, DIGITAL_INPUT_ID, DIG_IN_KLEP, DigInNotUsed, UCHAR, &Motorgroup[0].TypeKlep };

static void * const lcd_dig_in_open[] = { &disp_group_ico, &disp_open_str, &disp_dig_in_raam_open_sel, &disp_dig_in_doek_open_sel, &disp_dig_in_vent_open_sel, &disp_dig_in_klep_open_sel, 0 };
//-----------------------------------------------------------------------------------------------------------
// Digitale ingang dicht
static void const * const tekst_dicht_14[] = { &tekst_inst.Dicht_14, &tekst_inst.Dicht_14, &tekst_inst.Lager_14 };
static s_disp_tekst_array const disp_dicht_str  = { Disp_Draw_Tekst_Array_L, 47, 22, &tekst_dicht_14, UCHAR, &opt_app.Motorgroup[0].Type, 3 };
static s_disp_board_IO_Selection_option_on const disp_dig_in_raam_dicht_sel = { Disp_Draw_Board_IO_Select_Option_On, &opt_app.Motorgroup[0].DigInClose, 1, (unsigned char *)&uchar_1, 1, DIGITAL_INPUT_ID, DIG_IN_RAAM, DigInNotUsed, UCHAR, &Motorgroup[0].TypeRaam };
static s_disp_board_IO_Selection_option_on const disp_dig_in_doek_dicht_sel = { Disp_Draw_Board_IO_Select_Option_On, &opt_app.Motorgroup[0].DigInClose, 1, (unsigned char *)&uchar_1, 1, DIGITAL_INPUT_ID, DIG_IN_DOEK, DigInNotUsed, UCHAR, &Motorgroup[0].TypeDoek };
static s_disp_board_IO_Selection_option_on const disp_dig_in_vent_dicht_sel = { Disp_Draw_Board_IO_Select_Option_On, &opt_app.Motorgroup[0].DigInClose, 1, (unsigned char *)&uchar_1, 1, DIGITAL_INPUT_ID, DIG_IN_VENT, DigInNotUsed, UCHAR, &Motorgroup[0].TypeVent };
static s_disp_board_IO_Selection_option_on const disp_dig_in_klep_dicht_sel = { Disp_Draw_Board_IO_Select_Option_On, &opt_app.Motorgroup[0].DigInClose, 1, (unsigned char *)&uchar_1, 1, DIGITAL_INPUT_ID, DIG_IN_KLEP, DigInNotUsed, UCHAR, &Motorgroup[0].TypeKlep };

static void * const lcd_dig_in_dicht[] = { &disp_group_ico, &disp_dicht_str, &disp_dig_in_raam_dicht_sel, &disp_dig_in_doek_dicht_sel, &disp_dig_in_vent_dicht_sel, &disp_dig_in_klep_dicht_sel, 0 };
//-----------------------------------------------------------------------------------------------------------
// Analoge ingang positie
static s_disp_tekst const disp_position_str = { Disp_Draw_Tekst_L, 47, 22, &tekst_inst.Position_14 };
static s_disp_board_IO_Selection_option_on const disp_ana_in_raam_pos_sel = { Disp_Draw_Board_IO_Select_Option_On, &opt_app.Motorgroup[0].AnaInPosition, 1, (unsigned char *)&uchar_1, 1, ANALOG_INPUT_ID, ANA_IN_RAAM, AnaInNotUsed, UCHAR, &Motorgroup[0].TypeRaam };
static s_disp_board_IO_Selection_option_on const disp_ana_in_doek_pos_sel = { Disp_Draw_Board_IO_Select_Option_On, &opt_app.Motorgroup[0].AnaInPosition, 1, (unsigned char *)&uchar_1, 1, ANALOG_INPUT_ID, ANA_IN_DOEK, AnaInNotUsed, UCHAR, &Motorgroup[0].TypeDoek };
static s_disp_board_IO_Selection_option_on const disp_ana_in_vent_pos_sel = { Disp_Draw_Board_IO_Select_Option_On, &opt_app.Motorgroup[0].AnaInPosition, 1, (unsigned char *)&uchar_1, 1, ANALOG_INPUT_ID, ANA_IN_VENT, AnaInNotUsed, UCHAR, &Motorgroup[0].TypeVent };
static s_disp_board_IO_Selection_option_on const disp_ana_in_klep_pos_sel = { Disp_Draw_Board_IO_Select_Option_On, &opt_app.Motorgroup[0].AnaInPosition, 1, (unsigned char *)&uchar_1, 1, ANALOG_INPUT_ID, ANA_IN_KLEP, AnaInNotUsed, UCHAR, &Motorgroup[0].TypeKlep };

static void * const lcd_ana_in_position[] = { &disp_group_ico, &disp_position_str, &disp_ana_in_raam_pos_sel, &disp_ana_in_doek_pos_sel, &disp_ana_in_vent_pos_sel, &disp_ana_in_klep_pos_sel, 0 };
//-----------------------------------------------------------------------------
// Analoge uitgang terugmelding
static s_disp_tekst const disp_terugmelding_str = { Disp_Draw_Tekst_L, 47, 22, &tekst_inst.Terugmelding_14 };
static s_disp_board_IO_Selection_option_on const disp_ana_out_raam_sel = { Disp_Draw_Board_IO_Select_Option_On, &opt_app.Motorgroup[0].AnaOutPosition, 1, (unsigned char *)&uchar_1, 1, ANALOG_OUTPUT_ID, ANA_OUT_RAAM, AnaOutNotUsed, UCHAR, &Motorgroup[0].TypeRaam };
static s_disp_board_IO_Selection_option_on const disp_ana_out_doek_sel = { Disp_Draw_Board_IO_Select_Option_On, &opt_app.Motorgroup[0].AnaOutPosition, 1, (unsigned char *)&uchar_1, 1, ANALOG_OUTPUT_ID, ANA_OUT_DOEK, AnaOutNotUsed, UCHAR, &Motorgroup[0].TypeDoek };
static s_disp_board_IO_Selection_option_on const disp_ana_out_vent_sel = { Disp_Draw_Board_IO_Select_Option_On, &opt_app.Motorgroup[0].AnaOutPosition, 1, (unsigned char *)&uchar_1, 1, ANALOG_OUTPUT_ID, ANA_OUT_VENT, AnaOutNotUsed, UCHAR, &Motorgroup[0].TypeVent };
static s_disp_board_IO_Selection_option_on const disp_ana_out_klep_sel = { Disp_Draw_Board_IO_Select_Option_On, &opt_app.Motorgroup[0].AnaOutPosition, 1, (unsigned char *)&uchar_1, 1, ANALOG_OUTPUT_ID, ANA_OUT_KLEP, AnaOutNotUsed, UCHAR, &Motorgroup[0].TypeKlep };

static void * const lcd_ana_out_terugmelding[] = { &disp_group_ico, &disp_terugmelding_str, &disp_ana_out_raam_sel, &disp_ana_out_doek_sel, &disp_ana_out_vent_sel, &disp_ana_out_klep_sel, 0 };
//-----------------------------------------------------------------------------
// Stuurtijd
static s_disp_tekst const disp_stuurtijd_str = { Disp_Draw_Tekst_L,  37, 22, &tekst_inst.Looptijd_14 };
static s_disp_value const disp_stuurtijd_val = { Disp_Draw_Value,   207, 75, (SIZE_14 | RECHTS), INT, 1, &opt_app.Motorgroup[0].Runtime };
static s_disp_tekst const disp_stuurtijd_sec = { Disp_Draw_Tekst_L, 210, 75, &tekst.s_10 };

static void * const lcd_stuurtijd_disp[] = { &disp_group_ico, &disp_stuurtijd_str, &disp_stuurtijd_val, &disp_stuurtijd_sec, 0 };

static s_key_value const key_stuurtijd_val = { INT, 4, &opt_app.Motorgroup[0].Runtime, &int_0, &int_6000 };
//-----------------------------------------------------------------------------
// Aantal motoren
static s_disp_tekst const disp_aantal_motoren_str   = { Disp_Draw_Tekst_L, 37, 22, &tekst_inst.Aantal_motoren_14 };
static s_disp_value const disp_aantal_motoren_val   = { Disp_Draw_Value,  207, 75, (SIZE_14 | RECHTS), UCHAR, 0, &opt_app.Motorgroup[0].NumberMotors };
static s_disp_tekst const disp_nummer_msg_str       = { Disp_Draw_Tekst_L, 88, 62, &tekst_inst.Nummer_7 };
static s_disp_tekst const disp_niet_beschikbaar_str = { Disp_Draw_Tekst_L, 88, 77, &tekst_inst.niet_beschikbaar_7 };

static void * const lcd_aantal_motoren_disp[]   = { &disp_motor_ico, &disp_aantal_motoren_str, &disp_aantal_motoren_val, 0 };
static void * const lcd_niet_beschikbaar_disp[] = { &disp_motor_ico, &disp_aantal_motoren_str, &disp_aantal_motoren_val, &disp_messagebox_error, &disp_nummer_msg_str, &disp_niet_beschikbaar_str, 0 };

static s_key_value const key_aantal_motoren_val = { UCHAR, 3, &opt_app.Motorgroup[0].NumberMotors, &uchar_0, &MaxMotor };
//-----------------------------------------------------------------------------------------------------------
// Type Bus - ebmBus / ModBus
static void const * const tekst_type_ventilatoren_14[] = { &tekst_ebmBus_14, &tekst_ECblue_Premium_14, &tekst_ebm_Modbus_14, &tekst_ECblue_Modbus_14, &tekst_leeg, &tekst_leeg, &tekst_Rosenberg_14, &tekst_Climafan_14, &tekst_Rosenberg_Gen3_14, &tekst_Nicotra_Gebhardt_14 };
static s_disp_tekst       const disp_type_vent_str = { Disp_Draw_Tekst_L,       37, 22, &tekst_inst.Type_14 };
static s_disp_tekst_array const disp_type_vent_val = { Disp_Draw_Tekst_Array_L, 46, 75, &tekst_type_ventilatoren_14, UCHAR, &opt_app.Motorgroup[0].BusType, TYPE_STURING_COUNT };

static void * const lcd_type_vent_disp[] = { &disp_group_ico, &disp_type_vent_str, &disp_type_vent_val, 0 };

static s_key_value const key_type_vent_val = { UCHAR, 1, &opt_app.Motorgroup[0].BusType, &uchar_0, &uchar_7 };
//-----------------------------------------------------------------------------------------------------------
// Operation Mode - ClosedLoop / OpenLoop
static void const * const tekst_closed_loop_open_loop_14[] = { &tekst_inst.Closed_Loop_14, &tekst_inst.Open_Loop_14 };
static s_disp_tekst       const disp_operation_mode_str = { Disp_Draw_Tekst_L,       37, 22, &tekst_inst.Operation_Mode_14 };
static s_disp_tekst_array const disp_operation_mode_val = { Disp_Draw_Tekst_Array_L, 46, 75, &tekst_closed_loop_open_loop_14, UCHAR, &opt_app.Motorgroup[0].ClosedLoopOpenLoop, 2 };

static void * const lcd_operation_mode_disp[] = { &disp_group_ico, &disp_operation_mode_str, &disp_operation_mode_val, 0 };

static s_key_value const key_operation_mode_val = { UCHAR, 1, &opt_app.Motorgroup[0].ClosedLoopOpenLoop, &uchar_0, &uchar_1 };
//-----------------------------------------------------------------------------------------------------------
// Watchdog Mode
static s_disp_tekst const disp_watchdog_mode_str = { Disp_Draw_Tekst_L, 37, 22, &tekst_inst.Watchdog_Mode_14 };
static s_disp_bitmap_array const disp_watchdog_mode_val = { Disp_Draw_Bitmap_Array, 190, 57, bmp_false_true, UCHAR, &opt_app.Motorgroup[0].WatchdogMode, 2 };
static s_key_value const key_watchdog_mode_val = { UCHAR, 2, &opt_app.Motorgroup[0].WatchdogMode, &uchar_0, &uchar_1 };

static void * const lcd_watchdog_mode_disp[] = { &disp_group_ico, &disp_watchdog_mode_str, &disp_watchdog_mode_val, 0 };
//-----------------------------------------------------------------------------------------------------------
// Position at communication failure
static s_disp_tekst const disp_watchdog_position_str = { Disp_Draw_Tekst_L,  37, 22, &tekst_inst.Watchdog_Position_14 };
static s_disp_value const disp_watchdog_position_val = { Disp_Draw_Value,   207, 75, (SIZE_14 | RECHTS), UCHAR, 0, &opt_app.Motorgroup[0].PositionAtCommunicationFailure };
static s_disp_tekst const disp_watchdog_position_prc = { Disp_Draw_Tekst_L, 210, 68, &tekst_inst.perc_7 };
static s_key_value const key_watchdog_position_val = { UCHAR, 3, &opt_app.Motorgroup[0].PositionAtCommunicationFailure, &uchar_0, &uchar_100 };

static void * const lcd_watchdog_position_disp[] = { &disp_group_ico, &disp_watchdog_position_str, &disp_watchdog_position_val, &disp_watchdog_position_prc, 0 };
//-----------------------------------------------------------------------------------------------------------
// RampUp / RampDown
static s_disp_tekst const disp_ramp_up_down_str = { Disp_Draw_Tekst_L,  37, 22, &tekst_inst.RampUpDown_14 };
static s_disp_tekst const disp_ramp_up_str      = { Disp_Draw_Tekst_L,  56, 49, &tekst_inst.RampUp_14     };
static s_disp_tekst const disp_ramp_down_str    = { Disp_Draw_Tekst_L,  56, 75, &tekst_inst.RampDown_14   };
static s_disp_value const disp_ramp_up_val      = { Disp_Draw_Value,   207, 49, (SIZE_14 | RECHTS), UCHAR, 0, &opt_app.Motorgroup[0].RampUp   };
static s_disp_value const disp_ramp_down_val    = { Disp_Draw_Value,   207, 75, (SIZE_14 | RECHTS), UCHAR, 0, &opt_app.Motorgroup[0].RampDown };
static s_disp_tekst const disp_ramp_up_sec      = { Disp_Draw_Tekst_L, 210, 49, &tekst.s_10 };
static s_disp_tekst const disp_ramp_down_sec    = { Disp_Draw_Tekst_L, 210, 75, &tekst.s_10 };

static void * const lcd_ramp_up_down_disp[] =
{
  &disp_group_ico, &disp_ramp_up_down_str,
  &disp_ramp_up_str,   &disp_ramp_up_val,   &disp_ramp_up_sec,
  &disp_ramp_down_str, &disp_ramp_down_val, &disp_ramp_down_sec,
  0
};

static s_key_value const key_ramp_up_val   = { UCHAR, 3, &opt_app.Motorgroup[0].RampUp,   &uchar_0, &uchar_255 };
static s_key_value const key_ramp_down_val = { UCHAR, 3, &opt_app.Motorgroup[0].RampDown, &uchar_0, &uchar_255 };
//-----------------------------------------------------------------------------------------------------------
// VentAtMax
static s_disp_tekst const disp_vent_at_max_str = { Disp_Draw_Tekst_L,  37, 22, &tekst_inst.Vent_At_Max_14 };
static s_disp_value const disp_vent_at_max_val = { Disp_Draw_Value,   207, 75, (SIZE_14 | RECHTS), UCHAR, 0, &opt_app.Motorgroup[0].VentAtMax };
static s_disp_tekst const disp_vent_at_max_uni = { Disp_Draw_Tekst_L, 210, 68, &tekst_inst.perc_7 };

static void * const lcd_vent_at_max_disp[] = { &disp_group_ico, &disp_vent_at_max_str, &disp_vent_at_max_val, &disp_vent_at_max_uni, 0 };

static s_key_value const key_vent_at_max_val = { UCHAR, 3, &opt_app.Motorgroup[0].VentAtMax, &uchar_0, &uchar_100 };
//-----------------------------------------------------------------------------------------------------------
// PowerFactor
static s_disp_tekst const disp_power_factor_str = { Disp_Draw_Tekst_L,  37, 22, &tekst_inst.PowerFactor_14 };
static s_disp_value const disp_power_factor_val = { Disp_Draw_Value,   207, 75, (SIZE_14 | RECHTS), UCHAR, 2, &opt_app.Motorgroup[0].PowerFactor };

static void * const lcd_power_factor_disp[] = { &disp_group_ico, &disp_power_factor_str, &disp_power_factor_val, 0 };

static s_key_value const key_power_factor_val = { UCHAR, 3, &opt_app.Motorgroup[0].PowerFactor, &uchar_0, &uchar_255 };
//-----------------------------------------------------------------------------------------------------------
// SensorType
static void const * const tekst_sensor_type_14[] = { &tekst_inst.Geen_14, &tekst_inst.Eindschakelaar_14, &tekst_inst.Extern_alarm_14 };
static s_disp_tekst       const disp_sensor_type_str = { Disp_Draw_Tekst_L,       37, 22, &tekst_inst.SensorType_14 };
static s_disp_tekst_array const disp_sensor_type_val = { Disp_Draw_Tekst_Array_L, 46, 75, &tekst_sensor_type_14, UCHAR, &opt_app.Motorgroup[0].SensorType, 3 };

static void * const lcd_sensor_type_disp[] = { &disp_group_ico, &disp_sensor_type_str, &disp_sensor_type_val, 0 };

static s_key_value const key_sensor_type_val = { UCHAR, 1, &opt_app.Motorgroup[0].SensorType, &uchar_0, &uchar_2 };
//-----------------------------------------------------------------------------
// IO motoren (H2MC's / Ventilatoren)
static s_disp_tekst const disp_motoren_str = { Disp_Draw_Tekst_L, 47, 22, &tekst_inst.Motoren_14 };
static s_disp_board_IO_Selection           const disp_vent_485_sel   = { Disp_Draw_Board_IO_Select, opt_app.Ventgroup[0].RS485Bus, 4, (unsigned char *)&uchar_4, 1, RS485_BUS_ID, 0, DummyNotUsed };
static s_disp_board_IO_Selection_option_on const disp_motor_raam_sel = { Disp_Draw_Board_Motor_IO_Select_Option_On, MotorIO, 16, &opt_app.Motorgroup[0].NumberMotors, 0, MOTOR_CONTROL_ID, MOTOR_CONTROL_RAAM, MotorControlNotUsed, UCHAR, &Motorgroup[0].TypeRaam };
static s_disp_board_IO_Selection_option_on const disp_motor_doek_sel = { Disp_Draw_Board_Motor_IO_Select_Option_On, MotorIO, 16, &opt_app.Motorgroup[0].NumberMotors, 0, MOTOR_CONTROL_ID, MOTOR_CONTROL_DOEK, MotorControlNotUsed, UCHAR, &Motorgroup[0].TypeDoek };

static void * const lcd_ventilatoren[] = { &disp_motor_ico, &disp_motoren_str, &disp_vent_485_sel, 0 };
static void * const lcd_motoren[]      = { &disp_motor_ico, &disp_motoren_str, &disp_motor_raam_sel, &disp_motor_doek_sel, 0 };
//-----------------------------------------------------------------------------
// Adressen (ventilatoren)
static int FirstNumber,  LastNumber;
static int FirstAddress, LastAddress;

static void AddressRangeFunc(void)
{
  LastNumber  = opt_app.Motorgroup[GroupIndex].FirstNumber  + opt_app.Motorgroup[GroupIndex].NumberMotors - 1;
  LastAddress = opt_app.Motorgroup[GroupIndex].FirstAddress + opt_app.Motorgroup[GroupIndex].NumberMotors - 1;
}

static s_tekst_6 const tekst_tm = { 4, 50, SIZE_14, "..." };
static s_disp_func  const disp_adressen_func      = { Disp_Control_Func, AddressRangeFunc };
static s_disp_tekst const disp_adressen_str       = { Disp_Draw_Tekst_L,  37, 22, &tekst_inst.Adressen_14    };
static s_disp_tekst const disp_nummer_str         = { Disp_Draw_Tekst_L,  14, 49, &tekst_inst.Nummer_10 };
static s_disp_tekst const disp_adres_str          = { Disp_Draw_Tekst_L,  14, 75, &tekst_inst.Adres_10  };
static s_disp_tekst const disp_tm_nummer_str      = { Disp_Draw_Tekst_L, 163, 49, &tekst_tm };
static s_disp_tekst const disp_tm_adres_str       = { Disp_Draw_Tekst_L, 163, 75, &tekst_tm };
static s_disp_value const disp_eerste_nummer_val  = { Disp_Draw_Value,   153, 49, (SIZE_14 | RECHTS), UCHAR, 0, &opt_app.Motorgroup[0].FirstNumber  };
static s_disp_value const disp_laatste_nummer_val = { Disp_Draw_Value,   207, 49, (SIZE_14 | RECHTS), INT,   0, &LastNumber                         };
static s_disp_value const disp_eerste_adres_val   = { Disp_Draw_Value,   153, 75, (SIZE_14 | RECHTS), UCHAR, 0, &opt_app.Motorgroup[0].FirstAddress };
static s_disp_value const disp_laatste_adres_val  = { Disp_Draw_Value,   207, 75, (SIZE_14 | RECHTS), INT,   0, &LastAddress                        };

static void * const lcd_adressen_disp[] =
{
  &disp_group_ico,  &disp_adressen_func, &disp_adressen_str, 
  &disp_nummer_str, &disp_dp_14_L, &disp_eerste_nummer_val, &disp_tm_nummer_str, &disp_laatste_nummer_val,
  &disp_adres_str,  &disp_dp_14_L, &disp_eerste_adres_val,  &disp_tm_adres_str,  &disp_laatste_adres_val,
  0
};

static s_key_value const key_eerste_nummer_val = { UCHAR, 3, &opt_app.Motorgroup[0].FirstNumber,  &uchar_1, &uchar_255 };
static s_key_value const key_eerste_adres_val  = { UCHAR, 3, &opt_app.Motorgroup[0].FirstAddress, &uchar_1, &uchar_255 };
//-----------------------------------------------------------------------------
// Wijzig adres
static s_disp_tekst            const disp_wijzig_adres_str     = { Disp_Draw_Tekst_L,            37, 22, &tekst_inst.Wijzig_adres_14 };
static s_disp_tekst_option_on  const disp_wijzigen_gelukt_str  = { Disp_Draw_Tekst_L_Option_On,  37, 42, &tekst_inst.gewijzigd_10,         UCHAR, &AdresWijzigenOk      };
static s_disp_tekst_option_on  const disp_wijzigen_mislukt_str = { Disp_Draw_Tekst_L_Option_On,  37, 42, &tekst_inst.communicatie_fout_10, UCHAR, &AdresWijzigenMislukt };
static s_disp_value            const disp_oud_adres_val        = { Disp_Draw_Value,             147, 75, (SIZE_14 | RECHTS), UINT,  0, &OudAdres   };
static s_disp_value            const disp_nieuw_adres_val      = { Disp_Draw_Value,             207, 75, (SIZE_14 | RECHTS), UINT,  0, &NieuwAdres };
static s_disp_bitmap           const disp_pijl_rechts_ico      = { Disp_Draw_Bitmap,            157, 62, &ico_pijl_rechts };
static s_disp_bitmap_option_on const disp_wijzigen_status_bmp  = { Disp_Draw_Bitmap_Option_On,   90, 40, &zandloper_ico, UCHAR, &AdresWijzigenStatus };

static void * const lcd_wijzig_adres_disp[]    = { &disp_motor_ico, &disp_wijzig_adres_str, &disp_oud_adres_val, &disp_pijl_rechts_ico, &disp_nieuw_adres_val,
                                                   &disp_wijzigen_gelukt_str, &disp_wijzigen_mislukt_str, &disp_wijzigen_status_bmp, 0 };
static void * const lcd_wijzig_adres_ok_disp[] = { &disp_motor_ico, &disp_wijzig_adres_str, &disp_oud_adres_val, &disp_pijl_rechts_ico, &disp_nieuw_adres_val, &disp_messagebox_bevestig,
                                                   &disp_zeker_weten_inst_str, &disp_ok_str, &disp_space_10_L, &disp_is_teken_10_L, &disp_space_10_L, &disp_wijzigen_str, 0 };

static s_key_value const key_oud_adres_val   = { UINT, 3, &OudAdres,   &uint_1, &MaxAdres };
static s_key_value const key_nieuw_adres_val = { UINT, 3, &NieuwAdres, &uint_1, &MaxAdres };
//-----------------------------------------------------------------------------
// Frequentie gestuurd
static s_disp_tekst const disp_frequentie_gestuurd_str = { Disp_Draw_Tekst_L, 37, 22, &tekst_inst.Frequentie_gestuurd_14 };
static s_disp_bitmap_array const disp_frequentie_gestuurd_val = { Disp_Draw_Bitmap_Array, 190, 57, bmp_false_true, UCHAR, &opt_app.Motorgroup[0].FrequencyControlled, 2 };

static void * const lcd_frequentie_gestuurd_disp[] = { &disp_group_ico, &disp_frequentie_gestuurd_str, &disp_frequentie_gestuurd_val, 0 };

static s_key_value const key_frequentie_gestuurd_val = { UCHAR, 1, &opt_app.Motorgroup[0].FrequencyControlled, &uchar_0, &uchar_1 };
//-----------------------------------------------------------------------------
// Frequentie gestuurd verstel
static s_disp_tekst const disp_frequentie_verstel_str  = { Disp_Draw_Tekst_L,  37, 22, &tekst_inst.Frequentie_verstel_14 };
static s_disp_value const disp_frequentie_verstel_val  = { Disp_Draw_Value,   192, 75, (SIZE_14 | RECHTS), INT, 1, &opt_app.Motorgroup[0].FrequentieVerstel };
static s_disp_tekst const disp_frequentie_verstel_perc = { Disp_Draw_Tekst_L, 195, 68, &tekst.perc_7 };

static void * const lcd_frequentie_verstel_disp[] =
{
  &disp_group_ico, &disp_frequentie_verstel_str,
  &disp_frequentie_verstel_val, &disp_frequentie_verstel_perc, 0
};

static s_key_value const key_frequentie_verstel_val = { INT, 4, &opt_app.Motorgroup[0].FrequentieVerstel, &opt_app.Motorgroup[0].PositionLowSpeed, &int_1000 };
//-----------------------------------------------------------------------------
// Digitale ingang hoge snelheid
static s_disp_tekst const disp_hoge_snelheid_str = { Disp_Draw_Tekst_L, 47, 22, &tekst_inst.Hoge_snelheid_14 };
static s_disp_board_IO_Selection_option_on const disp_dig_in_raam_hoge_snelheid_sel = { Disp_Draw_Board_IO_Select_Option_Off, &opt_app.Motorgroup[0].DigInHiSpeed, 1, (unsigned char *)&uchar_1, 1, DIGITAL_INPUT_ID, DIG_IN_RAAM, DigInHiSpeedNotUsed, UCHAR, &opt_app.Motorgroup[0].Type };
static s_disp_board_IO_Selection_option_on const disp_dig_in_doek_hoge_snelheid_sel = { Disp_Draw_Board_IO_Select_Option_On,  &opt_app.Motorgroup[0].DigInHiSpeed, 1, (unsigned char *)&uchar_1, 1, DIGITAL_INPUT_ID, DIG_IN_DOEK, DigInHiSpeedNotUsed, UCHAR, &opt_app.Motorgroup[0].Type };

static void * const lcd_dig_in_hoge_snelheid[] = { &disp_group_ico, &disp_hoge_snelheid_str, &disp_dig_in_raam_hoge_snelheid_sel, &disp_dig_in_doek_hoge_snelheid_sel, 0 };
//-----------------------------------------------------------------------------
// Uitgang snelheid analoog/digitaal
static s_disp_tekst       const disp_uitgang_snelheid_str = { Disp_Draw_Tekst_L,       37, 22, &tekst_inst.Uitgang_snelheid_14 };
static s_disp_tekst_array const disp_digitaal_analoog_str = { Disp_Draw_Tekst_Array_L, 76, 75, &tekst_digitaal_analoog_14, UCHAR, &opt_app.Motorgroup[0].AnalogSpeed, 2 };

static void * const lcd_uitgang_snelheid_disp[] = { &disp_group_ico, &disp_uitgang_snelheid_str, &disp_digitaal_analoog_str, 0 };

static s_key_value const key_uitgang_snelheid_val = { UCHAR, 1, &opt_app.Motorgroup[0].AnalogSpeed, &uchar_0, &uchar_1 };
//-----------------------------------------------------------------------------
// Snelheid laag/hoog
static s_disp_tekst const disp_snelheid_str       = { Disp_Draw_Tekst_L,  37, 22, &tekst_inst.Snelheid_14 };
static s_disp_tekst const disp_snelheid_laag_str  = { Disp_Draw_Tekst_L,  56, 49, &tekst_inst.Laag_14 };
static s_disp_value const disp_snelheid_laag_val  = { Disp_Draw_Value,   192, 49, (SIZE_14 | RECHTS), UCHAR, 0, &opt_app.Motorgroup[0].SpeedLow };
static s_disp_tekst const disp_snelheid_laag_perc = { Disp_Draw_Tekst_L, 195, 42, &tekst.perc_7 };
static s_disp_tekst const disp_snelheid_hoog_str  = { Disp_Draw_Tekst_L,  56, 75, &tekst_inst.Hoog_14 };
static s_disp_value const disp_snelheid_hoog_val  = { Disp_Draw_Value,   192, 75, (SIZE_14 | RECHTS), UCHAR, 0, &opt_app.Motorgroup[0].SpeedHi };
static s_disp_tekst const disp_snelheid_hoog_perc = { Disp_Draw_Tekst_L, 195, 68, &tekst.perc_7 };

static void * const lcd_snelheid_disp[] =
{
  &disp_group_ico, &disp_snelheid_str,
  &disp_snelheid_laag_str, &disp_snelheid_laag_val, &disp_snelheid_laag_perc,
  &disp_snelheid_hoog_str, &disp_snelheid_hoog_val, &disp_snelheid_hoog_perc, 0
};

static s_key_value const key_snelheid_laag_val = { UCHAR, 3, &opt_app.Motorgroup[0].SpeedLow, &uchar_0, &opt_app.Motorgroup[0].SpeedHi };
static s_key_value const key_snelheid_hoog_val = { UCHAR, 3, &opt_app.Motorgroup[0].SpeedHi,  &opt_app.Motorgroup[0].SpeedLow, &uchar_100 };
//-----------------------------------------------------------------------------
// Terugschakel positie open/dicht
static s_disp_tekst const disp_terugschakel_positie_str  = { Disp_Draw_Tekst_L,  37, 22, &tekst_inst.Terugschakel_positie_14 };
static s_disp_value const disp_terugschakel_positie_val  = { Disp_Draw_Value,   192, 75, (SIZE_14 | RECHTS), INT, 1, &opt_app.Motorgroup[0].PositionLowSpeed };
static s_disp_tekst const disp_terugschakel_positie_perc = { Disp_Draw_Tekst_L, 195, 68, &tekst.perc_7 };

static void * const lcd_terugschakel_positie_disp[] =
{
  &disp_group_ico, &disp_terugschakel_positie_str,
  &disp_terugschakel_positie_val, &disp_terugschakel_positie_perc, 0
};

static s_key_value const key_terugschakel_positie_val = { INT, 4, &opt_app.Motorgroup[0].PositionLowSpeed, &int_50, &int_950 };
//-----------------------------------------------------------------------------------------------------------
// Position
//static s_disp_tekst const disp_position_str = { Disp_Draw_Tekst_L,  37, 22, &tekst_inst.Position_14 };
static s_disp_value const disp_position_val = { Disp_Draw_Value,   207, 75, (SIZE_14 | RECHTS), INT, 1, &val_hr_alg.Motorgroup[0].PositionPerc };
static s_disp_tekst const disp_14_perc      = { Disp_Draw_Tekst_L, 210, 68, &tekst.perc_7 };

static void * const lcd_position_disp[] = { &disp_group_ico, &disp_position_str, &disp_position_val, &disp_14_perc, 0 };

static s_key_value const key_position_val = { INT, 4, &val_hr_alg.Motorgroup[0].PositionPerc, &int_0, &int_1000 };
//-----------------------------------------------------------------------------
// Pulse system
static s_disp_tekst        const disp_pulse_system_str = { Disp_Draw_Tekst_L, 37, 22, &tekst_inst.Pulse_system_14 };
static s_disp_bitmap_array const disp_pulse_system_val = { Disp_Draw_Bitmap_Array, 190, 57, bmp_false_true, UCHAR, &opt_app.Motorgroup[0].PulseSystem, 2 };

static void * const lcd_pulse_system_disp[] = { &disp_pulse_system_ico, &disp_pulse_system_str, &disp_pulse_system_val, 0 };

static s_key_value const key_pulse_system_val = { UCHAR, 1, &opt_app.Motorgroup[0].PulseSystem, &uchar_0, &uchar_1 };
//-----------------------------------------------------------------------------
// Kier regeling
static s_disp_tekst        const disp_kier_regeling_str = { Disp_Draw_Tekst_L, 37, 22, &tekst_inst.Kier_regeling_14 };
static s_disp_bitmap_array const disp_kier_regeling_val = { Disp_Draw_Bitmap_Array, 190, 57, bmp_false_true, UCHAR, &opt_app.Motorgroup[0].KierRegeling, 2 };

static void * const lcd_kier_regeling_disp[] = { &disp_kierregeling_ico, &disp_kier_regeling_str, &disp_kier_regeling_val, 0 };

static s_key_value const key_kier_regeling_val = { UCHAR, 1, &opt_app.Motorgroup[0].KierRegeling, &uchar_0, &uchar_1 };
//-----------------------------------------------------------------------------
// Digitale uitgang alarm
static s_disp_bitmap const disp_alarm_ico = { Disp_Draw_Bitmap,   9, 10, &ico_alarm };
static s_disp_tekst  const disp_alarm_str = { Disp_Draw_Tekst_L, 47, 22, &tekst_inst.Alarm_Contact_14 };
static s_disp_board_IO_Selection const disp_dig_out_alarm_sel = { Disp_Draw_Board_IO_Select, &opt_app.Motorgroup[0].DigOutAlarm, 1, (unsigned char *)&uchar_1, 1, DIGITAL_OUTPUT_ID, DIG_OUT_ALARM, DigOutNotUsed };

static void * const lcd_dig_out_alarm[] = { &disp_alarm_ico, &disp_alarm_str, &disp_dig_out_alarm_sel, 0 };
//-----------------------------------------------------------------------------
// Digitale uitgang urgent alarm
static s_disp_tekst  const disp_alarm_urgent_str = { Disp_Draw_Tekst_L, 47, 22, &tekst_inst.Alarm_urgent_14 };
static s_disp_board_IO_Selection const disp_dig_out_alarm_urgent_sel = { Disp_Draw_Board_IO_Select, &opt_app.Ventgroup[0].DigOutAlarmUrgent, 1, (unsigned char *)&uchar_1, 1, DIGITAL_OUTPUT_ID, DIG_OUT_ALARM, DigOutNotUsed };

static void * const lcd_dig_out_alarm_urgent[] = { &disp_alarm_ico, &disp_alarm_urgent_str, &disp_dig_out_alarm_urgent_sel, 0 };
//-----------------------------------------------------------------------------
// Digitale uitgang eindschakelaar alarm
static void const * const tekst_alarm_type_14[] = { &tekst_inst.Geen_14, &tekst_inst.Alarm_eindschakelaar_14, &tekst_inst.Extern_alarm_14 };
static s_disp_tekst_array const disp_alarm_type_str = { Disp_Draw_Tekst_Array_L, 47, 22, &tekst_alarm_type_14, UCHAR, &opt_app.Motorgroup[0].SensorType, 3 };
//static s_disp_tekst  const disp_alarm_eindschakelaar_str = { Disp_Draw_Tekst_L, 47, 22, &tekst_inst.Alarm_eindschakelaar_14 };
static s_disp_board_IO_Selection const disp_dig_out_alarm_eindschakelaar_sel = { Disp_Draw_Board_IO_Select, &opt_app.Motorgroup[0].DigOutAlarmFlap, 1, (unsigned char *)&uchar_1, 1, DIGITAL_OUTPUT_ID, DIG_OUT_ALARM, DigOutNotUsed };

static void * const lcd_dig_out_alarm_eindschakelaar[] = { &disp_alarm_ico, &disp_alarm_type_str, /*&disp_alarm_eindschakelaar_str,*/ &disp_dig_out_alarm_eindschakelaar_sel, 0 };
//-----------------------------------------------------------------------------
// Alarm analoge uitgang 50%
static s_disp_tekst const disp_alarm_analoog_str = { Disp_Draw_Tekst_L, 37, 22, &tekst_inst.Alarm_analoog_14 };
static s_disp_bitmap_array const disp_alarm_analoog_val = { Disp_Draw_Bitmap_Array, 190, 57, bmp_false_true, UCHAR, &opt_app.Motorgroup[0].AlarmAnaloog, 2 };

static void * const lcd_alarm_analoog_disp[] = { &disp_alarm_ico, &disp_alarm_analoog_str, &disp_alarm_analoog_val, 0 };

static s_key_value const key_alarm_analoog_val = { UCHAR, 1, &opt_app.Motorgroup[0].AlarmAnaloog, &uchar_0, &uchar_1 };
//-----------------------------------------------------------------------------



s_key_action const opt_groep_2_key_action[] =
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
  { // Type
    1,                                // nr
    0,                                // index
    (unsigned char *)&option_on,      // option
    &GroupIndex,                      // Optie index 
    lcd_type_disp,                    // display
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
    &GroupIndex,                      // Optie index 
    lcd_type_disp,                    // display
    &disp_cursor_225_78_150,          // cursor
    &key_type_val,                    // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Type_Value,                 // void (*arrow)(void); 
    Enter_Type_Value,                 // void (*enter)(void);
  },
  { // Type sturing digitaal/analoog
    2,                                // nr
    0,                                // index
    (unsigned char *)&option_on,      // option
    &GroupIndex,                      // Optie index 
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
    (unsigned char *)&option_on,      // option
    &GroupIndex,                      // Optie index 
    lcd_type_sturing_disp,            // display
    &disp_cursor_225_78_150,          // cursor
    &key_type_sturing_val,            // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Scroll_Value_Refresh_Screen,// void (*arrow)(void); 
    Enter_Value_Refresh_Screen,       // void (*enter)(void);
  },
  { // Digitale ingang open
    3,                                   // nr
    0,                                   // index
    &opt_app.Motorgroup[0].DigitalInput, // option
    &GroupIndex,                         // Optie index 
    lcd_dig_in_open,                     // display
    &disp_cursor,                        // cursor
    &dummy_value,                        // *value
    Dummy_Func,                          // void (*number)(void); 
    Arrow_IO_Select_Func,                // void (*arrow)(void); 
    Enter_IO_Select_Func,                // void (*enter)(void);
  },
  { // Digitale ingang dicht
    4,                                   // nr
    0,                                   // index
    &opt_app.Motorgroup[0].DigitalInput, // option
    &GroupIndex,                         // Optie index 
    lcd_dig_in_dicht,                    // display
    &disp_cursor,                        // cursor
    &dummy_value,                        // *value
    Dummy_Func,                          // void (*number)(void); 
    Arrow_IO_Select_Func,                // void (*arrow)(void); 
    Enter_IO_Select_Func,                // void (*enter)(void);
  },
  { // Analoge ingang positie
    5,                                   // nr
    0,                                   // index
    &opt_app.Motorgroup[0].AnalogInput,  // option
    &GroupIndex,                         // Optie index 
    lcd_ana_in_position,                 // display
    &disp_cursor,                        // cursor
    &dummy_value,                        // *value
    Dummy_Func,                          // void (*number)(void); 
    Arrow_IO_Select_Func,                // void (*arrow)(void); 
    Enter_IO_Select_Func,                // void (*enter)(void);
  },
  { // Digitale ingang aan/uit
    6,                                // nr
    0,                                // index
    &Motorgroup[0].TypeVent,          // option
    &GroupIndex,                      // Optie index 
    lcd_dig_in_aan_uit,               // display
    &disp_cursor,                     // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_Select_Func,             // void (*arrow)(void); 
    Enter_IO_Select_Func,             // void (*enter)(void);
  },
  { // Analoge uitgang terugmelding
    7,                                   // nr
    0,                                   // index
    &opt_app.Motorgroup[0].AnalogOutput, // option
    &GroupIndex,                         // Optie index 
    lcd_ana_out_terugmelding,            // display
    &disp_cursor,                        // cursor
    &dummy_value,                        // *value
    Dummy_Func,                          // void (*number)(void); 
    Arrow_IO_Select_Func,                // void (*arrow)(void); 
    Enter_IO_Select_Func,                // void (*enter)(void);
  },
  { // Looptijd
    8,                                // nr
    0,                                // index
    &DispLooptijd,                    // option
    &GroupIndex,                      // Optie index 
    lcd_stuurtijd_disp,               // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Option_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    8,                                // nr
    1,                                // index
    &DispLooptijd,                    // option
    &GroupIndex,                      // Optie index 
    lcd_stuurtijd_disp,               // display
    &disp_cursor_207_78_8,            // cursor
    &key_stuurtijd_val,               // *value
    Number_Value,                     // void (*number)(void); 
    Arrow_Option_Value,               // void (*arrow)(void); 
    Increment_Func_Index_Enter_Option_Value, // void (*enter)(void);
  },
  { // Aantal motoren
    9,                                // nr
    0,                                // index
    (unsigned char *)&option_on,      // option
    &GroupIndex,                      // Optie index 
    lcd_aantal_motoren_disp,          // display
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
    &GroupIndex,                      // Optie index 
    lcd_aantal_motoren_disp,          // display
    &disp_cursor_207_78_8,            // cursor
    &key_aantal_motoren_val,          // *value
    Number_Value,                     // void (*number)(void); 
    Arrow_Value_Refresh_Screen,       // void (*arrow)(void); 
    Enter_Value_Refresh_Screen,       // void (*enter)(void);
  },
  {
    9,                                // nr
    2,                                // index
    &DispDeviceNumberNotAvailable,      // option
    &GroupIndex,                      // Optie index 
    lcd_niet_beschikbaar_disp,        // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Increment_Func_Index,             // void (*arrow)(void); 
    Increment_Func_Index,             // void (*enter)(void);
  },
  { // Type ventilatoren - ebmBus / ModBus
    10,                               // nr
    0,                                // index
    &Motorgroup[0].TypeVent,          // option
    &GroupIndex,                      // Optie index 
    lcd_type_vent_disp,               // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Option_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    10,                               // nr
    1,                                // index
    &Motorgroup[0].TypeVent,          // option
    &GroupIndex,                      // Optie index 
    lcd_type_vent_disp,               // display
    &disp_cursor_225_78_180,          // cursor
    &key_type_vent_val,               // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Type_Device_Value,            // void (*arrow)(void); 
    Enter_Type_Device_Value,            // void (*enter)(void);
  },
  { // Operation Mode - ClosedLoop / OpenLoop
    11,                               // nr
    0,                                // index
    &DispClosedLoopOpenLoop,          // option
    &GroupIndex,                      // Optie index 
    lcd_operation_mode_disp,          // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Option_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    11,                               // nr
    1,                                // index
    &DispClosedLoopOpenLoop,          // option
    &GroupIndex,                      // Optie index 
    lcd_operation_mode_disp,          // display
    &disp_cursor_225_78_180,          // cursor
    &key_operation_mode_val,          // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Scroll_Option_Value,               // void (*arrow)(void); 
    Increment_Func_Index_Enter_Option_Value, // void (*enter)(void);
  },
  { // RampUpDown
    12,                               // nr
    0,                                // index
    &Motorgroup[0].TypeVent,          // option
    &GroupIndex,                      // Optie index 
    lcd_ramp_up_down_disp,            // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Option_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  { // RampUp
    12,                               // nr
    1,                                // index
    &Motorgroup[0].TypeVent,          // option
    &GroupIndex,                      // Optie index 
    lcd_ramp_up_down_disp,            // display
    &disp_cursor_207_52_8,            // cursor
    &key_ramp_up_val,                 // *value
    Number_Value,                     // void (*number)(void); 
    Arrow_Option_Value,               // void (*arrow)(void); 
    Increment_Func_Index_Enter_Option_Value, // void (*enter)(void);
  },
  { // RampDown
    12,                               // nr
    2,                                // index
    &Motorgroup[0].TypeVent,          // option
    &GroupIndex,                      // Optie index 
    lcd_ramp_up_down_disp,            // display
    &disp_cursor_207_78_8,            // cursor
    &key_ramp_down_val,               // *value
    Number_Value,                     // void (*number)(void); 
    Arrow_Option_Value,               // void (*arrow)(void); 
    Increment_Func_Index_Enter_Option_Value, // void (*enter)(void);
  },
  { // VentAtMax
    13,                               // nr
    0,                                // index
    &Motorgroup[0].TypeVent,          // option
    &GroupIndex,                      // Optie index 
    lcd_vent_at_max_disp,             // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Option_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    13,                               // nr
    1,                                // index
    &Motorgroup[0].TypeVent,          // option
    &GroupIndex,                      // Optie index 
    lcd_vent_at_max_disp,             // display
    &disp_cursor_207_78_8,            // cursor
    &key_vent_at_max_val,             // *value
    Number_Value,                     // void (*number)(void); 
    Arrow_Option_Value,               // void (*arrow)(void); 
    Increment_Func_Index_Enter_Option_Value, // void (*enter)(void);
  },
  { // PowerFactor
    14,                               // nr
    0,                                // index
    &Motorgroup[0].TypeVent,          // option
    &GroupIndex,                      // Optie index 
    lcd_power_factor_disp,            // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Option_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    14,                               // nr
    1,                                // index
    &Motorgroup[0].TypeVent,          // option
    &GroupIndex,                      // Optie index 
    lcd_power_factor_disp,            // display
    &disp_cursor_207_78_8,            // cursor
    &key_power_factor_val,            // *value
    Number_Value,                     // void (*number)(void); 
    Arrow_Option_Value,               // void (*arrow)(void); 
    Increment_Func_Index_Enter_Option_Value, // void (*enter)(void);
  },
  { // Watchdog Mode
    15,                               // nr
    0,                                // index
    &DispWatchdogMode,                // option
    &GroupIndex,                      // Optie index 
    lcd_watchdog_mode_disp,           // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Option_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    15,                               // nr
    1,                                // index
    &DispWatchdogMode,                // option
    &GroupIndex,                      // Optie index 
    lcd_watchdog_mode_disp,           // display
    &disp_cursor_checkbox,            // cursor
    &key_watchdog_mode_val,           // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Scroll_Option_Value_Refresh_ScreenNr, // void (*arrow)(void); 
    Increment_Func_Index,             // void (*enter)(void);
  },
  { // Safety position
    16,                               // nr
    0,                                // index
    &opt_app.Motorgroup[0].WatchdogMode, // option
    &GroupIndex,                      // Optie index 
    lcd_watchdog_position_disp,       // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Option_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    16,                               // nr
    1,                                // index
    &opt_app.Motorgroup[0].WatchdogMode, // option
    &GroupIndex,                      // Optie index 
    lcd_watchdog_position_disp,       // display
    &disp_cursor_207_78_8,            // cursor
    &key_watchdog_position_val,       // *value
    Number_Value,                     // void (*number)(void); 
    Arrow_Option_Value,               // void (*arrow)(void); 
    Increment_Func_Index_Enter_Option_Value, // void (*enter)(void);
  },
  { // SensorType
    17,                               // nr
    0,                                // index
    &Motorgroup[0].TypeKlep,          // option
    &GroupIndex,                      // Optie index 
    lcd_sensor_type_disp,             // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Option_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    17,                               // nr
    1,                                // index
    &Motorgroup[0].TypeKlep,          // option
    &GroupIndex,                      // Optie index 
    lcd_sensor_type_disp,             // display
    &disp_cursor_225_78_180,          // cursor
    &key_sensor_type_val,             // *value
    Number_Value,                     // void (*number)(void); 
    Arrow_Scroll_Value_Refresh_Screen,// void (*arrow)(void); 
    Enter_Value_Refresh_Screen,       // void (*enter)(void);
  },
  { // Address range
    18,                               // nr
    0,                                // index
    &DispModbusIO,                    // option
    &GroupIndex,                      // Optie index 
    lcd_adressen_disp,                // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Option_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    18,                               // nr
    1,                                // index
    &DispModbusIO,                    // option
    &GroupIndex,                      // Optie index 
    lcd_adressen_disp,                // display
    &disp_cursor_153_52_8,            // cursor
    &key_eerste_nummer_val,           // *value
    Number_Value,                     // void (*number)(void); 
    Arrow_Range_Device_Number_Value,    // void (*arrow)(void); 
    Enter_Range_Device_Number_Value,    // void (*arrow)(void); 
  },
  {
    18,                               // nr
    2,                                // index
    &DispModbusIO,                    // option
    &GroupIndex,                      // Optie index 
    lcd_adressen_disp,                // display
    &disp_cursor_153_78_8,            // cursor
    &key_eerste_adres_val,            // *value
    Number_Value,                     // void (*number)(void); 
    Arrow_Range_Device_Address_Value,   // void (*arrow)(void); 
    Enter_Range_Device_Address_Value,   // void (*arrow)(void); 
  },

  { // IO ventilatoren (RS485)
    19,                               // nr
    0,                                // index
    &DispModbusIO,                    // option
    &GroupIndex,                      // Optie index 
    lcd_ventilatoren,                 // display
    &disp_cursor,                     // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_Device_Select_Func,        // void (*arrow)(void); 
    Enter_IO_Device_Select_Func,        // void (*enter)(void);
  },
  { // IO motoren
    20,                               // nr
    0,                                // index
    &DispMotorIO,                     // option
    &GroupIndex,                      // Optie index 
    lcd_motoren,                      // display
    &disp_cursor,                     // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_Motor_Func,              // void (*arrow)(void); 
    Enter_IO_Select_Func,             // void (*enter)(void);
  },

  { // Wijzig adres
    21,                               // nr
    0,                                // index
    &Motorgroup[0].TypeVent,          // option
    &GroupIndex,                      // Optie index 
    lcd_wijzig_adres_disp,            // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Wijzig_Adres_Func,          // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    21,                               // nr
    1,                                // index
    &Motorgroup[0].TypeVent,          // option
    &GroupIndex,                      // Optie index 
    lcd_wijzig_adres_disp,            // display
    &disp_cursor_147_78_8,            // cursor
    &key_oud_adres_val,               // *value
    Number_Value,                     // void (*number)(void); 
    Arrow_Value,                      // void (*arrow)(void); 
    Increment_Func_Index_Enter_Value, // void (*enter)(void);
  },
  {
    21,                               // nr
    2,                                // index
    &Motorgroup[0].TypeVent,          // option
    &GroupIndex,                      // Optie index 
    lcd_wijzig_adres_disp,            // display
    &disp_cursor_207_78_8,            // cursor
    &key_nieuw_adres_val,             // *value
    Number_Value,                     // void (*number)(void); 
    Arrow_Wijzig_Adres_Value,         // void (*arrow)(void); 
    Enter_Wijzig_Adres_Value,         // void (*enter)(void);
  },
  {
    21,                               // nr
    3,                                // index
    &Motorgroup[0].TypeVent,          // option
    &GroupIndex,                      // Optie index 
    lcd_wijzig_adres_ok_disp,         // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Increment_Func_Index,             // void (*arrow)(void); 
    Enter_Wijzig_Adres_Func,          // void (*enter)(void);
  },
  { // Frequentie gestuurd
    22,                               // nr
    0,                                // index
    &DispFrequency,                   // option
    &GroupIndex,                      // Optie index 
    lcd_frequentie_gestuurd_disp,     // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Option_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    22,                               // nr
    1,                                // index
    &DispFrequency,                   // option
    &GroupIndex,                      // Optie index 
    lcd_frequentie_gestuurd_disp,     // display
    &disp_cursor_checkbox,            // cursor
    &key_frequentie_gestuurd_val,     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Scroll_Value_Refresh_Screen,// void (*arrow)(void); 
    Enter_Value_Refresh_Screen,       // void (*enter)(void);
  },
  { // Frequentie verstel
    23,                               // nr
    0,                                // index
    &DispFrequencyVerstel,            // option
    &GroupIndex,                      // Optie index 
    lcd_frequentie_verstel_disp,      // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Option_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    23,                               // nr
    1,                                // index
    &DispFrequencyVerstel,            // option
    &GroupIndex,                      // Optie index 
    lcd_frequentie_verstel_disp,      // display
    &disp_cursor_192_78_8,            // cursor
    &key_frequentie_verstel_val,      // *value
    Number_Value,                     // void (*number)(void); 
    Arrow_Value_Refresh_Screen,       // void (*arrow)(void); 
    Enter_Value_Refresh_Screen,       // void (*enter)(void);
  },
  { // Digitale ingang hoge snelheid
    24,                                      // nr
    0,                                       // index
    &opt_app.Motorgroup[0].DispHiSpeedInput, // option
    &GroupIndex,                             // Optie index 
    lcd_dig_in_hoge_snelheid,                // display
    &disp_cursor,                            // cursor
    &dummy_value,                            // *value
    Dummy_Func,                              // void (*number)(void); 
    Arrow_IO_Select_Func,                    // void (*arrow)(void); 
    Enter_IO_Select_Func,                    // void (*enter)(void);
  },
  { // Uitgang snelheid digitaal/analoog
    25,                                         // nr
    0,                                          // index
    &opt_app.Motorgroup[0].FrequencyControlled, // option
    &GroupIndex,                                // Optie index 
    lcd_uitgang_snelheid_disp,                  // display
    0,                                          // cursor
    &dummy_value,                               // *value
    Dummy_Func,                                 // void (*number)(void); 
    Arrow_Option_Func,                          // void (*arrow)(void); 
    Dummy_Func,                                 // void (*enter)(void);
  },
  {
    25,                                         // nr
    1,                                          // index
    &opt_app.Motorgroup[0].FrequencyControlled, // option
    &GroupIndex,                                // Optie index 
    lcd_uitgang_snelheid_disp,                  // display
    &disp_cursor_225_78_150,                    // cursor
    &key_uitgang_snelheid_val,                  // *value
    Dummy_Func,                                 // void (*number)(void); 
    Arrow_Scroll_Value_Refresh_Screen,          // void (*arrow)(void); 
    Enter_Value_Refresh_Screen,                 // void (*enter)(void);
  },
  { // Snelheid laag/hoog
    26,                                 // nr
    0,                                  // index
    &opt_app.Motorgroup[0].AnalogSpeed, // option
    &GroupIndex,                        // Optie index 
    lcd_snelheid_disp,                  // display
    0,                                  // cursor
    &dummy_value,                       // *value
    Dummy_Func,                         // void (*number)(void); 
    Arrow_Option_Func,                  // void (*arrow)(void); 
    Dummy_Func,                         // void (*enter)(void);
  },
  {
    26,                                 // nr
    1,                                  // index
    &opt_app.Motorgroup[0].AnalogSpeed, // option
    &GroupIndex,                        // Optie index 
    lcd_snelheid_disp,                  // display
    &disp_cursor_192_52_8,              // cursor
    &key_snelheid_laag_val,             // *value
    Number_Value,                       // void (*number)(void); 
    Arrow_Value_Refresh_Screen,         // void (*arrow)(void); 
    Enter_Value_Refresh_Screen,         // void (*enter)(void);
  },
  {
    26,                                 // nr
    2,                                  // index
    &opt_app.Motorgroup[0].AnalogSpeed, // option
    &GroupIndex,                        // Optie index 
    lcd_snelheid_disp,                  // display
    &disp_cursor_192_78_8,              // cursor
    &key_snelheid_hoog_val,             // *value
    Number_Value,                       // void (*number)(void); 
    Arrow_Value_Refresh_Screen,         // void (*arrow)(void); 
    Enter_Value_Refresh_Screen,         // void (*enter)(void);
  },
  { // Terugschakel positie open/dicht
    27,                                         // nr
    0,                                          // index
    &opt_app.Motorgroup[0].FrequencyControlled, // option
    &GroupIndex,                                // Optie index 
    lcd_terugschakel_positie_disp,              // display
    0,                                          // cursor
    &dummy_value,                               // *value
    Dummy_Func,                                 // void (*number)(void); 
    Arrow_Option_Func,                          // void (*arrow)(void); 
    Dummy_Func,                                 // void (*enter)(void);
  },
  {
    27,                                         // nr
    1,                                          // index
    &opt_app.Motorgroup[0].FrequencyControlled, // option
    &GroupIndex,                                // Optie index 
    lcd_terugschakel_positie_disp,              // display
    &disp_cursor_192_78_8,                      // cursor
    &key_terugschakel_positie_val,              // *value
    Number_Value,                               // void (*number)(void); 
    Arrow_Value_Refresh_Screen,                 // void (*arrow)(void); 
    Enter_Value_Refresh_Screen,                 // void (*enter)(void);
  },
  { // Pulse system
    28,                               // nr
    0,                                // index
    &Motorgroup[0].TypeDoek,          // option
    &GroupIndex,                      // Optie index
    lcd_pulse_system_disp,            // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Option_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    28,                               // nr
    1,                                // index
    &Motorgroup[0].TypeDoek,          // option
    &GroupIndex,                      // Optie index 
    lcd_pulse_system_disp,            // display
    &disp_cursor_checkbox,            // cursor
    &key_pulse_system_val,            // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Scroll_Value_Refresh_Screen,// void (*arrow)(void); 
    Enter_Value_Refresh_Screen,       // void (*enter)(void);
  },
  { // Pulse system
    29,                               // nr
    0,                                // index
    &Motorgroup[0].TypeDoek,          // option
    &GroupIndex,                      // Optie index
    lcd_kier_regeling_disp,           // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Option_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    29,                               // nr
    1,                                // index
    &Motorgroup[0].TypeDoek,          // option
    &GroupIndex,                      // Optie index 
    lcd_kier_regeling_disp,           // display
    &disp_cursor_checkbox,            // cursor
    &key_kier_regeling_val,           // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Scroll_Value_Refresh_Screen,// void (*arrow)(void); 
    Enter_Value_Refresh_Screen,       // void (*enter)(void);
  },
  { // Position
    30,                                  // nr
    0,                                   // index
    &opt_app.Motorgroup[0].DigitalInput, // option
    &GroupIndex,                         // Optie index 
    lcd_position_disp,                   // display
    0,                                   // cursor
    &dummy_value,                        // *value
    Dummy_Func,                          // void (*number)(void); 
    Arrow_Position_Func,                 // void (*arrow)(void); 
    Dummy_Func,                          // void (*enter)(void);
  },
  {
    30,                                  // nr
    1,                                   // index
    &opt_app.Motorgroup[0].DigitalInput, // option
    &GroupIndex,                         // Optie index 
    lcd_position_disp,                   // display
    &disp_cursor_207_78_8,               // cursor
    &key_position_val,                   // *value
    Number_Position_Value,               // void (*number)(void); 
    Arrow_Position_Value,                // void (*arrow)(void); 
    Enter_Position_Value,                // void (*enter)(void);
  },
  { // Digitale uitgang alarm
    31,                               // nr
    0,                                // index
    (unsigned char *)&option_on,      // option
    &GroupIndex,                      // Optie index 
    lcd_dig_out_alarm,                // display
    &disp_cursor,                     // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_Select_Func,             // void (*arrow)(void); 
    Enter_IO_Select_Func,             // void (*enter)(void);
  },
  { // Digitale uitgang urgent alarm
    32,                               // nr
    0,                                // index
    &Motorgroup[0].TypeVent,          // option
    &GroupIndex,                      // Optie index 
    lcd_dig_out_alarm_urgent,         // display
    &disp_cursor,                     // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_Select_Func,             // void (*arrow)(void); 
    Enter_IO_Select_Func,             // void (*enter)(void);
  },
  { // Digitale uitgang eindschakelaar alarm
    33,                               // nr
    0,                                // index
    &opt_app.Motorgroup[0].SensorType,// option
    &GroupIndex,                      // Optie index 
    lcd_dig_out_alarm_eindschakelaar, // display
    &disp_cursor,                     // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_Select_Func,             // void (*arrow)(void); 
    Enter_IO_Select_Func,             // void (*enter)(void);
  },
  { // Alarm analoog
    34,                               // nr
    0,                                // index
    &DispAlarmAnaloog,                // option
    &GroupIndex,                      // Optie index 
    lcd_alarm_analoog_disp,           // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Option_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    34,                               // nr
    1,                                // index
    &DispAlarmAnaloog,                // option
    &GroupIndex,                      // Optie index 
    lcd_alarm_analoog_disp,           // display
    &disp_cursor_checkbox,            // cursor
    &key_alarm_analoog_val,           // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Scroll_Value_Refresh_Screen,// void (*arrow)(void); 
    Enter_Value_Refresh_Screen,       // void (*enter)(void);
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
s_screen screen_opt_groep_2;
s_screen const screen_opt_groep_2_default =
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
  &opt_groep_2_key_action[0], // first_action
  &opt_groep_2_key_action[sizeof(opt_groep_2_key_action)/sizeof(s_key_action) - 1], // last action 
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

void Control_Screen_Option_Groep_2(unsigned char nr)
{
  if (password_enabled & PASSWORD_ENABLED_OPT_MASK)
    start_flag = end_flag = 1;
  else
    start_flag = end_flag = 0;
  Enter_IO_Select_Func();
  GroupIndex = nr;
  GetMaxMotor();
  
  SetDisplayOptions();

  Control_Screen(&screen_opt_groep_2, &screen_opt_groep_2_default, 1, 1);

  OudAdres   = 1;
  NieuwAdres = 1;
  AdresWijzigenOk      = 0;
  AdresWijzigenMislukt = 0;
  AdresWijzigenStatus  = 0;
}

//------------------------------------------------------------------------------
static char isWatchdogModePossible(unsigned char index)
{
  if (module.WatchdogMode == 0)
    return 0;
  if (opt_app.Motorgroup[index].Type != TYPE_VENT)
    return 0;

  return ((opt_app.Motorgroup[index].BusType == dtECblueModbus) || (opt_app.Motorgroup[index].BusType == dtEbmModbus));
//  return ((opt_app.Motorgroup[index].BusType == dtECblueModbus) || (opt_app.Motorgroup[index].BusType == dtECbluePremium) || (opt_app.Motorgroup[index].BusType == dtEbmModbus));
}

static void SetDisplayOptions(void)
{
  switch (opt_app.Motorgroup[GroupIndex].Type)
  {
    case TYPE_RAAM:
      if (opt_app.Motorgroup[GroupIndex].NumberMotors > 0)
        DispMotorIO = 1;
      else
        DispMotorIO = 0;
      DispModbusIO  = 0;
      DispLooptijd  = 0;
      DispFrequency = 1;
      DispClosedLoopOpenLoop = 0;
      if (opt_alg.can_backbone || opt_alg.BACnet_enabled)
        DispAlarmAnaloog = 0;
      else
        DispAlarmAnaloog = 1;
      break;
    case TYPE_DOEK:
      if (opt_app.Motorgroup[GroupIndex].NumberMotors > 0)
        DispMotorIO = 1;
      else
        DispMotorIO = 0;
      DispModbusIO  = 0;
      DispLooptijd  = 0;
      DispFrequency = 1;
      DispClosedLoopOpenLoop = 0;
      if (opt_alg.can_backbone || opt_alg.BACnet_enabled)
        DispAlarmAnaloog = 0;
      else
        DispAlarmAnaloog = 1;
      break;
    case TYPE_VENT:
      if (opt_app.Motorgroup[GroupIndex].NumberMotors > 0)
        DispModbusIO = 1;
      else
        DispModbusIO = 0;
      if (opt_app.Motorgroup[GroupIndex].DigitalInput)
        DispLooptijd = 1;
      else
        DispLooptijd = 0;
      if ((opt_app.Motorgroup[GroupIndex].BusType == dtEbmBus) || (opt_app.Motorgroup[GroupIndex].BusType == dtEbmModbus))
        DispClosedLoopOpenLoop = 1;
      else
        DispClosedLoopOpenLoop = 0;
      DispMotorIO      = 0;
      DispFrequency    = 0;
      DispAlarmAnaloog = 0;
      break;
    case TYPE_KLEP:
      if (opt_app.Motorgroup[GroupIndex].NumberMotors > 0)
        DispModbusIO = 1;
      else
        DispModbusIO = 0;
      if (opt_app.Motorgroup[GroupIndex].DigitalInput)
        DispLooptijd = 1;
      else
        DispLooptijd = 0;
      DispMotorIO    = 0;
      DispFrequency  = 0;
      DispAlarmAnaloog = 0;
      DispClosedLoopOpenLoop = 0;
      break;
  }
  DispWatchdogMode = isWatchdogModePossible(GroupIndex);
  if (opt_app.Motorgroup[GroupIndex].FrequencyControlled && (opt_alg.can_backbone || opt_alg.BACnet_enabled))
    DispFrequencyVerstel = 1;
  else
    DispFrequencyVerstel = 0;
}

//------------------------------------------------------------------------------
static unsigned char GetFirstDeviceNumberAvailable(void)
{
int i, j;
int FirstAvailable = 0;

  j = 0;
  for (i = 1; i < opt_app.NumberMotorgroups; i++)
  {
    if (opt_app.Motorgroup[i].FirstNumber > opt_app.Motorgroup[j].FirstNumber)
      j = i;
  }
  if (opt_app.Motorgroup[j].FirstNumber > 0)
    FirstAvailable = opt_app.Motorgroup[j].FirstNumber + opt_app.Motorgroup[j].NumberMotors;
  else
    FirstAvailable = 1;
  if (FirstAvailable > 255)
    FirstAvailable = 0;
  return (FirstAvailable);
}

static unsigned char GetFirstDeviceAddressAvailable(void)
{
int i, j;
int FirstAvailable = 0;

  j = 0;
  for (i = 1; i < opt_app.NumberMotorgroups; i++)
  {
    if (opt_app.Motorgroup[i].FirstAddress > opt_app.Motorgroup[j].FirstAddress)
      j = i;
  }
  if (opt_app.Motorgroup[j].FirstAddress > 0)
    FirstAvailable = opt_app.Motorgroup[j].FirstAddress + opt_app.Motorgroup[j].NumberMotors;
  else
    FirstAvailable = 1;
  if (FirstAvailable > 255)
    FirstAvailable = 0;
  return (FirstAvailable);
}

static unsigned char GetMaxDeviceAvailable(unsigned char nr)
{
int i;
int LastNumber  = MAX_DEVICE;
int LastAddress = MAX_DEVICE;
int MaxNumber;
int MaxAddress;

  for (i = 0; i < opt_app.NumberMotorgroups; i++)
  {
    if ((opt_app.Motorgroup[i].FirstNumber > opt_app.Motorgroup[nr].FirstNumber) && (opt_app.Motorgroup[i].FirstNumber < LastNumber))
      LastNumber = opt_app.Motorgroup[i].FirstNumber;
    if ((opt_app.Motorgroup[i].FirstAddress > opt_app.Motorgroup[nr].FirstAddress) && (opt_app.Motorgroup[i].FirstAddress < LastAddress))
      LastAddress = opt_app.Motorgroup[i].FirstAddress;
  }
  MaxNumber  = LastNumber  - opt_app.Motorgroup[nr].FirstNumber;
  MaxAddress = LastAddress - opt_app.Motorgroup[nr].FirstAddress;
  if (MaxAddress < MaxNumber)
    MaxNumber = MaxAddress;
  if (MaxNumber > 255)
    MaxNumber = 255;
  return (MaxNumber);
}

//------------------------------------------------------------------------------
void CheckOptionsGroup(void)
{
int i, j;
int MaxAvailable;

  DispDeviceNumberNotAvailable = 0;
  for (i = 0; i < MAX_GROUP; i++)
  {
    if ((opt_app.Motorgroup[i].Type == TYPE_VENT) && (module.Ventilatie == 0))
      opt_app.Motorgroup[i].Enabled = 0;
    if ((opt_app.Motorgroup[i].Type == TYPE_KLEP) && (module.Klep == 0))
      opt_app.Motorgroup[i].Enabled = 0;

    if (opt_app.Motorgroup[i].Enabled)
    {
      switch (opt_app.Motorgroup[i].Type)
      {
        case TYPE_RAAM:
          Motorgroup[i].TypeRaam = 1;
          Motorgroup[i].TypeDoek = 0;
          Motorgroup[i].TypeVent = 0;
          Motorgroup[i].TypeKlep = 0;
	      Motorgroup[i].AlarmAfw = 1;
          opt_app.Motorgroup[i].PulseSystem  = 0;
          opt_app.Motorgroup[i].KierRegeling = 0;
          opt_app.Motorgroup[i].FirstNumber  = 0;
          opt_app.Motorgroup[i].FirstAddress = 0;
		  opt_app.Motorgroup[i].SensorType   = 0;
          opt_app.Motorgroup[i].DigOutAlarmFlap  = IO_empty;
          opt_app.Ventgroup[i].DigInOnOff        = IO_empty;
          opt_app.Ventgroup[i].DigOutAlarmUrgent = IO_empty;
          for (j = 0; j < 4; j++)
            opt_app.Ventgroup[i].RS485Bus[j] = IO_empty;
          break;
        case TYPE_DOEK:
          Motorgroup[i].TypeRaam = 0;
          Motorgroup[i].TypeDoek = 1;
          Motorgroup[i].TypeVent = 0;
          Motorgroup[i].TypeKlep = 0;
	      Motorgroup[i].AlarmAfw = 1;
          opt_app.Motorgroup[i].FirstNumber  = 0;
          opt_app.Motorgroup[i].FirstAddress = 0;
		  opt_app.Motorgroup[i].SensorType   = 0;
          opt_app.Motorgroup[i].DigOutAlarmFlap  = IO_empty;
          opt_app.Ventgroup[i].DigInOnOff        = IO_empty;
          opt_app.Ventgroup[i].DigOutAlarmUrgent = IO_empty;
          for (j = 0; j < 4; j++)
            opt_app.Ventgroup[i].RS485Bus[j] = IO_empty;
          break;
        case TYPE_VENT:
          Motorgroup[i].TypeRaam = 0;
          Motorgroup[i].TypeDoek = 0;
          Motorgroup[i].TypeVent = 1;
          Motorgroup[i].TypeKlep = 0;
	      Motorgroup[i].AlarmAfw = 0;
          opt_app.Motorgroup[i].ControlType         = 0;
          opt_app.Motorgroup[i].ControlIndex        = 0;
          opt_app.Motorgroup[i].FrequencyControlled = 0;
          opt_app.Motorgroup[i].PulseSystem         = 0;
          opt_app.Motorgroup[i].KierRegeling        = 0;
		  opt_app.Motorgroup[i].SensorType          = 0;
          opt_app.Motorgroup[i].DigOutAlarmFlap     = IO_empty;

          if (opt_app.Motorgroup[i].VentAtMax == 0)
            opt_app.Motorgroup[i].VentAtMax = 100; 
          if (opt_app.Motorgroup[i].NumberMotors == 0)
          {
            opt_app.Motorgroup[i].FirstNumber  = 0;
            opt_app.Motorgroup[i].FirstAddress = 0;
          }
          else
          {
            if (opt_app.Motorgroup[i].FirstNumber == 0)
            {
              opt_app.Motorgroup[i].FirstNumber  = GetFirstDeviceNumberAvailable();
              opt_app.Motorgroup[i].FirstAddress = GetFirstDeviceAddressAvailable();
              if ((opt_app.Motorgroup[i].FirstNumber == 0) || (opt_app.Motorgroup[i].FirstAddress == 0))
              {
                DispDeviceNumberNotAvailable = 1;
                opt_app.Motorgroup[i].NumberMotors = 0;
                opt_app.Motorgroup[i].FirstNumber  = 0;
                opt_app.Motorgroup[i].FirstAddress = 0;
              }
            }
            if (opt_app.Motorgroup[i].NumberMotors > 0)
            {
              MaxAvailable = GetMaxDeviceAvailable(i);
              if (opt_app.Motorgroup[i].NumberMotors > MaxAvailable)
              {
                DispDeviceNumberNotAvailable = 1;
                opt_app.Motorgroup[i].NumberMotors = MaxAvailable;
              }
            }
          }
          break;
        case TYPE_KLEP:
          Motorgroup[i].TypeRaam = 0;
          Motorgroup[i].TypeDoek = 0;
          Motorgroup[i].TypeVent = 0;
          Motorgroup[i].TypeKlep = 1;
	      Motorgroup[i].AlarmAfw = 1;
          opt_app.Motorgroup[i].ControlType         = 0;
          opt_app.Motorgroup[i].ControlIndex        = 0;
          opt_app.Motorgroup[i].FrequencyControlled = 0;
          opt_app.Motorgroup[i].PulseSystem         = 0;
          opt_app.Motorgroup[i].KierRegeling        = 0;
		  if (opt_app.Motorgroup[i].SensorType == 0)
		    opt_app.Motorgroup[i].DigOutAlarmFlap = IO_empty;
          if (opt_app.Motorgroup[i].NumberMotors == 0)
          {
            opt_app.Motorgroup[i].FirstNumber  = 0;
            opt_app.Motorgroup[i].FirstAddress = 0;
          }
          else
          {
            if (opt_app.Motorgroup[i].FirstNumber == 0)
            {
              opt_app.Motorgroup[i].FirstNumber  = GetFirstDeviceNumberAvailable();
              opt_app.Motorgroup[i].FirstAddress = GetFirstDeviceAddressAvailable();
              if ((opt_app.Motorgroup[i].FirstNumber == 0) || (opt_app.Motorgroup[i].FirstAddress == 0))
              {
                DispDeviceNumberNotAvailable = 1;
                opt_app.Motorgroup[i].NumberMotors = 0;
                opt_app.Motorgroup[i].FirstNumber  = 0;
                opt_app.Motorgroup[i].FirstAddress = 0;
              }
            }
            if (opt_app.Motorgroup[i].NumberMotors > 0)
            {
              MaxAvailable = GetMaxDeviceAvailable(i);
              if (opt_app.Motorgroup[i].NumberMotors > MaxAvailable)
              {
                DispDeviceNumberNotAvailable = 1;
                opt_app.Motorgroup[i].NumberMotors = MaxAvailable;
              }
            }
          }
          break;
      }
      if (opt_alg.can_backbone || opt_alg.BACnet_enabled)
      {
        opt_app.Motorgroup[i].DispHiSpeedInput = 0;
        opt_app.Motorgroup[i].AnalogInput      = 0;
        opt_app.Motorgroup[i].DigitalInput     = 0;
        opt_app.Motorgroup[i].AnalogOutput     = 0;
        opt_app.Motorgroup[i].DigInOpen        = IO_empty;
        opt_app.Motorgroup[i].DigInClose       = IO_empty;
        opt_app.Motorgroup[i].AnaInPosition    = IO_empty;
        opt_app.Motorgroup[i].AnaOutPosition   = IO_empty;
      }
      else
      {
        opt_app.Motorgroup[i].AnalogOutput     = 1;
        if (opt_app.Motorgroup[i].AnalogInput)
        {
          opt_app.Motorgroup[i].DigitalInput = 0;
          opt_app.Motorgroup[i].DigInOpen  = IO_empty;
          opt_app.Motorgroup[i].DigInClose = IO_empty;
        }
        else
        {
          opt_app.Motorgroup[i].DigitalInput = 1;
          opt_app.Motorgroup[i].AnaInPosition = IO_empty;
        }

        if (opt_app.Motorgroup[i].FrequencyControlled)
          opt_app.Motorgroup[i].DispHiSpeedInput = 1;
        else
          opt_app.Motorgroup[i].DispHiSpeedInput = 0;
      }
      if (opt_app.Motorgroup[i].FrequencyControlled == 0)
        opt_app.Motorgroup[i].AnalogSpeed = 0;
      if (opt_app.Motorgroup[i].DispHiSpeedInput == 0)
        opt_app.Motorgroup[i].DigInHiSpeed = IO_empty;
      if (opt_app.Motorgroup[i].FrequentieVerstel < opt_app.Motorgroup[i].PositionLowSpeed)
        opt_app.Motorgroup[i].FrequentieVerstel = opt_app.Motorgroup[i].PositionLowSpeed;
	  if (isWatchdogModePossible(i) == 0)
	    opt_app.Motorgroup[i].WatchdogMode = 0;
    }
    else
    {
      opt_app.Motorgroup[i]    = default_opt_app.Motorgroup[i];
      setp_alg.Motorgroup[i]   = default_setp_alg.Motorgroup[i];
      val_hr_alg.Motorgroup[i] = default_val_hr_alg.Motorgroup[i];

      opt_app.Ventgroup[i]     = default_opt_app.Ventgroup[i];
      setp_alg.Ventgroup[i]    = default_setp_alg.Ventgroup[i];
    }
  }
}

//-----------------------------------------------------------------------------
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
      val_hr_alg.Motor[i] = default_val_hr_alg.Motor[i];
    }
    else if (opt_app.Motorgroup[opt_app.Motor[i].GroupNumber - 1].Enabled == 0)
    {
      opt_app.Motor[i]    = default_opt_app.Motor[i];
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
              if (opt_app.Motorgroup[i].ControlType == CONTROL_NORMAL)
              {
                if (opt_app.Motorgroup[i].Type == TYPE_RAAM)
                  IO_Set_Motor_Control_Option(&opt_app.Motor[j].IO, OPTION_POSITION | OPTION_SINGLE_WINDOW);
                else if (opt_app.Motorgroup[i].Type == TYPE_DOEK)
                  IO_Set_Motor_Control_Option(&opt_app.Motor[j].IO, OPTION_POSITION | OPTION_SINGLE_SCREEN);
              }
              else
              {
                IO_Set_Motor_Control_Option(&opt_app.Motor[j].IO, OPTION_POSITION);
              }
              MotorIndex++;
            }
            else
            {
              opt_app.Motor[j]    = default_opt_app.Motor[j];
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
            if (opt_app.Motorgroup[i].ControlType == CONTROL_NORMAL)
            {
              if (opt_app.Motorgroup[i].Type == TYPE_RAAM)
                IO_Set_Motor_Control_Option(&opt_app.Motor[j].IO, OPTION_POSITION | OPTION_SINGLE_WINDOW);
              else if (opt_app.Motorgroup[i].Type == TYPE_DOEK)
                IO_Set_Motor_Control_Option(&opt_app.Motor[j].IO, OPTION_POSITION | OPTION_SINGLE_SCREEN);
            }
            else
            {
              IO_Set_Motor_Control_Option(&opt_app.Motor[j].IO, OPTION_POSITION);
            }
            if (MotorIndex >= opt_app.Motorgroup[i].NumberMotors)
              break;
          }
        }
      }
    }
  }
}

//-----------------------------------------------------------------------------
static void DeleteDevice(unsigned char index)
{
  ResetAlarmDevice(&Device[index]);
  opt_app.Device[index]    = default_opt_app.Device[index];
  setp_alg.Device[index]   = default_setp_alg.Device[index];
  val_hr_alg.Device[index] = default_val_hr_alg.Device[index];
}

void CheckOptionsDevice(void)
{
int i, j;

  for (i = 0; i < MAX_DEVICE; i++)
    opt_app.Device[i].Enabled = 0;

  for (i = 0; i < opt_app.NumberMotorgroups; i++)
  {
    switch (opt_app.Motorgroup[i].Type)
    {
      case TYPE_VENT:
        if(opt_app.Motorgroup[i].BusType == dtBelimoModbus)
		  opt_app.Motorgroup[i].BusType = dtEbmBus;  

        if (opt_app.Motorgroup[i].FirstNumber > 0)
        {
          for (j = opt_app.Motorgroup[i].FirstNumber; j < opt_app.Motorgroup[i].FirstNumber + opt_app.Motorgroup[i].NumberMotors; j++)
          {
            opt_app.Device[j - 1].Enabled     = 1;
            opt_app.Device[j - 1].GroupNumber = i + 1;
          }
        }
        break;
      case TYPE_KLEP:
        opt_app.Motorgroup[i].BusType = dtBelimoModbus;
        if (opt_app.Motorgroup[i].FirstNumber > 0)
        {
          for (j = opt_app.Motorgroup[i].FirstNumber; j < opt_app.Motorgroup[i].FirstNumber + opt_app.Motorgroup[i].NumberMotors; j++)
          {
            opt_app.Device[j - 1].Enabled     = 1;
            opt_app.Device[j - 1].GroupNumber = i + 1;
          }
        }
        break;
    }
  }

  for (i = 0; i < MAX_DEVICE; i++)
  {
    if (opt_app.Device[i].Enabled == 0)
      DeleteDevice(i);
  }
}

//------------------------------------------------------------------------------
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

//------------------------------------------------------------------------------
void CheckOptionsPulseSystem(void)
{
int i;
TMotor *pMotor;

  for (i = 0; i < opt_app.NumberMotorgroups; i++)
  {
    if (opt_app.Motorgroup[i].Type == TYPE_DOEK)
    {
      pMotor = Motorgroup[i].FirstMotor;
      while (pMotor != NULL)
      {
        pMotor->PulseSystem.Enabled   =   opt_app.Motorgroup[i].PulseSystem;
        pMotor->PulseSystem.Setpoints = &setp_alg.Motorgroup[i].PulseSystem;
        pMotor = pMotor->Next;
      }
    }
  }
}

//------------------------------------------------------------------------------
static unsigned char DummyNotUsed(s_board_IO_on_off IO_new)
{
  IO_new;
  return (1);
}

static unsigned char AnaInNotUsed(s_board_IO_on_off IO_new)
{
int i;

  for (i = 0; i < MAX_LUCHTMENGKAST_GROEP; i++)
  {
    if (Board_IO_Used(IO_new, &opt_app.LuchtmengkastGroep[i].Afblaasvent.AnaIn, 1)) return (0);
    if (Board_IO_Used(IO_new, &opt_app.LuchtmengkastGroep[i].Inblaasvent.AnaIn, 1)) return (0);
  }
  for (i = 0; i < MAX_LUCHTMENGKAST; i++)
  {
    if (Board_IO_Used(IO_new, &opt_app.Luchtmengkast[i].Afblaasvent.AnaIn, 1)) return (0);
    if (Board_IO_Used(IO_new, &opt_app.Luchtmengkast[i].Inblaasvent.AnaIn, 1)) return (0);
  }
  for (i = 0; i < MAX_GROUP; i++)
  {
    if (Board_IO_Used(IO_new, &opt_app.Motorgroup[i].AnaInPosition, 1)) return (0);
  }
  return (1);
}

static unsigned char DigInHiSpeedNotUsed(s_board_IO_on_off IO_new)
{
int i;

  for (i = 0; i < MAX_GROUP; i++)
  {
    if (Board_IO_Used(IO_new, &opt_app.Ventgroup[i].DigInOnOff,  1)) return (0);
    if (Board_IO_Used(IO_new, &opt_app.Motorgroup[i].DigInOpen,  1)) return (0);
    if (Board_IO_Used(IO_new, &opt_app.Motorgroup[i].DigInClose, 1)) return (0);
  }
  return (1);
}

static unsigned char AnaOutNotUsed(s_board_IO_on_off IO_new)
{
int i;

  for (i = 0; i < MAX_LUCHTMENGKAST_GROEP; i++)
  {
    if (Board_IO_Used(IO_new, &opt_app.LuchtmengkastGroep[i].Afblaasvent.AnaOut, 1)) return (0);
    if (Board_IO_Used(IO_new, &opt_app.LuchtmengkastGroep[i].Inblaasvent.AnaOut, 1)) return (0);
  }
  for (i = 0; i < MAX_LUCHTMENGKAST; i++)
  {
    if (Board_IO_Used(IO_new, &opt_app.Luchtmengkast[i].Afblaasvent.AnaOut, 1)) return (0);
    if (Board_IO_Used(IO_new, &opt_app.Luchtmengkast[i].Inblaasvent.AnaOut, 1)) return (0);
  }
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
  for (i = 0; i < MAX_LUCHTMENGKAST_GROEP; i++)
  {
    if (Board_IO_Used(IO_new, &opt_app.LuchtmengkastGroep[i].DigOutAlarm, 1)) return (0);
  }
  for (i = 0; i < MAX_LUCHTMENGKAST; i++)
  {
    if (Board_IO_Used(IO_new, &opt_app.Luchtmengkast[i].Afblaasvent.Open,  1)) return (0);
    if (Board_IO_Used(IO_new, &opt_app.Luchtmengkast[i].Afblaasvent.Close, 1)) return (0);
    if (Board_IO_Used(IO_new, &opt_app.Luchtmengkast[i].Inblaasvent.Open,  1)) return (0);
    if (Board_IO_Used(IO_new, &opt_app.Luchtmengkast[i].Inblaasvent.Close, 1)) return (0);
  }
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
    if (opt_app.Motor[i].GroupNumber != GroupIndex + 1)
    {
      if (Board_IO_Used(IO_new, &opt_app.Motor[i].IO, 1)) return (0);
    }
  }
  return (1);
}

//-----------------------------------------------------------------------------
static void Arrow_Type_Value(void)
{
  switch (key)
  {
    case LEFT:
      Enter_Type_Value();
      break;
    case RIGHT:
      Enter_Type_Value();
      break;
    case UP:
      Increment_Scroll_Option_Value_No_Enter();
	  while (((module.Ventilatie == 0) && (screen_ptr->value == TYPE_VENT)) || ((module.Klep == 0) && (screen_ptr->value == TYPE_KLEP)))
        Increment_Scroll_Option_Value_No_Enter();
      break;
    case DOWN:
      Decrement_Scroll_Option_Value_No_Enter();
	  while (((module.Ventilatie == 0) && (screen_ptr->value == TYPE_VENT)) || ((module.Klep == 0) && (screen_ptr->value == TYPE_KLEP)))
        Decrement_Scroll_Option_Value_No_Enter();
      break;
  }
}

static void Enter_Type_Value(void)
{
  Enter_Value();
  CheckOptions();
  SetDisplayOptions();
  GetMaxMotor();
  Refresh_Screen_Nr_Aantal();
  Increment_Func_Index();
}

//-----------------------------------------------------------------------------
static void Arrow_Value_Refresh_Screen(void)
{
  switch (key)
  {
    case LEFT:
      if (!screen_ptr->change_flag)
      {
        CheckOptions();
        SetDisplayOptions();
        Refresh_Screen_Nr_Aantal();
      }
      Arrow_Left_Value();
      break;
    case RIGHT:
      CheckOptions();
      SetDisplayOptions();
      Refresh_Screen_Nr_Aantal();
      Increment_Func_Index();
      break;
    case UP:
      Increment_Option_Value();
      break;
    case DOWN:
      Decrement_Option_Value();
      break;
  }
}

static void Arrow_Scroll_Value_Refresh_Screen(void)
{
  switch (key)
  {
    case LEFT:
      if (!screen_ptr->change_flag)
      {
        CheckOptions();
        SetDisplayOptions();
        Refresh_Screen_Nr_Aantal();
      }
      Arrow_Left_Value();
      break;
    case RIGHT:
      CheckOptions();
      SetDisplayOptions();
      Refresh_Screen_Nr_Aantal();
      Increment_Func_Index();
      break;
    case UP:
      Increment_Scroll_Option_Value();
      break;
    case DOWN:
      Decrement_Scroll_Option_Value();
      break;
  }
}

static void Enter_Value_Refresh_Screen(void)
{
  Enter_Value();
  CheckOptions();
  SetDisplayOptions();
  Refresh_Screen_Nr_Aantal();
  Increment_Func_Index();
}

//-----------------------------------------------------------------------------
static unsigned char RangeDeviceNumberOk(int Number)
{
int i;

  for (i = 0; i < opt_app.NumberMotorgroups; i++)
  {
    if (i != GroupIndex)
    {
      if (Number == opt_app.Motorgroup[i].FirstNumber)
        return (0);
      if ((Number < opt_app.Motorgroup[i].FirstNumber) && ((Number + opt_app.Motorgroup[GroupIndex].NumberMotors) > opt_app.Motorgroup[i].FirstNumber))
        return (0);
      if ((Number > opt_app.Motorgroup[i].FirstNumber) && (Number < ((int)opt_app.Motorgroup[i].FirstNumber + opt_app.Motorgroup[i].NumberMotors)))
        return (0);
    }
  }
  return (1);
}

static unsigned char RangeDeviceAddressOk(int Address)
{
int i;

  for (i = 0; i < opt_app.NumberMotorgroups; i++)
  {
    if (i != GroupIndex)
    {
      if (Address == opt_app.Motorgroup[i].FirstAddress)
        return (0);
      if ((Address < opt_app.Motorgroup[i].FirstAddress) && ((Address + opt_app.Motorgroup[GroupIndex].NumberMotors) > opt_app.Motorgroup[i].FirstAddress))
        return (0);
      if ((Address > opt_app.Motorgroup[i].FirstAddress) && (Address < ((int)opt_app.Motorgroup[i].FirstAddress + opt_app.Motorgroup[i].NumberMotors)))
        return (0);
    }
  }
  return (1);
}

static void Arrow_Range_Device_Number_Value(void)
{
long old_val = screen_ptr->value;

  switch (key)
  {
    case LEFT:
      if (!screen_ptr->change_flag)
      {
        CheckOptions();
        SetDisplayOptions();
        Refresh_Screen_Nr_Aantal();
      }
      Arrow_Left_Value();
      break;
    case RIGHT:
      CheckOptions();
      SetDisplayOptions();
      Refresh_Screen_Nr_Aantal();
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
      while (RangeDeviceNumberOk(screen_ptr->value) == 0);
      if (old_val != screen_ptr->value)
      {
        Put_Value();
        option_change_flag = 1;
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
      while (RangeDeviceNumberOk(screen_ptr->value) == 0);
      if (old_val != screen_ptr->value)
      {
        Put_Value();
        option_change_flag = 1;
      }
      break;
  }
}

static void Enter_Range_Device_Number_Value(void)
{
  if (screen_ptr->change_flag)
  {
    if ((screen_ptr->value >= screen_ptr->min_value) && (screen_ptr->value <= screen_ptr->max_value))
    {
      if (RangeDeviceNumberOk(screen_ptr->value))
      {
        Enter_Value();
        CheckOptions();
        SetDisplayOptions();
        Refresh_Screen_Nr_Aantal();
        option_change_flag = 1;
      }
    }
  }
  Increment_Func_Index();
}

static void Arrow_Range_Device_Address_Value(void)
{
long old_val = screen_ptr->value;

  switch (key)
  {
    case LEFT:
      if (!screen_ptr->change_flag)
      {
        CheckOptions();
        SetDisplayOptions();
        Refresh_Screen_Nr_Aantal();
      }
      Arrow_Left_Value();
      break;
    case RIGHT:
      CheckOptions();
      SetDisplayOptions();
      Refresh_Screen_Nr_Aantal();
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
      while (RangeDeviceAddressOk(screen_ptr->value) == 0);
      if (old_val != screen_ptr->value)
      {
        Put_Value();
        option_change_flag = 1;
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
      while (RangeDeviceAddressOk(screen_ptr->value) == 0);
      if (old_val != screen_ptr->value)
      {
        Put_Value();
        option_change_flag = 1;
      }
      break;
  }
}

static void Enter_Range_Device_Address_Value(void)
{
  if (screen_ptr->change_flag)
  {
    if ((screen_ptr->value >= screen_ptr->min_value) && (screen_ptr->value <= screen_ptr->max_value))
    {
      if (RangeDeviceAddressOk(screen_ptr->value))
      {
        Enter_Value();
        CheckOptions();
        SetDisplayOptions();
        Refresh_Screen_Nr_Aantal();
        option_change_flag = 1;
      }
    }
  }
  Increment_Func_Index();
}

//-----------------------------------------------------------------------------
static void Arrow_Type_Device_Value(void)
{
  switch (key)
  {
    case LEFT:
      if (!screen_ptr->change_flag)
      {
        CheckOptions();
        SetDisplayOptions();
        Refresh_Screen_Nr_Aantal();
      }
      Arrow_Left_Value();
      break;
    case RIGHT:
      CheckOptions();
      SetDisplayOptions();
      Refresh_Screen_Nr_Aantal();
      Increment_Func_Index();
      break;
    case UP:
      switch (screen_ptr->value)
      {
        case dtEbmBus         : screen_ptr->value = dtEbmModbus;       break;
        case dtEbmModbus      : screen_ptr->value = dtECblueModbus;    break;
        case dtECblueModbus   : screen_ptr->value = dtECbluePremium;   break;
        case dtECbluePremium  : screen_ptr->value = dtRosenberg;       break;
		case dtRosenberg      : screen_ptr->value = dtRosenbergGen3;   break;
		case dtRosenbergGen3  : screen_ptr->value = dtClimafan;        break;
		case dtClimafan       : screen_ptr->value = dtNicotraGebhardt; break;
		case dtNicotraGebhardt: screen_ptr->value = dtEbmBus;          break;
		default               : screen_ptr->value = dtEbmModbus;       break;
		// Use default(dtEbmBus) for dtBelimoModbus and dtDptMod
      }
      Put_Value();
      option_change_flag = 1;
      break;
    case DOWN:
      switch (screen_ptr->value)
      {
        case dtEbmBus         : screen_ptr->value = dtNicotraGebhardt; break;
        case dtEbmModbus      : screen_ptr->value = dtEbmBus;          break;
        case dtECblueModbus   : screen_ptr->value = dtEbmModbus;       break;
        case dtECbluePremium  : screen_ptr->value = dtECblueModbus;    break;
		case dtRosenberg      : screen_ptr->value = dtECbluePremium;   break;
		case dtRosenbergGen3  : screen_ptr->value = dtRosenberg;       break;
		case dtClimafan       : screen_ptr->value = dtRosenbergGen3;   break;
		case dtNicotraGebhardt: screen_ptr->value = dtClimafan;        break;
		default               : screen_ptr->value = dtEbmModbus;       break;
		// Use default(dtEbmBus) for dtBelimoModbus and dtDptMod 
      }
      Put_Value();
      option_change_flag = 1;
      break;
  }
}

static void Enter_Type_Device_Value(void)
{
  Enter_Value();
  CheckOptions();
  SetDisplayOptions();
  Refresh_Screen_Nr_Aantal();
  Increment_Func_Index();
}

//================================================================================
static void ChangeAddressCompleted(unsigned char Failure)
{
  if (Failure)
  {
    AdresWijzigenStatus  = 0;
    AdresWijzigenOk      = 0;
    AdresWijzigenMislukt = 1;
  }
  else
  {
    AdresWijzigenStatus  = 0;
    AdresWijzigenOk      = 1;
    AdresWijzigenMislukt = 0;
  }
  mbDeviceInit();
}

static void Arrow_Wijzig_Adres_Func(void)
{
  if (AdresWijzigenStatus == 0)
  {
    Arrow_Option_Func();
    AdresWijzigenOk      = 0;
    AdresWijzigenMislukt = 0;
  }
}

static void Arrow_Wijzig_Adres_Value(void)
{
  switch (key)
  {
    case LEFT:
      Arrow_Left_Value();
      break;
    case RIGHT:
      Increment_Func_Index();
      Increment_Func_Index();
      break;
    case UP:
      Increment_Value();
      break;
    case DOWN:
      Decrement_Value();
      break;
  }
}

static void Enter_Wijzig_Adres_Value(void)
{
  Increment_Func_Index_Enter_Value();
  if (OudAdres == NieuwAdres)
    Increment_Func_Index();
}

static void Enter_Wijzig_Adres_Func(void)
{
  Enter_Value();
  AdresWijzigenOk      = 0;
  AdresWijzigenMislukt = 0;
  AdresWijzigenStatus  = 1;
  if (mbDeviceChangeAddress(opt_app.Motorgroup[GroupIndex].BusType, OudAdres, NieuwAdres, opt_app.Ventgroup[GroupIndex].RS485Bus, 4, ChangeAddressCompleted) == 0)
  {
    AdresWijzigenStatus  = 0;
    AdresWijzigenOk      = 0;
    AdresWijzigenMislukt = 1;
  }    
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
                if (screen_ptr->index == 0)
                  val_hr_alg.Motorgroup[GroupIndex].PositionTime = Calc_Prop(0, 1000, 0, opt_app.Motorgroup[GroupIndex].Runtime, PositionManual);
                break;
    case RIGHT: Increment_Func_Index();
                if (screen_ptr->index == 0)
                  val_hr_alg.Motorgroup[GroupIndex].PositionTime = Calc_Prop(0, 1000, 0, opt_app.Motorgroup[GroupIndex].Runtime, PositionManual);
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
  if (screen_ptr->index == 0)
    val_hr_alg.Motorgroup[GroupIndex].PositionTime = Calc_Prop(0, 1000, 0, opt_app.Motorgroup[GroupIndex].Runtime, PositionManual);
}
                                
//-----------------------------------------------------------------------------
static void GetMaxMotor(void)
{
int MaxValue;
int i;
  
  switch (opt_app.Motorgroup[GroupIndex].Type)
  {
    case TYPE_RAAM:
    case TYPE_DOEK:
      MaxValue = MAX_MOTOR;
      for (i = 0; i < opt_app.NumberMotorgroups; i++)
      {
        if (i != GroupIndex)
          MaxValue -= opt_app.Motorgroup[i].NumberMotors;
      }
      if (MaxValue > 16)
        MaxValue = 16;
      MaxMotor = MaxValue;
      break;
    case TYPE_VENT:
      MaxValue = MAX_DEVICE;
      for (i = 0; i < opt_app.NumberMotorgroups; i++)
      {
        if (i != GroupIndex)
          MaxValue -= opt_app.Motorgroup[i].NumberMotors;
      }
      if (MaxValue > 255)
        MaxValue = 255;
      MaxMotor = MaxValue;
      break;
    case TYPE_KLEP:
      MaxValue = MAX_KLEP;
      for (i = 0; i < opt_app.NumberMotorgroups; i++)
      {
        if (i != GroupIndex)
          MaxValue -= opt_app.Motorgroup[i].NumberMotors;
      }
      if (MaxValue > 255)
        MaxValue = 255;
      MaxMotor = MaxValue;
      break;
  }
}

//-----------------------------------------------------------------------------
static void Arrow_IO_Device_Select_Func(void)
{
  Arrow_IO_Select_Func();
  if (screen_ptr->index == 0)
    mbDeviceInit();
}

static void Enter_IO_Device_Select_Func(void)
{
  Enter_IO_Select_Func();
  if (screen_ptr->index == 0)
    mbDeviceInit();
}

//-----------------------------------------------------------------------------
static unsigned char IO_Motor_Select_Nr_Used(unsigned char nr)
{
int loop;

  for (loop = 0; loop < board_IO_max; loop++)
  {
    if (board_IO[loop].on_off == nr)
      return (1);
  }  
  return (0);
}

static unsigned char IO_Motor_Select_First_Free(void)
{
unsigned char nr;

  switch (opt_app.Motorgroup[GroupIndex].Type)
  {
    case TYPE_VENT:
      for (nr = 0; nr < MAX_DEVICE; nr++)
      {
        if ((opt_app.Device[nr].GroupNumber == GroupIndex + 1) && !IO_Motor_Select_Nr_Used(nr + 1))
          return (nr + 1);
      }
      break;
    default:
      for (nr = 0; nr < MAX_MOTOR; nr++)
      {
        if ((opt_app.Motor[nr].GroupNumber == GroupIndex + 1) && !IO_Motor_Select_Nr_Used(nr + 1))
          return (nr + 1);
      }
      break;
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
    if (opt_app.Motor[i].Enabled && (opt_app.Motor[i].GroupNumber == GroupIndex + 1))
    {
      MotorIO[IO_index] = opt_app.Motor[i].IO;
      IO_index++;
    }
  }
}

static void CopyArrayToMotorIO(void)
{
int i;

  for (i = 0; i < board_IO_array_size; i++)
  {
    if ((board_IO[i].board_type != MotorIO[i].board_type) || (board_IO[i].board_nr != MotorIO[i].board_nr) || (board_IO[i].IO_nr != MotorIO[i].IO_nr) || (board_IO[i].on_off != MotorIO[i].on_off))
      option_change_flag = 1;
  }
  for (i = 0; i < MAX_MOTOR; i++)
  {
    if (opt_app.Motor[i].Enabled && (opt_app.Motor[i].GroupNumber == GroupIndex + 1))
      opt_app.Motor[i].IO = IO_empty;
  }
  for (i = 0; i < board_IO_max; i++)
  {
    if (board_IO[i].board_type && board_IO[i].on_off)
      opt_app.Motor[board_IO[i].on_off-1].IO = board_IO[i];
  }
}

//-----------------------------------------------------------------------------
static void Disp_Draw_Board_Motor_IO_Select(void *s)
{
static unsigned char copy = 0;
s_disp_board_IO_Selection *ptr;
s_board_IO_on_off *IO_ptr;
unsigned char max_nr;

  ptr = s;
  IO_ptr = Get_Ptr(ptr->IO, lcd_rel_disp_index);
  max_nr = *((unsigned char *)Get_Ptr(ptr->max_nr, lcd_rel_disp_index));
  if (index_array == 0)
  {
    if (copy)
    {
      CopyArrayToMotorIO();
      copy = 0;
    }
    else
    {
      CopyMotorIOToArray();
      Install_Copy_IO_To_Board_IO(IO_ptr, ptr->max, max_nr, ptr->on_off, ptr->IO_type, ptr->IO_type_sel, ptr->NotUsedFunc);
    }
  }
  else
  {
    copy = 1;
  }
  
  switch (ptr->IO_type)
  {
    case ANALOG_OUTPUT_ID:
      Disp_Draw_Bitmap(&disp_ana_output);
      if (board_IO_max == 0)
        LCD_Draw_Bitmap(29, 36, ico_IO_ana_out_array[ptr->IO_type_sel]);
      break;
    case MOTOR_CONTROL_ID:
      Disp_Draw_Bitmap(&disp_motor_control);
      if (board_IO_max == 0)
        LCD_Draw_Bitmap(29, 36, ico_IO_motor_control_array[ptr->IO_type_sel]);
      break;
  }
  Disp_Draw_Tekst_Array_L(&disp_NO_IO);
  
  Disp_Draw_Bitmap_Array(&disp_box_IO_pos1_bmp);
  Disp_Draw_Bitmap_Array(&disp_box_IO_pos2_bmp);
  Disp_Draw_Bitmap_Array(&disp_box_IO_pos3_bmp);
  Disp_Draw_Bitmap_Array(&disp_box_IO_pos4_bmp);
  Disp_Draw_Bitmap_Array(&disp_box_IO_pos5_bmp);
  Disp_Draw_Bitmap_Array(&disp_box_IO_pos6_bmp);
  Disp_Draw_Bitmap_Array(&disp_box_IO_pos7_bmp);
  Disp_Draw_Bitmap_Array(&disp_box_IO_pos8_bmp);
  Disp_Draw_Bitmap_Array(&disp_box_IO_pos9_bmp);
  Disp_Draw_Bitmap_Array(&disp_box_IO_pos10_bmp);
  Disp_Draw_Bitmap_Array(&disp_box_IO_pos11_bmp);
  Disp_Draw_Bitmap_Array(&disp_box_IO_pos12_bmp);
  Disp_Draw_Bitmap_Array(&disp_box_IO_pos13_bmp);
  Disp_Draw_Bitmap_Array(&disp_box_IO_pos14_bmp);
  Disp_Draw_Bitmap_Array(&disp_box_IO_pos15_bmp);
  Disp_Draw_Bitmap_Array(&disp_box_IO_pos16_bmp);
  
  Disp_Invert_Block(&disp_block);

  Disp_Draw_Value_2_Size_Option_On(&disp_IO_nr_pos1);
  Disp_Draw_Value_2_Size_Option_On(&disp_IO_nr_pos2);
  Disp_Draw_Value_2_Size_Option_On(&disp_IO_nr_pos3);
  Disp_Draw_Value_2_Size_Option_On(&disp_IO_nr_pos4);
  Disp_Draw_Value_2_Size_Option_On(&disp_IO_nr_pos5);
  Disp_Draw_Value_2_Size_Option_On(&disp_IO_nr_pos6);
  Disp_Draw_Value_2_Size_Option_On(&disp_IO_nr_pos7);
  Disp_Draw_Value_2_Size_Option_On(&disp_IO_nr_pos8);
  Disp_Draw_Value_2_Size_Option_On(&disp_IO_nr_pos9);
  Disp_Draw_Value_2_Size_Option_On(&disp_IO_nr_pos10);
  Disp_Draw_Value_2_Size_Option_On(&disp_IO_nr_pos11);
  Disp_Draw_Value_2_Size_Option_On(&disp_IO_nr_pos12);
  Disp_Draw_Value_2_Size_Option_On(&disp_IO_nr_pos13);
  Disp_Draw_Value_2_Size_Option_On(&disp_IO_nr_pos14);
  Disp_Draw_Value_2_Size_Option_On(&disp_IO_nr_pos15);
  Disp_Draw_Value_2_Size_Option_On(&disp_IO_nr_pos16);
  
  Disp_Draw_Tekst_L(&disp_board_str);
}

static void Disp_Draw_Board_Motor_IO_Select_Option_On(void *s)
{
s_disp_board_IO_Selection_option_on *ptr;

  ptr = s;
  if (Return_Value(ptr->option_type, Get_Ptr(ptr->option, lcd_rel_disp_index)))
    Disp_Draw_Board_Motor_IO_Select(s);
}

static void Disp_Draw_Board_Motor_IO_Select_Option_Off(void *s)
{
s_disp_board_IO_Selection_option_on *ptr;

  ptr = s;
  if (!Return_Value(ptr->option_type, Get_Ptr(ptr->option, lcd_rel_disp_index)))
    Disp_Draw_Board_Motor_IO_Select(s);
}

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

      Refresh_Screen_Nr_Aantal();
      break;
    case RIGHT:
      break;
  }
}

