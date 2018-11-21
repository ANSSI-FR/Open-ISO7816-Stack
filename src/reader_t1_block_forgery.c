#include "reader_t1_block_forgery.h"





READER_Status READER_T1_FORGE_ChainingRBlockForCard(READER_T1_ContextHandler *pContext, READER_T1_Block *pBlockDest){
	uint32_t cardSeqNum, nextSeqNum;
	READER_Status retVal;
	
	
	/* On recupere le expected Sequence Number qu'on attend du prochain I-Block de la carte */
	retVal = READER_T1_CONTEXT_GetCardSeqNum(pContext, &cardSeqNum);
	if(retVal != READER_OK) return retVal;
	
	nextSeqNum = (cardSeqNum + 1) & 0x00000001;     /* Numero de sequence modulo 2 ... */
	
	
	/* On fabrique un R-Block avec ce numero de sequence    */
	retVal = READER_T1_ForgeRBlock(pBlockDest, READER_T1_ACKTYPE_ACK, nextSeqNum);
	if(retVal != READER_OK) return retVal;
	
	
	return READER_OK;
}


READER_Status READER_T1_FORGE_NACKRBlock(READER_T1_ContextHandler *pContext, READER_T1_Block *pBlockDest){
	
}


READER_Status READER_T1_FORGE_NACKCorrCodeRBlock(READER_T1_ContextHandler *pContext, READER_T1_Block *pBlockDest){
	
}


READER_Status READER_T1_FORGE_ACKIBlock(READER_T1_ContextHandler *pContext, READER_T1_Block *pBlockDest){
	
}


READER_Status READER_T1_FORGE_DataIBlock(READER_T1_ContextHandler *pContext, READER_T1_Block *pBlockDest, uint8_t *dataBuff, uint32_t dataSize, uint32_t mBit){
	
}


READER_Status READER_T1_FORGE_SliceDataAndFillBuffer(READER_T1_ContextHandler *pContext, uint8_t *dataBuff, uint32_t dataSize){
	
}


