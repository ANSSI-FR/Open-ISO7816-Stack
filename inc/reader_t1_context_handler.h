#ifndef __READER_T1_CONTEXT_HANDLER_H__
#define __READER_T1_CONTEXT_HANDLER_H__


#include "reader.h"
#include "reader_hal.h"
#include "reader_t1_block"
#include "reader_t1_buffer_handler.h"







#define READER_T1_MAX_REAPEAT 0x03
#define READER_T1_MAX_RESYNCH 0x03



typedef enum READER_T1_ACKStatus READER_T1_ACKStatus;
enum READER_T1_ACKStatus{
	READER_T1_NOACK        =       (uint32_t)(0x00000000),
	READER_T1_ACK          =       (uint32_t)(0x00000001)
};


typedef enum READER_T1_ChainingStatus READER_T1_ChainingStatus;
enum READER_T1_ChainingStatus{
	READER_T1_CHAINING_YES         =       (uint32_t)(0x00000000),
	READER_T1_CHAINING_NO          =       (uint32_t)(0x00000001)
};


typedef enum READER_T1_SBlockExpected READER_T1_SBlockExpected;
enum READER_T1_SBlockExpected{
	READER_T1_SBLOCK_EXPECTED_YES         =       (uint32_t)(0x00000000),
	READER_T1_SBLOCK_EXPECTED_NO          =       (uint32_t)(0x00000001)
};


typedef struct READER_T1_ContextHandler READER_T1_ContextHandler;
struct READER_T1_ContextHandler{
	READER_T1_BlockBuffer blockBuff;
	READER_APDU_Response RcvdApduResp;                /* Donnees recues                                  */
	
	uint32_t currentIFSC;
	uint32_t currentIFSD;
	uint32_t currentCWI;
	uint32_t currentBWI;
	uint32_t currentBWT;
	uint32_t currentBGT;
	uint32_t currentCWT;
	uint32_t currentCGT;
	READER_T1_RedundancyType currentRType;
	
	uint32_t resynchCounter;
	uint32_t repeatCounter;
	
	READER_T1_ACKStatus ACKStatus;
	READER_T1_Block lastSent;
	READER_T1_Block lastIBlockSent;
	READER_T1_Block lastRcvd;
	READER_T1_Block lastIBlockRcvd;
	
	READER_T1_ChainingStatus cardIsChainingLastBlock;
	READER_T1_ChainingStatus deviceIsChainingLastBlock;
	READER_T1_ChainingStatus cardIsChaining;
	READER_T1_ChainingStatus deviceIsChaining;
	
	uint32_t deviceCompleteSeqNum;
	uint32_t cardCompleteSeqNum;
	
	
	READER_T1_SBlockExpected SBlockExpected;
	READER_T1_SBlockType SBlockExpectedType;
	uint32_t SblockRequCounter;
	
	
};

/* Initialisation de la structure */
READER_Status READER_T1_CONTEXT_Init(READER_T1_ContextHandler *pContext);


/* Accesseurs sur les parametres actuels de communication */
READER_Status READER_T1_CONTEXT_GetCurrentBGT(READER_T1_ContextHandler *pContext, uint32_t *pBgt);
READER_Status READER_T1_CONTEXT_GetCurrentBWT(READER_T1_ContextHandler *pContext, uint32_t *pBwt);
READER_Status READER_T1_CONTEXT_GetCurrentCGT(READER_T1_ContextHandler *pContext, uint32_t *pCgt);
READER_Status READER_T1_CONTEXT_GetCurrentCWT(READER_T1_ContextHandler *pContext, uint32_t *pCwt);
READER_Status READER_T1_CONTEXT_GetCurrentCWI(READER_T1_ContextHandler *pContext, uint32_t *pCwi);
READER_Status READER_T1_CONTEXT_GetCurrentBWI(READER_T1_ContextHandler *pContext, uint32_t *pBwi);
READER_Status READER_T1_CONTEXT_GetCurrentIFSC(READER_T1_ContextHandler *pContext, uint32_t *pIfsc);
READER_Status READER_T1_CONTEXT_GetCurrentIFSD(READER_T1_ContextHandler *pContext, uint32_t *pIfsd);
READER_Status READER_T1_CONTEXT_GetCurrentRedundancyType(READER_T1_ContextHandler *pContext, READER_T1_RedundancyType *pRType);

READER_Status READER_T1_CONTEXT_SetCurrentBGT(READER_T1_ContextHandler *pContext, uint32_t bgt);
READER_Status READER_T1_CONTEXT_SetCurrentBWT(READER_T1_ContextHandler *pContext, uint32_t bwt);
READER_Status READER_T1_CONTEXT_SetCurrentCGT(READER_T1_ContextHandler *pContext, uint32_t cgt);
READER_Status READER_T1_CONTEXT_SetCurrentCWT(READER_T1_ContextHandler *pContext, uint32_t cwt);
READER_Status READER_T1_CONTEXT_SetCurrentCWI(READER_T1_ContextHandler *pContext, uint32_t cwi);
READER_Status READER_T1_CONTEXT_SetCurrentBWI(READER_T1_ContextHandler *pContext, uint32_t bwi);
READER_Status READER_T1_CONTEXT_SetCurrentIFSC(READER_T1_ContextHandler *pContext, uint32_t ifsc);
READER_Status READER_T1_CONTEXT_SetCurrentIFSD(READER_T1_ContextHandler *pContext, uint32_t ifsd);
READER_Status READER_T1_CONTEXT_SetCurrentRedundancyType(READER_T1_ContextHandler *pContext, READER_T1_RedundancyType rType);


READER_Status READER_T1_CONTEXT_GetApduResponse(READER_T1_ContextHandler *pContext, READER_APDU_Response *pApduResp);


/* Manipulation des compteurs de redemande d'infos et de demandes de resynchro ... */
READER_Status READER_T1_CONTEXT_IncRepeatCounter(READER_T1_ContextHandler *pContext);
READER_Status READER_T1_CONTEXT_GetRepeatCounter(READER_T1_ContextHandler *pContext, uint32_t *pCounter);
READER_Status READER_T1_CONTEXT_SetRepeatCounter(READER_T1_ContextHandler *pContext, uint32_t counter);
READER_Status READER_T1_CONTEXT_CheckRepeatCounter(READER_T1_ContextHandler *pContext);

READER_Status READER_T1_CONTEXT_IncResynchCounter(READER_T1_ContextHandler *pContext);
READER_Status READER_T1_CONTEXT_GetResynchCounter(READER_T1_ContextHandler *pContext, uint32_t *pCounter);
READER_Status READER_T1_CONTEXT_SetResynchCounter(READER_T1_ContextHandler *pContext, uint32_t counter);
READER_Status READER_T1_CONTEXT_CheckResynchCounter(READER_T1_ContextHandler *pContext);


READER_Status READER_T1_CONTEXT_GetACKStatus(READER_T1_ContextHandler *pContext, READER_T1_ACKStatus *pACKStatus);
READER_Status READER_T1_CONTEXT_SetACKStatus(READER_T1_ContextHandler *pContext, READER_T1_ACKStatus ACKStatus);


/* Manipulation des derniers blocs envoyes/recus */
READER_Status READER_T1_CONTEXT_GetLastSent(READER_T1_ContextHandler *pContext, READER_T1_Block *pBlock);
READER_Status READER_T1_CONTEXT_GetLastIBlockSent(READER_T1_ContextHandler *pContext, READER_T1_Block *pBlock);
READER_Status READER_T1_CONTEXT_GetLastRcvd(READER_T1_ContextHandler *pContext, READER_T1_Block *pBlock);
READER_Status READER_T1_CONTEXT_GetLastIBlockRcvd(READER_T1_ContextHandler *pContext, READER_T1_Block *pBlock);

READER_Status READER_T1_CONTEXT_SetLastSent(READER_T1_ContextHandler *pContext, READER_T1_Block *pBlock);
READER_Status READER_T1_CONTEXT_SetLastIBlockSent(READER_T1_ContextHandler *pContext, READER_T1_Block *pBlock);
READER_Status READER_T1_CONTEXT_SetLastRcvd(READER_T1_ContextHandler *pContext, READER_T1_Block *pBlock);

READER_Status READER_T1_CONTEXT_GetLastSentType(READER_T1_ContextHandler *pContext, READER_T1_BlockType *pType);



/* Manipulation des numeros de sequence */
READER_Status READER_T1_CONTEXT_GetDeviceCompleteSeqNum(READER_T1_ContextHandler *pContext, uint32_t *pSeqNum);
READER_Status READER_T1_CONTEXT_SetDeviceCompleteSeqNum(READER_T1_ContextHandler *pContext, uint32_t seqNum);
READER_Status READER_T1_CONTEXT_IncDeviceCompleteSeqNum(READER_T1_ContextHandler *pContext);

READER_Status READER_T1_CONTEXT_GetCardCompleteSeqNum(READER_T1_ContextHandler *pContext, uint32_t *pSeqNum);
READER_Status READER_T1_CONTEXT_SetCardCompleteSeqNum(READER_T1_ContextHandler *pContext, uint32_t seqNum);
READER_Status READER_T1_CONTEXT_IncCardCompleteSeqNum(READER_T1_ContextHandler *pContext);

READER_Status READER_T1_CONTEXT_GetCardSeqNum(READER_T1_ContextHandler *pContext, uint32_t *pSeqNum);
READER_Status READER_T1_CONTEXT_GetDeviceSeqNum(READER_T1_ContextHandler *pContext, uint32_t *pSeqNum);


/* Gestion du chainage */
READER_Status READER_T1_CONTEXT_DeviceIsChainingLastBlock(READER_T1_ContextHandler *pContext, READER_T1_ChainingStatus *pChainingStatus);
READER_Status READER_T1_CONTEXT_CardIsChainingLastBlock(READER_T1_ContextHandler *pContext, READER_T1_ChainingStatus *pChainingStatus);
READER_Status READER_T1_CONTEXT_DeviceIsChaining(READER_T1_ContextHandler *pContext, READER_T1_ChainingStatus *pChainingStatus);
READER_Status READER_T1_CONTEXT_CardIsChaining(READER_T1_ContextHandler *pContext, READER_T1_ChainingStatus *pChainingStatus);



/* Manipuation des S-Blocks */
READER_Status READER_T1_CONTEXT_IsSblockExpectedNow(READER_T1_ContextHandler *pContext);



#endif
