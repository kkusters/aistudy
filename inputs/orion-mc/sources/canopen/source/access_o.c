/*
 *++ access_o - contains routines for the access to the object dictionary
 *-- access_o - beinhaltet Rotuinen zum Zugriff auf das Objektverzeichnis
 *
 * Copyright (c) 1997-2002 port GmbH Halle (Saale)
 *------------------------------------------------------------------
 * $Header$
 *
 *------------------------------------------------------------------
 *
 * modification history
 * --------------------
 * $Log$
 * Revision 1.0  2008-03-07 17:03:55+01  driet
 * Initial revision
 *
 * Revision 2.27  2003/10/01 13:21:21  boe
 * usage of domain entries in records possible
 *
 * Revision 2.26  2003/07/08 13:47:46  boe
 * swap compare parameter for limit check (to smoth compiler error 16bit)
 *
 * Revision 2.25  2003/06/13 13:15:43  boe
 * define MAX_DATA_SIZE changed to CO_MAX_NUMDATA_SIZE
 * default is set to 8 in acces.h
 *
 * Revision 2.24  2003/03/11 13:50:48  boe
 * correct limit checks for 16bit cpus
 *
 * Revision 2.23  2002/11/14 09:36:52  boe
 * pointer changed from DOMAINFIELD255_T to DOMAINFILED8_T
 * adapt command line comments on doxygen
 *
 * Revision 2.22  2002/10/30 09:52:30  boe
 * add variable descriptions
 * transfer size information to getVirtualObjAddr
 * add function getOvDataTypeLen
 *
 * Revision 2.21  2002/08/30 08:07:30  boe
 * add support for virtual objects
 *
 * Revision 2.20  2002/05/30 14:05:55  hae
 * documentation correction
 *
 * Revision 2.19  2002/05/21 12:44:16  boe
 * change copyright
 *
 * Revision 2.18  2001/10/12 13:31:23  boe
 * change access for 16bit dsp cpus
 *
 * Revision 2.17  2001/06/19 15:18:42  boe
 * fix writing at subindex 0 for big endian machines
 *
 * Revision 2.16  2001/04/05 08:45:39  boe
 * comment changed
 *
 * Revision 2.15  2001/04/04 09:23:53  boe
 * cast for domain updownload added
 *
 * Revision 2.14  2001/03/29 14:23:47  boe
 * comment changed
 *
 * Revision 2.13  2001/03/28 12:43:16  boe
 * bittest explicite tested unequal 0
 *
 * Revision 2.12  2001/02/26 14:00:25  boe
 * documentation format chenged
 *
 * Revision 2.11  2001/01/26 10:40:02  boe
 * split include files into function specific headers
 *
 * Revision 2.10  2001/01/17 16:01:03  boe
 * expand all implicite if tests
 *
 * Revision 2.9  2000/10/05 11:22:09  boe
 * memcpy replaced by CO_MEMCPY/CO_NUM_MEMCPY
 * special thinks for real 16 Bit CPUs added
 * CO_PART_ALLOC saved
 *
 * Revision 2.8  2000/06/23 09:43:07  oe
 * Reworking for generating the Regerence Manual
 *
 * Revision 2.7  2000/06/21 08:47:01  boe
 * subIndex 0 at multiple subIndices always returns size 1
 *
 * Revision 2.6  2000/06/13 08:15:45  boe
 * unused variable for ALIGNMENT=1 removed
 *
 * Revision 2.5  2000/05/02 12:47:38  boe
 * Domain Up/Download for entries without subIndex possible
 *
 * Revision 2.4  2000/04/18 08:30:18  boe
 * special datatype DATA replaced with CO_DATA
 *
 * Revision 2.3  2000/04/04 08:09:40  boe
 * access for float values added
 *
 * Revision 2.2  2000/03/28 14:13:48  boe
 * adaption for multi-line version
 * limit monitoring changed
 *
 * Revision 2.1  2000/02/04 13:38:09  oe
 * - Changes for generating reference pages
 *
 * Revision 2.0  2000/01/21 10:59:55  boe
 * Überarbeitet und an Version 4.0 angepasst
 *
 *
 *------------------------------------------------------------------
 */

/**
*  \file access_o.c
*  \author port GmbH Halle (Saale)
*  $Revision$
*  $Date$
*
*++ This module contains functions for accessing
*++ objects in the object dictionary.
*++ The values and the properties of the object dictionary entries can be read
*++ and written via these functions.
*-- Dieses Modul enthält Zugriffsfunktionen zum Objektverzeichnis.
*-- Mit diesen Funktionen können Inhalt und Eigenschaften
*-- von Elementen des Objektverzeichnisses gelesen oder geschrieben werden.
*
*/

/* header of standard C - libraries */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* header of project specific types */
#include <cal_conf.h>

#include <co_mcpy.h>
#include "access.h"

/* constant definitions
---------------------------------------------------------------------------*/

/* local defined data types
---------------------------------------------------------------------------*/

/* list of external used functions, if not in headers
---------------------------------------------------------------------------*/

/* list of global defined functions
---------------------------------------------------------------------------*/

/* list of local defined functions
---------------------------------------------------------------------------*/

/* external variables
---------------------------------------------------------------------------*/

/* global variables
---------------------------------------------------------------------------*/

/* local defined variables
---------------------------------------------------------------------------*/
#ifdef CONFIG_RCS_IDENT
static char _rcsid[] = "$Id$";
#endif /* CONFIG_RCS_IDENT */

#if defined(CONFIG_BIG_ENDIAN) || defined(CONFIG_16BIT_CPU)
/* buffer for converted data, if BIG_ENDIAN machine */
static UNSIGNED8	convBuffer[CO_MAX_NUMDATA_SIZE];
#endif /* defined(CONFIG_BIG_ENDIAN) || defined(CONFIG_16BIT_CPU) */


/****************************************************************************/
/**
*
*++ \brief getObjEntry - get an object entry from the local object dictionary
*-- \brief getObjEntry - liefert einen Eintrag des lokalen Objektverzeichnisses
*
*++ The function gets the value and the data size of an object
*++ referenced by \em index and \em subIndex.
*++ \em Size
*++ is the object size of a single element in bytes
*++ in cases of arrays or records as an object.
*++ It tests the limits of indices and the read permission.
*++ If the entry is of type domain, then the function returns the address
*++ of this entry
*-- Die Funktion ermittelt den Wert und Datengröße eines
*-- über \em index und \em subIndex referenzierten Objektes des Objektverzeichnisses.
*-- \em Size
*-- ist die Größe eines Elementes des Objektes in Bytes
*-- (bei Arrays oder Records).
*-- Der übergebene Index wird auf das zulässige Limit überprüft
*-- und es erfolgt eine Überprüfung der Zulässigkeit des Lesezugriffs.
*-- Bei Objektverzeichniseinträgen vom Typ Domain
*-- wird statt des Wertes die Adresse übergeben
*
* \par Endianess
*++ On BIG_ENDIAN machines it converts the data, if the parameter
*++ \b local
*++ is set to \c CO_FALSE .
*-- Bei BIG_ENDIAN Prozessoren erfolgt eine Datenwandlung,
*-- wenn der Parameter
*-- \b local == \c CO_FALSE gesetzt ist.
*
* \code
* UNSIGNED8 data[4];
* UNSIGNED32 size;
*
* // get value and size of Object 0x2000:1
* getObjEntry(0x2000, 1, &data[0], &size, CO_TRUE);
* \endcode
*
* \retval OK
*++ success
*-- Erfolg
* \retval CO_E_NONEXIST_OBJECT
*++ object doesn't exist
*-- Das angegebene Objekt existiert nicht
* \retval CO_E_NO_READ_PERM
*++ no read permission
*-- Keine Leseerlaubnis für dieses Objekt
* \retval CO_E_NONEXIST_SUBINDEX
*++ subindex doesn't exist
*-- Der angegebene Subindex existiert nicht
*
*/
RET_T getObjEntry(
      UNSIGNED16 index,	   /**< main-index */
      UNSIGNED8  subIndex, /**< sub-index */
      UNSIGNED8  *pData,   /**< destination for data */
      UNSIGNED32 *pSize,   /**< destination for data size */
      BOOL_T     local     /**< data only for local usage */
      )
{
#ifdef CONFIG_FAST_GETOBJ
LIST_ELEMENT_T	*curObj;	/* pointer to current object */
UNSIGNED8	subIndexDesc;	/* Subindex for description */
#else /* CONFIG_FAST_GETOBJ */
UNSIGNED8	*ppData;	/* pointer to pointer of data */
RET_T		ret;		/* retval */
UNSIGNED8	attr;		/* attribut */
#endif /* CONFIG_FAST_GETOBJ */

#ifdef CONFIG_FAST_GETOBJ
    /* index value exceeds the physical limitations */
    if ((curObj = searchObj(index CO_COMMA_LINE_PARA)) == NULL)	{
	/* object doesn't exist */
	return(CO_E_NONEXIST_OBJECT);
    }

    if (curObj->numOfElem <= subIndex)  {
	/* subindex does not exist*/
	return(CO_E_NONEXIST_SUBINDEX);
    }

    /* test for short array description */
    if ((curObj->pValDesc[0].attribute & CO_SHORT_ARRAY_DESC) != 0)
    {
	subIndexDesc = 1;
    } else {
	subIndexDesc = subIndex;
    }
    /* security checks only for remote access */
    if (local == CO_FALSE) {
	if ((curObj->pValDesc[subIndexDesc].attribute & CO_READ_PERM)
		!= CO_READ_PERM) {
	    return(CO_E_NO_READ_PERM);
        }
    }

    if ((curObj->pValDesc[0].attribute & CO_UP_DN_LD_DOMAIN) != 0) {
	/* size of domain (special case) */
	*pSize = (UNSIGNED32)curObj->pValDesc[0].defaultVal;
	*pData = (UNSIGNED32)getSubIndexAddr(curObj, subIndex);
    } else {
	/* allocate security mechanism for object dictionary consistency */
	if (index < START_MANU_PROF) {
	    CO_COM_PART_ALLOC(CO_LINE_PARA);
	} else {
	    CO_APPL_PART_ALLOC(CO_LINE_PARA);
	}
	/* size of the other element types */
	*pSize = (UNSIGNED32)abs(curObj->pValDesc[subIndexDesc].size);

	/* copy only if size < 5 */
	if (*pSize < 5)  {
	    /* get address of subindex */
	    if (subIndex == 0)  {
		/* if there are more subIndizes,
		 * subindex 0 has per definition only 1 byte */
		if (curObj->numOfElem > 1)  {
#ifdef CONFIG_BIG_ENDIAN
		    *pData = *(curObj->pObj + pSize - 1);
		    *pSize = 1;
		} else  {
		    CO_MEMCPY(pData, curObj->pObj, *pSize);
		}
#else /* CONFIG_BIG_ENDIAN */
		    *pSize = 1;
		}
		CO_NUM_MEMCPY(pData, (UNSIGNED8 *)curObj->pObj, *pSize,
			curObj->pValDesc[0].attribute & CO_NUM_VAL);
#endif /* CONFIG_BIG_ENDIAN */
	    } else  {
		/* *ppData = getSubIndexAddr(curObj, subIndex); */
		CO_NUM_MEMCPY(pData, getSubIndexAddr(curObj, subIndex), *pSize,
			curObj->pValDesc[0].attribute & CO_NUM_VAL);
	    }
# ifdef CONFIG_BIG_ENDIAN
	    if (local == CO_FALSE) {
		/* Byteswapping for num values (and size < 5) */
		if (*pSize > 1)  {
		    CO_UNPACK_MEMCPY(&convBuffer[0], pData, *pSize, 
		    curObj->pValDesc[subIndexDesc].attribute & CO_NUM_VAL);
		    pData = &convBuffer[0];
		}
	    }
# endif /* CONFIG_BIG_ENDIAN */
	} else  {
	    *(UNSIGNED32 *)pData = (UNSIGNED32)getSubIndexAddr(curObj,subIndex);
	}
	/* release security mechanism for object dictionary consistency */
	if(index < START_MANU_PROF) {
	    CO_COM_PART_RELEASE(CO_LINE_PARA);
	} else {
	    CO_APPL_PART_RELEASE(CO_LINE_PARA);
	}
    }

#else /* CONFIG_FAST_GETOBJ */

    if ((ret = getObjAddr(index, subIndex, &ppData, pSize CO_COMMA_LINE_PARA)) 
	!= CO_OK)  {
	return(ret);
    }

    attr = getObjAttr(index, subIndex CO_COMMA_LINE_PARA);

    /* security checks only for remote access */
    if (local == CO_FALSE) {
	if ((attr & CO_READ_PERM) != CO_READ_PERM) {
	    return(CO_E_NO_READ_PERM);
        }
    }

    if ((attr & CO_UP_DN_LD_DOMAIN) != 0) {
	pData = ppData;
    } else {
	/* allocate security mechanism for object dictionary consistency */
	if (index < START_MANU_PROF) {
	    CO_COM_PART_ALLOC(CO_LINE_PARA);
	} else {
	    CO_APPL_PART_ALLOC(CO_LINE_PARA);
	}
	/* copy only if size < 5 */
	if (*pSize < 5)  {
	    /* get address of subindex */
# if defined(CONFIG_BIG_ENDIAN) || defined(CONFIG_16BIT_CPU)
	    if (local == CO_TRUE) {
		CO_NUM_MEMCPY(pData, ppData, *pSize, attr & CO_NUM_VAL);
	    } else {
		CO_UNPACK_MEMCPY(pData, ppData, *pSize, attr & CO_NUM_VAL);
	    }
# else /* defined(CONFIG_BIG_ENDIAN) || defined(CONFIG_16BIT_CPU) */
	    CO_MEMCPY(pData, ppData, *pSize);
# endif /* defined(CONFIG_BIG_ENDIAN) || defined(CONFIG_16BIT_CPU) */
	}
	/* release security mechanism for object dictionary consistency */
	if(index < START_MANU_PROF) {
	    CO_COM_PART_RELEASE(CO_LINE_PARA);
	} else {
	    CO_APPL_PART_RELEASE(CO_LINE_PARA);
	}
    }
#endif /* CONFIG_FAST_GETOBJ */
    return(CO_OK);
}


/****************************************************************************/
/**
*
*++ \brief getObjAddr - get the address of the object in the local object dictionary
*-- \brief getObjAddr - liefert Adresse eines Eintrages im lokalen Objektverzeichnis
*
*++ The function delivers the address of an object
*++ referenced by \em index and \em subIndex.
*++ It tests only the limit of the provided indices.
*-- Die Funktion ermittelt die Adresse des über \em index und \em subIndex
*-- referenzierten Objektes des Objektverzeichnisses.
*-- Der übergebene Index wird auf das zulässige Limit überprüft
*-- Wenn das \c #define \c CONFIG_VIRTUAL_OBJECTS gesetzt ist,
*-- und das Objekt \b nicht
*-- im herstellerspezifischen Bereich des Objektverzeichnis
*-- vorhanden ist, wird die Funktion \em getVirtualObjectAddr() aufgerufen.
*++ If \c CONFIG_VIRTUAL_OBJECTS  is \#define-d
*++ and the adressed object is \b not in the manufacturer specific area
*++ of the object dictionary then the function
*++ \em getVirtualObjectAddr() is called.
*
*-- Sie besitzt dieselben Parameter und Rückgabewerte wie \em getObjAddr(),
*-- ermöglicht dem Anwender aber die Nutzung virtueller Objekte.
*-- Der Anwender ist für die korrekte Artbeitsweise der Funktion verantwortlich.
*-- Alle virtuellen Objekte werden immer
*-- als numerische, lesbare und schreibbare Objekte angenommen.
*++ This function has the same parameters and return values as \em getObjAddr(),
*++ but enables the user to have so-called virtual objects
*++ in the object dictionary.
*++ The user is responsible for correct coding of this function.
*++ It is assumed that all virtual objects are numeric, readable and writeable. 

*
* \code
* UNSIGNED8 data[4];
* UNSIGNED8 *pData;
* UNSIGNED32 size;
*
* // get address and size of Object 0x2000:1
* getObjAddr(0x2000, 1, &pData, &size);
* // copy Object 0x2000 to local array data[]
* memcpy(&data[0], pData, size);
* \endcode
*
* \retval OK
*++ success
*-- Erfolg
* \retval CO_E_NONEXIST_OBJECT
*++ object doesn't exist
*-- Das angegebene Objekt existiert nicht
* \retval CO_E_NONEXIST_SUBINDEX
*++ subindex doesn't exist
*-- Der angegebene Subindex existiert nicht
*
*/

RET_T getObjAddr(
      UNSIGNED16 index,	   /**< main-index */
      UNSIGNED8  subIndex, /**< sub-index */
      UNSIGNED8  **pData,  /**< destination for data address*/
      UNSIGNED32 *pSize    /**< destination for data size */
      )
{
OBJDIR_T *curObj;		/* pointer to current object */
UNSIGNED8 subIndexDesc;	/* Subindex for description */

    /* index value exceeds the physical limitations */
    if ((curObj = searchObj(index CO_COMMA_LINE_PARA)) == NULL)	{
	/* object doesn't exist */

#ifdef CONFIG_VIRTUAL_OBJECTS
	/* virtual objects are allowed only in manufacturer area */
	if ((index < START_MANU_PROF) || (index > END_MANU_PROF)) {
	    return(CO_E_NONEXIST_OBJECT);
	} else  {
	    return(getVirtualObjAddr(index, subIndex, pData, pSize));
	}
#else /* CONFIG_VIRTUAL_OBJECTS */
	return(CO_E_NONEXIST_OBJECT);
#endif /* CONFIG_VIRTUAL_OBJECTS */
    }

    if (curObj->numOfElem <= subIndex)  {
	/* subindex does not exist*/
	return(CO_E_NONEXIST_SUBINDEX);
    }

    /* test for short array description */
    if ((curObj->pValDesc[0].attribute & CO_SHORT_ARRAY_DESC) != 0)
    {
	subIndexDesc = 1;
    } else {
	subIndexDesc = subIndex;
    }

    /* get address of subindex */
    if (subIndex == 0)  {

	*pData = curObj->pObj;
	*pSize = (UNSIGNED32)abs(curObj->pValDesc[0].size);

	/* if there are more subIndizes ,*/
	if (curObj->numOfElem > 1)  {
	    /* subindex 0 has per definition only 1 byte */
	    *pSize = 1;

#ifdef CONFIG_BIG_ENDIAN
	    *pData = curObj->pObj + *pSize - 1;
#endif /* CONFIG_BIG_ENDIAN */
	}

    } else  {
	/* subindex > 0 */
	*pData = (UNSIGNED8 *)getSubIndexAddr(curObj, subIndex);

#ifdef CONFIG_DOMAIN_UPDNLD
	if ((curObj->pValDesc[subIndex].attribute & CO_UP_DN_LD_DOMAIN) != 0) {
	    /* size of domain (special case) */
	    *pSize = (UNSIGNED32)curObj->pValDesc[subIndex].defaultVal;
	} else {
#endif /* CONFIG_DOMAIN_UPDNLD */

	    /* size of the other element types */
	    *pSize = (UNSIGNED32)abs(curObj->pValDesc[subIndexDesc].size);

#ifdef CONFIG_DOMAIN_UPDNLD
	}
#endif /* CONFIG_DOMAIN_UPDNLD */

    }

    return(CO_OK);
}


/****************************************************************************/
/**
*
*++ \brief putObj - put an object to the local object dictionary
*-- \brief putObj - Schreiben eines Elementes im lokalen Objektverzeichnis
*
*++ The function copies the data into the object
*++ referenced by \em index and \em subIndex.
*++ The upper limits of indices are tested and also
*++ the write permission 
*++ if the object is remote,
*++ \b local is \c CO_FALSE.
*++ If the \em subindex equals zero
*++ then the first element or the whole structure/array
*++ will be put into the dictionary.
*++ The parameter
*++ \b size specifies the size of the data in bytes.
*-- Die Funktion kopiert die übergebenen Daten in das
*-- durch \em index und \em subIndex angegebene Objekt.
*-- Sie testet den übergebenen Index auf das obere Limit und
*-- bei \em remote Zugriffen die Schreiberlaubnis
*-- für das Objekt.
*-- Der Parameter \b size gibt die Größe, der zu kopierenden 
*-- Daten, in Bytes an.
*
*++ On BIG_ENDIAN machines it converts the data,
*++ if parameter
*++ \b local == \c CO_FALSE.
*-- Bei BIG_ENDIAN Prozessoren erfolgt eine Datenwandlung,
*-- wenn der Parameter
*-- \b local == \c CO_FALSE gesetzt ist.
*
*-- Wenn das \c #define CONFIG_VIRTUAL_OBJECTS gesetzt ist,
*-- und das Objekt nicht im herstellerspezifischen Bereich des Objektverzeichnis
*-- vorhanden ist,
*-- wird die Funktion 
*-- getVirtualObjectAddr()
*-- aufgerufen.
*-- Sie besitzt dieselben Parameter und Rückgabewerte wie getObjAddr(),
*-- ermöglicht dem Anwender aber die Nutzung virtueller Objekte.
*-- Der Anwender ist für die korrekte Artbeitsweise der Funktion verantwortlich.
*-- Alle virtuellen Objekte wird immer als numerische, lesbare und schreibbare
*-- Objekte angenommen.
*-- In dieser Funktion,
*-- \em putObj(), wird auf die Adresse des virtuellen Objekts
*-- die mit \em pData übergebenen Daten geschrieben.
*-- Als Datenlänge wird immer die von \em getVirtualObjAddr()
*-- erhaltene Länge genutzt.
*-- Der Anwender ist dafür verantwortlich,
*-- dass die übergebene Datenlänge korrekt ist.
*
*++ If \c CONFIG_VIRTUAL_OBJECTS  is \#define-d
*++ and the adressed object is \b not in the manufacturer specific area
*++ of the object dictionary then the function
*++ \em getVirtualObjectAddr() is called.
*++ This function has the same parameters and return values as \em getObjAddr(),
*++ but enables the user to have so-called virtual objects
*++ in the object dictionary.
*++ The user is responsible for correct coding of this function.
*++ It is assumed that all virtual objects are numeric, readable and writeable. 
*++ \em putObj () uses the address and data size information
*++ of an \em virtual \em object returned by \em getVirtualObjAddr ()
*++ to write the data \em pData is pointing to to this address.
*++ The user is responsible for an correct data size information.
*
* \retval CO_OK
*++ success
*-- Erfolg
* \retval CO_E_NONEXIST_OBJECT
*++ object doesn't exist
*-- das angegebene Objekt existiert nicht
* \retval CO_E_NO_WRITE_PERM
*++ no write permission
*-- keine Schreiberlaubnis für dieses Objekt
* \retval CO_E_NONEXIST_SUBINDEX
*++ subindex doesn't exist
*-- der angegebene Subindex existiert nicht
* \retval CO_E_VALUE_TO_LOW
*++ value is too low
*-- der zu schreibende Wert liegt unter dem Limit
* \retval CO_E_VALUE_TO_HIGH
*++ value is too high
*-- der zu schreibende Wert liegt über dem Limit
* \retval CO_E_WRONG_SIZE
*++ size of data has wrong size
*-- falsche Datengröße
*
*/

RET_T putObj(
      UNSIGNED16 index,	   /**< main-index */
      UNSIGNED8  subIndex, /**< sub-index */
      UNSIGNED8  *pData,   /**< data address */
      UNSIGNED32 size,     /**< data size */
      BOOL_T     local     /**< data only for local usage */
      )
{
OBJDIR_T 	*curObj;	/* pointer to current object */
UNSIGNED8  	*address;	/* adress pointer */
UNSIGNED8	subIndexDesc;	/* Subindex for description */
UNSIGNED8	attr;		/* object attributs */
#ifdef CONFIG_LIMITS_CHECK
INTEGER32	tmpI32 = 0;	/* temp i32 val */
UNSIGNED32      tmpU32 = 0;	/* temp u32 val */
INTEGER32	tmpMin = 0;	/* temp min val */
INTEGER32	tmpMax = 0;	/* temp max val */
INTEGER8	objSize;	/* object size */
#endif /* CONFIG_LIMITS_CHECK */
#ifdef CONFIG_VIRTUAL_OBJECTS
RET_T		retval;		/* return value */
UNSIGNED32	vsize;		/* object size */
#endif /* CONFIG_VIRTUAL_OBJECTS */

    curObj = searchObj(index CO_COMMA_LINE_PARA);
    if (curObj == NULL)			/* object doesn't exist */
    {
#ifdef CONFIG_VIRTUAL_OBJECTS
	/* virtual objects are allowed only in manufacturer area */
	if ((index < START_MANU_PROF) || (index > END_MANU_PROF)) {
	    return(CO_E_NONEXIST_OBJECT);
	} else  {
	    /* transfer given size to this function */
	    vsize = size;
	    retval = getVirtualObjAddr(index, subIndex, &address, &vsize);
	    if (retval == CO_OK)  {
		/* copy data */
		CO_NUM_MEMCPY(address, pData, vsize, CO_NUM_VAL);
	    }
	    return(retval);
	}
#else /* CONFIG_VIRTUAL_OBJECTS */
	return(CO_E_NONEXIST_OBJECT);
#endif /* CONFIG_VIRTUAL_OBJECTS */
    }

    /* test for short arrays */
    if ((curObj->pValDesc[0].attribute & CO_SHORT_ARRAY_DESC) != 0) {
	subIndexDesc = 1;
    } else  {
	subIndexDesc = subIndex; 
    }

    attr = curObj->pValDesc[subIndexDesc].attribute;

    /* security checks only for remote access */
    if (local == CO_FALSE) {
# if defined(CONFIG_BIG_ENDIAN) || defined(CONFIG_16BIT_CPU)
	/* convert data into internal format for numerical values */
	if ((attr & CO_NUM_VAL) != 0) {
	    CO_PACK_MEMCPY(convBuffer, pData, size, attr & CO_NUM_VAL);
#  if defined(CONFIG_16BIT_CPU)
	    /* only 8 bits are valid for byte vars */
	    if (size == 1)  {
		convBuffer[0] &= 0xff;
	    }
#  endif /*  defined(CONFIG_16BIT_CPU) */
	    pData = convBuffer;
	}
# endif /* defined(CONFIG_BIG_ENDIAN) || defined(CONFIG_16BIT_CPU) */

	/* test the write permission */
	if ((attr & CO_WRITE_PERM) != CO_WRITE_PERM) {
	    return(CO_E_NO_WRITE_PERM);
	}

#ifdef CONFIG_LIMITS_CHECK
	/* test the limits for numerical values */
	if ((attr & CO_NUM_VAL) != 0) {

	    objSize = curObj->pValDesc[subIndexDesc].size;
	    if (subIndex == 0)  {
		/* if there are more subIndizes ,
		 * subindex 0 has per definition only 1 byte */
		if (curObj->numOfElem > 1)  {
		    objSize = 1;
		}
	    }

	    switch(objSize) {
		case -1:
		    tmpI32 = (INTEGER32)(*((INTEGER8 *)pData));
		    tmpMin = (INTEGER8)(curObj->pValDesc[subIndexDesc].minRange);
		    tmpMax = (INTEGER8)(curObj->pValDesc[subIndexDesc].maxRange);
# if defined(CONFIG_16BIT_CPU)
		    if (tmpI32 > 127)  {
			tmpI32 |= (INTEGER32)-256l;
		    }
# endif /* defined(CONFIG_16BIT_CPU) */
		    break;
		case -2:
		    tmpI32 = *((INTEGER16 *)pData);
		    tmpMin = (INTEGER16)(curObj->pValDesc[subIndexDesc].minRange);
		    tmpMax = (INTEGER16)(curObj->pValDesc[subIndexDesc].maxRange);
		    break;
		case -4:
		    tmpI32 = *((INTEGER32 *)pData);
		    tmpMin = (INTEGER32)(curObj->pValDesc[subIndexDesc].minRange);
		    tmpMax = (INTEGER32)(curObj->pValDesc[subIndexDesc].maxRange);
		    break;
		case 1:
		    tmpU32 = (*((UNSIGNED8 *)pData)) & 0xff;
		    break;
		case 2:
		    tmpU32 = *((UNSIGNED16 *)pData);
		    break;
		case 4:
		    tmpU32 = *((UNSIGNED32 *)pData);
		    break;
	    }
# ifdef CONFIG_FLOAT_VALUES
	    /* test for float values (4 bytes) */
	    if ((attr & CO_FLOAT_VAL) != 0)  {
		if (*(REAL32 *)pData >
			*(REAL32 *)&curObj->pValDesc[subIndexDesc].maxRange)  {
		    return(CO_E_VALUE_TO_HIGH);
		}
		if (*(REAL32 *)pData <
		    	*(REAL32 *)&curObj->pValDesc[subIndexDesc].minRange)  {
		    return(CO_E_VALUE_TO_LOW);
		}
	    } else  {
# endif /* CONFIG_FLOAT_VALUES */

	    /* signed values */
	    if (objSize < 0) {
		if (tmpI32 < tmpMin) {
		    return(CO_E_VALUE_TO_LOW);
		}
		if (tmpMax < tmpI32) {
		    return(CO_E_VALUE_TO_HIGH);
		}
	    } else { /* unsigned values */
		if (tmpU32
			< (UNSIGNED32)(curObj->pValDesc[subIndexDesc].minRange)
			) {
		    return(CO_E_VALUE_TO_LOW);
		}
		if ((UNSIGNED32)(curObj->pValDesc[subIndexDesc].maxRange)
			< tmpU32) {
		    return(CO_E_VALUE_TO_HIGH);
		}
	    }
# ifdef CONFIG_FLOAT_VALUES
	    }
# endif /* CONFIG_FLOAT_VALUES */

	}
#endif /* CONFIG_LIMITS_CHECK */
    }

    if(curObj->numOfElem <= subIndex)  { /* subindex does not exist */
	return(CO_E_NONEXIST_SUBINDEX);
    }

    /* get address of subindex */
    if (subIndex == 0)  {
	address = curObj->pObj;

	/* if there are more subIndizes ,
	 * subindex 0 has per definition only 1 byte */
#ifdef CONFIG_BIG_ENDIAN
	if (curObj->numOfElem > 1)  {
	    address = curObj->pObj + curObj->pValDesc[0].size - 1;
	}
#endif /* CONFIG_BIG_ENDIAN */

    }  else  {
	address = (UNSIGNED8 *)getSubIndexAddr(curObj, subIndex);
    }

    if ((curObj->pValDesc[subIndexDesc].attribute & CO_UP_DN_LD_DOMAIN) != 0) {
	address = (UNSIGNED8 *)(*(UPDNLD_DOMAIN_T **)address);
    }

    /* allocate security mechanism for object dictionary consistency */
    if (index < START_MANU_PROF) {
	CO_COM_PART_ALLOC(CO_LINE_PARA);
    } else {
	CO_APPL_PART_ALLOC(CO_LINE_PARA);
    }

    /* copy to dictionary */
    CO_NUM_MEMCPY(address, pData, size, attr & CO_NUM_VAL);

    /* release security mechanism for object dictionary consistency */
    if(index < START_MANU_PROF) {
	CO_COM_PART_RELEASE(CO_LINE_PARA);
    }
    else {
	CO_APPL_PART_RELEASE(CO_LINE_PARA);
    }

    return(CO_OK);
}


/*****************************************************************************/
/*
*
*++ \brief getSubIndexAddr - search the address of an object
*-- \brief getSubIndexAddr - ermittelt die Subindex Adresse eines Objekts
*
* \internal
*
*++ This function searches for the object address
*++ referenced by
*++ \em subIndex .
*-- Die Funktion sucht nach der über den \em subIndex referenzierten
*-- Adresse eines Objektes.
*
* \return
*++ address of object
*++ if successful
*-- Objektadresse
*-- bei Erfolg
*
*/

void *getSubIndexAddr(
     OBJDIR_T *curObj,	/**< pointer to object entry */
     UNSIGNED8      subIndex	/**< subindex */
     )
{
LOOPCNT_U8	i; 		/* loop counter */
UNSIGNED8	size;		/* size of subindex element */
UNSIGNED8	*pAddr;		/* pointer to subindex elements */
#if CONFIG_ALIGNMENT > 1
UNSIGNED8	nextSize;	/* size of subindex element */
#endif /* CONFIG_ALIGNMENT > 1 */

    /* load base address */
    pAddr = curObj->pObj;
    /* get address of subindex */
    for (i=0 ; i < subIndex; i++) {
	/* add size for last element */
	if ((curObj->pValDesc[i].attribute & CO_UP_DN_LD_DOMAIN) != 0) {
	    size = sizeof(UNSIGNED8 *);
	} else  {
	    size = (UNSIGNED8)abs(curObj->pValDesc[i].size);
	}

#ifdef CONFIG_16BIT_CPU
	if ((curObj->pValDesc[i].attribute & CO_NUM_VAL) != 0) {
	    size = (size + 1) >> 1;
	}
#endif /* CONFIG_16BIT_CPU */
	/* if no Byte Alignment */
	pAddr += size;

#if CONFIG_ALIGNMENT > 1
	/*
	   test whether address is a multiple of the CONFIG_ALIGNMENT
	   and the size of the next element must be greater or
	   equal ALIGNMENT, because the compiler set array elements
	   linear in the memory
	 */

	/* get size of next element */
	nextSize = size;
	/* test for short arrays */
	if ((curObj->pValDesc[0].attribute & CO_SHORT_ARRAY_DESC) != 0) {
	    if ((curObj->pValDesc[1].attribute & CO_NUM_VAL) != 0) {
		nextSize = (UNSIGNED8)abs(curObj->pValDesc[1].size);
# ifdef CONFIG_16BIT_CPU
		nextSize = (nextSize + 1) >> 1;
# endif /* CONFIG_16BIT_CPU */
	    }
	    if ((curObj->pValDesc[1].attribute & CO_UP_DN_LD_DOMAIN) != 0) {
		nextSize = sizeof(UNSIGNED8 *);
	    }
	} else {
	    if ((curObj->pValDesc[i+1].attribute & CO_NUM_VAL) != 0) {
		nextSize = (UNSIGNED8)abs(curObj->pValDesc[i+1].size);
# ifdef CONFIG_16BIT_CPU
		nextSize = (nextSize + 1) >> 1;
# endif /* CONFIG_16BIT_CPU */
	    }
	    if ((curObj->pValDesc[i+1].attribute & CO_UP_DN_LD_DOMAIN) != 0) {
		nextSize = sizeof(UNSIGNED8 *);
	    }
	}

	if (nextSize > size)  {
	    if (nextSize > CONFIG_ALIGNMENT)  {
		nextSize = CONFIG_ALIGNMENT;
	    }
	    while ( (UNSIGNED32)pAddr % nextSize) {
		pAddr++;
	    }
	}
#endif /* CONFIG_ALIGNMENT > 1 */

    }
    return(pAddr);
}

/****************************************************************************/
/*
*
*++ \brief searchObj - search the object in the local dictionary
*-- \brief searchObj - sucht nach einem Objekt im lokalen Objektverzeichnis
*
* NOMANUAL
*
*++ This function searches for the object referenced by
*++ \em index
*++ in the object dictionary 
*++ and returns its address.
*-- Die Funktion sucht nach dem über den \em index referenzierten
*-- Objekt im Objektverzeichnis und liefert dessen Adresse.
*
* \return
*++ address of object
*++ if successful
*-- Objektadresse
*-- bei Erfolg
* \retval NULL
*++ searching failed
*-- Objekt nicht gefunden
*
*/

OBJDIR_T *searchObj(
	      UNSIGNED16 index   /* index to search for */
	       )
{
UNSIGNED16 CO_DATA start = 0;	/* start index od */
				/* end index od */
UNSIGNED16 CO_DATA end = maxObjDicElements  - 1;
				/* middle index od */
UNSIGNED16 CO_DATA mid = (maxObjDicElements  - 1) / 2;


    while ((objDir[mid].index != index) && (start != mid) && (end != mid))
    {
	if (objDir[mid].index < index)
	{
	    start = mid;
	}
	else
	{
	    end = mid;
	}
	mid = (end + start) / 2;
    }
    if(objDir[mid].index != index)
    {
	if(   (objDir[mid+1].index == index)
	   && (mid < (maxObjDicElements  - 1)))
	{
	    return(&objDir[mid+1]);
	}
	else if ((objDir[mid-1].index == index) && (mid > 0))
	{
	    return(&objDir[mid-1]);
	}
	else
	{
	    return(NULL);
	}
    }
    return(&objDir[mid]);
}

/****************************************************************************/
/**
*
*++ \brief getObjAttr - delivers the attributes of an object
*-- \brief getObjAttr - ermittelt die Attribute eines Objektes
*
*++ This function delivers the attributes of the object
*++ referenced by \em index and \em subIndex.
*++ The return values are \b OR-ed combinations from the
*++ listed values below.
*-- Die Funktion ermittelt die Attribute des über
*-- \em index und \em subIndex referenzierten
*-- Objektes.
*-- Die Rückgabewerte sind \b OR-Verknüpfungen der unten gelisteten
*-- Werte.
*
*-- Wird ein virtuelles Objekt im herstellerspezifischen Bereich
*-- adressiert, wird immer
*++ If an \em virtual object in the manufacturer specific part
*++ of the object dictionary is addressed,
*++ the function  \b alwyays returns
* \c (CO_NUM_VAL \c | \c CO_READ_PERM \c | \c CO_WRITE_PERM)
*-- zurückgegeben.
*
* \retval 0
*++ object doesn't exist or attributte is zero
*-- Objekt existiert nicht oder das Attribut ist 0
* \retval CO_MAP_PERM
*++ PDO mapping permission
*-- PDO Mapping ist erlaubt
* \retval CO_READ_PERM
*++ read access permission
*-- Lesezugriffe sind erlaubt
* \retval CO_WRITE_PERM
*++ write access permission
*-- Schreibzugriffe sind erlaubt
* \retval CO_NUM_VAL
*++ object has numerical type
*-- Objekt hat einen numerischer Typ
* \retval CO_UP_DN_LD_DOMAIN
*++ object is a domain type
*-- Objekttyp ist Domain
*
*/

UNSIGNED8 getObjAttr(
	  UNSIGNED16 index,    /**< index of object */
	  UNSIGNED8  subIndex  /**< subindex of object */
	  )
{
OBJDIR_T *pObjEntry;	/* pointer to object entry */

    pObjEntry = searchObj(index CO_COMMA_LINE_PARA);
    if (pObjEntry == NULL) {

#ifdef CONFIG_VIRTUAL_OBJECTS
	/* virtual objects are allowed only in manufacturer area */
	if ((index < START_MANU_PROF) || (index > END_MANU_PROF)) {
	    return((UNSIGNED8)0);
	} else {
	    /* for virtual objects return always numeric, read and write */
	    return(CO_NUM_VAL | CO_READ_PERM | CO_WRITE_PERM);
	}
#else /* CONFIG_VIRTUAL_OBJECTS */
	return((UNSIGNED8)0);
#endif /* CONFIG_VIRTUAL_OBJECTS */
    }


        /* test for short arrays */
    if ((pObjEntry->pValDesc[0].attribute & CO_SHORT_ARRAY_DESC) != 0)  {
	return(pObjEntry->pValDesc[1].attribute);
    } else {
	return(pObjEntry->pValDesc[subIndex].attribute);
    }
}

/****************************************************************************/
/**
*
*++ \brief setObjAttr - sets the attributes of an object
*-- \brief setObjAttr - setzt die Attribute eines Objektes
*
*++ This function sets the attributes of the object
*++ referenced by \em index and \em subindex.
*++ Possible attributes can be any \b ORed combination from the
*++ listed values below.
*-- Die Funktion setzt die Attribute des über
*-- \em index und \em subindex referenzierten
*-- Objektes.
*-- Mögliche Attribute sind beliebige \b OR-Verknüpfungen
*-- der unten gelisteten Werte.
* 
*++ attributes:
*-- Attribute:
*
* \arg \c CO_MAP_PERM
*++ PDO mapping permission
*-- PDO Mapping ist erlaubt
* \arg \c CO_READ_PERM
*++ read access permission
*-- Lesezugriffe sind erlaubt
* \arg \c CO_WRITE_PERM
*++ write access permission
*-- Schreibzugriffe sind erlaubt
* \arg \c CO_NUM_VAL
*++ object has numerical type
*-- Objekt hat einen numerischer Typ
* \arg \c CO_UP_DN_LD_DOMAIN
*++ object is a domain type
*-- Objekttyp ist Domain
*
* \retval CO_TRUE
*++ success
*-- Erfolg
* \retval CO_FALSE
*++ object doesn't exist
*-- Objekt existiert nicht
*/

BOOL_T setObjAttr(
       UNSIGNED16 index,    /**< index of object */
       UNSIGNED8  subIndex, /**< subindex of object */
       UNSIGNED8  attribute /**< attribute of object */
       )
{
OBJDIR_T *pObjEntry;	/* pointer to object entry */

    pObjEntry = searchObj(index CO_COMMA_LINE_PARA);
    if (pObjEntry == NULL) {
	return(CO_FALSE);
    }

    if (subIndex > (pObjEntry->numOfElem - 1))  {
	return(CO_FALSE);
    }

    /* test for short arrays */
    if ((pObjEntry->pValDesc[0].attribute & CO_SHORT_ARRAY_DESC) != 0) {
	pObjEntry->pValDesc[1].attribute = attribute;
    } else {
	pObjEntry->pValDesc[subIndex].attribute = attribute;
    }
    return(CO_TRUE);
}


/* function for CANopen domain transfer */
#ifdef CONFIG_DOMAIN_UPDNLD
/****************************************************************************/
/**
*
*++ \brief getDomainSize - get the size information of a domain object
*-- \brief getDomainSize - ermittelt die Größeninformation eines Domainobjektes
*
*++ This function gets the size information of the domain object
*++ referenced by \em index.
*++ This is necessary, because the user is responsible for
*++ the domain target location and size.
*-- Die Funktion ermittelt die Größeninformation des über
*-- \em index und referenzierten Domainobjektes.
*-- Diese Funktion ist notwendig, da der Anwender für den
*-- Domainspeicherbereich verantwortlich ist.
*
* \retval 0
*++ object doesn't exist or size is zero
*-- Objekt existiert nicht oder Größe ist 0
* \retval size
*++ size of domain location
*-- Größe des Domainspeicherbereiches
*
*/

UNSIGNED32 getDomainSize(
	   UNSIGNED16 index,    /**< index of object */
	   UNSIGNED8  subIndex  /**< subindex of object */
	   )
{
LIST_ELEMENT_T *pObjEntry;	/* pointer to object entry */

    pObjEntry = searchObj(index CO_COMMA_LINE_PARA);
    if (pObjEntry == NULL) {
	return(0);
    }
    return(pObjEntry->pValDesc[subIndex].defaultVal);
}

/****************************************************************************/
/**
*
*++ \brief setDomainSize - set the size information of a domain object
*-- \brief setDomainSize - setzt die Größeninformation eines Domainobjektes
*
*++ This function sets the size information of the domain object
*++ referenced by \em index.
*++ This is necessary, because the user is responsible for
*++ the domain target location and size.
*-- Die Funktion setzt die Größeninformation des über
*-- \em index und referenzierten Domainobjektes.
*-- Diese Funktion ist notwendig, da der Anwender für den
*-- Domainspeicherbereich und dessen Größe verantwortlich ist.
*
* \retval CO_TRUE
*++ success
*-- Erfolg
* \retval CO_FALSE
*++ object doesn't exist
*-- Objekt existiert nicht
*
*/

BOOL_T setDomainSize(
       UNSIGNED16 index,    /**< index of object */
       UNSIGNED8  subIndex, /**< subindex of object */
       UNSIGNED32 size      /**< attribute of object */
       )
{
LIST_ELEMENT_T *pObjEntry;	/* pointer to object entry */

    pObjEntry = searchObj(index CO_COMMA_LINE_PARA);
    if (pObjEntry == NULL) {
	return(CO_FALSE);
    }

    /* test for short arrays */
    if ((pObjEntry->pValDesc[0].attribute & CO_SHORT_ARRAY_DESC) != 0)  {
	pObjEntry->pValDesc[1].defaultVal = size;
    } else  {
	pObjEntry->pValDesc[subIndex].defaultVal = size;
    }

    return(CO_TRUE);
}

/****************************************************************************/
/**
*
*++ \brief getDomainAddr - get the address of a domain object
*-- \brief getDomainAddr - ermittelt die Adresse eines Domainobjektes
*
*++ This function gets the address of the domain object
*++ referenced by \em index and \em subIndex.
*++ This is necessary, because the user is responsible for
*++ the domain target location.
*-- Die Funktion ermittelt die Adresse des über
*-- \em index und \em subIndex referenzierten Domainobjektes.
*-- Diese Funktion ist notwendig, da der Anwender für den
*-- Domainspeicherbereich verantwortlich ist.
*
* \retval NULL
*++ object doesn't exist or address is NULL
*-- Objekt existiert nicht oder Adresse ist 0
* \retval address
*++ address of domain location
*-- Adresse des Domainspeicherbereiches
*
*/

UNSIGNED8 *getDomainAddr(
	   UNSIGNED16 index,    /**< index of object */
	   UNSIGNED8  subIndex  /**< subindex of object */
	   )
{
UNSIGNED8  *pAddr;	/**< destination for data address*/
UNSIGNED32 size ;	/**< destination for data size */

    /* check for domain entry */
    if (((getObjAttr(index, subIndex CO_COMMA_LINE_PARA) & CO_UP_DN_LD_DOMAIN))
		== 0)  {
	return(NULL);
    }

    /* get pointer to address */
    if (getObjAddr(index, subIndex, &pAddr, &size CO_COMMA_LINE_PARA) != CO_OK){
	return(NULL);
    }

    return((UNSIGNED8 *)(*(UPDNLD_DOMAIN_T **)pAddr));
}


/****************************************************************************/
/**
*
*++ \brief setDomainAddr - set the address of a domain object
*-- \brief setDomainAddr - setzt die Adresse eines Domainobjektes
*
*++ This function sets the address of the domain object
*++ referenced by \em index and \em subIndex.
*++ This is necessary, because the user is responsible for
*++ the domain target location.
*-- Die Funktion setzt die Adresse des über
*-- \em index und \em subIndex referenzierten Domainobjektes.
*-- Diese Funktion ist notwendig, da der Anwender für den
*-- Domainspeicherbereich verantwortlich ist.
*
* \retval CO_TRUE
*++ success
*-- Erfolg
* \retval CO_FALSE
*++ object doesn't exist
*-- Objekt existiert nicht
*
*/
BOOL_T setDomainAddr(
	UNSIGNED16 index,    /**< index of object */
	UNSIGNED8  subIndex, /**< subindex of object */
	UNSIGNED8  *addr     /**< address of object */
       )
{
UNSIGNED8  *pAddr;	/**< destination for data address*/
UNSIGNED32 size ;	/**< destination for data size */

    /* check for domain entry */
    if (((getObjAttr(index, subIndex CO_COMMA_LINE_PARA) & CO_UP_DN_LD_DOMAIN))
		== 0)  {
	return(CO_FALSE);
    }

    /* get pointer to address */
    if (getObjAddr(index, subIndex, &pAddr, &size CO_COMMA_LINE_PARA) != CO_OK){
	return(CO_FALSE);
    }

    *(void **)pAddr = addr;

    return(CO_TRUE);
}
#endif /* CONFIG_DOMAIN_UPDNLD */


/****************************************************************************/
/**
*
*++ \brief getNumOfElem - gets the number of elements of an array or struct
*-- \brief getNumOfElem - liefert Anzahl der Elemente eines Arrays oder Struktur
*
*++ The number of elements of the object referenced by \em index
*++ of an array or record is returned.
*-- Es wird die Anzahl der Elemente eines Array- oder Record-Objektes
*-- im Objektverzeichnis ermittelt.
*-- Das Objekt wird über den angegebenen \em index adressiert.
*
* \return
*++ number of elements if > 0
*-- Anzahl der Elemente, wenn > 0
*++ success
*-- Erfolg
* \retval 0
*++ object doesn't exist
*-- Objekt existiert nicht
*
*/

UNSIGNED8 getNumOfElem (
	UNSIGNED16 index   /**< main index of variable */
	)
{
OBJDIR_T *pObjEntry;	/* pointer to object entry */

    pObjEntry = searchObj(index CO_COMMA_LINE_PARA);
    if (pObjEntry == NULL) {
	return(0);
    }
    return(pObjEntry->numOfElem);
}


/*******************************************************************/
/**
*++ \brief getOvDataTypeLen - get length of standard data types
*-- \brief getOvDataTypeLen - gibt die Länge von Standard Daten Typen zurück
*
*++ This function returns the length of standard data types.
*++ It is only valid for data types 1..7 (index 1..7).
*++ If it's called by invalid index,
*++ it returns a length of 0.
*++ The returned length is in bits.
*-- Diese Funktion liefert die Länge von Standard Datentypen zurück.
*-- Sie kann nur für die Datentypen 1..7 (index 1..7) verwendet werden.
*-- Wenn ein ungültiger Index übergeben wurde,
*-- wird die Länge 0 zurückgeliefert.
*-- Die Länge wird in Bits angeben.
*
* \return
*++ length of standard data types in bit
*-- Länge von Standard Daten Typ in Bit
*
*/
UNSIGNED8 getOvDataTypeLen(
	UNSIGNED16	index		/**< index */
    )
{
UNSIGNED8 lenTab[] = { 0, 1, 8, 16, 32, 8, 16, 32 };

    /* only index from 1..7 are allowed */
    if (index < 8) {
	return(lenTab[index]);
    }  else {
	/* bad index */
	return(0);
    }
}


/*______________________________________________________________________EOF_*/
