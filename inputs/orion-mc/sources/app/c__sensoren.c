// C__SENSOREN.C  

#include "ch_define.h"

#include "ch_alarm.h"
#include "ch_alg.h"
#include "ch_ds301.h"
#include "ch_IO.h"
#include "ch_mb_device.h" 
#include "ch_timer.h"
#include "ch_sensoren.h"


//==============================================================================
//------------------------ Sensoren - Globals ----------------------------------
//==============================================================================
//------------------------------------------------------------------------------
static TTimer Timer_100ms;

//==============================================================================
//------------------------ Device - Init ---------------------------------------
//==============================================================================
//------------------------------------------------------------------------------
void InitSensoren(void)
{
  TimerSet(&Timer_100ms, TIMER_100MS);
}

//==============================================================================
//------------------------ Device - Main ---------------------------------------
//==============================================================================
//------------------------------------------------------------------------------
static unsigned char DeviceAddressLegal(int DeviceAddress)
{
  if ((DeviceAddress > 0) && (DeviceAddress < 248))
    return (1);
  else
    return (0);
}

void MainSensoren(void)
{
int DeviceAddress;
int i;

  if (TimerExpired(&Timer_100ms))
  {
    for (i = 0; i < opt_app.Sensoren.Drukverschil; i++)
	{
	  DeviceAddress = opt_app.Sensoren.FirstAddress + i;
	  if (DeviceAddressLegal(DeviceAddress))
	  {
	    mbDeviceCreateAlarm(DeviceAddress, DRUKVERSCHIL_MB_AL, i, i + 1, 0);
        if (!mbDeviceConnected(DeviceAddress))
        {
	      mbDeviceConnect(DeviceAddress, dtDptMod, opt_app.Sensoren.RS485Bus, MAX_SENSOREN);
		}
		else
		{
	      val_hr_alg.Sensoren.Drukverschil[i] = mbDeviceGetActualValue(DeviceAddress);
		}
	  }
	}
  }
}

//==============================================================================
//------------------------ End of file -----------------------------------------
//==============================================================================
