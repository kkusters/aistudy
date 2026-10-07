// CH_ROSENBERG_GEN_3.H

#ifndef _CH_ROSENBERG_GEN_3_H
#define _CH_ROSENBERG_GEN_3_H

//==============================================================================
//------------------------ Rosenberg EC-ventilatoren GEN 3 - modBus ------------
//==============================================================================
typedef enum
{ 
  rbGen3MinRpm = 1,     
  rbGen3MaxRpm = 2
} TRosenbergGen3ReadHolding;

typedef enum
{ 
  rbGen3VersionNumber       = 1,
  rbGen3Speed               = 4,
  rbGen3InternalTemperature = 5,
  rbGen3PowerIn             = 8,
  rbGen3OperationMinute     = 10,
  rbGen3OperationDay        = 11,
  rbGen3PowerConsumptionKilowattHour  = 32,
  rbGen3PowerConsumptionMegawattHour  = 33
} TRosenbergGen3ReadInputRegisters;

typedef enum
{ 
  rbGen3MotorOnOff  = 0,
  rbGen3ResetAlarm  = 1,
  rbGen3Rotation    = 5,
  rbGen3ControlMode = 7 
} TRosenbergGen3WriteCoil;

typedef enum
{ 
  rbGen3Setpoint          = 0,
  rbGen3ModbusAdress      = 16, 
  rbGen3CommunicationRate = 22
} TRosenbergGen3WriteRegisters;

typedef union
{
  struct
  {
    unsigned int error;
    unsigned int warning;
  } code;

  struct
  {
    unsigned underVoltage              : 1;
    unsigned overVoltage               : 1;
    unsigned overcurrentMotor          : 1;
    unsigned overtemperatureElectronic : 1;
    unsigned inputPhaseMissing         : 1;
    unsigned blockedRotor              : 1;
    unsigned bit_6                     : 1;
    unsigned bit_7                     : 1;	
       
    unsigned wrongRotationSense        : 1;
    unsigned bit_9                     : 1;
    unsigned internalStop              : 1;
    unsigned bit_11                    : 1;
    unsigned bit_12                    : 1;
    unsigned motorPhaseMissing         : 1;
    unsigned bit_14                    : 1;
    unsigned bit_15                    : 1;
    
    unsigned bit_16                    : 1;
    unsigned bit_17                    : 1;
    unsigned _24VSupplyOverloaded      : 1;
    unsigned bit_19                    : 1;
    unsigned bit_20                    : 1;
    unsigned bit_21                    : 1;
    unsigned bit_22                    : 1;
    unsigned overtemperatureMotor      : 1;
    
    unsigned bit_24                    : 1;
    unsigned bit_25                    : 1;
    unsigned bit_26                    : 1;
    unsigned bit_27                    : 1;
    unsigned bit_28                    : 1;
    unsigned bit_29                    : 1;
    unsigned underspeed                : 1;
    unsigned bit_31                    : 1;
  } actual;
} TRosenbergGen3ErrorCodes;

//==============================================================================
//------------------------ Rosenberg EC-ventilatoren GEN 3 - Control -----------
//==============================================================================
unsigned char RosenbergGen3RequestChangeAddress(unsigned char DeviceAddress, unsigned char NewAddress, s_board_IO_on_off *IO, unsigned char IO_size, void (*CompletedFunc)(unsigned char Failure));

unsigned char RosenbergGen3CreateAlarm(unsigned char DeviceAddress, int FunctionCode, int Index, int Value, int Group);
unsigned char RosenbergGen3AlarmActive(unsigned char DeviceAddress);
unsigned char RosenbergGen3ClearAllAlarms(unsigned char DeviceAddress, int FunctionCode, int Index);

//==============================================================================
//------------------------ Rosenberg EC-ventilatoren GEN 3 - Main routines -----
//==============================================================================
//------------------------------------------------------------------------------
unsigned char RosenbergGen3Main(int DeviceAddress);

#endif