#include <stdint.h>
#include "reader_hal.h"






READER_Status READER_ATR_Receive(READER_ATR_Atr *atr){
	uint8_t TS, T0;
	
	if(READER_HAL_RcvChar(&TS) != READER_OK) return READER_ERR;
	if(READER_ATR_CheckTS(TS) != READER_OK) return READER_ERR;
	atr->encodingConv = READER_ATR_RetrieveEncoding(TS);
	
	if(READER_HAL_RcvChar(&T0) != READER_OK) return READER_ERR;
	
	
	
}




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


uint8_t READER_ATR_GetY(uint8_t TD){
	return (TD >> 4) & 0xF0;
}


uint8_t READER_ATR_GetT(uint8_t TD){
	return TD & 0x0F;
}

uint8_t READER_ATR_GetK(uint8_t T0){
	return T0 & 0x0F;
}


void READER_ATR_ErrHandler(void){
	while(1){
		
	}
}



READER_Status READER_ATR_CheckTS(uint8_t TS){
	if(TS == READER_ATR_ENCODING_DIRECT){
		return READER_OK;
	}
	else if(TS == READER_ATR_ENCODING_REVERSE){
		return READER_OK;
	}
	else{
		return READER_ERR;
	}
}


READER_ATR_EncodingConv READER_ATR_RetrieveEncoding(uint8_t TS){
	if(TS == READER_ATR_ENCODING_DIRECT){
		return READER_ATR_ENCODING_DIRECT;
	}
	else{
		return READER_ATR_ENCODING_REVERSE;
	}
}
