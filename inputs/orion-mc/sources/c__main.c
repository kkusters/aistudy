// C__MAIN.CPP

#include "ch_define.h"

#include "ch_alarm.h"
#include "ch_asc0.h"
#include "ch_asc1.h"
#include "ch_cabriokas.h"
#include "ch_can_io.h"
#include "ch_can_backbone_appl.h"
#include "ch_can_backbone_pc.h"
#include "ch_disp.h"
#include "ch_disp_alarm_0.h"
#include "ch_disp_func.h"
#include "ch_disp_option_1.h"
#include "ch_disp_opt_IO_Select.h"
#include "ch_disp_password.h"
#include "ch_disp_F1_0.h"
#include "ch_disp_start.h"
#include "ch_eep.h"
#include "ch_eep_taal.h"
#include "ch_ethernet.h"
#include "ch_ethernet_app.h"
#include "ch_event.h"
#include "ch_htrap.h"
#include "ch_key.h"
#include "ch_kiersturing.h"
#include "ch_lcd_bitmap.h"
#include "ch_lcd_hardware.h"
#include "ch_lcd_pixel.h"
#include "ch_luchtmengkast.h"
#include "ch_device.h"
#include "ch_mb_device.h"
#include "ch_motor.h"
#include "ch_motorgroep.h"
#include "ch_rs232_0.h"
#include "ch_rs232_1.h"
#include "ch_rtc.h"
#include "ch_sd.h"
#include "ch_sensoren.h"
#include "ch_string.h"
#include "ch_test.h" // TD
//#include "ch_test_log.h" // TD
#include "ch_tijd.h"
#include "ch_timer.h"
#include "ch_wdi.h"
#include "NetworkDevice.h"
#include "ch_main.h"

#include "co_nmt.h"
#include "ch_bacnet_server.h"

unsigned char second_switch;
unsigned char comm_blink_ico;

void ModuleCheck(void)
{
unsigned char ethernet_module_old     = opt_alg.ethernet_module; // gebruik van ethernet is mogelijk
unsigned char sd_module_old           = opt_alg.sd_module;       // gebruik van sd kaart is mogelijk
unsigned char OldModuleCANopen        = module.CanOpen;
unsigned char OldModuleVentilatie     = module.Ventilatie;
unsigned char OldModuleLuchtmengkast  = module.Luchtmengkast;
unsigned char OldModuleBACnet         = module.BACnet;
unsigned char OldModuleSchakelgroepen = module.Schakelgroepen;
unsigned char OldModuleKlep           = module.Klep;
unsigned char OldModuleDrukverschil   = module.Drukverschil;
unsigned char OldModuleWatchdogMode   = module.WatchdogMode;
unsigned char OldModuleHoogendoorn    = module.Hoogendoorn;
unsigned char OptionsChanged          = 0;
int i;

  if ((module.serie_number <= 0 || module.serie_number > 0x7FFFFFFF) && tijd.tm_year >= 2010)
  {
	module.serie_number = time( NULL );
	vlag_module_write = 1;
  }
  opt_alg.ethernet_module = 1;
  opt_alg.sd_module       = 1;

  // CANopen
  if (module.type & MODULE_CANOPEN)
  {
    module.CanOpen = 1;
    opt_alg.CANopenPossible = 1;
  }
  else
  {
    module.CanOpen = 0;
    opt_alg.CANopenPossible = 0;
    opt_alg.can_backbone    = 0;
  }

  // Hoogendoorn module
  if (module.type & MODULE_HOOGENDOORN)
  {
    module.Hoogendoorn = 1;
	opt_alg.hoogendoorn_possible = opt_alg.ethernet_enabled;
  }
  else
  {
    module.Hoogendoorn = 0;
	opt_alg.hoogendoorn_possible = 0;
	opt_alg.hoogendoorn_enabled = 0;
  }

  // Ventilation
  if (module.type & MODULE_VENTILATIE)
  {
    module.Ventilatie = 1;
  }
  else
  {
    module.Ventilatie = 0;
    for (i = 0; i < MAX_GROUP; i++)
	{
	  if (opt_app.Motorgroup[i].Type == TYPE_VENT)
	    opt_app.Motorgroup[i].Enabled = 0;
	}
  }

  // Luchtmengkast
  if (module.type & MODULE_LUCHTMENGKAST)
    module.Luchtmengkast = 1;
  else
    module.Luchtmengkast = 0;

  // BACnet
  if (module.type & MODULE_BACNET)
    module.BACnet = 1;
  else
    module.BACnet = 0;

  // Schakelgroepen
  if (module.type & MODULE_SCHAKELGROEPEN)
  {
    module.Schakelgroepen = 1;
  }
  else
  {
    module.Schakelgroepen = 0;
	opt_app.VrijgaveVent = default_opt_app.VrijgaveVent;
  }

  // Klep
  if (module.type & MODULE_KLEP)
  {
    module.Klep = 1;
  }
  else
  {
    module.Klep = 0;
    for (i = 0; i < MAX_GROUP; i++)
	{
	  if (opt_app.Motorgroup[i].Type == TYPE_KLEP)
	    opt_app.Motorgroup[i].Enabled = 0;
	}
  }

  // Drukverschil
  if (module.type & MODULE_DRUKVERSCHIL)
  {
    module.Drukverschil = 1;
  }
  else
  {
    module.Drukverschil = 0;
	opt_app.Sensoren = default_opt_app.Sensoren;
  }

  if (module.type & MODULE_WATCHDOGMODE)
    module.WatchdogMode = 1;
  else
    module.WatchdogMode = 0;

  // Ethernet
  if (opt_alg.ethernet_module == 0)
  {
    opt_alg.ethernet_enabled = 0;
	opt_alg.BACnet_possible = 0;
	if (opt_alg.BACnet_enabled)
    {
      opt_alg.BACnet_enabled = 0;
      Bacnet_Cleanup();
    }
  }
  else
  {
    if (module.BACnet && opt_alg.ethernet_enabled)
    {	
	  opt_alg.BACnet_possible = 1;
    }
    else
    {
	  opt_alg.BACnet_possible = 0;  
	  if (opt_alg.BACnet_enabled)
      {
        opt_alg.BACnet_enabled = 0;
        Bacnet_Cleanup();
      }
	}
  }

  if ((OldModuleCANopen        != module.CanOpen) ||
      (OldModuleVentilatie     != module.Ventilatie) ||
	  (OldModuleLuchtmengkast  != module.Luchtmengkast) ||
	  (OldModuleBACnet         != module.BACnet) ||
	  (OldModuleSchakelgroepen != module.Schakelgroepen) ||
      (OldModuleKlep           != module.Klep) ||
      (OldModuleDrukverschil   != module.Drukverschil) ||
      (OldModuleWatchdogMode   != module.WatchdogMode) ||
      (ethernet_module_old != opt_alg.ethernet_module) ||
	  (OldModuleHoogendoorn    != module.Hoogendoorn) ||
      (sd_module_old != opt_alg.sd_module))
  {
    can_backbone_appl_init_switch = 1;
    CheckOptions();
  }
}


//-----------------------------------------------------------------------------
#pragma class HB=LAST_BYTE_256KB_RAM
#pragma noclear
volatile unsigned int huge controleer_geheugen_256kbyte;
#pragma clear
#pragma default_attributes

#pragma class HB=LAST_BYTE_1MB_RAM
#pragma noclear
volatile unsigned int huge controleer_geheugen_1Mbyte;
#pragma clear
#pragma default_attributes

unsigned char Controleer_Ram_Geheugen_Grote(void)
// return 0 als geheugen maar 256kb
// return 1 als geheugen 1MB (in orde)
{
  controleer_geheugen_256kbyte = 0;
  controleer_geheugen_1Mbyte = 0x1234;
  if (controleer_geheugen_256kbyte != 0)
    return (0);
  else
    return (1);
}

//-----------------------------------------------------------------------------
static void DisplayStartupWarning(int code)
{
  Copy_Strings();
  alarm_disp_act.code    = code;
  alarm_disp_act.index   = 0;
  alarm_disp_act.value   = 0;
  alarm_disp_act.hard    = HARD_ALARM;
  alarm_disp_act.state   = AL_HARD;
  alarm_disp_act.pc_type = ALARM_VAL_0;
  If_Exist_Goto_Screen_Alarm_0();
  lcd_refresh_fast_switch = 1;
  while (1)
  {  
    WDI_Trigger();
    Timer_Control(); // Place this after RTC
    LCD_Control();
    if (Timer_125ms_Check_Flag())
    {
      if (Timer_500ms_Check_Flag())
      {
        cursor_state = !cursor_state;
        alarm_blink_ico = (cursor_state) ? 6 : 0;
        lcd_refresh_slow_switch = 1;
      }
    }  
  }  
}

//-----------------------------------------------------------------------------
void Main_Init(void)
{
  EXISEL0 = 0x0000;  // load the external Interrupt  select register
  EXISEL1 = 0x0000;  // load the external Interrupt  select register
  EXICON = 0x0000;   // load the external Interrupt control register
  POCON20 &= 0x0F00;
  IEN = 1;           // enable all interrupts
  WDI_Init();

  Default_Strings();
  WDI_Trigger();

//  pwr_down_while_write_eep_flag = CheckWriteEEP();
  Hardware_Trap_Init();
//  Tijd_Init();
//  RTC_Init(); // must be placed after I2C0_Init(); 
  if (Controleer_Ram_Geheugen_Grote() == 1)
  {
    Key_Init(); 
    RTC_Read();
    timer_init_switch = 1;
    Timer_Control(); // Place this after RTC
    lcd_init_switch = 1;
    LCD_Control(); 

    while (RestoreDataAreas()) // must be placed after RTC_Init
    {
    }
    //  idchip  = IDCHIP;
    //  idmanuf = IDMANUF;
    //  idmem   = IDMEM;
    //  idmem2  = IDMEM2;
    //  idprog  = IDPROG;
    WDI_Trigger();
    Copy_Strings(); // zet tekst strings op default straks verwijderen en alleen op default zetten bij batery low en ander programma
    WDI_Trigger();
    RS232_0_Init();
    RS232_1_Init();
    ASC0_Control();
    ASC1_Control();

    CAN_IO_Board_Init();
    CAN_IO_Init();
    Can_Backbone_Appl_Init();

    #ifdef SD_CARD
    SD_Control();
    #endif // SD_CARD
  }
  else
  {
    timer_init_switch = 1;
    Timer_Control(); // Place this after RTC
    lcd_init_switch = 1;
    LCD_Control();
    
    DisplayStartupWarning(SYSTEEM_GEHEUGEN_256K_AL); 
  }
  WDI_Trigger();
}

//-----------------------------------------------------------------------------
void Second(void)
{
static unsigned char loop = 0;

  switch (loop)
  {
    case  0:
      if (alarm_switch)
        Alarm_Proc();
      loop++;
      break;
    case  1:
      if (uur_switch)
        Uur_Control();
      loop++;
      break;
    case  2:
      if (dag_switch)
        Dag_Control();
      loop++;
      break;
    case  3:
      if (einde_dag_switch)
        Einde_Dag_Control();
      loop++;
      break;
    case  4:
      #ifdef SD_CARD
      SD_Second_Control(); // TD: 8-1-2008
      #endif // SD_CARD
      loop++;
      break;
    case  5:
      if (screen_catcher_time_out)
        screen_catcher_time_out--;
      if (restore_data_busy)
        restore_data_busy--;
	  #ifdef SD_CARD
      if (read_data_file_busy)
	    read_data_file_busy--;
	  #endif // SD_CARD
      loop++;
      break;
    case  6:
//    loop++;
//    break;
    default:
      second_switch = 0;
      loop = 0;
      break;
  } 
}

//-----------------------------------------------------------------------------
void BuildLinkedList(void)
{
TMotor  *pMotor;
TDevice *pDevice;
int i, index;

  // Reset all pointers
  for (i = 0; i < MAX_GROUP; i++)
  {
    Motorgroup[i].FirstMotor = NULL;
    Motorgroup[i].LastMotor  = NULL;
    Motorgroup[i].Link       = NULL;
  }
  // Build motor list
  for (i = 0; i < MAX_MOTOR; i++)
  {
    if (opt_app.Motor[i].Enabled)
    {
      index = opt_app.Motor[i].GroupNumber;
      index--;
      if (index != -1)
      {
        pMotor = Motorgroup[index].LastMotor;
        if (pMotor == NULL)
        {
          Motor[i].Prev = NULL;
          Motor[i].Next = NULL;
          Motorgroup[index].FirstMotor = &Motor[i];
          Motorgroup[index].LastMotor  = &Motor[i];
        }
        else
        {
          pMotor->Next  = &Motor[i];
          Motor[i].Prev = pMotor;
          Motor[i].Next = NULL;
          Motorgroup[index].LastMotor = &Motor[i];
        }
        Motor[i].PulseSystem.Enabled   = opt_app.Motorgroup[index].PulseSystem;
        Motor[i].PulseSystem.Setpoints = &setp_alg.Motorgroup[index].PulseSystem;
      }
      if (opt_app.Motor[i].Link != -1)
        Motor[i].Link = &Motor[opt_app.Motor[i].Link];
      else
        Motor[i].Link = NULL;
    }
    else
    {
      Motor[i].Prev = NULL;
      Motor[i].Next = NULL;
      Motor[i].Link = NULL;
    }
  }
  // Build connections between groups
  for (i = 0; i < MAX_SCREEN; i++)
  {
    if (opt_app.DualScreen[i].Enabled)
    {
      if ((opt_app.DualScreen[i].GroupA != -1) && (opt_app.DualScreen[i].GroupB != -1))
      {
        Motorgroup[opt_app.DualScreen[i].GroupA].Link = &Motorgroup[opt_app.DualScreen[i].GroupB];
        Motorgroup[opt_app.DualScreen[i].GroupB].Link = &Motorgroup[opt_app.DualScreen[i].GroupA];
      }
    }
  }
  for (i = 0; i < MAX_CABRIO; i++)
  {
    if (opt_app.Cabriokas[i].Enabled)
    {
      if ((opt_app.Cabriokas[i].GroupA != -1) && (opt_app.Cabriokas[i].GroupB != -1))
      {
        Motorgroup[opt_app.Cabriokas[i].GroupA].Link = &Motorgroup[opt_app.Cabriokas[i].GroupB];
        Motorgroup[opt_app.Cabriokas[i].GroupB].Link = &Motorgroup[opt_app.Cabriokas[i].GroupA];
      }
    }
  }
  // Build device list
  for (i = 0; i < MAX_DEVICE; i++)
  {
    if (opt_app.Device[i].GroupNumber > 0)
    {
      index = opt_app.Device[i].GroupNumber - 1;
      pDevice = Motorgroup[index].LastMotor;
      if (pDevice == NULL)
      {
        Device[i].Prev = NULL;
        Device[i].Next = NULL;
        Motorgroup[index].FirstMotor = &Device[i];
        Motorgroup[index].LastMotor  = &Device[i];
      }
      else
      {
        pDevice->Next  = &Device[i];
        Device[i].Prev = pDevice;
        Device[i].Next = NULL;
        Motorgroup[index].LastMotor = &Device[i];
      }
    }
    else
    {
      Device[i].Prev = NULL;
      Device[i].Next = NULL;
    }
  }
  mbDeviceInit();
}

//-----------------------------------------------------------------------------
void CreateObjects(void)
{
int i;

  // Motorgroup object
  for (i = 0; i < MAX_GROUP; i++)
    CreateMotorgroup(&Motorgroup[i], i);
  // Motor object
  for (i = 0; i < MAX_MOTOR; i++)
    CreateMotor(&Motor[i], i);
  // Device object
  for (i = 0; i < MAX_DEVICE; i++)
    CreateDevice(&Device[i], i);
  // DualScreen object
  for (i = 0; i < MAX_SCREEN; i++)
    CreateDualScreen(&DualScreen[i], i);
  // Cabriokas object
  for (i = 0; i < MAX_CABRIO; i++)
    CreateCabriokas(&Cabriokas[i], i);
  // Linked list
  BuildLinkedList();
}

//-----------------------------------------------------------------------------
#ifdef SD_CARD
void LogTestData(void)
{
/*
static int OldData[14] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
int DataChanged = 0;
TMotorgroup *pGroupA;
TMotor *pMotorA;
s_board_IO_on_off *IO_A;

  pGroupA = &Motorgroup[2];
  pMotorA = pGroupA->FirstMotor;
  IO_A = &pMotorA->Option->IO;

  if (pGroupA->Value->PositionPerc != OldData[0])
    DataChanged = 1;
  else if (pMotorA->Value->PositionAuto != OldData[4])
    DataChanged = 1;
  else if (pMotorA->Value->Position != OldData[6])
    DataChanged = 1;
  else if (val_hr_alg.IO_H2MC[IO_A->board_nr].motor_control[IO_A->IO_nr].Ctrl != OldData[8])
    DataChanged = 1;
  else if (val_hr_alg.IO_H2MC[IO_A->board_nr].motor_control[IO_A->IO_nr].Status != OldData[9])
    DataChanged = 1;
  else if (val_hr_alg.IO_H2MC[IO_A->board_nr].motor_control[IO_A->IO_nr].Status2 != OldData[10])
    DataChanged = 1;
  
  if (DataChanged)
  {
    File_Printf(LOG_FILE, "%02i:%02i:%02i %02i-%02i-%04i %3i %3i %3i %3i %i %i %i\r\n",
                tijd.tm_hour, tijd.tm_min, tijd.tm_sec, tijd.tm_mday, tijd.tm_mon, tijd.tm_year,
                pGroupA->Value->PositionPerc, val_hr_alg.DualScreen[0].PositionGroupA, pMotorA->Value->PositionAuto, pMotorA->Value->Position,
                val_hr_alg.IO_H2MC[IO_A->board_nr].motor_control[IO_A->IO_nr].Ctrl, val_hr_alg.IO_H2MC[IO_A->board_nr].motor_control[IO_A->IO_nr].Status, val_hr_alg.IO_H2MC[IO_A->board_nr].motor_control[IO_A->IO_nr].Status2);
  }

  OldData[ 0] = pGroupA->Value->PositionPerc;
  OldData[ 4] = pMotorA->Value->PositionAuto;
  OldData[ 6] = pMotorA->Value->Position;
  OldData[ 8] = val_hr_alg.IO_H2MC[IO_A->board_nr].motor_control[IO_A->IO_nr].Ctrl;
  OldData[ 9] = val_hr_alg.IO_H2MC[IO_A->board_nr].motor_control[IO_A->IO_nr].Status;
  OldData[10] = val_hr_alg.IO_H2MC[IO_A->board_nr].motor_control[IO_A->IO_nr].Status2;
*/
}
#endif // SD_CARD

//-----------------------------------------------------------------------------
void Communicatie_Control(void)
{
static int fase = 0;

  switch (fase)
  { 
    default: fase = 0;
    case 0: RS232_0_Control(); fase++; break;
    case 1: RS232_1_Control(); fase++; break;
    #ifdef ETHERNET
    case 2: Ethernet_Control_Port(0); fase++; break;
    case 3: Ethernet_Control_Port(1); fase++; break;
    case 4: Ethernet_Control_Port(2); fase++; break;
    case 5: Ethernet_Control_Port(3); fase = 0; break;
    #endif // ETHERNET_CONTROL
  }  
}

void main(void)
{
static int GroupIndex = 0;
static int MotorIndex = 0;
int i;

  Main_Init();
  Alarm_Control_Data_Algemeen();
  Alarm_Log_Opstart_Algemeen();
  Init_All_Screen();
  Password_Init();
  ModuleCheck();

  WDI_Trigger();

  mbDeviceInit();

  CreateObjects();
  Event_Init();

  LuchtmengkastInit();
  InitSensoren();

  CheckOptions();

  if (opt_alg.BACnet_enabled)
    Bacnet_Init();

  while (1)
  {
    WDI_Trigger();
//    TEST_BIT_TOGGLE();
    Timer_Control(); // in plaats van Timer_1ms_Check()
    if (tijd_init_switch)
    {
//      Tijd_Init();
      RTC_read_switch = 1;
    }

    if (RTC_read_switch)
      RTC_Read();
    if (RTC_write_switch)
      RTC_Write();

    EEP_Taal_Proc();

    Communicatie_Control();
    ASC0_Control();
    ASC1_Control();

    #ifdef ETHERNET
    if (opt_alg.ethernet_enabled)
      Main_NetworkDevice();
    else
      Reset_NetworkDevice();  
  //Ethernet_Control();
    #endif // ETHERNET

	if (opt_alg.BACnet_enabled)
	  Main_Bacnet_Server();

    Key_Control(); // nieuw in V3

    if (comp_ram_eep_switch)
      comp_ram_eep(); // check if data part is changed, if so then write it to eeprom

    CAN_IO_Proc();
    Can_Backbone_Appl_Control();
    #ifndef CANopen
    Can_Backbone_PC_Control();
    #endif // CANopen

    #ifdef SD_CARD
    SD_Control();
    #endif // SD_CARD

    mbDeviceMain();

    LCD_Control();

    if (second_switch)
      Second();

    LuchtmengkastMain();
	MainSensoren();

    // MotorgroupControl
    for (GroupIndex = 0; GroupIndex < MAX_GROUP; GroupIndex++)
      ControlMotorgroup(&Motorgroup[GroupIndex]);

    // MotorControl
    if (MotorIndex >= MAX_MOTOR)
      MotorIndex = 0;
    i = MotorIndex;
    while ((i < MotorIndex + 16) && (i < MAX_MOTOR))
    {
      ControlMotor(&Motor[i]);
      i++;
    }
    MotorIndex = i;

    if (Timer_125ms_Check_Flag())
    {
//    if (comp_ram_eep_switch) JP 23-08-07 kijken of deze 2 regels buiten de 125 ms check kunnen blijven staan
//      comp_ram_eep(); // check if data part is changed, if so then write it to eeprom

      Display_Key_Proc(); // verwerken toetsen en aanpassen display
      #ifdef ASC1_CAN_IO
      CAN_IO_Test();
      #endif // ASC1_CAN_IO
      CAN_IO_100ms();

      #ifdef CANopen
      Can_Backbone_Diagnose_Msg();
      #else // CANopen
      Can_Backbone_Appl_Timing_Control();
      #endif // CANopen

      #ifdef ETHERNET
      Ethernet_Timing_Control();
      #endif // ETHERNET

      RS232_0_125ms_Time_Out(); 
      RS232_1_125ms_Time_Out(); 
      #ifdef ETHERNET
      Ethernet_125ms_Time_Out();
      #endif // ETHERNET
      ASC0_Timing_Control();
      ASC1_Timing_Control();

      if (Timer_500ms_Check_Flag())
      {
        cursor_state = !cursor_state;
        switch (alarm_disp_ico)
        {
          case 0: alarm_blink_ico = 2; break; // geen alarm
          case 1: alarm_blink_ico = (cursor_state) ? 6 : 0; break; // alarm hard
          case 2: alarm_blink_ico = (cursor_state) ? 6 : 1; break; // alarm hard meerdere
          case 3: alarm_blink_ico = (cursor_state) ? 6 : 2; break; // alarm zacht
          case 4: alarm_blink_ico = (cursor_state) ? 6 : 3; break; // alarm zacht meerdere
          case 5: alarm_blink_ico = (cursor_state) ? 6 : 4; break; // alarm onderdrukken
          case 6: alarm_blink_ico = (cursor_state) ? 6 : 5; break; // alarm onderdrukken meerdere
        }
        lcd_refresh_slow_switch = 1;
        if (opt_alg.can_backbone)
        {
          switch (getNodeState())
          {
            case OPERATIONAL:
              comm_blink_ico = 0;
              break;
            default:
              comm_blink_ico ^= 1;
              break;
          }
        }
        else
        {
          comm_blink_ico = 0;
        }
        if (Tijd_Check_Flag_1s())
        {
          #ifdef SD_CARD
          LogTestData();
          #endif // SD_CARD
          if ((unsigned long)screen_ptr == (unsigned long)&screen_start)
          {
            if (start_delay)
              start_delay--;
            else
              Prev_Screen();
          }
          if (block_display_cursor_block)
            block_display_cursor_block--;
          alarm_switch = 1;
          second_switch = 1;
          if (can_io_init_switch)
            CAN_IO_Init();
          if (Tijd_Check_Flag_1min())
          {
            Password_Control();
            if (lcd_dimmen_cnt)
              lcd_dimmen_cnt--;
            comp_ram_eep_switch = 1; 
            if (Tijd_Check_Flag_1hour())
            {
              // commands every hour
              uur_switch = 1;
              if (Tijd_Check_Flag_1day())
              {
                dag_switch = 1;
              }
            }
          }
        }
      } 
    }
  }
}
