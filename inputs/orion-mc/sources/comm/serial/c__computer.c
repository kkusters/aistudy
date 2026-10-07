// C__COMPUTER.C

#include <string.h>

#include "ch_define.h"
#include "ch_lcd_pixel.h"
#include "ch_string.h"
#include "ch_computer.h"

s_disp_func const disp_computer_soort_type_control = { Disp_Control_Func, Disp_Control_Computer_Soort_Type };
s_tekst_15 tekst_computer_soort_type = { 15, 120, SIZE_14, "" };
s_tekst_4 tekst_serie_nummer_extensie = { 4, 27, SIZE_14, "-MC" };

void Disp_Control_Computer_Soort_Type(void)
{
  strcpy(tekst_computer_soort_type.string, "ORION-");
  switch (module.type)
  {
    default:
    case MODULE_BASIS:
      strcat(tekst_computer_soort_type.string, "MC"); 
      break;
  }
}

static int const sub_versie = VERSIE_PROG_SUB;
s_disp_value_add           const disp_add_versie_val      = { Disp_Draw_Value_Add,           (SIZE_14 | LINKS), INT, 2, &rom.versie_programma };
s_disp_tekst_add_option_on const disp_add_sub_versie_punt = { Disp_Draw_Tekst_Add_L_Option_On,                       &tekst_punt_14, UCHAR, &sub_versie };
s_disp_value_add_option_on const disp_add_sub_versie_nr   = { Disp_Draw_Value_Add_Option_On, (SIZE_14 | LINKS), INT, 0, &sub_versie, UCHAR, &sub_versie };
