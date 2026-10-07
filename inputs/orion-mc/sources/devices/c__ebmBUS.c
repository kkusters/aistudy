// C__VENT.C  

#include "ch_define.h"

#include "ch_alarm.h"
#include "ch_alg.h"
#include "ch_mb_device.h"
#include "ch_event.h"
#include "ch_IO.h"
#include "ch_ebmBUS.h"

//==============================================================================
//------------------------ ebmBUS - Protocol -----------------------------------
//==============================================================================
//------------------------------------------------------------------------------
//
// |----------|-----------------------|-------------------|------------|------------|----------|
// | Preamble | Command + fan address | fan group address | Databyte 1 | Databyte n | Checksum |
// |----------|-----------------------|-------------------|------------|------------|----------|
//
// Preamble:
// |----|----|----|---|-----|----|----|----|
// | d2 | d1 | d0 | 1 | Svc | MS | x1 | x0 |
// |----|----|----|---|-----|----|----|----|
//    x:      synchronisation bits
//    MS:     Master - Slave - data direction bit
//    Svc:    service bit
//    d0..d2: number of data bytes sent
//
//

//==============================================================================
//------------------------ ebmBUS - Globals ------------------------------------
//==============================================================================
//------------------------------------------------------------------------------
#define MAX_TIME_OUT 10

TebmBus ebmBus[MAX_EBM_BUS];

static TTimer ManagementTimer; // Timer for collecting management data like running hours
static unsigned char ManagementTimerCnt = 0;

static void (*ChangeAddressCompleted)(unsigned char Failure);

//==============================================================================
//------------------------ ebmBUS - Prototyping --------------------------------
//==============================================================================
//------------------------------------------------------------------------------
static unsigned char ebmBusGetStatus(unsigned char index, int FanAddress, unsigned char IdCode);
static unsigned char ebmBusWriteEeprom(unsigned char index, int FanAddress, unsigned char EepromAddress, unsigned char EepromData);
static unsigned char ebmBusResetCommand(unsigned char index, int FanAddress);

//==============================================================================
//------------------------ ebmBUS - Init ---------------------------------------
//==============================================================================
//------------------------------------------------------------------------------
void ebmBusInit(void)
{
unsigned char *ptr;
int i;

  ptr = (unsigned char *)&ebmBus;
  for (i = 0; i < sizeof(ebmBus); i++)
    ptr[i] = 0;

  TimerSet(&ManagementTimer, TIMER_5MIN);
  ManagementTimerCnt = 0;
  ChangeAddressCompleted = NULL;
}

//==============================================================================
//------------------------ ebmBUS - Transmit message ---------------------------
//==============================================================================
//------------------------------------------------------------------------------
static unsigned char ebmBusTransmitMessage(unsigned char index, TebmMessage *pMessage)
{
unsigned char Preamble;
unsigned char Message[11];
unsigned char Checksum = 0xFF;
int i;

  if (ebmBus[index].Ctrl.Busy || ebmBus[index].Ctrl.ShutUp)
    return (0);

  Preamble = PREAMBLE_MASTER | PREAMBLE_SYNC | 0x10 | (pMessage->NrDataBytes << 5);
  if (pMessage->Service)
    Preamble |= PREAMBLE_SERVICE;

  Message[0] = Preamble;
  Message[1] = (pMessage->FanAddress | (pMessage->Command << 5));
  Message[2] = pMessage->GroupAddress;
  Checksum ^= Message[0];
  Checksum ^= Message[1];
  Checksum ^= Message[2];

  for (i = 0; i < pMessage->NrDataBytes; i++)
  {
    Message[3+i] = pMessage->Data[i];
    Checksum ^= Message[3+i];
  }
  Message[3 + pMessage->NrDataBytes] = Checksum;

  IO_Reset_RS485_RxBuffer(&ebmBus[index].IO);
  IO_Set_RS485_Message(&ebmBus[index].IO, Message, 4 + pMessage->NrDataBytes);
  TimerSet(&ebmBus[index].TimeOutTimer, TIMER_500MS);

  if (pMessage->Command == cmWriteEeprom)
  {
    if ((pMessage->Data[0] == edGroupAddress) || (pMessage->Data[0] == edFanAddress))
      ebmBus[index].Ctrl.ChangeAddress = 1;
  }
  ebmBus[index].Ctrl.Busy   = 1;
  ebmBus[index].Ctrl.ShutUp = 0;

  return (1);
}

//==============================================================================
//------------------------ ebmBUS - Receive message ----------------------------
//==============================================================================
//------------------------------------------------------------------------------
static void ebmBusReceiveStatus(unsigned char index, TebmMessage *pMessage)
{
unsigned long value;
unsigned int  pwr;
int DeviceAddress;


  DeviceAddress = ((pMessage->GroupAddress - 1) * 31) + pMessage->FanAddress;
  switch (pMessage->Data[1])
  {
    case svMotorStatusLowByte:
      mbDevice[DeviceAddress - 1].ErrorCode = (unsigned int)pMessage->Data[0];
	  if (mbDevice[DeviceAddress - 1].ErrorCode & 0x0010) // Fan-Bad... Read high byte
        ebmBusGetStatus(index, DeviceAddress, svMotorStatusHighByte);
      break;
    case svMotorStatusHighByte:
      mbDevice[DeviceAddress - 1].ErrorCode |= (unsigned int)pMessage->Data[0] << 8;
      break;
    case svDcLinkVoltage:
      ebmBus[index].Value = (((unsigned long)pMessage->Data[0] * mbDevice[DeviceAddress - 1].ReferenceVoltage * 20) + 128000) / 256000;
      break;
    case svDcLinkCurrent:
      value = (((unsigned long)pMessage->Data[0] * mbDevice[DeviceAddress - 1].ReferenceCurrent * 2) + 128) / 256;
      pwr   = (ebmBus[index].Value * value) / 1000;
	  mbDevice[DeviceAddress - 1].EnergyConsumption = ((long)mbDevice[DeviceAddress - 1].EnergyConsumption + ((long)pwr * 2)) / 3;
      break;
    case svTempPowerModule:
      mbDevice[DeviceAddress - 1].TempPowerModule = pMessage->Data[0];
      break;
    case svTempMotor:
      mbDevice[DeviceAddress - 1].TempMotor = pMessage->Data[0];
      break;
    case svTempElectronics:
      mbDevice[DeviceAddress - 1].TempElectronics = pMessage->Data[0];
      mbDevice[DeviceAddress - 1].Ctrl.GetTemperature = 0;
      break;
  }
}

static void ebmBusReceiveGetActualSpeed(unsigned char index, TebmMessage *pMessage)
{
int DeviceAddress;

  index;
  DeviceAddress = ((pMessage->GroupAddress - 1) * 31) + pMessage->FanAddress;
  mbDevice[DeviceAddress - 1].ActualValue = (((unsigned int)pMessage->Data[0] * 100) + 125) / 250;
}

static void ebmBusReceiveSetTargetSpeed(unsigned char index, TebmMessage *pMessage)
{
int DeviceAddress;

  index;
  DeviceAddress = ((pMessage->GroupAddress - 1) * 31) + pMessage->FanAddress;
  mbDevice[DeviceAddress - 1].Ctrl.SetTargetValue = 0;
}

static void ebmBusReceiveSoftwareReset(unsigned char index, TebmMessage *pMessage)
{
  pMessage;

  if (ebmBus[index].Ctrl.ChangeAddress)
  {
    if (ChangeAddressCompleted != NULL)
	  ChangeAddressCompleted(0);
	ChangeAddressCompleted = NULL;
    ebmBus[index].Ctrl.ChangeAddress = 0;
  }
}

static void ebmBusReceiveWriteEeprom(unsigned char index, TebmMessage *pMessage)
{
  index;
  pMessage;
}

static void ebmBusReceiveReadEeprom(unsigned char index, TebmMessage *pMessage)
{
int DeviceAddress;

  DeviceAddress = ((pMessage->GroupAddress - 1) * 31) + pMessage->FanAddress;
  switch (pMessage->Data[1])
  {
    case edOperationModes1:
	  if (mbDevice[DeviceAddress - 1].ClosedLoopOpenLoop == omOpenLoop)
      {
        if ((pMessage->Data[0] & 0xFD) != 0x21)
		{
          ebmBusWriteEeprom(index, DeviceAddress, edOperationModes1, (pMessage->Data[0] & 0x02) | 0x21); // write operation mode but leave direction of rotation unchanged
          ebmBusResetCommand(index, DeviceAddress);
		}
	  }
	  else
	  {
        if ((pMessage->Data[0] & 0xFD) != 0x01)
        {
          ebmBusWriteEeprom(index, DeviceAddress, edOperationModes1, (pMessage->Data[0] & 0x02) | 0x01); // write operation mode but leave direction of rotation unchanged
          ebmBusResetCommand(index, DeviceAddress);
        }
	  }
      break;
    case edMaxRpm1:
      ebmBus[index].Value &= 0x0000FFFF;
      ebmBus[index].Value |= (unsigned long)pMessage->Data[0] << 16;
      break;
    case edMaxRpm2:
      ebmBus[index].Value &= 0x00FF00FF;
      ebmBus[index].Value |= (unsigned long)pMessage->Data[0] << 8;
      break;
    case edMaxRpm3:
      ebmBus[index].Value &= 0x00FFFF00;
      ebmBus[index].Value |= (unsigned long)pMessage->Data[0];
      mbDevice[DeviceAddress - 1].MaxRpm = (unsigned long)1875000000 / ebmBus[index].Value;
      break;
    case edHourMeterMSB:
      ebmBus[index].Value &= 0x000000FF;
      ebmBus[index].Value |= (unsigned long)pMessage->Data[0] << 8;
      break;
    case edHourMeterLSB:
      ebmBus[index].Value &= 0x0000FF00;
      ebmBus[index].Value |= (unsigned long)pMessage->Data[0];
      mbDevice[DeviceAddress - 1].RunningHours = ((unsigned long)ebmBus[index].Value * 3600);
      mbDevice[DeviceAddress - 1].Ctrl.GetManagement = 0;
      break;
    case edIdentification:
      if (pMessage->Data[0] < 100)
        mbDevice[DeviceAddress - 1].Version = 0;
      else
        mbDevice[DeviceAddress - 1].Version = pMessage->Data[0];
      break;
    case edRefVoltageMSB:
      ebmBus[index].Value &= 0x000000FF;
      ebmBus[index].Value |= (unsigned long)pMessage->Data[0] << 8;
      break;
    case edRefVoltageLSB:
      ebmBus[index].Value &= 0x0000FF00;
      ebmBus[index].Value |= (unsigned long)pMessage->Data[0];
      mbDevice[DeviceAddress - 1].ReferenceVoltage = ebmBus[index].Value;
      break;
    case edRefCurrentMSB:
      ebmBus[index].Value &= 0x000000FF;
      ebmBus[index].Value |= (unsigned long)pMessage->Data[0] << 8;
      break;
    case edRefCurrentLSB:
      ebmBus[index].Value &= 0x0000FF00;
      ebmBus[index].Value |= (unsigned long)pMessage->Data[0];
      mbDevice[DeviceAddress - 1].ReferenceCurrent = ebmBus[index].Value;
      mbDevice[DeviceAddress - 1].Ctrl.Init = 0;
      break;
  }
}

//==============================================================================
//------------------------ ebmBUS - Process message ----------------------------
//==============================================================================
//------------------------------------------------------------------------------
static void ebmBusNextMessage(unsigned char index)
{
  if (ebmBus[index].GetIndex != ebmBus[index].PutIndex)
    ebmBus[index].GetIndex++;
  if (ebmBus[index].GetIndex >= EBM_BUFFER_SIZE)
    ebmBus[index].GetIndex = 0;
}

static void ebmBusProcessMessage(unsigned char index, TebmMessage *pMessage)
{
int DeviceAddress;

  DeviceAddress = ((pMessage->GroupAddress - 1) * 31) + pMessage->FanAddress;
  mbDevice[DeviceAddress - 1].Ctrl.LostCommunication = 0;
//  mbDevice[DeviceAddress - 1].Path &= (0x0001 << index); // TD - FiwiHex (21-12-2012)
  mbDevice[DeviceAddress - 1].Path = (0x0001 << index);
  switch (pMessage->Command)
  {
    case cmGetStatus:
      ebmBusReceiveStatus(index, pMessage);
      break;
    case cmGetActualSpeed:
      ebmBusReceiveGetActualSpeed(index, pMessage);
      break;
    case cmSetTargetSpeed:
      ebmBusReceiveSetTargetSpeed(index, pMessage);
      break;
    case cmSoftwareReset:
      ebmBusReceiveSoftwareReset(index, pMessage);
      break;
    case cmNoFunction:
      break;
    case cmDiagnosis:
      break;
    case cmWriteEeprom:
      ebmBusReceiveWriteEeprom(index, pMessage);
      break;
    case cmReadEeprom:
      ebmBusReceiveReadEeprom(index, pMessage);
      break;
  }
}

static unsigned char ebmBusMessageReceived(unsigned char index)
{
TebmMessage Message;
unsigned char Data[11];
unsigned char GroupAddress;
unsigned char FanAddress;
unsigned char Command;
unsigned char DataBytes;
unsigned char Checksum = 0;
int i;

  // Read preamble ----------------------------------------------
  Data[0] = IO_Get_RS485_Char(&ebmBus[index].IO); // Preamble

  // Check if preamble correct ----------------------------------
  if ((Data[0] & PREAMBLE_MASK) != PREAMBLE_SLAVE)
    return (0);

  // Read data from IO ------------------------------------------
  Data[1] = IO_Get_RS485_Char(&ebmBus[index].IO); // Command
  Data[2] = IO_Get_RS485_Char(&ebmBus[index].IO); // Address

  DataBytes    = (Data[0] >> 5) & 0x07;
  Command      = (Data[1] >> 5) & 0x07;
  FanAddress   = (Data[1] & 0x1F);
  GroupAddress = (Data[2]);

  for (i = 0; i < DataBytes; i++)
    Data[3+i] = IO_Get_RS485_Char(&ebmBus[index].IO); // Data

  Data[3+DataBytes] = IO_Get_RS485_Char(&ebmBus[index].IO); // Checksum

  for (i = 0; i < 4+DataBytes; i++)
    Checksum ^= Data[i];

  // Check if data correct --------------------------------------
  if (Checksum != 0xFF)
  {
    // checksum error
    IO_Reset_RS485_RxBuffer(&ebmBus[index].IO);
    return (0);
  }
  if ((GroupAddress == 0) || (FanAddress == 0))
  {
    // error - wrong address
    IO_Reset_RS485_RxBuffer(&ebmBus[index].IO);
    return (0);
  }

  // Process message --------------------------------------------
  Message.Command      = Command;
  Message.FanAddress   = FanAddress;
  Message.GroupAddress = GroupAddress;
  Message.NrDataBytes  = DataBytes;
  for (i = 0; i < DataBytes; i++)
    Message.Data[i] = Data[3+i];

  ebmBusProcessMessage(index, &Message);
  ebmBusNextMessage(index);

  IO_Reset_RS485_RxBuffer(&ebmBus[index].IO);

  TimerSet(&ebmBus[index].ShutUpTimer, TIMER_25MS);
  ebmBus[index].NrTimeOuts  = 0;
  ebmBus[index].Ctrl.ShutUp = 1;
  ebmBus[index].Ctrl.Busy   = 0;
  return (1);
}

//==============================================================================
//------------------------ ebmBUS - Timing -------------------------------------
//==============================================================================
//------------------------------------------------------------------------------
static void ebmBusTimeOut(unsigned char index)
{
unsigned char OnBus;
int DeviceAddress, NextAddress;

  ebmBus[index].NrTimeOuts++;
  DeviceAddress = ((ebmBus[index].MessageBuffer[ebmBus[index].GetIndex].GroupAddress - 1) * 31) + ebmBus[index].MessageBuffer[ebmBus[index].GetIndex].FanAddress;
  if (mbDevice[DeviceAddress - 1].Path & (0x0001 << index))
    OnBus = 1;
  else
    OnBus = 0;

  if ((OnBus || ebmBus[index].Ctrl.ChangeAddress) && (ebmBus[index].NrTimeOuts < MAX_TIME_OUT))
  {
    ebmBus[index].Ctrl.Busy   = 0;
    ebmBus[index].Ctrl.ShutUp = 0;
    ebmBusTransmitMessage(index, &ebmBus[index].MessageBuffer[ebmBus[index].GetIndex]);
  }
  else
  {
    if (OnBus)
    {
      mbDevice[DeviceAddress - 1].Ctrl.LostCommunication = 1;
//      mbDevice[DeviceAddress - 1].Ctrl.InitVent          = 1; // TD - FiwiHex (21-12-2012)
      mbDevice[DeviceAddress - 1].Ctrl.Connected         = 0;
    }
    if (ebmBus[index].Ctrl.ChangeAddress)
    {
      if (ChangeAddressCompleted != NULL)
        ChangeAddressCompleted(1);
	  ChangeAddressCompleted = NULL;
      ebmBus[index].Ctrl.ChangeAddress = 0;
    }
    ebmBus[index].Ctrl.Busy   = 0;
    ebmBus[index].Ctrl.ShutUp = 0;
    ebmBus[index].NrTimeOuts  = 0;
    do
    {
      ebmBusNextMessage(index);
      NextAddress = ((ebmBus[index].MessageBuffer[ebmBus[index].GetIndex].GroupAddress - 1) * 31) + ebmBus[index].MessageBuffer[ebmBus[index].GetIndex].FanAddress;
    } while ((ebmBus[index].GetIndex != ebmBus[index].PutIndex) && (NextAddress == DeviceAddress));
  }
}

static void ebmBusReady(unsigned char index)
{
  ebmBus[index].Ctrl.Busy   = 0;
  ebmBus[index].Ctrl.ShutUp = 0;
}

//==============================================================================
//------------------------ ebmBUS - Create messages ----------------------------
//==============================================================================
//------------------------------------------------------------------------------
static unsigned char ebmBusAddMessage(unsigned char index, TebmMessage *pMessage)
{
  if ((ebmBus[index].PutIndex + 1) == ebmBus[index].GetIndex)
    return (0); // Buffer overflow

  ebmBus[index].MessageBuffer[ebmBus[index].PutIndex] = *pMessage;
  ebmBus[index].PutIndex++;
  if (ebmBus[index].PutIndex >= EBM_BUFFER_SIZE)
    ebmBus[index].PutIndex = 0;
  return (1);
}

static unsigned char ebmBusSetTargetValue(unsigned char index, int FanAddress, int Value)
{
TebmMessage Message;

  Message.Command      = cmSetTargetSpeed;
  Message.Service      = 0;
  Message.GroupAddress = ((FanAddress - 1) / 31) + 1;
  Message.FanAddress   = ((FanAddress - 1) % 31) + 1;
  Message.NrDataBytes  = 1;
  if (mbDevice[FanAddress - 1].ClosedLoopOpenLoop == omOpenLoop)
    Message.Data[0]    = (((unsigned int)Value * 255) + 50) / 100;
  else
    Message.Data[0]    = (((unsigned int)Value * 250) + 50) / 100;

  return (ebmBusAddMessage(index, &Message));
}

static unsigned char ebmBusGetActualSpeed(unsigned char index, int FanAddress)
{
TebmMessage Message;

  Message.Command      = cmGetActualSpeed;
  Message.Service      = 0;
  Message.GroupAddress = ((FanAddress - 1) / 31) + 1;
  Message.FanAddress   = ((FanAddress - 1) % 31) + 1;
  Message.NrDataBytes  = 0;

  return (ebmBusAddMessage(index, &Message));
}

static unsigned char ebmBusGetStatus(unsigned char index, int FanAddress, unsigned char IdCode)
{
TebmMessage Message;

  Message.Command      = cmGetStatus;
  Message.Service      = 0;
  Message.GroupAddress = ((FanAddress - 1) / 31) + 1;
  Message.FanAddress   = ((FanAddress - 1) % 31) + 1;
  Message.NrDataBytes  = 1;
  Message.Data[0]      = IdCode;

  return (ebmBusAddMessage(index, &Message));
}

static unsigned char ebmBusWriteEeprom(unsigned char index, int FanAddress, unsigned char EepromAddress, unsigned char EepromData)
{
TebmMessage Message;

  Message.Command      = cmWriteEeprom;
  Message.Service      = 0;
  Message.GroupAddress = ((FanAddress - 1) / 31) + 1;
  Message.FanAddress   = ((FanAddress - 1) % 31) + 1;
  Message.NrDataBytes  = 2;
  Message.Data[0]      = EepromAddress;
  Message.Data[1]      = EepromData;

  return (ebmBusAddMessage(index, &Message));
}

static unsigned char ebmBusReadEeprom(unsigned char index, int FanAddress, unsigned char EepromAddress)
{
TebmMessage Message;

  Message.Command      = cmReadEeprom;
  Message.Service      = 0;
  Message.GroupAddress = ((FanAddress - 1) / 31) + 1;
  Message.FanAddress   = ((FanAddress - 1) % 31) + 1;
  Message.NrDataBytes  = 1;
  Message.Data[0]      = EepromAddress;

  return (ebmBusAddMessage(index, &Message));
}

static unsigned char ebmBusResetCommand(unsigned char index, int FanAddress)
{
TebmMessage Message;

  Message.Command      = cmSoftwareReset;
  Message.Service      = 0;
  Message.GroupAddress = ((FanAddress - 1) / 31) + 1;
  Message.FanAddress   = ((FanAddress - 1) % 31) + 1;
  Message.NrDataBytes  = 0;

  return (ebmBusAddMessage(index, &Message));
}

//==============================================================================
//------------------------ ebmBUS - State Machine ------------------------------
//==============================================================================
//------------------------------------------------------------------------------
static void ebmBusNextNode(unsigned char index)
{
unsigned int mask = (0x0001 << index);
int i;

  for (i = ebmBus[index].Address; i < MAX_MB_DEVICE; i++)
  {
    if (mbDevice[i].Ctrl.Connected && (mbDevice[i].Type == dtEbmBus) && (mbDevice[i].Path & mask))
    {
      ebmBus[index].Address = i + 1;
      return;
    }
  }
  for (i = 0; i < MAX_MB_DEVICE; i++)
  {
    if (mbDevice[i].Ctrl.Connected && (mbDevice[i].Type == dtEbmBus) && (mbDevice[i].Path & mask))
    {
      ebmBus[index].Address = i + 1;
      return;
    }
  }
  ebmBus[index].Address = 0;
}

static void ebmBusInitNode(unsigned char index, int DeviceAddress)
{
unsigned int RampUp, RampDown;

  ebmBusReadEeprom(index, DeviceAddress, edOperationModes1);
  ebmBusReadEeprom(index, DeviceAddress, edIdentification);
  ebmBusReadEeprom(index, DeviceAddress, edMaxRpm1);
  ebmBusReadEeprom(index, DeviceAddress, edMaxRpm2);
  ebmBusReadEeprom(index, DeviceAddress, edMaxRpm3);
  ebmBusReadEeprom(index, DeviceAddress, edRefVoltageMSB);
  ebmBusReadEeprom(index, DeviceAddress, edRefVoltageLSB);
  ebmBusReadEeprom(index, DeviceAddress, edRefCurrentMSB);
  ebmBusReadEeprom(index, DeviceAddress, edRefCurrentLSB);
  RampUp   = (((unsigned int)mbDevice[DeviceAddress - 1].RampUp   * 4) + 5) / 10;
  RampDown = (((unsigned int)mbDevice[DeviceAddress - 1].RampDown * 4) + 5) / 10;
  ebmBusWriteEeprom(index, DeviceAddress, edRampUp,   RampUp  );
  ebmBusWriteEeprom(index, DeviceAddress, edRampDown, RampDown);
  ebmBusResetCommand(index, DeviceAddress);
  ebmBusSetTargetValue(index, DeviceAddress, mbDevice[DeviceAddress - 1].TargetValue);

  mbDevice[DeviceAddress - 1].Ctrl.GetManagement  = 1;
  mbDevice[DeviceAddress - 1].Ctrl.GetTemperature = 1;
  mbDevice[DeviceAddress - 1].Ctrl.SetTargetValue = 1;
  ebmBusNextNode(index);
}

static void ebmBusGetActualData(unsigned char index, int DeviceAddress)
{
  ebmBusSetTargetValue(index, DeviceAddress, mbDevice[DeviceAddress - 1].TargetValue);
  ebmBusGetActualSpeed(index, DeviceAddress);
  ebmBusGetStatus(index, DeviceAddress, svMotorStatusLowByte);
  ebmBusGetStatus(index, DeviceAddress, svDcLinkVoltage);
  ebmBusGetStatus(index, DeviceAddress, svDcLinkCurrent);

  ebmBusNextNode(index);
}

static void ebmBusGetManagement(unsigned char index, int DeviceAddress)
{
  ebmBusReadEeprom(index, DeviceAddress, edHourMeterMSB);
  ebmBusReadEeprom(index, DeviceAddress, edHourMeterLSB);

  ebmBusNextNode(index);
}

static void ebmBusGetManagementAllNodes(void)
{
unsigned int i;

  for (i = 0; i < MAX_MB_DEVICE; i++)
    mbDevice[i].Ctrl.GetManagement = 1;
}

static void ebmBusGetTemperature(unsigned char index, int DeviceAddress)
{
  ebmBusGetStatus(index, DeviceAddress, svTempPowerModule);
  ebmBusGetStatus(index, DeviceAddress, svTempMotor);
  ebmBusGetStatus(index, DeviceAddress, svTempElectronics);

  ebmBusNextNode(index);
}

static void ebmBusGetTemperatureAllNodes(void)
{
unsigned int i;

  for (i = 0; i < MAX_MB_DEVICE; i++)
    mbDevice[i].Ctrl.GetTemperature = 1;
}

//==============================================================================
//------------------------ ebmBUS - Interface ----------------------------------
//==============================================================================
//------------------------------------------------------------------------------
unsigned char ebmBusAddConnection(int DeviceAddress, s_board_IO_on_off *IO, unsigned char IO_size)
{
int i, IO_index;
unsigned char RetVal = 0;

  if ((DeviceAddress == 0) || (DeviceAddress > MAX_MB_DEVICE)) // Wrong address
    return (0);

  for (IO_index = 0; IO_index < IO_size; IO_index++)
  {
    if (IO[IO_index].board_type != 0)
    {
      for (i = 0; i < MAX_EBM_BUS; i++)
      {
        if ((ebmBus[i].IO.board_type == 0) || ((ebmBus[i].IO.board_type == IO[IO_index].board_type) && (ebmBus[i].IO.board_nr == IO[IO_index].board_nr)))
        {
          ebmBus[i].IO = IO[IO_index];

//          mbDevice[DeviceAddress - 1].Path |= (1 << i); // TD - FiwiHex (21-12-2012)
          mbDevice[DeviceAddress - 1].Path = 0xFFFF;
          mbDevice[DeviceAddress - 1].Ctrl.Init = 1;

          if (ebmBus[i].Address == 0)
            ebmBus[i].Address = DeviceAddress;

          RetVal++;
          break;
        }
      }
    }
  }
  return (RetVal);
}


unsigned char ebmBusRequestChangeAddress(int OldAddress, int NewAddress, s_board_IO_on_off *IO, unsigned char IO_size, void (*CompletedFunc)(unsigned char Failure))
{
unsigned char RetVal = 0;
int i;

  if ((OldAddress == 0) || (OldAddress > MAX_MB_DEVICE) || (NewAddress == 0) || (NewAddress > MAX_MB_DEVICE)) // Wrong address
    return (0);

  IO;
  IO_size;

  for (i = 0; i < MAX_EBM_BUS; i++)
  {
    if (ebmBus[i].IO.board_type)
    {
      ebmBusWriteEeprom(i, OldAddress, edGroupAddress, ((NewAddress - 1) / 31) + 1);
      ebmBusWriteEeprom(i, OldAddress, edFanAddress,   ((NewAddress - 1) % 31) + 1);
      ebmBusResetCommand(i, OldAddress);
      ChangeAddressCompleted = CompletedFunc;
	  RetVal = 1;
    }
  }
  return (RetVal);
}

void ebmNodeSetMaximumRpm(int DeviceAddress, unsigned int Rpm)
{
int i;
unsigned int mask = 1;
unsigned long Bin = 0;

  if ((DeviceAddress == 0) || (DeviceAddress > MAX_MB_DEVICE))
    return;

  for (i = 0; i < MAX_EBM_BUS; i++)
  {
    if (mbDevice[DeviceAddress - 1].Path & mask)
    {
      Bin = (unsigned long)1875000000 / Rpm;
      ebmBusWriteEeprom (i, DeviceAddress, edMaxRpm1, Bin >> 16);
      ebmBusWriteEeprom (i, DeviceAddress, edMaxRpm2, Bin >> 8);
      ebmBusWriteEeprom (i, DeviceAddress, edMaxRpm3, Bin);
      ebmBusResetCommand(i, DeviceAddress);
      mbDevice[DeviceAddress - 1].MaxRpm = Rpm;
      mbDevice[DeviceAddress - 1].Ctrl.Init = 1;
    }
    mask <<= 1;
  }
}

//==============================================================================
//------------------------ ebmBUS - Alarm --------------------------------------
//==============================================================================
//------------------------------------------------------------------------------
unsigned char ebmNodeCreateAlarm(int DeviceAddress, int FunctionCode, int Index, int Value, int Group)
{
int i;
unsigned char Alarm = 0;

  if ((DeviceAddress == 0) || (DeviceAddress > MAX_MB_DEVICE))
    return (0);

  i = DeviceAddress - 1;
  if (mbDevice[i].Ctrl.LostCommunication)
  {
    if (alarm_hr_alg.mbDevice[i].Communication == 0)
      ebmNodeClearAllAlarms(DeviceAddress, FunctionCode, Index);
    CreateAlarm(&alarm_hr_alg.mbDevice[i].Communication, FunctionCode + MB_DEVICE_COMMUNICATION_AL, Index, Value, Group, HARD_ALARM);
	Alarm = 1;
  }
  else
  {
    ClearAlarm(&alarm_hr_alg.mbDevice[i].Communication, FunctionCode + MB_DEVICE_COMMUNICATION_AL, Index);
    if (alarm_hr_alg.mbDevice[i].AlarmCode != mbDevice[i].ErrorCode)
    {
      alarm_hr_alg.mbDevice[i].AlarmCode = mbDevice[i].ErrorCode;

      if (alarm_hr_alg.mbDevice[i].AlarmCode & 0x0080)
        CreateAlarm(&alarm_hr_alg.mbDevice[i].LockedMotor, FunctionCode + MB_DEVICE_LOCKED_MOTOR_AL, Index, Value, Group, HARD_ALARM);
	  else
        ClearAlarm(&alarm_hr_alg.mbDevice[i].LockedMotor,  FunctionCode + MB_DEVICE_LOCKED_MOTOR_AL, Index);

      if (alarm_hr_alg.mbDevice[i].AlarmCode & 0x0040)
        CreateAlarm(&alarm_hr_alg.mbDevice[i].HallFailure, FunctionCode + MB_DEVICE_HALL_FAILURE_AL, Index, Value, Group, HARD_ALARM);
      else
        ClearAlarm(&alarm_hr_alg.mbDevice[i].HallFailure,  FunctionCode + MB_DEVICE_HALL_FAILURE_AL, Index);

      if (alarm_hr_alg.mbDevice[i].AlarmCode & 0x0020)
        CreateAlarm(&alarm_hr_alg.mbDevice[i].ThermalMotor, FunctionCode + MB_DEVICE_THERMAL_MOTOR_AL, Index, Value, Group, HARD_ALARM);
	  else
        ClearAlarm(&alarm_hr_alg.mbDevice[i].ThermalMotor,  FunctionCode + MB_DEVICE_THERMAL_MOTOR_AL, Index);

      if (alarm_hr_alg.mbDevice[i].AlarmCode & 0x0008)
        CreateAlarm(&alarm_hr_alg.mbDevice[i].CommErrorMasterSlavePIC, FunctionCode + MB_DEVICE_COMM_ERROR_MASTER_SLAVE_AL, Index, Value, Group, HARD_ALARM);
	  else
        ClearAlarm(&alarm_hr_alg.mbDevice[i].CommErrorMasterSlavePIC,  FunctionCode + MB_DEVICE_COMM_ERROR_MASTER_SLAVE_AL, Index);

      if (alarm_hr_alg.mbDevice[i].AlarmCode & 0x0004)
        CreateAlarm(&alarm_hr_alg.mbDevice[i].ThermalPowerModule, FunctionCode + MB_DEVICE_THERMAL_POWER_MODULE_AL, Index, Value, Group, HARD_ALARM);
	  else
        ClearAlarm(&alarm_hr_alg.mbDevice[i].ThermalPowerModule,  FunctionCode + MB_DEVICE_THERMAL_POWER_MODULE_AL, Index);

      if (alarm_hr_alg.mbDevice[i].AlarmCode & 0x0002)
        CreateAlarm(&alarm_hr_alg.mbDevice[i].CommErrorRemoteUnit, FunctionCode + MB_DEVICE_COMM_ERROR_REMOTE_UNIT_AL, Index, Value, Group, HARD_ALARM);
	  else
        ClearAlarm(&alarm_hr_alg.mbDevice[i].CommErrorRemoteUnit,  FunctionCode + MB_DEVICE_COMM_ERROR_REMOTE_UNIT_AL, Index);

      if (alarm_hr_alg.mbDevice[i].AlarmCode & 0x0001)
        CreateAlarm(&alarm_hr_alg.mbDevice[i].PhaseFailure, FunctionCode + MB_DEVICE_PHASE_FAILURE_AL, Index, Value, Group, HARD_ALARM);
	  else
        ClearAlarm(&alarm_hr_alg.mbDevice[i].PhaseFailure,  FunctionCode + MB_DEVICE_PHASE_FAILURE_AL, Index);

      if (alarm_hr_alg.mbDevice[i].AlarmCode & 0x8000)
        CreateAlarm(&alarm_hr_alg.mbDevice[i].Brake, FunctionCode + MB_DEVICE_BRAKE_AL, Index, Value, Group, HARD_ALARM);
	  else
        ClearAlarm(&alarm_hr_alg.mbDevice[i].Brake,  FunctionCode + MB_DEVICE_BRAKE_AL, Index);

      if (alarm_hr_alg.mbDevice[i].AlarmCode & 0x2000)
        CreateAlarm(&alarm_hr_alg.mbDevice[i].LowLineVoltage, FunctionCode + MB_DEVICE_LOW_LINE_VOLTAGE_AL, Index, Value, Group, HARD_ALARM);
	  else
        ClearAlarm(&alarm_hr_alg.mbDevice[i].LowLineVoltage,  FunctionCode + MB_DEVICE_LOW_LINE_VOLTAGE_AL, Index);

      if (alarm_hr_alg.mbDevice[i].AlarmCode & 0x1000)
        CreateAlarm(&alarm_hr_alg.mbDevice[i].LowDcLinkVoltage, FunctionCode + MB_DEVICE_LOW_DC_LINK_VOLTAGE_AL, Index, Value, Group, HARD_ALARM);
	  else
        ClearAlarm(&alarm_hr_alg.mbDevice[i].LowDcLinkVoltage,  FunctionCode + MB_DEVICE_LOW_DC_LINK_VOLTAGE_AL, Index);

      if (alarm_hr_alg.mbDevice[i].AlarmCode & 0x0800)
        CreateAlarm(&alarm_hr_alg.mbDevice[i].HighDcLinkVoltage, FunctionCode + MB_DEVICE_HIGH_DC_LINK_VOLTAGE_AL, Index, Value, Group, HARD_ALARM);
	  else
        ClearAlarm(&alarm_hr_alg.mbDevice[i].HighDcLinkVoltage,  FunctionCode + MB_DEVICE_HIGH_DC_LINK_VOLTAGE_AL, Index);

      if (alarm_hr_alg.mbDevice[i].AlarmCode & 0x0400)
        CreateAlarm(&alarm_hr_alg.mbDevice[i].DriverProblem, FunctionCode + MB_DEVICE_DRIVER_PROBLEM_AL, Index, Value, Group, HARD_ALARM);
	  else
        ClearAlarm(&alarm_hr_alg.mbDevice[i].DriverProblem,  FunctionCode + MB_DEVICE_DRIVER_PROBLEM_AL, Index);

      if (alarm_hr_alg.mbDevice[i].AlarmCode & 0x0200)
        CreateAlarm(&alarm_hr_alg.mbDevice[i].ElectronicBoxOverHeat, FunctionCode + MB_DEVICE_ELECTRONIC_BOX_OVER_HEAT_AL, Index, Value, Group, HARD_ALARM);
	  else
        ClearAlarm(&alarm_hr_alg.mbDevice[i].ElectronicBoxOverHeat,  FunctionCode + MB_DEVICE_ELECTRONIC_BOX_OVER_HEAT_AL, Index);

      if (alarm_hr_alg.mbDevice[i].AlarmCode & 0x0100)
        CreateAlarm(&alarm_hr_alg.mbDevice[i].ExcessiveDcLinkCurrent, FunctionCode + MB_DEVICE_EXCESSIVE_DC_LINK_CURRENT_AL, Index, Value, Group, HARD_ALARM);
	  else
        ClearAlarm(&alarm_hr_alg.mbDevice[i].ExcessiveDcLinkCurrent,  FunctionCode + MB_DEVICE_EXCESSIVE_DC_LINK_CURRENT_AL, Index);

      if (alarm_hr_alg.mbDevice[i].AlarmCode == 0x0010)
        CreateAlarm(&alarm_hr_alg.mbDevice[i].Unknown, FunctionCode + MB_DEVICE_UNKNOWN_AL, Index, Value, Group, HARD_ALARM);
	  else
        ClearAlarm(&alarm_hr_alg.mbDevice[i].Unknown,  FunctionCode + MB_DEVICE_UNKNOWN_AL, Index);
	}
	if (alarm_hr_alg.mbDevice[i].AlarmCode != 0x0000)
	  Alarm = 1;
  }
  return (Alarm);
}

unsigned char ebmNodeAlarmActive(int DeviceAddress)
{
int i;

  if ((DeviceAddress == 0) || (DeviceAddress > MAX_MB_DEVICE))
    return (0);

  i = DeviceAddress - 1;
  if ((alarm_hr_alg.mbDevice[i].AlarmCode) || (mbDevice[i].Ctrl.LostCommunication))
    return (1);
  else
    return (0);
}

unsigned char ebmNodeClearAllAlarms(int DeviceAddress, int FunctionCode, int Index)
{
int i;

  if ((DeviceAddress == 0) || (DeviceAddress > MAX_MB_DEVICE))
    return (0);

  i = DeviceAddress - 1;
  alarm_hr_alg.mbDevice[i].AlarmCode = 0;

  ClearAlarm(&alarm_hr_alg.mbDevice[i].Unknown,                 FunctionCode + MB_DEVICE_UNKNOWN_AL,                   Index);
  ClearAlarm(&alarm_hr_alg.mbDevice[i].Communication,           FunctionCode + MB_DEVICE_COMMUNICATION_AL,             Index);
  ClearAlarm(&alarm_hr_alg.mbDevice[i].LockedMotor,             FunctionCode + MB_DEVICE_LOCKED_MOTOR_AL,              Index);
  ClearAlarm(&alarm_hr_alg.mbDevice[i].HallFailure,             FunctionCode + MB_DEVICE_HALL_FAILURE_AL,              Index);
  ClearAlarm(&alarm_hr_alg.mbDevice[i].ThermalMotor,            FunctionCode + MB_DEVICE_THERMAL_MOTOR_AL,             Index);
  ClearAlarm(&alarm_hr_alg.mbDevice[i].CommErrorMasterSlavePIC, FunctionCode + MB_DEVICE_COMM_ERROR_MASTER_SLAVE_AL,   Index);
  ClearAlarm(&alarm_hr_alg.mbDevice[i].ThermalPowerModule,      FunctionCode + MB_DEVICE_THERMAL_POWER_MODULE_AL,      Index);
  ClearAlarm(&alarm_hr_alg.mbDevice[i].CommErrorRemoteUnit,     FunctionCode + MB_DEVICE_COMM_ERROR_REMOTE_UNIT_AL,    Index);
  ClearAlarm(&alarm_hr_alg.mbDevice[i].PhaseFailure,            FunctionCode + MB_DEVICE_PHASE_FAILURE_AL,             Index);
  ClearAlarm(&alarm_hr_alg.mbDevice[i].Brake,                   FunctionCode + MB_DEVICE_BRAKE_AL,                     Index);
  ClearAlarm(&alarm_hr_alg.mbDevice[i].LowLineVoltage,          FunctionCode + MB_DEVICE_LOW_LINE_VOLTAGE_AL,          Index);
  ClearAlarm(&alarm_hr_alg.mbDevice[i].LowDcLinkVoltage,        FunctionCode + MB_DEVICE_LOW_DC_LINK_VOLTAGE_AL,       Index);
  ClearAlarm(&alarm_hr_alg.mbDevice[i].HighDcLinkVoltage,       FunctionCode + MB_DEVICE_HIGH_DC_LINK_VOLTAGE_AL,      Index);
  ClearAlarm(&alarm_hr_alg.mbDevice[i].DriverProblem,           FunctionCode + MB_DEVICE_DRIVER_PROBLEM_AL,            Index);
  ClearAlarm(&alarm_hr_alg.mbDevice[i].ElectronicBoxOverHeat,   FunctionCode + MB_DEVICE_ELECTRONIC_BOX_OVER_HEAT_AL,  Index);
  ClearAlarm(&alarm_hr_alg.mbDevice[i].ExcessiveDcLinkCurrent,  FunctionCode + MB_DEVICE_EXCESSIVE_DC_LINK_CURRENT_AL, Index);
  return (1);
}

//==============================================================================
//------------------------ ebmBUS - Main ---------------------------------------
//==============================================================================
//------------------------------------------------------------------------------
void ebmBusMain(void)
{
static int index = 0;
int DeviceAddress;

  // Check if buffer empty, if so put data in buffer
  DeviceAddress = ebmBus[index].Address;
  if ((DeviceAddress == 0) || (DeviceAddress > MAX_MB_DEVICE) || (mbDevice[DeviceAddress - 1].Ctrl.Connected == 0) || (mbDevice[DeviceAddress - 1].Type != dtEbmBus))
  {
    ebmBusNextNode(index);
  }
  else
  {
    if (ebmBus[index].GetIndex == ebmBus[index].PutIndex)
    {
      if (mbDevice[DeviceAddress - 1].Ctrl.Init)
        ebmBusInitNode(index, DeviceAddress);
      else if (mbDevice[DeviceAddress - 1].Ctrl.GetManagement)
        ebmBusGetManagement(index, DeviceAddress);
      else if (mbDevice[DeviceAddress - 1].Ctrl.GetTemperature)
        ebmBusGetTemperature(index, DeviceAddress);
      else
        ebmBusGetActualData(index, DeviceAddress);
    }
  }

  // Check if bus ready, if so check if data in buffer
  if (IO_RS485_MessageReady(&ebmBus[index].IO))
  {
    if (ebmBusMessageReceived(index) == 0)
	{
      if (TimerExpired(&ebmBus[index].TimeOutTimer))
        ebmBusTimeOut(index);
	}
  }
  else if (ebmBus[index].Ctrl.Busy)
  {
    if (TimerExpired(&ebmBus[index].TimeOutTimer))
      ebmBusTimeOut(index);
  }
  else if (ebmBus[index].Ctrl.ShutUp)
  {
    if (TimerExpired(&ebmBus[index].ShutUpTimer))
      ebmBusReady(index);
  }
  else if (ebmBus[index].GetIndex != ebmBus[index].PutIndex)
  {
    ebmBusTransmitMessage(index, &ebmBus[index].MessageBuffer[ebmBus[index].GetIndex]);
  }

  // Check if timer expired for collecting management data
  if (TimerExpired(&ManagementTimer))
  {
    TimerReset(&ManagementTimer);
    ebmBusGetTemperatureAllNodes();
    ManagementTimerCnt++;
    if (ManagementTimerCnt >= 6) // 30 minutes
    {
      ManagementTimerCnt = 0;
      ebmBusGetManagementAllNodes();
    }
  }

  index++;
  index %= MAX_EBM_BUS;
}

//==============================================================================
//------------------------ End of file -----------------------------------------
//==============================================================================
