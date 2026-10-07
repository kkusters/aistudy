// C__IO_07_07_ADC.C

#include "ch_define.h"

#include "ch_alarm.h"
#include "ch_IO_05_07_adc.h"

void IO_07_07_Analog_Input_PDO_Receive(unsigned char *ptr, unsigned char length, unsigned char card_nr, unsigned char component_nr)
{
int help;

  if ((component_nr < IO_07_07_ANALOG_INPUT) &&
      (length == 2) &&
      (opt_io.IO_07_07[card_nr].analog_input[component_nr].option & 0x000F))
  {
	help = (ptr[1] << 8) + ptr[0];
    switch (opt_io.IO_07_07[card_nr].analog_input[component_nr].option & 0x000F)
	{
	  case ANA_INPUT_CELSIUS: // temperatuur in graden celsius
	    val_hr_alg.IO_07_07[card_nr].analog_input[component_nr].value = ((help >= -500) && (help <= 1020)) ? help : (int)0x8000;
        break;
	  case ANA_INPUT_FAHRENHEID: // temperatuur in graden fahrenheid
        val_hr_alg.IO_07_07[card_nr].analog_input[component_nr].value = ((help >= -580) && (help <= 2156)) ? help : (int)0x8000;
		break;
	  case ANA_INPUT_0_5V: // 0-5V geschaald
        val_hr_alg.IO_07_07[card_nr].analog_input[component_nr].value = help;
	    break;
	  case ANA_INPUT_0_5V_NO_LIMIT: // 0-5V geschaald no limit
	  default:
        val_hr_alg.IO_07_07[card_nr].analog_input[component_nr].value = help;
	    break;
	}
  }
}

unsigned int IO_07_07_Analog_Input_SDO_Master_Transmit(u_SDO_data *ptr, unsigned char card_nr, unsigned char component_nr)
{
  if (component_nr < IO_07_07_ANALOG_INPUT) 
  {
	ptr->analog_input.value = val_hr_alg.IO_07_07[card_nr].analog_input[component_nr].value;
    ptr->analog_input.command = val_hr_alg.IO_07_07[card_nr].analog_input[component_nr].command;
    ptr->analog_input.option = opt_io.IO_07_07[card_nr].analog_input[component_nr].option;
    ptr->analog_input.min_in = opt_io.IO_07_07[card_nr].analog_input[component_nr].min_in;
    ptr->analog_input.max_in = opt_io.IO_07_07[card_nr].analog_input[component_nr].max_in;
    ptr->analog_input.min_out = opt_io.IO_07_07[card_nr].analog_input[component_nr].min_out;
    ptr->analog_input.max_out = opt_io.IO_07_07[card_nr].analog_input[component_nr].max_out;
    ptr->analog_input.enable = opt_io.IO_07_07[card_nr].analog_input[component_nr].enable;
    ptr->analog_input.upper_limit = opt_io.IO_07_07[card_nr].analog_input[component_nr].upper_limit;
    ptr->analog_input.lower_limit = opt_io.IO_07_07[card_nr].analog_input[component_nr].lower_limit;
    ptr->analog_input.difference = opt_io.IO_07_07[card_nr].analog_input[component_nr].difference;
    ptr->analog_input.interval_time = opt_io.IO_07_07[card_nr].analog_input[component_nr].interval_time;
	ptr->analog_input.class_nr = opt_io.IO_07_07[card_nr].analog_input[component_nr].class_nr;
	return (sizeof(s_analog_input));
  }
  return (0);
}

void IO_07_07_Analog_Input_SDO_Master_Transmit_OK(unsigned char card_nr, unsigned char component_nr)
{
  if (component_nr < IO_07_07_ANALOG_INPUT) 
  {
	value.IO_07_07[card_nr].analog_input[component_nr].ctrl &= 0xFFFB;
  }
}

void IO_07_07_Analog_Input_SDO_Master_Receive(u_SDO_data *ptr, unsigned char card_nr, unsigned char component_nr)
// wordt momenteel niet gebruikt
{
  ptr->analog_input.value = 0; // deze regel is er alleen om een waarschuwing te voorkomen
  if (component_nr < IO_07_07_ANALOG_INPUT) 
  {
//	val_hr_alg.IO_07_07[card_nr].analog_input[component_nr].value = ptr->analog_input.value;
//  val_hr_alg.IO_07_07[card_nr].analog_input[component_nr].command = ptr->analog_input.command;
//  opt_io.IO_07_07[card_nr].analog_input[component_nr].option = ptr->analog_input.option;
//  opt_io.IO_07_07[card_nr].analog_input[component_nr].min_in = ptr->analog_input.min_in;
//  opt_io.IO_07_07[card_nr].analog_input[component_nr].max_in = ptr->analog_input.max_in;
//  opt_io.IO_07_07[card_nr].analog_input[component_nr].min_out = ptr->analog_input.min_out;
//  opt_io.IO_07_07[card_nr].analog_input[component_nr].max_out = ptr->analog_input.max_out;
//  opt_io.IO_07_07[card_nr].analog_input[component_nr].enable = ptr->analog_input.enable;
//  opt_io.IO_07_07[card_nr].analog_input[component_nr].upper_limit = ptr->analog_input.upper_limit;
//  opt_io.IO_07_07[card_nr].analog_input[component_nr].lower_limit = ptr->analog_input.lower_limit;
//  opt_io.IO_07_07[card_nr].analog_input[component_nr].difference = ptr->analog_input.difference;
//  opt_io.IO_07_07[card_nr].analog_input[component_nr].interval_time = ptr->analog_input.interval_time;
//	opt_io.IO_07_07[card_nr].analog_input[component_nr].class_nr = ptr->analog_input.class_nr;
    value.IO_07_07[card_nr].analog_input[component_nr].ctrl &= 0xFFF7;
  }
}

void IO_07_07_Analog_Input_Init(unsigned char card_nr)
// wordt aan geroepen als can opnieuw geinitialiseerd wordt
{
unsigned char comp_nr;

  for (comp_nr = 0; comp_nr < IO_07_07_ANALOG_INPUT; comp_nr++)
  {
    value.IO_07_07[card_nr].analog_input[comp_nr].ctrl = 0x0000;
  }
}

void IO_07_07_Analog_Input_Alarmen_Control(unsigned char card_nr)
// controleerd ana in alarmen en zet de alarmen uit als de ingangen of het board niet geinstalleerd zij
{
int comp_nr;

  // CAN-IO-7-7
  if ((opt_io.IO_07_07[card_nr].board_component.option & 0x000F) != 0)
  {
    for (comp_nr = 0; comp_nr < IO_07_07_ANALOG_INPUT; comp_nr++)
    {
      switch (opt_io.IO_07_07[card_nr].analog_input[comp_nr].option & 0x000F)
      {
        case 1: // temperatuur in graden celsius
        case 2: // temperatuur in graden fahrenheid
          if (val_hr_alg.IO_07_07[card_nr].analog_input[comp_nr].value == (int)0x8000)
            CreateAlarm(&alarm_hr_alg.IO_07_07[card_nr].ana_in_al[comp_nr], IO_07_07_ANA_IN_1_AL + (IO_07_07_ANA_IN_2_AL - IO_07_07_ANA_IN_1_AL) * comp_nr, card_nr, comp_nr + 1, 0, HARD_ALARM);
          else
            ClearAlarm(&alarm_hr_alg.IO_07_07[card_nr].ana_in_al[comp_nr], IO_07_07_ANA_IN_1_AL + (IO_07_07_ANA_IN_2_AL - IO_07_07_ANA_IN_1_AL) * comp_nr, card_nr);
          break;
        case 3: // 0-5V geschaald
        case 4: // 0-5V geschaald no limit
        default:
          ClearAlarm(&alarm_hr_alg.IO_07_07[card_nr].ana_in_al[comp_nr], IO_07_07_ANA_IN_1_AL + (IO_07_07_ANA_IN_2_AL - IO_07_07_ANA_IN_1_AL) * comp_nr, card_nr);
          break;
      }
    }
  }
  else
  {
    for (comp_nr = 0; comp_nr < IO_07_07_ANALOG_INPUT; comp_nr++)
    {
      ClearAlarm(&alarm_hr_alg.IO_07_07[card_nr].ana_in_al[comp_nr], IO_07_07_ANA_IN_1_AL + (IO_07_07_ANA_IN_2_AL - IO_07_07_ANA_IN_1_AL) * comp_nr, card_nr);
    }
  }
}


