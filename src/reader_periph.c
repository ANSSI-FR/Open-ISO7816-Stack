#include "stm32f4xx_hal.h"
#include "reader_periph.h"


void READER_PERIPH_InitIOLine(void){
	GPIO_InitTypeDef gpioInitStruct;
	
	gpioInitStruct.Pin = READER_PERIPH_IO_PIN;
	gpioInitStruct.Mode = GPIO_MODE_AF_OD;              /* Voir en.DM00105879 section 30.3.11  - TX Open-Drain */
	gpioInitStruct.Pull = GPIO_PULLUP;
	gpioInitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	gpioInitStruct.Alternate = GPIO_AF7_USART2;
	
	__HAL_RCC_GPIOA_CLK_ENABLE();
	HAL_GPIO_Init(GPIOA, &gpioInitStruct);
}



void READER_PERIPH_InitClkLine(void){
	GPIO_InitTypeDef gpioInitStruct;
	
	gpioInitStruct.Pin = READER_PERIPH_CLK_PIN;
	gpioInitStruct.Mode = GPIO_MODE_AF_PP; 
	gpioInitStruct.Pull = GPIO_PULLUP;
	//gpioInitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
	gpioInitStruct.Alternate = GPIO_AF7_USART2;
	
	__HAL_RCC_GPIOA_CLK_ENABLE();
	HAL_GPIO_Init(GPIOA, &gpioInitStruct);
}



void READER_PERIPH_InitRstLine(void){
	GPIO_InitTypeDef gpioInitStruct;
	
	gpioInitStruct.Pin = READER_PERIPH_RST_PIN;
	gpioInitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	gpioInitStruct.Pull = GPIO_NOPULL;
	gpioInitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
	
	__HAL_RCC_GPIOA_CLK_ENABLE();
	HAL_GPIO_Init(GPIOA, &gpioInitStruct);
}



void READER_PERIPH_InitPwrLine(void){
	GPIO_InitTypeDef gpioInitStruct;
	
	gpioInitStruct.Pin = READER_PERIPH_PWR_PIN;
	gpioInitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	gpioInitStruct.Pull = GPIO_NOPULL;
	gpioInitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
	
	__HAL_RCC_GPIOA_CLK_ENABLE();
	HAL_GPIO_Init(GPIOA, &gpioInitStruct);
}



void READER_PERIPH_SetPwrLine(READER_PERIPH_State state){
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



void READER_PERIPH_SetRstLine(READER_PERIPH_State state){
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



void READER_PERIPH_SetIOLine(READER_PERIPH_State state){
	if(state == READER_PERIPH_STATE_ON){
		/* Chgt alternate fct, chgt etat ... mais apres la main par l'uart est perdue */
	}
	else if(state == READER_PERIPH_STATE_OFF){
		
	}
	else{
		READER_PERIPH_ErrHandler();
	}
}


void READER_PERIPH_SetClkLine(READER_PERIPH_State state){
	if(state == READER_PERIPH_STATE_ON){
		
	}
	else if(state == READER_PERIPH_STATE_OFF){
		
	}
	else{
		READER_PERIPH_ErrHandler();
	}
}



void READER_PERIPH_Init(void){
	READER_PERIPH_InitIOLine();
	READER_PERIPH_InitClkLine();
	READER_PERIPH_InitRstLine();
	READER_PERIPH_InitPwrLine();
}


void READER_PERIPH_ErrHandler(void){
	while(1){
		
	}
}
