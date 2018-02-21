#include "reader_hal.h"
#include "reader_periph.h"

#include "stm32f4xx_hal.h"


SMARTCARD_HandleTypeDef smartcardHandleStruct;


READER_Status READER_HAL_Init(void){
	RCC_ClkInitTypeDef RCC_ClkInitStruct;
	RCC_OscInitTypeDef RCC_OscInitStruct;


	/* Initialisation de tous les peripheriques materiels */
	READER_PERIPH_Init();
	
	/* Initialisation de la HAL , init du SYSCLK etc ...*/
	HAL_Init();
	
				/* Enable Power Control clock */
	__HAL_RCC_PWR_CLK_ENABLE();
  
				/* Initialisation de l'horloge */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLM = 8;
	RCC_OscInitStruct.PLL.PLLN = 336;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	RCC_OscInitStruct.PLL.PLLQ = 7;
	
	HAL_RCC_OscConfig(&RCC_OscInitStruct);
	
	
	RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;  
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2; 

	HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);
	
}



READER_Status READER_HAL_SendCharFrame(uint8_t *frame, uint32_t frameSize, uint32_t timeout){
	
}


READER_Status READER_HAL_RcvCharFrame(uint8_t *frame, uint32_t frameSize, uint32_t timeout){
	
}


READER_Status READER_HAL_SetFreq(uint32_t freq){
	
}


READER_Status READER_HAL_SetEtu(uint32_t F, uint32_t D){
	
}




void READER_HAL_SetPwrLine(READER_HAL_State state){
	if(state == READER_PERIPH_STATE_ON){
		HAL_GPIO_WritePin(GPIOA, READER_PERIPH_PWR_PIN, GPIO_PIN_SET);
	}
	else if(state == READER_PERIPH_STATE_OFF){
		HAL_GPIO_WritePin(GPIOA, READER_PERIPH_PWR_PIN, GPIO_PIN_RESET);
	}
	else{
		READER_PERIPH_ErrHandler();
	}
}



void READER_HAL_SetRstLine(READER_HAL_State state){
	if(state == READER_PERIPH_STATE_ON){
		HAL_GPIO_WritePin(GPIOA, READER_PERIPH_RST_PIN, GPIO_PIN_SET);
	}
	else if(state == READER_PERIPH_STATE_OFF){
		HAL_GPIO_WritePin(GPIOA, READER_PERIPH_RST_PIN, GPIO_PIN_RESET);
	}
	else{
		READER_PERIPH_ErrHandler();
	}
}



void READER_HAL_SetIOLine(READER_HAL_State state){
	if(state == READER_PERIPH_STATE_ON){
		/* Chgt alternate fct, chgt etat ... mais apres la main par l'uart est perdue */
	}
	else if(state == READER_PERIPH_STATE_OFF){
		
	}
	else{
		READER_PERIPH_ErrHandler();
	}
}


void READER_HAL_SetClkLine(READER_HAL_State state){
	GPIO_InitTypeDef gpioInitStruct;
	
	if(state == READER_PERIPH_STATE_ON){
		/* On connecte le GPIO sur la clock du block UART */
		gpioInitStruct.Pin = READER_PERIPH_CLK_PIN;
		gpioInitStruct.Mode = GPIO_MODE_AF_PP; 
		gpioInitStruct.Pull = GPIO_PULLUP;
		//gpioInitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
		gpioInitStruct.Alternate = GPIO_AF7_USART2;
		
		__HAL_RCC_GPIOA_CLK_ENABLE();
		HAL_GPIO_Init(READER_PERIPH_CLK_PORT, &gpioInitStruct);
	}
	else if(state == READER_PERIPH_STATE_OFF){
		/* On connecte le GPIO sur la clock du block UART */
		gpioInitStruct.Pin = READER_PERIPH_CLK_PIN;
		gpioInitStruct.Mode = GPIO_MODE_PP; 
		gpioInitStruct.Pull = GPIO_PULLUP;
		//gpioInitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
		//gpioInitStruct.Alternate = GPIO_AF7_USART2;
		
		__HAL_RCC_GPIOA_CLK_ENABLE();
		HAL_GPIO_Init(READER_PERIPH_CLK_PORT, &gpioInitStruct);
	}
	else{
		READER_HAL_ErrHandler();
	}
}
