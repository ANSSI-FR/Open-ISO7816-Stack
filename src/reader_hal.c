/**
 * \file reader_hal.c
 * \brief Contient le code des fonctions de la couche d'abstraction du matériel du lecteur.
 * \author B. Simunovic
 * \date 28 mars 2018
 */



#include "reader_hal.h"


#define SMARTCARD_TX_FROM_SCRATCH
#define SMARTCARD_RX_FROM_SCRATCH

SMARTCARD_HandleTypeDef smartcardHandleStruct;
//extern UART_HandleTypeDef uartHandleStruct;   // juste pour debug

//uint32_t globalWaitTimeMili;
//READER_HAL_CommSettings globalCurrentSettings;



READER_Status READER_HAL_InitHardware(void){
	READER_Status retVal;
	
	RCC_ClkInitTypeDef RCC_ClkInitStruct;
	RCC_OscInitTypeDef RCC_OscInitStruct;
	
	
	/* Initialisation de la HAL */
	if(HAL_Init() != HAL_OK) return READER_ERR;
	
	/* Configuration du reseau d'horloges du stm32 */
	__HAL_RCC_PWR_CLK_ENABLE();
	
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLM = 8;
	RCC_OscInitStruct.PLL.PLLN = 336;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	RCC_OscInitStruct.PLL.PLLQ = 7;	
	
	if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) return READER_ERR;


	RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;  
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2; 
	
	HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);   // Attention pas de valeur de retour
	
	
	/* Initialisation de tous les peripheriques materiels                                      */
	if(READER_PERIPH_Init() != READER_OK) return READER_ERR;
	
	
	/* Configuration de tous les etats initiaux des E/S                                        */
	if(READER_HAL_SetIOLine(READER_HAL_STATE_OFF) != READER_OK)       return READER_ERR;
	if(READER_HAL_SetPwrLine(READER_HAL_STATE_OFF) != READER_OK)      return READER_ERR;
	if(READER_HAL_SetRstLine(READER_HAL_STATE_OFF) != READER_OK)      return READER_ERR;
	if(READER_HAL_SetClkLine(READER_HAL_STATE_OFF) != READER_OK)      return READER_ERR;
	
	
	return READER_OK;
}




/**
 * \fn READER_Status READER_HAL_Init(void)
 * \brief Fonction pour initialiser la couche d'abstraction.
 * \return Valeur de type READER_Status. READER_OK si l'exécution s'est correctement déroulée. Toute autre valeur indique une erreur.
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
	uint32_t tickstart;
	uint32_t i;
	
	
	for(i=0; i<frameSize; i++){
		//tickstart = READER_HAL_GetTick();
		
		retVal = READER_HAL_SendChar(pSettings, protocol, frame[i], timeout);
		if(retVal != READER_OK) return retVal;
		
		/* On mets a jour la date du debut de l'envoi du dernier caractere de la frame. (On soustrait le temps en milisec qu'il faut pour envoyer un carac) */
		if(i == (frameSize-1)){
			tickstart = READER_HAL_GetTick() - (uint32_t)(READER_UTILS_ComputeEtuMiliFloat(READER_HAL_GetFi(pSettings), READER_HAL_GetDi(pSettings), READER_HAL_GetFreq(pSettings)) *10);  /*  10 etu dans un caractere (10 moments) */
			*pTickstart = MAX(1, tickstart);
		}
		
		/* On fait attention a respecter le Guard Time (GT) entre les caracteres */
		//while((READER_HAL_GetTick()-tickstart) < READER_HAL_GetGTMili()){
		//	
		//}
	}
	
	
	/* On attend le flag Transmit Complete */
	/* Permet de bloquer la fonction pour empecher une eventuelle reception (avant d'avoir transmit complete) */
	while(!(USART2->SR & USART_SR_TC)){
		
	}
	
	USART2->CR1 &= ~USART_CR1_TE;
	
	
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
READER_Status READER_HAL_RcvCharFrameCountTickstart(READER_HAL_CommSettings *pSettings, READER_HAL_Protocol protocol, uint8_t *frame, uint32_t frameSize, uint32_t *rcvCount, uint32_t timeout, uint32_t *tickstart){
	READER_Status retVal;
	uint32_t i = 0;
	uint8_t rcvByte;
	//uint32_t timeoutMili;
	uint32_t rcvCounter = 0;
	uint32_t dummy;
	
	
	/* Si l'utilisateur ne veut pas compter le nb de caracteres recus et qu'il mets NULL en guise de pointeur */
	if(rcvCount == NULL){
		rcvCount = &dummy;
	}
	
	
	/* Le nombre de caracteres recus est de 0 pour l'instant */
	*rcvCount = 0;
	
	/* Finalement, on considere que le GT ne concerne que l'envoi. Ici, on suppose que le WT a ete calcule intelligemment de sorte a prendre en compte l'enventuel GT ...  */
	///* Prise en compte du Guard Time (GT). On ajoute simplement le delai de GT au timeout. */
	//if(timeout == READER_HAL_USE_ISO_WT){
	//	timeoutMili = READER_HAL_GetWT() + READER_HAL_GetGTMili();
	//}
	//else{
	//	timeoutMili = timeout + READER_HAL_GetGTMili();
	//}
	
	
	while(i<frameSize){
		//tickstart = READER_HAL_GetTick();
		
		retVal = READER_HAL_RcvChar(pSettings, protocol, &rcvByte, timeout);
		if(retVal != READER_OK) return retVal;
		
		/* Si on vient de recevoir le premier caractere, alors on mets a jour le tickstart du leading edge de la frame ...  */
		if(i == (frameSize-1)){
			/* On veut la date du debut de la reception de la frame, donc on retire le temps en milisecondes que dure un caractere ...  */
			*tickstart = READER_HAL_GetTick() - (uint32_t)(READER_UTILS_ComputeEtuMili(READER_HAL_GetFi(pSettings), READER_HAL_GetDi(pSettings), READER_HAL_GetFreq(pSettings)) *10);
		}
		
		rcvCounter++;
		*rcvCount = rcvCounter;
		
		frame[i] = rcvByte;
		
		///* On prend en compte le Guard Time (GT). On attend l'ecoulement du GT avant de commencer une nouvelle reception de caractere. */
		//while((READER_HAL_GetTick()-tickstart) < READER_HAL_GetGTMili()-1   ){   /* La definition de la fonction READER_HAL_GetGTMili() garantit que la valeur >= 1 */
		//	/* On s'efforce de respecter le GT mais c'est peu evident a cause du manque de precision des delais (granularite de 1ms) */
		//	/* La fonction READER_HAL_GetGTMili() renvoie la valeur arrondie a l'entier superieur */
		//	/* Ici on ne peut pas se le permettre car si on attend 1ms trop longtemps il se peut que l'on rate la reception du caratere suivant. */
		//	/* Donc on retranche 1 & la valeur de retour de READER_HAL_GetGTMili() */
		//}
		
		/* Ou sinon on pourrait ajouter le GT au timeout ? */
		
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



#ifndef SMARTCARD_RX_FROM_SCRATCH
/**
 * \fn READER_Status READER_HAL_RcvChar(uint8_t *character, uint32_t timeout)
 * \brief Cette fonction permet de lire un seul octet du la ligne IO.
 * \return Valeur de type READER_Status. READER_OK si l'exécution s'est correctement déroulée. Toute autre valeur suggère une erreur.
 * \param *character Pointeur vers un octet dans lequel sera stocké le caractère reçu.
 * \param timeout Valeur du timeout en milisecondes à utiliser. Si cette valeur est READER_HAL_USE_ISO_WT alors le timeout utilisé sera celui spécifié dans la norme ISO.
 */
READER_Status READER_HAL_RcvChar(uint8_t *character, uint32_t timeout){
	HAL_StatusTypeDef retVal;
	
	
	if(timeout==0){
		return READER_BAD_ARG;
	}
	
	
	retVal = HAL_SMARTCARD_Receive(&smartcardHandleStruct, character, 1, timeout);
	
	switch(retVal){
		case HAL_TIMEOUT:
			return READER_TIMEOUT;
			break;
		case HAL_OK:
			return READER_OK;
			break;
		default:
			return READER_ERR;
	}
}

#else



/* Fonction "from scratch" pour recevoir un caractere */
READER_Status READER_HAL_RcvChar(READER_HAL_CommSettings *pSettings, READER_HAL_Protocol protocol, uint8_t *character, uint32_t timeout){
	uint32_t tickstart;
	
	
	if(timeout==0){
		return READER_BAD_ARG;
	}
	
	/* Reception d'un caractere */
	/* On suppose ici que le bloc USART2 a deja ete configure en mode smartcard et qu'il est active et correctement initailise avec les bon parametres de communication */
	tickstart = READER_HAL_GetTick();
	
	/* On active le bloc USART */
	USART2->CR1 |= USART_CR1_UE;
	
	/* On bloque l'envoi pendant la duree de la reception.  (Communication half duplex, RX et TX internally connected). Explication en.DM00031020 section 30.3.10 */
	USART2->CR1 &= ~USART_CR1_TE;
	
	/* On active la partie reception de l'USART */
	USART2->CR1 |= USART_CR1_RE;
	//USART2->SR &= ~USART_SR_RXNE;
	
	while(!(USART2->SR & USART_SR_RXNE) && !(READER_HAL_GetTick()-tickstart >= timeout)){
			
	}
	
	
	/* Quand on sort de la boucle d'attente, on verifie si on est sorti a cause d'un timeout */
	if(READER_HAL_GetTick()-tickstart >= timeout){
		USART2->CR1 &= ~USART_CR1_RE;
		return READER_TIMEOUT;
	}
	
	
	/* On verifie Parity Error, Frame Error, Overrun Error */
	//if( (USART2->SR & USART_SR_FE) || (USART2->SR & USART_SR_ORE) || (USART2->SR & USART_SR_PE) ){ 
	//	//HAL_UART_Transmit_IT(&uartHandleStruct, "FE", 2);
	//	*character = USART2->DR;
	//	USART2->CR1 &= ~USART_CR1_RE;
	//	return READER_ERR;
	//}
	
	
	/* On recupere la donnee recue dans le Data Register */
	*character = USART2->DR;
	
	/* Une fois la reception termine, on reactive l'envoi. */
	//USART2->CR1 |= USART_CR1_TE;
	
	/* On desactive la partie reception de l'USART. Cela evite d'avoir des Overrun si on recoit des caracteres inattendus */
	USART2->CR1 &= ~USART_CR1_RE;
	
	return READER_OK;
}

#endif

#ifndef SMARTCARD_TX_FROM_SCRATCH
/**
 * \fn READER_Status READER_HAL_RcvChar(uint8_t *character, uint32_t timeout)
 * \brief Cette fonction permet d'envoyer un seul octet du la ligne IO.
 * \return Valeur de type READER_Status. READER_OK si l'exécution s'est correctement déroulée. Toute autre valeur suggère une erreur.
 * \param character uint8_t qui indique le caractère à envoyer.
 * \param timeout Valeur du timeout en milisecondes à utiliser. Si cette valeur est READER_HAL_USE_ISO_WT alors le timeout utilisé sera celui spécifié dans la norme ISO.
 */
READER_Status READER_HAL_SendChar(uint8_t character, uint32_t timeout){
	HAL_StatusTypeDef retVal;
	
	
	if(timeout==0){
		return READER_BAD_ARG;
	}
	
	
	retVal = HAL_SMARTCARD_Transmit(&smartcardHandleStruct, &character, 1, timeout);
	
	switch(retVal){
		case HAL_TIMEOUT:
			return READER_TIMEOUT;
			break;
		case HAL_OK:
			return READER_OK;
			break;
		default:
			return READER_ERR;
	}
}

#else

READER_Status READER_HAL_SendChar(READER_HAL_CommSettings *pSettings, READER_HAL_Protocol protocol, uint8_t character, uint32_t timeout){
	uint32_t tickstart;
	uint32_t guardTime;
	
	
	if(timeout==0){
		return READER_BAD_ARG;
	}
	
	/* On active le bloc USART */
	USART2->CR1 |= USART_CR1_UE;
	
	/* On desactive la reception le temps de l'envoi. (Communication half duplex, RX et TX internally connected). Explication en.DM00031020 section 30.3.10 */
	USART2->CR1 &= ~USART_CR1_RE;
	
	/* On suppose ici que le bloc USART2 a deja ete configure en mode smartcard et qu'il est active et correctement initailise avec les bon parametres de communication */
	USART2->CR1 |= USART_CR1_TE;
	
	/* On set le GT register. Attention pour le STM32 le GT defini dans la datasheet ne correspond pas exactement a celui defini dans la norme ISO7816-S section 7.2 */
	/* Dans la norme ISO il s'agit du nombre d'etu entre les starts bits de deux caracteres consecutifs                                                              */
	/* Dans le STM32 il s'agit du delai (en nombre d'etu) a ajouter apres le stop bit du premier caractere avant d'envoyer le start bit du second                    */
	guardTime = READER_HAL_GetGT(pSettings);
	MODIFY_REG(USART2->GTPR, USART_GTPR_GT, ((guardTime-READER_HAL_DEFAULT_GT)<<8U));
	
	tickstart = READER_HAL_GetTick();
	
	/* On attend que le buffer d'envoi soit empty. On verifie aussi qu'on depasse pas timeout. */
	while(!(USART2->SR & USART_SR_TXE) && ((READER_HAL_GetTick()-tickstart < timeout))){
		
	}
	
	/* On attend le flag Transmit Complete (TC). Cela permet de respecter le Guard Time (GT) entre les caracteres                    */
	/* En effet, le bloc materiel USART du STM32 est concu de sorte a set TC lorsque la transmission est termine ET le GT est ecoule */
	/* Dans une implementation sans bloc materiel USART de ce type il faudrait faire une verification manuelle de delai              */
	while(!(USART2->SR & USART_SR_TC)){
		
	}
	
	if((READER_HAL_GetTick()-tickstart) >= timeout){
		return READER_TIMEOUT;
	}
	
		
	/* On place le caractere dans le Data Register */
	USART2->DR = character;
	
	/* On reactive la reception une fois l'envoi termine */
	/* ... En fait on ne peut pas le reactiver ici : Il faudrait en realite le faire uniquement lorsque TC (transmit complete). Or, ici on a fait le choix de quitter la fonction avant TC. */
	//USART2->CR1 |= USART_CR1_RE;
	
	return READER_OK;
}

#endif




/**
 * \fn READER_Status READER_HAL_SetPwrLine(READER_HAL_State state)
 * \brief Cette fonction permet de changer l'état de la broche PWR.
 * \return Valeur de type READER_Status. READER_OK si l'exécution s'est correctement déroulée. Toute autre valeur suggère une erreur.
 * \param state paramètre de type READER_HAL_State. Peut prendre les valeurs READER_HAL_STATE_ON et READER_HAL_STATE_OFF. Indique l'état à imposer à la broche.
 */
READER_Status READER_HAL_SetPwrLine(READER_HAL_State state){
	if(state == READER_HAL_STATE_ON){
		HAL_GPIO_WritePin(GPIOA, READER_PERIPH_PWR_PIN, GPIO_PIN_SET);
	}
	else if(state == READER_HAL_STATE_OFF){
		HAL_GPIO_WritePin(GPIOA, READER_PERIPH_PWR_PIN, GPIO_PIN_RESET);
	}
	else{
		return READER_ERR;
	}
	
	return READER_OK;
}



/**
 * \fn READER_Status READER_HAL_SetRstLine(READER_HAL_State state)
 * \brief Cette fonction permet de changer l'état de la broche RST.
 * \return Valeur de type READER_Status. READER_OK si l'exécution s'est correctement déroulée. Toute autre valeur suggère une erreur.
 * \param state paramètre de type READER_HAL_State. Peut prendre les valeurs READER_HAL_STATE_ON et READER_HAL_STATE_OFF. Indique l'état à imposer à la broche.
 */
READER_Status READER_HAL_SetRstLine(READER_HAL_State state){
	if(state == READER_HAL_STATE_ON){
		HAL_GPIO_WritePin(READER_PERIPH_RST_PORT, READER_PERIPH_RST_PIN, GPIO_PIN_SET);
	}
	else if(state == READER_HAL_STATE_OFF){
		HAL_GPIO_WritePin(READER_PERIPH_RST_PORT, READER_PERIPH_RST_PIN, GPIO_PIN_RESET);
	}
	else{
		return READER_ERR;
	}
	
	return READER_OK;
}



/**
 * \fn READER_Status READER_HAL_SetIOLine(READER_HAL_State state)
 * \brief Cette fonction permet de changer l'état de la broche IO.
 * \return Valeur de type READER_Status. READER_OK si l'exécution s'est correctement déroulée. Toute autre valeur suggère une erreur.
 * \param state paramètre de type READER_HAL_State. Peut prendre les valeurs READER_HAL_STATE_ON et READER_HAL_STATE_OFF. Indique l'état à imposer à la broche.
 */
READER_Status READER_HAL_SetIOLine(READER_HAL_State state){
	/* On veut forcer l'etat de la ligne IO, donc on deconnecte le GPIO du bloc USART. PB: Quand est ce que on rend la main au bloc usart ? */
	/* Peut etre possible de pull down la ligne meme lorsque l'UART est dessus */
	
	if(state == READER_HAL_STATE_ON){
		/*  ... Chgt alternate fct, chgt etat ... mais apres la main par l'uart est perdue */
		HAL_GPIO_WritePin(READER_PERIPH_IO_PORT, READER_PERIPH_IO_PIN, GPIO_PIN_SET);
	}
	else if(state == READER_HAL_STATE_OFF){
		HAL_GPIO_WritePin(READER_PERIPH_IO_PORT, READER_PERIPH_IO_PIN, GPIO_PIN_RESET);
	}
	else{
		return READER_ERR;
	}
	
	return READER_OK;
}


/**
 * \fn READER_Status READER_HAL_SetClkLine(READER_HAL_State state)
 * \brief Cette fonction permet de changer l'état de la broche CLK. En l'occurence elle permet d'activer ou non la génération de l'horloge.
 * \return Valeur de type READER_Status. READER_OK si l'exécution s'est correctement déroulée. Toute autre valeur suggère une erreur.
 * \param state paramètre de type READER_HAL_State. Peut prendre les valeurs READER_HAL_STATE_ON et READER_HAL_STATE_OFF. Indique l'état à imposer à la broche.
 */
READER_Status READER_HAL_SetClkLine(READER_HAL_State state){
	GPIO_InitTypeDef gpioInitStruct;
	
	if(state == READER_HAL_STATE_ON){
		/* On connecte le GPIO sur la clock du block UART */
		gpioInitStruct.Pin = READER_PERIPH_CLK_PIN;
		gpioInitStruct.Mode = GPIO_MODE_AF_PP; 
		gpioInitStruct.Pull = GPIO_PULLUP;
		//gpioInitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
		gpioInitStruct.Alternate = GPIO_AF7_USART2;
		
		__HAL_RCC_GPIOA_CLK_ENABLE();
		HAL_GPIO_Init(READER_PERIPH_CLK_PORT, &gpioInitStruct);
	}
	else if(state == READER_HAL_STATE_OFF){
		/* On connecte le GPIO sur la clock du block UART */
		gpioInitStruct.Pin = READER_PERIPH_CLK_PIN;
		gpioInitStruct.Mode = GPIO_MODE_OUTPUT_PP; 
		gpioInitStruct.Pull = GPIO_NOPULL;
		//gpioInitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
		//gpioInitStruct.Alternate = GPIO_AF7_USART2;
		
		__HAL_RCC_GPIOA_CLK_ENABLE();
		HAL_GPIO_Init(READER_PERIPH_CLK_PORT, &gpioInitStruct);
	}
	else{
		return READER_ERR;
	}
	
	return READER_OK;
}



void READER_HAL_ErrHandler(void){
	while(1){
		HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_14);
		HAL_Delay(100);
	}
}

void READER_HAL_Delay(uint32_t tMili){
	HAL_Delay(tMili);
}

uint32_t READER_HAL_GetTick(void){
	return HAL_GetTick();
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

