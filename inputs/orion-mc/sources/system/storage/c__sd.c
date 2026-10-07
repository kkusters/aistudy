// C__SD.C

#include <stdio.h>
#include <string.h> 

#include "ch_define.h"
#include "ch_alg.h"
#include "ch_alarm.h"
#include "ch_asc1.h"
#include "ch_can_backbone_appl.h"
#include "ch_can_io.h"
#include "ch_disp.h"
#include "ch_disp_diag_sd_card_1.h"
#include "ch_disp_option_0.h"
#include "ch_disp_option_1.h"
#include "ch_main.h"
#include "ch_pc_com.h"
#include "ch_sd_log.h"
#include "ch_tijd.h"
#include "ch_timer.h"
#include "ch_sd.h"

#ifdef SD_CARD

//*****************************************************************************
// JP 26-04-07
#define _MCU_ENDIAN     2
//#define _MCU_ENDIAN       0
// The _MCU_ENDIAN defines which access method is used to the FAT structure.
// 1: Enable word access.
// 2: Disable word access and use byte-by-byte access instead.
// When the architectural byte order of the MCU is big-endian and/or address
// miss-aligned access is prohibited, the _MCU_ENDIAN must be set to 2.
// If it is not the case, it can be set to 1 for good code efficiency.   

#define _USE_SJIS   1
// When _USE_SJIS is set to 1, Shift-JIS code transparency is enabled, otherwise
// only US-ASCII(7bit) code can be accepted as file/directory name.   

#define _USE_NTFLAG 1
// When _USE_NTFLAG is set to 1, upper/lower case of the file name is preserved.
// Note that the files are always accessed in case insensitive.   


//*****************************************************************************
#define MAX_READ_BUFFER_SD_CARD 512
#define MAX_WRITE_BUFFER_SD_CARD 512
#define WRITE_CNT_MAX MAX_WRITE_BUFFER_SD_CARD

//*****************************************************************************
// File access control and file status flags (FIL.flag)   

#define FA_READ             0x01
#define FA_OPEN_EXISTING    0x00
#define FA_WRITE            0x02
#define FA_CREATE_NEW       0x04
#define FA_CREATE_ALWAYS    0x08
#define FA_OPEN_ALWAYS      0x10
#define FA__WRITTEN         0x20
#define FA__ERROR           0x80

// FAT sub type (FATFS.fs_type)   

#define FS_FAT12    1
#define FS_FAT16    2
#define FS_FAT32    3


//*****************************************************************************
// nieuw SD kaart besturing
// JP 07-04-09

// MMC/SD command (in SPI)
#define CMD0   (0x40+0)  /* GO_IDLE_STATE */
#define CMD1   (0x40+1)  /* SEND_OP_COND */
#define CMD8   (0x40+8)  /* SEND_IF_COND */
#define CMD9   (0x40+9)  /* SEND_CSD */
#define CMD10  (0x40+10) /* SEND_CID */
#define CMD12  (0x40+12) /* STOP_TRANSMISSION */
#define CMD16  (0x40+16) /* SET_BLOCKLEN */
#define CMD17  (0x40+17) /* READ_SINGLE_BLOCK */
#define CMD18  (0x40+18) /* READ_MULTIPLE_BLOCK */
#define CMD24  (0x40+24) /* WRITE_BLOCK */
#define CMD25  (0x40+25) /* WRITE_MULTIPLE_BLOCK */
#define ACMD41 (0x40+41) /* SEND_OP_COND (ACMD) */
#define CMD55  (0x40+55) /* APP_CMD */
#define CMD58  (0x40+58) /* READ_OCR */

#define SD_CARD_CD P5_6
#define SD_CARD_CD_D P5D_6

#define SD_CARD_WP P5_7
#define SD_CARD_WP_D P5D_7

#define SD_CARD_CS P3_7
#define SD_CARD_CS_DP DP3_7
#define SD_CARD_CS_ODP ODP3_7

#define SD_CARD_DI P3_8
#define SD_CARD_DI_DP DP3_8
#define SD_CARD_DI_ODP ODP3_8
#define SD_CARD_DI_ALTSEL0 AS0P3_8

#define SD_CARD_DO P3_9
#define SD_CARD_DO_DP DP3_9
#define SD_CARD_DO_ODP ODP3_9
#define SD_CARD_DO_ALTSEL0 AS0P3_9

#define SD_CARD_CLK P3_13
#define SD_CARD_CLK_DP DP3_13
#define SD_CARD_CLK_ODP ODP3_13
#define SD_CARD_CLK_ALTSEL0 AS0P3_13

#define SD_CARD_TB SSC0_TB
#define SD_CARD_RB SSC0_RB

#define SD_OK              0
#define SD_ERROR           1
#define SD_NO_VALID_NAME   2
#define SD_NO_PATH         3
#define SD_NO_FILE         4
#define SD_ROOT            5
#define SD_WRITE_PROTECT   6
#define SD_EXIST           7
#define SD_DIR_NOT_FOUND   8
#define SD_CLUST_END       9
#define SD_INVALID_OBJECT 10
#define SD_DIR_NOT_EMPTY  11
#define SD_BEZIG          12

#define SD_ERROR_RESET     0
#define SD_ERROR_INCREMENT 1

#define R1_IN_IDLE_STATE        0x01
#define R1_ERASE_RESET          0x02
#define R1_ILLEGAL_COMMAND      0x04
#define R1_COM_CRC_ERROR        0x08
#define R1_ERASE_SEQUENCE_ERROR 0x10
#define R1_ADDRESS_ERROR        0x20
#define R1_PARAMETER_ERROR      0x40
#define R1_NOT_OK               0x80

// File attribute bits for directory entry

#define AM_RDO  0x01    /* Read only */
#define AM_HID  0x02    /* Hidden */
#define AM_SYS  0x04    /* System */
#define AM_VOL  0x08    /* Volume label */
#define AM_LFN  0x0F    /* LFN entry */
#define AM_DIR  0x10    /* Directory */
#define AM_ARC  0x20    /* Archive */

#define BS_jmpBoot          0
#define BS_OEMName          3
#define BPB_BytsPerSec      11
#define BPB_SecPerClus      13
#define BPB_RsvdSecCnt      14
#define BPB_NumFATs         16
#define BPB_RootEntCnt      17
#define BPB_TotSec16        19
#define BPB_Media           21
#define BPB_FATSz16         22
#define BPB_SecPerTrk       24
#define BPB_NumHeads        26
#define BPB_HiddSec         28
#define BPB_TotSec32        32
#define BS_55AA             510

#define BS_DrvNum           36
#define BS_BootSig          38
#define BS_VolID            39
#define BS_VolLab           43
#define BS_FilSysType       54

#define BPB_FATSz32         36
#define BPB_ExtFlags        40
#define BPB_FSVer           42
#define BPB_RootClus        44
#define BPB_FSInfo          48
#define BPB_BkBootSec       50
#define BS_DrvNum32         64
#define BS_BootSig32        66
#define BS_VolID32          67
#define BS_VolLab32         71
#define BS_FilSysType32     82

#define FSI_LeadSig         0
#define FSI_StrucSig        484
#define FSI_Free_Count      488
#define FSI_Nxt_Free        492

#define MBR_Table           446

#define DIR_Name            0
#define DIR_Attr            11
#define DIR_NTres           12
#define DIR_CrtTime         14
#define DIR_CrtDate         16
#define DIR_FstClusHI       20
#define DIR_WrtTime         22
#define DIR_WrtDate         24
#define DIR_FstClusLO       26
#define DIR_FileSize        28

#define N_ROOTDIR   512         /* Multiple of 32 and <= 2048 */
#define N_FATS      2           /* 1 or 2 */
#define MAX_SECTOR  131072000UL /* Maximum partition size */
#define MIN_SECTOR  2000UL      /* Minimum partition size */

// aantal karakters blok_cnt + lengte + checksum + '\r' + \'n'
#define SD_DATA_BYTES 50

#define OPT_RENAME_MAX 2

// Multi-byte word access macros

#if _MCU_ENDIAN == 1    /* Use word access */
#define LD_WORD(ptr)        (unsigned int)(*(unsigned int*)(unsigned char*)(ptr))
#define LD_DWORD(ptr)       (unsigned long)(*(unsigned long*)(unsigned char*)(ptr))
#define ST_WORD(ptr,val)    *(unsigned int*)(unsigned char*)(ptr)=(unsigned int)(val)
#define ST_DWORD(ptr,val)   *(unsigned long*)(BYTE*)(ptr)=(unsigned long)(val)
#else
#if _MCU_ENDIAN == 2    /* Use byte-by-byte access */
#define LD_WORD(ptr)        (unsigned int)(((unsigned int)*(unsigned char*)((ptr)+1)<<8)|(unsigned int)*(unsigned char*)(ptr))
#define LD_DWORD(ptr)       (unsigned long)(((unsigned long)*(unsigned char*)((ptr)+3)<<24)|((unsigned long)*(unsigned char*)((ptr)+2)<<16)|((unsigned int)*(unsigned char*)((ptr)+1)<<8)|*(unsigned char*)(ptr))
#define ST_WORD(ptr,val)    *(unsigned char*)(ptr)=(unsigned char)(val); *(unsigned char*)((ptr)+1)=(unsigned char)((unsigned int)(val)>>8)
#define ST_DWORD(ptr,val)   *(unsigned char*)(ptr)=(unsigned char)(val); *(unsigned char*)((ptr)+1)=(unsigned char)((unsigned int)(val)>>8); *(unsigned char*)((ptr)+2)=(unsigned char)((unsigned long)(val)>>16); *(unsigned char*)((ptr)+3)=(unsigned char)((unsigned long)(val)>>24)
#else
#error Do not forget to set _MCU_ENDIAN properly!
#endif
#endif

typedef enum
{
  F_RESET = 0,
  F_MOUNT,
  F_OPEN,
  F_CLOSE,
  F_READ,
  F_WRITE,
  F_LSEEK,
  F_SYNC,
  F_OPENDIR,
  F_READDIR,
  F_GETFREE,
  F_STAT,
  F_MKDIR,
  F_UNLINK,
  F_CHMOD,
  F_RENAME,
  F_MKFS
} e_operation;

// Directory object structure
typedef struct _DIR
{
  unsigned int  id;     // Owner file system mount ID
  unsigned int  index;  // Current index
  FATFS*  fs;           // Pointer to the owner file system object
  unsigned long sclust; // Start cluster
  unsigned long clust;  // Current cluster
  unsigned long sect;   // Current sector
} DIR;


typedef struct 
{
  unsigned char csd_structure;
  unsigned char taac;
  unsigned char nsac;
  unsigned char tran_speed;
  unsigned int  ccc;
  unsigned char read_bl_len;
  unsigned char read_bl_partial;
  unsigned char write_blk_misalign;
  unsigned char read_blk_misalign;
  unsigned char dsr_imp;
  unsigned long c_size;
  unsigned char vdd_r_curr_min; // only version 1
  unsigned char vdd_r_curr_max; // only version 1
  unsigned char vdd_w_curr_min; // only version 1
  unsigned char vdd_w_curr_max; // only version 1
  unsigned char c_size_mult;    // only version 1
  unsigned char erase_blk_en;
  unsigned char sector_size;
  unsigned char wp_grp_size;
  unsigned char wp_grp_enable;
  unsigned char r2w_factor;
  unsigned char write_bl_len;
  unsigned char write_bl_partial;
  unsigned char file_format_grp;
  unsigned char copy;
  unsigned char perm_write_protect;
  unsigned char tmp_write_protect;
  unsigned char file_format;
} s_csd;

typedef struct 
{
  unsigned char status;       // 0x80 bootable or 0x00 else error
  unsigned char CHS_first[3]; // 0x01, 0x01, 0x00
  unsigned char type;         // 0x0B FAT32
  unsigned char CHS_last[3];
  unsigned char LBA_first_sector[4];
  unsigned char number_of_blocks[4];
} s_partition_record;

typedef struct
{
  unsigned char jmpBoot[3];    //  0 default(0xEB, 0x58, 0x90)
  unsigned char OEMName[8];    //  3 default "MSDOS5.0"
  unsigned char BytsPerSec[2]; // 11 default 512 (0x00, 0x02)
  unsigned char SecPerClus;    // 13 default 8 (0x08)
  unsigned char RsvdSecCnt[2]; // 14 default 32 (0x20, 0x00)
  unsigned char Num_Fasts;     // 16 default 2
  unsigned char RootEntCnt[2]; // 17 default 0 (0x00, 0x00)
  unsigned char TotSec16[2];   // 19 default 0 (0x00, 0x00)
  unsigned char Media;         // 21 default 0xF8
  unsigned char FATSz16[2];    // 22 default 0 (0x00, 0x00)
  unsigned char SecPerTrk[2];  // 24 default 0x3F (0x3F, 0x00)
  unsigned char NumHeads[2];   // 26 default 0xFF (0xFF, 0x00)
  unsigned char HiddSec[4];    // 28 default 0x3F (0x3F, 0x00, 0x00, 0x00)
  unsigned char TotSec32[4];   // 32 berekend gelijk aan number_of_blocks partition_record
  unsigned char FATSz32[4];    // 36 berekend
  unsigned char ExtFlags[2];   // 40 default 0 (0x00, 0x00)
  unsigned char FSVer[2];      // 42 default 0 (0x00, 0x00)
  unsigned char RootClus[4];   // 44 default 2 (0x02, 0x00, 0x00, 0x00)
  unsigned char FSInfo[2];     // 48 default 1 (0x01, 0x00)
  unsigned char BkBootSec[2];  // 50 default 6 (0x06, 0x00)
  unsigned char Reserved[12];  // 52 all 0x00
  unsigned char DrvNum;        // 64 default 0
  unsigned char Reserved1;     // 65 0x00
  unsigned char Bootsig;       // 66 default 0x29
  unsigned char VolID[4];      // 67 
  unsigned char VolLab[11];    // 71 default "NO NAME    "
  unsigned char FilSysType[8]; // 82 default "FAT32   "
} s_boot_record;

static void SD_File_Reset(s_file *ptr, char *file_name, unsigned char *ptr_log_on, unsigned int rename_cnt_max, unsigned char hex, unsigned int adres);

s_csd csd;
//#pragma class HB=EXTENDED_MEMORY
#pragma noclear
s_file log_file[MAX_FILES];
#pragma clear
#pragma default_attributes
unsigned char sd_placed_disp = 0;
unsigned char sd_write_protect_disp = 0;
unsigned long disk_space;
unsigned long free_disk_space;
unsigned char read_write_setpoints = 0; // 0 = niets, 1 = write, 2 = read
unsigned char read_write_options = 0; // 0 = niets, 1 = write, 2 = read
#ifdef SD_MANAGEMENT
unsigned char read_write_management = 0; // 0 = niets, 1 = write, 2 = read
#endif // SD_MANAGEMENT

static unsigned int sd_fsid = 0; // File system mount ID
static unsigned char sd_init_state = 0;
static unsigned char sd_init_file_systeem_state = 0;
static unsigned char sd_blok_adressering = 0; // was BlkAddr
static FILINFO empty_file_info = {0};
//#pragma class HB=EXTENDED_MEMORY
#pragma noclear
static FATFS fs;
#pragma clear
#pragma default_attributes
static unsigned char file_control_init_switch = 1;

volatile unsigned char sd_status = STA_NOINIT | STA_NODISK; // Disk status

static unsigned char sd_read_buffer[MAX_WRITE_BUFFER_SD_CARD];
static unsigned int  sd_read_cnt;

//*****************************************************************************
void File_Put_Char(s_file *ptr, char ch)
// write one character byte
{
  if ((setp_alg.sd_card_status == 0) && // alleen loggen als sd card is active
      (sd_init_state == 99) &&
      (*ptr->ptr_log_on == 1) &&
      ((sd_status & STA_PROTECT) == 0) &&
      (ptr->write.cnt < MAX_FILE_BUFFER))
  {
    ptr->write.cnt += 1;
    if (ptr->write_cnt_max < ptr->write.cnt)
      ptr->write_cnt_max = ptr->write.cnt;  
    ptr->write.data[ptr->write.put_index] = ch;
    ptr->write.put_index += 1;
    ptr->write.put_index %= MAX_FILE_BUFFER;
  }  
}

static void File_Put_String(s_file *ptr, char *string)
// write a character string ended with '/0'
// don't send '/0' character
{
  while (*string)
  {
    File_Put_Char(ptr, *string);
    string++;
  }
}

void File_Printf(s_file *ptr, const char *format, ... )
{
va_list ap;
char data[1024];

  if ((setp_alg.sd_card_status == 0) && // alleen loggen als sd card is active
      (sd_init_state == 99) &&
      (*ptr->ptr_log_on == 1) &&
      ((sd_status & STA_PROTECT) == 0) &&
      (ptr->write.cnt < MAX_FILE_BUFFER))
  {
    va_start(ap, format);
    vsprintf(data, format, ap);
    va_end(ap);
    File_Put_String(ptr, data); 
  }
}

//*****************************************************************************

static void Fill_Read_Buffer(s_buffer *buffer, unsigned char *sd_buffer, unsigned int cnt)
{
  while (cnt)
  {
    if (buffer->cnt == 1024)
      break;
    buffer->data[buffer->put_index] = *sd_buffer;
    sd_buffer++;
    buffer->cnt++;
    buffer->put_index++;
    buffer->put_index %= 1024;
    cnt--;
  }
}

unsigned char Read_Buffer_Byte(s_buffer *buffer)
{
unsigned char c;
  
  if (buffer->cnt == 0)
    return (0);
  c = buffer->data[buffer->get_index];
  buffer->cnt--;
  buffer->get_index++;  
  buffer->get_index %= 1024;  
  return (c);
}

unsigned char Read_Buffer_Char(s_buffer *buffer)
{
unsigned char c;

  c = Asc_To_Hex(Read_Buffer_Byte(buffer));
  c <<= 4;
  c |= Asc_To_Hex(Read_Buffer_Byte(buffer));
  return (c);
}

static unsigned char Read_Buffer_Byte_Checksum(s_buffer *buffer)
{
unsigned char c;
  
  c = Read_Buffer_Byte(buffer);
  buffer->checksum ^= c;
  return (c);
}

unsigned char Read_Buffer_Char_Checksum(s_buffer *buffer)
{
unsigned char c;

  c = Asc_To_Hex(Read_Buffer_Byte_Checksum(buffer));
  c <<= 4;
  c |= Asc_To_Hex(Read_Buffer_Byte_Checksum(buffer));
  return (c);
}

static unsigned int Read_Buffer_Int_Checksum(s_buffer *buffer)
{
unsigned int i;

  i = Read_Buffer_Char_Checksum(buffer);
  i <<= 8;
  i |= Read_Buffer_Char_Checksum(buffer);
  return (i);
}

static unsigned long Read_Buffer_Long_Checksum(s_buffer *buffer)
{
unsigned long l;

  l = Read_Buffer_Int_Checksum(buffer);
  l <<= 16;
  l |= Read_Buffer_Int_Checksum(buffer);
  return (l);
}

//*****************************************************************************
static void File_Send_Byte(s_file *ptr, unsigned char c)
{
  c = Hex_To_Asc(c);
  File_Put_Char(ptr, c);
}

void File_Send_Char(s_file *ptr, unsigned char c)
{
  File_Send_Byte(ptr, (c >> 4) & 0x0F); 
  File_Send_Byte(ptr, c & 0x0F); 
}

static void File_Send_Byte_Checksum(s_file *ptr, unsigned char c)
{
  c = Hex_To_Asc(c);
  File_Put_Char(ptr, c);
  ptr->write.checksum ^= c;
}

void File_Send_Char_Checksum(s_file *ptr, unsigned char c)
{
  File_Send_Byte_Checksum(ptr, (c >> 4) & 0x0F); 
  File_Send_Byte_Checksum(ptr, c & 0x0F); 
}

void File_Send_Int_Checksum(s_file *ptr, unsigned int i)
{
  File_Send_Char_Checksum(ptr, (i >> 8) & 0xFF);
  File_Send_Char_Checksum(ptr, i & 0xFF);
}

void File_Send_Long_Checksum(s_file *ptr, unsigned long l)
{
  File_Send_Int_Checksum(ptr, (l >> 16) & 0xFFFF);
  File_Send_Int_Checksum(ptr, l & 0xFFFF);
}

//*****************************************************************************

void SD_Second_Control(void)
{
int loop;

  for (loop = 0; loop < MAX_FILES; loop++)
  {
    if ((*log_file[loop].ptr_log_on == 1) && 
        (log_file[loop].write_status == 0) &&
        (log_file[loop].write.cnt))
      log_file[loop].write_time_out++;
    else
      log_file[loop].write_time_out = 0;
  }
}

void File_Rename(s_file *ptr)
{
  ptr->rename_flag = 1;
}

//*****************************************************************************
static unsigned long SD_Get_Fattime(void)
{
unsigned long tmr;

  tmr = (((unsigned long)tijd.tm_year - 1980) << 25) |
        ((unsigned long)tijd.tm_mon << 21) |
        ((unsigned long)tijd.tm_mday << 16) |
        (unsigned int)(tijd.tm_hour << 11) |
        (unsigned int)(tijd.tm_min << 5) |
        (unsigned int)(tijd.tm_sec >> 1);
  return (tmr);
}
//*****************************************************************************

static void SD_Transmit_Byte(unsigned char dat)
{
  SD_CARD_TB = dat;
  _nop(); _nop(); _nop(); _nop(); _nop();
  while (SSC0_CON_BSY);
  dat = SD_CARD_RB;
}

static unsigned char SD_Receive_Byte(void)
{
unsigned char data;

  SD_CARD_TB = 0xFF;
  _nop(); _nop(); _nop(); _nop(); _nop();
  while (SSC0_CON_BSY);
  data = SD_CARD_RB;
  return (data);
}

#ifdef EMULATOR
static unsigned int sd_wait_ready_max = 0;
#endif // EMULATOR
static unsigned char SD_Wait_Ready(void)
// return SD_OK of SD_ERROR
{																		 
s_timer timer_wait_ready;
unsigned int sd_wait_ready;
unsigned char res;

  Timer_Set(&timer_wait_ready, 30000, TIME_BASE_1_MSEC);
  do
  {
    res = SD_Receive_Byte();
  }  
  while (res != 0xFF);
  sd_wait_ready = Timer_Delay(&timer_wait_ready);
  #ifdef EMULATOR
  if (sd_wait_ready_max < sd_wait_ready)
    sd_wait_ready_max = sd_wait_ready;
  #endif // EMULATOR
  if (res == 0xFF)
    return (SD_OK);
  else
    return (SD_ERROR);
}

static unsigned char SD_Transmit_Data(const unsigned char *buff) // 512 byte data block to be transmitted
// return SD_OK of SD_ERROR
{
unsigned char resp, wc;

  if (SD_Wait_Ready() != SD_OK)
    return (SD_ERROR);
  SD_Transmit_Byte(0xFE); // Xmit data token
  wc = 0;
  do
  { // Xmit the 512 byte data block to MMC
    SD_Transmit_Byte(*buff++);
    SD_Transmit_Byte(*buff++);
  } while (--wc);
  SD_Transmit_Byte(0xFF); // CRC (Dummy)
  SD_Transmit_Byte(0xFF);
  resp = SD_Receive_Byte(); // Reveive data response
  if ((resp & 0x1F) != 0x05) // If not accepted, return with error
    return (SD_ERROR);
  return (SD_OK);
}

#ifdef EMULATOR
int sd_receive_data_max = 0;
#endif // EMULATOR
static unsigned char SD_Receive_Data(unsigned char *buff, // Data buffer to store received data
                                     unsigned int nr)   // Byte count (must be even number)
// return SD_OK of SD_ERROR
{
s_timer timer_receive_data;
unsigned int sd_receive_data;
unsigned char token;

  Timer_Set(&timer_receive_data, 30000, TIME_BASE_1_MSEC);
  do
  { // Wait for data packet in timeout of 100ms
    token = SD_Receive_Byte();
  } while ((token == 0xFF));
  sd_receive_data = Timer_Delay(&timer_receive_data);
  #ifdef EMULATOR
  if (sd_receive_data_max < sd_receive_data)
    sd_receive_data_max = sd_receive_data;
  #endif // EMULATOR
  if (token != 0xFE)
    return (SD_ERROR); // If not valid data token, retutn with error
  do
  { // Receive the data block into buffer
    *buff++ = SD_Receive_Byte();
    *buff++ = SD_Receive_Byte();
  } while (nr -= 2);
  SD_Receive_Byte(); // Discard CRC
  SD_Receive_Byte();
  return (SD_OK); // Return with success
}

//*****************************************************************************
#ifdef EMULATOR
int sd_command_respond_r1_max = 0;
#endif // EMULATOR
static unsigned char SD_Command_Respond_R1(void)
{
s_timer timer_command_respond_r1;
unsigned int sd_command_respond_r1;
unsigned char res;

  Timer_Set(&timer_command_respond_r1, 30000, TIME_BASE_1_MSEC);
  do
  {
    res = SD_Receive_Byte();
  }  
  while (res & R1_NOT_OK);
  sd_command_respond_r1 = Timer_Delay(&timer_command_respond_r1);
  #ifdef EMULATOR
  if (sd_command_respond_r1_max < sd_command_respond_r1)
    sd_command_respond_r1_max = sd_command_respond_r1;
  #endif // EMULATOR
  return (res);
}

#ifdef EMULATOR
int sd_command_respond_r1b_max = 0;
#endif // EMULATOR
static unsigned char SD_Command_Respond_R1b(void)
{
s_timer timer_command_respond_r1b;
unsigned int sd_command_respond_r1b;
unsigned char res;
unsigned char busy;

  res = SD_Command_Respond_R1();
  if ((res & (R1_NOT_OK | R1_ILLEGAL_COMMAND | R1_COM_CRC_ERROR | R1_ADDRESS_ERROR | R1_PARAMETER_ERROR)) == 0)
  {
    Timer_Set(&timer_command_respond_r1b, 30000, TIME_BASE_1_MSEC);
    do
    {
      busy = SD_Receive_Byte();
    }
    while (busy == 0);
    sd_command_respond_r1b = Timer_Delay(&timer_command_respond_r1b);
    #ifdef EMULATOR
    if (sd_command_respond_r1b_max < sd_command_respond_r1b)
      sd_command_respond_r1b_max = sd_command_respond_r1b;
    #endif // EMULATOR
  }
  return (res);
}

static unsigned char SD_Command_Respond_R3(unsigned char *data_ptr)
{
unsigned char res;

  res = SD_Command_Respond_R1();
  if ((res & (R1_NOT_OK | R1_ILLEGAL_COMMAND | R1_COM_CRC_ERROR | R1_ADDRESS_ERROR | R1_PARAMETER_ERROR)) == 0)
  {
    data_ptr[0] = SD_Receive_Byte();
    data_ptr[1] = SD_Receive_Byte();
    data_ptr[2] = SD_Receive_Byte();
    data_ptr[3] = SD_Receive_Byte();
  }
  return (res);
}

static unsigned char SD_Command_Respond_R7(unsigned char *data_ptr)
{
unsigned char res;

  res = SD_Command_Respond_R1();
  if ((res & (R1_NOT_OK | R1_ILLEGAL_COMMAND | R1_COM_CRC_ERROR | R1_ADDRESS_ERROR | R1_PARAMETER_ERROR)) == 0)
  {
    data_ptr[0] = SD_Receive_Byte();
    data_ptr[1] = SD_Receive_Byte();
    data_ptr[2] = SD_Receive_Byte();
    data_ptr[3] = SD_Receive_Byte();
  }
  return (res);
}

static unsigned char SD_Command_Respond(unsigned char cmd, unsigned char *data_ptr)
{
  switch (cmd)
  {
    case CMD0:   return (SD_Command_Respond_R1());
    case CMD1:   return (SD_Command_Respond_R1());
    case CMD8:   return (SD_Command_Respond_R7(data_ptr));
    case CMD9:   return (SD_Command_Respond_R1());
    case CMD10:  return (SD_Command_Respond_R1());
    case CMD12:  return (SD_Command_Respond_R1b());
    case CMD16:  return (SD_Command_Respond_R1());
    case CMD17:  return (SD_Command_Respond_R1());
    case CMD18:  return (SD_Command_Respond_R1());
    case CMD24:  return (SD_Command_Respond_R1());
    case CMD25:  return (SD_Command_Respond_R1());
    case ACMD41: return (SD_Command_Respond_R1());
    case CMD55:  return (SD_Command_Respond_R1());
    case CMD58:  return (SD_Command_Respond_R3(data_ptr));
    default:     return (4); 
  }
}

static unsigned char SD_Send_Command(unsigned char cmd, unsigned long arg, unsigned char *data_ptr)
{
  if (SD_Wait_Ready() != SD_OK)
    return (0xFF);
  SD_Transmit_Byte(cmd);
  SD_Transmit_Byte((unsigned char)(arg >> 24));
  SD_Transmit_Byte((unsigned char)(arg >> 16));
  SD_Transmit_Byte((unsigned char)(arg >> 8));
  SD_Transmit_Byte((unsigned char)(arg));
  if(cmd==CMD0) // CRC for CMD0
    SD_Transmit_Byte(0x95);
  else if (cmd == CMD8) // CRC for CMD8 (0x1AA)
    SD_Transmit_Byte(0x87);
  else // for other commands no CRC nessessary
    SD_Transmit_Byte(0xFF);
  return (SD_Command_Respond(cmd,data_ptr));
}

//*****************************************************************************
#ifdef EMULATOR
int sd_error_cnt_max = 0;
#endif // EMULATOR
static void SD_Error_Cnt(unsigned char set)
{
static unsigned char error_cnt = 0;

  if (set == SD_ERROR_RESET)
    error_cnt = 0;
  else if (error_cnt < 100)
  {
    error_cnt++;
    #ifdef EMULATOR
    if (sd_error_cnt_max < error_cnt)
      sd_error_cnt_max = error_cnt;
    #endif // EMULATOR
  }  
  else
  {
    error_cnt = 0;
    sd_init_state = 0;
  }  
}

#ifdef EMULATOR
int sd_ready_cnt_max = 0;
#endif // EMULATOR
static unsigned char SD_Ready(void)
{
static int error_cnt = 0;
s_timer timer_ready;
unsigned char res;

  SD_CARD_CS = 0; // CS = L
  Timer_Set(&timer_ready, 5, TIME_BASE_1_MSEC);
  do
  {
    res = SD_Receive_Byte();
  }  
  while ((res != 0xFF) && Timer_Expired(&timer_ready));
  SD_CARD_CS = 1; // CS = H
  SD_Receive_Byte(); // Idle (Release DO)
  if (res == 0xFF)
  {
    error_cnt = 0;
    return (SD_OK);
  }  
  else
  {
    if (error_cnt < 500)
    {
      error_cnt++;
      #ifdef EMULATOR
      if (sd_ready_cnt_max < error_cnt)
        sd_ready_cnt_max = error_cnt;
      #endif // EMULATOR
    }    
    else
    {
      error_cnt = 0;
      sd_init_state = 0;
    }  
    return (SD_BEZIG);
  }  
}

static unsigned char sd_read_sector_state = 0;
static unsigned char SD_Read_Sector(unsigned char *buff,  // Pointer to the data buffer to store read data
                                    unsigned long sector) // Start sector number (LBA)
// return SD_OK, SD_BEZIG of SD_ERROR
{
unsigned char data[4]; 
s_timer timer_read_sector;

  Timer_Set(&timer_read_sector, 5, TIME_BASE_1_MSEC);
  switch (sd_read_sector_state)
  {
    default:
      sd_read_sector_state = 0;
      return (SD_ERROR);
    case 0:
      if (!sd_blok_adressering)
        sector *= 512; // Convert to byte address if needed

      SD_CARD_CS = 0; // CS = L

      // Single block read
      if (SD_Send_Command(CMD17, sector, data) != 0)
      {
        SD_CARD_CS = 1; // CS = H
        SD_Error_Cnt(SD_ERROR_INCREMENT);
        sd_read_sector_state = 0;
        return (SD_ERROR);
      }  
      if (SD_Receive_Data(buff, 512) != SD_OK)
      {
        SD_CARD_CS = 1; // CS = H
        SD_Error_Cnt(SD_ERROR_INCREMENT);
        sd_read_sector_state = 0;
        return (SD_ERROR);
      }
      SD_CARD_CS = 1; // CS = H
      SD_Receive_Byte(); // Idle (Release DO)
      SD_Error_Cnt(SD_ERROR_RESET);
      sd_read_sector_state = 1;
      if (Timer_Expired(&timer_read_sector))
        return (SD_BEZIG);
    case 1:
      if (SD_Ready() != SD_OK)
        return (SD_BEZIG);
      else
      {
        sd_read_sector_state = 0;
        return (SD_OK);
      }  
  }
}

static unsigned char sd_write_sector_state = 0;
static unsigned char SD_Write_Sector(const unsigned char *buff, // Pointer to the data to be written
                                     unsigned long sector)      // Start sector number
// return SD_OK, SD_BEZIG of SD_ERROR
{
unsigned char data[4];
s_timer timer_write_sector;

  Timer_Set(&timer_write_sector, 5, TIME_BASE_1_MSEC);
  switch (sd_write_sector_state)
  {
    default:
      sd_write_sector_state = 0;
    case 0:
      if (sd_status & STA_PROTECT)
        return (SD_ERROR);
    
      if (!sd_blok_adressering)
        sector *= 512; // Convert to byte address if needed

      SD_CARD_CS = 0; // CS = L

      if (SD_Send_Command(CMD24, sector, data) != 0)
      {
        SD_CARD_CS = 1; // CS = H
        SD_Error_Cnt(SD_ERROR_INCREMENT);
        sd_write_sector_state = 0;
        return (SD_ERROR);
      }  
      if (SD_Transmit_Data(buff) != SD_OK)
      {
        SD_CARD_CS = 1; // CS = H
        SD_Error_Cnt(SD_ERROR_INCREMENT);
        sd_write_sector_state = 0;
        return (SD_ERROR);
      }  
      SD_CARD_CS = 1; // CS = H
      SD_Receive_Byte(); // Idle (Release DO)
      SD_Error_Cnt(SD_ERROR_RESET);
      sd_write_sector_state = 1;
      if (Timer_Expired(&timer_write_sector))
        return (SD_BEZIG);
    case 1:    
      if (SD_Ready() != SD_OK)
        return (SD_BEZIG);
      else
      {
        sd_write_sector_state = 0;
        return (SD_OK);
      }  
  }
}

static unsigned char sd_move_window_state = 0;
static unsigned char SD_Move_Window(unsigned long sector) // Sector number to make apperance in the FatFs->win 
// return SD_OK, SD_BEZIG of SD_ERROR
{
s_timer timer_move_window;
static unsigned long wsect;
static unsigned char n;

  Timer_Set(&timer_move_window, 5, TIME_BASE_1_MSEC);
  switch (sd_move_window_state)
  {
    default:
      sd_move_window_state = 0;
    case 0:
      wsect = fs.winsect;
      if (wsect != sector)
      {
        if (fs.winflag)
        {
          sd_move_window_state = 1;
        }
        else
        {
          sd_move_window_state = 10;
          goto sd_move_window_label_10;
        }  
      }
      else
      {
        return (SD_OK);
      }
    case 1:
      // Write back dirty window if needed 
      switch (SD_Write_Sector(fs.win, wsect))
      {
        case SD_OK:
          break;
        case SD_ERROR:
          sd_move_window_state = 0;
          return (SD_ERROR);
        case SD_BEZIG:
          return (SD_BEZIG);
      }
      if (wsect < (fs.fatbase + fs.sects_fat))
      { // In FAT area 
        n = fs.n_fats;
        if (n >= 2)
        {
          sd_move_window_state = 2;  
          if (Timer_Expired(&timer_move_window))
            return (SD_BEZIG);
        }
        else
        {
          fs.winflag = 0;
          sd_move_window_state = 10;  
          if (Timer_Expired(&timer_move_window))
            return (SD_BEZIG);
          goto sd_move_window_label_10;
        }    
      }  
      else
      { 
        fs.winflag = 0;
        sd_move_window_state = 10;  
        if (Timer_Expired(&timer_move_window))
          return (SD_BEZIG);
        goto sd_move_window_label_10;
      }
sd_move_window_label_2:
    case 2:
      wsect += fs.sects_fat;
      sd_move_window_state = 3;  
    case 3:
      switch (SD_Write_Sector(fs.win, wsect))
      {
        case SD_OK:
          break;
        case SD_ERROR:
          sd_move_window_state = 0;
          return (SD_ERROR);
        case SD_BEZIG:
          return (SD_BEZIG);
      }
      sd_move_window_state = 4;  
      if (Timer_Expired(&timer_move_window))
        return (SD_BEZIG);
    case 4:
      n--;
      if (n >= 2)
      {
        sd_move_window_state = 2;
        if (Timer_Expired(&timer_move_window))
          return (SD_BEZIG);
        goto sd_move_window_label_2;  
      }
      else
      {
        fs.winflag = 0;
        sd_move_window_state = 10;
        if (Timer_Expired(&timer_move_window))
          return (SD_BEZIG);
      }
sd_move_window_label_10:
    case 10:
      if (sector)
      {
        switch (SD_Read_Sector(fs.win, sector))
        {
          case SD_OK:
            break;
          case SD_ERROR:
            sd_move_window_state = 0;
            return (SD_ERROR);
          case SD_BEZIG:
            return (SD_BEZIG);
        }
        fs.winsect = sector;
      }
      sd_move_window_state = 0;
      return (SD_OK);
  }  
}

//*****************************************************************************
static void SD_State(void)
{
static unsigned char wp,cd;
unsigned char s;

  SD_CARD_CD_D = DIGITAL;
  SD_CARD_WP_D = DIGITAL;
  if ((wp == SD_CARD_WP) && (cd == SD_CARD_CD))
  { // Have contacts stabled?
    s = sd_status;

    if (wp == 1) // WP is H (write protected)
      s |= STA_PROTECT;
    else // WP is L (write enabled)
      s &= ~STA_PROTECT;

    if (cd == 1) // INS = H (Socket empty)
      s |= (STA_NODISK | STA_NOINIT);
    else // INS = L (Card inserted)
      s &= ~STA_NODISK;

    sd_status = s;
  }
  else
  {
    wp = SD_CARD_WP;
    cd = SD_CARD_CD;
  }
  sd_placed_disp = (sd_status & STA_NODISK) ? 0 : 1;
  if (sd_placed_disp)
  {
    sd_write_protect_disp = (sd_status & STA_PROTECT) ? 1 : 0; // 0 = read write; 1 = write
	if (setp_alg.sd_card_status == 3)
	  setp_alg.sd_card_status = 0;
  }
  else
  {
    setp_alg.sd_card_status = 3;
  }
}

//*****************************************************************************
static void SD_Fill_Struct_CSD_V1(unsigned char const *csd_data)
{
  csd.csd_structure = (csd_data[0] >> 6) & 0x03;
  csd.taac = csd_data[1];
  csd.nsac = csd_data[2];
  csd.tran_speed = csd_data[3];
  csd.ccc = ((unsigned int)csd_data[4] << 4) | ((csd_data[5] >> 4) & 0x0F);
  csd.read_bl_len = csd_data[5] & 0x0F;
  csd.read_bl_partial = (csd_data[6] >> 7) & 0x01;
  csd.write_blk_misalign = (csd_data[6] >> 6) & 0x01;
  csd.read_blk_misalign = (csd_data[6] >> 5) & 0x01;
  csd.dsr_imp = (csd_data[6] >> 4) & 0x01;
  csd.c_size = (((unsigned int)csd_data[6] << 10) & 0x0C00) | ((unsigned int)csd_data[7] << 2) | ((csd_data[8] >> 6) & 0x03);
  csd.vdd_r_curr_min = (csd_data[8] >> 3) & 0x07; // only version 1
  csd.vdd_r_curr_max = csd_data[8] & 0x07; // only version 1
  csd.vdd_w_curr_min = (csd_data[9] >> 5) & 0x07; // only version 1
  csd.vdd_w_curr_max = (csd_data[9] >> 2) & 0x07; // only version 1
  csd.c_size_mult = ((csd_data[9] << 1) & 0x06) | ((csd_data[10] >> 7) & 0x01);    // only version 1
  csd.erase_blk_en = (csd_data[10] >> 6) & 0x01;
  csd.sector_size = ((csd_data[10] << 1) & 0x7E) | ((csd_data[11] >> 7) & 0x01);
  csd.wp_grp_size = csd_data[11] & 0x7F;
  csd.wp_grp_enable = (csd_data[12] >> 7) & 0x01;
  csd.r2w_factor = (csd_data[12] >> 2) & 0x07;
  csd.write_bl_len = ((csd_data[12] << 2) & 0x0C) | ((csd_data[13] >> 6) & 0x03);
  csd.write_bl_partial = (csd_data[13] >> 5) & 0x01;
  csd.file_format_grp = (csd_data[14] >> 7) & 0x01;
  csd.copy = (csd_data[14] >> 6) & 0x01;
  csd.perm_write_protect = (csd_data[14] >> 5) & 0x01;
  csd.tmp_write_protect = (csd_data[14] >> 4) & 0x01;
  csd.file_format = (csd_data[14] >> 2) & 0x03;
}

static void SD_Fill_Struct_CSD_V2(unsigned char const *csd_data)
{
  csd.csd_structure = (csd_data[0] >> 6) & 0x03;
  csd.taac = csd_data[1];
  csd.nsac = csd_data[2];
  csd.tran_speed = csd_data[3];
  csd.ccc = ((unsigned int)csd_data[4] << 4) | ((csd_data[5] >> 4) & 0x0F);
  csd.read_bl_len = csd_data[5] & 0x0F;
  csd.read_bl_partial = (csd_data[6] >> 7) & 0x01;
  csd.write_blk_misalign = (csd_data[6] >> 6) & 0x01;
  csd.read_blk_misalign = (csd_data[6] >> 5) & 0x01;
  csd.dsr_imp = (csd_data[6] >> 4) & 0x01;
  csd.c_size = (((unsigned long)csd_data[7] << 16) & 0x003F0000) | ((unsigned int)csd_data[8] << 8) | csd_data[9];
  csd.erase_blk_en = (csd_data[10] >> 6) & 0x01;
  csd.sector_size = ((csd_data[10] << 1) & 0x7E) | ((csd_data[11] >> 7) & 0x01);
  csd.wp_grp_size = csd_data[11] & 0x7F;
  csd.wp_grp_enable = (csd_data[12] >> 7) & 0x01;
  csd.r2w_factor = (csd_data[12] >> 2) & 0x07;
  csd.write_bl_len = ((csd_data[12] << 2) & 0x0C) | ((csd_data[13] >> 6) & 0x03);
  csd.write_bl_partial = (csd_data[13] >> 5) & 0x01;
  csd.file_format_grp = (csd_data[14] >> 7) & 0x01;
  csd.copy = (csd_data[14] >> 6) & 0x01;
  csd.perm_write_protect = (csd_data[14] >> 5) & 0x01;
  csd.tmp_write_protect = (csd_data[14] >> 4) & 0x01;
  csd.file_format = (csd_data[14] >> 2) & 0x03;
}

static unsigned char sd_read_csd_state = 0;
static unsigned char SD_Read_Csd(void)
{
static unsigned char csd_data[16];
unsigned char n;
unsigned char data[4]; 
s_timer timer_read_csd;

  Timer_Set(&timer_read_csd, 5, TIME_BASE_1_MSEC);
  switch (sd_read_csd_state)
  {
    default:
      sd_read_csd_state = 0;
      return (SD_ERROR);
    case 0:
      SD_CARD_CS = 0; // CS = L

      // Single block read
      if (SD_Send_Command(CMD9, 0, data) != 0)
      {
        SD_CARD_CS = 1; // CS = H
        SD_Error_Cnt(SD_ERROR_INCREMENT);
        sd_read_csd_state = 0;
        return (SD_ERROR);
      }  
      if (SD_Receive_Data(csd_data, 16) != SD_OK)
      {
        SD_CARD_CS = 1; // CS = H
        SD_Error_Cnt(SD_ERROR_INCREMENT);
        sd_read_csd_state = 0;
        return (SD_ERROR);
      }
      SD_Receive_Byte(); // Idle (Release DO)
      SD_Error_Cnt(SD_ERROR_RESET);
      sd_read_csd_state = 1;
      if (Timer_Expired(&timer_read_csd))
        return (SD_BEZIG);
    case 1:
      if (SD_Ready() != SD_OK)
        return (SD_BEZIG);
      sd_read_csd_state = 0;
      n = csd_data[0] >> 6;
      switch (n)
      {
        default:
          return (SD_ERROR);
        case 0: // CSD ver 1.XX
          SD_Fill_Struct_CSD_V1(csd_data);
          return (SD_OK);
        case 1:  // CSD ver 2.00
          SD_Fill_Struct_CSD_V2(csd_data);
          return (SD_OK);
      }
  }
}

//*****************************************************************************
static unsigned char SD_Auto_Mount(const char **path, // Pointer to pointer to the path name (drive number)
                                   unsigned char chk_wp)       // !=0: Check media write protection for wrinting fuctions
// return SD_OK of SD_WRITE_PROTECT
{
const char *p = *path;

  while (*p == ' ')
    p++; // Strip leading spaces
  //if (*p == '/')
  if (*p == '\\')
    p++; // Strip heading slash
  *path = p; // Return pointer to the path name

  // Check if the logical drive has been mounted or not
  if (chk_wp && (sd_status & STA_PROTECT)) // Check write protection if needed
    return (SD_WRITE_PROTECT);
  return (SD_OK); // The file system object is valid
}

//-----------------------------------------------------------------------------
//unsigned char SD_F_Getfree(const char *path) // Logical drive number
//{
//  SD_Auto_Mount(&path, 0);
//  // If number of free cluster is valid, return it without cluster scan.
//  if (fs.free_clust <= fs.max_clust - 2)
//    return (SD_OK);
//  else
//    return (SD_ERROR);
//}

//-----------------------------------------------------------------------------
static unsigned long SD_Clust_To_Sect(unsigned long clust) // Cluster# to be converted
// !=0: sector number, 0: failed - invalid cluster# 
{
  clust -= 2;
  if (clust >= (fs.max_clust - 2))
    return (0); // Invalid cluster#
  return (clust * fs.sects_clust + fs.database);
}

//-----------------------------------------------------------------------------

static unsigned char sd_get_cluster_state = 0;
static unsigned char SD_Get_Cluster(unsigned long clust, unsigned long *ptr_link) // Cluster# to get the link information
// return SD_OK, SD_BEZIG of SD_ERROR
{
s_timer timer_get_cluster;

  Timer_Set(&timer_get_cluster, 5, TIME_BASE_1_MSEC);
  switch (sd_get_cluster_state)
  {
    default:
      sd_get_cluster_state = 0;
    case 0:
      if (clust >= 2 && clust < fs.max_clust)
      {
        switch (fs.fs_type)
        {
          case FS_FAT16:
            sd_get_cluster_state = 1;
            goto sd_get_cluster_label_1;
          case FS_FAT32:
            sd_get_cluster_state = 2;
            goto sd_get_cluster_label_2;
          default:  
            *ptr_link = 1;
            return (SD_OK); // There is no cluster information, or an error occured 
        }
      }  
      else
      {
        *ptr_link = 1;
        return (SD_OK); // There is no cluster information, or an error occured 
      }
sd_get_cluster_label_1:
    case 1:  
      switch (SD_Move_Window(fs.fatbase + clust / 256))
      {
        case SD_OK:
          sd_get_cluster_state = 0;
          *ptr_link = LD_WORD(&fs.win[((unsigned int)clust * 2) % 512]); 
          return (SD_OK);
        default: 
        case SD_ERROR:
          sd_get_cluster_state = 0;
          *ptr_link = 1;
          return (SD_ERROR);
        case SD_BEZIG:
          return (SD_BEZIG);
      }
      break;
sd_get_cluster_label_2:
    case 2:
      switch (SD_Move_Window(fs.fatbase + clust / 128))
      {
        case SD_OK:
          sd_get_cluster_state = 0;
          *ptr_link = LD_DWORD(&fs.win[((unsigned int)clust * 4) % 512]) & 0x0FFFFFFF; 
          return (SD_OK);
        default: 
        case SD_ERROR:
          sd_get_cluster_state = 0;
          *ptr_link = 1;
          return (SD_ERROR);
        case SD_BEZIG:
          return (SD_BEZIG);
      }
      break;
  }
}

//-----------------------------------------------------------------------------
static unsigned char SD_Put_Cluster(unsigned long clust, // Cluster# to change
                                    unsigned long val)   // New value to mark the cluster
{
// return SD_OK of SD_ERROR
  switch (fs.fs_type)
  {
    case FS_FAT16:
      switch (SD_Move_Window(fs.fatbase + clust / 256))
      {
        case SD_OK:
          ST_DWORD(&fs.win[((unsigned int)clust * 2) % 512], val);
          fs.winflag = 1;
          return (SD_OK);
        default:
        case SD_ERROR:
          return (SD_ERROR);
        case SD_BEZIG:
          return (SD_BEZIG);
      }
    case FS_FAT32:
      switch (SD_Move_Window(fs.fatbase + clust / 128))
      {
        case SD_OK:
          ST_DWORD(&fs.win[((unsigned int)clust * 4) % 512], val);
          fs.winflag = 1;
          return (SD_OK);
        default:
        case SD_ERROR:
          return (SD_ERROR);
        case SD_BEZIG:
          return (SD_BEZIG);
      }
    default:
      return (SD_ERROR);
  }
}

//-----------------------------------------------------------------------------

static unsigned char sd_next_dir_entry_state = 0;
static unsigned char SD_Next_Dir_Entry(DIR *dirobj)
// return SD_OK, SD_ERROR, SD_DIR_NOT_FOUND of SD_BEZIG
{
unsigned long clust;
static unsigned int idx;
s_timer timer_next_dir_entry;

  Timer_Set(&timer_next_dir_entry, 5, TIME_BASE_1_MSEC);
  switch (sd_next_dir_entry_state)
  {
    default:
      sd_next_dir_entry_state = 0;
    case 0:
      idx = dirobj->index + 1;
      if ((idx & 15) == 0)
      { // Table sector changed?
        dirobj->sect++; // Next sector
        if (!dirobj->clust)
        { // In static table
          if (idx >= fs.n_rootdir)
            return (SD_DIR_NOT_FOUND); // Reached to end of table
        }
        else
        { // In dynamic table
          if (((idx / 16) & (fs.sects_clust - 1)) == 0)
          { // Cluster changed?
            sd_next_dir_entry_state = 1;
            goto sd_next_dir_entry_label_1;
          }
        }
      }
      dirobj->index = idx; // Lower 4 bit of dirobj->index indicates offset in dirobj->sect
      return (SD_OK);
sd_next_dir_entry_label_1:
    case 1:      
      switch (SD_Get_Cluster(dirobj->clust, &clust))
      {
        case SD_OK:
          break;
        case SD_ERROR:
          sd_next_dir_entry_state = 0;
          return (SD_ERROR);
        case SD_BEZIG:
          return (SD_BEZIG);
      }
      if (clust < 2 || clust >= fs.max_clust) // Reached to end of table
        return (SD_DIR_NOT_FOUND);
      dirobj->clust = clust; // Initialize for new cluster
      dirobj->sect = SD_Clust_To_Sect(clust);
      dirobj->index = idx; // Lower 4 bit of dirobj->index indicates offset in dirobj->sect
      sd_next_dir_entry_state = 0;
      return (SD_OK);
  }
}
//-----------------------------------------------------------------------------

static char SD_Make_Dirfile(const char **path, // Pointer to the file path pointer
                            char *dirname)     // Pointer to directory name buffer {Name(8), Ext(3), NT flag(1)}
// 1: error - detected an invalid format, '\0'or'/': next character */
{
unsigned char n, t, c, a, b;

  memset(dirname, ' ', 8+3); // Fill buffer with spaces
  a = 0; b = 0x18; // NT flag
  n = 0; t = 8;
  for (;;)
  {
    c = *(*path)++;
    //if (c == '\0' || c == '/')
    if (c == '\0' || c == '\\')
    { // Reached to end of str or directory separator
      if (n == 0)
        break;
      dirname[11] = _USE_NTFLAG ? (a & b) : 0;
      return (c);
    }
    if (c <= ' ' || c == 0x7F)
      break; // Reject invisible chars
    if (c == '.')
    {
      if(!(a & 1) && n >= 1 && n <= 8)
      { // Enter extension part
        n = 8;
        t = 11;
        continue;
      }
      break;
    }
    if (_USE_SJIS &&
        ((c >= 0x81 && c <= 0x9F) || // Accept S-JIS code
        (c >= 0xE0 && c <= 0xFC)))
    {
      if (n == 0 && c == 0xE5) // Change heading \xE5 to \x05
        c = 0x05;
      a ^= 1;
      goto sd_md_l2;
    }
    if (c == '"')
      break; // Reject "
    if (c <= ')')
      goto sd_md_l1; // Accept ! # $ % & ' ( )
    if (c <= ',')
      break; // Reject * + ,
    if (c <= '9')
      goto sd_md_l1; // Accept - 0-9
    if (c <= '?')
      break; // Reject : ; < = > ?
    if (!(a & 1))
    { // These checks are not applied to S-JIS 2nd byte
      if (c == '|')
        break; // Reject |
      if (c >= '[' && c <= ']')
        break; // Reject [ \ ]
      if (_USE_NTFLAG && c >= 'A' && c <= 'Z')
        (t == 8) ? (b &= ~(unsigned char)0x08) : (b &= ~(unsigned char)0x10);
      if (c >= 'a' && c <= 'z')
      { // Convert to upper case
        c -= 0x20;
        if (_USE_NTFLAG) (t == 8) ? (a |= 0x08) : (a |= 0x10);
      }
    }
sd_md_l1:
    a &= ~(unsigned char)1; 
sd_md_l2:
    if (n >= t)
      break;
    dirname[n++] = c;
  }
  return (1);
}

//-----------------------------------------------------------------------------
static unsigned char sd_trace_path_state = 0;
static unsigned char SD_Trace_Path(DIR *dirobj_new,         // Pointer to directory object to return last directory
                                   char *fn_new,            // Pointer to last segment name to return
                                   const char *path_new,    // Full-path strin to trace a file or directory
                                   unsigned char **dir_new) // Directory pointer in win[] to return
// return SD_OK, SD_ERROR, SD_NO_VALID_NAME, SD_NO_PATH, SD_NO_FILE, SD_ROOT, SD_DIR_NOT_FOUND of SD_BEZIG
{
static DIR *dirobj;
static char *fn;
static const char *path;
static unsigned char **dir;
static unsigned long clust;
static char ds;
static unsigned char *dptr;
int cnt = 0;
s_timer timer_trace_path;

  Timer_Set(&timer_trace_path, 5, TIME_BASE_1_MSEC);
  switch (sd_trace_path_state)
  {
    default:
    case 0:
      dirobj = dirobj_new;
      fn = fn_new;
      path = path_new;
      dir = dir_new;
      dptr = NULL;
      sd_trace_path_state = 1;
    case 1:
      clust = fs.dirbase;
      if (fs.fs_type == FS_FAT32)
      {
        dirobj->clust = dirobj->sclust = clust;
        dirobj->sect = SD_Clust_To_Sect(clust);
      }
      else
      {
        dirobj->clust = dirobj->sclust = 0;
        dirobj->sect = clust;
      }  
      dirobj->index = 0;
      dirobj->fs = &fs;
      if (*path == '\0')
      { // Null path means the root directory 
        *dir = NULL; 
        sd_trace_path_state = 0;
        dirobj_new = dirobj;
        fn_new = fn;
        dir_new = dir;
        return (SD_ROOT);
      }
      sd_trace_path_state = 2;
sd_trace_path_label_2:
    case 2: // for(;;)
      ds = SD_Make_Dirfile(&path, fn); // Get a paragraph into fn[]
      if (ds == 1)
      {
        sd_trace_path_state = 0;
        dirobj_new = dirobj;
        fn_new = fn;
        dir_new = dir;
        return (SD_NO_VALID_NAME);
      }  
      sd_trace_path_state = 3;
      if (Timer_Expired(&timer_trace_path))
        return (SD_BEZIG);
sd_trace_path_label_3:
    case 3: // for(;;)
      switch (SD_Move_Window(dirobj->sect))
      {
        case SD_OK:
          break;
        case SD_ERROR:
          sd_trace_path_state = 0;
          dirobj_new = dirobj;
          fn_new = fn;
          dir_new = dir;
          return (SD_ERROR);
        case SD_BEZIG:
          return (SD_BEZIG);
      }
      dptr = &fs.win[(dirobj->index & 15) * 32]; // Pointer to the directory entry
      if (dptr[DIR_Name] == 0) // Has it reached to end of dir?
      {
        sd_trace_path_state = 0;
        dirobj_new = dirobj;
        fn_new = fn;
        dir_new = dir;
        return (!ds ? SD_NO_FILE : SD_NO_PATH);
      }  
      if (dptr[DIR_Name] != 0xE5 && // Matched?
          !(dptr[DIR_Attr] & AM_VOL) &&
          !memcmp(&dptr[DIR_Name], fn, 8+3))
      {    
        sd_trace_path_state = 10;
        goto sd_trace_path_label_10;
      }  
      sd_trace_path_state = 4; 
      if (Timer_Expired(&timer_trace_path))
        return (SD_BEZIG);
    case 4:  
      switch (SD_Next_Dir_Entry(dirobj))
      {
        case SD_OK:  
          break;
        default:   
        case SD_ERROR:
          sd_trace_path_state = 0;
          dirobj_new = dirobj;
          fn_new = fn;
          dir_new = dir;
          return (SD_ERROR);
        case SD_DIR_NOT_FOUND:  
          sd_trace_path_state = 0;
          dirobj_new = dirobj;
          fn_new = fn;
          dir_new = dir;
          return (!ds ? SD_NO_FILE : SD_NO_PATH);
        case SD_BEZIG:
          return (SD_BEZIG);  
      }
      sd_trace_path_state = 3;
      if (Timer_Expired(&timer_trace_path))
        return (SD_BEZIG);
      goto sd_trace_path_label_3;
sd_trace_path_label_10:
    case 10:  
      if (!ds)
      {
        *dir = dptr; 
        sd_trace_path_state = 0;
        dirobj_new = dirobj;
        fn_new = fn;
        dir_new = dir;
        return (SD_OK);
      } // Matched with end of path
      if (!(dptr[DIR_Attr] & AM_DIR))
      {
        sd_trace_path_state = 0;
        dirobj_new = dirobj;
        fn_new = fn;
        dir_new = dir;
        return (SD_NO_PATH); // Cannot trace because it is a file
      }  
      clust = ((unsigned long)LD_WORD(&dptr[DIR_FstClusHI]) << 16) | LD_WORD(&dptr[DIR_FstClusLO]);
      dirobj->clust = dirobj->sclust = clust; // Restart scannig with the new directory
      dirobj->sect = SD_Clust_To_Sect(clust);
      dirobj->index = 2;
      sd_trace_path_state = 2;
      if (Timer_Expired(&timer_trace_path))
        return (SD_BEZIG);
      goto sd_trace_path_label_2;  
  }
}
                            
//-----------------------------------------------------------------------------
static void SD_Get_Fileinfo(FILINFO *finfo,  // Ptr to store the File Information
                            const unsigned char *dir) // Ptr to the directory entry
{
unsigned char n, c, a;
char *p;

  p = &finfo->fname[0];
  a = _USE_NTFLAG ? dir[DIR_NTres] : 0; // NT flag
  for (n = 0; n < 8; n++)
  { // Convert file name (body)
    c = dir[n];
    if (c == ' ')
      break;
    if (c == 0x05)
      c = 0xE5;
    if (a & 0x08 && c >= 'A' && c <= 'Z')
      c += 0x20;
    *p++ = c;
  }
  if (dir[8] != ' ')
  { // Convert file name (extension)
    *p++ = '.';
    for (n = 8; n < 11; n++)
    {
      c = dir[n];
      if (c == ' ')
        break;
      if (a & 0x10 && c >= 'A' && c <= 'Z')
        c += 0x20;
      *p++ = c;
    }
  }
  *p = '\0';

  finfo->fattrib = dir[DIR_Attr];              // Attribute
  finfo->fsize = LD_DWORD(&dir[DIR_FileSize]); // Size
  finfo->fdate = LD_WORD(&dir[DIR_WrtDate]);   // Date
  finfo->ftime = LD_WORD(&dir[DIR_WrtTime]);   // Time
}
                            
//-----------------------------------------------------------------------------
static unsigned char sd_f_stat_state = 0;
static unsigned char SD_F_Stat(const char *path_new, // Pointer to the file path
                               FILINFO *finfo)       // Pointer to file information to return
// return SD_OK, SD_ERROR, SD_NO_VALID_NAME, SD_NO_PATH, SD_NO_FILE, SD_ROOT of SD_BEZIG
{
static const char *path;
static unsigned char *dir;
static char fn[8+3+1];
static DIR dirobj;

  switch (sd_f_stat_state)
  { 
    default:
    case 0:
      path = path_new;
      sd_f_stat_state = 1;
    case 1:  
      SD_Auto_Mount(&path, 0);
      sd_f_stat_state = 2;
    case 2:
      switch (SD_Trace_Path(&dirobj, fn, path, &dir)) // Trace the file path
      {
        case SD_OK: // Trace completed
          sd_f_stat_state = 0;
          path_new = path;
          SD_Get_Fileinfo(finfo, dir);
          return (SD_OK);
        default:
        case SD_ERROR:
          sd_f_stat_state = 0;
          path_new = path;
          return (SD_ERROR);
        case SD_NO_VALID_NAME:
          sd_f_stat_state = 0;
          path_new = path;
          return (SD_NO_VALID_NAME);
        case SD_NO_PATH:
          sd_f_stat_state = 0;
          path_new = path;
          return (SD_NO_PATH);
        case SD_NO_FILE:
          sd_f_stat_state = 0;
          path_new = path;
          return (SD_NO_FILE);
        case SD_ROOT:
          sd_f_stat_state = 0;
          path_new = path;
          return (SD_ROOT);
        case SD_BEZIG:
          return (SD_BEZIG);
      }
  }
}                        

//-----------------------------------------------------------------------------
static unsigned char sd_create_chain_state = 0;
static unsigned char SD_Create_Chain(unsigned long clust, unsigned long *clust_new) // Cluster# to stretch, 0 means create new
// return SD_OK, SD_ERROR, SD_BEZIG
{
static unsigned long ncl;
static unsigned long scl;
unsigned long cstat;
s_timer timer_create_chain;

  Timer_Set(&timer_create_chain, 5, TIME_BASE_1_MSEC);
  switch (sd_create_chain_state)
  {
    default:
    case 0:  
      sd_create_chain_state = 1;
    case 1:
      if (clust == 0)
      { // Create new chain
        scl = fs.last_clust; // Get last allocated cluster
        if ((scl < 2) ||
            (scl >= fs.max_clust))
          scl = 1;
      }
      else
      { // Stretch existing chain
        switch (SD_Get_Cluster(clust, &cstat))
        {
          case SD_OK:
            break;
          case SD_ERROR:
            *clust_new = 1;
            sd_create_chain_state = 0;
            return (SD_ERROR);
          case SD_BEZIG:
            return (SD_BEZIG);
        }
        if (cstat < 2) 
        {
          *clust_new = 1;
          sd_create_chain_state = 0;
          return (SD_OK); // It is an invalid cluster
        }  
        if (cstat < fs.max_clust)
        {
          *clust_new = cstat;
          sd_create_chain_state = 0;
          return (SD_OK); // It is already followed by next cluster
        }  
        scl = clust;
      }
      ncl = scl; // Start cluster
      sd_create_chain_state = 2;
      if (Timer_Expired(&timer_create_chain))
        return (SD_BEZIG);
sd_create_chain_label_2:
    case 2:
      ncl++; // Next cluster
      if (ncl >= fs.max_clust)
      { // Wrap around
        ncl = 2;
        if (ncl > scl)
        {
          *clust_new = 0;
          sd_create_chain_state = 0;
          return (SD_OK); // No free custer
        }  
      }
      sd_create_chain_state = 3;
    case 3:  
      switch (SD_Get_Cluster(ncl, &cstat))
      {
        case SD_OK:
          break;
        case SD_ERROR:
          *clust_new = 1;
          sd_create_chain_state = 0;
          return (SD_ERROR);
        case SD_BEZIG:
          return (SD_BEZIG);
      }
      if (cstat == 0)
      { // Found a free cluster
        sd_create_chain_state = 4;
        if (Timer_Expired(&timer_create_chain))
          return (SD_BEZIG);
        else
          goto sd_create_chain_label_4;
      }  
      if (cstat == 1)
      {
        *clust_new = 1;
        sd_create_chain_state = 0;
        return (SD_OK); // Any error occured
      }  
      if (ncl == scl)
      {
        *clust_new = 0;
        sd_create_chain_state = 0;
        return (SD_OK); // No free custer
      }
      sd_create_chain_state = 2;  
      if (Timer_Expired(&timer_create_chain))
        return (SD_BEZIG);
      else
        goto sd_create_chain_label_2;  
sd_create_chain_label_4:
    case 4:
      switch (SD_Put_Cluster(ncl, (unsigned long)0x0FFFFFFF))
      {
        case SD_OK:
          break;
        case SD_ERROR:
          *clust_new = 1;
          sd_create_chain_state = 0;
          return (SD_ERROR);
        case SD_BEZIG:
          return (SD_BEZIG);
      }
      sd_create_chain_state = 5;
      if (Timer_Expired(&timer_create_chain))
        return (SD_BEZIG);
    case 5:  
      if (clust)
      {
        switch (SD_Put_Cluster(clust, ncl))
        {
          case SD_OK:
            break;
          case SD_ERROR:
            *clust_new = 1;
            sd_create_chain_state = 0;
            return (SD_ERROR);
          case SD_BEZIG:
            return (SD_BEZIG);
        }
      }
      fs.last_clust = ncl;
      if (fs.free_clust != (unsigned long)0xFFFFFFFF)
      {
        fs.free_clust--;
        fs.fsi_flag = 1;
      }
      *clust_new = ncl;
      sd_create_chain_state = 0;
      return (SD_OK); // Return new cluster number
  }
}

//-----------------------------------------------------------------------------
static unsigned char sd_reserve_direntry_state = 0;
static unsigned char SD_Reserve_Direntry(DIR *dirobj_new, // Target directory to create new entry
                                         unsigned char **dir_new)  // Pointer to pointer to created entry to retutn
// return SD_OK, SD_ERROR, SD_DIR_NOT_FOUND, SD_CLUST_END of SD_BEZIG
{
static DIR *dirobj;
static unsigned char **dir;
static unsigned char n;
static unsigned long clust;
static unsigned long sector;
unsigned char c;
unsigned char *dptr;
int cnt = 0;
s_timer timer_reserve_direntry;

  Timer_Set(&timer_reserve_direntry, 5, TIME_BASE_1_MSEC);
  switch (sd_reserve_direntry_state)
  {
    default:
    case 0:
      dirobj = dirobj_new;
      dir = dir_new;
      sd_reserve_direntry_state = 1;
    case 1:
      // Re-initialize directory object
      clust = dirobj->sclust;
      if (clust)
      { // Dyanmic directory table
        dirobj->clust = clust;
        dirobj->sect = SD_Clust_To_Sect(clust);
      }
      else
      { // Static directory table
        dirobj->sect = fs.dirbase;
      }
      dirobj->index = 0;
      sd_reserve_direntry_state = 2;
sd_reserve_direntry_label_2:
    case 2: // while (1)
      switch (SD_Move_Window(dirobj->sect))
      {
        case SD_OK:
          break;
        case SD_ERROR:
          sd_reserve_direntry_state = 0;
          dirobj_new = dirobj;
          dir_new = dir;
          return (SD_ERROR);
        case SD_BEZIG:
          return (SD_BEZIG);
      }
      dptr = &fs.win[(dirobj->index & 15) * 32]; // Pointer to the directory entry
      c = dptr[DIR_Name];
      if (c == 0 || c == 0xE5)
      { // Found an empty entry!
        *dir = dptr;
        sd_reserve_direntry_state = 0;
        dirobj_new = dirobj;
        dir_new = dir;
        return (SD_OK);
      }
      sd_reserve_direntry_state = 3;
      if (Timer_Expired(&timer_reserve_direntry))
        return (SD_BEZIG);
    case 3:
      switch (SD_Next_Dir_Entry(dirobj))
      {
        case SD_OK:  
          break;
        default:   
        case SD_ERROR:
          sd_reserve_direntry_state = 0;
          dirobj_new = dirobj;
          dir_new = dir;
          return (SD_ERROR);
        case SD_DIR_NOT_FOUND:  
          goto sd_reserve_direntry_label_8;
        case SD_BEZIG:
          return (SD_BEZIG);  
      }
      if (Timer_Expired(&timer_reserve_direntry))
        return (SD_BEZIG);
      goto sd_reserve_direntry_label_2;
      // Reached to end of the directory table
sd_reserve_direntry_label_8:
    case 8:
      if (!clust)
      {
        sd_reserve_direntry_state = 0;
        dirobj_new = dirobj;
        dir_new = dir;
        return (SD_CLUST_END);
      }  
      sd_reserve_direntry_state = 9;
      if (Timer_Expired(&timer_reserve_direntry))
        return (SD_BEZIG);
    case 9:  
      switch (SD_Create_Chain(dirobj->clust, &clust))
      {
        case SD_OK: 
          break;
        case SD_ERROR:
          sd_reserve_direntry_state = 0;
          dirobj_new = dirobj;
          dir_new = dir;
          return (SD_ERROR);
        case SD_BEZIG:
          return (SD_BEZIG);
      }
      if (!clust)
      {
        sd_reserve_direntry_state = 0;
        dirobj_new = dirobj;
        dir_new = dir;
        return (SD_CLUST_END);
      }  
      if (clust == 1)
      {
        sd_reserve_direntry_state = 0;
        dirobj_new = dirobj;
        dir_new = dir;
        return (SD_CLUST_END);
      }
      fs.winsect = sector = SD_Clust_To_Sect(clust); // Cleanup the expanded table
      memset(fs.win, 0, 512);
      n = fs.sects_clust;
      sd_reserve_direntry_state = 10;
      if (Timer_Expired(&timer_reserve_direntry))
        return (SD_BEZIG);
sd_reserve_direntry_label_10:
    case 10:
      // Abort when static table or could not stretch dynamic table
//      for (cnt = 0; cnt < CNT_MAX_RESERVE_DIRENTRY; cnt++)
      switch (SD_Write_Sector(fs.win, sector))
      {
        case SD_OK:
          break;
        case SD_ERROR:
          sd_reserve_direntry_state = 0;
          dirobj_new = dirobj;
          dir_new = dir;
          return (SD_ERROR);
        case SD_BEZIG:
          return (SD_BEZIG);
      }
      sector++;
      if (n)
      {
        n--;
        if (Timer_Expired(&timer_reserve_direntry))
          return(SD_BEZIG);
        goto sd_reserve_direntry_label_10;  
      }  
      fs.winflag = 1;
      *dir = fs.win;
      sd_reserve_direntry_state = 0;
      dirobj_new = dirobj;
      dir_new = dir;
      return (SD_OK);
  }
}

//-----------------------------------------------------------------------------
static unsigned char sd_remove_chain_state = 0;
static unsigned char SD_Remove_Chain(unsigned long clust_new) // Cluster# to remove chain from
// return SD_OK, SD_ERROR of SD_BEZIG
{
static unsigned long clust;
static unsigned long nxt;
int cnt = 0;
s_timer timer_remove_chain;

  Timer_Set(&timer_remove_chain, 5, TIME_BASE_1_MSEC);
  switch (sd_remove_chain_state)
  {
    default:
    case 0:
      clust = clust_new;
      sd_remove_chain_state = 1;
sd_remove_chain_label_1:      
    case 1:
      if ((clust >= 2) && 
          (clust < fs.max_clust))
      {
        sd_remove_chain_state = 2;
        goto sd_remove_chain_label_2;
      }
      sd_remove_chain_state = 0;
      return (SD_OK);
sd_remove_chain_label_2:
    case 2:
      switch (SD_Get_Cluster(clust, &nxt))
      {
        case SD_OK:
          break;
        case SD_ERROR:
          sd_remove_chain_state = 0;
          return (SD_ERROR);
        case SD_BEZIG:
          return (SD_BEZIG);
      }
      sd_remove_chain_state = 3;
      if (Timer_Expired(&timer_remove_chain))
        return (SD_BEZIG);
    case 3:
      switch (SD_Put_Cluster(clust, 0))
      {
        case SD_OK:
          break;
        case SD_ERROR:
          sd_remove_chain_state = 0;
          return (SD_ERROR);
        case SD_BEZIG:
          return (SD_BEZIG);
      }
      if (fs.free_clust != (unsigned long)0xFFFFFFFF)
      {
        fs.free_clust++;
        fs.fsi_flag = 1;
      }
      clust = nxt;
      sd_remove_chain_state = 1;
      if (Timer_Expired(&timer_remove_chain))
        return (SD_BEZIG);
      goto sd_remove_chain_label_1;
  }
}

//-----------------------------------------------------------------------------
static unsigned char sd_f_open_state = 0;
static unsigned char SD_F_Open(FIL *fp_new,      // Pointer to the blank file object
                               const char *path_new, // Pointer to the file name
                               unsigned char mode)        // Access mode and file open mode flags
// return SD_OK, SD_ERROR, SD_ROOT, SD_NO_VALID_NAME, SD_NO_FILE, SD_NO_PATH, SD_DIR_NOT_FOUND, SD_CLUST_END, SD_EXIST, SD_WRITE_PROTECT of SD_BEZIG
{
static const char *path;
static FIL *fp;
unsigned char res;
static unsigned char *dir;
static DIR dirobj;
static char fn[8+3+1];
static unsigned long rs;
static unsigned long dw;
s_timer timer_f_open;

  Timer_Set(&timer_f_open, 5, TIME_BASE_1_MSEC);
  switch (sd_f_open_state)
  {
    default:
    case 0:
      fp = fp_new;
      path = path_new;
      sd_f_open_state = 1;
    case 1:
      fp->fs = NULL;
      mode &= (FA_READ|FA_WRITE|FA_CREATE_ALWAYS|FA_OPEN_ALWAYS|FA_CREATE_NEW);
      if (SD_Auto_Mount(&path, (unsigned char)(mode & (FA_WRITE|FA_CREATE_ALWAYS|FA_OPEN_ALWAYS|FA_CREATE_NEW))) != SD_OK)
      {
        sd_f_open_state = 0;
        fp_new = fp;
        path_new = path;
        return (SD_ERROR);
      }  
      sd_f_open_state = 2;
    case 2:
      // Trace the file path
      res = SD_Trace_Path(&dirobj, fn, path, &dir); // Trace the file path
      switch (res) // Trace the file path
      {
        case SD_OK: // Trace completed
          break;
        case SD_ERROR:
          sd_f_open_state = 0;
          path_new = path;
          fp_new = fp;
          return (SD_ERROR);
        case SD_NO_VALID_NAME:
          break;  
        case SD_NO_FILE:
          break;
        case SD_NO_PATH:
          break;
        case SD_ROOT:
          res = SD_OK;
          break;
        case SD_BEZIG:
          return (SD_BEZIG);
      }
      // Create or Open a File
      if (mode & (FA_CREATE_ALWAYS|FA_OPEN_ALWAYS|FA_CREATE_NEW))
      {
        if (res != SD_OK)
        { // No file, create new
          if (res != SD_NO_FILE)
          {
            sd_f_open_state = 0;
            path_new = path;
            fp_new = fp;
            return (res);
          }  
          sd_f_open_state = 3;
          if (Timer_Expired(&timer_f_open))
            return (SD_BEZIG);
          goto sd_f_open_label_3;
        }
        else
        { // Any object is already existing
          if (mode & FA_CREATE_NEW) // Cannot create new
          {
            sd_f_open_state = 0;
            path_new = path;
            fp_new = fp;
            return (SD_EXIST);
          }  
          if (dir == NULL || (dir[DIR_Attr] & (AM_RDO|AM_DIR))) // Cannot overwrite (R/O or DIR)
          {
            sd_f_open_state = 0;
            path_new = path;
            fp_new = fp;
            return (SD_WRITE_PROTECT);
          }  
          if (mode & FA_CREATE_ALWAYS)
          { // Resize it to zero
            rs = ((unsigned long)LD_WORD(&dir[DIR_FstClusHI]) << 16) | LD_WORD(&dir[DIR_FstClusLO]);
            ST_WORD(&dir[DIR_FstClusHI], 0);
            ST_WORD(&dir[DIR_FstClusLO], 0); // cluster = 0
            ST_DWORD(&dir[DIR_FileSize], 0); // size = 0
            fs.winflag = 1;
            dw = fs.winsect;                // Remove the cluster chain
            sd_f_open_state = 4;
            if (Timer_Expired(&timer_f_open))
              return (SD_BEZIG);
            goto sd_f_open_label_4;
          }
          else
          {
            sd_f_open_state = 6;
            if (Timer_Expired(&timer_f_open))
              return (SD_BEZIG);
            goto sd_f_open_label_6;
          }  
        }
      }  
      else
      {
        if (res != SD_OK)
        {
          sd_f_open_state = 0;
          path_new = path;
          fp_new = fp;
          return (res); // Trace failed
        }  
        if (dir == NULL || (dir[DIR_Attr] & AM_DIR)) // It is a directory
        {
          sd_f_open_state = 0;
          path_new = path;
          fp_new = fp;
          return (SD_NO_FILE);
        }
        if ((mode & FA_WRITE) && (dir[DIR_Attr] & AM_RDO)) // R/O violation
        {
          sd_f_open_state = 0;
          path_new = path;
          fp_new = fp;
          return (SD_WRITE_PROTECT);
        }  
      }
      sd_f_open_state = 7; 
      if (Timer_Expired(&timer_f_open))
        return (SD_BEZIG);
      goto sd_f_open_label_7;
sd_f_open_label_3:
    case 3:  
      switch (SD_Reserve_Direntry(&dirobj, &dir))
      {
        case SD_OK:
          break;
        default:
        case SD_ERROR:
          sd_f_open_state = 0;
          path_new = path;
          fp_new = fp;
          return (SD_ERROR);  
        case SD_DIR_NOT_FOUND:
          sd_f_open_state = 0;
          path_new = path;
          fp_new = fp;
          return (SD_DIR_NOT_FOUND);
        case SD_CLUST_END:
          sd_f_open_state = 0;
          path_new = path;
          fp_new = fp;
          return (SD_CLUST_END);
        case SD_BEZIG:
          return (SD_BEZIG);
      }    
      memset(dir, 0, 32); // Initialize the new entry
      memcpy(&dir[DIR_Name], fn, 8+3);
      dir[DIR_NTres] = fn[11];
      mode |= FA_CREATE_ALWAYS;
      goto sd_f_open_label_6;
sd_f_open_label_4:
    case 4:
      switch (SD_Remove_Chain(rs))
      {
        case SD_OK:
          break;
        case SD_ERROR:
          sd_f_open_state = 0;
          path_new = path;
          fp_new = fp;
          return (SD_ERROR);
        case SD_BEZIG:
          return (SD_BEZIG);
      }
      sd_f_open_state = 5;
    case 5:  
      switch (SD_Move_Window(dw))
      {
        case SD_OK:
          break;
        case SD_ERROR:
          sd_f_open_state = 0;
          path_new = path;
          fp_new = fp;
          return (SD_ERROR);
        case SD_BEZIG:
          return (SD_BEZIG);
      }
      fs.last_clust = rs - 1;         // Reuse the cluster hole
      sd_f_open_state = 6;
sd_f_open_label_6:
    case 6:  
      if (mode & FA_CREATE_ALWAYS)
      {
        dir[DIR_Attr] = AM_ARC;          // New attribute
        dw = SD_Get_Fattime();
        ST_DWORD(&dir[DIR_WrtTime], dw); // Updated time
        ST_DWORD(&dir[DIR_CrtTime], dw); // Created time
        fs.winflag = 1;
      }  
      sd_f_open_state = 7;
sd_f_open_label_7: // OK
    case 7:
      fp->dir_sect = fs.winsect; // Pointer to the directory entry
      fp->dir_ptr = dir;
      fp->flag = mode;                              // File access mode
      fp->org_clust = ((unsigned long)LD_WORD(&dir[DIR_FstClusHI]) << 16) | LD_WORD(&dir[DIR_FstClusLO]);
      fp->fsize = LD_DWORD(&dir[DIR_FileSize]);     // File size
      fp->fptr = 0;                                 // File ptr
      fp->sect_clust = 1;                           // Sector counter
      fp->fs = &fs; fp->id = fs.id;                 // Owner file system object of the file
      sd_f_open_state = 0;
      path_new = path;
      fp_new = fp;
      return (SD_OK);
  }
}

//-----------------------------------------------------------------------------
static unsigned char SD_Validate(const FATFS *fs, // Pointer to the file system object
                                 unsigned int id) // id member of the target object to be checked
// return SD_OK of SD_INVALID_OBJECT
{
  if (!fs || fs->id != id)
    return SD_INVALID_OBJECT;
  return (SD_OK);
}

//-----------------------------------------------------------------------------
static unsigned char sd_f_lseek_state = 0;
static unsigned char SD_F_Lseek(FIL *fp,   // Pointer to the file object
                                unsigned long ofs_new) // File pointer from top of file
// return SD_OK, SD_ERROR, SD_INVALID_OBJECT of SD_BEZIG
{
static unsigned long ofs;
static unsigned long clust;
static unsigned long csize;
unsigned char csect;
FATFS *fs = fp->fs;
s_timer timer_f_lseek;

  Timer_Set(&timer_f_lseek, 5, TIME_BASE_1_MSEC);
  switch (sd_f_lseek_state)
  {
    default:
    case 0:
      ofs = ofs_new;
      sd_f_lseek_state = 1;
    case 1:  
      switch (SD_Validate(fs, fp->id)) // Check validity of the object
      {
        case SD_OK:
          break;
        case SD_INVALID_OBJECT:
          sd_f_lseek_state = 0;
          return (SD_ERROR);
      }
      if (fp->flag & FA__ERROR)
      {
        fp->flag |= FA__ERROR;
        sd_f_lseek_state = 0;
        return (SD_ERROR);
      }  
      if (ofs > fp->fsize && !(fp->flag & FA_WRITE))
        ofs = fp->fsize;
      fp->fptr = 0; fp->sect_clust = 1; // Set file R/W pointer to top of the file
      if (ofs == 0)
      {
        sd_f_lseek_state = 9;
        goto sd_f_lseek_label_9;
      }
      clust = fp->org_clust;
      if (clust)
      {
        sd_f_lseek_state = 3;
        goto sd_f_lseek_label_3;
      }  
      sd_f_lseek_state = 2;
    case 2: // !clust
      switch (SD_Create_Chain(0, &clust))
      {
        case SD_OK: 
          break;
        case SD_ERROR:
          fp->flag |= FA__ERROR;
          sd_f_lseek_state = 0;
          return (SD_ERROR);
        case SD_BEZIG:
          return (SD_BEZIG);
      }
      if (clust == 1)
      {
        fp->flag |= FA__ERROR;
        sd_f_lseek_state = 0;
        return (SD_ERROR);
      }  
      fp->org_clust = clust;
      if (!clust)
      {
        sd_f_lseek_state = 9;
        goto sd_f_lseek_label_9;
      }
      sd_f_lseek_state = 3;
sd_f_lseek_label_3: // clust exist
    case 3:
      csize = (unsigned long)fs->sects_clust * 512; // Cluster size in unit of byte
      sd_f_lseek_state = 4;
      if (Timer_Expired(&timer_f_lseek))
        return (SD_BEZIG);
sd_f_lseek_label_4:
    case 4: // for (;;)  
      fp->curr_clust = clust;                 // Update current cluster
      if (ofs <= csize)
      {
        sd_f_lseek_state = 8;
        goto sd_f_lseek_label_8;
      }  
      if (fp->flag & FA_WRITE) // Check if in write mode or not
      {
        sd_f_lseek_state = 6;
        goto sd_f_lseek_label_6;
      }  
      sd_f_lseek_state = 5;  
    case 5:
      switch (SD_Get_Cluster(clust, &clust))
      {
        case SD_OK:
          break;
        case SD_ERROR:
          fp->flag |= FA__ERROR;
          sd_f_lseek_state = 0;
          return (SD_ERROR);
        case SD_BEZIG:
          return (SD_BEZIG);
      }
      sd_f_lseek_state = 7;
      if (Timer_Expired(&timer_f_lseek))
        return (SD_BEZIG);
      goto sd_f_lseek_label_7;
sd_f_lseek_label_6:
    case 6:
      switch (SD_Create_Chain(clust, &clust))
      {
        case SD_OK: 
          break;
        case SD_ERROR:
          fp->flag |= FA__ERROR;
          sd_f_lseek_state = 0;
          return (SD_ERROR);
        case SD_BEZIG:
          return (SD_BEZIG);
      }
      sd_f_lseek_state = 7;
      if (Timer_Expired(&timer_f_lseek))
        return (SD_BEZIG);
sd_f_lseek_label_7:
    case 7:
      if (clust == 0)
      { // Stop if could not follow the cluster chain
        ofs = csize;
        sd_f_lseek_state = 8;
        goto sd_f_lseek_label_8;
      }
      if (clust == 1 || clust >= fs->max_clust)
      {
        fp->flag |= FA__ERROR;
        sd_f_lseek_state = 0;
        return (SD_ERROR);
      }  
      fp->fptr += csize;
      ofs -= csize;
      sd_f_lseek_state = 4;
      goto sd_f_lseek_label_4;
sd_f_lseek_label_8:
    case 8:
      csect = (unsigned char)((ofs - 1) / 512); // Sector offset in the cluster
      fp->curr_sect = SD_Clust_To_Sect(clust) + csect; // Get current cluster
      fp->sect_clust = fs->sects_clust - csect;  // Left sector counter in the cluster
      fp->fptr += ofs;                           // Update file R/W pointer
      sd_f_lseek_state = 9;
sd_f_lseek_label_9:
    case 9:
      if ((fp->flag & FA_WRITE) && fp->fptr > fp->fsize)
      { // Set updated flag if in write mode
        fp->fsize = fp->fptr;
        fp->flag |= FA__WRITTEN;
      }
      sd_f_lseek_state = 0;
      return (SD_OK);
  }
}

//-----------------------------------------------------------------------------
static unsigned char sd_f_write_state = 0;
static unsigned char SD_F_Write(FIL *fp,              // Pointer to the file object
                                const void *buff_new, // Pointer to the data to be written
                                unsigned int btw_new, // Number of bytes to write
                                unsigned int *bw_new) // Pointer to number of bytes written
// return SD_OK, SD_ERROR, SD_INVALID_OBJECT of SD_BEZIG
{
static unsigned char *buff;
static unsigned int btw;
static unsigned int bw;
static unsigned int cc;
static unsigned long clust; // JP 15-04-09
static unsigned long sect;
unsigned int wcnt;
FATFS *fs = fp->fs;
s_timer timer_f_write;

  Timer_Set(&timer_f_write, 5, TIME_BASE_1_MSEC);
  switch (sd_f_write_state)
  {
    default:
    case 0:
      buff = buff_new;
      btw = btw_new;
      bw = 0;
      sd_f_write_state = 1;  
      switch (SD_Validate(fs, fp->id)) // Check validity of the object
      {
        case SD_OK:
          break;
        case SD_INVALID_OBJECT:
          *bw_new = bw;
          sd_f_write_state = 0;
          return (SD_INVALID_OBJECT);
      }
      if (fp->flag & FA__ERROR)
      {
        *bw_new = bw;
        sd_f_write_state = 0;
        return (SD_ERROR); // Check error flag
      }  
      if (!(fp->flag & FA_WRITE))
      {
        *bw_new = bw;
        sd_f_write_state = 0;
        return (SD_WRITE_PROTECT); // Check access mode
      }  
      if (fp->fsize + btw < fp->fsize)
      {
        *bw_new = bw;
        sd_f_write_state = 0;
        return (SD_OK); // File size cannot reach 4GB
      }  
sd_f_write_label_1:
    case 1:
      if (btw)
      {
        if ((fp->fptr % 512) == 0)
        { // On the sector boundary 
          if (--(fp->sect_clust))
          { // Decrement left sector counter
            sect = fp->curr_sect + 1; // Get current sector
          }
          else
          { // On the cluster boundary, get next cluster
            if (fp->fptr == 0)
            { // Is top of the file
              clust = fp->org_clust;
              if (clust == 0) // No cluster is created yet
              {
                sd_f_write_state = 2;
                goto sd_f_write_label_2;
              }  
            }
            else
            { // Middle or end of file
              sd_f_write_state = 3;
              goto sd_f_write_label_3;
            }
            sd_f_write_state = 4;
            goto sd_f_write_label_4;
          }
          sd_f_write_state = 5;
          goto sd_f_write_label_5;
        }
        sd_f_write_state = 8;
        if (Timer_Expired(&timer_f_write))
        {
          *bw_new = bw;
          return (SD_BEZIG);
        }
        goto sd_f_write_label_8;  
      }
      else
      {
        if (fp->fptr > fp->fsize)
          fp->fsize = fp->fptr; // Update file size if needed
        fp->flag |= FA__WRITTEN; // Set file changed flag
        sd_f_write_state = 0;
        *bw_new = bw;
        return (SD_OK);
      }
sd_f_write_label_2:
    case 2: // No cluster is created yet
      switch (SD_Create_Chain(0, &clust))
      {
        case SD_OK: 
          break;
        case SD_ERROR:
          sd_f_write_state = 0;  
          *bw_new = bw;
          return (SD_ERROR);
        case SD_BEZIG:
          *bw_new = bw;
          return (SD_BEZIG);
      }
      fp->org_clust = clust; // Create a new cluster chain
      sd_f_write_state = 4;
      *bw_new = bw;
      if (Timer_Expired(&timer_f_write))
        return (SD_BEZIG);
      goto sd_f_write_label_4;    
sd_f_write_label_3:
    case 3: // Middle or end of file
      switch (SD_Create_Chain(fp->curr_clust, &clust))
      {
        case SD_OK: 
          break;
        case SD_ERROR:
          sd_f_write_state = 0;  
          *bw_new = bw;
          return (SD_ERROR);
        case SD_BEZIG:
          *bw_new = bw;
          return (SD_BEZIG);
      }
      sd_f_write_state = 4;
      *bw_new = bw;
      if (Timer_Expired(&timer_f_write))
        return (SD_BEZIG);
sd_f_write_label_4:
    case 4:
      if (clust == 0)
      {
        if (fp->fptr > fp->fsize)
          fp->fsize = fp->fptr; // Update file size if needed
        fp->flag |= FA__WRITTEN; // Set file changed flag
        sd_f_write_state = 0;
        *bw_new = bw;
        return (SD_OK);
        //  break; // Disk full
      }
      if (clust == 1 || clust >= fs->max_clust)
      {
        fp->flag |= FA__ERROR;
        sd_f_write_state = 0;
        *bw_new = bw;
        return (SD_ERROR);
        // goto fw_error;
      }  
      fp->curr_clust = clust;           // Current cluster
      sect = SD_Clust_To_Sect(clust);         // Get current sector
      fp->sect_clust = fs->sects_clust; // Re-initialize the left sector counter
      sd_f_write_state = 5;
      if (Timer_Expired(&timer_f_write))
      {
        *bw_new = bw;
        return (SD_BEZIG);
      }  
sd_f_write_label_5:
    case 5:
      fp->curr_sect = sect; // Update current sector
      cc = btw / 512;       // When left bytes >= 512,
      if (cc != 0)
      { // Write maximum contiguous sectors directly
        if (cc > fp->sect_clust)
          cc = fp->sect_clust;
        sd_f_write_state = 6;
        goto sd_f_write_label_6;    
      }
      if (fp->fptr >= fp->fsize)
      { // Flush R/W window if needed
        sd_f_write_state = 7;
        goto sd_f_write_label_7;    
      }
      sd_f_write_state = 8;
      *bw_new = bw;
      if (Timer_Expired(&timer_f_write))
        return (SD_BEZIG);
      goto sd_f_write_label_8;  
sd_f_write_label_6:
    case 6:
      switch (SD_Write_Sector(buff, sect))
      {
        case SD_OK:
          break;
        case SD_ERROR:
          fp->flag |= FA__ERROR;
          sd_f_write_state = 0;
          *bw_new = bw;
          return (SD_ERROR);
        case SD_BEZIG:
          *bw_new = bw;
          return (SD_BEZIG);
      }
      fp->sect_clust -= (unsigned char)(cc - 1);
      fp->curr_sect += cc - 1;
      wcnt = cc * 512;
      //****
      buff += wcnt;
      fp->fptr += wcnt;
      btw -= wcnt; 
      bw += wcnt;
      sd_f_write_state = 1;  
      *bw_new = bw;
      if (Timer_Expired(&timer_f_write))
        return (SD_BEZIG);
      goto sd_f_write_label_1;  
sd_f_write_label_7:
    case 7:  
      switch (SD_Move_Window(0))
      {
        case SD_OK:
          break;
        case SD_ERROR:
          fp->flag |= FA__ERROR;
          sd_f_write_state = 0;
          *bw_new = bw;
          return (SD_ERROR);
        case SD_BEZIG:
          *bw_new = bw;
          return (SD_BEZIG);
      }
      fs->winsect = fp->curr_sect;
      sd_f_write_state = 8;
      *bw_new = bw;
      if (Timer_Expired(&timer_f_write))
        return (SD_BEZIG);
sd_f_write_label_8:
    case 8:
      switch (SD_Move_Window(fp->curr_sect))
      {
        case SD_OK:
          break;
        case SD_ERROR:
          fp->flag |= FA__ERROR;
          sd_f_write_state = 0;
          *bw_new = bw;
          return (SD_ERROR);
        case SD_BEZIG:
          *bw_new = bw;
          return (SD_BEZIG);
      }
      wcnt = 512 - (unsigned int)(fp->fptr % 512); // Copy fractional bytes bytes to sector window
      if (wcnt > btw)
        wcnt = btw;
      memcpy(&fs->win[(unsigned int)fp->fptr % 512], buff, wcnt);
      fs->winflag = 1;
      buff += wcnt;
      fp->fptr += wcnt;
      btw -= wcnt; 
      bw += wcnt;
      if (btw == 0)
      {
        if (fp->fptr > fp->fsize)
          fp->fsize = fp->fptr; // Update file size if needed
        fp->flag |= FA__WRITTEN; // Set file changed flag
        sd_f_write_state = 0;
        *bw_new = bw;
        return (SD_OK);
      }
      else
      {
        sd_f_write_state = 1;
        *bw_new = bw;
        if (Timer_Expired(&timer_f_write))
          return (SD_BEZIG);
        goto sd_f_write_label_1;  
      }  
  }
}

//-----------------------------------------------------------------------------
static unsigned char sd_sync_state = 0;
static unsigned char SD_Sync(void) 
//static FRESULT sync(void) 
// return SD_OK, SD_ERROR of SD_BEZIG
{
s_timer timer_sync;

  Timer_Set(&timer_sync, 5, TIME_BASE_1_MSEC);
  switch (sd_sync_state)
  {
    default:
    case 0:
      fs.winflag = 1;
      sd_sync_state = 1;
    case 1:
      switch (SD_Move_Window(0))
      {
        case SD_OK:
          break;
        case SD_ERROR:
          sd_sync_state = 0;
          return (SD_ERROR);
        case SD_BEZIG:
          return (SD_BEZIG);  
      } 
      if (fs.fs_type == FS_FAT32 && 
          fs.fsi_flag)
      { // Update FSInfo sector if needed
        fs.winsect = 0;
        memset(fs.win, 0, 512);
        ST_WORD(&fs.win[BS_55AA], 0xAA55);
        ST_DWORD(&fs.win[FSI_LeadSig], 0x41615252);
        ST_DWORD(&fs.win[FSI_StrucSig], 0x61417272);
        ST_DWORD(&fs.win[FSI_Free_Count], fs.free_clust);
        ST_DWORD(&fs.win[FSI_Nxt_Free], fs.last_clust);
        sd_sync_state = 2;
      }
      else
      {
        sd_sync_state = 0;
        return (SD_OK);
      }  
    case 2:  
      switch (SD_Write_Sector(fs.win, fs.fsi_sector))
      {
        case SD_OK:
          break;
        case SD_ERROR:
          sd_sync_state = 0;
          return (SD_ERROR);
        case SD_BEZIG:
          return (SD_BEZIG);  
      }
      fs.fsi_flag = 0;
      sd_sync_state = 0;
      return (SD_OK);
  }
}

static unsigned char sd_f_sync_state = 0;
static unsigned char SD_F_Sync(FIL *fp) // Pointer to the file object
// return SD_OK, SD_ERROR, SD_INVALID_OBJECT of SD_BEZIG
{
unsigned long tim;
unsigned char *dir;
FATFS *fs = fp->fs;
s_timer timer_f_sync;

  Timer_Set(&timer_f_sync, 5, TIME_BASE_1_MSEC);
  switch (sd_f_sync_state)
  {
    default:
      sd_f_sync_state = 0;
    case 0:
      switch (SD_Validate(fs, fp->id)) // Check validity of the object
      {
        case SD_OK:
          if (fp->flag & FA__WRITTEN)
          { // Has the file been written?
            // Update the directory entry
            sd_f_sync_state = 1;
            goto sd_f_sync_label_1;
          }
          return (SD_OK);
        case SD_INVALID_OBJECT:
          return (SD_INVALID_OBJECT);
      }
sd_f_sync_label_1:
    case 1:
      switch (SD_Move_Window(fp->dir_sect))
      {
        case SD_OK:
          break;
        case SD_ERROR:
          sd_f_sync_state = 0;
          return (SD_ERROR);
        case SD_BEZIG:
          return (SD_BEZIG);
      }
      dir = fp->dir_ptr;
      dir[DIR_Attr] |= AM_ARC;                     // Set archive bit
      ST_DWORD(&dir[DIR_FileSize], fp->fsize);     // Update file size
      ST_WORD(&dir[DIR_FstClusLO], fp->org_clust); // Update start cluster
      ST_WORD(&dir[DIR_FstClusHI], fp->org_clust >> 16);
      tim = SD_Get_Fattime(); // Updated time
      ST_DWORD(&dir[DIR_WrtTime], tim);
      fp->flag &= ~(unsigned char)FA__WRITTEN;
      sd_f_sync_state = 2;
      if (Timer_Expired(&timer_f_sync))
        return (SD_BEZIG);
    case 2:
      switch (SD_Sync())
      {
        case SD_OK:
          break;
        case SD_ERROR:
          sd_f_sync_state = 0;
          return (SD_ERROR);
        case SD_BEZIG:
          return (SD_BEZIG);
      }
      sd_f_sync_state = 0;
      return (SD_OK);
  }
}

static unsigned char SD_F_Close(FIL *fp) // Pointer to the file object to be closed
// return SD_OK of SD_ERROR
{
  switch (SD_F_Sync(fp))
  {
    case SD_OK:
      fp->fs = NULL;
      return (SD_OK);
    default:
    case SD_ERROR:
      return (SD_ERROR);
    case SD_INVALID_OBJECT:
      return (SD_INVALID_OBJECT);
    case SD_BEZIG:
      return (SD_BEZIG);  
  }
}

//-----------------------------------------------------------------------------
static unsigned char sd_f_rename_state = 0;
static unsigned char SD_F_Rename(const char *path_old_in, // Pointer to the old name
                                 const char *path_new) // Pointer to the new name
{
static char const *path_old;
static unsigned long sect_old;
static unsigned char *dir_old;
static unsigned char *dir_new;
static unsigned char direntry[32-11];
static DIR dirobj;
static char fn[8+3+1];
s_timer timer_f_rename;

  Timer_Set(&timer_f_rename, 5, TIME_BASE_1_MSEC);
  switch (sd_f_rename_state)
  {
    default:
    case 0:
      path_old = path_old_in;
      sd_f_rename_state = 1;
    case 1:  
      if (SD_Auto_Mount(&path_old, 1) != SD_OK)
      {
        sd_f_rename_state = 0;
        return (SD_ERROR);
      }  
      sd_f_rename_state = 2;
    case 2:
      switch (SD_Trace_Path(&dirobj, fn, path_old, &dir_old)) // Trace the file path
      {
        case SD_OK: // Trace completed
          break;
        default:
        case SD_ERROR:
          sd_f_rename_state = 0;
          return (SD_ERROR);
        case SD_NO_VALID_NAME:
          sd_f_rename_state = 0;
          return (SD_NO_VALID_NAME);
        case SD_NO_PATH:
          sd_f_rename_state = 0;
          return (SD_NO_PATH);
        case SD_NO_FILE:
          sd_f_rename_state = 0;
          return (SD_NO_FILE);
        case SD_ROOT:
          sd_f_rename_state = 0;
          return (SD_ROOT);
        case SD_BEZIG:
          return (SD_BEZIG);
      }
      if (!dir_old)
      {
        sd_f_rename_state = 0;
        return (SD_NO_FILE);
      }  
      sect_old = fs.winsect; // Save the object information
      memcpy(direntry, &dir_old[11], 32-11);
      sd_f_rename_state = 3;
      if (Timer_Expired(&timer_f_rename))
        return (SD_BEZIG);
    case 3:
      switch (SD_Trace_Path(&dirobj, fn, path_new, &dir_new)) // Trace the file path
      {
        case SD_OK: // Trace completed
          sd_f_rename_state = 0;
          return (SD_EXIST); // The new object name is already existing
        default:
        case SD_ERROR:
          sd_f_rename_state = 0;
          return (SD_ERROR);
        case SD_NO_VALID_NAME:
          sd_f_rename_state = 0;
          return (SD_NO_VALID_NAME);
        case SD_NO_PATH:
          sd_f_rename_state = 0;
          return (SD_NO_PATH);
        case SD_NO_FILE:
          break;
        case SD_ROOT:
          sd_f_rename_state = 0;
          return (SD_ROOT);
        case SD_BEZIG:
          return (SD_BEZIG);
      }
      sd_f_rename_state = 4;
      if (Timer_Expired(&timer_f_rename))
        return (SD_BEZIG);
    case 4:
      switch (SD_Reserve_Direntry(&dirobj, &dir_new))
      {
        case SD_OK:
          break;
        default:
        case SD_ERROR:
          sd_f_rename_state = 0;
          return (SD_ERROR);  
        case SD_DIR_NOT_FOUND:
          sd_f_rename_state = 0;
          return (SD_DIR_NOT_FOUND);
        case SD_CLUST_END:
          sd_f_rename_state = 0;
          return (SD_CLUST_END);
        case SD_BEZIG:
          return (SD_BEZIG);
      }    
      memcpy(&dir_new[DIR_Attr], direntry, 32-11); // Create new entry
      memcpy(&dir_new[DIR_Name], fn, 8+3);
      dir_new[DIR_NTres] = fn[11];
      fs.winflag = 1;
      sd_f_rename_state = 5;
      if (Timer_Expired(&timer_f_rename))
        return (SD_BEZIG);
    case 5:  
      switch (SD_Move_Window(sect_old))
      {
        case SD_OK:
          break;
        case SD_ERROR:
          sd_f_rename_state = 0;
          return (SD_ERROR); // Remove old entry
        case SD_BEZIG:
          return (SD_BEZIG);
      }
      dir_old[DIR_Name] = 0xE5;
      sd_f_rename_state = 6;
      if (Timer_Expired(&timer_f_rename))
        return (SD_BEZIG);
    case 6:
      switch (SD_Sync())
      {
        case SD_OK:
          break;
        case SD_ERROR:
          sd_f_rename_state = 0;
          return (SD_ERROR);
        case SD_BEZIG:
          return (SD_BEZIG);
      }
      sd_f_rename_state = 0;
      return (SD_OK);
  }
}

//-----------------------------------------------------------------------------
static unsigned sd_f_unlink_state = 0;
static unsigned char SD_F_Unlink(const char *path_new) // Pointer to the file or directory path
// return SD_OK, SD_ERROR, SD_NO_VALID_NAME, SD_NO_PATH, SD_NO_FILE, SD_ROOT, SD_WRITE_PROTECT of SD_BEZIG
{
static const char *path;
static unsigned char *dir;
static unsigned char *sdir;
static unsigned long dsect;
static char fn[8+3+1];
static unsigned long dclust;
static DIR dirobj;
s_timer timer_f_unlink;

  Timer_Set(&timer_f_unlink, 5, TIME_BASE_1_MSEC);
  switch (sd_f_unlink_state)
  {
    default:
    case 0:
      path = path_new;
      sd_f_unlink_state = 1;
    case 1:
      if (SD_Auto_Mount(&path, 1) != SD_OK)
      {
        sd_f_unlink_state = 0;
        path_new = path;
        return (SD_ERROR);
      }  
      sd_f_unlink_state = 2;
    case 2:
      switch (SD_Trace_Path(&dirobj, fn, path, &dir)) // Trace the file path
      {
        case SD_OK:
          break;
        default:
        case SD_ERROR:
          sd_f_unlink_state = 0;
          path_new = path;
          return (SD_ERROR);
        case SD_NO_VALID_NAME:
          sd_f_unlink_state = 0;
          path_new = path;
          return (SD_NO_VALID_NAME);
        case SD_NO_PATH:
          sd_f_unlink_state = 0;
          path_new = path;
          return (SD_NO_PATH);
        case SD_NO_FILE:
          sd_f_unlink_state = 0;
          path_new = path;
          return (SD_NO_FILE);
        case SD_ROOT:
          sd_f_unlink_state = 0;
          path_new = path;
          return (SD_ROOT);
        case SD_BEZIG:
          return (SD_BEZIG);
      }
      if (dir == NULL)
      {
        sd_f_unlink_state = 0;
        path_new = path;
        return SD_NO_VALID_NAME; // It is the root directory
      }  
      if (dir[DIR_Attr] & AM_RDO)
      {
        sd_f_unlink_state = 0;
        path_new = path;
        return (SD_WRITE_PROTECT); // It is a R/O object
      }  
      dsect = fs.winsect;
      dclust = ((unsigned long)LD_WORD(&dir[DIR_FstClusHI]) << 16) | LD_WORD(&dir[DIR_FstClusLO]);
      if ((dir[DIR_Attr] & AM_DIR) == 0)
      {
        sd_f_unlink_state = 5;
        goto sd_f_unlink_label_5;
      }
      // It is a sub-directory
      dirobj.clust = dclust; // Check if the sub-dir is empty or not
      dirobj.sect = SD_Clust_To_Sect(dclust);
      dirobj.index = 2;
      sd_f_unlink_state = 3; 
      if (Timer_Expired(&timer_f_unlink))
        return (SD_BEZIG);
sd_f_unlink_label_3:      
    case 3: // while (1)
      switch (SD_Move_Window(dirobj.sect))
      {
        case SD_OK:
          break;
        default:  
        case SD_ERROR:
          sd_f_unlink_state = 0;
          path_new = path;
          return (SD_ERROR);
        case SD_BEZIG:
          return (SD_BEZIG);
      }
      sdir = &fs.win[(dirobj.index & 15) * 32];
      if (sdir[DIR_Name] == 0)
      {
        sd_f_unlink_state = 5; 
        goto sd_f_unlink_label_5;
      }
      if (sdir[DIR_Name] != 0xE5 && !(sdir[DIR_Attr] & AM_VOL))
      {
        sd_f_unlink_state = 0;
        return SD_DIR_NOT_EMPTY; // The directory is not empty
      }  
      sd_f_unlink_state = 4; 
      if (Timer_Expired(&timer_f_unlink))
        return (SD_BEZIG);
    case 4:  
      switch (SD_Next_Dir_Entry(&dirobj))
      {
        case SD_OK:  
          break;
        default:   
        case SD_ERROR:
          sd_f_unlink_state = 0;
          path_new = path;
          return (SD_ERROR);
        case SD_DIR_NOT_FOUND:  
          sd_f_unlink_state = 5;
          goto sd_f_unlink_label_5;
        case SD_BEZIG:
          return (SD_BEZIG);  
      }
      sd_f_unlink_state = 3; 
      if (Timer_Expired(&timer_f_unlink))
        return (SD_BEZIG);
      goto sd_f_unlink_label_3;
sd_f_unlink_label_5:      
    case 5:
      switch (SD_Move_Window(dsect))
      {
        case SD_OK:
          break;
        default:  
        case SD_ERROR:
          sd_f_unlink_state = 0;
          path_new = path;
          return (SD_ERROR); // Mark the directory entry 'deleted'
        case SD_BEZIG:
          return (SD_BEZIG);
      }
      dir[DIR_Name] = 0xE5;
      fs.winflag = 1;
      sd_f_unlink_state = 6;
      if (Timer_Expired(&timer_f_unlink))
        return (SD_BEZIG);
    case 6:
      switch (SD_Remove_Chain(dclust))
      {
        default:
        case SD_ERROR:
          sd_f_unlink_state = 0;
          path_new = path;
          return (SD_ERROR);
        case SD_OK:
          break;
        case SD_BEZIG:
          return (SD_BEZIG);
      }
      sd_f_unlink_state = 7;
      if (Timer_Expired(&timer_f_unlink))
        return (SD_BEZIG);
    case 7:
      switch (SD_Sync())
      {
        case SD_OK:
          break;
        case SD_ERROR:
          sd_f_unlink_state = 0;
          path_new = path;
          return (SD_ERROR);
        case SD_BEZIG:
          return (SD_BEZIG);
      }
      sd_f_unlink_state = 0;
      path_new = path;
      return (SD_OK);
  }
}

//-----------------------------------------------------------------------------
static unsigned char sd_f_read_state = 0;
static unsigned char SD_F_Read(FIL *fp,              // Pointer to the file object
                               void *buff_new,       // Pointer to data buffer
                               unsigned int btr_new, // Number of bytes to read
                               unsigned int *br_new) // Pointer to number of bytes read
// return SD_OK, SD_ERROR of SD_BEZIG
{
static unsigned char *buff;
static unsigned int btr;
static unsigned int br;
static unsigned long sect;
static unsigned long remain;
static unsigned int rcnt;
static unsigned int cc;
static unsigned long clust;
FATFS *fs = fp->fs;
s_timer timer_f_read;

  Timer_Set(&timer_f_read, 5, TIME_BASE_1_MSEC);
  switch (sd_f_read_state)
  {
    default:
    case 0:
      buff = buff_new;
      btr = btr_new;
      br = 0;
      sd_f_read_state = 1;
      clust = fp->curr_clust;
    case 1:
      switch (SD_Validate(fs, fp->id)) // Check validity of the object
      {
        case SD_OK:
          break;
        case SD_INVALID_OBJECT:
          sd_f_read_state = 0;
          *br_new = br;
          return (SD_INVALID_OBJECT);
      }
      if (fp->flag & FA__ERROR)
      {
        sd_f_read_state = 0;
        *br_new = br;
        return (SD_ERROR); // Check error flag
      }  
      if (!(fp->flag & FA_READ))
      {
        sd_f_read_state = 0;
        *br_new = br;
        return (SD_ERROR); // Check access mode
      }  
      remain = fp->fsize - fp->fptr;
      if (btr > remain)
        btr = remain; // Truncate read count by number of bytes left
      sd_f_read_state = 2;
sd_f_read_label_2:
    case 2:
      if (btr)
      {
        if ((fp->fptr % 512) == 0)
        { // On the sector boundary
          if (--fp->sect_clust)
          { // Decrement left sector counter
            sect = fp->curr_sect + 1; // Get current sector
          }
          else
          { // On the cluster boundary, get next cluster
            if (fp->fptr == 0)
              clust = fp->org_clust;
            else  
            {
              sd_f_read_state = 6;
              goto sd_f_read_label_6;
            }
            sd_f_read_state = 7;
            goto sd_f_read_label_7;
          }
          sd_f_read_state = 8;
          goto sd_f_read_label_8;
        }
        sd_f_read_state = 10;
        goto sd_f_read_label_10;
      }
      sd_f_read_state = 0;
      *br_new = br;
      return (SD_OK);  
sd_f_read_label_6:
    case 6:
      switch (SD_Get_Cluster(clust, &clust))
      {
        case SD_OK:
          break;
        case SD_ERROR:
          sd_f_read_state = 0;
          fp->flag |= FA__ERROR;
          *br_new = br;
          return (SD_ERROR);
        case SD_BEZIG:
          return (SD_BEZIG);
      }
      sd_f_read_state = 7;
      if (Timer_Expired(&timer_f_read))
        return (SD_BEZIG);
sd_f_read_label_7:
    case 7:
      if (clust < 2 || clust >= fs->max_clust)
      {
        sd_f_read_state = 0;
        fp->flag |= FA__ERROR;
        *br_new = br;
        return (SD_ERROR);
      }  
      fp->curr_clust = clust;           // Current cluster
      sect = SD_Clust_To_Sect(clust);         // Get current sector
      fp->sect_clust = fs->sects_clust; // Re-initialize the left sector counter
sd_f_read_label_8:
    case 8:
      fp->curr_sect = sect; // Update current sector
      cc = btr / 512;       // When left bytes >= 512,
      if (cc != 0)
      { // Read maximum contiguous sectors directly
        if (cc > fp->sect_clust)
          cc = fp->sect_clust;
        sd_f_read_state = 9;
        goto sd_f_read_label_9;
      }
      sd_f_read_state = 10;
      goto sd_f_read_label_10;
sd_f_read_label_9:
    case 9:
      switch (SD_Read_Sector(buff, sect))
      {
        case SD_OK:
          break;
        case SD_ERROR:
          sd_f_read_state = 0;
          fp->flag |= FA__ERROR;
          *br_new = br;
          return (SD_ERROR);
        case SD_BEZIG:
          return (SD_BEZIG);  
      }  
      fp->sect_clust -= (unsigned char)(cc - 1);
      fp->curr_sect += cc - 1;
      rcnt = cc * 512;
      sd_f_read_state = 11;
      if (Timer_Expired(&timer_f_read))
        return (SD_BEZIG);
      goto sd_f_read_label_11;
sd_f_read_label_10:
    case 10:
      switch (SD_Move_Window(fp->curr_sect))
      {
        case SD_OK:
          break;
        case SD_ERROR:
          sd_f_read_state = 0;
          fp->flag |= FA__ERROR;
          *br_new = br;
          return (SD_ERROR);
        case SD_BEZIG:
          return (SD_BEZIG);
      }
      rcnt = 512 - (unsigned int)(fp->fptr % 512); // Copy fractional bytes from sector window
      if (rcnt > btr)
        rcnt = btr;
      memcpy(buff, &fs->win[(unsigned int)fp->fptr % 512], rcnt);
      sd_f_read_state = 11;
      if (Timer_Expired(&timer_f_read))
        return (SD_BEZIG);
sd_f_read_label_11:      
    case 11:
      buff += rcnt;
      fp->fptr += rcnt;
      br += rcnt;
      btr -= rcnt;
      if (btr == 0)
      {
        sd_f_read_state = 0;
        *br_new = br;
        return (SD_OK);  
      }
      sd_f_read_state = 2;
      if (Timer_Expired(&timer_f_read))
        return (SD_BEZIG);
      goto sd_f_read_label_2;  
  }
}

//-----------------------------------------------------------------------------
static unsigned char sd_f_chmod_state = 0;
static unsigned char SD_F_Change_Mode(const char *path_new,    // Pointer to the file path
                                      unsigned char value, // Attribute bits
                                      unsigned char mask)  // Attribute mask to change
{
static const char *path;
static unsigned char *dir;
static DIR dirobj;
static char fn[8+3+1];
s_timer timer_f_change_mode;

  Timer_Set(&timer_f_change_mode, 5, TIME_BASE_1_MSEC);
  switch (sd_f_chmod_state)
  {
    default:
    case 0:
      path = path_new;
      sd_f_chmod_state = 1;
    case 1:
      if (SD_Auto_Mount(&path, 1) != SD_OK)
      {
        sd_f_chmod_state = 0;
        path_new = path;
        return (SD_ERROR);
      }  
      sd_f_chmod_state = 2;
    case 2:
      switch (SD_Trace_Path(&dirobj, fn, path, &dir)) // Trace the file path
      {
        case SD_OK: // Trace completed
          mask &= AM_RDO|AM_HID|AM_SYS|AM_ARC; // Valid attribute mask
          dir[DIR_Attr] = (value & mask) | (dir[DIR_Attr] & (unsigned char)~mask); // Apply attribute change
          break;
        default:
        case SD_ERROR:
          sd_f_chmod_state = 0;
          path_new = path;
          return (SD_ERROR);
        case SD_NO_VALID_NAME:
          sd_f_chmod_state = 0;
          path_new = path;
          return (SD_NO_VALID_NAME);
        case SD_NO_PATH:
          sd_f_chmod_state = 0;
          path_new = path;
          return (SD_NO_PATH);
        case SD_NO_FILE:
          sd_f_chmod_state = 0;
          path_new = path;
          return (SD_NO_FILE);
        case SD_ROOT:
          sd_f_chmod_state = 0;
          path_new = path;
          return (SD_ROOT);
        case SD_BEZIG:
          return (SD_BEZIG);
      }
      sd_f_chmod_state = 2;
      if (Timer_Expired(&timer_f_change_mode))
        return (SD_BEZIG);
    case 3:
      switch (SD_Sync())
      {
        case SD_OK:
          break;
        case SD_ERROR:
          sd_f_chmod_state = 0;
          path_new = path;
          return (SD_ERROR);
        case SD_BEZIG:
          return (SD_BEZIG);
      }
      sd_f_chmod_state = 0;
      path_new = path;
      return (SD_OK);
  }
}

//-----------------------------------------------------------------------------
static unsigned char sd_f_opendir_state = 0;
static unsigned char SD_F_Opendir(DIR *dirobj_new,      // Pointer to directory object to create
                                  const char *path_new) // Pointer to the directory path
{
static DIR *dirobj;
static const char *path;
static unsigned char *dir;
static char fn[8+3+1];

  switch (sd_f_opendir_state)
  {
    default:
    case 0:
      dirobj = dirobj_new;
      path = path_new;
      sd_f_opendir_state = 1;
    case 1:  
      if (SD_Auto_Mount(&path, 1) != SD_OK)
      {
        sd_f_opendir_state = 0;
        dirobj_new = dirobj;
        path_new = path;
        return (SD_ERROR);
      }  
      sd_f_opendir_state = 2;
    case 2:
      switch (SD_Trace_Path(dirobj, fn, path, &dir)) // Trace the file path
      {
        case SD_OK: // Trace completed
          if (dir[DIR_Attr] & AM_DIR)
          { // The entry is a directory
            dirobj->clust = ((unsigned long)LD_WORD(&dir[DIR_FstClusHI]) << 16) | LD_WORD(&dir[DIR_FstClusLO]);
            dirobj->sect = SD_Clust_To_Sect(dirobj->clust);
            dirobj->index = 2;
          }
          else
          { // The entry is not a directory
            sd_f_opendir_state = 0;
            dirobj_new = dirobj;
            path_new = path;
            return (SD_NO_FILE);
          }
          dirobj->id = fs.id;
          sd_f_opendir_state = 0;
          dirobj_new = dirobj;
          path_new = path;
          return (SD_OK);
        default:
        case SD_ERROR:
          sd_f_opendir_state = 0;
          dirobj_new = dirobj;
          path_new = path;
          return (SD_ERROR);
        case SD_NO_VALID_NAME:
          sd_f_opendir_state = 0;
          dirobj_new = dirobj;
          path_new = path;
          return (SD_NO_VALID_NAME);
        case SD_NO_PATH:
          sd_f_opendir_state = 0;
          dirobj_new = dirobj;
          path_new = path;
          return (SD_NO_PATH);
        case SD_NO_FILE:
          sd_f_opendir_state = 0;
          dirobj_new = dirobj;
          path_new = path;
          return (SD_NO_FILE);
        case SD_ROOT:
          dirobj->id = fs.id;
          sd_f_opendir_state = 0;
          dirobj_new = dirobj;
          path_new = path;
          return (SD_OK);
        case SD_BEZIG:
          return (SD_BEZIG);
      }
  }    
}

//-----------------------------------------------------------------------------
static unsigned char sd_f_mkdir_state = 0;
static unsigned char SD_F_Mkdir(const char *path_new) // Pointer to the directory path
{
static const char *path;
static unsigned char *dir;
static unsigned char *fw;
static char fn[8+3+1];
static unsigned long dclust;
static DIR dirobj;
static unsigned long sect;
static unsigned long dsect;
static unsigned char n;
static unsigned long tim;
unsigned long pclust;
s_timer timer_f_mkdir;

  Timer_Set(&timer_f_mkdir, 5, TIME_BASE_1_MSEC);
  switch (sd_f_mkdir_state)
  {
    default:
    case 0:
      dir = 0;
      path = path_new;
      sd_f_mkdir_state = 1;
    case 1:
      if (SD_Auto_Mount(&path, 1) != SD_OK)
      {
        sd_f_mkdir_state = 0;
        path_new = path;
        return (SD_ERROR);
      }  
      sd_f_mkdir_state = 2;
    case 2:
      switch (SD_Trace_Path(&dirobj, fn, path, &dir)) // Trace the file path
      {
        case SD_OK: // Trace completed
          sd_f_mkdir_state = 0; // path bestaat
          path_new = path;
          return (SD_OK);
        default:
        case SD_ERROR:
          sd_f_mkdir_state = 0;
          path_new = path;
          return (SD_ERROR);
        case SD_NO_VALID_NAME:
          sd_f_mkdir_state = 0;
          path_new = path;
          return (SD_NO_VALID_NAME);
        case SD_NO_PATH:
          sd_f_mkdir_state = 0;
          path_new = path;
          return (SD_NO_PATH);
        case SD_NO_FILE:
          break;
        case SD_ROOT:
          sd_f_mkdir_state = 0;
          path_new = path;
          return (SD_OK);
        case SD_BEZIG:
          return (SD_BEZIG);
      }
      sd_f_mkdir_state = 3; // path bestaat
      if (Timer_Expired(&timer_f_mkdir))
        return (SD_BEZIG);
    case 3:
      switch (SD_Reserve_Direntry(&dirobj, &dir))
      {
        case SD_OK:
          break;
        default:
        case SD_ERROR:
          sd_f_mkdir_state = 0;
          path_new = path;
          return (SD_ERROR);  
        case SD_DIR_NOT_FOUND:
          sd_f_mkdir_state = 0;
          path_new = path;
          return (SD_DIR_NOT_FOUND);
        case SD_CLUST_END:
          sd_f_mkdir_state = 0;
          path_new = path;
          return (SD_CLUST_END);
        case SD_BEZIG:
          return (SD_BEZIG);
      }    
      sect = fs.winsect;
      sd_f_open_state = 4;
      if (Timer_Expired(&timer_f_mkdir))
        return (SD_BEZIG);
    case 4:
      switch (SD_Create_Chain(0, &dclust))
      {
        case SD_OK: 
          break;
        case SD_ERROR:
          sd_f_mkdir_state = 0;
          path_new = path;
          return (SD_ERROR);
        case SD_BEZIG:
          return (SD_BEZIG);
      }
      if (dclust == 1)
      {
        sd_f_mkdir_state = 0;
        path_new = path;
        return (SD_ERROR);
      }  
      dsect = SD_Clust_To_Sect(dclust);
      if (!dsect)
      {
        sd_f_mkdir_state = 0;
        path_new = path;
        return (SD_ERROR);
      }  
      sd_f_open_state = 5;
      if (Timer_Expired(&timer_f_mkdir))
        return (SD_BEZIG);
    case 5:  
      switch (SD_Move_Window(dsect))
      {
        case SD_OK:
          break;
        case SD_ERROR:
          sd_f_mkdir_state = 0;
          path_new = path;
          return (SD_ERROR);
        case SD_BEZIG:
          return (SD_BEZIG);
      }
      fw = fs.win;
      memset(fw, 0, 512); // Initialize the directory table
      n = 1;
      dsect++;
      sd_f_mkdir_state = 6;
      if (Timer_Expired(&timer_f_mkdir))
        return (SD_BEZIG);
sd_f_mkdir_label_6:
    case 6:  
      switch (SD_Write_Sector(fw, dsect))
      {
        case SD_OK:
          break;
        case SD_ERROR:
          sd_f_mkdir_state = 0;
          path_new = path;
          return (SD_ERROR);
        case SD_BEZIG:
          return (SD_BEZIG);
      }
      if (n < fs.sects_clust)
      {
        n++;
        dsect++;
        if (Timer_Expired(&timer_f_mkdir))
          return (SD_BEZIG);
        goto sd_f_mkdir_label_6;  
      }  
      memset(&fw[DIR_Name], ' ', 8+3); // "." entry
      fw[DIR_Name] = '.';
      fw[DIR_Attr] = AM_DIR;
      tim = SD_Get_Fattime();
      ST_DWORD(&fw[DIR_WrtTime], tim);
      memcpy(&fw[32], &fw[0], 32); fw[33] = '.'; // ".." entry
      pclust = dirobj.sclust;
      ST_WORD(&fw[  DIR_FstClusHI], dclust >> 16);
      if ((fs.fs_type == FS_FAT32) && 
          (pclust == fs.dirbase))
        pclust = 0;
      ST_WORD(&fw[32+DIR_FstClusHI], pclust >> 16);
      ST_WORD(&fw[   DIR_FstClusLO], dclust);
      ST_WORD(&fw[32+DIR_FstClusLO], pclust);
      fs.winflag = 1;
      sd_f_mkdir_state = 7;
      if (Timer_Expired(&timer_f_mkdir))
        return (SD_BEZIG);
    case 7:
      switch (SD_Move_Window(sect))
      {
        case SD_OK:
          break;
        case SD_ERROR:
          sd_f_mkdir_state = 0;
          path_new = path;
          return (SD_ERROR);
        case SD_BEZIG:
          return (SD_BEZIG);
      }
      memset(&dir[0], 0, 32);               // Initialize the new entry
      memcpy(&dir[DIR_Name], fn, 8+3);      // Name
      dir[DIR_NTres] = fn[11];
      dir[DIR_Attr] = AM_DIR;               // Attribute
      ST_DWORD(&dir[DIR_WrtTime], tim);     // Crated time
      ST_WORD(&dir[DIR_FstClusLO], dclust); // Table start cluster
      ST_WORD(&dir[DIR_FstClusHI], dclust >> 16);
      sd_f_mkdir_state = 8;
      if (Timer_Expired(&timer_f_mkdir))
        return (SD_BEZIG);
    case 8:
      switch (SD_Sync())
      {
        case SD_OK:
          break;
        case SD_ERROR:
          sd_f_mkdir_state = 0;
          return (SD_ERROR);
        case SD_BEZIG:
          return (SD_BEZIG);
      }
      sd_f_mkdir_state = 0;
      return (SD_OK);
  }
}

/*-----------------------------------------------------------------------*/
/* Read Directory Entry in Sequense                                      */
/*-----------------------------------------------------------------------*/
void get_fileinfo(FILINFO *finfo,  // Ptr to store the File Information
                  const unsigned char *dir) // Ptr to the directory entry
//static void get_fileinfo(FILINFO *finfo,  // Ptr to store the File Information
//                         const BYTE *dir) // Ptr to the directory entry
// No return code
{
unsigned char n, c, a;
char *p;

  p = &finfo->fname[0];
  a = _USE_NTFLAG ? dir[DIR_NTres] : 0; // NT flag
  for (n = 0; n < 8; n++)
  { // Convert file name (body)
    c = dir[n];
    if (c == ' ')
      break;
    if (c == 0x05)
      c = 0xE5;
    if (a & 0x08 && c >= 'A' && c <= 'Z')
      c += 0x20;
    *p++ = c;
  }
  if (dir[8] != ' ')
  { // Convert file name (extension)
    *p++ = '.';
    for (n = 8; n < 11; n++)
    {
      c = dir[n];
      if (c == ' ')
        break;
      if (a & 0x10 && c >= 'A' && c <= 'Z')
        c += 0x20;
      *p++ = c;
    }
  }
  *p = '\0';

  finfo->fattrib = dir[DIR_Attr];              // Attribute
  finfo->fsize = LD_DWORD(&dir[DIR_FileSize]); // Size
  finfo->fdate = LD_WORD(&dir[DIR_WrtDate]);   // Date
  finfo->ftime = LD_WORD(&dir[DIR_WrtTime]);   // Time
}


static unsigned char sd_f_readdir_state = 0;
static unsigned char SD_F_Readdir(DIR *dirobj,    // Pointer to the directory object
                                  FILINFO *finfo) // Pointer to file information to return
{
FATFS *fs = dirobj->fs;
unsigned char *dir;
unsigned char c;
s_timer timer_f_readdir;

  Timer_Set(&timer_f_readdir, 5, TIME_BASE_1_MSEC);
  finfo->fname[0] = 0;
  if (dirobj->sect == 0)
  {
    sd_f_readdir_state = 0;
    return (SD_OK);
  }
  switch (sd_f_readdir_state)
  {
    default:
    case 0:
      sd_f_readdir_state = 1;
    case 1:
      switch (SD_Validate(fs, dirobj->id)) // Check validity of the object
      {
        case SD_OK:
          break;
        case SD_INVALID_OBJECT:
          sd_f_readdir_state = 0;
          return (SD_INVALID_OBJECT);
      }
sd_f_readdir_label_2:
      sd_f_readdir_state = 2;
    case 2:
      switch (SD_Move_Window(dirobj->sect))
      {
        case SD_OK:
          break;
        case SD_ERROR:
          sd_f_readdir_state = 0;
          return (SD_ERROR);
        case SD_BEZIG:
          return (SD_BEZIG);
      }
      dir = &fs->win[(dirobj->index & 15) * 32]; // pointer to the directory entry
      c = dir[DIR_Name];
      if (c == 0)
      {
        sd_f_readdir_state = 0;
        return (SD_OK);
      }
      if (c != 0xE5 && !(dir[DIR_Attr] & AM_VOL)) // Is it a valid entry?
        get_fileinfo(finfo, dir);
      sd_f_readdir_state = 3;
    case 3:
      switch (SD_Next_Dir_Entry(dirobj))
      {
        case SD_OK:  
          break;
        default:   
        case SD_ERROR:
          dirobj->sect = 0;
          sd_f_readdir_state = 0;
          return (SD_ERROR);
        case SD_DIR_NOT_FOUND:  
          dirobj->sect = 0;
          sd_f_readdir_state = 0;
          return (SD_DIR_NOT_FOUND);
        case SD_BEZIG:
          return (SD_BEZIG);  
      }
      sd_f_readdir_state = 0;
      if (finfo->fname[0])
        return (SD_OK);
      if (Timer_Expired(&timer_f_readdir))
        return (SD_BEZIG);
      goto sd_f_readdir_label_2;
  }
}

//-----------------------------------------------------------------------------
static unsigned char SD_Card_Operation(unsigned char operation, ...)
// alle sd card functie moeten via deze functie worden aangeroepen
// deze functie zorgt ervoor dat er slechts 1 sd functie tegelijk actief is
// indien een andere functie nog actief is wordt met SD_BEZIG geantwoord
// het is mogelijk de zaak te resetten door de functie als volgt aan te roepen
// SD_Card_Operation(F_RESET);
{
va_list ap;
FIL *ptr_file;
const char *ptr_name;
const char *ptr_name_new;
unsigned char mode;
unsigned long fsize;
unsigned char value;
unsigned char mask;
FILINFO *ptr_file_info;
void *ptr_buffer;
DIR *ptr_dir;
unsigned int nr_bytes;  // Number of bytes to read
unsigned int *ptr_nr_bytes; // Pointer to number of bytes read
static unsigned char res = SD_OK;
static unsigned char old_operation = F_RESET;
 
  if (operation == F_RESET)
  {
    res = SD_OK;
    old_operation = F_RESET;
    sd_create_chain_state = 0;
    sd_remove_chain_state = 0;
    sd_trace_path_state = 0;
    sd_reserve_direntry_state = 0;
    sd_f_open_state = 0;
    sd_f_read_state = 0;
    sd_f_write_state = 0;
    sd_f_lseek_state = 0;
    sd_f_opendir_state = 0;
    sd_f_stat_state = 0;
    sd_f_unlink_state = 0;
    sd_f_mkdir_state = 0;
    sd_f_chmod_state = 0;
    sd_f_rename_state = 0;

    sd_sync_state = 0;
    sd_f_sync_state = 0;
    sd_write_sector_state = 0;
    sd_read_sector_state = 0;
    sd_move_window_state = 0;
    sd_get_cluster_state = 0;
    sd_next_dir_entry_state = 0;
    return (SD_OK);
  }  
  else if (res != SD_BEZIG)
    old_operation = operation;
  else if (old_operation != operation)
    return (SD_BEZIG);
    
  switch (operation)
  {
    default:
      res = SD_BEZIG;
      return (res);
    //case F_RESET: 
    //  res = SD_OK;
    //  return (res);
    //case F_MOUNT:
    //  va_start(ap, operation);
    //  drv_nr = va_arg(ap, BYTE);
    //  ptr_fs = va_arg(ap, FATFS *);
    //  mode = va_arg(ap, unsigned char);
    //  va_end(ap);
    //  res = f_mount(drv_nr, ptr_fs);
    //  return (res);
    case F_OPEN:
      va_start(ap, operation);
      ptr_file = va_arg(ap, FIL *);
      ptr_name = va_arg(ap, const char *);
      mode = va_arg(ap, unsigned char);
      va_end(ap);
      res = SD_F_Open(ptr_file, ptr_name, mode);
      return (res);
    case F_CLOSE:
      va_start(ap, operation);
      ptr_file = va_arg(ap, FIL *);
      va_end(ap);
      res = SD_F_Close(ptr_file);
      return (res);
    case F_READ:
      va_start(ap, operation);
      ptr_file = va_arg(ap, FIL *);
      ptr_buffer = va_arg(ap, void *);
      nr_bytes = va_arg(ap, unsigned int);
      ptr_nr_bytes = va_arg(ap, unsigned int *);
      va_end(ap);
      res = SD_F_Read(ptr_file, ptr_buffer, nr_bytes, ptr_nr_bytes);
      return (res);
    case F_WRITE:
      va_start(ap, operation);
      ptr_file = va_arg(ap, FIL *);
      ptr_buffer = va_arg(ap, void *);
      nr_bytes = va_arg(ap, unsigned int);
      ptr_nr_bytes = va_arg(ap, unsigned int *);
      va_end(ap);
      res = SD_F_Write(ptr_file, ptr_buffer, nr_bytes, ptr_nr_bytes);
      return (res);
    case F_LSEEK:
      va_start(ap, operation);
      ptr_file = va_arg(ap, FIL *);
      fsize = va_arg(ap, unsigned long);
      va_end(ap);
      res = SD_F_Lseek(ptr_file, fsize);
      return (res);
    case F_SYNC:
      res = SD_OK;
      return (res);
    case F_OPENDIR:
      va_start(ap, operation);
      ptr_dir = va_arg(ap, DIR *);
      ptr_name = va_arg(ap, const char *);
      va_end(ap);
      res = SD_F_Opendir(ptr_dir, ptr_name);
      return (res);
    case F_READDIR:
      va_start(ap, operation);
      ptr_dir = va_arg(ap, DIR *);
      ptr_file_info = va_arg(ap, FILINFO *);
      va_end(ap);
      res = SD_F_Readdir(ptr_dir, ptr_file_info);
      return (res);
//    case F_GETFREE:
//      va_start(ap, operation);
//      drv = va_arg(ap, const char *);
//      va_end(ap);
//      res = SD_F_Getfree(drv);
//      return (res);
    case F_STAT:
      va_start(ap, operation);
      ptr_name = va_arg(ap, const char *);
      ptr_file_info = va_arg(ap, FILINFO *);
      va_end(ap);
//      res = f_stat_JP(ptr_name, ptr_file_info);
      res = SD_F_Stat(ptr_name, ptr_file_info);
      return (res);
    case F_MKDIR:
      va_start(ap, operation);
      ptr_name = va_arg(ap, const char *);
      va_end(ap);
      res = SD_F_Mkdir(ptr_name);
      return (res);
    case F_UNLINK:
      va_start(ap, operation);
      ptr_name = va_arg(ap, const char *);
      va_end(ap);
      res = SD_F_Unlink(ptr_name);
      return (res);
    case F_CHMOD:
      va_start(ap, operation);
      ptr_name = va_arg(ap, const char *);
      value = va_arg(ap, unsigned char);
      mask = va_arg(ap, unsigned char);
      va_end(ap);
      res = SD_F_Change_Mode(ptr_name, value, mask);
      return (res);
    case F_RENAME:
      va_start(ap, operation);
      ptr_name = va_arg(ap, const char *);
      ptr_name_new = va_arg(ap, const char *);
      va_end(ap);
      res = SD_F_Rename(ptr_name, ptr_name_new);
      return (res);
    case F_MKFS:
      res = SD_OK;
      return (res);
  }
}

static unsigned char SD_File_State_Control(s_file *ptr)
// return SD_OK, SD_ERROR of SD_BEZIG
{
FILINFO file_info = {0};

  if (//(setpoint.sd_card_remove != 2) || // card may be removed
      ((sd_status & STA_NODISK) == 0))
  {
    if (ptr->file_info_is_read == 0)
    {
      switch (SD_Card_Operation(F_STAT, (const char *)ptr->file_name, &ptr->file_info))
      {
        default:
        case SD_NO_VALID_NAME:
        case SD_NO_PATH:
        case SD_NO_FILE:
        case SD_ROOT:
          ptr->file_info = file_info;
        case SD_OK:
          ptr->file_info_is_read = 1;
          ptr->second = (ptr->file_info.ftime & 0x001F) * 2;
          ptr->minute = (ptr->file_info.ftime >> 5) & 0x003F;
          ptr->hour =    ptr->file_info.ftime >> 11;
          ptr->day =     ptr->file_info.fdate & 0x001F;
          ptr->month =  (ptr->file_info.fdate >> 5) & 0x000F;
          ptr->year =   (ptr->file_info.fdate >> 9) + 1980;
          break;
        case SD_ERROR:
          return (SD_ERROR);
        case SD_BEZIG:
          return (SD_BEZIG);
      }
    }    
  }    
  return (SD_OK);
}

//#pragma class HB=EXTENDED_MEMORY
#pragma noclear
static unsigned char write_buffer_sd_card[MAX_WRITE_BUFFER_SD_CARD];
#pragma clear
#pragma default_attributes

static unsigned char SD_File_Write_Control(s_file *ptr)
// return 0 if file not busy
{
FILINFO file_info = {0};
unsigned int bw;
static unsigned int write_cnt_sd_card;
//static unsigned char write_buffer_sd_card[MAX_WRITE_BUFFER_SD_CARD];

  switch (ptr->write_status)
  {
    default: 
      ptr->write_status = 0;
    case 0:
      if (setp_alg.sd_card_status != 2) // card may be removed
      {
        if (((ptr->write.cnt >= WRITE_CNT_MAX) && (ptr->rename_flag == 0)) ||
            (((ptr->write_time_out > WRITE_TIME_OUT_MAX) || (setp_alg.sd_card_status == 1) || (*ptr->ptr_log_on == 0) || (ptr->rename_flag == 1)) && (ptr->write.cnt)))
        {
          ptr->write_time_out = 0;
          ptr->file_info_is_read = 0;
          ptr->write_status = 1;  
        }  
        if (ptr->rename_flag == 1)
          ptr->rename_flag = 2; 
      }    
      break;  
    case 1: 
      switch (SD_Card_Operation(F_OPEN, &ptr->file_write, (const char *)ptr->file_name, FA_OPEN_ALWAYS | FA_WRITE))
      {
        case SD_OK:
          ptr->write_status = 2;
          break;
        default:  
        case SD_ERROR:
          ptr->write_status = 0;
          break;
        case SD_ROOT:  
        case SD_NO_VALID_NAME:
        case SD_NO_FILE:
        case SD_NO_PATH:
        case SD_DIR_NOT_FOUND:
        case SD_CLUST_END:
        case SD_EXIST:
        case SD_WRITE_PROTECT:
          ptr->write_status = 0;
          break;
        case SD_BEZIG:
          break;
      }
      break; 
    case 2:
      switch (SD_Card_Operation(F_LSEEK, &ptr->file_write, ptr->file_write.fsize))
      {
        case SD_OK:
          ptr->write_status = 3;
          break;
        case SD_ERROR:
          ptr->write_status = 5;
          break;
        case SD_INVALID_OBJECT:
          ptr->write_status = 5;
          break;
        case SD_BEZIG:
          break;
      }
      break;
    case 3:
      write_cnt_sd_card = 0;
      while ((ptr->write.cnt) && 
             (write_cnt_sd_card < MAX_WRITE_BUFFER_SD_CARD))
      {
        write_buffer_sd_card[write_cnt_sd_card] = ptr->write.data[ptr->write.get_index];
        write_cnt_sd_card += 1;
        ptr->write.get_index += 1;
        ptr->write.get_index %= MAX_FILE_BUFFER;
        ptr->write.cnt -= 1;
      }       
      ptr->write_status = 4;
      break;
    case 4: 
      switch (SD_Card_Operation(F_WRITE, &ptr->file_write, write_buffer_sd_card, write_cnt_sd_card, &bw))
      {
        case SD_OK:
          if (((setp_alg.sd_card_status == 1) || (*ptr->ptr_log_on == 0) || (ptr->rename_flag == 2)) && (ptr->write.cnt))
            ptr->write_status = 3;
          else  
            ptr->write_status = 5;
          break;
        case SD_ERROR:
          ptr->write_status = 5;
          break;
        case SD_INVALID_OBJECT:
          ptr->write_status = 5;
          break;
        case SD_BEZIG:
          break;
      }
      break;
    case 5:
      switch (SD_Card_Operation(F_CLOSE, &ptr->file_write))
      {
        case SD_OK:
          ptr->write_status = 0;
          break;
        case SD_ERROR:
          ptr->write_status = 0;
          break;
        case SD_INVALID_OBJECT:
          ptr->write_status = 0;
          break;
        case SD_BEZIG:
          break;  
      }
      break;  
  }
  return (ptr->write_status);
}

static unsigned char SD_File_Rename_Control(s_file *ptr)
// return 0 if file not busy
{
static char new_file_name[20];
FILINFO file_info = {0};

  switch (ptr->rename_status)
  {
    default: 
      ptr->rename_status = 0;
    case 0:
      if ((ptr->file_info.fname[0] != 0) && // file bestaat op sd kaart
          (setp_alg.sd_card_status != 2)) // card may be removed
      {
        if (ptr->rename_flag == 2)
        {
          ptr->file_info_is_read = 0;
          if (ptr->rename_cnt_max == 0)
          {
            ptr->rename_status = 4;  
          }
          else
          {
            if (ptr->rename_cnt >= ptr->rename_cnt_max)
              ptr->rename_cnt = 0;
            ptr->rename_status = 1;  
          }  
        }  
      }    
      else
        ptr->rename_flag = 0;
      break;  
    case 1: 
      sprintf(new_file_name, "%s_%03i.%03i", ptr->file_name_base, ptr->rename_cnt, ptr->adres);
      switch (SD_Card_Operation(F_STAT, new_file_name, &file_info))
      {
        case SD_OK:
          if (ptr->rename_aantal < ptr->rename_cnt_max)
          {
            if ((ptr->rename_oudste_fdate == 0) && 
                (ptr->rename_oudste_ftime == 0))
            {
              ptr->rename_oudste_cnt = ptr->rename_cnt;
              ptr->rename_oudste_fdate = file_info.fdate;
              ptr->rename_oudste_ftime = file_info.ftime;
            }    
            else
            {
              if (ptr->rename_oudste_fdate < file_info.fdate)
              {
                ptr->rename_oudste_cnt = ptr->rename_cnt;
                ptr->rename_oudste_fdate = file_info.fdate;
                ptr->rename_oudste_ftime = file_info.ftime;
              }
              else if ((ptr->rename_oudste_fdate == file_info.fdate) &&
                       (ptr->rename_oudste_ftime < file_info.ftime)) 
              {
                ptr->rename_oudste_cnt = ptr->rename_cnt;
                ptr->rename_oudste_fdate = file_info.fdate;
                ptr->rename_oudste_ftime = file_info.ftime;
              }
            }
            ptr->rename_aantal++;
            if (ptr->rename_aantal == ptr->rename_cnt_max)
              ptr->rename_cnt = ptr->rename_oudste_cnt;
            ptr->rename_cnt++;
            ptr->rename_cnt %= ptr->rename_cnt_max;
          }  
          else  
          {
            // wis deze file en rename nieuwe file naar deze naam
            ptr->rename_status = 3;
          }  
          break;
        case SD_NO_FILE:  
          if (ptr->rename_aantal < ptr->rename_cnt_max)
          {
            ptr->rename_aantal++;
            if (ptr->rename_aantal == ptr->rename_cnt_max)
              ptr->rename_cnt = ptr->rename_oudste_cnt;
            ptr->rename_cnt++;
            ptr->rename_cnt %= ptr->rename_cnt_max;
          }
          else
          {
            ptr->rename_status = 2;
          }  
          break;
        case SD_BEZIG:
          break;
        default:
          ptr->rename_flag = 0;
          ptr->rename_status = 0;
          break;
      }
      break;
    case 2: 
      switch (SD_Card_Operation(F_RENAME, ptr->file_name, new_file_name))
      {
        case SD_OK:
          ptr->rename_cnt++;
          ptr->rename_cnt %= ptr->rename_cnt_max;
          if (ptr->rename_aantal < ptr->rename_cnt_max)
            ptr->rename_aantal++;
          break;
        case SD_BEZIG:
          break;
        default:
          ptr->rename_flag = 0;
          ptr->rename_status = 0;
          break;
      }    
      break;
    case 3: 
      switch (SD_Card_Operation(F_UNLINK, new_file_name))
      {
        case SD_OK: // ga naar wegschrijven data
          ptr->rename_status = 2;
          break;
        case SD_BEZIG:
          break;
        default:
          ptr->rename_flag = 0;
          ptr->rename_status = 0;
          break;
      }
      break;
    case 4: 
      switch (SD_Card_Operation(F_UNLINK, ptr->file_name))
      {
        case SD_OK: // ga naar wegschrijven data
          ptr->rename_flag = 0;
          ptr->rename_status = 0;
          break;
        case SD_BEZIG:
          break;
        default:
          ptr->rename_flag = 0;
          ptr->rename_status = 0;
          break;
      }
      break;
  }
  return (ptr->rename_status);
}

#pragma noclear
static unsigned char read_buffer_sd_card[MAX_READ_BUFFER_SD_CARD];
#pragma clear
#pragma default_attributes

static void Buffer_Put_Byte(s_buffer *ptr, unsigned char ch)
{
  ptr->cnt++;
  ptr->data[ptr->put_index] = ch;
  ptr->put_index++;
  ptr->put_index %= MAX_FILE_BUFFER;
}

static void Buffer_Put_Data(s_buffer *ptr, unsigned char *data, int lengte)
{
int loop;

  for (loop = 0; loop < lengte; loop++)
    Buffer_Put_Byte(ptr, data[loop]);
}

unsigned char Buffer_Get_Byte(s_buffer *ptr)
{
unsigned char c;
  
  if (ptr->cnt == 0)
    return (0);
  c = ptr->data[ptr->get_index];
  ptr->cnt--;
  ptr->get_index++;  
  ptr->get_index %= MAX_FILE_BUFFER;  
  return (c);
}

void SD_File_Read_Start(char *file_name, unsigned char hex)
{
s_file file_empty = { 0 };
  
  log_file[LOG_READ] = file_empty;
  strncpy(log_file[LOG_READ].file_name, file_name, sizeof(log_file[LOG_READ].file_name) - 1);
  log_file[LOG_READ].ptr_log_on = &log_off;
  log_file[LOG_READ].file_info_is_read = 0;
  log_file[LOG_READ].rename_cnt_max = 0;
  log_file[LOG_READ].hex = hex;
  if (file_name[0] != 0)
    log_file[LOG_READ].used = 1;
  log_file[LOG_READ].read_flag = SD_READ_START;
}

static unsigned char SD_File_Read_Control(s_file *ptr)
// read_flag = SD_READ_READY
//             SD_READ_START
//             SD_READ_BEZIG
//             SD_READ_STOP
//             SD_READ_RESET
{
int bytes_read;

  switch (ptr->read_status)
  {
    default:
      ptr->read_status = 0;
    case 0:
      if ((ptr->file_info.fname[0] != 0) && // file bestaat op sd kaart
          (setp_alg.sd_card_status != 2)) // card may be removed
      {
        switch (ptr->read_flag)
        {
          case SD_READ_READY: // wacht op start lezen
            break;
          case SD_READ_START: // start 
            ptr->read_fptr = 0;
            ptr->read_status = 1;
            ptr->read.get_index = 0;
            ptr->read.put_index = 0;
            ptr->read.cnt = 0;
            ptr->read_flag = SD_READ_BEZIG;
            break;
          case SD_READ_BEZIG: // bezig
            if (ptr->read.cnt <= 512)
              ptr->read_status = 1;
            break;
          case SD_READ_STOP: // stoppen
            if (ptr->read.cnt <= 0)
              ptr->read_flag = SD_READ_RESET;
            break;
          default:
          case SD_READ_RESET: // init
            ptr->read_fptr = 0;
            ptr->read_status = 0;
            ptr->read.get_index = 0;
            ptr->read.put_index = 0;
            ptr->read.cnt = 0;
            ptr->read_flag = SD_READ_READY;
            break;
        }
      }
      break;
    case 1:
      switch (SD_Card_Operation(F_OPEN, &ptr->file_read, (const char *)ptr->file_name, FA_READ))
      {
        case SD_OK:
          ptr->read_status = 2;
          break;
        case SD_ERROR:
          ptr->read_status = 0;
          ptr->read_flag = SD_READ_RESET;
          break;
        case SD_ROOT:  
        case SD_NO_VALID_NAME:
        case SD_NO_FILE:
        case SD_NO_PATH:
        case SD_DIR_NOT_FOUND:
        case SD_CLUST_END:
        case SD_EXIST:
        case SD_WRITE_PROTECT:
          ptr->read_status = 0;
          ptr->read_flag = SD_READ_RESET;
          break;
        case SD_BEZIG:
          break;
      }
      break;
    case 2:
      switch (SD_Card_Operation(F_LSEEK, &ptr->file_read, ptr->read_fptr))
      {
        case SD_OK:
          ptr->read_status = 3;
          break;
        case SD_ERROR:
          ptr->read_status = 4;
          ptr->read_flag = SD_READ_RESET;
          break;
        case SD_BEZIG:
          break;
      }
      break;
    case 3:
      switch (SD_Card_Operation(F_READ, &ptr->file_read, read_buffer_sd_card, 512, &bytes_read))
      {
        case SD_OK:
          ptr->read_fptr = ptr->file_read.fptr;
          Buffer_Put_Data(&ptr->read, read_buffer_sd_card, bytes_read);
          if (ptr->read_fptr >= ptr->file_read.fsize)
            ptr->read_flag = SD_READ_STOP;
          ptr->read_status = 4;
          break;
        case SD_ERROR:
          ptr->read_status = 4;
          ptr->read_flag = SD_READ_RESET;
          break;
        case SD_BEZIG:
          break;
      }
      break;
    case 4:
      switch (SD_Card_Operation(F_CLOSE, &ptr->file_read))
      {
        case SD_OK:
          ptr->read_status = 0;
          break;
        case SD_ERROR:
          ptr->read_status = 0;
          ptr->read_flag = SD_READ_RESET;
          break;
        case SD_BEZIG:
          break;
      }
      break;
  }
  return (ptr->read_status);
}

//
char sd_directory_path[50] = "\\";
unsigned char sd_directory_flag = 0;
s_buffer sd_directory_buffer;

void SD_Directory_Read_Start(char *path)
{
  strcpy(sd_directory_path,path);
  sd_directory_flag = SD_DIRECTORY_START;
}

static unsigned char SD_Directory_Read_Control(void)
{
char str[100];
static unsigned char fase = 0;
static DIR dir;
static FILINFO finfo;

  if (//(setpoint.sd_card_remove != 2) || // card may be removed
      ((sd_status & STA_NODISK) == 0))
  {
    if (sd_directory_flag == 1) // opnieuw starten inlezen
      fase = 0;
    switch (fase)
    {
      default:
        fase = 0;
      case 0:
        if (setp_alg.sd_card_status != 2)
        {
          switch (sd_directory_flag)
          {
            case SD_DIRECTORY_READY:
              break;
            case SD_DIRECTORY_START:
              sd_directory_buffer.cnt = 0;
              sd_directory_buffer.get_index = 0;
              sd_directory_buffer.put_index = 0;
              sd_directory_flag = SD_DIRECTORY_BEZIG;
              fase = 1;
              break;
            case SD_DIRECTORY_BEZIG:
              if (sd_directory_buffer.cnt <= 512)
                fase = 2;
              break;
            case SD_DIRECTORY_STOP:
              if (sd_directory_buffer.cnt <= 0)
                sd_directory_flag = SD_DIRECTORY_RESET;
              break;
            default:
            case SD_DIRECTORY_RESET:
              sd_directory_buffer.cnt = 0;
              sd_directory_buffer.get_index = 0;
              sd_directory_buffer.put_index = 0;
              sd_directory_flag = SD_DIRECTORY_READY;
              break;
          }
        }
        break;
      case 1:   
        switch (SD_Card_Operation(F_OPENDIR, &dir, sd_directory_path))
        {
          case SD_OK:
            fase = 2;
            break;
          case SD_ERROR:
          case SD_NO_FILE:
          case SD_NO_VALID_NAME:
          case SD_NO_PATH:
            fase = 0;
            sd_directory_flag = 0;
            return (SD_OK);
          case SD_ROOT:
            fase = 2;
            break;
          case SD_BEZIG:
            return (SD_BEZIG);
        }
        break;
      case 2:
        switch (SD_Card_Operation(F_READDIR, &dir, &finfo))
        {
          case SD_OK:
            if (finfo.fname[0] == 0)
            {
              fase = 0;
              sd_directory_flag = SD_DIRECTORY_STOP;
              return (SD_OK);
            }
            if (finfo.fattrib & AM_DIR)
            { // directory '>' geeft aan dat dit een directory is
              sprintf(str, ">%s %li %02i-%02i-%04i %02i:%02i:%02i ", finfo.fname, 
                                                                     finfo.fsize,
                                                                     finfo.fdate & 0x001F, (finfo.fdate & 0x01E0) >> 5, (finfo.fdate >> 9)+1980,
                                                                     finfo.ftime >> 11, (finfo.ftime & 0x07E0) >> 5, (finfo.ftime & 0x0001F) * 2);
            }
            else
            { // file
              sprintf(str, "%s %li %02i-%02i-%04i %02i:%02i:%02i ", finfo.fname, 
                                                                    finfo.fsize,
                                                                    finfo.fdate & 0x001F, (finfo.fdate & 0x01E0) >> 5, (finfo.fdate >> 9)+1980,
                                                                    finfo.ftime >> 11, (finfo.ftime & 0x07E0) >> 5, (finfo.ftime & 0x0001F) * 2);
            }
            Buffer_Put_Data(&sd_directory_buffer, (unsigned char *)str, strlen(str) + 1);
            if (sd_directory_buffer.cnt > 512)
              fase = 0;
          case SD_BEZIG:    
            return (SD_BEZIG);
          default:
            fase = 0;
            sd_directory_flag = SD_DIRECTORY_RESET;
            break;
        }
    }
  }
  else
  {
    fase = 0;
    sd_directory_flag = 0;
  }
  return (SD_OK);
}

//*****************************************************************************
char sd_delete_file_path[50] = "\\";
unsigned char sd_delete_file_flag = 0;

void SD_File_Delete_Start(char *file_name)
{
s_file file_empty = { 0 };
  
  log_file[LOG_READ] = file_empty;
  strncpy(log_file[LOG_READ].file_name, file_name, sizeof(log_file[LOG_READ].file_name) - 1);
  log_file[LOG_READ].ptr_log_on = &log_off;
  log_file[LOG_READ].file_info_is_read = 0;
  log_file[LOG_READ].rename_cnt_max = 0;
  log_file[LOG_READ].hex = TEKST_FILE_FORMAT;
  if (file_name[0] != 0)
    log_file[LOG_READ].used = 1;
  sd_delete_file_flag = 1;
}

unsigned char SD_File_Delete_Ready(void)
{
  return (sd_delete_file_flag == 0);
}

static unsigned char SD_Delete_File_Control(void)
{
static unsigned char fase = 0;
  
  switch (fase)
  {
    case 0:
      if (sd_delete_file_flag)
        fase = 1;
      break;
    case 1:
      if (log_file[LOG_READ].file_info_is_read)
        fase = 2;
      else
        return (SD_BEZIG);
    case 2:
      switch (SD_Card_Operation(F_UNLINK, log_file[LOG_READ].file_name))
      {
        case SD_OK: // ga naar wegschrijven data
          fase = 0;
          sd_delete_file_flag = 0;
          break;
        case SD_BEZIG:
          return (SD_BEZIG);
        default:
          fase = 0;
          sd_delete_file_flag = 0;
          break;
      }
      break;
        
  } 
  return (SD_OK);
}

//*****************************************************************************
static void SD_Files_Control(void)
{
//FATFS *ptr_fs;
static unsigned char fase = 0;
static unsigned char file_cnt = 0;
static unsigned char file_remove_cnt = 0;

  switch (fase)
  {
    default:
    case 0:
      fase = 1;
    case 1:
      disk_space = (unsigned long)(fs.max_clust - 2) * fs.sects_clust / 2;
      free_disk_space = fs.free_clust * fs.sects_clust / 2;
      if (free_disk_space > 10)
      {
        fase = 2;
        file_cnt = 0;
        file_remove_cnt = 0;
      }
      else
      {
        // disk full
      }  
      break;
    case 2:
      while (file_cnt < MAX_FILES)
      {
        if (log_file[file_cnt].used)
        {
          switch (SD_File_State_Control(&log_file[file_cnt]))
          {
            case SD_OK:
              break;
            default:
            case SD_ERROR:
              return;
            case SD_BEZIG:
              return;
          }    
          file_cnt++;
          if (setp_alg.sd_card_status == 1)
            file_remove_cnt++;  
          break;
        }
        else
        {
          file_cnt++;
          if (setp_alg.sd_card_status == 1)
            file_remove_cnt++;  
        }        
      }  
      if (file_cnt >= MAX_FILES)
      {
        file_cnt = 0;
        fase = 3;
      }  
      break;
    case 3: // write data to sd card
      while (file_cnt < MAX_FILES)
      {
        if (log_file[file_cnt].used)
        {
          if (SD_File_Write_Control(&log_file[file_cnt]) == 0)
          {
            file_cnt++;
            if (setp_alg.sd_card_status == 1)
              file_remove_cnt++;  
          }
          break;
        }
        else
        {
          file_cnt++;
          if (setp_alg.sd_card_status == 1)
            file_remove_cnt++;  
        }      
      }        
      if (file_cnt >= MAX_FILES)
      {
        file_cnt = 0;
        fase = 4;
      }  
      break;
    case 4: // rename file
      while (file_cnt < MAX_FILES)
      {
        if (log_file[file_cnt].used)
        {
          if (SD_File_Rename_Control(&log_file[file_cnt]) == 0)
          {
            file_cnt++;
            if (setp_alg.sd_card_status == 1)
              file_remove_cnt++;
          }
          break;
        }
        else
        {
          file_cnt++;
          if (setp_alg.sd_card_status == 1)
            file_remove_cnt++;
        }
      }
      if (file_cnt >= MAX_FILES)
      {
        file_cnt = 0;
        fase = 5;
      }
      break;
    case 5: // read data from sd card
      while (file_cnt < MAX_FILES)
      {
        if (log_file[file_cnt].used)
        {
          if (SD_File_Read_Control(&log_file[file_cnt]) == 0)
          {
            file_cnt++;
            if (setp_alg.sd_card_status == 1)
              file_remove_cnt++;
          }
          break;
        }
        else
        {
          file_cnt++;
          if (setp_alg.sd_card_status == 1)
            file_remove_cnt++;
        }
      }
      if (file_cnt >= MAX_FILES)
      {
        file_cnt = 0;
        fase = 6;
      }
      break;
    case 6: // lezen directory
      if (SD_Directory_Read_Control() == 0)
      {
        if (setp_alg.sd_card_status == 1)
          file_remove_cnt++;  
        fase = 7;
      }
      break;
    case 7: // delete file
      if (SD_Delete_File_Control() == 0)
      {
        if (setp_alg.sd_card_status == 1)
          file_remove_cnt++;
        fase = 8;
      }  
      break;
    case 8: // controleer of ergens data geschreven wordt
      if ((file_remove_cnt >= MAX_FILES * 4 + 1 + 1) &&
          (read_write_options == 0) &&
          #ifdef SD_MANAGEMENT
          (read_write_management == 0) &&
          #endif // SD_MANAGEMENT
          (read_write_setpoints == 0)) // all files are not in use
        setp_alg.sd_card_status = 2;  
      fase = 0;
      break;  
  }  
}

static void SD_Read_Write_Options(void)
{
static unsigned char *ptr;
static unsigned char *ptr_end;
static unsigned char state = 0;
static int cnt;
static int wait_cnt;
static unsigned char blok_cnt;
s_timer timer_options;
char file_naam[9];
unsigned int default_area_size;
unsigned int aantal;
unsigned int computer;
unsigned int soort;
unsigned char taal_oud;
unsigned char taal_inst_oud;

  switch (read_write_options)
  {
    case 0: // doe niets
      *log_file[OPT].ptr_log_on = 0;
      if (log_file[OPT].reset_file_naam)
      {
        strcpy(file_naam, LOG_FILE_2_NAME);
        switch (selection_opt_nr)
        {
          case 0: strcat(file_naam,"A"); break;
          case 1: strcat(file_naam,"I"); break;
          case 2: strcat(file_naam,"P"); break;
        }
        SD_File_Reset(&log_file[OPT], file_naam, &setp_alg.sd_card_log_on[OPT], OPT_RENAME_MAX, HEX_FILE_FORMAT, opt_alg.adres);
      }
      state = 0;
      break;
    case 1: // write
      switch (state)
      {
        case 0:
          selection_opt_nr = 0;
          state = 1;
        case 1:
          if ((setp_alg.sd_card_status == 0) &&
              ((sd_status & (STA_NODISK | STA_PROTECT)) == 0))
          {
            strcpy(file_naam, LOG_FILE_2_NAME);
            switch (selection_opt_nr)
            {
              case 0: 
                strcat(file_naam,"A"); 
                ptr = (unsigned char *)&opt_alg;
                ptr_end = (unsigned char *)&opt_alg.end;
                break;
              case 1: 
                strcat(file_naam,"I"); 
                ptr = (unsigned char *)&opt_io;
                ptr_end = (unsigned char *)&opt_io.end;
                break;
              case 2: 
                strcat(file_naam,"P"); 
                ptr = (unsigned char *)&opt_app;
                ptr_end = (unsigned char *)&opt_app.end;
                break;
            }
            SD_File_Reset(&log_file[OPT], file_naam, &setp_alg.sd_card_log_on[OPT], OPT_RENAME_MAX, HEX_FILE_FORMAT, opt_alg.adres);
            blok_cnt = 0;
            state = 2;
          }
          else
          {
            state = 0;
            *log_file[OPT].ptr_log_on = 0;
            log_file[OPT].reset_file_naam = 1;
            read_write_options = 0;
            break;
          }
        case 2:
          if ((setp_alg.sd_card_status == 0) &&
              ((sd_status & (STA_NODISK | STA_PROTECT)) == 0))
          {
            switch (SD_Card_Operation(F_STAT, (const char *)log_file[OPT].file_name, &log_file[OPT].file_info))
            {
              default:
              case SD_NO_VALID_NAME:
              case SD_NO_PATH:
              case SD_NO_FILE:
              case SD_ROOT:
                *log_file[OPT].ptr_log_on = 1;
                state = 4;
                break;
              case SD_OK:
                log_file[OPT].file_info_is_read = 0;
                log_file[OPT].rename_flag = 1;
                state = 3;
                break;
              case SD_ERROR:
                break;
              case SD_BEZIG:
                break;
            }
          }
          else
          {
            state = 0;
            *log_file[OPT].ptr_log_on = 0;
            log_file[OPT].reset_file_naam = 1;
            read_write_options = 0;
          }
          break;
        case 3: // wis reeds aanwezige file
          if ((setp_alg.sd_card_status == 0) &&
              ((sd_status & (STA_NODISK | STA_PROTECT)) == 0))
          {
            if ((log_file[OPT].rename_flag == 0) &&
                (log_file[OPT].rename_status == 0))
            {
              *log_file[OPT].ptr_log_on = 1;
              state = 4;
            }    
		  }
          else
          {
            state = 0;
            *log_file[OPT].ptr_log_on = 0;
            log_file[OPT].reset_file_naam = 1;
            read_write_options = 0;
          }
          break;
        case 4: // wegschrijven data
          if ((setp_alg.sd_card_status == 0) &&
              ((sd_status & (STA_NODISK | STA_PROTECT)) == 0))
          {
            Timer_Set(&timer_options, 5, TIME_BASE_1_MSEC);
            while (log_file[OPT].write.cnt < WRITE_CNT_MAX)
            {
              if ((unsigned long)ptr < (unsigned long)ptr_end)
              {
                if (Timer_Expired(&timer_options))
                  break;
                cnt = 0;
                log_file[OPT].write.checksum = 0;
                File_Send_Char_Checksum(OPT_FILE, blok_cnt);
                if (((unsigned long)ptr + SD_DATA_BYTES) < (unsigned long)ptr_end)
                  cnt = SD_DATA_BYTES;
                else
                  cnt = (unsigned long)ptr_end - (unsigned long)ptr;
                File_Send_Char_Checksum(OPT_FILE, cnt);
                while (cnt > 0)
                {
                  File_Send_Char_Checksum(OPT_FILE, *ptr);
                  ptr++;
                  cnt--;
                }       
                File_Send_Char(OPT_FILE, log_file[OPT].write.checksum);
                File_Put_Char(OPT_FILE, '\r');
                File_Put_Char(OPT_FILE, '\n');
                blok_cnt++;
              }
              else
              {
                log_file[OPT].write_time_out = WRITE_TIME_OUT_MAX + 1;
                state = 5;
                *log_file[OPT].ptr_log_on = 0;
                //read_write_options = 0;
                break;
              }  
            }
          }
          else
          {
            state = 0;
            *log_file[OPT].ptr_log_on = 0;
            log_file[OPT].reset_file_naam = 1;
            read_write_options = 0;
          }
          break;  
        case 5:
          if ((setp_alg.sd_card_status == 0) &&
              ((sd_status & (STA_NODISK | STA_PROTECT)) == 0))
          {
            if ((log_file[OPT].write.cnt == 0) &&
                (log_file[OPT].write_status == 0))
            {
              if (selection_opt_nr < 2)
              {
                selection_opt_nr++;
                state = 1;
              }
              else
              {
                state = 0;
                *log_file[OPT].ptr_log_on = 0;
                read_write_options = 0;
              }  
            }
          }
          else
          {
            state = 0;
            *log_file[OPT].ptr_log_on = 0;
            log_file[OPT].reset_file_naam = 1;
            read_write_options = 0;
          }
          break;
      }
      break;
    case 2: // read
      switch (state)
      {
        case 0:
          if (restore_data_busy == 0)
          {
            restore_data_busy = TIME_OUT_RESTORE_DATA_BUSY;
            selection_opt_nr = 0;
            state = 1;
          }
          else
          {
            break;
          }  
        case 1:
          if ((setp_alg.sd_card_status == 0) &&
              ((sd_status & (STA_NODISK | STA_PROTECT)) == 0))
          {
            strcpy(file_naam, LOG_FILE_2_NAME);
            switch (selection_opt_nr)
            {
              case 0: 
                strcat(file_naam,"A"); 
                ptr = (unsigned char *)&opt_alg;
                ptr_end = restore_data_buffer + sizeof(s_opt_alg);
                break;
              case 1: 
                strcat(file_naam,"I"); 
                ptr_end = restore_data_buffer + sizeof(s_opt_io);
                break;
              case 2: 
                strcat(file_naam,"P"); 
                ptr_end = restore_data_buffer + sizeof(s_opt_app);
                break;
            }
            SD_File_Reset(&log_file[OPT], file_naam, &setp_alg.sd_card_log_on[OPT], OPT_RENAME_MAX, HEX_FILE_FORMAT, opt_alg.adres);
            log_file[OPT].read_flag = SD_READ_START;
            restore_data_busy = TIME_OUT_RESTORE_DATA_BUSY;
            ptr = restore_data_buffer;
            blok_cnt = 0;
            wait_cnt = 0;
            state = 2;
          }
          else
          {
            log_file[OPT].reset_file_naam = 1;
            restore_data_busy = 0;
            state = 0;
            read_write_options = 0;
            log_file[OPT].read_flag = SD_READ_RESET;
            break;
          }  
read_options_label_2:
        case 2:
          if (log_file[OPT].read.cnt < SD_OVERHEAD) // lengte gegevens in buffer moet groter zijn dan overhead
          {
            wait_cnt++;
            if ((wait_cnt > 100) || // wachten op sd data duurt te lang
                (log_file[OPT].read_flag == SD_READ_STOP)) // als read_flag == SD_READ_STOP is alle data ingelezen
            {
              log_file[OPT].reset_file_naam = 1;
              restore_data_busy = 0;
              state = 0;
              read_write_options = 0;
              log_file[OPT].read_flag = SD_READ_RESET;
            }    
            break;
          }
          wait_cnt = 0;
          state = 3;
        case 3:
          log_file[OPT].read.checksum = 0;
          if (blok_cnt != Read_Buffer_Char_Checksum(&log_file[OPT].read))
          {
            log_file[OPT].reset_file_naam = 1;
            restore_data_busy = 0;
            state = 0;
            read_write_options = 0;
            log_file[OPT].read_flag = SD_READ_RESET;
            break;
          }
          cnt = Read_Buffer_Char_Checksum(&log_file[OPT].read);
          if (cnt > 100)
          {
            log_file[OPT].reset_file_naam = 1;
            restore_data_busy = 0;
            state = 0;
            read_write_options = 0;
            log_file[OPT].read_flag = SD_READ_RESET;
            break;
          }
          state = 4;
        case 4:
          if (log_file[OPT].read.cnt < cnt * 2 + 4) // aantal data bytes * 2 + checksum + \r + \n
          {
            wait_cnt++;
            if ((wait_cnt > 100) || // wachten op sd data duurt te lang
                (log_file[OPT].read_flag == SD_READ_STOP)) // als read_flag == 3 is alle data ingelezen
            {
              log_file[OPT].reset_file_naam = 1;
              restore_data_busy = 0;
              state = 0;
              read_write_options = 0;
              log_file[OPT].read_flag = SD_READ_RESET;
            }
            break;
          }
          wait_cnt = 0;
          while (cnt)
          {
            *ptr = Read_Buffer_Char_Checksum(&log_file[OPT].read);
            ptr++;
            cnt--;
          }
          if ((log_file[OPT].read.checksum != Read_Buffer_Char(&log_file[OPT].read)) ||
              ('\r' != Read_Buffer_Byte(&log_file[OPT].read)) ||
              ('\n' != Read_Buffer_Byte(&log_file[OPT].read)))
          {
            log_file[OPT].reset_file_naam = 1;
            restore_data_busy = 0; 
            state = 0;
            read_write_options = 0;
            log_file[OPT].read_flag = SD_READ_RESET;
            break;
          }
          blok_cnt++;
          if (((unsigned long)ptr >= (unsigned long)ptr_end) ||
              ((log_file[OPT].read_flag == SD_READ_STOP) && (log_file[OPT].read.cnt == 0)))
          {
            log_file[OPT].read_flag = SD_READ_RESET;
            state = 5;
          }
          else
          {
            state = 2;
            goto read_options_label_2;
          }
          break;
        case 5:
          switch (selection_opt_nr)
          {
            case 0: // opt_alg
              default_area_size = (unsigned char *)&default_opt_alg.end - (unsigned char *)&default_opt_alg;
              aantal   = *(unsigned int *)restore_data_buffer;
              computer = *(unsigned int *)&restore_data_buffer[2];
              soort    = *(unsigned int *)&restore_data_buffer[4];
              if ((computer == default_opt_alg.computer) && (soort == default_opt_alg.soort))
              {
                if (aantal > default_area_size)
                  aantal = default_area_size;
                taal_oud = opt_alg.taalkeuze;
                taal_inst_oud = opt_alg.taalkeuze_inst;
                BlockCopy((unsigned char *)restore_data_buffer,((unsigned char *)&opt_alg),aantal);
                opt_alg.taalkeuze = taal_oud;
                opt_alg.taalkeuze_inst = taal_inst_oud;
                if (default_area_size > aantal)
                  AddNewOptAlg();
                Can_Backbone_Address_Check();
                can_backbone_appl_init_switch = 1;
                restore_data_busy = TIME_OUT_RESTORE_DATA_BUSY;
                selection_opt_nr = 1;
                state = 1;
              }
              else
              {
                CreateAlarm(&alarm_hr_alg.restore_option_setpoint_failed_al, SYSTEEM_AL_RESTORE_OPTION_SETPOINT_FAILED, 0, 0, 0, HARD_ALARM);
                restore_data_busy = 0;
                state = 0;
                read_write_options = 0;
              }
              break;
            case 1: // opt_io
              default_area_size = (unsigned char *)&default_opt_io.end - (unsigned char *)&default_opt_io;
              aantal   = *(unsigned int *)restore_data_buffer;
              computer = *(unsigned int *)&restore_data_buffer[2];
              soort    = *(unsigned int *)&restore_data_buffer[4];
              if ((computer == default_opt_io.computer) && (soort == default_opt_io.soort))
              {
                if (aantal > default_area_size)
                  aantal = default_area_size;
                BlockCopy((unsigned char *)restore_data_buffer,((unsigned char *)&opt_io),aantal);
                if (default_area_size > aantal)
                  AddNewOptIO();
                VersieAfhankelijkOptIO();  
                CAN_IO_Init_All_Boards();
                restore_data_busy = TIME_OUT_RESTORE_DATA_BUSY;
                selection_opt_nr = 2;
                state = 1;
              }
              else
              {
                CreateAlarm(&alarm_hr_alg.restore_option_setpoint_failed_al, SYSTEEM_AL_RESTORE_OPTION_SETPOINT_FAILED, 0, 0, 0, HARD_ALARM);
                restore_data_busy = 0;
                state = 0;
                read_write_options = 0;
              }
              break;
            case 2: // opt_app
              default_area_size = (unsigned char *)&default_opt_app.end - (unsigned char *)&default_opt_app;
              aantal   = *(unsigned int *)restore_data_buffer;
              computer = *(unsigned int *)&restore_data_buffer[2];
              soort    = *(unsigned int *)&restore_data_buffer[4];
              if ((computer == default_opt_app.computer) && (soort == default_opt_app.soort))
              {
                if (aantal > default_area_size)
                  aantal = default_area_size;
                BlockCopy((unsigned char *)restore_data_buffer,((unsigned char *)&opt_app),aantal);
                if (default_area_size > aantal)
                  AddNewOptApp();
                VersieAfhankelijkOptApp();  
                ModuleCheck();
                Init_All_Screen();
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
                CheckOptions();
                Init_All_Screen();
                CreateAlarm(&alarm_hr_alg.new_option_from_pc_al, SYSTEEM_AL_NEW_OPTION, 0, 0, 0, ZACHT_ALARM);
                ClearAlarm(&alarm_hr_alg.opt_al, SYSTEEM_AL_OPT, 0);
                restore_data_busy = 0;
                state = 0;
                read_write_options = 0;
              }
              else
              {
                CreateAlarm(&alarm_hr_alg.restore_option_setpoint_failed_al, SYSTEEM_AL_RESTORE_OPTION_SETPOINT_FAILED, 0, 0, 0, HARD_ALARM);
                restore_data_busy = 0;
                state = 0;
                read_write_options = 0;
              }
              break;
            default:
              restore_data_busy = 0;
              state = 0;
              read_write_options = 0;
              break;
          }
          break;
      }
      break;
  }
}



















/*
//#pragma class HB=EXTENDED_MEMORY
#pragma noclear
static s_buffer option_buffer;
#pragma clear
#pragma default_attributes
static void SD_Read_Write_Options(void)
{
static unsigned char *ptr;
static unsigned char *ptr_end;
static unsigned char state = 0;
static unsigned char end_of_file;
int cnt;
unsigned int default_area_size;
unsigned int aantal;
unsigned int computer;
unsigned int soort;
unsigned char taal_oud;
unsigned char taal_inst_oud;
static unsigned char blok_cnt;
s_timer timer_options;

  switch (read_write_options)
  {
    case 0: // doe niets
      *log_file[F_OPT_ALG].ptr_log_on = 0;
      *log_file[F_OPT_IO ].ptr_log_on = 0;
      *log_file[F_OPT_APP].ptr_log_on = 0;
      state = 0;
      read_write_options_area = F_OPT_ALG;
      break;
    case 1: // write
      switch (state)
      {
        case 0: // controleer of file bestaat
          if ((setp_alg.sd_card_status == 0) &&
              ((sd_status & (STA_NODISK | STA_PROTECT)) == 0))
          {
            switch (SD_Card_Operation(F_STAT, (const char *)log_file[read_write_options_area].file_name, &log_file[read_write_options_area].file_info))
            {
              default:
              case SD_NO_VALID_NAME:
              case SD_NO_PATH:
              case SD_NO_FILE:
              case SD_ROOT:
                *log_file[read_write_options_area].ptr_log_on = 1;
                switch (read_write_options_area)
                {
                  case F_OPT_ALG:
                    ptr     = (unsigned char *)&opt_alg;
                    ptr_end = (unsigned char *)&opt_alg.end;
                    blok_cnt = 0;
                    state = 2;
                    break;
                  case F_OPT_IO:
                    ptr     = (unsigned char *)&opt_io;
                    ptr_end = (unsigned char *)&opt_io.end;
                    blok_cnt = 0;
                    state = 2;
                    break;
                  case F_OPT_APP:
                    ptr     = (unsigned char *)&opt_app;
                    ptr_end = (unsigned char *)&opt_app.end;
                    blok_cnt = 0;
                    state = 2;
                    break;
                  default:
                    state = 0;
                    read_write_options_area = F_OPT_ALG;
                    *log_file[F_OPT_ALG].ptr_log_on = 0;
                    *log_file[F_OPT_IO ].ptr_log_on = 0;
                    *log_file[F_OPT_APP].ptr_log_on = 0;
                    read_write_options = 0;
                    break;
                }
                break;
              case SD_OK:
                log_file[read_write_options_area].file_info_is_read = 0;
                log_file[read_write_options_area].rename_flag = 1;
                state = 1;
                break;
              case SD_ERROR:
                break;
              case SD_BEZIG:
                break;
            }
          }
          else
          {
            state = 0;
            read_write_options_area = F_OPT_ALG;
            *log_file[F_OPT_ALG].ptr_log_on = 0;
            *log_file[F_OPT_IO ].ptr_log_on = 0;
            *log_file[F_OPT_APP].ptr_log_on = 0;
            read_write_options = 0;
          }
          break;
        case 1: // wis reeds aanwezige file
          if ((setp_alg.sd_card_status == 0) &&
              ((sd_status & (STA_NODISK | STA_PROTECT)) == 0))
          {
            if ((log_file[OPT].rename_flag == 0) &&
                (log_file[OPT].rename_status == 0))
            {
              *log_file[read_write_options_area].ptr_log_on = 1;
              switch (read_write_options_area)
              {
                case F_OPT_ALG:
                  ptr     = (unsigned char *)&opt_alg;
                  ptr_end = (unsigned char *)&opt_alg.end;
                  blok_cnt = 0;
                  state = 2;
                  break;
                case F_OPT_IO:
                  ptr     = (unsigned char *)&opt_io;
                  ptr_end = (unsigned char *)&opt_io.end;
                  blok_cnt = 0;
                  state = 2;
                  break;
                case F_OPT_APP:
                  ptr     = (unsigned char *)&opt_app;
                  ptr_end = (unsigned char *)&opt_app.end;
                  blok_cnt = 0;
                  state = 2;
                  break;
                default:
                  state = 0;
                  read_write_options_area = F_OPT_ALG;
                  *log_file[F_OPT_ALG].ptr_log_on = 0;
                  *log_file[F_OPT_IO ].ptr_log_on = 0;
                  *log_file[F_OPT_APP].ptr_log_on = 0;
                  read_write_options = 0;
                  break;
              }
            }
          }
          else
          {
            state = 0;
            read_write_options_area = F_OPT_ALG;
            *log_file[F_OPT_ALG].ptr_log_on = 0;
            *log_file[F_OPT_IO ].ptr_log_on = 0;
            *log_file[F_OPT_APP].ptr_log_on = 0;
            read_write_options = 0;
          }
          break;  
        case 2: // wegschrijven data
          if ((setp_alg.sd_card_status == 0) &&
              ((sd_status & (STA_NODISK | STA_PROTECT)) == 0))
          {
            Timer_Set(&timer_options, 5, TIME_BASE_1_MSEC);
            while (log_file[read_write_options_area].write.cnt < WRITE_CNT_MAX)
            {
              if ((unsigned long)ptr <= (unsigned long)ptr_end)
              {
                if (Timer_Expired(&timer_options))
                  break;
                cnt = 0;
                log_file[read_write_options_area].write.checksum = 0;
                File_Send_Char_Checksum(&log_file[read_write_options_area], blok_cnt);
                if (((unsigned long)ptr + SD_DATA_BYTES) <= (unsigned long)ptr_end)
                  cnt = SD_DATA_BYTES;
                else
                  cnt = (unsigned long)ptr_end + 1 - (unsigned long)ptr;
                File_Send_Char_Checksum(&log_file[read_write_options_area], cnt);
                while (cnt > 0)
                {
                  File_Send_Char_Checksum(&log_file[read_write_options_area], *ptr);
                  ptr++;
                  cnt--;
                }       
                File_Send_Char(&log_file[read_write_options_area], log_file[read_write_options_area].write.checksum);
                File_Put_Char(&log_file[read_write_options_area], '\r');
                File_Put_Char(&log_file[read_write_options_area], '\n');
                blok_cnt++;
              }
              else
              {
                log_file[read_write_options_area].write_time_out = WRITE_TIME_OUT_MAX + 1;
                switch (read_write_options_area)
                {
                  case F_OPT_ALG:
                    state = 0;
                    read_write_options_area = F_OPT_IO;
                    *log_file[F_OPT_ALG].ptr_log_on = 0;
                    return; //break;
                  case F_OPT_IO:
                    state = 0;
                    read_write_options_area = F_OPT_APP;
                    *log_file[F_OPT_IO].ptr_log_on = 0;
                    return; //break;
                  case F_OPT_APP:
                    state = 0;
                    read_write_options_area = F_OPT_ALG;
                    *log_file[F_OPT_APP].ptr_log_on = 0;
                    read_write_options = 0;
                    return; //break;
                  default:
                    state = 0;
                    read_write_options_area = F_OPT_ALG;
                    *log_file[F_OPT_ALG].ptr_log_on = 0;
                    *log_file[F_OPT_IO ].ptr_log_on = 0;
                    *log_file[F_OPT_APP].ptr_log_on = 0;
                    read_write_options = 0;
                    return; //break;
                }
              }  
            }
          }
          else
          {
            state = 0;
            read_write_options_area = F_OPT_ALG;
            *log_file[F_OPT_ALG].ptr_log_on = 0;
            *log_file[F_OPT_IO ].ptr_log_on = 0;
            *log_file[F_OPT_APP].ptr_log_on = 0;
            read_write_options = 0;
          }
          break;  
      }
      break;
    case 2: // read
      switch (state)
      {
        case 0:
          if ((setp_alg.sd_card_status == 0) &&
              (restore_data_busy == 0) &&
              ((sd_status & (STA_NODISK)) == 0))
          {
            switch (SD_Card_Operation(F_STAT, (const char *)log_file[read_write_options_area].file_name, &log_file[read_write_options_area].file_info))
            {
              default:
              case SD_NO_VALID_NAME:
              case SD_NO_PATH:
              case SD_NO_FILE:
              case SD_ROOT:
                restore_data_busy = 0;
                state = 0;
                read_write_options = 0;
                read_write_options_area = F_OPT_ALG;
                break;
              case SD_OK:
                restore_data_busy = TIME_OUT_RESTORE_DATA_BUSY;
                state = 1;
                break;
              case SD_ERROR:
                break;
              case SD_BEZIG:
                break;
            }
          }
          else
          {
            restore_data_busy = 0;
            state = 0;
            read_write_options = 0;
            read_write_options_area = F_OPT_ALG;
          }
          break;  
        case 1:  
          if ((setp_alg.sd_card_status == 0) &&
              ((sd_status & (STA_NODISK)) == 0))
          {
            switch (SD_Card_Operation(F_OPEN, &log_file[read_write_options_area].file_read, (const char *)log_file[read_write_options_area].file_name, FA_READ))
            {
              case SD_OK:
                restore_data_busy = TIME_OUT_RESTORE_DATA_BUSY;
                switch (read_write_options_area)
                {
                  case F_OPT_ALG:
                    ptr = restore_data_buffer;
                    ptr_end = restore_data_buffer + (unsigned long)&opt_alg.end - (unsigned long)&opt_alg;
                    option_buffer.cnt = 0;
                    option_buffer.get_index = 0;
                    option_buffer.put_index = 0;
                    blok_cnt = 0;
                    end_of_file = 0;
                    state = 2;
                    break;
                  case F_OPT_IO:
                    ptr = restore_data_buffer;
                    ptr_end = restore_data_buffer + (unsigned long)&opt_io.end - (unsigned long)&opt_io;
                    option_buffer.cnt = 0;
                    option_buffer.get_index = 0;
                    option_buffer.put_index = 0;
                    blok_cnt = 0;
                    end_of_file = 0;
                    state = 2;
                    break;
                  case F_OPT_APP:
                    ptr = restore_data_buffer;
                    ptr_end = restore_data_buffer + (unsigned long)&opt_app.end - (unsigned long)&opt_app;
                    option_buffer.cnt = 0;
                    option_buffer.get_index = 0;
                    option_buffer.put_index = 0;
                    blok_cnt = 0;
                    end_of_file = 0;
                    state = 2;
                    break;
                  default:
                    restore_data_busy = 0;
                    state = 0;
                    read_write_options = 0;
                    read_write_options_area = F_OPT_ALG;
                    break;
                }
                break;
              default:  
              case SD_ERROR:
              case SD_ROOT:  
              case SD_NO_VALID_NAME:
              case SD_NO_FILE:
              case SD_NO_PATH:
              case SD_DIR_NOT_FOUND:
              case SD_CLUST_END:
              case SD_EXIST:
              case SD_WRITE_PROTECT:
                restore_data_busy = 0;
                state = 0;
                read_write_options = 0;
                read_write_options_area = F_OPT_ALG;
                break;
              case SD_BEZIG:
                break;
            }
          }
          else
          {
            restore_data_busy = 0;
            state = 0;
            read_write_options = 0;
            read_write_options_area = F_OPT_ALG;
          }
          break;  
        case 2:
          if ((setp_alg.sd_card_status == 0) &&
              ((sd_status & (STA_NODISK)) == 0))
          {
            if (((unsigned long)ptr <= (unsigned long)ptr_end) &&
                (end_of_file == 0))
            {
              switch (SD_Card_Operation(F_READ, &log_file[read_write_options_area].file_read, sd_read_buffer, 512, &sd_read_cnt))
              {
                case SD_OK:
                  restore_data_busy = TIME_OUT_RESTORE_DATA_BUSY;
                  Fill_Read_Buffer(&option_buffer, sd_read_buffer, sd_read_cnt);
                  if (sd_read_cnt == 0)
                  {
                    end_of_file = 1;
                    if (option_buffer.cnt == 0)
                      break;
                    else if ((option_buffer.cnt < SD_OVERHEAD) ||
                             ((option_buffer.cnt % 2) == 1))
                    {
                      restore_data_busy = 0; 
                      state = 0;
                      read_write_options = 0;
                    }
                    else
                    {
                      option_buffer.checksum = 0;
                      cnt = (option_buffer.cnt - SD_OVERHEAD) / 2;
                      if ((blok_cnt != Read_Buffer_Char_Checksum(&option_buffer)) ||
                          (cnt != Read_Buffer_Char_Checksum(&option_buffer)))
                      {
                        restore_data_busy = 0; 
                        state = 0;
                        read_write_options = 0;
                      }
                      while (cnt)
                      {
                        *ptr = Read_Buffer_Char_Checksum(&option_buffer);
                        ptr++;
                        cnt--;
                      }
                      if ((option_buffer.checksum != Read_Buffer_Char(&option_buffer)) ||
                          ('\r' != Read_Buffer_Byte(&option_buffer)) ||
                          ('\n' != Read_Buffer_Byte(&option_buffer)))
                      {
                        restore_data_busy = 0; 
                        state = 0;
                        read_write_options = 0;
                      }
                      blok_cnt++;
                    }
                  }
                  else
                  {
                    while (option_buffer.cnt >= SD_DATA_BYTES * 2 + SD_OVERHEAD)
                    {
                      option_buffer.checksum = 0;
                      cnt = SD_DATA_BYTES;
                      if ((blok_cnt != Read_Buffer_Char_Checksum(&option_buffer)) ||
                          (cnt != Read_Buffer_Char_Checksum(&option_buffer)))
                      {
                        restore_data_busy = 0; 
                        state = 0;
                        read_write_options = 0;
                      }
                      while (cnt)
                      {
                        *ptr = Read_Buffer_Char_Checksum(&option_buffer);
                        ptr++;
                        cnt--;
                      }
                      if ((option_buffer.checksum != Read_Buffer_Char(&option_buffer)) ||
                          ('\r' != Read_Buffer_Byte(&option_buffer)) ||
                          ('\n' != Read_Buffer_Byte(&option_buffer)))
                      {
                        restore_data_busy = 0; 
                        state = 0;
                        read_write_options = 0;
                      }
                      blok_cnt++;
                    }
                  }
                  break;
                case SD_BEZIG:
                  break;
                default:
                case SD_ERROR:
                  restore_data_busy = 0;
                  state = 0;
                  read_write_options = 0;
                  read_write_options_area = F_OPT_ALG;
                  break;
              }
            }
            else
            {
              switch (SD_Card_Operation(F_CLOSE, &log_file[read_write_options_area].file_read))
              {
                case SD_OK:
                  restore_data_busy = TIME_OUT_RESTORE_DATA_BUSY;
                  state = 3;
                  break;
                default:
                case SD_ERROR:
                  restore_data_busy = 0;
                  state = 0;
                  read_write_options = 0;
                  break;
                case SD_INVALID_OBJECT:
                  restore_data_busy = 0;
                  state = 0;
                  read_write_options = 0;
                  read_write_options_area = F_OPT_ALG;
                  break;
                case SD_BEZIG:
                  break;  
              }
            }
          }
          else
          {
            restore_data_busy = 0;
            state = 0;
            read_write_options = 0;
            read_write_options_area = F_OPT_ALG;
          }
          break;  
        case 3:
          restore_data_busy = 0;
          switch (read_write_options_area)
          {
            case F_OPT_ALG:
              default_area_size = (unsigned char *)&default_opt_alg.end - (unsigned char *)&default_opt_alg;
              aantal   = *(unsigned int *)restore_data_buffer;
              computer = *(unsigned int *)&restore_data_buffer[2];
              soort    = *(unsigned int *)&restore_data_buffer[4];
              if ((computer == default_opt_alg.computer) && (soort == default_opt_alg.soort))
              {
                if (aantal > default_area_size)
                  aantal = default_area_size;
                taal_oud = opt_alg.taalkeuze;
                taal_inst_oud = opt_alg.taalkeuze_inst;
                BlockCopy((unsigned char *)restore_data_buffer,((unsigned char *)&opt_alg),aantal);
                opt_alg.taalkeuze = taal_oud;
                opt_alg.taalkeuze_inst = taal_inst_oud;
                if (default_area_size > aantal)
                  AddNewOptAlg();
                VersieAfhankelijkOptAlg();  
                Can_Backbone_Address_Check();
                install_flag = 0;
                state = 0;
                read_write_options_area = F_OPT_IO;
              }
              else
              {
                CreateAlarm(&alarm_hr_alg.restore_option_setpoint_failed_al, SYSTEEM_AL_RESTORE_OPTION_SETPOINT_FAILED, 0, 0, 0, HARD_ALARM);
                state = 0;
                read_write_options = 0;
                read_write_options_area = F_OPT_ALG;
              }
              break;
            case F_OPT_IO:
              default_area_size = (unsigned char *)&default_opt_io.end - (unsigned char *)&default_opt_io;
              aantal   = *(unsigned int *)restore_data_buffer;
              computer = *(unsigned int *)&restore_data_buffer[2];
              soort    = *(unsigned int *)&restore_data_buffer[4];
              if ((computer == default_opt_io.computer) && (soort == default_opt_io.soort))
              {
                if (aantal > default_area_size)
                  aantal = default_area_size;
                BlockCopy((unsigned char *)restore_data_buffer,((unsigned char *)&opt_io),aantal);
                if (default_area_size > aantal)
                  AddNewOptIO();
                VersieAfhankelijkOptIO();  
                CAN_IO_Init_All_Boards();
                install_flag = 0;
                state = 0;
                read_write_options_area = F_OPT_APP;
              }
              else
              {
                CreateAlarm(&alarm_hr_alg.restore_option_setpoint_failed_al, SYSTEEM_AL_RESTORE_OPTION_SETPOINT_FAILED, 0, 0, 0, HARD_ALARM);
                state = 0;
                read_write_options = 0;
                read_write_options_area = F_OPT_ALG;
              }
              break;
            case F_OPT_APP:
              default_area_size = (unsigned char *)&default_opt_app.end - (unsigned char *)&default_opt_app;
              aantal   = *(unsigned int *)restore_data_buffer;
              computer = *(unsigned int *)&restore_data_buffer[2];
              soort    = *(unsigned int *)&restore_data_buffer[4];
              if ((computer == default_opt_app.computer) && (soort == default_opt_app.soort))
              {
                if (aantal > default_area_size)
                  aantal = default_area_size;
                BlockCopy((unsigned char *)restore_data_buffer,((unsigned char *)&opt_app),aantal);
                if (default_area_size > aantal)
                  AddNewOptApp();
                VersieAfhankelijkOptApp();  
                ModuleCheck();
                Init_All_Screen();
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
                CheckOptions();
                Init_All_Screen();
                install_flag = 0;
                CreateAlarm(&alarm_hr_alg.new_option_from_pc_al, SYSTEEM_AL_NEW_OPTION, 0, 0, 0, ZACHT_ALARM);
                ClearAlarm(&alarm_hr_alg.opt_al, SYSTEEM_AL_OPT, 0);
                state = 0;
                read_write_options = 0;
                read_write_options_area = F_OPT_APP;
              }
              else
              {
                CreateAlarm(&alarm_hr_alg.restore_option_setpoint_failed_al, SYSTEEM_AL_RESTORE_OPTION_SETPOINT_FAILED, 0, 0, 0, HARD_ALARM);
                state = 0;
                read_write_options = 0;
                read_write_options_area = F_OPT_ALG;
              }
              break;
            default:
              state = 0;
              read_write_options = 0;
              read_write_options_area = F_OPT_ALG;
              break;
          }
      }
      break;
  }
}
*/

static void SD_Read_Write_Setpoints(void)
{
static unsigned char *ptr;
static unsigned char *ptr_end;
static unsigned char state = 0;
static int cnt;
static int wait_cnt;
unsigned int default_area_size;
unsigned int aantal;
unsigned int computer;
unsigned int soort;
static unsigned char blok_cnt;
s_timer timer_setpoints;

  switch (read_write_setpoints)
  {
    case 0: // doe niets
      *log_file[SETP].ptr_log_on = 0;
      state = 0;
      break;
    case 1: // write
      switch (state)
      {
        case 0: // controleer of file reeds aanwezig
          if ((setp_alg.sd_card_status == 0) &&
              ((sd_status & (STA_NODISK | STA_PROTECT)) == 0))
          {
            switch (SD_Card_Operation(F_STAT, (const char *)log_file[SETP].file_name, &log_file[SETP].file_info))
            {
              default:
              case SD_NO_VALID_NAME:
              case SD_NO_PATH:
              case SD_NO_FILE:
              case SD_ROOT:
                *log_file[SETP].ptr_log_on = 1;
                ptr = (unsigned char *)&setp_alg;
                blok_cnt = 0;
                state = 2;
                break;
              case SD_OK:
                log_file[SETP].file_info_is_read = 0; 
                log_file[SETP].rename_flag = 1;
                state = 1;
                break;
              case SD_ERROR:
                break;
              case SD_BEZIG:
                break;
            }
          }    
          else
          {
            state = 0;
            *log_file[SETP].ptr_log_on = 0;
            read_write_setpoints = 0;
          }  
          break;
        case 1: // als file aanwezig dan delete file
          if ((setp_alg.sd_card_status == 0) &&
              ((sd_status & (STA_NODISK | STA_PROTECT)) == 0))
          {
            if ((log_file[SETP].rename_flag == 0) &&
                (log_file[SETP].rename_status == 0))
            {
              *log_file[SETP].ptr_log_on = 1;
              ptr = (unsigned char *)&setp_alg;
              blok_cnt = 0;
              state = 2;
            }    
          }
          else  
          {
            state = 0;
            *log_file[SETP].ptr_log_on = 0;
            read_write_setpoints = 0;
          }  
          break;
        case 2: // write data to file
          if ((setp_alg.sd_card_status == 0) &&
              ((sd_status & (STA_NODISK | STA_PROTECT)) == 0))
          {
            Timer_Set(&timer_setpoints, 5, TIME_BASE_1_MSEC);
            while (log_file[SETP].write.cnt < WRITE_CNT_MAX)
            {
              if ((unsigned long)ptr <= (unsigned long)&setp_alg.end)
              {       
                if (Timer_Expired(&timer_setpoints))
                  break;
                cnt = 0;
                log_file[SETP].write.checksum = 0;
                File_Send_Char_Checksum(SETP_FILE, blok_cnt);
                if (((unsigned long)ptr + SD_DATA_BYTES) <= (unsigned long)&setp_alg.end)
                  cnt = SD_DATA_BYTES;
                else
                  cnt = (unsigned long)&setp_alg.end + 1 - (unsigned long)ptr;
                File_Send_Char_Checksum(SETP_FILE, cnt);
                while (cnt > 0)
                {
                  File_Send_Char_Checksum(SETP_FILE, *ptr);
                  ptr++;
                  cnt--;
                }       
                File_Send_Char(SETP_FILE, log_file[SETP].write.checksum);
                File_Put_Char(SETP_FILE, '\r');
                File_Put_Char(SETP_FILE, '\n');
                blok_cnt++;
              }  
              else
              {
                log_file[SETP].write_time_out = WRITE_TIME_OUT_MAX + 1;
                state = 0;
                *log_file[SETP].ptr_log_on = 0;
                read_write_setpoints = 0;
                break;
              }  
            }
          }
          else  
          {
            state = 0;
            *log_file[SETP].ptr_log_on = 0;
            read_write_setpoints = 0;
          }
          break;
      }
      break;
    case 2: // read
      switch (state)
      {
        case 0:
          log_file[SETP].read_flag = SD_READ_START;
          restore_data_busy = TIME_OUT_RESTORE_DATA_BUSY;
          ptr = restore_data_buffer;
          ptr_end = restore_data_buffer + (unsigned long)&setp_alg.end - (unsigned long)&setp_alg;
          blok_cnt = 0;
          wait_cnt = 0;
          state = 1;
          break; 
read_setpoints_label_1:
        case 1:
          if (log_file[SETP].read.cnt < SD_OVERHEAD) // lengte gegevens in buffer moet groter zijn dan overhead regel
          {
            wait_cnt++;
            if ((wait_cnt > 100) || // wachten op sd data duurt te lang
                (log_file[SETP].read_flag == SD_READ_STOP)) // als read_flag == 3 is alle data ingelezen
            {
              restore_data_busy = 0;
              state = 0;
              read_write_setpoints = 0;
              log_file[SETP].read_flag = SD_READ_RESET;
            }
            break;
          }
          wait_cnt = 0;
          state = 2;
        case 2:
          log_file[SETP].read.checksum = 0;
          if (blok_cnt != Read_Buffer_Char_Checksum(&log_file[SETP].read))
          {
            restore_data_busy = 0;
            state = 0;
            read_write_setpoints = 0;
            log_file[SETP].read_flag = SD_READ_RESET;
            break;
          }
          cnt = Read_Buffer_Char_Checksum(&log_file[SETP].read);
          if (cnt > 100)
          {
            restore_data_busy = 0;
            state = 0;
            read_write_setpoints = 0;
            log_file[SETP].read_flag = SD_READ_RESET;
            break;
          }
          state = 3;
        case 3:
          if (log_file[SETP].read.cnt < cnt * 2 + 4) // aantal data bytes * 2 + checksum + \r + \n
          {
            wait_cnt++;
            if ((wait_cnt > 100) || // wachten op sd data duurt te lang
                (log_file[SETP].read_flag == SD_READ_STOP)) // als read_flag == 3 is alle data ingelezen
            {
              restore_data_busy = 0;
              state = 0;
              read_write_setpoints = 0;
              log_file[SETP].read_flag = SD_READ_RESET;
            }
            break;
          }
          wait_cnt = 0;
          while (cnt)
          {
            *ptr = Read_Buffer_Char_Checksum(&log_file[SETP].read);
            ptr++;
            cnt--;
          }
          if ((log_file[SETP].read.checksum != Read_Buffer_Char(&log_file[SETP].read)) ||
              ('\r' != Read_Buffer_Byte(&log_file[SETP].read)) ||
              ('\n' != Read_Buffer_Byte(&log_file[SETP].read)))
          {
            restore_data_busy = 0; 
            state = 0;
            read_write_setpoints = 0;
            log_file[SETP].read_flag = SD_READ_RESET;
            break;
          }
          blok_cnt++;
          if (((unsigned long)ptr >= (unsigned long)ptr_end) ||
              ((log_file[SETP].read_flag == SD_READ_STOP) && (log_file[SETP].read.cnt == 0)))
          {
            log_file[SETP].read_flag = SD_READ_RESET;
            state = 4;
          }
          else
          {
            state = 1;
            goto read_setpoints_label_1;
          }
          break;
        case 4:
          restore_data_busy = 0;
          state = 0;
          read_write_setpoints = 0;
          default_area_size = (unsigned char *)&default_setp_alg.end - (unsigned char *)&default_setp_alg;
          aantal = *(unsigned int *)restore_data_buffer;
          computer = *(unsigned int *)&restore_data_buffer[2];
          soort = *(unsigned int *)&restore_data_buffer[4];
          if ((computer == default_opt_alg.computer) &&
              (soort == default_opt_alg.soort))
          {
            if (aantal > default_area_size)
              aantal = default_area_size;
            BlockCopy((unsigned char *)restore_data_buffer,((unsigned char *)&setp_alg),aantal); // JP 03-11-08
            if (default_area_size > aantal)
              AddNewSetpAlg();
            VersieAfhankelijkSetpAlg();  
            CreateAlarm(&alarm_hr_alg.new_setpoint_from_pc_al, SYSTEEM_AL_NEW_SETPOINT, 0, 0, 0, ZACHT_ALARM);
            ClearAlarm(&alarm_hr_alg.setp_al, SYSTEEM_AL_SETP, 0);
          }
          else
          {
            CreateAlarm(&alarm_hr_alg.restore_option_setpoint_failed_al, SYSTEEM_AL_RESTORE_OPTION_SETPOINT_FAILED, 0, 0, 0, ZACHT_ALARM);
          }
          break;
      }
      break;
  }
}

//*****************************************************************************

static void SD_File_Reset(s_file *ptr, char *file_name, unsigned char *ptr_log_on, unsigned int rename_cnt_max, unsigned char hex, unsigned int adres)
{
s_file file_empty = { 0 };
  
  *ptr = file_empty;
  ptr->ptr_log_on = ptr_log_on;
  ptr->rename_cnt_max = rename_cnt_max;
  ptr->hex = hex;
  ptr->adres = adres;
  if (ptr->adres == 0) 
  { // deze file wordt alleen om te lezen gebruikt
    ptr->file_name_base[0] = 0;
    strncpy(ptr->file_name, file_name, sizeof(ptr->file_name) - 1);
  }
  else
  {
    strncpy(ptr->file_name_base, file_name, sizeof(ptr->file_name_base) - 1);
    ptr->file_name_base[sizeof(ptr->file_name_base)-1] = 0;
    sprintf(ptr->file_name, "%s.%03i", ptr->file_name_base, ptr->adres);
    if (file_name[0] != 0)
      ptr->used = 1;
    else
      *ptr->ptr_log_on = 0;  
  }  
}

//*****************************************************************************
static unsigned char sd_init_hardware_state = 0;
static unsigned char SD_Init_Hardware(void)
// return SD_OK of SD_ERROR
{
int i;
unsigned char resp;
unsigned char ocr[4];
s_timer timer_init_hardware;

  Timer_Set(&timer_init_hardware, 5, TIME_BASE_1_MSEC);
  switch (sd_init_hardware_state)
  {
    default:
      sd_init_hardware_state = 0;
    case 0:  
      SD_CARD_CS = 1;
      SD_CARD_CS_DP = OUTPUT;
      SD_CARD_CS_ODP = PUSH_PULL;

      SD_CARD_DI_ALTSEL0 = 0;
      SD_CARD_DI = 1;
      SD_CARD_DI_DP = INPUT;
      SD_CARD_DI_ODP = PUSH_PULL;

      SD_CARD_DO_ALTSEL0 = 1;
      //  SD_CARD_DO_ALTSEL0 = 0; // standaard IO
      SD_CARD_DO = 1;
      SD_CARD_DO_DP = OUTPUT;
      SD_CARD_DO_ODP = PUSH_PULL;

      SD_CARD_CLK_ALTSEL0 = 1;
      //  SD_CARD_CLK_ALTSEL0 = 1; // standaard IO
      SD_CARD_CLK = 1;
      SD_CARD_CLK_DP = OUTPUT;
      SD_CARD_CLK_ODP = PUSH_PULL;

      SSC0_CON_EN = 0; // enable access to control bits

      //  SSC0_BR = 0x0000; // 20Mbaud  
      //  SSC0_BR = 0x0001; // 10Mbaud  
      //  SSC0_BR = 0x0004; // 4Mbaud
      //  SSC0_BR = 0x0009; // 2Mbaud
      //  SSC0_BR = 0x0013; // 1Mbaud
      SSC0_BR = 0x0031; // 400kbaud
      //  SSC0_BR = 0x0063; // 200kbaud
      //  SSC0_BR = 0x00C7; // 100kbaud
    
      //  SSC0_CON = 0xCF57; 
      SSC0_CON = 0xCF57; 
      //  SSC0_CON = 0x4F57; 
      //   SSC0_CON_MS = 1;   // Master Select
      //   SSC0_CON_AREN = 0; // Automatic Reset Enable
      //   SSC0_CON_BEN = 1;  // Baudrate Error Enable 
      //   SSC0_CON_PEN = 1;  // Phase Error Enable
      //   SSC0_CON_REN = 1;  // Receive Error Enable
      //   SSC0_CON_TEN = 1;  // Trasnmit Error Enable 
      //   SSC0_CON_LB = 0;   // Loop Back Control (normal output)
      //   SSC0_CON_PO = 1;   // Clock Polatiry Control (Idle clock is high, leading clock edge is high-to-low transition)
      //   SSC0_CON_PH = 0;   // Clock Phase Control (shift transmit data on the leading clock edge, latch on trailing edge)
      //   SSC0_CON_HB = 1;   // Heading Control (MSB First)
      //   SSC0_CON_BM = 7;   // Data Witdh Selection (8 bits)
      //   SSC0_CON_EN = 1;   // Enable bit

      resp = SD_CARD_RB; // maak receive buffer leeg
      sd_init_hardware_state = 1;
      if (Timer_Expired(&timer_init_hardware))
        return (SD_BEZIG);
    case 1:  
      for (i = 0; i < 74; i++)
        SD_Receive_Byte(); // 80 dummy clocks
      SD_CARD_CS = 0; // CS = L
      sd_init_hardware_state = 2;
      if (Timer_Expired(&timer_init_hardware))
        return (SD_BEZIG);
    case 2:    
      if (SD_Send_Command(CMD0, 0, ocr) != 1)
      {
        SD_CARD_CS = 1; // CS = L
        sd_init_hardware_state = 0;
        return (SD_ERROR);
      }  
      SSC0_BR = 0x0001; // 10Mbaud
      sd_init_hardware_state = 2;
      if (Timer_Expired(&timer_init_hardware))
        return (SD_BEZIG);
    case 3:
      if (SD_Send_Command(CMD8, 0x1AA, ocr) == 1) // alleen dit argument is toegestaan want anders klopt checksum niet
      { // sd versie 2.00 of later
        if ((ocr[2] != 0x01) || (ocr[3] != 0xAA)) // spanning of test patroon niet ok
        {
          SD_CARD_CS = 1; // CS = L
          sd_init_hardware_state = 0;
          return (SD_ERROR);
        }  
        sd_init_hardware_state = 10;
        if (Timer_Expired(&timer_init_hardware))
          return (SD_BEZIG);
        goto sd_init_hardware_label_10;  
      }
      else
      {
        sd_init_hardware_state = 20;
        if (Timer_Expired(&timer_init_hardware))
          return (SD_BEZIG);
        goto sd_init_hardware_label_20;  
      }
sd_init_hardware_label_10:
    case 10:
      if (SD_Send_Command(CMD58, 0, ocr) != 1) 
      {
        SD_CARD_CS = 1; // CS = L
        sd_init_hardware_state = 0;
        return (SD_ERROR);
      }  
      if ((ocr[1] & 0x78) != 0x78) // voltage niet in range van 3,1 tot en met 3,5 volt  
      {
        SD_CARD_CS = 1; // CS = L
        sd_init_hardware_state = 0;
        return (SD_ERROR);
      }  
      sd_init_hardware_state = 11;
      return (SD_BEZIG);
    case 11:
      do
      {  
        if (SD_Send_Command(CMD55, 0, ocr) > 1)
        {
          SD_CARD_CS = 1; // CS = L
          sd_init_hardware_state = 0;
          return (SD_ERROR);
        }  
      }
      while ((SD_Send_Command(ACMD41, 1UL << 30, ocr) != 0) && (Timer_Expired(&timer_init_hardware) == 0));
      if (Timer_Expired(&timer_init_hardware))
        return (SD_BEZIG);
      sd_init_hardware_state = 12;
      if (Timer_Expired(&timer_init_hardware))
        return (SD_BEZIG);
    case 12:    
      if (SD_Send_Command(CMD58, 0, ocr) != 0)
      {
        SD_CARD_CS = 1; // CS = L
        sd_init_hardware_state = 0;
        return (SD_ERROR);
      }  
      if ((ocr[0] & 0x80) == 0)
      {
        SD_CARD_CS = 1; // CS = L
        sd_init_hardware_state = 0;
        return (SD_ERROR);
      }  
      if (ocr[0] & 0x40)
      { // hoge capaciteit sd kaart
        sd_blok_adressering = 1;
      }
      else
      { // lage capaciteit sd kaart
        sd_blok_adressering = 0;
      }
      sd_init_hardware_state = 30;
      if (Timer_Expired(&timer_init_hardware))
        return (SD_BEZIG);
      goto sd_init_hardware_label_30;  
sd_init_hardware_label_20:
    case 20:
      if (SD_Send_Command(CMD58, 0, ocr) != 1)
      {
        SD_CARD_CS = 1; // CS = L
        sd_init_hardware_state = 0;
        return (SD_ERROR);
      }  
      if ((ocr[1] & 0x78) != 0x78) // voltage niet in range van 3,1 tot en met 3,5 volt  
      {
        SD_CARD_CS = 1; // CS = L
        sd_init_hardware_state = 0;
        return (SD_ERROR);
      }  
      sd_init_hardware_state = 21;
      return (SD_BEZIG);
    case 21:    
      do
      {  
        if (SD_Send_Command(CMD55, 0, ocr) > 1)
        {
          SD_CARD_CS = 1; // CS = L
          sd_init_hardware_state = 0;
          return (SD_ERROR);
        }  
      }
      while ((SD_Send_Command(ACMD41, 0, ocr) != 0) && (Timer_Expired(&timer_init_hardware) == 0));
      if (Timer_Expired(&timer_init_hardware))
        return (SD_BEZIG);
      sd_blok_adressering = 0;
      sd_init_hardware_state = 30;
      if (Timer_Expired(&timer_init_hardware))
        return (SD_BEZIG);
sd_init_hardware_label_30:
    case 30:
      // sd kaart power up routine gereed     
      if (SD_Send_Command(CMD16, 512, ocr) != 0)
      {
        SD_CARD_CS = 1; // CS = L
        sd_init_hardware_state = 0;
        return (SD_ERROR); 
      }  
      sd_status &= ~STA_NOINIT; // Clear STA_NOINIT
      SD_CARD_CS = 1; // CS = L
      sd_read_csd_state = 0;
      sd_init_hardware_state = 31;
    case 31:
      switch (SD_Read_Csd())
      {
        case SD_OK:
          break;
        case SD_ERROR:
          sd_init_hardware_state = 0;
          return (SD_ERROR);
        case SD_BEZIG:
          return (SD_BEZIG);
      }
      sd_init_hardware_state = 99;
    case 99:  
      return (SD_OK);  
  }
}

//*****************************************************************************
static unsigned char SD_Init_File_Systeem(void)
// return SD_OK, SD_ERROR of SD_BEZIG
{
static unsigned char error_cnt = 0;
static unsigned long bootsect = 0;
unsigned long fatsize;
unsigned long totalsect;
// volgende variabel zijn nodig om aantal vrije sectors te tellen
static unsigned long free_clust;
static unsigned long clust;
static unsigned long sect;
static unsigned char f;
static unsigned char *p;
s_timer timer_init_file_systeem;
int file_cnt;

  Timer_Set(&timer_init_file_systeem, 5, TIME_BASE_1_MSEC);
  switch (sd_init_file_systeem_state)
  {
    case 0:
      memset(&fs, 0, sizeof(FATFS)); // wis file systeem
      for (file_cnt = 0; file_cnt < MAX_FILES; file_cnt++)
      {
        log_file[file_cnt].file_info_is_read = 0;
        log_file[file_cnt].file_info = empty_file_info;
        log_file[file_cnt].second = 0;
        log_file[file_cnt].minute = 0;
        log_file[file_cnt].hour = 0;
        log_file[file_cnt].day = 1;
        log_file[file_cnt].month = 1;
        log_file[file_cnt].year = 1980;

        log_file[file_cnt].write_status = 0;
        log_file[file_cnt].rename_cnt = 0;
        log_file[file_cnt].rename_flag = 0;
        log_file[file_cnt].rename_status = 0;
        log_file[file_cnt].rename_aantal = 0;
        log_file[file_cnt].rename_oudste_cnt = 0;
        log_file[file_cnt].rename_oudste_fdate = 0;
        log_file[file_cnt].rename_oudste_ftime = 0;

        log_file[file_cnt].read_status = 0;
        log_file[file_cnt].read_flag = SD_READ_RESET;
      }  
      sd_init_file_systeem_state = 1;
      error_cnt = 0;
    case 1:  
      bootsect = 0;
      switch (SD_Read_Sector(fs.win, bootsect))
      {
        case SD_OK:
          break;
        case SD_ERROR:
          sd_init_state = 0;
          return (SD_ERROR);
        case SD_BEZIG:
          return (SD_BEZIG);
      }
      if (LD_WORD(&fs.win[BS_55AA]) != 0xAA55)
      {
        sd_init_file_systeem_state = 0;
        return (SD_ERROR);
      }  
      if (memcmp(&fs.win[BS_FilSysType], "FAT", 3) == 0) // FAT 12 en FAT 16 niet toegestaan
      {
        sd_init_file_systeem_state = 3;
        if (Timer_Expired(&timer_init_file_systeem))
          return (SD_BEZIG);
        goto sd_init_files_systeem_label_3;  
      }  
      if (memcmp(&fs.win[BS_FilSysType32], "FAT32", 5) == 0)
      {
        sd_init_file_systeem_state = 3;
        if (Timer_Expired(&timer_init_file_systeem))
          return (SD_BEZIG);
        goto sd_init_files_systeem_label_3;  
      }  
      sd_init_file_systeem_state = 2;
      if (Timer_Expired(&timer_init_file_systeem))
        return (SD_BEZIG);
    case 2:   
      if (fs.win[MBR_Table+4] == 0) // geen 1ste partitie aanwezig
      {
        sd_init_file_systeem_state = 0;
        return (SD_ERROR);
      }  
      bootsect = LD_DWORD(&fs.win[MBR_Table+8]);
      switch (SD_Read_Sector(fs.win, bootsect))
      {
        case SD_OK:
          break;
        default:  
        case SD_ERROR:
          sd_init_state = 0;
          return (SD_ERROR);
        case SD_BEZIG:
          return (SD_BEZIG);
      }
      if (LD_WORD(&fs.win[BS_55AA]) != 0xAA55)
      {
        sd_init_file_systeem_state = 0;
        return (SD_ERROR);
      }  
      if (memcmp(&fs.win[BS_FilSysType], "FAT", 3) == 0) // FAT 12 niet toegestaan FAT 16 wel
      {
        sd_init_file_systeem_state = 3;
      }  
      else if (memcmp(&fs.win[BS_FilSysType32], "FAT32", 5) == 0)
      {
        sd_init_file_systeem_state = 3;
      }  
      else 
      {
        sd_init_file_systeem_state = 0;
        return (SD_ERROR);
      }  
      if (Timer_Expired(&timer_init_file_systeem))
        return (SD_BEZIG);
sd_init_files_systeem_label_3:
    case 3:  
      if (LD_WORD(&fs.win[BPB_BytsPerSec]) != 512)
      {
        sd_init_file_systeem_state = 0;
        return (SD_ERROR);
      }
      fatsize = LD_WORD(&fs.win[BPB_FATSz16]);
      if (!fatsize)
        fatsize = LD_WORD(&fs.win[BPB_FATSz32]);
      fs.sects_fat = fatsize;
      fs.n_fats = fs.win[BPB_NumFATs]; // number of FAT copies
      fatsize *= fs.n_fats;           // (Number of sectors in FAT area)
      fs.fatbase = bootsect + LD_WORD(&fs.win[BPB_RsvdSecCnt]); // FAT start sector (lba)
      fs.sects_clust = fs.win[BPB_SecPerClus];          // Number of sectors per cluster
      fs.n_rootdir = LD_WORD(&fs.win[BPB_RootEntCnt]);  // Nmuber of root directory entries
      totalsect = LD_WORD(&fs.win[BPB_TotSec16]);
      if (!totalsect)
        totalsect = LD_DWORD(&fs.win[BPB_TotSec32]);
      fs.max_clust = (totalsect -             // Last cluster# + 1
                      LD_WORD(&fs.win[BPB_RsvdSecCnt]) - fatsize - fs.n_rootdir / 16) /
                     fs.sects_clust + 2;
      if (fs.max_clust < 0xFF7)               
      {
        sd_init_file_systeem_state = 0;
        return (SD_ERROR);
      }  
      fs.fs_type = FS_FAT16;
      if (fs.max_clust >= 0xFFF7)  
        fs.fs_type = FS_FAT32;
      if (fs.fs_type == FS_FAT32)
        fs.dirbase = LD_DWORD(&fs.win[BPB_RootClus]); // Root directory start cluster
      else  
        fs.dirbase = fs.fatbase + fatsize; // Root directory start cluster
      fs.database = fs.fatbase + fatsize + fs.n_rootdir / 16;     // Data start sector (lba)
      fs.free_clust = 0xFFFFFFFF;
      if (fs.fs_type == FS_FAT32)
      {
        fs.fsi_sector = bootsect + LD_WORD(&fs.win[BPB_FSInfo]);
        sd_init_file_systeem_state = 4;
        if (Timer_Expired(&timer_init_file_systeem))
          return (SD_BEZIG);
      }
      else
      {
        clust = fs.max_clust;
        sect = fs.fatbase;
        f = 0;
        p = 0;
        free_clust = 0;
        fs.id = ++sd_fsid;
        sd_init_file_systeem_state = 5;
        if (Timer_Expired(&timer_init_file_systeem))
          return (SD_BEZIG);
        goto sd_init_file_systeem_label_5;  
      }    
    case 4:
      switch (SD_Read_Sector(fs.win, fs.fsi_sector))
      {
        case SD_OK:
          break;
        case SD_ERROR:
          sd_init_state = 0;
          return (SD_ERROR);
        case SD_BEZIG:
          return (SD_BEZIG);
      }
      if (LD_WORD(&fs.win[BS_55AA]) == 0xAA55 &&
          LD_DWORD(&fs.win[FSI_LeadSig]) == 0x41615252 &&
          LD_DWORD(&fs.win[FSI_StrucSig]) == 0x61417272)
      {
        fs.last_clust = LD_DWORD(&fs.win[FSI_Nxt_Free]);
        fs.free_clust = LD_DWORD(&fs.win[FSI_Free_Count]);
      }
      if ((fs.free_clust == 0xFFFFFFFF)/* ||
          (fs.free_clust > fs.max_clust - 2)*/)
      {
        clust = fs.max_clust;
        sect = fs.fatbase;
        f = 0;
        p = 0;
        free_clust = 0;
        fs.id = ++sd_fsid;
        sd_init_file_systeem_state = 5;
        if (Timer_Expired(&timer_init_file_systeem))
          return (SD_BEZIG);
        goto sd_init_file_systeem_label_5;  
      }  
      else
      {
        fs.id = ++sd_fsid;
        sd_init_file_systeem_state = 7;
        if (Timer_Expired(&timer_init_file_systeem))
          return (SD_BEZIG);
        goto sd_init_file_systeem_label_7;  
      }    
sd_init_file_systeem_label_5:
    case 5: // inlezen vrije clusters
      do
      {
        if (!f)
        {
          switch (SD_Move_Window(sect))
          {
            case SD_OK:
              break;
            case SD_ERROR:
              sd_init_state = 0;
              return (SD_ERROR);
            case SD_BEZIG:
              return (SD_BEZIG);
          }
          if (Timer_Expired(&timer_init_file_systeem))
            return (SD_BEZIG);
          sect++;  
          p = fs.win;  
        }
        if (fs.fs_type == FS_FAT16)
        {
          if (LD_WORD(p) == 0)
            free_clust++;
          p += 2;
          f += 1;
        }
        else
        {
          if (LD_DWORD(p) == 0)
            free_clust++;
          p += 4;
          f += 2;
        }  
        clust--;
      }
      while (clust);
      fs.free_clust = free_clust;
      if (fs.fs_type == FS_FAT32)
        fs.fsi_flag = 1;
      sd_init_file_systeem_state = 6;
      if (Timer_Expired(&timer_init_file_systeem))
        return (SD_BEZIG);
    case 6: // sync free_clust
      if ((sd_status & STA_PROTECT) == 0)
      {  
        if ((fs.fs_type == FS_FAT32) && fs.fsi_flag)
        {
          fs.winsect = 0;
          memset(fs.win, 0, 512);
          ST_WORD(&fs.win[BS_55AA], 0xAA55);
          ST_DWORD(&fs.win[FSI_LeadSig], 0x41615252);
          ST_DWORD(&fs.win[FSI_StrucSig], 0x61417272);
          ST_DWORD(&fs.win[FSI_Free_Count], fs.free_clust);
          ST_DWORD(&fs.win[FSI_Nxt_Free], fs.last_clust);
          switch (SD_Write_Sector(fs.win, fs.fsi_sector))
          {
            case SD_OK:
              break;
            case SD_ERROR:
              sd_init_state = 0;
              return (SD_ERROR);
            case SD_BEZIG:
              return (SD_BEZIG);
          }
          fs.fsi_flag = 0;
        }  
      }  
      sd_init_file_systeem_state = 7;
      if (Timer_Expired(&timer_init_file_systeem))
        return (SD_BEZIG);
sd_init_file_systeem_label_7:
    case 7:
      SD_Card_Operation(F_RESET);
      sd_init_file_systeem_state = 99;
    case 99: // init gereed  
      return (SD_OK);
  }
  return (SD_BEZIG);
}

//*****************************************************************************

static unsigned char SD_Init(void)
// return SD_OK of SD_BEZIG
{
  switch (sd_init_state)
  {
    default:
    case 0:
      sd_init_state = 1;
      sd_init_hardware_state = 0;
    case 1:
      if (SD_Init_Hardware() == SD_OK)
      {
        sd_init_state = 2;
        sd_init_file_systeem_state = 0;
      }  
      break;
    case 2:
      if (SD_Init_File_Systeem() == SD_OK)
        sd_init_state = 99;
      break;
    case 99: 
      return (SD_OK);
  }
  return (SD_BEZIG);
}

//*****************************************************************************
static unsigned char SD_Get_Sector_Count(unsigned long *sector_count)
{
unsigned char n;
unsigned int csize;

  switch (csd.csd_structure)
  {
    default:
      return (SD_ERROR);
    case 0: // CSD ver 1.XX
      n = csd.read_bl_len + csd.c_size_mult + 2;
      csize = csd.c_size + 1;
      *sector_count = (unsigned long)csize << (n - 9);
      return (SD_OK);
    case 1:  // CSD ver 2.00
      csize = csd.c_size + 1;
      *sector_count = (unsigned long)csize << 10;
      return (SD_OK);
  }
}

static void SD_Get_Block_Size(unsigned long *block_size)
{
  *block_size = (csd.sector_size + 1) << (csd.write_bl_len - 1);
}

static unsigned char sd_mkfs_state = 0;
static unsigned char SD_F_Mkfs(unsigned int allocsize_new)       /* Allocation unit size [bytes] */
{
static const unsigned long sstbl[] = { 2048000, 1024000, 512000, 256000, 128000, 64000, 32000, 16000, 8000, 4000,   0 };
static const unsigned int cstbl[] =  {   32768,   16384,   8192,   4096,   2048, 16384,  8192,  4096, 2048, 1024, 512 };
static unsigned char m;
static unsigned long b_part, b_fat, b_dir, b_data;     /* Area offset (LBA) */
static unsigned long n_part, n_rsv, n_fat, n_dir;      /* Area size */
static unsigned long n_clst, n;
static unsigned int as;
static unsigned int n_cylinder; // JP 24-04-09
static unsigned int allocsize;
s_timer timer_f_mkfs;
s_partition_record *ptr_partition;
s_boot_record *ptr_boot_record;

  Timer_Set(&timer_f_mkfs, 5, TIME_BASE_1_MSEC);
  switch (sd_mkfs_state)
  {
    default:
      sd_mkfs_state = 0;
    case 0:
      memset(&fs, 0, sizeof(FATFS)); // wis file systeem
//      FatFs = &fs; // kan straks misschien weg als FatFs niet gebruitk wordt
      fs.fs_type = 0;
      allocsize = allocsize_new;
      sd_init_hardware_state = 0;
      sd_mkfs_state = 1;
    case 1:
      if (SD_Init_Hardware() != SD_OK)
        return (SD_BEZIG);
      if (sd_status & STA_PROTECT)
      {
        sd_mkfs_state = 0;
        return (SD_ERROR);
      }
      sd_mkfs_state = 2;
      if (Timer_Expired(&timer_f_mkfs))
        return (SD_BEZIG);
    case 2:  
      switch (SD_Get_Sector_Count(&n_part))
      {
        case SD_OK:
          break;
        case SD_ERROR:
          sd_mkfs_state = 0;
          return (SD_ERROR);
        case SD_BEZIG:
          return (SD_BEZIG);  
      }
      sd_mkfs_state = 3;
      if (Timer_Expired(&timer_f_mkfs))
        return (SD_BEZIG);
    case 3:  
      if (n_part < MIN_SECTOR)
      {
        sd_mkfs_state = 0;
        return (SD_ERROR);
      }
      if (n_part > MAX_SECTOR)
        n_part = MAX_SECTOR;
      b_part = 63;      // Boot sector 
      n_part -= b_part;
      if (!allocsize)
      { // Auto selection of cluster size 
        for (n = 0; n_part < sstbl[n]; n++);
        allocsize = cstbl[n];
      }
      for (as = 512; as <= 32768U && as != allocsize; as <<= 1);
      if (as != allocsize)
      {
        sd_mkfs_state = 0;
        return (SD_ERROR);
      }  
      allocsize /= (unsigned long)512; // Number of sectors per cluster
      // Pre-compute number of clusters and FAT type
      n_clst = n_part / allocsize;
      n_fat = ((n_clst * 4) + 8 + (unsigned long)512 - 1) / (unsigned long)512;
      n_rsv = 0x24 - 0;
      n_dir = 0;
      b_fat = b_part + n_rsv;         // FATs start sector
      b_dir = b_fat + n_fat * N_FATS; // Directory start sector
      b_data = b_dir + n_dir;         // Data start sector
      // Determine number of cluster and final check of validity of the FAT type 
      n_clst = (n_part - n_rsv - n_fat * N_FATS - n_dir) / allocsize;
        
      n_cylinder = (b_part + n_part) / 63 / 255;
      memset(fs.win, 0, (unsigned long)512); // JP 24-04-09
      ptr_partition = (s_partition_record *)(fs.win+MBR_Table);
      ptr_partition->status = 0x80;
      ptr_partition->CHS_first[0] = 0x01;
      ptr_partition->CHS_first[1] = 0x01;
      ptr_partition->CHS_first[2] = 0x00;
      ptr_partition->type = 0x0B;
      ptr_partition->CHS_last[0] = 0xFE;
      ptr_partition->CHS_last[1] = ((n_cylinder & 0x0300) >> 2) | 0x3F;
      ptr_partition->CHS_last[2] = n_cylinder & 0xFF;
      ST_DWORD(ptr_partition->LBA_first_sector,b_part);
      ST_DWORD(ptr_partition->number_of_blocks,n_part);
      ST_WORD(fs.win+BS_55AA, 0xAA55);        /* Signature */
      sd_mkfs_state = 4;
      if (Timer_Expired(&timer_f_mkfs))
        return (SD_BEZIG);
    case 4:
      switch (SD_Write_Sector(fs.win, 0))
      {
        case SD_OK:
          break;
        case SD_ERROR:
          sd_mkfs_state = 0;
          return (SD_ERROR);
        case SD_BEZIG:
          return (SD_BEZIG);
      }
      sd_mkfs_state = 5;
      if (Timer_Expired(&timer_f_mkfs))
        return (SD_BEZIG);
    case 5:    
      // Create boot record
      memset(fs.win, 0, (unsigned long)512);
      ptr_boot_record = (s_boot_record *)fs.win;
      ptr_boot_record->jmpBoot[0] = 0xEB;
      ptr_boot_record->jmpBoot[1] = 0x58;
      ptr_boot_record->jmpBoot[2] = 0x90;
      strncpy((char *)ptr_boot_record->OEMName, "MSDOS5.0", 8);
      ST_WORD(ptr_boot_record->BytsPerSec, 512);
      ptr_boot_record->SecPerClus = (unsigned char)allocsize;
      ST_WORD(ptr_boot_record->RsvdSecCnt, n_rsv); 
      ptr_boot_record->Num_Fasts = N_FATS;
      ST_WORD(ptr_boot_record->RootEntCnt, 0);
      ST_WORD(ptr_boot_record->TotSec16, 0);
      ptr_boot_record->Media = 0xF8;
      ST_WORD(ptr_boot_record->FATSz16, 0);
      ST_WORD(ptr_boot_record->SecPerTrk, 0x3F);
      ST_WORD(ptr_boot_record->NumHeads, 0xFF);
      ST_DWORD(ptr_boot_record->HiddSec, 0x3F);
      ST_DWORD(ptr_boot_record->TotSec32, n_part);
      ST_DWORD(ptr_boot_record->FATSz32, n_fat);
      ST_WORD(ptr_boot_record->ExtFlags, 0);
      ST_WORD(ptr_boot_record->FSVer, 0);
      ST_DWORD(&ptr_boot_record->RootClus, 2);
      ST_WORD(ptr_boot_record->FSInfo, 1);
      ST_WORD(ptr_boot_record->BkBootSec, 6);
      ptr_boot_record->DrvNum = 0;
      ptr_boot_record->Bootsig = 0x29;
      n = SD_Get_Fattime(); // Use current time as a VSN
      ST_DWORD(ptr_boot_record->VolID, n);
      strncpy((char *)ptr_boot_record->VolLab, "NO NAME    ", 11);
      strncpy((char *)ptr_boot_record->FilSysType, "FAT32   ", 8);
      ST_WORD(fs.win+BS_55AA, 0xAA55);               // Signature
      sd_mkfs_state = 6;
      if (Timer_Expired(&timer_f_mkfs))
        return (SD_BEZIG);
    case 6:
      switch (SD_Write_Sector(fs.win, b_part+0))
      {
        case SD_OK:
          break;
        case SD_ERROR:
          sd_mkfs_state = 0;
          return (SD_ERROR);
        case SD_BEZIG:
          return (SD_BEZIG);
      }
      sd_mkfs_state = 7;
      if (Timer_Expired(&timer_f_mkfs))
        return (SD_BEZIG);
    case 7:
      switch (SD_Write_Sector(fs.win, b_part+6))
      {
        case SD_OK:
          break;
        case SD_ERROR:
          sd_mkfs_state = 0;
          return (SD_ERROR);
        case SD_BEZIG:
          return (SD_BEZIG);
      }
      // Initialize FAT area
      m = 0;
      sd_mkfs_state = 8;
      if (Timer_Expired(&timer_f_mkfs))
        return (SD_BEZIG);
sd_mkfs_label_8:
    case 8: 
      if (m < N_FATS)
      {
        memset(fs.win, 0, (unsigned long)512); // 1st sector of the FAT 
        ST_DWORD(fs.win+0, 0xFFFFFFF8);        // Reserve cluster #0-1 (FAT32) 
        ST_DWORD(fs.win+4, 0xFFFFFFFF);
        ST_DWORD(fs.win+8, 0x0FFFFFFF);        // Reserve cluster #2 for root dir
        sd_mkfs_state = 9;
        goto sd_mkfs_label_9;
      } 
      // Initialize Root directory
      m = (unsigned char)allocsize;
      sd_mkfs_state = 11;
      goto sd_mkfs_label_11;
sd_mkfs_label_9:
    case 9:      
      switch (SD_Write_Sector(fs.win, b_fat))
      {
        case SD_OK:
          break;
        case SD_ERROR:
          sd_mkfs_state = 0;
          return (SD_ERROR);
        case SD_BEZIG:
          return (SD_BEZIG);
      }
      b_fat++;
      n = 1;
      memset(fs.win, 0, (unsigned long)512); // Following FAT entries are filled by zero
      sd_mkfs_state = 10;
      if (Timer_Expired(&timer_f_mkfs))
        return (SD_BEZIG);
sd_mkfs_label_10:
    case 10:
      if (n < n_fat)
      {
        switch (SD_Write_Sector(fs.win, b_fat))
        {
          case SD_OK:
            break;
          case SD_ERROR:
            sd_mkfs_state = 0;
            return (SD_ERROR);
          case SD_BEZIG:
            return (SD_BEZIG);
        }
        b_fat++;
        n++;
        if (Timer_Expired(&timer_f_mkfs))
          return (SD_BEZIG);
        goto sd_mkfs_label_10;  
      }
      m++;
      sd_mkfs_state = 8;
      goto sd_mkfs_label_8;
sd_mkfs_label_11:
    case 11:
      switch (SD_Write_Sector(fs.win, b_fat))
      {
        case SD_OK:
          break;
        case SD_ERROR:
          sd_mkfs_state = 0;
          return (SD_ERROR);
        case SD_BEZIG:
          return (SD_BEZIG);
      }
      b_fat++;
      m--;
      if (m)
      {
        if (Timer_Expired(&timer_f_mkfs))
          return (SD_BEZIG);
        goto sd_mkfs_label_11;  
      }
      // Create FSInfo record if needed 
      ST_WORD(fs.win+BS_55AA, 0xAA55);
      ST_DWORD(fs.win+FSI_LeadSig, 0x41615252);
      ST_DWORD(fs.win+FSI_StrucSig, 0x61417272);
      ST_DWORD(fs.win+FSI_Free_Count, n_clst - 1);
      ST_DWORD(fs.win+FSI_Nxt_Free, 0xFFFFFFFF);
      sd_mkfs_state = 12;
      if (Timer_Expired(&timer_f_mkfs))
        return (SD_BEZIG);
    case 12:    
      switch (SD_Write_Sector(fs.win, b_part+1))
      {
        case SD_OK:
          break;
        case SD_ERROR:
          sd_mkfs_state = 0;
          return (SD_ERROR);
        case SD_BEZIG:
          return (SD_BEZIG);
      }
      sd_mkfs_state = 13;
      if (Timer_Expired(&timer_f_mkfs))
        return (SD_BEZIG);
    case 13:
      switch (SD_Write_Sector(fs.win, b_part+7))
      {
        case SD_OK:
          break;
        case SD_ERROR:
          sd_mkfs_state = 0;
          return (SD_ERROR);
        case SD_BEZIG:
          return (SD_BEZIG);
      }
      sd_mkfs_state = 0;
      return (SD_OK);
  }
}

//*****************************************************************************
unsigned char format = 0;

void SD_Format_Start(void)
{
  format = 1;
}

unsigned char SD_Format_Ready(void)
{
  return (format == 0);
}

//*****************************************************************************

#ifdef EMULATOR
unsigned int sd_control_max = 0;
#endif // EMULATOR
void SD_Control(void)
{
static changed = 0;
static unsigned int adres;
unsigned char file_cnt = 0;
char file_naam[9];
#ifdef EMULATOR
s_timer timer_sd_control;
unsigned int sd_control;

  Timer_Set(&timer_sd_control, 30000, TIME_BASE_1_MSEC);
  #endif // EMULATOR
  SD_State();
  if (file_control_init_switch)
  {

    adres = opt_alg.adres;
    SD_File_Reset(&log_file[0],  LOG_FILE_0_NAME,  &setp_alg.sd_card_log_on[0],              0, TEKST_FILE_FORMAT, opt_alg.adres); // AL
    SD_File_Reset(&log_file[1],  LOG_FILE_1_NAME,  &setp_alg.sd_card_log_on[1],             20, LOG_FILE_FORMAT,   opt_alg.adres); // LOG

    strcpy(file_naam, LOG_FILE_2_NAME);
    switch (selection_opt_nr)
    {
      default: selection_opt_nr = 0;
      case 0:  strcat(file_naam,"A"); break;
      case 1:  strcat(file_naam,"I"); break;
      case 2:  strcat(file_naam,"P"); break;
    }
    SD_File_Reset(&log_file[2],  file_naam,        &setp_alg.sd_card_log_on[2], OPT_RENAME_MAX, HEX_FILE_FORMAT,   opt_alg.adres); // OPTA
    SD_File_Reset(&log_file[3],  LOG_FILE_3_NAME,  &setp_alg.sd_card_log_on[3],              0, HEX_FILE_FORMAT,   opt_alg.adres); // SETP
    SD_File_Reset(&log_file[4],  LOG_FILE_4_NAME,  &setp_alg.sd_card_log_on[4],             20, TEKST_FILE_FORMAT, opt_alg.adres); // MOT
    SD_File_Reset(&log_file[5],  LOG_FILE_5_NAME,  &setp_alg.sd_card_log_on[5],              0, TEKST_FILE_FORMAT, opt_alg.adres); // COM
    SD_File_Reset(&log_file[6],  LOG_FILE_6_NAME,  &setp_alg.sd_card_log_on[6],              0, TEKST_FILE_FORMAT, opt_alg.adres);
    SD_File_Reset(&log_file[7],  LOG_FILE_7_NAME,  &setp_alg.sd_card_log_on[7],              0, TEKST_FILE_FORMAT, opt_alg.adres);
    SD_File_Reset(&log_file[8],  LOG_FILE_8_NAME,  &setp_alg.sd_card_log_on[8],              0, TEKST_FILE_FORMAT, opt_alg.adres);
    SD_File_Reset(&log_file[9],  "",               &log_off,                                 0, TEKST_FILE_FORMAT, 0);             // LOG_READ
    if (changed == 2)
      setp_alg.sd_card_status = 0;
    changed = 0;
    file_control_init_switch = 0;
  }
  else
  {
    if (changed == 0)
    {
      changed = (opt_alg.adres != adres) ? 1 : 0;
      if (changed)
      {
        setp_alg.sd_card_log_on[LOG] = 0;
        switch (setp_alg.sd_card_status)
        {
          case 0:
            setp_alg.sd_card_status = 1;
            changed = 2;
          case 1: break;
          case 2: file_control_init_switch = 1; break;
        }
      }
    }
    else
    {
      if (setp_alg.sd_card_status == 2)
        file_control_init_switch = 1;
    }
    if ((sd_status & STA_NODISK) == 0)  // sd kaart aanwezig
    {
      if (format)
      {
        if (SD_F_Mkfs(4096) == SD_OK)
          format = 0;
        sd_init_state = 0;
      }
      else
      {
        if (SD_Init() == SD_OK) // sd kaart is geinitialiseerd
        {
          SD_Log_Control();
          SD_Files_Control();
          SD_Read_Write_Options();
          SD_Read_Write_Setpoints();
          #ifdef SD_MANAGEMENT
          SD_Read_Write_Management();
          #endif // SD_MANAGEMENT
        }
      }  
    }
    else
    {
      read_write_options = 0;
      read_write_setpoints = 0;
      *log_file[OPT].ptr_log_on = 0;
      *log_file[SETP].ptr_log_on = 0;
      #ifdef SD_MANAGEMENT
      read_write_management = 0;
      *log_file[MAN].ptr_log_on = 0;
      #endif // SD_MANAGEMENT
      disk_space = 0;
      free_disk_space = 0;
      sd_init_state = 0; // reset sd init
      sd_status |= STA_NOINIT;
      SD_CARD_CS = 1; // CS = H
      SD_Receive_Byte(); // Idle (Release DO)
    }  
  }  
  #ifdef EMULATOR
  sd_control = Timer_Delay(&timer_sd_control);
  if (sd_control_max < sd_control)
    sd_control_max = sd_control;
  #endif // EMULATOR
}

#endif // SD_CARD
