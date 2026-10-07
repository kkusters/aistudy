// C__IO_12_06_COUNT.C

#include "ch_define.h"

#include "ch_IO_12_06_count.h"

// deze waarde straks vergroten naar grotere waarde om aantal SDO berichten zo klein mogelijk te houden
#define RESET_COUNT 1000

static unsigned char sdo_count_busy[IO_12_06_COUNTER_INPUT] = {0,0,0,0};

void IO_12_06_Counter_Input_PDO_Receive(unsigned char *ptr, unsigned char length, unsigned char card_nr, unsigned char component_nr)
{
int val;
int nr;  // nummer teller

  if ((component_nr < IO_12_06_COUNTER_INPUT) &&
	  (sdo_count_busy[component_nr] == 0) &&
      (length == 4) &&
      (opt_io.IO_12_06[card_nr].counter_input[component_nr].option & 0x000F) &&
      !(value.IO_12_06[card_nr].counter_input[component_nr].ctrl & 0x0004))
  {
    val = (ptr[1] << 8) + ptr[0];
	nr = ptr[2];
	val_hr_alg.IO_12_06[card_nr].counter_input[component_nr].error[nr] = ptr[3];
    if ((opt_io.IO_12_06[card_nr].counter_input[component_nr].option & 0x000F) == 0x0002) // teller
	{
	  if (val > val_hr_alg.IO_12_06[card_nr].counter_input[component_nr].value[nr]) // alleen tellen als verschil positief
	  {
        val_hr_alg.IO_12_06[card_nr].counter_input[component_nr].count[nr] += (val - val_hr_alg.IO_12_06[card_nr].counter_input[component_nr].value[nr]);
	  }

	  val_hr_alg.IO_12_06[card_nr].counter_input[component_nr].value[nr] = val;
	  if (val >= RESET_COUNT) // value groter dan constante dan reset value met behulp van een SDO bericht
	  {
	    val_hr_alg.IO_12_06[card_nr].counter_input[component_nr].command |= 0x0002; // set command bit for reset
		value.IO_12_06[card_nr].counter_input[component_nr].ctrl |= 0x0004; // send SDO
	  }
	}
    else
      val_hr_alg.IO_12_06[card_nr].counter_input[component_nr].value[nr] = val;
  }
}

unsigned int IO_12_06_Counter_Input_SDO_Master_Transmit(u_SDO_data *ptr, unsigned char card_nr, unsigned char component_nr)
{
int loop;

  if (component_nr < IO_12_06_COUNTER_INPUT) 
  {
	sdo_count_busy[component_nr] = 1;
	for (loop = 0; loop < 16; loop++)
	{
	  val_hr_alg.IO_12_06[card_nr].counter_input[component_nr].old_value[loop] = val_hr_alg.IO_12_06[card_nr].counter_input[component_nr].value[loop];
  	  ptr->counter_input.value[loop] = val_hr_alg.IO_12_06[card_nr].counter_input[component_nr].old_value[loop];
	}
    ptr->counter_input.command = val_hr_alg.IO_12_06[card_nr].counter_input[component_nr].command;
    ptr->counter_input.option = opt_io.IO_12_06[card_nr].counter_input[component_nr].option;
    ptr->counter_input.pulses_per_count = opt_io.IO_12_06[card_nr].counter_input[component_nr].pulses_per_count;
    ptr->counter_input.enable = opt_io.IO_12_06[card_nr].counter_input[component_nr].enable;
    ptr->counter_input.difference = opt_io.IO_12_06[card_nr].counter_input[component_nr].difference;
    ptr->counter_input.interval_time = opt_io.IO_12_06[card_nr].counter_input[component_nr].interval_time;
	ptr->counter_input.class_nr = opt_io.IO_12_06[card_nr].counter_input[component_nr].class_nr;
	return (sizeof(s_counter_input));
  }
  return (0);
}

void IO_12_06_Counter_Input_SDO_Master_Transmit_OK(unsigned char card_nr, unsigned char component_nr)
{
int loop;

  if (component_nr < IO_12_06_COUNTER_INPUT) 
  {
    if ((opt_io.IO_12_06[card_nr].counter_input[component_nr].option & 0x000F) == 0x0002) // teller
	{
	  if (val_hr_alg.IO_12_06[card_nr].counter_input[component_nr].command & 0x0002) // command voor resetten teller
	  {
        val_hr_alg.IO_12_06[card_nr].counter_input[component_nr].command = 0;
		for (loop = 0; loop < 16; loop++)
          val_hr_alg.IO_12_06[card_nr].counter_input[component_nr].value[loop] -= val_hr_alg.IO_12_06[card_nr].counter_input[component_nr].old_value[loop];
	  }
	}
	sdo_count_busy[component_nr] = 0;
	value.IO_12_06[card_nr].counter_input[component_nr].ctrl &= 0xFFFB;
  }
}

void IO_12_06_Counter_Input_SDO_Master_Receive(u_SDO_data *ptr, unsigned char card_nr, unsigned char component_nr)
// wordt momenteel niet gebruikt
{
  ptr->counter_input.value[0] = 0; // deze regel is er alleen om een waarschuwing te voorkomen
  if (component_nr < IO_12_06_COUNTER_INPUT) 
  {
//  val_hr_alg.IO_12_06[card_nr].digital_input[component_nr].value = ptr->digital_input.value;
//	val_hr_alg.IO_12_06[card_nr].digital_input[component_nr].command = ptr->digital_input.command;
//	opt_io.IO_12_06[card_nr].digital_input[component_nr].option = ptr->digital_input.option;
//	opt_io.IO_12_06[card_nr].digital_input[component_nr].counts_per_puls = ptr->digital_input.counts_per_puls;
//	opt_io.IO_12_06[card_nr].digital_input[component_nr].pulses_per_count = ptr->digital_input.pulses_per_count;
//	opt_io.IO_12_06[card_nr].digital_input[component_nr].enable = ptr->digital_input.enable;
//	opt_io.IO_12_06[card_nr].digital_input[component_nr].upper_limit = ptr->digital_input.upper_limit;
//	opt_io.IO_12_06[card_nr].digital_input[component_nr].lower_limit = ptr->digital_input.lower_limit;
//	opt_io.IO_12_06[card_nr].digital_input[component_nr].difference = ptr->digital_input.difference;
//	opt_io.IO_12_06[card_nr].digital_input[component_nr].interval_time = ptr->digital_input.interval_time;
//	opt_io.IO_12_06[card_nr].digital_input[component_nr].class_nr = ptr->digital_input.class_nr;
    value.IO_12_06[card_nr].counter_input[component_nr].ctrl &= 0xFFF7;
  }
}

void IO_12_06_Counter_Input_Init(unsigned char card_nr)
// wordt aan geroepen als can opnieuw geinitialiseerd wordt
{
unsigned char comp_nr;

  for (comp_nr = 0; comp_nr < IO_12_06_COUNTER_INPUT; comp_nr++)
  {
    value.IO_12_06[card_nr].counter_input[comp_nr].ctrl = 0x0000;
  }
}

