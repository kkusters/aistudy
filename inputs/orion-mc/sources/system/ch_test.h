// CH_TEST.H

#ifndef _CH_TEST_H
#define _CH_TEST_H

//#define HARDWARE_TEST_0 1
//#define HARDWARE_TEST_1 1
#define TIMING_TEST 1

#ifdef HARDWARE_TEST_0
//sfrbit TEST_bit _atbit(P3, 7); // don't use directly in program, 
//                               // but use the function WDI_Trigger()
//sfrbit TEST_bit_0 _atbit(P2, 4); // don't use directly in program, 
//                               // but use the function WDI_Trigger()
sfrbit TEST_bit_0 _atbit(P9, 2); // LETOP dit is van i2c1
#endif
#ifdef HARDWARE_TEST_1
//sfrbit TEST_bit _atbit(P3, 7); // don't use directly in program, 
//                               // but use the function WDI_Trigger()
//sfrbit TEST_bit_1 _atbit(P2, 5); // don't use directly in program, 
//                               // but use the function WDI_Trigger()
#endif

#ifdef TIMING_TEST
extern unsigned int test_cnt;
extern unsigned int test_normal;
extern unsigned int test_max;
extern unsigned int test_min;
extern unsigned int test_middel;
#endif 

void Test_0_Init(void);
void Test_1_Init(void);

_inline void TEST_BIT_ON(void)
{
#ifdef TIMING_TEST
  test_cnt = 1;
#endif
}

_inline void TEST_BIT_0_ON(void)
{
#ifdef HARDWARE_TEST_0
  TEST_bit_0 = 0;
#endif
}

_inline void TEST_BIT_1_ON(void)
{
#ifdef HARDWARE_TEST_1
  TEST_bit_1 = 0;
#endif
}

_inline void TEST_BIT_OFF(void)
{
#ifdef TIMING_TEST
unsigned int help;

  help = test_cnt;
  test_normal = help;

  if (help > test_middel)
    test_middel++;
  else if (help < test_middel)
    test_middel--;

  if (test_min > help)
    test_min = help;
  if (test_max < help)
    test_max = help;
#endif
}

_inline void TEST_BIT_0_OFF(void)
{
#ifdef HARDWARE_TEST_0
  TEST_bit_0 = 1;
#endif
}

_inline void TEST_BIT_1_OFF(void)
{
#ifdef HARDWARE_TEST_1
  TEST_bit_1 = 1;
#endif
}

_inline void TEST_BIT_TOGGLE(void)
{
#ifdef TIMING_TEST
unsigned int help;

  help = test_cnt;
  test_cnt = 0;
  test_normal = help;

  test_middel = (test_middel + help) / 2;

  if (test_min > help)
    test_min = help;
  if (test_max < help)
    test_max = help;
#endif
}

_inline void TEST_BIT_0_TOGGLE(void)
{
#ifdef HARDWARE_TEST_0
  TEST_bit_0 = ~TEST_bit_0;
#endif
}

_inline void TEST_BIT_1_TOGGLE(void)
{
#ifdef HARDWARE_TEST_1
  TEST_bit_1 = ~TEST_bit_1;
#endif
}

_inline void TEST_RESET(void)
{
#ifdef TIMING_TEST
  test_min = test_max = test_normal = test_cnt = test_middel = 0;
#endif
}

_inline void TEST_0_RESET(void)
{
#ifdef HARDWARE_TEST_0
  TEST_bit_0 = 1;
#endif
}

_inline void TEST_1_RESET(void)
{
#ifdef HARDWARE_TEST_0
  TEST_bit_0 = 1;
#endif
}

#endif