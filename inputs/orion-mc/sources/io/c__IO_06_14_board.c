// C__IO_06_14_BOARD.C

#include "ch_define.h"

#include "ch_io_06_14_board.h"

unsigned char IO_06_14_init_switch[IO_06_14_MAX] = {1,1,1,1};

/*************************************************************************************************/
void IO_06_14_Alarm_Check(unsigned char alarm)
// routine wordt aangeroepen vanuit de alarm procedure om er voor te zorgen dat bij het wijzigen van de 
// alarm toestand het alarm zo snel mogelijk verzonden wordt.
{
unsigned char card_nr;
bit help;

  for (card_nr = 0; card_nr < IO_06_14_MAX; card_nr++)
  {
    if ((opt_io.IO_06_14[card_nr].board_component.option & 0x000F) &&
        (value.IO_06_14[card_nr].board_component.ctrl & 0x0020)) // check if card installed
	{
	  help = (val_hr_alg.IO_06_14[card_nr].board_component.command & 0x0002);
	  if (help) 
	  {	// alarm bij IO bord is aan (geen alarm)
	    if (alarm && opt_io.IO_06_14[card_nr].alarm)
		{
		  val_hr_alg.IO_06_14[card_nr].board_component.command &= 0xFFFD;
          if (value.IO_06_14[card_nr].board_component.ctrl & 0x0020) // test JP 25-06-04 2x init bord
		    value.IO_06_14[card_nr].board_component.ctrl |= 0x0001;
		}
	  }
	  else
	  { // alarm bij IO bord is uit (alarm)
	    if (!alarm || !opt_io.IO_06_14[card_nr].alarm)
		{
		  val_hr_alg.IO_06_14[card_nr].board_component.command |= 0x0002;
          if (value.IO_06_14[card_nr].board_component.ctrl & 0x0020) // test JP 25-06-04 2x init bord
		    value.IO_06_14[card_nr].board_component.ctrl |= 0x0001;
		}
	  }
	}
  }
}

/*************************************************************************************************/
unsigned char IO_06_14_Board_Component_PDO_Transmit(unsigned char *ptr, unsigned char card_nr)
{
unsigned int i;

  i = (10 * rom.IO_06_14[card_nr].board_component.watchdog_time) / PULSES_PER_SECOND; // send nieuw watchdog value naar IO voor triggering
  ptr[0] = i & 0x00FF;
  ptr[1] = i >> 8;
  i = val_hr_alg.IO_06_14[card_nr].board_component.command;
  ptr[2] = i & 0x00FF;
  ptr[3] = i >> 8;
  value.IO_06_14[card_nr].board_component.ctrl &= 0xFFFE;
  value.IO_06_14[card_nr].board_component.ctrl |= 0x0100;
  return (4);
}

void IO_06_14_Board_Component_PDO_Transmit_OK(unsigned char card_nr)
{
  value.IO_06_14[card_nr].board_component.ctrl &= 0xFEFF;
}

void IO_06_14_Board_Component_PDO_Receive(unsigned char *ptr, unsigned char length, unsigned char card_nr)
{
unsigned char comp_nr;
static unsigned long received_time_stamp;

  if (length == 8)
  {
    if (ptr[1] || ptr[0])
    {
      value.IO_06_14[card_nr].board_component.watchdog_timer = rom.IO_06_14[card_nr].board_component.watchdog_time;
      value.IO_06_14[card_nr].board_component.watchdog_update_timer = rom.IO_06_14[card_nr].board_component.watchdog_update_time;
    }
    else if ((value.IO_06_14[card_nr].board_component.ctrl & 0x0030) == 0x0030)
    {
      value.IO_06_14[card_nr].board_component.watchdog_timer = value.IO_06_14[card_nr].board_component.watchdog_update_timer = 0;
    }

    if (value.IO_06_14[card_nr].board_component.ctrl & 0x0010) // Comm OK ?
    {
      if (!value.IO_06_14[card_nr].board_component.ctrl & 0x0020) // Board Init?
	  {
	    return;
	  }
	  else
	  {
        received_time_stamp = ((((((unsigned long)ptr[7] << 8) + ptr[6]) << 8) + ptr[5]) << 8) + ptr[4];
        if (opt_io.IO_06_14[card_nr].board_component.time_stamp == received_time_stamp) // timestamp OK?
		{
          val_hr_alg.IO_06_14[card_nr].board_component.error_code = ((unsigned int)ptr[3] << 8) + ptr[2];
          if (val_hr_alg.IO_06_14[card_nr].board_component.error_code != 1) // error_code = 1; bord niet meer geinitialiseerd, dus opnieuw initialiseren
		    return;
		}
	  }
    }
    else
	{
      value.IO_06_14[card_nr].board_component.ctrl |= 0x0010;
	}

    // send all SDO components to the IO board
	// except the board component. 
	// the board component is send if all other SDO components are sended
    value.IO_06_14[card_nr].board_component.ctrl &= 0xFFDB; // board init = 0; send SDO board component = 0
	value.IO_06_14[card_nr].board_component.ctrl |= 0x0008; // receive SDO board component for version numbers
    for (comp_nr = 0; comp_nr < IO_06_14_ANALOG_INPUT; comp_nr++)
      value.IO_06_14[card_nr].analog_input[comp_nr].ctrl = 0x0004;
    for (comp_nr = 0; comp_nr < IO_06_14_DIGITAL_INPUT; comp_nr++)
      value.IO_06_14[card_nr].digital_input[comp_nr].ctrl = 0x0004;
    value.IO_06_14[card_nr].analog_output.ctrl = 0x0004;
    value.IO_06_14[card_nr].digital_output.ctrl = 0x0004;
  }
}

unsigned int IO_06_14_Board_Component_SDO_Master_Transmit(u_SDO_data *ptr, unsigned char card_nr)
{
  ptr->board_component.watchdog_timer = (10 * rom.IO_06_14[card_nr].board_component.watchdog_time) / PULSES_PER_SECOND;
  ptr->board_component.command = val_hr_alg.IO_06_14[card_nr].board_component.command;
  ptr->board_component.error_code = 0;
  ptr->board_component.option = opt_io.IO_06_14[card_nr].board_component.option;
  ptr->board_component.version_hardware = value.IO_06_14[card_nr].board_component.version_hardware; 
  ptr->board_component.version_software = value.IO_06_14[card_nr].board_component.version_software;
  ptr->board_component.time_stamp = opt_io.IO_06_14[card_nr].board_component.time_stamp; 
  return (sizeof(s_board_component));
}

void IO_06_14_Board_Component_SDO_Master_Transmit_OK(unsigned char card_nr)
{
  value.IO_06_14[card_nr].board_component.ctrl &= 0xFFFB;
  value.IO_06_14[card_nr].board_component.ctrl |= 0x0020;
}

void IO_06_14_Board_Component_SDO_Master_Receive(u_SDO_data *ptr, unsigned char card_nr)
{
  if (ptr->board_component.watchdog_timer)
  {
    value.IO_06_14[card_nr].board_component.watchdog_timer = rom.IO_06_14[card_nr].board_component.watchdog_time;
    value.IO_06_14[card_nr].board_component.watchdog_update_timer = rom.IO_06_14[card_nr].board_component.watchdog_update_time;
  }
  else
  {
    value.IO_06_14[card_nr].board_component.watchdog_timer = value.IO_06_14[card_nr].board_component.watchdog_update_timer = 0;
	return;
  }
//  val_hr_alg.IO_06_14[card_nr].board_component.error_code = ptr->board_component.error_code;
//  opt_io.IO_06_14[card_nr].board_component.option = ptr->board_component.option;
  value.IO_06_14[card_nr].board_component.version_hardware = ptr->board_component.version_hardware;
  value.IO_06_14[card_nr].board_component.version_software = ptr->board_component.version_software;
//  opt_io.IO_06_14[card_nr].board_component.time_stamp = ptr->board_component.time_stamp;
  value.IO_06_14[card_nr].board_component.ctrl &= 0xFFF7;
}

void IO_06_14_Board_Component_Init(unsigned char card_nr)
// wordt aan geroepen als can opnieuw geinitialiseerd wordt
{
  value.IO_06_14[card_nr].board_component.ctrl = 0x0001;
  value.IO_06_14[card_nr].board_component.watchdog_timer = rom.IO_06_14[card_nr].board_component.watchdog_time;
  value.IO_06_14[card_nr].board_component.watchdog_update_timer = rom.IO_06_14[card_nr].board_component.watchdog_update_time;
  value.IO_06_14[card_nr].board_component.version_hardware = 0;
  value.IO_06_14[card_nr].board_component.version_software = 0;  
  val_hr_alg.IO_06_14[card_nr].board_component.command = 0;
  val_hr_alg.IO_06_14[card_nr].board_component.error_code = 0;
}

unsigned char IO_06_14_Board_Component_100ms(unsigned char card_nr)
// wordt elke 100ms aangeroepen om te kijken of er periodieke data verzonden moet worden
{
  if (opt_io.IO_06_14[card_nr].board_component.option & 0x000F)
  {
    if (value.IO_06_14[card_nr].board_component.watchdog_timer)
    {
      value.IO_06_14[card_nr].board_component.watchdog_timer--;
      if (value.IO_06_14[card_nr].board_component.watchdog_update_timer)
      { 
        value.IO_06_14[card_nr].board_component.watchdog_update_timer--;
        if (value.IO_06_14[card_nr].board_component.watchdog_update_timer == 0)
		{
          value.IO_06_14[card_nr].board_component.ctrl |= 0x0001;
		}
	  }
	  return (1);
    }
    else
    {
      IO_06_14_init_switch[card_nr] = 1;
    }
  }
  return (0);
}


