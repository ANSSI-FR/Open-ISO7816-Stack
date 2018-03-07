#ifndef __READER_TPDU_H__
#define __READER_TPDU_H__


#include "reader.h"
#include <stdint.h>



#define READER_TPU_MAX_DATA     0xFF;




typedef struct READER_TPDU_Tpdu READER_TPDU_Tpdu;
struct READER_TPDU_Tpdu{
	uint8_t CLA;
	uint8_t INS;
	uint8_t P1;
	uint8_t P2;
	uint8_t P3;
	
	uint8_t dataSize;
	uint8_t dataField[READER_TPU_MAX_DATA];
};



#endif
