#include "reader_tpdu.h"
#include "reader.h"
#include "reader_hal.h"
#include <stdint.h>



extern uint32_t globalWaitTimeMili;


READER_Status READER_TPDU_Send(READER_TPDU_Command *tpdu, uint32_t timeout){
	READER_Status retVal;
	uint8_t ACKType;
	
	/* Envoi du header TPDU */
	retVal = READER_TPDU_SendHeader(tpdu, timeout);
	if(retVal != READER_OK) return retVal;
	
	/* Si la requette TPDU ne contient pas de donnees alors on s'arrete la ... */
	if(tpdu->dataField.size == 0){
		return READER_OK;
	}
	
	/* Si la requette TPDU contient des donees ... Attente d'une reponse */
	retVal = READER_TPDU_WaitACK(tpdu->headerField.INS, &ACKType, timeout);
	if(retVal != READER_OK) return retVal;
	
	/* Envoi du champs de donnees selon le type de ACK recu */
	if(ACKType == READER_TPDU_ACK_NORMAL){
		retVal = READER_TPDU_SendDataOneshot(tpdu, timeout);
		if(retVal != READER_OK) return retVal;
	}
	else if(ACKType == READER_TPDU_ACK_XORED){
		retVal = READER_TPDU_SendDataSliced(tpdu, timeout);
		if(retVal != READER_OK) return retVal;
	}
	else{
		return READER_ERR;
	}
	
	return READER_OK;
}


READER_Status READER_TPDU_SendHeader(READER_TPDU_Command *tpdu, uint32_t timeout){
	uint8_t headerBuff[READER_TPDU_HEADER_SIZE];
	READER_Status retVal;
	
	headerBuff[0] = tpdu->headerField.CLA;
	headerBuff[1] = tpdu->headerField.INS;
	headerBuff[2] = tpdu->headerField.P1;
	headerBuff[3] = tpdu->headerField.P2;
	headerBuff[4] = tpdu->headerField.P3;
	
	retVal = READER_HAL_SendCharFrame(headerBuff, READER_TPDU_HEADER_SIZE, timeout);
	
	return retVal;
}


READER_Status READER_TPDU_SendDataOneshot(READER_TPDU_Command *tpdu, uint32_t timeout){
	READER_Status retVal;
	
	/* On envoie les donnees si il y en a ... */
	if(tpdu->dataField.size != 0){
		retVal = READER_HAL_SendCharFrame(tpdu->dataField.data, tpdu->dataField.size, timeout);
		return retVal;
	}
	
	return READER_OK;
}


READER_Status READER_TPDU_SendDataSliced(READER_TPDU_Command *tpdu, uint32_t timeout){
	uint32_t i;
	uint32_t timeoutMili;
	uint8_t procedureByte;
	READER_Status retVal;
	READER_TPDU_DataField *tpduDataField;
	
	tpduDataField = &(tpdu->dataField);
	
	/* Si il n'y a pas de donnees a envoyer alosr il n'y a rien a faire ... */
	if(tpduDataField->size == 0) return READER_OK;
	
	/* Verification des parametres */
	if(tpduDataField->size > READER_TPDU_MAX_DATA) return READER_ERR;
	if(tpduDataField->size == 0x00) return READER_ERR;
	
	/* Calcul du timeout */
	if(timeout == READER_HAL_USE_ISO_WT){
		timeoutMili = READER_HAL_USE_ISO_WT;
	}
	else{
		timeoutMili = (timeout / tpduDataField->size) + 1;
	}
	
	/* Penser a remplacer le for par un while (degeu) */
	for(i=0; i<tpduDataField->size; i++){
		retVal = READER_HAL_SendChar(tpduDataField->data[i], timeoutMili);
		if(retVal != READER_OK) return retVal;
		
		retVal = READER_TPDU_WaitProcedureByte(&procedureByte, tpdu->headerField.INS, timeoutMili);
		if(retVal != READER_OK) return retVal;
		
		if((!READER_TPDU_IsXoredACK(procedureByte, tpdu->headerField.INS)) && !READER_TPDU_IsNullByte(procedureByte)) return READER_ERR;
		
		if(READER_TPDU_IsNullByte(procedureByte)){
			/* Attendre le temps necessaire */
			READER_HAL_Delay(globalWaitTimeMili);
			
		}
	}
	
	return READER_OK;
}



READER_Status READER_TPDU_RcvSW(uint16_t *SW, uint32_t timeout){
	uint8_t rcvBuff[2];
	READER_Status retVal;
	
	retVal = READER_HAL_RcvCharFrame(rcvBuff, 2, timeout);
	if(retVal != READER_OK) return retVal;
	
	if(!READER_TPDU_IsSW1(rcvBuff[0])) return READER_ERR;
	
	*SW = rcvBuff[1] + (rcvBuff[0] * 256);
	
	return READER_OK;
}



READER_Status READER_TPDU_IsACK(uint8_t byte, uint8_t INS){
	if(byte == INS){
		return READER_OK;
	}
	else{
		return READER_NO;
	}
}


READER_Status READER_TPDU_IsXoredACK(uint8_t byte, uint8_t INS){
	if(byte == (INS ^ 0xFF)){
		return READER_OK;
	}
	else{
		return READER_NO;
	}
}


READER_Status READER_TPDU_IsNullByte(uint8_t byte){
	if(byte == 0x60){
		return READER_OK;
	}
	else{
		return READER_NO;
	}
}


READER_Status READER_TPDU_IsSW1(uint8_t byte){
	if( ((byte & 0xF0) == 0x60) || ((byte & 0xF0) == 0x90) ){
		if(byte != 0x60){
			return READER_OK;
		}
		else{
			return READER_NO;
		}
	}
	else{
		return READER_NO;
	}
}


READER_Status READER_TPDU_IsProcedureByte(uint8_t byte, uint8_t INS){
	if(READER_TPDU_IsACK(byte, INS)){
		return READER_OK;
	}
	else if(READER_TPDU_IsXoredACK(byte, INS)){
		return READER_OK;
	}
	else if(READER_TPDU_IsNullByte(byte)){
		return READER_OK;
	}
	else if(READER_TPDU_IsSW1(byte)){
		return READER_OK;
	}
	else{
		return READER_NO;
	}
}


READER_Status READER_TPDU_WaitProcedureByte(uint8_t *procedureByte, uint8_t INS, uint32_t timeout){
	uint32_t timeoutMili;
	uint8_t byte;
	READER_Status retVal;
	
	if(timeout == READER_HAL_USE_ISO_WT){
		timeoutMili = globalWaitTimeMili;
	}
	else{
		timeoutMili = timeout;
	}
	
	retVal = READER_HAL_RcvChar(&byte, timeoutMili);
	if(retVal != READER_OK){
		return retVal;
	}
	
	if(READER_TPDU_IsProcedureByte(byte, INS)){
		*procedureByte = byte;
		return READER_OK;
	}
	else{
		return READER_ERR;
	}
}


READER_Status READER_TPDU_WaitACK(uint8_t INS, uint8_t *ACKType, uint32_t timeout){
	READER_Status retVal;
	uint8_t byte;
	
	do{
		retVal = READER_HAL_RcvChar(&byte, timeout);
	} while( (retVal==READER_OK) && (READER_TPDU_IsNullByte(byte)) && !(READER_TPDU_IsACK(byte, INS)) && !(READER_TPDU_IsXoredACK(byte, INS)));
	
	if(retVal != READER_OK) return retVal;
	
	if(READER_TPDU_IsXoredACK(byte, INS)){
		*ACKType = READER_TPDU_ACK_XORED;
	}
	else{
		*ACKType = READER_TPDU_ACK_NORMAL;
	}
	
	return READER_OK;
}
