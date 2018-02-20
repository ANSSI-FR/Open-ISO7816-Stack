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
	
	
	
	/* Initialisation de l'USART/SMARTCARD */
	uartHandleStruct.Instance = USART1;
	uartHandleStruct.Init.BaudRate = 9600;
	uartHandleStruct.Init.WordLength = UART_WORDLENGTH_8B;
	uartHandleStruct.Init.StopBits = UART_STOPBITS_1;
	uartHandleStruct.Init.Parity = UART_PARITY_NONE;
	uartHandleStruct.Init.Mode = UART_MODE_TX_RX;
	uartHandleStruct.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	uartHandleStruct.Init.OverSampling = UART_OVERSAMPLING_8;
}



READER_Status READER_HAL_SendCharFrame(uint8_t *frame, uint32_t frameSize, uint32_t timeout){
	
}


READER_Status READER_HAL_RcvCharFrame(uint8_t *frame, uint32_t frameSize, uint32_t timeout){
	
}


READER_Status READER_HAL_SetClk(uint32_t freq){
	
}


READER_Status READER_HAL_SetEtu(uint32_t F, uint32_t D){
	
}
