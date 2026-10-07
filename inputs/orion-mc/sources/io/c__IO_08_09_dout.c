// CH_IO_08_09_DOUT.C

#include "ch_define.h"

#include "ch_IO_08_09_dout.h"

unsigned char IO_08_09_Digital_Output_PDO_Transmit(unsigned char *ptr, unsigned char card_nr, unsigned char component_nr)
{
unsigned int i;

  if ((component_nr == 0) &&
      (opt_io.IO_08_09[card_nr].digital_output.option & 0x000F))
  {
    i = val_hr_alg.IO_08_09[card_nr].digital_output.value;
	ptr[0] = i & 0x00FF;
	ptr[1] = i >> 8;
    value.IO_08_09[card_nr].digital_output.ctrl &= 0xFFFE;
    value.IO_08_09[card_nr].digital_output.ctrl |= 0x0100;
	return (2);
  }
  return (0);
}

void IO_08_09_Digital_Output_PDO_Transmit_OK(unsigned char card_nr, unsigned char component_nr)
{
  if ((component_nr == 0) &&
      (opt_io.IO_08_09[card_nr].digital_output.option & 0x000F))
  {
    value.IO_08_09[card_nr].digital_output.ctrl &= 0xFEFF;
  }
}

unsigned int IO_08_09_Digital_Output_SDO_Master_Transmit(u_SDO_data *ptr, unsigned char card_nr, unsigned char component_nr)
{
int loop;

  if (component_nr == 0) 
  {
	ptr->digital_output.value = val_hr_alg.IO_08_09[card_nr].digital_output.value;
    ptr->digital_output.command = val_hr_alg.IO_08_09[card_nr].digital_output.command;
	ptr->digital_output.option = opt_io.IO_08_09[card_nr].digital_output.option;
	ptr->digital_output.invers = opt_io.IO_08_09[card_nr].digital_output.invers;
	for (loop = 0; loop < 16; loop++)
	{
	  ptr->digital_output.class_nr[loop] = opt_io.IO_08_09[card_nr].digital_output.class_nr[loop];
	}
	return (sizeof(s_digital_output));
  }
  return (0);
}

void IO_08_09_Digital_Output_SDO_Master_Transmit_OK(unsigned char card_nr, unsigned char component_nr)
{
  if (component_nr == 0) 
  {
	value.IO_08_09[card_nr].digital_output.ctrl &= 0xFFFB;
  }
}

void IO_08_09_Digital_Output_SDO_Master_Receive(u_SDO_data *ptr, unsigned char card_nr, unsigned char component_nr)
// wordt momenteel niet gebruikt
{
//int loop;

  ptr->digital_output.value = 0; // deze regel is er alleen om een waarschuwing te voorkomen
  if (component_nr == 0) 
  {
//	val_hr_alg.IO_08_09[card_nr].digital_output.value = ptr->digital_output.value;
//	val_hr_alg.IO_08_09[card_nr].digital_output.command = ptr->digital_output.command;
//	opt_io.IO_08_09[card_nr].digital_output.option = ptr->digital_output.option;
//	opt_io.IO_08_09[card_nr].digital_output.invers = ptr->digital_output.invers;
//	for (loop = 0; loop < 16; loop++)
//	{
//	  opt_io.IO_08_09[card_nr].digital_output.class_nr[loop] = ptr->digital_output.class_nr[loop];
//	}
    value.IO_08_09[card_nr].digital_output.ctrl &= 0xFFF7;
  }
}

void IO_08_09_Digital_Output_Init(unsigned char card_nr)
// wordt aan geroepen als can opnieuw geinitialiseerd wordt
{
  value.IO_08_09[card_nr].digital_output.ctrl = 0x0000;
  value.IO_08_09[card_nr].digital_output.interval_timer = rom.IO_08_09[card_nr].digital_output.interval_time;
}

void IO_08_09_Digital_Output_100ms(unsigned char card_nr)
// wordt elke 100ms aangeroepen om te kijken of er periodieke data verzonden moet worden
{
  if ((opt_io.IO_08_09[card_nr].digital_output.option & 0x000F) &&
      (rom.IO_08_09[card_nr].digital_output.interval_time)) 
  {
    if (value.IO_08_09[card_nr].digital_output.interval_timer)
      value.IO_08_09[card_nr].digital_output.interval_timer--;
	else
	{
	  value.IO_08_09[card_nr].digital_output.interval_timer = rom.IO_08_09[card_nr].digital_output.interval_time;
	  value.IO_08_09[card_nr].digital_output.ctrl |= 0x0001;
	}
  }
}

