#ifndef __READER_TESTS_BLOCK_H__
#define __READER_TESTS_BLOCK_H__



#include "reader_t1_block.h"





void test_READER_BLOCK_all(void);

void test_READER_T1_ForgeBlock_shouldSetCorrectInitValues(void);
void test_READER_T1_SetBlockData_shouldCheckMaxLength(void);
void test_READER_T1_SetBlockData_shouldUpdateLRC(void);
void test_READER_T1_SetBlockType_shouldIBlockWork(void);
void test_READER_T1_SetBlockType_shouldRBlockWork(void);
void test_READER_T1_SetBlockType_shouldSBlockWork(void);
void test_READER_T1_SetBlockData_shouldUpdateLEN(void);
void test_READER_T1_SetBlockMBit_shouldWork(void);
void test_READER_T1_SetBlockSeqNumber_shouldWork(void);
void test_READER_T1_ComputeBlockLRC_shouldWork(void);
void test_READER_T1_SetBlockSType_shouldResynchRequestWork(void);
void test_READER_T1_SetBlockSType_shouldResynchResponseWork(void);
void test_READER_T1_SetBlockSType_shouldIFSRequestWork(void);
void test_READER_T1_SetBlockSType_shouldIFSResponseWork(void);
void test_READER_T1_SetBlockSType_shouldWTXRequestWork(void);
void test_READER_T1_SetBlockSType_shouldWTXResponseWork(void);


#endif
