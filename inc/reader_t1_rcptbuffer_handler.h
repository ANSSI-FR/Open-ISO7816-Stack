/**
 * \file reader_t1_rcptbuffer_handler.h
 * \copyright This file is part of the Open-ISO7816-Stack project and is distributed under the MIT license. See LICENSE file in the root directory. 
 */


#ifndef __READER_T1_RCPTBUFFER_HANDLER_H__
#define __READER_T1_RCPTBUFFER_HANDLER_H__


#include "reader.h"
#include "reader_t1_context_handler.h"


READER_Status READER_T1_RCPTBUFF_Init(READER_T1_ContextHandler *pContext);
READER_Status READER_T1_RCPTBUFF_ExtractDataFromIBlock(READER_T1_ContextHandler *pContext, READER_T1_Block *pBlock);
READER_Status READER_T1_RCPTBUFF_CreateAPDUResp(READER_T1_ContextHandler *pContext, READER_APDU_Response *pApduResp);



#endif
