// CT_HTRAP.H

#ifndef _CT_HTRAP_H
#define _CT_HTRAP_H

#include "ch_define.h"

typedef struct
{
  int check; // control word to check if data ok
  int reset;
  int pfo;
  int stkof;
  int stkuf;
  int pacer;
  int illopa;
  int prtflt;
  int undopc;
} s_htrap_error_cnt;

#endif
