#ifndef __READER_TESTS_TPDU_H__
#define __READER_TESTS_TPDU_H__


#include "reader_tpdu.h"





void test_READER_TPDU_all(void);

void test_READER_TPDU_Forge_shouldVerifyDataSize(void);
void test_READER_TPDU_Forge_shouldCopyHeader(void);
void test_READER_TPDU_Forge_shouldCopyData(void);
void test_READER_TPDU_SendHeader_shouldSendRightFrame(void);
void test_READER_TPDU_SendDataOneshot_shouldSendRightFrame(void);




#endif
