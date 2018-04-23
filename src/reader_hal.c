/**
 * \file reader_hal.c
 * \brief Contient le code des fonctions de la couche d'abstraction du matériel du lecteur.
 * \author B. Simunovic
 * \date 28 mars 2018
 */


#include "reader_hal.h"
#include "reader.h"
#include "reader_periph.h"
#include "reader_utils.h"
#include "stm32f4xx_hal.h"

#include <stdint.h>


SMARTCARD_HandleTypeDef smartcardHandleStruct;

//uint32_t globalWaitTimeMili;
READER_HAL_CommSettings globalCurrentSettings;



/**
 * \fn READER_Status READER_HAL_Init(void)
 * \brief Fonction pour initialiser la couche d'abstraction.
 * \return Valeur de type READER_Status. READER_OK si l'exécution s'est correctement déroulée. READER_ERR dans le cas contraire.
 */ 
READER_Status READER_HAL_Init(void){
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
	
	
	/* Initialisation de tous les peripheriques materiels */
	if(READER_PERIPH_Init() != READER_OK) return READER_ERR;
	
	
	/* Configuration de tous les etats initiaux des E/S */
	if(READER_HAL_SetIOLine(READER_HAL_STATE_OFF) != READER_OK)  return READER_ERR;
	if(READER_HAL_SetPwrLine(READER_HAL_STATE_OFF) != READER_OK) return READER_ERR;
	if(READER_HAL_SetRstLine(READER_HAL_STATE_OFF) != READER_OK) return READER_ERR;
	if(READER_HAL_SetClkLine(READER_HAL_STATE_OFF) != READER_OK) return READER_ERR;
	
	/* Initialisation du WT (Wait Time) */
	READER_HAL_SetWT(READER_DEFAULT_WT_MILI);
	
	return READER_OK;
}



/**
 * \fn READER_Status READER_HAL_SendCharFrame(uint8_t *frame, uint32_t frameSize, uint32_t timeout)
 * \brief Cette fonction permet de placer uen chaine d'octets sur la ligne IO. Cette fonction a un comportement bloquant.
 * \return Valeur de type READER_Status. READER_OK si l'exécution s'est correctement déroulée. READER_ERR dans le cas contraire.
 * \param *frame Pointeur sur la chaine d'octets à envoyer.
 * \param frameSize Taille de la chaine d'octets à envoyer.
 * \param timeout Valeur du timeout en milisecondes à utiliser. Si cette valeur est READER_HAL_USE_ISO_WT alors le timeout utilisé sera celui spécifié dans la norme ISO.
 */
READER_Status READER_HAL_SendCharFrame(uint8_t *frame, uint32_t frameSize, uint32_t timeout){
	uint32_t timeoutMili;
	READER_Status retVal;
	
	if(timeout == READER_HAL_USE_ISO_WT){
		timeoutMili = globalWaitTimeMili; //On prevoit un WT pour chaque byte envoye  //READER_UTILS_ComputeTimeoutMiliSec(smartcardHandleStruct.Init.BaudRate, globalWaitTimeMili);
	}
	else{
		timeoutMili = timeout;
	}
	
	retVal = HAL_SMARTCARD_Transmit(&smartcardHandleStruct, frame, frameSize, timeoutMili);
	
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


/**
 * \fn READER_HAL_RcvCharFrame(uint8_t *frame, uint32_t frameSize, uint32_t timeout)
 * \brief Cette fonction permet se se mettre à l'écoute d'une chaine d'octets sur la ligne IO. Cette fonction a un comportement bloquant.
 * \return Valeur de type READER_Status. READER_OK si l'exécution s'est correctement déroulée. READER_ERR dans le cas contraire.
 * \param *frame Pointeur sur le buffer à utiliser pour stocker les octets reçus.
 * \param frameSize Nombre d'octets à recevoir.
 * \param timeout Valeur du timeout en milisecondes à utiliser. Si cette valeur est READER_HAL_USE_ISO_WT alors le timeout utilisé sera celui spécifié dans la norme ISO.
 */
READER_Status READER_HAL_RcvCharFrame(uint8_t *frame, uint32_t frameSize, uint32_t timeout){
	uint32_t timeoutMili;
	READER_Status retVal;
	
	if(timeout == READER_HAL_USE_ISO_WT){
		timeoutMili = globalWaitTimeMili * frameSize; //On prevoit un WT pour chaque byte envoye  //READER_UTILS_ComputeTimeoutMiliSec(smartcardHandleStruct.Init.BaudRate, globalWaitTimeMili);
	}
	else{
		timeoutMili = timeout;
	}
	
	
	retVal = HAL_SMARTCARD_Receive(&smartcardHandleStruct, frame, frameSize, timeoutMili);
	
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



#ifndef SMARCARD_RX_FROM_SCRATCH
/**
 * \fn READER_Status READER_HAL_RcvChar(uint8_t *character, uint32_t timeout)
 * \brief Cette fonction permet de lire un seul octet du la ligne IO.
 * \return Valeur de type READER_Status. READER_OK si l'exécution s'est correctement déroulée. READER_ERR dans le cas contraire.
 * \param *character Pointeur vers un octet dans lequel sera stocké le caractère reçu.
 * \param timeout Valeur du timeout en milisecondes à utiliser. Si cette valeur est READER_HAL_USE_ISO_WT alors le timeout utilisé sera celui spécifié dans la norme ISO.
 */
READER_Status READER_HAL_RcvChar(uint8_t *character, uint32_t timeout){
	uint32_t timeoutMili;
	READER_Status retVal;
	
	if(timeout == READER_HAL_USE_ISO_WT){
		timeoutMili = globalWaitTimeMili; //READER_UTILS_ComputeTimeoutMiliSec(smartcardHandleStruct.Init.BaudRate, globalWaitTimeMili);
	}
	else{
		timeoutMili = timeout;
	}
	
	
	retVal = HAL_SMARTCARD_Receive(&smartcardHandleStruct, character, 1, timeoutMili);
	
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
READER_Status READER_HAL_RcvChar(uint8_t *character, uint32_t timeout){
	uint32_t timeoutMili, tickStart;
	READER_Status retVal;
	
	
	/* Calcul du timeout effectif en milisecondes */
	if(timeout == READER_HAL_USE_ISO_WT){
		timeoutMili = globalWaitTimeMili; //READER_UTILS_ComputeTimeoutMiliSec(smartcardHandleStruct.Init.BaudRate, globalWaitTimeMili);
	}
	else{
		timeoutMili = timeout;
	}
	
	
	/* Reception d'un caractere */
	/* On suppose ici que le bloc USART2 a deja ete configure en mode smartcard et qu'il est active et correctement initailise avec les bon parametres de communication */
	tickStart = HAL_GetTick();
	USART2->SR &= ~USART_SR_RXNE;
	USART2->CR1 |= USART_CR1_RE;
	while(!(USART2->SR & USART_SR_RXNE) && !(HAL_GetTick()-tickStart > timeoutMili)){
			
		}
	
	/* Quand on sort de la boucle d'attente, on verifie si on est sorti a cause d'un timeout */
	if(HAL_GetTick()-tickStart > timeoutMili){
		return READER_TIMEOUT;
	}
	
	/* On verifie Parity Error, Frame Error, Overrun Error */
	if((USART2->SR & USART_SR_FE) || (USART2->SR & USART_SR_ORE) || (USART2->SR & USART_SR_PE)){
		return READER_ERR;
	}
	
	/* On recupere la donnee recue dans le Data Register */
	*character = USART2->DR;
	
	return READER_OK;
}


#ifndef SMARTCARD_TX_FROM_SCRATCH
/**
 * \fn READER_Status READER_HAL_RcvChar(uint8_t *character, uint32_t timeout)
 * \brief Cette fonction permet d'envoyer un seul octet du la ligne IO.
 * \return Valeur de type READER_Status. READER_OK si l'exécution s'est correctement déroulée. READER_ERR dans le cas contraire.
 * \param character uint8_t qui indique le caractère à envoyer.
 * \param timeout Valeur du timeout en milisecondes à utiliser. Si cette valeur est READER_HAL_USE_ISO_WT alors le timeout utilisé sera celui spécifié dans la norme ISO.
 */
READER_Status READER_HAL_SendChar(uint8_t character, uint32_t timeout){
	uint32_t timeoutMili;
	READER_Status retVal;
	
	if(timeout == READER_HAL_USE_ISO_WT){
		timeoutMili = globalWaitTimeMili; //READER_UTILS_ComputeTimeoutMiliSec(smartcardHandleStruct.Init.BaudRate, globalWaitTimeMili);
	}
	else{
		timeoutMili = timeout;
	}
	
	
	retVal = HAL_SMARTCARD_Transmit(&smartcardHandleStruct, &character, 1, timeoutMili);
	
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

READER_Status READER_HAL_SendChar(uint8_t character, uint32_t timeout){
	uint32_t timeoutMili;
	READER_Status retVal;
	
	/* On calcule le timeout effectif (Celui fourni en parametre ou celui defini dans la norme ISO) */
	if(timeout == READER_HAL_USE_ISO_WT){
		timeoutMili = globalWaitTimeMili; //READER_UTILS_ComputeTimeoutMiliSec(smartcardHandleStruct.Init.BaudRate, globalWaitTimeMili);
	}
	else{
		timeoutMili = timeout;
	}
	
	/* On suppose ici que le bloc USART2 a deja ete configure en mode smartcard et qu'il est active et correctement initailise avec les bon parametres de communication */
	USART2->CR1 |= USART_CR1_UE;
	USART2->CR1 |= USART_CR1_TE;
	/* On attend que le buffer d'envoi soit empty */
	while(!(USART2->SR & USART_SR_TXE)){
		
	}
	/* On place le caractere dans le Data Register */
	USART2->DR = character;
	
	/* On attend le flag Transmit Complete */
	while(!(USART2->SR & USART_SR_TC)){
		
	}
}

#endif


/**
 * \fn READER_Status READER_HAL_SetFreq(uint32_t newFreq)
 * \brief Cette fonction permet de changer la fréquence de l'horloge fournie à la carte à puce.
 * \return Valeur de type READER_Status. READER_OK si l'exécution s'est correctement déroulée. READER_ERR dans le cas contraire.
 * \param newFreq uint32_t indiquant la nouvelle fréquence à adopter (en Hz). Attention, selon l'implémentation matérielle toutes les fréquences ne sont pas permises. Pour plus d'informations sur les fréquences supportées voir l'implémentation de la fonction READER_UTILS_ComputePrescFromFreq() dans le fichier "reader_utils.h". Attention l'implémentation de cette fonction varie selon la cible matérielle.
 * \param *currentSettings Pointeur vers une variable de type READER_HAL_CommSettings. Cette structure contient en permanance les parametres de communication utilisés.
 */
READER_Status READER_HAL_SetFreq(READER_HAL_CommSettings *currentSettings, uint32_t newFreq){
	READER_Status retVal;
	uint32_t oldFreq, oldBaudRate;
	uint32_t newBaudRate;
	uint32_t newWaitTime;
	
	/* On recupere la frequence et la baudrate actuel. Peut aussi etre recupere a partir des infos de *currentSettings */
	oldFreq = READER_UTILS_GetCardFreq(168000000, 1, 4, smartcardHandleStruct.Init.Prescaler);
	oldBaudRate = smartcardHandleStruct.Init.BaudRate;
	newBaudRate = READER_UTILS_ComputeNewBaudRate(oldBaudRate, oldFreq, newFreq);
	
	/* On applique les changements au bloc materiel USART */
	smartcardHandleStruct.Init.BaudRate = newBaudRate;
	smartcardHandleStruct.Init.Prescaler = READER_UTILS_ComputePrescFromFreq(newFreq);
	
	if(HAL_SMARTCARD_Init(&smartcardHandleStruct) != HAL_OK) return READER_ERR;
	
	/* On modifie en consequence la valeur du Wait Time (WT) (car WT est dependant de f) */
	newWaitTime = READER_UTILS_ComputeWT1(newFreq, currentSettings->Fi);
	retVal = READER_HAL_SetWT(newWaitTime); 
	if(retVal != READER_OK) return retVal;
	
	/* Mise a jour des informations dans la structure qui contient les parametres de communication */
	currentSettings->f = newFreq;
	
	return READER_OK;
}


/**
 * \fn READER_Status READER_HAL_SetEtu(uint32_t Fi, uint32_t Di)
 * \brief Cette fonction permet de configurer la valeur du "Elementary Time Unit" (ETU) utilisé dans les communications sur la ligne IO.
 * \return Valeur de type READER_Status. READER_OK si l'exécution s'est correctement déroulée. READER_ERR dans le cas contraire.
 * \param *currentSettings Pointeur vers une variable de type READER_HAL_CommSettings. Cette structure contient en permanance les parametres de communication utilisés.
 * \param Fi "Clock Rate Conversion Integer"
 * \param Di "Baudrate Adjustement Integer"
 */
READER_Status READER_HAL_SetEtu(READER_HAL_CommSettings *currentSettings, uint32_t Fi, uint32_t Di){
	READER_Status retVal;
	uint32_t freq, newBaudRate;
	uint32_t newWT;
	
	/* On recupere les parametres de communication actuels. On aurait aussi pu le faire a partir de la structure *currentSettings */
	freq = READER_UTILS_GetCardFreq(168000000, 1, 4, smartcardHandleStruct.Init.Prescaler);
	
	/* On calcule le nouveau baudrate qui correspond a la nouvelle config (nouveau etu) */
	newBaudRate = freq / (Fi / Di);
	
	/* On applique les changements au bloc materiel USART */
	smartcardHandleStruct.Init.BaudRate = newBaudRate;
	if(HAL_SMARTCARD_Init(&smartcardHandleStruct) != HAL_OK) return READER_ERR;
	
	/* Le changement de l'etu a pour consequence la modification du Wait Time (WT) */
	newWT = READER_UTILS_ComputeWT1(freq, Fi);
	retVal = READER_HAL_SetWT(currentSettings, newWT);
	if(retVal != READER_OK) return retVal;
	
	/* On met a jour la structure *currentSettings */
	currentSettings->Fi = Fi;
	currentSettings->Di = Di;
	
	return READER_OK;	
}



/**
 * \fn READER_Status READER_HAL_SetPwrLine(READER_HAL_State state)
 * \brief Cette fonction permet de changer l'état de la broche PWR.
 * \return Valeur de type READER_Status. READER_OK si l'exécution s'est correctement déroulée. READER_ERR dans le cas contraire.
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
 * \return Valeur de type READER_Status. READER_OK si l'exécution s'est correctement déroulée. READER_ERR dans le cas contraire.
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
 * \return Valeur de type READER_Status. READER_OK si l'exécution s'est correctement déroulée. READER_ERR dans le cas contraire.
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
 * \return Valeur de type READER_Status. READER_OK si l'exécution s'est correctement déroulée. READER_ERR dans le cas contraire.
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


/**
 * \fn READER_Status READER_HAL_SetGT(uint32_t newGT)
 * \brief Cette fonction permet de configurer le "Gard Time" (GT) à utiliser lors des communications sur la ligne IO. Le GT est défini dans la norme ISO7816-3 à la section 7.2.
 * \return Valeur de type READER_Status. READER_OK si l'exécution s'est correctement déroulée. READER_ERR dans le cas contraire.
 * \param uint32_t newGT uint32_t indiquant la nouvelle valeur de GT qu'il faut désormais utiliser. Cette valeur est un nombre entier d'ETU.
 * \param *currentSettings Pointeur vers une variable de type READER_HAL_CommSettings. Cette structure contient en permanance les parametres de communication utilisés.
 */
READER_Status READER_HAL_SetGT(READER_HAL_CommSettings *currentSettings, uint32_t newGT){
	smartcardHandleStruct.Init.GuardTime = newGT;
	
	if(HAL_SMARTCARD_Init(&smartcardHandleStruct) != HAL_OK) return READER_ERR;
	
	currentSettings->GT = newGT;
	
	return READER_OK;	
}

/**
 * \fn READER_Status READER_HAL_SetWT(uint32_t newWT)
 * \brief Cette fonction permet de configurer le "Wait Time" (WT) à utiliser lors des communications sur la ligne IO. Le GT est défini dans la norme ISO7816-3 à la section 7.2.
 * \return Valeur de type READER_Status. READER_OK si l'exécution s'est correctement déroulée. READER_ERR dans le cas contraire.
 * \param uint32_t newGT uint32_t indiquant la nouvelle valeur de WT qu'il faut désormais utiliser. Cette valeur est un nombre entier d'ETU.
 * \param *currentSettings Pointeur vers une variable de type READER_HAL_CommSettings. Cette structure contient en permanance les parametres de communication utilisés.
 */
READER_Status READER_HAL_SetWT(READER_HAL_CommSettings *currentSettings, uint32_t newWT){
	//globalWaitTimeMili = newWT;
	currentSettings->WT = newWT;
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

