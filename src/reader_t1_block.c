#include "reader_t1_block.h"
#include "reader_hal.h"



READER_Status READER_T1_SetBlockSAD(READER_T1_Block *pBlock, uint8_t blockSAD){
	READER_Status retVal;
	uint8_t *pCurrentNAD;
	uint8_t *blockFrame;

	
	if(blockSAD > 0x07){
		return READER_ERR;
	}
	
	blockFrame = READER_T1_GetBlockFrame(pBlock);
	
	pCurrentNAD = blockFrame + READER_T1_BLOCKFRAME_NAD_POSITION;
	*pCurrentNAD = (*pCurrentNAD & 0xF8) | (blockSAD & 0x07);                              /* Voir ISO7816-3 section 11.3.2.1 */
	
	/* On met a jour le checksum du Block */
	retVal = READER_T1_UpdateBlockChecksum(pBlock);
	if(retVal != READER_OK) return retVal;
	
	return READER_OK;
}


READER_Status READER_T1_SetBlockDAD(READER_T1_Block *pBlock, uint8_t blockDAD){
	READER_Status retVal;
	uint8_t *pCurrentNAD;
	uint8_t *blockFrame;
	
	if(blockDAD > 0x07){
		return READER_ERR;
	}
	
	blockFrame = READER_T1_GetBlockFrame(pBlock);
	
	pCurrentNAD = blockFrame + READER_T1_BLOCKFRAME_NAD_POSITION;
	*pCurrentNAD = (*pCurrentNAD & 0x8F) | (4<<(blockDAD & 0x07));                              /* Voir ISO7816-3 section 11.3.2.1 */
	
	/* On met a jour le checksum du Block */
	retVal = READER_T1_UpdateBlockChecksum(pBlock);
	if(retVal != READER_OK) return retVal;
	
	return READER_OK;
}


READER_Status READER_T1_SetBlockNAD(READER_T1_Block *pBlock, uint8_t blockNAD){
	READER_Status retVal;
	uint8_t *pCurrentNAD;
	uint8_t *blockFrame;
	
	
	blockFrame = READER_T1_GetBlockFrame(pBlock);
	
	pCurrentNAD = blockFrame + READER_T1_BLOCKFRAME_NAD_POSITION;
	*pCurrentNAD = blockNAD;                                                                   /* Voir ISO7816-3 section 11.3.2.1 */
	
	/* On met a jour le checksum du Block */
	retVal = READER_T1_UpdateBlockChecksum(pBlock);
	if(retVal != READER_OK) return retVal;
	
	return READER_OK;
}


READER_Status READER_T1_SetBlockPCB(READER_T1_Block *pBlock, uint8_t blockPCB){
	READER_Status retVal;
	uint8_t *pCurrentPCB;
	uint8_t *blockFrame;
	
	
	blockFrame = READER_T1_GetBlockFrame(pBlock);
	
	pCurrentPCB = blockFrame + READER_T1_BLOCKFRAME_PCB_POSITION;
	*pCurrentPCB = blockPCB;                                                                   /* Voir ISO7816-3 section 11.3.2.1 */
	
	/* On met a jour le checksum du Block */
	retVal = READER_T1_UpdateBlockChecksum(pBlock);
	if(retVal != READER_OK) return retVal;
	
	return READER_OK;
}


READER_Status READER_T1_SetBlockType(READER_T1_Block *pBlock, READER_T1_BlockType type){
	/* Voir ISO7816-3 section 11.3.2.2 */
	READER_Status retVal;
	uint8_t currentPCB, newPCB;
	
	
	currentPCB = READER_T1_GetBlockPCB(pBlock);
	
	
	if(type == READER_T1_IBLOCK){
		newPCB = (currentPCB) & (0x7F);       /* On force bit 8 a 0 */
	}
	else if(type == READER_T1_RBLOCK){
		newPCB = (currentPCB | 0x80) & 0xBF;  /* On force bits 8 et 7 a 10 */
	}
	else if(type == READER_T1_SBLOCK){
		newPCB = (currentPCB | 0x80) | 0x40;  /* On force bits 8 et 7 a 11 */
	}
	else{
		return READER_ERR;
	}
	
	retVal = READER_T1_SetBlockPCB(pBlock, newPCB);
	if(retVal != READER_OK) return retVal;
		
	/* On met a jour le checksum du Block */
	retVal = READER_T1_UpdateBlockChecksum(pBlock);
	if(retVal != READER_OK) return retVal;
	
	return READER_OK;
}


READER_Status READER_T1_SetBlockLEN(READER_T1_Block *pBlock, uint8_t blockLEN){
	READER_Status retVal;
	uint8_t *pCurrentLEN;
	uint8_t *blockFrame;

	
	if(blockLEN > READER_T1_BLOCK_MAX_DATA_SIZE){
		return READER_ERR;
	}

	blockFrame = READER_T1_GetBlockFrame(pBlock);

	pCurrentLEN = blockFrame + READER_T1_BLOCKFRAME_LEN_POSITION;
	*pCurrentLEN = blockLEN;
	
	/* On met a jour le checksum du Block */
	retVal = READER_T1_UpdateBlockChecksum(pBlock);
	if(retVal != READER_OK) return retVal;
	
	return READER_OK;
}


READER_Status READER_T1_SetBlockRedundancyType(READER_T1_Block *pBlock, READER_T1_RedundancyType type){
	if((type != READER_T1_CRC) && (type != READER_T1_LRC)){
		return READER_ERR;
	}
	
	pBlock->RedundancyType = type;

	///* On met a jour le checksum du Block */
	//retVal = READER_T1_UpdateBlockChecksum(pBlock);
	//if(retVal != READER_OK) return retVal;
	/* Ici, on  n'a pas besoin de recalculer le checksum, on a pas agit directement sur le Block ... */
	
	return READER_OK;
}


READER_Status READER_T1_SetBlockLRC(READER_T1_Block *pBlock, uint8_t blockLRC){
	READER_Status retVal;
	uint8_t currentLEN;	
	uint8_t *pCurrentLRC;
	uint8_t *blockFrame;
	
	/* On recupere un pointeur sur le block brut */
	blockFrame = READER_T1_GetBlockFrame(pBlock);
	
	/* On place le LRC au bon endroit            */
	currentLEN = READER_T1_GetBlockLEN(pBlock);
	pCurrentLRC = blockFrame + READER_T1_BLOCK_PROLOGUE_SIZE + currentLEN ;
	
	*pCurrentLRC = blockLRC;

	/* On mets a jour le RedundancyType a LRC    */
	retVal = READER_T1_SetBlockRedundancyType(pBlock, READER_T1_LRC);
	if(retVal != READER_OK) return retVal;
	
	///* On met a jour le checksum du Block */
	//retVal = READER_T1_UpdateBlockChecksum(pBlock);
	//if(retVal != READER_OK) return retVal;
	/* Pas besoin de mettre a jour le Checksum et surtout, le pb c'est que ca cree un appel recursif infini qui bloque tout ! */
	
	return READER_OK;
}


READER_Status READER_T1_SetBlockCRC(READER_T1_Block *pBlock, uint16_t blockCRC){
	READER_Status retVal;
	uint8_t currentLEN;	
	uint16_t *pCurrentCRC;
	uint8_t *blockFrame;
	
	
	/* On recupere un pointeur sur le block brut */
	blockFrame = READER_T1_GetBlockFrame(pBlock);
	
	/* On place le CRC au bon endroit            */
	currentLEN = READER_T1_GetBlockLEN(pBlock);
	pCurrentCRC = (uint16_t*)(blockFrame + READER_T1_BLOCKFRAME_LEN_POSITION + currentLEN);
	
	*pCurrentCRC = blockCRC;
	
	/* On mets a jour le RedundancyType a LRC    */
	retVal = READER_T1_SetBlockRedundancyType(pBlock, READER_T1_CRC);
	if(retVal != READER_OK) return retVal;
	
	///* On met a jour le checksum du Block */
	//retVal = READER_T1_UpdateBlockChecksum(pBlock);
	//if(retVal != READER_OK) return retVal;
	/* Pas besoin de mettre a jour le Checksum et surtout, le pb c'est que ca cree un appel recursif infini qui bloque tout ! */
	
	return READER_OK;
}


READER_Status READER_T1_SetBlockData(READER_T1_Block *pBlock, uint8_t *data, uint8_t dataSize){
	READER_Status retVal;
	//READER_T1_BlockType bType;
	READER_T1_RedundancyType rType;
	uint32_t i;
	uint8_t *pBlockData;
	uint8_t tmpLRC;
	uint16_t tmpCRC;
	
	
	/* Attention, ici, l'ordre des actions effectuees sur le Block est important !  */
	
	/* On recupere un pointeur sur la zone de donnees du Block ...  */
	pBlockData = READER_T1_GetBlockData(pBlock);
	
	/* On fait les verifications elementaires sur le Block ...  */
	if(dataSize > READER_T1_BLOCK_MAX_DATA_SIZE){
		return READER_ERR;
	}
	
	//bType = READER_T1_GetBlockType(pBlock);
	//if(bType != READER_T1_IBLOCK){
	//	return READER_ERR;
	//}
	
	/* On sauvegarde temporairement le champ LRC/CRC (il va etre ecrase quand on va ecrire les donnees) */
	rType = READER_T1_GetBlockRedundancyType(pBlock);
	if(rType == READER_T1_LRC){
		tmpLRC = READER_T1_GetBlockLRC(pBlock);
	}
	else if(rType == READER_T1_CRC){
		tmpCRC = READER_T1_GetBlockCRC(pBlock);
	}
	else{
		return READER_ERR;
	}
	
	
	/* On ecrit le champ de donnees dans le Block ...  */
	for(i=0; i<dataSize; i++){
		pBlockData[i] = data[i];
	}
	
	/* Mise a jour du LEN du Block */
	retVal = READER_T1_SetBlockLEN(pBlock, dataSize);
	if(retVal != READER_OK) return retVal;
	
	/* On peut s'en passer, de toute facon on mets a jour le checksum juste apres ...  */
	///* On re-ecrit le champ LRC/CRC a la fin du Block ...  */
	//if(rType == READER_T1_LRC){
	//	retVal = READER_T1_SetBlockLRC(pBlock, tmpLRC);
	//	if(retVal != READER_OK) return retVal;
	//}
	//else if(rType == READER_T1_CRC){
	//	retVal = READER_T1_SetBlockCRC(pBlock, tmpCRC);
	//	if(retVal != READER_OK) return retVal;
	//}
	//else{
	//	return READER_ERR;
	//}
	
	/* On met a jour le checksum du Block */
	retVal = READER_T1_UpdateBlockChecksum(pBlock);
	if(retVal != READER_OK) return retVal;
	
	return READER_OK;
}








uint8_t READER_T1_GetBlockSAD(READER_T1_Block *pBlock){
	uint8_t *pCurrentNAD;
	uint8_t *blockFrame;
	
	
	blockFrame = READER_T1_GetBlockFrame(pBlock);
	
	
	pCurrentNAD = blockFrame + READER_T1_BLOCKFRAME_NAD_POSITION;
	
	return (*pCurrentNAD) & 0x07;
}


uint8_t READER_T1_GetBlockDAD(READER_T1_Block *pBlock){
	uint8_t *pCurrentNAD;
	uint8_t *blockFrame;
	
	
	blockFrame = READER_T1_GetBlockFrame(pBlock);	
	
	pCurrentNAD = blockFrame + READER_T1_BLOCKFRAME_NAD_POSITION;
	
	return (*pCurrentNAD & 0xE0) >> 4;
}


uint8_t READER_T1_GetBlockNAD(READER_T1_Block *pBlock){
	uint8_t *pCurrentNAD;
	uint8_t *blockFrame;
	
	
	blockFrame = READER_T1_GetBlockFrame(pBlock);
	
	pCurrentNAD = blockFrame + READER_T1_BLOCKFRAME_NAD_POSITION;
	
	return *pCurrentNAD;
}


uint8_t READER_T1_GetBlockPCB(READER_T1_Block *pBlock){
	uint8_t *pCurrentPCB;
	uint8_t *blockFrame;
	
	
	blockFrame = READER_T1_GetBlockFrame(pBlock);	
	
	pCurrentPCB = blockFrame + READER_T1_BLOCKFRAME_PCB_POSITION;
	
	return *pCurrentPCB;
}


/**
 * \fn READER_T1_BlockType READER_T1_GetBlockType(READER_T1_Block *pBlock)
 * \brief Cette fonction permet de connaitre le type d'un Block tel (tel que décrit dans ISO7816-3 section 11.3.2.2). I-Block, R-Block ou S-Block.
 * \param *pBlock est un pointeur sur une structure de type READER_T1_BLOCK.
 * \return La fonction retourne une valeur de type READER_T1_BlockType selon le type de Block. Attention, cette valeur est erronée si la stucture Block n'a jamais été initailisée. (En principe un init est fait au moment de la construction du Block avec  READER_T1_ForgeBlock())
 */
READER_T1_BlockType READER_T1_GetBlockType(READER_T1_Block *pBlock){
	/* Voir ISO7816-3 section 11.3.2.2 */
	uint8_t currentPCB;
	
	
	currentPCB = READER_T1_GetBlockPCB(pBlock);
	
	if((currentPCB & 0x80) == 0x00){
		return READER_T1_IBLOCK;
	}
	else if((currentPCB & 0xC0) == 0x80){
		return READER_T1_RBLOCK;
	}
	else if((currentPCB & 0xC0) == 0xC0){
		return READER_T1_SBLOCK;
	}
	else{
		return READER_T1_BLOCK_ERR;
	}
}



/**
 * \fn uint8_t READER_T1_GetBlockLEN(READER_T1_Block *pBlock)
 * \brief Cette fonction permet de récupérer le champ LEN d'un Block. Le champs LEN est décrit dans ISO7816-3 section 11.3.1.
 * Ici, on ne calcule pas cette valeur, il s'agit d'un accesseur sur cette valeur dans la stucture Block.
 * \param *pBlock est un pointeur sur une stucture de type READER_T1_BLOCK.
 * \return La fonction retourne un entier uint8_t. La valeur de cet entier est le champ LEN tel que décrit ISO7816-3 section 11.3.2.3.
 * Attention, si la strucure Block n'a jamais été initialisée, cette valeur est erronée. (En principe un init est fait au moment de la construction du Block avec  READER_T1_ForgeBlock())
 */
uint8_t READER_T1_GetBlockLEN(READER_T1_Block *pBlock){
	uint8_t *pCurrentLEN;
	uint8_t *blockFrame;
	
	
	/* Ici on ne calcule pas une taille. On va lire dans la sructure du Block le champ LEN */
	
	blockFrame = READER_T1_GetBlockFrame(pBlock);
	
	pCurrentLEN = blockFrame + READER_T1_BLOCKFRAME_LEN_POSITION;
	
	return *pCurrentLEN;
}


READER_T1_RedundancyType READER_T1_GetBlockRedundancyType(READER_T1_Block *pBlock){
	return pBlock->RedundancyType;
}


uint8_t READER_T1_GetBlockLRC(READER_T1_Block *pBlock){
	uint8_t currentLEN;	
	uint8_t *pCurrentLRC;
	uint8_t *blockFrame;
	
	
	blockFrame = READER_T1_GetBlockFrame(pBlock);
	
	currentLEN = READER_T1_GetBlockLEN(pBlock);
	pCurrentLRC = blockFrame + READER_T1_BLOCK_PROLOGUE_SIZE + currentLEN;
	
	return *pCurrentLRC;
}


uint16_t READER_T1_GetBlockCRC(READER_T1_Block *pBlock){
	uint8_t currentLEN;	
	uint16_t *pCurrentCRC;
	uint8_t *blockFrame;
	
	
	blockFrame = READER_T1_GetBlockFrame(pBlock);
	
	currentLEN = READER_T1_GetBlockLEN(pBlock);
	pCurrentCRC = (uint16_t*)(blockFrame + READER_T1_BLOCKFRAME_LEN_POSITION + currentLEN);
	
	return *pCurrentCRC;
}


/* retourne un pointeur sur les donnes du Block */
uint8_t* READER_T1_GetBlockData(READER_T1_Block *pBlock){
	uint8_t *blockFrame;
	
	blockFrame = READER_T1_GetBlockFrame(pBlock);
	
	return blockFrame + READER_T1_BLOCKFRAME_INF_POSITION;
}


uint8_t READER_T1_ComputeBlockLRC(READER_T1_Block *pBlock){
	uint8_t xorSum;
	uint8_t *pBlockFrame;
	uint8_t dataSize;
	uint32_t blockFrameSize;
	uint32_t i;
	
	
	dataSize = READER_T1_GetBlockLEN(pBlock);
	blockFrameSize = (uint32_t)dataSize + (uint32_t)READER_T1_BLOCK_PROLOGUE_SIZE;   /* > 3 */
	
	pBlockFrame = READER_T1_GetBlockFrame(pBlock);
	
	xorSum = pBlockFrame[0];
	for(i=1; i<blockFrameSize; i++){
		xorSum = xorSum ^ (pBlockFrame[i]);
	}
	//if(xorSum == 0xEA){
	//	READER_PERIPH_ErrHandler();
	//}
	return xorSum;
}


uint16_t READER_T1_ComputeBlockCRC(READER_T1_Block *pBlock){	
	uint8_t *blockFrame;
	uint16_t crc = 0x0000;
	uint32_t len;
	uint8_t currentByte;
	uint32_t i, j;
	
	
	blockFrame = READER_T1_GetBlockFrame(pBlock);
	
	
	len = READER_T1_GetBlockSizeWithoutCheck(pBlock);
	
	for(i=0; i<len; i++){
		currentByte = blockFrame[i];
		
		crc ^= currentByte;
		
		for(j=0; j<8; j++){
			crc ^= READER_T1_CRC_POLY;
			crc >>= 1;
		}
	}
	
	return crc;
}

/**
 * \fn uint32_t READER_T1_GetBlockTotalSize(READER_T1_Block *pBlock)
 * \brief Cette fonction calcule le nombre d'octets necessaires pour représeter le block tel que décrit dans ISO7816-3 section 11.3.1.
 * Il s'agt ici de la taille totale, c'est a dire :  len(Prologue field) + len(Information field) + len(Epilogue field)
 * \param *pBlock est un pointeur sur une stucture Block : READER_T1_BLOCK.
 * \return La fonction retourne un entier uint32_t. La valeur de cet entier est le nombre d'octets necessaires pour représenter le Block.
 */
uint32_t READER_T1_GetBlockTotalSize(READER_T1_Block *pBlock){
	READER_T1_RedundancyType checkType;
	uint32_t checkLen;
	uint32_t tmpLen;
	
	
	tmpLen = READER_T1_GetBlockSizeWithoutCheck(pBlock);
	
	checkType = READER_T1_GetBlockRedundancyType(pBlock);
	
	if(checkType == READER_T1_CRC){
		checkLen = 2;
	}
	else{
		checkLen = 1;
	}
	
	return tmpLen + checkLen;
}


/**
 * \fn READER_T1_GetBlockSizeWithoutCheck(READER_T1_Block *pBlock)
 * \brief Cette fonction calcule le nombre d'octets necessaires pour représeter le block privé de son Epilogue field, tel que décrit dans ISO7816-3 section 11.3.1.
 * la fonction calcule : len(Prologue field) + len(Information field).
 * \param *pBlock est un pointeur sur une stucture Block : READER_T1_BLOCK.
 * \return La fonction retourne un entier uint32_t. La valeur de cet entier est le nombre d'octets necessaires pour représenter le Block (sans compte le Epilogue field).
 */
uint32_t READER_T1_GetBlockSizeWithoutCheck(READER_T1_Block *pBlock){
	uint8_t dataLEN;
	
	
	dataLEN = READER_T1_GetBlockLEN(pBlock);
	
	return READER_T1_BLOCK_PROLOGUE_SIZE + (uint32_t)dataLEN;
}



uint8_t* READER_T1_GetBlockFrame(READER_T1_Block *pBlock){
	return pBlock->blockFrame;
}


uint32_t READER_T1_GetBlockRedundancyLen(READER_T1_Block *pBlock){
	READER_T1_RedundancyType rType;
	
	rType = READER_T1_GetBlockRedundancyType(pBlock);
	
	if(rType == READER_T1_LRC){
		return 1;
	}
	else{
		return 2;
	}
}


READER_Status READER_T1_UpdateBlockChecksum(READER_T1_Block *pBlock){
	READER_Status retVal;
	READER_T1_RedundancyType rType;
	uint16_t blockCRC;
	uint8_t blockLRC;
	
	
	rType = READER_T1_GetBlockRedundancyType(pBlock);
	
	if(rType == READER_T1_LRC){
		blockLRC = READER_T1_ComputeBlockLRC(pBlock);
		retVal = READER_T1_SetBlockLRC(pBlock, blockLRC);
		if(retVal != READER_OK) return retVal;
	}
	else if(rType == READER_T1_CRC){
		blockCRC = READER_T1_ComputeBlockCRC(pBlock);
		retVal = READER_T1_SetBlockCRC(pBlock, blockCRC);
		if(retVal != READER_OK) return retVal;
	}
	else{
		return READER_ERR;
	}
	
	return READER_OK;
}



/**
 * \fn READER_Status READER_T1_ForgeBlock(READER_T1_Block *pBlock, READER_T1_RedundancyType rType)
 * \brief Cette fonction permet de construire un Block vierge. La fonction prends un pointeur sur un structure Block qui n'est pas initialisée, la fonction initialise ce Block de sorte à ce qu'il puisse être utilisé par les autres fonctions.
 * \param *pBlock est un pointeur sur une structure de type READER_T1_Block. Il s'agit d'un pointeur sur un espace mémoire déjà alloué et capable de contenir un Block. Ce Block n'est pas initialisé.
 * \param rType est uen valeur de type READER_T1_RedundancyType. Elle indique le type de code correcteur qui va etre utilisé pour ce Block.
 * \return La fonction retourne une valeur de type READER_Status. READER_OK indiue le bon déroulement de la fonction.
 */
READER_Status READER_T1_ForgeBlock(READER_T1_Block *pBlock, READER_T1_RedundancyType rType){
	READER_Status retVal;
	
	retVal = READER_T1_SetBlockRedundancyType(pBlock, rType);
	if(retVal != READER_OK) return retVal;
	
	/* On initialise la taille des data a zero */
	retVal = READER_T1_SetBlockLEN(pBlock, READER_T1_BLOCK_INITIAL_LEN);
	if(retVal != READER_OK) return retVal;
	
	/* On initialise le PCB à zero ...  */
	retVal = READER_T1_SetBlockPCB(pBlock, READER_T1_BLOCK_INITIAL_PCB);
	if(retVal != READER_OK) return retVal;
	
	/* A priori on utilise pas l'adresse, donc NAD et SAD sont a 000. Voir ISO7815-3 section 11.3.2.1 */
	retVal = READER_T1_SetBlockNAD(pBlock, READER_T1_BLOCK_INITIAL_NAD);
	if(retVal != READER_OK) return retVal;
	
	
	return READER_OK;
}


/**
 * \fn READER_Status READER_T1_SendBlock(READER_T1_Block *pBlock, uint32_t currentCWT, uint32_t extraStartDelay, uint32_t *pTickstart)
 * \brief Cette fonction permet de placer une structure de type READER_T1_Block sur le support support de transmission.
 * Cette fonction permet l'envoi des octets et assure le respect des délais (WT, GT, BWT, BGT ...). Elle n'a pas conscience du contexte de communication.
 * Elle traduit le niveau logique Block en uen chaine de caractères à envoyer.
 * Elle permet également de remonter de l'information sur l'envoi du Block.
 * \param *pBlock est un pointeur sur une structure de type READER_T1_Block. Il s'agit du Block à envoyer.
 * \param currentCWT est un uint32_t qui indique la valeur de CWT (voir ISO7816-3 section 11.4.3) à appliquer pour l'envoi de ce Block. C'est une valeur en milisecondes.
 * \param extraStartDelay est un uint32_t permet d'indiquer un délai supplémentaire à appliquer avant de déclencher l'envoi du premier caractère du Block. C'est une valeur en milisecondes. Permet d'assurer le respect du BGT par exemple (voir ISO7816-3 section 11.4.3).
 * \param *pTickStart est un pointeur sur un uint32_t. Il permet  la fonction de retourner la date (en milisecondes) du début de l'envoi du dernier caractère du Block. (leading edge du dernier caractère du block, voir ISO7816-3 section 11.4.3).
 * \return La fonction retourne un code d'erreur de type READER_Status. READER_Ok indique le bon déroulement.
 */
READER_Status READER_T1_SendBlock(READER_T1_Block *pBlock, uint32_t currentCWT, uint32_t extraStartDelay, uint32_t *pTickstart, READER_HAL_CommSettings *pSettings){
	READER_Status retVal;
	uint8_t *blockFrame;
	uint32_t blockFrameSize;
	uint32_t tickstart;
	
	
	/* On recupere les donnees a envoyer sous format brut ...  */
	blockFrame = READER_T1_GetBlockFrame(pBlock);
	blockFrameSize = READER_T1_GetBlockTotalSize(pBlock);
	
	/* On patiente extraDelay milisecondes (arrondi a la milisec inf, pour eviter de depasser) ...  */
	tickstart = READER_HAL_GetTick();
	while((READER_HAL_GetTick() - tickstart) < extraStartDelay){
		
	}
	
	/* On envoie toutes les donnees brutes. Le temps maximal dont ont dispose pour envoyer un caractere est currentCWT ... */
	retVal = READER_HAL_SendCharFrameTickstart(pSettings, READER_HAL_PROTOCOL_T1, blockFrame, blockFrameSize, currentCWT, &tickstart);
	if(retVal != READER_OK) return retVal;
	
	*pTickstart = tickstart;
	
	return READER_OK;
}


/* A CORRIGER, DESCRIPTION N'EST PAS A JOUR !!!  ??  */
/**
 * \fn READER_Status READER_T1_RcvBlock(READER_T1_Block *pBlock, uint32_t timeout)
 * \brief Cette fonction permet de recevoir un Block. La fonction recoit d'abord le prologue du Block, puis à partir des informations qui y sont contenues, elle recoit le reste du Block. Attention cette fonction ne vérifie pas l'intégrité du Block, pas de vérification du checksum.
 * \return La fonction revoie un code de type READER_Status. Le retour est READER_OK si le Block esr reçu en entier et si le formatage du Block reçu est correct.
 * \param *pBlock Pointeur sur une structure de type READER_T1_Block. Le contenu du Block reçu sera placé à l'intérieur.
 * \param timeout Il s'agit de la valeur du timeout pour chaque caractère en milisecondes.
 */
READER_Status READER_T1_RcvBlock(READER_T1_Block *pBlock, READER_T1_RedundancyType rType, uint32_t currentCWT, uint32_t extraTimeout, uint32_t *pTickstart, READER_HAL_CommSettings *pSettings){
	READER_Status retVal;
	uint8_t buffPrologue[READER_T1_BLOCK_PROLOGUE_SIZE];
	uint8_t buff[READER_T1_BLOCK_MAX_DATA_SIZE + 2];  /* MAXDATA + MAX CRC */
	uint32_t buffSize;
	uint8_t blockLRC;
	uint16_t blockCRC;
	uint32_t count;
	uint32_t tickstart;
	
	
	/* Quoi qu'il arrive on veut recuperer les trois premiers caracteres du Block (Prologue).   */
	/* Ensuite, en fonction des valeurs recues, on decide ...                                   */
	
	/* On recoit le premier caractere separepment (on prend en compte le WT entre deux Blocks)  */
	/* En l'occurence on ajoute le WT du Block au WT du premier caractere                       */
	/* Il s'agit du premier caractere du Prologue                                               */
	/* Le extraTimeout est calcule a plus haut niveau ...                                       */
	retVal = READER_HAL_RcvCharFrameCount(pSettings, READER_HAL_PROTOCOL_T1, buffPrologue, 1, &count, currentCWT + extraTimeout);
	if(retVal != READER_OK) return retVal;
	
	if(count != 1){
		return READER_ERR;
	}
	
	/* Ensuite on recupere les deux suivants du prologue ...                                    */

	retVal = READER_HAL_RcvCharFrameCount(pSettings, READER_HAL_PROTOCOL_T1, buffPrologue +1, READER_T1_BLOCK_PROLOGUE_SIZE-1, &count, currentCWT);
	if(retVal != READER_OK) return retVal;
	
	if(count != READER_T1_BLOCK_PROLOGUE_SIZE -1){
		return READER_ERR;
	}
	
	
	/* Selon le prologue recu ... */
	//rType = READER_T1_GetBlockRedundancyType(pBlock);
	buffSize = buffPrologue[READER_T1_BLOCKFRAME_LEN_POSITION];
	
	if(buffSize > READER_T1_BLOCK_MAX_DATA_SIZE) return READER_ERR;
	
	/* On regarde combien de place il faut prevoir pour recevoir le code correcteur ...  */
	if(rType == READER_T1_LRC){
		buffSize += 1;
	}
	else if(rType == READER_T1_CRC){
		buffSize += 2;
	}
	else{
		return READER_ERR;
	}
	
	/* On recoit les data et CRC/LRC d'un seul coups */
	retVal = READER_HAL_RcvCharFrameCountTickstart(pSettings, READER_HAL_PROTOCOL_T1, buff, buffSize, &count, currentCWT, &tickstart);
	//if(count == 6){
	//	READER_PERIPH_ErrHandler();
	//}
	if(retVal != READER_OK) return retVal;
	if(count != buffSize) return READER_ERR;
	
	/* On mets a jour la date du leading edge du dernier caractere du Block ...  */
	*pTickstart = tickstart;
	
	
	/* On forge un block vide                                           */
	retVal = READER_T1_ForgeBlock(pBlock, rType);
	if(retVal != READER_OK) return retVal;
	
	/* On commence a fabriquer le block a partir des donnees recues ... */
	retVal = READER_T1_SetBlockNAD(pBlock, buffPrologue[READER_T1_BLOCKFRAME_NAD_POSITION]);
	if(retVal != READER_OK) return retVal;
	
	retVal = READER_T1_SetBlockPCB(pBlock, buffPrologue[READER_T1_BLOCKFRAME_PCB_POSITION]);
	if(retVal != READER_OK) return retVal;
	
	retVal = READER_T1_SetBlockLEN(pBlock, buffPrologue[READER_T1_BLOCKFRAME_LEN_POSITION]);
	if(retVal != READER_OK) return retVal;
	
	
	
	/* Recuperation du code correcteur d'erreur  */
	if(rType == READER_T1_LRC){
		retVal = READER_T1_SetBlockData(pBlock, buff, buffSize -1); 
		if(retVal != READER_OK) return retVal;
	
		blockLRC = buff[buffSize-1];
		retVal = READER_T1_SetBlockLRC(pBlock, blockLRC);
		if(retVal != READER_OK) return retVal;
	}
	else if(rType == READER_T1_CRC){
		retVal = READER_T1_SetBlockData(pBlock, buff, buffSize -2); 
		if(retVal != READER_OK) return retVal;
	
		blockCRC = *(uint16_t*)(buff + buffSize - 2);
		retVal = READER_T1_SetBlockCRC(pBlock, blockCRC);
		if(retVal != READER_OK) return retVal;
	}
	else{
		return READER_ERR;
	}
	
	/* A la fin parceque potentiellment traitement long */
	/* On complete le nv block forge avec les data      */
	
	//retVal = READER_T1_SetBlockData(pBlock, buff, buffSize); 
	//if(retVal != READER_OK) return retVal;
	
	return READER_OK;
}


READER_Status READER_T1_CheckBlockIntegrity(READER_T1_Block *pBlock, READER_T1_RedundancyType rType){
	uint8_t blockLRC, expectedBlockLRC;
	uint16_t blockCRC, expectedBlockCRC;
	
	
	if(rType == READER_T1_LRC){
		expectedBlockLRC = READER_T1_GetBlockLRC(pBlock);
		blockLRC = READER_T1_ComputeBlockLRC(pBlock);
		
		if(expectedBlockLRC != blockLRC) return READER_INTEGRITY;
	}
	else if(rType == READER_T1_CRC){
		expectedBlockCRC = READER_T1_GetBlockCRC(pBlock);
		blockCRC = READER_T1_ComputeBlockCRC(pBlock);
		
		if(expectedBlockCRC != blockCRC) return READER_INTEGRITY;
	}
	else{
		return READER_ERR;
	}
	
	return READER_OK;
}


READER_Status READER_T1_CopyBlock(READER_T1_Block *pBlockDest, READER_T1_Block *pBlockSource){
	uint32_t i;

	/* Ici, on peut encore optimiser la fonction en ne copiant que les LEN donnees */
	for(i=0; i<READER_T1_BLOCK_MAX_DATA_SIZE+6; i++){
		pBlockDest->blockFrame[i] = pBlockSource->blockFrame[i];
	}
	
	pBlockDest->RedundancyType = pBlockSource->RedundancyType;
	
	return READER_OK;
}



//uint32_t READER_T1_GetBlockMBit(READER_T1_Block *pBlock){
//	uint8_t blockPCB;
//	uint32_t mBit;
//	
//	blockPCB = READER_T1_GetBlockPCB(pBlock);
//	mBit = (uint32_t)((blockPCB & 0x20)>>5);            /* Voir ISO7816-3 section 11.3.2.2 */
//	
//	return mBit;
//}


//READER_Status READER_T1_SetBlockMBit(READER_T1_Block *pBlock, uint32_t mBit){
//	READER_Status retVal;
//	uint8_t blockPCB, newBlockPCB;
//	
//	
//	blockPCB = READER_T1_GetBlockPCB(pBlock);
//	
//	if(mBit == 0){
//		newBlockPCB = blockPCB & 0xDF;             /* On passe le 6ieme bit a 0. Voir ISO7816-3 section 11.3.2.2 */
//	}
//	else if(mBit == 1){
//		newBlockPCB = blockPCB | 0x20;             /* On passe le 6ieme bit a 1. Voir ISO7816-3 section 11.3.2.2 */
//	}
//	else{
//		return READER_ERR;
//	}
//	
//	retVal = READER_T1_SetBlockPCB(pBlock, newBlockPCB);
//	if(retVal != READER_OK) return retVal;
//	
//	return READER_OK;
//}


READER_Status READER_T1_CopyBlockData(READER_T1_Block *pBlock, uint8_t *destBuffer, uint32_t destBufferSize){
	uint8_t *pBlockData;
	uint8_t blockLEN;
	uint32_t i;
	
	
	pBlockData = READER_T1_GetBlockData(pBlock);
	blockLEN = READER_T1_GetBlockLEN(pBlock);
	
	if(blockLEN > destBufferSize){
		return READER_OVERFLOW;
	}
	
	for(i=0; i<blockLEN; i++){
		destBuffer[i] = pBlockData[i];
	}
	
	return READER_OK;
}
