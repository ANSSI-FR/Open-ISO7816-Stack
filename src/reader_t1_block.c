#include "reader_t1_block.h"
#include "reader_hal.h"



READER_Status READER_T1_SetBlockSAD(READER_T1_Block *pBlock, uint8_t SAD){
	uint8_t *pCurrentNAD;
	
	if(SAD > 0x07){
		return READER_ERR;
	}
	
	pCurrentNAD = pBlock->blockFrame + READER_T1_BLOCKFRAME_NAD_POSITION;
	*pCurrentNAD = (*pCurrentNAD & 0xF8) | (SAD & 0x07);                              /* Voir ISO7816-3 section 11.3.2.1 */
	
	return READER_OK;
}


READER_Status READER_T1_SetBlockDAD(READER_T1_Block *pBlock, uint8_t DAD){
	uint8_t *pCurrentNAD;
	
	if(DAD > 0x07){
		return READER_ERR;
	}
	
	pCurrentNAD = pBlock->blockFrame + READER_T1_BLOCKFRAME_NAD_POSITION;
	*pCurrentNAD = (*pCurrentNAD & 0x8F) | (4<<(DAD & 0x07));                              /* Voir ISO7816-3 section 11.3.2.1 */
	
	return READER_OK;
}


READER_Status READER_T1_SetBlockNAD(READER_T1_Block *pBlock, uint8_t NAD){
	uint8_t *pCurrentNAD;
	
	
	pCurrentNAD = pBlock->blockFrame + READER_T1_BLOCKFRAME_NAD_POSITION;
	*pCurrentNAD = NAD;                                                                   /* Voir ISO7816-3 section 11.3.2.1 */
	
	return READER_OK;
}


READER_Status READER_T1_SetBlockPCB(READER_T1_Block *pBlock, uint8_t PCB){
	uint8_t *pCurrentPCB;
	
	
	pCurrentPCB = pBlock->blockFrame + READER_T1_BLOCKFRAME_PCB_POSITION;
	*pCurrentPCB = PCB;                                                                   /* Voir ISO7816-3 section 11.3.2.1 */
	
	return READER_OK;
}


READER_Status READER_T1_SetBlockType(READER_T1_Block *pBlock, READER_T1_BlockType type){
	/* Voir ISO7816-3 section 11.3.2.2 */
	uint8_t *pCurrentPCB;
	

	pCurrentPCB = pBlock->blockFrame + READER_T1_BLOCKFRAME_PCB_POSITION;
	
	
	if(type == READER_T1_IBLOCK){
		*pCurrentPCB = (*pCurrentPCB) & (0x7F);       /* On force bit 8 a 0 */
	}
	else if(type == READER_T1_RBLOCK){
		*pCurrentPCB = (*pCurrentPCB | 0x80) & 0xBF;  /* On force bits 8 et 7 a 10 */
	}
	else if(type == READER_T1_SBLOCK){
		*pCurrentPCB = (*pCurrentPCB | 0x80) | 0x40;  /* On force bits 8 et 7 a 11 */
	}
	else{
		return READER_ERR;
	}
	
	return READER_OK;
}


READER_Status READER_T1_SetBlockLEN(READER_T1_Block *pBlock, uint8_t LEN){
	uint8_t *pCurrentLEN;
	
	if(LEN > READER_T1_BLOCK_MAX_DATA_SIZE){
		return READER_ERR;
	}

	pCurrentLEN = pBlock->blockFrame + READER_T1_BLOCKFRAME_LEN_POSITION;
	*pCurrentLEN = LEN;
	
	return READER_OK;
}


READER_Status READER_T1_SetBlockRedundancyType(READER_T1_Block *pBlock, READER_T1_RedundancyType type){
	if((type != READER_T1_CRC) && (type != READER_T1_LRC)){
		return READER_ERR;
	}
	
	pBlock->RedundancyType = type;
	
	return READER_OK;
}


READER_Status READER_T1_SetBlockLRC(READER_T1_Block *pBlock, uint8_t LRC){
	uint8_t currentLEN;	
	uint8_t *pCurrentLRC;
	
	currentLEN = READER_T1_GetBlockLEN(pBlock);
	pCurrentLRC = pBlock->blockFrame + READER_T1_BLOCKFRAME_LEN_POSITION + currentLEN;
	
	*pCurrentLRC = LRC;
	
	return READER_OK;
}


READER_Status READER_T1_SetBlockCRC(READER_T1_Block *pBlock, uint16_t CRC){
	
}


READER_Status READER_T1_SetBlockData(READER_T1_Block *pBlock, uint8_t *data, uint8_t dataSize){
	
	
}
