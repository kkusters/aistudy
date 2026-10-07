// CH_EEP_TAAL.H

#ifndef _CH_EEP_TAAL_H
#define _CH_EEP_TAAL_H

#include "ch_string.h"

// 24C128 16k block write 64
// 24c64 8k block write 32
#define EEP_TAAL_BLOCK_SIZE 64
// size eeprom is 16k 
#define EEP_TAAL_SIZE 0x8000
// number of blocks in EEPROM
#define EEP_TAAL_BLOCK_AANT (EEP_TAAL_SIZE/EEP_TAAL_BLOCK_SIZE)

// size data blok zonde checksum
#define BLOCK_SIZE (EEP_TAAL_BLOCK_SIZE-1)

typedef struct
{
  int inc_dec; // 1 blokken opwaarts lezen (teksten), -1 blokken neerwaart lezen (inst teksten)
  int blok_nr; // te lezen blok nummer
  int blok_cnt; // aantal nog te lezen blokken
  unsigned int size_tekst;
  unsigned char data[BLOCK_SIZE*2];
  unsigned char blok_empty[2]; // (data bestaat uit 2 blokken ingelezen gegeven uit EEPROM) 0 = blok vol; 1 is blok gevuld
  int index; // index binnen blok dat gelezen wordt
  s_tekst_50 regel;
} s_tekst_read;

typedef struct
{
  unsigned int area_size;
  unsigned int computer;
  unsigned int soort;
  unsigned int versie_tekst;
  s_tekst_15 Gekozen_Taal_14;
  unsigned char data[38];
} s_tekst_exist;

extern s_tekst_exist eeprom_taal;
extern s_tekst_exist eeprom_taal_inst;
extern bit EEP_taal_init_switch;
extern bit EEP_taal_eprom_aanwezig;
extern bit EEP_taal_tekst_aanwezig;
extern bit EEP_taal_tekst_inst_aanwezig;
extern bit EEP_taal_tekst_write_switch;
extern bit EEP_taal_tekst_inst_write_switch;
extern bit EEP_taal_read_switch;

void EEP_Taal_Init(void);

unsigned char EEP_Tekst_Read_First_String(unsigned char tekst, s_tekst_read *tekst_read);
unsigned char EEP_Tekst_Read_Next_String(s_tekst_read *tekst_read);

void EEP_Taal_Proc(void);

#endif
