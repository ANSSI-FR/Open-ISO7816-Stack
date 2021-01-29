/**
 * \file reader_t0_apdu.h
 * \copyright This file is part of the Open-ISO7816-Stack project and is distributed under the MIT license. See LICENSE file in the root directory. 
 * \brief This file contains the code of all the functions defined in reader_t0_apdu.h. These functions are the ones needed to send APDU commands an receive APDU responses when using the protocol T=0.  
 */

#include "reader_t0_apdu.h"
#include "reader_t0_context_handler.h"
#include "reader_hal_comm_settings.h"



/**
 * \fn READER_Status READER_T0_APDU_Init(READER_T0_ContextHandler *pContext, READER_HAL_CommSettings *pSettings)
 * \return The function returns an execution code of type READER_Status that indicates if the function behaved as expected or not.
 * \param *pContext is a pointer on a READER_T0_ContextHandler data structure to be initailized by these funtion.
 * \param *pSettings is a pointer on a READER_HAL_CommSettings data structure that should already be containing the low level communications settings for the hardware abstraction layer.
 * The function have to be called before using APDUs with the T=0 protocol.
 * This function will initialize the T=0 communication context that is stored into *pContext.  
 */
READER_Status READER_T0_APDU_Init(READER_T0_ContextHandler *pContext, READER_HAL_CommSettings *pSettings){
	READER_Status retVal;
	
	
	retVal = READER_T0_CONTEXT_Init(pContext, pSettings);
	if(retVal != READER_OK) return retVal;
	
	
	return READER_OK;
}

/**
 * \fn READER_Status READER_T0_APDU_Execute(READER_T0_ContextHandler *pContext, READER_APDU_Command *pApduCmd, READER_APDU_Response *pApduResp)
 * \return The function returns an execution code of type READER_Status that indicates if the function behaved as expected or not.
 * \param *pContext is a pointer to a READER_T0_ContextHandler data structure. It contains the current T=0 protocol communication state. It should have been previously initialized by the READER_T0_APDU_Init() function.
 * \param *pApduCommand is a pointer to a READER_APDU_Command data structure. It should have been previously filled with an APDU command to be sent to the card. For example with the READER_T0_APDU_Forge() function.
 * \param *pApduResp is a pointer to a READER_APDU_Response data structure. This function will fill it with the response aswered by the card.
 * This function is used to execute an APDU command using the T=0 protocol regardingless to the cases 1, 2, 2E etc ... It means sending the APDU command to the card and getting back the response from the card.
 */
READER_Status READER_T0_APDU_Execute(READER_T0_ContextHandler *pContext, READER_APDU_Command *pApduCmd, READER_APDU_Response *pApduResp){
	READER_APDU_ProtocolCase protocolCase;
	
	protocolCase = READER_APDU_GetProtocolCase(pApduCmd);
	
	switch(protocolCase){
		case READER_APDU_CASE_1:
			return READER_T0_APDU_ExecuteCase1(pContext, pApduCmd, pApduResp);
			break;
			
		case READER_APDU_CASE_2E:
			return READER_T0_APDU_ExecuteCase2E(pContext, pApduCmd, pApduResp);
			break;
			
		case READER_APDU_CASE_2S:
			return READER_T0_APDU_ExecuteCase2S(pContext, pApduCmd, pApduResp);
			break;
			
		case READER_APDU_CASE_3E:
			return READER_T0_APDU_ExecuteCase3E(pContext, pApduCmd, pApduResp);
			break;
			
		case READER_APDU_CASE_3S:
			return READER_T0_APDU_ExecuteCase3S(pContext, pApduCmd, pApduResp);
			break;
			
		case READER_APDU_CASE_4E:
			return READER_T0_APDU_ExecuteCase4E(pContext, pApduCmd, pApduResp);
			break;
			
		case READER_APDU_CASE_4S:
			return READER_T0_APDU_ExecuteCase4S(pContext, pApduCmd, pApduResp);
			break;
			
		case READER_APDU_CASE_ERR:
			return READER_ERR;
			break;
			
		default:
			return READER_ERR;
	}
}

/**
 * \fn READER_Status READER_T0_APDU_ExecuteCase1(READER_T0_ContextHandler *pContext, READER_APDU_Command *pApduCmd, READER_APDU_Response *pApduResp)
 * \return Returns an execution code of type READER_Status that indicates if the function behaved as expected or not.
 * \param *pContext is a pointer to a READER_T0_ContextHandler data structure. It contains the current T=0 protocol communication state. It should have been previously initialized by the READER_T0_APDU_Init() function.
 * \param *pApduCommand is a pointer to a READER_APDU_Command data structure. It should have been previously filled with an APDU command to be sent to the card. For example with the READER_T0_APDU_Forge() function.
 * \param *pApduResp is a pointer to a READER_APDU_Response data structure. This function will fill it with the response aswered by the card.
 * This function is used to execute an APDU command in the Case 1 described in ISO7816-3 section 12.1.2.
 */
READER_Status READER_T0_APDU_ExecuteCase1(READER_T0_ContextHandler *pContext, READER_APDU_Command *pApduCmd, READER_APDU_Response *pApduResp){
	/* Voir ISO7816-3 section 12.2.2 */
	READER_TPDU_Command tpduCmd;
	READER_TPDU_Response tpduResp;
	READER_HAL_CommSettings *pSettings;
	READER_Status retVal;
	uint32_t timeout;
	
	
	/* Recuperation d'un pointeur sur les parametres de comm bas niveau actuellement utilises ...  */
	retVal = READER_T0_CONTEXT_GetHalCommSettingsPtr(pContext, &pSettings);
	if(retVal != READER_OK) return retVal;
	
	/* Recuperation de la valeur de timeout actuellement utilisee dans le contexte de communication ...  */
	retVal = READER_T0_CONTEXT_GetCurrentWTMilli(pContext, &timeout);
	if(retVal != READER_OK) return retVal;
	
	
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
	retVal = READER_TPDU_Send(&tpduCmd, &tpduResp, timeout, pSettings);
	if((retVal != READER_OK) && (retVal != READER_GOT_SW1)) return retVal;
	
	/* On recupere le reponse */
	if(retVal != READER_GOT_SW1){
		retVal = READER_TPDU_RcvResponse(&tpduResp, 0, timeout, pSettings);
		if(retVal != READER_OK) return retVal;
	}
	
	/* On map la reponse TPDU sur la reponse APDU */
	retVal = READER_T0_APDU_MapTpduRespToApdu(pContext, &tpduResp, pApduResp);
	if(retVal != READER_OK) return READER_ERR;
	
	return READER_OK;
}

/**
 * \fn READER_Status READER_T0_APDU_ExecuteCase2S(READER_T0_ContextHandler *pContext, READER_APDU_Command *pApduCmd, READER_APDU_Response *pApduResp)
 * \param *pContext is a pointer to a READER_T0_ContextHandler data structure. It contains the current T=0 protocol communication state. It should have been previously initialized by the READER_T0_APDU_Init() function.
 * \param *pApduCommand is a pointer to a READER_APDU_Command data structure. It should have been previously filled with an APDU command to be sent to the card. For example with the READER_T0_APDU_Forge() function.
 * \param *pApduResp is a pointer to a READER_APDU_Response data structure. This function will fill it with the response aswered by the card.
 * \return READER_Status execution code.
 * This function executes an APDU command in the case 2S. See ISO7816-3 section 12.1.3.
 */
READER_Status READER_T0_APDU_ExecuteCase2S(READER_T0_ContextHandler *pContext, READER_APDU_Command *pApduCmd, READER_APDU_Response *pApduResp){
	/* Voir ISO7816-3 section 12.2.3 */
	READER_TPDU_Command tpduCmd;
	READER_TPDU_Response tpduResp;
	READER_HAL_CommSettings *pSettings;
	READER_Status retVal;
	uint8_t Na, Ne;
	uint32_t timeout;
	
	
	/* Recuperation d'un pointeur sur les parametres de comm bas niveau actuellement utilises ...  */
	retVal = READER_T0_CONTEXT_GetHalCommSettingsPtr(pContext, &pSettings);
	if(retVal != READER_OK) return retVal;
	
	/* Recuperation de la valeur de timeout actuellement utilisee dans le contexte de communication ...  */
	retVal = READER_T0_CONTEXT_GetCurrentWTMilli(pContext, &timeout);
	if(retVal != READER_OK) return retVal;
	
	Ne = pApduCmd->body.Ne;
	
	/* On fabrique la requette TPDU qui correspond */
	retVal = READER_TPDU_Forge(
		&tpduCmd,
		pApduCmd->header.CLA,
		pApduCmd->header.INS,
		pApduCmd->header.P1,
		pApduCmd->header.P2,
		READER_APDU_NeToLe(Ne),
		NULL,
		0
	);
	if(retVal != READER_OK) return retVal;
	
	/* On envoie la requette TPDU */
	retVal = READER_TPDU_Send(&tpduCmd, &tpduResp, timeout, pSettings);
	if((retVal != READER_OK) && (retVal != READER_GOT_SW1)) return retVal;
	
	/* On recupere le TPDU response */
	if(retVal != READER_GOT_SW1){
		retVal = READER_TPDU_RcvResponse(&tpduResp, Ne, timeout, pSettings);
		if((retVal != READER_OK) && (retVal != READER_TIMEOUT_GOT_ONLY_SW)) return retVal;
	}
	
	/* Case 2S.2 Process Aborted, Ne definitely not accepted        */
	if((tpduResp.SW1 == 0x67) && (tpduResp.SW2 == 0x00)){
		retVal = READER_T0_APDU_MapTpduRespToApdu(pContext, &tpduResp, pApduResp);
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
			READER_APDU_NeToLe(Na),
			NULL,
			0
		);
		if(retVal != READER_OK) return retVal;
	
		retVal = READER_TPDU_Send(&tpduCmd, &tpduResp, timeout, pSettings);
		if((retVal != READER_OK) && (retVal != READER_GOT_SW1)) return retVal;
		
		if(retVal != READER_GOT_SW1){
			/* 2S3 Dans tous les cas, au niveau TPDU, on en recupere Na Bytes .. */
			retVal = READER_TPDU_RcvResponse(&tpduResp, Na, timeout, pSettings);    /* Voir ISO7816-3 section 12.2.3 case 2S.3 */
			if(retVal != READER_OK) return retVal;
			
			if(Ne<Na){
				tpduResp.dataSize = Ne;
			}
		}
		
		retVal = READER_T0_APDU_MapTpduRespToApdu(pContext, &tpduResp, pApduResp);
		if(retVal != READER_OK) return READER_ERR;
	}
	/* Case 2S.1 Process Completed, Ne accepted                     */
	/* Case 2S.4 SW1SW2 = 8XYZ et SW1SW2 != 9000                    */
	else{
		retVal = READER_T0_APDU_MapTpduRespToApdu(pContext, &tpduResp, pApduResp);
		if(retVal != READER_OK) return READER_ERR;
	}
	
	return READER_OK;
}

/**
 * \fn READER_Status READER_T0_APDU_ExecuteCase3S(READER_T0_ContextHandler *pContext, READER_APDU_Command *pApduCmd, READER_APDU_Response *pApduResp)
 * \param *pContext is a pointer to a READER_T0_ContextHandler data structure. It contains the current T=0 protocol communication state. It should have been previously initialized by the READER_T0_APDU_Init() function.
 * \param *pApduCommand is a pointer to a READER_APDU_Command data structure. It should have been previously filled with an APDU command to be sent to the card. For example with the READER_T0_APDU_Forge() function.
 * \param *pApduResp is a pointer to a READER_APDU_Response data structure. This function will fill it with the response aswered by the card.
 * \return READER_Status execution code.
 * This function executes an APDU command in the case 3S. See ISO7816-3 section 12.1.4.
 */
READER_Status READER_T0_APDU_ExecuteCase3S(READER_T0_ContextHandler *pContext, READER_APDU_Command *pApduCmd, READER_APDU_Response *pApduResp){
	/* Voir ISO7816-3 section 12.2.4 */
	READER_TPDU_Command tpduCmd;
	READER_TPDU_Response tpduResp;
	READER_HAL_CommSettings *pSettings;
	READER_Status retVal;
	uint32_t timeout;
	
	
	/* Recuperation d'un pointeur sur les parametres de comm bas niveau actuellement utilises ...  */
	retVal = READER_T0_CONTEXT_GetHalCommSettingsPtr(pContext, &pSettings);
	if(retVal != READER_OK) return retVal;
	
	/* Recuperation de la valeur de timeout actuellement utilisee dans le contexte de communication ...  */
	retVal = READER_T0_CONTEXT_GetCurrentWTMilli(pContext, &timeout);
	if(retVal != READER_OK) return retVal;
	
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
		pApduCmd->body.dataBytes,
		pApduCmd->body.Nc
	);
	if(retVal != READER_OK) return retVal;
	
	/* On envoie la requette TPDU */
	retVal = READER_TPDU_Send(&tpduCmd, &tpduResp, timeout, pSettings);
	if((retVal != READER_OK) && (retVal != READER_GOT_SW1)) return retVal;
	
	/* On recupere la reponse. On attend pas de donnees en retour, juste SW */
	if(retVal != READER_GOT_SW1){
		retVal = READER_TPDU_RcvResponse(&tpduResp, 0, timeout, pSettings);
		if(retVal != READER_OK) return retVal;
	}
	
	/* On mape la reponse APDU sur la reponse TPDU */
	retVal = READER_T0_APDU_MapTpduRespToApdu(pContext, &tpduResp, pApduResp);
	if(retVal != READER_OK) return READER_ERR;
	
	return READER_OK;
}


/**
 * \fn READER_Status READER_T0_APDU_ExecuteCase3E(READER_T0_ContextHandler *pContext, READER_APDU_Command *pApduCmd, READER_APDU_Response *pApduResp)
 * \param *pContext is a pointer to a READER_T0_ContextHandler data structure. It contains the current T=0 protocol communication state. It should have been previously initialized by the READER_T0_APDU_Init() function.
 * \param *pApduCommand is a pointer to a READER_APDU_Command data structure. It should have been previously filled with an APDU command to be sent to the card. For example with the READER_T0_APDU_Forge() function.
 * \param *pApduResp is a pointer to a READER_APDU_Response data structure. This function will fill it with the response aswered by the card.
 * \return READER_Status execution code.
 * This function executes an APDU command in the case 3E. See ISO7816-3 section 12.2.7.
 */
READER_Status READER_T0_APDU_ExecuteCase3E(READER_T0_ContextHandler *pContext, READER_APDU_Command *pApduCmd, READER_APDU_Response *pApduResp){
	/* Voir ISO7816-3 section 12.2.7 */
	READER_TPDU_Command tpduCmd;
	READER_TPDU_Response tpduResp;
	READER_HAL_CommSettings *pSettings;
	READER_Status retVal;
	
	uint8_t tmpTpduBuff[255];
	uint32_t i,j;
	uint32_t Nc;
	uint32_t nbSegments, nbResidualBytes;
	uint32_t timeout;
	
	
	/* Recuperation d'un pointeur sur les parametres de comm bas niveau actuellement utilises ...  */
	retVal = READER_T0_CONTEXT_GetHalCommSettingsPtr(pContext, &pSettings);
	if(retVal != READER_OK) return retVal;
	
	/* Recuperation de la valeur de timeout actuellement utilisee dans le contexte de communication ...  */
	retVal = READER_T0_CONTEXT_GetCurrentWTMilli(pContext, &timeout);
	if(retVal != READER_OK) return retVal;
	
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
		tmpTpduBuff[i+4] = pApduCmd->body.dataBytes[i];
	}
	
	/* La premiere trame est prete. On forge un TPDU avec, on l'envoie puis on regarde comment reagit la carte a la commande ENVELOPE */
	retVal = READER_TPDU_Forge(&tpduCmd, pApduCmd->header.CLA, READER_APDU_INS_ENVELOPE, 0x00, 0x00, 255, tmpTpduBuff, 255);
	if(retVal != READER_OK) return retVal;
	
	
	/* On envoie la requette TPDU */
	retVal = READER_TPDU_Send(&tpduCmd, &tpduResp, timeout, pSettings);
	if((retVal != READER_OK) && (retVal != READER_GOT_SW1)) return retVal;
	
	/* On recupere la reponse. On attend pas de donnees en retour, juste SW */
	if(retVal != READER_GOT_SW1){
		retVal = READER_TPDU_RcvResponse(&tpduResp, 0, timeout, pSettings);
		if(retVal != READER_OK) return retVal;
	}
	
	/* On regarde le SW de la reponse et on en déduit si la carte supporte ou non l'instruction ENVELOPE                 */
	/* Si la carte ne supporte pas ENVELOPE                                                                              */
	if((tpduResp.SW1 == 0x6D) && (tpduResp.SW2 == 0x00)){
		retVal = READER_T0_APDU_MapTpduRespToApdu(pContext, &tpduResp, pApduResp);    /* On mape la reponse APDU sur la reponse TPDU  */
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
				tmpTpduBuff[j] = pApduCmd->body.dataBytes[251 + (i*255) + j];
			}
			retVal = READER_TPDU_Forge(&tpduCmd, pApduCmd->header.CLA, READER_APDU_INS_ENVELOPE, 0x00, 0x00, 255, tmpTpduBuff, 255);
			if(retVal != READER_OK) return retVal;
			
			/* On envoie la requette TPDU */
			retVal = READER_TPDU_Send(&tpduCmd, &tpduResp, timeout, pSettings);
			if((retVal != READER_OK) && (retVal != READER_GOT_SW1)) return retVal;
			
			/* On recupere la reponse. On attend pas de donnees en retour, juste SW */
			if(retVal != READER_GOT_SW1){
				retVal = READER_TPDU_RcvResponse(&tpduResp, 0, timeout, pSettings);
				if(retVal != READER_OK) return retVal;
			}
			
			if((tpduResp.SW1 != 0x90) || (tpduResp.SW2 != 0x00)){  /* A propri la commande ENVELOPE est mal passée */
				return READER_ERR;  /* A determiner si vraiment on renvoi un erreur a cet endroit */
			}
		}
		
		/* On envoie le reste des residual bytes */
		for(i=0; i<nbResidualBytes; i++){
			tmpTpduBuff[i] = pApduCmd->body.dataBytes[251 + (nbSegments*255) + i]; 
		}
		retVal = READER_TPDU_Forge(&tpduCmd, pApduCmd->header.CLA, READER_APDU_INS_ENVELOPE, 0x00, 0x00, nbResidualBytes, tmpTpduBuff, nbResidualBytes);
		if(retVal != READER_OK) return retVal;
		
		/* On envoie la requette TPDU */
		retVal = READER_TPDU_Send(&tpduCmd, &tpduResp, timeout, pSettings);
		if((retVal != READER_OK) && (retVal != READER_GOT_SW1)) return retVal;
		
		/* On recupere la reponse. On attend pas de donnees en retour, juste SW */
		if(retVal != READER_GOT_SW1){
			retVal = READER_TPDU_RcvResponse(&tpduResp, 0, timeout, pSettings);
			if(retVal != READER_OK) return retVal;
		}
		
		if((tpduResp.SW1 != 0x90) || (tpduResp.SW2 != 0x00)){  /* A propri la commande ENVELOPE est mal passée */
			return READER_ERR;  /* A determiner si vraiment on renvoi un erreur a cet endroit */
		}
		
		/* On forge une commande ENVELOPE vide. Cela permet d'indiquer a la carte que toutes les donnees on ete envoyees */
		retVal = READER_TPDU_Forge(&tpduCmd, pApduCmd->header.CLA, READER_APDU_INS_ENVELOPE, 0x00, 0x00, 0, NULL, 0);
		if(retVal != READER_OK) return retVal;
		
		/* On envoie la requette TPDU */
		retVal = READER_TPDU_Send(&tpduCmd, &tpduResp, timeout, pSettings);
		if((retVal != READER_OK) && (retVal != READER_GOT_SW1)) return retVal;
		
		/* On recupere la reponse. On attend pas de donnees en retour, juste SW */
		if(retVal != READER_GOT_SW1){
			retVal = READER_TPDU_RcvResponse(&tpduResp, 0, timeout, pSettings);
			if(retVal != READER_OK) return retVal;
		}
		
		/* Le SW de la derniere commande ENVELOPE vide est la SW de la commande globale */
		/* Donc on mape le dernier TPDU response sur le APDU response          */
		retVal = READER_T0_APDU_MapTpduRespToApdu(pContext, &tpduResp, pApduResp);
		if(retVal != READER_OK) return READER_ERR;
	}                                                                                                                
	/* Si on a recu un SW qui correspond pas a la sequence prevue par le protocole                                       */
	else{
		return READER_ERR;
	}
		
	
	return READER_OK;
}


/**
 * \fn READER_Status READER_T0_APDU_ExecuteCase2E(READER_T0_ContextHandler *pContext, READER_APDU_Command *pApduCmd, READER_APDU_Response *pApduResp)
 * \param *pContext is a pointer to a READER_T0_ContextHandler data structure. It contains the current T=0 protocol communication state. It should have been previously initialized by the READER_T0_APDU_Init() function.
 * \param *pApduCommand is a pointer to a READER_APDU_Command data structure. It should have been previously filled with an APDU command to be sent to the card. For example with the READER_T0_APDU_Forge() function.
 * \param *pApduResp is a pointer to a READER_APDU_Response data structure. This function will fill it with the response aswered by the card.
 * \return READER_Status execution code.
 * This function executes an APDU command in the case 2E. See ISO7816-3 section 12.2.6.
 */
READER_Status READER_T0_APDU_ExecuteCase2E(READER_T0_ContextHandler *pContext, READER_APDU_Command *pApduCmd, READER_APDU_Response *pApduResp){
	/* Voir ISO7816-3 section 12.2.6 */
	READER_TPDU_Command tpduCmd;
	READER_TPDU_Response tpduResp;
	READER_HAL_CommSettings *pSettings;
	READER_Status retVal;
	
	uint32_t Ne, Na, Nm, Nx;            /* Meme notation que ISO7816  */
	uint32_t i;
	uint32_t timeout;
	
	
	/* Recuperation d'un pointeur sur les parametres de comm bas niveau actuellement utilises ...  */
	retVal = READER_T0_CONTEXT_GetHalCommSettingsPtr(pContext, &pSettings);
	if(retVal != READER_OK) return retVal;
	
	/* Recuperation de la valeur de timeout actuellement utilisee dans le contexte de communication ...  */
	retVal = READER_T0_CONTEXT_GetCurrentWTMilli(pContext, &timeout);
	if(retVal != READER_OK) return retVal;
	
	Ne = pApduCmd->body.Ne;         /* Ne = N expected            */
	Nm = Ne;                        /* Nm = N remaining           */
	
	/* On envoie le premier TPDU avec le header de l'APDU et P3=0 */
	retVal = READER_TPDU_Forge(&tpduCmd, pApduCmd->header.CLA, pApduCmd->header.INS, pApduCmd->header.P1, pApduCmd->header.P2, 0, NULL, 0);
	if(retVal != READER_OK) return retVal;
	
	/* On envoie la requette TPDU */
	retVal = READER_TPDU_Send(&tpduCmd, &tpduResp, timeout, pSettings);
	if((retVal != READER_OK) && (retVal != READER_GOT_SW1)) return retVal;
	
	/* On recupere la reponse */
	if(retVal != READER_GOT_SW1){
		retVal = READER_TPDU_RcvResponse(&tpduResp, 256, timeout, pSettings);
		if((retVal != READER_OK) && (retVal != READER_TIMEOUT_GOT_ONLY_SW)) return retVal;
	}
	
	
	/* On verifie la reponse de la carte */
	if((tpduResp.SW1 == 0x67) && (tpduResp.SW2 == 0x00)){                   /* La carte a abort total a cause de wrong length */
		retVal = READER_T0_APDU_MapTpduRespToApdu(pContext, &tpduResp, pApduResp);
		if(retVal != READER_OK) return READER_ERR;
		return READER_OK;
	}
	else if(tpduResp.SW1 == 0x6C){                                          /* La carte a abort a cause de wrong lenth, mais indique Na */
		/* On forge un nouveau TPDU avec P3 = SW2  exactement comme dans le cas 2S.3*/
		Na = tpduResp.SW2;
		retVal = READER_TPDU_Forge(&tpduCmd, pApduCmd->header.CLA, pApduCmd->header.INS, pApduCmd->header.P1, pApduCmd->header.P2, READER_APDU_NeToLe(Na), NULL, 0);
		if(retVal != READER_OK) return retVal;
	
		retVal = READER_TPDU_Send(&tpduCmd, &tpduResp, timeout, pSettings);
		if((retVal != READER_OK) && (retVal != READER_GOT_SW1)) return retVal;
		
		if(retVal != READER_GOT_SW1){
			retVal = READER_TPDU_RcvResponse(&tpduResp, (Ne<Na)?Ne:Na, timeout, pSettings);    /* Voir ISO7816-3 section 12.2.3 case 2S.3 */
			if(retVal != READER_OK) return retVal;
		}
		
		retVal = READER_T0_APDU_MapTpduRespToApdu(pContext, &tpduResp, pApduResp);
		if(retVal != READER_OK) return READER_ERR;
	}
	else if((tpduResp.SW1 == 0x90) && (tpduResp.SW2 == 0x00)){
		if(tpduResp.dataSize == 256){
			retVal = READER_T0_APDU_MapTpduRespToApdu(pContext, &tpduResp, pApduResp);
			if(retVal != READER_OK) return READER_ERR;
		}
		else{
			return READER_ERR;
		}
	}
	else if(tpduResp.SW1 == 0x61){
		/* Tantque il reste des donees a recevoir on envoie des TPDU de type GET RESPONSE */
		while((Nm > 0) && (tpduResp.SW1 != 0x90)){
			Nx = READER_APDU_LeToNe(tpduResp.SW2);
			
			/* Preparation de la commande GET RESPONSE */
			retVal = READER_TPDU_Forge(&tpduCmd, pApduCmd->header.CLA, READER_APDU_INS_GETRESPONSE, 0x00, 0x00, (Nx < Nm)? Nx:Nm, NULL, 0);
			if(retVal != READER_ERR) return retVal;
			
			
			/* Execution de la commande  */
			retVal = READER_TPDU_Send(&tpduCmd, &tpduResp, timeout, pSettings);
			if((retVal != READER_OK) && (retVal != READER_GOT_SW1)) return retVal;
		
			if(retVal != READER_GOT_SW1){
				retVal = READER_TPDU_RcvResponse(&tpduResp, Nx, timeout, pSettings);    /* Voir ISO7816-3 section 12.2.3 case 2S.3 */
				if(retVal != READER_OK) return retVal;
			}
			
			
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


/**
 * \fn READER_Status READER_T0_APDU_ExecuteCase4S(READER_T0_ContextHandler *pContext, READER_APDU_Command *pApduCmd, READER_APDU_Response *pApduResp)
 * \param *pContext is a pointer to a READER_T0_ContextHandler data structure. It contains the current T=0 protocol communication state. It should have been previously initialized by the READER_T0_APDU_Init() function.
 * \param *pApduCommand is a pointer to a READER_APDU_Command data structure. It should have been previously filled with an APDU command to be sent to the card. For example with the READER_T0_APDU_Forge() function.
 * \param *pApduResp is a pointer to a READER_APDU_Response data structure. This function will fill it with the response aswered by the card.
 * \return READER_Status execution code.
 * This function executes an APDU command in the case 4S.
 */
READER_Status READER_T0_APDU_ExecuteCase4S(READER_T0_ContextHandler *pContext, READER_APDU_Command *pApduCmd, READER_APDU_Response *pApduResp){
	READER_TPDU_Command tpduCmd;
	READER_TPDU_Response tpduResp;
	READER_APDU_Command newApduCmd;
	READER_HAL_CommSettings *pSettings;
	//READER_APDU_Response newApduResp;
	READER_Status retVal;
	uint32_t Na, Ne;
	uint32_t timeout;
	

	/* Recuperation d'un pointeur sur les parametres de comm bas niveau actuellement utilises ...  */
	retVal = READER_T0_CONTEXT_GetHalCommSettingsPtr(pContext, &pSettings);
	if(retVal != READER_OK) return retVal;

	/* Recuperation de la valeur de timeout actuellement utilisee dans le contexte de communication ...  */
	retVal = READER_T0_CONTEXT_GetCurrentWTMilli(pContext, &timeout);
	if(retVal != READER_OK) return retVal;
	
	
	Ne = pApduCmd->body.Ne;
	
	/* On forge le premier TPDU avec le header de l'APDU et P3=Lc */
	retVal = READER_TPDU_Forge(&tpduCmd, pApduCmd->header.CLA, pApduCmd->header.INS, pApduCmd->header.P1, pApduCmd->header.P2, READER_APDU_NcToLc(pApduCmd->body.Nc), pApduCmd->body.dataBytes, pApduCmd->body.Nc);
	if(retVal != READER_OK) return retVal;
	
	/* On envoie le TPDU */
	retVal = READER_TPDU_Send(&tpduCmd, &tpduResp, timeout, pSettings);
	if((retVal != READER_OK) && (retVal != READER_GOT_SW1)) return retVal;
	
	
	/* On attend en reponse juste un SW (avant d'envoyer le GET RESPONSE) */
	if(retVal != READER_GOT_SW1){
		retVal = READER_TPDU_RcvResponse(&tpduResp, 0, timeout, pSettings);
		if(retVal != READER_OK) return retVal;
	}
	
	/* Selon le SW du premier TPDU response */
	/* Cas 4S.1 */
	if((tpduResp.SW1 == 0x60) || (tpduResp.SW1 == 0x64) || (tpduResp.SW1 == 0x65) || (tpduResp.SW1 == 0x66) || (tpduResp.SW1 == 0x67) || (tpduResp.SW1 == 0x68) || (tpduResp.SW1 == 0x69) || (tpduResp.SW1 == 0x6A) || (tpduResp.SW1 == 0x6B) || (tpduResp.SW1 == 0x6C) || (tpduResp.SW1 == 0x6D)  || (tpduResp.SW1 == 0x6E) || (tpduResp.SW1 == 0x6F)){
		retVal = READER_T0_APDU_MapTpduRespToApdu(pContext, &tpduResp, pApduResp);
		if(retVal != READER_OK) return READER_ERR;
		return READER_OK;
	}
	/* Cas 4S.2 */
	else if((tpduResp.SW1 == 0x90) && (tpduResp.SW2 == 0x00)){
		/* On envoie un APDU de type GET RESPONSE, on se rabat sur le cas 2S. */
		retVal = READER_APDU_Forge(&newApduCmd, pApduCmd->header.CLA, READER_APDU_INS_GETRESPONSE, 0x00, 0x00, 0, NULL, pApduCmd->body.Ne);
		if(retVal != READER_OK) return retVal;
		
		retVal = READER_T0_APDU_ExecuteCase2S(pContext, &newApduCmd, pApduResp);
		if(retVal != READER_OK) return retVal;
		
		//retVal = READER_APDU_CopyResponse(&newApduResp, pApduResp);
		//if(retVal != READER_OK) return retVal;
	}
	/* Cas 4S.3 */
	else if(tpduResp.SW1 == 0x61){
		Na = READER_APDU_LeToNe(tpduResp.SW2);
		retVal = READER_APDU_Forge(&newApduCmd, pApduCmd->header.CLA, READER_APDU_INS_GETRESPONSE, 0x00, 0x00, 0, NULL, (Ne<Na)?Na:Ne);
		if(retVal != READER_OK) return retVal;
		
		retVal = READER_T0_APDU_ExecuteCase2S(pContext, &newApduCmd, pApduResp);
		if(retVal != READER_OK) return retVal;
		
		return READER_OK;
	}
	/* Cas 4S.4 */
	else if((tpduResp.SW1 == 0x62) || (tpduResp.SW1 == 0x63) || (((tpduResp.SW1 & 0xF0) == 0x90) && (tpduResp.SW2 != 0x00)) ){
		retVal = READER_T0_APDU_MapTpduRespToApdu(pContext, &tpduResp, pApduResp);
		if(retVal != READER_OK) return READER_ERR;
		return READER_OK;
	}
	else{
		return READER_ERR;
	}
	
	return READER_OK;
}


/**
 * \fn READER_Status READER_T0_APDU_ExecuteCase4E(READER_T0_ContextHandler *pContext, READER_APDU_Command *pApduCmd, READER_APDU_Response *pApduResp)
 * \param *pContext is a pointer to a READER_T0_ContextHandler data structure. It contains the current T=0 protocol communication state. It should have been previously initialized by the READER_T0_APDU_Init() function.
 * \param *pApduCommand is a pointer to a READER_APDU_Command data structure. It should have been previously filled with an APDU command to be sent to the card. For example with the READER_T0_APDU_Forge() function.
 * \param *pApduResp is a pointer to a READER_APDU_Response data structure. This function will fill it with the response aswered by the card.
 * \return READER_Status execution code.
 * This function executes an APDU command in the case 4E.
 */
READER_Status READER_T0_APDU_ExecuteCase4E(READER_T0_ContextHandler *pContext, READER_APDU_Command *pApduCmd, READER_APDU_Response *pApduResp){
	uint32_t timeout;
	READER_HAL_CommSettings *pSettings;
	READER_Status retVal;
	
	
	/* Recuperation d'un pointeur sur les parametres de comm bas niveau actuellement utilises ...  */
	retVal = READER_T0_CONTEXT_GetHalCommSettingsPtr(pContext, &pSettings);
	if(retVal != READER_OK) return retVal;
	
	/* Recuperation de la valeur de timeout actuellement utilisee dans le contexte de communication ...  */
	retVal = READER_T0_CONTEXT_GetCurrentWTMilli(pContext, &timeout);
	if(retVal != READER_OK) return retVal;
	
	
	return READER_OK;
}


/**
 * \fn READER_Status READER_T0_APDU_MapTpduRespToApdu(READER_TPDU_Response *pTpduResp, READER_APDU_Response *pApduResp)
 * \brief Cette fonction pemret de mapper un object de type "TPDU Response" sur un objet de type "APDU Response". Attention cette fonction n'est pas prévue pour le cas où la taille des données du "APDU Response" est strictement supérieure à 256 octets (cas 2E et 4E).
 */
READER_Status READER_T0_APDU_MapTpduRespToApdu(READER_T0_ContextHandler *pContext, READER_TPDU_Response *pTpduResp, READER_APDU_Response *pApduResp){
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

