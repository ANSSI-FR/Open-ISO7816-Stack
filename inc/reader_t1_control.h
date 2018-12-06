#include "reader_t1.h"


#ifndef __READER_T1_CONTROL_H__
#define __READER_T1_CONTROL_H__




READER_Status READER_T1_CONTROL_CheckRBlockIsValid(READER_T1_Block *pBlock);
READER_Status READER_T1_CONTROL_IsSeqNumValid(READER_T1_ContextHandler *pContext, READER_T1_Block *pBlock);
READER_Status READER_T1_CONTROL_IsRBlockACK(READER_T1_ContextHandler *pContext, READER_T1_Block *pBlock);
READER_Status READER_T1_CONTROL_IsRBlockError(READER_T1_ContextHandler *pContext, READER_T1_Block *pBlock);
READER_Status READER_T1_CONTROL_IsIBlockACK(READER_T1_ContextHandler *pContext, READER_T1_Block *pBlock);



READER_Status READER_T1_CONTROL_SendBlock(READER_T1_ContextHandler *pContext, READER_T1_Block *pBlockToSend);
READER_Status READER_T1_CONTROL_RcvBlock(READER_T1_ContextHandler *pContext, READER_T1_Block *pBlock);


READER_Status READER_T1_CONTROL_IBlockSentUpdateContext(READER_T1_ContextHandler *pContext, READER_T1_Block *pBlock);
READER_Status READER_T1_CONTROL_RBlockSentUpdateContext(READER_T1_ContextHandler *pContext, READER_T1_Block *pBlock);
READER_Status READER_T1_CONTROL_SBlockSentUpdateContext(READER_T1_ContextHandler *pContext, READER_T1_Block *pBlock);

READER_Status READER_T1_CONTROL_ApplyIBlockRcvd(READER_T1_ContextHandler *pContext, READER_T1_Block *pBlock);
READER_Status READER_T1_CONTROL_ApplyRBlockRcvd(READER_T1_ContextHandler *pContext, READER_T1_Block *pBlock);
READER_Status READER_T1_CONTROL_ApplySBlockRcvd(READER_T1_ContextHandler *pContext, READER_T1_Block *pBlock);


#endif
