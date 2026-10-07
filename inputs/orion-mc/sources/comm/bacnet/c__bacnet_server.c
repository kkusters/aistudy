/**************************************************************************
*
* Copyright (C) 2006 Steve Karg <skarg@users.sourceforge.net>
*
* Permission is hereby granted, free of charge, to any person obtaining
* a copy of this software and associated documentation files (the
* "Software"), to deal in the Software without restriction, including
* without limitation the rights to use, copy, modify, merge, publish,
* distribute, sublicense, and/or sell copies of the Software, and to
* permit persons to whom the Software is furnished to do so, subject to
* the following conditions:
*
* The above copyright notice and this permission notice shall be included
* in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*
*********************************************************************/
#include <time.h>
#include "config.h"
#include "config-services.h"
#include "bacdef.h"
#include "handlers.h"
#include "bacdcode.h"
#ifdef BACFILE
#include "bacfile.h"
#endif
#include "net.h"
#include "txbuf.h"
#include "version.h"
#include "dcc.h"
#include "tsm.h"
#include "bip.h"
#include "ch_data.h"
#include "ch_bacnet_server.h"
#include "ch_bacnet_object.h"
#include "ch_timer.h"
/* include the objects */
#include "device.h"
#include "av.h"
#include "bv.h"
#include "msv.h"

static time_t last_seconds = 0;
//static TTimer Timer_Bacnet_Server_100ms;

// Functie die de functie pointers voor verschillende handlers instelt.
// Deze functie word gebruikt door Init_Objects
static void Init_Object(
    BACNET_OBJECT_TYPE object_type,
    rpm_property_lists_function rpm_list_function,
    read_property_function rp_function,
    object_valid_instance_function object_valid_function,
    write_property_function wp_function,
    object_count_function count_function,
    object_index_to_instance_function index_function,
    object_name_function name_function)
{
    handler_read_property_object_set(object_type, rp_function,
        object_valid_function);
    handler_write_property_object_set(object_type, wp_function);
#ifdef SERVICE_READ_PROPERTY_MULTIPLE
    handler_read_property_multiple_list_set(object_type, rpm_list_function);
#else
    rpm_list_function;
#endif
    Device_Object_Function_Set(object_type, count_function, index_function,
        name_function);
}

// Initialiseer alle objecten
// Deze functie word gebruikt door Bacnet_Init
static void Init_Objects(
    void)
{
    Device_Init();
    Init_Object(OBJECT_DEVICE, Device_Property_Lists,
        Device_Encode_Property_APDU, Device_Valid_Object_Instance_Number,
        Device_Write_Property, NULL, NULL, NULL);

    Analog_Value_Init();
    Init_Object(OBJECT_ANALOG_VALUE, Analog_Value_Property_Lists,
        Analog_Value_Encode_Property_APDU, Analog_Value_Valid_Instance,
        Analog_Value_Write_Property, Analog_Value_Count,
        Analog_Value_Index_To_Instance, Analog_Value_Name);
    
    Binary_Value_Init();
    Init_Object(OBJECT_BINARY_VALUE, Binary_Value_Property_Lists,
        Binary_Value_Encode_Property_APDU, Binary_Value_Valid_Instance,
        Binary_Value_Write_Property, Binary_Value_Count,
        Binary_Value_Index_To_Instance, Binary_Value_Name);
    
    Multistate_Value_Init();
    Init_Object(OBJECT_MULTI_STATE_VALUE, Multistate_Value_Property_Lists,
        Multistate_Value_Encode_Property_APDU, Multistate_Value_Valid_Instance,
        Multistate_Value_Write_Property, Multistate_Value_Count,
        Multistate_Value_Index_To_Instance, Multistate_Value_Name);
}

// Initialiseert alle Service Handlers.
// Deze functie word gebruikt door Bacnet_Init
// Het in- of uitschakelen van handlers moet in config-services.h
static void Init_Service_Handlers(
    void)
{
    /* set the handler for all the services we don't implement */
    /* It is required to send the proper reject message... */
    apdu_set_unrecognized_service_handler_handler(handler_unrecognized_service);

    /* we need to handle who-is to support dynamic device binding */
    apdu_set_unconfirmed_handler(SERVICE_UNCONFIRMED_WHO_IS, handler_who_is);
#ifdef SERVICE_WHO_HAS
    apdu_set_unconfirmed_handler(SERVICE_UNCONFIRMED_WHO_HAS, handler_who_has);
#endif

    /* Set the handlers for any confirmed services that we support. */
    /* We must implement read property - it's required! */
#ifdef SERVICE_READ_PROPERTY
    apdu_set_confirmed_handler(SERVICE_CONFIRMED_READ_PROPERTY,
        handler_read_property);
#endif
#ifdef SERVICE_READ_PROPERTY_MULTIPLE
    apdu_set_confirmed_handler(SERVICE_CONFIRMED_READ_PROP_MULTIPLE,
        handler_read_property_multiple);
#endif
#ifdef SERVICE_WRITE_PROPERTY
    apdu_set_confirmed_handler(SERVICE_CONFIRMED_WRITE_PROPERTY,
        handler_write_property);
#endif
#if defined(BACFILE)
    apdu_set_confirmed_handler(SERVICE_CONFIRMED_ATOMIC_READ_FILE,
        handler_atomic_read_file);
    apdu_set_confirmed_handler(SERVICE_CONFIRMED_ATOMIC_WRITE_FILE,
        handler_atomic_write_file);
#endif
#ifdef SERVICE_REINITIALIZE_DEVICE
    apdu_set_confirmed_handler(SERVICE_CONFIRMED_REINITIALIZE_DEVICE,
        handler_reinitialize_device);
#endif
#ifdef SERVICE_TIME_SYNCHRONIZATION
    apdu_set_unconfirmed_handler(SERVICE_UNCONFIRMED_UTC_TIME_SYNCHRONIZATION,
        handler_timesync_utc);
    apdu_set_unconfirmed_handler(SERVICE_UNCONFIRMED_TIME_SYNCHRONIZATION,
        handler_timesync);
#endif
#ifdef SERVICE_COV
    apdu_set_confirmed_handler(SERVICE_CONFIRMED_SUBSCRIBE_COV,
        handler_cov_subscribe); 

    // Deze service is om binnengekomen notifications af te handelen.
    // aangezien we alleen data aanbieden is deze dus overbodig.
    //apdu_set_unconfirmed_handler(SERVICE_UNCONFIRMED_COV_NOTIFICATION,
    //    handler_ucov_notification);
#endif
#ifdef SERVICE_DEVICE_COMMUNICATION_CONTROL
    apdu_set_confirmed_handler(SERVICE_CONFIRMED_DEVICE_COMMUNICATION_CONTROL,
        handler_device_communication_control);
#endif
}

void Bacnet_Init()
{
    // Set the Identifier for this device
    Device_Set_Object_Instance_Number((uint32_t)opt_alg.BACnet_Device_Id);

    Init_Objects();
    Init_Service_Handlers();
    
    // For the main function
    last_seconds = time(NULL);
//    TimerSet(&Timer_Bacnet_Server_100ms, TIMER_100MS);
}

void Bacnet_Reinit()
{
    Bacnet_Cleanup();

    if (opt_alg.BACnet_enabled) {
        Bacnet_Init();
        datalink_init(NULL);
    }
}

void Bacnet_Cleanup()
{
    datalink_cleanup();
}

// Functie die aangeroepen moet worden in de main loop van de applicatie
void Main_Bacnet_Server()
{
    time_t current_seconds = 0;
    uint32_t elapsed_seconds = 0;
    uint32_t elapsed_milliseconds = 0;
    int i = 0;

    current_seconds = time(NULL);
    elapsed_seconds = current_seconds - last_seconds;

    if (elapsed_seconds) {
        last_seconds = current_seconds;

        dcc_timer_seconds(elapsed_seconds);

        //elapsed_milliseconds = elapsed_seconds * 1000;
        //tsm_timer_milliseconds(elapsed_milliseconds);
    }

    // MV - Door veranderingen met het verzenden van udp data moet dit in een poll van de uip stack gebeuren
    // if (TimerExpired(&Timer_Bacnet_Server_100ms))
    // {
    //   handler_cov_task(elapsed_seconds);
    //   TimerReset(&Timer_Bacnet_Server_100ms); 
    // }
}