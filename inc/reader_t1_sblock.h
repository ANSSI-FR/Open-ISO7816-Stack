#ifndef __READER_T1_SBLOCK_H__
#define __READER_T1_SBLOCK_H__





typedef enum READER_T1_ACKType READER_T1_ACKType;
enum READER_T1_SBlockType{
	READER_T1_STYPE_RESYNCH_REQU        =       (uint32_t)(0x00000000),
	READER_T1_STYPE_RESYNCH_RESP        =       (uint32_t)(0x00000001),
	READER_T1_STYPE_IFS_REQU            =       (uint32_t)(0x00000002),
	READER_T1_STYPE_IFS_RESP            =       (uint32_t)(0x00000003),
	READER_T1_STYPE_ABORT_REQU          =       (uint32_t)(0x00000004),
	READER_T1_STYPE_ABORT_RESP          =       (uint32_t)(0x00000005),
	READER_T1_STYPE_WTX_REQU            =       (uint32_t)(0x00000006),
	READER_T1_STYPE_WTX_RESP            =       (uint32_t)(0x00000007),
	READER_T1_STYPE_DEPRECATED          =       (uint32_t)(0x00000008)
};



READER_Status READER_T1_ForgeSBlock(READER_T1_Block *pBlock, READER_T1_SBlockType type);


READER_Status READER_T1_SendBlockRESYNCHRequ(uint32_t timeout);
READER_Status READER_T1_RcvBlockRESYNCHResp(uint32_t timeout);
READER_Status READER_T1_SendBlockRESYNCHResp(uint32_t timeout);
READER_Status READER_T1_ExecuteRESYNCH(uint32_t timeout);

READER_Status READER_T1_SendBlockIFSRequ(uint8_t requValue, uint32_t timeout);
READER_Status READER_T1_RcvBlockIFSResp(uint8_t requValue, uint32_t timeout);
READER_Status READER_T1_SendBlockIFSResp(uint8_t requValue, uint32_t timeout);
READER_Status READER_T1_ExecuteIFS(uint8_t requValue, uint32_t timeout);

READER_Status READER_T1_SendBlockABORTRequ(uint32_t timeout);
READER_Status READER_T1_RcvBlockABORTResp(uint32_t timeout);
READER_Status READER_T1_SendBlockABORTResp(uint32_t timeout);
READER_Status READER_T1_ExecuteABORT(uint32_t timeout);

READER_Status READER_T1_SendBlockWTXRequ(uint8_t requValue, uint32_t timeoutv);
READER_Status READER_T1_RcvBlockWTXResp(uint8_t requValue, uint32_t timeout);
READER_Status READER_T1_SendBlockWTXResp(uint8_t requValue, uint32_t timeout);
READER_Status READER_T1_ExecuteWTX(uint8_t requValue, uint32_t timeout);


#endif
