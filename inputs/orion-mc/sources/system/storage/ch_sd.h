// CH_SD.H

#ifndef _CH_SD_H
#define _CH_SD_H

#ifdef SD_CARD

#include <stdarg.h>

#define MAX_FILE_BUFFER 1024

// Disk Status Bits (DSTATUS)
#define STA_NOINIT		((unsigned char)0x01)	/* Drive not initialized */
#define STA_NODISK		((unsigned char)0x02)	/* No medium in the drive */
#define STA_PROTECT		((unsigned char)0x04)	/* Write protected */

#define SD_OVERHEAD 8

#define TEKST_FILE_FORMAT 0
#define HEX_FILE_FORMAT 1
#define LOG_FILE_FORMAT 2

// File system object structure
typedef struct _FATFS
{
  unsigned int  id;          // File system mount ID   
  unsigned int  n_rootdir;   // Number of root directory entries
  unsigned long winsect;     // Current sector appearing in the win[]
  unsigned long fatbase;     // FAT start sector
  unsigned long dirbase;     // Root directory start sector
  unsigned long database;    // Data start sector
  unsigned long sects_fat;   // Sectors per fat
  unsigned long max_clust;   // Maximum cluster# + 1
  unsigned long last_clust;  // Last allocated cluster
  unsigned long free_clust;  // Number of free clusters
  unsigned long fsi_sector;  // fsinfo sector
  unsigned char fsi_flag;    // fsinfo dirty flag (1:must be written back)
  unsigned char pad1;
  unsigned char fs_type;     // FAT sub type
  unsigned char sects_clust; // Sectors per cluster
  unsigned char n_fats;      // Number of FAT copies
  unsigned char winflag;     // win[] dirty flag (1:must be written back)
  unsigned char win[512];    // Disk access window for Directory/FAT/File
} FATFS;

typedef struct _FIL
{
  unsigned int   id;         // Owner file system mount ID   
  unsigned char  flag;       // File status flags   
  unsigned char  sect_clust; // Left sectors in cluster
  FATFS*  fs;                // Pointer to owner file system
  unsigned long  fptr;       // File R/W pointer
  unsigned long  fsize;      // File size
  unsigned long  org_clust;  // File start cluster
  unsigned long  curr_clust; // Current cluster
  unsigned long  curr_sect;  // Current sector
  unsigned long  dir_sect;   // Sector containing the directory entry
  unsigned char* dir_ptr;    // Ponter to the directory entry in the window
} FIL;

typedef struct _FILINFO
{
  unsigned long fsize;   // Size
  unsigned int fdate;    // Date
  unsigned int ftime;    // Time
  unsigned char fattrib; // Attribute
  char fname[8+1+3+1];   // Name (8.3 format)
} FILINFO;

typedef struct
{
  unsigned char data[MAX_FILE_BUFFER];
  unsigned int cnt;
  unsigned int get_index;
  unsigned int put_index;
  unsigned char checksum;
} s_buffer;

typedef struct
{
  unsigned char used; // 0 as file not used, 1 as file used
  unsigned char *ptr_log_on;
  s_buffer write;
  unsigned char file_info_is_read; 
  unsigned char write_status;
  unsigned int write_time_out; // indien write time out en data om te schrijven kleiner dan WRITE_BUFFER_START dan toch wegschrijven
  unsigned int write_cnt_max; // alleen om te testen daarna weer verwijderen
  unsigned char second;
  unsigned char minute;
  unsigned char hour;
  unsigned char day;
  unsigned char month;
  unsigned int  year;
  FILINFO file_info;
  FIL file_write;
  unsigned int  adres; // adres computer (zorgt er voor dat extensie nummer gelijk is aan adres computer)
  char file_name_base[5]; // filenaam zonder nummer count en zonder adres
  char file_name[8+1+3+1+20];
  unsigned char reset_file_naam;
  FIL file_read; // na aflopp controleren of file_read en file_write door file vervangen kunnen worden
  unsigned int rename_cnt;
  unsigned int rename_cnt_max; // maximum files bij rename (als 10 dan wordt na 10 files de eerste weer overschreden
  unsigned char rename_flag; // 0 doe niets, 1 rename aangevraagd maar eerst data wegschrijven; 2 rename kan nu gebeuren
  unsigned char rename_status;
  unsigned int rename_aantal; // JP 29-04-09
  unsigned int rename_oudste_cnt; // JP 29-04-09
  unsigned int rename_oudste_fdate; // JP 29-04-09
  unsigned int rename_oudste_ftime; // JP 29-04-09
  s_buffer read;
  unsigned char read_flag;
  unsigned char read_status;
  unsigned long read_fptr; // File R pointer
  unsigned int read_time_out; // indien write time out en data om te schrijven kleiner dan WRITE_BUFFER_START dan toch wegschrijven
  unsigned char hex; // 0 als tekst file, 1 als hex file (opt, setp, man)
} s_file;

extern volatile unsigned char sd_status;
extern s_file log_file[MAX_FILES];
extern unsigned char sd_placed_disp;
extern unsigned char sd_write_protect_disp;
extern unsigned long disk_space;
extern unsigned long free_disk_space;
extern unsigned char read_write_options; // 0 = niets, 1 = write, 2 = read
extern unsigned char read_write_setpoints; // 0 = niets, 1 = write, 2 = read
#ifdef SD_MANAGEMENT
extern unsigned char read_write_management; // 0 = niets, 1 = write, 2 = read
#endif // SD_MANAGEMENT

// use define instead of &log_file[?] in File_Printf command
#define ALARM      0
#define LOG        1
#define OPT        2
#define SETP       3
#define F_MOTOR    4
#define COM        5
#define F_DUMMY_2  6
#define F_DUMMY_3  7
#define F_DUMMY_4  8
//#define F_DUMMY_5  9
#define LOG_READ   9

#define ALARM_FILE   &log_file[ALARM]
#define LOG_FILE     &log_file[LOG]
#define OPT_FILE     &log_file[OPT]
#define SETP_FILE    &log_file[SETP]
#define MOTOR_FILE   &log_file[F_MOTOR]
#define COM_FILE     &log_file[COM]
#define DUMMY_FILE_2 &log_file[F_DUMMY_2]
#define DUMMY_FILE_3 &log_file[F_DUMMY_3]
#define DUMMY_FILE_4 &log_file[F_DUMMY_4]
//#define DUMMY_FILE_5 &log_file[F_DUMMY_5]
#define READ_FILE    &log_file[LOG_READ]

#define LOG_FILE_0_NAME  "AL"
#define LOG_FILE_1_NAME  "LOG"
#define LOG_FILE_2_NAME  "OPT"
#define LOG_FILE_3_NAME  "SETP"
#define LOG_FILE_4_NAME  "MOT"
#define LOG_FILE_5_NAME  "COM"
#define LOG_FILE_6_NAME  ""
#define LOG_FILE_7_NAME  ""
#define LOG_FILE_8_NAME  ""
#define LOG_FILE_9_NAME  ""

#define SD_READ_READY 0
#define SD_READ_START 1
#define SD_READ_BEZIG 2
#define SD_READ_STOP  3
#define SD_READ_RESET 4

#define SD_DIRECTORY_READY 0
#define SD_DIRECTORY_START 1
#define SD_DIRECTORY_BEZIG 2
#define SD_DIRECTORY_STOP  3
#define SD_DIRECTORY_RESET 4

extern char sd_directory_path[50];
extern unsigned char sd_directory_flag;
extern s_buffer sd_directory_buffer;

void File_Printf(s_file *file_ptr, const char *format, ... );
void File_Rename(s_file *ptr);

void File_Put_Char(s_file *ptr, char ch);
unsigned char Read_Buffer_Byte(s_buffer *buffer);
unsigned char Read_Buffer_Char(s_buffer *buffer);
unsigned char Read_Buffer_Char_Checksum(s_buffer *buffer);
void File_Send_Char(s_file *ptr, unsigned char c);
void File_Send_Char_Checksum(s_file *ptr, unsigned char c);
void File_Send_Int_Checksum(s_file *ptr, unsigned int i);
void File_Send_Long_Checksum(s_file *ptr, unsigned long l);
void SD_Format_Start(void);
unsigned char SD_Format_Ready(void);
void SD_Directory_Read_Start(char *path);
unsigned char Buffer_Get_Byte(s_buffer *ptr);
void SD_File_Read_Start(char *file_name, unsigned char hex);
unsigned char SD_File_Delete_Ready(void);
void SD_File_Delete_Start(char *file_name);

void SD_Second_Control(void);
void SD_Control(void);

#endif // SD_CARD

#endif // _CH_SD_H