/**
 * \file reader_hal_basis.c
 * \author Boris
 * Il a ete choisi de decomposer cette HAL en deux parties (en deux niveaux d'abstraction).
 * Ces deux parties correspondent aux fichiers reader_hal.c et reader_hal_basis.c.
 * Ce fichier (reader_hal_basis.c) sert a former des primitives pour les fonctions definies dans le fichier reader_hal.c (niveau d'abstraction superieur).
 * Le but de ce fichier est d'isoler encore plus finement les fonctions reelement dependantes du materiel.
 * En l'occurence, dans le cas de cette implementation, ce fichier est le seul a inclure la HAL de ST.
 */


//#include "reader_hal_.h"
#include "reader_hal_basis.h"
#include "reader_periph.h"
#include "stm32f4xx_hal.h"



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



/* Fonction "from scratch" pour recevoir un caractere */
READER_Status READER_HAL_RcvChar(READER_HAL_CommSettings *pSettings, READER_HAL_Protocol protocol, uint8_t *character, uint32_t timeout){
	uint32_t newTimeout, currentGTMilli, tickstart;
	uint8_t dummy;
	
	
	/* Verification elementaire des parametres ...  */
	if(timeout==0){
		return READER_BAD_ARG;
	}
	
	/* Si on a un overrun ici, c'est que des carateres parasites sont arrives avant que l'on commence a recevoir. On les elimine ...  */
	if(USART2->SR & USART_SR_ORE){
		dummy = USART2->DR;
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
	
	/* Pour prendre en compte le GT, on ajoute le GTMilli au timeout ...  */
	currentGTMilli = READER_HAL_GetGTMili(pSettings);
	newTimeout = timeout + currentGTMilli;
	
	while(!(USART2->SR & USART_SR_RXNE) && !(READER_HAL_GetTick()-tickstart >= newTimeout)){
			
	}
	
	/* Quand on sort de la boucle d'attente, on verifie si on est sorti a cause d'un timeout */
	if(READER_HAL_GetTick()-tickstart >= newTimeout){
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


void READER_HAL_Delay(uint32_t tMili){
	HAL_Delay(tMili);
}

uint32_t READER_HAL_GetTick(void){
	return (uint32_t)HAL_GetTick();
}



/**
 * \fn READER_Status READER_HAL_WaitSendComplete(READER_HAL_CommSettings *pSettings)
 * \brief Waits until a byte send process is over.
 * \param *pSettings is a pointer a #READER_HAL_CommSettings strcut containing the HAL low level communication parameters.
 * \return This function returns an #HAL_Status execution code.
 */
READER_Status READER_HAL_WaitUntilSendComplete(READER_HAL_CommSettings *pSettings){
	while(!(USART2->SR & USART_SR_TC));
	
	return READER_OK;
}
