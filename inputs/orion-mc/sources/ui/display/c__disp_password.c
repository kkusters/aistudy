// C__DISP_PASSWORD.C

#include <string.h>

#include "ch_define.h"

#include "ch_const.h"
#include "ch_disp.h"
#include "ch_disp_func.h"
#include "ch_disp_option_0.h"
#include "ch_disp_option_1.h"
#include "ch_disp_value.h"
#include "ch_key.h"
#include "ch_lcd_10.h"
#include "ch_lcd_bitmap.h"
#include "ch_lcd_pixel.h"
#include "ch_sd.h"
#include "ch_string.h"
#include "ch_tijd.h"
#include "ch_disp_password.h"

static void Arrow_Code(void);
static void Enter_Code(void);

static s_screen *screen_password_prev_ptr = 0;
static s_screen *screen_password_next_ptr = 0;
#ifdef PASSWORD
unsigned char password_mask = 0;
static unsigned long hotraco_code = 0;
static unsigned long hotraco_password = 0;
static unsigned long code = 0;
#else // PASSWORD
static unsigned int code = 0;
unsigned char password_installateur_enabled = 0;
unsigned char password_gebruiker_enabled = 0;
unsigned char block_gebruiker_password = 0;
#endif // PASSWORD
bit screen_password_increment = 0;
unsigned char password_enabled = 0;
unsigned char password_enabled_delay = 0;

static s_disp_agri_header const disp_header = { Disp_Draw_Agri_Header, FN0, EMPTY, &tekst.Password_10, HK_GEEN, 0, 0};
static void * const lcd_disp_header[] = { &disp_header, 0 };

static s_disp_tekst const disp_password_dp_string = { Disp_Draw_Tekst_R, 81, 40, &tekst_dp_10 };
static s_disp_tekst_add const disp_password_string = { Disp_Draw_Tekst_Add_R, &tekst.Password_10 };
//static s_disp_tekst const disp_password_string = { Disp_Draw_Tekst_R, 81, 40, &tekst.Password_10 };
#ifdef PASSWORD
static s_disp_tekst const disp_openings_haar_string = { Disp_Draw_Tekst_L,  40, 18, &tekst_ronde_openings_haak_7 };
static s_disp_value_add const disp_syst_code =   { Disp_Draw_Value_Add, (SIZE_7 | LINKS), LONG, 0, &hotraco_code };
static s_disp_value const disp_code =            { Disp_Draw_Code, 83, 26, (SIZE_14 | LINKS), LONG, 0, &code };
#else // PASSWORD
static s_disp_value const disp_code =            { Disp_Draw_Code, 83, 26, (SIZE_14 | LINKS), UINT, 0, &code };
#endif // PASSWORD

#ifdef PASSWORD
static void * const lcd_password_disp[] = { &disp_sleutel, &disp_openings_haar_string, &disp_syst_code, &disp_ronde_sluit_haak_7_L, &disp_password_dp_string, &disp_password_string, &disp_code, &disp_password_help, 0 };
#else // PASSWORD
static void * const lcd_password_disp[] = { &disp_sleutel, &disp_password_dp_string, &disp_password_string, &disp_code, &disp_password_help, 0 };
#endif // PASSWORD

#ifdef PASSWORD
static s_key_value const key_code_value = { LONG, 5, &code, &long_0, &long_99999 };
#else // PASSWORD
static s_key_value const key_code_value = { UINT, 4, &code, &int_0, &int_9999 };
#endif // PASSWORD

s_key_action const password_key_action[] =
{
  {
    1,                                // nr
    0,                                // index
    (unsigned char *)&option_on,      // option
    (unsigned char *)&option_index_0, // Optie index 
    lcd_password_disp,                // display
    &disp_cursor_password,            // cursor
    &key_code_value,                  // *value
    Number_Code,                      // void (*number)(void); 
    Arrow_Code,                       // void (*arrow)(void); 
    Enter_Code,                       // void (*enter)(void);
  },
};

s_screen screen_password;
s_screen const screen_password_default =
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
  &password_key_action[0], // first_action
  &password_key_action[sizeof(password_key_action)/sizeof(s_key_action) - 1], // last action 
  0, // change_flag
  0, // punt in normaal display
  0, // punt tijdens invoeren
  0, // aantal digits ingevoerd (punt telt niet mee)
  0, // value
  0, // min value
  0, // max value
  0, // vorige scherm
  0  // prev_next_func
};

//*****************************************************************************
#ifdef PASSWORD
#ifdef SD_CARD
static void Log_Password_To_Sd(char start)
{
char username[21];

  switch (password_enabled & PASSWORD_GEBRUIKER_MASK)
  {
    default:                   strcpy(username,"Unknown");                        break;
    case PASSWORD_BEHEERDER:   strcpy(username,tekst_inst.Beheerder_14.string);   break;
    case PASSWORD_GEBRUIKER_1: strcpy(username,tekst_inst.Gebruiker_1_14.string); break;
    case PASSWORD_GEBRUIKER_2: strcpy(username,tekst_inst.Gebruiker_2_14.string); break;
    case PASSWORD_GEBRUIKER_3: strcpy(username,tekst_inst.Gebruiker_3_14.string); break;
    case PASSWORD_GEBRUIKER_4: strcpy(username,tekst_inst.Gebruiker_4_14.string); break;
    case PASSWORD_HOTRACO:     strcpy(username,"Hotraco"); break;
    case PASSWORD_HOTRACO_ADM: strcpy(username,"Hotraco Admin"); break;
  }
  File_Printf(COM_FILE, "PASSWORD: %s%s%s%s %s %02i-%02i-%04i %02i:%02i:%02i\r\n",
              username,
              (password_enabled & PASSWORD_ENABLED_SETP_MASK) ? " SETP" : "",
              (password_enabled & PASSWORD_ENABLED_SYST_MASK) ? " SYST" : "",
              (password_enabled & PASSWORD_ENABLED_OPT_MASK)  ? " OPT" : "",
              start ? "START" : "STOP",
              tijd.tm_mday, tijd.tm_mon, tijd.tm_year,
              tijd.tm_hour, tijd.tm_min, tijd.tm_sec);
}
#else
static void Log_Password_To_Sd(char start)
{
  start;
}
#endif // SD_CARD

void Genereer_Hotraco_Code(void)
{
  if (module.serie_number == 0)
    hotraco_code = 123;
  else
    hotraco_code = module.serie_number % 1000;
  hotraco_password = (hotraco_code * 5 + 3 * (tijd.tm_mday + tijd.tm_mon)) * 10;
  hotraco_code++;
  hotraco_code *= (tijd.tm_mday + tijd.tm_mon) * 200; 
  hotraco_code += (2 * (50 - tijd.tm_mday - tijd.tm_mon));
}
#else
static void Log_Password_To_Sd(char start)
{
  start;
}
#endif // PASSWORD

void Control_Screen_Password(void)
// zorg voor een correcte inhoud in klimaat scherm 0
{
  #ifdef PASSWORD
  Genereer_Hotraco_Code();
  #endif // PASSWORD
  Control_Screen(&screen_password, &screen_password_default, 1, 1);
}

static void Arrow_Code(void)
{
  switch (key)
  {
    case LEFT:  
      if (Left_Value())
      {
        password_enabled_delay = 0;
        password_enabled = 0;
        Log_Password_To_Sd(0);
        install_flag = 0;
        Prev_Screen();
      }
      break;
    case RIGHT: 
    case UP:
    case DOWN:
      password_enabled_delay = 0;
      password_enabled = 0;
      Log_Password_To_Sd(0);
      install_flag = 0;
      Prev_Screen();
      break;
  }
}

#ifdef PASSWORD
unsigned char Check_Password(unsigned char index)
// return 1 als password gevonden anders return 0
{
static const unsigned char gebruiker[] = { PASSWORD_BEHEERDER, PASSWORD_GEBRUIKER_1, PASSWORD_GEBRUIKER_2, PASSWORD_GEBRUIKER_3, PASSWORD_GEBRUIKER_4 };

  if (opt_alg.password_nummer[index] && screen_ptr->value == opt_alg.password_nummer[index])
  {
    password_enabled = gebruiker[index];
    switch (opt_alg.password_level[index])
    {
      default:                           
      case PASSWORD_LEVEL_GEEN:          password_enabled |= PASSWORD_ENABLED_GEEN; break;
      case PASSWORD_LEVEL_SETP:          password_enabled |= PASSWORD_ENABLED_SETP_MASK; break;
      case PASSWORD_LEVEL_SETP_SYST:     password_enabled |= PASSWORD_ENABLED_SETP_MASK | PASSWORD_ENABLED_SYST_MASK; break;
      case PASSWORD_LEVEL_SETP_SYST_OPT: password_enabled |= PASSWORD_ENABLED_SETP_MASK | PASSWORD_ENABLED_SYST_MASK | PASSWORD_ENABLED_OPT_MASK; break;
    }  
    return (1);
  }
  else
  {
    password_enabled = 0;
    return (0);
  }  
}

unsigned char Check_Password_Hotraco(void)
{
long help;

  help = screen_ptr->value - hotraco_password;
  if ((help % 1234) == 0)
  {
    switch (help / 1234)
    {
      case 1: password_enabled = PASSWORD_ENABLED_SETP_MASK                                                           | PASSWORD_HOTRACO; return (1);
      case 2: password_enabled = PASSWORD_ENABLED_SETP_MASK | PASSWORD_ENABLED_SYST_MASK                              | PASSWORD_HOTRACO; return (1);
      case 3: password_enabled = PASSWORD_ENABLED_SETP_MASK | PASSWORD_ENABLED_SYST_MASK | PASSWORD_ENABLED_OPT_MASK  | PASSWORD_HOTRACO; return (1);
      case 4: password_enabled = PASSWORD_ENABLED_SETP_MASK | PASSWORD_ENABLED_SYST_MASK | PASSWORD_ENABLED_OPT_MASK  | PASSWORD_HOTRACO_ADM; return (1);
    }
  }
  password_enabled = 0;
  return (0);
}

static void Enter_Code(void)
{
int loop = 0;

  password_enabled = 0;
  if (Check_Password_Hotraco())
  {
  }
  else
  {  
    for (loop = 0; loop < 5; loop++)
    {
      if (Check_Password(loop))
        break;
    }  
  }  
  if (password_enabled & password_mask)
  {
    password_enabled_delay = PASSWORD_DELAY;
    Next_Screen(screen_password_next_ptr);
    screen_ptr->prev_screen = screen_password_prev_ptr;
    Log_Password_To_Sd(1);
    if (screen_password_increment)
      Increment_Func_Index();
  }
  else
  {
    password_enabled = 0;
    password_enabled_delay = 0;
    Log_Password_To_Sd(0);
    install_flag = 0;
    Prev_Screen();
  }
}
#else // PASSWORD
static void Enter_Code(void)
{
  password_gebruiker_enabled = (opt_alg.password_gebruiker && (screen_ptr->value == opt_alg.password_gebruiker) && !block_gebruiker_password) ? 1 : 0;
  password_installateur_enabled = ((opt_alg.password_installateur && (screen_ptr->value == opt_alg.password_installateur)) ||
                                   (!opt_alg.password_installateur && password_gebruiker_enabled) || 
                                   (screen_ptr->value == tijd.tm_mday * 10 + tijd.tm_mon)) ? 1 : 0;
  password_enabled = (password_gebruiker_enabled || password_installateur_enabled) ? 1 : 0;
  if (password_enabled)
  {
    password_enabled_delay = PASSWORD_DELAY;
    Next_Screen(screen_password_next_ptr);
    screen_ptr->prev_screen = screen_password_prev_ptr;
    if (screen_password_increment)
      Increment_Func_Index();
  }
  else
  {
    password_enabled_delay = 0;
    install_flag = 0;
    Prev_Screen();
  }
  block_gebruiker_password = 0;
}
#endif // PASSWORD

//*****************************************************************************
void Right_Password(void)
{
  screen_password_prev_ptr = screen_ptr->prev_screen;
  screen_password_next_ptr = screen_ptr;
  screen_password_increment = 1;
  #ifdef PASSWORD
  password_enabled = password_enabled_delay = 0;
  password_mask = PASSWORD_ENABLED_SETP_MASK;
  #else // PASSWORD
  password_installateur_enabled = password_gebruiker_enabled = password_enabled = password_enabled_delay = 0;
  #endif // PASSWORD
  code = 0;
  Control_Screen_Password();
  Next_Screen(&screen_password);
}

void Right_Setpoint_Syst_Password(void)
{
  screen_password_prev_ptr = screen_ptr->prev_screen;
  screen_password_next_ptr = screen_ptr;
  screen_password_increment = 1;
  #ifdef PASSWORD
  password_enabled = password_enabled_delay = 0;
  password_mask = PASSWORD_ENABLED_SYST_MASK;
  #else // PASSWORD
  password_installateur_enabled = password_gebruiker_enabled = password_enabled = password_enabled_delay = 0;
  #endif // PASSWORD
  code = 0;
  Control_Screen_Password();
  Next_Screen(&screen_password);
}

#ifdef PASSWORD
void Right_Option_0_Password(void)
{
  if (opt_alg.password_nummer[0])
  {
    screen_password_prev_ptr = &screen_option_0;
    screen_password_next_ptr = &screen_option_1;
    screen_password_increment = 0;
    password_enabled = password_enabled_delay = 0;
    password_mask = PASSWORD_ENABLED_OPT_MASK;
    code = 0;
    Control_Screen_Password();
    Next_Screen(&screen_password);
  }
  else
  {
    password_enabled = PASSWORD_ENABLED_SETP_MASK | PASSWORD_ENABLED_SYST_MASK | PASSWORD_ENABLED_OPT_MASK;
    password_enabled_delay = PASSWORD_DELAY;
    Next_Screen(&screen_option_1);
  }
}
#else // PASSWORD
void Right_Option_0_Password(void)
{
  if (opt_alg.password_gebruiker && opt_alg.password_installateur)
    block_gebruiker_password = 1;
  if (opt_alg.password_gebruiker || opt_alg.password_installateur)
  {
    screen_password_prev_ptr = &screen_option_0;
    screen_password_next_ptr = &screen_option_1;
    screen_password_increment = 0;
    password_installateur_enabled = password_gebruiker_enabled = password_enabled = password_enabled_delay = 0;
    code = 0;
    Control_Screen_Password();
    Next_Screen(&screen_password);
  }
  else
  {
    password_installateur_enabled = password_gebruiker_enabled = password_enabled = 1;
    password_enabled_delay = PASSWORD_DELAY;
    Next_Screen(&screen_option_1);
  }
}
#endif // PASSWORD
//*****************************************************************************
#ifdef PASSWORD
void Password_Control(void)
{
  if (!install_flag)
  {
    if ((!opt_alg.password_nummer[0]) ||
        (!opt_alg.password_nummer[1] && !opt_alg.password_nummer[2] && !opt_alg.password_nummer[3] && !opt_alg.password_nummer[4]))
    {
      password_enabled = PASSWORD_ENABLED_SETP_MASK | PASSWORD_ENABLED_SYST_MASK | PASSWORD_ENABLED_OPT_MASK;
      password_enabled_delay = PASSWORD_DELAY;
    }
    else if (password_enabled)
    {
      if (password_enabled_delay)
        password_enabled_delay--;
      else  
      {
        Log_Password_To_Sd(0);
        password_enabled = 0;
      }  
    }
  }
}
#else // PASSWORD
void Password_Control(void)
{
  if (!install_flag)
  {
    if (!opt_alg.password_gebruiker)
    {
      password_enabled = 1;
      password_enabled_delay = PASSWORD_DELAY;
    }
    else if (password_enabled)
    {
      if (password_enabled_delay)
        password_enabled_delay--;
      else
        password_enabled = 0;
    }
    password_gebruiker_enabled = password_installateur_enabled = 0;
  }
}
#endif // PASSWORD

void Password_Init(void)
{
  #ifdef PASSWORD
  password_enabled = ((!opt_alg.password_nummer[0]) ||
                      (!opt_alg.password_nummer[1] && !opt_alg.password_nummer[2] && !opt_alg.password_nummer[3] && !opt_alg.password_nummer[4])) ?
                         PASSWORD_ENABLED_SETP_MASK | PASSWORD_ENABLED_SYST_MASK | PASSWORD_ENABLED_OPT_MASK : 0;
  password_enabled_delay = (password_enabled) ? PASSWORD_DELAY : 0;
  #else // PASSWORD
  password_enabled = (!opt_alg.password_gebruiker) ? 1 : 0;
  password_enabled_delay = (password_enabled) ? PASSWORD_DELAY : 0;
  password_gebruiker_enabled = password_installateur_enabled =  0;
  #endif // PASSWORD
}

