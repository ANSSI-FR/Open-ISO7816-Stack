#ifndef __READER_TPDU_H__
#define __READER_TPDU_H__


#include "reader.h"
#include "reader_hal.h"
#include <stdint.h>



#define READER_TPU_MAX_DATA     0xFF;



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
	uint8_t size;
	uint8_t data[READER_TPU_MAX_DATA];
};


typedef struct READER_TPDU_Tpdu READER_TPDU_Command;
struct READER_TPDU_Command{
	READER_TPDU_Header header;
	READER_TPDU_DataField dataField;
};



READER_Status READER_TPDU_Send(READER_TPDU_Command *tpdu);
READER_Status READER_TPDU_SendHeader(READER_TPDU_Header *tpduHeader);
READER_Status READER_TPDU_SendDataOneshot(READER_TPDU_DataField *tpduDataField);
READER_Status READER_TPDU_SendDataSliced(READER_TPDU_DataField *tpduDataField);

READER_Status READER_TPDU_WaitProcedureByte(uint8_t *procedureByte, uint32_t timeout);
READER_Status READER_TPDU_IsACK(uint8_t byte, uint8_t INS);
READER_Status READER_TPDU_IsXoredACK(uint8_t byte, uint8_t INS);
READER_Status READER_TPDU_IsNull(uint8_t byte);
READER_Status READER_TPDU_IsSW1(uint8_t byte);



#endif
