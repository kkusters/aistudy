// CH_CAN_BACKBONE.H

#ifndef __CH_CAN_BACKBONE_H
#define __CH_CAN_BACKBONE_H

#include "ch_can_backbone_hardware.h"

#define RECEIVE_VALUE_ERROR 0
#define RECEIVE_VALUE_WAIT 1
#define RECEIVE_VALUE_OK 2
#define RECEIVE_ALARM_ERROR 0
#define RECEIVE_ALARM_OK 1

#define TRANSMIT_ALARM_BLOCKED 1
#define TRANSMIT_VALUE_BLOCKED 1

// some forward references needed for compiler
//typedef struct s_can_backbone_node s_can_backbone_node;
//typedef struct s_can_backbone_master s_can_backbone_master;
//typedef struct s_can_backbone_slave s_can_backbone_slave;
//typedef struct s_can_backbone_transmit_value s_can_backbone_transmit_value;
//typedef struct s_can_backbone_receive_value s_can_backbone_receive_value;
//typedef struct s_can_backbone_transmit_alarm s_can_backbone_transmit_alarm;
//typedef struct s_can_backbone_receive_alarm s_can_backbone_receive_alarm;

typedef struct ss_can_backbone_node
{
  unsigned int address_node; // node address of this node
                             // 0 node is not initialised
  // for each internal node we need alarm receive and transmit value's!
  struct ss_can_backbone_transmit_alarm *first_transmit_alarm_ptr;
  struct ss_can_backbone_receive_alarm *first_receive_alarm_ptr;

  // define number of master and slaves to communicatie with the remote nodes
  // if entry == -1, not filled, otherwise it's the index in the "master_table"
  struct ss_can_backbone_master *first_master_ptr;
  struct ss_can_backbone_slave *first_slave_ptr;

  struct ss_can_backbone_transmit_value *first_transmit_value_ptr;
  struct ss_can_backbone_receive_value *first_receive_value_ptr;

  void *appl_node_ptr; // void pointer to can_backbone_appl_node (stuct not the same in every application
                  // needed to faal back to information in this struct
} s_can_backbone_node;


//******* receive alarm ***********
typedef struct ss_can_backbone_receive_alarm
{
  unsigned int address_sender;    // if address_sender == 0 then every sender_address is correct
//  unsigned int address_receiver;  // 0 for receiving broadcoast other for exact addres
  unsigned char node_list_end;	 // '1' if this is the last one in a node list
  s_can_backbone_node *node_ptr;  // pointer need to check on witch node the element is coupled
  struct ss_can_backbone_receive_alarm *next_ptr; // pointer link the can backbone list
  unsigned char message_size;
  char buffer[8];
  unsigned int address_sender_in; // incomming address of sender
  unsigned char time_reset;       // set in interrupt to reset time_cnt in time control function
  unsigned int time_cnt;          // time_cnt increment every second until time_cnt == time_out_max
  unsigned int time_out_max;      // if time_out_cnt max == 0 then there is no time out on this receive value
                                  // else if time_cnt == time_out_cnt then (*receive_func)(FALSE, last correct value)
                                  //                                       only call this func the ones
  void (*receive_func)(struct ss_can_backbone_receive_alarm *receive_alarm_ptr, unsigned char ok); // this func is called when a new value is received. time_out_cnt = time_out_max
                                  // or value is not valid any more
                                  // if function pointer is 0 then don't call function
                                  // ok = TRUE if new value
                                  // ok = FALSE if value not valid any more 
  unsigned int  RECEIVE_OK;
  unsigned int  RECEIVE_BAD;
} s_can_backbone_receive_alarm;

//******* transmit alarm *********
typedef struct ss_can_backbone_transmit_alarm
{
//  unsigned int address_sender;       // youre address
  unsigned int address_receiver;     // 0 for broadcoast else exact address 
  unsigned char priority;

  unsigned char node_list_end;	 // '1' if this is the last one in a node list
  s_can_backbone_node *node_ptr;  // pointer need to check on witch node the element is coupled
  struct ss_can_backbone_transmit_alarm *next_ptr; // pointer link the can backbone list

  unsigned char message_size;
  char buffer[8];

  unsigned char time_reset;       // set in interrupt to reset time_cnt in time control function
  unsigned int time_cnt;            // time_cnt increment every second until time_cnt == time_out_repeat_max
  unsigned int time_repeat_max;     // if time_repeat_max == 0 then message is only sended if send is set to one
                                     // else if time_cnt == time_repeat_max then send = 1
  unsigned int time_delay_max;      // minimum time to wait before sending again

  unsigned char send;               // if send == 1 and time_cnt >= time_delay_max then send message as soon as possible
  unsigned char send_reset;         // set in interrupt to reset the send_timeout and send_retry_counter in the timing control
  unsigned int send_timeout;        // timeout for message to send in can buffer
  unsigned int send_retry_counter;  // retry counter for message to send in can buffer

  void (*transmit_func)(struct ss_can_backbone_transmit_alarm *transmit_alarm_ptr, unsigned char ok); // function is called function correct sended or on error
                                           // if function pointer is 0 then don't call function

  unsigned int  TRANSMIT_OK;     // alarm and watchdog counters for each alarm transmit
  unsigned int  TRANSMIT_BAD;
} s_can_backbone_transmit_alarm;
//******* receive value *********
typedef struct ss_can_backbone_receive_value
{
  unsigned int address_sender;    // if address_sender == 0 then every sender_address is correct
//  unsigned int address_receiver;  // 0 for receiving broadcoast other for exact addres
  unsigned char command;		  // incoming command in the received value

  unsigned char node_list_end;    // '1' if this is the last one in a node list
  s_can_backbone_node *node_ptr;  // pointer need to check on witch node the element is coupled
  struct ss_can_backbone_receive_value *next_ptr; // pointer link the can backbone list

  unsigned char message_size;
  char buffer[8];
  
  unsigned int address_sender_in; // incomming addres of sender
  unsigned char time_reset;       // set in interrupt to reset time_cnt in time control function
  unsigned int time_cnt;          // time_cnt increment every second until time_cnt == time_out_max
  unsigned int time_out_max;      // if time_out_cnt max == 0 then there is no time out on this receive value
                                  // else if time_cnt == time_out_cnt then (*receive_func)(FALSE, last correct value)
                                  //                                       only call this func the ones

  void (*receive_func)(struct ss_can_backbone_receive_value *receive_value_ptr, unsigned char ok); // this func is called when a new value is received. time_out_cnt = time_out_max
                                  // or value is not valid any more
                                  // if function pointer is 0 then don't call function
                                  // ok = TRUE if new value
                                  // ok = FALSE if value not valid any more
  unsigned int RECEIVE_OK;
  unsigned int RECEIVE_BAD;
} s_can_backbone_receive_value;

//******* transmit value ********
typedef struct ss_can_backbone_transmit_value
{
//  unsigned int address_sender;   // youre address
  unsigned int address_receiver; // 0 for broadcoast else exact address 
  unsigned char	command;
  unsigned char priority;
  unsigned char node_list_end;	 // '1' if this is the last one in a node list
  s_can_backbone_node *node_ptr;  // pointer need to check on witch node the element is coupled
  struct ss_can_backbone_transmit_value *next_ptr; // pointer link the can backbone list
  unsigned char message_size;
  char buffer[8];

  unsigned char time_reset;       // set in interrupt to reset time_cnt in time control function
  unsigned int time_cnt;         // time_cnt increment every second until time_cnt == time_out_repeat_max
  unsigned int time_repeat_max;  // if time_repeat_max == 0 then message is only sended if send is set to 1
                                 // else if time_cnt == time_repeat_max then send = 1
  unsigned int time_delay_max;   // minimum time to wait before sending again

  unsigned char send;            // if send == 1 and time_cnt >= time_delay_max then send message as soon as possible
  unsigned char send_reset;         // set in interrupt to reset the send_timeout and send_retry_counter in the timing control
  unsigned int send_timeout;
  unsigned int send_retry_counter;

  void (*transmit_func)(struct ss_can_backbone_transmit_value *transmit_value_ptr, unsigned char ok); // function is called function correct sended or on error
                                 // if function pointer is 0 then don't call function

  unsigned int TRANSMIT_OK;
  unsigned int TRANSMIT_BAD;
} s_can_backbone_transmit_value;

//******* SDO SLAVE ********
typedef struct ss_can_backbone_slave
{
  unsigned int address_sender; 
//  unsigned int address_receiver; 
  unsigned char toggle_bit;

  unsigned char node_list_end;	 // '1' if this is the last one in a node list
  s_can_backbone_node *node_ptr;  // pointer need to check on witch node the element is coupled
  struct ss_can_backbone_slave *next_ptr; // pointer link the can backbone list

  unsigned int message_size;           // size of receive message (don't needed)
  unsigned int max_size;           // max size of buffer
  char *buffer_ptr;  // pointer of buffer where to store incomming message

  unsigned char time_reset;       // set in interrupt to reset time_cnt in time control function
  unsigned int  time_cnt;
  unsigned int  time_out_max;
  unsigned char watchdog_flag;     // 1 if the other node is still there, else is 0

  void (*receive_func)(struct ss_can_backbone_slave *sdo_slave, unsigned char ok); // this func is called message is correct sended or when an error

  unsigned int  RECEIVE_OK;  // receive ok and bad counters
  unsigned int  RECEIVE_BAD;

} s_can_backbone_slave;

//******* SDO MASTER *******
typedef struct ss_can_backbone_master
{
//  unsigned int address_sender;       // 
  unsigned int address_receiver;        //
  unsigned char toggle_bit;
  unsigned char priority;
  unsigned char node_list_end;	 // '1' if this is the last one in a node list
  s_can_backbone_node *node_ptr;  // pointer need to check on witch node the element is coupled
  struct ss_can_backbone_master *next_ptr; // pointer link the can backbone list

  unsigned int message_size;             // size of the message to transmit
  char *buffer_ptr;    // pointer of buffer where data to transmit is stored

  unsigned char send;
  unsigned char send_reset;         // set in interrupt to reset the send_timeout and send_retry_counter in the timing control
  unsigned int send_timeout;
  unsigned int send_retry_counter;
  unsigned int buffer_index;
  s_CAN_SWObj   prev_msg;

  void (*transmit_func)(struct ss_can_backbone_master *sdo_master, unsigned char ok); // this func is called message is correct sended or there is an error
                                     // ok = TRUE if message sended
                                     // ok = FALSE if error (message not sended)

  unsigned int  TRANSMIT_OK;  // succes and error counters for transmission
  unsigned int  TRANSMIT_BAD;
  unsigned char fast;
} s_can_backbone_master;

//********************************************************************************************************
// Variable defines
//********************************************************************************************************

extern s_can_backbone_receive_value *can_backbone_first_receive_value_ptr;
     
extern s_can_backbone_transmit_value *can_backbone_first_transmit_value_ptr;

extern s_can_backbone_receive_alarm *can_backbone_first_receive_alarm_ptr;

extern s_can_backbone_transmit_alarm *can_backbone_first_transmit_alarm_ptr;

extern s_can_backbone_slave *can_backbone_first_slave_ptr;

extern s_can_backbone_master *can_backbone_first_master_ptr;


//Function protorypes
// receive value
// transmit value

// receive alarm
// transmit alarm

// Slave
// Master

//Watchdog control
/*
struct s_can_backbone_node
{
  unsigned int address_node; // node address of this node
                             // 0 node is not initialised
  // for each internal node we need alarm receive and transmit value's!
  s_can_backbone_transmit_alarm *first_transmit_alarm_ptr;
  s_can_backbone_receive_alarm *first_receive_alarm_ptr;

  // define number of master and slaves to communicatie with the remote nodes
  // if entry == -1, not filled, otherwise it's the index in the "master_table"
  s_can_backbone_master *first_master_ptr;
  s_can_backbone_slave *first_slave_ptr;

  s_can_backbone_transmit_value *first_transmit_value_ptr;
  s_can_backbone_receive_value *first_receive_value_ptr;

  void *appl_node_ptr; // void pointer to can_backbone_appl_node (stuct not the same in every application
                  // needed to faal back to information in this struct
};
*/
void Can_Backbone_Control(void);
void Can_Backbone_Reset(void);
void Can_Backbone_Timing_Control(void);
void Can_Backbone_Free_Transmit_Value(s_can_backbone_transmit_value *delete_transmit_value_ptr);
void Can_Backbone_Transmit_Value_Init(s_can_backbone_node *node_ptr, 
                         unsigned int address_receiver, 
                         unsigned char command,
                         void(*transmit_func)( s_can_backbone_transmit_value *transmit_value_ptr, unsigned char ok),
                         s_can_backbone_transmit_value *transmit_value_ptr,
                         unsigned int repeat_time);
void Can_Backbone_Receive_Value_Init(s_can_backbone_node *node_ptr,
                        unsigned int address_sender,
                        unsigned char command,
                        void(*receive_func)(s_can_backbone_receive_value *pdo_receive, unsigned char ok),
                        s_can_backbone_receive_value *pdo_receive,
                        unsigned int timeout);
void Can_Backbone_Master_Init(s_can_backbone_node *node_ptr, 
                 unsigned int address_receiver, 
                 void(*transmit_func)(s_can_backbone_master *sdo_master, unsigned char ok),
                 s_can_backbone_master *master_ptr);
void Can_Backbone_Master_Transmit(s_can_backbone_master *sdo_master, char *ptr, unsigned int message_size);
void Can_Backbone_Slave_Init(s_can_backbone_node *node_ptr, 
                unsigned int address_sender, 
                unsigned int max_size,
                char *ptr,
                void(*receive_func)(s_can_backbone_slave *sdo_slave, unsigned char ok),
                s_can_backbone_slave *slave, 
                unsigned int timeout);
void Can_Backbone_Receive_Alarm_Init(s_can_backbone_node *node_ptr,     // pointer naar struct met gegevens van 1 afdeling
                                     void (*receive_func)(s_can_backbone_receive_alarm *receive_alarm_ptr, unsigned char ok), // function called after receive a alarm (with ok = true)
                                                                                                                              // and after a timeout (with ok = false)
                                     s_can_backbone_receive_alarm *receive_alarm_ptr,
                                     unsigned int timeout); // tijd waarbinnen nieuw bericht moet zijn binnen gekomen, anders
                                                            // wordt bovenstaande functie met false aangeroepen
void Can_Backbone_Transmit_Alarm_Init(s_can_backbone_node *node_ptr, 
                                      void(*transmit_func)(s_can_backbone_transmit_alarm *transmit_alarm_ptr, unsigned char ok),
                                      s_can_backbone_transmit_alarm *transmit_alarm_ptr,
                                      unsigned int repeat_time);
void Can_Backbone_Node_Init(s_can_backbone_node *node_ptr,
                            unsigned int address_node);

// moet eigenlijk in ch_can_backbone_harde.h staan
void Send_Alarm(s_can_backbone_transmit_alarm *transmit_alarm_ptr);
void Send_Value(s_can_backbone_transmit_value *transmit_value_ptr);
void Send_Master(s_can_backbone_master *master_ptr);


#endif // __CH_CAN_BACKBONE_H