#ifndef __READER_TESTS_TEST_H__
#define __READER_TESTS_TEST_H__


#include "unity.h"


void emulate_RcvCharFrame(uint8_t *data, uint32_t dataSize);
void set_expected_CharFrame(uint8_t *expectedBytes, uint32_t size);




#endif
