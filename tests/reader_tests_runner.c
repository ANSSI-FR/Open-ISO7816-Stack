#include "unity.h"
#include "reader_tests_test.h"
#include "reader_tests_tpdu.h"
#include "reader_tests_block.h"
#include "reader_tests_apdu_t1.h"

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
	READER_HAL_InitHardware_IgnoreAndReturn(READER_OK);
	READER_HAL_SetFreq_IgnoreAndReturn(READER_OK);
	READER_HAL_SetFi_IgnoreAndReturn(READER_OK);
	READER_HAL_SetDi_IgnoreAndReturn(READER_OK);
	READER_HAL_SetGT_IgnoreAndReturn(READER_OK);
	READER_HAL_SetEtu_IgnoreAndReturn(READER_OK);
	READER_HAL_Delay_Ignore();
	READER_HAL_SetPwrLine_IgnoreAndReturn(READER_OK);
	READER_HAL_SetClkLine_IgnoreAndReturn(READER_OK);
	READER_HAL_SetRstLine_IgnoreAndReturn(READER_OK);
	READER_HAL_SetIOLine_IgnoreAndReturn(READER_OK);
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
	test_READER_T1_APDU_all();
	
	return UNITY_END();
}
#endif
