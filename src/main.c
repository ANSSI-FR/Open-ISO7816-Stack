#include "stm32f4xx_hal.h"
#include "stm32f4xx.h"

#include "main.h"


RCC_ClkInitTypeDef RCC_ClkInitStruct;
RCC_OscInitTypeDef RCC_OscInitStruct;
	
USART_HandleTypeDef usartHandleStruct;
uint8_t pUsartTxBuff[100];
uint8_t pUsartRxBuff[100];



int main(void){	
	HAL_Init();
	
	pUsartTxBuff[0] = 'H';
	pUsartTxBuff[1] = 'E';
	pUsartTxBuff[2] = 'L';
	pUsartTxBuff[3] = 'L';
	pUsartTxBuff[4] = 'O';
	pUsartTxBuff[5] = ' ';
	pUsartTxBuff[6] = 'W';
	pUsartTxBuff[7] = 'O';
	pUsartTxBuff[8] = 'R';
	pUsartTxBuff[9] = 'L';
	pUsartTxBuff[10] = 'D';
	pUsartTxBuff[11] = ' ';
	pUsartTxBuff[12] = '!';
	
	pUsartTxBuff[0+13] = 'H';
	pUsartTxBuff[1+13] = 'E';
	pUsartTxBuff[2+13] = 'L';
	pUsartTxBuff[3+13] = 'L';
	pUsartTxBuff[4+13] = 'O';
	pUsartTxBuff[5+13] = ' ';
	pUsartTxBuff[6+13] = 'W';
	pUsartTxBuff[7+13] = 'O';
	pUsartTxBuff[8+13] = 'R';
	pUsartTxBuff[9+13] = 'L';
	pUsartTxBuff[10+13] = 'D';
	pUsartTxBuff[11+13] = ' ';
	pUsartTxBuff[12+13] = '!';

	while(1){
		HAL_GPIO_WritePin(GPIOD, PIN_LED_VERTE, GPIO_PIN_SET);
		HAL_USART_TransmitReceive_IT(&usartHandleStruct, pUsartTxBuff, pUsartRxBuff, 26);
		HAL_Delay(1000);
	}
	
	return 0;
}


void HAL_USART_TxCpltCallback(USART_HandleTypeDef *husart){
	HAL_GPIO_WritePin(GPIOD, PIN_LED_VERTE, GPIO_PIN_RESET);
}

void HAL_USART_RxCpltCallback(USART_HandleTypeDef *husart){
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


void init_usart_handle(USART_HandleTypeDef *usartHandleStruct){
	/* Initialisation de l'USART1 */	
	usartHandleStruct->Instance = USART1;
	usartHandleStruct->Init->BaudRate = 9600;
	usartHandleStruct->Init->WordLength = USART_WORDLENGTH_8B;
	usartHandleStruct->Init->StopBits = USART_STOPBITS_1;
	usartHandleStruct->Init->Parity = USART_PARITY_NONE;
	usartHandleStruct->Init->Mode = USART_MODE_TX_RX;
	usartHandleStruct->Init->CLKPolarity = USART_POLARITY_HIGH;
	usartHandleStruct->Init->CLKPhase = USART_PHASE_1EDGE;
	usartHandleStruct->Init->CLKLastBit = USART_LASTBIT_DISABLE;
	usartHandleStruct->pTxBuffPtr = NULL;
	usartHandleStruct->TxXferSize = 0;
	usartHandleStruct->pRxBuffPtr = NULL;
	usartHandleStruct->RxXferSize = 0;
	usartHandleStruct->hdmatx = NULL;
	usartHandleStruct->hdmarx = NULL;
	usartHandleStruct->Lock = HAL_UNLOCKED;
}



void init_horloge(RCC_ClkInitStruct *RCC_ClkInitStruct, RCC_OscInitStruct *RCC_OscInitStruct){
	/* Initialisation de l'horloge */
	RCC_OscInitStruct->OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct->HSEState = RCC_HSE_ON;
	RCC_OscInitStruct->PLL->PLLState = RCC_PLL_ON;
	RCC_OscInitStruct->PLL->PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct->PLL->PLLM = 8;
	RCC_OscInitStruct->PLL->PLLN = 336;
	RCC_OscInitStruct->PLL->PLLP = RCC_PLLP_DIV2;
	RCC_OscInitStruct->PLL->PLLQ = 7;
	
	HAL_RCC_OscConfig(&RCC_OscInitStruct);
	
	
	RCC_ClkInitStruct->ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
	RCC_ClkInitStruct->SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct->AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct->APB1CLKDivider = RCC_HCLK_DIV4;  
	RCC_ClkInitStruct->APB2CLKDivider = RCC_HCLK_DIV2; 

	HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);
}
