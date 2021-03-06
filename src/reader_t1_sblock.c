/**
 * \file reader_t1_sblock.c
 * \copyright This file is part of the Open-ISO7816-Stack project and is distributed under the MIT license. See LICENSE file in the root directory. 
 * This file provides functions for operating safely on READER_T1_Block data structures in the cases when it is an S-Block.
 */

#include "reader_t1_sblock.h"



READER_Status READER_T1_ForgeSBlock(READER_T1_Block *pBlock, READER_T1_SBlockType type, READER_T1_RedundancyType rType){
	READER_Status retVal;
	
	
	/* On fabrique un Block vierge     */
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


/* Retourne READER_OK si c'est une S-Block Request, READER_NO si ca n'en n'est pas une. Retourne toute autre valeur en cas d'erreur interne. */
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
