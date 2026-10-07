// C__LCD_HARDWARE.C

#include "ch_define.h"
#include "ch_disp.h"
#include "ch_cp2200.h"
#include "ch_htrap.h"
//#include "ch_test.h"
#include "ch_lcd_bitmap.h"
#include "ch_lcd_pixel.h"

unsigned char lcd_cursor_x;
unsigned char lcd_x_offset;
unsigned char lcd_cursor_y;
unsigned char lcd_y_offset;

unsigned char lcd_init_switch = 1;
unsigned char lcd_refresh_fast_switch = 1;
unsigned char lcd_refresh_slow_switch = 0;
unsigned char lcd_refresh_slow_state = 0;
unsigned char lcd_rel_disp_index = 0;
unsigned char weergeven_zandloper = 0;

static void LCD_Hardware_Init(void);

//*****************************************************************************
void LCD_Set_Cursor(unsigned char x, unsigned char y)
{
  lcd_cursor_x = x;
  lcd_cursor_y = y;
}

//*****************************************************************************
void LCD_Set_Offset(unsigned char x, unsigned char y)
{
  lcd_x_offset = x;
  lcd_y_offset = y;
}

//*****************************************************************************
void LCD_Init(void)
{
  LCD_Hardware_Init();
  lcd_cursor_x = 0;
  lcd_cursor_y = 0;
}

//*****************************************************************************
#define LCD_C_D P20_2 
#define LCD_C_D_DP DP20_2
#define LCD_RD P3_3 
#define LCD_RD_DP DP3_3
#define LCD_RD_ODP ODP3_3
#define LCD_RD_ALTSEL0 AS0P3_3
#define LCD_WR P3_2
#define LCD_WR_DP DP3_2
#define LCD_WR_ODP ODP3_2
#define LCD_CE P3_15
#define LCD_CE_DP DP3_15
#define LCD_CE_ODP ODP3_15
#define LCD_RESET P20_12
#define LCD_RESET_DP P20_12

#define LCD_ANGLE P6_6
#define LCD_ANGLE_DP DP6_6
#define LCD_ANGLE_ODP ODP6_6
#define LCD_ANGLE_ALTSEL0 AS0P6_6
#define LCD_ON_OFF P6_7
#define LCD_ON_OFF_DP DP6_7
#define LCD_ON_OFF_ODP ODP6_7
#define LCD_ON_OFF_ALTSEL0 AS0P6_7

#define LCD_STA3 P2_11

#define DATA 0
#define COMMAND 1
#define LCD_DATA P2
#define LCD_DATA_DP DP2
#define LCD_DATA_ODP ODP2
#define DATA_IN 0x0000
#define DATA_OUT 0xFFFF

// 20khz frequentie
#if (CLKFREQ==16000000L)
#define LCD_MULTIPLY 4
#endif // (CLKFREQ==16000000L)
#if (CLKFREQ==32000000L)
#define LCD_MULTIPLY 8
#endif // (CLKFREQ==32000000L)
#if (CLKFREQ==40000000L)
#define LCD_MULTIPLY 10
#endif // (CLKFREQ==40000000L)
#define LCD_CYCLE (LCD_MULTIPLY*200)
// JP 29-09-10
#define LCD_MAX_LIGHT_GREEN (LCD_MULTIPLY*46)
#define LCD_MIN_LIGHT_GREEN (LCD_MULTIPLY*12)
#define LCD_MAX_LIGHT_WHITE (LCD_MULTIPLY*100)
#define LCD_MIN_LIGHT_WHITE (LCD_MULTIPLY*25)
// end JP 29-09-10
#define LCD_BASE_ANGLE (LCD_MULTIPLY*121)
//#define LCD_CYCLE 2000
//#define LCD_MAX_LIGHT 460
//#define LCD_MIN_LIGHT 120
//#define LCD_BASE_ANGLE 1210

#define test_nop _nop();

unsigned char lcd_screen[128][30];
unsigned char lcd_pc_screen[128][30]; 
unsigned char lcd_cursor_x;
unsigned char lcd_x_offset;
unsigned char lcd_cursor_y;
unsigned char lcd_y_offset;

unsigned int lcd_light_level = LCD_MAX_LIGHT_GREEN;
unsigned char lcd_state = 1; // 0 = uit; 1 = on ; 2 = blinking
unsigned char lcd_dimmen_cnt = 2;

// functions to init LCD display
//*****************************************************************************
// funtions for backlight
static void LCD_Backlight_On(void)
{
  CC1_M1 = CC1_M1 & 0x0FFF | 0xF000;
}

static void LCD_Backlight_Off(void)
{
  CC1_M1 = CC1_M1 & 0x0FFF | 0x0000;
  LCD_ON_OFF = 0; // backlight off
}

static void LCD_Backlight_Refresh(void)
{
  switch (lcd_state)
  {
    case 0: // off
      LCD_Backlight_Off();
      break;
    case 1: // on
      LCD_Backlight_On();
      break;
    case 2: // blinking
      if (CC1_M1 & 0xF000)
        LCD_Backlight_Off();
      else
        LCD_Backlight_On();
      break;
  }
}

static void LCD_Backlight(char state)
{
  lcd_state = state;
}

static void LCD_Backlight_Init(void)
{
//  LCD_Backlight_Off(); // TEST
  // LCD backlight aansturing via Capture compare puls pauze P8.6
  LCD_ON_OFF_ALTSEL0 = 1;
  LCD_ON_OFF = 0;
  LCD_ON_OFF_ODP = 0;
  LCD_ON_OFF_DP = 1;

  CC1_CC7 = (unsigned int)-lcd_light_level;
  CC1_M1 = CC1_M1 & 0x0FFF | 0xF000;
  CC1_CC7IC = 0x0000; // geen interrupt van capcom 6

  LCD_Backlight(1);
  LCD_Backlight_Refresh();
}

static void LCD_Backlight_Update(void)
{
static unsigned int old_lcd_light_level = LCD_MAX_LIGHT_GREEN;

#ifdef ETHERNET
  if (cp2200_flash.lcd_color == 1) // white
  {
    if (opt_alg.lcd_dimmen)
      lcd_light_level = (lcd_dimmen_cnt == 0) ? LCD_MIN_LIGHT_WHITE : LCD_MAX_LIGHT_WHITE;
    else
      lcd_light_level = LCD_MAX_LIGHT_WHITE;
  }
  else // green
#endif // ETHERNET
  {
    if (opt_alg.lcd_dimmen)
      lcd_light_level = (lcd_dimmen_cnt == 0) ? LCD_MIN_LIGHT_GREEN : LCD_MAX_LIGHT_GREEN;
    else
      lcd_light_level = LCD_MAX_LIGHT_GREEN;
  }    
  if (old_lcd_light_level != lcd_light_level)
  {
    LCD_Backlight_Init();
    old_lcd_light_level = lcd_light_level;
  }
  LCD_Backlight_Refresh();
}
//*****************************************************************************

// funtions for lcd_angle (lcd spanning)
static void LCD_Angle_Update(void)
{
  if (CC1_CC6 != (unsigned int)-(LCD_BASE_ANGLE+opt_alg.lcd_angle*LCD_MULTIPLY))
  {
    CC1_T1 = (unsigned int)-LCD_CYCLE;
    CC1_CC6 = (unsigned int)-(LCD_BASE_ANGLE+opt_alg.lcd_angle*LCD_MULTIPLY);
  }
}

static void LCD_Angle_Init(void)
{
  LCD_ANGLE_ALTSEL0 = 1;
  LCD_ANGLE = 1;
  LCD_ANGLE_ODP = 0;
  LCD_ANGLE_DP = 1;

  CC1_IOC = 0x0004; 
  _bfld(CC1_T01CON, 0xFF00, 0);
  CC1_T1    = (unsigned int)-LCD_CYCLE;
  CC1_T1REL = (unsigned int)-LCD_CYCLE;
  CC1_CC6 = (unsigned int)-(LCD_BASE_ANGLE+opt_alg.lcd_angle*LCD_MULTIPLY); // hier lcd hoek invullen
  CC1_M1 = CC1_M1 & 0xF0FF | 0x0F00;
 
  CC1_T1IC = 0x0000; // geen interrupt van timer 1
  CC1_CC6IC = 0x0000; // geen interrupt van capcom 6

  CC1_T01CON_T1R = 1; // set CAPCOM1 timer 1 run bit
}

//*****************************************************************************

_inline void LCD_Write_Data_STA01(unsigned char ch)
{
  // check STA0 en STA1
  LCD_C_D = COMMAND; // data
  LCD_CE = 0;
  LCD_DATA_DP = DATA_IN;
  LCD_RD = 0;
  _nop();
  _nop();
  _nop();
  while ((LCD_DATA & 0x0300) != 0x0300);
  LCD_RD = 1;
  LCD_CE = 1;
  // write ch
  LCD_C_D = DATA; // data
  LCD_CE = 0;
  LCD_WR = 0;
  LCD_DATA_DP = DATA_OUT;
  LCD_DATA = (unsigned int)ch << 8;
  _nop();
  _nop();
  _nop();
  LCD_WR = 1;
  LCD_CE = 1;
}

_inline void LCD_Write_Command_STA01(unsigned char ch)
{
  // check STA0 en STA1
  LCD_C_D = COMMAND; // command
  LCD_CE = 0;
  LCD_DATA_DP = DATA_IN;
  LCD_RD = 0;
  _nop();
  _nop();
  _nop();
  while ((LCD_DATA & 0x0300) != 0x0300);
  LCD_RD = 1;
  LCD_CE = 1;
  // write ch
  LCD_CE = 0;
  LCD_WR = 0;
  LCD_DATA_DP = DATA_OUT;
  LCD_DATA = (unsigned int)ch << 8;
  _nop();
  _nop();
  _nop();
  LCD_WR = 1;
  LCD_CE = 1;
}

_inline unsigned char LCD_Read_Data_STA01(void)
{
unsigned char ch;

  // check STA0 en STA1
  LCD_C_D = COMMAND; // data
  LCD_CE = 0;
  LCD_DATA_DP = DATA_IN;
  LCD_RD = 0;
  _nop();
  _nop();
  _nop();
  while ((LCD_DATA & 0x0300) != 0x0300);
  LCD_RD = 1;
  LCD_CE = 1;
  // write ch
  LCD_C_D = DATA; // data
  LCD_CE = 0;
  LCD_DATA_DP = DATA_IN;
  LCD_RD = 0;
  _nop();
  _nop();
  _nop();
  ch = LCD_DATA >> 8;
  LCD_RD = 1;
  LCD_CE = 1;
  return (ch);
}

//-----------------------------------------------------------------------------
_inline void LCD_Write_Data_STA3(unsigned char ch)
{
  // check STA3
  LCD_C_D = COMMAND; // command
  LCD_CE = 0;
  LCD_DATA_DP = DATA_IN;
  LCD_RD = 0;
  _nop();
  _nop();
  _nop();
  while (!LCD_STA3);
  LCD_RD = 1;
  LCD_CE = 1;
  // write ch
  LCD_C_D = DATA; // data
  LCD_CE = 0;
  LCD_WR = 0;
  LCD_DATA_DP = DATA_OUT;
  LCD_DATA = (unsigned int)ch << 8; 
  _nop();
  _nop();
  _nop();
  LCD_WR = 1;
  LCD_CE = 1;
}

_inline void LCD_Write_Command_STA3(unsigned char ch)
{
  // check STA3
  LCD_C_D = COMMAND; // command
  LCD_CE = 0;
  LCD_DATA_DP = DATA_IN;
  LCD_RD = 0;
  _nop();
  _nop();
  _nop();
  while (!LCD_STA3);
  LCD_RD = 1;
  LCD_CE = 1;
  // write ch
  LCD_CE = 0;
  LCD_WR = 0;
  LCD_DATA_DP = DATA_OUT;
  LCD_DATA = (unsigned int)ch << 8;
  _nop();
  _nop();
  _nop();
  LCD_WR = 1;
  LCD_CE = 1;
}
//-----------------------------------------------------------------------------
static void LCD_Hardware_Init(void)
{
  // init poort 3 bit 6 LCD backlight on
#ifdef ETHERNET
  CP2200_Read_Display_Color();
#endif // ETHERNET
  LCD_Backlight_Init();
  LCD_Angle_Init();

// new grafisch display
//  LCD_Drawing_Mode = LCD_Or;
  // reset display
  LCD_CE = 1;
  LCD_CE_ODP = 0;
  LCD_CE_DP = 1;
  LCD_RD_ALTSEL0 = 0;
  LCD_RD = 1;
  LCD_RD_ODP = 0;
  LCD_RD_DP = 1;
  LCD_WR = 1;
  LCD_WR_ODP = 0;
  LCD_WR_DP = 1;
  LCD_C_D = DATA;
  LCD_C_D_DP = 1;
  POCON20 &= 0x0FF0;
  POCON3 &= 0x0FF0;
  POCON2 &= 0x00FF;
  LCD_DATA = 0xFFFF;
  LCD_DATA_ODP = 0x0000;
  LCD_DATA_DP = DATA_IN;
  LCD_RESET = 1;
  LCD_RESET_DP = 1;
  _nop();
  _nop();
  _nop();
  _nop();
  _nop();
  _nop();
  LCD_RESET = 0;
  _nop();
  _nop();
  _nop();
  _nop();
  _nop();
  _nop();
  LCD_RESET = 1;
  _nop();
  _nop();
  _nop();
  _nop();
  _nop();
  _nop();
  //LCD_CE = 0;
  // text wordt niet gebruikt moet echter wel worden toegewezen
  // anders start scherm niet correct
  // set text home adres
  LCD_Write_Data_STA01(0x00);
  LCD_Write_Data_STA01(0x00);
  LCD_Write_Command_STA01(0x40);

  // set text area set
  LCD_Write_Data_STA01(0x1E);
  LCD_Write_Data_STA01(0x00);
  LCD_Write_Command_STA01(0x41);

  // set mode set
  LCD_Write_Command_STA01(0x80);

  // set graphics home adres
  LCD_Write_Data_STA01(0x00);
  LCD_Write_Data_STA01(0x00);
  LCD_Write_Command_STA01(0x42);

  // set graphics area set
  LCD_Write_Data_STA01(0x1E);
  LCD_Write_Data_STA01(0x00);
  LCD_Write_Command_STA01(0x43);

  // set display mode
  LCD_Write_Command_STA01(0x90);

  // onderstaande regels toegevoegd voor RAIO Display zodat display 
  // niet geinverteerd wordt weergegeven
  LCD_Write_Data_STA01(0x00);
  LCD_Write_Data_STA01(0x00);
  LCD_Write_Command_STA01(0xD0);
  //LCD_CE = 1;
}

static void LCD_Clear_Screen(void)
{
int loop;
unsigned int *ptr = (unsigned int *)lcd_screen;
  
  for (loop = 0; loop < 0x780; loop++)
    *ptr++ = 0x0000;  
}

static void LCD_Copy_Screen(void)
{
int loop;
unsigned int *ptr_lcd = (unsigned int *)lcd_screen;
unsigned int *ptr_lcd_pc = (unsigned int *)lcd_pc_screen;

  for (loop = 0; loop < 0x780; loop++)
    *ptr_lcd_pc++ = *ptr_lcd++;  
}

static void LCD_Refresh_Slow(void)
{
static s_screen *scrn_ptr;
static unsigned char *ptr;
static void **disp_ptr;
static s_disp_cursor cursor;
static unsigned char cursor_present;
int loop;

  switch (lcd_refresh_slow_state)
  {
    case  0:
      LCD_Backlight(1);
      LCD_Angle_Update();
      LCD_Backlight_Update();
      LCD_Clear_Screen();
      LCD_Set_Offset(0, 0);
      cursor_present = 0;
      scrn_ptr = screen_ptr;
      if (scrn_ptr->header_disp == 0) // geen scherm data
        lcd_refresh_slow_state = 9;
      else
        lcd_refresh_slow_state++;
      break;
    case  1:
      // weergeven header
      lcd_rel_disp_index = 0;
      disp_ptr = scrn_ptr->header_disp;
      while (*disp_ptr)
      {
        ((void (*)(void *))(*(long *)*disp_ptr))(*disp_ptr);
        disp_ptr++;
      }
      if (scrn_ptr->rel[0].key_action == 0) 
        lcd_refresh_slow_state = 9;
      else
        lcd_refresh_slow_state++;
      break;
    case  2:
	  // scherm 1 weergeven
      if (scrn_ptr->rel_actief != 0)
      {
        LCD_Set_Offset(scrn_ptr->rel[0].x, scrn_ptr->rel[0].y);
        lcd_rel_disp_index = *scrn_ptr->rel[0].key_action->option_index;
        disp_ptr = scrn_ptr->rel[0].key_action->disp;
        while (*disp_ptr)
        {
          ((void (*)(void *))(*(long *)*disp_ptr))(*disp_ptr);
          disp_ptr++;
        }
	  }
      if (scrn_ptr->rel[1].key_action == 0) 
        lcd_refresh_slow_state = 5;
      else
        lcd_refresh_slow_state++;
      break;
    case  3:
	  // scherm 2 weergeven
      if (scrn_ptr->rel_actief != 1)
      {
        LCD_Set_Offset(scrn_ptr->rel[1].x, scrn_ptr->rel[1].y);
        lcd_rel_disp_index = *scrn_ptr->rel[1].key_action->option_index;
        disp_ptr = scrn_ptr->rel[1].key_action->disp;
        while (*disp_ptr)
        {
          ((void (*)(void *))(*(long *)*disp_ptr))(*disp_ptr);
          disp_ptr++;
        }
	  }
      if (scrn_ptr->rel[2].key_action == 0) 
        lcd_refresh_slow_state = 5;
      else
        lcd_refresh_slow_state++;
      break;
    case  4:
	  // scherm 3 weergeven
      if (scrn_ptr->rel_actief != 2)
      {
        LCD_Set_Offset(scrn_ptr->rel[2].x, scrn_ptr->rel[2].y);
        lcd_rel_disp_index = *scrn_ptr->rel[2].key_action->option_index;
        disp_ptr = scrn_ptr->rel[2].key_action->disp;
        while (*disp_ptr)
        {
          ((void (*)(void *))(*(long *)*disp_ptr))(*disp_ptr);
          disp_ptr++;
        }
	  }
      lcd_refresh_slow_state = 5;
      break;
    case  5:
      LCD_Set_Offset(screen_ptr->rel[screen_ptr->rel_actief].x, screen_ptr->rel[screen_ptr->rel_actief].y);
      if (screen_ptr->rel_max > 1)
        Disp_Draw_Bitmap(&disp_rel_cursor);
      if ((unsigned long)screen_ptr->rel[screen_ptr->rel_actief].key_action->cursor != 0)
      {
        cursor_present = 1;
        cursor = *(s_disp_cursor *)screen_ptr->rel[screen_ptr->rel_actief].key_action->cursor;
        cursor.x += screen_ptr->rel[screen_ptr->rel_actief].x;
        cursor.y += screen_ptr->rel[screen_ptr->rel_actief].y;
      }
      lcd_rel_disp_index = *screen_ptr->rel[screen_ptr->rel_actief].key_action->option_index;
      disp_ptr = screen_ptr->rel[screen_ptr->rel_actief].key_action->disp;
      while (*disp_ptr)
      {
        ((void (*)(void *))(*(long *)*disp_ptr))(*disp_ptr);
        disp_ptr++;
      }
      lcd_refresh_slow_state = 9;
      break;
    case  9: // copier lcd scherm naar scherm voor pc
      if (weergeven_zandloper)
      {
        LCD_Set_Offset(0, 0);
        LCD_Draw_White_Block(99, 47, 117, 75);
        LCD_Draw_Bitmap(100, 48, &zandloper_ico);
      }  
      LCD_Copy_Screen();
      if (cursor_present)
      {
        Disp_Cursor_PC(&cursor);
        Disp_Cursor(&cursor);
      }  
      lcd_refresh_slow_state = 10;
      break;
    case 10: // 0x000-0x0FF
      // set  adres pointer
      ptr = (unsigned char *)lcd_screen;
      LCD_CE = 0;
      LCD_Write_Data_STA01(0x00);
      LCD_Write_Data_STA01(0x00);
      LCD_Write_Command_STA01(0x24);
      // schrijven data 
      LCD_Write_Command_STA01(0xB0);
      for (loop = 0; loop < 0x100; loop++)
        LCD_Write_Data_STA3(*ptr++);
      LCD_Write_Command_STA3(0xB2);
      LCD_CE = 1;
      lcd_refresh_slow_state++;
      break;
    case 11: // 0x100-0x1FF
    case 12: // 0x200-0x2FF
    case 13: // 0x300-0x3FF
    case 14: // 0x400-0x4FF
    case 15: // 0x500-0x5FF
    case 16: // 0x600-0x6FF
    case 17: // 0x700-0x7FF
    case 18: // 0x800-0x8FF
    case 19: // 0x900-0x9FF
    case 20: // 0xA00-0xAFF
    case 21: // 0xB00-0xBFF
    case 22: // 0xC00-0xCFF
    case 23: // 0xD00-0xDFF
      LCD_CE = 0;
      LCD_Write_Command_STA01(0xB0);
      for (loop = 0; loop < 0x100; loop++)
        LCD_Write_Data_STA3(*ptr++);
      LCD_Write_Command_STA3(0xB2);
      LCD_CE = 1;           
      lcd_refresh_slow_state++;
      break;
    case 24: // 0xE00-0xEFF
      LCD_CE = 0;
      // schrijven data
      LCD_Write_Command_STA01(0xB0);
      for (loop = 0; loop < 0x100; loop++)
        LCD_Write_Data_STA3(*ptr++);
      LCD_Write_Command_STA3(0xB2);
      // set graphics home adres
      LCD_Write_Data_STA01(0x00);
      LCD_Write_Data_STA01(0x00);
      LCD_Write_Command_STA01(0x42);
      // set graphics area set
      LCD_Write_Data_STA01(0x1E);
      LCD_Write_Data_STA01(0x00);
      LCD_Write_Command_STA01(0x43);
      // set diplay on for graphics
      LCD_Write_Command_STA01(0x98);
      LCD_CE = 1;
      lcd_refresh_slow_state = 0;
      lcd_refresh_slow_switch = 0;
      break;
    default:
      lcd_refresh_slow_state = 0;
      break;
  }
}

static void LCD_Refresh_Fast(void)
{
register /*static*/ unsigned char *ptr;
static void **disp_ptr;
static s_disp_cursor cursor;
unsigned char cursor_present = 0;
int loop;

  LCD_Backlight(1);
  LCD_Angle_Update();
  LCD_Backlight_Update();
  LCD_Clear_Screen();
  LCD_Set_Offset(0, 0);
  if (screen_ptr->header_disp != 0) // geen scherm data
  {
    lcd_rel_disp_index = 0;
    disp_ptr = screen_ptr->header_disp;
    while (*disp_ptr)
    {
      ((void (*)(void *))(*(long *)*disp_ptr))(*disp_ptr);
      disp_ptr++;
    }

    for (loop = 0; loop < 3; loop++)
    {
      if (screen_ptr->rel[loop].key_action != 0)
      {
        if (screen_ptr->rel_actief != loop) // screen exists and is not active -> draw screen
        {
          LCD_Set_Offset(screen_ptr->rel[loop].x, screen_ptr->rel[loop].y);
          lcd_rel_disp_index = *screen_ptr->rel[loop].key_action->option_index;
          disp_ptr = screen_ptr->rel[loop].key_action->disp;
          while (*disp_ptr)
          {
            ((void (*)(void *))(*(long *)*disp_ptr))(*disp_ptr);
            disp_ptr++;
          }
        }
      }
      else
        break;
    }
    if ((screen_ptr->rel_actief >= 0) && (screen_ptr->rel_actief < 3))
    {
      if (screen_ptr->rel[screen_ptr->rel_actief].key_action != 0) // active screen - draw last
      {
        LCD_Set_Offset(screen_ptr->rel[screen_ptr->rel_actief].x, screen_ptr->rel[screen_ptr->rel_actief].y);
        if (screen_ptr->rel_max > 1)
          Disp_Draw_Bitmap(&disp_rel_cursor);
        if ((unsigned long)screen_ptr->rel[screen_ptr->rel_actief].key_action->cursor != 0)
        {
          cursor_present = 1;
          cursor = *(s_disp_cursor *)screen_ptr->rel[screen_ptr->rel_actief].key_action->cursor;
          cursor.x += screen_ptr->rel[screen_ptr->rel_actief].x;
          cursor.y += screen_ptr->rel[screen_ptr->rel_actief].y;
        }
        lcd_rel_disp_index = *screen_ptr->rel[screen_ptr->rel_actief].key_action->option_index;
        disp_ptr = screen_ptr->rel[screen_ptr->rel_actief].key_action->disp;
        while (*disp_ptr)
        {
          ((void (*)(void *))(*(long *)*disp_ptr))(*disp_ptr);
          disp_ptr++;
        }
      }
    }
    if (weergeven_zandloper)
    {
      LCD_Set_Offset(0, 0);
      LCD_Draw_White_Block(99, 47, 117, 75);
      LCD_Draw_Bitmap(100, 48, &zandloper_ico);
    }  
    LCD_Copy_Screen();
    if (cursor_present)
    {
      Disp_Cursor_PC(&cursor);
      Disp_Cursor(&cursor);
    }  
  }
  LCD_CE = 0;
  // set adres pointer
  LCD_Write_Data_STA01(0x00);
  LCD_Write_Data_STA01(0x00);
  LCD_Write_Command_STA01(0x24);
  // schrijven data 
  LCD_Write_Command_STA01(0xB0);
  ptr = (unsigned char *)lcd_screen;
  for (loop = 0; loop < 0xF00; loop++)
    LCD_Write_Data_STA3(*ptr++);
  LCD_Write_Command_STA3(0xB2);
  // set graphics home adres
  LCD_Write_Data_STA01(0x00);
  LCD_Write_Data_STA01(0x00);
  LCD_Write_Command_STA01(0x42);
  // set graphics area set
  LCD_Write_Data_STA01(0x1E);
  LCD_Write_Data_STA01(0x00);
  LCD_Write_Command_STA01(0x43);
  // set diplay on for graphics
  LCD_Write_Command_STA01(0x98);
  LCD_CE = 1;
}

void LCD_Control(void)
{
  if (lcd_init_switch)
  {
    LCD_Init();
    lcd_init_switch = 0;
    lcd_refresh_slow_state = 0;
    lcd_refresh_slow_switch = 0;
    lcd_refresh_fast_switch = 1;
  }
  else if (lcd_refresh_fast_switch)
  {
    LCD_Refresh_Fast();
    lcd_refresh_fast_switch = 0;
    lcd_refresh_slow_state = 0;  // screen just refreshed wait until next slow refresh
    lcd_refresh_slow_switch = 0;
  }
  else if (lcd_refresh_slow_switch)
  {
    LCD_Refresh_Slow();
  }
}

