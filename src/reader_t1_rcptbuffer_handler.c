#include "reader_t1_rcptbuffer_handler.h"





READER_Status READER_T1_RCPTBUFF_Init(READER_T1_ContextHandler *pContext){
	pContext->receptionBuff.dataSize = 0;
	
	return READER_OK;
}


READER_Status READER_T1_RCPTBUFF_ExtractDataFromIBlock(READER_T1_ContextHandler *pContext, READER_T1_Block *pBlock){
	READER_Status retVal;
	READER_T1_BlockType bType;
	uint8_t blockLength;
	uint32_t currentIFSD;
	uint32_t i;
	uint8_t *pRcptBuff;
	uint32_t rcptBuffDataSize;
	uint8_t *pBlockDataBuff;
	
	
	/* On fait des verifications elementaires sur le I-Block ...      */
	if(pBlock == NULL){
		return READER_ERR;
	}
	
	bType = READER_T1_GetBlockType(pBlock);
	if(bType != READER_T1_IBLOCK){
		return READER_ERR;
	}
	
	/* On verifie que le LEN du I-Block est bien inferieur au current IFSD ... */
	blockLength = READER_T1_GetBlockLEN(pBlock);
	if((uint32_t)(blockLength) > currentIFSD){
		return READER_ERR;
	}
	
	/* On recopie les donnees du I-Block et on les place dans le Buffer de Reception ...   */
	rcptBuffDataSize = pContext->receptionBuff.dataSize;
	pRcptBuff = pContext->receptionBuff.rawReceptionBuff;
	pBlockDataBuff = READER_T1_GetBlockData(pBlock);
	
	if(rcptBuffDataSize + blockLength > READER_APDU_RESP_MAX_TOTALSIZE){
		return READER_OK;
	}
	
	pRcptBuff = pRcptBuff + rcptBuffDataSize;
	
	for(i=0; i<blockLength; i++){
		pRcptBuff[i] = pBlockDataBuff[i];
	}
	
	
	return READER_OK;
}


READER_Status READER_T1_RCPTBUFF_CreateAPDUResp(READER_T1_ContextHandler *pContext, READER_APDU_Response *pApduResp){
	uint32_t i;
	uint32_t rcptBuffDataSize;
	uint8_t *pRcptBuff;
	uint8_t *pApduRespBuff;

	
	/* On simplifie les notations ...                   */
	rcptBuffDataSize = pContext->receptionBuff.dataSize;
	pRcptBuff = pContext->receptionBuff.rawReceptionBuff;
	pApduRespBuff = pApduResp->dataBytes;
	
	/* On fait des verifications elementaires ...       */
	if(rcptBuffDataSize > READER_APDU_RESP_MAX_TOTALSIZE){
		return READER_ERR;
	}
	
	if(rcptBuffDataSize < 2){        /* Pour fabriquer un APDU Response il faut au minimum SW1 et SW2 sinon il y a un probleme ... */
		return READER_ERR;
	}
	
	/* On recopie les donnees                           */
	for(i=0; i<rcptBuffDataSize-2; i++){
		pApduResp->dataBytes[i] = pRcptBuff[i];
	}
	
	/* On indique SW1 et SW2                            */
	pApduResp->SW1 = pRcptBuff[rcptBuffDataSize - 2];
	pApduResp->SW2 = pRcptBuff[rcptBuffDataSize - 1];
	
	return READER_OK;
}

