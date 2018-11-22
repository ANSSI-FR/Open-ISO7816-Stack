#include "reader_t1_context_handler.h"


/* Initialisation de la structure */

READER_Status READER_T1_CONTEXT_Init(READER_T1_ContextHandler *pContext){
	READER_Status retVal;
	
	retVal = READER_T1_CONTEXT_InitParams(pContext);
	if(retVal != READER_OK) return retVal;
	
	retVal = READER_T1_CONTEXT_InitBuffer(pContext);
	if(retVal != READER_OK) return retVal;
	
	return READER_OK;
}


READER_Status READER_T1_CONTEXT_InitParams(READER_T1_ContextHandler *pContext){
	/* On reinitialise tous les parametres de communication dans le contexte               */
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
	
	retVal = READER_T1_CONTEXT_SetLastSent(pContext, NULL);
	if(retVal != READER_OK) return retVal;
	
	retVal = READER_T1_CONTEXT_SetLastIBlockSent(pContext, NULL);
	if(retVal != READER_OK) return retVal;
	
	retVal = READER_T1_CONTEXT_SetLastRcvd(pContext, NULL);
	if(retVal != READER_OK) return retVal;
	
	retVal = READER_T1_CONTEXT_SetLastIBlockRcvd(pContext, NULL);
	if(retVal != READER_OK) return retVal;
	
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
	*pIfsc = pContext->currentIFSC;
	return READER_OK;
}


READER_Status READER_T1_CONTEXT_GetCurrentIFSD(READER_T1_ContextHandler *pContext, uint32_t *pIfsd){
	*pIfsd = pContext->currentIFSD;
	return READER_OK;
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
	if((ifsc >= 0x01) && (ifsc <= 0xFE)){   /* Voir ISO7816-3 section 11.4.2 */
		pContext->currentIFSC = ifsc;
	}
	else{
		return READER_ERR;
	}
	
	return READER_OK;
}


READER_Status READER_T1_CONTEXT_SetCurrentIFSD(READER_T1_ContextHandler *pContext, uint32_t ifsd){
	if((ifsd >= 0x01) && (ifsd <= 0xFE)){   /* Voir ISO7816-3 section 11.4.2 */
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



READER_Status READER_T1_CONTEXT_GetApduResponse(READER_T1_ContextHandler *pContext, READER_APDU_Response *pApduResp){
	
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
	pContext-resynchCounter = counter;
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

/* Attention, on ne recopie pas le Block. On retourne juste un pointeur sur le Block qui se trouve a l'interieur du contexte.         */
READER_Status READER_T1_CONTEXT_GetLastSent(READER_T1_ContextHandler *pContext, READER_T1_Block **ppBlockDest){
	/* **ppBlockDest, On recupere un pointeur qui pointe sur un pointeur du Block de destination                                      */
	/* Attention il faut que l'espace memoire soit deja allour auaravent pour le Block                                                */
	/* Attention, fuite memeoire si on fait ensuite pointer sur NULL !!!                                                              */

	READER_T1_Block *pBlockDest;
	READER_Status retVal;
	
	
	/* On recupere un pointeur qui pointe sur le pointeur qui pointe sur le dernier Block Envoye qui se trouve dans le contexte       */
	/* Le pointeur sur le pointeur nous permet de modifier le pointeur qui pointe sur le Block.                                       */
	/* Cette teechnique nous permet de renvoyer un pointeur sans utiliser les valeurs de retour de la fonction (qui sont deja utilise pour le code d'erreur) */
	
	pBlockDest = *ppBlockDest;
	
	if(&(pContext->lastSent) == NULL){
		pBlockDest = NULL; 
	}
	else{
		pBlockDest = &(pContext->lastSent);    /* Finalement on n'effectue pas de recopie de Blocks */
		//retVal = READER_T1_CopyBlock(pBlockDest, &(pContext->lastSent));
		//if(retVal != READER_OK) return retVal;
	}
	
	return READER_OK;
}


READER_Status READER_T1_CONTEXT_GetLastIBlockSent(READER_T1_ContextHandler *pContext, READER_T1_Block **ppBlockDest){
	READER_Status retVal;
	READER_T1_BlockType bType;
	READER_T1_Block *pBlockDest;
	
	
	pBlockDest = *ppBlockDest;
	
	/* Si il n'y a pas de dernier I-Block envoye (on en a pas encore envoye) */
	if(&(pContext->lastIBlockSent) == NULL){
		pBlockDest = NULL;
	}
	/* Si il y en a un ... On le copie ... */
	else{
		/* On ne fait plus de recopie du Block. On renvoie un pointeur sur le Block qui se trouve dans le contexte */
		//retVal = READER_T1_CopyBlock(pBlock, &(pContext->lastIBlockSent));
		//if(retVal != READER_OK) return retVal;
		pBlockDest = &(pContext->lastIBlockSent);
		
		/* On verifie que c'est effectivement un I-Block */
		bType = READER_T1_GetBlockType(pBlockDest);
		if(bType != READER_T1_IBLOCK) return READER_ERR;
	}
	
	
	return READER_OK;
}


READER_Status READER_T1_CONTEXT_GetLastRcvd(READER_T1_ContextHandler *pContext, READER_T1_Block **ppBlockDest){
	READER_Status retVal;
	READER_T1_Block *pBlockDest;
	
	
	pBlockDest = *ppBlockDest;
	
	if(&(pContext->lastRcvd) == NULL){
		pBlockDest = NULL;
	}
	else{
		/* On ne fait plus de recopie du Block. On renvoie un pointeur sur le Block qui se trouve dans le contexte */
		//retVal = READER_T1_CopyBlock(pBlock, &(pContext->lastRcvd));
		//if(retVal != READER_OK) return retVal;
		pBlockDest = &(pContext->lastRcvd);
	}
	
	return READER_OK;
}


READER_Status READER_T1_CONTEXT_GetLastIBlockRcvd(READER_T1_ContextHandler *pContext, READER_T1_Block **ppBlockDest){
	READER_Status retVal;
	READER_T1_BlockType bType;
	READER_T1_Block *pBlockDest;
	
	
	pBlockDest = *ppBlockDest;	
	
	/* Si il n'y a pas de dernier I-Block recu (on en a pas encore recu) */
	if(&(pContext->lastIBlockRcvd) == NULL){
		pBlockDest = NULL;
	}
	/* Si il y en a un ... On le copie ... */
	else{
		/* On ne fait plus de recopie du Block. On renvoie un pointeur sur le Block qui se trouve dans le contexte */
		//retVal = READER_T1_CopyBlock(pBlock, &(pContext->lastIBlockRcvd));
		//if(retVal != READER_OK) return retVal;
		pBlockDest = &(pContext->lastIBlockRcvd);
		
		/* On verifie que c'est effectivement un I-Block */
		bType = READER_T1_GetBlockType(pBlockDest);
		if(bType != READER_T1_IBLOCK) return READER_ERR;
	}
	
	
	return READER_OK;
}



READER_Status READER_T1_CONTEXT_SetLastSent(READER_T1_ContextHandler *pContext, READER_T1_Block *pBlock){
	READER_Status retVal;
	
	if(pBlock == NULL){
		&(pContext->lastSent) = NULL;
	}
	else{
		retVal = READER_T1_CopyBlock(&(pContext->lastSent), pBlock);
		if(retVal != READER_OK) return retVal;
	}
	
	return READER_OK;
}


READER_Status READER_T1_CONTEXT_SetLastIBlockSent(READER_T1_ContextHandler *pContext, READER_T1_Block *pBlock){
	READER_Status retVal;
	
	if(pBlock == NULL){
		&(pContext->lastIBlockSent) = NULL;
	}
	else{
		retVal = READER_T1_CopyBlock(&(pContext->lastIBlockSent), pBlock);
		if(retVal != READER_OK) return retVal;
	}
	
	return READER_OK;
}


READER_Status READER_T1_CONTEXT_SetLastIBlockRcvd(READER_T1_ContextHandler *pContext, READER_T1_Block *pBlock){
	READER_Status retVal;
	
	if(pBlock == NULL){
		&(pContext->lastIBlockRcvd) = NULL;
	}
	else{
		retVal = READER_T1_CopyBlock(&(pContext->lastIBlockRcvd), pBlock);
		if(retVal != READER_OK) return retVal;
	}
	
	return READER_OK;
}


READER_Status READER_T1_CONTEXT_SetLastRcvd(READER_T1_ContextHandler *pContext, READER_T1_Block *pBlock){
	READER_Status retVal;
	
	if(pBlock == NULL){
		&(pContext->lastRcvd) = NULL;
	}
	else{
		retVal = READER_T1_CopyBlock(&(pContext->lastRcvd), pBlock);
		if(retVal != READER_OK) return retVal;
	}
	
	return READER_OK;
}


READER_Status READER_T1_CONTEXT_GetLastSentType(READER_T1_ContextHandler *pContext, READER_T1_BlockType *pType){
	READER_T1_BlockType bType;
	
	
	bType = READER_T1_GetBlockType(&(pContext->lastSent));
	
	if(bType == READER_T1_BLOCK_ERR){
		return READER_ERR;
	}
	
	*pType = bType;

	return READER_OK;
}


READER_Status READER_T1_CONTEXT_GetLastRcvdType(READER_T1_ContextHandler *pContext, READER_T1_BlockType *pType){
	READER_T1_BlockType bType;
	
	
	bType = READER_T1_GetBlockType(&(pContext->lastRcvd));
	
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


/* Gestion du chainage */
READER_Status READER_T1_CONTEXT_DeviceIsChainingLastBlock(READER_T1_ContextHandler *pContext, READER_T1_ChainingStatus *pChainingStatus){
	READER_Status retVal;
	READER_T1_Block block;
	uint32_t mBit;
	
	
	/* On recupere a partir du context handler le dernier I-Block qu'on a envoye */
	retval = READER_T1_CONTEXT_GetLastIBlockSent(pContext, &block);
	if(retVal != READER_OK) return retVal;
	
	/* On verifie qu'il existe effectivement un dernier I-Block envoye */
	if(&block == NULL){
		*pChainingStatus = READER_T1_CHAINING_NO;
	}
	
	/* On recupere le M-Bit du dernier I-Block ... */
	mBit = READER_T1_GetBlockMBit(&block);
	
	if(mBit == 0){
		*pChainingStatus = READER_T1_CHAINING_NO;
	}
	else if(mBit == 1){
		*pChainingStatus = READER_T1_CHAINING_YES;
	}
	else{
		return READER_ERR;
	}
	
	return READER_OK;
}


READER_Status READER_T1_CONTEXT_CardIsChainingLastBlock(READER_T1_ContextHandler *pContext, READER_T1_ChainingStatus *pChainingStatus){
	READER_Status retVal;
	READER_T1_Block block;
	uint32_t mBit;
	
	
	/* On recupere le dernier I-Block qu'on a recu de la carte */
	retVal = READER_T1_CONTEXT_GetLastIBlockRcvd(pContext, &block);
	if(retVal != READER_OK) return retVal;
	
	/* On verifie qu'il existe effectivement un dernier I-Block Recu */
	if(&block == NULL){
		*pChainingStatus = READER_T1_CHAINING_NO;
	}
	
	/* On recupere le mBit du dernier I-Block recu */
	mBit = READER_T1_GetBlockMBit(&block);
	
	if(mBit == 0){
		*pChainingStatus = READER_T1_CHAINING_NO;
	}
	else if(mBit == 1){
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
	else of(pContext->SBlockExpected == READER_T1_SBLOCK_EXPECTED_NO){
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


READER_Status READER_T1_CONTEXT_GetBlockBuff(READER_T1_ContextHandler *pContext, READER_T1_BlockBuffer **pBlockBuff){
	/* On recupere un pointeur qui pointe sur le pointeur de blockBuff                                              */
	/* On veut que ce pointeur pointe desormais sur le blockBuff qui se trouve dans le contexte   de communication  */
	
	/* (*pBlockBuff) est la valeur du pointeur qui est pointe par pBlockBuff                                        */
	/* (*pBlockBuff) est un pointeur. C'est celui qui est utilise en dehors de la fonction pour pointer sur le buff */
	/* On modifie (*pBlockBuff) pour qu'il pointe desormais sur le buff qui se trouve dans le contexte              */
	/* On accede a (*pBlockBuff) via un pointeur qui pointe dessus. Ce pointeur est pBlockBuff (param de la fct)    */
	/* (A priori pContext->blockBuff n'est pas un pointeur, mais la struct en elle meme ???)                        */
	*pBlockBuff = &(pContext->blockBuff);
	
	
	return READER_OK;
}

