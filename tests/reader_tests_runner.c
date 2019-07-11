#include "unity.h"
#include "reader_tests_test.h"
#include "reader_tests_tpdu.h"
#include "reader_tests_block.h"

#include "mock_reader_hal_basis.h"
#include "mock_reader_hal_comm_settings.h"
#include "mock_reader_periph.h"

#include <sys/time.h>


#ifdef TEST

uint32_t tick_callback(int NumCalls){
	struct timeval tv;
	
	gettimeofday(&tv, NULL);
	return (uint32_t)(tv.tv_usec/1000);
}




void setUp(void){
	mock_reader_hal_basis_Init();
	mock_reader_hal_comm_settings_Init();
	mock_reader_periph_Init();
	
	READER_HAL_GetFreq_IgnoreAndReturn(READER_HAL_DEFAULT_FREQ);
	READER_HAL_GetDi_IgnoreAndReturn(READER_HAL_DEFAULT_DI);
	READER_HAL_GetFi_IgnoreAndReturn(READER_HAL_DEFAULT_FI);
	READER_HAL_GetGT_IgnoreAndReturn(READER_HAL_DEFAULT_GT);
	READER_HAL_GetTick_StubWithCallback(tick_callback);
}


void tearDown(void){
	mock_reader_hal_basis_Verify();
	mock_reader_hal_comm_settings_Verify();
	mock_reader_periph_Verify();
}


int main(int argc, char *argv[]){
	UNITY_BEGIN();
	
	test_READER_TPDU_all();
	test_READER_BLOCK_all();
	test_READER_BUFFER_all();
	
	return UNITY_END();
}
#endif
