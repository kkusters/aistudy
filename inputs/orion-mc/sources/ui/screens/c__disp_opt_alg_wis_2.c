// C__DISP_OPT_ALG_WIS_2.C

#include "ch_define.h"
#include "ch_const.h"
#include "ch_alarm.h"
#include "ch_can_IO.h"
#include "ch_can_backbone_appl.h"
#include "ch_disp.h"
#include "ch_disp_func.h"
#include "ch_disp_opt_alg_2.h"
#include "ch_disp_password.h"
#include "ch_disp_value.h"
#include "ch_key.h"
#include "ch_lcd_bitmap.h"
#include "ch_lcd_pixel.h"
#include "ch_string.h"
#include "ch_disp_opt_alg_wis_2.h"

#define DISP_OPT_WIS_OPTION 0
#define DISP_OPT_WIS_SETPOI 1
#define DISP_OPT_WIS_VAL_HR 2
#define DISP_OPT_WIS_VALUES 3
#define DISP_OPT_WISSEN_MAX 4
     
typedef struct
{
  unsigned char possible;
  unsigned char wissen;
} s_data_wissen;
     
static s_data_wissen wisdata[DISP_OPT_WISSEN_MAX];
     
#define TEKST_SELECT  0
#define TEKST_DRUK_OK 1
#define TEKST_GEWIST  2

static unsigned char wissen_bevestigen = 0;
static unsigned char kop_tekst = TEKST_SELECT;
                            
static void Arrow_Wissen_Func(void);
static void Enter_Wissen_Func(void);
static void Arrow_Wissen_Value(void);
static void Enter_Wissen_Value(void);
static void Disp_Control_Data_Wissen(void);

static s_disp_cursor const disp_cursor_checkbox_mini = { 48, 21, 12, 1, 14 };

static s_disp_agri_header const disp_header  = { Disp_Draw_Agri_Header, FN6, EMPTY, &tekst_inst.Opties_algemeen_10, 2};
static s_disp_func        const disp_control = { Disp_Control_Func, Disp_Control_Data_Wissen };

static s_disp_bitmap const disp_afvalemmer_bmp = { Disp_Draw_Bitmap, 12, 19, &ico_afvalemmer }; 

void const * const tekst_data_wissen_14[] = { &tekst_inst.Selecteer_Data_14, &tekst_inst.Druk_OK_Om_Te_Wissen_14, &tekst_inst.Data_Gewist_14 };
static s_disp_tekst_array const disp_data_wissen_str = { Disp_Draw_Tekst_Array_L, 37+3, 22+19, &tekst_data_wissen_14, UCHAR, &kop_tekst, 3 };

static void * const messagebox_bevestig_wissen[] = { &disp_messagebox_bevestig, &disp_zeker_weten_str, &disp_bevestig_arrow_up_bmp, &disp_bevestig_is_teken, &disp_space_10_L, &disp_wissen_str, 0 };
static s_disp_data_component_option_on const disp_messagebox_bevestig_wissen = { Disp_Draw_Component_Abs_Option_On, messagebox_bevestig_wissen, UCHAR, &wissen_bevestigen };
                 
static s_disp_tekst const disp_wis_1_str = { Disp_Draw_Tekst_L,  55, 20, &tekst_inst.Opties_10 };
static s_disp_tekst const disp_wis_2_str = { Disp_Draw_Tekst_L,  55, 20, &tekst_inst.Setpoints_10 };
static s_disp_tekst const disp_wis_3_str = { Disp_Draw_Tekst_L,  55, 20, &tekst_inst.Werkgeheugen_10 };
static s_disp_tekst const disp_wis_4_str = { Disp_Draw_Tekst_L,  55, 20, &tekst_inst.Restart_10 };

static s_disp_bitmap_array const disp_wis_1_bmp = { Disp_Draw_Bitmap_Array,  37, 8, bmp_false_true_klein, UCHAR, &wisdata[0].wissen, 3 };
static s_disp_bitmap_array const disp_wis_2_bmp = { Disp_Draw_Bitmap_Array,  37, 8, bmp_false_true_klein, UCHAR, &wisdata[1].wissen, 3 };
static s_disp_bitmap_array const disp_wis_3_bmp = { Disp_Draw_Bitmap_Array,  37, 8, bmp_false_true_klein, UCHAR, &wisdata[2].wissen, 3 };
static s_disp_bitmap_array const disp_wis_4_bmp = { Disp_Draw_Bitmap_Array,  37, 8, bmp_false_true_klein, UCHAR, &wisdata[3].wissen, 3 };

static void * const lcd_disp_header[] = { &disp_control, &disp_afvalemmer_bmp, &disp_data_wissen_str, &disp_header,  0};   
static void * const lcd_wis_1[] =       { &disp_wis_1_str, &disp_wis_1_bmp, &disp_messagebox_bevestig_wissen, 0 };
static void * const lcd_wis_2[] =       { &disp_wis_2_str, &disp_wis_2_bmp, &disp_messagebox_bevestig_wissen, 0 };
static void * const lcd_wis_3[] =       { &disp_wis_3_str, &disp_wis_3_bmp, &disp_messagebox_bevestig_wissen, 0 };
static void * const lcd_wis_4[] =       { &disp_wis_4_str, &disp_wis_4_bmp, &disp_messagebox_bevestig_wissen, 0 };

static s_key_value const key_lcd_wis_1_value = { UCHAR, 2, &wisdata[0].wissen,   &uchar_0, &uchar_1 };
static s_key_value const key_lcd_wis_2_value = { UCHAR, 2, &wisdata[1].wissen,   &uchar_0, &uchar_1 };
static s_key_value const key_lcd_wis_3_value = { UCHAR, 2, &wisdata[2].wissen,   &uchar_0, &uchar_1 };
static s_key_value const key_lcd_wis_4_value = { UCHAR, 2, &wisdata[3].wissen,   &uchar_0, &uchar_1 };

s_key_action const opt_alg_wis_2_key_action[] =
{
  { 
    DISP_OPT_WIS_OPTION,                
    0,                
    (unsigned char *)&option_on,        
    (unsigned char *)&option_index_0,     
    lcd_wis_1,           
    0,                
    &dummy_value,     
    Dummy_Func,       
    Arrow_Wissen_Func,
    Enter_Wissen_Func,       
  },
  {
    DISP_OPT_WIS_OPTION,                
    1,                
    &wisdata[0].possible,        
    (unsigned char *)&option_index_0,     
    lcd_wis_1,             
    &disp_cursor_checkbox_mini,
    &key_lcd_wis_1_value,    
    Dummy_Func,        
    Arrow_Wissen_Value,  
    Enter_Wissen_Value,      
  },
  { 
    2,                
    0,                
    (unsigned char *)&option_on,        
    (unsigned char *)&option_index_0,     
    lcd_wis_2,           
    0,                
    &dummy_value,     
    Dummy_Func,       
    Arrow_Wissen_Func,
    Enter_Wissen_Func,       
  },
  {
    2,                
    1,                
    &wisdata[1].possible,        
    (unsigned char *)&option_index_0,     
    lcd_wis_2,             
    &disp_cursor_checkbox_mini,
    &key_lcd_wis_2_value,    
    Dummy_Func,        
    Arrow_Wissen_Value,  
    Enter_Wissen_Value,      
  },
  { 
    3,                
    0,                
    (unsigned char *)&option_on,        
    (unsigned char *)&option_index_0,     
    lcd_wis_3,           
    0,                
    &dummy_value,     
    Dummy_Func,       
    Arrow_Wissen_Func,
    Enter_Wissen_Func,       
  },
  {
    3,                
    1,                
    &wisdata[2].possible,        
    (unsigned char *)&option_index_0,     
    lcd_wis_3,             
    &disp_cursor_checkbox_mini,
    &key_lcd_wis_3_value,    
    Dummy_Func,        
    Arrow_Wissen_Value,  
    Enter_Wissen_Value,      
  },
  { 
    4,                
    0,                
    (unsigned char *)&option_on,        
    (unsigned char *)&option_index_0,     
    lcd_wis_4,           
    0,                
    &dummy_value,     
    Dummy_Func,       
    Arrow_Wissen_Func,
    Enter_Wissen_Func,       
  },
  {
    4,                
    1,                
    &wisdata[3].possible,        
    (unsigned char *)&option_index_0,     
    lcd_wis_4,             
    &disp_cursor_checkbox_mini,
    &key_lcd_wis_4_value,    
    Dummy_Func,        
    Arrow_Wissen_Value,  
    Enter_Wissen_Value,      
  },
};

s_screen screen_opt_alg_wis_2;
s_screen const screen_opt_alg_wis_2_default =
{
  0, // functie nr
  0, // index
  3, // max rel
  0, // rel actief
  0, // nr actief nummer weergegeven bin aantal ingeschakelde functie nummers
  0, // aantal ingeschakelde functie nummers
  lcd_disp_header, // algemene scherm opmaak
  {0,0,0,0,0,0}, // functies behorende bij functie toetsen
  0,3,40, // rel[0]
  0,3,59, // rel[1]
  0,3,78, // rel[2]
  &opt_alg_wis_2_key_action[0], // first_action
  &opt_alg_wis_2_key_action[sizeof(opt_alg_wis_2_key_action)/sizeof(s_key_action) - 1], // last action 
  0, // change_flag
  0, // punt in normaal display
  0, // punt tijdens invoeren
  0, // aantal digits ingevoerd (punt telt niet mee)
  0, // value
  0, // min value
  0, // max value
  &screen_opt_alg_2  // vorige scherm
};
     
static unsigned char Data_Wissen_Possible(unsigned char type)
{
#ifdef PASSWORD
  switch(type)
  {
    case DISP_OPT_WIS_OPTION:
      if((password_enabled & PASSWORD_GEBRUIKER_MASK) == PASSWORD_BEHEERDER) return(1);
      if((password_enabled & PASSWORD_GEBRUIKER_MASK) == PASSWORD_HOTRACO_ADM) return(1);
      if(password_enabled & PASSWORD_ENABLED_OPT_MASK)  return(1);
      break;
    case DISP_OPT_WIS_SETPOI:
      if(password_enabled & PASSWORD_ENABLED_OPT_MASK)  return(1);
      break;
    case DISP_OPT_WIS_VAL_HR:
      if(password_enabled & PASSWORD_ENABLED_OPT_MASK)  return(1);
      break;
    case DISP_OPT_WIS_VALUES:
      if(password_enabled & PASSWORD_ENABLED_OPT_MASK)  return(1);
	  break;
  } 
  return(0);
#else // PASSWORD
  type;
  return((password_enabled & PASSWORD_ENABLED_OPT_MASK) ? 1 : 0);
#endif // PASSWORD
}

void If_Exist_Goto_Screen_Opt_Alg_Wis_2(void)
{
int loop;

  if(password_enabled & PASSWORD_ENABLED_OPT_MASK)
  {
    for(loop = 0; loop < DISP_OPT_WISSEN_MAX; loop++)
    {
      wisdata[loop].possible = Data_Wissen_Possible(loop);
      wisdata[loop].wissen = (wisdata[loop].possible) ? 0 : 2;
    }
    wissen_bevestigen = 0;
    kop_tekst = TEKST_SELECT;
    Control_Screen(&screen_opt_alg_wis_2, &screen_opt_alg_wis_2_default, 1, 3);
    if (screen_opt_alg_wis_2.nr_aantal)
      Next_Screen(&screen_opt_alg_wis_2);
  }
}
//-----------------------------------------------------------------------------
static void Data_Wissen(void)
{
int loop;
unsigned char save_taalkeuze;
  
  for(loop = 0; loop < DISP_OPT_WISSEN_MAX; loop++)
  {
    if(wisdata[loop].wissen == 1)
    {
      switch(loop)
      {
        case DISP_OPT_WIS_OPTION:
		  save_taalkeuze = opt_alg.taalkeuze;
          Data_Default_Opt();
		  opt_alg.taalkeuze = save_taalkeuze;
          ClearAlarm(&alarm_hr_alg.opt_al, SYSTEEM_AL_OPT, 0);
          CAN_IO_Init_All_Boards();
          can_backbone_appl_init_switch = 1;  
          Control_Options_Password();
          dont_restore_opt_setp |= 0x01;
          break;
        case DISP_OPT_WIS_SETPOI:
          Data_Default_Setp();
          ClearAlarm(&alarm_hr_alg.setp_al, SYSTEEM_AL_SETP, 0);
          dont_restore_opt_setp |= 0x02;
          break;
        case DISP_OPT_WIS_VAL_HR:
          Data_Default_Val_Hr();
          ClearAlarm(&alarm_hr_alg.val_hr_al, SYSTEEM_AL_VAL_HR, 0);
          break;
        case DISP_OPT_WIS_VALUES:
          while(1);
      }
      wisdata[loop].wissen = 0;
      kop_tekst = TEKST_GEWIST;
    }
  } 
  dont_restore_opt_setp = 0;  // Bij reset blijft deze waarde bewaard.
}

static void Arrow_Wissen_Func(void)
{
  if(wissen_bevestigen)
  {
    wissen_bevestigen = 0;
    switch (key)
    {
      case UP:    Data_Wissen(); break;
      case DOWN:  break;
      case LEFT:  break;
      case RIGHT: break;
    }
  }
  else
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
        Prev_Screen();          
        Refresh_Screen_Nr_Aantal();
        break;
      case RIGHT: 
        Increment_Func_Index(); 
        break;
    }
  }
}

static void Enter_Wissen_Func(void)
{
  switch(kop_tekst)
  {
    case TEKST_SELECT:    wissen_bevestigen = 0;                           break;
    case TEKST_DRUK_OK:   wissen_bevestigen = (wissen_bevestigen) ? 0 : 1; break;
    case TEKST_GEWIST:    wissen_bevestigen = 0; kop_tekst = TEKST_SELECT; break;
  }
}

static void Disp_Control_Data_Wissen(void)
{
int loop;

  if(screen_ptr->index == 0) 
  {
    for(loop = 0; loop < DISP_OPT_WISSEN_MAX; loop++)
    {
      if(wisdata[loop].wissen == 1)
        kop_tekst = TEKST_DRUK_OK;
    }
  }
  else
  {
    kop_tekst = TEKST_SELECT;
  }
}

static void Arrow_Wissen_Value(void)
{
  switch (key)
  {
	case LEFT:  
	  Decrement_Func_Index(); 
	  break;
	case RIGHT: 
	  Increment_Func_Index(); 
      Increment_Func();
	  break;
    case UP:    
      Increment_Scroll_Value(); 
      break;
	case DOWN:  
	  Decrement_Scroll_Value(); 
	  break;
  }
}

static void Enter_Wissen_Value(void)
{
  Increment_Func_Index();
  Increment_Func();
}









