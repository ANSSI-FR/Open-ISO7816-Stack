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
	uint8_t *pCurrentPCB;
	uint8_t *blockFrame;
	
	
	blockFrame = READER_T1_GetBlockFrame(pBlock);	

	pCurrentPCB = blockFrame + READER_T1_BLOCKFRAME_PCB_POSITION;
	
	
	if(type == READER_T1_IBLOCK){
		*pCurrentPCB = (*pCurrentPCB) & (0x7F);       /* On force bit 8 a 0 */
	}
	else if(type == READER_T1_RBLOCK){
		*pCurrentPCB = (*pCurrentPCB | 0x80) & 0xBF;  /* On force bits 8 et 7 a 10 */
	}
	else if(type == READER_T1_SBLOCK){
		*pCurrentPCB = (*pCurrentPCB | 0x80) | 0x40;  /* On force bits 8 et 7 a 11 */
	}
	else{
		return READER_ERR;
	}
	
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
	READER_Status retVal;
	if((type != READER_T1_CRC) && (type != READER_T1_LRC)){
		return READER_ERR;
	}
	
	pBlock->RedundancyType = type;
	
	/* On met a jour le checksum du Block */
	retVal = READER_T1_UpdateBlockChecksum(pBlock);
	if(retVal != READER_OK) return retVal;
	
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
	pCurrentLRC = blockFrame + READER_T1_BLOCKFRAME_LEN_POSITION + currentLEN;
	
	*pCurrentLRC = blockLRC;
	
	/* On mets a jour le RedundancyType a LRC    */
	retVal = READER_T1_SetBlockRedundancyType(pBlock, READER_T1_LRC);
	if(retVal != READER_OK) return retVal;
	
	/* On met a jour le checksum du Block */
	retVal = READER_T1_UpdateBlockChecksum(pBlock);
	if(retVal != READER_OK) return retVal;
	
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
	
	/* On met a jour le checksum du Block */
	retVal = READER_T1_UpdateBlockChecksum(pBlock);
	if(retVal != READER_OK) return retVal;
	
	return READER_OK;
}


READER_Status READER_T1_SetBlockData(READER_T1_Block *pBlock, uint8_t *data, uint8_t dataSize){
	READER_Status retVal;
	uint32_t i;
	uint8_t *pBlockData;
	uint8_t *blockFrame;
	
	
	blockFrame = READER_T1_GetBlockFrame(pBlock);
	
	if(dataSize > READER_T1_BLOCK_MAX_DATA_SIZE){
		return READER_ERR;
	}
	
	/* Mise a jour du LEN du Block */
	retVal = READER_T1_SetBlockLEN(pBlock, dataSize);
	if(retVal != READER_OK) return retVal;
	
	/* Recuperation du pointeur sur le champ de donnees du block et insertion des data */
	pBlockData = blockFrame + READER_T1_BLOCKFRAME_INF_POSITION;
	
	for(i=0; i<dataSize; i++){
		pBlockData[i] = data[i];
	}
	
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


uint8_t READER_T1_GetBlockLEN(READER_T1_Block *pBlock){
	uint8_t *pCurrentLEN;
	uint8_t *blockFrame;
	
	
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
	pCurrentLRC = blockFrame + READER_T1_BLOCKFRAME_LEN_POSITION + currentLEN;
	
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
	uint8_t *data;
	uint8_t dataSize;
	uint32_t i;
	
	
	dataSize = READER_T1_GetBlockLEN(pBlock);
	data = READER_T1_GetBlockData(pBlock);
	
	if(dataSize == 0){
		return 0x00;
	}
	
	if(dataSize == 1){
		return data[0];
	}
	
	xorSum = data[0];
	for(i=1; i<dataSize; i++){
		xorSum = xorSum ^ data[i];
	}
	
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


uint32_t READER_T1_GetBlockSizeWithoutCheck(READER_T1_Block *pBlock){
	uint8_t dataLEN;
	
	
	dataLEN = READER_T1_GetBlockLEN(pBlock);
	
	return READER_T1_BLOCKFRAME_INF_POSITION + (uint32_t)dataLEN;
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



READER_Status READER_T1_ForgeBlock(READER_T1_Block *pBlock, READER_T1_RedundancyType rType){
	READER_Status retVal;
	
	retVal = READER_T1_SetBlockRedundancyType(pBlock, rType);
	if(retVal != READER_OK) return retVal;
	
	/* On initialise la taille des data a zero */
	retVal = READER_T1_SetBlockLEN(pBlock, 0x00);
	if(retVal != READER_OK) return retVal;
	
	/* A priori on utilise pas l'adresse, donc NAD et SAD sont a 000. Voir ISO7815-3 section 11.3.2.1 */
	retVal = READER_T1_SetBlockNAD(pBlock, 0x00);
	if(retVal != READER_OK) return retVal;
	
	
	return READER_OK;
}


READER_Status READER_T1_SendBlock(READER_T1_Block *pBlock, uint32_t timeout){
	READER_Status retVal;
	uint8_t *blockFrame;
	uint32_t blockFrameSize;
	
	blockFrame = READER_T1_GetBlockFrame(pBlock);
	blockFrameSize = READER_T1_GetBlockTotalSize(pBlock);
	
	retVal = READER_HAL_SendCharFrame(blockFrame, blockFrameSize, timeout);
	if(retVal != READER_OK) return retVal;
	
	return READER_OK;
}


/**
 * \fn READER_Status READER_T1_RcvBlock(READER_T1_Block *pBlock, uint32_t timeout)
 * \brief Cette fonction permet de recevoir un Block. La fonction recoit d'abord le prologue du Block, puis à partir des informations qui y sont contenues, elle recoit le reste du Block. Attention cette fonction ne vérifie pas l'intégrité du Block, pas de vérification du checksum.
 * \return La fonction revoie un code de type READER_Status. Le retour est READER_OK si le Block esr reçu en entier et si le formatage du Block reçu est correct.
 * \param *pBlock Pointeur sur une structure de type READER_T1_Block. Le contenu du Block reçu sera placé à l'intérieur.
 * \param timeout Il s'agit de la valeur du timeout pour chaque caractère en milisecondes.
 */
READER_Status READER_T1_RcvBlock(READER_T1_Block *pBlock, uint32_t timeout){
	READER_Status retVal;
	READER_T1_RedundancyType rType;
	uint8_t buffPrologue[READER_T1_BLOCK_PROLOGUE_SIZE];
	uint8_t buff[READER_T1_BLOCK_MAX_DATA_SIZE + 2];  /* MAXDATA + MAX CRC */
	uint32_t buffSize;
	uint8_t blockLRC;
	uint16_t blockCRC;
	uint32_t count;
	
	
	/* On recoit d'abord les trois premiers caracteres (Prologue du block) quoiqu'il arrive. Ensuite on decide ... */
	retVal = READER_HAL_RcvCharFrameCount(buffPrologue, READER_T1_BLOCK_PROLOGUE_SIZE, &count, timeout);
	if(retVal != READER_OK) return retVal;
	
	if(count != READER_T1_BLOCK_PROLOGUE_SIZE){
		return READER_ERR;
	}
	
	
	/* On forge un block vide                                           */
	rType = READER_HAL_GetRedunancyType();
	retVal = READER_T1_ForgeBlock(pBlock, rType);
	if(retVal != READER_OK) return retVal;
	
	
	/* On commence a fabriquer le block a partir des donnees recues ... */
	retVal = READER_T1_SetBlockNAD(pBlock, buffPrologue[READER_T1_BLOCKFRAME_NAD_POSITION]);
	if(retVal != READER_OK) return retVal;
	
	retVal = READER_T1_SetBlockPCB(pBlock, buffPrologue[READER_T1_BLOCKFRAME_PCB_POSITION]);
	if(retVal != READER_OK) return retVal;
	
	retVal = READER_T1_SetBlockLEN(pBlock, buffPrologue[READER_T1_BLOCKFRAME_LEN_POSITION]);
	if(retVal != READER_OK) return retVal;
	
	
	/* Selon le prologue recu ... */
	rType = READER_T1_GetBlockRedundancyType(pBlock);
	buffSize = READER_T1_GetBlockLEN(pBlock);
	
	if(buffSize > READER_T1_BLOCK_MAX_DATA_SIZE) return READER_ERR;
	
	buffSize += READER_T1_GetBlockRedundancyLen(pBlock);   /* On prevoit de recevoir les Data et LRC/CRC */
	
	/* On recoit les data et CRC/LRC d'un seul coups */
	retVal = READER_HAL_RcvCharFrameCount(buff, buffSize, &count, timeout);
	if(retVal != READER_OK) return retVal;
	if(count != buffSize) return READER_ERR;
	
	/* Recuperation du code correcteur d'erreur  */
	if(rType == READER_T1_LRC){
		blockLRC = buff[buffSize-1];
		retVal = READER_T1_SetBlockLRC(pBlock, blockLRC);
		if(retVal != READER_OK) return retVal;
	}
	else if(rType == READER_T1_CRC){
		blockCRC = *(uint16_t*)(buff + buffSize - 2);
		retVal = READER_T1_SetBlockCRC(pBlock, blockCRC);
		if(retVal != READER_OK) return retVal;
	}
	else{
		return READER_ERR;
	}
	
	/* A la fin parceque potentiellment traitement long */
	/* On complete le nv block forge avec les data      */
	retVal = READER_T1_SetBlockData(pBlock, buff, buffSize); 
	if(retVal != READER_OK) return retVal;
	
	
	return READER_OK;
}

/**
 * \fn READER_Status READER_T1_CheckBlockIntegrity(READER_T1_Block *pBlock)
 * \brief Cette fonction permet de vérifier qu'un Block est intègre (ie : son checksum est correct). La fonction récupère le type de checksum du Block (LRC ou CRC), calcule le checksum du Prologue field et du Data field, puis le compare au Epilogue field transporté par le Block.
 * \return La fonction retourne un code de type READER_Status. La valeur est READER_OK si la cérification donne un résultat correct. Tout autre valeur indique une erreur.
 * \param *pBlock est un pointeur sur une structure de type READER_T1_Block. Il pointe sur le Block à vérifier.
 */
READER_Status READER_T1_CheckBlockIntegrity(READER_T1_Block *pBlock){
	READER_T1_RedundancyType rType;
	uint8_t blockLRC, expectedBlockLRC;
	uint16_t blockCRC, expectedBlockCRC;
	
	
	rType = READER_T1_GetBlockRedundancyType(pBlock);
	
	
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



uint32_t READER_T1_GetBlockMBit(READER_T1_Block *pBlock){
	uint8_t blockPCB;
	uint32_t mBit;
	
	blockPCB = READER_T1_GetBlockPCB(pBlock);
	mBit = (uint32_t)((blockPCB & 0x20)>>5);            /* Voir ISO7816-3 section 11.3.2.2 */
	
	return mBit;
}


READER_Status READER_T1_SetBlockMBit(READER_T1_Block *pBlock, uint32_t mBit){
	READER_Status retVal;
	uint8_t blockPCB, newBlockPCB;
	
	
	blockPCB = READER_T1_GetBlockPCB(pBlock);
	
	if(mBit == 0){
		newBlockPCB = blockPCB & 0xDF;             /* On passe le 6ieme bit a 0. Voir ISO7816-3 section 11.3.2.2 */
	}
	else if(mBit == 1){
		newBlockPCB = blockPCB | 0x20;             /* On passe le 6ieme bit a 1. Voir ISO7816-3 section 11.3.2.2 */
	}
	else{
		return READER_ERR;
	}
	
	retVal = READER_T1_SetBlockPCB(pBlock, newBlockPCB);
	if(retVal != READER_OK) return retVal;
	
	return READER_OK;
}
