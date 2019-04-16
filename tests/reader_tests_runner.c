#include "unity.h"
#include "reader_tests_test.h"
#include "reader_tests_tpdu.h"


#ifdef TEST
int main(int argc, char *argv[]){
	UNITY_BEGIN();

	//RUN_TEST(test_addition_shouldWork);
	//RUN_TEST(test_moyenne_shouldWork);
	RUN_TEST(test_all_READER_TPDU);
	
	return UNITY_END();
}
#endif
