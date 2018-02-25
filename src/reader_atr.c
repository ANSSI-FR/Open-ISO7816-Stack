#include <stdint.h>
#include "reader_hal.h"




/* Voire table 7 section 8.3 ISO7816_3 */
static uint16_t globalFiConvTable[0x10] = {372, 372, 558, 744, 1116, 1488, 1860, 372, 372, 512, 768, 1024, 1536, 2048, 372, 372};
static uint8_t  globalDiConvTable[0x10] = {1, 1, 2, 4, 8, 16, 32, 64, 12, 20, 1, 1, 1, 1, 1, 1};
static uint32_t globalFMaxConvTable[0x10] = {4000000, 5000000, 6000000, 8000000, 12000000, 16000000, 20000000, 0xFFFFFFFF, 0xFFFFFFFF, 5000000, 7500000, 10000000, 15000000, 20000000, 0xFFFFFFFF, 0xFFFFFFFF};



READER_Status READER_ATR_Receive(READER_ATR_Atr *atr){
	uint32_t i = 0;
	uint8_t TS, T0;
	uint8_t TA, TB, TC, TD;
	uint8_t Y, T;
	
	
	/* Recuperation de TS et T0 */
	if(READER_HAL_RcvChar(&TS) != READER_OK) return READER_ERR;
	if(READER_ATR_CheckTS(TS) != READER_OK) return READER_ERR;
	atr->encodingConv = READER_ATR_GetEncoding(TS);
	
	if(READER_HAL_RcvChar(&T0) != READER_OK) return READER_ERR;
	atr->K = READER_ATR_GetK(T0);
	
	Y = READER_ATR_GetY(T0);
	
	/* Recupertion de tous les Interfaces Bytes */
	while(READER_ATR_IsInterfacesBytesToRead(Y)){
		if(READER_ATR_IsTAToRead(Y)){
			if(READER_HAL_RcvChar(&TA) != READER_OK) return READER_ERR;
		}
		if(READER_ATR_IsTBToRead(Y)){
			if(READER_HAL_RcvChar(&TB) != READER_OK) return READER_ERR;
		}
		if(READER_ATR_IsTCToRead(Y)){
			if(READER_HAL_RcvChar(&TC) != READER_OK) return READER_ERR;
		}
		if(READER_ATR_IsTDToRead(Y)){
			if(READER_HAL_RcvChar(&TD) != READER_OK) return READER_ERR;
			Y = READER_ATR_GetY(TD);
			T = READER_ATR_GetT(TD);
		}
		else{
			Y = 0x00;
		}
		
		i++;
	}
	
	/* Recuperation de tous les Historical Bytes */
	
	
	/* Recuperation du Check Byte */
	
	
	return READER_OK;
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


uint32_t READER_ATR_GetFi(uint8_t TA1){
	uint8_t k;
	
	k = (TA1 >> 4) & 0x0F;
	return globalFiConvTable[k];
}


uint32_t READER_ATR_GetFMax(uint8_t TA1){
	uint8_t k;
	
	k = (TA1 >> 4) & 0x0F;
	return globalFMaxConvTable[k];
}


uint32_t READER_ATR_GetDi(uint8_t TA1){
	uint8_t k;
	
	k = TA1 & 0xF0;
	return globalDiConvTable[k];
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


READER_ATR_EncodingConv READER_ATR_GetEncoding(uint8_t TS){
	if(TS == READER_ATR_ENCODING_DIRECT){
		return READER_ATR_ENCODING_DIRECT;
	}
	else{
		return READER_ATR_ENCODING_REVERSE;
	}
}


READER_Status READER_ATR_ProcessTA(READER_ATR_Atr *atr, uint8_t TA, uint32_t i, uint8_t T){
	if(i == 1){
		
	}
	else{
		
	}
	
	return READER_OK;
}


READER_Status READER_ATR_ProcessTB(READER_ATR_Atr *atr, uint8_t TB, uint32_t i, uint8_t T){
	
}


READER_Status READER_ATR_ProcessTC(READER_ATR_Atr *atr, uint8_t TC, uint32_t i, uint8_t T){
	
}

