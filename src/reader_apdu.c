#include "reader_tpdu.h"
#include "reader_hal.h"
#include "reader_apdu.h"



/**
 * \fn READER_APDU_ProtocolCase READER_APDU_GetProtocolCase(READER_APDU_Command *pApduCmd)
 * \brief Cette fonction analyse une structure de type READER_APDU_Command et en déduit dans quel "sous cas" du protocole cet APDU appartient. Par exemple 2E, 2S, 3E, 3S, 4E, 4S etc ...
 * \return Retourne une valeur de type READER_APDU_ProtocolCase.
 * \param *pApduCmd Un pointeur sur un structure de type READER_APDU_Command.
 */
READER_APDU_ProtocolCase READER_APDU_GetProtocolCase(READER_APDU_Command *pApduCmd){
	uint32_t Nc, Ne;
	
	Nc = pApduCmd->body.Nc;
	Ne = pApduCmd->body.Ne;
	
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
	else if((Nc!=0) && (Ne!=0)){
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
	else{
		return READER_APDU_CASE_ERR;
	}
}


/**
 * \fn READER_Status READER_APDU_Send(READER_APDU_Command *pApduCmd)
 * \brief Cette fonction permet d'envoyer un commande APDU. Elle analyse l'APDU en paramètre pour en déduire le "sous cas" du protocole (3E, 3S, 4E, etc ...). Elle appelle alors une autre fonction READER_APDU_SendCase[..]() selon le cas.
 * \return Valeur de type READER_Status. READER_OK si l'exécution s'est correctement déroulée. READER_ERR dans le cas contraire.
 * \param *pApduCmd Un pointeur sur un structure de type READER_APDU_Command.
 */
READER_Status READER_APDU_Execute(READER_APDU_Command *pApduCmd, READER_APDU_Response *pApduResp, uint32_t timeout){
	READER_APDU_ProtocolCase protocolCase;
	
	protocolCase = READER_APDU_GetProtocolCase(pApduCmd);
	
	switch(protocolCase){
		case READER_APDU_CASE_1:
			return READER_APDU_ExecuteCase1(pApduCmd, pApduResp, timeout);
			break;
			
		case READER_APDU_CASE_2E:
			return READER_APDU_ExecuteCase2E(pApduCmd, pApduResp, timeout);
			break;
			
		case READER_APDU_CASE_2S:
			return READER_APDU_ExecuteCase2S(pApduCmd, pApduResp, timeout);
			break;
			
		case READER_APDU_CASE_3E:
			return READER_APDU_ExecuteCase3E(pApduCmd, pApduResp, timeout);
			break;
			
		case READER_APDU_CASE_3S:
			return READER_APDU_ExecuteCase3S(pApduCmd, pApduResp, timeout);
			break;
			
		case READER_APDU_CASE_4E:
			return READER_APDU_ExecuteCase4E(pApduCmd, pApduResp, timeout);
			break;
			
		case READER_APDU_CASE_4S:
			return READER_APDU_ExecuteCase4S(pApduCmd, pApduResp, timeout);
			break;
			
		case READER_APDU_CASE_ERR:
			return READER_ERR;
			break;
			
		default:
			return READER_ERR;
	}
}



READER_Status READER_APDU_ExecuteCase1(READER_APDU_Command *pApduCmd, READER_APDU_Response *pApduResp, uint32_t timeout){
	/* Voir ISO7816-3 section 12.2.2 */
	READER_TPDU_Command tpduCmd;
	READER_TPDU_Response tpduResp;
	READER_Status retVal;
	
	/* On forge une structure de type commande TPDU */
	retVal = READER_TPDU_Forge(
		&tpduCmd,
		pApduCmd->header.CLA,
		pApduCmd->header.INS,
		pApduCmd->header.P1,
		pApduCmd->header.P2,
		0,
		NULL,
		0
	);
	if(retVal != READER_OK) return retVal;
	
	/* On envoie cet TPDU */
	retVal = READER_TPDU_Send(&tpduCmd, timeout);
	if(retVal != READER_OK) return retVal;
	
	/* On recupere le reponse */
	retVal = READER_TPDU_RcvResponse(&tpduResp, 0, timeout);
	if(retVal != READER_OK) return retVal;	
	
	/* On map la reponse TPDU sur la reponse APDU */
	retVal = READER_APDU_MapTpduRespToApdu(&tpduResp, pApduResp);
	if(retVal != READER_OK) return READER_ERR;
	
	return READER_OK;
}


READER_Status READER_APDU_ExecuteCase2S(READER_APDU_Command *pApduCmd, READER_APDU_Response *pApduResp, uint32_t timeout){
	/* Voir ISO7816-3 section 12.2.3 */
	READER_TPDU_Command tpduCmd;
	READER_TPDU_Response tpduResp;
	READER_Status retVal;
	uint32_t Na;
	
	/* On fabrique la requette TPDU qui correspond */
	retVal = READER_TPDU_Forge(
		&tpduCmd,
		pApduCmd->header.CLA,
		pApduCmd->header.INS,
		pApduCmd->header.P1,
		pApduCmd->header.P2,
		READER_APDU_NeToLe(pApduCmd->body.Ne),
		NULL,
		0
	);
	if(retVal != READER_OK) return retVal;
	
	/* On envoie la requette TPDU */
	retVal = READER_TPDU_Send(&tpduCmd, timeout);
	if(retVal != READER_OK) return retVal;
	
	/* On recupere le TPDU response */
	retVal = READER_TPDU_RcvResponse(&tpduResp, pApduCmd->body.Ne, timeout);
	if(retVal != READER_OK) return retVal;
	
	/* Case 2S.2 Process Aborted, Ne definitely not accepted        */
	if((tpduResp.SW1 == 0x67) && (tpduResp.SW2 == 0x00)){
		retVal = READER_APDU_MapTpduRespToApdu(&tpduResp, pApduResp);
		if(retVal != READER_OK) return READER_ERR;
	}	
	/* Case 2S.3 Process Aborted, Ne not accepted, Na indicated     */
	else if(tpduResp.SW1 == 0x6C){
		/* On forge un nouveau TPDU avec P3 = SW2 */
		Na = tpduResp.SW2;
		retVal = READER_TPDU_Forge(
			&tpduCmd,
			pApduCmd->header.CLA,
			pApduCmd->header.INS,
			pApduCmd->header.P1,
			pApduCmd->header.P2,
			Na,
			NULL,
			0
		);
		if(retVal != READER_OK) return retVal;
	
		retVal = READER_TPDU_Send(&tpduCmd, timeout);
		if(retVal != READER_OK) return retVal;
		
		retVal = READER_TPDU_RcvResponse(&tpduResp, Na, timeout);
		if(retVal != READER_OK) return retVal;
		
		retVal = READER_APDU_MapTpduRespToApdu(&tpduResp, pApduResp);
		if(retVal != READER_OK) return READER_ERR;
	}
	/* Case 2S.1 Process Completed, Ne accepted                     */
	/* Case 2S.4 SW1SW2 = 8XYZ et SW1SW2 != 9000                    */
	else{
		retVal = READER_APDU_MapTpduRespToApdu(&tpduResp, pApduResp);
		if(retVal != READER_OK) return READER_ERR;
	}
	
	return READER_OK;
}


READER_Status READER_APDU_ExecuteCase3S(READER_APDU_Command *pApduCmd, READER_APDU_Response *pApduResp, uint32_t timeout){
	/* Voir ISO7816-3 section 12.2.4 */
	READER_TPDU_Command tpduCmd;
	READER_TPDU_Response tpduResp;
	READER_Status retVal;
	
	/* verifications preliminaires */
	if((pApduCmd->body.Nc > 255) || (pApduCmd->body.Ne != 0)){
		return READER_ERR;
	}
	
	/* On forge une TPDU avec P3 = Nc et TPDU data = APDU data */
	retVal = READER_TPDU_Forge(
		&tpduCmd,
		pApduCmd->header.CLA,
		pApduCmd->header.INS,
		pApduCmd->header.P1,
		pApduCmd->header.P2,
		READER_APDU_NcToLc(pApduCmd->body.Nc),
		pApduCmd->body.pDataField,
		pApduCmd->body.Nc
	);
	if(retVal != READER_OK) return retVal;
	
	/* On envoie la requette TPDU */
	retVal = READER_TPDU_Send(&tpduCmd, timeout);
	if(retVal != READER_OK) return retVal;
	
	/* On recupere la reponse. On attend pas de donnees en retour, juste SW */
	retVal = READER_TPDU_RcvResponse(&tpduResp, 0, timeout);
	if(retVal != READER_OK) return retVal;
	
	/* On mape la reponse APDU sur la reponse TPDU */
	retVal = READER_APDU_MapTpduRespToApdu(&tpduResp, pApduResp);
	if(retVal != READER_OK) return READER_ERR;
	
	return READER_OK;
}



READER_Status READER_APDU_ExecuteCase3E(READER_APDU_Command *pApduCmd, READER_APDU_Response *pApduResp, uint32_t timeout){
	/* Voir ISO7816-3 section 12.2.7 */
	READER_TPDU_Command tpduCmd;
	READER_TPDU_Response tpduResp;
	READER_Status retVal;
	
	uint8_t tmpTpduBuff[255];
	uint32_t i,j;
	uint32_t Nc;
	uint32_t nbSegments, nbResidualBytes;
	
	Nc = pApduCmd->body.Nc;
	
	
	/* On segmente le long APDU en un ensemble de trames de taille <= 255                                                             */
	/* Chacune des trames ainsi formes sera sera placee dans le data field d'un TPDU.                                                 */
	/* Chacun des TPDU ainsi formes sera de type INS={ENVELOPE}                                                                       */
																									                                  
	/* Dans la premiere trame de cet ensemble on y mets egalement le header de l'APDU original                                        */
																														             
	tmpTpduBuff[0] = pApduCmd->header.CLA;                                                                                           
	tmpTpduBuff[1] = pApduCmd->header.INS;                                                                                           
	tmpTpduBuff[2] = pApduCmd->header.P1;                                                                                            
	tmpTpduBuff[3] = pApduCmd->header.P2;                                                                                            
																														             
	/* On recupere 251 caracteres dans le data field de la commande APDU et on les place dans la 1ere trame (251+4=255)               */
	for(i=0; i<251; i++){
		tmpTpduBuff[i+4] = pApduCmd->body.pDataField[i];
	}
	
	/* La premiere trame est prete. On forge un TPDU avec, on l'envoie puis on regarde comment reagit la carte a la commande ENVELOPE */
	retVal = READER_TPDU_Forge(&tpduCmd, pApduCmd->header.CLA, READER_APDU_INS_ENVELOPE, 0x00, 0x00, 255, tmpTpduBuff, 255);
	if(retVal != READER_OK) return retVal;
	
	retVal = READER_TPDU_Execute(&tpduCmd, &tpduResp, timeout);
	if(retVal != READER_OK) return retVal;
	
	/* On regarde le SW de la reponse et on en déduit si la carte supporte ou non l'instruction ENVELOPE                 */
	/* Si la carte ne supporte pas ENVELOPE                                                                              */
	if((tpduResp.SW1 == 0x6D) && (tpduResp.SW2 == 0x00)){
		retVal = READER_APDU_MapTpduRespToApdu(&tpduResp, pApduResp);    /* On mape la reponse APDU sur la reponse TPDU  */
		if(retVal != READER_OK) return READER_ERR;
		
		return READER_OK;                                                /* On termine la et on ne renvoie pas d'erreur. L'exec s'est correctement deroulee, simplement la carte ne supporte pas */
	}
	/* Si la carte supporte correctement ENVELOPE                                                                        */
	else if((tpduResp.SW1 == 0x90) && (tpduResp.SW2 == 0x00)){                                                          
		/* On va decouper les data restantes de l'APDU ... */
		nbSegments = (Nc - 251) / 255;
		nbResidualBytes = (Nc - 251) % 255;
		
		for(i=0; i<nbSegments; i++){
			for(j=0; j<255; j++){
				tmpTpduBuff[j] = pApduCmd->body.pDataField[251 + (i*255) + j];
			}
			retVal = READER_TPDU_Forge(&tpduCmd, pApduCmd->header.CLA, READER_APDU_INS_ENVELOPE, 0x00, 0x00, 255, tmpTpduBuff, 255);
			if(retVal != READER_OK) return retVal;
			
			retVal = READER_TPDU_Execute(&tpduCmd, &tpduResp, timeout);
			if(retVal != READER_OK) return retVal;
			
			if((tpduResp.SW1 != 0x90) || (tpduResp.SW2 != 0x00)){  /* A propri la commande ENVELOPE est mal passée */
				return READER_ERR;  /* A determiner si vraiment on renvoi un erreur a cet endroit */
			}
		}
		
		/* On envoie le reste des residual bytes */
		for(i=0; i<nbResidualBytes; i++){
			tmpTpduBuff[i] = pApduCmd->body.pDataField[251 + (nbSegments*255) + i]; 
		}
		retVal = READER_TPDU_Forge(&tpduCmd, pApduCmd->header.CLA, READER_APDU_INS_ENVELOPE, 0x00, 0x00, nbResidualBytes, tmpTpduBuff, nbResidualBytes);
		if(retVal != READER_OK) return retVal;
		
		retVal = READER_TPDU_Execute(&tpduCmd, &tpduResp, timeout);
		if(retVal != READER_OK) return retVal;
		
		if((tpduResp.SW1 != 0x90) || (tpduResp.SW2 != 0x00)){  /* A propri la commande ENVELOPE est mal passée */
			return READER_ERR;  /* A determiner si vraiment on renvoi un erreur a cet endroit */
		}
		
		/* On envoie une commande ENVELOPE vide. Cela permet d'indiquer a la carte que toutes les donnees on ete envoyees */
		retVal = READER_TPDU_Forge(&tpduCmd, pApduCmd->header.CLA, READER_APDU_INS_ENVELOPE, 0x00, 0x00, 0, NULL, 0);
		if(retVal != READER_OK) return retVal;
		retVal = READER_TPDU_Execute(&tpduCmd, &tpduResp, timeout);
		if(retVal != READER_OK) return retVal;
		
		/* Le SW de la derniere commande ENVELOPE vide est la SW de la commande globale */
		/* Donc on mape le dernier TPDU response sur le APDU response          */
		retVal = READER_APDU_MapTpduRespToApdu(&tpduResp, pApduResp);
		if(retVal != READER_OK) return READER_ERR;
	}                                                                                                                   
	/* Si on a recu un SW qui correspond pas a la sequence prevue par le protocole                                       */
	else{
		return READER_ERR;
	}
		
	
	return READER_OK;
}



READER_Status READER_APDU_ExecuteCase2E(READER_APDU_Command *pApduCmd, READER_APDU_Response *pApduResp, uint32_t timeout){
	/* Voir ISO7816-3 section 12.2.6 */
	READER_TPDU_Command tpduCmd;
	READER_TPDU_Response tpduResp;
	READER_Status retVal;
	
	uint32_t Ne, Nm, Nx;            /* Meme notation que ISO7816  */
	uint32_t i;
	
	Ne = pApduCmd->body.Ne;         /* Ne = N expected            */
	Nm = Ne;                        /* Nm = N remaining           */
	
	/* On envoie le premier TPDU avec le header de l'APDU et P3=0 */
	retVal = READER_TPDU_Forge(&tpduCmd, pApduCmd->header.CLA, pApduCmd->header.INS, pApduCmd->header.P1, pApduCmd->header.P2, 0, NULL, 0);
	if(retVal != READER_OK) return retVal;
	
	retVal = READER_TPDU_Execute(&tpduCmd, &tpduResp, timeout);
	if(retVal != READER_OK) return retVal;
	
	/* On verifie la reponse de la carte */
	if((tpduResp.SW1 == 0x67) && (tpduResp.SW2 == 0x00)){                   /* La carte a abort total a cause de wrong length */
		retVal = READER_APDU_MapTpduRespToApdu(&tpduResp, pApduResp);
		if(retVal != READER_OK) return READER_ERR;
		return READER_OK;
	}
	else if(tpduResp.SW1 == 0x6C){                                          /* La carte a abort a cause de wrong lenth, mais indique Na */
		retVal = READER_APDU_MapTpduRespToApdu(&tpduResp, pApduResp);
		if(retVal != READER_OK) return READER_ERR;
		return READER_OK;
	}
	else if((tpduResp.SW1 == 0x90) && (tpduResp.SW2 == 0x00)){
		
	}
	else if(tpduResp.SW1 == 0x61){
		/* Tantque il reste des donees a recevoir on envoie des TPDU de type GET RESPONSE */
		while((Nm > 0) && (tpduResp.SW1 != 0x90)){
			Nx = tpduResp.SW2;
			
			/* Preparation de la commande GET RESPONSE */
			retVal = READER_TPDU_Forge(&tpduCmd, pApduCmd->header.CLA, READER_APDU_INS_GETRESPONSE, 0x00, 0x00, (Nx < Nm)? Nx:Nm, NULL, 0);
			if(retVal != READER_ERR) return retVal;
			
			retVal = READER_TPDU_Execute(&tpduCmd, &tpduResp, timeout);
			if(retVal != READER_OK) return retVal;
			
			/* Selon le status word du GET RESPONSE ... */
			if(tpduResp.SW1 == 0x61){
				for(i=0; i<tpduResp.dataSize; i++){
					pApduResp->dataBytes[(pApduResp->dataSize) + i] = tpduResp.dataBytes[i];   /* On copie les data du GET RESPONSE dans le APDU response au bon endroit */
				}
				pApduResp->dataSize += tpduResp.dataSize;
				Nm = Nm - tpduResp.dataSize;
			}
			else if((tpduResp.SW1 == 0x90) && (tpduResp.SW2 == 0x00)){
				for(i=0; i<tpduResp.dataSize; i++){
					pApduResp->dataBytes[(pApduResp->dataSize) + i] = tpduResp.dataBytes[i];   /* On copie les data du GET RESPONSE dans le APDU response au bon endroit */
				}
				pApduResp->dataSize += tpduResp.dataSize;
				Nm = Nm - tpduResp.dataSize;
			}
			else{
				return READER_ERR;
			}
		}
		
		/* On complete le SW du response APDU. C'est le SW du dernier TPDU response recu  */
		pApduResp->SW1 = tpduResp.SW1;
		pApduResp->SW2 = tpduResp.SW2;
	}
	else{
		return READER_ERR;
	}
	
	return READER_OK;
}



READER_Status READER_APDU_ExecuteCase4S(READER_APDU_Command *pApduCmd, READER_APDU_Response *pApduResp, uint32_t timeout){
	READER_TPDU_Command tpduCmd;
	READER_TPDU_Response tpduResp;
	READER_APDU_Command newApduCmd;
	READER_APDU_Response newApduResp;
	READER_Status retVal;
	uint32_t Na;
	
	/* On forge le premier TPDU avec le header de l'APDU et P3=Lc */
	retVal = READER_TPDU_Forge(&tpduCmd, pApduCmd->header.CLA, pApduCmd->header.INS, pApduCmd->header.P1, pApduCmd->header.P2, READER_APDU_NcToLc(pApduCmd->body.Nc), NULL, 0);
	if(retVal != READER_OK) return retVal;
	
	/* On evoie le TPDU */
	retVal = READER_TPDU_Send(&tpduCmd, timeout);
	if(retVal != READER_OK) return retVal;
	
	/* On attend en reponse juste un SW (avant d'envoyer le GET RESPONSE) */
	retVal = READER_TPDU_RcvResponse(&tpduResp, 0, timeout);
	if(retVal != READER_OK) return retVal;
	
	
	/* Selon le SW du premier TPDU response */
	/* Cas 4S.1 */
	if((tpduResp.SW1 == 0x60) || (tpduResp.SW1 == 0x64) || (tpduResp.SW1 == 0x65) || (tpduResp.SW1 == 0x66) || (tpduResp.SW1 == 0x67) || (tpduResp.SW1 == 0x68) || (tpduResp.SW1 == 0x69) || (tpduResp.SW1 == 0x6A) || (tpduResp.SW1 == 0x6B) || (tpduResp.SW1 == 0x6C) || (tpduResp.SW1 == 0x6D)  || (tpduResp.SW1 == 0x6E) || (tpduResp.SW1 == 0x6F)){
		retVal = READER_APDU_MapTpduRespToApdu(&tpduResp, pApduResp);
		if(retVal != READER_OK) return READER_ERR;
		return READER_OK;
	}
	/* Cas 4S.2 */
	else if((tpduResp.SW1 == 0x90) && (tpduResp.SW2 == 0x00)){
		/* On envoie un APDU de type GET RESPONSE, on se rabat sur le cas 2S. */
		retVal = READER_APDU_Forge(&newApduCmd, pApduCmd->header.CLA, READER_APDU_INS_GETRESPONSE, 0x00, 0x00, 0, NULL, pApduCmd->body.Ne);
		if(retVal != READER_OK) return retVal;
		
		retVal = READER_APDU_ExecuteCase2S(&newApduCmd, pApduResp, timeout);
		if(retVal != READER_OK) return retVal;
		
		//retVal = READER_APDU_CopyResponse(&newApduResp, pApduResp);
		//if(retVal != READER_OK) return retVal;
	}
	/* Cas 4S.3 */
	else if(tpduResp.SW1 == 0x61){
		Na = tpduResp.SW2;
		retVal = READER_APDU_Forge(&newApduCmd, pApduCmd->header.CLA, READER_APDU_INS_GETRESPONSE, 0x00, 0x00, 0, NULL, Na);
		if(retVal != READER_OK) return retVal;
		
		retVal = READER_APDU_ExecuteCase2S(&newApduCmd, pApduResp, timeout);
		if(retVal != READER_OK) return retVal;
	}
	/* Cas 4S.4 */
	else if((tpduResp.SW1 == 0x61) || (tpduResp.SW1 == 0x62) || (tpduResp.SW1 == 0x63) || (((tpduResp.SW1 & 0xF0) == 0x90) && (tpduResp.SW2 != 0x00)) ){
		retVal = READER_APDU_MapTpduRespToApdu(&tpduResp, pApduResp);
		if(retVal != READER_OK) return READER_ERR;
		return READER_OK;
	}
	else{
		return READER_ERR;
	}
	
	return READER_OK;
}



/**
 * \fn READER_Status READER_APDU_MapTpduRespToApdu(READER_TPDU_Response *pTpduResp, READER_APDU_Response *pApduResp)
 * \brief Cette fonction pemret de mapper un object de type "TPDU Response" sur un objet de type "APDU Response". Attention cette fonction n'est pas prévue pour le cas où la taille des données du "APDU Response" est strictement supérieure à 256 octets (cas 2E et 4E).
 */
READER_Status READER_APDU_MapTpduRespToApdu(READER_TPDU_Response *pTpduResp, READER_APDU_Response *pApduResp){
	uint32_t i;
	
	if(pTpduResp->dataSize > 256) return READER_ERR;
	
	/* On map la reponse TPDU sur la reponse APDU */
	pApduResp->SW1 = pTpduResp->SW1;
	pApduResp->SW2 = pTpduResp->SW2;
	pApduResp->dataSize = pTpduResp->dataSize;
	for(i=0; i<pTpduResp->dataSize; i++){
		pApduResp->dataBytes[i] = pTpduResp->dataBytes[i];
	}
	
	return READER_OK;
}


/**
 * \fn READER_Status READER_APDU_CopyCommand(READER_APDU_Command *pSourceApdu, READER_APDU_Command *pDestApdu)
 * \brief Cette fonction permet de copier le contenu d'une commande APDU source dans une commande APDU de destination.
 * \return Valeur de retour de type READER_Status. READER_OK indique le bon déroulement de la fonction. Toute autre valeur indique une erreur.
 * \param *pSourceApdu est un pointeur sur l'APDU source. Il s'agit d'une structure de type READER_APDU_Command. 
 * \param *pDestApdu est un pointeur sur l'APDU de destination. Il s'agit d'une structure de type READER_APDU_Command.
 */
READER_Status READER_APDU_CopyCommand(READER_APDU_Command *pSourceApdu, READER_APDU_Command *pDestApdu){
	uint32_t i;
	
	pDestApdu->header.CLA = pSourceApdu->header.CLA;
	pDestApdu->header.INS = pSourceApdu->header.INS;
	pDestApdu->header.P1  = pSourceApdu->header.P1;
	pDestApdu->header.P2  = pSourceApdu->header.P2;
	
	pDestApdu->body.Nc    = pSourceApdu->body.Nc;
	pDestApdu->body.Ne    = pSourceApdu->body.Ne;
	
	for(i=0; i<pSourceApdu->body.Nc; i++){
		pDestApdu->body.dataBytes[i] = pSourceApdu->body.dataBytes[i];
	}
	
	return READER_OK;
}


/**
 * \fn READER_Status READER_APDU_CopyResponse(READER_APDU_Response *pSourceApdu, READER_APDU_Response *pDestApdu)
 * \brief Cette fonction permet de copier le contenu d'une réponse APDU source dans une réponse APDU de destination.
 * \return Valeur de retour de type READER_Status. READER_OK indique le bon déroulement de la fonction. Toute autre valeur indique une erreur.
 * \param *pSourceApdu est un pointeur sur l'APDU source. Il s'agit d'une structure de type READER_APDU_Response. 
 * \param *pDestApdu est un pointeur sur l'APDU de destination. Il s'agit d'une structure de type READER_APDU_Response.
 */
READER_Status READER_APDU_CopyResponse(READER_APDU_Response *pSourceApdu, READER_APDU_Response *pDestApdu){
	uint32_t i;
	
	pDestApdu->SW1  = pSourceApdu->SW1;
	pDestApdu->SW2  = pSourceApdu->SW2;
	pDestApdu->dataSize  = pSourceApdu->dataSize;
	
	for(i=0; i<pApduSource->dataSize; i++){
		pApduDest->dataBytes[i] = pApduSource->dataBytes[i];
	}
	
	return READER_OK;
}


uint16_t READER_APDU_NcToLc(uint16_t Nc){
	return Nc;
}

uint16_t READER_APDU_NeToLe(uint16_t Ne){
	if(Ne == 256){
		return 0;
	}
	else{
		return Ne;
	}
}

uint16_t READER_APDU_LcToNc(uint16_t Lc){
	return Lc;
}

uint16_t READER_APDU_LeToNe(uint16_t Le){
	if(Le == 0){
		return 256;
	}
	else{
		return Le;
	}
}
