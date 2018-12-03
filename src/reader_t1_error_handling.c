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


READER_Status READER_T1_ERR_ClearRepeatCounter(READER_T1_ContextHandler *pContext){
	pContext->repeatCounter = 0;
	
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


READER_Status READER_T1_ERR_ClearResynchCounter(READER_T1_ContextHandler *pContext){
	pContext->resynchCounter = 0;
	
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
	uint32_t deviceCompleteSeqNum, cardCompleteSeqNum;
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


/* Juste un wraper pour READER_T1_FORGE_ErrorBlock */
READER_Status READER_T1_ERR_ForgeErrorBlock(READER_T1_ContextHandler *pContext, READER_T1_Block *pBlockDest, uint32_t integrityFlag){
	READER_Status retVal;
	
	
	retVal = READER_T1_FORGE_ErrorBlock(pContext, pBlockDest, integrityFlag);
	if(retVal != READER_OK) return retVal;
	
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
			retVal = READER_T1_ERR_DoReset(pContext);
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


/* On fait des verifications et on envoie de requette de resynchro, on mets a jour les flags ...  */
/* Attention, on retourne READER_NO si on a pas pu faire de Resynch (compteur max atteint), c'est le contexte exterieur a la fonction qui doit effectuer le DoReset() */
READER_Status READER_T1_ERR_PrepareResynchRequ(READER_T1_ContextHandler *pContext){
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


/* On applique la resynchro sur le contexte de communcation                                    */
READER_Status READER_T1_ERR_DoResynch(READER_T1_ContextHandler *pContext){
	READER_T1_Block *pLastBlock;
	READER_T1_Block tmpBlock;
	READER_Status retVal;
	
	
	/* On recupere un pointeur sur le dernier I-Block envoye                                               */
	retVal = READER_T1_CONTEXT_GetLastIBlockSent(pContext, &pLastBlock);
	if(retVal != READER_OK) return retVal;
	
	/* On fait une copie de ce Block dans une variable locale a la fonction (le contexte va etre reinitialise par la suite) */
	retVal = READER_T1_CopyBlock(&tmpBlock, pLastBlock);
	if(retVal != READER_OK) return retVal;
	
	/* On reinitialise tous les parametres de communication dans le contexte               */
	retVal = READER_T1_CONTEXT_InitContextSettings(pContext);
	if(retVal != READER_OK) return retVal;
	
	/* On enleve les S-Blocks et R-Blocks en sortie du Buffer d'envoi ?                    */
	retVal = READER_T1_BUFFER_StrapControlBlocks(pContext);
	if(retVal != READER_OK) return retVal;
	
	/* On Stack le dernier I-Block qu'on avait envoye sur la pile d'envoi (Rule 6.5) (pour le re-envoyer)  */
	retVal = READER_T1_BUFFER_Stack(pContext, &tmpBlock);
	if(retVal != READER_OK) return retVal;	
	
	return READER_OK;
}


READER_Status READER_T1_ERR_DoReset(READER_T1_ContextHandler *pContext){
	READER_Status retVal;
	
	
	retVal = READER_T1_CONTEXT_Init(pContext);
	if(retVal != READER_OK) return retVal;
	
	retVal = READER_HAL_Init();
	if(retVal != READER_OK) return retVal;
	
	retVal = READER_HAL_DoColdReset();
	if(retVal != READER_OK) return retVal;
	
	return READER_OK;
}


