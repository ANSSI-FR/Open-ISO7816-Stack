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
	uint8_t rcvdBytes[] = {0xA4, 0x90, 0x00};                        /* ACK et SW1SW2 */
	
	set_expected_CharFrame(expectedSentFrame, 5);
	emulate_RcvCharFrame(rcvdBytes, 3);
	
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
	uint8_t rcvdBytes[] = {0x60, 0x60, 0xA4, 0x90, 0x00};                        /* ACK et SW1SW2 */
	
	set_expected_CharFrame(expectedSentFrame, 5);
	emulate_RcvCharFrame(rcvdBytes, 5);
	
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
	uint8_t rcvdBytes[] = {0x60, 0x60, 0xA4, 0x90, 0x00};                        /* ACK et SW1SW2 */
	
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
	uint8_t rcvdBytes[] = {0xA4, 0x10, 0x00};                        /* ACK et SW1SW2 */
	
	set_expected_CharFrame(expectedSentFrame, 5);
	READER_HAL_RcvChar_ExpectAnyArgsAndReturn(READER_OK);
	READER_HAL_RcvChar_ReturnThruPtr_character(rcvdBytes);
	READER_HAL_RcvChar_ExpectAnyArgsAndReturn(READER_TIMEOUT);
	
	/* On execute l'APDU et on verifie e bon fonctionnement ...  */
	retVal = READER_T0_APDU_Execute(&context, &apduCmd, &apduResp);
	TEST_ASSERT_TRUE(retVal == READER_INCORRECT_SW1);
}

