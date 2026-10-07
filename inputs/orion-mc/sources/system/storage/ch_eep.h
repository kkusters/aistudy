// CH_EEP.H

#ifndef _CH_EEP_H
#define _CH_EEP_H

// 24C128 16k block write 64
// 24c64 8k block write 32
//#define EEP_BLOCK_SIZE 64 // 256kBit
#define EEP_BLOCK_SIZE ((unsigned int)128) // 512kBit
// size eeprom is 32k 
//#define EEP_SIZE 0x8000
#define EEP_SIZE 0x10000
// number of blocks in EEPROM
#define EEP_BLOCK_AANT (EEP_SIZE/EEP_BLOCK_SIZE)

bit EEP_Read_Block(unsigned int block_nr, unsigned char *data);
bit EEP_Write_Block(unsigned int block_nr, unsigned char *data, unsigned char checksum);

extern bit EEP_init_switch;
//extern bit EEP_read_switch;
//extern bit EEP_write_switch;
#endif
