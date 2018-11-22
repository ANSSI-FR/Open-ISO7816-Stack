#include "reader_t1_error_handling.h"





READER_Status READER_T1_ERR_IncRepeatCounter(READER_T1_ContextHandler *pContext){
	if(pContext->repeatCounter < READER_T1_MAX_REAPEAT){
		pContext->repeatCounter = pContext->repeatCounter + 1;
	}
	else{
		return READER_ERR;
	}
	
	return READER_OK;
}



READER_Status READER_T1_ERR_CheckRepeatCounter(READER_T1_ContextHandler *pContext){
	if(pContext->repeatCounter < READER_T1_MAX_REAPEAT){
		return READER_OK;
	}
	else{
		return READER_NO;
	}
}



READER_Status READER_T1_ERR_IncResynchCounter(READER_T1_ContextHandler *pContext){
	if(pContext->resynchCounter < READER_T1_MAX_RESYNCH){
		pContext->resynchCounter = pContext->resynchCounter + 1;
	}
	else{
		return READER_ERR;
	}
	
	return READER_OK;
}



READER_Status READER_T1_ERR_CheckResynchCounter(READER_T1_ContextHandler *pContext){
	if(pContext->resynchCounter < READER_T1_MAX_RESYNCH){
		return READER_OK;
	}
	else{
		return READER_NO;
	}
}



READER_Status READER_T1_ERR_IsItFirstReception(READER_T1_ContextHandler *pContext){
	uint32_t deviceCompleteSeqNum, deviceCompleteSeqNum;
	READER_Status retVal;
	
	/* On recupere les numeros de sequence cote carte et cote lecteur */
	retVal = READER_T1_CONTEXT_GetCardCompleteSeqNum(pContext, &cardCompleteSeqNum);
	if(retVal != READER_OK) return retVal;
	
	retVal = READER_T1_CONTEXT_GetDeviceCompleteSeqNum(pContext, &deviceCompleteSeqNum);
	if(retVal != READER_OK) return retVal;
	
	/* On regarde si c'est la premiere reception */
	if((cardCompleteSeqNum == 0) && (deviceCompleteSeqNum == 0)){
		return READER_OK;
	}
	else{
		return READER_NO;
	}
}



READER_Status READER_T1_ERR_ForgeErrorBlock(READER_T1_ContextHandler *pContext, READER_T1_Block *pBlockDest, uint32_t integrityFlag){
	READER_Status retVal;
	READER_T1_Block *pPreviouslySentBlock;
	READER_T1_BlockType bType;
	
	
	/* On regarde si c'est la premiere reception  */
	retVal = READER_T1_ERR_IsItFirstReception(pContext);
	
	if(retVal == READER_OK){    /* Rules 7.5 and 7.6 ... */
		retVal = READER_T1_FORGE_NACKR0(pContext, pBlockDest, integrityFlag);
		if(retVal != READER_OK) return retVal;
	}
	/* Il y a deja eu au moins un echange (1 emission et 1 reception) */
	else{
		/* On recupere le type du dernier Block envoye ...            */
		retVal = READER_T1_CONTEXT_GetLastSentType(pContext, &bType);
		if(retVal != READER_OK) return retVal;
		
		/* Selon le type du dernier Block envoye (rules 7.1, 7.2 rt 7.3) ... */
		if(bType == READER_T1_IBLOCK){
			retVal = READER_T1_FORGE_NACK71(pContext, pBlockDest, integrityFlag);
			if(retVal != READER_OK) return retVal;
		}
		else if(bType == READER_T1_RBLOCK){
			retVal = READER_T1_FORGE_NACK72(pContext, pBlockDest, integrityFlag);
			if(retVal != READER_OK) return retVal;
		}
		else if(bType == READER_T1_SBLOCK){
			/* A coder plus tard */
		}
		else{
			return READER_ERR;
		}
	}
	
	
	return READER_OK;
}




READER_Status READER_T1_ERR_DealWithError(READER_T1_ContextHandler *pContext, uint32_t integrityFlag){
	READER_Status retVal;
	READER_T1_Block errorBlock;
	
	
	/* On verifie qu'on a pas depasse le compteur de redemande d'informations */
	retVal = READER_T1_ERR_CheckRepeatCounter(pContext);
	if(retVal == READER_OK){
		/* On forge un Block d'erreur */
		retVal = READER_T1_ERR_ForgeErrorBlock(pContext, &errorBlock, integrityFlag);
		if(retVal != READER_OK) return retVal;
		
		/* On stack le Block d'erreur dans le Buffer */
		retVal = READER_T1_ERR_StackErrorBlock(pContext, &errorBlock);
		if(retVal != READER_OK) return retVal;
		
		/* On incremente le compteur de redemande d'informations */
		retVal = READER_T1_ERR_IncRepeatCounter(pContext);
		if(retVal != READER_OK) return retVal;
	}
	else if(retVal == READER_NO){

		/* On verifie le compteur de Resynchro */
		retVal = READER_T1_ERR_CheckResynchCounter(pContext);
		if(retVal == READER_OK){
			/* On fait une demande de Resynchro */
			retVal = READER_T1_ERR_DoResynch(pContext);
			if(retVal != READER_OK) return retVal;
		}
		else if(retVal == READER_NO){
			/* On procede a une reinitialisation de la carte */
			retVal = READER_T1_ERR_DoReset();
			if(retVal != READER_OK) return retVal;
		}
		else{
			return retVal;
		}
	}
	else{
		return retVal;
	}
	
	return READER_OK;
}



/* Attention, on retourne READER_NO si on a pas pu faire de Resynch (compteur max atteint), c'est le contexte exterieur a la fonction qui doit effectuer le DoReset() */
READER_Status READER_T1_ERR_DoResynchRequ(READER_T1_ContextHandler *pContext){
	READER_Status retVal;
	READER_T1_Block block;
	
	
	/* On verifie le compteur de Resynch                                                    */
	retVal = READER_T1_ERR_CheckResynchCounter(pContext);
	if(retVal != READER_OK) return retVal;
	
	/* On forge un Block de requette de Resynchro                                           */
	retVal = READER_T1_ForgeSBlock(&block, READER_T1_STYPE_RESYNCH_REQU);
	if(retVal != READER_OK) return retVal;
	
	/* On Stack le S-Block ainsi forge dans le Buffer d'envoi                               */
	retVal = READER_T1_BUFFER_Stack(pContext, &block);
	if(retVal != READER_OK) return retVal;
	
	/* On positionne les flags qui indiquent qu'on attend un S-Block en retour ...          */
	retVal = READER_T1_CONTEXT_SetSBlockExpected(pContext, READER_T1_STYPE_RESYNCH_RESP);	
	if(retVal != READER_OK) return retVal;
		
	/* On incremente le compteur de demandes de Resynchro                                   */
	retVal = READER_T1_ERR_IncResynchCounter(pContext);
	if(retVal != READER_OK) return retVal;
	
	return READER_OK;
}
