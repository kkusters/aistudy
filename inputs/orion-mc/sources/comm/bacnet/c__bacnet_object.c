
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ch_bacnet_object.h"
#include "ch_data.h"
#include "ch_motorgroep.h"
#include "ch_alg.h"
#include "av.h"
#include "bv.h"
#include "msv.h"
#include "bacapp.h"

// Voorbeeld opbouw Object_Name AV301
//
// AV: Object_Type, in dit geval een Analog Value. Kan ook BV (Binary Value) of MS (Multistate Value)
// 301: 301 % 100 = De waarde van het type object in dit geval CurrentPosition.
//      301 / 100 = Motorgroup nummer.
// 
// AVxxx1 = Analog Value, Current Position
// AVxxx2 = Analog Value, Desired Position
// BVxxx1 = Binary Value, Hold Position
// MSxxx1 = Multistate Value, Operation Mode
// MSxxx2 = Multistate Value, Operation State

// Prioriteit die gebruikt wordt voor het schrijven van de CurrentValue
int orion_app_priority = 15;


//*****************************************************************************
// Object Name functies
//*****************************************************************************

// Returned het Instantie nummer van een BACnet object
//
// value_id: value_id waarvan we het bijhorende Object_Type willen.
// group_nr: nummer van  de motorgroups
// return: Het object type, -1 bij fout
int Get_Instance_By_Value_Id(
    POSITION_CONTROLLER_VALUE_ID value_id,
    int position_controller_nr)
{
    switch(value_id) {
        case POSITION_CONTROLLER_CURRENT_POSITION:
            return ((position_controller_nr + 1) * 100) + 1;
        case POSITION_CONTROLLER_DESIRED_POSITION:
            return ((position_controller_nr + 1) * 100) + 2;
        case POSITION_CONTROLLER_HOLD_POSITION:
            return ((position_controller_nr + 1) * 100) + 1;
        case POSITION_CONTROLLER_OPERATION_MODE:
            return ((position_controller_nr + 1) * 100) + 1;
        case POSITION_CONTROLLER_OPERATION_STATE:
            return ((position_controller_nr + 1) * 100) + 2;
    }

    return -1;
}


//*****************************************************************************
// Get & Set object values
//*****************************************************************************

// Controleer of in het BACnet object een waarde is ingesteld via bacnet sinds
// de vorige herstart.
// 
// position_controller_index: Index van de motorgroup in de orion (beginnend bij 0)
// value_id: Id van de waarde die het object representeert
// return: True als er een waarde is ingesteld, anders false
bool Bacnet_Object_Has_Valid_Value(
	int position_controller_index,
	POSITION_CONTROLLER_VALUE_ID value_id)
{
    uint32_t instance = Get_Instance_By_Value_Id(value_id, position_controller_index);

    switch(value_id) {
        case POSITION_CONTROLLER_CURRENT_POSITION:
        case POSITION_CONTROLLER_DESIRED_POSITION:
            return Analog_Value_Has_Valid_Value(instance);
        case POSITION_CONTROLLER_HOLD_POSITION:
            return Binary_Value_Has_Valid_Value(instance);
        case POSITION_CONTROLLER_OPERATION_MODE:
        case POSITION_CONTROLLER_OPERATION_STATE:
            return Multistate_Value_Has_Valid_Value(instance);
    }

  return FALSE;
}

// Haal een waarde op van een object
// 
// position_controller_index: Index van de motorgroup in de orion (beginnend bij 0)
// value_id: Id van de waarde die het object representeert
// return: De waarde
void * Get_Bacnet_Object_Value(
    int position_controller_index,
    POSITION_CONTROLLER_VALUE_ID value_id)
{
    float real_value;
    int int_value;
    static BACNET_BINARY_PV enum_value;
    static unsigned int uint_value;
    uint32_t instance = Get_Instance_By_Value_Id(value_id, position_controller_index);
        
    switch(value_id) {
        case POSITION_CONTROLLER_CURRENT_POSITION:
        case POSITION_CONTROLLER_DESIRED_POSITION:
			real_value = Analog_Value_Present_Value(instance);
            // Controleer het limiet voor de zekerheid
            if (real_value > 100.0) {
                real_value = 100.0;
            }
            if (real_value < 0.0) {
                real_value = 0.0;
            }
			int_value = real_value * 10;
			return &int_value;  
        case POSITION_CONTROLLER_HOLD_POSITION:
            enum_value = Binary_Value_Present_Value(instance);
			return &enum_value;  
        case POSITION_CONTROLLER_OPERATION_MODE:
        case POSITION_CONTROLLER_OPERATION_STATE:
            uint_value = Multistate_Value_Present_Value(instance);
            return &uint_value; 
    }

    return NULL;
}

// Stel de waarde van een bacnet object in
// 
// position_controller_index: Index van de motorgroup in de orion (beginnend bij 0)
// value_id: Id van de waarde die het object 
// value: De waarde
// priority: De prioriteit
// return: TRUE bij succes, anders FALSE
bool Set_Bacnet_Object_Value(
    int position_controller_index,
    POSITION_CONTROLLER_VALUE_ID value_id,
    BACNET_APPLICATION_DATA_VALUE * value)
{
    bool status = false;
    uint32_t instance = Get_Instance_By_Value_Id(value_id, position_controller_index);
        
    switch(value_id) {
        case POSITION_CONTROLLER_CURRENT_POSITION:
        case POSITION_CONTROLLER_DESIRED_POSITION:
            if (value->tag == BACNET_APPLICATION_TAG_NULL) {
                value->tag = BACNET_APPLICATION_TAG_REAL;
                value->type.Real = ANALOG_LEVEL_NULL;
            }
            Analog_Value_Present_Value_Set(instance, (value->type.Real/10), orion_app_priority);
            return TRUE;
        case POSITION_CONTROLLER_HOLD_POSITION:
            if (value->tag == BACNET_APPLICATION_TAG_NULL) {
                value->tag = BACNET_APPLICATION_TAG_ENUMERATED;
                value->type.Real = BINARY_NULL;
            }
            Binary_Value_Present_Value_Set(instance, (BACNET_BINARY_PV)value->type.Enumerated, orion_app_priority);
            return TRUE;
        case POSITION_CONTROLLER_OPERATION_MODE:
        case POSITION_CONTROLLER_OPERATION_STATE:
            if (value->tag == BACNET_APPLICATION_TAG_NULL) {
                value->tag = BACNET_APPLICATION_TAG_UNSIGNED_INT;
                value->type.Real = MULTISTATE_NULL;
            }
            Multistate_Value_Present_Value_Set(instance, value->type.Unsigned_Int, orion_app_priority);
            return TRUE; 
    }
    
    return FALSE; 
}