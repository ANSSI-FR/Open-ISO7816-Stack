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
void test_T1_ExampleA23_Scenario3_ValidIFSRequFromCardWhileSendingCommand(void);
void test_T1_ExampleA23_Scenario4_IFSRequFromDeviceAcceptedByCard(void);
void test_T1_ExampleA24_Scenario5_DeviceTransmitsChain(void);
void test_T1_ExampleA24_Scenario6_CardTransmitsChain(void);
void test_T1_ExampleA31_Scenario8(void);
void test_T1_ExampleA31_Scenario9_CaseErrorOnLRC(void);
void test_T1_ExampleA31_Scenario9_CaseTimeout(void);
void test_T1_ExampleA31_Scenario10(void);
void test_T1_ExampleA31_Scenario11_CaseLRCError(void);
void test_T1_ExampleA31_Scenario11_CaseTimeoutOn112(void);
void test_T1_ExampleA31_Scenario12(void);
void test_T1_ExampleA31_Scenario13(void);







#endif
