// C__EEP_TAAL.C

#include <string.h> 

#include "ch_define.h"
#include "ch_alarm.h"
#include "ch_alg.h"
#include "ch_asc1.h"
#include "ch_i2c.h"
#include "ch_eep_taal.h"

// 24C256 32k block write 64
// 24C128 16k block write 64
// 24c64 8k block write 32
//#define EEP_BLOCK_SIZE 64
//#define EEP_SIZE 0x8000

#define EEP_LAST_BLOCK 511

#define IIC_ADDRESS_EEP_LANGUAGE 0xA6
#define IIC_EEP_LANGUAGE_WC P3_5
#define IIC_EEP_LANGUAGE_WC_DP DP3_5
#define IIC_EEP_LANGUAGE_WC_ODP ODP3_5

bit EEP_taal_init_switch = 1;             // initialiseer taal eeprom
bit EEP_taal_read_switch = 0;             // zorgt er voor dat tekst en tekst_inst uit eeprom gelezen wordt
bit EEP_taal_eprom_aanwezig = 0;          // geeft aan dat eeprom aanwezig is
bit EEP_taal_bestaat_switch = 1;          // zorgt er voor dat gekeken wordt of tekst en tekst_inst in eeprom aanwezig
bit EEP_taal_tekst_aanwezig = 0;          // geeft aan dat tekst in eeprom aanwezig
bit EEP_taal_tekst_inst_aanwezig = 0;     // geeft aan dat tekst_inst in eeprom aanwezig
bit EEP_taal_tekst_write_switch = 0;      // zorgt ervoor dat tekst naar eeprom geschreven wordt
bit EEP_taal_tekst_inst_write_switch = 0; // zorgt ervoor dat tekst inst naar eeprom geschreven wordt
s_tekst_exist eeprom_taal;
s_tekst_exist eeprom_taal_inst;

_inline void EEP_Taal_Write_Enable(void)
{
  IIC_EEP_LANGUAGE_WC = 0;
}

_inline void EEP_Taal_Write_Disable(void)
{
  IIC_EEP_LANGUAGE_WC = 1;
}


bit EEP_Taal_Read_Block(unsigned int block_nr, unsigned char *data)
{
unsigned char temp_eep_block[EEP_TAAL_BLOCK_SIZE];
unsigned int address = block_nr*EEP_TAAL_BLOCK_SIZE;
int loop;

  if (IIC_Read(IIC_ADDRESS_EEP_LANGUAGE, address >> 8, address & 0x00FF, IIC_TWO_SUB_ADDRESSES, temp_eep_block, EEP_TAAL_BLOCK_SIZE))
  {
    return (1);
  } 
  if (temp_eep_block[EEP_TAAL_BLOCK_SIZE-1] != ChecksumBlock(temp_eep_block, EEP_TAAL_BLOCK_SIZE))
  {
    return (1);
  }
  else
  {
    for (loop = 0; loop < EEP_TAAL_BLOCK_SIZE-1; loop++)
    {
      data[loop] = temp_eep_block[loop];
    }
  }
  return (0);
}

static bit EEP_Taal_Write_Block(unsigned int block_nr, unsigned char *data)
{
unsigned char block[EEP_TAAL_BLOCK_SIZE];
unsigned int address = block_nr*EEP_TAAL_BLOCK_SIZE;

  EEP_Taal_Write_Enable();
  Copy_N_Bytes(block, data, EEP_TAAL_BLOCK_SIZE-1);
  block[EEP_TAAL_BLOCK_SIZE-1] = ChecksumBlock(block, EEP_TAAL_BLOCK_SIZE);
  if (IIC_Write(IIC_ADDRESS_EEP_LANGUAGE, address >> 8, address & 0x00FF, IIC_TWO_SUB_ADDRESSES, block, EEP_TAAL_BLOCK_SIZE))
  {
    EEP_Taal_Write_Disable();
    return (1);
  } 
  EEP_Taal_Write_Disable();
  return (0);
}

//*****************************************************************************

static void EEP_Taal_Bestaat(void)
{
static unsigned int error_cnt = 0;

  if ((EEP_Taal_Read_Block(0, (unsigned char *)&eeprom_taal)) ||
      (EEP_Taal_Read_Block(EEP_LAST_BLOCK, (unsigned char *)&eeprom_taal_inst)))
//    error_cnt++;
//  else if (EEP_Taal_Read_Block(EEP_LAST_BLOCK, (unsigned char *)&eeprom_taal_inst))
  {
    error_cnt++;
    if (error_cnt >= 10) // na tien pogingen pas weer opnieuw proberen na opnieuw opstarten
    { 
      error_cnt = 0;
      if (opt_alg.taalkeuze == USER)
      {
	    CreateAlarm(&alarm_hr_alg.EEP_taal_al, SYSTEEM_AL_EEP_TAAL, 0, 0, 0, ZACHT_ALARM);
        eeprom_taal.Gekozen_Taal_14 = tekst_no_language;
        tekst = tekst_engels;
        tekst_inst = tekst_inst_engels;
      }  
      EEP_taal_read_switch = 0;
      EEP_taal_tekst_aanwezig = 0;
      EEP_taal_tekst_inst_aanwezig = 0;
      EEP_taal_bestaat_switch = 0;
    }  
  }
  else
  {
    error_cnt = 0;  
    if ((eeprom_taal.computer == rom.computer) &&
        (eeprom_taal.soort == rom.soort) &&
        (eeprom_taal.versie_tekst == rom.versie_tekst))
      EEP_taal_tekst_aanwezig = 1; // taal aanwezig in EEPROM
    if ((eeprom_taal_inst.computer == rom.computer) &&
        (eeprom_taal_inst.soort == rom.soort) &&
        (eeprom_taal_inst.versie_tekst == rom.versie_tekst_inst))
      EEP_taal_tekst_inst_aanwezig = 1; // taal aanwezig in EEPROM
    if ((opt_alg.taalkeuze == USER) && (!EEP_taal_tekst_aanwezig || !EEP_taal_tekst_inst_aanwezig))
    {
      tekst = tekst_engels;
      tekst_inst = tekst_inst_engels;
    }  
    EEP_taal_bestaat_switch = 0;
  }    
}

static void EEP_Taal_Tekst_Write(void)
{
static unsigned int error_cnt = 0;
static unsigned char *ptr;
static unsigned int block_nr;
static unsigned int end_block;
static unsigned char fase = 0;

  switch (fase)
  {
    default:
      fase = 0;
    case 0:
      ptr = (unsigned char *)&tekst;
      block_nr = 0;
      end_block = block_nr + Number_Of_Blocks(sizeof(s_tekst), EEP_TAAL_BLOCK_SIZE);
      fase++;
      error_cnt = 0;
    case 1:   
      if (!EEP_Taal_Write_Block(block_nr, ptr))
      {
        error_cnt = 0;
        ptr += EEP_TAAL_BLOCK_SIZE-1;
        block_nr++;
        if (block_nr >= end_block)
        {
          // gereed met schrijven tekst naar taal eeprom
          if (!EEP_taal_tekst_inst_aanwezig)
            EEP_taal_tekst_inst_write_switch = 1;
          EEP_taal_tekst_aanwezig = 1;
          eeprom_taal.Gekozen_Taal_14 = tekst.Gekozen_Taal_14;
          EEP_taal_tekst_write_switch = 0;
          fase = 0;
        }  
      }  
      else
      {
        // error
        error_cnt++;
        if (error_cnt > 200) // een malig alarm na opstarten als usertaal ingesteld en geen EEPROM aanwezig
        {
          if (opt_alg.taalkeuze == USER)
          {
            CreateAlarm(&alarm_hr_alg.EEP_taal_al, SYSTEEM_AL_EEP_TAAL, 0, 0, 0, ZACHT_ALARM);
            eeprom_taal.Gekozen_Taal_14 = tekst_no_language;
            tekst = tekst_engels;
            tekst_inst = tekst_inst_engels;
          }  
          EEP_taal_tekst_write_switch = 0;
          fase = 0;
        }
      }  
      break;
  }  
}

static void EEP_Taal_Tekst_Inst_Write(void)
{
static unsigned int error_cnt = 0;
static unsigned char *ptr;
static unsigned int block_nr;
static unsigned int end_block;
static unsigned char fase = 0;

  switch (fase)
  {
    default:
      fase = 0;
    case 0:
      ptr = (unsigned char *)&tekst_inst;
      block_nr = EEP_LAST_BLOCK;
      end_block = block_nr - Number_Of_Blocks(sizeof(s_tekst_inst), EEP_TAAL_BLOCK_SIZE);
      fase++;
      error_cnt = 0;
    case 1:   
      if (!EEP_Taal_Write_Block(block_nr, ptr))
      {
        // schrijven blok in EEPROM gereed
        error_cnt = 0;
        ptr += EEP_TAAL_BLOCK_SIZE-1;
        block_nr--;
        if (block_nr <= end_block)
        {
          // gereed met schrijven tekst naar taal eeprom
          if (!EEP_taal_tekst_aanwezig)
            EEP_taal_tekst_write_switch = 1;
          EEP_taal_tekst_inst_aanwezig = 1;
          EEP_taal_tekst_inst_write_switch = 0;
          fase = 0;
        }  
      }
      else
      {
        // error
        error_cnt++;
        if (error_cnt > 200) // een malig alarm na opstarten als usertaal ingesteld en geen EEPROM aanwezig
        {
          if (opt_alg.taalkeuze == USER)
          {
            CreateAlarm(&alarm_hr_alg.EEP_taal_al, SYSTEEM_AL_EEP_TAAL, 0, 0, 0, ZACHT_ALARM);
            eeprom_taal.Gekozen_Taal_14 = tekst_no_language;
            tekst = tekst_engels;
            tekst_inst = tekst_inst_engels;
          }  
          EEP_taal_tekst_inst_write_switch = 0;
          fase = 0;
        }
      }  
      break;
  }  
}

//*****************************************************************************

static _near unsigned char EEP_Tekst_Read_String(s_tekst_read *tekst_read)
{
unsigned char loop;
unsigned char max_char;
unsigned char blok_actueel;
int index = tekst_read->index;

  blok_actueel = (index >= BLOCK_SIZE);

  tekst_read->regel.max_char = max_char = tekst_read->data[index];
  if ((max_char % 2) == 0) // uitlijnen op woord waarde
    max_char++;
  if (tekst_read->size_tekst < 3 + max_char)
  {
    CreateAlarm(&alarm_hr_alg.EEP_taal_al, SYSTEEM_AL_EEP_TAAL, 0, 0, 0, ZACHT_ALARM);
    eeprom_taal.Gekozen_Taal_14 = tekst_no_language;
    tekst = tekst_engels;
    tekst_inst = tekst_inst_engels;
    EEP_taal_read_switch = 0;
    return (1);                                                         
  }  
  else  
  {
    tekst_read->size_tekst -= 3 + max_char;
    index++;
    if (index >= 2*BLOCK_SIZE)
      index = 0;
    tekst_read->regel.max_bits = tekst_read->data[index];
    index++;
    if (index >= 2*BLOCK_SIZE)
      index = 0;
    tekst_read->regel.font_type = tekst_read->data[index];
    index++;
    if (index >= 2*BLOCK_SIZE)
      index = 0;
    for (loop = 0; loop < max_char; loop++)
    {
      tekst_read->regel.string[loop] = tekst_read->data[index];
      index++;
      if (index >= 2*BLOCK_SIZE)
        index = 0;
    }
    if (blok_actueel != (index >= BLOCK_SIZE))
      tekst_read->blok_empty[blok_actueel] = 1;
    tekst_read->index = index;
  }    
  return (0);
}

unsigned char EEP_Tekst_Read_First_String(unsigned char tekst, s_tekst_read *tekst_read)
// input
// tekst = 0 dan start teksten inlezen
// tekst = 1 dan start teksten installatie inlezen
// data pointer naar ingelezen regel met tekst
// return (0) alles OK
// return (1) I2C niet OK of eeprom niet OK
{
  if (tekst == 0) // start inlezen tekst
  {
   tekst_read->inc_dec = 1;
   tekst_read->blok_nr = 0;
  }
  else // start inlezen installatie tekst
  {
    tekst_read->inc_dec = -1;
    tekst_read->blok_nr = 511;
  }
  if (EEP_Taal_Read_Block(tekst_read->blok_nr,tekst_read->data))
    return (1);
  else
  {
    tekst_read->blok_empty[0] = 0;
    tekst_read->blok_empty[1] = 1;
    tekst_read->size_tekst = ((unsigned int)tekst_read->data[1] << 8) + tekst_read->data[0];
    tekst_read->blok_cnt = Number_Of_Blocks(tekst_read->size_tekst, EEP_TAAL_BLOCK_SIZE) - 1; // aantal blokken min 1 (reeds ingelezen)
    tekst_read->size_tekst -= 8;
    tekst_read->blok_nr += tekst_read->inc_dec; // volgende in te lezen blok
    tekst_read->index = 8; // zet index op eerste tekst string
  }    
  return (EEP_Tekst_Read_String(tekst_read));
}

unsigned char EEP_Tekst_Read_Next_String(s_tekst_read *tekst_read)
// return (0) alles OK
// return (1) I2C niet OK of eeprom niet OK
{
unsigned char loop;

  if (tekst_read->blok_cnt)
  {
    for (loop = 0; loop < 2; loop++)
    {
      if (tekst_read->blok_empty[loop] == 1)
      {
        if (EEP_Taal_Read_Block(tekst_read->blok_nr,&tekst_read->data[loop*(EEP_TAAL_BLOCK_SIZE-1)]))
          return (1);
        else  
        {
          tekst_read->blok_empty[loop] = 0;
          tekst_read->blok_nr += tekst_read->inc_dec;
          tekst_read->blok_cnt--;
        }    
      }    
    }
  }
  return (EEP_Tekst_Read_String(tekst_read));
}

//*****************************************************************************

static unsigned char *EEP_Taal_Copier_Regel(unsigned char *out, unsigned char *in)
// out pointer waar data naar toe geschreven moet worden
// in pointer naar data die gelezen moet worden
// return pointer van geschreven data verzet naar volgende te schrijven regel
{
unsigned char loop;
unsigned char max_char_out;
unsigned char max_char_in;

  max_char_out = *out; out++;
  max_char_in = *in; in++;
  out++; // sla max_bits over
  in++; // sla max_bits over
  *out = *in; *out++; *in++;// copieer font_type
  for (loop = 0; loop < max_char_out - 1; loop++) // vul string met data en vul rest aan met 0
  {
    if (loop < max_char_in)
    {
      *out = *in; in++;
    }  
    else
      *out = 0;
    out++;  
  }
  *out = 0; out++; // zorg er voor dat laatste karakter altijd 0
  if ((max_char_out % 2) == 0) // uitlijne op woord niveau
  {
    *out = 0; out++; // vul uitlijn karakter met  0
  }
  return (out); // return adres voor volgende regel
}

static void EEP_Taal_Tekst_En_Tekst_Inst_Read(void)
{
static unsigned int error_cnt = 0;
static s_tekst_read tekst_read;
static unsigned char fase = 0;
static unsigned char *ptr;

  switch (fase)
  {
    default:
      error_cnt = 0;
      fase = 0;
    case 0: // start inlezen tekst uit taal eeprom
      if (EEP_Tekst_Read_First_String(0,&tekst_read))
      {
        // error
        error_cnt++;
        if (error_cnt > 10)
          fase = 4;
      }
      else  
      {
        error_cnt = 0;
        if (strncmp(tekst_nederlands.Gekozen_Taal_14.string, eeprom_taal.Gekozen_Taal_14.string, strlen(tekst_nederlands.Gekozen_Taal_14.string)) == 0)
          tekst = tekst_nederlands;
        else if (strncmp(tekst_duits.Gekozen_Taal_14.string, eeprom_taal.Gekozen_Taal_14.string, strlen(tekst_duits.Gekozen_Taal_14.string)) == 0)
          tekst = tekst_duits;
        else if (strncmp(tekst_spaans.Gekozen_Taal_14.string, eeprom_taal.Gekozen_Taal_14.string, strlen(tekst_spaans.Gekozen_Taal_14.string)) == 0)
          tekst = tekst_spaans;
        ptr = (unsigned char *)&tekst.Gekozen_Taal_14;
        ptr = EEP_Taal_Copier_Regel(ptr,(unsigned char *)&tekst_read.regel);
        if ((tekst_read.size_tekst == 0) ||
            (ptr >= (unsigned char *)&tekst + sizeof(s_tekst)))
          fase = 2; // tekst ingelezen ga naar inlezen tekst_inst    
        else
          fase = 1; // ga verder met inlezen tekst
      }
      break;
    case 1:   
      if (EEP_Tekst_Read_Next_String(&tekst_read))
      {
        // error
        error_cnt++;
        if (error_cnt > 10)
          fase = 4;
      }
      else
      {
        error_cnt = 0;
        if (strncmp(tekst_inst_nederlands.Gekozen_Taal_14.string, eeprom_taal_inst.Gekozen_Taal_14.string, strlen(tekst_inst_nederlands.Gekozen_Taal_14.string)) == 0)
          tekst_inst = tekst_inst_nederlands;
        else if (strncmp(tekst_inst_duits.Gekozen_Taal_14.string, eeprom_taal_inst.Gekozen_Taal_14.string, strlen(tekst_inst_duits.Gekozen_Taal_14.string)) == 0)
          tekst_inst = tekst_inst_duits;
        else if (strncmp(tekst_inst_spaans.Gekozen_Taal_14.string, eeprom_taal_inst.Gekozen_Taal_14.string, strlen(tekst_inst_spaans.Gekozen_Taal_14.string)) == 0)
          tekst_inst = tekst_inst_spaans;
        ptr = EEP_Taal_Copier_Regel(ptr,(unsigned char *)&tekst_read.regel);
        if ((tekst_read.size_tekst == 0) ||
            (ptr >= (unsigned char *)&tekst + sizeof(s_tekst)))
          fase = 2; // tekst ingelezen ga naar inlezen tekst_inst
      }
      break;
    case 2: // start inlezen tekst_inst uit taal eeprom
      if (EEP_Tekst_Read_First_String(1,&tekst_read))
      {
        // error
        error_cnt++;
        if (error_cnt > 10)
          fase = 4;
      }
      else
      {
        error_cnt = 0;
        ptr = (unsigned char *)&tekst_inst.Gekozen_Taal_14;
        ptr = EEP_Taal_Copier_Regel(ptr,(unsigned char *)&tekst_read.regel);
        if ((tekst_read.size_tekst == 0) ||
            (ptr >= (unsigned char *)&tekst_inst + sizeof(s_tekst_inst)))
          fase = 5; // tekst_inst inlezen gereed (klaar met inlezen)
        else
          fase = 3; // ga verder met inlezen 
      }
      break;
    case 3:   
      if (EEP_Tekst_Read_Next_String(&tekst_read))
      {
        // error
        error_cnt++;
        if (error_cnt > 10)
          fase = 4;
      }
      else
      {
        error_cnt = 0;
        ptr = EEP_Taal_Copier_Regel(ptr,(unsigned char *)&tekst_read.regel);
        if ((tekst_read.size_tekst == 0) ||
            (ptr >= (unsigned char *)&tekst_inst + sizeof(s_tekst_inst)))
          fase = 5; // tekst_inst inlezen gereed (klaar met inlezen)
      }
      break;
    case 4: // I2C1 niet OK
      // error wat doen
      error_cnt = 0;
      if (opt_alg.taalkeuze == USER)
      {
        CreateAlarm(&alarm_hr_alg.EEP_taal_al, SYSTEEM_AL_EEP_TAAL, 0, 0, 0, ZACHT_ALARM);
        eeprom_taal.Gekozen_Taal_14 = tekst_no_language;
        tekst = tekst_engels;
        tekst_inst = tekst_inst_engels;
      }  
      EEP_taal_init_switch = 1;
      EEP_taal_read_switch = 0;
      fase = 0;
      break;
    case 5: // inlezen gereed
      EEP_taal_read_switch = 0;
      fase = 0;
      break;  
  }
}

void EEP_Taal_Init(void)
{
unsigned char ch;
static unsigned int error_cnt = 0;

/*  #ifdef EMULATOR
  {
    if (Number_Of_Blocks(sizeof(s_tekst), EEP_TAAL_BLOCK_SIZE) + Number_Of_Blocks(sizeof(s_tekst_inst), EEP_TAAL_BLOCK_SIZE) > EEP_TAAL_BLOCK_AANT)
    {
      // eeprom taal is vol (slechts 63 bytes per blok van 64 beschikbaar)
      while (1);
    }  
  }
  #endif // EMULATOR   */
  IIC_EEP_LANGUAGE_WC = 1;
  IIC_EEP_LANGUAGE_WC_DP = OUTPUT;
  POCON3 = POCON3 & 0xFF0F;
  IIC_EEP_LANGUAGE_WC_ODP = PUSH_PULL;

  if (IIC_Read(IIC_ADDRESS_EEP_LANGUAGE, 0, 0, IIC_TWO_SUB_ADDRESSES, &ch, 1)) // check if eeprom is placed
  {
    error_cnt++;
    if (error_cnt >= 100) // een malig alarm na opstarten als usertaal ingesteld en geen EEPROM aanwezig
    {
      error_cnt = 0;
      if (opt_alg.taalkeuze == USER)
      {
        CreateAlarm(&alarm_hr_alg.EEP_taal_al, SYSTEEM_AL_EEP_TAAL, 0, 0, 0, ZACHT_ALARM);
        eeprom_taal.Gekozen_Taal_14 = tekst_no_language;
        tekst = tekst_engels;
        tekst_inst = tekst_inst_engels;
      }  
      // na honderd pogingen stop met proberen totdat opnieuw spanning op orion gezet wordt
      EEP_taal_init_switch = 0; 
    }  
    EEP_taal_eprom_aanwezig = 0;
    return;
  }
  error_cnt = 0;
  EEP_taal_eprom_aanwezig = 1;
  EEP_taal_init_switch = 0; 
}

void EEP_Taal_Proc(void)
{
  // aangesloten op I2C1
  if (EEP_taal_init_switch)
    EEP_Taal_Init();
  if (EEP_taal_eprom_aanwezig)
  {
    if (EEP_taal_bestaat_switch && (!EEP_taal_tekst_aanwezig || !EEP_taal_tekst_inst_aanwezig))
      EEP_Taal_Bestaat();
    else if (opt_alg.taalkeuze == USER)
    {
      if (EEP_taal_tekst_write_switch)
        EEP_Taal_Tekst_Write();  
      else if (EEP_taal_tekst_inst_write_switch)
        EEP_Taal_Tekst_Inst_Write();  
      else if (EEP_taal_read_switch && EEP_taal_tekst_aanwezig && EEP_taal_tekst_inst_aanwezig)
        EEP_Taal_Tekst_En_Tekst_Inst_Read();
    }    
  }
}
