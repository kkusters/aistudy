// C__EEP.C


#include "ch_define.h"

#include "ch_alg.h"
#include "ch_i2c.h"
#include "ch_eep.h"

// 24C256 32k block write 64
// 24C128 16k block write 64
// 24c64 8k block write 32
//#define EEP_BLOCK_SIZE 128
//#define EEP_SIZE 0x10000

#define IIC_ADDRESS_EEP_SETP_OPT 0xA0
#define IIC_EEP_SETP_OPT_WC P3_4
#define IIC_EEP_SETP_OPT_WC_DP DP3_4
#define IIC_EEP_SETP_OPT_WC_ODP ODP3_4

bit EEP_init_switch = 1;
//bit EEP_read_switch = 0;
//bit EEP_write_switch = 0;

_inline void EEP_Write_Enable(void)
{
  IIC_EEP_SETP_OPT_WC = 0;
}

_inline void EEP_Write_Disable(void)
{
  IIC_EEP_SETP_OPT_WC = 1;
}

static _near void EEP_Error(unsigned char error) // error = 1 by fault and 0 if ok
{
static int error_cnt = 0;

  if (error)
  {
    error_cnt++;
    if (error_cnt >= 5000) // 50 is afhankelijk van schrijven data naar eeprom
    {
      error_cnt = 5000;
      EEP_init_switch = 1;
    }
  }
  else
    error_cnt = 0;
}

bit EEP_Init(void)
{
unsigned char ch;

/*  #ifdef EMULATOR
  if (Number_Of_Blocks(sizeof(s_modules), EEP_BLOCK_SIZE) + 
      Number_Of_Blocks(sizeof(s_opt_io), EEP_BLOCK_SIZE) + 
      Number_Of_Blocks(sizeof(s_opt_alg), EEP_BLOCK_SIZE) + 
      Number_Of_Blocks(sizeof(s_opt_app), EEP_BLOCK_SIZE) + 
      Number_Of_Blocks(sizeof(s_setp_alg), EEP_BLOCK_SIZE) > EEP_BLOCK_AANT)
  {
    // eeprom opties/setpoints is vol (slechts 127(63) bytes per blok van 128(64) beschikbaar)
    while (1);
  }
  #endif // EMULATOR	   */
  IIC_EEP_SETP_OPT_WC = 1; // disable WP
  IIC_EEP_SETP_OPT_WC_ODP = 1;	// opendrain
  POCON3 = POCON3 & 0xFF0F;
  IIC_EEP_SETP_OPT_WC_DP = 1; // output
  
  if (IIC_Read(IIC_ADDRESS_EEP_SETP_OPT, 0, 0, IIC_TWO_SUB_ADDRESSES, &ch, 1)) // check if eeprom is placed
  {
    EEP_Error(1);
    return (1);
  }
  EEP_Error(0);
  EEP_init_switch = 0;
  return (0);
}

bit EEP_Read_Block(unsigned int block_nr, unsigned char *data)
{
unsigned int address = block_nr*EEP_BLOCK_SIZE;

  if (EEP_init_switch)
  {
    if (EEP_Init())
      return (1);
  }  
  if (IIC_Read(IIC_ADDRESS_EEP_SETP_OPT, address >> 8, address & 0x00FF, IIC_TWO_SUB_ADDRESSES, data, EEP_BLOCK_SIZE))
  {
    EEP_Error(1);
    return (1);
  } 
  EEP_Error(0);
  return (0);
}

bit EEP_Write_Block(unsigned int block_nr, unsigned char *data, unsigned char checksum)
{
unsigned char block[EEP_BLOCK_SIZE];
unsigned int address = block_nr*EEP_BLOCK_SIZE;

  if (EEP_init_switch)
  {
    if (EEP_Init())
      return (1);
  }  
  Copy_N_Bytes(block, data, EEP_BLOCK_SIZE-1);
  block[EEP_BLOCK_SIZE-1] = checksum;
  EEP_Write_Enable();
  if (IIC_Write(IIC_ADDRESS_EEP_SETP_OPT, address >> 8, address & 0x00FF, IIC_TWO_SUB_ADDRESSES, block, EEP_BLOCK_SIZE))
  {
    EEP_Write_Disable();
    EEP_Error(1);
    return (1);
  } 
  EEP_Write_Disable();
  EEP_Error(0);
  return (0);
}

