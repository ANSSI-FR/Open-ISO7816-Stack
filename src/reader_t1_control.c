#include "reader_t1_control.h"




/* Permet de faire des verificatios elementaires sur un R-Block ...       */
/* Retourne  READER_NO si e n'est pas un R-Block valide. READER_OK sinon. */
READER_Status READER_T1_CONTROL_CheckRBlockIsValid(READER_T1_Block *pBlock){
	READER_T1_BlockType bType;
	
	
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


/* Retourne READER_NO si le numero de sequence c'est pas Valide. Retourne READER_ERR en cas d'erreur interne. */
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
	READER_T1_ChainingStatus chainingStatus;
	READER_T1_ExpSeqNumber blockSeqNum;
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
	else if(tmpNextIBlockSeqNum == 1){
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


/* Retourne READER_OK si ce I-Block indique une erreur. READER_NO si il n'indique pas d'erreur. Autre valeur si la fonction a rencontre une erreur. */
READER_Status READER_T1_CONTROL_IsIBlockACK(READER_T1_ContextHandler *pContext, READER_T1_Block *pBlock){
	READER_Status retVal;
	READER_T1_BlockType bType;
	READER_T1_Block *pTmpBlock;
	READER_T1_MBit mBit;
	
	
	/* On procede de la maniere suivante (non detaille spec) :                                */
	/* Un I-Block recu est considere comme un ACK cote Device si et seulement si :            */
	/*      1) Le dernier Block envoye par le Device existe et est un I-Block                 */
	/*      2) Le M-Bit de ce Block est 0 (le device ne faisait pas/plus de chainage)         */
	/*      3) Le numero de sequence du Block recu est correct                                */
	
	
	/* On recupere le type du dernier Block envoye (et on verifie en meme temps si il existe) */
	retVal = READER_T1_CONTEXT_GetLastSentType(pContext, &bType);
	if((retVal != READER_OK) && (retVal != READER_DOESNT_EXIST)) return retVal;
	
	if(retVal == READER_DOESNT_EXIST){
		return READER_NO;
	}
	
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


READER_Status READER_T1_CONTROL_SendBlock(READER_T1_ContextHandler *pContext, READER_T1_Block *pBlock){
	READER_Status retVal;
	READER_T1_Block blockToSend;
	READER_T1_BlockType bType;
	uint32_t currentCWT, currentBGT;
	uint32_t tickLastBlock, extraStartDelay;
	
	
	/* On recupere le Block suivant a envoyer dans le Buffer du Contexte        */
	retVal = READER_T1_BUFFER_Enqueue(pContext, &blockToSend);
	if(retVal != READER_OK) return retVal; 
	
	/* Avant d'envoyer le Block, on calcul le delai a appliquer pour respecter le BGT entre les Blocks */
	retVal = READER_T1_CONTEXT_GetCurrentBGT(pContext, &currentBGT);
	if(retVal != READER_OK) return retVal;
	
	retVal = READER_T1_CONTEXT_GetTickLastBlock(pContext, &tickLastBlock);
	if((retVal != READER_OK) && (retVal != READER_DOESNT_EXIST)) return retVal;
	
	if(retVal == READER_DOESNT_EXIST){
		extraStartDelay = 0;
	}
	else{
		extraStartDelay = tickLastBlock + currentBGT - READER_HAL_GetTick();
	}
	   
	/* On envoie le Block                                                       */
	retVal = READER_T1_CONTEXT_GetCurrentCWT(pContext, &currentCWT);
	if(retVal != READER_OK) return retVal;
	
	retVal = READER_T1_SendBlock(&blockToSend, currentCWT, extraStartDelay, &tickLastBlock);
	if(retVal != READER_OK) return retVal;
	
	/* On mets a jour la date du leading edge du dernier Block envoye ...  */
	retVal = READER_T1_CONTEXT_SetTickLastBlock(pContext, tickLastBlock);
	if(retVal != READER_OK) return retVal;
	
	/* On set le dernier Block envoye                                */
	retVal = READER_T1_CONTEXT_SetLastSent(pContext, &blockToSend);
	if(retVal != READER_OK) return retVal;
	
	/* On recupere le type du Block que l'on vient d'envoyer et selon son type, on mets a jour le contexte de communication .... */
	bType = READER_T1_GetBlockType(&blockToSend);
	
	if(bType == READER_T1_IBLOCK){
		retVal = READER_T1_CONTROL_IBlockSentUpdateContext(pContext, &blockToSend);
		if(retVal != READER_OK) return retVal;		
	}
	else if(bType == READER_T1_RBLOCK){
		retVal = READER_T1_CONTROL_RBlockSentUpdateContext(pContext, &blockToSend);
		if(retVal != READER_OK) return retVal;	
	}
	else if(bType == READER_T1_SBLOCK){
		retVal = READER_T1_CONTROL_SBlockSentUpdateContext(pContext, &blockToSend);
		if(retVal != READER_OK) return retVal;	
	}
	else{
		return READER_ERR;
	}
	
	/* On remets a zero les compteurs d'erreurs                      */
	retVal = READER_T1_ERR_ClearRepeatCounter(pContext);
	if(retVal != READER_OK) return retVal;
	
	retVal = READER_T1_ERR_ClearResynchCounter(pContext);
	if(retVal != READER_OK) return retVal;
	
	
	return READER_OK;
}





READER_Status READER_T1_CONTROL_IBlockSentUpdateContext(READER_T1_ContextHandler *pContext, READER_T1_Block *pBlock){
	READER_Status retVal;
	READER_T1_MBit mBit;
	
	/* On Set le dernier I-Block envoye                                                */
	retVal = READER_T1_CONTEXT_SetLastIBlockSent(pContext, pBlock);
	if(retVal != READER_OK) return retVal;
	
	/* On met a jour le flag de chainage du dernier Block et le flag de situation de chainage                            */
	mBit = READER_T1_GetBlockMBit(pBlock);
	
	if(mBit == READER_T1_MBIT_ONE){
		retVal = READER_T1_CONTEXT_SetDeviceChainingLastBlockFlag(pContext, READER_T1_CHAINING_YES);
		if(retVal != READER_OK) return retVal;
		
		retVal = READER_T1_CONTEXT_SetDeviceChainingSituationFlag(pContext, READER_T1_CHAINING_YES);
		if(retVal != READER_OK) return retVal;
	}
	else if(mBit == READER_T1_MBIT_ZERO){
		retVal = READER_T1_CONTEXT_SetDeviceChainingLastBlockFlag(pContext, READER_T1_CHAINING_NO);
		if(retVal != READER_OK) return retVal;
		
		retVal = READER_T1_CONTEXT_SetDeviceChainingSituationFlag(pContext, READER_T1_CHAINING_NO);
		if(retVal != READER_OK) return retVal;
	}
	else{
		return READER_ERR;
	}	
	
	/* On mets 	a jour le numero de sequence du Device               */
	retVal = READER_T1_CONTEXT_IncDeviceCompleteSeqNum(pContext);
	if(retVal != READER_OK) return retVal;
	
	
	
	return READER_OK;
}



READER_Status READER_T1_CONTROL_RBlockSentUpdateContext(READER_T1_ContextHandler *pContext, READER_T1_Block *pBlock){
	
	
	return READER_OK;
}


READER_Status READER_T1_CONTROL_SBlockSentUpdateContext(READER_T1_ContextHandler *pContext, READER_T1_Block *pBlock){
	
	
	return READER_OK;
}




READER_Status READER_T1_CONTROL_RcvBlock(READER_T1_ContextHandler *pContext, READER_T1_Block *pBlock){
	READER_Status retVal;
	READER_T1_BlockType bType;
	READER_T1_SBlockExpected SBlockExpected;
	READER_T1_SBlockType SBlockExpectedType, SBlockType;
	uint32_t currentCWT, currentBWT;
	uint32_t extraTimeout;
	uint32_t tickLastBlock;
	
	
	/* On recupere les valeurs de timeout a appliquer ... */
	retVal = READER_T1_CONTEXT_GetCurrentCWT(pContext, &currentCWT);
	if(retVal != READER_OK) return retVal;
	
	retVal = READER_T1_CONTEXT_GetCurrentBWT(pContext, &currentBWT);
	if(retVal != READER_OK) return retVal;
	
	/* On calcule le extraTimout pour le premeir caractere du Block que l'on va recevoir. Sert a garantir le BWT  */
	retVal = READER_T1_CONTEXT_GetTickLastBlock(pContext, &tickLastBlock);
	if((retVal != READER_OK) && (retVal != READER_DOESNT_EXIST)) return retVal;
	
	if(retVal == READER_DOESNT_EXIST){
		extraTimeout = currentBWT;
	}
	else{
		extraTimeout = tickLastBlock + currentBWT - READER_HAL_GetTick();
	}
	
	/* On receptionne le Block ... */
	retVal = READER_T1_RcvBlock(pBlock, currentCWT, extraTimeout, &tickLastBlock);    /* La fonction de reception de Block fait remonter la date du leading Edge du Block recu ...  */
	if((retVal != READER_OK) && (retVal != READER_TIMEOUT)) return retVal;
	
	/* On regarde si on a eu un timeout     */
	if(retVal == READER_TIMEOUT){
		retVal = READER_T1_ERR_DealWithError(pContext, 0);
		if(retVal != READER_OK) return retVal;
	}
	
	/* On mets a jour le tick du leading edge du dernier Block recu ...  */
	retVal = READER_T1_CONTEXT_SetTickLastBlock(pContext, tickLastBlock);
	if(retVal != READER_OK) return retVal;
	
	/* On regarde si le Block est corrompu ... */
	retVal = READER_T1_CheckBlockIntegrity(pBlock);
	if((retVal != READER_OK) && (retVal != READER_INTEGRITY)) return retVal;
	
	if(retVal == READER_INTEGRITY){
		retVal = READER_T1_ERR_DealWithError(pContext, 1);
		if(retVal != READER_OK) return retVal;
	}
	
	/* On regarde si on attendait un S-Block ... */
	retVal = READER_T1_CONTEXT_IsSblockExpectedNow(pContext, &SBlockExpected);
	if(retVal != READER_OK) return retVal;
	
	/* On regarde le type du Block recu ... */
	bType = READER_T1_GetBlockType(pBlock);
	if(bType == READER_T1_IBLOCK){
		if(SBlockExpected == READER_T1_SBLOCK_EXPECTED_YES){
			/* Action a decider et coder plus tard (en gros routine de reenvoi de S-Block Request ...) */
		}
		
		retVal = READER_T1_CONTROL_ApplyIBlockRcvd(pContext, pBlock);
		if(retVal != READER_OK) return retVal;
	}
	else if(bType == READER_T1_RBLOCK){
		if(SBlockExpected == READER_T1_SBLOCK_EXPECTED_YES){
			/* Action a decider et coder plus tard (en gros routine de reenvoi de S-Block Request ...) */
		}
		
		retVal = READER_T1_CONTROL_ApplyRBlockRcvd(pContext, pBlock);
		if(retVal != READER_OK) return retVal;
	}
	else if(bType == READER_T1_SBLOCK){
		/* Le type du S-Block recu correspond au type de S-Block que l'ona attendait ? */
		retVal = READER_T1_CONTEXT_GetSBlockExpectedType(pContext, &SBlockExpectedType);
		if(retVal != READER_OK) return retVal;
		
		SBlockType = READER_T1_GetBlockSType(pBlock);
		
		if(SBlockType != SBlockExpectedType){
			/* Action a decider et coder plus tard (en gros routine de reenvoi de S-Block Request ...) */
		}
		
		/* Sinon, si tout va bien ... */
		retVal = READER_T1_CONTROL_ApplySBlockRcvd(pContext, pBlock);
		if(retVal != READER_OK) return retVal;
	}
	else{
		return READER_ERR;
	}
	
	return READER_OK;
}


READER_Status READER_T1_CONTROL_ApplyIBlockRcvd(READER_T1_ContextHandler *pContext, READER_T1_Block *pBlock){
	READER_Status retVal;
	READER_T1_ACKStatus ACKStatus;
	READER_T1_ChainingStatus chainingStatus;
	READER_T1_ChainingStatus globalChainingStatus, lastBlockChainingStatus;
	READER_T1_MBit mBit;
	READER_T1_Block tmpBlock;


	/* On verifie si c'est pertinant de recevoir un I-Block maintenant ... */
	

	/* On verifie le numero de sequence */
	retVal = READER_T1_CONTROL_IsSeqNumValid(pContext, pBlock);
	if((retVal != READER_OK) && (retVal != READER_NO)) return retVal;
	
	if(retVal == READER_NO){
		retVal = READER_T1_ERR_DealWithError(pContext, 0);     /* On Stack le bon error Block dans le Buffer d'envoi et on s'arrete la ... */
		if(retVal != READER_OK) return retVal;
		
		return READER_OK;
	}
	
	
	/* On regarde si ce I-Block est un ACK ...                                                                                      */
	retVal = READER_T1_CONTROL_IsIBlockACK(pContext, pBlock);
	if((retVal != READER_OK) && (retVal != READER_NO)) return retVal;
	
	/* Si ce I-Block est un ACK, on mets a jour la situation de l'ACK dans le contexte (Flag)                                       */
	if(retVal == READER_OK){
		retVal = READER_T1_CONTEXT_SetACKStatus(pContext, READER_T1_ACK);
		if(retVal != READER_OK) return retVal;
	}
	
	/* Si ce I-Block n'est pas un ACK ...                                                                                           */
	else if(retVal == READER_NO){
		/* On regarde si on a deja precedement recu un ACK */
		retVal = READER_T1_CONTEXT_GetACKStatus(pContext, &ACKStatus);
		if(retVal != READER_OK) return retVal;
		
		/* On regarde si on est en situation de chainage   */
		retVal = READER_T1_CONTEXT_CardIsChaining(pContext, &chainingStatus);
		if(retVal != READER_OK) return retVal;
		
		/* On gere les cas problematiques ...                                                                                        */
		/* Si on a pas deja recu d'ACK auparavent et que ce I-Block n'est pas lui meme un ACK ...                                    */
		if(ACKStatus == READER_T1_NOACK){
			retVal = READER_T1_ERR_DoResynch(pContext);
			if(retVal != READER_OK) return retVal;
		}
		
		/* Si ce Block ne constitue pas un ACK, qu'on a deja recu un ACK auparavent, mais qu'on n'est pas en situation de chainage   */
		if((ACKStatus == READER_T1_ACK) && (chainingStatus == READER_T1_CHAINING_NO)){
			retVal = READER_T1_ERR_DoResynch(pContext);
			if(retVal != READER_OK) return retVal;
		}
	}
	else{
		return READER_ERR;
	}
	
	
	/* On extrait les donnees contenues dans le I-Block                                                        */
	retVal = READER_T1_RCPTBUFF_ExtractDataFromIBlock(pContext, pBlock);
	if(retVal != READER_OK) return retVal;
	
	
	/* On regarde le M-Bit de la du Block en provenance de la carte et on mets a jour les flags ...            */
	mBit = READER_T1_GetBlockMBit(pBlock);
	if(mBit == READER_T1_MBIT_ZERO){
		/* On mets a jour la situation de chainage                                                             */
		retVal = READER_T1_CONTEXT_SetCardChainingLastBlockFlag(pContext, READER_T1_CHAINING_NO);
		if(retVal != READER_OK) return retVal;
	}
	else if(mBit == READER_T1_MBIT_ONE){
		/* On mets a jour la situation de chainage                                                             */
		retVal = READER_T1_CONTEXT_SetCardChainingLastBlockFlag(pContext, READER_T1_CHAINING_YES);
		if(retVal != READER_OK) return retVal;
		
		retVal = READER_T1_CONTEXT_SetCardChainingSituationFlag(pContext, READER_T1_CHAINING_YES);
		if(retVal != READER_OK) return retVal;
	}
	else{
		return READER_ERR;
	}
	
	
	/* Si on est en situation globale de chainage ...                                                         */
	retVal = READER_T1_CONTEXT_CardIsChainingLastBlock(pContext, &lastBlockChainingStatus);
	if(retVal != READER_OK) return retVal;
	
	retVal = READER_T1_CONTEXT_CardIsChaining(pContext, &globalChainingStatus);
	if(retVal != READER_OK) return retVal;
	
	
	if(globalChainingStatus == READER_T1_CHAINING_YES){
		if(lastBlockChainingStatus == READER_T1_CHAINING_YES){
			/* On prepare un R-Block de chainage */
			retVal = READER_T1_FORGE_ChainingRBlockForCard(pContext, &tmpBlock);
			if(retVal != READER_OK) return retVal;
			
			retVal = READER_T1_BUFFER_Stack(pContext, &tmpBlock);
			if(retVal != READER_OK) return retVal;
		}
		else if(lastBlockChainingStatus == READER_T1_CHAINING_NO){
			/* On prepare un I-Block d'ACK       */
			retVal = READER_T1_FORGE_ACKIBlock(pContext, &tmpBlock);
			if(retVal != READER_OK) return retVal;
			
			retVal = READER_T1_BUFFER_Stack(pContext, &tmpBlock);
			if(retVal != READER_OK) return retVal;
		}
		else{
			return READER_ERR;
		}
	}
	
	return READER_OK;
}


READER_Status READER_T1_CONTROL_ApplyRBlockRcvd(READER_T1_ContextHandler *pContext, READER_T1_Block *pBlock){
	READER_Status retVal;
	READER_T1_Block *pLastBlock;
	READER_T1_SeqNumber tmpSeqNumRBlock;
	uint32_t seqNumRBlock, seqNumIBlock;
	
	
	/* On verifie si c'est pertinant de recevoir un R-Block maintenant  */
	
	
	/* Ce R-Block est il un ACK d'un precedent Block qu'on a envoye ?  */
	retVal = READER_T1_CONTROL_IsRBlockACK(pContext, pBlock);
	if((retVal != READER_OK) && (retVal != READER_NO)) return retVal;
	
	/* Si ce R-Block est un ACK ...                                        */
	if(retVal == READER_OK){
		retVal = READER_T1_CONTEXT_SetACKStatus(pContext, READER_T1_ACK);
		if(retVal != READER_OK) return retVal;
	}
	/* Si ce R-Block n'est pas un ACK ...                                  */
	else{
		/* On regarde le numero de sequence du R-Block et du dernier I-Block qu'on a envoye ...  */
		tmpSeqNumRBlock = READER_T1_GetBlockSeqNumber(pBlock);
		if(tmpSeqNumRBlock == READER_T1_SEQNUM_ONE){
			seqNumRBlock = 1;
		}
		else if(tmpSeqNumRBlock == READER_T1_SEQNUM_ZERO){
			seqNumRBlock = 0;
		}
		else{
			return READER_ERR;
		}
		
		retVal = READER_T1_CONTEXT_GetLastIBlockSentSeqSum(pContext, &seqNumIBlock);
		if(retVal != READER_OK) return retVal;
		
		/* On compare les numeros de sequence du R-Block et du dernier I-Block envoye ...   */
		if(seqNumRBlock == seqNumIBlock){
			/* On recupere et on prepare le dernier I-Block qu'on a envoye */
			retVal = READER_T1_CONTEXT_GetLastIBlockSent(pContext, &pLastBlock);
			if(retVal != READER_OK) return retVal;
			
			retVal = READER_T1_BUFFER_Stack(pContext, pLastBlock);
			if(retVal != READER_OK) return retVal;
		}
		else{
			/* On recupere et on prepare le dernier Block qu'on a envoye   */
			retVal = READER_T1_CONTEXT_GetLastSent(pContext, &pLastBlock);
			if(retVal != READER_OK) return retVal;
			
			retVal = READER_T1_BUFFER_Stack(pContext, pLastBlock);
			if(retVal != READER_OK) return retVal;
		}
	}
	
	return READER_OK;
}


READER_Status READER_T1_CONTROL_ApplySBlockRcvd(READER_T1_ContextHandler *pContext, READER_T1_Block *pBlock){
	
	/* A coder plus tard ...  */
	
	/* Requ ou Resp ? Un S-Block etait attendu ? Il est du bon Type ? */
	
	
	return READER_OK;
}

