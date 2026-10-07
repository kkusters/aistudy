// CH_LCD_HARDWARE.H

#ifndef __CH_LCD_HARDWARE_H
#define __CH_LCD_HARDWARE_H

extern unsigned char lcd_screen[128][30];
extern unsigned char lcd_pc_screen[128][30]; 
extern unsigned char lcd_dimmen_cnt;
extern unsigned char lcd_rel_disp_index;
extern unsigned char weergeven_zandloper;

extern unsigned char lcd_cursor_x;
extern unsigned char lcd_x_offset;
extern unsigned char lcd_cursor_y;
extern unsigned char lcd_y_offset;

extern unsigned char lcd_refresh_fast_switch;
extern unsigned char lcd_refresh_slow_switch;

extern unsigned char lcd_init_switch;

void LCD_Set_Cursor(unsigned char x, unsigned char y);
void LCD_Set_Offset(unsigned char x, unsigned char y);

void LCD_Control(void);

#endif
