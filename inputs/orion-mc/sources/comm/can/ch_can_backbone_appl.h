// CH_CAN_BACKBONE_APPL.H

#ifndef __CH_CAN_BACKBONE_APPL_H
#define __CH_CAN_BACKBONE_APPL_H

#include <time.h>

#include "ch_define.h"
#include "ch_can_backbone.h"

#define MAX_NR_INTERNAL_NODES 1   // maximum number of internal nodes ("computers")

#define MAX_PC_BUFFER (512+20)
#define MAX_PC_PACKED_BUFFER ((MAX_PC_BUFFER-7)/2)

#define TRANSMIT_ALARM_REPEAT (PULSES_PER_SECOND*10)
#define RECEIVE_ALARM_TIMEOUT (TRANSMIT_ALARM_REPEAT*3)
#define RECEIVE_SLAVE_TIMEOUT (RECEIVE_ALARM_TIMEOUT)
#define TRANSMIT_VALUE_REPEAT (PULSES_PER_SECOND*10)
#define RECEIVE_VALUE_TIMEOUT (TRANSMIT_VALUE_REPEAT*3)

#define CAN_BACKBONE_OUTSIDETEMP 0
#define CAN_BACKBONE_CLOCK_IN_SEC 1
#define CAN_BACKBONE_OUTSIDE_HUMIDITY 2
#define CAN_BACKBONE_WIND_SPEED 3
#define CAN_BACKBONE_WIND_DIRECTION 4
// the next define is only used in the old can backbone
#define CAN_BACKBONE_SOFTWARE_NUMBER 5
#define CAN_BACKBONE_SUNLIGHT 6
#define CAN_BACKBONE_RAIN 7
#define CAN_BACKBONE_ASK_FWS 8
#define CAN_BACKBONE_ASK_OPT 9

typedef struct
{
  s_can_backbone_master master;
  s_can_backbone_slave slave;
  unsigned char can_slave_message_ready; // 1 if message is ready else 0
  char *can_slave_buffer;
  char *can_master_buffer;
} s_can_backbone_master_slave;

typedef struct
{
  s_can_backbone_node node;

  s_can_backbone_receive_value receive_value; // for receiving every value
  s_can_backbone_receive_alarm receive_alarm;
  s_can_backbone_transmit_alarm transmit_alarm;

  s_can_backbone_master_slave *pc_0;

  s_can_backbone_master_slave *pc_1;
  
  s_can_backbone_master_slave *pc_2;
  
  s_can_backbone_master_slave *fws;

  s_can_backbone_master_slave *opt;
  
//  s_can_backbone_master master_pc_0;
//  s_can_backbone_slave slave_pc_0;
//  unsigned char can_slave_pc_0_message_ready; // 1 if message is ready else 0
//  char *can_slave_pc_0_buffer_ptr; // memory needed by can to receive message
//  char *can_master_pc_0_buffer_ptr; // memory needed by can to transmit message

//  s_can_backbone_master master_pc_1;
//  s_can_backbone_slave slave_pc_1;
//  unsigned char can_slave_pc_1_message_ready; // 1 if message is ready else 0
//  char *can_slave_pc_1_buffer_ptr; // memory needed by can to receive message
//  char *can_master_pc_1_buffer_ptr; // memory needed by can to transmit message

//  s_can_backbone_master master_fws;
//  s_can_backbone_slave slave_fws;
//  unsigned char can_slave_fws_message_ready; // 1 if message is ready else 0
//  char *can_slave_fws_buffer_ptr; // memory needed by can to receive message
//  char *can_master_fws_buffer_ptr; // memory needed by can to transmit message
  
//  s_can_backbone_master master_opt;
//  s_can_backbone_slave slave_opt;
//  unsigned char can_slave_opt_message_ready; // 1 if message is ready else 0
//  char *can_slave_opt_buffer_ptr; // memory needed by can to receive message
//  char *can_master_opt_buffer_ptr; // memory needed by can to transmit message
  
  unsigned int receive_alarm_address;  // address from the computer where teh alarm is
  unsigned int receive_alarm_code;     // hold last incoming alarm code (not 0) is 0 after x time no alarm
  unsigned int receive_alarm_value;    // alarm_value by alarm_code
  unsigned int receive_computer_type;  // computer_type by alarm_code
  unsigned int receive_computer_soort; // computer_soort by alarm_code
  int receive_alarm_time_cnt;
  int receive_alarm_time_out_max;

  int outsidetemp;      // 0 (received or transmit)
  unsigned char outsidetemp_ok;
  unsigned char outsidetemp_time_reset; // set in interrupt to reset time_cnt
  int outsidetemp_time_cnt;
  int outsidetemp_time_out_max;

//  time_t clock_in_sec;  // 1 for synchronistation of time (received or transmit)
  unsigned char clock_in_sec_time_reset; // set in interrupt to reset time_cnt
  int clock_in_sec_time_cnt;
  int clock_in_sec_time_out_max;

  int outside_humidity; // 2
  unsigned char outside_humidity_ok;
  unsigned char outside_humidity_time_reset; // set in interrupt to reset time_cnt
  int outside_humidity_time_cnt; // 2
  int outside_humidity_time_out_max;

  int wind_speed;       // 3 (received or transmit)
  unsigned char wind_speed_ok;
  unsigned char wind_speed_time_reset; // set in interrupt to reset time_cnt
  int wind_speed_time_cnt;       // 3 (received or transmit)
  int wind_speed_time_out_max;

  int wind_direction;   // 4
  unsigned char wind_direction_ok;
  unsigned char wind_direction_time_reset; // set in interrupt to reset time_cnt
  int wind_direction_time_cnt;   // 4
  int wind_direction_time_out_max;
/*
  int fws_ask;      // 0 (received or transmit)
  unsigned char fws_ask_ok;
  unsigned char fws_ask_time_reset; // set in interrupt to reset time_cnt
  int fws_ask_time_cnt;
  int fws_ask_time_out_max;
*/
/*
  int opt_ask;      // 0 (received or transmit)
  unsigned char opt_ask_ok;
  unsigned char opt_ask_time_reset; // set in interrupt to reset time_cnt
  int opt_ask_time_cnt;
  int opt_ask_time_out_max;
*/
} s_can_backbone_appl_node;

extern unsigned char can_backbone_appl_init_switch;
extern s_can_backbone_appl_node can_backbone_appl_node_alg;

extern s_can_backbone_transmit_value can_backbone_transmit_outsidetemp;
extern s_can_backbone_transmit_value can_backbone_transmit_clock_in_sec;
extern s_can_backbone_transmit_value can_backbone_transmit_outside_humidity;
extern s_can_backbone_transmit_value can_backbone_transmit_wind_speed;
extern s_can_backbone_transmit_value can_backbone_transmit_wind_direction;
extern s_can_backbone_transmit_value can_backbone_transmit_fws_ask;
extern s_can_backbone_transmit_value can_backbone_transmit_opt_ask;

void Can_Backbone_Appl_Init(void);
void Can_Backbone_Appl_Control(void);
void Can_Backbone_Appl_Timing_Control(void);

//-----------------------------------------------------------------------------
void Can_Backbone_Set_Master_PC_0_And_Send(s_can_backbone_appl_node *appl_node_ptr, 
                                           char *buffer_ptr);
void Can_Backbone_Set_Master_PC_1_And_Send(s_can_backbone_appl_node *appl_node_ptr, 
                                           char *buffer_ptr);
void Can_Backbone_Set_Master_PC_2_And_Send(s_can_backbone_appl_node *appl_node_ptr, 
                                           char *buffer_ptr);
void Can_Backbone_Set_Alarm(s_can_backbone_transmit_alarm *transmit_alarm_ptr, 
                            unsigned int alarm_code,
                            unsigned int alarm_value,
                            unsigned int computer_type,
                            unsigned int computer_soort);
void Can_Backbone_Set_Alarm_And_Send(s_can_backbone_transmit_alarm *transmit_alarm_ptr,
                                     unsigned int alarm_code,
                                     unsigned int alarm_value,
                                     unsigned int computer_type,
                                     unsigned int computer_soort);
void Can_Backbone_Get_Alarm(s_can_backbone_appl_node *appl_node_ptr, 
                            unsigned int *alarm_code,
                            unsigned int *alarm_value,
                            unsigned int *computer_type,
                            unsigned int *computer_soort);

void Can_Backbone_Set_Outsidetemp(s_can_backbone_transmit_value *transmit_value_ptr, int new);
void Can_Backbone_Set_Outsidetemp_And_Send(s_can_backbone_transmit_value *transmit_value_ptr, int new);
unsigned char Can_Backbone_Get_Outsidetemp(s_can_backbone_appl_node *appl_node_ptr, unsigned int *actual);

void Can_Backbone_Transmit_Clock_In_Sec_Func(s_can_backbone_transmit_value *transmit_value_ptr, unsigned char ok);
void Can_Backbone_Set_Clock_In_Sec(s_can_backbone_transmit_value *transmit_value_ptr, time_t new);
void Can_Backbone_Set_Clock_In_Sec_And_Send(s_can_backbone_transmit_value *transmit_value_ptr, time_t new);
//unsigned char Can_Backbone_Get_Clock_In_Sec(s_can_backbone_appl_node *appl_node_ptr, time_t *actual);

void Can_Backbone_Set_Outside_Humidity(s_can_backbone_transmit_value *transmit_value_ptr, int new);
void Can_Backbone_Set_Outside_Humidity_And_Send(s_can_backbone_transmit_value *transmit_value_ptr, int new);
unsigned char Can_Backbone_Get_Outside_Humidity(s_can_backbone_appl_node *appl_node_ptr, unsigned int *actual);

void Can_Backbone_Set_Wind_Speed(s_can_backbone_transmit_value *transmit_value_ptr, int new);
void Can_Backbone_Set_Wind_Speed_And_Send(s_can_backbone_transmit_value *transmit_value_ptr, int new);
unsigned char Can_Backbone_Get_Wind_Speed(s_can_backbone_appl_node *appl_node_ptr, unsigned int *actual);

void Can_Backbone_Set_Wind_Direction(s_can_backbone_transmit_value *transmit_value_ptr, int new);
void Can_Backbone_Set_Wind_Direction_And_Send(s_can_backbone_transmit_value *transmit_value_ptr, int new);
unsigned char Can_Backbone_Get_Wind_Direction(s_can_backbone_appl_node *appl_node_ptr, unsigned int *actual);

void Can_Backbone_Set_FWS_Ask(s_can_backbone_transmit_value *transmit_value_ptr, int new);
void Can_Backbone_Set_FWS_Ask_And_Send(s_can_backbone_transmit_value *transmit_value_ptr, int new);
//unsigned char Can_Backbone_Get_FWS_Ask(s_can_backbone_appl_node *appl_node_ptr, unsigned int *actual);

void Can_Backbone_Set_OPT_Ask(s_can_backbone_transmit_value *transmit_value_ptr, int new);
void Can_Backbone_Set_OPT_Ask_And_Send(s_can_backbone_transmit_value *transmit_value_ptr, int new);
//unsigned char Can_Backbone_Get_OPT_Ask(s_can_backbone_appl_node *appl_node_ptr, unsigned int *actual);

void Can_Backbone_Address_Check(void);

#endif // __CH_CAN_BACKBONE_APPL_H

