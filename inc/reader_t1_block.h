/**
* \file reader_t1_block.h
* \copyright This file is part of the Open-ISO7816-Stack project and is distributed under the MIT license. See LICENSE file in the root directory. 
*/

#ifndef __READER_T1_BLOCK_H__
#define __READER_T1_BLOCK_H__



#include "reader.h"
#include "reader_hal_comm_settings.h"
#include "reader_hal.h"




#define READER_T1_BLOCK_MAX_DATA_SIZE          (uint32_t)(254) 
#define READER_T1_BLOCK_PROLOGUE_SIZE          (uint32_t)(3)
#define READER_T1_BLOCK_EPILOGUE_MAXSIZE       (uint32_t)(2)
#define READER_T1_BLOCKFRAME_NAD_POSITION      (uint32_t)(0)
#define READER_T1_BLOCKFRAME_PCB_POSITION      (uint32_t)(1)
#define READER_T1_BLOCKFRAME_LEN_POSITION      (uint32_t)(2)
#define READER_T1_BLOCKFRAME_INF_POSITION      (uint32_t)(3)
#define READER_T1_CRC_POLY                     (uint32_t)(0x91)
#define READER_T1_BLOCK_MAX_TOTAL_LENGTH       (uint32_t)(READER_T1_BLOCK_MAX_DATA_SIZE + READER_T1_BLOCK_EPILOGUE_MAXSIZE + READER_T1_BLOCK_PROLOGUE_SIZE)



#define READER_T1_BLOCK_INITIAL_NAD            (uint8_t)(0x00)
#define READER_T1_BLOCK_INITIAL_PCB            (uint8_t)(0x00)
#define READER_T1_BLOCK_INITIAL_LEN            (uint8_t)(0x00)


/**
* \def READER_T1_BLOCK_MAX_DATA_SIZE
* READER_T1_BLOCK_MAX_DATA_SIZE is the maximum number of data bytes that are accepted by the reader into one Block. 
* This value defines the number of bytes that are statically allocated in the stack into the READER_T1_Block data structure.
* The developper can lower this value in order to reduce the memory footprint. 
* Warning : This length have to be greater than the current IFSC (defined in READER_T1_ContextHandler).
* The maximum data length is defined to be 254 in ISO7816-3 section 11.3.2.3. 
*/

/**
* \def READER_T1_BLOCK_PROLOGUE_SIZE
* READER_T1_BLOCK_PROLOGUE_SIZE is the size of the Block prologue as defined in ISO7816-3 section 11.3.1. 
*/
 
 
 /**
 * \def READER_T1_BLOCKFRAME_NAD_POSITION
 * READER_T1_BLOCKFRAME_NAD_POSITION is the index of NAD character into the blockFrame defined in READER_T1_Block.
 */
 
  /**
 * \def READER_T1_BLOCKFRAME_PCB_POSITION
 * READER_T1_BLOCKFRAME_PCB_POSITION is the index of PCB character into the blockFrame defined in READER_T1_Block.
 */
 
  /**
 * \def READER_T1_BLOCKFRAME_LEN_POSITION
 * READER_T1_BLOCKFRAME_LEN_POSITION is the index of LEN character into the blockFrame defined in READER_T1_Block.
 */
 
  /**
 * \def READER_T1_BLOCKFRAME_INF_POSITION
 * READER_T1_BLOCKFRAME_INF_POSITION is the index of the beginning of the data field into the blockFrame defined in READER_T1_Block.
 */


/**
 * \def READER_T1_BLOCK_MAX_TOTAL_LENGTH
 * READER_T1_BLOCK_MAX_TOTAL_LENGTH is the maximum length of the blockFrame defined into READER_T1_Block.
 * This maximum length takes in account READER_T1_BLOCK_MAX_DATA_SIZE (the maximul size of the data field) and all the meta data (NAD, PCB, LEN, CRC/LRC ... ).
 */


/**
 * \enum READER_T1_RedundancyType
 * This type is used to encode the type of correction code that is currently in use.
 */
typedef enum READER_T1_RedundancyType READER_T1_RedundancyType;
enum READER_T1_RedundancyType{
	READER_T1_CRC                      = (uint32_t)(0x00000000),   /*!< Longitudial correction code. */
	READER_T1_LRC                      = (uint32_t)(0x00000001)    /*!< Cyclic redundancy code.      */  
};




/**
 * \enum READER_T1_BlockType
 * This type is used to encode the type of a Block as described in ISO7816-3 at section 11.3.1.
 */
typedef enum READER_T1_BlockType READER_T1_BlockType;
enum READER_T1_BlockType{
	READER_T1_SBLOCK                   = (uint32_t)(0x00000000),  /*!< To describe an S-Block. */
	READER_T1_RBLOCK                   = (uint32_t)(0x00000001),  /*!< To describe an R-Block. */
	READER_T1_IBLOCK                   = (uint32_t)(0x00000002),  /*!< To describe an I-Block. */
	READER_T1_BLOCK_ERR                = (uint32_t)(0x00000003)   /*!< To notify an error. */
};



/**
 * \struct READER_T1_Block
 * This struture represents a Block object as described in ISO7816-3 section 11.3.1. 
 */
typedef struct READER_T1_Block READER_T1_Block;
struct READER_T1_Block{
	uint8_t blockFrame[READER_T1_BLOCK_MAX_TOTAL_LENGTH];  /*!< The block is represented as an array of characters. These characters are INF +NAD +PCB +LEN  followed by the data bytes and CRC/LRC */
	READER_T1_RedundancyType RedundancyType;               /*!< The type of correction code used in this Block. */
};



READER_Status READER_T1_SetBlockSAD(READER_T1_Block *pBlock, uint8_t blockSAD);
READER_Status READER_T1_SetBlockDAD(READER_T1_Block *pBlock, uint8_t blockDAD);
READER_Status READER_T1_SetBlockNAD(READER_T1_Block *pBlock, uint8_t blockNAD);
READER_Status READER_T1_SetBlockPCB(READER_T1_Block *pBlock, uint8_t blockPCB);
READER_Status READER_T1_SetBlockType(READER_T1_Block *pBlock, READER_T1_BlockType type);
READER_Status READER_T1_SetBlockLEN(READER_T1_Block *pBlock, uint8_t blockLEN);
READER_Status READER_T1_SetBlockRedundancyType(READER_T1_Block *pBlock, READER_T1_RedundancyType type);
READER_Status READER_T1_SetBlockLRC(READER_T1_Block *pBlock, uint8_t blockLRC);
READER_Status READER_T1_SetBlockCRC(READER_T1_Block *pBlock, uint16_t blockCRC);
READER_Status READER_T1_SetBlockData(READER_T1_Block *pBlock, uint8_t *data, uint8_t dataSize);

READER_Status READER_T1_UpdateBlockChecksum(READER_T1_Block *pBlock);


uint8_t READER_T1_ComputeBlockLRC(READER_T1_Block *pBlock);
uint16_t READER_T1_ComputeBlockCRC(READER_T1_Block *pBlock);


uint8_t READER_T1_GetBlockSAD(READER_T1_Block *pBlock);
uint8_t READER_T1_GetBlockDAD(READER_T1_Block *pBlock);
uint8_t READER_T1_GetBlockNAD(READER_T1_Block *pBlock);
uint8_t READER_T1_GetBlockPCB(READER_T1_Block *pBlock);
READER_T1_BlockType READER_T1_GetBlockType(READER_T1_Block *pBlock);
uint8_t READER_T1_GetBlockLEN(READER_T1_Block *pBlock);
READER_T1_RedundancyType READER_T1_GetBlockRedundancyType(READER_T1_Block *pBlock);
uint32_t READER_T1_GetBlockRedundancyLen(READER_T1_Block *pBlock);
uint8_t READER_T1_GetBlockLRC(READER_T1_Block *pBlock);
uint16_t READER_T1_GetBlockCRC(READER_T1_Block *pBlock);
uint8_t* READER_T1_GetBlockData(READER_T1_Block *pBlock);
uint32_t READER_T1_GetBlockTotalSize(READER_T1_Block *pBlock);
uint32_t READER_T1_GetBlockSizeWithoutCheck(READER_T1_Block *pBlock);
uint8_t* READER_T1_GetBlockFrame(READER_T1_Block *pBlock);

READER_Status READER_T1_CheckBlockIntegrity(READER_T1_Block *pBlock, READER_T1_RedundancyType rType);

READER_Status READER_T1_ForgeBlock(READER_T1_Block *pBlock, READER_T1_RedundancyType rType);
READER_Status READER_T1_SendBlock(READER_T1_Block *pBlock, uint32_t currentCWT, uint32_t extraStartDelay, uint32_t *pTickstart, READER_HAL_CommSettings *pSettings);
READER_Status READER_T1_RcvBlock(READER_T1_Block *pBlock, READER_T1_RedundancyType rType, uint32_t currentCWT, uint32_t extraTimeout, uint32_t *pTickstart, READER_HAL_CommSettings *pSettings);


READER_Status READER_T1_CopyBlock(READER_T1_Block *pBlockDest, READER_T1_Block *pBlockSource);
READER_Status READER_T1_CopyBlockData(READER_T1_Block *pBlock, uint8_t *destBuffer, uint32_t destBufferSize);


#endif

