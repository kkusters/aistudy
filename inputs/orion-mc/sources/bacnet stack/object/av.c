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

/* Analog Value Objects - customize for your use */

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bacdef.h"
#include "bacdcode.h"
#include "bacenum.h"
#include "bacapp.h"
#include "bacstr.h"
#include "config.h"     /* the custom stuff */
#include "wp.h"
#include "av.h"
#include "ch_bacnet_object.h"

/* When all the priorities are level null, the present value returns */
/* the Relinquish Default value */
#define ANALOG_RELINQUISH_DEFAULT 0

#pragma noclear
/* Here is our Priority Array.  They are supposed to be Real, but */
/* we don't have that kind of memory, so we will use a single byte */
/* and load a Real for returning the value when asked. */
static float Analog_Value_Level[MAX_ANALOG_VALUES][BACNET_MAX_PRIORITY];
/* Writable out-of-service allows others to play with our Present Value */
/* without changing the physical output */
static bool Analog_Value_Out_Of_Service_Value[MAX_ANALOG_VALUES];
/* Array to store the COV_Increment property for each instance. */
static float Analog_Value_COV_Increment[MAX_ANALOG_VALUES];
/* For Change of Value reporting */
static float Analog_Value_Previous_Level[MAX_ANALOG_VALUES];
static bool Analog_Value_Change_Of_Value_Flag[MAX_ANALOG_VALUES];
/* we need to have our arrays initialized before answering any calls */
static bool Analog_Value_Initialized = false;
/* Flag to check if a value has been set since init */
bool Analog_Value_Valid_Value[MAX_ANALOG_VALUES];
#pragma clear

/* These three arrays are used by the ReadPropertyMultiple handler */
static const int Analog_Value_Properties_Required[] = {
    PROP_OBJECT_IDENTIFIER,
    PROP_OBJECT_NAME,
    PROP_OBJECT_TYPE,
    PROP_PRESENT_VALUE,
    PROP_STATUS_FLAGS,
    PROP_EVENT_STATE,
    PROP_OUT_OF_SERVICE,
    PROP_UNITS,
    -1
};
static const int Analog_Value_Properties_Optional[] = {
    PROP_DESCRIPTION,
    PROP_PRIORITY_ARRAY,
    PROP_RELINQUISH_DEFAULT,
    PROP_COV_INCREMENT,
    -1
};
static const int Analog_Value_Properties_Proprietary[] = {
    -1
};

void Analog_Value_Property_Lists(
    const int **pRequired,
    const int **pOptional,
    const int **pProprietary)
{
    if (pRequired)
        *pRequired = Analog_Value_Properties_Required;
    if (pOptional)
        *pOptional = Analog_Value_Properties_Optional;
    if (pProprietary)
        *pProprietary = Analog_Value_Properties_Proprietary;

    return;
}

void Analog_Value_Init(
    void)
{
    unsigned i, j;

    if (!Analog_Value_Initialized) {
        Analog_Value_Initialized = true;

        /* initialize all the analog output priority arrays to NULL */
        for (i = 0; i < MAX_ANALOG_VALUES; i++) {
            for (j = 0; j < BACNET_MAX_PRIORITY; j++) {
                Analog_Value_Level[i][j] = ANALOG_LEVEL_NULL;
            }
            Analog_Value_Out_Of_Service_Value[i] = FALSE;
            Analog_Value_COV_Increment[i] = 1.0;
            Analog_Value_Previous_Level[i] = ANALOG_RELINQUISH_DEFAULT;
            Analog_Value_Change_Of_Value_Flag[i] = 0;
            Analog_Value_Valid_Value[i] = FALSE;
        }
    }

    return;
}

/* we simply have 0-n object instances.  Yours might be */
/* more complex, and then you need validate that the */
/* given instance exists */
bool Analog_Value_Valid_Instance(
    uint32_t object_instance)
{
    int group;
	int value;

    group = object_instance / 100;
	value = object_instance % 100;
    if ((group > 0 && group <= MAX_ANALOG_VALUES / 2) && (value >= 1 && value <= 2))
        return true;

    return false;
}

/* we simply have 0-n object instances.  Yours might be */
/* more complex, and then count how many you have */
unsigned Analog_Value_Count(
    void)
{
    Analog_Value_Init();
    return MAX_ANALOG_VALUES;
}

/* we simply have 0-n object instances.  Yours might be */
/* more complex, and then you need to return the instance */
/* that correlates to the correct index */
uint32_t Analog_Value_Index_To_Instance(
    unsigned index)
{
    int position_controller_nr = index / 2;
    int value_nr = index % 2;
    int instance = ((position_controller_nr + 1) * 100);

    if (value_nr == 0) { // current position
        instance += 1;
    } else if (value_nr == 1) { // desires position
        instance += 2;
    }

    return instance;
}

/* we simply have 0-n object instances.  Yours might be */
/* more complex, and then you need to return the index */
/* that correlates to the correct instance number */
unsigned Analog_Value_Instance_To_Index(
    uint32_t object_instance)
{
    int a;
    int b;
    a = (object_instance % 100) - 1;
    b = (((object_instance - a) / 100) - 1) * 2;
    return a + b;
}

/* note: the object name must be unique within this device */
//void Analog_Value_Generate_Name(
char *Analog_Value_Name(
    uint32_t object_instance)
{
    static char str[20] = "";

    if (object_instance % 100 == 1) { // current position
        sprintf( str, "CurrentPosition %d", (object_instance / 100) );
    } else if (object_instance % 100 == 2) { // desires position
        sprintf( str, "DesiredPosition %d", (object_instance / 100) );
    }

    return str;
}

uint32_t Analog_Value_Name_To_Instance(
        char *name)
{
    char * numberStr;
	int number;
    uint32_t instance = 0;

    numberStr = _fstrchr((const char far *)name, ' ');
	numberStr++;
	number = atoi(numberStr);
    if (_fstrstr((const char far *)name, (const char far *)"CurrentPosition") != NULL)
	{
		instance = (100 * number) + 1;
	}
	else if (_fstrstr((const char far *)name, (const char far *)"DesiredPosition") != NULL)
	{
		instance = (100 * number) + 2;
	}
    
    return instance;
}

bool Analog_Value_Present_Value_Set(
    uint32_t object_instance,
    float value,
    uint8_t priority)
{
    unsigned index = 0;
    bool status = false;
	float present;

    Analog_Value_Init();
    index = Analog_Value_Instance_To_Index(object_instance);
    if (index < MAX_ANALOG_VALUES && !Analog_Value_Out_Of_Service_Value[index]) {
        if (priority && (priority <= BACNET_MAX_PRIORITY) &&
            (priority != 6 /* reserved */ )) {
            if ((value >= 0.0) && (value <= 1000.0)) {
                Analog_Value_Level[index][priority - 1] = value;
                Analog_Value_Valid_Value[index] = true;
                status = true;
            } else if (value == ANALOG_LEVEL_NULL) {
                Analog_Value_Level[index][priority - 1] = value;
                Analog_Value_Valid_Value[index] = true;
                status = true;
            }

            present = Analog_Value_Present_Value(object_instance);
            if( (Analog_Value_Previous_Level[index] <= (present - Analog_Value_COV_Increment[index])) ||
              (Analog_Value_Previous_Level[index] >= (present + Analog_Value_COV_Increment[index]))) {
                Analog_Value_Change_Of_Value_Flag[index] = TRUE;
            }
        }
    }

    return status;
}


float Analog_Value_Present_Value(
    uint32_t object_instance)
{
    float value = ANALOG_RELINQUISH_DEFAULT;
    unsigned index = 0;
    unsigned i = 0;
    char *object_name = Analog_Value_Name(object_instance);

    Analog_Value_Init();
    index = Analog_Value_Instance_To_Index(object_instance);

	if (index < MAX_ANALOG_VALUES)
	{
        for (i = 0; i < BACNET_MAX_PRIORITY; i++) {
            if (Analog_Value_Level[index][i] != ANALOG_LEVEL_NULL) {
                value = Analog_Value_Level[index][i];
                break;
            }
        }
    }
	
	return value;
}

bool Analog_Value_Change_Of_Value(
    uint32_t object_instance)
{
    bool status = false;
    unsigned index;

    index = Analog_Value_Instance_To_Index(object_instance);
    if (index < MAX_ANALOG_VALUES) {
        status = Analog_Value_Change_Of_Value_Flag[index];
    }

    return status;
}

void Analog_Value_Change_Of_Value_Clear(
    uint32_t object_instance)
{
    unsigned index;

    index = Analog_Value_Instance_To_Index(object_instance);
    if (index < MAX_ANALOG_VALUES) {
        Analog_Value_Change_Of_Value_Flag[index] = false;
        Analog_Value_Previous_Level[index] = Analog_Value_Present_Value(object_instance);
    }
}

bool Analog_Value_Out_Of_Service(
    uint32_t object_instance)
{
    bool value = false;
    unsigned index = 0;

    index = Analog_Value_Instance_To_Index(object_instance);
    if (index < MAX_ANALOG_VALUES) {
        value = Analog_Value_Out_Of_Service_Value[index];
    }

    return value;
}

bool Analog_Value_Has_Valid_Value(
    uint32_t object_instance)
{
    return Analog_Value_Valid_Value[Analog_Value_Instance_To_Index(object_instance)];
}

BACNET_CHARACTER_STRING Analog_Value_Description(
        uint32_t object_instance)
{
    BACNET_CHARACTER_STRING return_string;
    char string_value[60];

    if (object_instance % 100 == 1) {
        sprintf( string_value, "The current position of position controller %d.", (object_instance / 100) );
    } else if (object_instance % 100 == 2) {
        sprintf( string_value, "The desired position of position controller %d.", (object_instance / 100) );
    } else
	string_value[0] = 0;

    characterstring_init_ansi(&return_string, &string_value[0]);

    return return_string;
}

// Voor COV notifications
bool Analog_Value_Encode_Value_List(
    uint32_t object_instance,
    BACNET_PROPERTY_VALUE * value_list)
{
    value_list->propertyIdentifier = PROP_PRESENT_VALUE;
    value_list->propertyArrayIndex = BACNET_ARRAY_ALL;
    value_list->value.context_specific = false;
    value_list->value.tag = BACNET_APPLICATION_TAG_REAL;
    value_list->value.type.Real =
        Analog_Value_Present_Value(object_instance);

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
    if (Analog_Value_Out_Of_Service_Value[object_instance]) {
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
int Analog_Value_Encode_Property_APDU(
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
    float real_value = (float) 1.414;
    unsigned object_index = 0;
    unsigned i = 0;
    bool state = false;

    Analog_Value_Init();
    switch (property) {
        case PROP_OBJECT_IDENTIFIER:
            apdu_len =
                encode_application_object_id(&apdu[0], OBJECT_ANALOG_VALUE,
                object_instance);
            break;
        case PROP_DESCRIPTION:
            char_string = Analog_Value_Description(object_instance);
            apdu_len = encode_application_character_string(&apdu[0], &char_string);
            break;
        case PROP_OBJECT_NAME:
            characterstring_init_ansi(&char_string,
                Analog_Value_Name(object_instance));
            apdu_len =
                encode_application_character_string(&apdu[0], &char_string);
            break;
        case PROP_OBJECT_TYPE:
            apdu_len =
                encode_application_enumerated(&apdu[0], OBJECT_ANALOG_VALUE);
            break;
        case PROP_PRESENT_VALUE:
            real_value = Analog_Value_Present_Value(object_instance);
            apdu_len = encode_application_real(&apdu[0], real_value);
            break;
        case PROP_STATUS_FLAGS:
            object_index = Analog_Value_Instance_To_Index(object_instance);
            bitstring_init(&bit_string);
            bitstring_set_bit(&bit_string, STATUS_FLAG_IN_ALARM, false);
            bitstring_set_bit(&bit_string, STATUS_FLAG_FAULT, false);
            bitstring_set_bit(&bit_string, STATUS_FLAG_OVERRIDDEN, false);
            bitstring_set_bit(&bit_string, STATUS_FLAG_OUT_OF_SERVICE, Analog_Value_Out_Of_Service_Value[object_index]);
            apdu_len = encode_application_bitstring(&apdu[0], &bit_string);
            break;
        case PROP_EVENT_STATE:
            apdu_len =
                encode_application_enumerated(&apdu[0], EVENT_STATE_NORMAL);
            break;
        case PROP_OUT_OF_SERVICE:
            object_index = Analog_Value_Instance_To_Index(object_instance);
            state = Analog_Value_Out_Of_Service_Value[object_index];
            apdu_len = encode_application_boolean(&apdu[0], state);
            break;
        case PROP_UNITS:
            apdu_len = encode_application_enumerated(&apdu[0], UNITS_PERCENT);
            break;
        case PROP_PRIORITY_ARRAY:
            /* Array element zero is the number of elements in the array */
            if (array_index == 0)
                apdu_len =
                    encode_application_unsigned(&apdu[0], BACNET_MAX_PRIORITY);
            /* if no index was specified, then try to encode the entire list */
            /* into one packet. */
            else if (array_index == BACNET_ARRAY_ALL) {
                object_index = Analog_Value_Instance_To_Index(object_instance);
                for (i = 0; i < BACNET_MAX_PRIORITY; i++) {
                    /* FIXME: check if we have room before adding it to APDU */
                    if (Analog_Value_Level[object_index][i] ==
                        ANALOG_LEVEL_NULL)
                        len = encode_application_null(&apdu[apdu_len]);
                    else {
                        real_value = Analog_Value_Level[object_index][i];
                        len =
                            encode_application_real(&apdu[apdu_len],
                            real_value);
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
                object_index = Analog_Value_Instance_To_Index(object_instance);
                if (array_index <= BACNET_MAX_PRIORITY) {
                    if (Analog_Value_Level[object_index][array_index - 1] ==
                        ANALOG_LEVEL_NULL)
                        apdu_len = encode_application_null(&apdu[0]);
                    else {
                        real_value =
                            Analog_Value_Level[object_index][array_index - 1];
                        apdu_len =
                            encode_application_real(&apdu[0], real_value);
                    }
                } else {
                    *error_class = ERROR_CLASS_PROPERTY;
                    *error_code = ERROR_CODE_INVALID_ARRAY_INDEX;
                    apdu_len = -1;
                }
            }

            break;
        case PROP_RELINQUISH_DEFAULT:
            real_value = ANALOG_RELINQUISH_DEFAULT;
            apdu_len = encode_application_real(&apdu[0], real_value);
            break;
        case PROP_COV_INCREMENT:
            object_index = Analog_Value_Instance_To_Index(object_instance);
            real_value = Analog_Value_COV_Increment[object_index];
            apdu_len = encode_application_real(&apdu[0], real_value);
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
bool Analog_Value_Write_Property(
    BACNET_WRITE_PROPERTY_DATA * wp_data,
    BACNET_ERROR_CLASS * error_class,
    BACNET_ERROR_CODE * error_code)
{
    bool status = false;        /* return value */
    unsigned int object_index = 0;
    unsigned int priority = 0;
    uint8_t level = ANALOG_LEVEL_NULL;
    int len = 0;
    BACNET_APPLICATION_DATA_VALUE value;

    Analog_Value_Init();
    if (!Analog_Value_Valid_Instance(wp_data->object_instance)) {
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
        case PROP_PRESENT_VALUE:
            object_index = Analog_Value_Instance_To_Index(wp_data->object_instance);
            if (wp_data->object_instance % 100 == 2) { // desired position
                if (value.tag == BACNET_APPLICATION_TAG_REAL) {
                    /* Command priority 6 is reserved for use by Minimum On/Off
                       algorithm and may not be used for other purposes in any
                       object. */
                    if (Analog_Value_Present_Value_Set(wp_data->object_instance,
                            value.type.Real, wp_data->priority)) {
                        status = true;
                    } else if (wp_data->priority == 6 || Analog_Value_Out_Of_Service_Value[object_index]) {
                        /* Command priority 6 is reserved for use by Minimum On/Off
                           algorithm and may not be used for other purposes in any
                           object. */
                        *error_class = ERROR_CLASS_PROPERTY;
                        *error_code = ERROR_CODE_WRITE_ACCESS_DENIED;
                    } else {
                        *error_class = ERROR_CLASS_PROPERTY;
                        *error_code = ERROR_CODE_VALUE_OUT_OF_RANGE;
                    }
                } else if (value.tag == BACNET_APPLICATION_TAG_NULL) {
                    priority = wp_data->priority;
                    if (priority && (priority <= BACNET_MAX_PRIORITY)) {
                        priority--;
						Analog_Value_Present_Value_Set(wp_data->object_instance, ANALOG_LEVEL_NULL, priority);
                        /* Note: you could set the physical output here to the next
                           highest priority, or to the relinquish default if no
                           priorities are set.
                           However, if Out of Service is TRUE, then don't set the
                           physical output.  This comment may apply to the
                           main loop (i.e. check out of service before changing output) */
                        status = true;
                    } else {
                        *error_class = ERROR_CLASS_PROPERTY;
                        *error_code = ERROR_CODE_VALUE_OUT_OF_RANGE;
                    }
                } else {
                    *error_class = ERROR_CLASS_PROPERTY;
                    *error_code = ERROR_CODE_INVALID_DATA_TYPE;
                }
            } else {
                *error_class = ERROR_CLASS_PROPERTY;
                *error_code = ERROR_CODE_WRITE_ACCESS_DENIED;
            }
            break;
        //case PROP_OUT_OF_SERVICE:
        //    if (value.tag == BACNET_APPLICATION_TAG_BOOLEAN) {
        //        object_index =
        //            Analog_Value_Instance_To_Index(wp_data->object_instance);
        //        Analog_Value_Out_Of_Service_Value[object_index] = value.type.Boolean;
        //        status = true;
        //    } else {
        //        *error_class = ERROR_CLASS_PROPERTY;
        //        *error_code = ERROR_CODE_INVALID_DATA_TYPE;
        //    }
        //    break;
        case PROP_COV_INCREMENT:
            if (value.tag == BACNET_APPLICATION_TAG_REAL) {
                if (value.type.Real >= 0.0) {
                        object_index = Analog_Value_Instance_To_Index(wp_data->object_instance);
                        Analog_Value_COV_Increment[object_index] = value.type.Real;
                        status = TRUE;
                } else {
                    *error_class = ERROR_CLASS_PROPERTY;
                    *error_code = ERROR_CODE_VALUE_OUT_OF_RANGE;
                }
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

void testAnalog_Value(
    Test * pTest)
{
    uint8_t apdu[MAX_APDU] = { 0 };
    int len = 0;
    uint32_t len_value = 0;
    uint8_t tag_number = 0;
    BACNET_OBJECT_TYPE decoded_type = OBJECT_ANALOG_VALUE;
    uint32_t decoded_instance = 0;
    uint32_t instance = 123;
    BACNET_ERROR_CLASS error_class;
    BACNET_ERROR_CODE error_code;


    len =
        Analog_Value_Encode_Property_APDU(&apdu[0], instance,
        PROP_OBJECT_IDENTIFIER, BACNET_ARRAY_ALL, &error_class, &error_code);
    ct_test(pTest, len != 0);
    len = decode_tag_number_and_value(&apdu[0], &tag_number, &len_value);
    ct_test(pTest, tag_number == BACNET_APPLICATION_TAG_OBJECT_ID);
    len =
        decode_object_id(&apdu[len], (int *) &decoded_type, &decoded_instance);
    ct_test(pTest, decoded_type == OBJECT_ANALOG_VALUE);
    ct_test(pTest, decoded_instance == instance);

    return;
}

#ifdef TEST_ANALOG_VALUE
int main(
    void)
{
    Test *pTest;
    bool rc;

    pTest = ct_create("BACnet Analog Value", NULL);
    /* individual tests */
    rc = ct_addTestFunction(pTest, testAnalog_Value);
    assert(rc);

    ct_setStream(pTest, stdout);
    ct_run(pTest);
    (void) ct_report(pTest);
    ct_destroy(pTest);

    return 0;
}
#endif /* TEST_ANALOG_VALUE */
#endif /* TEST */
