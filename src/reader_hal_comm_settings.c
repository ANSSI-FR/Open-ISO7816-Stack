#include "reader_hal_comm_settings.h"
#include "stdint.h"
#include "reader.h"
#include "reader_utils.h"
#include "stm32f4xx_hal.h"




extern SMARTCARD_HandleTypeDef smartcardHandleStruct;


/**
 * \fn READER_Status READER_HAL_SetFreq(uint32_t newFreq)
 * \brief Cette fonction permet de changer la fréquence de l'horloge fournie à la carte à puce. Attention cette fonction dépend de la variable globale : globalCurrentSettings. Cette variable est locale au fichier "reader_hal.c". Cette structure contient en permanance les parametres de communication utilisés.
 * \return Valeur de type READER_Status. READER_OK si l'exécution s'est correctement déroulée. Toute autre valeur suggère une erreur.
 * \param newFreq uint32_t indiquant la nouvelle fréquence à adopter (en Hz). Attention, selon l'implémentation matérielle toutes les fréquences ne sont pas permises. Pour plus d'informations sur les fréquences supportées voir l'implémentation de la fonction READER_UTILS_ComputePrescFromFreq() dans le fichier "reader_utils.h". Attention l'implémentation de cette fonction varie selon la cible matérielle.
 */
READER_Status READER_HAL_SetFreq(READER_HAL_CommSettings *pSettings, uint32_t newFreq){
	//READER_Status retVal;
	uint32_t oldFreq, oldBaudRate;
	uint32_t newBaudRate;
	//uint32_t newWaitTime;
	//uint32_t newBlockWaitTimeEtu, newBlockWaitTimeMili;
	//uint32_t BWI, WI;
	
	
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
 * \fn READER_Status READER_HAL_SetEtu(uint32_t Fi, uint32_t Di)
 * \brief Cette fonction permet de configurer la valeur du "Elementary Time Unit" (ETU) utilisé dans les communications sur la ligne IO. Attention cette fonction dépend de la variable globale : globalCurrentSettings. Cette variable est locale au fichier "reader_hal.c". Cette structure contient en permanance les parametres de communication utilisés.
 * \return Valeur de type READER_Status. READER_OK si l'exécution s'est correctement déroulée. Toute autre valeur suggère une erreur.
 * \param Fi "Clock Rate Conversion Integer"
 * \param Di "Baudrate Adjustement Integer"
 */
READER_Status READER_HAL_SetEtu(READER_HAL_CommSettings *pSettings, uint32_t Fi, uint32_t Di){
	//READER_Status retVal;
	uint32_t freq, newBaudRate;
	//uint32_t newWT;
	//uint32_t WI;
	
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


READER_Status READER_HAL_SetFi(READER_HAL_CommSettings *pSettings, uint32_t Fi){
	READER_Status retVal;
	uint32_t Di;
	
	Di = READER_HAL_GetDi(pSettings);
	retVal = READER_HAL_SetEtu(pSettings, Fi, Di);
	if(retVal != READER_OK) return retVal;
	
	pSettings->Fi = Fi;
	
	return READER_OK;
}

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
 * \fn READER_Status READER_HAL_SetGT(uint32_t newGT)
 * \brief Cette fonction permet de configurer le "Gard Time" (GT) à utiliser lors des communications sur la ligne IO. Le GT est défini dans la norme ISO7816-3 à la section 7.2. Attention cette fonction dépend de la variable globale : globalCurrentSettings. Cette variable est locale au fichier "reader_hal.c". Cette structure contient en permanance les parametres de communication utilisés.
 * \return Valeur de type READER_Status. READER_OK si l'exécution s'est correctement déroulée. Toute autre valeur suggère une erreur.
 * \param uint32_t newGT uint32_t indiquant la nouvelle valeur de GT qu'il faut désormais utiliser. Cette valeur est un nombre entier d'ETU.
 */
READER_Status READER_HAL_SetGT(READER_HAL_CommSettings *pSettings, uint32_t newGT){
	if(newGT < 12) return READER_ERR;
	
	smartcardHandleStruct.Init.GuardTime = newGT;
	
	if(HAL_SMARTCARD_Init(&smartcardHandleStruct) != HAL_OK) return READER_ERR;
	
	pSettings->GT = newGT;
	
	return READER_OK;	
}

/**
 * \fn READER_Status READER_HAL_SetWT(uint32_t newWT)
 * \brief Cette fonction permet de configurer le "Wait Time" (WT) à utiliser lors des communications sur la ligne IO. Le GT est défini dans la norme ISO7816-3 à la section 7.2. Attention cette fonction dépend de la variable globale : globalCurrentSettings. Cette variable est locale au fichier "reader_hal.c". Cette structure contient en permanance les parametres de communication utilisés.
 * \return Valeur de type READER_Status. READER_OK si l'exécution s'est correctement déroulée. Toute autre valeur suggère une erreur.
 * \param uint32_t newGT uint32_t indiquant la nouvelle valeur de WT qu'il faut désormais utiliser. Cette valeur est un nombre entier d'ETU.
 
 */
//READER_Status READER_HAL_SetWT(READER_HAL_CommSettings *pSettings, uint32_t newWT){
//	//globalWaitTimeMili = newWT;
//	pSettings->WT = newWT;
//	return READER_OK;
//}

//READER_Status READER_HAL_SetWI(READER_HAL_CommSettings *pSettings, uint32_t WI){
//	READER_Status retVal;
//	uint32_t WTMili;
//	uint32_t f;
//	uint32_t Fi;
//	
//	pSettings->WI = WI;
//	
//	/* Modification de WT en consequence. Voir ISO7816-3 section 10.2 */
//	f = READER_HAL_GetFreq(pSettings);
//	Fi = READER_HAL_GetFi(pSettings);
//	
//	WTMili = READER_UTILS_ComputeWT1(f, Fi, WI);
//	retVal = READER_HAL_SetWT(pSettings, WTMili);
//	if(retVal != READER_OK) return retVal;
//	
//	
//	return READER_OK;
//}


//READER_Status READER_HAL_SetIFSC(uint32_t IFSC){
//	globalCurrentSettings.IFSC = IFSC;
//	
//	return READER_OK;
//}


//READER_Status READER_HAL_SetIFSD(uint32_t IFSD){
//	globalCurrentSettings.IFSD = IFSD;
//	
//	return READER_OK;
//}


//READER_Status READER_HAL_SetBWT(uint32_t BWT){
//	globalCurrentSettings.BWT = BWT;
//	
//	return READER_OK;
//}


//READER_Status READER_HAL_SetBWI(uint32_t BWI){
//	READER_Status retVal;
//	uint32_t BWTEtu, BWTMili;
//	uint32_t currentFreq, currentF, currentD;
//	
//	currentFreq = READER_HAL_GetFreq();
//	currentF = READER_HAL_GetFi();
//	currentD = READER_HAL_GetDi();
//	
//	BWTEtu = READER_UTILS_ComputeBWTEtu(BWI, currentFreq);
//	BWTMili = READER_UTILS_ComputeBWTMili(BWTEtu, currentF, currentD, currentFreq);
//	
//	/* On modifie le BWT en consequence  */
//	retVal = READER_HAL_SetBWT(BWTMili);
//	if(retVal != READER_OK) return retVal;
//	
//	
//	globalCurrentSettings.BWI = BWI;
//	
//	
//	return READER_OK;
//}


//READER_Status READER_HAL_SetBGT(uint32_t BGT){
//	globalCurrentSettings.BGT = BGT;
//	
//	return READER_OK;
//}


//READER_Status READER_HAL_SetRedundancyType(READER_HAL_CommSettings *pSettings, uint32_t rType){
//	pSettings->redundancyType = rType;
//	
//	return READER_OK;
//}





/**
 * \fn uint32_t READER_HAL_GetWT(void)
 * \brief Cette fonction permet d'obtenir le Wait Time (WT) selon les parametres de communication actuels. Attention cette fonction dépend de la variable globale : globalCurrentSettings. Cette variable est locale au fichier "reader_hal.c".
 * \return Retourne la valeur du WT (tel que defini dans la norme ISO) en milisecondes.
 */
//uint32_t READER_HAL_GetWT(READER_HAL_CommSettings *pSettings){
//	pSettings->WT;
//}


//uint32_t READER_HAL_GetWI(READER_HAL_CommSettings *pSettings){
//	pSettings->WI;
//}


//uint32_t READER_HAL_GetBWT(void){
//	return globalCurrentSettings.BWT;
//}

//uint32_t READER_HAL_GetBWI(void){
//	return globalCurrentSettings.BWI;
//}

//uint32_t READER_HAL_GetBGT(void){
//	return globalCurrentSettings.BGT;
//}


/**
 * \fn uint32_t READER_HAL_GetGT(void)
 * \brief Cette fonction retourne le Guard Time (GT) actuellement utilisé par toutes les fonctions de la bibliothèque. 
 * \return Guard Time exprimé en nombre d'ETU.
 */
uint32_t READER_HAL_GetGT(READER_HAL_CommSettings *pSettings){
	return pSettings->GT;
}

/**
 * \fn uint32_t READER_HAL_GetGTMili(void)
 * \brief Cette fonction renvoie le Guard Time (GT) utilié actuellment.
 * \return Guard Time exprimé en milisecondes. Attention cette valeur est arrondie à l'entier supérieur. Dans certains cas cela peut être problématique, il faut rester vigilant.
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

uint32_t READER_HAL_GetFreq(READER_HAL_CommSettings *pSettings){
	return pSettings->f;
}

uint32_t READER_HAL_GetFi(READER_HAL_CommSettings *pSettings){
	return pSettings->Fi;
}

uint32_t READER_HAL_GetDi(READER_HAL_CommSettings *pSettings){
	return pSettings->Di;
}

//uint32_t READER_HAL_GetRedunancyType(READER_HAL_CommSettings *pSettings){
//	return pSettings->redundancyType;
//}

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
