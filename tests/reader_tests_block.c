#include "unity.h"
#include "mock_reader_hal_basis.h"
#include "mock_reader_hal_comm_settings.h"
#include "mock_reader_periph.h"

#include "reader_t1_block.h"
#include "reader_t1_iblock.h"
#include "reader_t1_sblock.h"
#include "reader_t1_rblock.h"

#include "reader_tests_block.h"
#include "reader_tests_test.h"

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
	RUN_TEST(test_READER_T1_SetBlockSType_shouldABORTRequestWork);
	RUN_TEST(test_READER_T1_SetBlockSType_shouldABORTResponseWork);
	RUN_TEST(test_READER_T1_GetBlockPCB_shouldWork);
	RUN_TEST(test_READER_T1_SetExpectedBlockSeqNumber_shouldWork);
	RUN_TEST(test_READER_T1_GetBlockACKType_shouldWork);
	RUN_TEST(test_READER_T1_RcvBlock_IBlockShouldWork);
	RUN_TEST(test_READER_T1_RcvBlock_shouldCheckMaxSize);
	RUN_TEST(test_READER_T1_RcvBlock_shouldTimeout_case1);
	RUN_TEST(test_READER_T1_RcvBlock_shouldTimeout_case2);
	RUN_TEST(test_READER_T1_RcvBlock_shouldTimeout_case3);
	RUN_TEST(test_READER_T1_CheckBlockIntegrity_shouldDetectBitFlip);
	RUN_TEST(test_READER_T1_SendBlock_shouldWork);
	RUN_TEST(test_READER_T1_CopyBlock_shouldWork);
	RUN_TEST(test_READER_T1_CopyBlock_shouldCheckMaxSize);
	RUN_TEST(test_READER_T1_SetBlockLEN_shouldCheckSize);
	RUN_TEST(test_READER_T1_SetBlockType_shouldCheckTypeCorrectness);
	RUN_TEST(test_READER_T1_SetBlockRedundancyType_shouldCheckTypeCorrectness);
	RUN_TEST(test_READER_T1_CopyBlockData_shouldWork);
	RUN_TEST(test_READER_T1_CopyBlockData_shouldCheckMaxSize);
	RUN_TEST(test_READER_T1_SetBlockSeqNumber_shouldCheckTypeCorrectness);
	RUN_TEST(test_READER_T1_SetBlockMBit_shouldCheckTypeCorrectness);
	RUN_TEST(test_READER_T1_CheckIBlock_shouldWork);
	RUN_TEST(test_READER_T1_SetBlockACKType_shouldCheckTypeCorrectness);
	RUN_TEST(test_READER_T1_SetExpectedBlockSeqNumber_shouldCheckTypeCorrectness);
	RUN_TEST(test_READER_T1_CheckRBlock_shouldWork);
	RUN_TEST(test_READER_T1_SetBlockSType_shouldCheckTypeCorrectness);
	RUN_TEST(test_READER_T1_SetBlockSPayload_shouldWork);
	RUN_TEST(test_READER_T1_CheckSBlock_shouldWork);
	RUN_TEST(test_READER_T1_IsSBlockRequest_shouldWork);
	RUN_TEST(test_READER_T1_IsSBlockResponse_shouldWork);
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


void test_READER_T1_SetBlockSType_shouldABORTRequestWork(void){
	READER_T1_Block block;
	READER_T1_SBlockType sBlockType;
	READER_T1_BlockType bType;
	READER_Status retVal;
	uint8_t blockPCB;
	
	
	retVal = READER_T1_ForgeSBlock(&block, READER_T1_STYPE_ABORT_REQU, READER_T1_LRC);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	bType = READER_T1_GetBlockType(&block);
	TEST_ASSERT_TRUE(bType == READER_T1_SBLOCK);
	
	sBlockType = READER_T1_GetBlockSType(&block);
	TEST_ASSERT_TRUE(sBlockType == READER_T1_STYPE_ABORT_REQU);

	blockPCB = READER_T1_GetBlockPCB(&block);
	TEST_ASSERT_EQUAL_UINT8(0b00000010, blockPCB&0b00111111);       /* Voir ISO7816-3 section 11.3.2.2 */
}


void test_READER_T1_SetBlockSType_shouldABORTResponseWork(void){
	READER_T1_Block block;
	READER_T1_SBlockType sBlockType;
	READER_T1_BlockType bType;
	READER_Status retVal;
	uint8_t blockPCB;
	
	
	retVal = READER_T1_ForgeSBlock(&block, READER_T1_STYPE_ABORT_RESP, READER_T1_LRC);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	bType = READER_T1_GetBlockType(&block);
	TEST_ASSERT_TRUE(bType == READER_T1_SBLOCK);
	
	sBlockType = READER_T1_GetBlockSType(&block);
	TEST_ASSERT_TRUE(sBlockType == READER_T1_STYPE_ABORT_RESP);

	blockPCB = READER_T1_GetBlockPCB(&block);
	TEST_ASSERT_EQUAL_UINT8(0b00100010, blockPCB&0b00111111);       /* Voir ISO7816-3 section 11.3.2.2 */
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


void test_READER_T1_SetExpectedBlockSeqNumber_shouldWork(void){
	READER_T1_Block block;
	READER_Status retVal;
	READER_T1_ExpSeqNumber expSeqNum;
	uint8_t blockPCB;
	
	
	retVal = READER_T1_ForgeRBlock(&block, READER_T1_ACKTYPE_ACK, READER_T1_EXPSEQNUM_ZERO, READER_T1_LRC);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	expSeqNum = READER_T1_GetExpectedBlockSeqNumber(&block);
	TEST_ASSERT_TRUE(expSeqNum == READER_T1_EXPSEQNUM_ZERO);

	blockPCB = READER_T1_GetBlockPCB(&block);
	TEST_ASSERT_EQUAL_UINT8(0b10000000, blockPCB);
	
	
	
	retVal = READER_T1_ForgeRBlock(&block, READER_T1_ACKTYPE_ACK, READER_T1_EXPSEQNUM_ONE, READER_T1_LRC);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	expSeqNum = READER_T1_GetExpectedBlockSeqNumber(&block);
	TEST_ASSERT_TRUE(expSeqNum == READER_T1_EXPSEQNUM_ONE);

	blockPCB = READER_T1_GetBlockPCB(&block);
	TEST_ASSERT_EQUAL_UINT8(0b10010000, blockPCB);
}


void test_READER_T1_GetBlockACKType_shouldWork(void){
	READER_T1_Block block;
	READER_Status retVal;
	READER_T1_ACKType ackType;
	uint8_t blockPCB;
	
	
	retVal = READER_T1_ForgeRBlock(&block, READER_T1_ACKTYPE_ACK, READER_T1_EXPSEQNUM_ZERO, READER_T1_LRC);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	ackType = READER_T1_GetBlockACKType(&block);
	TEST_ASSERT_TRUE(ackType == READER_T1_ACKTYPE_ACK);

	blockPCB = READER_T1_GetBlockPCB(&block);
	TEST_ASSERT_EQUAL_UINT8(0b10000000, blockPCB);    /* Voir ISO7816-3 section 11.3.2.2 */
	
	
	
	retVal = READER_T1_ForgeRBlock(&block, READER_T1_ACKTYPE_NACK, READER_T1_EXPSEQNUM_ZERO, READER_T1_LRC);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	ackType = READER_T1_GetBlockACKType(&block);
	TEST_ASSERT_TRUE(ackType == READER_T1_ACKTYPE_NACK);

	blockPCB = READER_T1_GetBlockPCB(&block);
	TEST_ASSERT_EQUAL_UINT8(0b10000010, blockPCB);     /* Voir ISO7816-3 section 11.3.2.2 */
	
	
	
	retVal = READER_T1_ForgeRBlock(&block, READER_T1_ACKTYPE_NACK_CRCLRC, READER_T1_EXPSEQNUM_ZERO, READER_T1_LRC);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	ackType = READER_T1_GetBlockACKType(&block);
	TEST_ASSERT_TRUE(ackType == READER_T1_ACKTYPE_NACK_CRCLRC);

	blockPCB = READER_T1_GetBlockPCB(&block);
	TEST_ASSERT_EQUAL_UINT8(0b10000001, blockPCB);     /* Voir ISO7816-3 section 11.3.2.2 */
}

void test_READER_T1_RcvBlock_IBlockShouldWork(void){
	READER_T1_Block expectedBlock, rcvdBlock;
	READER_HAL_CommSettings dummySettings;
	READER_Status retVal;
	uint8_t character;
	uint8_t emulatedRcvdFrame[] = {0b00000000, 0b00000000, 0b00000010, 0b11111111, 0b11111111, 0b00000010};
	uint8_t emulatedRcvdFrameSize = 6;
	uint8_t emulatedRcvdDataField[] = {0b11111111, 0b11111111};
	uint8_t emulatedRcvdDataFieldSize = 2;
	uint32_t i, tickstart;
	uint32_t timeout = 1000;
	
	
	/* On construit la Block que l'on s'attend a recevoir ...  */
	retVal = READER_T1_ForgeBlock(&expectedBlock, READER_T1_LRC);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	retVal = READER_T1_SetBlockNAD(&expectedBlock, emulatedRcvdFrame[READER_T1_BLOCKFRAME_NAD_POSITION]);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	retVal = READER_T1_SetBlockPCB(&expectedBlock, emulatedRcvdFrame[READER_T1_BLOCKFRAME_PCB_POSITION]);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	retVal = READER_T1_SetBlockLEN(&expectedBlock, emulatedRcvdFrame[READER_T1_BLOCKFRAME_LEN_POSITION]);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	retVal = READER_T1_SetBlockData(&expectedBlock, emulatedRcvdFrame+READER_T1_BLOCKFRAME_INF_POSITION, emulatedRcvdDataFieldSize);
	TEST_ASSERT_TRUE(retVal == READER_OK);

	/* On prepare l'emulation de la reception du Block ...  */
	for(i=0; i<emulatedRcvdFrameSize; i++){
		READER_HAL_RcvChar_ExpectAndReturn(&dummySettings, READER_HAL_PROTOCOL_T1, &character, timeout, READER_OK);
		READER_HAL_RcvChar_IgnoreArg_pSettings();
		READER_HAL_RcvChar_IgnoreArg_character();
		READER_HAL_RcvChar_IgnoreArg_timeout();
		READER_HAL_RcvChar_ReturnThruPtr_character(emulatedRcvdFrame+i);
	}
	
	/* On effectue une reception de Block ...  */
	retVal = READER_T1_RcvBlock(&rcvdBlock, READER_T1_LRC, timeout, 0, &tickstart, &dummySettings);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	
	/* On compare le Block recu avec le Block attendu ...  */
	TEST_ASSERT_EQUAL_UINT8_ARRAY(expectedBlock.blockFrame, rcvdBlock.blockFrame, emulatedRcvdFrameSize);
}


void test_READER_T1_RcvBlock_shouldCheckMaxSize(void){
	READER_T1_Block block;
	READER_HAL_CommSettings dummySettings;
	READER_Status retVal;
	uint32_t timeout = 1000;
	uint32_t tickstart;
	uint8_t nad = 0x00;
	uint8_t pcb = 0x00;
	uint32_t len32 = READER_T1_BLOCK_MAX_DATA_SIZE + 1;
	uint8_t len = (uint8_t)(len32);
	uint8_t data = 0x00;
	uint8_t lrc = len;
	uint32_t i;
	
	
	if(len32 > 0xFF){
		TEST_IGNORE();
	}
	
	/* On simule la reception d'un Block avec LEN > READER_T1_BLOCK_MAX_DATA_SIZE ...  */
	READER_HAL_RcvChar_ExpectAnyArgsAndReturn(READER_OK);
	READER_HAL_RcvChar_ReturnThruPtr_character(&nad);
	
	READER_HAL_RcvChar_ExpectAnyArgsAndReturn(READER_OK);
	READER_HAL_RcvChar_ReturnThruPtr_character(&pcb);
	
	READER_HAL_RcvChar_ExpectAnyArgsAndReturn(READER_OK);
	READER_HAL_RcvChar_ReturnThruPtr_character(&len);
	
	//READER_HAL_RcvChar_IgnoreAndReturn(READER_OK);
	
	
	/* On effectue une reception et on regarde le comportement de la fonction ...  */
	retVal = READER_T1_RcvBlock(&block, READER_T1_LRC, timeout, 1, &tickstart, &dummySettings);
	TEST_ASSERT_FALSE(retVal == READER_OK);
}



void test_READER_T1_RcvBlock_shouldTimeout_case1(void){
	READER_T1_Block block;
	READER_HAL_CommSettings dummySettings;
	READER_Status retVal;
	uint32_t timeout = 1000;
	uint32_t tickstart;
	
	
	/* On simule in timeout des la reception du 1er caractere ...  */
	READER_HAL_RcvChar_ExpectAnyArgsAndReturn(READER_TIMEOUT);
	
	/* On verifie le comportement de la fonction ...  */
	retVal = READER_T1_RcvBlock(&block, READER_T1_LRC, timeout, 0, &tickstart, &dummySettings);
	TEST_ASSERT_TRUE(retVal == READER_TIMEOUT);
}


void test_READER_T1_RcvBlock_shouldTimeout_case2(void){
	READER_T1_Block block;
	READER_HAL_CommSettings dummySettings;
	READER_Status retVal;
	uint32_t timeout = 1000;
	uint32_t tickstart;
	uint8_t character = 0x00;
	
	
	
	/* On simule un timeout a la reception du deuxiemme caratere du header ...  */
	READER_HAL_RcvChar_ExpectAnyArgsAndReturn(READER_OK);
	READER_HAL_RcvChar_ReturnThruPtr_character(&character);
	
	READER_HAL_RcvChar_ExpectAnyArgsAndReturn(READER_TIMEOUT);
	
	/* On verifie le comportement de la fonction ...  */
	retVal = READER_T1_RcvBlock(&block, READER_T1_LRC, timeout, 0, &tickstart, &dummySettings);
	TEST_ASSERT_TRUE(retVal == READER_TIMEOUT);
}


void test_READER_T1_RcvBlock_shouldTimeout_case3(void){
	READER_T1_Block block;
	READER_HAL_CommSettings dummySettings;
	READER_Status retVal;
	uint32_t timeout = 1000;
	uint32_t tickstart;
	uint8_t nad = 0x00;
	uint8_t pcb = 0x00;
	uint8_t len = 0x03;
	uint8_t data1 = 0xFF;
	uint8_t data2 = 0xFF;
	
	
	/* On simule le cas ou on recoit moins de donnees que indique dans le champs LEN du Block ...  */
	READER_HAL_RcvChar_ExpectAnyArgsAndReturn(READER_OK);
	READER_HAL_RcvChar_ReturnThruPtr_character(&nad);
	
	READER_HAL_RcvChar_ExpectAnyArgsAndReturn(READER_OK);
	READER_HAL_RcvChar_ReturnThruPtr_character(&pcb);
	
	READER_HAL_RcvChar_ExpectAnyArgsAndReturn(READER_OK);
	READER_HAL_RcvChar_ReturnThruPtr_character(&len);
	
	READER_HAL_RcvChar_ExpectAnyArgsAndReturn(READER_OK);
	READER_HAL_RcvChar_ReturnThruPtr_character(&data1);
	
	READER_HAL_RcvChar_ExpectAnyArgsAndReturn(READER_OK);
	READER_HAL_RcvChar_ReturnThruPtr_character(&data2);
	
	READER_HAL_RcvChar_ExpectAnyArgsAndReturn(READER_TIMEOUT);
	
	
	/* On verifie le comportement de la fonction ...  */
	retVal = READER_T1_RcvBlock(&block, READER_T1_LRC, timeout, 0, &tickstart, &dummySettings);
	TEST_ASSERT_TRUE(retVal == READER_TIMEOUT);	
}


void test_READER_T1_CheckBlockIntegrity_shouldDetectBitFlip(void){
	READER_T1_Block block;
	READER_Status retVal;
	uint8_t blockData[] = {0x00, 0x01, 0x02};
	uint8_t blockDataSize = 3;
	
	
	retVal = READER_T1_ForgeIBlock(&block, blockData, blockDataSize, READER_T1_SEQNUM_ZERO, READER_T1_MBIT_ZERO, READER_T1_LRC);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	retVal = READER_T1_CheckBlockIntegrity(&block, READER_T1_LRC);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	block.blockFrame[READER_T1_BLOCKFRAME_INF_POSITION + 1] = block.blockFrame[READER_T1_BLOCKFRAME_INF_POSITION + 1] ^ 0b00000001;
	
	retVal = READER_T1_CheckBlockIntegrity(&block, READER_T1_LRC);
	TEST_ASSERT_FALSE(retVal == READER_OK);
}



void test_READER_T1_SendBlock_shouldWork(void){
	READER_T1_Block block;
	READER_HAL_CommSettings dummySettings;
	READER_Status retVal;
	uint8_t data[READER_T1_BLOCK_MAX_DATA_SIZE];
	uint32_t dataSize = READER_T1_BLOCK_MAX_DATA_SIZE;
	uint8_t prologue[READER_T1_BLOCK_PROLOGUE_SIZE];
	uint8_t lrc;
	uint32_t timeout = 1000;
	uint32_t tickstart;
	uint32_t i;
	
	
	/* Preparation de donnees aleatoires ...  */
	srand(time(NULL));
	for(i=0; i<dataSize; i++){
		data[i] = (uint8_t)(rand());
	}
	
	/* Fabrication d'un I-Block avec ces donnees ...  */
	retVal = READER_T1_ForgeIBlock(&block, data, dataSize, READER_T1_SEQNUM_ONE, READER_T1_MBIT_ONE, READER_T1_LRC);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	/* On paramettre les donnees que l'on s'attend a etre envoyees ...  */
	prologue[READER_T1_BLOCKFRAME_NAD_POSITION] = block.blockFrame[READER_T1_BLOCKFRAME_NAD_POSITION];
	prologue[READER_T1_BLOCKFRAME_PCB_POSITION] = block.blockFrame[READER_T1_BLOCKFRAME_PCB_POSITION];
	prologue[READER_T1_BLOCKFRAME_LEN_POSITION] = block.blockFrame[READER_T1_BLOCKFRAME_LEN_POSITION];
	
	lrc = READER_T1_ComputeBlockLRC(&block);
	
	set_expected_CharFrame(prologue, READER_T1_BLOCK_PROLOGUE_SIZE);
	set_expected_CharFrame(data, dataSize);
	set_expected_CharFrame(&lrc, 1);
	
	/* On effecture l'envoi ...  */
	retVal = READER_T1_SendBlock(&block, timeout, 0, &tickstart, &dummySettings);
	TEST_ASSERT_TRUE(retVal == READER_OK);
}



void test_READER_T1_CopyBlock_shouldWork(void){
	READER_T1_Block blockSrc, blockDst;
	READER_Status retVal;
	uint8_t data[READER_T1_BLOCK_MAX_DATA_SIZE];
	uint32_t dataSize = READER_T1_BLOCK_MAX_DATA_SIZE;
	uint32_t i;
	
	
	/* Preparation de donnees aleatoires ...  */
	srand(time(NULL));
	for(i=0; i<dataSize; i++){
		data[i] = (uint8_t)(rand());
	}
	
	/* Fabrication d'un I-Block avec ces donnees ...  */
	retVal = READER_T1_ForgeIBlock(&blockSrc, data, dataSize, READER_T1_SEQNUM_ONE, READER_T1_MBIT_ONE, READER_T1_LRC);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	/* On copie le Block ...  */
	retVal = READER_T1_CopyBlock(&blockDst, &blockSrc);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	/* On verifie que les deux Blocks sont identiques ...  */
	TEST_ASSERT_EQUAL_UINT8_ARRAY(blockSrc.blockFrame, blockDst.blockFrame, READER_T1_BLOCK_PROLOGUE_SIZE + READER_T1_BLOCK_MAX_DATA_SIZE + 1);
}


void test_READER_T1_CopyBlock_shouldCheckMaxSize(void){
	READER_T1_Block blockSrc, blockDst;
	READER_Status retVal;
	uint8_t data[READER_T1_BLOCK_MAX_DATA_SIZE];
	uint32_t dataSize = READER_T1_BLOCK_MAX_DATA_SIZE;
	uint32_t len32 = READER_T1_BLOCK_MAX_DATA_SIZE + 1;
	
	
	if(len32 > 0xFF){
		TEST_IGNORE();
	}
	
	blockSrc.blockFrame[READER_T1_BLOCKFRAME_LEN_POSITION] = (uint8_t)(len32);
	
	retVal = READER_T1_CopyBlock(&blockDst, &blockSrc);
	TEST_ASSERT_TRUE(retVal == READER_ERR);
}



void test_READER_T1_SetBlockLEN_shouldCheckSize(void){
	READER_T1_Block block;
	READER_Status retVal;
	uint32_t len32;
	uint8_t len;
	
	
	len32 = (uint32_t)(READER_T1_BLOCK_MAX_DATA_SIZE) + 1;
	if(len32 > 0x000000FF){
		TEST_IGNORE();
	}
	
	len = (uint8_t)(len32);
	
	retVal = READER_T1_SetBlockLEN(&block, len);
	TEST_ASSERT_FALSE(retVal == READER_OK);
}


void test_READER_T1_SetBlockType_shouldCheckTypeCorrectness(void){
	READER_T1_Block block;
	READER_Status retVal;
	
	retVal = READER_T1_ForgeBlock(&block, READER_T1_LRC);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	retVal = READER_T1_SetBlockType(&block, READER_T1_SBLOCK);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	retVal = READER_T1_SetBlockType(&block, (uint32_t)(0xFFFFFFFF));
	TEST_ASSERT_FALSE(retVal == READER_OK);
	
	retVal = READER_T1_SetBlockType(&block, (uint32_t)(0xFFFFFF00));
	TEST_ASSERT_FALSE(retVal == READER_OK);
}


void test_READER_T1_SetBlockRedundancyType_shouldCheckTypeCorrectness(void){
	READER_T1_Block block;
	READER_Status retVal;
	
	
	retVal = READER_T1_ForgeBlock(&block, READER_T1_LRC);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	retVal = READER_T1_SetBlockRedundancyType(&block, READER_T1_LRC);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	retVal = READER_T1_SetBlockRedundancyType(&block, READER_T1_CRC);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	retVal = READER_T1_SetBlockRedundancyType(&block, (uint32_t)(0xFF00FF00));
	TEST_ASSERT_FALSE(retVal == READER_OK);
}



void test_READER_T1_CopyBlockData_shouldWork(void){
	READER_T1_Block block;
	READER_Status retVal;
	uint8_t data[READER_T1_BLOCK_MAX_DATA_SIZE];
	uint32_t dataSize = READER_T1_BLOCK_MAX_DATA_SIZE;
	uint8_t destDataBuff[READER_T1_BLOCK_MAX_DATA_SIZE];
	uint8_t destDataBuffSize = READER_T1_BLOCK_MAX_DATA_SIZE;
	uint32_t i;
	
	
	/* Preparation de donnees aleatoires ...  */
	srand(time(NULL));
	for(i=0; i<dataSize; i++){
		data[i] = (uint8_t)(rand());
	}
	
	/* Fabrication d'un I-Block avec ces donnees ...  */
	retVal = READER_T1_ForgeIBlock(&block, data, dataSize, READER_T1_SEQNUM_ONE, READER_T1_MBIT_ONE, READER_T1_LRC);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	
	/* On copie les Data du Block ...  */
	retVal = READER_T1_CopyBlockData(&block, destDataBuff, destDataBuffSize);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	/* On verifie que les Data des deux Blocks sont identiques ...  */
	TEST_ASSERT_EQUAL_UINT8_ARRAY(block.blockFrame + READER_T1_BLOCKFRAME_INF_POSITION, destDataBuff, dataSize);
}


void test_READER_T1_CopyBlockData_shouldCheckMaxSize(void){
	READER_T1_Block block;
	READER_Status retVal;
	uint8_t data[READER_T1_BLOCK_MAX_DATA_SIZE];
	uint32_t dataSize = READER_T1_BLOCK_MAX_DATA_SIZE;
	uint8_t destDataBuff[READER_T1_BLOCK_MAX_DATA_SIZE-1];
	uint8_t destDataBuffSize = READER_T1_BLOCK_MAX_DATA_SIZE-1;
	uint32_t i;
	
	
	/* Preparation de donnees aleatoires ...  */
	srand(time(NULL));
	for(i=0; i<dataSize; i++){
		data[i] = (uint8_t)(rand());
	}
	
	/* Fabrication d'un I-Block avec ces donnees ...  */
	retVal = READER_T1_ForgeIBlock(&block, data, dataSize, READER_T1_SEQNUM_ONE, READER_T1_MBIT_ONE, READER_T1_LRC);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	
	/* On copie les Data du Block ...  */
	retVal = READER_T1_CopyBlockData(&block, destDataBuff, destDataBuffSize);
	TEST_ASSERT_FALSE(retVal == READER_OK);
}


void test_READER_T1_SetBlockSeqNumber_shouldCheckTypeCorrectness(void){
	READER_T1_Block block;
	READER_Status retVal;
	uint8_t data[READER_T1_BLOCK_MAX_DATA_SIZE];
	uint32_t dataSize = READER_T1_BLOCK_MAX_DATA_SIZE;
	
	
	retVal = READER_T1_ForgeIBlock(&block, data, dataSize, READER_T1_SEQNUM_ZERO, READER_T1_MBIT_ZERO, READER_T1_LRC);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	retVal = READER_T1_ForgeIBlock(&block, data, dataSize, READER_T1_SEQNUM_ONE, READER_T1_MBIT_ZERO, READER_T1_LRC);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	retVal = READER_T1_ForgeIBlock(&block, data, dataSize, (uint32_t)(0xFF00FF00), READER_T1_MBIT_ZERO, READER_T1_LRC);
	TEST_ASSERT_FALSE(retVal == READER_OK);
	
	retVal = READER_T1_ForgeIBlock(&block, data, dataSize, (uint32_t)(0x0000FFFF), READER_T1_MBIT_ZERO, READER_T1_LRC);
	TEST_ASSERT_FALSE(retVal == READER_OK);
}

void test_READER_T1_SetBlockMBit_shouldCheckTypeCorrectness(void){
	READER_T1_Block block;
	READER_Status retVal;
	uint8_t data[READER_T1_BLOCK_MAX_DATA_SIZE];
	uint32_t dataSize = READER_T1_BLOCK_MAX_DATA_SIZE;
	
	
	retVal = READER_T1_ForgeIBlock(&block, data, dataSize, READER_T1_SEQNUM_ZERO, READER_T1_MBIT_ZERO, READER_T1_LRC);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	retVal = READER_T1_ForgeIBlock(&block, data, dataSize, READER_T1_SEQNUM_ZERO, READER_T1_MBIT_ONE, READER_T1_LRC);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	retVal = READER_T1_ForgeIBlock(&block, data, dataSize, READER_T1_SEQNUM_ZERO, (uint32_t)(0xFF00FF00), READER_T1_LRC);
	TEST_ASSERT_FALSE(retVal == READER_OK);
	
	retVal = READER_T1_ForgeIBlock(&block, data, dataSize, READER_T1_SEQNUM_ZERO, (uint32_t)(0x0000FFFF), READER_T1_LRC);
	TEST_ASSERT_FALSE(retVal == READER_OK);
}


void test_READER_T1_CheckIBlock_shouldWork(void){
	READER_T1_Block block;
	READER_Status retVal;
	uint8_t data[READER_T1_BLOCK_MAX_DATA_SIZE];
	uint32_t dataSize = READER_T1_BLOCK_MAX_DATA_SIZE;
	
	
	retVal = READER_T1_ForgeIBlock(&block, data, dataSize, READER_T1_SEQNUM_ZERO, READER_T1_MBIT_ZERO, READER_T1_LRC);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	retVal = READER_T1_CheckIBlock(&block);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	retVal = READER_T1_ForgeSBlock(&block, READER_T1_STYPE_RESYNCH_REQU, READER_T1_LRC);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	retVal = READER_T1_CheckIBlock(&block);
	TEST_ASSERT_FALSE(retVal == READER_OK);
	
	retVal = READER_T1_ForgeRBlock(&block, READER_T1_ACKTYPE_NACK, READER_T1_EXPSEQNUM_ONE, READER_T1_LRC);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	retVal = READER_T1_CheckIBlock(&block);
	TEST_ASSERT_FALSE(retVal == READER_OK);
}


void test_READER_T1_SetBlockACKType_shouldCheckTypeCorrectness(void){
	READER_T1_Block block;
	READER_Status retVal;
	
	
	retVal = READER_T1_ForgeRBlock(&block, READER_T1_ACKTYPE_ACK, READER_T1_EXPSEQNUM_ONE, READER_T1_LRC);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	retVal = READER_T1_ForgeRBlock(&block, READER_T1_ACKTYPE_NACK, READER_T1_EXPSEQNUM_ONE, READER_T1_LRC);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	retVal = READER_T1_ForgeRBlock(&block, READER_T1_ACKTYPE_NACK_CRCLRC, READER_T1_EXPSEQNUM_ONE, READER_T1_LRC);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	retVal = READER_T1_ForgeRBlock(&block, (uint32_t)(0x0000FFFF), READER_T1_EXPSEQNUM_ONE, READER_T1_LRC);
	TEST_ASSERT_FALSE(retVal == READER_OK);
	
	retVal = READER_T1_ForgeRBlock(&block, (uint32_t)(0xF0F0F0F0), READER_T1_EXPSEQNUM_ONE, READER_T1_LRC);
	TEST_ASSERT_FALSE(retVal == READER_OK);
}



void test_READER_T1_SetExpectedBlockSeqNumber_shouldCheckTypeCorrectness(void){
	READER_T1_Block block;
	READER_Status retVal;
	
	
	retVal = READER_T1_ForgeRBlock(&block, READER_T1_ACKTYPE_ACK, READER_T1_EXPSEQNUM_ONE, READER_T1_LRC);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	retVal = READER_T1_ForgeRBlock(&block, READER_T1_ACKTYPE_NACK, READER_T1_EXPSEQNUM_ZERO, READER_T1_LRC);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	retVal = READER_T1_ForgeRBlock(&block, READER_T1_ACKTYPE_NACK_CRCLRC, (uint32_t)(0xF0F0F0F0), READER_T1_LRC);
	TEST_ASSERT_FALSE(retVal == READER_OK);
	
	retVal = READER_T1_ForgeRBlock(&block, READER_T1_ACKTYPE_ACK, (uint32_t)(0xFFFF0000), READER_T1_LRC);
	TEST_ASSERT_FALSE(retVal == READER_OK);
	
	retVal = READER_T1_ForgeRBlock(&block, READER_T1_ACKTYPE_ACK, (uint32_t)(0xFF00FF00), READER_T1_LRC);
	TEST_ASSERT_FALSE(retVal == READER_OK);
}


void test_READER_T1_CheckRBlock_shouldWork(void){
	READER_T1_Block block;
	READER_Status retVal;
	uint8_t data[READER_T1_BLOCK_MAX_DATA_SIZE];
	uint32_t dataSize = READER_T1_BLOCK_MAX_DATA_SIZE;
	
	
	retVal = READER_T1_ForgeRBlock(&block, READER_T1_ACKTYPE_ACK, READER_T1_EXPSEQNUM_ONE, READER_T1_LRC);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	retVal = READER_T1_CheckRBlock(&block);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	retVal = READER_T1_ForgeSBlock(&block, READER_T1_STYPE_RESYNCH_REQU, READER_T1_LRC);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	retVal = READER_T1_CheckRBlock(&block);
	TEST_ASSERT_FALSE(retVal == READER_OK);
	
	retVal = READER_T1_ForgeIBlock(&block, data, dataSize, READER_T1_SEQNUM_ZERO, READER_T1_MBIT_ZERO, READER_T1_LRC);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	retVal = READER_T1_CheckRBlock(&block);
	TEST_ASSERT_FALSE(retVal == READER_OK);
}


void test_READER_T1_SetBlockSType_shouldCheckTypeCorrectness(void){
	READER_T1_Block block;
	READER_Status retVal;
	
	
	retVal = READER_T1_ForgeSBlock(&block, READER_T1_STYPE_RESYNCH_REQU, READER_T1_LRC);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	retVal = READER_T1_ForgeSBlock(&block, (uint32_t)(0xFF00FF00), READER_T1_LRC);
	TEST_ASSERT_FALSE(retVal == READER_OK);
	
	retVal = READER_T1_ForgeSBlock(&block, (uint32_t)(0xFEFEFEFE), READER_T1_LRC);
	TEST_ASSERT_FALSE(retVal == READER_OK);
}


void test_READER_T1_SetBlockSPayload_shouldWork(void){
	READER_T1_Block block;
	READER_Status retVal;
	uint8_t payload = 0xAB;
	uint8_t len, inf;
	
	
	retVal = READER_T1_ForgeSBlock(&block, READER_T1_STYPE_WTX_REQU, READER_T1_LRC);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	retVal = READER_T1_SetBlockSPayload(&block, payload);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	len = READER_T1_GetBlockLEN(&block);
	TEST_ASSERT_EQUAL_UINT8(1, len);
	
	inf = block.blockFrame[READER_T1_BLOCKFRAME_INF_POSITION];
	TEST_ASSERT_EQUAL_UINT8(payload, inf);
	
	inf = READER_T1_GetBlockSPayload(&block);
	TEST_ASSERT_EQUAL_UINT8(inf, payload);
}


void test_READER_T1_CheckSBlock_shouldWork(void){
	READER_T1_Block block;
	READER_Status retVal;
	uint8_t data[READER_T1_BLOCK_MAX_DATA_SIZE];
	uint32_t dataSize = READER_T1_BLOCK_MAX_DATA_SIZE;
	
	
	
	retVal = READER_T1_ForgeSBlock(&block, READER_T1_STYPE_RESYNCH_REQU, READER_T1_LRC);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	retVal = READER_T1_CheckSBlock(&block);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	retVal = READER_T1_ForgeRBlock(&block, READER_T1_ACKTYPE_ACK, READER_T1_EXPSEQNUM_ONE, READER_T1_LRC);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	retVal = READER_T1_CheckSBlock(&block);
	TEST_ASSERT_FALSE(retVal == READER_OK);
	
	retVal = READER_T1_ForgeIBlock(&block, data, dataSize, READER_T1_SEQNUM_ZERO, READER_T1_MBIT_ZERO, READER_T1_LRC);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	retVal = READER_T1_CheckSBlock(&block);
	TEST_ASSERT_FALSE(retVal == READER_OK);
}



void test_READER_T1_IsSBlockRequest_shouldWork(void){
	READER_T1_Block block;
	READER_Status retVal;
	uint8_t data[READER_T1_BLOCK_MAX_DATA_SIZE];
	uint32_t dataSize = READER_T1_BLOCK_MAX_DATA_SIZE;
	
	
	
	retVal = READER_T1_ForgeSBlock(&block, READER_T1_STYPE_RESYNCH_REQU, READER_T1_LRC);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	retVal = READER_T1_IsSBlockRequest(&block);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	
	retVal = READER_T1_ForgeSBlock(&block, READER_T1_STYPE_ABORT_REQU, READER_T1_LRC);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	retVal = READER_T1_IsSBlockRequest(&block);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	
	retVal = READER_T1_ForgeSBlock(&block, READER_T1_STYPE_IFS_REQU, READER_T1_LRC);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	retVal = READER_T1_IsSBlockRequest(&block);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	
	retVal = READER_T1_ForgeSBlock(&block, READER_T1_STYPE_WTX_REQU, READER_T1_LRC);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	retVal = READER_T1_IsSBlockRequest(&block);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	
	retVal = READER_T1_ForgeSBlock(&block, READER_T1_STYPE_RESYNCH_RESP, READER_T1_LRC);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	retVal = READER_T1_IsSBlockRequest(&block);
	TEST_ASSERT_TRUE(retVal == READER_NO);
	
	
	retVal = READER_T1_ForgeSBlock(&block, READER_T1_STYPE_ABORT_RESP, READER_T1_LRC);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	retVal = READER_T1_IsSBlockRequest(&block);
	TEST_ASSERT_TRUE(retVal == READER_NO);
	
	
	retVal = READER_T1_ForgeSBlock(&block, READER_T1_STYPE_IFS_RESP, READER_T1_LRC);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	retVal = READER_T1_IsSBlockRequest(&block);
	TEST_ASSERT_TRUE(retVal == READER_NO);
	
	
	retVal = READER_T1_ForgeSBlock(&block, READER_T1_STYPE_WTX_RESP, READER_T1_LRC);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	retVal = READER_T1_IsSBlockRequest(&block);
	TEST_ASSERT_TRUE(retVal == READER_NO);
	
	
	retVal = READER_T1_ForgeIBlock(&block, data, dataSize, READER_T1_SEQNUM_ZERO, READER_T1_MBIT_ZERO, READER_T1_LRC);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	retVal = READER_T1_IsSBlockRequest(&block);
	TEST_ASSERT_FALSE(retVal == READER_OK);
	
	
	retVal = READER_T1_ForgeRBlock(&block, READER_T1_ACKTYPE_ACK, READER_T1_EXPSEQNUM_ONE, READER_T1_LRC);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	retVal = READER_T1_IsSBlockRequest(&block);
	TEST_ASSERT_FALSE(retVal == READER_OK);
}



void test_READER_T1_IsSBlockResponse_shouldWork(void){
	READER_T1_Block block;
	READER_Status retVal;
	uint8_t data[READER_T1_BLOCK_MAX_DATA_SIZE];
	uint32_t dataSize = READER_T1_BLOCK_MAX_DATA_SIZE;
	
	
	
	retVal = READER_T1_ForgeSBlock(&block, READER_T1_STYPE_RESYNCH_REQU, READER_T1_LRC);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	retVal = READER_T1_IsSBlockResponse(&block);
	TEST_ASSERT_TRUE(retVal == READER_NO);
	
	
	retVal = READER_T1_ForgeSBlock(&block, READER_T1_STYPE_ABORT_REQU, READER_T1_LRC);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	retVal = READER_T1_IsSBlockResponse(&block);
	TEST_ASSERT_TRUE(retVal == READER_NO);
	
	
	retVal = READER_T1_ForgeSBlock(&block, READER_T1_STYPE_IFS_REQU, READER_T1_LRC);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	retVal = READER_T1_IsSBlockResponse(&block);
	TEST_ASSERT_TRUE(retVal == READER_NO);
	
	
	retVal = READER_T1_ForgeSBlock(&block, READER_T1_STYPE_WTX_REQU, READER_T1_LRC);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	retVal = READER_T1_IsSBlockResponse(&block);
	TEST_ASSERT_TRUE(retVal == READER_NO);
	
	
	retVal = READER_T1_ForgeSBlock(&block, READER_T1_STYPE_RESYNCH_RESP, READER_T1_LRC);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	retVal = READER_T1_IsSBlockResponse(&block);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	
	retVal = READER_T1_ForgeSBlock(&block, READER_T1_STYPE_ABORT_RESP, READER_T1_LRC);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	retVal = READER_T1_IsSBlockResponse(&block);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	
	retVal = READER_T1_ForgeSBlock(&block, READER_T1_STYPE_IFS_RESP, READER_T1_LRC);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	retVal = READER_T1_IsSBlockResponse(&block);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	
	retVal = READER_T1_ForgeSBlock(&block, READER_T1_STYPE_WTX_RESP, READER_T1_LRC);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	retVal = READER_T1_IsSBlockResponse(&block);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	
	retVal = READER_T1_ForgeIBlock(&block, data, dataSize, READER_T1_SEQNUM_ZERO, READER_T1_MBIT_ZERO, READER_T1_LRC);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	retVal = READER_T1_IsSBlockResponse(&block);
	TEST_ASSERT_FALSE(retVal == READER_OK);
	
	
	retVal = READER_T1_ForgeRBlock(&block, READER_T1_ACKTYPE_ACK, READER_T1_EXPSEQNUM_ONE, READER_T1_LRC);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	retVal = READER_T1_IsSBlockResponse(&block);
	TEST_ASSERT_FALSE(retVal == READER_OK);
}

