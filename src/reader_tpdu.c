#include "reader_tpdu.h"
#include "reader.h"
#include "reader_hal.h"
#include <stdint.h>


#ifdef TEST
#include <stdio.h>
#endif



extern uint32_t globalWaitTimeMili;




READER_Status READER_TPDU_Execute(READER_TPDU_Command *pTpdu, READER_TPDU_Response *pResp, uint32_t timeout, READER_HAL_CommSettings *pSettings){
	READER_Status retVal;
	
	retVal = READER_TPDU_Send(pTpdu, timeout, pSettings);
	if(retVal != READER_OK) return retVal;
	
	retVal = READER_TPDU_RcvResponse(pResp, 256, timeout, pSettings);
	if(retVal != READER_OK) return retVal;
	
	return READER_OK;
}


/**
 * \fn READER_Status READER_TPDU_Send(READER_TPDU_Command *tpdu, uint32_t timeout)
 * \brief Cette fonction permet d'envoyer une commande TPDU. Cependant elle ne sert pas a attendre la réponse. La fonction envoie le header, attend l'ACK, puis envoie le champs de données (si il y en a un).
 * \return Valeur de type READER_Status. READER_OK si l'exécution s'est correctement déroulée. Toute autre valeur suggère une erreur.
 * \param *tpdu Pointeur sur une structure de type READER_TPDU_Command. Cette structure contient le TPDU complet (header et champ de données).
 * \param timeout Valeur du timeout à appliquer lors de l'envoi de chacun des caractères. Indiquer la valeur READER_HAL_USE_ISO_WT pour utiliser le Wait Time (WT) tel que défini dans la norme ISO en guise de timeout. Indiquer toute autre valeur différente de READER_HAL_USE_ISO_WT pour un timeout personalisé (en milisecondes).
 */
READER_Status READER_TPDU_Send(READER_TPDU_Command *tpdu, uint32_t timeout, READER_HAL_CommSettings *pSettings){
	READER_Status retVal;
	uint8_t ACKType;
	
	/* Envoi du header TPDU */
	retVal = READER_TPDU_SendHeader(tpdu, timeout, pSettings);
	if(retVal != READER_OK) return retVal;
	
	
	/* Attente d'une reponse ACK ... */
	retVal = READER_TPDU_WaitACK(tpdu->headerField.INS, &ACKType, timeout, pSettings);
	if(retVal != READER_OK) return retVal;
	
	
	/* Si la requette TPDU ne contient pas de donnees alors on s'arrete la ... */
	if(tpdu->dataField.size == 0){
		return READER_OK;
	}
	
	if(tpdu->dataField.size > READER_TPDU_MAX_DATA){
		return READER_ERR;
	}
	
	
	/* Le la requette TPDU contient des donnees alors envoi du champs de donnees selon le type de ACK recu */
	if(ACKType == READER_TPDU_ACK_NORMAL){
		retVal = READER_TPDU_SendDataOneshot(tpdu, timeout, pSettings);
		if(retVal != READER_OK) return retVal;
	}
	else if(ACKType == READER_TPDU_ACK_XORED){
		retVal = READER_TPDU_SendDataSliced(tpdu, timeout, pSettings);
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
 * \return Valeur de type READER_Status. READER_OK si l'exécution s'est correctement déroulée. Toute autre valeur suggère une erreur.
 * \param *tpdu Pointeur vers une structure de type READER_TPDU_Command. Cette structure contient le TPDU complet (header et champ de données). La fonction utilisera ce header pour l'envoyer.
 * \param timeout Valeur du timeout à appliquer lors de l'envoi de chacun des caractères du champ de données. Indiquer la valeur READER_HAL_USE_ISO_WT pour utiliser le Wait Time (WT) tel que défini dans la norme ISO en guise de timeout. Indiquer toute autre valeur différente de READER_HAL_USE_ISO_WT pour un timeout personalisé (en milisecondes).
 */
READER_Status READER_TPDU_SendHeader(READER_TPDU_Command *tpdu, uint32_t timeout, READER_HAL_CommSettings *pSettings){
	uint8_t headerBuff[READER_TPDU_HEADER_SIZE];
	READER_Status retVal;
	
	headerBuff[0] = tpdu->headerField.CLA;
	headerBuff[1] = tpdu->headerField.INS;
	headerBuff[2] = tpdu->headerField.P1;
	headerBuff[3] = tpdu->headerField.P2;
	headerBuff[4] = tpdu->headerField.P3;
	
	retVal = READER_HAL_SendCharFrame(pSettings, READER_HAL_PROTOCOL_T0, headerBuff, READER_TPDU_HEADER_SIZE, timeout);
	if(retVal != READER_OK) return retVal;
	
	return READER_OK;
}


/**
 * \fn READER_Status READER_TPDU_SendDataOneshot(READER_TPDU_Command *tpdu, uint32_t timeout)
 * \brief Cette fonction permet d'envoyer tout le contenu du champs de données d'une commande TPDU. Cette fonction envoie les données d'une seule traite. Elle n'attend pas de confiramtion (ACK ou Xored ACK) de la carte après chaque caractère. Voir ISO7816-3 section 10.3.3.
 * \return Valeur de type READER_Status. READER_OK si l'exécution s'est correctement déroulée. Toute autre valeur suggère une erreur.
 * \param *tpdu Pointeur sur une structure de type READER_TPDU_Command. Cette structure contient le TPDU complet (header et champ de données). La fonction utilisera ce champ de données pour l'envoyer.
 * \param timeout Valeur du timeout à appliquer lors de l'envoi de chacun des caractères du champ de données. Indiquer la valeur READER_HAL_USE_ISO_WT pour utiliser le Wait Time (WT) tel que défini dans la norme ISO en guise de timeout. Indiquer toute autre valeur différente de READER_HAL_USE_ISO_WT pour un timeout personalisé (en milisecondes).
 */
READER_Status READER_TPDU_SendDataOneshot(READER_TPDU_Command *tpdu, uint32_t timeout, READER_HAL_CommSettings *pSettings){
	READER_Status retVal;
	
	/* On envoie les donnees si il y en a ... */
	if(tpdu->dataField.size == 0){
		return READER_OK;
	}
	
	if(tpdu->dataField.size > READER_TPDU_MAX_DATA){
		return READER_ERR;
	}
	
	retVal = READER_HAL_SendCharFrame(pSettings, READER_HAL_PROTOCOL_T0, tpdu->dataField.data, tpdu->dataField.size, timeout);
	if(retVal != READER_OK) return retVal;
	
	return READER_OK;
}


READER_Status READER_TPDU_SendDataSliced(READER_TPDU_Command *tpdu, uint32_t timeout, READER_HAL_CommSettings *pSettings){
	uint32_t i = 0;
	uint8_t ACKType;
	READER_Status retVal;
	READER_TPDU_DataField *tpduDataField;
	
	
	/* Simplification d'ecriture */
	tpduDataField = &(tpdu->dataField);
	
	/* Si il n'y a pas de donnees a envoyer alors il n'y a rien a faire ... */
	if(tpduDataField->size == 0) return READER_OK;
	
	/* Verification des parametres */
	if(tpduDataField->size > READER_TPDU_MAX_DATA) return READER_ERR;
	

	/* On envoie les caracteres un par un tantque il en reste et tantque la carte ne demande pas de tout envoyer d'un seul coups */
	do{
		retVal = READER_HAL_SendChar(pSettings, READER_HAL_PROTOCOL_T0, tpduDataField->data[i], timeout);
		if(retVal != READER_OK) return retVal;
		
		retVal = READER_TPDU_WaitACK(tpdu->headerField.INS, &ACKType, timeout, pSettings);
		if(retVal != READER_OK) return retVal;
		
		i++;
	}while((ACKType == READER_TPDU_ACK_XORED) && (i<tpduDataField->size));
	
	
	/* Si on est sortis de la boucle parceque toutes les donnes on ete envoyes alors on termine */
	if(i == tpduDataField->size){
		return READER_OK;
	}
	else{
		/* On envoie le reste en oneshot */
		retVal = READER_HAL_SendCharFrame(pSettings, READER_HAL_PROTOCOL_T0, tpduDataField->data+i, tpduDataField->size-i, timeout);
		if(retVal != READER_OK) return retVal;
		
		return READER_OK;
	}
}


/**
 * \fn READER_Status READER_TPDU_RcvSW(uint16_t *SW, uint32_t timeout)
 * \brief Cette fonction permet d'attendre la réception d'un status word (SW).
 * \return Valeur de type READER_Status. READER_OK si l'exécution s'est correctement déroulée. Toute autre valeur suggère une erreur.
 * \param timeout Valeur du timeout en milisecondes pour recevoir un octet parmi les deux du SW. A la réception d'un null byte le compteur repart à zéro. Lorsque SW1 est reçu, le compteur repart également à zéro pour la réception de SW2.
 * \param *SW1 Pointeur sur une variable dans laquelle stocker la première partie du Status Word (SW1).
 * \param *SW2 Pointeur sur une variable dans laquelle stocker la deuxième partie du Status Word (SW2).
 */
READER_Status READER_TPDU_RcvSW(uint8_t *SW1, uint8_t *SW2, uint32_t timeout, READER_HAL_CommSettings *pSettings){	
	READER_Status retVal;
	uint8_t byte1, byte2;
	
	/* On attend SW1 en prenant en compte les null bytes ... */
	do{
		retVal = READER_HAL_RcvChar(pSettings, READER_HAL_PROTOCOL_T0, &byte1, timeout);
		
		if((retVal == READER_OK) && (READER_TPDU_IsNullByte(byte1) == READER_NO) &&  (READER_TPDU_IsSW1(byte1) == READER_NO)){
			return READER_INCORRECT_SW1;
		}
		
	} while( (retVal==READER_OK) && (READER_TPDU_IsNullByte(byte1) == READER_OK) );
	
	if(retVal == READER_TIMEOUT){
		return READER_TIMEOUT_ON_SW1;
	}
	if(retVal != READER_OK) return retVal;
	
	/* On retourne tout de suite le resultat pour SW1 ...  */
	*SW1 = byte1;
	
	/* On recupere SW2 */
	retVal = READER_HAL_RcvChar(pSettings, READER_HAL_PROTOCOL_T0, &byte2, timeout);
	
	if(retVal == READER_TIMEOUT){
		return READER_TIMEOUT_ON_SW2;
	}
	if(retVal != READER_OK) return retVal;
	
	/* On retourne le resultat pour SW2 ...  */
	*SW2 = byte2;
	
	return READER_OK;
}



/**
 * \fn READER_Status READER_TPDU_RcvDataField(uint8_t *buffer, uint32_t Ne)
 * \brief Cette fonction permet de recevoir la partie data d'une TPDU Response.
 * \return Valeur de type READER_Status. READER_OK si l'exécution s'est correctement déroulée. Toute autre valeur suggère une erreur.
 * \param *buffer Pointeur sur un buffer dans lequel stocker les données reçues.
 * \param Ne (N expected) Nombre d'octets attendus dans la réponse.
 * \param timeout Pour l'instant non décidé si c'est le timeout pour chaque carac ou pour toute le frame. !!!!!!!!
 */
READER_Status READER_TPDU_RcvDataField(uint8_t *buffer, uint32_t Ne, uint32_t timeout, READER_HAL_CommSettings *pSettings){
	READER_Status retVal;
	
	if(Ne != 0){
		retVal = READER_HAL_RcvCharFrame(pSettings, READER_HAL_PROTOCOL_T0, buffer, Ne, timeout);
	}
	else{
		retVal = READER_OK;
	}
	
	return retVal;
}



/**
 * \fn READER_Status READER_TPDU_RcvResponse(uint8_t *dataField, uint32_t Ne, uint16_t SW, uint32_t timeout)
 * \brief Cette fonction permet de recevoir la réponse à une commande TPDU. Elle permet de récupérer le champs de données et le Status Word (SW).
 *        Si il y a timeout et qu'on a reçu deux octets de données alors on considere que les deux octets reçus sont SW1 et SW2. Ils seront placés dans les champs SW1 et SW2 de la structure TPDU response.
 * \return Valeur de type READER_Status. READER_OK si l'exécution s'est correctement déroulée. Toute autre valeur suggère une erreur.
 * \param *pResp Un pointeur sur une structure de type READER_TPDU_Response. Toutes les données de la réponse (SW1SW2, data field) seront stockées dans cette structure.
 * \param expectedDataSize Nombre de caractères attendus en réponse.
 * \param timeout Valeur en milisecondes du timeout à appliquer pour la réception de chacun des caractères de la réponse. Indiquer la valeur READER_HAL_USE_ISO_WT pour utiliser le Wait Time (WT) tel que défini dans la norme ISO en guise de timeout. Indiquer toute autre valeur différente de READER_HAL_USE_ISO_WT pour un timeout personalisé (en milisecondes).
 */
READER_Status READER_TPDU_RcvResponse(READER_TPDU_Response *pResp, uint32_t expectedDataSize, uint32_t timeout, READER_HAL_CommSettings *pSettings){
	READER_Status retVal;
	uint32_t rcvdCount;
	uint8_t rcvdSW1, rcvdSW2;
	
	/* Une TPDU Response ne peut pas contenir plus de 256 caracteres. */
	if(expectedDataSize > READER_TPDU_MAX_DATA){
		return READER_OVERFLOW;
	}
	
	/* On recupere les donnees */
	if(expectedDataSize != 0){
		retVal = READER_HAL_RcvCharFrameCount(pSettings, READER_HAL_PROTOCOL_T0, pResp->dataBytes, expectedDataSize, &rcvdCount, timeout);
		pResp->dataSize = rcvdCount;
		
		if((retVal == READER_TIMEOUT) && ((pResp->dataSize) >= 2)){
			/* On a probablement recu que le SW1SW2 et pas de data */
			
			pResp->SW1 = pResp->dataBytes[(pResp->dataSize)-2];
			pResp->SW2 = pResp->dataBytes[(pResp->dataSize)-1];
			pResp->dataSize = (pResp->dataSize)-2;
			
			if(READER_TPDU_IsSW1(pResp->SW1) != READER_OK){
				return READER_TIMEOUT;
			}
			
			return READER_TIMEOUT_GOT_ONLY_SW;
		}
		else if((retVal == READER_TIMEOUT) && ((pResp->dataSize) < 2)){
			return READER_TIMEOUT;
		}
		else if((retVal != READER_OK) && (retVal != READER_TIMEOUT)){
			return retVal;
		}
	}
	else{
		pResp->dataSize = 0;
	}
	
	/* On recupere le Status Word (SW) */
	retVal = READER_TPDU_RcvSW(&rcvdSW1, &rcvdSW2, timeout, pSettings);
	pResp->SW1 = rcvdSW1;
	pResp->SW2 = rcvdSW2;
	
	if((retVal == READER_TIMEOUT_ON_SW1)){
		/* Si on n'arrive pas a recevoir de SW, ca veut dire que la carte a tres probablement envoyer l'integralite des donnees ... */
		if((pResp->dataSize) < 2){
			return READER_TIMEOUT;
		}
		
		pResp->SW1 = pResp->dataBytes[(pResp->dataSize)-2];
		pResp->SW2 = pResp->dataBytes[(pResp->dataSize)-1];
		pResp->dataSize = (pResp->dataSize)-2;
		
		if(READER_TPDU_IsSW1(pResp->SW1) != READER_OK){
			return READER_TIMEOUT;
		}
		return READER_TIMEOUT_GOT_ONLY_SW;
	}
	else if((retVal == READER_TIMEOUT_ON_SW2)){
		
		if((pResp->dataSize) < 1){
			return READER_TIMEOUT;
		}
		
		pResp->SW1 = pResp->dataBytes[(pResp->dataSize)-1];
		pResp->SW2 = rcvdSW1;
		pResp->dataSize = (pResp->dataSize)-1;
		
		if(READER_TPDU_IsSW1(pResp->SW1) != READER_OK){
			return READER_TIMEOUT;
		}
		return READER_TIMEOUT_GOT_ONLY_SW;
	}
	else if(retVal != READER_OK){
		return retVal;
	}
	
	/* Si le caractere cense etre un SW1 n'en est pas un ... */
	if(READER_TPDU_IsSW1(pResp->SW1) != READER_OK){
		return READER_ERR;
	}
	
	
	return READER_OK;
}



READER_Status READER_TPDU_IsACK(uint8_t byte, uint8_t INS){
	/* Voir ISO7816-3 section 10.3.3 ...  */
	if((byte & 0xF0) == 0x90){
		return READER_NO;
	}
	
	if((byte & 0xF0) == 0x60){
		return READER_NO;
	}
	
	if(byte == INS){
		return READER_OK;
	}
	else{
		return READER_NO;
	}
}


READER_Status READER_TPDU_IsXoredACK(uint8_t byte, uint8_t INS){
	/* Voir ISO7816-3 section 10.3.3 ...  */
	if((byte & 0xF0) == 0x90){
		return READER_NO;
	}
	
	if((byte & 0xF0) == 0x60){
		return READER_NO;
	}
	
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


/**
 * \fn READER_Status READER_TPDU_IsProcedureByte(uint8_t byte, uint8_t INS)
 * \brief Cette fonction permet de déterminer si un caractère est un "Procedure Byte". La notion de "Procedure Byte" est définie dans la norme ISO7816-3 à la section 10.3.3.
 * \return Retourne READER_OK si il s'agit d'un "Procedure Byte" et READER_NO dans le cas contraire.
 * \param byte Il s'agit du caractère à tester.
 * \param INS Il s'agit du code de l'instruction (dans le cas ou on veut tester si il s'agit d'un ACK).
 */
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


READER_Status READER_TPDU_WaitProcedureByte(uint8_t *procedureByte, uint8_t INS, uint32_t timeout, READER_HAL_CommSettings *pSettings){
	uint8_t byte;
	READER_Status retVal;
	
	
	retVal = READER_HAL_RcvChar(pSettings, READER_HAL_PROTOCOL_T0, &byte, timeout);
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
 * \return Valeur de type READER_Status. READER_OK si l'exécution s'est correctement déroulée. Toute autre valeur suggère une erreur.
 * \param INS Code de l'instruction précédement envoyée à la carte. C'est l'instruction pour laquelle l'ACK est attendu (La valeur de l'ACK est liée à l'instruction précédente. Voir ISO7816-3 section 10.3.3).
 * \param *ACKType Pointeur sur une variable de type uint8_t. Si un ACK est reçu sans erreur alors la fonction écrira dans cette variable le type d'ACK reçu. Il peut être de deux types : READER_TPDU_ACK_NORMAL ou READER_TPDU_ACK_XORED. Voir ISO7816-E section 10.3.3.
 * \param timeout Valeur de timeout en milisecondes pour recevoir l'ACK. Indiquer la valeur READER_HAL_USE_ISO_WT pour utiliser le Wait Time (WT) tel que défini dans la norme ISO en guise de timeout. Indiquer toute autre valeur différente de READER_HAL_USE_ISO_WT pour un timeout personalisé (en milisecondes).
 */
READER_Status READER_TPDU_WaitACK(uint8_t INS, uint8_t *ACKType, uint32_t timeout, READER_HAL_CommSettings *pSettings){
	READER_Status retVal;
	uint8_t byte;
	
	do{
		retVal = READER_HAL_RcvChar(pSettings, READER_HAL_PROTOCOL_T0, &byte, timeout);
		//HAL_UART_Transmit_IT(&uartHandleStruct, &byte, 0x01);  // DEBUG !!!
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


/**
 * \fn READER_Status READER_TPDU_Forge(READER_TPDU_Command *tpdu, uint8_t CLA, uint8_t INS, uint8_t P1, uint8_t P2, uint8_t P3, uint8_t *dataBuff, uint8_t dataSize)
 * \brief Cette fonction permet d'initialiser un structure de type READER_TPDU_Command à partir des informations fournies en paramètres.
 * \return Valeur de retour de type READER_Status. READER_OK indique le bon déroulement de la fonction. Toute autre valeur indique une erreur.
 * \param *pApduCmd est un pointeur sur la structure de type READER_TPDU_Command à initialiser.
 * \param CLA est la classe de l'instruction telle que définie dans la norme ISO7816.
 * \param INS est le code de l'instruction telle que définie dans la norme ISO7816.
 * \param P1 est le premier paramètre de l'instruction tel que défini dans la norme ISO7816.
 * \param P2 est le deuxième paramètre de l'instruction tel que défini dans la norme ISO7816.
 * \param P3 est le troisième paramètre de l'instruction tel que défini dans la norme ISO7816.
 * \param *dataBuff est un pointeur vers un tableau de caractères. Celui-ci contient les caractères du champ de données de la commande.
 * \param dataSize est le nombre de caractères contenus dans le champs de données de la commande.
 */
READER_Status READER_TPDU_Forge(READER_TPDU_Command *tpdu, uint8_t CLA, uint8_t INS, uint8_t P1, uint8_t P2, uint8_t P3, uint8_t *dataBuff, uint32_t dataSize){
	uint32_t i;
	
	
	if(dataSize > READER_TPDU_MAX_DATA){
		return READER_ERR;
	}
	
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
