/*
 * access - defines for access to object dictionary
 *
 * Copyright (c) 2001-2002 port GmbH Halle/Saale
 *------------------------------------------------------------------
 * $Header$
 *
 *------------------------------------------------------------------
 *
 * modification history
 * --------------------
 * $Log$
 * Revision 1.0  2008-03-07 17:05:30+01  driet
 * Initial revision
 *
 * Revision 2.5  2002/11/18 10:49:48  boe
 * add prototype for getOvDataTypeLen
 *
 * Revision 2.4  2002/08/30 08:48:54  boe
 * add prototype for getVirtualObjAddr
 *
 * Revision 2.3  2002/05/21 14:53:21  boe
 * copyright changed
 *
 * Revision 2.2  2001/03/29 14:36:52  boe
 * defines for VALUE_DESC moved from internal header
 *
 * Revision 2.1  2001/01/26 12:32:00  boe
 * access to the object dictionary
 *
 *
 *
 *------------------------------------------------------------------
 */

/*
DESCRIPTION

The file contains definitions of structures and complex data types
for access to object dictionary

*/

#ifndef __CO_ACCES_H
# define __CO_ACCES_H

#include <co_def.h>


/* value description for each entry */
typedef struct
{
    UNSIGNED32  defaultVal;        /* default value or size for domains */
#ifdef CONFIG_LIMITS_CHECK
    UNSIGNED32  minRange;          /* min. range of objectelement */
    UNSIGNED32  maxRange;          /* max. range of objectelement*/
#endif
    INTEGER8    size;              /* size of element in Bytes
					if size negativ is objectvalue
					a signed type*/
    UNSIGNED8   attribute;         /* domain type = 1, short desc = 2,
				      float = 4, num_val = 0x10,
				      read permitted = 0x20,
				      write permitted = 0x40,
				      pdoMAPPING allowed = 0x80 bitcoded ! */
} VALUE_DESC_T;


/* The LIST_ELEMENT is a type for a element of the object dictionary.
   It describes the features of the dataobject which ist stored at
   the address pObj */

typedef struct
{
    UNSIGNED8    *pObj;         /* pointer to data */
    VALUE_DESC_T *pValDesc;     /* value description */
    UNSIGNED16   index;		/* index of object */
    UNSIGNED8    numOfElem; 	/* number of elements */
} LIST_ELEMENT_T;


#ifdef CONFIG_CONST_OBJDIR
# define OBJDIR_T LIST_ELEMENT_T CO_CONST
#else
  typedef LIST_ELEMENT_T OBJDIR_T;
#endif


/* definition of object dictionary ranges */

#define START_OBJ_DIC           0x1000
#define END_OBJ_DIC             0xFFFF
#define START_COM_PROF          0x1000
#define END_COM_PROF            0x1FFF
#define START_MANU_PROF         0x2000
#define END_MANU_PROF           0x5FFF
#define START_DEVICE_PROF       0x6000
#define END_DEVICE_PROF		0x9FFF


/* defines for special in VALUE_DESC */

#define CO_NUM_VAL    		((UNSIGNED8)0x10)  /* numeric value (for byte swapping) */
#define CO_READ_PERM  		((UNSIGNED8)0x20)  /* read permission */
#define CO_WRITE_PERM 		((UNSIGNED8)0x40)  /* write permission */
#define CO_MAP_PERM   		((UNSIGNED8)0x80)  /* pdo mapping permission */
#define CO_UP_DN_LD_DOMAIN	((UNSIGNED8)0x01)  /* domain type for up and down load */
#define CO_SHORT_ARRAY_DESC	((UNSIGNED8)0x02)  /* array element all equal to subindex 1*/
#define CO_FLOAT_VAL		((UNSIGNED8)0x04)  /* float value */


/* external variable declarations */


/* function prototypes */

RET_T 		getObjEntry(UNSIGNED16, UNSIGNED8, UNSIGNED8 *, UNSIGNED32 *,
			BOOL_T );
RET_T 		getObjAddr(UNSIGNED16, UNSIGNED8, UNSIGNED8 **, UNSIGNED32 *
			);
UNSIGNED8  	getObjAttr(UNSIGNED16, UNSIGNED8 );
BOOL_T     	setObjAttr(UNSIGNED16, UNSIGNED8, UNSIGNED8
			);
UNSIGNED32 	getDomainSize(UNSIGNED16, UNSIGNED8 );
BOOL_T     	setDomainSize(UNSIGNED16, UNSIGNED8, UNSIGNED32
			);
UNSIGNED8 	*getDomainAddr(UNSIGNED16, UNSIGNED8 );
BOOL_T     	setDomainAddr(UNSIGNED16, UNSIGNED8, UNSIGNED8 *
			);
UNSIGNED8 	getNumOfElem(UNSIGNED16 );
RET_T 		putObj(UNSIGNED16, UNSIGNED8 , UNSIGNED8 *, UNSIGNED32, BOOL_T
			);
UNSIGNED8	getOvDataTypeLen(UNSIGNED16 index);

RET_T 		getVirtualObjAddr(UNSIGNED16, UNSIGNED8, UNSIGNED8 **,
			UNSIGNED32 * );

#endif /* __CO_ACCES_H */

/* end of source */

