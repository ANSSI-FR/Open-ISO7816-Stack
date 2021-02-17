/**
 * \file reader_t0_context_handler.h
 * \copyright This file is part of the Open-ISO7816-Stack project and is distributed under the MIT license. See LICENSE file in the root directory. 
 * This file contains the prototypes and the type definitions needed to deal with the T=0 protocol communication context.
 */



#ifndef __READER_HAL_CONTEXT_HANDLER_H__
#define __READER_HAL_CONTEXT_HANDLER_H__



#include "reader_hal_comm_settings.h"
#include "reader.h"




#define READER_T0_CONTEXT_DEFAULT_WI         (uint32_t)(10)
//#define READER_DEFAULT_WT_MILI    (uint32_t)((1000 * READER_DEFAULT_WI * 960 * READER_HAL_DEFAULT_FI) / (float)READER_HAL_DEFAULT_FREQ)      /* Voir ISO7816-3 section 10.2 */





typedef struct READER_T0_ContextHandler READER_T0_ContextHandler;
struct READER_T0_ContextHandler{
	READER_HAL_CommSettings halCommSettings;
	uint32_t currentWI;
	//uint32_t currentWT;
};




READER_Status READER_T0_CONTEXT_Init(READER_T0_ContextHandler *pContext, READER_HAL_CommSettings *pSettings);

READER_Status READER_T0_CONTEXT_GetHalCommSettingsPtr(READER_T0_ContextHandler *pContext, READER_HAL_CommSettings **ppCommSettings);

READER_Status READER_T0_CONTEXT_GetHalCommSettingsFreq(READER_T0_ContextHandler *pContext, uint32_t *pFreq);
READER_Status READER_T0_CONTEXT_GetHalCommSettingsFi(READER_T0_ContextHandler *pContext, uint32_t *pFi);
READER_Status READER_T0_CONTEXT_GetHalCommSettingsDi(READER_T0_ContextHandler *pContext, uint32_t *pDi);
READER_Status READER_T0_CONTEXT_GetHalCommSettingsGT(READER_T0_ContextHandler *pContext, uint32_t *pGT);

READER_Status READER_T0_CONTEXT_SetHalCommSettingsFreq(READER_T0_ContextHandler *pContext, uint32_t freq);
READER_Status READER_T0_CONTEXT_SetHalCommSettingsFi(READER_T0_ContextHandler *pContext, uint32_t Fi);
READER_Status READER_T0_CONTEXT_SetHalCommSettingsDi(READER_T0_ContextHandler *pContext, uint32_t Di);
READER_Status READER_T0_CONTEXT_SetHalCommSettingsGT(READER_T0_ContextHandler *pContext, uint32_t GT);


READER_Status READER_T0_CONTEXT_ImportHalCommSettingsToContext(READER_T0_ContextHandler *pContext, READER_HAL_CommSettings *pSettings);
READER_Status READER_T0_CONTEXT_ExportHalCommSettingsFromContext(READER_T0_ContextHandler *pContext, READER_HAL_CommSettings *pSettings);



READER_Status READER_T0_CONTEXT_GetCurrentWI(READER_T0_ContextHandler *pContext, uint32_t *pCurrentWI);
READER_Status READER_T0_CONTEXT_GetCurrentWTMilli(READER_T0_ContextHandler *pContext, uint32_t *pCurrentWT);

READER_Status READER_T0_CONTEXT_SetCurrentWI(READER_T0_ContextHandler *pContext, uint32_t newWI);

#endif
