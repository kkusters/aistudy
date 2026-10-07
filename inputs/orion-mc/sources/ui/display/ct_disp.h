// CT_DISP.H

#ifndef _CT_DISP_H
#define _CT_DISP_H

#include <time.h>
#include "ct_data.h"

typedef enum
{
  CHAR = (unsigned char)0,
  UCHAR,
  INT,
  UINT,
  LONG,
  TIME_CHAR,
  TIME_INT,
  HEX_CHAR,
  HEX_INT,
  HEX_LONG
/*
  CHARACTER, // 1 karakter 
  STRING,	 // karakter string
  HIDDEN,	 // verborgen unsigned int
  DATEUCHAR, // datum of tijd waarbij vel met nullen aangevuld moet worden
  DATEUINT   // datum of tijd waarbij vel met nullen aangevuld moet worden 
*/
} e_type;

// links uitlijne hoogste bit e_size is 0
// rechts uitlijnen hoogste bit e_size is 1
typedef enum
{
  LINKS = 0,
  RECHTS = 0x80
} e_uitlijnen;

// language reserve second nibble 0x0000 - 0x00F0
#define MASK_LINE_OUT 0x0080
#define MASK_NOT_LINE_OUT 0xFF7F
#define MASK_LANGUAGE 0x0070
#define MASK_NOT_LANGUAGE 0xFF8F
#define MASK_SIZE 0x000F
#define MASK_NOT_SIZE 0xFFF0

typedef enum
{
  SIZE_7 = 0, // code page 1252 (western) multi
  SIZE_10,
  SIZE_14,
  SIZE_20,
  SIZE_RUS_7 = 0x10, // code page 1251 (cyrl) russisch
  SIZE_RUS_10,
  SIZE_RUS_14,
  SIZE_RUS_20,
  SIZE_EE_7 = 0x20, // code page 1250 (EE) east europe
  SIZE_EE_10,
  SIZE_EE_14,
  SIZE_EE_20
} e_size;

typedef struct
{
  e_type type;			    // Type
  unsigned char max_digits; // maximum aantal digits die kunnen worden ingevoerd
  void *value;              // Waarde
  void *min_value;		    // Minimum grens value=>min_value
  void *max_value;		    // Maximum grens value<=max_value
} s_key_value;

typedef struct
{
  unsigned int nr;                      // functie nummer
  unsigned char index;                  // Veld nummer
  unsigned char *option;                // Scherm opties
  unsigned char *option_index;          // Optie index afdeling
  void *disp;
  void *cursor;
  s_key_value const *value;             // Struct value
  void (*number)(void);                 //
  void (*arrow)(void);                  //
  void (*enter)(void);                  //
} s_key_action;

typedef struct
{
  s_key_action const *key_action;
  unsigned char x;
  unsigned char y;
} s_rel_key_action;

typedef struct ss_screen
{
  unsigned int  nr;		    // Actueel functie nr
  unsigned char index;	    // Actueel index nummer
  unsigned char rel_max;    // maximum aantal regels per scherm
  unsigned char rel_actief; // Actueele regel (0, 1, 2)
  int nr_actief;            // Actueel ingeschakelde functie nummer (voor weergave balk)
  int nr_aantal;            // aantal ingeschakelde functie nummers (voor weergave balk)
  void *header_disp;        // Algemene scherm opmaak
  void (*func[6])(void);    // functies behorende bij functie toetsen (niet gebruikt)
  s_rel_key_action rel[3];	// Pointer naar scherm, Relative positie x, y 
  s_key_action const *first_action;	 // pointer naar eerste scherm.
  s_key_action const *last_action;	 // pointer naar laatste scherm.
  unsigned char change_flag;		 // change_flag, variable gewijzigd
  unsigned char norm_point; // "punt" in normaale display
  unsigned char point;      // "punt" tijdens invoeren
  unsigned char nr_digits;	// Aantal digits
  long value;
  long min_value;
  long max_value;
  struct ss_screen *prev_screen;
  void (*prev_next_func)(void);
} s_screen;

//*****************************************************************************
typedef struct
{
  unsigned char width; // width bitmap with space
  unsigned char height; // heigth bitmap with space
  unsigned char const *pixel;
} s_bitmap;

typedef struct
{
  unsigned char width; // width bitmap
  unsigned char height; // heigth bitmap
  unsigned char spacing; // spacing between characters
  unsigned char x_offset; // x offset bitmap within 
  unsigned char y_offset; // y offset bitmap within 
  unsigned char b_width; // width bitmap with space
  unsigned char b_height; // heigth bitmap with space
  unsigned char const *pixel;
} s_character;

//*****************************************************************************

typedef struct
{
  unsigned char x;	  // rechts onder (wordt toch meestal voor value gebruikt
  unsigned char y;
  unsigned char dx;   // breedte cursor
  unsigned char dy1;  // cursor lijn
  unsigned char dy2;  // cursor block
                      // ongelijk 0 dan wisselt cursor met hoogte dy1 en hoogte dy2 elkaar af, 
					  // gelijk   0 dan cursor dy1 of geen cursor
} s_disp_cursor;

typedef struct
{
  void (*func)(void *s);
  unsigned char x;
  unsigned char y;
  s_bitmap const *data;
} s_disp_bitmap;

typedef struct
{
  void (*func)(void *s);
  unsigned char x;
  unsigned char y;
  s_bitmap const *data;
  e_type option_type;
  void *option;
} s_disp_bitmap_option_on;

typedef struct
{
  void (*func)(void *s);
  unsigned char x;
  unsigned char y;
  s_bitmap const * const *data_array;
  e_type type;
  void *index;
  long max; // geeft aantal strings weer
} s_disp_bitmap_array;

typedef struct
{
  void (*func)(void *s);
  unsigned char x;
  unsigned char y;
  s_bitmap const * const *data_array;
  e_type type;
  void *index;
  long max; // geeft aantal strings weer
  e_type option_type;
  void *option;
} s_disp_bitmap_array_option_on;

typedef struct
{
  void (*func)(void *s);
  unsigned char x;
  unsigned char y;
  s_bitmap const *data;
  e_type option_type;
  void *option_value;
  e_type invert_type;
  void *invert_value;
} s_disp_bitmap_option_on_invert;

typedef struct
{
  void (*func)(void *s);
  unsigned char x;
  unsigned char y;
  s_bitmap const *data;
  e_type invert_type;
  void *invert_value;
} s_disp_bitmap_invert;

typedef struct
{
  void (*func)(void *s);
  s_bitmap const *data;
  e_type option_type;
  void *option_value;
  e_type invert_type;
  void *invert_value;
} s_disp_bitmap_option_on_invert_add;

typedef struct
{
  void (*func)(void *s);
  unsigned char x;
  unsigned char y;
  e_size size;
  e_type type;
  unsigned char point;
  void *value;
} s_disp_value;

typedef struct
{
  void (*func)(void *s);
  unsigned char x;
  unsigned char y;
  e_size size_1; // grootte eerste letters
  e_size size_2; // grootte laatste letters
  e_type type;
  unsigned char point; // punt van waaraf lettergrootte veranderd
  void *value;
} s_disp_value_2_size_no_point;

typedef struct
{
  void (*func)(void *s);
  unsigned char x;
  unsigned char y;
  e_size size_1; // grootte eerste letters
  e_size size_2; // grootte laatste letters
  e_type type;
  unsigned char point;
  void *value;
  unsigned char nr_digits; // punt van waaraf lettergrootte veranderd
  e_type option_type;
  void *option;
} s_disp_value_2_size_option_on;

typedef struct
{
  void (*func)(void *s);
  e_size size;
  e_type type;
  unsigned char point;
  void *value;
} s_disp_value_add;

typedef struct
{
  void (*func)(void *s);
  unsigned char x;
  unsigned char y;
  e_size size;
  e_type type;
  unsigned char point;
  void *value;
  e_type option_type;
  void *option;
} s_disp_value_option_on;

typedef struct
{
  void (*func)(void *s);
  e_size size;
  e_type type;
  unsigned char point;
  void *value;
  e_type option_type;
  void *option;
} s_disp_value_add_option_on;

/*
typedef struct 
{
  void (*func)(void *s);
  unsigned char x;
  unsigned char y;
  e_size size;
  char const *string;
} s_disp_string;

typedef struct
{
  void (*func)(void *s);
  unsigned char x;
  unsigned char y;
  e_size size;
  char const *string;
  e_type option_type;
  void *option;
} s_disp_string_option_on;

typedef struct 
{
  void (*func)(void *s);
  e_size size;
  char const *string;
} s_disp_string_add;

typedef struct 
{
  void (*func)(void *s);
  unsigned char x;
  unsigned char y;
  e_size size;
  char const * const *string_array;
  e_type type;
  void *index;
  long max; // geeft aantal strings weer
} s_disp_string_array;

typedef struct 
{
  void (*func)(void *s);
  e_size size;
  char const * const *string_array;
  e_type type;
  void *index;
  long max; // geeft aantal strings weer
} s_disp_string_array_add;

typedef struct 
{
  void (*func)(void *s);
  unsigned char x;
  unsigned char y;
  e_size size;
  char const *ch;
} s_disp_char;

typedef struct 
{
  void (*func)(void *s);
  e_size size;
  char const *ch;
} s_disp_char_add;
*/
typedef struct
{
  void (*func)(void *s);
  unsigned char x1;
  unsigned char y1;
  unsigned char x2;
  unsigned char y2;
} s_disp_block;

typedef struct
{
  void (*func)(void *s);
} s_disp_loper;

typedef struct
{
  void (*func)(void *);
  void *disp;
} s_disp_data_component;

typedef struct
{
  void (*func)(void *);
  void *disp;
  e_type option_type;
  void *option;
} s_disp_data_component_option_on;

typedef struct
{
  void (*func)(void *);
  void *disp_array;
  e_type type;
  void *index;
  long max; 
} s_disp_data_component_array;

typedef struct
{
  void (*func)(void *);
  void *disp_array;
  e_type type;
  void *index;
  long max; 
  e_type option_type;
  void *option;
} s_disp_data_component_array_option_on;

typedef enum
{
  HORIZONTAAL_LINKS_RECHTS = 0, // balk horizontaal van links naar rechts
  HORIZONTAAL_RECHTS_LINKS,     // balk horizontaal van links naar rechts
  VERTIKAAL_BOVEN_ONDER,        // balk vertikaal van boven naar onder
  VERTIKAAL_ONDER_BOVEN         // balk vertikaal van onder naar boven
} e_balk;

typedef struct
{
  void (*func)(void *s);
  unsigned char x1;
  unsigned char y1;
  unsigned char x2;
  unsigned char y2;
  e_balk richting; 
  e_type type;
  void *value;
  void *min_value;
  void *max_value;
} s_disp_balk;

typedef struct
{
  void (*func)(void *s);
  unsigned char x;
  unsigned char y;
  e_size size;
  time_t *time;
} s_disp_time;

typedef struct
{
  void (*func)(void *s);
  void (*function)(void);
} s_disp_func;

typedef struct
{
  void (*func)(void *s);
  s_board_IO_on_off *IO;
  unsigned char max;
  unsigned char *max_nr;
  unsigned char on_off;
  unsigned char IO_type;
  unsigned char IO_type_sel;
  unsigned char (*NotUsedFunc)(s_board_IO_on_off IO_new);
} s_disp_board_IO_Selection;

typedef struct
{
  void (*func)(void *s);
  s_board_IO_on_off *IO;
  unsigned char max;
  unsigned char *max_nr;
  unsigned char on_off;
  unsigned char IO_type;
  unsigned char IO_type_sel;
  unsigned char (*NotUsedFunc)(s_board_IO_on_off IO_new);
  e_type option_type;
  void *option;
} s_disp_board_IO_Selection_option_on;

typedef struct
{
  void (*func)(void *s);
  unsigned char *array;
  unsigned char max;
  unsigned char *max_nr;
} s_disp_array_Selection;

typedef struct
{
  void (*func)(void *s);
  unsigned char type_ingang; // ANA_IN, DIG_IN, ANA_OUT, DIG_OUT
  s_board_IO_on_off *IO;
} s_disp_board_IO;

//*****************************************************************************
typedef struct
{
  void (*func)(void *s);
  unsigned char x;
  unsigned char y;
  void const *tekst;
} s_disp_tekst;

typedef struct
{
  void (*func)(void *s);
  unsigned char x;
  unsigned char y;
  void const *tekst;
  e_type option_type;
  void *option;
} s_disp_tekst_option_on;

typedef struct 
{
  void (*func)(void *s);
  void const *tekst;
} s_disp_tekst_add;

typedef struct 
{
  void (*func)(void *s);
  void const *tekst;
  e_type option_type;
  void *option;
} s_disp_tekst_add_option_on;

typedef struct 
{
  void (*func)(void *s);
  unsigned char x;
  unsigned char y;
  void const *tekst_array;
  e_type type;
  void *index;
  long max; // geeft aantal strings weer
} s_disp_tekst_array;

typedef struct 
{
  void (*func)(void *s);
  unsigned char x;
  unsigned char y;
  void const *tekst_array;
  e_type type;
  void *index;
  long max; // geeft aantal strings weer
  e_type option_type;
  void *option;
} s_disp_tekst_array_option_on;

typedef struct 
{
  void (*func)(void *s);
  void const *tekst_array;
  e_type type;
  void *index;
  long max; // geeft aantal strings weer
} s_disp_tekst_array_add;

typedef struct 
{
  void (*func)(void *s);
  void const *tekst_array;
  e_type type;
  void *index;
  long max; // geeft aantal strings weer
  e_type option_type;
  void *option;
} s_disp_tekst_array_add_option_on;

typedef enum
{
  EMPTY = (unsigned char)0,
  HD_FN,
  HD_SYST,
  HD_DIAG,
  HD_CURVE
} e_kop_tekst;

typedef enum
{
  HK_GEEN = (unsigned char)0,
  HK_RECHT,
  HK_ROND
} e_type_haak;

typedef union
{
  s_disp_bitmap bitmap;
  s_disp_tekst tekst;
  s_disp_tekst_array tekst_array;
} s_disp_union;

typedef enum
{
  FN0 = (unsigned char)0,
  FN1,
  FN2,
  FN3,
  FN4,
  FN5,
  FN6
} e_tab;

typedef struct
{
  void (*func)(void *s);
  e_tab tab;
  e_kop_tekst kop_tekst;
  void const *titel_tekst;		   // Pointer naar tekst "Titel"
  e_type_haak type_haak;
  unsigned char *titel_toevoeging; // Nummer toevoeging achter titel tussen rechte haken -> voorbeeld: Groep[1]
  int layer; 					   // voorste nummer van bladwijzer
} s_disp_agri_header;

#endif
