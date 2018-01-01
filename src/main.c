#include "stm32f4xx_hal.h"

#include "main.h"


RCC_ClkInitTypeDef RCC_ClkInitStruct;
RCC_OscInitTypeDef RCC_OscInitStruct;
	
UART_HandleTypeDef uartHandleStruct;
uint8_t pUartTxBuff[100];
uint8_t pUartRxBuff[100];



int main(void){	
	HAL_Init();
	
	init_uart_handle(&uartHandleStruct);
	HAL_UART_Init(&uartHandleStruct);
	
	pUartTxBuff[0] = 'H';
	pUartTxBuff[1] = 'E';
	pUartTxBuff[2] = 'L';
	pUartTxBuff[3] = 'L';
	pUartTxBuff[4] = 'O';
	pUartTxBuff[5] = ' ';
	pUartTxBuff[6] = 'W';
	pUartTxBuff[7] = 'O';
	pUartTxBuff[8] = 'R';
	pUartTxBuff[9] = 'L';
	pUartTxBuff[10] = 'D';
	pUartTxBuff[11] = ' ';
	pUartTxBuff[12] = '!';
	
	pUartTxBuff[0+13] = 'H';
	pUartTxBuff[1+13] = 'E';
	pUartTxBuff[2+13] = 'L';
	pUartTxBuff[3+13] = 'L';
	pUartTxBuff[4+13] = 'O';
	pUartTxBuff[5+13] = ' ';
	pUartTxBuff[6+13] = 'W';
	pUartTxBuff[7+13] = 'O';
	pUartTxBuff[8+13] = 'R';
	pUartTxBuff[9+13] = 'L';
	pUartTxBuff[10+13] = 'D';
	pUartTxBuff[11+13] = ' ';
	pUartTxBuff[12+13] = '!';

	while(1){
		HAL_GPIO_WritePin(GPIOD, PIN_LED_VERTE, GPIO_PIN_SET);
		HAL_UART_Transmit_IT(&uartHandleStruct, pUartTxBuff, 26);
		HAL_Delay(1000);
	}
	
	return 0;
}


void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart){
	HAL_GPIO_WritePin(GPIOD, PIN_LED_VERTE, GPIO_PIN_RESET);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	HAL_GPIO_WritePin(GPIOD, PIN_LED_ROUGE, GPIO_PIN_SET);
	HAL_Delay(100);
	HAL_GPIO_WritePin(GPIOD, PIN_LED_ROUGE, GPIO_PIN_RESET);
}


void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
	if(GPIO_Pin == PIN_BOUTON_BLEU){
		HAL_GPIO_TogglePin(GPIOD, PIN_LED_VERTE);
	}
	else{
		return;
	}
}


void init_uart_handle(UART_HandleTypeDef *uartHandleStruct){
	/* Initialisation de l'UART1 */	
	uartHandleStruct->Instance = USART1;
	uartHandleStruct->Init.BaudRate = 9600;
	uartHandleStruct->Init.WordLength = UART_WORDLENGTH_8B;
	uartHandleStruct->Init.StopBits = UART_STOPBITS_1;
	uartHandleStruct->Init.Parity = UART_PARITY_NONE;
	uartHandleStruct->Init.Mode = UART_MODE_TX_RX;
	uartHandleStruct->Init.HwFlowCtl = UART_HWCONTROL_NONE;
	uartHandleStruct->Init.OverSampling = UART_OVERSAMPLING_8;
	uartHandleStruct->pTxBuffPtr = NULL;
	uartHandleStruct->TxXferSize = 0;
	uartHandleStruct->pRxBuffPtr = NULL;
	uartHandleStruct->RxXferSize = 0;
	uartHandleStruct->hdmatx = NULL;
	uartHandleStruct->hdmarx = NULL;
	uartHandleStruct->Lock = HAL_UNLOCKED;
}



void init_horloge(RCC_ClkInitTypeDef *RCC_ClkInitStruct, RCC_OscInitTypeDef *RCC_OscInitStruct){
	/* Initialisation de l'horloge */
	RCC_OscInitStruct->OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct->HSEState = RCC_HSE_ON;
	RCC_OscInitStruct->PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct->PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct->PLL.PLLM = 8;
	RCC_OscInitStruct->PLL.PLLN = 336;
	RCC_OscInitStruct->PLL.PLLP = RCC_PLLP_DIV2;
	RCC_OscInitStruct->PLL.PLLQ = 7;
	
	HAL_RCC_OscConfig(RCC_OscInitStruct);
	
	
	RCC_ClkInitStruct->ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
	RCC_ClkInitStruct->SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct->AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct->APB1CLKDivider = RCC_HCLK_DIV4;  
	RCC_ClkInitStruct->APB2CLKDivider = RCC_HCLK_DIV2; 

	HAL_RCC_ClockConfig(RCC_ClkInitStruct, FLASH_LATENCY_5);
}
