/*
 * sdo - defines for sdo usage
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
 * Revision 1.0  2008-03-07 17:05:54+01  driet
 * Initial revision
 *
 * Revision 2.8  2003/10/01 13:27:38  boe
 * delete unused line parameter for setSdoCobId
 *
 * Revision 2.7  2003/06/13 14:08:29  boe
 * new formatted
 *
 * Revision 2.6  2003/02/13 09:52:07  boe
 * save values for split_indication at sdo_t
 *
 * Revision 2.5  2002/11/18 10:39:10  boe
 * add/change prototypes
 * add co_ to all global library variables
 * remove defines for block transfer
 *
 * Revision 2.4  2002/05/21 14:28:02  boe
 * copyright changed
 *
 * Revision 2.3  2001/04/12 14:18:26  boe
 * duplicate prototype removed
 *
 * Revision 2.2  2001/03/14 15:58:12  ro
 * prototype setDefSdoCobId() added
 *
 * Revision 2.1  2001/01/26 11:17:48  boe
 * defines for sdo usage
 *
 *
 *
 *------------------------------------------------------------------
 */

/*
DESCRIPTION

The file contains definitions of structures and data types for sdo usage

*/

#ifndef __SDO_H
# define __SDO_H

# include <co_stru.h>
# include <co_sdo.h>

/* Multiplexortype for SDO */
typedef struct
{
    UNSIGNED16 index;
    UNSIGNED8  subIndex;
} MULTIPLEXOR_T;


/* structure of a CMS domain */

struct CMS_DOMAIN {
struct	CMS_DOMAIN  *pNext;          /* pointer to next domain */
	COB_T       *pReqInd_COB;    /* COB for Request/Indication */
	COB_T       *pResCon_COB;    /* COB for Response/Confirmation */
	UNSIGNED32  domSize;         /* size of whole domain */
	UNSIGNED32  restSize;        /* size of not transfered data */
	MULTIPLEXOR_T odIndex;	     /* Index and Subindex to od */
	UNSIGNED16  timeOut;         /* time out value in 1/10 ms */
	USER_T      userType;        /* Client <-> Server */
	UNSIGNED8   pData[8];        /* data buffer */
	UNSIGNED8   toggleBit;       /* toggle bit */
	UNSIGNED8   *pDomData;       /* pointer to data location */
	UNSIGNED8   *pActualDomData; /* pointer to actual data position for upload */
	UNSIGNED8   num;             /* code number of domain */
	UNSIGNED8   state;	     /* actual sdo state */
#ifdef CONFIG_SDO_BLOCKTRANSFER
# ifdef CONFIG_BLOCK_CRC
	UNSIGNED16  blkCrcSum;	     /* CRC Checksum */
# endif
	UNSIGNED8   blkSegSize;	     /* block segment size */
	UNSIGNED8   blkSegNr;	     /* actual block segment number */
	BOOL_T	    blkCRC;	     /* block transfer uses CRC check */
#endif
#ifdef CONFIG_SPLIT_INDICATION
	UNSIGNED8   oldVar[4];        /* buffer for saving former value */
	BOOL_T	    saved;	      /* flag signs if old value is stored */
#endif /* CONFIG_SPLIT_INDICATION */
#if defined(CONFIG_BIG_ENDIAN) || defined(CONFIG_16BIT_CPU)
	BOOL_T	    numeric;         /* flag shows numeric contents of SDO
					only for BIG_ENDIAN/16bit devices */
#endif /* defined(CONFIG_BIG_ENDIAN) || defined(CONFIG_16BIT_CPU) */
#if defined(CONFIG_16BIT_CPU)
	BOOL_T	    halfWord;	     /* half word not processed */
#endif /* defined(CONFIG_16BIT_CPU) */
};

typedef struct  CMS_DOMAIN 	CMS_DOMAIN_T;
typedef struct  CMS_DOMAIN 	SDO_T;


# define CO_SIZE_VALID	1  


/* D O M A I N */

#define EXPED_TRANSFER		0x02	/* field e is setting to 1 */


  /* C C S   =   C l i e n t   C o m m a n d   S p e c i f i e r */

#define CO_SDO_CCS_MASK		0xe0	/* client command specifier */

#define CCS_INI_DN_LD_REQ	0x20	/* Initiate Download Request */
#define CCS_DN_LD_SEG_REQ	0x00	/* Download Segment Request */
#define CCS_INI_UP_LD_REQ	0x40	/* Initiate Upload Request */
#define CCS_UP_LD_SEG_REQ	0x60	/* Upload Segment Request */

  /* S C S   =   S e r v e r C o m m a n d   S p e c i f i e r */

#define SCS_INI_DN_LD_RES	0x60	/* Initiate Download Response */
#define SCS_DN_LD_SEG_RES	0x20	/* Download Segment Response */
#define SCS_INI_UP_LD_RES	0x40	/* Initiate Download Response */
#define SCS_UP_LD_SEG_RES	0x00	/* Upload Segment Response */

  /* C S   =   C o m m a n d   S p e c i f i e r */

#define CS_ABORT_TRANSFER	0x80	/* Abort Transfer Request */

#define SDO_TOGGLE_BIT		0x10	/* SDO Toggle Bit */

#define CO_SDO_SIZE_TYPE_MASK	0x3	/* sdo size type mask (e + s bit) */
#define CO_SDO_SCS_MASK		0xe0	/* client command specifier mask */

#define CO_SDO_MORE		0	/* more data for down/uploading */
#define CO_SDO_LAST		1	/* no more data for down/uploading */

/* SDO states */
#define SDOSTATE_DISABLED	0x00
#define SDOSTATE_IND_BUSY	0x10
#define SDOSTATE_READY		0x20
#define SDOSTATE_DNLD		0x80
#define SDOSTATE_UPLD		0x40
#define SDOSTATE_DNLD_INIT	(SDOSTATE_DNLD + 1)
#define SDOSTATE_DNLD_SEG	(SDOSTATE_DNLD + 2)
#define SDOSTATE_DNLD_BLK_INIT	(SDOSTATE_DNLD + 3)
#define SDOSTATE_DNLD_BLK_SEG	(SDOSTATE_DNLD + 4)
#define SDOSTATE_DNLD_BLK_END	(SDOSTATE_DNLD + 5)
#define SDOSTATE_UPLD_INIT	(SDOSTATE_UPLD + 1)
#define SDOSTATE_UPLD_SEG	(SDOSTATE_UPLD + 2)
#define SDOSTATE_UPLD_BLK_INIT	(SDOSTATE_UPLD + 3)
#define SDOSTATE_UPLD_BLK_SEG	(SDOSTATE_UPLD + 4)
#define SDOSTATE_UPLD_BLK_END	(SDOSTATE_UPLD + 5)



/* external data declarations */
extern CMS_DOMAIN_T	*co_pFirstDomEntry	;
#ifdef CONFIG_DYN_SDO_CONNECTION
extern UNSIGNED8	coSdoConError	;
#endif /* CONFIG_DYN_SDO_CONNECTION */


/* function prototypes */
void		sdoServerMsgInd(SDO_T *pSdo, CAN_MSG_T *canMsg);
void		sdoClientMsgCon(SDO_T *pSdo, CAN_MSG_T *canMsg);
CMS_DOMAIN_T	*CMS_DomExist (UNSIGNED8, USER_T );
RET_T		initUpDnLd_req(SDO_T *, UNSIGNED8 *, UNSIGNED32, UNSIGNED8
			);
RET_T		abortSdoTransf_Req  (SDO_T *, RET_T);
void		dnLdBlk_ind(SDO_T *, UNSIGNED8 *);
void		initDnLdBlk_ind(SDO_T *, UNSIGNED8 * );
void		endDnLdBlk_ind(SDO_T *, UNSIGNED8 *);
void		initUpLdBlk_ind(SDO_T *, UNSIGNED8 * );
void		dnLdBlkEnd_req(SDO_T *pSdo);
void		upLdBlkEnd_req(SDO_T *pSdo);
void		initUpLd_res(SDO_T *);
void		upLdSeg_ind(SDO_T *);
RET_T		initUpLd_ind(SDO_T *pCurSdo, UNSIGNED8 *canBuf
			);
void		dnLdBlk_con	(SDO_T *, UNSIGNED8 *);
void		upLdBlk_con	(SDO_T *, UNSIGNED8 * );
void		upLdBlk_ind	(SDO_T *, UNSIGNED8 *);
void		initUpLdBlk_con	(SDO_T *, UNSIGNED8 *);
void	 	setDefSdoCobId(void);

RET_T		setSdoCobId(SDO_T *pSdo, UNSIGNED32 cobId, UNSIGNED8 subIndex);

#endif		/*  __SDO_H */

/* end of source */

