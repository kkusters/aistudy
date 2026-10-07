// C__LCD_PIXEL.C

#include <string.h> 
#include <time.h> 

#include "ch_define.h"
#include "ch_alg.h"
#include "ch_alarm.h"
#include "ch_const.h"
#include "ch_disp.h"
#include "ch_disp_alarm_0.h"
#include "ch_disp_alarmen_actief_1.h"
#include "ch_disp_option_1.h"
#include "ch_disp_opt_IO_Select.h"
#include "ch_disp_password.h"
#include "ch_IO.h"
#include "ch_lcd_7.h"
#include "ch_lcd_10.h"
#include "ch_lcd_14.h"
#include "ch_lcd_20.h"
#include "ch_lcd_bitmap.h"
#include "ch_lcd_hardware.h"
#include "ch_main.h"
#include "ch_string.h"
#include "ch_lcd_pixel.h"

#define LOPER_SIZE 76

bit cursor_state = 0;
void (*draw_char)(char c);

typedef union
{
  unsigned char c[2];
  unsigned int i;
} s_unsigned_char_int;

//*****************************************************************************
// horizontale stippellijn
void LCD_Draw_Line_Hor_Abs(unsigned char x1, unsigned char x2, unsigned char y)
{
int start_byte;
int start_bit;
int byte_nr, bit_nr;
int line_nr;
int bit_cnt;
int dx;
int start_y;

  if ((x1 < (unsigned char)0) ||
      (y  < (unsigned char)0) ||
      (x2 >= (unsigned char)240) ||
      (y  >= (unsigned char)128))
    return;
  start_byte = 29 - (x1 / 8);
  start_bit  = x1 % 8;
  dx = x2 - x1 + 1;
  start_y = 127 - y;

  byte_nr = start_byte;
  bit_nr = start_bit;
  line_nr = start_y;
  for (bit_cnt = 0; bit_cnt < dx; bit_cnt += 2)
  {
    lcd_screen[line_nr][byte_nr] |= (0x01 << bit_nr);
    bit_nr += 2;
    if (bit_nr > 7)
    {
      byte_nr--;
      bit_nr = bit_nr % 8;
    }
  }
}

void LCD_Draw_Line_Hor_Rel(unsigned char x1, unsigned char x2, unsigned char y)
{
  x1 += lcd_x_offset;
  x2 += lcd_x_offset;
  y  += lcd_y_offset;
  LCD_Draw_Line_Hor_Abs(x1, x2, y);
}

//*****************************************************************************
// verticale stippellijn
void LCD_Draw_Line_Vert_Abs(unsigned char y1, unsigned char y2, unsigned char x)
{
int mask_bit;
int byte_nr, bit_nr;
int line_nr;
int start_y;
int einde_y;

  if ((x  < (unsigned char)0) ||
      (y1 < (unsigned char)0) ||
      (x  >= (unsigned char)240) ||
      (y2 >= (unsigned char)128))
    return;
  byte_nr = 29 - (x / 8);
  bit_nr  = x % 8;
  mask_bit = 0x01 << bit_nr;
  start_y = 127 - y1;
  einde_y = 127 - y2 - 1;
  
  for (line_nr = start_y; line_nr > einde_y; line_nr -= 2)
  {
    lcd_screen[line_nr][byte_nr] |= mask_bit;
  }
}

void LCD_Draw_Line_Vert_Rel(unsigned char y1, unsigned char y2, unsigned char x)
{
  x  += lcd_x_offset;
  y1 += lcd_y_offset;
  y2 += lcd_y_offset;
  LCD_Draw_Line_Vert_Abs(y1, y2, x);
}
//*****************************************************************************
void LCD_Draw_Black_Block_Abs(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2)
{
static unsigned char const mask_start_tbl[] = { 0xFF, 0xFE, 0xFC, 0xF8, 0xF0, 0xE0, 0xC0, 0x80 };
static unsigned char const mask_end_tbl[] =   { 0xFF, 0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3F, 0x7F };
int start;
int start_bit;
int mask_start;
int mask_end;
int line_bytes_nr;
int line_cnt;
int dx;
unsigned char *ptr;
int loop_y;
int start_y;
int einde_y;

  if ((x1 < (unsigned char)0) ||
      (y1 < (unsigned char)0) ||
      (x2 >= (unsigned char)240) ||
      (y2 >= (unsigned char)128))
    return;
  start = 29 - (x1 / 8);
  start_bit = x1 % 8;
  mask_start = mask_start_tbl[start_bit];
  mask_end = mask_end_tbl[(x2 + 1) % 8];
  dx = x2 - x1 + 1;
  start_y = 127 - y1;
  einde_y = 127 - y2 - 1;
  line_bytes_nr = dx / 8;
  if (dx % 8)
    line_bytes_nr++;
  if ((8 - start_bit) <= ((dx - 1) % 8))
    line_bytes_nr++;

  if (line_bytes_nr == 1) // data binnen zelfde unsigned int op 1 regel
  {
    mask_start &= mask_end;
    for (loop_y = start_y; loop_y > einde_y; loop_y--)
      lcd_screen[loop_y][start] |= mask_start;
  }
  else // waarde verdeeld over meer dan 2 unsigned int op 1 regel
  {
    line_bytes_nr--;
    for (loop_y = start_y; loop_y > einde_y; loop_y--)
    {
      // eerste byte, houd rekening met start_mask
      ptr = &lcd_screen[loop_y][start];
      line_cnt = line_bytes_nr;
      *ptr |= mask_start;
      ptr--;
      // tussen liggende data bytes indien aanwezig
      while (line_cnt > 1)
      {
        *ptr |= 0xFF;
        line_cnt--;
        ptr--;
      }
      // laatste byte, houdt rekening met eind_mask
      *ptr |= mask_end;
    }
  }
}

void LCD_Draw_Black_Block_Abs_PC(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2)
{
static unsigned char const mask_start_tbl[] = { 0xFF, 0xFE, 0xFC, 0xF8, 0xF0, 0xE0, 0xC0, 0x80 };
static unsigned char const mask_end_tbl[] =   { 0xFF, 0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3F, 0x7F };
int start;
int start_bit;
int mask_start;
int mask_end;
int line_bytes_nr;
int line_cnt;
int dx;
unsigned char *ptr;
int loop_y;
int start_y;
int einde_y;

  if ((x1 < (unsigned char)0) ||
      (y1 < (unsigned char)0) ||
      (x2 >= (unsigned char)240) ||
      (y2 >= (unsigned char)128))
    return;
  start = 29 - (x1 / 8);
  start_bit = x1 % 8;
  mask_start = mask_start_tbl[start_bit];
  mask_end = mask_end_tbl[(x2 + 1) % 8];
  dx = x2 - x1 + 1;
  start_y = 127 - y1;
  einde_y = 127 - y2 - 1;
  line_bytes_nr = dx / 8;
  if (dx % 8)
    line_bytes_nr++;
  if ((8 - start_bit) <= ((dx - 1) % 8))
    line_bytes_nr++;

  if (line_bytes_nr == 1) // data binnen zelfde unsigned int op 1 regel
  {
    mask_start &= mask_end;
    for (loop_y = start_y; loop_y > einde_y; loop_y--)
      lcd_pc_screen[loop_y][start] |= mask_start;
  }
  else // waarde verdeeld over meer dan 2 unsigned int op 1 regel
  {
    line_bytes_nr--;
    for (loop_y = start_y; loop_y > einde_y; loop_y--)
    {
      // eerste byte, houd rekening met start_mask
      ptr = &lcd_pc_screen[loop_y][start];
      line_cnt = line_bytes_nr;
      *ptr |= mask_start;
      ptr--;
      // tussen liggende data bytes indien aanwezig
      while (line_cnt > 1)
      {
        *ptr |= 0xFF;
        line_cnt--;
        ptr--;
      }
      // laatste byte, houdt rekening met eind_mask
      *ptr |= mask_end;
    }
  }
}


void LCD_Draw_Black_Block_Rel(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2)
{
  x1 += lcd_x_offset;
  x2 += lcd_x_offset;
  y1 += lcd_y_offset;
  y2 += lcd_y_offset;
  LCD_Draw_Black_Block_Abs(x1, y1, x2, y2);
}

void LCD_Draw_Black_Vierkant(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2)
{
  LCD_Draw_Black_Block_Rel(x1, y1, x2, y1); 
  LCD_Draw_Black_Block_Rel(x1, y2, x2, y2); 
  LCD_Draw_Black_Block_Rel(x1, y1, x1, y2); 
  LCD_Draw_Black_Block_Rel(x2, y1, x2, y2); 
  
}
//-----------------------------------------------------------------------------

void LCD_Draw_White_Block(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2)
{
static unsigned char const mask_start_tbl[] = { 0x00, 0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3F, 0xFF };
static unsigned char const mask_end_tbl[] =   { 0x00, 0xFE, 0xFC, 0xF8, 0xF0, 0xE0, 0xC0, 0x80 };
int start;
int start_bit;
int mask_start;
int mask_end;
int line_bytes_nr;
int line_cnt;
int dx;
unsigned char *ptr;
int loop_y;
int start_y;
int einde_y;

  x1 += lcd_x_offset;
  x2 += lcd_x_offset;
  y1 += lcd_y_offset;
  y2 += lcd_y_offset;
  if ((x1 < (unsigned char)0) ||
      (y1 < (unsigned char)0) ||
      (x2 >= (unsigned char)240) ||
      (y2 >= (unsigned char)128))
    return;
  start = 29 - (x1 / 8);
  start_bit = x1 % 8;
  mask_start = mask_start_tbl[start_bit];
  mask_end = mask_end_tbl[(x2 + 1) % 8];
  dx = x2 - x1 + 1;
  start_y = 127 - y1;
  einde_y = 127 - y2 - 1;
  line_bytes_nr = dx / 8;
  if (dx % 8)
    line_bytes_nr++;
  if ((8 - start_bit) <= ((dx - 1) % 8))
    line_bytes_nr++;

  if (line_bytes_nr == 1) // data binnen zelfde unsigned int op 1 regel
  {
    mask_start |= mask_end;
    for (loop_y = start_y; loop_y > einde_y; loop_y--)
      lcd_screen[loop_y][start] &= mask_start;
  }
  else // waarde verdeeld over meer dan 2 unsigned int op 1 regel
  {
    line_bytes_nr--;
    for (loop_y = start_y; loop_y > einde_y; loop_y--)
    {
      // eerste byte, houd rekening met start_mask
      ptr = &lcd_screen[loop_y][start];
      line_cnt = line_bytes_nr;
      *ptr &= mask_start;
      ptr--;
      // tussen liggende data bytes indien aanwezig
      while (line_cnt > 1)
      {
        *ptr &= 0x00;
        line_cnt--;
        ptr--;
      }
      // laatste byte, houdt rekening met eind_mask
      *ptr &= mask_end;
    }
  }
}

void LCD_Draw_White_Vierkant(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2)
{
  LCD_Draw_Black_Block_Rel(x1, y1, x2, y1); 
  LCD_Draw_Black_Block_Rel(x1, y2, x2, y2); 
  LCD_Draw_Black_Block_Rel(x1, y1, x1, y2); 
  LCD_Draw_Black_Block_Rel(x2, y1, x2, y2); 
  
}
//-----------------------------------------------------------------------------

void LCD_Invert_Block(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2)
{
static unsigned char const mask_start_tbl[] = { 0xFF, 0xFE, 0xFC, 0xF8, 0xF0, 0xE0, 0xC0, 0x80 };
static unsigned char const mask_end_tbl[] =   { 0xFF, 0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3F, 0x7F };
int start = 29 - (x1 / 8);
int start_bit = x1 % 8;
int mask_start = mask_start_tbl[start_bit];
int mask_end = mask_end_tbl[(x2 + 1) % 8];
int line_bytes_nr;
int line_cnt;
int dx = x2 - x1 + 1;
unsigned char *ptr;
int loop_y;
int start_y = 127 - y1;
int einde_y = 127 - y2 - 1;

  x1 += lcd_x_offset;
  x2 += lcd_x_offset;
  y1 += lcd_y_offset;
  y2 += lcd_y_offset;
  if ((x1 < (unsigned char)0) ||
      (y1 < (unsigned char)0) ||
      (x2 >= (unsigned char)240) ||
      (y2 >= (unsigned char)128))
    return;
  start = 29 - (x1 / 8);
  start_bit = x1 % 8;
  mask_start = mask_start_tbl[start_bit];
  mask_end = mask_end_tbl[(x2 + 1) % 8];
  dx = x2 - x1 + 1;
  start_y = 127 - y1;
  einde_y = 127 - y2 - 1;
  line_bytes_nr = dx / 8;
  if (dx % 8)
    line_bytes_nr++;
  if ((8 - start_bit) <= ((dx - 1) % 8))
    line_bytes_nr++;

  if (line_bytes_nr == 1) // data binnen zelfde unsigned int op 1 regel
  {
    mask_start &= mask_end;
    for (loop_y = start_y; loop_y > einde_y; loop_y--)
    {
      ptr = &lcd_screen[loop_y][start];
      *ptr = (~(*ptr) & mask_start) | (*ptr & ~mask_start);
    }
  }
  else // waarde verdeeld over meer dan 2 unsigned int op 1 regel
  {
    line_bytes_nr--;
    for (loop_y = start_y; loop_y > einde_y; loop_y--)
    {
      // eerste byte, houd rekening met start_mask
      ptr = &lcd_screen[loop_y][start];
      line_cnt = line_bytes_nr;
      *ptr = (~(*ptr) & mask_start) | (*ptr & ~mask_start);
      ptr--;
      // tussen liggende data bytes indien aanwezig
      while (line_cnt > 1)
      {
        *ptr = ~(*ptr);
        line_cnt--;
        ptr--;
      }
      // laatste byte, houdt rekening met eind_mask
      *ptr = (~(*ptr) & mask_end) | (*ptr & ~mask_end);
    }
  }
}

//*****************************************************************************
void LCD_Draw_Bitmap_Data_Generic(unsigned char x, unsigned char y,
                                  unsigned char dx, unsigned char dy,
                                  unsigned char const *bitmap)
{
int data_one_byte_less_then_lcd;
int start;
int start_bit;
int line_bytes_nr;
int line_cnt;
unsigned char *ptr;
int loop_y;
s_unsigned_char_int chi;
int start_y;
int einde_y;

  x += lcd_x_offset;
  y += lcd_y_offset;
  data_one_byte_less_then_lcd = 0;
  start = 29 - (x / 8);
  start_bit = x % 8;
  start_y = 127 - y;
  einde_y = start_y - dy;
  // bitmap niet afdrukken als deze buiten het scherm valt
  if ((x < 0) ||
      (y < 0) ||
      (dx <= 0) ||
      (dy <= 0) ||
      (x + dx > 240) ||
      (y + dy > 128))
    return;

  line_bytes_nr = dx / 8;
  if (dx % 8)
    line_bytes_nr++;
  if ((8 - start_bit) <= ((dx - 1) % 8))
  {
    line_bytes_nr++;
    data_one_byte_less_then_lcd = 1;
  }
  if (line_bytes_nr == 1) // data binnen zelfde unsigned int op 1 regel
  {
    for (loop_y = start_y; loop_y > einde_y; loop_y--)
      lcd_screen[loop_y][start] |= *bitmap++ << start_bit;
  }
  else // waarde verdeeld over meer dan 2 unsigned int op 1 regel
  {
    line_bytes_nr--;
    for (loop_y = start_y; loop_y > einde_y; loop_y--)
    {
      // eerste byte, houd rekening met start_mask
      ptr = &lcd_screen[loop_y][start];
      line_cnt = line_bytes_nr;
      chi.c[0] = *bitmap++;
      *ptr |= chi.c[0] << start_bit;
      ptr--;
      // tussen liggende data bytes indien aanwezig
      while (line_cnt > 1)
      {
        chi.c[1] = *bitmap++;
        *ptr |= (unsigned char)(chi.i >> (8 - start_bit));
        chi.c[0] = chi.c[1];
        line_cnt--;
        ptr--;
      }
      // laatste byte, houdt rekening met eind_mask
      if (data_one_byte_less_then_lcd)
        *ptr |= chi.c[0] >> (8 - start_bit);
      else
      {
        chi.c[1] = *bitmap++;
        *ptr |= (unsigned char)(chi.i >> (8 - start_bit));
      }
    }
  }
}

void LCD_Draw_Bitmap(unsigned char x, unsigned char y, s_bitmap const *bitmap)
{
  LCD_Draw_Bitmap_Data_Generic(x, y, bitmap->width, bitmap->height, bitmap->pixel);
}

//-----------------------------------------------------------------------------

void LCD_Draw_Character(unsigned char x ,unsigned char y, s_character const *character)
{
  LCD_Draw_Bitmap_Data_Generic(x + character->x_offset, y + character->y_offset,
                               character->b_width, character->b_height, character->pixel);
}

//*****************************************************************************
s_disp_cursor const disp_cursor_56_23_8 =    {  56, 23,   8, 2, 18 };
s_disp_cursor const disp_cursor_57_23_28 =   {  57, 23,  28, 2, 18 };
s_disp_cursor const disp_cursor_80_23_8 =    {  80, 23,   8, 2, 18 };
s_disp_cursor const disp_cursor_88_52_8 =    {  88, 52,   8, 2, 18 };
s_disp_cursor const disp_cursor_88_78_8 =    {  88, 78,   8, 2, 18 };
s_disp_cursor const disp_cursor_91_23_8 =    {  91, 23,   8, 2, 18 };
s_disp_cursor const disp_cursor_106_61_50 =  { 122, 61, 100, 2, 18 };
s_disp_cursor const disp_cursor_127_78_8 =   { 127, 78,   8, 2, 18 };
s_disp_cursor const disp_cursor_135_23_8 =   { 135, 23,   8, 2, 18 };
s_disp_cursor const disp_cursor_142_25_8 =   { 142, 25,   8, 2, 18 };
s_disp_cursor const disp_cursor_147_23_8 =   { 147, 23,   8, 2, 18 };
s_disp_cursor const disp_cursor_153_52_8 =   { 153, 52,   8, 2, 18 };
s_disp_cursor const disp_cursor_153_78_8 =   { 153, 78,   8, 2, 18 };
s_disp_cursor const disp_cursor_147_78_8 =   { 147, 78,   8, 2, 18 };
s_disp_cursor const disp_cursor_150_52_142 = { 150, 52, 142, 2, 18 };
s_disp_cursor const disp_cursor_157_23_28 =  { 157, 23,  28, 2, 18 };
s_disp_cursor const disp_cursor_157_52_8 =   { 157, 52,   8, 2, 18 };
s_disp_cursor const disp_cursor_161_23_8 =   { 161, 23,   8, 2, 18 };
s_disp_cursor const disp_cursor_161_78_8 =   { 161, 78,   8, 2, 18 };
s_disp_cursor const disp_cursor_170_23_8 =   { 170, 23,   8, 2, 18 };
s_disp_cursor const disp_cursor_171_23_8 =   { 171, 23,   8, 2, 18 };
s_disp_cursor const disp_cursor_172_78_8 =   { 172, 78,   8, 2, 18 };
s_disp_cursor const disp_cursor_183_23_8 =   { 183, 23,   8, 2, 18 };
s_disp_cursor const disp_cursor_183_78_8 =   { 183, 78,   8, 2, 18 };
s_disp_cursor const disp_cursor_186_25_150 = { 186, 25, 150, 2, 14 };
s_disp_cursor const disp_cursor_192_52_8 =   { 192, 52,   8, 2, 18 };
s_disp_cursor const disp_cursor_192_78_8 =   { 192, 78,   8, 2, 18 };
s_disp_cursor const disp_cursor_193_61_8 =   { 193, 61,   8, 2, 18 };
s_disp_cursor const disp_cursor_193_81_8 =   { 193, 81,   8, 2, 18 };
s_disp_cursor const disp_cursor_194_23_8 =   { 194, 23,   8, 2, 18 };
s_disp_cursor const disp_cursor_221_25_185 = { 221, 25, 185, 2, 18 };
s_disp_cursor const disp_cursor_207_23_8 =   { 207, 23,   8, 2, 18 };
s_disp_cursor const disp_cursor_207_25_8 =   { 207, 25,   8, 2, 18 };
s_disp_cursor const disp_cursor_207_27_18 =  { 207, 26,  18, 2, 22 };
s_disp_cursor const disp_cursor_207_44_8 =   { 207, 44,   8, 2, 18 };
s_disp_cursor const disp_cursor_207_52_8 =   { 207, 52,   8, 2, 18 };
s_disp_cursor const disp_cursor_207_53_8 =   { 207, 53,   8, 2, 18 };
s_disp_cursor const disp_cursor_207_63_8 =   { 207, 63,   8, 2, 18 };
s_disp_cursor const disp_cursor_207_78_8 =   { 207, 78,   8, 2, 18 };
s_disp_cursor const disp_cursor_207_82_8 =   { 207, 82,   8, 2, 18 };
s_disp_cursor const disp_cursor_210_23_8 =   { 210, 23,   8, 2, 18 };    
s_disp_cursor const disp_cursor_212_23_8 =   { 212, 23,   8, 2, 18 };    
s_disp_cursor const disp_cursor_215_23_8 =   { 215, 23,   8, 2, 18 };
s_disp_cursor const disp_cursor_215_23_70 =  { 215, 23,  90, 2, 14 };
s_disp_cursor const disp_cursor_221_78_45 =  { 221, 78,  45, 2, 18 };
s_disp_cursor const disp_cursor_220_23_8 =   { 220, 23,   8, 2, 18 };
s_disp_cursor const disp_cursor_225_23_120 = { 225, 23, 120, 2, 14 };
s_disp_cursor const disp_cursor_225_23_90 =  { 225, 23,  90, 2, 14 };
s_disp_cursor const disp_cursor_225_78_150 = { 225, 78, 150, 2, 18 };
s_disp_cursor const disp_cursor_225_78_180 = { 225, 78, 180, 2, 18 };

s_disp_cursor const disp_cursor_214_78_151 = { 214, 78, 151, 2, 18 };

s_disp_cursor const disp_cursor_checkbox_207_25 = { 207, 25,  18, 2, 22 };

s_disp_cursor const disp_cursor_checkbox =     { 207, 78,  18, 2, 22 };
s_disp_cursor const disp_cursor_curve_dag =    {  39, 24,   8, 2, 17 };
s_disp_cursor const disp_cursor_curve_val =    { 101, 24,   8, 2, 17 };
s_disp_cursor const disp_cursor_curve_hour =   {  71, 24,   8, 2, 17 };
s_disp_cursor const disp_cursor_curve_min =    {  95, 24,   8, 2, 17 }; 
s_disp_cursor const disp_cursor_curve_aantal = { 150, 24,   8, 2, 17 };
//s_disp_cursor const disp_cursor_password =     { 133, 25,  14, 2, 18 };
//s_disp_cursor const disp_cursor_password_1 =   { 133, 53,  14, 2, 18 };
//s_disp_cursor const disp_cursor_password_2 =   { 133, 78,  14, 2, 18 };
s_disp_cursor const disp_cursor_password =     {  96, 43,  14, 2, 18 };
s_disp_cursor const disp_cursor_password_1 =   { 103, 43,  14, 2, 18 };
s_disp_cursor const disp_cursor_password_2 =   { 103, 68,  14, 2, 18 };

s_disp_cursor const disp_cursor_operation = { 217, 23, 48, 2, 14 };

void Disp_Cursor(s_disp_cursor const *ptr)
{
// s_disp_cursor const *ptr = screen_ptr->rel[screen_ptr->rel_actief].key_action->cursor;

  if ((ptr != 0) && (ptr->x != 0x255))
  {
    if (screen_ptr->change_flag == 0)
    {
      if (ptr->dy2)
      {
        if (cursor_state && !block_display_cursor_block)
          LCD_Draw_Black_Block_Abs(ptr->x + 1 - ptr->dx, ptr->y + 1 - ptr->dy2, ptr->x, ptr->y + 1 - ptr->dy1);
        LCD_Draw_Black_Block_Abs(ptr->x + 1 - ptr->dx, ptr->y + 1 - ptr->dy1, ptr->x, ptr->y);
      }
      else if (cursor_state)
        LCD_Draw_Black_Block_Abs(ptr->x + 1 - ptr->dx, ptr->y + 1 - ptr->dy1, ptr->x, ptr->y);
    }
    else
      LCD_Draw_Black_Block_Abs(ptr->x + 1 - ptr->dx, ptr->y + 1 - ptr->dy1, ptr->x, ptr->y);
  }
}

void Disp_Cursor_PC(s_disp_cursor const *ptr)
{
  if ((ptr != 0) && (ptr->x != 0x255))
  {
    LCD_Draw_Black_Block_Abs_PC(ptr->x + 1 - ptr->dx, ptr->y + 1 - ptr->dy1, ptr->x, ptr->y);
  }
}

//-----------------------------------------------------------------------------
static void Disp_Draw_String_Func(e_size draw_size, char const *ch_ptr)
{
int cnt;

  if (draw_size & RECHTS)
  {
    // rechts uitlijnen
    switch (draw_size & ~RECHTS)
    {
      default: 
      case SIZE_7:      draw_char = LCD_Draw_ASCII_Multi_7_R;  break;
      case SIZE_10:     draw_char = LCD_Draw_ASCII_Multi_10_R; break;
      case SIZE_14:     draw_char = LCD_Draw_ASCII_Multi_14_R; break;
      case SIZE_20:     draw_char = LCD_Draw_ASCII_Multi_20_R; break;
      case SIZE_RUS_7:  draw_char = LCD_Draw_ASCII_Rus_7_R;    break;
      case SIZE_RUS_10: draw_char = LCD_Draw_ASCII_Rus_10_R;   break;
      case SIZE_RUS_14: draw_char = LCD_Draw_ASCII_Rus_14_R;   break;
      case SIZE_RUS_20: draw_char = LCD_Draw_ASCII_Rus_20_R;   break;
      case SIZE_EE_7:   draw_char = LCD_Draw_ASCII_EE_7_R;     break;
      case SIZE_EE_10:  draw_char = LCD_Draw_ASCII_EE_10_R;    break;
      case SIZE_EE_14:  draw_char = LCD_Draw_ASCII_EE_14_R;    break;
      case SIZE_EE_20:  draw_char = LCD_Draw_ASCII_EE_20_R;    break;
    }
    cnt = strlen((char *)ch_ptr);
    ch_ptr = ch_ptr + cnt - 1;
    while (cnt)
    {
      (*draw_char)(*ch_ptr);
      ch_ptr--;
      cnt--;
    }
  }
  else
  {
    // links uitlijnen
    switch (draw_size)
    {
      default: 
      case SIZE_7:      draw_char = LCD_Draw_ASCII_Multi_7_L;  break;
      case SIZE_10:     draw_char = LCD_Draw_ASCII_Multi_10_L; break;
      case SIZE_14:     draw_char = LCD_Draw_ASCII_Multi_14_L; break;
      case SIZE_20:     draw_char = LCD_Draw_ASCII_Multi_20_L; break;
      case SIZE_RUS_7:  draw_char = LCD_Draw_ASCII_Rus_7_L;    break;
      case SIZE_RUS_10: draw_char = LCD_Draw_ASCII_Rus_10_L;   break;
      case SIZE_RUS_14: draw_char = LCD_Draw_ASCII_Rus_14_L;   break;
      case SIZE_RUS_20: draw_char = LCD_Draw_ASCII_Rus_20_L;   break;
      case SIZE_EE_7:   draw_char = LCD_Draw_ASCII_EE_7_L;     break;
      case SIZE_EE_10:  draw_char = LCD_Draw_ASCII_EE_10_L;    break;
      case SIZE_EE_14:  draw_char = LCD_Draw_ASCII_EE_14_L;    break;
      case SIZE_EE_20:  draw_char = LCD_Draw_ASCII_EE_20_L;    break;
    }
    while (*ch_ptr)
    {
      (*draw_char)(*ch_ptr);
      ch_ptr++;
    }
  }
}

//-----------------------------------------------------------------------------

void Disp_Draw_Bitmap(void *s)
{
s_disp_bitmap *ptr;

  ptr = s;
  LCD_Draw_Bitmap(ptr->x,ptr->y,ptr->data);
}

void Disp_Draw_Bitmap_Option_On(void *s)
{
s_disp_bitmap_option_on *ptr;

  ptr = s;
  if (Return_Value(ptr->option_type, Get_Ptr(ptr->option, lcd_rel_disp_index)))
    LCD_Draw_Bitmap(ptr->x,ptr->y,ptr->data);
}

//void Disp_Draw_Bitmap_Option_Off(void *s)
//{
//s_disp_bitmap_option_on *ptr;
//
//  ptr = s;
//  if (!Return_Value(ptr->option_type, Get_Ptr(ptr->option, lcd_rel_disp_index)))
//    LCD_Draw_Bitmap(ptr->x,ptr->y,ptr->data);
//}

void Disp_Draw_Bitmap_Array(void *s)
{
s_disp_bitmap_array *ptr;
long index;
  
  ptr = s;
  index = Return_Value(ptr->type, Get_Ptr(ptr->index, lcd_rel_disp_index));
  if (index < 0)
    index = -index;
  if ((long)ptr->index == (long)screen_ptr->rel[screen_ptr->rel_actief].key_action->value->value)
  {
    if (screen_ptr->change_flag)
     index = screen_ptr->value;
  }
  if (index >= ptr->max)
    index = ptr->max - 1;
  LCD_Draw_Bitmap(ptr->x,ptr->y,ptr->data_array[index]);
}

void Disp_Draw_Bitmap_Array_Option_On(void *s)
{
s_disp_bitmap_array_option_on *ptr;

  ptr = s;
  if (Return_Value(ptr->option_type, Get_Ptr(ptr->option, lcd_rel_disp_index)))
    Disp_Draw_Bitmap_Array(s);
}

//void Disp_Draw_Bitmap_Array_Option_Off(void *s)
//{
//s_disp_bitmap_array_option_on *ptr;
//
//  ptr = s;
//  if (!Return_Value(ptr->option_type, Get_Ptr(ptr->option, lcd_rel_disp_index)))
//    Disp_Draw_Bitmap_Array(s);
//}

void Disp_Draw_Bitmap_Option_On_Invert(void *s)
// diagnose geeft aan of ingang of uitgang ingeschakeld is
// option uit niets afdrukken
// anders invert true inverteren anders normaal afdrukken
{
s_disp_bitmap_option_on_invert *ptr;
long val;

  ptr = s;
  val = Return_Value(ptr->option_type, Get_Ptr(ptr->option_value, lcd_rel_disp_index));
  if (val) // option is true
  {
    LCD_Set_Cursor(ptr->x + ptr->data->width, ptr->y);
    val = Return_Value(ptr->invert_type, Get_Ptr(ptr->invert_value, lcd_rel_disp_index));
    LCD_Draw_Bitmap(ptr->x, ptr->y, ptr->data);
    if (val)
      LCD_Invert_Block(ptr->x+1, ptr->y+1, ptr->x + ptr->data->width - 2, ptr->y + ptr->data->height - 2);
  }
  else 
    LCD_Set_Cursor(ptr->x, ptr->y);
}

void Disp_Draw_Bitmap_Invert(void *s)
// invert true inverteren anders normaal afdrukken
{
s_disp_bitmap_invert *ptr;
long val;

  ptr = s;
  LCD_Set_Cursor(ptr->x + ptr->data->width, ptr->y);
  val = Return_Value(ptr->invert_type, Get_Ptr(ptr->invert_value, lcd_rel_disp_index));
  LCD_Draw_Bitmap(ptr->x, ptr->y, ptr->data);
  if (val)
    LCD_Invert_Block(ptr->x+1, ptr->y+1, ptr->x + ptr->data->width - 2, ptr->y + ptr->data->height - 2);
}

void Disp_Draw_Bitmap_Option_On_Invert_Add(void *s)
{
s_disp_bitmap_option_on_invert_add *ptr;
long val;

  ptr = s;
  val = Return_Value(ptr->option_type, Get_Ptr(ptr->option_value, lcd_rel_disp_index));
  if (val) // option is true
  {
    val = Return_Value(ptr->invert_type, Get_Ptr(ptr->invert_value, lcd_rel_disp_index));
    LCD_Draw_Bitmap(lcd_cursor_x, lcd_cursor_y, ptr->data);
    if (val)
      LCD_Invert_Block(lcd_cursor_x+1, lcd_cursor_y+1, lcd_cursor_x + ptr->data->width - 2, lcd_cursor_y + ptr->data->height - 2);
    LCD_Set_Cursor(lcd_cursor_x + ptr->data->width, lcd_cursor_y);
  }
}

//-----------------------------------------------------------------------------
void Disp_Draw_Value_Func(unsigned char punt, e_size size, e_type type, void *value)
{
unsigned char change_value = 0;
unsigned char min;
unsigned char point;
unsigned char zero = 1;
unsigned char nr_digits = 0;
char val_str[15] = "";
char *ch_ptr;
// int cnt;
long val;

  val = Return_Value(type, Get_Ptr(value, lcd_rel_disp_index));
  if ((long)value == (long)screen_ptr->rel[screen_ptr->rel_actief].key_action->value->value) 
  {
    change_value = 1;
    if (screen_ptr->change_flag)
      val = screen_ptr->value;
    else
      screen_ptr->norm_point = screen_ptr->point = punt;
    point = screen_ptr->point;
  }
  else
    point = punt;

  val_str[14] = 0;
  ch_ptr = &val_str[14];
  switch (type)
  {
    default:
    case CHAR:
    case UCHAR:
    case INT:
    case UINT:
    case LONG:
      if (val < 0)
      {
        min = 1;
        val = -val;
      }
      else
        min = 0;
      do
      {
        ch_ptr--;
        *ch_ptr = (val % 10) + '0';
        nr_digits++;
        val /= 10;
        if (point)
        {
          point--;
          if (point == 0)
          {
            ch_ptr--;
            *ch_ptr = '.';
            if (val == 0)
            {
              ch_ptr--;
              *ch_ptr = '0';
              nr_digits++;
            }
          }
        }
      }
      while (val || point);
      if (min)
      {
        ch_ptr--;
        *ch_ptr = '-';
        nr_digits++;
      }
      break;
    case TIME_CHAR:
    case TIME_INT:
      ch_ptr--;
      *ch_ptr = (val % 10) + '0';
      ch_ptr--;
      *ch_ptr = (val / 10) + '0';
      nr_digits = (val / 10) ? 2 : 1;
      break;
    case HEX_CHAR:
      do
      {
        ch_ptr--;
        *ch_ptr = Hex_To_Asc(val & 0x0F);
        nr_digits++;
        val >>= 4;
	  }
      while (val || (nr_digits < 2));
	  break;
    case HEX_INT:
      do
      {
        ch_ptr--;
        *ch_ptr = Hex_To_Asc(val & 0x000F);
        nr_digits++;
        val >>= 4;
	  }
      while (val || (nr_digits < 4));
	  break;
    case HEX_LONG:
      do
      {
        ch_ptr--;
        *ch_ptr = Hex_To_Asc(val & 0x0000000F);
        nr_digits++;
        val >>= 4;
	  }
      while (val || (nr_digits < 8));
	  break;
  }
  if (change_value)
    screen_ptr->nr_digits = nr_digits;
  Disp_Draw_String_Func(size, ch_ptr);
}

void Disp_Draw_Value_Signed_Func(unsigned char punt, e_size size, e_type type, void *value)
{
unsigned char change_value = 0;
unsigned char min;
unsigned char point;
unsigned char zero = 1;
unsigned char nr_digits = 0;
char val_str[15] = "";
char *ch_ptr;
// int cnt;
long val;

  val = Return_Value(type, Get_Ptr(value, lcd_rel_disp_index));
  if ((long)value == (long)screen_ptr->rel[screen_ptr->rel_actief].key_action->value->value) 
  {
    change_value = 1;
    if (screen_ptr->change_flag)
      val = screen_ptr->value;
    else
      screen_ptr->norm_point = screen_ptr->point = punt;
    point = screen_ptr->point;
  }
  else
    point = punt;

  val_str[14] = 0;
  ch_ptr = &val_str[14];
  switch (type)
  {
    default:
    case CHAR:
    case UCHAR:
    case INT:
    case UINT:
    case LONG:
      if (val < 0)
      {
        min = 1;
        val = -val;
      }
      else
        min = 0;
      do
      {
        ch_ptr--;
        *ch_ptr = (val % 10) + '0';
        nr_digits++;
        val /= 10;
        if (point)
        {
          point--;
          if (point == 0)
          {
            ch_ptr--;
            *ch_ptr = '.';
            if (val == 0)
            {
              ch_ptr--;
              *ch_ptr = '0';
              nr_digits++;
            }
          }
        }
      }
      while (val || point);
      if (min)
      {
        ch_ptr--;
        *ch_ptr = '-';
        nr_digits++;
      }
      else
      {
        ch_ptr--;
        *ch_ptr = '+';
        nr_digits++;
      }
      break;
    case TIME_CHAR:
    case TIME_INT:
      ch_ptr--;
      *ch_ptr = (val % 10) + '0';
      ch_ptr--;
      *ch_ptr = (val / 10) + '0';
      nr_digits = (val / 10) ? 2 : 1;
      break;
    case HEX_CHAR:
      do
      {
        ch_ptr--;
        *ch_ptr = Hex_To_Asc(val & 0x0F);
        nr_digits++;
        val >>= 4;
	  }
      while (val || (nr_digits < 2));
	  break;
    case HEX_INT:
      do
      {
        ch_ptr--;
        *ch_ptr = Hex_To_Asc(val & 0x000F);
        nr_digits++;
        val >>= 4;
	  }
      while (val || (nr_digits < 4));
	  break;
    case HEX_LONG:
      do
      {
        ch_ptr--;
        *ch_ptr = Hex_To_Asc(val & 0x0000000F);
        nr_digits++;
        val >>= 4;
	  }
      while (val || (nr_digits < 8));
	  break;
  }
  if (change_value)
    screen_ptr->nr_digits = nr_digits;
  Disp_Draw_String_Func(size, ch_ptr);
}

void Disp_Draw_Value(void *s)
{
s_disp_value *ptr;

  ptr = s;
  LCD_Set_Cursor(ptr->x+1, ptr->y);
  Disp_Draw_Value_Func(ptr->point,ptr->size,ptr->type,ptr->value);
}

void Disp_Draw_Value_Signed(void *s)
{
s_disp_value *ptr;

  ptr = s;
  LCD_Set_Cursor(ptr->x+1, ptr->y);
  Disp_Draw_Value_Signed_Func(ptr->point,ptr->size,ptr->type,ptr->value);
}

void Disp_Draw_Value_Add(void *s)
{
s_disp_value_add *ptr;

  ptr = s;
  Disp_Draw_Value_Func(ptr->point,ptr->size,ptr->type,ptr->value);
}

void Disp_Draw_Value_Option_On(void *s)
{
s_disp_value_option_on *ptr;

  ptr = s;
  if (Return_Value(ptr->option_type, Get_Ptr(ptr->option, lcd_rel_disp_index)))
  {
    LCD_Set_Cursor(ptr->x+1, ptr->y);
    Disp_Draw_Value_Func(ptr->point,ptr->size,ptr->type,ptr->value);
  }
}

void Disp_Draw_Value_Option_Off(void *s)
{
s_disp_value_option_on *ptr;

  ptr = s;
  if (!Return_Value(ptr->option_type, Get_Ptr(ptr->option, lcd_rel_disp_index)))
  {
    LCD_Set_Cursor(ptr->x+1, ptr->y);
    Disp_Draw_Value_Func(ptr->point,ptr->size,ptr->type,ptr->value);
  }
}

void Disp_Draw_Value_Add_Option_On(void *s)
{
s_disp_value_add_option_on *ptr;

  ptr = s;
  if (Return_Value(ptr->option_type, Get_Ptr(ptr->option, lcd_rel_disp_index)))
    Disp_Draw_Value_Func(ptr->point,ptr->size,ptr->type,ptr->value);
}

//-----------------------------------------------------------------------------
void  Disp_Draw_Value_2_Size_No_Point_Func(unsigned char punt, e_size size_1, e_size size_2, e_type type, void *value)
// punt geeft overgang aan tussen de letter groottes geteld vanaf achteren
{
unsigned char change_value = 0;
unsigned char min;
unsigned char zero = 1;
unsigned char nr_digits = 0;
char val_1_str[15] = "";
char val_2_str[15] = "";
char *ch_1_ptr;
char *ch_2_ptr;
// int cnt;
long val;

  val = Return_Value(type, Get_Ptr(value, lcd_rel_disp_index));
  if ((long)value == (long)screen_ptr->rel[screen_ptr->rel_actief].key_action->value->value) 
  {
    change_value = 1;
    if (screen_ptr->change_flag)
      val = screen_ptr->value;
  }

  val_1_str[14] = 0;
  ch_1_ptr = &val_1_str[14];
  val_2_str[14] = 0;
  ch_2_ptr = &val_2_str[14];
  if (val < 0)
  {
    min = 1;
    val = -val;
  }
  else
    min = 0;
  do
  {
    if (nr_digits < punt)
    {
      ch_2_ptr--;
      *ch_2_ptr = (val % 10) + '0';
      nr_digits++;
      val /= 10;
    }
    else
    {
      ch_1_ptr--;
      *ch_1_ptr = (val % 10) + '0';
      nr_digits++;
      val /= 10;
    }
  }
  while (val);
  if (min)
  {
    ch_1_ptr--;
    *ch_1_ptr = '-';
    nr_digits++;
  }
  if (change_value)
    screen_ptr->nr_digits = nr_digits;
  if (nr_digits > punt+1)
  {
    Disp_Draw_String_Func(size_2, ch_2_ptr);
    Disp_Draw_String_Func(size_1, ch_1_ptr);
  }
  else
  {
    Disp_Draw_String_Func(size_1, ch_2_ptr);
    Disp_Draw_String_Func(size_1, ch_1_ptr);
  }
}

void Disp_Draw_Value_2_Size_No_Point(void *s)
{
s_disp_value_2_size_no_point *ptr;

  ptr = s;
  LCD_Set_Cursor(ptr->x+1, ptr->y);
  Disp_Draw_Value_2_Size_No_Point_Func(ptr->point, ptr->size_1, ptr->size_2, ptr->type, ptr->value);
}

//-----------------------------------------------------------------------------
void Disp_Draw_Value_2_Size_Func(unsigned char nr_digits, unsigned char punt, e_size size_1, e_size size_2, e_type type, void *value)
{
long val;
long val_size = 1;
int i;

  for (i = 1; i < nr_digits; i++)
    val_size *= 10;

  val = Return_Value(type, Get_Ptr(value, lcd_rel_disp_index));
  if ((nr_digits != 0) && ((val / val_size) == 0))
    Disp_Draw_Value_Func(punt,size_1,type,value);
  else
  {
    lcd_cursor_x += 2;
    Disp_Draw_Value_Func(punt,size_2,type,value);
  }
}

void Disp_Draw_Value_2_Size_Option_On(void *s)
{
s_disp_value_2_size_option_on *ptr;

  ptr = s;
  if (Return_Value(ptr->option_type, Get_Ptr(ptr->option, lcd_rel_disp_index)))
  {
    LCD_Set_Cursor(ptr->x+1, ptr->y);
    Disp_Draw_Value_2_Size_Func(ptr->nr_digits, ptr->point, ptr->size_1, ptr->size_2, ptr->type, ptr->value);
  }
}

//-----------------------------------------------------------------------------

void Disp_Draw_Black_Block(void *s)
{
s_disp_block *ptr;

  ptr = s;
  LCD_Draw_Black_Block_Rel(ptr->x1, ptr->y1, ptr->x2, ptr->y2);
}

void Disp_Draw_Black_Vierkant(void *s)
{
s_disp_block *ptr;

  ptr = s;
  LCD_Draw_Black_Vierkant(ptr->x1, ptr->y1, ptr->x2, ptr->y2);
}

//-----------------------------------------------------------------------------

void Disp_Draw_White_Block(void *s)
{
s_disp_block *ptr;

  ptr = s;
  LCD_Draw_White_Block(ptr->x1, ptr->y1, ptr->x2, ptr->y2);
}

void Disp_Draw_White_Vierkant(void *s)
{
s_disp_block *ptr;

  ptr = s;
  LCD_Draw_White_Block(ptr->x1, ptr->y1, ptr->x2, ptr->y2);
}

//-----------------------------------------------------------------------------

void Disp_Invert_Block(void *s)
{
s_disp_block *ptr;

  ptr = s;
  if (ptr->x1 != 255)
    LCD_Invert_Block(ptr->x1, ptr->y1, ptr->x2, ptr->y2);
}

//-----------------------------------------------------------------------------

void Disp_Draw_Loper(void *s)
{
s_disp_loper *ptr;
int nr_actief = screen_ptr->nr_actief;
int nr_aantal = screen_ptr->nr_aantal;
int rel_actief = screen_ptr->rel_actief;
int rel_max = screen_ptr->rel_max;
int blok_begin;
int blok_einde;
int blok_grootte;

  ptr = s;
  if (nr_aantal > rel_max)
  {
    blok_grootte = (rel_max * LOPER_SIZE + nr_aantal / 2) / nr_aantal;
    if (blok_grootte < 2)
      blok_grootte = 2;
    blok_begin = ((LOPER_SIZE - blok_grootte) * (nr_actief - rel_actief) + (nr_aantal - rel_max) / 2) / (nr_aantal - rel_max);
    blok_einde = blok_begin + blok_grootte;
    if (blok_einde > LOPER_SIZE)
      blok_einde = LOPER_SIZE;
    if (blok_einde - blok_begin < 2)
      blok_begin = blok_einde - 2;
    LCD_Draw_Black_Block_Rel(231, 13, 231,104); // vertikaal 
    LCD_Draw_Black_Block_Rel(232,104, 238,104); 
    LCD_Draw_Bitmap(233, 18,&ico_arrow_up_0);
    LCD_Draw_Bitmap(233, 100,&ico_arrow_down_0);
    LCD_Draw_Black_Block_Rel(233,   22 + blok_begin, 237, 22 + blok_einde);
  } 
}

s_disp_loper const disp_loper = { Disp_Draw_Loper };

//-----------------------------------------------------------------------------
void Disp_Draw_Code(void *s)
{
unsigned char nr_digits = 0;
s_disp_value *ptr;
long val;

  ptr = s;
  // laad val met waarde die weergegeven moet worden
  val = Return_Value(ptr->type, Get_Ptr(ptr->value, lcd_rel_disp_index));
  if (((long)ptr->value == (long)screen_ptr->rel[screen_ptr->rel_actief].key_action->value->value) &&
      screen_ptr->change_flag)
    val = screen_ptr->value;
  
  LCD_Set_Cursor(ptr->x, ptr->y);
  while (val)
  {
    LCD_Draw_Bitmap(lcd_cursor_x, lcd_cursor_y, &ico_14_code);
    lcd_cursor_x += 16;
    val /= 10;
    nr_digits++;
  }
  screen_ptr->nr_digits = nr_digits;
}

//-----------------------------------------------------------------------------

void Disp_Draw_Component(void *s)
{
s_disp_data_component *ptr;
void **disp_ptr;

  ptr = s;
  disp_ptr = ptr->disp;
  while (*disp_ptr)
  {
    ((void (*)(void *))(*(long *)*disp_ptr))(*disp_ptr);
    disp_ptr++;
  }
}



void Disp_Draw_Component_Option_On(void *s)
{
s_disp_data_component_option_on *ptr;

  ptr = s;
  if (Return_Value(ptr->option_type, ptr->option))
    Disp_Draw_Component(ptr);
}

void Disp_Draw_Component_Array(void *s)
{
s_disp_data_component_array *ptr;
long index;
void **disp_ptr;
  
  ptr = s;
  index = Return_Value(ptr->type, Get_Ptr(ptr->index, lcd_rel_disp_index));
  if ((long)ptr->index == (long)screen_ptr->rel[screen_ptr->rel_actief].key_action->value->value)
  {
    if (screen_ptr->change_flag)
    index = screen_ptr->value;
  }
  if (index < 0)
    index = -index;
  if (index >= ptr->max)
    index = ptr->max - 1;
  disp_ptr = ptr->disp_array;
  while(index)
  {
    index--;
    disp_ptr++;
  }
  if(*disp_ptr)
  {
    ((void (*)(void *))(*(long *)*disp_ptr))(*disp_ptr);
  }
}

void Disp_Draw_Component_Array_Option_On(void *s)
{
s_disp_data_component_array_option_on *ptr;
long index;
void **disp_ptr;
  
  ptr = s;
  if (Return_Value(ptr->option_type, ptr->option)) // option is true
  {
    index = Return_Value(ptr->type, ptr->index);
    if ((long)ptr->index == (long)screen_ptr->rel[screen_ptr->rel_actief].key_action->value->value)
    {
      if (screen_ptr->change_flag)
      index = screen_ptr->value;
    }
    if (index < 0)
      index = -index;
    if (index >= ptr->max)
      index = ptr->max - 1;
    disp_ptr = ptr->disp_array;
    while(index)
    {
      index--;
      disp_ptr++;
    }
    if(*disp_ptr)
    {
      ((void (*)(void *))(*(long *)*disp_ptr))(*disp_ptr);
    }
  }
}

void Disp_Draw_Component_Abs(void *s)
{
s_disp_data_component *ptr;
void **disp_ptr;

  ptr = s;
  disp_ptr = ptr->disp;
  LCD_Set_Offset(0,0);
//  LCD_Set_Offset(screen_ptr->rel[0].x, screen_ptr->rel[0].y);
  while (*disp_ptr)
  {
    ((void (*)(void *))(*(long *)*disp_ptr))(*disp_ptr);
    disp_ptr++;
  }
}

void Disp_Draw_Component_Abs_Option_On(void *s)
{
s_disp_data_component_option_on *ptr;

  ptr = s;
  if (Return_Value(ptr->option_type, ptr->option))
    Disp_Draw_Component_Abs(ptr);
}
//-----------------------------------------------------------------------------

void Disp_Draw_Balk(void *s)
{
s_disp_balk *ptr;
long val;
long min_val;
long max_val;
unsigned char grootte;

  ptr = s;
  if ((ptr->x2 - ptr->x1 < 4) ||
      (ptr->y2 - ptr->y1 < 4))
    return;    
  val = Return_Value(ptr->type, Get_Ptr(ptr->value, lcd_rel_disp_index));
  min_val = Return_Value(ptr->type, ptr->min_value);
  max_val = Return_Value(ptr->type, ptr->max_value);
  LCD_Draw_Black_Vierkant(ptr->x1, ptr->y1, ptr->x2, ptr->y2);
  if (val < min_val)
    return;
  if ((val > max_val) ||
      (min_val == max_val))
  {
    LCD_Draw_Black_Block_Rel(ptr->x1, ptr->y1, ptr->x2, ptr->y2);
    return;
  }
  switch (ptr->richting)
  {
    case HORIZONTAAL_LINKS_RECHTS:
      grootte = (ptr->x2 - ptr->x1 - 3 - 1) * (val - min_val) / (max_val - min_val);
      if (val > min_val) 
        LCD_Draw_Black_Block_Rel(ptr->x1 + 2, ptr->y1 + 2, ptr->x1 + 2 + grootte , ptr->y2 - 2);
      break;
    case HORIZONTAAL_RECHTS_LINKS:
      grootte = (ptr->x2 - ptr->x1 - 3 - 1) * (val - min_val) / (max_val - min_val);
      if (val > min_val) 
        LCD_Draw_Black_Block_Rel(ptr->x2 - grootte - 2, ptr->y1 + 2, ptr->x2 - 2 , ptr->y2 - 2);
      break;
    case VERTIKAAL_BOVEN_ONDER:
      grootte = (ptr->y2 - ptr->y1 - 3 - 1) * (val - min_val) / (max_val - min_val);
      if (val > min_val) 
        LCD_Draw_Black_Block_Rel(ptr->x1 + 2, ptr->y1 + 2, ptr->x2 - 2 , ptr->y1 + 2 + grootte);
      break;
    case VERTIKAAL_ONDER_BOVEN:
      grootte = (ptr->y2 - ptr->y1 - 3 - 1) * (val - min_val) / (max_val - min_val);
      if (val > min_val) 
        LCD_Draw_Black_Block_Rel(ptr->x1 + 2, ptr->y2 - grootte - 2, ptr->x2 - 2 , ptr->y2 - 2);
      break;
  }
}

//-----------------------------------------------------------------------------

void Disp_Draw_Time(void *s)
{
s_disp_time *ptr;
struct tm *tm_ptr;
int val;
void (*draw_char)(char c);
void (*draw_char_sec)(char c);
unsigned char offset_dp; // voor plaatsing dubbele punt
unsigned char offset_sec; // voor plaatsing seconde

  ptr = s;
  if (*ptr->time == 0)
    return;

  switch (ptr->size & MASK_SIZE)
  {
    default: 
//    case SIZE_7:  draw_char = LCD_Draw_ASCII_Multi_7_L;  draw_char_sec = LCD_Draw_ASCII_Multi_7_L;  offset_dp = 1; offset_sec = 0; break;
    case SIZE_10: draw_char = LCD_Draw_ASCII_Multi_10_L; draw_char_sec = LCD_Draw_ASCII_Multi_7_L;  offset_dp = 1; offset_sec = 3; break;
    case SIZE_14: draw_char = LCD_Draw_ASCII_Multi_14_L; draw_char_sec = LCD_Draw_ASCII_Multi_7_L;  offset_dp = 2; offset_sec = 7; break;
//    case SIZE_20: draw_char = LCD_Draw_ASCII_Multi_20_L; draw_char_sec = LCD_Draw_ASCII_Multi_10_L; offset_dp = 2; offset_sec = 9; break;
//    case SIZE_RUS_7:  draw_char = LCD_Draw_ASCII_Rus_7_L;  draw_char_sec = LCD_Draw_ASCII_Rus_7_L;  offset_dp = 1; offset_sec = 0; break;
    case SIZE_RUS_10: draw_char = LCD_Draw_ASCII_Rus_10_L; draw_char_sec = LCD_Draw_ASCII_Rus_7_L;  offset_dp = 1; offset_sec = 3; break;
    case SIZE_RUS_14: draw_char = LCD_Draw_ASCII_Rus_14_L; draw_char_sec = LCD_Draw_ASCII_Rus_7_L;  offset_dp = 2; offset_sec = 7; break;
//    case SIZE_RUS_20: draw_char = LCD_Draw_ASCII_Rus_20_L; draw_char_sec = LCD_Draw_ASCII_Rus_10_L; offset_dp = 2; offset_sec = 9; break;
//    case SIZE_EE_7:  draw_char = LCD_Draw_ASCII_EE_7_L;  draw_char_sec = LCD_Draw_ASCII_EE_7_L;  offset_dp = 1; offset_sec = 0; break;
    case SIZE_EE_10: draw_char = LCD_Draw_ASCII_EE_10_L; draw_char_sec = LCD_Draw_ASCII_EE_7_L;  offset_dp = 1; offset_sec = 3; break;
    case SIZE_EE_14: draw_char = LCD_Draw_ASCII_EE_14_L; draw_char_sec = LCD_Draw_ASCII_EE_7_L;  offset_dp = 2; offset_sec = 7; break;
//    case SIZE_EE_20: draw_char = LCD_Draw_ASCII_EE_20_L; draw_char_sec = LCD_Draw_ASCII_EE_10_L; offset_dp = 2; offset_sec = 9; break;
  }
  LCD_Set_Cursor(ptr->x+1, ptr->y);
  tm_ptr = gmtime(ptr->time);
  val = tm_ptr->tm_hour;
  (*draw_char)((val / 10) + '0');
  (*draw_char)((val % 10) + '0');
  lcd_cursor_y -= offset_dp;
  (*draw_char)(':');
  lcd_cursor_y += offset_dp;
  val = tm_ptr->tm_min;
  (*draw_char)((val / 10) + '0');
  (*draw_char)((val % 10) + '0');

  lcd_cursor_y -= offset_sec;
  val = tm_ptr->tm_sec;
  (*draw_char_sec)((val / 10) + '0');
  (*draw_char_sec)((val % 10) + '0');
  lcd_cursor_y += offset_sec;

  (*draw_char)(' ');
  (*draw_char)(' ');
  (*draw_char)(' ');
  val = tm_ptr->tm_mday;
  (*draw_char)((val / 10) + '0');
  (*draw_char)((val % 10) + '0');
  lcd_cursor_y -= offset_dp;
  (*draw_char)('-');
  lcd_cursor_y += offset_dp;
  val = tm_ptr->tm_mon + 1;
  (*draw_char)((val / 10) + '0');
  (*draw_char)((val % 10) + '0');
  lcd_cursor_y -= offset_dp;
  (*draw_char)('-');
  lcd_cursor_y += offset_dp;
  val = tm_ptr->tm_year + 1900;
  (*draw_char)((val / 1000) + '0');
  val %= 1000;
  (*draw_char)((val / 100) + '0');
  val %= 100;
  (*draw_char)((val / 10) + '0');
  (*draw_char)((val % 10) + '0');
}

//-----------------------------------------------------------------------------

void Disp_Draw_Urenteller(void *s)
{
s_disp_time *ptr;
int val;
unsigned long time;
void (*draw_char)(char c);
void (*draw_char_sec)(char c);
unsigned char offset_dp; // voor plaatsing dubbele punt
unsigned char offset_sec; // voor plaatsing seconde

  ptr = s;
  time = *(time_t *)Get_Ptr((void *)ptr->time, lcd_rel_disp_index);
/* TD - 07-09-07
  if ((long)ptr->time == (long)screen_ptr->rel[screen_ptr->rel_actief].key_action->value->value)
  {
    if (screen_ptr->change_flag)
      time = screen_ptr->value;
  }
*/
  switch (ptr->size)
  {
    default: 
//    case (SIZE_7 | RECHTS):  draw_char = LCD_Draw_ASCII_Multi_7_R;  draw_char_sec = LCD_Draw_ASCII_Multi_7_R;  offset_dp = 1; offset_sec = 0; break;
    case (SIZE_10 | RECHTS): draw_char = LCD_Draw_ASCII_Multi_10_R; draw_char_sec = LCD_Draw_ASCII_Multi_7_R;  offset_dp = 1; offset_sec = 3; break;
    case (SIZE_14 | RECHTS): draw_char = LCD_Draw_ASCII_Multi_14_R; draw_char_sec = LCD_Draw_ASCII_Multi_7_R;  offset_dp = 2; offset_sec = 7; break;
//    case (SIZE_20 | RECHTS): draw_char = LCD_Draw_ASCII_Multi_20_R; draw_char_sec = LCD_Draw_ASCII_Multi_10_R; offset_dp = 2; offset_sec = 9; break;
//    case (SIZE_RUS_7 | RECHTS):  draw_char = LCD_Draw_ASCII_Rus_7_R;  draw_char_sec = LCD_Draw_ASCII_Rus_7_R;  offset_dp = 1; offset_sec = 0; break;
    case (SIZE_RUS_10 | RECHTS): draw_char = LCD_Draw_ASCII_Rus_10_R; draw_char_sec = LCD_Draw_ASCII_Rus_7_R;  offset_dp = 1; offset_sec = 3; break;
    case (SIZE_RUS_14 | RECHTS): draw_char = LCD_Draw_ASCII_Rus_14_R; draw_char_sec = LCD_Draw_ASCII_Rus_7_R;  offset_dp = 2; offset_sec = 7; break;
//    case (SIZE_RUS_20 | RECHTS): draw_char = LCD_Draw_ASCII_Rus_20_R; draw_char_sec = LCD_Draw_ASCII_Rus_10_R; offset_dp = 2; offset_sec = 9; break;
//    case (SIZE_EE_7 | RECHTS):  draw_char = LCD_Draw_ASCII_EE_7_R;  draw_char_sec = LCD_Draw_ASCII_EE_7_R;  offset_dp = 1; offset_sec = 0; break;
    case (SIZE_EE_10 | RECHTS): draw_char = LCD_Draw_ASCII_EE_10_R; draw_char_sec = LCD_Draw_ASCII_EE_7_R;  offset_dp = 1; offset_sec = 3; break;
    case (SIZE_EE_14 | RECHTS): draw_char = LCD_Draw_ASCII_EE_14_R; draw_char_sec = LCD_Draw_ASCII_EE_7_R;  offset_dp = 2; offset_sec = 7; break;
//    case (SIZE_EE_20 | RECHTS): draw_char = LCD_Draw_ASCII_EE_20_R; draw_char_sec = LCD_Draw_ASCII_EE_10_R; offset_dp = 2; offset_sec = 9; break;
  }
  LCD_Set_Cursor(ptr->x+1, ptr->y);

  lcd_cursor_y -= offset_sec;
  val = time % 60;  // seconden
  (*draw_char_sec)((val % 10) + '0');
  (*draw_char_sec)((val / 10) + '0');
  lcd_cursor_y += offset_sec;

  time = time / 60;
  val  = time % 60; // minuten
  (*draw_char)((val % 10) + '0');
  (*draw_char)((val / 10) + '0');
  lcd_cursor_y -= offset_dp;
  (*draw_char)(':');
  lcd_cursor_y += offset_dp;
  val = time / 60;
  (*draw_char)((val % 10)  + '0');
  val = val / 10;
  while( val != 0)
  {
    (*draw_char)((val % 10)  + '0');
    val = val / 10;
  }
}

//-----------------------------------------------------------------------------
void Disp_Control_Func(void *s)
{
s_disp_func *ptr;

  ptr = s;
  (ptr->function)();
}

//*****************************************************************************
static void Disp_Draw_Tekst_Func(e_uitlijnen uitlijnen, s_tekst_50 *ptr)
{
int cnt;
char *ch_ptr = ptr->string;

  if (uitlijnen == RECHTS)
  {
    // rechts uitlijnen
    switch (ptr->font_type)
    {
      default: 
      case SIZE_7:      draw_char = LCD_Draw_ASCII_Multi_7_R;  break;
      case SIZE_10:     draw_char = LCD_Draw_ASCII_Multi_10_R; break;
      case SIZE_14:     draw_char = LCD_Draw_ASCII_Multi_14_R; break;
      case SIZE_20:     draw_char = LCD_Draw_ASCII_Multi_20_R; break;
      case SIZE_RUS_7:  draw_char = LCD_Draw_ASCII_Rus_7_R;    break;
      case SIZE_RUS_10: draw_char = LCD_Draw_ASCII_Rus_10_R;   break;
      case SIZE_RUS_14: draw_char = LCD_Draw_ASCII_Rus_14_R;   break;
      case SIZE_RUS_20: draw_char = LCD_Draw_ASCII_Rus_20_R;   break;
      case SIZE_EE_7:   draw_char = LCD_Draw_ASCII_EE_7_R;     break;
      case SIZE_EE_10:  draw_char = LCD_Draw_ASCII_EE_10_R;    break;
      case SIZE_EE_14:  draw_char = LCD_Draw_ASCII_EE_14_R;    break;
      case SIZE_EE_20:  draw_char = LCD_Draw_ASCII_EE_20_R;    break;
    }
    cnt = _hstrlen((char *)ch_ptr);
    ch_ptr = ch_ptr + cnt - 1;
    while (cnt)
    {
      (*draw_char)(*ch_ptr);
      ch_ptr--;
      cnt--;
    }
  }
  else
  {
    // links uitlijnen
    switch (ptr->font_type)
    {
      default: 
      case SIZE_7:      draw_char = LCD_Draw_ASCII_Multi_7_L;  break;
      case SIZE_10:     draw_char = LCD_Draw_ASCII_Multi_10_L; break;
      case SIZE_14:     draw_char = LCD_Draw_ASCII_Multi_14_L; break;
      case SIZE_20:     draw_char = LCD_Draw_ASCII_Multi_20_L; break;
      case SIZE_RUS_7:  draw_char = LCD_Draw_ASCII_Rus_7_L;    break;
      case SIZE_RUS_10: draw_char = LCD_Draw_ASCII_Rus_10_L;   break;
      case SIZE_RUS_14: draw_char = LCD_Draw_ASCII_Rus_14_L;   break;
      case SIZE_RUS_20: draw_char = LCD_Draw_ASCII_Rus_20_L;   break;
      case SIZE_EE_7:   draw_char = LCD_Draw_ASCII_EE_7_L;     break;
      case SIZE_EE_10:  draw_char = LCD_Draw_ASCII_EE_10_L;    break;
      case SIZE_EE_14:  draw_char = LCD_Draw_ASCII_EE_14_L;    break;
      case SIZE_EE_20:  draw_char = LCD_Draw_ASCII_EE_20_L;    break;
    }
    while (*ch_ptr)
    {
      (*draw_char)(*ch_ptr);
      ch_ptr++;
    }
  }
}

#ifdef TEST_STRING
void Disp_Draw_Tekst_Block_L(s_tekst_50 *ptr)
{
unsigned char x1, x2, y1, y2;

  if (ptr->max_bits > 0)
  {
    x1 = x2 = lcd_cursor_x;
    x2 += (ptr->max_bits - 1);
    y1 = y2 = lcd_cursor_y;
    switch (ptr->font_type)
    {
      case SIZE_7:      y1 += 0; y2 -=  7; break;
      case SIZE_10:     y1 += 2; y2 -= 10; break;
      case SIZE_14:     y1 += 3; y2 -= 14; break;
      case SIZE_20:     y1 += 4; y2 -= 19; break;
      case SIZE_RUS_7:  y1 += 0; y2 -=  7; break;
      case SIZE_RUS_10: y1 += 2; y2 -= 10; break;
      case SIZE_RUS_14: y1 += 3; y2 -= 14; break;
      case SIZE_RUS_20: y1 += 4; y2 -= 19; break;
      case SIZE_EE_7:   y1 += 0; y2 -=  7; break;
      case SIZE_EE_10:  y1 += 2; y2 -= 10; break;
      case SIZE_EE_14:  y1 += 3; y2 -= 14; break;
      case SIZE_EE_20:  y1 += 4; y2 -= 19; break;
    }
    LCD_Draw_Black_Vierkant(x1, y2, x2, y1);
  }
}

void Disp_Draw_Tekst_Block_R(s_tekst_50 *ptr)
{
unsigned char x1, x2, y1, y2;

  if (ptr->max_bits > 0)
  {
    x1 = x2 = lcd_cursor_x;
    x1 -= (ptr->max_bits);
    x2--;
    y1 = y2 = lcd_cursor_y;
    switch (ptr->font_type)
    {
      case SIZE_7:      y1 += 0; y2 -=  7; break;
      case SIZE_10:     y1 += 2; y2 -= 10; break;
      case SIZE_14:     y1 += 3; y2 -= 14; break;
      case SIZE_20:     y1 += 4; y2 -= 19; break;
      case SIZE_RUS_7:  y1 += 0; y2 -=  7; break;
      case SIZE_RUS_10: y1 += 2; y2 -= 10; break;
      case SIZE_RUS_14: y1 += 3; y2 -= 14; break;
      case SIZE_RUS_20: y1 += 4; y2 -= 19; break;
      case SIZE_EE_7:   y1 += 0; y2 -=  7; break;
      case SIZE_EE_10:  y1 += 2; y2 -= 10; break;
      case SIZE_EE_14:  y1 += 3; y2 -= 14; break;
      case SIZE_EE_20:  y1 += 4; y2 -= 19; break;
    }
    LCD_Draw_Black_Vierkant(x1, y2, x2, y1);
  }
}
#endif

void Disp_Draw_Tekst_L(void *s)
{
s_disp_tekst *ptr;

  ptr = s;
  LCD_Set_Cursor(ptr->x, ptr->y);
#ifdef TEST_STRING
  Disp_Draw_Tekst_Block_L(ptr->tekst);
#endif
  Disp_Draw_Tekst_Func(LINKS, ptr->tekst);
}

void Disp_Draw_Tekst_R(void *s)
{
s_disp_tekst *ptr;

  ptr = s;
  LCD_Set_Cursor(ptr->x, ptr->y);
#ifdef TEST_STRING
  Disp_Draw_Tekst_Block_R(ptr->tekst);
#endif
  Disp_Draw_Tekst_Func(RECHTS, ptr->tekst);
}

void Disp_Draw_Tekst_L_Option_On(void *s)
{
s_disp_tekst_option_on *ptr;

  ptr = s;
  if (Return_Value(ptr->option_type, Get_Ptr(ptr->option, lcd_rel_disp_index)))
  {
    LCD_Set_Cursor(ptr->x, ptr->y);
#ifdef TEST_STRING
    Disp_Draw_Tekst_Block_L(ptr->tekst);
#endif
    Disp_Draw_Tekst_Func(LINKS, ptr->tekst);
  }
}

void Disp_Draw_Tekst_R_Option_On(void *s)
{
s_disp_tekst_option_on *ptr;

  ptr = s;
  if (Return_Value(ptr->option_type, Get_Ptr(ptr->option, lcd_rel_disp_index)))
  {
    LCD_Set_Cursor(ptr->x, ptr->y);
#ifdef TEST_STRING
    Disp_Draw_Tekst_Block_R(ptr->tekst);
#endif
    Disp_Draw_Tekst_Func(RECHTS, ptr->tekst);
  }
}

void Disp_Draw_Tekst_L_Option_Off(void *s)
{
s_disp_tekst_option_on *ptr;

  ptr = s;
  if (!Return_Value(ptr->option_type, Get_Ptr(ptr->option, lcd_rel_disp_index)))
  {
    LCD_Set_Cursor(ptr->x, ptr->y);
#ifdef TEST_STRING
    Disp_Draw_Tekst_Block_L(ptr->tekst);
#endif
    Disp_Draw_Tekst_Func(LINKS, ptr->tekst);
  }
}

//void Disp_Draw_Tekst_R_Option_Off(void *s)
//{
//s_disp_tekst_option_on *ptr;
//
//  ptr = s;
//  if (!Return_Value(ptr->option_type, Get_Ptr(ptr->option, lcd_rel_disp_index)))
//  {
//    LCD_Set_Cursor(ptr->x, ptr->y);
//#ifdef TEST_STRING
//    Disp_Draw_Tekst_Block_R(ptr->tekst);
//#endif
//    Disp_Draw_Tekst_Func(RECHTS, ptr->tekst);
//  }
//}

void Disp_Draw_Tekst_Add_L(void *s)
{
s_disp_tekst_add *ptr;

  ptr = s;
#ifdef TEST_STRING
  Disp_Draw_Tekst_Block_L(ptr->tekst);
#endif
  Disp_Draw_Tekst_Func(LINKS, ptr->tekst);
}

void Disp_Draw_Tekst_Add_R(void *s)
{
s_disp_tekst_add *ptr;

  ptr = s;
#ifdef TEST_STRING
  Disp_Draw_Tekst_Block_R(ptr->tekst);
#endif
  Disp_Draw_Tekst_Func(RECHTS, ptr->tekst);
}

void Disp_Draw_Tekst_Add_L_Option_On(void *s)
{
s_disp_tekst_add_option_on *ptr;

  ptr = s;
  if (Return_Value(ptr->option_type, Get_Ptr(ptr->option, lcd_rel_disp_index)))
  {
#ifdef TEST_STRING
    Disp_Draw_Tekst_Block_L(ptr->tekst);
#endif
    Disp_Draw_Tekst_Func(LINKS, ptr->tekst);
  }
}

void Disp_Draw_Tekst_Add_R_Option_On(void *s)
{
s_disp_tekst_add_option_on *ptr;

  ptr = s;
  if (Return_Value(ptr->option_type, Get_Ptr(ptr->option, lcd_rel_disp_index)))
  {
#ifdef TEST_STRING
    Disp_Draw_Tekst_Block_R(ptr->tekst);
#endif
    Disp_Draw_Tekst_Func(RECHTS, ptr->tekst);
  }
}

void Disp_Draw_Tekst_Array_L(void *s)
{
s_disp_tekst_array *ptr;
long index;

  ptr = s;
  LCD_Set_Cursor(ptr->x, ptr->y);
  index = Return_Value(ptr->type, Get_Ptr(ptr->index, lcd_rel_disp_index));
  if (index >= ptr->max)
    index = ptr->max - 1;
  if ((long)ptr->index == (long)screen_ptr->rel[screen_ptr->rel_actief].key_action->value->value)
  {
    if (screen_ptr->change_flag)
      index = screen_ptr->value;
  }
#ifdef TEST_STRING
  Disp_Draw_Tekst_Block_L(((s_tekst_50 **)(ptr->tekst_array))[index]);
#endif
  Disp_Draw_Tekst_Func(LINKS, ((s_tekst_50  **)(ptr->tekst_array))[index]);
}

void Disp_Draw_Tekst_Array_R(void *s)
{
s_disp_tekst_array *ptr;
long index;

  ptr = s;
  LCD_Set_Cursor(ptr->x, ptr->y);
  index = Return_Value(ptr->type, Get_Ptr(ptr->index, lcd_rel_disp_index));
  if (index >= ptr->max)
    index = ptr->max - 1;
  if ((long)ptr->index == (long)screen_ptr->rel[screen_ptr->rel_actief].key_action->value->value)
  {
    if (screen_ptr->change_flag)
      index = screen_ptr->value;
  }
#ifdef TEST_STRING
  Disp_Draw_Tekst_Block_R(((s_tekst_50 **)(ptr->tekst_array))[index]);
#endif
  Disp_Draw_Tekst_Func(RECHTS, ((s_tekst_50 **)(ptr->tekst_array))[index]);
}

void Disp_Draw_Tekst_Array_L_Option_On(void *s)
{
s_disp_tekst_array_option_on *ptr;
long index;

  ptr = s;
  if (Return_Value(ptr->option_type, Get_Ptr(ptr->option, lcd_rel_disp_index)))
  {
    LCD_Set_Cursor(ptr->x, ptr->y);
    index = Return_Value(ptr->type, Get_Ptr(ptr->index, lcd_rel_disp_index));
    if (index >= ptr->max)
      index = ptr->max - 1;
    if ((long)ptr->index == (long)screen_ptr->rel[screen_ptr->rel_actief].key_action->value->value)
    {
      if (screen_ptr->change_flag)
        index = screen_ptr->value;
    }
#ifdef TEST_STRING
    Disp_Draw_Tekst_Block_L(((s_tekst_50 **)(ptr->tekst_array))[index]);
#endif
    Disp_Draw_Tekst_Func(LINKS, ((s_tekst_50 **)(ptr->tekst_array))[index]);
  }
}

void Disp_Draw_Tekst_Array_R_Option_On(void *s)
{
s_disp_tekst_array_option_on *ptr;
long index;

  ptr = s;
  if (Return_Value(ptr->option_type, Get_Ptr(ptr->option, lcd_rel_disp_index)))
  {
    LCD_Set_Cursor(ptr->x, ptr->y);
    index = Return_Value(ptr->type, Get_Ptr(ptr->index, lcd_rel_disp_index));
    if (index >= ptr->max)
      index = ptr->max - 1;
    if ((long)ptr->index == (long)screen_ptr->rel[screen_ptr->rel_actief].key_action->value->value)
    {
      if (screen_ptr->change_flag)
        index = screen_ptr->value;
    }
#ifdef TEST_STRING
    Disp_Draw_Tekst_Block_R(((s_tekst_50 **)(ptr->tekst_array))[index]);
#endif
    Disp_Draw_Tekst_Func(RECHTS, ((s_tekst_50 **)(ptr->tekst_array))[index]);
  }
}

//void Disp_Draw_Tekst_Array_L_Option_Off(void *s)
//{
//s_disp_tekst_array_option_on *ptr;
//long index;
//
//  ptr = s;
//  if (!Return_Value(ptr->option_type, Get_Ptr(ptr->option, lcd_rel_disp_index)))
//  {
//    LCD_Set_Cursor(ptr->x, ptr->y);
//    index = Return_Value(ptr->type, Get_Ptr(ptr->index, lcd_rel_disp_index));
//    if (index >= ptr->max)
//      index = ptr->max - 1;
//    if ((long)ptr->index == (long)screen_ptr->rel[screen_ptr->rel_actief].key_action->value->value)
//    {
//      if (screen_ptr->change_flag)
//        index = screen_ptr->value;
//    }
//#ifdef TEST_STRING
//    Disp_Draw_Tekst_Block_L(((s_tekst_50 **)(ptr->tekst_array))[index]);
//#endif
//    Disp_Draw_Tekst_Func(LINKS, ((s_tekst_50 **)(ptr->tekst_array))[index]);
//  }
//}

void Disp_Draw_Tekst_Array_R_Option_Off(void *s)
{
s_disp_tekst_array_option_on *ptr;
long index;

  ptr = s;
  if (!Return_Value(ptr->option_type, Get_Ptr(ptr->option, lcd_rel_disp_index)))
  {
    LCD_Set_Cursor(ptr->x, ptr->y);
    index = Return_Value(ptr->type, Get_Ptr(ptr->index, lcd_rel_disp_index));
    if (index >= ptr->max)
      index = ptr->max - 1;
    if ((long)ptr->index == (long)screen_ptr->rel[screen_ptr->rel_actief].key_action->value->value)
    {
      if (screen_ptr->change_flag)
        index = screen_ptr->value;
    }
#ifdef TEST_STRING
    Disp_Draw_Tekst_Block_R(((s_tekst_50 **)(ptr->tekst_array))[index]);
#endif
    Disp_Draw_Tekst_Func(RECHTS, ((s_tekst_50 **)(ptr->tekst_array))[index]);
  }
}

void Disp_Draw_Tekst_Array_Add_L(void *s)
{
s_disp_tekst_array_add *ptr;
long index;

  ptr = s;
  index = Return_Value(ptr->type, ptr->index);
  if (index >= ptr->max)
    index = ptr->max - 1;
#ifdef TEST_STRING
  Disp_Draw_Tekst_Block_L(((s_tekst_50 **)(ptr->tekst_array))[index]);
#endif
  Disp_Draw_Tekst_Func(LINKS, ((s_tekst_50 **)(ptr->tekst_array))[index]);
}

void Disp_Draw_Tekst_Array_Add_R(void *s)
{
s_disp_tekst_array_add *ptr;
long index;

  ptr = s;
  index = Return_Value(ptr->type, ptr->index);
  if (index >= ptr->max)
    index = ptr->max - 1;
#ifdef TEST_STRING
  Disp_Draw_Tekst_Block_R(((s_tekst_50 **)(ptr->tekst_array))[index]);
#endif
  Disp_Draw_Tekst_Func(RECHTS, ((s_tekst_50 **)(ptr->tekst_array))[index]);
}

void Disp_Draw_Tekst_Array_Add_L_Option_On(void *s)
{
s_disp_tekst_array_add_option_on *ptr;
long index;

  ptr = s;
  if (Return_Value(ptr->option_type, Get_Ptr(ptr->option, lcd_rel_disp_index)))
  {
    index = Return_Value(ptr->type, ptr->index);
    if (index >= ptr->max)
      index = ptr->max - 1;
#ifdef TEST_STRING
    Disp_Draw_Tekst_Block_L(((s_tekst_50 **)(ptr->tekst_array))[index]);
#endif
    Disp_Draw_Tekst_Func(LINKS, ((s_tekst_50 **)(ptr->tekst_array))[index]);
  }
}

void Disp_Draw_Tekst_Array_Add_R_Option_On(void *s)
{
s_disp_tekst_array_add_option_on *ptr;
long index;

  ptr = s;
  if (Return_Value(ptr->option_type, Get_Ptr(ptr->option, lcd_rel_disp_index)))
  {
    index = Return_Value(ptr->type, ptr->index);
    if (index >= ptr->max)
      index = ptr->max - 1;
#ifdef TEST_STRING
    Disp_Draw_Tekst_Block_R(((s_tekst_50 **)(ptr->tekst_array))[index]);
#endif
    Disp_Draw_Tekst_Func(RECHTS, ((s_tekst_50 **)(ptr->tekst_array))[index]);
  }
}

void Disp_Draw_Board_IO(void *s)
{
s_disp_board_IO *ptr;
s_board_IO_on_off *IOptr;
unsigned char board_nr;
unsigned char IO_nr;

  ptr = s;
  IOptr = Get_Ptr(ptr->IO, lcd_rel_disp_index);
  if (IOptr->board_type != 0)
  {
    board_nr = Return_Value(UCHAR, &IOptr->board_nr);
    IO_nr =  Return_Value(UCHAR, &IOptr->IO_nr);
    switch (ptr->type_ingang)
    {
      case ANA_IN:        LCD_Draw_Bitmap(37, 4, &ico_ingang); break;
      case DIG_IN:        LCD_Draw_Bitmap(37, 4, &ico_ingang); break;
      case ANA_OUT:       LCD_Draw_Bitmap(37, 4, &ico_uitgang); break;
      case DIG_OUT:       LCD_Draw_Bitmap(37, 4, &ico_uitgang); break;
	  case MOTOR_CONTROL: LCD_Draw_Bitmap(37, 4, &ico_uitgang); break;
    }
    LCD_Set_Cursor(59, 20);
    switch (IOptr->board_type)
    {
      case IO_06_14_ID:  
#ifdef TEST_STRING
        Disp_Draw_Tekst_Block_L(tekst_IO_06_14_10[board_nr]);
#endif
        Disp_Draw_Tekst_Func(LINKS, tekst_IO_06_14_10[board_nr]); break;
      case IO_12_06_ID:  
#ifdef TEST_STRING
        Disp_Draw_Tekst_Block_L(tekst_IO_12_06_10[board_nr]);
#endif
        Disp_Draw_Tekst_Func(LINKS, tekst_IO_12_06_10[board_nr]); break;
      case IO_08_09_ID:  
#ifdef TEST_STRING
        Disp_Draw_Tekst_Block_L(tekst_IO_08_09_10[board_nr]);
#endif
        Disp_Draw_Tekst_Func(LINKS, tekst_IO_08_09_10[board_nr]); break;
      case IO_EKU_ID:  
#ifdef TEST_STRING
        Disp_Draw_Tekst_Block_L(tekst_IO_EKU_10[board_nr]);
#endif
        Disp_Draw_Tekst_Func(LINKS, tekst_IO_EKU_10[board_nr]); break;
      case IO_H2MC_ID:  
#ifdef TEST_STRING
        Disp_Draw_Tekst_Block_L(tekst_IO_H2MC_10[board_nr]);
#endif
        Disp_Draw_Tekst_Func(LINKS, tekst_IO_H2MC_10[board_nr]); break;
      case IO_H1MC_ID:  
#ifdef TEST_STRING
        Disp_Draw_Tekst_Block_L(tekst_IO_H1MC_10[board_nr]);
#endif
        Disp_Draw_Tekst_Func(LINKS, tekst_IO_H1MC_10[board_nr]); break;
      case IO_05_07_ID:  
#ifdef TEST_STRING
        Disp_Draw_Tekst_Block_L(tekst_IO_05_07_10[board_nr]);
#endif
        Disp_Draw_Tekst_Func(LINKS, tekst_IO_05_07_10[board_nr]); break;
      case IO_07_07_ID:  
#ifdef TEST_STRING
        Disp_Draw_Tekst_Block_L(tekst_IO_07_07_10[board_nr]);
#endif
        Disp_Draw_Tekst_Func(LINKS, tekst_IO_07_07_10[board_nr]); break;
    }
    Disp_Draw_Tekst_Func(LINKS, (s_tekst_50 *)&tekst_space_10);
    switch (ptr->type_ingang)
    {
      case ANA_IN:  
#ifdef TEST_STRING
        Disp_Draw_Tekst_Block_L((s_tekst_50 *)&tekst.Ana_10);
#endif
        Disp_Draw_Tekst_Func(LINKS, (s_tekst_50 *)&tekst.Ana_10);
        break;
      case DIG_IN:
#ifdef TEST_STRING
        Disp_Draw_Tekst_Block_L((s_tekst_50 *)&tekst.Dig_10);
#endif
        Disp_Draw_Tekst_Func(LINKS, (s_tekst_50 *)&tekst.Dig_10);
        break;
      case ANA_OUT:
#ifdef TEST_STRING
        Disp_Draw_Tekst_Block_L((s_tekst_50 *)&tekst.Ana_10);
#endif
        Disp_Draw_Tekst_Func(LINKS, (s_tekst_50 *)&tekst.Ana_10);
        break;
      case DIG_OUT:
#ifdef TEST_STRING
        Disp_Draw_Tekst_Block_L((s_tekst_50 *)&tekst.Dig_10);
#endif
        Disp_Draw_Tekst_Func(LINKS, (s_tekst_50 *)&tekst.Dig_10);
        break;
      case MOTOR_CONTROL:
#ifdef TEST_STRING
        Disp_Draw_Tekst_Block_L((s_tekst_50 *)&tekst.MC_10);
#endif
        Disp_Draw_Tekst_Func(LINKS, (s_tekst_50 *)&tekst.MC_10);
        break;
    }
    IO_nr++;
	if (IO_nr < 10)
      Disp_Draw_Tekst_Func(LINKS, (s_tekst_50 *)&tekst_space_10);
    Disp_Draw_Value_Func(0, (SIZE_10 | LINKS), UCHAR, &IO_nr);
  }
  else
  {
    LCD_Set_Cursor(37, 20);
#ifdef TEST_STRING
    Disp_Draw_Tekst_Block_L((s_tekst_50 *)&tekst.Geen_IO_Toegewezen_10);
#endif
    Disp_Draw_Tekst_Func(LINKS, (s_tekst_50 *)&tekst.Geen_IO_Toegewezen_10);
  }
}

void Disp_Draw_Board_IO_Select(void *s)
{
static unsigned char copy = 0;
s_disp_board_IO_Selection *ptr;
s_board_IO_on_off *IO_ptr;
unsigned char max_nr;

  ptr = s;
  IO_ptr = Get_Ptr(ptr->IO, lcd_rel_disp_index);
  max_nr = *((unsigned char *)Get_Ptr(ptr->max_nr, lcd_rel_disp_index));
  if (index_array == 0)
  {
    if (copy)
    {
      Install_Copy_Board_IO_To_IO();
      copy = 0;
    }
    else
    {
      Install_Copy_IO_To_Board_IO(IO_ptr, ptr->max, max_nr, ptr->on_off, ptr->IO_type, ptr->IO_type_sel, ptr->NotUsedFunc);
    }
  }
  else
  {
    copy = 1;
  }
  
  switch (ptr->IO_type)
  {
    case ANALOG_INPUT_ID:   
      Disp_Draw_Bitmap(&disp_ana_input);
	  if (board_IO_max == 0)
	    LCD_Draw_Bitmap(29, 36, ico_IO_ana_in_array[ptr->IO_type_sel]);
      break;
    case DIGITAL_INPUT_ID:
      Disp_Draw_Bitmap(&disp_dig_input);
	  if (board_IO_max == 0)
	    LCD_Draw_Bitmap(29, 36, ico_IO_dig_in_array[ptr->IO_type_sel]);
      break;
    case ANALOG_OUTPUT_ID:
      Disp_Draw_Bitmap(&disp_ana_output);
	  if (board_IO_max == 0)
	    LCD_Draw_Bitmap(29, 36, ico_IO_ana_out_array[ptr->IO_type_sel]);
      break;
    case DIGITAL_OUTPUT_ID:
      Disp_Draw_Bitmap(&disp_dig_output);
	  if (board_IO_max == 0)
	    LCD_Draw_Bitmap(29, 36, ico_IO_dig_out_array[ptr->IO_type_sel]);
      break;
    case ANALOG_HIGH_INPUT_ID:
      Disp_Draw_Bitmap(&disp_ana_input);
	  if (board_IO_max == 0)
	    LCD_Draw_Bitmap(29, 36, ico_IO_ana_high_in_array[ptr->IO_type_sel]);
      break;
    case MOTOR_CONTROL_ID:
	  Disp_Draw_Bitmap(&disp_motor_control);
	  if (board_IO_max == 0)
	    LCD_Draw_Bitmap(29, 36, ico_IO_motor_control_array[ptr->IO_type_sel]);
	  break;
    case RS485_BUS_ID:
	  Disp_Draw_Bitmap(&disp_RS485_bus);
	  if (board_IO_max == 0)
	    LCD_Draw_Bitmap(29, 36, &ico_RS485_bus);
	  break;
  }
  Disp_Draw_Tekst_Array_L(&disp_NO_IO);
  
  Disp_Draw_Bitmap_Array(&disp_box_IO_pos1_bmp);
  Disp_Draw_Bitmap_Array(&disp_box_IO_pos2_bmp);
  Disp_Draw_Bitmap_Array(&disp_box_IO_pos3_bmp);
  Disp_Draw_Bitmap_Array(&disp_box_IO_pos4_bmp);
  Disp_Draw_Bitmap_Array(&disp_box_IO_pos5_bmp);
  Disp_Draw_Bitmap_Array(&disp_box_IO_pos6_bmp);
  Disp_Draw_Bitmap_Array(&disp_box_IO_pos7_bmp);
  Disp_Draw_Bitmap_Array(&disp_box_IO_pos8_bmp);
  Disp_Draw_Bitmap_Array(&disp_box_IO_pos9_bmp);
  Disp_Draw_Bitmap_Array(&disp_box_IO_pos10_bmp);
  Disp_Draw_Bitmap_Array(&disp_box_IO_pos11_bmp);
  Disp_Draw_Bitmap_Array(&disp_box_IO_pos12_bmp);
  Disp_Draw_Bitmap_Array(&disp_box_IO_pos13_bmp);
  Disp_Draw_Bitmap_Array(&disp_box_IO_pos14_bmp);
  Disp_Draw_Bitmap_Array(&disp_box_IO_pos15_bmp);
  Disp_Draw_Bitmap_Array(&disp_box_IO_pos16_bmp);
  
  Disp_Invert_Block(&disp_block);
  
  if (ptr->on_off)
  {
    Disp_Draw_Bitmap_Array(&disp_IO_vink_pos1_bmp);
    Disp_Draw_Bitmap_Array(&disp_IO_vink_pos2_bmp);
    Disp_Draw_Bitmap_Array(&disp_IO_vink_pos3_bmp);
    Disp_Draw_Bitmap_Array(&disp_IO_vink_pos4_bmp);
    Disp_Draw_Bitmap_Array(&disp_IO_vink_pos5_bmp);
    Disp_Draw_Bitmap_Array(&disp_IO_vink_pos6_bmp);
    Disp_Draw_Bitmap_Array(&disp_IO_vink_pos7_bmp);
    Disp_Draw_Bitmap_Array(&disp_IO_vink_pos8_bmp);
    Disp_Draw_Bitmap_Array(&disp_IO_vink_pos9_bmp);
    Disp_Draw_Bitmap_Array(&disp_IO_vink_pos10_bmp);
    Disp_Draw_Bitmap_Array(&disp_IO_vink_pos11_bmp);
    Disp_Draw_Bitmap_Array(&disp_IO_vink_pos12_bmp);
    Disp_Draw_Bitmap_Array(&disp_IO_vink_pos13_bmp);
    Disp_Draw_Bitmap_Array(&disp_IO_vink_pos14_bmp);
    Disp_Draw_Bitmap_Array(&disp_IO_vink_pos15_bmp);
    Disp_Draw_Bitmap_Array(&disp_IO_vink_pos16_bmp);
  }
  else
  {
    Disp_Draw_Value_2_Size_Option_On(&disp_IO_nr_pos1);
    Disp_Draw_Value_2_Size_Option_On(&disp_IO_nr_pos2);
    Disp_Draw_Value_2_Size_Option_On(&disp_IO_nr_pos3);
    Disp_Draw_Value_2_Size_Option_On(&disp_IO_nr_pos4);
    Disp_Draw_Value_2_Size_Option_On(&disp_IO_nr_pos5);
    Disp_Draw_Value_2_Size_Option_On(&disp_IO_nr_pos6);
    Disp_Draw_Value_2_Size_Option_On(&disp_IO_nr_pos7);
    Disp_Draw_Value_2_Size_Option_On(&disp_IO_nr_pos8);
    Disp_Draw_Value_2_Size_Option_On(&disp_IO_nr_pos9);
    Disp_Draw_Value_2_Size_Option_On(&disp_IO_nr_pos10);
    Disp_Draw_Value_2_Size_Option_On(&disp_IO_nr_pos11);
    Disp_Draw_Value_2_Size_Option_On(&disp_IO_nr_pos12);
    Disp_Draw_Value_2_Size_Option_On(&disp_IO_nr_pos13);
    Disp_Draw_Value_2_Size_Option_On(&disp_IO_nr_pos14);
    Disp_Draw_Value_2_Size_Option_On(&disp_IO_nr_pos15);
    Disp_Draw_Value_2_Size_Option_On(&disp_IO_nr_pos16);
  }
  
  Disp_Draw_Tekst_L(&disp_board_str);
}

void Disp_Draw_Board_IO_Select_Option_On(void *s)
{
s_disp_board_IO_Selection_option_on *ptr;

  ptr = s;
  if (Return_Value(ptr->option_type, Get_Ptr(ptr->option, lcd_rel_disp_index)))
    Disp_Draw_Board_IO_Select(ptr);
}

void Disp_Draw_Board_IO_Select_Option_Off(void *s)
{
s_disp_board_IO_Selection_option_on *ptr;

  ptr = s;
  if (!Return_Value(ptr->option_type, Get_Ptr(ptr->option, lcd_rel_disp_index)))
    Disp_Draw_Board_IO_Select(ptr);
}

void Disp_Draw_Array_Select(void *s)
{
static unsigned char copy = 0;
s_disp_array_Selection *ptr;
unsigned char *opt_ptr;
unsigned char max_nr;

  ptr = s;
  opt_ptr = Get_Ptr(ptr->array, lcd_rel_disp_index);
  max_nr = *((unsigned char *)Get_Ptr(ptr->max_nr, lcd_rel_disp_index));
  if (index_array == 0)
  {
    if (copy)
    {
      Install_Copy_Board_IO_To_Option();
      copy = 0;
    }
    else
    {
	  Install_Copy_Option_To_Board_IO(opt_ptr, ptr->max, max_nr);
    }
  }
  else
  {
    copy = 1;
  }
  
  Disp_Draw_Bitmap_Array(&disp_box_IO_pos1_bmp);
  Disp_Draw_Bitmap_Array(&disp_box_IO_pos2_bmp);
  Disp_Draw_Bitmap_Array(&disp_box_IO_pos3_bmp);
  Disp_Draw_Bitmap_Array(&disp_box_IO_pos4_bmp);
  Disp_Draw_Bitmap_Array(&disp_box_IO_pos5_bmp);
  Disp_Draw_Bitmap_Array(&disp_box_IO_pos6_bmp);
  Disp_Draw_Bitmap_Array(&disp_box_IO_pos7_bmp);
  Disp_Draw_Bitmap_Array(&disp_box_IO_pos8_bmp);
  Disp_Draw_Bitmap_Array(&disp_box_IO_pos9_bmp);
  Disp_Draw_Bitmap_Array(&disp_box_IO_pos10_bmp);
  Disp_Draw_Bitmap_Array(&disp_box_IO_pos11_bmp);
  Disp_Draw_Bitmap_Array(&disp_box_IO_pos12_bmp);
  Disp_Draw_Bitmap_Array(&disp_box_IO_pos13_bmp);
  Disp_Draw_Bitmap_Array(&disp_box_IO_pos14_bmp);
  Disp_Draw_Bitmap_Array(&disp_box_IO_pos15_bmp);
  Disp_Draw_Bitmap_Array(&disp_box_IO_pos16_bmp);
  
  Disp_Invert_Block(&disp_block);
  
  Disp_Draw_Bitmap_Array(&disp_IO_vink_pos1_bmp);
  Disp_Draw_Bitmap_Array(&disp_IO_vink_pos2_bmp);
  Disp_Draw_Bitmap_Array(&disp_IO_vink_pos3_bmp);
  Disp_Draw_Bitmap_Array(&disp_IO_vink_pos4_bmp);
  Disp_Draw_Bitmap_Array(&disp_IO_vink_pos5_bmp);
  Disp_Draw_Bitmap_Array(&disp_IO_vink_pos6_bmp);
  Disp_Draw_Bitmap_Array(&disp_IO_vink_pos7_bmp);
  Disp_Draw_Bitmap_Array(&disp_IO_vink_pos8_bmp);
  Disp_Draw_Bitmap_Array(&disp_IO_vink_pos9_bmp);
  Disp_Draw_Bitmap_Array(&disp_IO_vink_pos10_bmp);
  Disp_Draw_Bitmap_Array(&disp_IO_vink_pos11_bmp);
  Disp_Draw_Bitmap_Array(&disp_IO_vink_pos12_bmp);
  Disp_Draw_Bitmap_Array(&disp_IO_vink_pos13_bmp);
  Disp_Draw_Bitmap_Array(&disp_IO_vink_pos14_bmp);
  Disp_Draw_Bitmap_Array(&disp_IO_vink_pos15_bmp);
  Disp_Draw_Bitmap_Array(&disp_IO_vink_pos16_bmp);
}

//-----------------------------------------------------------------------------
// Speciale functie voor de disp_header
//-----------------------------------------------------------------------------
static void LCD_Disp_Loper(void)
{
int nr_actief = screen_ptr->nr_actief;
int nr_aantal = screen_ptr->nr_aantal;
int rel_actief = screen_ptr->rel_actief;
int rel_max = screen_ptr->rel_max;
int blok_begin;
int blok_einde;
int blok_grootte;

  if (nr_aantal > rel_max)
  {
  blok_grootte = (rel_max * LOPER_SIZE + nr_aantal / 2) / nr_aantal;
  if (blok_grootte < 2)
    blok_grootte = 2;
  blok_begin = ((LOPER_SIZE - blok_grootte) * (nr_actief - rel_actief) + (nr_aantal - rel_max) / 2) / (nr_aantal - rel_max);
  blok_einde = blok_begin + blok_grootte;
  if (blok_einde > LOPER_SIZE)
    blok_einde = LOPER_SIZE;
  if (blok_einde - blok_begin < 2)
    blok_begin = blok_einde - 2;
    LCD_Draw_Black_Block_Rel(231, 17, 231,104); // vertikaal 
    LCD_Draw_Black_Block_Rel(232,104, 238,104); 
    LCD_Draw_Bitmap(233, 18,&ico_arrow_up_0);
    LCD_Draw_Bitmap(233, 100,&ico_arrow_down_0);
    LCD_Draw_Black_Block_Rel(233,   22 + blok_begin, 237, 22 + blok_einde);
  } 
}

void Disp_Draw_Agri_Header(void *s)
{
  s_disp_agri_header *ptr;
  static int layer = 0;
  void const *kop_tekst;           //Pointer naar tekst "Afd"
  void const *space_tekst;     //Pointer naar tekst " "
//  void const *dp_tekst;            //Pointer naar tekst ":"
  void const *min_tekst;       //Pointer naar tekst "-"
  ptr = s;
  layer =  ptr->layer;
  switch(ptr->kop_tekst)
  {
    case EMPTY:
      kop_tekst = &tekst_space_10;
      break;
    case HD_FN:
      kop_tekst = &tekst.Fn_10;
      break;
    case HD_SYST:
      kop_tekst = &tekst.Syst_10;
      break;
    case HD_DIAG:
      kop_tekst = &tekst.Diag_10;
      break;
    //case HD_CURVE:
    //  kop_tekst = &tekst.Curve_10;
    //  break;
    default:
      kop_tekst = &tekst_space_10;
      break;
  }
  space_tekst = &tekst_space_10;
//  dp_tekst  = &tekst_dp_10;
  min_tekst = &tekst_min_7;
  // rhl_tekst = &tekst_rechte_openings_haak_10;
  // rhr_tekst = &tekst_rechte_sluit_haak_10; 
  //Slot 
  if(password_enabled)
  {
    LCD_Draw_Bitmap(2, 2, &ico_10_slot_open);
  }
  else
  {
    LCD_Draw_Bitmap(2, 2, &ico_10_slot_dicht);
  }
  //Tekst Afd
  if(ptr->kop_tekst != EMPTY)
  {          
    LCD_Set_Cursor(12, 12);
    Disp_Draw_Tekst_Func(LINKS, kop_tekst);
//    Disp_Draw_Tekst_Func(LINKS, dp_tekst);
    Disp_Draw_Tekst_Func(LINKS, space_tekst);
  }
  else
  {
    LCD_Set_Cursor(12, 12);
  }
// Titel van het scherm  
#ifdef TEST_STRING
    Disp_Draw_Tekst_Block_L((s_tekst_50 *) ptr->titel_tekst);
#endif
  Disp_Draw_Tekst_Func(LINKS, ptr->titel_tekst);

  if (ptr->titel_toevoeging != 0)
  {
    switch (ptr->type_haak)
	{
	  case HK_RECHT:
	    Disp_Draw_Tekst_Add_L(&disp_rechte_openings_haak_10_L);
		Disp_Draw_Value_Func(0,(SIZE_10 | LINKS),UCHAR,(void *)ptr->titel_toevoeging);
	    Disp_Draw_Tekst_Add_L(&disp_rechte_sluit_haak_10_L);
	    break;
	  case HK_ROND:
	    Disp_Draw_Tekst_Add_L(&disp_ronde_openings_haak_10_L);
		Disp_Draw_Value_Func(0,(SIZE_10 | LINKS),UCHAR,(void *)ptr->titel_toevoeging);
	    Disp_Draw_Tekst_Add_L(&disp_ronde_sluit_haak_10_L);
	    break;
	  default:
	    Disp_Draw_Tekst_Add_L(&disp_space_10_L);
		Disp_Draw_Value_Func(0,(SIZE_10 | LINKS),UCHAR,(void *)ptr->titel_toevoeging);
	    break;
	}
  }

  if (comm_blink_ico)
    LCD_Draw_Bitmap(187, 3, &ico_bus_ok);
//Schermnummer
  if(layer)
  {
    LCD_Set_Cursor(238, 12);
    Disp_Draw_Value_Func(0, (SIZE_7 | RECHTS), UINT, &screen_ptr->nr);
// laag nr en minteken  
    LCD_Set_Cursor(204, 12);
    Disp_Draw_Value_Func(0, (SIZE_7 | LINKS), INT, &layer);
    Disp_Draw_Tekst_Func(LINKS, space_tekst);
    Disp_Draw_Tekst_Func(LINKS, min_tekst);
  }
//Inverteren van de titel balk
  LCD_Invert_Block(  0,  0, 239, 16);
//Verticale lijnen
  LCD_Draw_Black_Block_Abs(   0, 13,   0, 104);
  LCD_Draw_Black_Block_Abs( 239, 13, 239, 104);
//Lijnen boven de tabbalk
  switch(ptr->tab)
  {
    case FN1:
      LCD_Draw_Black_Block_Abs( 44, 104, 238, 104);
      break;
    case FN2:
      LCD_Draw_Black_Block_Abs(  1, 104,  38, 104);
      LCD_Draw_Black_Block_Abs( 83, 104, 238, 104);
      break;
    case FN3:
      LCD_Draw_Black_Block_Abs(  1, 104,  77, 104);
      LCD_Draw_Black_Block_Abs(122, 104, 238, 104);
      break;
    case FN4:
      LCD_Draw_Black_Block_Abs(  1, 104, 116, 104);
      LCD_Draw_Black_Block_Abs(161, 104, 238, 104);
      break;
    case FN5:
      LCD_Draw_Black_Block_Abs(  1, 104, 155, 104);
      LCD_Draw_Black_Block_Abs(200, 104, 238, 104);
      break;
    case FN6:
      LCD_Draw_Black_Block_Abs(  1, 104, 194, 104);
      break;
    default:
      LCD_Draw_Black_Block_Abs(  1, 104, 238, 104);
      break;
  }
// Bitmaps voor de tabbalk
  LCD_Draw_Bitmap(   6, 105, &ico_fn_1);
  LCD_Draw_Bitmap(  45, 105, &ico_fn_2);
  LCD_Draw_Bitmap(  84, 105, &ico_fn_3);
  switch(alarm_blink_ico)
  {
    case 0:  // alarm hard
      LCD_Draw_Bitmap( 123, 105, &ico_fn_4_0);
      break;
    case 1:  // alarm hard meerdere
      LCD_Draw_Bitmap( 123, 105, &ico_fn_4_1);
      break;
    case 2:  // alarm zacht
      LCD_Draw_Bitmap( 123, 105, &ico_fn_4_2);
      break;
    case 3:  // alarm zacht meerdere
      LCD_Draw_Bitmap( 123, 105, &ico_fn_4_3);
      break;
    case 4:  // alarm onderdrukken
      LCD_Draw_Bitmap( 123, 105, &ico_fn_4_4);
      break;
    case 5:  // alarm onderdrukken meerdere
      LCD_Draw_Bitmap( 123, 105, &ico_fn_4_5);
      break;
    case 6:  // geen alarm 
      LCD_Draw_Bitmap( 123, 105, &ico_fn_4_6);
      break;
  }
  if (screen_catcher_time_out == 0)
    LCD_Draw_Bitmap( 162, 105, &ico_fn_5);
  else
    LCD_Draw_Bitmap( 162, 105, &ico_fn_5_com);
  LCD_Draw_Bitmap( 201, 105, &ico_fn_6);
// Lijnen voor de tabbalk
  LCD_Draw_Bitmap(   0, 105, &ico_tab_begin);
  if(ptr->tab == FN2)
  {
    LCD_Draw_Bitmap(  38, 105, &ico_tab_act);
  }
  else
  {
    LCD_Draw_Bitmap(  38, 105, &ico_tab_norm);
  }
  if(ptr->tab == FN3)
  {
    LCD_Draw_Bitmap(  77, 105, &ico_tab_act);
  }
  else
  {
    LCD_Draw_Bitmap(  77, 105, &ico_tab_norm);
  }
  if(ptr->tab == FN4)
  {
    LCD_Draw_Bitmap( 116, 105, &ico_tab_act);
  }
  else
  {
    LCD_Draw_Bitmap( 116, 105, &ico_tab_norm);
  }
  if(ptr->tab == FN5)
  {
    LCD_Draw_Bitmap( 155, 105, &ico_tab_act);
  }
  else
  {
    LCD_Draw_Bitmap( 155, 105, &ico_tab_norm);
  }
  if(ptr->tab == FN6)
  {
    LCD_Draw_Bitmap( 194, 105, &ico_tab_act);
  }
  else
  {
    LCD_Draw_Bitmap( 194, 105, &ico_tab_norm);
  }
  LCD_Draw_Bitmap( 233, 105, &ico_tab_end);
  // loper
  LCD_Disp_Loper();
}    

static unsigned char const bitmap_zet_alarm_hard[] = 
{
  0x00,0x00,0x00,0x00,0x00,0x20,0x00,0x00,0x10,0x00,0xc0,0x0b,0x00,0x38,0x62,0x1f,
  0x07,0x19,0xf1,0x00,0x01,0x11,0x80,0x7c,0x11,0x80,0x00,0xf1,0x40,0x06,0x1f,0x47,
  0x18,0x00,0x38,0x61,0x00,0x00,0x02,0x00,0x00,0x04,0x00,0x00,0x08,0x00,0x00,0x10,
};
static s_bitmap const ico_zet_alarm_hard = { 23,16, bitmap_zet_alarm_hard };
static unsigned char const bitmap_zet_alarm_zacht[] = 
{
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1e,0x00,0xe0,0x11,0x1f,
  0x1e,0x08,0xf1,0x01,0x08,0x11,0x00,0x04,0x11,0x00,0x04,0xf1,0x01,0x02,0x1f,0x1e,
  0x02,0x00,0xe0,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
};
static s_bitmap const ico_zet_alarm_zacht = { 23,16, bitmap_zet_alarm_zacht };
static unsigned char const bitmap_zet_alarm_onderdrukken[] = 
{
  0x04,0x00,0x02,0x08,0x00,0x01,0x10,0x80,0x00,0x20,0x40,0x1e,0x40,0xe0,0x11,0x9f,
  0x1e,0x08,0xf1,0x09,0x08,0x11,0x06,0x04,0x11,0x06,0x04,0xf1,0x09,0x02,0x9f,0x1e,
  0x02,0x40,0xe0,0x01,0x20,0x40,0x00,0x10,0x80,0x00,0x08,0x00,0x01,0x04,0x00,0x02,
};
static s_bitmap const ico_zet_alarm_onderdrukken = { 23,16, bitmap_zet_alarm_onderdrukken };
static unsigned char const bitmap_zet_alarm_uit[] = 
{
  0xf0,0x01,0xfe,0x0f,0x01,0x10,0xff,0x1f,0x00,0x00,0xfc,0x07,0x02,0x08,0x12,0x09,
  0x12,0x09,0x12,0x09,0x12,0x09,0x12,0x09,0x12,0x09,0x12,0x09,0x12,0x09,0x12,0x09,
  0x12,0x09,0x02,0x08,0xfe,0x0f,
};
static s_bitmap const ico_zet_alarm_uit = { 13, 19, bitmap_zet_alarm_uit };
static unsigned char const bitmap_is_teken[] = // '='
{
  0x7f,0x7f,0x00,0x00,0x7f,0x7f,
};
static s_bitmap const ico_is_teken = { 6, 6, bitmap_is_teken };

static void LCD_Draw_Next_Alarm_State(unsigned char x, unsigned char y, unsigned char next_alarm_state)
{
  if(next_alarm_state)
  {
    LCD_Draw_Bitmap(x, y, &ico_is_teken);
    switch(next_alarm_state)
    {
      case MASK_AL_HARD:       LCD_Draw_Bitmap(x-20, y-4, &ico_arrow_up_14);    LCD_Draw_Bitmap(x+15, y-5, &ico_zet_alarm_hard);         break;
      case MASK_AL_ZACHT:      LCD_Draw_Bitmap(x-20, y-4, &ico_arrow_right_14); LCD_Draw_Bitmap(x+15, y-5, &ico_zet_alarm_zacht);        break;
      case MASK_AL_ONDERD: LCD_Draw_Bitmap(x-20, y-4, &ico_arrow_down_14);  LCD_Draw_Bitmap(x+15, y-5, &ico_zet_alarm_onderdrukken); break;
      case MASK_AL_WISSEN:     LCD_Draw_Bitmap(x-28, y-4, &ico_enter_OK_14);    LCD_Draw_Bitmap(x+15, y-7, &ico_zet_alarm_uit);          break;
    }                                          
  }
}

void Disp_Draw_Alarm_State(void)
{
unsigned char loop = 0;
unsigned char next_alarm_state[4] = {0,0,0,0};

  switch (alarm_actueel.state)
  {
    case MASK_AL_HARD:        LCD_Draw_Bitmap(3, 65, &ico_zet_alarm_hard);           break;
    case MASK_AL_ZACHT:       LCD_Draw_Bitmap(3, 65, &ico_zet_alarm_zacht);          break;
    case MASK_AL_ONDERD:  LCD_Draw_Bitmap(3, 65, &ico_zet_alarm_onderdrukken);   break;
  }                                                                                                                      

  if(change_actief_alarm)
  {
    if((alarm_actueel.mask & MASK_AL_HARD) && (alarm_actueel.state != MASK_AL_HARD))
    {
      next_alarm_state[loop] = MASK_AL_HARD;
      loop++;
    }
    if((alarm_actueel.mask & MASK_AL_ZACHT) && (alarm_actueel.state != MASK_AL_ZACHT))
    {
      next_alarm_state[loop] = MASK_AL_ZACHT;
      loop++;
    }
    if((alarm_actueel.mask & MASK_AL_ONDERD) && (alarm_actueel.state != MASK_AL_ONDERD))
    {
      next_alarm_state[loop] = MASK_AL_ONDERD;
      loop++;
    }
    if(alarm_actueel.mask & MASK_AL_WISSEN)
    {
      next_alarm_state[loop] = MASK_AL_WISSEN;
      loop++;
    }

    if(next_alarm_state[0])
    {
      LCD_Draw_White_Block(32, 20, 196, 74);  //add_messagebox
      LCD_Draw_Black_Block_Rel(31, 18, 196, 19);
      LCD_Draw_Black_Block_Rel(30, 19,  31, 74);
      LCD_Draw_Bitmap(30,74, &ico_messages_screen_bot);
      LCD_Draw_Bitmap(196,18, &ico_messages_screen_right); 
      if(next_alarm_state[2])
      {
        LCD_Draw_Next_Alarm_State( 65, 30, next_alarm_state[0]);                     
        LCD_Draw_Next_Alarm_State( 65, 58, next_alarm_state[1]);                     
        LCD_Draw_Next_Alarm_State(140, 44, next_alarm_state[2]);                     
      }
      else 
      {
        LCD_Draw_Next_Alarm_State( 65, 44, next_alarm_state[0]);
        LCD_Draw_Next_Alarm_State(140, 44, next_alarm_state[1]);
      }
    }
  }
}
