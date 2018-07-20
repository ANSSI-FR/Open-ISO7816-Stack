#ifndef __READER_T1_PROTOCOL_H__
#define __READER_T1_PROTOCOL_H__



#include "reader_t1_protocol.h"
#include "reader_hal.h"
#include "reader.h"




READER_Status READER_T1_IsBlockACK(READER_T1_Block *pBlockSnd, READER_T1_Block *pBlockRcvd);
READER_Status READER_T1_IsBlockNACK(READER_T1_Block *pBlockSnd, READER_T1_Block *pBlockRcvd);
READER_Status READER_T1_ProcessSBlockRequ(READER_T1_Block *pSBlockRequ);
READER_Status READER_T1_ProcessResynchRequ(READER_T1_Block *pSBlockRequ);
READER_Status READER_T1_ProcessAbortRequ(READER_T1_Block *pSBlockRequ);
READER_Status READER_T1_ProcessWTXRequ(READER_T1_Block *pSBlockRequ);
READER_Status READER_T1_ProcessIFSRequ(READER_T1_Block *pSBlockRequ);





#endif
