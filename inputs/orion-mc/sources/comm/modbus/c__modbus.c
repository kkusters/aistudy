// C__MODBUS.C  

#include "ch_define.h"

#include "ch_alarm.h"
#include "ch_alg.h"
#include "ch_event.h"
#include "ch_IO.h"
#include "ch_IO_RS485.h"
#include "ch_sd.h"
#include "ch_tijd.h"
#include "ch_modbus.h"
#include "ch_modbus_crc.h"

//==============================================================================
//------------------------ modbus - Protocol -----------------------------------
//==============================================================================
//------------------------------------------------------------------------------
//
// |----------|----------|------------------|--------------|
// | Address  | Command  | Data (N x 8bits) | CRC Checksum |
// |----------|----------|------------------|--------------|
//

//==============================================================================
//------------------------ modbus - Globals ------------------------------------
//==============================================================================
//------------------------------------------------------------------------------
#define MAX_RETRIES 10

static Tmodbus mb;
static TmbNode mbNode[MAX_MODBUS_NODE + 1];

static unsigned char mbTxMsgBuf[MB_MSG_MAX_SIZE];
static unsigned char mbTxMsgBufDataSize;
static unsigned char mbRxMsgBuf[MB_MSG_MAX_SIZE];
static unsigned char mbRxMsgBufDataSize;

//==============================================================================
//------------------------ modbus - Prototyping --------------------------------
//==============================================================================
//------------------------------------------------------------------------------
static unsigned char mbTxMessage(unsigned char *Data, unsigned char DataSize);

//==============================================================================
//------------------------ modbus - modbusMode ---------------------------------
//==============================================================================
//------------------------------------------------------------------------------
static unsigned char mbIsTransparantMode(void)
{
  return 1;
}

static unsigned char mbIsModbusMode(void)
{
  return 0;
}

static unsigned char* getTxMsgBuffer(void)
{
  unsigned char base = mbIsModbusMode() ? 2 : 0;
  return &mbTxMsgBuf[base];
}

static unsigned char getTxMsgBufferDataSize(void)
{
  return mbIsModbusMode() ? mbTxMsgBufDataSize - 2 : mbTxMsgBufDataSize;
}

static void addTxMsgHeader(void)
{
  if (mbIsTransparantMode())
    return;

  mbTxMsgBuf[MB_OFFSET_MSG_SIZE] = mbTxMsgBufDataSize;
  if (mbTxMsgBufDataSize > 7)
    mbTxMsgBuf[MB_OFFSET_COMMAND] = cmdStartMsgMore7Bytes;
  else
    mbTxMsgBuf[MB_OFFSET_COMMAND] = cmdStartMsgMax7Bytes;
  mbTxMsgBufDataSize += 2;
}

static unsigned char getRxMsgCommand(s_board_IO_on_off* IO)
{
  return mbIsModbusMode() ? IO_Get_RS485_Char(IO) : 0;
}

static unsigned char getRxMsgDataSize(s_board_IO_on_off* IO)
{
  return mbIsModbusMode() ? IO_Get_RS485_Char(IO) : IO_RS485_RxBuffer_Size(IO);
}

static unsigned char getRxMsgMinimumDataSize(void)
{
  return mbIsModbusMode() ? 2 : 4;
}

static unsigned char isRxMsgDataSizeValid(unsigned char DataSize)
{
  return DataSize >= getRxMsgMinimumDataSize();
}

static unsigned char isRxMsgCrcValid(unsigned char* Data, unsigned char DataSize)
{
  return mbIsModbusMode() ? 1 : mbCRC16(Data, DataSize) == 0;
}

//==============================================================================
//------------------------ modbus - Init ---------------------------------------
//==============================================================================
//------------------------------------------------------------------------------
void mbInit(void)
{
unsigned char *ptr;
int i;

  ptr = (unsigned char *)&mb;
  for (i = 0; i < sizeof(mb); i++)
    ptr[i] = 0;

  ptr = (unsigned char *)&mbNode;
  for (i = 0; i < sizeof(mbNode); i++)
    ptr[i] = 0;

  IO_RS485_Stop_All_Periodic_Messages();
}

static void mbSetRS485ModbusMode(s_board_IO_on_off *IO)
{
  if (mbIsModbusMode())
    IO_Set_RS485_Mode(IO, RS485ModeModbus);
  else
    IO_Set_RS485_Mode(IO, RS485ModeTransparant);
}

unsigned char mbAddConnection(unsigned char DeviceAddress, s_board_IO_on_off *IO, unsigned char IO_size)
{
int i, IO_index;
unsigned char cnt = 0;

  if ((DeviceAddress == 0) || (DeviceAddress > MAX_MODBUS_NODE))
    return (0);

  for (IO_index = 0; IO_index < IO_size; IO_index++)
  {
    if (IO[IO_index].board_type != 0)
	{
      for (i = 0; i < MAX_MODBUS; i++)
      {
        if ((mb.IO[i].board_type == 0) || ((mb.IO[i].board_type == IO[IO_index].board_type) && (mb.IO[i].board_nr == IO[IO_index].board_nr)))
        {
		  mbSetRS485ModbusMode(&IO[IO_index]);
          mb.IO[i] = IO[IO_index];
          mbNode[DeviceAddress].Path = 0xFFFF;
          cnt++;
		  break;
        }
	  }
	}
  }
  return (cnt);
}

unsigned char mbResetPath(unsigned char DeviceAddress)
{
  if ((DeviceAddress == 0) || (DeviceAddress > MAX_MODBUS_NODE))
    return (0);

  mbNode[DeviceAddress].Path = 0;
  return (1);
}

//==============================================================================
//------------------------ modbus - Timing -------------------------------------
//==============================================================================
//------------------------------------------------------------------------------
static unsigned char isRetryNeeded(void)
{
  return (mbIsTransparantMode() && (mb.NrTimeOuts < MAX_RETRIES));
}

static void mbTimeOut(void)
{
TmbMessage Msg;
void (*CopyRxFunc)(TmbErrorCode ErrorCode, TmbMessage *Msg);

  mb.NrTimeOuts++;
  mb.State = mbsIdle;
  if (isRetryNeeded())
  {
    mbTxMessage(mbTxMsgBuf, mbTxMsgBufDataSize);
  }
  else
  {
    mb.NrTimeOuts = 0;
    if (mb.RxFunc != NULL)
    {
	  unsigned char* txMsgBuf = getTxMsgBuffer();
      Msg.FunctionCode = txMsgBuf[MB_OFFSET_FUNCTION];
	  Msg.Address      = txMsgBuf[MB_OFFSET_ADDRESS];
      Msg.Data         = &txMsgBuf[MB_OFFSET_DATA];
      Msg.NrDataBytes  = getTxMsgBufferDataSize();

	  CopyRxFunc = mb.RxFunc;
	  mb.RxFunc  = NULL;
      CopyRxFunc(mbeTimeOut, &Msg);
    }
  }
}

static void mbReady(void)
{
  mb.RxFunc = NULL;
  mb.State  = mbsIdle;
}

static void mbShutUp(void)
{
  TimerSet(&mb.ShutUpTimer, TIMER_25MS);
  mb.State = mbsShutUp;
}

unsigned char mbBusy(void)
{
  switch (mb.State)
  {
    case mbsIdle           : return (0);
    case mbsBusy           :
    case mbsShutUp         :
	case mbsMessagePending :
	default                : return (1);
  }
}

//==============================================================================
//------------------------ modbus - Process message ----------------------------
//==============================================================================
//------------------------------------------------------------------------------
static void mbProcessMessage(TmbMessage *pMsg)
{
  mbShutUp();
  mb.NrTimeOuts = 0;

  if (mb.RxFunc != NULL)
  {
    if (pMsg->FunctionCode & MB_EXCEPTION) // device threw an exception
      mb.RxFunc(pMsg->Data[0], pMsg);
    else
      mb.RxFunc(mbeNoError, pMsg);
  }
}

static void mbProcessPeriodicMessage(TmbMessage *pMsg)
{
  if (mb.RxPeriodicFunc != NULL)
  {
    if (pMsg->FunctionCode & MB_EXCEPTION) // device threw an exception
      mb.RxPeriodicFunc(pMsg->Data[0], pMsg);
    else
      mb.RxPeriodicFunc(mbeNoError, pMsg);
  }
}

static void mbTimeOutPeriodicMessage(TmbMessage *pMsg)
{
  if (mb.RxPeriodicFunc != NULL)
  {
    pMsg->FunctionCode |= MB_EXCEPTION;
    mb.RxPeriodicFunc(mbeTimeOut, pMsg);
  }
}

static void mbProcessMessageTransparant(TmbMessage *pMsg)
{
  mbProcessMessage(pMsg);
}

static void mbProcessMessageModbus(unsigned char command, TmbMessage *pMsg)
{
  switch (command)
  {
    case cmdStartMsgMax7Bytes:
    case cmdStartMsgMore7Bytes:
	  mbProcessMessage(pMsg);
	  break;
	case cmdStartPeriodicMsgMax7Bytes:
    case cmdStartPeriodicMsgMore7Bytes:
      mbProcessPeriodicMessage(pMsg);
	  break;
    case cmdTimeOutPeriodicMsg:
	  mbTimeOutPeriodicMessage(pMsg);
	  break;
  }
}

//==============================================================================
//------------------------ modbus - Receive message ----------------------------
//==============================================================================
//------------------------------------------------------------------------------
static unsigned char mbRxMessage(unsigned char nr)
{
TmbMessage Msg;
unsigned int DataSize;
unsigned char command;
unsigned char i;

  // Check modbus nr --------------------------------------------
  if (nr >= MAX_MODBUS)
    return (0);

  // Read data from IO ------------------------------------------
  command = getRxMsgCommand(&mb.IO[nr]);
  DataSize = getRxMsgDataSize(&mb.IO[nr]);
  for (i = 0; i < DataSize; i++)
    mbRxMsgBuf[i] = IO_Get_RS485_Char(&mb.IO[nr]);

  IO_Reset_RS485_RxBuffer(&mb.IO[nr]);

  // Check length and CRC ---------------------------------------
  if (!isRxMsgDataSizeValid(DataSize) || !isRxMsgCrcValid(mbRxMsgBuf, DataSize))
    return (0);

  // Process message --------------------------------------------
  Msg.Address      = mbRxMsgBuf[MB_OFFSET_ADDRESS ];
  Msg.FunctionCode = mbRxMsgBuf[MB_OFFSET_FUNCTION];
  Msg.NrDataBytes  = DataSize - getRxMsgMinimumDataSize();
  Msg.Data         = &mbRxMsgBuf[MB_OFFSET_DATA];

  mbNode[Msg.Address].Path = (0x0001 << nr);

  if (mbIsTransparantMode())
    mbProcessMessageTransparant(&Msg);
  else
    mbProcessMessageModbus(command, &Msg);
  
  return(1);
}

//==============================================================================
//------------------------ modbus - Transmit message ---------------------------
//==============================================================================
//------------------------------------------------------------------------------
static void mbAddCrcToMessage(unsigned char *Data, unsigned char *DataSize)
{
unsigned int Checksum;

  if (mbIsTransparantMode())
  {
    Checksum = mbCRC16(Data, *DataSize);
    Data[(*DataSize)++] = (unsigned char)(Checksum & 0xFF);
    Data[(*DataSize)++] = (unsigned char)(Checksum >> 8);
  }
}

static unsigned char mbTxMessage(unsigned char *Data, unsigned char DataSize)
{
unsigned int i;
unsigned char address = 0;
unsigned int path = 0;

  if ((mb.State == mbsIdle) || (mb.State == mbsMessagePending))
  {
    mbAddCrcToMessage(Data, &DataSize);

	address = mbIsTransparantMode() ? Data[MB_OFFSET_ADDRESS] : Data[MB_OFFSET_ADDRESS+2];
	path = mbNode[address].Path;

    for (i = 0; i < MAX_MODBUS; i++)
	{
      if (((path == 0) || (path & (0x0001 << i)) || (address == 0)) && (mb.IO[i].board_type != 0))
	  {
	    IO_Reset_RS485_TxBuffer(&mb.IO[i]); // JP 03-12-15
        IO_Reset_RS485_RxBuffer(&mb.IO[i]);
        IO_Set_RS485_Message(&mb.IO[i], Data, DataSize);
        if (address == 0) // broadcast
		{
          TimerSet(&mb.ShutUpTimer, TIMER_25MS * 2);
          mb.State = mbsShutUp;
		}
		else
		{
          TimerSet(&mb.TimeOutTimer, TIMER_500MS);
          mb.State = mbsBusy;
		}
	  }
	}
    return (1);
  }
  return (0);
}

//==============================================================================
//------------------------ modbus - Create messages ----------------------------
//==============================================================================
//------------------------------------------------------------------------------
static void addTxMsgReadCoilStatus(unsigned char DeviceAddress, unsigned int RegisterAddress, unsigned int NrRegisters)
{
  unsigned char* txMsgBuf = getTxMsgBuffer();
  txMsgBuf[MB_OFFSET_ADDRESS ] = DeviceAddress;
  txMsgBuf[MB_OFFSET_FUNCTION] = fcReadCoilStatus;
  txMsgBuf[MB_OFFSET_DATA + 0] = (unsigned char)(RegisterAddress >> 8);
  txMsgBuf[MB_OFFSET_DATA + 1] = (unsigned char)(RegisterAddress & 0xFF);
  txMsgBuf[MB_OFFSET_DATA + 2] = (unsigned char)(NrRegisters >> 8);
  txMsgBuf[MB_OFFSET_DATA + 3] = (unsigned char)(NrRegisters & 0xFF);
}

unsigned char mbReadCoilStatus(unsigned char DeviceAddress, unsigned int RegisterAddress, unsigned int NrRegisters, void (*RxFunc)(TmbErrorCode ErrorCode, TmbMessage *Msg))
{
  if ((mb.State == mbsIdle) || (mb.State == mbsShutUp))
  {
    mbTxMsgBufDataSize = 6;
    addTxMsgReadCoilStatus(DeviceAddress, RegisterAddress, NrRegisters);
    addTxMsgHeader();
    
    mb.RxFunc = RxFunc;
    
    if (mbTxMessage(mbTxMsgBuf, mbTxMsgBufDataSize) == 0)
      mb.State = mbsMessagePending;
    return (1);
  }
  return (0);
}
	 
static void addTxMsgReadInputStatus(unsigned char DeviceAddress, unsigned int RegisterAddress, unsigned int NrRegisters)
{
  unsigned char* txMsgBuf = getTxMsgBuffer();
  txMsgBuf[MB_OFFSET_ADDRESS ] = DeviceAddress;
  txMsgBuf[MB_OFFSET_FUNCTION] = fcReadInputStatus;
  txMsgBuf[MB_OFFSET_DATA + 0] = (unsigned char)(RegisterAddress >> 8);
  txMsgBuf[MB_OFFSET_DATA + 1] = (unsigned char)(RegisterAddress & 0xFF);
  txMsgBuf[MB_OFFSET_DATA + 2] = (unsigned char)(NrRegisters >> 8);
  txMsgBuf[MB_OFFSET_DATA + 3] = (unsigned char)(NrRegisters & 0xFF);
}

unsigned char mbReadInputStatus(unsigned char DeviceAddress, unsigned int RegisterAddress, unsigned int NrRegisters, void (*RxFunc)(TmbErrorCode ErrorCode, TmbMessage *Msg))
{
  if ((mb.State == mbsIdle) || (mb.State == mbsShutUp))
  {
    mbTxMsgBufDataSize = 6;
    addTxMsgReadInputStatus(DeviceAddress, RegisterAddress, NrRegisters);
    addTxMsgHeader();
    
    mb.RxFunc = RxFunc;
    
    if (mbTxMessage(mbTxMsgBuf, mbTxMsgBufDataSize) == 0)
      mb.State = mbsMessagePending;
    return (1);
  }
  return (0);
}
	
static void addTxMsgReadHoldingRegister(unsigned char DeviceAddress, unsigned int RegisterAddress, unsigned int NrRegisters)
{
  unsigned char* txMsgBuf = getTxMsgBuffer();
  txMsgBuf[MB_OFFSET_ADDRESS ] = DeviceAddress;
  txMsgBuf[MB_OFFSET_FUNCTION] = fcReadHoldingRegister;
  txMsgBuf[MB_OFFSET_DATA + 0] = (unsigned char)(RegisterAddress >> 8);
  txMsgBuf[MB_OFFSET_DATA + 1] = (unsigned char)(RegisterAddress & 0xFF);
  txMsgBuf[MB_OFFSET_DATA + 2] = (unsigned char)(NrRegisters >> 8);
  txMsgBuf[MB_OFFSET_DATA + 3] = (unsigned char)(NrRegisters & 0xFF);
}

unsigned char mbReadHoldingRegister(unsigned char DeviceAddress, unsigned int RegisterAddress, unsigned int NrRegisters, void (*RxFunc)(TmbErrorCode ErrorCode, TmbMessage *Msg))
{
  if ((mb.State == mbsIdle) || (mb.State == mbsShutUp))
  {
    mbTxMsgBufDataSize = 6;
    addTxMsgReadHoldingRegister(DeviceAddress, RegisterAddress, NrRegisters);
    addTxMsgHeader();

    mb.RxFunc = RxFunc;

    if (mbTxMessage(mbTxMsgBuf, mbTxMsgBufDataSize) == 0)
	  mb.State = mbsMessagePending;
    return (1);
  }
  return (0);
}

static void addTxMsgReadInputRegister(unsigned char DeviceAddress, unsigned int RegisterAddress, unsigned int NrRegisters)
{
  unsigned char* txMsgBuf = getTxMsgBuffer();
  txMsgBuf[MB_OFFSET_ADDRESS ] = DeviceAddress;
  txMsgBuf[MB_OFFSET_FUNCTION] = fcReadInputRegister;
  txMsgBuf[MB_OFFSET_DATA + 0] = (unsigned char)(RegisterAddress >> 8);
  txMsgBuf[MB_OFFSET_DATA + 1] = (unsigned char)(RegisterAddress & 0xFF);
  txMsgBuf[MB_OFFSET_DATA + 2] = (unsigned char)(NrRegisters >> 8);
  txMsgBuf[MB_OFFSET_DATA + 3] = (unsigned char)(NrRegisters & 0xFF);
}

unsigned char mbReadInputRegister(unsigned char DeviceAddress, unsigned int RegisterAddress, unsigned int NrRegisters, void (*RxFunc)(TmbErrorCode ErrorCode, TmbMessage *Msg))
{
  if ((mb.State == mbsIdle) || (mb.State == mbsShutUp))
  {
    mbTxMsgBufDataSize = 6;
    addTxMsgReadInputRegister(DeviceAddress, RegisterAddress, NrRegisters);
    addTxMsgHeader();

    mb.RxFunc = RxFunc;

    if (mbTxMessage(mbTxMsgBuf, mbTxMsgBufDataSize) == 0)
	  mb.State = mbsMessagePending;
    return (1);
  }
  return (0);
}

unsigned char mbReadInputRegisterPeriodic(unsigned char DeviceAddress, unsigned int RegisterAddress, unsigned int NrRegisters, void (*RxFunc)(TmbErrorCode ErrorCode, TmbMessage *Msg), void (*RxPeriodicFunc)(TmbErrorCode ErrorCode, TmbMessage *Msg))
{
  if (mbIsTransparantMode())
    return 0;

  if ((mb.State == mbsIdle) || (mb.State == mbsShutUp))
  {
    mbTxMsgBufDataSize = 8;
    mbTxMsgBuf[MB_OFFSET_COMMAND ] = cmdStartPeriodicMsgMax7Bytes;
    mbTxMsgBuf[MB_OFFSET_MSG_SIZE] = 6;
    addTxMsgReadInputRegister(DeviceAddress, RegisterAddress, NrRegisters);

    mb.RxFunc = RxFunc;
    mb.RxPeriodicFunc = RxPeriodicFunc;

    if (mbTxMessage(mbTxMsgBuf, mbTxMsgBufDataSize) == 0)
      mb.State = mbsMessagePending;
    return (1);
  }
  return (0);
}

static void addTxMsgWriteSingleCoil(unsigned char DeviceAddress, unsigned int RegisterAddress, unsigned int NrRegisters)
{
  unsigned char* txMsgBuf = getTxMsgBuffer();
  txMsgBuf[MB_OFFSET_ADDRESS ] = DeviceAddress;
  txMsgBuf[MB_OFFSET_FUNCTION] = fcForceSingleCoil;
  txMsgBuf[MB_OFFSET_DATA + 0] = (unsigned char)(RegisterAddress >> 8);
  txMsgBuf[MB_OFFSET_DATA + 1] = (unsigned char)(RegisterAddress & 0xFF);
  txMsgBuf[MB_OFFSET_DATA + 2] = (unsigned char)(NrRegisters >> 8);
  txMsgBuf[MB_OFFSET_DATA + 3] = (unsigned char)(NrRegisters & 0xFF);
}

unsigned char mbWriteSingleCoil(unsigned char DeviceAddress, unsigned int RegisterAddress, unsigned int NrRegisters, void (*RxFunc)(TmbErrorCode ErrorCode, TmbMessage *Msg))
{
  if ((mb.State == mbsIdle) || (mb.State == mbsShutUp))
  {
    mbTxMsgBufDataSize = 6;
    addTxMsgWriteSingleCoil(DeviceAddress, RegisterAddress, NrRegisters);
    addTxMsgHeader();

    mb.RxFunc = RxFunc;

    if (mbTxMessage(mbTxMsgBuf, mbTxMsgBufDataSize) == 0)
	  mb.State = mbsMessagePending;
    return (1);
  }
  return (0);
}
  
unsigned char mbWriteSingleRegister(unsigned char DeviceAddress, unsigned int RegisterAddress, unsigned int Data, void (*RxFunc)(TmbErrorCode ErrorCode, TmbMessage *Msg))
{
  if ((mb.State == mbsIdle) || (mb.State == mbsShutUp))
  {
    unsigned char* txMsgBuf = getTxMsgBuffer();
    txMsgBuf[MB_OFFSET_ADDRESS ] = DeviceAddress;
    txMsgBuf[MB_OFFSET_FUNCTION] = fcPresetSingleRegister;
    txMsgBuf[MB_OFFSET_DATA + 0] = (unsigned char)(RegisterAddress >> 8);
    txMsgBuf[MB_OFFSET_DATA + 1] = (unsigned char)(RegisterAddress & 0xFF);
    txMsgBuf[MB_OFFSET_DATA + 2] = (unsigned char)(Data >> 8);
    txMsgBuf[MB_OFFSET_DATA + 3] = (unsigned char)(Data & 0xFF);
	mbTxMsgBufDataSize = 6;
    addTxMsgHeader();

    mb.RxFunc = RxFunc;

    if (mbTxMessage(mbTxMsgBuf, mbTxMsgBufDataSize) == 0)
	  mb.State = mbsMessagePending;
    return (1);
  }
  return (0);
}

unsigned char mbWriteMultipleRegister(unsigned char DeviceAddress, unsigned int RegisterAddress, unsigned int NrRegisters, unsigned int *Data, void (*RxFunc)(TmbErrorCode ErrorCode, TmbMessage *Msg))
{
int i;

  if ((mb.State == mbsIdle) || (mb.State == mbsShutUp))
  {
    unsigned char* txMsgBuf = getTxMsgBuffer();
    txMsgBuf[MB_OFFSET_ADDRESS ] = DeviceAddress;
    txMsgBuf[MB_OFFSET_FUNCTION] = fcPresetMultipleRegisters;
    txMsgBuf[MB_OFFSET_DATA + 0] = (unsigned char)(RegisterAddress >> 8);
    txMsgBuf[MB_OFFSET_DATA + 1] = (unsigned char)(RegisterAddress & 0xFF);
    txMsgBuf[MB_OFFSET_DATA + 2] = (unsigned char)(NrRegisters >> 8);
    txMsgBuf[MB_OFFSET_DATA + 3] = (unsigned char)(NrRegisters & 0xFF);
    txMsgBuf[MB_OFFSET_DATA + 4] = NrRegisters * 2;
    for (i = 0; i < NrRegisters; i++)
    {
      txMsgBuf[MB_OFFSET_DATA + 5 + (i * 2)] = (unsigned char)(Data[i] >> 8);
      txMsgBuf[MB_OFFSET_DATA + 6 + (i * 2)] = (unsigned char)(Data[i] & 0xFF);
    }
	mbTxMsgBufDataSize = 7 + (NrRegisters * 2);
    addTxMsgHeader();

    mb.RxFunc = RxFunc;

    if (mbTxMessage(mbTxMsgBuf, mbTxMsgBufDataSize) == 0)
	  mb.State = mbsMessagePending;
    return (1);
  }
  return (0);
}

//==============================================================================
//------------------------ modbus - Main routines ------------------------------
//==============================================================================
//------------------------------------------------------------------------------
void mbMain(void)
{
int i;

  // Check if a message was received
  for (i = 0; i < MAX_MODBUS; i++)
  {
    while (IO_RS485_MessageReady(&mb.IO[i]))
	  mbRxMessage(i);
  }
 
  // Check bus timers
  switch (mb.State)
  {
    case mbsBusy:
      if (TimerExpired(&mb.TimeOutTimer))
        mbTimeOut();
      break;
    case mbsShutUp:
      if (TimerExpired(&mb.ShutUpTimer))
        mbReady();
      break;
    case mbsMessagePending:
      if (TimerExpired(&mb.ShutUpTimer))
        mbTxMessage(mbTxMsgBuf, mbTxMsgBufDataSize);
      break;
	default:
	  mb.State = mbsIdle;
	  break;
  }
}

//==============================================================================
//------------------------ modbus - Register routines --------------------------
//==============================================================================
//------------------------------------------------------------------------------
unsigned int mbReadInt(unsigned char *Data)
{
unsigned int Value;

  Value = Data[0];
  Value <<= 8;
  Value |= Data[1];
  return (Value);
}

unsigned long mbReadLong(unsigned char *Data)
{
unsigned long Value;

  Value = Data[2];
  Value <<= 8;
  Value |= Data[3];
  Value <<= 8;
  Value |= Data[0];
  Value <<= 8;
  Value |= Data[1];
  return (Value);
}

//==============================================================================
//------------------------ End of file -----------------------------------------
//==============================================================================
