// C__TEST.C

#include "ch_define.h"
#include "ch_test.h"

// LETOP HARDWARE TEST IS ALLEN GEMAATK VOOR C167CS
#ifdef HARDWARE_TEST_0
// sfrbit TEST_bit _atbift(P3, 7); defined in ch_test.h
//sfrbit DTEST_bit _atbit(DP3, 7);
//esfrbit ODTEST_bit _atbit(ODP3, 7);
//sfrbit DTEST_bit_0 _atbit(DP2, 4);
//esfrbit ODTEST_bit_0 _atbit(ODP2, 4);
//sfrbit DTEST_bit_1 _atbit(DP2, 5);
//esfrbit ODTEST_bit_1 _atbit(ODP2, 5);
//sfrbit DTEST_bit_0 _atbit(DP2, 4);
//esfrbit ODTEST_bit_0 _atbit(ODP2, 4);
//sfrbit DTEST_bit_1 _atbit(DP2, 5);
//esfrbit ODTEST_bit_1 _atbit(ODP2, 5);
#endif

#ifdef TIMING_TEST
unsigned int test_cnt = 0;
unsigned int test_normal = 0;
unsigned int test_max = 0;
unsigned int test_min = 0;
unsigned int test_middel = 0;
#endif 

void Test_0_Init(void)
{
#ifdef HARDWARE_TEST_0
//  TEST_bit_0 = 0;
//  ODTEST_bit_0 = 0;
//  DTEST_bit_0 = 1;  
#endif	 
}

void Test_1_Init(void)
{
#ifdef HARDWARE_TEST_1
//  TEST_bit_1 = 0;
//  ODTEST_bit_1 = 0;
//  DTEST_bit_1 = 1;  
#endif	 
}

