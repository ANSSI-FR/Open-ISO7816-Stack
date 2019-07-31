/**
 * \file reader_hal.c
 * \author Boris
 */



#include "reader_hal.h"
#include "reader_hal_basis.h"
#include "reader_periph.h"



/**
 * \fn READER_Status READER_HAL_InitWithDefaults(READER_HAL_CommSettings *pSettings)
 * \return The function returns an execution code of type READER_Status that indicates if the function behaved as expected or not.
 * \param *pSettings is a pointer on a READER_HAL_CommSettings data structure that contains low level communication settings for the hardware abstraction layer.
 * This function is used i order to initialize the READER_HAL_CommSettings structure pointed by *pSettings to the initial default values as defined in the ISO7816-3 section 8.3.
 */ 
READER_Status READER_HAL_InitWithDefaults(READER_HAL_CommSettings *pSettings){
	READER_Status retVal;
	
	
	retVal = READER_HAL_InitHardware();
	if(retVal != READER_OK) return retVal;
	
	
	/* Initialisation du WT, du GT, du Etu (Fi, Di) et de f                                    */
	/* Attention, important que pour la toute premiere init, F et D soint init en premier      */
	//retVal = READER_HAL_SetWT(READER_DEFAULT_WT_MILI);                if(retVal != READER_OK) return retVal;
	/* Attention l'ordre peut etre important, certaines valeurs dependant des autres           */
	retVal = READER_HAL_SetGT(pSettings, READER_HAL_DEFAULT_GT);                     if(retVal != READER_OK) return retVal;
	retVal = READER_HAL_SetFi(pSettings, READER_HAL_DEFAULT_FI);                     if(retVal != READER_OK) return retVal;
	retVal = READER_HAL_SetDi(pSettings, READER_HAL_DEFAULT_DI);                     if(retVal != READER_OK) return retVal;
	retVal = READER_HAL_SetEtu(pSettings, READER_HAL_DEFAULT_FI, READER_HAL_DEFAULT_DI); if(retVal != READER_OK) return retVal;
	retVal = READER_HAL_SetFreq(pSettings, READER_HAL_DEFAULT_FREQ);                 if(retVal != READER_OK) return retVal;
	//retVal = READER_HAL_SetWI(pSettings, READER_DEFAULT_WI);                     if(retVal != READER_OK) return retVal;
	//retVal = READER_HAL_SetBWI(READER_DEFAULT_BWI);                   if(retVal != READER_OK) return retVal;
	//retVal = READER_HAL_SetBWT(READER_DEFAULT_BWT);                   if(retVal != READER_OK) return retVal;
	//retVal = READER_HAL_SetBGT(READER_DEFAULT_BGT);                   if(retVal != READER_OK) return retVal;
	//retVal = READER_HAL_SetIFSC(READER_DEFAULT_IFSC);                 if(retVal != READER_OK) return retVal;
	//retVal = READER_HAL_SetIFSD(READER_DEFAULT_IFSD);                 if(retVal != READER_OK) return retVal;
	//retVal = READER_HAL_SetRedundancyType(pSettings, READER_DEFAULT_REDUNDANCY_TYPE); if(retVal != READER_OK) return retVal;
	
	
	return READER_OK;
}




READER_Status READER_HAL_SendCharFrameTickstart(READER_HAL_CommSettings *pSettings, READER_HAL_Protocol protocol, uint8_t *frame, uint32_t frameSize, uint32_t timeout, uint32_t *pTickstart){
	READER_Status retVal;
	uint32_t characterDelay;
	uint32_t i;
	
	
	for(i=0; i<frameSize; i++){
		//tickstart = READER_HAL_GetTick();
		
		retVal = READER_HAL_SendChar(pSettings, protocol, frame[i], timeout);
		if(retVal != READER_OK) return retVal;
		
		/* On mets a jour la date du debut de l'envoi du dernier caractere de la frame. (On soustrait le temps en milisec qu'il faut pour envoyer un carac) */
		if(i == (frameSize-1)){
			characterDelay = (uint32_t)(READER_UTILS_ComputeEtuMiliFloat(READER_HAL_GetFi(pSettings), READER_HAL_GetDi(pSettings), READER_HAL_GetFreq(pSettings)) *10);
			characterDelay = MAX(1, characterDelay);
			
			pTickstart = READER_HAL_GetTick() - characterDelay;  /*  10 etu dans un caractere (10 moments) */
		}
		
		/* On fait attention a respecter le Guard Time (GT) entre les caracteres */
		//while((READER_HAL_GetTick()-tickstart) < READER_HAL_GetGTMili()){
		//	
		//}
	}
	
	
	/* On attend le flag Transmit Complete */
	/* Permet de bloquer la fonction pour empecher une eventuelle reception (avant d'avoir transmit complete) */
	
	#ifndef TEST
		while(!(USART2->SR & USART_SR_TC)){
			
		}
		
		USART2->CR1 &= ~USART_CR1_TE;
	#endif
	
	return READER_OK;
}


/* ATTENTION, VERIFIER SI DESCRIPTION TOUJOURS A JOUR ...  */
/**
 * \fn READER_Status READER_HAL_SendCharFrame(uint8_t *frame, uint32_t frameSize, uint32_t timeout)
 * \brief Cette fonction permet de placer uen chaine d'octets sur la ligne IO. Cette fonction a un comportement bloquant.
 * \return Valeur de type READER_Status. READER_OK si l'exécution s'est correctement déroulée. Toute autre valeur indique uen erreur.
 * \param *frame Pointeur sur la chaine d'octets à envoyer.
 * \param frameSize Taille de la chaine d'octets à envoyer.
 * \param timeout Valeur du timeout en milisecondes à utiliser. Si cette valeur est READER_HAL_USE_ISO_WT alors le timeout utilisé sera celui spécifié dans la norme ISO.
 */
READER_Status READER_HAL_SendCharFrame(READER_HAL_CommSettings *pSettings, READER_HAL_Protocol protocol, uint8_t *frame, uint32_t frameSize, uint32_t timeout){
	READER_Status retVal;
	uint32_t tickstart;
	
	
	retVal = READER_HAL_SendCharFrameTickstart(pSettings, protocol, frame, frameSize, timeout, &tickstart);
	if(retVal != READER_OK) return retVal;
	
	return READER_OK;
}


/* Retourne le timestamp du leading edge de la frame (Tickstart) ...  */
READER_Status READER_HAL_RcvCharFrameCountTickstart(READER_HAL_CommSettings *pSettings, READER_HAL_Protocol protocol, uint8_t *frame, uint32_t frameSize, uint32_t *rcvCount, uint32_t timeout, uint32_t *pTickstart){
	READER_Status retVal;
	uint32_t i = 0;
	uint8_t rcvByte;
	uint32_t characterDelay;
	uint32_t rcvCounter = 0;
	uint32_t dummy;
	
	
	/* Si l'utilisateur ne veut pas compter le nb de caracteres recus et qu'il mets NULL en guise de pointeur */
	if(rcvCount == NULL){
		rcvCount = &dummy;
	}
	
	/* Le nombre de caracteres recus est de 0 pour l'instant */
	*rcvCount = 0;
	
	while(i<frameSize){
		retVal = READER_HAL_RcvChar(pSettings, protocol, &rcvByte, timeout);
		if(retVal != READER_OK) return retVal;
		
		/* Si on vient de recevoir le dernier caractere, alors on mets a jour le tickstart du leading edge de la frame ...  */
		if(i == (frameSize-1)){
			characterDelay = (uint32_t)(READER_UTILS_ComputeEtuMiliFloat(READER_HAL_GetFi(pSettings), READER_HAL_GetDi(pSettings), READER_HAL_GetFreq(pSettings)) *10);   /*  10 etu dans un caractere (10 moments) */
			characterDelay = MAX(1, characterDelay);
			/* On veut la date du debut de la reception de la frame, donc on retire le temps en milisecondes que dure un caractere ...  */
			*pTickstart = READER_HAL_GetTick() - characterDelay; 
		}
		
		rcvCounter++;
		*rcvCount = rcvCounter;
		
		frame[i] = rcvByte;	
		i++;
	}
	
	return READER_OK;
}


/* ATTENTION, VERIFIER SI DESCRIPTION TOUJOURS VALIDE ...  */
/**
 * \fn READER_Status READER_HAL_RcvCharFrameCount(uint8_t *frame, uint32_t frameSize, uint32_t *rcvCount, uint32_t timeout)
 * \brief Cette fonction permet se se mettre à l'écoute d'une chaine d'octets sur la ligne IO. Cette fonction a un comportement bloquant. La fonction indique le nombre de caractères qu'elle a pu lire.
 * \return Valeur de type READER_Status. READER_OK si l'exécution s'est correctement déroulée. Toute autre valeur suggère une erreur.
 * \param *frame Pointeur sur le buffer à utiliser pour stocker les octets reçus.
 * \param frameSize Nombre d'octets à recevoir.
 * \param timeout Valeur du timeout en milisecondes à utiliser. Si cette valeur est READER_HAL_USE_ISO_WT alors le timeout utilisé sera celui spécifié dans la norme ISO.
 * \param *rcvCount Pointeur sur une variable de type unit32_t. Elle sera remplie avec le nombre de caractères qui ont pu être lus. Si pas de necessite de recuperer cette valeur alors il est possible d'indoquer NULL.
 */
READER_Status READER_HAL_RcvCharFrameCount(READER_HAL_CommSettings *pSettings, READER_HAL_Protocol protocol, uint8_t *frame, uint32_t frameSize, uint32_t *rcvCount, uint32_t timeout){
	READER_Status retVal;
	uint32_t tickstart;
	
	
	retVal = READER_HAL_RcvCharFrameCountTickstart(pSettings, protocol, frame, frameSize, rcvCount, timeout, &tickstart);
	if(retVal != READER_OK) return retVal;
	
	return READER_OK;
}





/**
 * \fn READER_HAL_RcvCharFrame(uint8_t *frame, uint32_t frameSize, uint32_t timeout)
 * \brief Cette fonction permet se se mettre à l'écoute d'une chaine d'octets sur la ligne IO. Cette fonction a un comportement bloquant.
 * \return Valeur de type READER_Status. READER_OK si l'exécution s'est correctement déroulée. Toute autre valeur suggère une erreur.
 * \param *frame Pointeur sur le buffer à utiliser pour stocker les octets reçus.
 * \param frameSize Nombre d'octets à recevoir.
 * \param timeout Valeur du timeout en milisecondes à utiliser. Si cette valeur est READER_HAL_USE_ISO_WT alors le timeout utilisé sera celui spécifié dans la norme ISO.
 */
 
 /* Cette fonction est plus limitee que la fonction READER_HAL_RcvCharFrameCount()
  * Cette fonction fourni juste une interface simplifiee pour utiliser la fonction READER_HAL_RcvCharFrameCount()
  * Elle fait la meme chose a part qu'elle ne compte pas le nombre de caracteres recus.
  * Cette fonction est conservee pour des resos de compatibilite avec du code plus ancien.
  * Elle peret egalement de ne pas s'embeter avec des parametres supplementaires lorsque il n'y a pas besoin de compter le nombre de caracteres recus.
  */
READER_Status READER_HAL_RcvCharFrame(READER_HAL_CommSettings *pSettings, READER_HAL_Protocol protocol, uint8_t *frame, uint32_t frameSize, uint32_t timeout){
	READER_Status retVal;
	uint32_t rcvCount;
	
	retVal = READER_HAL_RcvCharFrameCount(pSettings, protocol, frame, frameSize, &rcvCount, timeout);
	if(retVal != READER_OK) return retVal;
	
	return READER_OK;
}








void READER_HAL_ErrHandler(void){
	//while(1){
	//	HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_14);
	//	HAL_Delay(100);
	//}
}




/**
 * \fn READER_Status READER_HAL_DoColdReset(void)
 * \brief Cette fonction réalise la procédure de "Cold Reset" définie dans la norme ISO7816-3 section 6.2.2.
 */
READER_Status READER_HAL_DoColdReset(void){
	READER_Status retVal;
	
	/* Activation et Cold Reset */
	READER_HAL_Delay(50);
	retVal = READER_HAL_SetPwrLine(READER_HAL_STATE_ON);      if(retVal != READER_OK) return retVal;
	READER_HAL_Delay(1);                                      
	retVal = READER_HAL_SetClkLine(READER_HAL_STATE_ON);      if(retVal != READER_OK) return retVal;
	READER_HAL_Delay(1);                                      
	retVal = READER_HAL_SetRstLine(READER_HAL_STATE_ON);      if(retVal != READER_OK) return retVal;
	
	return READER_OK;
}

