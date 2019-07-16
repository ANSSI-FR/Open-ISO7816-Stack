#ifndef __READER_TESTS_APDU_T1_H__
#define __READER_TESTS_APDU_T1_H__


#include "reader_t1_apdu.h"





void test_READER_T1_APDU_all(void);


void test_T1_ExampleA21_Case3S(void);
void test_T1_ExampleA21_Case3S_shouldIncSeqNumberOnNextCommand(void);
void test_T1_ExampleA21_Case2S(void);
void test_T1_ExampleA21_Case2S_RcvOnlySW(void);
void test_T1_ExampleA21_Case4S(void);
void test_T1_ExampleA21_Case4S_RcvOnlySW(void);
void test_T1_ExampleA22_ValidWTXRequFromCardWhileSendingCommand(void);



#endif
