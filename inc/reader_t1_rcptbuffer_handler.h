#ifndef __READER_T1_RCPTBUFFER_HANDLER_H__
#define __READER_T1_RCPTBUFFER_HANDLER_H__


#include "reader.h"



typedef struct READER_T1_ReceptionBuff READER_T1_ReceptionBuff;
struct READER_T1_ReceptionBuff{
	uint8_t rawReceptionBuff[READER_APDU_RESP_MAX_TOTALSIZE];
	uint32_t dataSize;
};




READER_Status READER_T1_RCPTBUFF_Init(READER_T1_ContextHandler *pContext);
READER_Status READER_T1_RCPTBUFF_ExtractDataFromIBlock(READER_T1_ContextHandler *pContext, READER_T1_Block *pBlock);
READER_Status READER_T1_RCPTBUFF_CreateAPDUResp(READER_T1_ContextHandler *pContext, READER_APDU_Response *pApduResp);




#endif
