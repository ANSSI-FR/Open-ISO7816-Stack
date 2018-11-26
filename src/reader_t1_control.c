#include "reader_t1_control.h"








/* Permet de faire des verificatios elementaires sur un R-Block ...       */
/* Retourne  READER_NO si e n'est pas un R-Block valide. READER_OK sinon. */
READER_Status READER_T1_CONTROL_CheckRBlockIsValid(READER_T1_Block *pBlock){
	READER_Status retVal;
	
	
	/* On verifie que le Block passe en parametre existe ... */
	if(pBlock == NULL){
		return READER_NO;
	}
	
	/* On verifie qu'il est bien du type R-Block ...         */
	bType = READER_T1_GetBlockType(pBlock);
	if(bType != READER_T1_RBLOCK){
		return READER_NO;
	}
	
	return READER_OK;
}



READER_Status READER_T1_CONTROL_IsSeqNumValid(READER_T1_ContextHandler *pContext, READER_T1_Block *pBlock){
	READER_Status retVal;
	READER_T1_BlockType bType;
	READER_T1_SeqNumber tmpSeqNum;
	uint32_t expectedSeqNum, blockSeqNum;
	
	
	/* On fait des verifications preliminaires ...                                      */
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


/* Retourne READER_OK si OK, READER_NO si c'est pas un ACK et READER_ERR en cas d'erreur               */
READER_Status READER_T1_CONTROL_IsRBlockACK(READER_T1_ContextHandler *pContext, READER_T1_Block *pBlock){
	READER_Status retVal;
	READER_T1_BlockType bType;
	READER_T1_ChainingStatus chainingStatus;
	READER_T1_SeqNumber nextIBlockSeqNum;
	READER_T1_ExpSeqNumber blockSeqNum;
	READER_T1_Block *pLastBlock;
	uint32_t tmpNextIBlockSeqNum, tmpBlockSeqNum;
	
	/* On procede de la maniere suivante (non detaille dans la spec) :                                            */
	/* Du cote du Device, un R-Block recu est considere comme un ACK si et seulement si :                         */
	/*      1) Le Device est en train de chainer le dernier I-Block  (M-Bit du dernier == 1)                      */
	/*      2) Le numero de sequence contenu dans le R-Block correspond au I-Block suivant que l'on veut envoyer  */
	/*      ?) Le I-Block suivant existe                                                                          */
	
	
	/* On verifie qu'on a bien un R-Block                                                                         */
	retVal = READER_T1_CONTROL_CheckRBlockIsValid(pBlock);
	if(retVal != READER_OK) return retVal;
	
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
	retVal = READER_T1_CONTEXT_GetLastIBlockSentSeqSum(pContext, &tmpNextIBlockSeqNum);
	if(retVal != READER_OK) return retVal;
	
	if(tmpNextIBlockSeqNum == 0){
		tmpNextIBlockSeqNum = 1;
	}
	else if(tmpNExtIBlockSeqNum == 1){
		tmpNextIBlockSeqNum = 0;
	}
	else{
		return READER_ERR;
	}
	
	
	/* On verifie que le numero de sequence contenu dans le R-Block correspond au numero de sequence du prochain I-Block que l'on veut envoyer/chainer */
	if(tmpBlockSeqNum == tmpNextIBlockSeqNum){
		return READER_OK;
	}
	else{
		return READER_NO;
	}
}


/* Retourne READER_OK si ce R-Block indique une erreur. READER_NO si il n'indique pas d'erreur. Autre valeur si la fonction a rencontre une erreur. */
READER_Status READER_T1_CONTROL_IsRBlockError(READER_T1_ContextHandler *pContext, READER_T1_Block *pBlock){
	READER_Status retVal;
	READER_T1_ACKType ackType;
	
	
	retVal = READER_T1_CONTROL_CheckRBlockIsValid(pBlock);
	if(retVal != READER_OK) return retVal;
	
	ackType = READER_T1_GetBlockACKType(pBlock);
	if(ackType == READER_T1_ACKTYPE_ACK){
		return READER_NO;
	}
	else if(ackType == READER_T1_ACKTYPE_NACK){
		return READER_OK;
	}
	else if(ackType == READER_T1_ACKTYPE_NACK_CRCLRC){
		return READER_OK;
	}
	else{
		return READER_ERR;
	}
}


/* Retourne READER_OK si ce R-Block indique une erreur. READER_NO si il n'indique pas d'erreur. Autre valeur si la fonction a rencontre une erreur. */
READER_Status READER_T1_CONTROL_IsIBlockACK(READER_T1_ContextHandler *pContext, READER_T1_Block *pBlock){
	READER_Status retVal;
	READER_T1_BlockType bType;
	READER_T1_Block *pTmpBlock;
	READER_T1_MBit mBit;
	
	
	/* On procede de la maniere suivante (non detaille spec) :                            */
	/* Un I-Block recu est considere comme un ACK cote Device si et seulement si :        */
	/*      1) Le dernier Block envoye par le Device existe et est un I-Block             */
	/*      2) Le M-Bit de ce Block est 0 (le device ne faisait pas/plus de chainage)     */
	/*      3) Le numero de sequence du Block recu est correct                              */
	
	
	/* On recupere le type du dernier Block envoye (et on verifie en meme temps si il existe) */
	retVal = READER_T1_CONTEXT_GetLastSentType(pContext, &bType);
	if(retVal != READER_OK) return retVal;
	
	/* On examine le type du dernier Block envoye ...                                         */
	if(bType == READER_T1_BLOCK_ERR){
		return READER_ERR;
	}
	
	if(bType != READER_T1_IBLOCK){
		return READER_NO;
	}
	
	/* On regarde le M-Bit du dernier I-Block qu'on a envoye           */
	retVal = READER_T1_CONTEXT_GetLastSent(pContext, &pTmpBlock);
	if(retVal != READER_OK) return retVal;
	
	mBit = READER_T1_GetBlockMBit(pTmpBlock);
	
	if(mBit == READER_T1_MBIT_ONE){
		return READER_NO;
	}
	
	/* On verifie que le numero de sequence du I-Block recu est correct  */
	retVal = READER_T1_CONTROL_IsSeqNumValid(pContext, pBlock);
	if(retVal != READER_OK) return retVal;
	
	return READER_OK;
}
