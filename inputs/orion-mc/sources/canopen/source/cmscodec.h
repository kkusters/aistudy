/*
 * - defines for codec routines
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
 * Revision 1.0  2008-03-07 17:05:28+01  driet
 * Initial revision
 *
 * Revision 2.4  2002/05/21 12:44:37  boe
 * change copyright
 *
 * Revision 2.3  2001/05/10 12:27:18  boe
 * structures for Data mapping changed (implizites changed commandlines)
 *
 * Revision 2.2  2001/02/26 14:47:21  boe
 * sdo.h included
 *
 * Revision 2.1  2001/01/26 10:42:31  boe
 * defines for codec functions
 *
 *
 *
 *------------------------------------------------------------------
 */

/*
DESCRIPTION

The file contains definitions of structures and data types for
codec functions
*/

#ifndef __CMSCODEC_H
# define __CMSCODEC_H

# include "cmsevent.h"

/* defines for function compiling */
/* #define CONFIG_NO_SDOCODEC_MACRO */
/* #define CONFIG_FAST_GETOBJ */

#ifdef CONFIG_NO_SDOCODEC_MACRO
# include "sdo.h"
UNSIGNED8	CMS_SdoEncode (MULTIPLEXOR_T *, UNSIGNED8* );
void		CMS_SdoDecode (MULTIPLEXOR_T *, UNSIGNED8* );
#else /* CONFIG_NO_SDOCODEC_MACRO */

/* CMS DeCode for SDO Multiplexor
 * Para 1: MULTIPLEXOR_T *pValue	pointer to application data
 * Para 2: UNSIGNED8   *pData		pointer to CAN message buffer
 */
# ifdef CONFIG_BIG_ENDIAN
#  define CMS_SdoDecode(pValue, pData)	\
    *((UNSIGNED8 *)pValue + 0) = pData[2];		\
    *((UNSIGNED8 *)pValue + 1) = pData[1];		\
    *((UNSIGNED8 *)pValue + 2) = pData[3];
# elif defined(CONFIG_16BIT_CPU)
#  error No Macro for CMS_SdoDecode defined
# else
#  define CMS_SdoDecode(pValue, pData) memcpy(pValue, pData+1, 3);
# endif

/* CMS EnCode for SDO Multiplexor
 * Para 1: MULTIPLEXOR_T *pValue	pointer to application data
 * Para 2: UNSIGNED8   *pData		pointer to CAN message buffer
 */
# ifdef CONFIG_BIG_ENDIAN
#  define CMS_SdoEncode(pValue, pData) \
    pData[2] = *((UNSIGNED8 *)pValue + 0);		\
    pData[1] = *((UNSIGNED8 *)pValue + 1);		\
    pData[3] = *((UNSIGNED8 *)pValue + 2);
# elif defined(CONFIG_16BIT_CPU)
#  error No Macro for CMS_SdoEncode defined
# else
#  define CMS_SdoEncode(pValue, pData)	memcpy((pData)+1, pValue, 3);
# endif

#endif /* CONFIG_NO_SDOCODEC_MACRO */

/* external data declarations */

/* function prototypes */

UNSIGNED8	CMS_Encode (PDO_MAP_T *, UNSIGNED8 *, UNSIGNED8 * );
void		CMS_Decode (PDO_MAP_T *, UNSIGNED8 *, UNSIGNED8 * );
UNSIGNED8	CMS_MapEncode (PDO_MAP_T *, UNSIGNED8 * );
UNSIGNED8	CMS_MapDecode (PDO_MAP_T *, UNSIGNED8 * );

#endif		/*  __CMSCODEC_H */

/* end of source */

