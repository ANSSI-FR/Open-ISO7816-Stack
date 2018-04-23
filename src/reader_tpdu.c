#include "reader_tpdu.h"
#include "reader.h"
#include "reader_hal.h"
#include <stdint.h>


extern UART_HandleTypeDef uartHandleStruct;   // juste pour debug
extern SMARTCARD_HandleTypeDef smartcardHandleStruct; // debug
extern uint32_t globalWaitTimeMili;




READER_Status READER_TPDU_Execute(READER_TPDU_Command *pTpdu, READER_TPDU_Response *pResp, uint32_t timeout){
	READER_Status retVal;
	
	retVal = READER_TPDU_Send(pTpdu, timeout);
	if(retVal != READER_OK) return retVal;
	
	retVal = READER_TPDU_RcvResponse(pResp, timeout);
	if(retVal != READER_OK) return retVal;
	
	return READER_OK;
}



READER_Status READER_TPDU_Send(READER_TPDU_Command *tpdu, uint32_t timeout){
	READER_Status retVal;
	uint8_t ACKType;
	uint32_t i;
	
	/* Envoi du header TPDU */
	retVal = READER_TPDU_SendHeader(tpdu, timeout);
	if(retVal != READER_OK) return retVal;
	
	
	/* Si la requette TPDU ne contient pas de donnees alors on s'arrete la ... */
	if(tpdu->dataField.size == 0){
		return READER_OK;
	}
	
	/* Si la requette TPDU contient des donnees ... Attente d'une reponse */
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


/**
 * \fn READER_Status READER_TPDU_SendHeader(READER_TPDU_Command *tpdu, uint32_t timeout)
 * \brief Cette fonction permet d'envoyer sur la ligne IO le header de la commande TPDU
 * \return Valeur de type READER_Status. READER_OK si l'exécution s'est correctement déroulée. READER_ERR dans le cas contraire.
 * \param *tpdu Pointeur vers une structure de type READER_TPDU_Command.
 * \param timeout !!!!
 */
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


/**
 * \fn READER_Status READER_TPDU_RcvSW(uint16_t *SW, uint32_t timeout)
 * \brief Cette fonction permet d'attendre la réception d'un status word (SW).
 * \return Valeur de type READER_Status. READER_OK si l'exécution s'est correctement déroulée. READER_ERR dans le cas contraire.
 * \param timeout Valeur du timeout en milisecondes pour recevoir un octet parmi les deux du SW. A la réception d'un null byte le compteur repart à zéro. Lorsque SW1 est reçu, le compteur repart également à zéro pour la réception de SW2.
 * \param *SW1 Pointeur une uen variable dans laquelle stocker la première partie du Status Word (SW1).
 * \param *SW2 Pointeur une uen variable dans laquelle stocker la deuxième partie du Status Word (SW2).
 */
READER_Status READER_TPDU_RcvSW(uint8_t *SW1, uint8_t *SW2, uint32_t timeout){	
	READER_Status retVal;
	uint8_t byte1, byte2;
	
	/* On attend SW1 en prenant en compte les null bytes ... */
	do{
		retVal = READER_HAL_RcvChar(&byte1, timeout);
	} while( (retVal==READER_OK) && (READER_TPDU_IsNullByte(byte1)) && !(READER_TPDU_IsSW1(byte1)) );
	
	if(retVal != READER_OK) return retVal;
	
	/* On recupere SW2 */
	retVal = READER_HAL_RcvChar(&byte2, timeout);
	if(retVal != READER_OK) return retVal;
	
	/* On retourne le resultat */
	*SW1 = *byte1;
	*SW2 = *byte2;
	
	return READER_OK;
}



/**
 * \fn READER_Status READER_TPDU_RcvDataField(uint8_t *buffer, uint32_t Ne)
 * \brief Cette fonction permet de recevoir la partie data d'une TPDU Response.
 * \return Valeur de type READER_Status. READER_OK si l'exécution s'est correctement déroulée. READER_ERR dans le cas contraire.
 * \param *buffer Pointeur sur un buffer dans lequel stocker les données reçues.
 * \param Ne (N expected) Nombre d'octets attendus dans la réponse.
 * \param timeout Pour l'instant non décidé si c'est le timeout pour chaque carac ou pour toute le frame. !!!!!!!!
 */
READER_Status READER_TPDU_RcvDataField(uint8_t *buffer, uint32_t Ne, uint32_t timeout){
	READER_Status retVal;
	
	if(Ne != 0){
		retVal = READER_HAL_RcvCharFrame(buffer, Ne, timeout);
	}
	else{
		retVal = READER_OK;
	}
	
	return retVal;
}



/**
 * \fn READER_Status READER_TPDU_RcvResponse(uint8_t *dataField, uint32_t Ne, uint16_t SW, uint32_t timeout)
 * \brief Cette fonction permet de recevoir la réponse à une commande TPDU. Elle permet de récupérer le champs de données et le Status Word (SW).
 * \return Valeur de type READER_Status. READER_OK si l'exécution s'est correctement déroulée. READER_ERR dans le cas contraire.
 * \param *pResp Un pointeur sur une structure de type READER_TPDU_Response. Toutes les données de la réponse (SW1SW2, data field) seront stockées dans cette structure.
 */
READER_Status READER_TPDU_RcvResponse(READER_TPDU_Response *pResp, uint32_t timeout){
	READER_Status retVal;
	uint8_t rcvByte;
	uint32_t rcvCount = 0;
	uint8_t tmpRcvBuff[258];  /* (taille max data TPDU reponse = 256) + SW1 + SW2 */
	uint32_t i;
	

	while( ((retVal = READER_HAL_RcvChar(&rcvByte, timeout))  == READER_OK) && (rcvCount < 258)){
		tmpRcvBuff[rcvCount] = rcvByte;
		rcvCount++;
	}
	
	/* Si on est sorti pour une raison autre qu'un timeout */
	if(retVal != READER_TIMEOUT){
		return READER_ERR;
	}
	
	/* Si on a recu plus de donnees que la normale */
	if(rcvCount > 258){
		return READER_ERR;
	}
	
	/* Si on a meme pas recu un status word (SW) complet */
	if(rcvCount < 2){
		return READER_ERR;
	}
	
	/* Si on est ici c'est que on s'est arrete de recevoir des caractères a cause d'un TIMEOUT. cad plus rien a recevoir */
	/* Donc c'est la fin de la reponse TPDU */
	/* Les deux derniers octets du buffer de response dervaient etre SW1 et SW2 */
	
	/* Si le caractere cense etre un SW1 n'en est pas un ... */
	if(!READER_TPDU_IsSW1(tmpRcvBuff[rcvCount-2])){
		return READER_ERR;
	}
	
	/* On complete la structure de reponse TPDU */
	pResp->SW1 = tmpRcvBuff[rcvCount-2];
	pResp->SW2 = tmpRcvBuff[rcvCount-1];
	pResp->dataSize = rcvCount-2;
	for(i=0; i<rcvCount-2; i++){
		pResp->dataBytes[i] = tmpRcvBuff[i];
	}
	
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
	uint8_t byte;
	READER_Status retVal;
	
	
	retVal = READER_HAL_RcvChar(&byte, timeout);
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

/**
 * \fn READER_Status READER_TPDU_WaitACK(uint8_t INS, uint8_t *ACKType, uint32_t timeout)
 * \brief Cette fonction permet d'attendre la réception d'un ACK en provenance de la carte.
 * \return Valeur de type READER_Status. READER_OK si l'exécution s'est correctement déroulée. READER_ERR dans le cas contraire.
 * \param INS Code de l'instruction précédement envoyée à la carte. C'est l'instruction pour laquelle l'ACK est attendu.
 * \param *ACKType Pointeur sur une variable de type uint8_t. Si un ACK est reçu dans les temps alors la fonction écrira dans cette variable le type d'ACK reçu. Il peut être de deux types : READER_TPDU_ACK_NORMAL ou READER_TPDU_ACK_XORED. Voir ISO7816-E section 10.3.3.
 * \param timeout Valeur de timeout en milisecondes pour recevoir l'ACK.
 */
READER_Status READER_TPDU_WaitACK(uint8_t INS, uint8_t *ACKType, uint32_t timeout){
	READER_Status retVal;
	uint8_t byte;
	
	do{
		retVal = READER_HAL_RcvChar(&byte, timeout);
	} while( (retVal==READER_OK) && (READER_TPDU_IsNullByte(byte)) && !(READER_TPDU_IsACK(byte, INS)) && !(READER_TPDU_IsXoredACK(byte, INS)));
	
	if(retVal != READER_OK) return retVal;
	
	if(READER_TPDU_IsXoredACK(byte, INS)){
		*ACKType = READER_TPDU_ACK_XORED;
		return READER_OK;
	}
	else if(READER_TPDU_IsACK(byte, INS)){
		*ACKType = READER_TPDU_ACK_NORMAL;
		return READER_OK;
	}
	else{
		return READER_ERR;
	}
}

READER_Status READER_TPDU_Forge(READER_TPDU_Command *tpdu, uint8_t CLA, uint8_t INS, uint8_t P1, uint8_t P2, uint8_t P3, uint8_t *dataBuff, uint8_t dataSize){
	uint8_t i;
	
	tpdu->headerField.CLA = CLA;
	tpdu->headerField.INS = INS;
	tpdu->headerField.P1  = P1;
	tpdu->headerField.P2  = P2;
	tpdu->headerField.P3  = P3;
	
	tpdu->dataField.size = dataSize;
	
	for(i=0; i<dataSize; i++){
		tpdu->dataField.data[i] = dataBuff[i];
	}
	
	return READER_OK;
}
