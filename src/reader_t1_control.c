#include "reader_t1_control.h"








READER_Status READER_T1_CONTROL_IsSeqNumValid(READER_T1_ContextHandler *pContext, READER_T1_Block *pBlock){
	READER_Status retVal;
	READER_T1_BlockType bType;
	READER_T1_SeqNumber tmpSeqNum;
	uint32_t expectedSeqNum, blockSeqNum;
	
	
	/* On fait des verifications ...                                                    */
	if(pBlock == NULL){
		return READER_ERR;
	}
	
	bType = READER_T1_GetBlockType(pBlock);	
	if(bType != READER_T1_IBLOCK){
		return READER_ERR;
	}
	
	/* On recupere le numero de sequence de l'on attend en provenance de la carte ...   */
	retVal = READER_T1_CONTEXT_GetCardExpectedSeqNum(pContext, &expectedSeqNum);
	if(retVal != READER_OK) return retVal;
	
	/* On recupere le numero de sequence du Block que l'on veut verifier ...            */
	tmpSeqNum = READER_T1_GetBlockSeqNumber(pBlock);
	if(tmpSeqNum == READER_T1_SEQNUM_ONE){
		blockSeqNum = 1;
	}
	else if(tmpSeqNum == READER_T1_SEQNUM_ZERO){
		blockSeqNum = 0;
	}
	else{
		return READER_ERR;
	}
	
	/* On compare les numeros de sequence ...                                           */
	if(expectedSeqNum == blockSeqNum){
		return READER_OK;
	}
	else{
		return READER_NO;
	}
}



READER_Status READER_T1_CONTROL_IsRBlockACK(READER_T1_ContextHandler *pContext, READER_T1_Block *pBlock){
	READER_Status retVal;
	READER_T1_BlockType bType;
	READER_T1_ChainingStatus chainingStatus;
	READER_T1_SeqNumber nextIBlockSeqNum;
	READER_T1_ExpSeqNumber blockSeqNum;
	uint32_t tmpNextIBlockSeqNum, tmpBlockSeqNum;
	
	/* On procede de la maniere suivante (non detaille dans la spec) :                                            */
	/* Du cote du Device, un R-Block recu est considere comme un ACK si et seulement si :                         */
	/*      1) Le Device est en train de chainer le dernier I-Block  (M-Bit du dernier == 1)                      */
	/*      2) Le numero de sequence contenu dans le R-Block correspond au I-Block suivant que l'on veut envoyer  */
	/*      3) Le I-Block suivant existe                                                                          */
	
	
	/* On verifie qu'on a bien un R-Block                                                                         */
	if(pBlock == NULL){
		return READER_ERR;
	}
	
	bType = READER_T1_GetBlockType(pBlock);
	if(bType != READER_T1_RBLOCK){
		return READER_ERR;
	}
	
	/* On verifie que cote Device on est bien en train d'essayer de chainer le dernier I-Block qu'on a envoye     */
	retVal = READER_T1_CONTEXT_DeviceIsChainingLastBlock(pContext, &chainingStatus);
	if(retVal != READER_OK) return retVal;
	
	if(chainingStatus == READER_T1_CHAINING_NO){
		return READER_NO;
	}
	
	/* On regarde le numero de sequence contenu dans le R-Block                                                   */
	blockSeqNum = READER_T1_GetExpectedBlockSeqNumber(pBlock);
	if(blockSeqNum == READER_T1_EXPSEQNUM_ZERO){
		tmpBlockSeqNum = 0;
	}
	else if(blockSeqNum == READER_T1_EXPSEQNUM_ONE){
		tmpBlockSeqNum = 1;
	}
	else{
		return READER_ERR;
	}
	
	/* On regarde le numero de sequence du prochain I-Block que l'on doit envoyer (ie : negation du dernier I-Block envoye) */
	
	
	return READER_OK;
}
