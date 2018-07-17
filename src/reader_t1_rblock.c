#include "reader_t1_rblock.h"
#include "reader_t1_block.h"
#include "reader_hal.h"





READER_Status READER_T1_SetBlockACK(READER_T1_Block *pBlock, READER_T1_ACKType ack){
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


