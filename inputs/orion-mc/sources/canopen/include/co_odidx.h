/*
 * co_odidx - declarations for public od index
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
 * Revision 1.0  2008-03-07 17:05:39+01  driet
 * Initial revision
 *
 * Revision 2.8  2003/07/22 07:27:53  boe
 * add defines for program download
 *
 * Revision 2.7  2002/05/21 14:55:34  boe
 * copyright changed
 *
 * Revision 2.6  2002/04/05 13:27:12  boe
 * add entries for srdo access
 *
 * Revision 2.5  2001/08/22 08:23:01  boe
 * index for SRD connection request added
 *
 * Revision 2.4  2001/06/19 15:30:26  boe
 * added define EMCY_CONSUMER_INDEX
 *
 * Revision 2.3  2001/05/10 14:01:43  boe
 * adaption for new ds307 names (change master to manager)
 *
 * Revision 2.2  2001/03/14 17:25:35  ro
 * Flying Master Support added
 *
 * Revision 2.1  2001/01/26 12:39:37  boe
 * index at the object dictionary
 *
 *
 *
 *------------------------------------------------------------------
 */

/*
DESCRIPTION

The file contains definitions of structures and complex data types
for the public od index

*/

#ifndef __CO_ODIDX_
# define __CO_ODIDX_

/* od index defines */
#define ERROR_FIELD_INDEX	0x1003

#define SYNC_COB_ID_INDEX       0x1005
#define COMM_CYCLE_INDEX        0x1006

#define GUARD_TIME_INDEX        0x100C
#define LIFE_TIME_FAC_INDEX     0x100D

#define STORE_PARA_INDEX	0x1010
#define RESTORE_DEF_PARA_INDEX	0x1011
#define TIME_COB_ID_INDEX       0x1012

#define EMCY_COB_ID_INDEX	0x1014
#define EMCY_INHIBIT_INDEX	0x1015
#define HEARTBEAT_CON_INDEX	0x1016
#define HEARTBEAT_PROD_INDEX	0x1017
#define IDENTITY_INDEX		0x1018

#define EMCY_CONSUMER_INDEX	0x1028

#define SSDO_PARA_BASE_INDEX    0x1200
#define SSDO_PARA_LAST_INDEX    0x127F
#define CSDO_PARA_BASE_INDEX    0x1280
#define CSDO_PARA_LAST_INDEX    0x12FF

#define SRDO_GFC		0x1300
#define SRDO_PARA_BASE_INDEX	0x1301
#define SRDO_PARA_LAST_INDEX	0x1340
#define SRDO_MAP_BASE_INDEX	0x1381
#define SRDO_MAP_LAST_INDEX	0x13C0
#define SRDO_CONFIG_VALID	0x13FE
#define SRDO_CONFIG_CHECKSUM	0x13FF

#define RPDO_PARA_BASE_INDEX    0x1400
#define RPDO_PARA_LAST_INDEX    0x15FF
#define RPDO_MAP_BASE_INDEX     0x1600
#define RPDO_MAP_LAST_INDEX     0x17FF

#define TPDO_PARA_BASE_INDEX    0x1800
#define TPDO_PARA_LAST_INDEX    0x19FF
#define TPDO_MAP_BASE_INDEX     0x1A00
#define TPDO_MAP_LAST_INDEX     0x1BFF

#define SRD_REQUEST_SDO_INDEX	0x1F00
#define SRD_RELEASE_SDO_INDEX	0x1F01
#define SRD_COBID_TAB_INDEX	0x1F02
#define SRD_CONNPART1_INDEX	0x1F03
#define SRD_CONNPART2_INDEX	0x1F04
#define SRD_CONNPART3_INDEX	0x1F05
#define SRD_CONNPART4_INDEX	0x1F06
#define SRD_CONN_REQ		0x1F10

#define DOWNLOAD_PROGRAM_DATA	0x1F50
#define DOWNLOAD_PROGRAM_CONTROL 0x1F51

#define MPDO_SCANNER_LIST_INDEX	0x1FA0
#define MPDO_SCANNER_LIST_LAST	0x1FCF
#define MPDO_DISP_LIST_INDEX	0x1FD0
#define MPDO_DISP_LIST_LAST	0x1FFF

#define FLYMANAGER_TIMEPAR	0x6000
#define FLYMANAGER_CAPDEVPAR	0x6001




#endif /* __CO_ODIDX_ */

/* end of source */

