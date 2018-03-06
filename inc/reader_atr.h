#include <stdint.h>
#include "reader_hal.h"


#define READER_ATR_MAX_HIST_BYTES        15
#define READER_ATR_MAX_SPECIFIC_BYTES    8


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
};




READER_Status READER_ATR_IsInterfacesBytesToRead(uint8_t Y);
READER_Status READER_ATR_IsTAToRead(uint8_t Y);
READER_Status READER_ATR_IsTBToRead(uint8_t Y);
READER_Status READER_ATR_IsTCToRead(uint8_t Y);
READER_Status READER_ATR_IsTDToRead(uint8_t Y);

READER_Status READER_ATR_ProcessTA(READER_ATR_Atr *atr, uint8_t TA, uint32_t i, uint8_t T);
READER_Status READER_ATR_ProcessTB(READER_ATR_Atr *atr, uint8_t TB, uint32_t i, uint8_t T);
READER_Status READER_ATR_ProcessTC(READER_ATR_Atr *atr, uint8_t TC, uint32_t i, uint8_t T);

uint8_t READER_ATR_GetY(uint8_t TD);
uint8_t READER_ATR_GetT(uint8_t TD);
uint8_t READER_ATR_GetK(uint8_t T0);
uint32_t READER_ATR_GetFi(uint8_t TA1);
uint32_t READER_ATR_GetFMax(uint8_t TA1);
uint32_t READER_ATR_GetDi(uint8_t TA1);
READER_ATR_ClockStopIndicator READER_ATR_GetClockStopIndic(uint8_t TA15);
READER_ATR_ClassIndicator READER_ATR_GetClassIndic(uint8_t TA15);
READER_ATR_UseOfSPU READER_ATR_GetUseSPU(uint8_t TB15);
READER_ATR_EncodingConv READER_ATR_GetEncoding(uint8_t TS);

READER_Status READER_ATR_Receive(READER_ATR_Atr *atr);

void READER_ATR_ErrHandler(void);

READER_Status READER_ATR_CheckTS(uint8_t TS);
