// C__key.C

#include "ch_define.h"

#include "ch_key.h"

// first not used keys in function Key_Check
#define NO_KEY	 36
#define TWO_KEYS 37
#define KEY_INC_CHANGE 9

#define KEY_IN_MASK 0x003F
#define KEY_OUT_ALL 0x00
#define KEY_OUT_0 0xFE
#define KEY_OUT_1 0xFD
#define KEY_OUT_2 0xFB
#define KEY_OUT_3 0xF7
#define KEY_OUT_4 0xEF
#define KEY_OUT_5 0xDF

#define KEY_CNT_MIN 5

#define NO_KEY 36
#define TWO_KEYS 37

#define KEY_WAIT_FOR_KEY 0
#define KEY_CHECKING 1

#if (CLKFREQ==16000000L)
#define SET_T8_1_MS 0xC180
#endif // (CLKFREQ==16000000L)
#if (CLKFREQ==32000000L)
#define SET_T8_1_MS 0x8300
#endif // (CLKFREQ==32000000L)
#if (CLKFREQ==40000000L)
#define SET_T8_1_MS 0x63C0
#endif // (CLKFREQ==40000000L)

#define KEY_IN P5
#define KEY_IN0 P5_0
#define KEY_IN0_D P5D_0
#define KEY_IN1 P5_1
#define KEY_IN1_D P5D_1
#define KEY_IN2 P5_2
#define KEY_IN2_D P5D_2
#define KEY_IN3 P5_3
#define KEY_IN3_D P5D_3
#define KEY_IN4 P5_4
#define KEY_IN4_D P5D_4
#define KEY_IN5 P5_5
#define KEY_IN5_D P5D_5

#pragma class HB=MEM_KEY
volatile unsigned char huge key_out;
#pragma default_attributes

unsigned char key = 0;
unsigned char key_func = 0;
unsigned char key_init_switch = 1;
unsigned char key_status = KEY_WAIT_FOR_KEY;
int key_inc = 1;

void Key_Control(void)
{
  if (key_init_switch)
  {
    KEY_IN0_D = 0;
    KEY_IN1_D = 0;
    KEY_IN2_D = 0;
    KEY_IN3_D = 0;
    KEY_IN4_D = 0;
    KEY_IN5_D = 0;
    key_out = 0;
    
    CC2_T78CON &= 0x00FF; // timer 8 timer mode
    CC2_T8 = SET_T8_1_MS; // 1ms cycle
    CC2_T8REL = SET_T8_1_MS;
    CC2_T8IC = KEY_CHECK_EVENT_LEVEL;
    CC2_T8IC_IE = 1;
     
    key_init_switch = 0;
  }
  switch (key_status)
  {
    case KEY_WAIT_FOR_KEY:
      key_out = 0;
      if ((KEY_IN & KEY_IN_MASK) != KEY_IN_MASK)
      {
        key_status = KEY_CHECKING;
        CC2_T78CON_T8R = 1;
      }
      break;
    case KEY_CHECKING:
      // Key_interrupt is looking what key is pressed
      break;  
  }
}

void Key_Init(void)
{
  key_init_switch = 1;
  Key_Control();
}

interrupt KEY_INT_ADR using(KEY_INT_RB) void Key_Interrupt(void)
{
static unsigned char const array_code[] =
{
  TWO_KEYS, TWO_KEYS, TWO_KEYS, TWO_KEYS, TWO_KEYS, TWO_KEYS, TWO_KEYS, TWO_KEYS,
  TWO_KEYS, TWO_KEYS, TWO_KEYS, TWO_KEYS, TWO_KEYS, TWO_KEYS, TWO_KEYS, TWO_KEYS,
  TWO_KEYS, TWO_KEYS, TWO_KEYS, TWO_KEYS, TWO_KEYS, TWO_KEYS, TWO_KEYS, TWO_KEYS,
  TWO_KEYS, TWO_KEYS, TWO_KEYS, TWO_KEYS, TWO_KEYS, TWO_KEYS, TWO_KEYS,        5,
  TWO_KEYS, TWO_KEYS, TWO_KEYS, TWO_KEYS, TWO_KEYS, TWO_KEYS, TWO_KEYS, TWO_KEYS,
  TWO_KEYS, TWO_KEYS, TWO_KEYS, TWO_KEYS, TWO_KEYS, TWO_KEYS, TWO_KEYS,        4,
  TWO_KEYS, TWO_KEYS, TWO_KEYS, TWO_KEYS, TWO_KEYS, TWO_KEYS, TWO_KEYS,        3,
  TWO_KEYS, TWO_KEYS, TWO_KEYS,        2, TWO_KEYS,        1,        0,   NO_KEY
};
static unsigned char const array_key[] =
{
// last two keys are for NO_KEY and TWO)KEYS
       '1', '2', '3',     0,    0, PREV,
       '4', '5', '6',     0,   UP,    0,
       '7', '8', '9', RIGHT,   OK, LEFT,
  PLUS_MIN, '0', '.',     0, DOWN,    0,
        F6,  F5,  F4,    F3,   F2,   F1,
      NEXT,   0,   0,     0,    0,    0,
         0,   0
};
static unsigned char loop = 0;
static unsigned char key_prev_code = 0;
static unsigned char key_new_code;
static unsigned char key_code;
static int key_pressed_time = 0;
static int key_pressed_delay = 100;
static int key_cnt;
unsigned char code;
static int key_inc_change = KEY_INC_CHANGE;

  switch (loop)
  {
    case 0:
      loop++;
      key_cnt = 0;
      key_pressed_time = 0;
      key_out = KEY_OUT_0;
      break;
    case 1:
      loop++;
      key_new_code = array_code[KEY_IN & 0x003F];
      switch (key_new_code)
      {
        case TWO_KEYS:
          loop = 7;
          break;
      }
      key_out = KEY_OUT_1;
      break;
    case 2:
      loop++;
      code = array_code[KEY_IN & 0x003F];  
      switch (code)
      {
        case NO_KEY:
          break;
        case TWO_KEYS:
          key_new_code = TWO_KEYS;
          loop = 7;
          break;
        default:
          if (key_new_code != NO_KEY)
          {
            key_new_code = TWO_KEYS;
            loop = 7;
          }  
          else
            key_new_code = code + 6;
          break;
      }
      key_out = KEY_OUT_2;
      break;
    case 3:
      loop++;
      code = array_code[KEY_IN & 0x003F];  
      switch (code)
      {
        case NO_KEY:
          break;
        case TWO_KEYS:
          key_new_code = TWO_KEYS;
          loop = 7;
          break;
        default:
          if (key_new_code != NO_KEY)
          {
            key_new_code = TWO_KEYS;
            loop = 7;
          }  
          else
            key_new_code = code + 12;
          break;
      }
      key_out = KEY_OUT_3;
      break;
    case 4:
      loop++;
      code = array_code[KEY_IN & 0x003F];  
      switch (code)
      {
        case NO_KEY:
          break;
        case TWO_KEYS:
          key_new_code = TWO_KEYS;
          loop = 7;
          break;
        default:
          if (key_new_code != NO_KEY)
          {
            key_new_code = TWO_KEYS;
            loop = 7;
          }  
          else
            key_new_code = code + 18;
          break;
      }
      key_out = KEY_OUT_4;
      break;
    case 5:
      loop++;
      code = array_code[KEY_IN & 0x003F];  
      switch (code)
      {
        case NO_KEY:
          break;
        case TWO_KEYS:
          key_new_code = TWO_KEYS;
          loop = 7;
          break;
        default:
          if (key_new_code != NO_KEY)
          {
            key_new_code = TWO_KEYS;
            loop = 7;
          }  
          else
            key_new_code = code + 24;
          break;
      }
      key_out = KEY_OUT_5;
      break;
    case 6:
      loop++;
      code = array_code[KEY_IN & 0x003F];  
      switch (code)
      {
        case NO_KEY:
          break;
        case TWO_KEYS:
          key_new_code = TWO_KEYS;
          break;
        default:
          if (key_new_code != NO_KEY)
            key_new_code = TWO_KEYS;
          else  
            key_new_code = code + 30;
          break;
      }
      break;
    case 7:
      if (key_prev_code != key_new_code)
      {
        key_cnt = 0;
        key_prev_code = key_new_code;
        key_pressed_delay = 100;
		key_inc_change = KEY_INC_CHANGE;
		key_inc = 1;
      }  
      if (key_cnt < KEY_CNT_MIN)
      {
        key_cnt++;
        key_out = KEY_OUT_0;
        loop = 1;
      }
      else
      {
        key_code = array_key[key_new_code];
        loop++;
      }
      break;
    case 8:
      if (key_code == 0)
      {
        // key not long enough pressed or double key then stop timer and wait for next key
        key_cnt = 0;
        key_status = KEY_WAIT_FOR_KEY;
        CC2_T78CON_T8R = 0;
        loop = 0;
      }
      else
      {
        key_pressed_time++;
        if (key_pressed_time > 1000)
          key_pressed_time = 1000;
        if (key_pressed_time == 1)
        {
          if (key_code < F1)
            key = key_code;
          else
            key_func = key_code;
        }
        if (key_pressed_time > key_pressed_delay)
        {
          key_pressed_delay /= 2;
          if (key_pressed_delay < 10)
            key_pressed_delay = 10;
          if (key_code < F1)
            key = key_code;
		  if (key_inc_change <= 0)
		  {
			switch (key_inc)
			{
			  default:
			    key_inc = 1;
				key_inc_change = KEY_INC_CHANGE;
				break;
			  case 1: 
			    key_inc = 10;
		        key_inc_change = KEY_INC_CHANGE;
			    break;
			  case 10:
			    key_inc_change = 0;
			    break;
			}
		  }
		  else
		    key_inc_change--;
          key_pressed_time = 1;
        }
        key_out = KEY_OUT_0;
        loop = 1;
      }
      break;
    default:
      key_status = KEY_WAIT_FOR_KEY;
      CC2_T78CON_T8R = 0;
      loop = 0;
      break;
  }
}
