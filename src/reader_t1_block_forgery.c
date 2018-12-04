#include "reader_t1_block_forgery.h"




READER_Status READER_T1_FORGE_ChainingRBlockForCard(READER_T1_ContextHandler *pContext, READER_T1_Block *pBlockDest){
	uint32_t nextSeqNum;
	READER_Status retVal;
	
	
	///* On recupere le expected Sequence Number qu'on attend du prochain I-Block de la carte */
	//retVal = READER_T1_CONTEXT_GetCardSeqNum(pContext, &cardSeqNum);
	//if(retVal != READER_OK) return retVal;
	//
	//nextSeqNum = (cardSeqNum + 1) & 0x00000001;     /* Numero de sequence modulo 2 ... */
	
	/* On recupere le expected Sequence Number qu'on attend du prochain I-Block de la carte */
	retVal = READER_T1_CONTEXT_ComputeNextCardSeqNum(pContext, &nextSeqNum);
	if(retVal != READER_OK) return retVal;
	
	/* On fabrique un R-Block avec ce numero de sequence    */
	retVal = READER_T1_ForgeRBlock(pBlockDest, READER_T1_ACKTYPE_ACK, nextSeqNum);
	if(retVal != READER_OK) return retVal;
	
	
	return READER_OK;
}



READER_Status READER_T1_FORGE_ErrorBlock(READER_T1_ContextHandler *pContext, READER_T1_Block *pBlockDest, uint32_t integrityFlag){
	READER_Status retVal;
	READER_T1_BlockType bType;
	
	
	/* On regarde si c'est la premiere reception  */
	retVal = READER_T1_ERR_IsItFirstReception(pContext);
	
	if((retVal != READER_OK) && (retVal != READER_NO)){                         /* Si une erreur s'est produit au moment ou on verifie que c'est la 1ere reception ... */
		return retVal;
	}
	else if(retVal == READER_OK){    /* Rules 7.5 and 7.6 ... */                /* Si pas d'erreur et que c'est la premiere reception ...     */
		retVal = READER_T1_FORGE_NACKR0(pContext, pBlockDest, integrityFlag);
		if(retVal != READER_OK) return retVal;
	}
	/* Il y a deja eu au moins un echange (1 emission et 1 reception) */
	else{
		/* On recupere le type du dernier Block envoye ...            */
		retVal = READER_T1_CONTEXT_GetLastSentType(pContext, &bType);
		if(retVal != READER_OK) return retVal;
		
		/* Selon le type du dernier Block envoye (rules 7.1, 7.2 rt 7.3) ... */
		if(bType == READER_T1_IBLOCK){
			retVal = READER_T1_FORGE_NACK71(pContext, pBlockDest, integrityFlag);
			if(retVal != READER_OK) return retVal;
		}
		else if(bType == READER_T1_RBLOCK){
			retVal = READER_T1_FORGE_NACK72(pContext, pBlockDest, integrityFlag);
			if(retVal != READER_OK) return retVal;
		}
		else if(bType == READER_T1_SBLOCK){
			/* A coder plus tard */
		}
		else{
			return READER_ERR;
		}
	}
	
	
	return READER_OK;
}



READER_Status READER_T1_FORGE_NACKR0(READER_T1_ContextHandler *pContext, READER_T1_Block *pBlockDest, uint32_t integrityFlag){
	READER_Status retVal;
		
	
	if(integrityFlag == 0){
		retVal = READER_T1_ForgeRBlock(pBlockDest, READER_T1_ACKTYPE_NACK, READER_T1_SEQNUM_ZERO);
		if(retVal != READER_OK) return retVal;
	}
	else if(integrityFlag == 1){
		retVal = READER_T1_ForgeRBlock(pBlockDest, READER_T1_ACKTYPE_NACK_CRCLRC, READER_T1_SEQNUM_ZERO);
		if(retVal != READER_OK) return retVal;
	}
	else{
		return READER_BAD_ARG;
	}
	
	return READER_OK;
}

/* Cas de la Rule 7.1. Ce R-Block fait suite a l'envoi d'un I-Block. C'est une redemande d'un I-Block precis a la carte  */
READER_Status READER_T1_FORGE_NACK71(READER_T1_ContextHandler *pContext, READER_T1_Block *pBlockDest, uint32_t integrityFlag){
	READER_Status retVal;
	READER_T1_SeqNumber seqNum;
	READER_T1_ACKType ACKType;
	READER_T1_Block *pLastSent;
	READER_T1_BlockType bType;
	uint32_t seq;
	
	
	/* On verifie que le dernier Block envoye existe et c'est bien un I-Block                           */
	retVal = READER_T1_CONTEXT_GetLastSent(pContext, &pLastSent);
	if(retVal != READER_OK) return retVal;
	
	bType = READER_T1_GetBlockType(pLastSent);
	if(bType != READER_T1_RBLOCK){
		return READER_ERR;
	}
	
	/*Recuperation a partir du contexte du numero de sequence qu'on attend de la carte ...  */
	retVal = READER_T1_CONTEXT_GetCardSeqNum(pContext, &seq);
	if(retVal != READER_OK) return retVal;
	
	if(seq == 0){
		seqNum = READER_T1_SEQNUM_ZERO;
	}
	else if(seq == 1){
		seqNum = READER_T1_SEQNUM_ONE;
	}
	else{
		return READER_ERR;
	}
	
	
	/* On indique le type de NACK ...                     */
	if(integrityFlag == 0){
		ACKType = READER_T1_ACKTYPE_NACK;
	}
	else if(integrityFlag == 1){
		ACKType = READER_T1_ACKTYPE_NACK_CRCLRC;
	}
	else{
		return READER_BAD_ARG;
	}
	
	/* On forge le R-Block qui correspond ...             */
	retVal = READER_T1_ForgeRBlock(pBlockDest, ACKType, seqNum);
	if(retVal != READER_OK) return retVal;
	
	return READER_OK;
}


READER_Status READER_T1_FORGE_NACK72(READER_T1_ContextHandler *pContext, READER_T1_Block *pBlockDest, uint32_t integrityFlag){
	READER_Status retVal;
	READER_T1_Block *pLastSent;
	READER_T1_BlockType bType;
	
	
	/* Si on est ici, c'est que le Block precedent envoye est un R-Block. */
	/* La Rule 7.2 nous dit qu'il faut renvoyer ce meme R-Block           */
	retVal = READER_T1_CONTEXT_GetLastSent(pContext, &pLastSent);
	if(retVal != READER_OK) return retVal;
	
	
	/* On verifie que le dernier Block envoye est bien un R-Block ...     */
	bType = READER_T1_GetBlockType(pLastSent);
	if(bType != READER_T1_RBLOCK){
		return READER_ERR;
	}
	
	/* On forge un R-Block qui est identique au precedent ...             */
	retVal = READER_T1_CopyBlock(pBlockDest, pLastSent);
	if(retVal != READER_OK) return retVal;
	
	
	return READER_OK;
}


READER_Status READER_T1_FORGE_ACKIBlock(READER_T1_ContextHandler *pContext, READER_T1_Block *pBlockDest){
	READER_Status retVal;
	READER_T1_SeqNumber seqNum;
	uint32_t tmpSeqNum;
	
	
	/* On forge un I-Block vide, mais avec le bon numero de sequence. Il sert a aquitter le dernier Block recu dans une sequence chainee. */
	
	/* On recupere le bon numero de sequence ...       */
	retVal = READER_T1_CONTEXT_GetDeviceSeqNum(pContext, &tmpSeqNum);
	if(retVal != READER_OK) return retVal;
	
	if(tmpSeqNum == 1){
		seqNum = READER_T1_SEQNUM_ONE;
	}
	else if(tmpSeqNum == 0){
		seqNum = READER_T1_SEQNUM_ZERO;
	}
	else{
		return READER_ERR;
	}
	
	/* On forge le I-Block                             */
	retVal = READER_T1_ForgeIBlock(pBlockDest, NULL, 0, seqNum, READER_T1_MBIT_ZERO);
	if(retVal != READER_OK) return retVal;
	
	return READER_OK;
}


/* C'est un peu nul comme fonction ...  */
/* Attention ca se base sur le numero de sequence contenu dans le contexte. Si on appelle deux fois de suite cette fonction alors les Blocks generes auront le meme numero de sequence ....  */
/* Cette fonction permet de fabriquer un I-Block en calculant automatiqumenet le numero de sequence a partir du Contexte ... */
READER_Status READER_T1_FORGE_DataIBlock(READER_T1_ContextHandler *pContext, READER_T1_Block *pBlockDest, uint8_t *dataBuff, uint32_t dataSize, uint32_t mBit){
	uint32_t nextDeviceSeqNum;
	READER_Status retVal;
	READER_T1_MBit tmpMBit;
	
	
	/* On calcule le numero de sequence que l'on doit placer dans le I-Block qu'on va forger ...  */
	retVal = READER_T1_CONTEXT_ComputeNextDeviceSeqNum(pContext, &nextDeviceSeqNum);
	if(retVal != READER_OK) return retVal;
	
	/* On verifie et convertit le M-Bit sous le bon format ...                                    */
	if(mBit == 0){
		tmpMBit = READER_T1_MBIT_ZERO;
	}
	else if(mBit == 1){
		tmpMBit = READER_T1_MBIT_ONE;
	}
	else{
		return READER_ERR;
	}
	
	/* On fabrique un I-Block avec le bon numero de sequence ...                                                                    */
	/* Pas besoin de faire de verifications sur la Size etc... (elles sont deja faites dans la fonction qui forge les I-Blocks ...) */
	retVal = READER_T1_ForgeIBlock(pBlockDest, dataBuff, dataSize, nextDeviceSeqNum, tmpMBit);
	if(retVal != READER_OK) return retVal;
	
	return READER_OK;
}



READER_Status READER_T1_FORGE_SliceDataAndFillBuffer(READER_T1_ContextHandler *pContext, uint8_t *dataBuff, uint32_t dataSize){
	READER_Status retVal;
	READER_T1_Block tmpBlock;
	READER_T1_SeqNumber seqNum;
	READER_T1_MBit mBit;
	uint32_t tmpSeqNum;
	uint32_t currentIFSC;
	uint32_t nbBlocksNeeded, buffPlacesLeft;
	uint32_t bytesCounter;
	uint32_t i;
	
	
	/* On verifie que la qte de donnees passes en parametres ne depassent pas la taille MAX d'un APDU dans l'implem */
	if(dataSize > READER_APDU_CMD_MAX_TOTALSIZE){
		return READER_TOO_LONG;
	}
	
	/* On recupere la qte de donnees max que l'on peut mettre dans un Block (IFSC)  (Voir ISO7816-3 section 11.4.2) */
	retVal = READER_T1_CONTEXT_GetCurrentIFSC(pContext, &currentIFSC);
	if(retVal != READER_OK) return retVal;
	
	/* On verifie que le nombre de Blocks que l'on veut generer peut rentre dans le Buffer de Blocks */
	nbBlocksNeeded = (dataSize / currentIFSC) + 1;
	
	retVal = READER_T1_BUFFER_PlacesLeft(pContext, &buffPlacesLeft);
	if(retVal != READER_OK) return retVal;
	
	if(buffPlacesLeft < nbBlocksNeeded){
		return READER_TOO_LONG;
	}
	
	/* On procede au decoupage et enfilage dans le Buffer ...                              */
	bytesCounter = 0;
	for(i=0; i<nbBlocksNeeded; i+=currentIFSC){
		/* On determine le numero de sequence   */
		retVal = READER_T1_CONTEXT_ComputeNextDeviceSeqNum(pContext, &tmpSeqNum);
		if(retVal != READER_OK) return retVal;
		
		if(tmpSeqNum == 0){
			seqNum = READER_T1_SEQNUM_ZERO;
		}
		else if(tmpSeqNum == 1){
			seqNum = READER_T1_SEQNUM_ONE;
		}
		else{
			return READER_ERR;
		}
		
		/* On determine le M-Bit  (est ce que il y a de la donnee qui suit ? C'est le dernier Block que l'on prepare ?)              */
		if(dataSize-bytesCounter <= currentIFSC){  /* Il reste encore de quoi fabriquer un Block ou moins  */
			
			mBit = READER_T1_MBIT_ZERO;
		}
		else{
			mBit = READER_T1_MBIT_ONE;
		}
		
		/* On forge le I-Block                                                             */
		retVal = READER_T1_ForgeIBlock(&tmpBlock, dataBuff+i, MIN(currentIFSC, dataSize-bytesCounter), seqNum, mBit);
		if(retVal != READER_OK) return retVal;										                                           
												                                           
		/* On enfile le I-Block dans le Buffer                                             */
		retVal = READER_T1_BUFFER_Enqueue(pContext, &tmpBlock);
		if(retVal != READER_OK) return retVal;								                                           
												                                           
		/* On incremente le byte counter                                                   */
		bytesCounter += MIN(currentIFSC, dataSize-bytesCounter);
	}
	
	if(bytesCounter != dataSize){
		return READER_ERR;
	}
	
	return READER_OK;
}



/* A optimiser pour consommer moins de memoire ... */
READER_Status READER_T1_FORGE_FillBuffWithAPDU(READER_T1_ContextHandler *pContext, READER_APDU_Command *pApduCmd){
	/* On pourrait faire une optimisation qui consiste a ne pas reutiliser directement la fonction READER_T1_FORGE_SliceDataAndFillBuffer() pour des raisons d'optimisation de la consomation mememoire (on ne veut pas creeer un buffer intermediaire) */
	READER_Status retVal;
	READER_APDU_ProtocolCase protocolCase;
	
	
	protocolCase = READER_APDU_GetProtocolCase(pApduCmd);
	
	switch(protocolCase){
		case READER_APDU_CASE_1:
			retVal = READER_T1_FORGE_FillBuffWithAPDUCase1(pContext, pApduCmd);
			break;
		case READER_APDU_CASE_2S:
			retVal = READER_T1_FORGE_FillBuffWithAPDUCase2S(pContext, pApduCmd);
			break;
		case READER_APDU_CASE_2E:
			retVal = READER_T1_FORGE_FillBuffWithAPDUCase2E(pContext, pApduCmd);
			break;
		case READER_APDU_CASE_3S:
			retVal = READER_T1_FORGE_FillBuffWithAPDUCase3S(pContext, pApduCmd);
			break;
		case READER_APDU_CASE_3E:
			retVal = READER_T1_FORGE_FillBuffWithAPDUCase3E(pContext, pApduCmd);
			break;
		case READER_APDU_CASE_4S:
			retVal = READER_T1_FORGE_FillBuffWithAPDUCase4S(pContext, pApduCmd);
			break;
		case READER_APDU_CASE_4E:
			retVal = READER_T1_FORGE_FillBuffWithAPDUCase4E(pContext, pApduCmd);
			break;
		default:
			return READER_ERR;
			break;
	}
	
	if(retVal != READER_OK) return retVal;
	
	return READER_OK;
}



READER_Status READER_T1_FORGE_FillBuffWithAPDUCase1(READER_T1_ContextHandler *pContext, READER_APDU_Command *pApduCmd){
	READER_Status retVal;
	uint8_t rawBuff[READER_APDU_HEADER_SIZE];    /* On sait que dans le Case 1 on ne depasse pas 4 en taille */
	
	/* Voir ISO7816-3 section 12.3.2                                                   */
	
	/* On fait des verifications sur l'APDU ...                                        */
	retVal = READER_APDU_CheckCmdValidity(pApduCmd);
	if(retVal != READER_OK) return retVal;
	
	/* On remplit le buffer brut/intermediaire                                         */
	rawBuff[0] = pApduCmd->header.CLA;
	rawBuff[1] = pApduCmd->header.INS;
	rawBuff[2] = pApduCmd->header.P1;
	rawBuff[3] = pApduCmd->header.P2;
	
	/* On decoupe ce buffer en I-Blocks que l'on place dans le Buffer d'envoi ....     */
	retVal = READER_T1_FORGE_SliceDataAndFillBuffer(pContext, rawBuff, READER_APDU_HEADER_SIZE);
	if(retVal != READER_OK) return retVal;
	
	return READER_OK;
}


READER_Status READER_T1_FORGE_FillBuffWithAPDUCase2S(READER_T1_ContextHandler *pContext, READER_APDU_Command *pApduCmd){
	READER_Status retVal;
	uint8_t rawBuff[READER_APDU_HEADER_SIZE + 1];   /* CLA, INS, P1, P2 ET Le */
	
	
	/* Voir ISO7816-3 section 12.3.3                                                   */
	
	/* On fait des verifications sur l'APDU ...                                        */
	retVal = READER_APDU_CheckCmdValidity(pApduCmd);
	
	/* On remplit le buffer brut/intermediaire                                         */
	rawBuff[0] = pApduCmd->header.CLA;
	rawBuff[1] = pApduCmd->header.INS;
	rawBuff[2] = pApduCmd->header.P1;
	rawBuff[3] = pApduCmd->header.P2;
	rawBuff[4] = (uint8_t)(READER_APDU_NeToLe(pApduCmd->body.Ne));
	
	/* On decoupe ce buffer en I-Blocks que l'on place dans le Buffer d'envoi ....     */
	retVal = READER_T1_FORGE_SliceDataAndFillBuffer(pContext, rawBuff, READER_APDU_HEADER_SIZE +1);
	if(retVal != READER_OK) return retVal;
	
	
	return READER_OK;
}


READER_Status READER_T1_FORGE_FillBuffWithAPDUCase2E(READER_T1_ContextHandler *pContext, READER_APDU_Command *pApduCmd){
	/* Voir ISO7816-3 section 12.3.3   et section 12.1.3                               */
	READER_Status retVal;
	uint8_t rawBuff[READER_APDU_HEADER_SIZE + 3];   /* CLA, INS, P1, P2 ET Le (taille 3) */
	
	
	/* Voir ISO7816-3 section 12.3.3                                                   */
	
	/* On fait des verifications sur l'APDU ...                                        */
	retVal = READER_APDU_CheckCmdValidity(pApduCmd);
	
	/* On remplit le buffer brut/intermediaire                                         */
	rawBuff[0] = pApduCmd->header.CLA;
	rawBuff[1] = pApduCmd->header.INS;
	rawBuff[2] = pApduCmd->header.P1;
	rawBuff[3] = pApduCmd->header.P2;
	rawBuff[4] = 0x00;
	rawBuff[5] = (uint8_t)(READER_APDU_NeToLe(pApduCmd->body.Ne)>>8);
	rawBuff[6] = (uint8_t)(READER_APDU_NeToLe(pApduCmd->body.Ne));
	
	/* On decoupe ce buffer en I-Blocks que l'on place dans le Buffer d'envoi ....     */
	retVal = READER_T1_FORGE_SliceDataAndFillBuffer(pContext, rawBuff, READER_APDU_HEADER_SIZE +3);
	if(retVal != READER_OK) return retVal;
	
	
	return READER_OK;
}


READER_Status READER_T1_FORGE_FillBuffWithAPDUCase3S(READER_T1_ContextHandler *pContext, READER_APDU_Command *pApduCmd){
	READER_Status retVal;
	uint8_t rawBuff[READER_APDU_HEADER_SIZE + 1 + READER_APDU_CMD_DATA_MAX_SIZE];      /* Taille Header + Lc + Data */
	uint32_t i;
	
	/* Voir ISO7816-3 section 12.3.4                                                   */
	
	/* On fait des verifications sur l'APDU ...                                        */
	retVal = READER_APDU_CheckCmdValidity(pApduCmd);
	if(retVal != READER_OK) return retVal;
	
	/* On remplit le buffer brut/intermediaire                                         */
	rawBuff[0] = pApduCmd->header.CLA;
	rawBuff[1] = pApduCmd->header.INS;
	rawBuff[2] = pApduCmd->header.P1;
	rawBuff[3] = pApduCmd->header.P2;
	rawBuff[4] = READER_APDU_NcToLc(pApduCmd->body.Nc);
	
	for(i=0; i<pApduCmd->body.Nc; i++){
		rawBuff[i+5] = pApduCmd->body.dataBytes[i];
	}
	
	/* On decoupe ce buffer en I-Blocks que l'on place dans le Buffer d'envoi ....     */
	retVal = READER_T1_FORGE_SliceDataAndFillBuffer(pContext, rawBuff, READER_APDU_HEADER_SIZE +1 +pApduCmd->body.Nc);
	if(retVal != READER_OK) return retVal;
	
	return READER_OK;
}


READER_Status READER_T1_FORGE_FillBuffWithAPDUCase3E(READER_T1_ContextHandler *pContext, READER_APDU_Command *pApduCmd){
	READER_Status retVal;
	uint8_t rawBuff[READER_APDU_HEADER_SIZE + 3 + READER_APDU_CMD_DATA_MAX_SIZE];      /* Taille Header + Lc + Data */
	uint32_t i;
	
	/* Voir ISO7816-3 section 12.3.4                                                   */
	
	/* On fait des verifications sur l'APDU ...                                        */
	retVal = READER_APDU_CheckCmdValidity(pApduCmd);
	if(retVal != READER_OK) return retVal;
	
	/* On remplit le buffer brut/intermediaire                                         */
	rawBuff[0] = pApduCmd->header.CLA;
	rawBuff[1] = pApduCmd->header.INS;
	rawBuff[2] = pApduCmd->header.P1;
	rawBuff[3] = pApduCmd->header.P2;
	rawBuff[4] = 0x00;
	rawBuff[5] = (uint8_t)(READER_APDU_NcToLc(pApduCmd->body.Nc)>>8);
	rawBuff[6] = (uint8_t)(READER_APDU_NcToLc(pApduCmd->body.Nc));
	
	for(i=0; i<pApduCmd->body.Nc; i++){
		rawBuff[i+7] = pApduCmd->body.dataBytes[i];
	}
	
	/* On decoupe ce buffer en I-Blocks que l'on place dans le Buffer d'envoi ....     */
	retVal = READER_T1_FORGE_SliceDataAndFillBuffer(pContext, rawBuff, READER_APDU_HEADER_SIZE +3 +pApduCmd->body.Nc);
	if(retVal != READER_OK) return retVal;
	
	return READER_OK;
}


READER_Status READER_T1_FORGE_FillBuffWithAPDUCase4S(READER_T1_ContextHandler *pContext, READER_APDU_Command *pApduCmd){
	READER_Status retVal;
	uint8_t rawBuff[READER_APDU_HEADER_SIZE + 1 + READER_APDU_CMD_DATA_MAX_SIZE + 1];      /* Taille Header + Lc + Le + Data */
	uint32_t i;
	
	/* Voir ISO7816-3 section 12.3.5                                                   */
	
	/* On fait des verifications sur l'APDU ...                                        */
	retVal = READER_APDU_CheckCmdValidity(pApduCmd);
	if(retVal != READER_OK) return retVal;
	
	/* On remplit le buffer brut/intermediaire                                         */
	rawBuff[0] = pApduCmd->header.CLA;
	rawBuff[1] = pApduCmd->header.INS;
	rawBuff[2] = pApduCmd->header.P1;
	rawBuff[3] = pApduCmd->header.P2;
	rawBuff[4] = READER_APDU_NcToLc(pApduCmd->body.Nc);
	
	for(i=0; i<pApduCmd->body.Nc; i++){
		rawBuff[i+5] = pApduCmd->body.dataBytes[i];
	}
	
	rawBuff[pApduCmd->body.Nc + 5] = READER_APDU_NeToLe(pApduCmd->body.Ne);
	
	/* On decoupe ce buffer en I-Blocks que l'on place dans le Buffer d'envoi ....     */
	retVal = READER_T1_FORGE_SliceDataAndFillBuffer(pContext, rawBuff, READER_APDU_HEADER_SIZE + pApduCmd->body.Nc + 2);
	if(retVal != READER_OK) return retVal;
	
	return READER_OK;
}


READER_Status READER_T1_FORGE_FillBuffWithAPDUCase4E(READER_T1_ContextHandler *pContext, READER_APDU_Command *pApduCmd){
	READER_Status retVal;
	uint8_t rawBuff[READER_APDU_HEADER_SIZE + 3 + READER_APDU_CMD_DATA_MAX_SIZE + 2];      /* Taille Header + 3Lc + Data +  2Le */
	uint32_t i;
	
	/* Voir ISO7816-3 section 12.3.5   12.1.3                                          */
	
	/* On fait des verifications sur l'APDU ...                                        */
	retVal = READER_APDU_CheckCmdValidity(pApduCmd);
	if(retVal != READER_OK) return retVal;
	
	/* On remplit le buffer brut/intermediaire                                         */
	rawBuff[0] = pApduCmd->header.CLA;
	rawBuff[1] = pApduCmd->header.INS;
	rawBuff[2] = pApduCmd->header.P1;
	rawBuff[3] = pApduCmd->header.P2;
	rawBuff[4] = 0x00;
	rawBuff[5] = (uint8_t)(READER_APDU_NcToLc(pApduCmd->body.Nc)>>8);
	rawBuff[6] = (uint8_t)(READER_APDU_NcToLc(pApduCmd->body.Nc));
	
	for(i=0; i<pApduCmd->body.Nc; i++){
		rawBuff[i+7] = pApduCmd->body.dataBytes[i];
	}
	
	rawBuff[pApduCmd->body.Nc + 7] = (uint8_t)(READER_APDU_NeToLe(pApduCmd->body.Ne)>>8);
	rawBuff[pApduCmd->body.Nc + 8] = (uint8_t)(READER_APDU_NeToLe(pApduCmd->body.Ne));
	
	/* On decoupe ce buffer en I-Blocks que l'on place dans le Buffer d'envoi ....     */
	retVal = READER_T1_FORGE_SliceDataAndFillBuffer(pContext, rawBuff, READER_APDU_HEADER_SIZE + pApduCmd->body.Nc + 2 + 3);
	if(retVal != READER_OK) return retVal;
	
	return READER_OK;
}
