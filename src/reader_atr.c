#include <stdint.h>
#include "reader_atr.h"
#include "reader_hal.h"




/* Voir table 7 section 8.3 ISO7816_3 */
static uint16_t globalFiConvTable[0x10] = {372, 372, 558, 744, 1116, 1488, 1860, 372, 372, 512, 768, 1024, 1536, 2048, 372, 372};
static uint8_t  globalDiConvTable[0x10] = {1, 1, 2, 4, 8, 16, 32, 64, 12, 20, 1, 1, 1, 1, 1, 1};
static uint32_t globalFMaxConvTable[0x10] = {4000000, 5000000, 6000000, 8000000, 12000000, 16000000, 20000000, 0xFFFFFFFF, 0xFFFFFFFF, 5000000, 7500000, 10000000, 15000000, 20000000, 0xFFFFFFFF, 0xFFFFFFFF};



READER_Status READER_ATR_Receive(READER_ATR_Atr *atr){
	uint32_t i = 1;
	uint8_t TS, T0;
	uint8_t TA, TB, TC, TD;
	uint8_t Y, T = 0;
	
	
	/* Initialisation des certains elements de la structure ATR */
	READER_ATR_InitStruct(atr);
		
	
	/* Recuperation de TS et T0 */
	if(READER_HAL_RcvChar(&TS, READER_HAL_USE_ISO_WT) != READER_OK) return READER_ERR;
	if(READER_ATR_CheckTS(TS) != READER_OK) return READER_ERR;
	atr->encodingConv = READER_ATR_GetEncoding(TS);
	
	if(READER_HAL_RcvChar(&T0, READER_HAL_USE_ISO_WT) != READER_OK) return READER_ERR;
	atr->K = READER_ATR_ComputeK(T0);
	
	Y = READER_ATR_ComputeY(T0);
	
	/* Recupertion de tous les Interfaces Bytes */
	while(READER_ATR_IsInterfacesBytesToRead(Y)){
		if(READER_ATR_IsTAToRead(Y)){
			if(READER_HAL_RcvChar(&TA, READER_HAL_USE_ISO_WT) != READER_OK) return READER_ERR;
			if(READER_ATR_ProcessTA(atr, TA, i, T) != READER_OK) return READER_ERR;
		}
		if(READER_ATR_IsTBToRead(Y)){
			if(READER_HAL_RcvChar(&TB, READER_HAL_USE_ISO_WT) != READER_OK) return READER_ERR;
			if(READER_ATR_ProcessTB(atr, TB, i, T) != READER_OK) return READER_ERR;
		}
		if(READER_ATR_IsTCToRead(Y)){
			if(READER_HAL_RcvChar(&TC, READER_HAL_USE_ISO_WT) != READER_OK) return READER_ERR;
			if(READER_ATR_ProcessTC(atr, TC, i, T) != READER_OK) return READER_ERR;
		}
		if(READER_ATR_IsTDToRead(Y)){
			if(READER_HAL_RcvChar(&TD, READER_HAL_USE_ISO_WT) != READER_OK) return READER_ERR;
			Y = READER_ATR_ComputeY(TD);
			T = READER_ATR_ComputeT(TD);
		}
		else{
			Y = 0x00;
		}
		i++;
	}
	
	/* Recuperation de tous les Historical Bytes */
	READER_HAL_RcvCharFrame(atr->histBytes, atr->K, READER_HAL_USE_ISO_WT);
	
	/* Recuperation du Check Byte */
	
	
	return READER_OK;
}


/**
 * \fn READER_Status READER_ATR_ApplySettings(READER_ATR_Atr *atr)
 * \brief Cette fonction permet d'appliquer les parametres de communication indiqués dans l'ATR.
 * \return Retourne uen code d'erreur de stype READER_Status. Retourne READER_OK si l'exécution s'est correctement déroulée. Pour toute autre valeur il s'agit d'une erreur.
 * \param *atr Pointeur sur une structure de type READER_ATR_Atr;
 */
READER_Status READER_ATR_ApplySettings(READER_ATR_Atr *atr){
	READER_Status retVal;
	
	retVal = READER_HAL_SetEtu(atr->Fi, atr->Di);  if(retVal != READER_OK) return retVal; 
	retVal = READER_HAL_SetGT();                   if(retVal != READER_OK) return retVal; 
	retVal = READER_HAL_SetWT();                   if(retVal != READER_OK) return retVal; 
	retVal = READER_HAL_SetFreq();                 if(retVal != READER_OK) return retVal;  
	
	return READER_OK;   
}



READER_Status READER_ATR_InitStruct(READER_ATR_Atr *atr){
	uint32_t j;
	
	atr->Fi = READER_ATR_VALUE_NOT_INDICATED;
	atr->Di = READER_ATR_VALUE_NOT_INDICATED;
	atr->fMax = READER_ATR_VALUE_NOT_INDICATED;
	atr->N = 0;
	
	atr->T0Protocol.TABytesCount = 0;
	atr->T0Protocol.TBBytesCount = 0;
	atr->T0Protocol.TCBytesCount = 0;
	atr->T1Protocol.TABytesCount = 0;
	atr->T1Protocol.TBBytesCount = 0;
	atr->T1Protocol.TCBytesCount = 0;
	
	atr->clockStopIndicator      = READER_ATR_CLOCKSTOP_NOTINDICATED;
	atr->classIndicator          = READER_ATR_CLASS_NOTINDICATED;
	atr->useOfSPU                = READER_ATR_SPU_NOTINDICATED;
	atr->K                       = 0;
	
	
	for(j=0; j<READER_ATR_MAX_SPECIFIC_BYTES; j++){
		atr->T0Protocol.TABytes[j] = 0x00;
	}
	for(j=0; j<READER_ATR_MAX_SPECIFIC_BYTES; j++){
		atr->T0Protocol.TBBytes[j] = 0x00;
	}
	for(j=0; j<READER_ATR_MAX_SPECIFIC_BYTES; j++){
		atr->T0Protocol.TCBytes[j] = 0x00;
	}
	
	
	for(j=0; j<READER_ATR_MAX_SPECIFIC_BYTES; j++){
		atr->T1Protocol.TABytes[j] = 0x00;
	}
	for(j=0; j<READER_ATR_MAX_SPECIFIC_BYTES; j++){
		atr->T1Protocol.TBBytes[j] = 0x00;
	}
	for(j=0; j<READER_ATR_MAX_SPECIFIC_BYTES; j++){
		atr->T1Protocol.TCBytes[j] = 0x00;
	}
	
	for(j=0; j<READER_ATR_MAX_HIST_BYTES; j++){
		atr->histBytes[j] = 0x00;
	}
	
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


uint8_t READER_ATR_ComputeY(uint8_t TD){
	return (TD >> 4) & 0x0F;
}


uint8_t READER_ATR_ComputeT(uint8_t TD){
	return TD & 0x0F;
}

uint8_t READER_ATR_ComputeK(uint8_t T0){
	return T0 & 0x0F;
}


uint32_t READER_ATR_ComputeFi(uint8_t TA1){
	uint8_t k;
	
	k = (TA1 >> 4) & 0x0F;
	return (uint32_t)(globalFiConvTable[k]);
}


uint32_t READER_ATR_ComputeFMax(uint8_t TA1){
	uint8_t k;
	
	k = (TA1 >> 4) & 0x0F;
	return (uint32_t)(globalFMaxConvTable[k]);
}


uint32_t READER_ATR_ComputeDi(uint8_t TA1){
	uint8_t k;
	
	k = TA1 & 0x0F;
	return (uint32_t)(globalDiConvTable[k]);
}

READER_ATR_ClockStopIndicator READER_ATR_GetClockStopIndic(uint8_t TA15){
	return (TA15 >> 6) & 0x03;
}


READER_ATR_ClassIndicator READER_ATR_GetClassIndic(uint8_t TA15){
	return  TA15 & 0x40;
}

READER_ATR_UseOfSPU READER_ATR_GetUseSPU(uint8_t TB15){
	if(TB15 == 0x00){
		return READER_ATR_SPU_NOTUSED;
	}
	else if((TB15 & 0x80) == 0){
		return READER_ATR_SPU_PROPRIETARY;
	}
	else{
		return READER_ATR_SPU_STANDARD;
	}
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
	if(i == 1){          /* Global interface Byte */
		atr->Fi = READER_ATR_ComputeFi(TA);
		atr->Di = READER_ATR_ComputeDi(TA);
		atr->fMax = READER_ATR_ComputeFMax(TA);
	}
	else if(T == 15){    /* Global Interface Byte */
		atr->clockStopIndicator = READER_ATR_GetClockStopIndic(TA);
		atr->classIndicator = READER_ATR_GetClassIndic(TA);
	}
	else if(T == 0){
		atr->T0Protocol.TABytes[atr->T0Protocol.TABytesCount] = TA;
		atr->T0Protocol.TABytesCount++;
	}
	else if(T == 1){
		atr->T1Protocol.TABytes[atr->T1Protocol.TABytesCount] = TA;
		atr->T1Protocol.TABytesCount++;
	}
	else{
		return READER_ERR;
	}
	
	return READER_OK;
}


READER_Status READER_ATR_ProcessTB(READER_ATR_Atr *atr, uint8_t TB, uint32_t i, uint8_t T){
	if((i == 1) || (i == 2)){
		return READER_OK;        /* TB1 et TB2 sont deprecated voir section 8.3  ISO7816_3. Ils doivent etre ignores */
	}
	else if(TB == 15){
		atr->useOfSPU = READER_ATR_GetUseSPU(TB);
	}
	else if(T == 0){
		atr->T0Protocol.TBBytes[atr->T0Protocol.TBBytesCount] = TB;
		atr->T0Protocol.TBBytesCount++;
	}
	else if(T == 1){
		atr->T1Protocol.TBBytes[atr->T1Protocol.TBBytesCount] = TB;
		atr->T1Protocol.TBBytesCount++;
	}
	else{
		return READER_ERR;
	}
	
	return READER_OK;
}


READER_Status READER_ATR_ProcessTC(READER_ATR_Atr *atr, uint8_t TC, uint32_t i, uint8_t T){
	if(i == 1){
		atr->N = TC;  /* Extra guardtime */
	}
	else if(T == 0){
		atr->T0Protocol.TCBytes[atr->T0Protocol.TCBytesCount] = TC;
		atr->T0Protocol.TCBytesCount++;
	}
	else if(T == 1){
		atr->T1Protocol.TCBytes[atr->T1Protocol.TCBytesCount] = TC;
		atr->T1Protocol.TCBytesCount++;
	}
	else{
		return READER_ERR;
	}
	
	return READER_OK;
}

