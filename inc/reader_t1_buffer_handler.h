#ifndef __READER_T1_BUFFER_HANDLER_H__
#define __READER_T1_BUFFER_HANDLER_H__


#include "reader.h"
#include "reader_hal.h"
#include "reader_t1_block"
#include "reader_t1_context_handler.h"





#define MAX(x,y) x > y ? x : y



/* Choix de ces valeurs en fonction de ISO7816-3 section 11.4.2 et contraintes cible dev */
#define READER_T1_MAX_IFSD_ACCEPTED 0xFE
#define READER_T1_MIN_IFSD_ACCEPTED 0x10
#define READER_T1_MAX_IFSC_ACCEPTED 0xFE
#define READER_T1_MIN_IFSC_ACCEPTED 0x10  


#define COMPUTE_STATICBUFF_MAXSIZE(apdu_maxlength, block_data_minlength, tolerence) ((apdu_maxlength / block_data_minlength) + 1) + tolerence
#define APDU_MAXLENGTH READER_APDU_CMD_MAX_SIZE

#define STATICBUFF_MAXSIZE COMPUTE_STATICBUFF_MAXSIZE(APDU_MAXLENGTH, READER_T1_MIN_IFSC_ACCEPTED, 20)





typedef struct READER_T1_BlockBuffer READER_T1_BlockBuffer;
struct READER_T1_BlockBuffer{
	READER_T1_Block blockBuff[STATICBUFF_MAXSIZE];
	uint32_t indexBottom;
	uint32_t indexTop;
	uint32_t length;
	uint32_t flagEmpty;
	uint32_t flagFull;
};



typedef enum READER_T1_BufferStatus READER_T1_BufferStatus;
enum READER_T1_BufferStatus{
	READER_T1_BUFFER_EMPTY             =       (uint32_t)(0x00000000),
	READER_T1_BUFFER_NOTEMPTY          =       (uint32_t)(0x00000001),
	READER_T1_BUFFER_FULL              =       (uint32_t)(0x00000002),
	READER_T1_BUFFER_NOTFULL           =       (uint32_t)(0x00000003)
};



READER_Status READER_T1_BUFFER_Init(READER_T1_ContextHandler *pContext);
READER_Status READER_T1_BUFFER_Clear(READER_T1_ContextHandler *pContext);
READER_Status READER_T1_BUFFER_PlacesLeft(READER_T1_ContextHandler *pContext, uint32_t *places);
READER_Status READER_T1_BUFFER_IsEmpty(READER_T1_ContextHandler *pContext, READER_T1_BufferStatus *pStatus);
READER_Status READER_T1_BUFFER_IsFull(READER_T1_ContextHandler *pContext, READER_T1_BufferStatus *pStatus);
READER_Status READER_T1_BUFFER_Enqueue(READER_T1_ContextHandler *pContext, READER_T1_Block *pBlock);
READER_Status READER_T1_BUFFER_Dequeue(READER_T1_ContextHandler *pContext, READER_T1_Block *pBlock);
READER_Status READER_T1_BUFFER_Stack(READER_T1_ContextHandler *pContext, READER_T1_Block *pBlock);


#endif
