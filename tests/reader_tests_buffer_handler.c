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
	RUN_TEST(test_READER_T1_BUFFER_IsFull_shouldWork);
	RUN_TEST(test_READER_T1_BUFFER_IsEmpty_shouldWork);
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
