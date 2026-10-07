// C__IO_06_14_ADC.C

#include "ch_define.h"

#include "ch_io_06_14_adc.h"

void IO_06_14_Analog_Input_PDO_Receive(unsigned char *ptr, unsigned char length, unsigned char card_nr, unsigned char component_nr)
{
int help;

  if ((component_nr < IO_06_14_ANALOG_INPUT) &&
      (length == 2) &&
      (opt_io.IO_06_14[card_nr].analog_input[component_nr].option & 0x000F))
  {
	help = (ptr[1] << 8) + ptr[0];
    switch (opt_io.IO_06_14[card_nr].analog_input[component_nr].option)
	{
	  case ANA_INPUT_CELSIUS: // temperatuur in graden celsius
        break;
	  case ANA_INPUT_FAHRENHEID: // temperatuur in graden fahrenheid
		break;
	  case ANA_INPUT_0_5V: // 0-5V geschaald
        val_hr_alg.IO_06_14[card_nr].analog_input[component_nr].value = help;
	    break;
	  case ANA_INPUT_0_5V_NO_LIMIT: // 0-5V geschaald no limit
	  default:
        val_hr_alg.IO_06_14[card_nr].analog_input[component_nr].value = help;
	    break;
	}
  }
}

unsigned int IO_06_14_Analog_Input_SDO_Master_Transmit(u_SDO_data *ptr, unsigned char card_nr, unsigned char component_nr)
{
  if (component_nr < IO_06_14_ANALOG_INPUT) 
  {
	ptr->analog_input.value = val_hr_alg.IO_06_14[card_nr].analog_input[component_nr].value;
    ptr->analog_input.command = val_hr_alg.IO_06_14[card_nr].analog_input[component_nr].command;
    ptr->analog_input.option = opt_io.IO_06_14[card_nr].analog_input[component_nr].option;
    ptr->analog_input.min_in = opt_io.IO_06_14[card_nr].analog_input[component_nr].min_in;
    ptr->analog_input.max_in = opt_io.IO_06_14[card_nr].analog_input[component_nr].max_in;
    ptr->analog_input.min_out = opt_io.IO_06_14[card_nr].analog_input[component_nr].min_out;
    ptr->analog_input.max_out = opt_io.IO_06_14[card_nr].analog_input[component_nr].max_out;
    ptr->analog_input.enable = opt_io.IO_06_14[card_nr].analog_input[component_nr].enable;
    ptr->analog_input.upper_limit = opt_io.IO_06_14[card_nr].analog_input[component_nr].upper_limit;
    ptr->analog_input.lower_limit = opt_io.IO_06_14[card_nr].analog_input[component_nr].lower_limit;
    ptr->analog_input.difference = opt_io.IO_06_14[card_nr].analog_input[component_nr].difference;
    ptr->analog_input.interval_time = opt_io.IO_06_14[card_nr].analog_input[component_nr].interval_time;
	ptr->analog_input.class_nr = opt_io.IO_06_14[card_nr].analog_input[component_nr].class_nr;
	return (sizeof(s_analog_input));
  }
  return (0);
}

void IO_06_14_Analog_Input_SDO_Master_Transmit_OK(unsigned char card_nr, unsigned char component_nr)
{
  if (component_nr < IO_06_14_ANALOG_INPUT) 
  {
	value.IO_06_14[card_nr].analog_input[component_nr].ctrl &= 0xFFFB;
  }
}

void IO_06_14_Analog_Input_SDO_Master_Receive(u_SDO_data *ptr, unsigned char card_nr, unsigned char component_nr)
// wordt momenteel niet gebruikt
{
  ptr->analog_input.value = 0; // deze regel is er alleen om een waarschuwing te voorkomen
  if (component_nr < IO_06_14_ANALOG_INPUT) 
  {
//	val_hr_alg.IO_06_14[card_nr].analog_input[component_nr].value = ptr->analog_input.value;
//  val_hr_alg.IO_06_14[card_nr].analog_input[component_nr].command = ptr->analog_input.command;
//  opt_io.IO_06_14[card_nr].analog_input[component_nr].option = ptr->analog_input.option;
//  opt_io.IO_06_14[card_nr].analog_input[component_nr].min_in = ptr->analog_input.min_in;
//  opt_io.IO_06_14[card_nr].analog_input[component_nr].max_in = ptr->analog_input.max_in;
//  opt_io.IO_06_14[card_nr].analog_input[component_nr].min_out = ptr->analog_input.min_out;
//  opt_io.IO_06_14[card_nr].analog_input[component_nr].max_out = ptr->analog_input.max_out;
//  opt_io.IO_06_14[card_nr].analog_input[component_nr].enable = ptr->analog_input.enable;
//  opt_io.IO_06_14[card_nr].analog_input[component_nr].upper_limit = ptr->analog_input.upper_limit;
//  opt_io.IO_06_14[card_nr].analog_input[component_nr].lower_limit = ptr->analog_input.lower_limit;
//  opt_io.IO_06_14[card_nr].analog_input[component_nr].difference = ptr->analog_input.difference;
//  opt_io.IO_06_14[card_nr].analog_input[component_nr].interval_time = ptr->analog_input.interval_time;
//	opt_io.IO_06_14[card_nr].analog_input[component_nr].class_nr = ptr->analog_input.class_nr;
    value.IO_06_14[card_nr].analog_input[component_nr].ctrl &= 0xFFF7;
  }
}

void IO_06_14_Analog_Input_Init(unsigned char card_nr)
// wordt aan geroepen als can opnieuw geinitialiseerd wordt
{
unsigned char comp_nr;

  for (comp_nr = 0; comp_nr < IO_06_14_ANALOG_INPUT; comp_nr++)
  {
    value.IO_06_14[card_nr].analog_input[comp_nr].ctrl = 0x0000;
  }
}


