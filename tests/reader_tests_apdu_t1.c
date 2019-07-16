#include "unity.h"
#include "mock_reader_hal_basis.h"
#include "mock_reader_hal_comm_settings.h"
#include "mock_reader_periph.h"

#include "reader_t1_buffer_handler.h"
#include "reader_t1_context_handler.h"

#include "reader_tests_apdu_t1.h"
#include "reader_tests_test.h"

#include <time.h>
#include <stdio.h>
#include <stdlib.h>







void test_READER_T1_APDU_all(void){
	RUN_TEST(test_T1_ExampleA21_Case3S);
	RUN_TEST(test_T1_ExampleA21_Case3S_shouldIncSeqNumberOnNextCommand);
	RUN_TEST(test_T1_ExampleA21_Case2S);
	RUN_TEST(test_T1_ExampleA21_Case2S_RcvOnlySW);
	RUN_TEST(test_T1_ExampleA21_Case4S);
	RUN_TEST(test_T1_ExampleA21_Case4S_RcvOnlySW);
	RUN_TEST(test_T1_ExampleA22_ValidWTXRequFromCardWhileSendingCommand);
}



void test_T1_ExampleA21_Case3S(void){
	READER_APDU_Command apduCmd;
	READER_APDU_Response apduResp;
	READER_T1_ContextHandler context;
	READER_HAL_CommSettings settings;
	READER_Status retVal;
	uint8_t buff[] = {0x45, 0x75, 0x74, 0x77, 0x74, 0x75, 0x36, 0x41, 0x70, 0x70};
	
	
	/* On forge un APDU ...  */
	retVal = READER_APDU_Forge(&apduCmd, 0x00, 0xA4, 0x04, 0x00, 10, buff, 0);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	/* On initialise l'environnement ...  */
	READER_HAL_InitWithDefaults(&settings);
	READER_HAL_DoColdReset();
	
	retVal = READER_T1_APDU_Init(&context, &settings);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	/* On prepare les Mocks ...   */
	uint8_t expectedSentFrame[] = {0b00000000, 0b00000000, 15, 0x00, 0xA4, 0x04, 0x00, 10, 0x45, 0x75, 0x74, 0x77, 0x74, 0x75, 0x36, 0x41, 0x70, 0x70, 0xE0};
	uint8_t rcvdBytes[] = {0x00, 0b00000000, 2, 0x90, 0x00, 0x92};
	
	set_expected_CharFrame(expectedSentFrame, 19);
	emulate_RcvCharFrame(rcvdBytes, 6);
	
	/* On execute l'APDU et on verfie le resultat ...  */
	retVal = READER_T1_APDU_Execute(&context, &apduCmd, &apduResp);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	TEST_ASSERT_EQUAL_UINT32(0x00000000, apduResp.dataSize);
	TEST_ASSERT_EQUAL_UINT8(0x90, apduResp.SW1);
	TEST_ASSERT_EQUAL_UINT8(0x00, apduResp.SW2);
}


void test_T1_ExampleA21_Case3S_shouldIncSeqNumberOnNextCommand(void){
	READER_APDU_Command apduCmd;
	READER_APDU_Response apduResp;
	READER_T1_ContextHandler context;
	READER_HAL_CommSettings settings;
	READER_Status retVal;
	uint8_t buff[] = {0x45, 0x75, 0x74, 0x77, 0x74, 0x75, 0x36, 0x41, 0x70, 0x70};
	
	
	/* On forge un APDU ...  */
	retVal = READER_APDU_Forge(&apduCmd, 0x00, 0xA4, 0x04, 0x00, 10, buff, 0);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	/* On initialise l'environnement ...  */
	READER_HAL_InitWithDefaults(&settings);
	READER_HAL_DoColdReset();
	
	retVal = READER_T1_APDU_Init(&context, &settings);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	/* On prepare les Mocks ...   */
	uint8_t expectedSentFrame[] = {0b00000000, 0b00000000, 15, 0x00, 0xA4, 0x04, 0x00, 10, 0x45, 0x75, 0x74, 0x77, 0x74, 0x75, 0x36, 0x41, 0x70, 0x70, 0xE0};
	uint8_t rcvdBytes[] = {0x00, 0b00000000, 2, 0x90, 0x00, 0x92};
	
	set_expected_CharFrame(expectedSentFrame, 19);
	emulate_RcvCharFrame(rcvdBytes, 6);
	
	/* On execute l'APDU et on verfie le resultat ...  */
	retVal = READER_T1_APDU_Execute(&context, &apduCmd, &apduResp);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	TEST_ASSERT_EQUAL_UINT32(0x00000000, apduResp.dataSize);
	TEST_ASSERT_EQUAL_UINT8(0x90, apduResp.SW1);
	TEST_ASSERT_EQUAL_UINT8(0x00, apduResp.SW2);
	
	
	/* On forge un deuxieme APDU ...  */
	retVal = READER_APDU_Forge(&apduCmd, 0x00, 0xA4, 0x04, 0x00, 10, buff, 0);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	/* On prepare les Mocks ...   */
	uint8_t expectedSentFrame2[] = {0b00000000, 0b01000000, 15, 0x00, 0xA4, 0x04, 0x00, 10, 0x45, 0x75, 0x74, 0x77, 0x74, 0x75, 0x36, 0x41, 0x70, 0x70, 0xA0};
	uint8_t rcvdBytes2[] = {0x00, 0b01000000, 2, 0x90, 0x00, 0xD2};
	
	set_expected_CharFrame(expectedSentFrame2, 19);
	emulate_RcvCharFrame(rcvdBytes2, 6);
	
	/* On execute l'APDU et on verfie le resultat ...  */
	retVal = READER_T1_APDU_Execute(&context, &apduCmd, &apduResp);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	TEST_ASSERT_EQUAL_UINT32(0x00000000, apduResp.dataSize);
	TEST_ASSERT_EQUAL_UINT8(0x90, apduResp.SW1);
	TEST_ASSERT_EQUAL_UINT8(0x00, apduResp.SW2);
}


void test_T1_ExampleA21_Case2S(void){
	READER_APDU_Command apduCmd;
	READER_APDU_Response apduResp;
	READER_T1_ContextHandler context;
	READER_HAL_CommSettings settings;
	READER_Status retVal;
	uint8_t rcvBuff[READER_APDU_RESP_MAX_TOTALSIZE];
	
	
	/* On forge un APDU ...  */
	retVal = READER_APDU_Forge(&apduCmd, 0x00, 0xA4, 0x04, 0x00, 0, NULL, 5);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	/* On initialise l'environnement ...  */
	READER_HAL_InitWithDefaults(&settings);
	READER_HAL_DoColdReset();
	
	retVal = READER_T1_APDU_Init(&context, &settings);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	
	/* On prepare les Mocks ...   */
	uint8_t expectedSentFrame[] = {0b00000000, 0b00000000, 5, 0x00, 0xA4, 0x04, 0x00, 5, 0xA0};
	uint8_t rcvdBytes[] = {0x00, 0b00000000, 7, 0x75, 0x74, 0x77, 0x74, 0x75, 0x90, 0x00, 0xE0};
	
	set_expected_CharFrame(expectedSentFrame, 9);
	emulate_RcvCharFrame(rcvdBytes, 11);
	
	/* On execute l'APDU et on verfie le resultat ...  */
	retVal = READER_T1_APDU_Execute(&context, &apduCmd, &apduResp);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	TEST_ASSERT_EQUAL_UINT32(0x00000005, apduResp.dataSize);
	TEST_ASSERT_EQUAL_UINT8_ARRAY(rcvdBytes+3, apduResp.dataBytes, 0x00000005);
	TEST_ASSERT_EQUAL_UINT8(0x90, apduResp.SW1);
	TEST_ASSERT_EQUAL_UINT8(0x00, apduResp.SW2);
}


void test_T1_ExampleA21_Case2S_RcvOnlySW(void){
	READER_APDU_Command apduCmd;
	READER_APDU_Response apduResp;
	READER_T1_ContextHandler context;
	READER_HAL_CommSettings settings;
	READER_Status retVal;
	uint8_t rcvBuff[READER_APDU_RESP_MAX_TOTALSIZE];
	
	
	/* On forge un APDU ...  */
	retVal = READER_APDU_Forge(&apduCmd, 0x00, 0xA4, 0x04, 0x00, 0, NULL, 5);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	/* On initialise l'environnement ...  */
	READER_HAL_InitWithDefaults(&settings);
	READER_HAL_DoColdReset();
	
	retVal = READER_T1_APDU_Init(&context, &settings);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	
	/* On prepare les Mocks ...   */
	uint8_t expectedSentFrame[] = {0b00000000, 0b00000000, 5, 0x00, 0xA4, 0x04, 0x00, 5, 0xA0};
	uint8_t rcvdBytes[] = {0x00, 0b00000000, 2, 0x6A, 0x82, 0xEA};
	
	set_expected_CharFrame(expectedSentFrame, 9);
	emulate_RcvCharFrame(rcvdBytes, 6);
	
	/* On execute l'APDU et on verfie le resultat ...  */
	retVal = READER_T1_APDU_Execute(&context, &apduCmd, &apduResp);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	TEST_ASSERT_EQUAL_UINT32(0x00000000, apduResp.dataSize);
	TEST_ASSERT_EQUAL_UINT8(0x6A, apduResp.SW1);
	TEST_ASSERT_EQUAL_UINT8(0x82, apduResp.SW2);
}



void test_T1_ExampleA21_Case4S(void){
	READER_APDU_Command apduCmd;
	READER_APDU_Response apduResp;
	READER_T1_ContextHandler context;
	READER_HAL_CommSettings settings;
	READER_Status retVal;
	uint8_t buff[] = {0x75, 0x36, 0x41, 0x70, 0x70};
	uint32_t Ne = 5;
	uint32_t Nc = 5;
	
	
	if((Ne >= 0x000000FF) || (Nc >= 0x000000FF)){
		TEST_IGNORE();
	}
	
	/* On forge un APDU ...  */
	retVal = READER_APDU_Forge(&apduCmd, 0x00, 0xA4, 0x04, 0x00, Nc, buff, Ne);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	/* On initialise l'environnement ...  */
	READER_HAL_InitWithDefaults(&settings);
	READER_HAL_DoColdReset();
	
	retVal = READER_T1_APDU_Init(&context, &settings);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	/* On prepare les Mocks ...   */
	uint8_t expectedSentFrame[] = {0b00000000, 0b00000000, 11, 0x00, 0xA4, 0x04, 0x00, (uint8_t)(Nc), 0x75, 0x36, 0x41, 0x70, 0x70, (uint8_t)(Ne), 0xA9};
	uint8_t rcvdBytes[] = {0x00, 0b00000000, 7,  0x75, 0x74, 0x77, 0x74, 0x75, 0x90, 0x00, 0xE0};
	
	set_expected_CharFrame(expectedSentFrame, 15);
	emulate_RcvCharFrame(rcvdBytes, 11);
	
	/* On execute l'APDU et on verfie le resultat ...  */
	retVal = READER_T1_APDU_Execute(&context, &apduCmd, &apduResp);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	TEST_ASSERT_EQUAL_UINT32(0x00000005, apduResp.dataSize);
	TEST_ASSERT_EQUAL_UINT8_ARRAY(rcvdBytes+3, apduResp.dataBytes, 5);
	TEST_ASSERT_EQUAL_UINT8(0x90, apduResp.SW1);
	TEST_ASSERT_EQUAL_UINT8(0x00, apduResp.SW2);
}


void test_T1_ExampleA21_Case4S_RcvOnlySW(void){
	READER_APDU_Command apduCmd;
	READER_APDU_Response apduResp;
	READER_T1_ContextHandler context;
	READER_HAL_CommSettings settings;
	READER_Status retVal;
	uint8_t buff[] = {0x75, 0x36, 0x41, 0x70, 0x70};
	uint32_t Ne = 5;
	uint32_t Nc = 5;
	
	
	if((Ne >= 0x000000FF) || (Nc >= 0x000000FF)){
		TEST_IGNORE();
	}
	
	/* On forge un APDU ...  */
	retVal = READER_APDU_Forge(&apduCmd, 0x00, 0xA4, 0x04, 0x00, Nc, buff, Ne);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	/* On initialise l'environnement ...  */
	READER_HAL_InitWithDefaults(&settings);
	READER_HAL_DoColdReset();
	
	retVal = READER_T1_APDU_Init(&context, &settings);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	/* On prepare les Mocks ...   */
	uint8_t expectedSentFrame[] = {0b00000000, 0b00000000, 11, 0x00, 0xA4, 0x04, 0x00, (uint8_t)(Nc), 0x75, 0x36, 0x41, 0x70, 0x70, (uint8_t)(Ne), 0xA9};
	uint8_t rcvdBytes[] = {0x00, 0b00000000, 2, 0x90, 0x00, 0x92};
	
	set_expected_CharFrame(expectedSentFrame, 15);
	emulate_RcvCharFrame(rcvdBytes, 6);
	
	/* On execute l'APDU et on verfie le resultat ...  */
	retVal = READER_T1_APDU_Execute(&context, &apduCmd, &apduResp);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	TEST_ASSERT_EQUAL_UINT32(0x00000000, apduResp.dataSize);
	TEST_ASSERT_EQUAL_UINT8(0x90, apduResp.SW1);
	TEST_ASSERT_EQUAL_UINT8(0x00, apduResp.SW2);
}


void test_T1_ExampleA22_ValidWTXRequFromCardWhileSendingCommand(void){
	READER_APDU_Command apduCmd;
	READER_APDU_Response apduResp;
	READER_T1_ContextHandler context;
	READER_HAL_CommSettings settings;
	READER_Status retVal;
	uint8_t buff[] = {0x75, 0x36, 0x41, 0x70, 0x70};
	uint32_t Ne = 5;
	uint32_t Nc = 5;
	
	/* On doit verifier que : */
	/*      Le lecteur repond correctement a la S-Request */
	/*      Le lecteur applique correctepent en interne la WTX request */
	/*      Le ecteur recupere la reponse correcte au I-Block precedement envoye */
	
	if((Ne >= 0x000000FF) || (Nc >= 0x000000FF)){
		TEST_IGNORE();
	}
	
	/* On forge un APDU ...  */
	retVal = READER_APDU_Forge(&apduCmd, 0x00, 0xA4, 0x04, 0x00, Nc, buff, Ne);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	/* On initialise l'environnement ...  */
	READER_HAL_InitWithDefaults(&settings);
	READER_HAL_DoColdReset();
	
	retVal = READER_T1_APDU_Init(&context, &settings);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	/* On prepare les Mocks ...   */
	uint8_t expectedSentFrame1[] = {0b00000000, 0b00000000, 11, 0x00, 0xA4, 0x04, 0x00, (uint8_t)(Nc), 0x75, 0x36, 0x41, 0x70, 0x70, (uint8_t)(Ne), 0xA9};
	uint8_t rcvdBytes1[] = {0x00, 0b11000011, 1, 0x02, 0xC0};   /* S-Block WTX Request ...  */
	
	set_expected_CharFrame(expectedSentFrame1, 15);
	emulate_RcvCharFrame(rcvdBytes1, 5);
	
	uint8_t expectedSentFrame2[] = {0b00000000, 0b11100011, 1, 0x02, 0xE0};  /* S-Block WTX Response OK */
	uint8_t rcvdBytes2[] = {0x00, 0b00000000, 7,  0x75, 0x74, 0x77, 0x74, 0x75, 0x90, 0x00, 0xE0};   /* I-Block que l'on attendait au depart ...  */
	
	set_expected_CharFrame(expectedSentFrame2, 5);
	emulate_RcvCharFrame(rcvdBytes2, 11);
	
	/* On execute l'APDU et on verfie le resultat ...  */
	retVal = READER_T1_APDU_Execute(&context, &apduCmd, &apduResp);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	TEST_ASSERT_EQUAL_UINT32(0x00000005, apduResp.dataSize);
	TEST_ASSERT_EQUAL_UINT8_ARRAY(rcvdBytes2+3, apduResp.dataBytes, 5);
	TEST_ASSERT_EQUAL_UINT8(0x90, apduResp.SW1);
	TEST_ASSERT_EQUAL_UINT8(0x00, apduResp.SW2);
}

