#include "reader_t1_buffer_handler.h"




READER_Status READER_T1_BUFFER_Init(READER_T1_ContextHandler *pContext){
	READER_T1_BlockBuffer *pBlockBuff;    /* Pointeur sur le buffer */
	READER_Status retVal;
	
	
	retVal = READER_T1_CONTEXT_GetBlockBuff(pContext, &pBlockBuff);   /* On passe le pointeur sur le pointeur */
	if(retVal != REDAER_OK) return retVal;
	
	pBlockBuff->indexBottom = 0;
	pBlockBuff->indexTop = 0;
	pBlockBuff->length = 0;
	
	return READER_OK;
}


READER_Status READER_T1_BUFFER_Clear(READER_T1_ContextHandler *pContext){
	READER_Status retVal;
	
	
	retVal = READER_T1_BUFFER_Init(pContext);
	if(retVal != READER_OK) return retVal;
	
	return READER_OK;
}


READER_Status READER_T1_BUFFER_GetLength(READER_T1_ContextHandler *pContext, uint32_t *length){
	READER_T1_BlockBuffer *pBlockBuff;
	READER_Status retVal;
	//uint32_t indexBottom, indexTop;
	//uint32_t placesUsed;
	
	
	retVal = READER_T1_CONTEXT_GetBlockBuff(pContext, &pBlockBuff);
	if(retVal != READER_OK) return retVal;
	
	//indexBottom = pBlockBuff->indexBottom;
	//indexTop = pBlockBuff->indexTop;
	//
	//if(indexBottom <= indexTop){
	//	placesUsed = (indexTop - indexBottom) + 1;
	//}
	//else{
	//	palcesUsed = STATICBUFF_MAXSIZE - indexBottom;
	//	placesUsed += indexTop + 1;
	//}
	//
	//*length = placesUsed;
	
	*length = pBlockBuff->length;
	
	return READER_OK;
}


READER_Status READER_T1_BUFFER_PlacesLeft(READER_T1_ContextHandler *pContext, uint32_t *places){
	READER_Status retVal;
	uint32_t placesUsed;
	
	
	retVal = READER_T1_BUFFER_PlacesLeft(pContext, &placesUsed);
	if(retVal != READER_OK) return retVal;
	
	*places = STATICBUFF_MAXSIZE - placesUsed;
	
	return READER_OK;
}


READER_Status READER_T1_BUFFER_IsEmpty(READER_T1_ContextHandler *pContext, READER_T1_BufferStatus *pStatus){
	uint32_t length;
	READER_Status retVal;
	
	/* On recupere la taille des donnees dans le buffer */
	retVal = READER_T1_BUFFER_GetLength(pContext, &length);
	if(retVal != READER_OK) return retVal;
	
	/* Le buffer est vide si la taille des donnees est de zero */
	if(length == 0){
		*pStatus = READER_T1_BUFFER_EMPTY;
	}
	else{
		*pStatus = READER_T1_BUFFER_NOTEMPTY;
	}
	
	return READER_OK;
}


READER_Status READER_T1_BUFFER_IsFull(READER_T1_ContextHandler *pContext, READER_T1_BufferStatus *pStatus){
	uint32_t length;
	READER_Status retVal;
	
	retVal = READER_T1_BUFFER_GetLength(pContext, &length);
	if(retVal != READER_OK) return retVal;
	
	if(length == STATICBUFF_MAXSIZE){
		*pStatus = READER_T1_BUFFER_FULL;
	}
	else{
		*pStatus = READER_T1_BUFFER_NOTFULL;
	}
	
	return READER_OK;
}


READER_Status READER_T1_BUFFER_Enqueue(READER_T1_ContextHandler *pContext, READER_T1_Block *pBlock){
	READER_T1_BlockBuffer *pBlockBuffer;
	READER_T1_Block *pBlockTab;
	READER_T1_BufferStatus bStatus;
	uint32_t newTopIndex;
	uint32_t placesLeft;
	READER_Status retVal;
	
	
	/* On verifie  que le buffer n'est pas plein */
	retVal = READER_T1_BUFFER_IsFull(pContext, &bStatus);
	if(retVal != READER_OK) return retVal;
	
	if(bStatus == READER_T1_BUFFER_FULL){
		return READER_ERR;
	}
	
	/* On recupere un pointeur sur la structure du buffer dans le contexte */
	retVal = READER_T1_CONTEXT_GetBlockBuff(pContext, &pBlockBuff);
	if(retVal != READER_OK) return retVal;
	
	/* Dans la stucture du buffer on recupere un pointeur sur le tableau de Blocks */
	pBlockTab = &(pBlockBuff->blockBuff);
	
	/* On calcule le nouveau top index et on place le Block a la position caculee */
	newTopIndex = (pBlockBuff->indexTop + 1) % STATICBUFF_MAXSIZE;
	
	retVal = READER_T1_CopyBlock(&(pBlockTab[newTopIndex]), pBlock);
	if(retVal != READER_OK) return retVal;
	
	pBlockBuff->indexTop = newTopIndex;
	
	/* On mets a jour la length */
	pBlockBuff->length += 1;
	
	
	return READER_OK;
}


READER_Status READER_T1_BUFFER_Dequeue(READER_T1_ContextHandler *pContext, READER_T1_Block *pBlock){
	READER_T1_BlockBuffer *pBlockBuffer;
	READER_T1_Block *pBlockTab;
	READER_T1_BufferStatus bStatus;
	READER_Status retVal;
	uint32_t indexBottom, newBottomIndex, length;
	
	
	/* On verifie  que le buffer n'est pas vide */
	retVal = READER_T1_BUFFER_IsEmpty(pContext, &bStatus);
	if(retVal != READER_OK) return retVal;
	
	if(bStatus == READER_T1_BUFFER_EMPTY){
		return READER_ERR;
	}
	
	/* On recupere un pointeur sur la structure du buffer dans le contexte */
	retVal = READER_T1_CONTEXT_GetBlockBuff(pContext, &pBlockBuff);
	if(retVal != READER_OK) return retVal;
	
	/* Dans la stucture du buffer on recupere un pointeur sur le tableau de Blocks */
	pBlockTab = &(pBlockBuff->blockBuff);
	
	
	/* On recupere le Block qui est pointe par indexBottom */
	indexBottom = pBlockBuff->indexBottom;
	
	retVal = READER_T1_CopyBlock(pBlock, &(pBlockTab[indexBottom]));
	if(retVal != READER_OK) return retVal;
	
	/* On mets a jour la valeur de indexBottom */
	retVal = READER_T1_BUFFER_GetLength(pContext, &length);
	if(retVal != READER_OK) return retVal;
	
	/* Si le Block suivant existe, alors on decale l'index de Bottom. */
	if(length > 1){
		newBottomIndex = (indexBottom + 1) % STATICBUFF_MAXSIZE;
		pBlockBuff->indexBottom = newBottomIndex;
	}
	
	/* On mets a jour la length */
	pBlockBuff->length -= 1;
	
	
	return READER_OK;
}


READER_Status READER_T1_BUFFER_Stack(READER_T1_ContextHandler *pContext, READER_T1_Block *pBlock){
	
}


