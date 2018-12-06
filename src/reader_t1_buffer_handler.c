#include "reader_t1_buffer_handler.h"




READER_Status READER_T1_BUFFER_Init(READER_T1_ContextHandler *pContext){
	READER_T1_BlockBuffer *pBlockBuff;    /* Pointeur sur le buffer */
	READER_Status retVal;
	
	
	retVal = READER_T1_CONTEXT_GetBlockBuff(pContext, &pBlockBuff);   /* On passe le pointeur sur le pointeur */
	if(retVal != READER_OK) return retVal;
	
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
	//	palcesUsed = READER_T1_CONTEXT_STATICBUFF_MAXSIZE - indexBottom;
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
	
	
	retVal = READER_T1_BUFFER_GetLength(pContext, &placesUsed);
	if(retVal != READER_OK) return retVal;
	
	*places = READER_T1_CONTEXT_STATICBUFF_MAXSIZE - placesUsed;
	
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
	
	if(length == READER_T1_CONTEXT_STATICBUFF_MAXSIZE){
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
	uint32_t newTopIndex, topIndex;
	READER_Status retVal;
	
	
	/* On verifie  que le buffer n'est pas plein */
	retVal = READER_T1_BUFFER_IsFull(pContext, &bStatus);
	if(retVal != READER_OK) return retVal;
	
	if(bStatus == READER_T1_BUFFER_FULL){
		return READER_FULL;
	}
	
	/* On recupere un pointeur sur la structure du buffer dans le contexte */
	retVal = READER_T1_CONTEXT_GetBlockBuff(pContext, &pBlockBuffer);
	if(retVal != READER_OK) return retVal;
	
	/* Dans la stucture du buffer on recupere un pointeur sur le tableau de Blocks */
	pBlockTab = pBlockBuffer->blockBuff;
	
	topIndex = pBlockBuffer->indexTop;
	/* Attention, important de faire une copie ici. Le Block que l'on stack existe generalement dans le contexte local d'une fonction. Il faut imperativement le copier dans la memoire du contexte. */
	retVal = READER_T1_CopyBlock(pBlockTab+topIndex, pBlock);
	if(retVal != READER_OK) return retVal;
	
	/* On calcule le nouveau top index */
	newTopIndex = (pBlockBuffer->indexTop + 1) % READER_T1_CONTEXT_STATICBUFF_MAXSIZE;
	pBlockBuffer->indexTop = newTopIndex;
	
	/* On mets a jour la length */
	pBlockBuffer->length += 1;
	
	
	return READER_OK;
}


/* Cree une copie du Block a l'emplacement du pointeur de destination ... */
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
		return READER_EMPTY;
	}
	
	/* On recupere un pointeur sur la structure du buffer dans le contexte */
	retVal = READER_T1_CONTEXT_GetBlockBuff(pContext, &pBlockBuffer);
	if(retVal != READER_OK) return retVal;
	
	/* Dans la stucture du buffer on recupere un pointeur sur le tableau de Blocks */
	pBlockTab = pBlockBuffer->blockBuff;
	
	
	/* On recupere le Block qui est pointe par indexBottom */
	indexBottom = pBlockBuffer->indexBottom;
	
	retVal = READER_T1_CopyBlock(pBlock, pBlockTab+indexBottom);
	if(retVal != READER_OK) return retVal;
	
	/* On mets a jour la valeur de indexBottom */
	retVal = READER_T1_BUFFER_GetLength(pContext, &length);
	if(retVal != READER_OK) return retVal;
	
	/* Si le Block suivant existe, alors on decale l'index de Bottom. */
	if(length > 1){
		newBottomIndex = (indexBottom + 1) % READER_T1_CONTEXT_STATICBUFF_MAXSIZE;
		pBlockBuffer->indexBottom = newBottomIndex;
	}
	
	/* On mets a jour la length */
	pBlockBuffer->length -= 1;
	
	
	return READER_OK;
}


READER_Status READER_T1_BUFFER_Stack(READER_T1_ContextHandler *pContext, READER_T1_Block *pBlock){
	READER_T1_BlockBuffer *pBlockBuffer;
	READER_T1_Block *pBlockTab;
	READER_T1_BufferStatus bStatus;
	uint32_t newBottomIndex;
	READER_Status retVal;
	
	
	/* On verifie  que le buffer n'est pas plein */
	retVal = READER_T1_BUFFER_IsFull(pContext, &bStatus);
	if(retVal != READER_OK) return retVal;
	
	if(bStatus == READER_T1_BUFFER_FULL){
		return READER_FULL;
	}
	
	/* On recupere un pointeur sur la structure du buffer dans le contexte */
	retVal = READER_T1_CONTEXT_GetBlockBuff(pContext, &pBlockBuffer);
	if(retVal != READER_OK) return retVal;
	
	/* Dans la stucture du buffer on recupere un pointeur sur le tableau de Blocks */
	pBlockTab = pBlockBuffer->blockBuff;
	
	
	/* On calcule le nouveau IndexBottom et on mets le Block a la position calculee */
	if(pBlockBuffer->indexBottom == 0){
		newBottomIndex = READER_T1_CONTEXT_STATICBUFF_MAXSIZE + pBlockBuffer->indexBottom - 1;
	}
	else{
		newBottomIndex = pBlockBuffer->indexBottom - 1;
	}
	
	/* Attention, important de faire une copie ici. Le Block que l'on stack existe generalement dans le contexte local d'une fonction. Il faut imperativement le copier dans la memoire du contexte. */
	retVal = READER_T1_CopyBlock(&(pBlockTab[newBottomIndex]), pBlock);
	if(retVal != READER_OK) return retVal;
	
	pBlockBuffer->indexBottom = newBottomIndex;
	
	
	
	/* On mets a jour la length */
	pBlockBuffer->length += 1;
	
	return READER_OK;
}


//READER_Status READER_T1_BUFFER_Delete(READER_T1_ContextHandler *pContext, uint32_t index){
//	READER_T1_BlockBuffer *pBlockBuffer;
//	READER_T1_Block *pBlockTab;
//	READER_Status retVal;
//	uint32_t currentIndex;
//	
//	
//	/* On recupere un pointeur sur la structure du buffer dans le contexte            */
//	retVal = READER_T1_CONTEXT_GetBlockBuff(pContext, &pBlockBuffer);
//	if(retVal != READER_OK) return retVal;
//	
//	/* Dans la stucture du Buffer on recupere un pointeur sur le tableau de Blocks    */
//	pBlockTab = &(pBlockBuffer->blockBuff);
//	
//	/* On verifie que le Buffer n'est pas vide ...                                    */
//	
//	
//	/* On regarde si index existe ...                                                 */
//	
//	
//	/*  */
//	
//	
//	return READER_OK;
//}


//READER_Status READER_T1_BUFFER_ShiftInBottomDirection(READER_T1_ContextHandler *pContext){
//	
//}
//
//
//READER_Status READER_T1_BUFFER_ShiftInTopDirection(READER_T1_ContextHandler *pContext){
//	
//}


/* On enleve les R-Blocks et S-BLocks qui se trouvent dans le Buffer */
READER_Status READER_T1_BUFFER_StrapControlBlocks(READER_T1_ContextHandler *pContext){
	READER_Status retVal, retVal2;
	READER_T1_Block tmpBlock;
	READER_T1_BlockType bType;
	
	
	/* On depile les Blocks jusqu'au premier I-Block                               */
	do{
		retVal = READER_T1_BUFFER_Dequeue(pContext, &tmpBlock);
		if((retVal != READER_OK) && (retVal != READER_EMPTY)) return retVal;        /* Si on a une erreur autre que Buffer vide, alors on quitte de suite. */
		                                                                            /* A partir de la, retVal vaut sois READER_OK, sois READER_EMPTY ...   */
		if(retVal != READER_EMPTY){                                                 
			bType = READER_T1_GetBlockType(&tmpBlock);                              
		}
		else{
			bType = READER_T1_BLOCK_ERR;
		}
		
		if(bType == READER_T1_IBLOCK){
			retVal2 = READER_T1_BUFFER_Stack(pContext, &tmpBlock);
			if(retVal2 != READER_OK) return retVal2;
		}                                                                           
	}while((bType != READER_T1_IBLOCK) && (retVal != READER_EMPTY));                /* ie : on s'arrete si on a un I-Block, ou si c'est vide               */
	
	
	return READER_OK;
}

