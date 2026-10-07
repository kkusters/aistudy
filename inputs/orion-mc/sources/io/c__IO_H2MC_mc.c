// CH_IO_H2MC_MC.C

#include "ch_define.h"

#include "ch_const.h"
#include "ch_event.h"
#include "ch_IO_H2MC_mc.h"

unsigned char IO_H2MC_Motor_Control_PDO_Transmit(unsigned char *ptr, unsigned char card_nr, unsigned char component_nr)
{
unsigned int i;

  if ((component_nr < IO_H2MC_MOTOR_CONTROL) &&
      (opt_io.IO_H2MC[card_nr].motor_control[component_nr].Option & 0x000F))
  {
    i = val_hr_alg.IO_H2MC[card_nr].motor_control[component_nr].Position;
    ptr[0] = i & 0x00FF;
    ptr[1] = i >> 8;
    i = val_hr_alg.IO_H2MC[card_nr].motor_control[component_nr].Ctrl;
    ptr[2] = i & 0x00FF;
    ptr[3] = i >> 8;
    value.IO_H2MC[card_nr].motor_control[component_nr].ctrl &= 0xFFFE;
    value.IO_H2MC[card_nr].motor_control[component_nr].ctrl |= 0x0100;
    return (4);
  }
  return (0);
}

void IO_H2MC_Motor_Control_PDO_Transmit_OK(unsigned char card_nr, unsigned char component_nr)
{
  if ((component_nr < IO_H2MC_MOTOR_CONTROL) &&
      (opt_io.IO_H2MC[card_nr].motor_control[component_nr].Option & 0x000F))
  {
    value.IO_H2MC[card_nr].motor_control[component_nr].ctrl &= 0xFEFF;
  }
}

void IO_H2MC_Motor_Control_PDO_Receive(unsigned char *ptr, unsigned char length, unsigned char card_nr, unsigned char component_nr)
{
int help;

  if ((component_nr < IO_H2MC_MOTOR_CONTROL) &&
      ((length == 6) || (length == 8)) &&
      (opt_io.IO_H2MC[card_nr].motor_control[component_nr].Option & 0x000F))
  {
    help = (ptr[1] << 8) + ptr[0];
    val_hr_alg.IO_H2MC[card_nr].motor_control[component_nr].Value = help;
    help = (ptr[3] << 8) + ptr[2];
    val_hr_alg.IO_H2MC[card_nr].motor_control[component_nr].Status = help;
    help = (ptr[5] << 8) + ptr[4];
    val_hr_alg.IO_H2MC[card_nr].motor_control[component_nr].Status2 = help;

//    help = (ptr[7] << 8) + ptr[6];
//    value.IO_H2MC[card_nr].motor_control[component_nr].Speed = help;

    if (val_hr_alg.IO_H2MC[card_nr].motor_control[component_nr].Status & 0x0008)
      value.IO_H2MC[card_nr].motor_control[component_nr].ctrl |= 0x0008;

    // PDO event
    if (event_list.IO_H2MC[card_nr].PDO_received != NULL)
	{
	  s_board_IO_on_off IO;

	  IO.board_type = IO_H2MC_ID;
	  IO.board_nr   = card_nr;
	  IO.IO_nr      = component_nr;
	  event_list.IO_H2MC[card_nr].PDO_received(&IO);
	}
  }
}

unsigned int IO_H2MC_Motor_Control_SDO_Master_Transmit(u_SDO_data *ptr, unsigned char card_nr, unsigned char component_nr)
{
  if (component_nr < IO_H2MC_MOTOR_CONTROL) 
  {
    ptr->motor_control.Command                 = val_hr_alg.IO_H2MC[card_nr].motor_control[component_nr].Command;
    ptr->motor_control.Option                  = opt_io.IO_H2MC[card_nr].motor_control[component_nr].Option;
    ptr->motor_control.MinIn                   = opt_io.IO_H2MC[card_nr].motor_control[component_nr].MinIn;  
    ptr->motor_control.MaxIn                   = opt_io.IO_H2MC[card_nr].motor_control[component_nr].MaxIn;
    ptr->motor_control.MinOut                  = opt_io.IO_H2MC[card_nr].motor_control[component_nr].MinOut;
    ptr->motor_control.MaxOut                  = opt_io.IO_H2MC[card_nr].motor_control[component_nr].MaxOut;
    ptr->motor_control.Runtime                 = opt_io.IO_H2MC[card_nr].motor_control[component_nr].Runtime;
    ptr->motor_control.MaxDev                  = opt_io.IO_H2MC[card_nr].motor_control[component_nr].MaxDev;
    ptr->motor_control.StDigIn                 = value.IO_H2MC[card_nr].motor_control[component_nr].StDigIn;
    ptr->motor_control.StDigOut                = value.IO_H2MC[card_nr].motor_control[component_nr].StDigOut;
    ptr->motor_control.StFeedback              = value.IO_H2MC[card_nr].motor_control[component_nr].StFeedback;
    ptr->motor_control.StAnaIn_0_10V           = value.IO_H2MC[card_nr].motor_control[component_nr].StAnaIn_0_10V;
    ptr->motor_control.StAnaOut_0_5V           = value.IO_H2MC[card_nr].motor_control[component_nr].StAnaOut_0_5V;
    ptr->motor_control.SpeedLow                = opt_io.IO_H2MC[card_nr].motor_control[component_nr].SpeedLow;
    ptr->motor_control.SpeedHi                 = opt_io.IO_H2MC[card_nr].motor_control[component_nr].SpeedHi;
	ptr->motor_control.PosLowSpeedClose        = opt_io.IO_H2MC[card_nr].motor_control[component_nr].PosLowSpeedClose;
	ptr->motor_control.PosLowSpeedOpen         = opt_io.IO_H2MC[card_nr].motor_control[component_nr].PosLowSpeedOpen;
    ptr->motor_control.MotorManagementRuntime  = val_hr_alg.IO_H2MC[card_nr].motor_control[component_nr].MotorManagementRuntime;
    ptr->motor_control.MotorManagementSwitches = val_hr_alg.IO_H2MC[card_nr].motor_control[component_nr].MotorManagementSwitches;
    ptr->motor_control.MotorManagementFailures = val_hr_alg.IO_H2MC[card_nr].motor_control[component_nr].MotorManagementFailures;
//	ptr->motor_control.NotUsed                 = opt_io.IO_H2MC[card_nr].motor_control[component_nr].NotUsed;
//	ptr->motor_control.NotUsed                 = opt_io.IO_H2MC[card_nr].motor_control[component_nr].NotUsed;
    ptr->motor_control.Difference              = opt_io.IO_H2MC[card_nr].motor_control[component_nr].Difference;
    ptr->motor_control.IntervalTime            = opt_io.IO_H2MC[card_nr].motor_control[component_nr].IntervalTime;
    ptr->motor_control.ClassNr                 = opt_io.IO_H2MC[card_nr].motor_control[component_nr].ClassNr;
    return (sizeof(s_motor_control));
  }
  return (0);
}

void IO_H2MC_Motor_Control_SDO_Master_Transmit_OK(unsigned char card_nr, unsigned char component_nr)
{
  if (component_nr < IO_H2MC_MOTOR_CONTROL) 
  {
    value.IO_H2MC[card_nr].motor_control[component_nr].ctrl &= 0xFFFB;
  }
}

void IO_H2MC_Motor_Control_SDO_Master_Receive(u_SDO_data *ptr, unsigned char card_nr, unsigned char component_nr)
{
  if (component_nr < IO_H2MC_MOTOR_CONTROL) 
  {
//  val_hr_alg.IO_H2MC[card_nr].motor_control[component_nr].Command                 = ptr->motor_control.Command;
//  opt_io.IO_H2MC[card_nr].motor_control[component_nr].Option                     = ptr->motor_control.Option;
//  opt_io.IO_H2MC[card_nr].motor_control[component_nr].MinIn                      = ptr->motor_control.MinIn;
//  opt_io.IO_H2MC[card_nr].motor_control[component_nr].MaxIn                      = ptr->motor_control.MaxIn;
//  opt_io.IO_H2MC[card_nr].motor_control[component_nr].MinOut                     = ptr->motor_control.MinOut;
//  opt_io.IO_H2MC[card_nr].motor_control[component_nr].MaxOut                     = ptr->motor_control.MaxOut;
    opt_io.IO_H2MC[card_nr].motor_control[component_nr].Runtime                    = ptr->motor_control.Runtime;
//  opt_io.IO_H2MC[card_nr].motor_control[component_nr].MaxDev                     = ptr->motor_control.MaxDev;
    value.IO_H2MC[card_nr].motor_control[component_nr].StDigIn                      = ptr->motor_control.StDigIn;
    value.IO_H2MC[card_nr].motor_control[component_nr].StDigOut                     = ptr->motor_control.StDigOut;
    value.IO_H2MC[card_nr].motor_control[component_nr].StFeedback                   = ptr->motor_control.StFeedback;
    value.IO_H2MC[card_nr].motor_control[component_nr].StAnaIn_0_10V                = ptr->motor_control.StAnaIn_0_10V;
    value.IO_H2MC[card_nr].motor_control[component_nr].StAnaOut_0_5V                = ptr->motor_control.StAnaOut_0_5V;
//  opt_io.IO_H2MC[card_nr].motor_control[component_nr].SpeedLow                   = ptr->motor_control.SpeedLow;
//  opt_io.IO_H2MC[card_nr].motor_control[component_nr].SpeedHi                    = ptr->motor_control.SpeedHi;
//	opt_io.IO_H2MC[card_nr].motor_control[component_nr].PosLowSpeedClose           = ptr->motor_control.PosLowSpeedClose;
//	opt_io.IO_H2MC[card_nr].motor_control[component_nr].PosLowSpeedOpen            = ptr->motor_control.PosLowSpeedOpen;
    val_hr_alg.IO_H2MC[card_nr].motor_control[component_nr].MotorManagementRuntime  = ptr->motor_control.MotorManagementRuntime;
    val_hr_alg.IO_H2MC[card_nr].motor_control[component_nr].MotorManagementSwitches = ptr->motor_control.MotorManagementSwitches;
    val_hr_alg.IO_H2MC[card_nr].motor_control[component_nr].MotorManagementFailures = ptr->motor_control.MotorManagementFailures;
//	opt_io.IO_H2MC[card_nr].motor_control[component_nr].NotUsed_1                  = ptr->motor_control.NotUsed_1;
//	opt_io.IO_H2MC[card_nr].motor_control[component_nr].NotUsed_2                  = ptr->motor_control.NotUsed_2;
//  opt_io.IO_H2MC[card_nr].motor_control[component_nr].Difference                 = ptr->motor_control.Difference;
//  opt_io.IO_H2MC[card_nr].motor_control[component_nr].IntervalTime               = ptr->motor_control.IntervalTime;
//  opt_io.IO_H2MC[card_nr].motor_control[component_nr].ClassNr                    = ptr->motor_control.ClassNr;
    value.IO_H2MC[card_nr].motor_control[component_nr].ctrl &= 0xFFF7;

    // SDO event
    if (event_list.IO_H2MC[card_nr].SDO_received != NULL)
	{
	  s_board_IO_on_off IO;

	  IO.board_type = IO_H2MC_ID;
	  IO.board_nr   = card_nr;
	  IO.IO_nr      = component_nr;
	  event_list.IO_H2MC[card_nr].SDO_received(&IO);
	}
  }
}

void IO_H2MC_Motor_Control_Init(unsigned char card_nr)
// wordt aan geroepen als can opnieuw geinitialiseerd wordt
{
unsigned char comp_nr;

  for (comp_nr = 0; comp_nr < IO_H2MC_MOTOR_CONTROL; comp_nr++)
  {
    value.IO_H2MC[card_nr].motor_control[comp_nr].ctrl = 0x0000;
    value.IO_H2MC[card_nr].motor_control[comp_nr].interval_timer = rom.IO_H2MC[card_nr].motor_control[comp_nr].interval_time;

    switch (opt_io.IO_H2MC[card_nr].motor_control[comp_nr].OptType)
	{
	  case MOTOR_CONTROL_RAAM:
	    opt_io.IO_H2MC[card_nr].motor_control[comp_nr].Difference   = option_motor_control_raam.Difference;
	    opt_io.IO_H2MC[card_nr].motor_control[comp_nr].IntervalTime = option_motor_control_raam.IntervalTime;
	    break;
	  case MOTOR_CONTROL_DOEK:
	    opt_io.IO_H2MC[card_nr].motor_control[comp_nr].Difference   = option_motor_control_doek.Difference;
	    opt_io.IO_H2MC[card_nr].motor_control[comp_nr].IntervalTime = option_motor_control_doek.IntervalTime;
	    break;
	}
  }
}

void IO_H2MC_Motor_Control_100ms(unsigned char card_nr)
// wordt elke 100ms aangeroepen om te kijken of er periodieke data verzonden moet worden
{
unsigned char comp_nr;

  for (comp_nr = 0; comp_nr < IO_H2MC_MOTOR_CONTROL; comp_nr++)
  {
    if ((opt_io.IO_H2MC[card_nr].motor_control[comp_nr].Option & 0x000F) &&
        (rom.IO_H2MC[card_nr].motor_control[comp_nr].interval_time)) 
    {
      if (value.IO_H2MC[card_nr].motor_control[comp_nr].interval_timer)
        value.IO_H2MC[card_nr].motor_control[comp_nr].interval_timer--;
      else
      {
        value.IO_H2MC[card_nr].motor_control[comp_nr].interval_timer = rom.IO_H2MC[card_nr].motor_control[comp_nr].interval_time;
        value.IO_H2MC[card_nr].motor_control[comp_nr].ctrl |= 0x0001;
      }
    }
  }
}

