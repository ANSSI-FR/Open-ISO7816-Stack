/**
 * \file reader_t1_iblock.c
 * \copyright This file is part of the Open-ISO7816-Stack project and is distributed under the MIT license. See LICENSE file in the root directory. 
 * This file provides functions for operating safely on READER_T1_Block data structures in the cases when it is an I-Block.
 */


#include "reader_t1_iblock.h"



READER_Status READER_T1_SetBlockSeqNumber(READER_T1_Block *pBlock, READER_T1_SeqNumber seq){
	/* Voir ISO7816-3 section 11.3.2.2 */
	READER_Status retVal;
	uint8_t currentPCB, newPCB;
	
	currentPCB = READER_T1_GetBlockPCB(pBlock);
	
	if(seq == READER_T1_SEQNUM_ZERO){
		newPCB = currentPCB & 0xBF ;
	}
	else if(seq == READER_T1_SEQNUM_ONE){
		newPCB = currentPCB | (~0xBF);
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
		newPCB = currentPCB | (~0xDF) ;
	}
	else{
		return READER_ERR;
	}
	
	
	retVal = READER_T1_SetBlockPCB(pBlock, newPCB);
	if(retVal != READER_OK) return retVal;
	
	return READER_OK;
}


READER_T1_SeqNumber READER_T1_GetBlockSeqNumber(READER_T1_Block *pBlock){
	/* Voir ISO7816-3 section 11.3.2.2 */
	uint8_t currentPCB;
	
	
	
	currentPCB = READER_T1_GetBlockPCB(pBlock);
	
	if((currentPCB & 0x40) == 0x00){
		return READER_T1_SEQNUM_ZERO;
	}
	else{
		return READER_T1_SEQNUM_ONE;
	}
}


READER_T1_MBit READER_T1_GetBlockMBit(READER_T1_Block *pBlock){
	/* Voir ISO7816-3 section 11.3.2.2 */
	uint8_t currentPCB;
	
	
	currentPCB = READER_T1_GetBlockPCB(pBlock);
	
	if((currentPCB & ~0xDF) == 0x00){
		return READER_T1_MBIT_ZERO;
	}
	else{
		return READER_T1_MBIT_ONE;
	}
}


READER_Status READER_T1_ForgeIBlock(READER_T1_Block *pBlock, uint8_t *data, uint32_t dataSize, READER_T1_SeqNumber seq, READER_T1_MBit mBit, READER_T1_RedundancyType rType){
	READER_Status retVal;
	

	/* On fabrique un Block vierge       */
	retVal = READER_T1_ForgeBlock(pBlock, rType);
	if(retVal != READER_OK) return retVal;
	
	/* On indique que c'est un I-BLOCK   */
	retVal = READER_T1_SetBlockType(pBlock, READER_T1_IBLOCK);
	if(retVal != READER_OK) return retVal;
	
	/* On indique les infos propres aux I-BLOCKs */
	retVal = READER_T1_SetBlockData(pBlock, data, dataSize);
	if(retVal != READER_OK) return retVal;
	
	retVal = READER_T1_SetBlockSeqNumber(pBlock, seq);
	if(retVal != READER_OK) return retVal;
	
	retVal = READER_T1_SetBlockMBit(pBlock, mBit);
	if(retVal != READER_OK) return retVal;
	
	return READER_OK;
}



READER_Status READER_T1_CheckIBlock(READER_T1_Block *pBlock){
	READER_T1_BlockType bType;
	
	
	bType = READER_T1_GetBlockType(pBlock);
	if(bType != READER_T1_IBLOCK){
		return READER_INVALID_BLOCK;
	}
	
	return READER_OK;
}
