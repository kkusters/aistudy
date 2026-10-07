// C__IO_05_07.C

#include "ch_define.h"

#include "ch_alarm.h"
#include "ch_can_io.h"
#include "ch_IO.h"
#include "ch_IO_05_07_board.h"
#include "ch_IO_05_07_adc.h"
#include "ch_IO_05_07_dac.h"
#include "ch_IO_05_07_dout.h"
#include "ch_IO_05_07_RS485.h"
#include "ch_IO_05_07.h"

#define IO_05_07_VERSION_SOFTWARE 115

// OPMERKING als transmit PDO CPU evenlang als transmit PDO IO board dan 
// bij 2 aangesloten IO boarden met het zelfde kaard nr, zal PDO van IO board als PDO van CPU board geaccepteerd worden

/**********************************************************************************************************/
void IO_05_07_Alarm_One_Board(unsigned char nr)
{
static int cnt[IO_05_07_MAX] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

  if (opt_io.IO_05_07[nr].board_component.option & 0x000F)
  {
    if (!(value.IO_05_07[nr].board_component.ctrl & 0x0020))  // board niet geinitialiseerd
    {
      if (cnt[nr] < 5)
        cnt[nr]++;
      else
        CreateAlarm(&alarm_hr_alg.IO_05_07[nr].board_al, IO_05_07_BOARD_AL, nr, 0, 0, HARD_ALARM);
    }
    else 
    {
      cnt[nr] = 0;
      ClearAlarm(&alarm_hr_alg.IO_05_07[nr].board_al, IO_05_07_BOARD_AL, nr);
      if (val_hr_alg.IO_05_07[nr].board_component.error_code == 2)
        CreateAlarm(&alarm_hr_alg.IO_05_07[nr].adc_al, IO_05_07_ADC_AL, nr, 0, 0, HARD_ALARM);
      else
        ClearAlarm(&alarm_hr_alg.IO_05_07[nr].adc_al, IO_05_07_ADC_AL, nr);
      if (val_hr_alg.IO_05_07[nr].board_component.error_code == 5)
        CreateAlarm(&alarm_hr_alg.IO_05_07[nr].externe_24v_al, IO_05_07_EXTERNE_24V_AL, nr, 0, 0, HARD_ALARM);
      else
        ClearAlarm(&alarm_hr_alg.IO_05_07[nr].externe_24v_al, IO_05_07_EXTERNE_24V_AL, nr);
      if ((val_hr_alg.IO_05_07[nr].board_component.error_code > 1) && (val_hr_alg.IO_05_07[nr].board_component.error_code != 2) && (val_hr_alg.IO_05_07[nr].board_component.error_code != 5))
        CreateAlarm(&alarm_hr_alg.IO_05_07[nr].onbekend_al, IO_05_07_ONBEKEND_AL, nr, val_hr_alg.IO_05_07[nr].board_component.error_code, 0, HARD_ALARM);
      else  
        ClearAlarm(&alarm_hr_alg.IO_05_07[nr].onbekend_al, IO_05_07_ONBEKEND_AL, nr);
      if (((opt_io.IO_05_07[nr].RS485_bus.Option & 0x000F) == RS485ModeModbus) && value.IO_05_07[nr].board_component.version_software && (value.IO_05_07[nr].board_component.version_software < IO_05_07_VERSION_SOFTWARE))
        CreateAlarm(&alarm_hr_alg.IO_05_07_versie_al[nr], IO_05_07_VERSIE_AL, nr, 0, 0, ZACHT_ALARM);
      else 
        ClearAlarm(&alarm_hr_alg.IO_05_07_versie_al[nr], IO_05_07_VERSIE_AL, nr);
    }
  }
  else
  {
    cnt[nr] = 0;
    ClearAlarm(&alarm_hr_alg.IO_05_07[nr].board_al,       IO_05_07_BOARD_AL,       nr);
    ClearAlarm(&alarm_hr_alg.IO_05_07[nr].adc_al,         IO_05_07_ADC_AL,         nr);
    ClearAlarm(&alarm_hr_alg.IO_05_07[nr].externe_24v_al, IO_05_07_EXTERNE_24V_AL, nr);
    ClearAlarm(&alarm_hr_alg.IO_05_07[nr].onbekend_al,    IO_05_07_ONBEKEND_AL,    nr);
    ClearAlarm(&alarm_hr_alg.IO_05_07_versie_al[nr],      IO_05_07_VERSIE_AL,      nr);
  }
}

void IO_05_07_Alarm_All_Boards(void)
{
int loop;

  for (loop = 0; loop < IO_05_07_MAX; loop++)
  {
    IO_05_07_Alarm_One_Board(loop);
    IO_05_07_Analog_Input_Alarmen_Control(loop);
  }
}

/**********************************************************************************************************/
unsigned char IO_05_07_Component_PDO_Transmit(unsigned char *ptr, unsigned char card_nr, unsigned char component_id, unsigned char component_nr)
{
  if ((card_nr < IO_05_07_MAX) &&
      (opt_io.IO_05_07[card_nr].board_component.option & 0x000F))
  {
    switch (component_id)
    {
      case BOARD_COMPONENT_ID: // alarm relais
        return (IO_05_07_Board_Component_PDO_Transmit(ptr, card_nr));
      case CLASS_ID: // class component
        switch (component_nr)
        {  
          case 1: // voerweger
            break;
        }
        break;
      case ANALOG_INPUT_ID: // analog input
        break;
      case DIGITAL_INPUT_ID: // digital input
        break;
      case ANALOG_OUTPUT_ID: // analog output
        return (IO_05_07_Analog_Output_PDO_Transmit(ptr, card_nr, component_nr));
      case DIGITAL_OUTPUT_ID: // digital output
        return (IO_05_07_Digital_Output_PDO_Transmit(ptr, card_nr, component_nr));
      case RS485_BUS_ID:
	    return (IO_05_07_RS485_Bus_PDO_Transmit(ptr, card_nr, component_nr));
      default:
        break;
    }
  }
  return (0);
}

/*************************************************************************************************/
void IO_05_07_Component_PDO_Transmit_OK(unsigned char card_nr, unsigned char component_id, unsigned char component_nr)
{
  if ((card_nr < IO_05_07_MAX) &&
      (opt_io.IO_05_07[card_nr].board_component.option & 0x000F))
  {
    switch (component_id) // resetten PDO transmit vlaggen
    {
      case BOARD_COMPONENT_ID: // board component
        IO_05_07_Board_Component_PDO_Transmit_OK(card_nr);
        break;
      case CLASS_ID: // class component
        switch (component_nr)
        {
          case 1: // voerweger
            break;
        }
        break;
      case ANALOG_INPUT_ID: // analog input
        break;
      case DIGITAL_INPUT_ID: // digital input
        break;
      case ANALOG_OUTPUT_ID: // analog output
        IO_05_07_Analog_Output_PDO_Transmit_OK(card_nr, component_nr);
        break;
      case DIGITAL_OUTPUT_ID: // digital output
        IO_05_07_Digital_Output_PDO_Transmit_OK(card_nr, component_nr);
        break;
      case RS485_BUS_ID:
        IO_05_07_RS485_Bus_PDO_Transmit_OK(card_nr, component_nr);
        break;
    }
  }
}

/*************************************************************************************************/
void IO_05_07_Component_PDO_Receive(unsigned char *ptr, unsigned char length, unsigned char card_nr, unsigned char component_id, unsigned char component_nr)
{
  if ((card_nr < IO_05_07_MAX) &&
      (opt_io.IO_05_07[card_nr].board_component.option & 0x000F))
  {
    switch (component_id)
    {
      case BOARD_COMPONENT_ID: // board component
        IO_05_07_Board_Component_PDO_Receive(ptr, length, card_nr);
        break;
      case CLASS_ID: // class component
        switch (component_nr)
        {
          case 2: // voerweger
            break;
        }
        break;
      case ANALOG_INPUT_ID: // analog input 
		if (value.IO_05_07[card_nr].board_component.ctrl & 0x0020)
		  IO_05_07_Analog_Input_PDO_Receive(ptr, length, card_nr, component_nr);
        break;
      case DIGITAL_INPUT_ID: // digital input
        break;                                                   
      case ANALOG_OUTPUT_ID: // Analog output 
        break;
      case DIGITAL_OUTPUT_ID: // Digital output
        break;
      case RS485_BUS_ID:
		if (value.IO_05_07[card_nr].board_component.ctrl & 0x0020)
	      IO_05_07_RS485_Bus_PDO_Receive(ptr, length, card_nr, component_nr);
        break;                                                   
    }
  }
}

/*************************************************************************************************/
unsigned int IO_05_07_Component_SDO_Master_Transmit(u_SDO_data *ptr, unsigned char card_nr, unsigned char component_id, unsigned char component_nr)
{
  if ((card_nr < IO_05_07_MAX) &&
      (opt_io.IO_05_07[card_nr].board_component.option & 0x000F))
  {
    switch (component_id)
    {
      case BOARD_COMPONENT_ID: // board component
        return (IO_05_07_Board_Component_SDO_Master_Transmit(ptr, card_nr));
      case CLASS_ID: // class component
        switch (component_nr)
        {
          case 1: // voerweger
            break;
        }
        return (0);
      case ANALOG_INPUT_ID: // Analog input
        return (IO_05_07_Analog_Input_SDO_Master_Transmit(ptr, card_nr, component_nr));
      case DIGITAL_INPUT_ID: // Digital input
        return (0);
      case ANALOG_OUTPUT_ID: // Analog output
        return (IO_05_07_Analog_Output_SDO_Master_Transmit(ptr, card_nr, component_nr));
      case DIGITAL_OUTPUT_ID: // Digital output
        return (IO_05_07_Digital_Output_SDO_Master_Transmit(ptr, card_nr, component_nr));
      case RS485_BUS_ID:
	    return (IO_05_07_RS485_Bus_SDO_Master_Transmit(ptr, card_nr, component_nr));
    }
  }
  return (0);
}

/*************************************************************************************************/
void IO_05_07_Component_SDO_Master_Transmit_OK(unsigned char card_nr, unsigned char component_id, unsigned char component_nr)
{
  if ((card_nr < IO_05_07_MAX) &&
      (opt_io.IO_05_07[card_nr].board_component.option & 0x000F))
  {
    switch (component_id) // reset vlaggen dat SDO component goed verzonden is
    {
      case BOARD_COMPONENT_ID: // Alarm relais and board component
        IO_05_07_Board_Component_SDO_Master_Transmit_OK(card_nr);
        break;
      case CLASS_ID: // class component
        switch (component_nr)
        {
          case 1: // voerweger
            break;
        }
        break;
      case ANALOG_INPUT_ID: // Analog input 
	    IO_05_07_Analog_Input_SDO_Master_Transmit_OK(card_nr, component_nr);
        break;
      case DIGITAL_INPUT_ID: // digital input
        break;                                                   
      case ANALOG_OUTPUT_ID: // Analog output 
        IO_05_07_Analog_Output_SDO_Master_Transmit_OK(card_nr, component_nr);
        break;
      case DIGITAL_OUTPUT_ID: // Digital output
        IO_05_07_Digital_Output_SDO_Master_Transmit_OK(card_nr, component_nr);
        break;
      case RS485_BUS_ID:
        IO_05_07_RS485_Bus_SDO_Master_Transmit_OK(card_nr, component_nr);
        break;                                                   
    }
  }
}

/*************************************************************************************************/
// Receive SDO Master functies worden op dit moment nog niet gebruikt
void IO_05_07_Component_SDO_Master_Receive(u_SDO_data *ptr, unsigned char card_nr, unsigned char component_id, unsigned char component_nr)
{
  if ((card_nr < IO_05_07_MAX) &&
      (opt_io.IO_05_07[card_nr].board_component.option & 0x000F))
  {
    switch (component_id)
    {
      case BOARD_COMPONENT_ID: // board component 
        IO_05_07_Board_Component_SDO_Master_Receive(ptr, card_nr);
        break;
      case CLASS_ID: // class component
        switch (component_nr)
        {
          case 1: // voerweger
            break;
        }
        break;
      case ANALOG_INPUT_ID: // Analog input 
	    IO_05_07_Analog_Input_SDO_Master_Receive(ptr,card_nr,component_nr);
        break;
      case DIGITAL_INPUT_ID: // digital input (not used only with SDO)
        break;
      case ANALOG_OUTPUT_ID: // Analog output
        IO_05_07_Analog_Output_SDO_Master_Receive(ptr,card_nr,component_nr);
        break;
      case DIGITAL_OUTPUT_ID: // digital output
        IO_05_07_Digital_Output_SDO_Master_Receive(ptr,card_nr,component_nr);
        break;
      case RS485_BUS_ID:
	    IO_05_07_RS485_Bus_SDO_Master_Receive(ptr,card_nr,component_nr);
        break;
    }
  }
}

void IO_05_07_Init(unsigned char card_nr)
// wordt aan geroepen als can opnieuw geinitialiseerd wordt
{
  if (opt_io.IO_05_07[card_nr].board_component.option & 0x000F)
  {
    IO_05_07_Board_Component_Init(card_nr);
    IO_05_07_Analog_Input_Init(card_nr);
    IO_05_07_Analog_Output_Init(card_nr);
    IO_05_07_Digital_Output_Init(card_nr);
    IO_05_07_RS485_Bus_Init(card_nr);
  }
  IO_05_07_init_switch[card_nr] = 0;
}

void IO_05_07_100ms(unsigned char card_nr)
// wordt elke 100ms aangeroepen om te kijken of er periodieke data verzonden moet worden
{
  if (IO_05_07_Board_Component_100ms(card_nr))
  {
    IO_05_07_Analog_Output_100ms(card_nr);
    IO_05_07_Digital_Output_100ms(card_nr);
  }
} 

/*************************************************************************************************/
unsigned char IO_05_07_ID_CAN_Check_PDO_Component(unsigned char card_nr,
                                                   void (*CAN_PDO_Transmit)(unsigned char priority,
                                                                            unsigned char card_id, unsigned char card_nr, 
                                                                            unsigned char component_id, unsigned char component_nr))
// CAN_PDO_Transmit wordt als parameter doorgegeven zodat deze functie zowel van CAN1 als CAN2 kan zijn
{
static unsigned char component_id[IO_05_07_MAX] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
static unsigned char component_nr[IO_05_07_MAX] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
unsigned char old_component_id = component_id[card_nr];
unsigned char old_component_nr = component_nr[card_nr];

  if (opt_io.IO_05_07[card_nr].board_component.option & 0x000F)
  {
    if (value.IO_05_07[card_nr].board_component.ctrl & 0x0020) // board init?
    {
      do 
      {
        switch (component_id[card_nr])
        {
          case BOARD_COMPONENT_ID: // board component
            if (value.IO_05_07[card_nr].board_component.ctrl & 0x0101)
            {
              CAN_PDO_Transmit(1,IO_05_07_ID,card_nr,BOARD_COMPONENT_ID,0x00); // priotriteit PDO = 1
              return (1);
            }
            component_id[card_nr] = ANALOG_OUTPUT_ID;
            component_nr[card_nr] = 0;
            break;
          case ANALOG_OUTPUT_ID: // analog_output
            if (opt_io.IO_05_07[card_nr].analog_output[component_nr[card_nr]].option & 0x000F)
            {
              if (value.IO_05_07[card_nr].analog_output[component_nr[card_nr]].ctrl & 0x0101)
              {
                CAN_PDO_Transmit(3,IO_05_07_ID,card_nr,ANALOG_OUTPUT_ID,component_nr[card_nr]);
                return (1);
              }
            }
            if (component_nr[card_nr] < IO_05_07_ANALOG_OUTPUT - 1)
              component_nr[card_nr]++;
            else
            {
              component_id[card_nr] = DIGITAL_OUTPUT_ID;
              component_nr[card_nr] = 0;
            }
            break;
          case DIGITAL_OUTPUT_ID: // digital output
            if (opt_io.IO_05_07[card_nr].digital_output.option & 0x000F)
            {  
              if (value.IO_05_07[card_nr].digital_output.ctrl & 0x0101)
              {
                CAN_PDO_Transmit(3,IO_05_07_ID,card_nr,DIGITAL_OUTPUT_ID,0);
                return (1);
              }
            }
            component_id[card_nr] = RS485_BUS_ID;
            component_nr[card_nr] = 0;
            break;
          case RS485_BUS_ID:
            if (opt_io.IO_05_07[card_nr].RS485_bus.Option & 0x000F)
            {
              if (value.IO_05_07[card_nr].RS485_bus.ctrl & 0x0101)
              {
                CAN_PDO_Transmit(5,IO_05_07_ID,card_nr,RS485_BUS_ID,0);
                return (1);
              }
            }
            component_id[card_nr] = BOARD_COMPONENT_ID;
            component_nr[card_nr] = 0;
            break;
          default:
            component_id[card_nr] = BOARD_COMPONENT_ID;
            component_nr[card_nr] = 0;
            break;
        }
      }
      while ((old_component_id != component_id[card_nr]) ||
             (old_component_nr != component_nr[card_nr]));
    }
    else // board not init
    {
      component_id[card_nr] = BOARD_COMPONENT_ID;
      component_nr[card_nr] = 0;
      if (value.IO_05_07[card_nr].board_component.ctrl & 0x0101)
      {
        CAN_PDO_Transmit(1,IO_05_07_ID,card_nr,BOARD_COMPONENT_ID,0x00);
        return (1);
      }    
    }
  }
  else
  {
    component_id[card_nr] = 0;
    component_nr[card_nr] = 0;
  }
  return (0);
}

unsigned char IO_05_07_CAN_Check_PDO_Component(void (*CAN_PDO_Transmit)(unsigned char priority,
                                                                         unsigned char card_id, unsigned char card_nr, 
                                                                         unsigned char component_id, unsigned char component_nr))
{
static unsigned char nr = 0;
unsigned char old_nr = nr;

   do
   {
     if (IO_05_07_ID_CAN_Check_PDO_Component(nr, CAN_PDO_Transmit))
     {
       nr++;
       nr %= IO_05_07_MAX;
       return (1);
     }
     nr++;
     nr %= IO_05_07_MAX;
   }
   while (old_nr != nr);
   return (0);
}

unsigned char IO_05_07_ID_CAN_Check_SDO_Master_Component(unsigned char card_nr,
                                                          void (*CAN_SDO_Master_Transmit)(unsigned char priority,
                                                                                          unsigned char card_id, unsigned char card_nr, 
                                                                                          unsigned char component_id, unsigned char component_nr),
                                                          void (*CAN_SDO_Master_Receive)(unsigned char priority,
                                                                                         unsigned char card_id, unsigned char card_nr, 
                                                                                         unsigned char component_id, unsigned char component_nr))
// CAN_SDO_Master_Transmit wordt als parameter doorgegeven zodat deze functie zowel van CAN1 als CAN2 kan zijn
// CAN_SDO_Master_Receive wordt als parameter doorgegeven zodat deze functie zowel van CAN1 als CAN2 kan zijn
{
static unsigned char component_id[IO_05_07_MAX] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
static unsigned char component_nr[IO_05_07_MAX] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
unsigned char old_component_id = component_id[card_nr];
unsigned char old_component_nr = component_nr[card_nr];

  if ((opt_io.IO_05_07[card_nr].board_component.option & 0x000F) && // board used
      (value.IO_05_07[card_nr].board_component.watchdog_update_timer) && // SDO pas zenden als watchdog timer bij IO bord gevuld is
      (value.IO_05_07[card_nr].board_component.ctrl & 0x0010)) // comm ok?
  {
    do 
    {
      switch (component_id[card_nr])
      {
        case BOARD_COMPONENT_ID: // board component
          if (value.IO_05_07[card_nr].board_component.ctrl & 0x0004)
          {
            CAN_SDO_Master_Transmit(2,IO_05_07_ID,card_nr,BOARD_COMPONENT_ID,0x00); // prioriteit SDO = 2
            return (1);
          }
          else if (value.IO_05_07[card_nr].board_component.ctrl & 0x0008)
          {
            CAN_SDO_Master_Receive(5,IO_05_07_ID,card_nr,BOARD_COMPONENT_ID,0x00);
            return (1);
          }
          component_id[card_nr] = ANALOG_INPUT_ID;
          component_nr[card_nr] = 0;
          break;
        case ANALOG_INPUT_ID:
          if (value.IO_05_07[card_nr].analog_input[component_nr[card_nr]].ctrl & 0x0004)
          {
            CAN_SDO_Master_Transmit(5,IO_05_07_ID,card_nr,ANALOG_INPUT_ID,component_nr[card_nr]);
            return (1);
          }
          else if (value.IO_05_07[card_nr].analog_input[component_nr[card_nr]].ctrl & 0x0008)
          {
            CAN_SDO_Master_Receive(5,IO_05_07_ID,card_nr,ANALOG_INPUT_ID,component_nr[card_nr]);
            return (1);
          }
          if (component_nr[card_nr] < IO_05_07_ANALOG_INPUT - 1)
            component_nr[card_nr]++;
          else
          {
            component_id[card_nr] = ANALOG_OUTPUT_ID;
            component_nr[card_nr] = 0;
          }
          break;
        case ANALOG_OUTPUT_ID: // analog_output
          if (value.IO_05_07[card_nr].analog_output[component_nr[card_nr]].ctrl & 0x0004)
          {
            CAN_SDO_Master_Transmit(5,IO_05_07_ID,card_nr,ANALOG_OUTPUT_ID,component_nr[card_nr]);
            return (1);
          }
          else if (value.IO_05_07[card_nr].analog_output[component_nr[card_nr]].ctrl & 0x0008)
          {
            CAN_SDO_Master_Receive(5,IO_05_07_ID,card_nr,ANALOG_OUTPUT_ID,component_nr[card_nr]);
           return (1);
          }
          if (component_nr[card_nr] < IO_05_07_ANALOG_OUTPUT - 1)
            component_nr[card_nr]++;
          else
          {
            component_id[card_nr] = DIGITAL_OUTPUT_ID;
            component_nr[card_nr] = 0;
          }
          break;
        case DIGITAL_OUTPUT_ID: // digital output
          if (value.IO_05_07[card_nr].digital_output.ctrl & 0x0004)
          {
            CAN_SDO_Master_Transmit(5,IO_05_07_ID,card_nr,DIGITAL_OUTPUT_ID,0);
            return (1);
          }
          else if (value.IO_05_07[card_nr].digital_output.ctrl & 0x0008)
          {
            CAN_SDO_Master_Receive(5,IO_05_07_ID,card_nr,DIGITAL_OUTPUT_ID,0);
            return (1);
          }
          component_id[card_nr] = RS485_BUS_ID;
          component_nr[card_nr] = 0;
          break;
        case RS485_BUS_ID: // analog_output
          if (value.IO_05_07[card_nr].RS485_bus.ctrl & 0x0004)
          {
            CAN_SDO_Master_Transmit(5,IO_05_07_ID,card_nr,RS485_BUS_ID,0);
            return (1);
          }
          else if (value.IO_05_07[card_nr].RS485_bus.ctrl & 0x0008)
          {
            CAN_SDO_Master_Receive(5,IO_05_07_ID,card_nr,RS485_BUS_ID,component_nr[card_nr]);
            return (1);
          }
          component_id[card_nr] = BOARD_COMPONENT_ID;
          component_nr[card_nr] = 0;
          break;
        default:
          component_id[card_nr] = BOARD_COMPONENT_ID;
          component_nr[card_nr] = 0;
          break;
      }
    }
    while ((old_component_id != component_id[card_nr]) ||
           (old_component_nr != component_nr[card_nr]));
    // als communicatie in orde maar board not initialized then send board SDO
    if ((value.IO_05_07[card_nr].board_component.ctrl & 0x0010) &&
        !(value.IO_05_07[card_nr].board_component.ctrl & 0x0020)/* &&
        (value.IO_20_33P[card_nr].board_component.version_hardware >= VERSION_HARDWARE) &&
        (value.IO_20_33P[card_nr].board_component.version_software >= VERSION_SOFTWARE)*/) // board init?
    {
      CAN_SDO_Master_Transmit(2,IO_05_07_ID,card_nr,BOARD_COMPONENT_ID,0x00);
      return (1);
    }
  }
  else
  {
    component_id[card_nr] = BOARD_COMPONENT_ID;
    component_nr[card_nr] = 0;
  }
  return (0);
}

unsigned char IO_05_07_CAN_Check_SDO_Master_Component(void (*CAN_SDO_Master_Transmit)(unsigned char priority,
                                                                                unsigned char card_id, unsigned char card_nr, 
                                                                                unsigned char component_id, unsigned char component_nr),
                                                       void (*CAN_SDO_Master_Receive)(unsigned char priority,
                                                                                      unsigned char card_id, unsigned char card_nr, 
                                                                                      unsigned char component_id, unsigned char component_nr))
{
static unsigned char nr = 0;
unsigned char old_nr = nr;

   do
   {
     if (IO_05_07_ID_CAN_Check_SDO_Master_Component(nr, CAN_SDO_Master_Transmit, CAN_SDO_Master_Receive))
     {
       nr++;
       nr %= IO_05_07_MAX;
       return (1);
     }
     nr++;
     nr %= IO_05_07_MAX;
   }
   while (old_nr != nr);
   return (0);
}
