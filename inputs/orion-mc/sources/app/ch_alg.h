// CH_ALG.H

#ifndef _CH_ALG_H
#define _CH_ALG_H

#include "ct_disp.h"

unsigned char * Copy_N_Bytes(unsigned char *destination, unsigned char *source, unsigned int nr);
unsigned char Bcd_Byte_To_Dec(unsigned char bcd);
unsigned char Dec_Byte_To_Bcd(unsigned char dec);
unsigned char Asc_To_Hex(unsigned char ch);
unsigned char Hex_To_Asc(unsigned value);

int Avg(long val, int number);
long Avg_long(long value, int number);
int Calc_Prop(int x_min ,int x_max ,int y_min ,int y_max ,int x_meet);
long Calc_Prop_Long(long x_min ,long x_max ,long y_min ,long y_max ,long x_meet);
int Calc_Prop_NoLimit(int x_min ,int x_max ,int y_min ,int y_max ,int x_meet);
void Calc_Integrated_Pos(int act, int streef, int *pos, int min_pos, int max_pos, unsigned char bandbreedte, unsigned char max_stap, unsigned char hysterese, unsigned char cyclus_tijd, unsigned char *timer);
void Calc_Integrated_Neg(int act, int streef, int *pos, int min_pos, int max_pos, unsigned char bandbreedte, unsigned char max_stap, unsigned char hysterese, unsigned char cyclus_tijd, unsigned char *timer);
long Calc_Perc(long deel, long totaal, long perc);
long Calc_Perc_Abs(long deel, long totaal, long perc);
long Return_Value(e_type type, void *ptr);
int abs_int( int arg );
long abs_long(long arg);
int Sin_Angle_Degrees(int angle);

void *Get_Ptr(void *val, unsigned char afd);

#endif
