/**
 * \file reader_atr.h
 * \author Boris
 * This file contains the prototypes of the functions to deal with ATR.
 */
 
 
#ifndef __READER_ATR_H__
#define __READER_ATR_H__


#include <stdint.h>
//#include "reader_hal.h"
//#include "reader_t1.h"
#include "reader.h"
#include "reader_hal_comm_settings.h"




#define READER_ATR_MAX_HIST_BYTES        15
#define READER_ATR_MAX_SPECIFIC_BYTES    8
#define READER_ATR_VALUE_NOT_INDICATED   0
#define READER_ATR_MAX_SIZE              32
#define READER_ATR_INDICATED             1
#define READER_ATR_NOT_INDICATED         0

#define READER_ATR_DEFAULT_TIMEOUT       1000



typedef enum READER_ATR_ClockStopIndicator READER_ATR_ClockStopIndicator;
enum READER_ATR_ClockStopIndicator{
	READER_ATR_CLOCKSTOP_NOTSUPPORTED    =   (uint32_t)(0x00000000),
	READER_ATR_CLOCKSTOP_STATE_L         =   (uint32_t)(0x00000001),
	READER_ATR_CLOCKSTOP_STATE_H         =   (uint32_t)(0x00000002),
	READER_ATR_CLOCKSTOP_NOPREF          =   (uint32_t)(0x00000003),
	READER_ATR_CLOCKSTOP_NOTINDICATED    =   (uint32_t)(0x00000004)
};



typedef enum READER_ATR_ClassIndicator READER_ATR_ClassIndicator;
enum READER_ATR_ClassIndicator{
	READER_ATR_CLASS_A_ONLY              =   (uint32_t)(0x00000001),
	READER_ATR_CLASS_B_ONLY              =   (uint32_t)(0x00000002),
	READER_ATR_CLASS_C_ONLY              =   (uint32_t)(0x00000004),
	READER_ATR_CLASS_AB_ONLY             =   (uint32_t)(0x00000003),
	READER_ATR_CLASS_BC_ONLY             =   (uint32_t)(0x00000006),
	READER_ATR_CLASS_ABC                 =   (uint32_t)(0x00000007),
	READER_ATR_CLASS_NOTINDICATED        =   (uint32_t)(0x00000000)
};


typedef enum READER_ATR_UseOfSPU READER_ATR_UseOfSPU;
enum READER_ATR_UseOfSPU{
	READER_ATR_SPU_STANDARD              =   (uint32_t)(0x00000002),
	READER_ATR_SPU_PROPRIETARY           =   (uint32_t)(0x00000001),
	READER_ATR_SPU_NOTUSED               =   (uint32_t)(0x00000000),
	READER_ATR_SPU_NOTINDICATED          =   (uint32_t)(0x00000003)
};


typedef enum READER_ATR_EncodingConv READER_ATR_EncodingConv;
enum READER_ATR_EncodingConv{
	READER_ATR_ENCODING_DIRECT           =   (uint32_t)(0x0000003B),
	READER_ATR_ENCODING_REVERSE          =   (uint32_t)(0x00000003)
};




typedef struct READER_ATR_ProtocolSpecificBytes READER_ATR_ProtocolSpecificBytes;
struct READER_ATR_ProtocolSpecificBytes{
	uint8_t TABytesCount;
	uint8_t TBBytesCount;
	uint8_t TCBytesCount;
	
	uint8_t TABytes[READER_ATR_MAX_SPECIFIC_BYTES];
	uint8_t TBBytes[READER_ATR_MAX_SPECIFIC_BYTES];
	uint8_t TCBytes[READER_ATR_MAX_SPECIFIC_BYTES];
};


typedef struct READER_ATR_Atr READER_ATR_Atr;
struct READER_ATR_Atr{
	uint32_t Fi;
	uint32_t Di;
	uint32_t fMax;
	uint8_t N;
	READER_ATR_ClockStopIndicator clockStopIndicator;
	READER_ATR_ClassIndicator classIndicator;
	READER_ATR_UseOfSPU useOfSPU;
	READER_ATR_EncodingConv encodingConv;
	READER_ATR_ProtocolSpecificBytes T0Protocol;
	READER_ATR_ProtocolSpecificBytes T1Protocol;
	uint32_t K;
	uint8_t histBytes[READER_ATR_MAX_HIST_BYTES];
	uint32_t isT0Indicated;
	uint32_t isT1Indicated;
	uint32_t isT15Indicated;
};




READER_Status READER_ATR_IsInterfacesBytesToRead(uint8_t Y);
READER_Status READER_ATR_IsTAToRead(uint8_t Y);
READER_Status READER_ATR_IsTBToRead(uint8_t Y);
READER_Status READER_ATR_IsTCToRead(uint8_t Y);
READER_Status READER_ATR_IsTDToRead(uint8_t Y);

READER_Status READER_ATR_ProcessTA(READER_ATR_Atr *atr, uint8_t TA, uint32_t i, uint8_t T);
READER_Status READER_ATR_ProcessTB(READER_ATR_Atr *atr, uint8_t TB, uint32_t i, uint8_t T);
READER_Status READER_ATR_ProcessTC(READER_ATR_Atr *atr, uint8_t TC, uint32_t i, uint8_t T);
READER_Status READER_ATR_ProcessT(READER_ATR_Atr *atr, uint8_t T);

READER_Status READER_ATR_IsT0(READER_ATR_Atr *atr);
READER_Status READER_ATR_IsT1(READER_ATR_Atr *atr);
READER_Status READER_ATR_IsT15(READER_ATR_Atr *atr);

READER_Status READER_ATR_CheckTCK(uint8_t *rcvdBytesList, uint32_t rcvdBytesCount, uint8_t TCK);
READER_Status READER_ATR_AddRcvdByte(uint8_t byte, uint8_t *byteList, uint32_t *byteCount);

uint8_t READER_ATR_ComputeY(uint8_t TD);
uint8_t READER_ATR_ComputeT(uint8_t TD);
uint8_t READER_ATR_ComputeK(uint8_t T0);
uint32_t READER_ATR_ComputeFi(uint8_t TA1);
uint32_t READER_ATR_ComputeFMax(uint8_t TA1);
uint32_t READER_ATR_ComputeDi(uint8_t TA1);
READER_ATR_ClockStopIndicator READER_ATR_GetClockStopIndic(uint8_t TA15);
READER_ATR_ClassIndicator READER_ATR_GetClassIndic(uint8_t TA15);
READER_ATR_UseOfSPU READER_ATR_GetUseSPU(uint8_t TB15);
READER_ATR_EncodingConv READER_ATR_GetEncoding(uint8_t TS);
uint32_t READER_ATR_GetBWI(READER_ATR_Atr *pAtr);
uint32_t READER_ATR_GetRedundancyType(READER_ATR_Atr *pAtr);
uint32_t READER_ATR_GetIFSC(READER_ATR_Atr *pAtr);
uint32_t READER_ATR_GetWI(READER_ATR_Atr *pAtr);

READER_Status READER_ATR_Receive(READER_ATR_Atr *atr, READER_HAL_CommSettings *pSettings);
READER_Status READER_ATR_InitStruct(READER_ATR_Atr *atr);
READER_Status READER_ATR_ApplySettings(READER_ATR_Atr *atr);

void READER_ATR_ErrHandler(void);

READER_Status READER_ATR_CheckTS(uint8_t TS);

#endif
