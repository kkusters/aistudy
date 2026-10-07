// CH_COMPUTER.H

#ifndef _CH_COMPUTER_H
#define _CH_COMPUTER_H

#include "ct_disp.h"
#include "ct_string.h"

extern s_tekst_15 tekst_computer_soort_type;
extern s_disp_func const disp_computer_soort_type_control;
extern s_tekst_4 tekst_serie_nummer_extensie;

void Disp_Control_Computer_Soort_Type(void);

extern s_disp_value_add           const disp_add_versie_val;     
extern s_disp_tekst_add_option_on const disp_add_sub_versie_punt;
extern s_disp_value_add_option_on const disp_add_sub_versie_nr;  

#endif