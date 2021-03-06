/**
 * \file reader_t1_rblock.c
 * \copyright This file is part of the Open-ISO7816-Stack project and is distributed under the MIT license. See LICENSE file in the root directory. 
 * This file provides functions for operating safely on READER_T1_Block data structures in the cases when it is an R-Block.
 */


#include "reader_t1_rblock.h"



READER_Status READER_T1_SetBlockACKType(READER_T1_Block *pBlock, READER_T1_ACKType ack){
	READER_Status retVal;
	uint8_t currentPCB, newPCB;
	
	
	currentPCB = READER_T1_GetBlockPCB(pBlock);
	
	if(ack == READER_T1_ACKTYPE_ACK){
		newPCB = currentPCB & 0xD0;
	}
	else if(ack == READER_T1_ACKTYPE_NACK){
		newPCB = currentPCB & 0xD0;
		newPCB = newPCB | 0x02;
	}
	else if(ack == READER_T1_ACKTYPE_NACK_CRCLRC){
		newPCB = currentPCB & 0xD0;
		newPCB = newPCB | 0x01;
	}
	else{
		return READER_ERR;
	}
	
	retVal = READER_T1_SetBlockPCB(pBlock, newPCB);
	if(retVal != READER_OK) return retVal;
	
	return READER_OK;
}


READER_T1_ACKType READER_T1_GetBlockACKType(READER_T1_Block *pBlock){
	READER_Status retVal;
	uint8_t currentPCB;
	
	
	/* Verifications elementaires sur le Block que l'on manipule (sensé etre un R-Block) ...  */
	retVal = READER_T1_CheckRBlock(pBlock);
	if(retVal != READER_OK) return retVal;
	
	/* Recuperation du PCB Byte du R-Block ...  */
	currentPCB = READER_T1_GetBlockPCB(pBlock);
	
	/* Check du type d'ACK ...  */
	if((currentPCB & 0x0F) == 0x00){
		return READER_T1_ACKTYPE_ACK;
	}
	else if((currentPCB & 0x0F) == 0x01){
		return READER_T1_ACKTYPE_NACK_CRCLRC;
	}
	else{
		return READER_T1_ACKTYPE_NACK;
	}
}


READER_Status READER_T1_SetExpectedBlockSeqNumber(READER_T1_Block *pBlock, READER_T1_ExpSeqNumber seq){
	/* Voir ISO7816-3 section 11.3.2.2 */
	READER_Status retVal;
	uint8_t currentPCB, newPCB;
	
	currentPCB = READER_T1_GetBlockPCB(pBlock);
	
	if(seq == READER_T1_EXPSEQNUM_ZERO){
		newPCB = currentPCB & 0xEF ;
	}
	else if(seq == READER_T1_EXPSEQNUM_ONE){
		newPCB = currentPCB | 0x10 ;
	}
	else{
		return READER_ERR;
	}
	
	
	retVal = READER_T1_SetBlockPCB(pBlock, newPCB);
	if(retVal != READER_OK) return retVal;
	
	return READER_OK;
}


READER_T1_ExpSeqNumber READER_T1_GetExpectedBlockSeqNumber(READER_T1_Block *pBlock){
	/* Voir ISO7816-3 section 11.3.2.2 */
	uint8_t currentPCB;
	
	
	currentPCB = READER_T1_GetBlockPCB(pBlock);
	
	if((currentPCB & 0x10) == 0x00){
		return READER_T1_EXPSEQNUM_ZERO;
	}
	else{
		return READER_T1_EXPSEQNUM_ONE;
	}
}



READER_Status READER_T1_ForgeRBlock(READER_T1_Block *pBlock, READER_T1_ACKType ack, READER_T1_ExpSeqNumber expctdBlockSeq, READER_T1_RedundancyType rType){
	READER_Status retVal;
	
	/* On fabrique un Block vierge      */
	retVal = READER_T1_ForgeBlock(pBlock, rType);
	if(retVal != READER_OK) return retVal;
	
	/* On indique que c'est un R-BLOCK  */
	retVal = READER_T1_SetBlockType(pBlock, READER_T1_RBLOCK);
	if(retVal != READER_OK) return retVal;
	
	/* On indique les infos propres aux R-BLOCKs */
	retVal = READER_T1_SetBlockACKType(pBlock, ack);
	if(retVal != READER_OK) return retVal;
	
	retVal = READER_T1_SetExpectedBlockSeqNumber(pBlock, expctdBlockSeq);
	if(retVal != READER_OK) return retVal;
	
	return READER_OK;
}

READER_Status READER_T1_CheckRBlock(READER_T1_Block *pBlock){
	READER_T1_BlockType bType;
	
	
	bType = READER_T1_GetBlockType(pBlock);
	if(bType != READER_T1_RBLOCK){
		return READER_INVALID_BLOCK;
	}
	
	return READER_OK;
}
