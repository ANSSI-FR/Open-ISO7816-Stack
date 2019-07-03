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
	RUN_TEST(test_READER_T1_SetBlockData_shouldCheckMaxLength);
	RUN_TEST(test_READER_T1_SetBlockData_shouldUpdateLRC);
	RUN_TEST(test_READER_T1_SetBlockType_shouldIBlockWork);
}



void test_READER_T1_ForgeBlock_shouldSetCorrectInitValues(void){
	READER_T1_Block block;
	READER_Status retVal;
	READER_T1_RedundancyType blockRType;
	uint8_t nad, pcb, len, lrc;
	uint8_t expectedLRC;
	
	
	retVal = READER_T1_ForgeBlock(&block, READER_T1_LRC);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	nad = block.blockFrame[0];
	pcb = block.blockFrame[1];
	len = block.blockFrame[2];
	lrc = block.blockFrame[3];
	blockRType = block.RedundancyType;
	expectedLRC = nad ^ pcb ^ len;
	
	TEST_ASSERT_EQUAL_UINT8(READER_T1_BLOCK_INITIAL_NAD, nad);
	TEST_ASSERT_EQUAL_UINT8(READER_T1_BLOCK_INITIAL_PCB, pcb);
	TEST_ASSERT_EQUAL_UINT8(READER_T1_BLOCK_INITIAL_LEN, len);
	TEST_ASSERT_EQUAL_UINT8(expectedLRC, lrc);
	TEST_ASSERT_TRUE(blockRType == READER_T1_LRC);
}


void test_READER_T1_SetBlockData_shouldCheckMaxLength(void){
	READER_T1_Block block;
	READER_Status retVal;
	uint8_t dataBuff[READER_T1_BLOCK_MAX_DATA_SIZE+1];
	
	retVal = READER_T1_ForgeBlock(&block, READER_T1_LRC);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	retVal = READER_T1_SetBlockData(&block, dataBuff, READER_T1_BLOCK_MAX_DATA_SIZE+1);
	TEST_ASSERT_FALSE(retVal == READER_OK);
}

void test_READER_T1_SetBlockData_shouldUpdateLRC(void){
	READER_T1_Block block;
	READER_Status retVal;
	uint8_t dataBuff[] = {0xFF};
	uint8_t dataLength = 1;
	uint8_t oldLRC, newLRC;
	
	
	retVal = READER_T1_ForgeBlock(&block, READER_T1_LRC);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	retVal = READER_T1_SetBlockData(&block, dataBuff, dataLength);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	oldLRC = READER_T1_GetBlockLRC(&block);
	
	dataBuff[0] = 0xFE;
	retVal = READER_T1_SetBlockData(&block, dataBuff, dataLength);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	newLRC = READER_T1_GetBlockLRC(&block);
	
	TEST_ASSERT_EQUAL_UINT8(oldLRC^0xFF^0xFE, newLRC);
}


void test_READER_T1_SetBlockType_shouldIBlockWork(void){
	READER_T1_Block block;
	READER_Status retVal;
	READER_T1_BlockType bType;
	uint8_t blockPCB;
	
	
	retVal = READER_T1_ForgeBlock(&block, READER_T1_LRC);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	retVal = READER_T1_SetBlockType(&block, READER_T1_IBLOCK);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	bType = READER_T1_GetBlockType(&block);
	TEST_ASSERT_TRUE(bType == READER_T1_IBLOCK);
	
	blockPCB = READER_T1_GetBlockPCB(&block);
	TEST_ASSERT_EQUAL_UINT8(0x00, blockPCB&0x80);
}
