#include "unity.h"
#include "mock_reader_hal.h"
#include "mock_reader_hal_comm_settings.h"
#include "mock_reader_periph.h"

#include "reader_t1_block.h"

#include "reader_tests_block.h"

#include <time.h>
#include <stdio.h>
#include <stdlib.h>



void test_READER_BLOCK_all(void){
	RUN_TEST(test_READER_T1_ForgeBlock_shouldSetCorrectInitValues);
}



void test_READER_T1_ForgeBlock_shouldSetCorrectInitValues(void){
	READER_T1_Block block;
	READER_Status retVal;
	READER_T1_RedundancyType blockRType;
	uint8_t nad, pcb, len;
	
	
	retVal = READER_T1_ForgeBlock(&block, READER_T1_LRC);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	nad = block.blockFrame[0];
	pcb = block.blockFrame[1];
	len = block.blockFrame[2];
	blockRType = block.RedundancyType;
	
	TEST_ASSERT_EQUAL_UINT8(READER_T1_BLOCK_INITIAL_NAD, nad);
	TEST_ASSERT_EQUAL_UINT8(READER_T1_BLOCK_INITIAL_PCB, pcb);
	TEST_ASSERT_EQUAL_UINT8(READER_T1_BLOCK_INITIAL_LEN, len);
	TEST_ASSERT_TRUE(blockRType == READER_T1_LRC);
}
