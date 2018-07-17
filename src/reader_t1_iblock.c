#include "reader_t1_iblock.h"
#include "reader_t1_block.h"
#include "reader_hal.h"




READER_Status READER_T1_SetBlockSeqNumber(READER_T1_Block *pBlock, READER_T1_SeqNumber seq){
	/* Voir ISO7816-3 section 11.3.2.2 */
	READER_Status retVal;
	uint8_t currentPCB, newPCB;
	
	currentPCB = READER_T1_GetBlockPCB(pBlock);
	
	if(seq == READER_T1_SEQNUM_ZERO){
		newPCB = currentPCB & 0xBF ;
	}
	else if(seq == READER_T1_SEQNUM_ONE){
		newPCB = currentPCB | ~0xBF ;
	}
	else{
		return READER_ERR;
	}
	
	
	retVal = READER_T1_SetBlockPCB(pBlock, newPCB);
	if(retVal != READER_OK) return retVal;
	
	return READER_OK;
}


READER_Status READER_T1_SetBlockMBit(READER_T1_Block *pBlock, READER_T1_MBit mBit){
	/* Voir ISO7816-3 section 11.3.2.2 */
	READER_Status retVal;
	uint8_t currentPCB, newPCB;
	
	
	
	currentPCB = READER_T1_GetBlockPCB(pBlock);
	
	if(mBit == READER_T1_MBIT_ZERO){
		newPCB = currentPCB & 0xDF ;
	}
	else if(mBit == READER_T1_MBIT_ONE){
		newPCB = currentPCB & ~0xDF ;
	}
	else{
		return READER_ERR;
	}
	
	
	retVal = READER_T1_SetBlockPCB(pBlock, newPCB);
	if(retVal != READER_OK) return retVal;
	
	return READER_OK;
}


READER_T1_SeqNumber READER_T1_GetBlock_SeqNumber(READER_T1_Block *pBlock){
	
}


READER_T1_MBit READER_T1_GetBlockMBit(READER_T1_Block *pBlock){
	
}


READER_Status READER_T1_ForgeIBlock(READER_T1_Block *pBlock, uint8_t *data, uint32_t dataSize, READER_T1_SeqNumber seq, READER_T1_MBit mBit){
	
}
