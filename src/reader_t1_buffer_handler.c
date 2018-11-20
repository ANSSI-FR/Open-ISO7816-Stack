#include "reader_t1_buffer_handler.h"




READER_Status READER_T1_BUFFER_Init(READER_T1_ContextHandler *pContext){
	READER_T1_BlockBuffer *pBlockBuff;    /* Pointeur sur le buffer */
	READER_Status retVal;
	
	
	retVal = READER_T1_CONTEXT_GetBlockBuff(pContext, &pBlockBuff);   /* On passe le pointeur sur le pointeur */
	if(retVal != REDAER_OK) return retVal;
	
	pBlockBuff->indexBottom = 0;
	pBlockBuff->indexTop = 0;
	
	return READER_OK;
}


READER_Status READER_T1_BUFFER_Clear(READER_T1_ContextHandler *pContext){
	READER_Status retVal;
	
	
	retVal = READER_T1_BUFFER_Init(pContext);
	if(retVal != READER_OK) return retVal;
	
	return READER_OK;
}


READER_Status READER_T1_BUFFER_PlacesLeft(READER_T1_ContextHandler *pContext, uint32_t *places){
	READER_T1_BlockBuffer *pBlockBuff;
	READER_Status retVal;
	uint32_t indexBottom, indexTop;
	uint32_t placesUsed;
	
	
	retVal = READER_T1_CONTEXT_GetBlockBuff(pContext, &pBlockBuff);
	if(retVal != READER_OK) return retVal;
	
	indexBottom = pBlockBuff->indexBottom;
	indexTop = pBlockBuff->indexTop;
	
	if(indexBottom <= indexTop){
		placesUsed = (indexTop - indexBottom) + 1;
	}
	else{
		palcesUsed = STATICBUFF_MAXSIZE - indexBottom;
		placesUsed += indexTop + 1;
	}
	
	*places = STATICBUFF_MAXSIZE - placesUsed;
	
	return READER_OK;
}


READER_Status READER_T1_BUFFER_IsEmpty(READER_T1_ContextHandler *pContext, READER_T1_BufferStatus *pStatus){
	READER_T1_BlockBuffer *pBlockBuff;
	READER_Status retVal;
	
	retVal = READER_T1_CONTEXT_GetBlockBuff(pContext, &pBlockBuff);
	if(retVal != READER_OK) return retVal;
	
	if(pBlockBuff->length == 0){
		*pStatus = READER_T1_BUFFER_EMPTY;
	}
	else{
		*pStatus = READER_T1_BUFFER_NOTEMPTY;
	}
	
	return READER_OK;
}


READER_Status READER_T1_BUFFER_IsFull(READER_T1_ContextHandler *pContext, READER_T1_BufferStatus *pStatus){
	READER_T1_BlockBuffer *pBlockBuff;
	READER_Status retVal;
	
	retVal = READER_T1_CONTEXT_GetBlockBuff(pContext, &pBlockBuff);
	if(retVal != READER_OK) return retVal;
	
	if(pBlockBuff->length == STATICBUFF_MAXSIZE){
		*pStatus = READER_T1_BUFFER_FULL;
	}
	else{
		*pStatus = READER_T1_BUFFER_NOTFULL;
	}
	
	return READER_OK;
}


READER_Status READER_T1_BUFFER_Enqueue(READER_T1_ContextHandler *pContext, READER_T1_Block *pBlock){
	
}


READER_Status READER_T1_BUFFER_Dequeue(READER_T1_ContextHandler *pContext, READER_T1_Block *pBlock){
	
}


READER_Status READER_T1_BUFFER_Stack(READER_T1_ContextHandler *pContext, READER_T1_Block *pBlock){
	
}


