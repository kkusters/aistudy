
#ifndef _CT_DATA_H
#define _CT_DATA_H

#include <time.h>
#include "ch_timer.h"
#include "ct_string.h"

// onderstaande regels van commentaar voorzien bij definitieve versie
// Test tekstkaders
//#define TEST_STRING 1
// test posities H2MC
//#define TEST_H2MC

// versie nummer programma 
// VERSIE % 100 sub versie nummer (aanpassen bij correctie)
// VERSIE / 100 hoofd versie nummer (aanpassen bij toevoegen nieuw module)
#define VERSIE_PROG 442
#define VERSIE_PROG_SUB 0
// versie nummer voor PC
// dit versienummer moet worden opgehoogd als door het wijzigen van dit programma
// de tabellen in de PC moeten worden aangepast
#define VERSIE_ALG_PC 2
#define VERSIE_XML_PC 2
// versie_tekst ophogen als tekst veranderd
#define VERSIE_TEKST 0
// versie_tekst_inst ophogen als tekst_inst veranderd
#define VERSIE_TEKST_INST 0

#define BLACKBOX_ADR 0x03FA     // blackbox can nr.
#define PC_0_ADR 0x03FF
#define PC_1_ADR 0x03FE
#define PC_2_ADR 0x03FC

// geen can backbone communicatie value
#define CAN_BACKBONE_VALUE_GEEN 0
// can backbone communicatie value ok
#define CAN_BACKBONE_VALUE_OK 1
// can backbone communicatie value error
#define CAN_BACKBONE_VALUE_ERROR 2

#define ENGELS 0
#define NEDERLANDS 1
#define DUITS 2
#define SPAANS 3
#define USER 4

#define FALSE 0
#define TRUE 1

#define OFF 0
#define ON 1

#define DISABLE 0
#define ENABLE 1

#define AAN_UIT 0
#define PROPORTIONEEL 1
#define OPEN_DICHT 2

#define TYPE_STURING_DIGITAAL            0
#define TYPE_STURING_ANALOOG             1
#define TYPE_STURING_EBMBUS              2
#define TYPE_STURING_EC_BLUE_PREMIUM     3
#define TYPE_STURING_EBM_MODBUS          4
#define TYPE_STURING_EC_BLUE_MODBUS      5
#define TYPE_STURING_MB_ROSENBERG        6
#define TYPE_STURING_MB_CLIMAFAN         7
#define TYPE_STURING_MB_ROSENBERG_GEN3   8
#define TYPE_STURING_MB_NICOTRA_GEBHARDT 9

#define TYPE_STURING_COUNT 10

#define TYPE_STURING_CANOPEN  2
#define TYPE_STURING_BACNET   3

// Alarm bits
// wordt gezet bij optreden alarm
#define AL_ON      ((unsigned char)0x01)
// wordt gezet als alarm uit 
// (in controle alarm worden alle alarm bits uitgezet als AL_OFF en AL_OLD_ON 1 zijn)
#define AL_OFF     ((unsigned char)0x02)
// (alarm wordt in log tabel geplaatst als AL_ON 1 is en AL_OLD_ON 0)
// wordt bij controle alarm op 1 gezet als nieuw alarm in log tabel geplaatst wordt
#define AL_OLD_ON  ((unsigned char)0x04)
// wordt voor eiertelling gebruikt als eiertel alarm aktief dan AL_ACTIVE 1
// als AL_ACTIVE echter 0 is omdat er geen alarm meer is dan kan het alarm gewist worden
#define AL_ACTIVE  ((unsigned char)0x08)
// als AL_NOT_BLINK 0 dan wordt alarm op scherm weergegeven
// als AL_NOT_BLINK 1 dan wordt alarm aleen in alarmen actief en alarm history weergegeven
//#define AL_NOT_BLINK ((unsigned char)0x10)

#define AL_HARD        ((unsigned char)0x20)
#define AL_ZACHT       ((unsigned char)0x40)
#define AL_ONDERDRUKT  ((unsigned char)0x80)

#define MASK_AL_WISSEN 	   ((unsigned char)0x10)
#define MASK_AL_HARD	   ((unsigned char)0x20) 
#define MASK_AL_ZACHT	   ((unsigned char)0x40) 
#define MASK_AL_ONDERD	   ((unsigned char)0x80)
#define MASK_AL_HA_ZA	   (MASK_AL_HARD   | MASK_AL_ZACHT)
#define MASK_AL_ZA_ON	   (MASK_AL_ZACHT  | MASK_AL_ONDERD)
#define MASK_AL_HA_ON	   (MASK_AL_HARD   | MASK_AL_ONDERD)
#define MASK_HA_ZA_ON	   (MASK_AL_HARD   | MASK_AL_ONDERD | MASK_AL_ZACHT)
#define MASK_AL_HARD_WIS   (MASK_AL_WISSEN | MASK_AL_HARD )
#define MASK_AL_ZACHT_WIS  (MASK_AL_WISSEN | MASK_AL_ZACHT)
#define MASK_AL_ONDER_WIS  (MASK_AL_WISSEN | MASK_AL_ONDERD)
#define MASK_AL_HA_ZA_WIS  (MASK_AL_WISSEN | MASK_AL_HA_ZA)
#define MASK_AL_ZA_ON_WIS  (MASK_AL_WISSEN | MASK_AL_ZA_ON)
#define MASK_AL_HA_ON_WIS  (MASK_AL_WISSEN | MASK_AL_HA_ON)
#define MASK_HA_ZA_ON_WIS  (MASK_AL_WISSEN | MASK_HA_ZA_ON)
												
#define MASK_ALG_DISP  ((unsigned char)0x01)
#define MASK_ALG_PC    ((unsigned char)0x02)
#define MASK_AFD_DISP  ((unsigned char)0x04)
#define MASK_AFD_PC    ((unsigned char)0x08)
#define MASK_ALL       (MASK_ALG_DISP | MASK_ALG_PC | MASK_AFD_DISP | MASK_AFD_PC)
#define MASK_ALG       (MASK_ALG_DISP | MASK_ALG_PC)
#define MASK_AFD       (MASK_AFD_DISP | MASK_AFD_PC)
#define MASK_DISP      (MASK_ALG_DISP | MASK_AFD_DISP)
#define MASK_PC        (MASK_ALG_PC   | MASK_AFD_PC)

#define ALARM_VAL_0 0
#define ALARM_INDEX 1
#define ALARM_VALUE 2

#define BOARD_COMPONENT_ID 0
#define CLASS_ID 1
#define ANALOG_INPUT_ID 2
#define DIGITAL_INPUT_ID 3
#define ANALOG_OUTPUT_ID 4
#define DIGITAL_OUTPUT_ID 5       
#define COUNTER_INPUT_ID 6
#define EMEC_COUNTER_INPUT_ID 7
#define IRIS_OPTIONS 8
#define IRIS_SETPOINTS 9
#define IRIS_VALUES 10
#define ANALOG_HIGH_INPUT_ID 11
#define MOTOR_CONTROL_ID 12
#define RS485_BUS_ID 13

#define CLASS_VOERWEGER 1

#define BROADCOAST_ID 0

#define IO_06_14_ID 5
#define IO_06_14_MAX 4
#define IO_06_14_ANALOG_INPUT 2
#define IO_06_14_DIGITAL_INPUT 4
#define IO_06_14_ANALOG_OUTPUT 1 
#define IO_06_14_DIGITAL_OUTPUT 12

#define IO_12_06_ID 6
#define IO_12_06_MAX 4
#define IO_12_06_DIGITAL_INPUT 12
#define IO_12_06_COUNTER_INPUT 4
#define IO_12_06_ANALOG_OUTPUT 2
#define IO_12_06_DIGITAL_OUTPUT 2

#define IO_08_09_ID 13
#define IO_08_09_MAX 8
#define IO_08_09_DIGITAL_INPUT 8
#define IO_08_09_ANALOG_OUTPUT 4
#define IO_08_09_DIGITAL_OUTPUT 4

#define IO_EKU_ID  14
#define IO_EKU_MAX 16
#define IO_EKU_MOTOR_CONTROL 1

#define IO_H2MC_ID 15   // H2MC  1-32
//#define IO_H2MC_ID 16 // H2MC 33-64
#define IO_H2MC_MAX 32
#define IO_H2MC_MOTOR_CONTROL 2

#define IO_H1MC_ID 18
#define IO_H1MC_MAX 32
#define IO_H1MC_MOTOR_CONTROL 1

#define IO_05_07_ID 19
#define IO_05_07_MAX 16
#define IO_05_07_ANALOG_INPUT   5
#define IO_05_07_ANALOG_OUTPUT  2
#define IO_05_07_DIGITAL_OUTPUT 4
#define IO_05_07_RS485_BUS      1

#define IO_07_07_ID 20
#define IO_07_07_MAX 32
#define IO_07_07_ANALOG_INPUT   4
#define IO_07_07_ANALOG_OUTPUT  2
#define IO_07_07_DIGITAL_INPUT  3
#define IO_07_07_DIGITAL_OUTPUT 4
#define IO_07_07_RS485_BUS      1

#define ANA_INPUT_GEEN 0
#define ANA_INPUT_CELSIUS 1
#define ANA_INPUT_FAHRENHEID 2
#define ANA_INPUT_0_5V 3
#define ANA_INPUT_0_5V_NO_LIMIT 4

#define ANA_HIGH_INPUT_GEEN 0
#define ANA_HIGH_INPUT_WEGING 1
#define ANA_HIGH_INPUT_DIERWEGING 2
#define ANA_HIGH_INPUT_BINAIR 15

#define ANA_HIGH_INPUT_MASK     0x0E00
#define ANA_HIGH_INPUT_M5_5MV   0x0200
#define ANA_HIGH_INPUT_M10_10MV 0x0400
#define ANA_HIGH_INPUT_M20_20MV 0x0600
#define ANA_HIGH_INPUT_M40_40MV 0x0800
#define ANA_HIGH_INPUT_M80_80MV 0x0A00

#define RS485_BUFFER_SIZE 128

#define SECTIONS 1

#define ALARM_DISP_VALUES 256

#define CTRL_OFF 0
#define CTRL_ON 1
#define CTRL_NORMAL 2

#define MAX_GROUP      32 // Maximum nr motorgroups
#define MAX_MOTOR      64 // Maximum nr motors
#define MAX_SCREEN      8 // Maximum nr control dual screen
#define MAX_DEVICE    256 // Maximum nr devices (ventilatoren / kleppen)
#define MAX_KLEP      256 // Maximum nr kleppen
#define MAX_MB_DEVICE 256 // Maximum nr modbus devices (ebmBus / ebmModBus / ecBlueModbus / ecBluePremium / BelimoModbus)
#define MAX_CABRIO      8 // Maximum nr cabriokassen
#define MAX_LUCHTMENGKAST_GROEP 4 // Maximum aantal luchtmengkast groepen 
#define MAX_LUCHTMENGKAST      32 // Maximum aantal luchtmengkasten
#define MAX_VRIJGAVE 16
#define MAX_SENSOREN 16

// LET OP:
//  geheugen gebied van eeprom is 32kb dit is meer dan 16k
//  daarom moet je zorgen dat elk onderstaand gedefinieerd gebied
//  compleet ligt in 0xF04004-0xF07FFF
//  of compleet lig in 0xF08000-0xF0BE04
//  BLOK nummer 259 is het laatste blok in het gebied 0xF04004-0xF07FFF
//  BLOK nummer 260 is het eerste blok in het gebied 0xF08000-0xF0BE04
//#define EEPROM_START_ADRES (unsigned int)0x404004
#define EEPROM_START_ADRES (unsigned long)0x400000
// offsets in EEP_BLOCK's for starting off data in eeprom
// bij EEPROM 24256 blok grootte fysiek 64 byte in ram 63 omdat 1 byte voor checksum wordt gebruikt
// aantal blokken is 512
// bij EEPROM 24512 blok grootte fysiek 128 byte in ram 127 omdat 1 byte voor checksum wordt gebruikt
// aantal blokken is 512
#define OFFSET_MODULES        0
#define OFFSET_OPT_IO         2
#define OFFSET_EE_CHECK_256 255
#define OFFSET_OPT_ALG      256
#define OFFSET_OPT_APP      260
#define OFFSET_SETP_ALG     420
#define OFFSET_EE_CHECK_512 511
// Let Op geen blok maar bytes

//----------------------------------------------------------------------------- 
#define FN_OFF      0x00
#define FN_ON       0x01
#define FN_CURVE_ON 0x03

#define BUITENTEMP_ONTVANGEN_DELAY 60
#define WINDRICHTING_ONTVANGEN_DELAY 60
#define WINDSNELHEID_ONTVANGEN_DELAY 60

// LET OP: structures zijn altijd word aligned

#define MAX_LOG_VALUES 50
#define WRITE_TIME_OUT_MAX 60

// structure voor vastleggen welke modules, optie de klant gekocht heeft
typedef struct
{
  unsigned int computer;         // 0 
  unsigned int soort;            // 2
  unsigned int type;             // 4 
  unsigned int firma;            // 6 
  unsigned int versie_programma; // 8
  unsigned int versie_PC;        // 10
  unsigned long serie_number;    // 12
  unsigned int versie_tekst;     // 16
  unsigned int versie_tekst_inst;// 18
  unsigned int reserve2;         // 20
  unsigned int reserve3;         // 22
  unsigned int reserve4;         // 24
  unsigned int dummy0;           // 26
  unsigned int dummy1;           // 28
  unsigned int dummy2;           // 30
  unsigned int dummy3;           // 32       
  unsigned int dummy4;           // 34
  unsigned int dummy5;           // 36
  unsigned int dummy6;           // 38
  unsigned char CanOpen;         // 40 Vrij
  unsigned char Ventilatie;      // 41 Vrij
  unsigned char Luchtmengkast;   // 42 Vrij
  unsigned char BACnet;          // 43 Vrij
  unsigned char Schakelgroepen;  // 44 Vrij
  unsigned char Klep;            // 45 Vrij
  unsigned char Drukverschil;    // 46 Vrij
  unsigned char WatchdogMode;    // 47 Vrij
  unsigned char Hoogendoorn;     // 48 Vrij
  unsigned char module9;       // 49 Vrij
  unsigned char module10;      // 50 Vrij
  unsigned char module11;      // 51 Vrij
  unsigned char module12;      // 52 Vrij
  unsigned char module13;      // 53 Vrij
  unsigned char module14;      // 54 Vrij
  unsigned char module15;      // 55 Vrij
  unsigned char module16;      // 56 Vrij
  unsigned char module17;      // 57 Vrij
  unsigned char module18;      // 58 Vrij
  unsigned char module19;      // 59 Vrij
  unsigned char module20;      // 60 Vrij
  unsigned char module21;      // 61 Vrij
  unsigned char module22;      // 62 Vrij
  unsigned char module23;      // 63 Vrij
  unsigned char module24;      // 64 Vrij
  unsigned char module25;      // 65 Vrij
  unsigned char module26;      // 66 Vrij
  unsigned char module27;      // 67 Vrij
  unsigned char module28;      // 68 Vrij
  unsigned char module29;      // 69 Vrij
  unsigned char module30;      // 70 Vrij
  unsigned char module31;      // 71 Vrij
  unsigned char module32;      // 72 Vrij
  unsigned char module33;      // 73 Vrij
  unsigned char module34;      // 74 Vrij
  unsigned char module35;      // 75 Vrij
  unsigned char module36;      // 76 Vrij
  unsigned char module37;      // 77 Vrij
  unsigned char module38;      // 78 Vrij
  unsigned char module39;      // 79 Vrij
  unsigned char module40;      // 80 Vrij
  unsigned char module41;      // 81 Vrij
  unsigned char module42;      // 82 Vrij
  unsigned char module43;      // 83 Vrij
  unsigned char module44;      // 84 Vrij
  unsigned char module45;      // 85 Vrij
  unsigned char module46;      // 86 Vrij
  unsigned char module47;      // 87 Vrij
  unsigned char module48;      // 88 Vrij
  unsigned char module49;      // 89 Vrij
  unsigned char module50;      // 90 Vrij
  unsigned char module51;      // 91 Vrij
  unsigned char module52;      // 92 Vrij
  unsigned char module53;      // 93 Vrij
  unsigned char module54;      // 94 Vrij
  unsigned char module55;      // 95 Vrij
  unsigned char module56;      // 96 Vrij
  unsigned char module57;      // 97 Vrij
  unsigned char module58;      // 98 Vrij
  unsigned char module59;      // 99 Vrij
  unsigned char module60;      // 100 Vrij
  unsigned char module61;      // 101 Vrij
  unsigned char module62;      // 102 Vrij
  unsigned char module63;      // 103 Vrij
  unsigned char module64;      // 104 Vrij
  unsigned char module65;      // 105 Vrij
  unsigned char module66;      // 106 Vrij
  unsigned char module67;      // 107 Vrij
  unsigned char module68;      // 108 Vrij
  unsigned char module69;      // 109 Vrij
  unsigned char module70;      // 110 Vrij
  unsigned char module71;      // 111 Vrij
  unsigned char module72;      // 112 Vrij
  unsigned char module73;      // 113 Vrij
  unsigned char module74;      // 114 Vrij
  unsigned char module75;      // 115 Vrij
  unsigned char module76;      // 116 Vrij
  unsigned char module77;      // 117 Vrij
  unsigned char module78;      // 118 Vrij
  unsigned char module79;      // 119 Vrij
  unsigned char module80;      // 120 Vrij
  unsigned char module81;      // 121 Vrij
  unsigned char module82;      // 122 Vrij
  unsigned char module83;      // 123 Vrij
  unsigned char module84;      // 124 Vrij
  unsigned char module85;      // 125 Vrij
} s_modules;

// structures to send and receive data from CPU to I/O board
typedef struct
{
  int watchdog_timer;
  unsigned int command;
  unsigned int error_code; // meld een error van het bord bij geen fout waarde is 0
  unsigned int option;  // lowest digit is 1 then board is in use
  unsigned int version_hardware;
  unsigned int version_software;
  unsigned long time_stamp;
//  unsigned char test[1024];
} s_board_component;

typedef struct
{
  int value;
  unsigned int command;
  unsigned int option;
  unsigned char ana_in_nr;
  unsigned char sensor_nr;
  unsigned char losklep_nr;
  unsigned char silo_nr[9];
  int losklep_time;
  int interval_time;
  int max_vul_gewicht; // maximum vulgewicht (eenheden van 10 gram)
} s_class_voerweger_component;

typedef struct
{
  int value;
  unsigned int command;
  unsigned int option;
  int min_in;
  int max_in;
  int min_out;
  int max_out;
  unsigned int enable;
  int upper_limit;
  int lower_limit;
  int difference;
  int interval_time;
  unsigned int class_nr;
} s_analog_input;

typedef struct
{
  long value;
  unsigned int command;
  unsigned int option;
  long min_in;
  long max_in;
  long min_out;
  long max_out;
  unsigned int enable;
  int upper_limit_1;
  int lower_limit_1;
  int difference;
  int interval_time;
  int upper_limit_2;
  int lower_limit_2;
  int correctie;
  int reserve_0;
  int reserve_1;
  unsigned int class_nr;
} s_analog_high_input;

typedef struct
{
  int value;
  unsigned int command;
  unsigned int option;
  int counts_per_puls; // aantal counts per binnenkomende puls
  int pulses_per_count; // aantal pulsen nodig voor 1 count (prioriteit voen counts_per_puls)
  unsigned int enable;
  int upper_limit;
  int lower_limit;
  int difference;
  int interval_time;
  unsigned int class_nr;
} s_digital_input;

typedef struct
{
  int value[16];
  unsigned int command;
  unsigned int option;
  int pulses_per_count;
  unsigned int enable;
  int difference;
  int interval_time;
  unsigned int class_nr;
} s_counter_input;

typedef struct
{
  int value;
  unsigned int command;
  unsigned int option;
  int min_in;
  int max_in;
  int min_out;
  int max_out;
  unsigned int class_nr;
} s_analog_output;

typedef struct
{
  int value;
  unsigned int command;
  unsigned int option;
  unsigned int invers;
  unsigned int class_nr[16];
} s_digital_output;

typedef struct
{
  unsigned int Command;
  unsigned int Option;
  int  MinIn;
  int  MaxIn;
  int  MinOut;
  int  MaxOut;
  int  Runtime;
  int  MaxDev;
  unsigned int StDigIn;
  unsigned int StDigOut;
  unsigned int StFeedback;
  unsigned int StAnaIn_0_10V;
  unsigned int StAnaOut_0_5V;
  unsigned char SpeedLow;
  unsigned char SpeedHi;
  int  PosLowSpeedClose;
  int  PosLowSpeedOpen;
  long MotorManagementRuntime;
  long MotorManagementSwitches;
  int  MotorManagementFailures;
  int  NotUsed_1;
  int  NotUsed_2;
  int  Difference;
  int  IntervalTime;
  unsigned int ClassNr;
} s_motor_control;

typedef struct
{
  int  Command;
  int  Option;
  int  CommSettings; // BaudRate, parity, etc.
  char Difference;   // Pdo after x received bytes
  char EndOfFrame;   // Pdo after receiving this character
  int  IntervalTime; // Pdo after x ms since last pdo
  int  IdleTime;     // Pdo after x us since last received byte
  int  InhibitTime;
  int  ClassNr;
} s_RS485_bus;

typedef union
{
  s_board_component board_component;
  s_class_voerweger_component voerweger_component;
  s_analog_input analog_input;
  s_digital_input digital_input;
  s_counter_input counter_input;
  s_analog_output analog_output;
  s_digital_output digital_output;
  s_analog_high_input analog_high_input;
  s_motor_control motor_control;
  s_RS485_bus RS485_bus;
} u_SDO_data;

//****************************************************************************
// Structures for options
//**************************************************************************** 
typedef struct
{
  unsigned int option; // lowest digit is 1 then board is in use
  unsigned long time_stamp;
} s_option_board_component;

typedef struct
{
  unsigned int option;
  int min_in;
  int max_in;
  int min_out;
  int max_out;
  unsigned int enable;
  int upper_limit;
  int lower_limit;
  int difference;
  int interval_time;
  unsigned int class_nr;
  unsigned char opt_type; 
} s_option_analog_input;

typedef struct
{
  unsigned int option;
  long min_in;
  long max_in;
  long min_out;
  long max_out;
  long min_out_mV;
  long max_out_mV;
  unsigned int enable;
  int difference;
  int interval_time;
  unsigned int class_nr;
  unsigned char opt_type;
  long mV_per_V_loadcel; // mV/V per loadcel
} s_option_analog_high_input;

typedef struct
{
  unsigned int option;
  int counts_per_puls; // aantal counts per binnenkomende puls
  int pulses_per_count; // aantal pulsen nodig voor 1 count (prioriteit voen counts_per_puls)
                        // bij windsnelheid hier invullen aantal Hz bij 10m/s
  unsigned int enable;
  int upper_limit;
  int lower_limit;
  int difference;
  int interval_time;
  unsigned int class_nr;
  unsigned char opt_type; 
} s_option_digital_input;

typedef struct
{
  unsigned int option;
  int pulses_per_count;
  unsigned int enable;
  int difference;
  int interval_time;
  unsigned int class_nr;
  unsigned char opt_type; 
} s_option_counter_input;

typedef struct
{
  unsigned int option;
  int min_in;
  int max_in;
  int min_out;
  int max_out;
  unsigned int class_nr;
  unsigned char opt_type; 
} s_option_analog_output;

typedef struct
{
  unsigned int option;
  unsigned int invers;
  unsigned int class_nr[16];
  unsigned char opt_type[16]; 
} s_option_digital_output;

typedef struct
{
  unsigned int Option;
  int MinIn;
  int MaxIn;
  int MinOut;
  int MaxOut;
  int Runtime; // x100ms
  int MaxDev; // maximum deviation in case of synchronised
  unsigned char SpeedLow;
  unsigned char SpeedHi;
  int PosLowSpeedClose;
  int PosLowSpeedOpen;
  int NotUsed_1;
  int NotUsed_2;
  int Difference;
  int IntervalTime;
  unsigned int ClassNr;
  unsigned char OptType;
} s_option_motor_control;

typedef struct
{
  int  Option;
  int  BaudRate;
  char Parity;
  char Difference;   // Pdo after x received bytes
  char EndOfFrame;   // Pdo after receiving this character
  int  IntervalTime; // Pdo after x ms since last pdo
  int  IdleTime;     // Pdo after x us since last received byte
  int  InhibitTime;
  int  ClassNr;
} s_option_RS485_bus;

//-----------------------------------------------------------------------------
typedef struct
{
  s_option_board_component board_component;             
  s_option_analog_input analog_input[IO_06_14_ANALOG_INPUT];
  s_option_digital_input digital_input[IO_06_14_DIGITAL_INPUT];
  s_option_analog_output analog_output;
  s_option_digital_output digital_output;
  int offset_windrichting[IO_06_14_ANALOG_INPUT]; 
  unsigned char alarm;
} s_option_IO_06_14;

typedef struct
{
  s_option_board_component board_component;             
  s_option_digital_input digital_input[IO_12_06_DIGITAL_INPUT];
  s_option_counter_input counter_input[IO_12_06_COUNTER_INPUT];
  s_option_analog_output analog_output[IO_12_06_ANALOG_OUTPUT];
  s_option_digital_output digital_output;
  int aantal_counters; // aantal aanwezige counters (aan de hand heir van wordt de counter_input component aangemaakt
  unsigned char alarm;
} s_option_IO_12_06;

typedef struct
{
  s_option_board_component board_component;             
  s_option_digital_input digital_input[IO_08_09_DIGITAL_INPUT];
  s_option_analog_output analog_output[IO_08_09_ANALOG_OUTPUT];
  s_option_digital_output digital_output;
  unsigned char alarm;
} s_option_IO_08_09;

typedef struct
{
  s_option_board_component board_component;
  s_option_motor_control motor_control[IO_H2MC_MOTOR_CONTROL];
  unsigned char alarm;
} s_option_IO_H2MC;

typedef struct
{
  s_option_board_component board_component;
  s_option_motor_control motor_control;
  unsigned char alarm;
} s_option_IO_EKU;

typedef struct
{
  s_option_board_component board_component;
  s_option_motor_control motor_control;
  unsigned char alarm;
} s_option_IO_H1MC;

typedef struct
{
  s_option_board_component board_component;             
  s_option_analog_input    analog_input[IO_05_07_ANALOG_INPUT];
  s_option_analog_output   analog_output[IO_05_07_ANALOG_OUTPUT];
  s_option_digital_output  digital_output;
  s_option_RS485_bus       RS485_bus;
  unsigned char alarm;
} s_option_IO_05_07;

typedef struct
{
  s_option_board_component board_component;             
  s_option_analog_input    analog_input[IO_07_07_ANALOG_INPUT];
  s_option_digital_input   digital_input[IO_07_07_DIGITAL_INPUT];
  s_option_analog_output   analog_output[IO_07_07_ANALOG_OUTPUT];
  s_option_digital_output  digital_output;
  s_option_RS485_bus       RS485_bus;
  unsigned char alarm;
} s_option_IO_07_07;

typedef struct
{
  unsigned char board_type; // 2 = IO_20_33P_ID; 3 = IO_16_00_ID; 4 = IO_00_16_ID; 5 = IO_06_14_ID
  unsigned char board_nr;   // 0 = eerste board; 1 = 2de board; enz
//  unsigned char IO_type;  // 1 = CLASS_ID; 2 = ANALOG_INPUT_ID; 3 = DIGITAL_INPUT_ID; 4 = ANALOG_OUTPUT_ID; 5 = DIGITAL_OUTPUT_ID
  unsigned char IO_nr;     // 0 = eerste; 1 = tweede; enz
} s_board_IO;

typedef struct
{
  unsigned char board_type; // 2 = IO_20_33P_ID; 3 = IO_16_00_ID; 4 = IO_00_16_ID; 5 = IO_06_14_ID
  unsigned char board_nr;   // 0 = eerste board; 1 = 2de board; enz
  unsigned char IO_nr;      // 0 = eerste; 1 = tweede; enz
  unsigned char on_off;
} s_board_IO_on_off;
 
typedef enum
{
  br9600,
  br19200,
  br38400,
  br57600,
  br115200
} TBaudRate;

typedef enum
{
  ptNone,
  ptEven,
  ptOdd
} TParity;

//-----------------------------------------------------------------------------
typedef struct
{
  unsigned char Enabled;
  unsigned char Number;
  unsigned char GroupNumber;
  unsigned char ControlType;
  int Link; // In case of two screens on one rail, this is the opposite screen
  int Runtime; // x100ms
  s_board_IO_on_off IO;
} sOptMotor;

typedef struct
{
  unsigned char Enabled;
  unsigned char GroupNumber;
} sOptDevice;

typedef struct
{
  unsigned char Enabled;
  unsigned char Number;
  unsigned char Type;
  unsigned char NumberMotors;
  unsigned char BusType;
  unsigned char FirstNumber;
  unsigned char FirstAddress;
  unsigned char ControlType;
  unsigned char ControlIndex;
  unsigned char FrequencyControlled;
  unsigned char DispHiSpeedInput;
  unsigned char AnalogSpeed;
  unsigned char SpeedLow;
  unsigned char SpeedHi;
  int PositionLowSpeed;
  unsigned char PulseSystem;
  unsigned char KierRegeling;
  unsigned char AnalogInput;
  unsigned char DigitalInput;
  unsigned char AlarmAnaloog;
  unsigned char AnalogOutput;
  int FrequentieVerstel;
  unsigned char ClosedLoopOpenLoop; // 0 = Closed Loop; 1 = Open Loop
  unsigned char PowerFactor;        // Correctie factor voor vermogen ventilatoren [1.00]
  unsigned char RampUp;
  unsigned char RampDown;
  unsigned char SensorType;
  unsigned char VentAtMax;  // uitsturing naar ventilatoren bij 100%
  unsigned char WatchdogMode; // watchdog mode enabled bij RS485 sturing
  unsigned char PositionAtCommunicationFailure; // positie bij een watchdog fault bij RS485 sturing
  int Reserved[2];
  int Runtime; // x100ms
  s_board_IO_on_off DigInHiSpeed;
  s_board_IO_on_off DigInOpen;
  s_board_IO_on_off DigInClose;
  s_board_IO_on_off AnaOutPosition;
  s_board_IO_on_off DigOutAlarm;
  s_board_IO_on_off AnaInPosition;
  s_board_IO_on_off DigOutAlarmFlap;
} sOptMotorgroup;

typedef struct
{
  s_board_IO_on_off DigInOnOff;
  s_board_IO_on_off RS485Bus[4];
  s_board_IO_on_off DigOutAlarmUrgent;
} sOptVentgroup;

typedef struct
{
  unsigned char Enabled;
  unsigned char Number;
  unsigned char Standby;
  unsigned char Master;     // 0 = Geen, 1 = A, 2 = B
  unsigned char CombiMatic; // doeken achter elkaar ipv tegenover elkaar
  unsigned char Absolute;   // 0 = relatief, 1 = absoluut (alleen bij CombiMatic)
  int GroupA;
  int GroupB;
} sOptDualScreen;

typedef struct
{
  unsigned char Enabled;
  unsigned char Type;
  unsigned char Voorloop;
  unsigned char Hysterese;
  int GroupA;
  int GroupB;
} sOptCabriokas;

typedef struct
{
  unsigned int  Runtime;     // Time from 0-100% [x100ms]
  unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
  unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
  s_board_IO_on_off Open;
  s_board_IO_on_off Close;
  s_board_IO_on_off AnaIn;
  s_board_IO_on_off AnaOut;
} TOptServo;

typedef struct
{
  unsigned char Enabled;
  unsigned char TypeKlep;
  unsigned char Naregelen;
  unsigned char BovenklepEnabled;
  unsigned char VerwarmingEnabled;
  unsigned char AfblaasventEnabled;
  unsigned char Dummy_AfblaasventRegeling;
  unsigned char Dummy_AfblaasventOpOnderdruk;
  unsigned char AfblaasventGekoppeldAanKlep;
  unsigned char Dummy_BovenklepRegeling;
  unsigned char Dummy_BovenklepOpOnderdruk;
  unsigned char BovenklepGekoppeldAanKlep;

  s_board_IO_on_off Streeftemp;
  s_board_IO_on_off Dummy_OnderdrukSensor;

  TOptServo Inblaasvent;
  TOptServo Afblaasvent;
  TOptServo Binnenklep; // Binnenklep of Bovenklep (bovenklep alleen mogelijk bij recirculatieklep)
  TOptServo Buitenklep; // Buitenklep of Recirculatieklep
  TOptServo Verwarming;

  s_board_IO_on_off DigOutAlarm;
  s_board_IO_on_off Dummy_DigInVrijgave;
} TOptLuchtmengkastGroep; // LBK: Lucht Behandelings Kast

typedef struct
{
  unsigned char Enabled;
  unsigned char TypeKlep;
  unsigned char Groep;
  unsigned char Naregelen;
  unsigned char BovenklepEnabled;
  unsigned char VerwarmingEnabled;
  unsigned char AfblaasventEnabled;

  s_board_IO_on_off AnaInInblaastemp;
  s_board_IO_on_off AnaInMengtemp;
  s_board_IO_on_off DigInVorst;
  s_board_IO_on_off DigInAlarm;
  s_board_IO_on_off DigInDrukverschil;

  TOptServo Inblaasvent;
  TOptServo Afblaasvent;
  TOptServo Binnenklep; // Binnenklep of Bovenklep (bovenklep alleen mogelijk bij recirculatieklep)
  TOptServo Buitenklep; // Buitenklep of Recirculatieklep
  TOptServo Verwarming;
} TOptLuchtmengkast;

typedef struct
{
  s_board_IO_on_off DigOutZacht;
} TOptAlarm;

typedef struct
{
  unsigned char Aantal;
  unsigned int  Ventilator[MAX_DEVICE];
  s_board_IO_on_off DigInVrijgave[MAX_VRIJGAVE];
  s_board_IO_on_off RS485Bus[MAX_VRIJGAVE];
} TOptVrijgaveVent;

typedef struct
{
  unsigned char Drukverschil; // Aantal
  unsigned char Type;         // Type (Modbus/Analoog)
  unsigned char FirstAddress;
  s_board_IO_on_off RS485Bus[MAX_SENSOREN]; // IO
} TOptSensoren;

//=============================================================================
typedef struct
{
  unsigned int area_size;        // Option size, verschil begin option, end option
  unsigned int computer;         // 2; computer bv 0 = blackbox; 1 = sirius; 2 = orion
  unsigned int soort;            // 4; soort bv 0 = basis; 1 = pluimvee; 2 = varkens; 3 = mooij
  unsigned int type;             // 6; type bv 0 = basis(CL); 1 = PB; 2 = PS
  unsigned int firma;            // 8; firma 0 = hotraco
  unsigned int versie_programma; // versie nummer eprom
  unsigned int versie_PC;        // versie PC (nodig voor tabellen in PC)
  unsigned long serie_number;    // serie number
  unsigned int versie_tekst;     // 
  unsigned int versie_tekst_inst;// 
  unsigned int reserve2;         // reserve voor toekomst wat vooraan moet staan zoals bovenstaade items
  unsigned int reserve3;         // reserve voor toekomst wat vooraan moet staan zoals bovenstaade items
  unsigned int reserve4;         // reserve voor toekomst wat vooraan moet staan zoals bovenstaade items

  int change_cnt;                // variabele wordt met 1 opgehoogd als options veranderd zijn

  #ifdef PASSWORD
  unsigned int password_gebruiker_;
  unsigned int password_installateur_;
  #else // PASSWORD
  unsigned int password_gebruiker;
  unsigned int password_installateur;
  #endif // PASSWORD
  unsigned int password_pc;
  char lcd_angle;                // LCD angle (Helderheid)
  unsigned char lcd_dimmen;      // LCD dimmen als de Orion niet bediend wordt.
  unsigned char taalkeuze;       // 0 = default taal, 1 = 2e taal, 2 = 3e taal, 3 = Taal in RAM
  unsigned char taalkeuze_inst;  // 0 = default taal  1 = 2e taal  2 = Taal in RAM

  int rs232;                     // 0=geen; 96=9k6; 192=19k2; 384=38k4; 576=57k6
  unsigned char can_backbone;    // 0 = geen can_backbone; 1 = can_backbone (backbone communication used)
  #ifdef CAN_BACKBONE_PC_WARNING
  unsigned char _can_rs232;      // wordt niet meer gebruikt
  #else // CAN_BACKBONE_PC_WARNING
  unsigned char can_rs232;      // 0 = geen CAN-RS232; 1 = CAN-RS232 (blackbox)
  #endif // CAN_BACKBONE_PC_WARNING
  unsigned int adres;            // adres orion voor rainbow

  unsigned char Dummy;
  unsigned char CANopenPossible;

  int CanBaudrate; // Baudrate CAN Backbone in kBaud

  unsigned char com1_enabled;
  int           com1_bd;
  unsigned char com1_modem;
  unsigned char com1_modem_answer;
  unsigned char com2_enabled;
  int           com2_bd;
  unsigned char com2_modem;
  unsigned char com2_modem_answer;
  unsigned char ethernet_enabled;
  unsigned char ethernet[3][4]; // ethernet[0] = netmask
                                // ethernet[1] = hostmask
                                // ethernet[2] = routeradres
  unsigned int  ethernet_port;                              
  unsigned char ethernet_module; // gebruik van ethernet is mogelijk
  unsigned char sd_module;       // gebruik van sd kaart is mogelijk
  unsigned char BACnet_possible;
  unsigned char BACnet_enabled;

  //#ifdef CAN_BACKBONE_PC_WARNING
  unsigned char can_backbone_pc_warning[3]; // voor aanzetten waarschuwing indien een smartlink verbinding wegvalt
  //#endif // CAN_BACKBONE_PC_WARNING
  #ifdef PASSWORD
  unsigned char password_level[5];
  unsigned int password_nummer[5];
  #endif // PASSWORD

  unsigned long BACnet_Device_Id;
  unsigned int BACnet_Network_Nr;

  unsigned char hoogendoorn_possible;
  unsigned char hoogendoorn_enabled;

  unsigned int end; // Laatste adres opties.
} s_opt_alg;

//=============================================================================
typedef struct
{
  unsigned int area_size;        // Option size, verschil begin option, end option
  unsigned int computer;         // 2; computer bv 0 = blackbox; 1 = sirius; 2 = orion
  unsigned int soort;            // 4; soort bv 0 = basis; 1 = pluimvee; 2 = varkens; 3 = mooij
  unsigned int type;             // 6; type bv 0 = basis(CL); 1 = PB; 2 = PS
  unsigned int firma;            // 8; firma 0 = hotraco
  unsigned int versie_programma; // versie nummer eprom
  unsigned int versie_PC;        // versie PC (nodig voor tabellen in PC)
  unsigned long serie_number;    // serie number
  unsigned int versie_tekst;     // 
  unsigned int versie_tekst_inst;// 
  unsigned int reserve2;         // reserve voor toekomst wat vooraan moet staan zoals bovenstaade items
  unsigned int reserve3;         // reserve voor toekomst wat vooraan moet staan zoals bovenstaade items
  unsigned int reserve4;         // reserve voor toekomst wat vooraan moet staan zoals bovenstaade items

  int change_cnt;                // variabele wordt met 1 opgehoogd als options veranderd zijn

  unsigned char NumberMotorgroups;

  sOptMotorgroup Motorgroup[MAX_GROUP];
  sOptMotor      Motor[MAX_MOTOR];
  sOptDualScreen DualScreen[MAX_SCREEN];
  sOptCabriokas  Cabriokas[MAX_CABRIO];
  sOptDevice     Device[MAX_DEVICE];
  sOptVentgroup  Ventgroup[MAX_GROUP];

  TOptLuchtmengkastGroep LuchtmengkastGroep[MAX_LUCHTMENGKAST_GROEP];
  TOptLuchtmengkast      Luchtmengkast[MAX_LUCHTMENGKAST];

  TOptAlarm Alarm;

  unsigned char LuchtmengkastInblaasventClosedLoopOpenLoop;
  unsigned char LuchtmengkastAfblaasventClosedLoopOpenLoop;

  unsigned char LuchtmengkastInblaasventPowerFactor;
  unsigned char LuchtmengkastInblaasventRampUp;
  unsigned char LuchtmengkastInblaasventRampDown;
  unsigned char LuchtmengkastAfblaasventPowerFactor;
  unsigned char LuchtmengkastAfblaasventRampUp;
  unsigned char LuchtmengkastAfblaasventRampDown;

  TOptVrijgaveVent VrijgaveVent;
  TOptSensoren     Sensoren;

  unsigned int end; // Laatste adres opties.
} s_opt_app;

//=============================================================================
typedef struct
{
  unsigned int area_size;        // Option size, verschil begin option, end option
  unsigned int computer;         // 2; computer bv 0 = blackbox; 1 = sirius; 2 = orion
  unsigned int soort;            // 4; soort bv 0 = basis; 1 = pluimvee; 2 = varkens; 3 = mooij
  unsigned int type;             // 6; type bv 0 = basis(CL); 1 = PB; 2 = PS
  unsigned int firma;            // 8; firma 0 = hotraco
  unsigned int versie_programma; // versie nummer eprom
  unsigned int versie_PC;        // versie PC (nodig voor tabellen in PC)
  unsigned long serie_number;    // serie number
  unsigned int versie_tekst;     // 
  unsigned int versie_tekst_inst;// 
  unsigned int reserve2;         // reserve voor toekomst wat vooraan moet staan zoals bovenstaade items
  unsigned int reserve3;         // reserve voor toekomst wat vooraan moet staan zoals bovenstaade items
  unsigned int reserve4;         // reserve voor toekomst wat vooraan moet staan zoals bovenstaade items

  int change_cnt;                // variabele wordt met 1 opgehoogd als options veranderd zijn

  s_option_IO_05_07  IO_05_07[IO_05_07_MAX];
  s_option_IO_06_14  IO_06_14[IO_06_14_MAX];
  s_option_IO_07_07  IO_07_07[IO_07_07_MAX];
  s_option_IO_08_09  IO_08_09[IO_08_09_MAX];
  s_option_IO_12_06  IO_12_06[IO_12_06_MAX];
  s_option_IO_H1MC   IO_H1MC[IO_H1MC_MAX];
  s_option_IO_H2MC   IO_H2MC[IO_H2MC_MAX];
  s_option_IO_EKU    IO_EKU[IO_EKU_MAX];

  unsigned int end; // Laatste adres opties.
} s_opt_io;

//*****************************************************************************
// Structures for setpoints 
//*****************************************************************************
//-----------------------------------------------------------------------------
typedef struct
{
  unsigned char hour;
  unsigned char min;
} s_time;

typedef struct
{
  char Offset;
} sSetpDevice;

typedef struct
{
  unsigned char MinVent;
  unsigned char MaxVent;
  unsigned char AlarmUrgent;
  unsigned char Offset;
} sSetpVentgroup;

typedef struct
{
  unsigned char PulseZone;
  unsigned char CycleTime;
  unsigned char PulseWidth;
  unsigned char Reserved;
} sSetpPulseSystem;

typedef struct
{
  int DelayAlarmManual;
  sSetpPulseSystem PulseSystem;
  unsigned char DiffPositionAlarm;
  unsigned char TimePositionAlarm;
  int Reserved[7];
} sSetpMotorgroup;

typedef struct
{
  int Opening;     // [%]
  int Hysteresis;  // [%]
  int StandbyTime; // [min]
} sSetpDualScreen;

typedef struct
{
  unsigned char DelayAlarmManual;
  unsigned char OnderdrukAlarm;
  unsigned char DiffPositionAlarm;
  unsigned char TimePositionAlarm;

  unsigned char InblaasventMinimum;
  unsigned char InblaasventMaximum;
  unsigned char AfblaasventMinimum;
  unsigned char AfblaasventMaximum;

  unsigned char VerwarmingMinimum;
  unsigned char VerwarmingStap;
  unsigned char VerwarmingCyclustijd;
  unsigned char VerwarmingHysterese;
  unsigned char VerwarmingBandbreedte;

  unsigned char KlepstandAan;
  unsigned char KlepstandUit;
} TSetpLuchtmengkastGroep;

//-----------------------------------------------------------------------------
typedef struct
{
  unsigned int area_size;        // 0; Setpoint size, verschil begin setpoint, end setpoint
  unsigned int computer;         // 2; computer bv 0 = blackbox; 1 = sirius; 2 = orion
  unsigned int soort;            // 4; soort bv 0 = basis; 1 = pluimvee; 2 = varkens; 3 = mooij
  unsigned int type;             // 6; type bv 0 = basis(CL); 1 = PB; 2 = PS
  unsigned int firma;            // 8; firma 0 = hotraco
  unsigned int versie_programma; // versie nummer eprom
  unsigned int versie_PC;        // versie PC (nodig voor tabellen in PC)
  unsigned long serie_number;    // serie number
  unsigned int versie_tekst;     // 
  unsigned int versie_tekst_inst;// 
  unsigned int reserve2;         // reserve voor toekomst wat vooraan moet staan zoals bovenstaade items
  unsigned int reserve3;         // reserve voor toekomst wat vooraan moet staan zoals bovenstaade items
  unsigned int reserve4;         // reserve voor toekomst wat vooraan moet staan zoals bovenstaade items

  unsigned char regelaar_on;     // regelaar aan uit

  int dagenteller;               // dagenteller
  unsigned char tijd_sync;       // wel of niet synchroniseren tijd als computer niet CAN master

  unsigned char sd_card_status; // 0 = loggen ingeschakeld, 1 = loggen stoppen, 2 = loggen uitgeschakeld (sd_card mag verwijderd worden), 3 = sd_card niet aanwezig
  unsigned char sd_card_log_on[MAX_FILES];

  sSetpMotorgroup         Motorgroup[MAX_GROUP];
  sSetpVentgroup          Ventgroup[MAX_GROUP];
  sSetpDevice             Device[MAX_DEVICE];
  sSetpDualScreen         DualScreen[MAX_SCREEN];
  TSetpLuchtmengkastGroep LuchtmengkastGroep[MAX_LUCHTMENGKAST_GROEP];

  unsigned int end;              // 829; eind adres setpoints
} s_setp_alg;

//*****************************************************************************
// Structures for values
//***************************************************************************** 
// unsigned int ctrl;
// 0x0001: Transmit PDO
// 0x0004: Transmit SDO
// 0x0008: Receive SDO
// 0x0010: Communication ok
// 0x0020: Board Initialised

typedef struct
{
  unsigned int ctrl;
  unsigned int watchdog_timer;
  unsigned int watchdog_update_timer;
  unsigned int version_hardware;
  unsigned int version_software;
} s_value_board_component;

typedef struct
{
  unsigned int ctrl;
} s_value_class_voerweger_component;

typedef struct
{
  unsigned int ctrl;
} s_value_analog_input;

typedef struct
{
  unsigned int ctrl;
  unsigned int interval_timer;
} s_value_analog_high_input;

typedef struct
{
  unsigned int ctrl;
} s_value_digital_input;

typedef struct
{
  unsigned int ctrl;
} s_value_counter_input;

typedef struct
{
  unsigned int ctrl;
  unsigned int interval_timer;
} s_value_analog_output;

typedef struct
{
  unsigned int ctrl;
  unsigned int interval_timer;
} s_value_digital_output;

typedef struct
{
  unsigned int ctrl;
  unsigned int interval_timer;
  unsigned int StDigIn;
  unsigned int StDigInMMI;
  unsigned int StDigOut;
  unsigned int StFeedback;
  unsigned int StAnaIn_0_10V;
  unsigned int StAnaOut_0_5V;
} s_value_motor_control;

typedef struct
{
  unsigned int ctrl;

  char Reset;
  char Transmit;
  char MessageReady;
  char TxBusy;
  char RxBusy;
  char TxToggle;
  char RxToggle;
 
  unsigned char TxPutIndex;
  unsigned char TxGetIndex;
  unsigned char TxInBuffer;

  unsigned char RxPutIndex;
  unsigned char RxGetIndex;
  unsigned char RxInBuffer;

  char TxBuffer[RS485_BUFFER_SIZE];
  char RxBuffer[RS485_BUFFER_SIZE];
} s_value_RS485_bus;

//-----------------------------------------------------------------------------
typedef struct
{
  s_value_board_component board_component;
  s_value_analog_input analog_input[IO_06_14_ANALOG_INPUT];
  s_value_digital_input digital_input[IO_06_14_DIGITAL_INPUT];
  s_value_analog_output analog_output;
  s_value_digital_output digital_output;
} s_value_IO_06_14;

typedef struct
{
  s_value_board_component board_component;
  s_value_digital_input digital_input[IO_12_06_DIGITAL_INPUT];
  s_value_counter_input counter_input[IO_12_06_COUNTER_INPUT];
  s_value_analog_output analog_output[IO_12_06_ANALOG_OUTPUT];
  s_value_digital_output digital_output;
} s_value_IO_12_06;

typedef struct
{
  s_value_board_component board_component;
  s_value_digital_input digital_input[IO_08_09_DIGITAL_INPUT];
  s_value_analog_output analog_output[IO_08_09_ANALOG_OUTPUT];
  s_value_digital_output digital_output;
} s_value_IO_08_09;

typedef struct
{
  s_value_board_component board_component;
  s_value_motor_control motor_control;
} s_value_IO_EKU;

typedef struct
{
  s_value_board_component board_component;
  s_value_motor_control motor_control[IO_H2MC_MOTOR_CONTROL];
} s_value_IO_H2MC;

typedef struct
{
  s_value_board_component board_component;
  s_value_motor_control motor_control;
} s_value_IO_H1MC;

typedef struct
{
  s_value_board_component board_component;
  s_value_analog_input    analog_input[IO_05_07_ANALOG_INPUT];
  s_value_analog_output   analog_output[IO_05_07_ANALOG_OUTPUT];
  s_value_digital_output  digital_output;
  s_value_RS485_bus       RS485_bus;
} s_value_IO_05_07;

typedef struct
{
  s_value_board_component board_component;
  s_value_analog_input    analog_input[IO_07_07_ANALOG_INPUT];
  s_value_digital_input   digital_input[IO_07_07_DIGITAL_INPUT];
  s_value_analog_output   analog_output[IO_07_07_ANALOG_OUTPUT];
  s_value_digital_output  digital_output;
  s_value_RS485_bus       RS485_bus;
} s_value_IO_07_07;

//----------------------------------------------------------------------------- 
typedef struct
{
  s_value_IO_06_14 IO_06_14[IO_06_14_MAX];
  s_value_IO_12_06 IO_12_06[IO_12_06_MAX];
  s_value_IO_08_09 IO_08_09[IO_08_09_MAX];
  s_value_IO_H2MC  IO_H2MC[IO_H2MC_MAX];
  s_value_IO_EKU   IO_EKU[IO_EKU_MAX];
  s_value_IO_H1MC  IO_H1MC[IO_H1MC_MAX];
  s_value_IO_05_07 IO_05_07[IO_05_07_MAX];
  s_value_IO_07_07 IO_07_07[IO_07_07_MAX];
} s_value;

//*****************************************************************************
// Structures for values HR
//*****************************************************************************
typedef struct
{
  unsigned int command;
  unsigned int error_code;
} s_value_hr_board_component;

typedef struct
{
  int value;
  unsigned int command;
  unsigned int opdracht; // PDO (CPU->IO) opdracht
  int gewicht_gewenst;   // PDO (CPU->IO) gewenst gewicht voerweger (absoluut)
  unsigned int status;   // PDO (IO->CPU) 
  int max_vul_gewicht;   // maximum vulgewicht (eenheden van 10 gram)
} s_value_hr_class_voerweger_component;

typedef struct
{
  int value;
  unsigned int command;
} s_value_hr_analog_input;

typedef struct
{
  long value;
  unsigned int command;
  int upper_limit_1;
  int lower_limit_1;
  int upper_limit_2;
  int lower_limit_2;
  long value_middel;
  unsigned char slow_cnt;
  unsigned char slow_index;
  unsigned char fast_cnt;
  unsigned char fast_index;
  long val_array_fast[10];
  long val_array_slow[10];
} s_value_hr_analog_high_input;

typedef struct
{
  int value;
  unsigned int command;
  unsigned long count; // totaal teller waarde
  int old_value;
} s_value_hr_digital_input;

typedef struct
{
  int value[16];
  unsigned int command;
  unsigned long count[16]; // totaal teller waarde
  int old_value[16];
  unsigned char error[16]; // error code afkomstig van counter
} s_value_hr_counter_input;

typedef struct
{
  int value;
  unsigned int command;
} s_value_hr_analog_output;

typedef struct
{
  int value;
  unsigned int command;
} s_value_hr_digital_output;

typedef struct
{
  int Value; // actual position
  int Position; // desired position
  unsigned int Ctrl;
  unsigned int Status;
  unsigned int Status2;
  unsigned int Command;
  long MotorManagementRuntime;
  long MotorManagementSwitches;
  int  MotorManagementFailures;
} s_value_hr_motor_control;

typedef struct
{
  unsigned int Command;
  char Data[7];
} s_value_hr_RS485_bus;

//-----------------------------------------------------------------------------
typedef struct
{
  s_value_hr_board_component board_component;
  s_value_hr_analog_input analog_input[IO_06_14_ANALOG_INPUT];
  s_value_hr_digital_input digital_input[IO_06_14_DIGITAL_INPUT];
  s_value_hr_analog_output analog_output;
  s_value_hr_digital_output digital_output;
} s_value_hr_IO_06_14; 

typedef struct
{
  s_value_hr_board_component board_component;
  s_value_hr_digital_input digital_input[IO_12_06_DIGITAL_INPUT];
  s_value_hr_counter_input counter_input[IO_12_06_COUNTER_INPUT];
  s_value_hr_analog_output analog_output[IO_12_06_ANALOG_OUTPUT];
  s_value_hr_digital_output digital_output;
} s_value_hr_IO_12_06; 

typedef struct
{
  s_value_hr_board_component board_component;
  s_value_hr_digital_input digital_input[IO_08_09_DIGITAL_INPUT];
  s_value_hr_analog_output analog_output[IO_08_09_ANALOG_OUTPUT];
  s_value_hr_digital_output digital_output;
} s_value_hr_IO_08_09; 

typedef struct
{
  s_value_hr_board_component board_component;
  s_value_hr_motor_control motor_control[IO_H2MC_MOTOR_CONTROL];
} s_value_hr_IO_H2MC; 

typedef struct
{
  s_value_hr_board_component board_component;
  s_value_hr_motor_control motor_control;
} s_value_hr_IO_EKU; 

typedef struct
{
  s_value_hr_board_component board_component;
  s_value_hr_motor_control motor_control;
} s_value_hr_IO_H1MC;

typedef struct
{
  s_value_hr_board_component board_component;
  s_value_hr_analog_input    analog_input[IO_05_07_ANALOG_INPUT];
  s_value_hr_analog_output   analog_output[IO_05_07_ANALOG_OUTPUT];
  s_value_hr_digital_output  digital_output;
  s_value_hr_RS485_bus       RS485_bus;
} s_value_hr_IO_05_07;

typedef struct
{
  s_value_hr_board_component board_component;
  s_value_hr_analog_input    analog_input[IO_07_07_ANALOG_INPUT];
  s_value_hr_digital_input   digital_input[IO_07_07_DIGITAL_INPUT];
  s_value_hr_analog_output   analog_output[IO_07_07_ANALOG_OUTPUT];
  s_value_hr_digital_output  digital_output;
  s_value_hr_RS485_bus       RS485_bus;
} s_value_hr_IO_07_07;

//-----------------------------------------------------------------------------
typedef enum
{
  omAuto,
  omManual,
  omOff
} TOperationMode;

typedef enum
{
  rmStop,
  rmOpen,
  rmClose
} TRunningMode;

typedef struct
{
  TOperationMode OperationMode;
  unsigned char Overruled;
  int Position;
  int PositionAuto;
  int PositionManual;
} sValHrMotor;

typedef struct
{
  TOperationMode OperationMode;
  unsigned char TargetValue;
  unsigned char ActualValue;
} sValHrDevice;

typedef struct
{
  TOperationMode OperationMode;
  int PositionTime;
  int PositionPerc;
} sValHrMotorgroup;

typedef struct
{
  int PositionGroupA; // [x.x %]
  int PositionGroupB; // [x.x %]
  int TimerStandby;   // [s]
  int Master;         // 0 = A is master, 1 = B is master
} sValHrDualScreen;

typedef struct
{
  unsigned int  PositionTime;
  unsigned char PositionPerc;
} TValHrServoIn;

typedef struct
{
  TOperationMode OperationMode;
  TValHrServoIn  Inblaasvent;
  TValHrServoIn  Afblaasvent;
  TValHrServoIn  Binnenklep;
  TValHrServoIn  Buitenklep;
  TValHrServoIn  Verwarming;
} TValHrLuchtmengkastGroep;

typedef struct
{
  unsigned char Actual;
  unsigned char Setpoint;
  unsigned char OldSetpoint;
  char Offset;
} TValHrServoOut;

typedef struct
{
  TOperationMode OperationMode;

  int Inblaastemp;
  int Mengtemp;
  unsigned char VorstBewaking;

  TValHrServoOut Inblaasvent;
  TValHrServoOut Afblaasvent;
  TValHrServoOut Binnenklep;
  TValHrServoOut Buitenklep;
  TValHrServoOut Verwarming;
} TValHrLuchtmengkast;

typedef struct
{
  int Drukverschil[MAX_SENSOREN];
} TValHrSensoren;

//=============================================================================
typedef struct
{
  unsigned char changed;                    // er is iets veranderd file indien aanwezig omnoemen en starten met loggen met nieuwe gegevens
  unsigned char aantal_variabelen;          // aantal variabelen dat gelogd moet worden
  unsigned char length;                     // lengte regel
  int aantal_regels;                        // telt aantal regels om file te rename als maximum aantal regels bereikt is
  unsigned int code[MAX_LOG_VALUES];  // variabelen regel;
  unsigned int interval_in_seconden;        // interval tussen metingen in seconden
  time_t start_tijd;
  time_t stop_tijd;
  s_timer timer;
} s_value_hr_sd_log;

//=============================================================================
typedef struct
{
  unsigned int control_0; // eerste controle getal
  unsigned int area_size; // Grote value_hr gebied

  s_value_hr_IO_05_07 IO_05_07[IO_05_07_MAX];
  s_value_hr_IO_06_14 IO_06_14[IO_06_14_MAX];
  s_value_hr_IO_07_07 IO_07_07[IO_07_07_MAX];
  s_value_hr_IO_08_09 IO_08_09[IO_08_09_MAX];
  s_value_hr_IO_12_06 IO_12_06[IO_12_06_MAX];
  s_value_hr_IO_H1MC  IO_H1MC[IO_H1MC_MAX];
  s_value_hr_IO_H2MC  IO_H2MC[IO_H2MC_MAX];
  s_value_hr_IO_EKU   IO_EKU[IO_EKU_MAX];

  unsigned char orion_switched_off; // orion is uitgeschakeld, genereer alarm tot een toets is gedrukt
  unsigned char can_node_alarm[128];  // 0 als node aanwezig 
                                      // 1 als heartbeat van node gemist wordt
  int can_node_adres[128];     // software nummer van de hoofdcomputer van de aangesloten Orion of sirius

  sValHrMotorgroup Motorgroup[MAX_GROUP];
  sValHrMotor      Motor[MAX_MOTOR];
  sValHrDualScreen DualScreen[MAX_SCREEN];

  sValHrDevice Device[MAX_DEVICE];

  TValHrLuchtmengkastGroep LuchtmengkastGroep[MAX_LUCHTMENGKAST_GROEP];
  TValHrLuchtmengkast      Luchtmengkast[MAX_LUCHTMENGKAST];

  TValHrSensoren Sensoren;

  unsigned int control_1; // eerste controle getal  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! KAN WEG als de eerste versie klaar is !!!!!!!!!!!!!!!!!!!!!!!!!!!!!

  s_value_hr_sd_log sd_log;

} s_val_hr_alg; // letop deze waarde moeten in een gebied geplaatst worden
              //   dat bij opstarten niet overschreven wordt.

typedef struct
{
  int code;
  int index;
  int value;
  int group;
  time_t on;
  time_t off;
  unsigned char *hard;
  int priority;
  unsigned char state;   // AL_HARD indien hoogst mogelijk alarm is alarm hard
                         // AL_ZACHT indien hoogst mogelijk alarm is alarm zacht
                         // AL_ONDERDRUKT indien hoogst mogelijk alarm is alarm onderdrukt
  unsigned char mask;    // keuze mogelijk heden MASK_AL_HARD | MASK_AL_ZACHT | MASK_AL_ONDERDRUKT                   
  unsigned char pc_type; // Geeft aan wat er in de value naar rainbow gezonden moet worden, al.index  al.value of combinatie             
} s_alarm_disp;

typedef struct
{
  int control;
  int index; // wijst naar laatste alarm
  int nr; // aantal alarmen geregistreed loopt op tot maximaal ALARM_VALUES
  int nr_actief; // aantal alarmen dat nog actief is (maximaal ALARM_VALUES)
  s_alarm_disp al[ALARM_DISP_VALUES];
} s_alarmen_alg;

typedef struct
{
  unsigned char AlarmHard;
  unsigned char Manual;
} sAlarmHrMotorgroup;

typedef struct
{
  unsigned char AlarmManualHard;
  unsigned char AlarmEncoderHard;
  unsigned char AlarmCode;
  unsigned char AlarmSlave;

  unsigned char Manual;
  unsigned char EmergencySwitch;
  unsigned char ThermalClose;
  unsigned char ThermalOpen;
  unsigned char BreakInput;
  unsigned char SpeedToLow;
  unsigned char EncoderFailure;
  unsigned char EncoderFailureA;
  unsigned char EncoderFailureB;
  unsigned char NoFeedback;
  unsigned char NotEnoughPulses;
  unsigned char PulsesToFast;
  unsigned char NotInstalled;
  unsigned char InstallMode;
  unsigned char DirectionsNotDefined;
  unsigned char EncoderInterference;
  unsigned char DualscreenNotPossible;
  unsigned char ErrorDeviationPosition;
  unsigned char WarningDeviationPosition;
  unsigned char LimitSwitchSafetySpeed;
  unsigned char ErrorCommunication;
  unsigned char LimitSwitchesNotEqual;
  unsigned char SpeedNotEqual;
  unsigned char MultipleMaster;
  unsigned char SlaveNotInit;
  unsigned char FrequencyController;
  unsigned char NotSynchronous;
  unsigned char LimitswitchNotReached;
  unsigned char WrongDirection;
  unsigned char LinkUnknown;
  unsigned char MotorNotRunning;
  unsigned char PositionNotReached;
  unsigned char Unknown;
} sAlarmHrMotor;

typedef struct
{
  unsigned char AlarmHard;
  unsigned char Manual;
  unsigned char TargetNotReached;
  unsigned char LimitSwitch;
  unsigned char ExternAlarm;
} sAlarmHrDevice;

typedef struct
{
  unsigned int  AlarmCode;
  unsigned int  WarningCode;

  unsigned char Unknown;
  unsigned char Communication;
  unsigned char LockedMotor;
  unsigned char HallFailure;
  unsigned char ThermalMotor;
  unsigned char CommErrorMasterSlavePIC;
  unsigned char ThermalPowerModule;
  unsigned char CommErrorRemoteUnit;
  unsigned char PhaseFailure;
  unsigned char Brake;
  unsigned char LowLineVoltage;
  unsigned char LowDcLinkVoltage;
  unsigned char HighDcLinkVoltage;
  unsigned char DriverProblem;
  unsigned char ElectronicBoxOverHeat;
  unsigned char ExcessiveDcLinkCurrent;
  unsigned char GeneralError;
  unsigned char MotorFault;
  unsigned char HeatSinkTemperature;
  unsigned char GroundFault;
  unsigned char LineIntHeatSinkSensor;
  unsigned char WrongDirection;
  unsigned char TemperatureLowering;
  unsigned char WrongConnection;
  unsigned char ExternalFault;
  unsigned char FactorySettings;
  unsigned char EepError;
  unsigned char RtcGeneralFault;
  unsigned char RtcVoltageFault;
  unsigned char FilterContamination;
  unsigned char TransferError;
  unsigned char DataConnectionLine;
  unsigned char DataConnectionChecksum;
  unsigned char SensorFaultInput1;
  unsigned char SensorFaultInput2;
  unsigned char SensorFaultInput3;
  unsigned char HighLineVoltage;
  unsigned char ILimit;
  unsigned char PLimit;
  unsigned char TEHigh;
  unsigned char TMHigh;
  unsigned char TEIHigh;
  unsigned char UzLow;
  unsigned char nLow;
  unsigned char IGBT;
  unsigned char UzkHi;
  unsigned char UzkLo;
  unsigned char UinHi;
  unsigned char UinLo;
  unsigned char _24VSupplyOverloaded;
  unsigned char InputPhaseError;
  unsigned char MemoryError;
  unsigned char ShortCircuit;
  unsigned char LossOfSynchronism;
  unsigned char InputVoltageError;
  unsigned char InputRelayNotClosed;
  unsigned char HighStartingCurrent;
} sAlarmHrmbDevice;

typedef struct
{
  unsigned char AlarmHard;
  unsigned char Manual;
} TAlarmHrLuchtmengkastGroep;

typedef struct
{
  unsigned char Manual;
  unsigned char Vorst;
  unsigned char Extern;
  unsigned char Drukverschil;

  unsigned char RecircklepTargetNotReached;
  unsigned char BuitenklepTargetNotReached;
  unsigned char BinnenklepTargetNotReached;
  unsigned char BovenklepTargetNotReached;
  unsigned char InblaasventTargetNotReached;
  unsigned char AfblaasventTargetNotReached;
  unsigned char VerwarmingTargetNotReached;
} TAlarmHrLuchtmengkast;

typedef struct
{
  unsigned char board_al;
  unsigned char ana_in_al[IO_05_07_ANALOG_INPUT];
  unsigned char adc_al;
  unsigned char externe_24v_al;
  unsigned char onbekend_al;
} s_alarm_hr_IO_05_07;

typedef struct
{
  unsigned char board_al;
  unsigned char onbekend_al;
} s_alarm_hr_IO_06_14;

typedef struct
{
  unsigned char board_al;
  unsigned char ana_in_al[IO_07_07_ANALOG_INPUT];
  unsigned char adc_al;
  unsigned char externe_24v_al;
  unsigned char onbekend_al;
} s_alarm_hr_IO_07_07;

typedef struct
{
  unsigned char board_al;
  unsigned char onbekend_al;
} s_alarm_hr_IO_08_09;

typedef struct
{
  unsigned char board_al;
  unsigned char onbekend_al;
} s_alarm_hr_IO_12_06;

typedef struct
{
  unsigned char board_al;
  unsigned char onbekend_al;
} s_alarm_hr_IO_H1MC;

typedef struct
{
  unsigned char board_al;
  unsigned char onbekend_al;
} s_alarm_hr_IO_H2MC;

typedef struct
{
  unsigned char board_al;
  unsigned char onbekend_al;
} s_alarm_hr_IO_EKU;

typedef struct
{
  s_alarmen_alg alarmen;
  unsigned char first_al;
  unsigned char computer_al;
  int computer_al_code;      // binnen gekomen alarm code van andere computer
  int computer_al_value;     // value van een alarm (bij master can waarde computer waarmee niet gecommuniceerd wordt)
  int computer_al_computer;  // computer : soort 10 = BLACKBOX; 11 = SIRIUS; 12 = ORION; enz
  int computer_al_soort;     // soort : 0 = BASIS; 1 = PLUIMVEE; 2 = VARKENS; enz
  int computer_al_nr;        // nummer van computer waar alarm is
                             // als nummer gelijk aan eigen dan alarm verzenden elke 15 seconden
                             // als nummer van andere computer dan als geen refresh binnen 30 seconden alarm opheffen
                             // 0 = geen alarm

  unsigned char orion_off_al;
  unsigned char opt_al;
  unsigned char setp_al;
  unsigned char val_hr_al;
  unsigned char I2C0_al;
  unsigned char EEP_al;
  unsigned char EEP_taal_al;
  unsigned char RTC_al;
  unsigned char timer_1ms_al;
  unsigned char htrap_al;
  unsigned char PLL_al;

  unsigned char orion_on_al; // niet nodig geen temperaturen

  unsigned char restore_option_setpoint_failed_al;
  unsigned char new_option_from_pc_al;
  unsigned char new_setpoint_from_pc_al;
  #ifdef CAN_BACKBONE_PC_WARNING
  unsigned char can_backbone_pc_warning[3]; // meerdere afdelingen mogelijk
  #else // CAN_BACKBONE_PC_WARNING
  unsigned char can_pc_al; // meerdere afdelingen mogelijk
  #endif // CAN_BACKBONE_PC_WARNING
  unsigned char can_al;

  unsigned char geen_alarm_contact_al;

  s_alarm_hr_IO_05_07 IO_05_07[IO_05_07_MAX];
  s_alarm_hr_IO_06_14 IO_06_14[IO_06_14_MAX];
  s_alarm_hr_IO_07_07 IO_07_07[IO_07_07_MAX];
  s_alarm_hr_IO_08_09 IO_08_09[IO_08_09_MAX];
  s_alarm_hr_IO_12_06 IO_12_06[IO_12_06_MAX];
  s_alarm_hr_IO_H1MC  IO_H1MC[IO_H1MC_MAX];
  s_alarm_hr_IO_H2MC  IO_H2MC[IO_H2MC_MAX];
  s_alarm_hr_IO_EKU   IO_EKU[IO_EKU_MAX];

  sAlarmHrMotorgroup Motorgroup[MAX_GROUP];
  sAlarmHrMotor      Motor[MAX_MOTOR];
  sAlarmHrDevice     Device[MAX_DEVICE];
  sAlarmHrmbDevice   mbDevice[MAX_MB_DEVICE];
  
  TAlarmHrLuchtmengkastGroep LuchtmengkastGroep[MAX_LUCHTMENGKAST_GROEP];
  TAlarmHrLuchtmengkast      Luchtmengkast[MAX_LUCHTMENGKAST];
  unsigned char eeprom_256k_al;
  unsigned char IO_05_07_versie_al[IO_05_07_MAX];
  unsigned char IO_07_07_versie_al[IO_07_07_MAX];
  unsigned char last_al;
} s_alarm_hr_alg;

//*****************************************************************************
// structures for ROM
//*****************************************************************************
typedef struct
{
  unsigned int watchdog_time;
  unsigned int watchdog_update_time;
} s_rom_board_component;

typedef struct
{
  unsigned int interval_time;
} s_rom_analog_high_input;

typedef struct
{
  unsigned int interval_time;
} s_rom_analog_output;

typedef struct
{
  unsigned int interval_time;
} s_rom_digital_output;

typedef struct
{
  unsigned int interval_time;
} s_rom_motor_control;

//-----------------------------------------------------------------------------
typedef struct
{
  s_rom_board_component board_component;
  s_rom_analog_output analog_output;
  s_rom_digital_output digital_output;
} s_rom_IO_06_14;

typedef struct
{
  s_rom_board_component board_component;
  s_rom_analog_output analog_output[IO_12_06_ANALOG_OUTPUT];
  s_rom_digital_output digital_output;
} s_rom_IO_12_06;

typedef struct
{
  s_rom_board_component board_component;
  s_rom_analog_output analog_output[IO_08_09_ANALOG_OUTPUT];
  s_rom_digital_output digital_output;
} s_rom_IO_08_09;

typedef struct
{
  s_rom_board_component board_component;
  s_rom_motor_control motor_control[IO_H2MC_MOTOR_CONTROL];
} s_rom_IO_H2MC;

typedef struct
{
  s_rom_board_component board_component;
  s_rom_motor_control motor_control;
} s_rom_IO_EKU;

typedef struct
{
  s_rom_board_component board_component;
  s_rom_motor_control motor_control;
} s_rom_IO_H1MC;

typedef struct
{
  s_rom_board_component board_component;
  s_rom_analog_output   analog_output[IO_05_07_ANALOG_OUTPUT];
  s_rom_digital_output  digital_output;
} s_rom_IO_05_07;

typedef struct
{
  s_rom_board_component board_component;
  s_rom_analog_output   analog_output[IO_07_07_ANALOG_OUTPUT];
  s_rom_digital_output  digital_output;
} s_rom_IO_07_07;

//----------------------------------------------------------------------------
typedef struct
{
  unsigned int computer;         // identification computer
  unsigned int soort;            // soort 0 = BASIS; 1 = PLUIMVEE; 2 = VARKENS
  unsigned int type;             // type onderverdeling 0 = PLUIMVEE_CL; 1 = PLUIMVEE_PB;  
  unsigned int firma;            // firma 0 = hotraco
  unsigned int versie_programma; // versie nummer eprom
  unsigned int versie_PC;        // versie pc
  unsigned long serie_number;    // serie number
  unsigned int versie_tekst;     // 
  unsigned int versie_tekst_inst;// 
  unsigned int reserve2;         // reserve voor toekomst wat vooraan moet staan zoals bovenstaade items
  unsigned int reserve3;         // reserve voor toekomst wat vooraan moet staan zoals bovenstaade items
  unsigned int reserve4;         // reserve voor toekomst wat vooraan moet staan zoals bovenstaade items

  s_rom_IO_06_14 IO_06_14[IO_06_14_MAX];
  s_rom_IO_12_06 IO_12_06[IO_12_06_MAX];
  s_rom_IO_08_09 IO_08_09[IO_08_09_MAX];
  s_rom_IO_H2MC  IO_H2MC[IO_H2MC_MAX];
  s_rom_IO_EKU   IO_EKU[IO_EKU_MAX];
  s_rom_IO_H1MC  IO_H1MC[IO_H1MC_MAX];
  s_rom_IO_05_07 IO_05_07[IO_05_07_MAX];
  s_rom_IO_07_07 IO_07_07[IO_07_07_MAX];
} s_rom;

#endif
