#include "reader_t0_context_handler.h"




/* Manipulation des commSettings ...  */
READER_Status READER_T0_CONTEXT_GetHalCommSettingsPtr(READER_T0_ContextHandler *pContext, READER_HAL_CommSettings **ppCommSettings){
	READER_HAL_CommSettings *pCommSettings;
	
	
	pCommSettings = &(pContext->halCommSettings);
	*ppCommSettings = pCommSettings;
	
	return READER_OK;
}


READER_Status READER_T0_CONTEXT_GetHalCommSettingsFreq(READER_T0_ContextHandler *pContext, uint32_t *pFreq){
	READER_HAL_CommSettings *pCommSettings;
	READER_Status retVal;
	
	
	retVal = READER_T0_CONTEXT_GetHalCommSettingsPtr(pContext, &pCommSettings);
	if(retVal != READER_OK) return READER_ERR;
	
	*pFreq = READER_HAL_GetFreq(pCommSettings);	
	
	
	return READER_OK;
}


READER_Status READER_T0_CONTEXT_GetHalCommSettingsFi(READER_T0_ContextHandler *pContext, uint32_t *pFi){
	READER_HAL_CommSettings *pCommSettings;
	READER_Status retVal;
	
	
	retVal = READER_T0_CONTEXT_GetHalCommSettingsPtr(pContext, &pCommSettings);
	if(retVal != READER_OK) return READER_ERR;
	
	*pFi = READER_HAL_GetFi(pCommSettings);	
	
	
	return READER_OK;
}


READER_Status READER_T0_CONTEXT_GetHalCommSettingsDi(READER_T0_ContextHandler *pContext, uint32_t *pDi){
	READER_HAL_CommSettings *pCommSettings;
	READER_Status retVal;
	
	
	retVal = READER_T0_CONTEXT_GetHalCommSettingsPtr(pContext, &pCommSettings);
	if(retVal != READER_OK) return READER_ERR;
	
	*pDi = READER_HAL_GetDi(pCommSettings);	
	
	
	return READER_OK;
}


READER_Status READER_T0_CONTEXT_GetHalCommSettingsGT(READER_T0_ContextHandler *pContext, uint32_t *pGT){
	READER_HAL_CommSettings *pCommSettings;
	READER_Status retVal;
	
	
	retVal = READER_T0_CONTEXT_GetHalCommSettingsPtr(pContext, &pCommSettings);
	if(retVal != READER_OK) return READER_ERR;
	
	*pGT = READER_HAL_GetGT(pCommSettings);	
	
	
	return READER_OK;
}



READER_Status READER_T0_CONTEXT_SetHalCommSettingsFreq(READER_T0_ContextHandler *pContext, uint32_t freq){
	READER_HAL_CommSettings *pCommSettings;
	READER_Status retVal;
	
	
	if(frequ == 0){
		return READER_BAD_ARG;
	}
	
	retVal = READER_T0_CONTEXT_GetHalCommSettingsPtr(pContext, &pCommSettings);
	if(retVal != READER_OK) return READER_ERR;
	
	retVal = READER_HAL_SetFreq(pCommSettings, freq);
	if(retVal != READER_OK) return READER_ERR;
	
	
	return READER_OK;
}


READER_Status READER_T0_CONTEXT_SetHalCommSettingsFi(READER_T0_ContextHandler *pContext, uint32_t Fi){
	READER_HAL_CommSettings *pCommSettings;
	READER_Status retVal;
	
	
	if(Fi == 0){
		return READER_BAD_ARG;
	}
	
	retVal = READER_T0_CONTEXT_GetHalCommSettingsPtr(pContext, &pCommSettings);
	if(retVal != READER_OK) return READER_ERR;
	
	retVal = READER_HAL_SetFi(pCommSettings, Fi);
	if(retVal != READER_OK) return READER_ERR;
	
	
	return READER_OK;
}


READER_Status READER_T0_CONTEXT_SetHalCommSettingsDi(READER_T0_ContextHandler *pContext, uint32_t Di){
	READER_HAL_CommSettings *pCommSettings;
	READER_Status retVal;
	
	
	if(Di == 0){
		return READER_BAD_ARG;
	}
	
	retVal = READER_T0_CONTEXT_GetHalCommSettingsPtr(pContext, &pCommSettings);
	if(retVal != READER_OK) return READER_ERR;
	
	retVal = READER_HAL_SetDi(pCommSettings, Di);
	if(retVal != READER_OK) return READER_ERR;
	
	
	return READER_OK;
}


READER_Status READER_T0_CONTEXT_SetHalCommSettingsGT(READER_T0_ContextHandler *pContext, uint32_t GT){
	READER_HAL_CommSettings *pCommSettings;
	READER_Status retVal;
	
	
	retVal = READER_T0_CONTEXT_GetHalCommSettingsPtr(pContext, &pCommSettings);
	if(retVal != READER_OK) return READER_ERR;
	
	retVal = READER_HAL_SetGT(pCommSettings, GT);
	if(retVal != READER_OK) return READER_ERR;
	
	
	return READER_OK;
}




READER_Status READER_T0_CONTEXT_ImportHalCommSettingsToContext(READER_T0_ContextHandler *pContext, READER_HAL_CommSettings *pSettings){
	READER_HAL_CommSettings *pCommSettings;
	READER_Status retVal;
	uint32_t freq, Fi, Di, GT;
	
	
	freq = READER_HAL_GetFreq(pSettings);
	Fi = READER_HAL_GetFi(pSettings);
	Di = READER_HAL_GetDi(pSettings);
	GT = READER_HAL_GetGT(pSettings);
	
	
	retVal = READER_T0_CONTEXT_GetHalCommSettingsPtr(pContext, &pCommSettings);
	if(retVal != READER_OK) return READER_ERR;
	
	
	retVal = READER_T0_CONTEXT_SetHalCommSettingsFreq(pContext, freq);
	if(retVal != READER_OK) return READER_ERR;
	
	retVal = READER_T0_CONTEXT_SetHalCommSettingsFi(pContext, Fi);
	if(retVal != READER_OK) return READER_ERR;
	
	retVal = READER_T0_CONTEXT_SetHalCommSettingsDi(pContext, Di);
	if(retVal != READER_OK) return READER_ERR;
	
	retVal = READER_T0_CONTEXT_SetHalCommSettingsGT(pContext, GT);
	if(retVal != READER_OK) return READER_ERR;
	
	
	return READER_OK;
}


READER_Status READER_T0_CONTEXT_ExportHalCommSettingsFromContext(READER_T0_ContextHandler *pContext, READER_HAL_CommSettings *pSettings){
	
}

