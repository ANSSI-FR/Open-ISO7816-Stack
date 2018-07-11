#include "reader_t1_block.h"
#include "reader_hal.h"



READER_Status READER_T1_SetBlockSAD(READER_T1_Block *pBlock, uint8_t blockSAD){
	uint8_t *pCurrentNAD;
	
	if(blockSAD > 0x07){
		return READER_ERR;
	}
	
	pCurrentNAD = pBlock->blockFrame + READER_T1_BLOCKFRAME_NAD_POSITION;
	*pCurrentNAD = (*pCurrentNAD & 0xF8) | (blockSAD & 0x07);                              /* Voir ISO7816-3 section 11.3.2.1 */
	
	return READER_OK;
}


READER_Status READER_T1_SetBlockDAD(READER_T1_Block *pBlock, uint8_t blockDAD){
	uint8_t *pCurrentNAD;
	
	if(blockDAD > 0x07){
		return READER_ERR;
	}
	
	pCurrentNAD = pBlock->blockFrame + READER_T1_BLOCKFRAME_NAD_POSITION;
	*pCurrentNAD = (*pCurrentNAD & 0x8F) | (4<<(blockDAD & 0x07));                              /* Voir ISO7816-3 section 11.3.2.1 */
	
	return READER_OK;
}


READER_Status READER_T1_SetBlockNAD(READER_T1_Block *pBlock, uint8_t blockNAD){
	uint8_t *pCurrentNAD;
	
	
	pCurrentNAD = pBlock->blockFrame + READER_T1_BLOCKFRAME_NAD_POSITION;
	*pCurrentNAD = blockNAD;                                                                   /* Voir ISO7816-3 section 11.3.2.1 */
	
	return READER_OK;
}


READER_Status READER_T1_SetBlockPCB(READER_T1_Block *pBlock, uint8_t blockPCB){
	uint8_t *pCurrentPCB;
	
	
	pCurrentPCB = pBlock->blockFrame + READER_T1_BLOCKFRAME_PCB_POSITION;
	*pCurrentPCB = blockPCB;                                                                   /* Voir ISO7816-3 section 11.3.2.1 */
	
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


READER_Status READER_T1_SetBlockLEN(READER_T1_Block *pBlock, uint8_t blockLEN){
	uint8_t *pCurrentLEN;
	
	if(blockLEN > READER_T1_BLOCK_MAX_DATA_SIZE){
		return READER_ERR;
	}

	pCurrentLEN = pBlock->blockFrame + READER_T1_BLOCKFRAME_LEN_POSITION;
	*pCurrentLEN = blockLEN;
	
	return READER_OK;
}


READER_Status READER_T1_SetBlockRedundancyType(READER_T1_Block *pBlock, READER_T1_RedundancyType type){
	if((type != READER_T1_CRC) && (type != READER_T1_LRC)){
		return READER_ERR;
	}
	
	pBlock->RedundancyType = type;
	
	return READER_OK;
}


READER_Status READER_T1_SetBlockLRC(READER_T1_Block *pBlock, uint8_t blockLRC){
	uint8_t currentLEN;	
	uint8_t *pCurrentLRC;
	
	currentLEN = READER_T1_GetBlockLEN(pBlock);
	pCurrentLRC = pBlock->blockFrame + READER_T1_BLOCKFRAME_LEN_POSITION + currentLEN;
	
	*pCurrentLRC = blockLRC;
	
	return READER_OK;
}


READER_Status READER_T1_SetBlockCRC(READER_T1_Block *pBlock, uint16_t blockCRC){
	uint8_t currentLEN;	
	uint16_t *pCurrentCRC;
	
	currentLEN = READER_T1_GetBlockLEN(pBlock);
	pCurrentCRC = (uint16_t*)(pBlock->blockFrame + READER_T1_BLOCKFRAME_LEN_POSITION + currentLEN);
	
	*pCurrentCRC = blockCRC;
	
	return READER_OK;
}


READER_Status READER_T1_SetBlockData(READER_T1_Block *pBlock, uint8_t *data, uint8_t dataSize){
	uint32_t i;
	uint8_t *pBlockData;
	
	
	if(dataSize > READER_T1_BLOCK_MAX_DATA_SIZE){
		return READER_ERR;
	}
	
	
	pBlockData = pBlock->blockFrame + READER_T1_BLOCKFRAME_INF_POSITION;
	
	for(i=0; i<dataSize; i++){
		pBlockData[i] = data[i];
	}
	
	
	return READER_OK;
}








uint8_t READER_T1_GetBlockSAD(const READER_T1_Block *pBlock){
	uint8_t *pCurrentNAD;
	
	
	pCurrentNAD = pBlock->blockFrame + READER_T1_BLOCKFRAME_NAD_POSITION;
	
	return (*pCurrentNAD) & 0x07;
}


uint8_t READER_T1_GetBlockDAD(const READER_T1_Block *pBlock){
	uint8_t *pCurrentNAD;
	
	
	pCurrentNAD = pBlock->blockFrame + READER_T1_BLOCKFRAME_NAD_POSITION;
	
	return (*pCurrentDAD & 0xE0) >> 4;
}


uint8_t READER_T1_GetBlockNAD(const READER_T1_Block *pBlock){
	uint8_t *pCurrentNAD;
	
	
	pCurrentNAD = pBlock->blockFrame + READER_T1_BLOCKFRAME_NAD_POSITION;
	
	return *pCurrentNAD;
}


uint8_t READER_T1_GetBlockPCB(const READER_T1_Block *pBlock){
	uint8_t *pCurrentPCB;
	
	
	pCurrentPCB = pBlock->blockFrame + READER_T1_BLOCKFRAME_PCB_POSITION;
	
	return *pCurrentPCB;
}


READER_T1_BlockType READER_T1_GetBlockType(const READER_T1_Block *pBlock){
	uint8_t currentPCB;
	
	
	currentPCB = READER_T1_GetBlockPCB(pBlock);
	
	if((currentPCB & 0x80) == 0){
		return READER_T1_IBLOCK;
	}
	else if((currentPCB & 0xC0) == 0x80){
		return READER_T1_RBLOCK;
	}
	else if((currentPCB & 0xC0) == 0xC0){
		return READER_T1_SBLOCK;
	}
	else{
		return READER_T1_BLOCK_ERR;
	}
}


uint8_t READER_T1_GetBlockLEN(const READER_T1_Block *pBlock){
	
}


READER_T1_RedundancyType READER_T1_GetBlockRedundancyType(const READER_T1_Block *pBlock){
	
}


uint8_t READER_T1_GetBlockLRC(const READER_T1_Block *pBlock){
	
}


uint16_t READER_T1_GetBlockCRC(const READER_T1_Block *pBlock){
	
}


uint8_t* READER_T1_GetBlockData(const READER_T1_Block *pBlock){
	
}
