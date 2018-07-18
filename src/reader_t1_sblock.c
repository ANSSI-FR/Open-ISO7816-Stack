#include "reader_t1_sblock.h"
#include "reader_t1_block.h"
#include "reader_hal.h"








READER_Status READER_T1_ForgeSBlock(READER_T1_Block *pBlock, READER_T1_SBlockType type){
	READER_T1_RedundancyType rType;
	READER_Status retVal;
	
	
	rType = READER_HAL_GetRedunancyType();
	retVal = READER_T1_ForgeBlock(pBlock, rType);
	if(retVal != READER_OK) return retVal;
	
	retVal = READER_T1_SetBlock
	
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


