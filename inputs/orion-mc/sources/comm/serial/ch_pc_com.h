// CH_PC_COM.H

#ifndef _CH_PC_COM_H
#define _CH_PC_COM_H

#include "ct_data.h"
#include "ct_pc_com.h"

// READ altijd even
// WRITE altijd oneven
#define NOCOMMAND               0
#define SHUTUPCOMMAND           1
#define CONFIGURATIONREAD       2

#define TIMEWRITE               3 

#define OPTIONS_APP_READ        4
#define OPTIONS_APP_WRITE       5

#define MAINGROUPREAD           6
#define MAINGROUPWRITE          7

#define TEKST_MC_READ          10
#define TEKST_MC_WRITE         11

#define OPTIONS_ALG_READ       12
#define OPTIONS_ALG_WRITE      13

#define ALLOPTIONSREAD         14
#define ALLOPTIONSWRITE        15
#define ALLSETPOINTSREAD       16
#define ALLSETPOINTSWRITE      17
#define ALARMREAD              18 
#define ALARMWRITE             19

#define STATUS_READ            20
#define STATUS_WRITE           21
#define MOTOR_MANAGEMENT_READ  22
#define MOTOR_MANAGEMENT_WRITE 23

#define MOTORGROUP_READ        30
#define MOTORGROUP_WRITE       31

#define OPTIONS_IO_READ        40
#define OPTIONS_IO_WRITE       41

#define CAPTURESCREEN 92
#define RECEIVEKEY 93
#define CAPTURESCREENNOTAB 94
#define CAPTURESCREENFAST 96

#define MAINGROUPCHANGED  100
#define BLACKBOXMAINGROUPREAD 102

#define MODULEREAD       900
#define MODULEWRITE      901
#define TEKSTREAD        902
#define TEKSTWRITE       903
#define TEKSTINSTREAD    904
#define TEKSTINSTWRITE   905
#define TEKSTVERSIEREAD  906
#define TEKSTREAD_0      908
#define TEKSTINSTREAD_0  910
#define TEKSTREAD_1      912
#define TEKSTINSTREAD_1  914
#define TEKSTREAD_2      916
#define TEKSTINSTREAD_2  918
#define TEKSTREAD_3      920 
#define TEKSTINSTREAD_3  922
#define ALARMTEKSTREAD   924

#define FAN_DATA_READ  926
#define FAN_DATA_WRITE 927

#define OPTIONS_IO_APP_READ  928
#define OPTIONS_IO_APP_WRITE 929

#define XML_READ         978
#define XML_WRITE        979

#define SD_DIRECTORY_READ   980
#define SD_FILE_READ        982
#define SD_LOG_CONFIG_WRITE 985    
#define SD_FILE_DELETE      986
#define SD_FORMAT           988
#define SD_STATUS_WRITE     989

#define MEM_DUMP_READ   990
#define MEM_DUMP_WRITE  991
#define VAL_DEBUG_READ  992
#define VAL_DEBUG_WRITE 993

#define SUBCOMMANDBEGIN 0
#define SUBCOMMANDPREV  1
#define SUBCOMMANDNEXT  2
#define SUBCOMMANDEND   5

// wordt gebruikt om geen 
#define ERROR_NO               0
#define ERRORCHECKSUM          1
#define ERRORORIONADRES        (2+0x80)
#define ERRORPCADRES           (3+0x80)
#define ERRORCOMMANDUNKNOWN    (4+0x80)
#define ERRORSUBCOMMANDEND     (5+0x80)
#define ERRORSUBCOMMANDUNKNOWN (6+0x80)
#define ERRORLENGTE            (7+0x80)
#define ERRORADRESBLACKBOX	   (8+0x80)
#define ERRORNOTETXORETB       (9+0x80)
#define ERRORBLOKCNT           (10+0x80)
#define ERROROLDMODULEASK      (11+0x80)
// options of setpoints worden momenteel via andere pc terug geschreven
#define ERRORRESTOREOPTIONSETPOINTBUSY (12+0x80)
// gegevens van verkeerde sectie worden gevraagd
#define ERRORSECTION           (13+0x80)
#define ERRORSDTIMEOUT         (14+0x80)
#define ERRORSDFILES           (15+0x80)

#define MAINGROUPBLOCKED 10
// zelfstandig versturen blokkeren voor 0.1*xxx seconden
#define COMMANDORIONERRORMAX 5
#define COMMANDORIONTIMEOUT 50
#define COMMANDPCTIMEOUT 50
// timer die afloopt en telkens als deze aflopen is de maingroup verzend
#define MAINGROUPTIME 600

#define BUFFERSIZE 1024

typedef union
{
  char opt_alg_ch[sizeof(s_opt_alg)];
  char opt_app_ch[sizeof(s_opt_app)];
  char opt_io_ch[sizeof(s_opt_io)];
  char setp_alg_ch[sizeof(s_setp_alg)];
  char opt_io_app_ch[sizeof(s_opt_io) + sizeof(s_opt_app)];
} u_buffer_restore_data;
#define MAX_BUFFER_RESTORE_DATA (sizeof(u_buffer_restore_data))
extern unsigned char restore_data_buffer[MAX_BUFFER_RESTORE_DATA];
#define TIME_OUT_RESTORE_DATA_BUSY 10
extern unsigned char restore_data_busy;

#ifdef SD_CARD
#define TIME_OUT_READ_FILE_BUSY 10
extern unsigned char read_data_file_busy;
#endif // SD_CARD

typedef struct
{
  int code;
  int index_or_value;
  time_t on;
  time_t off;
} s_alarm_pc_com;

typedef union
{
  s_alarm_pc_com al[ALARM_DISP_VALUES]; 
  char lcd_ch[128][30];
} u_buffer_com;
#define MAX_BUFFER_COM (sizeof(u_buffer_com)+100)

typedef struct
{
  unsigned char *ptr_receive_buffer;   // pointer naar tussen buffer voor data (opties, setpoints terugzetten)
  unsigned int command; 			   // commando van pc dat actief is
  int command_timeout;                 // timeout counter voor commando van pc
  unsigned char error_cnt;             // counter voor maximaal aantal maal proberen communicatie
  unsigned char blok_cnt;              // help variabele om commando's reentrant te houden teller dier per verzonden blok wordt opgehoogd
  int index;                           // help variabele om commando's reentrant te houden index van huidige te verzenden blok data
  int old_index;					   // help variabele om commando's reentrant te houden index van vorige verzonden blok data
  int cnt;                             // help variabele om commando's reentrant te houden teller om aantal bij te houden
  unsigned char *ptr;              // help variabele om commando's reentrant te houden ptr van huidige te verzenden blok data
  unsigned char *old_ptr;          // help variabele om commando's reentrant te houden ptr van vorige verzonden blok data
                                       // old_ptr wordt bij overzenden lcd scherm gebruikt om gebruike om laatste databyte van lcd scherm aan te geven    
  unsigned char fast; // 1 if fast sdo else 0
} s_pc_com;

extern unsigned char pc_0_read_configuration_alg;
extern unsigned char pc_1_read_configuration_alg;
extern unsigned char pc_2_read_configuration_alg;
extern unsigned char rs232_0_read_configuration_alg;
extern unsigned char rs232_1_read_configuration_alg;
#ifdef ETHERNET
extern unsigned char ethernet_read_configuration_alg[4];
#endif // ETHERNET

void PC_Main_Group_Data(s_pc_com *pc_com);
void Verwerk_Main_Group_Changed(s_pc_com *pc_com, char *answer_buffer);
char Controleer_Data(int *command, char *command_buffer, char *answer_buffer);
void PC_Command_Not_Possible(void);
void PC_Command_Old_Config_Read(void);
void Verwerk_Data(s_pc_com *pc_com);

#endif
