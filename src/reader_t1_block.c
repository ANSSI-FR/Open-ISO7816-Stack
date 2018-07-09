#include "reader_t1_block.h"
#include "reader_hal.h"



READER_Status READER_T1_SetBlockSAD(READER_T1_Block *pBlock, uint8_t SAD){
	uint8_t *currentNAD;
	
	if(SAD > 0x07){
		return READER_ERR;
	}
	
	currentNAD = pBlock->blockFrame + READER_T1_BLOCKFRAME_NAD_POSITION;
	*currentNAD = (currentNAD & 0x07) | (SAD & 0x07);                              /* Voir ISO7816-3 section 11.3.2.1 */
	
	return READER_OK;
}


READER_Status READER_T1_SetBlockDAD(READER_T1_Block *pBlock, uint8_t DAD){
	
}


READER_Status READER_T1_SetBlockNAD(READER_T1_Block *pBlock, uint8_t NAD){
	
}


READER_Status READER_T1_SetBlockPCB(READER_T1_Block *pBlock, uint8_t PCB){
	
}


READER_Status READER_T1_SetBlockType(READER_T1_Block *pBlock, READER_T1_BlockType type){
	
}


READER_Status READER_T1_SetBlockLEN(READER_T1_Block *pBlock, uint8_t LEN){
	
}


READER_Status READER_T1_SetBlockRedundancyType(READER_T1_Block *pBlock, READER_T1_RedundancyType type){
	
	
}


READER_Status READER_T1_SetBlockLRC(READER_T1_Block *pBlock, uint8_t LRC){
	
}


READER_Status READER_T1_SetBlockCRC(READER_T1_Block *pBlock, uint16_t CRC){
	
}


READER_Status READER_T1_SetBlockData(READER_T1_Block *pBlock, uint8_t *data, uint8_t dataSize){
	
	
}
