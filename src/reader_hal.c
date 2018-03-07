#include "reader_hal.h"
#include "reader.h"
#include "reader_periph.h"
#include "reader_utils.h"
#include "stm32f4xx_hal.h"

#include <stdint.h>


SMARTCARD_HandleTypeDef smartcardHandleStruct;

uint32_t globalWaitTimeMili;



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



READER_Status READER_HAL_SendCharFrame(uint8_t *frame, uint32_t frameSize, uint32_t timeout){
	uint32_t timeoutMili;
	HAL_Status retVal;
	
	if(timeout == READER_HAL_USE_ISO_WT){
		timeoutMili = globalWaitTimeMili * frameSize; //On prevoit un WT pour chaque byte envoye  //READER_UTILS_ComputeTimeoutMiliSec(smartcardHandleStruct.Init.BaudRate, globalWaitTimeMili);
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


READER_Status READER_HAL_RcvCharFrame(uint8_t *frame, uint32_t frameSize, uint32_t timeout){
	uint32_t timeoutMili;
	HAL_Status retVal;
	
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


READER_Status READER_HAL_RcvChar(uint8_t *character, uint32_t timeout){
	uint32_t timeoutMili;
	HAL_Status retVal;
	
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


READER_Status READER_HAL_SendChar(uint8_t character, uint32_t timeout){
	uint32_t timeoutMili;
	HAL_Status retVal;
	
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


READER_Status READER_HAL_SetFreq(uint32_t newFreq){
	uint32_t oldFreq, oldBaudRate;
	
	oldFreq = READER_UTILS_GetCardFreq(168000000, 1, 4, smartcardHandleStruct.Init.Prescaler);
	oldBaudRate = smartcardHandleStruct.Init.BaudRate;
	
	smartcardHandleStruct.Init.BaudRate = READER_UTILS_ComputeNewBaudRate(oldBaudRate, oldFreq, newFreq);
	smartcardHandleStruct.Init.Prescaler = READER_UTILS_ComputePrescFromFreq(newFreq);
	
	if(HAL_SMARTCARD_Init(&smartcardHandleStruct) != HAL_OK) return READER_ERR;
	
	return READER_OK;
}


READER_Status READER_HAL_SetEtu(uint32_t Fi, uint32_t Di){
	uint32_t freq, newBaudRate;
	
	freq = READER_UTILS_GetCardFreq(168000000, 1, 4, smartcardHandleStruct.Init.Prescaler);
	newBaudRate = freq / (Fi / Di);
	
	smartcardHandleStruct.Init.BaudRate = newBaudRate;
	
	if(HAL_SMARTCARD_Init(&smartcardHandleStruct) != HAL_OK) return READER_ERR;
	
	return READER_OK;	
}




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


READER_Status READER_HAL_SetGT(uint32_t newGT){
	smartcardHandleStruct.Init.GuardTime = newGT;
	
	if(HAL_SMARTCARD_Init(&smartcardHandleStruct) != HAL_OK) return READER_ERR;
	
	return READER_OK;	
}


READER_Status READER_HAL_SetWT(uint32_t newWT){
	globalWaitTimeMili = newWT;
	
	return READER_OK;
}



void READER_HAL_ErrHandler(void){
	while(1){
		HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_14);
		HAL_Delay(100);
	}
}

void READER_HAL_DelayMili(uint32_t tMili){
	HAL_Delay(tMili);
}

