// CH_DISP_OPT_IO_2.H
					
#ifndef _CH_DISP_OPT_IO_2_H
#define _CH_DISP_OPT_IO_2_H

#include "ct_disp.h"

#define DISP_IO_08_09  1
#define DISP_IO_05_07  (DISP_IO_08_09 + IO_08_09_MAX + 1)
#define DISP_IO_06_14  (DISP_IO_05_07 + IO_05_07_MAX + 1)
#define DISP_IO_07_07  (DISP_IO_06_14 + IO_06_14_MAX + 1)
#define DISP_IO_12_06  (DISP_IO_07_07 + IO_07_07_MAX + 1)
#define DISP_IO_H1MC   (DISP_IO_12_06 + IO_12_06_MAX + 1)
#define DISP_IO_H2MC   (DISP_IO_H1MC  + IO_H1MC_MAX  + 1)
#define DISP_IO_EKU	   (DISP_IO_H2MC  + IO_H2MC_MAX  + 1)

extern s_screen screen_opt_IO_2;

void Control_Screen_Option_IO_2(void);

#endif