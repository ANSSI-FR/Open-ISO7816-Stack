#include "reader_tpdu.h"
#include "reader_hal.h"



/**
 * \fn READER_APDU_ProtocolCase READER_APDU_GetProtocolCase(READER_APDU_Command *pApdu)
 * \brief Cette fonction analyse une structure de type READER_APDU_Command et en déduit dans quel "sous cas" du protocole cet APDU appartient. Par exemple 2E, 2S, 3E, 3S, 4E, 4S etc ...
 * \return Retourne une valeur de type READER_APDU_ProtocolCase.
 * \param *pApdu Un pointeur sur un structure de type READER_APDU_Command.
 */
READER_APDU_ProtocolCase READER_APDU_GetProtocolCase(READER_APDU_Command *pApdu){
	uint32_t Nc, Ne;
	
	Nc = pApdu->body.Nc;
	Ne = pApdu->body.Ne;
	
	if((Nc==0) && (Ne==0)){
		return READER_APDU_CASE_1;
	}
	else if((Nc==0) && (Ne!=0)){
		if((Ne>0) && (Ne<=256)){
			return READER_APDU_CASE_2S;
		}
		else if(Ne <= 65536){
			return READER_APDU_CASE_2E;
		}
		else{
			return READER_ERR;
		}
	}
	else if((Nc!=0) && (Ne==0)){
		if((Nc>0) && (Nc<=255)){
			return READER_APDU_CASE_3S;
		}
		else if(Nc <= 65535){
			return READER_APDU_CASE_3E;
		}
		else{
			return READER_APDU_CASE_ERR;
		}
	}
	else((Nc!=0) && (Ne!=0)){
		if((Nc <= 255) && (Ne <= 256)){
			return READER_APDU_CASE_4S;
		}
		else if((Nc <= 65535) && (Ne <= 65536)){
			return READER_APDU_CASE_4E;
		}
		else{
			return READER_APDU_CASE_ERR;
		}
	}
}


/**
 * \fn READER_Status READER_APDU_Send(READER_APDU_Command *pApdu)
 * \brief Cette fonction permet d'envoyer un commande APDU. Elle analyse l'APDU en paramètre pour en déduire le "sous cas" du protocole (3E, 3S, 4E, etc ...). Elle appelle alors une autre fonction READER_APDU_SendCase[..]() selon le cas.
 * \return Valeur de type READER_Status. READER_OK si l'exécution s'est correctement déroulée. READER_ERR dans le cas contraire.
 * \param \param *pApdu Un pointeur sur un structure de type READER_APDU_Command.
 */
READER_Status READER_APDU_Execute(READER_APDU_Command *pApdu, READER_APDU_Response *pResp){
	READER_APDU_ProtocolCase protocolCase;
	
	protocolCase = READER_APDU_GetProtocolCase(pApdu);
	
	switch(protocolCase){
		case READER_APDU_CASE_1:
			return READER_APDU_ExecuteCase1(pApdu, pResp);
			break;
			
		case READER_APDU_CASE_2E:
			return READER_APDU_ExecuteCase2E(pApdu, pResp);
			break;
			
		case READER_APDU_CASE_2S:
			return READER_APDU_ExecuteCase2S(pApdu, pResp);
			break;
			
		case READER_APDU_CASE_3E:
			return READER_APDU_ExecuteCase3E(pApdu, pResp);
			break;
			
		case READER_APDU_CASE_3S:
			return READER_APDU_ExecuteCase3S(pApdu, pResp);
			break;
			
		case READER_APDU_CASE_4E:
			return READER_APDU_ExecuteCase4E(pApdu, pResp);
			break;
			
		case READER_APDU_CASE_4S:
			return READER_APDU_ExecuteCase4S(pApdu, pResp);
			break;
			
		case READER_APDU_CASE_ERR:
			return READER_ERR;
			break;
			
		default:
			return READER_ERR;
	}
}



READER_Status READER_APDU_ExecuteCase1(READER_APDU_Command *pApdu, READER_APDU_Response *pResp){
	READER_TPDU_Command tpduCmd;
	READER_TPDU_Response tpduResp;
	READER_Status retVal;
	uint32_t i;
	
	retVal = READER_TPDU_Forge(&tpduCmd,
		pApdu->header.CLA,
		pApdu->header.INS,
		pApdu->header.P1,
		pApdu->header.P2,
		0,
		NULL,
		0
	);
	if(retVal != READER_OK) return retVal;
	
	retVal = READER_TPDU_Execute(&tpduCmd, &tpduResp, timeout);
	if(retVal != READER_OK) return retVal;
	
	
	/* On map la reponse TPDU sur la reponse APDU */
	pResp->SW1 = tpduResp.SW1;
	pResp->SW2 = tpduResp.SW2;
	pREsp->dataSize = tpduResp.dataSize;
	for(i=0; i<tpduResp.dataSize; i++){
		pResp->dataBytes[i] = tpduResp.dataBytes[i];
	}
	
	return READER_OK;
}
