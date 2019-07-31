#ifndef __READER_T1_RCPTBUFFER_HANDLER_H__
#define __READER_T1_RCPTBUFFER_HANDLER_H__


#include "reader_t1.h"



READER_Status READER_T1_RCPTBUFF_Init(READER_T1_ContextHandler *pContext);
READER_Status READER_T1_RCPTBUFF_ExtractDataFromIBlock(READER_T1_ContextHandler *pContext, READER_T1_Block *pBlock);
READER_Status READER_T1_RCPTBUFF_CreateAPDUResp(READER_T1_ContextHandler *pContext, READER_APDU_Response *pApduResp);




#endif
