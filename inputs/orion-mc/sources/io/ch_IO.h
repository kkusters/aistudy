// CH_IO.H

#ifndef _CH_IO_H
#define _CH_IO_H

#include "ch_motor.h"

#define TEMP_FOUT  0x8000

#define OPTION_POSITION   0x0001
#define OPTION_OPEN_CLOSE 0x0002
#define OPTION_DUALSCREEN 0x0003
#define OPTION_TEST_MODE  0x000A

#define OPTION_SINGLE_WINDOW 0x0010
#define OPTION_SINGLE_SCREEN 0x0020

int IO_Get_Ana_In_One(s_board_IO_on_off *IO);
int IO_Get_Ana_In(s_board_IO_on_off *IO, unsigned char max);
int IO_Get_Dig_In_Status(s_board_IO_on_off *IO);
unsigned long IO_Get_Dig_In_Count(s_board_IO_on_off *IO);
void IO_Set_Ana_Out(s_board_IO_on_off *IO, int val);
void IO_Get_Comp_Ana_Out(s_board_IO_on_off *IO, s_option_analog_output *ana_out);
void IO_Set_Comp_Ana_Out(s_board_IO_on_off *IO, s_option_analog_output *ana_out);
void IO_Set_Dig_Out(s_board_IO_on_off *IO, unsigned char on_off);
unsigned char IO_Get_Dig_Out(s_board_IO_on_off *IO);

void IO_Set_Motor_Control_Option(s_board_IO_on_off *IO, unsigned char External);
int IO_Get_Motor_Control_Option(s_board_IO_on_off *IO);
unsigned char IO_Set_Motor_Control_Position(s_board_IO_on_off *IO, int val);
void IO_Set_Motor_Control_Hold(s_board_IO_on_off *IO, TRunningMode RunningMode);
void IO_Set_Motor_Control_Release(s_board_IO_on_off *IO, TRunningMode RunningMode);
void IO_Set_Motor_Control_HiSpeed(s_board_IO_on_off *IO, unsigned char val);
int IO_Get_Motor_Control_HiSpeed(s_board_IO_on_off *IO);
int IO_Get_Motor_Control_Position(s_board_IO_on_off *IO);
int IO_Get_Motor_Control_Runtime(s_board_IO_on_off *IO);
int IO_Get_Motor_Control_State(s_board_IO_on_off *IO);
int IO_Get_Motor_Alarm_Code(s_board_IO_on_off *IO);
int IO_Get_Motor_Alarm_Slave(s_board_IO_on_off *IO);
void IO_Get_Motor_Control_MotorManagement(s_board_IO_on_off *IO, TMotorManagement *MotorManagement);
void IO_Update_Motor_Control_MotorManagement(s_board_IO_on_off *IO, TMotorManagement *MotorManagement);
void IO_Set_Motor_Control_Frequency(s_board_IO_on_off *IO, unsigned char SpeedLow, unsigned char SpeedHi, int PositionLowSpeedClose, int PositionLowSpeedOpen);
int IO_Get_Board_Alarm(s_board_IO_on_off *IO);

//================================================================================================
typedef enum {
  RS485ModeOff = 0,
  RS485ModeTransparant,
  RS485ModeModbus
} TRS485Mode;

void IO_Set_RS485_Mode(s_board_IO_on_off *IO, TRS485Mode RS485Mode);
void IO_RS485_Stop_All_Periodic_Messages(void);
unsigned char IO_Set_RS485_Message(s_board_IO_on_off *IO, unsigned char *Data, int DataSize);
void IO_Reset_RS485_TxBuffer(s_board_IO_on_off *IO); // JP 03-12-15
void IO_Reset_RS485_RxBuffer(s_board_IO_on_off *IO);
unsigned char IO_RS485_RxBuffer_Size(s_board_IO_on_off *IO);
unsigned char IO_RS485_MessageReady(s_board_IO_on_off *IO);
unsigned char IO_Get_RS485_Char(s_board_IO_on_off *IO);

//================================================================================================
unsigned char IOCreateObject(s_board_IO_on_off *IO, unsigned char ComponentId, void *opt);

#endif
