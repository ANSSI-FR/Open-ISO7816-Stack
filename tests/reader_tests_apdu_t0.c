#include "unity.h"
#include "mock_reader_hal_basis.h"
#include "mock_reader_hal_comm_settings.h"
#include "mock_reader_periph.h"

#include "reader_t1_buffer_handler.h"
#include "reader_t1_context_handler.h"

#include "reader_tests_apdu_t0.h"
#include "reader_tests_test.h"

#include <time.h>
#include <stdio.h>
#include <stdlib.h>







void test_READER_T0_APDU_all(void){
	RUN_TEST(test_T0_Case1_shouldWork);
	RUN_TEST(test_T0_Case1_shouldWaitOnNullByte);
	RUN_TEST(test_T0_Case1_shouldTimeout);
	RUN_TEST(test_T0_Case1_shouldDetectIncorrectSW1);
	RUN_TEST(test_T0_Case2S1_shouldWork);
	RUN_TEST(test_T0_Case2S2_shouldWork);
	RUN_TEST(test_T0_Case2S3_shouldWork_Case1);
	RUN_TEST(test_T0_Case2S3_shouldWork_Case2);
	RUN_TEST(test_T0_Case3S_shouldWork);
	RUN_TEST(test_T0_Case4S1_shouldWork);
	RUN_TEST(test_T0_Case4S2_Case2S1_shouldWork);
	RUN_TEST(test_T0_Case4S2_Case2S2_shouldWork);
	RUN_TEST(test_T0_Case4S2_Case2S3_Case1_shouldWork);
	RUN_TEST(test_T0_Case4S2_Case2S3_Case2_shouldWork);
}




void test_T0_Case1_shouldWork(void){
	READER_APDU_Command apduCmd;
	READER_APDU_Response apduResp;
	READER_T0_ContextHandler context;
	READER_HAL_CommSettings settings;
	READER_Status retVal;
	uint8_t buff[] = {0x45, 0x75, 0x74, 0x77, 0x74, 0x75, 0x36, 0x41, 0x70, 0x70};
	
	/* VOIR ISO7816-3 Section 12.2.2 ...  */
	
	/* On forge un APDU ...  */
	retVal = READER_APDU_Forge(&apduCmd, 0x00, 0xA4, 0x04, 0x00, 0, NULL, 0);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	/* On initialise l'environnement ...  */
	READER_HAL_InitWithDefaults(&settings);
	READER_HAL_DoColdReset();
	
	retVal = READER_T0_APDU_Init(&context, &settings);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	/* On prepare les Mocks ...   */
	uint8_t expectedSentFrame[] = {0x00, 0xA4, 0x04, 0x00, 0x00};    /* TPDU : CLA INS P1 P2 P3=00 */
	uint8_t rcvdBytes[] = {0x90, 0x00};                              /* SW1SW2 */
	
	set_expected_CharFrame(expectedSentFrame, 5);
	emulate_RcvCharFrame(rcvdBytes, 2);
	
	/* On execute l'APDU et on verifie e bon fonctionnement ...  */
	retVal = READER_T0_APDU_Execute(&context, &apduCmd, &apduResp);
	TEST_ASSERT_TRUE_MESSAGE(retVal == READER_OK, "Il s'est produit une erreur lors de l'execution de l'APDU.");
	
	TEST_ASSERT_EQUAL_UINT32(0x00000000, apduResp.dataSize);
	TEST_ASSERT_EQUAL_UINT8(0x90, apduResp.SW1);
	TEST_ASSERT_EQUAL_UINT8(0x00, apduResp.SW2);
}


void test_T0_Case1_shouldWaitOnNullByte(void){
	READER_APDU_Command apduCmd;
	READER_APDU_Response apduResp;
	READER_T0_ContextHandler context;
	READER_HAL_CommSettings settings;
	READER_Status retVal;
	uint8_t buff[] = {0x45, 0x75, 0x74, 0x77, 0x74, 0x75, 0x36, 0x41, 0x70, 0x70};
	
	/* VOIR ISO7816-3 Section 12.2.2 ...  */
	
	/* On forge un APDU ...  */
	retVal = READER_APDU_Forge(&apduCmd, 0x00, 0xA4, 0x04, 0x00, 0, NULL, 0);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	/* On initialise l'environnement ...  */
	READER_HAL_InitWithDefaults(&settings);
	READER_HAL_DoColdReset();
	
	retVal = READER_T0_APDU_Init(&context, &settings);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	/* On prepare les Mocks ...   */
	uint8_t expectedSentFrame[] = {0x00, 0xA4, 0x04, 0x00, 0x00};    /* TPDU : CLA INS P1 P2 P3=00 */
	uint8_t rcvdBytes[] = {0x60, 0x60, 0x90, 0x00};                        /* Null Byte et SW1SW2 */
	
	set_expected_CharFrame(expectedSentFrame, 5);
	emulate_RcvCharFrame(rcvdBytes, 4);
	
	/* On execute l'APDU et on verifie e bon fonctionnement ...  */
	retVal = READER_T0_APDU_Execute(&context, &apduCmd, &apduResp);
	TEST_ASSERT_TRUE_MESSAGE(retVal == READER_OK, "Il s'est produit une erreur lors de l'execution de l'APDU.");
	
	TEST_ASSERT_EQUAL_UINT32(0x00000000, apduResp.dataSize);
	TEST_ASSERT_EQUAL_UINT8(0x90, apduResp.SW1);
	TEST_ASSERT_EQUAL_UINT8(0x00, apduResp.SW2);
}


void test_T0_Case1_shouldTimeout(void){
	READER_APDU_Command apduCmd;
	READER_APDU_Response apduResp;
	READER_T0_ContextHandler context;
	READER_HAL_CommSettings settings;
	READER_Status retVal;
	uint8_t buff[] = {0x45, 0x75, 0x74, 0x77, 0x74, 0x75, 0x36, 0x41, 0x70, 0x70};
	
	/* VOIR ISO7816-3 Section 12.2.2 ...  */
	
	/* On forge un APDU ...  */
	retVal = READER_APDU_Forge(&apduCmd, 0x00, 0xA4, 0x04, 0x00, 0, NULL, 0);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	/* On initialise l'environnement ...  */
	READER_HAL_InitWithDefaults(&settings);
	READER_HAL_DoColdReset();
	
	retVal = READER_T0_APDU_Init(&context, &settings);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	/* On prepare les Mocks ...   */
	uint8_t expectedSentFrame[] = {0x00, 0xA4, 0x04, 0x00, 0x00};    /* TPDU : CLA INS P1 P2 P3=00 */
	uint8_t rcvdBytes[] = {0x60, 0x60, 0x90, 0x00};                        /* Null Byte et SW1SW2 */
	
	set_expected_CharFrame(expectedSentFrame, 5);
	READER_HAL_RcvChar_ExpectAnyArgsAndReturn(READER_OK);
	READER_HAL_RcvChar_ReturnThruPtr_character(rcvdBytes);
	READER_HAL_RcvChar_ExpectAnyArgsAndReturn(READER_TIMEOUT);
	
	/* On execute l'APDU et on verifie e bon fonctionnement ...  */
	retVal = READER_T0_APDU_Execute(&context, &apduCmd, &apduResp);
	TEST_ASSERT_TRUE(retVal == READER_TIMEOUT);
}


void test_T0_Case1_shouldDetectIncorrectSW1(void){
	READER_APDU_Command apduCmd;
	READER_APDU_Response apduResp;
	READER_T0_ContextHandler context;
	READER_HAL_CommSettings settings;
	READER_Status retVal;
	uint8_t buff[] = {0x45, 0x75, 0x74, 0x77, 0x74, 0x75, 0x36, 0x41, 0x70, 0x70};
	
	/* VOIR ISO7816-3 Section 12.2.2 ...  */
	
	/* On forge un APDU ...  */
	retVal = READER_APDU_Forge(&apduCmd, 0x00, 0xA4, 0x04, 0x00, 0, NULL, 0);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	/* On initialise l'environnement ...  */
	READER_HAL_InitWithDefaults(&settings);
	READER_HAL_DoColdReset();
	
	retVal = READER_T0_APDU_Init(&context, &settings);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	/* On prepare les Mocks ...   */
	uint8_t expectedSentFrame[] = {0x00, 0xA4, 0x04, 0x00, 0x00};    /* TPDU : CLA INS P1 P2 P3=00 */
	uint8_t rcvdBytes[] = {0x10, 0x00};                        /* ACK et SW1SW2 */
	
	set_expected_CharFrame(expectedSentFrame, 5);
	emulate_RcvCharFrame(rcvdBytes, 2);
	
	/* On execute l'APDU et on verifie e bon fonctionnement ...  */
	retVal = READER_T0_APDU_Execute(&context, &apduCmd, &apduResp);
	TEST_ASSERT_TRUE(retVal != READER_OK);
}


void test_T0_Case2S1_shouldWork(void){
	READER_APDU_Command apduCmd;
	READER_APDU_Response apduResp;
	READER_T0_ContextHandler context;
	READER_HAL_CommSettings settings;
	READER_Status retVal;
	uint8_t buff[] = {0x45, 0x75, 0x74, 0x77, 0x74, 0x75, 0x36, 0x41, 0x70, 0x70};
	
	/* VOIR ISO7816-3 Section 12.2.3 ...  */
	
	/* On forge un APDU ...  */
	retVal = READER_APDU_Forge(&apduCmd, 0x00, 0xA4, 0x04, 0x00, 0, NULL, 5);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	/* On initialise l'environnement ...  */
	READER_HAL_InitWithDefaults(&settings);
	READER_HAL_DoColdReset();
	
	retVal = READER_T0_APDU_Init(&context, &settings);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	/* On prepare les Mocks ...   */
	uint8_t expectedSentFrame[] = {0x00, 0xA4, 0x04, 0x00, 5};                /* TPDU : CLA INS P1 P2 P3=Le */
	uint8_t rcvdBytes[] = {0xA4, 0x45, 0x75, 0x74, 0x77, 0x74, 0x90, 0x00};   /* ACK, 5 de data et SW1SW2 */
	
	set_expected_CharFrame(expectedSentFrame, 5);
	emulate_RcvCharFrame(rcvdBytes, 8);
	
	/* On execute l'APDU et on verifie le bon fonctionnement ...  */
	retVal = READER_T0_APDU_Execute(&context, &apduCmd, &apduResp);
	TEST_ASSERT_TRUE(retVal == READER_OK);

	TEST_ASSERT_EQUAL_UINT32(0x00000005, apduResp.dataSize);
	TEST_ASSERT_EQUAL_UINT8(0x90, apduResp.SW1);
	TEST_ASSERT_EQUAL_UINT8(0x00, apduResp.SW2);
	TEST_ASSERT_EQUAL_UINT8_ARRAY(apduResp.dataBytes, rcvdBytes+1, 5);
}



void test_T0_Case2S2_shouldWork(void){
	READER_APDU_Command apduCmd;
	READER_APDU_Response apduResp;
	READER_T0_ContextHandler context;
	READER_HAL_CommSettings settings;
	READER_Status retVal;
	uint8_t buff[] = {0x45, 0x75, 0x74, 0x77, 0x74, 0x75, 0x36, 0x41, 0x70, 0x70};
	
	/* VOIR ISO7816-3 Section 12.2.3 ...  */
	
	/* On forge un APDU ...  */
	retVal = READER_APDU_Forge(&apduCmd, 0x00, 0xA4, 0x04, 0x00, 0, NULL, 5);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	/* On initialise l'environnement ...  */
	READER_HAL_InitWithDefaults(&settings);
	READER_HAL_DoColdReset();
	
	retVal = READER_T0_APDU_Init(&context, &settings);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	/* On prepare les Mocks ...   */
	uint8_t expectedSentFrame[] = {0x00, 0xA4, 0x04, 0x00, 5};                /* TPDU : CLA INS P1 P2 P3=Le */
	uint8_t rcvdBytes[] = {0x67, 0x00};   /* ACK, 5 de data et SW1SW2 */
	
	set_expected_CharFrame(expectedSentFrame, 5);
	emulate_RcvCharFrame(rcvdBytes, 2);
	
	/* On execute l'APDU et on verifie le bon fonctionnement ...  */
	retVal = READER_T0_APDU_Execute(&context, &apduCmd, &apduResp);
	TEST_ASSERT_TRUE(retVal == READER_OK);

	TEST_ASSERT_EQUAL_UINT32(0x00000000, apduResp.dataSize);
	TEST_ASSERT_EQUAL_UINT8(0x67, apduResp.SW1);
	TEST_ASSERT_EQUAL_UINT8(0x00, apduResp.SW2);
}



void test_T0_Case2S3_shouldWork_Case1(void){
	READER_APDU_Command apduCmd;
	READER_APDU_Response apduResp;
	READER_T0_ContextHandler context;
	READER_HAL_CommSettings settings;
	READER_Status retVal;
	uint8_t buff[] = {0x45, 0x75, 0x74, 0x77, 0x74, 0x75, 0x36, 0x41, 0x70, 0x70};
	
	
	/* VOIR ISO7816-3 Section 12.2.3 ...  */
	/* Le cas ou Na inferieur a Ne ...    */
	
	/* On forge un APDU ...  */
	retVal = READER_APDU_Forge(&apduCmd, 0x00, 0xA4, 0x04, 0x00, 0, NULL, 5);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	/* On initialise l'environnement ...  */
	READER_HAL_InitWithDefaults(&settings);
	READER_HAL_DoColdReset();
	
	retVal = READER_T0_APDU_Init(&context, &settings);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	/* On prepare les Mocks ...   */
	uint8_t expectedSentFrame[] = {0x00, 0xA4, 0x04, 0x00, 5};     /* TPDU : CLA INS P1 P2 P3=Le */
	uint8_t rcvdBytes[] = {0x6C, 0x03};                            /* SW1SW2 */
	
	set_expected_CharFrame(expectedSentFrame, 5);
	emulate_RcvCharFrame(rcvdBytes, 2);
	
	uint8_t expectedSentFrame2[] = {0x00, 0xA4, 0x04, 0x00, 3};     /* TPDU : CLA INS P1 P2 P3=SW2 */
	uint8_t rcvdBytes2[] = {0x45, 0x75, 0x74, 0x90, 0x00};          /* 3o Data et SW1SW2 */
	
	set_expected_CharFrame(expectedSentFrame2, 5);
	emulate_RcvCharFrame(rcvdBytes2, 5);
	
	/* On execute l'APDU et on verifie le bon fonctionnement ...  */
	retVal = READER_T0_APDU_Execute(&context, &apduCmd, &apduResp);
	TEST_ASSERT_TRUE(retVal == READER_OK);

	TEST_ASSERT_EQUAL_UINT32(0x00000003, apduResp.dataSize);
	TEST_ASSERT_EQUAL_UINT8_ARRAY(apduResp.dataBytes, buff, 3);
	TEST_ASSERT_EQUAL_UINT8(0x90, apduResp.SW1);
	TEST_ASSERT_EQUAL_UINT8(0x00, apduResp.SW2);
}


void test_T0_Case2S3_shouldWork_Case2(void){
	READER_APDU_Command apduCmd;
	READER_APDU_Response apduResp;
	READER_T0_ContextHandler context;
	READER_HAL_CommSettings settings;
	READER_Status retVal;
	uint8_t buff[] = {0x45, 0x75, 0x74, 0x77, 0x74, 0x75, 0x36, 0x41, 0x70, 0x70};
	
	
	/* VOIR ISO7816-3 Section 12.2.3 ...  */
	/* Le cas ou Na superieur a Ne ...    */
	
	/* On forge un APDU ...  */
	retVal = READER_APDU_Forge(&apduCmd, 0x00, 0xA4, 0x04, 0x00, 0, NULL, 5);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	/* On initialise l'environnement ...  */
	READER_HAL_InitWithDefaults(&settings);
	READER_HAL_DoColdReset();
	
	retVal = READER_T0_APDU_Init(&context, &settings);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	/* On prepare les Mocks ...   */
	uint8_t expectedSentFrame[] = {0x00, 0xA4, 0x04, 0x00, 5};     /* TPDU : CLA INS P1 P2 P3=Le */
	uint8_t rcvdBytes[] = {0x6C, 0x03};                            /* SW1SW2 */
	
	set_expected_CharFrame(expectedSentFrame, 5);
	emulate_RcvCharFrame(rcvdBytes, 2);
	
	uint8_t expectedSentFrame2[] = {0x00, 0xA4, 0x04, 0x00, 6};     /* TPDU : CLA INS P1 P2 P3=SW2 */
	uint8_t rcvdBytes2[] = {0x45, 0x75, 0x74, 0x77, 0x74, 0x75, 0x90, 0x00};          /* 6o Data et SW1SW2 */
	
	set_expected_CharFrame(expectedSentFrame2, 5);
	emulate_RcvCharFrame(rcvdBytes2, 8);
	
	/* On execute l'APDU et on verifie le bon fonctionnement ...  */
	retVal = READER_T0_APDU_Execute(&context, &apduCmd, &apduResp);
	TEST_ASSERT_TRUE(retVal == READER_OK);

	TEST_ASSERT_EQUAL_UINT32(0x00000005, apduResp.dataSize);
	TEST_ASSERT_EQUAL_UINT8_ARRAY(apduResp.dataBytes, buff, 5);
	TEST_ASSERT_EQUAL_UINT8(0x90, apduResp.SW1);
	TEST_ASSERT_EQUAL_UINT8(0x00, apduResp.SW2);
}


void test_T0_Case3S_shouldWork(void){
	READER_APDU_Command apduCmd;
	READER_APDU_Response apduResp;
	READER_T0_ContextHandler context;
	READER_HAL_CommSettings settings;
	READER_Status retVal;
	uint8_t buff[] = {0x45, 0x75, 0x74, 0x77, 0x74, 0x75, 0x36, 0x41, 0x70, 0x70};
	
	/* VOIR ISO7816-3 Section 12.2.3 ...  */
	
	/* On forge un APDU ...  */
	retVal = READER_APDU_Forge(&apduCmd, 0x00, 0xA4, 0x04, 0x00, 5, buff, 0);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	/* On initialise l'environnement ...  */
	READER_HAL_InitWithDefaults(&settings);
	READER_HAL_DoColdReset();
	
	retVal = READER_T0_APDU_Init(&context, &settings);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	/* On prepare les Mocks ...   */
	uint8_t expectedSentFrame[] = {0x00, 0xA4, 0x04, 0x00, 5};                /* TPDU : CLA INS P1 P2 P3=Lc */
	uint8_t rcvdBytes[] = {0xA4};                                             /* ACK                        */	
	
	set_expected_CharFrame(expectedSentFrame, 5);
	emulate_RcvCharFrame(rcvdBytes, 1);
	
	uint8_t expectedSentFrame2[] = {0x45, 0x75, 0x74, 0x77, 0x74};            /* 5o de Data                 */
	uint8_t rcvdBytes2[] = {0x90, 0x00};                                      /* SW1SW2                     */
	
	set_expected_CharFrame(expectedSentFrame2, 5);
	emulate_RcvCharFrame(rcvdBytes2, 2);
	
	
	/* On execute l'APDU et on verifie le bon fonctionnement ...  */
	retVal = READER_T0_APDU_Execute(&context, &apduCmd, &apduResp);
	TEST_ASSERT_TRUE(retVal == READER_OK);

	TEST_ASSERT_EQUAL_UINT32(0x00000000, apduResp.dataSize);
	TEST_ASSERT_EQUAL_UINT8(0x90, apduResp.SW1);
	TEST_ASSERT_EQUAL_UINT8(0x00, apduResp.SW2);
}


void test_T0_Case4S1_shouldWork(void){
	READER_APDU_Command apduCmd;
	READER_APDU_Response apduResp;
	READER_T0_ContextHandler context;
	READER_HAL_CommSettings settings;
	READER_Status retVal;
	uint8_t buff1[] = {0x45, 0x75, 0x74, 0x77, 0x74, 0x75, 0x36, 0x41, 0x70, 0x70};
	uint8_t buff2[] = {0x55, 0x85, 0x84, 0x87, 0x84, 0x85, 0x46, 0x51, 0x80, 0x80};
	
	
	/* VOIR ISO7816-3 Section 12.2.4 ...  */
	/* Case 4S1 process aborted ...       */
	
	/* On forge un APDU ...  */
	retVal = READER_APDU_Forge(&apduCmd, 0x00, 0xA4, 0x04, 0x00, 5, buff1, 6);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	/* On initialise l'environnement ...  */
	READER_HAL_InitWithDefaults(&settings);
	READER_HAL_DoColdReset();
	
	retVal = READER_T0_APDU_Init(&context, &settings);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	/* On prepare les Mocks ...   */
	uint8_t expectedSentFrame[] = {0x00, 0xA4, 0x04, 0x00, 5};    
	uint8_t rcvdBytes[] = {0xA4};                             
	
	set_expected_CharFrame(expectedSentFrame, 5);
	emulate_RcvCharFrame(rcvdBytes, 1);
	
	uint8_t expectedSentFrame2[] = {0x45, 0x75, 0x74, 0x77, 0x74};           
	uint8_t rcvdBytes2[] = {0x64, 0x00};                                  
	
	set_expected_CharFrame(expectedSentFrame2, 5);
	emulate_RcvCharFrame(rcvdBytes2, 2);
	
	
	/* On execute l'APDU et on verifie le bon fonctionnement ...  */
	retVal = READER_T0_APDU_Execute(&context, &apduCmd, &apduResp);
	TEST_ASSERT_TRUE(retVal == READER_OK);

	TEST_ASSERT_EQUAL_UINT32(0x00000000, apduResp.dataSize);
	TEST_ASSERT_EQUAL_UINT8(0x64, apduResp.SW1);
	TEST_ASSERT_EQUAL_UINT8(0x00, apduResp.SW2);
}


void test_T0_Case4S2_Case2S1_shouldWork(void){
	READER_APDU_Command apduCmd;
	READER_APDU_Response apduResp;
	READER_T0_ContextHandler context;
	READER_HAL_CommSettings settings;
	READER_Status retVal;
	uint8_t buff1[] = {0x45, 0x75, 0x74, 0x77, 0x74, 0x75, 0x36, 0x41, 0x70, 0x70};
	uint8_t buff2[] = {0x55, 0x85, 0x84, 0x87, 0x84, 0x85, 0x46, 0x51, 0x80, 0x80};
	
	
	/* VOIR ISO7816-3 Section 12.2.4 ...  */
	/* Case 4S2 process completed ...     */
	
	/* On forge un APDU ...  */
	retVal = READER_APDU_Forge(&apduCmd, 0x00, 0xA4, 0x04, 0x00, 5, buff1, 6);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	/* On initialise l'environnement ...  */
	READER_HAL_InitWithDefaults(&settings);
	READER_HAL_DoColdReset();
	
	retVal = READER_T0_APDU_Init(&context, &settings);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	/* On prepare les Mocks ...   */
	uint8_t expectedSentFrame[] = {0x00, 0xA4, 0x04, 0x00, 5};    
	uint8_t rcvdBytes[] = {0xA4};                             
	
	set_expected_CharFrame(expectedSentFrame, 5);
	emulate_RcvCharFrame(rcvdBytes, 1);
	
	uint8_t expectedSentFrame2[] = {0x45, 0x75, 0x74, 0x77, 0x74};           
	uint8_t rcvdBytes2[] = {0x90, 0x00};                                  
	
	set_expected_CharFrame(expectedSentFrame2, 5);
	emulate_RcvCharFrame(rcvdBytes2, 2);
	
	uint8_t expectedSentFrame3[] = {0x00, 0xC0, 0x04, 0x00, 6};
	uint8_t rcvdBytes3[] = {0x55, 0x85, 0x84, 0x87, 0x84, 0x85, 0x90, 0x00};                                  
	
	set_expected_CharFrame(expectedSentFrame3, 5);
	emulate_RcvCharFrame(rcvdBytes3, 8);
	
	
	/* On execute l'APDU et on verifie le bon fonctionnement ...  */
	retVal = READER_T0_APDU_Execute(&context, &apduCmd, &apduResp);
	TEST_ASSERT_TRUE(retVal == READER_OK);

	TEST_ASSERT_EQUAL_UINT32(0x00000006, apduResp.dataSize);
	TEST_ASSERT_EQUAL_UINT8_ARRAY(buff2, apduResp.dataBytes, 6);
	TEST_ASSERT_EQUAL_UINT8(0x90, apduResp.SW1);
	TEST_ASSERT_EQUAL_UINT8(0x00, apduResp.SW2);
}


void test_T0_Case4S2_Case2S2_shouldWork(void){
	READER_APDU_Command apduCmd;
	READER_APDU_Response apduResp;
	READER_T0_ContextHandler context;
	READER_HAL_CommSettings settings;
	READER_Status retVal;
	uint8_t buff1[] = {0x45, 0x75, 0x74, 0x77, 0x74, 0x75, 0x36, 0x41, 0x70, 0x70};
	uint8_t buff2[] = {0x55, 0x85, 0x84, 0x87, 0x84, 0x85, 0x46, 0x51, 0x80, 0x80};
	
	
	/* VOIR ISO7816-3 Section 12.2.4 ...  */
	/* Case 4S2 process completed ...     */
	
	/* On forge un APDU ...  */
	retVal = READER_APDU_Forge(&apduCmd, 0x00, 0xA4, 0x04, 0x00, 5, buff1, 6);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	/* On initialise l'environnement ...  */
	READER_HAL_InitWithDefaults(&settings);
	READER_HAL_DoColdReset();
	
	retVal = READER_T0_APDU_Init(&context, &settings);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	/* On prepare les Mocks ...   */
	uint8_t expectedSentFrame[] = {0x00, 0xA4, 0x04, 0x00, 5};    
	uint8_t rcvdBytes[] = {0xA4};                             
	
	set_expected_CharFrame(expectedSentFrame, 5);
	emulate_RcvCharFrame(rcvdBytes, 1);
	
	uint8_t expectedSentFrame2[] = {0x45, 0x75, 0x74, 0x77, 0x74};           
	uint8_t rcvdBytes2[] = {0x90, 0x00};                                  
	
	set_expected_CharFrame(expectedSentFrame2, 5);
	emulate_RcvCharFrame(rcvdBytes2, 2);
	
	uint8_t expectedSentFrame3[] = {0x00, 0xC0, 0x04, 0x00, 6};
	uint8_t rcvdBytes3[] = {0x67, 0x00};                                  
	
	set_expected_CharFrame(expectedSentFrame3, 5);
	emulate_RcvCharFrame(rcvdBytes3, 2);
	
	
	/* On execute l'APDU et on verifie le bon fonctionnement ...  */
	retVal = READER_T0_APDU_Execute(&context, &apduCmd, &apduResp);
	TEST_ASSERT_TRUE(retVal == READER_OK);

	TEST_ASSERT_EQUAL_UINT32(0x00000000, apduResp.dataSize);
	TEST_ASSERT_EQUAL_UINT8(0x67, apduResp.SW1);
	TEST_ASSERT_EQUAL_UINT8(0x00, apduResp.SW2);
}


void test_T0_Case4S2_Case2S3_Case1_shouldWork(void){
	READER_APDU_Command apduCmd;
	READER_APDU_Response apduResp;
	READER_T0_ContextHandler context;
	READER_HAL_CommSettings settings;
	READER_Status retVal;
	uint8_t buff1[] = {0x45, 0x75, 0x74, 0x77, 0x74, 0x75, 0x36, 0x41, 0x70, 0x70};
	uint8_t buff2[] = {0x55, 0x85, 0x84, 0x87, 0x84, 0x85, 0x46, 0x51, 0x80, 0x80};
	
	
	/* VOIR ISO7816-3 Section 12.2.4 ...  */
	/* Case 4S2 process completed ...     */
	
	/* On forge un APDU ...  */
	retVal = READER_APDU_Forge(&apduCmd, 0x00, 0xA4, 0x04, 0x00, 5, buff1, 6);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	/* On initialise l'environnement ...  */
	READER_HAL_InitWithDefaults(&settings);
	READER_HAL_DoColdReset();
	
	retVal = READER_T0_APDU_Init(&context, &settings);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	/* On prepare les Mocks ...   */
	uint8_t expectedSentFrame[] = {0x00, 0xA4, 0x04, 0x00, 5};    
	uint8_t rcvdBytes[] = {0xA4};                             
	
	set_expected_CharFrame(expectedSentFrame, 5);
	emulate_RcvCharFrame(rcvdBytes, 1);
	
	uint8_t expectedSentFrame2[] = {0x45, 0x75, 0x74, 0x77, 0x74};           
	uint8_t rcvdBytes2[] = {0x90, 0x00};                                  
	
	set_expected_CharFrame(expectedSentFrame2, 5);
	emulate_RcvCharFrame(rcvdBytes2, 2);
	
	uint8_t expectedSentFrame3[] = {0x00, 0xC0, 0x04, 0x00, 6};
	uint8_t rcvdBytes3[] = {0x6C, 0x03};                                  
	
	set_expected_CharFrame(expectedSentFrame3, 5);
	emulate_RcvCharFrame(rcvdBytes3, 2);
	
	uint8_t expectedSentFrame4[] = {0x00, 0xC0, 0x04, 0x00, 3};
	uint8_t rcvdBytes4[] = {0x55, 0x85, 0x84, 0x90, 0x00};                                  
	
	set_expected_CharFrame(expectedSentFrame4, 5);
	emulate_RcvCharFrame(rcvdBytes4, 5);
	
	
	/* On execute l'APDU et on verifie le bon fonctionnement ...  */
	retVal = READER_T0_APDU_Execute(&context, &apduCmd, &apduResp);
	TEST_ASSERT_TRUE(retVal == READER_OK);

	TEST_ASSERT_EQUAL_UINT32(0x00000003, apduResp.dataSize);
	TEST_ASSERT_EQUAL_UINT8_ARRAY(buff2, apduResp.dataBytes, 3);
	TEST_ASSERT_EQUAL_UINT8(0x90, apduResp.SW1);
	TEST_ASSERT_EQUAL_UINT8(0x00, apduResp.SW2);
}


void test_T0_Case4S2_Case2S3_Case2_shouldWork(void){
	READER_APDU_Command apduCmd;
	READER_APDU_Response apduResp;
	READER_T0_ContextHandler context;
	READER_HAL_CommSettings settings;
	READER_Status retVal;
	uint8_t buff1[] = {0x45, 0x75, 0x74, 0x77, 0x74, 0x75, 0x36, 0x41, 0x70, 0x70};
	uint8_t buff2[] = {0x55, 0x85, 0x84, 0x87, 0x84, 0x85, 0x46, 0x51, 0x80, 0x80};
	
	
	/* VOIR ISO7816-3 Section 12.2.4 ...  */
	/* Case 4S2 process completed ...     */
	
	/* On forge un APDU ...  */
	retVal = READER_APDU_Forge(&apduCmd, 0x00, 0xA4, 0x04, 0x00, 5, buff1, 6);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	/* On initialise l'environnement ...  */
	READER_HAL_InitWithDefaults(&settings);
	READER_HAL_DoColdReset();
	
	retVal = READER_T0_APDU_Init(&context, &settings);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	/* On prepare les Mocks ...   */
	uint8_t expectedSentFrame[] = {0x00, 0xA4, 0x04, 0x00, 5};    
	uint8_t rcvdBytes[] = {0xA4};                             
	
	set_expected_CharFrame(expectedSentFrame, 5);
	emulate_RcvCharFrame(rcvdBytes, 1);
	
	uint8_t expectedSentFrame2[] = {0x45, 0x75, 0x74, 0x77, 0x74};           
	uint8_t rcvdBytes2[] = {0x90, 0x00};                                  
	
	set_expected_CharFrame(expectedSentFrame2, 5);
	emulate_RcvCharFrame(rcvdBytes2, 2);
	
	uint8_t expectedSentFrame3[] = {0x00, 0xC0, 0x04, 0x00, 6};
	uint8_t rcvdBytes3[] = {0x6C, 0x06};                                  
	
	set_expected_CharFrame(expectedSentFrame3, 5);
	emulate_RcvCharFrame(rcvdBytes3, 2);
	
	uint8_t expectedSentFrame4[] = {0x00, 0xC0, 0x04, 0x00, 6};
	uint8_t rcvdBytes4[] = {0x55, 0x85, 0x84, 0x87, 0x84, 0x85, 0x90, 0x00};                                  
	
	set_expected_CharFrame(expectedSentFrame4, 5);
	emulate_RcvCharFrame(rcvdBytes4, 8);
	
	
	/* On execute l'APDU et on verifie le bon fonctionnement ...  */
	retVal = READER_T0_APDU_Execute(&context, &apduCmd, &apduResp);
	TEST_ASSERT_TRUE(retVal == READER_OK);

	TEST_ASSERT_EQUAL_UINT32(0x00000005, apduResp.dataSize);
	TEST_ASSERT_EQUAL_UINT8_ARRAY(buff2, apduResp.dataBytes, 5);
	TEST_ASSERT_EQUAL_UINT8(0x90, apduResp.SW1);
	TEST_ASSERT_EQUAL_UINT8(0x00, apduResp.SW2);
}



