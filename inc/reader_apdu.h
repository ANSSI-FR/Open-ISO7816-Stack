#ifndef __READER_APDU_H__
#define __READER_APDU_H__

#include "reader_hal.h"
#include "reader_tpdu.h"





typedef struct READER_APDU_Body READER_APDU_Body;
struct READER_APDU_Body{
	uint16_t Nc;
	uint16_t Ne;
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









#endif
