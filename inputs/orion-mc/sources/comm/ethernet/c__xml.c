// C__XML.C  

#include "ch_define.h"

#include "ch_mb_device.h"
#include "ch_motorgroep.h"
#include "ch_device.h"
#include "ch_xml.h"

void XML_GetData_Vent_Group(unsigned char i, TXML *XML)
{
int  DeviceAddress;
long SomPosition = 0;
int  AantalPosition = 0;
long SomRpm = 0;
int  AantalRpm = 0;
int  AantalError = 0;
unsigned int SomEnergy = 0;
TDevice *pDevice;

  pDevice = Motorgroup[i].FirstMotor;
  while (pDevice != NULL)
  {
    DeviceAddress = opt_app.Motorgroup[i].FirstAddress + (pDevice->Number + 1 - opt_app.Motorgroup[i].FirstNumber);

    if (alarm_hr_alg.Device[pDevice->Number].TargetNotReached || mbDeviceAlarmActive(DeviceAddress))
	{
	  AantalError++;
	}
	else
    {
      if (val_hr_alg.Device[pDevice->Number].OperationMode == omAuto)
	  {
	    SomPosition += val_hr_alg.Device[pDevice->Number].ActualValue;
	    AantalPosition++;

		SomRpm += (((long)val_hr_alg.Device[pDevice->Number].ActualValue * mbDeviceGetMaximumRpm(DeviceAddress)) / 100);
		AantalRpm++;
	  }
	}

    SomEnergy += mbDeviceGetEnergyConsumption(DeviceAddress);

    pDevice = pDevice->Next;
  }

  if (AantalPosition > 0)
    XML->Vent.Position = SomPosition / AantalPosition;
  else
    XML->Vent.Position = 0;

  if (AantalRpm > 0)
    XML->Vent.Rpm = SomRpm / AantalRpm;
  else
    XML->Vent.Rpm = 0;

  XML->Vent.EnergyConsumption = SomEnergy;

  if (AantalError >= setp_alg.Ventgroup[i].AlarmUrgent)
    XML->Vent.ErrorUrgent = 1;
  else
    XML->Vent.ErrorUrgent = 0;

  if (AantalError > 0)
    XML->Vent.ErrorNotUrgent = 1;
  else
    XML->Vent.ErrorNotUrgent = 0;

  if (val_hr_alg.Motorgroup[i].OperationMode != omAuto)
    XML->Vent.Status = XML_VENT_STATUS_NOT_AVAILABLE;
  else if (val_hr_alg.Motorgroup[i].PositionPerc == 0)
    XML->Vent.Status = XML_VENT_STATUS_OFF;
  else
    XML->Vent.Status = XML_VENT_STATUS_ON;
}

int XML_GetData_Vent(unsigned char i, TXML *XML)
{
int DeviceAddress;
int group;

  group = opt_app.Device[i].GroupNumber - 1;
  if ((group < 0) || (group > MAX_GROUP))
    return (0);

  DeviceAddress = opt_app.Motorgroup[group].FirstAddress + (i + 1 - opt_app.Motorgroup[group].FirstNumber);

  XML->Vent.Position          = val_hr_alg.Device[i].ActualValue;
  XML->Vent.Rpm               = ((long)val_hr_alg.Device[i].ActualValue * mbDeviceGetMaximumRpm(DeviceAddress)) / 100;
  XML->Vent.EnergyConsumption = mbDeviceGetEnergyConsumption(DeviceAddress);

  if (alarm_hr_alg.Device[i].TargetNotReached || mbDeviceAlarmActive(DeviceAddress))
    XML->Vent.ErrorNotUrgent = XML->Vent.ErrorUrgent = 1;
  else
    XML->Vent.ErrorNotUrgent = XML->Vent.ErrorUrgent = 0;

  if (mbDeviceLostCommunication(DeviceAddress) || (val_hr_alg.Device[i].OperationMode != omAuto))
	XML->Vent.Status = XML_VENT_STATUS_NOT_AVAILABLE;
  else if (val_hr_alg.Device[i].TargetValue == 0)
	XML->Vent.Status = XML_VENT_STATUS_OFF;
  else if (val_hr_alg.Device[i].ActualValue > 0)
	XML->Vent.Status = XML_VENT_STATUS_ON;
  else
	XML->Vent.Status = XML_VENT_STATUS_ACTIVE;

  return (DeviceAddress);
}


void XML_GetData_Klep_Group(unsigned char i, TXML *XML)
{
int  DeviceAddress;
long SomPosition = 0;
int  AantalPosition = 0;
int  AantalLimitswNotClose = 0;
int  AantalError = 0;
int  AantalErrorLimitswitch = 0;

unsigned int SomEnergy = 0;
TDevice *pDevice;

  pDevice = Motorgroup[i].FirstMotor;
  while (pDevice != NULL)
  {
    DeviceAddress = opt_app.Motorgroup[i].FirstAddress + (pDevice->Number + 1 - opt_app.Motorgroup[i].FirstNumber);

    if (alarm_hr_alg.Device[pDevice->Number].TargetNotReached || mbDeviceAlarmActive(DeviceAddress))
	{
	  AantalError++;
	}
	else
    {
      if (val_hr_alg.Device[pDevice->Number].OperationMode == omAuto)
	  {
	    SomPosition += val_hr_alg.Device[pDevice->Number].ActualValue;
	    AantalPosition++;

        switch (opt_app.Motorgroup[i].SensorType)
        {
          case SENSOR_TYPE_GEEN:			                                                                           break;
          case SENSOR_TYPE_EINDSCHAKELAAR:  AantalLimitswNotClose += (mbDeviceGetSensorValue(DeviceAddress)) ? 0 : 1;  break;
          case SENSOR_TYPE_EXTERN_ALARM:	                                                                           break;
        }

        AantalErrorLimitswitch += (alarm_hr_alg.Device[pDevice->Number].LimitSwitch) ? 1 : 0;
        AantalError            += (alarm_hr_alg.Device[pDevice->Number].ExternAlarm) ? 1 : 0;
	  }
	}
    pDevice = pDevice->Next;
  }

  if (AantalPosition > 0)
    XML->Klep.Position = SomPosition / AantalPosition;
  else
    XML->Klep.Position = 0;

  XML->Klep.Limitswitch      = (AantalLimitswNotClose > 0) ? 0 : 1;
  XML->Klep.ErrorLimitswitch = (AantalErrorLimitswitch > 0) ? 1 : 0;
  XML->Klep.Error            = (AantalError > 0)            ? 1 : 0;

  if (val_hr_alg.Motorgroup[i].OperationMode != omAuto)
    XML->Klep.Motorstatus = XML_KLEP_STATUS_NOT_AVAILABLE;
  else if (val_hr_alg.Motorgroup[i].PositionPerc == 0)
    XML->Klep.Motorstatus = XML_KLEP_STATUS_OFF;
  else
    XML->Klep.Motorstatus = XML_KLEP_STATUS_ON;
}

int XML_GetData_Klep(unsigned char i, TXML *XML)
{
int DeviceAddress;
int group;

  group = opt_app.Device[i].GroupNumber - 1;
  if ((group < 0) || (group > MAX_GROUP))
    return (0);

  DeviceAddress = opt_app.Motorgroup[group].FirstAddress + (i + 1 - opt_app.Motorgroup[group].FirstNumber);

  XML->Klep.Position    = val_hr_alg.Device[i].ActualValue;
  switch (opt_app.Motorgroup[group].SensorType)
  {
    case SENSOR_TYPE_GEEN:			   XML->Klep.Limitswitch = 0;                                      break;
    case SENSOR_TYPE_EINDSCHAKELAAR:   XML->Klep.Limitswitch = mbDeviceGetSensorValue(DeviceAddress);  break;
    case SENSOR_TYPE_EXTERN_ALARM:	   XML->Klep.Limitswitch = 0;                                      break;
    default:                      	   XML->Klep.Limitswitch = 0;                                      break;
  }
  XML->Klep.Limitswitch = mbDeviceGetSensorValue(DeviceAddress);
  XML->Klep.ErrorLimitswitch = (alarm_hr_alg.Device[i].LimitSwitch) ? 1 : 0;
  XML->Klep.Error            = (alarm_hr_alg.Device[i].ExternAlarm) ? 1 : 0;

  if (mbDeviceLostCommunication(DeviceAddress) || (val_hr_alg.Device[i].OperationMode != omAuto))
	XML->Klep.Motorstatus = XML_KLEP_STATUS_NOT_AVAILABLE;
  else if (val_hr_alg.Device[i].TargetValue == 0)
	XML->Klep.Motorstatus = XML_KLEP_STATUS_OFF;
  else if (val_hr_alg.Device[i].ActualValue > 0)
	XML->Klep.Motorstatus = XML_KLEP_STATUS_ON;
  else
	XML->Klep.Motorstatus = XML_KLEP_STATUS_ACTIVE;

  return (DeviceAddress);
}


