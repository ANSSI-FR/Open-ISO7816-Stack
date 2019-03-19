#include "reader_hal.h"
#include "reader_t1.h"


#ifndef __READER_T1_IBLOCK_H__
#define __READER_T1_IBLOCK_H__



typedef enum READER_T1_SeqNumber READER_T1_SeqNumber;
enum READER_T1_SeqNumber{
	READER_T1_SEQNUM_ZERO            =      (uint32_t)(0x00000000),
	READER_T1_SEQNUM_ONE             =      (uint32_t)(0x00000001)
};


typedef enum READER_T1_MBit READER_T1_MBit;
enum READER_T1_MBit{
	READER_T1_MBIT_ZERO              =      (uint32_t)(0x00000000),
	READER_T1_MBIT_ONE               =      (uint32_t)(0x00000001)
};



READER_Status READER_T1_SetBlockSeqNumber(READER_T1_Block *pBlock, READER_T1_SeqNumber seq);
READER_Status READER_T1_SetBlockMBit(READER_T1_Block *pBlock, READER_T1_MBit mBit);

READER_T1_SeqNumber READER_T1_GetBlockSeqNumber(READER_T1_Block *pBlock);
READER_T1_MBit READER_T1_GetBlockMBit(READER_T1_Block *pBlock);

READER_Status READER_T1_ForgeIBlock(READER_T1_Block *pBlock, uint8_t *data, uint32_t dataSize, READER_T1_SeqNumber seq, READER_T1_MBit mBit);

READER_Status READER_T1_CheckIBlock(READER_T1_Block *pBlock);

#endif
