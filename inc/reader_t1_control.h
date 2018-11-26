#ifndef __READER_T1_CONTROL_H__
#define __READER_T1_CONTROL_H__


#include "reader.h"
#include "reader_t1_block"
#include "reader_t1_context_handler.h"




READER_Status READER_T1_CONTROL_CheckRBlockIsValid(READER_T1_Block *pBlock);
READER_Status READER_T1_CONTROL_IsSeqNumValid(READER_T1_ContextHandler *pContext, READER_T1_Block *pBlock);
READER_Status READER_T1_CONTROL_IsRBlockACK(READER_T1_ContextHandler *pContext, READER_T1_Block *pBlock);
READER_Status READER_T1_CONTROL_IsRBlockError(READER_T1_ContextHandler *pContext, READER_T1_Block *pBlock);
READER_Status READER_T1_CONTROL_IsIBlockACK(READER_T1_ContextHandler *pContext, READER_T1_Block *pBlock);




READER_Status READER_T1_CONTROL_SendBlock(READER_T1_ContextHandler *pContext, READER_T1_Block *pBlock);
READER_Status READER_T1_CONTROL_RcvBlock(READER_T1_ContextHandler *pContext, READER_T1_Block *pBlock);



#endif
