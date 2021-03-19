#include "unity.h"
#include "mock_reader_hal_basis.h"



void emulate_RcvCharFrame(uint8_t *data, uint32_t dataSize){
	//READER_HAL_CommSettings dummySettings;
	//READER_HAL_Protocol dummyProtocol;
	//READER_Status retVal;
	//uint8_t character;
	//uint32_t timeout = 1000;
	uint32_t i;
	
	for(i=0; i<dataSize; i++){
		//READER_HAL_RcvChar_ExpectAndReturn(&dummySettings, dummyProtocol, &character, timeout, READER_OK);
		//READER_HAL_RcvChar_IgnoreArg_pSettings();
		//READER_HAL_RcvChar_IgnoreArg_protocol();
		//READER_HAL_RcvChar_IgnoreArg_character();
		//READER_HAL_RcvChar_IgnoreArg_timeout();
		READER_HAL_RcvChar_ExpectAnyArgsAndReturn(READER_OK);
		READER_HAL_RcvChar_ReturnThruPtr_character(data+i);
	}
}


void set_expected_CharFrame(uint8_t *expectedBytes, uint32_t size){
	READER_HAL_CommSettings dummySettings;
	uint32_t timeout = 1000;
	uint32_t i;
	
	for(i=0; i<size; i++){
		READER_HAL_SendChar_ExpectAndReturn(&dummySettings, READER_HAL_PROTOCOL_T1, expectedBytes[i], timeout, READER_OK);
		READER_HAL_SendChar_IgnoreArg_pSettings();
		READER_HAL_SendChar_IgnoreArg_protocol();
		READER_HAL_SendChar_IgnoreArg_timeout();
	}
}
