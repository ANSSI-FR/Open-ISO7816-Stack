#include "reader_t1_sblock.h"
#include "reader_t1_block.h"
#include "reader_hal.h"








READER_Status READER_T1_ForgeSBlock(READER_T1_Block *pBlock, READER_T1_SBlockType type){
	READER_T1_RedundancyType rType;
	READER_Status retVal;
	
	
	rType = READER_HAL_GetRedunancyType();
	retVal = READER_T1_ForgeBlock(pBlock, rType);
	if(retVal != READER_OK) return retVal;
	
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
			newPCB = currentPCB & 0xC0 ;
			newPCB = newPCB | 0x20;
			break;
		case READER_T1_STYPE_IFS_REQU :
			newPCB = currentPCB & 0xC0 ;
			newPCB = newPCB | 0x01 ;
			break;
		case READER_T1_STYPE_IFS_RESP :
			newPCB = currentPCB & 0xC0 ;
			newPCB = newPCB | 0x21 ;
			break;
		case READER_T1_STYPE_ABORT_REQU :
			newPCB = currentPCB & 0xC0 ;
			newPCB = newPCB | 0x02 ;
			break;
		case READER_T1_STYPE_ABORT_RESP :
			newPCB = currentPCB & 0xC0 ;
			newPCB = newPCB | 0x22 ;
			break;
		case READER_T1_STYPE_WTX_REQU :
			newPCB = currentPCB & 0xC0 ;
			newPCB = newPCB | 0x03 ;
			break;
		case READER_T1_STYPE_WTX_RESP :
			newPCB = currentPCB & 0xC0 ;
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
	
	currentPCB = READER_T1_GetBlockPCB(pBlock);
	
	if(currentPCB & 0x3F == 0x00){
		return READER_T1_STYPE_RESYNCH_REQU;
	}
	else if(currentPCB & 0x3F == 0x20){
		return READER_T1_STYPE_RESYNCH_RESP;
	}
	else if(currentPCB & 0x3F == 0x01){
		return READER_T1_STYPE_IFS_REQU;
	}
	else if(currentPCB & 0x3F == 0x21){
		return READER_T1_STYPE_IFS_RESP;
	}
	else if(currentPCB & 0x3F == 0x02){
		return READER_T1_STYPE_ABORT_REQU;
	}
	else if(currentPCB & 0x3F == 0x22){
		return READER_T1_STYPE_ABORT_RESP;
	}
	else if(currentPCB & 0x3F == 0x03){
		return READER_T1_STYPE_WTX_REQU;
	}
	else if(currentPCB & 0x3F == 0x23){
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

