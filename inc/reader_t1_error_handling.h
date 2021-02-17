/**
 * \file reader_t1_error_handling.h
 * \copyright This file is part of the Open-ISO7816-Stack project and is distributed under the MIT license. See LICENSE file in the root directory. 
 */


#ifndef __READER_T1_ERROR_HANDLING_H__
#define __READER_T1_ERROR_HANDLING_H__


#include "reader_t1.h"



READER_Status READER_T1_ERR_DealWithError(READER_T1_ContextHandler *pContext, uint32_t integrityFlag);

READER_Status READER_T1_ERR_IsItFirstReception(READER_T1_ContextHandler *pContext);

READER_Status READER_T1_ERR_ForgeErrorBlock(READER_T1_ContextHandler *pContext, READER_T1_Block *pBlockDest, uint32_t integrityFlag);
READER_Status READER_T1_ERR_StackErrorBlock(READER_T1_ContextHandler *pContext, READER_T1_Block *pErrorBlock);

READER_Status READER_T1_ERR_IncRepeatCounter(READER_T1_ContextHandler *pContext);
READER_Status READER_T1_ERR_ClearRepeatCounter(READER_T1_ContextHandler *pContext);
READER_Status READER_T1_ERR_CheckRepeatCounter(READER_T1_ContextHandler *pContext);

READER_Status READER_T1_ERR_IncResynchCounter(READER_T1_ContextHandler *pContext);
READER_Status READER_T1_ERR_ClearResynchCounter(READER_T1_ContextHandler *pContext);
READER_Status READER_T1_ERR_CheckResynchCounter(READER_T1_ContextHandler *pContext);
READER_Status READER_T1_ERR_PrepareResynchRequ(READER_T1_ContextHandler *pContext);
READER_Status READER_T1_ERR_DoResynch(READER_T1_ContextHandler *pContext);

READER_Status READER_T1_ERR_DoReset(READER_T1_ContextHandler *pContext);


#endif
