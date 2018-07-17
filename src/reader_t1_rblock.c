#include "reader_t1_rblock.h"
#include "reader_t1_block.h"
#include "reader_hal.h"





READER_Status READER_T1_SetBlockACKType(READER_T1_Block *pBlock, READER_T1_ACKType ack){
	READER_Status retVal;
	uint8_t currentPCB, newPCB;
	
	
	
	currentPCB = READER_T1_GetBlockPCB(pBlock);
	
	
	
	if(ack == READER_T1_ACKTYPE_ACK){
		newPCB = currentPCB & 0x;
	}
	else if(ack == READER_T1_ACKTYPE_NACK){
		
	}
	else if(ack == READER_T1_ACKTYPE_NACK_CRCLRC){
		
	}
	else{
		return READER_ERR;
	}
	
	
	retVal = READER_T1_SetBlockPCB(pBlock, newPCB);
	if(retVal != READER_OK) return retVal;
	
	return READER_OK;
}



READER_Status READER_T1_SetExpectedBlockSeqNumber(READER_T1_Block *pBlock, READER_T1_SeqNumber seq){
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


READER_T1_SeqNumber READER_T1_GetExpectedBlockSeqNumber(READER_T1_Block *pBlock){
	/* Voir ISO7816-3 section 11.3.2.2 */
	READER_Status retVal;
	uint8_t currentPCB, newPCB;
	
	
	
	currentPCB = READER_T1_GetBlockPCB(pBlock);
	
	if((currentPCB & ~0xBF) == 0x00){
		return READER_T1_SEQNUM_ZERO;
	}
	else{
		return READER_T1_SEQNUM_ONE;
	}
}

