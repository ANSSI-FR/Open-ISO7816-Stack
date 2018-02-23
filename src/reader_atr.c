#include <stdint.h>
#include "reader_hal.h"






READER_Status READER_ATR_IsInterfacesBytesToRead(uint8_t Y){
	if(Y != 0){
		return READER_OK;
	}
	else{
		return READER_NO;
	}
}


READER_Status READER_ATR_IsTAToRead(uint8_t Y){
	if(0x0001 & Y){
		return READER_OK;
	}
	else{
		return READER_NO;
	}
}


READER_Status READER_ATR_IsTBToRead(uint8_t Y){
	if(0x0002 & Y){
		return READER_OK;
	}
	else{
		return READER_NO;
	}
}


READER_Status READER_ATR_IsTCToRead(uint8_t Y){
	if(0x0004 & Y){
		return READER_OK;
	}
	else{
		return READER_NO;
	}
}


READER_Status READER_ATR_IsTDToRead(uint8_t Y){
	if(0x0008 & Y){
		return READER_OK;
	}
	else{
		return READER_NO;
	}
}
