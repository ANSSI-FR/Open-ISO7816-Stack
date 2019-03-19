#include "reader_t1_sblock.h"




READER_Status READER_T1_ForgeSBlock(READER_T1_Block *pBlock, READER_T1_SBlockType type){
	READER_T1_RedundancyType rType;
	READER_Status retVal;
	
	
	/* On fabrique un Block vierge     */
	rType = READER_HAL_GetRedunancyType();
	retVal = READER_T1_ForgeBlock(pBlock, rType);
	if(retVal != READER_OK) return retVal;
	
	/* On indique de c'est un S-BLOCK  */
	retVal = READER_T1_SetBlockType(pBlock, READER_T1_SBLOCK);
	if(retVal != READER_OK) return retVal;
	
	/* On indique le type de S-BLOCK (RESYNCH, ABORT, ...) */
	retVal = READER_T1_SetBlockSType(pBlock, type);
	if(retVal != READER_OK) return retVal;
	
	return READER_OK;
}



READER_Status READER_T1_SetBlockSType(READER_T1_Block *pBlock, READER_T1_SBlockType type){
	/* Voir ISO7816-3 section 11.3.2.2 */
	READER_Status retVal;
	uint8_t currentPCB, newPCB;
	
	
	currentPCB = READER_T1_GetBlockPCB(pBlock);
	
	switch(type){
		case READER_T1_STYPE_RESYNCH_REQU :
			newPCB = currentPCB & 0xC0 ;
			break;
		case READER_T1_STYPE_RESYNCH_RESP :
			newPCB = currentPCB & 0xD0 ;
			newPCB = newPCB | 0x20;
			break;
		case READER_T1_STYPE_IFS_REQU :
			newPCB = currentPCB & 0xC1 ;
			newPCB = newPCB | 0x01 ;
			break;
		case READER_T1_STYPE_IFS_RESP :
			newPCB = currentPCB & 0xE1 ;
			newPCB = newPCB | 0x21 ;
			break;
		case READER_T1_STYPE_ABORT_REQU :
			newPCB = currentPCB & 0xC2 ;
			newPCB = newPCB | 0x02 ;
			break;
		case READER_T1_STYPE_ABORT_RESP :
			newPCB = currentPCB & 0xE2 ;
			newPCB = newPCB | 0x22 ;
			break;
		case READER_T1_STYPE_WTX_REQU :
			newPCB = currentPCB & 0xC3 ;
			newPCB = newPCB | 0x03 ;
			break;
		case READER_T1_STYPE_WTX_RESP :
			newPCB = currentPCB & 0xE3 ;
			newPCB = newPCB | 0x23;
			break;
		default :
			return READER_ERR;
	}
	
	
	retVal = READER_T1_SetBlockPCB(pBlock, newPCB);
	if(retVal != READER_OK) return retVal;
	
	return READER_OK;
}



READER_T1_SBlockType READER_T1_GetBlockSType(READER_T1_Block *pBlock){
	/* Voir ISO7816-3 section 11.3.2.2 */
	uint8_t currentPCB;
	READER_Status retVal;
	
	
	/* Verifications elementaires sur le Block ...  */
	retVal = READER_T1_CheckSBlock(pBlock);
	if(retVal != READER_OK) return READER_T1_STYPE_ERROR;
	
	/* On regatde le champ PCB du Block ...  */
	currentPCB = READER_T1_GetBlockPCB(pBlock);
	
	if((currentPCB & 0x3F) == 0x00){
		return READER_T1_STYPE_RESYNCH_REQU;
	}
	else if((currentPCB & 0x3F) == 0x20){
		return READER_T1_STYPE_RESYNCH_RESP;
	}
	else if((currentPCB & 0x3F) == 0x01){
		return READER_T1_STYPE_IFS_REQU;
	}
	else if((currentPCB & 0x3F) == 0x21){
		return READER_T1_STYPE_IFS_RESP;
	}
	else if((currentPCB & 0x3F) == 0x02){
		return READER_T1_STYPE_ABORT_REQU;
	}
	else if((currentPCB & 0x3F) == 0x22){
		return READER_T1_STYPE_ABORT_RESP;
	}
	else if((currentPCB & 0x3F) == 0x03){
		return READER_T1_STYPE_WTX_REQU;
	}
	else if((currentPCB & 0x3F) == 0x23){
		return READER_T1_STYPE_WTX_RESP;
	}
	else{
		return READER_T1_STYPE_DEPRECATED;
	}
}


READER_Status READER_T1_SetBlockSPayload(READER_T1_Block *pBlock, uint8_t payload){
	READER_Status retVal;
	
	retVal = READER_T1_SetBlockData(pBlock, &payload, 1);
	if(retVal != READER_OK) return retVal;
	
	
	return READER_OK;
}


uint8_t READER_T1_GetBlockSPayload(READER_T1_Block *pBlock){
	uint8_t *pPayload;
	
	pPayload = READER_T1_GetBlockData(pBlock);
	
	return *pPayload;
}


/* Retourne READER_OK si c'est une S-Block Request, READER_NO si ca n'en n'est pas une. Retounre toute autre valeur en cas d'erreru interne. */
READER_Status READER_T1_IsSBlockRequest(READER_T1_Block *pBlock){
	READER_T1_BlockType bType;
	READER_T1_SBlockType SBlockType;
	
	
	/* Si le Block n'est pas meme un S-Block, alors on en deduit directement que ce n'est pas une S-Block Request ...  */
	bType = READER_T1_GetBlockType(pBlock);
	if(bType != READER_T1_SBLOCK){
		return READER_NO;
	}
	
	
	/* On regarde le type du S-Block ...  */
	SBlockType = READER_T1_GetBlockSType(pBlock);
	if(SBlockType == READER_T1_STYPE_ABORT_REQU){
		return READER_OK;
	}
	else if(SBlockType == READER_T1_STYPE_IFS_REQU){
		return READER_OK;
	}
	else if(SBlockType == READER_T1_STYPE_RESYNCH_REQU){
		return READER_OK;
	}
	else if(SBlockType == READER_T1_STYPE_WTX_REQU){
		return READER_OK;
	}
	else{
		return READER_NO;
	}
}


/* Retourne READER_OK si c'est une S-Block Response, READER_NO si ca n'en n'est pas une. Retounre toute autre valeur en cas d'erreru interne. */
READER_Status READER_T1_IsSBlockResponse(READER_T1_Block *pBlock){
	READER_T1_BlockType bType;
	READER_T1_SBlockType SBlockType;
	
	
	/* Si le Block n'est pas meme un S-Block, alors on en deduit directement que ce n'est pas une S-Block Request ...  */
	bType = READER_T1_GetBlockType(pBlock);
	if(bType != READER_T1_SBLOCK){
		return READER_NO;
	}
	
	
	/* On regarde le type du S-Block ...  */
	SBlockType = READER_T1_GetBlockSType(pBlock);
	if(SBlockType == READER_T1_STYPE_ABORT_RESP){
		return READER_OK;
	}
	else if(SBlockType == READER_T1_STYPE_IFS_RESP){
		return READER_OK;
	}
	else if(SBlockType == READER_T1_STYPE_RESYNCH_RESP){
		return READER_OK;
	}
	else if(SBlockType == READER_T1_STYPE_WTX_RESP){
		return READER_OK;
	}
	else{
		return READER_NO;
	}
}


READER_Status READER_T1_CheckSBlock(READER_T1_Block *pBlock){
	READER_T1_BlockType bType;
	
	
	bType = READER_T1_GetBlockType(pBlock);
	if(bType != READER_T1_SBLOCK){
		return READER_INVALID_BLOCK;
	}
	
	return READER_OK;
}


READER_Status READER_T1_ForgeSBlockResynchRequest(READER_T1_Block *pBlock){
	READER_Status retVal;
	
	
	retVal = READER_T1_ForgeSBlock(pBlock, READER_T1_STYPE_RESYNCH_REQU);
	if(retVal != READER_OK) return retVal;
	
	
	return READER_OK;
}


READER_Status READER_T1_ForgeSBlockResynchResponse(READER_T1_Block *pBlock){
	READER_Status retVal;
	
	
	retVal = READER_T1_ForgeSBlock(pBlock, READER_T1_STYPE_RESYNCH_RESP);
	if(retVal != READER_OK) return retVal;
	
	
	return READER_OK;
}


READER_Status READER_T1_ForgeSBlockIfsRequest(READER_T1_Block *pBlock, uint8_t newIfs){
	READER_Status retVal;
	
	
	retVal = READER_T1_ForgeSBlock(pBlock, READER_T1_STYPE_IFS_REQU);
	if(retVal != READER_OK) return retVal;
	
	retVal = READER_T1_SetBlockSPayload(pBlock, newIfs);
	if(retVal != READER_OK) return retVal;
	
	
	return READER_OK;
}


READER_Status READER_T1_ForgeSBlockIfsResponse(READER_T1_Block *pBlock, uint8_t newIfs){
	READER_Status retVal;
	
	
	retVal = READER_T1_ForgeSBlock(pBlock, READER_T1_STYPE_IFS_RESP);
	if(retVal != READER_OK) return retVal;
	
	retVal = READER_T1_SetBlockSPayload(pBlock, newIfs);
	if(retVal != READER_OK) return retVal;
	
	
	return READER_OK;
}


READER_Status READER_T1_ForgeSBlockAbortRequest(READER_T1_Block *pBlock){
	READER_Status retVal;
	
	
	retVal = READER_T1_ForgeSBlock(pBlock, READER_T1_STYPE_ABORT_REQU);
	if(retVal != READER_OK) return retVal;
	
	
	return READER_OK;
}


READER_Status READER_T1_ForgeSBlockAbortResponse(READER_T1_Block *pBlock){
	READER_Status retVal;
	
	
	retVal = READER_T1_ForgeSBlock(pBlock, READER_T1_STYPE_ABORT_RESP);
	if(retVal != READER_OK) return retVal;
	
	
	return READER_OK;
}


READER_Status READER_T1_ForgeSBlockWtxRequest(READER_T1_Block *pBlock, uint8_t wtMultiplier){
	READER_Status retVal;
	
	
	retVal = READER_T1_ForgeSBlock(pBlock, READER_T1_STYPE_WTX_REQU);
	if(retVal != READER_OK) return retVal;
	
	retVal = READER_T1_SetBlockSPayload(pBlock, wtMultiplier);
	if(retVal != READER_OK) return retVal;
	
	
	return READER_OK;
}


READER_Status READER_T1_ForgeSBlockWtxResponse(READER_T1_Block *pBlock, uint8_t wtMultiplier){
	READER_Status retVal;
	
	
	retVal = READER_T1_ForgeSBlock(pBlock, READER_T1_STYPE_WTX_RESP);
	if(retVal != READER_OK) return retVal;
	
	retVal = READER_T1_SetBlockSPayload(pBlock, wtMultiplier);
	if(retVal != READER_OK) return retVal;
	
	
	return READER_OK;
}



//READER_Status READER_T1_SendBlockRESYNCHRequ(uint32_t timeout){
//	READER_Status retVal;
//	READER_T1_Block block;
//	
//	
//	retVal = READER_T1_ForgeSBlock(&block, READER_T1_STYPE_RESYNCH_REQU);
//	if(retVal != READER_OK) return retVal;
//	
//	retVal = READER_T1_SendBlock(&block, timeout);
//	if(retVal != READER_OK) return retVal;
//	
//	return READER_OK;
//}


//READER_Status READER_T1_RcvBlockRESYNCHResp(uint32_t timeout){
//	READER_Status retVal;
//	READER_T1_Block block;
//	READER_T1_BlockType blockType;
//	READER_T1_SBlockType sBlockType;
//	
//	
//	/* On recoit un Block                            */
//	retVal = READER_T1_RcvBlock(&block, timeout);
//	if(retVal != READER_OK) return retVal;
//	
//	/* On verifie que c'est bien un RESYNCH RESPONSE */
//	blockType = READER_T1_GetBlockType(&block);
//	sBlockType = READER_T1_GetBlockSType(&block);
//	
//	if((blockType == READER_T1_SBLOCK) && (sBlockType == READER_T1_STYPE_RESYNCH_RESP)){
//		return READER_OK;
//	}
//	else{
//		return READER_ERR;
//	}
//}


//READER_Status READER_T1_SendBlockRESYNCHResp(uint32_t timeout){
//	READER_Status retVal;
//	READER_T1_Block block;
//	
//	
//	retVal = READER_T1_ForgeSBlock(&block, READER_T1_STYPE_RESYNCH_RESP);
//	if(retVal != READER_OK) return retVal;
//	
//	retVal = READER_T1_SendBlock(&block, timeout);
//	if(retVal != READER_OK) return retVal;
//	
//	return READER_OK;
//}


//READER_Status READER_T1_ExecuteRESYNCH(uint32_t timeout){
//	READER_Status retVal;
//	
//	
//	retVal = READER_T1_SendBlockRESYNCHRequ(timeout);
//	if(retVal != READER_OK) return retVal;
//	
//	retVal = READER_T1_RcvBlockRESYNCHResp(timeout);
//	if(retVal != READER_OK) return retVal;
//	
//	return READER_OK;
//}







//READER_Status READER_T1_SendBlockABORTRequ(uint32_t timeout){
//	READER_Status retVal;
//	READER_T1_Block block;
//	
//	
//	retVal = READER_T1_ForgeSBlock(&block, READER_T1_STYPE_ABORT_REQU);
//	if(retVal != READER_OK) return retVal;
//	
//	retVal = READER_T1_SendBlock(&block, timeout);
//	if(retVal != READER_OK) return retVal;
//	
//	return READER_OK;
//}
//
//
//READER_Status READER_T1_RcvBlockABORTResp(uint32_t timeout){
//	READER_Status retVal;
//	READER_T1_Block block;
//	READER_T1_BlockType blockType;
//	READER_T1_SBlockType sBlockType;
//	
//	
//	/* On recoit un Block                            */
//	retVal = READER_T1_RcvBlock(&block, timeout);
//	if(retVal != READER_OK) return retVal;
//	
//	/* On verifie que c'est bien un ABORT RESPONSE */
//	blockType = READER_T1_GetBlockType(&block);
//	sBlockType = READER_T1_GetBlockSType(&block);
//	
//	if((blockType == READER_T1_SBLOCK) && (sBlockType == READER_T1_STYPE_ABORT_RESP)){
//		return READER_OK;
//	}
//	else{
//		return READER_ERR;
//	}
//}
//
//
//READER_Status READER_T1_SendBlockABORTResp(uint32_t timeout){
//	READER_Status retVal;
//	READER_T1_Block block;
//	
//	
//	retVal = READER_T1_ForgeSBlock(&block, READER_T1_STYPE_ABORT_RESP);
//	if(retVal != READER_OK) return retVal;
//	
//	retVal = READER_T1_SendBlock(&block, timeout);
//	if(retVal != READER_OK) return retVal;
//	
//	return READER_OK;
//}
//
//
//READER_Status READER_T1_ExecuteABORT(uint32_t timeout){
//	READER_Status retVal;
//	
//	
//	retVal = READER_T1_SendBlockABORTRequ(timeout);
//	if(retVal != READER_OK) return retVal;
//	
//	retVal = READER_T1_RcvBlockABORTResp(timeout);
//	if(retVal != READER_OK) return retVal;
//	
//	return READER_OK;
//}
//
//
//
//
//
//
//
//READER_Status READER_T1_SendBlockIFSRequ(uint8_t requValue, uint32_t timeout){
//	READER_Status retVal;
//	READER_T1_Block block;
//	
//	
//	retVal = READER_T1_ForgeSBlock(&block, READER_T1_STYPE_IFS_REQU);
//	if(retVal != READER_OK) return retVal;
//	
//	retVal = READER_T1_SetBlockSPayload(&block, requValue);
//	if(retVal != READER_OK) return retVal;
//	
//	retVal = READER_T1_SendBlock(&block, timeout);
//	if(retVal != READER_OK) return retVal;
//	
//	return READER_OK;
//}
//
//
//READER_Status READER_T1_RcvBlockIFSResp(uint8_t requValue, uint32_t timeout){
//	READER_Status retVal;
//	READER_T1_Block block;
//	READER_T1_BlockType blockType;
//	READER_T1_SBlockType sBlockType;
//	uint8_t payload;
//	
//	
//	/* On recoit un Block                            */
//	retVal = READER_T1_RcvBlock(&block, timeout);
//	if(retVal != READER_OK) return retVal;
//	
//	/* On verifie que c'est bien un IFS RESPONSE */
//	blockType = READER_T1_GetBlockType(&block);
//	sBlockType = READER_T1_GetBlockSType(&block);
//	
//	if((blockType != READER_T1_SBLOCK) || (sBlockType != READER_T1_STYPE_IFS_RESP)){
//		return READER_ERR;
//	}
//
//	
//	/* On verifie que la payload du S-BLOCK correspond bien au requValue */
//	payload = READER_T1_GetBlockSPayload(&block);
//	if(payload != requValue){
//		return READER_ERR;
//	}
//	
//	return READER_OK;
//}
//
//
//READER_Status READER_T1_SendBlockIFSResp(uint8_t requValue, uint32_t timeout){
//	READER_Status retVal;
//	READER_T1_Block block;
//	
//	
//	retVal = READER_T1_ForgeSBlock(&block, READER_T1_STYPE_IFS_RESP);
//	if(retVal != READER_OK) return retVal;
//	
//	retVal = READER_T1_SetBlockSPayload(&block, requValue);
//	if(retVal != READER_OK) return retVal;
//	
//	retVal = READER_T1_SendBlock(&block, timeout);
//	if(retVal != READER_OK) return retVal;
//	
//	return READER_OK;
//}
//
//
//READER_Status READER_T1_ExecuteIFS(uint8_t requValue, uint32_t timeout){
//	READER_Status retVal;
//	
//	
//	retVal = READER_T1_SendBlockIFSRequ(requValue, timeout);
//	if(retVal != READER_OK) return retVal;
//	
//	retVal = READER_T1_RcvBlockIFSResp(requValue, timeout);
//	if(retVal != READER_OK) return retVal;
//	
//	return READER_OK;
//}









//READER_Status READER_T1_SendBlockWTXRequ(uint8_t requValue, uint32_t timeout){
//	READER_Status retVal;
//	READER_T1_Block block;
//	
//	
//	retVal = READER_T1_ForgeSBlock(&block, READER_T1_STYPE_WTX_REQU);
//	if(retVal != READER_OK) return retVal;
//	
//	retVal = READER_T1_SetBlockSPayload(&block, requValue);
//	if(retVal != READER_OK) return retVal;
//	
//	retVal = READER_T1_SendBlock(&block, timeout);
//	if(retVal != READER_OK) return retVal;
//	
//	return READER_OK;
//}
//
//
//READER_Status READER_T1_RcvBlockWTXResp(uint8_t requValue, uint32_t timeout){
//	READER_Status retVal;
//	READER_T1_Block block;
//	READER_T1_BlockType blockType;
//	READER_T1_SBlockType sBlockType;
//	uint8_t payload;
//	
//	
//	/* On recoit un Block                            */
//	retVal = READER_T1_RcvBlock(&block, timeout);
//	if(retVal != READER_OK) return retVal;
//	
//	/* On verifie que c'est bien un WTX RESPONSE */
//	blockType = READER_T1_GetBlockType(&block);
//	sBlockType = READER_T1_GetBlockSType(&block);
//	
//	if((blockType != READER_T1_SBLOCK) || (sBlockType != READER_T1_STYPE_WTX_RESP)){
//		return READER_ERR;
//	}
//
//	
//	/* On verifie que la payload du S-BLOCK correspond bien au requValue */
//	payload = READER_T1_GetBlockSPayload(&block);
//	if(payload != requValue){
//		return READER_ERR;
//	}
//	
//	return READER_OK;
//}
//
//
//READER_Status READER_T1_SendBlockWTXResp(uint8_t requValue, uint32_t timeout){
//	READER_Status retVal;
//	READER_T1_Block block;
//	
//	
//	retVal = READER_T1_ForgeSBlock(&block, READER_T1_STYPE_WTX_RESP);
//	if(retVal != READER_OK) return retVal;
//	
//	retVal = READER_T1_SetBlockSPayload(&block, requValue);
//	if(retVal != READER_OK) return retVal;
//	
//	retVal = READER_T1_SendBlock(&block, timeout);
//	if(retVal != READER_OK) return retVal;
//	
//	return READER_OK;
//}
//
//
//READER_Status READER_T1_ExecuteWTX(uint8_t requValue, uint32_t timeout){
//	READER_Status retVal;
//	
//	
//	retVal = READER_T1_SendBlockWTXRequ(requValue, timeout);
//	if(retVal != READER_OK) return retVal;
//	
//	retVal = READER_T1_RcvBlockWTXResp(requValue, timeout);
//	if(retVal != READER_OK) return retVal;
//	
//	return READER_OK;
//}


