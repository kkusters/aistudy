// CH_NICOTRA_GEBHARDT.H

#ifndef _CH_NICOTRA_GEBHARDT
#define _CH_NICOTRA_GEBHARDT

//==============================================================================
//------------------ Nicotra Gebhardt EC-ventilatoren - modBus -----------------
//==============================================================================
typedef enum
{ 
  rbNicoGebMinRpm = 1,     
  rbNicoGebMaxRpm = 2
} TNicotraGebhardtReadHolding;

typedef enum
{ 
  rbNicoGebReset        = 0,  // TODO KK
  rbNicoGebInputType    = 34,
  rbNicoGebModbusAdress = 45, 
  rbNicoGebSpeed        = 66
} TNicotraGebhardtWriteHolding;

typedef enum
{ 
  rbNicoGebVersion            = 0,
  rbNicoGebSpeedMeasuredSpeed = 3,
  rbNicoGebAlarm1             = 10,
  rbNicoGebMotorCurrent       = 12,
  rbNicoGebMotorVoltage       = 13,
  rbNicoGebInternalTemp       = 15,
  rbNicoGebAlarm2             = 17,
  rbNicoGebMeasuredPower      = 31
} TNicotraGebhardtReadInputRegisters;

typedef enum
{
  nicoGebNoError           = 0,
  nicoGebMemoryError       = 1,
  nicoGebShortCircuit      = 2,
  nicoGebLossOfSynchronism = 3
} TNicotraGebhardtAlarmCodesRegister10;

typedef enum
{
  nicoGebInputVoltageOutsideRange = 1,
  nicoGebBusOverVoltage           = 32,
  nicoGebBusUnderVoltage          = 33,
  nicoGebInputRelayNotClosed      = 34,
  nicoGebMissingPhaseU            = 49,
  nicoGebMissingPhaseV            = 50,
  nicoGebMissingPhaseW            = 51,
  nicoGebHighStartingCurrent      = 52,
  nicoGebOvertemperature          = 113,
  nicoGebLossOfCommunication      = 255
} TNicotraGebhardtAlarmCodesRegister17;

//==============================================================================
//------------------ Nicotra Gebhardt EC-ventilatoren - Control ----------------
//==============================================================================
unsigned char NicotraGebhardtRequestChangeAddress(unsigned char DeviceAddress, unsigned char NewAddress, s_board_IO_on_off *IO, unsigned char IO_size, void (*CompletedFunc)(unsigned char Failure));

unsigned char NicotraGebhardtCreateAlarm(unsigned char DeviceAddress, int FunctionCode, int Index, int Value, int Group);
unsigned char NicotraGebhardtAlarmActive(unsigned char DeviceAddress);
unsigned char NicotraGebhardtClearAllAlarms(unsigned char DeviceAddress, int FunctionCode, int Index);

//==============================================================================
//------------------ Nicotra Gebhardt EC-ventilatoren - Main routines ----------
//==============================================================================
//------------------------------------------------------------------------------
unsigned char NicotraGebhardtMain(int DeviceAddress);

#endif