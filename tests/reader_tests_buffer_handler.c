#include "unity.h"
#include "mock_reader_hal_basis.h"
#include "mock_reader_hal_comm_settings.h"
#include "mock_reader_periph.h"

#include "reader_t1_buffer_handler.h"
#include "reader_t1_context_handler.h"

#include "reader_tests_buffer_handler.h"
#include "reader_tests_test.h"

#include <time.h>
#include <stdio.h>
#include <stdlib.h>







void test_READER_BUFFER_all(void){
	RUN_TEST(test_READER_T1_BUFFER_Init_shouldSetInitValues);
	RUN_TEST(test_READER_T1_BUFFER_Clear_shouldReinitValues);
	RUN_TEST(test_READER_T1_BUFFER_SetLength_shouldWork);	
	RUN_TEST(test_T1_BUFFER_SetLength_shoulCheckMaxSize);	
	RUN_TEST(test_READER_T1_BUFFER_IncLength_shouldWork);	
	RUN_TEST(test_READER_T1_BUFFER_IncLength_shouldCheckMaxSize);	
	RUN_TEST(test_READER_T1_BUFFER_DecLength_shouldWork);	
	RUN_TEST(test_READER_T1_BUFFER_DecLength_shouldCheckMinSize);
	RUN_TEST(test_READER_T1_BUFFER_PlacesLeft_shouldWork);
	RUN_TEST(test_READER_T1_BUFFER_PlacesLeft_shouldWorkWhenFull);
	RUN_TEST(test_READER_T1_BUFFER_IsFull_shouldWork);
	RUN_TEST(test_READER_T1_BUFFER_IsEmpty_shouldWork);
	RUN_TEST(test_READER_T1_BUFFER_Stack_stackAndDequeueShouldWorkWithSBlock_case1);
	RUN_TEST(test_READER_T1_BUFFER_Stack_stackAndDequeueShouldWorkWithSBlock_case2);
	RUN_TEST(test_READER_T1_BUFFER_Enqueue_enqueueAndDequeueShouldWorkWithSBlock_case1);
	RUN_TEST(test_READER_T1_BUFFER_Enqueue_enqueueAndDequeueShouldWorkWithSBlock_case2);
	RUN_TEST(test_READER_T1_BUFFER_Dequeue_shouldCheckIfEmpty);
	RUN_TEST(test_READER_T1_BUFFER_Stack_shouldCheckIfFull);
	RUN_TEST(test_READER_T1_BUFFER_Enqueue_shouldCheckIfFull);
	RUN_TEST(test_READER_T1_BUFFER_Enqueue_shouldUpdateIBlockCounter);
	RUN_TEST(test_READER_T1_BUFFER_Stack_shouldUpdateIBlockCounter);
	RUN_TEST(test_READER_T1_BUFFER_GetRBlockAndSBlockCount_shouldWork);
	RUN_TEST(test_READER_T1_BUFFER_ExtractRawDataFromBuffer_shouldWork_case1);
	RUN_TEST(test_READER_T1_BUFFER_ExtractRawDataFromBuffer_shouldWork_case2);
	RUN_TEST(test_READER_T1_BUFFER_ExtractRawDataFromBuffer_shouldCheckMaxSize);
	RUN_TEST(test_READER_T1_BUFFER_ExtractRawDataFromBuffer_shouldWorkWhenContolBlocksInsideBuffer);
	RUN_TEST(test_READER_T1_BUFFER_StripIBlocks_shouldWork);
	RUN_TEST(test_READER_T1_BUFFER_StripControlBlocks_shouldWork);
	RUN_TEST(test_READER_T1_BUFFER_GetBottomBlockPtr_shouldPointCorrectBlock);
	RUN_TEST(test_READER_T1_BUFFER_GetBottomBlockPtr_shouldDetectEmptyBuffer);
	RUN_TEST(test_READER_T1_BUFFER_GetBottomBlockType_shouldDetectEmptyBuffer);
	RUN_TEST(test_READER_T1_BUFFER_GetBottomBlockType_shouldWork);
}



void test_READER_T1_BUFFER_Init_shouldSetInitValues(void){
	READER_T1_ContextHandler context;
	READER_Status retVal;
	uint32_t indexTop, indexBottom, length, IBlockCount;
	
	
	retVal = READER_T1_BUFFER_Init(&context);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	indexTop = context.blockBuff.indexTop;
	indexBottom = context.blockBuff.indexBottom;
	length = context.blockBuff.length;
	IBlockCount = context.blockBuff.IBlockCount;
	
	
	TEST_ASSERT_EQUAL_UINT32(0x00000000, indexTop);
	TEST_ASSERT_EQUAL_UINT32(0x00000000, indexBottom);
	TEST_ASSERT_EQUAL_UINT32(0x00000000, length);
	TEST_ASSERT_EQUAL_UINT32(0x00000000, IBlockCount);
}


void test_READER_T1_BUFFER_Clear_shouldReinitValues(void){
	READER_T1_ContextHandler context;
	READER_Status retVal;
	uint32_t indexTop, indexBottom, length, IBlockCount;
	
	
	srand(time(NULL));
	
	retVal = READER_T1_BUFFER_Init(&context);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	context.blockBuff.indexTop = (uint32_t)(rand());
	context.blockBuff.indexBottom = (uint32_t)(rand());
	context.blockBuff.length = (uint32_t)(rand());
	context.blockBuff.IBlockCount = (uint32_t)(rand());
	
	retVal = READER_T1_BUFFER_Clear(&context);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	indexTop = context.blockBuff.indexTop;
	indexBottom = context.blockBuff.indexBottom;
	length = context.blockBuff.length;
	IBlockCount = context.blockBuff.IBlockCount;
	
	
	TEST_ASSERT_EQUAL_UINT32(0x00000000, indexTop);
	TEST_ASSERT_EQUAL_UINT32(0x00000000, indexBottom);
	TEST_ASSERT_EQUAL_UINT32(0x00000000, length);
	TEST_ASSERT_EQUAL_UINT32(0x00000000, IBlockCount);
	
}



void test_READER_T1_BUFFER_SetLength_shouldWork(void){
	READER_T1_ContextHandler context;
	READER_Status retVal;
	
	
	retVal = READER_T1_BUFFER_Init(&context);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	if(READER_T1_CONTEXT_STATICBUFF_MAXSIZE < 1){
		TEST_IGNORE();
	}
	retVal = READER_T1_BUFFER_SetLength(&context, 1);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	TEST_ASSERT_EQUAL_UINT32(1, context.blockBuff.length);
	
	retVal = READER_T1_BUFFER_SetLength(&context, 0);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	TEST_ASSERT_EQUAL_UINT32(0, context.blockBuff.length);
}


void test_T1_BUFFER_SetLength_shoulCheckMaxSize(void){
	READER_T1_ContextHandler context;
	READER_Status retVal;
	
	
	retVal = READER_T1_BUFFER_Init(&context);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	retVal = READER_T1_BUFFER_SetLength(&context, READER_T1_CONTEXT_STATICBUFF_MAXSIZE-1);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	retVal = READER_T1_BUFFER_SetLength(&context, READER_T1_CONTEXT_STATICBUFF_MAXSIZE);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	if(READER_T1_CONTEXT_STATICBUFF_MAXSIZE < READER_T1_CONTEXT_STATICBUFF_MAXSIZE +1){
		retVal = READER_T1_BUFFER_SetLength(&context, READER_T1_CONTEXT_STATICBUFF_MAXSIZE +1);
		TEST_ASSERT_FALSE(retVal == READER_OK);
	}
}



void test_READER_T1_BUFFER_IncLength_shouldWork(void){
	READER_T1_ContextHandler context;
	READER_Status retVal;
	
	
	retVal = READER_T1_BUFFER_Init(&context);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	if(READER_T1_CONTEXT_STATICBUFF_MAXSIZE < 1){
		TEST_IGNORE();
	}
	retVal = READER_T1_BUFFER_IncLength(&context);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	TEST_ASSERT_EQUAL_UINT32(1, context.blockBuff.length);
	
	if(READER_T1_CONTEXT_STATICBUFF_MAXSIZE < 2){
		TEST_IGNORE();
	}
	retVal = READER_T1_BUFFER_IncLength(&context);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	TEST_ASSERT_EQUAL_UINT32(2, context.blockBuff.length);
	
	if(READER_T1_CONTEXT_STATICBUFF_MAXSIZE < 3){
		TEST_IGNORE();
	}
	retVal = READER_T1_BUFFER_IncLength(&context);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	TEST_ASSERT_EQUAL_UINT32(3, context.blockBuff.length);
}


void test_READER_T1_BUFFER_IncLength_shouldCheckMaxSize(void){
	READER_T1_ContextHandler context;
	READER_Status retVal;
	
	
	retVal = READER_T1_BUFFER_Init(&context);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	retVal = READER_T1_BUFFER_SetLength(&context, READER_T1_CONTEXT_STATICBUFF_MAXSIZE-1);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	retVal = READER_T1_BUFFER_IncLength(&context);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	retVal = READER_T1_BUFFER_IncLength(&context);
	TEST_ASSERT_FALSE(retVal == READER_OK);
}


void test_READER_T1_BUFFER_DecLength_shouldWork(void){
	READER_T1_ContextHandler context;
	READER_Status retVal;
	
	
	retVal = READER_T1_BUFFER_Init(&context);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	if(READER_T1_CONTEXT_STATICBUFF_MAXSIZE <= 3){
		TEST_IGNORE();
	}
	
	retVal = READER_T1_BUFFER_SetLength(&context, 3);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	
	retVal = READER_T1_BUFFER_DecLength(&context);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	TEST_ASSERT_EQUAL_UINT32(2, context.blockBuff.length);
	

	retVal = READER_T1_BUFFER_DecLength(&context);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	TEST_ASSERT_EQUAL_UINT32(1, context.blockBuff.length);
	

	retVal = READER_T1_BUFFER_DecLength(&context);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	TEST_ASSERT_EQUAL_UINT32(0, context.blockBuff.length);
}


void test_READER_T1_BUFFER_DecLength_shouldCheckMinSize(void){
	READER_T1_ContextHandler context;
	READER_Status retVal;
	
	
	retVal = READER_T1_BUFFER_Init(&context);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	retVal = READER_T1_BUFFER_DecLength(&context);
	TEST_ASSERT_FALSE(retVal == READER_OK);
	TEST_ASSERT_EQUAL_UINT32(0, context.blockBuff.length);
}


void test_READER_T1_BUFFER_PlacesLeft_shouldWork(void){
	READER_T1_ContextHandler context;
	READER_T1_Block block;
	READER_Status retVal;
	uint32_t places;
	
	
	retVal = READER_T1_BUFFER_Init(&context);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	if(READER_T1_CONTEXT_STATICBUFF_MAXSIZE < 1){
		TEST_IGNORE();
	}	
	
	retVal = READER_T1_BUFFER_PlacesLeft(&context, &places);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	TEST_ASSERT_EQUAL_UINT32(READER_T1_CONTEXT_STATICBUFF_MAXSIZE, places);
	
	retVal = READER_T1_ForgeSBlock(&block, READER_T1_STYPE_IFS_REQU, READER_T1_LRC);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	retVal = READER_T1_BUFFER_Stack(&context, &block);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	retVal = READER_T1_BUFFER_PlacesLeft(&context, &places);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	TEST_ASSERT_EQUAL_UINT32(READER_T1_CONTEXT_STATICBUFF_MAXSIZE-1, places);
}


void test_READER_T1_BUFFER_PlacesLeft_shouldWorkWhenFull(void){
	READER_T1_ContextHandler context;
	READER_T1_Block block;
	READER_Status retVal;
	uint32_t places;
	uint32_t i;
	
	
	retVal = READER_T1_BUFFER_Init(&context);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	if(READER_T1_CONTEXT_STATICBUFF_MAXSIZE < 1){
		TEST_IGNORE();
	}
	
	retVal = READER_T1_ForgeSBlock(&block, READER_T1_STYPE_RESYNCH_REQU, READER_T1_LRC);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	for(i=0; i<READER_T1_CONTEXT_STATICBUFF_MAXSIZE; i++){
		retVal = READER_T1_BUFFER_Enqueue(&context, &block);
		TEST_ASSERT_TRUE(retVal == READER_OK);
	}
	
	retVal = READER_T1_BUFFER_PlacesLeft(&context, &places);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	TEST_ASSERT_EQUAL_UINT32(0, places);
}


void test_READER_T1_BUFFER_IsFull_shouldWork(void){
	READER_T1_ContextHandler context;
	READER_T1_Block block;
	READER_T1_BufferStatus bufferStatus;
	READER_Status retVal;
	uint32_t i;
	
	
	retVal = READER_T1_BUFFER_Init(&context);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	if(READER_T1_CONTEXT_STATICBUFF_MAXSIZE < 2){
		TEST_IGNORE();
	}
	
	/* Cas 1 */
	retVal = READER_T1_BUFFER_IsFull(&context, &bufferStatus);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	TEST_ASSERT_TRUE(bufferStatus == READER_T1_BUFFER_NOTFULL);
	
	/* Cas 2 */
	retVal = READER_T1_ForgeSBlock(&block, READER_T1_STYPE_IFS_REQU, READER_T1_LRC);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	retVal = READER_T1_BUFFER_Stack(&context, &block);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	retVal = READER_T1_BUFFER_IsFull(&context, &bufferStatus);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	TEST_ASSERT_TRUE(bufferStatus == READER_T1_BUFFER_NOTFULL);
	
	
	/* Cas 3 */
	for(i=0; i<READER_T1_CONTEXT_STATICBUFF_MAXSIZE-1;i++){
		retVal = READER_T1_BUFFER_Stack(&context, &block);
		TEST_ASSERT_TRUE(retVal == READER_OK);
	}
	
	retVal = READER_T1_BUFFER_IsFull(&context, &bufferStatus);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	TEST_ASSERT_TRUE(bufferStatus == READER_T1_BUFFER_FULL);
}


void test_READER_T1_BUFFER_IsEmpty_shouldWork(void){
	READER_T1_ContextHandler context;
	READER_T1_Block block;
	READER_T1_BufferStatus bufferStatus;
	READER_Status retVal;
	uint32_t i;
	
	
	if(READER_T1_CONTEXT_STATICBUFF_MAXSIZE < 2){
		TEST_IGNORE();
	}
	
	retVal = READER_T1_BUFFER_Init(&context);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	retVal = READER_T1_BUFFER_IsEmpty(&context, &bufferStatus);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	TEST_ASSERT_TRUE(bufferStatus == READER_T1_BUFFER_EMPTY);
	
	
	retVal = READER_T1_ForgeSBlock(&block, READER_T1_STYPE_IFS_REQU, READER_T1_LRC);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	retVal = READER_T1_BUFFER_Stack(&context, &block);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	retVal = READER_T1_BUFFER_IsEmpty(&context, &bufferStatus);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	TEST_ASSERT_TRUE(bufferStatus == READER_T1_BUFFER_NOTEMPTY);
}


void test_READER_T1_BUFFER_Stack_stackAndDequeueShouldWorkWithSBlock_case1(void){
	READER_T1_ContextHandler context;
	READER_T1_Block block1, block2;
	READER_Status retVal;
	uint8_t dataLEN;
	uint8_t corrCodeLength;
	
	
	if(READER_T1_CONTEXT_STATICBUFF_MAXSIZE < 1){
		TEST_IGNORE();
	}
	
	retVal = READER_T1_BUFFER_Init(&context);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	retVal = READER_T1_ForgeSBlock(&block1, READER_T1_STYPE_IFS_REQU, READER_T1_LRC);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	dataLEN = READER_T1_GetBlockLEN(&block1);
	corrCodeLength = READER_T1_GetBlockRedundancyLen(&block1);
	
	retVal = READER_T1_BUFFER_Stack(&context, &block1);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	retVal = READER_T1_BUFFER_Dequeue(&context, &block2);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	TEST_ASSERT_EQUAL_UINT8_ARRAY(block1.blockFrame, block2.blockFrame, READER_T1_BLOCK_PROLOGUE_SIZE + dataLEN + corrCodeLength);
	TEST_ASSERT_EQUAL_UINT32(block1.RedundancyType, block2.RedundancyType);
}


void test_READER_T1_BUFFER_Stack_stackAndDequeueShouldWorkWithSBlock_case2(void){
	READER_T1_ContextHandler context;
	READER_T1_Block block1, block2, block3;
	READER_Status retVal;
	uint8_t dataLEN;
	uint8_t corrCodeLength;
	
	
	/* On stack deux Blocks, on regarde lequel sort en premier ...  */
	
	if(READER_T1_CONTEXT_STATICBUFF_MAXSIZE < 2){
		TEST_IGNORE();
	}
	
	retVal = READER_T1_BUFFER_Init(&context);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	retVal = READER_T1_ForgeSBlock(&block1, READER_T1_STYPE_IFS_REQU, READER_T1_LRC);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	retVal = READER_T1_BUFFER_Stack(&context, &block1);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	retVal = READER_T1_ForgeRBlock(&block2, READER_T1_ACKTYPE_ACK, READER_T1_EXPSEQNUM_ONE, READER_T1_LRC);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	dataLEN = READER_T1_GetBlockLEN(&block2);
	corrCodeLength = READER_T1_GetBlockRedundancyLen(&block2);
	
	retVal = READER_T1_BUFFER_Stack(&context, &block2);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	retVal = READER_T1_BUFFER_Dequeue(&context, &block3);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	TEST_ASSERT_EQUAL_UINT8_ARRAY(block2.blockFrame, block3.blockFrame, READER_T1_BLOCK_PROLOGUE_SIZE + dataLEN + corrCodeLength);
	TEST_ASSERT_EQUAL_UINT32(block2.RedundancyType, block3.RedundancyType);
}


void test_READER_T1_BUFFER_Enqueue_enqueueAndDequeueShouldWorkWithSBlock_case1(void){
	READER_T1_ContextHandler context;
	READER_T1_Block block1, block2;
	READER_Status retVal;
	uint8_t dataLEN;
	uint8_t corrCodeLength;
	
	
	if(READER_T1_CONTEXT_STATICBUFF_MAXSIZE < 1){
		TEST_IGNORE();
	}
	
	retVal = READER_T1_BUFFER_Init(&context);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	retVal = READER_T1_ForgeSBlock(&block1, READER_T1_STYPE_IFS_REQU, READER_T1_LRC);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	dataLEN = READER_T1_GetBlockLEN(&block1);
	corrCodeLength = READER_T1_GetBlockRedundancyLen(&block1);
	
	retVal = READER_T1_BUFFER_Enqueue(&context, &block1);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	retVal = READER_T1_BUFFER_Dequeue(&context, &block2);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	TEST_ASSERT_EQUAL_UINT8_ARRAY(block1.blockFrame, block2.blockFrame, READER_T1_BLOCK_PROLOGUE_SIZE + dataLEN + corrCodeLength);
	TEST_ASSERT_EQUAL_UINT32(block1.RedundancyType, block2.RedundancyType);
}



void test_READER_T1_BUFFER_Enqueue_enqueueAndDequeueShouldWorkWithSBlock_case2(void){
	READER_T1_ContextHandler context;
	READER_T1_Block block1, block2, block3;
	READER_Status retVal;
	uint8_t dataLEN;
	uint8_t corrCodeLength;
	
	
	/* On enfile deux blocks et on regarde si c'est le bon qui sort ...  */
	
	
	if(READER_T1_CONTEXT_STATICBUFF_MAXSIZE < 2){
		TEST_IGNORE();
	}
	
	retVal = READER_T1_BUFFER_Init(&context);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	retVal = READER_T1_ForgeSBlock(&block1, READER_T1_STYPE_IFS_REQU, READER_T1_LRC);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	dataLEN = READER_T1_GetBlockLEN(&block1);
	corrCodeLength = READER_T1_GetBlockRedundancyLen(&block1);
	
	retVal = READER_T1_BUFFER_Enqueue(&context, &block1);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	retVal = READER_T1_ForgeRBlock(&block2, READER_T1_ACKTYPE_ACK, READER_T1_EXPSEQNUM_ONE, READER_T1_LRC);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	retVal = READER_T1_BUFFER_Enqueue(&context, &block2);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	retVal = READER_T1_BUFFER_Dequeue(&context, &block3);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	TEST_ASSERT_EQUAL_UINT8_ARRAY(block1.blockFrame, block3.blockFrame, READER_T1_BLOCK_PROLOGUE_SIZE + dataLEN + corrCodeLength);
	TEST_ASSERT_EQUAL_UINT32(block1.RedundancyType, block3.RedundancyType);
}


void test_READER_T1_BUFFER_Dequeue_shouldCheckIfEmpty(void){
	READER_T1_ContextHandler context;
	READER_T1_Block block;
	READER_Status retVal;
	
	
	if(READER_T1_CONTEXT_STATICBUFF_MAXSIZE < 1){
		TEST_IGNORE();
	}
	
	retVal = READER_T1_BUFFER_Init(&context);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	retVal = READER_T1_BUFFER_Dequeue(&context, &block);
	TEST_ASSERT_TRUE(retVal == READER_EMPTY);
}


void test_READER_T1_BUFFER_Stack_shouldCheckIfFull(void){
	READER_T1_ContextHandler context;
	READER_T1_Block block;
	READER_Status retVal;
	uint32_t i;
	
	
	if(READER_T1_CONTEXT_STATICBUFF_MAXSIZE < 1){
		TEST_IGNORE();
	}
	
	retVal = READER_T1_BUFFER_Init(&context);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	retVal = READER_T1_ForgeSBlock(&block, READER_T1_STYPE_WTX_REQU, READER_T1_LRC);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	for(i=0; i<READER_T1_CONTEXT_STATICBUFF_MAXSIZE; i++){
		retVal = READER_T1_BUFFER_Stack(&context, &block);
		TEST_ASSERT_TRUE(retVal == READER_OK);
	}
	
	retVal = READER_T1_BUFFER_Stack(&context, &block);
	TEST_ASSERT_TRUE(retVal == READER_FULL);
	
}


void test_READER_T1_BUFFER_Enqueue_shouldCheckIfFull(void){
	READER_T1_ContextHandler context;
	READER_T1_Block block;
	READER_Status retVal;
	uint32_t i;
	
	
	if(READER_T1_CONTEXT_STATICBUFF_MAXSIZE < 1){
		TEST_IGNORE();
	}
	
	retVal = READER_T1_BUFFER_Init(&context);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	retVal = READER_T1_ForgeSBlock(&block, READER_T1_STYPE_WTX_REQU, READER_T1_LRC);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	for(i=0; i<READER_T1_CONTEXT_STATICBUFF_MAXSIZE; i++){
		retVal = READER_T1_BUFFER_Enqueue(&context, &block);
		TEST_ASSERT_TRUE(retVal == READER_OK);
	}
	
	retVal = READER_T1_BUFFER_Enqueue(&context, &block);
	TEST_ASSERT_TRUE(retVal == READER_FULL);
	
}


void test_READER_T1_BUFFER_Enqueue_shouldUpdateIBlockCounter(void){
	READER_T1_ContextHandler context;
	READER_T1_Block blockS, blockI, block;
	READER_Status retVal;
	uint8_t data[READER_T1_BLOCK_MAX_DATA_SIZE];
	uint8_t dataSize = READER_T1_BLOCK_MAX_DATA_SIZE;
	uint32_t i;
	uint32_t IBlockCount;
	
	
	srand(time(NULL));
	for(i=0; i<READER_T1_BLOCK_MAX_DATA_SIZE; i++){
		data[i] = (uint8_t)(rand());
	}
	
	if(READER_T1_CONTEXT_STATICBUFF_MAXSIZE < 2){
		TEST_IGNORE();
	}
	
	retVal = READER_T1_BUFFER_Init(&context);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	/* On forge un S-Block et un I-Block ...  */
	retVal = READER_T1_ForgeSBlock(&blockS, READER_T1_STYPE_WTX_REQU, READER_T1_LRC);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	retVal = READER_T1_ForgeIBlock(&blockI, data, dataSize, READER_T1_SEQNUM_ZERO, READER_T1_MBIT_ZERO, READER_T1_LRC);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	/* On enfile le S-Block, on verifie que ca n'incremente pas le compteur de I-Blocks ...  */
	retVal = READER_T1_BUFFER_Enqueue(&context, &blockS);
	TEST_ASSERT_TRUE(retVal == READER_OK);

	retVal = READER_T1_BUFFER_GetIBlockCount(&context, &IBlockCount);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	TEST_ASSERT_EQUAL_UINT32(0, IBlockCount);
	
	/* On enfile le I-Block, on verifie que ca incremente le compteur de I-Blocks ...  */
	retVal = READER_T1_BUFFER_Enqueue(&context, &blockI);
	TEST_ASSERT_TRUE(retVal == READER_OK);

	retVal = READER_T1_BUFFER_GetIBlockCount(&context, &IBlockCount);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	TEST_ASSERT_EQUAL_UINT32(1, IBlockCount);
	
	/* On defile le dernier Block (S-Block), on verifie ca ca ne change pas le compteur de I-Blocks ...  */
	retVal = READER_T1_BUFFER_Dequeue(&context, &block);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	retVal = READER_T1_BUFFER_GetIBlockCount(&context, &IBlockCount);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	TEST_ASSERT_EQUAL_UINT32(1, IBlockCount);
	
	/* On defile le dernier Block (I-Block), on verifie ca decremente le compteur de I-Blocks ...  */
	retVal = READER_T1_BUFFER_Dequeue(&context, &block);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	retVal = READER_T1_BUFFER_GetIBlockCount(&context, &IBlockCount);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	TEST_ASSERT_EQUAL_UINT32(0, IBlockCount);
}



void test_READER_T1_BUFFER_Stack_shouldUpdateIBlockCounter(void){
	READER_T1_ContextHandler context;
	READER_T1_Block blockS, blockI, block;
	READER_Status retVal;
	uint8_t data[READER_T1_BLOCK_MAX_DATA_SIZE];
	uint8_t dataSize = READER_T1_BLOCK_MAX_DATA_SIZE;
	uint32_t i;
	uint32_t IBlockCount;
	
	
	srand(time(NULL));
	for(i=0; i<READER_T1_BLOCK_MAX_DATA_SIZE; i++){
		data[i] = (uint8_t)(rand());
	}
	
	if(READER_T1_CONTEXT_STATICBUFF_MAXSIZE < 2){
		TEST_IGNORE();
	}
	
	retVal = READER_T1_BUFFER_Init(&context);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	/* On forge un S-Block et un I-Block ...  */
	retVal = READER_T1_ForgeSBlock(&blockS, READER_T1_STYPE_WTX_REQU, READER_T1_LRC);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	retVal = READER_T1_ForgeIBlock(&blockI, data, dataSize, READER_T1_SEQNUM_ZERO, READER_T1_MBIT_ZERO, READER_T1_LRC);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	/* On empile le S-Block, on verifie que ca n'incremente pas le compteur de I-Blocks ...  */
	retVal = READER_T1_BUFFER_Stack(&context, &blockS);
	TEST_ASSERT_TRUE(retVal == READER_OK);

	retVal = READER_T1_BUFFER_GetIBlockCount(&context, &IBlockCount);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	TEST_ASSERT_EQUAL_UINT32(0, IBlockCount);
	
	/* On empile le I-Block, on verifie que ca incremente le compteur de I-Blocks ...  */
	retVal = READER_T1_BUFFER_Stack(&context, &blockI);
	TEST_ASSERT_TRUE(retVal == READER_OK);

	retVal = READER_T1_BUFFER_GetIBlockCount(&context, &IBlockCount);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	TEST_ASSERT_EQUAL_UINT32(1, IBlockCount);
	
	/* On defile le dernier Block (I-Block), on verifie ca decremente le compteur de I-Blocks ...  */
	retVal = READER_T1_BUFFER_Dequeue(&context, &block);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	retVal = READER_T1_BUFFER_GetIBlockCount(&context, &IBlockCount);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	TEST_ASSERT_EQUAL_UINT32(0, IBlockCount);
	
	/* On defile le dernier Block (S-Block), on verifie ca ca ne change pas le compteur de I-Blocks ...  */
	retVal = READER_T1_BUFFER_Dequeue(&context, &block);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	retVal = READER_T1_BUFFER_GetIBlockCount(&context, &IBlockCount);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	TEST_ASSERT_EQUAL_UINT32(0, IBlockCount);
}



void test_READER_T1_BUFFER_GetRBlockAndSBlockCount_shouldWork(void){
	READER_T1_ContextHandler context;
	READER_T1_Block blockS, blockI, blockR, block;
	READER_Status retVal;
	uint8_t data[READER_T1_BLOCK_MAX_DATA_SIZE];
	uint8_t dataSize = READER_T1_BLOCK_MAX_DATA_SIZE;
	uint32_t i;
	uint32_t count;
	
	
	srand(time(NULL));
	for(i=0; i<READER_T1_BLOCK_MAX_DATA_SIZE; i++){
		data[i] = (uint8_t)(rand());
	}
	
	if(READER_T1_CONTEXT_STATICBUFF_MAXSIZE < 3){
		TEST_IGNORE();
	}
	
	retVal = READER_T1_BUFFER_Init(&context);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	/* On forge un S-Block, un R-Block et un I-Block ...  */
	retVal = READER_T1_ForgeSBlock(&blockS, READER_T1_STYPE_WTX_REQU, READER_T1_LRC);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	retVal = READER_T1_ForgeIBlock(&blockI, data, dataSize, READER_T1_SEQNUM_ZERO, READER_T1_MBIT_ZERO, READER_T1_LRC);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	retVal = READER_T1_ForgeRBlock(&blockR, READER_T1_ACKTYPE_NACK, READER_T1_EXPSEQNUM_ONE, READER_T1_LRC);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	
	/* Quand on est vide on verfie que le resultat est 0 ...  */
	retVal = READER_T1_BUFFER_GetRBlockAndSBlockCount(&context, &count);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	TEST_ASSERT_EQUAL_UINT32(0, count);
	
	/* On empile le S-Block, on verifie que ca incremente ...  */
	retVal = READER_T1_BUFFER_Stack(&context, &blockS);
	TEST_ASSERT_TRUE(retVal == READER_OK);

	retVal = READER_T1_BUFFER_GetRBlockAndSBlockCount(&context, &count);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	TEST_ASSERT_EQUAL_UINT32(1, count);
	
	/* On empile le I-Block, on verifie que ca incremente pas ...  */
	retVal = READER_T1_BUFFER_Stack(&context, &blockI);
	TEST_ASSERT_TRUE(retVal == READER_OK);

	retVal = READER_T1_BUFFER_GetRBlockAndSBlockCount(&context, &count);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	TEST_ASSERT_EQUAL_UINT32(1, count);
	
	/* On empile le R-Block, on verifie que ca incremente ...  */
	retVal = READER_T1_BUFFER_Stack(&context, &blockR);
	TEST_ASSERT_TRUE(retVal == READER_OK);

	retVal = READER_T1_BUFFER_GetRBlockAndSBlockCount(&context, &count);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	TEST_ASSERT_EQUAL_UINT32(2, count);
	
	/* On defile le R-Block, on verifie que ca decremente ...  */
	retVal = READER_T1_BUFFER_Dequeue(&context, &block);
	TEST_ASSERT_TRUE(retVal == READER_OK);

	retVal = READER_T1_BUFFER_GetRBlockAndSBlockCount(&context, &count);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	TEST_ASSERT_EQUAL_UINT32(1, count);
	
	/* On defile le I-Block, on verifie que ca decremente pas ...  */
	retVal = READER_T1_BUFFER_Dequeue(&context, &block);
	TEST_ASSERT_TRUE(retVal == READER_OK);

	retVal = READER_T1_BUFFER_GetRBlockAndSBlockCount(&context, &count);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	TEST_ASSERT_EQUAL_UINT32(1, count);
	
	/* On defile le S-Block, on verifie que ca decremente ...  */
	retVal = READER_T1_BUFFER_Dequeue(&context, &block);
	TEST_ASSERT_TRUE(retVal == READER_OK);

	retVal = READER_T1_BUFFER_GetRBlockAndSBlockCount(&context, &count);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	TEST_ASSERT_EQUAL_UINT32(0, count);
}



void test_READER_T1_BUFFER_ExtractRawDataFromBuffer_shouldWork_case1(void){
	READER_T1_ContextHandler context;
	READER_T1_Block block1, block2;
	READER_Status retVal;
	uint8_t data1[READER_T1_BLOCK_MAX_DATA_SIZE];
	uint8_t dataSize1 = READER_T1_BLOCK_MAX_DATA_SIZE;
	uint8_t data2[READER_T1_BLOCK_MAX_DATA_SIZE];
	uint8_t dataSize2 = READER_T1_BLOCK_MAX_DATA_SIZE;
	uint8_t dataGlobal[READER_T1_BLOCK_MAX_DATA_SIZE + READER_T1_BLOCK_MAX_DATA_SIZE];
	uint32_t dataSizeGlobal = READER_T1_BLOCK_MAX_DATA_SIZE + READER_T1_BLOCK_MAX_DATA_SIZE;
	uint8_t dataBuff[dataSize1 + dataSize2];
	uint32_t dataBuffSize = dataSize1 + dataSize2;
	uint32_t nbExtracted;
	uint32_t i;
	
	
	/* On prepare deux I-Blocks ...  */
	srand(time(NULL));
	for(i=0; i<dataSize1; i++){
		data1[i] = (uint8_t)(rand());
	}
	for(i=0; i<dataSize2; i++){
		data2[i] = (uint8_t)(rand());
	}
	for(i=0; i<dataSize1; i++){
		dataGlobal[i] = data1[i];
	}
	for(i=0; i<dataSize2; i++){
		dataGlobal[i+dataSize2] = data2[i];
	}
	
	retVal = READER_T1_ForgeIBlock(&block1, data1, dataSize1, READER_T1_SEQNUM_ZERO, READER_T1_MBIT_ONE, READER_T1_LRC);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	retVal = READER_T1_ForgeIBlock(&block2, data2, dataSize2, READER_T1_SEQNUM_ONE, READER_T1_MBIT_ZERO, READER_T1_LRC);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	
	/* On initialise la Stack ...  */
	if(READER_T1_CONTEXT_STATICBUFF_MAXSIZE < 1){
		TEST_IGNORE();
	}
	
	retVal = READER_T1_BUFFER_Init(&context);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	/* On enfile les deux I-Blocks dans la file ...  */
	retVal = READER_T1_BUFFER_Enqueue(&context, &block1);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	retVal = READER_T1_BUFFER_Enqueue(&context, &block2);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	/* On verifie le fonctionnement de la fonction ...  */
	retVal = READER_T1_BUFFER_ExtractRawDataFromBuffer(&context, dataBuff, dataBuffSize, &nbExtracted);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	TEST_ASSERT_EQUAL_UINT32(dataBuffSize, nbExtracted);
	TEST_ASSERT_EQUAL_UINT8_ARRAY(dataGlobal, dataBuff, dataSizeGlobal);
}


void test_READER_T1_BUFFER_ExtractRawDataFromBuffer_shouldWork_case2(void){
	READER_T1_ContextHandler context;
	READER_T1_Block block1, block2, block3;
	READER_Status retVal;
	uint8_t data1[READER_T1_BLOCK_MAX_DATA_SIZE];
	uint8_t dataSize1 = READER_T1_BLOCK_MAX_DATA_SIZE;
	uint8_t data2[READER_T1_BLOCK_MAX_DATA_SIZE];
	uint8_t dataSize2 = READER_T1_BLOCK_MAX_DATA_SIZE;
	uint8_t dataGlobal[READER_T1_BLOCK_MAX_DATA_SIZE + READER_T1_BLOCK_MAX_DATA_SIZE];
	uint32_t dataSizeGlobal = READER_T1_BLOCK_MAX_DATA_SIZE + READER_T1_BLOCK_MAX_DATA_SIZE;
	uint8_t dataBuff[dataSize1 + dataSize2];
	uint32_t dataBuffSize = dataSize1 + dataSize2;
	uint32_t nbExtracted;
	uint32_t i;
	
	
	/* On teste le cas ou top > bottom ...  */
	
	/* On prepare deux I-Blocks ...  */
	srand(time(NULL));
	for(i=0; i<dataSize1; i++){
		data1[i] = (uint8_t)(rand());
	}
	for(i=0; i<dataSize2; i++){
		data2[i] = (uint8_t)(rand());
	}
	for(i=0; i<dataSize1; i++){
		dataGlobal[i] = data1[i];
	}
	for(i=0; i<dataSize2; i++){
		dataGlobal[i+dataSize2] = data2[i];
	}
	
	retVal = READER_T1_ForgeIBlock(&block1, data1, dataSize1, READER_T1_SEQNUM_ZERO, READER_T1_MBIT_ONE, READER_T1_LRC);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	retVal = READER_T1_ForgeIBlock(&block2, data2, dataSize2, READER_T1_SEQNUM_ONE, READER_T1_MBIT_ZERO, READER_T1_LRC);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	
	/* On initialise la Stack ...  */
	if(READER_T1_CONTEXT_STATICBUFF_MAXSIZE < 1){
		TEST_IGNORE();
	}
	
	retVal = READER_T1_BUFFER_Init(&context);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	/* On se place dans le cas ou top > bottom ...  */
	retVal = READER_T1_ForgeSBlock(&block3, READER_T1_STYPE_WTX_REQU, READER_T1_LRC);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	for(i=0; i<READER_T1_CONTEXT_STATICBUFF_MAXSIZE; i++){
		retVal = READER_T1_BUFFER_Enqueue(&context, &block3);
		TEST_ASSERT_TRUE(retVal == READER_OK);
	}
	
	for(i=0; i<READER_T1_CONTEXT_STATICBUFF_MAXSIZE -1; i++){
		retVal = READER_T1_BUFFER_Dequeue(&context, &block3);
		TEST_ASSERT_TRUE(retVal == READER_OK);
	}
	
	/* On enfile les deux I-Blocks dans la file ...  */
	retVal = READER_T1_BUFFER_Enqueue(&context, &block1);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	retVal = READER_T1_BUFFER_Enqueue(&context, &block2);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	/* On verifie le fonctionnement de la fonction ...  */
	retVal = READER_T1_BUFFER_ExtractRawDataFromBuffer(&context, dataBuff, dataBuffSize, &nbExtracted);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	TEST_ASSERT_EQUAL_UINT32(dataBuffSize, nbExtracted);
	TEST_ASSERT_EQUAL_UINT8_ARRAY(dataGlobal, dataBuff, dataSizeGlobal);
}


void test_READER_T1_BUFFER_ExtractRawDataFromBuffer_shouldCheckMaxSize(void){
	READER_T1_ContextHandler context;
	READER_T1_Block block1, block2, block3;
	READER_Status retVal;
	uint8_t data1[READER_T1_BLOCK_MAX_DATA_SIZE];
	uint8_t dataSize1 = READER_T1_BLOCK_MAX_DATA_SIZE;
	uint8_t data2[READER_T1_BLOCK_MAX_DATA_SIZE];
	uint8_t dataSize2 = READER_T1_BLOCK_MAX_DATA_SIZE;
	uint8_t dataGlobal[READER_T1_BLOCK_MAX_DATA_SIZE + READER_T1_BLOCK_MAX_DATA_SIZE];
	uint32_t dataSizeGlobal = READER_T1_BLOCK_MAX_DATA_SIZE + READER_T1_BLOCK_MAX_DATA_SIZE ;
	uint8_t dataBuff[dataSize1 + dataSize2 -1];
	uint32_t dataBuffSize = dataSize1 + dataSize2 -1;
	uint32_t nbExtracted;
	uint32_t i;
	
	
	/* On teste le cas ou top > bottom ...  */
	
	/* On prepare deux I-Blocks ...  */
	srand(time(NULL));
	for(i=0; i<dataSize1; i++){
		data1[i] = (uint8_t)(rand());
	}
	for(i=0; i<dataSize2; i++){
		data2[i] = (uint8_t)(rand());
	}
	for(i=0; i<dataSize1; i++){
		dataGlobal[i] = data1[i];
	}
	for(i=0; i<dataSize2; i++){
		dataGlobal[i+dataSize2] = data2[i];
	}
	
	retVal = READER_T1_ForgeIBlock(&block1, data1, dataSize1, READER_T1_SEQNUM_ZERO, READER_T1_MBIT_ONE, READER_T1_LRC);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	retVal = READER_T1_ForgeIBlock(&block2, data2, dataSize2, READER_T1_SEQNUM_ONE, READER_T1_MBIT_ZERO, READER_T1_LRC);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	
	/* On initialise la Stack ...  */
	if(READER_T1_CONTEXT_STATICBUFF_MAXSIZE < 1){
		TEST_IGNORE();
	}
	
	retVal = READER_T1_BUFFER_Init(&context);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	/* On enfile les deux I-Blocks dans la file ...  */
	retVal = READER_T1_BUFFER_Enqueue(&context, &block1);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	retVal = READER_T1_BUFFER_Enqueue(&context, &block2);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	/* On verifie le fonctionnement de la fonction ...  */
	retVal = READER_T1_BUFFER_ExtractRawDataFromBuffer(&context, dataBuff, dataBuffSize, &nbExtracted);
	TEST_ASSERT_TRUE(retVal == READER_OVERFLOW);
}


void test_READER_T1_BUFFER_ExtractRawDataFromBuffer_shouldWorkWhenContolBlocksInsideBuffer(void){
	READER_T1_ContextHandler context;
	READER_T1_Block block1, block2, block3, block4;
	READER_Status retVal;
	uint8_t data1[READER_T1_BLOCK_MAX_DATA_SIZE];
	uint8_t dataSize1 = READER_T1_BLOCK_MAX_DATA_SIZE;
	uint8_t data2[READER_T1_BLOCK_MAX_DATA_SIZE];
	uint8_t dataSize2 = READER_T1_BLOCK_MAX_DATA_SIZE;
	uint8_t dataGlobal[READER_T1_BLOCK_MAX_DATA_SIZE + READER_T1_BLOCK_MAX_DATA_SIZE];
	uint32_t dataSizeGlobal = READER_T1_BLOCK_MAX_DATA_SIZE + READER_T1_BLOCK_MAX_DATA_SIZE;
	uint8_t dataBuff[dataSize1 + dataSize2];
	uint32_t dataBuffSize = dataSize1 + dataSize2;
	uint32_t nbExtracted;
	uint32_t i;
	
	
	/* On prepare deux I-Blocks ...  */
	srand(time(NULL));
	for(i=0; i<dataSize1; i++){
		data1[i] = (uint8_t)(rand());
	}
	for(i=0; i<dataSize2; i++){
		data2[i] = (uint8_t)(rand());
	}
	for(i=0; i<dataSize1; i++){
		dataGlobal[i] = data1[i];
	}
	for(i=0; i<dataSize2; i++){
		dataGlobal[i+dataSize2] = data2[i];
	}
	
	retVal = READER_T1_ForgeIBlock(&block1, data1, dataSize1, READER_T1_SEQNUM_ZERO, READER_T1_MBIT_ONE, READER_T1_LRC);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	retVal = READER_T1_ForgeIBlock(&block2, data2, dataSize2, READER_T1_SEQNUM_ONE, READER_T1_MBIT_ZERO, READER_T1_LRC);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	retVal = READER_T1_ForgeRBlock(&block3, READER_T1_ACKTYPE_NACK_CRCLRC, READER_T1_EXPSEQNUM_ONE, READER_T1_LRC);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	retVal = READER_T1_ForgeSBlock(&block4, READER_T1_STYPE_ABORT_REQU, READER_T1_LRC);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	/* On initialise la Stack ...  */
	if(READER_T1_CONTEXT_STATICBUFF_MAXSIZE < 1){
		TEST_IGNORE();
	}
	
	retVal = READER_T1_BUFFER_Init(&context);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	/* On enfile les deux I-Blocks dans la file ...  */
	retVal = READER_T1_BUFFER_Enqueue(&context, &block1);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	retVal = READER_T1_BUFFER_Enqueue(&context, &block3);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	retVal = READER_T1_BUFFER_Enqueue(&context, &block2);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	retVal = READER_T1_BUFFER_Stack(&context, &block4);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	/* On verifie le fonctionnement de la fonction ...  */
	retVal = READER_T1_BUFFER_ExtractRawDataFromBuffer(&context, dataBuff, dataBuffSize, &nbExtracted);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	TEST_ASSERT_EQUAL_UINT32(dataBuffSize, nbExtracted);
	TEST_ASSERT_EQUAL_UINT8_ARRAY(dataGlobal, dataBuff, dataSizeGlobal);
}


void test_READER_T1_BUFFER_StripIBlocks_shouldWork(void){
	READER_T1_ContextHandler context;
	READER_T1_Block block1, block2, block3, block4, block;
	READER_T1_BufferStatus buffStatus;
	READER_Status retVal;
	uint8_t data1[READER_T1_BLOCK_MAX_DATA_SIZE];
	uint8_t dataSize1 = READER_T1_BLOCK_MAX_DATA_SIZE;
	uint8_t data2[READER_T1_BLOCK_MAX_DATA_SIZE];
	uint8_t dataSize2 = READER_T1_BLOCK_MAX_DATA_SIZE;
	uint8_t dataGlobal[READER_T1_BLOCK_MAX_DATA_SIZE + READER_T1_BLOCK_MAX_DATA_SIZE];
	uint32_t dataSizeGlobal = READER_T1_BLOCK_MAX_DATA_SIZE + READER_T1_BLOCK_MAX_DATA_SIZE;
	uint8_t dataBuff[dataSize1 + dataSize2];
	uint32_t dataBuffSize = dataSize1 + dataSize2;
	uint32_t nbExtracted;
	uint32_t i, count;
	uint8_t dataLEN;
	uint8_t corrCodeLength;
	
	
	/* On prepare deux I-Blocks ...  */
	srand(time(NULL));
	for(i=0; i<dataSize1; i++){
		data1[i] = (uint8_t)(rand());
	}
	for(i=0; i<dataSize2; i++){
		data2[i] = (uint8_t)(rand());
	}
	for(i=0; i<dataSize1; i++){
		dataGlobal[i] = data1[i];
	}
	for(i=0; i<dataSize2; i++){
		dataGlobal[i+dataSize2] = data2[i];
	}
	
	retVal = READER_T1_ForgeIBlock(&block1, data1, dataSize1, READER_T1_SEQNUM_ZERO, READER_T1_MBIT_ONE, READER_T1_LRC);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	retVal = READER_T1_ForgeIBlock(&block2, data2, dataSize2, READER_T1_SEQNUM_ONE, READER_T1_MBIT_ZERO, READER_T1_LRC);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	retVal = READER_T1_ForgeRBlock(&block3, READER_T1_ACKTYPE_NACK_CRCLRC, READER_T1_EXPSEQNUM_ONE, READER_T1_LRC);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	retVal = READER_T1_ForgeSBlock(&block4, READER_T1_STYPE_ABORT_REQU, READER_T1_LRC);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	/* On initialise la Stack ...  */
	if(READER_T1_CONTEXT_STATICBUFF_MAXSIZE < 1){
		TEST_IGNORE();
	}
	
	retVal = READER_T1_BUFFER_Init(&context);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	/* On enfile les deux I-Blocks dans la file ...  */
	retVal = READER_T1_BUFFER_Enqueue(&context, &block1);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	retVal = READER_T1_BUFFER_Enqueue(&context, &block3);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	retVal = READER_T1_BUFFER_Enqueue(&context, &block2);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	retVal = READER_T1_BUFFER_Stack(&context, &block4);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	/* On verifie le fonctionnement de la fonction ...  */
	retVal = READER_T1_BUFFER_GetIBlockCount(&context, &count);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	TEST_ASSERT_EQUAL_UINT32(2, count);
	
	retVal = READER_T1_BUFFER_GetRBlockAndSBlockCount(&context, &count);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	TEST_ASSERT_EQUAL_UINT32(2, count);
	
	retVal = READER_T1_BUFFER_StripIBlocks(&context);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	retVal = READER_T1_BUFFER_GetIBlockCount(&context, &count);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	TEST_ASSERT_EQUAL_UINT32(0, count);
	
	retVal = READER_T1_BUFFER_GetRBlockAndSBlockCount(&context, &count);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	TEST_ASSERT_EQUAL_UINT32(2, count);
	
	/* On verifie que ca n'a pas altere les autres Block dans le Buffer ...  */
	retVal = READER_T1_BUFFER_Dequeue(&context, &block);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	dataLEN = READER_T1_GetBlockLEN(&block4);
	corrCodeLength = READER_T1_GetBlockRedundancyLen(&block4);
	TEST_ASSERT_EQUAL_UINT8_ARRAY(block4.blockFrame, block.blockFrame, READER_T1_BLOCK_PROLOGUE_SIZE + dataLEN + corrCodeLength);
	TEST_ASSERT_EQUAL_UINT32(block4.RedundancyType, block.RedundancyType);
	
	retVal = READER_T1_BUFFER_Dequeue(&context, &block);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	dataLEN = READER_T1_GetBlockLEN(&block3);
	corrCodeLength = READER_T1_GetBlockRedundancyLen(&block3);
	TEST_ASSERT_EQUAL_UINT8_ARRAY(block3.blockFrame, block.blockFrame, READER_T1_BLOCK_PROLOGUE_SIZE + dataLEN + corrCodeLength);
	TEST_ASSERT_EQUAL_UINT32(block3.RedundancyType, block.RedundancyType);
	
	retVal = READER_T1_BUFFER_IsEmpty(&context, &buffStatus);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	TEST_ASSERT_TRUE(buffStatus == READER_T1_BUFFER_EMPTY);
}



void test_READER_T1_BUFFER_StripControlBlocks_shouldWork(void){
	READER_T1_ContextHandler context;
	READER_T1_Block block1, block2, block3, block4, block;
	READER_T1_BufferStatus buffStatus;
	READER_Status retVal;
	uint8_t data1[READER_T1_BLOCK_MAX_DATA_SIZE];
	uint8_t dataSize1 = READER_T1_BLOCK_MAX_DATA_SIZE;
	uint8_t data2[READER_T1_BLOCK_MAX_DATA_SIZE];
	uint8_t dataSize2 = READER_T1_BLOCK_MAX_DATA_SIZE;
	uint8_t dataGlobal[READER_T1_BLOCK_MAX_DATA_SIZE + READER_T1_BLOCK_MAX_DATA_SIZE];
	uint32_t dataSizeGlobal = READER_T1_BLOCK_MAX_DATA_SIZE + READER_T1_BLOCK_MAX_DATA_SIZE;
	uint8_t dataBuff[dataSize1 + dataSize2];
	uint32_t dataBuffSize = dataSize1 + dataSize2;
	uint32_t nbExtracted;
	uint32_t i, count;
	uint8_t dataLEN;
	uint8_t corrCodeLength;
	
	
	/* On prepare deux I-Blocks ...  */
	srand(time(NULL));
	for(i=0; i<dataSize1; i++){
		data1[i] = (uint8_t)(rand());
	}
	for(i=0; i<dataSize2; i++){
		data2[i] = (uint8_t)(rand());
	}
	for(i=0; i<dataSize1; i++){
		dataGlobal[i] = data1[i];
	}
	for(i=0; i<dataSize2; i++){
		dataGlobal[i+dataSize2] = data2[i];
	}
	
	retVal = READER_T1_ForgeIBlock(&block1, data1, dataSize1, READER_T1_SEQNUM_ZERO, READER_T1_MBIT_ONE, READER_T1_LRC);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	retVal = READER_T1_ForgeIBlock(&block2, data2, dataSize2, READER_T1_SEQNUM_ONE, READER_T1_MBIT_ZERO, READER_T1_LRC);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	retVal = READER_T1_ForgeRBlock(&block3, READER_T1_ACKTYPE_NACK_CRCLRC, READER_T1_EXPSEQNUM_ONE, READER_T1_LRC);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	retVal = READER_T1_ForgeSBlock(&block4, READER_T1_STYPE_ABORT_REQU, READER_T1_LRC);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	/* On initialise la Stack ...  */
	if(READER_T1_CONTEXT_STATICBUFF_MAXSIZE < 1){
		TEST_IGNORE();
	}
	
	retVal = READER_T1_BUFFER_Init(&context);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	/* On enfile les deux I-Blocks dans la file ...  */
	retVal = READER_T1_BUFFER_Enqueue(&context, &block1);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	retVal = READER_T1_BUFFER_Enqueue(&context, &block3);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	retVal = READER_T1_BUFFER_Enqueue(&context, &block2);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	retVal = READER_T1_BUFFER_Stack(&context, &block4);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	/* On verifie le fonctionnement de la fonction ...  */
	retVal = READER_T1_BUFFER_GetIBlockCount(&context, &count);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	TEST_ASSERT_EQUAL_UINT32(2, count);
	
	retVal = READER_T1_BUFFER_GetRBlockAndSBlockCount(&context, &count);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	TEST_ASSERT_EQUAL_UINT32(2, count);
	
	
	retVal = READER_T1_BUFFER_StripControlBlocks(&context);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	retVal = READER_T1_BUFFER_GetIBlockCount(&context, &count);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	TEST_ASSERT_EQUAL_UINT32(2, count);
	
	retVal = READER_T1_BUFFER_GetRBlockAndSBlockCount(&context, &count);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	TEST_ASSERT_EQUAL_UINT32(0, count);
	
	/* On verifie que ca n'a pas altere les autres Block dans le Buffer ...  */
	retVal = READER_T1_BUFFER_Dequeue(&context, &block);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	dataLEN = READER_T1_GetBlockLEN(&block1);
	corrCodeLength = READER_T1_GetBlockRedundancyLen(&block1);
	TEST_ASSERT_EQUAL_UINT8_ARRAY(block1.blockFrame, block.blockFrame, READER_T1_BLOCK_PROLOGUE_SIZE + dataLEN + corrCodeLength);
	TEST_ASSERT_EQUAL_UINT32(block1.RedundancyType, block.RedundancyType);
	
	retVal = READER_T1_BUFFER_Dequeue(&context, &block);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	dataLEN = READER_T1_GetBlockLEN(&block2);
	corrCodeLength = READER_T1_GetBlockRedundancyLen(&block2);
	TEST_ASSERT_EQUAL_UINT8_ARRAY(block2.blockFrame, block.blockFrame, READER_T1_BLOCK_PROLOGUE_SIZE + dataLEN + corrCodeLength);
	TEST_ASSERT_EQUAL_UINT32(block2.RedundancyType, block.RedundancyType);
	
	retVal = READER_T1_BUFFER_IsEmpty(&context, &buffStatus);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	TEST_ASSERT_TRUE(buffStatus == READER_T1_BUFFER_EMPTY);
}



void test_READER_T1_BUFFER_GetBottomBlockPtr_shouldPointCorrectBlock(void){
	READER_T1_ContextHandler context;
	READER_T1_Block block1, block2;
	READER_T1_Block *pBlock;
	READER_Status retVal;
	uint8_t dataLEN;
	uint8_t corrCodeLength;
	
	
	/* On initialise le Buffer ...  */
	if(READER_T1_CONTEXT_STATICBUFF_MAXSIZE < 1){
		TEST_IGNORE();
	}
	
	retVal = READER_T1_BUFFER_Init(&context);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	/* On prepare des Blocks a utiliser ...  */
	retVal = READER_T1_ForgeRBlock(&block1, READER_T1_ACKTYPE_NACK_CRCLRC, READER_T1_EXPSEQNUM_ONE, READER_T1_LRC);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	retVal = READER_T1_ForgeSBlock(&block2, READER_T1_STYPE_ABORT_REQU, READER_T1_LRC);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	/* On enfile un premier Block et on verifie que ca pointe bien au bon endroit ...  */
	retVal = READER_T1_BUFFER_Stack(&context, &block1);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	retVal = READER_T1_BUFFER_GetBottomBlockPtr(&context, &pBlock);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	dataLEN = READER_T1_GetBlockLEN(&block1);
	corrCodeLength = READER_T1_GetBlockRedundancyLen(&block1);
	TEST_ASSERT_EQUAL_UINT8_ARRAY(block1.blockFrame, pBlock->blockFrame, READER_T1_BLOCK_PROLOGUE_SIZE + dataLEN + corrCodeLength);
	TEST_ASSERT_EQUAL_UINT32(block1.RedundancyType, pBlock->RedundancyType);
	
	
	/* On enfile un deuxieme Block et on verifie que ca pointe bien au bon endroit ...  */
	retVal = READER_T1_BUFFER_Stack(&context, &block2);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	retVal = READER_T1_BUFFER_GetBottomBlockPtr(&context, &pBlock);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	dataLEN = READER_T1_GetBlockLEN(&block2);
	corrCodeLength = READER_T1_GetBlockRedundancyLen(&block2);
	TEST_ASSERT_EQUAL_UINT8_ARRAY(block2.blockFrame, pBlock->blockFrame, READER_T1_BLOCK_PROLOGUE_SIZE + dataLEN + corrCodeLength);
	TEST_ASSERT_EQUAL_UINT32(block2.RedundancyType, pBlock->RedundancyType);
}

void test_READER_T1_BUFFER_GetBottomBlockPtr_shouldDetectEmptyBuffer(void){
	READER_T1_ContextHandler context;
	READER_T1_Block *pBlock;
	READER_Status retVal;
	
	
	
	if(READER_T1_CONTEXT_STATICBUFF_MAXSIZE < 1){
		TEST_IGNORE();
	}
	
	retVal = READER_T1_BUFFER_Init(&context);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	retVal = READER_T1_BUFFER_GetBottomBlockPtr(&context, &pBlock);
	TEST_ASSERT_FALSE(retVal == READER_OK);
}


void test_READER_T1_BUFFER_GetBottomBlockType_shouldDetectEmptyBuffer(void){
	READER_T1_ContextHandler context;
	READER_T1_BlockType bType;
	READER_Status retVal;
	
	
	if(READER_T1_CONTEXT_STATICBUFF_MAXSIZE < 1){
		TEST_IGNORE();
	}
	
	retVal = READER_T1_BUFFER_Init(&context);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	retVal = READER_T1_BUFFER_GetBottomBlockType(&context, &bType);
	TEST_ASSERT_FALSE(retVal == READER_OK);
}


void test_READER_T1_BUFFER_GetBottomBlockType_shouldWork(void){
	READER_T1_ContextHandler context;
	READER_T1_BlockType bType;
	READER_T1_Block block1, block2, block3;
	READER_Status retVal;
	uint8_t data[READER_T1_BLOCK_MAX_DATA_SIZE];
	uint8_t dataSize = READER_T1_BLOCK_MAX_DATA_SIZE;
	uint32_t i;
	
	
	/* On initialise le Buffer ...  */
	if(READER_T1_CONTEXT_STATICBUFF_MAXSIZE < 1){
		TEST_IGNORE();
	}
	
	retVal = READER_T1_BUFFER_Init(&context);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	srand(time(NULL));
	for(i=0; i<dataSize; i++){
		data[i] = (uint8_t)(rand());
	}
	
	/* On forge un Block de chaque type ...  */
	retVal = READER_T1_ForgeSBlock(&block1, READER_T1_STYPE_WTX_REQU, READER_T1_LRC);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	retVal = READER_T1_ForgeIBlock(&block2, data, dataSize, READER_T1_SEQNUM_ZERO, READER_T1_MBIT_ZERO, READER_T1_LRC);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	retVal = READER_T1_ForgeRBlock(&block3, READER_T1_ACKTYPE_NACK, READER_T1_EXPSEQNUM_ONE, READER_T1_LRC);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	
	/* On effecture les tests ...  */
	retVal = READER_T1_BUFFER_Stack(&context, &block1);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	retVal = READER_T1_BUFFER_GetBottomBlockType(&context, &bType);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	TEST_ASSERT_TRUE(bType == READER_T1_SBLOCK);
	
	
	retVal = READER_T1_BUFFER_Stack(&context, &block2);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	retVal = READER_T1_BUFFER_GetBottomBlockType(&context, &bType);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	TEST_ASSERT_TRUE(bType == READER_T1_IBLOCK);
	
	
	retVal = READER_T1_BUFFER_Stack(&context, &block3);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	
	retVal = READER_T1_BUFFER_GetBottomBlockType(&context, &bType);
	TEST_ASSERT_TRUE(retVal == READER_OK);
	TEST_ASSERT_TRUE(bType == READER_T1_RBLOCK);
}
