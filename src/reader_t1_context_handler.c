#include "reader_t1_context_handler.h"


/* Initialisation de la structure */

READER_Status READER_T1_CONTEXT_Init(READER_T1_ContextHandler *pContext){
	READER_Status retVal;
	
	retVal = READER_T1_CONTEXT_InitSettings(pContext);
	if(retVal != READER_OK) return retVal;
	
	retVal = READER_T1_CONTEXT_InitBuffer(pContext);
	if(retVal != READER_OK) return retVal;
	
	return READER_OK;
}


READER_Status READER_T1_CONTEXT_InitCommSettings(READER_T1_ContextHandler *pContext){
	READER_Status retVal;
	
	
	retVal = READER_T1_CONTEXT_SetCurrentBGT(pContext, READER_T1_CONTEXT_DEFAULT_BGT);
	if(retVal != READER_OK) return retVal;
	
	retVal = READER_T1_CONTEXT_SetCurrentBWT(pContext, READER_T1_CONTEXT_DEFAULT_BWT);
	if(retVal != READER_OK) return retVal;
	
	retVal = READER_T1_CONTEXT_SetCurrentCGT(pContext, READER_T1_CONTEXT_DEFAULT_CGT);
	if(retVal != READER_OK) return retVal;
	
	retVal = READER_T1_CONTEXT_SetCurrentCWT(pContext, READER_HAL_GetWT());
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
	
	return READER_OK;
}


READER_Status READER_T1_CONTEXT_InitContextSettings(READER_T1_ContextHandler *pContext){
	READER_Status retVal;
	
	
	retVal = READER_T1_CONTEXT_SetDeviceCompleteSeqNum(pContext, 0);
	if(retVal != READER_OK) return retVal;
	
	retVal = READER_T1_CONTEXT_SetCardCompleteSeqNum(pContext, 0);
	if(retVal != READER_OK) return retVal;
	
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
	
	retVal = READER_T1_CONTEXT_SetNoSBlockExpected(pContext);
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
	
	return READER_OK;
}


READER_Status READER_T1_CONTEXT_InitSettings(READER_T1_ContextHandler *pContext){
	READER_Status retVal;
	
	
	/* On reinitialise tous les parametres de communication dans le contexte               */
	
	retVal = READER_T1_CONTEXT_InitCommSettings(pContext);
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


/* Accesseurs sur les parametres actuels de communication */
READER_Status READER_T1_CONTEXT_GetCurrentBGT(READER_T1_ContextHandler *pContext, uint32_t *pBgt){
	*pBgt = pContext->currentBGT;
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
	if((ifsc >= READER_T1_MIN_IFSC_ACCEPTED) && (ifsc <= READER_T1_MAX_IFSC_ACCEPTED)){   /* Voir ISO7816-3 section 11.4.2 */
		pContext->currentIFSC = ifsc;
	}
	else{
		return READER_ERR;
	}
	
	return READER_OK;
}


READER_Status READER_T1_CONTEXT_SetCurrentIFSD(READER_T1_ContextHandler *pContext, uint32_t ifsd){
	if((ifsd >= READER_T1_MIN_IFSD_ACCEPTED) && (ifsd <= READER_T1_MAX_IFSD_ACCEPTED)){   /* Voir ISO7816-3 section 11.4.2 */
		pContext->currentIFSC = ifsd;
	}
	else{
		return READER_ERR;
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
	READER_T1_Block *pBlockDest;
	READER_Status retVal;
	
	
	pBlockDest = *ppBlockDest;
	
	/* On verifie qu'il existe effectivement un dernier I-Block envoye ...  */
	retVal = READER_T1_CONTEXT_LastIBlockSentExists(pContext);
	if(retVal != READER_OK) return retVal;
	
	/* Si ce Block existe, on verifie son type avant de le servir ...  */
	bType = READER_T1_GetBlockType(pBlockDest);
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


READER_Status READER_T1_CONTEXT_GetLastIBlockSentSeqSum(READER_T1_ContextHandler *pContext, uint32_t *pSeqNum){
	READER_Status retVal;
	READER_T1_Block *pLastBlock;
	READER_T1_SeqNumber tmpSeqNum;
	
	
	/* On recupere le dernier I-Block envoye                                 */
	retVal = READER_T1_CONTEXT_GetLastIBlockSent(pContext, &pLastBlock);
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


/* On retourne READER_OK si existe, READER_DOESNT_EXIST so n'existe pas et READER_ERR en cas d'erreur interne ...  */
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
	
	nextCardSeqNum = (cardSeqNum + 1) & 0x00000001;     /* Numero de sequence modulo 2 ... */
	
	*pSeqNum = nextCardSeqNum;
	
	return READER_OK;
}


READER_Status READER_T1_CONTEXT_GetCardExpectedSeqNum(READER_T1_ContextHandler *pContext, uint32_t *pSeqNum){
	uint32_t seqNum;
	READER_Status retVal;
	
	
	retVal = READER_T1_CONTEXT_ComputeNextCardSeqNum(pContext, &seqNum);
	if(retVal != READER_OK) return retVal;
	
	*pSeqNum = seqNum;
	
	return READER_OK;
}


/* Gestion du chainage */
READER_Status READER_T1_CONTEXT_DeviceIsChainingLastBlock(READER_T1_ContextHandler *pContext, READER_T1_ChainingStatus *pChainingStatus){
	READER_Status retVal;
	READER_T1_Block *pBlock;
	READER_T1_MBit mBit;
	
	
	/* On recupere a partir du context handler un pointeur sur le dernier I-Block qu'on a envoye */
	retVal = READER_T1_CONTEXT_GetLastIBlockSent(pContext, &pBlock);
	if(retVal != READER_OK) return retVal;
	
	/* On recupere le M-Bit du dernier I-Block ... */
	mBit = READER_T1_GetBlockMBit(pBlock);
	
	if(mBit == READER_T1_MBIT_ZERO){
		*pChainingStatus = READER_T1_CHAINING_NO;
	}
	else if(mBit == READER_T1_MBIT_ONE){
		*pChainingStatus = READER_T1_CHAINING_YES;
	}
	else{
		return READER_ERR;
	}
	
	return READER_OK;
}


READER_Status READER_T1_CONTEXT_CardIsChainingLastBlock(READER_T1_ContextHandler *pContext, READER_T1_ChainingStatus *pChainingStatus){
	READER_Status retVal;
	READER_T1_Block *pBlock;
	READER_T1_MBit mBit;
	
	
	/* On recupere un pointeur sur le dernier I-Block qu'on a recu de la carte */
	retVal = READER_T1_CONTEXT_GetLastIBlockRcvd(pContext, &pBlock);
	if((retVal != READER_OK) && (retVal != READER_DOESNT_EXIST)) return retVal;
	
	/* On verifie qu'il existe effectivement un dernier I-Block Recu */
	if(retVal == READER_DOESNT_EXIST){
		*pChainingStatus = READER_T1_CHAINING_NO;
	}
	
	/* On recupere le mBit du dernier I-Block recu */
	mBit = READER_T1_GetBlockMBit(pBlock);
	
	if(mBit == READER_T1_MBIT_ZERO){
		*pChainingStatus = READER_T1_CHAINING_NO;
	}
	else if(mBit == READER_T1_MBIT_ONE){
		*pChainingStatus = READER_T1_CHAINING_YES;
	}
	else{
		return READER_ERR;
	}
	
	return READER_OK;
}


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
READER_Status READER_T1_CONTEXT_IsSblockExpectedNow(READER_T1_ContextHandler *pContext, READER_T1_SBlockExpected *pExp){
	if(pContext->SBlockExpected == READER_T1_SBLOCK_EXPECTED_YES){
		*pExp = READER_T1_SBLOCK_EXPECTED_YES;
	}
	else if(pContext->SBlockExpected == READER_T1_SBLOCK_EXPECTED_NO){
		*pExp = READER_T1_SBLOCK_EXPECTED_NO;
	}
	else{
		return READER_ERR;
	}
	
	return READER_OK;
}


READER_Status READER_T1_CONTEXT_GetSBlockExpectedType(READER_T1_ContextHandler *pContext, READER_T1_SBlockType *pType){
	READER_Status retVal;
	READER_T1_SBlockExpected isExp;
	READER_T1_SBlockType tmpType;
	
	
	/* On verifie qu'il y a effectivement un S-Block qui est attendu ... */
	retVal = READER_T1_CONTEXT_IsSblockExpectedNow(pContext, &isExp);
	if(retVal != READER_OK) return retVal;
	if(isExp != READER_T1_SBLOCK_EXPECTED_YES) return READER_ERR;
	
	/* On verifie le type de S-Block attendu, on regare aussi si ce type est valide ... */
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


READER_Status READER_T1_CONTEXT_SetSBlockExpected(READER_T1_ContextHandler *pContext, READER_T1_SBlockType type){
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
	pContext->SBlockExpected = READER_T1_SBLOCK_EXPECTED_YES;
	
	return READER_OK;
}


READER_Status READER_T1_CONTEXT_SetNoSBlockExpected(READER_T1_ContextHandler *pContext){
	pContext->SBlockExpected = READER_T1_SBLOCK_EXPECTED_NO;
	
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

