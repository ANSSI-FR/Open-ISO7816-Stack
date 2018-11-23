#ifndef __READER_T1_BLOCK_FORGERY_H__
#define __READER_T1_BLOCK_FORGERY_H__


#include "reader.h"
#include "reader_t1_block"
#include "reader_t1_context_handler.h"




READER_Status READER_T1_FORGE_ChainingRBlockForCard(READER_T1_ContextHandler *pContext, READER_T1_Block *pBlockDest);

READER_Status READER_T1_FORGE_ErrorBlock(READER_T1_ContextHandler *pContext, READER_T1_Block *pBlockDest, integrityFlag);

READER_Status READER_T1_FORGE_NACKR0(READER_T1_ContextHandler *pContext, READER_T1_Block *pBlockDest, uint32_t integrityFlag);
READER_Status READER_T1_FORGE_NACK71(READER_T1_ContextHandler *pContext, READER_T1_Block *pBlockDest, uint32_t integrityFlag);
READER_Status READER_T1_FORGE_NACK72(READER_T1_ContextHandler *pContext, READER_T1_Block *pBlockDest, uint32_t integrityFlag);


READER_Status READER_T1_FORGE_ACKIBlock(READER_T1_ContextHandler *pContext, READER_T1_Block *pBlockDest);
READER_Status READER_T1_FORGE_DataIBlock(READER_T1_ContextHandler *pContext, READER_T1_Block *pBlockDest, uint8_t *dataBuff, uint32_t dataSize, uint32_t mBit);

READER_Status READER_T1_FORGE_SliceDataAndFillBuffer(READER_T1_ContextHandler *pContext, uint8_t *dataBuff, uint32_t dataSize);







#endif
