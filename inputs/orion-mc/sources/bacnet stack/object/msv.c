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

/* Multi-state Output Objects - customize for your use */

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bacdef.h"
#include "bacdcode.h"
#include "bacenum.h"
#include "bacapp.h"
#include "config.h"     /* the custom stuff */
#include "wp.h"
#include "ch_bacnet_object.h"
#include "msv.h"

/* When all the priorities are level null, the present value returns */
/* the Relinquish Default value */
#define MULTISTATE_RELINQUISH_DEFAULT 1
/* how many states? 0-253 is 254 states */
#define MULTISTATE_NUMBER_OF_STATES (8)
#define MULTISTATE_NUMBER_OF_STATES_OPERATION_MODE (6)
#define MULTISTATE_NUMBER_OF_STATES_OPERATION_STATE (8)

#pragma noclear
/* Here is our Priority Array.*/
static uint8_t Multistate_Value_Level[MAX_MULTISTATE_VALUES][BACNET_MAX_PRIORITY];
/* Writable out-of-service allows others to play with our Present Value */
/* without changing the physical output */
static bool Multistate_Value_Out_Of_Service[MAX_MULTISTATE_VALUES];
/* The State Text. There are 2 arrays because we have 2 kinds of Multistate Values */
BACNET_CHARACTER_STRING Multistate_Value_State_Text_Operation_Mode[MULTISTATE_NUMBER_OF_STATES_OPERATION_MODE];
BACNET_CHARACTER_STRING Multistate_Value_State_Text_Operation_State[MULTISTATE_NUMBER_OF_STATES_OPERATION_STATE];
/* For Change of Value reporting */
static uint8_t Multistate_Value_Previous_Level[MAX_MULTISTATE_VALUES];
bool Multistate_Value_Change_Of_Value_Flag[MAX_MULTISTATE_VALUES];
/* Flag to check if a value is set since init */
bool Multistate_Value_Valid_Value[MAX_MULTISTATE_VALUES];
#pragma clear

/* These three arrays are used by the ReadPropertyMultiple handler */
static const int Multistate_Value_Properties_Required[] = {
    PROP_OBJECT_IDENTIFIER,
    PROP_OBJECT_NAME,
    PROP_OBJECT_TYPE,
    PROP_PRESENT_VALUE,
    PROP_STATUS_FLAGS,
    PROP_EVENT_STATE,
    PROP_OUT_OF_SERVICE,
    PROP_NUMBER_OF_STATES,
    PROP_PRIORITY_ARRAY,
    PROP_RELINQUISH_DEFAULT,
    -1
};
static const int Multistate_Value_Properties_Optional[] = {
    PROP_DESCRIPTION,
    PROP_STATE_TEXT,
    -1
};
static const int Multistate_Value_Properties_Proprietary[] = {
    -1
};

void Multistate_Value_Property_Lists(
    const int **pRequired,
    const int **pOptional,
    const int **pProprietary)
{
    if (pRequired)
        *pRequired = Multistate_Value_Properties_Required;
    if (pOptional)
        *pOptional = Multistate_Value_Properties_Optional;
    if (pProprietary)
        *pProprietary = Multistate_Value_Properties_Proprietary;

    return;
}

void Multistate_Value_Init(
    void)
{
    unsigned i, j;
    static bool initialized = false;

    if (!initialized) {
        initialized = true;

        /* initialize all the analog output priority arrays to NULL */
        for (i = 0; i < MAX_MULTISTATE_VALUES; i++) {
            for (j = 0; j < BACNET_MAX_PRIORITY; j++) {
                Multistate_Value_Level[i][j] = MULTISTATE_NULL;
            }

            // Init OperationMode values op NotAvailable omdat uitgeschakelde groepen niet ingesteld worden.
            // Een default waarde van 1 voor OperationState is wel goed.
            if ((i % 2) == 0)
                Multistate_Value_Level[i][orion_app_priority] = 6;

            Multistate_Value_Previous_Level[i] = MULTISTATE_RELINQUISH_DEFAULT;
            Multistate_Value_Change_Of_Value_Flag[i] = false;
            Multistate_Value_Out_Of_Service[i] = false;
            Multistate_Value_Valid_Value[i] = false;
        }

        /* Init the state texts */
        characterstring_init_ansi(&Multistate_Value_State_Text_Operation_Mode[1], "NormalOperation");
        characterstring_init_ansi(&Multistate_Value_State_Text_Operation_Mode[2], "OutOfOrder");
        characterstring_init_ansi(&Multistate_Value_State_Text_Operation_Mode[3], "Fault");
        characterstring_init_ansi(&Multistate_Value_State_Text_Operation_Mode[4], "Manual");
        characterstring_init_ansi(&Multistate_Value_State_Text_Operation_Mode[5], "InMaintenance");
        characterstring_init_ansi(&Multistate_Value_State_Text_Operation_Mode[6], "NotAvailable");
        
        characterstring_init_ansi(&Multistate_Value_State_Text_Operation_State[1], "AtPosition");
        characterstring_init_ansi(&Multistate_Value_State_Text_Operation_State[2], "GoingDown");
        characterstring_init_ansi(&Multistate_Value_State_Text_Operation_State[3], "GoingUp");
        characterstring_init_ansi(&Multistate_Value_State_Text_Operation_State[4], "AtLowLimit");
        characterstring_init_ansi(&Multistate_Value_State_Text_Operation_State[5], "AtHighLimit");
        characterstring_init_ansi(&Multistate_Value_State_Text_Operation_State[6], "Holding");
        characterstring_init_ansi(&Multistate_Value_State_Text_Operation_State[7], "Held");
        characterstring_init_ansi(&Multistate_Value_State_Text_Operation_State[8], "Restarting");
    }
}

/* we simply have 0-n object instances.  Yours might be */
/* more complex, and then you need validate that the */
/* given instance exists */
bool Multistate_Value_Valid_Instance(
    uint32_t object_instance)
{
    int group;
	int value;

    group = object_instance / 100;
	value = object_instance % 100;
    if ((group > 0 && group <= MAX_MULTISTATE_VALUES / 2) && (value >= 1 && value <= 2))
        return true;

    return false;
}

/* we simply have 0-n object instances.  Yours might be */
/* more complex, and then count how many you have */
unsigned Multistate_Value_Count(
    void)
{
    return MAX_MULTISTATE_VALUES;
}

/* we simply have 0-n object instances.  Yours might be */
/* more complex, and then you need to return the instance */
/* that correlates to the correct index */
uint32_t Multistate_Value_Index_To_Instance(
    unsigned index)
{
    int position_controller_nr = index / 2;
    int value_nr = index % 2;
    int instance = ((position_controller_nr + 1) * 100);

    if (value_nr == 0) { // operation mode
        instance += 1;
    } else if (value_nr == 1) { // operation state
        instance += 2;
    }

    return instance;
}

/* we simply have 0-n object instances.  Yours might be */
/* more complex, and then you need to return the index */
/* that correlates to the correct instance number */
unsigned Multistate_Value_Instance_To_Index(
    uint32_t object_instance)
{
    int a;
    int b;
    a = (object_instance % 100) - 1;
    b = (((object_instance - a) / 100) - 1) * 2;
    return a + b;
}

/* note: the object name must be unique within this device */
char *Multistate_Value_Name(
    uint32_t object_instance)
{
    static char str[20] = "";

    if (object_instance % 100 == 1) { // operation mode
        sprintf( str, "OperationMode %d", (object_instance / 100) );
    } else if (object_instance % 100 == 2) { // operation state
        sprintf( str, "OperationState %d", (object_instance / 100) );
    }

    return str;
}

uint32_t Multistate_Value_Name_To_Instance(
    char *name)
{
    char * numberStr;
	int number;
    uint32_t instance = 0;

    numberStr = _fstrchr((const char far *)name, ' ');
	numberStr++;
	number = atoi(numberStr);
    if (_fstrstr((const char far *)name, (const char far *)"OperationMode") != NULL)
	{
		instance = (100 * number) + 1;
	}
	else if (_fstrstr((const char far *)name, (const char far *)"OperationState") != NULL)
	{
		instance = (100 * number) + 2;
	}
    
    return instance;
}

bool Multistate_Value_Present_Value_Set(
    uint32_t object_instance,
    uint8_t value,
    uint8_t priority)
{
    unsigned index = 0;
    bool status = false;

    Multistate_Value_Init();
    index = Multistate_Value_Instance_To_Index(object_instance);
    if (index < MAX_MULTISTATE_VALUES && !Multistate_Value_Out_Of_Service[index]) {
        if (priority && (priority <= BACNET_MAX_PRIORITY) &&
            (priority != 6 ) && (value <= MULTISTATE_NUMBER_OF_STATES)) {
            Multistate_Value_Level[index][priority] = value;
            status = true;
        }

        if (Multistate_Value_Previous_Level[index] != Multistate_Value_Present_Value(object_instance))
            Multistate_Value_Change_Of_Value_Flag[index] = true;
    }           

    return status;
}

uint32_t Multistate_Value_Present_Value(
    uint32_t object_instance)
{
    uint32_t value = MULTISTATE_RELINQUISH_DEFAULT;
    unsigned index = 0;
    unsigned i = 0;

    Multistate_Value_Init();
    index = Multistate_Value_Instance_To_Index(object_instance);
    if (index < MAX_MULTISTATE_VALUES) {
        for (i = 0; i < BACNET_MAX_PRIORITY; i++) {
            if (Multistate_Value_Level[index][i] != MULTISTATE_NULL) {
                value = Multistate_Value_Level[index][i];
                break;
            }
        }
    }

    return value;
} 

bool Multistate_Value_Change_Of_Value(
    uint32_t object_instance)
{
    bool status = false;
    unsigned index;

    index = Multistate_Value_Instance_To_Index(object_instance);

    if (index < MAX_MULTISTATE_VALUES) {
        status = Multistate_Value_Change_Of_Value_Flag[index];
    }

    return status;
}

void Multistate_Value_Change_Of_Value_Clear(
    uint32_t object_instance)
{
    unsigned index;

    index = Multistate_Value_Instance_To_Index(object_instance);
    if (index < MAX_MULTISTATE_VALUES) {
        Multistate_Value_Change_Of_Value_Flag[index] = false;
		Multistate_Value_Previous_Level[index] = Multistate_Value_Present_Value(object_instance);
    }
}

bool Multistate_Value_Has_Valid_Value(
    uint32_t object_instance)
{
    return Multistate_Value_Valid_Value[Multistate_Value_Instance_To_Index(object_instance)];
}

void Multistate_Value_State_Text(
    uint32_t object_instance,
    BACNET_CHARACTER_STRING * char_string)
{

    unsigned index = 0;
    unsigned state = 0;

    Multistate_Value_Init();
    index = Multistate_Value_Instance_To_Index(object_instance);
    if (index < MAX_MULTISTATE_VALUES) {
        state = Multistate_Value_Present_Value(object_instance);
        if (object_instance % 100 == 1) { // operation mode
            *char_string = Multistate_Value_State_Text_Operation_Mode[state];
        } else if (object_instance % 100 == 2) { // operation state
            *char_string = Multistate_Value_State_Text_Operation_State[state];
        }
    }
}


BACNET_CHARACTER_STRING Multistate_Value_Description(
    uint32_t object_instance)
{
    BACNET_CHARACTER_STRING return_string;
    char string_value[60];

    if (object_instance % 100 == 1) {
        sprintf( string_value, "The operation mode of position controller %d.", (object_instance / 100) );
    } else if (object_instance % 100 == 2) {
        sprintf( string_value, "The operation state of position controller %d.", (object_instance / 100) );
    } else
	string_value[0] = 0;

    characterstring_init_ansi(&return_string, &string_value[0]);

    return return_string;
}

bool Multistate_Value_Encode_Value_List(
    uint32_t object_instance,
    BACNET_PROPERTY_VALUE * value_list)
{
    value_list->propertyIdentifier = PROP_PRESENT_VALUE;
    value_list->propertyArrayIndex = BACNET_ARRAY_ALL;
    value_list->value.context_specific = false;
    value_list->value.tag = BACNET_APPLICATION_TAG_UNSIGNED_INT;
    value_list->value.type.Unsigned_Int =
        Multistate_Value_Present_Value(object_instance);
    value_list->priority = BACNET_NO_PRIORITY;

    value_list = value_list->next;

    value_list->propertyIdentifier = PROP_STATUS_FLAGS;
    value_list->propertyArrayIndex = BACNET_ARRAY_ALL;
    value_list->value.context_specific = false;
    value_list->value.tag = BACNET_APPLICATION_TAG_BIT_STRING;
    bitstring_init((BACNET_BIT_STRING *)&value_list->value.type.Bit_String);
    bitstring_set_bit((BACNET_BIT_STRING *)&value_list->value.type.Bit_String, STATUS_FLAG_IN_ALARM,
        false);
    bitstring_set_bit((BACNET_BIT_STRING *)&value_list->value.type.Bit_String, STATUS_FLAG_FAULT,
        false);
    bitstring_set_bit((BACNET_BIT_STRING *)&value_list->value.type.Bit_String,
        STATUS_FLAG_OVERRIDDEN, false);
    if (Multistate_Value_Out_Of_Service[object_instance]) {
        bitstring_set_bit((BACNET_BIT_STRING *)&value_list->value.type.Bit_String,
            STATUS_FLAG_OUT_OF_SERVICE, true);
    } else {
        bitstring_set_bit((BACNET_BIT_STRING *)&value_list->value.type.Bit_String,
            STATUS_FLAG_OUT_OF_SERVICE, false);
    }
    value_list->priority = BACNET_NO_PRIORITY;

    return true;
}

/* return apdu len, or -1 on error */
int Multistate_Value_Encode_Property_APDU(
    uint8_t * apdu,
    uint32_t object_instance,
    BACNET_PROPERTY_ID property,
    int32_t array_index,
    BACNET_ERROR_CLASS * error_class,
    BACNET_ERROR_CODE * error_code)
{
    int len = 0;
    int apdu_len = 0;   /* return value */
    BACNET_BIT_STRING bit_string;
    BACNET_CHARACTER_STRING char_string;
    uint32_t present_value = 0;
    unsigned object_index = 0;
    unsigned i = 0;
    bool state = false;

    Multistate_Value_Init();
    switch (property) {
        case PROP_OBJECT_IDENTIFIER:
            apdu_len =
                encode_application_object_id(&apdu[0],
                OBJECT_MULTI_STATE_VALUE, object_instance);
            break;
            /* note: Name and Description don't have to be the same.
               You could make Description writable and different */
        case PROP_DESCRIPTION:
            char_string = Multistate_Value_Description(object_instance);
            apdu_len = encode_application_character_string(&apdu[0], &char_string);
            break;
        case PROP_OBJECT_NAME:
            characterstring_init_ansi(&char_string,
                Multistate_Value_Name(object_instance));
            apdu_len =
                encode_application_character_string(&apdu[0], &char_string);
            break;
        case PROP_OBJECT_TYPE:
            apdu_len =
                encode_application_enumerated(&apdu[0],
                OBJECT_MULTI_STATE_VALUE);
            break;
        case PROP_PRESENT_VALUE:
            present_value = Multistate_Value_Present_Value(object_instance);
            apdu_len = encode_application_unsigned(&apdu[0], present_value);
            break;
        case PROP_STATUS_FLAGS:
            object_index = Multistate_Value_Instance_To_Index(object_instance);
            bitstring_init(&bit_string);
            bitstring_set_bit(&bit_string, STATUS_FLAG_IN_ALARM, false);
            bitstring_set_bit(&bit_string, STATUS_FLAG_FAULT, false);
            bitstring_set_bit(&bit_string, STATUS_FLAG_OVERRIDDEN, false);
            bitstring_set_bit(&bit_string, STATUS_FLAG_OUT_OF_SERVICE, Multistate_Value_Out_Of_Service[object_index]);
            apdu_len = encode_application_bitstring(&apdu[0], &bit_string);
            break;
        case PROP_EVENT_STATE:
            /* note: see the details in the standard on how to use this */
            apdu_len =
                encode_application_enumerated(&apdu[0], EVENT_STATE_NORMAL);
            break;
        case PROP_OUT_OF_SERVICE:
            object_index =
                Multistate_Value_Instance_To_Index(object_instance);
            state = Multistate_Value_Out_Of_Service[object_index];
            apdu_len = encode_application_boolean(&apdu[0], state);
            break;
        case PROP_PRIORITY_ARRAY:
            /* Array element zero is the number of elements in the array */
            if (array_index == 0)
                apdu_len =
                    encode_application_unsigned(&apdu[0], BACNET_MAX_PRIORITY);
            /* if no index was specified, then try to encode the entire list */
            /* into one packet. */
            else if (array_index == BACNET_ARRAY_ALL) {
                object_index =
                    Multistate_Value_Instance_To_Index(object_instance);
                for (i = 0; i < BACNET_MAX_PRIORITY; i++) {
                    /* FIXME: check if we have room before adding it to APDU */
                    if (Multistate_Value_Level[object_index][i] ==
                        MULTISTATE_NULL)
                        len = encode_application_null(&apdu[apdu_len]);
                    else {
                        present_value =
                            Multistate_Value_Level[object_index][i];
                        len =
                            encode_application_unsigned(&apdu[apdu_len],
                            present_value);
                    }
                    /* add it if we have room */
                    if ((apdu_len + len) < MAX_APDU)
                        apdu_len += len;
                    else {
                        *error_class = ERROR_CLASS_SERVICES;
                        *error_code = ERROR_CODE_NO_SPACE_FOR_OBJECT;
                        apdu_len = -1;
                        break;
                    }
                }
            } else {
                object_index =
                    Multistate_Value_Instance_To_Index(object_instance);
                if (array_index <= BACNET_MAX_PRIORITY) {
                    if (Multistate_Value_Level[object_index][array_index -
                            1] == MULTISTATE_NULL)
                        apdu_len = encode_application_null(&apdu[0]);
                    else {
                        present_value =
                            Multistate_Value_Level[object_index][array_index -
                            1];
                        apdu_len =
                            encode_application_unsigned(&apdu[0],
                            present_value);
                    }
                } else {
                    *error_class = ERROR_CLASS_PROPERTY;
                    *error_code = ERROR_CODE_INVALID_ARRAY_INDEX;
                    apdu_len = -1;
                }
            }

            break;
        case PROP_RELINQUISH_DEFAULT:
            present_value = MULTISTATE_RELINQUISH_DEFAULT;
            apdu_len = encode_application_enumerated(&apdu[0], present_value);
            break;
        case PROP_NUMBER_OF_STATES:
            switch(object_instance % 100) {
                case 1: // operation mode
                    apdu_len =
                        encode_application_unsigned(&apdu[apdu_len],
                        MULTISTATE_NUMBER_OF_STATES_OPERATION_MODE);
                    break;
                case 2: // operation state                   
                    apdu_len =
                        encode_application_unsigned(&apdu[apdu_len],
                        MULTISTATE_NUMBER_OF_STATES_OPERATION_STATE);
                    break;
                default:
                    apdu_len =
                        encode_application_unsigned(&apdu[apdu_len],
                        MULTISTATE_NUMBER_OF_STATES);
                    break;
            }
            break;
        case PROP_STATE_TEXT:
            Multistate_Value_State_Text(object_instance, &char_string);
            apdu_len = encode_application_character_string(
                &apdu[apdu_len], &char_string);
            break;
        default:
            *error_class = ERROR_CLASS_PROPERTY;
            *error_code = ERROR_CODE_UNKNOWN_PROPERTY;
            apdu_len = -1;
            break;
    }

    return apdu_len;
}

/* returns true if successful */
bool Multistate_Value_Write_Property(
    BACNET_WRITE_PROPERTY_DATA * wp_data,
    BACNET_ERROR_CLASS * error_class,
    BACNET_ERROR_CODE * error_code)
{
    bool status = false;        /* return value */
    unsigned int object_index = 0;
    unsigned int priority = 0;
    uint32_t level = 0;
    int len = 0;
    BACNET_APPLICATION_DATA_VALUE value;

    Multistate_Value_Init();
    if (!Multistate_Value_Valid_Instance(wp_data->object_instance)) {
        *error_class = ERROR_CLASS_OBJECT;
        *error_code = ERROR_CODE_UNKNOWN_OBJECT;
        return false;
    }
    /* decode the some of the request */
    len =
        bacapp_decode_application_data(wp_data->application_data,
        wp_data->application_data_len, &value);
    /* FIXME: len < application_data_len: more data? */
    /* FIXME: len == 0: unable to decode? */
    switch (wp_data->object_property) {
        /*case PROP_PRESENT_VALUE:
            if (value.tag == BACNET_APPLICATION_TAG_UNSIGNED_INT ||
                value.tag == BACNET_APPLICATION_TAG_NULL) {
                priority = wp_data->priority;
                object_index = Multistate_Value_Instance_To_Index(wp_data->object_instance);
                if (value.tag == BACNET_APPLICATION_TAG_NULL) {
                    level = MULTISTATE_NULL;
                } else {
                    level = value.type.Unsigned_Int;
                }
                if (Multistate_Value_Present_Value_Set(wp_data->object_instance, 
                    (uint8_t)value.type.Unsigned_Int, priority)) {
                    status = true;
                } else if (priority == 6 || Multistate_Value_Out_Of_Service[index]) {
                    *error_class = ERROR_CLASS_PROPERTY;
                    *error_code = ERROR_CODE_WRITE_ACCESS_DENIED;
                } else {
                    *error_class = ERROR_CLASS_PROPERTY;
                    *error_code = ERROR_CODE_VALUE_OUT_OF_RANGE;
                }
            } else {
                *error_class = ERROR_CLASS_PROPERTY;
                *error_code = ERROR_CODE_INVALID_DATA_TYPE;
            }
            break;*/
        case PROP_OUT_OF_SERVICE:
            if (value.tag == BACNET_APPLICATION_TAG_BOOLEAN) {
                object_index =
                    Multistate_Value_Instance_To_Index
                    (wp_data->object_instance);
                Multistate_Value_Out_Of_Service[object_index] =
                    value.type.Boolean;
                status = true;
            } else {
                *error_class = ERROR_CLASS_PROPERTY;
                *error_code = ERROR_CODE_INVALID_DATA_TYPE;
            }
            break;
        default:
            *error_class = ERROR_CLASS_PROPERTY;
            *error_code = ERROR_CODE_WRITE_ACCESS_DENIED;
            break;
    }

    return status;
}


#ifdef TEST
#include <assert.h>
#include <string.h>
#include "ctest.h"

void testMultistateValue(
    Test * pTest)
{
    uint8_t apdu[MAX_APDU] = { 0 };
    int len = 0;
    uint32_t len_value = 0;
    uint8_t tag_number = 0;
    BACNET_OBJECT_TYPE decoded_type = OBJECT_MULTI_STATE_VALUE;
    uint32_t decoded_instance = 0;
    uint32_t instance = 123;
    BACNET_ERROR_CLASS error_class;
    BACNET_ERROR_CODE error_code;


    len =
        Multistate_Value_Encode_Property_APDU(&apdu[0], instance,
        PROP_OBJECT_IDENTIFIER, BACNET_ARRAY_ALL, &error_class, &error_code);
    ct_test(pTest, len != 0);
    len = decode_tag_number_and_value(&apdu[0], &tag_number, &len_value);
    ct_test(pTest, tag_number == BACNET_APPLICATION_TAG_OBJECT_ID);
    len =
        decode_object_id(&apdu[len], (int *) &decoded_type, &decoded_instance);
    ct_test(pTest, decoded_type == OBJECT_MULTI_STATE_VALUE);
    ct_test(pTest, decoded_instance == instance);

    return;
}

#ifdef TEST_MULTISTATE_VALUE
int main(
    void)
{
    Test *pTest;
    bool rc;

    pTest = ct_create("BACnet Multi-state Value", NULL);
    /* individual tests */
    rc = ct_addTestFunction(pTest, testMultistateValue);
    assert(rc);

    ct_setStream(pTest, stdout);
    ct_run(pTest);
    (void) ct_report(pTest);
    ct_destroy(pTest);

    return 0;
}
#endif /* TEST_MULTISTATE_VALUE*/
#endif /* TEST */
