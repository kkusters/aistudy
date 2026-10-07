// C__DATA.C
// file for controling option and setpoint data

// LET OP: als je de module wilt vullen voor het instellen van
//         de door de gebruiker bestelde opties, dan verwijder commentaar voor volgende define
/******************************************************************************
/ Optie uitbreiden moet mogelijk zijn zonder dat de bestaande gewist worden.
/ Value_HR kontrole (op diverse plaatsen een controle integer die bekent is)
/
/
/*****************************************************************************/
#include "ch_define.h"

#include "ch_alarm.h"
#include "ch_alg.h"
#include "ch_asc0.h"
#include "ch_const.h"
#include "ch_disp_password.h"
#include "ch_eep.h"
#include "ch_main.h"
#include "ch_pc_com.h"
#include "ch_wdi.h"           
#include "ch_data.h"

// strak niet meer hiet nodig
#pragma class HB=EEPROM
#pragma noclear
s_modules  huge module   _at(EEPROM_START_ADRES);
s_opt_io   huge opt_io   _at(EEPROM_START_ADRES+(EEP_BLOCK_SIZE-1)*OFFSET_OPT_IO);   
s_opt_alg  huge opt_alg  _at(EEPROM_START_ADRES+(EEP_BLOCK_SIZE-1)*OFFSET_OPT_ALG);   
s_opt_app  huge opt_app  _at(EEPROM_START_ADRES+(EEP_BLOCK_SIZE-1)*OFFSET_OPT_APP);   
s_setp_alg huge setp_alg _at(EEPROM_START_ADRES+(EEP_BLOCK_SIZE-1)*OFFSET_SETP_ALG);
#pragma clear
#pragma default_attributes

#pragma class HB=TEKST
#pragma noclear
s_tekst huge tekst;
#pragma clear
#pragma default_attributes

#pragma class HB=TEKST_INST
#pragma noclear
s_tekst_inst huge tekst_inst;
#pragma clear
#pragma default_attributes

#pragma class HB=VAL_HR_ALG
#pragma noclear
unsigned char huge RTC_voltage_low; // variabele wordt 1 als spanning bij opstarten te laag is, en dus de inhoud van de ram niet correct is
unsigned char huge dont_restore_opt_setp;
s_val_hr_alg huge val_hr_alg;
#pragma clear
#pragma default_attributes

#pragma class HB=ALARM_HR_ALG
#pragma noclear
s_alarm_hr_alg huge alarm_hr_alg;
#pragma clear
#pragma default_attributes

//=============================================================================
#define WD_TIME_0 (PULSES_PER_SECOND*10)
#define WD_TIME_1 (PULSES_PER_SECOND*4)


unsigned char vlag_module_write = 0;
unsigned char vlag_options_added = 0;   // nodig voor change_cnt (opties gewijzigd)
unsigned char vlag_setpoints_added = 0; // nodig voor voorlopige versie

unsigned char const temp_unit = 0; // 0 = graden celsius 1 = graden farenheid

//=============================================================================
s_rom const rom =                                      
{
  ORION,               // unsigned int computer;         // identification computer
  MULTI_CONNECT,       // unsigned int soort;            // soort 0 = BASIS; 1 = PLUIMVEE; 2 = VARKENS
  MODULE_BASIS,        // unsigned int type;             // type onderverdeling 0 = PLUIMVEE_CL; 1 = PLUIMVEE_PB;  
  HOTRACO,             // unsigned int firma;            // firma 0 = hotraco
  VERSIE_PROG,         // unsigned int versie_programma; // versie nummer eprom
  VERSIE_ALG_PC,       // unsigned int versie_PC;        // versie pc
  0,                   // unsigned long serie_number;    // serie number
  VERSIE_TEKST,        // unsigned int versie_tekst;     // reserve voor toekomst wat vooraan moet staan zoals bovenstaade items
  VERSIE_TEKST_INST,   // unsigned int versie_tekst_inst;// reserve voor toekomst wat vooraan moet staan zoals bovenstaade items
  0,                   // unsigned int reserve2;         // reserve voor toekomst wat vooraan moet staan zoals bovenstaade items
  0,                   // unsigned int reserve3;         // reserve voor toekomst wat vooraan moet staan zoals bovenstaade items
  0,                   // unsigned int reserve4;         // reserve voor toekomst wat vooraan moet staan zoals bovenstaade items
  { // IO_06_14
    { // IO_06_14[0]
      // board component
      { WD_TIME_0, WD_TIME_1 }, // { 50, 25 },
      // analog output
      { REFRESH_TIME_125MS_BASE },
      // digital output
      { REFRESH_TIME_125MS_BASE }
    },
    { // IO_06_14[1]
      // board component
      { WD_TIME_0, WD_TIME_1 }, // { 50, 25 },
      // analog output
      { REFRESH_TIME_125MS_BASE },
      // digital output
      { REFRESH_TIME_125MS_BASE }
    },
    { // IO_06_14[2]
      // board component
      { WD_TIME_0, WD_TIME_1 }, // { 50, 25 },
      // analog output
      { REFRESH_TIME_125MS_BASE },
      // digital output
      { REFRESH_TIME_125MS_BASE }
    },
    { // IO_06_14[3]
      // board component
      { WD_TIME_0, WD_TIME_1 }, // { 50, 25 },
      // analog output
      { REFRESH_TIME_125MS_BASE },
      // digital output
      { REFRESH_TIME_125MS_BASE }
    },
  },
  { // IO_12_06
    { // IO_12_06[0]
      // board component
      { WD_TIME_0, WD_TIME_1 }, // { 50, 25 },
      { // analog_output
        // analog output[0]
        { REFRESH_TIME_125MS_BASE },
        // analog output[1]
        { REFRESH_TIME_125MS_BASE }
      },
      // digital output
      { REFRESH_TIME_125MS_BASE }
    },
    { // IO_12_06[1]
      // board component
      { WD_TIME_0, WD_TIME_1 }, // { 50, 25 },
      { // analog_output
        // analog output[0]
        { REFRESH_TIME_125MS_BASE },
        // analog output[1]
        { REFRESH_TIME_125MS_BASE }
      },
      // digital output
      { REFRESH_TIME_125MS_BASE }
    },
    { // IO_12_06[2]
      // board component
      { WD_TIME_0, WD_TIME_1 }, // { 50, 25 },
      { // analog_output
        // analog output[0]
        { REFRESH_TIME_125MS_BASE },
        // analog output[1]
        { REFRESH_TIME_125MS_BASE }
      },
      // digital output
      { REFRESH_TIME_125MS_BASE }
    },
    { // IO_12_06[3]
      // board component
      { WD_TIME_0, WD_TIME_1 }, // { 50, 25 },
      { // analog_output
        // analog output[0]
        { REFRESH_TIME_125MS_BASE },
        // analog output[1]
        { REFRESH_TIME_125MS_BASE }
      },
      // digital output
      { REFRESH_TIME_125MS_BASE }
    }
  },
  { // s_rom_IO_08_09 IO_08_09[IO_08_09_MAX];
    { // IO_08_09[0]
      // board component
      { WD_TIME_0, WD_TIME_1 },
      { // analog_output
        // analog output[0]
        { REFRESH_TIME_125MS_BASE },
        // analog output[1]
        { REFRESH_TIME_125MS_BASE },
        // analog output[2]
        { REFRESH_TIME_125MS_BASE },
        // analog output[3]
        { REFRESH_TIME_125MS_BASE },
      },
      // digital output
      { REFRESH_TIME_125MS_BASE },
    },
    { // IO_08_09[1]
      // board component
      { WD_TIME_0, WD_TIME_1 },
      { // analog_output
        // analog output[0]
        { REFRESH_TIME_125MS_BASE },
        // analog output[1]
        { REFRESH_TIME_125MS_BASE },
        // analog output[2]
        { REFRESH_TIME_125MS_BASE },
        // analog output[3]
        { REFRESH_TIME_125MS_BASE },
      },
      // digital output
      { REFRESH_TIME_125MS_BASE },
    },
    { // IO_08_09[2]
      // board component
      { WD_TIME_0, WD_TIME_1 },
      { // analog_output
        // analog output[0]
        { REFRESH_TIME_125MS_BASE },
        // analog output[1]
        { REFRESH_TIME_125MS_BASE },
        // analog output[2]
        { REFRESH_TIME_125MS_BASE },
        // analog output[3]
        { REFRESH_TIME_125MS_BASE },
      },
      // digital output
      { REFRESH_TIME_125MS_BASE },
    },
    { // IO_08_09[3]
      // board component
      { WD_TIME_0, WD_TIME_1 },
      { // analog_output
        // analog output[0]
        { REFRESH_TIME_125MS_BASE },
        // analog output[1]
        { REFRESH_TIME_125MS_BASE },
        // analog output[2]
        { REFRESH_TIME_125MS_BASE },
        // analog output[3]
        { REFRESH_TIME_125MS_BASE },
      },
      // digital output
      { REFRESH_TIME_125MS_BASE },
    },
    { // IO_08_09[4]
      // board component
      { WD_TIME_0, WD_TIME_1 },
      { // analog_output
        // analog output[0]
        { REFRESH_TIME_125MS_BASE },
        // analog output[1]
        { REFRESH_TIME_125MS_BASE },
        // analog output[2]
        { REFRESH_TIME_125MS_BASE },
        // analog output[3]
        { REFRESH_TIME_125MS_BASE },
      },
      // digital output
      { REFRESH_TIME_125MS_BASE },
    },
    { // IO_08_09[5]
      // board component
      { WD_TIME_0, WD_TIME_1 },
      { // analog_output
        // analog output[0]
        { REFRESH_TIME_125MS_BASE },
        // analog output[1]
        { REFRESH_TIME_125MS_BASE },
        // analog output[2]
        { REFRESH_TIME_125MS_BASE },
        // analog output[3]
        { REFRESH_TIME_125MS_BASE },
      },
      // digital output
      { REFRESH_TIME_125MS_BASE },
    },
    { // IO_08_09[6]
      // board component
      { WD_TIME_0, WD_TIME_1 },
      { // analog_output
        // analog output[0]
        { REFRESH_TIME_125MS_BASE },
        // analog output[1]
        { REFRESH_TIME_125MS_BASE },
        // analog output[2]
        { REFRESH_TIME_125MS_BASE },
        // analog output[3]
        { REFRESH_TIME_125MS_BASE },
      },
      // digital output
      { REFRESH_TIME_125MS_BASE },
    },
    { // IO_08_09[7]
      // board component
      { WD_TIME_0, WD_TIME_1 },
      { // analog_output
        // analog output[0]
        { REFRESH_TIME_125MS_BASE },
        // analog output[1]
        { REFRESH_TIME_125MS_BASE },
        // analog output[2]
        { REFRESH_TIME_125MS_BASE },
        // analog output[3]
        { REFRESH_TIME_125MS_BASE },
      },
      // digital output
      { REFRESH_TIME_125MS_BASE },
    },
  },
  { // s_rom_IO_H2MC IO_H2MC[IO_H2MC_MAX];
    { // IO_H2MC[0]
      // board component
      { WD_TIME_0, WD_TIME_1 },
      { // motor_control
        // motor_control[0]
        { REFRESH_TIME_125MS_BASE },
        // motor_control[1]
        { REFRESH_TIME_125MS_BASE },
      },
    },
    { // IO_H2MC[1]
      // board component
      { WD_TIME_0, WD_TIME_1 },
      { // motor_control
        // motor_control[0]
        { REFRESH_TIME_125MS_BASE },
        // motor_control[1]
        { REFRESH_TIME_125MS_BASE },
      },
    },
    { // IO_H2MC[2]
      // board component
      { WD_TIME_0, WD_TIME_1 },
      { // motor_control
        // motor_control[0]
        { REFRESH_TIME_125MS_BASE },
        // motor_control[1]
        { REFRESH_TIME_125MS_BASE },
      },
    },
    { // IO_H2MC[3]
      // board component
      { WD_TIME_0, WD_TIME_1 },
      { // motor_control
        // motor_control[0]
        { REFRESH_TIME_125MS_BASE },
        // motor_control[1]
        { REFRESH_TIME_125MS_BASE },
      },
    },
    { // IO_H2MC[4]
      // board component
      { WD_TIME_0, WD_TIME_1 },
      { // motor_control
        // motor_control[0]
        { REFRESH_TIME_125MS_BASE },
        // motor_control[1]
        { REFRESH_TIME_125MS_BASE },
      },
    },
    { // IO_H2MC[5]
      // board component
      { WD_TIME_0, WD_TIME_1 },
      { // motor_control
        // motor_control[0]
        { REFRESH_TIME_125MS_BASE },
        // motor_control[1]
        { REFRESH_TIME_125MS_BASE },
      },
    },
    { // IO_H2MC[6]
      // board component
      { WD_TIME_0, WD_TIME_1 },
      { // motor_control
        // motor_control[0]
        { REFRESH_TIME_125MS_BASE },
        // motor_control[1]
        { REFRESH_TIME_125MS_BASE },
      },
    },
    { // IO_H2MC[7]
      // board component
      { WD_TIME_0, WD_TIME_1 },
      { // motor_control
        // motor_control[0]
        { REFRESH_TIME_125MS_BASE },
        // motor_control[1]
        { REFRESH_TIME_125MS_BASE },
      },
    },
    { // IO_H2MC[8]
      // board component
      { WD_TIME_0, WD_TIME_1 },
      { // motor_control
        // motor_control[0]
        { REFRESH_TIME_125MS_BASE },
        // motor_control[1]
        { REFRESH_TIME_125MS_BASE },
      },
    },
    { // IO_H2MC[9]
      // board component
      { WD_TIME_0, WD_TIME_1 },
      { // motor_control
        // motor_control[0]
        { REFRESH_TIME_125MS_BASE },
        // motor_control[1]
        { REFRESH_TIME_125MS_BASE },
      },
    },
    { // IO_H2MC[10]
      // board component
      { WD_TIME_0, WD_TIME_1 },
      { // motor_control
        // motor_control[0]
        { REFRESH_TIME_125MS_BASE },
        // motor_control[1]
        { REFRESH_TIME_125MS_BASE },
      },
    },
    { // IO_H2MC[11]
      // board component
      { WD_TIME_0, WD_TIME_1 },
      { // motor_control
        // motor_control[0]
        { REFRESH_TIME_125MS_BASE },
        // motor_control[1]
        { REFRESH_TIME_125MS_BASE },
      },
    },
    { // IO_H2MC[12]
      // board component
      { WD_TIME_0, WD_TIME_1 },
      { // motor_control
        // motor_control[0]
        { REFRESH_TIME_125MS_BASE },
        // motor_control[1]
        { REFRESH_TIME_125MS_BASE },
      },
    },
    { // IO_H2MC[13]
      // board component
      { WD_TIME_0, WD_TIME_1 },
      { // motor_control
        // motor_control[0]
        { REFRESH_TIME_125MS_BASE },
        // motor_control[1]
        { REFRESH_TIME_125MS_BASE },
      },
    },
    { // IO_H2MC[14]
      // board component
      { WD_TIME_0, WD_TIME_1 },
      { // motor_control
        // motor_control[0]
        { REFRESH_TIME_125MS_BASE },
        // motor_control[1]
        { REFRESH_TIME_125MS_BASE },
      },
    },
    { // IO_H2MC[15]
      // board component
      { WD_TIME_0, WD_TIME_1 },
      { // motor_control
        // motor_control[0]
        { REFRESH_TIME_125MS_BASE },
        // motor_control[1]
        { REFRESH_TIME_125MS_BASE },
      },
    },
    { // IO_H2MC[16]
      // board component
      { WD_TIME_0, WD_TIME_1 },
      { // motor_control
        // motor_control[0]
        { REFRESH_TIME_125MS_BASE },
        // motor_control[1]
        { REFRESH_TIME_125MS_BASE },
      },
    },
    { // IO_H2MC[17]
      // board component
      { WD_TIME_0, WD_TIME_1 },
      { // motor_control
        // motor_control[0]
        { REFRESH_TIME_125MS_BASE },
        // motor_control[1]
        { REFRESH_TIME_125MS_BASE },
      },
    },
    { // IO_H2MC[18]
      // board component
      { WD_TIME_0, WD_TIME_1 },
      { // motor_control
        // motor_control[0]
        { REFRESH_TIME_125MS_BASE },
        // motor_control[1]
        { REFRESH_TIME_125MS_BASE },
      },
    },
    { // IO_H2MC[19]
      // board component
      { WD_TIME_0, WD_TIME_1 },
      { // motor_control
        // motor_control[0]
        { REFRESH_TIME_125MS_BASE },
        // motor_control[1]
        { REFRESH_TIME_125MS_BASE },
      },
    },
    { // IO_H2MC[20]
      // board component
      { WD_TIME_0, WD_TIME_1 },
      { // motor_control
        // motor_control[0]
        { REFRESH_TIME_125MS_BASE },
        // motor_control[1]
        { REFRESH_TIME_125MS_BASE },
      },
    },
    { // IO_H2MC[21]
      // board component
      { WD_TIME_0, WD_TIME_1 },
      { // motor_control
        // motor_control[0]
        { REFRESH_TIME_125MS_BASE },
        // motor_control[1]
        { REFRESH_TIME_125MS_BASE },
      },
    },
    { // IO_H2MC[22]
      // board component
      { WD_TIME_0, WD_TIME_1 },
      { // motor_control
        // motor_control[0]
        { REFRESH_TIME_125MS_BASE },
        // motor_control[1]
        { REFRESH_TIME_125MS_BASE },
      },
    },
    { // IO_H2MC[23]
      // board component
      { WD_TIME_0, WD_TIME_1 },
      { // motor_control
        // motor_control[0]
        { REFRESH_TIME_125MS_BASE },
        // motor_control[1]
        { REFRESH_TIME_125MS_BASE },
      },
    },
    { // IO_H2MC[24]
      // board component
      { WD_TIME_0, WD_TIME_1 },
      { // motor_control
        // motor_control[0]
        { REFRESH_TIME_125MS_BASE },
        // motor_control[1]
        { REFRESH_TIME_125MS_BASE },
      },
    },
    { // IO_H2MC[25]
      // board component
      { WD_TIME_0, WD_TIME_1 },
      { // motor_control
        // motor_control[0]
        { REFRESH_TIME_125MS_BASE },
        // motor_control[1]
        { REFRESH_TIME_125MS_BASE },
      },
    },
    { // IO_H2MC[26]
      // board component
      { WD_TIME_0, WD_TIME_1 },
      { // motor_control
        // motor_control[0]
        { REFRESH_TIME_125MS_BASE },
        // motor_control[1]
        { REFRESH_TIME_125MS_BASE },
      },
    },
    { // IO_H2MC[27]
      // board component
      { WD_TIME_0, WD_TIME_1 },
      { // motor_control
        // motor_control[0]
        { REFRESH_TIME_125MS_BASE },
        // motor_control[1]
        { REFRESH_TIME_125MS_BASE },
      },
    },
    { // IO_H2MC[28]
      // board component
      { WD_TIME_0, WD_TIME_1 },
      { // motor_control
        // motor_control[0]
        { REFRESH_TIME_125MS_BASE },
        // motor_control[1]
        { REFRESH_TIME_125MS_BASE },
      },
    },
    { // IO_H2MC[29]
      // board component
      { WD_TIME_0, WD_TIME_1 },
      { // motor_control
        // motor_control[0]
        { REFRESH_TIME_125MS_BASE },
        // motor_control[1]
        { REFRESH_TIME_125MS_BASE },
      },
    },
    { // IO_H2MC[30]
      // board component
      { WD_TIME_0, WD_TIME_1 },
      { // motor_control
        // motor_control[0]
        { REFRESH_TIME_125MS_BASE },
        // motor_control[1]
        { REFRESH_TIME_125MS_BASE },
      },
    },
    { // IO_H2MC[31]
      // board component
      { WD_TIME_0, WD_TIME_1 },
      { // motor_control
        // motor_control[0]
        { REFRESH_TIME_125MS_BASE },
        // motor_control[1]
        { REFRESH_TIME_125MS_BASE },
      },
    },
  },
  { // s_rom_IO_EKU IO_EKU[IO_EKU_MAX];
    { // IO_EKU[0]
      // board component
      { WD_TIME_0, WD_TIME_1 },
      { // motor_control
        { REFRESH_TIME_125MS_BASE },
      },
    },
    { // IO_EKU[1]
      // board component
      { WD_TIME_0, WD_TIME_1 },
      { // motor_control
        { REFRESH_TIME_125MS_BASE },
      },
    },
    { // IO_EKU[2]
      // board component
      { WD_TIME_0, WD_TIME_1 },
      { // motor_control
        { REFRESH_TIME_125MS_BASE },
      },
    },
    { // IO_EKU[3]
      // board component
      { WD_TIME_0, WD_TIME_1 },
      { // motor_control
        { REFRESH_TIME_125MS_BASE },
      },
    },
    { // IO_EKU[4]
      // board component
      { WD_TIME_0, WD_TIME_1 },
      { // motor_control
        { REFRESH_TIME_125MS_BASE },
      },
    },
    { // IO_EKU[5]
      // board component
      { WD_TIME_0, WD_TIME_1 },
      { // motor_control
        { REFRESH_TIME_125MS_BASE },
      },
    },
    { // IO_EKU[6]
      // board component
      { WD_TIME_0, WD_TIME_1 },
      { // motor_control
        { REFRESH_TIME_125MS_BASE },
      },
    },
    { // IO_EKU[7]
      // board component
      { WD_TIME_0, WD_TIME_1 },
      { // motor_control
        { REFRESH_TIME_125MS_BASE },
      },
    },
    { // IO_EKU[8]
      // board component
      { WD_TIME_0, WD_TIME_1 },
      { // motor_control
        { REFRESH_TIME_125MS_BASE },
      },
    },
    { // IO_EKU[9]
      // board component
      { WD_TIME_0, WD_TIME_1 },
      { // motor_control
        { REFRESH_TIME_125MS_BASE },
      },
    },
    { // IO_EKU[10]
      // board component
      { WD_TIME_0, WD_TIME_1 },
      { // motor_control
        { REFRESH_TIME_125MS_BASE },
      },
    },
    { // IO_EKU[11]
      // board component
      { WD_TIME_0, WD_TIME_1 },
      { // motor_control
        { REFRESH_TIME_125MS_BASE },
      },
    },
    { // IO_EKU[12]
      // board component
      { WD_TIME_0, WD_TIME_1 },
      { // motor_control
        { REFRESH_TIME_125MS_BASE },
      },
    },
    { // IO_EKU[13]
      // board component
      { WD_TIME_0, WD_TIME_1 },
      { // motor_control
        { REFRESH_TIME_125MS_BASE },
      },
    },
    { // IO_EKU[14]
      // board component
      { WD_TIME_0, WD_TIME_1 },
      { // motor_control
        { REFRESH_TIME_125MS_BASE },
      },
    },
    { // IO_EKU[15]
      // board component
      { WD_TIME_0, WD_TIME_1 },
      { // motor_control
        { REFRESH_TIME_125MS_BASE },
      },
    },
  },
  { // s_rom_IO_H1MC IO_H1MC[IO_H1MC_MAX];
    { // IO_H1MC[0]
      // board component
      { WD_TIME_0, WD_TIME_1 },
      { // motor_control
        { REFRESH_TIME_125MS_BASE },
      },
    },
    { // IO_H1MC[1]
      // board component
      { WD_TIME_0, WD_TIME_1 },
      { // motor_control
        { REFRESH_TIME_125MS_BASE },
      },
    },
    { // IO_H1MC[2]
      // board component
      { WD_TIME_0, WD_TIME_1 },
      { // motor_control
        { REFRESH_TIME_125MS_BASE },
      },
    },
    { // IO_H1MC[3]
      // board component
      { WD_TIME_0, WD_TIME_1 },
      { // motor_control
        { REFRESH_TIME_125MS_BASE },
      },
    },
    { // IO_H1MC[4]
      // board component
      { WD_TIME_0, WD_TIME_1 },
      { // motor_control
        { REFRESH_TIME_125MS_BASE },
      },
    },
    { // IO_H1MC[5]
      // board component
      { WD_TIME_0, WD_TIME_1 },
      { // motor_control
        { REFRESH_TIME_125MS_BASE },
      },
    },
    { // IO_H1MC[6]
      // board component
      { WD_TIME_0, WD_TIME_1 },
      { // motor_control
        { REFRESH_TIME_125MS_BASE },
      },
    },
    { // IO_H1MC[7]
      // board component
      { WD_TIME_0, WD_TIME_1 },
      { // motor_control
        { REFRESH_TIME_125MS_BASE },
      },
    },
    { // IO_H1MC[8]
      // board component
      { WD_TIME_0, WD_TIME_1 },
      { // motor_control
        { REFRESH_TIME_125MS_BASE },
      },
    },
    { // IO_H1MC[9]
      // board component
      { WD_TIME_0, WD_TIME_1 },
      { // motor_control
        { REFRESH_TIME_125MS_BASE },
      },
    },
    { // IO_H1MC[10]
      // board component
      { WD_TIME_0, WD_TIME_1 },
      { // motor_control
        { REFRESH_TIME_125MS_BASE },
      },
    },
    { // IO_H1MC[11]
      // board component
      { WD_TIME_0, WD_TIME_1 },
      { // motor_control
        { REFRESH_TIME_125MS_BASE },
      },
    },
    { // IO_H1MC[12]
      // board component
      { WD_TIME_0, WD_TIME_1 },
      { // motor_control
        { REFRESH_TIME_125MS_BASE },
      },
    },
    { // IO_H1MC[13]
      // board component
      { WD_TIME_0, WD_TIME_1 },
      { // motor_control
        { REFRESH_TIME_125MS_BASE },
      },
    },
    { // IO_H1MC[14]
      // board component
      { WD_TIME_0, WD_TIME_1 },
      { // motor_control
        { REFRESH_TIME_125MS_BASE },
      },
    },
    { // IO_H1MC[15]
      // board component
      { WD_TIME_0, WD_TIME_1 },
      { // motor_control
        { REFRESH_TIME_125MS_BASE },
      },
    },
    { // IO_H1MC[16]
      // board component
      { WD_TIME_0, WD_TIME_1 },
      { // motor_control
        { REFRESH_TIME_125MS_BASE },
      },
    },
    { // IO_H1MC[17]
      // board component
      { WD_TIME_0, WD_TIME_1 },
      { // motor_control
        { REFRESH_TIME_125MS_BASE },
      },
    },
    { // IO_H1MC[18]
      // board component
      { WD_TIME_0, WD_TIME_1 },
      { // motor_control
        { REFRESH_TIME_125MS_BASE },
      },
    },
    { // IO_H1MC[19]
      // board component
      { WD_TIME_0, WD_TIME_1 },
      { // motor_control
        { REFRESH_TIME_125MS_BASE },
      },
    },
    { // IO_H1MC[20]
      // board component
      { WD_TIME_0, WD_TIME_1 },
      { // motor_control
        { REFRESH_TIME_125MS_BASE },
      },
    },
    { // IO_H1MC[21]
      // board component
      { WD_TIME_0, WD_TIME_1 },
      { // motor_control
        { REFRESH_TIME_125MS_BASE },
      },
    },
    { // IO_H1MC[22]
      // board component
      { WD_TIME_0, WD_TIME_1 },
      { // motor_control
        { REFRESH_TIME_125MS_BASE },
      },
    },
    { // IO_H1MC[23]
      // board component
      { WD_TIME_0, WD_TIME_1 },
      { // motor_control
        { REFRESH_TIME_125MS_BASE },
      },
    },
    { // IO_H1MC[24]
      // board component
      { WD_TIME_0, WD_TIME_1 },
      { // motor_control
        { REFRESH_TIME_125MS_BASE },
      },
    },
    { // IO_H1MC[25]
      // board component
      { WD_TIME_0, WD_TIME_1 },
      { // motor_control
        { REFRESH_TIME_125MS_BASE },
      },
    },
    { // IO_H1MC[26]
      // board component
      { WD_TIME_0, WD_TIME_1 },
      { // motor_control
        { REFRESH_TIME_125MS_BASE },
      },
    },
    { // IO_H1MC[27]
      // board component
      { WD_TIME_0, WD_TIME_1 },
      { // motor_control
        { REFRESH_TIME_125MS_BASE },
      },
    },
    { // IO_H1MC[28]
      // board component
      { WD_TIME_0, WD_TIME_1 },
      { // motor_control
        { REFRESH_TIME_125MS_BASE },
      },
    },
    { // IO_H1MC[29]
      // board component
      { WD_TIME_0, WD_TIME_1 },
      { // motor_control
        { REFRESH_TIME_125MS_BASE },
      },
    },
    { // IO_H1MC[30]
      // board component
      { WD_TIME_0, WD_TIME_1 },
      { // motor_control
        { REFRESH_TIME_125MS_BASE },
      },
    },
    { // IO_H1MC[31]
      // board component
      { WD_TIME_0, WD_TIME_1 },
      { // motor_control
        { REFRESH_TIME_125MS_BASE },
      },
    },
  },

  { // IO_05_07
    { // IO_05_07[0]
      // board component
      { WD_TIME_0, WD_TIME_1 }, // { 50, 25 },
      // analog_output
      { { REFRESH_TIME_125MS_BASE },{ REFRESH_TIME_125MS_BASE } },
      // digital output
      { REFRESH_TIME_125MS_BASE }
    },
    { // IO_05_07[1]
      // board component
      { WD_TIME_0, WD_TIME_1 }, // { 50, 25 },
      // analog_output
      { { REFRESH_TIME_125MS_BASE },{ REFRESH_TIME_125MS_BASE } },
      // digital output
      { REFRESH_TIME_125MS_BASE }
    },
    { // IO_05_07[2]
      // board component
      { WD_TIME_0, WD_TIME_1 }, // { 50, 25 },
      // analog_output
      { { REFRESH_TIME_125MS_BASE },{ REFRESH_TIME_125MS_BASE } },
      // digital output
      { REFRESH_TIME_125MS_BASE }
    },
    { // IO_05_07[3]
      // board component
      { WD_TIME_0, WD_TIME_1 }, // { 50, 25 },
      // analog_output
      { { REFRESH_TIME_125MS_BASE },{ REFRESH_TIME_125MS_BASE } },
      // digital output
      { REFRESH_TIME_125MS_BASE }
    },
    { // IO_05_07[4]
      // board component
      { WD_TIME_0, WD_TIME_1 }, // { 50, 25 },
      // analog_output
      { { REFRESH_TIME_125MS_BASE },{ REFRESH_TIME_125MS_BASE } },
      // digital output
      { REFRESH_TIME_125MS_BASE }
    },
    { // IO_05_07[5]
      // board component
      { WD_TIME_0, WD_TIME_1 }, // { 50, 25 },
      // analog_output
      { { REFRESH_TIME_125MS_BASE },{ REFRESH_TIME_125MS_BASE } },
      // digital output
      { REFRESH_TIME_125MS_BASE }
    },
    { // IO_05_07[6
      // board component
      { WD_TIME_0, WD_TIME_1 }, // { 50, 25 },
      // analog_output
      { { REFRESH_TIME_125MS_BASE },{ REFRESH_TIME_125MS_BASE } },
      // digital output
      { REFRESH_TIME_125MS_BASE }
    },
    { // IO_05_07[7]
      // board component
      { WD_TIME_0, WD_TIME_1 }, // { 50, 25 },
      // analog_output
      { { REFRESH_TIME_125MS_BASE },{ REFRESH_TIME_125MS_BASE } },
      // digital output
      { REFRESH_TIME_125MS_BASE }
    },
    { // IO_05_07[8]
      // board component
      { WD_TIME_0, WD_TIME_1 }, // { 50, 25 },
      // analog_output
      { { REFRESH_TIME_125MS_BASE },{ REFRESH_TIME_125MS_BASE } },
      // digital output
      { REFRESH_TIME_125MS_BASE }
    },
    { // IO_05_07[9]
      // board component
      { WD_TIME_0, WD_TIME_1 }, // { 50, 25 },
      // analog_output
      { { REFRESH_TIME_125MS_BASE },{ REFRESH_TIME_125MS_BASE } },
      // digital output
      { REFRESH_TIME_125MS_BASE }
    },
    { // IO_05_07[10]
      // board component
      { WD_TIME_0, WD_TIME_1 }, // { 50, 25 },
      // analog_output
      { { REFRESH_TIME_125MS_BASE },{ REFRESH_TIME_125MS_BASE } },
      // digital output
      { REFRESH_TIME_125MS_BASE }
    },
    { // IO_05_07[11]
      // board component
      { WD_TIME_0, WD_TIME_1 }, // { 50, 25 },
      // analog_output
      { { REFRESH_TIME_125MS_BASE },{ REFRESH_TIME_125MS_BASE } },
      // digital output
      { REFRESH_TIME_125MS_BASE }
    },
    { // IO_05_07[12]
      // board component
      { WD_TIME_0, WD_TIME_1 }, // { 50, 25 },
      // analog_output
      { { REFRESH_TIME_125MS_BASE },{ REFRESH_TIME_125MS_BASE } },
      // digital output
      { REFRESH_TIME_125MS_BASE }
    },
    { // IO_05_07[13]
      // board component
      { WD_TIME_0, WD_TIME_1 }, // { 50, 25 },
      // analog_output
      { { REFRESH_TIME_125MS_BASE },{ REFRESH_TIME_125MS_BASE } },
      // digital output
      { REFRESH_TIME_125MS_BASE }
    },
    { // IO_05_07[14]
      // board component
      { WD_TIME_0, WD_TIME_1 }, // { 50, 25 },
      // analog_output
      { { REFRESH_TIME_125MS_BASE },{ REFRESH_TIME_125MS_BASE } },
      // digital output
      { REFRESH_TIME_125MS_BASE }
    },
    { // IO_05_07[15]
      // board component
      { WD_TIME_0, WD_TIME_1 }, // { 50, 25 },
      // analog_output
      { { REFRESH_TIME_125MS_BASE },{ REFRESH_TIME_125MS_BASE } },
      // digital output
      { REFRESH_TIME_125MS_BASE }
    },
  },

  { // IO_07_07
    { // IO_07_07[0]
      // board component
      { WD_TIME_0, WD_TIME_1 }, // { 50, 25 },
      // analog_output
      { { REFRESH_TIME_125MS_BASE },{ REFRESH_TIME_125MS_BASE } },
      // digital output
      { REFRESH_TIME_125MS_BASE }
    },
    { // IO_07_07[1]
      // board component
      { WD_TIME_0, WD_TIME_1 }, // { 50, 25 },
      // analog_output
      { { REFRESH_TIME_125MS_BASE },{ REFRESH_TIME_125MS_BASE } },
      // digital output
      { REFRESH_TIME_125MS_BASE }
    },
    { // IO_07_07[2]
      // board component
      { WD_TIME_0, WD_TIME_1 }, // { 50, 25 },
      // analog_output
      { { REFRESH_TIME_125MS_BASE },{ REFRESH_TIME_125MS_BASE } },
      // digital output
      { REFRESH_TIME_125MS_BASE }
    },
    { // IO_07_07[3]
      // board component
      { WD_TIME_0, WD_TIME_1 }, // { 50, 25 },
      // analog_output
      { { REFRESH_TIME_125MS_BASE },{ REFRESH_TIME_125MS_BASE } },
      // digital output
      { REFRESH_TIME_125MS_BASE }
    },
    { // IO_07_07[4]
      // board component
      { WD_TIME_0, WD_TIME_1 }, // { 50, 25 },
      // analog_output
      { { REFRESH_TIME_125MS_BASE },{ REFRESH_TIME_125MS_BASE } },
      // digital output
      { REFRESH_TIME_125MS_BASE }
    },
    { // IO_07_07[5]
      // board component
      { WD_TIME_0, WD_TIME_1 }, // { 50, 25 },
      // analog_output
      { { REFRESH_TIME_125MS_BASE },{ REFRESH_TIME_125MS_BASE } },
      // digital output
      { REFRESH_TIME_125MS_BASE }
    },
    { // IO_07_07[6]
      // board component
      { WD_TIME_0, WD_TIME_1 }, // { 50, 25 },
      // analog_output
      { { REFRESH_TIME_125MS_BASE },{ REFRESH_TIME_125MS_BASE } },
      // digital output
      { REFRESH_TIME_125MS_BASE }
    },
    { // IO_07_07[7]
      // board component
      { WD_TIME_0, WD_TIME_1 }, // { 50, 25 },
      // analog_output
      { { REFRESH_TIME_125MS_BASE },{ REFRESH_TIME_125MS_BASE } },
      // digital output
      { REFRESH_TIME_125MS_BASE }
    },
    { // IO_07_07[8]
      // board component
      { WD_TIME_0, WD_TIME_1 }, // { 50, 25 },
      // analog_output
      { { REFRESH_TIME_125MS_BASE },{ REFRESH_TIME_125MS_BASE } },
      // digital output
      { REFRESH_TIME_125MS_BASE }
    },
    { // IO_07_07[9]
      // board component
      { WD_TIME_0, WD_TIME_1 }, // { 50, 25 },
      // analog_output
      { { REFRESH_TIME_125MS_BASE },{ REFRESH_TIME_125MS_BASE } },
      // digital output
      { REFRESH_TIME_125MS_BASE }
    },
    { // IO_07_07[10]
      // board component
      { WD_TIME_0, WD_TIME_1 }, // { 50, 25 },
      // analog_output
      { { REFRESH_TIME_125MS_BASE },{ REFRESH_TIME_125MS_BASE } },
      // digital output
      { REFRESH_TIME_125MS_BASE }
    },
    { // IO_07_07[11]
      // board component
      { WD_TIME_0, WD_TIME_1 }, // { 50, 25 },
      // analog_output
      { { REFRESH_TIME_125MS_BASE },{ REFRESH_TIME_125MS_BASE } },
      // digital output
      { REFRESH_TIME_125MS_BASE }
    },
    { // IO_07_07[12]
      // board component
      { WD_TIME_0, WD_TIME_1 }, // { 50, 25 },
      // analog_output
      { { REFRESH_TIME_125MS_BASE },{ REFRESH_TIME_125MS_BASE } },
      // digital output
      { REFRESH_TIME_125MS_BASE }
    },
    { // IO_07_07[13]
      // board component
      { WD_TIME_0, WD_TIME_1 }, // { 50, 25 },
      // analog_output
      { { REFRESH_TIME_125MS_BASE },{ REFRESH_TIME_125MS_BASE } },
      // digital output
      { REFRESH_TIME_125MS_BASE }
    },
    { // IO_07_07[14]
      // board component
      { WD_TIME_0, WD_TIME_1 }, // { 50, 25 },
      // analog_output
      { { REFRESH_TIME_125MS_BASE },{ REFRESH_TIME_125MS_BASE } },
      // digital output
      { REFRESH_TIME_125MS_BASE }
    },
    { // IO_07_07[15]
      // board component
      { WD_TIME_0, WD_TIME_1 }, // { 50, 25 },
      // analog_output
      { { REFRESH_TIME_125MS_BASE },{ REFRESH_TIME_125MS_BASE } },
      // digital output
      { REFRESH_TIME_125MS_BASE }
    },
    { // IO_07_07[16]
      // board component
      { WD_TIME_0, WD_TIME_1 }, // { 50, 25 },
      // analog_output
      { { REFRESH_TIME_125MS_BASE },{ REFRESH_TIME_125MS_BASE } },
      // digital output
      { REFRESH_TIME_125MS_BASE }
    },
    { // IO_07_07[17]
      // board component
      { WD_TIME_0, WD_TIME_1 }, // { 50, 25 },
      // analog_output
      { { REFRESH_TIME_125MS_BASE },{ REFRESH_TIME_125MS_BASE } },
      // digital output
      { REFRESH_TIME_125MS_BASE }
    },
    { // IO_07_07[18]
      // board component
      { WD_TIME_0, WD_TIME_1 }, // { 50, 25 },
      // analog_output
      { { REFRESH_TIME_125MS_BASE },{ REFRESH_TIME_125MS_BASE } },
      // digital output
      { REFRESH_TIME_125MS_BASE }
    },
    { // IO_07_07[19]
      // board component
      { WD_TIME_0, WD_TIME_1 }, // { 50, 25 },
      // analog_output
      { { REFRESH_TIME_125MS_BASE },{ REFRESH_TIME_125MS_BASE } },
      // digital output
      { REFRESH_TIME_125MS_BASE }
    },
    { // IO_07_07[20]
      // board component
      { WD_TIME_0, WD_TIME_1 }, // { 50, 25 },
      // analog_output
      { { REFRESH_TIME_125MS_BASE },{ REFRESH_TIME_125MS_BASE } },
      // digital output
      { REFRESH_TIME_125MS_BASE }
    },
    { // IO_07_07[21]
      // board component
      { WD_TIME_0, WD_TIME_1 }, // { 50, 25 },
      // analog_output
      { { REFRESH_TIME_125MS_BASE },{ REFRESH_TIME_125MS_BASE } },
      // digital output
      { REFRESH_TIME_125MS_BASE }
    },
    { // IO_07_07[22]
      // board component
      { WD_TIME_0, WD_TIME_1 }, // { 50, 25 },
      // analog_output
      { { REFRESH_TIME_125MS_BASE },{ REFRESH_TIME_125MS_BASE } },
      // digital output
      { REFRESH_TIME_125MS_BASE }
    },
    { // IO_07_07[23]
      // board component
      { WD_TIME_0, WD_TIME_1 }, // { 50, 25 },
      // analog_output
      { { REFRESH_TIME_125MS_BASE },{ REFRESH_TIME_125MS_BASE } },
      // digital output
      { REFRESH_TIME_125MS_BASE }
    },
    { // IO_07_07[24]
      // board component
      { WD_TIME_0, WD_TIME_1 }, // { 50, 25 },
      // analog_output
      { { REFRESH_TIME_125MS_BASE },{ REFRESH_TIME_125MS_BASE } },
      // digital output
      { REFRESH_TIME_125MS_BASE }
    },
    { // IO_07_07[25]
      // board component
      { WD_TIME_0, WD_TIME_1 }, // { 50, 25 },
      // analog_output
      { { REFRESH_TIME_125MS_BASE },{ REFRESH_TIME_125MS_BASE } },
      // digital output
      { REFRESH_TIME_125MS_BASE }
    },
    { // IO_07_07[26]
      // board component
      { WD_TIME_0, WD_TIME_1 }, // { 50, 25 },
      // analog_output
      { { REFRESH_TIME_125MS_BASE },{ REFRESH_TIME_125MS_BASE } },
      // digital output
      { REFRESH_TIME_125MS_BASE }
    },
    { // IO_07_07[27]
      // board component
      { WD_TIME_0, WD_TIME_1 }, // { 50, 25 },
      // analog_output
      { { REFRESH_TIME_125MS_BASE },{ REFRESH_TIME_125MS_BASE } },
      // digital output
      { REFRESH_TIME_125MS_BASE }
    },
    { // IO_07_07[28]
      // board component
      { WD_TIME_0, WD_TIME_1 }, // { 50, 25 },
      // analog_output
      { { REFRESH_TIME_125MS_BASE },{ REFRESH_TIME_125MS_BASE } },
      // digital output
      { REFRESH_TIME_125MS_BASE }
    },
    { // IO_07_07[29]
      // board component
      { WD_TIME_0, WD_TIME_1 }, // { 50, 25 },
      // analog_output
      { { REFRESH_TIME_125MS_BASE },{ REFRESH_TIME_125MS_BASE } },
      // digital output
      { REFRESH_TIME_125MS_BASE }
    },
    { // IO_07_07[30]
      // board component
      { WD_TIME_0, WD_TIME_1 }, // { 50, 25 },
      // analog_output
      { { REFRESH_TIME_125MS_BASE },{ REFRESH_TIME_125MS_BASE } },
      // digital output
      { REFRESH_TIME_125MS_BASE }
    },
    { // IO_07_07[31]
      // board component
      { WD_TIME_0, WD_TIME_1 }, // { 50, 25 },
      // analog_output
      { { REFRESH_TIME_125MS_BASE },{ REFRESH_TIME_125MS_BASE } },
      // digital output
      { REFRESH_TIME_125MS_BASE }
    },
  },
};

s_value huge value = { 0 };

s_modules const default_modules =
{
  ORION,               // unsigned int computer;         // 0;
  MULTI_CONNECT,       // unsigned int soort;            // 2; soort 0 = BASIS; 1 = PLUIMVEE; 2 = VARKENS
  MODULE_BASIS,        // unsigned int type;             // 4; type onderverdeling 0 = PLUIMVEE_CL; 1 = PLUIMVEE_PB;  
  HOTRACO,             // unsigned int firma;            // 6; firma 0 = hotraco
  VERSIE_PROG,         // unsigned int versie_programma; // 8; versie nummer eprom
  VERSIE_ALG_PC,       // unsigned int versie_PC;        // 10; versie pc
  0,                   // unsigned long serie_number;    // 12; serie number
  VERSIE_TEKST,        // unsigned int versie_tekst;     // reserve voor toekomst wat vooraan moet staan zoals bovenstaade items
  VERSIE_TEKST_INST,   // unsigned int versie_tekst_inst;// reserve voor toekomst wat vooraan moet staan zoals bovenstaade items
  0,   // unsigned int reserve2;         // 20; reserve voor toekomst wat vooraan moet staan zoals bovenstaade items
  0,   // unsigned int reserve3;         // 22; reserve voor toekomst wat vooraan moet staan zoals bovenstaade items
  0,   // unsigned int reserve4;         // 24; reserve voor toekomst wat vooraan moet staan zoals bovenstaade items
  0,   // unsigned int dummy0;           // 26
  0,   // unsigned int dummy1;           // 28
  0,   // unsigned int dummy2;           // 30
  0,   // unsigned int dummy3;           // 32
  0,   // unsigned int dummy4;           // 34
  0,   // unsigned int dummy5;           // 36
  0,   // unsigned int dummy6;           // 38
  0,   // unsigned char CanOpen;         // 40
  0,   // unsigned char Ventilatie;      // 41
  0,   // unsigned char Luchtmengkast;   // 42
  0,   // unsigned char BACnet;          // 43
  0,   // unsigned char Schakelgroepen;  // 44
  0,   // unsigned char module5;      // 45
  0,   // unsigned char module6;      // 46
  0,   // unsigned char module7;      // 47
  0,   // unsigned char module8;      // 48
  0,   // unsigned char module9;      // 49
  0,   // unsigned char module10;     // 50
  0,   // unsigned char module11;     // 51
  0,   // unsigned char module12;     // 52
  0,   // unsigned char module13;     // 53
  0,   // unsigned char module14;     // 54
  0,   // unsigned char module15;     // 55
  0,   // unsigned char module16;     // 56
  0,   // unsigned char module17;     // 57
  0,   // unsigned char module18;     // 58
  0,   // unsigned char module19;     // 59
  0,   // unsigned char module20;     // 60
  0,   // unsigned char module21;     // 61
  0,   // unsigned char module22;     // 62
  0,   // unsigned char module23;     // 63
  0,   // unsigned char module24;     // 64
  0,   // unsigned char module25;     // 65
  0,   // unsigned char module26;     // 66
  0,   // unsigned char module27;     // 67
  0,   // unsigned char module28;     // 68
  0,   // unsigned char module29;     // 69
  0,   // unsigned char module30;     // 70
  0,   // unsigned char module31;     // 71
  0,   // unsigned char module32;     // 72
  0,   // unsigned char module33;     // 73
  0,   // unsigned char module34;     // 74
  0,   // unsigned char module35;     // 75
  0,   // unsigned char module36;     // 76
  0,   // unsigned char module37;     // 77
  0,   // unsigned char module38;     // 78
  0,   // unsigned char module39;     // 79
  0,   // unsigned char module40;     // 80
  0,   // unsigned char module41;     // 81
  0,   // unsigned char module42;     // 82
  0,   // unsigned char module43;     // 83
  0,   // unsigned char module44;     // 84
  0,   // unsigned char module45;     // 85
  0,   // unsigned char module46;     // 86
  0,   // unsigned char module47;     // 87
  0,   // unsigned char module48;     // 88
  0,   // unsigned char module49;     // 89
  0,   // unsigned char module50;     // 90
  0,   // unsigned char module51;     // 91
  0,   // unsigned char module52;     // 92
  0,   // unsigned char module53;     // 93
  0,   // unsigned char module54;     // 94
  0,   // unsigned char module55;     // 95
  0,   // unsigned char module56;     // 96
  0,   // unsigned char module57;     // 97
  0,   // unsigned char module58;     // 98
  0,   // unsigned char module59;     // 99
  0,   // unsigned char module60;     // 100
  0,   // unsigned char module61;     // 101
  0,   // unsigned char module62;     // 102
  0,   // unsigned char module63;     // 103
  0,   // unsigned char module64;     // 104
  0,   // unsigned char module65;     // 105
  0,   // unsigned char module66;     // 106
  0,   // unsigned char module67;     // 107
  0,   // unsigned char module68;     // 108
  0,   // unsigned char module69;     // 109
  0,   // unsigned char module70;     // 110
  0,   // unsigned char module71;     // 111
  0,   // unsigned char module72;     // 112
  0,   // unsigned char module73;     // 113
  0,   // unsigned char module74;     // 114
  0,   // unsigned char module75;     // 115
  0,   // unsigned char module76;     // 116
  0,   // unsigned char module77;     // 117
  0,   // unsigned char module78;     // 118
  0,   // unsigned char module79;     // 119
  0,   // unsigned char module80;     // 120
  0,   // unsigned char module81;     // 121
  0,   // unsigned char module82;     // 122
  0,   // unsigned char module83;     // 123
  0,   // unsigned char checksum_xor; // 124 van de 126 bytes
  0,   // unsigned char checksum_add; // 125 van de 126 bytes
};

//=============================================================================
s_opt_alg const default_opt_alg = 
{
  0,                   // unsigned int area_size;        // Option size, verschil begin option, end option
  ORION,               // unsigned int computer;         // identification computer
  MULTI_CONNECT,       // unsigned int soort;            // soort 0 = BASIS; 1 = PLUIMVEE; 2 = VARKENS
  MODULE_BASIS,        // unsigned int type;             // type onderverdeling 0 = PLUIMVEE_CL; 1 = PLUIMVEE_PB;  
  HOTRACO,             // unsigned int firma;            // firma 0 = hotraco
  VERSIE_PROG,         // unsigned int versie_programma; // versie nummer eprom
  VERSIE_ALG_PC,       // unsigned int versie_PC;        // versie pc
  0,                   // unsigned long serie_number;    // serie number
  VERSIE_TEKST,        // unsigned int versie_tekst;     // reserve voor toekomst wat vooraan moet staan zoals bovenstaade items
  VERSIE_TEKST_INST,   // unsigned int versie_tekst_inst;// reserve voor toekomst wat vooraan moet staan zoals bovenstaade items
  0,   // unsigned int reserve2;         // reserve voor toekomst wat vooraan moet staan zoals bovenstaade items
  0,   // unsigned int reserve3;         // reserve voor toekomst wat vooraan moet staan zoals bovenstaade items
  0,   // unsigned int reserve4;         // reserve voor toekomst wat vooraan moet staan zoals bovenstaade items

  0,   // int change_cnt;           // variabele wordt met 1 opgehoogd als options veranderd zijn

  #ifdef PASSWORD
  0,   // unsigned int password_gebruiker_;
  0,   // unsigned int password_installateur_;
  #else // PASSWORD
  0,   // unsigned int password_gebruiker;
  0,   // unsigned int password_installateur;
  #endif // PASSWORD
  0,   // password_pc
  0,   // char lcd_angle;               // LCD angle (Helderheid)
  1,   // unsigned char lcd_dimmen;     // LCD dimmen als de Orion niet bediend wordt.
  0,   // unsigned char taalkeuze;      // 0 = default taal  1 = 2e taal  2 = Taal in RAM
  0,   // unsigned char taalkeuze_inst; // 0 = default taal  1 = 2e taal  2 = Taal in RAM
  384, // int rs232;                    // 0=geen; 96=9k6; 192=19k2; 384=38k4; 576=57k6
  0,   // unsigned char can_backbone;   // 0 = geen can_backbone; 1 = can_backbone (backbone communication used)
  #ifdef CAN_BACKBONE_PC_WARNING
  0, // unsigned char _can_rs232;        // wordt niet meer gebruikt
  #else // CAN_BACKBONE_PC_WARNING
  0, // unsigned char can_rs232;         // 0 = geen CAN-RS232; 1 = CAN-RS232 (blackbox)
  #endif // CAN_BACKBONE_PC_WARNING
  1,   // unsigned int adres;           // adres orion voor rainbow

  0, // unsigned char Dummy;
  0, // unsigned char CANopenPossible;

  50, // int CanBaudrate; // Baudrate CAN Backbone in kBaud

  1,   // unsigned char com1_enabled;
  384, // int           com1_bd;
  0,   // unsigned char com1_modem;
  5,   // unsigned char com1_modem_answer;
  0,   // unsigned char com2_enabled;
  384, // int           com2_bd;
  0,   // unsigned char com2_modem;
  5,   // unsigned char com2_modem_answer;
  0,   // unsigned char ethernet_enabled;
  { // unsigned char ethernet[3][4];
    { 192, 168,   1,  40 }, // ethernet[0] = netmask
    { 255, 255, 255,   0 }, // ethernet[1] = hostmask
    { 192, 168,   1, 254 }, // ethernet[2] = routeradres
  },  
  5843, // unsigned int  ethernet_port;                              
  
  0, // unsigned char ethernet_module; // gebruik van ethernet is mogelijk
  0, // unsigned char sd_module;       // gebruik van sd kaart is mogelijk
  0, // unsigned char BACnet_possible;
  0, // unsigned char BACnet_enabled;

  //#ifdef CAN_BACKBONE_PC_WARNING
  { 0,0,0 }, // unsigned char can_backbone_pc_warning[3]; // voor aanzetten waarschuwing indien een smartlink verbinding wegvalt
  //#endif // CAN_BACKBONE_PC_WARNING
  #ifdef PASSWORD
  { 0,0,0,0,0 }, // unsigned char password_level[5];
  { 0,0,0,0,0 }, // unsigned int password_nummer[5];
  #endif // PASSWORD

  1, // unsigned long BACnet_Device_Id;
  65000, // unsigned int BACnet_Network_Nr;

  0, // unsigned char hoogendoorn_possible;
  0, // unsigned char hoogendoorn_enabled;

  0xAAAA,     // unsigned int end;    // Laatste adres opties. Bevat de checksum
};

//=============================================================================
s_opt_app const default_opt_app = 
{
  0,                   // unsigned int area_size;        // Option size, verschil begin option, end option
  ORION,               // unsigned int computer;         // identification computer
  MULTI_CONNECT,       // unsigned int soort;            // soort 0 = BASIS; 1 = PLUIMVEE; 2 = VARKENS
  MODULE_BASIS,        // unsigned int type;             // type onderverdeling 0 = PLUIMVEE_CL; 1 = PLUIMVEE_PB;  
  HOTRACO,             // unsigned int firma;            // firma 0 = hotraco
  VERSIE_PROG,         // unsigned int versie_programma; // versie nummer eprom
  VERSIE_ALG_PC,       // unsigned int versie_PC;        // versie pc
  0,                   // unsigned long serie_number;    // serie number
  VERSIE_TEKST,        // unsigned int versie_tekst;     // reserve voor toekomst wat vooraan moet staan zoals bovenstaade items
  VERSIE_TEKST_INST,   // unsigned int versie_tekst_inst;// reserve voor toekomst wat vooraan moet staan zoals bovenstaade items
  0,   // unsigned int reserve2;         // reserve voor toekomst wat vooraan moet staan zoals bovenstaade items
  0,   // unsigned int reserve3;         // reserve voor toekomst wat vooraan moet staan zoals bovenstaade items
  0,   // unsigned int reserve4;         // reserve voor toekomst wat vooraan moet staan zoals bovenstaade items

  0,   // int change_cnt;           // variabele wordt met 1 opgehoogd als options veranderd zijn

  0, // unsigned char NumberMotorgroups;

  { // sOptMotorgroup Motorgroup[MAX_GROUP];
    { // Motorgroup[0]
      0,                 // unsigned char Enabled;
      0,                 // unsigned char Number;
      0,                 // unsigned char Type;
      0,                 // unsigned char NumberMotors;
      0,                 // unsigned char BusType;
      0,                 // unsigned char FirstNumber;
      0,                 // unsigned char FirstAddress;
      0,                 // unsigned char ControlType;
      0,                 // unsigned char ControlIndex;
      0,                 // unsigned char FrequencyControlled;
      0,                 // unsigned char DispHiSpeedInput;
      0,                 // unsigned char AnalogSpeed;
      50,                // unsigned char SpeedLow;
      100,               // unsigned char SpeedHi;
      100,               // int PositionLowSpeed;
      0,                 // unsigned char PulseSystem;
      0,                 // unsigned char KierRegeling;
      0,                 // unsigned char AnalogInput;
      0,                 // unsigned char DigitalInput;
      1,                 // unsigned char AlarmAnaloog;
      0,                 // unsigned char AnalogOutput;
      150,               // int FrequentieVerstel;
      0,                 // unsigned char ClosedLoopOpenLoop; // 0 = Closed Loop; 1 = Open Loop
      100,               // unsigned char PowerFactor;        // Correctie factor voor vermogen ventilatoren [1.00]
      12,                // unsigned char RampUp;
      12,                // unsigned char RampDown;
      0,                 // unsigned char SensorType;
      100,               // unsigned char VentAtMax;  // uitsturing naar ventilatoren bij 100%
      0,                 // unsigned char WatchdogMode; // watchdog mode enabled bij RS485 sturing
      0,                 // unsigned char PositionAtCommunicationFailure; // positie bij een watchdog fault bij RS485 sturing
      {0,0},             // int Reserved[2];
      1800,              // int Runtime; // x100ms
      {0,0,0,0},         // s_board_IO_on_off DigInHiSpeed;
      {0,0,0,0},         // s_board_IO_on_off DigInOpen;
      {0,0,0,0},         // s_board_IO_on_off DigInClose;
      {0,0,0,0},         // s_board_IO_on_off AnaOutPosition;
      {0,0,0,0},         // s_board_IO_on_off DigOutAlarm;
      {0,0,0,0},         // s_board_IO_on_off AnaInPosition;
      {0,0,0,0},         // s_board_IO_on_off DigOutAlarmFlap;
    },
    { // Motorgroup[1]
      0,                 // unsigned char Enabled;
      1,                 // unsigned char Number;
      0,                 // unsigned char Type;
      0,                 // unsigned char NumberMotors;
      0,                 // unsigned char BusType;
      0,                 // unsigned char FirstNumber;
      0,                 // unsigned char FirstAddress;
      0,                 // unsigned char ControlType;
      0,                 // unsigned char ControlIndex;
      0,                 // unsigned char FrequencyControlled;
      0,                 // unsigned char DispHiSpeedInput;
      0,                 // unsigned char AnalogSpeed;
      50,                // unsigned char SpeedLow;
      100,               // unsigned char SpeedHi;
      100,               // int PositionLowSpeed;
      0,                 // unsigned char PulseSystem;
      0,                 // unsigned char KierRegeling;
      0,                 // unsigned char AnalogInput;
      0,                 // unsigned char DigitalInput;
      1,                 // unsigned char AlarmAnaloog;
      0,                 // unsigned char AnalogOutput;
      150,               // int FrequentieVerstel;
      0,                 // unsigned char ClosedLoopOpenLoop; // 0 = Closed Loop; 1 = Open Loop
      100,               // unsigned char PowerFactor;        // Correctie factor voor vermogen ventilatoren [1.00]
      12,                // unsigned char RampUp;
      12,                // unsigned char RampDown;
      0,                 // unsigned char SensorType;
      100,               // unsigned char VentAtMax;  // uitsturing naar ventilatoren bij 100%
      0,                 // unsigned char WatchdogMode; // watchdog mode enabled bij RS485 sturing
      0,                 // unsigned char PositionAtCommunicationFailure; // positie bij een watchdog fault bij RS485 sturing
      {0,0},             // int Reserved[2];
      1800,              // int Runtime; // x100ms
      {0,0,0,0},         // s_board_IO_on_off DigInHiSpeed;
      {0,0,0,0},         // s_board_IO_on_off DigInOpen;
      {0,0,0,0},         // s_board_IO_on_off DigInClose;
      {0,0,0,0},         // s_board_IO_on_off AnaOutPosition;
      {0,0,0,0},         // s_board_IO_on_off DigOutAlarm;
      {0,0,0,0},         // s_board_IO_on_off AnaInPosition;
      {0,0,0,0},         // s_board_IO_on_off DigOutAlarmFlap;
    },
    { // Motorgroup[2]
      0,                 // unsigned char Enabled;
      2,                 // unsigned char Number;
      0,                 // unsigned char Type;
      0,                 // unsigned char NumberMotors;
      0,                 // unsigned char BusType;
      0,                 // unsigned char FirstNumber;
      0,                 // unsigned char FirstAddress;
      0,                 // unsigned char ControlType;
      0,                 // unsigned char ControlIndex;
      0,                 // unsigned char FrequencyControlled;
      0,                 // unsigned char DispHiSpeedInput;
      0,                 // unsigned char AnalogSpeed;
      50,                // unsigned char SpeedLow;
      100,               // unsigned char SpeedHi;
      100,               // int PositionLowSpeed;
      0,                 // unsigned char PulseSystem;
      0,                 // unsigned char KierRegeling;
      0,                 // unsigned char AnalogInput;
      0,                 // unsigned char DigitalInput;
      1,                 // unsigned char AlarmAnaloog;
      0,                 // unsigned char AnalogOutput;
      150,               // int FrequentieVerstel;
      0,                 // unsigned char ClosedLoopOpenLoop; // 0 = Closed Loop; 1 = Open Loop
      100,               // unsigned char PowerFactor;        // Correctie factor voor vermogen ventilatoren [1.00]
      12,                // unsigned char RampUp;
      12,                // unsigned char RampDown;
      0,                 // unsigned char SensorType;
      100,               // unsigned char VentAtMax;  // uitsturing naar ventilatoren bij 100%
      0,                 // unsigned char WatchdogMode; // watchdog mode enabled bij RS485 sturing
      0,                 // unsigned char PositionAtCommunicationFailure; // positie bij een watchdog fault bij RS485 sturing
      {0,0},             // int Reserved[2];
      1800,              // int Runtime; // x100ms
      {0,0,0,0},         // s_board_IO_on_off DigInHiSpeed;
      {0,0,0,0},         // s_board_IO_on_off DigInOpen;
      {0,0,0,0},         // s_board_IO_on_off DigInClose;
      {0,0,0,0},         // s_board_IO_on_off AnaOutPosition;
      {0,0,0,0},         // s_board_IO_on_off DigOutAlarm;
      {0,0,0,0},         // s_board_IO_on_off AnaInPosition;
      {0,0,0,0},         // s_board_IO_on_off DigOutAlarmFlap;
    },
    { // Motorgroup[3]
      0,                 // unsigned char Enabled;
      3,                 // unsigned char Number;
      0,                 // unsigned char Type;
      0,                 // unsigned char NumberMotors;
      0,                 // unsigned char BusType;
      0,                 // unsigned char FirstNumber;
      0,                 // unsigned char FirstAddress;
      0,                 // unsigned char ControlType;
      0,                 // unsigned char ControlIndex;
      0,                 // unsigned char FrequencyControlled;
      0,                 // unsigned char DispHiSpeedInput;
      0,                 // unsigned char AnalogSpeed;
      50,                // unsigned char SpeedLow;
      100,               // unsigned char SpeedHi;
      100,               // int PositionLowSpeed;
      0,                 // unsigned char PulseSystem;
      0,                 // unsigned char KierRegeling;
      0,                 // unsigned char AnalogInput;
      0,                 // unsigned char DigitalInput;
      1,                 // unsigned char AlarmAnaloog;
      0,                 // unsigned char AnalogOutput;
      150,               // int FrequentieVerstel;
      0,                 // unsigned char ClosedLoopOpenLoop; // 0 = Closed Loop; 1 = Open Loop
      100,               // unsigned char PowerFactor;        // Correctie factor voor vermogen ventilatoren [1.00]
      12,                // unsigned char RampUp;
      12,                // unsigned char RampDown;
      0,                 // unsigned char SensorType;
      100,               // unsigned char VentAtMax;  // uitsturing naar ventilatoren bij 100%
      0,                 // unsigned char WatchdogMode; // watchdog mode enabled bij RS485 sturing
      0,                 // unsigned char PositionAtCommunicationFailure; // positie bij een watchdog fault bij RS485 sturing
      {0,0},             // int Reserved[2];
      1800,              // int Runtime; // x100ms
      {0,0,0,0},         // s_board_IO_on_off DigInHiSpeed;
      {0,0,0,0},         // s_board_IO_on_off DigInOpen;
      {0,0,0,0},         // s_board_IO_on_off DigInClose;
      {0,0,0,0},         // s_board_IO_on_off AnaOutPosition;
      {0,0,0,0},         // s_board_IO_on_off DigOutAlarm;
      {0,0,0,0},         // s_board_IO_on_off AnaInPosition;
      {0,0,0,0},         // s_board_IO_on_off DigOutAlarmFlap;
    },
    { // Motorgroup[4]
      0,                 // unsigned char Enabled;
      4,                 // unsigned char Number;
      0,                 // unsigned char Type;
      0,                 // unsigned char NumberMotors;
      0,                 // unsigned char BusType;
      0,                 // unsigned char FirstNumber;
      0,                 // unsigned char FirstAddress;
      0,                 // unsigned char ControlType;
      0,                 // unsigned char ControlIndex;
      0,                 // unsigned char FrequencyControlled;
      0,                 // unsigned char DispHiSpeedInput;
      0,                 // unsigned char AnalogSpeed;
      50,                // unsigned char SpeedLow;
      100,               // unsigned char SpeedHi;
      100,               // int PositionLowSpeed;
      0,                 // unsigned char PulseSystem;
      0,                 // unsigned char KierRegeling;
      0,                 // unsigned char AnalogInput;
      0,                 // unsigned char DigitalInput;
      1,                 // unsigned char AlarmAnaloog;
      0,                 // unsigned char AnalogOutput;
      150,               // int FrequentieVerstel;
      0,                 // unsigned char ClosedLoopOpenLoop; // 0 = Closed Loop; 1 = Open Loop
      100,               // unsigned char PowerFactor;        // Correctie factor voor vermogen ventilatoren [1.00]
      12,                // unsigned char RampUp;
      12,                // unsigned char RampDown;
      0,                 // unsigned char SensorType;
      100,               // unsigned char VentAtMax;  // uitsturing naar ventilatoren bij 100%
      0,                 // unsigned char WatchdogMode; // watchdog mode enabled bij RS485 sturing
      0,                 // unsigned char PositionAtCommunicationFailure; // positie bij een watchdog fault bij RS485 sturing
      {0,0},             // int Reserved[2];
      1800,              // int Runtime; // x100ms
      {0,0,0,0},         // s_board_IO_on_off DigInHiSpeed;
      {0,0,0,0},         // s_board_IO_on_off DigInOpen;
      {0,0,0,0},         // s_board_IO_on_off DigInClose;
      {0,0,0,0},         // s_board_IO_on_off AnaOutPosition;
      {0,0,0,0},         // s_board_IO_on_off DigOutAlarm;
      {0,0,0,0},         // s_board_IO_on_off AnaInPosition;
      {0,0,0,0},         // s_board_IO_on_off DigOutAlarmFlap;
    },
    { // Motorgroup[5]
      0,                 // unsigned char Enabled;
      5,                 // unsigned char Number;
      0,                 // unsigned char Type;
      0,                 // unsigned char NumberMotors;
      0,                 // unsigned char BusType;
      0,                 // unsigned char FirstNumber;
      0,                 // unsigned char FirstAddress;
      0,                 // unsigned char ControlType;
      0,                 // unsigned char ControlIndex;
      0,                 // unsigned char FrequencyControlled;
      0,                 // unsigned char DispHiSpeedInput;
      0,                 // unsigned char AnalogSpeed;
      50,                // unsigned char SpeedLow;
      100,               // unsigned char SpeedHi;
      100,               // int PositionLowSpeed;
      0,                 // unsigned char PulseSystem;
      0,                 // unsigned char KierRegeling;
      0,                 // unsigned char AnalogInput;
      0,                 // unsigned char DigitalInput;
      1,                 // unsigned char AlarmAnaloog;
      0,                 // unsigned char AnalogOutput;
      150,               // int FrequentieVerstel;
      0,                 // unsigned char ClosedLoopOpenLoop; // 0 = Closed Loop; 1 = Open Loop
      100,               // unsigned char PowerFactor;        // Correctie factor voor vermogen ventilatoren [1.00]
      12,                // unsigned char RampUp;
      12,                // unsigned char RampDown;
      0,                 // unsigned char SensorType;
      100,               // unsigned char VentAtMax;  // uitsturing naar ventilatoren bij 100%
      0,                 // unsigned char WatchdogMode; // watchdog mode enabled bij RS485 sturing
      0,                 // unsigned char PositionAtCommunicationFailure; // positie bij een watchdog fault bij RS485 sturing
      {0,0},             // int Reserved[2];
      1800,              // int Runtime; // x100ms
      {0,0,0,0},         // s_board_IO_on_off DigInHiSpeed;
      {0,0,0,0},         // s_board_IO_on_off DigInOpen;
      {0,0,0,0},         // s_board_IO_on_off DigInClose;
      {0,0,0,0},         // s_board_IO_on_off AnaOutPosition;
      {0,0,0,0},         // s_board_IO_on_off DigOutAlarm;
      {0,0,0,0},         // s_board_IO_on_off AnaInPosition;
      {0,0,0,0},         // s_board_IO_on_off DigOutAlarmFlap;
    },
    { // Motorgroup[6]
      0,                 // unsigned char Enabled;
      6,                 // unsigned char Number;
      0,                 // unsigned char Type;
      0,                 // unsigned char NumberMotors;
      0,                 // unsigned char BusType;
      0,                 // unsigned char FirstNumber;
      0,                 // unsigned char FirstAddress;
      0,                 // unsigned char ControlType;
      0,                 // unsigned char ControlIndex;
      0,                 // unsigned char FrequencyControlled;
      0,                 // unsigned char DispHiSpeedInput;
      0,                 // unsigned char AnalogSpeed;
      50,                // unsigned char SpeedLow;
      100,               // unsigned char SpeedHi;
      100,               // int PositionLowSpeed;
      0,                 // unsigned char PulseSystem;
      0,                 // unsigned char KierRegeling;
      0,                 // unsigned char AnalogInput;
      0,                 // unsigned char DigitalInput;
      1,                 // unsigned char AlarmAnaloog;
      0,                 // unsigned char AnalogOutput;
      150,               // int FrequentieVerstel;
      0,                 // unsigned char ClosedLoopOpenLoop; // 0 = Closed Loop; 1 = Open Loop
      100,               // unsigned char PowerFactor;        // Correctie factor voor vermogen ventilatoren [1.00]
      12,                // unsigned char RampUp;
      12,                // unsigned char RampDown;
      0,                 // unsigned char SensorType;
      100,               // unsigned char VentAtMax;  // uitsturing naar ventilatoren bij 100%
      0,                 // unsigned char WatchdogMode; // watchdog mode enabled bij RS485 sturing
      0,                 // unsigned char PositionAtCommunicationFailure; // positie bij een watchdog fault bij RS485 sturing
      {0,0},             // int Reserved[2];
      1800,              // int Runtime; // x100ms
      {0,0,0,0},         // s_board_IO_on_off DigInHiSpeed;
      {0,0,0,0},         // s_board_IO_on_off DigInOpen;
      {0,0,0,0},         // s_board_IO_on_off DigInClose;
      {0,0,0,0},         // s_board_IO_on_off AnaOutPosition;
      {0,0,0,0},         // s_board_IO_on_off DigOutAlarm;
      {0,0,0,0},         // s_board_IO_on_off AnaInPosition;
      {0,0,0,0},         // s_board_IO_on_off DigOutAlarmFlap;
    },
    { // Motorgroup[7]
      0,                 // unsigned char Enabled;
      7,                 // unsigned char Number;
      0,                 // unsigned char Type;
      0,                 // unsigned char NumberMotors;
      0,                 // unsigned char BusType;
      0,                 // unsigned char FirstNumber;
      0,                 // unsigned char FirstAddress;
      0,                 // unsigned char ControlType;
      0,                 // unsigned char ControlIndex;
      0,                 // unsigned char FrequencyControlled;
      0,                 // unsigned char DispHiSpeedInput;
      0,                 // unsigned char AnalogSpeed;
      50,                // unsigned char SpeedLow;
      100,               // unsigned char SpeedHi;
      100,               // int PositionLowSpeed;
      0,                 // unsigned char PulseSystem;
      0,                 // unsigned char KierRegeling;
      0,                 // unsigned char AnalogInput;
      0,                 // unsigned char DigitalInput;
      1,                 // unsigned char AlarmAnaloog;
      0,                 // unsigned char AnalogOutput;
      150,               // int FrequentieVerstel;
      0,                 // unsigned char ClosedLoopOpenLoop; // 0 = Closed Loop; 1 = Open Loop
      100,               // unsigned char PowerFactor;        // Correctie factor voor vermogen ventilatoren [1.00]
      12,                // unsigned char RampUp;
      12,                // unsigned char RampDown;
      0,                 // unsigned char SensorType;
      100,               // unsigned char VentAtMax;  // uitsturing naar ventilatoren bij 100%
      0,                 // unsigned char WatchdogMode; // watchdog mode enabled bij RS485 sturing
      0,                 // unsigned char PositionAtCommunicationFailure; // positie bij een watchdog fault bij RS485 sturing
      {0,0},             // int Reserved[2];
      1800,              // int Runtime; // x100ms
      {0,0,0,0},         // s_board_IO_on_off DigInHiSpeed;
      {0,0,0,0},         // s_board_IO_on_off DigInOpen;
      {0,0,0,0},         // s_board_IO_on_off DigInClose;
      {0,0,0,0},         // s_board_IO_on_off AnaOutPosition;
      {0,0,0,0},         // s_board_IO_on_off DigOutAlarm;
      {0,0,0,0},         // s_board_IO_on_off AnaInPosition;
      {0,0,0,0},         // s_board_IO_on_off DigOutAlarmFlap;
    },
    { // Motorgroup[8]
      0,                 // unsigned char Enabled;
      8,                 // unsigned char Number;
      0,                 // unsigned char Type;
      0,                 // unsigned char NumberMotors;
      0,                 // unsigned char BusType;
      0,                 // unsigned char FirstNumber;
      0,                 // unsigned char FirstAddress;
      0,                 // unsigned char ControlType;
      0,                 // unsigned char ControlIndex;
      0,                 // unsigned char FrequencyControlled;
      0,                 // unsigned char DispHiSpeedInput;
      0,                 // unsigned char AnalogSpeed;
      50,                // unsigned char SpeedLow;
      100,               // unsigned char SpeedHi;
      100,               // int PositionLowSpeed;
      0,                 // unsigned char PulseSystem;
      0,                 // unsigned char KierRegeling;
      0,                 // unsigned char AnalogInput;
      0,                 // unsigned char DigitalInput;
      1,                 // unsigned char AlarmAnaloog;
      0,                 // unsigned char AnalogOutput;
      150,               // int FrequentieVerstel;
      0,                 // unsigned char ClosedLoopOpenLoop; // 0 = Closed Loop; 1 = Open Loop
      100,               // unsigned char PowerFactor;        // Correctie factor voor vermogen ventilatoren [1.00]
      12,                // unsigned char RampUp;
      12,                // unsigned char RampDown;
      0,                 // unsigned char SensorType;
      100,               // unsigned char VentAtMax;  // uitsturing naar ventilatoren bij 100%
      0,                 // unsigned char WatchdogMode; // watchdog mode enabled bij RS485 sturing
      0,                 // unsigned char PositionAtCommunicationFailure; // positie bij een watchdog fault bij RS485 sturing
      {0,0},             // int Reserved[2];
      1800,              // int Runtime; // x100ms
      {0,0,0,0},         // s_board_IO_on_off DigInHiSpeed;
      {0,0,0,0},         // s_board_IO_on_off DigInOpen;
      {0,0,0,0},         // s_board_IO_on_off DigInClose;
      {0,0,0,0},         // s_board_IO_on_off AnaOutPosition;
      {0,0,0,0},         // s_board_IO_on_off DigOutAlarm;
      {0,0,0,0},         // s_board_IO_on_off AnaInPosition;
      {0,0,0,0},         // s_board_IO_on_off DigOutAlarmFlap;
    },
    { // Motorgroup[9]
      0,                 // unsigned char Enabled;
      9,                 // unsigned char Number;
      0,                 // unsigned char Type;
      0,                 // unsigned char NumberMotors;
      0,                 // unsigned char BusType;
      0,                 // unsigned char FirstNumber;
      0,                 // unsigned char FirstAddress;
      0,                 // unsigned char ControlType;
      0,                 // unsigned char ControlIndex;
      0,                 // unsigned char FrequencyControlled;
      0,                 // unsigned char DispHiSpeedInput;
      0,                 // unsigned char AnalogSpeed;
      50,                // unsigned char SpeedLow;
      100,               // unsigned char SpeedHi;
      100,               // int PositionLowSpeed;
      0,                 // unsigned char PulseSystem;
      0,                 // unsigned char KierRegeling;
      0,                 // unsigned char AnalogInput;
      0,                 // unsigned char DigitalInput;
      1,                 // unsigned char AlarmAnaloog;
      0,                 // unsigned char AnalogOutput;
      150,               // int FrequentieVerstel;
      0,                 // unsigned char ClosedLoopOpenLoop; // 0 = Closed Loop; 1 = Open Loop
      100,               // unsigned char PowerFactor;        // Correctie factor voor vermogen ventilatoren [1.00]
      12,                // unsigned char RampUp;
      12,                // unsigned char RampDown;
      0,                 // unsigned char SensorType;
      100,               // unsigned char VentAtMax;  // uitsturing naar ventilatoren bij 100%
      0,                 // unsigned char WatchdogMode; // watchdog mode enabled bij RS485 sturing
      0,                 // unsigned char PositionAtCommunicationFailure; // positie bij een watchdog fault bij RS485 sturing
      {0,0},             // int Reserved[2];
      1800,              // int Runtime; // x100ms
      {0,0,0,0},         // s_board_IO_on_off DigInHiSpeed;
      {0,0,0,0},         // s_board_IO_on_off DigInOpen;
      {0,0,0,0},         // s_board_IO_on_off DigInClose;
      {0,0,0,0},         // s_board_IO_on_off AnaOutPosition;
      {0,0,0,0},         // s_board_IO_on_off DigOutAlarm;
      {0,0,0,0},         // s_board_IO_on_off AnaInPosition;
      {0,0,0,0},         // s_board_IO_on_off DigOutAlarmFlap;
    },
    { // Motorgroup[10]
      0,                 // unsigned char Enabled;
      10,                // unsigned char Number;
      0,                 // unsigned char Type;
      0,                 // unsigned char NumberMotors;
      0,                 // unsigned char BusType;
      0,                 // unsigned char FirstNumber;
      0,                 // unsigned char FirstAddress;
      0,                 // unsigned char ControlType;
      0,                 // unsigned char ControlIndex;
      0,                 // unsigned char FrequencyControlled;
      0,                 // unsigned char DispHiSpeedInput;
      0,                 // unsigned char AnalogSpeed;
      50,                // unsigned char SpeedLow;
      100,               // unsigned char SpeedHi;
      100,               // int PositionLowSpeed;
      0,                 // unsigned char PulseSystem;
      0,                 // unsigned char KierRegeling;
      0,                 // unsigned char AnalogInput;
      0,                 // unsigned char DigitalInput;
      1,                 // unsigned char AlarmAnaloog;
      0,                 // unsigned char AnalogOutput;
      150,               // int FrequentieVerstel;
      0,                 // unsigned char ClosedLoopOpenLoop; // 0 = Closed Loop; 1 = Open Loop
      100,               // unsigned char PowerFactor;        // Correctie factor voor vermogen ventilatoren [1.00]
      12,                // unsigned char RampUp;
      12,                // unsigned char RampDown;
      0,                 // unsigned char SensorType;
      100,               // unsigned char VentAtMax;  // uitsturing naar ventilatoren bij 100%
      0,                 // unsigned char WatchdogMode; // watchdog mode enabled bij RS485 sturing
      0,                 // unsigned char PositionAtCommunicationFailure; // positie bij een watchdog fault bij RS485 sturing
      {0,0},             // int Reserved[2];
      1800,              // int Runtime; // x100ms
      {0,0,0,0},         // s_board_IO_on_off DigInHiSpeed;
      {0,0,0,0},         // s_board_IO_on_off DigInOpen;
      {0,0,0,0},         // s_board_IO_on_off DigInClose;
      {0,0,0,0},         // s_board_IO_on_off AnaOutPosition;
      {0,0,0,0},         // s_board_IO_on_off DigOutAlarm;
      {0,0,0,0},         // s_board_IO_on_off AnaInPosition;
      {0,0,0,0},         // s_board_IO_on_off DigOutAlarmFlap;
    },
    { // Motorgroup[11]
      0,                 // unsigned char Enabled;
      11,                // unsigned char Number;
      0,                 // unsigned char Type;
      0,                 // unsigned char NumberMotors;
      0,                 // unsigned char BusType;
      0,                 // unsigned char FirstNumber;
      0,                 // unsigned char FirstAddress;
      0,                 // unsigned char ControlType;
      0,                 // unsigned char ControlIndex;
      0,                 // unsigned char FrequencyControlled;
      0,                 // unsigned char DispHiSpeedInput;
      0,                 // unsigned char AnalogSpeed;
      50,                // unsigned char SpeedLow;
      100,               // unsigned char SpeedHi;
      100,               // int PositionLowSpeed;
      0,                 // unsigned char PulseSystem;
      0,                 // unsigned char KierRegeling;
      0,                 // unsigned char AnalogInput;
      0,                 // unsigned char DigitalInput;
      1,                 // unsigned char AlarmAnaloog;
      0,                 // unsigned char AnalogOutput;
      150,               // int FrequentieVerstel;
      0,                 // unsigned char ClosedLoopOpenLoop; // 0 = Closed Loop; 1 = Open Loop
      100,               // unsigned char PowerFactor;        // Correctie factor voor vermogen ventilatoren [1.00]
      12,                // unsigned char RampUp;
      12,                // unsigned char RampDown;
      0,                 // unsigned char SensorType;
      100,               // unsigned char VentAtMax;  // uitsturing naar ventilatoren bij 100%
      0,                 // unsigned char WatchdogMode; // watchdog mode enabled bij RS485 sturing
      0,                 // unsigned char PositionAtCommunicationFailure; // positie bij een watchdog fault bij RS485 sturing
      {0,0},             // int Reserved[2];
      1800,              // int Runtime; // x100ms
      {0,0,0,0},         // s_board_IO_on_off DigInHiSpeed;
      {0,0,0,0},         // s_board_IO_on_off DigInOpen;
      {0,0,0,0},         // s_board_IO_on_off DigInClose;
      {0,0,0,0},         // s_board_IO_on_off AnaOutPosition;
      {0,0,0,0},         // s_board_IO_on_off DigOutAlarm;
      {0,0,0,0},         // s_board_IO_on_off AnaInPosition;
      {0,0,0,0},         // s_board_IO_on_off DigOutAlarmFlap;
    },
    { // Motorgroup[12]
      0,                 // unsigned char Enabled;
      12,                // unsigned char Number;
      0,                 // unsigned char Type;
      0,                 // unsigned char NumberMotors;
      0,                 // unsigned char BusType;
      0,                 // unsigned char FirstNumber;
      0,                 // unsigned char FirstAddress;
      0,                 // unsigned char ControlType;
      0,                 // unsigned char ControlIndex;
      0,                 // unsigned char FrequencyControlled;
      0,                 // unsigned char DispHiSpeedInput;
      0,                 // unsigned char AnalogSpeed;
      50,                // unsigned char SpeedLow;
      100,               // unsigned char SpeedHi;
      100,               // int PositionLowSpeed;
      0,                 // unsigned char PulseSystem;
      0,                 // unsigned char KierRegeling;
      0,                 // unsigned char AnalogInput;
      0,                 // unsigned char DigitalInput;
      1,                 // unsigned char AlarmAnaloog;
      0,                 // unsigned char AnalogOutput;
      150,               // int FrequentieVerstel;
      0,                 // unsigned char ClosedLoopOpenLoop; // 0 = Closed Loop; 1 = Open Loop
      100,               // unsigned char PowerFactor;        // Correctie factor voor vermogen ventilatoren [1.00]
      12,                // unsigned char RampUp;
      12,                // unsigned char RampDown;
      0,                 // unsigned char SensorType;
      100,               // unsigned char VentAtMax;  // uitsturing naar ventilatoren bij 100%
      0,                 // unsigned char WatchdogMode; // watchdog mode enabled bij RS485 sturing
      0,                 // unsigned char PositionAtCommunicationFailure; // positie bij een watchdog fault bij RS485 sturing
      {0,0},             // int Reserved[2];
      1800,              // int Runtime; // x100ms
      {0,0,0,0},         // s_board_IO_on_off DigInHiSpeed;
      {0,0,0,0},         // s_board_IO_on_off DigInOpen;
      {0,0,0,0},         // s_board_IO_on_off DigInClose;
      {0,0,0,0},         // s_board_IO_on_off AnaOutPosition;
      {0,0,0,0},         // s_board_IO_on_off DigOutAlarm;
      {0,0,0,0},         // s_board_IO_on_off AnaInPosition;
      {0,0,0,0},         // s_board_IO_on_off DigOutAlarmFlap;
    },
    { // Motorgroup[13]
      0,                 // unsigned char Enabled;
      13,                // unsigned char Number;
      0,                 // unsigned char Type;
      0,                 // unsigned char NumberMotors;
      0,                 // unsigned char BusType;
      0,                 // unsigned char FirstNumber;
      0,                 // unsigned char FirstAddress;
      0,                 // unsigned char ControlType;
      0,                 // unsigned char ControlIndex;
      0,                 // unsigned char FrequencyControlled;
      0,                 // unsigned char DispHiSpeedInput;
      0,                 // unsigned char AnalogSpeed;
      50,                // unsigned char SpeedLow;
      100,               // unsigned char SpeedHi;
      100,               // int PositionLowSpeed;
      0,                 // unsigned char PulseSystem;
      0,                 // unsigned char KierRegeling;
      0,                 // unsigned char AnalogInput;
      0,                 // unsigned char DigitalInput;
      1,                 // unsigned char AlarmAnaloog;
      0,                 // unsigned char AnalogOutput;
      150,               // int FrequentieVerstel;
      0,                 // unsigned char ClosedLoopOpenLoop; // 0 = Closed Loop; 1 = Open Loop
      100,               // unsigned char PowerFactor;        // Correctie factor voor vermogen ventilatoren [1.00]
      12,                // unsigned char RampUp;
      12,                // unsigned char RampDown;
      0,                 // unsigned char SensorType;
      100,               // unsigned char VentAtMax;  // uitsturing naar ventilatoren bij 100%
      0,                 // unsigned char WatchdogMode; // watchdog mode enabled bij RS485 sturing
      0,                 // unsigned char PositionAtCommunicationFailure; // positie bij een watchdog fault bij RS485 sturing
      {0,0},             // int Reserved[2];
      1800,              // int Runtime; // x100ms
      {0,0,0,0},         // s_board_IO_on_off DigInHiSpeed;
      {0,0,0,0},         // s_board_IO_on_off DigInOpen;
      {0,0,0,0},         // s_board_IO_on_off DigInClose;
      {0,0,0,0},         // s_board_IO_on_off AnaOutPosition;
      {0,0,0,0},         // s_board_IO_on_off DigOutAlarm;
      {0,0,0,0},         // s_board_IO_on_off AnaInPosition;
      {0,0,0,0},         // s_board_IO_on_off DigOutAlarmFlap;
    },
    { // Motorgroup[14]
      0,                 // unsigned char Enabled;
      14,                // unsigned char Number;
      0,                 // unsigned char Type;
      0,                 // unsigned char NumberMotors;
      0,                 // unsigned char BusType;
      0,                 // unsigned char FirstNumber;
      0,                 // unsigned char FirstAddress;
      0,                 // unsigned char ControlType;
      0,                 // unsigned char ControlIndex;
      0,                 // unsigned char FrequencyControlled;
      0,                 // unsigned char DispHiSpeedInput;
      0,                 // unsigned char AnalogSpeed;
      50,                // unsigned char SpeedLow;
      100,               // unsigned char SpeedHi;
      100,               // int PositionLowSpeed;
      0,                 // unsigned char PulseSystem;
      0,                 // unsigned char KierRegeling;
      0,                 // unsigned char AnalogInput;
      0,                 // unsigned char DigitalInput;
      1,                 // unsigned char AlarmAnaloog;
      0,                 // unsigned char AnalogOutput;
      150,               // int FrequentieVerstel;
      0,                 // unsigned char ClosedLoopOpenLoop; // 0 = Closed Loop; 1 = Open Loop
      100,               // unsigned char PowerFactor;        // Correctie factor voor vermogen ventilatoren [1.00]
      12,                // unsigned char RampUp;
      12,                // unsigned char RampDown;
      0,                 // unsigned char SensorType;
      100,               // unsigned char VentAtMax;  // uitsturing naar ventilatoren bij 100%
      0,                 // unsigned char WatchdogMode; // watchdog mode enabled bij RS485 sturing
      0,                 // unsigned char PositionAtCommunicationFailure; // positie bij een watchdog fault bij RS485 sturing
      {0,0},             // int Reserved[2];
      1800,              // int Runtime; // x100ms
      {0,0,0,0},         // s_board_IO_on_off DigInHiSpeed;
      {0,0,0,0},         // s_board_IO_on_off DigInOpen;
      {0,0,0,0},         // s_board_IO_on_off DigInClose;
      {0,0,0,0},         // s_board_IO_on_off AnaOutPosition;
      {0,0,0,0},         // s_board_IO_on_off DigOutAlarm;
      {0,0,0,0},         // s_board_IO_on_off AnaInPosition;
      {0,0,0,0},         // s_board_IO_on_off DigOutAlarmFlap;
    },
    { // Motorgroup[15]
      0,                 // unsigned char Enabled;
      15,                // unsigned char Number;
      0,                 // unsigned char Type;
      0,                 // unsigned char NumberMotors;
      0,                 // unsigned char BusType;
      0,                 // unsigned char FirstNumber;
      0,                 // unsigned char FirstAddress;
      0,                 // unsigned char ControlType;
      0,                 // unsigned char ControlIndex;
      0,                 // unsigned char FrequencyControlled;
      0,                 // unsigned char DispHiSpeedInput;
      0,                 // unsigned char AnalogSpeed;
      50,                // unsigned char SpeedLow;
      100,               // unsigned char SpeedHi;
      100,               // int PositionLowSpeed;
      0,                 // unsigned char PulseSystem;
      0,                 // unsigned char KierRegeling;
      0,                 // unsigned char AnalogInput;
      0,                 // unsigned char DigitalInput;
      1,                 // unsigned char AlarmAnaloog;
      0,                 // unsigned char AnalogOutput;
      150,               // int FrequentieVerstel;
      0,                 // unsigned char ClosedLoopOpenLoop; // 0 = Closed Loop; 1 = Open Loop
      100,               // unsigned char PowerFactor;        // Correctie factor voor vermogen ventilatoren [1.00]
      12,                // unsigned char RampUp;
      12,                // unsigned char RampDown;
      0,                 // unsigned char SensorType;
      100,               // unsigned char VentAtMax;  // uitsturing naar ventilatoren bij 100%
      0,                 // unsigned char WatchdogMode; // watchdog mode enabled bij RS485 sturing
      0,                 // unsigned char PositionAtCommunicationFailure; // positie bij een watchdog fault bij RS485 sturing
      {0,0},             // int Reserved[2];
      1800,              // int Runtime; // x100ms
      {0,0,0,0},         // s_board_IO_on_off DigInHiSpeed;
      {0,0,0,0},         // s_board_IO_on_off DigInOpen;
      {0,0,0,0},         // s_board_IO_on_off DigInClose;
      {0,0,0,0},         // s_board_IO_on_off AnaOutPosition;
      {0,0,0,0},         // s_board_IO_on_off DigOutAlarm;
      {0,0,0,0},         // s_board_IO_on_off AnaInPosition;
      {0,0,0,0},         // s_board_IO_on_off DigOutAlarmFlap;
    },
    { // Motorgroup[16]
      0,                 // unsigned char Enabled;
      16,                // unsigned char Number;
      0,                 // unsigned char Type;
      0,                 // unsigned char NumberMotors;
      0,                 // unsigned char BusType;
      0,                 // unsigned char FirstNumber;
      0,                 // unsigned char FirstAddress;
      0,                 // unsigned char ControlType;
      0,                 // unsigned char ControlIndex;
      0,                 // unsigned char FrequencyControlled;
      0,                 // unsigned char DispHiSpeedInput;
      0,                 // unsigned char AnalogSpeed;
      50,                // unsigned char SpeedLow;
      100,               // unsigned char SpeedHi;
      100,               // int PositionLowSpeed;
      0,                 // unsigned char PulseSystem;
      0,                 // unsigned char KierRegeling;
      0,                 // unsigned char AnalogInput;
      0,                 // unsigned char DigitalInput;
      1,                 // unsigned char AlarmAnaloog;
      0,                 // unsigned char AnalogOutput;
      150,               // int FrequentieVerstel;
      0,                 // unsigned char ClosedLoopOpenLoop; // 0 = Closed Loop; 1 = Open Loop
      100,               // unsigned char PowerFactor;        // Correctie factor voor vermogen ventilatoren [1.00]
      12,                // unsigned char RampUp;
      12,                // unsigned char RampDown;
      0,                 // unsigned char SensorType;
      100,               // unsigned char VentAtMax;  // uitsturing naar ventilatoren bij 100%
      0,                 // unsigned char WatchdogMode; // watchdog mode enabled bij RS485 sturing
      0,                 // unsigned char PositionAtCommunicationFailure; // positie bij een watchdog fault bij RS485 sturing
      {0,0},             // int Reserved[2];
      1800,              // int Runtime; // x100ms
      {0,0,0,0},         // s_board_IO_on_off DigInHiSpeed;
      {0,0,0,0},         // s_board_IO_on_off DigInOpen;
      {0,0,0,0},         // s_board_IO_on_off DigInClose;
      {0,0,0,0},         // s_board_IO_on_off AnaOutPosition;
      {0,0,0,0},         // s_board_IO_on_off DigOutAlarm;
      {0,0,0,0},         // s_board_IO_on_off AnaInPosition;
      {0,0,0,0},         // s_board_IO_on_off DigOutAlarmFlap;
    },
    { // Motorgroup[17]
      0,                 // unsigned char Enabled;
      17,                // unsigned char Number;
      0,                 // unsigned char Type;
      0,                 // unsigned char NumberMotors;
      0,                 // unsigned char BusType;
      0,                 // unsigned char FirstNumber;
      0,                 // unsigned char FirstAddress;
      0,                 // unsigned char ControlType;
      0,                 // unsigned char ControlIndex;
      0,                 // unsigned char FrequencyControlled;
      0,                 // unsigned char DispHiSpeedInput;
      0,                 // unsigned char AnalogSpeed;
      50,                // unsigned char SpeedLow;
      100,               // unsigned char SpeedHi;
      100,               // int PositionLowSpeed;
      0,                 // unsigned char PulseSystem;
      0,                 // unsigned char KierRegeling;
      0,                 // unsigned char AnalogInput;
      0,                 // unsigned char DigitalInput;
      1,                 // unsigned char AlarmAnaloog;
      0,                 // unsigned char AnalogOutput;
      150,               // int FrequentieVerstel;
      0,                 // unsigned char ClosedLoopOpenLoop; // 0 = Closed Loop; 1 = Open Loop
      100,               // unsigned char PowerFactor;        // Correctie factor voor vermogen ventilatoren [1.00]
      12,                // unsigned char RampUp;
      12,                // unsigned char RampDown;
      0,                 // unsigned char SensorType;
      100,               // unsigned char VentAtMax;  // uitsturing naar ventilatoren bij 100%
      0,                 // unsigned char WatchdogMode; // watchdog mode enabled bij RS485 sturing
      0,                 // unsigned char PositionAtCommunicationFailure; // positie bij een watchdog fault bij RS485 sturing
      {0,0},             // int Reserved[2];
      1800,              // int Runtime; // x100ms
      {0,0,0,0},         // s_board_IO_on_off DigInHiSpeed;
      {0,0,0,0},         // s_board_IO_on_off DigInOpen;
      {0,0,0,0},         // s_board_IO_on_off DigInClose;
      {0,0,0,0},         // s_board_IO_on_off AnaOutPosition;
      {0,0,0,0},         // s_board_IO_on_off DigOutAlarm;
      {0,0,0,0},         // s_board_IO_on_off AnaInPosition;
      {0,0,0,0},         // s_board_IO_on_off DigOutAlarmFlap;
    },
    { // Motorgroup[18]
      0,                 // unsigned char Enabled;
      18,                // unsigned char Number;
      0,                 // unsigned char Type;
      0,                 // unsigned char NumberMotors;
      0,                 // unsigned char BusType;
      0,                 // unsigned char FirstNumber;
      0,                 // unsigned char FirstAddress;
      0,                 // unsigned char ControlType;
      0,                 // unsigned char ControlIndex;
      0,                 // unsigned char FrequencyControlled;
      0,                 // unsigned char DispHiSpeedInput;
      0,                 // unsigned char AnalogSpeed;
      50,                // unsigned char SpeedLow;
      100,               // unsigned char SpeedHi;
      100,               // int PositionLowSpeed;
      0,                 // unsigned char PulseSystem;
      0,                 // unsigned char KierRegeling;
      0,                 // unsigned char AnalogInput;
      0,                 // unsigned char DigitalInput;
      1,                 // unsigned char AlarmAnaloog;
      0,                 // unsigned char AnalogOutput;
      150,               // int FrequentieVerstel;
      0,                 // unsigned char ClosedLoopOpenLoop; // 0 = Closed Loop; 1 = Open Loop
      100,               // unsigned char PowerFactor;        // Correctie factor voor vermogen ventilatoren [1.00]
      12,                // unsigned char RampUp;
      12,                // unsigned char RampDown;
      0,                 // unsigned char SensorType;
      100,               // unsigned char VentAtMax;  // uitsturing naar ventilatoren bij 100%
      0,                 // unsigned char WatchdogMode; // watchdog mode enabled bij RS485 sturing
      0,                 // unsigned char PositionAtCommunicationFailure; // positie bij een watchdog fault bij RS485 sturing
      {0,0},             // int Reserved[2];
      1800,              // int Runtime; // x100ms
      {0,0,0,0},         // s_board_IO_on_off DigInHiSpeed;
      {0,0,0,0},         // s_board_IO_on_off DigInOpen;
      {0,0,0,0},         // s_board_IO_on_off DigInClose;
      {0,0,0,0},         // s_board_IO_on_off AnaOutPosition;
      {0,0,0,0},         // s_board_IO_on_off DigOutAlarm;
      {0,0,0,0},         // s_board_IO_on_off AnaInPosition;
      {0,0,0,0},         // s_board_IO_on_off DigOutAlarmFlap;
    },
    { // Motorgroup[19]
      0,                 // unsigned char Enabled;
      19,                // unsigned char Number;
      0,                 // unsigned char Type;
      0,                 // unsigned char NumberMotors;
      0,                 // unsigned char BusType;
      0,                 // unsigned char FirstNumber;
      0,                 // unsigned char FirstAddress;
      0,                 // unsigned char ControlType;
      0,                 // unsigned char ControlIndex;
      0,                 // unsigned char FrequencyControlled;
      0,                 // unsigned char DispHiSpeedInput;
      0,                 // unsigned char AnalogSpeed;
      50,                // unsigned char SpeedLow;
      100,               // unsigned char SpeedHi;
      100,               // int PositionLowSpeed;
      0,                 // unsigned char PulseSystem;
      0,                 // unsigned char KierRegeling;
      0,                 // unsigned char AnalogInput;
      0,                 // unsigned char DigitalInput;
      1,                 // unsigned char AlarmAnaloog;
      0,                 // unsigned char AnalogOutput;
      150,               // int FrequentieVerstel;
      0,                 // unsigned char ClosedLoopOpenLoop; // 0 = Closed Loop; 1 = Open Loop
      100,               // unsigned char PowerFactor;        // Correctie factor voor vermogen ventilatoren [1.00]
      12,                // unsigned char RampUp;
      12,                // unsigned char RampDown;
      0,                 // unsigned char SensorType;
      100,               // unsigned char VentAtMax;  // uitsturing naar ventilatoren bij 100%
      0,                 // unsigned char WatchdogMode; // watchdog mode enabled bij RS485 sturing
      0,                 // unsigned char PositionAtCommunicationFailure; // positie bij een watchdog fault bij RS485 sturing
      {0,0},             // int Reserved[2];
      1800,              // int Runtime; // x100ms
      {0,0,0,0},         // s_board_IO_on_off DigInHiSpeed;
      {0,0,0,0},         // s_board_IO_on_off DigInOpen;
      {0,0,0,0},         // s_board_IO_on_off DigInClose;
      {0,0,0,0},         // s_board_IO_on_off AnaOutPosition;
      {0,0,0,0},         // s_board_IO_on_off DigOutAlarm;
      {0,0,0,0},         // s_board_IO_on_off AnaInPosition;
      {0,0,0,0},         // s_board_IO_on_off DigOutAlarmFlap;
    },
    { // Motorgroup[20]
      0,                 // unsigned char Enabled;
      20,                // unsigned char Number;
      0,                 // unsigned char Type;
      0,                 // unsigned char NumberMotors;
      0,                 // unsigned char BusType;
      0,                 // unsigned char FirstNumber;
      0,                 // unsigned char FirstAddress;
      0,                 // unsigned char ControlType;
      0,                 // unsigned char ControlIndex;
      0,                 // unsigned char FrequencyControlled;
      0,                 // unsigned char DispHiSpeedInput;
      0,                 // unsigned char AnalogSpeed;
      50,                // unsigned char SpeedLow;
      100,               // unsigned char SpeedHi;
      100,               // int PositionLowSpeed;
      0,                 // unsigned char PulseSystem;
      0,                 // unsigned char KierRegeling;
      0,                 // unsigned char AnalogInput;
      0,                 // unsigned char DigitalInput;
      1,                 // unsigned char AlarmAnaloog;
      0,                 // unsigned char AnalogOutput;
      150,               // int FrequentieVerstel;
      0,                 // unsigned char ClosedLoopOpenLoop; // 0 = Closed Loop; 1 = Open Loop
      100,               // unsigned char PowerFactor;        // Correctie factor voor vermogen ventilatoren [1.00]
      12,                // unsigned char RampUp;
      12,                // unsigned char RampDown;
      0,                 // unsigned char SensorType;
      100,               // unsigned char VentAtMax;  // uitsturing naar ventilatoren bij 100%
      0,                 // unsigned char WatchdogMode; // watchdog mode enabled bij RS485 sturing
      0,                 // unsigned char PositionAtCommunicationFailure; // positie bij een watchdog fault bij RS485 sturing
      {0,0},             // int Reserved[2];
      1800,              // int Runtime; // x100ms
      {0,0,0,0},         // s_board_IO_on_off DigInHiSpeed;
      {0,0,0,0},         // s_board_IO_on_off DigInOpen;
      {0,0,0,0},         // s_board_IO_on_off DigInClose;
      {0,0,0,0},         // s_board_IO_on_off AnaOutPosition;
      {0,0,0,0},         // s_board_IO_on_off DigOutAlarm;
      {0,0,0,0},         // s_board_IO_on_off AnaInPosition;
      {0,0,0,0},         // s_board_IO_on_off DigOutAlarmFlap;
    },
    { // Motorgroup[21]
      0,                 // unsigned char Enabled;
      21,                // unsigned char Number;
      0,                 // unsigned char Type;
      0,                 // unsigned char NumberMotors;
      0,                 // unsigned char BusType;
      0,                 // unsigned char FirstNumber;
      0,                 // unsigned char FirstAddress;
      0,                 // unsigned char ControlType;
      0,                 // unsigned char ControlIndex;
      0,                 // unsigned char FrequencyControlled;
      0,                 // unsigned char DispHiSpeedInput;
      0,                 // unsigned char AnalogSpeed;
      50,                // unsigned char SpeedLow;
      100,               // unsigned char SpeedHi;
      100,               // int PositionLowSpeed;
      0,                 // unsigned char PulseSystem;
      0,                 // unsigned char KierRegeling;
      0,                 // unsigned char AnalogInput;
      0,                 // unsigned char DigitalInput;
      1,                 // unsigned char AlarmAnaloog;
      0,                 // unsigned char AnalogOutput;
      150,               // int FrequentieVerstel;
      0,                 // unsigned char ClosedLoopOpenLoop; // 0 = Closed Loop; 1 = Open Loop
      100,               // unsigned char PowerFactor;        // Correctie factor voor vermogen ventilatoren [1.00]
      12,                // unsigned char RampUp;
      12,                // unsigned char RampDown;
      0,                 // unsigned char SensorType;
      100,               // unsigned char VentAtMax;  // uitsturing naar ventilatoren bij 100%
      0,                 // unsigned char WatchdogMode; // watchdog mode enabled bij RS485 sturing
      0,                 // unsigned char PositionAtCommunicationFailure; // positie bij een watchdog fault bij RS485 sturing
      {0,0},             // int Reserved[2];
      1800,              // int Runtime; // x100ms
      {0,0,0,0},         // s_board_IO_on_off DigInHiSpeed;
      {0,0,0,0},         // s_board_IO_on_off DigInOpen;
      {0,0,0,0},         // s_board_IO_on_off DigInClose;
      {0,0,0,0},         // s_board_IO_on_off AnaOutPosition;
      {0,0,0,0},         // s_board_IO_on_off DigOutAlarm;
      {0,0,0,0},         // s_board_IO_on_off AnaInPosition;
      {0,0,0,0},         // s_board_IO_on_off DigOutAlarmFlap;
    },
    { // Motorgroup[22]
      0,                 // unsigned char Enabled;
      22,                // unsigned char Number;
      0,                 // unsigned char Type;
      0,                 // unsigned char NumberMotors;
      0,                 // unsigned char BusType;
      0,                 // unsigned char FirstNumber;
      0,                 // unsigned char FirstAddress;
      0,                 // unsigned char ControlType;
      0,                 // unsigned char ControlIndex;
      0,                 // unsigned char FrequencyControlled;
      0,                 // unsigned char DispHiSpeedInput;
      0,                 // unsigned char AnalogSpeed;
      50,                // unsigned char SpeedLow;
      100,               // unsigned char SpeedHi;
      100,               // int PositionLowSpeed;
      0,                 // unsigned char PulseSystem;
      0,                 // unsigned char KierRegeling;
      0,                 // unsigned char AnalogInput;
      0,                 // unsigned char DigitalInput;
      1,                 // unsigned char AlarmAnaloog;
      0,                 // unsigned char AnalogOutput;
      150,               // int FrequentieVerstel;
      0,                 // unsigned char ClosedLoopOpenLoop; // 0 = Closed Loop; 1 = Open Loop
      100,               // unsigned char PowerFactor;        // Correctie factor voor vermogen ventilatoren [1.00]
      12,                // unsigned char RampUp;
      12,                // unsigned char RampDown;
      0,                 // unsigned char SensorType;
      100,               // unsigned char VentAtMax;  // uitsturing naar ventilatoren bij 100%
      0,                 // unsigned char WatchdogMode; // watchdog mode enabled bij RS485 sturing
      0,                 // unsigned char PositionAtCommunicationFailure; // positie bij een watchdog fault bij RS485 sturing
      {0,0},             // int Reserved[2];
      1800,              // int Runtime; // x100ms
      {0,0,0,0},         // s_board_IO_on_off DigInHiSpeed;
      {0,0,0,0},         // s_board_IO_on_off DigInOpen;
      {0,0,0,0},         // s_board_IO_on_off DigInClose;
      {0,0,0,0},         // s_board_IO_on_off AnaOutPosition;
      {0,0,0,0},         // s_board_IO_on_off DigOutAlarm;
      {0,0,0,0},         // s_board_IO_on_off AnaInPosition;
      {0,0,0,0},         // s_board_IO_on_off DigOutAlarmFlap;
    },
    { // Motorgroup[23]
      0,                 // unsigned char Enabled;
      23,                // unsigned char Number;
      0,                 // unsigned char Type;
      0,                 // unsigned char NumberMotors;
      0,                 // unsigned char BusType;
      0,                 // unsigned char FirstNumber;
      0,                 // unsigned char FirstAddress;
      0,                 // unsigned char ControlType;
      0,                 // unsigned char ControlIndex;
      0,                 // unsigned char DispHiSpeedInput;
      0,                 // unsigned char AnalogSpeed;
      0,                 // unsigned char FrequencyControlled;
      50,                // unsigned char SpeedLow;
      100,               // unsigned char SpeedHi;
      100,               // int PositionLowSpeed;
      0,                 // unsigned char PulseSystem;
      0,                 // unsigned char KierRegeling;
      0,                 // unsigned char AnalogInput;
      0,                 // unsigned char DigitalInput;
      1,                 // unsigned char AlarmAnaloog;
      0,                 // unsigned char AnalogOutput;
      150,               // int FrequentieVerstel;
      0,                 // unsigned char ClosedLoopOpenLoop; // 0 = Closed Loop; 1 = Open Loop
      100,               // unsigned char PowerFactor;        // Correctie factor voor vermogen ventilatoren [1.00]
      12,                // unsigned char RampUp;
      12,                // unsigned char RampDown;
      0,                 // unsigned char SensorType;
      100,               // unsigned char VentAtMax;  // uitsturing naar ventilatoren bij 100%
      0,                 // unsigned char WatchdogMode; // watchdog mode enabled bij RS485 sturing
      0,                 // unsigned char PositionAtCommunicationFailure; // positie bij een watchdog fault bij RS485 sturing
      {0,0},             // int Reserved[2];
      1800,              // int Runtime; // x100ms
      {0,0,0,0},         // s_board_IO_on_off DigInHiSpeed;
      {0,0,0,0},         // s_board_IO_on_off DigInOpen;
      {0,0,0,0},         // s_board_IO_on_off DigInClose;
      {0,0,0,0},         // s_board_IO_on_off AnaOutPosition;
      {0,0,0,0},         // s_board_IO_on_off DigOutAlarm;
      {0,0,0,0},         // s_board_IO_on_off AnaInPosition;
      {0,0,0,0},         // s_board_IO_on_off DigOutAlarmFlap;
    },
    { // Motorgroup[24]
      0,                 // unsigned char Enabled;
      24,                // unsigned char Number;
      0,                 // unsigned char Type;
      0,                 // unsigned char NumberMotors;
      0,                 // unsigned char BusType;
      0,                 // unsigned char FirstNumber;
      0,                 // unsigned char FirstAddress;
      0,                 // unsigned char ControlType;
      0,                 // unsigned char ControlIndex;
      0,                 // unsigned char FrequencyControlled;
      0,                 // unsigned char DispHiSpeedInput;
      0,                 // unsigned char AnalogSpeed;
      50,                // unsigned char SpeedLow;
      100,               // unsigned char SpeedHi;
      100,               // int PositionLowSpeed;
      0,                 // unsigned char PulseSystem;
      0,                 // unsigned char KierRegeling;
      0,                 // unsigned char AnalogInput;
      0,                 // unsigned char DigitalInput;
      1,                 // unsigned char AlarmAnaloog;
      0,                 // unsigned char AnalogOutput;
      150,               // int FrequentieVerstel;
      0,                 // unsigned char ClosedLoopOpenLoop; // 0 = Closed Loop; 1 = Open Loop
      100,               // unsigned char PowerFactor;        // Correctie factor voor vermogen ventilatoren [1.00]
      12,                // unsigned char RampUp;
      12,                // unsigned char RampDown;
      0,                 // unsigned char SensorType;
      100,               // unsigned char VentAtMax;  // uitsturing naar ventilatoren bij 100%
      0,                 // unsigned char WatchdogMode; // watchdog mode enabled bij RS485 sturing
      0,                 // unsigned char PositionAtCommunicationFailure; // positie bij een watchdog fault bij RS485 sturing
      {0,0},             // int Reserved[2];
      1800,              // int Runtime; // x100ms
      {0,0,0,0},         // s_board_IO_on_off DigInHiSpeed;
      {0,0,0,0},         // s_board_IO_on_off DigInOpen;
      {0,0,0,0},         // s_board_IO_on_off DigInClose;
      {0,0,0,0},         // s_board_IO_on_off AnaOutPosition;
      {0,0,0,0},         // s_board_IO_on_off DigOutAlarm;
      {0,0,0,0},         // s_board_IO_on_off AnaInPosition;
      {0,0,0,0},         // s_board_IO_on_off DigOutAlarmFlap;
    },
    { // Motorgroup[25]
      0,                 // unsigned char Enabled;
      25,                // unsigned char Number;
      0,                 // unsigned char Type;
      0,                 // unsigned char NumberMotors;
      0,                 // unsigned char BusType;
      0,                 // unsigned char FirstNumber;
      0,                 // unsigned char FirstAddress;
      0,                 // unsigned char ControlType;
      0,                 // unsigned char ControlIndex;
      0,                 // unsigned char FrequencyControlled;
      0,                 // unsigned char DispHiSpeedInput;
      0,                 // unsigned char AnalogSpeed;
      50,                // unsigned char SpeedLow;
      100,               // unsigned char SpeedHi;
      100,               // int PositionLowSpeed;
      0,                 // unsigned char PulseSystem;
      0,                 // unsigned char KierRegeling;
      0,                 // unsigned char AnalogInput;
      0,                 // unsigned char DigitalInput;
      1,                 // unsigned char AlarmAnaloog;
      0,                 // unsigned char AnalogOutput;
      150,               // int FrequentieVerstel;
      0,                 // unsigned char ClosedLoopOpenLoop; // 0 = Closed Loop; 1 = Open Loop
      100,               // unsigned char PowerFactor;        // Correctie factor voor vermogen ventilatoren [1.00]
      12,                // unsigned char RampUp;
      12,                // unsigned char RampDown;
      0,                 // unsigned char SensorType;
      100,               // unsigned char VentAtMax;  // uitsturing naar ventilatoren bij 100%
      0,                 // unsigned char WatchdogMode; // watchdog mode enabled bij RS485 sturing
      0,                 // unsigned char PositionAtCommunicationFailure; // positie bij een watchdog fault bij RS485 sturing
      {0,0},             // int Reserved[2];
      1800,              // int Runtime; // x100ms
      {0,0,0,0},         // s_board_IO_on_off DigInHiSpeed;
      {0,0,0,0},         // s_board_IO_on_off DigInOpen;
      {0,0,0,0},         // s_board_IO_on_off DigInClose;
      {0,0,0,0},         // s_board_IO_on_off AnaOutPosition;
      {0,0,0,0},         // s_board_IO_on_off DigOutAlarm;
      {0,0,0,0},         // s_board_IO_on_off AnaInPosition;
      {0,0,0,0},         // s_board_IO_on_off DigOutAlarmFlap;
    },
    { // Motorgroup[26]
      0,                 // unsigned char Enabled;
      26,                // unsigned char Number;
      0,                 // unsigned char Type;
      0,                 // unsigned char NumberMotors;
      0,                 // unsigned char BusType;
      0,                 // unsigned char FirstNumber;
      0,                 // unsigned char FirstAddress;
      0,                 // unsigned char ControlType;
      0,                 // unsigned char ControlIndex;
      0,                 // unsigned char FrequencyControlled;
      0,                 // unsigned char DispHiSpeedInput;
      0,                 // unsigned char AnalogSpeed;
      50,                // unsigned char SpeedLow;
      100,               // unsigned char SpeedHi;
      100,               // int PositionLowSpeed;
      0,                 // unsigned char PulseSystem;
      0,                 // unsigned char KierRegeling;
      0,                 // unsigned char AnalogInput;
      0,                 // unsigned char DigitalInput;
      1,                 // unsigned char AlarmAnaloog;
      0,                 // unsigned char AnalogOutput;
      150,               // int FrequentieVerstel;
      0,                 // unsigned char ClosedLoopOpenLoop; // 0 = Closed Loop; 1 = Open Loop
      100,               // unsigned char PowerFactor;        // Correctie factor voor vermogen ventilatoren [1.00]
      12,                // unsigned char RampUp;
      12,                // unsigned char RampDown;
      0,                 // unsigned char SensorType;
      100,               // unsigned char VentAtMax;  // uitsturing naar ventilatoren bij 100%
      0,                 // unsigned char WatchdogMode; // watchdog mode enabled bij RS485 sturing
      0,                 // unsigned char PositionAtCommunicationFailure; // positie bij een watchdog fault bij RS485 sturing
      {0,0},             // int Reserved[2];
      1800,              // int Runtime; // x100ms
      {0,0,0,0},         // s_board_IO_on_off DigInHiSpeed;
      {0,0,0,0},         // s_board_IO_on_off DigInOpen;
      {0,0,0,0},         // s_board_IO_on_off DigInClose;
      {0,0,0,0},         // s_board_IO_on_off AnaOutPosition;
      {0,0,0,0},         // s_board_IO_on_off DigOutAlarm;
      {0,0,0,0},         // s_board_IO_on_off AnaInPosition;
      {0,0,0,0},         // s_board_IO_on_off DigOutAlarmFlap;
    },
    { // Motorgroup[27]
      0,                 // unsigned char Enabled;
      27,                // unsigned char Number;
      0,                 // unsigned char Type;
      0,                 // unsigned char NumberMotors;
      0,                 // unsigned char BusType;
      0,                 // unsigned char FirstNumber;
      0,                 // unsigned char FirstAddress;
      0,                 // unsigned char ControlType;
      0,                 // unsigned char ControlIndex;
      0,                 // unsigned char FrequencyControlled;
      0,                 // unsigned char DispHiSpeedInput;
      0,                 // unsigned char AnalogSpeed;
      50,                // unsigned char SpeedLow;
      100,               // unsigned char SpeedHi;
      100,               // int PositionLowSpeed;
      0,                 // unsigned char PulseSystem;
      0,                 // unsigned char KierRegeling;
      0,                 // unsigned char AnalogInput;
      0,                 // unsigned char DigitalInput;
      1,                 // unsigned char AlarmAnaloog;
      0,                 // unsigned char AnalogOutput;
      150,               // int FrequentieVerstel;
      0,                 // unsigned char ClosedLoopOpenLoop; // 0 = Closed Loop; 1 = Open Loop
      100,               // unsigned char PowerFactor;        // Correctie factor voor vermogen ventilatoren [1.00]
      12,                // unsigned char RampUp;
      12,                // unsigned char RampDown;
      0,                 // unsigned char SensorType;
      100,               // unsigned char VentAtMax;  // uitsturing naar ventilatoren bij 100%
      0,                 // unsigned char WatchdogMode; // watchdog mode enabled bij RS485 sturing
      0,                 // unsigned char PositionAtCommunicationFailure; // positie bij een watchdog fault bij RS485 sturing
      {0,0},             // int Reserved[2];
      1800,              // int Runtime; // x100ms
      {0,0,0,0},         // s_board_IO_on_off DigInHiSpeed;
      {0,0,0,0},         // s_board_IO_on_off DigInOpen;
      {0,0,0,0},         // s_board_IO_on_off DigInClose;
      {0,0,0,0},         // s_board_IO_on_off AnaOutPosition;
      {0,0,0,0},         // s_board_IO_on_off DigOutAlarm;
      {0,0,0,0},         // s_board_IO_on_off AnaInPosition;
      {0,0,0,0},         // s_board_IO_on_off DigOutAlarmFlap;
    },
    { // Motorgroup[28]
      0,                 // unsigned char Enabled;
      28,                // unsigned char Number;
      0,                 // unsigned char Type;
      0,                 // unsigned char NumberMotors;
      0,                 // unsigned char BusType;
      0,                 // unsigned char FirstNumber;
      0,                 // unsigned char FirstAddress;
      0,                 // unsigned char ControlType;
      0,                 // unsigned char ControlIndex;
      0,                 // unsigned char FrequencyControlled;
      0,                 // unsigned char DispHiSpeedInput;
      0,                 // unsigned char AnalogSpeed;
      50,                // unsigned char SpeedLow;
      100,               // unsigned char SpeedHi;
      100,               // int PositionLowSpeed;
      0,                 // unsigned char PulseSystem;
      0,                 // unsigned char KierRegeling;
      0,                 // unsigned char AnalogInput;
      0,                 // unsigned char DigitalInput;
      1,                 // unsigned char AlarmAnaloog;
      0,                 // unsigned char AnalogOutput;
      150,               // int FrequentieVerstel;
      0,                 // unsigned char ClosedLoopOpenLoop; // 0 = Closed Loop; 1 = Open Loop
      100,               // unsigned char PowerFactor;        // Correctie factor voor vermogen ventilatoren [1.00]
      12,                // unsigned char RampUp;
      12,                // unsigned char RampDown;
      0,                 // unsigned char SensorType;
      100,               // unsigned char VentAtMax;  // uitsturing naar ventilatoren bij 100%
      0,                 // unsigned char WatchdogMode; // watchdog mode enabled bij RS485 sturing
      0,                 // unsigned char PositionAtCommunicationFailure; // positie bij een watchdog fault bij RS485 sturing
      {0,0},             // int Reserved[2];
      1800,              // int Runtime; // x100ms
      {0,0,0,0},         // s_board_IO_on_off DigInHiSpeed;
      {0,0,0,0},         // s_board_IO_on_off DigInOpen;
      {0,0,0,0},         // s_board_IO_on_off DigInClose;
      {0,0,0,0},         // s_board_IO_on_off AnaOutPosition;
      {0,0,0,0},         // s_board_IO_on_off DigOutAlarm;
      {0,0,0,0},         // s_board_IO_on_off AnaInPosition;
      {0,0,0,0},         // s_board_IO_on_off DigOutAlarmFlap;
    },
    { // Motorgroup[29]
      0,                 // unsigned char Enabled;
      29,                // unsigned char Number;
      0,                 // unsigned char Type;
      0,                 // unsigned char NumberMotors;
      0,                 // unsigned char BusType;
      0,                 // unsigned char FirstNumber;
      0,                 // unsigned char FirstAddress;
      0,                 // unsigned char ControlType;
      0,                 // unsigned char ControlIndex;
      0,                 // unsigned char FrequencyControlled;
      0,                 // unsigned char DispHiSpeedInput;
      0,                 // unsigned char AnalogSpeed;
      50,                // unsigned char SpeedLow;
      100,               // unsigned char SpeedHi;
      100,               // int PositionLowSpeed;
      0,                 // unsigned char PulseSystem;
      0,                 // unsigned char KierRegeling;
      0,                 // unsigned char AnalogInput;
      0,                 // unsigned char DigitalInput;
      1,                 // unsigned char AlarmAnaloog;
      0,                 // unsigned char AnalogOutput;
      150,               // int FrequentieVerstel;
      0,                 // unsigned char ClosedLoopOpenLoop; // 0 = Closed Loop; 1 = Open Loop
      100,               // unsigned char PowerFactor;        // Correctie factor voor vermogen ventilatoren [1.00]
      12,                // unsigned char RampUp;
      12,                // unsigned char RampDown;
      0,                 // unsigned char SensorType;
      100,               // unsigned char VentAtMax;  // uitsturing naar ventilatoren bij 100%
      0,                 // unsigned char WatchdogMode; // watchdog mode enabled bij RS485 sturing
      0,                 // unsigned char PositionAtCommunicationFailure; // positie bij een watchdog fault bij RS485 sturing
      {0,0},             // int Reserved[2];
      1800,              // int Runtime; // x100ms
      {0,0,0,0},         // s_board_IO_on_off DigInHiSpeed;
      {0,0,0,0},         // s_board_IO_on_off DigInOpen;
      {0,0,0,0},         // s_board_IO_on_off DigInClose;
      {0,0,0,0},         // s_board_IO_on_off AnaOutPosition;
      {0,0,0,0},         // s_board_IO_on_off DigOutAlarm;
      {0,0,0,0},         // s_board_IO_on_off AnaInPosition;
      {0,0,0,0},         // s_board_IO_on_off DigOutAlarmFlap;
    },
    { // Motorgroup[30]
      0,                 // unsigned char Enabled;
      30,                // unsigned char Number;
      0,                 // unsigned char Type;
      0,                 // unsigned char NumberMotors;
      0,                 // unsigned char BusType;
      0,                 // unsigned char FirstNumber;
      0,                 // unsigned char FirstAddress;
      0,                 // unsigned char ControlType;
      0,                 // unsigned char ControlIndex;
      0,                 // unsigned char FrequencyControlled;
      0,                 // unsigned char DispHiSpeedInput;
      0,                 // unsigned char AnalogSpeed;
      50,                // unsigned char SpeedLow;
      100,               // unsigned char SpeedHi;
      100,               // int PositionLowSpeed;
      0,                 // unsigned char PulseSystem;
      0,                 // unsigned char KierRegeling;
      0,                 // unsigned char AnalogInput;
      0,                 // unsigned char DigitalInput;
      1,                 // unsigned char AlarmAnaloog;
      0,                 // unsigned char AnalogOutput;
      150,               // int FrequentieVerstel;
      0,                 // unsigned char ClosedLoopOpenLoop; // 0 = Closed Loop; 1 = Open Loop
      100,               // unsigned char PowerFactor;        // Correctie factor voor vermogen ventilatoren [1.00]
      12,                // unsigned char RampUp;
      12,                // unsigned char RampDown;
      0,                 // unsigned char SensorType;
      100,               // unsigned char VentAtMax;  // uitsturing naar ventilatoren bij 100%
      0,                 // unsigned char WatchdogMode; // watchdog mode enabled bij RS485 sturing
      0,                 // unsigned char PositionAtCommunicationFailure; // positie bij een watchdog fault bij RS485 sturing
      {0,0},             // int Reserved[2];
      1800,              // int Runtime; // x100ms
      {0,0,0,0},         // s_board_IO_on_off DigInHiSpeed;
      {0,0,0,0},         // s_board_IO_on_off DigInOpen;
      {0,0,0,0},         // s_board_IO_on_off DigInClose;
      {0,0,0,0},         // s_board_IO_on_off AnaOutPosition;
      {0,0,0,0},         // s_board_IO_on_off DigOutAlarm;
      {0,0,0,0},         // s_board_IO_on_off AnaInPosition;
      {0,0,0,0},         // s_board_IO_on_off DigOutAlarmFlap;
    },
    { // Motorgroup[31]
      0,                 // unsigned char Enabled;
      31,                // unsigned char Number;
      0,                 // unsigned char Type;
      0,                 // unsigned char NumberMotors;
      0,                 // unsigned char BusType;
      0,                 // unsigned char FirstNumber;
      0,                 // unsigned char FirstAddress;
      0,                 // unsigned char ControlType;
      0,                 // unsigned char ControlIndex;
      0,                 // unsigned char FrequencyControlled;
      0,                 // unsigned char DispHiSpeedInput;
      0,                 // unsigned char AnalogSpeed;
      50,                // unsigned char SpeedLow;
      100,               // unsigned char SpeedHi;
      100,               // int PositionLowSpeed;
      0,                 // unsigned char PulseSystem;
      0,                 // unsigned char KierRegeling;
      0,                 // unsigned char AnalogInput;
      0,                 // unsigned char DigitalInput;
      1,                 // unsigned char AlarmAnaloog;
      0,                 // unsigned char AnalogOutput;
      150,               // int FrequentieVerstel;
      0,                 // unsigned char ClosedLoopOpenLoop; // 0 = Closed Loop; 1 = Open Loop
      100,               // unsigned char PowerFactor;        // Correctie factor voor vermogen ventilatoren [1.00]
      12,                // unsigned char RampUp;
      12,                // unsigned char RampDown;
      0,                 // unsigned char SensorType;
      100,               // unsigned char VentAtMax;  // uitsturing naar ventilatoren bij 100%
      0,                 // unsigned char WatchdogMode; // watchdog mode enabled bij RS485 sturing
      0,                 // unsigned char PositionAtCommunicationFailure; // positie bij een watchdog fault bij RS485 sturing
      {0,0},             // int Reserved[2];
      1800,              // int Runtime; // x100ms
      {0,0,0,0},         // s_board_IO_on_off DigInHiSpeed;
      {0,0,0,0},         // s_board_IO_on_off DigInOpen;
      {0,0,0,0},         // s_board_IO_on_off DigInClose;
      {0,0,0,0},         // s_board_IO_on_off AnaOutPosition;
      {0,0,0,0},         // s_board_IO_on_off DigOutAlarm;
      {0,0,0,0},         // s_board_IO_on_off AnaInPosition;
      {0,0,0,0},         // s_board_IO_on_off DigOutAlarmFlap;
    },
  },

  { // sOptMotor Motor[MAX_MOTOR];
    { // Motor[0]
      0,         // unsigned char Enabled;
      0,         // unsigned char Number;
      0,         // unsigned char GroupNumber;
      0,         // unsigned char ControlType;
      -1,        // int Link;
      1800,      // int Runtime; // x100ms
      {0,0,0,0}, // s_board_IO_on_off IO;
    },
    { // Motor[1]
      0,         // unsigned char Enabled;
      1,         // unsigned char Number;
      0,         // unsigned char GroupNumber;
      0,         // unsigned char ControlType;
      -1,        // int Link;
      1800,      // int Runtime; // x100ms
      {0,0,0,0}, // s_board_IO_on_off IO;
    },
    { // Motor[2]
      0,         // unsigned char Enabled;
      2,         // unsigned char Number;
      0,         // unsigned char GroupNumber;
      0,         // unsigned char ControlType;
      -1,        // int Link;
      1800,      // int Runtime; // x100ms
      {0,0,0,0}, // s_board_IO_on_off IO;
    },
    { // Motor[3]
      0,         // unsigned char Enabled;
      3,         // unsigned char Number;
      0,         // unsigned char GroupNumber;
      0,         // unsigned char ControlType;
      -1,        // int Link;
      1800,      // int Runtime; // x100ms
      {0,0,0,0}, // s_board_IO_on_off IO;
    },
    { // Motor[4]
      0,         // unsigned char Enabled;
      4,         // unsigned char Number;
      0,         // unsigned char GroupNumber;
      0,         // unsigned char ControlType;
      -1,        // int Link;
      1800,      // int Runtime; // x100ms
      {0,0,0,0}, // s_board_IO_on_off IO;
    },
    { // Motor[5]
      0,         // unsigned char Enabled;
      5,         // unsigned char Number;
      0,         // unsigned char GroupNumber;
      0,         // unsigned char ControlType;
      -1,        // int Link;
      1800,      // int Runtime; // x100ms
      {0,0,0,0}, // s_board_IO_on_off IO;
    },
    { // Motor[6]
      0,         // unsigned char Enabled;
      6,         // unsigned char Number;
      0,         // unsigned char GroupNumber;
      0,         // unsigned char ControlType;
      -1,        // int Link;
      1800,      // int Runtime; // x100ms
      {0,0,0,0}, // s_board_IO_on_off IO;
    },
    { // Motor[7]
      0,         // unsigned char Enabled;
      7,         // unsigned char Number;
      0,         // unsigned char GroupNumber;
      0,         // unsigned char ControlType;
      -1,        // int Link;
      1800,      // int Runtime; // x100ms
      {0,0,0,0}, // s_board_IO_on_off IO;
    },
    { // Motor[8]
      0,         // unsigned char Enabled;
      8,         // unsigned char Number;
      0,         // unsigned char GroupNumber;
      0,         // unsigned char ControlType;
      -1,        // int Link;
      1800,      // int Runtime; // x100ms
      {0,0,0,0}, // s_board_IO_on_off IO;
    },
    { // Motor[9]
      0,         // unsigned char Enabled;
      9,         // unsigned char Number;
      0,         // unsigned char GroupNumber;
      0,         // unsigned char ControlType;
      -1,        // int Link;
      1800,      // int Runtime; // x100ms
      {0,0,0,0}, // s_board_IO_on_off IO;
    },
    { // Motor[10]
      0,         // unsigned char Enabled;
      10,        // unsigned char Number;
      0,         // unsigned char GroupNumber;
      0,         // unsigned char ControlType;
      -1,        // int Link;
      1800,      // int Runtime; // x100ms
      {0,0,0,0}, // s_board_IO_on_off IO;
    },
    { // Motor[11]
      0,         // unsigned char Enabled;
      11,        // unsigned char Number;
      0,         // unsigned char GroupNumber;
      0,         // unsigned char ControlType;
      -1,        // int Link;
      1800,      // int Runtime; // x100ms
      {0,0,0,0}, // s_board_IO_on_off IO;
    },
    { // Motor[12]
      0,         // unsigned char Enabled;
      12,        // unsigned char Number;
      0,         // unsigned char GroupNumber;
      0,         // unsigned char ControlType;
      -1,        // int Link;
      1800,      // int Runtime; // x100ms
      {0,0,0,0}, // s_board_IO_on_off IO;
    },
    { // Motor[13]
      0,         // unsigned char Enabled;
      13,        // unsigned char Number;
      0,         // unsigned char GroupNumber;
      0,         // unsigned char ControlType;
      -1,        // int Link;
      1800,      // int Runtime; // x100ms
      {0,0,0,0}, // s_board_IO_on_off IO;
    },
    { // Motor[14]
      0,         // unsigned char Enabled;
      14,        // unsigned char Number;
      0,         // unsigned char GroupNumber;
      0,         // unsigned char ControlType;
      -1,        // int Link;
      1800,      // int Runtime; // x100ms
      {0,0,0,0}, // s_board_IO_on_off IO;
    },
    { // Motor[15]
      0,         // unsigned char Enabled;
      15,        // unsigned char Number;
      0,         // unsigned char GroupNumber;
      0,         // unsigned char ControlType;
      -1,        // int Link;
      1800,      // int Runtime; // x100ms
      {0,0,0,0}, // s_board_IO_on_off IO;
    },
    { // Motor[16]
      0,         // unsigned char Enabled;
      16,        // unsigned char Number;
      0,         // unsigned char GroupNumber;
      0,         // unsigned char ControlType;
      -1,        // int Link;
      1800,      // int Runtime; // x100ms
      {0,0,0,0}, // s_board_IO_on_off IO;
    },
    { // Motor[17]
      0,         // unsigned char Enabled;
      17,        // unsigned char Number;
      0,         // unsigned char GroupNumber;
      0,         // unsigned char ControlType;
      -1,        // int Link;
      1800,      // int Runtime; // x100ms
      {0,0,0,0}, // s_board_IO_on_off IO;
    },
    { // Motor[18]
      0,         // unsigned char Enabled;
      18,        // unsigned char Number;
      0,         // unsigned char GroupNumber;
      0,         // unsigned char ControlType;
      -1,        // int Link;
      1800,      // int Runtime; // x100ms
      {0,0,0,0}, // s_board_IO_on_off IO;
    },
    { // Motor[19]
      0,         // unsigned char Enabled;
      19,        // unsigned char Number;
      0,         // unsigned char GroupNumber;
      0,         // unsigned char ControlType;
      -1,        // int Link;
      1800,      // int Runtime; // x100ms
      {0,0,0,0}, // s_board_IO_on_off IO;
    },
    { // Motor[20]
      0,         // unsigned char Enabled;
      20,        // unsigned char Number;
      0,         // unsigned char GroupNumber;
      0,         // unsigned char ControlType;
      -1,        // int Link;
      1800,      // int Runtime; // x100ms
      {0,0,0,0}, // s_board_IO_on_off IO;
    },
    { // Motor[21]
      0,         // unsigned char Enabled;
      21,        // unsigned char Number;
      0,         // unsigned char GroupNumber;
      0,         // unsigned char ControlType;
      -1,        // int Link;
      1800,      // int Runtime; // x100ms
      {0,0,0,0}, // s_board_IO_on_off IO;
    },
    { // Motor[22]
      0,         // unsigned char Enabled;
      22,        // unsigned char Number;
      0,         // unsigned char GroupNumber;
      0,         // unsigned char ControlType;
      -1,        // int Link;
      1800,      // int Runtime; // x100ms
      {0,0,0,0}, // s_board_IO_on_off IO;
    },
    { // Motor[23]
      0,         // unsigned char Enabled;
      23,        // unsigned char Number;
      0,         // unsigned char GroupNumber;
      0,         // unsigned char ControlType;
      -1,        // int Link;
      1800,      // int Runtime; // x100ms
      {0,0,0,0}, // s_board_IO_on_off IO;
    },
    { // Motor[24]
      0,         // unsigned char Enabled;
      24,        // unsigned char Number;
      0,         // unsigned char GroupNumber;
      0,         // unsigned char ControlType;
      -1,        // int Link;
      1800,      // int Runtime; // x100ms
      {0,0,0,0}, // s_board_IO_on_off IO;
    },
    { // Motor[25]
      0,         // unsigned char Enabled;
      25,        // unsigned char Number;
      0,         // unsigned char GroupNumber;
      0,         // unsigned char ControlType;
      -1,        // int Link;
      1800,      // int Runtime; // x100ms
      {0,0,0,0}, // s_board_IO_on_off IO;
    },
    { // Motor[26]
      0,         // unsigned char Enabled;
      26,        // unsigned char Number;
      0,         // unsigned char GroupNumber;
      0,         // unsigned char ControlType;
      -1,        // int Link;
      1800,      // int Runtime; // x100ms
      {0,0,0,0}, // s_board_IO_on_off IO;
    },
    { // Motor[27]
      0,         // unsigned char Enabled;
      27,        // unsigned char Number;
      0,         // unsigned char GroupNumber;
      0,         // unsigned char ControlType;
      -1,        // int Link;
      1800,      // int Runtime; // x100ms
      {0,0,0,0}, // s_board_IO_on_off IO;
    },
    { // Motor[28]
      0,         // unsigned char Enabled;
      28,        // unsigned char Number;
      0,         // unsigned char GroupNumber;
      0,         // unsigned char ControlType;
      -1,        // int Link;
      1800,      // int Runtime; // x100ms
      {0,0,0,0}, // s_board_IO_on_off IO;
    },
    { // Motor[29]
      0,         // unsigned char Enabled;
      29,        // unsigned char Number;
      0,         // unsigned char GroupNumber;
      0,         // unsigned char ControlType;
      -1,        // int Link;
      1800,      // int Runtime; // x100ms
      {0,0,0,0}, // s_board_IO_on_off IO;
    },
    { // Motor[30]
      0,         // unsigned char Enabled;
      30,        // unsigned char Number;
      0,         // unsigned char GroupNumber;
      0,         // unsigned char ControlType;
      -1,        // int Link;
      1800,      // int Runtime; // x100ms
      {0,0,0,0}, // s_board_IO_on_off IO;
    },
    { // Motor[31]
      0,         // unsigned char Enabled;
      31,        // unsigned char Number;
      0,         // unsigned char GroupNumber;
      0,         // unsigned char ControlType;
      -1,        // int Link;
      1800,      // int Runtime; // x100ms
      {0,0,0,0}, // s_board_IO_on_off IO;
    },
    { // Motor[32]
      0,         // unsigned char Enabled;
      32,        // unsigned char Number;
      0,         // unsigned char GroupNumber;
      0,         // unsigned char ControlType;
      -1,        // int Link;
      1800,      // int Runtime; // x100ms
      {0,0,0,0}, // s_board_IO_on_off IO;
    },
    { // Motor[33]
      0,         // unsigned char Enabled;
      33,        // unsigned char Number;
      0,         // unsigned char GroupNumber;
      0,         // unsigned char ControlType;
      -1,        // int Link;
      1800,      // int Runtime; // x100ms
      {0,0,0,0}, // s_board_IO_on_off IO;
    },
    { // Motor[34]
      0,         // unsigned char Enabled;
      34,        // unsigned char Number;
      0,         // unsigned char GroupNumber;
      0,         // unsigned char ControlType;
      -1,        // int Link;
      1800,      // int Runtime; // x100ms
      {0,0,0,0}, // s_board_IO_on_off IO;
    },
    { // Motor[35]
      0,         // unsigned char Enabled;
      35,        // unsigned char Number;
      0,         // unsigned char GroupNumber;
      0,         // unsigned char ControlType;
      -1,        // int Link;
      1800,      // int Runtime; // x100ms
      {0,0,0,0}, // s_board_IO_on_off IO;
    },
    { // Motor[36]
      0,         // unsigned char Enabled;
      36,        // unsigned char Number;
      0,         // unsigned char GroupNumber;
      0,         // unsigned char ControlType;
      -1,        // int Link;
      1800,      // int Runtime; // x100ms
      {0,0,0,0}, // s_board_IO_on_off IO;
    },
    { // Motor[37]
      0,         // unsigned char Enabled;
      37,        // unsigned char Number;
      0,         // unsigned char GroupNumber;
      0,         // unsigned char ControlType;
      -1,        // int Link;
      1800,      // int Runtime; // x100ms
      {0,0,0,0}, // s_board_IO_on_off IO;
    },
    { // Motor[38]
      0,         // unsigned char Enabled;
      38,        // unsigned char Number;
      0,         // unsigned char GroupNumber;
      0,         // unsigned char ControlType;
      -1,        // int Link;
      1800,      // int Runtime; // x100ms
      {0,0,0,0}, // s_board_IO_on_off IO;
    },
    { // Motor[39]
      0,         // unsigned char Enabled;
      39,        // unsigned char Number;
      0,         // unsigned char GroupNumber;
      0,         // unsigned char ControlType;
      -1,        // int Link;
      1800,      // int Runtime; // x100ms
      {0,0,0,0}, // s_board_IO_on_off IO;
    },
    { // Motor[40]
      0,         // unsigned char Enabled;
      40,        // unsigned char Number;
      0,         // unsigned char GroupNumber;
      0,         // unsigned char ControlType;
      -1,        // int Link;
      1800,      // int Runtime; // x100ms
      {0,0,0,0}, // s_board_IO_on_off IO;
    },
    { // Motor[41]
      0,         // unsigned char Enabled;
      41,        // unsigned char Number;
      0,         // unsigned char GroupNumber;
      0,         // unsigned char ControlType;
      -1,        // int Link;
      1800,      // int Runtime; // x100ms
      {0,0,0,0}, // s_board_IO_on_off IO;
    },
    { // Motor[42]
      0,         // unsigned char Enabled;
      42,        // unsigned char Number;
      0,         // unsigned char GroupNumber;
      0,         // unsigned char ControlType;
      -1,        // int Link;
      1800,      // int Runtime; // x100ms
      {0,0,0,0}, // s_board_IO_on_off IO;
    },
    { // Motor[43]
      0,         // unsigned char Enabled;
      43,        // unsigned char Number;
      0,         // unsigned char GroupNumber;
      0,         // unsigned char ControlType;
      -1,        // int Link;
      1800,      // int Runtime; // x100ms
      {0,0,0,0}, // s_board_IO_on_off IO;
    },
    { // Motor[44]
      0,         // unsigned char Enabled;
      44,        // unsigned char Number;
      0,         // unsigned char GroupNumber;
      0,         // unsigned char ControlType;
      -1,        // int Link;
      1800,      // int Runtime; // x100ms
      {0,0,0,0}, // s_board_IO_on_off IO;
    },
    { // Motor[45]
      0,         // unsigned char Enabled;
      45,        // unsigned char Number;
      0,         // unsigned char GroupNumber;
      0,         // unsigned char ControlType;
      -1,        // int Link;
      1800,      // int Runtime; // x100ms
      {0,0,0,0}, // s_board_IO_on_off IO;
    },
    { // Motor[46]
      0,         // unsigned char Enabled;
      46,        // unsigned char Number;
      0,         // unsigned char GroupNumber;
      0,         // unsigned char ControlType;
      -1,        // int Link;
      1800,      // int Runtime; // x100ms
      {0,0,0,0}, // s_board_IO_on_off IO;
    },
    { // Motor[47]
      0,         // unsigned char Enabled;
      47,        // unsigned char Number;
      0,         // unsigned char GroupNumber;
      0,         // unsigned char ControlType;
      -1,        // int Link;
      1800,      // int Runtime; // x100ms
      {0,0,0,0}, // s_board_IO_on_off IO;
    },
    { // Motor[48]
      0,         // unsigned char Enabled;
      48,        // unsigned char Number;
      0,         // unsigned char GroupNumber;
      0,         // unsigned char ControlType;
      -1,        // int Link;
      1800,      // int Runtime; // x100ms
      {0,0,0,0}, // s_board_IO_on_off IO;
    },
    { // Motor[49]
      0,         // unsigned char Enabled;
      49,        // unsigned char Number;
      0,         // unsigned char GroupNumber;
      0,         // unsigned char ControlType;
      -1,        // int Link;
      1800,      // int Runtime; // x100ms
      {0,0,0,0}, // s_board_IO_on_off IO;
    },
    { // Motor[50]
      0,         // unsigned char Enabled;
      50,        // unsigned char Number;
      0,         // unsigned char GroupNumber;
      0,         // unsigned char ControlType;
      -1,        // int Link;
      1800,      // int Runtime; // x100ms
      {0,0,0,0}, // s_board_IO_on_off IO;
    },
    { // Motor[51]
      0,         // unsigned char Enabled;
      51,        // unsigned char Number;
      0,         // unsigned char GroupNumber;
      0,         // unsigned char ControlType;
      -1,        // int Link;
      1800,      // int Runtime; // x100ms
      {0,0,0,0}, // s_board_IO_on_off IO;
    },
    { // Motor[52]
      0,         // unsigned char Enabled;
      52,        // unsigned char Number;
      0,         // unsigned char GroupNumber;
      0,         // unsigned char ControlType;
      -1,        // int Link;
      1800,      // int Runtime; // x100ms
      {0,0,0,0}, // s_board_IO_on_off IO;
    },
    { // Motor[53]
      0,         // unsigned char Enabled;
      53,        // unsigned char Number;
      0,         // unsigned char GroupNumber;
      0,         // unsigned char ControlType;
      -1,        // int Link;
      1800,      // int Runtime; // x100ms
      {0,0,0,0}, // s_board_IO_on_off IO;
    },
    { // Motor[54]
      0,         // unsigned char Enabled;
      54,        // unsigned char Number;
      0,         // unsigned char GroupNumber;
      0,         // unsigned char ControlType;
      -1,        // int Link;
      1800,      // int Runtime; // x100ms
      {0,0,0,0}, // s_board_IO_on_off IO;
    },
    { // Motor[55]
      0,         // unsigned char Enabled;
      55,        // unsigned char Number;
      0,         // unsigned char GroupNumber;
      0,         // unsigned char ControlType;
      -1,        // int Link;
      1800,      // int Runtime; // x100ms
      {0,0,0,0}, // s_board_IO_on_off IO;
    },
    { // Motor[56]
      0,         // unsigned char Enabled;
      56,        // unsigned char Number;
      0,         // unsigned char GroupNumber;
      0,         // unsigned char ControlType;
      -1,        // int Link;
      1800,      // int Runtime; // x100ms
      {0,0,0,0}, // s_board_IO_on_off IO;
    },
    { // Motor[57]
      0,         // unsigned char Enabled;
      57,        // unsigned char Number;
      0,         // unsigned char GroupNumber;
      0,         // unsigned char ControlType;
      -1,        // int Link;
      1800,      // int Runtime; // x100ms
      {0,0,0,0}, // s_board_IO_on_off IO;
    },
    { // Motor[58]
      0,         // unsigned char Enabled;
      58,        // unsigned char Number;
      0,         // unsigned char GroupNumber;
      0,         // unsigned char ControlType;
      -1,        // int Link;
      1800,      // int Runtime; // x100ms
      {0,0,0,0}, // s_board_IO_on_off IO;
    },
    { // Motor[59]
      0,         // unsigned char Enabled;
      59,        // unsigned char Number;
      0,         // unsigned char GroupNumber;
      0,         // unsigned char ControlType;
      -1,        // int Link;
      1800,      // int Runtime; // x100ms
      {0,0,0,0}, // s_board_IO_on_off IO;
    },
    { // Motor[60]
      0,         // unsigned char Enabled;
      60,        // unsigned char Number;
      0,         // unsigned char GroupNumber;
      0,         // unsigned char ControlType;
      -1,        // int Link;
      1800,      // int Runtime; // x100ms
      {0,0,0,0}, // s_board_IO_on_off IO;
    },
    { // Motor[61]
      0,         // unsigned char Enabled;
      61,        // unsigned char Number;
      0,         // unsigned char GroupNumber;
      0,         // unsigned char ControlType;
      -1,        // int Link;
      1800,      // int Runtime; // x100ms
      {0,0,0,0}, // s_board_IO_on_off IO;
    },
    { // Motor[62]
      0,         // unsigned char Enabled;
      62,        // unsigned char Number;
      0,         // unsigned char GroupNumber;
      0,         // unsigned char ControlType;
      -1,        // int Link;
      1800,      // int Runtime; // x100ms
      {0,0,0,0}, // s_board_IO_on_off IO;
    },
    { // Motor[63]
      0,         // unsigned char Enabled;
      63,        // unsigned char Number;
      0,         // unsigned char GroupNumber;
      0,         // unsigned char ControlType;
      -1,        // int Link;
      1800,      // int Runtime; // x100ms
      {0,0,0,0}, // s_board_IO_on_off IO;
    },
  },
  { // sOptDualScreen DualScreen[MAX_SCREEN];
    { // DualScreen[0]
      0,  // unsigned char Enabled;
      0,  // unsigned char Number;
      0,  // unsigned char Standby;
      0,  // unsigned char Master;     // 0 = Geen, 1 = A, 2 = B
      0,  // unsigned char CombiMatic; // doeken achter elkaar ipv tegenover elkaar
      0,  // unsigned char Absolute;   // 0 = relatief, 1 = absoluut (alleen bij CombiMatic)
      -1, // int GroupA;
      -1, // int GroupB;
    },
    { // DualScreen[1]
      0,  // unsigned char Enabled;
      1,  // unsigned char Number;
      0,  // unsigned char Standby;
      0,  // unsigned char Master;     // 0 = Geen, 1 = A, 2 = B
      0,  // unsigned char CombiMatic; // doeken achter elkaar ipv tegenover elkaar
      0,  // unsigned char Absolute;   // 0 = relatief, 1 = absoluut (alleen bij CombiMatic)
      -1, // int GroupA;
      -1, // int GroupB;
    },
    { // DualScreen[2]
      0,  // unsigned char Enabled;
      2,  // unsigned char Number;
      0,  // unsigned char Standby;
      0,  // unsigned char Master;     // 0 = Geen, 1 = A, 2 = B
      0,  // unsigned char CombiMatic; // doeken achter elkaar ipv tegenover elkaar
      0,  // unsigned char Absolute;   // 0 = relatief, 1 = absoluut (alleen bij CombiMatic)
      -1, // int GroupA;
      -1, // int GroupB;
    },
    { // DualScreen[3]
      0,  // unsigned char Enabled;
      3,  // unsigned char Number;
      0,  // unsigned char Standby;
      0,  // unsigned char Master;     // 0 = Geen, 1 = A, 2 = B
      0,  // unsigned char CombiMatic; // doeken achter elkaar ipv tegenover elkaar
      0,  // unsigned char Absolute;   // 0 = relatief, 1 = absoluut (alleen bij CombiMatic)
      -1, // int GroupA;
      -1, // int GroupB;
    },
    { // DualScreen[4]
      0,  // unsigned char Enabled;
      4,  // unsigned char Number;
      0,  // unsigned char Standby;
      0,  // unsigned char Master;     // 0 = Geen, 1 = A, 2 = B
      0,  // unsigned char CombiMatic; // doeken achter elkaar ipv tegenover elkaar
      0,  // unsigned char Absolute;   // 0 = relatief, 1 = absoluut (alleen bij CombiMatic)
      -1, // int GroupA;
      -1, // int GroupB;
    },
    { // DualScreen[5]
      0,  // unsigned char Enabled;
      5,  // unsigned char Number;
      0,  // unsigned char Standby;
      0,  // unsigned char Master;     // 0 = Geen, 1 = A, 2 = B
      0,  // unsigned char CombiMatic; // doeken achter elkaar ipv tegenover elkaar
      0,  // unsigned char Absolute;   // 0 = relatief, 1 = absoluut (alleen bij CombiMatic)
      -1, // int GroupA;
      -1, // int GroupB;
    },
    { // DualScreen[6]
      0,  // unsigned char Enabled;
      6,  // unsigned char Number;
      0,  // unsigned char Standby;
      0,  // unsigned char Master;     // 0 = Geen, 1 = A, 2 = B
      0,  // unsigned char CombiMatic; // doeken achter elkaar ipv tegenover elkaar
      0,  // unsigned char Absolute;   // 0 = relatief, 1 = absoluut (alleen bij CombiMatic)
      -1, // int GroupA;
      -1, // int GroupB;
    },
    { // DualScreen[7]
      0,  // unsigned char Enabled;
      7,  // unsigned char Number;
      0,  // unsigned char Standby;
      0,  // unsigned char Master;     // 0 = Geen, 1 = A, 2 = B
      0,  // unsigned char CombiMatic; // doeken achter elkaar ipv tegenover elkaar
      0,  // unsigned char Absolute;   // 0 = relatief, 1 = absoluut (alleen bij CombiMatic)
      -1, // int GroupA;
      -1, // int GroupB;
    },
  },

  { // sOptCabriokas Cabriokas[MAX_CABRIO];
    { // Cabriokas[0]
      0,  // unsigned char Enabled;
      0,  // unsigned char Type;
      50, // unsigned char Voorloop;
      10, // unsigned char Hysterese;
      -1, // int GroupA;
      -1, // int GroupB;
    },
    { // Cabriokas[1]
      0,  // unsigned char Enabled;
      0,  // unsigned char Type;
      50, // unsigned char Voorloop;
      10, // unsigned char Hysterese;
      -1, // int GroupA;
      -1, // int GroupB;
    },
    { // Cabriokas[2]
      0,  // unsigned char Enabled;
      0,  // unsigned char Type;
      50, // unsigned char Voorloop;
      10, // unsigned char Hysterese;
      -1, // int GroupA;
      -1, // int GroupB;
    },
    { // Cabriokas[3]
      0,  // unsigned char Enabled;
      0,  // unsigned char Type;
      50, // unsigned char Voorloop;
      10, // unsigned char Hysterese;
      -1, // int GroupA;
      -1, // int GroupB;
    },
    { // Cabriokas[4]
      0,  // unsigned char Enabled;
      0,  // unsigned char Type;
      50, // unsigned char Voorloop;
      10, // unsigned char Hysterese;
      -1, // int GroupA;
      -1, // int GroupB;
    },
    { // Cabriokas[5]
      0,  // unsigned char Enabled;
      0,  // unsigned char Type;
      50, // unsigned char Voorloop;
      10, // unsigned char Hysterese;
      -1, // int GroupA;
      -1, // int GroupB;
    },
    { // Cabriokas[6]
      0,  // unsigned char Enabled;
      0,  // unsigned char Type;
      50, // unsigned char Voorloop;
      10, // unsigned char Hysterese;
      -1, // int GroupA;
      -1, // int GroupB;
    },
    { // Cabriokas[7]
      0,  // unsigned char Enabled;
      0,  // unsigned char Type;
      50, // unsigned char Voorloop;
      10, // unsigned char Hysterese;
      -1, // int GroupA;
      -1, // int GroupB;
    },
  },

  { // sOptDevice Device[MAX_DEVICE];
    {0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}, //   1 ..  16
    {0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}, //  17 ..  32
    {0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}, //  33 ..  48
    {0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}, //  49 ..  64
    {0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}, //  65 ..  80
    {0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}, //  81 ..  96
    {0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}, //  97 .. 112
    {0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}, // 113 .. 128
    {0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}, // 129 .. 144
    {0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}, // 145 .. 160
    {0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}, // 161 .. 176
    {0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}, // 177 .. 192
    {0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}, // 193 .. 208
    {0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}, // 209 .. 224
    {0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}, // 225 .. 240
    {0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}, // 241 .. 256
  },

  { // sOptVentgroup Ventgroup[MAX_GROUP];
    { // Ventgroup[0]
      {0,0,0,0},                                 // s_board_IO_on_off DigInOnOff;
      {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}}, // s_board_IO_on_off RS485Bus[4];
      {0,0,0,0},                                 // s_board_IO_on_off DigOutAlarmUrgent;
    },
    { // Ventgroup[1]
      {0,0,0,0},                                 // s_board_IO_on_off DigInOnOff;
      {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}}, // s_board_IO_on_off RS485Bus[4];
      {0,0,0,0},                                 // s_board_IO_on_off DigOutAlarmUrgent;
    },
    { // Ventgroup[2]
      {0,0,0,0},                                 // s_board_IO_on_off DigInOnOff;
      {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}}, // s_board_IO_on_off RS485Bus[4];
      {0,0,0,0},                                 // s_board_IO_on_off DigOutAlarmUrgent;
    },
    { // Ventgroup[3]
      {0,0,0,0},                                 // s_board_IO_on_off DigInOnOff;
      {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}}, // s_board_IO_on_off RS485Bus[4];
      {0,0,0,0},                                 // s_board_IO_on_off DigOutAlarmUrgent;
    },
    { // Ventgroup[4]
      {0,0,0,0},                                 // s_board_IO_on_off DigInOnOff;
      {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}}, // s_board_IO_on_off RS485Bus[4];
      {0,0,0,0},                                 // s_board_IO_on_off DigOutAlarmUrgent;
    },
    { // Ventgroup[5]
      {0,0,0,0},                                 // s_board_IO_on_off DigInOnOff;
      {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}}, // s_board_IO_on_off RS485Bus[4];
      {0,0,0,0},                                 // s_board_IO_on_off DigOutAlarmUrgent;
    },
    { // Ventgroup[6]
      {0,0,0,0},                                 // s_board_IO_on_off DigInOnOff;
      {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}}, // s_board_IO_on_off RS485Bus[4];
      {0,0,0,0},                                 // s_board_IO_on_off DigOutAlarmUrgent;
    },
    { // Ventgroup[7]
      {0,0,0,0},                                 // s_board_IO_on_off DigInOnOff;
      {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}}, // s_board_IO_on_off RS485Bus[4];
      {0,0,0,0},                                 // s_board_IO_on_off DigOutAlarmUrgent;
    },
    { // Ventgroup[8]
      {0,0,0,0},                                 // s_board_IO_on_off DigInOnOff;
      {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}}, // s_board_IO_on_off RS485Bus[4];
      {0,0,0,0},                                 // s_board_IO_on_off DigOutAlarmUrgent;
    },
    { // Ventgroup[9]
      {0,0,0,0},                                 // s_board_IO_on_off DigInOnOff;
      {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}}, // s_board_IO_on_off RS485Bus[4];
      {0,0,0,0},                                 // s_board_IO_on_off DigOutAlarmUrgent;
    },
    { // Ventgroup[10]
      {0,0,0,0},                                 // s_board_IO_on_off DigInOnOff;
      {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}}, // s_board_IO_on_off RS485Bus[4];
      {0,0,0,0},                                 // s_board_IO_on_off DigOutAlarmUrgent;
    },
    { // Ventgroup[11]
      {0,0,0,0},                                 // s_board_IO_on_off DigInOnOff;
      {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}}, // s_board_IO_on_off RS485Bus[4];
      {0,0,0,0},                                 // s_board_IO_on_off DigOutAlarmUrgent;
    },
    { // Ventgroup[12]
      {0,0,0,0},                                 // s_board_IO_on_off DigInOnOff;
      {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}}, // s_board_IO_on_off RS485Bus[4];
      {0,0,0,0},                                 // s_board_IO_on_off DigOutAlarmUrgent;
    },
    { // Ventgroup[13]
      {0,0,0,0},                                 // s_board_IO_on_off DigInOnOff;
      {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}}, // s_board_IO_on_off RS485Bus[4];
      {0,0,0,0},                                 // s_board_IO_on_off DigOutAlarmUrgent;
    },
    { // Ventgroup[14]
      {0,0,0,0},                                 // s_board_IO_on_off DigInOnOff;
      {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}}, // s_board_IO_on_off RS485Bus[4];
      {0,0,0,0},                                 // s_board_IO_on_off DigOutAlarmUrgent;
    },
    { // Ventgroup[15]
      {0,0,0,0},                                 // s_board_IO_on_off DigInOnOff;
      {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}}, // s_board_IO_on_off RS485Bus[4];
      {0,0,0,0},                                 // s_board_IO_on_off DigOutAlarmUrgent;
    },
    { // Ventgroup[16]
      {0,0,0,0},                                 // s_board_IO_on_off DigInOnOff;
      {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}}, // s_board_IO_on_off RS485Bus[4];
      {0,0,0,0},                                 // s_board_IO_on_off DigOutAlarmUrgent;
    },
    { // Ventgroup[17]
      {0,0,0,0},                                 // s_board_IO_on_off DigInOnOff;
      {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}}, // s_board_IO_on_off RS485Bus[4];
      {0,0,0,0},                                 // s_board_IO_on_off DigOutAlarmUrgent;
    },
    { // Ventgroup[18]
      {0,0,0,0},                                 // s_board_IO_on_off DigInOnOff;
      {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}}, // s_board_IO_on_off RS485Bus[4];
      {0,0,0,0},                                 // s_board_IO_on_off DigOutAlarmUrgent;
    },
    { // Ventgroup[19]
      {0,0,0,0},                                 // s_board_IO_on_off DigInOnOff;
      {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}}, // s_board_IO_on_off RS485Bus[4];
      {0,0,0,0},                                 // s_board_IO_on_off DigOutAlarmUrgent;
    },
    { // Ventgroup[20]
      {0,0,0,0},                                 // s_board_IO_on_off DigInOnOff;
      {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}}, // s_board_IO_on_off RS485Bus[4];
      {0,0,0,0},                                 // s_board_IO_on_off DigOutAlarmUrgent;
    },
    { // Ventgroup[21]
      {0,0,0,0},                                 // s_board_IO_on_off DigInOnOff;
      {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}}, // s_board_IO_on_off RS485Bus[4];
      {0,0,0,0},                                 // s_board_IO_on_off DigOutAlarmUrgent;
    },
    { // Ventgroup[22]
      {0,0,0,0},                                 // s_board_IO_on_off DigInOnOff;
      {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}}, // s_board_IO_on_off RS485Bus[4];
      {0,0,0,0},                                 // s_board_IO_on_off DigOutAlarmUrgent;
    },
    { // Ventgroup[23]
      {0,0,0,0},                                 // s_board_IO_on_off DigInOnOff;
      {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}}, // s_board_IO_on_off RS485Bus[4];
      {0,0,0,0},                                 // s_board_IO_on_off DigOutAlarmUrgent;
    },
    { // Ventgroup[24]
      {0,0,0,0},                                 // s_board_IO_on_off DigInOnOff;
      {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}}, // s_board_IO_on_off RS485Bus[4];
      {0,0,0,0},                                 // s_board_IO_on_off DigOutAlarmUrgent;
    },
    { // Ventgroup[25]
      {0,0,0,0},                                 // s_board_IO_on_off DigInOnOff;
      {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}}, // s_board_IO_on_off RS485Bus[4];
      {0,0,0,0},                                 // s_board_IO_on_off DigOutAlarmUrgent;
    },
    { // Ventgroup[26]
      {0,0,0,0},                                 // s_board_IO_on_off DigInOnOff;
      {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}}, // s_board_IO_on_off RS485Bus[4];
      {0,0,0,0},                                 // s_board_IO_on_off DigOutAlarmUrgent;
    },
    { // Ventgroup[27]
      {0,0,0,0},                                 // s_board_IO_on_off DigInOnOff;
      {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}}, // s_board_IO_on_off RS485Bus[4];
      {0,0,0,0},                                 // s_board_IO_on_off DigOutAlarmUrgent;
    },
    { // Ventgroup[28]
      {0,0,0,0},                                 // s_board_IO_on_off DigInOnOff;
      {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}}, // s_board_IO_on_off RS485Bus[4];
      {0,0,0,0},                                 // s_board_IO_on_off DigOutAlarmUrgent;
    },
    { // Ventgroup[29]
      {0,0,0,0},                                 // s_board_IO_on_off DigInOnOff;
      {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}}, // s_board_IO_on_off RS485Bus[4];
      {0,0,0,0},                                 // s_board_IO_on_off DigOutAlarmUrgent;
    },
    { // Ventgroup[30]
      {0,0,0,0},                                 // s_board_IO_on_off DigInOnOff;
      {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}}, // s_board_IO_on_off RS485Bus[4];
      {0,0,0,0},                                 // s_board_IO_on_off DigOutAlarmUrgent;
    },
    { // Ventgroup[31]
      {0,0,0,0},                                 // s_board_IO_on_off DigInOnOff;
      {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}}, // s_board_IO_on_off RS485Bus[4];
      {0,0,0,0},                                 // s_board_IO_on_off DigOutAlarmUrgent;
    },
  },

  { // TOptLuchtmengkastGroep LuchtmengkastGroep[MAX_LUCHTMENGKAST_GROEP]; // LBK: Lucht Behandelings Kast
    { // LuchtmengkastGroep[0]
      0, // unsigned char Enabled;
      0, // unsigned char TypeKlep;
      0, // unsigned char Naregelen;
      0, // unsigned char BovenklepEnabled;
      0, // unsigned char VerwarmingEnabled;
      0, // unsigned char AfblaasventEnabled;
      0, // unsigned char AfblaasventRegeling
      0, // unsigned char AfblaasventOpOnderdruk;
      0, // unsigned char AfblaasventGekoppeldAanKlep;
      0, // unsigned char BovenklepRegeling;
      0, // unsigned char BovenklepOpOnderdruk;
      0, // unsigned char BovenklepGekoppeldAanKlep;

      {0,0,0,0}, // s_board_IO_on_off Streeftemp;
      {0,0,0,0}, // s_board_IO_on_off OnderdrukSensor;

      { // TOptServo Inblaasvent;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
      { // TOptServo Afblaasvent;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
      { // TOptServo Binnenklep;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
      { // TOptServo Buitenklep;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
      { // TOptServo Verwarming;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },

      {0,0,0,0}, // s_board_IO_on_off DigOutAlarm;
      {0,0,0,0}, // s_board_IO_on_off Dummy_DigInVrijgave;
    },
    { // LuchtmengkastGroep[1]
      0, // unsigned char Enabled;
      0, // unsigned char TypeKlep;
      0, // unsigned char Naregelen;
      0, // unsigned char BovenklepEnabled;
      0, // unsigned char VerwarmingEnabled;
      0, // unsigned char AfblaasventEnabled;
      0, // unsigned char AfblaasventRegeling
      0, // unsigned char AfblaasventOpOnderdruk;
      0, // unsigned char AfblaasventGekoppeldAanKlep;
      0, // unsigned char BovenklepRegeling;
      0, // unsigned char BovenklepOpOnderdruk;
      0, // unsigned char BovenklepGekoppeldAanKlep;

      {0,0,0,0}, // s_board_IO_on_off Streeftemp;
      {0,0,0,0}, // s_board_IO_on_off OnderdrukSensor;

      { // TOptServo Inblaasvent;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
      { // TOptServo Afblaasvent;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
      { // TOptServo Binnenklep;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
      { // TOptServo Buitenklep;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
      { // TOptServo Verwarming;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },

      {0,0,0,0}, // s_board_IO_on_off DigOutAlarm;
      {0,0,0,0}, // s_board_IO_on_off Dummy_DigInVrijgave;
    },
    { // LuchtmengkastGroep[2]
      0, // unsigned char Enabled;
      0, // unsigned char TypeKlep;
      0, // unsigned char Naregelen;
      0, // unsigned char BovenklepEnabled;
      0, // unsigned char VerwarmingEnabled;
      0, // unsigned char AfblaasventEnabled;
      0, // unsigned char AfblaasventRegeling
      0, // unsigned char AfblaasventOpOnderdruk;
      0, // unsigned char AfblaasventGekoppeldAanKlep;
      0, // unsigned char BovenklepRegeling;
      0, // unsigned char BovenklepOpOnderdruk;
      0, // unsigned char BovenklepGekoppeldAanKlep;

      {0,0,0,0}, // s_board_IO_on_off Streeftemp;
      {0,0,0,0}, // s_board_IO_on_off OnderdrukSensor;

      { // TOptServo Inblaasvent;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
      { // TOptServo Afblaasvent;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
      { // TOptServo Binnenklep;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
      { // TOptServo Buitenklep;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
      { // TOptServo Verwarming;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },

      {0,0,0,0}, // s_board_IO_on_off DigOutAlarm;
      {0,0,0,0}, // s_board_IO_on_off Dummy_DigInVrijgave;
    },
    { // LuchtmengkastGroep[3]
      0, // unsigned char Enabled;
      0, // unsigned char TypeKlep;
      0, // unsigned char Naregelen;
      0, // unsigned char BovenklepEnabled;
      0, // unsigned char VerwarmingEnabled;
      0, // unsigned char AfblaasventEnabled;
      0, // unsigned char AfblaasventRegeling
      0, // unsigned char AfblaasventOpOnderdruk;
      0, // unsigned char AfblaasventGekoppeldAanKlep;
      0, // unsigned char BovenklepRegeling;
      0, // unsigned char BovenklepOpOnderdruk;
      0, // unsigned char BovenklepGekoppeldAanKlep;

      {0,0,0,0}, // s_board_IO_on_off Streeftemp;
      {0,0,0,0}, // s_board_IO_on_off OnderdrukSensor;

      { // TOptServo Inblaasvent;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
      { // TOptServo Afblaasvent;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
      { // TOptServo Binnenklep;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
      { // TOptServo Buitenklep;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
      { // TOptServo Verwarming;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },

      {0,0,0,0}, // s_board_IO_on_off DigOutAlarm;
      {0,0,0,0}, // s_board_IO_on_off Dummy_DigInVrijgave;
    },
  },

  { // TOptLuchtmengkast Luchtmengkast[MAX_LUCHTMENGKAST];
    { // Luchtmengkast[0]
      0, // unsigned char Enabled;
      0, // unsigned char TypeKlep;
      0, // unsigned char Groep;
      0, // unsigned char Naregelen;
      0, // unsigned char BovenklepEnabled;
      0, // unsigned char VerwarmingEnabled;
      0, // unsigned char AfblaasventEnabled;

      {0,0,0,0}, // s_board_IO_on_off AnaInInblaastemp;
      {0,0,0,0}, // s_board_IO_on_off AnaInMengtemp;
      {0,0,0,0}, // s_board_IO_on_off DigInVorst;
      {0,0,0,0}, // s_board_IO_on_off DigInAlarm;
      {0,0,0,0}, // s_board_IO_on_off DigInDrukverschil;

      { // TOptServo Inblaasvent;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
      { // TOptServo Afblaasvent;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
      { // TOptServo Binnenklep;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
      { // TOptServo Buitenklep;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
      { // TOptServo Verwarming;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
    },
    { // Luchtmengkast[1]
      0, // unsigned char Enabled;
      0, // unsigned char TypeKlep;
      0, // unsigned char Groep;
      0, // unsigned char Naregelen;
      0, // unsigned char BovenklepEnabled;
      0, // unsigned char VerwarmingEnabled;
      0, // unsigned char AfblaasventEnabled;

      {0,0,0,0}, // s_board_IO_on_off AnaInInblaastemp;
      {0,0,0,0}, // s_board_IO_on_off AnaInMengtemp;
      {0,0,0,0}, // s_board_IO_on_off DigInVorst;
      {0,0,0,0}, // s_board_IO_on_off DigInAlarm;
      {0,0,0,0}, // s_board_IO_on_off DigInDrukverschil;

      { // TOptServo Inblaasvent;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
      { // TOptServo Afblaasvent;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
      { // TOptServo Binnenklep;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
      { // TOptServo Buitenklep;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
      { // TOptServo Verwarming;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
    },
    { // Luchtmengkast[2]
      0, // unsigned char Enabled;
      0, // unsigned char TypeKlep;
      0, // unsigned char Groep;
      0, // unsigned char Naregelen;
      0, // unsigned char BovenklepEnabled;
      0, // unsigned char VerwarmingEnabled;
      0, // unsigned char AfblaasventEnabled;

      {0,0,0,0}, // s_board_IO_on_off AnaInInblaastemp;
      {0,0,0,0}, // s_board_IO_on_off AnaInMengtemp;
      {0,0,0,0}, // s_board_IO_on_off DigInVorst;
      {0,0,0,0}, // s_board_IO_on_off DigInAlarm;
      {0,0,0,0}, // s_board_IO_on_off DigInDrukverschil;

      { // TOptServo Inblaasvent;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
      { // TOptServo Afblaasvent;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
      { // TOptServo Binnenklep;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
      { // TOptServo Buitenklep;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
      { // TOptServo Verwarming;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
    },
    { // Luchtmengkast[3]
      0, // unsigned char Enabled;
      0, // unsigned char TypeKlep;
      0, // unsigned char Groep;
      0, // unsigned char Naregelen;
      0, // unsigned char BovenklepEnabled;
      0, // unsigned char VerwarmingEnabled;
      0, // unsigned char AfblaasventEnabled;

      {0,0,0,0}, // s_board_IO_on_off AnaInInblaastemp;
      {0,0,0,0}, // s_board_IO_on_off AnaInMengtemp;
      {0,0,0,0}, // s_board_IO_on_off DigInVorst;
      {0,0,0,0}, // s_board_IO_on_off DigInAlarm;
      {0,0,0,0}, // s_board_IO_on_off DigInDrukverschil;

      { // TOptServo Inblaasvent;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
      { // TOptServo Afblaasvent;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
      { // TOptServo Binnenklep;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
      { // TOptServo Buitenklep;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
      { // TOptServo Verwarming;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
    },
    { // Luchtmengkast[4]
      0, // unsigned char Enabled;
      0, // unsigned char TypeKlep;
      0, // unsigned char Groep;
      0, // unsigned char Naregelen;
      0, // unsigned char BovenklepEnabled;
      0, // unsigned char VerwarmingEnabled;
      0, // unsigned char AfblaasventEnabled;

      {0,0,0,0}, // s_board_IO_on_off AnaInInblaastemp;
      {0,0,0,0}, // s_board_IO_on_off AnaInMengtemp;
      {0,0,0,0}, // s_board_IO_on_off DigInVorst;
      {0,0,0,0}, // s_board_IO_on_off DigInAlarm;
      {0,0,0,0}, // s_board_IO_on_off DigInDrukverschil;

      { // TOptServo Inblaasvent;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
      { // TOptServo Afblaasvent;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
      { // TOptServo Binnenklep;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
      { // TOptServo Buitenklep;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
      { // TOptServo Verwarming;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
    },
    { // Luchtmengkast[5]
      0, // unsigned char Enabled;
      0, // unsigned char TypeKlep;
      0, // unsigned char Groep;
      0, // unsigned char Naregelen;
      0, // unsigned char BovenklepEnabled;
      0, // unsigned char VerwarmingEnabled;
      0, // unsigned char AfblaasventEnabled;

      {0,0,0,0}, // s_board_IO_on_off AnaInInblaastemp;
      {0,0,0,0}, // s_board_IO_on_off AnaInMengtemp;
      {0,0,0,0}, // s_board_IO_on_off DigInVorst;
      {0,0,0,0}, // s_board_IO_on_off DigInAlarm;
      {0,0,0,0}, // s_board_IO_on_off DigInDrukverschil;

      { // TOptServo Inblaasvent;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
      { // TOptServo Afblaasvent;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
      { // TOptServo Binnenklep;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
      { // TOptServo Buitenklep;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
      { // TOptServo Verwarming;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
    },
    { // Luchtmengkast[6]
      0, // unsigned char Enabled;
      0, // unsigned char TypeKlep;
      0, // unsigned char Groep;
      0, // unsigned char Naregelen;
      0, // unsigned char BovenklepEnabled;
      0, // unsigned char VerwarmingEnabled;
      0, // unsigned char AfblaasventEnabled;

      {0,0,0,0}, // s_board_IO_on_off AnaInInblaastemp;
      {0,0,0,0}, // s_board_IO_on_off AnaInMengtemp;
      {0,0,0,0}, // s_board_IO_on_off DigInVorst;
      {0,0,0,0}, // s_board_IO_on_off DigInAlarm;
      {0,0,0,0}, // s_board_IO_on_off DigInDrukverschil;

      { // TOptServo Inblaasvent;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
      { // TOptServo Afblaasvent;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
      { // TOptServo Binnenklep;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
      { // TOptServo Buitenklep;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
      { // TOptServo Verwarming;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
    },
    { // Luchtmengkast[7]
      0, // unsigned char Enabled;
      0, // unsigned char TypeKlep;
      0, // unsigned char Groep;
      0, // unsigned char Naregelen;
      0, // unsigned char BovenklepEnabled;
      0, // unsigned char VerwarmingEnabled;
      0, // unsigned char AfblaasventEnabled;

      {0,0,0,0}, // s_board_IO_on_off AnaInInblaastemp;
      {0,0,0,0}, // s_board_IO_on_off AnaInMengtemp;
      {0,0,0,0}, // s_board_IO_on_off DigInVorst;
      {0,0,0,0}, // s_board_IO_on_off DigInAlarm;
      {0,0,0,0}, // s_board_IO_on_off DigInDrukverschil;

      { // TOptServo Inblaasvent;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
      { // TOptServo Afblaasvent;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
      { // TOptServo Binnenklep;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
      { // TOptServo Buitenklep;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
      { // TOptServo Verwarming;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
    },
    { // Luchtmengkast[8]
      0, // unsigned char Enabled;
      0, // unsigned char TypeKlep;
      0, // unsigned char Groep;
      0, // unsigned char Naregelen;
      0, // unsigned char BovenklepEnabled;
      0, // unsigned char VerwarmingEnabled;
      0, // unsigned char AfblaasventEnabled;

      {0,0,0,0}, // s_board_IO_on_off AnaInInblaastemp;
      {0,0,0,0}, // s_board_IO_on_off AnaInMengtemp;
      {0,0,0,0}, // s_board_IO_on_off DigInVorst;
      {0,0,0,0}, // s_board_IO_on_off DigInAlarm;
      {0,0,0,0}, // s_board_IO_on_off DigInDrukverschil;

      { // TOptServo Inblaasvent;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
      { // TOptServo Afblaasvent;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
      { // TOptServo Binnenklep;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
      { // TOptServo Buitenklep;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
      { // TOptServo Verwarming;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
    },
    { // Luchtmengkast[9]
      0, // unsigned char Enabled;
      0, // unsigned char TypeKlep;
      0, // unsigned char Groep;
      0, // unsigned char Naregelen;
      0, // unsigned char BovenklepEnabled;
      0, // unsigned char VerwarmingEnabled;
      0, // unsigned char AfblaasventEnabled;

      {0,0,0,0}, // s_board_IO_on_off AnaInInblaastemp;
      {0,0,0,0}, // s_board_IO_on_off AnaInMengtemp;
      {0,0,0,0}, // s_board_IO_on_off DigInVorst;
      {0,0,0,0}, // s_board_IO_on_off DigInAlarm;
      {0,0,0,0}, // s_board_IO_on_off DigInDrukverschil;

      { // TOptServo Inblaasvent;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
      { // TOptServo Afblaasvent;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
      { // TOptServo Binnenklep;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
      { // TOptServo Buitenklep;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
      { // TOptServo Verwarming;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
    },
    { // Luchtmengkast[10]
      0, // unsigned char Enabled;
      0, // unsigned char TypeKlep;
      0, // unsigned char Groep;
      0, // unsigned char Naregelen;
      0, // unsigned char BovenklepEnabled;
      0, // unsigned char VerwarmingEnabled;
      0, // unsigned char AfblaasventEnabled;

      {0,0,0,0}, // s_board_IO_on_off AnaInInblaastemp;
      {0,0,0,0}, // s_board_IO_on_off AnaInMengtemp;
      {0,0,0,0}, // s_board_IO_on_off DigInVorst;
      {0,0,0,0}, // s_board_IO_on_off DigInAlarm;
      {0,0,0,0}, // s_board_IO_on_off DigInDrukverschil;

      { // TOptServo Inblaasvent;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
      { // TOptServo Afblaasvent;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
      { // TOptServo Binnenklep;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
      { // TOptServo Buitenklep;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
      { // TOptServo Verwarming;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
    },
    { // Luchtmengkast[11]
      0, // unsigned char Enabled;
      0, // unsigned char TypeKlep;
      0, // unsigned char Groep;
      0, // unsigned char Naregelen;
      0, // unsigned char BovenklepEnabled;
      0, // unsigned char VerwarmingEnabled;
      0, // unsigned char AfblaasventEnabled;

      {0,0,0,0}, // s_board_IO_on_off AnaInInblaastemp;
      {0,0,0,0}, // s_board_IO_on_off AnaInMengtemp;
      {0,0,0,0}, // s_board_IO_on_off DigInVorst;
      {0,0,0,0}, // s_board_IO_on_off DigInAlarm;
      {0,0,0,0}, // s_board_IO_on_off DigInDrukverschil;

      { // TOptServo Inblaasvent;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
      { // TOptServo Afblaasvent;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
      { // TOptServo Binnenklep;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
      { // TOptServo Buitenklep;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
      { // TOptServo Verwarming;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
    },
    { // Luchtmengkast[12]
      0, // unsigned char Enabled;
      0, // unsigned char TypeKlep;
      0, // unsigned char Groep;
      0, // unsigned char Naregelen;
      0, // unsigned char BovenklepEnabled;
      0, // unsigned char VerwarmingEnabled;
      0, // unsigned char AfblaasventEnabled;

      {0,0,0,0}, // s_board_IO_on_off AnaInInblaastemp;
      {0,0,0,0}, // s_board_IO_on_off AnaInMengtemp;
      {0,0,0,0}, // s_board_IO_on_off DigInVorst;
      {0,0,0,0}, // s_board_IO_on_off DigInAlarm;
      {0,0,0,0}, // s_board_IO_on_off DigInDrukverschil;

      { // TOptServo Inblaasvent;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
      { // TOptServo Afblaasvent;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
      { // TOptServo Binnenklep;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
      { // TOptServo Buitenklep;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
      { // TOptServo Verwarming;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
    },
    { // Luchtmengkast[13]
      0, // unsigned char Enabled;
      0, // unsigned char TypeKlep;
      0, // unsigned char Groep;
      0, // unsigned char Naregelen;
      0, // unsigned char BovenklepEnabled;
      0, // unsigned char VerwarmingEnabled;
      0, // unsigned char AfblaasventEnabled;

      {0,0,0,0}, // s_board_IO_on_off AnaInInblaastemp;
      {0,0,0,0}, // s_board_IO_on_off AnaInMengtemp;
      {0,0,0,0}, // s_board_IO_on_off DigInVorst;
      {0,0,0,0}, // s_board_IO_on_off DigInAlarm;
      {0,0,0,0}, // s_board_IO_on_off DigInDrukverschil;

      { // TOptServo Inblaasvent;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
      { // TOptServo Afblaasvent;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
      { // TOptServo Binnenklep;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
      { // TOptServo Buitenklep;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
      { // TOptServo Verwarming;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
    },
    { // Luchtmengkast[14]
      0, // unsigned char Enabled;
      0, // unsigned char TypeKlep;
      0, // unsigned char Groep;
      0, // unsigned char Naregelen;
      0, // unsigned char BovenklepEnabled;
      0, // unsigned char VerwarmingEnabled;
      0, // unsigned char AfblaasventEnabled;

      {0,0,0,0}, // s_board_IO_on_off AnaInInblaastemp;
      {0,0,0,0}, // s_board_IO_on_off AnaInMengtemp;
      {0,0,0,0}, // s_board_IO_on_off DigInVorst;
      {0,0,0,0}, // s_board_IO_on_off DigInAlarm;
      {0,0,0,0}, // s_board_IO_on_off DigInDrukverschil;

      { // TOptServo Inblaasvent;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
      { // TOptServo Afblaasvent;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
      { // TOptServo Binnenklep;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
      { // TOptServo Buitenklep;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
      { // TOptServo Verwarming;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
    },
    { // Luchtmengkast[15]
      0, // unsigned char Enabled;
      0, // unsigned char TypeKlep;
      0, // unsigned char Groep;
      0, // unsigned char Naregelen;
      0, // unsigned char BovenklepEnabled;
      0, // unsigned char VerwarmingEnabled;
      0, // unsigned char AfblaasventEnabled;

      {0,0,0,0}, // s_board_IO_on_off AnaInInblaastemp;
      {0,0,0,0}, // s_board_IO_on_off AnaInMengtemp;
      {0,0,0,0}, // s_board_IO_on_off DigInVorst;
      {0,0,0,0}, // s_board_IO_on_off DigInAlarm;
      {0,0,0,0}, // s_board_IO_on_off DigInDrukverschil;

      { // TOptServo Inblaasvent;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
      { // TOptServo Afblaasvent;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
      { // TOptServo Binnenklep;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
      { // TOptServo Buitenklep;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
      { // TOptServo Verwarming;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
    },
    { // Luchtmengkast[16]
      0, // unsigned char Enabled;
      0, // unsigned char TypeKlep;
      0, // unsigned char Groep;
      0, // unsigned char Naregelen;
      0, // unsigned char BovenklepEnabled;
      0, // unsigned char VerwarmingEnabled;
      0, // unsigned char AfblaasventEnabled;

      {0,0,0,0}, // s_board_IO_on_off AnaInInblaastemp;
      {0,0,0,0}, // s_board_IO_on_off AnaInMengtemp;
      {0,0,0,0}, // s_board_IO_on_off DigInVorst;
      {0,0,0,0}, // s_board_IO_on_off DigInAlarm;
      {0,0,0,0}, // s_board_IO_on_off DigInDrukverschil;

      { // TOptServo Inblaasvent;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
      { // TOptServo Afblaasvent;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
      { // TOptServo Binnenklep;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
      { // TOptServo Buitenklep;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
      { // TOptServo Verwarming;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
    },
    { // Luchtmengkast[17]
      0, // unsigned char Enabled;
      0, // unsigned char TypeKlep;
      0, // unsigned char Groep;
      0, // unsigned char Naregelen;
      0, // unsigned char BovenklepEnabled;
      0, // unsigned char VerwarmingEnabled;
      0, // unsigned char AfblaasventEnabled;

      {0,0,0,0}, // s_board_IO_on_off AnaInInblaastemp;
      {0,0,0,0}, // s_board_IO_on_off AnaInMengtemp;
      {0,0,0,0}, // s_board_IO_on_off DigInVorst;
      {0,0,0,0}, // s_board_IO_on_off DigInAlarm;
      {0,0,0,0}, // s_board_IO_on_off DigInDrukverschil;

      { // TOptServo Inblaasvent;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
      { // TOptServo Afblaasvent;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
      { // TOptServo Binnenklep;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
      { // TOptServo Buitenklep;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
      { // TOptServo Verwarming;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
    },
    { // Luchtmengkast[18]
      0, // unsigned char Enabled;
      0, // unsigned char TypeKlep;
      0, // unsigned char Groep;
      0, // unsigned char Naregelen;
      0, // unsigned char BovenklepEnabled;
      0, // unsigned char VerwarmingEnabled;
      0, // unsigned char AfblaasventEnabled;

      {0,0,0,0}, // s_board_IO_on_off AnaInInblaastemp;
      {0,0,0,0}, // s_board_IO_on_off AnaInMengtemp;
      {0,0,0,0}, // s_board_IO_on_off DigInVorst;
      {0,0,0,0}, // s_board_IO_on_off DigInAlarm;
      {0,0,0,0}, // s_board_IO_on_off DigInDrukverschil;

      { // TOptServo Inblaasvent;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
      { // TOptServo Afblaasvent;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
      { // TOptServo Binnenklep;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
      { // TOptServo Buitenklep;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
      { // TOptServo Verwarming;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
    },
    { // Luchtmengkast[19]
      0, // unsigned char Enabled;
      0, // unsigned char TypeKlep;
      0, // unsigned char Groep;
      0, // unsigned char Naregelen;
      0, // unsigned char BovenklepEnabled;
      0, // unsigned char VerwarmingEnabled;
      0, // unsigned char AfblaasventEnabled;

      {0,0,0,0}, // s_board_IO_on_off AnaInInblaastemp;
      {0,0,0,0}, // s_board_IO_on_off AnaInMengtemp;
      {0,0,0,0}, // s_board_IO_on_off DigInVorst;
      {0,0,0,0}, // s_board_IO_on_off DigInAlarm;
      {0,0,0,0}, // s_board_IO_on_off DigInDrukverschil;

      { // TOptServo Inblaasvent;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
      { // TOptServo Afblaasvent;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
      { // TOptServo Binnenklep;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
      { // TOptServo Buitenklep;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
      { // TOptServo Verwarming;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
    },
    { // Luchtmengkast[20]
      0, // unsigned char Enabled;
      0, // unsigned char TypeKlep;
      0, // unsigned char Groep;
      0, // unsigned char Naregelen;
      0, // unsigned char BovenklepEnabled;
      0, // unsigned char VerwarmingEnabled;
      0, // unsigned char AfblaasventEnabled;

      {0,0,0,0}, // s_board_IO_on_off AnaInInblaastemp;
      {0,0,0,0}, // s_board_IO_on_off AnaInMengtemp;
      {0,0,0,0}, // s_board_IO_on_off DigInVorst;
      {0,0,0,0}, // s_board_IO_on_off DigInAlarm;
      {0,0,0,0}, // s_board_IO_on_off DigInDrukverschil;

      { // TOptServo Inblaasvent;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
      { // TOptServo Afblaasvent;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
      { // TOptServo Binnenklep;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
      { // TOptServo Buitenklep;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
      { // TOptServo Verwarming;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
    },
    { // Luchtmengkast[21]
      0, // unsigned char Enabled;
      0, // unsigned char TypeKlep;
      0, // unsigned char Groep;
      0, // unsigned char Naregelen;
      0, // unsigned char BovenklepEnabled;
      0, // unsigned char VerwarmingEnabled;
      0, // unsigned char AfblaasventEnabled;

      {0,0,0,0}, // s_board_IO_on_off AnaInInblaastemp;
      {0,0,0,0}, // s_board_IO_on_off AnaInMengtemp;
      {0,0,0,0}, // s_board_IO_on_off DigInVorst;
      {0,0,0,0}, // s_board_IO_on_off DigInAlarm;
      {0,0,0,0}, // s_board_IO_on_off DigInDrukverschil;

      { // TOptServo Inblaasvent;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
      { // TOptServo Afblaasvent;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
      { // TOptServo Binnenklep;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
      { // TOptServo Buitenklep;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
      { // TOptServo Verwarming;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
    },
    { // Luchtmengkast[22]
      0, // unsigned char Enabled;
      0, // unsigned char TypeKlep;
      0, // unsigned char Groep;
      0, // unsigned char Naregelen;
      0, // unsigned char BovenklepEnabled;
      0, // unsigned char VerwarmingEnabled;
      0, // unsigned char AfblaasventEnabled;

      {0,0,0,0}, // s_board_IO_on_off AnaInInblaastemp;
      {0,0,0,0}, // s_board_IO_on_off AnaInMengtemp;
      {0,0,0,0}, // s_board_IO_on_off DigInVorst;
      {0,0,0,0}, // s_board_IO_on_off DigInAlarm;
      {0,0,0,0}, // s_board_IO_on_off DigInDrukverschil;

      { // TOptServo Inblaasvent;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
      { // TOptServo Afblaasvent;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
      { // TOptServo Binnenklep;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
      { // TOptServo Buitenklep;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
      { // TOptServo Verwarming;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
    },
    { // Luchtmengkast[23]
      0, // unsigned char Enabled;
      0, // unsigned char TypeKlep;
      0, // unsigned char Groep;
      0, // unsigned char Naregelen;
      0, // unsigned char BovenklepEnabled;
      0, // unsigned char VerwarmingEnabled;
      0, // unsigned char AfblaasventEnabled;

      {0,0,0,0}, // s_board_IO_on_off AnaInInblaastemp;
      {0,0,0,0}, // s_board_IO_on_off AnaInMengtemp;
      {0,0,0,0}, // s_board_IO_on_off DigInVorst;
      {0,0,0,0}, // s_board_IO_on_off DigInAlarm;
      {0,0,0,0}, // s_board_IO_on_off DigInDrukverschil;

      { // TOptServo Inblaasvent;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
      { // TOptServo Afblaasvent;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
      { // TOptServo Binnenklep;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
      { // TOptServo Buitenklep;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
      { // TOptServo Verwarming;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
    },
    { // Luchtmengkast[24]
      0, // unsigned char Enabled;
      0, // unsigned char TypeKlep;
      0, // unsigned char Groep;
      0, // unsigned char Naregelen;
      0, // unsigned char BovenklepEnabled;
      0, // unsigned char VerwarmingEnabled;
      0, // unsigned char AfblaasventEnabled;

      {0,0,0,0}, // s_board_IO_on_off AnaInInblaastemp;
      {0,0,0,0}, // s_board_IO_on_off AnaInMengtemp;
      {0,0,0,0}, // s_board_IO_on_off DigInVorst;
      {0,0,0,0}, // s_board_IO_on_off DigInAlarm;
      {0,0,0,0}, // s_board_IO_on_off DigInDrukverschil;

      { // TOptServo Inblaasvent;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
      { // TOptServo Afblaasvent;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
      { // TOptServo Binnenklep;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
      { // TOptServo Buitenklep;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
      { // TOptServo Verwarming;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
    },
    { // Luchtmengkast[25]
      0, // unsigned char Enabled;
      0, // unsigned char TypeKlep;
      0, // unsigned char Groep;
      0, // unsigned char Naregelen;
      0, // unsigned char BovenklepEnabled;
      0, // unsigned char VerwarmingEnabled;
      0, // unsigned char AfblaasventEnabled;

      {0,0,0,0}, // s_board_IO_on_off AnaInInblaastemp;
      {0,0,0,0}, // s_board_IO_on_off AnaInMengtemp;
      {0,0,0,0}, // s_board_IO_on_off DigInVorst;
      {0,0,0,0}, // s_board_IO_on_off DigInAlarm;
      {0,0,0,0}, // s_board_IO_on_off DigInDrukverschil;

      { // TOptServo Inblaasvent;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
      { // TOptServo Afblaasvent;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
      { // TOptServo Binnenklep;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
      { // TOptServo Buitenklep;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
      { // TOptServo Verwarming;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
    },
    { // Luchtmengkast[26]
      0, // unsigned char Enabled;
      0, // unsigned char TypeKlep;
      0, // unsigned char Groep;
      0, // unsigned char Naregelen;
      0, // unsigned char BovenklepEnabled;
      0, // unsigned char VerwarmingEnabled;
      0, // unsigned char AfblaasventEnabled;

      {0,0,0,0}, // s_board_IO_on_off AnaInInblaastemp;
      {0,0,0,0}, // s_board_IO_on_off AnaInMengtemp;
      {0,0,0,0}, // s_board_IO_on_off DigInVorst;
      {0,0,0,0}, // s_board_IO_on_off DigInAlarm;
      {0,0,0,0}, // s_board_IO_on_off DigInDrukverschil;

      { // TOptServo Inblaasvent;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
      { // TOptServo Afblaasvent;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
      { // TOptServo Binnenklep;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
      { // TOptServo Buitenklep;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
      { // TOptServo Verwarming;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
    },
    { // Luchtmengkast[27]
      0, // unsigned char Enabled;
      0, // unsigned char TypeKlep;
      0, // unsigned char Groep;
      0, // unsigned char Naregelen;
      0, // unsigned char BovenklepEnabled;
      0, // unsigned char VerwarmingEnabled;
      0, // unsigned char AfblaasventEnabled;

      {0,0,0,0}, // s_board_IO_on_off AnaInInblaastemp;
      {0,0,0,0}, // s_board_IO_on_off AnaInMengtemp;
      {0,0,0,0}, // s_board_IO_on_off DigInVorst;
      {0,0,0,0}, // s_board_IO_on_off DigInAlarm;
      {0,0,0,0}, // s_board_IO_on_off DigInDrukverschil;

      { // TOptServo Inblaasvent;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
      { // TOptServo Afblaasvent;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
      { // TOptServo Binnenklep;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
      { // TOptServo Buitenklep;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
      { // TOptServo Verwarming;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
    },
    { // Luchtmengkast[28]
      0, // unsigned char Enabled;
      0, // unsigned char TypeKlep;
      0, // unsigned char Groep;
      0, // unsigned char Naregelen;
      0, // unsigned char BovenklepEnabled;
      0, // unsigned char VerwarmingEnabled;
      0, // unsigned char AfblaasventEnabled;

      {0,0,0,0}, // s_board_IO_on_off AnaInInblaastemp;
      {0,0,0,0}, // s_board_IO_on_off AnaInMengtemp;
      {0,0,0,0}, // s_board_IO_on_off DigInVorst;
      {0,0,0,0}, // s_board_IO_on_off DigInAlarm;
      {0,0,0,0}, // s_board_IO_on_off DigInDrukverschil;

      { // TOptServo Inblaasvent;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
      { // TOptServo Afblaasvent;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
      { // TOptServo Binnenklep;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
      { // TOptServo Buitenklep;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
      { // TOptServo Verwarming;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
    },
    { // Luchtmengkast[29]
      0, // unsigned char Enabled;
      0, // unsigned char TypeKlep;
      0, // unsigned char Groep;
      0, // unsigned char Naregelen;
      0, // unsigned char BovenklepEnabled;
      0, // unsigned char VerwarmingEnabled;
      0, // unsigned char AfblaasventEnabled;

      {0,0,0,0}, // s_board_IO_on_off AnaInInblaastemp;
      {0,0,0,0}, // s_board_IO_on_off AnaInMengtemp;
      {0,0,0,0}, // s_board_IO_on_off DigInVorst;
      {0,0,0,0}, // s_board_IO_on_off DigInAlarm;
      {0,0,0,0}, // s_board_IO_on_off DigInDrukverschil;

      { // TOptServo Inblaasvent;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
      { // TOptServo Afblaasvent;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
      { // TOptServo Binnenklep;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
      { // TOptServo Buitenklep;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
      { // TOptServo Verwarming;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
    },
    { // Luchtmengkast[30]
      0, // unsigned char Enabled;
      0, // unsigned char TypeKlep;
      0, // unsigned char Groep;
      0, // unsigned char Naregelen;
      0, // unsigned char BovenklepEnabled;
      0, // unsigned char VerwarmingEnabled;
      0, // unsigned char AfblaasventEnabled;

      {0,0,0,0}, // s_board_IO_on_off AnaInInblaastemp;
      {0,0,0,0}, // s_board_IO_on_off AnaInMengtemp;
      {0,0,0,0}, // s_board_IO_on_off DigInVorst;
      {0,0,0,0}, // s_board_IO_on_off DigInAlarm;
      {0,0,0,0}, // s_board_IO_on_off DigInDrukverschil;

      { // TOptServo Inblaasvent;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
      { // TOptServo Afblaasvent;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
      { // TOptServo Binnenklep;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
      { // TOptServo Buitenklep;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
      { // TOptServo Verwarming;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
    },
    { // Luchtmengkast[31]
      0, // unsigned char Enabled;
      0, // unsigned char TypeKlep;
      0, // unsigned char Groep;
      0, // unsigned char Naregelen;
      0, // unsigned char BovenklepEnabled;
      0, // unsigned char VerwarmingEnabled;
      0, // unsigned char AfblaasventEnabled;

      {0,0,0,0}, // s_board_IO_on_off AnaInInblaastemp;
      {0,0,0,0}, // s_board_IO_on_off AnaInMengtemp;
      {0,0,0,0}, // s_board_IO_on_off DigInVorst;
      {0,0,0,0}, // s_board_IO_on_off DigInAlarm;
      {0,0,0,0}, // s_board_IO_on_off DigInDrukverschil;

      { // TOptServo Inblaasvent;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
      { // TOptServo Afblaasvent;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
      { // TOptServo Binnenklep;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
      { // TOptServo Buitenklep;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
      { // TOptServo Verwarming;
        600,       // unsigned int  Runtime;     // Time from 0-100% [x100ms]
        0,         // unsigned char TypeSturing; // Analoog, Digitaal, CANopen, BACnet
        0,         // unsigned char Adres;       // Groep nummer in geval van ebmBus, CANopen of BACnet
        {0,0,0,0}, // s_board_IO_on_off Open;
        {0,0,0,0}, // s_board_IO_on_off Close;
        {0,0,0,0}, // s_board_IO_on_off AnaIn;
        {0,0,0,0}, // s_board_IO_on_off AnaOut;
      },
    },
  },

  { // TOptAlarm Alarm;
    {0,0,0,0}, // s_board_IO_on_off DigOutZacht;
  },

  0, // unsigned char LuchtmengkastInblaasventClosedLoopOpenLoop;
  0, // unsigned char LuchtmengkastAfblaasventClosedLoopOpenLoop;

  100, // unsigned char LuchtmengkastInblaasventPowerFactor;
  12,  // unsigned char LuchtmengkastInblaasventRampUp;
  12,  // unsigned char LuchtmengkastInblaasventRampDown;
  100, // unsigned char LuchtmengkastAfblaasventPowerFactor;
  12,  // unsigned char LuchtmengkastAfblaasventRampUp;
  12,  // unsigned char LuchtmengkastAfblaasventRampDown;

  { // TOptVrijgaveVent VrijgaveVent;
    0, // unsigned char Aantal;
    {  // unsigned int  Ventilator[MAX_EC_VENT];
	  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	},
    {  // s_board_IO_on_off DigInVrijgave[MAX_VRIJGAVE];
	  {0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},
	  {0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}
    },
    {  // s_board_IO_on_off RS485Bus[MAX_VRIJGAVE];
	  {0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},
	  {0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}
    },
  },

  { // TOptSensoren Sensoren;
    0, // unsigned char Drukverschil; // Aantal
    0, // unsigned char Type;         // Type (Modbus/Analoog)
    0, // unsigned char FirstAddress;
    {  // s_board_IO_on_off RS485Bus[MAX_SENSOREN]; // IO
      {0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},
      {0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}
    },
  },

  0xAAAA,     // unsigned int end;    // Laatste adres opties. Bevat de checksum
};

//=============================================================================
s_opt_io const default_opt_io = 
{
  0,                   // unsigned int area_size;        // Option size, verschil begin option, end option
  ORION,               // unsigned int computer;         // identification computer
  MULTI_CONNECT,       // unsigned int soort;            // soort 0 = BASIS; 1 = PLUIMVEE; 2 = VARKENS
  MODULE_BASIS,        // unsigned int type;             // type onderverdeling 0 = PLUIMVEE_CL; 1 = PLUIMVEE_PB;  
  HOTRACO,             // unsigned int firma;            // firma 0 = hotraco
  VERSIE_PROG,         // unsigned int versie_programma; // versie nummer eprom
  VERSIE_ALG_PC,       // unsigned int versie_PC;        // versie pc
  0,                   // unsigned long serie_number;    // serie number
  VERSIE_TEKST,        // unsigned int versie_tekst;     // reserve voor toekomst wat vooraan moet staan zoals bovenstaade items
  VERSIE_TEKST_INST,   // unsigned int versie_tekst_inst;// reserve voor toekomst wat vooraan moet staan zoals bovenstaade items
  0,   // unsigned int reserve2;         // reserve voor toekomst wat vooraan moet staan zoals bovenstaade items
  0,   // unsigned int reserve3;         // reserve voor toekomst wat vooraan moet staan zoals bovenstaade items
  0,   // unsigned int reserve4;         // reserve voor toekomst wat vooraan moet staan zoals bovenstaade items

  0,   // int change_cnt;           // variabele wordt met 1 opgehoogd als options veranderd zijn

  { // s_option_IO_05_07  IO_05_07[IO_05_07_MAX];
    { // IO_05_07[0]
	  // s_option_board_component board_component;
      { 0,0x12345678 },
	  // s_option_analog_input analog_input[IO_05_07_ANALOG_INPUT];
      {
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  0
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  1
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  2
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  3
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  4
      },
      { // s_option_analog_output analog_output[IO_05_07_ANALOG_OUTPUT];
        {0,0,0,0,0x3FFF,0,ANA_OUT_EMPTY}, // 0
        {0,0,0,0,0x3FFF,0,ANA_OUT_EMPTY}, // 1
      },
      // s_option_digital_output digital_output;
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // 0
      // s_option_RS485_bus RS485_bus;
      {0,br9600,0,7,0,REFRESH_TIME_100MS_BASE,1500,0,0},
      1, // unsigned char alarm;
    },
    { // IO_05_07[1]
      // s_option_board_component board_component;
      { 0,0x12345678 },
      // s_option_analog_input analog_input[IO_05_07_ANALOG_INPUT];
      { 
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  0
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  1
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  2
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  3
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  4
      },
      { // s_option_analog_output analog_output[IO_05_07_ANALOG_OUTPUT];
        {0,0,0,0,0x3FFF,0,ANA_OUT_EMPTY}, // 0
        {0,0,0,0,0x3FFF,0,ANA_OUT_EMPTY}, // 1
      },
      // s_option_digital_output digital_output;
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // 0
      // s_option_RS485_bus RS485_bus;
      {0,br9600,0,7,0,REFRESH_TIME_100MS_BASE,1500,0,0},
      1, // unsigned char alarm;
    },
    { // IO_05_07[2]
      // s_option_board_component board_component;
      { 0,0x12345678 },
      // s_option_analog_input analog_input[IO_05_07_ANALOG_INPUT];
      { 
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  0
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  1
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  2
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  3
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  4
      },
      { // s_option_analog_output analog_output[IO_05_07_ANALOG_OUTPUT];
        {0,0,0,0,0x3FFF,0,ANA_OUT_EMPTY}, // 0
        {0,0,0,0,0x3FFF,0,ANA_OUT_EMPTY}, // 1
      },
      // s_option_digital_output digital_output;
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // 0
      // s_option_RS485_bus RS485_bus;
      {0,br9600,0,7,0,REFRESH_TIME_100MS_BASE,1500,0,0},
      1, // unsigned char alarm;
    },
    { // IO_05_07[3]
      // s_option_board_component board_component;
      { 0,0x12345678 },
      // s_option_analog_input analog_input[IO_05_07_ANALOG_INPUT];
      { 
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  0
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  1
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  2
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  3
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  4
      },
      { // s_option_analog_output analog_output[IO_05_07_ANALOG_OUTPUT];
        {0,0,0,0,0x3FFF,0,ANA_OUT_EMPTY}, // 0
        {0,0,0,0,0x3FFF,0,ANA_OUT_EMPTY}, // 1
      },
      // s_option_digital_output digital_output;
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // 0
      // s_option_RS485_bus RS485_bus;
      {0,br9600,0,7,0,REFRESH_TIME_100MS_BASE,1500,0,0},
      1, // unsigned char alarm;
    },
    { // IO_05_07[4]
      // s_option_board_component board_component;
      { 0,0x12345678 },
      // s_option_analog_input analog_input[IO_05_07_ANALOG_INPUT];
      { 
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  0
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  1
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  2
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  3
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  4
      },
      { // s_option_analog_output analog_output[IO_05_07_ANALOG_OUTPUT];
        {0,0,0,0,0x3FFF,0,ANA_OUT_EMPTY}, // 0
        {0,0,0,0,0x3FFF,0,ANA_OUT_EMPTY}, // 1
      },
      // s_option_digital_output digital_output;
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // 0
      // s_option_RS485_bus RS485_bus;
      {0,br9600,0,7,0,REFRESH_TIME_100MS_BASE,1500,0,0},
      1, // unsigned char alarm;
    },
    { // IO_05_07[5]
      // s_option_board_component board_component;
      { 0,0x12345678 },
      // s_option_analog_input analog_input[IO_05_07_ANALOG_INPUT];
      { 
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  0
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  1
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  2
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  3
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  4
      },
      { // s_option_analog_output analog_output[IO_05_07_ANALOG_OUTPUT];
        {0,0,0,0,0x3FFF,0,ANA_OUT_EMPTY}, // 0
        {0,0,0,0,0x3FFF,0,ANA_OUT_EMPTY}, // 1
      },
      // s_option_digital_output digital_output;
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // 0
      // s_option_RS485_bus RS485_bus;
      {0,br9600,0,7,0,REFRESH_TIME_100MS_BASE,1500,0,0},
      1, // unsigned char alarm;
    },
    { // IO_05_07[6]
      // s_option_board_component board_component;
      { 0,0x12345678 },
      // s_option_analog_input analog_input[IO_05_07_ANALOG_INPUT];
      { 
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  0
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  1
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  2
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  3
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  4
      },
      { // s_option_analog_output analog_output[IO_05_07_ANALOG_OUTPUT];
        {0,0,0,0,0x3FFF,0,ANA_OUT_EMPTY}, // 0
        {0,0,0,0,0x3FFF,0,ANA_OUT_EMPTY}, // 1
      },
      // s_option_digital_output digital_output;
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // 0
      // s_option_RS485_bus RS485_bus;
      {0,br9600,0,7,0,REFRESH_TIME_100MS_BASE,1500,0,0},
      1, // unsigned char alarm;
    },
    { // IO_05_07[7]
      // s_option_board_component board_component;
      { 0,0x12345678 },
      // s_option_analog_input analog_input[IO_05_07_ANALOG_INPUT];
      { 
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  0
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  1
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  2
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  3
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  4
      },
      { // s_option_analog_output analog_output[IO_05_07_ANALOG_OUTPUT];
        {0,0,0,0,0x3FFF,0,ANA_OUT_EMPTY}, // 0
        {0,0,0,0,0x3FFF,0,ANA_OUT_EMPTY}, // 1
      },
      // s_option_digital_output digital_output;
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // 0
      // s_option_RS485_bus RS485_bus;
      {0,br9600,0,7,0,REFRESH_TIME_100MS_BASE,1500,0,0},
      1, // unsigned char alarm;
    },
    { // IO_05_07[8]
      // s_option_board_component board_component;
      { 0,0x12345678 },
      // s_option_analog_input analog_input[IO_05_07_ANALOG_INPUT];
      { 
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  0
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  1
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  2
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  3
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  4
      },
      { // s_option_analog_output analog_output[IO_05_07_ANALOG_OUTPUT];
        {0,0,0,0,0x3FFF,0,ANA_OUT_EMPTY}, // 0
        {0,0,0,0,0x3FFF,0,ANA_OUT_EMPTY}, // 1
      },
      // s_option_digital_output digital_output;
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // 0
      // s_option_RS485_bus RS485_bus;
      {0,br9600,0,7,0,REFRESH_TIME_100MS_BASE,1500,0,0},
      1, // unsigned char alarm;
    },
    { // IO_05_07[9]
      // s_option_board_component board_component;
      { 0,0x12345678 },
      // s_option_analog_input analog_input[IO_05_07_ANALOG_INPUT];
      { 
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  0
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  1
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  2
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  3
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  4
      },
      { // s_option_analog_output analog_output[IO_05_07_ANALOG_OUTPUT];
        {0,0,0,0,0x3FFF,0,ANA_OUT_EMPTY}, // 0
        {0,0,0,0,0x3FFF,0,ANA_OUT_EMPTY}, // 1
      },
      // s_option_digital_output digital_output;
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // 0
      // s_option_RS485_bus RS485_bus;
      {0,br9600,0,7,0,REFRESH_TIME_100MS_BASE,1500,0,0},
      1, // unsigned char alarm;
    },
    { // IO_05_07[10]
      // s_option_board_component board_component;
      { 0,0x12345678 },
      // s_option_analog_input analog_input[IO_05_07_ANALOG_INPUT];
      { 
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  0
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  1
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  2
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  3
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  4
      },
      { // s_option_analog_output analog_output[IO_05_07_ANALOG_OUTPUT];
        {0,0,0,0,0x3FFF,0,ANA_OUT_EMPTY}, // 0
        {0,0,0,0,0x3FFF,0,ANA_OUT_EMPTY}, // 1
      },
      // s_option_digital_output digital_output;
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // 0
      // s_option_RS485_bus RS485_bus;
      {0,br9600,0,7,0,REFRESH_TIME_100MS_BASE,1500,0,0},
      1, // unsigned char alarm;
    },
    { // IO_05_07[11]
      // s_option_board_component board_component;
      { 0,0x12345678 },
      // s_option_analog_input analog_input[IO_05_07_ANALOG_INPUT];
      { 
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  0
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  1
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  2
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  3
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  4
      },
      { // s_option_analog_output analog_output[IO_05_07_ANALOG_OUTPUT];
        {0,0,0,0,0x3FFF,0,ANA_OUT_EMPTY}, // 0
        {0,0,0,0,0x3FFF,0,ANA_OUT_EMPTY}, // 1
      },
      // s_option_digital_output digital_output;
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // 0
      // s_option_RS485_bus RS485_bus;
      {0,br9600,0,7,0,REFRESH_TIME_100MS_BASE,1500,0,0},
      1, // unsigned char alarm;
    },
    { // IO_05_07[12]
      // s_option_board_component board_component;
      { 0,0x12345678 },
      // s_option_analog_input analog_input[IO_05_07_ANALOG_INPUT];
      { 
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  0
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  1
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  2
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  3
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  4
      },
      { // s_option_analog_output analog_output[IO_05_07_ANALOG_OUTPUT];
        {0,0,0,0,0x3FFF,0,ANA_OUT_EMPTY}, // 0
        {0,0,0,0,0x3FFF,0,ANA_OUT_EMPTY}, // 1
      },
      // s_option_digital_output digital_output;
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // 0
      // s_option_RS485_bus RS485_bus;
      {0,br9600,0,7,0,REFRESH_TIME_100MS_BASE,1500,0,0},
      1, // unsigned char alarm;
    },
    { // IO_05_07[13]
      // s_option_board_component board_component;
      { 0,0x12345678 },
      // s_option_analog_input analog_input[IO_05_07_ANALOG_INPUT];
      { 
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  0
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  1
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  2
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  3
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  4
      },
      { // s_option_analog_output analog_output[IO_05_07_ANALOG_OUTPUT];
        {0,0,0,0,0x3FFF,0,ANA_OUT_EMPTY}, // 0
        {0,0,0,0,0x3FFF,0,ANA_OUT_EMPTY}, // 1
      },
      // s_option_digital_output digital_output;
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // 0
      // s_option_RS485_bus RS485_bus;
      {0,br9600,0,7,0,REFRESH_TIME_100MS_BASE,1500,0,0},
      1, // unsigned char alarm;
    },
    { // IO_05_07[14]
      // s_option_board_component board_component;
      { 0,0x12345678 },
      // s_option_analog_input analog_input[IO_05_07_ANALOG_INPUT];
      { 
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  0
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  1
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  2
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  3
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  4
      },
      { // s_option_analog_output analog_output[IO_05_07_ANALOG_OUTPUT];
        {0,0,0,0,0x3FFF,0,ANA_OUT_EMPTY}, // 0
        {0,0,0,0,0x3FFF,0,ANA_OUT_EMPTY}, // 1
      },
      // s_option_digital_output digital_output;
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // 0
      // s_option_RS485_bus RS485_bus;
      {0,br9600,0,7,0,REFRESH_TIME_100MS_BASE,1500,0,0},
      1, // unsigned char alarm;
    },
    { // IO_05_07[15]
      // s_option_board_component board_component;
      { 0,0x12345678 },
      // s_option_analog_input analog_input[IO_05_07_ANALOG_INPUT];
      { 
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  0
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  1
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  2
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  3
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  4
      },
      { // s_option_analog_output analog_output[IO_05_07_ANALOG_OUTPUT];
        {0,0,0,0,0x3FFF,0,ANA_OUT_EMPTY}, // 0
        {0,0,0,0,0x3FFF,0,ANA_OUT_EMPTY}, // 1
      },
      // s_option_digital_output digital_output;
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // 0
      // s_option_RS485_bus RS485_bus;
      {0,br9600,0,7,0,REFRESH_TIME_100MS_BASE,1500,0,0},
      1, // unsigned char alarm;
    },
  },

  { // s_option_IO_06_14  IO_06_14[IO_06_14_MAX];
    { // IO_06_14[0]
      // board component
      { 0,0x12345678 },
      // analog input
      { 
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  0
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  1
      },
      // digital input
      {
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  0
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  1
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  2
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  3
      },                       
      // analog output
      {0,0,0,0,0x3FFF,0,ANA_OUT_EMPTY}, // 0
      // digital output
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // 0
      {0,0}, // int offset_windrichting[IO_06_14_ANALOG_INPUT]; 
      1, // unsigned char alarm;
    },
    { // IO_06_14[1]
      // board component
      { 0,0x12345678 },
      // analog input
      { 
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  0
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  1
      },
      // digital input
      {
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  0
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  1
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  2
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  3
      },                       
      // analog output
      {0,0,0,0,0x3FFF,0,ANA_OUT_EMPTY}, // 0
      // digital output
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // 0
      {0,0}, // int offset_windrichting[IO_06_14_ANALOG_INPUT]; 
      1, // unsigned char alarm;
    },
    { // IO_06_14[2]
      // board component
      { 0,0x12345678 },
      // analog input
      { 
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  0
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  1
      },
      // digital input
      {
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  0
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  1
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  2
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  3
      },                       
      // analog output
      {0,0,0,0,0x3FFF,0,ANA_OUT_EMPTY}, // 0
      // digital output
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, 
      {0,0}, // int offset_windrichting[IO_06_14_ANALOG_INPUT]; 
      1, // unsigned char alarm;
    },
    { // IO_06_14[3]
      // board component
      { 0,0x12345678 },
      // analog input
      { 
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  0
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  1
      },
      // digital input
      {
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  0
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  1
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  2
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  3
      },                       
      // analog output
      {0,0,0,0,0x3FFF,0,ANA_OUT_EMPTY}, // 0
      // digital output
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // 0
      {0,0}, // int offset_windrichting[IO_06_14_ANALOG_INPUT]; 
      1, // unsigned char alarm;
    },
  },

  { // s_option_IO_07_07  IO_07_07[IO_07_07_MAX];
    { // IO_07_07[0]
      // s_option_board_component board_component;
      { 0,0x12345678 },
      // s_option_analog_input analog_input[IO_07_07_ANALOG_INPUT];
      { 
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  0
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  1
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  2
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  3
      },
      // s_option_digital_input digital_input[IO_07_07_DIGITAL_INPUT];
      {
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  0
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  1
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  2
      },                       
      { // s_option_analog_output analog_output[IO_07_07_ANALOG_OUTPUT];
        {0,0,0,0,0x3FFF,0,ANA_OUT_EMPTY}, // 0
        {0,0,0,0,0x3FFF,0,ANA_OUT_EMPTY}, // 1
      },
      // s_option_digital_output digital_output;
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // 0
      // s_option_RS485_bus RS485_bus;
      {0,br9600,0,7,0,REFRESH_TIME_100MS_BASE,1500,0,0},
      1, // unsigned char alarm;
    },
    { // IO_07_07[1]
      // s_option_board_component board_component;
      { 0,0x12345678 },
      // s_option_analog_input analog_input[IO_07_07_ANALOG_INPUT];
      { 
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  0
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  1
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  2
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  3
      },
      // s_option_digital_input digital_input[IO_07_07_DIGITAL_INPUT];
      {
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  0
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  1
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  2
      },                       
      { // s_option_analog_output analog_output[IO_07_07_ANALOG_OUTPUT];
        {0,0,0,0,0x3FFF,0,ANA_OUT_EMPTY}, // 0
        {0,0,0,0,0x3FFF,0,ANA_OUT_EMPTY}, // 1
      },
      // s_option_digital_output digital_output;
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // 0
      // s_option_RS485_bus RS485_bus;
      {0,br9600,0,7,0,REFRESH_TIME_100MS_BASE,1500,0,0},
      1, // unsigned char alarm;
    },
    { // IO_07_07[2]
      // s_option_board_component board_component;
      { 0,0x12345678 },
      // s_option_analog_input analog_input[IO_07_07_ANALOG_INPUT];
      { 
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  0
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  1
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  2
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  3
      },
      // s_option_digital_input digital_input[IO_07_07_DIGITAL_INPUT];
      {
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  0
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  1
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  2
      },                       
      { // s_option_analog_output analog_output[IO_07_07_ANALOG_OUTPUT];
        {0,0,0,0,0x3FFF,0,ANA_OUT_EMPTY}, // 0
        {0,0,0,0,0x3FFF,0,ANA_OUT_EMPTY}, // 1
      },
      // s_option_digital_output digital_output;
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // 0
      // s_option_RS485_bus RS485_bus;
      {0,br9600,0,7,0,REFRESH_TIME_100MS_BASE,1500,0,0},
      1, // unsigned char alarm;
    },
    { // IO_07_07[3]
      // s_option_board_component board_component;
      { 0,0x12345678 },
      // s_option_analog_input analog_input[IO_07_07_ANALOG_INPUT];
      { 
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  0
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  1
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  2
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  3
      },
      // s_option_digital_input digital_input[IO_07_07_DIGITAL_INPUT];
      {
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  0
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  1
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  2
      },                       
      { // s_option_analog_output analog_output[IO_07_07_ANALOG_OUTPUT];
        {0,0,0,0,0x3FFF,0,ANA_OUT_EMPTY}, // 0
        {0,0,0,0,0x3FFF,0,ANA_OUT_EMPTY}, // 1
      },
      // s_option_digital_output digital_output;
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // 0
      // s_option_RS485_bus RS485_bus;
      {0,br9600,0,7,0,REFRESH_TIME_100MS_BASE,1500,0,0},
      1, // unsigned char alarm;
    },
    { // IO_07_07[4]
      // s_option_board_component board_component;
      { 0,0x12345678 },
      // s_option_analog_input analog_input[IO_07_07_ANALOG_INPUT];
      { 
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  0
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  1
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  2
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  3
      },
      // s_option_digital_input digital_input[IO_07_07_DIGITAL_INPUT];
      {
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  0
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  1
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  2
      },                       
      { // s_option_analog_output analog_output[IO_07_07_ANALOG_OUTPUT];
        {0,0,0,0,0x3FFF,0,ANA_OUT_EMPTY}, // 0
        {0,0,0,0,0x3FFF,0,ANA_OUT_EMPTY}, // 1
      },
      // s_option_digital_output digital_output;
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // 0
      // s_option_RS485_bus RS485_bus;
      {0,br9600,0,7,0,REFRESH_TIME_100MS_BASE,1500,0,0},
      1, // unsigned char alarm;
    },
    { // IO_07_07[5]
      // s_option_board_component board_component;
      { 0,0x12345678 },
      // s_option_analog_input analog_input[IO_07_07_ANALOG_INPUT];
      { 
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  0
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  1
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  2
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  3
      },
      // s_option_digital_input digital_input[IO_07_07_DIGITAL_INPUT];
      {
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  0
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  1
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  2
      },                       
      { // s_option_analog_output analog_output[IO_07_07_ANALOG_OUTPUT];
        {0,0,0,0,0x3FFF,0,ANA_OUT_EMPTY}, // 0
        {0,0,0,0,0x3FFF,0,ANA_OUT_EMPTY}, // 1
      },
      // s_option_digital_output digital_output;
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // 0
      // s_option_RS485_bus RS485_bus;
      {0,br9600,0,7,0,REFRESH_TIME_100MS_BASE,1500,0,0},
      1, // unsigned char alarm;
    },
    { // IO_07_07[6]
      // s_option_board_component board_component;
      { 0,0x12345678 },
      // s_option_analog_input analog_input[IO_07_07_ANALOG_INPUT];
      { 
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  0
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  1
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  2
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  3
      },
      // s_option_digital_input digital_input[IO_07_07_DIGITAL_INPUT];
      {
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  0
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  1
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  2
      },                       
      { // s_option_analog_output analog_output[IO_07_07_ANALOG_OUTPUT];
        {0,0,0,0,0x3FFF,0,ANA_OUT_EMPTY}, // 0
        {0,0,0,0,0x3FFF,0,ANA_OUT_EMPTY}, // 1
      },
      // s_option_digital_output digital_output;
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // 0
      // s_option_RS485_bus RS485_bus;
      {0,br9600,0,7,0,REFRESH_TIME_100MS_BASE,1500,0,0},
      1, // unsigned char alarm;
    },
    { // IO_07_07[7]
      // s_option_board_component board_component;
      { 0,0x12345678 },
      // s_option_analog_input analog_input[IO_07_07_ANALOG_INPUT];
      { 
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  0
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  1
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  2
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  3
      },
      // s_option_digital_input digital_input[IO_07_07_DIGITAL_INPUT];
      {
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  0
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  1
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  2
      },                       
      { // s_option_analog_output analog_output[IO_07_07_ANALOG_OUTPUT];
        {0,0,0,0,0x3FFF,0,ANA_OUT_EMPTY}, // 0
        {0,0,0,0,0x3FFF,0,ANA_OUT_EMPTY}, // 1
      },
      // s_option_digital_output digital_output;
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // 0
      // s_option_RS485_bus RS485_bus;
      {0,br9600,0,7,0,REFRESH_TIME_100MS_BASE,1500,0,0},
      1, // unsigned char alarm;
    },
    { // IO_07_07[8]
      // s_option_board_component board_component;
      { 0,0x12345678 },
      // s_option_analog_input analog_input[IO_07_07_ANALOG_INPUT];
      { 
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  0
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  1
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  2
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  3
      },
      // s_option_digital_input digital_input[IO_07_07_DIGITAL_INPUT];
      {
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  0
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  1
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  2
      },                       
      { // s_option_analog_output analog_output[IO_07_07_ANALOG_OUTPUT];
        {0,0,0,0,0x3FFF,0,ANA_OUT_EMPTY}, // 0
        {0,0,0,0,0x3FFF,0,ANA_OUT_EMPTY}, // 1
      },
      // s_option_digital_output digital_output;
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // 0
      // s_option_RS485_bus RS485_bus;
      {0,br9600,0,7,0,REFRESH_TIME_100MS_BASE,1500,0,0},
      1, // unsigned char alarm;
    },
    { // IO_07_07[9]
      // s_option_board_component board_component;
      { 0,0x12345678 },
      // s_option_analog_input analog_input[IO_07_07_ANALOG_INPUT];
      { 
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  0
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  1
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  2
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  3
      },
      // s_option_digital_input digital_input[IO_07_07_DIGITAL_INPUT];
      {
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  0
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  1
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  2
      },                       
      { // s_option_analog_output analog_output[IO_07_07_ANALOG_OUTPUT];
        {0,0,0,0,0x3FFF,0,ANA_OUT_EMPTY}, // 0
        {0,0,0,0,0x3FFF,0,ANA_OUT_EMPTY}, // 1
      },
      // s_option_digital_output digital_output;
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // 0
      // s_option_RS485_bus RS485_bus;
      {0,br9600,0,7,0,REFRESH_TIME_100MS_BASE,1500,0,0},
      1, // unsigned char alarm;
    },
    { // IO_07_07[10]
      // s_option_board_component board_component;
      { 0,0x12345678 },
      // s_option_analog_input analog_input[IO_07_07_ANALOG_INPUT];
      { 
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  0
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  1
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  2
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  3
      },
      // s_option_digital_input digital_input[IO_07_07_DIGITAL_INPUT];
      {
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  0
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  1
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  2
      },                       
      { // s_option_analog_output analog_output[IO_07_07_ANALOG_OUTPUT];
        {0,0,0,0,0x3FFF,0,ANA_OUT_EMPTY}, // 0
        {0,0,0,0,0x3FFF,0,ANA_OUT_EMPTY}, // 1
      },
      // s_option_digital_output digital_output;
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // 0
      // s_option_RS485_bus RS485_bus;
      {0,br9600,0,7,0,REFRESH_TIME_100MS_BASE,1500,0,0},
      1, // unsigned char alarm;
    },
    { // IO_07_07[11]
      // s_option_board_component board_component;
      { 0,0x12345678 },
      // s_option_analog_input analog_input[IO_07_07_ANALOG_INPUT];
      { 
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  0
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  1
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  2
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  3
      },
      // s_option_digital_input digital_input[IO_07_07_DIGITAL_INPUT];
      {
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  0
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  1
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  2
      },                       
      { // s_option_analog_output analog_output[IO_07_07_ANALOG_OUTPUT];
        {0,0,0,0,0x3FFF,0,ANA_OUT_EMPTY}, // 0
        {0,0,0,0,0x3FFF,0,ANA_OUT_EMPTY}, // 1
      },
      // s_option_digital_output digital_output;
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // 0
      // s_option_RS485_bus RS485_bus;
      {0,br9600,0,7,0,REFRESH_TIME_100MS_BASE,1500,0,0},
      1, // unsigned char alarm;
    },
    { // IO_07_07[12]
      // s_option_board_component board_component;
      { 0,0x12345678 },
      // s_option_analog_input analog_input[IO_07_07_ANALOG_INPUT];
      { 
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  0
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  1
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  2
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  3
      },
      // s_option_digital_input digital_input[IO_07_07_DIGITAL_INPUT];
      {
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  0
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  1
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  2
      },                       
      { // s_option_analog_output analog_output[IO_07_07_ANALOG_OUTPUT];
        {0,0,0,0,0x3FFF,0,ANA_OUT_EMPTY}, // 0
        {0,0,0,0,0x3FFF,0,ANA_OUT_EMPTY}, // 1
      },
      // s_option_digital_output digital_output;
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // 0
      // s_option_RS485_bus RS485_bus;
      {0,br9600,0,7,0,REFRESH_TIME_100MS_BASE,1500,0,0},
      1, // unsigned char alarm;
    },
    { // IO_07_07[13]
      // s_option_board_component board_component;
      { 0,0x12345678 },
      // s_option_analog_input analog_input[IO_07_07_ANALOG_INPUT];
      { 
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  0
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  1
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  2
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  3
      },
      // s_option_digital_input digital_input[IO_07_07_DIGITAL_INPUT];
      {
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  0
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  1
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  2
      },                       
      { // s_option_analog_output analog_output[IO_07_07_ANALOG_OUTPUT];
        {0,0,0,0,0x3FFF,0,ANA_OUT_EMPTY}, // 0
        {0,0,0,0,0x3FFF,0,ANA_OUT_EMPTY}, // 1
      },
      // s_option_digital_output digital_output;
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // 0
      // s_option_RS485_bus RS485_bus;
      {0,br9600,0,7,0,REFRESH_TIME_100MS_BASE,1500,0,0},
      1, // unsigned char alarm;
    },
    { // IO_07_07[14]
      // s_option_board_component board_component;
      { 0,0x12345678 },
      // s_option_analog_input analog_input[IO_07_07_ANALOG_INPUT];
      { 
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  0
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  1
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  2
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  3
      },
      // s_option_digital_input digital_input[IO_07_07_DIGITAL_INPUT];
      {
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  0
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  1
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  2
      },                       
      { // s_option_analog_output analog_output[IO_07_07_ANALOG_OUTPUT];
        {0,0,0,0,0x3FFF,0,ANA_OUT_EMPTY}, // 0
        {0,0,0,0,0x3FFF,0,ANA_OUT_EMPTY}, // 1
      },
      // s_option_digital_output digital_output;
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // 0
      // s_option_RS485_bus RS485_bus;
      {0,br9600,0,7,0,REFRESH_TIME_100MS_BASE,1500,0,0},
      1, // unsigned char alarm;
    },
    { // IO_07_07[15]
      // s_option_board_component board_component;
      { 0,0x12345678 },
      // s_option_analog_input analog_input[IO_07_07_ANALOG_INPUT];
      { 
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  0
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  1
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  2
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  3
      },
      // s_option_digital_input digital_input[IO_07_07_DIGITAL_INPUT];
      {
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  0
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  1
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  2
      },                       
      { // s_option_analog_output analog_output[IO_07_07_ANALOG_OUTPUT];
        {0,0,0,0,0x3FFF,0,ANA_OUT_EMPTY}, // 0
        {0,0,0,0,0x3FFF,0,ANA_OUT_EMPTY}, // 1
      },
      // s_option_digital_output digital_output;
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // 0
      // s_option_RS485_bus RS485_bus;
      {0,br9600,0,7,0,REFRESH_TIME_100MS_BASE,1500,0,0},
      1, // unsigned char alarm;
    },
    { // IO_07_07[16]
      // s_option_board_component board_component;
      { 0,0x12345678 },
      // s_option_analog_input analog_input[IO_07_07_ANALOG_INPUT];
      { 
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  0
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  1
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  2
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  3
      },
      // s_option_digital_input digital_input[IO_07_07_DIGITAL_INPUT];
      {
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  0
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  1
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  2
      },                       
      { // s_option_analog_output analog_output[IO_07_07_ANALOG_OUTPUT];
        {0,0,0,0,0x3FFF,0,ANA_OUT_EMPTY}, // 0
        {0,0,0,0,0x3FFF,0,ANA_OUT_EMPTY}, // 1
      },
      // s_option_digital_output digital_output;
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // 0
      // s_option_RS485_bus RS485_bus;
      {0,br9600,0,7,0,REFRESH_TIME_100MS_BASE,1500,0,0},
      1, // unsigned char alarm;
    },
    { // IO_07_07[17]
      // s_option_board_component board_component;
      { 0,0x12345678 },
      // s_option_analog_input analog_input[IO_07_07_ANALOG_INPUT];
      { 
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  0
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  1
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  2
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  3
      },
      // s_option_digital_input digital_input[IO_07_07_DIGITAL_INPUT];
      {
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  0
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  1
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  2
      },                       
      { // s_option_analog_output analog_output[IO_07_07_ANALOG_OUTPUT];
        {0,0,0,0,0x3FFF,0,ANA_OUT_EMPTY}, // 0
        {0,0,0,0,0x3FFF,0,ANA_OUT_EMPTY}, // 1
      },
      // s_option_digital_output digital_output;
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // 0
      // s_option_RS485_bus RS485_bus;
      {0,br9600,0,7,0,REFRESH_TIME_100MS_BASE,1500,0,0},
      1, // unsigned char alarm;
    },
    { // IO_07_07[18]
      // s_option_board_component board_component;
      { 0,0x12345678 },
      // s_option_analog_input analog_input[IO_07_07_ANALOG_INPUT];
      { 
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  0
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  1
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  2
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  3
      },
      // s_option_digital_input digital_input[IO_07_07_DIGITAL_INPUT];
      {
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  0
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  1
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  2
      },                       
      { // s_option_analog_output analog_output[IO_07_07_ANALOG_OUTPUT];
        {0,0,0,0,0x3FFF,0,ANA_OUT_EMPTY}, // 0
        {0,0,0,0,0x3FFF,0,ANA_OUT_EMPTY}, // 1
      },
      // s_option_digital_output digital_output;
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // 0
      // s_option_RS485_bus RS485_bus;
      {0,br9600,0,7,0,REFRESH_TIME_100MS_BASE,1500,0,0},
      1, // unsigned char alarm;
    },
    { // IO_07_07[19]
      // s_option_board_component board_component;
      { 0,0x12345678 },
      // s_option_analog_input analog_input[IO_07_07_ANALOG_INPUT];
      { 
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  0
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  1
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  2
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  3
      },
      // s_option_digital_input digital_input[IO_07_07_DIGITAL_INPUT];
      {
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  0
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  1
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  2
      },                       
      { // s_option_analog_output analog_output[IO_07_07_ANALOG_OUTPUT];
        {0,0,0,0,0x3FFF,0,ANA_OUT_EMPTY}, // 0
        {0,0,0,0,0x3FFF,0,ANA_OUT_EMPTY}, // 1
      },
      // s_option_digital_output digital_output;
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // 0
      // s_option_RS485_bus RS485_bus;
      {0,br9600,0,7,0,REFRESH_TIME_100MS_BASE,1500,0,0},
      1, // unsigned char alarm;
    },
    { // IO_07_07[20]
      // s_option_board_component board_component;
      { 0,0x12345678 },
      // s_option_analog_input analog_input[IO_07_07_ANALOG_INPUT];
      { 
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  0
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  1
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  2
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  3
      },
      // s_option_digital_input digital_input[IO_07_07_DIGITAL_INPUT];
      {
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  0
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  1
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  2
      },                       
      { // s_option_analog_output analog_output[IO_07_07_ANALOG_OUTPUT];
        {0,0,0,0,0x3FFF,0,ANA_OUT_EMPTY}, // 0
        {0,0,0,0,0x3FFF,0,ANA_OUT_EMPTY}, // 1
      },
      // s_option_digital_output digital_output;
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // 0
      // s_option_RS485_bus RS485_bus;
      {0,br9600,0,7,0,REFRESH_TIME_100MS_BASE,1500,0,0},
      1, // unsigned char alarm;
    },
    { // IO_07_07[21]
      // s_option_board_component board_component;
      { 0,0x12345678 },
      // s_option_analog_input analog_input[IO_07_07_ANALOG_INPUT];
      { 
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  0
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  1
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  2
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  3
      },
      // s_option_digital_input digital_input[IO_07_07_DIGITAL_INPUT];
      {
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  0
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  1
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  2
      },                       
      { // s_option_analog_output analog_output[IO_07_07_ANALOG_OUTPUT];
        {0,0,0,0,0x3FFF,0,ANA_OUT_EMPTY}, // 0
        {0,0,0,0,0x3FFF,0,ANA_OUT_EMPTY}, // 1
      },
      // s_option_digital_output digital_output;
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // 0
      // s_option_RS485_bus RS485_bus;
      {0,br9600,0,7,0,REFRESH_TIME_100MS_BASE,1500,0,0},
      1, // unsigned char alarm;
    },
    { // IO_07_07[22]
      // s_option_board_component board_component;
      { 0,0x12345678 },
      // s_option_analog_input analog_input[IO_07_07_ANALOG_INPUT];
      { 
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  0
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  1
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  2
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  3
      },
      // s_option_digital_input digital_input[IO_07_07_DIGITAL_INPUT];
      {
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  0
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  1
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  2
      },                       
      { // s_option_analog_output analog_output[IO_07_07_ANALOG_OUTPUT];
        {0,0,0,0,0x3FFF,0,ANA_OUT_EMPTY}, // 0
        {0,0,0,0,0x3FFF,0,ANA_OUT_EMPTY}, // 1
      },
      // s_option_digital_output digital_output;
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // 0
      // s_option_RS485_bus RS485_bus;
      {0,br9600,0,7,0,REFRESH_TIME_100MS_BASE,1500,0,0},
      1, // unsigned char alarm;
    },
    { // IO_07_07[23]
      // s_option_board_component board_component;
      { 0,0x12345678 },
      // s_option_analog_input analog_input[IO_07_07_ANALOG_INPUT];
      { 
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  0
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  1
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  2
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  3
      },
      // s_option_digital_input digital_input[IO_07_07_DIGITAL_INPUT];
      {
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  0
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  1
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  2
      },                       
      { // s_option_analog_output analog_output[IO_07_07_ANALOG_OUTPUT];
        {0,0,0,0,0x3FFF,0,ANA_OUT_EMPTY}, // 0
        {0,0,0,0,0x3FFF,0,ANA_OUT_EMPTY}, // 1
      },
      // s_option_digital_output digital_output;
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // 0
      // s_option_RS485_bus RS485_bus;
      {0,br9600,0,7,0,REFRESH_TIME_100MS_BASE,1500,0,0},
      1, // unsigned char alarm;
    },
    { // IO_07_07[24]
      // s_option_board_component board_component;
      { 0,0x12345678 },
      // s_option_analog_input analog_input[IO_07_07_ANALOG_INPUT];
      { 
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  0
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  1
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  2
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  3
      },
      // s_option_digital_input digital_input[IO_07_07_DIGITAL_INPUT];
      {
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  0
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  1
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  2
      },                       
      { // s_option_analog_output analog_output[IO_07_07_ANALOG_OUTPUT];
        {0,0,0,0,0x3FFF,0,ANA_OUT_EMPTY}, // 0
        {0,0,0,0,0x3FFF,0,ANA_OUT_EMPTY}, // 1
      },
      // s_option_digital_output digital_output;
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // 0
      // s_option_RS485_bus RS485_bus;
      {0,br9600,0,7,0,REFRESH_TIME_100MS_BASE,1500,0,0},
      1, // unsigned char alarm;
    },
    { // IO_07_07[25]
      // s_option_board_component board_component;
      { 0,0x12345678 },
      // s_option_analog_input analog_input[IO_07_07_ANALOG_INPUT];
      { 
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  0
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  1
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  2
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  3
      },
      // s_option_digital_input digital_input[IO_07_07_DIGITAL_INPUT];
      {
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  0
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  1
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  2
      },                       
      { // s_option_analog_output analog_output[IO_07_07_ANALOG_OUTPUT];
        {0,0,0,0,0x3FFF,0,ANA_OUT_EMPTY}, // 0
        {0,0,0,0,0x3FFF,0,ANA_OUT_EMPTY}, // 1
      },
      // s_option_digital_output digital_output;
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // 0
      // s_option_RS485_bus RS485_bus;
      {0,br9600,0,7,0,REFRESH_TIME_100MS_BASE,1500,0,0},
      1, // unsigned char alarm;
    },
    { // IO_07_07[26]
      // s_option_board_component board_component;
      { 0,0x12345678 },
      // s_option_analog_input analog_input[IO_07_07_ANALOG_INPUT];
      { 
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  0
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  1
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  2
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  3
      },
      // s_option_digital_input digital_input[IO_07_07_DIGITAL_INPUT];
      {
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  0
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  1
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  2
      },                       
      { // s_option_analog_output analog_output[IO_07_07_ANALOG_OUTPUT];
        {0,0,0,0,0x3FFF,0,ANA_OUT_EMPTY}, // 0
        {0,0,0,0,0x3FFF,0,ANA_OUT_EMPTY}, // 1
      },
      // s_option_digital_output digital_output;
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // 0
      // s_option_RS485_bus RS485_bus;
      {0,br9600,0,7,0,REFRESH_TIME_100MS_BASE,1500,0,0},
      1, // unsigned char alarm;
    },
    { // IO_07_07[27]
      // s_option_board_component board_component;
      { 0,0x12345678 },
      // s_option_analog_input analog_input[IO_07_07_ANALOG_INPUT];
      { 
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  0
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  1
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  2
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  3
      },
      // s_option_digital_input digital_input[IO_07_07_DIGITAL_INPUT];
      {
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  0
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  1
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  2
      },                       
      { // s_option_analog_output analog_output[IO_07_07_ANALOG_OUTPUT];
        {0,0,0,0,0x3FFF,0,ANA_OUT_EMPTY}, // 0
        {0,0,0,0,0x3FFF,0,ANA_OUT_EMPTY}, // 1
      },
      // s_option_digital_output digital_output;
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // 0
      // s_option_RS485_bus RS485_bus;
      {0,br9600,0,7,0,REFRESH_TIME_100MS_BASE,1500,0,0},
      1, // unsigned char alarm;
    },
    { // IO_07_07[28]
      // s_option_board_component board_component;
      { 0,0x12345678 },
      // s_option_analog_input analog_input[IO_07_07_ANALOG_INPUT];
      { 
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  0
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  1
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  2
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  3
      },
      // s_option_digital_input digital_input[IO_07_07_DIGITAL_INPUT];
      {
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  0
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  1
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  2
      },                       
      { // s_option_analog_output analog_output[IO_07_07_ANALOG_OUTPUT];
        {0,0,0,0,0x3FFF,0,ANA_OUT_EMPTY}, // 0
        {0,0,0,0,0x3FFF,0,ANA_OUT_EMPTY}, // 1
      },
      // s_option_digital_output digital_output;
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // 0
      // s_option_RS485_bus RS485_bus;
      {0,br9600,0,7,0,REFRESH_TIME_100MS_BASE,1500,0,0},
      1, // unsigned char alarm;
    },
    { // IO_07_07[29]
      // s_option_board_component board_component;
      { 0,0x12345678 },
      // s_option_analog_input analog_input[IO_07_07_ANALOG_INPUT];
      { 
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  0
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  1
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  2
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  3
      },
      // s_option_digital_input digital_input[IO_07_07_DIGITAL_INPUT];
      {
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  0
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  1
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  2
      },                       
      { // s_option_analog_output analog_output[IO_07_07_ANALOG_OUTPUT];
        {0,0,0,0,0x3FFF,0,ANA_OUT_EMPTY}, // 0
        {0,0,0,0,0x3FFF,0,ANA_OUT_EMPTY}, // 1
      },
      // s_option_digital_output digital_output;
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // 0
      // s_option_RS485_bus RS485_bus;
      {0,br9600,0,7,0,REFRESH_TIME_100MS_BASE,1500,0,0},
      1, // unsigned char alarm;
    },
    { // IO_07_07[30]
      // s_option_board_component board_component;
      { 0,0x12345678 },
      // s_option_analog_input analog_input[IO_07_07_ANALOG_INPUT];
      { 
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  0
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  1
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  2
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  3
      },
      // s_option_digital_input digital_input[IO_07_07_DIGITAL_INPUT];
      {
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  0
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  1
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  2
      },                       
      { // s_option_analog_output analog_output[IO_07_07_ANALOG_OUTPUT];
        {0,0,0,0,0x3FFF,0,ANA_OUT_EMPTY}, // 0
        {0,0,0,0,0x3FFF,0,ANA_OUT_EMPTY}, // 1
      },
      // s_option_digital_output digital_output;
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // 0
      // s_option_RS485_bus RS485_bus;
      {0,br9600,0,7,0,REFRESH_TIME_100MS_BASE,1500,0,0},
      1, // unsigned char alarm;
    },
    { // IO_07_07[31]
      // s_option_board_component board_component;
      { 0,0x12345678 },
      // s_option_analog_input analog_input[IO_07_07_ANALOG_INPUT];
      { 
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  0
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  1
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  2
        {0,0,0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  3
      },
      // s_option_digital_input digital_input[IO_07_07_DIGITAL_INPUT];
      {
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  0
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  1
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  2
      },                       
      { // s_option_analog_output analog_output[IO_07_07_ANALOG_OUTPUT];
        {0,0,0,0,0x3FFF,0,ANA_OUT_EMPTY}, // 0
        {0,0,0,0,0x3FFF,0,ANA_OUT_EMPTY}, // 1
      },
      // s_option_digital_output digital_output;
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // 0
      // s_option_RS485_bus RS485_bus;
      {0,br9600,0,7,0,REFRESH_TIME_100MS_BASE,1500,0,0},
      1, // unsigned char alarm;
    },
  },

  { // s_option_IO_08_09 IO_08_09[IO_08_09_MAX];
    { // IO_08_09[0]
      // board component
      { 1,0x12345678 },
      // digital input
      {
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  0
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  1
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  2
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  3
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  4
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  5
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  6
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  7
      },                       
      { // analog_output
        {0,0,0,0,0x3FFF,0,ANA_OUT_EMPTY}, // 0
        {0,0,0,0,0x3FFF,0,ANA_OUT_EMPTY}, // 1
        {0,0,0,0,0x3FFF,0,ANA_OUT_EMPTY}, // 2
        {0,0,0,0,0x3FFF,0,ANA_OUT_EMPTY}, // 3
      },
      // digital output
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // 0
      1, // unsigned char alarm;
    },
    { // IO_08_09[1]
      // board component
      { 0,0x12345678 },
      // digital input
      {
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  0
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  1
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  2
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  3
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  4
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  5
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  6
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  7
      },                       
      { // analog_output
        {0,0,0,0,0x3FFF,0,ANA_OUT_EMPTY}, // 0
        {0,0,0,0,0x3FFF,0,ANA_OUT_EMPTY}, // 1
        {0,0,0,0,0x3FFF,0,ANA_OUT_EMPTY}, // 2
        {0,0,0,0,0x3FFF,0,ANA_OUT_EMPTY}, // 3
      },
      // digital output
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // 0
      1, // unsigned char alarm;
    },
    { // IO_08_09[2]
      // board component
      { 0,0x12345678 },
      // digital input
      {
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  0
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  1
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  2
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  3
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  4
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  5
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  6
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  7
      },                       
      { // analog_output
        {0,0,0,0,0x3FFF,0,ANA_OUT_EMPTY}, // 0
        {0,0,0,0,0x3FFF,0,ANA_OUT_EMPTY}, // 1
        {0,0,0,0,0x3FFF,0,ANA_OUT_EMPTY}, // 2
        {0,0,0,0,0x3FFF,0,ANA_OUT_EMPTY}, // 3
      },
      // digital output
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // 0
      1, // unsigned char alarm;
    },
    { // IO_08_09[3]
      // board component
      { 0,0x12345678 },
      // digital input
      {
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  0
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  1
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  2
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  3
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  4
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  5
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  6
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  7
      },                       
      { // analog_output
        {0,0,0,0,0x3FFF,0,ANA_OUT_EMPTY}, // 0
        {0,0,0,0,0x3FFF,0,ANA_OUT_EMPTY}, // 1
        {0,0,0,0,0x3FFF,0,ANA_OUT_EMPTY}, // 2
        {0,0,0,0,0x3FFF,0,ANA_OUT_EMPTY}, // 3
      },
      // digital output
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // 0
      1, // unsigned char alarm;
    },
    { // IO_08_09[4]
      // board component
      { 0,0x12345678 },
      // digital input
      {
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  0
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  1
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  2
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  3
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  4
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  5
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  6
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  7
      },                       
      { // analog_output
        {0,0,0,0,0x3FFF,0,ANA_OUT_EMPTY}, // 0
        {0,0,0,0,0x3FFF,0,ANA_OUT_EMPTY}, // 1
        {0,0,0,0,0x3FFF,0,ANA_OUT_EMPTY}, // 2
        {0,0,0,0,0x3FFF,0,ANA_OUT_EMPTY}, // 3
      },
      // digital output
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // 0
      1, // unsigned char alarm;
    },
    { // IO_08_09[5]
      // board component
      { 0,0x12345678 },
      // digital input
      {
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  0
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  1
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  2
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  3
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  4
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  5
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  6
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  7
      },                       
      { // analog_output
        {0,0,0,0,0x3FFF,0,ANA_OUT_EMPTY}, // 0
        {0,0,0,0,0x3FFF,0,ANA_OUT_EMPTY}, // 1
        {0,0,0,0,0x3FFF,0,ANA_OUT_EMPTY}, // 2
        {0,0,0,0,0x3FFF,0,ANA_OUT_EMPTY}, // 3
      },
      // digital output
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // 0
      1, // unsigned char alarm;
    },
    { // IO_08_09[6]
      // board component
      { 0,0x12345678 },
      // digital input
      {
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  0
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  1
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  2
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  3
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  4
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  5
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  6
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  7
      },                       
      { // analog_output
        {0,0,0,0,0x3FFF,0,ANA_OUT_EMPTY}, // 0
        {0,0,0,0,0x3FFF,0,ANA_OUT_EMPTY}, // 1
        {0,0,0,0,0x3FFF,0,ANA_OUT_EMPTY}, // 2
        {0,0,0,0,0x3FFF,0,ANA_OUT_EMPTY}, // 3
      },
      // digital output
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // 0
      1, // unsigned char alarm;
    },
    { // IO_08_09[7]
      // board component
      { 0,0x12345678 },
      // digital input
      {
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  0
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  1
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  2
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  3
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  4
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  5
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  6
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  7
      },                       
      { // analog_output
        {0,0,0,0,0x3FFF,0,ANA_OUT_EMPTY}, // 0
        {0,0,0,0,0x3FFF,0,ANA_OUT_EMPTY}, // 1
        {0,0,0,0,0x3FFF,0,ANA_OUT_EMPTY}, // 2
        {0,0,0,0,0x3FFF,0,ANA_OUT_EMPTY}, // 3
      },
      // digital output
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // 0
      1, // unsigned char alarm;
    },
  },

  { // s_option_IO_12_06  IO_12_06[IO_12_06_MAX];
    { // IO_12_06[0]
      // board component
      { 0,0x12345678 },
      // digital input
      {
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  0
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  1
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  2
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  3
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  4
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  5
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  6
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  7
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  8
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  9
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  10
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}  //  11
      },                       
      { // counter_input
        {0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  0
        {0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  1
        {0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  2
        {0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}  //  3
      },
      { // analog_output
        {0,0,0,0,0x3FFF,0,ANA_OUT_EMPTY}, // 0
        {0,0,0,0,0x3FFF,0,ANA_OUT_EMPTY}, // 1
      },
      // digital output
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // 0
      0, // int aantal_counters; // aantal aanwezige counters (aan de hand heir van wordt de counter_input component aangemaakt
      1, // unsigned char alarm;
    },
    { // IO_12_06[1]
      // board component
      { 0,0x12345678 },
      // digital input
      {
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  0
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  1
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  2
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  3
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  4
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  5
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  6
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  7
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  8
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  9
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  10
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}  //  11
      },                       
      { // counter_input
        {0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  0
        {0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  1
        {0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  2
        {0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}  //  3
      },
      { // analog_output
        {0,0,0,0,0x3FFF,0,ANA_OUT_EMPTY}, // 0
        {0,0,0,0,0x3FFF,0,ANA_OUT_EMPTY}, // 1
      },
      // digital output
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, 
      0, // int aantal_counters; // aantal aanwezige counters (aan de hand heir van wordt de counter_input component aangemaakt
      1, // unsigned char alarm;
    },
    { // IO_12_06[2]
      // board component
      { 0,0x12345678 },
      // digital input
      {
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  0
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  1
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  2
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  3
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  4
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  5
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  6
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  7
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  8
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  9
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  10
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}  //  11
      },                       
      { // counter_input
        {0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  0
        {0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  1
        {0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  2
        {0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}  //  3
      },
      { // analog_output
        {0,0,0,0,0x3FFF,0,ANA_OUT_EMPTY}, // 0
        {0,0,0,0,0x3FFF,0,ANA_OUT_EMPTY}, // 1
      },
      // digital output
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // 0
      0, // int aantal_counters; // aantal aanwezige counters (aan de hand heir van wordt de counter_input component aangemaakt
      1, // unsigned char alarm;
    },
    { // IO_12_06[3]
      // board component
      { 0,0x12345678 },
      // digital input
      {
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  0
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  1
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  2
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  3
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  4
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  5
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  6
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  7
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  8
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  9
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  10
        {0,0,0,0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}  //  11
      },                       
      { // counter_input
        {0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  0
        {0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  1
        {0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}, //  2
        {0,0,0,0,REFRESH_TIME_100MS_BASE,0,0}  //  3
      },
      { // analog_output
        {0,0,0,0,0x3FFF,0,ANA_OUT_EMPTY}, // 0
        {0,0,0,0,0x3FFF,0,ANA_OUT_EMPTY}, // 1
      },
      // digital output
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, 
      0, // int aantal_counters; // aantal aanwezige counters (aan de hand heir van wordt de counter_input component aangemaakt
      1, // unsigned char alarm;
    }
  },

  { // s_option_IO_H1MC IO_H1MC[IO_H1MC_MAX];
    { // IO_H1MC[0]
      // board component
      { 0,0x12345678 },
      // motor_control
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      1, // unsigned char alarm;
    },
    { // IO_H1MC[1]
      // board component
      { 0,0x12345678 },
      // motor_control
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      1, // unsigned char alarm;
    },
    { // IO_H1MC[2]
      // board component
      { 0,0x12345678 },
      // motor_control
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      1, // unsigned char alarm;
    },
    { // IO_H1MC[3]
      // board component
      { 0,0x12345678 },
      // motor_control
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      1, // unsigned char alarm;
    },
    { // IO_H1MC[4]
      // board component
      { 0,0x12345678 },
      // motor_control
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      1, // unsigned char alarm;
    },
    { // IO_H1MC[5]
      // board component
      { 0,0x12345678 },
      // motor_control
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      1, // unsigned char alarm;
    },
    { // IO_H1MC[6]
      // board component
      { 0,0x12345678 },
      // motor_control
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      1, // unsigned char alarm;
    },
    { // IO_H1MC[7]
      // board component
      { 0,0x12345678 },
      // motor_control
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      1, // unsigned char alarm;
    },
    { // IO_H1MC[8]
      // board component
      { 0,0x12345678 },
      // motor_control
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      1, // unsigned char alarm;
    },
    { // IO_H1MC[9]
      // board component
      { 0,0x12345678 },
      // motor_control
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      1, // unsigned char alarm;
    },
    { // IO_H1MC[10]
      // board component
      { 0,0x12345678 },
      // motor_control
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      1, // unsigned char alarm;
    },
    { // IO_H1MC[11]
      // board component
      { 0,0x12345678 },
      // motor_control
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      1, // unsigned char alarm;
    },
    { // IO_H1MC[12]
      // board component
      { 0,0x12345678 },
      // motor_control
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      1, // unsigned char alarm;
    },
    { // IO_H1MC[13]
      // board component
      { 0,0x12345678 },
      // motor_control
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      1, // unsigned char alarm;
    },
    { // IO_H1MC[14]
      // board component
      { 0,0x12345678 },
      // motor_control
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      1, // unsigned char alarm;
    },
    { // IO_H1MC[15]
      // board component
      { 0,0x12345678 },
      // motor_control
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      1, // unsigned char alarm;
    },
    { // IO_H1MC[16]
      // board component
      { 0,0x12345678 },
      // motor_control
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      1, // unsigned char alarm;
    },
    { // IO_H1MC[17]
      // board component
      { 0,0x12345678 },
      // motor_control
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      1, // unsigned char alarm;
    },
    { // IO_H1MC[18]
      // board component
      { 0,0x12345678 },
      // motor_control
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      1, // unsigned char alarm;
    },
    { // IO_H1MC[19]
      // board component
      { 0,0x12345678 },
      // motor_control
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      1, // unsigned char alarm;
    },
    { // IO_H1MC[20]
      // board component
      { 0,0x12345678 },
      // motor_control
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      1, // unsigned char alarm;
    },
    { // IO_H1MC[21]
      // board component
      { 0,0x12345678 },
      // motor_control
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      1, // unsigned char alarm;
    },
    { // IO_H1MC[22]
      // board component
      { 0,0x12345678 },
      // motor_control
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      1, // unsigned char alarm;
    },
    { // IO_H1MC[23]
      // board component
      { 0,0x12345678 },
      // motor_control
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      1, // unsigned char alarm;
    },
    { // IO_H1MC[24]
      // board component
      { 0,0x12345678 },
      // motor_control
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      1, // unsigned char alarm;
    },
    { // IO_H1MC[25]
      // board component
      { 0,0x12345678 },
      // motor_control
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      1, // unsigned char alarm;
    },
    { // IO_H1MC[26]
      // board component
      { 0,0x12345678 },
      // motor_control
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      1, // unsigned char alarm;
    },
    { // IO_H1MC[27]
      // board component
      { 0,0x12345678 },
      // motor_control
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      1, // unsigned char alarm;
    },
    { // IO_H1MC[28]
      // board component
      { 0,0x12345678 },
      // motor_control
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      1, // unsigned char alarm;
    },
    { // IO_H1MC[29]
      // board component
      { 0,0x12345678 },
      // motor_control
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      1, // unsigned char alarm;
    },
    { // IO_H1MC[30]
      // board component
      { 0,0x12345678 },
      // motor_control
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      1, // unsigned char alarm;
    },
    { // IO_H1MC[31]
      // board component
      { 0,0x12345678 },
      // motor_control
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      1, // unsigned char alarm;
    },
  },

  { // s_option_IO_H2MC IO_H2MC[IO_H2MC_MAX];
    { // IO_H2MC[0]
      // board component
      { 0,0x12345678 },
      // motor_control
      {
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // 0
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // 1
      },
      1, // unsigned char alarm;
    },
    { // IO_H2MC[1]
      // board component
      { 0,0x12345678 },
      // motor_control
      {
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // 0
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // 1
      },
      1, // unsigned char alarm;
    },
    { // IO_H2MC[2]
      // board component
      { 0,0x12345678 },
      // motor_control
      {
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // 0
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // 1
      },
      1, // unsigned char alarm;
    },
    { // IO_H2MC[3]
      // board component
      { 0,0x12345678 },
      // motor_control
      {
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // 0
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // 1
      },
      1, // unsigned char alarm;
    },
    { // IO_H2MC[4]
      // board component
      { 0,0x12345678 },
      // motor_control
      {
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // 0
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // 1
      },
      1, // unsigned char alarm;
    },
    { // IO_H2MC[5]
      // board component
      { 0,0x12345678 },
      // motor_control
      {
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // 0
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // 1
      },
      1, // unsigned char alarm;
    },
    { // IO_H2MC[6]
      // board component
      { 0,0x12345678 },
      // motor_control
      {
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // 0
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // 1
      },
      1, // unsigned char alarm;
    },
    { // IO_H2MC[7]
      // board component
      { 0,0x12345678 },
      // motor_control
      {
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // 0
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // 1
      },
      1, // unsigned char alarm;
    },
    { // IO_H2MC[8]
      // board component
      { 0,0x12345678 },
      // motor_control
      {
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // 0
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // 1
      },
      1, // unsigned char alarm;
    },
    { // IO_H2MC[9]
      // board component
      { 0,0x12345678 },
      // motor_control
      {
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // 0
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // 1
      },
      1, // unsigned char alarm;
    },
    { // IO_H2MC[10]
      // board component
      { 0,0x12345678 },
      // motor_control
      {
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // 0
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // 1
      },
      1, // unsigned char alarm;
    },
    { // IO_H2MC[11]
      // board component
      { 0,0x12345678 },
      // motor_control
      {
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // 0
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // 1
      },
      1, // unsigned char alarm;
    },
    { // IO_H2MC[12]
      // board component
      { 0,0x12345678 },
      // motor_control
      {
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // 0
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // 1
      },
      1, // unsigned char alarm;
    },
    { // IO_H2MC[13]
      // board component
      { 0,0x12345678 },
      // motor_control
      {
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // 0
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // 1
      },
      1, // unsigned char alarm;
    },
    { // IO_H2MC[14]
      // board component
      { 0,0x12345678 },
      // motor_control
      {
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // 0
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // 1
      },
      1, // unsigned char alarm;
    },
    { // IO_H2MC[15]
      // board component
      { 0,0x12345678 },
      // motor_control
      {
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // 0
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // 1
      },
      1, // unsigned char alarm;
    },
    { // IO_H2MC[16]
      // board component
      { 0,0x12345678 },
      // motor_control
      {
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // 0
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // 1
      },
      1, // unsigned char alarm;
    },
    { // IO_H2MC[17]
      // board component
      { 0,0x12345678 },
      // motor_control
      {
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // 0
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // 1
      },
      1, // unsigned char alarm;
    },
    { // IO_H2MC[18]
      // board component
      { 0,0x12345678 },
      // motor_control
      {
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // 0
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // 1
      },
      1, // unsigned char alarm;
    },
    { // IO_H2MC[19]
      // board component
      { 0,0x12345678 },
      // motor_control
      {
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // 0
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // 1
      },
      1, // unsigned char alarm;
    },
    { // IO_H2MC[20]
      // board component
      { 0,0x12345678 },
      // motor_control
      {
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // 0
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // 1
      },
      1, // unsigned char alarm;
    },
    { // IO_H2MC[21]
      // board component
      { 0,0x12345678 },
      // motor_control
      {
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // 0
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // 1
      },
      1, // unsigned char alarm;
    },
    { // IO_H2MC[22]
      // board component
      { 0,0x12345678 },
      // motor_control
      {
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // 0
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // 1
      },
      1, // unsigned char alarm;
    },
    { // IO_H2MC[23]
      // board component
      { 0,0x12345678 },
      // motor_control
      {
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // 0
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // 1
      },
      1, // unsigned char alarm;
    },
    { // IO_H2MC[24]
      // board component
      { 0,0x12345678 },
      // motor_control
      {
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // 0
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // 1
      },
      1, // unsigned char alarm;
    },
    { // IO_H2MC[25]
      // board component
      { 0,0x12345678 },
      // motor_control
      {
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // 0
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // 1
      },
      1, // unsigned char alarm;
    },
    { // IO_H2MC[26]
      // board component
      { 0,0x12345678 },
      // motor_control
      {
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // 0
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // 1
      },
      1, // unsigned char alarm;
    },
    { // IO_H2MC[27]
      // board component
      { 0,0x12345678 },
      // motor_control
      {
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // 0
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // 1
      },
      1, // unsigned char alarm;
    },
    { // IO_H2MC[28]
      // board component
      { 0,0x12345678 },
      // motor_control
      {
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // 0
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // 1
      },
      1, // unsigned char alarm;
    },
    { // IO_H2MC[29]
      // board component
      { 0,0x12345678 },
      // motor_control
      {
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // 0
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // 1
      },
      1, // unsigned char alarm;
    },
    { // IO_H2MC[30]
      // board component
      { 0,0x12345678 },
      // motor_control
      {
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // 0
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // 1
      },
      1, // unsigned char alarm;
    },
    { // IO_H2MC[31]
      // board component
      { 0,0x12345678 },
      // motor_control
      {
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // 0
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // 1
      },
      1, // unsigned char alarm;
    },
  },

  { // s_option_IO_EKU IO_EKU[IO_EKU_MAX];
    { // IO_EKU[0]
      // board component
      { 0,0x12345678 },
      // motor_control
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      1, // unsigned char alarm;
    },
    { // IO_EKU[1]
      // board component
      { 0,0x12345678 },
      // motor_control
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      1, // unsigned char alarm;
    },
    { // IO_EKU[2]
      // board component
      { 0,0x12345678 },
      // motor_control
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      1, // unsigned char alarm;
    },
    { // IO_EKU[3]
      // board component
      { 0,0x12345678 },
      // motor_control
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      1, // unsigned char alarm;
    },
    { // IO_EKU[4]
      // board component
      { 0,0x12345678 },
      // motor_control
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      1, // unsigned char alarm;
    },
    { // IO_EKU[5]
      // board component
      { 0,0x12345678 },
      // motor_control
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      1, // unsigned char alarm;
    },
    { // IO_EKU[6]
      // board component
      { 0,0x12345678 },
      // motor_control
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      1, // unsigned char alarm;
    },
    { // IO_EKU[7]
      // board component
      { 0,0x12345678 },
      // motor_control
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      1, // unsigned char alarm;
    },
    { // IO_EKU[8]
      // board component
      { 0,0x12345678 },
      // motor_control
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      1, // unsigned char alarm;
    },
    { // IO_EKU[9]
      // board component
      { 0,0x12345678 },
      // motor_control
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      1, // unsigned char alarm;
    },
    { // IO_EKU[10]
      // board component
      { 0,0x12345678 },
      // motor_control
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      1, // unsigned char alarm;
    },
    { // IO_EKU[11]
      // board component
      { 0,0x12345678 },
      // motor_control
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      1, // unsigned char alarm;
    },
    { // IO_EKU[12]
      // board component
      { 0,0x12345678 },
      // motor_control
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      1, // unsigned char alarm;
    },
    { // IO_EKU[13]
      // board component
      { 0,0x12345678 },
      // motor_control
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      1, // unsigned char alarm;
    },
    { // IO_EKU[14]
      // board component
      { 0,0x12345678 },
      // motor_control
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      1, // unsigned char alarm;
    },
    { // IO_EKU[15]
      // board component
      { 0,0x12345678 },
      // motor_control
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      1, // unsigned char alarm;
    },
  },

  0xAAAA,     // unsigned int end;    // Laatste adres opties. Bevat de checksum
};

//=============================================================================
s_setp_alg const default_setp_alg =
{
  0,                   //  0 unsigned int  area_size;       // Setpoint size, verschil begin setpoint, end setpoint
  ORION,               //  2 unsigned int computer;         // identification computer
  MULTI_CONNECT,       //  4 unsigned int soort;            // soort 0 = BASIS; 1 = PLUIMVEE; 2 = VARKENS
  MODULE_BASIS,        //  6 unsigned int type;             // type onderverdeling 0 = PLUIMVEE_CL; 1 = PLUIMVEE_PB;  
  HOTRACO,             //  8 unsigned int firma;            // firma 0 = hotraco
  VERSIE_PROG,         // 10 unsigned int versie_programma; // versie nummer eprom
  VERSIE_ALG_PC,       // 12 unsigned int versie_PC;        // versie pc
  0,                   // 14 unsigned long serie_number;    // serie number
  VERSIE_TEKST,        // unsigned int versie_tekst;        // reserve voor toekomst wat vooraan moet staan zoals bovenstaade items
  VERSIE_TEKST_INST,   // unsigned int versie_tekst_inst;   // reserve voor toekomst wat vooraan moet staan zoals bovenstaade items
  0,   // 22 unsigned int reserve2;         // reserve voor toekomst wat vooraan moet staan zoals bovenstaade items
  0,   // 24 unsigned int reserve3;         // reserve voor toekomst wat vooraan moet staan zoals bovenstaade items
  0,   // 26 unsigned int reserve4;         // reserve voor toekomst wat vooraan moet staan zoals bovenstaade items

  1,   // 29 unsigned char on_off;              // regelaar aan uit

  0,   // int dagenteller;
  1,   // unsigned char tijd_sync;          // wel of niet synchroniseren tijd als computer niet CAN master

  3,   // unsigned char sd_card_remove; // 0 = loggen ingeschakeld, 1 = loggen stoppen, 2 = loggen uitgeschakeld (sd_card mag verwijderd worden), 3 = sd_card niet aanwezig
  {0,0,0,0,0,0,0,0,0,0}, // unsigned char sd_card_log_on[MAX_FILES]; // if 1 then logging of this file is on

  { // sSetpMotorgroup Motorgroup[MAX_GROUP];
    { // Motorgroup[0]
      30,                // int DelayAlarmManual;
      { // sSetpPulseSystem PulseSystem;
        50,              // unsigned char PulseZone;
        30,              // unsigned char CycleTime;
        10,              // unsigned char PulseWidth;
        0,               // unsigned char Reserved;
      },
      20, // unsigned char DiffPositionAlarm;
      5,  // unsigned char TimePositionAlarm;
      {0,0,0,0,0,0,0}, // int Reserved[7];
    },
    { // Motorgroup[1]
      30,                // int DelayAlarmManual;
      { // sSetpPulseSystem PulseSystem;
        50,              // unsigned char PulseZone;
        30,              // unsigned char CycleTime;
        10,              // unsigned char PulseWidth;
        0,               // unsigned char Reserved;
      },
      20, // unsigned char DiffPositionAlarm;
      5,  // unsigned char TimePositionAlarm;
      {0,0,0,0,0,0,0}, // int Reserved[7];
    },
    { // Motorgroup[2]
      30,                // int DelayAlarmManual;
      { // sSetpPulseSystem PulseSystem;
        50,              // unsigned char PulseZone;
        30,              // unsigned char CycleTime;
        10,              // unsigned char PulseWidth;
        0,               // unsigned char Reserved;
      },
      20, // unsigned char DiffPositionAlarm;
      5,  // unsigned char TimePositionAlarm;
      {0,0,0,0,0,0,0}, // int Reserved[7];
    },
    { // Motorgroup[3]
      30,                // int DelayAlarmManual;
      { // sSetpPulseSystem PulseSystem;
        50,              // unsigned char PulseZone;
        30,              // unsigned char CycleTime;
        10,              // unsigned char PulseWidth;
        0,               // unsigned char Reserved;
      },
      20, // unsigned char DiffPositionAlarm;
      5,  // unsigned char TimePositionAlarm;
      {0,0,0,0,0,0,0}, // int Reserved[7];
    },
    { // Motorgroup[4]
      30,                // int DelayAlarmManual;
      { // sSetpPulseSystem PulseSystem;
        50,              // unsigned char PulseZone;
        30,              // unsigned char CycleTime;
        10,              // unsigned char PulseWidth;
        0,               // unsigned char Reserved;
      },
      20, // unsigned char DiffPositionAlarm;
      5,  // unsigned char TimePositionAlarm;
      {0,0,0,0,0,0,0}, // int Reserved[7];
    },
    { // Motorgroup[5]
      30,                // int DelayAlarmManual;
      { // sSetpPulseSystem PulseSystem;
        50,              // unsigned char PulseZone;
        30,              // unsigned char CycleTime;
        10,              // unsigned char PulseWidth;
        0,               // unsigned char Reserved;
      },
      20, // unsigned char DiffPositionAlarm;
      5,  // unsigned char TimePositionAlarm;
      {0,0,0,0,0,0,0}, // int Reserved[7];
    },
    { // Motorgroup[6]
      30,                // int DelayAlarmManual;
      { // sSetpPulseSystem PulseSystem;
        50,              // unsigned char PulseZone;
        30,              // unsigned char CycleTime;
        10,              // unsigned char PulseWidth;
        0,               // unsigned char Reserved;
      },
      20, // unsigned char DiffPositionAlarm;
      5,  // unsigned char TimePositionAlarm;
      {0,0,0,0,0,0,0}, // int Reserved[7];
    },
    { // Motorgroup[7]
      30,                // int DelayAlarmManual;
      { // sSetpPulseSystem PulseSystem;
        50,              // unsigned char PulseZone;
        30,              // unsigned char CycleTime;
        10,              // unsigned char PulseWidth;
        0,               // unsigned char Reserved;
      },
      20, // unsigned char DiffPositionAlarm;
      5,  // unsigned char TimePositionAlarm;
      {0,0,0,0,0,0,0}, // int Reserved[7];
    },
    { // Motorgroup[8]
      30,                // int DelayAlarmManual;
      { // sSetpPulseSystem PulseSystem;
        50,              // unsigned char PulseZone;
        30,              // unsigned char CycleTime;
        10,              // unsigned char PulseWidth;
        0,               // unsigned char Reserved;
      },
      20, // unsigned char DiffPositionAlarm;
      5,  // unsigned char TimePositionAlarm;
      {0,0,0,0,0,0,0}, // int Reserved[7];
    },
    { // Motorgroup[9]
      30,                // int DelayAlarmManual;
      { // sSetpPulseSystem PulseSystem;
        50,              // unsigned char PulseZone;
        30,              // unsigned char CycleTime;
        10,              // unsigned char PulseWidth;
        0,               // unsigned char Reserved;
      },
      20, // unsigned char DiffPositionAlarm;
      5,  // unsigned char TimePositionAlarm;
      {0,0,0,0,0,0,0}, // int Reserved[7];
    },
    { // Motorgroup[10]
      30,                // int DelayAlarmManual;
      { // sSetpPulseSystem PulseSystem;
        50,              // unsigned char PulseZone;
        30,              // unsigned char CycleTime;
        10,              // unsigned char PulseWidth;
        0,               // unsigned char Reserved;
      },
      20, // unsigned char DiffPositionAlarm;
      5,  // unsigned char TimePositionAlarm;
      {0,0,0,0,0,0,0}, // int Reserved[7];
    },
    { // Motorgroup[11]
      30,                // int DelayAlarmManual;
      { // sSetpPulseSystem PulseSystem;
        50,              // unsigned char PulseZone;
        30,              // unsigned char CycleTime;
        10,              // unsigned char PulseWidth;
        0,               // unsigned char Reserved;
      },
      20, // unsigned char DiffPositionAlarm;
      5,  // unsigned char TimePositionAlarm;
      {0,0,0,0,0,0,0}, // int Reserved[7];
    },
    { // Motorgroup[12]
      30,                // int DelayAlarmManual;
      { // sSetpPulseSystem PulseSystem;
        50,              // unsigned char PulseZone;
        30,              // unsigned char CycleTime;
        10,              // unsigned char PulseWidth;
        0,               // unsigned char Reserved;
      },
      20, // unsigned char DiffPositionAlarm;
      5,  // unsigned char TimePositionAlarm;
      {0,0,0,0,0,0,0}, // int Reserved[7];
    },
    { // Motorgroup[13]
      30,                // int DelayAlarmManual;
      { // sSetpPulseSystem PulseSystem;
        50,              // unsigned char PulseZone;
        30,              // unsigned char CycleTime;
        10,              // unsigned char PulseWidth;
        0,               // unsigned char Reserved;
      },
      20, // unsigned char DiffPositionAlarm;
      5,  // unsigned char TimePositionAlarm;
      {0,0,0,0,0,0,0}, // int Reserved[7];
    },
    { // Motorgroup[14]
      30,                // int DelayAlarmManual;
      { // sSetpPulseSystem PulseSystem;
        50,              // unsigned char PulseZone;
        30,              // unsigned char CycleTime;
        10,              // unsigned char PulseWidth;
        0,               // unsigned char Reserved;
      },
      20, // unsigned char DiffPositionAlarm;
      5,  // unsigned char TimePositionAlarm;
      {0,0,0,0,0,0,0}, // int Reserved[7];
    },
    { // Motorgroup[15]
      30,                // int DelayAlarmManual;
      { // sSetpPulseSystem PulseSystem;
        50,              // unsigned char PulseZone;
        30,              // unsigned char CycleTime;
        10,              // unsigned char PulseWidth;
        0,               // unsigned char Reserved;
      },
      20, // unsigned char DiffPositionAlarm;
      5,  // unsigned char TimePositionAlarm;
      {0,0,0,0,0,0,0}, // int Reserved[7];
    },
    { // Motorgroup[16]
      30,                // int DelayAlarmManual;
      { // sSetpPulseSystem PulseSystem;
        50,              // unsigned char PulseZone;
        30,              // unsigned char CycleTime;
        10,              // unsigned char PulseWidth;
        0,               // unsigned char Reserved;
      },
      20, // unsigned char DiffPositionAlarm;
      5,  // unsigned char TimePositionAlarm;
      {0,0,0,0,0,0,0}, // int Reserved[7];
    },
    { // Motorgroup[17]
      30,                // int DelayAlarmManual;
      { // sSetpPulseSystem PulseSystem;
        50,              // unsigned char PulseZone;
        30,              // unsigned char CycleTime;
        10,              // unsigned char PulseWidth;
        0,               // unsigned char Reserved;
      },
      20, // unsigned char DiffPositionAlarm;
      5,  // unsigned char TimePositionAlarm;
      {0,0,0,0,0,0,0}, // int Reserved[7];
    },
    { // Motorgroup[18]
      30,                // int DelayAlarmManual;
      { // sSetpPulseSystem PulseSystem;
        50,              // unsigned char PulseZone;
        30,              // unsigned char CycleTime;
        10,              // unsigned char PulseWidth;
        0,               // unsigned char Reserved;
      },
      20, // unsigned char DiffPositionAlarm;
      5,  // unsigned char TimePositionAlarm;
      {0,0,0,0,0,0,0}, // int Reserved[7];
    },
    { // Motorgroup[19]
      30,                // int DelayAlarmManual;
      { // sSetpPulseSystem PulseSystem;
        50,              // unsigned char PulseZone;
        30,              // unsigned char CycleTime;
        10,              // unsigned char PulseWidth;
        0,               // unsigned char Reserved;
      },
      20, // unsigned char DiffPositionAlarm;
      5,  // unsigned char TimePositionAlarm;
      {0,0,0,0,0,0,0}, // int Reserved[7];
    },
    { // Motorgroup[20]
      30,                // int DelayAlarmManual;
      { // sSetpPulseSystem PulseSystem;
        50,              // unsigned char PulseZone;
        30,              // unsigned char CycleTime;
        10,              // unsigned char PulseWidth;
        0,               // unsigned char Reserved;
      },
      20, // unsigned char DiffPositionAlarm;
      5,  // unsigned char TimePositionAlarm;
      {0,0,0,0,0,0,0}, // int Reserved[7];
    },
    { // Motorgroup[21]
      30,                // int DelayAlarmManual;
      { // sSetpPulseSystem PulseSystem;
        50,              // unsigned char PulseZone;
        30,              // unsigned char CycleTime;
        10,              // unsigned char PulseWidth;
        0,               // unsigned char Reserved;
      },
      20, // unsigned char DiffPositionAlarm;
      5,  // unsigned char TimePositionAlarm;
      {0,0,0,0,0,0,0}, // int Reserved[7];
    },
    { // Motorgroup[22]
      30,                // int DelayAlarmManual;
      { // sSetpPulseSystem PulseSystem;
        50,              // unsigned char PulseZone;
        30,              // unsigned char CycleTime;
        10,              // unsigned char PulseWidth;
        0,               // unsigned char Reserved;
      },
      20, // unsigned char DiffPositionAlarm;
      5,  // unsigned char TimePositionAlarm;
      {0,0,0,0,0,0,0}, // int Reserved[7];
    },
    { // Motorgroup[23]
      30,                // int DelayAlarmManual;
      { // sSetpPulseSystem PulseSystem;
        50,              // unsigned char PulseZone;
        30,              // unsigned char CycleTime;
        10,              // unsigned char PulseWidth;
        0,               // unsigned char Reserved;
      },
      20, // unsigned char DiffPositionAlarm;
      5,  // unsigned char TimePositionAlarm;
      {0,0,0,0,0,0,0}, // int Reserved[7];
    },
    { // Motorgroup[24]
      30,                // int DelayAlarmManual;
      { // sSetpPulseSystem PulseSystem;
        50,              // unsigned char PulseZone;
        30,              // unsigned char CycleTime;
        10,              // unsigned char PulseWidth;
        0,               // unsigned char Reserved;
      },
      20, // unsigned char DiffPositionAlarm;
      5,  // unsigned char TimePositionAlarm;
      {0,0,0,0,0,0,0}, // int Reserved[7];
    },
    { // Motorgroup[25]
      30,                // int DelayAlarmManual;
      { // sSetpPulseSystem PulseSystem;
        50,              // unsigned char PulseZone;
        30,              // unsigned char CycleTime;
        10,              // unsigned char PulseWidth;
        0,               // unsigned char Reserved;
      },
      20, // unsigned char DiffPositionAlarm;
      5,  // unsigned char TimePositionAlarm;
      {0,0,0,0,0,0,0}, // int Reserved[7];
    },
    { // Motorgroup[26]
      30,                // int DelayAlarmManual;
      { // sSetpPulseSystem PulseSystem;
        50,              // unsigned char PulseZone;
        30,              // unsigned char CycleTime;
        10,              // unsigned char PulseWidth;
        0,               // unsigned char Reserved;
      },
      20, // unsigned char DiffPositionAlarm;
      5,  // unsigned char TimePositionAlarm;
      {0,0,0,0,0,0,0}, // int Reserved[7];
    },
    { // Motorgroup[27]
      30,                // int DelayAlarmManual;
      { // sSetpPulseSystem PulseSystem;
        50,              // unsigned char PulseZone;
        30,              // unsigned char CycleTime;
        10,              // unsigned char PulseWidth;
        0,               // unsigned char Reserved;
      },
      20, // unsigned char DiffPositionAlarm;
      5,  // unsigned char TimePositionAlarm;
      {0,0,0,0,0,0,0}, // int Reserved[7];
    },
    { // Motorgroup[28]
      30,                // int DelayAlarmManual;
      { // sSetpPulseSystem PulseSystem;
        50,              // unsigned char PulseZone;
        30,              // unsigned char CycleTime;
        10,              // unsigned char PulseWidth;
        0,               // unsigned char Reserved;
      },
      20, // unsigned char DiffPositionAlarm;
      5,  // unsigned char TimePositionAlarm;
      {0,0,0,0,0,0,0}, // int Reserved[7];
    },
    { // Motorgroup[29]
      30,                // int DelayAlarmManual;
      { // sSetpPulseSystem PulseSystem;
        50,              // unsigned char PulseZone;
        30,              // unsigned char CycleTime;
        10,              // unsigned char PulseWidth;
        0,               // unsigned char Reserved;
      },
      20, // unsigned char DiffPositionAlarm;
      5,  // unsigned char TimePositionAlarm;
      {0,0,0,0,0,0,0}, // int Reserved[7];
    },
    { // Motorgroup[30]
      30,                // int DelayAlarmManual;
      { // sSetpPulseSystem PulseSystem;
        50,              // unsigned char PulseZone;
        30,              // unsigned char CycleTime;
        10,              // unsigned char PulseWidth;
        0,               // unsigned char Reserved;
      },
      20, // unsigned char DiffPositionAlarm;
      5,  // unsigned char TimePositionAlarm;
      {0,0,0,0,0,0,0}, // int Reserved[7];
    },
    { // Motorgroup[31]
      30,                // int DelayAlarmManual;
      { // sSetpPulseSystem PulseSystem;
        50,              // unsigned char PulseZone;
        30,              // unsigned char CycleTime;
        10,              // unsigned char PulseWidth;
        0,               // unsigned char Reserved;
      },
      20, // unsigned char DiffPositionAlarm;
      5,  // unsigned char TimePositionAlarm;
      {0,0,0,0,0,0,0}, // int Reserved[7];
    },
  },

  { // sSetpVentgroup Ventgroup[MAX_GROUP];
    { // VentGroup[0]
      0,    // unsigned char MinVent;
      100,  // unsigned char MaxVent;
      1,    // unsigned char AlarmUrgent;
    },
    { // VentGroup[1]
      0,    // unsigned char MinVent;
      100,  // unsigned char MaxVent;
      1,    // unsigned char AlarmUrgent;
    },
    { // VentGroup[2]
      0,    // unsigned char MinVent;
      100,  // unsigned char MaxVent;
      1,    // unsigned char AlarmUrgent;
    },
    { // VentGroup[3]
      0,    // unsigned char MinVent;
      100,  // unsigned char MaxVent;
      1,    // unsigned char AlarmUrgent;
    },
    { // VentGroup[4]
      0,    // unsigned char MinVent;
      100,  // unsigned char MaxVent;
      1,    // unsigned char AlarmUrgent;
    },
    { // VentGroup[5]
      0,    // unsigned char MinVent;
      100,  // unsigned char MaxVent;
      1,    // unsigned char AlarmUrgent;
    },
    { // VentGroup[6]
      0,    // unsigned char MinVent;
      100,  // unsigned char MaxVent;
      1,    // unsigned char AlarmUrgent;
    },
    { // VentGroup[7]
      0,    // unsigned char MinVent;
      100,  // unsigned char MaxVent;
      1,    // unsigned char AlarmUrgent;
    },
    { // VentGroup[8]
      0,    // unsigned char MinVent;
      100,  // unsigned char MaxVent;
      1,    // unsigned char AlarmUrgent;
    },
    { // VentGroup[9]
      0,    // unsigned char MinVent;
      100,  // unsigned char MaxVent;
      1,    // unsigned char AlarmUrgent;
    },
    { // VentGroup[10]
      0,    // unsigned char MinVent;
      100,  // unsigned char MaxVent;
      1,    // unsigned char AlarmUrgent;
    },
    { // VentGroup[11]
      0,    // unsigned char MinVent;
      100,  // unsigned char MaxVent;
      1,    // unsigned char AlarmUrgent;
    },
    { // VentGroup[12]
      0,    // unsigned char MinVent;
      100,  // unsigned char MaxVent;
      1,    // unsigned char AlarmUrgent;
    },
    { // VentGroup[13]
      0,    // unsigned char MinVent;
      100,  // unsigned char MaxVent;
      1,    // unsigned char AlarmUrgent;
    },
    { // VentGroup[14]
      0,    // unsigned char MinVent;
      100,  // unsigned char MaxVent;
      1,    // unsigned char AlarmUrgent;
    },
    { // VentGroup[15]
      0,    // unsigned char MinVent;
      100,  // unsigned char MaxVent;
      1,    // unsigned char AlarmUrgent;
    },
    { // VentGroup[16]
      0,    // unsigned char MinVent;
      100,  // unsigned char MaxVent;
      1,    // unsigned char AlarmUrgent;
    },
    { // VentGroup[17]
      0,    // unsigned char MinVent;
      100,  // unsigned char MaxVent;
      1,    // unsigned char AlarmUrgent;
    },
    { // VentGroup[18]
      0,    // unsigned char MinVent;
      100,  // unsigned char MaxVent;
      1,    // unsigned char AlarmUrgent;
    },
    { // VentGroup[19]
      0,    // unsigned char MinVent;
      100,  // unsigned char MaxVent;
      1,    // unsigned char AlarmUrgent;
    },
    { // VentGroup[20]
      0,    // unsigned char MinVent;
      100,  // unsigned char MaxVent;
      1,    // unsigned char AlarmUrgent;
    },
    { // VentGroup[21]
      0,    // unsigned char MinVent;
      100,  // unsigned char MaxVent;
      1,    // unsigned char AlarmUrgent;
    },
    { // VentGroup[22]
      0,    // unsigned char MinVent;
      100,  // unsigned char MaxVent;
      1,    // unsigned char AlarmUrgent;
    },
    { // VentGroup[23]
      0,    // unsigned char MinVent;
      100,  // unsigned char MaxVent;
      1,    // unsigned char AlarmUrgent;
    },
    { // VentGroup[24]
      0,    // unsigned char MinVent;
      100,  // unsigned char MaxVent;
      1,    // unsigned char AlarmUrgent;
    },
    { // VentGroup[25]
      0,    // unsigned char MinVent;
      100,  // unsigned char MaxVent;
      1,    // unsigned char AlarmUrgent;
    },
    { // VentGroup[26]
      0,    // unsigned char MinVent;
      100,  // unsigned char MaxVent;
      1,    // unsigned char AlarmUrgent;
    },
    { // VentGroup[27]
      0,    // unsigned char MinVent;
      100,  // unsigned char MaxVent;
      1,    // unsigned char AlarmUrgent;
    },
    { // VentGroup[28]
      0,    // unsigned char MinVent;
      100,  // unsigned char MaxVent;
      1,    // unsigned char AlarmUrgent;
    },
    { // VentGroup[29]
      0,    // unsigned char MinVent;
      100,  // unsigned char MaxVent;
      1,    // unsigned char AlarmUrgent;
    },
    { // VentGroup[30]
      0,    // unsigned char MinVent;
      100,  // unsigned char MaxVent;
      1,    // unsigned char AlarmUrgent;
    },
    { // VentGroup[31]
      0,    // unsigned char MinVent;
      100,  // unsigned char MaxVent;
      1,    // unsigned char AlarmUrgent;
    },
  },

  { // sSetpDevice Device[MAX_DEVICE];
    { // Device[0]
      0,    // char Offset;
    },
    { // Device[1]
      0,    // char Offset;
    },
    { // Device[2]
      0,    // char Offset;
    },
    { // Device[3]
      0,    // char Offset;
    },
    { // Device[4]
      0,    // char Offset;
    },
    { // Device[5]
      0,    // char Offset;
    },
    { // Device[6]
      0,    // char Offset;
    },
    { // Device[7]
      0,    // char Offset;
    },
    { // Device[8]
      0,    // char Offset;
    },
    { // Device[9]
      0,    // char Offset;
    },
    { // Device[10]
      0,    // char Offset;
    },
    { // Device[11]
      0,    // char Offset;
    },
    { // Device[12]
      0,    // char Offset;
    },
    { // Device[13]
      0,    // char Offset;
    },
    { // Device[14]
      0,    // char Offset;
    },
    { // Device[15]
      0,    // char Offset;
    },
    { // Device[16]
      0,    // char Offset;
    },
    { // Device[17]
      0,    // char Offset;
    },
    { // Device[18]
      0,    // char Offset;
    },
    { // Device[19]
      0,    // char Offset;
    },
    { // Device[20]
      0,    // char Offset;
    },
    { // Device[21]
      0,    // char Offset;
    },
    { // Device[22]
      0,    // char Offset;
    },
    { // Device[23]
      0,    // char Offset;
    },
    { // Device[24]
      0,    // char Offset;
    },
    { // Device[25]
      0,    // char Offset;
    },
    { // Device[26]
      0,    // char Offset;
    },
    { // Device[27]
      0,    // char Offset;
    },
    { // Device[28]
      0,    // char Offset;
    },
    { // Device[29]
      0,    // char Offset;
    },
    { // Device[30]
      0,    // char Offset;
    },
    { // Device[31]
      0,    // char Offset;
    },
    { // Device[32]
      0,    // char Offset;
    },
    { // Device[33]
      0,    // char Offset;
    },
    { // Device[34]
      0,    // char Offset;
    },
    { // Device[35]
      0,    // char Offset;
    },
    { // Device[36]
      0,    // char Offset;
    },
    { // Device[37]
      0,    // char Offset;
    },
    { // Device[38]
      0,    // char Offset;
    },
    { // Device[39]
      0,    // char Offset;
    },
    { // Device[40]
      0,    // char Offset;
    },
    { // Device[41]
      0,    // char Offset;
    },
    { // Device[42]
      0,    // char Offset;
    },
    { // Device[43]
      0,    // char Offset;
    },
    { // Device[44]
      0,    // char Offset;
    },
    { // Device[45]
      0,    // char Offset;
    },
    { // Device[46]
      0,    // char Offset;
    },
    { // Device[47]
      0,    // char Offset;
    },
    { // Device[48]
      0,    // char Offset;
    },
    { // Device[49]
      0,    // char Offset;
    },
    { // Device[50]
      0,    // char Offset;
    },
    { // Device[51]
      0,    // char Offset;
    },
    { // Device[52]
      0,    // char Offset;
    },
    { // Device[53]
      0,    // char Offset;
    },
    { // Device[54]
      0,    // char Offset;
    },
    { // Device[55]
      0,    // char Offset;
    },
    { // Device[56]
      0,    // char Offset;
    },
    { // Device[57]
      0,    // char Offset;
    },
    { // Device[58]
      0,    // char Offset;
    },
    { // Device[59]
      0,    // char Offset;
    },
    { // Device[60]
      0,    // char Offset;
    },
    { // Device[61]
      0,    // char Offset;
    },
    { // Device[62]
      0,    // char Offset;
    },
    { // Device[63]
      0,    // char Offset;
    },
    { // Device[64]
      0,    // char Offset;
    },
    { // Device[65]
      0,    // char Offset;
    },
    { // Device[66]
      0,    // char Offset;
    },
    { // Device[67]
      0,    // char Offset;
    },
    { // Device[68]
      0,    // char Offset;
    },
    { // Device[69]
      0,    // char Offset;
    },
    { // Device[70]
      0,    // char Offset;
    },
    { // Device[71]
      0,    // char Offset;
    },
    { // Device[72]
      0,    // char Offset;
    },
    { // Device[73]
      0,    // char Offset;
    },
    { // Device[74]
      0,    // char Offset;
    },
    { // Device[75]
      0,    // char Offset;
    },
    { // Device[76]
      0,    // char Offset;
    },
    { // Device[77]
      0,    // char Offset;
    },
    { // Device[78]
      0,    // char Offset;
    },
    { // Device[79]
      0,    // char Offset;
    },
    { // Device[80]
      0,    // char Offset;
    },
    { // Device[81]
      0,    // char Offset;
    },
    { // Device[82]
      0,    // char Offset;
    },
    { // Device[83]
      0,    // char Offset;
    },
    { // Device[84]
      0,    // char Offset;
    },
    { // Device[85]
      0,    // char Offset;
    },
    { // Device[86]
      0,    // char Offset;
    },
    { // Device[87]
      0,    // char Offset;
    },
    { // Device[88]
      0,    // char Offset;
    },
    { // Device[89]
      0,    // char Offset;
    },
    { // Device[90]
      0,    // char Offset;
    },
    { // Device[91]
      0,    // char Offset;
    },
    { // Device[92]
      0,    // char Offset;
    },
    { // Device[93]
      0,    // char Offset;
    },
    { // Device[94]
      0,    // char Offset;
    },
    { // Device[95]
      0,    // char Offset;
    },
    { // Device[96]
      0,    // char Offset;
    },
    { // Device[97]
      0,    // char Offset;
    },
    { // Device[98]
      0,    // char Offset;
    },
    { // Device[99]
      0,    // char Offset;
    },
    { // Device[100]
      0,    // char Offset;
    },
    { // Device[101]
      0,    // char Offset;
    },
    { // Device[102]
      0,    // char Offset;
    },
    { // Device[103]
      0,    // char Offset;
    },
    { // Device[104]
      0,    // char Offset;
    },
    { // Device[105]
      0,    // char Offset;
    },
    { // Device[106]
      0,    // char Offset;
    },
    { // Device[107]
      0,    // char Offset;
    },
    { // Device[108]
      0,    // char Offset;
    },
    { // Device[109]
      0,    // char Offset;
    },
    { // Device[110]
      0,    // char Offset;
    },
    { // Device[111]
      0,    // char Offset;
    },
    { // Device[112]
      0,    // char Offset;
    },
    { // Device[113]
      0,    // char Offset;
    },
    { // Device[114]
      0,    // char Offset;
    },
    { // Device[115]
      0,    // char Offset;
    },
    { // Device[116]
      0,    // char Offset;
    },
    { // Device[117]
      0,    // char Offset;
    },
    { // Device[118]
      0,    // char Offset;
    },
    { // Device[119]
      0,    // char Offset;
    },
    { // Device[120]
      0,    // char Offset;
    },
    { // Device[121]
      0,    // char Offset;
    },
    { // Device[122]
      0,    // char Offset;
    },
    { // Device[123]
      0,    // char Offset;
    },
    { // Device[124]
      0,    // char Offset;
    },
    { // Device[125]
      0,    // char Offset;
    },
    { // Device[126]
      0,    // char Offset;
    },
    { // Device[127]
      0,    // char Offset;
    },
    { // Device[128]
      0,    // char Offset;
    },
    { // Device[129]
      0,    // char Offset;
    },
    { // Device[130]
      0,    // char Offset;
    },
    { // Device[131]
      0,    // char Offset;
    },
    { // Device[132]
      0,    // char Offset;
    },
    { // Device[133]
      0,    // char Offset;
    },
    { // Device[134]
      0,    // char Offset;
    },
    { // Device[135]
      0,    // char Offset;
    },
    { // Device[136]
      0,    // char Offset;
    },
    { // Device[137]
      0,    // char Offset;
    },
    { // Device[138]
      0,    // char Offset;
    },
    { // Device[139]
      0,    // char Offset;
    },
    { // Device[140]
      0,    // char Offset;
    },
    { // Device[141]
      0,    // char Offset;
    },
    { // Device[142]
      0,    // char Offset;
    },
    { // Device[143]
      0,    // char Offset;
    },
    { // Device[144]
      0,    // char Offset;
    },
    { // Device[145]
      0,    // char Offset;
    },
    { // Device[146]
      0,    // char Offset;
    },
    { // Device[147]
      0,    // char Offset;
    },
    { // Device[148]
      0,    // char Offset;
    },
    { // Device[149]
      0,    // char Offset;
    },
    { // Device[150]
      0,    // char Offset;
    },
    { // Device[151]
      0,    // char Offset;
    },
    { // Device[152]
      0,    // char Offset;
    },
    { // Device[153]
      0,    // char Offset;
    },
    { // Device[154]
      0,    // char Offset;
    },
    { // Device[155]
      0,    // char Offset;
    },
    { // Device[156]
      0,    // char Offset;
    },
    { // Device[157]
      0,    // char Offset;
    },
    { // Device[158]
      0,    // char Offset;
    },
    { // Device[159]
      0,    // char Offset;
    },
    { // Device[160]
      0,    // char Offset;
    },
    { // Device[161]
      0,    // char Offset;
    },
    { // Device[162]
      0,    // char Offset;
    },
    { // Device[163]
      0,    // char Offset;
    },
    { // Device[164]
      0,    // char Offset;
    },
    { // Device[165]
      0,    // char Offset;
    },
    { // Device[166]
      0,    // char Offset;
    },
    { // Device[167]
      0,    // char Offset;
    },
    { // Device[168]
      0,    // char Offset;
    },
    { // Device[169]
      0,    // char Offset;
    },
    { // Device[170]
      0,    // char Offset;
    },
    { // Device[171]
      0,    // char Offset;
    },
    { // Device[172]
      0,    // char Offset;
    },
    { // Device[173]
      0,    // char Offset;
    },
    { // Device[174]
      0,    // char Offset;
    },
    { // Device[175]
      0,    // char Offset;
    },
    { // Device[176]
      0,    // char Offset;
    },
    { // Device[177]
      0,    // char Offset;
    },
    { // Device[178]
      0,    // char Offset;
    },
    { // Device[179]
      0,    // char Offset;
    },
    { // Device[180]
      0,    // char Offset;
    },
    { // Device[181]
      0,    // char Offset;
    },
    { // Device[182]
      0,    // char Offset;
    },
    { // Device[183]
      0,    // char Offset;
    },
    { // Device[184]
      0,    // char Offset;
    },
    { // Device[185]
      0,    // char Offset;
    },
    { // Device[186]
      0,    // char Offset;
    },
    { // Device[187]
      0,    // char Offset;
    },
    { // Device[188]
      0,    // char Offset;
    },
    { // Device[189]
      0,    // char Offset;
    },
    { // Device[190]
      0,    // char Offset;
    },
    { // Device[191]
      0,    // char Offset;
    },
    { // Device[192]
      0,    // char Offset;
    },
    { // Device[193]
      0,    // char Offset;
    },
    { // Device[194]
      0,    // char Offset;
    },
    { // Device[195]
      0,    // char Offset;
    },
    { // Device[196]
      0,    // char Offset;
    },
    { // Device[197]
      0,    // char Offset;
    },
    { // Device[198]
      0,    // char Offset;
    },
    { // Device[199]
      0,    // char Offset;
    },
    { // Device[200]
      0,    // char Offset;
    },
    { // Device[201]
      0,    // char Offset;
    },
    { // Device[202]
      0,    // char Offset;
    },
    { // Device[203]
      0,    // char Offset;
    },
    { // Device[204]
      0,    // char Offset;
    },
    { // Device[205]
      0,    // char Offset;
    },
    { // Device[206]
      0,    // char Offset;
    },
    { // Device[207]
      0,    // char Offset;
    },
    { // Device[208]
      0,    // char Offset;
    },
    { // Device[209]
      0,    // char Offset;
    },
    { // Device[210]
      0,    // char Offset;
    },
    { // Device[211]
      0,    // char Offset;
    },
    { // Device[212]
      0,    // char Offset;
    },
    { // Device[213]
      0,    // char Offset;
    },
    { // Device[214]
      0,    // char Offset;
    },
    { // Device[215]
      0,    // char Offset;
    },
    { // Device[216]
      0,    // char Offset;
    },
    { // Device[217]
      0,    // char Offset;
    },
    { // Device[218]
      0,    // char Offset;
    },
    { // Device[219]
      0,    // char Offset;
    },
    { // Device[220]
      0,    // char Offset;
    },
    { // Device[221]
      0,    // char Offset;
    },
    { // Device[222]
      0,    // char Offset;
    },
    { // Device[223]
      0,    // char Offset;
    },
    { // Device[224]
      0,    // char Offset;
    },
    { // Device[225]
      0,    // char Offset;
    },
    { // Device[226]
      0,    // char Offset;
    },
    { // Device[227]
      0,    // char Offset;
    },
    { // Device[228]
      0,    // char Offset;
    },
    { // Device[229]
      0,    // char Offset;
    },
    { // Device[230]
      0,    // char Offset;
    },
    { // Device[231]
      0,    // char Offset;
    },
    { // Device[232]
      0,    // char Offset;
    },
    { // Device[233]
      0,    // char Offset;
    },
    { // Device[234]
      0,    // char Offset;
    },
    { // Device[235]
      0,    // char Offset;
    },
    { // Device[236]
      0,    // char Offset;
    },
    { // Device[237]
      0,    // char Offset;
    },
    { // Device[238]
      0,    // char Offset;
    },
    { // Device[239]
      0,    // char Offset;
    },
    { // Device[240]
      0,    // char Offset;
    },
    { // Device[241]
      0,    // char Offset;
    },
    { // Device[242]
      0,    // char Offset;
    },
    { // Device[243]
      0,    // char Offset;
    },
    { // Device[244]
      0,    // char Offset;
    },
    { // Device[245]
      0,    // char Offset;
    },
    { // Device[246]
      0,    // char Offset;
    },
    { // Device[247]
      0,    // char Offset;
    },
    { // Device[248]
      0,    // char Offset;
    },
    { // Device[249]
      0,    // char Offset;
    },
    { // Device[250]
      0,    // char Offset;
    },
    { // Device[251]
      0,    // char Offset;
    },
    { // Device[252]
      0,    // char Offset;
    },
    { // Device[253]
      0,    // char Offset;
    },
    { // Device[254]
      0,    // char Offset;
    },
    { // Device[255]
      0,    // char Offset;
    },
  },

  { // sSetpDualScreen DualScreen[MAX_SCREEN];
    { // DualScreen[0]
      10, // int Opening;     // [%]
      5,  // int Hysteresis;  // [%]
      30, // int StandbyTime; // [min]
    },
    { // DualScreen[1]
      10, // int Opening;     // [%]
      5,  // int Hysteresis;  // [%]
      30, // int StandbyTime; // [min]
    },
    { // DualScreen[2]
      10, // int Opening;     // [%]
      5,  // int Hysteresis;  // [%]
      30, // int StandbyTime; // [min]
    },
    { // DualScreen[3]
      10, // int Opening;     // [%]
      5,  // int Hysteresis;  // [%]
      30, // int StandbyTime; // [min]
    },
    { // DualScreen[4]
      10, // int Opening;     // [%]
      5,  // int Hysteresis;  // [%]
      30, // int StandbyTime; // [min]
    },
    { // DualScreen[5]
      10, // int Opening;     // [%]
      5,  // int Hysteresis;  // [%]
      30, // int StandbyTime; // [min]
    },
    { // DualScreen[6]
      10, // int Opening;     // [%]
      5,  // int Hysteresis;  // [%]
      30, // int StandbyTime; // [min]
    },
    { // DualScreen[7]
      10, // int Opening;     // [%]
      5,  // int Hysteresis;  // [%]
      30, // int StandbyTime; // [min]
    },
  },
    
  { // TSetpLuchtmengkastGroep LuchtmengkastGroep[MAX_LUCHTMENGKAST_GROEP];
    { // LuchtmengkastGroep[0]
      30,  // unsigned char DelayAlarmManual;
      0,   // unsigned char OnderdrukAlarm;
      5,   // unsigned char DiffPositionAlarm;
      10,  // unsigned char TimePositionAlarm;

      10,  // unsigned char InblaasventMinimum;
      100, // unsigned char InblaasventMaximum;
      20,  // unsigned char AfblaasventMinimum;
      100, // unsigned char AfblaasventMaximum;

      0,   // unsigned char VerwarmingMinimum;
      5,   // unsigned char VerwarmingStap;
      30,  // unsigned char VerwarmingCyclustijd;
      5,   // unsigned char VerwarmingHysterese;
      10,  // unsigned char VerwarmingBandbreedte;

      20,  // unsigned char KlepstandAan;
      18,  // unsigned char KlepstandUit;
    },
    { // LuchtmengkastGroep[1]
      30,  // unsigned char DelayAlarmManual;
      0,   // unsigned char OnderdrukAlarm;
      5,   // unsigned char DiffPositionAlarm;
      10,  // unsigned char TimePositionAlarm;

      10,  // unsigned char InblaasventMinimum;
      100, // unsigned char InblaasventMaximum;
      20,  // unsigned char AfblaasventMinimum;
      100, // unsigned char AfblaasventMaximum;

      0,   // unsigned char VerwarmingMinimum;
      5,   // unsigned char VerwarmingStap;
      30,  // unsigned char VerwarmingCyclustijd;
      5,   // unsigned char VerwarmingHysterese;
      10,  // unsigned char VerwarmingBandbreedte;

      20,  // unsigned char KlepstandAan;
      18,  // unsigned char KlepstandUit;
    },
    { // LuchtmengkastGroep[2]
      30,  // unsigned char DelayAlarmManual;
      0,   // unsigned char OnderdrukAlarm;
      5,   // unsigned char DiffPositionAlarm;
      10,  // unsigned char TimePositionAlarm;

      10,  // unsigned char InblaasventMinimum;
      100, // unsigned char InblaasventMaximum;
      20,  // unsigned char AfblaasventMinimum;
      100, // unsigned char AfblaasventMaximum;

      0,   // unsigned char VerwarmingMinimum;
      5,   // unsigned char VerwarmingStap;
      30,  // unsigned char VerwarmingCyclustijd;
      5,   // unsigned char VerwarmingHysterese;
      10,  // unsigned char VerwarmingBandbreedte;

      20,  // unsigned char KlepstandAan;
      18,  // unsigned char KlepstandUit;
    },
    { // LuchtmengkastGroep[3]
      30,  // unsigned char DelayAlarmManual;
      0,   // unsigned char OnderdrukAlarm;
      5,   // unsigned char DiffPositionAlarm;
      10,  // unsigned char TimePositionAlarm;

      10,  // unsigned char InblaasventMinimum;
      100, // unsigned char InblaasventMaximum;
      20,  // unsigned char AfblaasventMinimum;
      100, // unsigned char AfblaasventMaximum;

      0,   // unsigned char VerwarmingMinimum;
      5,   // unsigned char VerwarmingStap;
      30,  // unsigned char VerwarmingCyclustijd;
      5,   // unsigned char VerwarmingHysterese;
      10,  // unsigned char VerwarmingBandbreedte;

      20,  // unsigned char KlepstandAan;
      18,  // unsigned char KlepstandUit;
    },
  },


  0xAAAA, // unsigned char end_setpoints;     // eind adres setpoints
};

//=============================================================================
s_val_hr_alg const default_val_hr_alg =
{
  0xA5A5, // unsigned int control_0;          // eerste controle getal
  0,      // unsigned int area_size; // Grote value_hr gebied

  { // s_value_hr_IO_05_07 IO_05_07[IO_05_07_MAX];
    { // IO_05_07[0]
      { 0,0 },                         // board component
      {{0,0},{0,0},{0,0},{0,0},{0,0}}, // analog input
      {{0,0},{0,0}},                   // analog output
      { 0,0 },                         // digital output
      { 0, {0,0,0,0,0,0,0}},           // RS485 bus
    },
    { // IO_05_07[1]
      { 0,0 },                         // board component
      {{0,0},{0,0},{0,0},{0,0},{0,0}}, // analog input
      {{0,0},{0,0}},                   // analog output
      { 0,0 },                         // digital output
      { 0, {0,0,0,0,0,0,0}},           // RS485 bus
    },
    { // IO_05_07[2]
      { 0,0 },                         // board component
      {{0,0},{0,0},{0,0},{0,0},{0,0}}, // analog input
      {{0,0},{0,0}},                   // analog output
      { 0,0 },                         // digital output
      { 0, {0,0,0,0,0,0,0}},           // RS485 bus
    },
    { // IO_05_07[3]
      { 0,0 },                         // board component
      {{0,0},{0,0},{0,0},{0,0},{0,0}}, // analog input
      {{0,0},{0,0}},                   // analog output
      { 0,0 },                         // digital output
      { 0, {0,0,0,0,0,0,0}},           // RS485 bus
    },
    { // IO_05_07[4]
      { 0,0 },                         // board component
      {{0,0},{0,0},{0,0},{0,0},{0,0}}, // analog input
      {{0,0},{0,0}},                   // analog output
      { 0,0 },                         // digital output
      { 0, {0,0,0,0,0,0,0}},           // RS485 bus
    },
    { // IO_05_07[5]
      { 0,0 },                         // board component
      {{0,0},{0,0},{0,0},{0,0},{0,0}}, // analog input
      {{0,0},{0,0}},                   // analog output
      { 0,0 },                         // digital output
      { 0, {0,0,0,0,0,0,0}},           // RS485 bus
    },
    { // IO_05_07[6]
      { 0,0 },                         // board component
      {{0,0},{0,0},{0,0},{0,0},{0,0}}, // analog input
      {{0,0},{0,0}},                   // analog output
      { 0,0 },                         // digital output
      { 0, {0,0,0,0,0,0,0}},           // RS485 bus
    },
    { // IO_05_07[7]
      { 0,0 },                         // board component
      {{0,0},{0,0},{0,0},{0,0},{0,0}}, // analog input
      {{0,0},{0,0}},                   // analog output
      { 0,0 },                         // digital output
      { 0, {0,0,0,0,0,0,0}},           // RS485 bus
    },
    { // IO_05_07[8]
      { 0,0 },                         // board component
      {{0,0},{0,0},{0,0},{0,0},{0,0}}, // analog input
      {{0,0},{0,0}},                   // analog output
      { 0,0 },                         // digital output
      { 0, {0,0,0,0,0,0,0}},           // RS485 bus
    },
    { // IO_05_07[9]
      { 0,0 },                         // board component
      {{0,0},{0,0},{0,0},{0,0},{0,0}}, // analog input
      {{0,0},{0,0}},                   // analog output
      { 0,0 },                         // digital output
      { 0, {0,0,0,0,0,0,0}},           // RS485 bus
    },
    { // IO_05_07[10]
      { 0,0 },                         // board component
      {{0,0},{0,0},{0,0},{0,0},{0,0}}, // analog input
      {{0,0},{0,0}},                   // analog output
      { 0,0 },                         // digital output
      { 0, {0,0,0,0,0,0,0}},           // RS485 bus
    },
    { // IO_05_07[11]
      { 0,0 },                         // board component
      {{0,0},{0,0},{0,0},{0,0},{0,0}}, // analog input
      {{0,0},{0,0}},                   // analog output
      { 0,0 },                         // digital output
      { 0, {0,0,0,0,0,0,0}},           // RS485 bus
    },
    { // IO_05_07[12]
      { 0,0 },                         // board component
      {{0,0},{0,0},{0,0},{0,0},{0,0}}, // analog input
      {{0,0},{0,0}},                   // analog output
      { 0,0 },                         // digital output
      { 0, {0,0,0,0,0,0,0}},           // RS485 bus
    },
    { // IO_05_07[13]
      { 0,0 },                         // board component
      {{0,0},{0,0},{0,0},{0,0},{0,0}}, // analog input
      {{0,0},{0,0}},                   // analog output
      { 0,0 },                         // digital output
      { 0, {0,0,0,0,0,0,0}},           // RS485 bus
    },
    { // IO_05_07[14]
      { 0,0 },                         // board component
      {{0,0},{0,0},{0,0},{0,0},{0,0}}, // analog input
      {{0,0},{0,0}},                   // analog output
      { 0,0 },                         // digital output
      { 0, {0,0,0,0,0,0,0}},           // RS485 bus
    },
    { // IO_05_07[15]
      { 0,0 },                         // board component
      {{0,0},{0,0},{0,0},{0,0},{0,0}}, // analog input
      {{0,0},{0,0}},                   // analog output
      { 0,0 },                         // digital output
      { 0, {0,0,0,0,0,0,0}},           // RS485 bus
    },
  },

  { // s_value_hr_IO_06_14 IO_06_14[IO_06_14_MAX];
    { // IO_06_14[0]
      { 0,0 },                                   // board component
      {{0,0},{0,0}},                             // analog input
      {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}}, // digital input
      { 0,0 },                                   // analog output
      { 0,0 }                                    // digital output
    },
    { // IO_06_14[1]
      { 0,0 },                                   // board component
      {{0,0},{0,0}},                             // analog input
      {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}}, // digital input
      { 0,0 },                                   // analog output
      { 0,0 }                                    // digital output
    },
    { // IO_06_14[2]
      { 0,0 },                                   // board component
      {{0,0},{0,0}},                             // analog input
      {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}}, // digital input
      { 0,0 },                                   // analog output
      { 0,0 }                                    // digital output
    },
    { // IO_06_14[3]
      { 0,0 },                                   // board component
      {{0,0},{0,0}},                             // analog input
      {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}}, // digital input
      { 0,0 },                                   // analog output
      { 0,0 }                                    // digital output
    },
  },

  { // s_value_hr_IO_07_07 IO_07_07[IO_07_07_MAX];
    { // IO_07_07[0]
      { 0,0 },                         // board component
      {{0,0},{0,0},{0,0},{0,0}},       // analog input
      {{0,0,0,0},{0,0,0,0},{0,0,0,0}}, // digital input
      {{0,0},{0,0}},                   // analog output
      { 0,0 },                         // digital output
      { 0,{0,0,0,0,0,0,0}},            // RS485 bus
    },
    { // IO_07_07[1]
      { 0,0 },                         // board component
      {{0,0},{0,0},{0,0},{0,0}},       // analog input
      {{0,0,0,0},{0,0,0,0},{0,0,0,0}}, // digital input
      {{0,0},{0,0}},                   // analog output
      { 0,0 },                         // digital output
      { 0,{0,0,0,0,0,0,0}},            // RS485 bus
    },
    { // IO_07_07[2]
      { 0,0 },                         // board component
      {{0,0},{0,0},{0,0},{0,0}},       // analog input
      {{0,0,0,0},{0,0,0,0},{0,0,0,0}}, // digital input
      {{0,0},{0,0}},                   // analog output
      { 0,0 },                         // digital output
      { 0,{0,0,0,0,0,0,0}},            // RS485 bus
    },
    { // IO_07_07[3]
      { 0,0 },                         // board component
      {{0,0},{0,0},{0,0},{0,0}},       // analog input
      {{0,0,0,0},{0,0,0,0},{0,0,0,0}}, // digital input
      {{0,0},{0,0}},                   // analog output
      { 0,0 },                         // digital output
      { 0,{0,0,0,0,0,0,0}},            // RS485 bus
    },
    { // IO_07_07[4]
      { 0,0 },                         // board component
      {{0,0},{0,0},{0,0},{0,0}},       // analog input
      {{0,0,0,0},{0,0,0,0},{0,0,0,0}}, // digital input
      {{0,0},{0,0}},                   // analog output
      { 0,0 },                         // digital output
      { 0,{0,0,0,0,0,0,0}},            // RS485 bus
    },
    { // IO_07_07[5]
      { 0,0 },                         // board component
      {{0,0},{0,0},{0,0},{0,0}},       // analog input
      {{0,0,0,0},{0,0,0,0},{0,0,0,0}}, // digital input
      {{0,0},{0,0}},                   // analog output
      { 0,0 },                         // digital output
      { 0,{0,0,0,0,0,0,0}},            // RS485 bus
    },
    { // IO_07_07[6]
      { 0,0 },                         // board component
      {{0,0},{0,0},{0,0},{0,0}},       // analog input
      {{0,0,0,0},{0,0,0,0},{0,0,0,0}}, // digital input
      {{0,0},{0,0}},                   // analog output
      { 0,0 },                         // digital output
      { 0,{0,0,0,0,0,0,0}},            // RS485 bus
    },
    { // IO_07_07[7]
      { 0,0 },                         // board component
      {{0,0},{0,0},{0,0},{0,0}},       // analog input
      {{0,0,0,0},{0,0,0,0},{0,0,0,0}}, // digital input
      {{0,0},{0,0}},                   // analog output
      { 0,0 },                         // digital output
      { 0,{0,0,0,0,0,0,0}},            // RS485 bus
    },
    { // IO_07_07[8]
      { 0,0 },                         // board component
      {{0,0},{0,0},{0,0},{0,0}},       // analog input
      {{0,0,0,0},{0,0,0,0},{0,0,0,0}}, // digital input
      {{0,0},{0,0}},                   // analog output
      { 0,0 },                         // digital output
      { 0,{0,0,0,0,0,0,0}},            // RS485 bus
    },
    { // IO_07_07[9]
      { 0,0 },                         // board component
      {{0,0},{0,0},{0,0},{0,0}},       // analog input
      {{0,0,0,0},{0,0,0,0},{0,0,0,0}}, // digital input
      {{0,0},{0,0}},                   // analog output
      { 0,0 },                         // digital output
      { 0,{0,0,0,0,0,0,0}},            // RS485 bus
    },
    { // IO_07_07[10]
      { 0,0 },                         // board component
      {{0,0},{0,0},{0,0},{0,0}},       // analog input
      {{0,0,0,0},{0,0,0,0},{0,0,0,0}}, // digital input
      {{0,0},{0,0}},                   // analog output
      { 0,0 },                         // digital output
      { 0,{0,0,0,0,0,0,0}},            // RS485 bus
    },
    { // IO_07_07[11]
      { 0,0 },                         // board component
      {{0,0},{0,0},{0,0},{0,0}},       // analog input
      {{0,0,0,0},{0,0,0,0},{0,0,0,0}}, // digital input
      {{0,0},{0,0}},                   // analog output
      { 0,0 },                         // digital output
      { 0,{0,0,0,0,0,0,0}},            // RS485 bus
    },
    { // IO_07_07[12]
      { 0,0 },                         // board component
      {{0,0},{0,0},{0,0},{0,0}},       // analog input
      {{0,0,0,0},{0,0,0,0},{0,0,0,0}}, // digital input
      {{0,0},{0,0}},                   // analog output
      { 0,0 },                         // digital output
      { 0,{0,0,0,0,0,0,0}},            // RS485 bus
    },
    { // IO_07_07[13]
      { 0,0 },                         // board component
      {{0,0},{0,0},{0,0},{0,0}},       // analog input
      {{0,0,0,0},{0,0,0,0},{0,0,0,0}}, // digital input
      {{0,0},{0,0}},                   // analog output
      { 0,0 },                         // digital output
      { 0,{0,0,0,0,0,0,0}},            // RS485 bus
    },
    { // IO_07_07[14]
      { 0,0 },                         // board component
      {{0,0},{0,0},{0,0},{0,0}},       // analog input
      {{0,0,0,0},{0,0,0,0},{0,0,0,0}}, // digital input
      {{0,0},{0,0}},                   // analog output
      { 0,0 },                         // digital output
      { 0,{0,0,0,0,0,0,0}},            // RS485 bus
    },
    { // IO_07_07[15]
      { 0,0 },                         // board component
      {{0,0},{0,0},{0,0},{0,0}},       // analog input
      {{0,0,0,0},{0,0,0,0},{0,0,0,0}}, // digital input
      {{0,0},{0,0}},                   // analog output
      { 0,0 },                         // digital output
      { 0,{0,0,0,0,0,0,0}},            // RS485 bus
    },
    { // IO_07_07[16]
      { 0,0 },                         // board component
      {{0,0},{0,0},{0,0},{0,0}},       // analog input
      {{0,0,0,0},{0,0,0,0},{0,0,0,0}}, // digital input
      {{0,0},{0,0}},                   // analog output
      { 0,0 },                         // digital output
      { 0,{0,0,0,0,0,0,0}},            // RS485 bus
    },
    { // IO_07_07[17]
      { 0,0 },                         // board component
      {{0,0},{0,0},{0,0},{0,0}},       // analog input
      {{0,0,0,0},{0,0,0,0},{0,0,0,0}}, // digital input
      {{0,0},{0,0}},                   // analog output
      { 0,0 },                         // digital output
      { 0,{0,0,0,0,0,0,0}},            // RS485 bus
    },
    { // IO_07_07[18]
      { 0,0 },                         // board component
      {{0,0},{0,0},{0,0},{0,0}},       // analog input
      {{0,0,0,0},{0,0,0,0},{0,0,0,0}}, // digital input
      {{0,0},{0,0}},                   // analog output
      { 0,0 },                         // digital output
      { 0,{0,0,0,0,0,0,0}},            // RS485 bus
    },
    { // IO_07_07[19]
      { 0,0 },                         // board component
      {{0,0},{0,0},{0,0},{0,0}},       // analog input
      {{0,0,0,0},{0,0,0,0},{0,0,0,0}}, // digital input
      {{0,0},{0,0}},                   // analog output
      { 0,0 },                         // digital output
      { 0,{0,0,0,0,0,0,0}},            // RS485 bus
    },
    { // IO_07_07[20]
      { 0,0 },                         // board component
      {{0,0},{0,0},{0,0},{0,0}},       // analog input
      {{0,0,0,0},{0,0,0,0},{0,0,0,0}}, // digital input
      {{0,0},{0,0}},                   // analog output
      { 0,0 },                         // digital output
      { 0,{0,0,0,0,0,0,0}},            // RS485 bus
    },
    { // IO_07_07[21]
      { 0,0 },                         // board component
      {{0,0},{0,0},{0,0},{0,0}},       // analog input
      {{0,0,0,0},{0,0,0,0},{0,0,0,0}}, // digital input
      {{0,0},{0,0}},                   // analog output
      { 0,0 },                         // digital output
      { 0,{0,0,0,0,0,0,0}},            // RS485 bus
    },
    { // IO_07_07[22]
      { 0,0 },                         // board component
      {{0,0},{0,0},{0,0},{0,0}},       // analog input
      {{0,0,0,0},{0,0,0,0},{0,0,0,0}}, // digital input
      {{0,0},{0,0}},                   // analog output
      { 0,0 },                         // digital output
      { 0,{0,0,0,0,0,0,0}},            // RS485 bus
    },
    { // IO_07_07[23]
      { 0,0 },                         // board component
      {{0,0},{0,0},{0,0},{0,0}},       // analog input
      {{0,0,0,0},{0,0,0,0},{0,0,0,0}}, // digital input
      {{0,0},{0,0}},                   // analog output
      { 0,0 },                         // digital output
      { 0,{0,0,0,0,0,0,0}},            // RS485 bus
    },
    { // IO_07_07[24]
      { 0,0 },                         // board component
      {{0,0},{0,0},{0,0},{0,0}},       // analog input
      {{0,0,0,0},{0,0,0,0},{0,0,0,0}}, // digital input
      {{0,0},{0,0}},                   // analog output
      { 0,0 },                         // digital output
      { 0,{0,0,0,0,0,0,0}},            // RS485 bus
    },
    { // IO_07_07[25]
      { 0,0 },                         // board component
      {{0,0},{0,0},{0,0},{0,0}},       // analog input
      {{0,0,0,0},{0,0,0,0},{0,0,0,0}}, // digital input
      {{0,0},{0,0}},                   // analog output
      { 0,0 },                         // digital output
      { 0,{0,0,0,0,0,0,0}},            // RS485 bus
    },
    { // IO_07_07[26]
      { 0,0 },                         // board component
      {{0,0},{0,0},{0,0},{0,0}},       // analog input
      {{0,0,0,0},{0,0,0,0},{0,0,0,0}}, // digital input
      {{0,0},{0,0}},                   // analog output
      { 0,0 },                         // digital output
      { 0,{0,0,0,0,0,0,0}},            // RS485 bus
    },
    { // IO_07_07[27]
      { 0,0 },                         // board component
      {{0,0},{0,0},{0,0},{0,0}},       // analog input
      {{0,0,0,0},{0,0,0,0},{0,0,0,0}}, // digital input
      {{0,0},{0,0}},                   // analog output
      { 0,0 },                         // digital output
      { 0,{0,0,0,0,0,0,0}},            // RS485 bus
    },
    { // IO_07_07[28]
      { 0,0 },                         // board component
      {{0,0},{0,0},{0,0},{0,0}},       // analog input
      {{0,0,0,0},{0,0,0,0},{0,0,0,0}}, // digital input
      {{0,0},{0,0}},                   // analog output
      { 0,0 },                         // digital output
      { 0,{0,0,0,0,0,0,0}},            // RS485 bus
    },
    { // IO_07_07[29]
      { 0,0 },                         // board component
      {{0,0},{0,0},{0,0},{0,0}},       // analog input
      {{0,0,0,0},{0,0,0,0},{0,0,0,0}}, // digital input
      {{0,0},{0,0}},                   // analog output
      { 0,0 },                         // digital output
      { 0,{0,0,0,0,0,0,0}},            // RS485 bus
    },
    { // IO_07_07[30]
      { 0,0 },                         // board component
      {{0,0},{0,0},{0,0},{0,0}},       // analog input
      {{0,0,0,0},{0,0,0,0},{0,0,0,0}}, // digital input
      {{0,0},{0,0}},                   // analog output
      { 0,0 },                         // digital output
      { 0,{0,0,0,0,0,0,0}},            // RS485 bus
    },
    { // IO_07_07[31]
      { 0,0 },                         // board component
      {{0,0},{0,0},{0,0},{0,0}},       // analog input
      {{0,0,0,0},{0,0,0,0},{0,0,0,0}}, // digital input
      {{0,0},{0,0}},                   // analog output
      { 0,0 },                         // digital output
      { 0,{0,0,0,0,0,0,0}},            // RS485 bus
    },
  },

  { // s_value_hr_IO_08_09 IO_08_09[IO_08_09_MAX];
    { // IO_08_09[0]
      { 0,0 },                                                                           // board component
      {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}}, // digital input
      {{0,0},{0,0},{0,0},{0,0}},                                                         // analog output
      { 0,0 },                                                                           // digital output
    },
    { // IO_08_09[1]
      { 0,0 },                                                                           // board component
      {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}}, // digital input
      {{0,0},{0,0},{0,0},{0,0}},                                                         // analog output
      { 0,0 },                                                                           // digital output
    },
    { // IO_08_09[2]
      { 0,0 },                                                                           // board component
      {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}}, // digital input
      {{0,0},{0,0},{0,0},{0,0}},                                                         // analog output
      { 0,0 },                                                                           // digital output
    },
    { // IO_08_09[3]
      { 0,0 },                                                                           // board component
      {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}}, // digital input
      {{0,0},{0,0},{0,0},{0,0}},                                                         // analog output
      { 0,0 },                                                                           // digital output
    },
    { // IO_08_09[4]
      { 0,0 },                                                                           // board component
      {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}}, // digital input
      {{0,0},{0,0},{0,0},{0,0}},                                                         // analog output
      { 0,0 },                                                                           // digital output
    },
    { // IO_08_09[5]
      { 0,0 },                                                                           // board component
      {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}}, // digital input
      {{0,0},{0,0},{0,0},{0,0}},                                                         // analog output
      { 0,0 },                                                                           // digital output
    },
    { // IO_08_09[6]
      { 0,0 },                                                                           // board component
      {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}}, // digital input
      {{0,0},{0,0},{0,0},{0,0}},                                                         // analog output
      { 0,0 },                                                                           // digital output
    },
    { // IO_08_09[7]
      { 0,0 },                                                                           // board component
      {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}}, // digital input
      {{0,0},{0,0},{0,0},{0,0}},                                                         // analog output
      { 0,0 },                                                                           // digital output
    },
  },

  { // s_value_hr_IO_12_06 IO_12_06[IO_12_06_MAX];
    { // IO_12_06[0]
      { 0,0 }, // board component
      { // digital input
        {0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},
        {0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}
      },
      { // counter_input
        { // counter_input[0]
          {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // int value[16];
          0,                                 // unsigned int command;
          {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // long count[16];
          {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // int old_value[16];
          {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}  // unsigned char error[16];
        },
        { // counter_input[1]
          {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // int value[16];
          0,                                 // unsigned int command;
          {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // long count[16];
          {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // int old_value[16];
          {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}  // unsigned char error[16];
        },
        { // counter_input[2]
          {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // int value[16];
          0,                                 // unsigned int command;
          {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // long count[16];
          {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // int old_value[16];
          {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}  // unsigned char error[16];
        },
        { // counter_input[3]
          {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // int value[16];
          0,                                 // unsigned int command;
          {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // long count[16];
          {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // int old_value[16];
          {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}  // unsigned char error[16];
        }
      },
      {{0,0},{0,0}}, // analog output
      { 0,0 }        // digital output
    },
    { // IO_12_06[1]
      { 0,0 }, // board component
      { // digital input
        {0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},
        {0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}
      },
      { // counter_input
        { // counter_input[0]
          {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // int value[16];
          0,                                 // unsigned int command;
          {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // long count[16];
          {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // int old_value[16];
          {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}  // unsigned char error[16];
        },
        { // counter_input[1]
          {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // int value[16];
          0,                                 // unsigned int command;
          {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // long count[16];
          {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // int old_value[16];
          {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}  // unsigned char error[16];
        },
        { // counter_input[2]
          {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // int value[16];
          0,                                 // unsigned int command;
          {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // long count[16];
          {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // int old_value[16];
          {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}  // unsigned char error[16];
        },
        { // counter_input[3]
          {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // int value[16];
          0,                                 // unsigned int command;
          {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // long count[16];
          {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // int old_value[16];
          {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}  // unsigned char error[16];
        }
      },
      {{0,0},{0,0}}, // analog output
      { 0,0 }        // digital output
    },
    { // IO_12_06[2]
      { 0,0 }, // board component
      { // digital input
        {0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},
        {0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}
      },
      { // counter_input
        { // counter_input[0]
          {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // int value[16];
          0,                                 // unsigned int command;
          {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // long count[16];
          {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // int old_value[16];
          {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}  // unsigned char error[16];
        },
        { // counter_input[1]
          {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // int value[16];
          0,                                 // unsigned int command;
          {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // long count[16];
          {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // int old_value[16];
          {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}  // unsigned char error[16];
        },
        { // counter_input[2]
          {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // int value[16];
          0,                                 // unsigned int command;
          {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // long count[16];
          {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // int old_value[16];
          {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}  // unsigned char error[16];
        },
        { // counter_input[3]
          {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // int value[16];
          0,                                 // unsigned int command;
          {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // long count[16];
          {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // int old_value[16];
          {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}  // unsigned char error[16];
        }
      },
      {{0,0},{0,0}}, // analog output
      { 0,0 }        // digital output
    },
    { // IO_12_06[3]
      { 0,0 }, // board component
      { // digital input
        {0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},
        {0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}
      },
      { // counter_input
        { // counter_input[0]
          {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // int value[16];
          0,                                 // unsigned int command;
          {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // long count[16];
          {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // int old_value[16];
          {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}  // unsigned char error[16];
        },
        { // counter_input[1]
          {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // int value[16];
          0,                                 // unsigned int command;
          {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // long count[16];
          {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // int old_value[16];
          {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}  // unsigned char error[16];
        },
        { // counter_input[2]
          {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // int value[16];
          0,                                 // unsigned int command;
          {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // long count[16];
          {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // int old_value[16];
          {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}  // unsigned char error[16];
        },
        { // counter_input[3]
          {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // int value[16];
          0,                                 // unsigned int command;
          {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // long count[16];
          {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // int old_value[16];
          {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}  // unsigned char error[16];
        }
      },
      {{0,0},{0,0}}, // analog output
      { 0,0 }        // digital output
    },
  },

  { // s_value_hr_IO_H1MC IO_H1MC[IO_H1MC_MAX];
    { // IO_H1MC[0]
      {0,0},               // board component
      {0,0,0,0,0,0,0,0,0}, // motor_control
    },
    { // IO_H1MC[1]
      {0,0},               // board component
      {0,0,0,0,0,0,0,0,0}, // motor_control
    },
    { // IO_H1MC[2]
      {0,0},               // board component
      {0,0,0,0,0,0,0,0,0}, // motor_control
    },
    { // IO_H1MC[3]
      {0,0},               // board component
      {0,0,0,0,0,0,0,0,0}, // motor_control
    },
    { // IO_H1MC[4]
      {0,0},               // board component
      {0,0,0,0,0,0,0,0,0}, // motor_control
    },
    { // IO_H1MC[5]
      {0,0},               // board component
      {0,0,0,0,0,0,0,0,0}, // motor_control
    },
    { // IO_H1MC[6]
      {0,0},               // board component
      {0,0,0,0,0,0,0,0,0}, // motor_control
    },
    { // IO_H1MC[7]
      {0,0},               // board component
      {0,0,0,0,0,0,0,0,0}, // motor_control
    },
    { // IO_H1MC[8]
      {0,0},               // board component
      {0,0,0,0,0,0,0,0,0}, // motor_control
    },
    { // IO_H1MC[9]
      {0,0},               // board component
      {0,0,0,0,0,0,0,0,0}, // motor_control
    },
    { // IO_H1MC[10]
      {0,0},               // board component
      {0,0,0,0,0,0,0,0,0}, // motor_control
    },
    { // IO_H1MC[11]
      {0,0},               // board component
      {0,0,0,0,0,0,0,0,0}, // motor_control
    },
    { // IO_H1MC[12]
      {0,0},               // board component
      {0,0,0,0,0,0,0,0,0}, // motor_control
    },
    { // IO_H1MC[13]
      {0,0},               // board component
      {0,0,0,0,0,0,0,0,0}, // motor_control
    },
    { // IO_H1MC[14]
      {0,0},               // board component
      {0,0,0,0,0,0,0,0,0}, // motor_control
    },
    { // IO_H1MC[15]
      {0,0},               // board component
      {0,0,0,0,0,0,0,0,0}, // motor_control
    },
    { // IO_H1MC[16]
      {0,0},               // board component
      {0,0,0,0,0,0,0,0,0}, // motor_control
    },
    { // IO_H1MC[17]
      {0,0},               // board component
      {0,0,0,0,0,0,0,0,0}, // motor_control
    },
    { // IO_H1MC[18]
      {0,0},               // board component
      {0,0,0,0,0,0,0,0,0}, // motor_control
    },
    { // IO_H1MC[19]
      {0,0},               // board component
      {0,0,0,0,0,0,0,0,0}, // motor_control
    },
    { // IO_H1MC[20]
      {0,0},               // board component
      {0,0,0,0,0,0,0,0,0}, // motor_control
    },
    { // IO_H1MC[21]
      {0,0},               // board component
      {0,0,0,0,0,0,0,0,0}, // motor_control
    },
    { // IO_H1MC[22]
      {0,0},               // board component
      {0,0,0,0,0,0,0,0,0}, // motor_control
    },
    { // IO_H1MC[23]
      {0,0},               // board component
      {0,0,0,0,0,0,0,0,0}, // motor_control
    },
    { // IO_H1MC[24]
      {0,0},               // board component
      {0,0,0,0,0,0,0,0,0}, // motor_control
    },
    { // IO_H1MC[25]
      {0,0},               // board component
      {0,0,0,0,0,0,0,0,0}, // motor_control
    },
    { // IO_H1MC[26]
      {0,0},               // board component
      {0,0,0,0,0,0,0,0,0}, // motor_control
    },
    { // IO_H1MC[27]
      {0,0},               // board component
      {0,0,0,0,0,0,0,0,0}, // motor_control
    },
    { // IO_H1MC[28]
      {0,0},               // board component
      {0,0,0,0,0,0,0,0,0}, // motor_control
    },
    { // IO_H1MC[29]
      {0,0},               // board component
      {0,0,0,0,0,0,0,0,0}, // motor_control
    },
    { // IO_H1MC[30]
      {0,0},               // board component
      {0,0,0,0,0,0,0,0,0}, // motor_control
    },
    { // IO_H1MC[31]
      {0,0},               // board component
      {0,0,0,0,0,0,0,0,0}, // motor_control
    },
  },

  { // s_value_hr_IO_H2MC IO_H2MC[IO_H2MC_MAX];
    { // IO_H2MC[0]
      { 0,0 },                                   // board component
      {{0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0}}, // motor_control
    },
    { // IO_H2MC[1]
      { 0,0 },                                   // board component
      {{0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0}}, // motor_control
    },
    { // IO_H2MC[2]
      { 0,0 },                                   // board component
      {{0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0}}, // motor_control
    },
    { // IO_H2MC[3]
      { 0,0 },                                   // board component
      {{0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0}}, // motor_control
    },
    { // IO_H2MC[4]
      { 0,0 },                                   // board component
      {{0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0}}, // motor_control
    },
    { // IO_H2MC[5]
      { 0,0 },                                   // board component
      {{0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0}}, // motor_control
    },
    { // IO_H2MC[6]
      { 0,0 },                                   // board component
      {{0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0}}, // motor_control
    },
    { // IO_H2MC[7]
      { 0,0 },                                   // board component
      {{0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0}}, // motor_control
    },
    { // IO_H2MC[8]
      { 0,0 },                                   // board component
      {{0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0}}, // motor_control
    },
    { // IO_H2MC[9]
      { 0,0 },                                   // board component
      {{0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0}}, // motor_control
    },
    { // IO_H2MC[10]
      { 0,0 },                                   // board component
      {{0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0}}, // motor_control
    },
    { // IO_H2MC[11]
      { 0,0 },                                   // board component
      {{0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0}}, // motor_control
    },
    { // IO_H2MC[12]
      { 0,0 },                                   // board component
      {{0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0}}, // motor_control
    },
    { // IO_H2MC[13]
      { 0,0 },                                   // board component
      {{0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0}}, // motor_control
    },
    { // IO_H2MC[14]
      { 0,0 },                                   // board component
      {{0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0}}, // motor_control
    },
    { // IO_H2MC[15]
      { 0,0 },                                   // board component
      {{0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0}}, // motor_control
    },
    { // IO_H2MC[16]
      { 0,0 },                                   // board component
      {{0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0}}, // motor_control
    },
    { // IO_H2MC[17]
      { 0,0 },                                   // board component
      {{0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0}}, // motor_control
    },
    { // IO_H2MC[18]
      { 0,0 },                                   // board component
      {{0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0}}, // motor_control
    },
    { // IO_H2MC[19]
      { 0,0 },                                   // board component
      {{0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0}}, // motor_control
    },
    { // IO_H2MC[20]
      { 0,0 },                                   // board component
      {{0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0}}, // motor_control
    },
    { // IO_H2MC[21]
      { 0,0 },                                   // board component
      {{0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0}}, // motor_control
    },
    { // IO_H2MC[22]
      { 0,0 },                                   // board component
      {{0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0}}, // motor_control
    },
    { // IO_H2MC[23]
      { 0,0 },                                   // board component
      {{0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0}}, // motor_control
    },
    { // IO_H2MC[24]
      { 0,0 },                                   // board component
      {{0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0}}, // motor_control
    },
    { // IO_H2MC[25]
      { 0,0 },                                   // board component
      {{0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0}}, // motor_control
    },
    { // IO_H2MC[26]
      { 0,0 },                                   // board component
      {{0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0}}, // motor_control
    },
    { // IO_H2MC[27]
      { 0,0 },                                   // board component
      {{0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0}}, // motor_control
    },
    { // IO_H2MC[28]
      { 0,0 },                                   // board component
      {{0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0}}, // motor_control
    },
    { // IO_H2MC[29]
      { 0,0 },                                   // board component
      {{0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0}}, // motor_control
    },
    { // IO_H2MC[30]
      { 0,0 },                                   // board component
      {{0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0}}, // motor_control
    },
    { // IO_H2MC[31]
      { 0,0 },                                   // board component
      {{0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0}}, // motor_control
    },
  },

  { // s_value_hr_IO_EKU IO_EKU[IO_EKU_MAX];
    { // IO_EKU[0]
      { 0,0 },             // board component
      {0,0,0,0,0,0,0,0,0}, // motor_control
    },
    { // IO_EKU[1]
      { 0,0 },             // board component
      {0,0,0,0,0,0,0,0,0}, // motor_control
    },
    { // IO_EKU[2]
      { 0,0 },             // board component
      {0,0,0,0,0,0,0,0,0}, // motor_control
    },
    { // IO_EKU[3]
      { 0,0 },             // board component
      {0,0,0,0,0,0,0,0,0}, // motor_control
    },
    { // IO_EKU[4]
      { 0,0 },             // board component
      {0,0,0,0,0,0,0,0,0}, // motor_control
    },
    { // IO_EKU[5]
      { 0,0 },             // board component
      {0,0,0,0,0,0,0,0,0}, // motor_control
    },
    { // IO_EKU[6]
      { 0,0 },             // board component
      {0,0,0,0,0,0,0,0,0}, // motor_control
    },
    { // IO_EKU[7]
      { 0,0 },             // board component
      {0,0,0,0,0,0,0,0,0}, // motor_control
    },
    { // IO_EKU[8]
      { 0,0 },             // board component
      {0,0,0,0,0,0,0,0,0}, // motor_control
    },
    { // IO_EKU[9]
      { 0,0 },             // board component
      {0,0,0,0,0,0,0,0,0}, // motor_control
    },
    { // IO_EKU[10]
      { 0,0 },             // board component
      {0,0,0,0,0,0,0,0,0}, // motor_control
    },
    { // IO_EKU[11]
      { 0,0 },             // board component
      {0,0,0,0,0,0,0,0,0}, // motor_control
    },
    { // IO_EKU[12]
      { 0,0 },             // board component
      {0,0,0,0,0,0,0,0,0}, // motor_control
    },
    { // IO_EKU[13]
      { 0,0 },             // board component
      {0,0,0,0,0,0,0,0,0}, // motor_control
    },
    { // IO_EKU[14]
      { 0,0 },             // board component
      {0,0,0,0,0,0,0,0,0}, // motor_control
    },
    { // IO_EKU[15]
      { 0,0 },             // board component
      {0,0,0,0,0,0,0,0,0}, // motor_control
    },
  },

  0, // unsigned char orion_switched_off; // orion is uitgeschakeld, genereer alarm tot een toets is gedrukt
  { // unsigned char can_node_alarm[128];     // 0 als node aanwezig
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,          // 1 als heartbeat node gemist wordt
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
  },
  { // int can_node_adres[128];     // software nummer van de hoofdcomputer van de aangesloten Orion of sirius
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,          
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
  },

  { // sValHrMotorgroup Motorgroup[MAX_GROUP];
    { // Motorgroup[0]
      omAuto, // TOperationMode OperationMode;
      0,      // int PositionTime;
      0,      // int PositionPerc;
    },
    { // Motorgroup[1]
      omAuto, // TOperationMode OperationMode;
      0,      // int PositionTime;
      0,      // int PositionPerc;
    },
    { // Motorgroup[2]
      omAuto, // TOperationMode OperationMode;
      0,      // int PositionTime;
      0,      // int PositionPerc;
    },
    { // Motorgroup[3]
      omAuto, // TOperationMode OperationMode;
      0,      // int PositionTime;
      0,      // int PositionPerc;
    },
    { // Motorgroup[4]
      omAuto, // TOperationMode OperationMode;
      0,      // int PositionTime;
      0,      // int PositionPerc;
    },
    { // Motorgroup[5]
      omAuto, // TOperationMode OperationMode;
      0,      // int PositionTime;
      0,      // int PositionPerc;
    },
    { // Motorgroup[6]
      omAuto, // TOperationMode OperationMode;
      0,      // int PositionTime;
      0,      // int PositionPerc;
    },
    { // Motorgroup[7]
      omAuto, // TOperationMode OperationMode;
      0,      // int PositionTime;
      0,      // int PositionPerc;
    },
    { // Motorgroup[8]
      omAuto, // TOperationMode OperationMode;
      0,      // int PositionTime;
      0,      // int PositionPerc;
    },
    { // Motorgroup[9]
      omAuto, // TOperationMode OperationMode;
      0,      // int PositionTime;
      0,      // int PositionPerc;
    },
    { // Motorgroup[10]
      omAuto, // TOperationMode OperationMode;
      0,      // int PositionTime;
      0,      // int PositionPerc;
    },
    { // Motorgroup[11]
      omAuto, // TOperationMode OperationMode;
      0,      // int PositionTime;
      0,      // int PositionPerc;
    },
    { // Motorgroup[12]
      omAuto, // TOperationMode OperationMode;
      0,      // int PositionTime;
      0,      // int PositionPerc;
    },
    { // Motorgroup[13]
      omAuto, // TOperationMode OperationMode;
      0,      // int PositionTime;
      0,      // int PositionPerc;
    },
    { // Motorgroup[14]
      omAuto, // TOperationMode OperationMode;
      0,      // int PositionTime;
      0,      // int PositionPerc;
    },
    { // Motorgroup[15]
      omAuto, // TOperationMode OperationMode;
      0,      // int PositionTime;
      0,      // int PositionPerc;
    },
    { // Motorgroup[16]
      omAuto, // TOperationMode OperationMode;
      0,      // int PositionTime;
      0,      // int PositionPerc;
    },
    { // Motorgroup[17]
      omAuto, // TOperationMode OperationMode;
      0,      // int PositionTime;
      0,      // int PositionPerc;
    },
    { // Motorgroup[18]
      omAuto, // TOperationMode OperationMode;
      0,      // int PositionTime;
      0,      // int PositionPerc;
    },
    { // Motorgroup[19]
      omAuto, // TOperationMode OperationMode;
      0,      // int PositionTime;
      0,      // int PositionPerc;
    },
    { // Motorgroup[20]
      omAuto, // TOperationMode OperationMode;
      0,      // int PositionTime;
      0,      // int PositionPerc;
    },
    { // Motorgroup[21]
      omAuto, // TOperationMode OperationMode;
      0,      // int PositionTime;
      0,      // int PositionPerc;
    },
    { // Motorgroup[22]
      omAuto, // TOperationMode OperationMode;
      0,      // int PositionTime;
      0,      // int PositionPerc;
    },
    { // Motorgroup[23]
      omAuto, // TOperationMode OperationMode;
      0,      // int PositionTime;
      0,      // int PositionPerc;
    },
    { // Motorgroup[24]
      omAuto, // TOperationMode OperationMode;
      0,      // int PositionTime;
      0,      // int PositionPerc;
    },
    { // Motorgroup[25]
      omAuto, // TOperationMode OperationMode;
      0,      // int PositionTime;
      0,      // int PositionPerc;
    },
    { // Motorgroup[26]
      omAuto, // TOperationMode OperationMode;
      0,      // int PositionTime;
      0,      // int PositionPerc;
    },
    { // Motorgroup[27]
      omAuto, // TOperationMode OperationMode;
      0,      // int PositionTime;
      0,      // int PositionPerc;
    },
    { // Motorgroup[28]
      omAuto, // TOperationMode OperationMode;
      0,      // int PositionTime;
      0,      // int PositionPerc;
    },
    { // Motorgroup[29]
      omAuto, // TOperationMode OperationMode;
      0,      // int PositionTime;
      0,      // int PositionPerc;
    },
    { // Motorgroup[30]
      omAuto, // TOperationMode OperationMode;
      0,      // int PositionTime;
      0,      // int PositionPerc;
    },
    { // Motorgroup[31]
      omAuto, // TOperationMode OperationMode;
      0,      // int PositionTime;
      0,      // int PositionPerc;
    },
  },

  { // sValHrMotor Motor[MAX_MOTOR];
    { // Motor[0]
      omAuto, // TOperationMode OperationMode;
      0, // unsigned char Overruled;
      0, // int Position;
      0, // int PositionAuto;
      0, // int PositionManual;
    },
    { // Motor[1]
      omAuto, // TOperationMode OperationMode;
      0, // unsigned char Overruled;
      0, // int Position;
      0, // int PositionAuto;
      0, // int PositionManual;
    },
    { // Motor[2]
      omAuto, // TOperationMode OperationMode;
      0, // unsigned char Overruled;
      0, // int Position;
      0, // int PositionAuto;
      0, // int PositionManual;
    },
    { // Motor[3]
      omAuto, // TOperationMode OperationMode;
      0, // unsigned char Overruled;
      0, // int Position;
      0, // int PositionAuto;
      0, // int PositionManual;
    },
    { // Motor[4]
      omAuto, // TOperationMode OperationMode;
      0, // unsigned char Overruled;
      0, // int Position;
      0, // int PositionAuto;
      0, // int PositionManual;
    },
    { // Motor[5]
      omAuto, // TOperationMode OperationMode;
      0, // unsigned char Overruled;
      0, // int Position;
      0, // int PositionAuto;
      0, // int PositionManual;
    },
    { // Motor[6]
      omAuto, // TOperationMode OperationMode;
      0, // unsigned char Overruled;
      0, // int Position;
      0, // int PositionAuto;
      0, // int PositionManual;
    },
    { // Motor[7]
      omAuto, // TOperationMode OperationMode;
      0, // unsigned char Overruled;
      0, // int Position;
      0, // int PositionAuto;
      0, // int PositionManual;
    },
    { // Motor[8]
      omAuto, // TOperationMode OperationMode;
      0, // unsigned char Overruled;
      0, // int Position;
      0, // int PositionAuto;
      0, // int PositionManual;
    },
    { // Motor[9]
      omAuto, // TOperationMode OperationMode;
      0, // unsigned char Overruled;
      0, // int Position;
      0, // int PositionAuto;
      0, // int PositionManual;
    },
    { // Motor[10]
      omAuto, // TOperationMode OperationMode;
      0, // unsigned char Overruled;
      0, // int Position;
      0, // int PositionAuto;
      0, // int PositionManual;
    },
    { // Motor[11]
      omAuto, // TOperationMode OperationMode;
      0, // unsigned char Overruled;
      0, // int Position;
      0, // int PositionAuto;
      0, // int PositionManual;
    },
    { // Motor[12]
      omAuto, // TOperationMode OperationMode;
      0, // unsigned char Overruled;
      0, // int Position;
      0, // int PositionAuto;
      0, // int PositionManual;
    },
    { // Motor[13]
      omAuto, // TOperationMode OperationMode;
      0, // unsigned char Overruled;
      0, // int Position;
      0, // int PositionAuto;
      0, // int PositionManual;
    },
    { // Motor[14]
      omAuto, // TOperationMode OperationMode;
      0, // unsigned char Overruled;
      0, // int Position;
      0, // int PositionAuto;
      0, // int PositionManual;
    },
    { // Motor[15]
      omAuto, // TOperationMode OperationMode;
      0, // unsigned char Overruled;
      0, // int Position;
      0, // int PositionAuto;
      0, // int PositionManual;
    },
    { // Motor[16]
      omAuto, // TOperationMode OperationMode;
      0, // unsigned char Overruled;
      0, // int Position;
      0, // int PositionAuto;
      0, // int PositionManual;
    },
    { // Motor[17]
      omAuto, // TOperationMode OperationMode;
      0, // unsigned char Overruled;
      0, // int Position;
      0, // int PositionAuto;
      0, // int PositionManual;
    },
    { // Motor[18]
      omAuto, // TOperationMode OperationMode;
      0, // unsigned char Overruled;
      0, // int Position;
      0, // int PositionAuto;
      0, // int PositionManual;
    },
    { // Motor[19]
      omAuto, // TOperationMode OperationMode;
      0, // unsigned char Overruled;
      0, // int Position;
      0, // int PositionAuto;
      0, // int PositionManual;
    },
    { // Motor[20]
      omAuto, // TOperationMode OperationMode;
      0, // unsigned char Overruled;
      0, // int Position;
      0, // int PositionAuto;
      0, // int PositionManual;
    },
    { // Motor[21]
      omAuto, // TOperationMode OperationMode;
      0, // unsigned char Overruled;
      0, // int Position;
      0, // int PositionAuto;
      0, // int PositionManual;
    },
    { // Motor[22]
      omAuto, // TOperationMode OperationMode;
      0, // unsigned char Overruled;
      0, // int Position;
      0, // int PositionAuto;
      0, // int PositionManual;
    },
    { // Motor[23]
      omAuto, // TOperationMode OperationMode;
      0, // unsigned char Overruled;
      0, // int Position;
      0, // int PositionAuto;
      0, // int PositionManual;
    },
    { // Motor[24]
      omAuto, // TOperationMode OperationMode;
      0, // unsigned char Overruled;
      0, // int Position;
      0, // int PositionAuto;
      0, // int PositionManual;
    },
    { // Motor[25]
      omAuto, // TOperationMode OperationMode;
      0, // unsigned char Overruled;
      0, // int Position;
      0, // int PositionAuto;
      0, // int PositionManual;
    },
    { // Motor[26]
      omAuto, // TOperationMode OperationMode;
      0, // unsigned char Overruled;
      0, // int Position;
      0, // int PositionAuto;
      0, // int PositionManual;
    },
    { // Motor[27]
      omAuto, // TOperationMode OperationMode;
      0, // unsigned char Overruled;
      0, // int Position;
      0, // int PositionAuto;
      0, // int PositionManual;
    },
    { // Motor[28]
      omAuto, // TOperationMode OperationMode;
      0, // unsigned char Overruled;
      0, // int Position;
      0, // int PositionAuto;
      0, // int PositionManual;
    },
    { // Motor[29]
      omAuto, // TOperationMode OperationMode;
      0, // unsigned char Overruled;
      0, // int Position;
      0, // int PositionAuto;
      0, // int PositionManual;
    },
    { // Motor[30]
      omAuto, // TOperationMode OperationMode;
      0, // unsigned char Overruled;
      0, // int Position;
      0, // int PositionAuto;
      0, // int PositionManual;
    },
    { // Motor[31]
      omAuto, // TOperationMode OperationMode;
      0, // unsigned char Overruled;
      0, // int Position;
      0, // int PositionAuto;
      0, // int PositionManual;
    },
    { // Motor[32]
      omAuto, // TOperationMode OperationMode;
      0, // unsigned char Overruled;
      0, // int Position;
      0, // int PositionAuto;
      0, // int PositionManual;
    },
    { // Motor[33]
      omAuto, // TOperationMode OperationMode;
      0, // unsigned char Overruled;
      0, // int Position;
      0, // int PositionAuto;
      0, // int PositionManual;
    },
    { // Motor[34]
      omAuto, // TOperationMode OperationMode;
      0, // unsigned char Overruled;
      0, // int Position;
      0, // int PositionAuto;
      0, // int PositionManual;
    },
    { // Motor[35]
      omAuto, // TOperationMode OperationMode;
      0, // unsigned char Overruled;
      0, // int Position;
      0, // int PositionAuto;
      0, // int PositionManual;
    },
    { // Motor[36]
      omAuto, // TOperationMode OperationMode;
      0, // unsigned char Overruled;
      0, // int Position;
      0, // int PositionAuto;
      0, // int PositionManual;
    },
    { // Motor[37]
      omAuto, // TOperationMode OperationMode;
      0, // unsigned char Overruled;
      0, // int Position;
      0, // int PositionAuto;
      0, // int PositionManual;
    },
    { // Motor[38]
      omAuto, // TOperationMode OperationMode;
      0, // unsigned char Overruled;
      0, // int Position;
      0, // int PositionAuto;
      0, // int PositionManual;
    },
    { // Motor[39]
      omAuto, // TOperationMode OperationMode;
      0, // unsigned char Overruled;
      0, // int Position;
      0, // int PositionAuto;
      0, // int PositionManual;
    },
    { // Motor[40]
      omAuto, // TOperationMode OperationMode;
      0, // unsigned char Overruled;
      0, // int Position;
      0, // int PositionAuto;
      0, // int PositionManual;
    },
    { // Motor[41]
      omAuto, // TOperationMode OperationMode;
      0, // unsigned char Overruled;
      0, // int Position;
      0, // int PositionAuto;
      0, // int PositionManual;
    },
    { // Motor[42]
      omAuto, // TOperationMode OperationMode;
      0, // unsigned char Overruled;
      0, // int Position;
      0, // int PositionAuto;
      0, // int PositionManual;
    },
    { // Motor[43]
      omAuto, // TOperationMode OperationMode;
      0, // unsigned char Overruled;
      0, // int Position;
      0, // int PositionAuto;
      0, // int PositionManual;
    },
    { // Motor[44]
      omAuto, // TOperationMode OperationMode;
      0, // unsigned char Overruled;
      0, // int Position;
      0, // int PositionAuto;
      0, // int PositionManual;
    },
    { // Motor[45]
      omAuto, // TOperationMode OperationMode;
      0, // unsigned char Overruled;
      0, // int Position;
      0, // int PositionAuto;
      0, // int PositionManual;
    },
    { // Motor[46]
      omAuto, // TOperationMode OperationMode;
      0, // unsigned char Overruled;
      0, // int Position;
      0, // int PositionAuto;
      0, // int PositionManual;
    },
    { // Motor[47]
      omAuto, // TOperationMode OperationMode;
      0, // unsigned char Overruled;
      0, // int Position;
      0, // int PositionAuto;
      0, // int PositionManual;
    },
    { // Motor[48]
      omAuto, // TOperationMode OperationMode;
      0, // unsigned char Overruled;
      0, // int Position;
      0, // int PositionAuto;
      0, // int PositionManual;
    },
    { // Motor[49]
      omAuto, // TOperationMode OperationMode;
      0, // unsigned char Overruled;
      0, // int Position;
      0, // int PositionAuto;
      0, // int PositionManual;
    },
    { // Motor[50]
      omAuto, // TOperationMode OperationMode;
      0, // unsigned char Overruled;
      0, // int Position;
      0, // int PositionAuto;
      0, // int PositionManual;
    },
    { // Motor[51]
      omAuto, // TOperationMode OperationMode;
      0, // unsigned char Overruled;
      0, // int Position;
      0, // int PositionAuto;
      0, // int PositionManual;
    },
    { // Motor[52]
      omAuto, // TOperationMode OperationMode;
      0, // unsigned char Overruled;
      0, // int Position;
      0, // int PositionAuto;
      0, // int PositionManual;
    },
    { // Motor[53]
      omAuto, // TOperationMode OperationMode;
      0, // unsigned char Overruled;
      0, // int Position;
      0, // int PositionAuto;
      0, // int PositionManual;
    },
    { // Motor[54]
      omAuto, // TOperationMode OperationMode;
      0, // unsigned char Overruled;
      0, // int Position;
      0, // int PositionAuto;
      0, // int PositionManual;
    },
    { // Motor[55]
      omAuto, // TOperationMode OperationMode;
      0, // unsigned char Overruled;
      0, // int Position;
      0, // int PositionAuto;
      0, // int PositionManual;
    },
    { // Motor[56]
      omAuto, // TOperationMode OperationMode;
      0, // unsigned char Overruled;
      0, // int Position;
      0, // int PositionAuto;
      0, // int PositionManual;
    },
    { // Motor[57]
      omAuto, // TOperationMode OperationMode;
      0, // unsigned char Overruled;
      0, // int Position;
      0, // int PositionAuto;
      0, // int PositionManual;
    },
    { // Motor[58]
      omAuto, // TOperationMode OperationMode;
      0, // unsigned char Overruled;
      0, // int Position;
      0, // int PositionAuto;
      0, // int PositionManual;
    },
    { // Motor[59]
      omAuto, // TOperationMode OperationMode;
      0, // unsigned char Overruled;
      0, // int Position;
      0, // int PositionAuto;
      0, // int PositionManual;
    },
    { // Motor[60]
      omAuto, // TOperationMode OperationMode;
      0, // unsigned char Overruled;
      0, // int Position;
      0, // int PositionAuto;
      0, // int PositionManual;
    },
    { // Motor[61]
      omAuto, // TOperationMode OperationMode;
      0, // unsigned char Overruled;
      0, // int Position;
      0, // int PositionAuto;
      0, // int PositionManual;
    },
    { // Motor[62]
      omAuto, // TOperationMode OperationMode;
      0, // unsigned char Overruled;
      0, // int Position;
      0, // int PositionAuto;
      0, // int PositionManual;
    },
    { // Motor[63]
      omAuto, // TOperationMode OperationMode;
      0, // unsigned char Overruled;
      0, // int Position;
      0, // int PositionAuto;
      0, // int PositionManual;
    },
  },

  { // sValHrDualScreen DualScreen[MAX_SCREEN];
    { // DualScreen[0];
      0, // int PositionGroupA; // [x.x %]
      0, // int PositionGroupB; // [x.x %]
      0, // int TimerStandby;   // [s]
      0, // int Master;         // 0 = A is master, 1 = B is master
    },
    { // DualScreen[1];
      0, // int PositionGroupA; // [x.x %]
      0, // int PositionGroupB; // [x.x %]
      0, // int TimerStandby;   // [s]
      0, // int Master;         // 0 = A is master, 1 = B is master
    },
    { // DualScreen[2];
      0, // int PositionGroupA; // [x.x %]
      0, // int PositionGroupB; // [x.x %]
      0, // int TimerStandby;   // [s]
      0, // int Master;         // 0 = A is master, 1 = B is master
    },
    { // DualScreen[3];
      0, // int PositionGroupA; // [x.x %]
      0, // int PositionGroupB; // [x.x %]
      0, // int TimerStandby;   // [s]
      0, // int Master;         // 0 = A is master, 1 = B is master
    },
    { // DualScreen[4];
      0, // int PositionGroupA; // [x.x %]
      0, // int PositionGroupB; // [x.x %]
      0, // int TimerStandby;   // [s]
      0, // int Master;         // 0 = A is master, 1 = B is master
    },
    { // DualScreen[5];
      0, // int PositionGroupA; // [x.x %]
      0, // int PositionGroupB; // [x.x %]
      0, // int TimerStandby;   // [s]
      0, // int Master;         // 0 = A is master, 1 = B is master
    },
    { // DualScreen[6];
      0, // int PositionGroupA; // [x.x %]
      0, // int PositionGroupB; // [x.x %]
      0, // int TimerStandby;   // [s]
      0, // int Master;         // 0 = A is master, 1 = B is master
    },
    { // DualScreen[7];
      0, // int PositionGroupA; // [x.x %]
      0, // int PositionGroupB; // [x.x %]
      0, // int TimerStandby;   // [s]
      0, // int Master;         // 0 = A is master, 1 = B is master
    },
  },

  { // sValHrDevice Device[MAX_DEVICE];
    { // Device[0]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[1]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[2]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[3]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[4]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[5]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[6]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[7]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[8]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[9]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[10]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[11]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[12]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[13]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[14]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[15]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[16]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[17]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[18]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[19]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[20]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[21]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[22]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[23]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[24]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[25]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[26]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[27]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[28]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[29]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[30]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[31]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[32]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[33]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[34]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[35]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[36]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[37]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[38]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[39]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[40]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[41]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[42]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[43]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[44]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[45]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[46]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[47]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[48]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[49]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[50]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[51]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[52]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[53]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[54]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[55]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[56]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[57]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[58]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[59]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[60]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[61]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[62]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[63]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[64]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[65]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[66]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[67]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[68]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[69]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[70]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[71]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[72]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[73]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[74]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[75]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[76]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[77]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[78]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[79]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[80]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[81]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[82]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[83]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[84]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[85]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[86]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[87]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[88]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[89]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[90]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[91]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[92]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[93]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[94]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[95]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[96]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[97]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[98]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[99]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[100]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[101]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[102]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[103]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[104]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[105]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[106]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[107]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[108]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[109]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[110]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[111]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[112]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[113]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[114]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[115]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[116]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[117]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[118]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[119]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[120]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[121]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[122]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[123]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[124]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[125]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[126]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[127]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[128]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[129]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[130]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[131]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[132]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[133]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[134]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[135]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[136]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[137]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[138]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[139]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[140]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[141]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[142]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[143]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[144]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[145]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[146]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[147]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[148]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[149]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[150]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[151]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[152]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[153]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[154]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[155]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[156]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[157]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[158]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[159]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[160]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[161]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[162]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[163]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[164]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[165]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[166]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[167]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[168]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[169]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[170]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[171]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[172]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[173]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[174]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[175]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[176]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[177]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[178]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[179]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[180]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[181]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[182]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[183]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[184]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[185]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[186]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[187]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[188]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[189]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[190]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[191]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[192]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[193]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[194]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[195]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[196]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[197]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[198]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[199]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[200]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[201]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[202]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[203]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[204]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[205]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[206]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[207]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[208]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[209]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[210]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[211]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[212]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[213]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[214]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[215]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[216]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[217]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[218]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[219]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[220]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[221]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[222]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[223]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[224]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[225]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[226]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[227]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[228]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[229]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[230]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[231]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[232]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[233]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[234]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[235]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[236]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[237]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[238]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[239]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[240]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[241]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[242]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[243]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[244]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[245]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[246]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[247]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[248]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[249]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[250]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[251]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[252]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[253]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[254]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
    { // Device[255]
      omAuto, // TOperationMode OperationMode;
      0,      // unsigned char SetPosition;
      0,      // unsigned char GetPosition;
    },
  },

  { // TValHrLuchtmengkastGroep LuchtmengkastGroep[MAX_LUCHTMENGKAST_GROEP];
    { // LuchtmengkastGroep[0]
      omAuto, // TOperationMode OperationMode;
      {0,0},  // TValHrServoIn  Inblaasvent;
      {0,0},  // TValHrServoIn  Afblaasvent;
      {0,0},  // TValHrServoIn  Binnenklep;
      {0,0},  // TValHrServoIn  Buitenklep;
      {0,0},  // TValHrServoIn  Verwarming;
    },
    { // LuchtmengkastGroep[1]
      omAuto, // TOperationMode OperationMode;
      {0,0},  // TValHrServoIn  Inblaasvent;
      {0,0},  // TValHrServoIn  Afblaasvent;
      {0,0},  // TValHrServoIn  Binnenklep;
      {0,0},  // TValHrServoIn  Buitenklep;
      {0,0},  // TValHrServoIn  Verwarming;
    },
    { // LuchtmengkastGroep[2]
      omAuto, // TOperationMode OperationMode;
      {0,0},  // TValHrServoIn  Inblaasvent;
      {0,0},  // TValHrServoIn  Afblaasvent;
      {0,0},  // TValHrServoIn  Binnenklep;
      {0,0},  // TValHrServoIn  Buitenklep;
      {0,0},  // TValHrServoIn  Verwarming;
    },
    { // LuchtmengkastGroep[3]
      omAuto, // TOperationMode OperationMode;
      {0,0},  // TValHrServoIn  Inblaasvent;
      {0,0},  // TValHrServoIn  Afblaasvent;
      {0,0},  // TValHrServoIn  Binnenklep;
      {0,0},  // TValHrServoIn  Buitenklep;
      {0,0},  // TValHrServoIn  Verwarming;
    },
  },

  { // TValHrLuchtmengkast Luchtmengkast[MAX_LUCHTMENGKAST];
    { // Luchtmengkast[0]
      omAuto,    // TOperationMode OperationMode;
      0,         // int Inblaastemp;
      0,         // int Mengtemp;
      0,         // unsigned char VorstBewaking;
      {0,0,0,0}, // unsigned char Inblaasvent;
      {0,0,0,0}, // unsigned char Afblaasvent;
      {0,0,0,0}, // unsigned char Binnenklep;
      {0,0,0,0}, // unsigned char Buitenklep;
      {0,0,0,0}, // unsigned char Verwarming;
    },
    { // Luchtmengkast[1]
      omAuto,    // TOperationMode OperationMode;
      0,         // int Inblaastemp;
      0,         // int Mengtemp;
      0,         // unsigned char VorstBewaking;
      {0,0,0,0}, // unsigned char Inblaasvent;
      {0,0,0,0}, // unsigned char Afblaasvent;
      {0,0,0,0}, // unsigned char Binnenklep;
      {0,0,0,0}, // unsigned char Buitenklep;
      {0,0,0,0}, // unsigned char Verwarming;
    },
    { // Luchtmengkast[2]
      omAuto,    // TOperationMode OperationMode;
      0,         // int Inblaastemp;
      0,         // int Mengtemp;
      0,         // unsigned char VorstBewaking;
      {0,0,0,0}, // unsigned char Inblaasvent;
      {0,0,0,0}, // unsigned char Afblaasvent;
      {0,0,0,0}, // unsigned char Binnenklep;
      {0,0,0,0}, // unsigned char Buitenklep;
      {0,0,0,0}, // unsigned char Verwarming;
    },
    { // Luchtmengkast[3]
      omAuto,    // TOperationMode OperationMode;
      0,         // int Inblaastemp;
      0,         // int Mengtemp;
      0,         // unsigned char VorstBewaking;
      {0,0,0,0}, // unsigned char Inblaasvent;
      {0,0,0,0}, // unsigned char Afblaasvent;
      {0,0,0,0}, // unsigned char Binnenklep;
      {0,0,0,0}, // unsigned char Buitenklep;
      {0,0,0,0}, // unsigned char Verwarming;
    },
    { // Luchtmengkast[4]
      omAuto,    // TOperationMode OperationMode;
      0,         // int Inblaastemp;
      0,         // int Mengtemp;
      0,         // unsigned char VorstBewaking;
      {0,0,0,0}, // unsigned char Inblaasvent;
      {0,0,0,0}, // unsigned char Afblaasvent;
      {0,0,0,0}, // unsigned char Binnenklep;
      {0,0,0,0}, // unsigned char Buitenklep;
      {0,0,0,0}, // unsigned char Verwarming;
    },
    { // Luchtmengkast[5]
      omAuto,    // TOperationMode OperationMode;
      0,         // int Inblaastemp;
      0,         // int Mengtemp;
      0,         // unsigned char VorstBewaking;
      {0,0,0,0}, // unsigned char Inblaasvent;
      {0,0,0,0}, // unsigned char Afblaasvent;
      {0,0,0,0}, // unsigned char Binnenklep;
      {0,0,0,0}, // unsigned char Buitenklep;
      {0,0,0,0}, // unsigned char Verwarming;
    },
    { // Luchtmengkast[6]
      omAuto,    // TOperationMode OperationMode;
      0,         // int Inblaastemp;
      0,         // int Mengtemp;
      0,         // unsigned char VorstBewaking;
      {0,0,0,0}, // unsigned char Inblaasvent;
      {0,0,0,0}, // unsigned char Afblaasvent;
      {0,0,0,0}, // unsigned char Binnenklep;
      {0,0,0,0}, // unsigned char Buitenklep;
      {0,0,0,0}, // unsigned char Verwarming;
    },
    { // Luchtmengkast[7]
      omAuto,    // TOperationMode OperationMode;
      0,         // int Inblaastemp;
      0,         // int Mengtemp;
      0,         // unsigned char VorstBewaking;
      {0,0,0,0}, // unsigned char Inblaasvent;
      {0,0,0,0}, // unsigned char Afblaasvent;
      {0,0,0,0}, // unsigned char Binnenklep;
      {0,0,0,0}, // unsigned char Buitenklep;
      {0,0,0,0}, // unsigned char Verwarming;
    },
    { // Luchtmengkast[8]
      omAuto,    // TOperationMode OperationMode;
      0,         // int Inblaastemp;
      0,         // int Mengtemp;
      0,         // unsigned char VorstBewaking;
      {0,0,0,0}, // unsigned char Inblaasvent;
      {0,0,0,0}, // unsigned char Afblaasvent;
      {0,0,0,0}, // unsigned char Binnenklep;
      {0,0,0,0}, // unsigned char Buitenklep;
      {0,0,0,0}, // unsigned char Verwarming;
    },
    { // Luchtmengkast[9]
      omAuto,    // TOperationMode OperationMode;
      0,         // int Inblaastemp;
      0,         // int Mengtemp;
      0,         // unsigned char VorstBewaking;
      {0,0,0,0}, // unsigned char Inblaasvent;
      {0,0,0,0}, // unsigned char Afblaasvent;
      {0,0,0,0}, // unsigned char Binnenklep;
      {0,0,0,0}, // unsigned char Buitenklep;
      {0,0,0,0}, // unsigned char Verwarming;
    },
    { // Luchtmengkast[10]
      omAuto,    // TOperationMode OperationMode;
      0,         // int Inblaastemp;
      0,         // int Mengtemp;
      0,         // unsigned char VorstBewaking;
      {0,0,0,0}, // unsigned char Inblaasvent;
      {0,0,0,0}, // unsigned char Afblaasvent;
      {0,0,0,0}, // unsigned char Binnenklep;
      {0,0,0,0}, // unsigned char Buitenklep;
      {0,0,0,0}, // unsigned char Verwarming;
    },
    { // Luchtmengkast[11]
      omAuto,    // TOperationMode OperationMode;
      0,         // int Inblaastemp;
      0,         // int Mengtemp;
      0,         // unsigned char VorstBewaking;
      {0,0,0,0}, // unsigned char Inblaasvent;
      {0,0,0,0}, // unsigned char Afblaasvent;
      {0,0,0,0}, // unsigned char Binnenklep;
      {0,0,0,0}, // unsigned char Buitenklep;
      {0,0,0,0}, // unsigned char Verwarming;
    },
    { // Luchtmengkast[12]
      omAuto,    // TOperationMode OperationMode;
      0,         // int Inblaastemp;
      0,         // int Mengtemp;
      0,         // unsigned char VorstBewaking;
      {0,0,0,0}, // unsigned char Inblaasvent;
      {0,0,0,0}, // unsigned char Afblaasvent;
      {0,0,0,0}, // unsigned char Binnenklep;
      {0,0,0,0}, // unsigned char Buitenklep;
      {0,0,0,0}, // unsigned char Verwarming;
    },
    { // Luchtmengkast[13]
      omAuto,    // TOperationMode OperationMode;
      0,         // int Inblaastemp;
      0,         // int Mengtemp;
      0,         // unsigned char VorstBewaking;
      {0,0,0,0}, // unsigned char Inblaasvent;
      {0,0,0,0}, // unsigned char Afblaasvent;
      {0,0,0,0}, // unsigned char Binnenklep;
      {0,0,0,0}, // unsigned char Buitenklep;
      {0,0,0,0}, // unsigned char Verwarming;
    },
    { // Luchtmengkast[14]
      omAuto,    // TOperationMode OperationMode;
      0,         // int Inblaastemp;
      0,         // int Mengtemp;
      0,         // unsigned char VorstBewaking;
      {0,0,0,0}, // unsigned char Inblaasvent;
      {0,0,0,0}, // unsigned char Afblaasvent;
      {0,0,0,0}, // unsigned char Binnenklep;
      {0,0,0,0}, // unsigned char Buitenklep;
      {0,0,0,0}, // unsigned char Verwarming;
    },
    { // Luchtmengkast[15]
      omAuto,    // TOperationMode OperationMode;
      0,         // int Inblaastemp;
      0,         // int Mengtemp;
      0,         // unsigned char VorstBewaking;
      {0,0,0,0}, // unsigned char Inblaasvent;
      {0,0,0,0}, // unsigned char Afblaasvent;
      {0,0,0,0}, // unsigned char Binnenklep;
      {0,0,0,0}, // unsigned char Buitenklep;
      {0,0,0,0}, // unsigned char Verwarming;
    },
    { // Luchtmengkast[16]
      omAuto,    // TOperationMode OperationMode;
      0,         // int Inblaastemp;
      0,         // int Mengtemp;
      0,         // unsigned char VorstBewaking;
      {0,0,0,0}, // unsigned char Inblaasvent;
      {0,0,0,0}, // unsigned char Afblaasvent;
      {0,0,0,0}, // unsigned char Binnenklep;
      {0,0,0,0}, // unsigned char Buitenklep;
      {0,0,0,0}, // unsigned char Verwarming;
    },
    { // Luchtmengkast[17]
      omAuto,    // TOperationMode OperationMode;
      0,         // int Inblaastemp;
      0,         // int Mengtemp;
      0,         // unsigned char VorstBewaking;
      {0,0,0,0}, // unsigned char Inblaasvent;
      {0,0,0,0}, // unsigned char Afblaasvent;
      {0,0,0,0}, // unsigned char Binnenklep;
      {0,0,0,0}, // unsigned char Buitenklep;
      {0,0,0,0}, // unsigned char Verwarming;
    },
    { // Luchtmengkast[18]
      omAuto,    // TOperationMode OperationMode;
      0,         // int Inblaastemp;
      0,         // int Mengtemp;
      0,         // unsigned char VorstBewaking;
      {0,0,0,0}, // unsigned char Inblaasvent;
      {0,0,0,0}, // unsigned char Afblaasvent;
      {0,0,0,0}, // unsigned char Binnenklep;
      {0,0,0,0}, // unsigned char Buitenklep;
      {0,0,0,0}, // unsigned char Verwarming;
    },
    { // Luchtmengkast[19]
      omAuto,    // TOperationMode OperationMode;
      0,         // int Inblaastemp;
      0,         // int Mengtemp;
      0,         // unsigned char VorstBewaking;
      {0,0,0,0}, // unsigned char Inblaasvent;
      {0,0,0,0}, // unsigned char Afblaasvent;
      {0,0,0,0}, // unsigned char Binnenklep;
      {0,0,0,0}, // unsigned char Buitenklep;
      {0,0,0,0}, // unsigned char Verwarming;
    },
    { // Luchtmengkast[20]
      omAuto,    // TOperationMode OperationMode;
      0,         // int Inblaastemp;
      0,         // int Mengtemp;
      0,         // unsigned char VorstBewaking;
      {0,0,0,0}, // unsigned char Inblaasvent;
      {0,0,0,0}, // unsigned char Afblaasvent;
      {0,0,0,0}, // unsigned char Binnenklep;
      {0,0,0,0}, // unsigned char Buitenklep;
      {0,0,0,0}, // unsigned char Verwarming;
    },
    { // Luchtmengkast[21]
      omAuto,    // TOperationMode OperationMode;
      0,         // int Inblaastemp;
      0,         // int Mengtemp;
      0,         // unsigned char VorstBewaking;
      {0,0,0,0}, // unsigned char Inblaasvent;
      {0,0,0,0}, // unsigned char Afblaasvent;
      {0,0,0,0}, // unsigned char Binnenklep;
      {0,0,0,0}, // unsigned char Buitenklep;
      {0,0,0,0}, // unsigned char Verwarming;
    },
    { // Luchtmengkast[22]
      omAuto,    // TOperationMode OperationMode;
      0,         // int Inblaastemp;
      0,         // int Mengtemp;
      0,         // unsigned char VorstBewaking;
      {0,0,0,0}, // unsigned char Inblaasvent;
      {0,0,0,0}, // unsigned char Afblaasvent;
      {0,0,0,0}, // unsigned char Binnenklep;
      {0,0,0,0}, // unsigned char Buitenklep;
      {0,0,0,0}, // unsigned char Verwarming;
    },
    { // Luchtmengkast[23]
      omAuto,    // TOperationMode OperationMode;
      0,         // int Inblaastemp;
      0,         // int Mengtemp;
      0,         // unsigned char VorstBewaking;
      {0,0,0,0}, // unsigned char Inblaasvent;
      {0,0,0,0}, // unsigned char Afblaasvent;
      {0,0,0,0}, // unsigned char Binnenklep;
      {0,0,0,0}, // unsigned char Buitenklep;
      {0,0,0,0}, // unsigned char Verwarming;
    },
    { // Luchtmengkast[24]
      omAuto,    // TOperationMode OperationMode;
      0,         // int Inblaastemp;
      0,         // int Mengtemp;
      0,         // unsigned char VorstBewaking;
      {0,0,0,0}, // unsigned char Inblaasvent;
      {0,0,0,0}, // unsigned char Afblaasvent;
      {0,0,0,0}, // unsigned char Binnenklep;
      {0,0,0,0}, // unsigned char Buitenklep;
      {0,0,0,0}, // unsigned char Verwarming;
    },
    { // Luchtmengkast[25]
      omAuto,    // TOperationMode OperationMode;
      0,         // int Inblaastemp;
      0,         // int Mengtemp;
      0,         // unsigned char VorstBewaking;
      {0,0,0,0}, // unsigned char Inblaasvent;
      {0,0,0,0}, // unsigned char Afblaasvent;
      {0,0,0,0}, // unsigned char Binnenklep;
      {0,0,0,0}, // unsigned char Buitenklep;
      {0,0,0,0}, // unsigned char Verwarming;
    },
    { // Luchtmengkast[26]
      omAuto,    // TOperationMode OperationMode;
      0,         // int Inblaastemp;
      0,         // int Mengtemp;
      0,         // unsigned char VorstBewaking;
      {0,0,0,0}, // unsigned char Inblaasvent;
      {0,0,0,0}, // unsigned char Afblaasvent;
      {0,0,0,0}, // unsigned char Binnenklep;
      {0,0,0,0}, // unsigned char Buitenklep;
      {0,0,0,0}, // unsigned char Verwarming;
    },
    { // Luchtmengkast[27]
      omAuto,    // TOperationMode OperationMode;
      0,         // int Inblaastemp;
      0,         // int Mengtemp;
      0,         // unsigned char VorstBewaking;
      {0,0,0,0}, // unsigned char Inblaasvent;
      {0,0,0,0}, // unsigned char Afblaasvent;
      {0,0,0,0}, // unsigned char Binnenklep;
      {0,0,0,0}, // unsigned char Buitenklep;
      {0,0,0,0}, // unsigned char Verwarming;
    },
    { // Luchtmengkast[28]
      omAuto,    // TOperationMode OperationMode;
      0,         // int Inblaastemp;
      0,         // int Mengtemp;
      0,         // unsigned char VorstBewaking;
      {0,0,0,0}, // unsigned char Inblaasvent;
      {0,0,0,0}, // unsigned char Afblaasvent;
      {0,0,0,0}, // unsigned char Binnenklep;
      {0,0,0,0}, // unsigned char Buitenklep;
      {0,0,0,0}, // unsigned char Verwarming;
    },
    { // Luchtmengkast[29]
      omAuto,    // TOperationMode OperationMode;
      0,         // int Inblaastemp;
      0,         // int Mengtemp;
      0,         // unsigned char VorstBewaking;
      {0,0,0,0}, // unsigned char Inblaasvent;
      {0,0,0,0}, // unsigned char Afblaasvent;
      {0,0,0,0}, // unsigned char Binnenklep;
      {0,0,0,0}, // unsigned char Buitenklep;
      {0,0,0,0}, // unsigned char Verwarming;
    },
    { // Luchtmengkast[30]
      omAuto,    // TOperationMode OperationMode;
      0,         // int Inblaastemp;
      0,         // int Mengtemp;
      0,         // unsigned char VorstBewaking;
      {0,0,0,0}, // unsigned char Inblaasvent;
      {0,0,0,0}, // unsigned char Afblaasvent;
      {0,0,0,0}, // unsigned char Binnenklep;
      {0,0,0,0}, // unsigned char Buitenklep;
      {0,0,0,0}, // unsigned char Verwarming;
    },
    { // Luchtmengkast[31]
      omAuto,    // TOperationMode OperationMode;
      0,         // int Inblaastemp;
      0,         // int Mengtemp;
      0,         // unsigned char VorstBewaking;
      {0,0,0,0}, // unsigned char Inblaasvent;
      {0,0,0,0}, // unsigned char Afblaasvent;
      {0,0,0,0}, // unsigned char Binnenklep;
      {0,0,0,0}, // unsigned char Buitenklep;
      {0,0,0,0}, // unsigned char Verwarming;
    },
  },

  { // TValHrSensoren Sensoren;
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0} // int Drukverschil[MAX_SENSOREN];
  },


  0xA5A5, // unsigned int control_1; // eerste controle getal

  { //  s_value_hr_sd_log sd_log;
    0,  // unsigned char changed;                    // er is iets veranderd file indien aanwezig omnoemen en starten met loggen met nieuwe gegevens
    0,  // unsigned char aantal_variabelen;          // aantal variabelen dat gelogd moet worden
    4,  // unsigned char length;                     // lengte regel
    0,  // int aantal_regels;                        // telt aantal regels om file te rename als maximum aantal regels bereikt is
    { //  unsigned int code[MAX_LOG_VALUES];  // variabelen regel;
      { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, 
      { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, 
      { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, 
      { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, 
      { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
    },
    60, // unsigned int interval_in_seconden;        // interval tussen metingen in seconden
    0,  // time_t start_tijd;
    0,  //time_t stop_tijd;
    { 0, 0, 0} // s_timer timer;
  },
};

unsigned char comp_ram_eep_switch = 1;
// unsigned char data_modules_beschadigd_flag = 0; // TD
// int cnt_write_time_eep; // Als dit permanent gebruikt wordt dan MOET het adres niet veranderen tijdens het upgrade van programma's // TD
// unsigned char pwr_down_while_write_eep_flag = 0; // TD

//-----------------------------------------------------------------------------
// Data in EEPROM split in to blocks of size EEP_BLOCK_SIZE
// last byte of data block is checksum
// In ram mirror, the checksum is not present
// data block in ram mirror is of size EEP_BLOCK_SIZE-1
unsigned char ChecksumBlock(unsigned char *data, unsigned int block_size)
// calculate data checksum for one block of data
{
unsigned char chk = 0;
unsigned int cnt = block_size-1;

  chk = 173;
  do
  {
    chk ^= *data++;
  }
  while (--cnt);
  return (chk);
}

//-----------------------------------------------------------------------------
bit Data_Block_Ram_Equal_EEP(unsigned char *ram_data, unsigned char *eep_data)
{
unsigned int cnt = EEP_BLOCK_SIZE-1;

  do
  {
    if (*ram_data != *eep_data++)
      return (0);
    ram_data++;
  }
  while (--cnt);
  return (1);
}
//-----------------------------------------------------------------------------
void CopyBlockToRam(unsigned char *ptr, unsigned char *d)
{
unsigned int cnt = EEP_BLOCK_SIZE-1;

  do
  {
    *ptr = *d++;
    ptr++;
  } 
  while (--cnt);
}
//-----------------------------------------------------------------------------
void BlockCopy(unsigned char *ptr_source,unsigned char *ptr_destination, unsigned int aantal)
{
  while (aantal--)
  {
    *ptr_destination++ = *ptr_source++;
  }
}
//-----------------------------------------------------------------------------
void Default_Modules(void)
{
  module = default_modules;
}

//-----------------------------------------------------------------------------
void IncrementOptionsChangeCount(void)
{
  opt_alg.change_cnt++;
  if (opt_alg.change_cnt == 0)
    opt_alg.change_cnt++;
  opt_app.change_cnt = opt_alg.change_cnt;
  opt_io.change_cnt  = opt_alg.change_cnt;
}

//-----------------------------------------------------------------------------
void Data_Default_Opt_Alg(void)
{
unsigned int help;

  help                 = opt_alg.change_cnt;
  opt_alg              = default_opt_alg;
  opt_alg.type         = module.type;
  opt_alg.firma        = module.firma;
  opt_alg.serie_number = module.serie_number;
  opt_alg.change_cnt   = help + 1;
  opt_alg.area_size    = (unsigned long)&default_opt_alg.end - (unsigned long)&default_opt_alg;

  opt_io              = default_opt_io;
  opt_io.type         = module.type;
  opt_io.firma        = module.firma;
  opt_io.serie_number = module.serie_number;
  opt_io.change_cnt   = opt_alg.change_cnt;
  opt_io.area_size    = (unsigned long)&default_opt_io.end - (unsigned long)&default_opt_io;

  opt_app              = default_opt_app;
  opt_app.type         = module.type;
  opt_app.firma        = module.firma;
  opt_app.serie_number = module.serie_number;
  opt_app.change_cnt   = opt_alg.change_cnt;
  opt_app.area_size    = (unsigned long)&default_opt_app.end - (unsigned long)&default_opt_app;

  pc_0_read_configuration_alg = 1;  
  pc_1_read_configuration_alg = 1;
  pc_2_read_configuration_alg = 1;
  rs232_0_read_configuration_alg = 1;
  rs232_1_read_configuration_alg = 1;
  #ifdef ETHERNET
  ethernet_read_configuration_alg[0] = 1;
  ethernet_read_configuration_alg[1] = 1;
  ethernet_read_configuration_alg[2] = 1;
  ethernet_read_configuration_alg[3] = 1;
  #endif // ETHERNET

  CreateAlarm(&alarm_hr_alg.opt_al, SYSTEEM_AL_OPT, 0, 0, 0, HARD_ALARM);
  ModuleCheck();
}

void Data_Default_Opt(void)
{
  Data_Default_Opt_Alg();
}
//-----------------------------------------------------------------------------
void Data_Default_Setp_Alg(void)
{
  setp_alg = default_setp_alg;
  setp_alg.type = module.type;
  setp_alg.firma = module.firma;
  setp_alg.serie_number = module.serie_number;
  CreateAlarm(&alarm_hr_alg.setp_al, SYSTEEM_AL_SETP, 0, 0, 0, HARD_ALARM);
  setp_alg.area_size = (unsigned long)&default_setp_alg.end - (unsigned long)&default_setp_alg;
}

void Data_Default_Setp(void)
{
  Data_Default_Setp_Alg();
}
//-----------------------------------------------------------------------------
void Data_Default_Val_Hr_Alg(void)
{
  val_hr_alg = default_val_hr_alg;
  CreateAlarm(&alarm_hr_alg.val_hr_al, SYSTEEM_AL_VAL_HR, 0, 0, 0, ZACHT_ALARM);
  val_hr_alg.area_size = sizeof(default_val_hr_alg);
}

void Data_Default_Val_Hr(void)
{
  Data_Default_Val_Hr_Alg();
}
//-----------------------------------------------------------------------------
void Data_Default_Alg(void)
{
  Data_Default_Val_Hr_Alg();
  ClearAlarm(&alarm_hr_alg.val_hr_al, SYSTEEM_AL_VAL_HR, 0);
  Data_Default_Setp_Alg();
  ClearAlarm(&alarm_hr_alg.setp_al, SYSTEEM_AL_SETP, 0);
  Data_Default_Opt_Alg();
}

void Data_Default(void)
{
  Alarm_Reset_Data_Algemeen();
  Data_Default_Alg();
}
//-----------------------------------------------------------------------------
unsigned char Check_RAM_Alg(void)
{
  if (val_hr_alg.control_0 != 0xA5A5)
  {
    return (1); // RAM beschadigd
  }
  else
  {
    return (0);
  }
}

//-----------------------------------------------------------------------------
/*
unsigned char CheckWriteEEP(void)
{
  if (Check_RAM_ALG() || RTC_voltage_low)
    return (0);
  else
  {
    if (cnt_write_time_eep)
      return (1);
    else
      return (0);
  }
}
*/
//-----------------------------------------------------------------------------
unsigned int Number_Of_Blocks(unsigned int aantal_bytes, unsigned int block_size)
{
unsigned int number_of_blocks;
  
  block_size--;
  number_of_blocks = aantal_bytes / block_size; // Laatste byte is de checksum
  if (aantal_bytes % block_size)
    number_of_blocks += 1;
  return (number_of_blocks);
}
//-----------------------------------------------------------------------------
unsigned char RestoreData(unsigned int block_nummer, unsigned int aantal_bytes)
// Controleer data in eeprom en copieer deze naar RAM
// return 0 als block goed gelezen en in ram geschreven
// return 1 als error bij lezen block
// return 2 als Checksum error
{
unsigned char temp_eep_block[EEP_BLOCK_SIZE];
unsigned int end_block;

  end_block = block_nummer + Number_Of_Blocks(aantal_bytes, EEP_BLOCK_SIZE);
  if (end_block > 512) // JP 18-09-07
    end_block = 512;    // JP 18-09-07

  do
  {
    if (EEP_Read_Block(block_nummer,temp_eep_block))
      return (1); // Read error eeprom

    if (temp_eep_block[EEP_BLOCK_SIZE-1] == ChecksumBlock(temp_eep_block, EEP_BLOCK_SIZE))
      CopyBlockToRam(((unsigned char *)&module)+block_nummer*(EEP_BLOCK_SIZE-1),temp_eep_block);
    else
    {
      return (2); // Checksum error
    }
    WDI_Trigger(); // JP 18-09-7
    block_nummer++;
  }
  while (block_nummer < end_block);
  return (0);
}
//-----------------------------------------------------------------------------
void AddNewData(unsigned long def_begin, unsigned long def_end, 
                unsigned long data_begin, unsigned int *data_size)
{
unsigned int aantal;

  aantal = def_end - def_begin - *data_size;
  BlockCopy((unsigned char *)def_begin + *data_size, 
            (unsigned char *)data_begin + *data_size,
            aantal);
  *data_size = def_end - def_begin;
}

void AddNewOptIO(void)
{
  AddNewData((unsigned long)&default_opt_io, (unsigned long)&default_opt_io.end, 
             (unsigned long)&opt_io, &opt_io.area_size);
}

void AddNewOptAlg(void)
{
  AddNewData((unsigned long)&default_opt_alg, (unsigned long)&default_opt_alg.end, 
             (unsigned long)&opt_alg, &opt_alg.area_size);
}

void AddNewOptApp(void)
{
  AddNewData((unsigned long)&default_opt_app, (unsigned long)&default_opt_app.end, 
             (unsigned long)&opt_app, &opt_app.area_size);
}

//-----------------------------------------------------------------------------
void AddNewSetpAlg(void)
{
  AddNewData((unsigned long)&default_setp_alg, (unsigned long)&default_setp_alg.end, 
             (unsigned long)&setp_alg, &setp_alg.area_size);
}

//-----------------------------------------------------------------------------
void AddNewValueHr(unsigned long def_begin, unsigned long def_size, 
                   unsigned long data_begin, unsigned int *data_size)
{
unsigned int aantal;

  aantal = def_size - *data_size;
  BlockCopy((unsigned char *)def_begin + *data_size, 
            (unsigned char *)data_begin + *data_size,
            aantal);
  *data_size = def_size;
}

void AddNewValueHrAlg(void)
{
  AddNewValueHr((unsigned long)&default_val_hr_alg, sizeof(default_val_hr_alg), 
                (unsigned long)&val_hr_alg, &val_hr_alg.area_size);
}

//-----------------------------------------------------------------------------
/*static unsigned char Check_EEP_Size(void)
{
static unsigned char eep_checked = 0;
unsigned char eep_block_000[EEP_BLOCK_SIZE];
unsigned char eep_block_512[EEP_BLOCK_SIZE];
unsigned char eep_foute_checksum;
unsigned int cnt[6] = {0,0,0,0,0,0};
s_timer EEP_timer;

  if(eep_checked == 0)
  {
    // 1e en 512e block uitlezen
    while(EEP_Read_Block(0, eep_block_000)) 
    {
      Timer_Set(&EEP_timer, 10, TIME_BASE_1_MSEC);
      while (!Timer_Expired(&EEP_timer));
      cnt[0]++;
      if(cnt[0] >= 200) 
        return(0);
    }
    while(EEP_Read_Block(512, eep_block_512)) 
    {
      Timer_Set(&EEP_timer, 10, TIME_BASE_1_MSEC);
      while (!Timer_Expired(&EEP_timer));
      cnt[1]++;
      if(cnt[1] >= 200) 
        return(0);
    }
    WDI_Trigger();		 
    if((eep_block_000[EEP_BLOCK_SIZE-1] == eep_block_512[EEP_BLOCK_SIZE-1]) && Data_Block_Ram_Equal_EEP(eep_block_000, eep_block_512))
    { // beide blokken exact gelijk
      // checksum van blok 512 beschadigen
	  eep_foute_checksum = ~eep_block_512[EEP_BLOCK_SIZE - 1];
      while(EEP_Write_Block(512, eep_block_512, eep_foute_checksum))
	  {
        Timer_Set(&EEP_timer, 10, TIME_BASE_1_MSEC);
        while (!Timer_Expired(&EEP_timer));
        cnt[2]++;
        if(cnt[2] >= 200) 
          return(0);
	  }
      // wacht tot schrijfactie gereed is en blok 0 uitlezen kan worden 
      while(EEP_Read_Block(0, eep_block_000))
      {
        Timer_Set(&EEP_timer, 10, TIME_BASE_1_MSEC);
        while (!Timer_Expired(&EEP_timer));
        cnt[3]++;
        if(cnt[3] >= 200) 
          return(0);
      }	 
      WDI_Trigger();		 
      // checksum van blok 512 herstellen
      while(EEP_Write_Block(512, eep_block_512, eep_block_512[EEP_BLOCK_SIZE - 1]))
	  {
        Timer_Set(&EEP_timer, 10, TIME_BASE_1_MSEC);
        while (!Timer_Expired(&EEP_timer));
        cnt[4]++;
        if(cnt[4] >= 200) 
          return(0);
	  }
	  if(eep_block_000[EEP_BLOCK_SIZE - 1] == eep_foute_checksum)
	  {	// checksum van blok 0 is ook fout
	    Data_Default();
	    CreateAlarm(&alarm_hr_alg.eeprom_256k_al, SYSTEEM_EEPROM_256K_AL, 0, 0, 0, HARD_ALARM);
	    eep_checked = 1;
	  }
	  else
	  {
	    ClearAlarm(&alarm_hr_alg.eeprom_256k_al, SYSTEEM_EEPROM_256K_AL, 0); 
	    eep_checked = 2;
	  }
      // wachten tot schrijf actie gereed is
      while(EEP_Read_Block(0, eep_block_000))
      {
        Timer_Set(&EEP_timer, 10, TIME_BASE_1_MSEC);
        while (!Timer_Expired(&EEP_timer));
        cnt[5]++;
        if(cnt[5] >= 200)
          return(0);
      }	
    }
    else
    {
	  ClearAlarm(&alarm_hr_alg.eeprom_256k_al, SYSTEEM_EEPROM_256K_AL, 0); 
      eep_checked = 2;
    }
  }
  return(eep_checked);
}					 */
//-----------------------------------------------------------------------------
static unsigned char Check_EEP_Size(void)
{
static unsigned char eep_checked = 0;
unsigned char eep_block_000[EEP_BLOCK_SIZE];
unsigned char eep_block_256[EEP_BLOCK_SIZE];
unsigned char eep_foute_checksum;
unsigned int cnt[6] = {0,0,0,0,0,0};
s_timer EEP_timer;

  if(eep_checked == 0)
  {
    // 1e en 256e block uitlezen
    while(EEP_Read_Block(0, eep_block_000)) 
    {
      Timer_Set(&EEP_timer, 10, TIME_BASE_1_MSEC);
      while (!Timer_Expired(&EEP_timer));
      cnt[0]++;
      if(cnt[0] >= 200) 
        return(0);
    }
    while(EEP_Read_Block(256, eep_block_256)) 
    {
      Timer_Set(&EEP_timer, 10, TIME_BASE_1_MSEC);
      while (!Timer_Expired(&EEP_timer));
      cnt[1]++;
      if(cnt[1] >= 200) 
        return(0);
    }
    WDI_Trigger();		 
    if((eep_block_000[EEP_BLOCK_SIZE-1] == eep_block_256[EEP_BLOCK_SIZE-1]) && Data_Block_Ram_Equal_EEP(eep_block_000, eep_block_256))
    { // beide blokken exact gelijk
      // checksum van blok 256 beschadigen
	  eep_foute_checksum = ~eep_block_256[EEP_BLOCK_SIZE - 1];
      while(EEP_Write_Block(256, eep_block_256, eep_foute_checksum))
	  {
        Timer_Set(&EEP_timer, 10, TIME_BASE_1_MSEC);
        while (!Timer_Expired(&EEP_timer));
        cnt[2]++;
        if(cnt[2] >= 200) 
          return(0);
	  }
      // wacht tot schrijfactie gereed is en blok 0 uitlezen kan worden 
      while(EEP_Read_Block(0, eep_block_000))
      {
        Timer_Set(&EEP_timer, 10, TIME_BASE_1_MSEC);
        while (!Timer_Expired(&EEP_timer));
        cnt[3]++;
        if(cnt[3] >= 200) 
          return(0);
      }	 
      WDI_Trigger();		 
      // checksum van blok 256 herstellen
      while(EEP_Write_Block(256, eep_block_256, eep_block_256[EEP_BLOCK_SIZE - 1]))
	  {
        Timer_Set(&EEP_timer, 10, TIME_BASE_1_MSEC);
        while (!Timer_Expired(&EEP_timer));
        cnt[4]++;
        if(cnt[4] >= 200) 
          return(0);
	  }
	  if((eep_block_000[EEP_BLOCK_SIZE - 1] == eep_foute_checksum) || (Data_Block_Ram_Equal_EEP(eep_block_000, eep_block_256) == 0))
	  {	// checksum van blok 0 is ook fout
	    Data_Default();
	    CreateAlarm(&alarm_hr_alg.eeprom_256k_al, SYSTEEM_EEPROM_256K_AL, 0, 0, 0, HARD_ALARM);
	    eep_checked = 1;
	  }
	  else
	  {
	    ClearAlarm(&alarm_hr_alg.eeprom_256k_al, SYSTEEM_EEPROM_256K_AL, 0); 
	    eep_checked = 2;
	  }
      // wachten tot schrijf actie gereed is
      while(EEP_Read_Block(0, eep_block_000))
      {
        Timer_Set(&EEP_timer, 10, TIME_BASE_1_MSEC);
        while (!Timer_Expired(&EEP_timer));
        cnt[5]++;
        if(cnt[5] >= 200)
          return(0);
      }	
    }
    else
    {
	  ClearAlarm(&alarm_hr_alg.eeprom_256k_al, SYSTEEM_EEPROM_256K_AL, 0); 
      eep_checked = 2;
    }
  }
  return(eep_checked);
}

unsigned char RestoreModuleArea(void)
// return 0: module uit eeprom is naar ram gekopieerd
//           bij checksum error module uit default rom is naar ram gekopieerd
// return 1: read error eeprom
{
  switch (RestoreData(OFFSET_MODULES,(EEP_BLOCK_SIZE-1)*2)) // check if module correct
  {
    case 0:
      break;
    case 1: // lees error
      return (1); // Read error eeprom
    case 2: // checksum error
      Default_Modules();
      break;
  }
  return (0);
}

unsigned char VersieAfhankelijkModule(void)
// mogelijkheid om module te controleren en of aan te passen bij een andere versie
{
  if ((module.computer != default_modules.computer) || (module.soort != default_modules.soort))
  {
    Default_Modules();
  }
  else 
  {
    // Bijwerken modules indien nodig
    if (module.versie_programma < 304)
    {
      switch (module.type)
      {
        default:
        case MODULE_BASIS:
          module.CanOpen    = 0;
          module.Ventilatie = 0;
          break;
        case MODULE_CANOPEN:
          module.CanOpen    = 1;
          module.Ventilatie = 0;
          break;
      }
    }
  }
  return(0);
}
//-----------------------------------------------------------------------------
unsigned char RestoreOptAreaIO(void)
// return 0 data area goed ingelezen
// return 1 read error eeprom
// return 2 default data terug gezet beeindig RestoreDataArea omdat opties en setpoints op default zijn gezet
{
unsigned int default_area_size = (unsigned long)&default_opt_io.end - (unsigned long)&default_opt_io;

  if(dont_restore_opt_setp & 0x01)
  {
    Data_Default_Opt_Alg();
    return(0);
  }
  switch (RestoreData(OFFSET_OPT_IO, (EEP_BLOCK_SIZE-1)))
  {
    case 0: // Restored from eeprom
      break;
    case 1: // Read error
      return (1);
    case 2: // Checksum error
      if (Check_RAM_Alg() || RTC_voltage_low) // ram niet meer betrouwbaar
      {
        Data_Default();
//        RTC_voltage_low = 0;
        return (2);
      }
      break;
  }
  if (opt_io.area_size > ((OFFSET_OPT_ALG - OFFSET_OPT_IO) * (unsigned int)(EEP_BLOCK_SIZE-1))) // optie gebied is groter dan maximum geheugen ruimte 
  {
    // opties beschadigd
    Data_Default();
    return (2);
  }
  else
  {
    if (opt_io.area_size > (EEP_BLOCK_SIZE-1))
    {
      switch (RestoreData(OFFSET_OPT_IO+1, opt_io.area_size - (EEP_BLOCK_SIZE-1)))
      {
        case 0: // Restored from eeprom
          break;
        case 1: // Read error
          return (1);
        case 2: // Checksum error
          if (Check_RAM_Alg() || RTC_voltage_low) // ram niet meer betrouwbaar
          {
            Data_Default();
//            RTC_voltage_low = 0;
            return (2);
          }
          break;
      }
    }
    if (default_area_size > opt_io.area_size) // nieuwe opties toegevoegd
    {
      AddNewOptIO();
      vlag_options_added = 1;
    }
    else
      vlag_options_added = 0;
  }

  return (0);
}

unsigned char VersieAfhankelijkOptIO(void)
// mogelijkheid om option te controleren en of aan te passen bij een andere versie
// hier ook controleren wat te doen als aantal options minder is geworden (vorige versie terug geplaatst)
// return 0 data area goed ingelezen
// return 1 default data terug gezet beeindig RestoreDataArea omdat alles op default is gezet
{
//unsigned char *ptr;
//unsigned int loop;
unsigned char help_options_added = vlag_options_added; // nodig voor voorlopige versies

  if (vlag_options_added == 1)  
  {
    IncrementOptionsChangeCount();
    pc_0_read_configuration_alg = 1;  
    pc_1_read_configuration_alg = 1;
    pc_2_read_configuration_alg = 1;
    rs232_0_read_configuration_alg = 1;
    rs232_1_read_configuration_alg = 1;
    #ifdef ETHERNET
    ethernet_read_configuration_alg[0] = 1;
    ethernet_read_configuration_alg[1] = 1;
    ethernet_read_configuration_alg[2] = 1;
    ethernet_read_configuration_alg[3] = 1;
    #endif // ETHERNET
    vlag_options_added = 0;
  }
  if ((opt_io.computer != default_opt_io.computer) ||
      (opt_io.soort != default_opt_io.soort))
  {
    Data_Default();
    opt_io.type = module.type;
    opt_io.firma = module.firma;
    opt_io.serie_number = module.serie_number;
    return (1);
  }
  else 
  {
    // Bijwerken opties indien nodig
    if ((opt_io.versie_programma < 400) &&// voorlopige versie default opties zodra optie gebied groter wordt
        (opt_io.versie_programma != default_opt_io.versie_programma)) 
    {
      if (help_options_added ||
          (opt_io.area_size != (unsigned long)&default_opt_io.end - (unsigned long)&default_opt_io))
      {
        Data_Default();
        opt_io.type = module.type;
        opt_io.firma = module.firma;
        opt_io.serie_number = module.serie_number;
        return (1);
      }
      opt_io.versie_programma = default_opt_io.versie_programma;
      opt_io.type = module.type;
      opt_io.firma = module.firma;
      opt_io.serie_number = module.serie_number;
      return (0);
    }
    if (opt_io.versie_programma < default_opt_io.versie_programma)
    {
      opt_io.area_size = (unsigned long)&default_opt_io.end - (unsigned long)&default_opt_io;
      opt_io.versie_programma = default_opt_io.versie_programma;
      IncrementOptionsChangeCount();
      pc_0_read_configuration_alg = 1;  
      pc_1_read_configuration_alg = 1;
      pc_2_read_configuration_alg = 1;
      rs232_0_read_configuration_alg = 1;
      rs232_1_read_configuration_alg = 1;
      #ifdef ETHERNET
      ethernet_read_configuration_alg[0] = 1;
      ethernet_read_configuration_alg[1] = 1;
      ethernet_read_configuration_alg[2] = 1;
      ethernet_read_configuration_alg[3] = 1;
      #endif // ETHERNET
    }
    else if (opt_io.versie_programma > default_opt_io.versie_programma)
    {
      opt_io.area_size = (unsigned long)&default_opt_io.end - (unsigned long)&default_opt_io;
      opt_io.versie_programma = default_opt_io.versie_programma;
      IncrementOptionsChangeCount();
      pc_0_read_configuration_alg = 1;  
      pc_1_read_configuration_alg = 1;
      pc_2_read_configuration_alg = 1;
      rs232_0_read_configuration_alg = 1;
      rs232_1_read_configuration_alg = 1;
      #ifdef ETHERNET
      ethernet_read_configuration_alg[0] = 1;
      ethernet_read_configuration_alg[1] = 1;
      ethernet_read_configuration_alg[2] = 1;
      ethernet_read_configuration_alg[3] = 1;
      #endif // ETHERNET
    }
    if (opt_io.type != module.type)
    {
      opt_io.type = module.type;
      IncrementOptionsChangeCount();
      pc_0_read_configuration_alg = 1;  
      pc_1_read_configuration_alg = 1;
      pc_2_read_configuration_alg = 1;
      rs232_0_read_configuration_alg = 1;
      rs232_1_read_configuration_alg = 1;
      #ifdef ETHERNET
      ethernet_read_configuration_alg[0] = 1;
      ethernet_read_configuration_alg[1] = 1;
      ethernet_read_configuration_alg[2] = 1;
      ethernet_read_configuration_alg[3] = 1;
      #endif // ETHERNET
    }
    if (opt_io.firma != module.firma)
    {
      opt_io.firma = module.firma;
      IncrementOptionsChangeCount();
      pc_0_read_configuration_alg = 1;  
      pc_1_read_configuration_alg = 1;
      pc_2_read_configuration_alg = 1;
      rs232_0_read_configuration_alg = 1;
      rs232_1_read_configuration_alg = 1;
      #ifdef ETHERNET
      ethernet_read_configuration_alg[0] = 1;
      ethernet_read_configuration_alg[1] = 1;
      ethernet_read_configuration_alg[2] = 1;
      ethernet_read_configuration_alg[3] = 1;
      #endif // ETHERNET
    }
    if (opt_io.serie_number != module.serie_number)
    {
      opt_io.serie_number = module.serie_number;
      IncrementOptionsChangeCount();
      pc_0_read_configuration_alg = 1;  
      pc_1_read_configuration_alg = 1;
      pc_2_read_configuration_alg = 1;
      rs232_0_read_configuration_alg = 1;
      rs232_1_read_configuration_alg = 1;
      #ifdef ETHERNET
      ethernet_read_configuration_alg[0] = 1;
      ethernet_read_configuration_alg[1] = 1;
      ethernet_read_configuration_alg[2] = 1;
      ethernet_read_configuration_alg[3] = 1;
      #endif // ETHERNET
    }
    if (opt_io.versie_PC != default_opt_io.versie_PC)
      opt_io.versie_PC = default_opt_io.versie_PC;
  }
  return(0);
}
//-----------------------------------------------------------------------------
unsigned char RestoreOptAreaAlg(void)
// return 0 data area goed ingelezen
// return 1 read error eeprom
// return 2 default data terug gezet beeindig RestoreDataArea omdat opties en setpoints op default zijn gezet
{
unsigned int default_area_size = (unsigned long)&default_opt_alg.end - (unsigned long)&default_opt_alg;

  if(dont_restore_opt_setp & 0x01)
  {
    Data_Default_Opt_Alg();
    return(0);
  }
  switch (RestoreData(OFFSET_OPT_ALG, (EEP_BLOCK_SIZE-1)))
  {
    case 0: // Restored from eeprom
      break;
    case 1: // Read error
      return (1);
    case 2: // Checksum error
      if (Check_RAM_Alg() || RTC_voltage_low) // ram niet meer betrouwbaar
      {
        Data_Default();
//        RTC_voltage_low = 0;
        return (2);
      }
      break;
  }
  if (opt_alg.area_size > ((OFFSET_OPT_APP - OFFSET_OPT_ALG) * (unsigned int)(EEP_BLOCK_SIZE-1))) // optie gebied is groter dan maximum geheugen ruimte 
  {
    // opties beschadigd
    Data_Default();
    return (2);
  }
  else
  {
    if (opt_alg.area_size > (EEP_BLOCK_SIZE-1))
    {
      switch (RestoreData(OFFSET_OPT_ALG+1, opt_alg.area_size - (EEP_BLOCK_SIZE-1)))
      {
        case 0: // Restored from eeprom
          break;
        case 1: // Read error
          return (1);
        case 2: // Checksum error
          if (Check_RAM_Alg() || RTC_voltage_low) // ram niet meer betrouwbaar
          {
            Data_Default();
//            RTC_voltage_low = 0;
            return (2);
          }
          break;
      }
    }
    if (default_area_size > opt_alg.area_size) // nieuwe opties toegevoegd
    {
      AddNewOptAlg();
      vlag_options_added = 1;
    }
    else
      vlag_options_added = 0;
  }

  return (0);
}

unsigned char VersieAfhankelijkOptAlg(void)
// mogelijkheid om option te controleren en of aan te passen bij een andere versie
// hier ook controleren wat te doen als aantal options minder is geworden (vorige versie terug geplaatst)
// return 0 data area goed ingelezen
// return 1 default data terug gezet beeindig RestoreDataArea omdat alles op default is gezet
{
//unsigned char *ptr;
//unsigned int loop;
unsigned char help_options_added = vlag_options_added; // nodig voor voorlopige versies

  if (vlag_options_added == 1)  
  {
    IncrementOptionsChangeCount();
    pc_0_read_configuration_alg = 1;  
    pc_1_read_configuration_alg = 1;
    pc_2_read_configuration_alg = 1;
    rs232_0_read_configuration_alg = 1;
    rs232_1_read_configuration_alg = 1;
    #ifdef ETHERNET
    ethernet_read_configuration_alg[0] = 1;
    ethernet_read_configuration_alg[1] = 1;
    ethernet_read_configuration_alg[2] = 1;
    ethernet_read_configuration_alg[3] = 1;
    #endif // ETHERNET
    vlag_options_added = 0;
  }
  if ((opt_alg.computer != default_opt_alg.computer) ||
      (opt_alg.soort != default_opt_alg.soort))
  {
    Data_Default();
    opt_alg.type = module.type;
    opt_alg.firma = module.firma;
    opt_alg.serie_number = module.serie_number;
    return (1);
  }
  else 
  {
    // Bijwerken opties indien nodig
    if ((opt_alg.versie_programma < 400) &&// voorlopige versie default opties zodra optie gebied groter wordt
        (opt_alg.versie_programma != default_opt_alg.versie_programma)) 
    {
      if (help_options_added ||
          (opt_alg.area_size != (unsigned long)&default_opt_alg.end - (unsigned long)&default_opt_alg))
      {
        Data_Default();
        opt_alg.type = module.type;
        opt_alg.firma = module.firma;
        opt_alg.serie_number = module.serie_number;
        return (1);
      }
      opt_alg.versie_programma = default_opt_alg.versie_programma;
      opt_alg.type = module.type;
      opt_alg.firma = module.firma;
      opt_alg.serie_number = module.serie_number;
      return (0);
    }
    if (opt_alg.versie_programma < default_opt_alg.versie_programma)
    {      
      opt_alg.area_size = (unsigned long)&default_opt_alg.end - (unsigned long)&default_opt_alg;
      opt_alg.versie_programma = default_opt_alg.versie_programma;
      IncrementOptionsChangeCount();
      pc_0_read_configuration_alg = 1;  
      pc_1_read_configuration_alg = 1;
      pc_2_read_configuration_alg = 1;
      rs232_0_read_configuration_alg = 1;
      rs232_1_read_configuration_alg = 1;
      #ifdef ETHERNET
      ethernet_read_configuration_alg[0] = 1;
      ethernet_read_configuration_alg[1] = 1;
      ethernet_read_configuration_alg[2] = 1;
      ethernet_read_configuration_alg[3] = 1;
      #endif // ETHERNET
    }
    else if (opt_alg.versie_programma > default_opt_alg.versie_programma)
    {
      opt_alg.area_size = (unsigned long)&default_opt_alg.end - (unsigned long)&default_opt_alg;
      opt_alg.versie_programma = default_opt_alg.versie_programma;
      IncrementOptionsChangeCount();
      pc_0_read_configuration_alg = 1;  
      pc_1_read_configuration_alg = 1;
      pc_2_read_configuration_alg = 1;
      rs232_0_read_configuration_alg = 1;
      rs232_1_read_configuration_alg = 1;
      #ifdef ETHERNET
      ethernet_read_configuration_alg[0] = 1;
      ethernet_read_configuration_alg[1] = 1;
      ethernet_read_configuration_alg[2] = 1;
      ethernet_read_configuration_alg[3] = 1;
      #endif // ETHERNET
    }
    if (opt_alg.type != module.type)
    {
      opt_alg.type = module.type;
      IncrementOptionsChangeCount();
      pc_0_read_configuration_alg = 1;  
      pc_1_read_configuration_alg = 1;
      pc_2_read_configuration_alg = 1;
      rs232_0_read_configuration_alg = 1;
      rs232_1_read_configuration_alg = 1;
      #ifdef ETHERNET
      ethernet_read_configuration_alg[0] = 1;
      ethernet_read_configuration_alg[1] = 1;
      ethernet_read_configuration_alg[2] = 1;
      ethernet_read_configuration_alg[3] = 1;
      #endif // ETHERNET
    }
    if (opt_alg.firma != module.firma)
    {
      opt_alg.firma = module.firma;
      IncrementOptionsChangeCount();
      pc_0_read_configuration_alg = 1;  
      pc_1_read_configuration_alg = 1;
      pc_2_read_configuration_alg = 1;
      rs232_0_read_configuration_alg = 1;
      rs232_1_read_configuration_alg = 1;
      #ifdef ETHERNET
      ethernet_read_configuration_alg[0] = 1;
      ethernet_read_configuration_alg[1] = 1;
      ethernet_read_configuration_alg[2] = 1;
      ethernet_read_configuration_alg[3] = 1;
      #endif // ETHERNET
    }
    if (opt_alg.serie_number != module.serie_number)
    {
      opt_alg.serie_number = module.serie_number;
      IncrementOptionsChangeCount();
      pc_0_read_configuration_alg = 1;  
      pc_1_read_configuration_alg = 1;
      pc_2_read_configuration_alg = 1;
      rs232_0_read_configuration_alg = 1;
      rs232_1_read_configuration_alg = 1;
      #ifdef ETHERNET
      ethernet_read_configuration_alg[0] = 1;
      ethernet_read_configuration_alg[1] = 1;
      ethernet_read_configuration_alg[2] = 1;
      ethernet_read_configuration_alg[3] = 1;
      #endif // ETHERNET
    }
    if (opt_alg.versie_PC != default_opt_alg.versie_PC)
      opt_alg.versie_PC = default_opt_alg.versie_PC;
  }
  return(0);
}
//-----------------------------------------------------------------------------
unsigned char RestoreOptAreaApp(void)
// return 0 data area goed ingelezen
// return 1 read error eeprom
// return 2 default data terug gezet beeindig RestoreDataArea omdat opties en setpoints op default zijn gezet
{
unsigned int default_area_size = (unsigned long)&default_opt_app.end - (unsigned long)&default_opt_app;

  if(dont_restore_opt_setp & 0x01)
  {
    Data_Default_Opt_Alg();
    return(0);
  }
  switch (RestoreData(OFFSET_OPT_APP, (EEP_BLOCK_SIZE-1)))
  {
    case 0: // Restored from eeprom
      break;
    case 1: // Read error
      return (1);
    case 2: // Checksum error
      if (Check_RAM_Alg() || RTC_voltage_low) // ram niet meer betrouwbaar
      {
        Data_Default();
//        RTC_voltage_low = 0;
        return (2);
      }
      break;
  }
  if (opt_app.area_size > ((OFFSET_SETP_ALG - OFFSET_OPT_APP) * (unsigned int)(EEP_BLOCK_SIZE-1))) // optie gebied is groter dan maximum geheugen ruimte 
  {
    // opties beschadigd
    Data_Default();
    return (2);
  }
  else
  {
    if (opt_app.area_size > (EEP_BLOCK_SIZE-1))
    {
      switch (RestoreData(OFFSET_OPT_APP+1, opt_app.area_size - (EEP_BLOCK_SIZE-1)))
      {
        case 0: // Restored from eeprom
          break;
        case 1: // Read error
          return (1);
        case 2: // Checksum error
          if (Check_RAM_Alg() || RTC_voltage_low) // ram niet meer betrouwbaar
          {
            Data_Default();
//            RTC_voltage_low = 0;
            return (2);
          }
          break;
      }
    }
    if (default_area_size > opt_app.area_size) // nieuwe opties toegevoegd
    {
      AddNewOptApp();
      vlag_options_added = 1;
    }
    else
      vlag_options_added = 0;
  }

  return (0);
}

unsigned char VersieAfhankelijkOptApp(void)
// mogelijkheid om option te controleren en of aan te passen bij een andere versie
// hier ook controleren wat te doen als aantal options minder is geworden (vorige versie terug geplaatst)
// return 0 data area goed ingelezen
// return 1 default data terug gezet beeindig RestoreDataArea omdat alles op default is gezet
{
//unsigned char *ptr;
//unsigned int loop;
unsigned char help_options_added = vlag_options_added; // nodig voor voorlopige versies

  if (vlag_options_added == 1)  
  {
    IncrementOptionsChangeCount();
    pc_0_read_configuration_alg = 1;  
    pc_1_read_configuration_alg = 1;
    pc_2_read_configuration_alg = 1;
    rs232_0_read_configuration_alg = 1;
    rs232_1_read_configuration_alg = 1;
    #ifdef ETHERNET
    ethernet_read_configuration_alg[0] = 1;
    ethernet_read_configuration_alg[1] = 1;
    ethernet_read_configuration_alg[2] = 1;
    ethernet_read_configuration_alg[3] = 1;
    #endif // ETHERNET
    vlag_options_added = 0;
  }
  if ((opt_app.computer != default_opt_app.computer) ||
      (opt_app.soort != default_opt_app.soort))
  {
    Data_Default();
    opt_app.type = module.type;
    opt_app.firma = module.firma;
    opt_app.serie_number = module.serie_number;
    return (1);
  }
  else 
  {
    // Bijwerken opties indien nodig
    if ((opt_app.versie_programma < 400) &&// voorlopige versie default opties zodra optie gebied groter wordt
        (opt_app.versie_programma != default_opt_app.versie_programma)) 
    {
      if (help_options_added ||
          (opt_app.area_size != (unsigned long)&default_opt_app.end - (unsigned long)&default_opt_app))
      {
        Data_Default();
	    Alarm_Reset_Data_Algemeen();		// JL
        opt_app.type = module.type;
        opt_app.firma = module.firma;
        opt_app.serie_number = module.serie_number;
        return (1);
      }
      opt_app.versie_programma = default_opt_app.versie_programma;
      opt_app.type = module.type;
      opt_app.firma = module.firma;
      opt_app.serie_number = module.serie_number;
      return (0);
    }
    if (opt_app.versie_programma < default_opt_app.versie_programma)
    {
      if (opt_app.versie_programma < 402)
	  {
	    Alarm_Reset_Data_Algemeen();
	  }
      if (opt_app.versie_programma < 410)
      {
        int i;

        for (i = 0; i < MAX_GROUP; i++)
          opt_app.Motorgroup[i].ClosedLoopOpenLoop = 0;
		opt_app.LuchtmengkastInblaasventClosedLoopOpenLoop = 0;
		opt_app.LuchtmengkastAfblaasventClosedLoopOpenLoop = 0;
      }
      if (opt_app.versie_programma < 415)
      {
        int i;

        for (i = 0; i < MAX_GROUP; i++)
		{
           opt_app.Motorgroup[i].PowerFactor = 100;
           opt_app.Motorgroup[i].RampUp      = 120;
           opt_app.Motorgroup[i].RampDown    = 120;
	    }
		opt_app.LuchtmengkastInblaasventPowerFactor = 100;
		opt_app.LuchtmengkastInblaasventRampUp      = 120;
		opt_app.LuchtmengkastInblaasventRampDown    = 120;
		opt_app.LuchtmengkastAfblaasventPowerFactor = 100;
		opt_app.LuchtmengkastAfblaasventRampUp      = 120;
		opt_app.LuchtmengkastAfblaasventRampDown    = 120;
      }
      if (opt_app.versie_programma < 423)
	  {
        int i;
        for (i = 0; i < MAX_GROUP; i++)
          opt_app.Motorgroup[i].VentAtMax = default_opt_app.Motorgroup[i].VentAtMax;
	  }

      opt_app.area_size = (unsigned long)&default_opt_app.end - (unsigned long)&default_opt_app;
      opt_app.versie_programma = default_opt_app.versie_programma;
      IncrementOptionsChangeCount();
      pc_0_read_configuration_alg = 1;  
      pc_1_read_configuration_alg = 1;
      pc_2_read_configuration_alg = 1;
      rs232_0_read_configuration_alg = 1;
      rs232_1_read_configuration_alg = 1;
      #ifdef ETHERNET
      ethernet_read_configuration_alg[0] = 1;
      ethernet_read_configuration_alg[1] = 1;
      ethernet_read_configuration_alg[2] = 1;
      ethernet_read_configuration_alg[3] = 1;
      #endif // ETHERNET
    }
    else if (opt_app.versie_programma > default_opt_app.versie_programma)
    {
      opt_app.area_size = (unsigned long)&default_opt_app.end - (unsigned long)&default_opt_app;
      opt_app.versie_programma = default_opt_app.versie_programma;
      IncrementOptionsChangeCount();
      pc_0_read_configuration_alg = 1;  
      pc_1_read_configuration_alg = 1;
      pc_2_read_configuration_alg = 1;
      rs232_0_read_configuration_alg = 1;
      rs232_1_read_configuration_alg = 1;
      #ifdef ETHERNET
      ethernet_read_configuration_alg[0] = 1;
      ethernet_read_configuration_alg[1] = 1;
      ethernet_read_configuration_alg[2] = 1;
      ethernet_read_configuration_alg[3] = 1;
      #endif // ETHERNET
    }
    if (opt_app.type != module.type)
    {
      opt_app.type = module.type;
      IncrementOptionsChangeCount();
      pc_0_read_configuration_alg = 1;  
      pc_1_read_configuration_alg = 1;
      pc_2_read_configuration_alg = 1;
      rs232_0_read_configuration_alg = 1;
      rs232_1_read_configuration_alg = 1;
      #ifdef ETHERNET
      ethernet_read_configuration_alg[0] = 1;
      ethernet_read_configuration_alg[1] = 1;
      ethernet_read_configuration_alg[2] = 1;
      ethernet_read_configuration_alg[3] = 1;
      #endif // ETHERNET
    }
    if (opt_app.firma != module.firma)
    {
      opt_app.firma = module.firma;
      IncrementOptionsChangeCount();
      pc_0_read_configuration_alg = 1;  
      pc_1_read_configuration_alg = 1;
      pc_2_read_configuration_alg = 1;
      rs232_0_read_configuration_alg = 1;
      rs232_1_read_configuration_alg = 1;
      #ifdef ETHERNET
      ethernet_read_configuration_alg[0] = 1;
      ethernet_read_configuration_alg[1] = 1;
      ethernet_read_configuration_alg[2] = 1;
      ethernet_read_configuration_alg[3] = 1;
      #endif // ETHERNET
    }
    if (opt_app.serie_number != module.serie_number)
    {
      opt_app.serie_number = module.serie_number;
      IncrementOptionsChangeCount();
      pc_0_read_configuration_alg = 1;  
      pc_1_read_configuration_alg = 1;
      pc_2_read_configuration_alg = 1;
      rs232_0_read_configuration_alg = 1;
      rs232_1_read_configuration_alg = 1;
      #ifdef ETHERNET
      ethernet_read_configuration_alg[0] = 1;
      ethernet_read_configuration_alg[1] = 1;
      ethernet_read_configuration_alg[2] = 1;
      ethernet_read_configuration_alg[3] = 1;
      #endif // ETHERNET
    }
    if (opt_app.versie_PC != default_opt_app.versie_PC)
      opt_app.versie_PC = default_opt_app.versie_PC;
  }
  return(0);
}
//-----------------------------------------------------------------------------
unsigned char RestoreSetpAreaAlg(void)
// return 0 data area goed ingelezen
// return 1 read error eeprom
{
unsigned int default_area_size = (unsigned long)&default_setp_alg.end - (unsigned long)&default_setp_alg;

  if(dont_restore_opt_setp & 0x02)
  {
    Data_Default_Setp_Alg();
    return(0);
  }
  switch (RestoreData(OFFSET_SETP_ALG, (EEP_BLOCK_SIZE-1)))
  {
    case 0: // Restored from eeprom
      break;
    case 1: // Read error
      return (1);
    case 2: // Checksum error
      if (Check_RAM_Alg() || RTC_voltage_low) // ram niet meer betrouwbaar
      {
        Data_Default_Setp_Alg();
        Data_Default_Val_Hr_Alg();
        return (2);
      }
      break;
  }
  if (setp_alg.area_size > ((EEP_BLOCK_AANT - OFFSET_SETP_ALG) * (EEP_BLOCK_SIZE-1))) // optie gebied is groter dan maximum geheugen ruimte 
  {
    // opties beschadigd
    Data_Default_Setp_Alg();
    return (0);
  }
  else
  {
    if (setp_alg.area_size > (EEP_BLOCK_SIZE-1))
    {
      switch (RestoreData(OFFSET_SETP_ALG+1, setp_alg.area_size - (EEP_BLOCK_SIZE-1)))
      {
        case 0: // Restored from eeprom
          break;
        case 1: // Read error
          return (1);
        case 2: // Checksum error
         if (Check_RAM_Alg() || RTC_voltage_low) // ram niet meer betrouwbaar
          { 
            Data_Default_Setp_Alg();
            Data_Default_Val_Hr_Alg();
            return (2);
          }
          break;
      }
    }
    if (default_area_size > setp_alg.area_size) // nieuwe setpoints worden toegevoegd
    {
      AddNewSetpAlg();
      vlag_setpoints_added = 1;
    }
    else
      vlag_setpoints_added = 0;
  }
  return (0);
}

unsigned char VersieAfhankelijkSetpAlg(void)
// hier ook controleren wat te doen als aantal setpoints minder is geworden (vorige versie terug geplaatst)
// return 0 data area goed ingelezen
// return 1 niet gebruikt
{
//unsigned char *ptr;
//unsigned int loop;

  if ((setp_alg.computer != default_setp_alg.computer) ||
      (setp_alg.soort != default_setp_alg.soort))
  {
    Data_Default();
    setp_alg.type = module.type;
    setp_alg.firma = module.firma;
    setp_alg.serie_number = module.serie_number;
    return (1);
  }
  else 
  {
    // Bijwerken setpoints indien nodig
    if ((setp_alg.versie_programma < 400) && // voorlopige versie default opties zodra optie gebied groter wordt
        (setp_alg.versie_programma != default_setp_alg.versie_programma))
    {
      if (vlag_setpoints_added ||
          (setp_alg.area_size != (unsigned long)&default_setp_alg.end - (unsigned long)&default_setp_alg))
        Data_Default_Setp_Alg();
      setp_alg.versie_programma = default_setp_alg.versie_programma;
      setp_alg.type = module.type;
      setp_alg.firma = module.firma;
      setp_alg.serie_number = module.serie_number;
      return (0);
    }
    if (setp_alg.versie_programma < default_setp_alg.versie_programma)
    {
      setp_alg.area_size = (unsigned long)&default_setp_alg.end - (unsigned long)&default_setp_alg;
      setp_alg.versie_programma = default_setp_alg.versie_programma;
    }
    else if (setp_alg.versie_programma > default_setp_alg.versie_programma)
    {
      setp_alg.area_size = (unsigned long)&default_setp_alg.end - (unsigned long)&default_setp_alg;
      setp_alg.versie_programma = default_setp_alg.versie_programma;
    }
    if (setp_alg.type != module.type)
      setp_alg.type = module.type;
    if (setp_alg.firma != module.firma)
      setp_alg.firma = module.firma;
    if (setp_alg.serie_number != module.serie_number)
      setp_alg.serie_number = module.serie_number;
    if (setp_alg.versie_PC != default_setp_alg.versie_PC)
      setp_alg.versie_PC = default_setp_alg.versie_PC;
  }
  return(0);
}

//*****************************************************************************
unsigned char CheckIfUpdateValid(void)
{
  if (val_hr_alg.control_0 == 0x5A5A) // vanaf versie 4.00 is dit 0xA5A5
    return (0);
  else
    return (1);
}

//*****************************************************************************
bit RestoreDataAreas(void)
{
static unsigned int error_cnt = 0;
//unsigned int default_area_size;

  if (error_cnt > 10)
    while (1);     // Reset controller
  error_cnt++;

  if(RTC_voltage_low || (opt_alg.versie_programma != VERSIE_PROG))
    dont_restore_opt_setp = 0;

  WDI_Trigger();   
  switch(Check_EEP_Size())
  {         
    case 0: return(1); // test mislukt
    case 1: return(0); // EEP_size alarm
    case 2: break;     // EEP_size ok
  }		 	  
  WDI_Trigger();   
  if (RestoreModuleArea())
    return (1);
  WDI_Trigger();   
  if (VersieAfhankelijkModule())
    return (0);

  // check option
  WDI_Trigger();   
  switch (RestoreOptAreaIO())
  {
    case 0: // option area is ingelezen
      WDI_Trigger();
      // controleer versie afhankelijkheid
      if (VersieAfhankelijkOptIO())
      {
        // versie was niet correct opties, setpoints en value_hr teruggezet (ook van stallen)
        return (0); // break;
      }
      else
      {
        WDI_Trigger();
        switch (RestoreOptAreaAlg())
        {
          case 0: // option area is ingelezen
            WDI_Trigger();
            // controleer versie afhakelijkheid
            if (VersieAfhankelijkOptAlg())
            {
              // versie was niet correct opties, setpoints en value_hr teruggezet (ook van stallen)
              return (0); // break;
            }
            else
            {
              WDI_Trigger();
              switch (RestoreOptAreaApp())
              {
                case 0: // option area is ingelezen
                  WDI_Trigger();
                  // controleer versie afhakelijkheid
                  if (VersieAfhankelijkOptApp())
                  {
                    // versie was niet correct opties, setpoints en value_hr teruggezet (ook van stallen)
                    return (0); // break;
                  }
                  else
                  {
                    WDI_Trigger();
                    switch (RestoreSetpAreaAlg())
                    {
                      case 0: 
                        WDI_Trigger();
                        // controleer versie afhakelijkheid
                        if (VersieAfhankelijkSetpAlg())
                        {
                          // versie was niet correct opties, setpoints en value_hr teruggezet (ook van stallen)
                          return (0);
                        }
                        WDI_Trigger();
                        if (Check_RAM_Alg() || RTC_voltage_low)
                          Data_Default_Val_Hr_Alg(); // vul value_hr met default waarden
                        else if (sizeof(default_val_hr_alg) < val_hr_alg.area_size)  
                          val_hr_alg.area_size = sizeof(default_val_hr_alg);
                        else if (sizeof(default_val_hr_alg) > val_hr_alg.area_size)  
                          AddNewValueHrAlg();
                        break;
                      case 1: // lees error eeprom
                        return (1); // probeer opnieuw nadat I2C opnieuw geinitialiseerd
                      case 2: // checksum error eeprom
                        // versie was niet correct setpoints teruggezet
                        break;
                    }
                  }
                  break;
                case 1: // read error eeprom
                  return (1); // probeer opnieuw nadat I2C opnieuw geinitialiseerd
                case 2: // checksum error
                  // checksum was niet correct opties, setpoints en value_hr teruggezet (ook van stallen)
                  return (0); // break;
              }
            }
            break;
          case 1: // read error eeprom
            return (1); // probeer opnieuw nadat I2C opnieuw geinitialiseerd
          case 2: // checksum error
            // checksum was niet correct opties, setpoints en value_hr teruggezet (ook van stallen)
            return (0); // break;
        }
      }
      break;
    case 1: // read error eeprom
      return (1); // probeer opnieuw nadat I2C opnieuw geinitialiseerd
    case 2: // checksum error
      // checksum was niet correct opties, setpoints en value_hr teruggezet (ook van stallen)
      return (0); // break;
  }
  dont_restore_opt_setp = 0;
  RTC_voltage_low = 0;
  return (0);
}

//-----------------------------------------------------------------------------
unsigned char comp_write_ram_eep_block(unsigned int nr)
{
unsigned char temp_eep_block[EEP_BLOCK_SIZE];
unsigned char checksum;
unsigned char *adr = ((unsigned char *)&module)+nr*(EEP_BLOCK_SIZE-1);

  if (EEP_Read_Block(nr,temp_eep_block))
    return (0);
  if (temp_eep_block[EEP_BLOCK_SIZE-1] == ChecksumBlock(temp_eep_block, EEP_BLOCK_SIZE))
  {
    if (!Data_Block_Ram_Equal_EEP(adr, temp_eep_block))
    {
      checksum = ChecksumBlock(adr, EEP_BLOCK_SIZE);
      if (EEP_Write_Block(nr,adr,checksum))
        return (0);
    }
  }
  else
  {
    // waarschuwing eeprom was beschadigd
    checksum = ChecksumBlock(adr, EEP_BLOCK_SIZE);
    if (EEP_Write_Block(nr,adr,checksum))
      return (0);
  }
  return (1);
}

void comp_ram_eep(void)
{
unsigned int opt_io_size = (unsigned long)&default_opt_io.end - (unsigned long)&default_opt_io;
unsigned int opt_alg_size = (unsigned long)&default_opt_alg.end - (unsigned long)&default_opt_alg;
unsigned int opt_app_size = (unsigned long)&default_opt_app.end - (unsigned long)&default_opt_app;
unsigned int setp_alg_size = (unsigned long)&default_setp_alg.end - (unsigned long)&default_setp_alg;
//static unsigned int block_nr = OFFSET_OPT_IO;
static unsigned int block_nr = OFFSET_OPT_IO; // JP 01-10-12

  if (vlag_module_write)
  {
    block_nr = OFFSET_MODULES;
    vlag_module_write = 0;
  }
  if (block_nr < OFFSET_OPT_IO)
  {
    if (comp_write_ram_eep_block(block_nr))
    {
      block_nr++;
    }
  }
  else if (block_nr < OFFSET_OPT_IO + Number_Of_Blocks(opt_io_size, EEP_BLOCK_SIZE))
  {
    if (comp_write_ram_eep_block(block_nr))
    {
      block_nr++;
      if (block_nr >= OFFSET_OPT_IO + Number_Of_Blocks(opt_io_size, EEP_BLOCK_SIZE))
        block_nr = OFFSET_OPT_ALG;
    }
  }
  else if (block_nr < OFFSET_OPT_ALG + Number_Of_Blocks(opt_alg_size, EEP_BLOCK_SIZE))
  {
    if (comp_write_ram_eep_block(block_nr))
    {
      block_nr++;
      if (block_nr >= OFFSET_OPT_ALG + Number_Of_Blocks(opt_alg_size, EEP_BLOCK_SIZE))
        block_nr = OFFSET_OPT_APP;
    }
  }
  else if (block_nr < OFFSET_OPT_APP + Number_Of_Blocks(opt_app_size, EEP_BLOCK_SIZE))
  {
    if (comp_write_ram_eep_block(block_nr))
    {
      block_nr++;
      if (block_nr >= OFFSET_OPT_APP + Number_Of_Blocks(opt_app_size, EEP_BLOCK_SIZE))
        block_nr = OFFSET_SETP_ALG;
    }
  }
  else if (block_nr < OFFSET_SETP_ALG + Number_Of_Blocks(setp_alg_size, EEP_BLOCK_SIZE))
  {
    if (comp_write_ram_eep_block(block_nr))
    {
      block_nr++;
      if (block_nr >= OFFSET_SETP_ALG + Number_Of_Blocks(setp_alg_size, EEP_BLOCK_SIZE))
      {
//        EEP_Write_Disable();
          block_nr = OFFSET_OPT_IO;
          if (comp_ram_eep_switch == 1)
            comp_ram_eep_switch = 0;
          else
            comp_ram_eep_switch--;
      }
    }
  }
  else
  {
//    EEP_Write_Disable();
    block_nr = OFFSET_OPT_IO;
    if (comp_ram_eep_switch == 1)
      comp_ram_eep_switch = 0;
    else
      comp_ram_eep_switch--;
  }
}
