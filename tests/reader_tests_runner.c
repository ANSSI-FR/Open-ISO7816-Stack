#include "unity.h"
#include "reader_tests_test.h"
#include "reader_tests_tpdu.h"


#ifdef TEST
int main(int argc, char *argv[]){
	UNITY_BEGIN();

	test_READER_TPDU_all();
	
	return UNITY_END();
}
#endif
