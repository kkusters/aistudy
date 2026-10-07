// CH_DATA.H

#ifndef _CH_DATA_H
#define _CH_DATA_H

#include "ct_computer.h"             
#include "ct_data.h"

extern s_modules module;
extern s_opt_alg opt_alg;
extern s_opt_alg const default_opt_alg;
extern s_opt_app opt_app;
extern s_opt_app const default_opt_app;
extern s_opt_io opt_io;
extern s_opt_io const default_opt_io;
extern s_setp_alg setp_alg;
extern s_setp_alg const default_setp_alg;
extern s_value value;
extern s_val_hr_alg val_hr_alg;
extern s_val_hr_alg const default_val_hr_alg;
extern s_tekst tekst;
extern s_tekst_inst tekst_inst;
extern s_alarm_hr_alg alarm_hr_alg;
extern s_rom const rom;
extern unsigned char RTC_voltage_low;
extern unsigned char dont_restore_opt_setp;
extern unsigned char comp_ram_eep_switch;
//extern int cnt_write_time_eep;
//extern unsigned char pwr_down_while_write_eep_flag;
extern unsigned char vlag_module_write;
extern unsigned char const temp_unit;

//==============================================================================
unsigned int Number_Of_Blocks(unsigned int aantal_bytes, unsigned int block_size);
void Data_Default_Opt(void);
unsigned char VersieAfhankelijkOptAlg(void);
unsigned char VersieAfhankelijkOptIO(void);
unsigned char VersieAfhankelijkOptApp(void);
void Data_Default_Setp_Alg(void);
void Data_Default_Setp(void);
unsigned char VersieAfhankelijkSetpAlg(void);
void Data_Default_Val_Hr_Alg(void);
void Data_Default_Val_Hr(void);
void Data_Default(void);
unsigned char CheckIfUpdateValid(void);
bit RestoreDataAreas(void);
unsigned char ChecksumBlock(unsigned char *data, unsigned int block_size);
void comp_ram_eep(void);
unsigned char CheckWriteEEP(void);
void BlockCopy(unsigned char *ptr_source,unsigned char *ptr_destination, unsigned int aantal);
void AddNewOptAlg(void);
void AddNewOptIO(void);
void AddNewOptApp(void);
void AddNewSetpAlg(void);
void IncrementOptionsChangeCount(void);

#endif
