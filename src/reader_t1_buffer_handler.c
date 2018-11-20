#include "reader_t1_buffer_handler.h"




READER_Status READER_T1_BUFFER_Init(READER_T1_ContextHandler *pContext){
	READER_T1_BlockBuffer *pBlockBuff;    /* Pointeur sur le buffer */
	READER_Status retVal;
	
	
	retVal = READER_T1_CONTEXT_GetBlockBuff(pContext, &pBlockBuff);   /* On passe le pointeur sur le pointeur */
	if(retVal != REDAER_OK) return retVal;
	
	pBlockBuff->indexFirst = 0;
	pBlockBuff->indexLast = 0;
	pBlockBuff->flagEmpty = 1;
	pBlockBuff->flagFull = 0;
	pBlockBuff->length = 0;
	
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
	uint32_t indexFirst, indexLast;
	uint32_t placesLeft;
	
	
	retVal = READER_T1_CONTEXT_GetBlockBuff(pContext, &pBlockBuff);
	if(retVal != READER_OK) return retVal;
	
	indexFirst = pBlockBuff->indexFirst;
	indexLast = pBlockBuff->indexLast;
	
	if(indexFirst <= indexLast){
		placesLeft = (indexLast - indexFirst) + 1;
	}
	else{
		palcesLeft = STATICBUFF_MAXSIZE - indexFirst;
		placesLeft += indexLast + 1;
	}
	
	*places = placesLeft;
	
	return READER_OK;
}


READER_Status READER_T1_BUFFER_IsEmpty(READER_T1_ContextHandler *pContext, READER_T1_BufferStatus *pStatus){
	
}


READER_Status READER_T1_BUFFER_IsFull(READER_T1_ContextHandler *pContext, READER_T1_BufferStatus *pStatus){
	
}


READER_Status READER_T1_BUFFER_Enqueue(READER_T1_ContextHandler *pContext, READER_T1_Block *pBlock){
	
}


READER_Status READER_T1_BUFFER_Dequeue(READER_T1_ContextHandler *pContext, READER_T1_Block *pBlock){
	
}


READER_Status READER_T1_BUFFER_Stack(READER_T1_ContextHandler *pContext, READER_T1_Block *pBlock){
	
}


