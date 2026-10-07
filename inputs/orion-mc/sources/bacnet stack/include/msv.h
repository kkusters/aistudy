/**************************************************************************
*
* Copyright (C) 2005 Steve Karg <skarg@users.sourceforge.net>
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
#ifndef MSV_H
#define MSV_H

#include <stdbool.h>
#include <stdint.h>
#include "bacdef.h"
#include "bacerror.h"
#include "wp.h"
#include "ct_data.h"

#ifndef MAX_MULTISTATE_VALUES
#define MAX_MULTISTATE_VALUES (MAX_GROUP * 2)
// * 2 omdat we per group 2 waardes hebben (OperationMode en OperationState)
#endif

/* NULL part of the array */
#define MULTISTATE_NULL (255)

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

	typedef enum {
		OM_NORMAL_OPERATION = 1,
		OM_OUT_OF_ORDER = 2,
		OM_FAULT = 3,
		OM_MANUAL = 4,
		OM_IN_MAINTENANCE = 5,
		OM_NOT_AVAILABLE = 6
	} OPERATION_MODE;

	typedef enum {
		OS_AT_POSITION = 1,
		OS_GOING_DOWN = 2,
		OS_GOING_UP = 3,
		OS_AT_LOW_LIMIT = 4,
		OS_AT_HIGH_LIMIT = 5,
		OS_HOLDING = 6,
		OS_HELD = 7,
		OS_RESTARTING = 8
	} OPERATION_STATE;


    void Multistate_Value_Property_Lists(
        const int **pRequired,
        const int **pOptional,
        const int **pProprietary);

    void Multistate_Value_Init(
        void);

    bool Multistate_Value_Valid_Instance(
        uint32_t object_instance);

    unsigned Multistate_Value_Count(
        void);

    uint32_t Multistate_Value_Index_To_Instance(
        unsigned index);

	unsigned Multistate_Value_Instance_To_Index(
    	uint32_t object_instance);
	    
	void Multistate_Value_Generate_Name(
        uint32_t object_instance,
        char *text_string);

    char *Multistate_Value_Name(
        uint32_t object_instance);

	uint32_t Multistate_Value_Name_To_Instance(
		char *name);

	bool Multistate_Value_Present_Value_Set(
	    uint32_t object_instance,
	    uint8_t value,
	    uint8_t priority);

	uint32_t Multistate_Value_Present_Value(
	    uint32_t object_instance);

	bool Multistate_Value_Change_Of_Value(
	    uint32_t object_instance);

	void Multistate_Value_Change_Of_Value_Clear(
	    uint32_t object_instance);

	bool Multistate_Value_Has_Valid_Value(
		uint32_t object_instance);
	    
	void Multistate_Value_State_Text(
    	uint32_t object_instance,
    	BACNET_CHARACTER_STRING * char_string);

	BACNET_CHARACTER_STRING Multistate_Value_Description(
		uint32_t object_instance);

	bool Multistate_Value_Encode_Value_List(
	    uint32_t object_instance,
	    BACNET_PROPERTY_VALUE * value_list);

    int Multistate_Value_Encode_Property_APDU(
        uint8_t * apdu,
        uint32_t object_instance,
        BACNET_PROPERTY_ID property,
        int32_t array_index,
        BACNET_ERROR_CLASS * error_class,
        BACNET_ERROR_CODE * error_code);

    bool Multistate_Value_Write_Property(
        BACNET_WRITE_PROPERTY_DATA * wp_data,
        BACNET_ERROR_CLASS * error_class,
        BACNET_ERROR_CODE * error_code);

#ifdef TEST
#include "ctest.h"
    void testMultistateValue(
        Test * pTest);
#endif

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif
