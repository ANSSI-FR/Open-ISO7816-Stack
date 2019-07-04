#include "reader.h"
#include "reader_hal.h"
#include <stdint.h>


#include "reader_hal_comm_settings.h"



#ifndef __READER_TPDU_H__
#define __READER_TPDU_H__



#ifdef TEST
	#define READER_TPDU_MAX_DATA            (uint32_t)(256)
#else
	#define READER_TPDU_MAX_DATA            (uint32_t)(256)             /* Valeur MAX = 256, voir spec */
#endif

#define READER_TPDU_HEADER_SIZE         (uint32_t)(0x00000005)
#define READER_TPDU_DUMMY_FALSE_INS     (uint32_t)(0x00000061)
#define READER_TPDU_ACK_NORMAL          (uint32_t)(0x00000001)
#define READER_TPDU_ACK_XORED           (uint32_t)(0x00000002)





typedef struct READER_TPDU_Header READER_TPDU_Header;
struct READER_TPDU_Header{
	uint8_t CLA;
	uint8_t INS;
	uint8_t P1;
	uint8_t P2;
	uint8_t P3;
};


typedef struct READER_TPDU_DataField READER_TPDU_DataField;
struct READER_TPDU_DataField{
	uint32_t size;
	uint8_t data[READER_TPDU_MAX_DATA];
};


typedef struct READER_TPDU_Command READER_TPDU_Command;
struct READER_TPDU_Command{
	READER_TPDU_Header headerField;
	READER_TPDU_DataField dataField;
};


/**
 * \struct struct READER_TPDU_Response
 * \brief Cette structure permet de stocker les informations relatives à un "TPDU Response"
 */
typedef struct READER_TPDU_Response READER_TPDU_Response;
struct READER_TPDU_Response{
	uint8_t SW1;                                                                  /* !< Stocke la valeur de la première partie du Status Word (SW1) */   
	uint8_t SW2;                                                                  /* !< Stocke la valeur de la deuxième partie du Status Word (SW2) */ 
	uint8_t dataBytes[READER_TPDU_MAX_DATA];  /* 256 est la taille max d'une reponse TPDU */       /* !< Tabelau d'octets permettant de stocker les caractères reçus. Un "TPDU Response" fait au plus 256 caractères. */    
	uint32_t dataSize;                                                            /* !< Stocke la quantité de données reçues. */
};


READER_Status READER_TPDU_Execute(READER_TPDU_Command *pTpdu, READER_TPDU_Response *pResp, uint32_t timeout, READER_HAL_CommSettings *pSettings);

READER_Status READER_TPDU_Send(READER_TPDU_Command *tpdu, uint32_t timeout, READER_HAL_CommSettings *pSettings);
READER_Status READER_TPDU_SendHeader(READER_TPDU_Command *tpdu, uint32_t timeout, READER_HAL_CommSettings *pSettings);
READER_Status READER_TPDU_SendDataOneshot(READER_TPDU_Command *tpdu, uint32_t timeout, READER_HAL_CommSettings *pSettings);
READER_Status READER_TPDU_SendDataSliced(READER_TPDU_Command *tpdu, uint32_t timeout, READER_HAL_CommSettings *pSettings);

READER_Status READER_TPDU_RcvSW(uint8_t *SW1, uint8_t *SW2, uint32_t timeout, READER_HAL_CommSettings *pSettings);
READER_Status READER_TPDU_RcvDataField(uint8_t *buffer, uint32_t Ne, uint32_t timeout, READER_HAL_CommSettings *pSettings);
READER_Status READER_TPDU_RcvResponse(READER_TPDU_Response *pResp, uint32_t expectedDataSize, uint32_t timeout, READER_HAL_CommSettings *pSettings);

READER_Status READER_TPDU_WaitProcedureByte(uint8_t *procedureByte, uint8_t INS, uint32_t timeout, READER_HAL_CommSettings *pSettings);
READER_Status READER_TPDU_WaitACK(uint8_t INS, uint8_t *ACKType, uint32_t timeout, READER_HAL_CommSettings *pSettings);

READER_Status READER_TPDU_IsACK(uint8_t byte, uint8_t INS);
READER_Status READER_TPDU_IsXoredACK(uint8_t byte, uint8_t INS);
READER_Status READER_TPDU_IsNullByte(uint8_t byte);
READER_Status READER_TPDU_IsSW1(uint8_t byte);
READER_Status READER_TPDU_IsProcedureByte(uint8_t byte, uint8_t INS);

READER_Status READER_TPDU_Forge(READER_TPDU_Command *tpdu, uint8_t CLA, uint8_t INS, uint8_t P1, uint8_t P2, uint8_t P3, uint8_t *dataBuff, uint32_t dataSize);



#endif
