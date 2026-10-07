#ifndef CH_BACNET_OBJECT_H
#define CH_BACNET_OBJECT_H 1

#include "bacstr.h"
#include "bacapp.h"
#include "msv.h"

typedef enum {
	POSITION_CONTROLLER_INVALID_VALUE_ID = 0,
	POSITION_CONTROLLER_CURRENT_POSITION = 1,
	POSITION_CONTROLLER_DESIRED_POSITION = 2,
	POSITION_CONTROLLER_HOLD_POSITION = 3,
	POSITION_CONTROLLER_OPERATION_MODE = 4,
	POSITION_CONTROLLER_OPERATION_STATE	= 5
} POSITION_CONTROLLER_VALUE_ID;

extern int orion_app_priority;

// Get & Set object values
bool Bacnet_Object_Has_Valid_Value(
	int position_controller_index,
	POSITION_CONTROLLER_VALUE_ID value_id);

void * Get_Bacnet_Object_Value(
	int position_controller_index,
	POSITION_CONTROLLER_VALUE_ID value_id);

bool Set_Bacnet_Object_Value(
    int position_controller_index,
    POSITION_CONTROLLER_VALUE_ID value_id,
    BACNET_APPLICATION_DATA_VALUE * value);

#endif // CH_BACNET_OBJECT_H
