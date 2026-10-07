// C__DISP_OPT_IO_2.C

#include "ch_define.h"

#include "ch_const.h"
#include "ch_disp.h"
#include "ch_disp_func.h"
#include "ch_disp_option_1.h"
#include "ch_disp_opt_IO_05_07_3.h"
#include "ch_disp_opt_IO_06_14_3.h"
#include "ch_disp_opt_IO_07_07_3.h"
#include "ch_disp_opt_IO_08_09_3.h"
#include "ch_disp_opt_IO_12_06_3.h"
#include "ch_disp_opt_IO_EKU_3.h"
#include "ch_disp_opt_IO_H1MC_3.h"
#include "ch_disp_opt_IO_H2MC_3.h"
#include "ch_disp_opt_IO_select.h"
#include "ch_disp_password.h"
#include "ch_disp_value.h"
#include "ch_key.h"
#include "ch_lcd_10.h"
#include "ch_lcd_bitmap.h"
#include "ch_lcd_pixel.h"
#include "ch_string.h"
#include "ch_disp_opt_IO_2.h"

//-------------------------------------------------------------------------------------------------------------------------
// Header
static s_disp_agri_header const disp_header = { Disp_Draw_Agri_Header, FN6, EMPTY, &tekst_inst.Opties_IO_10, HK_GEEN, 0, 3};
static void * const lcd_disp_header[] = { &disp_header, 0 };

static s_disp_tekst_add const disp_versienummer = { Disp_Draw_Tekst_Add_L, &tekst_inst.Versie_7 };
//-------------------------------------------------------------------------------------------------------------------------
// IO-08-09
static void Arrow_IO_08_09_Func(void);
static void Arrow_IO_08_09_Empty_Func(void);

static unsigned char option_IO_08_09[IO_08_09_MAX+1];

static s_disp_tekst const disp_IO_08_09_1 = { Disp_Draw_Tekst_L, 9, 22, &tekst_IO_08_09_1_14 };
static s_disp_tekst const disp_IO_08_09_2 = { Disp_Draw_Tekst_L, 9, 22, &tekst_IO_08_09_2_14 };
static s_disp_tekst const disp_IO_08_09_3 = { Disp_Draw_Tekst_L, 9, 22, &tekst_IO_08_09_3_14 };
static s_disp_tekst const disp_IO_08_09_4 = { Disp_Draw_Tekst_L, 9, 22, &tekst_IO_08_09_4_14 };
static s_disp_tekst const disp_IO_08_09_5 = { Disp_Draw_Tekst_L, 9, 22, &tekst_IO_08_09_5_14 };
static s_disp_tekst const disp_IO_08_09_6 = { Disp_Draw_Tekst_L, 9, 22, &tekst_IO_08_09_6_14 };
static s_disp_tekst const disp_IO_08_09_7 = { Disp_Draw_Tekst_L, 9, 22, &tekst_IO_08_09_7_14 };
static s_disp_tekst const disp_IO_08_09_8 = { Disp_Draw_Tekst_L, 9, 22, &tekst_IO_08_09_8_14 };
static s_disp_tekst const disp_IO_08_09_e = { Disp_Draw_Tekst_L, 9, 22, &tekst_IO_08_09_e_14 };
static s_disp_value_add_option_on const disp_IO_08_09_1_version_software = { Disp_Draw_Value_Add_Option_On, (SIZE_7 | LINKS), INT, 2, &value.IO_08_09[0].board_component.version_software, INT, &value.IO_08_09[0].board_component.version_software};
static s_disp_value_add_option_on const disp_IO_08_09_2_version_software = { Disp_Draw_Value_Add_Option_On, (SIZE_7 | LINKS), INT, 2, &value.IO_08_09[1].board_component.version_software, INT, &value.IO_08_09[1].board_component.version_software};
static s_disp_value_add_option_on const disp_IO_08_09_3_version_software = { Disp_Draw_Value_Add_Option_On, (SIZE_7 | LINKS), INT, 2, &value.IO_08_09[2].board_component.version_software, INT, &value.IO_08_09[2].board_component.version_software};
static s_disp_value_add_option_on const disp_IO_08_09_4_version_software = { Disp_Draw_Value_Add_Option_On, (SIZE_7 | LINKS), INT, 2, &value.IO_08_09[3].board_component.version_software, INT, &value.IO_08_09[3].board_component.version_software};
static s_disp_value_add_option_on const disp_IO_08_09_5_version_software = { Disp_Draw_Value_Add_Option_On, (SIZE_7 | LINKS), INT, 2, &value.IO_08_09[4].board_component.version_software, INT, &value.IO_08_09[4].board_component.version_software};
static s_disp_value_add_option_on const disp_IO_08_09_6_version_software = { Disp_Draw_Value_Add_Option_On, (SIZE_7 | LINKS), INT, 2, &value.IO_08_09[5].board_component.version_software, INT, &value.IO_08_09[5].board_component.version_software};
static s_disp_value_add_option_on const disp_IO_08_09_7_version_software = { Disp_Draw_Value_Add_Option_On, (SIZE_7 | LINKS), INT, 2, &value.IO_08_09[6].board_component.version_software, INT, &value.IO_08_09[6].board_component.version_software};
static s_disp_value_add_option_on const disp_IO_08_09_8_version_software = { Disp_Draw_Value_Add_Option_On, (SIZE_7 | LINKS), INT, 2, &value.IO_08_09[7].board_component.version_software, INT, &value.IO_08_09[7].board_component.version_software};

static void * const lcd_IO_08_09_1_disp[] = { &disp_IO_08_09_1, &disp_space_14_L, &disp_versienummer, &disp_space_7_L, &disp_IO_08_09_1_version_software, 0 };
static void * const lcd_IO_08_09_2_disp[] = { &disp_IO_08_09_2, &disp_space_14_L, &disp_versienummer, &disp_space_7_L, &disp_IO_08_09_2_version_software, 0 };
static void * const lcd_IO_08_09_3_disp[] = { &disp_IO_08_09_3, &disp_space_14_L, &disp_versienummer, &disp_space_7_L, &disp_IO_08_09_3_version_software, 0 };
static void * const lcd_IO_08_09_4_disp[] = { &disp_IO_08_09_4, &disp_space_14_L, &disp_versienummer, &disp_space_7_L, &disp_IO_08_09_4_version_software, 0 };
static void * const lcd_IO_08_09_5_disp[] = { &disp_IO_08_09_5, &disp_space_14_L, &disp_versienummer, &disp_space_7_L, &disp_IO_08_09_5_version_software, 0 };
static void * const lcd_IO_08_09_6_disp[] = { &disp_IO_08_09_6, &disp_space_14_L, &disp_versienummer, &disp_space_7_L, &disp_IO_08_09_6_version_software, 0 };
static void * const lcd_IO_08_09_7_disp[] = { &disp_IO_08_09_7, &disp_space_14_L, &disp_versienummer, &disp_space_7_L, &disp_IO_08_09_7_version_software, 0 };
static void * const lcd_IO_08_09_8_disp[] = { &disp_IO_08_09_8, &disp_space_14_L, &disp_versienummer, &disp_space_7_L, &disp_IO_08_09_8_version_software, 0 };
static void * const lcd_IO_08_09_e_disp[] = { &disp_IO_08_09_e, 0 };

//-------------------------------------------------------------------------------------------------------------------------
// IO-05-07
static void Arrow_IO_05_07_Func(void);
static void Arrow_IO_05_07_Empty_Func(void);

static unsigned char option_IO_05_07[IO_05_07_MAX+1];

static s_disp_tekst const disp_IO_05_07_1  = { Disp_Draw_Tekst_L, 9, 22, &tekst_IO_05_07_1_14  };
static s_disp_tekst const disp_IO_05_07_2  = { Disp_Draw_Tekst_L, 9, 22, &tekst_IO_05_07_2_14  };
static s_disp_tekst const disp_IO_05_07_3  = { Disp_Draw_Tekst_L, 9, 22, &tekst_IO_05_07_3_14  };
static s_disp_tekst const disp_IO_05_07_4  = { Disp_Draw_Tekst_L, 9, 22, &tekst_IO_05_07_4_14  };
static s_disp_tekst const disp_IO_05_07_5  = { Disp_Draw_Tekst_L, 9, 22, &tekst_IO_05_07_5_14  };
static s_disp_tekst const disp_IO_05_07_6  = { Disp_Draw_Tekst_L, 9, 22, &tekst_IO_05_07_6_14  };
static s_disp_tekst const disp_IO_05_07_7  = { Disp_Draw_Tekst_L, 9, 22, &tekst_IO_05_07_7_14  };
static s_disp_tekst const disp_IO_05_07_8  = { Disp_Draw_Tekst_L, 9, 22, &tekst_IO_05_07_8_14  };
static s_disp_tekst const disp_IO_05_07_9  = { Disp_Draw_Tekst_L, 9, 22, &tekst_IO_05_07_9_14  };
static s_disp_tekst const disp_IO_05_07_10 = { Disp_Draw_Tekst_L, 9, 22, &tekst_IO_05_07_10_14 };
static s_disp_tekst const disp_IO_05_07_11 = { Disp_Draw_Tekst_L, 9, 22, &tekst_IO_05_07_11_14 };
static s_disp_tekst const disp_IO_05_07_12 = { Disp_Draw_Tekst_L, 9, 22, &tekst_IO_05_07_12_14 };
static s_disp_tekst const disp_IO_05_07_13 = { Disp_Draw_Tekst_L, 9, 22, &tekst_IO_05_07_13_14 };
static s_disp_tekst const disp_IO_05_07_14 = { Disp_Draw_Tekst_L, 9, 22, &tekst_IO_05_07_14_14 };
static s_disp_tekst const disp_IO_05_07_15 = { Disp_Draw_Tekst_L, 9, 22, &tekst_IO_05_07_15_14 };
static s_disp_tekst const disp_IO_05_07_16 = { Disp_Draw_Tekst_L, 9, 22, &tekst_IO_05_07_16_14 };
static s_disp_tekst const disp_IO_05_07_e  = { Disp_Draw_Tekst_L, 9, 22, &tekst_IO_05_07_e_14  };
static s_disp_value_add_option_on const disp_IO_05_07_1_version_software  = { Disp_Draw_Value_Add_Option_On, (SIZE_7 | LINKS), INT, 2, &value.IO_05_07[0].board_component.version_software,  INT, &value.IO_05_07[0].board_component.version_software  };
static s_disp_value_add_option_on const disp_IO_05_07_2_version_software  = { Disp_Draw_Value_Add_Option_On, (SIZE_7 | LINKS), INT, 2, &value.IO_05_07[1].board_component.version_software,  INT, &value.IO_05_07[1].board_component.version_software  };
static s_disp_value_add_option_on const disp_IO_05_07_3_version_software  = { Disp_Draw_Value_Add_Option_On, (SIZE_7 | LINKS), INT, 2, &value.IO_05_07[2].board_component.version_software,  INT, &value.IO_05_07[2].board_component.version_software  };
static s_disp_value_add_option_on const disp_IO_05_07_4_version_software  = { Disp_Draw_Value_Add_Option_On, (SIZE_7 | LINKS), INT, 2, &value.IO_05_07[3].board_component.version_software,  INT, &value.IO_05_07[3].board_component.version_software  };
static s_disp_value_add_option_on const disp_IO_05_07_5_version_software  = { Disp_Draw_Value_Add_Option_On, (SIZE_7 | LINKS), INT, 2, &value.IO_05_07[4].board_component.version_software,  INT, &value.IO_05_07[4].board_component.version_software  };
static s_disp_value_add_option_on const disp_IO_05_07_6_version_software  = { Disp_Draw_Value_Add_Option_On, (SIZE_7 | LINKS), INT, 2, &value.IO_05_07[5].board_component.version_software,  INT, &value.IO_05_07[5].board_component.version_software  };
static s_disp_value_add_option_on const disp_IO_05_07_7_version_software  = { Disp_Draw_Value_Add_Option_On, (SIZE_7 | LINKS), INT, 2, &value.IO_05_07[6].board_component.version_software,  INT, &value.IO_05_07[6].board_component.version_software  };
static s_disp_value_add_option_on const disp_IO_05_07_8_version_software  = { Disp_Draw_Value_Add_Option_On, (SIZE_7 | LINKS), INT, 2, &value.IO_05_07[7].board_component.version_software,  INT, &value.IO_05_07[7].board_component.version_software  };
static s_disp_value_add_option_on const disp_IO_05_07_9_version_software  = { Disp_Draw_Value_Add_Option_On, (SIZE_7 | LINKS), INT, 2, &value.IO_05_07[8].board_component.version_software,  INT, &value.IO_05_07[8].board_component.version_software  };
static s_disp_value_add_option_on const disp_IO_05_07_10_version_software = { Disp_Draw_Value_Add_Option_On, (SIZE_7 | LINKS), INT, 2, &value.IO_05_07[9].board_component.version_software,  INT, &value.IO_05_07[9].board_component.version_software  };
static s_disp_value_add_option_on const disp_IO_05_07_11_version_software = { Disp_Draw_Value_Add_Option_On, (SIZE_7 | LINKS), INT, 2, &value.IO_05_07[10].board_component.version_software, INT, &value.IO_05_07[10].board_component.version_software };
static s_disp_value_add_option_on const disp_IO_05_07_12_version_software = { Disp_Draw_Value_Add_Option_On, (SIZE_7 | LINKS), INT, 2, &value.IO_05_07[11].board_component.version_software, INT, &value.IO_05_07[11].board_component.version_software };
static s_disp_value_add_option_on const disp_IO_05_07_13_version_software = { Disp_Draw_Value_Add_Option_On, (SIZE_7 | LINKS), INT, 2, &value.IO_05_07[12].board_component.version_software, INT, &value.IO_05_07[12].board_component.version_software };
static s_disp_value_add_option_on const disp_IO_05_07_14_version_software = { Disp_Draw_Value_Add_Option_On, (SIZE_7 | LINKS), INT, 2, &value.IO_05_07[13].board_component.version_software, INT, &value.IO_05_07[13].board_component.version_software };
static s_disp_value_add_option_on const disp_IO_05_07_15_version_software = { Disp_Draw_Value_Add_Option_On, (SIZE_7 | LINKS), INT, 2, &value.IO_05_07[14].board_component.version_software, INT, &value.IO_05_07[14].board_component.version_software };
static s_disp_value_add_option_on const disp_IO_05_07_16_version_software = { Disp_Draw_Value_Add_Option_On, (SIZE_7 | LINKS), INT, 2, &value.IO_05_07[15].board_component.version_software, INT, &value.IO_05_07[15].board_component.version_software };

static void * const lcd_IO_05_07_1_disp[]  = { &disp_IO_05_07_1,  &disp_space_14_L, &disp_versienummer, &disp_space_7_L, &disp_IO_05_07_1_version_software,  0 };
static void * const lcd_IO_05_07_2_disp[]  = { &disp_IO_05_07_2,  &disp_space_14_L, &disp_versienummer, &disp_space_7_L, &disp_IO_05_07_2_version_software,  0 };
static void * const lcd_IO_05_07_3_disp[]  = { &disp_IO_05_07_3,  &disp_space_14_L, &disp_versienummer, &disp_space_7_L, &disp_IO_05_07_3_version_software,  0 };
static void * const lcd_IO_05_07_4_disp[]  = { &disp_IO_05_07_4,  &disp_space_14_L, &disp_versienummer, &disp_space_7_L, &disp_IO_05_07_4_version_software,  0 };
static void * const lcd_IO_05_07_5_disp[]  = { &disp_IO_05_07_5,  &disp_space_14_L, &disp_versienummer, &disp_space_7_L, &disp_IO_05_07_5_version_software,  0 };
static void * const lcd_IO_05_07_6_disp[]  = { &disp_IO_05_07_6,  &disp_space_14_L, &disp_versienummer, &disp_space_7_L, &disp_IO_05_07_6_version_software,  0 };
static void * const lcd_IO_05_07_7_disp[]  = { &disp_IO_05_07_7,  &disp_space_14_L, &disp_versienummer, &disp_space_7_L, &disp_IO_05_07_7_version_software,  0 };
static void * const lcd_IO_05_07_8_disp[]  = { &disp_IO_05_07_8,  &disp_space_14_L, &disp_versienummer, &disp_space_7_L, &disp_IO_05_07_8_version_software,  0 };
static void * const lcd_IO_05_07_9_disp[]  = { &disp_IO_05_07_9,  &disp_space_14_L, &disp_versienummer, &disp_space_7_L, &disp_IO_05_07_9_version_software,  0 };
static void * const lcd_IO_05_07_10_disp[] = { &disp_IO_05_07_10, &disp_space_14_L, &disp_versienummer, &disp_space_7_L, &disp_IO_05_07_10_version_software, 0 };
static void * const lcd_IO_05_07_11_disp[] = { &disp_IO_05_07_11, &disp_space_14_L, &disp_versienummer, &disp_space_7_L, &disp_IO_05_07_11_version_software, 0 };
static void * const lcd_IO_05_07_12_disp[] = { &disp_IO_05_07_12, &disp_space_14_L, &disp_versienummer, &disp_space_7_L, &disp_IO_05_07_12_version_software, 0 };
static void * const lcd_IO_05_07_13_disp[] = { &disp_IO_05_07_13, &disp_space_14_L, &disp_versienummer, &disp_space_7_L, &disp_IO_05_07_13_version_software, 0 };
static void * const lcd_IO_05_07_14_disp[] = { &disp_IO_05_07_14, &disp_space_14_L, &disp_versienummer, &disp_space_7_L, &disp_IO_05_07_14_version_software, 0 };
static void * const lcd_IO_05_07_15_disp[] = { &disp_IO_05_07_15, &disp_space_14_L, &disp_versienummer, &disp_space_7_L, &disp_IO_05_07_15_version_software, 0 };
static void * const lcd_IO_05_07_16_disp[] = { &disp_IO_05_07_16, &disp_space_14_L, &disp_versienummer, &disp_space_7_L, &disp_IO_05_07_16_version_software, 0 };
static void * const lcd_IO_05_07_e_disp[]  = { &disp_IO_05_07_e, 0 };

//-------------------------------------------------------------------------------------------------------------------------
// IO-06-14
static void Arrow_IO_06_14_Func(void);
static void Arrow_IO_06_14_Empty_Func(void);

static unsigned char option_IO_06_14[IO_06_14_MAX+1];

static s_disp_tekst const disp_IO_06_14_1 = { Disp_Draw_Tekst_L, 9, 22, &tekst_IO_06_14_1_14 };
static s_disp_tekst const disp_IO_06_14_2 = { Disp_Draw_Tekst_L, 9, 22, &tekst_IO_06_14_2_14 };
static s_disp_tekst const disp_IO_06_14_3 = { Disp_Draw_Tekst_L, 9, 22, &tekst_IO_06_14_3_14 };
static s_disp_tekst const disp_IO_06_14_4 = { Disp_Draw_Tekst_L, 9, 22, &tekst_IO_06_14_4_14 };
static s_disp_tekst const disp_IO_06_14_e = { Disp_Draw_Tekst_L, 9, 22, &tekst_IO_06_14_e_14 };
static s_disp_value_add_option_on const disp_IO_06_14_1_version_software = { Disp_Draw_Value_Add_Option_On, (SIZE_7 | LINKS), INT, 2, &value.IO_06_14[0].board_component.version_software, INT, &value.IO_06_14[0].board_component.version_software};
static s_disp_value_add_option_on const disp_IO_06_14_2_version_software = { Disp_Draw_Value_Add_Option_On, (SIZE_7 | LINKS), INT, 2, &value.IO_06_14[1].board_component.version_software, INT, &value.IO_06_14[1].board_component.version_software};
static s_disp_value_add_option_on const disp_IO_06_14_3_version_software = { Disp_Draw_Value_Add_Option_On, (SIZE_7 | LINKS), INT, 2, &value.IO_06_14[2].board_component.version_software, INT, &value.IO_06_14[2].board_component.version_software};
static s_disp_value_add_option_on const disp_IO_06_14_4_version_software = { Disp_Draw_Value_Add_Option_On, (SIZE_7 | LINKS), INT, 2, &value.IO_06_14[3].board_component.version_software, INT, &value.IO_06_14[3].board_component.version_software};

static void * const lcd_IO_06_14_1_disp[] = { &disp_IO_06_14_1, &disp_space_14_L, &disp_versienummer, &disp_space_7_L, &disp_IO_06_14_1_version_software, 0 };
static void * const lcd_IO_06_14_2_disp[] = { &disp_IO_06_14_2, &disp_space_14_L, &disp_versienummer, &disp_space_7_L, &disp_IO_06_14_2_version_software, 0 };
static void * const lcd_IO_06_14_3_disp[] = { &disp_IO_06_14_3, &disp_space_14_L, &disp_versienummer, &disp_space_7_L, &disp_IO_06_14_3_version_software, 0 };
static void * const lcd_IO_06_14_4_disp[] = { &disp_IO_06_14_4, &disp_space_14_L, &disp_versienummer, &disp_space_7_L, &disp_IO_06_14_4_version_software, 0 };
static void * const lcd_IO_06_14_e_disp[] = { &disp_IO_06_14_e, 0 };
//-------------------------------------------------------------------------------------------------------------------------
// IO-07-07
static void Arrow_IO_07_07_Func(void);
static void Arrow_IO_07_07_Empty_Func(void);

static unsigned char option_IO_07_07[IO_07_07_MAX+1];

static s_disp_tekst const disp_IO_07_07_1  = { Disp_Draw_Tekst_L, 9, 22, &tekst_IO_07_07_1_14  };
static s_disp_tekst const disp_IO_07_07_2  = { Disp_Draw_Tekst_L, 9, 22, &tekst_IO_07_07_2_14  };
static s_disp_tekst const disp_IO_07_07_3  = { Disp_Draw_Tekst_L, 9, 22, &tekst_IO_07_07_3_14  };
static s_disp_tekst const disp_IO_07_07_4  = { Disp_Draw_Tekst_L, 9, 22, &tekst_IO_07_07_4_14  };
static s_disp_tekst const disp_IO_07_07_5  = { Disp_Draw_Tekst_L, 9, 22, &tekst_IO_07_07_5_14  };
static s_disp_tekst const disp_IO_07_07_6  = { Disp_Draw_Tekst_L, 9, 22, &tekst_IO_07_07_6_14  };
static s_disp_tekst const disp_IO_07_07_7  = { Disp_Draw_Tekst_L, 9, 22, &tekst_IO_07_07_7_14  };
static s_disp_tekst const disp_IO_07_07_8  = { Disp_Draw_Tekst_L, 9, 22, &tekst_IO_07_07_8_14  };
static s_disp_tekst const disp_IO_07_07_9  = { Disp_Draw_Tekst_L, 9, 22, &tekst_IO_07_07_9_14  };
static s_disp_tekst const disp_IO_07_07_10 = { Disp_Draw_Tekst_L, 9, 22, &tekst_IO_07_07_10_14 };
static s_disp_tekst const disp_IO_07_07_11 = { Disp_Draw_Tekst_L, 9, 22, &tekst_IO_07_07_11_14 };
static s_disp_tekst const disp_IO_07_07_12 = { Disp_Draw_Tekst_L, 9, 22, &tekst_IO_07_07_12_14 };
static s_disp_tekst const disp_IO_07_07_13 = { Disp_Draw_Tekst_L, 9, 22, &tekst_IO_07_07_13_14 };
static s_disp_tekst const disp_IO_07_07_14 = { Disp_Draw_Tekst_L, 9, 22, &tekst_IO_07_07_14_14 };
static s_disp_tekst const disp_IO_07_07_15 = { Disp_Draw_Tekst_L, 9, 22, &tekst_IO_07_07_15_14 };
static s_disp_tekst const disp_IO_07_07_16 = { Disp_Draw_Tekst_L, 9, 22, &tekst_IO_07_07_16_14 };
static s_disp_tekst const disp_IO_07_07_17 = { Disp_Draw_Tekst_L, 9, 22, &tekst_IO_07_07_17_14 };
static s_disp_tekst const disp_IO_07_07_18 = { Disp_Draw_Tekst_L, 9, 22, &tekst_IO_07_07_18_14 };
static s_disp_tekst const disp_IO_07_07_19 = { Disp_Draw_Tekst_L, 9, 22, &tekst_IO_07_07_19_14 };
static s_disp_tekst const disp_IO_07_07_20 = { Disp_Draw_Tekst_L, 9, 22, &tekst_IO_07_07_20_14 };
static s_disp_tekst const disp_IO_07_07_21 = { Disp_Draw_Tekst_L, 9, 22, &tekst_IO_07_07_21_14 };
static s_disp_tekst const disp_IO_07_07_22 = { Disp_Draw_Tekst_L, 9, 22, &tekst_IO_07_07_22_14 };
static s_disp_tekst const disp_IO_07_07_23 = { Disp_Draw_Tekst_L, 9, 22, &tekst_IO_07_07_23_14 };
static s_disp_tekst const disp_IO_07_07_24 = { Disp_Draw_Tekst_L, 9, 22, &tekst_IO_07_07_24_14 };
static s_disp_tekst const disp_IO_07_07_25 = { Disp_Draw_Tekst_L, 9, 22, &tekst_IO_07_07_25_14 };
static s_disp_tekst const disp_IO_07_07_26 = { Disp_Draw_Tekst_L, 9, 22, &tekst_IO_07_07_26_14 };
static s_disp_tekst const disp_IO_07_07_27 = { Disp_Draw_Tekst_L, 9, 22, &tekst_IO_07_07_27_14 };
static s_disp_tekst const disp_IO_07_07_28 = { Disp_Draw_Tekst_L, 9, 22, &tekst_IO_07_07_28_14 };
static s_disp_tekst const disp_IO_07_07_29 = { Disp_Draw_Tekst_L, 9, 22, &tekst_IO_07_07_29_14 };
static s_disp_tekst const disp_IO_07_07_30 = { Disp_Draw_Tekst_L, 9, 22, &tekst_IO_07_07_30_14 };
static s_disp_tekst const disp_IO_07_07_31 = { Disp_Draw_Tekst_L, 9, 22, &tekst_IO_07_07_31_14 };
static s_disp_tekst const disp_IO_07_07_32 = { Disp_Draw_Tekst_L, 9, 22, &tekst_IO_07_07_32_14 };
static s_disp_tekst const disp_IO_07_07_e  = { Disp_Draw_Tekst_L, 9, 22, &tekst_IO_07_07_e_14  };
static s_disp_value_add_option_on const disp_IO_07_07_1_version_software  = { Disp_Draw_Value_Add_Option_On, (SIZE_7 | LINKS), INT, 2, &value.IO_07_07[0].board_component.version_software,  INT, &value.IO_07_07[0].board_component.version_software  };
static s_disp_value_add_option_on const disp_IO_07_07_2_version_software  = { Disp_Draw_Value_Add_Option_On, (SIZE_7 | LINKS), INT, 2, &value.IO_07_07[1].board_component.version_software,  INT, &value.IO_07_07[1].board_component.version_software  };
static s_disp_value_add_option_on const disp_IO_07_07_3_version_software  = { Disp_Draw_Value_Add_Option_On, (SIZE_7 | LINKS), INT, 2, &value.IO_07_07[2].board_component.version_software,  INT, &value.IO_07_07[2].board_component.version_software  };
static s_disp_value_add_option_on const disp_IO_07_07_4_version_software  = { Disp_Draw_Value_Add_Option_On, (SIZE_7 | LINKS), INT, 2, &value.IO_07_07[3].board_component.version_software,  INT, &value.IO_07_07[3].board_component.version_software  };
static s_disp_value_add_option_on const disp_IO_07_07_5_version_software  = { Disp_Draw_Value_Add_Option_On, (SIZE_7 | LINKS), INT, 2, &value.IO_07_07[4].board_component.version_software,  INT, &value.IO_07_07[4].board_component.version_software  };
static s_disp_value_add_option_on const disp_IO_07_07_6_version_software  = { Disp_Draw_Value_Add_Option_On, (SIZE_7 | LINKS), INT, 2, &value.IO_07_07[5].board_component.version_software,  INT, &value.IO_07_07[5].board_component.version_software  };
static s_disp_value_add_option_on const disp_IO_07_07_7_version_software  = { Disp_Draw_Value_Add_Option_On, (SIZE_7 | LINKS), INT, 2, &value.IO_07_07[6].board_component.version_software,  INT, &value.IO_07_07[6].board_component.version_software  };
static s_disp_value_add_option_on const disp_IO_07_07_8_version_software  = { Disp_Draw_Value_Add_Option_On, (SIZE_7 | LINKS), INT, 2, &value.IO_07_07[7].board_component.version_software,  INT, &value.IO_07_07[7].board_component.version_software  };
static s_disp_value_add_option_on const disp_IO_07_07_9_version_software  = { Disp_Draw_Value_Add_Option_On, (SIZE_7 | LINKS), INT, 2, &value.IO_07_07[8].board_component.version_software,  INT, &value.IO_07_07[8].board_component.version_software  };
static s_disp_value_add_option_on const disp_IO_07_07_10_version_software = { Disp_Draw_Value_Add_Option_On, (SIZE_7 | LINKS), INT, 2, &value.IO_07_07[9].board_component.version_software,  INT, &value.IO_07_07[9].board_component.version_software  };
static s_disp_value_add_option_on const disp_IO_07_07_11_version_software = { Disp_Draw_Value_Add_Option_On, (SIZE_7 | LINKS), INT, 2, &value.IO_07_07[10].board_component.version_software, INT, &value.IO_07_07[10].board_component.version_software };
static s_disp_value_add_option_on const disp_IO_07_07_12_version_software = { Disp_Draw_Value_Add_Option_On, (SIZE_7 | LINKS), INT, 2, &value.IO_07_07[11].board_component.version_software, INT, &value.IO_07_07[11].board_component.version_software };
static s_disp_value_add_option_on const disp_IO_07_07_13_version_software = { Disp_Draw_Value_Add_Option_On, (SIZE_7 | LINKS), INT, 2, &value.IO_07_07[12].board_component.version_software, INT, &value.IO_07_07[12].board_component.version_software };
static s_disp_value_add_option_on const disp_IO_07_07_14_version_software = { Disp_Draw_Value_Add_Option_On, (SIZE_7 | LINKS), INT, 2, &value.IO_07_07[13].board_component.version_software, INT, &value.IO_07_07[13].board_component.version_software };
static s_disp_value_add_option_on const disp_IO_07_07_15_version_software = { Disp_Draw_Value_Add_Option_On, (SIZE_7 | LINKS), INT, 2, &value.IO_07_07[14].board_component.version_software, INT, &value.IO_07_07[14].board_component.version_software };
static s_disp_value_add_option_on const disp_IO_07_07_16_version_software = { Disp_Draw_Value_Add_Option_On, (SIZE_7 | LINKS), INT, 2, &value.IO_07_07[15].board_component.version_software, INT, &value.IO_07_07[15].board_component.version_software };
static s_disp_value_add_option_on const disp_IO_07_07_17_version_software = { Disp_Draw_Value_Add_Option_On, (SIZE_7 | LINKS), INT, 2, &value.IO_07_07[16].board_component.version_software, INT, &value.IO_07_07[16].board_component.version_software };
static s_disp_value_add_option_on const disp_IO_07_07_18_version_software = { Disp_Draw_Value_Add_Option_On, (SIZE_7 | LINKS), INT, 2, &value.IO_07_07[17].board_component.version_software, INT, &value.IO_07_07[17].board_component.version_software };
static s_disp_value_add_option_on const disp_IO_07_07_19_version_software = { Disp_Draw_Value_Add_Option_On, (SIZE_7 | LINKS), INT, 2, &value.IO_07_07[18].board_component.version_software, INT, &value.IO_07_07[18].board_component.version_software };
static s_disp_value_add_option_on const disp_IO_07_07_20_version_software = { Disp_Draw_Value_Add_Option_On, (SIZE_7 | LINKS), INT, 2, &value.IO_07_07[19].board_component.version_software, INT, &value.IO_07_07[19].board_component.version_software };
static s_disp_value_add_option_on const disp_IO_07_07_21_version_software = { Disp_Draw_Value_Add_Option_On, (SIZE_7 | LINKS), INT, 2, &value.IO_07_07[20].board_component.version_software, INT, &value.IO_07_07[20].board_component.version_software };
static s_disp_value_add_option_on const disp_IO_07_07_22_version_software = { Disp_Draw_Value_Add_Option_On, (SIZE_7 | LINKS), INT, 2, &value.IO_07_07[21].board_component.version_software, INT, &value.IO_07_07[21].board_component.version_software };
static s_disp_value_add_option_on const disp_IO_07_07_23_version_software = { Disp_Draw_Value_Add_Option_On, (SIZE_7 | LINKS), INT, 2, &value.IO_07_07[22].board_component.version_software, INT, &value.IO_07_07[22].board_component.version_software };
static s_disp_value_add_option_on const disp_IO_07_07_24_version_software = { Disp_Draw_Value_Add_Option_On, (SIZE_7 | LINKS), INT, 2, &value.IO_07_07[23].board_component.version_software, INT, &value.IO_07_07[23].board_component.version_software };
static s_disp_value_add_option_on const disp_IO_07_07_25_version_software = { Disp_Draw_Value_Add_Option_On, (SIZE_7 | LINKS), INT, 2, &value.IO_07_07[24].board_component.version_software, INT, &value.IO_07_07[24].board_component.version_software };
static s_disp_value_add_option_on const disp_IO_07_07_26_version_software = { Disp_Draw_Value_Add_Option_On, (SIZE_7 | LINKS), INT, 2, &value.IO_07_07[25].board_component.version_software, INT, &value.IO_07_07[25].board_component.version_software };
static s_disp_value_add_option_on const disp_IO_07_07_27_version_software = { Disp_Draw_Value_Add_Option_On, (SIZE_7 | LINKS), INT, 2, &value.IO_07_07[26].board_component.version_software, INT, &value.IO_07_07[26].board_component.version_software };
static s_disp_value_add_option_on const disp_IO_07_07_28_version_software = { Disp_Draw_Value_Add_Option_On, (SIZE_7 | LINKS), INT, 2, &value.IO_07_07[27].board_component.version_software, INT, &value.IO_07_07[27].board_component.version_software };
static s_disp_value_add_option_on const disp_IO_07_07_29_version_software = { Disp_Draw_Value_Add_Option_On, (SIZE_7 | LINKS), INT, 2, &value.IO_07_07[28].board_component.version_software, INT, &value.IO_07_07[28].board_component.version_software };
static s_disp_value_add_option_on const disp_IO_07_07_30_version_software = { Disp_Draw_Value_Add_Option_On, (SIZE_7 | LINKS), INT, 2, &value.IO_07_07[29].board_component.version_software, INT, &value.IO_07_07[29].board_component.version_software };
static s_disp_value_add_option_on const disp_IO_07_07_31_version_software = { Disp_Draw_Value_Add_Option_On, (SIZE_7 | LINKS), INT, 2, &value.IO_07_07[30].board_component.version_software, INT, &value.IO_07_07[30].board_component.version_software };
static s_disp_value_add_option_on const disp_IO_07_07_32_version_software = { Disp_Draw_Value_Add_Option_On, (SIZE_7 | LINKS), INT, 2, &value.IO_07_07[31].board_component.version_software, INT, &value.IO_07_07[31].board_component.version_software };

static void * const lcd_IO_07_07_1_disp[]  = { &disp_IO_07_07_1,  &disp_space_14_L, &disp_versienummer, &disp_space_7_L, &disp_IO_07_07_1_version_software,  0 };
static void * const lcd_IO_07_07_2_disp[]  = { &disp_IO_07_07_2,  &disp_space_14_L, &disp_versienummer, &disp_space_7_L, &disp_IO_07_07_2_version_software,  0 };
static void * const lcd_IO_07_07_3_disp[]  = { &disp_IO_07_07_3,  &disp_space_14_L, &disp_versienummer, &disp_space_7_L, &disp_IO_07_07_3_version_software,  0 };
static void * const lcd_IO_07_07_4_disp[]  = { &disp_IO_07_07_4,  &disp_space_14_L, &disp_versienummer, &disp_space_7_L, &disp_IO_07_07_4_version_software,  0 };
static void * const lcd_IO_07_07_5_disp[]  = { &disp_IO_07_07_5,  &disp_space_14_L, &disp_versienummer, &disp_space_7_L, &disp_IO_07_07_5_version_software,  0 };
static void * const lcd_IO_07_07_6_disp[]  = { &disp_IO_07_07_6,  &disp_space_14_L, &disp_versienummer, &disp_space_7_L, &disp_IO_07_07_6_version_software,  0 };
static void * const lcd_IO_07_07_7_disp[]  = { &disp_IO_07_07_7,  &disp_space_14_L, &disp_versienummer, &disp_space_7_L, &disp_IO_07_07_7_version_software,  0 };
static void * const lcd_IO_07_07_8_disp[]  = { &disp_IO_07_07_8,  &disp_space_14_L, &disp_versienummer, &disp_space_7_L, &disp_IO_07_07_8_version_software,  0 };
static void * const lcd_IO_07_07_9_disp[]  = { &disp_IO_07_07_9,  &disp_space_14_L, &disp_versienummer, &disp_space_7_L, &disp_IO_07_07_9_version_software,  0 };
static void * const lcd_IO_07_07_10_disp[] = { &disp_IO_07_07_10, &disp_space_14_L, &disp_versienummer, &disp_space_7_L, &disp_IO_07_07_10_version_software, 0 };
static void * const lcd_IO_07_07_11_disp[] = { &disp_IO_07_07_11, &disp_space_14_L, &disp_versienummer, &disp_space_7_L, &disp_IO_07_07_11_version_software, 0 };
static void * const lcd_IO_07_07_12_disp[] = { &disp_IO_07_07_12, &disp_space_14_L, &disp_versienummer, &disp_space_7_L, &disp_IO_07_07_12_version_software, 0 };
static void * const lcd_IO_07_07_13_disp[] = { &disp_IO_07_07_13, &disp_space_14_L, &disp_versienummer, &disp_space_7_L, &disp_IO_07_07_13_version_software, 0 };
static void * const lcd_IO_07_07_14_disp[] = { &disp_IO_07_07_14, &disp_space_14_L, &disp_versienummer, &disp_space_7_L, &disp_IO_07_07_14_version_software, 0 };
static void * const lcd_IO_07_07_15_disp[] = { &disp_IO_07_07_15, &disp_space_14_L, &disp_versienummer, &disp_space_7_L, &disp_IO_07_07_15_version_software, 0 };
static void * const lcd_IO_07_07_16_disp[] = { &disp_IO_07_07_16, &disp_space_14_L, &disp_versienummer, &disp_space_7_L, &disp_IO_07_07_16_version_software, 0 };
static void * const lcd_IO_07_07_17_disp[] = { &disp_IO_07_07_17, &disp_space_14_L, &disp_versienummer, &disp_space_7_L, &disp_IO_07_07_17_version_software, 0 };
static void * const lcd_IO_07_07_18_disp[] = { &disp_IO_07_07_18, &disp_space_14_L, &disp_versienummer, &disp_space_7_L, &disp_IO_07_07_18_version_software, 0 };
static void * const lcd_IO_07_07_19_disp[] = { &disp_IO_07_07_19, &disp_space_14_L, &disp_versienummer, &disp_space_7_L, &disp_IO_07_07_19_version_software, 0 };
static void * const lcd_IO_07_07_20_disp[] = { &disp_IO_07_07_20, &disp_space_14_L, &disp_versienummer, &disp_space_7_L, &disp_IO_07_07_20_version_software, 0 };
static void * const lcd_IO_07_07_21_disp[] = { &disp_IO_07_07_21, &disp_space_14_L, &disp_versienummer, &disp_space_7_L, &disp_IO_07_07_21_version_software, 0 };
static void * const lcd_IO_07_07_22_disp[] = { &disp_IO_07_07_22, &disp_space_14_L, &disp_versienummer, &disp_space_7_L, &disp_IO_07_07_22_version_software, 0 };
static void * const lcd_IO_07_07_23_disp[] = { &disp_IO_07_07_23, &disp_space_14_L, &disp_versienummer, &disp_space_7_L, &disp_IO_07_07_23_version_software, 0 };
static void * const lcd_IO_07_07_24_disp[] = { &disp_IO_07_07_24, &disp_space_14_L, &disp_versienummer, &disp_space_7_L, &disp_IO_07_07_24_version_software, 0 };
static void * const lcd_IO_07_07_25_disp[] = { &disp_IO_07_07_25, &disp_space_14_L, &disp_versienummer, &disp_space_7_L, &disp_IO_07_07_25_version_software, 0 };
static void * const lcd_IO_07_07_26_disp[] = { &disp_IO_07_07_26, &disp_space_14_L, &disp_versienummer, &disp_space_7_L, &disp_IO_07_07_26_version_software, 0 };
static void * const lcd_IO_07_07_27_disp[] = { &disp_IO_07_07_27, &disp_space_14_L, &disp_versienummer, &disp_space_7_L, &disp_IO_07_07_27_version_software, 0 };
static void * const lcd_IO_07_07_28_disp[] = { &disp_IO_07_07_28, &disp_space_14_L, &disp_versienummer, &disp_space_7_L, &disp_IO_07_07_28_version_software, 0 };
static void * const lcd_IO_07_07_29_disp[] = { &disp_IO_07_07_29, &disp_space_14_L, &disp_versienummer, &disp_space_7_L, &disp_IO_07_07_29_version_software, 0 };
static void * const lcd_IO_07_07_30_disp[] = { &disp_IO_07_07_30, &disp_space_14_L, &disp_versienummer, &disp_space_7_L, &disp_IO_07_07_30_version_software, 0 };
static void * const lcd_IO_07_07_31_disp[] = { &disp_IO_07_07_31, &disp_space_14_L, &disp_versienummer, &disp_space_7_L, &disp_IO_07_07_31_version_software, 0 };
static void * const lcd_IO_07_07_32_disp[] = { &disp_IO_07_07_32, &disp_space_14_L, &disp_versienummer, &disp_space_7_L, &disp_IO_07_07_32_version_software, 0 };
static void * const lcd_IO_07_07_e_disp[]  = { &disp_IO_07_07_e, 0 };

//-------------------------------------------------------------------------------------------------------------------------
// IO-12-06
static void Arrow_IO_12_06_Func(void);
static void Arrow_IO_12_06_Empty_Func(void);

static unsigned char option_IO_12_06[IO_12_06_MAX+1];

static s_disp_tekst const disp_IO_12_06_1 = { Disp_Draw_Tekst_L, 9, 22, &tekst_IO_12_06_1_14 };
static s_disp_tekst const disp_IO_12_06_2 = { Disp_Draw_Tekst_L, 9, 22, &tekst_IO_12_06_2_14 };
static s_disp_tekst const disp_IO_12_06_3 = { Disp_Draw_Tekst_L, 9, 22, &tekst_IO_12_06_3_14 };
static s_disp_tekst const disp_IO_12_06_4 = { Disp_Draw_Tekst_L, 9, 22, &tekst_IO_12_06_4_14 };
static s_disp_tekst const disp_IO_12_06_e = { Disp_Draw_Tekst_L, 9, 22, &tekst_IO_12_06_e_14 };
static s_disp_value_add_option_on const disp_IO_12_06_1_version_software = { Disp_Draw_Value_Add_Option_On, (SIZE_7 | LINKS), INT, 2, &value.IO_12_06[0].board_component.version_software, INT, &value.IO_12_06[0].board_component.version_software};
static s_disp_value_add_option_on const disp_IO_12_06_2_version_software = { Disp_Draw_Value_Add_Option_On, (SIZE_7 | LINKS), INT, 2, &value.IO_12_06[1].board_component.version_software, INT, &value.IO_12_06[1].board_component.version_software};
static s_disp_value_add_option_on const disp_IO_12_06_3_version_software = { Disp_Draw_Value_Add_Option_On, (SIZE_7 | LINKS), INT, 2, &value.IO_12_06[2].board_component.version_software, INT, &value.IO_12_06[2].board_component.version_software};
static s_disp_value_add_option_on const disp_IO_12_06_4_version_software = { Disp_Draw_Value_Add_Option_On, (SIZE_7 | LINKS), INT, 2, &value.IO_12_06[3].board_component.version_software, INT, &value.IO_12_06[3].board_component.version_software};

static void * const lcd_IO_12_06_1_disp[] = { &disp_IO_12_06_1, &disp_space_14_L, &disp_versienummer, &disp_space_7_L, &disp_IO_12_06_1_version_software, 0 };
static void * const lcd_IO_12_06_2_disp[] = { &disp_IO_12_06_2, &disp_space_14_L, &disp_versienummer, &disp_space_7_L, &disp_IO_12_06_2_version_software, 0 };
static void * const lcd_IO_12_06_3_disp[] = { &disp_IO_12_06_3, &disp_space_14_L, &disp_versienummer, &disp_space_7_L, &disp_IO_12_06_3_version_software, 0 };
static void * const lcd_IO_12_06_4_disp[] = { &disp_IO_12_06_4, &disp_space_14_L, &disp_versienummer, &disp_space_7_L, &disp_IO_12_06_4_version_software, 0 };
static void * const lcd_IO_12_06_e_disp[] = { &disp_IO_12_06_e, 0 };
//-------------------------------------------------------------------------------------------------------------------------
// IO-H1MC
static void Arrow_IO_H1MC_Func(void);
static void Arrow_IO_H1MC_Empty_Func(void);

static unsigned char option_IO_H1MC[IO_H1MC_MAX+1];

static s_disp_tekst const disp_IO_H1MC_1  = { Disp_Draw_Tekst_L, 9, 22, &tekst_IO_H1MC_1_14  };
static s_disp_tekst const disp_IO_H1MC_2  = { Disp_Draw_Tekst_L, 9, 22, &tekst_IO_H1MC_2_14  };
static s_disp_tekst const disp_IO_H1MC_3  = { Disp_Draw_Tekst_L, 9, 22, &tekst_IO_H1MC_3_14  };                                            
static s_disp_tekst const disp_IO_H1MC_4  = { Disp_Draw_Tekst_L, 9, 22, &tekst_IO_H1MC_4_14  };
static s_disp_tekst const disp_IO_H1MC_5  = { Disp_Draw_Tekst_L, 9, 22, &tekst_IO_H1MC_5_14  };
static s_disp_tekst const disp_IO_H1MC_6  = { Disp_Draw_Tekst_L, 9, 22, &tekst_IO_H1MC_6_14  };
static s_disp_tekst const disp_IO_H1MC_7  = { Disp_Draw_Tekst_L, 9, 22, &tekst_IO_H1MC_7_14  };                                            
static s_disp_tekst const disp_IO_H1MC_8  = { Disp_Draw_Tekst_L, 9, 22, &tekst_IO_H1MC_8_14  };
static s_disp_tekst const disp_IO_H1MC_9  = { Disp_Draw_Tekst_L, 9, 22, &tekst_IO_H1MC_9_14  };
static s_disp_tekst const disp_IO_H1MC_10 = { Disp_Draw_Tekst_L, 9, 22, &tekst_IO_H1MC_10_14 };
static s_disp_tekst const disp_IO_H1MC_11 = { Disp_Draw_Tekst_L, 9, 22, &tekst_IO_H1MC_11_14 };                                            
static s_disp_tekst const disp_IO_H1MC_12 = { Disp_Draw_Tekst_L, 9, 22, &tekst_IO_H1MC_12_14 };
static s_disp_tekst const disp_IO_H1MC_13 = { Disp_Draw_Tekst_L, 9, 22, &tekst_IO_H1MC_13_14 };
static s_disp_tekst const disp_IO_H1MC_14 = { Disp_Draw_Tekst_L, 9, 22, &tekst_IO_H1MC_14_14 };
static s_disp_tekst const disp_IO_H1MC_15 = { Disp_Draw_Tekst_L, 9, 22, &tekst_IO_H1MC_15_14 };                                            
static s_disp_tekst const disp_IO_H1MC_16 = { Disp_Draw_Tekst_L, 9, 22, &tekst_IO_H1MC_16_14 };
static s_disp_tekst const disp_IO_H1MC_17 = { Disp_Draw_Tekst_L, 9, 22, &tekst_IO_H1MC_17_14 };
static s_disp_tekst const disp_IO_H1MC_18 = { Disp_Draw_Tekst_L, 9, 22, &tekst_IO_H1MC_18_14 };
static s_disp_tekst const disp_IO_H1MC_19 = { Disp_Draw_Tekst_L, 9, 22, &tekst_IO_H1MC_19_14 };                                            
static s_disp_tekst const disp_IO_H1MC_20 = { Disp_Draw_Tekst_L, 9, 22, &tekst_IO_H1MC_20_14 };
static s_disp_tekst const disp_IO_H1MC_21 = { Disp_Draw_Tekst_L, 9, 22, &tekst_IO_H1MC_21_14 };
static s_disp_tekst const disp_IO_H1MC_22 = { Disp_Draw_Tekst_L, 9, 22, &tekst_IO_H1MC_22_14 };
static s_disp_tekst const disp_IO_H1MC_23 = { Disp_Draw_Tekst_L, 9, 22, &tekst_IO_H1MC_23_14 };                                            
static s_disp_tekst const disp_IO_H1MC_24 = { Disp_Draw_Tekst_L, 9, 22, &tekst_IO_H1MC_24_14 };
static s_disp_tekst const disp_IO_H1MC_25 = { Disp_Draw_Tekst_L, 9, 22, &tekst_IO_H1MC_25_14 };
static s_disp_tekst const disp_IO_H1MC_26 = { Disp_Draw_Tekst_L, 9, 22, &tekst_IO_H1MC_26_14 };
static s_disp_tekst const disp_IO_H1MC_27 = { Disp_Draw_Tekst_L, 9, 22, &tekst_IO_H1MC_27_14 };                                            
static s_disp_tekst const disp_IO_H1MC_28 = { Disp_Draw_Tekst_L, 9, 22, &tekst_IO_H1MC_28_14 };
static s_disp_tekst const disp_IO_H1MC_29 = { Disp_Draw_Tekst_L, 9, 22, &tekst_IO_H1MC_29_14 };
static s_disp_tekst const disp_IO_H1MC_30 = { Disp_Draw_Tekst_L, 9, 22, &tekst_IO_H1MC_30_14 };
static s_disp_tekst const disp_IO_H1MC_31 = { Disp_Draw_Tekst_L, 9, 22, &tekst_IO_H1MC_31_14 };                                            
static s_disp_tekst const disp_IO_H1MC_32 = { Disp_Draw_Tekst_L, 9, 22, &tekst_IO_H1MC_32_14 };
static s_disp_tekst const disp_IO_H1MC_e  = { Disp_Draw_Tekst_L, 9, 22, &tekst_IO_H1MC_e_14  };
static s_disp_value_add_option_on const disp_IO_H1MC_1_version_software  = { Disp_Draw_Value_Add_Option_On, (SIZE_7 | LINKS), INT, 2, &value.IO_H1MC[ 0].board_component.version_software, INT, &value.IO_H1MC[ 0].board_component.version_software };
static s_disp_value_add_option_on const disp_IO_H1MC_2_version_software  = { Disp_Draw_Value_Add_Option_On, (SIZE_7 | LINKS), INT, 2, &value.IO_H1MC[ 1].board_component.version_software, INT, &value.IO_H1MC[ 1].board_component.version_software };
static s_disp_value_add_option_on const disp_IO_H1MC_3_version_software  = { Disp_Draw_Value_Add_Option_On, (SIZE_7 | LINKS), INT, 2, &value.IO_H1MC[ 2].board_component.version_software, INT, &value.IO_H1MC[ 2].board_component.version_software };
static s_disp_value_add_option_on const disp_IO_H1MC_4_version_software  = { Disp_Draw_Value_Add_Option_On, (SIZE_7 | LINKS), INT, 2, &value.IO_H1MC[ 3].board_component.version_software, INT, &value.IO_H1MC[ 3].board_component.version_software };
static s_disp_value_add_option_on const disp_IO_H1MC_5_version_software  = { Disp_Draw_Value_Add_Option_On, (SIZE_7 | LINKS), INT, 2, &value.IO_H1MC[ 4].board_component.version_software, INT, &value.IO_H1MC[ 4].board_component.version_software };
static s_disp_value_add_option_on const disp_IO_H1MC_6_version_software  = { Disp_Draw_Value_Add_Option_On, (SIZE_7 | LINKS), INT, 2, &value.IO_H1MC[ 5].board_component.version_software, INT, &value.IO_H1MC[ 5].board_component.version_software };
static s_disp_value_add_option_on const disp_IO_H1MC_7_version_software  = { Disp_Draw_Value_Add_Option_On, (SIZE_7 | LINKS), INT, 2, &value.IO_H1MC[ 6].board_component.version_software, INT, &value.IO_H1MC[ 6].board_component.version_software };
static s_disp_value_add_option_on const disp_IO_H1MC_8_version_software  = { Disp_Draw_Value_Add_Option_On, (SIZE_7 | LINKS), INT, 2, &value.IO_H1MC[ 7].board_component.version_software, INT, &value.IO_H1MC[ 7].board_component.version_software };
static s_disp_value_add_option_on const disp_IO_H1MC_9_version_software  = { Disp_Draw_Value_Add_Option_On, (SIZE_7 | LINKS), INT, 2, &value.IO_H1MC[ 8].board_component.version_software, INT, &value.IO_H1MC[ 8].board_component.version_software };
static s_disp_value_add_option_on const disp_IO_H1MC_10_version_software = { Disp_Draw_Value_Add_Option_On, (SIZE_7 | LINKS), INT, 2, &value.IO_H1MC[ 9].board_component.version_software, INT, &value.IO_H1MC[ 9].board_component.version_software };
static s_disp_value_add_option_on const disp_IO_H1MC_11_version_software = { Disp_Draw_Value_Add_Option_On, (SIZE_7 | LINKS), INT, 2, &value.IO_H1MC[10].board_component.version_software, INT, &value.IO_H1MC[10].board_component.version_software };
static s_disp_value_add_option_on const disp_IO_H1MC_12_version_software = { Disp_Draw_Value_Add_Option_On, (SIZE_7 | LINKS), INT, 2, &value.IO_H1MC[11].board_component.version_software, INT, &value.IO_H1MC[11].board_component.version_software };
static s_disp_value_add_option_on const disp_IO_H1MC_13_version_software = { Disp_Draw_Value_Add_Option_On, (SIZE_7 | LINKS), INT, 2, &value.IO_H1MC[12].board_component.version_software, INT, &value.IO_H1MC[12].board_component.version_software };
static s_disp_value_add_option_on const disp_IO_H1MC_14_version_software = { Disp_Draw_Value_Add_Option_On, (SIZE_7 | LINKS), INT, 2, &value.IO_H1MC[13].board_component.version_software, INT, &value.IO_H1MC[13].board_component.version_software };
static s_disp_value_add_option_on const disp_IO_H1MC_15_version_software = { Disp_Draw_Value_Add_Option_On, (SIZE_7 | LINKS), INT, 2, &value.IO_H1MC[14].board_component.version_software, INT, &value.IO_H1MC[14].board_component.version_software };
static s_disp_value_add_option_on const disp_IO_H1MC_16_version_software = { Disp_Draw_Value_Add_Option_On, (SIZE_7 | LINKS), INT, 2, &value.IO_H1MC[15].board_component.version_software, INT, &value.IO_H1MC[15].board_component.version_software };
static s_disp_value_add_option_on const disp_IO_H1MC_17_version_software = { Disp_Draw_Value_Add_Option_On, (SIZE_7 | LINKS), INT, 2, &value.IO_H1MC[16].board_component.version_software, INT, &value.IO_H1MC[16].board_component.version_software };
static s_disp_value_add_option_on const disp_IO_H1MC_18_version_software = { Disp_Draw_Value_Add_Option_On, (SIZE_7 | LINKS), INT, 2, &value.IO_H1MC[17].board_component.version_software, INT, &value.IO_H1MC[17].board_component.version_software };
static s_disp_value_add_option_on const disp_IO_H1MC_19_version_software = { Disp_Draw_Value_Add_Option_On, (SIZE_7 | LINKS), INT, 2, &value.IO_H1MC[18].board_component.version_software, INT, &value.IO_H1MC[18].board_component.version_software };
static s_disp_value_add_option_on const disp_IO_H1MC_20_version_software = { Disp_Draw_Value_Add_Option_On, (SIZE_7 | LINKS), INT, 2, &value.IO_H1MC[19].board_component.version_software, INT, &value.IO_H1MC[19].board_component.version_software };
static s_disp_value_add_option_on const disp_IO_H1MC_21_version_software = { Disp_Draw_Value_Add_Option_On, (SIZE_7 | LINKS), INT, 2, &value.IO_H1MC[20].board_component.version_software, INT, &value.IO_H1MC[20].board_component.version_software };
static s_disp_value_add_option_on const disp_IO_H1MC_22_version_software = { Disp_Draw_Value_Add_Option_On, (SIZE_7 | LINKS), INT, 2, &value.IO_H1MC[21].board_component.version_software, INT, &value.IO_H1MC[21].board_component.version_software };
static s_disp_value_add_option_on const disp_IO_H1MC_23_version_software = { Disp_Draw_Value_Add_Option_On, (SIZE_7 | LINKS), INT, 2, &value.IO_H1MC[22].board_component.version_software, INT, &value.IO_H1MC[22].board_component.version_software };
static s_disp_value_add_option_on const disp_IO_H1MC_24_version_software = { Disp_Draw_Value_Add_Option_On, (SIZE_7 | LINKS), INT, 2, &value.IO_H1MC[23].board_component.version_software, INT, &value.IO_H1MC[23].board_component.version_software };
static s_disp_value_add_option_on const disp_IO_H1MC_25_version_software = { Disp_Draw_Value_Add_Option_On, (SIZE_7 | LINKS), INT, 2, &value.IO_H1MC[24].board_component.version_software, INT, &value.IO_H1MC[24].board_component.version_software };
static s_disp_value_add_option_on const disp_IO_H1MC_26_version_software = { Disp_Draw_Value_Add_Option_On, (SIZE_7 | LINKS), INT, 2, &value.IO_H1MC[25].board_component.version_software, INT, &value.IO_H1MC[25].board_component.version_software };
static s_disp_value_add_option_on const disp_IO_H1MC_27_version_software = { Disp_Draw_Value_Add_Option_On, (SIZE_7 | LINKS), INT, 2, &value.IO_H1MC[26].board_component.version_software, INT, &value.IO_H1MC[26].board_component.version_software };
static s_disp_value_add_option_on const disp_IO_H1MC_28_version_software = { Disp_Draw_Value_Add_Option_On, (SIZE_7 | LINKS), INT, 2, &value.IO_H1MC[27].board_component.version_software, INT, &value.IO_H1MC[27].board_component.version_software };
static s_disp_value_add_option_on const disp_IO_H1MC_29_version_software = { Disp_Draw_Value_Add_Option_On, (SIZE_7 | LINKS), INT, 2, &value.IO_H1MC[28].board_component.version_software, INT, &value.IO_H1MC[28].board_component.version_software };
static s_disp_value_add_option_on const disp_IO_H1MC_30_version_software = { Disp_Draw_Value_Add_Option_On, (SIZE_7 | LINKS), INT, 2, &value.IO_H1MC[29].board_component.version_software, INT, &value.IO_H1MC[29].board_component.version_software };
static s_disp_value_add_option_on const disp_IO_H1MC_31_version_software = { Disp_Draw_Value_Add_Option_On, (SIZE_7 | LINKS), INT, 2, &value.IO_H1MC[30].board_component.version_software, INT, &value.IO_H1MC[30].board_component.version_software };
static s_disp_value_add_option_on const disp_IO_H1MC_32_version_software = { Disp_Draw_Value_Add_Option_On, (SIZE_7 | LINKS), INT, 2, &value.IO_H1MC[31].board_component.version_software, INT, &value.IO_H1MC[31].board_component.version_software };

static void * const lcd_IO_H1MC_1_disp[]  = { &disp_IO_H1MC_1,  &disp_space_14_L, &disp_versienummer, &disp_space_7_L, &disp_IO_H1MC_1_version_software,  0 };
static void * const lcd_IO_H1MC_2_disp[]  = { &disp_IO_H1MC_2,  &disp_space_14_L, &disp_versienummer, &disp_space_7_L, &disp_IO_H1MC_2_version_software,  0 };
static void * const lcd_IO_H1MC_3_disp[]  = { &disp_IO_H1MC_3,  &disp_space_14_L, &disp_versienummer, &disp_space_7_L, &disp_IO_H1MC_3_version_software,  0 };
static void * const lcd_IO_H1MC_4_disp[]  = { &disp_IO_H1MC_4,  &disp_space_14_L, &disp_versienummer, &disp_space_7_L, &disp_IO_H1MC_4_version_software,  0 };
static void * const lcd_IO_H1MC_5_disp[]  = { &disp_IO_H1MC_5,  &disp_space_14_L, &disp_versienummer, &disp_space_7_L, &disp_IO_H1MC_5_version_software,  0 };
static void * const lcd_IO_H1MC_6_disp[]  = { &disp_IO_H1MC_6,  &disp_space_14_L, &disp_versienummer, &disp_space_7_L, &disp_IO_H1MC_6_version_software,  0 };
static void * const lcd_IO_H1MC_7_disp[]  = { &disp_IO_H1MC_7,  &disp_space_14_L, &disp_versienummer, &disp_space_7_L, &disp_IO_H1MC_7_version_software,  0 };
static void * const lcd_IO_H1MC_8_disp[]  = { &disp_IO_H1MC_8,  &disp_space_14_L, &disp_versienummer, &disp_space_7_L, &disp_IO_H1MC_8_version_software,  0 };
static void * const lcd_IO_H1MC_9_disp[]  = { &disp_IO_H1MC_9,  &disp_space_14_L, &disp_versienummer, &disp_space_7_L, &disp_IO_H1MC_9_version_software,  0 };
static void * const lcd_IO_H1MC_10_disp[] = { &disp_IO_H1MC_10, &disp_space_14_L, &disp_versienummer, &disp_space_7_L, &disp_IO_H1MC_10_version_software, 0 };
static void * const lcd_IO_H1MC_11_disp[] = { &disp_IO_H1MC_11, &disp_space_14_L, &disp_versienummer, &disp_space_7_L, &disp_IO_H1MC_11_version_software, 0 };
static void * const lcd_IO_H1MC_12_disp[] = { &disp_IO_H1MC_12, &disp_space_14_L, &disp_versienummer, &disp_space_7_L, &disp_IO_H1MC_12_version_software, 0 };
static void * const lcd_IO_H1MC_13_disp[] = { &disp_IO_H1MC_13, &disp_space_14_L, &disp_versienummer, &disp_space_7_L, &disp_IO_H1MC_13_version_software, 0 };
static void * const lcd_IO_H1MC_14_disp[] = { &disp_IO_H1MC_14, &disp_space_14_L, &disp_versienummer, &disp_space_7_L, &disp_IO_H1MC_14_version_software, 0 };
static void * const lcd_IO_H1MC_15_disp[] = { &disp_IO_H1MC_15, &disp_space_14_L, &disp_versienummer, &disp_space_7_L, &disp_IO_H1MC_15_version_software, 0 };
static void * const lcd_IO_H1MC_16_disp[] = { &disp_IO_H1MC_16, &disp_space_14_L, &disp_versienummer, &disp_space_7_L, &disp_IO_H1MC_16_version_software, 0 };
static void * const lcd_IO_H1MC_17_disp[] = { &disp_IO_H1MC_17, &disp_space_14_L, &disp_versienummer, &disp_space_7_L, &disp_IO_H1MC_17_version_software, 0 };
static void * const lcd_IO_H1MC_18_disp[] = { &disp_IO_H1MC_18, &disp_space_14_L, &disp_versienummer, &disp_space_7_L, &disp_IO_H1MC_18_version_software, 0 };
static void * const lcd_IO_H1MC_19_disp[] = { &disp_IO_H1MC_19, &disp_space_14_L, &disp_versienummer, &disp_space_7_L, &disp_IO_H1MC_19_version_software, 0 };
static void * const lcd_IO_H1MC_20_disp[] = { &disp_IO_H1MC_20, &disp_space_14_L, &disp_versienummer, &disp_space_7_L, &disp_IO_H1MC_20_version_software, 0 };
static void * const lcd_IO_H1MC_21_disp[] = { &disp_IO_H1MC_21, &disp_space_14_L, &disp_versienummer, &disp_space_7_L, &disp_IO_H1MC_21_version_software, 0 };
static void * const lcd_IO_H1MC_22_disp[] = { &disp_IO_H1MC_22, &disp_space_14_L, &disp_versienummer, &disp_space_7_L, &disp_IO_H1MC_22_version_software, 0 };
static void * const lcd_IO_H1MC_23_disp[] = { &disp_IO_H1MC_23, &disp_space_14_L, &disp_versienummer, &disp_space_7_L, &disp_IO_H1MC_23_version_software, 0 };
static void * const lcd_IO_H1MC_24_disp[] = { &disp_IO_H1MC_24, &disp_space_14_L, &disp_versienummer, &disp_space_7_L, &disp_IO_H1MC_24_version_software, 0 };
static void * const lcd_IO_H1MC_25_disp[] = { &disp_IO_H1MC_25, &disp_space_14_L, &disp_versienummer, &disp_space_7_L, &disp_IO_H1MC_25_version_software, 0 };
static void * const lcd_IO_H1MC_26_disp[] = { &disp_IO_H1MC_26, &disp_space_14_L, &disp_versienummer, &disp_space_7_L, &disp_IO_H1MC_26_version_software, 0 };
static void * const lcd_IO_H1MC_27_disp[] = { &disp_IO_H1MC_27, &disp_space_14_L, &disp_versienummer, &disp_space_7_L, &disp_IO_H1MC_27_version_software, 0 };
static void * const lcd_IO_H1MC_28_disp[] = { &disp_IO_H1MC_28, &disp_space_14_L, &disp_versienummer, &disp_space_7_L, &disp_IO_H1MC_28_version_software, 0 };
static void * const lcd_IO_H1MC_29_disp[] = { &disp_IO_H1MC_29, &disp_space_14_L, &disp_versienummer, &disp_space_7_L, &disp_IO_H1MC_29_version_software, 0 };
static void * const lcd_IO_H1MC_30_disp[] = { &disp_IO_H1MC_30, &disp_space_14_L, &disp_versienummer, &disp_space_7_L, &disp_IO_H1MC_30_version_software, 0 };
static void * const lcd_IO_H1MC_31_disp[] = { &disp_IO_H1MC_31, &disp_space_14_L, &disp_versienummer, &disp_space_7_L, &disp_IO_H1MC_31_version_software, 0 };
static void * const lcd_IO_H1MC_32_disp[] = { &disp_IO_H1MC_32, &disp_space_14_L, &disp_versienummer, &disp_space_7_L, &disp_IO_H1MC_32_version_software, 0 };
static void * const lcd_IO_H1MC_e_disp[]  = { &disp_IO_H1MC_e, 0 };

//-------------------------------------------------------------------------------------------------------------------------
// IO-H2MC
static void Arrow_IO_H2MC_Func(void);
static void Arrow_IO_H2MC_Empty_Func(void);

static unsigned char option_IO_H2MC[IO_H2MC_MAX+1];

static s_disp_tekst const disp_IO_H2MC_1  = { Disp_Draw_Tekst_L, 9, 22, &tekst_IO_H2MC_1_14  };
static s_disp_tekst const disp_IO_H2MC_2  = { Disp_Draw_Tekst_L, 9, 22, &tekst_IO_H2MC_2_14  };
static s_disp_tekst const disp_IO_H2MC_3  = { Disp_Draw_Tekst_L, 9, 22, &tekst_IO_H2MC_3_14  };                                            
static s_disp_tekst const disp_IO_H2MC_4  = { Disp_Draw_Tekst_L, 9, 22, &tekst_IO_H2MC_4_14  };
static s_disp_tekst const disp_IO_H2MC_5  = { Disp_Draw_Tekst_L, 9, 22, &tekst_IO_H2MC_5_14  };
static s_disp_tekst const disp_IO_H2MC_6  = { Disp_Draw_Tekst_L, 9, 22, &tekst_IO_H2MC_6_14  };
static s_disp_tekst const disp_IO_H2MC_7  = { Disp_Draw_Tekst_L, 9, 22, &tekst_IO_H2MC_7_14  };                                            
static s_disp_tekst const disp_IO_H2MC_8  = { Disp_Draw_Tekst_L, 9, 22, &tekst_IO_H2MC_8_14  };
static s_disp_tekst const disp_IO_H2MC_9  = { Disp_Draw_Tekst_L, 9, 22, &tekst_IO_H2MC_9_14  };
static s_disp_tekst const disp_IO_H2MC_10 = { Disp_Draw_Tekst_L, 9, 22, &tekst_IO_H2MC_10_14 };
static s_disp_tekst const disp_IO_H2MC_11 = { Disp_Draw_Tekst_L, 9, 22, &tekst_IO_H2MC_11_14 };                                            
static s_disp_tekst const disp_IO_H2MC_12 = { Disp_Draw_Tekst_L, 9, 22, &tekst_IO_H2MC_12_14 };
static s_disp_tekst const disp_IO_H2MC_13 = { Disp_Draw_Tekst_L, 9, 22, &tekst_IO_H2MC_13_14 };
static s_disp_tekst const disp_IO_H2MC_14 = { Disp_Draw_Tekst_L, 9, 22, &tekst_IO_H2MC_14_14 };
static s_disp_tekst const disp_IO_H2MC_15 = { Disp_Draw_Tekst_L, 9, 22, &tekst_IO_H2MC_15_14 };                                            
static s_disp_tekst const disp_IO_H2MC_16 = { Disp_Draw_Tekst_L, 9, 22, &tekst_IO_H2MC_16_14 };
static s_disp_tekst const disp_IO_H2MC_17 = { Disp_Draw_Tekst_L, 9, 22, &tekst_IO_H2MC_17_14 };
static s_disp_tekst const disp_IO_H2MC_18 = { Disp_Draw_Tekst_L, 9, 22, &tekst_IO_H2MC_18_14 };
static s_disp_tekst const disp_IO_H2MC_19 = { Disp_Draw_Tekst_L, 9, 22, &tekst_IO_H2MC_19_14 };                                            
static s_disp_tekst const disp_IO_H2MC_20 = { Disp_Draw_Tekst_L, 9, 22, &tekst_IO_H2MC_20_14 };
static s_disp_tekst const disp_IO_H2MC_21 = { Disp_Draw_Tekst_L, 9, 22, &tekst_IO_H2MC_21_14 };
static s_disp_tekst const disp_IO_H2MC_22 = { Disp_Draw_Tekst_L, 9, 22, &tekst_IO_H2MC_22_14 };
static s_disp_tekst const disp_IO_H2MC_23 = { Disp_Draw_Tekst_L, 9, 22, &tekst_IO_H2MC_23_14 };                                            
static s_disp_tekst const disp_IO_H2MC_24 = { Disp_Draw_Tekst_L, 9, 22, &tekst_IO_H2MC_24_14 };
static s_disp_tekst const disp_IO_H2MC_25 = { Disp_Draw_Tekst_L, 9, 22, &tekst_IO_H2MC_25_14 };
static s_disp_tekst const disp_IO_H2MC_26 = { Disp_Draw_Tekst_L, 9, 22, &tekst_IO_H2MC_26_14 };
static s_disp_tekst const disp_IO_H2MC_27 = { Disp_Draw_Tekst_L, 9, 22, &tekst_IO_H2MC_27_14 };                                            
static s_disp_tekst const disp_IO_H2MC_28 = { Disp_Draw_Tekst_L, 9, 22, &tekst_IO_H2MC_28_14 };
static s_disp_tekst const disp_IO_H2MC_29 = { Disp_Draw_Tekst_L, 9, 22, &tekst_IO_H2MC_29_14 };
static s_disp_tekst const disp_IO_H2MC_30 = { Disp_Draw_Tekst_L, 9, 22, &tekst_IO_H2MC_30_14 };
static s_disp_tekst const disp_IO_H2MC_31 = { Disp_Draw_Tekst_L, 9, 22, &tekst_IO_H2MC_31_14 };                                            
static s_disp_tekst const disp_IO_H2MC_32 = { Disp_Draw_Tekst_L, 9, 22, &tekst_IO_H2MC_32_14 };
static s_disp_tekst const disp_IO_H2MC_e  = { Disp_Draw_Tekst_L, 9, 22, &tekst_IO_H2MC_e_14  };
static s_disp_value_add_option_on const disp_IO_H2MC_1_version_software  = { Disp_Draw_Value_Add_Option_On, (SIZE_7 | LINKS), INT, 2, &value.IO_H2MC[ 0].board_component.version_software, INT, &value.IO_H2MC[ 0].board_component.version_software };
static s_disp_value_add_option_on const disp_IO_H2MC_2_version_software  = { Disp_Draw_Value_Add_Option_On, (SIZE_7 | LINKS), INT, 2, &value.IO_H2MC[ 1].board_component.version_software, INT, &value.IO_H2MC[ 1].board_component.version_software };
static s_disp_value_add_option_on const disp_IO_H2MC_3_version_software  = { Disp_Draw_Value_Add_Option_On, (SIZE_7 | LINKS), INT, 2, &value.IO_H2MC[ 2].board_component.version_software, INT, &value.IO_H2MC[ 2].board_component.version_software };
static s_disp_value_add_option_on const disp_IO_H2MC_4_version_software  = { Disp_Draw_Value_Add_Option_On, (SIZE_7 | LINKS), INT, 2, &value.IO_H2MC[ 3].board_component.version_software, INT, &value.IO_H2MC[ 3].board_component.version_software };
static s_disp_value_add_option_on const disp_IO_H2MC_5_version_software  = { Disp_Draw_Value_Add_Option_On, (SIZE_7 | LINKS), INT, 2, &value.IO_H2MC[ 4].board_component.version_software, INT, &value.IO_H2MC[ 4].board_component.version_software };
static s_disp_value_add_option_on const disp_IO_H2MC_6_version_software  = { Disp_Draw_Value_Add_Option_On, (SIZE_7 | LINKS), INT, 2, &value.IO_H2MC[ 5].board_component.version_software, INT, &value.IO_H2MC[ 5].board_component.version_software };
static s_disp_value_add_option_on const disp_IO_H2MC_7_version_software  = { Disp_Draw_Value_Add_Option_On, (SIZE_7 | LINKS), INT, 2, &value.IO_H2MC[ 6].board_component.version_software, INT, &value.IO_H2MC[ 6].board_component.version_software };
static s_disp_value_add_option_on const disp_IO_H2MC_8_version_software  = { Disp_Draw_Value_Add_Option_On, (SIZE_7 | LINKS), INT, 2, &value.IO_H2MC[ 7].board_component.version_software, INT, &value.IO_H2MC[ 7].board_component.version_software };
static s_disp_value_add_option_on const disp_IO_H2MC_9_version_software  = { Disp_Draw_Value_Add_Option_On, (SIZE_7 | LINKS), INT, 2, &value.IO_H2MC[ 8].board_component.version_software, INT, &value.IO_H2MC[ 8].board_component.version_software };
static s_disp_value_add_option_on const disp_IO_H2MC_10_version_software = { Disp_Draw_Value_Add_Option_On, (SIZE_7 | LINKS), INT, 2, &value.IO_H2MC[ 9].board_component.version_software, INT, &value.IO_H2MC[ 9].board_component.version_software };
static s_disp_value_add_option_on const disp_IO_H2MC_11_version_software = { Disp_Draw_Value_Add_Option_On, (SIZE_7 | LINKS), INT, 2, &value.IO_H2MC[10].board_component.version_software, INT, &value.IO_H2MC[10].board_component.version_software };
static s_disp_value_add_option_on const disp_IO_H2MC_12_version_software = { Disp_Draw_Value_Add_Option_On, (SIZE_7 | LINKS), INT, 2, &value.IO_H2MC[11].board_component.version_software, INT, &value.IO_H2MC[11].board_component.version_software };
static s_disp_value_add_option_on const disp_IO_H2MC_13_version_software = { Disp_Draw_Value_Add_Option_On, (SIZE_7 | LINKS), INT, 2, &value.IO_H2MC[12].board_component.version_software, INT, &value.IO_H2MC[12].board_component.version_software };
static s_disp_value_add_option_on const disp_IO_H2MC_14_version_software = { Disp_Draw_Value_Add_Option_On, (SIZE_7 | LINKS), INT, 2, &value.IO_H2MC[13].board_component.version_software, INT, &value.IO_H2MC[13].board_component.version_software };
static s_disp_value_add_option_on const disp_IO_H2MC_15_version_software = { Disp_Draw_Value_Add_Option_On, (SIZE_7 | LINKS), INT, 2, &value.IO_H2MC[14].board_component.version_software, INT, &value.IO_H2MC[14].board_component.version_software };
static s_disp_value_add_option_on const disp_IO_H2MC_16_version_software = { Disp_Draw_Value_Add_Option_On, (SIZE_7 | LINKS), INT, 2, &value.IO_H2MC[15].board_component.version_software, INT, &value.IO_H2MC[15].board_component.version_software };
static s_disp_value_add_option_on const disp_IO_H2MC_17_version_software = { Disp_Draw_Value_Add_Option_On, (SIZE_7 | LINKS), INT, 2, &value.IO_H2MC[16].board_component.version_software, INT, &value.IO_H2MC[16].board_component.version_software };
static s_disp_value_add_option_on const disp_IO_H2MC_18_version_software = { Disp_Draw_Value_Add_Option_On, (SIZE_7 | LINKS), INT, 2, &value.IO_H2MC[17].board_component.version_software, INT, &value.IO_H2MC[17].board_component.version_software };
static s_disp_value_add_option_on const disp_IO_H2MC_19_version_software = { Disp_Draw_Value_Add_Option_On, (SIZE_7 | LINKS), INT, 2, &value.IO_H2MC[18].board_component.version_software, INT, &value.IO_H2MC[18].board_component.version_software };
static s_disp_value_add_option_on const disp_IO_H2MC_20_version_software = { Disp_Draw_Value_Add_Option_On, (SIZE_7 | LINKS), INT, 2, &value.IO_H2MC[19].board_component.version_software, INT, &value.IO_H2MC[19].board_component.version_software };
static s_disp_value_add_option_on const disp_IO_H2MC_21_version_software = { Disp_Draw_Value_Add_Option_On, (SIZE_7 | LINKS), INT, 2, &value.IO_H2MC[20].board_component.version_software, INT, &value.IO_H2MC[20].board_component.version_software };
static s_disp_value_add_option_on const disp_IO_H2MC_22_version_software = { Disp_Draw_Value_Add_Option_On, (SIZE_7 | LINKS), INT, 2, &value.IO_H2MC[21].board_component.version_software, INT, &value.IO_H2MC[21].board_component.version_software };
static s_disp_value_add_option_on const disp_IO_H2MC_23_version_software = { Disp_Draw_Value_Add_Option_On, (SIZE_7 | LINKS), INT, 2, &value.IO_H2MC[22].board_component.version_software, INT, &value.IO_H2MC[22].board_component.version_software };
static s_disp_value_add_option_on const disp_IO_H2MC_24_version_software = { Disp_Draw_Value_Add_Option_On, (SIZE_7 | LINKS), INT, 2, &value.IO_H2MC[23].board_component.version_software, INT, &value.IO_H2MC[23].board_component.version_software };
static s_disp_value_add_option_on const disp_IO_H2MC_25_version_software = { Disp_Draw_Value_Add_Option_On, (SIZE_7 | LINKS), INT, 2, &value.IO_H2MC[24].board_component.version_software, INT, &value.IO_H2MC[24].board_component.version_software };
static s_disp_value_add_option_on const disp_IO_H2MC_26_version_software = { Disp_Draw_Value_Add_Option_On, (SIZE_7 | LINKS), INT, 2, &value.IO_H2MC[25].board_component.version_software, INT, &value.IO_H2MC[25].board_component.version_software };
static s_disp_value_add_option_on const disp_IO_H2MC_27_version_software = { Disp_Draw_Value_Add_Option_On, (SIZE_7 | LINKS), INT, 2, &value.IO_H2MC[26].board_component.version_software, INT, &value.IO_H2MC[26].board_component.version_software };
static s_disp_value_add_option_on const disp_IO_H2MC_28_version_software = { Disp_Draw_Value_Add_Option_On, (SIZE_7 | LINKS), INT, 2, &value.IO_H2MC[27].board_component.version_software, INT, &value.IO_H2MC[27].board_component.version_software };
static s_disp_value_add_option_on const disp_IO_H2MC_29_version_software = { Disp_Draw_Value_Add_Option_On, (SIZE_7 | LINKS), INT, 2, &value.IO_H2MC[28].board_component.version_software, INT, &value.IO_H2MC[28].board_component.version_software };
static s_disp_value_add_option_on const disp_IO_H2MC_30_version_software = { Disp_Draw_Value_Add_Option_On, (SIZE_7 | LINKS), INT, 2, &value.IO_H2MC[29].board_component.version_software, INT, &value.IO_H2MC[29].board_component.version_software };
static s_disp_value_add_option_on const disp_IO_H2MC_31_version_software = { Disp_Draw_Value_Add_Option_On, (SIZE_7 | LINKS), INT, 2, &value.IO_H2MC[30].board_component.version_software, INT, &value.IO_H2MC[30].board_component.version_software };
static s_disp_value_add_option_on const disp_IO_H2MC_32_version_software = { Disp_Draw_Value_Add_Option_On, (SIZE_7 | LINKS), INT, 2, &value.IO_H2MC[31].board_component.version_software, INT, &value.IO_H2MC[31].board_component.version_software };

static void * const lcd_IO_H2MC_1_disp[]  = { &disp_IO_H2MC_1,  &disp_space_14_L, &disp_versienummer, &disp_space_7_L, &disp_IO_H2MC_1_version_software,  0 };
static void * const lcd_IO_H2MC_2_disp[]  = { &disp_IO_H2MC_2,  &disp_space_14_L, &disp_versienummer, &disp_space_7_L, &disp_IO_H2MC_2_version_software,  0 };
static void * const lcd_IO_H2MC_3_disp[]  = { &disp_IO_H2MC_3,  &disp_space_14_L, &disp_versienummer, &disp_space_7_L, &disp_IO_H2MC_3_version_software,  0 };
static void * const lcd_IO_H2MC_4_disp[]  = { &disp_IO_H2MC_4,  &disp_space_14_L, &disp_versienummer, &disp_space_7_L, &disp_IO_H2MC_4_version_software,  0 };
static void * const lcd_IO_H2MC_5_disp[]  = { &disp_IO_H2MC_5,  &disp_space_14_L, &disp_versienummer, &disp_space_7_L, &disp_IO_H2MC_5_version_software,  0 };
static void * const lcd_IO_H2MC_6_disp[]  = { &disp_IO_H2MC_6,  &disp_space_14_L, &disp_versienummer, &disp_space_7_L, &disp_IO_H2MC_6_version_software,  0 };
static void * const lcd_IO_H2MC_7_disp[]  = { &disp_IO_H2MC_7,  &disp_space_14_L, &disp_versienummer, &disp_space_7_L, &disp_IO_H2MC_7_version_software,  0 };
static void * const lcd_IO_H2MC_8_disp[]  = { &disp_IO_H2MC_8,  &disp_space_14_L, &disp_versienummer, &disp_space_7_L, &disp_IO_H2MC_8_version_software,  0 };
static void * const lcd_IO_H2MC_9_disp[]  = { &disp_IO_H2MC_9,  &disp_space_14_L, &disp_versienummer, &disp_space_7_L, &disp_IO_H2MC_9_version_software,  0 };
static void * const lcd_IO_H2MC_10_disp[] = { &disp_IO_H2MC_10, &disp_space_14_L, &disp_versienummer, &disp_space_7_L, &disp_IO_H2MC_10_version_software, 0 };
static void * const lcd_IO_H2MC_11_disp[] = { &disp_IO_H2MC_11, &disp_space_14_L, &disp_versienummer, &disp_space_7_L, &disp_IO_H2MC_11_version_software, 0 };
static void * const lcd_IO_H2MC_12_disp[] = { &disp_IO_H2MC_12, &disp_space_14_L, &disp_versienummer, &disp_space_7_L, &disp_IO_H2MC_12_version_software, 0 };
static void * const lcd_IO_H2MC_13_disp[] = { &disp_IO_H2MC_13, &disp_space_14_L, &disp_versienummer, &disp_space_7_L, &disp_IO_H2MC_13_version_software, 0 };
static void * const lcd_IO_H2MC_14_disp[] = { &disp_IO_H2MC_14, &disp_space_14_L, &disp_versienummer, &disp_space_7_L, &disp_IO_H2MC_14_version_software, 0 };
static void * const lcd_IO_H2MC_15_disp[] = { &disp_IO_H2MC_15, &disp_space_14_L, &disp_versienummer, &disp_space_7_L, &disp_IO_H2MC_15_version_software, 0 };
static void * const lcd_IO_H2MC_16_disp[] = { &disp_IO_H2MC_16, &disp_space_14_L, &disp_versienummer, &disp_space_7_L, &disp_IO_H2MC_16_version_software, 0 };
static void * const lcd_IO_H2MC_17_disp[] = { &disp_IO_H2MC_17, &disp_space_14_L, &disp_versienummer, &disp_space_7_L, &disp_IO_H2MC_17_version_software, 0 };
static void * const lcd_IO_H2MC_18_disp[] = { &disp_IO_H2MC_18, &disp_space_14_L, &disp_versienummer, &disp_space_7_L, &disp_IO_H2MC_18_version_software, 0 };
static void * const lcd_IO_H2MC_19_disp[] = { &disp_IO_H2MC_19, &disp_space_14_L, &disp_versienummer, &disp_space_7_L, &disp_IO_H2MC_19_version_software, 0 };
static void * const lcd_IO_H2MC_20_disp[] = { &disp_IO_H2MC_20, &disp_space_14_L, &disp_versienummer, &disp_space_7_L, &disp_IO_H2MC_20_version_software, 0 };
static void * const lcd_IO_H2MC_21_disp[] = { &disp_IO_H2MC_21, &disp_space_14_L, &disp_versienummer, &disp_space_7_L, &disp_IO_H2MC_21_version_software, 0 };
static void * const lcd_IO_H2MC_22_disp[] = { &disp_IO_H2MC_22, &disp_space_14_L, &disp_versienummer, &disp_space_7_L, &disp_IO_H2MC_22_version_software, 0 };
static void * const lcd_IO_H2MC_23_disp[] = { &disp_IO_H2MC_23, &disp_space_14_L, &disp_versienummer, &disp_space_7_L, &disp_IO_H2MC_23_version_software, 0 };
static void * const lcd_IO_H2MC_24_disp[] = { &disp_IO_H2MC_24, &disp_space_14_L, &disp_versienummer, &disp_space_7_L, &disp_IO_H2MC_24_version_software, 0 };
static void * const lcd_IO_H2MC_25_disp[] = { &disp_IO_H2MC_25, &disp_space_14_L, &disp_versienummer, &disp_space_7_L, &disp_IO_H2MC_25_version_software, 0 };
static void * const lcd_IO_H2MC_26_disp[] = { &disp_IO_H2MC_26, &disp_space_14_L, &disp_versienummer, &disp_space_7_L, &disp_IO_H2MC_26_version_software, 0 };
static void * const lcd_IO_H2MC_27_disp[] = { &disp_IO_H2MC_27, &disp_space_14_L, &disp_versienummer, &disp_space_7_L, &disp_IO_H2MC_27_version_software, 0 };
static void * const lcd_IO_H2MC_28_disp[] = { &disp_IO_H2MC_28, &disp_space_14_L, &disp_versienummer, &disp_space_7_L, &disp_IO_H2MC_28_version_software, 0 };
static void * const lcd_IO_H2MC_29_disp[] = { &disp_IO_H2MC_29, &disp_space_14_L, &disp_versienummer, &disp_space_7_L, &disp_IO_H2MC_29_version_software, 0 };
static void * const lcd_IO_H2MC_30_disp[] = { &disp_IO_H2MC_30, &disp_space_14_L, &disp_versienummer, &disp_space_7_L, &disp_IO_H2MC_30_version_software, 0 };
static void * const lcd_IO_H2MC_31_disp[] = { &disp_IO_H2MC_31, &disp_space_14_L, &disp_versienummer, &disp_space_7_L, &disp_IO_H2MC_31_version_software, 0 };
static void * const lcd_IO_H2MC_32_disp[] = { &disp_IO_H2MC_32, &disp_space_14_L, &disp_versienummer, &disp_space_7_L, &disp_IO_H2MC_32_version_software, 0 };
static void * const lcd_IO_H2MC_e_disp[]  = { &disp_IO_H2MC_e, 0 };

//-------------------------------------------------------------------------------------------------------------------------
// IO-EKU
static void Arrow_IO_EKU_Func(void);
static void Arrow_IO_EKU_Empty_Func(void);

static unsigned char option_IO_EKU[IO_EKU_MAX+1];

static s_disp_tekst const disp_IO_EKU_1  = { Disp_Draw_Tekst_L, 9, 22, &tekst_IO_EKU_1_14  };
static s_disp_tekst const disp_IO_EKU_2  = { Disp_Draw_Tekst_L, 9, 22, &tekst_IO_EKU_2_14  };
static s_disp_tekst const disp_IO_EKU_3  = { Disp_Draw_Tekst_L, 9, 22, &tekst_IO_EKU_3_14  };                                            
static s_disp_tekst const disp_IO_EKU_4  = { Disp_Draw_Tekst_L, 9, 22, &tekst_IO_EKU_4_14  };
static s_disp_tekst const disp_IO_EKU_5  = { Disp_Draw_Tekst_L, 9, 22, &tekst_IO_EKU_5_14  };
static s_disp_tekst const disp_IO_EKU_6  = { Disp_Draw_Tekst_L, 9, 22, &tekst_IO_EKU_6_14  };
static s_disp_tekst const disp_IO_EKU_7  = { Disp_Draw_Tekst_L, 9, 22, &tekst_IO_EKU_7_14  };                                            
static s_disp_tekst const disp_IO_EKU_8  = { Disp_Draw_Tekst_L, 9, 22, &tekst_IO_EKU_8_14  };
static s_disp_tekst const disp_IO_EKU_9  = { Disp_Draw_Tekst_L, 9, 22, &tekst_IO_EKU_9_14  };
static s_disp_tekst const disp_IO_EKU_10 = { Disp_Draw_Tekst_L, 9, 22, &tekst_IO_EKU_10_14 };
static s_disp_tekst const disp_IO_EKU_11 = { Disp_Draw_Tekst_L, 9, 22, &tekst_IO_EKU_11_14 };                                            
static s_disp_tekst const disp_IO_EKU_12 = { Disp_Draw_Tekst_L, 9, 22, &tekst_IO_EKU_12_14 };
static s_disp_tekst const disp_IO_EKU_13 = { Disp_Draw_Tekst_L, 9, 22, &tekst_IO_EKU_13_14 };
static s_disp_tekst const disp_IO_EKU_14 = { Disp_Draw_Tekst_L, 9, 22, &tekst_IO_EKU_14_14 };
static s_disp_tekst const disp_IO_EKU_15 = { Disp_Draw_Tekst_L, 9, 22, &tekst_IO_EKU_15_14 };                                            
static s_disp_tekst const disp_IO_EKU_16 = { Disp_Draw_Tekst_L, 9, 22, &tekst_IO_EKU_16_14 };
static s_disp_tekst const disp_IO_EKU_e  = { Disp_Draw_Tekst_L, 9, 22, &tekst_IO_EKU_e_14  };
static s_disp_value_add_option_on const disp_IO_EKU_1_version_software  = { Disp_Draw_Value_Add_Option_On, (SIZE_7 | LINKS), INT, 2, &value.IO_EKU[ 0].board_component.version_software, INT, &value.IO_EKU[ 0].board_component.version_software };
static s_disp_value_add_option_on const disp_IO_EKU_2_version_software  = { Disp_Draw_Value_Add_Option_On, (SIZE_7 | LINKS), INT, 2, &value.IO_EKU[ 1].board_component.version_software, INT, &value.IO_EKU[ 1].board_component.version_software };
static s_disp_value_add_option_on const disp_IO_EKU_3_version_software  = { Disp_Draw_Value_Add_Option_On, (SIZE_7 | LINKS), INT, 2, &value.IO_EKU[ 2].board_component.version_software, INT, &value.IO_EKU[ 2].board_component.version_software };
static s_disp_value_add_option_on const disp_IO_EKU_4_version_software  = { Disp_Draw_Value_Add_Option_On, (SIZE_7 | LINKS), INT, 2, &value.IO_EKU[ 3].board_component.version_software, INT, &value.IO_EKU[ 3].board_component.version_software };
static s_disp_value_add_option_on const disp_IO_EKU_5_version_software  = { Disp_Draw_Value_Add_Option_On, (SIZE_7 | LINKS), INT, 2, &value.IO_EKU[ 4].board_component.version_software, INT, &value.IO_EKU[ 4].board_component.version_software };
static s_disp_value_add_option_on const disp_IO_EKU_6_version_software  = { Disp_Draw_Value_Add_Option_On, (SIZE_7 | LINKS), INT, 2, &value.IO_EKU[ 5].board_component.version_software, INT, &value.IO_EKU[ 5].board_component.version_software };
static s_disp_value_add_option_on const disp_IO_EKU_7_version_software  = { Disp_Draw_Value_Add_Option_On, (SIZE_7 | LINKS), INT, 2, &value.IO_EKU[ 6].board_component.version_software, INT, &value.IO_EKU[ 6].board_component.version_software };
static s_disp_value_add_option_on const disp_IO_EKU_8_version_software  = { Disp_Draw_Value_Add_Option_On, (SIZE_7 | LINKS), INT, 2, &value.IO_EKU[ 7].board_component.version_software, INT, &value.IO_EKU[ 7].board_component.version_software };
static s_disp_value_add_option_on const disp_IO_EKU_9_version_software  = { Disp_Draw_Value_Add_Option_On, (SIZE_7 | LINKS), INT, 2, &value.IO_EKU[ 8].board_component.version_software, INT, &value.IO_EKU[ 8].board_component.version_software };
static s_disp_value_add_option_on const disp_IO_EKU_10_version_software = { Disp_Draw_Value_Add_Option_On, (SIZE_7 | LINKS), INT, 2, &value.IO_EKU[ 9].board_component.version_software, INT, &value.IO_EKU[ 9].board_component.version_software };
static s_disp_value_add_option_on const disp_IO_EKU_11_version_software = { Disp_Draw_Value_Add_Option_On, (SIZE_7 | LINKS), INT, 2, &value.IO_EKU[10].board_component.version_software, INT, &value.IO_EKU[10].board_component.version_software };
static s_disp_value_add_option_on const disp_IO_EKU_12_version_software = { Disp_Draw_Value_Add_Option_On, (SIZE_7 | LINKS), INT, 2, &value.IO_EKU[11].board_component.version_software, INT, &value.IO_EKU[11].board_component.version_software };
static s_disp_value_add_option_on const disp_IO_EKU_13_version_software = { Disp_Draw_Value_Add_Option_On, (SIZE_7 | LINKS), INT, 2, &value.IO_EKU[12].board_component.version_software, INT, &value.IO_EKU[12].board_component.version_software };
static s_disp_value_add_option_on const disp_IO_EKU_14_version_software = { Disp_Draw_Value_Add_Option_On, (SIZE_7 | LINKS), INT, 2, &value.IO_EKU[13].board_component.version_software, INT, &value.IO_EKU[13].board_component.version_software };
static s_disp_value_add_option_on const disp_IO_EKU_15_version_software = { Disp_Draw_Value_Add_Option_On, (SIZE_7 | LINKS), INT, 2, &value.IO_EKU[14].board_component.version_software, INT, &value.IO_EKU[14].board_component.version_software };
static s_disp_value_add_option_on const disp_IO_EKU_16_version_software = { Disp_Draw_Value_Add_Option_On, (SIZE_7 | LINKS), INT, 2, &value.IO_EKU[15].board_component.version_software, INT, &value.IO_EKU[15].board_component.version_software };

static void * const lcd_IO_EKU_1_disp[]  = { &disp_IO_EKU_1,  &disp_space_14_L, &disp_versienummer, &disp_space_7_L, &disp_IO_EKU_1_version_software,  0 };
static void * const lcd_IO_EKU_2_disp[]  = { &disp_IO_EKU_2,  &disp_space_14_L, &disp_versienummer, &disp_space_7_L, &disp_IO_EKU_2_version_software,  0 };
static void * const lcd_IO_EKU_3_disp[]  = { &disp_IO_EKU_3,  &disp_space_14_L, &disp_versienummer, &disp_space_7_L, &disp_IO_EKU_3_version_software,  0 };
static void * const lcd_IO_EKU_4_disp[]  = { &disp_IO_EKU_4,  &disp_space_14_L, &disp_versienummer, &disp_space_7_L, &disp_IO_EKU_4_version_software,  0 };
static void * const lcd_IO_EKU_5_disp[]  = { &disp_IO_EKU_5,  &disp_space_14_L, &disp_versienummer, &disp_space_7_L, &disp_IO_EKU_5_version_software,  0 };
static void * const lcd_IO_EKU_6_disp[]  = { &disp_IO_EKU_6,  &disp_space_14_L, &disp_versienummer, &disp_space_7_L, &disp_IO_EKU_6_version_software,  0 };
static void * const lcd_IO_EKU_7_disp[]  = { &disp_IO_EKU_7,  &disp_space_14_L, &disp_versienummer, &disp_space_7_L, &disp_IO_EKU_7_version_software,  0 };
static void * const lcd_IO_EKU_8_disp[]  = { &disp_IO_EKU_8,  &disp_space_14_L, &disp_versienummer, &disp_space_7_L, &disp_IO_EKU_8_version_software,  0 };
static void * const lcd_IO_EKU_9_disp[]  = { &disp_IO_EKU_9,  &disp_space_14_L, &disp_versienummer, &disp_space_7_L, &disp_IO_EKU_9_version_software,  0 };
static void * const lcd_IO_EKU_10_disp[] = { &disp_IO_EKU_10, &disp_space_14_L, &disp_versienummer, &disp_space_7_L, &disp_IO_EKU_10_version_software, 0 };
static void * const lcd_IO_EKU_11_disp[] = { &disp_IO_EKU_11, &disp_space_14_L, &disp_versienummer, &disp_space_7_L, &disp_IO_EKU_11_version_software, 0 };
static void * const lcd_IO_EKU_12_disp[] = { &disp_IO_EKU_12, &disp_space_14_L, &disp_versienummer, &disp_space_7_L, &disp_IO_EKU_12_version_software, 0 };
static void * const lcd_IO_EKU_13_disp[] = { &disp_IO_EKU_13, &disp_space_14_L, &disp_versienummer, &disp_space_7_L, &disp_IO_EKU_13_version_software, 0 };
static void * const lcd_IO_EKU_14_disp[] = { &disp_IO_EKU_14, &disp_space_14_L, &disp_versienummer, &disp_space_7_L, &disp_IO_EKU_14_version_software, 0 };
static void * const lcd_IO_EKU_15_disp[] = { &disp_IO_EKU_15, &disp_space_14_L, &disp_versienummer, &disp_space_7_L, &disp_IO_EKU_15_version_software, 0 };
static void * const lcd_IO_EKU_16_disp[] = { &disp_IO_EKU_16, &disp_space_14_L, &disp_versienummer, &disp_space_7_L, &disp_IO_EKU_16_version_software, 0 };
static void * const lcd_IO_EKU_e_disp[]  = { &disp_IO_EKU_e, 0 };

//-------------------------------------------------------------------------------------------------------------------------


s_key_action const opt_IO_2_key_action[] =
{
  {
    DISP_IO_08_09+0,                  // nr
    0,                                // index
    &option_IO_08_09[0],              // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_08_09_1_disp,              // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_08_09_Func,              // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    DISP_IO_08_09+1,                  // nr
    0,                                // index
    &option_IO_08_09[1],              // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_08_09_2_disp,              // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_08_09_Func,              // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    DISP_IO_08_09+2,                  // nr
    0,                                // index
    &option_IO_08_09[2],              // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_08_09_3_disp,              // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_08_09_Func,              // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    DISP_IO_08_09+3,                  // nr
    0,                                // index
    &option_IO_08_09[3],              // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_08_09_4_disp,              // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_08_09_Func,              // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    DISP_IO_08_09+4,                  // nr
    0,                                // index
    &option_IO_08_09[4],              // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_08_09_5_disp,              // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_08_09_Func,              // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    DISP_IO_08_09+5,                  // nr
    0,                                // index
    &option_IO_08_09[5],              // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_08_09_6_disp,              // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_08_09_Func,              // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    DISP_IO_08_09+6,                  // nr
    0,                                // index
    &option_IO_08_09[6],              // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_08_09_7_disp,              // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_08_09_Func,              // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    DISP_IO_08_09+7,                  // nr
    0,                                // index
    &option_IO_08_09[7],              // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_08_09_8_disp,              // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_08_09_Func,              // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    DISP_IO_08_09+8,                  // nr
    0,                                // index
    &option_IO_08_09[8],              // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_08_09_e_disp,              // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_08_09_Empty_Func,        // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    DISP_IO_05_07+0,                  // nr
    0,                                // index
    &option_IO_05_07[0],              // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_05_07_1_disp,              // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_05_07_Func,              // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    DISP_IO_05_07+1,                  // nr
    0,                                // index
    &option_IO_05_07[1],              // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_05_07_2_disp,              // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_05_07_Func,              // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    DISP_IO_05_07+2,                  // nr
    0,                                // index
    &option_IO_05_07[2],              // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_05_07_3_disp,              // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_05_07_Func,              // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    DISP_IO_05_07+3,                  // nr
    0,                                // index
    &option_IO_05_07[3],              // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_05_07_4_disp,              // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_05_07_Func,              // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    DISP_IO_05_07+4,                  // nr
    0,                                // index
    &option_IO_05_07[4],              // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_05_07_5_disp,              // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_05_07_Func,              // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    DISP_IO_05_07+5,                  // nr
    0,                                // index
    &option_IO_05_07[5],              // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_05_07_6_disp,              // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_05_07_Func,              // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    DISP_IO_05_07+6,                  // nr
    0,                                // index
    &option_IO_05_07[6],              // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_05_07_7_disp,              // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_05_07_Func,              // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    DISP_IO_05_07+7,                  // nr
    0,                                // index
    &option_IO_05_07[7],              // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_05_07_8_disp,              // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_05_07_Func,              // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    DISP_IO_05_07+8,                  // nr
    0,                                // index
    &option_IO_05_07[8],              // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_05_07_9_disp,              // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_05_07_Func,              // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    DISP_IO_05_07+9,                  // nr
    0,                                // index
    &option_IO_05_07[9],              // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_05_07_10_disp,             // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_05_07_Func,              // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    DISP_IO_05_07+10,                 // nr
    0,                                // index
    &option_IO_05_07[10],             // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_05_07_11_disp,             // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_05_07_Func,              // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    DISP_IO_05_07+11,                 // nr
    0,                                // index
    &option_IO_05_07[11],             // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_05_07_12_disp,             // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_05_07_Func,              // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    DISP_IO_05_07+12,                 // nr
    0,                                // index
    &option_IO_05_07[12],             // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_05_07_13_disp,             // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_05_07_Func,              // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    DISP_IO_05_07+13,                 // nr
    0,                                // index
    &option_IO_05_07[13],             // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_05_07_14_disp,             // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_05_07_Func,              // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    DISP_IO_05_07+14,                 // nr
    0,                                // index
    &option_IO_05_07[14],             // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_05_07_15_disp,             // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_05_07_Func,              // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    DISP_IO_05_07+15,                 // nr
    0,                                // index
    &option_IO_05_07[15],             // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_05_07_16_disp,             // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_05_07_Func,              // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    DISP_IO_05_07+16,                 // nr
    0,                                // index
    &option_IO_05_07[16],             // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_05_07_e_disp,              // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_05_07_Empty_Func,        // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    DISP_IO_06_14+0,                  // nr
    0,                                // index
    &option_IO_06_14[0],              // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_06_14_1_disp,              // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_06_14_Func,              // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    DISP_IO_06_14+1,                  // nr
    0,                                // index
    &option_IO_06_14[1],              // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_06_14_2_disp,              // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_06_14_Func,              // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    DISP_IO_06_14+2,                  // nr
    0,                                // index
    &option_IO_06_14[2],              // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_06_14_3_disp,              // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_06_14_Func,              // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    DISP_IO_06_14+3,                  // nr
    0,                                // index
    &option_IO_06_14[3],              // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_06_14_4_disp,              // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_06_14_Func,              // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    DISP_IO_06_14+4,                  // nr
    0,                                // index
    &option_IO_06_14[4],              // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_06_14_e_disp,              // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_06_14_Empty_Func,        // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    DISP_IO_07_07+0,                  // nr
    0,                                // index
    &option_IO_07_07[0],              // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_07_07_1_disp,              // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_07_07_Func,              // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    DISP_IO_07_07+1,                  // nr
    0,                                // index
    &option_IO_07_07[1],              // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_07_07_2_disp,              // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_07_07_Func,              // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    DISP_IO_07_07+2,                  // nr
    0,                                // index
    &option_IO_07_07[2],              // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_07_07_3_disp,              // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_07_07_Func,              // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    DISP_IO_07_07+3,                  // nr
    0,                                // index
    &option_IO_07_07[3],              // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_07_07_4_disp,              // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_07_07_Func,              // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    DISP_IO_07_07+4,                  // nr
    0,                                // index
    &option_IO_07_07[4],              // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_07_07_5_disp,              // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_07_07_Func,              // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    DISP_IO_07_07+5,                  // nr
    0,                                // index
    &option_IO_07_07[5],              // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_07_07_6_disp,              // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_07_07_Func,              // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    DISP_IO_07_07+6,                  // nr
    0,                                // index
    &option_IO_07_07[6],              // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_07_07_7_disp,              // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_07_07_Func,              // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    DISP_IO_07_07+7,                  // nr
    0,                                // index
    &option_IO_07_07[7],              // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_07_07_8_disp,              // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_07_07_Func,              // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    DISP_IO_07_07+8,                  // nr
    0,                                // index
    &option_IO_07_07[8],              // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_07_07_9_disp,              // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_07_07_Func,              // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    DISP_IO_07_07+9,                  // nr
    0,                                // index
    &option_IO_07_07[9],              // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_07_07_10_disp,             // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_07_07_Func,              // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    DISP_IO_07_07+10,                 // nr
    0,                                // index
    &option_IO_07_07[10],             // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_07_07_11_disp,             // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_07_07_Func,              // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    DISP_IO_07_07+11,                 // nr
    0,                                // index
    &option_IO_07_07[11],             // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_07_07_12_disp,             // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_07_07_Func,              // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    DISP_IO_07_07+12,                 // nr
    0,                                // index
    &option_IO_07_07[12],             // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_07_07_13_disp,             // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_07_07_Func,              // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    DISP_IO_07_07+13,                 // nr
    0,                                // index
    &option_IO_07_07[13],             // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_07_07_14_disp,             // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_07_07_Func,              // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    DISP_IO_07_07+14,                 // nr
    0,                                // index
    &option_IO_07_07[14],             // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_07_07_15_disp,             // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_07_07_Func,              // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    DISP_IO_07_07+15,                 // nr
    0,                                // index
    &option_IO_07_07[15],             // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_07_07_16_disp,             // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_07_07_Func,              // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    DISP_IO_07_07+16,                 // nr
    0,                                // index
    &option_IO_07_07[16],             // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_07_07_17_disp,             // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_07_07_Func,              // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    DISP_IO_07_07+17,                 // nr
    0,                                // index
    &option_IO_07_07[17],             // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_07_07_18_disp,             // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_07_07_Func,              // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    DISP_IO_07_07+18,                 // nr
    0,                                // index
    &option_IO_07_07[18],             // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_07_07_19_disp,             // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_07_07_Func,              // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    DISP_IO_07_07+19,                 // nr
    0,                                // index
    &option_IO_07_07[19],             // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_07_07_20_disp,             // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_07_07_Func,              // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    DISP_IO_07_07+20,                 // nr
    0,                                // index
    &option_IO_07_07[20],             // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_07_07_21_disp,             // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_07_07_Func,              // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    DISP_IO_07_07+21,                 // nr
    0,                                // index
    &option_IO_07_07[21],             // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_07_07_22_disp,             // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_07_07_Func,              // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    DISP_IO_07_07+22,                 // nr
    0,                                // index
    &option_IO_07_07[22],             // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_07_07_23_disp,             // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_07_07_Func,              // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    DISP_IO_07_07+23,                 // nr
    0,                                // index
    &option_IO_07_07[23],             // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_07_07_24_disp,             // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_07_07_Func,              // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    DISP_IO_07_07+24,                 // nr
    0,                                // index
    &option_IO_07_07[24],             // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_07_07_25_disp,             // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_07_07_Func,              // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    DISP_IO_07_07+25,                 // nr
    0,                                // index
    &option_IO_07_07[25],             // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_07_07_26_disp,             // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_07_07_Func,              // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    DISP_IO_07_07+26,                 // nr
    0,                                // index
    &option_IO_07_07[26],             // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_07_07_27_disp,             // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_07_07_Func,              // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    DISP_IO_07_07+27,                 // nr
    0,                                // index
    &option_IO_07_07[27],             // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_07_07_28_disp,             // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_07_07_Func,              // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    DISP_IO_07_07+28,                 // nr
    0,                                // index
    &option_IO_07_07[28],             // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_07_07_29_disp,             // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_07_07_Func,              // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    DISP_IO_07_07+29,                 // nr
    0,                                // index
    &option_IO_07_07[29],             // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_07_07_30_disp,             // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_07_07_Func,              // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    DISP_IO_07_07+30,                 // nr
    0,                                // index
    &option_IO_07_07[30],             // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_07_07_31_disp,             // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_07_07_Func,              // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    DISP_IO_07_07+31,                 // nr
    0,                                // index
    &option_IO_07_07[31],             // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_07_07_32_disp,             // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_07_07_Func,              // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    DISP_IO_07_07+32,                 // nr
    0,                                // index
    &option_IO_07_07[32],             // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_07_07_e_disp,              // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_07_07_Empty_Func,        // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    DISP_IO_12_06+0,                  // nr
    0,                                // index
    &option_IO_12_06[0],              // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_12_06_1_disp,              // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_12_06_Func,              // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    DISP_IO_12_06+1,                  // nr
    0,                                // index
    &option_IO_12_06[1],              // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_12_06_2_disp,              // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_12_06_Func,              // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    DISP_IO_12_06+2,                  // nr
    0,                                // index
    &option_IO_12_06[2],              // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_12_06_3_disp,              // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_12_06_Func,              // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    DISP_IO_12_06+3,                  // nr
    0,                                // index
    &option_IO_12_06[3],              // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_12_06_4_disp,              // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_12_06_Func,              // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    DISP_IO_12_06+4,                  // nr
    0,                                // index
    &option_IO_12_06[4],              // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_12_06_e_disp,              // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_12_06_Empty_Func,        // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    DISP_IO_H1MC+0,                   // nr
    0,                                // index
    &option_IO_H1MC[0],               // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_H1MC_1_disp,               // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_H1MC_Func,               // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    DISP_IO_H1MC+1,                   // nr
    0,                                // index
    &option_IO_H1MC[1],               // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_H1MC_2_disp,               // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_H1MC_Func,               // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    DISP_IO_H1MC+2,                   // nr
    0,                                // index
    &option_IO_H1MC[2],               // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_H1MC_3_disp,               // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_H1MC_Func,               // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    DISP_IO_H1MC+3,                   // nr
    0,                                // index
    &option_IO_H1MC[3],               // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_H1MC_4_disp,               // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_H1MC_Func,               // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    DISP_IO_H1MC+4,                   // nr
    0,                                // index
    &option_IO_H1MC[4],               // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_H1MC_5_disp,               // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_H1MC_Func,               // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    DISP_IO_H1MC+5,                   // nr
    0,                                // index
    &option_IO_H1MC[5],               // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_H1MC_6_disp,               // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_H1MC_Func,               // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    DISP_IO_H1MC+6,                   // nr
    0,                                // index
    &option_IO_H1MC[6],               // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_H1MC_7_disp,               // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_H1MC_Func,               // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    DISP_IO_H1MC+7,                   // nr
    0,                                // index
    &option_IO_H1MC[7],               // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_H1MC_8_disp,               // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_H1MC_Func,               // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    DISP_IO_H1MC+8,                   // nr
    0,                                // index
    &option_IO_H1MC[8],               // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_H1MC_9_disp,               // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_H1MC_Func,               // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    DISP_IO_H1MC+9,                   // nr
    0,                                // index
    &option_IO_H1MC[9],               // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_H1MC_10_disp,              // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_H1MC_Func,               // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    DISP_IO_H1MC+10,                  // nr
    0,                                // index
    &option_IO_H1MC[10],              // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_H1MC_11_disp,              // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_H1MC_Func,               // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    DISP_IO_H1MC+11,                  // nr
    0,                                // index
    &option_IO_H1MC[11],              // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_H1MC_12_disp,              // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_H1MC_Func,               // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    DISP_IO_H1MC+12,                  // nr
    0,                                // index
    &option_IO_H1MC[12],              // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_H1MC_13_disp,              // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_H1MC_Func,               // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    DISP_IO_H1MC+13,                  // nr
    0,                                // index
    &option_IO_H1MC[13],              // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_H1MC_14_disp,              // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_H1MC_Func,               // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    DISP_IO_H1MC+14,                  // nr
    0,                                // index
    &option_IO_H1MC[14],              // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_H1MC_15_disp,              // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_H1MC_Func,               // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    DISP_IO_H1MC+15,                  // nr
    0,                                // index
    &option_IO_H1MC[15],              // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_H1MC_16_disp,              // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_H1MC_Func,               // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    DISP_IO_H1MC+16,                  // nr
    0,                                // index
    &option_IO_H1MC[16],              // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_H1MC_17_disp,              // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_H1MC_Func,               // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    DISP_IO_H1MC+17,                  // nr
    0,                                // index
    &option_IO_H1MC[17],              // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_H1MC_18_disp,              // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_H1MC_Func,               // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    DISP_IO_H1MC+18,                  // nr
    0,                                // index
    &option_IO_H1MC[18],              // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_H1MC_19_disp,              // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_H1MC_Func,               // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    DISP_IO_H1MC+19,                  // nr
    0,                                // index
    &option_IO_H1MC[19],              // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_H1MC_20_disp,              // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_H1MC_Func,               // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    DISP_IO_H1MC+20,                  // nr
    0,                                // index
    &option_IO_H1MC[20],              // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_H1MC_21_disp,              // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_H1MC_Func,               // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    DISP_IO_H1MC+21,                  // nr
    0,                                // index
    &option_IO_H1MC[21],              // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_H1MC_22_disp,              // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_H1MC_Func,               // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    DISP_IO_H1MC+22,                  // nr
    0,                                // index
    &option_IO_H1MC[22],              // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_H1MC_23_disp,              // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_H1MC_Func,               // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    DISP_IO_H1MC+23,                  // nr
    0,                                // index
    &option_IO_H1MC[23],              // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_H1MC_24_disp,              // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_H1MC_Func,               // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    DISP_IO_H1MC+24,                  // nr
    0,                                // index
    &option_IO_H1MC[24],              // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_H1MC_25_disp,              // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_H1MC_Func,               // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    DISP_IO_H1MC+25,                  // nr
    0,                                // index
    &option_IO_H1MC[25],              // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_H1MC_26_disp,              // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_H1MC_Func,               // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    DISP_IO_H1MC+26,                  // nr
    0,                                // index
    &option_IO_H1MC[26],              // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_H1MC_27_disp,              // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_H1MC_Func,               // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    DISP_IO_H1MC+27,                  // nr
    0,                                // index
    &option_IO_H1MC[27],              // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_H1MC_28_disp,              // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_H1MC_Func,               // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    DISP_IO_H1MC+28,                  // nr
    0,                                // index
    &option_IO_H1MC[28],              // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_H1MC_29_disp,              // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_H1MC_Func,               // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    DISP_IO_H1MC+29,                  // nr
    0,                                // index
    &option_IO_H1MC[29],              // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_H1MC_30_disp,              // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_H1MC_Func,               // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    DISP_IO_H1MC+30,                  // nr
    0,                                // index
    &option_IO_H1MC[30],              // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_H1MC_31_disp,              // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_H1MC_Func,               // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    DISP_IO_H1MC+31,                  // nr
    0,                                // index
    &option_IO_H1MC[31],              // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_H1MC_32_disp,              // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_H1MC_Func,               // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    DISP_IO_H1MC+32,                  // nr
    0,                                // index
    &option_IO_H1MC[32],              // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_H1MC_e_disp,               // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_H1MC_Empty_Func,         // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    DISP_IO_H2MC+0,                   // nr
    0,                                // index
    &option_IO_H2MC[0],               // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_H2MC_1_disp,               // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_H2MC_Func,               // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    DISP_IO_H2MC+1,                   // nr
    0,                                // index
    &option_IO_H2MC[1],               // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_H2MC_2_disp,               // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_H2MC_Func,               // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    DISP_IO_H2MC+2,                   // nr
    0,                                // index
    &option_IO_H2MC[2],               // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_H2MC_3_disp,               // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_H2MC_Func,               // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    DISP_IO_H2MC+3,                   // nr
    0,                                // index
    &option_IO_H2MC[3],               // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_H2MC_4_disp,               // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_H2MC_Func,               // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    DISP_IO_H2MC+4,                   // nr
    0,                                // index
    &option_IO_H2MC[4],               // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_H2MC_5_disp,               // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_H2MC_Func,               // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    DISP_IO_H2MC+5,                   // nr
    0,                                // index
    &option_IO_H2MC[5],               // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_H2MC_6_disp,               // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_H2MC_Func,               // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    DISP_IO_H2MC+6,                   // nr
    0,                                // index
    &option_IO_H2MC[6],               // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_H2MC_7_disp,               // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_H2MC_Func,               // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    DISP_IO_H2MC+7,                   // nr
    0,                                // index
    &option_IO_H2MC[7],               // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_H2MC_8_disp,               // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_H2MC_Func,               // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    DISP_IO_H2MC+8,                   // nr
    0,                                // index
    &option_IO_H2MC[8],               // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_H2MC_9_disp,               // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_H2MC_Func,               // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    DISP_IO_H2MC+9,                   // nr
    0,                                // index
    &option_IO_H2MC[9],               // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_H2MC_10_disp,              // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_H2MC_Func,               // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    DISP_IO_H2MC+10,                  // nr
    0,                                // index
    &option_IO_H2MC[10],              // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_H2MC_11_disp,              // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_H2MC_Func,               // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    DISP_IO_H2MC+11,                  // nr
    0,                                // index
    &option_IO_H2MC[11],              // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_H2MC_12_disp,              // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_H2MC_Func,               // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    DISP_IO_H2MC+12,                  // nr
    0,                                // index
    &option_IO_H2MC[12],              // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_H2MC_13_disp,              // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_H2MC_Func,               // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    DISP_IO_H2MC+13,                  // nr
    0,                                // index
    &option_IO_H2MC[13],              // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_H2MC_14_disp,              // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_H2MC_Func,               // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    DISP_IO_H2MC+14,                  // nr
    0,                                // index
    &option_IO_H2MC[14],              // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_H2MC_15_disp,              // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_H2MC_Func,               // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    DISP_IO_H2MC+15,                  // nr
    0,                                // index
    &option_IO_H2MC[15],              // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_H2MC_16_disp,              // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_H2MC_Func,               // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    DISP_IO_H2MC+16,                  // nr
    0,                                // index
    &option_IO_H2MC[16],              // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_H2MC_17_disp,              // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_H2MC_Func,               // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    DISP_IO_H2MC+17,                  // nr
    0,                                // index
    &option_IO_H2MC[17],              // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_H2MC_18_disp,              // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_H2MC_Func,               // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    DISP_IO_H2MC+18,                  // nr
    0,                                // index
    &option_IO_H2MC[18],              // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_H2MC_19_disp,              // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_H2MC_Func,               // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    DISP_IO_H2MC+19,                  // nr
    0,                                // index
    &option_IO_H2MC[19],              // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_H2MC_20_disp,              // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_H2MC_Func,               // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    DISP_IO_H2MC+20,                  // nr
    0,                                // index
    &option_IO_H2MC[20],              // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_H2MC_21_disp,              // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_H2MC_Func,               // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    DISP_IO_H2MC+21,                  // nr
    0,                                // index
    &option_IO_H2MC[21],              // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_H2MC_22_disp,              // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_H2MC_Func,               // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    DISP_IO_H2MC+22,                  // nr
    0,                                // index
    &option_IO_H2MC[22],              // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_H2MC_23_disp,              // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_H2MC_Func,               // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    DISP_IO_H2MC+23,                  // nr
    0,                                // index
    &option_IO_H2MC[23],              // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_H2MC_24_disp,              // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_H2MC_Func,               // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    DISP_IO_H2MC+24,                  // nr
    0,                                // index
    &option_IO_H2MC[24],              // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_H2MC_25_disp,              // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_H2MC_Func,               // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    DISP_IO_H2MC+25,                  // nr
    0,                                // index
    &option_IO_H2MC[25],              // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_H2MC_26_disp,              // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_H2MC_Func,               // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    DISP_IO_H2MC+26,                  // nr
    0,                                // index
    &option_IO_H2MC[26],              // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_H2MC_27_disp,              // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_H2MC_Func,               // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    DISP_IO_H2MC+27,                  // nr
    0,                                // index
    &option_IO_H2MC[27],              // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_H2MC_28_disp,              // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_H2MC_Func,               // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    DISP_IO_H2MC+28,                  // nr
    0,                                // index
    &option_IO_H2MC[28],              // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_H2MC_29_disp,              // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_H2MC_Func,               // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    DISP_IO_H2MC+29,                  // nr
    0,                                // index
    &option_IO_H2MC[29],              // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_H2MC_30_disp,              // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_H2MC_Func,               // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    DISP_IO_H2MC+30,                  // nr
    0,                                // index
    &option_IO_H2MC[30],              // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_H2MC_31_disp,              // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_H2MC_Func,               // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    DISP_IO_H2MC+31,                  // nr
    0,                                // index
    &option_IO_H2MC[31],              // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_H2MC_32_disp,              // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_H2MC_Func,               // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    DISP_IO_H2MC+32,                  // nr
    0,                                // index
    &option_IO_H2MC[32],              // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_H2MC_e_disp,               // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_H2MC_Empty_Func,         // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    DISP_IO_EKU+0,                    // nr
    0,                                // index
    &option_IO_EKU[0],                // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_EKU_1_disp,                // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_EKU_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    DISP_IO_EKU+1,                    // nr
    0,                                // index
    &option_IO_EKU[1],                // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_EKU_2_disp,                // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_EKU_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    DISP_IO_EKU+2,                    // nr
    0,                                // index
    &option_IO_EKU[2],                // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_EKU_3_disp,                // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_EKU_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    DISP_IO_EKU+3,                    // nr
    0,                                // index
    &option_IO_EKU[3],                // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_EKU_4_disp,                // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_EKU_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    DISP_IO_EKU+4,                    // nr
    0,                                // index
    &option_IO_EKU[4],                // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_EKU_5_disp,                // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_EKU_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    DISP_IO_EKU+5,                    // nr
    0,                                // index
    &option_IO_EKU[5],                // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_EKU_6_disp,                // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_EKU_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    DISP_IO_EKU+6,                    // nr
    0,                                // index
    &option_IO_EKU[6],                // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_EKU_7_disp,                // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_EKU_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    DISP_IO_EKU+7,                    // nr
    0,                                // index
    &option_IO_EKU[7],                // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_EKU_8_disp,                // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_EKU_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    DISP_IO_EKU+8,                    // nr
    0,                                // index
    &option_IO_EKU[8],                // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_EKU_9_disp,                // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_EKU_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    DISP_IO_EKU+9,                    // nr
    0,                                // index
    &option_IO_EKU[9],                // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_EKU_10_disp,               // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_EKU_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    DISP_IO_EKU+10,                   // nr
    0,                                // index
    &option_IO_EKU[10],               // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_EKU_11_disp,               // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_EKU_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    DISP_IO_EKU+11,                   // nr
    0,                                // index
    &option_IO_EKU[11],               // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_EKU_12_disp,               // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_EKU_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    DISP_IO_EKU+12,                   // nr
    0,                                // index
    &option_IO_EKU[12],               // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_EKU_13_disp,               // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_EKU_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    DISP_IO_EKU+13,                   // nr
    0,                                // index
    &option_IO_EKU[13],               // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_EKU_14_disp,               // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_EKU_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    DISP_IO_EKU+14,                   // nr
    0,                                // index
    &option_IO_EKU[14],               // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_EKU_15_disp,               // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_EKU_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    DISP_IO_EKU+15,                   // nr
    0,                                // index
    &option_IO_EKU[15],               // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_EKU_16_disp,               // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_EKU_Func,                // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
  {
    DISP_IO_EKU+16,                   // nr
    0,                                // index
    &option_IO_EKU[16],               // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_IO_EKU_e_disp,                // display
    0,                                // cursor
    &dummy_value,                     // *value
    Dummy_Func,                       // void (*number)(void); 
    Arrow_IO_EKU_Empty_Func,          // void (*arrow)(void); 
    Dummy_Func,                       // void (*enter)(void);
  },
};

s_screen screen_opt_IO_2;
s_screen const screen_opt_IO_2_default =
{
  0, // functie nr
  0, // index
  3, // max rel
  0, // rel actief
  0, // nr actief nummer weergegeven bin aantal ingeschakelde functie nummers
  0, // aantal ingeschakelde functie nummers
  lcd_disp_header, // algemene scherm opmaak
  {0,0,0,0,0,0}, // functies behorende bij functie toetsen
  0,3,19,       // rel[0]
  0,3,19+28,    // rel[1]
  0,3,19+28+28, // rel[2]
  &opt_IO_2_key_action[0], // first_action
  &opt_IO_2_key_action[sizeof(opt_IO_2_key_action)/sizeof(s_key_action) - 1], // last action 
  0, // change_flag
  0, // punt in normaal display
  0, // punt tijdens invoeren
  0, // aantal digits ingevoerd (punt telt niet mee)
  0, // value
  0, // min value
  0, // max value
  &screen_option_1,  // vorige scherm
  0  // prev_next_func
};

void Control_Screen_Option_IO_2(void)
{
int loop;
int cnt;

  cnt = 0;
  for (loop = 0; loop < IO_08_09_MAX; loop++)
  {
    if (opt_io.IO_08_09[loop].board_component.option & 0x000F)
    {
      option_IO_08_09[loop] = 1;
      cnt++;
    }
    else
      option_IO_08_09[loop] = 0;
  }
  option_IO_08_09[IO_08_09_MAX] = (cnt < IO_08_09_MAX) ? 1 : 0;
    
  cnt = 0;
  for (loop = 0; loop < IO_05_07_MAX; loop++)
  {
    if (opt_io.IO_05_07[loop].board_component.option & 0x000F)
    {
      option_IO_05_07[loop] = 1;
      cnt++;
    }
    else
      option_IO_05_07[loop] = 0;
  }
  option_IO_05_07[IO_05_07_MAX] = (cnt < IO_05_07_MAX) ? 1 : 0;
    
  cnt = 0;
  for (loop = 0; loop < IO_06_14_MAX; loop++)
  {
    if (opt_io.IO_06_14[loop].board_component.option & 0x000F)
    {
      option_IO_06_14[loop] = 1;
      cnt++;
    }
    else
      option_IO_06_14[loop] = 0;
  }
  option_IO_06_14[IO_06_14_MAX] = (cnt < IO_06_14_MAX) ? 1 : 0;

  cnt = 0;
  for (loop = 0; loop < IO_07_07_MAX; loop++)
  {
    if (opt_io.IO_07_07[loop].board_component.option & 0x000F)
    {
      option_IO_07_07[loop] = 1;
      cnt++;
    }
    else
      option_IO_07_07[loop] = 0;
  }
  option_IO_07_07[IO_07_07_MAX] = (cnt < IO_07_07_MAX) ? 1 : 0;

  cnt = 0;
  for (loop = 0; loop < IO_12_06_MAX; loop++)
  {
    if (opt_io.IO_12_06[loop].board_component.option & 0x000F)
    {
      option_IO_12_06[loop] = 1;
      cnt++;
    }
    else
      option_IO_12_06[loop] = 0;
  }
  option_IO_12_06[IO_12_06_MAX] = (cnt < IO_12_06_MAX) ? 1 : 0;

  cnt = 0;
  for (loop = 0; loop < IO_H1MC_MAX; loop++)
  {
    if (opt_io.IO_H1MC[loop].board_component.option & 0x000F)
    {
      option_IO_H1MC[loop] = 1;
      cnt++;
    }
    else
      option_IO_H1MC[loop] = 0;
  }
  option_IO_H1MC[IO_H1MC_MAX] = (cnt < IO_H1MC_MAX) ? 1 : 0;
    
  cnt = 0;
  for (loop = 0; loop < IO_H2MC_MAX; loop++)
  {
    if (opt_io.IO_H2MC[loop].board_component.option & 0x000F)
    {
      option_IO_H2MC[loop] = 1;
      cnt++;
    }
    else
      option_IO_H2MC[loop] = 0;
  }
  option_IO_H2MC[IO_H2MC_MAX] = (cnt < IO_H2MC_MAX) ? 1 : 0;
    
  cnt = 0;
  for (loop = 0; loop < IO_EKU_MAX; loop++)
  {
    if (opt_io.IO_EKU[loop].board_component.option & 0x000F)
    {
      option_IO_EKU[loop] = 1;
      cnt++;
    }
    else
      option_IO_EKU[loop] = 0;
  }
  option_IO_EKU[IO_EKU_MAX] = (cnt < IO_EKU_MAX) ? 1 : 0;
    
  Control_Screen(&screen_opt_IO_2, &screen_opt_IO_2_default, 1, 3);
}

//-----------------------------------------------------------------------------
static void IO_Prev_Screen(void)
{
  Prev_Screen();
  if (password_enabled & PASSWORD_ENABLED_OPT_MASK)
  {
    Install_Control_All_Board_IO();
  }
}
//-----------------------------------------------------------------------------
static void Arrow_IO_08_09_Func(void)
{
  switch (key)
  {
    case UP:
      Decrement_Func();
      break;
    case DOWN:
      Increment_Func();
      break;
    case LEFT:
      IO_Prev_Screen();
      break;
    case RIGHT:
      Control_Screen_Option_IO_08_09_3();
      Next_Screen(&screen_opt_IO_08_09_3);
      break;
  }
}

static void Arrow_IO_08_09_Empty_Func(void)
{
  switch (key)
  {
    case UP:
      Decrement_Func();
      break;
    case DOWN:
      Increment_Func();
      break;
    case LEFT:
      IO_Prev_Screen();
      break;
    case RIGHT:
      if (password_enabled & PASSWORD_ENABLED_OPT_MASK)
      {
        Control_Screen_Option_IO_08_09_3();
        Next_Screen(&screen_opt_IO_08_09_3);
      }
      break;
  }
}

//-----------------------------------------------------------------------------
static void Arrow_IO_05_07_Func(void)
{
  switch (key)
  {
    case UP:
      Decrement_Func();
      break;
    case DOWN:
      Increment_Func();
      break;
    case LEFT:
      IO_Prev_Screen();
      break;
    case RIGHT:
      Control_Screen_Option_IO_05_07_3();
      Next_Screen(&screen_opt_IO_05_07_3);
      break;
  }
}

static void Arrow_IO_05_07_Empty_Func(void)
{
  switch (key)
  {
    case UP:
      Decrement_Func();
      break;
    case DOWN:
      Increment_Func();
      break;
    case LEFT:
      IO_Prev_Screen();
      break;
    case RIGHT:
      if (password_enabled & PASSWORD_ENABLED_OPT_MASK)
      {
        Control_Screen_Option_IO_05_07_3();
        Next_Screen(&screen_opt_IO_05_07_3);
      }
      break;
  }
}

//-----------------------------------------------------------------------------
static void Arrow_IO_06_14_Func(void)
{
  switch (key)
  {
    case UP:
      Decrement_Func();
      break;
    case DOWN:
      Increment_Func();
      break;
    case LEFT:
      IO_Prev_Screen();
      break;
    case RIGHT:
      Control_Screen_Option_IO_06_14_3();
      Next_Screen(&screen_opt_IO_06_14_3);
      break;
  }
}

static void Arrow_IO_06_14_Empty_Func(void)
{
  switch (key)
  {
    case UP:
      Decrement_Func();
      break;
    case DOWN:
      Increment_Func();
      break;
    case LEFT:
      IO_Prev_Screen();
      break;
    case RIGHT:
      if (password_enabled & PASSWORD_ENABLED_OPT_MASK)
      {
        Control_Screen_Option_IO_06_14_3();
        Next_Screen(&screen_opt_IO_06_14_3);
      }
      break;
  }
}

//-----------------------------------------------------------------------------
static void Arrow_IO_07_07_Func(void)
{
  switch (key)
  {
    case UP:
      Decrement_Func();
      break;
    case DOWN:
      Increment_Func();
      break;
    case LEFT:
      IO_Prev_Screen();
      break;
    case RIGHT:
      Control_Screen_Option_IO_07_07_3();
      Next_Screen(&screen_opt_IO_07_07_3);
      break;
  }
}

static void Arrow_IO_07_07_Empty_Func(void)
{
  switch (key)
  {
    case UP:
      Decrement_Func();
      break;
    case DOWN:
      Increment_Func();
      break;
    case LEFT:
      IO_Prev_Screen();
      break;
    case RIGHT:
      if (password_enabled & PASSWORD_ENABLED_OPT_MASK)
      {
        Control_Screen_Option_IO_07_07_3();
        Next_Screen(&screen_opt_IO_07_07_3);
      }
      break;
  }
}

//-----------------------------------------------------------------------------
static void Arrow_IO_12_06_Func(void)
{
  switch (key)
  {
    case UP:
      Decrement_Func();
      break;
    case DOWN:
      Increment_Func();
      break;
    case LEFT:
      IO_Prev_Screen();
      break;
    case RIGHT:
      Control_Screen_Option_IO_12_06_3();
      Next_Screen(&screen_opt_IO_12_06_3);
      break;
  }
}

static void Arrow_IO_12_06_Empty_Func(void)
{
  switch (key)
  {
    case UP:
      Decrement_Func();
      break;
    case DOWN:
      Increment_Func();
      break;
    case LEFT:
      IO_Prev_Screen();
      break;
    case RIGHT:
      if (password_enabled & PASSWORD_ENABLED_OPT_MASK)
      {
        Control_Screen_Option_IO_12_06_3();
        Next_Screen(&screen_opt_IO_12_06_3);
      }
      break;
  }
}

//-----------------------------------------------------------------------------
static void Arrow_IO_H1MC_Func(void)
{
  switch (key)
  {
    case UP:
      Decrement_Func();
      break;
    case DOWN:
      Increment_Func();
      break;
    case LEFT:
      IO_Prev_Screen();
      break;
    case RIGHT:
      Control_Screen_Option_IO_H1MC_3();
      Next_Screen(&screen_opt_IO_H1MC_3);
      break;
  }
}

static void Arrow_IO_H1MC_Empty_Func(void)
{
  switch (key)
  {
    case UP:
      Decrement_Func();
      break;
    case DOWN:
      Increment_Func();
      break;
    case LEFT:
      IO_Prev_Screen();
      break;
    case RIGHT:
      if (password_enabled & PASSWORD_ENABLED_OPT_MASK)
      {
        Control_Screen_Option_IO_H1MC_3();
        Next_Screen(&screen_opt_IO_H1MC_3);
      }
      break;
  }
}

//-----------------------------------------------------------------------------
static void Arrow_IO_H2MC_Func(void)
{
  switch (key)
  {
    case UP:
      Decrement_Func();
      break;
    case DOWN:
      Increment_Func();
      break;
    case LEFT:
      IO_Prev_Screen();
      break;
    case RIGHT:
      Control_Screen_Option_IO_H2MC_3();
      Next_Screen(&screen_opt_IO_H2MC_3);
      break;
  }
}

static void Arrow_IO_H2MC_Empty_Func(void)
{
  switch (key)
  {
    case UP:
      Decrement_Func();
      break;
    case DOWN:
      Increment_Func();
      break;
    case LEFT:
      IO_Prev_Screen();
      break;
    case RIGHT:
      if (password_enabled & PASSWORD_ENABLED_OPT_MASK)
      {
        Control_Screen_Option_IO_H2MC_3();
        Next_Screen(&screen_opt_IO_H2MC_3);
      }
      break;
  }
}

//-----------------------------------------------------------------------------
static void Arrow_IO_EKU_Func(void)
{
  switch (key)
  {
    case UP:
      Decrement_Func();
      break;
    case DOWN:
      Increment_Func();
      break;
    case LEFT:
      IO_Prev_Screen();
      break;
    case RIGHT:
      Control_Screen_Option_IO_EKU_3();
      Next_Screen(&screen_opt_IO_EKU_3);
      break;
  }
}

static void Arrow_IO_EKU_Empty_Func(void)
{
  switch (key)
  {
    case UP:
      Decrement_Func();
      break;
    case DOWN:
      Increment_Func();
      break;
    case LEFT:
      IO_Prev_Screen();
      break;
    case RIGHT:
      if (password_enabled & PASSWORD_ENABLED_OPT_MASK)
      {
        Control_Screen_Option_IO_EKU_3();
        Next_Screen(&screen_opt_IO_EKU_3);
      }
      break;
  }
}


