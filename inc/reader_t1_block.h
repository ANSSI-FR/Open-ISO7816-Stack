#include "reader.h"
#include "reader_hal.h"


#define READER_T1_BLOCK_MAX_DATA_SIZE     254     /* Voir ISO7816-3 section 11.3.2.3 */
#define READER_T1_BLOCK_PROLOGUE_SIZE     3
#define READER_T1_BLOCKFRAME_NAD_POSITION 0       /* Voir ISO7816-3 section 11.3.1   */
#define READER_T1_BLOCKFRAME_PCB_POSITION 1       /* Voir ISO7816-3 section 11.3.1   */
#define READER_T1_BLOCKFRAME_LEN_POSITION 2       /* Voir ISO7816-3 section 11.3.1   */
#define READER_T1_BLOCKFRAME_INF_POSITION 3       /* Voir ISO7816-3 section 11.3.1   */
#define READER_T1_CRC_POLY                0x91



typedef enum READER_T1_RedundancyType READER_T1_RedundancyType;
enum READER_T1_RedundancyType{
	READER_T1_CRC                      = (uint32_t)(0x00000000),
	READER_T1_LRC                      = (uint32_t)(0x00000001)
};




typedef enum READER_T1_BlockType READER_T1_BlockType;
enum READER_T1_BlockType{
	READER_T1_SBLOCK                   = (uint32_t)(0x00000000),
	READER_T1_RBLOCK                   = (uint32_t)(0x00000001),
	READER_T1_IBLOCK                   = (uint32_t)(0x00000002),
	READER_T1_BLOCK_ERR                = (uint32_t)(0x00000003)
};


//typedef struct READER_T1_Block READER_T1_Block;
//struct READER_T1_Block{
//	uint8_t NAD;
//	uint8_t PCB;
//	uint8_t LEN;
//	uint8_t data[READER_T1_BLOC_MAX_DATA_SIZE];
//	READER_T1_RedundancyType RedundancyType;
//	uint8_t LRC;
//	uint16_t CRC;
//};

typedef struct READER_T1_Block READER_T1_Block;
struct READER_T1_Block{
	uint8_t blockFrame[READER_T1_BLOCK_MAX_DATA_SIZE + 5];  /* INF +NAD +PCB +LEN +CRC/LRC*/
	READER_T1_RedundancyType RedundancyType;
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


uint8_t READER_T1_ComputeBlockLRC(READER_T1_Block *pBlock);
uint16_t READER_T1_ComputeBlockCRC(READER_T1_Block *pBlock);


uint8_t READER_T1_GetBlockSAD(const READER_T1_Block *pBlock);
uint8_t READER_T1_GetBlockDAD(const READER_T1_Block *pBlock);
uint8_t READER_T1_GetBlockNAD(const READER_T1_Block *pBlock);
uint8_t READER_T1_GetBlockPCB(const READER_T1_Block *pBlock);
READER_T1_BlockType READER_T1_GetBlockType(const READER_T1_Block *pBlock);
uint8_t READER_T1_GetBlockLEN(const READER_T1_Block *pBlock);
READER_T1_RedundancyType READER_T1_GetBlockRedundancyType(const READER_T1_Block *pBlock);
uint8_t READER_T1_GetBlockLRC(const READER_T1_Block *pBlock);
uint16_t READER_T1_GetBlockCRC(const READER_T1_Block *pBlock);
uint8_t* READER_T1_GetBlockData(const READER_T1_Block *pBlock);
uint32_t READER_T1_GetBlockTotalSize(const READER_T1_Block *pBlock);
uint32_t READER_T1_GetBlockSizeWithoutCheck(const READER_T1_Block *pBlock);
uint8_t* READER_T1_GetBlockFrame(const READER_T1_Block *pBlock);

READER_Status READER_T1_CheckBlockIntegrity(READER_T1_Block *pBlock);

READER_Status READER_T1_ForgeBlock(READER_T1_Block *pBlock, READER_T1_RedundancyType rType);
READER_Status READER_T1_SendBlock(READER_T1_Block *pBlock, uint32_t timeout);
READER_Status READER_T1_RcvBlock(READER_T1_Block *pBlock, uint32_t timeout);

