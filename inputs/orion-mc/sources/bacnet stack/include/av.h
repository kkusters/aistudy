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
#ifndef AV_H
#define AV_H

#include <stdbool.h>
#include <stdint.h>
#include "bacdef.h"
#include "bacerror.h"
#include "bacstr.h"
#include "wp.h"
#include "ct_data.h"

#ifndef MAX_ANALOG_VALUES
#define MAX_ANALOG_VALUES (MAX_GROUP * 2)
// * 2 omdat we per group 2 waardes hebben (CurrentPosition en DesiredPosition)
#endif

/* we choose to have a NULL level in our system represented by */
/* a particular value.  When the priorities are not in use, they */
/* will be relinquished (i.e. set to the NULL level). */
#define ANALOG_LEVEL_NULL 255

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
    void Analog_Value_Property_Lists(
        const int **pRequired,
        const int **pOptional,
        const int **pProprietary);

    void Analog_Value_Init(
        void);

    bool Analog_Value_Valid_Instance(
        uint32_t object_instance);

    unsigned Analog_Value_Count(
        void);

    uint32_t Analog_Value_Index_To_Instance(
        unsigned index);

    char *Analog_Value_Name(
        uint32_t object_instance);

	uint32_t Analog_Value_Name_To_Instance(
		char *name);

    bool Analog_Value_Present_Value_Set(
        uint32_t object_instance,
        float value,
        uint8_t priority);

    float Analog_Value_Present_Value(
        uint32_t object_instance);

	bool Analog_Value_Change_Of_Value(
	    uint32_t object_instance);

	void Analog_Value_Change_Of_Value_Clear(
	    uint32_t object_instance);

	bool Analog_Value_Out_Of_Service(
	    uint32_t object_instance);

    bool Analog_Value_Has_Valid_Value(
		uint32_t object_instance);

	BACNET_CHARACTER_STRING Analog_Value_Description(
		uint32_t object_instance);

	bool Analog_Value_Encode_Value_List(
	    uint32_t object_instance,
	    BACNET_PROPERTY_VALUE * value_list);

    int Analog_Value_Encode_Property_APDU(
        uint8_t * apdu,
        uint32_t object_instance,
        BACNET_PROPERTY_ID property,
        int32_t array_index,
        BACNET_ERROR_CLASS * error_class,
        BACNET_ERROR_CODE * error_code);

    bool Analog_Value_Write_Property(
        BACNET_WRITE_PROPERTY_DATA * wp_data,
        BACNET_ERROR_CLASS * error_class,
        BACNET_ERROR_CODE * error_code);

#ifdef TEST
#include "ctest.h"
    void testAnalog_Value(
        Test * pTest);
#endif

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif
