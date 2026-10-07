// C__CAN_BACKBONE_APPL.C

#include <string.h> 

#include "ch_define.h"

#include "ch_alarm.h"
#include "ch_tijd.h"
#include "ch_can_backbone_appl.h"

#ifdef CANopen
#include <co_drv.h>
#include "twincan.h"
#include "ch_DS401.h"

extern RET_T init_Library(void);
#endif // CANopen

//s_can_backbone_node can_backbone_node[MAX_NR_INTERNAL_NODES];   // this represents an internal computer
unsigned char can_backbone_appl_init_switch;
s_can_backbone_appl_node can_backbone_appl_node_alg;
s_can_backbone_master_slave pc_0_alg;
s_can_backbone_master_slave pc_1_alg;
s_can_backbone_master_slave pc_2_alg;
#ifdef CAN_BACKBONE_PC_WARNING
int pc_alarm_time_cnt[3] = {0,0,0};
#endif // CAN_BACKBONE_PC_WARNING
char pc_0_slave_buffer[MAX_PC_PACKED_BUFFER]; // memory needed by can to receive the message
char pc_0_master_buffer[MAX_PC_PACKED_BUFFER]; // memory needed by can to receive the message
char pc_1_slave_buffer[MAX_PC_PACKED_BUFFER]; // memory needed by can to receive the message
char pc_1_master_buffer[MAX_PC_PACKED_BUFFER]; // memory needed by can to receive the message
char pc_2_slave_buffer[MAX_PC_PACKED_BUFFER]; // memory needed by can to receive the message
char pc_2_master_buffer[MAX_PC_PACKED_BUFFER]; // memory needed by can to receive the message

//*****************************************************************************
// Master PC and Slave PC
//*****************************************************************************
#ifdef CAN_BACKBONE_PC_WARNING
void Can_Backbone_Slave_PC_0_Func(s_can_backbone_slave *slave_ptr, unsigned char ok)
{
  if (ok)
  {
    ((s_can_backbone_appl_node *)slave_ptr->node_ptr->appl_node_ptr)->pc_0->can_slave_message_ready = 1;
  }
}

void Can_Backbone_Slave_PC_1_Func(s_can_backbone_slave *slave_ptr, unsigned char ok)
{
  if (ok)
  {
    ((s_can_backbone_appl_node *)slave_ptr->node_ptr->appl_node_ptr)->pc_1->can_slave_message_ready = 1;
  }
}

void Can_Backbone_Slave_PC_2_Func(s_can_backbone_slave *slave_ptr, unsigned char ok)
{
  if (ok)
  {
    ((s_can_backbone_appl_node *)slave_ptr->node_ptr->appl_node_ptr)->pc_2->can_slave_message_ready = 1;
  }
}
#else // CAN_BACKBONE_PC_WARNING
void Can_Backbone_Slave_PC_0_Func(s_can_backbone_slave *slave_ptr, unsigned char ok)
{
  if (ok)
  {
    ((s_can_backbone_appl_node *)slave_ptr->node_ptr->appl_node_ptr)->pc_0->can_slave_message_ready = 1;
    if (slave_ptr->node_ptr->appl_node_ptr == &can_backbone_appl_node_alg)
    {
      ClearAlarm(&alarm_hr_alg.can_pc_al, CAN_PC_MASTER_AL, 0);
    }    
  }
  else
  {
    if (slave_ptr->node_ptr->appl_node_ptr == &can_backbone_appl_node_alg)
      CreateAlarm(&alarm_hr_alg.can_pc_al, CAN_PC_MASTER_AL, 0, 0, 0, ZACHT_ALARM);
  }
}

void Can_Backbone_Slave_PC_1_Func(s_can_backbone_slave *slave_ptr, unsigned char ok)
{
  if (ok)
  {
    ((s_can_backbone_appl_node *)slave_ptr->node_ptr->appl_node_ptr)->pc_1->can_slave_message_ready = 1;
  }
}

void Can_Backbone_Slave_PC_2_Func(s_can_backbone_slave *slave_ptr, unsigned char ok)
{
  if (ok)
  {
    ((s_can_backbone_appl_node *)slave_ptr->node_ptr->appl_node_ptr)->pc_2->can_slave_message_ready = 1;
  }
}
#endif // CAN_BACKBONE_PC_WARNING

void Can_Backbone_Master_PC_0_Func(s_can_backbone_master *master_ptr, unsigned char ok)
{
  master_ptr;
  if (ok)
  {
  }
  else
  {
//    while (1);
  }
}

void Can_Backbone_Master_PC_1_Func(s_can_backbone_master *master_ptr, unsigned char ok)
{
  master_ptr;
  if (ok)
  {
  }
  else
  {
//    while (1);
  }
}

void Can_Backbone_Master_PC_2_Func(s_can_backbone_master *master_ptr, unsigned char ok)
{
  master_ptr;
  if (ok)
  {
  }
  else
  {
//    while (1);
  }
}

void Can_Backbone_Set_Master_PC_0_And_Send(s_can_backbone_appl_node *appl_node_ptr, 
                                           char *buffer_ptr)
{
unsigned int length = (unsigned char)buffer_ptr[3] + 6;

  Can_Backbone_Master_Transmit(&appl_node_ptr->pc_0->master, buffer_ptr, length);
}

void Can_Backbone_Set_Master_PC_1_And_Send(s_can_backbone_appl_node *appl_node_ptr, 
                                           char *buffer_ptr)
{
unsigned int length = (unsigned char)buffer_ptr[3] + 6;

  Can_Backbone_Master_Transmit(&appl_node_ptr->pc_1->master, buffer_ptr, length);
}

void Can_Backbone_Set_Master_PC_2_And_Send(s_can_backbone_appl_node *appl_node_ptr, 
                                           char *buffer_ptr)
{
unsigned int length = (unsigned char)buffer_ptr[3] + 6;

  Can_Backbone_Master_Transmit(&appl_node_ptr->pc_2->master, buffer_ptr, length);
}

#ifdef CAN_BACKBONE_PC_WARNING
static void Can_Backbone_PC_Init(s_can_backbone_appl_node *appl_node_ptr)
{
  Can_Backbone_Master_Init(&appl_node_ptr->node,
                           PC_0_ADR, 
                           Can_Backbone_Master_PC_0_Func,
                           &appl_node_ptr->pc_0->master);
  appl_node_ptr->pc_0->can_slave_message_ready = 0; // set when CanBackbone_Slave_Init is called
  Can_Backbone_Slave_Init(&appl_node_ptr->node, 
                          PC_0_ADR, 
                          MAX_PC_PACKED_BUFFER,
                          appl_node_ptr->pc_0->can_slave_buffer,
                          Can_Backbone_Slave_PC_0_Func,
                          &appl_node_ptr->pc_0->slave, 
                          (opt_alg.can_backbone_pc_warning[0]) ? RECEIVE_SLAVE_TIMEOUT : 0);
  Can_Backbone_Master_Init(&appl_node_ptr->node,
                           PC_1_ADR, 
                           Can_Backbone_Master_PC_1_Func,
                           &appl_node_ptr->pc_1->master);
  appl_node_ptr->pc_1->can_slave_message_ready = 0; // set when CanBackbone_Slave_Init is called
  Can_Backbone_Slave_Init(&appl_node_ptr->node, 
                          PC_1_ADR, 
                          MAX_PC_PACKED_BUFFER,
                          appl_node_ptr->pc_1->can_slave_buffer,
                          Can_Backbone_Slave_PC_1_Func,
                          &appl_node_ptr->pc_1->slave, 
                          (opt_alg.can_backbone_pc_warning[1]) ? RECEIVE_SLAVE_TIMEOUT : 0);
  Can_Backbone_Master_Init(&appl_node_ptr->node,
                           PC_2_ADR, 
                           Can_Backbone_Master_PC_2_Func,
                           &appl_node_ptr->pc_2->master);
  appl_node_ptr->pc_2->can_slave_message_ready = 0; // set when CanBackbone_Slave_Init is called
  Can_Backbone_Slave_Init(&appl_node_ptr->node, 
                          PC_2_ADR, 
                          MAX_PC_PACKED_BUFFER,
                          appl_node_ptr->pc_2->can_slave_buffer,
                          Can_Backbone_Slave_PC_2_Func,
                          &appl_node_ptr->pc_2->slave, 
                          (opt_alg.can_backbone_pc_warning[2]) ? RECEIVE_SLAVE_TIMEOUT : 0);
}
#else // CAN_BACKBONE_PC_WARNING
void Can_Backbone_PC_Init(s_can_backbone_appl_node *appl_node_ptr)
{
  Can_Backbone_Master_Init(&appl_node_ptr->node,
                           PC_0_ADR, 
                           Can_Backbone_Master_PC_0_Func,
                           &appl_node_ptr->pc_0->master);
  appl_node_ptr->pc_0->can_slave_message_ready = 0; // set when CanBackbone_Slave_Init is called
  Can_Backbone_Slave_Init(&appl_node_ptr->node, 
                          PC_0_ADR, 
                          MAX_PC_PACKED_BUFFER,
                          appl_node_ptr->pc_0->can_slave_buffer,
                          Can_Backbone_Slave_PC_0_Func,
                          &appl_node_ptr->pc_0->slave, 
                          (opt_alg.can_rs232) ? RECEIVE_SLAVE_TIMEOUT : 0);
  if (opt_alg.can_rs232 == 0)
    ClearAlarm(&alarm_hr_alg.can_pc_al, CAN_PC_MASTER_AL, 0);
  Can_Backbone_Master_Init(&appl_node_ptr->node,
                           PC_1_ADR, 
                           Can_Backbone_Master_PC_1_Func,
                           &appl_node_ptr->pc_1->master);
  appl_node_ptr->pc_1->can_slave_message_ready = 0; // set when CanBackbone_Slave_Init is called
  Can_Backbone_Slave_Init(&appl_node_ptr->node, 
                          PC_1_ADR, 
                          MAX_PC_PACKED_BUFFER,
                          appl_node_ptr->pc_1->can_slave_buffer,
                          Can_Backbone_Slave_PC_1_Func,
                          &appl_node_ptr->pc_1->slave, 
                          0);
  Can_Backbone_Master_Init(&appl_node_ptr->node,
                           PC_2_ADR, 
                           Can_Backbone_Master_PC_2_Func,
                           &appl_node_ptr->pc_2->master);
  appl_node_ptr->pc_2->can_slave_message_ready = 0; // set when CanBackbone_Slave_Init is called
  Can_Backbone_Slave_Init(&appl_node_ptr->node, 
                          PC_2_ADR, 
                          MAX_PC_PACKED_BUFFER,
                          appl_node_ptr->pc_2->can_slave_buffer,
                          Can_Backbone_Slave_PC_2_Func,
                          &appl_node_ptr->pc_2->slave, 
                          0);
}
#endif // CAN_BACKBONE_PC_WARNING

//*****************************************************************************
// Master FWS and Slave FWS
//*****************************************************************************

//*****************************************************************************
// Master OPT and Slave OPT
//*****************************************************************************

//*****************************************************************************
// broadcast value to receive or transmit
//*****************************************************************************

void Can_Backbone_Receive_Alarm_Func(s_can_backbone_receive_alarm *receive_alarm_ptr, unsigned char ok)
{
s_can_backbone_appl_node *appl_node_ptr = receive_alarm_ptr->node_ptr->appl_node_ptr;
unsigned int receive_alarm_code;
unsigned int receive_alarm_value;
unsigned int receive_computer_type;
unsigned int receive_computer_soort;

  if (ok)
  {
    // not compleet only overwrite an alarm is it is from the same computer with an other alarm code
    // or if the appl_node_ptr->alarm_code is 0 then overwrite every time
    ((unsigned char *)&receive_alarm_code)[1] = receive_alarm_ptr->buffer[0];
    ((unsigned char *)&receive_alarm_code)[0] = receive_alarm_ptr->buffer[1];
    ((unsigned char *)&receive_alarm_value)[1] = receive_alarm_ptr->buffer[2];
    ((unsigned char *)&receive_alarm_value)[0] = receive_alarm_ptr->buffer[3];
    ((unsigned char *)&receive_computer_type)[1] = receive_alarm_ptr->buffer[4];
    ((unsigned char *)&receive_computer_type)[0] = receive_alarm_ptr->buffer[5];
    ((unsigned char *)&receive_computer_soort)[1] = receive_alarm_ptr->buffer[6];
    ((unsigned char *)&receive_computer_soort)[0] = receive_alarm_ptr->buffer[7];
    if ((receive_alarm_code != 0) ||
        (appl_node_ptr->receive_alarm_code == 0) ||
        (appl_node_ptr->receive_alarm_address == receive_alarm_ptr->address_sender_in))
    {
      appl_node_ptr->receive_alarm_time_cnt = 0; // JP 28-11-2006
      appl_node_ptr->receive_alarm_address = receive_alarm_ptr->address_sender_in;
      appl_node_ptr->receive_alarm_code = receive_alarm_code;
      appl_node_ptr->receive_alarm_value = receive_alarm_value;
      appl_node_ptr->receive_computer_type = receive_computer_type;
      appl_node_ptr->receive_computer_soort = receive_computer_soort;
    }
    #ifdef CAN_BACKBONE_PC_WARNING
    switch (receive_alarm_ptr->address_sender_in)
    {
      case PC_0_ADR: pc_alarm_time_cnt[0] = 0; break;
      case PC_1_ADR: pc_alarm_time_cnt[1] = 0; break;
      case PC_2_ADR: pc_alarm_time_cnt[2] = 0; break;
    }
    #endif // CAN_BACKBONE_PC_WARNING
  }
  else 
  {
    // no connection with can bus
  }
}

void Can_Backbone_Transmit_Alarm_Func(s_can_backbone_transmit_alarm *transmit_alarm_ptr, unsigned char ok)
{
s_can_backbone_appl_node *appl_node_ptr = transmit_alarm_ptr->node_ptr->appl_node_ptr;

  if (ok)
  {
  }
  else
  {
  }
}

void Can_Backbone_Set_Alarm(s_can_backbone_transmit_alarm *transmit_alarm_ptr, 
                            unsigned int alarm_code,
                            unsigned int alarm_value,
                            unsigned int computer_type,
                            unsigned int computer_soort)
{
  // fill message buffer with outsidetemp
  Disable_Can_Backbone_Int();
  transmit_alarm_ptr->message_size = 8;
  transmit_alarm_ptr->buffer[0] = ((unsigned char *)&alarm_code)[1];
  transmit_alarm_ptr->buffer[1] = ((unsigned char *)&alarm_code)[0];
  transmit_alarm_ptr->buffer[2] = ((unsigned char *)&alarm_value)[1];
  transmit_alarm_ptr->buffer[3] = ((unsigned char *)&alarm_value)[0];
  transmit_alarm_ptr->buffer[4] = ((unsigned char *)&computer_type)[1];
  transmit_alarm_ptr->buffer[5] = ((unsigned char *)&computer_type)[0];
  transmit_alarm_ptr->buffer[6] = ((unsigned char *)&computer_soort)[1];
  transmit_alarm_ptr->buffer[7] = ((unsigned char *)&computer_soort)[0];
  Enable_Can_Backbone_Int();
}

void Can_Backbone_Set_Alarm_And_Send(s_can_backbone_transmit_alarm *transmit_alarm_ptr,
                                     unsigned int alarm_code,
                                     unsigned int alarm_value,
                                     unsigned int computer_type,
                                     unsigned int computer_soort)
{
  Can_Backbone_Set_Alarm(transmit_alarm_ptr, alarm_code, alarm_value, computer_type, computer_soort);
  transmit_alarm_ptr->time_cnt = transmit_alarm_ptr->time_repeat_max + 1;
}

void Can_Backbone_Get_Alarm(s_can_backbone_appl_node *appl_node_ptr, 
                            unsigned int *alarm_code,
                            unsigned int *alarm_value,
                            unsigned int *computer_type,
                            unsigned int *computer_soort)
{
  Disable_Can_Backbone_Int();
  *alarm_code = appl_node_ptr->receive_alarm_code;
  *alarm_value = appl_node_ptr->receive_alarm_value;
  *computer_type = appl_node_ptr->receive_computer_type;
  *computer_soort = appl_node_ptr->receive_computer_soort;
  Enable_Can_Backbone_Int();
}

void Can_Backbone_Alarm_Init(s_can_backbone_appl_node *appl_node_ptr)
{
  appl_node_ptr->receive_alarm_address = 0; 
  appl_node_ptr->receive_alarm_code = 0;    
  appl_node_ptr->receive_alarm_value = 0;   
  appl_node_ptr->receive_computer_type = 0; 
  appl_node_ptr->receive_computer_soort = 0;
  appl_node_ptr->receive_alarm_time_cnt = 0;
  appl_node_ptr->receive_alarm_time_out_max = RECEIVE_ALARM_TIMEOUT;
  Can_Backbone_Receive_Alarm_Init(&appl_node_ptr->node,
                                  Can_Backbone_Receive_Alarm_Func,
                                  &appl_node_ptr->receive_alarm, 
                                  RECEIVE_ALARM_TIMEOUT);
  Can_Backbone_Transmit_Alarm_Init(&appl_node_ptr->node,
                                   Can_Backbone_Transmit_Alarm_Func,
                                   &appl_node_ptr->transmit_alarm, 
                                   TRANSMIT_ALARM_REPEAT);
  Can_Backbone_Set_Alarm_And_Send(&appl_node_ptr->transmit_alarm,
                                  0,  // alarm_code
                                  0,  // alarm_value
                                  module.computer,  // computer_type
                                  module.soort); // computer_soort
}

void Can_Backbone_Alarm_Timing_Control(s_can_backbone_appl_node *appl_node_ptr)
{
  if (appl_node_ptr->receive_alarm_code == 0)
  {
    appl_node_ptr->receive_alarm_time_cnt = 0;
	ClearAlarm(&alarm_hr_alg.computer_al, COMPUTER_AL, appl_node_ptr->receive_alarm_address);
  }  
  else  
  {
	CreateAlarm(&alarm_hr_alg.computer_al, COMPUTER_AL, appl_node_ptr->receive_alarm_address, 0, 0, ZACHT_ALARM);
    alarm_hr_alg.computer_al_nr = appl_node_ptr->receive_alarm_address;
    if (appl_node_ptr->receive_alarm_time_cnt < appl_node_ptr->receive_alarm_time_out_max)
      appl_node_ptr->receive_alarm_time_cnt++;
    else
    {
      Disable_Can_Backbone_Int();
      appl_node_ptr->receive_alarm_address = 0;
      appl_node_ptr->receive_alarm_code = 0;
      appl_node_ptr->receive_alarm_value = 0;
      appl_node_ptr->receive_computer_type = 0;
      appl_node_ptr->receive_computer_soort = 0;
      Enable_Can_Backbone_Int();
    }
  } 
}

//*****************************************************************************
// broadcast value to receive or transmit
//*****************************************************************************

void Can_Backbone_Receive_Value_Func(s_can_backbone_receive_value *receive_value_ptr, unsigned char ok)
{
unsigned int index = ((unsigned int)receive_value_ptr->buffer[0] << 8) + receive_value_ptr->buffer[1];
s_can_backbone_appl_node *appl_node_ptr = receive_value_ptr->node_ptr->appl_node_ptr;
time_t clock_in_sec;

  if (ok == RECEIVE_VALUE_OK)
  {
    switch (index)
    {
      case CAN_BACKBONE_OUTSIDETEMP:
        ((unsigned char *)&(appl_node_ptr->outsidetemp))[1] = receive_value_ptr->buffer[2];
        ((unsigned char *)&(appl_node_ptr->outsidetemp))[0] = receive_value_ptr->buffer[3];
        appl_node_ptr->outsidetemp_ok = 1;
        appl_node_ptr->outsidetemp_time_reset = 1;
        break;
      case CAN_BACKBONE_CLOCK_IN_SEC:
        if (opt_alg.can_backbone)
        {
          switch (setp_alg.tijd_sync)
          { 
            case 0: // tijd niet ontvangen of verzenden
              break;
            case 1: // tijd ontvangen
            case 2: // tijd verzenden (10-12-07 nu ook ontvangen)
              ((unsigned char *)&clock_in_sec)[3] = receive_value_ptr->buffer[2];
              ((unsigned char *)&clock_in_sec)[2] = receive_value_ptr->buffer[3];
              ((unsigned char *)&clock_in_sec)[1] = receive_value_ptr->buffer[4];
              ((unsigned char *)&clock_in_sec)[0] = receive_value_ptr->buffer[5];
              Tijd_PC_Set(clock_in_sec);
              break;
          }    
        }  
        appl_node_ptr->clock_in_sec_time_reset = 1;
        break;
      case CAN_BACKBONE_OUTSIDE_HUMIDITY:
        ((unsigned char *)&(appl_node_ptr->outside_humidity))[1] = receive_value_ptr->buffer[2];
        ((unsigned char *)&(appl_node_ptr->outside_humidity))[0] = receive_value_ptr->buffer[3];
        appl_node_ptr->outside_humidity_ok = 1;
        appl_node_ptr->outside_humidity_time_reset = 1;
        break;
      case CAN_BACKBONE_WIND_SPEED:
        ((unsigned char *)&(appl_node_ptr->wind_speed))[1] = receive_value_ptr->buffer[2];
        ((unsigned char *)&(appl_node_ptr->wind_speed))[0] = receive_value_ptr->buffer[3];
        appl_node_ptr->wind_speed_ok = 1;
        appl_node_ptr->wind_speed_time_reset = 1;
        break;
      case CAN_BACKBONE_WIND_DIRECTION:
        ((unsigned char *)&(appl_node_ptr->wind_direction))[1] = receive_value_ptr->buffer[2];
        ((unsigned char *)&(appl_node_ptr->wind_direction))[0] = receive_value_ptr->buffer[3];
        appl_node_ptr->wind_direction_ok = 1;
        appl_node_ptr->wind_direction_time_reset = 1;
        break;
      case CAN_BACKBONE_SOFTWARE_NUMBER: // only used in old can backbone
      case CAN_BACKBONE_SUNLIGHT: // only used by sirius-t
      case CAN_BACKBONE_RAIN:     // only used by sirius-t
      case CAN_BACKBONE_ASK_FWS:  // only used by orion-fws
      case CAN_BACKBONE_ASK_OPT:  // only used by orion-opt
        break;
      default:  
        break;
    }
  }
}

//*****************************************************************************
// outsidetemp
//*****************************************************************************

s_can_backbone_transmit_value can_backbone_transmit_outsidetemp;

void Can_Backbone_Transmit_Outsidetemp_Func(s_can_backbone_transmit_value *transmit_value_ptr, unsigned char ok)
// function is called if outside temp is transmitted or if there is a timeout for sending
{
  transmit_value_ptr;
  if (ok)
  {
    // outsidetemp is correct sended
  }
  else
  {
    // outsidetemp is not sended
  }
}

void Can_Backbone_Set_Outsidetemp(s_can_backbone_transmit_value *transmit_value_ptr, int new)
{
  // fill message buffer with outsidetemp
  Disable_Can_Backbone_Int();
  transmit_value_ptr->buffer[0] = CAN_BACKBONE_OUTSIDETEMP >> 8;
  transmit_value_ptr->buffer[1] = CAN_BACKBONE_OUTSIDETEMP & 0x00FF;
  transmit_value_ptr->buffer[2] = new >> 8;
  transmit_value_ptr->buffer[3] = new & 0x00FF;
  transmit_value_ptr->message_size = 4;
  Enable_Can_Backbone_Int();
  // fill value backbone with outsidetemp to transmit (not needed because value is never used)
//  ((s_can_backbone_appl_node *)transmit_value_ptr->node_ptr->appl_node_ptr)->outsidetemp = new;
}

void Can_Backbone_Set_Outsidetemp_And_Send(s_can_backbone_transmit_value *transmit_value_ptr, int new)
{
  Can_Backbone_Set_Outsidetemp(transmit_value_ptr, new);
  transmit_value_ptr->time_cnt = transmit_value_ptr->time_repeat_max + 1;
}

unsigned char Can_Backbone_Get_Outsidetemp(s_can_backbone_appl_node *appl_node_ptr, unsigned int *actual)
{
  if (appl_node_ptr->outsidetemp_ok)
  {
    Disable_Can_Backbone_Int();
    *actual = appl_node_ptr->outsidetemp;
    Enable_Can_Backbone_Int();
    return (RECEIVE_VALUE_OK);
  }
  else
  {
    if (appl_node_ptr->outsidetemp_time_out_max &&
        (appl_node_ptr->outsidetemp_time_cnt >= appl_node_ptr->outsidetemp_time_out_max))
      return (RECEIVE_VALUE_ERROR);
    else
      return (RECEIVE_VALUE_WAIT);
  }
}

void Can_Backbone_Outsidetemp_Init(s_can_backbone_appl_node *appl_node_ptr, unsigned char send, int outsidetemp)
{
  appl_node_ptr->outsidetemp_ok = 0;
  appl_node_ptr->outsidetemp_time_reset = 0;
  appl_node_ptr->outsidetemp_time_cnt = 0;
  appl_node_ptr->outsidetemp_time_out_max = RECEIVE_VALUE_TIMEOUT;
  if (send)
  {
    Can_Backbone_Transmit_Value_Init(&appl_node_ptr->node, 
                                     BROADCAST, 
                                     NO_COMMAND,
                                     Can_Backbone_Transmit_Outsidetemp_Func,
                                     &can_backbone_transmit_outsidetemp,
                                     TRANSMIT_VALUE_REPEAT);
    Can_Backbone_Set_Outsidetemp_And_Send(&can_backbone_transmit_outsidetemp, outsidetemp);
  }  
}

void Can_Backbone_Outsidetemp_Timing_Control(s_can_backbone_appl_node *appl_node_ptr)
{
  if (appl_node_ptr->outsidetemp_time_reset)
  {
    Disable_Can_Backbone_Int();
    appl_node_ptr->outsidetemp_time_cnt = 0;
    appl_node_ptr->outsidetemp_time_reset = 0;
    Enable_Can_Backbone_Int();
  }  
  if (appl_node_ptr->outsidetemp_time_out_max)
  {
    if (appl_node_ptr->outsidetemp_time_cnt < appl_node_ptr->outsidetemp_time_out_max)
      appl_node_ptr->outsidetemp_time_cnt++;
    else
      appl_node_ptr->outsidetemp_ok = 0;
  }    
}    

//*****************************************************************************
// clock_in_sec
//*****************************************************************************

s_can_backbone_transmit_value can_backbone_transmit_clock_in_sec;

void Can_Backbone_Transmit_Clock_In_Sec_Func(s_can_backbone_transmit_value *transmit_value_ptr, unsigned char ok)
// function is called if outside temp is transmitted or if there is a timeout for sending
{
  transmit_value_ptr;
  if (ok)
  {
    // outsidetemp is correct sended
  }
  else
  {
    // outsidetemp is not sended
  }
}

void Can_Backbone_Set_Clock_In_Sec(s_can_backbone_transmit_value *transmit_value_ptr, time_t new)
{
  // fill message buffer with outsidetemp
  Disable_Can_Backbone_Int();
  transmit_value_ptr->buffer[0] = CAN_BACKBONE_CLOCK_IN_SEC >> 8;
  transmit_value_ptr->buffer[1] = CAN_BACKBONE_CLOCK_IN_SEC & 0x00FF;
  transmit_value_ptr->buffer[2] = ((unsigned char *)&new)[3];
  transmit_value_ptr->buffer[3] = ((unsigned char *)&new)[2];
  transmit_value_ptr->buffer[4] = ((unsigned char *)&new)[1];
  transmit_value_ptr->buffer[5] = ((unsigned char *)&new)[0];
  transmit_value_ptr->message_size = 6;
  Enable_Can_Backbone_Int();
  // fill value backbone with outsidetemp to transmit (not needed because value is never used)
//  ((s_can_backbone_appl_node *)transmit_value_ptr->node_ptr->appl_node_ptr)->clock_in_sec = new;
}

void Can_Backbone_Set_Clock_In_Sec_And_Send(s_can_backbone_transmit_value *transmit_value_ptr, time_t new)
{
  Can_Backbone_Set_Clock_In_Sec(transmit_value_ptr, new);
  transmit_value_ptr->time_cnt = transmit_value_ptr->time_repeat_max + 1;
}

// NOG TEST CLOCK SYNC ALS ER EEN ORION MASTER IS
void Can_Backbone_Clock_In_Sec_Init(s_can_backbone_appl_node *appl_node_ptr)
{
  appl_node_ptr->clock_in_sec_time_reset = 0;
  appl_node_ptr->clock_in_sec_time_cnt = 0;
  appl_node_ptr->clock_in_sec_time_out_max = 0;
  if (opt_alg.can_backbone)
  {
    Can_Backbone_Transmit_Value_Init(&appl_node_ptr->node, 
                                     BROADCAST, 
                                     NO_COMMAND,
                                     Can_Backbone_Transmit_Clock_In_Sec_Func,
                                     &can_backbone_transmit_clock_in_sec,
                                     0);
//    Can_Backbone_Set_Clock_In_Sec_And_Send(&can_backbone_transmit_clock_in_sec, time(0));
  }  
}
  
void Can_Backbone_Clock_In_Sec_Timing_Control(s_can_backbone_appl_node *appl_node_ptr)
{
  appl_node_ptr;
}

//*****************************************************************************
// outside humidity
//*****************************************************************************

s_can_backbone_transmit_value can_backbone_transmit_outside_humidity;

void Can_Backbone_Transmit_Outside_Humidity_Func(s_can_backbone_transmit_value *transmit_value_ptr, unsigned char ok)
// function is called if outside temp is transmitted or if there is a timeout for sending
{
  transmit_value_ptr;
  if (ok)
  {
    // outsidetemp is correct sended
  }
  else
  {
    // outsidetemp is not sended
  }
}

void Can_Backbone_Set_Outside_Humidity(s_can_backbone_transmit_value *transmit_value_ptr, int new)
{
  // fill message buffer with outside humidity
  Disable_Can_Backbone_Int();
  transmit_value_ptr->buffer[0] = CAN_BACKBONE_OUTSIDE_HUMIDITY >> 8;
  transmit_value_ptr->buffer[1] = CAN_BACKBONE_OUTSIDE_HUMIDITY & 0x00FF;
  transmit_value_ptr->buffer[2] = new >> 8;
  transmit_value_ptr->buffer[3] = new & 0x00FF;
  transmit_value_ptr->message_size = 4;
  Enable_Can_Backbone_Int();
  // fill value backbone with outside humidity to transmit (not needed because value is never used)
//  ((s_can_backbone_appl_node *)transmit_value_ptr->node_ptr->appl_node_ptr)->outside_humidity = new;
}

void Can_Backbone_Set_Outside_Humidity_And_Send(s_can_backbone_transmit_value *transmit_value_ptr, int new)
{
  Can_Backbone_Set_Outside_Humidity(transmit_value_ptr, new);
  transmit_value_ptr->time_cnt = transmit_value_ptr->time_repeat_max + 1;
}

unsigned char Can_Backbone_Get_Outside_Humidity(s_can_backbone_appl_node *appl_node_ptr, unsigned int *actual)
{
  if (appl_node_ptr->outside_humidity_ok)
  {
    Disable_Can_Backbone_Int();
    *actual = appl_node_ptr->outside_humidity;
    Enable_Can_Backbone_Int();
    return (RECEIVE_VALUE_OK);
  }
  else
  {
    if (appl_node_ptr->outside_humidity_time_out_max &&
        (appl_node_ptr->outside_humidity_time_cnt >= appl_node_ptr->outside_humidity_time_out_max))
      return (RECEIVE_VALUE_ERROR);
    else
      return (RECEIVE_VALUE_WAIT);
  }
}

void Can_Backbone_Outside_Humidity_Init(s_can_backbone_appl_node *appl_node_ptr, unsigned char send, int outside_humidity)
{
  appl_node_ptr->outside_humidity_ok = 0;
  appl_node_ptr->outside_humidity_time_reset = 0;
  appl_node_ptr->outside_humidity_time_cnt = 0;
  appl_node_ptr->outside_humidity_time_out_max = RECEIVE_VALUE_TIMEOUT;
  if (send)
  {
    Can_Backbone_Transmit_Value_Init(&can_backbone_appl_node_alg.node, 
                                     BROADCAST, 
                                     NO_COMMAND,
                                     Can_Backbone_Transmit_Outside_Humidity_Func,
                                     &can_backbone_transmit_outside_humidity,
                                     TRANSMIT_VALUE_REPEAT);
    Can_Backbone_Set_Outside_Humidity_And_Send(&can_backbone_transmit_outside_humidity, outside_humidity);
  }  
}  

void Can_Backbone_Outside_Hunidity_Timing_Control(s_can_backbone_appl_node *appl_node_ptr)
{
  if (appl_node_ptr->outside_humidity_time_reset)
  {
    Disable_Can_Backbone_Int();
    appl_node_ptr->outside_humidity_time_cnt = 0;
    appl_node_ptr->outside_humidity_time_reset = 0;
    Enable_Can_Backbone_Int();
  }  
  if (appl_node_ptr->outside_humidity_time_out_max)
  {
    if (appl_node_ptr->outside_humidity_time_cnt < appl_node_ptr->outside_humidity_time_out_max)
      appl_node_ptr->outside_humidity_time_cnt++;
    else
      appl_node_ptr->outside_humidity_ok = 0;
  }  
}

//*****************************************************************************
// wind speed
//*****************************************************************************

s_can_backbone_transmit_value can_backbone_transmit_wind_speed;

void Can_Backbone_Transmit_Wind_Speed_Func(s_can_backbone_transmit_value *transmit_value_ptr, unsigned char ok)
// function is called if outside temp is transmitted or if there is a timeout for sending
{
  transmit_value_ptr;
  if (ok)
  {
    // outsidetemp is correct sended
  }
  else
  {
    // outsidetemp is not sended
  }
}

void Can_Backbone_Set_Wind_Speed(s_can_backbone_transmit_value *transmit_value_ptr, int new)
{
  // fill message buffer with wind speed
  Disable_Can_Backbone_Int();
  transmit_value_ptr->buffer[0] = CAN_BACKBONE_WIND_SPEED >> 8;
  transmit_value_ptr->buffer[1] = CAN_BACKBONE_WIND_SPEED & 0x00FF;
  transmit_value_ptr->buffer[2] = new >> 8;
  transmit_value_ptr->buffer[3] = new & 0x00FF;
  transmit_value_ptr->message_size = 4;
  Enable_Can_Backbone_Int();
  // fill value backbone with wind speed to transmit (not needed because value is never used)
//  ((s_can_backbone_appl_node *)transmit_value_ptr->node_ptr->appl_node_ptr)->wind_speed = new;
}

void Can_Backbone_Set_Wind_Speed_And_Send(s_can_backbone_transmit_value *transmit_value_ptr, int new)
{
  Can_Backbone_Set_Wind_Speed(transmit_value_ptr, new);
  transmit_value_ptr->time_cnt = transmit_value_ptr->time_repeat_max + 1;
}

unsigned char Can_Backbone_Get_Wind_Speed(s_can_backbone_appl_node *appl_node_ptr, unsigned int *actual)
{
  if (appl_node_ptr->wind_speed_ok)
  {
    Disable_Can_Backbone_Int();
    *actual = appl_node_ptr->wind_speed;
    Enable_Can_Backbone_Int();
    return (RECEIVE_VALUE_OK);
  }
  else
  {
    if (appl_node_ptr->wind_speed_time_out_max &&
        (appl_node_ptr->wind_speed_time_cnt >= appl_node_ptr->wind_speed_time_out_max))
      return (RECEIVE_VALUE_ERROR);
    else
      return (RECEIVE_VALUE_WAIT);
  }
}

void Can_Backbone_Wind_Speed_Init(s_can_backbone_appl_node *appl_node_ptr, unsigned char send, int wind_speed)
{
  appl_node_ptr->wind_speed_ok = 0;
  appl_node_ptr->wind_speed_time_reset = 0;
  appl_node_ptr->wind_speed_time_cnt = 0;
  appl_node_ptr->wind_speed_time_out_max = RECEIVE_VALUE_TIMEOUT;
  if (send)
  {
    Can_Backbone_Transmit_Value_Init(&can_backbone_appl_node_alg.node, 
                                     BROADCAST, 
                                     NO_COMMAND,
                                     Can_Backbone_Transmit_Wind_Speed_Func,
                                     &can_backbone_transmit_wind_speed,
                                     TRANSMIT_VALUE_REPEAT);
    Can_Backbone_Set_Wind_Speed_And_Send(&can_backbone_transmit_wind_speed, wind_speed);
  }  
}  
  
void Can_Backbone_Wind_Speed_Timing_Control(s_can_backbone_appl_node *appl_node_ptr)
{
  if (appl_node_ptr->wind_speed_time_reset)
  {
    Disable_Can_Backbone_Int();
    appl_node_ptr->wind_speed_time_cnt = 0;
    appl_node_ptr->wind_speed_time_reset = 0;
    Enable_Can_Backbone_Int();
  }  
  if (appl_node_ptr->wind_speed_time_out_max)
  {
    if (appl_node_ptr->wind_speed_time_cnt < appl_node_ptr->wind_speed_time_out_max)
      appl_node_ptr->wind_speed_time_cnt++;
    else
      appl_node_ptr->wind_speed_ok = 0;
  }      
}
    
//*****************************************************************************
// wind direction
//*****************************************************************************

s_can_backbone_transmit_value can_backbone_transmit_wind_direction;

void Can_Backbone_Transmit_Wind_Direction_Func(s_can_backbone_transmit_value *transmit_value_ptr, unsigned char ok)
// function is called if outside temp is transmitted or if there is a timeout for sending
{
  transmit_value_ptr;
  if (ok)
  {
    // outsidetemp is correct sended
  }
  else
  {
    // outsidetemp is not sended
  }
}

void Can_Backbone_Set_Wind_Direction(s_can_backbone_transmit_value *transmit_value_ptr, int new)
{
  // fill message buffer with wind direction
  Disable_Can_Backbone_Int();
  transmit_value_ptr->buffer[0] = CAN_BACKBONE_WIND_DIRECTION >> 8;
  transmit_value_ptr->buffer[1] = CAN_BACKBONE_WIND_DIRECTION & 0x00FF;
  transmit_value_ptr->buffer[2] = new >> 8;
  transmit_value_ptr->buffer[3] = new & 0x00FF;
  transmit_value_ptr->message_size = 4;
  Enable_Can_Backbone_Int();
  // fill value backbone with wind direction to transmit (not needed because value is never used)
//  ((s_can_backbone_appl_node *)transmit_value_ptr->node_ptr->appl_node_ptr)->wind_speed = new;
}

void Can_Backbone_Set_Wind_Direction_And_Send(s_can_backbone_transmit_value *transmit_value_ptr, int new)
{
  Can_Backbone_Set_Wind_Direction(transmit_value_ptr, new);
  transmit_value_ptr->time_cnt = transmit_value_ptr->time_repeat_max + 1;
}

unsigned char Can_Backbone_Get_Wind_Direction(s_can_backbone_appl_node *appl_node_ptr, unsigned int *actual)
{
  if (appl_node_ptr->wind_direction_ok)
  {
    Disable_Can_Backbone_Int();
    *actual = appl_node_ptr->wind_direction;
    Enable_Can_Backbone_Int();
    return (RECEIVE_VALUE_OK);
  }
  else
  {
    if (appl_node_ptr->wind_direction_time_out_max &&
        (appl_node_ptr->wind_direction_time_cnt >= appl_node_ptr->wind_direction_time_out_max))
      return (RECEIVE_VALUE_ERROR);
    else
      return (RECEIVE_VALUE_WAIT);
  }
}

void Can_Backbone_Wind_Direction_Init(s_can_backbone_appl_node *appl_node_ptr, unsigned char send, int wind_direction)
{
  appl_node_ptr->wind_direction_ok = 0;
  appl_node_ptr->wind_direction_time_reset = 0;
  appl_node_ptr->wind_direction_time_cnt = 0;
  appl_node_ptr->wind_direction_time_out_max = RECEIVE_VALUE_TIMEOUT;
  if (send)
  {
    Can_Backbone_Transmit_Value_Init(&can_backbone_appl_node_alg.node, 
                                     BROADCAST, 
                                     NO_COMMAND,
                                     Can_Backbone_Transmit_Wind_Direction_Func,
                                     &can_backbone_transmit_wind_direction,
                                     TRANSMIT_VALUE_REPEAT);
    Can_Backbone_Set_Wind_Direction_And_Send(&can_backbone_transmit_wind_direction, wind_direction);
  }  
}
  
void Can_Backbone_Wind_Direction_Timing_Control(s_can_backbone_appl_node *appl_node_ptr)
{
  if (appl_node_ptr->wind_direction_time_reset)
  {
    Disable_Can_Backbone_Int();
    appl_node_ptr->wind_direction_time_cnt = 0;
    appl_node_ptr->wind_direction_time_reset = 0;
    Enable_Can_Backbone_Int();
  }
  if (appl_node_ptr->wind_direction_time_out_max)
  {  
    if (appl_node_ptr->wind_direction_time_cnt < appl_node_ptr->wind_direction_time_out_max)
      appl_node_ptr->wind_direction_time_cnt++;
    else  
      appl_node_ptr->wind_direction_ok = 0;
  }    
}
    
//*****************************************************************************
// FWS ask
//*****************************************************************************

s_can_backbone_transmit_value can_backbone_transmit_fws_ask;

void Can_Backbone_Transmit_FWS_Ask_Func(s_can_backbone_transmit_value *transmit_value_ptr, unsigned char ok)
// function is called if outside temp is transmitted or if there is a timeout for sending
{
  transmit_value_ptr;
  if (ok)
  {
    // outsidetemp is correct sended
  }
  else
  {
    // outsidetemp is not sended
  }
}

void Can_Backbone_Set_FWS_Ask(s_can_backbone_transmit_value *transmit_value_ptr, int new)
{
  // fill message buffer with fws ask
  Disable_Can_Backbone_Int();
  transmit_value_ptr->buffer[0] = CAN_BACKBONE_ASK_FWS >> 8;
  transmit_value_ptr->buffer[1] = CAN_BACKBONE_ASK_FWS & 0x00FF;
  transmit_value_ptr->buffer[2] = new >> 8;
  transmit_value_ptr->buffer[3] = new & 0x00FF;
  transmit_value_ptr->message_size = 4;
  Enable_Can_Backbone_Int();
  // fill value backbone with fws ask to transmit (not needed because value is never used)
//  ((s_can_backbone_appl_node *)transmit_value_ptr->node_ptr->appl_node_ptr)->fws_ask = new;
}

void Can_Backbone_Set_FWS_Ask_And_Send(s_can_backbone_transmit_value *transmit_value_ptr, int new)
{
  Can_Backbone_Set_FWS_Ask(transmit_value_ptr, new);
  transmit_value_ptr->time_cnt = transmit_value_ptr->time_repeat_max + 1;
}

/*
unsigned char Can_Backbone_Get_FWS_Ask(s_can_backbone_appl_node *appl_node_ptr, unsigned int *actual)
{
  if (appl_node_ptr->fws_ask_ok)
  {
    Disable_Can_Backbone_Int();
    *actual = appl_node_ptr->fws_ask;
    Enable_Can_Backbone_Int();
    return (RECEIVE_VALUE_OK);
  }
  else
  {
    if (appl_node_ptr->fws_ask_time_out_max &&
        (appl_node_ptr->fws_ask_time_cnt >= appl_node_ptr->fws_ask_time_out_max))
      return (RECEIVE_VALUE_ERROR);
    else
      return (RECEIVE_VALUE_WAIT);
  }
}
*/

void Can_Backbone_FWS_Ask_Init(s_can_backbone_appl_node *appl_node_ptr, unsigned char send, int fws_ask)
{
/*
  appl_node_ptr->fws_ask_ok = 0;
  appl_node_ptr->fws_ask_time_reset = 0;
  appl_node_ptr->fws_ask_time_cnt = 0;
  appl_node_ptr->fws_ask_time_out_max = RECEIVE_VALUE_TIMEOUT;
*/
  if (send)
  {
    Can_Backbone_Transmit_Value_Init(&appl_node_ptr->node, 
                                     BROADCAST, 
                                     NO_COMMAND,
                                     Can_Backbone_Transmit_FWS_Ask_Func,
                                     &can_backbone_transmit_fws_ask,
                                     0);
    Can_Backbone_Set_FWS_Ask_And_Send(&can_backbone_transmit_fws_ask, fws_ask);
  }  
}

//*****************************************************************************
// OPT ask
//*****************************************************************************

s_can_backbone_transmit_value can_backbone_transmit_opt_ask;

void Can_Backbone_Transmit_OPT_Ask_Func(s_can_backbone_transmit_value *transmit_value_ptr, unsigned char ok)
// function is called if outside temp is transmitted or if there is a timeout for sending
{
  transmit_value_ptr;
  if (ok)
  {
    // outsidetemp is correct sended
  }
  else
  {
    // outsidetemp is not sended
  }
}

void Can_Backbone_Set_OPT_Ask(s_can_backbone_transmit_value *transmit_value_ptr, int new)
{
  // fill message buffer with fws ask
  Disable_Can_Backbone_Int();
  transmit_value_ptr->buffer[0] = CAN_BACKBONE_ASK_OPT >> 8;
  transmit_value_ptr->buffer[1] = CAN_BACKBONE_ASK_OPT & 0x00FF;
  transmit_value_ptr->buffer[2] = new >> 8;
  transmit_value_ptr->buffer[3] = new & 0x00FF;
  transmit_value_ptr->message_size = 4;
  Enable_Can_Backbone_Int();
  // fill value backbone with fws ask to transmit (not needed because value is never used)
//  ((s_can_backbone_appl_node *)transmit_value_ptr->node_ptr->appl_node_ptr)->fws_ask = new;
}

void Can_Backbone_Set_OPT_Ask_And_Send(s_can_backbone_transmit_value *transmit_value_ptr, int new)
{
  Can_Backbone_Set_OPT_Ask(transmit_value_ptr, new);
  transmit_value_ptr->time_cnt = transmit_value_ptr->time_repeat_max + 1;
}

/*
unsigned char Can_Backbone_Get_OPT_Ask(s_can_backbone_appl_node *appl_node_ptr, unsigned int *actual)
{
  if (appl_node_ptr->opt_ask_ok)
  {
    Disable_Can_Backbone_Int();
    *actual = appl_node_ptr->opt_ask;
    Enable_Can_Backbone_Int();
    return (RECEIVE_VALUE_OK);
  }
  else
  {
    if (appl_node_ptr->opt_ask_time_out_max &&
        (appl_node_ptr->opt_ask_time_cnt >= appl_node_ptr->opt_ask_time_out_max))
      return (RECEIVE_VALUE_ERROR);
    else
      return (RECEIVE_VALUE_WAIT);
  }
}
*/

void Can_Backbone_OPT_Ask_Init(s_can_backbone_appl_node *appl_node_ptr, unsigned char send, int opt_ask)
{
/*
  appl_node_ptr->opt_ask_ok = 0;
  appl_node_ptr->opt_ask_time_reset = 0;
  appl_node_ptr->opt_ask_time_cnt = 0;
  appl_node_ptr->opt_ask_time_out_max = RECEIVE_VALUE_TIMEOUT;
*/
  if (send)
  {
    Can_Backbone_Transmit_Value_Init(&appl_node_ptr->node, 
                                     BROADCAST, 
                                     NO_COMMAND,
                                     Can_Backbone_Transmit_OPT_Ask_Func,
                                     &can_backbone_transmit_opt_ask,
                                     0);
    Can_Backbone_Set_OPT_Ask_And_Send(&can_backbone_transmit_opt_ask, opt_ask);
  }  
}


//*****************************************************************************

void Can_Backbone_Appl_Node_Init(s_can_backbone_appl_node *appl_node_ptr, unsigned int address)
{
  Can_Backbone_Node_Init(&appl_node_ptr->node, address);
  appl_node_ptr->node.appl_node_ptr = appl_node_ptr;
}

//*****************************************************************************
// node 0
//*****************************************************************************
void Can_Backbone_Appl_Node_Alg_Init(unsigned int address)
{
  Can_Backbone_Appl_Node_Init(&can_backbone_appl_node_alg,
                              address); // node address
  Can_Backbone_Alarm_Init(&can_backbone_appl_node_alg);
// begin receive all values
  Can_Backbone_Receive_Value_Init(&can_backbone_appl_node_alg.node,
                                  BROADCAST, // receive address
                                  NO_COMMAND,
                                  Can_Backbone_Receive_Value_Func,
                                  &can_backbone_appl_node_alg.receive_value,
                                  RECEIVE_VALUE_TIMEOUT);
// end receive all values                                  
  Can_Backbone_Clock_In_Sec_Init(&can_backbone_appl_node_alg);
  pc_0_alg.can_slave_buffer = pc_0_slave_buffer;
  pc_0_alg.can_master_buffer = pc_0_master_buffer;
  pc_1_alg.can_slave_buffer = pc_1_slave_buffer;
  pc_1_alg.can_master_buffer = pc_1_master_buffer;
  pc_2_alg.can_slave_buffer = pc_2_slave_buffer;
  pc_2_alg.can_master_buffer = pc_2_master_buffer;
  can_backbone_appl_node_alg.pc_0 = &pc_0_alg;
  can_backbone_appl_node_alg.pc_1 = &pc_1_alg;
  can_backbone_appl_node_alg.pc_2 = &pc_2_alg;
  Can_Backbone_PC_Init(&can_backbone_appl_node_alg);
}

void Can_Backbone_Appl_Node_0_Timing_Control(void)
{
  Can_Backbone_Alarm_Timing_Control(&can_backbone_appl_node_alg);
  Can_Backbone_Outsidetemp_Timing_Control(&can_backbone_appl_node_alg);
  Can_Backbone_Clock_In_Sec_Timing_Control(&can_backbone_appl_node_alg);
  Can_Backbone_Outside_Hunidity_Timing_Control(&can_backbone_appl_node_alg);
  Can_Backbone_Wind_Speed_Timing_Control(&can_backbone_appl_node_alg);
  Can_Backbone_Wind_Direction_Timing_Control(&can_backbone_appl_node_alg);
}

//*****************************************************************************
void Can_Backbone_Appl_Init(void)
{
#ifdef CANopen
  Init_CAN(1, opt_alg.CanBaudrate);
  init_Library();
  if (opt_alg.can_backbone)
	Start_CAN();
  else
    Stop_CAN();
#else // CANopen
  Can_Backbone_Reset();
  Can_Backbone_Appl_Node_Alg_Init(opt_alg.adres);
  if (opt_alg.can_backbone) 
    Can_Backbone_Hardware_Init(CAN_BACKBONE_EXTERN);
  else
    Can_Backbone_Hardware_Init(CAN_BACKBONE_NOT_EXTERN);
#endif // CANopen
  can_backbone_appl_init_switch = 0;  
}

void Can_Backbone_Appl_Control(void)
{
#ifdef CANopen
  if (can_backbone_appl_init_switch)
    Can_Backbone_Appl_Init();
  if (opt_alg.can_backbone)
  {
    FlushMbox();
	servePdos();
  }
#else // CANopen
  if (can_backbone_hardware_busoff_reset_switch)
    Can_Backbone_Hardware_Busoff_Reset();

  if (can_backbone_appl_init_switch)
    Can_Backbone_Appl_Init();
  Can_Backbone_Control();
#endif // CANopen
}

#ifdef CAN_BACKBONE_PC_WARNING
void Can_Backbone_PC_Time_Control(void)
{
 int loop;
 
  for (loop = 0; loop < 3; loop++)
  {
    if (opt_alg.can_backbone_pc_warning[loop])
      pc_alarm_time_cnt[loop]++;
    else
      pc_alarm_time_cnt[loop] = 0;
  }
  if (pc_alarm_time_cnt[0] > RECEIVE_ALARM_TIMEOUT)
    CreateAlarm(&alarm_hr_alg.can_backbone_pc_warning[0], CAN_PC_1_MASTER_AL, 0, 0, 0, ZACHT_ALARM);
  else  
    ClearAlarm(&alarm_hr_alg.can_backbone_pc_warning[0], CAN_PC_1_MASTER_AL, 0);
  if (pc_alarm_time_cnt[1] > RECEIVE_ALARM_TIMEOUT)
    CreateAlarm(&alarm_hr_alg.can_backbone_pc_warning[1], CAN_PC_2_MASTER_AL, 0, 0, 0, ZACHT_ALARM);
  else  
    ClearAlarm(&alarm_hr_alg.can_backbone_pc_warning[1], CAN_PC_2_MASTER_AL, 0);
  if (pc_alarm_time_cnt[2] > RECEIVE_ALARM_TIMEOUT)
    CreateAlarm(&alarm_hr_alg.can_backbone_pc_warning[2], CAN_PC_3_MASTER_AL, 0, 0, 0, ZACHT_ALARM);
  else  
    ClearAlarm(&alarm_hr_alg.can_backbone_pc_warning[2], CAN_PC_3_MASTER_AL, 0);
}
#endif // CAN_BACKBONE_PC_WARNING

void Can_Backbone_Appl_Timing_Control(void)
{
  #ifdef CAN_BACKBONE_PC_WARNING
  Can_Backbone_PC_Time_Control();
  #endif // CAN_BACKBONE_PC_WARNING
  Can_Backbone_Appl_Node_0_Timing_Control();
  Can_Backbone_Timing_Control();
}

void Can_Backbone_Address_Check(void)
// check if one can backbone address is changed
// if so then init can backbone again
{
  if (opt_alg.adres != can_backbone_appl_node_alg.node.address_node)
    can_backbone_appl_init_switch = 1;  
}




                                                  