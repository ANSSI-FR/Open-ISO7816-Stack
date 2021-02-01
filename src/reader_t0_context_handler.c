/**
 * \file reader_t0_context_handler.c
 * \copyright This file is part of the Open-ISO7816-Stack project and is distributed under the MIT license. See LICENSE file in the root directory. 
 * This file provides functions for safely interacting with the READER_T0_ContextHandler data structure.
 */


#include "reader_t0_context_handler.h"



/**
 * \fn READER_T0_CONTEXT_Init(READER_T0_ContextHandler *pContext, READER_HAL_CommSettings *pSettings)
 * \return READER_Status execution code. READER_OK indicates successful execution. Any other value is an error.
 * \param *pContext is a pointer over a READER_T0_ContextHandler data structure storing the current T=0 communication context.
 * \param *pSettings is a pointer on a READER_HAL_CommSettings data structure. It has to be previously initialized and has to contain correct comminication parameters.
 */
READER_Status READER_T0_CONTEXT_Init(READER_T0_ContextHandler *pContext, READER_HAL_CommSettings *pSettings){
	READER_Status retVal;
	
	
	retVal = READER_T0_CONTEXT_ImportHalCommSettingsToContext(pContext, pSettings);
	if(retVal != READER_OK) return retVal;
	
	retVal = READER_T0_CONTEXT_SetCurrentWI(pContext, READER_T0_CONTEXT_DEFAULT_WI);  /* Quand on set le WI ca set aussi le WT .. */
	if(retVal != READER_OK) return retVal;
	
	
	return READER_OK;
}


/**
 * \fn READER_T0_CONTEXT_GetHalCommSettingsPtr(READER_T0_ContextHandler *pContext, READER_HAL_CommSettings **ppCommSettings)
 * \return READER_Status execution code. READER_OK indicates successful execution. Any other value is an error.
 * \param *pContext is a pointer over a READER_T0_ContextHandler data structure storing the current T=0 communication context.
 * \param **ppCommSettings is a pointer pointing on a READER_HAL_CommSettings pointer. This pointer is used to modify the second pointer in prder to make it point to the READER_HAL_CommSettings contained inside the T=0 communication context.
 * This function retries a pointer on the READER_HAL_CommSettings data structure contained inside the T=0 communication context.
 */
READER_Status READER_T0_CONTEXT_GetHalCommSettingsPtr(READER_T0_ContextHandler *pContext, READER_HAL_CommSettings **ppCommSettings){
	READER_HAL_CommSettings *pCommSettings;
	
	
	pCommSettings = &(pContext->halCommSettings);
	*ppCommSettings = pCommSettings;
	
	return READER_OK;
}


/**
 * \fn READER_T0_CONTEXT_GetHalCommSettingsFreq(READER_T0_ContextHandler *pContext, uint32_t *pFreq)
 * \return READER_Status execution code. READER_OK indicates successful execution. Any other value is an error.
 * \param *pContext is a pointer over a READER_T0_ContextHandler data structure storing the current T=0 communication context.
 * \param *pFreq is a pointer on an uint32_t value to be filled with the frequency (in Hz) currently being supplied to the smartcard.
 * Gets the current frequency (in Hz) currently being supplied to teh smartcard. It gets it from the T=0 communication context.
 */
READER_Status READER_T0_CONTEXT_GetHalCommSettingsFreq(READER_T0_ContextHandler *pContext, uint32_t *pFreq){
	READER_HAL_CommSettings *pCommSettings;
	READER_Status retVal;
	
	
	retVal = READER_T0_CONTEXT_GetHalCommSettingsPtr(pContext, &pCommSettings);
	if(retVal != READER_OK) return READER_ERR;
	
	*pFreq = READER_HAL_GetFreq(pCommSettings);	
	
	
	return READER_OK;
}


/**
 * \fn READER_T0_CONTEXT_GetHalCommSettingsFi(READER_T0_ContextHandler *pContext, uint32_t *pFi)
 * \return READER_Status execution code. READER_OK indicates successful execution. Any other value is an error.
 * \param *pContext is a pointer over a READER_T0_ContextHandler data structure storing the current T=0 communication context.
 * \param *pFi is a pointer on an uint32_t value to be filled with the current Fi parameter value.
 * Gets the current value of the Fi parameter (see ISO/IEC7816-3 section 7.1).
 */
READER_Status READER_T0_CONTEXT_GetHalCommSettingsFi(READER_T0_ContextHandler *pContext, uint32_t *pFi){
	READER_HAL_CommSettings *pCommSettings;
	READER_Status retVal;
	
	
	retVal = READER_T0_CONTEXT_GetHalCommSettingsPtr(pContext, &pCommSettings);
	if(retVal != READER_OK) return READER_ERR;
	
	*pFi = READER_HAL_GetFi(pCommSettings);	
	
	
	return READER_OK;
}


/**
 * \fn READER_T0_CONTEXT_GetHalCommSettingsDi(READER_T0_ContextHandler *pContext, uint32_t *pDi)
 * \return READER_Status execution code. READER_OK indicates successful execution. Any other value is an error.
 * \param *pContext is a pointer over a READER_T0_ContextHandler data structure storing the current T=0 communication context.
 * \param *pDi is a pointer on an uint32_t value to be filled with the current Di parameter value.
 * Gets the current value of the Di parameter (see ISO/IEC7816-3 section 7.1).
 */
READER_Status READER_T0_CONTEXT_GetHalCommSettingsDi(READER_T0_ContextHandler *pContext, uint32_t *pDi){
	READER_HAL_CommSettings *pCommSettings;
	READER_Status retVal;
	
	
	retVal = READER_T0_CONTEXT_GetHalCommSettingsPtr(pContext, &pCommSettings);
	if(retVal != READER_OK) return READER_ERR;
	
	*pDi = READER_HAL_GetDi(pCommSettings);	
	
	
	return READER_OK;
}


/**
 * \fn READER_T0_CONTEXT_GetHalCommSettingsGT(READER_T0_ContextHandler *pContext, uint32_t *pGT)
 * \return READER_Status execution code. READER_OK indicates successful execution. Any other value is an error.
 * \param *pContext is a pointer over a READER_T0_ContextHandler data structure storing the current T=0 communication context.
 * \param *pGT is a pointer on an uint32_t value to be filled with the current GT parameter value.
 * Gets the current value of the GT parameter (see ISO/IEC7816-3 section 7.2).
 */
READER_Status READER_T0_CONTEXT_GetHalCommSettingsGT(READER_T0_ContextHandler *pContext, uint32_t *pGT){
	READER_HAL_CommSettings *pCommSettings;
	READER_Status retVal;
	
	
	retVal = READER_T0_CONTEXT_GetHalCommSettingsPtr(pContext, &pCommSettings);
	if(retVal != READER_OK) return READER_ERR;
	
	*pGT = READER_HAL_GetGT(pCommSettings);	
	
	
	return READER_OK;
}


/**
 * \fn READER_T0_CONTEXT_SetHalCommSettingsFreq(READER_T0_ContextHandler *pContext, uint32_t freq)
 * \return READER_Status execution code. READER_OK indicates successful execution. Any other value is an error.
 * \param *pContext is a pointer over a READER_T0_ContextHandler data structure storing the current T=0 communication context.
 * \param freq is the new frequency (in Hz) to be applied to the CLK line (system clock provided to the smartcard). Currently, only a finite set of CLK values are supported. See implementation of the READER_UTILS_ComputePrescFromFreq() function for additional information.
 * This function is intended to define (or update) the system clock frequency provided to the smartcard.
 */
READER_Status READER_T0_CONTEXT_SetHalCommSettingsFreq(READER_T0_ContextHandler *pContext, uint32_t freq){
	READER_HAL_CommSettings *pCommSettings;
	READER_Status retVal;
	
	
	if(freq == 0){
		return READER_BAD_ARG;
	}
	
	retVal = READER_T0_CONTEXT_GetHalCommSettingsPtr(pContext, &pCommSettings);
	if(retVal != READER_OK) return READER_ERR;
	
	retVal = READER_HAL_SetFreq(pCommSettings, freq);
	if(retVal != READER_OK) return READER_ERR;
	
	
	return READER_OK;
}


/**
 * \fn READER_T0_CONTEXT_SetHalCommSettingsFi(READER_T0_ContextHandler *pContext, uint32_t Fi)
 * \return READER_Status execution code. READER_OK indicates successful execution. Any other value is an error.
 * \param *pContext is a pointer over a READER_T0_ContextHandler data structure storing the current T=0 communication context.
 * \param Fi is the new value to be used as the Fi ("Clock Rate Conversion Integer") parameter (see ISO/IEC7816-3 section 7.1).
 * Sets the new Fi value to be used.
 */
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


/**
 * \fn READER_T0_CONTEXT_SetHalCommSettingsDi(READER_T0_ContextHandler *pContext, uint32_t Di)
 * \return READER_Status execution code. READER_OK indicates successful execution. Any other value is an error.
 * \param *pContext is a pointer over a READER_T0_ContextHandler data structure storing the current T=0 communication context.
 * \param Di is the new value to be used as the Di ("Baudrate Adjustement Integer") parameter (see ISO/IEC7816-3 section 7.1).
 * Sets the new Di value to be used.
 */
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


/**
 * \fn READER_T0_CONTEXT_SetHalCommSettingsGT(READER_T0_ContextHandler *pContext, uint32_t GT)
 * \return READER_Status execution code. READER_OK indicates successful execution. Any other value is an error.
 * \param *pContext is a pointer over a READER_T0_ContextHandler data structure storing the current T=0 communication context.
 * \param GT is the new value to be used as the GT (Guard Time) parameter (see ISO/IEC7816-3 section 7.2). This value is an integer number of ETUs (Elementary Time Unit, see ISO/IEC7816-3 section 7.1).
 * Sets the new GT value to be used.
 */
READER_Status READER_T0_CONTEXT_SetHalCommSettingsGT(READER_T0_ContextHandler *pContext, uint32_t GT){
	READER_HAL_CommSettings *pCommSettings;
	READER_Status retVal;
	
	
	retVal = READER_T0_CONTEXT_GetHalCommSettingsPtr(pContext, &pCommSettings);
	if(retVal != READER_OK) return READER_ERR;
	
	retVal = READER_HAL_SetGT(pCommSettings, GT);
	if(retVal != READER_OK) return READER_ERR;
	
	
	return READER_OK;
}


/**
 * \fn READER_T0_CONTEXT_ImportHalCommSettingsToContext(READER_T0_ContextHandler *pContext, READER_HAL_CommSettings *pSettings)
 * \return READER_Status execution code. READER_OK indicates successful execution. Any other value is an error.
 * \param *pContext is a pointer over a READER_T0_ContextHandler data structure storing the current T=0 communication context.
 * \param *pSettings is a pointer on a READER_HAL_CommSettings data structure containg the current lowel level communication settings.
 */
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


/**
 * \fn READER_T0_CONTEXT_ExportHalCommSettingsFromContext(READER_T0_ContextHandler *pContext, READER_HAL_CommSettings *pSettings)
 * \return READER_Status execution code. READER_OK indicates successful execution. Any other value is an error.
 * \param *pContext is a pointer over a READER_T0_ContextHandler data structure storing the current T=0 communication context.
 * \param *pSettings is a pointer on a READER_HAL_CommSettings data structure containg the current lowel level communication settings.
 */
READER_Status READER_T0_CONTEXT_ExportHalCommSettingsFromContext(READER_T0_ContextHandler *pContext, READER_HAL_CommSettings *pSettings){
	return READER_OK;
}


/**
 * \fn READER_T0_CONTEXT_GetCurrentWI(READER_T0_ContextHandler *pContext, uint32_t *pCurrentWI)
 * \return READER_Status execution code. READER_OK indicates successful execution. Any other value is an error.
 * \param *pContext is a pointer over a READER_T0_ContextHandler data structure storing the current T=0 communication context.
 */
READER_Status READER_T0_CONTEXT_GetCurrentWI(READER_T0_ContextHandler *pContext, uint32_t *pCurrentWI){
	*pCurrentWI = pContext->currentWI;
	
	return READER_OK;
}


/**
 * \fn READER_T0_CONTEXT_GetCurrentWTMilli(READER_T0_ContextHandler *pContext, uint32_t *pCurrentWT)
 * \return READER_Status execution code. READER_OK indicates successful execution. Any other value is an error.
 * \param *pContext is a pointer over a READER_T0_ContextHandler data structure storing the current T=0 communication context.
 */
/* Attention on prends des millisecondes en param ...  */
READER_Status READER_T0_CONTEXT_GetCurrentWTMilli(READER_T0_ContextHandler *pContext, uint32_t *pCurrentWT){
	READER_Status retVal;
	uint32_t currentWI, currentFreq, currentFi, currentWT;
	
	
	/* On recupere les valeurs necessaires au recalcul de currentWT suite a la modification de currentWI ...  */
	retVal = READER_T0_CONTEXT_GetCurrentWI(pContext, &currentWI);
	if(retVal != READER_OK) return retVal;
	
	retVal = READER_T0_CONTEXT_GetHalCommSettingsFreq(pContext, &currentFreq);
	if(retVal != READER_OK) return retVal;
	
	retVal = READER_T0_CONTEXT_GetHalCommSettingsFi(pContext, &currentFi);
	if(retVal != READER_OK) return retVal;
	
	
	/* On calcule le nouveau currentWT. Voir ISO7816-3 section 10.2 ...  */
	/* Attention, ici contrairement a la spec on manipule des millisecondes et pas des secondes (d'ou le x1000) ...  */
	currentWT = (uint32_t)(1000 * currentWI * 960 * ((float)(currentFi) / (float)(currentFreq))) + 1;  /* Arrondi a l'entier superieur ...  */
	
	
	*pCurrentWT = currentWT;
	
	return READER_OK;
}


/**
 * \fn READER_T0_CONTEXT_SetCurrentWI(READER_T0_ContextHandler *pContext, uint32_t newWI)
 * \return READER_Status execution code. READER_OK indicates successful execution. Any other value is an error.
 * \param *pContext is a pointer over a READER_T0_ContextHandler data structure storing the current T=0 communication context.
 */
READER_Status READER_T0_CONTEXT_SetCurrentWI(READER_T0_ContextHandler *pContext, uint32_t newWI){
	if(newWI == 0){
		return READER_ERR;
	}
	
	
	pContext->currentWI = newWI;
	
	return READER_OK;
}
