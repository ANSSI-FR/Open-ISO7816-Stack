#ifndef __READER_T1_PROTOCOL_H__
#define __READER_T1_PROTOCOL_H__



#include "reader_t1_protocol.h"
#include "reader_hal.h"
#include "reader.h"




READER_Status READER_T1_IsIBlockACK(READER_T1_Block *pPrevIBlock, READER_T1_Block *pRcvdBlock);
READER_Status READER_T1_IsIBlockNACK(READER_T1_Block *pPrevIBlock, READER_T1_Block *pRcvdBlock);
READER_Status READER_T1_ProcessSBlockRequ(READER_T1_Block *pSBlockRequ);
READER_Status READER_T1_ProcessResynchRequ(READER_T1_Block *pSBlockRequ);
READER_Status READER_T1_ProcessAbortRequ(READER_T1_Block *pSBlockRequ);
READER_Status READER_T1_ProcessWTXRequ(READER_T1_Block *pSBlockRequ);
READER_Status READER_T1_ProcessIFSRequ(READER_T1_Block *pSBlockRequ);
READER_Status READER_T1_ProcessCorruptedBlock(READER_T1_Block *pPrevSndBlock, READER_T1_Block *pRcvdBlock, uint32_t timeout, uint32_t maxTries);
READER_Status READER_T1_RcvBlockAndCheck(READER_T1_Block *pPrevSndBlock, READER_T1_Block *pRcvdBlock, uint32_t timeout, uint32_t maxTries);





#endif
