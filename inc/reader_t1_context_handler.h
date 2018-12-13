#include "reader_hal.h"
#include "reader_t1.h"


#ifndef __READER_T1_CONTEXT_HANDLER_H__
#define __READER_T1_CONTEXT_HANDLER_H__




#define READER_T1_MAX_REAPEAT 0x03
#define READER_T1_MAX_RESYNCH 0x03


#define READER_T1_CONTEXT_DEFAULT_IFSC      (uint32_t)(32)        /* Voir ISO7816-3 section 11.4.2 */
#define READER_T1_CONTEXT_DEFAULT_IFSD      (uint32_t)(32)        /* Voir ISO7816-3 section 11.4.2 */
#define READER_T1_CONTEXT_DEFAULT_CWI       (uint32_t)(13)        /* Voir ISO7816-3 section 11.4.3 */
#define READER_T1_CONTEXT_DEFAULT_BWI       (uint32_t)(4)         /* Voir ISO7816-3 section 11.4.3 */
#define READER_T1_CONTEXT_DEFAULT_BWT       READER_DEFAULT_BWT
#define READER_T1_CONTEXT_DEFAULT_BGT       READER_DEFAULT_BGT
//#define READER_T1_CONTEXT_DEFAULT_CWT       READER_DEFAULT_
#define READER_T1_CONTEXT_DEFAULT_CGT       READER_DEFAULT_GT
#define READER_T1_CONTEXT_DEFAULT_CORRCODE  READER_T1_LRC         /* Voir ISO7816-3 section 11.4.4 */



/* Choix de ces valeurs en fonction de ISO7816-3 section 11.4.2 et contraintes cible dev */
#define READER_T1_MAX_IFSD_ACCEPTED 0xFE
#define READER_T1_MIN_IFSD_ACCEPTED 0x10
#define READER_T1_MAX_IFSC_ACCEPTED 0xFE
#define READER_T1_MIN_IFSC_ACCEPTED 0x10  


#define COMPUTE_READER_T1_CONTEXT_STATICBUFF_MAXSIZE(apdu_maxlength, block_data_minlength, tolerence) (((uint32_t)apdu_maxlength / (uint32_t)block_data_minlength) + 1) + (uint32_t)tolerence
#define APDU_MAXLENGTH READER_APDU_CMD_DATA_MAX_SIZE

#define READER_T1_CONTEXT_STATICBUFF_MAXSIZE (uint32_t)(COMPUTE_READER_T1_CONTEXT_STATICBUFF_MAXSIZE(APDU_MAXLENGTH, READER_T1_MIN_IFSC_ACCEPTED, 20))




typedef struct READER_T1_BlockBuffer READER_T1_BlockBuffer;
struct READER_T1_BlockBuffer{
	READER_T1_Block blockBuff[READER_T1_CONTEXT_STATICBUFF_MAXSIZE];
	uint32_t indexBottom;
	uint32_t indexTop;
	uint32_t length;
};


typedef struct READER_T1_ReceptionBuff READER_T1_ReceptionBuff;
struct READER_T1_ReceptionBuff{
	uint8_t rawReceptionBuff[READER_APDU_RESP_MAX_TOTALSIZE];
	uint32_t dataSize;
};



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


typedef enum READER_T1_BlockExistence READER_T1_BlockExistence;
enum READER_T1_BlockExistence{
	READER_T1_BLOCK_EXISTS_YES            =       (uint32_t)(0x00000001),
	READER_T1_BLOCK_EXISTS_NO             =       (uint32_t)(0x00000000)
};


typedef enum READER_T1_FlagStatus READER_T1_FlagStatus;
enum READER_T1_FlagStatus{
	READER_T1_FLAGSTATUS_SET              =       (uint32_t)(0x00000001),
	READER_T1_FLAGSTATUS_RESET            =       (uint32_t)(0x00000000)
};


typedef struct READER_T1_ContextHandler READER_T1_ContextHandler;
struct READER_T1_ContextHandler{
	READER_T1_BlockBuffer blockBuff;
	READER_T1_ReceptionBuff receptionBuff;
	
	
	uint32_t currentIFSC;
	uint32_t currentIFSD;
	uint32_t currentCWI;
	uint32_t currentBWI;
	uint32_t currentBWT;
	uint32_t currentBGT;
	uint32_t currentCWT;
	uint32_t currentCGT;
	READER_T1_RedundancyType currentRType;
	
	/* Error Handling */
	uint32_t resynchCounter;
	uint32_t repeatCounter;
	
	READER_T1_ACKStatus ACKStatus;
	
	READER_T1_Block lastSent;
	READER_T1_Block lastIBlockSent;
	READER_T1_Block lastRcvd;
	READER_T1_Block lastIBlockRcvd;
	
	READER_T1_BlockExistence lastSentExistenceFlag;
	READER_T1_BlockExistence lastIBlockSentExistenceFlag;
	READER_T1_BlockExistence lastRcvdExistenceFlag;
	READER_T1_BlockExistence lastIBlockRcvdExistenceFlag;
	
	READER_T1_ChainingStatus cardIsChainingLastBlock;
	READER_T1_ChainingStatus deviceIsChainingLastBlock;
	READER_T1_ChainingStatus cardIsChaining;
	READER_T1_ChainingStatus deviceIsChaining;
	
	uint32_t deviceCompleteSeqNum;      /* Numero de sequence attendu pour le prochain I-Block */
	uint32_t cardCompleteSeqNum;        /* Numero de sequence attendu pour le prochain I-Block */
	
	
	READER_T1_SBlockExpected SBlockExpected;
	READER_T1_SBlockType SBlockExpectedType;
	uint32_t SBlockRequCounter;
	
	uint32_t tickLastBlock;
	READER_T1_FlagStatus tickLastBlockFlag;
	
};

/* Initialisation de la structure */
READER_Status READER_T1_CONTEXT_Init(READER_T1_ContextHandler *pContext);
READER_Status READER_T1_CONTEXT_InitSettings(READER_T1_ContextHandler *pContext);
READER_Status READER_T1_CONTEXT_InitCommSettings(READER_T1_ContextHandler *pContext);
READER_Status READER_T1_CONTEXT_InitContextSettings(READER_T1_ContextHandler *pContext);
READER_Status READER_T1_CONTEXT_InitBuffer(READER_T1_ContextHandler *pContext);
READER_Status READER_T1_CONTEXT_InitRcptBuff(READER_T1_ContextHandler *pContext);


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
READER_Status READER_T1_CONTEXT_GetCurrentRedundancyLen(READER_T1_ContextHandler *pContext, uint32_t *pRedLen);

READER_Status READER_T1_CONTEXT_SetCurrentBGT(READER_T1_ContextHandler *pContext, uint32_t bgt);
READER_Status READER_T1_CONTEXT_SetCurrentBWT(READER_T1_ContextHandler *pContext, uint32_t bwt);
READER_Status READER_T1_CONTEXT_SetCurrentCGT(READER_T1_ContextHandler *pContext, uint32_t cgt);
READER_Status READER_T1_CONTEXT_SetCurrentCWT(READER_T1_ContextHandler *pContext, uint32_t cwt);
READER_Status READER_T1_CONTEXT_SetCurrentCWI(READER_T1_ContextHandler *pContext, uint32_t cwi);
READER_Status READER_T1_CONTEXT_SetCurrentBWI(READER_T1_ContextHandler *pContext, uint32_t bwi);
READER_Status READER_T1_CONTEXT_SetCurrentIFSC(READER_T1_ContextHandler *pContext, uint32_t ifsc);
READER_Status READER_T1_CONTEXT_SetCurrentIFSD(READER_T1_ContextHandler *pContext, uint32_t ifsd);
READER_Status READER_T1_CONTEXT_SetCurrentRedundancyType(READER_T1_ContextHandler *pContext, READER_T1_RedundancyType rType);


/* Manipulation des compteurs de redemande d'infos et de demandes de resynchro ... */
READER_Status READER_T1_CONTEXT_GetRepeatCounter(READER_T1_ContextHandler *pContext, uint32_t *pCounter);
READER_Status READER_T1_CONTEXT_SetRepeatCounter(READER_T1_ContextHandler *pContext, uint32_t counter);

READER_Status READER_T1_CONTEXT_GetResynchCounter(READER_T1_ContextHandler *pContext, uint32_t *pCounter);
READER_Status READER_T1_CONTEXT_SetResynchCounter(READER_T1_ContextHandler *pContext, uint32_t counter);


READER_Status READER_T1_CONTEXT_GetACKStatus(READER_T1_ContextHandler *pContext, READER_T1_ACKStatus *pACKStatus);
READER_Status READER_T1_CONTEXT_SetACKStatus(READER_T1_ContextHandler *pContext, READER_T1_ACKStatus ACKStatus);


/* Manipulation des derniers blocs envoyes/recus */

READER_Status READER_T1_CONTEXT_SetTickLastBlock(READER_T1_ContextHandler *pContext, uint32_t tickLastBlock);
READER_Status READER_T1_CONTEXT_GetTickLastBlock(READER_T1_ContextHandler *pContext, uint32_t *tickValue);

READER_Status READER_T1_CONTEXT_GetLastSent(READER_T1_ContextHandler *pContext, READER_T1_Block **ppBlockDest);
READER_Status READER_T1_CONTEXT_GetLastIBlockSent(READER_T1_ContextHandler *pContext, READER_T1_Block **ppBlockDest);
READER_Status READER_T1_CONTEXT_GetLastIBlockSentSeqNum(READER_T1_ContextHandler *pContext, uint32_t *pSeqNum);
READER_Status READER_T1_CONTEXT_GetLastRcvd(READER_T1_ContextHandler *pContext, READER_T1_Block **ppBlockDest);
READER_Status READER_T1_CONTEXT_GetLastIBlockRcvd(READER_T1_ContextHandler *pContext, READER_T1_Block **ppBlockDest);
READER_Status READER_T1_CONTEXT_GetLastIBlockRcvdSeqSum(READER_T1_ContextHandler *pContext, uint32_t *pSeqNum);

READER_Status READER_T1_CONTEXT_SetLastSent(READER_T1_ContextHandler *pContext, READER_T1_Block *pBlock);
READER_Status READER_T1_CONTEXT_SetLastIBlockSent(READER_T1_ContextHandler *pContext, READER_T1_Block *pBlock);
READER_Status READER_T1_CONTEXT_SetLastRcvd(READER_T1_ContextHandler *pContext, READER_T1_Block *pBlock);
READER_Status READER_T1_CONTEXT_SetLastIBlockRcvd(READER_T1_ContextHandler *pContext, READER_T1_Block *pBlock);

READER_Status READER_T1_CONTEXT_GetLastSentType(READER_T1_ContextHandler *pContext, READER_T1_BlockType *pType);
READER_Status READER_T1_CONTEXT_GetLastRcvdType(READER_T1_ContextHandler *pContext, READER_T1_BlockType *pType);

READER_Status READER_T1_CONTEXT_LastSentExists(READER_T1_ContextHandler *pContext);
READER_Status READER_T1_CONTEXT_LastIBlockSentExists(READER_T1_ContextHandler *pContext);
READER_Status READER_T1_CONTEXT_LastRcvdExists(READER_T1_ContextHandler *pContext);
READER_Status READER_T1_CONTEXT_LastIBlockRcvdExists(READER_T1_ContextHandler *pContext);


/* Manipulation des numeros de sequence */
READER_Status READER_T1_CONTEXT_GetDeviceCompleteSeqNum(READER_T1_ContextHandler *pContext, uint32_t *pSeqNum);
READER_Status READER_T1_CONTEXT_SetDeviceCompleteSeqNum(READER_T1_ContextHandler *pContext, uint32_t seqNum);
READER_Status READER_T1_CONTEXT_IncDeviceCompleteSeqNum(READER_T1_ContextHandler *pContext);

READER_Status READER_T1_CONTEXT_GetCardCompleteSeqNum(READER_T1_ContextHandler *pContext, uint32_t *pSeqNum);
READER_Status READER_T1_CONTEXT_SetCardCompleteSeqNum(READER_T1_ContextHandler *pContext, uint32_t seqNum);
READER_Status READER_T1_CONTEXT_IncCardCompleteSeqNum(READER_T1_ContextHandler *pContext);

READER_Status READER_T1_CONTEXT_GetCardSeqNum(READER_T1_ContextHandler *pContext, uint32_t *pSeqNum);
READER_Status READER_T1_CONTEXT_GetDeviceSeqNum(READER_T1_ContextHandler *pContext, uint32_t *pSeqNum);

READER_Status READER_T1_CONTEXT_ComputeNextDeviceSeqNum(READER_T1_ContextHandler *pContext, uint32_t *pSeqNum);
READER_Status READER_T1_CONTEXT_ComputeNextCardSeqNum(READER_T1_ContextHandler *pContext, uint32_t *pSeqNum);

READER_Status READER_T1_CONTEXT_GetCardExpectedSeqNum(READER_T1_ContextHandler *pContext, uint32_t *pSeqNum);


/* Gestion du chainage */
READER_Status READER_T1_CONTEXT_DeviceIsChainingLastBlock(READER_T1_ContextHandler *pContext, READER_T1_ChainingStatus *pChainingStatus);
READER_Status READER_T1_CONTEXT_CardIsChainingLastBlock(READER_T1_ContextHandler *pContext, READER_T1_ChainingStatus *pChainingStatus);
READER_Status READER_T1_CONTEXT_DeviceIsChaining(READER_T1_ContextHandler *pContext, READER_T1_ChainingStatus *pChainingStatus);
READER_Status READER_T1_CONTEXT_CardIsChaining(READER_T1_ContextHandler *pContext, READER_T1_ChainingStatus *pChainingStatus);

READER_Status READER_T1_CONTEXT_SetDeviceChainingSituationFlag(READER_T1_ContextHandler *pContext, READER_T1_ChainingStatus chainingStatus);
READER_Status READER_T1_CONTEXT_SetDeviceChainingLastBlockFlag(READER_T1_ContextHandler *pContext, READER_T1_ChainingStatus chainingStatus);
READER_Status READER_T1_CONTEXT_SetCardChainingSituationFlag(READER_T1_ContextHandler *pContext, READER_T1_ChainingStatus chainingStatus);
READER_Status READER_T1_CONTEXT_SetCardChainingLastBlockFlag(READER_T1_ContextHandler *pContext, READER_T1_ChainingStatus chainingStatus);


/* Manipuation des S-Blocks */
READER_Status READER_T1_CONTEXT_IsSblockExpectedNow(READER_T1_ContextHandler *pContext, READER_T1_SBlockExpected *pExp);
READER_Status READER_T1_CONTEXT_GetSBlockExpectedType(READER_T1_ContextHandler *pContext, READER_T1_SBlockType *pType);
READER_Status READER_T1_CONTEXT_SetSBlockExpected(READER_T1_ContextHandler *pContext, READER_T1_SBlockType type);
READER_Status READER_T1_CONTEXT_SetNoSBlockExpected(READER_T1_ContextHandler *pContext);

READER_Status READER_T1_CONTEXT_GetSBlockRequCounter(READER_T1_ContextHandler *pContext, uint32_t *pCounter);
READER_Status READER_T1_CONTEXT_SetSBlockRequCounter(READER_T1_ContextHandler *pContext, uint32_t counter);
READER_Status READER_T1_CONTEXT_IncSBlockRequCounter(READER_T1_ContextHandler *pContext);


READER_Status READER_T1_CONTEXT_GetBlockBuff(READER_T1_ContextHandler *pContext, READER_T1_BlockBuffer **ppBlockBuff);

#endif
