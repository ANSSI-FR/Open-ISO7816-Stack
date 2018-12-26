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


/* Mises a jour du contexte lors de m'envoi d'un Block ...  */
READER_Status READER_T1_CONTROL_IBlockSentUpdateContext(READER_T1_ContextHandler *pContext, READER_T1_Block *pBlock);
READER_Status READER_T1_CONTROL_RBlockSentUpdateContext(READER_T1_ContextHandler *pContext, READER_T1_Block *pBlock);
READER_Status READER_T1_CONTROL_SBlockSentUpdateContext(READER_T1_ContextHandler *pContext, READER_T1_Block *pBlock);

READER_Status READER_T1_CONTROL_SBlockRequestSentUpdateContext(READER_T1_ContextHandler *pContext, READER_T1_Block *pBlock);
READER_Status READER_T1_CONTROL_SBlockResponseSentUpdateContext(READER_T1_ContextHandler *pContext, READER_T1_Block *pBlock);

/* Mises a jour du contexte lors de la reception d'un Block ...  */
READER_Status READER_T1_CONTROL_ApplyIBlockRcvd(READER_T1_ContextHandler *pContext, READER_T1_Block *pBlock);
READER_Status READER_T1_CONTROL_ApplyRBlockRcvd(READER_T1_ContextHandler *pContext, READER_T1_Block *pBlock);
READER_Status READER_T1_CONTROL_ApplySBlockRcvd(READER_T1_ContextHandler *pContext, READER_T1_Block *pBlock);

READER_Status READER_T1_CONTROL_ApplySBlockRequestRcvd(READER_T1_ContextHandler *pContext, READER_T1_Block *pBlock);
READER_Status READER_T1_CONTROL_ApplySBlockResponseRcvd(READER_T1_ContextHandler *pContext, READER_T1_Block *pBlock);


READER_Status READER_T1_CONTROL_ApplySBlockResynch(READER_T1_ContextHandler *pContext, READER_T1_Block *pBlock);
READER_Status READER_T1_CONTROL_ApplySBlockIfsc(READER_T1_ContextHandler *pContext, READER_T1_Block *pBlock);
READER_Status READER_T1_CONTROL_ApplySBlockIfsd(READER_T1_ContextHandler *pContext, READER_T1_Block *pBlock);
READER_Status READER_T1_CONTROL_ApplySBlockAbort(READER_T1_ContextHandler *pContext, READER_T1_Block *pBlock);
READER_Status READER_T1_CONTROL_ApplySBlockWtx(READER_T1_ContextHandler *pContext, READER_T1_Block *pBlock);


READER_Status READER_T1_CONTROL_SendAbortRequest(READER_T1_ContextHandler *pContext);
READER_Status READER_T1_CONTROL_SendResynchRequest(READER_T1_ContextHandler *pContext);
READER_Status READER_T1_CONTROL_SendIfsdRequest(READER_T1_ContextHandler *pContext, uint8_t newIfsd);
READER_Status READER_T1_CONTROL_SendWtxRequest(READER_T1_ContextHandler *pContext, uint8_t wtMultiplier);


/* Gestion des problemes S-Blocks ...  */
READER_Status READER_T1_CONTROL_ResendRequest(READER_T1_ContextHandler *pContext);
READER_Status READER_T1_CONTROL_SBlockResponseNotReceived(READER_T1_ContextHandler *pContext);


#endif
