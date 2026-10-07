// CT_PC_COM.H

#ifndef _CT_PC_COM_H
#define _CT_PC_COM_H

typedef enum
{
  TypeChar        = 0,
  TypeInt         = 1,
  TypeLong        = 2,
  TypeCharIndexed = 3,
  TypeIntIndexed  = 4,
  TypeLongIndexed = 5,
  TypeCurve       = 6,
  TypeCurveTemp   = 7,
  TypeCurveKlok   = 8,
  TypeTime        = 9
} e_DataType;

typedef struct
{
  void *ptr;
  e_DataType type;
} s_PcDataTable;

#endif
