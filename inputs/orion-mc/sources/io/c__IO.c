// C__IO.C

#include "ch_define.h"

#include "ch_alg.h"
#include "ch_can_io.h"
#include "ch_disp_option_1.h"
#include "ch_IO_05_07_board.h"
#include "ch_IO_06_14_board.h"
#include "ch_IO_07_07_board.h"
#include "ch_IO_08_09_board.h"
#include "ch_IO_12_06_board.h"
#include "ch_IO_EKU_board.h"
#include "ch_IO_H1MC_board.h"
#include "ch_IO_H2MC_board.h"
#include "ch_IO.h"

int IO_Get_Ana_In_One(s_board_IO_on_off *IO)
{
  switch (IO->board_type)
  {
    case IO_06_14_ID: return (val_hr_alg.IO_06_14[IO->board_nr].analog_input[IO->IO_nr].value);
    case IO_05_07_ID: return (val_hr_alg.IO_05_07[IO->board_nr].analog_input[IO->IO_nr].value);
    case IO_07_07_ID: return (val_hr_alg.IO_07_07[IO->board_nr].analog_input[IO->IO_nr].value);
  }  
  return ((int)TEMP_FOUT); // foutieve waarde
}

int IO_Get_Ana_In(s_board_IO_on_off *IO, unsigned char max)
{
long val = 0;
int help;
unsigned char loop;
unsigned char aantal = 0;

  for (loop = 0; loop < max; loop++)
  {
    if (IO[loop].board_type)
    {
      if (IO[loop].on_off)
      {
        help = IO_Get_Ana_In_One(&IO[loop]);
        if (help != TEMP_FOUT)
        {
          val += help;
          aantal++;
        }
      }  
    }
  }
  if (aantal)
    return Avg(val,aantal);  
  else
    return ((int)TEMP_FOUT);
}

int IO_Get_Dig_In_Status(s_board_IO_on_off *IO)
{
  switch (IO->board_type)
  {
    case IO_06_14_ID:  return (val_hr_alg.IO_06_14[IO->board_nr].digital_input[IO->IO_nr].value); 
    case IO_12_06_ID:  return (val_hr_alg.IO_12_06[IO->board_nr].digital_input[IO->IO_nr].value);
    case IO_08_09_ID:  return (val_hr_alg.IO_08_09[IO->board_nr].digital_input[IO->IO_nr].value);
    case IO_07_07_ID:  return (val_hr_alg.IO_07_07[IO->board_nr].digital_input[IO->IO_nr].value);
    default:           return (0);
  }  
}

unsigned long IO_Get_Dig_In_Count(s_board_IO_on_off *IO)
{
unsigned long *value_ptr;
unsigned long help;

  switch (IO->board_type)
  {
    case IO_06_14_ID:  
      value_ptr = &val_hr_alg.IO_06_14[IO->board_nr].digital_input[IO->IO_nr].count; 
      Disable_Can_IO_Int(); // disable can 1 interrupt
      help = *value_ptr;
      *value_ptr -= help; // interrupt tussen deze regel en vorige kan waarde value_ptr veranderen
      Enable_Can_IO_Int(); // enable can 1 interrupt
      break;
    case IO_12_06_ID:  
      value_ptr = &val_hr_alg.IO_12_06[IO->board_nr].digital_input[IO->IO_nr].count; 
      Disable_Can_IO_Int(); // disable can 1 interrupt
      help = *value_ptr;
      *value_ptr -= help; // interrupt tussen deze regel en vorige kan waarde value_ptr veranderen
      Enable_Can_IO_Int(); // enable can 1 interrupt
      break;
    case IO_08_09_ID:  
      value_ptr = &val_hr_alg.IO_08_09[IO->board_nr].digital_input[IO->IO_nr].count; 
      Disable_Can_IO_Int(); // disable can 1 interrupt
      help = *value_ptr;
      *value_ptr -= help; // interrupt tussen deze regel en vorige kan waarde value_ptr veranderen
      Enable_Can_IO_Int(); // enable can 1 interrupt
      break;
    case IO_07_07_ID:  
      value_ptr = &val_hr_alg.IO_07_07[IO->board_nr].digital_input[IO->IO_nr].count; 
      Disable_Can_IO_Int(); // disable can 1 interrupt
      help = *value_ptr;
      *value_ptr -= help; // interrupt tussen deze regel en vorige kan waarde value_ptr veranderen
      Enable_Can_IO_Int(); // enable can 1 interrupt
      break;
    default:
      help = 0;
      break;
  }
  return (help);
}

void IO_Set_Ana_Out(s_board_IO_on_off *IO, int val)
{
  if (!install_ana_out_flag)
  {
    switch (IO->board_type)
    {
      case IO_06_14_ID:
        if (val_hr_alg.IO_06_14[IO->board_nr].analog_output.value != val)
        {
          val_hr_alg.IO_06_14[IO->board_nr].analog_output.value = val;
          value.IO_06_14[IO->board_nr].analog_output.ctrl |= 0x0001;
        }  
        break;
      case IO_12_06_ID:
        if (val_hr_alg.IO_12_06[IO->board_nr].analog_output[IO->IO_nr].value != val)
        {
          val_hr_alg.IO_12_06[IO->board_nr].analog_output[IO->IO_nr].value = val;
          value.IO_12_06[IO->board_nr].analog_output[IO->IO_nr].ctrl |= 0x0001;
        }  
        break;
      case IO_08_09_ID:
        if (val_hr_alg.IO_08_09[IO->board_nr].analog_output[IO->IO_nr].value != val)
        {
          val_hr_alg.IO_08_09[IO->board_nr].analog_output[IO->IO_nr].value = val;
          value.IO_08_09[IO->board_nr].analog_output[IO->IO_nr].ctrl |= 0x0001;
        }  
        break;
      case IO_05_07_ID:
        if (val_hr_alg.IO_05_07[IO->board_nr].analog_output[IO->IO_nr].value != val)
        {
          val_hr_alg.IO_05_07[IO->board_nr].analog_output[IO->IO_nr].value = val;
          value.IO_05_07[IO->board_nr].analog_output[IO->IO_nr].ctrl |= 0x0001;
        }  
        break;
      case IO_07_07_ID:
        if (val_hr_alg.IO_07_07[IO->board_nr].analog_output[IO->IO_nr].value != val)
        {
          val_hr_alg.IO_07_07[IO->board_nr].analog_output[IO->IO_nr].value = val;
          value.IO_07_07[IO->board_nr].analog_output[IO->IO_nr].ctrl |= 0x0001;
        }  
        break;
    }
  }
}


void IO_Get_Comp_Ana_Out(s_board_IO_on_off *IO, s_option_analog_output *ana_out)
{
  switch (IO->board_type)
  {
    case IO_06_14_ID:
      *ana_out = opt_io.IO_06_14[IO->board_nr].analog_output;
      break;
    case IO_12_06_ID:
      *ana_out = opt_io.IO_12_06[IO->board_nr].analog_output[IO->IO_nr];
      break;
    case IO_08_09_ID:
      *ana_out = opt_io.IO_08_09[IO->board_nr].analog_output[IO->IO_nr];
      break;
    case IO_05_07_ID:
      *ana_out = opt_io.IO_05_07[IO->board_nr].analog_output[IO->IO_nr];
      break;
    case IO_07_07_ID:
      *ana_out = opt_io.IO_07_07[IO->board_nr].analog_output[IO->IO_nr];
      break;
  }  
}

void IO_Set_Comp_Ana_Out(s_board_IO_on_off *IO, s_option_analog_output *ana_out)
{
  switch (IO->board_type)
  {
    case IO_06_14_ID:
      opt_io.IO_06_14[IO->board_nr].analog_output = *ana_out;
      value.IO_06_14[IO->board_nr].analog_output.ctrl |= 0x0004;
      break;
    case IO_12_06_ID:
      opt_io.IO_12_06[IO->board_nr].analog_output[IO->IO_nr] = *ana_out;
      value.IO_12_06[IO->board_nr].analog_output[IO->IO_nr].ctrl |= 0x0004;
      break;
    case IO_08_09_ID:
      opt_io.IO_08_09[IO->board_nr].analog_output[IO->IO_nr] = *ana_out;
      value.IO_08_09[IO->board_nr].analog_output[IO->IO_nr].ctrl |= 0x0004;
      break;
    case IO_05_07_ID:
      opt_io.IO_05_07[IO->board_nr].analog_output[IO->IO_nr] = *ana_out;
      value.IO_05_07[IO->board_nr].analog_output[IO->IO_nr].ctrl |= 0x0004;
      break;
    case IO_07_07_ID:
      opt_io.IO_07_07[IO->board_nr].analog_output[IO->IO_nr] = *ana_out;
      value.IO_07_07[IO->board_nr].analog_output[IO->IO_nr].ctrl |= 0x0004;
      break;
  }
}

void IO_Set_Dig_Out(s_board_IO_on_off *IO, unsigned char on_off)
{
unsigned char help;

  if (!install_dig_out_flag)
  {
    switch (IO->board_type)
    {
      case IO_06_14_ID:
        help = ((val_hr_alg.IO_06_14[IO->board_nr].digital_output.value >> IO->IO_nr) & 0x0001);
        if (help != on_off)
        {
          if (on_off) // aanzetten relais
            val_hr_alg.IO_06_14[IO->board_nr].digital_output.value |= (0x0001 << IO->IO_nr);
          else // uitzetten relais
            val_hr_alg.IO_06_14[IO->board_nr].digital_output.value &= ~(0x0001 << IO->IO_nr);
          value.IO_06_14[IO->board_nr].digital_output.ctrl |= 0x0001;
        }  
        break;
      case IO_12_06_ID:
        help = ((val_hr_alg.IO_12_06[IO->board_nr].digital_output.value >> IO->IO_nr) & 0x0001);
        if (help != on_off)
        {
          if (on_off) // aanzetten relais
            val_hr_alg.IO_12_06[IO->board_nr].digital_output.value |= (0x0001 << IO->IO_nr);
          else // uitzetten relais
            val_hr_alg.IO_12_06[IO->board_nr].digital_output.value &= ~(0x0001 << IO->IO_nr);
          value.IO_12_06[IO->board_nr].digital_output.ctrl |= 0x0001;
        }  
        break;
      case IO_08_09_ID:
        help = ((val_hr_alg.IO_08_09[IO->board_nr].digital_output.value >> IO->IO_nr) & 0x0001);
        if (help != on_off)
        {
          if (on_off) // aanzetten relais
            val_hr_alg.IO_08_09[IO->board_nr].digital_output.value |= (0x0001 << IO->IO_nr);
          else // uitzetten relais
            val_hr_alg.IO_08_09[IO->board_nr].digital_output.value &= ~(0x0001 << IO->IO_nr);
          value.IO_08_09[IO->board_nr].digital_output.ctrl |= 0x0001;
        }  
        break;
      case IO_05_07_ID:
        help = ((val_hr_alg.IO_05_07[IO->board_nr].digital_output.value >> IO->IO_nr) & 0x0001);
        if (help != on_off)
        {
          if (on_off) // aanzetten relais
            val_hr_alg.IO_05_07[IO->board_nr].digital_output.value |= (0x0001 << IO->IO_nr);
          else // uitzetten relais
            val_hr_alg.IO_05_07[IO->board_nr].digital_output.value &= ~(0x0001 << IO->IO_nr);
          value.IO_05_07[IO->board_nr].digital_output.ctrl |= 0x0001;
        }  
        break;
      case IO_07_07_ID:
        help = ((val_hr_alg.IO_07_07[IO->board_nr].digital_output.value >> IO->IO_nr) & 0x0001);
        if (help != on_off)
        {
          if (on_off) // aanzetten relais
            val_hr_alg.IO_07_07[IO->board_nr].digital_output.value |= (0x0001 << IO->IO_nr);
          else // uitzetten relais
            val_hr_alg.IO_07_07[IO->board_nr].digital_output.value &= ~(0x0001 << IO->IO_nr);
          value.IO_07_07[IO->board_nr].digital_output.ctrl |= 0x0001;
        }  
        break;
    }
  }
}

unsigned char IO_Get_Dig_Out(s_board_IO_on_off *IO)
{
unsigned char help;

  switch (IO->board_type)
  {
    case IO_06_14_ID:  help = (val_hr_alg.IO_06_14[IO->board_nr].digital_output.value & (0x0001 << IO->IO_nr)) ? 1 : 0; break;
    case IO_12_06_ID:  help = (val_hr_alg.IO_12_06[IO->board_nr].digital_output.value & (0x0001 << IO->IO_nr)) ? 1 : 0; break;
    case IO_08_09_ID:  help = (val_hr_alg.IO_08_09[IO->board_nr].digital_output.value & (0x0001 << IO->IO_nr)) ? 1 : 0; break;
    case IO_05_07_ID:  help = (val_hr_alg.IO_05_07[IO->board_nr].digital_output.value & (0x0001 << IO->IO_nr)) ? 1 : 0; break;
    case IO_07_07_ID:  help = (val_hr_alg.IO_07_07[IO->board_nr].digital_output.value & (0x0001 << IO->IO_nr)) ? 1 : 0; break;
    default: help = 0; break;
  }
  return (help);
}

//================================================================================================
void IO_Set_Motor_Control_Option(s_board_IO_on_off *IO, unsigned char Option)
{
  switch (IO->board_type)
  {
    case IO_EKU_ID:
      if ((opt_io.IO_EKU[IO->board_nr].motor_control.Option & 0x00FF) != Option)
      {
        opt_io.IO_EKU[IO->board_nr].motor_control.Option &= 0xFF00;
        opt_io.IO_EKU[IO->board_nr].motor_control.Option |= Option;
        value.IO_EKU[IO->board_nr].motor_control.ctrl |= 0x0004;
      }
      break;
    case IO_H2MC_ID:
      if ((opt_io.IO_H2MC[IO->board_nr].motor_control[IO->IO_nr].Option & 0x00FF) != Option)
      {
        opt_io.IO_H2MC[IO->board_nr].motor_control[IO->IO_nr].Option &= 0xFF00;
        opt_io.IO_H2MC[IO->board_nr].motor_control[IO->IO_nr].Option |= Option;
        value.IO_H2MC[IO->board_nr].motor_control[IO->IO_nr].ctrl |= 0x0004;
      }
      break;
    case IO_H1MC_ID:
      if ((opt_io.IO_H1MC[IO->board_nr].motor_control.Option & 0x00FF) != Option)
      {
        opt_io.IO_H1MC[IO->board_nr].motor_control.Option &= 0xFF00;
        opt_io.IO_H1MC[IO->board_nr].motor_control.Option |= Option;
        value.IO_H1MC[IO->board_nr].motor_control.ctrl |= 0x0004;
      }
      break;
  }
}

int IO_Get_Motor_Control_Option(s_board_IO_on_off *IO)
{
  switch (IO->board_type)
  {
    case IO_EKU_ID:  return (opt_io.IO_EKU[IO->board_nr].motor_control.Option); // & 0x000F);
    case IO_H2MC_ID: return (opt_io.IO_H2MC[IO->board_nr].motor_control[IO->IO_nr].Option); // & 0x000F);
    case IO_H1MC_ID: return (opt_io.IO_H1MC[IO->board_nr].motor_control.Option); // & 0x000F);
    default:         return (0);
  }
}

int IO_Get_Motor_Control_Position(s_board_IO_on_off *IO)
{
  switch (IO->board_type)
  {
    case IO_EKU_ID:  return (val_hr_alg.IO_EKU[IO->board_nr].motor_control.Value);
    case IO_H2MC_ID: return (val_hr_alg.IO_H2MC[IO->board_nr].motor_control[IO->IO_nr].Value);
    case IO_H1MC_ID: return (val_hr_alg.IO_H1MC[IO->board_nr].motor_control.Value);
    default:         return (0);
  }  
}

unsigned char IO_Set_Motor_Control_Position(s_board_IO_on_off *IO, int val)
{
  switch (IO->board_type)
  {
    case IO_EKU_ID:
      if (val_hr_alg.IO_EKU[IO->board_nr].motor_control.Position != val)
      {
        val_hr_alg.IO_EKU[IO->board_nr].motor_control.Position = val;
        value.IO_EKU[IO->board_nr].motor_control.ctrl |= 0x0001;
        return (1);
      }  
      break;
    case IO_H2MC_ID:
      if (val_hr_alg.IO_H2MC[IO->board_nr].motor_control[IO->IO_nr].Position != val)
      {
        val_hr_alg.IO_H2MC[IO->board_nr].motor_control[IO->IO_nr].Position = val;
        value.IO_H2MC[IO->board_nr].motor_control[IO->IO_nr].ctrl |= 0x0001;
        return (1);
      }  
      break;
    case IO_H1MC_ID:
      if (val_hr_alg.IO_H1MC[IO->board_nr].motor_control.Position != val)
      {
        val_hr_alg.IO_H1MC[IO->board_nr].motor_control.Position = val;
        value.IO_H1MC[IO->board_nr].motor_control.ctrl |= 0x0001;
        return (1);
      }  
      break;
  }
  return (0);
}

void IO_Set_Motor_Control_Hold(s_board_IO_on_off *IO, TRunningMode RunningMode)
{
unsigned int Mask = 0;

  switch (RunningMode)
  {
    case rmOpen:
      Mask = 0x0008;
      break;
    case rmClose:
      Mask = 0x0010;
      break;
  }
  switch (IO->board_type)
  {
    case IO_EKU_ID:
      if ((val_hr_alg.IO_EKU[IO->board_nr].motor_control.Ctrl & Mask) != Mask)
      {
        val_hr_alg.IO_EKU[IO->board_nr].motor_control.Ctrl |= Mask;
        value.IO_EKU[IO->board_nr].motor_control.ctrl |= 0x0001;
      }  
      break;
    case IO_H2MC_ID:
      if ((val_hr_alg.IO_H2MC[IO->board_nr].motor_control[IO->IO_nr].Ctrl & Mask) != Mask)
      {
        val_hr_alg.IO_H2MC[IO->board_nr].motor_control[IO->IO_nr].Ctrl |= Mask;
        value.IO_H2MC[IO->board_nr].motor_control[IO->IO_nr].ctrl |= 0x0001;
      }  
      break;
    case IO_H1MC_ID:
      if ((val_hr_alg.IO_H1MC[IO->board_nr].motor_control.Ctrl & Mask) != Mask)
      {
        val_hr_alg.IO_H1MC[IO->board_nr].motor_control.Ctrl |= Mask;
        value.IO_H1MC[IO->board_nr].motor_control.ctrl |= 0x0001;
      }  
      break;
  }
}

void IO_Set_Motor_Control_Release(s_board_IO_on_off *IO, TRunningMode RunningMode)
{
unsigned int Mask = 0;

  switch (RunningMode)
  {
    case rmOpen:
      Mask = 0x0008;
      break;
    case rmClose:
      Mask = 0x0010;
      break;
  }
  switch (IO->board_type)
  {
    case IO_EKU_ID:
      if ((val_hr_alg.IO_EKU[IO->board_nr].motor_control.Ctrl & Mask) != 0x0000)
      {
        val_hr_alg.IO_EKU[IO->board_nr].motor_control.Ctrl &= ~Mask;
        value.IO_EKU[IO->board_nr].motor_control.ctrl |= 0x0001;
      }  
      break;
    case IO_H2MC_ID:
      if ((val_hr_alg.IO_H2MC[IO->board_nr].motor_control[IO->IO_nr].Ctrl & Mask) != 0x0000)
      {
        val_hr_alg.IO_H2MC[IO->board_nr].motor_control[IO->IO_nr].Ctrl &= ~Mask;
        value.IO_H2MC[IO->board_nr].motor_control[IO->IO_nr].ctrl |= 0x0001;
      }  
      break;
    case IO_H1MC_ID:
      if ((val_hr_alg.IO_H1MC[IO->board_nr].motor_control.Ctrl & Mask) != 0x0000)
      {
        val_hr_alg.IO_H1MC[IO->board_nr].motor_control.Ctrl &= ~Mask;
        value.IO_H1MC[IO->board_nr].motor_control.ctrl |= 0x0001;
      }  
      break;
  }
}

void IO_Set_Motor_Control_HiSpeed(s_board_IO_on_off *IO, unsigned char val)
{
unsigned int Mask = 0;

  if (val)
    Mask = 0x0020;
  else
    Mask = 0x0000;
  switch (IO->board_type)
  {
    case IO_EKU_ID:
      if ((val_hr_alg.IO_EKU[IO->board_nr].motor_control.Ctrl & 0x0020) != Mask)
      {
        val_hr_alg.IO_EKU[IO->board_nr].motor_control.Ctrl &= 0xFFDF;
        val_hr_alg.IO_EKU[IO->board_nr].motor_control.Ctrl |= Mask;
        value.IO_EKU[IO->board_nr].motor_control.ctrl |= 0x0001;
      }  
      break;
    case IO_H2MC_ID:
      if ((val_hr_alg.IO_H2MC[IO->board_nr].motor_control[IO->IO_nr].Ctrl & 0x0020) != Mask)
      {
        val_hr_alg.IO_H2MC[IO->board_nr].motor_control[IO->IO_nr].Ctrl &= 0xFFDF;
        val_hr_alg.IO_H2MC[IO->board_nr].motor_control[IO->IO_nr].Ctrl |= Mask;
        value.IO_H2MC[IO->board_nr].motor_control[IO->IO_nr].ctrl |= 0x0001;
      }  
      break;
    case IO_H1MC_ID:
      if ((val_hr_alg.IO_H1MC[IO->board_nr].motor_control.Ctrl & 0x0020) != Mask)
      {
        val_hr_alg.IO_H1MC[IO->board_nr].motor_control.Ctrl &= 0xFFDF;
        val_hr_alg.IO_H1MC[IO->board_nr].motor_control.Ctrl |= Mask;
        value.IO_H1MC[IO->board_nr].motor_control.ctrl |= 0x0001;
      }  
      break;
  }
}

int IO_Get_Motor_Control_HiSpeed(s_board_IO_on_off *IO)
{
  switch (IO->board_type)
  {
    case IO_EKU_ID:
      if (val_hr_alg.IO_EKU[IO->board_nr].motor_control.Status2 & 0x0010)
        return (1);
      else
        return (0);
    case IO_H2MC_ID:
      if (val_hr_alg.IO_H2MC[IO->board_nr].motor_control[IO->IO_nr].Status2 & 0x0010)
        return (1);
      else
        return (0);
    case IO_H1MC_ID:
      if (val_hr_alg.IO_H1MC[IO->board_nr].motor_control.Status2 & 0x0010)
        return (1);
      else
        return (0);
  }
  return (0);
}

int IO_Get_Motor_Control_Runtime(s_board_IO_on_off *IO)
{
  switch (IO->board_type)
  {
    case IO_EKU_ID:  return (opt_io.IO_EKU[IO->board_nr].motor_control.Runtime);
    case IO_H2MC_ID: return (opt_io.IO_H2MC[IO->board_nr].motor_control[IO->IO_nr].Runtime);
    case IO_H1MC_ID: return (opt_io.IO_H1MC[IO->board_nr].motor_control.Runtime);
    default:         return (0);
  }  
}

int IO_Get_Motor_Control_State(s_board_IO_on_off *IO)
{
  switch (IO->board_type)
  {
    case IO_EKU_ID:  return (val_hr_alg.IO_EKU[IO->board_nr].motor_control.Status);
    case IO_H2MC_ID: return (val_hr_alg.IO_H2MC[IO->board_nr].motor_control[IO->IO_nr].Status);
    case IO_H1MC_ID: return (val_hr_alg.IO_H1MC[IO->board_nr].motor_control.Status);
    default:         return (0);
  }
}

int IO_Get_Board_Alarm(s_board_IO_on_off *IO)
{
  switch (IO->board_type)
  {
    case IO_05_07_ID: return (alarm_hr_alg.IO_05_07[IO->board_nr].board_al);
    case IO_06_14_ID: return (alarm_hr_alg.IO_06_14[IO->board_nr].board_al);
    case IO_08_09_ID: return (alarm_hr_alg.IO_08_09[IO->board_nr].board_al);
    case IO_12_06_ID: return (alarm_hr_alg.IO_12_06[IO->board_nr].board_al);
    case IO_H1MC_ID : return (alarm_hr_alg.IO_H1MC[IO->board_nr].board_al);
    case IO_H2MC_ID : return (alarm_hr_alg.IO_H2MC[IO->board_nr].board_al);
    case IO_EKU_ID  : return (alarm_hr_alg.IO_EKU[IO->board_nr].board_al);
    default         : return (0);
  }
}

int IO_Get_Motor_Alarm_Code(s_board_IO_on_off *IO)
{
  switch (IO->board_type)
  {
    case IO_EKU_ID : return (val_hr_alg.IO_EKU[IO->board_nr].motor_control.Status2 >> 10);
    case IO_H2MC_ID: return (val_hr_alg.IO_H2MC[IO->board_nr].motor_control[IO->IO_nr].Status2 >> 10);
    case IO_H1MC_ID: return (val_hr_alg.IO_H1MC[IO->board_nr].motor_control.Status2 >> 10);
    default:         return (0);
  }
}

int IO_Get_Motor_Alarm_Slave(s_board_IO_on_off *IO)
{
  switch (IO->board_type)
  {
//    case IO_EKU_ID:  return ((val_hr_alg.IO_EKU[IO->board_nr].motor_control.Status2 >> 5) & 0x07 ); // No slaves
//    case IO_H2MC_ID: return ((val_hr_alg.IO_H2MC[IO->board_nr].motor_control[IO->IO_nr].Status2 >> 5) & 0x07); // No slaves
    case IO_H1MC_ID: return ((val_hr_alg.IO_H1MC[IO->board_nr].motor_control.Status2 >> 5) & 0x07);
    default:         return (0);
  }
}

void IO_Get_Motor_Control_MotorManagement(s_board_IO_on_off *IO, TMotorManagement *MotorManagement)
{
  switch (IO->board_type)
  {
    case IO_EKU_ID:
      MotorManagement->Runtime  = val_hr_alg.IO_EKU[IO->board_nr].motor_control.MotorManagementRuntime;
      MotorManagement->Switches = val_hr_alg.IO_EKU[IO->board_nr].motor_control.MotorManagementSwitches;
      MotorManagement->Failures = val_hr_alg.IO_EKU[IO->board_nr].motor_control.MotorManagementFailures;
      value.IO_EKU[IO->board_nr].motor_control.ctrl |= 0x0008;
      break;
    case IO_H2MC_ID:
      MotorManagement->Runtime  = val_hr_alg.IO_H2MC[IO->board_nr].motor_control[IO->IO_nr].MotorManagementRuntime;
      MotorManagement->Switches = val_hr_alg.IO_H2MC[IO->board_nr].motor_control[IO->IO_nr].MotorManagementSwitches;
      MotorManagement->Failures = val_hr_alg.IO_H2MC[IO->board_nr].motor_control[IO->IO_nr].MotorManagementFailures;
      value.IO_H2MC[IO->board_nr].motor_control[IO->IO_nr].ctrl |= 0x0008;
      break;
    case IO_H1MC_ID:
      MotorManagement->Runtime  = val_hr_alg.IO_H1MC[IO->board_nr].motor_control.MotorManagementRuntime;
      MotorManagement->Switches = val_hr_alg.IO_H1MC[IO->board_nr].motor_control.MotorManagementSwitches;
      MotorManagement->Failures = val_hr_alg.IO_H1MC[IO->board_nr].motor_control.MotorManagementFailures;
      value.IO_H1MC[IO->board_nr].motor_control.ctrl |= 0x0008;
      break;
    default:
      MotorManagement->Runtime  = 0;
      MotorManagement->Switches = 0;
      MotorManagement->Failures = 0;
      break;
  }
}

void IO_Update_Motor_Control_MotorManagement(s_board_IO_on_off *IO, TMotorManagement *MotorManagement)
{
  switch (IO->board_type)
  {
    case IO_EKU_ID:
      MotorManagement->Runtime  = val_hr_alg.IO_EKU[IO->board_nr].motor_control.MotorManagementRuntime;
      MotorManagement->Switches = val_hr_alg.IO_EKU[IO->board_nr].motor_control.MotorManagementSwitches;
      MotorManagement->Failures = val_hr_alg.IO_EKU[IO->board_nr].motor_control.MotorManagementFailures;
      break;
    case IO_H2MC_ID:
      MotorManagement->Runtime  = val_hr_alg.IO_H2MC[IO->board_nr].motor_control[IO->IO_nr].MotorManagementRuntime;
      MotorManagement->Switches = val_hr_alg.IO_H2MC[IO->board_nr].motor_control[IO->IO_nr].MotorManagementSwitches;
      MotorManagement->Failures = val_hr_alg.IO_H2MC[IO->board_nr].motor_control[IO->IO_nr].MotorManagementFailures;
      break;
    case IO_H1MC_ID:
      MotorManagement->Runtime  = val_hr_alg.IO_H1MC[IO->board_nr].motor_control.MotorManagementRuntime;
      MotorManagement->Switches = val_hr_alg.IO_H1MC[IO->board_nr].motor_control.MotorManagementSwitches;
      MotorManagement->Failures = val_hr_alg.IO_H1MC[IO->board_nr].motor_control.MotorManagementFailures;
      break;
    default:
      MotorManagement->Runtime  = 0;
      MotorManagement->Switches = 0;
      MotorManagement->Failures = 0;
      break;
  }
}

void IO_Set_Motor_Control_Frequency(s_board_IO_on_off *IO, unsigned char SpeedLow, unsigned char SpeedHi, int PositionLowSpeedClose, int PositionLowSpeedOpen)
{
unsigned char TransmitFlag = 0;

  switch (IO->board_type)
  {
    case IO_EKU_ID:
      if (opt_io.IO_EKU[IO->board_nr].motor_control.SpeedLow != SpeedLow)
      {
        opt_io.IO_EKU[IO->board_nr].motor_control.SpeedLow = SpeedLow;
        TransmitFlag = 1;
      }
      if (opt_io.IO_EKU[IO->board_nr].motor_control.SpeedHi != SpeedHi)
      {
        opt_io.IO_EKU[IO->board_nr].motor_control.SpeedHi = SpeedHi;
        TransmitFlag = 1;
      }
      if (opt_io.IO_EKU[IO->board_nr].motor_control.PosLowSpeedClose != PositionLowSpeedClose)
      {
        opt_io.IO_EKU[IO->board_nr].motor_control.PosLowSpeedClose = PositionLowSpeedClose;
        TransmitFlag = 1;
      }
      if (opt_io.IO_EKU[IO->board_nr].motor_control.PosLowSpeedOpen != PositionLowSpeedOpen)
      {
        opt_io.IO_EKU[IO->board_nr].motor_control.PosLowSpeedOpen = PositionLowSpeedOpen;
        TransmitFlag = 1;
      }
      if (TransmitFlag)
        value.IO_EKU[IO->board_nr].motor_control.ctrl |= 0x0004;
      break;
    case IO_H2MC_ID:
      if (opt_io.IO_H2MC[IO->board_nr].motor_control[IO->IO_nr].SpeedLow != SpeedLow)
      {
        opt_io.IO_H2MC[IO->board_nr].motor_control[IO->IO_nr].SpeedLow = SpeedLow;
        TransmitFlag = 1;
      }
      if (opt_io.IO_H2MC[IO->board_nr].motor_control[IO->IO_nr].SpeedHi != SpeedHi)
      {
        opt_io.IO_H2MC[IO->board_nr].motor_control[IO->IO_nr].SpeedHi = SpeedHi;
        TransmitFlag = 1;
      }
      if (opt_io.IO_H2MC[IO->board_nr].motor_control[IO->IO_nr].PosLowSpeedClose != PositionLowSpeedClose)
      {
        opt_io.IO_H2MC[IO->board_nr].motor_control[IO->IO_nr].PosLowSpeedClose = PositionLowSpeedClose;
        TransmitFlag = 1;
      }
      if (opt_io.IO_H2MC[IO->board_nr].motor_control[IO->IO_nr].PosLowSpeedOpen != PositionLowSpeedOpen)
      {
        opt_io.IO_H2MC[IO->board_nr].motor_control[IO->IO_nr].PosLowSpeedOpen = PositionLowSpeedOpen;
        TransmitFlag = 1;
      }
      if (TransmitFlag)
        value.IO_H2MC[IO->board_nr].motor_control[IO->IO_nr].ctrl |= 0x0004;
      break;
    case IO_H1MC_ID:
      if (opt_io.IO_H1MC[IO->board_nr].motor_control.SpeedLow != SpeedLow)
      {
        opt_io.IO_H1MC[IO->board_nr].motor_control.SpeedLow = SpeedLow;
        TransmitFlag = 1;
      }
      if (opt_io.IO_H1MC[IO->board_nr].motor_control.SpeedHi != SpeedHi)
      {
        opt_io.IO_H1MC[IO->board_nr].motor_control.SpeedHi = SpeedHi;
        TransmitFlag = 1;
      }
      if (opt_io.IO_H1MC[IO->board_nr].motor_control.PosLowSpeedClose != PositionLowSpeedClose)
      {
        opt_io.IO_H1MC[IO->board_nr].motor_control.PosLowSpeedClose = PositionLowSpeedClose;
        TransmitFlag = 1;
      }
      if (opt_io.IO_H1MC[IO->board_nr].motor_control.PosLowSpeedOpen != PositionLowSpeedOpen)
      {
        opt_io.IO_H1MC[IO->board_nr].motor_control.PosLowSpeedOpen = PositionLowSpeedOpen;
        TransmitFlag = 1;
      }
      if (TransmitFlag)
        value.IO_H1MC[IO->board_nr].motor_control.ctrl |= 0x0004;
      break;
  }
}

//================================================================================================
static void IO_RS485bus_setMode(s_option_RS485_bus *opt, s_value_RS485_bus *val, TRS485Mode RS485Mode)
{
  int intervalTime = (RS485Mode == RS485ModeModbus) ? 100 : 0;
  int inhibitTime = 0;

  if (((opt->Option & 0x000F) != RS485Mode) || (opt->IntervalTime != intervalTime) || (opt->InhibitTime != inhibitTime))
  {
    opt->Option &= 0xFFF0;
    opt->Option |= RS485Mode;
    opt->IntervalTime = intervalTime;
    opt->InhibitTime = inhibitTime;
    val->ctrl |= 0x0004;
  }
}

void IO_Set_RS485_Mode(s_board_IO_on_off *IO, TRS485Mode RS485Mode)
{
  switch (IO->board_type)
  {
    case IO_05_07_ID: IO_RS485bus_setMode(&opt_io.IO_05_07[IO->board_nr].RS485_bus, &value.IO_05_07[IO->board_nr].RS485_bus, RS485Mode); return;
    case IO_07_07_ID: IO_RS485bus_setMode(&opt_io.IO_07_07[IO->board_nr].RS485_bus, &value.IO_07_07[IO->board_nr].RS485_bus, RS485Mode); return;
  }
}

void IO_RS485_Stop_All_Periodic_Messages(void)
{
int i;

  for (i = 0; i < IO_05_07_MAX; i++)
  {
    if ((opt_io.IO_05_07[i].RS485_bus.Option & 0x000F) == RS485ModeModbus)
      value.IO_05_07[i].RS485_bus.ctrl |= 0x0004; // Send SDO -> This will stop all the periodic messages.
  }
  for (i = 0; i < IO_07_07_MAX; i++)
  {
    if ((opt_io.IO_07_07[i].RS485_bus.Option & 0x000F) == RS485ModeModbus)
      value.IO_07_07[i].RS485_bus.ctrl |= 0x0004; // Send SDO -> This will stop all the periodic messages.
  }
}

unsigned char IO_Set_RS485_Message(s_board_IO_on_off *IO, unsigned char *Data, int DataSize)
{
int i;

  switch (IO->board_type)
  {
    case IO_05_07_ID:
      for (i = 0; i < DataSize; i++)
      {
        if (value.IO_05_07[IO->board_nr].RS485_bus.TxInBuffer < RS485_BUFFER_SIZE)
        {
          Disable_Can_IO_Int(); // disable can 1 interrupt
          value.IO_05_07[IO->board_nr].RS485_bus.TxBuffer[value.IO_05_07[IO->board_nr].RS485_bus.TxPutIndex] = Data[i];
          value.IO_05_07[IO->board_nr].RS485_bus.TxInBuffer++;
          value.IO_05_07[IO->board_nr].RS485_bus.TxPutIndex++;
          if (value.IO_05_07[IO->board_nr].RS485_bus.TxPutIndex >= RS485_BUFFER_SIZE)
            value.IO_05_07[IO->board_nr].RS485_bus.TxPutIndex = 0;
          value.IO_05_07[IO->board_nr].RS485_bus.ctrl |= 0x0001;
          Enable_Can_IO_Int(); // enable can 1 interrupt
        }
        else
        {
          return (1); // Buffer overflow
        }
      }
      break;
    case IO_07_07_ID:
      for (i = 0; i < DataSize; i++)
      {
        if (value.IO_07_07[IO->board_nr].RS485_bus.TxInBuffer < RS485_BUFFER_SIZE)
        {
          Disable_Can_IO_Int(); // disable can 1 interrupt
          value.IO_07_07[IO->board_nr].RS485_bus.TxBuffer[value.IO_07_07[IO->board_nr].RS485_bus.TxPutIndex] = Data[i];
          value.IO_07_07[IO->board_nr].RS485_bus.TxInBuffer++;
          value.IO_07_07[IO->board_nr].RS485_bus.TxPutIndex++;
          if (value.IO_07_07[IO->board_nr].RS485_bus.TxPutIndex >= RS485_BUFFER_SIZE)
            value.IO_07_07[IO->board_nr].RS485_bus.TxPutIndex = 0;
          value.IO_07_07[IO->board_nr].RS485_bus.ctrl |= 0x0001;
          Enable_Can_IO_Int(); // enable can 1 interrupt
        }
        else
        {
          return (1); // Buffer overflow
        }
      }
      break;
  }
  return (0);
}

void IO_Reset_RS485_RxBuffer(s_board_IO_on_off *IO)
{
  switch (IO->board_type)
  {
    case IO_05_07_ID:
      Disable_Can_IO_Int(); // disable can 1 interrupt
      value.IO_05_07[IO->board_nr].RS485_bus.RxGetIndex   = 0;
      value.IO_05_07[IO->board_nr].RS485_bus.RxPutIndex   = 0;
      value.IO_05_07[IO->board_nr].RS485_bus.RxInBuffer   = 0;
	  value.IO_05_07[IO->board_nr].RS485_bus.MessageReady = 0;
      Enable_Can_IO_Int(); // enable can 1 interrupt
      break;
    case IO_07_07_ID:
      Disable_Can_IO_Int(); // disable can 1 interrupt
      value.IO_07_07[IO->board_nr].RS485_bus.RxGetIndex   = 0;
      value.IO_07_07[IO->board_nr].RS485_bus.RxPutIndex   = 0;
      value.IO_07_07[IO->board_nr].RS485_bus.RxInBuffer   = 0;
	  value.IO_07_07[IO->board_nr].RS485_bus.MessageReady = 0;
      Enable_Can_IO_Int(); // enable can 1 interrupt
      break;
  }
}

void IO_Reset_RS485_TxBuffer(s_board_IO_on_off *IO) // JP 03-12-15
{
  switch (IO->board_type)
  {
    case IO_05_07_ID:
      Disable_Can_IO_Int(); // disable can 1 interrupt
      value.IO_05_07[IO->board_nr].RS485_bus.TxPutIndex = 0;
      value.IO_05_07[IO->board_nr].RS485_bus.TxGetIndex = 0;
      value.IO_05_07[IO->board_nr].RS485_bus.TxInBuffer = 0;
	  value.IO_05_07[IO->board_nr].RS485_bus.Transmit   = 0;
      Enable_Can_IO_Int(); // enable can 1 interrupt
      break;
    case IO_07_07_ID:
      Disable_Can_IO_Int(); // disable can 1 interrupt
      value.IO_07_07[IO->board_nr].RS485_bus.TxPutIndex = 0;
      value.IO_07_07[IO->board_nr].RS485_bus.TxGetIndex = 0;
      value.IO_07_07[IO->board_nr].RS485_bus.TxInBuffer = 0;
	  value.IO_07_07[IO->board_nr].RS485_bus.Transmit   = 0;
      Enable_Can_IO_Int(); // enable can 1 interrupt
      break;
  }
}

unsigned char IO_RS485_RxBuffer_Size(s_board_IO_on_off *IO)
{
  switch (IO->board_type)
  {
    case IO_05_07_ID: return (value.IO_05_07[IO->board_nr].RS485_bus.RxInBuffer);
    case IO_07_07_ID: return (value.IO_07_07[IO->board_nr].RS485_bus.RxInBuffer);
  }
  return (0);
}

unsigned char IO_RS485_MessageReady(s_board_IO_on_off *IO)
{
  switch (IO->board_type)
  {
    case IO_05_07_ID: return (value.IO_05_07[IO->board_nr].RS485_bus.MessageReady);
    case IO_07_07_ID: return (value.IO_07_07[IO->board_nr].RS485_bus.MessageReady);
  }
  return (0);
}

unsigned char IO_Get_RS485_Char(s_board_IO_on_off *IO)
{
unsigned char Data = 0;

  switch (IO->board_type)
  {
    case IO_05_07_ID:
      if (value.IO_05_07[IO->board_nr].RS485_bus.RxInBuffer > 0)
      {
        Disable_Can_IO_Int(); // disable can 1 interrupt
        Data = value.IO_05_07[IO->board_nr].RS485_bus.RxBuffer[value.IO_05_07[IO->board_nr].RS485_bus.RxGetIndex];
        value.IO_05_07[IO->board_nr].RS485_bus.RxInBuffer--;
        value.IO_05_07[IO->board_nr].RS485_bus.RxGetIndex++;
        if (value.IO_05_07[IO->board_nr].RS485_bus.RxGetIndex >= RS485_BUFFER_SIZE)
          value.IO_05_07[IO->board_nr].RS485_bus.RxGetIndex = 0;
		if (value.IO_05_07[IO->board_nr].RS485_bus.RxInBuffer == 0)
		  value.IO_05_07[IO->board_nr].RS485_bus.MessageReady = 0;
        Enable_Can_IO_Int(); // enable can 1 interrupt
      }
      break;
    case IO_07_07_ID:
      if (value.IO_07_07[IO->board_nr].RS485_bus.RxInBuffer > 0)
      {
        Disable_Can_IO_Int(); // disable can 1 interrupt
        Data = value.IO_07_07[IO->board_nr].RS485_bus.RxBuffer[value.IO_07_07[IO->board_nr].RS485_bus.RxGetIndex];
        value.IO_07_07[IO->board_nr].RS485_bus.RxInBuffer--;
        value.IO_07_07[IO->board_nr].RS485_bus.RxGetIndex++;
        if (value.IO_07_07[IO->board_nr].RS485_bus.RxGetIndex >= RS485_BUFFER_SIZE)
          value.IO_07_07[IO->board_nr].RS485_bus.RxGetIndex = 0;
		if (value.IO_07_07[IO->board_nr].RS485_bus.RxInBuffer == 0)
		  value.IO_07_07[IO->board_nr].RS485_bus.MessageReady = 0;
        Enable_Can_IO_Int(); // enable can 1 interrupt
      }
      break;
  }
  return (Data);
}

//================================================================================================
static unsigned char CreateBoardComponent(s_board_IO_on_off *IO)
{
  switch (IO->board_type)
  {
    case IO_05_07_ID:
	  if (opt_io.IO_05_07[IO->board_nr].board_component.option == 0)
	  {
        opt_io.IO_05_07[IO->board_nr].board_component.option = 1;
        IO_05_07_init_switch[IO->board_nr] = 1;
	  }
      break;
    case IO_06_14_ID:
	  if (opt_io.IO_06_14[IO->board_nr].board_component.option == 0)
	  {
        opt_io.IO_06_14[IO->board_nr].board_component.option = 1;
        IO_06_14_init_switch[IO->board_nr] = 1;
	  }
      break;
    case IO_07_07_ID:
	  if (opt_io.IO_07_07[IO->board_nr].board_component.option == 0)
	  {
        opt_io.IO_07_07[IO->board_nr].board_component.option = 1;
        IO_07_07_init_switch[IO->board_nr] = 1;
	  }
      break;
    case IO_08_09_ID:
	  if (opt_io.IO_08_09[IO->board_nr].board_component.option == 0)
	  {
        opt_io.IO_08_09[IO->board_nr].board_component.option = 1;
        IO_08_09_init_switch[IO->board_nr] = 1;
	  }
      break;
    case IO_12_06_ID:
	  if (opt_io.IO_12_06[IO->board_nr].board_component.option == 0)
	  {
        opt_io.IO_12_06[IO->board_nr].board_component.option = 1;
        IO_12_06_init_switch[IO->board_nr] = 1;
	  }
      break;
    case IO_EKU_ID:
	  if (opt_io.IO_EKU[IO->board_nr].board_component.option == 0)
	  {
        opt_io.IO_EKU[IO->board_nr].board_component.option = 1;
        IO_EKU_init_switch[IO->board_nr] = 1;
	  }
      break;
    case IO_H1MC_ID:
	  if (opt_io.IO_H1MC[IO->board_nr].board_component.option == 0)
	  {
        opt_io.IO_H1MC[IO->board_nr].board_component.option = 1;
        IO_H1MC_init_switch[IO->board_nr] = 1;
	  }
      break;
    case IO_H2MC_ID:
	  if (opt_io.IO_H2MC[IO->board_nr].board_component.option == 0)
	  {
        opt_io.IO_H2MC[IO->board_nr].board_component.option = 1;
        IO_H2MC_init_switch[IO->board_nr] = 1;
	  }
      break;
    default:
	  return (1);
  }
  return (0);
}

static unsigned char CreateDigitalInput(s_board_IO_on_off *IO, s_option_digital_input *opt)
{
  CreateBoardComponent(IO);
  switch (IO->board_type)
  {
    case IO_06_14_ID:
	  if (opt_io.IO_06_14[IO->board_nr].digital_input[IO->IO_nr].opt_type == DIG_IN_EMPTY)
	  {
        opt_io.IO_06_14[IO->board_nr].digital_input[IO->IO_nr] = *opt;
        value.IO_06_14[IO->board_nr].digital_input[IO->IO_nr].ctrl |= 0x0004;
	  }
      break;
    case IO_12_06_ID:
	  if (opt_io.IO_12_06[IO->board_nr].digital_input[IO->IO_nr].opt_type == DIG_IN_EMPTY)
	  {
        opt_io.IO_12_06[IO->board_nr].digital_input[IO->IO_nr] = *opt;
        value.IO_12_06[IO->board_nr].digital_input[IO->IO_nr].ctrl |= 0x0004;
	  }
      break;
    case IO_08_09_ID:
	  if (opt_io.IO_08_09[IO->board_nr].digital_input[IO->IO_nr].opt_type == DIG_IN_EMPTY)
	  {
        opt_io.IO_08_09[IO->board_nr].digital_input[IO->IO_nr] = *opt;
        value.IO_08_09[IO->board_nr].digital_input[IO->IO_nr].ctrl |= 0x0004;
	  }
      break;
    case IO_07_07_ID:
	  if (opt_io.IO_07_07[IO->board_nr].digital_input[IO->IO_nr].opt_type == DIG_IN_EMPTY)
	  {
        opt_io.IO_07_07[IO->board_nr].digital_input[IO->IO_nr] = *opt;
        value.IO_07_07[IO->board_nr].digital_input[IO->IO_nr].ctrl |= 0x0004;
	  }
      break;
    default:
      return (1);
  }

  return (0);
}

static unsigned char CreateRS485Bus(s_board_IO_on_off *IO, s_option_RS485_bus *opt)
{
  CreateBoardComponent(IO);
  switch (IO->board_type)
  {
    case IO_05_07_ID:
	  if (opt_io.IO_05_07[IO->board_nr].RS485_bus.Option == 0)
	  {
        opt_io.IO_05_07[IO->board_nr].RS485_bus = *opt;
        value.IO_05_07[IO->board_nr].RS485_bus.ctrl |= 0x0004;
	  }
      break;
    case IO_07_07_ID:
	  if (opt_io.IO_07_07[IO->board_nr].RS485_bus.Option == 0)
	  {
        opt_io.IO_07_07[IO->board_nr].RS485_bus = *opt;
        value.IO_07_07[IO->board_nr].RS485_bus.ctrl |= 0x0004;
	  }
      break;
    default:
      return (1);
  }

  return (0);
}

unsigned char IOCreateObject(s_board_IO_on_off *IO, unsigned char ComponentId, void *opt)
{
  if (IO == NULL)
    return (1);

  if (IO->board_type == 0)
    return (1);

  switch (ComponentId)
  {
    case ANALOG_INPUT_ID      : /*ret = CreateAnalogInput  (IO, Type);*/ break;
    case DIGITAL_INPUT_ID     : return CreateDigitalInput(IO, opt);
    case ANALOG_OUTPUT_ID     : /*ret = CreateAnalogOutput (IO, Type);*/ break;
    case DIGITAL_OUTPUT_ID    : /*ret = CreateDigitalOutput(IO, Type);*/ break;
    case COUNTER_INPUT_ID     : break;
    case EMEC_COUNTER_INPUT_ID: break;
    case IRIS_OPTIONS         : break;
    case IRIS_SETPOINTS       : break;
    case IRIS_VALUES          : break;
    case ANALOG_HIGH_INPUT_ID : /*ret = CreateAnalogHighInput(IO, Type);*/ break;
    case MOTOR_CONTROL_ID     : /*ret = CreateMotorControl   (IO, Type);*/ break;
    case RS485_BUS_ID         : return CreateRS485Bus(IO, opt);
  }
  return (0);
}

