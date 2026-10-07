/*
 * cmscodec - modul for cms decoder and encoder functions
 *
 * Copyright (c) 1996-2002 port GmbH Halle (Saale)
 *------------------------------------------------------------------
 * $Header$
 *
 *------------------------------------------------------------------
 *
 * modification history
 * --------------------
 * $Log$
 * Revision 1.1  2008-03-26 17:06:00+01  driet
 * <>
 *
 * Revision 1.0  2008-03-07 17:04:37+01  driet
 * Initial revision
 *
 * Revision 2.13  2003/06/13 13:17:29  boe
 * check for numeric values to eBasicType == CO_UNSIGNED
 *
 * Revision 2.12  2002/11/14 09:45:45  boe
 * correct calculation of mapping counter for bitwise mapping
 * add type castings
 * adapt comments on doxygen
 *
 * Revision 2.11  2002/05/21 14:01:46  boe
 * change copyright,
 * solve error for bit coding
 *
 * Revision 2.10  2001/10/12 13:45:57  boe
 * change access for 16bit dsp cpus
 *
 * Revision 2.9  2001/05/10 12:26:59  boe
 * structures for Data mapping changed (implizites changed commandlines)
 *
 * Revision 2.8  2001/03/28 15:16:35  boe
 * unsused variables for BIG_ENDIAN removed
 *
 * Revision 2.7  2001/02/26 14:46:06  boe
 * big endian changed
 *
 * Revision 2.6  2001/01/26 10:40:24  boe
 * split include files into function specific headers
 *
 * Revision 2.5  2000/10/05 11:35:11  boe
 * memcpy replaced by CO_MEMCPY/CO_NUM_MEMCPY
 * special thinks for real 16 Bit CPUs added
 * PDO_SERVER and PDO_CLIENT renamed as PDO_CONSUMER/PRODUCER
 *
 * Revision 2.4  2000/06/13 08:27:41  boe
 * function names (H_) changed
 *
 * Revision 2.3  2000/04/19 07:30:21  boe
 * variables optimization
 *
 * Revision 2.2  2000/04/18 08:32:21  boe
 * special datatype DATA replaced with CO_DATA
 *
 * Revision 2.1  2000/03/28 14:19:36  boe
 * adaption for multi-line version
 * debug prints removed
 *
 * Revision 2.0  2000/01/21 11:01:04  boe
 * Überarbeitet und an Version 4.0 angepasst
 *
 *
 *
 *------------------------------------------------------------------
 */


/*
* \file cmscodec.c
* \author port GmbH, Halle
* $Revision$
* $Date$
*
* This modul contains functions for encoding and coding of CMS objects.
* The function CMS_Encode transforms implementation dependent data structures
* into a transfer syntax corresponding the encoding rules
* defined by CiA in CiA/DS202-3 p. 2.
* CMS_Decode works in the opposite direction. It transforms
* a received message back into the defined data structure.
* 
* the data types are stored as followed.
* 
* \code
* - BOOLEAN         unsigned char (Bit 0 byte aligned)
* - INTEGER(1..8)   char (byte aligned)
* - INTEGER(9..16)  short (word aligned)
* - INTEGER(17..32) long (word aligned)
* - INTEGER(33..64) two long (Word Aligned, first LOW double Word)
* - UNSIGNED(x..y)  like INTEGER(x..y) with signed instead unsigned
* - FLOAT       like INTEGER(32) (not specified yet)
* - DUMMY_SPACE     not stored, but contains information length
* - NIL             not stored
* \endcode
* 
* The compiler directive CONFIG_BIT_ENCODING enables/disables bitewise 
* or bytewise encoding/decoding
* bytewise encoding/decoding has a better performance
* (code space) and run time behaviour.
*
* All of these functions are only called from within the library
* and not from the library user.
* Therefore there are no manual entries of the functions available.
* 
*/


/* header of standard C - libraries */

#include <string.h>
#include <stdio.h>

/* header of project specific types */

#include <cal_conf.h>

#include <co_mcpy.h>
#include "cmscodec.h"
#include "pdo.h"
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


#ifdef CONFIG_BIT_ENCODING

# ifdef CONFIG_PDO_PRODUCER
/*******************************************************************
*
* CMS_MapEncode - encode CMS objects for Mapping
*
* NOMANUAL
*
* CMS_MapEncode transforms pdo mapping data
* into a transfer syntax corresponding the encoding rules
* defined by CiA in CiA/DS202-3 p. 2.
*
* RETURNS
* .TP
* telegram length
*
*/

UNSIGNED8 CMS_MapEncode(
      PDO_MAP_T   *pMapList,  /* pointer to mapping list for PDOs */
      UNSIGNED8   *pData      /* pointer to CAN message buffer */
     )
{
UNSIGNED8 CO_DATA destBits = 0, /* number of bits in destination bytes */
        sourceBits = 0, /* number of bits in source bytes */
        bitSize;    /* bitsize */
UNSIGNED8   *pValue;    /* pointer to application data */
#  ifdef CONFIG_BIG_ENDIAN
UNSIGNED8   byteCnt;    /* byte counter */
#  endif /* CONFIG_BIG_ENDIAN */
PDO_MAP_T   *pCurMapObj = NULL; /* pointer to mapping structure */

    /* check maplist */
    if (pMapList == NULL)  {
    return(0);
    }

    /* if no value parameter then load address of value from mapping
    entry */
    /* if no mapping exist return */
    if (pMapList->eBasicType == CO_INVALID)  {
    return 0;
    }

    pCurMapObj = pMapList;

    /* init values */
    memset(pData, 0, 8);

    /* allocate security mechanism for object dictionary consistency */
    CO_COM_PART_ALLOC(CO_LINE_PARA);
    CO_APPL_PART_ALLOC(CO_LINE_PARA);

    do {
    pValue = pCurMapObj->pAddress;
    bitSize = pMapList->bBitSize;

    if ((destBits + bitSize) <= 64)  {

        if (pMapList->eBasicType != CO_DUMMY_SPACE) {
        /* determine alignment  */
        if (((bitSize % 8) == 0) && ((destBits % 8) == 0))  {
            /* copy bytewise */
            CO_UNPACK_MEMCPY(&pData[destBits >> 3], pValue,bitSize >> 3,
            (pMapList->eBasicType == CO_UNSIGNED));
            destBits += bitSize;
        } else {
            /* copy bitwise */
            sourceBits = bitSize - 1;

            while (bitSize > 0) {

#  ifdef CONFIG_BIG_ENDIAN
            byteCnt = (bitSize >> 3) + ((bitSize & 7) ? 1 : 0);
            byteCnt --;
            /* if sourcebit = 1 */
            if ((pValue[byteCnt] & (1 << (sourceBits & 0x7))) != 0){
                /* set destination bit */
                pData[destBits >> 3] |= (1 << (7 - (destBits & 0x7)));
            }

#  elif defined(CONFIG_16BIT_CPU)
            /* we have 16 bit for each address */
            if ((pValue[sourceBits >> 4] &
                (1 << (sourceBits & 0xf))) != 0)  {
                /* set destination bit */
                pData[destBits >> 3] |= (1 << (7 - (destBits & 0x7)));
            }
#  else /* CONFIG_16BIT_CPU */
            /* we have 8 bit for each address */
            if ((pValue[sourceBits >> 3] &
                (1 << (sourceBits & 0x7))) != 0) {
                /* set destination bit */
                pData[destBits >> 3] |= (1 << (7 - (destBits & 0x7)));
            }
#  endif /* CONFIG_16BIT_CPU */

            destBits++;
            sourceBits--;
            bitSize--;
            }
        }

        } else  {
        /* dummy */
        destBits += bitSize;
        }
    } else {
        /* for real calculation of mappings */
        destBits += bitSize;
    }
    /* get next address if mapping */
    /* if last valid element for PDO mapping exit loop*/
    pMapList = pMapList->pNext;
    if ((pMapList == NULL) || (pMapList->eBasicType == CO_INVALID))  {
        break;
    }
    pCurMapObj = pCurMapObj->pNext;
    } while (pMapList != NULL);

    /* release security mechanism for object dictionary consistency */
    CO_COM_PART_RELEASE(CO_LINE_PARA);
    CO_APPL_PART_RELEASE(CO_LINE_PARA);

    bitSize = destBits >> 3;
    if ((destBits % 8) != 0)  {
    bitSize++;
    }
    return(bitSize);
}
# endif /* PDO_PRODUCER */


# ifdef CONFIG_PDO_CONSUMER
/*******************************************************************
*
* CMS_MapDecode - decode CMS objects for Mapping
*
* NOMANUAL
*
* CMS_MapDecode transforms a transfer syntax corresponding the encoding rules
* defined by CiA in CiA/DS202-3 p. 2
* into pdo data structures.
*
* RETURNS
*   necessary byte count
*
*/

UNSIGNED8 CMS_MapDecode(
     PDO_MAP_T   *pMapList,  /* pointer to mapping list for PDOs */
     UNSIGNED8   *pData      /* pointer to CAN message buffer */
     )
{
UNSIGNED8   *pValue;        /* pointer to application data */
PDO_MAP_T   *pCurMapObj;    /* pointer to mapping structure */
UNSIGNED32  tmpVar;         /* temp u32 var */
UNSIGNED8   sourceBits = 0; /* count of source bits */
UNSIGNED8   bitSize,        /* actual bitsize */
            destBit,        /* count of destination bits */
            byteCnt;        /* byte count */

  /* check maplist */
  if (pMapList == NULL)
  {
    return(0);
  }

  /* if no value parameter then load address of value from mapping entry */
  /* if no mapping exist return */
  if (pMapList->eBasicType == CO_INVALID)
  {
    return 0;
  }
  pCurMapObj = pMapList;
  pValue = pCurMapObj->pAddress;

  /* allocate security mechanism for object dictionary consistency */
  CO_COM_PART_ALLOC(CO_LINE_PARA);
  CO_APPL_PART_ALLOC(CO_LINE_PARA);
  do
  {
    bitSize = pMapList->bBitSize;
    if ((sourceBits + bitSize) <= 64)
    {
      if (pMapList->eBasicType != CO_DUMMY_SPACE)
      {
        /* determine alignment */
        if (((bitSize % 8) == 0) && ((sourceBits % 8) == 0))
        {
          /* copy bytewise */
          CO_PACK_MEMCPY(pValue, &pData[sourceBits >> 3],bitSize >> 3, (pMapList->eBasicType == CO_UNSIGNED));
          sourceBits += bitSize;
        }
        else
        {
          /* copy bitwise */
          destBit = bitSize - 1;
          tmpVar = 0;
          byteCnt = (bitSize >> 3) + ((bitSize & 7) ? 1 : 0);
          while (bitSize > 0)
          {
            /* if sourcebit = 1 */
            if ((pData[sourceBits >> 3] & (1 << (7 - (sourceBits & 0x7)))) != 0)
            {
              /* set destination bit */
              tmpVar |= (1 << destBit);
            }
            destBit--;
            sourceBits++;
            bitSize--;
          }
#  ifdef CONFIG_BIG_ENDIAN
          CO_PACK_MEMCPY(pValue, (UNSIGNED8 *)&tmpVar + 4-byteCnt, byteCnt, (pMapList->eBasicType == CO_UNSIGNED));
#  else /* CONFIG_BIG_ENDIAN */
          CO_PACK_MEMCPY(pValue, (UNSIGNED8 *)&tmpVar, byteCnt, (pMapList->eBasicType == CO_UNSIGNED));
#  endif /* CONFIG_BIG_ENDIAN */
        }
      }
      else
      {
        /* dummy */
        sourceBits += bitSize;
      }
    }
    else
    {
      /* for real calculation of mappings */
      sourceBits += bitSize;
    }

    /* get next address if mapping */
    pMapList = pMapList->pNext;
    /* if last valid element for PDO mapping exit loop*/
    if ((pMapList == NULL) || (pMapList->eBasicType == CO_INVALID))
    {
      break;
    }
    pCurMapObj = pCurMapObj->pNext;
    pValue = pCurMapObj->pAddress;
  } while (pMapList != NULL);

  /* release security mechanism for object dictionary consistency */
  CO_COM_PART_RELEASE(CO_LINE_PARA);
  CO_APPL_PART_RELEASE(CO_LINE_PARA);

  bitSize = sourceBits >> 3;
  if ((sourceBits % 8) != 0)
  {
    bitSize++;
  }
  return(bitSize);
}
# endif /* PDO_CONSUMER */


#else /* CONFIG_BIT_ENCODING */


# ifdef CONFIG_PDO_PRODUCER
/*******************************************************************
*
* CMS_MapEncode - encode CMS objects for Mapping
*
* NOMANUAL
*
* CMS_MapEncode transforms pdo mapping data
* into a transfer syntax corresponding the encoding rules
* defined by CiA in CiA/DS202-3 p. 2.
* This variant supports only bytewise encoding.
*
* RETURNS
* .TP
* telegram length in bytes
*
*/
UNSIGNED8 CMS_MapEncode(
      PDO_MAP_T   *pMapList,   /* pointer to mapping list for PDOs */
      UNSIGNED8   *pData     /* pointer to CAN message buffer */
     )
{
UNSIGNED8   *pValue;    /* pointer to application data */
UNSIGNED8   destOffs = 0,   /* destination offset in CAN telegram */
        length;     /* length in bytes of data item */
PDO_MAP_T   *pCurMapObj;    /* pointer to PDO mapping */


    /* check maplist */
    if (pMapList == NULL)  {
    return(0);
    }

    /* if no mapping exist return */
    if (pMapList->eBasicType == CO_INVALID)  {
    return 0;
    }

    pCurMapObj = pMapList;
    pValue = (UNSIGNED8 *)pCurMapObj->pAddress;

    /* allocate security mechanism for object dictionary consistency */
    CO_COM_PART_ALLOC(CO_LINE_PARA);
    CO_APPL_PART_ALLOC(CO_LINE_PARA);

    do {
    /* fill in the bits until end of bitsize (bytewise) */
    length = pMapList->bBitSize >> 3;
    destOffs += length;
    /* without dummy mapping */
    if ((destOffs < 9) && (pValue != NULL))  {
        CO_UNPACK_MEMCPY(pData, pValue, length,
        (pMapList->eBasicType == CO_UNSIGNED));
        pData += length;
    }

    /* get next address if mapping */
    /* if last valid element for PDO mapping exit loop*/
    pMapList = pMapList->pNext;
    if ((pMapList == NULL) || (pMapList->eBasicType == CO_INVALID))  {
        break;
    }
    pCurMapObj = pCurMapObj->pNext;
    pValue = (UNSIGNED8 *)pCurMapObj->pAddress;
    } while (pMapList != NULL);

    /* release security mechanism for object dictionary consistency */
    CO_COM_PART_RELEASE(CO_LINE_PARA);
    CO_APPL_PART_RELEASE(CO_LINE_PARA);
    return(destOffs);
}
# endif /* PDO_PRODUCER */


# ifdef CONFIG_PDO_CONSUMER
/*******************************************************************
*
* CMS_MapDecode - decode CMS objects for Mapping
*
* NOMANUAL
*
* CMS_MapDecode transforms a transfer syntax corresponding the encoding rules
* defined by CiA in CiA/DS202-3 p. 2
* into pdo data structures.
* This variant supports only bytewise encoding.
*
* RETURNS
* .TP
*   necessary byte count
*
*/
UNSIGNED8 CMS_MapDecode(
     PDO_MAP_T   *pMapList, /* pointer to mapping list for PDOs */
     UNSIGNED8   *pData     /* pointer to CAN message buffer */
     )
{
UNSIGNED8   *pValue;    /* pointer to application data */
UNSIGNED8   length;     /* length in bytes of data item */
UNSIGNED8   len = 0;    /* mapping  length */
PDO_MAP_T   *pCurMapObj;    /* pointer to PDO mapping */

    /* check maplist */
    if (pMapList == NULL)  {
    return(0);
    }

    /* if no value parameter then load address of value from mapping
    entry */
    /* if no mapping exist return */
    if (pMapList->eBasicType == CO_INVALID)  {
    return(0);
    }
    pCurMapObj = pMapList;
    pValue = (UNSIGNED8 *)pCurMapObj->pAddress;

    /* allocate security mechanism for object dictionary consistency */
    CO_COM_PART_ALLOC(CO_LINE_PARA);
    CO_APPL_PART_ALLOC(CO_LINE_PARA);

    do {
    length = pMapList->bBitSize >> 3;
    len += length;

    /* ignore dummy mapping */
    if (pMapList->eBasicType != CO_DUMMY_SPACE) {

        CO_PACK_MEMCPY(pValue, pData, length,
        (pMapList->eBasicType == CO_UNSIGNED));
        pData += length;
    }  else  {
        /* dummy mapping */
        pData += length;
    }

    /* get next address if PDO mapping */
    /* if last valid element for PDO mapping exit loop */
    pMapList = pMapList->pNext;
    if ((pMapList == NULL) || (pMapList->eBasicType == CO_INVALID))  {
        break;
    }
    pCurMapObj = pCurMapObj->pNext;
    pValue = (UNSIGNED8 *)pCurMapObj->pAddress;
    }
    while (pMapList != NULL);

    /* release security mechanism for object dictionary consistency */
    CO_COM_PART_RELEASE(CO_LINE_PARA);
    CO_APPL_PART_RELEASE(CO_LINE_PARA);

    return(len);
}
# endif /* PDO_CONSUMER */

#endif /* CONFIG_BIT_ENCODING */


#if defined(CONFIG_EMCY_PRODUCER) || defined(CONFIG_TIME_PRODUCER)
/*******************************************************************
*
* CMS_Encode - encode CMS objects
*
* NOMANUAL
*
* CMS_Encode transforms implementation dependent data structures
* into a transfer syntax corresponding the encoding rules
* defined by CiA in CiA/DS202-3 p. 2.
* This variant supports only bytewise encoding.
*
* RETURNS
* .TP
* telegram length in bytes
*
*/
UNSIGNED8 CMS_Encode(
      PDO_MAP_T   *pDataType, /* pointer to data type description */
      UNSIGNED8   *pValue,    /* pointer to application data */
      UNSIGNED8   *pData      /* pointer to CAN message buffer */
     )
{
UNSIGNED8   length,     /* length in bytes of data item */
        destOffs = 0;   /* destination offset in CAN telegram */

    /* allocate security mechanism for object dictionary consistency */
    CO_COM_PART_ALLOC(CO_LINE_PARA);
    CO_APPL_PART_ALLOC(CO_LINE_PARA);

    do {
    length = pDataType->bBitSize >> 3;
    destOffs += length;

    /* fill in the bits until end of bitsize (bytewise) */
    CO_UNPACK_MEMCPY(pData, pValue, length, CO_NUM_VAL);
    pData += length;
#ifdef CONFIG_16BIT_CPU
    pValue += (length + 1) >> 1;
#else /* CONFIG_16BIT_CPU */
    pValue += length;
#endif /* CONFIG_16BIT_CPU */

    /* next element */
    pDataType = pDataType->pNext;

# if CONFIG_ALIGNMENT > 1
    /* get right start address within a structure
       if CPU doesn't support byte alignment */
    /*
       test whether address is a multiple of the CONFIG_ALIGNMENT
       and the size of the next element must be greater or
       equal CONFIG_ALIGNMENT, because the compiler set array elements
       linear in the memory
     */

    if (pDataType != NULL) {
        length = pDataType->bBitSize >> 3;
#  ifdef CONFIG_16BIT_CPU
        length = (length + 1) >> 1;
#  endif /* CONFIG_16BIT_CPU */

        /* correct the alignment */
        if (length > CONFIG_ALIGNMENT)  {
        length = CONFIG_ALIGNMENT;
        }
        while( (UNSIGNED32)pValue % length) {
        pValue++;
        }
    }
# endif /* CONFIG_ALIGNMENT > 1 */

    } while (pDataType != NULL);

    /* release security mechanism for object dictionary consistency */
    CO_COM_PART_RELEASE(CO_LINE_PARA);
    CO_APPL_PART_RELEASE(CO_LINE_PARA);
    return(destOffs);
}
#endif /* defined(CONFIG_EMCY_PRODUCER) || defined(CONFIG_TIME_PRODUCER) */


#if defined(CONFIG_EMCY_CONSUMER) || defined(CONFIG_TIME_CONSUMER)
/*******************************************************************
*
* CMS_Decode - decode CMS objects
*
* NOMANUAL
*
* CMS_Decode transforms a transfer syntax corresponding the encoding rules
* defined by CiA in CiA/DS202-3 p. 2
* into implementation dependent data structures.
* This variant supports only bytewise encoding.
*
* RETURNS
* .TP
* nothing
*
*/
void CMS_Decode(
     PDO_MAP_T   *pDataType, /* pointer to data type description */
     UNSIGNED8   *pValue,    /* pointer to application data */
     UNSIGNED8   *pData      /* pointer to CAN message buffer */
     )
{
UNSIGNED8   nextByteSize;   /* byte size for next entry */

    /* allocate security mechanism for object dictionary consistency */
    CO_COM_PART_ALLOC(CO_LINE_PARA);
    CO_APPL_PART_ALLOC(CO_LINE_PARA);

    do {
    nextByteSize = pDataType->bBitSize >> 3;

    /* fill in the bits until end of bitsize (bytewise) */
    CO_PACK_MEMCPY(pValue, pData, nextByteSize, CO_NUM_VAL);
    pData += nextByteSize;
    pValue += nextByteSize;

    /* next element */
    pDataType = pDataType->pNext;

# if CONFIG_ALIGNMENT > 1
    /* get right start address within a structure
     if CPU doesn't support byte alignment */
    /*
     test whether address is a multiple of the CONFIG_ALIGNMENT
     and the size of the next element must be greater or
     equal CONFIG_ALIGNMENT, because the compiler set array elements
     linear in the memory
    */

    if (pDataType != NULL) {
        nextByteSize = pDataType->bBitSize >> 3;
#  ifdef CONFIG_16BIT_CPU
        nextByteSize = (nextByteSize + 1) >> 1;
#  endif /* CONFIG_16BIT_CPU */

        if (nextByteSize > CONFIG_ALIGNMENT)  {
        nextByteSize = CONFIG_ALIGNMENT;
        }
        while( (UNSIGNED32)pValue % nextByteSize) {
        pValue++;
        }
    }
# endif /* CONFIG_ALIGNMENT > 1 */
    /* get next address if PDO mapping */
    } while (pDataType != NULL);

    /* release security mechanism for object dictionary consistency */
    CO_COM_PART_RELEASE(CO_LINE_PARA);
    CO_APPL_PART_RELEASE(CO_LINE_PARA);
}
#endif /* defined(CONFIG_EMCY_CONSUMER) || defined(CONFIG_TIME_CONSUMER) */


#ifdef CONFIG_NO_SDOCODEC_MACRO
/*******************************************************************
*
* CMS_SdoDecode - decode CMS objects for SDO Transfers
*
* NOMANUAL
*
* CMS_MapDecode transforms a transfer syntax corresponding the encoding rules
* defined by CiA in CiA/DS202-3 p. 2
* into implementation dependent data structures.
* This variant supports only bytewise encoding.
*
* RETURNS
* .TP
* nothing
*
*/
void CMS_SdoDecode(
     MULTIPLEXOR_T *pValue,   /* pointer to application data */
     UNSIGNED8   *pData     /* pointer to CAN message buffer */
     )
{
    /* copy index and subindex */
#ifdef CONFIG_BIG_ENDIAN
    *((UNSIGNED8 *)pValue + 0) = pData[2];
    *((UNSIGNED8 *)pValue + 1) = pData[1];
    *((UNSIGNED8 *)pValue + 2) = pData[3];
# elif CONFIG_16BIT_CPU
    pValue->index = (pData[2] << 8) + (pData[1] & 0xFF);
    pValue->subIndex = pData[3];
# else /* CONFIG_16BIT_CPU */
    CO_MEMCPY(pValue, pData+1, 3);
# endif /* CONFIG_16BIT_CPU */
}
#endif /* CONFIG_NO_SDOCODEC_MACRO */


#ifdef CONFIG_NO_SDOCODEC_MACRO
/*******************************************************************
*
* CMS_SdoEncode - encode CMS objects for SDO Transfers
*
* NOMANUAL
*
* CMS_SdoDecode transforms a transfer syntax corresponding the encoding rules
* defined by CiA in CiA/DS202-3 p. 2
* into implementation dependent data structures.
* This variant supports only bytewise encoding.
*
* RETURNS
* .TP
*   count of mapped bytes
*/
UNSIGNED8 CMS_SdoEncode(
      MULTIPLEXOR_T   *pValue,   /* pointer to application data */
      UNSIGNED8   *pData     /* pointer to CAN message buffer */
     )
{
    /* copy index and subindex */
# ifdef CONFIG_BIG_ENDIAN
    pData[2] = *((UNSIGNED8 *)pValue + 0);
    pData[1] = *((UNSIGNED8 *)pValue + 1);
    pData[3] = *((UNSIGNED8 *)pValue + 2);
# elif CONFIG_16BIT_CPU
    pData[2] = (UNSIGNED8)(pValue->index >> 8);
    pData[1] = (UNSIGNED8)(pValue->index & 0xFF);
    pData[3] = pValue->subIndex;
# else /* CONFIG_16BIT_CPU */
    CO_MEMCPY(pData+1, pValue, 3);
# endif /* CONFIG_16BIT_CPU */
    return(4);
}
#endif /* CONFIG_NO_SDOCODEC_MACRO */
