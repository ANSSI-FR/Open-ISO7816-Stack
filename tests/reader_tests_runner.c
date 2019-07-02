#include "unity.h"
#include "reader_tests_test.h"
#include "reader_tests_tpdu.h"
#include "reader_tests_block.h"


#ifdef TEST
int main(int argc, char *argv[]){
	UNITY_BEGIN();

	test_READER_TPDU_all();
	test_READER_BLOCK_all();
	
	return UNITY_END();
}
#endif
