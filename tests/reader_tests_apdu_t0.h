#ifndef __READER_TESTS_APDU_T0_H__
#define __READER_TESTS_APDU_T0_H__


#include "reader_t0_apdu.h"





void test_READER_T0_APDU_all(void);


void test_T0_Case1_shouldWork(void);
void test_T0_Case1_shouldWaitOnNullByte(void);
void test_T0_Case1_shouldTimeout(void);
void test_T0_Case1_shouldDetectIncorrectSW1(void);




#endif
