#ifndef __READER_TESTS_TPDU_H__
#define __READER_TESTS_TPDU_H__


#include "reader_tpdu.h"





void test_READER_TPDU_all(void);

void test_READER_TPDU_Forge_shouldVerifyDataSize(void);
void test_READER_TPDU_Forge_shouldCopyHeader(void);
void test_READER_TPDU_Forge_shouldCopyData(void);
void test_READER_TPDU_SendHeader_shouldSendRightFrame(void);
void test_READER_TPDU_SendDataOneshot_shouldSendRightFrame(void);
void test_READER_TPDU_SendDataOneshot_shouldVerifyDataSize(void);
void test_READER_TPDU_IsACK_shouldWork(void);
void test_READER_TPDU_IsXoredACK_shouldWork(void);
void test_READER_TPDU_IsNullByte_shouldWork(void);
void test_READER_TPDU_IsSW1_shouldWork(void);
void test_READER_TPDU_IsProcedureByte_shouldWork(void);
void test_READER_TPDU_WaitACK_shouldDetectINS(void);
void test_READER_TPDU_WaitACK_shouldDetectXoredINS(void);
void test_READER_TPDU_WaitACK_shouldTimeout(void);




#endif
