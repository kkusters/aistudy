// CH_IO_06_14_DAC.C

#include "ch_define.h"

#include "ch_IO_06_14_dac.h"

unsigned char IO_06_14_Analog_Output_PDO_Transmit(unsigned char *ptr, unsigned char card_nr, unsigned char component_nr)
{
unsigned int i;

  if ((component_nr < IO_06_14_ANALOG_OUTPUT) &&
      (opt_io.IO_06_14[card_nr].analog_output.option & 0x000F))
  {
    i = val_hr_alg.IO_06_14[card_nr].analog_output.value;
    ptr[0] = i & 0x00FF;
	ptr[1] = i >> 8;
	value.IO_06_14[card_nr].analog_output.ctrl &= 0xFFFE;
	value.IO_06_14[card_nr].analog_output.ctrl |= 0x0100;
	return (2);
  }
  return (0);
}

void IO_06_14_Analog_Output_PDO_Transmit_OK(unsigned char card_nr, unsigned char component_nr)
{
  if ((component_nr < IO_06_14_ANALOG_OUTPUT) &&
      (opt_io.IO_06_14[card_nr].analog_output.option & 0x000F))
  {
	value.IO_06_14[card_nr].analog_output.ctrl &= 0xFEFF;
  }
}

unsigned int IO_06_14_Analog_Output_SDO_Master_Transmit(u_SDO_data *ptr, unsigned char card_nr, unsigned char component_nr)
{
  if (component_nr < IO_06_14_ANALOG_OUTPUT) 
  {
	ptr->analog_output.value = val_hr_alg.IO_06_14[card_nr].analog_output.value;
    ptr->analog_output.command = val_hr_alg.IO_06_14[card_nr].analog_output.command;
	ptr->analog_output.option = opt_io.IO_06_14[card_nr].analog_output.option;
	ptr->analog_output.min_in = opt_io.IO_06_14[card_nr].analog_output.min_in;	
	ptr->analog_output.max_in = opt_io.IO_06_14[card_nr].analog_output.max_in;
	ptr->analog_output.min_out = opt_io.IO_06_14[card_nr].analog_output.min_out;
	ptr->analog_output.max_out = opt_io.IO_06_14[card_nr].analog_output.max_out;
	ptr->analog_output.class_nr = opt_io.IO_06_14[card_nr].analog_output.class_nr;
	return (sizeof(s_analog_output));
  }
  return (0);
}

void IO_06_14_Analog_Output_SDO_Master_Transmit_OK(unsigned char card_nr, unsigned char component_nr)
{
  if (component_nr < IO_06_14_ANALOG_OUTPUT) 
  {
	value.IO_06_14[card_nr].analog_output.ctrl &= 0xFFFB;
  }
}

void IO_06_14_Analog_Output_SDO_Master_Receive(u_SDO_data *ptr, unsigned char card_nr, unsigned char component_nr)
// wordt momenteel niet gebruikt
{
  ptr->analog_output.value = 0; // deze regel is er alleen om een waarschuwing te voorkomen
  if (component_nr < IO_06_14_ANALOG_OUTPUT) 
  {
//	val_hr_alg.IO_06_14[card_nr].analog_output.value = ptr->analog_output.value;
//	val_hr_alg.IO_06_14[card_nr].analog_output.command = ptr->analog_output.command;
//	opt_io.IO_06_14[card_nr].analog_output.option = ptr->analog_output.option;
//	opt_io.IO_06_14[card_nr].analog_output.min_in = ptr->analog_output.min_in;
//	opt_io.IO_06_14[card_nr].analog_output.max_in = ptr->analog_output.max_in;
//	opt_io.IO_06_14[card_nr].analog_output.min_out = ptr->analog_output.min_out;
//	opt_io.IO_06_14[card_nr].analog_output.max_out = ptr->analog_output.max_out;
//	opt_io.IO_06_14[card_nr].analog_output.class_nr = ptr->analog_output.class_nr;
    value.IO_06_14[card_nr].analog_output.ctrl &= 0xFFF7;
  }
}

void IO_06_14_Analog_Output_Init(unsigned char card_nr)
// wordt aan geroepen als can opnieuw geinitialiseerd wordt
{
  value.IO_06_14[card_nr].analog_output.ctrl = 0x0000;
  value.IO_06_14[card_nr].analog_output.interval_timer = rom.IO_06_14[card_nr].analog_output.interval_time;
}

void IO_06_14_Analog_Output_100ms(unsigned char card_nr)
// wordt elke 100ms aangeroepen om te kijken of er periodieke data verzonden moet worden
{
  if ((opt_io.IO_06_14[card_nr].analog_output.option & 0x000F) &&
      (rom.IO_06_14[card_nr].analog_output.interval_time)) 
  {
    if (value.IO_06_14[card_nr].analog_output.interval_timer)
      value.IO_06_14[card_nr].analog_output.interval_timer--;
    else
    {
      value.IO_06_14[card_nr].analog_output.interval_timer = rom.IO_06_14[card_nr].analog_output.interval_time;
      value.IO_06_14[card_nr].analog_output.ctrl |= 0x0001;
	}
  }
}

