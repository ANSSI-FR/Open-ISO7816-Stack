/**
 * \file reader_apdu.c
 * \copyright This file is part of the Open-ISO7816-Stack project and is distributed under the MIT license. See LICENSE file in the root directory. 
 * This file contains functions for operating safely on READER_APDU_Command and READER_APDU_Response data structures.
 */

#include "reader_tpdu.h"
#include "reader_hal.h"
#include "reader_apdu.h"



READER_Status READER_APDU_CheckCmdValidity(READER_APDU_Command *pApduCmd){
	if(pApduCmd->body.Nc > 65535){
		return READER_ERR;
	}
	
	if(pApduCmd->body.Ne > 65536){
		return READER_ERR;
	}	
	
	/* Voir si il n'y a pas d'autres tests a rajouter ... */
	
	return READER_OK;
}

/**
 * \fn READER_APDU_ProtocolCase READER_APDU_GetProtocolCase(READER_APDU_Command *pApduCmd)
 * This function analyses an APDU_Command data structures and returns its type (cases 2E, 2S, 3E, 3S, 4E, 4S etc ...).
 * \return Returns a READER_APDU_ProtocolCase type value representing the APDU type (2E, 2S, 3E, 3S, 4E, 4S etc ...).
 * \param *pApduCmd pointer on an READER_APDU_Command structure.
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
 * \fn READER_Status READER_APDU_CopyCommand(READER_APDU_Command *pSourceApdu, READER_APDU_Command *pDestApdu)
 * Cette fonction permet de copier le contenu d'une commande APDU source dans une commande APDU de destination.
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
 * Cette fonction permet de copier le contenu d'une réponse APDU source dans une réponse APDU de destination.
 * \return Valeur de retour de type READER_Status. READER_OK indique le bon déroulement de la fonction. Toute autre valeur indique une erreur.
 * \param *pSourceApdu est un pointeur sur l'APDU source. Il s'agit d'une structure de type READER_APDU_Response. 
 * \param *pDestApdu est un pointeur sur l'APDU de destination. Il s'agit d'une structure de type READER_APDU_Response.
 */
READER_Status READER_APDU_CopyResponse(READER_APDU_Response *pSourceApdu, READER_APDU_Response *pDestApdu){
	uint32_t i;
	
	pDestApdu->SW1  = pSourceApdu->SW1;
	pDestApdu->SW2  = pSourceApdu->SW2;
	pDestApdu->dataSize  = pSourceApdu->dataSize;
	
	for(i=0; i<pSourceApdu->dataSize; i++){
		pDestApdu->dataBytes[i] = pSourceApdu->dataBytes[i];
	}
	
	return READER_OK;
}

/**
 * \fn READER_Status READER_APDU_Forge(READER_APDU_Command *pApduCmd, uint8_t CLA, uint8_t INS, uint8_t P1, uint8_t P2, uint32_t Nc, uint8_t *pData, uint32_t Ne)
 * Cette fonction permet d'initialiser une structure de type READER_APDU_Command à partir des informations fournies en paramètres.
 * \return Valeur de retour de type READER_Status. READER_OK indique le bon déroulement de la fonction. Toute autre valeur indique une erreur.
 * \param *pApduCmd est un pointeur sur la structure de type READER_APDU_Command à initialiser.
 * \param CLA est la classe de l'instruction telle que définie dans la norme ISO7816.
 * \param INS est le code de l'instruction telle que définie dans la norme ISO7816.
 * \param P1 est le premier paramètre de l'instruction tel que défini dans la norme ISO7816.
 * \param P2 est le deuxième paramètre de l'instruction tel que défini dans la norme ISO7816.
 * \param Nc est le nombre de caractères contenus dans la commande. Nc est défini dans la norme ISO7816.
 * \param pData est un pointeur vers un tableau de caractères. Celui-ci contient les données de la commande.
 * \param Ne est le nombre de caractères attendus en réponse de la commande APDU. Ne est défini dans la norme ISO7816.
 */
READER_Status READER_APDU_Forge(READER_APDU_Command *pApduCmd, uint8_t CLA, uint8_t INS, uint8_t P1, uint8_t P2, uint32_t Nc, uint8_t *pData, uint32_t Ne){
	uint32_t i;
	
	pApduCmd->header.CLA  =  CLA;
	pApduCmd->header.INS  =  INS;
	pApduCmd->header.P1   =  P1;
	pApduCmd->header.P2   =  P2;
	pApduCmd->body.Nc     =  Nc;
	pApduCmd->body.Ne     =  Ne;
	
	if((pData == NULL) && (Nc != 0)){
		return READER_ERR;
	}
	else if(pData == NULL){
		return READER_OK;
	}
	
	for(i=0; i<Nc; i++){
		pApduCmd->body.dataBytes[i] = pData[i];
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

/**
 * \fn READER_Status READER_APDU_ExtractRespSW(READER_APDU_Response *pApduResp, uint8_t *pSW1, uint8_t *pSW2)
 * \return The function returns an execution code of type READER_Status that indicates if the function behaved as expected or not.
 * \param *pApduResp is a pointer on a READER_APDU_Response data structure. This structure have to be already filled with a valid APDU Response.
 * \param *pSW1 is a pointer on a character that will be filled with SW1 by the function.
 * \param *pSW2 is a pointer on a character that will be filled with SW2 by the function.
 * This function is used to exctract status word (SW) from a READER_APDU_Response data structure.
 * This structure have to be already filled with a valid APDU Response.
 * For example after using READER_T1_APDU_Execute().
 */
READER_Status READER_APDU_ExtractRespSW(READER_APDU_Response *pApduResp, uint8_t *pSW1, uint8_t *pSW2){
	*pSW1 = pApduResp->SW1;
	*pSW2 = pApduResp->SW2;
	
	return READER_OK;
}

/**
 * \fn READER_Status READER_APDU_ExtractRespDataPtr(READER_APDU_Response *pApduResp, uint8_t **ppData, uint32_t *pDataSize)
 * \return The function returns an execution code of type READER_Status that indicates if the function behaved as expected or not.
 * \param *pApduResp is a pointer on a READER_APDU_Response data structure. This structure have to be already filled with a valid APDU Response.
 * \param **ppData is a pointer A that points another pointer B that points to a charaters buffer. The function will modify the pointer B in order to make it point on the READER_APDU_Response data field.
 * \param *pDataSize is a pointer on a variable that will be filled (by the function) with the number of characters into the READER_APDU_Response.
 * This function is used in order to obtain a pointer onto the data field of a READER_APDU_Response.
 */
READER_Status READER_APDU_ExtractRespDataPtr(READER_APDU_Response *pApduResp, uint8_t **ppData, uint32_t *pDataSize){
	*ppData = pApduResp->dataBytes;
	*pDataSize = pApduResp->dataSize;
	
	return READER_OK;
}
