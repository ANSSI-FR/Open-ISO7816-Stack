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
	RUN_TEST(test_READER_T1_SetBlockType_shouldRBlockWork);
	RUN_TEST(test_READER_T1_SetBlockType_shouldSBlockWork);
	RUN_TEST(test_READER_T1_SetBlockData_shouldUpdateLEN);
	RUN_TEST(test_READER_T1_SetBlockMBit_shouldWork);
	RUN_TEST(test_READER_T1_SetBlockSeqNumber_shouldWork);
	RUN_TEST(test_READER_T1_ComputeBlockLRC_shouldWork);
	RUN_TEST(test_READER_T1_SetBlockSType_shouldResynchRequestWork);
	RUN_TEST(test_READER_T1_SetBlockSType_shouldResynchResponseWork);
	RUN_TEST(test_READER_T1_SetBlockSType_shouldIFSRequestWork);
	RUN_TEST(test_READER_T1_SetBlockSType_shouldIFSResponseWork);
	RUN_TEST(test_READER_T1_SetBlockSType_shouldWTXRequestWork);
	RUN_TEST(test_READER_T1_SetBlockSType_shouldWTXResponseWork);
	RUN_TEST(test_READER_T1_GetBlockPCB_shouldWork);
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


void test_READER_T1_SetBlockType_shouldRBlockWork(void){
	READER_T1_Block block;
	READER_Status retVal;
	READER_T1_BlockType bType;
	uint8_t blockPCB;
	
	
	retVal = READER_T1_ForgeBlock(&block, READER_T1_LRC);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	retVal = READER_T1_SetBlockType(&block, READER_T1_RBLOCK);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	bType = READER_T1_GetBlockType(&block);
	TEST_ASSERT_TRUE(bType == READER_T1_RBLOCK);
	
	blockPCB = READER_T1_GetBlockPCB(&block);
	TEST_ASSERT_EQUAL_UINT8(0x80, blockPCB&0xC0);
}

void test_READER_T1_SetBlockType_shouldSBlockWork(void){
	READER_T1_Block block;
	READER_Status retVal;
	READER_T1_BlockType bType;
	uint8_t blockPCB;
	
	
	retVal = READER_T1_ForgeBlock(&block, READER_T1_LRC);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	retVal = READER_T1_SetBlockType(&block, READER_T1_SBLOCK);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	bType = READER_T1_GetBlockType(&block);
	TEST_ASSERT_TRUE(bType == READER_T1_SBLOCK);
	
	blockPCB = READER_T1_GetBlockPCB(&block);
	TEST_ASSERT_EQUAL_UINT8(0xC0, blockPCB&0xC0);
}



void test_READER_T1_SetBlockData_shouldUpdateLEN(void){
	READER_T1_Block block;
	READER_Status retVal;
	uint8_t data1[] = {0x00, 0x01, 0x02};
	uint8_t data1Length = 3;
	uint8_t data2[] = {0x00, 0x01, 0x02, 0x03};
	uint8_t data2Length = 4;
	
	
	retVal = READER_T1_ForgeIBlock(&block, data1, data1Length, READER_T1_SEQNUM_ZERO, READER_T1_MBIT_ZERO, READER_T1_LRC);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	TEST_ASSERT_EQUAL_UINT8(data1Length, READER_T1_GetBlockLEN(&block));
	
	retVal = READER_T1_SetBlockData(&block, data2, data2Length);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	TEST_ASSERT_EQUAL_UINT8(data2Length, READER_T1_GetBlockLEN(&block));
}


void test_READER_T1_SetBlockMBit_shouldWork(void){
	READER_T1_Block block;
	READER_Status retVal;
	READER_T1_MBit mBit;
	uint8_t data[] = {0x00, 0x01};
	uint8_t dataSize = 2;
	
	
	retVal = READER_T1_ForgeIBlock(&block, data, dataSize, READER_T1_SEQNUM_ZERO, READER_T1_MBIT_ZERO, READER_T1_LRC);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	mBit = READER_T1_GetBlockMBit(&block);
	TEST_ASSERT_TRUE(mBit == READER_T1_MBIT_ZERO);
	
	retVal = READER_T1_SetBlockMBit(&block, READER_T1_MBIT_ONE);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	mBit = READER_T1_GetBlockMBit(&block);
	TEST_ASSERT_TRUE(mBit == READER_T1_MBIT_ONE);
}



void test_READER_T1_SetBlockSeqNumber_shouldWork(void){
	READER_T1_Block block;
	READER_Status retVal;
	READER_T1_MBit seqNum;
	uint8_t data[] = {0x00, 0x01};
	uint8_t dataSize = 2;
	
	
	retVal = READER_T1_ForgeIBlock(&block, data, dataSize, READER_T1_SEQNUM_ZERO, READER_T1_MBIT_ZERO, READER_T1_LRC);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	seqNum = READER_T1_GetBlockSeqNumber(&block);
	TEST_ASSERT_TRUE(seqNum == READER_T1_SEQNUM_ZERO);
	
	retVal = READER_T1_SetBlockSeqNumber(&block, READER_T1_SEQNUM_ONE);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	seqNum = READER_T1_GetBlockSeqNumber(&block);
	TEST_ASSERT_TRUE(seqNum == READER_T1_SEQNUM_ONE);
}


void test_READER_T1_ComputeBlockLRC_shouldWork(void){
	READER_T1_Block block;
	READER_Status retVal;
	uint8_t nad, pcb, len, lrc;
	uint8_t expectedLrc;
	uint8_t data[1];
	
	srand(time(NULL));
	
	len = 1;
	nad = (uint8_t)(rand());
	pcb = (uint8_t)(rand());
	data[0] = (uint8_t)(rand());
	
	retVal = READER_T1_ForgeBlock(&block, READER_T1_LRC);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	retVal = READER_T1_SetBlockNAD(&block, nad);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	retVal = READER_T1_SetBlockPCB(&block, pcb);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	retVal = READER_T1_SetBlockLEN(&block, len);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	retVal = READER_T1_SetBlockData(&block, data, len);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	
	lrc = READER_T1_ComputeBlockLRC(&block);
	
	expectedLrc = nad ^ pcb ^ len ^ data[0];
	
	TEST_ASSERT_EQUAL_UINT8(expectedLrc, lrc);
}


void test_READER_T1_SetBlockSType_shouldResynchRequestWork(void){
	READER_T1_Block block;
	READER_T1_SBlockType sBlockType;
	READER_T1_BlockType bType;
	READER_Status retVal;
	uint8_t blockPCB;
	
	
	retVal = READER_T1_ForgeSBlock(&block, READER_T1_STYPE_RESYNCH_REQU, READER_T1_LRC);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	bType = READER_T1_GetBlockType(&block);
	TEST_ASSERT_TRUE(bType == READER_T1_SBLOCK);
	
	sBlockType = READER_T1_GetBlockSType(&block);
	TEST_ASSERT_TRUE(sBlockType == READER_T1_STYPE_RESYNCH_REQU);

	blockPCB = READER_T1_GetBlockPCB(&block);
	TEST_ASSERT_EQUAL_UINT8(0b00000000, blockPCB&0b00111111);       /* Voir ISO7816-3 section 11.3.2.2 */
}


void test_READER_T1_SetBlockSType_shouldResynchResponseWork(void){
	READER_T1_Block block;
	READER_T1_SBlockType sBlockType;
	READER_T1_BlockType bType;
	READER_Status retVal;
	uint8_t blockPCB;
	
	
	retVal = READER_T1_ForgeSBlock(&block, READER_T1_STYPE_RESYNCH_RESP, READER_T1_LRC);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	bType = READER_T1_GetBlockType(&block);
	TEST_ASSERT_TRUE(bType == READER_T1_SBLOCK);
	
	sBlockType = READER_T1_GetBlockSType(&block);
	TEST_ASSERT_TRUE(sBlockType == READER_T1_STYPE_RESYNCH_RESP);

	blockPCB = READER_T1_GetBlockPCB(&block);
	TEST_ASSERT_EQUAL_UINT8(0b00100000, blockPCB&0b00111111);       /* Voir ISO7816-3 section 11.3.2.2 */
}



void test_READER_T1_SetBlockSType_shouldIFSRequestWork(void){
	READER_T1_Block block;
	READER_T1_SBlockType sBlockType;
	READER_T1_BlockType bType;
	READER_Status retVal;
	uint8_t blockPCB;
	
	
	retVal = READER_T1_ForgeSBlock(&block, READER_T1_STYPE_IFS_REQU, READER_T1_LRC);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	bType = READER_T1_GetBlockType(&block);
	TEST_ASSERT_TRUE(bType == READER_T1_SBLOCK);
	
	sBlockType = READER_T1_GetBlockSType(&block);
	TEST_ASSERT_TRUE(sBlockType == READER_T1_STYPE_IFS_REQU);

	blockPCB = READER_T1_GetBlockPCB(&block);
	TEST_ASSERT_EQUAL_UINT8(0b00000001, blockPCB&0b00111111);       /* Voir ISO7816-3 section 11.3.2.2 */
}


void test_READER_T1_SetBlockSType_shouldIFSResponseWork(void){
	READER_T1_Block block;
	READER_T1_SBlockType sBlockType;
	READER_T1_BlockType bType;
	READER_Status retVal;
	uint8_t blockPCB;
	
	
	retVal = READER_T1_ForgeSBlock(&block, READER_T1_STYPE_IFS_RESP, READER_T1_LRC);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	bType = READER_T1_GetBlockType(&block);
	TEST_ASSERT_TRUE(bType == READER_T1_SBLOCK);
	
	sBlockType = READER_T1_GetBlockSType(&block);
	TEST_ASSERT_TRUE(sBlockType == READER_T1_STYPE_IFS_RESP);

	blockPCB = READER_T1_GetBlockPCB(&block);
	TEST_ASSERT_EQUAL_UINT8(0b00100001, blockPCB&0b00111111);       /* Voir ISO7816-3 section 11.3.2.2 */
}

void test_READER_T1_SetBlockSType_shouldWTXRequestWork(void){
	READER_T1_Block block;
	READER_T1_SBlockType sBlockType;
	READER_T1_BlockType bType;
	READER_Status retVal;
	uint8_t blockPCB;
	
	
	retVal = READER_T1_ForgeSBlock(&block, READER_T1_STYPE_WTX_REQU, READER_T1_LRC);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	bType = READER_T1_GetBlockType(&block);
	TEST_ASSERT_TRUE(bType == READER_T1_SBLOCK);
	
	sBlockType = READER_T1_GetBlockSType(&block);
	TEST_ASSERT_TRUE(sBlockType == READER_T1_STYPE_WTX_REQU);

	blockPCB = READER_T1_GetBlockPCB(&block);
	TEST_ASSERT_EQUAL_UINT8(0b00000011, blockPCB&0b00111111);       /* Voir ISO7816-3 section 11.3.2.2 */
}

void test_READER_T1_SetBlockSType_shouldWTXResponseWork(void){
	READER_T1_Block block;
	READER_T1_SBlockType sBlockType;
	READER_T1_BlockType bType;
	READER_Status retVal;
	uint8_t blockPCB;
	
	
	retVal = READER_T1_ForgeSBlock(&block, READER_T1_STYPE_WTX_RESP, READER_T1_LRC);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	bType = READER_T1_GetBlockType(&block);
	TEST_ASSERT_TRUE(bType == READER_T1_SBLOCK);
	
	sBlockType = READER_T1_GetBlockSType(&block);
	TEST_ASSERT_TRUE(sBlockType == READER_T1_STYPE_WTX_RESP);

	blockPCB = READER_T1_GetBlockPCB(&block);
	TEST_ASSERT_EQUAL_UINT8(0b00100011, blockPCB&0b00111111);       /* Voir ISO7816-3 section 11.3.2.2 */
}


void test_READER_T1_GetBlockPCB_shouldWork(void){
	READER_T1_Block block;
	READER_Status retVal;
	uint8_t blockPCB, expectedBlockPCB;
	
	
	expectedBlockPCB = 0xAB;
	
	retVal = READER_T1_ForgeBlock(&block, READER_T1_LRC);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	READER_T1_SetBlockPCB(&block, expectedBlockPCB);
	blockPCB = READER_T1_GetBlockPCB(&block);
	TEST_ASSERT_EQUAL_UINT8(expectedBlockPCB, blockPCB);
}
