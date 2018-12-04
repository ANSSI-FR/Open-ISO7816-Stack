#include "reader.h"
#include "reader_hal.h"
#include "reader_t1_block.h"


#ifndef __READER_T1_RBLOCK_H__
#define __READER_T1_RBLOCK_H__




typedef enum READER_T1_ACKType READER_T1_ACKType;
enum READER_T1_ACKType{
	READER_T1_ACKTYPE_ACK               =     (uint32_t)(0x00000000),
	READER_T1_ACKTYPE_NACK              =     (uint32_t)(0x00000001),
	READER_T1_ACKTYPE_NACK_CRCLRC       =     (uint32_t)(0x00000002)
};


typedef enum READER_T1_ExpSeqNumber READER_T1_ExpSeqNumber;
enum READER_T1_ExpSeqNumber{
	READER_T1_EXPSEQNUM_ZERO            =      (uint32_t)(0x00000000),
	READER_T1_EXPSEQNUM_ONE             =      (uint32_t)(0x00000001)
};




READER_Status READER_T1_SetBlockACKType(READER_T1_Block *pBlock, READER_T1_ACKType ack);
READER_Status READER_T1_SetExpectedBlockSeqNumber(READER_T1_Block *pBlock, READER_T1_ExpSeqNumber seq);

READER_T1_ACKType READER_T1_GetBlockACKType(READER_T1_Block *pBlock);
READER_T1_ExpSeqNumber READER_T1_GetExpectedBlockSeqNumber(READER_T1_Block *pBlock);

READER_Status READER_T1_ForgeRBlock(READER_T1_Block *pBlock, READER_T1_ACKType ack, READER_T1_ExpSeqNumber expctdBlockSeq);

//READER_Status READER_T1_SendBlockACK(READER_T1_Block *pBlock, READER_T1_ExpSeqNumber expctdBlockSeq, uint32_t timeout);
//READER_Status READER_T1_SendBlockNACK(READER_T1_Block *pBlock, READER_T1_ExpSeqNumber expctdBlockSeq, uint32_t timeout);
//READER_Status READER_T1_SendBlockNACK_CRCLRC(READER_T1_Block *pBlock, READER_T1_ExpSeqNumber expctdBlockSeq, uint32_t timeout);



#endif
