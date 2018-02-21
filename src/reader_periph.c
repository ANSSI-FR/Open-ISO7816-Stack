#include "stm32f4xx_hal.h"
#include "reader_periph.h"


extern SMARTCARD_HandleTypeDef smartcardHandleStruct;


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





void READER_PERIPH_Init(void){
	READER_PERIPH_InitIOLine();
	READER_PERIPH_InitClkLine();
	READER_PERIPH_InitRstLine();
	READER_PERIPH_InitPwrLine();
	
	smartcardHandleStruct.Instance = USART2;
	smartcardHandleStruct.Init.BaudRate = 11290;  // Valeur Bidon
	smartcardHandleStruct.Init.WordLength = SMARTCARD_WORDLENGTH_9B;
	smartcardHandleStruct.Init.StopBits = SMARTCARD_STOPBITS_1_5;
	smartcardHandleStruct.Init.Parity = SMARTCARD_PARITY_EVEN;
	smartcardHandleStruct.Init.Mode = SMARTCARD_MODE_TX_RX;
	smartcardHandleStruct.Init.CLKPolarity = SMARTCARD_POLARITY_LOW;
	smartcardHandleStruct.Init.CLKPhase = SMARTCARD_PHASE_1EDGE;
	smartcardHandleStruct.Init.CLKLastBit = SMARTCARD_LASTBIT_ENABLE;
	smartcardHandleStruct.Init.Prescaler = SMARTCARD_PRESCALER_SYSCLK_DIV10; // Valeur Bidon
	smartcardHandleStruct.Init.GuardTime = 12;
	smartcardHandleStruct.Init.NACKState = SMARTCARD_NACK_ENABLE;
	
	HAL_SMARTCARD_Init(&smartcardHandleStruct);
}


void READER_PERIPH_ErrHandler(void){
	while(1){
		
	}
}
