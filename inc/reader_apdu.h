#ifndef __READER_APDU_H__
#define __READER_APDU_H__

#include "reader_hal.h"
#include "reader_tpdu.h"



typedef enum READER_APDU_ProtocolCase READER_APDU_ProtocolCase;
enum READER_APDU_ProtocolCase{
	READER_APDU_CASE_1  = (uint32_t)(0x00000001),
	READER_APDU_CASE_2S = (uint32_t)(0x00000002),
	READER_APDU_CASE_2E = (uint32_t)(0x00000003),
	READER_APDU_CASE_3S = (uint32_t)(0x00000004),
	READER_APDU_CASE_3E = (uint32_t)(0x00000005),
	READER_APDU_CASE_4S = (uint32_t)(0x00000006),
	READER_APDU_CASE_4E = (uint32_t)(0x00000007)
}



typedef struct READER_APDU_Body READER_APDU_Body;
struct READER_APDU_Body{
	uint32_t Nc;
	uint32_t Ne;
	uint8_t *pDataField;
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



READER_APDU_ProtocolCase READER_APDU_GetProtocolCase(READER_APDU_Command *apdu);
READER_Status READER_APDU_Send(READER_APDU_Command *apdu);

READER_Status READER_APDU_SendCase1(READER_APDU_Command *apdu);
READER_Status READER_APDU_SendCase2S(READER_APDU_Command *apdu);
READER_Status READER_APDU_SendCase2E(READER_APDU_Command *apdu);
READER_Status READER_APDU_SendCase3S(READER_APDU_Command *apdu);
READER_Status READER_APDU_SendCase3E(READER_APDU_Command *apdu);
READER_Status READER_APDU_SendCase4S(READER_APDU_Command *apdu);
READER_Status READER_APDU_SendCase4E(READER_APDU_Command *apdu);

READER_Status READER_APDU_RcvSW(uint16_t *SW, uint32_t timeout);
READER_Status READER_APDU_RcvResponse(uint8_t *buffer, uint32_t Ne, uint16_t *SW, uint32_t timeout);

READER_Status READER_APDU_Forge(READER_APDU_Command *apdu, uint8_t CLA, uint8_t INS, uint8_t P1, uint8_t P2, uint32_t Nc, uint8_t *pData, uint32_t Ne);


uint16_t READER_APDU_NcToLc(uint16_t Nc);
uint16_t READER_APDU_NeToLe(uint16_t Ne);
uint16_t READER_APDU_LcToNc(uint16_t Lc);
uint16_t READER_APDU_LeToNe(uint16_t Le);





#endif
