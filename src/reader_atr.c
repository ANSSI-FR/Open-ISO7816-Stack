/**
 * \file reader_atr.c
 * \copyright This file is part of the Open-ISO7816-Stack project and is distributed under the MIT license. See LICENSE file in the root directory. 
 * This file contains functions for dealing with ATR (Answer-To-Reset).
 */

#include "reader_atr.h"
#include "reader_hal.h"



/* Voir table 7 section 8.3 ISO7816_3 */
static uint16_t globalFiConvTable[0x10] = {372, 372, 558, 744, 1116, 1488, 1860, 372, 372, 512, 768, 1024, 1536, 2048, 372, 372};
static uint8_t  globalDiConvTable[0x10] = {1, 1, 2, 4, 8, 16, 32, 64, 12, 20, 1, 1, 1, 1, 1, 1};
static uint32_t globalFMaxConvTable[0x10] = {4000000, 5000000, 6000000, 8000000, 12000000, 16000000, 20000000, 0xFFFFFFFF, 0xFFFFFFFF, 5000000, 7500000, 10000000, 15000000, 20000000, 0xFFFFFFFF, 0xFFFFFFFF};



READER_Status READER_ATR_Receive(READER_ATR_Atr *atr, READER_HAL_CommSettings *pSettings){
	READER_Status retVal;
	uint32_t j, i = 1;
	uint8_t TS, T0, TA, TB, TC, TD;
	uint8_t Y, T = 0;
	uint8_t checkByte;
	uint8_t rcvdBytes[READER_ATR_MAX_SIZE];                     /* l'ATR fait au max 32 octets, voir ISO7816-3 section 8.2.1. */
	uint32_t rcvdCount = 0;
	uint8_t byte;
	
	
	/* Initialisation des certains elements de la structure ATR */
	retVal = READER_ATR_InitStruct(atr);
	if(retVal != READER_OK) return retVal;
		
	
	/* Recuperation de TS */
	retVal = READER_HAL_RcvChar(pSettings, READER_HAL_PROTOCOL_ANY, &TS, READER_ATR_DEFAULT_TIMEOUT);
	if(retVal != READER_OK) return retVal;
	retVal = READER_ATR_CheckTS(TS);
	if(retVal != READER_OK) return retVal;
	atr->encodingConv = READER_ATR_GetEncoding(TS);
	
	/* Recuperation de T0 */
	retVal = READER_HAL_RcvChar(pSettings, READER_HAL_PROTOCOL_ANY, &T0, READER_ATR_DEFAULT_TIMEOUT);
	if(retVal != READER_OK) return retVal;
	atr->K = READER_ATR_ComputeK(T0);
	retVal = READER_ATR_AddRcvdByte(T0, rcvdBytes, &rcvdCount);
	if(retVal != READER_OK) return retVal;
	
	
	Y = READER_ATR_ComputeY(T0);
	
	/* Recupertion de tous les Interfaces Bytes */
	while(READER_ATR_IsInterfacesBytesToRead(Y)){
		if(READER_ATR_IsTAToRead(Y)){
			if(READER_HAL_RcvChar(pSettings, READER_HAL_PROTOCOL_ANY, &TA, READER_ATR_DEFAULT_TIMEOUT) != READER_OK) return READER_ERR;
			if(READER_ATR_ProcessTA(atr, TA, i, T) != READER_OK) return READER_ERR;
			if(READER_ATR_AddRcvdByte(TA, rcvdBytes, &rcvdCount) != READER_OK) return READER_ERR;
		}
		if(READER_ATR_IsTBToRead(Y)){
			if(READER_HAL_RcvChar(pSettings, READER_HAL_PROTOCOL_ANY, &TB, READER_ATR_DEFAULT_TIMEOUT) != READER_OK) return READER_ERR;
			if(READER_ATR_ProcessTB(atr, TB, i, T) != READER_OK) return READER_ERR;
			if(READER_ATR_AddRcvdByte(TB, rcvdBytes, &rcvdCount) != READER_OK) return READER_ERR;
		}
		if(READER_ATR_IsTCToRead(Y)){
			if(READER_HAL_RcvChar(pSettings, READER_HAL_PROTOCOL_ANY, &TC, READER_ATR_DEFAULT_TIMEOUT) != READER_OK) return READER_ERR;
			if(READER_ATR_ProcessTC(atr, TC, i, T) != READER_OK) return READER_ERR;
			if(READER_ATR_AddRcvdByte(TC, rcvdBytes, &rcvdCount) != READER_OK) return READER_ERR;
		}
		if(READER_ATR_IsTDToRead(Y)){
			if(READER_HAL_RcvChar(pSettings, READER_HAL_PROTOCOL_ANY, &TD, READER_ATR_DEFAULT_TIMEOUT) != READER_OK) return READER_ERR;
			Y = READER_ATR_ComputeY(TD);
			T = READER_ATR_ComputeT(TD);
			READER_ATR_ProcessT(atr, T);
			if(READER_ATR_AddRcvdByte(TD, rcvdBytes, &rcvdCount) != READER_OK) return READER_ERR;
		}
		else{
			Y = 0x00;
		}
		i++;
	}
	
	/* Recuperation de tous les Historical Bytes */
	for(j=0; j<atr->K; j++){
		retVal = READER_HAL_RcvChar(pSettings, READER_HAL_PROTOCOL_ANY, &byte, READER_ATR_DEFAULT_TIMEOUT);
		if(retVal != READER_OK) return retVal;
		
		atr->histBytes[j] = byte;
		
		retVal = READER_ATR_AddRcvdByte(byte, rcvdBytes, &rcvdCount);
		if(retVal != READER_OK) return retVal;
	}
	
	
	/* Recuperation du Check Byte */
	/* La presence du check byte n'est pas systematique, voir ISO7816-3 section 8.2.5. */
	if(!(READER_ATR_IsT0(atr) && !READER_ATR_IsT15(atr))){
		retVal = READER_HAL_RcvChar(pSettings, READER_HAL_PROTOCOL_ANY, &checkByte, READER_ATR_DEFAULT_TIMEOUT);
		if(retVal != READER_OK) return retVal;	
		
		/* Verification des caracteres recus avec le TCK */
		retVal = READER_ATR_CheckTCK(rcvdBytes, rcvdCount, checkByte);	
		if(retVal != READER_OK) return retVal;
	}
	
	return READER_OK;
}


/**
 * \fn READER_Status READER_ATR_ApplySettings(READER_ATR_Atr *atr)
 * \brief Cette fonction permet d'appliquer les parametres de communication indiqués dans l'ATR.
 * \return Retourne uen code d'erreur de stype READER_Status. Retourne READER_OK si l'exécution s'est correctement déroulée. Pour toute autre valeur il s'agit d'une erreur.
 * \param *atr Pointeur sur une structure de type READER_ATR_Atr;
 */
 READER_Status READER_ATR_ApplySettings(READER_ATR_Atr *atr){
	 return READER_OK;
 }
//READER_Status READER_ATR_ApplySettings(READER_ATR_Atr *atr){
//	READER_Status retVal;
//	uint32_t Fi, Di, R, f;
//	uint32_t IFSC, BWI, BWTEtu, BWTMili, rType, WI;
//	uint32_t newGT;
//	
//	/* Application des parametres Fi et Di (qui definissent le ETU) */
//	if((atr->Fi != READER_ATR_VALUE_NOT_INDICATED) && (atr->Di != READER_ATR_VALUE_NOT_INDICATED)){
//		Fi = atr->Fi;
//		Di = atr->Di;
//	}
//	else if((atr->Fi != READER_ATR_VALUE_NOT_INDICATED) && (atr->Di == READER_ATR_VALUE_NOT_INDICATED)){
//		Fi = atr->Fi;
//		Di = READER_HAL_GetDi();
//	}
//	else if((atr->Fi == READER_ATR_VALUE_NOT_INDICATED) && (atr->Di != READER_ATR_VALUE_NOT_INDICATED)){
//		Fi = READER_HAL_GetFi();
//		Di = atr->Di;
//	}
//	else{
//		Fi = READER_HAL_GetFi();
//		Di = READER_HAL_GetDi();
//	}
//	
//	retVal = READER_HAL_SetEtu(Fi, Di);
//	if(retVal != READER_OK) return retVal;
//	
//	/* Application de WI                        */
//	WI = READER_ATR_GetWI(atr);
//	retVal = READER_HAL_SetWI(WI);
//	if(retVal != READER_OK) return retVal;
//	
//	/* Application du parametre GT (guard time) */
//	Fi = READER_HAL_GetFi();
//	Di = READER_HAL_GetDi();
//	R = Fi / Di;
//	f = READER_HAL_GetFreq(pSettings);
//	newGT = (uint32_t)((float)READER_HAL_DEFAULT_GT + (float)R*((float)atr->N / (float)f));    /* Voir ISO7816-3 section 8.3, TC1 */
//
//	retVal = READER_HAL_SetGT(newGT);                   
//	if(retVal != READER_OK) return retVal; 
//	
//	/* Application de parametres T1 */
//	IFSC = READER_ATR_GetIFSC(atr);
//	BWI = READER_ATR_GetBWI(atr);
//	BWTEtu = READER_UTILS_ComputeBWTEtu(BWI, f);
//	BWTMili = READER_UTILS_ComputeBWTMili(BWTEtu, Fi, Di, f);
//	rType = READER_ATR_GetRedundancyType(atr);
//	
//	//retVal = READER_HAL_SetIFSC(IFSC);
//	//if(retVal != READER_OK) return retVal;
//	
//	//retVal = READER_HAL_SetBWT(BWTMili);
//	//if(retVal != READER_OK) return retVal;
//	
//	retVal = READER_HAL_SetRedundancyType(rType);
//	if(retVal != READER_OK) return retVal;
// 
//	
//	return READER_OK;   
//}



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
	
	atr->isT1Indicated = READER_ATR_NOT_INDICATED;
	atr->isT0Indicated = READER_ATR_NOT_INDICATED;
	atr->isT15Indicated = READER_ATR_NOT_INDICATED;
	
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

READER_Status READER_ATR_IsT0(READER_ATR_Atr *atr){
	if(atr->isT0Indicated == READER_ATR_INDICATED){
		return READER_OK;
	}
	else{
		return READER_NO;
	}
}

READER_Status READER_ATR_IsT1(READER_ATR_Atr *atr){
	if(atr->isT1Indicated == READER_ATR_INDICATED){
		return READER_OK;
	}
	else{
		return READER_NO;
	}
}

READER_Status READER_ATR_IsT15(READER_ATR_Atr *atr){
	if(atr->isT15Indicated == READER_ATR_INDICATED){
		return READER_OK;
	}
	else{
		return READER_NO;
	}
}

/**
 * \fn READER_Status READER_ATR_ProcessT(READER_ATR_Atr *atr, uint8_t T)
 * \brief Cette fonction sert a mettre à jour la structure ATR en indiquant les protocoles (T=0 ou T=1) qui sont pris en charge par la carte.
 * \return La fonction retourne READER_OK si la fonction d'est exécutée correctement. Une autre valeur dans le cas contraire.
 * \param *atr est un pointeur vers une structure ATR. Elle sera mise à jour avec les informations du paramètre T, selon la prise en charge ou non du protocole.
 * \param T est le champs "T" des historical bytes TD. Voir ISO7816-3 section 8.2.3.
 */
READER_Status READER_ATR_ProcessT(READER_ATR_Atr *atr, uint8_t T){
	if(T==0){
		atr->isT0Indicated = READER_ATR_INDICATED;
	}
	
	if(T==1){
		atr->isT1Indicated = READER_ATR_INDICATED;
	}
	
	if(T==15){
		atr->isT15Indicated = READER_ATR_INDICATED;
	}
	
	return READER_OK;
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

/**
 * \fn READER_Status READER_ATR_CheckTCK(uint8_t *rcvdBytesList, uint32_t rcvdBytesCount, uint8_t TCK)
 * \brief Cette fonction permet d'effectuer le test d'intégrité des données reçues dans l'ATR. Il s'agit du test décrit dans l'ISO7816-3 section 8.2.5.
 * \param *rcvdBytesList est un pointeur sur un buffer qui contient tous les caractères qui ont été reçus dans l'ATR (à partir de T0).
 * \param rcvdBytesCount indique le nombre de caractères qui ont été reçus dans l'ATR.
 * \param TCK est le check byte reçu en fin d'ATR.
 * \return Valeur de retour de type READER_Status. READER_OK indique le bon déroulement de la fonction. Toute autre valeur indique une erreur.
 */
READER_Status READER_ATR_CheckTCK(uint8_t *rcvdBytesList, uint32_t rcvdBytesCount, uint8_t TCK){
	uint32_t i;
	uint8_t totalXor;
	
	totalXor = TCK;
	
	for(i=0; i<rcvdBytesCount; i++){
		totalXor = rcvdBytesList[i] ^ totalXor;
	}
	
	/* Voir ISO7816-3 section 8.2.5 pour explication du test */
	if(totalXor == 0x00){
		return READER_OK;
	}
	else{
		return READER_ERR;
	}
}

/**
 * \fn READER_Status READER_ATR_AddRcvdByte(uint8_t byte, uint8_t *byteList, uint32_t *byteCount)
 * \brief Cette fonction est utile en interne de la fonction READER_ATR_Receive(). Elle sert a tenir à jour une liste de tous les octets recus pendant l'ATR. Cette liste d'octets est utile pour le calcul du TCK.
 *        La fonction prend en paramètre l'octet reçu, l'ajoute à la liste des octets reçus et incrémente un compteur.
 * \param byte est l'octet reçu, que 'on veut ajouter à la liste de tous les octets reçus.
 * \param *byteList est un pointeur sur la liste des octets recus pendant l'ATR. On veut ajouter l'octet à cette liste.
 * \param *byteCount est un pointeur sur le compteur qui compte le nombre d'octets reçus durant l'ATR. Cette fonction mets à jour ce compteur, elle l'incrémente de 1.
 * \return Valeur de retour de type READER_Status. READER_OK indique le bon déroulement de la fonction. Toute autre valeur indique une erreur.
 */
READER_Status READER_ATR_AddRcvdByte(uint8_t byte, uint8_t *byteList, uint32_t *byteCount){
	if(*byteCount >= READER_ATR_MAX_SIZE){
		return READER_ERR;
	}
	
	byteList[*byteCount] = byte;
	(*byteCount)++;
	
	return READER_OK;
}
