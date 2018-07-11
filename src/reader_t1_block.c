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
	/* Voir ISO7816-3 section 11.3.2.2 */
	uint8_t currentPCB;
	
	
	currentPCB = READER_T1_GetBlockPCB(pBlock);
	
	if((currentPCB & 0x80) == 0x00){
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
	uint8_t *pCurrentLEN;
	
	
	pCurrentLEN = pBlock->blockFrame + READER_T1_BLOCKFRAME_LEN_POSITION;
	
	return *pCurrentLEN;
}


READER_T1_RedundancyType READER_T1_GetBlockRedundancyType(const READER_T1_Block *pBlock){
	return pBlock->RedundancyType;
}


uint8_t READER_T1_GetBlockLRC(const READER_T1_Block *pBlock){
	uint8_t currentLEN;	
	uint8_t *pCurrentLRC;
	
	currentLEN = READER_T1_GetBlockLEN(pBlock);
	pCurrentLRC = pBlock->blockFrame + READER_T1_BLOCKFRAME_LEN_POSITION + currentLEN;
	
	return *pCurrentLRC;
}


uint16_t READER_T1_GetBlockCRC(const READER_T1_Block *pBlock){
	uint8_t currentLEN;	
	uint16_t *pCurrentCRC;
	
	currentLEN = READER_T1_GetBlockLEN(pBlock);
	pCurrentCRC = (uint16_t*)(pBlock->blockFrame + READER_T1_BLOCKFRAME_LEN_POSITION + currentLEN);
	
	return *pCurrentCRC;
}


uint8_t* READER_T1_GetBlockData(const READER_T1_Block *pBlock){
	return pBlock->blockFrame + READER_T1_BLOCKFRAME_INF_POSITION;
}


uint8_t READER_T1_ComputeBlockLRC(READER_T1_Block *pBlock){
	uint8_t xorSum;
	uint8_t *data;
	uint8_t dataSize;
	uint32_t i;
	
	
	dataSize = READER_T1_GetBlockLEN(pBlock);
	data = READER_T1_GetBlockData(pBlock);
	
	if(dataSize == 0){
		return 0x00;
	}
	
	if(dataSize == 1){
		return data[0];
	}
	
	xorSum = data[0];
	for(i=1; i<dataSize; i++){
		xorSum = xorSum ^ data[i];
	}
	
	return xorSum;
}


uint16_t READER_T1_ComputeBlockCRC(READER_T1_Block *pBlock){	
	uint16_t crc = 0x0000;
	uint32_t len;
	uint8_t currentByte;
	uint32_t i, j;
	
	
	len = READER_T1_GetBlockSizeWithoutCheck(pBlock);
	
	for(i=0; i<len; i++){
		currentByte = pBlock->blockFrame[i];
		
		crc ^= currentByte;
		
		for(j=0; j<8; j++){
			crc ^= READER_T1_CRC_POLY;
			crc >>= 1;
		}
	}
	
	return crc;
}

uint32_t READER_T1_GetBlockTotalSize(const READER_T1_Block *pBlock){
	READER_T1_RedundancyType checkType;
	uint32_t checkLen;
	uint32_t tmpLen;
	
	
	tmpLen = READER_T1_GetBlockSizeWithoutCheck(pBlock);
	
	checkType = READER_T1_GetBlockRedundancyType(pBlock);
	
	if(checkType == READER_T1_CRC){
		checkLen = 2;
	}
	else{
		checkLen = 1;
	}
	
	return tmpLen + checkLen;
}


uint32_t READER_T1_GetBlockSizeWithoutCheck(const READER_T1_Block *pBlock){
	uint8_t dataLEN;
	
	
	dataLEN = READER_T1_GetBlockLEN(pBlock);
	
	return READER_T1_BLOCKFRAME_INF_POSITION + (uint32_t)dataLEN;
}
