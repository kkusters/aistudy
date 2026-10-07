// CH_IO_EKU_MC.C

#include "ch_define.h"

#include "ch_event.h"
#include "ch_IO_EKU_mc.h"

unsigned char IO_EKU_Motor_Control_PDO_Transmit(unsigned char *ptr, unsigned char card_nr, unsigned char component_nr)
{
unsigned int i;

  if ((component_nr < IO_EKU_MOTOR_CONTROL) &&
      (opt_io.IO_EKU[card_nr].motor_control.Option & 0x000F))
  {
    i = val_hr_alg.IO_EKU[card_nr].motor_control.Position;
    ptr[0] = i & 0x00FF;
    ptr[1] = i >> 8;
    i = val_hr_alg.IO_EKU[card_nr].motor_control.Ctrl;
    ptr[2] = i & 0x00FF;
    ptr[3] = i >> 8;
    value.IO_EKU[card_nr].motor_control.ctrl &= 0xFFFE;
    value.IO_EKU[card_nr].motor_control.ctrl |= 0x0100;
    return (4);
  }
  return (0);
}

void IO_EKU_Motor_Control_PDO_Transmit_OK(unsigned char card_nr, unsigned char component_nr)
{
  if ((component_nr < IO_EKU_MOTOR_CONTROL) &&
      (opt_io.IO_EKU[card_nr].motor_control.Option & 0x000F))
  {
    value.IO_EKU[card_nr].motor_control.ctrl &= 0xFEFF;
  }
}

void IO_EKU_Motor_Control_PDO_Receive(unsigned char *ptr, unsigned char length, unsigned char card_nr, unsigned char component_nr)
{
int help;

  if ((component_nr < IO_EKU_MOTOR_CONTROL) &&
      ((length == 6) || (length == 8)) &&
      (opt_io.IO_EKU[card_nr].motor_control.Option & 0x000F))
  {
    help = (ptr[1] << 8) + ptr[0];
    val_hr_alg.IO_EKU[card_nr].motor_control.Value = help;
    help = (ptr[3] << 8) + ptr[2];
    val_hr_alg.IO_EKU[card_nr].motor_control.Status = help;
    help = (ptr[5] << 8) + ptr[4];
    val_hr_alg.IO_EKU[card_nr].motor_control.Status2 = help;

//    help = (ptr[7] << 8) + ptr[6];
//    value.IO_EKU[card_nr].motor_control.Speed = help;

    if (val_hr_alg.IO_EKU[card_nr].motor_control.Status & 0x0008)
      value.IO_EKU[card_nr].motor_control.ctrl |= 0x0008;

    // PDO event
    if (event_list.IO_EKU[card_nr].PDO_received != NULL)
	{
	  s_board_IO_on_off IO;

	  IO.board_type = IO_EKU_ID;
	  IO.board_nr   = card_nr;
	  IO.IO_nr      = component_nr;
	  event_list.IO_EKU[card_nr].PDO_received(&IO);
	}
  }
}

unsigned int IO_EKU_Motor_Control_SDO_Master_Transmit(u_SDO_data *ptr, unsigned char card_nr, unsigned char component_nr)
{
  if (component_nr < IO_EKU_MOTOR_CONTROL) 
  {
    ptr->motor_control.Command                 = val_hr_alg.IO_EKU[card_nr].motor_control.Command;
    ptr->motor_control.Option                  = opt_io.IO_EKU[card_nr].motor_control.Option;
    ptr->motor_control.MinIn                   = opt_io.IO_EKU[card_nr].motor_control.MinIn;  
    ptr->motor_control.MaxIn                   = opt_io.IO_EKU[card_nr].motor_control.MaxIn;
    ptr->motor_control.MinOut                  = opt_io.IO_EKU[card_nr].motor_control.MinOut;
    ptr->motor_control.MaxOut                  = opt_io.IO_EKU[card_nr].motor_control.MaxOut;
    ptr->motor_control.Runtime                 = opt_io.IO_EKU[card_nr].motor_control.Runtime;
    ptr->motor_control.MaxDev                  = opt_io.IO_EKU[card_nr].motor_control.MaxDev;
    ptr->motor_control.StDigIn                 = value.IO_EKU[card_nr].motor_control.StDigIn;
    ptr->motor_control.StDigOut                = value.IO_EKU[card_nr].motor_control.StDigOut;
    ptr->motor_control.StFeedback              = value.IO_EKU[card_nr].motor_control.StFeedback;
    ptr->motor_control.StAnaIn_0_10V           = value.IO_EKU[card_nr].motor_control.StAnaIn_0_10V;
    ptr->motor_control.StAnaOut_0_5V           = value.IO_EKU[card_nr].motor_control.StAnaOut_0_5V;
    ptr->motor_control.SpeedLow                = opt_io.IO_EKU[card_nr].motor_control.SpeedLow;
    ptr->motor_control.SpeedHi                 = opt_io.IO_EKU[card_nr].motor_control.SpeedHi;
	ptr->motor_control.PosLowSpeedClose        = opt_io.IO_EKU[card_nr].motor_control.PosLowSpeedClose;
	ptr->motor_control.PosLowSpeedOpen         = opt_io.IO_EKU[card_nr].motor_control.PosLowSpeedOpen;
    ptr->motor_control.MotorManagementRuntime  = val_hr_alg.IO_EKU[card_nr].motor_control.MotorManagementRuntime;
    ptr->motor_control.MotorManagementSwitches = val_hr_alg.IO_EKU[card_nr].motor_control.MotorManagementSwitches;
    ptr->motor_control.MotorManagementFailures = val_hr_alg.IO_EKU[card_nr].motor_control.MotorManagementFailures;
//	ptr->motor_control.NotUsed                 = opt_io.IO_EKU[card_nr].motor_control.NotUsed;
//	ptr->motor_control.NotUsed                 = opt_io.IO_EKU[card_nr].motor_control.NotUsed;
    ptr->motor_control.Difference              = opt_io.IO_EKU[card_nr].motor_control.Difference;
    ptr->motor_control.IntervalTime            = opt_io.IO_EKU[card_nr].motor_control.IntervalTime;
    ptr->motor_control.ClassNr                 = opt_io.IO_EKU[card_nr].motor_control.ClassNr;
    return (sizeof(s_motor_control));
  }
  return (0);
}

void IO_EKU_Motor_Control_SDO_Master_Transmit_OK(unsigned char card_nr, unsigned char component_nr)
{
  if (component_nr < IO_EKU_MOTOR_CONTROL) 
  {
    value.IO_EKU[card_nr].motor_control.ctrl &= 0xFFFB;
  }
}

void IO_EKU_Motor_Control_SDO_Master_Receive(u_SDO_data *ptr, unsigned char card_nr, unsigned char component_nr)
{
  if (component_nr < IO_EKU_MOTOR_CONTROL) 
  {
//  val_hr_alg.IO_EKU[card_nr].motor_control.Command                 = ptr->motor_control.Command;
//  opt_io.IO_EKU[card_nr].motor_control.Option                      = ptr->motor_control.Option;
//  opt_io.IO_EKU[card_nr].motor_control.MinIn                       = ptr->motor_control.MinIn;
//  opt_io.IO_EKU[card_nr].motor_control.MaxIn                       = ptr->motor_control.MaxIn;
//  opt_io.IO_EKU[card_nr].motor_control.MinOut                      = ptr->motor_control.MinOut;
//  opt_io.IO_EKU[card_nr].motor_control.MaxOut                      = ptr->motor_control.MaxOut;
    opt_io.IO_EKU[card_nr].motor_control.Runtime                     = ptr->motor_control.Runtime;
//  opt_io.IO_EKU[card_nr].motor_control.MaxDev                      = ptr->motor_control.MaxDev;
    value.IO_EKU[card_nr].motor_control.StDigIn                      = ptr->motor_control.StDigIn;
    value.IO_EKU[card_nr].motor_control.StDigOut                     = ptr->motor_control.StDigOut;
    value.IO_EKU[card_nr].motor_control.StFeedback                   = ptr->motor_control.StFeedback;
    value.IO_EKU[card_nr].motor_control.StAnaIn_0_10V                = ptr->motor_control.StAnaIn_0_10V;
    value.IO_EKU[card_nr].motor_control.StAnaOut_0_5V                = ptr->motor_control.StAnaOut_0_5V;
//  opt_io.IO_EKU[card_nr].motor_control.SpeedLow                    = ptr->motor_control.SpeedLow;
//  opt_io.IO_EKU[card_nr].motor_control.SpeedHi                     = ptr->motor_control.SpeedHi;
//	opt_io.IO_EKU[card_nr].motor_control.PosLowSpeedClose            = ptr->motor_control.PosLowSpeedClose;
//	opt_io.IO_EKU[card_nr].motor_control.PosLowSpeedOpen             = ptr->motor_control.PosLowSpeedOpen;
    val_hr_alg.IO_EKU[card_nr].motor_control.MotorManagementRuntime  = ptr->motor_control.MotorManagementRuntime;
    val_hr_alg.IO_EKU[card_nr].motor_control.MotorManagementSwitches = ptr->motor_control.MotorManagementSwitches;
    val_hr_alg.IO_EKU[card_nr].motor_control.MotorManagementFailures = ptr->motor_control.MotorManagementFailures;
//	opt_io.IO_EKU[card_nr].motor_control.NotUsed_1                   = ptr->motor_control.NotUsed_1;
//	opt_io.IO_EKU[card_nr].motor_control.NotUsed_2                   = ptr->motor_control.NotUsed_2;
//  opt_io.IO_EKU[card_nr].motor_control.Difference                  = ptr->motor_control.Difference;
//  opt_io.IO_EKU[card_nr].motor_control.IntervalTime                = ptr->motor_control.IntervalTime;
//  opt_io.IO_EKU[card_nr].motor_control.ClassNr                     = ptr->motor_control.ClassNr;
    value.IO_EKU[card_nr].motor_control.ctrl &= 0xFFF7;

    // SDO event
    if (event_list.IO_EKU[card_nr].SDO_received != NULL)
	{
	  s_board_IO_on_off IO;

	  IO.board_type = IO_EKU_ID;
	  IO.board_nr   = card_nr;
	  IO.IO_nr      = component_nr;
	  event_list.IO_EKU[card_nr].SDO_received(&IO);
	}
  }
}

void IO_EKU_Motor_Control_Init(unsigned char card_nr)
// wordt aan geroepen als can opnieuw geinitialiseerd wordt
{
unsigned char comp_nr;

  for (comp_nr = 0; comp_nr < IO_EKU_MOTOR_CONTROL; comp_nr++)
  {
    value.IO_EKU[card_nr].motor_control.ctrl = 0x0000;
    value.IO_EKU[card_nr].motor_control.interval_timer = rom.IO_EKU[card_nr].motor_control.interval_time;
  }
}

void IO_EKU_Motor_Control_100ms(unsigned char card_nr)
// wordt elke 100ms aangeroepen om te kijken of er periodieke data verzonden moet worden
{
unsigned char comp_nr;

  for (comp_nr = 0; comp_nr < IO_EKU_MOTOR_CONTROL; comp_nr++)
  {
    if ((opt_io.IO_EKU[card_nr].motor_control.Option & 0x000F) &&
        (rom.IO_EKU[card_nr].motor_control.interval_time)) 
    {
      if (value.IO_EKU[card_nr].motor_control.interval_timer)
        value.IO_EKU[card_nr].motor_control.interval_timer--;
      else
      {
        value.IO_EKU[card_nr].motor_control.interval_timer = rom.IO_EKU[card_nr].motor_control.interval_time;
        value.IO_EKU[card_nr].motor_control.ctrl |= 0x0001;
      }
    }
  }
}

