#include <stdint.h>
#include "reader_hal.h"


#define READER_ATR_MAX_HIST_BYTES 15;


typedef enum READER_ATR_ClockStopIndicator READER_ATR_ClockStopIndicator:
enum READER_ATR_ClockStopIndicator{
	READER_ATR_CLOCKSTOP_NOTSUPPORTED    =   (uint32_t)(0x00000000),
	READER_ATR_CLOCKSTOP_STATE_L         =   (uint32_t)(0x00000001),
	READER_ATR_CLOCKSTOP_STATE_H         =   (uint32_t)(0x00000002),
	READER_ATR_CLOCKSTOP_NOPREF          =   (uint32_t)(0x00000003)
};



typedef enum READER_ATR_ClassIndicator READER_ATR_ClassIndicator;
READER_ATR_ClassIndicator{
	READER_ATR_CLASS_A_ONLY              =   (uint32_t)(0x00000001),
	READER_ATR_CLASS_B_ONLY              =   (uint32_t)(0x00000002),
	READER_ATR_CLASS_C_ONLY              =   (uint32_t)(0x00000004),
	READER_ATR_CLASS_AB_ONLY             =   (uint32_t)(0x00000003),
	READER_ATR_CLASS_BC_ONLY             =   (uint32_t)(0x00000006),
	READER_ATR_CLASS_ABC                 =   (uint32_t)(0x00000007)
};


typedef enum READER_ATR_UseOfSPU READER_ATR_UseOfSPU;
enum READER_ATR_UseOfSPU{
	READER_ATR_SPU_STANDARD              =   (uint32_t)(0x00000002),
	READER_ATR_SPU_PROPRIETARY           =   (uint32_t)(0x00000001),
	READER_ATR_SPU_NOTUSED               =   (uint32_t)(0x00000000)
};


typedef struct READER_ATR_Atr READER_ATR_Atr;
struct READER_ATR_Atr{
	uint32_t Fi;
	uint32_t Di;
	uint32_t fMax;
	READER_ATR_ClockStopIndicator clockStopIndicator;
	READER_ATR_ClassIndicator classIndicator;
	READER_ATR_UseOfSPU useOfSPU;
	uint32_t K;
	uint8_t histBytes[READER_ATR_MAX_HIST_BYTES];
};




READER_Status READER_ATR_IsInterfacesBytesToRead(uint8_t Y);
READER_Status READER_ATR_IsTAToRead(uint8_t Y);
READER_Status READER_ATR_IsTBToRead(uint8_t Y);
READER_Status READER_ATR_IsTCToRead(uint8_t Y);
READER_Status READER_ATR_IsTDToRead(uint8_t Y);

uint8_t READER_ATR_GetY(uint8_t TD);
uint8_t READER_ATR_GetT(uint8_t TD);

READER_Status READER_ATR_Receive(READER_ATR_Atr *atr);

void READER_ATR_ErrHandler(void);
