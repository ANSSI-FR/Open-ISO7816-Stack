/**
 * \file reader_hal_comm_settings.c
 * \copyright This file is part of the Open-ISO7816-Stack project and is distributed under the MIT license. See LICENSE file in the root directory. 
 * This file provides primitives for interacting (specifically configuring) with the hardware in order to manipulate ISO7816-3 signals (I/O line, clock, reset, power ...).
 */


#include "reader_hal_comm_settings.h"
#include "stdint.h"
#include "reader.h"
#include "reader_utils.h"
#include "stm32f4xx_hal.h"



extern SMARTCARD_HandleTypeDef smartcardHandleStruct;


/**
 * \fn READER_HAL_SetFreq(READER_HAL_CommSettings *pSettings, uint32_t newFreq)
 * \return READER_Status execution code. READER_OK indicates successful execution. Any other value is an error.
 * \param *pSettings is a pointer on a READER_HAL_CommSettings data structure.
 * \param newFreq is the new frequency (in Hz) to be applied to the CLK line (system clock provided to the smartcard). Currently, only a finite set of CLK values are supported. See implementation of the READER_UTILS_ComputePrescFromFreq() function for additional information.
 * This function is intended to define (or update) the system clock frequency provided to the smartcard.
 */
READER_Status READER_HAL_SetFreq(READER_HAL_CommSettings *pSettings, uint32_t newFreq){
	uint32_t oldFreq, oldBaudRate;
	uint32_t newBaudRate;
	
	
	/* On recupere la frequence et la baudrate actuel. Peut aussi etre recupere a partir des infos de *currentSettings */
	oldFreq = READER_UTILS_GetCardFreq(READER_HAL_STM32_SYSCLK, READER_HAL_STM32_AHB_PRESC, READER_HAL_STM32_APB1_PRESC, smartcardHandleStruct.Init.Prescaler);
	oldBaudRate = smartcardHandleStruct.Init.BaudRate;
	newBaudRate = READER_UTILS_ComputeNewBaudRate(oldBaudRate, oldFreq, newFreq);
	
	/* On applique les changements au bloc materiel USART */
	smartcardHandleStruct.Init.BaudRate = newBaudRate;
	smartcardHandleStruct.Init.Prescaler = READER_HAL_ComputePrescFromFreq(newFreq);
	
	if(HAL_SMARTCARD_Init(&smartcardHandleStruct) != HAL_OK) return READER_ERR;
	
	/* On modifie en consequence la valeur du Wait Time (WT) (car WT est dependant de f) */
	//WI = READER_HAL_GetWI(pSettings);
	//newWaitTime = READER_UTILS_ComputeWT1(newFreq, READER_HAL_GetFi(pSettings), WI);
	//retVal = READER_HAL_SetWT(pSettings, newWaitTime); 
	//if(retVal != READER_OK) return retVal;
	
	/* On modifie en consequence la valeur du BWT Voir ISO7816-3 section 11.4.3 */
	//BWI = READER_HAL_GetBWI();
	//newBlockWaitTimeEtu = READER_UTILS_ComputeBWTEtu(BWI, newFreq);
	//newBlockWaitTimeMili = READER_UTILS_ComputeBWTMili(newBlockWaitTimeEtu, READER_HAL_GetFi(), READER_HAL_GetDi(), newFreq);
	//READER_HAL_SetBWT(newBlockWaitTimeMili);
	
	/* Mise a jour des informations dans la structure qui contient les parametres de communication */
	pSettings->f = newFreq;
	
	return READER_OK;
}


/**
 * \fn READER_HAL_SetEtu(READER_HAL_CommSettings *pSettings, uint32_t Fi, uint32_t Di)
 * \return READER_Status execution code. READER_OK indicates successful execution. Any other value is an error.
 * \param *pSettings is a pointer on a READER_HAL_CommSettings data structure containg the current lowel level communication settings.
 * \param Fi "Clock Rate Conversion Integer" see ISO/IEC7816-3 section 7.1.
 * \param Di "Baudrate Adjustement Integer" see ISO/IEC7816-3 section 7.1.
 * This function modifies the "Elementary Time Unit" (ETU, see ISO/IEC7816-3 section 7.1) currently being in use.
 */
READER_Status READER_HAL_SetEtu(READER_HAL_CommSettings *pSettings, uint32_t Fi, uint32_t Di){
	uint32_t freq, newBaudRate;
	
	/* On recupere les parametres de communication actuels. On aurait aussi pu le faire a partir de la structure globalCurrentSettings */
	freq = READER_UTILS_GetCardFreq(READER_HAL_STM32_SYSCLK, READER_HAL_STM32_AHB_PRESC, READER_HAL_STM32_APB1_PRESC, smartcardHandleStruct.Init.Prescaler);
	
	/* On calcule le nouveau baudrate qui correspond a la nouvelle config (nouveau etu) */
	//newBaudRate = freq / (Fi / Di);
	newBaudRate = READER_UTILS_ComputeBaudRate(freq, Fi, Di);
	
	/* On applique les changements au bloc materiel USART */
	smartcardHandleStruct.Init.BaudRate = newBaudRate;
	if(HAL_SMARTCARD_Init(&smartcardHandleStruct) != HAL_OK) return READER_ERR;
	
	/* Le changement de l'etu a pour consequence la modification du Wait Time (WT) */
	//WI = READER_HAL_GetWI(pSettings);
	//newWT = READER_UTILS_ComputeWT1(freq, Fi, WI);
	//retVal = READER_HAL_SetWT(pSettings, newWT);
	//if(retVal != READER_OK) return retVal;	
	
	/* On met a jour la structure globalCurrentSettings */
	pSettings->Fi = Fi;
	pSettings->Di = Di;
	
	return READER_OK;	
}


/**
 * \fn READER_HAL_SetFi(READER_HAL_CommSettings *pSettings, uint32_t Fi)
 * \return READER_Status execution code. READER_OK indicates successful execution. Any other value is an error.
 * \param *pSettings is a pointer on a READER_HAL_CommSettings data structure containg the current lowel level communication settings.
 * \param Fi "Clock Rate Conversion Integer" see ISO/IEC7816-3 section 7.1.
 * This function modifies the "Clock Rate Conversion Integer" (Fi parameter, see ISO/IEC7816-3 section 7.1) currently being in use.
 */
READER_Status READER_HAL_SetFi(READER_HAL_CommSettings *pSettings, uint32_t Fi){
	READER_Status retVal;
	uint32_t Di;
	
	Di = READER_HAL_GetDi(pSettings);
	retVal = READER_HAL_SetEtu(pSettings, Fi, Di);
	if(retVal != READER_OK) return retVal;
	
	pSettings->Fi = Fi;
	
	return READER_OK;
}


/**
 * \fn READER_HAL_SetDi(READER_HAL_CommSettings *pSettings, uint32_t Di)
 * \return READER_Status execution code. READER_OK indicates successful execution. Any other value is an error.
 * \param *pSettings is a pointer on a READER_HAL_CommSettings data structure containg the current lowel level communication settings.
 * \param Di "Baudrate Adjustement Integer" see ISO/IEC7816-3 section 7.1.
 * This function modifies the "Baudrate Adjustement Integer" (Di parameter, see ISO/IEC7816-3 section 7.1) currently being in use.
 */
READER_Status READER_HAL_SetDi(READER_HAL_CommSettings *pSettings, uint32_t Di){
	READER_Status retVal;
	uint32_t Fi;
	
	Fi = READER_HAL_GetFi(pSettings);
	retVal = READER_HAL_SetEtu(pSettings, Fi, Di);
	if(retVal != READER_OK) return retVal;
	
	pSettings->Di = Di;
	
	return READER_OK;
}


/**
 * \fn READER_HAL_SetGT(READER_HAL_CommSettings *pSettings, uint32_t newGT)
 * \return READER_Status execution code. READER_OK indicates successful execution. Any other value is an error.
 * \param *pSettings is a pointer on a READER_HAL_CommSettings data structure containg the current lowel level communication settings.
 * \param newGT is the new GT (Guard Time) value to be set. This value is an integer number of ETUs (Elementary Time Unit, see ISO/IEC7816-3 section 7.1).
 * This function configures the GT (Guard Time) to be used when communication over the I/O transmission line. See ISO/IEC7816-3 section 7.2.
 */
READER_Status READER_HAL_SetGT(READER_HAL_CommSettings *pSettings, uint32_t newGT){
	if(newGT < 12) return READER_ERR;
	
	smartcardHandleStruct.Init.GuardTime = newGT;
	
	if(HAL_SMARTCARD_Init(&smartcardHandleStruct) != HAL_OK) return READER_ERR;
	
	pSettings->GT = newGT;
	
	return READER_OK;	
}


/**
 * \fn READER_HAL_GetGT(READER_HAL_CommSettings *pSettings)
 * \param *pSettings is a pointer on a READER_HAL_CommSettings data structure containg the current lowel level communication settings.
 * \return GT value currently being in use. Expressed in number of ETU.
 * Returns the GT (Guard Time, see ISO/IEC7816-3 section 7.2) value currently being used. This value is expressed as an integer number of ETU (Elementary Time Unit, see ISO/IEC7816-3 section 7.1).
 */
uint32_t READER_HAL_GetGT(READER_HAL_CommSettings *pSettings){
	return pSettings->GT;
}


/**
 * \fn READER_HAL_GetGTMili(READER_HAL_CommSettings *pSettings)
 * \param *pSettings is a pointer on a READER_HAL_CommSettings data structure containg the current lowel level communication settings.
 * \return GT value currently being applied to the communications over the I/O transmission line. Expressed in milliseconds.
 * Returns the GT (Guard Time, see ISO/IEC7816-3 section 7.2) value currently being used. This value is expressed in milliseconds.
 */
uint32_t READER_HAL_GetGTMili(READER_HAL_CommSettings *pSettings){
	uint32_t freq, Fi, Di, GTEtu;
	uint32_t GTMilli;
	
	Fi     =  READER_HAL_GetFi(pSettings);
	Di     =  READER_HAL_GetDi(pSettings);
	freq   =  READER_HAL_GetFreq(pSettings);
	GTEtu  =  READER_HAL_GetGT(pSettings);
	
	GTMilli = (uint32_t)(GTEtu * READER_UTILS_ComputeEtuMili(Fi, Di, freq));
	
	
	return GTMilli;
}


/**
 * \fn READER_HAL_GetFreq(READER_HAL_CommSettings *pSettings)
 * \param *pSettings is a pointer on a READER_HAL_CommSettings data structure containg the current lowel level communication settings.
 * \return Current frequency of the system clock provided to the smartcard over the CLK line.
 */
uint32_t READER_HAL_GetFreq(READER_HAL_CommSettings *pSettings){
	return pSettings->f;
}


/**
 * \fn READER_HAL_GetFi(READER_HAL_CommSettings *pSettings)
 * \param *pSettings is a pointer on a READER_HAL_CommSettings data structure containg the current lowel level communication settings.
 * \return Current value of the Fi paramter being in use for ETU computation (see ISO/IEC7816-3 section 7.1).
 */
uint32_t READER_HAL_GetFi(READER_HAL_CommSettings *pSettings){
	return pSettings->Fi;
}


/**
 * \fn READER_HAL_GetDi(READER_HAL_CommSettings *pSettings)
 * \param *pSettings is a pointer on a READER_HAL_CommSettings data structure containg the current lowel level communication settings.
 * \return Current value of the Di paramter being in use for ETU computation (see ISO/IEC7816-3 section 7.1).
 */
uint32_t READER_HAL_GetDi(READER_HAL_CommSettings *pSettings){
	return pSettings->Di;
}


/*
 * TODO: Seek the corresponding prescaler values and correct this function.
 */
#ifdef TARGET_STM32F411
uint32_t READER_HAL_ComputePrescFromFreq(uint32_t freq){
	switch(freq){
		case 4200000:
			return SMARTCARD_PRESCALER_SYSCLK_DIV6;
			break;
		case 3500000:
			return SMARTCARD_PRESCALER_SYSCLK_DIV6;
			break;
		case 3000000:
			return SMARTCARD_PRESCALER_SYSCLK_DIV6;
			break;
		case 5250000:
			return SMARTCARD_PRESCALER_SYSCLK_DIV6;
			break;
		default:
			return SMARTCARD_PRESCALER_SYSCLK_DIV6;
	}
}
#elif TARGET_STM32F407
uint32_t READER_HAL_ComputePrescFromFreq(uint32_t freq){
	switch(freq){
		case 4200000:
			return SMARTCARD_PRESCALER_SYSCLK_DIV10;
			break;
		case 3500000:
			return SMARTCARD_PRESCALER_SYSCLK_DIV12;
			break;
		case 3000000:
			return SMARTCARD_PRESCALER_SYSCLK_DIV14;
			break;
		case 5250000:
			return SMARTCARD_PRESCALER_SYSCLK_DIV8;
			break;
		default:
			return SMARTCARD_PRESCALER_SYSCLK_DIV10;
	}
}
#else
	#error No target is defined. Impossible to go through compilation. Please define a target by setting a constant in the style TARGET_STM32F411 or TARGET_STM32F407. See documentation for the list of supported targets.
#endif
