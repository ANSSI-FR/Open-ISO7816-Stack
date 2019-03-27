#include "reader_t1_context_handler.h"


/* Initialisation de la structure */

READER_Status READER_T1_CONTEXT_Init(READER_T1_ContextHandler *pContext, READER_HAL_CommSettings *pSettings){
	READER_Status retVal;
	
	
	
	retVal = READER_T1_CONTEXT_InitBuffer(pContext); /* Attention, il faut que Buffer soit initialise avant les Settings (nottament a cause de SetIFSC()/UpdateIFSC()) */
	if(retVal != READER_OK) return retVal;
	
	retVal = READER_T1_CONTEXT_InitSettings(pContext, pSettings);
	if(retVal != READER_OK) return retVal;
	
	retVal = READER_T1_CONTEXT_InitSeqNums(pContext);
	if(retVal != READER_OK) return retVal;
		
	
	return READER_OK;
}


READER_Status READER_T1_CONTEXT_InitCommSettings(READER_T1_ContextHandler *pContext, READER_HAL_CommSettings *pSettings){
	READER_Status retVal;
	
	
	retVal = READER_T1_CONTEXT_SetCurrentBGT(pContext, READER_T1_CONTEXT_DEFAULT_BGT);
	if(retVal != READER_OK) return retVal;
	
	retVal = READER_T1_CONTEXT_SetCurrentBWT(pContext, READER_T1_CONTEXT_DEFAULT_BWT);
	if(retVal != READER_OK) return retVal;
	
	retVal = READER_T1_CONTEXT_SetCurrentCGT(pContext, READER_T1_CONTEXT_DEFAULT_CGT);
	if(retVal != READER_OK) return retVal;
	
	retVal = READER_T1_CONTEXT_SetCurrentCWT(pContext, READER_T1_CONTEXT_DEFAULT_CWT);
	if(retVal != READER_OK) return retVal;
	
	retVal = READER_T1_CONTEXT_SetCurrentBWI(pContext, READER_T1_CONTEXT_DEFAULT_BWI);
	if(retVal != READER_OK) return retVal;
	
	retVal = READER_T1_CONTEXT_SetCurrentCWI(pContext, READER_T1_CONTEXT_DEFAULT_CWI);
	if(retVal != READER_OK) return retVal;
	
	retVal = READER_T1_CONTEXT_SetCurrentIFSC(pContext, READER_T1_CONTEXT_DEFAULT_IFSC);
	if(retVal != READER_OK) return retVal;
	
	retVal = READER_T1_CONTEXT_SetCurrentIFSD(pContext, READER_T1_CONTEXT_DEFAULT_IFSD);
	if(retVal != READER_OK) return retVal;
	
	retVal = READER_T1_CONTEXT_SetCurrentRedundancyType(pContext, READER_T1_CONTEXT_DEFAULT_CORRCODE);
	if(retVal != READER_OK) return retVal;
	
	retVal = READER_T1_CONTEXT_ImportHalCommSettingsToContext(pContext, pSettings);
	if(retVal != READER_OK) return retVal;
	
	return READER_OK;
}


READER_Status READER_T1_CONTEXT_InitContextSettings(READER_T1_ContextHandler *pContext){
	READER_Status retVal;
	
	
	retVal = READER_T1_CONTEXT_SetRepeatCounter(pContext, 0);
	if(retVal != READER_OK) return retVal;
	
	retVal = READER_T1_CONTEXT_SetResynchCounter(pContext, 0);
	if(retVal != READER_OK) return retVal;
	
	retVal = READER_T1_CONTEXT_SetACKStatus(pContext, READER_T1_NOACK);
	if(retVal != READER_OK) return retVal;
	
	//retVal = READER_T1_CONTEXT_SetLastSent(pContext, NULL);
	//if(retVal != READER_OK) return retVal;
	//
	//retVal = READER_T1_CONTEXT_SetLastIBlockSent(pContext, NULL);
	//if(retVal != READER_OK) return retVal;
	//
	//retVal = READER_T1_CONTEXT_SetLastRcvd(pContext, NULL);
	//if(retVal != READER_OK) return retVal;
	//
	//retVal = READER_T1_CONTEXT_SetLastIBlockRcvd(pContext, NULL);
	//if(retVal != READER_OK) return retVal;
	
	retVal = READER_T1_CONTEXT_SetDeviceChainingLastBlockFlag(pContext, READER_T1_CHAINING_NO);
	if(retVal != READER_OK) return retVal;
	
	retVal = READER_T1_CONTEXT_SetDeviceChainingSituationFlag(pContext, READER_T1_CHAINING_NO);
	if(retVal != READER_OK) return retVal;
	
	retVal = READER_T1_CONTEXT_SetCardChainingLastBlockFlag(pContext, READER_T1_CHAINING_NO);
	if(retVal != READER_OK) return retVal;
	
	retVal = READER_T1_CONTEXT_SetCardChainingSituationFlag(pContext, READER_T1_CHAINING_NO);
	if(retVal != READER_OK) return retVal;
	
	retVal = READER_T1_CONTEXT_SetNoSBlockExpectedResponse(pContext);
	if(retVal != READER_OK) return retVal;
	
	retVal = READER_T1_CONTEXT_SetSBlockRequCounter(pContext, 0);
	if(retVal != READER_OK) return retVal;
	
	//retVal = READER_T1_CONTEXT_SetTickLastSentStart(pContext);
	//if(retVal != READER_OK) return retVal;
	
	pContext->lastSentExistenceFlag = READER_T1_BLOCK_EXISTS_NO;
	pContext->lastIBlockSentExistenceFlag = READER_T1_BLOCK_EXISTS_NO;
	pContext->lastRcvdExistenceFlag = READER_T1_BLOCK_EXISTS_NO;
	pContext->lastIBlockRcvdExistenceFlag = READER_T1_BLOCK_EXISTS_NO;
	
	pContext->tickLastBlockFlag = READER_T1_FLAGSTATUS_RESET;
	
	retVal = READER_T1_CONTEXT_InitRcptBuff(pContext);
	if(retVal != READER_OK) return retVal;
	
	return READER_OK;
}


READER_Status READER_T1_CONTEXT_InitSeqNums(READER_T1_ContextHandler *pContext){
	READER_Status retVal;
	
	
	retVal = READER_T1_CONTEXT_SetDeviceCompleteSeqNum(pContext, 0);
	if(retVal != READER_OK) return retVal;
	
	retVal = READER_T1_CONTEXT_SetCardCompleteSeqNum(pContext, 0);
	if(retVal != READER_OK) return retVal;
	
	
	return READER_OK;
}


READER_Status READER_T1_CONTEXT_InitSettings(READER_T1_ContextHandler *pContext){
	READER_Status retVal;
	
	
	/* On reinitialise tous les parametres de communication dans le contexte               */
	
	retVal = READER_T1_CONTEXT_InitCommSettings(pContext, pSettings);
	if(retVal != READER_OK) return retVal;
	
	retVal = READER_T1_CONTEXT_InitContextSettings(pContext);
	if(retVal != READER_OK) return retVal;
	
	return READER_OK;
}


READER_Status READER_T1_CONTEXT_InitBuffer(READER_T1_ContextHandler *pContext){
	READER_Status retVal;
	
	
	retVal = READER_T1_BUFFER_Init(pContext);
	if(retVal != READER_OK) return retVal;
	
	return READER_OK;
}


READER_Status READER_T1_CONTEXT_InitRcptBuff(READER_T1_ContextHandler *pContext){
	pContext->receptionBuff.dataSize = 0;
	
	
	return READER_OK;
}


/* Accesseurs sur les parametres actuels de communication */

/* On retourne BGT en milisecondes ...  */
READER_Status READER_T1_CONTEXT_GetCurrentBGT(READER_T1_ContextHandler *pContext, uint32_t *pBgt){
	float etuMili;
	uint32_t currentFi, currentDi, currentFreq;
	READER_Status retVal;
	
	
	/* Recuperation des valeurs necessaires pour calculer la duree d'un ETU ...  */
	retVal = READER_T1_CONTEXT_GetHalCommSettingsFi(pContext, &currentFi);
	if(retVal != READER_OK) return retVal;
	
	retVal = READER_T1_CONTEXT_GetHalCommSettingsDi(pContext, &currentDi);
	if(retVal != READER_OK) return retVal;
	
	retVal = READER_T1_CONTEXT_GetHalCommSettingsFreq(pContext, &currentFreq);
	if(retVal != READER_OK) return retVal;
	
	/* Calcul de la duree d'un ETU en milisecondes ...  */
	etuMili = READER_UTILS_ComputeEtuMiliFloat(READER_HAL_GetFi(), READER_HAL_GetDi(), READER_HAL_GetFreq());
	etuMili = MAX(etuMili, 1);  /* Pour eviter uen multipication par zero dans 'etape suivante. */

	/* Dans le contexte, le BGT est stocke en nombre de caracteres, on le convertit en millisecondes ...  */
	*pBgt = (uint32_t)((float)(pContext->currentBGT) * etuMili) + 1;
	
	//etuMili = READER_UTILS_ComputeEtuMili(READER_HAL_GetFi(), READER_HAL_GetDi(), READER_HAL_GetFreq());
	//etuMili = MAX(etuMili, 1);
	//
	//*pBgt = (pContext->currentBGT) * etuMili;
	
	return READER_OK;
}


READER_Status READER_T1_CONTEXT_GetCurrentBWT(READER_T1_ContextHandler *pContext, uint32_t *pBwt){
	*pBwt = pContext->currentBWT;
	return READER_OK;
}


READER_Status READER_T1_CONTEXT_GetCurrentCGT(READER_T1_ContextHandler *pContext, uint32_t *pCgt){
	*pCgt = pContext->currentCGT;
	return READER_OK;
}


READER_Status READER_T1_CONTEXT_GetCurrentCWT(READER_T1_ContextHandler *pContext, uint32_t *pCwt){
	*pCwt = pContext->currentCWT;
	return READER_OK;
}


READER_Status READER_T1_CONTEXT_GetCurrentCWI(READER_T1_ContextHandler *pContext, uint32_t *pCwi){
	*pCwi = pContext->currentCWI;
	return READER_OK;
}


READER_Status READER_T1_CONTEXT_GetCurrentBWI(READER_T1_ContextHandler *pContext, uint32_t *pBwi){
	*pBwi = pContext->currentBWI;
	return READER_OK;
}


READER_Status READER_T1_CONTEXT_GetCurrentIFSC(READER_T1_ContextHandler *pContext, uint32_t *pIfsc){
	/* On verifie que la valeur est valide (Voir ISO7816-3 section 11.4.2) ...  */
	if((pContext->currentIFSC >= READER_T1_MIN_IFSC_ACCEPTED) && (pContext->currentIFSC <= READER_T1_MAX_IFSC_ACCEPTED)){
		*pIfsc = pContext->currentIFSC;
		return READER_OK;
	}
	else{
		return READER_ERR;
	}
}


READER_Status READER_T1_CONTEXT_GetCurrentIFSD(READER_T1_ContextHandler *pContext, uint32_t *pIfsd){
	/* On verifie que la valeur est valide (Voir ISO7816-3 section 11.4.2) ...  */
	if((pContext->currentIFSD >= READER_T1_MIN_IFSD_ACCEPTED) && (pContext->currentIFSD <= READER_T1_MAX_IFSD_ACCEPTED)){
		*pIfsd = pContext->currentIFSD;
		return READER_OK;
	}
	else{
		return READER_ERR;
	}
}


READER_Status READER_T1_CONTEXT_GetCurrentRedundancyType(READER_T1_ContextHandler *pContext, READER_T1_RedundancyType *pRType){
	if(pContext->currentRType == READER_T1_CRC){
		*pRType = pContext->currentRType;
	}
	else if(pContext->currentRType == READER_T1_LRC){
		*pRType = pContext->currentRType;
	}
	else{
		return READER_ERR;
	}
	
	return READER_OK;
}


READER_Status READER_T1_CONTEXT_GetCurrentRedundancyLen(READER_T1_ContextHandler *pContext, uint32_t *pRedLen){
	READER_Status retVal;
	READER_T1_RedundancyType rType;
	
	
	retVal = READER_T1_CONTEXT_GetCurrentRedundancyType(pContext, &rType);
	if(retVal != READER_OK) return retVal;
	
	if(rType == READER_T1_LRC){
		*pRedLen = 1;
	}
	else if(rType == READER_T1_CRC){
		*pRedLen = 2;
	}
	else{
		return READER_BAD_VALUE;
	}
	
	return READER_OK;
}



READER_Status READER_T1_CONTEXT_SetCurrentBGT(READER_T1_ContextHandler *pContext, uint32_t bgt){
	pContext->currentBGT = bgt;
	return READER_OK;
}


READER_Status READER_T1_CONTEXT_SetCurrentBWT(READER_T1_ContextHandler *pContext, uint32_t bwt){
	pContext->currentBWT = bwt;
	return READER_OK;
}


READER_Status READER_T1_CONTEXT_SetCurrentCGT(READER_T1_ContextHandler *pContext, uint32_t cgt){
	pContext->currentCGT = cgt;
	return READER_OK;
}


READER_Status READER_T1_CONTEXT_SetCurrentCWT(READER_T1_ContextHandler *pContext, uint32_t cwt){
	pContext->currentCWT = cwt;
	return READER_OK;
}


READER_Status READER_T1_CONTEXT_SetCurrentCWI(READER_T1_ContextHandler *pContext, uint32_t cwi){
	pContext->currentCWI = cwi;
	return READER_OK;
}


READER_Status READER_T1_CONTEXT_SetCurrentBWI(READER_T1_ContextHandler *pContext, uint32_t bwi){
	pContext->currentBWI = bwi;
	return READER_OK;
}


READER_Status READER_T1_CONTEXT_SetCurrentIFSC(READER_T1_ContextHandler *pContext, uint32_t ifsc){
	READER_Status retVal;
	
	
	/* On verifie la validite de la valeur de IFSC que l'on veut appliquer ...  */
	if((ifsc >= READER_T1_MIN_IFSC_ACCEPTED) && (ifsc <= READER_T1_MAX_IFSC_ACCEPTED)){   /* Voir ISO7816-3 section 11.4.2 */
		retVal = READER_T1_BUFFER_UpdateIfsc(pContext, ifsc);
		if(retVal != READER_OK) return retVal;
		
		pContext->currentIFSC = ifsc;
	}
	else{
		return READER_BAD_VALUE;
	}
	
	return READER_OK;
}


READER_Status READER_T1_CONTEXT_SetCurrentIFSD(READER_T1_ContextHandler *pContext, uint32_t ifsd){
	/* On verifie la validite de la valeur de IFSD que l'on veut appliquer ...  */
	if((ifsd >= READER_T1_MIN_IFSD_ACCEPTED) && (ifsd <= READER_T1_MAX_IFSD_ACCEPTED)){   /* Voir ISO7816-3 section 11.4.2 */
		pContext->currentIFSD = ifsd;
	}
	else{
		return READER_BAD_VALUE;
	}
	
	return READER_OK;
}


READER_Status READER_T1_CONTEXT_SetCurrentRedundancyType(READER_T1_ContextHandler *pContext, READER_T1_RedundancyType rType){
	if(rType == READER_T1_CRC){
		pContext->currentRType = rType;
	}
	else if(rType == READER_T1_LRC){
		pContext->currentRType = rType;
	}
	else{
		return READER_ERR;
	}
	
	return READER_OK;
}



/* Manipulation des compteurs de redemande d'infos et de demandes de resynchro ... */


READER_Status READER_T1_CONTEXT_GetRepeatCounter(READER_T1_ContextHandler *pContext, uint32_t *pCounter){
	*pCounter = pContext->repeatCounter;
	return READER_OK;
}


READER_Status READER_T1_CONTEXT_SetRepeatCounter(READER_T1_ContextHandler *pContext, uint32_t counter){
	pContext->repeatCounter = counter;
	return READER_OK;
}








READER_Status READER_T1_CONTEXT_GetResynchCounter(READER_T1_ContextHandler *pContext, uint32_t *pCounter){
	*pCounter = pContext->resynchCounter;
	return READER_OK;
}


READER_Status READER_T1_CONTEXT_SetResynchCounter(READER_T1_ContextHandler *pContext, uint32_t counter){
	pContext->resynchCounter = counter;
	return READER_OK;
}





READER_Status READER_T1_CONTEXT_GetACKStatus(READER_T1_ContextHandler *pContext, READER_T1_ACKStatus *pACKStatus){
	if(pContext->ACKStatus == READER_T1_ACK){
		*pACKStatus = READER_T1_ACK;
	}
	else if(pContext->ACKStatus == READER_T1_NOACK){
		*pACKStatus = READER_T1_NOACK;
	}
	else{
		return READER_ERR;
	}
	
	return READER_OK;
}


READER_Status READER_T1_CONTEXT_SetACKStatus(READER_T1_ContextHandler *pContext, READER_T1_ACKStatus ACKStatus){
	if(ACKStatus == READER_T1_ACK){
		pContext->ACKStatus = READER_T1_ACK;
	}
	else if(ACKStatus == READER_T1_NOACK){
		pContext->ACKStatus = READER_T1_NOACK;
	}
	else{
		return READER_ERR;
	}
	
	return READER_OK;
}


/* Manipulation des derniers blocs envoyes/recus                                                                                      */


READER_Status READER_T1_CONTEXT_GetLastSent(READER_T1_ContextHandler *pContext, READER_T1_Block **ppBlockDest){
	READER_Status retVal;
	
	
	/* On verifie qu'il existe effectivement un dernier Block envoye ...  */
	retVal = READER_T1_CONTEXT_LastSentExists(pContext);
	if(retVal != READER_OK) return retVal;
	
	/* On positionne le pointeur sur le dernier Block envoye ...  */
	*ppBlockDest = &(pContext->lastSent);
	
	return READER_OK;
}


READER_Status READER_T1_CONTEXT_GetLastIBlockSent(READER_T1_ContextHandler *pContext, READER_T1_Block **ppBlockDest){
	READER_T1_BlockType bType;
	READER_Status retVal;
	
	
	/* On verifie qu'il existe effectivement un dernier I-Block envoye ...  */
	retVal = READER_T1_CONTEXT_LastIBlockSentExists(pContext);
	if(retVal == READER_DOESNT_EXIST) return READER_DOESNT_EXIST;
	if(retVal != READER_OK) return retVal;
	
	/* Si ce Block existe, on verifie son type avant de le servir ...  */
	bType = READER_T1_GetBlockType(&(pContext->lastIBlockSent));
	if(bType != READER_T1_IBLOCK){
		return READER_ERR;
	}
	
	/* Si tout est bon, on positionne le pointeur sur le bon Block ...  */
	*ppBlockDest = &(pContext->lastIBlockSent);
	
	return READER_OK;
}


READER_Status READER_T1_CONTEXT_GetLastRcvd(READER_T1_ContextHandler *pContext, READER_T1_Block **ppBlockDest){
	READER_Status retVal; 
	
	
	/* On verifie qu'il existe un dernier Block recu ...  */
	retVal = READER_T1_CONTEXT_LastRcvdExists(pContext);
	if(retVal != READER_OK) return retVal;
	
	/* On positionne le pointeur sur le dernier Block recu ...  */
	*ppBlockDest = &(pContext->lastRcvd);
	
	return READER_OK;
}


READER_Status READER_T1_CONTEXT_GetLastIBlockRcvd(READER_T1_ContextHandler *pContext, READER_T1_Block **ppBlockDest){
	READER_T1_BlockType bType;
	READER_Status retVal;
	
	
	/* On verifie qu'il existe un dernier I-Block recu ...  */
	retVal = READER_T1_CONTEXT_LastIBlockRcvdExists(pContext);
	if(retVal != READER_OK) return retVal;
	
	/* On verifie que c'est bien un I-Block ...  */
	bType = READER_T1_GetBlockType(&(pContext->lastIBlockRcvd));
	if(bType != READER_T1_IBLOCK){
		return READER_ERR;
	}
	
	/* Si tout est bon, on positionne le pointeur sur le dernier I-Block recu qui se trouve dans le Contexte ...  */
	*ppBlockDest = &(pContext->lastIBlockRcvd);
	
	return READER_OK;
}


/**
 * \fn READER_Status READER_T1_CONTEXT_GetLastIBlockSentSeqNum(READER_T1_ContextHandler *pContext, uint32_t *pSeqNum)
 * \brief Cette fonction permet de connaitre (à partir du contexte de communication) le numéro de séquence du dernier I-Block envoyé.
 * \param *pContext est un pointeur sur une structure de type READER_T1_ContextHandler. La structure pointée stocke le contexte actuel de communication.
 * \param *pSeqNum est un pointeur sur un uint32_t. Ce pointeur permet à la fonction de retouner le résultat (0 ou 1).
 * \return La fonction retourne un code d'erreur du type READER_Status. READER_OK indique le bon déroulement. READER_DOESNT_EXIST indique que le dernier I-Block envoyé n'existe pas.
 */
READER_Status READER_T1_CONTEXT_GetLastIBlockSentSeqNum(READER_T1_ContextHandler *pContext, uint32_t *pSeqNum){
	READER_Status retVal;
	READER_T1_Block *pLastBlock;
	READER_T1_SeqNumber tmpSeqNum;
	
	
	/* On recupere le dernier I-Block envoye                                 */
	retVal = READER_T1_CONTEXT_GetLastIBlockSent(pContext, &pLastBlock);
	if(retVal == READER_DOESNT_EXIST) return READER_DOESNT_EXIST;
	if(retVal != READER_OK) return retVal;
	
	/* On recupere le numero de sequence                                     */
	tmpSeqNum = READER_T1_GetBlockSeqNumber(pLastBlock);
	if(tmpSeqNum == READER_T1_SEQNUM_ONE){
		*pSeqNum = 1;
	}
	else if(tmpSeqNum == READER_T1_SEQNUM_ZERO){
		*pSeqNum = 0;
	}
	else{
		return READER_ERR;
	}
	
	return READER_OK;
}



/**
 * \fn READER_Status READER_T1_CONTEXT_GetLastIBlockRcvdSeqSum(READER_T1_ContextHandler *pContext, uint32_t *pSeqNum)
 * \brief Cette fonction permet de connaitre (à partir du contexte de communication) le numéro de séquence du dernier I-Block reçu.
 * \param *pContext est un pointeur sur une structure de type READER_T1_ContextHandler. La structure pointée stocke le contexte actuel de communication.
 * \param *pSeqNum est un pointeur sur un uint32_t. Ce pointeur permet à la fonction de retouner le résultat (0 ou 1).
 * \return La fonction retourne un code d'erreur du type READER_Status. READER_OK indique le bon déroulement. READER_DOESNT_EXIST indique que le dernier I-Block recu n'existe pas.
 */
READER_Status READER_T1_CONTEXT_GetLastIBlockRcvdSeqSum(READER_T1_ContextHandler *pContext, uint32_t *pSeqNum){
	READER_Status retVal;
	READER_T1_Block *pLastBlock;
	READER_T1_SeqNumber tmpSeqNum;
	
	
	/* On recupere le dernier I-Block recu                                   */
	retVal = READER_T1_CONTEXT_GetLastIBlockRcvd(pContext, &pLastBlock);
	if(retVal != READER_OK) return retVal;
	
	/* On recupere le numero de sequence                                     */
	tmpSeqNum = READER_T1_GetBlockSeqNumber(pLastBlock);
	if(tmpSeqNum == READER_T1_SEQNUM_ONE){
		*pSeqNum = 1;
	}
	else if(tmpSeqNum == READER_T1_SEQNUM_ZERO){
		
		*pSeqNum = 0;
	}
	else{
		return READER_ERR;
	}
	
	return READER_OK;
}



/* On retourne READER_OK si existe, READER_DOESNT_EXIST so n'existe pas et READER_ERR en cas d'erreur interne ...  */
/**
 * \fn READER_Status READER_T1_CONTEXT_LastSentExists(READER_T1_ContextHandler *pContext)
 * \brief Cette fonction permet de savoir (à partir du contexte de communication) si il existe un dernier Block envoyé (est ce que on a deja envoyé un Block).
 * \param *pContext est un pointeur sur une structure de type READER_T1_ContextHandler. La structure pointée stocke le contexte actuel de communication.
 * \return La fonction retourne un code d'erreur du type READER_Status. READER_OK indique le bon déroulement. READER_DOESNT_EXIST indique que le dernier Block envoyé n'existe pas.
 */
READER_Status READER_T1_CONTEXT_LastSentExists(READER_T1_ContextHandler *pContext){
	if(pContext->lastSentExistenceFlag == READER_T1_BLOCK_EXISTS_YES){
		return READER_OK;
	}
	else if(pContext->lastSentExistenceFlag == READER_T1_BLOCK_EXISTS_NO){
		return READER_DOESNT_EXIST;
	}
	else{
		return READER_ERR;
	}	
}


/**
 * \fn READER_Status READER_T1_CONTEXT_LastIBlockSentExists(READER_T1_ContextHandler *pContext)
 * \brief Cette fonction permet de savoir (à partir du contexte de communication) si il existe un dernier I-Block envoyé (est ce que on a deja envoyé un I-Block).
 * \param *pContext est un pointeur sur une structure de type READER_T1_ContextHandler. La structure pointée stocke le contexte actuel de communication.
 * \return La fonction retourne un code d'erreur du type READER_Status. READER_OK indique le bon déroulement. READER_DOESNT_EXIST indique que le dernier I-Block envoyé n'existe pas.
 */
/* On retourne READER_OK si existe, READER_DOESNT_EXIST so n'existe pas et READER_ERR en cas d'erreur interne ...  */
READER_Status READER_T1_CONTEXT_LastIBlockSentExists(READER_T1_ContextHandler *pContext){
	if(pContext->lastIBlockSentExistenceFlag == READER_T1_BLOCK_EXISTS_YES){
		return READER_OK;
	}
	else if(pContext->lastIBlockSentExistenceFlag == READER_T1_BLOCK_EXISTS_NO){
		return READER_DOESNT_EXIST;
	}
	else{
		return READER_ERR;
	}	
}


/* On retourne READER_OK si existe, READER_DOESNT_EXIST si n'existe pas et READER_ERR en cas d'erreur interne ...  */
/**
 * \fn READER_Status READER_T1_CONTEXT_LastRcvdExists(READER_T1_ContextHandler *pContext)
 * \brief Cette fonction permet de savoir (à partir du contexte de communication) si il existe un dernier Block recu (est ce que on a deja recu un Block).
 * \param *pContext est un pointeur sur une structure de type READER_T1_ContextHandler. La structure pointée stocke le contexte actuel de communication.
 * \return La fonction retourne un code d'erreur du type READER_Status. READER_OK indique le bon déroulement. READER_DOESNT_EXIST indique que le dernier Block recu n'existe pas.
 */
READER_Status READER_T1_CONTEXT_LastRcvdExists(READER_T1_ContextHandler *pContext){
	if(pContext->lastRcvdExistenceFlag == READER_T1_BLOCK_EXISTS_YES){
		return READER_OK;
	}
	else if(pContext->lastRcvdExistenceFlag == READER_T1_BLOCK_EXISTS_NO){
		return READER_DOESNT_EXIST;
	}
	else{
		return READER_ERR;
	}	
}


/* On retourne READER_OK si existe, READER_DOESNT_EXIST so n'existe pas et READER_ERR en cas d'erreur interne ...  */
/**
 * \fn READER_Status READER_T1_CONTEXT_LastIBlockRcvdExists(READER_T1_ContextHandler *pContext)
 * \brief Cette fonction permet de savoir (à partir du contexte de communication) si il existe un dernier I-Block recu (est ce que on a deja recu un I-Block).
 * \param *pContext est un pointeur sur une structure de type READER_T1_ContextHandler. La structure pointée stocke le contexte actuel de communication.
 * \return La fonction retourne un code d'erreur du type READER_Status. READER_OK indique le bon déroulement. READER_DOESNT_EXIST indique que le dernier I-Block recu n'existe pas.
 */
READER_Status READER_T1_CONTEXT_LastIBlockRcvdExists(READER_T1_ContextHandler *pContext){
	if(pContext->lastIBlockRcvdExistenceFlag == READER_T1_BLOCK_EXISTS_YES){
		return READER_OK;
	}
	else if(pContext->lastIBlockRcvdExistenceFlag == READER_T1_BLOCK_EXISTS_NO){
		return READER_DOESNT_EXIST;
	}
	else{
		return READER_ERR;
	}	
}



/**
 * \fn READER_Status READER_T1_CONTEXT_SetLastSent(READER_T1_ContextHandler *pContext, READER_T1_Block *pBlock)
 * \brief Cette fonction permet de mettre à jour le contexte de communication avec le dernier Block envoyé par le DEVICE.
 * \param *pContext est un pointeur sur une structure de type READER_T1_ContextHandler. La structure pointée stocke le contexte actuel de communication (celui qu'il faut mettre à jour).
 * \param *pBlock est un pointeur sur une structure de type READER_T1_Block. Il s'agit du dernier Block qui vient d'être envoyé. Ce Block sera recopié dans le contexte.
 * \return La fonction retourne un code d'erreur de type READER_Status. READER_OK indique le bon déroulement de la fonction.
 */
READER_Status READER_T1_CONTEXT_SetLastSent(READER_T1_ContextHandler *pContext, READER_T1_Block *pBlock){
	READER_Status retVal;
	
	/* On verifie les parametres ...  */
	if(pBlock == NULL){
		return READER_ERR;
	}
	
	/* On recopie le Block dans le contexte de communication ...  */
	retVal = READER_T1_CopyBlock(&(pContext->lastSent), pBlock);
	if(retVal != READER_OK) return retVal;
	
	/* On mets a jour le flag d'existance de ce Block dans le contexte de communication ... */
	pContext->lastSentExistenceFlag = READER_T1_BLOCK_EXISTS_YES;
	
	return READER_OK;
}


/**
 * \fn READER_Status READER_T1_CONTEXT_SetLastIBlockSent(READER_T1_ContextHandler *pContext, READER_T1_Block *pBlock)
 * \brief Cette fonction permet de mettre à jour le contexte de communication avec le dernier I-Block envoyé par le DEVICE.
 * \param *pContext est un pointeur sur une structure de type READER_T1_ContextHandler. La structure pointée stocke le contexte actuel de communication (celui qu'il faut mettre à jour).
 * \param *pBlock est un pointeur sur une structure de type READER_T1_Block. Il s'agit du dernier Block qui vient d'être envoyé (à priori un I-Block). Ce Block sera recopié dans le contexte.
 * \return La fonction retourne un code d'erreur de type READER_Status. READER_OK indique le bon déroulement de la fonction.
 */
READER_Status READER_T1_CONTEXT_SetLastIBlockSent(READER_T1_ContextHandler *pContext, READER_T1_Block *pBlock){
	READER_Status retVal;
	
	
	/* On verifie les perametres ...  */
	if(pBlock == NULL){
		return READER_ERR;
	}
	
	/* On recopie le Block dans le contexte de communication ...  */
	retVal = READER_T1_CopyBlock(&(pContext->lastIBlockSent), pBlock);
	if(retVal != READER_OK) return retVal;
	
	/* On mets a jour le flag qui indique l'existance de ce Block dans le contexte de communication ...  */
	pContext->lastIBlockSentExistenceFlag = READER_T1_BLOCK_EXISTS_YES;
	
	return READER_OK;
}


/**
 * \fn READER_Status READER_T1_CONTEXT_SetLastIBlockRcvd(READER_T1_ContextHandler *pContext, READER_T1_Block *pBlock)
 * \brief Cette fonction permet d'indiquer dans le contexte de communication le dernier I-Block qui a été recu pr le DEVICE. Ce Block sera recopié dans le contexte de communication.
 * \param *pContext est un pointeur sur une structure de type READER_T1_ContextHandler. La structure pointée contient tout le contexte de communication courant.
 * \param *pBlock est un pointeur sur une structure de type READER_T1_Block. Il pointe sur le dernier I-Block recu par le DEVICE. Ce Bloc sera recopié dans le contexte de communication.
 * \return La fonction retourne un code d'erreur du type READER_Status. READER_OK indique le bon déroulement. 
 */
READER_Status READER_T1_CONTEXT_SetLastIBlockRcvd(READER_T1_ContextHandler *pContext, READER_T1_Block *pBlock){
	READER_Status retVal;
	
	
	/* On verifie les parametres ...  */
	if(pBlock == NULL){
		return READER_ERR;
	}

	/* On recopie le Block dans le contexte de communication ...  */
	retVal = READER_T1_CopyBlock(&(pContext->lastIBlockRcvd), pBlock);
	if(retVal != READER_OK) return retVal;
	
	/* On mets a jour le Flag qui indique l'existance de ce Block ...  */
	pContext->lastIBlockRcvdExistenceFlag = READER_T1_BLOCK_EXISTS_YES;
	
	return READER_OK;
}


/**
 * \fn READER_Status READER_T1_CONTEXT_SetLastRcvd(READER_T1_ContextHandler *pContext, READER_T1_Block *pBlock)
 * \brief Cette fonction permet d'indiquer dans le contexte de communication le dernier Block qui a été recu pr le DEVICE. Ce Block sera recopié dans le contexte de communication.
 * \param *pContext est un pointeur sur une structure de type READER_T1_ContextHandler. La structure pointée contient tout le contexte de communication courant.
 * \param *pBlock est un pointeur sur une structure de type READER_T1_Block. Il pointe sur le dernier Block recu par le DEVICE. Ce Bloc sera recopié dans le contexte de communication.
 * \return La fonction retourne un code d'erreur du type READER_Status. READER_OK indique le bon déroulement. 
 */
READER_Status READER_T1_CONTEXT_SetLastRcvd(READER_T1_ContextHandler *pContext, READER_T1_Block *pBlock){
	READER_Status retVal;
	
	
	/* On verifie les parametres ...  */
	if(pBlock == NULL){
		return READER_ERR;
	}
	
	/* On recopie le Block dans le contexte de communication ...  */
	retVal = READER_T1_CopyBlock(&(pContext->lastRcvd), pBlock);
	if(retVal != READER_OK) return retVal;
	
	/* On mets a jour le Flag qui inique l'existance de ce Block ... */
	pContext->lastRcvdExistenceFlag = READER_T1_BLOCK_EXISTS_YES;
	
	return READER_OK;
}


/* retourne une valeur autre que READER_OK si le dernier Block envoye n'existe pas ou si la fontion a rencontre une erreur */
/**
 * \fn READER_Status READER_T1_CONTEXT_GetLastSentType(READER_T1_ContextHandler *pContext, READER_T1_BlockType *pType)
 * \brief Cete fonction permet de déterminer (à partir du contexte de communication) le type du dernier Block qui à été envoyé par le DEVICE.
 * \param *pContext est un pointeur sur une structure de type READER_T1_ContextHandler. La structure pointée contient tout le contexte de communication courant.
 * \param *pType est un pointeur sur une structure du type READER_T1_BlockType. Il permet à la fonction de retourner le type du Block.
 * \return La fonction retourne un code d'erreur du type READER_Status. READER_OK indique le bon déroulement. READER_DOESNT_EXIST indique que le dernier Block envoyé n'existe pas.
 */
READER_Status READER_T1_CONTEXT_GetLastSentType(READER_T1_ContextHandler *pContext, READER_T1_BlockType *pType){
	READER_T1_BlockType bType;
	READER_T1_Block *pLastBlock;
	READER_Status retVal;
	
	
	/* On recupere un pointeur sur le dernier Block envoye           */
	retVal = READER_T1_CONTEXT_GetLastSent(pContext, &pLastBlock);
	if(retVal != READER_OK) return retVal;
	
	/* On recupere le Type du dernier Block Envoye */
	bType = READER_T1_GetBlockType(pLastBlock);
	
	if(bType == READER_T1_BLOCK_ERR){
		return READER_ERR;
	}
	
	*pType = bType;

	return READER_OK;
}


/* retourne une valeur autre que READER_OK si le dernier Block recu n'existe pas ou si la fontion a rencontre une erreur */
/**
 * \fn READER_Status READER_T1_CONTEXT_GetLastRcvdType(READER_T1_ContextHandler *pContext, READER_T1_BlockType *pType)
 * \brief Cete fonction permet de déterminer (à partir du contexte de communication) le type du dernier Block qui à été recu par le DEVICE.
 * \param *pContext est un pointeur sur une structure de type READER_T1_ContextHandler. La structure pointée contient tout le contexte de communication courant.
 * \param *pType est un pointeur sur une structure du type READER_T1_BlockType. Il permet à la fonction de retourner le type du Block.
 * \return La fonction retourne un code d'erreur du type READER_Status. READER_OK indique le bon déroulement. READER_DOESNT_EXIST indique que le dernier Block recu n'existe pas.
 */
READER_Status READER_T1_CONTEXT_GetLastRcvdType(READER_T1_ContextHandler *pContext, READER_T1_BlockType *pType){
	READER_T1_BlockType bType;
	READER_T1_Block *pLastBlock;
	READER_Status retVal;
	
	
	/* On recupere un pointeur sur le dernier Block recu              */
	retVal = READER_T1_CONTEXT_GetLastRcvd(pContext, &pLastBlock);
	if(retVal != READER_OK) return retVal;
	
	/* On recupere le Type du dernier Block Recu                      */
	bType = READER_T1_GetBlockType(pLastBlock);
	
	if(bType == READER_T1_BLOCK_ERR){
		return READER_ERR;
	}
	
	*pType = bType;

	return READER_OK;
}





/* Manipulation des numeros de sequence */
READER_Status READER_T1_CONTEXT_GetDeviceCompleteSeqNum(READER_T1_ContextHandler *pContext, uint32_t *pSeqNum){
	*pSeqNum = pContext->deviceCompleteSeqNum;
	return READER_OK;
}


READER_Status READER_T1_CONTEXT_SetDeviceCompleteSeqNum(READER_T1_ContextHandler *pContext, uint32_t seqNum){
	pContext->deviceCompleteSeqNum = seqNum;
	return READER_OK;
}


READER_Status READER_T1_CONTEXT_IncDeviceCompleteSeqNum(READER_T1_ContextHandler *pContext){
	/* A priori si on overflotte c'est pas grave, N(S) reste inchange */
	pContext->deviceCompleteSeqNum = pContext->deviceCompleteSeqNum + 1;
	return READER_OK;
}


READER_Status READER_T1_CONTEXT_GetCardCompleteSeqNum(READER_T1_ContextHandler *pContext, uint32_t *pSeqNum){
	*pSeqNum = pContext->cardCompleteSeqNum;
	return READER_OK;
}


READER_Status READER_T1_CONTEXT_SetCardCompleteSeqNum(READER_T1_ContextHandler *pContext, uint32_t seqNum){
	pContext->cardCompleteSeqNum = seqNum;
	return READER_OK;
}


READER_Status READER_T1_CONTEXT_IncCardCompleteSeqNum(READER_T1_ContextHandler *pContext){
	/* A priori si on overflotte c'est pas grave, N(S) reste inchange */
	pContext->cardCompleteSeqNum = pContext->cardCompleteSeqNum + 1;
	return READER_OK;
}


READER_Status READER_T1_CONTEXT_GetCardSeqNum(READER_T1_ContextHandler *pContext, uint32_t *pSeqNum){
	*pSeqNum = (pContext->cardCompleteSeqNum) & 0x00000001;
	
	if((*pSeqNum != 0) && (*pSeqNum != 1)){
		return READER_ERR;
	}
	
	return READER_OK;
}


READER_Status READER_T1_CONTEXT_GetDeviceSeqNum(READER_T1_ContextHandler *pContext, uint32_t *pSeqNum){
	*pSeqNum = (pContext->deviceCompleteSeqNum) & 0x00000001;
	
	if((*pSeqNum != 0) && (*pSeqNum != 1)){
		return READER_ERR;
	}
	
	return READER_OK;
}


READER_Status READER_T1_CONTEXT_ComputeNextDeviceSeqNum(READER_T1_ContextHandler *pContext, uint32_t *pSeqNum){
	uint32_t deviceSeqNum, nextDeviceSeqNum;
	READER_Status retVal;
	
	
	retVal = READER_T1_CONTEXT_GetDeviceCompleteSeqNum(pContext, &deviceSeqNum);
	if(retVal != READER_OK) return retVal;
	
	nextDeviceSeqNum = (deviceSeqNum + 1) & 0x00000001;     /* Numero de sequence modulo 2 ... */
	
	*pSeqNum = nextDeviceSeqNum;
	
	return READER_OK;
}


READER_Status READER_T1_CONTEXT_ComputeNextCardSeqNum(READER_T1_ContextHandler *pContext, uint32_t *pSeqNum){
	uint32_t cardSeqNum, nextCardSeqNum;
	READER_Status retVal;
	
	
	retVal = READER_T1_CONTEXT_GetCardCompleteSeqNum(pContext, &cardSeqNum);
	if(retVal != READER_OK) return retVal;
	
	//nextCardSeqNum = (cardSeqNum + 1) & 0x00000001;     /* Numero de sequence modulo 2 ... */
	nextCardSeqNum = (cardSeqNum) & 0x00000001;     /* Numero de sequence modulo 2 ... */
	
	*pSeqNum = nextCardSeqNum;
	
	return READER_OK;
}



/**
 * \fn READER_Status READER_T1_CONTEXT_GetCardExpectedSeqNum(READER_T1_ContextHandler *pContext, uint32_t *pSeqNum)
 * \brief Cette fonction permet de calculer (à partir du contexte de communication) le numero de sequence attendu par le DEVICE pour le prochain I-Block en provenance de la CARTE.
 * \param *pContext est un pointeur sur une structure de type READER_T1_ContextHandler. La structure pointée contient tout le contexte de communication courant.
 * \param *pSeqNum est un pointeur sur un uint32_t. Il permet à la fonction de retourner le numero de sequence (0 ou 1).
 * \return La fonction retourne un code d'erreur de type READER_Status. READER_OK indique que la fonction s'est correctement déroulée.
 */
READER_Status READER_T1_CONTEXT_GetCardExpectedSeqNum(READER_T1_ContextHandler *pContext, uint32_t *pSeqNum){
	uint32_t seqNum;
	READER_Status retVal;
	
	
	retVal = READER_T1_CONTEXT_GetCardSeqNum(pContext, &seqNum);
	//retVal = READER_T1_CONTEXT_ComputeNextCardSeqNum(pContext, &seqNum);
	if(retVal != READER_OK) return retVal;
	
	*pSeqNum = seqNum;
	
	return READER_OK;
}


/* Gestion du chainage */


/**
 * \fn READER_Status READER_T1_CONTEXT_CardIsChainingLastBlock(READER_T1_ContextHandler *pContext, READER_T1_ChainingStatus *pChainingStatus)
 * \brief Cette fonction permet de savoir si le dernier I-Block envoye par le DEVICE indique un chainage ou non.
 * \param *pContext est un pointeur sur une structure de type READER_T1_ContextHandler. La structure pointée contient tout le contexte de communication courant.
 * \param *pChainingStatus est un pointeur sur une struture de type READER_T1_ChainingStatus. La fonction utilise ce pointeur pour retourner l'état de chainage.
 * \return La fonction retourne un code d'erreur de type READER_Status. READER_OK indique que la fonction s'est correctement déroulée.
 */
READER_Status READER_T1_CONTEXT_DeviceIsChainingLastBlock(READER_T1_ContextHandler *pContext, READER_T1_ChainingStatus *pChainingStatus){
	READER_T1_ChainingStatus status;
	
	
	status = pContext->deviceIsChainingLastBlock;
	
	if(status == READER_T1_CHAINING_YES){
		*pChainingStatus = READER_T1_CHAINING_YES;
	}
	else if(status == READER_T1_CHAINING_NO){
		*pChainingStatus = READER_T1_CHAINING_NO;
	}
	else{
		return READER_BAD_VALUE;
	}
	
	return READER_OK;
	
	//READER_Status retVal;
	//READER_T1_Block *pBlock;
	//READER_T1_MBit mBit;
	//
	//
	///* On recupere a partir du context handler un pointeur sur le dernier I-Block qu'on a envoye */
	//retVal = READER_T1_CONTEXT_GetLastIBlockSent(pContext, &pBlock);
	//if((retVal != READER_OK) && (retVal != READER_DOESNT_EXIST)) return retVal;
	//
	//if(retVal == READER_DOESNT_EXIST){
	//	*pChainingStatus = READER_T1_CHAINING_NO;
	//}
	//
	///* On recupere le M-Bit du dernier I-Block ... */
	//mBit = READER_T1_GetBlockMBit(pBlock);
	//
	//if(mBit == READER_T1_MBIT_ZERO){
	//	*pChainingStatus = READER_T1_CHAINING_NO;
	//}
	//else if(mBit == READER_T1_MBIT_ONE){
	//	*pChainingStatus = READER_T1_CHAINING_YES;
	//}
	//else{
	//	return READER_ERR;
	//}
	//
	//return READER_OK;
}



/**
 * \fn READER_Status READER_T1_CONTEXT_CardIsChainingLastBlock(READER_T1_ContextHandler *pContext, READER_T1_ChainingStatus *pChainingStatus)
 * \brief Cette fonction permet de savoir si le dernier I-Block recu en provenance de la CARTE indique un chainage ou non.
 * \param *pContext est un pointeur sur une structure de type READER_T1_ContextHandler. La structure pointée contient tout le contexte de communication courant.
 * \param *pChainingStatus est un pointeur sur une struture de type READER_T1_ChainingStatus. La fonction utilise ce pointeur pour retourner l'état de chainage.
 * \return La fonction retourne un code d'erreur de type READER_Status. READER_OK indique que la fonction s'est correctement déroulée.
 */
READER_Status READER_T1_CONTEXT_CardIsChainingLastBlock(READER_T1_ContextHandler *pContext, READER_T1_ChainingStatus *pChainingStatus){
	//READER_Status retVal;
	READER_T1_ChainingStatus status;
	//READER_T1_Block *pBlock;
	//READER_T1_MBit mBit;
	
	
	status = pContext->cardIsChainingLastBlock;
	
	if(status == READER_T1_CHAINING_YES){
		*pChainingStatus = READER_T1_CHAINING_YES;
	}
	else if(status == READER_T1_CHAINING_NO){
		*pChainingStatus = READER_T1_CHAINING_NO;
	}
	else{
		return READER_BAD_VALUE;
	}
	
	
	return READER_OK;
	
	///* On recupere un pointeur sur le dernier I-Block qu'on a recu de la carte */
	//retVal = READER_T1_CONTEXT_GetLastIBlockRcvd(pContext, &pBlock);
	//if((retVal != READER_OK) && (retVal != READER_DOESNT_EXIST)) return retVal;
	//
	///* On verifie qu'il existe effectivement un dernier I-Block Recu */
	//if(retVal == READER_DOESNT_EXIST){
	//	*pChainingStatus = READER_T1_CHAINING_NO;
	//}
	//
	///* On recupere le mBit du dernier I-Block recu */
	//mBit = READER_T1_GetBlockMBit(pBlock);
	//
	//if(mBit == READER_T1_MBIT_ZERO){
	//	*pChainingStatus = READER_T1_CHAINING_NO;
	//}
	//else if(mBit == READER_T1_MBIT_ONE){
	//	*pChainingStatus = READER_T1_CHAINING_YES;
	//}
	//else{
	//	return READER_ERR;
	//}
	//
	//return READER_OK;
}


/**
 * \fn READER_Status READER_T1_CONTEXT_DeviceIsChaining(READER_T1_ContextHandler *pContext, READER_T1_ChainingStatus *pChainingStatus)
 * \brief Cette fonction permet de déterminer (a partir du contexte de communication) si le DEVICE est dans une situation de chainage.
 * Il s'agit là de la "situation globale de chainage". C'est plus large que le M-Bit du dernier I-Block recu.
 * \param *pContext est un pointeur sur une structure de type READER_T1_ContextHandler. La structure pointée contient tout le contexte de communication courant.
 * \param *pChainingStatus est un pointeur sur une struture de type READER_T1_ChainingStatus. La fonction utilise ce pointeur pour retourner l'état de chainage.
 * \return La fonction retourne un code d'erreur de type READER_Status. READER_OK indique que la fonction s'est correctement déroulée.
 */
READER_Status READER_T1_CONTEXT_DeviceIsChaining(READER_T1_ContextHandler *pContext, READER_T1_ChainingStatus *pChainingStatus){
	READER_T1_ChainingStatus status;
	
	/* On recupere le flag de situation globale de chainage, on fait des verif et on renvoi */
	status = pContext->deviceIsChaining;
	
	if(status == READER_T1_CHAINING_YES){
		*pChainingStatus = READER_T1_CHAINING_YES;
	}
	else if(status == READER_T1_CHAINING_NO){
		*pChainingStatus = READER_T1_CHAINING_NO;
	}
	else{
		return READER_ERR;
	}
	
	return READER_OK;
}



/**
 * \fn READER_Status READER_T1_CONTEXT_CardIsChaining(READER_T1_ContextHandler *pContext, READER_T1_ChainingStatus *pChainingStatus)
 * \brief Cette fonction permet de déterminer (a partir du contexte de communication) si la CARTE est dans une situation de chainage.
 * Il s'agit là de la "situation globale de chainage". C'est plus large que le M-Bit du dernier I-Block recu.
 * \param *pContext est un pointeur sur une structure de type READER_T1_ContextHandler. La structure pointée contient tout le contexte de communication courant.
 * \param *pChainingStatus est un pointeur sur une struture de type READER_T1_ChainingStatus. La fonction utilise ce pointeur pour retourner l'état de chainage.
 * \return La fonction retourne un code d'erreur de type READER_Status. READER_OK indique que la fonction s'est correctement déroulée.
 */
READER_Status READER_T1_CONTEXT_CardIsChaining(READER_T1_ContextHandler *pContext, READER_T1_ChainingStatus *pChainingStatus){
	READER_T1_ChainingStatus status;
	
	/* On recupere le flag de situation globale de chainage, on fait des verif et on renvoi */
	status = pContext->cardIsChaining;
	
	if(status == READER_T1_CHAINING_YES){
		*pChainingStatus = READER_T1_CHAINING_YES;
	}
	else if(status == READER_T1_CHAINING_NO){
		*pChainingStatus = READER_T1_CHAINING_NO;
	}
	else{
		return READER_ERR;
	}
	
	return READER_OK;
}



/**
 * \fn READER_Status READER_T1_CONTEXT_SetDeviceChainingSituationFlag(READER_T1_ContextHandler *pContext, READER_T1_ChainingStatus chainingStatus)
 * \brief Cette fonction mets à jour le contexte de communication avec la situation de chainage coté DEVICE.
 * La situation de chainage indique si e Block maniulé s'inscrit dans une chaine (ca peut etre le dernier de la chaine avec un M-Bit nul).
 * \param *pContext est un pointeur sur une structure de type READER_T1_ContextHandler. La structure pointée contient tout le contexte de communication courant.
 * \param chainingStatus est une valeur de type READER_T1_ChainingStatus. Elle indique si oui ou non il y a une SITUATION chainage.
 * \return La fonction retourne un code d'erreur de type READER_Status. READER_OK indique que le contexte a correctement été mis à jour.
 */
READER_Status READER_T1_CONTEXT_SetDeviceChainingSituationFlag(READER_T1_ContextHandler *pContext, READER_T1_ChainingStatus chainingStatus){
	if(chainingStatus == READER_T1_CHAINING_YES){
		pContext->deviceIsChaining = READER_T1_CHAINING_YES;
	}
	else if(chainingStatus == READER_T1_CHAINING_NO){
		pContext->deviceIsChaining = READER_T1_CHAINING_NO;
	}
	else{
		return READER_ERR;
	}	
	
	return READER_OK;
}



/**
 * \fn READER_Status READER_T1_CONTEXT_SetDeviceChainingLastBlockFlag(READER_T1_ContextHandler *pContext, READER_T1_ChainingStatus chainingStatus)
 * \brief Cette fonction mets à jour le contexte de communication avec la valeur du M-Bit du dernier I-Block envoyé par le DEVICE. C'est à dire, on indique si le DEVICE à l'intention de chainer le Block qu'il vient probablement d'envoyer.
 * \param *pContext est un pointeur sur une structure de type READER_T1_ContextHandler. La structure pointée contient tout le contexte de communication courant.
 * \param chainingStatus est une valeir de type READER_T1_ChainingStatus. Elle indique si oui ou non il y a chainage.
 * \return La fonction retourne un code d'erreur de type READER_Status. READER_OK indique que le contexte a correctement été mis à jour.
 */
READER_Status READER_T1_CONTEXT_SetDeviceChainingLastBlockFlag(READER_T1_ContextHandler *pContext, READER_T1_ChainingStatus chainingStatus){
	if(chainingStatus == READER_T1_CHAINING_YES){
		pContext->deviceIsChainingLastBlock = READER_T1_CHAINING_YES;
	}
	else if(chainingStatus == READER_T1_CHAINING_NO){
		pContext->deviceIsChainingLastBlock = READER_T1_CHAINING_NO;
	}
	else{
		return READER_ERR;
	}	
	
	return READER_OK;
}



/**
 * \fn READER_Status READER_T1_CONTEXT_SetCardChainingSituationFlag(READER_T1_ContextHandler *pContext, READER_T1_ChainingStatus chainingStatus)
 * \brief Cette fonction mets à jour le contexte de communication avec la situation de chainage coté CARTE.
 * La situation de chainage indique si e Block maniulé s'inscrit dans une chaine (ca peut etre le dernier de la chaine avec un M-Bit nul).
 * \param *pContext est un pointeur sur une structure de type READER_T1_ContextHandler. La structure pointée contient tout le contexte de communication courant.
 * \param chainingStatus est une valeur de type READER_T1_ChainingStatus. Elle indique si oui ou non il y a une SITUATION chainage.
 * \return La fonction retourne un code d'erreur de type READER_Status. READER_OK indique que le contexte a correctement été mis à jour.
 */
READER_Status READER_T1_CONTEXT_SetCardChainingSituationFlag(READER_T1_ContextHandler *pContext, READER_T1_ChainingStatus chainingStatus){
	if(chainingStatus == READER_T1_CHAINING_YES){
		pContext->cardIsChaining = READER_T1_CHAINING_YES;
	}
	else if(chainingStatus == READER_T1_CHAINING_NO){
		pContext->cardIsChaining = READER_T1_CHAINING_NO;
	}
	else{
		return READER_ERR;
	}	
	
	return READER_OK;
}


/**
 * \fn READER_Status READER_T1_CONTEXT_SetCardChainingLastBlockFlag(READER_T1_ContextHandler *pContext, READER_T1_ChainingStatus chainingStatus)
 * \brief Cette fonction mets à jour le contexte de communication avec la valeur du M-Bit du dernier I-Block envoyé par la CARTE. C'est à dire, on indique si la CARTE à l'intention de chainer le Block qu'elle vient probablement d'envoyer au Device.
 * \param *pContext est un pointeur sur une structure de type READER_T1_ContextHandler. La structure pointée contient tout le contexte de communication courant.
 * \param chainingStatus est une valeir de type READER_T1_ChainingStatus. Elle indique si oui ou non il y a chainage.
 * \return La fonction retourne un code d'erreur de type READER_Status. READER_OK indique que le contexte a correctement été mis à jour.
 */
READER_Status READER_T1_CONTEXT_SetCardChainingLastBlockFlag(READER_T1_ContextHandler *pContext, READER_T1_ChainingStatus chainingStatus){
	if(chainingStatus == READER_T1_CHAINING_YES){
		pContext->cardIsChainingLastBlock = READER_T1_CHAINING_YES;
	}
	else if(chainingStatus == READER_T1_CHAINING_NO){
		pContext->cardIsChainingLastBlock = READER_T1_CHAINING_NO;
	}
	else{
		return READER_ERR;
	}	
	
	return READER_OK;
}



/* Manipuation des S-Blocks */
/**
 * \fn READER_Status READER_T1_CONTEXT_IsSBlockResponseExpectedNow(READER_T1_ContextHandler *pContext, READER_T1_FlagStatus *pFlag)
 * \brief Cette fonction permet de savoir (à partir du contexte de communication) si un S-Block Response est attendu maintenant.
 * \param *pContext est un pointeur sur une structure de type READER_T1_ContextHandler. La structure pointée contient tout le contexte de communication courant.
 * \param *pFlag est un pointeur sur une structure de type READER_T1_FlagStatus. La fonction utilise ce pointeur pour indiquer si un S-Block Response est attendu maintenant ou pas.
 * \return La fonction retourne un code d'erreur de type READER_Status. READER_OK indique le bon déroulement.
 */
READER_Status READER_T1_CONTEXT_IsSBlockResponseExpectedNow(READER_T1_ContextHandler *pContext, READER_T1_FlagStatus *pFlag){
	if(pContext->isSBlockExpectedFlag == READER_T1_FLAGSTATUS_SET){
		*pFlag = READER_T1_FLAGSTATUS_SET;
	}
	else if(pContext->isSBlockExpectedFlag == READER_T1_FLAGSTATUS_RESET){
		*pFlag = READER_T1_FLAGSTATUS_RESET;
	}
	else{
		return READER_ERR;
	}
	
	return READER_OK;
}



/**
 * \fn READER_Status READER_T1_CONTEXT_GetSBlockExpectedResponseType(READER_T1_ContextHandler *pContext, READER_T1_SBlockType *pType)
 * \brief Cette fonction permet de savoir (à partir du contexte de communication) le type de S-Block Response qui est attendu en provenance de la carte (si toutefois un S-Block Response est attendu).
 * \param *pContext est un pointeur sur une structure de type READER_T1_ContextHandler. La structure pointée contient tout le contexte de communication courant.
 * \param *pType est un pointeur sur uen structure de type READER_T1_SBlockType. La fonction utilise ce pointeur pour y stocker le resultat, ie la type de S-Block Response attendu en provenance de la carte.
 * \return La fonction retourne un code d'erreur de type READER_Status. READER_OK indique le bon déroulement.
 */
READER_Status READER_T1_CONTEXT_GetSBlockExpectedResponseType(READER_T1_ContextHandler *pContext, READER_T1_SBlockType *pType){
	READER_Status retVal;
	READER_T1_FlagStatus isExp;
	READER_T1_SBlockType tmpType;
	
	
	/* On verifie qu'il y a effectivement un S-Block qui est attendu ... */
	retVal = READER_T1_CONTEXT_IsSBlockResponseExpectedNow(pContext, &isExp);
	if(retVal != READER_OK) return retVal;
	if(isExp != READER_T1_FLAGSTATUS_SET) return READER_ERR;
	
	/* On verifie le type de S-Block attendu, on regarde aussi si ce type est valide ... */
	tmpType = pContext->SBlockExpectedType;
	
	switch(tmpType){
		case READER_T1_STYPE_RESYNCH_RESP:
			*pType = tmpType;
			break;
		case READER_T1_STYPE_RESYNCH_REQU:
			*pType = tmpType;
			break;
		case READER_T1_STYPE_ABORT_RESP:
			*pType = tmpType;
			break;
		case READER_T1_STYPE_ABORT_REQU:
			*pType = tmpType;
			break;
		case READER_T1_STYPE_IFS_REQU:
			*pType = tmpType;
			break;
		case READER_T1_STYPE_IFS_RESP:
			*pType = tmpType;
			break;
		case READER_T1_STYPE_WTX_REQU:
			*pType = tmpType;
			break;
		case READER_T1_STYPE_WTX_RESP:
			*pType = tmpType;
			break;
		default :
			return READER_ERR;
	}
	
	return READER_OK;
}


READER_Status READER_T1_CONTEXT_SetSBlockExpectedResponse(READER_T1_ContextHandler *pContext, READER_T1_SBlockType type){
	/* On verifie que le type passe en parametre est valide                      */
	switch(type){
		case READER_T1_STYPE_RESYNCH_RESP:
			break;
		case READER_T1_STYPE_RESYNCH_REQU:
			break;
		case READER_T1_STYPE_ABORT_RESP:
			break;
		case READER_T1_STYPE_ABORT_REQU:
			break;
		case READER_T1_STYPE_IFS_REQU:
			break;
		case READER_T1_STYPE_IFS_RESP:
			break;
		case READER_T1_STYPE_WTX_REQU:
			break;
		case READER_T1_STYPE_WTX_RESP:
			break;
		default :
			return READER_ERR;
	}
	
	/* On indique dans le contexte le type de S-Block qui est attendu            */
	pContext->SBlockExpectedType = type;
	
	/* On positionne le flag qui indique qu'un S-Block est attendu               */
	pContext->isSBlockExpectedFlag = READER_T1_FLAGSTATUS_SET;
	
	return READER_OK;
}


READER_Status READER_T1_CONTEXT_SetNoSBlockExpectedResponse(READER_T1_ContextHandler *pContext){
	pContext->isSBlockExpectedFlag = READER_T1_FLAGSTATUS_RESET;
	
	return READER_OK;
}


READER_Status READER_T1_CONTEXT_GetSBlockRequCounter(READER_T1_ContextHandler *pContext, uint32_t *pCounter){
	*pCounter = pContext->SBlockRequCounter;
	
	return READER_OK;
}


READER_Status READER_T1_CONTEXT_SetSBlockRequCounter(READER_T1_ContextHandler *pContext, uint32_t counter){
	pContext->SBlockRequCounter = counter;
	
	return READER_OK;
}


READER_Status READER_T1_CONTEXT_ClearSBlockRequestCounter(READER_T1_ContextHandler *pContext){
	READER_Status retVal;
	
	
	retVal = READER_T1_CONTEXT_SetSBlockRequCounter(pContext, 0);
	if(retVal != READER_OK) return retVal;
	
	return READER_OK;
}


READER_Status READER_T1_CONTEXT_IncSBlockRequCounter(READER_T1_ContextHandler *pContext){
	uint32_t counter, newCounter;
	READER_Status retVal;
	
	
	retVal = READER_T1_CONTEXT_GetSBlockRequCounter(pContext, &counter);
	if(retVal != READER_OK) return retVal;
	
	if(counter < 0xFFFFFFFF){
		newCounter = counter + 1;
	}
	else{
		return READER_ERR;
	}
	
	retVal = READER_T1_CONTEXT_SetSBlockRequCounter(pContext, newCounter);
	if(retVal != READER_OK) return retVal;
	
	return READER_OK;
}


/* Retourne READER_OK si il est possible d'envoyer maintenant encore une S-Block Request. Retourne READER_NO si on a depasse le compteur. Retourne toute autre vaeur en vas d'erreur. */
READER_Status READER_T1_CONTEXT_CheckSBlockRequestCounter(READER_T1_ContextHandler *pContext){
	READER_Status retVal;
	uint32_t reqCounter;
	
	
	retVal = READER_T1_CONTEXT_GetSBlockRequCounter(pContext, &reqCounter);
	if(retVal != READER_OK) return retVal;
	
	if(reqCounter >= READER_T1_MAX_SREQUESTS){
		return READER_NO;
	}
	else{
		return READER_OK;
	}
}




READER_Status READER_T1_CONTEXT_SetSBlockExpectedINF(READER_T1_ContextHandler *pContext, uint8_t expectedINF){
	pContext->SBlockExpectedINF = expectedINF;
	
	
	return READER_OK;
}


READER_Status READER_T1_CONTEXT_GetSBlockExpectedINF(READER_T1_ContextHandler *pContext, uint8_t *pExpectedINF){
	*pExpectedINF = pContext->SBlockExpectedINF;
	
	
	return READER_OK;
}



/* On prends un S-Block Response et on verifie que le champ INF correspond a celui qui etait attendu ...  */
/* Retourne READER_ERR en cas d'erreur interne, READER_NO si le champ INF ne correspond pas, READER_OK si le champ INF correspond. Retourne toutte autre valeur si erreur interne. */
READER_Status READER_T1_CONTEXT_CheckSBlockExpectedINF(READER_T1_ContextHandler *pContext, READER_T1_Block *pBlock){
	READER_Status retVal;
	READER_T1_SBlockType SBlockType;
	uint8_t blockLEN;
	uint8_t blockINF, expectedBlockINF;
	
	
	/* On verifie que c'est bien un S-Block ...  */
	retVal = READER_T1_CheckSBlock(pBlock);
	if(retVal != READER_OK) return retVal;
	
	/* On regarde si ce S-Block possede un champ INF (on regarde le type de S-Block et le LEN du Block) ...  */
	SBlockType = READER_T1_GetBlockSType(pBlock);
	if((SBlockType != READER_T1_STYPE_IFS_RESP) && (SBlockType != READER_T1_STYPE_WTX_RESP)){
		return READER_ERR;
	}
	
	blockLEN = READER_T1_GetBlockLEN(pBlock);
	if(blockLEN != 1){
		return READER_ERR;
	}
	
	/* On recupere le champ INF du S-Block ...  */
	blockINF = READER_T1_GetBlockSPayload(pBlock);
	
	/* On recupere le champ INF qui est attendu dans la reponse ...  */
	retVal = READER_T1_CONTEXT_GetSBlockExpectedINF(pContext, &expectedBlockINF);
	if(retVal != READER_OK) return retVal;
	
	/* On verifie que les deux correspondent ...  */
	if(blockINF == expectedBlockINF){
		return READER_OK;
	}
	else{
		return READER_NO;
	}
}



/**
 * \fn READER_Status READER_T1_CONTEXT_GetBlockBuff(READER_T1_ContextHandler *pContext, READER_T1_BlockBuffer **ppBlockBuff)
 * \brief Cette fonction permet de récupérer un pointeur sur la structure du Buffer d'envoi (READER_T1_BlockBuffer) qui se trouve à l'intérieur du contexte de communication (READER_T1_ContextHandler).
 * \param *pContext est un pointeur sur une structure de type READER_T1_ContextHandler. La structure pointée stocke le contexte actuel de communication.
 * \param **ppBlockBuff est un pointeur sur un pointeur (qui pointe lui-même sur une structure de type READER_T1_BlockBuffer). Ce pointeur permet à la fonction d'aller modifier un pointeur de sorte a ce qu'il pointe désormais sur la structure READER_T1_BlockBuffer qui se trouve dans le contexte de communication.
 * \return La fonction retourne un code d'erreur de type READER_Status. READER_OK indique le bon déroulement de la fonction.
 */
READER_Status READER_T1_CONTEXT_GetBlockBuff(READER_T1_ContextHandler *pContext, READER_T1_BlockBuffer **ppBlockBuff){
	/* On recupere un pointeur qui pointe sur le pointeur de blockBuff                                              */
	/* On veut que ce pointeur pointe desormais sur le blockBuff qui se trouve dans le contexte   de communication  */
	
	/* (*pBlockBuff) est la valeur du pointeur qui est pointe par pBlockBuff                                        */
	/* (*pBlockBuff) est un pointeur. C'est celui qui est utilise en dehors de la fonction pour pointer sur le buff */
	/* On modifie (*pBlockBuff) pour qu'il pointe desormais sur le buff qui se trouve dans le contexte              */
	/* On accede a (*pBlockBuff) via un pointeur qui pointe dessus. Ce pointeur est pBlockBuff (param de la fct)    */
	/* (A priori pContext->blockBuff n'est pas un pointeur, mais la struct en elle meme ???)                        */
	*ppBlockBuff = &(pContext->blockBuff);
	
	
	return READER_OK;
}



/**
 * \fn READER_Status READER_T1_CONTEXT_SetTickLastBlock(READER_T1_ContextHandler *pContext, uint32_t tickLastBlock)
 * \brief Cette fonction permet d'enregistrer dans le contexte de communication le tick (la date en millisecondes) du leading edge du dernier caractère du dernier Block.
 * \param *pContext est un pointeur sur une structure de type READER_T1_ContextHandler. La structure pointée stocke le contexte actuel de communication.
 * \param tickLastBlock est un entier de type uint32_t. C'est la valeur du tick en millisecondes que le veut enregistrer dans le contexte.
 * \return La fonction retourne un code d'erreur de type READER_Status. READER_OK indique le bon déroulement de la fonction.
 */
READER_Status READER_T1_CONTEXT_SetTickLastBlock(READER_T1_ContextHandler *pContext, uint32_t tickLastBlock){
	pContext->tickLastBlock = tickLastBlock;
	pContext->tickLastBlockFlag = READER_T1_FLAGSTATUS_SET;
	
	return READER_OK;
}


READER_Status READER_T1_CONTEXT_GetTickLastBlock(READER_T1_ContextHandler *pContext, uint32_t *tickValue){
	if(pContext->tickLastBlockFlag == READER_T1_FLAGSTATUS_SET){
		*tickValue = pContext->tickLastBlock;
		return READER_OK;
	}
	else if(pContext->tickLastBlockFlag == READER_T1_FLAGSTATUS_RESET){
		return READER_DOESNT_EXIST;
	}
	else{
		return READER_ERR;
	}
}


/* Manipulation des commSettings ...  */
READER_Status READER_T1_CONTEXT_GetHalCommSettingsPtr(READER_T1_ContextHandler *pContext, READER_HAL_CommSettings **ppCommSettings){
	READER_HAL_CommSettings *pCommSettings;
	
	
	pCommSettings = &(pContext->halCommSettings);
	*ppCommSettings = pCommSettings;
	
	return READER_OK;
}


READER_Status READER_T1_CONTEXT_GetHalCommSettingsFreq(READER_T1_ContextHandler *pContext, uint32_t *pFreq){
	READER_HAL_CommSettings *pCommSettings;
	READER_Status retVal;
	
	
	retVal = READER_T1_CONTEXT_GetHalCommSettingsPtr(pContext, &pCommSettings);
	if(retVal != READER_OK) return READER_ERR;
	
	*pFreq = READER_HAL_GetFreq(pCommSettings);	
	
	
	return READER_OK;
}


READER_Status READER_T1_CONTEXT_GetHalCommSettingsFi(READER_T1_ContextHandler *pContext, uint32_t *pFi){
	READER_HAL_CommSettings *pCommSettings;
	READER_Status retVal;
	
	
	retVal = READER_T1_CONTEXT_GetHalCommSettingsPtr(pContext, &pCommSettings);
	if(retVal != READER_OK) return READER_ERR;
	
	*pFi = READER_HAL_GetFi(pCommSettings);	
	
	
	return READER_OK;
}


READER_Status READER_T1_CONTEXT_GetHalCommSettingsDi(READER_T1_ContextHandler *pContext, uint32_t *pDi){
	READER_HAL_CommSettings *pCommSettings;
	READER_Status retVal;
	
	
	retVal = READER_T1_CONTEXT_GetHalCommSettingsPtr(pContext, &pCommSettings);
	if(retVal != READER_OK) return READER_ERR;
	
	*pDi = READER_HAL_GetDi(pCommSettings);	
	
	
	return READER_OK;
}


READER_Status READER_T1_CONTEXT_GetHalCommSettingsGT(READER_T1_ContextHandler *pContext, uint32_t *pGT){
	READER_HAL_CommSettings *pCommSettings;
	READER_Status retVal;
	
	
	retVal = READER_T1_CONTEXT_GetHalCommSettingsPtr(pContext, &pCommSettings);
	if(retVal != READER_OK) return READER_ERR;
	
	*pGT = READER_HAL_GetGT(pCommSettings);	
	
	
	return READER_OK;
}



READER_Status READER_T1_CONTEXT_SetHalCommSettingsFreq(READER_T1_ContextHandler *pContext, uint32_t freq){
	READER_HAL_CommSettings *pCommSettings;
	READER_Status retVal;
	
	
	if(frequ == 0){
		return READER_BAD_ARG;
	}
	
	retVal = READER_T1_CONTEXT_GetHalCommSettingsPtr(pContext, &pCommSettings);
	if(retVal != READER_OK) return READER_ERR;
	
	retVal = READER_HAL_SetFreq(pCommSettings, freq);
	if(retVal != READER_OK) return READER_ERR;
	
	
	return READER_OK;
}


READER_Status READER_T1_CONTEXT_SetHalCommSettingsFi(READER_T1_ContextHandler *pContext, uint32_t Fi){
	READER_HAL_CommSettings *pCommSettings;
	READER_Status retVal;
	
	
	if(Fi == 0){
		return READER_BAD_ARG;
	}
	
	retVal = READER_T1_CONTEXT_GetHalCommSettingsPtr(pContext, &pCommSettings);
	if(retVal != READER_OK) return READER_ERR;
	
	retVal = READER_HAL_SetFi(pCommSettings, Fi);
	if(retVal != READER_OK) return READER_ERR;
	
	
	return READER_OK;
}


READER_Status READER_T1_CONTEXT_SetHalCommSettingsDi(READER_T1_ContextHandler *pContext, uint32_t Di){
	READER_HAL_CommSettings *pCommSettings;
	READER_Status retVal;
	
	
	if(Di == 0){
		return READER_BAD_ARG;
	}
	
	retVal = READER_T1_CONTEXT_GetHalCommSettingsPtr(pContext, &pCommSettings);
	if(retVal != READER_OK) return READER_ERR;
	
	retVal = READER_HAL_SetDi(pCommSettings, Di);
	if(retVal != READER_OK) return READER_ERR;
	
	
	return READER_OK;
}


READER_Status READER_T1_CONTEXT_SetHalCommSettingsGT(READER_T1_ContextHandler *pContext, uint32_t GT){
	READER_HAL_CommSettings *pCommSettings;
	READER_Status retVal;
	
	
	retVal = READER_T1_CONTEXT_GetHalCommSettingsPtr(pContext, &pCommSettings);
	if(retVal != READER_OK) return READER_ERR;
	
	retVal = READER_HAL_SetGT(pCommSettings, GT);
	if(retVal != READER_OK) return READER_ERR;
	
	
	return READER_OK;
}




READER_Status READER_T1_CONTEXT_ImportHalCommSettingsToContext(READER_T1_ContextHandler *pContext, READER_HAL_CommSettings *pSettings){
	READER_HAL_CommSettings *pCommSettings;
	READER_Status retVal;
	uint32_t freq, Fi, Di, GT;
	
	
	freq = READER_HAL_GetFreq(pSettings);
	Fi = READER_HAL_GetFi(pSettings);
	Di = READER_HAL_GetDi(pSettings);
	GT = READER_HAL_GetGT(pSettings);
	
	
	retVal = READER_T1_CONTEXT_GetHalCommSettingsPtr(pContext, &pCommSettings);
	if(retVal != READER_OK) return READER_ERR;
	
	
	retVal = READER_T1_CONTEXT_SetHalCommSettingsFreq(pContext, freq);
	if(retVal != READER_OK) return READER_ERR;
	
	retVal = READER_T1_CONTEXT_SetHalCommSettingsFi(pContext, Fi);
	if(retVal != READER_OK) return READER_ERR;
	
	retVal = READER_T1_CONTEXT_SetHalCommSettingsDi(pContext, Di);
	if(retVal != READER_OK) return READER_ERR;
	
	retVal = READER_T1_CONTEXT_SetHalCommSettingsGT(pContext, GT);
	if(retVal != READER_OK) return READER_ERR;
	
	
	return READER_OK;
}


READER_Status READER_T1_CONTEXT_ExportHalCommSettingsFromContext(READER_T1_ContextHandler *pContext, READER_HAL_CommSettings *pSettings){
	
}



