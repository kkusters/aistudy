// C__DISP_OPT_LUCHTMENGKAST_2.C

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
#include "ch_mb_device.h"
#include "ch_IO.h"
#include "ch_key.h"
#include "ch_lcd_10.h"
#include "ch_lcd_14.h"
#include "ch_lcd_20.h"
#include "ch_lcd_bitmap.h"
#include "ch_lcd_hardware.h"
#include "ch_lcd_pixel.h"
#include "ch_luchtmengkast.h"
#include "ch_main.h"
#include "ch_string.h" 
#include "ch_disp_opt_luchtmengkast_2.h"

static void SetDisplayOptions(void);

static unsigned char DummyNotUsed(s_board_IO_on_off IO_new);

static unsigned char AnaInTempNotUsed(s_board_IO_on_off IO_new);
static unsigned char AnaInKlepNotUsed(s_board_IO_on_off IO_new);
static unsigned char DigInKlepNotUsed(s_board_IO_on_off IO_new);
static unsigned char AnaOutKlepNotUsed(s_board_IO_on_off IO_new);
static unsigned char DigOutKlepNotUsed(s_board_IO_on_off IO_new);
static unsigned char AnaInVerwarmNotUsed(s_board_IO_on_off IO_new);
static unsigned char DigInVerwarmNotUsed(s_board_IO_on_off IO_new);
static unsigned char AnaOutVerwarmNotUsed(s_board_IO_on_off IO_new);
static unsigned char DigOutVerwarmNotUsed(s_board_IO_on_off IO_new);
static unsigned char AnaInVentNotUsed(s_board_IO_on_off IO_new);
static unsigned char DigInVentNotUsed(s_board_IO_on_off IO_new);
static unsigned char AnaOutVentNotUsed(s_board_IO_on_off IO_new);
static unsigned char DigOutVentNotUsed(s_board_IO_on_off IO_new);
static unsigned char DigInAlarmNotUsed(s_board_IO_on_off IO_new);
static unsigned char DigOutAlarmNotUsed(s_board_IO_on_off IO_new);

static void Disp_Copy_Buitenklep_IO(void);
static void Disp_Copy_Binnenklep_IO(void);
static void Disp_Copy_Verwarming_IO(void);
static void Disp_Copy_Afblaasvent_IO(void);
static void Disp_Copy_Inblaasvent_IO(void);
static void Disp_Copy_Alarm_IO(void);

static void Disp_Copy_Mengtemp_IO(void);
static void Disp_Copy_Inblaastemp_IO(void);
static void Disp_Copy_Streeftemp_IO(void);
static void Disp_Copy_Vorst_IO(void);
static void Disp_Copy_Alarm_In_IO(void);
static void Disp_Copy_Drukverschil_IO(void);

static void Disp_Copy_Unit_Buitenklep_IO(void);
static void Disp_Copy_Unit_Binnenklep_IO(void);
static void Disp_Copy_Unit_Verwarming_IO(void);
static void Disp_Copy_Unit_Afblaasvent_IO(void);
static void Disp_Copy_Unit_Inblaasvent_IO(void);

static void Disp_Copy_Units_1(void);
static void Disp_Copy_Units_2(void);
static void Disp_Copy_Units_3(void);
static void Disp_Copy_Units_4(void);

static void Arrow_Type_Sturing_Value(void);
static void Arrow_Luchtmengkast_Vent_Value(void);
static void Enter_Luchtmengkast_Vent_Value(void);
static void Arrow_Luchtmengkast_Scroll_Value(void);
static void Arrow_Luchtmengkast_Value(void);
static void Enter_Luchtmengkast_Value(void);

static void Arrow_IO_Vent_Select_Func(void);
static void Enter_IO_Vent_Select_Func(void);

static void Disp_Adres_Inblaasvent_Func(void);
static void Disp_Adres_Afblaasvent_Func(void);

static void Arrow_Adres_Inblaasvent_Value(void);
static void Enter_Adres_Inblaasvent_Value(void);
static void Arrow_Adres_Afblaasvent_Value(void);
static void Enter_Adres_Afblaasvent_Value(void);

static void Arrow_Wijzig_Adres_Func(void);
static void Arrow_Wijzig_Adres_Value(void);
static void Enter_Wijzig_Adres_Value(void);
static void Enter_Wijzig_Adres_Func(void);

static void Arrow_End_Func(void);

//*****************************************************************************
// GLOBALS
//*****************************************************************************
static unsigned char const MaxAantalGroepen = MAX_LUCHTMENGKAST_GROEP;
static unsigned char const MaxAantalUnits   = 16; //MAX_LUCHTMENGKAST;
static unsigned char AantalGroepen;
static unsigned char AantalUnits;

static unsigned char SturingDigitaal;
static unsigned char SturingAnaloog;
static unsigned char SturingTerugmelding;

static unsigned char SturingBinnenBovenklepDigitaal;
static unsigned char SturingBinnenBovenklepAnaloog;
static unsigned char SturingBinnenBovenklepTerugmelding;
static unsigned char SturingAfblaasventDigitaal;
static unsigned char SturingAfblaasventAnaloog;
static unsigned char SturingAfblaasventTerugmelding;
static unsigned char SturingVerwarmingDigitaal;
static unsigned char SturingVerwarmingAnaloog;
static unsigned char SturingVerwarmingTerugmelding;

static unsigned char Naregelen;
static unsigned char DispNaregelen;

static unsigned char BuitenklepDigitaal;
static unsigned char BuitenklepAnaloog;
static unsigned char BinnenklepEnabled;
static unsigned char BinnenklepDigitaal;
static unsigned char BinnenklepAnaloog;
static unsigned char VerwarmingDigitaal;
static unsigned char VerwarmingAnaloog;
static unsigned char AfblaasventDigitaal;
static unsigned char AfblaasventAnaloog;
static unsigned char AfblaasventRS485;
static unsigned char AfblaasventTerugmelding;
static unsigned char InblaasventDigitaal;
static unsigned char InblaasventAnaloog;
static unsigned char InblaasventRS485;
static unsigned char InblaasventTerugmelding;

static unsigned char VentNummer;
static unsigned char VentAdres;

static unsigned char CopyTempIO;

static s_board_IO_on_off TempHoger[MAX_LUCHTMENGKAST];
static s_board_IO_on_off TempLager[MAX_LUCHTMENGKAST];
static s_board_IO_on_off TempAnaIn[MAX_LUCHTMENGKAST];
static s_board_IO_on_off TempAnaOut[MAX_LUCHTMENGKAST];
static s_board_IO_on_off TempDigOut[MAX_LUCHTMENGKAST];

static unsigned char GroepUnits[MAX_LUCHTMENGKAST];

static unsigned int OudAdres;
static unsigned int NieuwAdres;
static unsigned int const MaxAdres = MAX_MB_DEVICE;
static unsigned char AdresWijzigenOk;
static unsigned char AdresWijzigenMislukt;
static unsigned char AdresWijzigenStatus;
static unsigned char DispAdresWijzigen;

//***********************************************************************************************************
// SCHERM OPBOUW
//***********************************************************************************************************
//-----------------------------------------------------------------------------------------------------------
// Header
static s_disp_agri_header const disp_header = { Disp_Draw_Agri_Header, FN6, EMPTY, &tekst_inst.Opties_Luchtmengkast_10, HK_GEEN, 0, 3};

static void * const lcd_disp_header[] = { &disp_header, 0 };
//-----------------------------------------------------------------------------------------------------------
// Aantal luchtmengkast groepen
static s_disp_tekst const disp_aantal_groepen_str = { Disp_Draw_Tekst_L, 37, 22, &tekst_inst.Aantal_groepen_14 };
static s_disp_value const disp_aantal_groepen_val = { Disp_Draw_Value,  207, 75, (SIZE_14 | RECHTS), UCHAR, 0, &AantalGroepen };

static void * const lcd_aantal_groepen_disp[] = { &disp_luchtmengkast_ico, &disp_aantal_groepen_str, &disp_aantal_groepen_val, 0 };

static s_key_value const key_aantal_groepen_val = { UCHAR, 1, &AantalGroepen, &uchar_0, &MaxAantalGroepen };
//-----------------------------------------------------------------------------------------------------------
// Type klep: Binnen/buiten / Recirculatie
static void const * const tekst_binnenbuiten_recirculatie_14[] = { &tekst_inst.Binnen_buiten_14, &tekst_inst.Recirculatie_14 };
static s_disp_tekst       const disp_type_klep_str = { Disp_Draw_Tekst_L,       37, 22, &tekst_inst.Type_klep_14 };
static s_disp_tekst_array const disp_type_klep_val = { Disp_Draw_Tekst_Array_L, 76, 75, &tekst_binnenbuiten_recirculatie_14, UCHAR, &opt_app.LuchtmengkastGroep[0].TypeKlep, 2 };

static void * const lcd_type_klep_disp[] = { &disp_luchtmengkast_ico, &disp_type_klep_str, &disp_type_klep_val, 0 };

static s_key_value const key_type_klep_val = { UCHAR, 1, &opt_app.LuchtmengkastGroep[0].TypeKlep, &uchar_0, &uchar_1 };
//-----------------------------------------------------------------------------------------------------------
// Computersturing: Digitaal / Analoog
static s_disp_tekst       const disp_type_sturing_str = { Disp_Draw_Tekst_L,       37, 22, &tekst_inst.Type_sturing_14 };
static s_disp_tekst_array const disp_type_sturing_val = { Disp_Draw_Tekst_Array_L, 76, 75, &tekst_Digitaal_Analoog_CANopen_BACnet_14, UCHAR, &opt_app.LuchtmengkastGroep[0].Buitenklep.TypeSturing, 3 };

static void * const lcd_type_sturing_disp[] = { &disp_luchtmengkast_ico, &disp_type_sturing_str, &disp_type_sturing_val, 0 };

static s_key_value const key_type_sturing_val = { UCHAR, 1, &opt_app.LuchtmengkastGroep[0].Buitenklep.TypeSturing, &uchar_0, &uchar_2 };
//-----------------------------------------------------------------------------------------------------------
// Looptijd hoger/lager sturing
static s_disp_tekst const disp_looptijd_str = { Disp_Draw_Tekst_L,  37, 22, &tekst_inst.Looptijd_14 };
static s_disp_value const disp_looptijd_val = { Disp_Draw_Value,   207, 75, (SIZE_14 | RECHTS), INT, 1, &opt_app.LuchtmengkastGroep[0].Buitenklep.Runtime };
static s_disp_tekst const disp_looptijd_sec = { Disp_Draw_Tekst_L, 210, 75, &tekst.s_10 };

static void * const lcd_looptijd_disp[] = { &disp_luchtmengkast_ico, &disp_looptijd_str, &disp_looptijd_val, &disp_looptijd_sec, 0 };

static s_key_value const key_looptijd_val = { INT, 4, &opt_app.LuchtmengkastGroep[0].Buitenklep.Runtime, &int_0, &int_6000 };
//-----------------------------------------------------------------------------------------------------------
// Bovenklep
static s_disp_tekst        const disp_bovenklep_str = { Disp_Draw_Tekst_L,       37, 22, &tekst_inst.Bovenklep_14 };
static s_disp_bitmap_array const disp_bovenklep_val = { Disp_Draw_Bitmap_Array, 190, 57, bmp_false_true, UCHAR, &opt_app.LuchtmengkastGroep[0].BovenklepEnabled, 2 };

static void * const lcd_bovenklep_disp[] = { &disp_luchtmengkast_ico, &disp_bovenklep_str, &disp_bovenklep_val, 0 };

static s_key_value const key_bovenklep_val = { UCHAR, 1, &opt_app.LuchtmengkastGroep[0].BovenklepEnabled, &uchar_0, &uchar_1 };
//-----------------------------------------------------------------------------
// Bovenklep gekoppeld aan klep
static s_disp_tekst        const disp_gekoppeld_aan_klep_str  = { Disp_Draw_Tekst_L,       37, 22, &tekst_inst.Gekoppeld_aan_klep_14 };
static s_disp_bitmap_array const disp_bovenklep_gekoppeld_val = { Disp_Draw_Bitmap_Array, 190, 57, bmp_false_true, UCHAR, &opt_app.LuchtmengkastGroep[0].BovenklepGekoppeldAanKlep, 2 };

static void * const lcd_bovenklep_gekoppeld_disp[] = { &disp_luchtmengkast_ico, &disp_gekoppeld_aan_klep_str, &disp_bovenklep_gekoppeld_val, 0 };

static s_key_value const key_bovenklep_gekoppeld_val = { UCHAR, 1, &opt_app.LuchtmengkastGroep[0].BovenklepGekoppeldAanKlep, &uchar_0, &uchar_1 };
//-----------------------------------------------------------------------------
// Ingangen en uitgangen buitenklep/binnenklep/recirculatieklep/bovenklep - open/dicht of analoog
static void const * const tekst_buitenklep_recirculatieklep_14[] = { &tekst_inst.Buitenklep_14, &tekst_inst.Recirculatieklep_14 };
static void const * const tekst_binnenklep_bovenklep_14[]        = { &tekst_inst.Binnenklep_14, &tekst_inst.Bovenklep_14        };
static s_disp_tekst_array const disp_buitenklep_str = { Disp_Draw_Tekst_Array_L, 47, 22, &tekst_buitenklep_recirculatieklep_14, UCHAR, &opt_app.LuchtmengkastGroep[0].TypeKlep, 2 };
static s_disp_tekst_array const disp_binnenklep_str = { Disp_Draw_Tekst_Array_L, 47, 22, &tekst_binnenklep_bovenklep_14,        UCHAR, &opt_app.LuchtmengkastGroep[0].TypeKlep, 2 };
static s_disp_tekst_add   const disp_open_str       = { Disp_Draw_Tekst_Add_L, &tekst_inst.Open_14  };
static s_disp_tekst_add   const disp_dicht_str      = { Disp_Draw_Tekst_Add_L, &tekst_inst.Dicht_14 };
static s_disp_board_IO_Selection const disp_dig_in_klep_open_sel  = { Disp_Draw_Board_IO_Select, TempHoger,  MAX_LUCHTMENGKAST_GROEP, &AantalGroepen, 0, DIGITAL_INPUT_ID, DIG_IN_LAMEL,  DigInKlepNotUsed  };
static s_disp_board_IO_Selection const disp_dig_in_klep_dicht_sel = { Disp_Draw_Board_IO_Select, TempLager,  MAX_LUCHTMENGKAST_GROEP, &AantalGroepen, 0, DIGITAL_INPUT_ID, DIG_IN_LAMEL,  DigInKlepNotUsed  };
static s_disp_board_IO_Selection const disp_ana_in_klep_sel       = { Disp_Draw_Board_IO_Select, TempAnaIn,  MAX_LUCHTMENGKAST_GROEP, &AantalGroepen, 0, ANALOG_INPUT_ID,  ANA_IN_LAMEL,  AnaInKlepNotUsed  };
static s_disp_board_IO_Selection const disp_ana_out_klep_sel      = { Disp_Draw_Board_IO_Select, TempAnaOut, MAX_LUCHTMENGKAST_GROEP, &AantalGroepen, 0, ANALOG_OUTPUT_ID, ANA_OUT_LAMEL, AnaOutKlepNotUsed };

static s_disp_func const disp_copy_buitenklep_IO_func = { Disp_Control_Func, Disp_Copy_Buitenklep_IO };
static s_disp_func const disp_copy_binnenklep_IO_func = { Disp_Control_Func, Disp_Copy_Binnenklep_IO };

static void * const lcd_dig_in_buitenklep_open[]  = { &disp_copy_buitenklep_IO_func, &disp_luchtmengkast_ico, &disp_buitenklep_str, &disp_space_14_L, &disp_open_str,  &disp_dig_in_klep_open_sel,  0 };
static void * const lcd_dig_in_buitenklep_dicht[] = { &disp_copy_buitenklep_IO_func, &disp_luchtmengkast_ico, &disp_buitenklep_str, &disp_space_14_L, &disp_dicht_str, &disp_dig_in_klep_dicht_sel, 0 };
static void * const lcd_dig_in_binnenklep_open[]  = { &disp_copy_binnenklep_IO_func, &disp_luchtmengkast_ico, &disp_binnenklep_str, &disp_space_14_L, &disp_open_str,  &disp_dig_in_klep_open_sel,  0 };
static void * const lcd_dig_in_binnenklep_dicht[] = { &disp_copy_binnenklep_IO_func, &disp_luchtmengkast_ico, &disp_binnenklep_str, &disp_space_14_L, &disp_dicht_str, &disp_dig_in_klep_dicht_sel, 0 };
static void * const lcd_ana_in_buitenklep[]       = { &disp_copy_buitenklep_IO_func, &disp_luchtmengkast_ico, &disp_buitenklep_str, &disp_ana_in_klep_sel,  0 };
static void * const lcd_ana_in_binnenklep[]       = { &disp_copy_binnenklep_IO_func, &disp_luchtmengkast_ico, &disp_binnenklep_str, &disp_ana_in_klep_sel,  0 };
static void * const lcd_ana_out_buitenklep[]      = { &disp_copy_buitenklep_IO_func, &disp_luchtmengkast_ico, &disp_buitenklep_str, &disp_ana_out_klep_sel, 0 };
static void * const lcd_ana_out_binnenklep[]      = { &disp_copy_binnenklep_IO_func, &disp_luchtmengkast_ico, &disp_binnenklep_str, &disp_ana_out_klep_sel, 0 };
//-----------------------------------------------------------------------------------------------------------
// Afblaasvent
static s_disp_tekst        const disp_afblaasventilator_str = { Disp_Draw_Tekst_L,       37, 22, &tekst_inst.Afblaasvent_14 };
static s_disp_bitmap_array const disp_afblaasventilator_val = { Disp_Draw_Bitmap_Array, 190, 57, bmp_false_true, UCHAR, &opt_app.LuchtmengkastGroep[0].AfblaasventEnabled, 2 };

static void * const lcd_afblaasvent_disp[] = { &disp_luchtmengkast_ico, &disp_afblaasventilator_str, &disp_afblaasventilator_val, 0 };

static s_key_value const key_afblaasvent_val = { UCHAR, 1, &opt_app.LuchtmengkastGroep[0].AfblaasventEnabled, &uchar_0, &uchar_1 };
//-----------------------------------------------------------------------------
// Afblaasvent gekoppeld aan klep
static s_disp_bitmap_array const disp_afblaasvent_gekoppeld_val = { Disp_Draw_Bitmap_Array, 190, 57, bmp_false_true, UCHAR, &opt_app.LuchtmengkastGroep[0].AfblaasventGekoppeldAanKlep, 2 };

static void * const lcd_afblaasvent_gekoppeld_disp[] = { &disp_luchtmengkast_ico, &disp_gekoppeld_aan_klep_str, &disp_afblaasvent_gekoppeld_val, 0 };

static s_key_value const key_afblaasvent_gekoppeld_val = { UCHAR, 1, &opt_app.LuchtmengkastGroep[0].AfblaasventGekoppeldAanKlep, &uchar_0, &uchar_1 };
//-----------------------------------------------------------------------------
// Ingangen ventilatie - hoger/lager of analoog
static s_disp_tekst     const disp_afblaasvent_str = { Disp_Draw_Tekst_L, 47, 22, &tekst_inst.Afblaasvent_14 };
static s_disp_tekst     const disp_inblaasvent_str = { Disp_Draw_Tekst_L, 47, 22, &tekst_inst.Inblaasvent_14 };
static s_disp_tekst_add const disp_hoger_str       = { Disp_Draw_Tekst_Add_L, &tekst_inst.Hoger_14  };
static s_disp_tekst_add const disp_lager_str       = { Disp_Draw_Tekst_Add_L, &tekst_inst.Lager_14 };
static s_disp_board_IO_Selection const disp_dig_in_hoger_vent_sel = { Disp_Draw_Board_IO_Select, TempHoger,  MAX_LUCHTMENGKAST_GROEP, &AantalGroepen, 0, DIGITAL_INPUT_ID, DIG_IN_VENT,  DigInVentNotUsed  };
static s_disp_board_IO_Selection const disp_dig_in_lager_vent_sel = { Disp_Draw_Board_IO_Select, TempLager,  MAX_LUCHTMENGKAST_GROEP, &AantalGroepen, 0, DIGITAL_INPUT_ID, DIG_IN_VENT,  DigInVentNotUsed  };
static s_disp_board_IO_Selection const disp_ana_in_vent_sel       = { Disp_Draw_Board_IO_Select, TempAnaIn,  MAX_LUCHTMENGKAST_GROEP, &AantalGroepen, 0, ANALOG_INPUT_ID,  ANA_IN_VENT,  AnaInVentNotUsed  };
static s_disp_board_IO_Selection const disp_ana_out_vent_sel      = { Disp_Draw_Board_IO_Select, TempAnaOut, MAX_LUCHTMENGKAST_GROEP, &AantalGroepen, 0, ANALOG_OUTPUT_ID, ANA_OUT_VENT, AnaOutVentNotUsed };

static s_disp_func const disp_copy_inblaasvent_IO_func = { Disp_Control_Func, Disp_Copy_Inblaasvent_IO };
static s_disp_func const disp_copy_afblaasvent_IO_func = { Disp_Control_Func, Disp_Copy_Afblaasvent_IO };

static void * const lcd_dig_in_inblaasvent_hoger[] = { &disp_copy_inblaasvent_IO_func, &disp_luchtmengkast_ico, &disp_inblaasvent_str, &disp_space_14_L, &disp_hoger_str, &disp_dig_in_hoger_vent_sel, 0 };
static void * const lcd_dig_in_inblaasvent_lager[] = { &disp_copy_inblaasvent_IO_func, &disp_luchtmengkast_ico, &disp_inblaasvent_str, &disp_space_14_L, &disp_lager_str, &disp_dig_in_lager_vent_sel, 0 };
static void * const lcd_ana_in_inblaasvent[]       = { &disp_copy_inblaasvent_IO_func, &disp_luchtmengkast_ico, &disp_inblaasvent_str, &disp_ana_in_vent_sel,  0 };
static void * const lcd_ana_out_inblaasvent[]      = { &disp_copy_inblaasvent_IO_func, &disp_luchtmengkast_ico, &disp_inblaasvent_str, &disp_ana_out_vent_sel, 0 };

static void * const lcd_dig_in_afblaasvent_hoger[] = { &disp_copy_afblaasvent_IO_func, &disp_luchtmengkast_ico, &disp_afblaasvent_str, &disp_space_14_L, &disp_hoger_str, &disp_dig_in_hoger_vent_sel, 0 };
static void * const lcd_dig_in_afblaasvent_lager[] = { &disp_copy_afblaasvent_IO_func, &disp_luchtmengkast_ico, &disp_afblaasvent_str, &disp_space_14_L, &disp_lager_str, &disp_dig_in_lager_vent_sel, 0 };
static void * const lcd_ana_in_afblaasvent[]       = { &disp_copy_afblaasvent_IO_func, &disp_luchtmengkast_ico, &disp_afblaasvent_str, &disp_ana_in_vent_sel,  0 };
static void * const lcd_ana_out_afblaasvent[]      = { &disp_copy_afblaasvent_IO_func, &disp_luchtmengkast_ico, &disp_afblaasvent_str, &disp_ana_out_vent_sel, 0 };
//-----------------------------------------------------------------------------------------------------------
// Verwarming
static s_disp_tekst        const disp_verwarm_enabled_str = { Disp_Draw_Tekst_L,       37, 22, &tekst_inst.Verwarming_14 };
static s_disp_bitmap_array const disp_verwarm_enabled_val = { Disp_Draw_Bitmap_Array, 190, 57, bmp_false_true, UCHAR, &opt_app.LuchtmengkastGroep[0].VerwarmingEnabled, 2 };

static void * const lcd_verwarming_disp[] = { &disp_luchtmengkast_ico, &disp_verwarm_enabled_str, &disp_verwarm_enabled_val, 0 };

static s_key_value const key_verwarming_val = { UCHAR, 1, &opt_app.LuchtmengkastGroep[0].VerwarmingEnabled, &uchar_0, &uchar_1 };
//-----------------------------------------------------------------------------------------------------------
// Ingangen verwarming - open/dicht of analoog
static s_disp_tekst const disp_verwarming_str = { Disp_Draw_Tekst_L, 47, 22, &tekst_inst.Verwarming_14 };
static s_disp_board_IO_Selection const disp_dig_in_verwarm_open_sel  = { Disp_Draw_Board_IO_Select, TempHoger,  MAX_LUCHTMENGKAST_GROEP, &AantalGroepen, 0, DIGITAL_INPUT_ID, DIG_IN_VERWARMING,  DigInVerwarmNotUsed  };
static s_disp_board_IO_Selection const disp_dig_in_verwarm_dicht_sel = { Disp_Draw_Board_IO_Select, TempLager,  MAX_LUCHTMENGKAST_GROEP, &AantalGroepen, 0, DIGITAL_INPUT_ID, DIG_IN_VERWARMING,  DigInVerwarmNotUsed  };
static s_disp_board_IO_Selection const disp_ana_in_verwarm_sel       = { Disp_Draw_Board_IO_Select, TempAnaIn,  MAX_LUCHTMENGKAST_GROEP, &AantalGroepen, 0, ANALOG_INPUT_ID,  ANA_IN_VERWARMING,  AnaInVerwarmNotUsed  };
static s_disp_board_IO_Selection const disp_ana_out_verwarm_sel      = { Disp_Draw_Board_IO_Select, TempAnaOut, MAX_LUCHTMENGKAST_GROEP, &AantalGroepen, 0, ANALOG_OUTPUT_ID, ANA_OUT_VERWARMING, AnaOutVerwarmNotUsed };

static s_disp_func const disp_copy_verwarming_IO_func = { Disp_Control_Func, Disp_Copy_Verwarming_IO };

static void * const lcd_dig_in_verwarming_open[]  = { &disp_copy_verwarming_IO_func, &disp_luchtmengkast_ico, &disp_verwarming_str, &disp_space_14_L, &disp_open_str,  &disp_dig_in_verwarm_open_sel,  0 };
static void * const lcd_dig_in_verwarming_dicht[] = { &disp_copy_verwarming_IO_func, &disp_luchtmengkast_ico, &disp_verwarming_str, &disp_space_14_L, &disp_dicht_str, &disp_dig_in_verwarm_dicht_sel, 0 };
static void * const lcd_ana_in_verwarming[]       = { &disp_copy_verwarming_IO_func, &disp_luchtmengkast_ico, &disp_verwarming_str, &disp_ana_in_verwarm_sel, 0 };
static void * const lcd_ana_out_verwarming[]      = { &disp_copy_verwarming_IO_func, &disp_luchtmengkast_ico, &disp_verwarming_str, &disp_ana_out_verwarm_sel,0 };
//-----------------------------------------------------------------------------
// Digitale uitgang alarm
static s_disp_bitmap const disp_alarm_ico = { Disp_Draw_Bitmap,   9, 10, &ico_alarm };
static s_disp_tekst  const disp_alarm_str = { Disp_Draw_Tekst_L, 47, 22, &tekst_inst.Alarm_Contact_14 };
static s_disp_board_IO_Selection const disp_dig_out_alarm_sel = { Disp_Draw_Board_IO_Select, TempDigOut, MAX_LUCHTMENGKAST_GROEP, &AantalGroepen, 0, DIGITAL_OUTPUT_ID, DIG_OUT_ALARM, DigOutAlarmNotUsed };

static s_disp_func const disp_copy_alarm_IO_func = { Disp_Control_Func, Disp_Copy_Alarm_IO };

static void * const lcd_dig_out_alarm[] = { &disp_copy_alarm_IO_func, &disp_alarm_ico, &disp_alarm_str, &disp_dig_out_alarm_sel, 0 };
//-----------------------------------------------------------------------------
// Aantal units
static s_disp_tekst const disp_aantal_units_str = { Disp_Draw_Tekst_L, 37, 22, &tekst_inst.Aantal_units_14 };
static s_disp_value const disp_aantal_units_val = { Disp_Draw_Value,  207, 75, (SIZE_14 | RECHTS), UCHAR, 0, &AantalUnits };

static void * const lcd_aantal_units_disp[] = { &disp_luchtmengkast_ico, &disp_aantal_units_str, &disp_aantal_units_val, 0 };

static s_key_value const key_aantal_units_val = { UCHAR, 2, &AantalUnits, &uchar_0, &MaxAantalUnits };
//-----------------------------------------------------------------------------------------------------------
// Temperatuur ingangen - inblaas / meng
static s_disp_tekst const disp_inblaastemp_str = { Disp_Draw_Tekst_L, 47, 22, &tekst_inst.Inblaastemp_14 };
static s_disp_tekst const disp_mengtemp_str  = { Disp_Draw_Tekst_L, 47, 22, &tekst_inst.Mengtemp_14  };
static s_disp_board_IO_Selection const disp_ana_in_temp_sel = { Disp_Draw_Board_IO_Select, TempAnaIn, MAX_LUCHTMENGKAST, &AantalUnits, 0, ANALOG_INPUT_ID, ANA_IN_TEMP, AnaInTempNotUsed };

static s_disp_func const disp_copy_inblaastemp_IO_func = { Disp_Control_Func, Disp_Copy_Inblaastemp_IO };
static s_disp_func const disp_copy_mengtemp_IO_func  = { Disp_Control_Func, Disp_Copy_Mengtemp_IO  };

static void * const lcd_ana_in_inblaastemp[] = { &disp_copy_inblaastemp_IO_func, &disp_luchtmengkast_ico, &disp_inblaastemp_str, &disp_ana_in_temp_sel, 0 };
static void * const lcd_ana_in_mengtemp[]  = { &disp_copy_mengtemp_IO_func,  &disp_luchtmengkast_ico, &disp_mengtemp_str,  &disp_ana_in_temp_sel, 0 };
//-----------------------------------------------------------------------------------------------------------
// Vorstbewaking
static s_disp_tekst const disp_vorstbewaking_str = { Disp_Draw_Tekst_L, 47, 22, &tekst_inst.Vorstbewaking_14 };
static s_disp_board_IO_Selection const disp_dig_in_vorst_sel = { Disp_Draw_Board_IO_Select, TempAnaIn, MAX_LUCHTMENGKAST, &AantalUnits, 0, DIGITAL_INPUT_ID, DIG_IN_VORST, DummyNotUsed };

static s_disp_func const disp_copy_vorst_IO_func = { Disp_Control_Func, Disp_Copy_Vorst_IO };

static void * const lcd_dig_in_vorst[] = { &disp_copy_vorst_IO_func, &disp_luchtmengkast_ico, &disp_vorstbewaking_str, &disp_dig_in_vorst_sel, 0 };
//-----------------------------------------------------------------------------------------------------------
// Extern alarm
static s_disp_tekst const disp_alarm_ingang_str = { Disp_Draw_Tekst_L, 47, 22, &tekst_inst.Alarm_ingang_14 };
static s_disp_board_IO_Selection const disp_dig_in_alarm_sel = { Disp_Draw_Board_IO_Select, TempAnaIn, MAX_LUCHTMENGKAST, &AantalUnits, 0, DIGITAL_INPUT_ID, DIG_IN_ALARM, DigInAlarmNotUsed };

static s_disp_func const disp_copy_alarm_in_IO_func = { Disp_Control_Func, Disp_Copy_Alarm_In_IO };

static void * const lcd_dig_in_alarm[] = { &disp_copy_alarm_in_IO_func, &disp_luchtmengkast_ico, &disp_alarm_ingang_str, &disp_dig_in_alarm_sel, 0 };
//-----------------------------------------------------------------------------------------------------------
// Drukverschilschakelaar
static s_disp_tekst const disp_drukverschil_str = { Disp_Draw_Tekst_L, 47, 22, &tekst_inst.Drukverschil_14 };
static s_disp_board_IO_Selection const disp_dig_in_drukverschil_sel = { Disp_Draw_Board_IO_Select, TempAnaIn, MAX_LUCHTMENGKAST, &AantalUnits, 0, DIGITAL_INPUT_ID, DIG_IN_ALARM, DigInAlarmNotUsed };

static s_disp_func const disp_copy_drukverschil_IO_func = { Disp_Control_Func, Disp_Copy_Drukverschil_IO };

static void * const lcd_dig_in_drukverschil[] = { &disp_copy_drukverschil_IO_func, &disp_luchtmengkast_ico, &disp_drukverschil_str, &disp_dig_in_drukverschil_sel, 0 };
//-----------------------------------------------------------------------------------------------------------
// Ingangen en uitgangen buitenklep/binnenklep/recirculatieklep/bovenklep - open/dicht of analoog
static s_disp_tekst_array const disp_type_buitenklep_val = { Disp_Draw_Tekst_Array_L, 76, 75, &tekst_digitaal_analoog_14, UCHAR, &opt_app.Luchtmengkast[0].Buitenklep.TypeSturing, 2 };
static s_disp_tekst_array const disp_type_binnenklep_val = { Disp_Draw_Tekst_Array_L, 76, 75, &tekst_digitaal_analoog_14, UCHAR, &opt_app.Luchtmengkast[0].Binnenklep.TypeSturing, 2 };

static s_disp_board_IO_Selection const disp_dig_out_klep_open_sel  = { Disp_Draw_Board_IO_Select, TempHoger,  MAX_LUCHTMENGKAST, &AantalUnits, 0, DIGITAL_OUTPUT_ID, DIG_OUT_LAMEL, DigOutKlepNotUsed };
static s_disp_board_IO_Selection const disp_dig_out_klep_dicht_sel = { Disp_Draw_Board_IO_Select, TempLager,  MAX_LUCHTMENGKAST, &AantalUnits, 0, DIGITAL_OUTPUT_ID, DIG_OUT_LAMEL, DigOutKlepNotUsed };
static s_disp_board_IO_Selection const disp_unit_ana_in_klep_sel   = { Disp_Draw_Board_IO_Select, TempAnaIn,  MAX_LUCHTMENGKAST, &AantalUnits, 0, ANALOG_INPUT_ID,   ANA_IN_LAMEL,  AnaInKlepNotUsed  };
static s_disp_board_IO_Selection const disp_unit_ana_out_klep_sel  = { Disp_Draw_Board_IO_Select, TempAnaOut, MAX_LUCHTMENGKAST, &AantalUnits, 0, ANALOG_OUTPUT_ID,  ANA_OUT_LAMEL, AnaOutKlepNotUsed };

static s_disp_func const disp_copy_unit_buitenklep_IO_func = { Disp_Control_Func, Disp_Copy_Unit_Buitenklep_IO };
static s_disp_func const disp_copy_unit_binnenklep_IO_func = { Disp_Control_Func, Disp_Copy_Unit_Binnenklep_IO };

static void * const lcd_type_buitenklep_disp[] = { &disp_luchtmengkast_ico, &disp_buitenklep_str, &disp_type_buitenklep_val, 0 };
static void * const lcd_type_binnenklep_disp[] = { &disp_luchtmengkast_ico, &disp_binnenklep_str, &disp_type_binnenklep_val, 0 };

static void * const lcd_dig_out_buitenklep_open[]  = { &disp_copy_unit_buitenklep_IO_func, &disp_luchtmengkast_ico, &disp_buitenklep_str, &disp_space_14_L, &disp_open_str,  &disp_dig_out_klep_open_sel,  0 };
static void * const lcd_dig_out_buitenklep_dicht[] = { &disp_copy_unit_buitenklep_IO_func, &disp_luchtmengkast_ico, &disp_buitenklep_str, &disp_space_14_L, &disp_dicht_str, &disp_dig_out_klep_dicht_sel, 0 };
static void * const lcd_dig_out_binnenklep_open[]  = { &disp_copy_unit_binnenklep_IO_func, &disp_luchtmengkast_ico, &disp_binnenklep_str, &disp_space_14_L, &disp_open_str,  &disp_dig_out_klep_open_sel,  0 };
static void * const lcd_dig_out_binnenklep_dicht[] = { &disp_copy_unit_binnenklep_IO_func, &disp_luchtmengkast_ico, &disp_binnenklep_str, &disp_space_14_L, &disp_dicht_str, &disp_dig_out_klep_dicht_sel, 0 };
static void * const lcd_unit_ana_in_buitenklep[]   = { &disp_copy_unit_buitenklep_IO_func, &disp_luchtmengkast_ico, &disp_buitenklep_str, &disp_unit_ana_in_klep_sel,  0 };
static void * const lcd_unit_ana_in_binnenklep[]   = { &disp_copy_unit_binnenklep_IO_func, &disp_luchtmengkast_ico, &disp_binnenklep_str, &disp_unit_ana_in_klep_sel,  0 };
static void * const lcd_unit_ana_out_buitenklep[]  = { &disp_copy_unit_buitenklep_IO_func, &disp_luchtmengkast_ico, &disp_buitenklep_str, &disp_unit_ana_out_klep_sel, 0 };
static void * const lcd_unit_ana_out_binnenklep[]  = { &disp_copy_unit_binnenklep_IO_func, &disp_luchtmengkast_ico, &disp_binnenklep_str, &disp_unit_ana_out_klep_sel, 0 };

static s_key_value const key_type_buitenklep_val = { UCHAR, 1, &opt_app.Luchtmengkast[0].Buitenklep.TypeSturing, &uchar_0, &uchar_1 };
static s_key_value const key_type_binnenklep_val = { UCHAR, 1, &opt_app.Luchtmengkast[0].Binnenklep.TypeSturing, &uchar_0, &uchar_1 };
//-----------------------------------------------------------------------------------------------------------
// Ingangen en uitgangen ventilatie - hoger/lager of analoog of ebmBus of modBus
static s_disp_tekst_array const disp_type_inblaasvent_val = { Disp_Draw_Tekst_Array_L, 46, 75, &tekst_digitaal_analoog_ebmBus_modBus_14, UCHAR, &opt_app.Luchtmengkast[0].Inblaasvent.TypeSturing, TYPE_STURING_COUNT };
static s_disp_tekst_array const disp_type_afblaasvent_val = { Disp_Draw_Tekst_Array_L, 46, 75, &tekst_digitaal_analoog_ebmBus_modBus_14, UCHAR, &opt_app.Luchtmengkast[0].Afblaasvent.TypeSturing, TYPE_STURING_COUNT };

static s_disp_board_IO_Selection const disp_dig_out_hoger_vent_sel = { Disp_Draw_Board_IO_Select, TempHoger,  MAX_LUCHTMENGKAST, &AantalUnits, 0, DIGITAL_OUTPUT_ID, DIG_OUT_VENT, DigOutVentNotUsed };
static s_disp_board_IO_Selection const disp_dig_out_lager_vent_sel = { Disp_Draw_Board_IO_Select, TempLager,  MAX_LUCHTMENGKAST, &AantalUnits, 0, DIGITAL_OUTPUT_ID, DIG_OUT_VENT, DigOutVentNotUsed };
static s_disp_board_IO_Selection const disp_unit_ana_in_vent_sel   = { Disp_Draw_Board_IO_Select, TempAnaIn,  MAX_LUCHTMENGKAST, &AantalUnits, 0, ANALOG_INPUT_ID,   ANA_IN_VENT,  AnaInVentNotUsed  };
static s_disp_board_IO_Selection const disp_unit_ana_out_vent_sel  = { Disp_Draw_Board_IO_Select, TempAnaOut, MAX_LUCHTMENGKAST, &AantalUnits, 0, ANALOG_OUTPUT_ID,  ANA_OUT_VENT, AnaOutVentNotUsed };
static s_disp_board_IO_Selection const disp_unit_RS485_vent_sel    = { Disp_Draw_Board_IO_Select, TempAnaIn,  MAX_LUCHTMENGKAST, &AantalUnits, 1, RS485_BUS_ID,      0,            DummyNotUsed  };

static s_disp_func const disp_copy_unit_inblaasvent_IO_func = { Disp_Control_Func, Disp_Copy_Unit_Inblaasvent_IO };
static s_disp_func const disp_copy_unit_afblaasvent_IO_func = { Disp_Control_Func, Disp_Copy_Unit_Afblaasvent_IO };

static void * const lcd_type_inblaasvent_disp[] = { &disp_luchtmengkast_ico, &disp_inblaasvent_str, &disp_type_inblaasvent_val, 0 };
static void * const lcd_type_afblaasvent_disp[] = { &disp_luchtmengkast_ico, &disp_afblaasvent_str, &disp_type_afblaasvent_val, 0 };

static void * const lcd_dig_out_inblaasvent_hoger[] = { &disp_copy_unit_inblaasvent_IO_func, &disp_luchtmengkast_ico, &disp_inblaasvent_str, &disp_space_14_L, &disp_hoger_str, &disp_dig_out_hoger_vent_sel, 0 };
static void * const lcd_dig_out_inblaasvent_lager[] = { &disp_copy_unit_inblaasvent_IO_func, &disp_luchtmengkast_ico, &disp_inblaasvent_str, &disp_space_14_L, &disp_lager_str, &disp_dig_out_lager_vent_sel, 0 };
static void * const lcd_unit_ana_in_inblaasvent[]   = { &disp_copy_unit_inblaasvent_IO_func, &disp_luchtmengkast_ico, &disp_inblaasvent_str, &disp_unit_ana_in_vent_sel,  0 };
static void * const lcd_unit_ana_out_inblaasvent[]  = { &disp_copy_unit_inblaasvent_IO_func, &disp_luchtmengkast_ico, &disp_inblaasvent_str, &disp_unit_ana_out_vent_sel, 0 };
static void * const lcd_unit_RS485_inblaasvent[]    = { &disp_copy_unit_inblaasvent_IO_func, &disp_luchtmengkast_ico, &disp_inblaasvent_str, &disp_unit_RS485_vent_sel,   0 };

static void * const lcd_dig_out_afblaasvent_hoger[] = { &disp_copy_unit_afblaasvent_IO_func, &disp_luchtmengkast_ico, &disp_afblaasvent_str, &disp_space_14_L, &disp_hoger_str, &disp_dig_out_hoger_vent_sel, 0 };
static void * const lcd_dig_out_afblaasvent_lager[] = { &disp_copy_unit_afblaasvent_IO_func, &disp_luchtmengkast_ico, &disp_afblaasvent_str, &disp_space_14_L, &disp_lager_str, &disp_dig_out_lager_vent_sel, 0 };
static void * const lcd_unit_ana_in_afblaasvent[]   = { &disp_copy_unit_afblaasvent_IO_func, &disp_luchtmengkast_ico, &disp_afblaasvent_str, &disp_unit_ana_in_vent_sel,  0 };
static void * const lcd_unit_ana_out_afblaasvent[]  = { &disp_copy_unit_afblaasvent_IO_func, &disp_luchtmengkast_ico, &disp_afblaasvent_str, &disp_unit_ana_out_vent_sel, 0 };
static void * const lcd_unit_RS485_afblaasvent[]    = { &disp_copy_unit_afblaasvent_IO_func, &disp_luchtmengkast_ico, &disp_afblaasvent_str, &disp_unit_RS485_vent_sel,   0 };

static s_key_value const key_type_afblaasvent_val = { UCHAR, 1, &opt_app.Luchtmengkast[0].Afblaasvent.TypeSturing, &uchar_0, &uchar_6 };
static s_key_value const key_type_inblaasvent_val = { UCHAR, 1, &opt_app.Luchtmengkast[0].Inblaasvent.TypeSturing, &uchar_0, &uchar_6 };
//-----------------------------------------------------------------------------------------------------------
// Operation Mode - ClosedLoop / OpenLoop
static void const * const tekst_closed_loop_open_loop_14[] = { &tekst_inst.Closed_Loop_14, &tekst_inst.Open_Loop_14 };
static s_disp_tekst_array const disp_operation_mode_inblaasvent_val = { Disp_Draw_Tekst_Array_L, 46, 75, &tekst_closed_loop_open_loop_14, UCHAR, &opt_app.LuchtmengkastInblaasventClosedLoopOpenLoop, 2 };
static s_disp_tekst_array const disp_operation_mode_afblaasvent_val = { Disp_Draw_Tekst_Array_L, 46, 75, &tekst_closed_loop_open_loop_14, UCHAR, &opt_app.LuchtmengkastAfblaasventClosedLoopOpenLoop, 2 };

static void * const lcd_operation_mode_inblaasvent_disp[] = { &disp_luchtmengkast_ico, &disp_inblaasvent_str, &disp_operation_mode_inblaasvent_val, 0 };
static void * const lcd_operation_mode_afblaasvent_disp[] = { &disp_luchtmengkast_ico, &disp_afblaasvent_str, &disp_operation_mode_afblaasvent_val, 0 };

static s_key_value const key_operation_mode_inblaasvent_val = { UCHAR, 1, &opt_app.LuchtmengkastInblaasventClosedLoopOpenLoop, &uchar_0, &uchar_1 };
static s_key_value const key_operation_mode_afblaasvent_val = { UCHAR, 1, &opt_app.LuchtmengkastAfblaasventClosedLoopOpenLoop, &uchar_0, &uchar_1 };
//-----------------------------------------------------------------------------------------------------------
// Adresseren ventilatoren
static s_disp_tekst     const disp_adres_str  = { Disp_Draw_Tekst_L, 47, 41, &tekst_inst.Adres_14 };
static s_disp_value     const disp_adres_val  = { Disp_Draw_Value,  207, 75, (SIZE_14 | RECHTS), UCHAR, 0, &VentAdres  };
static s_disp_value_add const disp_nummer_val = { Disp_Draw_Value_Add,       (SIZE_14 | LINKS ), UCHAR, 0, &VentNummer };

static s_disp_func const disp_adres_inblaasvent_func = { Disp_Control_Func, Disp_Adres_Inblaasvent_Func };
static s_disp_func const disp_adres_afblaasvent_func  = { Disp_Control_Func, Disp_Adres_Afblaasvent_Func   };

static void * const lcd_adres_inblaasvent[] = { &disp_adres_inblaasvent_func, &disp_luchtmengkast_ico, &disp_RS485_bus, &disp_inblaasvent_str, &disp_rechte_openings_haak_14_L, &disp_nummer_val, &disp_rechte_sluit_haak_14_L, &disp_adres_str, &disp_adres_val, 0 };
static void * const lcd_adres_afblaasvent[]  = { &disp_adres_afblaasvent_func,  &disp_luchtmengkast_ico, &disp_RS485_bus, &disp_afblaasvent_str,  &disp_rechte_openings_haak_14_L, &disp_nummer_val, &disp_rechte_sluit_haak_14_L, &disp_adres_str, &disp_adres_val, 0 };

static s_key_value const key_vent_adres_val = { UCHAR, 3, &VentAdres, &uchar_0, &uchar_255 };
//-----------------------------------------------------------------------------------------------------------
// Wijzig adres
static s_disp_tekst            const disp_wijzig_adres_str     = { Disp_Draw_Tekst_L,            37, 22, &tekst_inst.Wijzig_adres_14 };
static s_disp_tekst_option_on  const disp_wijzigen_gelukt_str  = { Disp_Draw_Tekst_L_Option_On,  37, 42, &tekst_inst.gewijzigd_10,         UCHAR, &AdresWijzigenOk      };
static s_disp_tekst_option_on  const disp_wijzigen_mislukt_str = { Disp_Draw_Tekst_L_Option_On,  37, 42, &tekst_inst.communicatie_fout_10, UCHAR, &AdresWijzigenMislukt };
static s_disp_value            const disp_oud_adres_val        = { Disp_Draw_Value,             147, 75, (SIZE_14 | RECHTS), UINT,  0, &OudAdres   };
static s_disp_value            const disp_nieuw_adres_val      = { Disp_Draw_Value,             207, 75, (SIZE_14 | RECHTS), UINT,  0, &NieuwAdres };
static s_disp_bitmap           const disp_pijl_rechts_ico      = { Disp_Draw_Bitmap,            157, 62, &ico_pijl_rechts };
static s_disp_bitmap_option_on const disp_wijzigen_status_bmp  = { Disp_Draw_Bitmap_Option_On,   90, 40, &zandloper_ico, UCHAR, &AdresWijzigenStatus };

static void * const lcd_wijzig_adres_disp[]    = { &disp_vent_ico_17x17, &disp_wijzig_adres_str, &disp_oud_adres_val, &disp_pijl_rechts_ico, &disp_nieuw_adres_val,
                                                   &disp_wijzigen_gelukt_str, &disp_wijzigen_mislukt_str, &disp_wijzigen_status_bmp, 0 };
static void * const lcd_wijzig_adres_ok_disp[] = { &disp_vent_ico_17x17, &disp_wijzig_adres_str, &disp_oud_adres_val, &disp_pijl_rechts_ico, &disp_nieuw_adres_val, &disp_messagebox_bevestig,
                                                   &disp_zeker_weten_inst_str, &disp_ok_str, &disp_space_10_L, &disp_is_teken_10_L, &disp_space_10_L, &disp_wijzigen_str, 0 };

static s_key_value const key_oud_adres_val   = { UINT, 3, &OudAdres,   &uint_1, &MaxAdres };
static s_key_value const key_nieuw_adres_val = { UINT, 3, &NieuwAdres, &uint_1, &MaxAdres };
//-----------------------------------------------------------------------------
// Ingangen en uitgangen verwarming - open/dicht of analoog
static s_disp_tekst_array const disp_type_verwarming_val = { Disp_Draw_Tekst_Array_L, 76, 75, &tekst_digitaal_analoog_14, UCHAR, &opt_app.Luchtmengkast[0].Verwarming.TypeSturing, 2 };

static s_disp_board_IO_Selection const disp_dig_out_verwarm_open_sel  = { Disp_Draw_Board_IO_Select, TempHoger,  MAX_LUCHTMENGKAST, &AantalUnits, 0, DIGITAL_OUTPUT_ID, DIG_OUT_VERWARMING, DigOutVerwarmNotUsed };
static s_disp_board_IO_Selection const disp_dig_out_verwarm_dicht_sel = { Disp_Draw_Board_IO_Select, TempLager,  MAX_LUCHTMENGKAST, &AantalUnits, 0, DIGITAL_OUTPUT_ID, DIG_OUT_VERWARMING, DigOutVerwarmNotUsed };
static s_disp_board_IO_Selection const disp_unit_ana_in_verwarm_sel   = { Disp_Draw_Board_IO_Select, TempAnaIn,  MAX_LUCHTMENGKAST, &AantalUnits, 0, ANALOG_INPUT_ID,   ANA_IN_VERWARMING,  AnaInVerwarmNotUsed  };
static s_disp_board_IO_Selection const disp_unit_ana_out_verwarm_sel  = { Disp_Draw_Board_IO_Select, TempAnaOut, MAX_LUCHTMENGKAST, &AantalUnits, 0, ANALOG_OUTPUT_ID,  ANA_OUT_VERWARMING, AnaOutVerwarmNotUsed };

static s_disp_func const disp_copy_unit_verwarming_IO_func = { Disp_Control_Func, Disp_Copy_Unit_Verwarming_IO };

static void * const lcd_type_verwarming_disp[] = { &disp_luchtmengkast_ico, &disp_verwarming_str, &disp_type_verwarming_val, 0 };

static void * const lcd_dig_out_verwarming_open[]  = { &disp_copy_unit_verwarming_IO_func, &disp_luchtmengkast_ico, &disp_verwarming_str, &disp_space_14_L, &disp_open_str,  &disp_dig_out_verwarm_open_sel,  0 };
static void * const lcd_dig_out_verwarming_dicht[] = { &disp_copy_unit_verwarming_IO_func, &disp_luchtmengkast_ico, &disp_verwarming_str, &disp_space_14_L, &disp_dicht_str, &disp_dig_out_verwarm_dicht_sel, 0 };
static void * const lcd_unit_ana_in_verwarming[]   = { &disp_copy_unit_verwarming_IO_func, &disp_luchtmengkast_ico, &disp_verwarming_str, &disp_unit_ana_in_verwarm_sel, 0 };
static void * const lcd_unit_ana_out_verwarming[]  = { &disp_copy_unit_verwarming_IO_func, &disp_luchtmengkast_ico, &disp_verwarming_str, &disp_unit_ana_out_verwarm_sel,0 };

static s_key_value const key_type_verwarming_val = { UCHAR, 1, &opt_app.Luchtmengkast[0].Verwarming.TypeSturing, &uchar_0, &uchar_1 };
//-----------------------------------------------------------------------------------------------------------
// Naregelen
static s_disp_tekst        const disp_naregelen_str = { Disp_Draw_Tekst_L,       37, 22, &tekst_inst.Naregelen_14 };
static s_disp_bitmap_array const disp_naregelen_val = { Disp_Draw_Bitmap_Array, 190, 57, bmp_false_true, UCHAR, &Naregelen, 2 };

static void * const lcd_naregelen_disp[] = { &disp_luchtmengkast_ico, &disp_naregelen_str, &disp_naregelen_val, 0 };

static s_key_value const key_naregelen_val = { UCHAR, 1, &Naregelen, &uchar_0, &uchar_1 };
//-----------------------------------------------------------------------------
// Streeftemperatuur ingangen
static s_disp_tekst const disp_streeftemp_str = { Disp_Draw_Tekst_L, 47, 22, &tekst_inst.Streeftemp_14 };
static s_disp_board_IO_Selection const disp_ana_in_streeftemp_sel = { Disp_Draw_Board_IO_Select, TempAnaIn, MAX_LUCHTMENGKAST, &AantalGroepen, 0, ANALOG_INPUT_ID, ANA_IN_TEMP, DummyNotUsed };

static s_disp_func const disp_copy_streeftemp_IO_func = { Disp_Control_Func, Disp_Copy_Streeftemp_IO };

static void * const lcd_ana_in_streeftemp[] = { &disp_copy_streeftemp_IO_func, &disp_luchtmengkast_ico, &disp_streeftemp_str, &disp_ana_in_streeftemp_sel, 0 };
//-----------------------------------------------------------------------------------------------------------
// Koppel units aan groepen
static s_disp_tekst     const disp_units_str = { Disp_Draw_Tekst_L, 47, 22, &tekst_inst.Units_14 };
static s_disp_tekst_add const disp_groep_str = { Disp_Draw_Tekst_Add_L, &tekst_inst.Groep_14 };
static s_disp_value_add const disp_groep_1   = { Disp_Draw_Value_Add, (SIZE_14 | LINKS), UCHAR, 0, &uchar_1 };
static s_disp_value_add const disp_groep_2   = { Disp_Draw_Value_Add, (SIZE_14 | LINKS), UCHAR, 0, &uchar_2 };
static s_disp_value_add const disp_groep_3   = { Disp_Draw_Value_Add, (SIZE_14 | LINKS), UCHAR, 0, &uchar_3 };
static s_disp_value_add const disp_groep_4   = { Disp_Draw_Value_Add, (SIZE_14 | LINKS), UCHAR, 0, &uchar_4 };
static s_disp_array_Selection const disp_select_units_sel = { Disp_Draw_Array_Select, GroepUnits, MAX_LUCHTMENGKAST, &AantalUnits };

static s_disp_func const disp_copy_units_1_func = { Disp_Control_Func, Disp_Copy_Units_1 };
static s_disp_func const disp_copy_units_2_func = { Disp_Control_Func, Disp_Copy_Units_2 };
static s_disp_func const disp_copy_units_3_func = { Disp_Control_Func, Disp_Copy_Units_3 };
static s_disp_func const disp_copy_units_4_func = { Disp_Control_Func, Disp_Copy_Units_4 };

static void * const lcd_select_units_groep_1[] = { &disp_copy_units_1_func, &disp_luchtmengkast_ico, &disp_units_str, &disp_space_14_L, &disp_groep_str, &disp_rechte_openings_haak_14_L, &disp_groep_1, &disp_rechte_sluit_haak_14_L, &disp_select_units_sel, 0 };
static void * const lcd_select_units_groep_2[] = { &disp_copy_units_2_func, &disp_luchtmengkast_ico, &disp_units_str, &disp_space_14_L, &disp_groep_str, &disp_rechte_openings_haak_14_L, &disp_groep_2, &disp_rechte_sluit_haak_14_L, &disp_select_units_sel, 0 };
static void * const lcd_select_units_groep_3[] = { &disp_copy_units_3_func, &disp_luchtmengkast_ico, &disp_units_str, &disp_space_14_L, &disp_groep_str, &disp_rechte_openings_haak_14_L, &disp_groep_3, &disp_rechte_sluit_haak_14_L, &disp_select_units_sel, 0 };
static void * const lcd_select_units_groep_4[] = { &disp_copy_units_4_func, &disp_luchtmengkast_ico, &disp_units_str, &disp_space_14_L, &disp_groep_str, &disp_rechte_openings_haak_14_L, &disp_groep_4, &disp_rechte_sluit_haak_14_L, &disp_select_units_sel, 0 };
//-----------------------------------------------------------------------------


//=============================================================================
s_key_action const opt_luchtmengkast_2_key_action[] =
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
  // --- Algemeen ----------------------------------------------
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
    Arrow_Luchtmengkast_Value,        // void (*arrow)(void); 
    Enter_Luchtmengkast_Value,        // void (*enter)(void);
  },
  { // Type klep (binnen/buiten of recirculatie)
    2,                                // nr
    0,                                // index
    &AantalGroepen,                   // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_type_klep_disp,               // display
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
    lcd_type_klep_disp,               // display
    &disp_cursor_225_78_150,          // cursor
    &key_type_klep_val,               // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Luchtmengkast_Scroll_Value, // void (*arrow)(void); 
    Enter_Luchtmengkast_Value,        // void (*enter)(void);
  },
  { // Type sturing (Digitaal/Analoog/CANopen/BACnet)
    3,                                // nr
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
    3,                                // nr
    1,                                // index
    &AantalGroepen,                   // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_type_sturing_disp,            // display
    &disp_cursor_225_78_150,          // cursor
    &key_type_sturing_val,            // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Type_Sturing_Value,         // void (*arrow)(void); 
    Enter_Luchtmengkast_Value,        // void (*enter)(void);
  },
  { // Looptijd bij hoger/lager sturing
    4,                                // nr
    0,                                // index
    &SturingDigitaal,                 // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_looptijd_disp,                // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Option_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    4,                                // nr
    1,                                // index
    &SturingDigitaal,                 // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_looptijd_disp,                // display
    &disp_cursor_207_78_8,            // cursor
    &key_looptijd_val,                // *value
    Number_Value,                     // void (*number)(void); 
    Arrow_Luchtmengkast_Value,        // void (*arrow)(void); 
    Enter_Luchtmengkast_Value,        // void (*enter)(void);
  },
  // --- Buitenklep / Recirculatieklep -------------------------
  { // Digitale ingang buitenklep hoger
    5,                                // nr
    0,                                // index
    &SturingDigitaal,                 // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_dig_in_buitenklep_open,       // display
    &disp_cursor,                     // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_Select_Func,             // void (*arrow)(void); 
    Enter_IO_Select_Func,             // void (*enter)(void);
  },
  { // Digitale ingang buitenklep lager
    6,                                // nr
    0,                                // index
    &SturingDigitaal,                 // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_dig_in_buitenklep_dicht,      // display
    &disp_cursor,                     // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_Select_Func,             // void (*arrow)(void); 
    Enter_IO_Select_Func,             // void (*enter)(void);
  },
  { // Analoge ingang buitenklep
    7,                                // nr
    0,                                // index
    &SturingAnaloog,                  // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_ana_in_buitenklep,            // display
    &disp_cursor,                     // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_Select_Func,             // void (*arrow)(void); 
    Enter_IO_Select_Func,             // void (*enter)(void);
  },
  { // Analoge uitgang buitenklep
    8,                                // nr
    0,                                // index
    &SturingTerugmelding,             // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_ana_out_buitenklep,           // display
    &disp_cursor,                     // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_Select_Func,             // void (*arrow)(void); 
    Enter_IO_Select_Func,             // void (*enter)(void);
  },
  // --- Bovenklep Ja/Nee --------------------------------------
  { // Bovenklep Ja/Nee bij recirculatieklep
    9,                                // nr
    0,                                // index
    &opt_app.LuchtmengkastGroep[0].TypeKlep,
    (unsigned char *)&option_index_0, // Optie index 
    lcd_bovenklep_disp,               // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Option_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    9,                                // nr
    1,                                // index
    &opt_app.LuchtmengkastGroep[0].TypeKlep,
    (unsigned char *)&option_index_0, // Optie index 
    lcd_bovenklep_disp,               // display
    &disp_cursor_checkbox,            // cursor
    &key_bovenklep_val,               // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Luchtmengkast_Scroll_Value, // void (*arrow)(void); 
    Enter_Luchtmengkast_Value,        // void (*enter)(void);
  },
  { // Bovenklep gekoppeld aan klep
    10,                               // nr
    0,                                // index
    &opt_app.LuchtmengkastGroep[0].BovenklepEnabled,
    (unsigned char *)&option_index_0, // Optie index 
    lcd_bovenklep_gekoppeld_disp,      // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Option_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    10,                               // nr
    1,                                // index
    &opt_app.LuchtmengkastGroep[0].BovenklepEnabled,
    (unsigned char *)&option_index_0, // Optie index 
    lcd_bovenklep_gekoppeld_disp,     // display
    &disp_cursor_checkbox,            // cursor
    &key_bovenklep_gekoppeld_val,     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Luchtmengkast_Scroll_Value, // void (*arrow)(void); 
    Enter_Luchtmengkast_Value,        // void (*enter)(void);
  },
  // --- Binnenklep / Bovenklep --------------------------------
  { // Digitale ingang binnenklep hoger
    11,                               // nr
    0,                                // index
    &SturingBinnenBovenklepDigitaal,  // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_dig_in_binnenklep_open,       // display
    &disp_cursor,                     // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_Select_Func,             // void (*arrow)(void); 
    Enter_IO_Select_Func,             // void (*enter)(void);
  },
  { // Digitale ingang binnenklep lager
    12,                               // nr
    0,                                // index
    &SturingBinnenBovenklepDigitaal,  // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_dig_in_binnenklep_dicht,      // display
    &disp_cursor,                     // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_Select_Func,             // void (*arrow)(void); 
    Enter_IO_Select_Func,             // void (*enter)(void);
  },
  { // Analoge ingang binnenklep
    13,                               // nr
    0,                                // index
    &SturingBinnenBovenklepAnaloog,   // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_ana_in_binnenklep,            // display
    &disp_cursor,                     // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_Select_Func,             // void (*arrow)(void); 
    Enter_IO_Select_Func,             // void (*enter)(void);
  },
  { // Analoge uitgang binnenklep
    14,                                  // nr
    0,                                   // index
    &SturingBinnenBovenklepTerugmelding, // option
    (unsigned char *)&option_index_0,    // Optie index 
    lcd_ana_out_binnenklep,              // display
    &disp_cursor,                        // cursor
    &dummy_value,                        // *value
    Dummy_Func,                          // void (*number)(void); 
    Arrow_IO_Select_Func,                // void (*arrow)(void); 
    Enter_IO_Select_Func,                // void (*enter)(void);
  },
  // --- Inblaas ventilator -----------------------------------
  { // Digitale ingang ventilatie hoger
    15,                               // nr
    0,                                // index
    &SturingDigitaal,                 // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_dig_in_inblaasvent_hoger,     // display
    &disp_cursor,                     // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_Select_Func,             // void (*arrow)(void); 
    Enter_IO_Select_Func,             // void (*enter)(void);
  },
  { // Digitale ingang ventilatie lager
    16,                               // nr
    0,                                // index
    &SturingDigitaal,                 // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_dig_in_inblaasvent_lager,     // display
    &disp_cursor,                     // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_Select_Func,             // void (*arrow)(void); 
    Enter_IO_Select_Func,             // void (*enter)(void);
  },
  { // Analoge ingang ventilatie
    17,                               // nr
    0,                                // index
    &SturingAnaloog,                  // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_ana_in_inblaasvent,           // display
    &disp_cursor,                     // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_Select_Func,             // void (*arrow)(void); 
    Enter_IO_Select_Func,             // void (*enter)(void);
  },
  { // Analoge uitgang ventilatie
    18,                               // nr
    0,                                // index
    &SturingTerugmelding,             // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_ana_out_inblaasvent,          // display
    &disp_cursor,                     // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_Select_Func,             // void (*arrow)(void); 
    Enter_IO_Select_Func,             // void (*enter)(void);
  },
  // --- Afblaas ventilator -------------------------------------
  { // Afblaasventilator Ja/Nee
    19,                               // nr
    0,                                // index
    &AantalGroepen,                   // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_afblaasvent_disp,             // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Option_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    19,                               // nr
    1,                                // index
    &AantalGroepen,                   // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_afblaasvent_disp,             // display
    &disp_cursor_checkbox,            // cursor
    &key_afblaasvent_val,             // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Luchtmengkast_Scroll_Value, // void (*arrow)(void); 
    Enter_Luchtmengkast_Value,        // void (*enter)(void);
  },
  { // Afblaasventilator gekoppeld aan klep
    20,                               // nr
    0,                                // index
    &opt_app.LuchtmengkastGroep[0].AfblaasventEnabled,
    (unsigned char *)&option_index_0, // Optie index 
    lcd_afblaasvent_gekoppeld_disp,   // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Option_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    20,                               // nr
    1,                                // index
    &opt_app.LuchtmengkastGroep[0].AfblaasventEnabled,
    (unsigned char *)&option_index_0, // Optie index 
    lcd_afblaasvent_gekoppeld_disp,   // display
    &disp_cursor_checkbox,            // cursor
    &key_afblaasvent_gekoppeld_val,   // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Luchtmengkast_Scroll_Value, // void (*arrow)(void); 
    Enter_Luchtmengkast_Value,        // void (*enter)(void);
  },
  { // Digitale ingang ventilatie hoger
    21,                               // nr
    0,                                // index
    &SturingAfblaasventDigitaal,      // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_dig_in_afblaasvent_hoger,     // display
    &disp_cursor,                     // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_Select_Func,             // void (*arrow)(void); 
    Enter_IO_Select_Func,             // void (*enter)(void);
  },
  { // Digitale ingang ventilatie lager
    22,                               // nr
    0,                                // index
    &SturingAfblaasventDigitaal,      // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_dig_in_afblaasvent_lager,     // display
    &disp_cursor,                     // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_Select_Func,             // void (*arrow)(void); 
    Enter_IO_Select_Func,             // void (*enter)(void);
  },
  { // Analoge ingang ventilatie
    23,                               // nr
    0,                                // index
    &SturingAfblaasventAnaloog,       // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_ana_in_afblaasvent,           // display
    &disp_cursor,                     // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_Select_Func,             // void (*arrow)(void); 
    Enter_IO_Select_Func,             // void (*enter)(void);
  },
  { // Analoge uitgang ventilatie
    24,                               // nr
    0,                                // index
    &SturingAfblaasventTerugmelding,  // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_ana_out_afblaasvent,          // display
    &disp_cursor,                     // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_Select_Func,             // void (*arrow)(void); 
    Enter_IO_Select_Func,             // void (*enter)(void);
  },
  // --- Verwarming Ja/Nee -------------------------------------
  { // Verwarming Ja/Nee
    25,                               // nr
    0,                                // index
    &AantalGroepen,                   // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_verwarming_disp,              // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Option_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    25,                               // nr
    1,                                // index
    &AantalGroepen,                   // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_verwarming_disp,              // display
    &disp_cursor_checkbox,            // cursor
    &key_verwarming_val,              // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Luchtmengkast_Scroll_Value, // void (*arrow)(void); 
    Enter_Luchtmengkast_Value,        // void (*enter)(void);
  },
  // --- Verwarming --------------------------------------------
  { // Digitale ingang verwarming hoger
    26,                               // nr
    0,                                // index
    &SturingVerwarmingDigitaal,       // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_dig_in_verwarming_open,       // display
    &disp_cursor,                     // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_Select_Func,             // void (*arrow)(void); 
    Enter_IO_Select_Func,             // void (*enter)(void);
  },
  { // Digitale ingang verwarming lager
    27,                               // nr
    0,                                // index
    &SturingVerwarmingDigitaal,       // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_dig_in_verwarming_dicht,      // display
    &disp_cursor,                     // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_Select_Func,             // void (*arrow)(void); 
    Enter_IO_Select_Func,             // void (*enter)(void);
  },
  { // Analoge ingang verwarming
    28,                               // nr
    0,                                // index
    &SturingVerwarmingAnaloog,        // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_ana_in_verwarming,            // display
    &disp_cursor,                     // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_Select_Func,             // void (*arrow)(void); 
    Enter_IO_Select_Func,             // void (*enter)(void);
  },
  { // Analoge uitgang verwarming
    29,                               // nr
    0,                                // index
    &SturingVerwarmingTerugmelding,   // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_ana_out_verwarming,           // display
    &disp_cursor,                     // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_Select_Func,             // void (*arrow)(void); 
    Enter_IO_Select_Func,             // void (*enter)(void);
  },
  // --- Alarm contact per luchtmengkast groep -----------------
  { // Digitale uitgang alarm
    30,                                  // nr
    0,                                   // index
    &AantalGroepen,                      // option
    (unsigned char *)&option_index_0,    // Optie index 
    lcd_dig_out_alarm,                   // display
    &disp_cursor,                        // cursor
    &dummy_value,                        // *value
    Dummy_Func,                          // void (*number)(void); 
    Arrow_IO_Select_Func,                // void (*arrow)(void); 
    Enter_IO_Select_Func,                // void (*enter)(void);
  },
  // --- Units -------------------------------------------------
  { // Aantal units
    31,                               // nr
    0,                                // index
    &AantalGroepen,                   // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_aantal_units_disp,            // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Option_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    31,                               // nr
    1,                                // index
    &AantalGroepen,                   // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_aantal_units_disp,            // display
    &disp_cursor_207_78_8,            // cursor
    &key_aantal_units_val,            // *value
    Number_Value,                     // void (*number)(void); 
    Arrow_Luchtmengkast_Value,        // void (*arrow)(void); 
    Enter_Luchtmengkast_Value,        // void (*enter)(void);
  },
  { // Inblaastemperatuur
    32,                               // nr
    0,                                // index
    &AantalUnits,                     // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_ana_in_inblaastemp,           // display
    &disp_cursor,                     // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_Select_Func,             // void (*arrow)(void); 
    Enter_IO_Select_Func,             // void (*enter)(void);
  },
  { // Afblaastemperatuur
    33,                               // nr
    0,                                // index
    &AantalUnits,                     // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_ana_in_mengtemp,           // display
    &disp_cursor,                     // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_Select_Func,             // void (*arrow)(void); 
    Enter_IO_Select_Func,             // void (*enter)(void);
  },
  { // Vorstbewaking
    34,                               // nr
    0,                                // index
    &AantalUnits,                     // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_dig_in_vorst,                 // display
    &disp_cursor,                     // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_Select_Func,             // void (*arrow)(void); 
    Enter_IO_Select_Func,             // void (*enter)(void);
  },
  { // Alarm ingang
    35,                               // nr
    0,                                // index
    &AantalUnits,                     // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_dig_in_alarm,                 // display
    &disp_cursor,                     // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_Select_Func,             // void (*arrow)(void); 
    Enter_IO_Select_Func,             // void (*enter)(void);
  },
  { // Drukverschil
    36,                               // nr
    0,                                // index
    &AantalUnits,                     // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_dig_in_drukverschil,          // display
    &disp_cursor,                     // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_Select_Func,             // void (*arrow)(void); 
    Enter_IO_Select_Func,             // void (*enter)(void);
  },
  // --- Buitenklep / Recirculatieklep -------------------------
  { // Type sturing (digitaal/analoog)
    37,                               // nr
    0,                                // index
    &AantalUnits,                     // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_type_buitenklep_disp,         // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Option_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    37,                               // nr
    1,                                // index
    &AantalUnits,                     // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_type_buitenklep_disp,         // display
    &disp_cursor_225_78_150,          // cursor
    &key_type_buitenklep_val,         // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Luchtmengkast_Scroll_Value, // void (*arrow)(void); 
    Enter_Luchtmengkast_Value,        // void (*enter)(void);
  },
  { // Digitale uitgang buitenklep hoger
    38,                               // nr
    0,                                // index
    &BuitenklepDigitaal,              // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_dig_out_buitenklep_open,      // display
    &disp_cursor,                     // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_Select_Func,             // void (*arrow)(void); 
    Enter_IO_Select_Func,             // void (*enter)(void);
  },
  { // Digitale uitgang buitenklep lager
    39,                               // nr
    0,                                // index
    &BuitenklepDigitaal,              // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_dig_out_buitenklep_dicht,     // display
    &disp_cursor,                     // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_Select_Func,             // void (*arrow)(void); 
    Enter_IO_Select_Func,             // void (*enter)(void);
  },
  { // Analoge uitgang buitenklep
    40,                               // nr
    0,                                // index
    &BuitenklepAnaloog,               // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_unit_ana_out_buitenklep,      // display
    &disp_cursor,                     // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_Select_Func,             // void (*arrow)(void); 
    Enter_IO_Select_Func,             // void (*enter)(void);
  },
  { // Analoge ingang buitenklep
    41,                               // nr
    0,                                // index
    &AantalUnits,                     // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_unit_ana_in_buitenklep,       // display
    &disp_cursor,                     // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_Select_Func,             // void (*arrow)(void); 
    Enter_IO_Select_Func,             // void (*enter)(void);
  },
  // --- Binnenklep / Bovenklep --------------------------------
  { // Type sturing (digitaal/analoog)
    42,                               // nr
    0,                                // index
    &BinnenklepEnabled,               // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_type_binnenklep_disp,         // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Option_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    42,                               // nr
    1,                                // index
    &BinnenklepEnabled,               // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_type_binnenklep_disp,         // display
    &disp_cursor_225_78_150,          // cursor
    &key_type_binnenklep_val,         // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Luchtmengkast_Scroll_Value, // void (*arrow)(void); 
    Enter_Luchtmengkast_Value,        // void (*enter)(void);
  },
  { // Digitale uitgang binnenklep hoger
    43,                               // nr
    0,                                // index
    &BinnenklepDigitaal,              // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_dig_out_binnenklep_open,      // display
    &disp_cursor,                     // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_Select_Func,             // void (*arrow)(void); 
    Enter_IO_Select_Func,             // void (*enter)(void);
  },
  { // Digitale uitgang binnenklep lager
    44,                               // nr
    0,                                // index
    &BinnenklepDigitaal,              // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_dig_out_binnenklep_dicht,     // display
    &disp_cursor,                     // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_Select_Func,             // void (*arrow)(void); 
    Enter_IO_Select_Func,             // void (*enter)(void);
  },
  { // Analoge uitgang binnenklep
    45,                               // nr
    0,                                // index
    &BinnenklepAnaloog,               // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_unit_ana_out_binnenklep,      // display
    &disp_cursor,                     // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_Select_Func,             // void (*arrow)(void); 
    Enter_IO_Select_Func,             // void (*enter)(void);
  },
  { // Analoge ingang binnenklep
    46,                               // nr
    0,                                // index
    &BinnenklepEnabled,               // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_unit_ana_in_binnenklep,       // display
    &disp_cursor,                     // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_Select_Func,             // void (*arrow)(void); 
    Enter_IO_Select_Func,             // void (*enter)(void);
  },
  // --- Inblaas ventilator -----------------------------------
  { // Type sturing (digitaal/analoog/ebmBus)
    47,                               // nr
    0,                                // index
    &AantalUnits,                     // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_type_inblaasvent_disp,        // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Option_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    47,                               // nr
    1,                                // index
    &AantalUnits,                     // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_type_inblaasvent_disp,        // display
    &disp_cursor_225_78_180,          // cursor
    &key_type_inblaasvent_val,        // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Luchtmengkast_Vent_Value,   // void (*arrow)(void); 
    Enter_Luchtmengkast_Vent_Value,   // void (*enter)(void);
  },
  { // Operation Mode - ClosedLoop / OpenLoop
    48,                                  // nr
    0,                                   // index
    &InblaasventRS485,                   // option
    (unsigned char *)&option_index_0,    // Optie index 
    lcd_operation_mode_inblaasvent_disp, // display
    0,                                   // cursor
    &dummy_value,                        // *value
    Dummy_Func,                          // void (*number)(void); 
    Arrow_Option_Func,                   // void (*arrow)(void); 
    Dummy_Func,                          // void (*enter)(void);
  },
  {
    48,                                  // nr
    1,                                   // index
    &InblaasventRS485,                   // option
    (unsigned char *)&option_index_0,    // Optie index 
    lcd_operation_mode_inblaasvent_disp, // display
    &disp_cursor_225_78_180,             // cursor
    &key_operation_mode_inblaasvent_val, // *value
    Dummy_Func,                          // void (*number)(void); 
    Arrow_Scroll_Option_Value,               // void (*arrow)(void); 
    Increment_Func_Index_Enter_Option_Value, // void (*enter)(void);
  },
  { // Digitale uitgang ventilatie hoger
    49,                               // nr
    0,                                // index
    &InblaasventDigitaal,            // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_dig_out_inblaasvent_hoger,    // display
    &disp_cursor,                     // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_Select_Func,             // void (*arrow)(void); 
    Enter_IO_Select_Func,             // void (*enter)(void);
  },
  { // Digitale uitgang ventilatie lager
    50,                               // nr
    0,                                // index
    &InblaasventDigitaal,            // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_dig_out_inblaasvent_lager,    // display
    &disp_cursor,                     // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_Select_Func,             // void (*arrow)(void); 
    Enter_IO_Select_Func,             // void (*enter)(void);
  },
  { // Analoge uitgang ventilatie
    51,                               // nr
    0,                                // index
    &InblaasventAnaloog,             // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_unit_ana_out_inblaasvent,     // display
    &disp_cursor,                     // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_Select_Func,             // void (*arrow)(void); 
    Enter_IO_Select_Func,             // void (*enter)(void);
  },
  { // Analoge ingang ventilatie
    52,                               // nr
    0,                                // index
    &InblaasventTerugmelding,         // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_unit_ana_in_inblaasvent,      // display
    &disp_cursor,                     // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_Select_Func,             // void (*arrow)(void); 
    Enter_IO_Select_Func,             // void (*enter)(void);
  },
  { // RS485 bus inblaasventilator
    53,                               // nr
    0,                                // index
    &InblaasventRS485,                // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_unit_RS485_inblaasvent,       // display
    &disp_cursor,                     // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_Vent_Select_Func,        // void (*arrow)(void); 
    Enter_IO_Vent_Select_Func,        // void (*enter)(void);
  },
  { // Inblaas ventilatoren adresseren
    54,                               // nr
    0,                                // index
    &InblaasventRS485,                // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_adres_inblaasvent,            // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Option_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    54,                               // nr
    1,                                // index
    &InblaasventRS485,                // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_adres_inblaasvent,            // display
    &disp_cursor_207_78_8,            // cursor
    &key_vent_adres_val,              // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Adres_Inblaasvent_Value,    // void (*arrow)(void); 
    Enter_Adres_Inblaasvent_Value,    // void (*enter)(void);
  },
  // --- Afblaas ventilator -------------------------------------
  { // Type sturing (digitaal/analoog/ebmBus)
    55,                                           // nr
    0,                                            // index
    &opt_app.Luchtmengkast[0].AfblaasventEnabled, // option
    (unsigned char *)&option_index_0,             // Optie index 
    lcd_type_afblaasvent_disp,                    // display
    0,                                            // cursor
    &dummy_value,                                 // *value
    Dummy_Func,                                   // void (*number)(void); 
    Arrow_Option_Func,                            // void (*arrow)(void); 
    Dummy_Func,                                   // void (*enter)(void);
  },
  {
    55,                                           // nr
    1,                                            // index
    &opt_app.Luchtmengkast[0].AfblaasventEnabled, // option
    (unsigned char *)&option_index_0,             // Optie index 
    lcd_type_afblaasvent_disp,                    // display
    &disp_cursor_225_78_180,                      // cursor
    &key_type_afblaasvent_val,                    // *value
    Dummy_Func,                                   // void (*number)(void); 
    Arrow_Luchtmengkast_Vent_Value,               // void (*arrow)(void); 
    Enter_Luchtmengkast_Vent_Value,               // void (*enter)(void);
  },
  { // Operation Mode - ClosedLoop / OpenLoop
    56,                                  // nr
    0,                                   // index
    &AfblaasventRS485,                   // option
    (unsigned char *)&option_index_0,    // Optie index 
    lcd_operation_mode_afblaasvent_disp, // display
    0,                                   // cursor
    &dummy_value,                        // *value
    Dummy_Func,                          // void (*number)(void); 
    Arrow_Option_Func,                   // void (*arrow)(void); 
    Dummy_Func,                          // void (*enter)(void);
  },
  {
    56,                                  // nr
    1,                                   // index
    &AfblaasventRS485,                   // option
    (unsigned char *)&option_index_0,    // Optie index 
    lcd_operation_mode_afblaasvent_disp, // display
    &disp_cursor_225_78_180,             // cursor
    &key_operation_mode_afblaasvent_val, // *value
    Dummy_Func,                          // void (*number)(void); 
    Arrow_Scroll_Option_Value,               // void (*arrow)(void); 
    Increment_Func_Index_Enter_Option_Value, // void (*enter)(void);
  },
  { // Digitale uitgang ventilatie hoger
    57,                               // nr
    0,                                // index
    &AfblaasventDigitaal,             // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_dig_out_afblaasvent_hoger,    // display
    &disp_cursor,                     // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_Select_Func,             // void (*arrow)(void); 
    Enter_IO_Select_Func,             // void (*enter)(void);
  },
  { // Digitale uitgang ventilatie lager
    58,                               // nr
    0,                                // index
    &AfblaasventDigitaal,             // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_dig_out_afblaasvent_lager,    // display
    &disp_cursor,                     // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_Select_Func,             // void (*arrow)(void); 
    Enter_IO_Select_Func,             // void (*enter)(void);
  },
  { // Analoge uitgang ventilatie
    59,                               // nr
    0,                                // index
    &AfblaasventAnaloog,              // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_unit_ana_out_afblaasvent,     // display
    &disp_cursor,                     // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_Select_Func,             // void (*arrow)(void); 
    Enter_IO_Select_Func,             // void (*enter)(void);
  },
  { // Analoge ingang ventilatie
    60,                               // nr
    0,                                // index
    &AfblaasventTerugmelding,         // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_unit_ana_in_afblaasvent,      // display
    &disp_cursor,                     // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_Select_Func,             // void (*arrow)(void); 
    Enter_IO_Select_Func,             // void (*enter)(void);
  },
  { // RS485 bus afblaas ventilator
    61,                               // nr
    0,                                // index
    &AfblaasventRS485,                // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_unit_RS485_afblaasvent,       // display
    &disp_cursor,                     // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_Vent_Select_Func,        // void (*arrow)(void); 
    Enter_IO_Vent_Select_Func,        // void (*enter)(void);
  },
  { // Afblaas ventilatoren adresseren
    62,                               // nr
    0,                                // index
    &AfblaasventRS485,                // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_adres_afblaasvent,            // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Option_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    62,                               // nr
    1,                                // index
    &AfblaasventRS485,                // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_adres_afblaasvent,            // display
    &disp_cursor_207_78_8,            // cursor
    &key_vent_adres_val,              // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Adres_Afblaasvent_Value,    // void (*arrow)(void); 
    Enter_Adres_Afblaasvent_Value,    // void (*enter)(void);
  },
  // --- Adres wijzigen ebmBus ---------------------------------
  { // Wijzig adres
    63,                               // nr
    0,                                // index
    &DispAdresWijzigen,               // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_wijzig_adres_disp,            // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Wijzig_Adres_Func,          // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    63,                               // nr
    1,                                // index
    &DispAdresWijzigen,               // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_wijzig_adres_disp,            // display
    &disp_cursor_147_78_8,            // cursor
    &key_oud_adres_val,               // *value
    Number_Value,                     // void (*number)(void); 
    Arrow_Value,                      // void (*arrow)(void); 
    Increment_Func_Index_Enter_Value, // void (*enter)(void);
  },
  {
    63,                               // nr
    2,                                // index
    &DispAdresWijzigen,               // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_wijzig_adres_disp,            // display
    &disp_cursor_207_78_8,            // cursor
    &key_nieuw_adres_val,             // *value
    Number_Value,                     // void (*number)(void); 
    Arrow_Wijzig_Adres_Value,         // void (*arrow)(void); 
    Enter_Wijzig_Adres_Value,         // void (*enter)(void);
  },
  {
    63,                               // nr
    3,                                // index
    &DispAdresWijzigen,               // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_wijzig_adres_ok_disp,         // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Increment_Func_Index,             // void (*arrow)(void); 
    Enter_Wijzig_Adres_Func,          // void (*enter)(void);
  },
  // --- Verwarming --------------------------------------------
  { // Type sturing (digitaal/analoog)
    64,                                          // nr
    0,                                           // index
    &opt_app.Luchtmengkast[0].VerwarmingEnabled, // option
    (unsigned char *)&option_index_0,            // Optie index 
    lcd_type_verwarming_disp,                    // display
    0,                                           // cursor
    &dummy_value,                                // *value
    Dummy_Func,                                  // void (*number)(void); 
    Arrow_Option_Func,                           // void (*arrow)(void); 
    Dummy_Func,                                  // void (*enter)(void);
  },
  {
    64,                                          // nr
    1,                                           // index
    &opt_app.Luchtmengkast[0].VerwarmingEnabled, // option
    (unsigned char *)&option_index_0,            // Optie index 
    lcd_type_verwarming_disp,                    // display
    &disp_cursor_225_78_150,                     // cursor
    &key_type_verwarming_val,                    // *value
    Dummy_Func,                                  // void (*number)(void); 
    Arrow_Luchtmengkast_Scroll_Value,            // void (*arrow)(void); 
    Enter_Luchtmengkast_Value,                   // void (*enter)(void);
  },
  { // Digitale uitgang verwarming hoger
    65,                               // nr
    0,                                // index
    &VerwarmingDigitaal,              // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_dig_out_verwarming_open,      // display
    &disp_cursor,                     // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_Select_Func,             // void (*arrow)(void); 
    Enter_IO_Select_Func,             // void (*enter)(void);
  },
  { // Digitale uitgang verwarming lager
    66,                               // nr
    0,                                // index
    &VerwarmingDigitaal,              // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_dig_out_verwarming_dicht,     // display
    &disp_cursor,                     // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_Select_Func,             // void (*arrow)(void); 
    Enter_IO_Select_Func,             // void (*enter)(void);
  },
  { // Analoge uitgang verwarming
    67,                               // nr
    0,                                // index
    &VerwarmingAnaloog,               // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_unit_ana_out_verwarming,      // display
    &disp_cursor,                     // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_Select_Func,             // void (*arrow)(void); 
    Enter_IO_Select_Func,             // void (*enter)(void);
  },
  { // Analoge ingang verwarming
    68,                                          // nr
    0,                                           // index
    &opt_app.Luchtmengkast[0].VerwarmingEnabled, // option
    (unsigned char *)&option_index_0,            // Optie index 
    lcd_unit_ana_in_verwarming,                  // display
    &disp_cursor,                                // cursor
    &dummy_value,                                // *value
    Dummy_Func,                                  // void (*number)(void); 
    Arrow_IO_Select_Func,                        // void (*arrow)(void); 
    Enter_IO_Select_Func,                        // void (*enter)(void);
  },
  // --- Naregelen verwarming op basis van inblaastemperatuur --
  { // Naregelen op basis van inblaastemperatuur
    69,                               // nr
    0,                                // index
    &DispNaregelen,                   // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_naregelen_disp,               // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Option_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    69,                               // nr
    1,                                // index
    &DispNaregelen,                   // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_naregelen_disp,               // display
    &disp_cursor_checkbox,            // cursor
    &key_naregelen_val,               // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_Luchtmengkast_Scroll_Value, // void (*arrow)(void); 
    Enter_Luchtmengkast_Value,        // void (*enter)(void);
  },
  { // Streeftemperatuur
    70,                                  // nr
    0,                                   // index
    &opt_app.Luchtmengkast[0].Naregelen, // option
    (unsigned char *)&option_index_0,    // Optie index 
    lcd_ana_in_streeftemp,               // display
    &disp_cursor,                        // cursor
    &dummy_value,                        // *value
    Dummy_Func,                          // void (*number)(void); 
    Arrow_IO_Select_Func,                // void (*arrow)(void); 
    Enter_IO_Select_Func,                // void (*enter)(void);
  },
  // --- Select units for each group ---------------------------
  { // Select units groep 1
    71,                                     // nr
    0,                                      // index
    &opt_app.LuchtmengkastGroep[0].Enabled, // option
    (unsigned char *)&option_index_0,       // Optie index 
    lcd_select_units_groep_1,               // display
    &disp_cursor,                           // cursor
    &dummy_value,                           // *value
    Dummy_Func,                             // void (*number)(void); 
    Arrow_IO_Select_No_Shift_Func,          // void (*arrow)(void); 
    Enter_IO_Select_Func,                   // void (*enter)(void);
  },
  { // Select units groep 2
    72,                                     // nr
    0,                                      // index
    &opt_app.LuchtmengkastGroep[1].Enabled, // option
    (unsigned char *)&option_index_0,       // Optie index 
    lcd_select_units_groep_2,               // display
    &disp_cursor,                           // cursor
    &dummy_value,                           // *value
    Dummy_Func,                             // void (*number)(void); 
    Arrow_IO_Select_No_Shift_Func,          // void (*arrow)(void); 
    Enter_IO_Select_Func,                   // void (*enter)(void);
  },
  { // Select units groep 3
    73,                                     // nr
    0,                                      // index
    &opt_app.LuchtmengkastGroep[2].Enabled, // option
    (unsigned char *)&option_index_0,       // Optie index 
    lcd_select_units_groep_3,               // display
    &disp_cursor,                           // cursor
    &dummy_value,                           // *value
    Dummy_Func,                             // void (*number)(void); 
    Arrow_IO_Select_No_Shift_Func,          // void (*arrow)(void); 
    Enter_IO_Select_Func,                   // void (*enter)(void);
  },
  { // Select units groep 4
    74,                                     // nr
    0,                                      // index
    &opt_app.LuchtmengkastGroep[3].Enabled, // option
    (unsigned char *)&option_index_0,       // Optie index 
    lcd_select_units_groep_4,               // display
    &disp_cursor,                           // cursor
    &dummy_value,                           // *value
    Dummy_Func,                             // void (*number)(void); 
    Arrow_IO_Select_No_Shift_Func,          // void (*arrow)(void); 
    Enter_IO_Select_Func,                   // void (*enter)(void);
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
s_screen screen_opt_luchtmengkast_2;
s_screen const screen_opt_luchtmengkast_2_default =
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
  &opt_luchtmengkast_2_key_action[0], // first_action
  &opt_luchtmengkast_2_key_action[sizeof(opt_luchtmengkast_2_key_action)/sizeof(s_key_action) - 1], // last action 
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

void Control_Screen_Option_Luchtmengkast_2(void)
{
  if (password_enabled & PASSWORD_ENABLED_OPT_MASK)
    start_flag = end_flag = 1;
  else
    start_flag = end_flag = 0;
  Enter_IO_Select_Func();
  
  SetDisplayOptions();

  Control_Screen(&screen_opt_luchtmengkast_2, &screen_opt_luchtmengkast_2_default, 1, 1);

  CopyTempIO = 0;

  OudAdres   = 1;
  NieuwAdres = 1;
  AdresWijzigenOk      = 0;
  AdresWijzigenMislukt = 0;
  AdresWijzigenStatus  = 0;
}

//------------------------------------------------------------------------------
static void SetDisplayOptions(void)
{
unsigned char i;

  AantalGroepen   = 0;
  AantalUnits     = 0;

  SturingDigitaal     = 0;
  SturingAnaloog      = 0;
  SturingTerugmelding = 0;

  SturingBinnenBovenklepDigitaal     = 0;
  SturingBinnenBovenklepAnaloog      = 0;
  SturingBinnenBovenklepTerugmelding = 0;
  SturingAfblaasventDigitaal         = 0;
  SturingAfblaasventAnaloog          = 0;
  SturingAfblaasventTerugmelding     = 0;
  SturingVerwarmingDigitaal          = 0;
  SturingVerwarmingAnaloog           = 0;
  SturingVerwarmingTerugmelding      = 0;

  BuitenklepDigitaal      = 0;
  BuitenklepAnaloog       = 0;
  BinnenklepEnabled       = 0;
  BinnenklepDigitaal      = 0;
  BinnenklepAnaloog       = 0;
  VerwarmingDigitaal      = 0;
  VerwarmingAnaloog       = 0;
  AfblaasventDigitaal     = 0;
  AfblaasventAnaloog      = 0;
  AfblaasventRS485        = 0;
  AfblaasventTerugmelding = 0;
  InblaasventDigitaal     = 0;
  InblaasventAnaloog      = 0;
  InblaasventRS485        = 0;
  InblaasventTerugmelding = 0;

  DispNaregelen           = 0;
  DispAdresWijzigen       = 0;

  if (opt_app.LuchtmengkastGroep[0].Enabled)
  {
    // Binnenklep / Bovenklep
    if ((opt_app.LuchtmengkastGroep[0].BovenklepEnabled && !opt_app.LuchtmengkastGroep[0].BovenklepGekoppeldAanKlep) || (opt_app.LuchtmengkastGroep[0].TypeKlep == TYPE_KLEP_BINNEN_BUITEN))
	{
	  switch (opt_app.LuchtmengkastGroep[0].Buitenklep.TypeSturing)
	  {
        case TYPE_STURING_DIGITAAL:
          SturingBinnenBovenklepTerugmelding = 1;
          SturingBinnenBovenklepDigitaal     = 1;
          SturingBinnenBovenklepAnaloog      = 0;
		  break;
        case TYPE_STURING_ANALOOG:
          SturingBinnenBovenklepTerugmelding = 1;
          SturingBinnenBovenklepDigitaal     = 0;
          SturingBinnenBovenklepAnaloog      = 1;
		  break;
        case TYPE_STURING_CANOPEN:
          SturingBinnenBovenklepTerugmelding = 0;
          SturingBinnenBovenklepDigitaal     = 0;
          SturingBinnenBovenklepAnaloog      = 0;
		  break;
	  }
    }
	// Afblaasventilator
    if (opt_app.LuchtmengkastGroep[0].AfblaasventEnabled && !opt_app.LuchtmengkastGroep[0].AfblaasventGekoppeldAanKlep)
    {
	  switch (opt_app.LuchtmengkastGroep[0].Afblaasvent.TypeSturing)
	  {
        case TYPE_STURING_DIGITAAL:
          SturingAfblaasventTerugmelding = 1;
          SturingAfblaasventDigitaal     = 1;
          SturingAfblaasventAnaloog      = 0;
		  break;
        case TYPE_STURING_ANALOOG:
          SturingAfblaasventTerugmelding = 1;
          SturingAfblaasventDigitaal     = 0;
          SturingAfblaasventAnaloog      = 1;
		  break;
        case TYPE_STURING_CANOPEN:
          SturingAfblaasventTerugmelding = 0;
          SturingAfblaasventDigitaal     = 0;
          SturingAfblaasventAnaloog      = 0;
		  break;
	  }
    }
	// Verwarming
    if (opt_app.LuchtmengkastGroep[0].VerwarmingEnabled)
    {
	  switch (opt_app.LuchtmengkastGroep[0].Verwarming.TypeSturing)
	  {
        case TYPE_STURING_DIGITAAL:
          SturingVerwarmingTerugmelding = 1;
          SturingVerwarmingDigitaal     = 1;
          SturingVerwarmingAnaloog      = 0;
		  break;
        case TYPE_STURING_ANALOOG:
          SturingVerwarmingTerugmelding = 1;
          SturingVerwarmingDigitaal     = 0;
          SturingVerwarmingAnaloog      = 1;
		  break;
        case TYPE_STURING_CANOPEN:
          SturingVerwarmingTerugmelding = 0;
          SturingVerwarmingDigitaal     = 0;
          SturingVerwarmingAnaloog      = 0;
		  break;
	  }
    }
    // Buitenklep
	switch (opt_app.LuchtmengkastGroep[0].Buitenklep.TypeSturing)
	{
      case TYPE_STURING_DIGITAAL:
        SturingTerugmelding = 1;
        SturingDigitaal     = 1;
        SturingAnaloog      = 0;
	    break;
      case TYPE_STURING_ANALOOG:
        SturingTerugmelding = 1;
        SturingDigitaal     = 0;
        SturingAnaloog      = 1;
	    break;
      case TYPE_STURING_CANOPEN:
        SturingTerugmelding = 0;
        SturingDigitaal     = 0;
        SturingAnaloog      = 0;
	    break;
	}

    for (i = 0; i < MAX_LUCHTMENGKAST_GROEP; i++)
    {
      if (opt_app.LuchtmengkastGroep[i].Enabled)
        AantalGroepen++;
    }
  }

  if (opt_app.Luchtmengkast[0].Enabled)
  {
    switch (opt_app.Luchtmengkast[0].Buitenklep.TypeSturing)
    {
      case TYPE_STURING_DIGITAAL: BuitenklepDigitaal = 1; break;
      case TYPE_STURING_ANALOOG : BuitenklepAnaloog  = 1; break;
    }
    if (opt_app.Luchtmengkast[0].BovenklepEnabled || (opt_app.Luchtmengkast[0].TypeKlep == TYPE_KLEP_BINNEN_BUITEN))
    {
      BinnenklepEnabled = 1;
      switch (opt_app.Luchtmengkast[0].Binnenklep.TypeSturing)
      {
        case TYPE_STURING_DIGITAAL: BinnenklepDigitaal = 1; break;
        case TYPE_STURING_ANALOOG : BinnenklepAnaloog  = 1; break;
      }
    }
    if (opt_app.Luchtmengkast[0].VerwarmingEnabled)
    {
      switch (opt_app.Luchtmengkast[0].Verwarming.TypeSturing)
      {
        case TYPE_STURING_DIGITAAL: VerwarmingDigitaal = 1; break;
        case TYPE_STURING_ANALOOG : VerwarmingAnaloog  = 1; break;
      }
    }
    if (opt_app.Luchtmengkast[0].AfblaasventEnabled)
    {
      switch (opt_app.Luchtmengkast[0].Afblaasvent.TypeSturing)
      {
        case TYPE_STURING_DIGITAAL           : AfblaasventDigitaal = 1; AfblaasventTerugmelding = 1; break;
        case TYPE_STURING_ANALOOG            : AfblaasventAnaloog  = 1; AfblaasventTerugmelding = 1; break;
        case TYPE_STURING_EBMBUS             : AfblaasventRS485    = 1; DispAdresWijzigen       = 1; break;
        case TYPE_STURING_EBM_MODBUS         : AfblaasventRS485    = 1; DispAdresWijzigen       = 1; break;
        case TYPE_STURING_EC_BLUE_MODBUS     : AfblaasventRS485    = 1; DispAdresWijzigen       = 1; break;
        case TYPE_STURING_EC_BLUE_PREMIUM    : AfblaasventRS485    = 1; DispAdresWijzigen       = 1; break;
        case TYPE_STURING_MB_ROSENBERG       : AfblaasventRS485    = 1; DispAdresWijzigen       = 1; break;
        case TYPE_STURING_MB_CLIMAFAN        : AfblaasventRS485    = 1; DispAdresWijzigen       = 1; break;
        case TYPE_STURING_MB_ROSENBERG_GEN3  : AfblaasventRS485    = 1; DispAdresWijzigen       = 1; break;
        case TYPE_STURING_MB_NICOTRA_GEBHARDT: AfblaasventRS485    = 1; DispAdresWijzigen       = 1; break;
      }
    }
    switch (opt_app.Luchtmengkast[0].Inblaasvent.TypeSturing)
    {
      case TYPE_STURING_DIGITAAL           : InblaasventDigitaal = 1; InblaasventTerugmelding = 1; break;
      case TYPE_STURING_ANALOOG            : InblaasventAnaloog  = 1; InblaasventTerugmelding = 1; break;
      case TYPE_STURING_EBMBUS             : InblaasventRS485    = 1; DispAdresWijzigen       = 1; break;
      case TYPE_STURING_EBM_MODBUS         : InblaasventRS485    = 1; DispAdresWijzigen       = 1; break;
      case TYPE_STURING_EC_BLUE_MODBUS     : InblaasventRS485    = 1; DispAdresWijzigen       = 1; break;
      case TYPE_STURING_EC_BLUE_PREMIUM    : InblaasventRS485    = 1; DispAdresWijzigen       = 1; break;
      case TYPE_STURING_MB_ROSENBERG       : InblaasventRS485    = 1; DispAdresWijzigen       = 1; break;
      case TYPE_STURING_MB_CLIMAFAN        : InblaasventRS485    = 1; DispAdresWijzigen       = 1; break;
      case TYPE_STURING_MB_ROSENBERG_GEN3  : InblaasventRS485    = 1; DispAdresWijzigen       = 1; break;
      case TYPE_STURING_MB_NICOTRA_GEBHARDT: InblaasventRS485    = 1; DispAdresWijzigen       = 1; break;
    }

    for (i = 0; i < MAX_LUCHTMENGKAST; i++)
    {
      if (opt_app.Luchtmengkast[i].Enabled)
      {
        AantalUnits++;
        
        if (opt_app.Luchtmengkast[i].AnaInInblaastemp.board_type && opt_app.Luchtmengkast[i].VerwarmingEnabled)
          DispNaregelen = 1;
        if (opt_app.Luchtmengkast[i].Naregelen)
          Naregelen = 1;
      }
    }
  }
}

//------------------------------------------------------------------------------
static void CheckOptionsServoIn(TOptServo *pServo, unsigned char Enabled, unsigned char Adres)
{
  if (Enabled)
  {
    switch (pServo->TypeSturing)
    {
      case TYPE_STURING_DIGITAAL:
        pServo->AnaIn = IO_empty;
        pServo->Adres  = 0;
        break;
      case TYPE_STURING_ANALOOG:
        pServo->Open  = IO_empty;
        pServo->Close = IO_empty;
        pServo->Adres  = 0;
        break;
	  case TYPE_STURING_CANOPEN:
	  case TYPE_STURING_BACNET:
        pServo->AnaIn  = IO_empty;
        pServo->AnaOut = IO_empty;
        pServo->Open   = IO_empty;
        pServo->Close  = IO_empty;
        pServo->Adres  = Adres;
	    break;
      default:
        pServo->AnaIn  = IO_empty;
        pServo->AnaOut = IO_empty;
        pServo->Open   = IO_empty;
        pServo->Close  = IO_empty;
        pServo->Adres  = 0;
		break;
    }
  }
  else
  {
    pServo->AnaIn  = IO_empty;
    pServo->AnaOut = IO_empty;
    pServo->Open   = IO_empty;
    pServo->Close  = IO_empty;
    pServo->Adres  = 0;
  }
}

static void CheckOptionsServoOut(TOptServo *pServo, unsigned char Enabled)
{
  if (Enabled)
  {
    switch (pServo->TypeSturing)
    {
      case TYPE_STURING_DIGITAAL:
        pServo->AnaOut = IO_empty;
        pServo->Adres  = 0;
        break;
      case TYPE_STURING_ANALOOG:
        pServo->Open  = IO_empty;
        pServo->Close = IO_empty;
        pServo->Adres  = 0;
        break;
      case TYPE_STURING_EBMBUS             :
      case TYPE_STURING_EBM_MODBUS         :
      case TYPE_STURING_EC_BLUE_MODBUS     :
      case TYPE_STURING_EC_BLUE_PREMIUM    :
      case TYPE_STURING_MB_ROSENBERG       :
      case TYPE_STURING_MB_CLIMAFAN        :
      case TYPE_STURING_MB_ROSENBERG_GEN3  :
      case TYPE_STURING_MB_NICOTRA_GEBHARDT:  
        pServo->AnaOut = IO_empty;
        pServo->Open   = IO_empty;
        pServo->Close  = IO_empty;
        break;
      default:
        pServo->AnaIn  = IO_empty;
        pServo->AnaOut = IO_empty;
        pServo->Open   = IO_empty;
        pServo->Close  = IO_empty;
        pServo->Adres  = 0;
        break;
    }
  }
  else
  {
    pServo->AnaIn  = IO_empty;
    pServo->AnaOut = IO_empty;
    pServo->Open   = IO_empty;
    pServo->Close  = IO_empty;
    pServo->Adres  = 0;
  }
}

void CheckOptionsLuchtmengkast(void)
{
int i;

  for (i = 0; i < MAX_LUCHTMENGKAST_GROEP; i++)
  {
    if (module.Luchtmengkast && opt_app.LuchtmengkastGroep[i].Enabled)
    {
      if ((opt_app.LuchtmengkastGroep[0].Buitenklep.TypeSturing == TYPE_STURING_CANOPEN) && !opt_alg.can_backbone)
	    opt_app.LuchtmengkastGroep[0].Buitenklep.TypeSturing = TYPE_STURING_DIGITAAL;

      opt_app.LuchtmengkastGroep[i].Binnenklep.TypeSturing  = opt_app.LuchtmengkastGroep[0].Buitenklep.TypeSturing;
      opt_app.LuchtmengkastGroep[i].Buitenklep.TypeSturing  = opt_app.LuchtmengkastGroep[0].Buitenklep.TypeSturing;
      opt_app.LuchtmengkastGroep[i].Verwarming.TypeSturing  = opt_app.LuchtmengkastGroep[0].Buitenklep.TypeSturing;
      opt_app.LuchtmengkastGroep[i].Afblaasvent.TypeSturing = opt_app.LuchtmengkastGroep[0].Buitenklep.TypeSturing;
      opt_app.LuchtmengkastGroep[i].Inblaasvent.TypeSturing = opt_app.LuchtmengkastGroep[0].Buitenklep.TypeSturing;

      opt_app.LuchtmengkastGroep[i].Binnenklep.Runtime  = opt_app.LuchtmengkastGroep[0].Buitenklep.Runtime;
      opt_app.LuchtmengkastGroep[i].Buitenklep.Runtime  = opt_app.LuchtmengkastGroep[0].Buitenklep.Runtime;
      opt_app.LuchtmengkastGroep[i].Verwarming.Runtime  = opt_app.LuchtmengkastGroep[0].Buitenklep.Runtime;
      opt_app.LuchtmengkastGroep[i].Afblaasvent.Runtime = opt_app.LuchtmengkastGroep[0].Buitenklep.Runtime;
      opt_app.LuchtmengkastGroep[i].Inblaasvent.Runtime = opt_app.LuchtmengkastGroep[0].Buitenklep.Runtime;

      opt_app.LuchtmengkastGroep[i].TypeKlep = opt_app.LuchtmengkastGroep[0].TypeKlep;
      if (opt_app.LuchtmengkastGroep[i].TypeKlep == TYPE_KLEP_BINNEN_BUITEN)
	  {
        opt_app.LuchtmengkastGroep[i].BovenklepEnabled          = 0;
		opt_app.LuchtmengkastGroep[i].BovenklepGekoppeldAanKlep = 0;
	  }
      else
	  {
        opt_app.LuchtmengkastGroep[i].BovenklepEnabled = opt_app.LuchtmengkastGroep[0].BovenklepEnabled;
		if (opt_app.LuchtmengkastGroep[i].BovenklepEnabled)
          opt_app.LuchtmengkastGroep[i].BovenklepGekoppeldAanKlep = opt_app.LuchtmengkastGroep[0].BovenklepGekoppeldAanKlep;
		else
		  opt_app.LuchtmengkastGroep[i].BovenklepGekoppeldAanKlep = 0;
	  }

      opt_app.LuchtmengkastGroep[i].VerwarmingEnabled  = opt_app.LuchtmengkastGroep[0].VerwarmingEnabled;
      opt_app.LuchtmengkastGroep[i].AfblaasventEnabled = opt_app.LuchtmengkastGroep[0].AfblaasventEnabled;
      if (opt_app.LuchtmengkastGroep[i].AfblaasventEnabled)
        opt_app.LuchtmengkastGroep[i].AfblaasventGekoppeldAanKlep = opt_app.LuchtmengkastGroep[0].AfblaasventGekoppeldAanKlep;
      else
        opt_app.LuchtmengkastGroep[i].AfblaasventGekoppeldAanKlep = 0;

      opt_app.LuchtmengkastGroep[i].Naregelen = 0;

      CheckOptionsServoIn(&opt_app.LuchtmengkastGroep[i].Binnenklep,  opt_app.LuchtmengkastGroep[i].BovenklepEnabled || (opt_app.LuchtmengkastGroep[i].TypeKlep == TYPE_KLEP_BINNEN_BUITEN), (i * 4) + 2);
      CheckOptionsServoIn(&opt_app.LuchtmengkastGroep[i].Buitenklep,  1, (i * 4) + 1);
      CheckOptionsServoIn(&opt_app.LuchtmengkastGroep[i].Inblaasvent, 1, (i * 4) + 0);
      CheckOptionsServoIn(&opt_app.LuchtmengkastGroep[i].Afblaasvent, opt_app.LuchtmengkastGroep[i].AfblaasventEnabled, (i * 4) + 3);
      CheckOptionsServoIn(&opt_app.LuchtmengkastGroep[i].Verwarming,  opt_app.LuchtmengkastGroep[i].VerwarmingEnabled,  (i * 4) + 3);
    }
    else
    {
      opt_app.LuchtmengkastGroep[i] = default_opt_app.LuchtmengkastGroep[i];
      ClearAlarm(&alarm_hr_alg.LuchtmengkastGroep[i].Manual, LUCHTMENGKASTGROEP_MANUAL_AL, i);
    }
  }

  for (i = 0; i < MAX_LUCHTMENGKAST; i++)
  {
    if (module.Luchtmengkast && opt_app.Luchtmengkast[i].Enabled && opt_app.LuchtmengkastGroep[opt_app.Luchtmengkast[i].Groep].Enabled)
    {
      opt_app.Luchtmengkast[i].Buitenklep.TypeSturing  = opt_app.Luchtmengkast[0].Buitenklep.TypeSturing;
      opt_app.Luchtmengkast[i].Binnenklep.TypeSturing  = opt_app.Luchtmengkast[0].Binnenklep.TypeSturing;
      opt_app.Luchtmengkast[i].Verwarming.TypeSturing  = opt_app.Luchtmengkast[0].Verwarming.TypeSturing;
      opt_app.Luchtmengkast[i].Afblaasvent.TypeSturing = opt_app.Luchtmengkast[0].Afblaasvent.TypeSturing;
      opt_app.Luchtmengkast[i].Inblaasvent.TypeSturing = opt_app.Luchtmengkast[0].Inblaasvent.TypeSturing;

      opt_app.Luchtmengkast[i].TypeKlep           = opt_app.LuchtmengkastGroep[opt_app.Luchtmengkast[i].Groep].TypeKlep;
      opt_app.Luchtmengkast[i].BovenklepEnabled   = opt_app.LuchtmengkastGroep[opt_app.Luchtmengkast[i].Groep].BovenklepEnabled;
      opt_app.Luchtmengkast[i].AfblaasventEnabled = opt_app.LuchtmengkastGroep[opt_app.Luchtmengkast[i].Groep].AfblaasventEnabled;
      opt_app.Luchtmengkast[i].VerwarmingEnabled  = opt_app.LuchtmengkastGroep[opt_app.Luchtmengkast[i].Groep].VerwarmingEnabled;

      CheckOptionsServoOut(&opt_app.Luchtmengkast[i].Binnenklep,  opt_app.Luchtmengkast[i].BovenklepEnabled || (opt_app.Luchtmengkast[i].TypeKlep == TYPE_KLEP_BINNEN_BUITEN));
      CheckOptionsServoOut(&opt_app.Luchtmengkast[i].Buitenklep,  1);
      CheckOptionsServoOut(&opt_app.Luchtmengkast[i].Inblaasvent, 1);
      CheckOptionsServoOut(&opt_app.Luchtmengkast[i].Afblaasvent, opt_app.Luchtmengkast[i].AfblaasventEnabled);
      CheckOptionsServoOut(&opt_app.Luchtmengkast[i].Verwarming,  opt_app.Luchtmengkast[i].VerwarmingEnabled);

      if (!opt_app.Luchtmengkast[i].AnaInInblaastemp.board_type || !opt_app.Luchtmengkast[i].VerwarmingEnabled)
        opt_app.Luchtmengkast[i].Naregelen = 0;
      if (opt_app.Luchtmengkast[i].Naregelen)
        opt_app.LuchtmengkastGroep[opt_app.Luchtmengkast[i].Groep].Naregelen = 1;

      // Clear alarms witch don't matter anymore
      if (opt_app.Luchtmengkast[i].TypeKlep == TYPE_KLEP_BINNEN_BUITEN)
      {
        ClearAlarm(&alarm_hr_alg.Luchtmengkast[i].RecircklepTargetNotReached, LUCHTMENGKAST_RECIRCKLEP_TARGET_NOT_REACHED_AL, i);
      }
      else
      {
        ClearAlarm(&alarm_hr_alg.Luchtmengkast[i].BuitenklepTargetNotReached, LUCHTMENGKAST_BUITENKLEP_TARGET_NOT_REACHED_AL, i);
        ClearAlarm(&alarm_hr_alg.Luchtmengkast[i].BinnenklepTargetNotReached, LUCHTMENGKAST_BINNENKLEP_TARGET_NOT_REACHED_AL, i);
      }
      if (!opt_app.Luchtmengkast[i].BovenklepEnabled)   ClearAlarm(&alarm_hr_alg.Luchtmengkast[i].BovenklepTargetNotReached,   LUCHTMENGKAST_BOVENKLEP_TARGET_NOT_REACHED_AL,   i);
      if (!opt_app.Luchtmengkast[i].VerwarmingEnabled)  ClearAlarm(&alarm_hr_alg.Luchtmengkast[i].VerwarmingTargetNotReached,  LUCHTMENGKAST_VERWARMING_TARGET_NOT_REACHED_AL,  i);
      if (!opt_app.Luchtmengkast[i].AfblaasventEnabled) ClearAlarm(&alarm_hr_alg.Luchtmengkast[i].AfblaasventTargetNotReached, LUCHTMENGKAST_AFBLAASVENT_TARGET_NOT_REACHED_AL, i);
    }
    else
    {
      opt_app.Luchtmengkast[i] = default_opt_app.Luchtmengkast[i];
      ClearAlarm(&alarm_hr_alg.Luchtmengkast[i].Manual,                      LUCHTMENGKAST_MANUAL_AL,                         i);
      ClearAlarm(&alarm_hr_alg.Luchtmengkast[i].Vorst,                       LUCHTMENGKAST_VORST_AL,                          i);
      ClearAlarm(&alarm_hr_alg.Luchtmengkast[i].Extern,                      LUCHTMENGKAST_EXTERN_AL,                         i);
      ClearAlarm(&alarm_hr_alg.Luchtmengkast[i].Drukverschil,                LUCHTMENGKAST_DRUKVERSCHIL_AL,                   i);
      ClearAlarm(&alarm_hr_alg.Luchtmengkast[i].BuitenklepTargetNotReached,  LUCHTMENGKAST_BUITENKLEP_TARGET_NOT_REACHED_AL,  i);
      ClearAlarm(&alarm_hr_alg.Luchtmengkast[i].BinnenklepTargetNotReached,  LUCHTMENGKAST_BINNENKLEP_TARGET_NOT_REACHED_AL,  i);
      ClearAlarm(&alarm_hr_alg.Luchtmengkast[i].RecircklepTargetNotReached,  LUCHTMENGKAST_RECIRCKLEP_TARGET_NOT_REACHED_AL,  i);
      ClearAlarm(&alarm_hr_alg.Luchtmengkast[i].BovenklepTargetNotReached,   LUCHTMENGKAST_BOVENKLEP_TARGET_NOT_REACHED_AL,   i);
      ClearAlarm(&alarm_hr_alg.Luchtmengkast[i].VerwarmingTargetNotReached,  LUCHTMENGKAST_VERWARMING_TARGET_NOT_REACHED_AL,  i);
      ClearAlarm(&alarm_hr_alg.Luchtmengkast[i].InblaasventTargetNotReached, LUCHTMENGKAST_INBLAASVENT_TARGET_NOT_REACHED_AL, i);
      ClearAlarm(&alarm_hr_alg.Luchtmengkast[i].AfblaasventTargetNotReached, LUCHTMENGKAST_AFBLAASVENT_TARGET_NOT_REACHED_AL, i);

	  mbDeviceClearAllAlarms(opt_app.Luchtmengkast[i].Inblaasvent.Adres, LUCHTMENGKAST_INBLAASVENT_MB_AL, i);
	  mbDeviceClearAllAlarms(opt_app.Luchtmengkast[i].Afblaasvent.Adres, LUCHTMENGKAST_AFBLAASVENT_MB_AL, i);
    }
  }
  LuchtmengkastInit();
}

//------------------------------------------------------------------------------
static unsigned char DummyNotUsed(s_board_IO_on_off IO_new)
{
  IO_new;
  return (1);
}

//------------------------------------------------------------------------------
static unsigned char AnaInTempNotUsed(s_board_IO_on_off IO_new)
{
int i;

  for (i = 0; i < MAX_LUCHTMENGKAST; i++)
  {
    if (Board_IO_Used(IO_new, &opt_app.Luchtmengkast[i].AnaInMengtemp,    1)) return (0);
    if (Board_IO_Used(IO_new, &opt_app.Luchtmengkast[i].AnaInInblaastemp, 1)) return (0);
  }
  return (1);
}

//------------------------------------------------------------------------------
static unsigned char AnaInKlepNotUsed(s_board_IO_on_off IO_new)
{
int i;

  for (i = 0; i < MAX_LUCHTMENGKAST_GROEP; i++)
  {
    if (Board_IO_Used(IO_new, &opt_app.LuchtmengkastGroep[i].Binnenklep.AnaIn, 1)) return (0);
    if (Board_IO_Used(IO_new, &opt_app.LuchtmengkastGroep[i].Buitenklep.AnaIn, 1)) return (0);
  }
  for (i = 0; i < MAX_LUCHTMENGKAST; i++)
  {
    if (Board_IO_Used(IO_new, &opt_app.Luchtmengkast[i].Binnenklep.AnaIn, 1)) return (0);
    if (Board_IO_Used(IO_new, &opt_app.Luchtmengkast[i].Buitenklep.AnaIn, 1)) return (0);
  }
  return (1);
}

static unsigned char AnaInVerwarmNotUsed(s_board_IO_on_off IO_new)
{
int i;

  for (i = 0; i < MAX_LUCHTMENGKAST_GROEP; i++)
  {
    if (Board_IO_Used(IO_new, &opt_app.LuchtmengkastGroep[i].Verwarming.AnaIn,   1)) return (0);
  }
  for (i = 0; i < MAX_LUCHTMENGKAST; i++)
  {
    if (Board_IO_Used(IO_new, &opt_app.Luchtmengkast[i].Verwarming.AnaIn,   1)) return (0);
  }
  return (1);
}

static unsigned char AnaInVentNotUsed(s_board_IO_on_off IO_new)
{
int i;

  for (i = 0; i < MAX_LUCHTMENGKAST_GROEP; i++)
  {
    if (Board_IO_Used(IO_new, &opt_app.LuchtmengkastGroep[i].Afblaasvent.AnaIn,   1)) return (0);
    if (Board_IO_Used(IO_new, &opt_app.LuchtmengkastGroep[i].Inblaasvent.AnaIn, 1)) return (0);
  }
  for (i = 0; i < MAX_LUCHTMENGKAST; i++)
  {
    if (Board_IO_Used(IO_new, &opt_app.Luchtmengkast[i].Afblaasvent.AnaIn,   1)) return (0);
    if (Board_IO_Used(IO_new, &opt_app.Luchtmengkast[i].Inblaasvent.AnaIn, 1)) return (0);
  }
  for (i = 0; i < MAX_GROUP; i++)
  {
    if (Board_IO_Used(IO_new, &opt_app.Motorgroup[i].AnaInPosition, 1)) return (0);
  }
  return (1);
}

//------------------------------------------------------------------------------
static unsigned char DigInKlepNotUsed(s_board_IO_on_off IO_new)
{
int i;

  for (i = 0; i < MAX_LUCHTMENGKAST_GROEP; i++)
  {
    if (Board_IO_Used(IO_new, &opt_app.LuchtmengkastGroep[i].Binnenklep.Open,  1)) return (0);
    if (Board_IO_Used(IO_new, &opt_app.LuchtmengkastGroep[i].Binnenklep.Close, 1)) return (0);
    if (Board_IO_Used(IO_new, &opt_app.LuchtmengkastGroep[i].Buitenklep.Open,  1)) return (0);
    if (Board_IO_Used(IO_new, &opt_app.LuchtmengkastGroep[i].Buitenklep.Close, 1)) return (0);
  }
  return (1);
}

static unsigned char DigInVerwarmNotUsed(s_board_IO_on_off IO_new)
{
int i;

  for (i = 0; i < MAX_LUCHTMENGKAST_GROEP; i++)
  {
    if (Board_IO_Used(IO_new, &opt_app.LuchtmengkastGroep[i].Verwarming.Open,  1)) return (0);
    if (Board_IO_Used(IO_new, &opt_app.LuchtmengkastGroep[i].Verwarming.Close, 1)) return (0);
  }
  return (1);
}

static unsigned char DigInVentNotUsed(s_board_IO_on_off IO_new)
{
int i;

  for (i = 0; i < MAX_LUCHTMENGKAST_GROEP; i++)
  {
    if (Board_IO_Used(IO_new, &opt_app.LuchtmengkastGroep[i].Afblaasvent.Open,  1)) return (0);
    if (Board_IO_Used(IO_new, &opt_app.LuchtmengkastGroep[i].Afblaasvent.Close, 1)) return (0);
    if (Board_IO_Used(IO_new, &opt_app.LuchtmengkastGroep[i].Inblaasvent.Open,  1)) return (0);
    if (Board_IO_Used(IO_new, &opt_app.LuchtmengkastGroep[i].Inblaasvent.Close, 1)) return (0);
  }
  for (i = 0; i < MAX_GROUP; i++)
  {
    if (Board_IO_Used(IO_new, &opt_app.Ventgroup[i].DigInOnOff,  1)) return (0);
    if (Board_IO_Used(IO_new, &opt_app.Motorgroup[i].DigInOpen,  1)) return (0);
    if (Board_IO_Used(IO_new, &opt_app.Motorgroup[i].DigInClose, 1)) return (0);
  }
  return (1);
}

static unsigned char DigInAlarmNotUsed(s_board_IO_on_off IO_new)
{
int i;

  for (i = 0; i < MAX_LUCHTMENGKAST; i++)
  {
    if (Board_IO_Used(IO_new, &opt_app.Luchtmengkast[i].DigInAlarm,        1)) return (0);
    if (Board_IO_Used(IO_new, &opt_app.Luchtmengkast[i].DigInDrukverschil, 1)) return (0);
  }
  return (1);
}

//------------------------------------------------------------------------------
static unsigned char AnaOutKlepNotUsed(s_board_IO_on_off IO_new)
{
int i;

  for (i = 0; i < MAX_LUCHTMENGKAST_GROEP; i++)
  {
    if (Board_IO_Used(IO_new, &opt_app.LuchtmengkastGroep[i].Binnenklep.AnaOut, 1)) return (0);
    if (Board_IO_Used(IO_new, &opt_app.LuchtmengkastGroep[i].Buitenklep.AnaOut, 1)) return (0);
  }
  for (i = 0; i < MAX_LUCHTMENGKAST; i++)
  {
    if (Board_IO_Used(IO_new, &opt_app.Luchtmengkast[i].Binnenklep.AnaOut, 1)) return (0);
    if (Board_IO_Used(IO_new, &opt_app.Luchtmengkast[i].Buitenklep.AnaOut, 1)) return (0);
  }
  return (1);
}

static unsigned char AnaOutVerwarmNotUsed(s_board_IO_on_off IO_new)
{
int i;

  for (i = 0; i < MAX_LUCHTMENGKAST_GROEP; i++)
  {
    if (Board_IO_Used(IO_new, &opt_app.LuchtmengkastGroep[i].Verwarming.AnaOut, 1)) return (0);
  }
  for (i = 0; i < MAX_LUCHTMENGKAST; i++)
  {
    if (Board_IO_Used(IO_new, &opt_app.Luchtmengkast[i].Verwarming.AnaOut, 1)) return (0);
  }
  return (1);
}

static unsigned char AnaOutVentNotUsed(s_board_IO_on_off IO_new)
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

//------------------------------------------------------------------------------
static unsigned char DigOutKlepNotUsed(s_board_IO_on_off IO_new)
{
int i;

  for (i = 0; i < MAX_LUCHTMENGKAST; i++)
  {
    if (Board_IO_Used(IO_new, &opt_app.Luchtmengkast[i].Binnenklep.Open,  1)) return (0);
    if (Board_IO_Used(IO_new, &opt_app.Luchtmengkast[i].Binnenklep.Close, 1)) return (0);
    if (Board_IO_Used(IO_new, &opt_app.Luchtmengkast[i].Buitenklep.Open,  1)) return (0);
    if (Board_IO_Used(IO_new, &opt_app.Luchtmengkast[i].Buitenklep.Close, 1)) return (0);
  }
  return (1);
}

static unsigned char DigOutVerwarmNotUsed(s_board_IO_on_off IO_new)
{
int i;

  for (i = 0; i < MAX_LUCHTMENGKAST; i++)
  {
    if (Board_IO_Used(IO_new, &opt_app.Luchtmengkast[i].Verwarming.Open,   1)) return (0);
    if (Board_IO_Used(IO_new, &opt_app.Luchtmengkast[i].Verwarming.Close,  1)) return (0);
  }
  return (1);
}

static unsigned char DigOutVentNotUsed(s_board_IO_on_off IO_new)
{
int i;

  for (i = 0; i < MAX_LUCHTMENGKAST; i++)
  {
    if (Board_IO_Used(IO_new, &opt_app.Luchtmengkast[i].Afblaasvent.Open,  1)) return (0);
    if (Board_IO_Used(IO_new, &opt_app.Luchtmengkast[i].Afblaasvent.Close, 1)) return (0);
    if (Board_IO_Used(IO_new, &opt_app.Luchtmengkast[i].Inblaasvent.Open,  1)) return (0);
    if (Board_IO_Used(IO_new, &opt_app.Luchtmengkast[i].Inblaasvent.Close, 1)) return (0);
  }
  return (1);
}

static unsigned char DigOutAlarmNotUsed(s_board_IO_on_off IO_new)
{
int i;

  if (Board_IO_Used(IO_new, &opt_app.Alarm.DigOutZacht, 1)) return (0);
  for (i = 0; i < MAX_LUCHTMENGKAST_GROEP; i++)
  {
    if (Board_IO_Used(IO_new, &opt_app.LuchtmengkastGroep[i].DigOutAlarm, 1)) return (0);
  }
  for (i = 0; i < MAX_GROUP; i++)
  {
    if (Board_IO_Used(IO_new, &opt_app.Motorgroup[i].DigOutAlarm,      1)) return (0);
    if (Board_IO_Used(IO_new, &opt_app.Motorgroup[i].DigOutAlarmFlap,  1)) return (0);
    if (Board_IO_Used(IO_new, &opt_app.Ventgroup[i].DigOutAlarmUrgent, 1)) return (0);
  }
  return (1);
}

//------------------------------------------------------------------------------
static void CopyTempIOToServo(TOptServo *pServo, unsigned char nr)
{
  pServo->Open   = TempHoger[nr];
  pServo->Close  = TempLager[nr];
  pServo->AnaIn  = TempAnaIn[nr];
  pServo->AnaOut = TempAnaOut[nr];
}

static void CopyServoToTempIO(TOptServo *pServo, unsigned char nr)
{
  TempHoger[nr]  = pServo->Open;
  TempLager[nr]  = pServo->Close;
  TempAnaIn[nr]  = pServo->AnaIn;
  TempAnaOut[nr] = pServo->AnaOut;
}

static void Disp_Copy_Buitenklep_IO(void)
{
int i;

  if (index_array == 0)
  {
    if (CopyTempIO)
    {
      Install_Copy_Board_IO_To_IO();
      for (i = 0; i < MAX_LUCHTMENGKAST_GROEP; i++)
        CopyTempIOToServo(&opt_app.LuchtmengkastGroep[i].Buitenklep, i);
      CopyTempIO = 0;
    }
    else
    {
      for (i = 0; i < MAX_LUCHTMENGKAST_GROEP; i++)
        CopyServoToTempIO(&opt_app.LuchtmengkastGroep[i].Buitenklep, i);
    }
  }
  else
  {
    CopyTempIO = 1;
  }
}

static void Disp_Copy_Binnenklep_IO(void)
{
int i;

  if (index_array == 0)
  {
    if (CopyTempIO)
    {
      Install_Copy_Board_IO_To_IO();
      for (i = 0; i < MAX_LUCHTMENGKAST_GROEP; i++)
        CopyTempIOToServo(&opt_app.LuchtmengkastGroep[i].Binnenklep, i);
      CopyTempIO = 0;
    }
    else
    {
      for (i = 0; i < MAX_LUCHTMENGKAST_GROEP; i++)
        CopyServoToTempIO(&opt_app.LuchtmengkastGroep[i].Binnenklep, i);
    }
  }
  else
  {
    CopyTempIO = 1;
  }
}

static void Disp_Copy_Verwarming_IO(void)
{
int i;

  if (index_array == 0)
  {
    if (CopyTempIO)
    {
      Install_Copy_Board_IO_To_IO();
      for (i = 0; i < MAX_LUCHTMENGKAST_GROEP; i++)
        CopyTempIOToServo(&opt_app.LuchtmengkastGroep[i].Verwarming, i);
      CopyTempIO = 0;
    }
    else
    {
      for (i = 0; i < MAX_LUCHTMENGKAST_GROEP; i++)
        CopyServoToTempIO(&opt_app.LuchtmengkastGroep[i].Verwarming, i);
    }
  }
  else
  {
    CopyTempIO = 1;
  }
}

static void Disp_Copy_Inblaasvent_IO(void)
{
int i;

  if (index_array == 0)
  {
    if (CopyTempIO)
    {
      Install_Copy_Board_IO_To_IO();
      for (i = 0; i < MAX_LUCHTMENGKAST_GROEP; i++)
        CopyTempIOToServo(&opt_app.LuchtmengkastGroep[i].Inblaasvent, i);
      CopyTempIO = 0;
    }
    else
    {
      for (i = 0; i < MAX_LUCHTMENGKAST_GROEP; i++)
        CopyServoToTempIO(&opt_app.LuchtmengkastGroep[i].Inblaasvent, i);
    }
  }
  else
  {
    CopyTempIO = 1;
  }
}

static void Disp_Copy_Afblaasvent_IO(void)
{
int i;

  if (index_array == 0)
  {
    if (CopyTempIO)
    {
      Install_Copy_Board_IO_To_IO();
      for (i = 0; i < MAX_LUCHTMENGKAST_GROEP; i++)
        CopyTempIOToServo(&opt_app.LuchtmengkastGroep[i].Afblaasvent, i);
      CopyTempIO = 0;
    }
    else
    {
      for (i = 0; i < MAX_LUCHTMENGKAST_GROEP; i++)
        CopyServoToTempIO(&opt_app.LuchtmengkastGroep[i].Afblaasvent, i);
    }
  }
  else
  {
    CopyTempIO = 1;
  }
}

static void Disp_Copy_Alarm_IO(void)
{
int i;

  if (index_array == 0)
  {
    if (CopyTempIO)
    {
      Install_Copy_Board_IO_To_IO();
      for (i = 0; i < MAX_LUCHTMENGKAST_GROEP; i++)
        opt_app.LuchtmengkastGroep[i].DigOutAlarm = TempDigOut[i];
      CopyTempIO = 0;
    }
    else
    {
      for (i = 0; i < MAX_LUCHTMENGKAST_GROEP; i++)
        TempDigOut[i] = opt_app.LuchtmengkastGroep[i].DigOutAlarm;
    }
  }
  else
  {
    CopyTempIO = 1;
  }
}

static void Disp_Copy_Inblaastemp_IO(void)
{
int i;

  if (index_array == 0)
  {
    if (CopyTempIO)
    {
      Install_Copy_Board_IO_To_IO();
      for (i = 0; i < MAX_LUCHTMENGKAST; i++)
        opt_app.Luchtmengkast[i].AnaInInblaastemp = TempAnaIn[i];
      SetDisplayOptions();
      Refresh_Screen_Nr_Aantal();
      CopyTempIO = 0;
    }
    else
    {
      for (i = 0; i < MAX_LUCHTMENGKAST; i++)
        TempAnaIn[i] = opt_app.Luchtmengkast[i].AnaInInblaastemp;
    }
  }
  else
  {
    CopyTempIO = 1;
  }
}

static void Disp_Copy_Mengtemp_IO(void)
{
int i;

  if (index_array == 0)
  {
    if (CopyTempIO)
    {
      Install_Copy_Board_IO_To_IO();
      for (i = 0; i < MAX_LUCHTMENGKAST; i++)
        opt_app.Luchtmengkast[i].AnaInMengtemp = TempAnaIn[i];
      CopyTempIO = 0;
    }
    else
    {
      for (i = 0; i < MAX_LUCHTMENGKAST; i++)
        TempAnaIn[i] = opt_app.Luchtmengkast[i].AnaInMengtemp;
    }
  }
  else
  {
    CopyTempIO = 1;
  }
}

static void Disp_Copy_Streeftemp_IO(void)
{
int i;

  if (index_array == 0)
  {
    if (CopyTempIO)
    {
      Install_Copy_Board_IO_To_IO();
      for (i = 0; i < MAX_LUCHTMENGKAST_GROEP; i++)
        opt_app.LuchtmengkastGroep[i].Streeftemp = TempAnaIn[i];
      CopyTempIO = 0;
    }
    else
    {
      for (i = 0; i < MAX_LUCHTMENGKAST_GROEP; i++)
        TempAnaIn[i] = opt_app.LuchtmengkastGroep[i].Streeftemp;
    }
  }
  else
  {
    CopyTempIO = 1;
  }
}

static void Disp_Copy_Vorst_IO(void)
{
int i;

  if (index_array == 0)
  {
    if (CopyTempIO)
    {
      Install_Copy_Board_IO_To_IO();
      for (i = 0; i < MAX_LUCHTMENGKAST; i++)
        opt_app.Luchtmengkast[i].DigInVorst = TempAnaIn[i];
      CopyTempIO = 0;
    }
    else
    {
      for (i = 0; i < MAX_LUCHTMENGKAST; i++)
        TempAnaIn[i] = opt_app.Luchtmengkast[i].DigInVorst;
    }
  }
  else
  {
    CopyTempIO = 1;
  }
}

static void Disp_Copy_Alarm_In_IO(void)
{
int i;

  if (index_array == 0)
  {
    if (CopyTempIO)
    {
      Install_Copy_Board_IO_To_IO();
      for (i = 0; i < MAX_LUCHTMENGKAST; i++)
        opt_app.Luchtmengkast[i].DigInAlarm = TempAnaIn[i];
      CopyTempIO = 0;
    }
    else
    {
      for (i = 0; i < MAX_LUCHTMENGKAST; i++)
        TempAnaIn[i] = opt_app.Luchtmengkast[i].DigInAlarm;
    }
  }
  else
  {
    CopyTempIO = 1;
  }
}

static void Disp_Copy_Drukverschil_IO(void)
{
int i;

  if (index_array == 0)
  {
    if (CopyTempIO)
    {
      Install_Copy_Board_IO_To_IO();
      for (i = 0; i < MAX_LUCHTMENGKAST; i++)
        opt_app.Luchtmengkast[i].DigInDrukverschil = TempAnaIn[i];
      CopyTempIO = 0;
    }
    else
    {
      for (i = 0; i < MAX_LUCHTMENGKAST; i++)
        TempAnaIn[i] = opt_app.Luchtmengkast[i].DigInDrukverschil;
    }
  }
  else
  {
    CopyTempIO = 1;
  }
}

static void Disp_Copy_Unit_Buitenklep_IO(void)
{
int i;

  if (index_array == 0)
  {
    if (CopyTempIO)
    {
      Install_Copy_Board_IO_To_IO();
      for (i = 0; i < MAX_LUCHTMENGKAST; i++)
        CopyTempIOToServo(&opt_app.Luchtmengkast[i].Buitenklep, i);
      CopyTempIO = 0;
    }
    else
    {
      for (i = 0; i < MAX_LUCHTMENGKAST; i++)
        CopyServoToTempIO(&opt_app.Luchtmengkast[i].Buitenklep, i);
    }
  }
  else
  {
    CopyTempIO = 1;
  }
}

static void Disp_Copy_Unit_Binnenklep_IO(void)
{
int i;

  if (index_array == 0)
  {
    if (CopyTempIO)
    {
      Install_Copy_Board_IO_To_IO();
      for (i = 0; i < MAX_LUCHTMENGKAST; i++)
        CopyTempIOToServo(&opt_app.Luchtmengkast[i].Binnenklep, i);
      CopyTempIO = 0;
    }
    else
    {
      for (i = 0; i < MAX_LUCHTMENGKAST; i++)
        CopyServoToTempIO(&opt_app.Luchtmengkast[i].Binnenklep, i);
    }
  }
  else
  {
    CopyTempIO = 1;
  }
}

static void Disp_Copy_Unit_Verwarming_IO(void)
{
int i;

  if (index_array == 0)
  {
    if (CopyTempIO)
    {
      Install_Copy_Board_IO_To_IO();
      for (i = 0; i < MAX_LUCHTMENGKAST; i++)
        CopyTempIOToServo(&opt_app.Luchtmengkast[i].Verwarming, i);
      SetDisplayOptions();
      Refresh_Screen_Nr_Aantal();
      CopyTempIO = 0;
    }
    else
    {
      for (i = 0; i < MAX_LUCHTMENGKAST; i++)
        CopyServoToTempIO(&opt_app.Luchtmengkast[i].Verwarming, i);
    }
  }
  else
  {
    CopyTempIO = 1;
  }
}

static void Disp_Copy_Unit_Inblaasvent_IO(void)
{
int i;

  if (index_array == 0)
  {
    if (CopyTempIO)
    {
      Install_Copy_Board_IO_To_IO();
      for (i = 0; i < MAX_LUCHTMENGKAST; i++)
        CopyTempIOToServo(&opt_app.Luchtmengkast[i].Inblaasvent, i);
      CopyTempIO = 0;
    }
    else
    {
      for (i = 0; i < MAX_LUCHTMENGKAST; i++)
        CopyServoToTempIO(&opt_app.Luchtmengkast[i].Inblaasvent, i);
    }
  }
  else
  {
    CopyTempIO = 1;
  }
}

static void Disp_Copy_Unit_Afblaasvent_IO(void)
{
int i;

  if (index_array == 0)
  {
    if (CopyTempIO)
    {
      Install_Copy_Board_IO_To_IO();
      for (i = 0; i < MAX_LUCHTMENGKAST; i++)
        CopyTempIOToServo(&opt_app.Luchtmengkast[i].Afblaasvent, i);
      CopyTempIO = 0;
    }
    else
    {
      for (i = 0; i < MAX_LUCHTMENGKAST; i++)
        CopyServoToTempIO(&opt_app.Luchtmengkast[i].Afblaasvent, i);
    }
  }
  else
  {
    CopyTempIO = 1;
  }
}

//-----------------------------------------------------------------------------
static void Disp_Copy_Units(unsigned char nr)
{
int i;

  if (index_array == 0)
  {
    if (CopyTempIO)
    {
      Install_Copy_Board_IO_To_Option();
      for (i = 0; i < AantalUnits; i++)
      {
        if (GroepUnits[i])
          opt_app.Luchtmengkast[i].Groep = nr;
        else if (opt_app.Luchtmengkast[i].Groep == nr)
          opt_app.Luchtmengkast[i].Groep++;
      }
      CopyTempIO = 0;
    }
    else
    {
      for (i = 0; i < AantalUnits; i++)
      {
        if (opt_app.Luchtmengkast[i].Groep == nr)
          GroepUnits[i] = 1;
        else
          GroepUnits[i] = 0;
      }
    }
  }
  else
  {
    CopyTempIO = 1;
  }
}

static void Disp_Copy_Units_1(void) { Disp_Copy_Units(0); }
static void Disp_Copy_Units_2(void) { Disp_Copy_Units(1); }
static void Disp_Copy_Units_3(void) { Disp_Copy_Units(2); }
static void Disp_Copy_Units_4(void) { Disp_Copy_Units(3); }

//-----------------------------------------------------------------------------
static void Key_Luchtmengkast_Value(void)
{
int i;

  if (screen_ptr->index == 0)
  {
    for (i = 0; i < AantalGroepen; i++)
      opt_app.LuchtmengkastGroep[i].Enabled = 1;

    for (i = AantalGroepen; i < MAX_LUCHTMENGKAST_GROEP; i++)
      opt_app.LuchtmengkastGroep[i].Enabled = 0;

    for (i = 0; i < AantalUnits; i++)
    {
      opt_app.Luchtmengkast[i].Enabled = 1;
      switch (opt_app.Luchtmengkast[i].Afblaasvent.TypeSturing)
      {
        case TYPE_STURING_ANALOOG            : Install_Control_Board_IO(&opt_app.Luchtmengkast[i].Afblaasvent.AnaIn, 1, 1, 0, ANALOG_INPUT_ID, ANA_IN_VENT); break;
        case TYPE_STURING_EBMBUS             : Install_Control_Board_IO(&opt_app.Luchtmengkast[i].Afblaasvent.AnaIn, 1, 1, 1, RS485_BUS_ID,    0          ); break;
        case TYPE_STURING_EBM_MODBUS         : Install_Control_Board_IO(&opt_app.Luchtmengkast[i].Afblaasvent.AnaIn, 1, 1, 1, RS485_BUS_ID,    0          ); break;
        case TYPE_STURING_EC_BLUE_MODBUS     : Install_Control_Board_IO(&opt_app.Luchtmengkast[i].Afblaasvent.AnaIn, 1, 1, 1, RS485_BUS_ID,    0          ); break;
        case TYPE_STURING_EC_BLUE_PREMIUM    : Install_Control_Board_IO(&opt_app.Luchtmengkast[i].Afblaasvent.AnaIn, 1, 1, 1, RS485_BUS_ID,    0          ); break;
        case TYPE_STURING_MB_ROSENBERG       : Install_Control_Board_IO(&opt_app.Luchtmengkast[i].Afblaasvent.AnaIn, 1, 1, 1, RS485_BUS_ID,    0          ); break;
        case TYPE_STURING_MB_CLIMAFAN        : Install_Control_Board_IO(&opt_app.Luchtmengkast[i].Afblaasvent.AnaIn, 1, 1, 1, RS485_BUS_ID,    0          ); break;
        case TYPE_STURING_MB_ROSENBERG_GEN3  : Install_Control_Board_IO(&opt_app.Luchtmengkast[i].Afblaasvent.AnaIn, 1, 1, 1, RS485_BUS_ID,    0          ); break;
        case TYPE_STURING_MB_NICOTRA_GEBHARDT: Install_Control_Board_IO(&opt_app.Luchtmengkast[i].Afblaasvent.AnaIn, 1, 1, 1, RS485_BUS_ID,    0          ); break;
      }
      switch (opt_app.Luchtmengkast[i].Inblaasvent.TypeSturing)
      {
        case TYPE_STURING_ANALOOG            : Install_Control_Board_IO(&opt_app.Luchtmengkast[i].Inblaasvent.AnaIn, 1, 1, 0, ANALOG_INPUT_ID, ANA_IN_VENT); break;
        case TYPE_STURING_EBMBUS             : Install_Control_Board_IO(&opt_app.Luchtmengkast[i].Inblaasvent.AnaIn, 1, 1, 1, RS485_BUS_ID,    0          ); break;
        case TYPE_STURING_EBM_MODBUS         : Install_Control_Board_IO(&opt_app.Luchtmengkast[i].Inblaasvent.AnaIn, 1, 1, 1, RS485_BUS_ID,    0          ); break;
        case TYPE_STURING_EC_BLUE_MODBUS     : Install_Control_Board_IO(&opt_app.Luchtmengkast[i].Inblaasvent.AnaIn, 1, 1, 1, RS485_BUS_ID,    0          ); break;
        case TYPE_STURING_EC_BLUE_PREMIUM    : Install_Control_Board_IO(&opt_app.Luchtmengkast[i].Inblaasvent.AnaIn, 1, 1, 1, RS485_BUS_ID,    0          ); break;
        case TYPE_STURING_MB_ROSENBERG       : Install_Control_Board_IO(&opt_app.Luchtmengkast[i].Inblaasvent.AnaIn, 1, 1, 1, RS485_BUS_ID,    0          ); break;
        case TYPE_STURING_MB_CLIMAFAN        : Install_Control_Board_IO(&opt_app.Luchtmengkast[i].Inblaasvent.AnaIn, 1, 1, 1, RS485_BUS_ID,    0          ); break;
        case TYPE_STURING_MB_ROSENBERG_GEN3  : Install_Control_Board_IO(&opt_app.Luchtmengkast[i].Inblaasvent.AnaIn, 1, 1, 1, RS485_BUS_ID,    0          ); break;
        case TYPE_STURING_MB_NICOTRA_GEBHARDT: Install_Control_Board_IO(&opt_app.Luchtmengkast[i].Inblaasvent.AnaIn, 1, 1, 1, RS485_BUS_ID,    0          ); break;
      }

      if (Naregelen && opt_app.Luchtmengkast[i].AnaInInblaastemp.board_type && opt_app.Luchtmengkast[i].VerwarmingEnabled)
        opt_app.Luchtmengkast[i].Naregelen = 1;
      else
        opt_app.Luchtmengkast[i].Naregelen = 0;
    }

    for (i = AantalUnits; i < MAX_LUCHTMENGKAST; i++)
      opt_app.Luchtmengkast[i].Enabled = 0;

    CheckOptionsLuchtmengkast();
    SetDisplayOptions();
    Refresh_Screen_Nr_Aantal();
  }
}

static void Arrow_Type_Sturing_Value(void)
{
  do
  {
    Arrow_Scroll_Option_Value();
  } while ((opt_app.LuchtmengkastGroep[0].Buitenklep.TypeSturing == TYPE_STURING_CANOPEN) && !opt_alg.can_backbone);
  Key_Luchtmengkast_Value();
}

static void Arrow_Luchtmengkast_Vent_Value(void)
{
  switch (key)
  {
	case LEFT:
	  Arrow_Left_Value();
	  break;
	case RIGHT:
	  Increment_Func_Index();
	  break;
    case UP:
	  switch (screen_ptr->value)
	  {
        case TYPE_STURING_DIGITAAL           : screen_ptr->value = TYPE_STURING_ANALOOG;             break;
        case TYPE_STURING_ANALOOG            : screen_ptr->value = TYPE_STURING_EBMBUS;              break;
        case TYPE_STURING_EBMBUS             : screen_ptr->value = TYPE_STURING_EBM_MODBUS;          break;
        case TYPE_STURING_EBM_MODBUS         : screen_ptr->value = TYPE_STURING_EC_BLUE_MODBUS;      break;
        case TYPE_STURING_EC_BLUE_MODBUS     : screen_ptr->value = TYPE_STURING_EC_BLUE_PREMIUM;     break;
        case TYPE_STURING_EC_BLUE_PREMIUM    : screen_ptr->value = TYPE_STURING_MB_ROSENBERG;        break;
        case TYPE_STURING_MB_ROSENBERG       : screen_ptr->value = TYPE_STURING_MB_ROSENBERG_GEN3;   break;
        case TYPE_STURING_MB_CLIMAFAN        : screen_ptr->value = TYPE_STURING_MB_NICOTRA_GEBHARDT; break;
        case TYPE_STURING_MB_ROSENBERG_GEN3  : screen_ptr->value = TYPE_STURING_MB_CLIMAFAN;         break;
        case TYPE_STURING_MB_NICOTRA_GEBHARDT: screen_ptr->value = TYPE_STURING_DIGITAAL;            break;
	  }
      Put_Value();
      option_change_flag = 1;
      break;
	case DOWN:
	  switch (screen_ptr->value)
	  {
        case TYPE_STURING_DIGITAAL           : screen_ptr->value = TYPE_STURING_MB_NICOTRA_GEBHARDT; break;
        case TYPE_STURING_ANALOOG            : screen_ptr->value = TYPE_STURING_DIGITAAL;            break;
        case TYPE_STURING_EBMBUS             : screen_ptr->value = TYPE_STURING_ANALOOG;             break;
        case TYPE_STURING_EBM_MODBUS         : screen_ptr->value = TYPE_STURING_EBMBUS;              break;
        case TYPE_STURING_EC_BLUE_MODBUS     : screen_ptr->value = TYPE_STURING_EBM_MODBUS;          break;
        case TYPE_STURING_EC_BLUE_PREMIUM    : screen_ptr->value = TYPE_STURING_EC_BLUE_MODBUS;      break;
        case TYPE_STURING_MB_ROSENBERG       : screen_ptr->value = TYPE_STURING_EC_BLUE_PREMIUM;     break;
        case TYPE_STURING_MB_CLIMAFAN        : screen_ptr->value = TYPE_STURING_MB_ROSENBERG_GEN3;   break;
        case TYPE_STURING_MB_ROSENBERG_GEN3  : screen_ptr->value = TYPE_STURING_MB_ROSENBERG;        break;
        case TYPE_STURING_MB_NICOTRA_GEBHARDT: screen_ptr->value = TYPE_STURING_MB_CLIMAFAN;         break;
	  }
      Put_Value();
      option_change_flag = 1;
	  break;
  }
  Key_Luchtmengkast_Value();
  if (screen_ptr->index == 0)
    mbDeviceInit();
}

static void Enter_Luchtmengkast_Vent_Value(void)
{
  Increment_Func_Index_Enter_Option_Value();
  Key_Luchtmengkast_Value();
  if (screen_ptr->index == 0)
    mbDeviceInit();
}

static void Arrow_Luchtmengkast_Scroll_Value(void)
{
  Arrow_Scroll_Option_Value();
  Key_Luchtmengkast_Value();
}

static void Arrow_Luchtmengkast_Value(void)
{
  Arrow_Option_Value();
  Key_Luchtmengkast_Value();
}

static void Enter_Luchtmengkast_Value(void)
{
  Increment_Func_Index_Enter_Option_Value();
  Key_Luchtmengkast_Value();
}

//-----------------------------------------------------------------------------
static void Arrow_IO_Vent_Select_Func(void)
{
  Arrow_IO_Select_Func();
  if (screen_ptr->index == 0)
    mbDeviceInit();
}

static void Enter_IO_Vent_Select_Func(void)
{
  Enter_IO_Select_Func();
  if (screen_ptr->index == 0)
    mbDeviceInit();
}

//-----------------------------------------------------------------------------
static void Disp_Adres_Inblaasvent_Func(void)
{
  if (screen_ptr->index == 0)
  {
    VentNummer = 1;
    VentAdres  = opt_app.Luchtmengkast[0].Inblaasvent.Adres;
  }
}

static unsigned char AdresInblaasventOk(int Adres)
{
int i;
int Last;

  if (Adres == 0)
    return (1);
  
  for (i = 0; i < opt_app.NumberMotorgroups; i++)
  {
    Last = opt_app.Motorgroup[i].FirstAddress + opt_app.Motorgroup[i].NumberMotors - 1;
    if ((Adres >= opt_app.Motorgroup[i].FirstAddress) && (Adres <= Last))
	  return (0);
  }
  for (i = 0; i < opt_app.Sensoren.Drukverschil; i++)
  {
    Last = opt_app.Sensoren.FirstAddress + opt_app.Sensoren.Drukverschil - 1;
    if ((Adres >= opt_app.Sensoren.FirstAddress) && (Adres <= Last))
	  return (0);
  }
  for (i = 0; i < MAX_LUCHTMENGKAST; i++)
  {
    if (opt_app.Luchtmengkast[i].Enabled)
    {
      if ((i != (VentNummer - 1)) && (Adres == opt_app.Luchtmengkast[i].Inblaasvent.Adres))
        return (0);
	  switch (opt_app.Luchtmengkast[i].Afblaasvent.TypeSturing)
	  {
        case TYPE_STURING_EBMBUS             :
        case TYPE_STURING_EBM_MODBUS         :
        case TYPE_STURING_EC_BLUE_MODBUS     :
        case TYPE_STURING_EC_BLUE_PREMIUM    :
        case TYPE_STURING_MB_ROSENBERG       :
        case TYPE_STURING_MB_CLIMAFAN        :
        case TYPE_STURING_MB_ROSENBERG_GEN3  :
        case TYPE_STURING_MB_NICOTRA_GEBHARDT:
          if (Adres == opt_app.Luchtmengkast[i].Afblaasvent.Adres)
            return (0);
	  }
    }
  }
  return (1);
}

static void Arrow_Adres_Inblaasvent_Value(void)
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
      while (AdresInblaasventOk(screen_ptr->value) == 0);
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
      while (AdresInblaasventOk(screen_ptr->value) == 0);
      if (old_val != screen_ptr->value)
      {
        option_change_flag = 1;
        Put_Value();
      }
      break;
    case LEFT:
      opt_app.Luchtmengkast[VentNummer - 1].Inblaasvent.Adres = VentAdres;
      if (VentNummer > 1)
      {
        VentNummer--;
        VentAdres = opt_app.Luchtmengkast[VentNummer - 1].Inblaasvent.Adres;
        Get_Value();
      }
      else
        Arrow_Left_Value();
      break;
    case RIGHT:
      opt_app.Luchtmengkast[VentNummer - 1].Inblaasvent.Adres = VentAdres;
      if (VentNummer < AantalUnits)
      {
        VentNummer++;
        VentAdres = opt_app.Luchtmengkast[VentNummer - 1].Inblaasvent.Adres;
        Get_Value();
      }
      else
        Increment_Func_Index();
      break;
  }
}

static void Enter_Adres_Inblaasvent_Value(void)
{
  if (screen_ptr->change_flag)
  {
    if ((screen_ptr->value >= screen_ptr->min_value) && (screen_ptr->value <= screen_ptr->max_value))
    {
      if ((VentAdres != screen_ptr->value) && AdresInblaasventOk(screen_ptr->value))
        Enter_Value();
    }
  }
  opt_app.Luchtmengkast[VentNummer - 1].Inblaasvent.Adres = VentAdres;
  if (VentNummer < AantalUnits)
  {
    VentNummer++;
    VentAdres = opt_app.Luchtmengkast[VentNummer - 1].Inblaasvent.Adres;
    Get_Value();
  }
  else
    Increment_Func_Index();
}

static void Disp_Adres_Afblaasvent_Func(void)
{
  if (screen_ptr->index == 0)
  {
    VentNummer = 1;
    VentAdres = opt_app.Luchtmengkast[0].Afblaasvent.Adres;
  }
}

static unsigned char AdresAfblaasventOk(int Adres)
{
int i;
int Last;

  if (Adres == 0)
    return (1);
  
  for (i = 0; i < opt_app.NumberMotorgroups; i++)
  {
    Last = opt_app.Motorgroup[i].FirstAddress + opt_app.Motorgroup[i].NumberMotors - 1;
    if ((Adres >= opt_app.Motorgroup[i].FirstAddress) && (Adres <= Last))
	  return (0);
  }
  for (i = 0; i < opt_app.Sensoren.Drukverschil; i++)
  {
    Last = opt_app.Sensoren.FirstAddress + opt_app.Sensoren.Drukverschil - 1;
    if ((Adres >= opt_app.Sensoren.FirstAddress) && (Adres <= Last))
	  return (0);
  }
  for (i = 0; i < MAX_LUCHTMENGKAST; i++)
  {
    if (opt_app.Luchtmengkast[i].Enabled)
    {
      if ((i != (VentNummer - 1)) && (Adres == opt_app.Luchtmengkast[i].Afblaasvent.Adres))
        return (0);
      switch (opt_app.Luchtmengkast[i].Inblaasvent.TypeSturing)
	  {
	    case TYPE_STURING_EBMBUS             :
	    case TYPE_STURING_EBM_MODBUS         :
	    case TYPE_STURING_EC_BLUE_MODBUS     :
	    case TYPE_STURING_EC_BLUE_PREMIUM    :
	    case TYPE_STURING_MB_ROSENBERG       :
	    case TYPE_STURING_MB_CLIMAFAN        : 
	    case TYPE_STURING_MB_ROSENBERG_GEN3  :
	    case TYPE_STURING_MB_NICOTRA_GEBHARDT:
	      if (Adres == opt_app.Luchtmengkast[i].Inblaasvent.Adres)
	        return (0);
	  }
    }
  }
  return (1);
}

static void Arrow_Adres_Afblaasvent_Value(void)
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
      while (AdresAfblaasventOk(screen_ptr->value) == 0);
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
      while (AdresAfblaasventOk(screen_ptr->value) == 0);
      if (old_val != screen_ptr->value)
      {
        option_change_flag = 1;
        Put_Value();
      }
      break;
    case LEFT:
      opt_app.Luchtmengkast[VentNummer - 1].Afblaasvent.Adres = VentAdres;
      if (VentNummer > 1)
      {
        VentNummer--;
        VentAdres = opt_app.Luchtmengkast[VentNummer - 1].Afblaasvent.Adres;
        Get_Value();
      }
      else
        Arrow_Left_Value();
      break;
    case RIGHT:
      opt_app.Luchtmengkast[VentNummer - 1].Afblaasvent.Adres = VentAdres;
      if (VentNummer < AantalUnits)
      {
        VentNummer++;
        VentAdres = opt_app.Luchtmengkast[VentNummer - 1].Afblaasvent.Adres;
        Get_Value();
      }
      else
        Increment_Func_Index();
      break;
  }
}

static void Enter_Adres_Afblaasvent_Value(void)
{
  if (screen_ptr->change_flag)
  {
    if ((screen_ptr->value >= screen_ptr->min_value) && (screen_ptr->value <= screen_ptr->max_value))
    {
      if ((VentAdres != screen_ptr->value) && AdresAfblaasventOk(screen_ptr->value))
        Enter_Value();
    }
  }
  opt_app.Luchtmengkast[VentNummer - 1].Afblaasvent.Adres = VentAdres;
  if (VentNummer < AantalUnits)
  {
    VentNummer++;
    VentAdres = opt_app.Luchtmengkast[VentNummer - 1].Afblaasvent.Adres;
    Get_Value();
  }
  else
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
  if (mbDeviceChangeAddress(opt_app.Luchtmengkast[0].Inblaasvent.TypeSturing - TYPE_STURING_EBMBUS, OudAdres, NieuwAdres, &opt_app.Luchtmengkast[0].Inblaasvent.AnaIn, 1, ChangeAddressCompleted) == 0)
  {
    AdresWijzigenStatus  = 0;
    AdresWijzigenOk      = 0;
    AdresWijzigenMislukt = 1;
  }    
  Increment_Func_Index();
}

//-----------------------------------------------------------------------------

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
      Prev_Screen();
      break;
    case RIGHT:
      break;
  }
}

