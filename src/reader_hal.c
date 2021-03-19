/**
 * \file reader_hal.c
 * \copyright This file is part of the Open-ISO7816-Stack project and is distributed under the MIT license. See LICENSE file in the root directory. 
 * This file is the higher level of the abstraction layer (HAL). The code contained in this file is almost not hardware dependent.
 * TODO: The goal in the next versions is to move the last (very few) pieces of hardware dependent code from this file to reader_hal_basis.c or reader_hal_comm_settings.c or reader_periph.c.
 */

#include "reader_hal.h"
#include "reader_periph.h"



/**
 * \fn READER_Status READER_HAL_InitWithDefaults(READER_HAL_CommSettings *pSettings)
 * \return The function returns an execution code of type READER_Status that indicates if the function behaved as expected or not.
 * \param *pSettings is a pointer on a READER_HAL_CommSettings data structure that contains low level communication settings for the hardware abstraction layer.
 * This function is used to initialize the READER_HAL_CommSettings structure pointed by *pSettings to the initial default values as defined in the ISO7816-3 section 8.3.
 */ 
READER_Status READER_HAL_InitWithDefaults(READER_HAL_CommSettings *pSettings){
	READER_Status retVal;
	
	
	retVal = READER_HAL_InitHardware();
	if(retVal != READER_OK) return retVal;
	
	retVal = READER_HAL_SetGT(pSettings, READER_HAL_DEFAULT_GT);                           if(retVal != READER_OK) return retVal;
	retVal = READER_HAL_SetFi(pSettings, READER_HAL_DEFAULT_FI);                           if(retVal != READER_OK) return retVal;
	retVal = READER_HAL_SetDi(pSettings, READER_HAL_DEFAULT_DI);                           if(retVal != READER_OK) return retVal;
	retVal = READER_HAL_SetEtu(pSettings, READER_HAL_DEFAULT_FI, READER_HAL_DEFAULT_DI);   if(retVal != READER_OK) return retVal;
	retVal = READER_HAL_SetFreq(pSettings, READER_HAL_DEFAULT_FREQ);                       if(retVal != READER_OK) return retVal;

	return READER_OK;
}


/**
 * \fn READER_HAL_SendCharFrameTickstart(READER_HAL_CommSettings *pSettings, READER_HAL_Protocol protocol, uint8_t *frame, uint32_t frameSize, uint32_t timeout, uint32_t *pTickstart)
 * \return READER_Status execution code. READER_OK indicates successful execution. Any other value is an error.
 * \param *pSettings is a pointer on a READER_HAL_CommSettings data structure. It has to be previously initialized and has to contain correct comminication parameters.
 * \param protocol is a READER_HAL_Protocol value indicating wether this function is called from a T=0 or a T=1 context (it has some slight influence over the character transmission).
 * \param *frame Pointer over an uint8_t array containing teh characters to be sent.
 * \param frameSize number of characters to be sent.
 * \param timeout Timeout value to be used (in milliseconds). If set to READER_HAL_USE_ISO_WT then, the default value as defined in ISO7816-3 is going to be used.
 * \param *pTickstart is a pointer to an unint32_t value. After execution it will set the pointed variable to the "timestamp" (in milliseconds) of the leading edge of the last character of the frame being sent. 
 * This function pushes an array of bytes on the transmission line. Blocking mode function.
 */
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
			
			pTickstart = READER_HAL_GetTick() - characterDelay;  /*  10 ETU dans un caractere (10 moments) */
		}
		
		/* On fait attention a respecter le Guard Time (GT) entre les caracteres */
		//while((READER_HAL_GetTick()-tickstart) < READER_HAL_GetGTMili()){
		//	
		//}
	}
	
	
	/* On attend le flag Transmit Complete */
	/* Permet de bloquer la fonction pour empecher une eventuelle reception (avant d'avoir transmit complete) */
	
	/* TODO: Try to move this piece of code to reader_hal_basis.c or reader_hal_comm_settings.c because it is the only hardware dependent code left in this file. */
	#ifndef TEST
		while(!(USART2->SR & USART_SR_TC)){
			
		}
		
		USART2->CR1 &= ~USART_CR1_TE;
	#endif
	
	return READER_OK;
}


/**
 * \fn READER_Status READER_HAL_SendCharFrame(READER_HAL_CommSettings *pSettings, READER_HAL_Protocol protocol, uint8_t *frame, uint32_t frameSize, uint32_t timeout)
 * \return READER_Status execution code. READER_OK indicates successful execution. Any other value is an error.
 * \param *pSettings is a pointer on a READER_HAL_CommSettings data structure. It has to be previously initialized and has to contain correct comminication parameters.
 * \param protocol is a READER_HAL_Protocol value indicating wether this function is called from a T=0 or a T=1 context (it has some slight influence over the character transmission).
 * \param *frame Pointer over a byte array of characters to be sent.
 * \param frameSize number of bytes to be sent.
 * \param timeout Timeout value to be used (in milliseconds). If set to READER_HAL_USE_ISO_WT then, the default value as defined in ISO7816-3 is going to be used.
 * This function pushes an array of bytes on the transmission line. Blocking mode function.
 */
READER_Status READER_HAL_SendCharFrame(READER_HAL_CommSettings *pSettings, READER_HAL_Protocol protocol, uint8_t *frame, uint32_t frameSize, uint32_t timeout){
	READER_Status retVal;
	uint32_t tickstart;
	
	
	retVal = READER_HAL_SendCharFrameTickstart(pSettings, protocol, frame, frameSize, timeout, &tickstart);
	if(retVal != READER_OK) return retVal;
	
	return READER_OK;
}


/**
 * \fn READER_HAL_RcvCharFrameCountTickstart(READER_HAL_CommSettings *pSettings, READER_HAL_Protocol protocol, uint8_t *frame, uint32_t frameSize, uint32_t *rcvCount, uint32_t timeout, uint32_t *pTickstart)
 * \return READER_Status execution code. READER_OK indicates successful execution. Any other value is an error. READER_TIMEOUT indicates that a timeout has occured.
 * \param *pSettings is a pointer on a READER_HAL_CommSettings data structure. It has to be previously initialized and has to contain correct comminication parameters.
 * \param protocol is a READER_HAL_Protocol value indicating wether this function is called from a T=0 or a T=1 context (it has some slight influence over the character transmission and reception).
 * \param *frame is a pointer to an uint8_t bytes buffer where the received data has to be copied to.
 * \param frameSize number of bytes expected to be received.
 * \param timeout Timeout value to be applied (in milliseconds).
 * \param *rcvCount is a pointer to an uint32_t value. After execution it will set the pointed variable to the number of bytes the function managed to receive.
 * \param *pTickstart is a pointer to an unint32_t value. After execution it will set the pointed variable to the "timestamp" (in milliseconds) of the leading edge of the received frame. 
 * The purpose of this function is to receive a frame of bytes from the I/O transmission line. This one is the most complete one of the set of functions having a similar purpose 
 * (READER_HAL_RcvCharFrameCount and READER_HAL_RcvCharFrame) as it returns as well the tickstart (timestamp of the leading edge of the received frame) and the number of fully received bytes.
 */
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

 
 /**
 * \fn READER_HAL_RcvCharFrameCount(READER_HAL_CommSettings *pSettings, READER_HAL_Protocol protocol, uint8_t *frame, uint32_t frameSize, uint32_t *rcvCount, uint32_t timeout)
 * \return READER_Status execution code. READER_OK indicates successful execution. Any other value is an error. READER_TIMEOUT indicates that a timeout has occured.
 * \param *pSettings is a pointer on a READER_HAL_CommSettings data structure. It has to be previously initialized and has to contain correct comminication parameters.
 * \param protocol is a READER_HAL_Protocol value indicating wether this function is called from a T=0 or a T=1 context (it has some slight influence over the character transmission and reception).
 * \param *frame is a pointer to an uint8_t bytes buffer where the received data has to be copied to.
 * \param frameSize number of bytes expected to be received.
 * \param timeout Timeout value to be applied (in milliseconds).
 * \param *rcvCount is a pointer to an uint32_t value. After execution it will set the pointed variable to the number of bytes the function managed to receive.
 * The purpose of this function is to receive a frame of bytes from the I/O transmission line. It returns as well the number of fully received bytes.
 */
READER_Status READER_HAL_RcvCharFrameCount(READER_HAL_CommSettings *pSettings, READER_HAL_Protocol protocol, uint8_t *frame, uint32_t frameSize, uint32_t *rcvCount, uint32_t timeout){
	READER_Status retVal;
	uint32_t tickstart;
	
	
	retVal = READER_HAL_RcvCharFrameCountTickstart(pSettings, protocol, frame, frameSize, rcvCount, timeout, &tickstart);
	if(retVal != READER_OK) return retVal;
	
	return READER_OK;
}





 /**
 * \fn READER_HAL_RcvCharFrame(READER_HAL_CommSettings *pSettings, READER_HAL_Protocol protocol, uint8_t *frame, uint32_t frameSize, uint32_t timeout)
 * \return READER_Status execution code. READER_OK indicates successful execution. Any other value is an error. READER_TIMEOUT indicates that a timeout has occured.
 * \param *pSettings is a pointer on a READER_HAL_CommSettings data structure. It has to be previously initialized and has to contain correct comminication parameters.
 * \param protocol is a READER_HAL_Protocol value indicating wether this function is called from a T=0 or a T=1 context (it has some slight influence over the character transmission and reception).
 * \param *frame is a pointer to an uint8_t bytes buffer where the received data has to be copied to.
 * \param frameSize number of bytes expected to be received.
 * \param timeout Timeout value to be applied (in milliseconds).
 * The purpose of this function is to receive a frame of bytes from the I/O transmission line.
 * This function is a simplified interface to the READER_HAL_RcvCharFrameCountTickstart function when there is no need for knowing the tickstart and the count of received bytes.
 * It has as well legacy purposes.
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
 * \return READER_Status execution code. READER_OK means nominal execution.
 * This function performs a cold reset procedure as defined in ISO/IEC7816-3 section 6.2.2.
 */
READER_Status READER_HAL_DoColdReset(void){
	READER_Status retVal;
	
	/* On eteint tout avant le cold reset ...  */
	retVal = READER_HAL_SetPwrLine(READER_HAL_STATE_OFF);     if(retVal != READER_OK) return retVal;
	retVal = READER_HAL_SetClkLine(READER_HAL_STATE_OFF);     if(retVal != READER_OK) return retVal;
	retVal = READER_HAL_SetRstLine(READER_HAL_STATE_OFF);     if(retVal != READER_OK) return retVal;

	/* Activation et Cold Reset */
	READER_HAL_Delay(50);
	retVal = READER_HAL_SetPwrLine(READER_HAL_STATE_ON);      if(retVal != READER_OK) return retVal;
	READER_HAL_Delay(1);                                      
	retVal = READER_HAL_SetClkLine(READER_HAL_STATE_ON);      if(retVal != READER_OK) return retVal;
	READER_HAL_Delay(1);                                      
	retVal = READER_HAL_SetRstLine(READER_HAL_STATE_ON);      if(retVal != READER_OK) return retVal;
	
	return READER_OK;
}
