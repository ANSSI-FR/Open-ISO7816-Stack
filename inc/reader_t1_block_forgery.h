/**
* \file reader_t1_block_forgery.h
* \author Boris
*/


#ifndef __READER_T1_BLOCK_FORGERY_H__
#define __READER_T1_BLOCK_FORGERY_H__



#include "reader_t1.h"



READER_Status READER_T1_FORGE_ChainingRBlockForCard(READER_T1_ContextHandler *pContext, READER_T1_Block *pBlockDest);

READER_Status READER_T1_FORGE_ErrorBlock(READER_T1_ContextHandler *pContext, READER_T1_Block *pBlockDest, uint32_t integrityFlag);

READER_Status READER_T1_FORGE_NACKR0(READER_T1_ContextHandler *pContext, READER_T1_Block *pBlockDest, uint32_t integrityFlag);
READER_Status READER_T1_FORGE_NACK71(READER_T1_ContextHandler *pContext, READER_T1_Block *pBlockDest, uint32_t integrityFlag);
READER_Status READER_T1_FORGE_NACK72(READER_T1_ContextHandler *pContext, READER_T1_Block *pBlockDest, uint32_t integrityFlag);


READER_Status READER_T1_FORGE_ACKIBlock(READER_T1_ContextHandler *pContext, READER_T1_Block *pBlockDest);
READER_Status READER_T1_FORGE_DataIBlock(READER_T1_ContextHandler *pContext, READER_T1_Block *pBlockDest, uint8_t *dataBuff, uint32_t dataSize, uint32_t mBit);

READER_Status READER_T1_FORGE_SliceDataAndFillBuffer(READER_T1_ContextHandler *pContext, uint8_t *dataBuff, uint32_t dataSize);
READER_Status READER_T1_FORGE_FillBuffWithAPDU(READER_T1_ContextHandler *pContext, READER_APDU_Command *pApduCmd);


READER_Status READER_T1_FORGE_FillBuffWithAPDUCase1(READER_T1_ContextHandler *pContext, READER_APDU_Command *pApduCmd);
READER_Status READER_T1_FORGE_FillBuffWithAPDUCase2S(READER_T1_ContextHandler *pContext, READER_APDU_Command *pApduCmd);
READER_Status READER_T1_FORGE_FillBuffWithAPDUCase2E(READER_T1_ContextHandler *pContext, READER_APDU_Command *pApduCmd);
READER_Status READER_T1_FORGE_FillBuffWithAPDUCase3S(READER_T1_ContextHandler *pContext, READER_APDU_Command *pApduCmd);
READER_Status READER_T1_FORGE_FillBuffWithAPDUCase3E(READER_T1_ContextHandler *pContext, READER_APDU_Command *pApduCmd);
READER_Status READER_T1_FORGE_FillBuffWithAPDUCase4S(READER_T1_ContextHandler *pContext, READER_APDU_Command *pApduCmd);
READER_Status READER_T1_FORGE_FillBuffWithAPDUCase4E(READER_T1_ContextHandler *pContext, READER_APDU_Command *pApduCmd);



READER_Status READER_T1_FORGE_IBlock(READER_T1_ContextHandler *pContext, READER_T1_Block *pBlock, uint8_t *data, uint32_t dataSize, READER_T1_SeqNumber seq, READER_T1_MBit mBit);
READER_Status READER_T1_FORGE_RBlock(READER_T1_ContextHandler *pContext, READER_T1_Block *pBlock, READER_T1_ACKType ack, READER_T1_ExpSeqNumber expctdBlockSeq);
READER_Status READER_T1_FORGE_SBlock(READER_T1_ContextHandler *pContext, READER_T1_Block *pBlock, READER_T1_SBlockType type);



READER_Status READER_T1_FORGE_SBlockResynchRequest(READER_T1_ContextHandler *pContext, READER_T1_Block *pBlock);
READER_Status READER_T1_FORGE_SBlockResynchResponse(READER_T1_ContextHandler *pContext, READER_T1_Block *pBlock);
READER_Status READER_T1_FORGE_SBlockIfsRequest(READER_T1_ContextHandler *pContext, READER_T1_Block *pBlock, uint8_t newIfs);
READER_Status READER_T1_FORGE_SBlockIfsResponse(READER_T1_ContextHandler *pContext, READER_T1_Block *pBlock, uint8_t newIfs);
READER_Status READER_T1_FORGE_SBlockAbortRequest(READER_T1_ContextHandler *pContext, READER_T1_Block *pBlock);
READER_Status READER_T1_FORGE_SBlockAbortResponse(READER_T1_ContextHandler *pContext, READER_T1_Block *pBlock);
READER_Status READER_T1_FORGE_SBlockWtxRequest(READER_T1_ContextHandler *pContext, READER_T1_Block *pBlock, uint8_t wtMultiplier);
READER_Status READER_T1_FORGE_SBlockWtxResponse(READER_T1_ContextHandler *pContext, READER_T1_Block *pBlock, uint8_t wtMultiplier);


#endif
