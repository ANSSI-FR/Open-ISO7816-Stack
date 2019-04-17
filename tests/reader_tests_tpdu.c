#include "unity.h"
#include "mock_reader_hal.h"
#include "mock_reader_hal_comm_settings.h"
#include "mock_reader_periph.h"

#include "reader_tpdu.h"

#include "reader_tests_tpdu.h"

#include <time.h>
#include <stdio.h>
#include <stdlib.h>




void test_READER_TPDU_all(void){
	RUN_TEST(test_READER_TPDU_Forge_shouldVerifyDataSize);
	RUN_TEST(test_READER_TPDU_Forge_shouldCopyHeader);
	RUN_TEST(test_READER_TPDU_Forge_shouldCopyData);
	RUN_TEST(test_READER_TPDU_SendHeader_shouldSendRightFrame);
	RUN_TEST(test_READER_TPDU_SendDataOneshot_shouldSendRightFrame);
	RUN_TEST(test_READER_TPDU_SendDataOneshot_shouldVerifyDataSize);
	RUN_TEST(test_READER_TPDU_IsACK_shouldWork);
}


void test_READER_TPDU_Forge_shouldVerifyDataSize(void){
	READER_TPDU_Command tpduCmd;
	READER_Status retVal;
	uint8_t pTestBuff[READER_TPDU_MAX_DATA + 1];
	
	
	retVal = READER_TPDU_Forge(&tpduCmd, 0x00, 0x00, 0x00, 0x00, 0x00, pTestBuff, READER_TPDU_MAX_DATA+1);
	TEST_ASSERT_FALSE(retVal == READER_OK);
}


void test_READER_TPDU_Forge_shouldCopyHeader(void){
	READER_TPDU_Command tpduCmd;
	READER_Status retVal;
	uint8_t pTestBuff[READER_TPDU_MAX_DATA];
	uint8_t r1, r2, r3, r4, r5;
	uint32_t i;
	
	
	srand(time(NULL));
	
	for(i=0; i<10; i++){
		r1=(uint8_t)(rand());
		r2=(uint8_t)(rand());
		r3=(uint8_t)(rand());
		r4=(uint8_t)(rand());
		r5=(uint8_t)(rand());
		
		retVal = READER_TPDU_Forge(&tpduCmd, r1, r2, r3, r4, r5, pTestBuff, READER_TPDU_MAX_DATA);
		TEST_ASSERT_TRUE(retVal == READER_OK);
		
		TEST_ASSERT_EQUAL_UINT8(r1, tpduCmd.headerField.CLA);
		TEST_ASSERT_EQUAL_UINT8(r2, tpduCmd.headerField.INS);
		TEST_ASSERT_EQUAL_UINT8(r3, tpduCmd.headerField.P1);
		TEST_ASSERT_EQUAL_UINT8(r4, tpduCmd.headerField.P2);
		TEST_ASSERT_EQUAL_UINT8(r5, tpduCmd.headerField.P3);
	}
}


void test_READER_TPDU_Forge_shouldCopyData(void){
	READER_TPDU_Command tpduCmd;
	READER_Status retVal;
	uint8_t pTestBuff[READER_TPDU_MAX_DATA];
	uint32_t i,j;
	
	srand(time(NULL));
	
	for(j=0; j<10; j++){
		for(i=0; i<READER_TPDU_MAX_DATA; i++){
			pTestBuff[i] = (uint8_t)(rand());
		}
		
		retVal = READER_TPDU_Forge(&tpduCmd, 0x01, 0x02, 0x03, 0x04, 0x05, pTestBuff, READER_TPDU_MAX_DATA);
		TEST_ASSERT_TRUE(retVal == READER_OK);
		
		TEST_ASSERT_EQUAL_UINT8_ARRAY(pTestBuff, tpduCmd.dataField.data, READER_TPDU_MAX_DATA);
	}
}

/* On verifie que la READER_HAL_SendCharFrame() recoit bien la bonne frame a envoyer et de la bonne taille ...   */
void test_READER_TPDU_SendHeader_shouldSendRightFrame(void){
	READER_TPDU_Command tpduCmd;
	READER_HAL_CommSettings dummySettings;
	READER_Status retVal;
	uint8_t pTestBuff[READER_TPDU_MAX_DATA];
	uint8_t r1, r2, r3, r4, r5;
	uint32_t dummyTimeout = 1000;
	uint8_t pExpectedFrame[5];
	
	
	srand(time(NULL));
	
	r1=(uint8_t)(rand());
	r2=(uint8_t)(rand());
	r3=(uint8_t)(rand());
	r4=(uint8_t)(rand());
	r5=(uint8_t)(rand());
	
	retVal = READER_TPDU_Forge(&tpduCmd, r1, r2, r3, r4, r5, pTestBuff, READER_TPDU_MAX_DATA);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	
	pExpectedFrame[0] = r1;
	pExpectedFrame[1] = r2;
	pExpectedFrame[2] = r3;
	pExpectedFrame[3] = r4;
	pExpectedFrame[4] = r5;
	
	READER_HAL_SendCharFrame_ExpectWithArrayAndReturn(&dummySettings, 1, READER_HAL_PROTOCOL_T0, pExpectedFrame, 5, 5, dummyTimeout, READER_OK);
	
	retVal = READER_TPDU_SendHeader(&tpduCmd, dummyTimeout, &dummySettings);
	TEST_ASSERT_TRUE(retVal == READER_OK);
}


void test_READER_TPDU_SendDataOneshot_shouldSendRightFrame(void){
	READER_TPDU_Command tpduCmd;
	READER_HAL_CommSettings dummySettings;
	READER_Status retVal;
	uint8_t pTestBuff[READER_TPDU_MAX_DATA];
	uint8_t pExpectedFrame[READER_TPDU_MAX_DATA];
	uint8_t r1, r2, r3, r4, r5;
	uint32_t dummyTimeout = 1000;
	uint32_t i;
	
	
	/* On prepare un TPDU et on le Forge ...  */
	srand(time(NULL));
	
	r1=(uint8_t)(rand());
	r2=(uint8_t)(rand());
	r3=(uint8_t)(rand());
	r4=(uint8_t)(rand());
	r5=(uint8_t)(rand());
	
	for(i=0; i<READER_TPDU_MAX_DATA; i++){
		pTestBuff[i] = (uint8_t)(rand());
	}
	
	retVal = READER_TPDU_Forge(&tpduCmd, r1, r2, r3, r4, r5, pTestBuff, READER_TPDU_MAX_DATA);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	/* On fait les verifications ...  */
	for(i=0; i<READER_TPDU_MAX_DATA; i++){
		pExpectedFrame[i] = pTestBuff[i];
	}
	
	READER_HAL_SendCharFrame_ExpectWithArrayAndReturn(&dummySettings, 1, READER_HAL_PROTOCOL_T0, pExpectedFrame, READER_TPDU_MAX_DATA, READER_TPDU_MAX_DATA, dummyTimeout, READER_OK);
	
	retVal = READER_TPDU_SendDataOneshot(&tpduCmd, dummyTimeout, &dummySettings);
	TEST_ASSERT_TRUE(retVal == READER_OK);
}



/* On fabrique un TPDU incorrect avec une dataSize plus grande que la taille max acceptee (READER_TPDU_MAX_DATA). On voit ce qui se passe au moment de l'envoi ...  */
void test_READER_TPDU_SendDataOneshot_shouldVerifyDataSize(void){
	READER_TPDU_Command tpduCmd;
	READER_HAL_CommSettings dummySettings;
	READER_Status retVal;
	uint8_t pTestBuff[READER_TPDU_MAX_DATA];
	uint8_t pExpectedFrame[READER_TPDU_MAX_DATA];
	uint8_t r1, r2, r3, r4, r5;
	uint32_t dummyTimeout = 1000;
	uint32_t i;
	
	
	/* On prepare un TPDU et on le Forge ...  */
	srand(time(NULL));
	
	r1=(uint8_t)(rand());
	r2=(uint8_t)(rand());
	r3=(uint8_t)(rand());
	r4=(uint8_t)(rand());
	r5=(uint8_t)(rand());
	
	for(i=0; i<READER_TPDU_MAX_DATA; i++){
		pTestBuff[i] = (uint8_t)(rand());
	}
	
	retVal = READER_TPDU_Forge(&tpduCmd, r1, r2, r3, r4, r5, pTestBuff, READER_TPDU_MAX_DATA);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	/* On modifie manuellement le TPDU de sorte a ce que sa taille soit anormale ...  */
	tpduCmd.dataField.size += READER_TPDU_MAX_DATA;
	
	/* On tente l'envoi et on check la reaction ...  */
	READER_HAL_SendCharFrame_IgnoreAndReturn(READER_OK);
	
	retVal = READER_TPDU_SendDataOneshot(&tpduCmd, dummyTimeout, &dummySettings);
	TEST_ASSERT_FALSE(retVal == READER_OK);
	
}



void test_READER_TPDU_IsACK_shouldWork(void){
	TEST_ASSERT_TRUE(READER_TPDU_IsACK(0x00, 0x00) == READER_OK);
	TEST_ASSERT_TRUE(READER_TPDU_IsACK(0x04, 0x04) == READER_OK);
	TEST_ASSERT_TRUE(READER_TPDU_IsACK(0x17, 0x17) == READER_OK);
	TEST_ASSERT_TRUE(READER_TPDU_IsACK(0x60, 0x60) == READER_NO);
	TEST_ASSERT_TRUE(READER_TPDU_IsACK(0x61, 0x61) == READER_NO);
	TEST_ASSERT_TRUE(READER_TPDU_IsACK(0x69, 0x69) == READER_NO);
	TEST_ASSERT_TRUE(READER_TPDU_IsACK(0x90, 0x90) == READER_NO);
	TEST_ASSERT_TRUE(READER_TPDU_IsACK(0x92, 0x92) == READER_NO);
	TEST_ASSERT_TRUE(READER_TPDU_IsACK(0x99, 0x99) == READER_NO);
	TEST_ASSERT_TRUE(READER_TPDU_IsACK(0x01, 0x04) == READER_NO);
}
