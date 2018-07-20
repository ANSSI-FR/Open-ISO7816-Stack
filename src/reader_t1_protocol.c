#include "reader_t1_protocol.h"
#include "reader_hal.h"
#include "reader.h"








READER_Status READER_T1_IsIBlockACK(READER_T1_Block *pPrevIBlock, READER_T1_Block *pRcvdBlock){
	READER_T1_BlockType bType;
	READER_T1_ACKType ackType;
	READER_T1_SeqNumber expectedSeqNum, ackSeqNum;
	READER_T1_MBit prevIBlockMBit;
	
	
	/* On verifie que le previous Block est un I-Block (Sinon ca n'a pas de sens de parler d'ACK) */
	bType = READER_T1_GetBlockType(pPrevIBlock);
	if(bType != READER_T1_IBLOCK) return READER_ERR;
	
	/* On recupere le num de sequence du previous I-Block et son M-Bit*/
	expectedSeqNum = READER_T1_GetBlockSeqNumber(pPrevIBlock);
	prevIBlockMBit = READER_T1_GetBlockMBit(pPrevIBlock);
	
	
	bType = READER_T1_GetBlockType(pRcvdBlock);
	
	if(bType == READER_T1_RBLOCK){
		ackType = READER_T1_GetBlockACKType(pRcvdBlock);
		if(ackType != READER_T1_ACKTYPE_ACK) return READER_NO;
		
		ackSeqNum = READER_T1_GetExpectedBlockSeqNumber(pRcvdBlock);
		if(ackSeqNum != expectedSeqNum) return READER_NO;
	}
	else if(bType == READER_T1_IBLOCK){
		/* On verifie que le M-Bit du previous I-Block est zero (T=1, rule 2.1)*/
		if(prevIBlockMBit != READER_T1_MBIT_ZERO) return READER_NO;
	}
	else{
		return READER_NO;
	}
	
	return READER_OK;
}


READER_Status READER_T1_IsBlockNACK(READER_T1_Block *pPrevIBlock, READER_T1_Block *pRcvdBlock){
	READER_T1_BlockType bType;
	READER_T1_ACKType ackType;
	READER_T1_SeqNumber expectedSeqNum, ackSeqNum;
	
	
	/* On verifie que le previous Block est un I-Block (Sinon ca n'a pas de sens de parler de NACK) */
	bType = READER_T1_GetBlockType(pPrevIBlock);
	if(bType != READER_T1_IBLOCK) return READER_ERR;
	
	/* On recupere le num de sequence du previous I-Block  */
	expectedSeqNum = READER_T1_GetBlockSeqNumber(pPrevIBlock);
	
	
	
	bType = READER_T1_GetBlockType(pRcvdBlock);
	
	if(bType != READER_T1_RBLOCK){
		return READER_NO;
	}
	
	ackType = READER_T1_GetBlockACKType(pRcvdBlock);
	if(((ackType == READER_T1_ACKTYPE_NACK) || (ackType == READER_T1_ACKTYPE_NACK_CRCLRC)){
		/* On verifie que le num de seq du NACK correspond au numero de sequence du I-BLOCK */
		ackSeqNum = READER_T1_GetExpectedBlockSeqNumber(pRcvdBlock);
		if(ackSeqNum != expectedSeqNum){
			 return READER_NO;
		}
		else{
			return READER_OK;
		}
	}
	else if(ackType == READER_T1_ACKTYPE_ACK){
		return READER_NO;
	}
	else{
		return READER_ERR;
	}
}


READER_Status READER_T1_RcvBlockAndCheck(READER_T1_Block *pBlock){
	
}
