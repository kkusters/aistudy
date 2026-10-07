// C__DISP_OPT_IO_SELECT.C

#include <string.h> 

#include "ch_define.h"
#include "ch_alarm.h"
#include "ch_const.h"
#include "ch_disp.h"
#include "ch_disp_func.h"
#include "ch_disp_option_1.h"
#include "ch_disp_password.h"
#include "ch_disp_value.h"
#include "ch_key.h"
#include "ch_kiersturing.h"
#include "ch_lcd_10.h"
#include "ch_lcd_bitmap.h"
#include "ch_lcd_pixel.h"
#include "ch_luchtmengkast.h"
#include "ch_motor.h"
#include "ch_string.h"
#include "ch_disp_opt_IO_select.h"

s_tekst_50 tekst_board_14 = { 50, 0, SIZE_14, "" };
int board_IO_max = 0;        // aantal IO waaruit gekozen kan worden
int board_IO_max_nr = 0;     // maximum aantal toe te wijzen IO
int board_IO_array_size = 0; // array grootte
int board_IO_on_off = 0;     // 1 = IO on off; 0 = IO nummers
unsigned char board_IO_type;
unsigned char board_IO_type_sel;
s_board_IO_on_off *board_IO_ptr;
s_board_IO_on_off const board_IO_empty = { 0,0,16,0 };
s_board_IO_on_off board_IO[16];
unsigned char (*IONotUsedFunc)(s_board_IO_on_off IO_new);

s_bitmap const * const bmp_IO_boxes[17] = { &ico_box_1, &ico_box_2,  &ico_box_3,  &ico_box_4,  &ico_box_5,  &ico_box_6,  &ico_box_7,  &ico_box_8,
                                            &ico_box_9, &ico_box_10, &ico_box_11, &ico_box_12, &ico_box_13, &ico_box_14, &ico_box_15, &ico_box_16,
                                            &ico_empty, };

s_disp_bitmap_array const disp_box_IO_pos1_bmp =  { Disp_Draw_Bitmap_Array,   0, 28, bmp_IO_boxes, UCHAR, &board_IO[ 0].IO_nr, 17 };
s_disp_bitmap_array const disp_box_IO_pos2_bmp =  { Disp_Draw_Bitmap_Array,  19, 28, bmp_IO_boxes, UCHAR, &board_IO[ 1].IO_nr, 17 };
s_disp_bitmap_array const disp_box_IO_pos3_bmp =  { Disp_Draw_Bitmap_Array,  38, 28, bmp_IO_boxes, UCHAR, &board_IO[ 2].IO_nr, 17 };
s_disp_bitmap_array const disp_box_IO_pos4_bmp =  { Disp_Draw_Bitmap_Array,  57, 28, bmp_IO_boxes, UCHAR, &board_IO[ 3].IO_nr, 17 };
s_disp_bitmap_array const disp_box_IO_pos5_bmp =  { Disp_Draw_Bitmap_Array,  76, 28, bmp_IO_boxes, UCHAR, &board_IO[ 4].IO_nr, 17 };
s_disp_bitmap_array const disp_box_IO_pos6_bmp =  { Disp_Draw_Bitmap_Array,  95, 28, bmp_IO_boxes, UCHAR, &board_IO[ 5].IO_nr, 17 };
s_disp_bitmap_array const disp_box_IO_pos7_bmp =  { Disp_Draw_Bitmap_Array, 114, 28, bmp_IO_boxes, UCHAR, &board_IO[ 6].IO_nr, 17 };
s_disp_bitmap_array const disp_box_IO_pos8_bmp =  { Disp_Draw_Bitmap_Array, 133, 28, bmp_IO_boxes, UCHAR, &board_IO[ 7].IO_nr, 17 };
s_disp_bitmap_array const disp_box_IO_pos9_bmp =  { Disp_Draw_Bitmap_Array, 152, 28, bmp_IO_boxes, UCHAR, &board_IO[ 8].IO_nr, 17 };
s_disp_bitmap_array const disp_box_IO_pos10_bmp = { Disp_Draw_Bitmap_Array, 171, 28, bmp_IO_boxes, UCHAR, &board_IO[ 9].IO_nr, 17 };
s_disp_bitmap_array const disp_box_IO_pos11_bmp = { Disp_Draw_Bitmap_Array, 190, 28, bmp_IO_boxes, UCHAR, &board_IO[10].IO_nr, 17 };
s_disp_bitmap_array const disp_box_IO_pos12_bmp = { Disp_Draw_Bitmap_Array, 209, 28, bmp_IO_boxes, UCHAR, &board_IO[11].IO_nr, 17 };
s_disp_bitmap_array const disp_box_IO_pos13_bmp = { Disp_Draw_Bitmap_Array,   0, 55, bmp_IO_boxes, UCHAR, &board_IO[12].IO_nr, 17 };
s_disp_bitmap_array const disp_box_IO_pos14_bmp = { Disp_Draw_Bitmap_Array,  19, 55, bmp_IO_boxes, UCHAR, &board_IO[13].IO_nr, 17 };
s_disp_bitmap_array const disp_box_IO_pos15_bmp = { Disp_Draw_Bitmap_Array,  38, 55, bmp_IO_boxes, UCHAR, &board_IO[14].IO_nr, 17 };
s_disp_bitmap_array const disp_box_IO_pos16_bmp = { Disp_Draw_Bitmap_Array,  57, 55, bmp_IO_boxes, UCHAR, &board_IO[15].IO_nr, 17 };
                                                                                                   
s_bitmap const * const bmp_IO_vink[2] = { &ico_empty, &ico_vink };
s_disp_bitmap_array const disp_IO_vink_pos1_bmp =  { Disp_Draw_Bitmap_Array,  4, 40, bmp_IO_vink, UCHAR, &board_IO[ 0].on_off, 2 };
s_disp_bitmap_array const disp_IO_vink_pos2_bmp =  { Disp_Draw_Bitmap_Array, 23, 40, bmp_IO_vink, UCHAR, &board_IO[ 1].on_off, 2 };
s_disp_bitmap_array const disp_IO_vink_pos3_bmp =  { Disp_Draw_Bitmap_Array, 42, 40, bmp_IO_vink, UCHAR, &board_IO[ 2].on_off, 2 };
s_disp_bitmap_array const disp_IO_vink_pos4_bmp =  { Disp_Draw_Bitmap_Array, 61, 40, bmp_IO_vink, UCHAR, &board_IO[ 3].on_off, 2 };
s_disp_bitmap_array const disp_IO_vink_pos5_bmp =  { Disp_Draw_Bitmap_Array, 80, 40, bmp_IO_vink, UCHAR, &board_IO[ 4].on_off, 2 };
s_disp_bitmap_array const disp_IO_vink_pos6_bmp =  { Disp_Draw_Bitmap_Array, 99, 40, bmp_IO_vink, UCHAR, &board_IO[ 5].on_off, 2 };
s_disp_bitmap_array const disp_IO_vink_pos7_bmp =  { Disp_Draw_Bitmap_Array,118, 40, bmp_IO_vink, UCHAR, &board_IO[ 6].on_off, 2 };
s_disp_bitmap_array const disp_IO_vink_pos8_bmp =  { Disp_Draw_Bitmap_Array,137, 40, bmp_IO_vink, UCHAR, &board_IO[ 7].on_off, 2 };
s_disp_bitmap_array const disp_IO_vink_pos9_bmp =  { Disp_Draw_Bitmap_Array,156, 40, bmp_IO_vink, UCHAR, &board_IO[ 8].on_off, 2 };
s_disp_bitmap_array const disp_IO_vink_pos10_bmp = { Disp_Draw_Bitmap_Array,175, 40, bmp_IO_vink, UCHAR, &board_IO[ 9].on_off, 2 };
s_disp_bitmap_array const disp_IO_vink_pos11_bmp = { Disp_Draw_Bitmap_Array,194, 40, bmp_IO_vink, UCHAR, &board_IO[10].on_off, 2 };
s_disp_bitmap_array const disp_IO_vink_pos12_bmp = { Disp_Draw_Bitmap_Array,213, 40, bmp_IO_vink, UCHAR, &board_IO[11].on_off, 2 };
s_disp_bitmap_array const disp_IO_vink_pos13_bmp = { Disp_Draw_Bitmap_Array,  4, 67, bmp_IO_vink, UCHAR, &board_IO[12].on_off, 2 };
s_disp_bitmap_array const disp_IO_vink_pos14_bmp = { Disp_Draw_Bitmap_Array, 23, 67, bmp_IO_vink, UCHAR, &board_IO[13].on_off, 2 };
s_disp_bitmap_array const disp_IO_vink_pos15_bmp = { Disp_Draw_Bitmap_Array, 42, 67, bmp_IO_vink, UCHAR, &board_IO[14].on_off, 2 };
s_disp_bitmap_array const disp_IO_vink_pos16_bmp = { Disp_Draw_Bitmap_Array, 61, 67, bmp_IO_vink, UCHAR, &board_IO[15].on_off, 2 };
/*
s_disp_value_option_on const disp_IO_nr_pos1 =  { Disp_Draw_Value_Option_On, 15, 52, (SIZE_10 | RECHTS), UCHAR, 0, &board_IO[ 0].on_off, UCHAR, &board_IO[ 0].on_off };
s_disp_value_option_on const disp_IO_nr_pos2 =  { Disp_Draw_Value_Option_On, 34, 52, (SIZE_10 | RECHTS), UCHAR, 0, &board_IO[ 1].on_off, UCHAR, &board_IO[ 1].on_off };
s_disp_value_option_on const disp_IO_nr_pos3 =  { Disp_Draw_Value_Option_On, 53, 52, (SIZE_10 | RECHTS), UCHAR, 0, &board_IO[ 2].on_off, UCHAR, &board_IO[ 2].on_off };
s_disp_value_option_on const disp_IO_nr_pos4 =  { Disp_Draw_Value_Option_On, 72, 52, (SIZE_10 | RECHTS), UCHAR, 0, &board_IO[ 3].on_off, UCHAR, &board_IO[ 3].on_off };
s_disp_value_option_on const disp_IO_nr_pos5 =  { Disp_Draw_Value_Option_On, 91, 52, (SIZE_10 | RECHTS), UCHAR, 0, &board_IO[ 4].on_off, UCHAR, &board_IO[ 4].on_off };
s_disp_value_option_on const disp_IO_nr_pos6 =  { Disp_Draw_Value_Option_On,110, 52, (SIZE_10 | RECHTS), UCHAR, 0, &board_IO[ 5].on_off, UCHAR, &board_IO[ 5].on_off };
s_disp_value_option_on const disp_IO_nr_pos7 =  { Disp_Draw_Value_Option_On,129, 52, (SIZE_10 | RECHTS), UCHAR, 0, &board_IO[ 6].on_off, UCHAR, &board_IO[ 6].on_off };
s_disp_value_option_on const disp_IO_nr_pos8 =  { Disp_Draw_Value_Option_On,148, 52, (SIZE_10 | RECHTS), UCHAR, 0, &board_IO[ 7].on_off, UCHAR, &board_IO[ 7].on_off };
s_disp_value_option_on const disp_IO_nr_pos9 =  { Disp_Draw_Value_Option_On,167, 52, (SIZE_10 | RECHTS), UCHAR, 0, &board_IO[ 8].on_off, UCHAR, &board_IO[ 8].on_off };
s_disp_value_option_on const disp_IO_nr_pos10 = { Disp_Draw_Value_Option_On,186, 52, (SIZE_10 | RECHTS), UCHAR, 0, &board_IO[ 9].on_off, UCHAR, &board_IO[ 9].on_off };
s_disp_value_option_on const disp_IO_nr_pos11 = { Disp_Draw_Value_Option_On,205, 52, (SIZE_10 | RECHTS), UCHAR, 0, &board_IO[10].on_off, UCHAR, &board_IO[10].on_off };
s_disp_value_option_on const disp_IO_nr_pos12 = { Disp_Draw_Value_Option_On,224, 52, (SIZE_10 | RECHTS), UCHAR, 0, &board_IO[11].on_off, UCHAR, &board_IO[11].on_off };
s_disp_value_option_on const disp_IO_nr_pos13 = { Disp_Draw_Value_Option_On, 15, 79, (SIZE_10 | RECHTS), UCHAR, 0, &board_IO[12].on_off, UCHAR, &board_IO[12].on_off };
s_disp_value_option_on const disp_IO_nr_pos14 = { Disp_Draw_Value_Option_On, 34, 79, (SIZE_10 | RECHTS), UCHAR, 0, &board_IO[13].on_off, UCHAR, &board_IO[13].on_off };
s_disp_value_option_on const disp_IO_nr_pos15 = { Disp_Draw_Value_Option_On, 53, 79, (SIZE_10 | RECHTS), UCHAR, 0, &board_IO[14].on_off, UCHAR, &board_IO[14].on_off };
s_disp_value_option_on const disp_IO_nr_pos16 = { Disp_Draw_Value_Option_On, 72, 79, (SIZE_10 | RECHTS), UCHAR, 0, &board_IO[15].on_off, UCHAR, &board_IO[15].on_off };
*/
s_disp_value_2_size_option_on const disp_IO_nr_pos1  = { Disp_Draw_Value_2_Size_Option_On, 15, 52, (SIZE_10 | RECHTS), (SIZE_7 | RECHTS), UCHAR, 0, &board_IO[ 0].on_off, 3, UCHAR, &board_IO[ 0].on_off };
s_disp_value_2_size_option_on const disp_IO_nr_pos2  = { Disp_Draw_Value_2_Size_Option_On, 34, 52, (SIZE_10 | RECHTS), (SIZE_7 | RECHTS), UCHAR, 0, &board_IO[ 1].on_off, 3, UCHAR, &board_IO[ 1].on_off };
s_disp_value_2_size_option_on const disp_IO_nr_pos3  = { Disp_Draw_Value_2_Size_Option_On, 53, 52, (SIZE_10 | RECHTS), (SIZE_7 | RECHTS), UCHAR, 0, &board_IO[ 2].on_off, 3, UCHAR, &board_IO[ 2].on_off };
s_disp_value_2_size_option_on const disp_IO_nr_pos4  = { Disp_Draw_Value_2_Size_Option_On, 72, 52, (SIZE_10 | RECHTS), (SIZE_7 | RECHTS), UCHAR, 0, &board_IO[ 3].on_off, 3, UCHAR, &board_IO[ 3].on_off };
s_disp_value_2_size_option_on const disp_IO_nr_pos5  = { Disp_Draw_Value_2_Size_Option_On, 91, 52, (SIZE_10 | RECHTS), (SIZE_7 | RECHTS), UCHAR, 0, &board_IO[ 4].on_off, 3, UCHAR, &board_IO[ 4].on_off };
s_disp_value_2_size_option_on const disp_IO_nr_pos6  = { Disp_Draw_Value_2_Size_Option_On,110, 52, (SIZE_10 | RECHTS), (SIZE_7 | RECHTS), UCHAR, 0, &board_IO[ 5].on_off, 3, UCHAR, &board_IO[ 5].on_off };
s_disp_value_2_size_option_on const disp_IO_nr_pos7  = { Disp_Draw_Value_2_Size_Option_On,129, 52, (SIZE_10 | RECHTS), (SIZE_7 | RECHTS), UCHAR, 0, &board_IO[ 6].on_off, 3, UCHAR, &board_IO[ 6].on_off };
s_disp_value_2_size_option_on const disp_IO_nr_pos8  = { Disp_Draw_Value_2_Size_Option_On,148, 52, (SIZE_10 | RECHTS), (SIZE_7 | RECHTS), UCHAR, 0, &board_IO[ 7].on_off, 3, UCHAR, &board_IO[ 7].on_off };
s_disp_value_2_size_option_on const disp_IO_nr_pos9  = { Disp_Draw_Value_2_Size_Option_On,167, 52, (SIZE_10 | RECHTS), (SIZE_7 | RECHTS), UCHAR, 0, &board_IO[ 8].on_off, 3, UCHAR, &board_IO[ 8].on_off };
s_disp_value_2_size_option_on const disp_IO_nr_pos10 = { Disp_Draw_Value_2_Size_Option_On,186, 52, (SIZE_10 | RECHTS), (SIZE_7 | RECHTS), UCHAR, 0, &board_IO[ 9].on_off, 3, UCHAR, &board_IO[ 9].on_off };
s_disp_value_2_size_option_on const disp_IO_nr_pos11 = { Disp_Draw_Value_2_Size_Option_On,205, 52, (SIZE_10 | RECHTS), (SIZE_7 | RECHTS), UCHAR, 0, &board_IO[10].on_off, 3, UCHAR, &board_IO[10].on_off };
s_disp_value_2_size_option_on const disp_IO_nr_pos12 = { Disp_Draw_Value_2_Size_Option_On,224, 52, (SIZE_10 | RECHTS), (SIZE_7 | RECHTS), UCHAR, 0, &board_IO[11].on_off, 3, UCHAR, &board_IO[11].on_off };
s_disp_value_2_size_option_on const disp_IO_nr_pos13 = { Disp_Draw_Value_2_Size_Option_On, 15, 79, (SIZE_10 | RECHTS), (SIZE_7 | RECHTS), UCHAR, 0, &board_IO[12].on_off, 3, UCHAR, &board_IO[12].on_off };
s_disp_value_2_size_option_on const disp_IO_nr_pos14 = { Disp_Draw_Value_2_Size_Option_On, 34, 79, (SIZE_10 | RECHTS), (SIZE_7 | RECHTS), UCHAR, 0, &board_IO[13].on_off, 3, UCHAR, &board_IO[13].on_off };
s_disp_value_2_size_option_on const disp_IO_nr_pos15 = { Disp_Draw_Value_2_Size_Option_On, 53, 79, (SIZE_10 | RECHTS), (SIZE_7 | RECHTS), UCHAR, 0, &board_IO[14].on_off, 3, UCHAR, &board_IO[14].on_off };
s_disp_value_2_size_option_on const disp_IO_nr_pos16 = { Disp_Draw_Value_2_Size_Option_On, 72, 79, (SIZE_10 | RECHTS), (SIZE_7 | RECHTS), UCHAR, 0, &board_IO[15].on_off, 3, UCHAR, &board_IO[15].on_off };

s_disp_tekst const disp_board_str = { Disp_Draw_Tekst_L,  95, 75, &tekst_board_14 };
static void const * const tekst_niet_toegewezen_leeg[] = { &tekst_inst.Niet_Toegewezen_14, &tekst_leeg };
s_disp_tekst_array const disp_NO_IO = { Disp_Draw_Tekst_Array_L, 47, 50, &tekst_niet_toegewezen_leeg, UCHAR, &board_IO_max, 2 };

//-----------------------------------------------------------------------------
typedef struct
{
  unsigned char IOType;
  unsigned char IOMax;
  unsigned int  IOSize;
  unsigned char *OptType;
} TIOSpecs;

typedef struct
{
  unsigned char BoardType;
  unsigned char BoardMax;
  unsigned int  BoardSize;
  unsigned int *Option;
  TIOSpecs const *IOSpecs;
} TBoardSpecs;

TIOSpecs const IO_06_14_IOSpecs[] =
{
  { ANALOG_INPUT_ID,   IO_06_14_ANALOG_INPUT,   sizeof(s_option_analog_input),  &opt_io.IO_06_14[0].analog_input[0].opt_type   },
  { DIGITAL_INPUT_ID,  IO_06_14_DIGITAL_INPUT,  sizeof(s_option_digital_input), &opt_io.IO_06_14[0].digital_input[0].opt_type  },
  { ANALOG_OUTPUT_ID,  IO_06_14_ANALOG_OUTPUT,  sizeof(s_option_analog_output), &opt_io.IO_06_14[0].analog_output.opt_type     },
  { DIGITAL_OUTPUT_ID, IO_06_14_DIGITAL_OUTPUT, sizeof(unsigned char),          &opt_io.IO_06_14[0].digital_output.opt_type[0] },
  NULL
};

TIOSpecs const IO_12_06_IOSpecs[] =
{
  { DIGITAL_INPUT_ID,  IO_12_06_DIGITAL_INPUT,  sizeof(s_option_digital_input), &opt_io.IO_12_06[0].digital_input[0].opt_type  },
  { ANALOG_OUTPUT_ID,  IO_12_06_ANALOG_OUTPUT,  sizeof(s_option_analog_output), &opt_io.IO_12_06[0].analog_output[0].opt_type  },
  { DIGITAL_OUTPUT_ID, IO_12_06_DIGITAL_OUTPUT, sizeof(unsigned char),          &opt_io.IO_12_06[0].digital_output.opt_type[0] },
  NULL
};

TIOSpecs const IO_08_09_IOSpecs[] =
{
  { DIGITAL_INPUT_ID,  IO_08_09_DIGITAL_INPUT,  sizeof(s_option_digital_input), &opt_io.IO_08_09[0].digital_input[0].opt_type  },
  { ANALOG_OUTPUT_ID,  IO_08_09_ANALOG_OUTPUT,  sizeof(s_option_analog_output), &opt_io.IO_08_09[0].analog_output[0].opt_type  },
  { DIGITAL_OUTPUT_ID, IO_08_09_DIGITAL_OUTPUT, sizeof(unsigned char),          &opt_io.IO_08_09[0].digital_output.opt_type[0] },
  NULL
};

TIOSpecs const IO_EKU_IOSpecs[] =
{
  { MOTOR_CONTROL_ID,  IO_EKU_MOTOR_CONTROL,  sizeof(s_option_motor_control), &opt_io.IO_EKU[0].motor_control.OptType },
  NULL
};

TIOSpecs const IO_H2MC_IOSpecs[] =
{
  { MOTOR_CONTROL_ID,  IO_H2MC_MOTOR_CONTROL,  sizeof(s_option_motor_control), &opt_io.IO_H2MC[0].motor_control[0].OptType },
  NULL
};

TIOSpecs const IO_H1MC_IOSpecs[] =
{
  { MOTOR_CONTROL_ID,  IO_H1MC_MOTOR_CONTROL,  sizeof(s_option_motor_control), &opt_io.IO_H1MC[0].motor_control.OptType },
  NULL
};

TIOSpecs const IO_05_07_IOSpecs[] =
{
  { ANALOG_INPUT_ID,   IO_05_07_ANALOG_INPUT,   sizeof(s_option_analog_input),  &opt_io.IO_05_07[0].analog_input[0].opt_type          },
  { ANALOG_OUTPUT_ID,  IO_05_07_ANALOG_OUTPUT,  sizeof(s_option_analog_output), &opt_io.IO_05_07[0].analog_output[0].opt_type         },
  { DIGITAL_OUTPUT_ID, IO_05_07_DIGITAL_OUTPUT, sizeof(unsigned char),          &opt_io.IO_05_07[0].digital_output.opt_type[0]        },
  { RS485_BUS_ID,      IO_05_07_RS485_BUS,      sizeof(s_option_RS485_bus),     (unsigned char *)&opt_io.IO_05_07[0].RS485_bus.Option },
  NULL
};

TIOSpecs const IO_07_07_IOSpecs[] =
{
  { ANALOG_INPUT_ID,   IO_07_07_ANALOG_INPUT,   sizeof(s_option_analog_input),  &opt_io.IO_07_07[0].analog_input[0].opt_type          },
  { DIGITAL_INPUT_ID,  IO_07_07_DIGITAL_INPUT,  sizeof(s_option_digital_input), &opt_io.IO_07_07[0].digital_input[0].opt_type         },
  { ANALOG_OUTPUT_ID,  IO_07_07_ANALOG_OUTPUT,  sizeof(s_option_analog_output), &opt_io.IO_07_07[0].analog_output[0].opt_type         },
  { DIGITAL_OUTPUT_ID, IO_07_07_DIGITAL_OUTPUT, sizeof(unsigned char),          &opt_io.IO_07_07[0].digital_output.opt_type[0]        },
  { RS485_BUS_ID,      IO_07_07_RS485_BUS,      sizeof(s_option_RS485_bus),     (unsigned char *)&opt_io.IO_07_07[0].RS485_bus.Option },
  NULL
};

TBoardSpecs const IO_06_14_BoardSpecs = { IO_06_14_ID, IO_06_14_MAX, sizeof(s_option_IO_06_14), &opt_io.IO_06_14[0].board_component.option, IO_06_14_IOSpecs };
TBoardSpecs const IO_12_06_BoardSpecs = { IO_12_06_ID, IO_12_06_MAX, sizeof(s_option_IO_12_06), &opt_io.IO_12_06[0].board_component.option, IO_12_06_IOSpecs };
TBoardSpecs const IO_08_09_BoardSpecs = { IO_08_09_ID, IO_08_09_MAX, sizeof(s_option_IO_08_09), &opt_io.IO_08_09[0].board_component.option, IO_08_09_IOSpecs };
TBoardSpecs const IO_EKU_BoardSpecs   = { IO_EKU_ID,   IO_EKU_MAX,   sizeof(s_option_IO_EKU),   &opt_io.IO_EKU[0].board_component.option,   IO_EKU_IOSpecs   };
TBoardSpecs const IO_H2MC_BoardSpecs  = { IO_H2MC_ID,  IO_H2MC_MAX,  sizeof(s_option_IO_H2MC),  &opt_io.IO_H2MC[0].board_component.option,  IO_H2MC_IOSpecs  };
TBoardSpecs const IO_H1MC_BoardSpecs  = { IO_H1MC_ID,  IO_H1MC_MAX,  sizeof(s_option_IO_H1MC),  &opt_io.IO_H1MC[0].board_component.option,  IO_H1MC_IOSpecs  };
TBoardSpecs const IO_05_07_BoardSpecs = { IO_05_07_ID, IO_05_07_MAX, sizeof(s_option_IO_05_07), &opt_io.IO_05_07[0].board_component.option, IO_05_07_IOSpecs };
TBoardSpecs const IO_07_07_BoardSpecs = { IO_07_07_ID, IO_07_07_MAX, sizeof(s_option_IO_07_07), &opt_io.IO_07_07[0].board_component.option, IO_07_07_IOSpecs };

TBoardSpecs const *BoardSpecs[] =
{
  &IO_06_14_BoardSpecs,
  &IO_12_06_BoardSpecs,
  &IO_08_09_BoardSpecs,
  &IO_EKU_BoardSpecs,
  &IO_H2MC_BoardSpecs,
  &IO_H1MC_BoardSpecs,
  &IO_05_07_BoardSpecs,
  &IO_07_07_BoardSpecs,
  0
};

//-----------------------------------------------------------------------------
void Get_Next_Board_IO(s_board_IO_on_off IO,
                       s_board_IO_on_off *IO_next,
                       int up_down,
                       unsigned char refresh,
                       unsigned char IO_type,
                       unsigned char IO_type_sel_first,
                       unsigned char IO_type_sel_last,
                       unsigned char (*NotUsedFunc)(s_board_IO_on_off IO_new))
// s_board_IO_on_off IO     : huidige IO
// s_board_IO_on_off IO_next: volgende IO
// int up_down              : zoekrichting [1 = vooruit zoeken, -1 = achteruit zoeken]
// unsigned char refresh    : IO_next wordt vergeleken met IO en on_off bit wordt gezet als deze gelijk zijn
// unsigned char IO_type    : ANALOG_INPUT_ID enz
// unsigned char IO_type_sel: bv analoge ingangen ANA_IN_TEMP, ANA_IN_PA, ANA_IN_RV enz
// unsigned char NotUsedFunc: retourneert '0' als IO al gebruikt wordt en '1' als deze nog niet gebruikt wordt
{
unsigned int  Option;
unsigned char OptType;
s_board_IO_on_off IO_new = {0,0,0,0};
unsigned char IO_on_off;
unsigned char IO_ok;
TBoardSpecs const **Board;
TIOSpecs const *IOSpec;
int BoardNr;
int index;

  Board = BoardSpecs;
  if (IO.board_type != 0)
  {
    index = up_down + IO.IO_nr;
    BoardNr = IO.board_nr;
    while (((*Board)->BoardType != NULL) && ((*Board)->BoardType != IO.board_type))
      Board++;
  }
  else
  {
    index = 0;
    BoardNr = 0;
  }
  while ((Board >= BoardSpecs) && ((*Board) != NULL))
  {
    while ((BoardNr >= 0) && (BoardNr < (*Board)->BoardMax))
    {
	  Option = *((unsigned int *)((unsigned long)(*Board)->Option + (BoardNr * (*Board)->BoardSize)));
      if ((Option & 0x000F) != 0)
      {
        IOSpec = (*Board)->IOSpecs;
        while ((IOSpec->IOType != 0) && (IOSpec->IOType != IO_type))
          IOSpec++;
        if (IOSpec->IOType != 0)
        {
          while ((index >= 0) && (index < IOSpec->IOMax))
          {
	        OptType = *((unsigned char *)((unsigned long)IOSpec->OptType + (BoardNr * (*Board)->BoardSize) + (index * IOSpec->IOSize)));
			IO_ok = 0;
			switch (IO_type) // switch for special IO's
			{
              case RS485_BUS_ID:
                if (OptType & 0x0F)
				  IO_ok = 1;
			    break;
			  default:
                if ((OptType >= IO_type_sel_first) && (OptType <= IO_type_sel_last))
				  IO_ok = 1;
			    break;
			}
            if (IO_ok)
            {
              IO_new.board_type = (*Board)->BoardType;
              IO_new.board_nr = BoardNr;
              IO_new.IO_nr = index;
              IO_on_off = Board_IO_On_Off(IO_new, board_IO_ptr, board_IO_array_size);
              if (refresh)
                IO_new.on_off = IO_on_off;
              else
                IO_new.on_off = 0;
              if (IO_on_off || NotUsedFunc(IO_new))
              {
                *IO_next = IO_new;
                return;
              }
            }
            index += up_down;
          }
          if (up_down == 1)
            index = 0;
          else
            index = IOSpec->IOMax - 1;
        }
      }
      BoardNr += up_down;
    }
    if (up_down == 1)
    {
      Board++;
      BoardNr = 0;
      index = 0;
    }
    else
    {
      Board--;
	  if (Board >= BoardSpecs)
	  {
        BoardNr = (*Board)->BoardMax - 1;
        IOSpec = (*Board)->IOSpecs;
        while ((IOSpec->IOType != 0) && (IOSpec->IOType != IO_type))
          IOSpec++;
        if (IOSpec != 0)
		  index = IOSpec->IOMax - 1;
		else
		  index = 0;
	  }
    }
  }
  *IO_next = IO_empty;
}                             

//-----------------------------------------------------------------------------
void Install_Fill_Board_IO(unsigned char max,
                           unsigned char IO_type,
                           unsigned char IO_type_sel_first,
                           unsigned char IO_type_sel_last,
                           unsigned char (*NotUsedFunc)(s_board_IO_on_off IO_new))
// unsigned char max        : maximum aantal IO
// unsigned char IO_type    : ANALOG_INPUT_ID enz
// unsigned char IO_type_sel: bv analoge ingangen ANA_IN_TEMP, ANA_IN_PA, ANA_IN_RV enz
// unsigned char NotUsedFunc: retourneert '0' als IO al gebruikt wordt en '1' als deze nog niet gebruikt wordt
{
s_board_IO_on_off IO_new = {0,0,0,0};
int index;
int i;

  for (index = 0; index < 16; index++)
    board_IO[index] = board_IO_empty;

  index = 0;
  Get_Next_Board_IO(IO_new, &IO_new, 1, 1, IO_type, IO_type_sel_first, IO_type_sel_last, NotUsedFunc);
  while (IO_new.board_type != 0)
  {
    if (index < max)
    {
      board_IO[index] = IO_new;
      index++;
    }
    else if (IO_new.on_off != 0)
    {
      for (i = (max - 1); i >= 0; i--)
      {
        if (board_IO[i].on_off == 0)
        {
          for (i; i < (max - 1); i++)
            board_IO[i] = board_IO[i+1];
          board_IO[i] = IO_new;
          break;
        }
      }
    }
    Get_Next_Board_IO(IO_new, &IO_new, 1, 1, IO_type, IO_type_sel_first, IO_type_sel_last, NotUsedFunc);
  }
  board_IO_max = index;
}

unsigned char Board_IO_Used(s_board_IO_on_off IO_new, s_board_IO_on_off *IO_array, unsigned char array_size)
{
int index;

  for (index = 0; index < array_size; index++)
  {
    if ((IO_new.board_type == IO_array[index].board_type) &&
        (IO_new.board_nr   == IO_array[index].board_nr) &&
        (IO_new.IO_nr      == IO_array[index].IO_nr))
      return (1);
  }
  return (0);
}

unsigned char Board_IO_On_Off(s_board_IO_on_off IO_new, s_board_IO_on_off *IO_array, unsigned char array_size)
// voegt gegevens board_IO samen met gegevens van voelers
{
int index = 0;

  for (index = 0; index < array_size; index++)
  {
    if ((IO_new.board_type == IO_array[index].board_type) &&
        (IO_new.board_nr   == IO_array[index].board_nr) &&
        (IO_new.IO_nr      == IO_array[index].IO_nr))
    {
      return (IO_array[index].on_off);
    }
  }
  return (0);
}

void Install_Board_String(void)
{
s_tekst_50 const tekst_default_board_14 = { 50, 0, SIZE_14, "" };

  if (index_array == 0)
    tekst_board_14 = tekst_default_board_14;
  else
  {
    switch (board_IO[index_array - 1].board_type)
    {
      case IO_06_14_ID:  tekst_board_14 = *(s_tekst_50 *)tekst_IO_06_14_14[board_IO[index_array - 1].board_nr]; break;
      case IO_12_06_ID:  tekst_board_14 = *(s_tekst_50 *)tekst_IO_12_06_14[board_IO[index_array - 1].board_nr]; break;
      case IO_08_09_ID:  tekst_board_14 = *(s_tekst_50 *)tekst_IO_08_09_14[board_IO[index_array - 1].board_nr]; break;
      case IO_EKU_ID:    tekst_board_14 = *(s_tekst_50 *)tekst_IO_EKU_14[board_IO[index_array - 1].board_nr]; break;
      case IO_H2MC_ID:   tekst_board_14 = *(s_tekst_50 *)tekst_IO_H2MC_14[board_IO[index_array - 1].board_nr]; break;
      case IO_H1MC_ID:   tekst_board_14 = *(s_tekst_50 *)tekst_IO_H1MC_14[board_IO[index_array - 1].board_nr]; break;
      case IO_05_07_ID:  tekst_board_14 = *(s_tekst_50 *)tekst_IO_05_07_14[board_IO[index_array - 1].board_nr]; break;
      case IO_07_07_ID:  tekst_board_14 = *(s_tekst_50 *)tekst_IO_07_07_14[board_IO[index_array - 1].board_nr]; break;
      default:           tekst_board_14 = tekst_default_board_14; break;
    }
  }
}

void Install_Copy_IO_To_Board_IO(s_board_IO_on_off *IO,
                                 unsigned char max,
                                 unsigned char max_nr,
                                 unsigned char on_off,
                                 unsigned char IO_type,
                                 unsigned char IO_type_sel,
                                 unsigned char (*NotUsedFunc)(s_board_IO_on_off IO_new))
{
  index_array = 0;
  disp_cursor = disp_cursor_array[index_array];
  disp_block = disp_block_array[index_array];
  Install_Board_String();

  board_IO_ptr = IO;
  board_IO_array_size = max;
  board_IO_max_nr = max_nr;
  board_IO_on_off = on_off; // 1 = IO on off; 0 = IO nummers
  board_IO_type = IO_type;
  board_IO_type_sel = IO_type_sel;
  IONotUsedFunc = NotUsedFunc;
  Install_Fill_Board_IO(16, board_IO_type, board_IO_type_sel, board_IO_type_sel, NotUsedFunc);
}

void Install_Copy_IO_To_Board_IO_Range(s_board_IO_on_off *IO,
                                       unsigned char max,
                                       unsigned char max_nr,
                                       unsigned char on_off,
                                       unsigned char IO_type,
                                       unsigned char IO_type_sel_first,
                                       unsigned char IO_type_sel_last,
                                       unsigned char (*NotUsedFunc)(s_board_IO_on_off IO_new))
{
  index_array = 0;
  disp_cursor = disp_cursor_array[index_array];
  disp_block = disp_block_array[index_array];
  Install_Board_String();

  board_IO_ptr = IO;
  board_IO_array_size = max;
  board_IO_max_nr = max_nr;
  board_IO_on_off = on_off; // 1 = IO on off; 0 = IO nummers
  board_IO_type = IO_type;
  board_IO_type_sel = IO_type_sel_first;
  IONotUsedFunc = NotUsedFunc;
  Install_Fill_Board_IO(16, board_IO_type, IO_type_sel_first, IO_type_sel_last, NotUsedFunc);
}

void Install_Copy_Board_IO_To_IO(void)
{
int loop;
int nr = 0;
s_board_IO_on_off board_IO_help[16] = 
{
  {0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},
  {0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},
  {0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},
  {0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}
};

  if (board_IO_on_off)
  {
    for (loop = 0; loop < board_IO_max; loop++)
    {
      if (board_IO[loop].board_type)
      {
        if (board_IO[loop].on_off)
        {
          board_IO_help[nr] = board_IO[loop];
          nr++;
        }
      }
    }
  }
  else
  {
    for (loop = 0; loop < board_IO_max; loop++)
    {
      if (board_IO[loop].board_type)
      {
        if (board_IO[loop].on_off)
          board_IO_help[board_IO[loop].on_off-1] = board_IO[loop];
      }
    }
  }
  for (loop = 0; loop < board_IO_array_size; loop++)
  {
    if ((board_IO_ptr[loop].board_type != board_IO_help[loop].board_type) ||
        (board_IO_ptr[loop].board_nr != board_IO_help[loop].board_nr) ||
        (board_IO_ptr[loop].IO_nr != board_IO_help[loop].IO_nr) ||
        (board_IO_ptr[loop].on_off != board_IO_help[loop].on_off))
    {
      option_change_flag = 1;
      board_IO_ptr[loop] = board_IO_help[loop];
    }
  }
}

//================================================================================
void Install_Control_Board_IO(s_board_IO_on_off *IO,
                              unsigned char max,
                              unsigned char max_nr,
                              unsigned char on_off,
                              unsigned char IO_type,
                              unsigned char IO_type_sel)
{
unsigned int  Option;
unsigned char OptType;
TBoardSpecs const **Board;
TIOSpecs const *IOSpec;
int i;

  on_off;
  for (i = 0; i < max; i++)
  {
	if (i < max_nr)
	{
      Board = BoardSpecs;
	  while ((*Board != NULL) && ((*Board)->BoardType != IO[i].board_type))
	    Board++;

      if (*Board != NULL)
	  {
		Option = *((unsigned int *)((unsigned long)(*Board)->Option + (IO[i].board_nr * (*Board)->BoardSize)));
		if ((Option & 0x000F) != 0)
		{
	      IOSpec = (*Board)->IOSpecs;
		  while ((IOSpec->IOType != 0) && (IOSpec->IOType != IO_type))
		    IOSpec++;

          if (IOSpec->IOType != 0)
		  {
		    OptType = *((unsigned char *)((unsigned long)IOSpec->OptType + (IO[i].board_nr * (*Board)->BoardSize) + (IO[i].IO_nr * IOSpec->IOSize)));
			switch (IO_type) // switch for special IO's
			{
			  case RS485_BUS_ID:
			    if ((OptType & 0x0F) == 0)
			      IO[i] = IO_empty;
				break;
			  default:
  			    if (OptType != IO_type_sel)
			      IO[i] = IO_empty;
				break;
			}
		  }
		  else
		    IO[i] = IO_empty;
		}
		else
          IO[i] = IO_empty;
	  }
	  else
        IO[i] = IO_empty;
	}
	else
      IO[i] = IO_empty;
  }
}

void Install_Control_Board_IO_Range(s_board_IO_on_off *IO,
                                    unsigned char max,
                                    unsigned char max_nr,
                                    unsigned char on_off,
                                    unsigned char IO_type,
                                    unsigned char IO_type_sel_first,
                                    unsigned char IO_type_sel_last)
{
unsigned int  Option;
unsigned char OptType;
TBoardSpecs const **Board;
TIOSpecs const *IOSpec;
int i;

  on_off;
  for (i = 0; i < max; i++)
  {
	if (i < max_nr)
	{
      Board = BoardSpecs;
	  while ((*Board != NULL) && ((*Board)->BoardType != IO[i].board_type))
	    Board++;

      if (*Board != NULL)
	  {
	    Option = *((unsigned int *)((unsigned long)(*Board)->Option + (IO[i].board_nr * (*Board)->BoardSize)));
		if ((Option & 0x000F) != 0)
		{
	      IOSpec = (*Board)->IOSpecs;
		  while ((IOSpec->IOType != 0) && (IOSpec->IOType != IO_type))
		    IOSpec++;

          if (IOSpec->IOType != 0)
		  {
		    OptType = *((unsigned char *)((unsigned long)IOSpec->OptType + (IO[i].board_nr * (*Board)->BoardSize) + (IO[i].IO_nr * IOSpec->IOSize)));
			switch (IO_type) // switch for special IO's
			{
			  case RS485_BUS_ID:
			    if ((OptType & 0x0F) == 0)
			      IO[i] = IO_empty;
				break;
			  default:
                if ((OptType < IO_type_sel_first) || (OptType > IO_type_sel_last))
			      IO[i] = IO_empty;
			}
		  }
		  else
		    IO[i] = IO_empty;
		}
		else
          IO[i] = IO_empty;
	  }
	  else
        IO[i] = IO_empty;
	}
	else
      IO[i] = IO_empty;
  }
}

void Install_Control_All_Board_IO(void)
{
int i;
TMotor *pMotor;
// controleren en herberekenen van alle IO gegevens als toewijzen in en uitgangen wordt verlaten

  for (i = 0; i < opt_app.NumberMotorgroups; i++)
  {
    Install_Control_Board_IO(&opt_app.Motorgroup[i].DigOutAlarm,      1, 1, 1, DIGITAL_OUTPUT_ID, DIG_OUT_ALARM);
    Install_Control_Board_IO(&opt_app.Motorgroup[i].DigOutAlarmFlap,  1, 1, 1, DIGITAL_OUTPUT_ID, DIG_OUT_ALARM);
    Install_Control_Board_IO(&opt_app.Ventgroup[i].DigOutAlarmUrgent, 1, 1, 1, DIGITAL_OUTPUT_ID, DIG_OUT_ALARM);
    Install_Control_Board_IO(&opt_app.Ventgroup[i].DigInOnOff,        1, 1, 1, DIGITAL_INPUT_ID,  DIG_IN_VENT);
    Install_Control_Board_IO( opt_app.Ventgroup[i].RS485Bus,          4, 4, 1, RS485_BUS_ID,      0);
    switch (opt_app.Motorgroup[i].Type)
    {
      case TYPE_RAAM:
        Install_Control_Board_IO(&opt_app.Motorgroup[i].DigInOpen,      1, 1, 1, DIGITAL_INPUT_ID, DIG_IN_RAAM);
        Install_Control_Board_IO(&opt_app.Motorgroup[i].DigInClose,     1, 1, 1, DIGITAL_INPUT_ID, DIG_IN_RAAM);
        Install_Control_Board_IO(&opt_app.Motorgroup[i].AnaOutPosition, 1, 1, 1, ANALOG_OUTPUT_ID, ANA_OUT_RAAM);
		break;
      case TYPE_DOEK:
        Install_Control_Board_IO(&opt_app.Motorgroup[i].DigInOpen,      1, 1, 1, DIGITAL_INPUT_ID, DIG_IN_DOEK);
        Install_Control_Board_IO(&opt_app.Motorgroup[i].DigInClose,     1, 1, 1, DIGITAL_INPUT_ID, DIG_IN_DOEK);
        Install_Control_Board_IO(&opt_app.Motorgroup[i].AnaOutPosition, 1, 1, 1, ANALOG_OUTPUT_ID, ANA_OUT_DOEK);
		break;
      case TYPE_VENT:
        Install_Control_Board_IO(&opt_app.Motorgroup[i].DigInOpen,      1, 1, 1, DIGITAL_INPUT_ID, DIG_IN_VENT);
        Install_Control_Board_IO(&opt_app.Motorgroup[i].DigInClose,     1, 1, 1, DIGITAL_INPUT_ID, DIG_IN_VENT);
        Install_Control_Board_IO(&opt_app.Motorgroup[i].AnaOutPosition, 1, 1, 1, ANALOG_OUTPUT_ID, ANA_OUT_VENT);
		break;
    }
    switch (opt_app.Motorgroup[i].Type)
    {
      case TYPE_RAAM:
      case TYPE_DOEK:
        pMotor = Motorgroup[i].FirstMotor;
        while (pMotor != NULL)
        {
          if (opt_app.Motorgroup[i].Type == 0)
            Install_Control_Board_IO(&opt_app.Motor[pMotor->Number].IO, 1, 1, 1, MOTOR_CONTROL_ID, MOTOR_CONTROL_RAAM);
          else
            Install_Control_Board_IO(&opt_app.Motor[pMotor->Number].IO, 1, 1, 1, MOTOR_CONTROL_ID, MOTOR_CONTROL_DOEK);
          pMotor = pMotor->Next;
        }
	    break;
	}
  }

  for (i = 0; i < MAX_LUCHTMENGKAST_GROEP; i++)
  {
    if (opt_app.LuchtmengkastGroep[i].Enabled)
	{
      Install_Control_Board_IO(&opt_app.LuchtmengkastGroep[i].Binnenklep.Open,     1, 1, 0, DIGITAL_INPUT_ID,  DIG_IN_LAMEL);
      Install_Control_Board_IO(&opt_app.LuchtmengkastGroep[i].Buitenklep.Open,     1, 1, 0, DIGITAL_INPUT_ID,  DIG_IN_LAMEL);
      Install_Control_Board_IO(&opt_app.LuchtmengkastGroep[i].Verwarming.Open,     1, 1, 0, DIGITAL_INPUT_ID,  DIG_IN_VERWARMING);
      Install_Control_Board_IO(&opt_app.LuchtmengkastGroep[i].Afblaasvent.Open,    1, 1, 0, DIGITAL_INPUT_ID,  DIG_IN_VENT);
      Install_Control_Board_IO(&opt_app.LuchtmengkastGroep[i].Inblaasvent.Open,    1, 1, 0, DIGITAL_INPUT_ID,  DIG_IN_VENT);
      Install_Control_Board_IO(&opt_app.LuchtmengkastGroep[i].Binnenklep.Close,    1, 1, 0, DIGITAL_INPUT_ID,  DIG_IN_LAMEL);
      Install_Control_Board_IO(&opt_app.LuchtmengkastGroep[i].Buitenklep.Close,    1, 1, 0, DIGITAL_INPUT_ID,  DIG_IN_LAMEL);
      Install_Control_Board_IO(&opt_app.LuchtmengkastGroep[i].Verwarming.Close,    1, 1, 0, DIGITAL_INPUT_ID,  DIG_IN_VERWARMING);
      Install_Control_Board_IO(&opt_app.LuchtmengkastGroep[i].Afblaasvent.Close,   1, 1, 0, DIGITAL_INPUT_ID,  DIG_IN_VENT);
      Install_Control_Board_IO(&opt_app.LuchtmengkastGroep[i].Inblaasvent.Close,   1, 1, 0, DIGITAL_INPUT_ID,  DIG_IN_VENT);
      Install_Control_Board_IO(&opt_app.LuchtmengkastGroep[i].Binnenklep.AnaIn,    1, 1, 0, ANALOG_INPUT_ID,   ANA_IN_LAMEL);
      Install_Control_Board_IO(&opt_app.LuchtmengkastGroep[i].Buitenklep.AnaIn,    1, 1, 0, ANALOG_INPUT_ID,   ANA_IN_LAMEL);
      Install_Control_Board_IO(&opt_app.LuchtmengkastGroep[i].Verwarming.AnaIn,    1, 1, 0, ANALOG_INPUT_ID,   ANA_IN_VERWARMING);
      Install_Control_Board_IO(&opt_app.LuchtmengkastGroep[i].Afblaasvent.AnaIn,   1, 1, 0, ANALOG_INPUT_ID,   ANA_IN_VENT);
      Install_Control_Board_IO(&opt_app.LuchtmengkastGroep[i].Inblaasvent.AnaIn,   1, 1, 0, ANALOG_INPUT_ID,   ANA_IN_VENT);
      Install_Control_Board_IO(&opt_app.LuchtmengkastGroep[i].Binnenklep.AnaOut,   1, 1, 0, ANALOG_OUTPUT_ID,  ANA_OUT_LAMEL);
      Install_Control_Board_IO(&opt_app.LuchtmengkastGroep[i].Buitenklep.AnaOut,   1, 1, 0, ANALOG_OUTPUT_ID,  ANA_OUT_LAMEL);
      Install_Control_Board_IO(&opt_app.LuchtmengkastGroep[i].Verwarming.AnaOut,   1, 1, 0, ANALOG_OUTPUT_ID,  ANA_OUT_VERWARMING);
      Install_Control_Board_IO(&opt_app.LuchtmengkastGroep[i].Afblaasvent.AnaOut,  1, 1, 0, ANALOG_OUTPUT_ID,  ANA_OUT_VENT);
      Install_Control_Board_IO(&opt_app.LuchtmengkastGroep[i].Inblaasvent.AnaOut,  1, 1, 0, ANALOG_OUTPUT_ID,  ANA_OUT_VENT);
      Install_Control_Board_IO(&opt_app.LuchtmengkastGroep[i].DigOutAlarm,         1, 1, 0, DIGITAL_OUTPUT_ID, DIG_OUT_ALARM);
	}
  }

  for (i = 0; i < MAX_LUCHTMENGKAST; i++)
  {
    if (opt_app.Luchtmengkast[i].Enabled)
	{
      Install_Control_Board_IO(&opt_app.Luchtmengkast[i].AnaInMengtemp,     1, 1, 0, ANALOG_INPUT_ID,  ANA_IN_TEMP);
      Install_Control_Board_IO(&opt_app.Luchtmengkast[i].AnaInInblaastemp,  1, 1, 0, ANALOG_INPUT_ID,  ANA_IN_TEMP);
      Install_Control_Board_IO(&opt_app.Luchtmengkast[i].DigInVorst,        1, 1, 0, DIGITAL_INPUT_ID, DIG_IN_VORST);
      Install_Control_Board_IO(&opt_app.Luchtmengkast[i].DigInAlarm,        1, 1, 0, DIGITAL_INPUT_ID, DIG_IN_ALARM);
      Install_Control_Board_IO(&opt_app.Luchtmengkast[i].DigInDrukverschil, 1, 1, 0, DIGITAL_INPUT_ID, DIG_IN_ALARM);
	  
      Install_Control_Board_IO(&opt_app.Luchtmengkast[i].Binnenklep.Open,     1, 1, 0, DIGITAL_OUTPUT_ID, DIG_OUT_LAMEL);
      Install_Control_Board_IO(&opt_app.Luchtmengkast[i].Buitenklep.Open,     1, 1, 0, DIGITAL_OUTPUT_ID, DIG_OUT_LAMEL);
      Install_Control_Board_IO(&opt_app.Luchtmengkast[i].Verwarming.Open,     1, 1, 0, DIGITAL_OUTPUT_ID, DIG_OUT_VERWARMING);
      Install_Control_Board_IO(&opt_app.Luchtmengkast[i].Afblaasvent.Open,    1, 1, 0, DIGITAL_OUTPUT_ID, DIG_OUT_VENT);
      Install_Control_Board_IO(&opt_app.Luchtmengkast[i].Inblaasvent.Open,    1, 1, 0, DIGITAL_OUTPUT_ID, DIG_OUT_VENT);
      Install_Control_Board_IO(&opt_app.Luchtmengkast[i].Binnenklep.Close,    1, 1, 0, DIGITAL_OUTPUT_ID, DIG_OUT_LAMEL);
      Install_Control_Board_IO(&opt_app.Luchtmengkast[i].Buitenklep.Close,    1, 1, 0, DIGITAL_OUTPUT_ID, DIG_OUT_LAMEL);
      Install_Control_Board_IO(&opt_app.Luchtmengkast[i].Verwarming.Close,    1, 1, 0, DIGITAL_OUTPUT_ID, DIG_OUT_VERWARMING);
      Install_Control_Board_IO(&opt_app.Luchtmengkast[i].Afblaasvent.Close,   1, 1, 0, DIGITAL_OUTPUT_ID, DIG_OUT_VENT);
      Install_Control_Board_IO(&opt_app.Luchtmengkast[i].Inblaasvent.Close,   1, 1, 0, DIGITAL_OUTPUT_ID, DIG_OUT_VENT);
      Install_Control_Board_IO(&opt_app.Luchtmengkast[i].Binnenklep.AnaOut,   1, 1, 0, ANALOG_OUTPUT_ID, ANA_OUT_LAMEL);
      Install_Control_Board_IO(&opt_app.Luchtmengkast[i].Buitenklep.AnaOut,   1, 1, 0, ANALOG_OUTPUT_ID, ANA_OUT_LAMEL);
      Install_Control_Board_IO(&opt_app.Luchtmengkast[i].Verwarming.AnaOut,   1, 1, 0, ANALOG_OUTPUT_ID, ANA_OUT_VERWARMING);
      Install_Control_Board_IO(&opt_app.Luchtmengkast[i].Afblaasvent.AnaOut,  1, 1, 0, ANALOG_OUTPUT_ID, ANA_OUT_VENT);
      Install_Control_Board_IO(&opt_app.Luchtmengkast[i].Inblaasvent.AnaOut,  1, 1, 0, ANALOG_OUTPUT_ID, ANA_OUT_VENT);
      Install_Control_Board_IO(&opt_app.Luchtmengkast[i].Binnenklep.AnaIn,    1, 1, 0, ANALOG_INPUT_ID,  ANA_IN_LAMEL);
      Install_Control_Board_IO(&opt_app.Luchtmengkast[i].Buitenklep.AnaIn,    1, 1, 0, ANALOG_INPUT_ID,  ANA_IN_LAMEL);
      Install_Control_Board_IO(&opt_app.Luchtmengkast[i].Verwarming.AnaIn,    1, 1, 0, ANALOG_INPUT_ID,  ANA_IN_VERWARMING);
	  switch (opt_app.Luchtmengkast[i].Afblaasvent.TypeSturing)
	  { 
        case TYPE_STURING_DIGITAAL:
        case TYPE_STURING_ANALOOG :
          Install_Control_Board_IO(&opt_app.Luchtmengkast[i].Afblaasvent.AnaIn, 1, 1, 0, ANALOG_INPUT_ID, ANA_IN_VENT);
          break;
        case TYPE_STURING_EBMBUS             :
        case TYPE_STURING_EBM_MODBUS         :
        case TYPE_STURING_EC_BLUE_MODBUS     :
        case TYPE_STURING_EC_BLUE_PREMIUM    : 
        case TYPE_STURING_MB_ROSENBERG       :
        case TYPE_STURING_MB_CLIMAFAN        :
        case TYPE_STURING_MB_ROSENBERG_GEN3  :
        case TYPE_STURING_MB_NICOTRA_GEBHARDT:
          Install_Control_Board_IO(&opt_app.Luchtmengkast[i].Afblaasvent.AnaIn, 1, 1, 1, RS485_BUS_ID, 0);
		  break;
	  }
	  switch (opt_app.Luchtmengkast[i].Inblaasvent.TypeSturing)
	  {
        case TYPE_STURING_DIGITAAL:
        case TYPE_STURING_ANALOOG :
          Install_Control_Board_IO(&opt_app.Luchtmengkast[i].Inblaasvent.AnaIn, 1, 1, 0, ANALOG_INPUT_ID, ANA_IN_VENT);
          break;
        case TYPE_STURING_EBMBUS             :
        case TYPE_STURING_EBM_MODBUS         :
        case TYPE_STURING_EC_BLUE_MODBUS     :
        case TYPE_STURING_EC_BLUE_PREMIUM    :
        case TYPE_STURING_MB_ROSENBERG       :
        case TYPE_STURING_MB_CLIMAFAN        :
        case TYPE_STURING_MB_ROSENBERG_GEN3  :  
        case TYPE_STURING_MB_NICOTRA_GEBHARDT:  
          Install_Control_Board_IO(&opt_app.Luchtmengkast[i].Inblaasvent.AnaIn, 1, 1, 1, RS485_BUS_ID, 0);
          break;
	  }
	}
  }

  Install_Control_Board_IO(opt_app.VrijgaveVent.DigInVrijgave, MAX_VRIJGAVE, opt_app.VrijgaveVent.Aantal, 0, DIGITAL_INPUT_ID, DIG_IN_VENT);
  Install_Control_Board_IO(opt_app.VrijgaveVent.RS485Bus,      MAX_VRIJGAVE, MAX_VRIJGAVE,                1, RS485_BUS_ID,     0          );

  CheckOptions();
}

//*****************************************************************************
unsigned char Shift_Board_IO_Right(void)
{
s_board_IO_on_off IO_new = {0,0,0,0};
int i;

  if ((board_IO_max == 16) && (index_array > 0))
  {
    Get_Next_Board_IO(board_IO[index_array - 1], &IO_new, 1, 0, board_IO_type, board_IO_type_sel, board_IO_type_sel, IONotUsedFunc);
    if (IO_new.board_type != 0)
    {
      if ((index_array == 16) || !Board_IO_Used(IO_new, &board_IO[index_array], 1))
      {
        for (i = 0; i < index_array; i++)
        {
          if (board_IO[i].on_off == 0)
          {
            for (i; i < index_array - 1; i++)
              board_IO[i] = board_IO[i+1];
            board_IO[i] = IO_new;
            return (1);
          }
        }
      }
    }
  }
  return (0);
}                             

unsigned char Shift_Board_IO_Left(void)
{
s_board_IO_on_off IO_new = {0,0,0,0};
int i;

  if ((board_IO_max == 16) && (index_array > 0))
  {
    Get_Next_Board_IO(board_IO[index_array - 1], &IO_new, -1, 0, board_IO_type, board_IO_type_sel, board_IO_type_sel, IONotUsedFunc);
    if (IO_new.board_type != 0)
    {
      if ((index_array == 1) || !Board_IO_Used(IO_new, &board_IO[index_array - 2], 1))
      {
        for (i = 15; i >= index_array - 1; i--)
        {
          if (board_IO[i].on_off == 0)
          {
            for (i; i > index_array - 1; i--)
              board_IO[i] = board_IO[i-1];
            board_IO[i] = IO_new;
            return (1);
          }
        }
      }
    }
  }
  return (0);
}                             

//*****************************************************************************
static void Set_Key_IO(void)
{
  key_IO.type = UCHAR;
  key_IO.max_digits = 1;
  key_IO.value = &board_IO[index_array - 1].on_off;
  key_IO.min_value = &key_min;
  key_IO.max_value = &key_max;
  key_min = 0;
  key_max = board_IO_max_nr - 1;
  Get_Value();
}

void Arrow_Left_IO_Select(void)
{
  if (((password_enabled & PASSWORD_ENABLED_OPT_MASK) == 0)	&& (index_array == 0))
    Prev_Screen();
  else
  {
    if ((board_IO_max == 16) && (index_array == 0))
    {
      while (index_array < 16)
      {
        if (!Shift_Board_IO_Right())
        {
          index_array++;
          index_array %= board_IO_max + 1;
          disp_cursor = disp_cursor_array[index_array];
          disp_block = disp_block_array[index_array];
        }
      }
      while (Shift_Board_IO_Right());
    }
    else if (!Shift_Board_IO_Left())
    {
      index_array += board_IO_max;
      index_array %= board_IO_max + 1;
      disp_cursor = disp_cursor_array[index_array];
      disp_block = disp_block_array[index_array];
    }
    Install_Board_String();
    if (index_array)
    {
      Set_Key_IO();
    }
  }
}

void Arrow_Right_IO_Select(void)
{
unsigned char shift = 0;

  if (!Shift_Board_IO_Right())
  {
    index_array++;
    index_array %= board_IO_max + 1;
    disp_cursor = disp_cursor_array[index_array];
    disp_block = disp_block_array[index_array];
  }
  Install_Board_String();
  if (index_array)
  {
    Set_Key_IO();
  }
}

static unsigned char IO_Select_Nr_Used(unsigned char nr)
{
int loop;

  for (loop = 0; loop < board_IO_max; loop++)
  {
    if (board_IO[loop].on_off == nr)
      return (1);
  }  
  return (0);
}
/* TD - LBK
static unsigned char IO_Select_First_Free(void)
{
unsigned char nr;

  for (nr = 1; nr <= board_IO_max_nr; nr++)
  {
    if (!IO_Select_Nr_Used(nr))
      return (nr);
  }
  return (0);
}
*/
static unsigned char IO_Select_First_Free(unsigned char old_nr)
{
unsigned char nr;

  for (nr = 1; nr <= board_IO_max_nr; nr++)
  {
    if ((nr > old_nr) && !IO_Select_Nr_Used(nr))
      return (nr);
  }
  return (0);
}

static unsigned char IO_Select_Aantal(void)
{
unsigned char nr;
unsigned char cnt = 0;

  for (nr = 0; nr < board_IO_max; nr++)
  {
    if (board_IO[nr].on_off != 0)
      cnt++;
  }
  return (cnt);
}
/* TD - LBK
void Arrow_Up_IO_Select(void)
{
int loop;

  if (password_enabled & PASSWORD_ENABLED_OPT_MASK)
  {
    if (board_IO_max_nr == 1)
    {
      for (loop = 0; loop < board_IO_max; loop++)
        board_IO[loop].on_off = 0;
    }
    if (!board_IO[index_array - 1].on_off)
    {
      if (board_IO_on_off)
      {
        if (IO_Select_Aantal() < board_IO_max_nr)
          board_IO[index_array - 1].on_off = 1;
      }
      else
        board_IO[index_array - 1].on_off = IO_Select_First_Free();
    } 
    Arrow_Right_IO_Select();
  }
}
*/
void Arrow_Up_IO_Select(void)
{
int loop;

  if (password_enabled & PASSWORD_ENABLED_OPT_MASK)
  {
    if (board_IO_max_nr == 1)
    {
      for (loop = 0; loop < board_IO_max; loop++)
        board_IO[loop].on_off = 0;
    }
//    if (!board_IO[index_array - 1].on_off)
//    {
      if (board_IO_on_off)
      {
        if (IO_Select_Aantal() < board_IO_max_nr)
          board_IO[index_array - 1].on_off = 1;
		Arrow_Right_IO_Select();
      }
      else
        board_IO[index_array - 1].on_off = IO_Select_First_Free(board_IO[index_array - 1].on_off);
//    } 
//    Arrow_Right_IO_Select();
  }
}

void Arrow_Down_IO_Select(void)
{
  if (password_enabled & PASSWORD_ENABLED_OPT_MASK)
  {
    board_IO[index_array - 1].on_off = 0;
    Arrow_Right_IO_Select();
  }
}

void Arrow_IO_Select_Func(void)
{
  switch (key)
  {
    case UP:
      if (index_array == 0)
        Decrement_Func();
      else
        Arrow_Up_IO_Select();
      break;
    case DOWN:
      if (index_array == 0)
        Increment_Func();
      else
        Arrow_Down_IO_Select();
      break;
    case LEFT:
      Arrow_Left_IO_Select();
      break;
    case RIGHT:
      Arrow_Right_IO_Select();
      break;
  }
}

void Enter_IO_Select_Func(void)
{
  index_array = 0;
  disp_cursor = disp_cursor_array[0];
  disp_block = disp_block_array[0];
  Install_Board_String();
}

//*****************************************************************************
static void Arrow_Left_IO_Select_No_Shift(void)
{
  if ((password_enabled & PASSWORD_ENABLED_OPT_MASK) == 0)
    Prev_Screen();
  else
  {
    index_array += board_IO_max;
    index_array %= board_IO_max + 1;
    disp_cursor = disp_cursor_array[index_array];
    disp_block = disp_block_array[index_array];
    Install_Board_String();
    if (index_array)
    {
      Set_Key_IO();
    }
  }
}

static void Arrow_Right_IO_Select_No_Shift(void)
{
  index_array++;
  index_array %= board_IO_max + 1;
  disp_cursor = disp_cursor_array[index_array];
  disp_block = disp_block_array[index_array];
  Install_Board_String();
  if (index_array)
  {
    Set_Key_IO();
  }
}
/* TD - LBK
static void Arrow_Up_IO_Select_No_Shift(void)
{
int loop;

  if (password_enabled & PASSWORD_ENABLED_OPT_MASK)
  {
    if (board_IO_max_nr == 1)
    {
      for (loop = 0; loop < board_IO_max; loop++)
        board_IO[loop].on_off = 0;
    }
    if (!board_IO[index_array - 1].on_off)
    {
      if (board_IO_on_off)
      {
        if (IO_Select_Aantal() < board_IO_max_nr)
          board_IO[index_array - 1].on_off = 1;
      }
      else
        board_IO[index_array - 1].on_off = IO_Select_First_Free();
    } 
    Arrow_Right_IO_Select_No_Shift();
  }
}
*/
static void Arrow_Up_IO_Select_No_Shift(void)
{
int loop;

  if (password_enabled & PASSWORD_ENABLED_OPT_MASK)
  {
    if (board_IO_max_nr == 1)
    {
      for (loop = 0; loop < board_IO_max; loop++)
        board_IO[loop].on_off = 0;
    }
    if (!board_IO[index_array - 1].on_off)
    {
      if (board_IO_on_off)
      {
        if (IO_Select_Aantal() < board_IO_max_nr)
          board_IO[index_array - 1].on_off = 1;
      }
      else
        board_IO[index_array - 1].on_off = IO_Select_First_Free(board_IO[index_array - 1].on_off);
    } 
    Arrow_Right_IO_Select_No_Shift();
  }
}

static void Arrow_Down_IO_Select_No_Shift(void)
{
  if (password_enabled & PASSWORD_ENABLED_OPT_MASK)
  {
    board_IO[index_array - 1].on_off = 0;
    Arrow_Right_IO_Select_No_Shift();
  }
}

void Arrow_IO_Select_No_Shift_Func(void)
{
  switch (key)
  {
    case UP:
      if (index_array == 0)
        Decrement_Func();
      else
        Arrow_Up_IO_Select_No_Shift();
      break;
    case DOWN:
      if (index_array == 0)
        Increment_Func();
      else
        Arrow_Down_IO_Select_No_Shift();
      break;
    case LEFT:
      Arrow_Left_IO_Select_No_Shift();
      break;
    case RIGHT:
      Arrow_Right_IO_Select_No_Shift();
      break;
  }
}

//*****************************************************************************
// routines die gebruik maken van board_IO maar waarbij alleen de optie aan/uit
// belangerijk is

void Install_Copy_Option_To_Board_IO(unsigned char *opt, unsigned char max, unsigned char max_nr)
{
int loop;

  index_array = 0;
  disp_cursor = disp_cursor_array[index_array];
  disp_block = disp_block_array[index_array];
  Install_Board_String();

  board_IO_ptr = (s_board_IO_on_off *)opt;
  board_IO_array_size = max;
  board_IO_max = board_IO_max_nr = max_nr;
  board_IO_on_off = 1; // 1 = IO on off; 0 = IO nummers

  for (loop = 0; loop < 16; loop++)
  {
    board_IO[loop].board_type = 0;
    board_IO[loop].board_nr = 0;
    if (loop < board_IO_max)
    {
      board_IO[loop].IO_nr = loop;
      board_IO[loop].on_off = opt[loop];
    }
    else
    {
      board_IO[loop].IO_nr = 17;
      board_IO[loop].on_off = 0;
    }  
  }
}

void Install_Copy_Board_IO_To_Option(void)
{
int loop;
unsigned char *ptr = (unsigned char *)board_IO_ptr;

  for (loop = 0; loop < board_IO_array_size; loop++)
  {
    if (ptr[loop] != board_IO[loop].on_off)
    {
      option_change_flag = 1;
      ptr[loop] = board_IO[loop].on_off;
    }
  }
}

