#include "reader_hal.h"
#include "reader_t1.h"



#ifndef __READER_T1_BUFFER_HANDLER_H__
#define __READER_T1_BUFFER_HANDLER_H__




#define READER_T1_BUFFER_MAXBYTES READER_APDU_CMD_MAX_TOTALSIZE + 3*(READER_T1_RBLOCK_MAXSIZE) + 3*(READER_T1_SBLOCK_MAXSIZE)



typedef enum READER_T1_BufferStatus READER_T1_BufferStatus;
enum READER_T1_BufferStatus{
	READER_T1_BUFFER_EMPTY             =       (uint32_t)(0x00000000),
	READER_T1_BUFFER_NOTEMPTY          =       (uint32_t)(0x00000001),
	READER_T1_BUFFER_FULL              =       (uint32_t)(0x00000002),
	READER_T1_BUFFER_NOTFULL           =       (uint32_t)(0x00000003)
};



READER_Status READER_T1_BUFFER_Init(READER_T1_ContextHandler *pContext);
READER_Status READER_T1_BUFFER_Clear(READER_T1_ContextHandler *pContext);
READER_Status READER_T1_BUFFER_PlacesLeft(READER_T1_ContextHandler *pContext, uint32_t *pPlaces);
READER_Status READER_T1_BUFFER_GetLength(READER_T1_ContextHandler *pContext, uint32_t *pLength);
READER_Status READER_T1_BUFFER_IsEmpty(READER_T1_ContextHandler *pContext, READER_T1_BufferStatus *pStatus);
READER_Status READER_T1_BUFFER_IsFull(READER_T1_ContextHandler *pContext, READER_T1_BufferStatus *pStatus);
READER_Status READER_T1_BUFFER_Enqueue(READER_T1_ContextHandler *pContext, READER_T1_Block *pBlock);
READER_Status READER_T1_BUFFER_Dequeue(READER_T1_ContextHandler *pContext, READER_T1_Block *pBlock);
READER_Status READER_T1_BUFFER_Stack(READER_T1_ContextHandler *pContext, READER_T1_Block *pBlock);
//READER_Status READER_T1_BUFFER_Delete(READER_T1_ContextHandler *pContext, uint32_t index);
//READER_Status READER_T1_BUFFER_ShiftInBottomDirection(READER_T1_ContextHandler *pContext);
//READER_Status READER_T1_BUFFER_ShiftInTopDirection(READER_T1_ContextHandler *pContext);


READER_Status READER_T1_BUFFER_StripControlBlocks(READER_T1_ContextHandler *pContext);

READER_Status READER_T1_BUFFER_UpdateIfsc(READER_T1_ContextHandler *pContext, uint32_t newIFSC);
READER_Status READER_T1_BUFFER_ExtractRawDataFromBuffer(READER_T1_ContextHandler *pContext, uint8_t *buffer, uint32_t bufferSize, uint32_t *pSizeExtracted);


#endif
