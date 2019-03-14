#ifndef __READER_HAL_CONTEXT_HANDLER_H__
#define __READER_HAL_CONTEXT_HANDLER_H__



#include "reader_hal_comm_settings.h"
#include "reader.h"





typedef struct READER_T0_ContextHandler READER_T0_ContextHandler;
struct READER_T0_ContextHandler{
	READER_HAL_CommSettings halCommSettings;
	uint32_t currentWI;
	uint32_t currentWT;
};





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




#endif
