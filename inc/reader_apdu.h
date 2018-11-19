#ifndef __READER_APDU_H__
#define __READER_APDU_H__

#include "reader_hal.h"
#include "reader_tpdu.h"



#define READER_APDU_INS_ENVELOPE        (uint8_t)(0xC2)
#define READER_APDU_INS_GETRESPONSE     (uint8_t)(0xC0)

#define READER_APDU_CMD_MAX_SIZE         400                        /* La norme ISO fixe un maximum de 65535 caracteres */
#define READER_APDU_RESP_MAX_SIZE        400                        /* La norme ISO fixe un maximum de 65536 caracteres */


typedef enum READER_APDU_ProtocolCase READER_APDU_ProtocolCase;
enum READER_APDU_ProtocolCase{
	READER_APDU_CASE_ERR       =   (uint32_t)(0x00000000),
	READER_APDU_CASE_1         =   (uint32_t)(0x00000001),
	READER_APDU_CASE_2S        =   (uint32_t)(0x00000002),
	READER_APDU_CASE_2E        =   (uint32_t)(0x00000003),
	READER_APDU_CASE_3S        =   (uint32_t)(0x00000004),
	READER_APDU_CASE_3E        =   (uint32_t)(0x00000005),
	READER_APDU_CASE_4S        =   (uint32_t)(0x00000006),
	READER_APDU_CASE_4E        =   (uint32_t)(0x00000007)
};



typedef struct READER_APDU_Body READER_APDU_Body;
struct READER_APDU_Body{
	uint32_t Nc;
	uint32_t Ne;
	uint8_t dataBytes[READER_APDU_CMD_MAX_SIZE];  /* Taille max du data field d'une commande APDU */
};


typedef struct READER_APDU_Header READER_APDU_Header;
struct READER_APDU_Header{
	uint8_t CLA;
	uint8_t INS;
	uint8_t P1;
	uint8_t P2;
};


typedef struct READER_APDU_Command READER_APDU_Command;
struct READER_APDU_Command{
	READER_APDU_Header header;
	READER_APDU_Body body;
};


typedef struct READER_APDU_Response READER_APDU_Response;
struct READER_APDU_Response{
	uint8_t dataBytes[READER_APDU_RESP_MAX_SIZE];   /* Taille max du data field d'une APDU Response. */
	uint32_t dataSize;
	uint8_t SW1;
	uint8_t SW2;
};


READER_APDU_ProtocolCase READER_APDU_GetProtocolCase(READER_APDU_Command *pApduCmd);
READER_Status READER_APDU_Execute(READER_APDU_Command *pApduCmd, READER_APDU_Response *pApduResp, uint32_t timeout);

READER_Status READER_APDU_ExecuteCase1(READER_APDU_Command *pApduCmd, READER_APDU_Response *pApduResp, uint32_t timeout);
READER_Status READER_APDU_ExecuteCase2S(READER_APDU_Command *pApduCmd, READER_APDU_Response *pApduResp, uint32_t timeout);
READER_Status READER_APDU_ExecuteCase2E(READER_APDU_Command *pApduCmd, READER_APDU_Response *pApduResp, uint32_t timeout);
READER_Status READER_APDU_ExecuteCase3S(READER_APDU_Command *pApduCmd, READER_APDU_Response *pApduResp, uint32_t timeout);
READER_Status READER_APDU_ExecuteCase3E(READER_APDU_Command *pApduCmd, READER_APDU_Response *pApduResp, uint32_t timeout);
READER_Status READER_APDU_ExecuteCase4S(READER_APDU_Command *pApduCmd, READER_APDU_Response *pApduResp, uint32_t timeout);
READER_Status READER_APDU_ExecuteCase4E(READER_APDU_Command *pApduCmd, READER_APDU_Response *pApduResp, uint32_t timeout);

READER_Status READER_APDU_RcvSW(uint16_t *SW, uint32_t timeout);
READER_Status READER_APDU_RcvResponse(uint8_t *buffer, uint32_t Ne, uint16_t *SW, uint32_t timeout);

READER_Status READER_APDU_Forge(READER_APDU_Command *pApduCmd, uint8_t CLA, uint8_t INS, uint8_t P1, uint8_t P2, uint32_t Nc, uint8_t *pData, uint32_t Ne);
READER_Status READER_APDU_CopyCommand(READER_APDU_Command *pSourceApdu, READER_APDU_Command *pDestApdu);
READER_Status READER_APDU_CopyResponse(READER_APDU_Response *pSourceApdu, READER_APDU_Response *pDestApdu);


uint16_t READER_APDU_NcToLc(uint16_t Nc);
uint16_t READER_APDU_NeToLe(uint16_t Ne);
uint16_t READER_APDU_LcToNc(uint16_t Lc);
uint16_t READER_APDU_LeToNe(uint16_t Le);

READER_Status READER_APDU_MapTpduRespToApdu(READER_TPDU_Response *pTpduResp, READER_APDU_Response *pApduResp);





#endif
