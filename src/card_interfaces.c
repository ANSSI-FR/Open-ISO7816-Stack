#include "stm32f4xx_hal.h"
#include "card_interfaces.h"



extern UART_HandleTypeDef uartHandleStruct;

extern SMARTCARD_HandleTypeDef smartcardHandleStruct;
uint8_t test[40];
uint8_t test2[] = "HELLO WOLD !  HELLO WOLD !  HELLO WOLD !\n";

void CARD_InitIOLine(void){
	GPIO_InitTypeDef gpio;
	
	gpio.Pin = CARD_IO_PIN;
	gpio.Mode = GPIO_MODE_AF_OD;              /* Voir en.DM00105879 section 30.3.11  - TX Open-Drain */
	gpio.Pull = GPIO_PULLUP;
	//gpio.Speed = GPIO_SPEED_FREQ_MEDIUM;
	gpio.Alternate = GPIO_AF7_USART2;
	
	__HAL_RCC_GPIOA_CLK_ENABLE();
	HAL_GPIO_Init(GPIOA, &gpio);
}



void CARD_InitClkLine(void){
	GPIO_InitTypeDef gpio;
	
	gpio.Pin = CARD_CLK_PIN;
	gpio.Mode = GPIO_MODE_AF_PP; 
	gpio.Pull = GPIO_PULLUP;
	//gpio.Speed = GPIO_SPEED_FREQ_MEDIUM;
	gpio.Alternate = GPIO_AF7_USART2;
	
	__HAL_RCC_GPIOA_CLK_ENABLE();
	HAL_GPIO_Init(GPIOA, &gpio);
}



void CARD_InitRstLine(void){
	GPIO_InitTypeDef gpio;
	
	gpio.Pin = CARD_RST_PIN;
	gpio.Mode = GPIO_MODE_OUTPUT_PP;
	gpio.Pull = GPIO_NOPULL;
	gpio.Speed = GPIO_SPEED_FREQ_MEDIUM;
	
	__HAL_RCC_GPIOA_CLK_ENABLE();
	HAL_GPIO_Init(GPIOA, &gpio);
}



void CARD_InitPwrLine(void){
	GPIO_InitTypeDef gpio;
	
	gpio.Pin = CARD_PWR_PIN;
	gpio.Mode = GPIO_MODE_OUTPUT_PP;
	gpio.Pull = GPIO_NOPULL;
	gpio.Speed = GPIO_SPEED_FREQ_MEDIUM;
	
	__HAL_RCC_GPIOA_CLK_ENABLE();
	HAL_GPIO_Init(GPIOA, &gpio);
}



void CARD_SetPwrLine(State pwrState){
	if(pwrState == STATE_ON){
		HAL_GPIO_WritePin(GPIOA, CARD_PWR_PIN, GPIO_PIN_SET);
	}
	else if(pwrState == STATE_OFF){
		HAL_GPIO_WritePin(GPIOA, CARD_PWR_PIN, GPIO_PIN_RESET);
	}
	else{
		CARD_ErrorHandler();
	}
}



void CARD_SetRstLine(State rstState){
	if(rstState == STATE_ON){
		HAL_GPIO_WritePin(GPIOA, CARD_RST_PIN, GPIO_PIN_SET);
	}
	else if(rstState == STATE_OFF){
		HAL_GPIO_WritePin(GPIOA, CARD_RST_PIN, GPIO_PIN_RESET);
	}
	else{
		CARD_ErrorHandler();
	}
}



void CARD_SetUSARTPeriph(State USARTState){
	if(USARTState == STATE_ON){
		CARD_InitSmartcardHandle(&smartcardHandleStruct);
		HAL_SMARTCARD_Init(&smartcardHandleStruct);
	}
	else if(USARTState == STATE_OFF){
		HAL_SMARTCARD_DeInit(&smartcardHandleStruct);
	}
	else{
		CARD_ErrorHandler();
	}
}



void CARD_ColdReset(void){
	//CARD_SetUSARTPeriph(STATE_OFF);
	//CARD_SetRstLine(STATE_ON);
	//CARD_SetPwrLine(STATE_OFF);
	//HAL_Delay(1000);
	//
	//CARD_SetRstLine(STATE_OFF);
	//HAL_Delay(4);
	//CARD_SetPwrLine(STATE_ON);
	//HAL_Delay(4);
	//CARD_SetUSARTPeriph(STATE_ON);
	//HAL_Delay(5);
	//CARD_SetRstLine(STATE_ON);
}



void CARD_InitSmartcardHandle(SMARTCARD_HandleTypeDef *smartcardHandleStruct){
	smartcardHandleStruct->Instance = USART2;
	smartcardHandleStruct->Init.BaudRate = 11290;
	smartcardHandleStruct->Init.WordLength = SMARTCARD_WORDLENGTH_9B;
	smartcardHandleStruct->Init.StopBits = SMARTCARD_STOPBITS_1_5;
	smartcardHandleStruct->Init.Parity = SMARTCARD_PARITY_EVEN;
	smartcardHandleStruct->Init.Mode = SMARTCARD_MODE_TX_RX;
	smartcardHandleStruct->Init.CLKPolarity = SMARTCARD_POLARITY_LOW;
	smartcardHandleStruct->Init.CLKPhase = SMARTCARD_PHASE_1EDGE;
	smartcardHandleStruct->Init.CLKLastBit = SMARTCARD_LASTBIT_ENABLE;
	smartcardHandleStruct->Init.Prescaler = SMARTCARD_PRESCALER_SYSCLK_DIV10;   //DIV14
	smartcardHandleStruct->Init.GuardTime = 16;
	smartcardHandleStruct->Init.NACKState = SMARTCARD_NACK_ENABLE;
	//smartcardHandleStruct->pTxBuffPtr = NULL;
	//smartcardHandleStruct->TxXferSize = 0;
	//smartcardHandleStruct->pRxBuffPtr = NULL;
	//smartcardHandleStruct->RxXferSize = 0;
	//smartcardHandleStruct->hdmatx = NULL;
	//smartcardHandleStruct->hdmarx = NULL;
	//smartcardHandleStruct->Lock = HAL_UNLOCKED;
}


void CARD_ReceiveBytes(uint8_t *rcvBuff, uint16_t buffSize){
	HAL_SMARTCARD_Receive_IT(&smartcardHandleStruct, rcvBuff, buffSize);
}


void HAL_SMARTCARD_RxCpltCallback(SMARTCARD_HandleTypeDef *hsc){
	uint32_t i;
	//CARD_ReceiveCallback(hsc->pRxBuffPtr, hsc->RxXferSize);
	
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_SET);
	//HAL_UART_Transmit_IT(&uartHandleStruct, hsc->pRxBuffPtr, hsc->RxXferSize);
	HAL_UART_Transmit_IT(&uartHandleStruct, test, 10);
	//HAL_UART_Transmit_IT(&uartHandleStruct, test2, 42);
}


__weak void CARD_ReceiveCallback(uint8_t *rcvBuff, uint16_t buffSize){
	
}


void CARD_Init(void){	
	CARD_InitPwrLine();
	CARD_InitRstLine();
	CARD_InitClkLine();
	CARD_InitIOLine();
	
	CARD_InitSmartcardHandle(&smartcardHandleStruct);
	
	HAL_Delay(100);
	
	
	CARD_SetRstLine(STATE_OFF);
	HAL_Delay(2);
	CARD_SetPwrLine(STATE_ON);
	HAL_Delay(2);
	HAL_SMARTCARD_Init(&smartcardHandleStruct);
	HAL_Delay(4);
	CARD_SetRstLine(STATE_ON);
	
	
	HAL_SMARTCARD_Receive_IT(&smartcardHandleStruct, test, 10);
	
	
	//CARD_SetRstLine(STATE_OFF);
	//CARD_SetPwrLine(STATE_ON);
	//CARD_SetUSARTPeriph(STATE_ON);
	//HAL_Delay(4);
	//CARD_SetRstLine(STATE_ON);
	//HAL_SMARTCARD_Receive_IT(&smartcardHandleStruct, test, 3);
}


void CARD_ErrorHandler(void){
	while(1);
}
