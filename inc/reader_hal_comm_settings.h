/**
 * \file reader_hal_comm_settings.h
 * \author Boris
 * This file contains the prototypes and the typdefs to deal with the low level communication settings.
 */



#ifndef __READER_HAL_COMM_SETTINGS_H__
#define __READER_HAL_COMM_SETTINGS_H__



#include "stdint.h"
#include "reader.h"
#include "reader_utils.h"



#define READER_HAL_DEFAULT_FI         (uint32_t)(372)
#define READER_HAL_DEFAULT_DI         (uint32_t)(1)
#define READER_HAL_DEFAULT_FREQ       (uint32_t)(4200000)
#define READER_HAL_DEFAULT_GT         (uint32_t)(12)



/**
 * \struct READER_HAL_CommSettings
 * This structure contains all the low level communication settings that are needed by the abstraction layer to work propelly.
 * These parameters are defined in ISO7816-3 section 7.1.
 * Warning : the values in this structure should not by modified otherwise than by the intended accessors.
 */
typedef struct READER_HAL_CommSettings READER_HAL_CommSettings;
struct READER_HAL_CommSettings{
	uint32_t f;     /*!< f is the frequency currently used on the shared CLK line between the reader device and the card. */
	uint32_t Fi;    /*!< Fi is the clock rate conversion integer as defined in ISO7816-3 section 7.1. */
	uint32_t Di;    /*!< Di is the baud rate adjustement integer as defined in ISO7816-3 section 7.1. */
	uint32_t GT;    /*!< GT is the Guard Time between two characters as defined in ISO7816-3 section 7.2. */
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

uint32_t READER_HAL_ComputePrescFromFreq(uint32_t freq);

#endif
