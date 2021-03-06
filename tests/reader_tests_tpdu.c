#include "unity.h"
#include "mock_reader_hal_basis.h"
#include "mock_reader_hal_comm_settings.h"
#include "mock_reader_periph.h"

#include "reader_tpdu.h"

#include "reader_tests_tpdu.h"
#include "reader_tests_test.h"

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
	RUN_TEST(test_READER_TPDU_Send_shouldWork);
	RUN_TEST(test_READER_TPDU_Send_shoulWaitOnNullByte);
	RUN_TEST(test_READER_TPDU_IsACK_shouldWork);
	RUN_TEST(test_READER_TPDU_IsXoredACK_shouldWork);
	RUN_TEST(test_READER_TPDU_IsNullByte_shouldWork);
	RUN_TEST(test_READER_TPDU_IsSW1_shouldWork);
	RUN_TEST(test_READER_TPDU_IsProcedureByte_shouldWork);
	RUN_TEST(test_READER_TPDU_WaitACK_shouldDetectINS);
	RUN_TEST(test_READER_TPDU_WaitACK_shouldDetectIfSWInsteadOfACK);
	RUN_TEST(test_READER_TPDU_WaitACK_shouldDetectXoredINS);
	RUN_TEST(test_READER_TPDU_WaitACK_shouldTimeout);
	RUN_TEST(test_READER_TPDU_WaitACK_shouldWaitOnNullByte);
	RUN_TEST(test_READER_TPDU_Send_shouldVerifyDataSize);
	RUN_TEST(test_READER_TPDU_RcvSW_shouldTimeoutOnSW1);
	RUN_TEST(test_READER_TPDU_RcvSW_shouldTimeoutOnSW2);
	RUN_TEST(test_READER_TPDU_RcvSW_shouldWaitOnNullByte);
	//RUN_TEST(test_READER_TPDU_RcvSW_shouldDetectIncorrectSW1); // (On ne realis eplus ce test, depuis le commit  98a44abd95e37ff833a4701eeb7b5ada6fc17ad0 on considere que cette verification doit etre realisee a l'exterieur de la fnction).
	RUN_TEST(test_READER_TPDU_RcvResponse_shouldVerifyExpectedSize);
	RUN_TEST(test_READER_TPDU_RcvSW_shouldReturnCorrectData);
	RUN_TEST(test_READER_TPDU_RcvResponse_shouldTimeout_case1);
	RUN_TEST(test_READER_TPDU_RcvResponse_shouldTimeout_case2);
	RUN_TEST(test_READER_TPDU_RcvResponse_shouldTimeout_case3);
	RUN_TEST(test_READER_TPDU_RcvResponse_shouldTimeout_case4);
	RUN_TEST(test_READER_TPDU_RcvResponse_shouldTimeout_case5);
	RUN_TEST(test_READER_TPDU_RcvResponse_shouldTimeout_case6);
	RUN_TEST(test_READER_TPDU_RcvResponse_shouldTimeout_case7);
	RUN_TEST(test_READER_TPDU_RcvResponse_shouldRetrieveCorrectData);
	RUN_TEST(test_READER_TPDU_RcvResponse_shouldDetectIfSWInsteadOfData_Case1);
	RUN_TEST(test_READER_TPDU_RcvResponse_shouldDetectIfSWInsteadOfData_Case2);
	RUN_TEST(test_READER_TPDU_RcvResponse_shouldDetectIfSWInsteadOfData_Case3);
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
	uint32_t i;
	
	
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
	
	//READER_HAL_SendCharFrame_ExpectWithArrayAndReturn(&dummySettings, 1, READER_HAL_PROTOCOL_T0, pExpectedFrame, 5, 5, dummyTimeout, READER_OK);
	for(i=0; i<5; i++){
		READER_HAL_SendChar_ExpectAndReturn(&dummySettings, READER_HAL_PROTOCOL_T0, pExpectedFrame[i], dummyTimeout, READER_OK);
		READER_HAL_SendChar_IgnoreArg_pSettings();
		READER_HAL_SendChar_IgnoreArg_timeout();
	}
	
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
	
	//mock_reader_hal_basis_Init();
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
	//for(i=0; i<READER_TPDU_MAX_DATA; i++){
	//	pExpectedFrame[i] = pTestBuff[i];
	//}
	
	//READER_HAL_SendCharFrame_ExpectWithArrayAndReturn(&dummySettings, 1, READER_HAL_PROTOCOL_T0, pExpectedFrame, READER_TPDU_MAX_DATA, READER_TPDU_MAX_DATA, dummyTimeout, READER_OK);
	for(i=0; i<READER_TPDU_MAX_DATA; i++){
		//READER_HAL_SendChar_ExpectAnyArgsAndReturn(READER_OK);
		READER_HAL_SendChar_ExpectAndReturn(&dummySettings, READER_HAL_PROTOCOL_T0, pTestBuff[i], dummyTimeout, READER_OK);
		READER_HAL_SendChar_IgnoreArg_pSettings();
		READER_HAL_SendChar_IgnoreArg_timeout();
	}
	
	TEST_ASSERT_EQUAL_UINT32(READER_TPDU_MAX_DATA, i);
	TEST_ASSERT_EQUAL_UINT32(READER_TPDU_MAX_DATA, tpduCmd.dataField.size);

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
	//READER_HAL_SendCharFrame_ExpectAnyArgsAndReturn(READER_OK);
	READER_HAL_SendChar_IgnoreAndReturn(READER_OK);
	
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



void test_READER_TPDU_IsXoredACK_shouldWork(void){
	TEST_ASSERT_TRUE(READER_TPDU_IsXoredACK(0x00, 0x00) == READER_NO);
	TEST_ASSERT_TRUE(READER_TPDU_IsXoredACK(0x04, 0x04) == READER_NO);
	TEST_ASSERT_TRUE(READER_TPDU_IsXoredACK(0x60, 0x60) == READER_NO);
	TEST_ASSERT_TRUE(READER_TPDU_IsXoredACK(0x91, 0x91) == READER_NO);
	TEST_ASSERT_TRUE(READER_TPDU_IsXoredACK(0x95^0xFF, 0x95) == READER_NO);
	TEST_ASSERT_TRUE(READER_TPDU_IsXoredACK(0x65^0xFF, 0x65) == READER_NO);
	TEST_ASSERT_TRUE(READER_TPDU_IsXoredACK(0x04^0xFF, 0x04) == READER_OK);
	TEST_ASSERT_TRUE(READER_TPDU_IsXoredACK(0x17^0xFF, 0x17) == READER_OK);
}


void test_READER_TPDU_IsNullByte_shouldWork(void){
	uint32_t i;
	
	
	TEST_ASSERT_TRUE(READER_TPDU_IsNullByte(0x60) == READER_OK);
	
	for(i=0x00; i<0xFF; i++){
		if(i != 0x60){
			TEST_ASSERT_TRUE(READER_TPDU_IsNullByte((uint8_t)(i)) == READER_NO);
		}
	}
	
}


void test_READER_TPDU_IsSW1_shouldWork(void){
	uint32_t i;
	
	
	for(i=0x00; i<=0xFF; i++){
		if((i>=0x90) && (i<=0x9F)){
			TEST_ASSERT_TRUE(READER_TPDU_IsSW1(i) == READER_OK);
		}
		else if(i == 0x60){
			TEST_ASSERT_TRUE(READER_TPDU_IsSW1(i) == READER_NO);
		}
		else if((i>=0x61) && (i<=0x6F)){
			TEST_ASSERT_TRUE(READER_TPDU_IsSW1(i) == READER_OK);
		}
		else{
			TEST_ASSERT_TRUE(READER_TPDU_IsSW1(i) == READER_NO);
		}
	}
}


void test_READER_TPDU_IsProcedureByte_shouldWork(void){
	uint32_t ins, byte;
	
	
	for(ins=0x00; ins<0xFF; ins++){
		for(byte=0x00; byte<=0xFF; byte++){
			if((byte >= 0x60) && (byte <= 0x6F)){
				TEST_ASSERT_TRUE(READER_TPDU_IsProcedureByte((uint8_t)(byte), (uint8_t)(ins)) == READER_OK);
			}
			else if((byte >= 0x90) && (byte<=0x9F)){
				TEST_ASSERT_TRUE(READER_TPDU_IsProcedureByte((uint8_t)(byte), (uint8_t)(ins)) == READER_OK);
			}
			else if(byte == ins){
				TEST_ASSERT_TRUE(READER_TPDU_IsProcedureByte(byte, ins) == READER_OK);
			}
			else if(byte == (ins^0xFF)){
				TEST_ASSERT_TRUE(READER_TPDU_IsProcedureByte(byte, ins) == READER_OK);
			}
			else{
				TEST_ASSERT_TRUE(READER_TPDU_IsProcedureByte(byte, ins) == READER_NO);
			}
		}	
	}
}


void test_READER_TPDU_WaitACK_shouldDetectINS(void){
	READER_HAL_CommSettings dummySettings;
	READER_Status retVal;
	uint8_t ins;
	uint8_t SW1, SW2;
	uint32_t ACKType;
	uint32_t timeout = 1000;
	
	
	for(ins=0x00; ins<0x60; ins++){
		READER_HAL_RcvChar_ExpectAnyArgsAndReturn(READER_OK);
		READER_HAL_RcvChar_ReturnThruPtr_character(&ins);
		
		retVal = READER_TPDU_WaitACK(ins, &ACKType, &SW1, &SW2, timeout, &dummySettings); 
		
		TEST_ASSERT_TRUE(retVal == READER_OK);
		TEST_ASSERT_EQUAL_UINT32(READER_TPDU_ACK_NORMAL, ACKType);
	}
}


void test_READER_TPDU_WaitACK_shouldDetectIfSWInsteadOfACK(void){
	READER_HAL_CommSettings dummySettings;
	READER_Status retVal;
	uint8_t byte1 = 0x6A;
	uint8_t byte2 = 0x82;
	uint8_t nullByte = 0x60;
	uint8_t ins = 0xA4;
	uint8_t SW1, SW2;
	uint32_t ACKType;
	uint32_t timeout = 1000;
	
	
	READER_HAL_RcvChar_ExpectAnyArgsAndReturn(READER_OK);
	READER_HAL_RcvChar_ReturnThruPtr_character(&nullByte);
	
	READER_HAL_RcvChar_ExpectAnyArgsAndReturn(READER_OK);
	READER_HAL_RcvChar_ReturnThruPtr_character(&byte1);
	
	READER_HAL_RcvChar_ExpectAnyArgsAndReturn(READER_OK);
	READER_HAL_RcvChar_ReturnThruPtr_character(&byte2);
	
	retVal = READER_TPDU_WaitACK(ins, &ACKType, &SW1, &SW2, timeout, &dummySettings); 
	
	TEST_ASSERT_TRUE(retVal == READER_OK);
	TEST_ASSERT_EQUAL_UINT32(READER_TPDU_ACK_SW1, ACKType);
	TEST_ASSERT_EQUAL_UINT8(byte1, SW1);
	TEST_ASSERT_EQUAL_UINT8(byte2, SW2);
}


void test_READER_TPDU_WaitACK_shouldDetectXoredINS(void){
	READER_HAL_CommSettings dummySettings;
	READER_Status retVal;
	uint8_t ins, xIns;
	uint8_t SW1, SW2;
	uint32_t ACKType;
	uint32_t timeout = 1000;
	
	
	for(ins=0x00; ins<10; ins++){
		xIns = ins^0xFF;
		
		READER_HAL_RcvChar_ExpectAnyArgsAndReturn(READER_OK);
		READER_HAL_RcvChar_ReturnThruPtr_character(&xIns);
		
		retVal = READER_TPDU_WaitACK(ins, &ACKType, &SW1, &SW2, timeout, &dummySettings); 
		
		TEST_ASSERT_TRUE(retVal == READER_OK);
		TEST_ASSERT_EQUAL_UINT32(READER_TPDU_ACK_XORED, ACKType);
	}
}


void test_READER_TPDU_WaitACK_shouldTimeout(void){
	READER_HAL_CommSettings dummySettings;
	READER_Status retVal;
	uint32_t ACKType;
	uint8_t ins = 0x04;
	uint8_t SW1, SW2;
	uint32_t timeout = 1000;
	
	
	READER_HAL_RcvChar_ExpectAnyArgsAndReturn(READER_TIMEOUT);
	
	retVal = READER_TPDU_WaitACK(ins, &ACKType, &SW1, &SW2, timeout, &dummySettings);
	TEST_ASSERT_TRUE(retVal == READER_TIMEOUT);
}


void test_READER_TPDU_WaitACK_shouldWaitOnNullByte(void){
	READER_HAL_CommSettings dummySettings;
	READER_Status retVal;
	uint32_t ACKType;
	uint8_t ins = 0x04;
	uint8_t nullByte = 0x60;
	uint8_t SW1, SW2;
	uint32_t timeout = 1000;
	
	
	READER_HAL_RcvChar_ExpectAnyArgsAndReturn(READER_OK);
	READER_HAL_RcvChar_ReturnThruPtr_character(&nullByte);
	READER_HAL_RcvChar_ExpectAnyArgsAndReturn(READER_OK);
	READER_HAL_RcvChar_ReturnThruPtr_character(&nullByte);
	READER_HAL_RcvChar_ExpectAnyArgsAndReturn(READER_OK);
	READER_HAL_RcvChar_ReturnThruPtr_character(&ins);
	
	retVal = READER_TPDU_WaitACK(ins, &ACKType, &SW1, &SW2, timeout, &dummySettings);
	
	TEST_ASSERT_TRUE(retVal == READER_OK);
	TEST_ASSERT_EQUAL_UINT32(READER_TPDU_ACK_NORMAL, ACKType);
}


void test_READER_TPDU_Send_shouldVerifyDataSize(void){
	READER_TPDU_Command tpduCmd;
	READER_TPDU_Response tpduResp;
	READER_HAL_CommSettings dummySettings;
	READER_Status retVal;
	uint32_t timeout = 1000;
	uint8_t ins = 0x04;
	
	
	tpduCmd.dataField.size = READER_TPDU_MAX_DATA + 1;
	tpduCmd.headerField.INS = ins;
	
	//READER_HAL_SendCharFrame_IgnoreAndReturn(READER_OK);
	READER_HAL_SendChar_IgnoreAndReturn(READER_OK);
	READER_HAL_RcvChar_ExpectAnyArgsAndReturn(READER_OK);
	READER_HAL_RcvChar_ReturnThruPtr_character(&ins);
	
	retVal = READER_TPDU_Send(&tpduCmd, &tpduResp, timeout, &dummySettings);
	TEST_ASSERT_FALSE(retVal == READER_OK);
	TEST_ASSERT_FALSE(retVal == READER_GOT_SW1);
}



void test_READER_TPDU_RcvSW_shouldTimeoutOnSW1(void){
	READER_HAL_CommSettings dummySettings;
	READER_Status retVal;
	uint8_t SW1, SW2;
	uint32_t timeout = 1000;
	
	
	READER_HAL_RcvChar_ExpectAnyArgsAndReturn(READER_TIMEOUT);
	
	retVal = READER_TPDU_RcvSW(&SW1, &SW2, timeout, &dummySettings);
	TEST_ASSERT_TRUE(retVal == READER_TIMEOUT_ON_SW1);
}

void test_READER_TPDU_RcvSW_shouldTimeoutOnSW2(void){
	READER_HAL_CommSettings dummySettings;
	READER_Status retVal;
	uint8_t SW1, SW2;
	uint8_t byte = 0x90;
	uint32_t timeout = 1000;
	
	
	READER_HAL_RcvChar_ExpectAnyArgsAndReturn(READER_OK);
	READER_HAL_RcvChar_ReturnThruPtr_character(&byte);
	READER_HAL_RcvChar_ExpectAnyArgsAndReturn(READER_TIMEOUT);
	
	retVal = READER_TPDU_RcvSW(&SW1, &SW2, timeout, &dummySettings);
	TEST_ASSERT_TRUE(retVal == READER_TIMEOUT_ON_SW2);
}


void test_READER_TPDU_RcvSW_shouldWaitOnNullByte(void){
	READER_HAL_CommSettings dummySettings;
	READER_Status retVal;
	uint8_t SW1, SW2;
	uint8_t nullByte = 0x60;
	uint8_t byteSW1 = 0x90;
	uint8_t byteSW2 = 0x00;
	uint32_t timeout = 1000;
	
	
	/* On simule la reception de trois Null Bytes avant le 1er carac de SW ...  */
	READER_HAL_RcvChar_ExpectAnyArgsAndReturn(READER_OK);
	READER_HAL_RcvChar_ReturnThruPtr_character(&nullByte);
	READER_HAL_RcvChar_ExpectAnyArgsAndReturn(READER_OK);
	READER_HAL_RcvChar_ReturnThruPtr_character(&nullByte);
	READER_HAL_RcvChar_ExpectAnyArgsAndReturn(READER_OK);
	READER_HAL_RcvChar_ReturnThruPtr_character(&nullByte);
	
	/* Ensuite on simule la reception de SW1SW2 = 9000 ...  */
	READER_HAL_RcvChar_ExpectAnyArgsAndReturn(READER_OK);
	READER_HAL_RcvChar_ReturnThruPtr_character(&byteSW1);
	READER_HAL_RcvChar_ExpectAnyArgsAndReturn(READER_OK);
	READER_HAL_RcvChar_ReturnThruPtr_character(&byteSW2);
	
	retVal = READER_TPDU_RcvSW(&SW1, &SW2, timeout, &dummySettings);
	
	TEST_ASSERT_TRUE(retVal == READER_OK);
	TEST_ASSERT_EQUAL_UINT8(byteSW1, SW1);
	TEST_ASSERT_EQUAL_UINT8(byteSW2, SW2);
}


void test_READER_TPDU_RcvSW_shouldDetectIncorrectSW1(void){
	READER_HAL_CommSettings dummySettings;
	READER_Status retVal;
	uint8_t SW1, SW2;
	uint8_t byte1 = 0x17;
	uint8_t byte2 = 0x23;
	uint8_t byte3 = 0x01;
	uint32_t timeout = 1000;
	
	
	/* On teste une valeur de 1er carac qui ne correspondent ni a un SW1 ni a un Null Byte ...  */
	READER_HAL_RcvChar_ExpectAnyArgsAndReturn(READER_OK);
	READER_HAL_RcvChar_ReturnThruPtr_character(&byte1);
	
	retVal = READER_TPDU_RcvSW(&SW1, &SW2, timeout, &dummySettings);
	TEST_ASSERT_FALSE(retVal == READER_OK);
	TEST_ASSERT_FALSE(retVal == READER_TIMEOUT);
	
	/* On teste une valeur de 1er carac qui ne correspondent ni a un SW1 ni a un Null Byte ...  */
	READER_HAL_RcvChar_ExpectAnyArgsAndReturn(READER_OK);
	READER_HAL_RcvChar_ReturnThruPtr_character(&byte2);
	
	retVal = READER_TPDU_RcvSW(&SW1, &SW2, timeout, &dummySettings);
	TEST_ASSERT_FALSE(retVal == READER_OK);
	TEST_ASSERT_FALSE(retVal == READER_TIMEOUT);
	
	/* On teste une valeur de 1er carac qui ne correspondent ni a un SW1 ni a un Null Byte ...  */
	READER_HAL_RcvChar_ExpectAnyArgsAndReturn(READER_OK);
	READER_HAL_RcvChar_ReturnThruPtr_character(&byte3);
	
	retVal = READER_TPDU_RcvSW(&SW1, &SW2, timeout, &dummySettings);
	TEST_ASSERT_FALSE(retVal == READER_OK);
	TEST_ASSERT_FALSE(retVal == READER_TIMEOUT);
}


void test_READER_TPDU_RcvSW_shouldReturnCorrectData(void){
	READER_HAL_CommSettings dummySettings;
	READER_Status retVal;
	uint8_t SW1, SW2;
	uint8_t byteSW1 = 0x90;
	uint8_t byteSW2 = 0x00;
	uint32_t timeout = 1000;
	
	
	TEST_ASSERT_TRUE(READER_TPDU_IsSW1(byteSW1) == READER_OK);
	
	READER_HAL_RcvChar_ExpectAnyArgsAndReturn(READER_OK);
	READER_HAL_RcvChar_ReturnThruPtr_character(&byteSW1);
	
	READER_HAL_RcvChar_ExpectAnyArgsAndReturn(READER_OK);
	READER_HAL_RcvChar_ReturnThruPtr_character(&byteSW2);
	
	
	retVal = READER_TPDU_RcvSW(&SW1, &SW2, timeout, &dummySettings);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	TEST_ASSERT_EQUAL_UINT8(byteSW1, SW1);
	TEST_ASSERT_EQUAL_UINT8(byteSW2, SW2);
}


void test_READER_TPDU_RcvResponse_shouldVerifyExpectedSize(void){
	READER_HAL_CommSettings dummySettings;
	READER_TPDU_Response tpduResp;
	READER_Status retVal;
	uint32_t timeout = 1000;
	
	
	/* Cette fonction ne doit pas etre appellee ...  */
	//READER_HAL_RcvCharFrameCount_IgnoreAndReturn(READER_OK);
	
	retVal = READER_TPDU_RcvResponse(&tpduResp, READER_TPDU_MAX_DATA+1, timeout, &dummySettings);
	TEST_ASSERT_TRUE(retVal == READER_OVERFLOW);
}


void test_READER_TPDU_RcvResponse_shouldTimeout_case1(void){
	READER_HAL_CommSettings dummySettings;
	READER_TPDU_Response tpduResp;
	READER_Status retVal;
	uint32_t timeout = 1000;
	uint32_t rcvCount = 1;
	uint8_t dummyData[READER_TPDU_MAX_DATA];
	uint32_t dataSize;
	uint32_t i;
	
	
	/* On initialise le TPDU Resp avec des donnees aleatoires ...  */
	srand(time(NULL));
	tpduResp.SW1 = 0x61;
	tpduResp.SW2 = (uint8_t)(rand());
	tpduResp.dataSize = READER_TPDU_MAX_DATA;
	for(i=0; i< READER_TPDU_MAX_DATA; i++){
		tpduResp.dataBytes[i] = (uint8_t)(rand());
		dummyData[i] = (uint8_t)(rand());
	}
	
	
	//READER_HAL_RcvCharFrameCount_ExpectAnyArgsAndReturn(READER_TIMEOUT);
	//READER_HAL_RcvCharFrameCount_ReturnThruPtr_rcvCount(&rcvCount);
	
	/* On simule un timeout sur le dernier caractere recu ...  */
	dataSize = MAX(1, READER_TPDU_MAX_DATA);
	dataSize = dataSize -1;
	emulate_RcvCharFrame(dummyData, dataSize);
	READER_HAL_RcvChar_ExpectAnyArgsAndReturn(READER_TIMEOUT);
	
	retVal = READER_TPDU_RcvResponse(&tpduResp, READER_TPDU_MAX_DATA, timeout, &dummySettings);
	TEST_ASSERT_TRUE(retVal == READER_TIMEOUT);
}



void test_READER_TPDU_RcvResponse_shouldTimeout_case2(void){
	READER_HAL_CommSettings dummySettings;
	READER_TPDU_Response tpduResp;
	READER_Status retVal;
	uint32_t timeout = 1000;
	uint8_t character = 0x00;
	
	
	READER_HAL_RcvChar_ExpectAnyArgsAndReturn(READER_OK);
	READER_HAL_RcvChar_ReturnThruPtr_character(&character);
	
	READER_HAL_RcvChar_ExpectAnyArgsAndReturn(READER_TIMEOUT);
	
	retVal = READER_TPDU_RcvResponse(&tpduResp, READER_TPDU_MAX_DATA, timeout, &dummySettings);
	TEST_ASSERT_TRUE(retVal == READER_TIMEOUT);
}


void test_READER_TPDU_RcvResponse_shouldTimeout_case3(void){
	READER_HAL_CommSettings dummySettings;
	READER_TPDU_Response tpduResp;
	READER_Status retVal;
	uint8_t data[] = {0xAA};
	uint8_t dataSize = 1;
	uint32_t timeout = 1000;
	
	
	/* On simule la reception des caracteres ...  */
	emulate_RcvCharFrame(data, dataSize);     /* La reception des data se passe corerctement   (mais on a une seule carac data) ...  */
	
	READER_HAL_RcvChar_ExpectAnyArgsAndReturn(READER_TIMEOUT);  /* On simule un timeout sur le SW1 ...  */
	
	
	/* On simule un timeout sur la reception du SW ...  */
	retVal = READER_TPDU_RcvResponse(&tpduResp, dataSize, timeout, &dummySettings);
	TEST_ASSERT_TRUE(retVal == READER_TIMEOUT);
}


void test_READER_TPDU_RcvResponse_shouldTimeout_case4(void){
	READER_HAL_CommSettings dummySettings;
	READER_TPDU_Response tpduResp;
	READER_Status retVal;
	uint8_t data[] = {0xAA, 0xBB};
	uint8_t dataSize = 2;
	uint32_t timeout = 1000;
	
	
	/* On simule le cas ou on a recu uniquement deux carac de data puis timeout sur SW1 ... ressemble a la carte qui envoie SW1SW2 a la place des data, mais ici le premier carac des data n'est pas assimilable a un SW1 ...  */
	/* On simule la reception des caracteres ...  */
	emulate_RcvCharFrame(data, dataSize);     /* La reception des data se passe corerctement   (mais on a une seule carac data) ...  */
	
	READER_HAL_RcvChar_ExpectAnyArgsAndReturn(READER_TIMEOUT);  /* On simule un timeout sur le SW1 ...  */
	
	
	/* On simule un timeout sur la reception du SW ...  */
	retVal = READER_TPDU_RcvResponse(&tpduResp, dataSize, timeout, &dummySettings);
	TEST_ASSERT_TRUE(retVal == READER_TIMEOUT);
}


void test_READER_TPDU_RcvResponse_shouldTimeout_case5(void){
	READER_HAL_CommSettings dummySettings;
	READER_TPDU_Response tpduResp;
	READER_Status retVal;
	uint8_t data[] = {};
	uint8_t dataSize = 0;
	uint8_t characterNotSW1 = 0xAB;
	uint32_t timeout = 1000;
	
	
	/* On simule la reception des caracteres ...  */
	//emulate_RcvCharFrame(data, dataSize);   
	
	READER_HAL_RcvChar_ExpectAnyArgsAndReturn(READER_OK);
	READER_HAL_RcvChar_ReturnThruPtr_character(&characterNotSW1);
	
	READER_HAL_RcvChar_ExpectAnyArgsAndReturn(READER_OK);
	READER_HAL_RcvChar_ReturnThruPtr_character(&characterNotSW1);
	
	/* On simule un timeout sur la reception du SW ...  */
	retVal = READER_TPDU_RcvResponse(&tpduResp, dataSize, timeout, &dummySettings);
	TEST_ASSERT_FALSE(retVal == READER_OK);
}




void test_READER_TPDU_RcvResponse_shouldRetrieveCorrectData(void){
	READER_TPDU_Response tpduResp;
	READER_HAL_CommSettings dummySettings;
	READER_Status retVal;
	uint8_t expectedRespBytes[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05};
	uint32_t expectedRespBytesSize = sizeof(expectedRespBytes);
	uint8_t expectedSW1 = 0x90;
	uint8_t expectedSW2 = 0x00;
	uint8_t SW1, SW2;
	uint32_t timeout = 1000;
	
	
	/* On Mock l'appel a READER_HAL_RcvCharFrameCount() ...  */
	//READER_HAL_RcvCharFrameCount_ExpectAndReturn(&dummySettings, READER_HAL_PROTOCOL_T0, NULL, expectedRespBytesSize, NULL, timeout, READER_OK);
	//READER_HAL_RcvCharFrameCount_IgnoreArg_pSettings();
	//READER_HAL_RcvCharFrameCount_IgnoreArg_frame();
	//READER_HAL_RcvCharFrameCount_IgnoreArg_rcvCount();
	//READER_HAL_RcvCharFrameCount_ReturnArrayThruPtr_frame(expectedRespBytes, expectedRespBytesSize);
	//READER_HAL_RcvCharFrameCount_ReturnThruPtr_rcvCount(&expectedRespBytesSize);
	
	emulate_RcvCharFrame(expectedRespBytes, expectedRespBytesSize);

	
	/* On prepare les Mocks pour la reception du SW1SW2 ...  */
	READER_HAL_RcvChar_ExpectAnyArgsAndReturn(READER_OK);
	READER_HAL_RcvChar_ReturnThruPtr_character(&expectedSW1);
	
	READER_HAL_RcvChar_ExpectAnyArgsAndReturn(READER_OK);
	READER_HAL_RcvChar_ReturnThruPtr_character(&expectedSW2);
	
	/* On efectue l'appel de la fonction que l'on veut tester, on verie la reponse ...  */
	retVal = READER_TPDU_RcvResponse(&tpduResp, expectedRespBytesSize, timeout, &dummySettings);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	TEST_ASSERT_EQUAL_UINT8(expectedSW1, tpduResp.SW1);
	TEST_ASSERT_EQUAL_UINT8(expectedSW2, tpduResp.SW2);
	TEST_ASSERT_EQUAL_UINT32(expectedRespBytesSize, tpduResp.dataSize);
	TEST_ASSERT_EQUAL_UINT8_ARRAY(expectedRespBytes, tpduResp.dataBytes, expectedRespBytesSize);
}


/* Ojn teste le cas ou a la place de recevoir les donnees attendues, on recoit uniquement le SW1SW2 ...  */
void test_READER_TPDU_RcvResponse_shouldDetectIfSWInsteadOfData_Case1(void){
	READER_TPDU_Response tpduResp;
	READER_HAL_CommSettings dummySettings;
	READER_Status retVal;
	uint8_t expectedRespBytes[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05};
	uint32_t expectedRespBytesSize = 6;
	uint32_t reallyRcvdBytesSize = 2;
	uint8_t reallyRcvdBytes[] = {0x90, 0x00};  /* Correspondent a SW1 et SW1 que l'ont recoit a la place des donnees */
	uint32_t timeout = 1000;
	
	

	READER_HAL_RcvChar_ExpectAnyArgsAndReturn(READER_OK);
	READER_HAL_RcvChar_ReturnThruPtr_character(reallyRcvdBytes);
	READER_HAL_RcvChar_ExpectAnyArgsAndReturn(READER_OK);
	READER_HAL_RcvChar_ReturnThruPtr_character(reallyRcvdBytes+1);
	READER_HAL_RcvChar_ExpectAnyArgsAndReturn(READER_TIMEOUT);

	
	/* On effectue l'appel de la fonction que l'on veut tester, on verifie la reponse ...  */
	retVal = READER_TPDU_RcvResponse(&tpduResp, expectedRespBytesSize, timeout, &dummySettings);
	TEST_ASSERT_TRUE(retVal == READER_TIMEOUT_GOT_ONLY_SW);
	
	TEST_ASSERT_EQUAL_UINT8(reallyRcvdBytes[0], tpduResp.SW1);
	TEST_ASSERT_EQUAL_UINT8(reallyRcvdBytes[1], tpduResp.SW2);
	TEST_ASSERT_EQUAL_UINT32(0, tpduResp.dataSize);
}


/* Ici, on simule le cas ou ou attends une reponse avec deux octets de donnees, mais la carte renvoie uniquement SW1SW2 ...  */
void test_READER_TPDU_RcvResponse_shouldDetectIfSWInsteadOfData_Case2(void){
	READER_TPDU_Response tpduResp;
	READER_HAL_CommSettings dummySettings;
	READER_Status retVal;
	uint8_t expectedRespBytes[] = {0x00, 0x01};
	uint32_t expectedRespBytesSize = 2;
	uint32_t reallyRcvdBytesSize = 2;
	uint8_t reallyRcvdBytes[] = {0x6A, 0x82};  /* Correspondent a SW1 et SW1 que l'ont recoit a la place des donnees */
	uint32_t timeout = 1000;
	
	
	emulate_RcvCharFrame(reallyRcvdBytes, reallyRcvdBytesSize);
	
	/* On Mock es eventuels appels a READER_HAL_RcvChar() ...  */
	READER_HAL_RcvChar_ExpectAnyArgsAndReturn(READER_TIMEOUT);
	
	/* On effectue l'appel de la fonction que l'on veut tester, on verifie la reponse ...  */
	retVal = READER_TPDU_RcvResponse(&tpduResp, expectedRespBytesSize, timeout, &dummySettings);
	TEST_ASSERT_TRUE(retVal == READER_TIMEOUT_GOT_ONLY_SW);
	
	TEST_ASSERT_EQUAL_UINT8(reallyRcvdBytes[0], tpduResp.SW1);
	TEST_ASSERT_EQUAL_UINT8(reallyRcvdBytes[1], tpduResp.SW2);
	TEST_ASSERT_EQUAL_UINT32(0, tpduResp.dataSize);
}


/* Ici, on simule le cas ou ou attends une reponse avec un octet de donnees, mais la carte renvoie uniquement SW1SW2 et pas de donnees ...  */
void test_READER_TPDU_RcvResponse_shouldDetectIfSWInsteadOfData_Case3(void){
	READER_TPDU_Response tpduResp;
	READER_HAL_CommSettings dummySettings;
	READER_Status retVal;
	uint8_t expectedRespBytes[] = {0x61};      /* Pour durcir le test on faot expres de mettre un byte qui peut etre assimile a un SW1. */
	uint32_t expectedRespBytesSize = 1;
	uint32_t reallyRcvdBytesSize = 2;
	uint8_t reallyRcvdBytes[] = {0x6A, 0x82};  /* Correspondent a SW1 et SW1 que l'ont recoit a la place des donnees. */
	uint32_t timeout = 1000;
	
	
	emulate_RcvCharFrame(reallyRcvdBytes, reallyRcvdBytesSize);
	READER_HAL_RcvChar_ExpectAnyArgsAndReturn(READER_TIMEOUT);
	
	/* On effectue l'appel de la fonction que l'on veut tester, on verifie la reponse ...  */
	retVal = READER_TPDU_RcvResponse(&tpduResp, expectedRespBytesSize, timeout, &dummySettings);
	TEST_ASSERT_TRUE(retVal == READER_TIMEOUT_GOT_ONLY_SW);
	
	TEST_ASSERT_EQUAL_UINT8(reallyRcvdBytes[0], tpduResp.SW1);
	TEST_ASSERT_EQUAL_UINT8(reallyRcvdBytes[1], tpduResp.SW2);
	TEST_ASSERT_EQUAL_UINT32(0, tpduResp.dataSize);
}



void test_READER_TPDU_Send_shouldWork(void){
	READER_TPDU_Command tpduCmd;
	READER_TPDU_Response tpduResp;
	READER_HAL_CommSettings dummySettings;
	READER_Status retVal;
	uint32_t timeout = 1000;
	uint8_t cla = 0x00;
	uint8_t ins = 0xA4;
	uint8_t p1 = 0x04;
	uint8_t p2 = 0x00;
	uint8_t p3 = 0x0D;
	uint8_t data[0x0D];
	uint8_t ack = ins;
	uint32_t i;
	
	
	/* On initialise des donneees aleatoires a envoyer ...  */
	srand(time(NULL));
	for(i=0; i<0x0D; i++){
		data[i] = (uint8_t)(rand());
	}
	
	/* On paramettre les caracteres que l'on s'attend a etre envoyes et on emule les caracteres recus ...  */
	READER_HAL_SendChar_ExpectAndReturn(&dummySettings, READER_HAL_PROTOCOL_T0, cla, timeout, READER_OK);
	READER_HAL_SendChar_IgnoreArg_pSettings();
	READER_HAL_SendChar_IgnoreArg_timeout();
	
	READER_HAL_SendChar_ExpectAndReturn(&dummySettings, READER_HAL_PROTOCOL_T0, ins, timeout, READER_OK);
	READER_HAL_SendChar_IgnoreArg_pSettings();
	READER_HAL_SendChar_IgnoreArg_timeout();
	
	READER_HAL_SendChar_ExpectAndReturn(&dummySettings, READER_HAL_PROTOCOL_T0, p1, timeout, READER_OK);
	READER_HAL_SendChar_IgnoreArg_pSettings();
	READER_HAL_SendChar_IgnoreArg_timeout();
	
	READER_HAL_SendChar_ExpectAndReturn(&dummySettings, READER_HAL_PROTOCOL_T0, p2, timeout, READER_OK);
	READER_HAL_SendChar_IgnoreArg_pSettings();
	READER_HAL_SendChar_IgnoreArg_timeout();
	
	READER_HAL_SendChar_ExpectAndReturn(&dummySettings, READER_HAL_PROTOCOL_T0, p3, timeout, READER_OK);
	READER_HAL_SendChar_IgnoreArg_pSettings();
	READER_HAL_SendChar_IgnoreArg_timeout();
	
	
	READER_HAL_RcvChar_ExpectAnyArgsAndReturn(READER_OK);
	READER_HAL_RcvChar_ReturnThruPtr_character(&ack);
	
	
	set_expected_CharFrame(data, 0x0D);
	
	
	/* On execute la fonction a tester ...  */
	retVal = READER_TPDU_Forge(&tpduCmd, cla, ins, p1, p2, p3, data, 0x0D);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	retVal = READER_TPDU_Send(&tpduCmd, &tpduResp, timeout, &dummySettings);
	TEST_ASSERT_TRUE(retVal == READER_OK);
}



void test_READER_TPDU_Send_shoulWaitOnNullByte(void){
	READER_TPDU_Command tpduCmd;
	READER_TPDU_Response tpduResp;
	READER_HAL_CommSettings dummySettings;
	READER_Status retVal;
	uint32_t timeout = 1000;
	uint8_t cla = 0x00;
	uint8_t ins = 0xA4;
	uint8_t p1 = 0x04;
	uint8_t p2 = 0x00;
	uint8_t p3 = 0x0D;
	uint8_t data[0x0D];
	uint8_t nullByte = 0x60;   /* Null byte ...  */
	uint8_t ack = ins;
	uint32_t i;
	
	
	/* On initialise des donneees aleatoires a envoyer ...  */
	srand(time(NULL));
	for(i=0; i<0x0D; i++){
		data[i] = (uint8_t)(rand());
	}
	
	/* On paramettre les caracteres que l'on s'attend a etre envoyes et on emule les caracteres recus ...  */
	READER_HAL_SendChar_ExpectAndReturn(&dummySettings, READER_HAL_PROTOCOL_T0, cla, timeout, READER_OK);
	READER_HAL_SendChar_IgnoreArg_pSettings();
	READER_HAL_SendChar_IgnoreArg_timeout();
	
	READER_HAL_SendChar_ExpectAndReturn(&dummySettings, READER_HAL_PROTOCOL_T0, ins, timeout, READER_OK);
	READER_HAL_SendChar_IgnoreArg_pSettings();
	READER_HAL_SendChar_IgnoreArg_timeout();
	
	READER_HAL_SendChar_ExpectAndReturn(&dummySettings, READER_HAL_PROTOCOL_T0, p1, timeout, READER_OK);
	READER_HAL_SendChar_IgnoreArg_pSettings();
	READER_HAL_SendChar_IgnoreArg_timeout();
	
	READER_HAL_SendChar_ExpectAndReturn(&dummySettings, READER_HAL_PROTOCOL_T0, p2, timeout, READER_OK);
	READER_HAL_SendChar_IgnoreArg_pSettings();
	READER_HAL_SendChar_IgnoreArg_timeout();
	
	READER_HAL_SendChar_ExpectAndReturn(&dummySettings, READER_HAL_PROTOCOL_T0, p3, timeout, READER_OK);
	READER_HAL_SendChar_IgnoreArg_pSettings();
	READER_HAL_SendChar_IgnoreArg_timeout();
	
	
	READER_HAL_RcvChar_ExpectAnyArgsAndReturn(READER_OK);
	READER_HAL_RcvChar_ReturnThruPtr_character(&nullByte);
	
	READER_HAL_RcvChar_ExpectAnyArgsAndReturn(READER_OK);
	READER_HAL_RcvChar_ReturnThruPtr_character(&nullByte);
	
	READER_HAL_RcvChar_ExpectAnyArgsAndReturn(READER_OK);
	READER_HAL_RcvChar_ReturnThruPtr_character(&ack);
	
	
	set_expected_CharFrame(data, 0x0D);
	
	
	/* On execute la fonction a tester ...  */
	retVal = READER_TPDU_Forge(&tpduCmd, cla, ins, p1, p2, p3, data, 0x0D);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	retVal = READER_TPDU_Send(&tpduCmd, &tpduResp, timeout, &dummySettings);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
}


void test_READER_TPDU_RcvResponse_shouldTimeout_case6(void){
	READER_HAL_CommSettings dummySettings;
	READER_TPDU_Response tpduResp;
	READER_Status retVal;
	uint8_t expectedDataSize = 2;
	uint8_t characterNotSW1 = 0xAB;
	uint8_t characterSW2 = 0xCD;
	uint32_t timeout = 1000;
	
	
	/* On se met dans le cas ou on recoit uniquement SW1SW2 alors que on attendait 2 octets de donnees. Le SW1 recu est incorrect, devrait etre detecte par la fonction ...  */ 
	
	READER_HAL_RcvChar_ExpectAnyArgsAndReturn(READER_OK);
	READER_HAL_RcvChar_ReturnThruPtr_character(&characterNotSW1);
	
	READER_HAL_RcvChar_ExpectAnyArgsAndReturn(READER_OK);
	READER_HAL_RcvChar_ReturnThruPtr_character(&characterSW2);
	
	READER_HAL_RcvChar_ExpectAnyArgsAndReturn(READER_TIMEOUT);
	
	
	/* On simule un timeout sur la reception du SW ...  */
	retVal = READER_TPDU_RcvResponse(&tpduResp, expectedDataSize, timeout, &dummySettings);
	TEST_ASSERT_TRUE(retVal == READER_TIMEOUT);
}


void test_READER_TPDU_RcvResponse_shouldTimeout_case7(void){
	READER_HAL_CommSettings dummySettings;
	READER_TPDU_Response tpduResp;
	READER_Status retVal;
	uint8_t expectedDataSize = 1;
	uint8_t characterNotSW1 = 0xAB;
	uint8_t characterSW2 = 0xCD;
	uint32_t timeout = 1000;
	
	
	/* On se met dans le cas ou on recoit uniquement SW1SW2 alors que on attendait 1 octet dee donnees. Le SW1 recu est incorrect, devrait etre detecte par la fonction (on est dans le cas timeout sur SW2) ...  */ 
	
	READER_HAL_RcvChar_ExpectAnyArgsAndReturn(READER_OK);
	READER_HAL_RcvChar_ReturnThruPtr_character(&characterNotSW1);
	
	READER_HAL_RcvChar_ExpectAnyArgsAndReturn(READER_OK);
	READER_HAL_RcvChar_ReturnThruPtr_character(&characterSW2);
	
	READER_HAL_RcvChar_ExpectAnyArgsAndReturn(READER_TIMEOUT);
	
	
	/* On simule un timeout sur la reception du SW ...  */
	retVal = READER_TPDU_RcvResponse(&tpduResp, expectedDataSize, timeout, &dummySettings);
	TEST_ASSERT_TRUE(retVal == READER_TIMEOUT);
}


