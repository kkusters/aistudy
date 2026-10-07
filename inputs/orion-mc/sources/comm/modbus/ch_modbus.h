// CH_MODBUS.H

#ifndef _CH_MODBUS_H
#define _CH_MODBUS_H

#include <time.h>

#include "ct_data.h"
#include "ch_timer.h"

//==============================================================================
//------------------------ modbus - Defines ------------------------------------
//==============================================================================
#define MAX_MODBUS       16 // 16 is maximum
#define MAX_MODBUS_NODE 247 // 0 is broadcast

#define MB_MSG_MAX_SIZE 256 // maximum message size

#define MB_EXCEPTION 0x80 // if this bit is set in the function code, the device threw an exception

#define MB_OFFSET_COMMAND  0
#define MB_OFFSET_MSG_SIZE 1

#define MB_OFFSET_ADDRESS  0
#define MB_OFFSET_FUNCTION 1
#define MB_OFFSET_DATA     2

//==============================================================================
//------------------------ modbus - Enum's -------------------------------------
//==============================================================================
typedef enum
{
  fcReadCoilStatus          = (unsigned char)1,
  fcReadInputStatus         =  2,
  fcReadHoldingRegister     =  3,
  fcReadInputRegister       =  4,
  fcForceSingleCoil         =  5,
  fcPresetSingleRegister    =  6,
  fcReadExceptionStatus     =  7,
  fcDiagnostics             =  8,
  fcPgrogram484             =  9,
  fcPoll484                 = 10,
  fcFetchCommEventCtr       = 11,
  fcFetchCommEventLog       = 12,
  fcProgramController       = 13,
  fcPollController          = 14,
  fcForceMultipleCoils      = 15,
  fcPresetMultipleRegisters = 16,
  fcReportSlaveID           = 17,
  fcProgram884M84           = 18,
  fcResetCommLink           = 19,
  fcReadGeneralReference    = 20,
  fcWriteGeneralReference   = 21,
  fcMaskWrite4xRegister     = 22,
  fcReadWrite4xRegister     = 23,
  fcReadFifoQueue           = 24
} TFunctionCode;

typedef enum
{
  mbeNoError = (unsigned char)0,
  mbeIllegalFunction,
  mbeIllegalDataAddress,
  mbeIllegalDataValue,
  mbeSlaveDeviceFailure,
  mbeAcknowledge,
  mbeSlaveDeviceBusy,
  mbeNegativeAcknowledge,
  mbeMemoryParityError,
  mbeIllegalDeviceAddress,
  mbeTimeOut
} TmbErrorCode;

typedef enum
{
  mbsIdle = (unsigned char)0,
  mbsBusy,
  mbsShutUp,
  mbsMessagePending
} TmbState;

//==============================================================================
//------------------------ modbus - Typedefs -----------------------------------
//==============================================================================
typedef struct
{
  TFunctionCode FunctionCode;
  unsigned char Address;
  unsigned char *Data;
  unsigned char NrDataBytes;
} TmbMessage;

typedef struct
{
  unsigned int Path;
} TmbNode;

typedef struct
{
  TmbState State;
  unsigned char NrTimeOuts;
  TTimer TimeOutTimer;
  TTimer ShutUpTimer;
  s_board_IO_on_off IO[MAX_MODBUS];
  void (*RxFunc)(TmbErrorCode ErrorCode, TmbMessage *Msg);
  void (*RxPeriodicFunc)(TmbErrorCode ErrorCode, TmbMessage *Msg);
} Tmodbus;

//==============================================================================
//------------------------ modbus - Globals ------------------------------------
//==============================================================================

//==============================================================================
//------------------------ modbus - Timing -------------------------------------
//==============================================================================
//------------------------------------------------------------------------------
unsigned char mbBusy(void);

//==============================================================================
//------------------------ modbus - Create messages ----------------------------
//==============================================================================
//------------------------------------------------------------------------------
unsigned char mbReadCoilStatus(unsigned char DeviceAddress, unsigned int RegisterAddress, unsigned int NrRegisters, void (*RxFunc)(TmbErrorCode ErrorCode, TmbMessage *Msg));
unsigned char mbReadInputStatus(unsigned char DeviceAddress, unsigned int RegisterAddress, unsigned int NrRegisters, void (*RxFunc)(TmbErrorCode ErrorCode, TmbMessage *Msg));
unsigned char mbReadHoldingRegister(unsigned char DeviceAddress, unsigned int RegisterAddress, unsigned int NrRegisters, void (*RxFunc)(TmbErrorCode ErrorCode, TmbMessage *Msg));
unsigned char mbReadInputRegister(unsigned char DeviceAddress, unsigned int RegisterAddress, unsigned int NrRegisters, void (*RxFunc)(TmbErrorCode ErrorCode, TmbMessage *Msg));
unsigned char mbReadInputRegisterPeriodic(unsigned char DeviceAddress, unsigned int RegisterAddress, unsigned int NrRegisters, void (*RxFunc)(TmbErrorCode ErrorCode, TmbMessage *Msg), void (*RxPeriodicFunc)(TmbErrorCode ErrorCode, TmbMessage *Msg));
unsigned char mbWriteSingleCoil(unsigned char DeviceAddress, unsigned int RegisterAddress, unsigned int NrRegisters, void (*RxFunc)(TmbErrorCode ErrorCode, TmbMessage *Msg));
unsigned char mbWriteSingleRegister(unsigned char DeviceAddress, unsigned int RegisterAddress, unsigned int Data, void (*RxFunc)(TmbErrorCode ErrorCode, TmbMessage *Msg));
unsigned char mbWriteMultipleRegister(unsigned char DeviceAddress, unsigned int RegisterAddress, unsigned int NrRegisters, unsigned int *Data, void (*RxFunc)(TmbErrorCode ErrorCode, TmbMessage *Msg));

//==============================================================================
//------------------------ modbus - Control ------------------------------------
//==============================================================================
unsigned char mbAddConnection(unsigned char DeviceAddress, s_board_IO_on_off *IO, unsigned char IO_size);
unsigned char mbResetPath(unsigned char DeviceAddress);

void mbInit(void);
void mbMain(void);

unsigned int  mbReadInt(unsigned char *Data);
unsigned long mbReadLong(unsigned char *Data);

#endif
