// C__DISP_START.C

#include "ch_define.h"
#include "ch_lcd_bitmap.h"
#include "ch_lcd_pixel.h"
#include "ch_disp_start.h"

int start_delay = 5;

#ifndef EMULATOR
static s_bitmap const * const bmp_start_logo[] = 
{ 
  &ico_hotraco_groep,     //  0 HOTRACO
  &ico_hotraco_groep,     //  1 MOOIJ
  &ico_hotraco_groep,     //  2 RR
  &ico_hotraco_groep,     //  3 FIENHAGE
  &ico_hotraco_groep,     //  4 ALKE
  &ico_hotraco_groep,     //  5 LASYSYSTEMS
  &ico_hotraco_groep,     //  6 STARTAGRI
  &ico_hotraco_groep,     //  7 ZUCAMI
  &ico_hotraco_groep,     //  8 PLETTENBURG
  &ico_hotraco_groep,     //  9 BRINK
  &ico_hotraco_groep,     // 10 ERMAF
  &ico_hotraco_groep,     // 11 BIGDUTCHMAN
  &ico_hotraco_groep,     // 12 PRULLAGE
  &ico_hotraco_groep,     // 13 VDL
  &ico_hotraco_groep,     // 14 ACS
  &ico_hotraco_groep,     // 15 Danvan
  &ico_hotraco_groep,     // 16 BERG
  &ico_hotraco_groep,     // 17 AVG
  &ico_hotraco_groep,     // 18 FAROMOR
  &ico_hotraco_groep,     // 19 WILDEBOER
  &ico_cogaszuid,         // 20 COGASZUID
  &priva_ico,             // 21 PRIVA
  &cogas_noord_ico,       // 22 COGASNOORD
  &ico_hotraco_groep,     // xx ONBEKEND
};
static s_disp_bitmap_array const disp_start_logo_bmp = { Disp_Draw_Bitmap_Array,  0, 0, bmp_start_logo, INT, &module.firma , 24 };
#else //EMULATOR
static s_tekst_10   const tekst_emulator = { 10, 3, SIZE_20, "Emulator" };
static s_tekst_50   const tekst_datum_tijd = { 10, 3, SIZE_10, __DATE__ "  " __TIME__ };

static s_disp_tekst const disp_start_logo_bmp = { Disp_Draw_Tekst_L,  37, 20, &tekst_emulator };
static s_disp_tekst const disp_datum_tijd_str = { Disp_Draw_Tekst_L,  37, 40, &tekst_datum_tijd };
#endif //EMULATOR

static void * const lcd_disp_header[] = { &disp_start_logo_bmp, 
#ifdef EMULATOR
&disp_datum_tijd_str,
#endif //EMULATOR
0 }; 

s_screen screen_start =
{
  1, // functie nr
  0, // index
  0, // max rel
  0, // rel actief
  0, // nummer weergegeven bin aantal ingeschakelde functie nummers
  0, // aantal ingeschakelde functie nummers
  lcd_disp_header,
  {0,0,0,0,0,0},
  0,0,17,
  0,0,17+28,
  0,0,17+28+28,
  0,
  0,
  0, // change_flag
  0,
  0,
  0,
  0,
  0,
  0,
  0
};
