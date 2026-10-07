// C__CAN_BACKBONE_PC.C

#include "ch_define.h"
#include "ch_alg.h"
#include "ch_can_backbone_appl.h"
#include "ch_pc_com.h"
#include "ch_can_backbone_pc.h"

char can_backbone_command_buffer[MAX_PC_BUFFER];
char can_backbone_answer_buffer[MAX_PC_BUFFER];
char Can_Backbone_OPT_command_buffer[MAX_PC_BUFFER]; // input buffer for data from an PC
char Can_Backbone_OPT_answer_buffer[MAX_PC_BUFFER]; // output buffer for data from an PC

unsigned char can_backbone_pc_0_receive_buffer[MAX_BUFFER_COM];
unsigned char can_backbone_pc_1_receive_buffer[MAX_BUFFER_COM];
unsigned char can_backbone_pc_2_receive_buffer[MAX_BUFFER_COM];

s_pc_com can_backbone_pc_0_com = { can_backbone_pc_0_receive_buffer,0,0,0,0,0,0,0,0,0,0 };
s_pc_com can_backbone_pc_1_com = { can_backbone_pc_1_receive_buffer,0,0,0,0,0,0,0,0,0,0 };
s_pc_com can_backbone_pc_2_com = { can_backbone_pc_2_receive_buffer,0,0,0,0,0,0,0,0,0,0 };

void Can_Pack_Message(char *packed_ptr, char *unpacked_ptr)
{
int loop;
int length_data;

  length_data = (Asc_To_Hex(unpacked_ptr[13]) << 4) + Asc_To_Hex(unpacked_ptr[14]);
  unpacked_ptr += 7;
  for (loop = 0; loop < length_data + 5; loop++)
  {
    *packed_ptr = (Asc_To_Hex(unpacked_ptr[0]) << 4) + Asc_To_Hex(unpacked_ptr[1]);
    packed_ptr++;
    unpacked_ptr += 2;
  }
  *packed_ptr = *unpacked_ptr;
}

void Can_Unpack_Message(char *unpacked_ptr, 
                        char *packed_ptr, 
                        unsigned int address_receiver, 
                        unsigned int address_sender)
{
int length_data;
int loop;

  length_data = (unsigned char)packed_ptr[3];
  *unpacked_ptr = '@'; unpacked_ptr++;
  *unpacked_ptr = Hex_To_Asc((address_receiver >> 8) & 0x000F); unpacked_ptr++;
  *unpacked_ptr = Hex_To_Asc((address_receiver >> 4) & 0x000F); unpacked_ptr++;
  *unpacked_ptr = Hex_To_Asc(address_receiver & 0x000F); unpacked_ptr++;
  *unpacked_ptr = Hex_To_Asc((address_sender >> 8) & 0x000F); unpacked_ptr++;
  *unpacked_ptr = Hex_To_Asc((address_sender >> 4) & 0x000F); unpacked_ptr++;
  *unpacked_ptr = Hex_To_Asc(address_sender & 0x000F); unpacked_ptr++;
  for (loop = 0; loop < length_data + 5; loop++)
  {
    *unpacked_ptr = Hex_To_Asc(*packed_ptr >> 4); unpacked_ptr++;
    *unpacked_ptr = Hex_To_Asc(*packed_ptr & 0x0F); unpacked_ptr++; packed_ptr++;
  }
  *unpacked_ptr = *packed_ptr; unpacked_ptr++;
  *unpacked_ptr = ETX; unpacked_ptr++;
  *unpacked_ptr = 0;
}

void Can_Backbone_PC_Control(void)
{
int command = NOCOMMAND;

//  if (opt_alg.can_backbone)
  {
    if (can_backbone_appl_node_alg.pc_0->can_slave_message_ready)
    {
      can_backbone_appl_node_alg.pc_0->can_slave_message_ready = 0;
      Can_Unpack_Message(can_backbone_command_buffer, 
                         can_backbone_appl_node_alg.pc_0->can_slave_buffer,
                         can_backbone_appl_node_alg.node.address_node,
                         can_backbone_appl_node_alg.pc_0->slave.address_sender);
      if (Controleer_Data(&command, 
                          can_backbone_command_buffer,
                          can_backbone_answer_buffer) == 0)
      {
        Verwerk_Data(&can_backbone_pc_0_com);
        if (can_backbone_answer_buffer[0] != 0) // JP 11-06-08
        {
          can_backbone_appl_node_alg.pc_0->master.fast = can_backbone_pc_0_com.fast;
          Can_Pack_Message(can_backbone_appl_node_alg.pc_0->can_master_buffer,can_backbone_answer_buffer);
          Can_Backbone_Set_Master_PC_0_And_Send(&can_backbone_appl_node_alg, 
                                                can_backbone_appl_node_alg.pc_0->can_master_buffer);
        }
      }  
    }
    if (can_backbone_appl_node_alg.pc_1->can_slave_message_ready)
    {
      can_backbone_appl_node_alg.pc_1->can_slave_message_ready = 0;
      Can_Unpack_Message(can_backbone_command_buffer, 
                         can_backbone_appl_node_alg.pc_1->can_slave_buffer,
                         can_backbone_appl_node_alg.node.address_node,
                         can_backbone_appl_node_alg.pc_1->slave.address_sender);
      if (Controleer_Data(&command, 
                          can_backbone_command_buffer,
                          can_backbone_answer_buffer) == 0)
      {
        Verwerk_Data(&can_backbone_pc_1_com);
        if (can_backbone_answer_buffer[0] != 0) // JP 11-06-08
        {
          can_backbone_appl_node_alg.pc_1->master.fast = can_backbone_pc_1_com.fast;
          Can_Pack_Message(can_backbone_appl_node_alg.pc_1->can_master_buffer,can_backbone_answer_buffer);
          Can_Backbone_Set_Master_PC_1_And_Send(&can_backbone_appl_node_alg, 
                                                can_backbone_appl_node_alg.pc_1->can_master_buffer);
        }
      }  
    }
    if (can_backbone_appl_node_alg.pc_2->can_slave_message_ready)
    {
      can_backbone_appl_node_alg.pc_2->can_slave_message_ready = 0;
      Can_Unpack_Message(can_backbone_command_buffer, 
                         can_backbone_appl_node_alg.pc_2->can_slave_buffer,
                         can_backbone_appl_node_alg.node.address_node,
                         can_backbone_appl_node_alg.pc_2->slave.address_sender);
      if (Controleer_Data(&command, 
                          can_backbone_command_buffer,
                          can_backbone_answer_buffer) == 0)
      {
        Verwerk_Data(&can_backbone_pc_2_com);
        if (can_backbone_answer_buffer[0] != 0) // JP 11-06-08
        {
          can_backbone_appl_node_alg.pc_2->master.fast = can_backbone_pc_2_com.fast;
          Can_Pack_Message(can_backbone_appl_node_alg.pc_2->can_master_buffer,can_backbone_answer_buffer);
          Can_Backbone_Set_Master_PC_2_And_Send(&can_backbone_appl_node_alg, 
                                                can_backbone_appl_node_alg.pc_2->can_master_buffer);
        }
      }  
    }
  }  
}
