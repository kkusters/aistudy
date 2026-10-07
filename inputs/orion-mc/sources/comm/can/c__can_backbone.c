// C__CAN_BACKBONE.C

#include "ch_define.h"
#include "ch_can_backbone.h"

// linked list that holds pointers to all initialized "s_can_backbone_receive_value"
s_can_backbone_receive_value *can_backbone_first_receive_value_ptr = (s_can_backbone_receive_value *)END_OF_CAN_LIST;
s_can_backbone_transmit_value *can_backbone_first_transmit_value_ptr = (s_can_backbone_transmit_value *)END_OF_CAN_LIST;
s_can_backbone_slave *can_backbone_first_slave_ptr = (s_can_backbone_slave *)END_OF_CAN_LIST;
s_can_backbone_master *can_backbone_first_master_ptr = (s_can_backbone_master *)END_OF_CAN_LIST;
s_can_backbone_receive_alarm *can_backbone_first_receive_alarm_ptr = (s_can_backbone_receive_alarm *)END_OF_CAN_LIST;
s_can_backbone_transmit_alarm *can_backbone_first_transmit_alarm_ptr = (s_can_backbone_transmit_alarm *)END_OF_CAN_LIST;

//*****************************************************************************
//* Can backbone Alarm
//*****************************************************************************

//-----------------------------------------------------------------------------
// Can backbone receive alarm
//-----------------------------------------------------------------------------

void Can_Backbone_Receive_Alarm_Reset(void)
// Clear list of receive alarm for can backbone
{
  can_backbone_first_receive_alarm_ptr = (s_can_backbone_receive_alarm *)END_OF_CAN_LIST;  
}

void Can_Backbone_Receive_Alarm_Timing_Control(void)
// this function checks every initialised receive value and looks if there is 
// a time out on a value so that the value is not longer valid, then calls
// (*receive_func)(FALSE);
// !!! FUNCTION MUST BE CALLED EVERY 100ms SECOND (XC161CJ every 125ms)!!!
{
s_can_backbone_receive_alarm *receive_alarm_ptr = can_backbone_first_receive_alarm_ptr;

  while (receive_alarm_ptr != (s_can_backbone_receive_alarm *)END_OF_CAN_LIST)
  {
    if (receive_alarm_ptr->time_reset)
    {
      Disable_Can_Backbone_Int();
      receive_alarm_ptr->time_cnt = 0;
      receive_alarm_ptr->time_reset = 0;
      Enable_Can_Backbone_Int();
    }
    receive_alarm_ptr->time_cnt++;
    if ((receive_alarm_ptr->time_out_max != 0) &&
        (receive_alarm_ptr->time_cnt >= receive_alarm_ptr->time_out_max))
    {
      receive_alarm_ptr->RECEIVE_BAD++;
      if (receive_alarm_ptr->receive_func != 0)
      {
        Disable_Can_Backbone_Int();
        (receive_alarm_ptr->receive_func)(receive_alarm_ptr, RECEIVE_ALARM_ERROR);   
        Enable_Can_Backbone_Int();
      }  
      receive_alarm_ptr->time_cnt = 0;
    }
    receive_alarm_ptr = receive_alarm_ptr->next_ptr;
  }
}

void Can_Backbone_Receive_Alarm_Node_Init(s_can_backbone_receive_alarm *new_receive_alarm_ptr, 
                                          s_can_backbone_receive_alarm **node_first_receive_alarm_ptr)
// put new receive_alarm component in the node list and in the can backbone list
{
s_can_backbone_receive_alarm *receive_alarm_ptr;

  Disable_Can_Backbone_Int();
  if (can_backbone_first_receive_alarm_ptr == (s_can_backbone_receive_alarm *)END_OF_CAN_LIST) // no element in can backbone list
  {
    // place element in can backbone list
    can_backbone_first_receive_alarm_ptr = new_receive_alarm_ptr;
    new_receive_alarm_ptr->next_ptr = (s_can_backbone_receive_alarm *)END_OF_CAN_LIST;
  }
  else
  {
    if ((*node_first_receive_alarm_ptr == (s_can_backbone_receive_alarm *)END_OF_CAN_LIST) || // no element in node list
                                                                              // no element from this node in can backbone list
         (can_backbone_first_receive_alarm_ptr->node_ptr == new_receive_alarm_ptr->node_ptr)) // first element of backbone list has the same node ptr
    {                                                                         // set new element in front of it
      // place element if first place of can backbone list
      new_receive_alarm_ptr->next_ptr = can_backbone_first_receive_alarm_ptr;
      can_backbone_first_receive_alarm_ptr = new_receive_alarm_ptr;
    }
    else
    {
      receive_alarm_ptr = can_backbone_first_receive_alarm_ptr;
      while (receive_alarm_ptr->next_ptr->node_ptr != new_receive_alarm_ptr->node_ptr)
        receive_alarm_ptr = receive_alarm_ptr->next_ptr;
      new_receive_alarm_ptr->next_ptr = receive_alarm_ptr->next_ptr;
      receive_alarm_ptr->next_ptr = new_receive_alarm_ptr;
    }
  }
  Enable_Can_Backbone_Int();
  if (*node_first_receive_alarm_ptr == (s_can_backbone_receive_alarm *)END_OF_CAN_LIST)
  {
    new_receive_alarm_ptr->node_list_end = 1; // last element of node list
    *node_first_receive_alarm_ptr = new_receive_alarm_ptr;
  }
  else if ((*node_first_receive_alarm_ptr)->node_ptr == new_receive_alarm_ptr->node_ptr) // first element of node list has the same node ptr
  {                                                                      // set new element as first element in node list
    // place element in first place of node list
    new_receive_alarm_ptr->node_list_end = 0; // not last element of node list
    *node_first_receive_alarm_ptr = new_receive_alarm_ptr;
  }
}

void Can_Backbone_Free_Receive_Alarm(s_can_backbone_receive_alarm *delete_receive_alarm_ptr)
// removes the receive_alarm from the node list and the can backbone list
{
s_can_backbone_receive_alarm *receive_alarm_ptr; 

  // we do NOT want the can interrupt to disrupt data!
  if ((can_backbone_first_receive_alarm_ptr != (s_can_backbone_receive_alarm *)END_OF_CAN_LIST) &&
      (delete_receive_alarm_ptr != (s_can_backbone_receive_alarm *)END_OF_CAN_LIST))
  {    
    Disable_Can_Backbone_Int();
    if (can_backbone_first_receive_alarm_ptr == delete_receive_alarm_ptr)
    { // component is first one one in the list! (remove it)
      can_backbone_first_receive_alarm_ptr = delete_receive_alarm_ptr->next_ptr;
    }
    else
    {
      receive_alarm_ptr = can_backbone_first_receive_alarm_ptr;
      while (receive_alarm_ptr->next_ptr != (s_can_backbone_receive_alarm *)END_OF_CAN_LIST)    
      {
        if (receive_alarm_ptr->next_ptr == delete_receive_alarm_ptr)
        { // we found the component!
          receive_alarm_ptr->next_ptr = delete_receive_alarm_ptr->next_ptr;
          // if component to delete the last from a node check if there is a component to the same node
          // and if so then set that component as last component of the node
          if ((delete_receive_alarm_ptr->node_list_end == 1) &&
              (receive_alarm_ptr->node_ptr == delete_receive_alarm_ptr->node_ptr))
            receive_alarm_ptr->node_list_end = 1;
          break;
        }
        receive_alarm_ptr = receive_alarm_ptr->next_ptr; // go to next one in linked list
      }
    }
    Enable_Can_Backbone_Int();

    // if the component to the delete is the same as where the can node
    // is pointed to then set them to the next component
    if (delete_receive_alarm_ptr->node_ptr->first_receive_alarm_ptr == delete_receive_alarm_ptr)
    {
      if (delete_receive_alarm_ptr->node_list_end == 1)
        delete_receive_alarm_ptr->node_ptr->first_receive_alarm_ptr = (s_can_backbone_receive_alarm *)END_OF_CAN_LIST;
      else
        delete_receive_alarm_ptr->node_ptr->first_receive_alarm_ptr = delete_receive_alarm_ptr->next_ptr;
    }  
  }  
}

//-----------------------------------------------------------------------------

void Can_Backbone_Receive_Alarm_Init(s_can_backbone_node *node_ptr,     // pointer naar struct met gegevens van 1 afdeling
                                     void (*receive_func)(s_can_backbone_receive_alarm *receive_alarm_ptr, unsigned char ok), // function called after receive a alarm (with ok = true)
                                                                                                                              // and after a timeout (with ok = false)
                                     s_can_backbone_receive_alarm *receive_alarm_ptr,
                                     unsigned int timeout) // tijd waarbinnen nieuw bericht moet zijn binnen gekomen, anders
                                                           // wordt bovenstaande functie met false aangeroepen
{
int loop;

  receive_alarm_ptr->address_sender = BROADCAST;
//  receive_alarm_ptr->address_receiver = node_ptr->address_node;
  receive_alarm_ptr->node_list_end = 1;
  receive_alarm_ptr->node_ptr = node_ptr;
  receive_alarm_ptr->next_ptr = (s_can_backbone_receive_alarm *)END_OF_CAN_LIST;

  receive_alarm_ptr->message_size = 8;
  for (loop = 0; loop < 8; loop++)
    receive_alarm_ptr->buffer[loop] = 0;

  receive_alarm_ptr->address_sender_in = 0;
  receive_alarm_ptr->time_reset = 0;
  receive_alarm_ptr->time_cnt = 0;
  receive_alarm_ptr->time_out_max = timeout;
  receive_alarm_ptr->receive_func = receive_func;
  
  Can_Backbone_Receive_Alarm_Node_Init(receive_alarm_ptr, &(node_ptr->first_receive_alarm_ptr));  
}

//-----------------------------------------------------------------------------
// Can backbone transmit alarm
//-----------------------------------------------------------------------------

void Can_Backbone_Transmit_Alarm_Reset(void)
// Clear list of transmit values for can backbone
{
  can_backbone_first_transmit_alarm_ptr = (s_can_backbone_transmit_alarm *)END_OF_CAN_LIST;  
}

void Can_Backbone_Transmit_Alarm_Control(void)
// function is called every main loop cycle
// this function checks if a alarm must be send
{
s_can_backbone_transmit_alarm *transmit_alarm_ptr;

  transmit_alarm_ptr = can_backbone_first_transmit_alarm_ptr;
  while (transmit_alarm_ptr != (s_can_backbone_transmit_alarm *)END_OF_CAN_LIST)
  {
    if (transmit_alarm_ptr->time_reset)
    {
      Disable_Can_Backbone_Int();
      transmit_alarm_ptr->time_cnt = 0;
      transmit_alarm_ptr->time_reset = 0;
      Enable_Can_Backbone_Int();
    }
    //if the time count reached the timeout value, or the alarm must have 
    // been sent earlier but it was not possible - send alarm
    switch (transmit_alarm_ptr->send)
    {
      case FALSE:
        if (transmit_alarm_ptr->time_repeat_max != 0)
        {
          if (transmit_alarm_ptr->time_cnt < transmit_alarm_ptr->time_repeat_max)
            break;
        }
        else if (transmit_alarm_ptr->time_cnt == 0)
          break;
      case PENDING:
        Send_Alarm(transmit_alarm_ptr);
        break;
    }    
    transmit_alarm_ptr = transmit_alarm_ptr->next_ptr;
  }
}

void Can_Backbone_Transmit_Alarm_Timing_Control(void)
// function is called every 100ms (125 msec in XC161CJ)
// to increment time_cnt (for repeated sending of the alarm)
{
s_can_backbone_transmit_alarm *transmit_alarm_ptr;

  transmit_alarm_ptr = can_backbone_first_transmit_alarm_ptr;
  while (transmit_alarm_ptr != (s_can_backbone_transmit_alarm *)END_OF_CAN_LIST)
  {
    if (transmit_alarm_ptr->time_reset)
    {
      Disable_Can_Backbone_Int();
      transmit_alarm_ptr->time_cnt = 0;
      transmit_alarm_ptr->time_reset = 0;
      Enable_Can_Backbone_Int();
    }
    if ((transmit_alarm_ptr->time_repeat_max != 0) &&
        (transmit_alarm_ptr->time_cnt < transmit_alarm_ptr->time_repeat_max))
      transmit_alarm_ptr->time_cnt++;
    transmit_alarm_ptr = transmit_alarm_ptr->next_ptr;
  }
}

void Can_Backbone_Transmit_Alarm_Node_Init(s_can_backbone_transmit_alarm *new_transmit_alarm_ptr, 
                                           s_can_backbone_transmit_alarm **node_first_transmit_alarm_ptr)
// put new transmit_alarm component in the node list and in the can backbone list
{
s_can_backbone_transmit_alarm *transmit_alarm_ptr;

  Disable_Can_Backbone_Int();
  if (can_backbone_first_transmit_alarm_ptr == (s_can_backbone_transmit_alarm *)END_OF_CAN_LIST) // no element in can backbone list
  {
    // place element in can backbone list
    can_backbone_first_transmit_alarm_ptr = new_transmit_alarm_ptr;
    new_transmit_alarm_ptr->next_ptr = (s_can_backbone_transmit_alarm *)END_OF_CAN_LIST;
  }
  else
  {
    if ((*node_first_transmit_alarm_ptr == (s_can_backbone_transmit_alarm *)END_OF_CAN_LIST) || // no element in node list
                                                                              // no element from this node in can backbone list
         (can_backbone_first_transmit_alarm_ptr->node_ptr == new_transmit_alarm_ptr->node_ptr)) // first element of backbone list has the same node ptr
    {                                                                         // set new element in front of it
      // place element if first place of can backbone list
      new_transmit_alarm_ptr->next_ptr = can_backbone_first_transmit_alarm_ptr;
      can_backbone_first_transmit_alarm_ptr = new_transmit_alarm_ptr;
    }
    else
    {
      transmit_alarm_ptr = can_backbone_first_transmit_alarm_ptr;
      while (transmit_alarm_ptr->next_ptr->node_ptr != new_transmit_alarm_ptr->node_ptr)
        transmit_alarm_ptr = transmit_alarm_ptr->next_ptr;
      new_transmit_alarm_ptr->next_ptr = transmit_alarm_ptr->next_ptr;
      transmit_alarm_ptr->next_ptr = new_transmit_alarm_ptr;
    }
  }
  Enable_Can_Backbone_Int();
  if (*node_first_transmit_alarm_ptr == (s_can_backbone_transmit_alarm *)END_OF_CAN_LIST)
  {
    new_transmit_alarm_ptr->node_list_end = 1; // last element of node list
    *node_first_transmit_alarm_ptr = new_transmit_alarm_ptr;
  }
  else if ((*node_first_transmit_alarm_ptr)->node_ptr == new_transmit_alarm_ptr->node_ptr) // first element of node list has the same node ptr
  {                                                                      // set new element as first element in node list
    // place element in first place of node list
    new_transmit_alarm_ptr->node_list_end = 0; // not last element of node list
    *node_first_transmit_alarm_ptr = new_transmit_alarm_ptr;
  }
}

void Can_Backbone_Free_Transmit_Alarm(s_can_backbone_transmit_alarm *delete_transmit_alarm_ptr)
// removes the transmit_alarm from the node list and the can backbone list
{
s_can_backbone_transmit_alarm *transmit_alarm_ptr; 

  // we do NOT want the can interrupt to disrupt data!
  if ((can_backbone_first_transmit_alarm_ptr != (s_can_backbone_transmit_alarm *)END_OF_CAN_LIST) &&
      (delete_transmit_alarm_ptr != (s_can_backbone_transmit_alarm *)END_OF_CAN_LIST))
  {    
    Disable_Can_Backbone_Int();
    if (can_backbone_first_transmit_alarm_ptr == delete_transmit_alarm_ptr)
    { // component is first one one in the list! (remove it)
      can_backbone_first_transmit_alarm_ptr = delete_transmit_alarm_ptr->next_ptr;
    }
    else
    {
      transmit_alarm_ptr = can_backbone_first_transmit_alarm_ptr;
      while (transmit_alarm_ptr->next_ptr != (s_can_backbone_transmit_alarm *)END_OF_CAN_LIST)    
      {
        if (transmit_alarm_ptr->next_ptr == delete_transmit_alarm_ptr)
        { // we found the component!
          transmit_alarm_ptr->next_ptr = delete_transmit_alarm_ptr->next_ptr;
          // if component to delete the last from a node check if there is a component to the same node
          // and if so then set that component as last component of the node
          if ((delete_transmit_alarm_ptr->node_list_end == 1) &&
              (transmit_alarm_ptr->node_ptr == delete_transmit_alarm_ptr->node_ptr))
            transmit_alarm_ptr->node_list_end = 1;
          break;
        }
        transmit_alarm_ptr = transmit_alarm_ptr->next_ptr; // go to next one in linked list
      }
    }
    Enable_Can_Backbone_Int();

    // if the component to the delete is the same as where the can node
    // is pointed to then set them to the next component
    if (delete_transmit_alarm_ptr->node_ptr->first_transmit_alarm_ptr == delete_transmit_alarm_ptr)
    {
      if (delete_transmit_alarm_ptr->node_list_end == 1)
        delete_transmit_alarm_ptr->node_ptr->first_transmit_alarm_ptr = (s_can_backbone_transmit_alarm *)END_OF_CAN_LIST;
      else
        delete_transmit_alarm_ptr->node_ptr->first_transmit_alarm_ptr = delete_transmit_alarm_ptr->next_ptr;
    }  
  }  
}

//-----------------------------------------------------------------------------

void Can_Backbone_Transmit_Alarm_Init(s_can_backbone_node *node_ptr, 
                                      void(*transmit_func)(s_can_backbone_transmit_alarm *transmit_alarm_ptr, unsigned char ok),
                                      s_can_backbone_transmit_alarm *transmit_alarm_ptr,
                                      unsigned int repeat_time)
{
int loop;

//  transmit_alarm_ptr->address_sender = node_ptr->address_node;
  transmit_alarm_ptr->address_receiver = BROADCAST;
  transmit_alarm_ptr->priority = PRIORITY_HIGH;
  transmit_alarm_ptr->node_list_end = 1;
  transmit_alarm_ptr->node_ptr = node_ptr;
  transmit_alarm_ptr->next_ptr = (s_can_backbone_transmit_alarm *)END_OF_CAN_LIST;

  transmit_alarm_ptr->message_size = 0;
  for (loop = 0; loop < 8; loop++)
    transmit_alarm_ptr->buffer[loop] = 0;
    
  transmit_alarm_ptr->time_reset = 0;
  transmit_alarm_ptr->time_cnt = 0;
  transmit_alarm_ptr->time_repeat_max = repeat_time;
  transmit_alarm_ptr->time_delay_max = TRANSMIT_ALARM_BLOCKED;

  transmit_alarm_ptr->send = 0;
  transmit_alarm_ptr->send_reset = 0;
  transmit_alarm_ptr->send_retry_counter = CAN_RETRY;
  transmit_alarm_ptr->send_timeout = CAN_ALARM_TIMEOUT;
  // set up callback function
  transmit_alarm_ptr->transmit_func = transmit_func;
  
  
  Can_Backbone_Transmit_Alarm_Node_Init(transmit_alarm_ptr, &(node_ptr->first_transmit_alarm_ptr));
}

//*****************************************************************************
//* Can backbone Value
//*****************************************************************************

//-----------------------------------------------------------------------------
// Can backbone receive value
//-----------------------------------------------------------------------------

void Can_Backbone_Receive_Value_Reset(void)
// Clear list of receive values for can backbone
{
  can_backbone_first_receive_value_ptr = (s_can_backbone_receive_value *)END_OF_CAN_LIST;
}

void Can_Backbone_Receive_Value_Timing_Control(void)
// this function checks every initialised receive value and looks if there is 
// a time out on a value so that the value is not longer valid, then calls
// (*receive_func)(FALSE);
// !!! FUNCTION MUST BE CALLED EVERY 100ms SECOND (XC161CJ every 125ms)!!!
{
s_can_backbone_receive_value *receive_value_ptr = can_backbone_first_receive_value_ptr; 

  while (receive_value_ptr != (s_can_backbone_receive_value *)END_OF_CAN_LIST)    
  {
    if (receive_value_ptr->time_reset)
    {
      Disable_Can_Backbone_Int();
      receive_value_ptr->time_cnt = 0;
      receive_value_ptr->time_reset = 0;
      Enable_Can_Backbone_Int();
    }
    receive_value_ptr->time_cnt++;
    if ((receive_value_ptr->time_out_max != 0) &&
        (receive_value_ptr->time_cnt >= receive_value_ptr->time_out_max))
    {
      receive_value_ptr->RECEIVE_BAD++;
      if (receive_value_ptr->receive_func != 0)
      {
        Disable_Can_Backbone_Int();
        (receive_value_ptr->receive_func)(receive_value_ptr, RECEIVE_VALUE_ERROR);           
        Enable_Can_Backbone_Int();
      }  
      receive_value_ptr->time_cnt = 0;
    }
    receive_value_ptr = receive_value_ptr->next_ptr;    // go to next one in linked list
  }
}

void Can_Backbone_Receive_Value_Node_Init(s_can_backbone_receive_value *new_receive_value_ptr, s_can_backbone_receive_value **node_first_receive_value_ptr)
// put new receive_value component in the node list and in the can backbone list
{
s_can_backbone_receive_value *receive_value_ptr;

  Disable_Can_Backbone_Int();
  if (can_backbone_first_receive_value_ptr == (s_can_backbone_receive_value *)END_OF_CAN_LIST) // no element in can backbone list
  {
    // place element in can backbone list
    can_backbone_first_receive_value_ptr = new_receive_value_ptr;
    new_receive_value_ptr->next_ptr = (s_can_backbone_receive_value *)END_OF_CAN_LIST;
  }
  else
  {
    if ((*node_first_receive_value_ptr == (s_can_backbone_receive_value *)END_OF_CAN_LIST) || // no element in node list
                                                                              // no element from this node in can backbone list
         (can_backbone_first_receive_value_ptr->node_ptr == new_receive_value_ptr->node_ptr)) // first element of backbone list has the same node ptr
    {                                                                         // set new element in front of it
      // place element if first place of can backbone list
      new_receive_value_ptr->next_ptr = can_backbone_first_receive_value_ptr;
      can_backbone_first_receive_value_ptr = new_receive_value_ptr;
    }
    else
    {
      receive_value_ptr = can_backbone_first_receive_value_ptr;
      while (receive_value_ptr->next_ptr->node_ptr != new_receive_value_ptr->node_ptr)
        receive_value_ptr = receive_value_ptr->next_ptr;
      new_receive_value_ptr->next_ptr = receive_value_ptr->next_ptr;
      receive_value_ptr->next_ptr = new_receive_value_ptr;
    }
  }
  Enable_Can_Backbone_Int();
  if (*node_first_receive_value_ptr == (s_can_backbone_receive_value *)END_OF_CAN_LIST)
  {
    new_receive_value_ptr->node_list_end = 1; // last element of node list
    *node_first_receive_value_ptr = new_receive_value_ptr;
  }
  else if ((*node_first_receive_value_ptr)->node_ptr == new_receive_value_ptr->node_ptr) // first element of node list has the same node ptr
  {                                                                      // set new element as first element in node list
    // place element in first place of node list
    new_receive_value_ptr->node_list_end = 0; // not last element of node list
    *node_first_receive_value_ptr = new_receive_value_ptr;
  }
}

void Can_Backbone_Free_Receive_Value(s_can_backbone_receive_value *delete_receive_value_ptr)
// removes the receive_value from the node list and the can backbone list
{
s_can_backbone_receive_value *receive_value_ptr; 

  // we do NOT want the can interrupt to disrupt data!
  if ((can_backbone_first_receive_value_ptr != (s_can_backbone_receive_value *)END_OF_CAN_LIST) &&
      (delete_receive_value_ptr != (s_can_backbone_receive_value *)END_OF_CAN_LIST))
  {    
    Disable_Can_Backbone_Int();
    if (can_backbone_first_receive_value_ptr == delete_receive_value_ptr)
    { // component is first one one in the list! (remove it)
      can_backbone_first_receive_value_ptr = delete_receive_value_ptr->next_ptr;
    }
    else
    {
      receive_value_ptr = can_backbone_first_receive_value_ptr;
      while (receive_value_ptr->next_ptr != (s_can_backbone_receive_value *)END_OF_CAN_LIST)    
      {
        if (receive_value_ptr->next_ptr == delete_receive_value_ptr)
        { // we found the component!
          receive_value_ptr->next_ptr = delete_receive_value_ptr->next_ptr;
          // if component to delete the last from a node check if there is a component to the same node
          // and if so then set that component as last component of the node
          if ((delete_receive_value_ptr->node_list_end == 1) &&
              (receive_value_ptr->node_ptr == delete_receive_value_ptr->node_ptr))
            receive_value_ptr->node_list_end = 1;
          break;
        }
        receive_value_ptr = receive_value_ptr->next_ptr; // go to next one in linked list
      }
    }
    Enable_Can_Backbone_Int();

    // if the component to the delete is the same as where the can node
    // is pointed to then set them to the next component
    if (delete_receive_value_ptr->node_ptr->first_receive_value_ptr == delete_receive_value_ptr)
    {
      if (delete_receive_value_ptr->node_list_end == 1)
        delete_receive_value_ptr->node_ptr->first_receive_value_ptr = (s_can_backbone_receive_value *)END_OF_CAN_LIST;
      else
        delete_receive_value_ptr->node_ptr->first_receive_value_ptr = delete_receive_value_ptr->next_ptr;
    }  
  }  
}

//-----------------------------------------------------------------------------

void Can_Backbone_Receive_Value_Init(s_can_backbone_node *node_ptr,
                                     unsigned int address_sender,
                                     unsigned char command,
                                     void(*receive_func)(s_can_backbone_receive_value *receive_value_ptr, unsigned char ok),
                                     s_can_backbone_receive_value *receive_value_ptr,
                                     unsigned int timeout)
{
int loop;

  receive_value_ptr->address_sender = address_sender;
//  receive_value_ptr->address_receiver = node_ptr->address_node;
  receive_value_ptr->command = command;
  receive_value_ptr->node_list_end = 1;
  receive_value_ptr->node_ptr = node_ptr;
  receive_value_ptr->next_ptr = (s_can_backbone_receive_value *)END_OF_CAN_LIST;

  receive_value_ptr->message_size = 0;
  for (loop = 0; loop < 8; loop++)
    receive_value_ptr->buffer[loop] = 0;

  receive_value_ptr->address_sender_in = 0;
  receive_value_ptr->time_reset = 0;
  receive_value_ptr->time_cnt = 0;
  receive_value_ptr->time_out_max = timeout;
  receive_value_ptr->receive_func = receive_func;
  Can_Backbone_Receive_Value_Node_Init(receive_value_ptr, &(node_ptr->first_receive_value_ptr));
}

//----------------------------------------------------------------------------- 
// Can backbone transmit value
//-----------------------------------------------------------------------------

void Can_Backbone_Transmit_Value_Reset(void)
// Clear list of transmit values for can backbone
{
  can_backbone_first_transmit_value_ptr = (s_can_backbone_transmit_value *)END_OF_CAN_LIST;
}

void Can_Backbone_Transmit_Value_Control(void)
// function is called every main loop cycle
// this function checks if a value must be send
{
s_can_backbone_transmit_value *transmit_value_ptr = can_backbone_first_transmit_value_ptr; 

  while (transmit_value_ptr != (s_can_backbone_transmit_value *)END_OF_CAN_LIST)    
  {
    if (transmit_value_ptr->time_reset)
    {
      Disable_Can_Backbone_Int();
      transmit_value_ptr->time_cnt = 0;
      transmit_value_ptr->time_reset = 0;
      Enable_Can_Backbone_Int();
    }
    //if the time count reached the timeout value, or the value must have 
    // been send earlier but it was not possible - send value
    switch (transmit_value_ptr->send)
    {
      case FALSE:
        if (transmit_value_ptr->time_repeat_max != 0)
        {
          if (transmit_value_ptr->time_cnt < transmit_value_ptr->time_repeat_max)
            break;
        }
        else if (transmit_value_ptr->time_cnt == 0)
          break;
      case PENDING:
        Send_Value(transmit_value_ptr);
        break;
    }
    transmit_value_ptr = transmit_value_ptr->next_ptr; // go to next one in linked list
  }
}

void Can_Backbone_Transmit_Value_Timing_Control(void)
// function is called every 100ms (125 msec in XC161CJ)
// to increment time_cnt (for repeated sending of the value)
{
s_can_backbone_transmit_value *transmit_value_ptr = can_backbone_first_transmit_value_ptr; 

  while (transmit_value_ptr != (s_can_backbone_transmit_value *)END_OF_CAN_LIST)    
  {
    if (transmit_value_ptr->time_reset)
    {
      Disable_Can_Backbone_Int();
      transmit_value_ptr->time_cnt = 0;
      transmit_value_ptr->time_reset = 0;
      Enable_Can_Backbone_Int();
    }
    if ((transmit_value_ptr->time_repeat_max != 0) &&
        (transmit_value_ptr->time_cnt < transmit_value_ptr->time_repeat_max))
      transmit_value_ptr->time_cnt++;
    transmit_value_ptr = transmit_value_ptr->next_ptr; // go to next one in linked list
  }
}

void Can_Backbone_Transmit_Value_Node_Init(s_can_backbone_transmit_value *new_transmit_value_ptr, s_can_backbone_transmit_value **node_first_transmit_value_ptr)
// put new transmit_value component in the node list and in the can backbone list
{
s_can_backbone_transmit_value *transmit_value_ptr;

  Disable_Can_Backbone_Int();
  if (can_backbone_first_transmit_value_ptr == (s_can_backbone_transmit_value *)END_OF_CAN_LIST) // no element in can backbone list
  {
    // place element in can backbone list
    can_backbone_first_transmit_value_ptr = new_transmit_value_ptr;
    new_transmit_value_ptr->next_ptr = (s_can_backbone_transmit_value *)END_OF_CAN_LIST;
  }
  else
  {
    if ((*node_first_transmit_value_ptr == (s_can_backbone_transmit_value *)END_OF_CAN_LIST) || // no element in node list
                                                                              // no element from this node in can backbone list
         (can_backbone_first_transmit_value_ptr->node_ptr == new_transmit_value_ptr->node_ptr)) // first element of backbone list has the same node ptr
    {                                                                         // set new element in front of it
      // place element if first place of can backbone list
      new_transmit_value_ptr->next_ptr = can_backbone_first_transmit_value_ptr;
      can_backbone_first_transmit_value_ptr = new_transmit_value_ptr;
    }
    else
    {
      transmit_value_ptr = can_backbone_first_transmit_value_ptr;
      while (transmit_value_ptr->next_ptr->node_ptr != new_transmit_value_ptr->node_ptr)
        transmit_value_ptr = transmit_value_ptr->next_ptr;
      new_transmit_value_ptr->next_ptr = transmit_value_ptr->next_ptr;
      transmit_value_ptr->next_ptr = new_transmit_value_ptr;
    }
  }
  Enable_Can_Backbone_Int();
  if (*node_first_transmit_value_ptr == (s_can_backbone_transmit_value *)END_OF_CAN_LIST)
  {
    new_transmit_value_ptr->node_list_end = 1; // last element of node list
    *node_first_transmit_value_ptr = new_transmit_value_ptr;
  }
  else if ((*node_first_transmit_value_ptr)->node_ptr == new_transmit_value_ptr->node_ptr) // first element of node list has the same node ptr
  {                                                                      // set new element as first element in node list
    // place element in first place of node list
    new_transmit_value_ptr->node_list_end = 0; // not last element of node list
    *node_first_transmit_value_ptr = new_transmit_value_ptr;
  }
}

void Can_Backbone_Free_Transmit_Value(s_can_backbone_transmit_value *delete_transmit_value_ptr)
// removes the transmit_value from the node list and the can backbone list
{
s_can_backbone_transmit_value *transmit_value_ptr; 
s_can_backbone_transmit_value const empty_transmit_value = {0}; // JP 21-03-07 toegevoegd om struct te wissen

  // we do NOT want the can interrupt to disrupt data!
  if ((can_backbone_first_transmit_value_ptr != (s_can_backbone_transmit_value *)END_OF_CAN_LIST) &&
      (delete_transmit_value_ptr != (s_can_backbone_transmit_value *)END_OF_CAN_LIST))
  {    
    Disable_Can_Backbone_Int();
    if (can_backbone_first_transmit_value_ptr == delete_transmit_value_ptr)
    { // component is first one one in the list! (remove it)
      can_backbone_first_transmit_value_ptr = delete_transmit_value_ptr->next_ptr;
    }
    else
    {
      transmit_value_ptr = can_backbone_first_transmit_value_ptr;
      while (transmit_value_ptr->next_ptr != (s_can_backbone_transmit_value *)END_OF_CAN_LIST)    
      {
        if (transmit_value_ptr->next_ptr == delete_transmit_value_ptr)
        { // we found the component!
          transmit_value_ptr->next_ptr = delete_transmit_value_ptr->next_ptr;
          // if component to delete the last from a node check if there is a component to the same node
          // and if so then set that component as last component of the node
          if ((delete_transmit_value_ptr->node_list_end == 1) &&
              (transmit_value_ptr->node_ptr == delete_transmit_value_ptr->node_ptr))
            transmit_value_ptr->node_list_end = 1;
          break;
        }
        transmit_value_ptr = transmit_value_ptr->next_ptr; // go to next one in linked list
      }
    }
    Enable_Can_Backbone_Int();

    // if the component to the delete is the same as where the can node
    // is pointed to then set them to the next component
    if (delete_transmit_value_ptr->node_ptr->first_transmit_value_ptr == delete_transmit_value_ptr)
    {
      if (delete_transmit_value_ptr->node_list_end == 1)
        delete_transmit_value_ptr->node_ptr->first_transmit_value_ptr = (s_can_backbone_transmit_value *)END_OF_CAN_LIST;
      else
        delete_transmit_value_ptr->node_ptr->first_transmit_value_ptr = delete_transmit_value_ptr->next_ptr;
    }  
    *delete_transmit_value_ptr = empty_transmit_value; // JP 21-03-07 toegevoegd om struct te wissen
  }  
}

//-----------------------------------------------------------------------------

void Can_Backbone_Transmit_Value_Init(s_can_backbone_node *node_ptr, 
                                      unsigned int address_receiver, 
                                      unsigned char command,
                                      void(*transmit_func)( s_can_backbone_transmit_value *transmit_value_ptr, unsigned char ok),
                                      s_can_backbone_transmit_value *transmit_value_ptr,
                                      unsigned int repeat_time)
// initialise a transmit value and then put it in the node list and the can backbone list                         
{
int loop;

//  transmit_value_ptr->address_sender = node_ptr->address_node;
  transmit_value_ptr->address_receiver = address_receiver;
  transmit_value_ptr->command = command;
  transmit_value_ptr->priority = PRIORITY_LOW;
  transmit_value_ptr->node_list_end = 1;
  transmit_value_ptr->node_ptr = node_ptr;
  transmit_value_ptr->next_ptr = (s_can_backbone_transmit_value *)END_OF_CAN_LIST;

  transmit_value_ptr->message_size = 0;
  for (loop = 0; loop < 8; loop++)
    transmit_value_ptr->buffer[loop] = 0;
    
  transmit_value_ptr->time_repeat_max = repeat_time;
  transmit_value_ptr->time_delay_max = TRANSMIT_VALUE_BLOCKED;
  transmit_value_ptr->time_cnt = 0;

  transmit_value_ptr->send = 0;
  transmit_value_ptr->send_reset = 0;
  transmit_value_ptr->send_timeout = CAN_VALUE_TIMEOUT;
  transmit_value_ptr->send_retry_counter = CAN_RETRY;
        
  transmit_value_ptr->transmit_func = transmit_func;
                    
  Can_Backbone_Transmit_Value_Node_Init(transmit_value_ptr, &(node_ptr->first_transmit_value_ptr));
}

//*****************************************************************************
//* Can backbone Master & Slave
//*****************************************************************************

//-----------------------------------------------------------------------------
// Can backbone slave
//-----------------------------------------------------------------------------

void Can_Backbone_Slave_Reset(void)
// Clear list of slave for can backbone
{
  can_backbone_first_slave_ptr = (s_can_backbone_slave *)END_OF_CAN_LIST;
}

void Can_Backbone_Slave_Timing_Control(void)
// the function checks the watchdog timer every 100ms (xc161cj 125ms) in every registered sdo slave object
// if the timer reaches the max value, then a soft alarm must be raised by calling the 
// sdo slave receive func with a special parameter 
{
s_can_backbone_slave *slave_ptr = slave_ptr = can_backbone_first_slave_ptr;

  while (slave_ptr != (s_can_backbone_slave *)END_OF_CAN_LIST)
  {
    if (slave_ptr->time_reset)
    {
      Disable_Can_Backbone_Int();
      slave_ptr->time_cnt = 0;
      slave_ptr->time_reset = 0;
      Enable_Can_Backbone_Int();
    }
    //increase counter
    slave_ptr->time_cnt++;
    //check if any counter reached max value, and call the receive func if that is the case
    if ((slave_ptr->time_out_max != 0) &&
        (slave_ptr->time_cnt >= slave_ptr->time_out_max))
    {
      slave_ptr->watchdog_flag = RESET;
      //call function
      slave_ptr->RECEIVE_BAD++;
      if (slave_ptr->receive_func != 0)
      {
        Disable_Can_Backbone_Int();
        (slave_ptr->receive_func)(slave_ptr, RECEIVE_SLAVE_ERROR);
        Enable_Can_Backbone_Int();
      }  
      //reset counter
      slave_ptr->time_cnt = 0;
    }
    slave_ptr = slave_ptr->next_ptr;
  }
}

void Can_Backbone_Slave_Node_Init(s_can_backbone_slave *new_slave_ptr, s_can_backbone_slave **node_first_slave_ptr)
// put new slave component in the node list and in the can backbone list
{
s_can_backbone_slave *slave_ptr;

  Disable_Can_Backbone_Int();
  if (can_backbone_first_slave_ptr == (s_can_backbone_slave *)END_OF_CAN_LIST) // no element in can backbone list
  {
    // place element in can backbone list
    can_backbone_first_slave_ptr = new_slave_ptr;
    new_slave_ptr->next_ptr = (s_can_backbone_slave *)END_OF_CAN_LIST;
  }
  else
  {
    if ((*node_first_slave_ptr == (s_can_backbone_slave *)END_OF_CAN_LIST) || // no element in node list
                                                                              // no element from this node in can backbone list
         (can_backbone_first_slave_ptr->node_ptr == new_slave_ptr->node_ptr)) // first element of backbone list has the same node ptr
    {                                                                         // set new element in front of it
      // place element if first place of can backbone list
      new_slave_ptr->next_ptr = can_backbone_first_slave_ptr;
      can_backbone_first_slave_ptr = new_slave_ptr;
    }
    else
    {
      slave_ptr = can_backbone_first_slave_ptr;
      while (slave_ptr->next_ptr->node_ptr != new_slave_ptr->node_ptr)
        slave_ptr = slave_ptr->next_ptr;
      new_slave_ptr->next_ptr = slave_ptr->next_ptr;
      slave_ptr->next_ptr = new_slave_ptr;
    }
  }
  Enable_Can_Backbone_Int();
  if (*node_first_slave_ptr == (s_can_backbone_slave *)END_OF_CAN_LIST)
  {
    new_slave_ptr->node_list_end = 1; // last element of node list
    *node_first_slave_ptr = new_slave_ptr;
  }
  else if ((*node_first_slave_ptr)->node_ptr == new_slave_ptr->node_ptr) // first element of node list has the same node ptr
  {                                                                      // set new element as first element in node list
    // place element in first place of node list
    new_slave_ptr->node_list_end = 0; // not last element of node list
    *node_first_slave_ptr = new_slave_ptr;
  }
}

void Can_Backbone_Free_Slave(s_can_backbone_slave *delete_slave_ptr)
// removes the slave from the node list and the can backbone list
{
s_can_backbone_slave *slave_ptr; 

  // we do NOT want the can interrupt to disrupt data!
  if ((can_backbone_first_slave_ptr != (s_can_backbone_slave *)END_OF_CAN_LIST) &&
      (delete_slave_ptr != (s_can_backbone_slave *)END_OF_CAN_LIST))
  {    
    Disable_Can_Backbone_Int();
    if (can_backbone_first_slave_ptr == delete_slave_ptr)
    { // component is first one one in the list! (remove it)
      can_backbone_first_slave_ptr = delete_slave_ptr->next_ptr;
    }
    else
    {
      slave_ptr = can_backbone_first_slave_ptr;
      while (slave_ptr->next_ptr != (s_can_backbone_slave *)END_OF_CAN_LIST)    
      {
        if (slave_ptr->next_ptr == delete_slave_ptr)
        { // we found the component!
          slave_ptr->next_ptr = delete_slave_ptr->next_ptr;
          // if component to delete the last from a node check if there is a component to the same node
          // and if so then set that component as last component of the node
          if ((delete_slave_ptr->node_list_end == 1) &&
              (slave_ptr->node_ptr == delete_slave_ptr->node_ptr))
            slave_ptr->node_list_end = 1;
          break;
        }
        slave_ptr = slave_ptr->next_ptr; // go to next one in linked list
      }
    }
    Enable_Can_Backbone_Int();

    // if the component to the delete is the same as where the can node
    // is pointed to then set them to the next component
    if (delete_slave_ptr->node_ptr->first_slave_ptr == delete_slave_ptr)
    {
      if (delete_slave_ptr->node_list_end == 1)
        delete_slave_ptr->node_ptr->first_slave_ptr = (s_can_backbone_slave *)END_OF_CAN_LIST;
      else
        delete_slave_ptr->node_ptr->first_slave_ptr = delete_slave_ptr->next_ptr;
    }  
  }  
}

//-----------------------------------------------------------------------------

void Can_Backbone_Slave_Init(s_can_backbone_node *node_ptr, 
                             unsigned int address_sender, 
                             unsigned int max_size,
                             char *buffer_ptr,
                             void(*receive_func)(s_can_backbone_slave *slave_ptr, unsigned char ok),
                             s_can_backbone_slave *slave_ptr, 
                             unsigned int timeout)
{
  slave_ptr->address_sender = address_sender;
//  slave_ptr->address_receiver = node_ptr->address_node;
  slave_ptr->toggle_bit = 0;
  slave_ptr->node_list_end = 1;
  slave_ptr->node_ptr = node_ptr;
  slave_ptr->next_ptr = (s_can_backbone_slave *)END_OF_CAN_LIST;

  slave_ptr->message_size = 0;
  slave_ptr->max_size = max_size;
  slave_ptr->buffer_ptr = buffer_ptr;
  slave_ptr->receive_func = receive_func;
        
  slave_ptr->time_out_max = timeout;
  slave_ptr->time_cnt = 0;
            
  Can_Backbone_Slave_Node_Init(slave_ptr, &(node_ptr->first_slave_ptr));
}

//-----------------------------------------------------------------------------
// Can backbone master
//-----------------------------------------------------------------------------

void Can_Backbone_Master_Reset(void)
// Clear list of master for can backbone
{
  can_backbone_first_master_ptr = (s_can_backbone_master *)END_OF_CAN_LIST;
}

void Can_Backbone_Master_Node_Init(s_can_backbone_master *new_master_ptr, s_can_backbone_master **node_first_master_ptr)
// put new master component in the node list and in the can backbone list
{
s_can_backbone_master *master_ptr;

  Disable_Can_Backbone_Int();
  if (can_backbone_first_master_ptr == (s_can_backbone_master *)END_OF_CAN_LIST) // no element in can backbone list
  {
    // place element in can backbone list
    can_backbone_first_master_ptr = new_master_ptr;
    new_master_ptr->next_ptr = (s_can_backbone_master *)END_OF_CAN_LIST;
  }
  else
  {
    if ((*node_first_master_ptr == (s_can_backbone_master *)END_OF_CAN_LIST) || // no element in node list
                                                                                // no element from this node in can backbone list
         (can_backbone_first_master_ptr->node_ptr == new_master_ptr->node_ptr)) // first element of backbone list has the same node ptr
    {                                                                           // set new element in front of it
      // place element if first place of can backbone list
      new_master_ptr->next_ptr = can_backbone_first_master_ptr;
      can_backbone_first_master_ptr = new_master_ptr;
    }
    else
    {
      master_ptr = can_backbone_first_master_ptr;
      while (master_ptr->next_ptr->node_ptr != new_master_ptr->node_ptr)
        master_ptr = master_ptr->next_ptr;
      new_master_ptr->next_ptr = master_ptr->next_ptr;
      master_ptr->next_ptr = new_master_ptr;
    }
  }
  Enable_Can_Backbone_Int();
  if (*node_first_master_ptr == (s_can_backbone_master *)END_OF_CAN_LIST)
  {
    new_master_ptr->node_list_end = 1; // last element of node list
    *node_first_master_ptr = new_master_ptr;
  }
  else if ((*node_first_master_ptr)->node_ptr == new_master_ptr->node_ptr) // first element of node list has the same node ptr
  {                                                                  // set new element as first element in node list
    // place element in first place of node list
    new_master_ptr->node_list_end = 0; // not last element of node list
    *node_first_master_ptr = new_master_ptr;
  }
}

void Can_Backbone_Free_Master(s_can_backbone_master *delete_master_ptr)
// removes the master from the node list and the can backbone list
{
s_can_backbone_master *master_ptr; 

  // we do NOT want the can interrupt to disrupt data!
  if ((can_backbone_first_master_ptr != (s_can_backbone_master *)END_OF_CAN_LIST) &&
      (delete_master_ptr != (s_can_backbone_master *)END_OF_CAN_LIST))
  {    
    Disable_Can_Backbone_Int();
    if (can_backbone_first_master_ptr == delete_master_ptr)
    { // component is first one one in the list! (remove it)
      can_backbone_first_master_ptr = delete_master_ptr->next_ptr;
    }
    else
    {
      master_ptr = can_backbone_first_master_ptr;
      while (master_ptr->next_ptr != (s_can_backbone_master *)END_OF_CAN_LIST)    
      {
        if (master_ptr->next_ptr == delete_master_ptr)
        { // we found the component!
          master_ptr->next_ptr = delete_master_ptr->next_ptr;
          // if component to delete the last from a node check if there is a component to the same node
          // and if so then set that component as last component of the node
          if ((delete_master_ptr->node_list_end == 1) &&
              (master_ptr->node_ptr == delete_master_ptr->node_ptr))
            master_ptr->node_list_end = 1;
          break;
        }
        master_ptr = master_ptr->next_ptr; // go to next one in linked list
      }
    }
    Enable_Can_Backbone_Int();

    // if the component to the delete is the same as where the can node
    // is pointed to then set them to the next component
    if (delete_master_ptr->node_ptr->first_master_ptr == delete_master_ptr)
    {
      if (delete_master_ptr->node_list_end == 1)
        delete_master_ptr->node_ptr->first_master_ptr = (s_can_backbone_master *)END_OF_CAN_LIST;
      else
        delete_master_ptr->node_ptr->first_master_ptr = delete_master_ptr->next_ptr;
    }  
  }  
}

void Can_Backbone_Master_Init(s_can_backbone_node *node_ptr, 
                              unsigned int address_receiver, 
                              void(*transmit_func)(s_can_backbone_master *master_ptr, unsigned char ok),
                              s_can_backbone_master *master_ptr)
{
//  master_ptr->address_sender = node_ptr->address_node; // fill master list entry
  master_ptr->address_receiver = address_receiver;
  master_ptr->toggle_bit = 0;
  master_ptr->priority = PRIORITY_LOW;
  master_ptr->node_list_end = 1;
  master_ptr->node_ptr = node_ptr;
  master_ptr->next_ptr = (s_can_backbone_master *)END_OF_CAN_LIST;

  master_ptr->message_size = 0;
  master_ptr->buffer_ptr = 0;

  master_ptr->send = 0;
  master_ptr->send_timeout = CAN_MASTER_TIMEOUT;
  master_ptr->send_reset = 0;
  master_ptr->send_retry_counter = CAN_RETRY;
  master_ptr->buffer_index = 0;
  
  master_ptr->transmit_func = transmit_func;
  Can_Backbone_Master_Node_Init(master_ptr, &(node_ptr->first_master_ptr));
}


// transmit a message byt the given sdo_master
void Can_Backbone_Master_Transmit(s_can_backbone_master *master_ptr, char *buffer_ptr, unsigned int message_size)
{
  master_ptr->buffer_ptr = buffer_ptr;
  master_ptr->message_size = message_size;  
  Send_Master(master_ptr);    
}

//*****************************************************************************
// Can backbone node
//*****************************************************************************

void Can_Backbone_Free_Node(s_can_backbone_node *node_ptr)
{
  if (node_ptr->address_node != 0) // check if node initialised
  {
    // remove node from can backbone
    while (node_ptr->first_transmit_alarm_ptr != (s_can_backbone_transmit_alarm *)END_OF_CAN_LIST)
      Can_Backbone_Free_Transmit_Alarm(node_ptr->first_transmit_alarm_ptr);
    while (node_ptr->first_receive_alarm_ptr != (s_can_backbone_receive_alarm *)END_OF_CAN_LIST)
      Can_Backbone_Free_Receive_Alarm(node_ptr->first_receive_alarm_ptr);
    while (node_ptr->first_transmit_value_ptr != (s_can_backbone_transmit_value *)END_OF_CAN_LIST)
      Can_Backbone_Free_Transmit_Value(node_ptr->first_transmit_value_ptr);
    while (node_ptr->first_receive_value_ptr != (s_can_backbone_receive_value *)END_OF_CAN_LIST)
      Can_Backbone_Free_Receive_Value(node_ptr->first_receive_value_ptr);
    while (node_ptr->first_master_ptr != (s_can_backbone_master *)END_OF_CAN_LIST)
      Can_Backbone_Free_Master(node_ptr->first_master_ptr);
    while (node_ptr->first_slave_ptr != (s_can_backbone_slave *)END_OF_CAN_LIST)
      Can_Backbone_Free_Slave(node_ptr->first_slave_ptr);
    node_ptr->address_node = 0;
  }  
}

//-----------------------------------------------------------------------------

void Can_Backbone_Node_Init(s_can_backbone_node *node_ptr,
                            unsigned int address_node)
{
  node_ptr->first_receive_alarm_ptr = (s_can_backbone_receive_alarm *)END_OF_CAN_LIST;
  node_ptr->first_transmit_alarm_ptr = (s_can_backbone_transmit_alarm *)END_OF_CAN_LIST;
  node_ptr->first_receive_value_ptr = (s_can_backbone_receive_value *)END_OF_CAN_LIST;
  node_ptr->first_transmit_value_ptr = (s_can_backbone_transmit_value *)END_OF_CAN_LIST;
  node_ptr->first_master_ptr = (s_can_backbone_master *)END_OF_CAN_LIST;
  node_ptr->first_slave_ptr = (s_can_backbone_slave *)END_OF_CAN_LIST;
  node_ptr->address_node = address_node;
}

//*****************************************************************************

void Can_Backbone_Reset(void)
{
  Can_Backbone_Receive_Alarm_Reset();
  Can_Backbone_Transmit_Alarm_Reset();
  Can_Backbone_Receive_Value_Reset();
  Can_Backbone_Transmit_Value_Reset();
  Can_Backbone_Slave_Reset();
  Can_Backbone_Master_Reset();
}

void Can_Backbone_Control(void)
// will be called in the main loop
{
  Can_Backbone_Transmit_Alarm_Control();

  Can_Backbone_Transmit_Value_Control();
}

void Can_Backbone_Timing_Control(void)
// will be called every 100ms (in XC161CJ every 125ms)
{
  Can_Backbone_Hardware_Timing_Control();

  Can_Backbone_Receive_Alarm_Timing_Control();
  Can_Backbone_Transmit_Alarm_Timing_Control();

  Can_Backbone_Receive_Value_Timing_Control();
  Can_Backbone_Transmit_Value_Timing_Control();

  Can_Backbone_Slave_Timing_Control();
}


