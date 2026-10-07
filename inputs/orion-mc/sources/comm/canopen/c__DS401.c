// C__DS401.C

#include "ch_define.h"

#include <co_acces.h>
#include <co_nmt.h>
#include <co_odidx.h>
#include <co_pdo.h>
#include <co_setcp.h>
#include <co_stru.h>
#include <co_type.h>

#include "objects.h"
#include "ch_DS401.h"

static UNSIGNED16 oldAnaInput[33] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
UNSIGNED8 AnaOutValid[33] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
UNSIGNED8 DigOutValid[9]  = {0,0,0,0,0,0,0,0,0};

UNSIGNED8 intTPDO[12] = {0,0,0,0,0,0,0,0,0,0,0,0}; // interrupt flag for transmitting PDO's

//----------------------------------------------------------------
// Table handling
//----------------------------------------------------------------
UNSIGNED16 getTPdoNr(void *objAddr)
{
UNSIGNED16 pdoNr;
UNSIGNED8  mc;
void *mapAddr;

  // search the TPDO where object is mapped
  for (pdoNr = 1; pdoNr <= CONFIG_PDO_PRODUCER; pdoNr++)
  {
    mc = 1;
    while((mapAddr = getMapObjAddr(TPDO_MAP_BASE_INDEX, pdoNr, mc++)) != NULL)
    {
      if(mapAddr == objAddr)
      {
        return (pdoNr);
      }
    }
  }
  return (0);
}

//----------------------------------------------------------------
// PDO handling
//----------------------------------------------------------------
void servePdos(void)
{
UNSIGNED16 pdoNr;

  for (pdoNr = 1; pdoNr <= CONFIG_PDO_PRODUCER; pdoNr++)
  {
    if (intTPDO[pdoNr])
    {
      writePdoReq(pdoNr);
      intTPDO[pdoNr] = 0;
    }
  }
}

//----------------------------------------------------------------
// nr     : digital input to set/reset
// on_off : 1 = set digital input, 0 = reset digital input
//----------------------------------------------------------------
RET_T DS401_SetDigitalInput(UNSIGNED8 nr, UNSIGNED8 val)
{
UNSIGNED8 oldValue;
UNSIGNED8 SubIndex;
UNSIGNED8 Mask;

  SubIndex = (nr / 8) + 1;

  if (SubIndex > p401_ReadState8[0]) // Check if SubIndex exists
    return (CO_E_NOT_EXIST);

  Mask = (0x01 << (nr % 8));
  
  oldValue = p401_ReadState8[SubIndex];

  if (val)
    p401_ReadState8[SubIndex] |= Mask;
  else
    p401_ReadState8[SubIndex] &= ~Mask;

  if (p401_ReadState8[SubIndex] != oldValue)
  {
    UNSIGNED16 pdoNr;

    pdoNr = getTPdoNr(&p401_ReadState8[SubIndex]);

    if(pdoNr == 0)
      return (CO_E_MAP);  

    intTPDO[pdoNr] = 1;
  }
  return (CO_OK);
}

RET_T DS401_GetDigitalOutput(UNSIGNED8 nr, UNSIGNED8 *val)
{
UNSIGNED8 SubIndex;
UNSIGNED8 Mask;

  if (getNodeState() != OPERATIONAL) 
    return(CO_E_STATE);

  SubIndex = (nr / 8) + 1;

  if (SubIndex > p401_WriteState8[0]) // Check if SubIndex exists
    return (CO_E_NOT_EXIST);

  if (!DigOutValid[SubIndex])
    return (CO_E_BUSY);

  DigOutValid[SubIndex] = 0;
   
  Mask = (0x01 << (nr % 8));
  
//  *val = p401_WriteState8[SubIndex] & Mask;
  if (p401_WriteState8[SubIndex] & Mask)
    *val = 1;
  else
    *val = 0;

  return (CO_OK);
}

//----------------------------------------------------------------
// nr  : analog input to set
// val : new value for analog input [nr]
//----------------------------------------------------------------
RET_T DS401_SetAnalogInput(UNSIGNED8 nr, INTEGER16 val)
{
UNSIGNED8  SubIndex;
UNSIGNED16 pdoNr;

  SubIndex = nr + 1;

  if (SubIndex > p401_ReadAnin16[0]) // Check if SubIndex exists
    return (CO_E_NOT_EXIST);

  p401_ReadAnin16[SubIndex] = val;

  // if interrupt enabled
  if (p401_AninIntEnable)
  {
    if ((abs(p401_ReadAnin16[SubIndex] - oldAnaInput[SubIndex])) > p401_AninIntDelta[SubIndex])
    {
      // store it as last sent
      oldAnaInput[SubIndex] = p401_ReadAnin16[SubIndex];

      // set TPDO interrupt
      pdoNr = getTPdoNr(&p401_ReadAnin16[SubIndex]);

      if(pdoNr == 0)
        return (CO_E_MAP);  

      intTPDO[pdoNr] = 1;
	}
  }
  return (CO_OK);
}

RET_T DS401_GetAnalogOutput(UNSIGNED8 nr, INTEGER16 *val)
{
UNSIGNED8 SubIndex;

  if (getNodeState() != OPERATIONAL) 
    return(CO_E_STATE);
 
  SubIndex = nr + 1;
  
  if (SubIndex > p401_WriteAnout16[0]) // Check if SubIndex exists
    return (CO_E_NOT_EXIST);

  if (!AnaOutValid[SubIndex])
    return (CO_E_BUSY);

  AnaOutValid[SubIndex] = 0;
   
  *val = p401_WriteAnout16[SubIndex];

  return (CO_OK);
}
