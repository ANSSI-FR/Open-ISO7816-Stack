#include "main.h"
#include "reader_hal.h"
#include "stm32f4xx_hal.h"
#include "reader_atr.h"


UART_HandleTypeDef uartHandleStruct;


uint8_t pSmartcardTxBuff[100];
uint8_t pSmartcardRxBuff[100];


int main(void){
	READER_ATR_Atr atr;
	
	if(READER_HAL_Init() != READER_OK){
		READER_HAL_ErrHandler();
	}
	initUart();
	
	
	/* Activation et Cold Reset */
	HAL_Delay(50);
	READER_HAL_SetPwrLine(READER_HAL_STATE_ON);
	HAL_Delay(1);
	READER_HAL_SetClkLine(READER_HAL_STATE_ON);
	HAL_Delay(1);
	READER_HAL_SetRstLine(READER_HAL_STATE_ON);
	
	//READER_HAL_RcvCharFrame(pSmartcardRxBuff, 17);
	READER_ATR_Receive(&atr);
	//HAL_UART_Transmit_IT(&uartHandleStruct, (uint8_t*)(&(atr.K)), 4);
	HAL_Delay(5);
	HAL_UART_Transmit_IT(&uartHandleStruct, (uint8_t*)(&(atr.T1Protocol.TABytesCount)), 1);
	HAL_UART_Transmit_IT(&uartHandleStruct, (uint8_t*)(&(atr.T1Protocol.TBBytesCount)), 1);
	HAL_UART_Transmit_IT(&uartHandleStruct, (uint8_t*)(&(atr.T1Protocol.TCBytesCount)), 1);
	
	/* Fabrication du header TPDU */
	pSmartcardTxBuff[0] = 0x00;        // CLA
	pSmartcardTxBuff[1] = 0xA4;        // INS
	pSmartcardTxBuff[2] = 0x04;        // P1
	pSmartcardTxBuff[3] = 0x00;        // P2
	pSmartcardTxBuff[4] = 0x0D;        // Lc = P3
	
	READER_HAL_SendCharFrame(pSmartcardTxBuff, 5);
	
	
	/* Attente du ACK */
	do{
		READER_HAL_RcvCharFrame(pSmartcardRxBuff, 1);
	}while(pSmartcardRxBuff[0] != 0xA4);
	
	HAL_Delay(10);
	
	
	pSmartcardTxBuff[0] = 0x48;
	pSmartcardTxBuff[1] = 0x65;
	pSmartcardTxBuff[2] = 0x6C;
	pSmartcardTxBuff[3] = 0x6C;
	pSmartcardTxBuff[4] = 0x6F;
	pSmartcardTxBuff[5] = 0x57;
	pSmartcardTxBuff[6] = 0x6F;
	pSmartcardTxBuff[7] = 0x72;
	pSmartcardTxBuff[8] = 0x6C;
	pSmartcardTxBuff[9] = 0x64;
	pSmartcardTxBuff[10] = 0x41;
	pSmartcardTxBuff[11] = 0x70;
	pSmartcardTxBuff[12] = 0x70;
	
	
	READER_HAL_SendCharFrame(pSmartcardTxBuff, 0x0D);
	
	
	
	/* Reception de SW1SW2 */
	READER_HAL_RcvCharFrame(pSmartcardRxBuff, 3);
	//HAL_UART_Transmit_IT(&uartHandleStruct, pSmartcardRxBuff, 3);

	HAL_Delay(10);
	
	/* Fabrication du header TPDU du GET RESPONSE */
	pSmartcardTxBuff[0] = 0x00;
	pSmartcardTxBuff[1] = 0xC0;
	pSmartcardTxBuff[2] = 0x00;
	pSmartcardTxBuff[3] = 0x00;
	pSmartcardTxBuff[4] = 0x12;
	
	READER_HAL_SendCharFrame(pSmartcardTxBuff, 5);
	
	
	/* Attente du ACK */
	do{
		READER_HAL_RcvCharFrame(pSmartcardRxBuff, 1);
	}while(pSmartcardRxBuff[0] != 0xC0);
	
	
	/* Reception reponse carte + SW1SW2 */
	READER_HAL_RcvCharFrame(pSmartcardRxBuff, 0x14);

	
	HAL_Delay(500);
	READER_HAL_SetFreq(3500000);

	while(1){
		
	}
	
	return 0;
}






void initUart(void){
	uartHandleStruct.Instance = USART1;
	uartHandleStruct.Init.BaudRate = 9600;
	uartHandleStruct.Init.Mode = UART_MODE_TX_RX;
	uartHandleStruct.Init.OverSampling = UART_OVERSAMPLING_16;
	uartHandleStruct.Init.Parity = UART_PARITY_NONE;
	uartHandleStruct.Init.StopBits = UART_STOPBITS_1;
	uartHandleStruct.Init.WordLength = UART_WORDLENGTH_8B;
	uartHandleStruct.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	
	GPIO_InitTypeDef gpioInitStruct;
	
	/* Initialisation pin TX */
	gpioInitStruct.Pin = GPIO_PIN_6;
	gpioInitStruct.Mode = GPIO_MODE_AF_PP;
	gpioInitStruct.Pull = GPIO_PULLUP;
	gpioInitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	gpioInitStruct.Alternate = GPIO_AF7_USART1;
	
	__HAL_RCC_GPIOA_CLK_ENABLE();
	HAL_GPIO_Init(GPIOB, &gpioInitStruct);
	
	
	/* Initialisation pin RX */
	gpioInitStruct.Pin = GPIO_PIN_7;
	gpioInitStruct.Mode = GPIO_MODE_AF_OD;
	gpioInitStruct.Pull = GPIO_PULLUP;
	gpioInitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	gpioInitStruct.Alternate = GPIO_AF7_USART1;
	
	__HAL_RCC_GPIOA_CLK_ENABLE();
	HAL_GPIO_Init(GPIOB, &gpioInitStruct);
	
	/* Activation clock bloc UART */
	__HAL_RCC_USART1_CLK_ENABLE();
	
	
	/* Configuration des Interruptions */
	HAL_NVIC_SetPriority(USART1_IRQn, 0, 1);
	HAL_NVIC_EnableIRQ(USART1_IRQn);
	
	
	HAL_UART_Init(&uartHandleStruct);
}
