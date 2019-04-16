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
	
	
	for(i=0; i<10; i++){
		r1=rand();
		r2=rand();
		r3=rand();
		r4=rand();
		r5=rand();
		
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
