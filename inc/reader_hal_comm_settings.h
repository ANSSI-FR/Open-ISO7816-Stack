#ifndef __READER_HAL_COMM_SETTINGS_H__
#define __READER_HAL_COMM_SETTINGS_H__



#include "stdint.h"
#include "reader.h"
#include "reader_utils.h"
#include "stm32f4xx_hal.h"



#define READER_HAL_DEFAULT_FI         (uint32_t)(372)
#define READER_HAL_DEFAULT_DI         (uint32_t)(1)
#define READER_HAL_DEFAULT_FREQ       (uint32_t)(4200000)
#define READER_HAL_DEFAULT_GT         (uint32_t)(12)




typedef struct READER_HAL_CommSettings READER_HAL_CommSettings;
struct READER_HAL_CommSettings{
	uint32_t f;
	uint32_t Fi;
	uint32_t Di;
	uint32_t GT;
};





READER_Status READER_HAL_SetFreq(READER_HAL_CommSettings *pSettings, uint32_t newFreq);
READER_Status READER_HAL_SetEtu(READER_HAL_CommSettings *pSettings, uint32_t Fi, uint32_t Di);
READER_Status READER_HAL_SetGT(READER_HAL_CommSettings *pSettings, uint32_t newGT);
//READER_Status READER_HAL_SetWT(READER_HAL_CommSettings *pSettings, uint32_t newWT);
READER_Status READER_HAL_SetFi(READER_HAL_CommSettings *pSettings, uint32_t Fi);
READER_Status READER_HAL_SetDi(READER_HAL_CommSettings *pSettings, uint32_t Di);
//READER_Status READER_HAL_SetIFSC(uint32_t IFSC);
//READER_Status READER_HAL_SetIFSD(uint32_t IFSD);
//READER_Status READER_HAL_SetBWT(uint32_t BWT);
//READER_Status READER_HAL_SetBWI(uint32_t BWI);
//READER_Status READER_HAL_SetBGT(uint32_t BGT);
READER_Status READER_HAL_SetRedundancyType(READER_HAL_CommSettings *pSettings, uint32_t rType);
//READER_Status READER_HAL_SetWI(READER_HAL_CommSettings *pSettings, uint32_t WI);
//uint32_t READER_HAL_GetWT(READER_HAL_CommSettings *pSettings);
uint32_t READER_HAL_GetGT(READER_HAL_CommSettings *pSettings);
uint32_t READER_HAL_GetGTMili(READER_HAL_CommSettings *pSettings);
uint32_t READER_HAL_GetFreq(READER_HAL_CommSettings *pSettings);
uint32_t READER_HAL_GetFi(READER_HAL_CommSettings *pSettings);
uint32_t READER_HAL_GetDi(READER_HAL_CommSettings *pSettings);
//uint32_t READER_HAL_GetBWT(void);
//uint32_t READER_HAL_GetBWI(void);
//uint32_t READER_HAL_GetBGT(void);
//uint32_t READER_HAL_GetRedunancyType(READER_HAL_CommSettings *pSettings);
//uint32_t READER_HAL_GetWI(READER_HAL_CommSettings *pSettings);

#endif
