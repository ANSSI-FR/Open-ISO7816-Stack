#include "main.h"
#include "reader_hal.h"
#include "stm32f4xx_hal.h"
#include "reader_atr.h"
#include "reader_tpdu.h"


UART_HandleTypeDef uartHandleStruct;
extern SMARTCARD_HandleTypeDef smartcardHandleStruct;


uint8_t pSmartcardTxBuff[100];
uint8_t pSmartcardRxBuff[100];


int main(void){
	READER_Status retVal;
	READER_ATR_Atr atr;
	READER_TPDU_Command tpdu;
	READER_TPDU_Response tpduResp;
	uint8_t SW1, SW2;
	uint8_t rcvByte1 = 0x11;
	uint8_t rcvByte2 = 0x12;
	uint8_t rcvByte3 = 0x13;
	
	
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
	
	/* Reception de l'ATR */
	READER_ATR_Receive(&atr);
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
	
	
	READER_TPDU_Forge(&tpdu, 0x00, 0xA4, 0x04, 0x00, 0x0D, pSmartcardTxBuff, 0x0D);
	READER_TPDU_Send(&tpdu, READER_HAL_USE_ISO_WT);
	READER_TPDU_RcvSW(&SW1, &SW2, READER_HAL_USE_ISO_WT);
	
	READER_HAL_Delay(10);
	
	
	READER_TPDU_Forge(&tpdu, 0x00, 0xC0, 0x00, 0x00, 0x12, NULL, 0);
	READER_TPDU_Send(&tpdu, READER_HAL_USE_ISO_WT);
	//READER_TPDU_RcvResponse(&tpduResp, 0x12, READER_HAL_USE_ISO_WT);
	retVal = READER_HAL_RcvCharFrame(pSmartcardRxBuff, 0x12, READER_HAL_USE_ISO_WT);
	//READER_TPDU_RcvSW(&SW1, &SW2, READER_HAL_USE_ISO_WT);
	//retVal = READER_HAL_RcvChar(&rcvByte1, READER_HAL_USE_ISO_WT);

	//HAL_SMARTCARD_Receive(&smartcardHandleStruct, pSmartcardRxBuff, 0x12, 1000);
	HAL_UART_Transmit(&uartHandleStruct, pSmartcardRxBuff, 0x12, 1000);
	if(retVal == READER_OK){
		HAL_UART_Transmit(&uartHandleStruct, "OK", 0x02, 1000);
	}
	else{
		HAL_UART_Transmit(&uartHandleStruct, "ERR", 0x03, 1000);
	}

	//if(retVal == READER_OK){
	//	HAL_UART_Transmit(&uartHandleStruct, pSmartcardRxBuff, 0x12, 1000);
	//}
	//else{
	//	HAL_UART_Transmit(&uartHandleStruct, "E", 0x01, 1000);
	//}
	
	
	
	//if(retVal == READER_ERR){
	//	READER_HAL_SendChar(SW1, READER_HAL_USE_ISO_WT);
	//	READER_HAL_SendChar(SW2, READER_HAL_USE_ISO_WT);
	//}
	
	//READER_HAL_SendChar(SW1, READER_HAL_USE_ISO_WT);
	//READER_HAL_SendChar(SW2, READER_HAL_USE_ISO_WT);
	
	//READER_HAL_SendCharFrame(tpduResp.dataBytes, tpduResp.dataSize, READER_HAL_USE_ISO_WT);
	//READER_HAL_SendCharFrame(pSmartcardRxBuff, 0x12, READER_HAL_USE_ISO_WT);
	
	
	
	
	
	//HAL_UART_Transmit_IT(&uartHandleStruct, (uint8_t*)(&SW), 2);
	
	
	
	//HAL_Delay(10);
	//
	///* Fabrication du header TPDU du GET RESPONSE */
	//pSmartcardTxBuff[0] = 0x00;
	//pSmartcardTxBuff[1] = 0xC0;
	//pSmartcardTxBuff[2] = 0x00;
	//pSmartcardTxBuff[3] = 0x00;
	//pSmartcardTxBuff[4] = 0x12;
	//
	//READER_HAL_SendCharFrame(pSmartcardTxBuff, 5, READER_HAL_USE_ISO_WT);
	//
	//
	///* Attente du ACK */
	//do{
	//	READER_HAL_RcvCharFrame(pSmartcardRxBuff, 1, READER_HAL_USE_ISO_WT);
	//}while(pSmartcardRxBuff[0] != 0xC0);
	//
	//
	///* Reception reponse carte + SW1SW2 */
	//READER_HAL_RcvCharFrame(pSmartcardRxBuff, 0x14, READER_HAL_USE_ISO_WT);
	//
	//
	//HAL_Delay(500);
	//READER_HAL_SetFreq(3500000);
	//
	//while(1){
	//	
	//}
	
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
	
	__HAL_RCC_GPIOB_CLK_ENABLE();
	HAL_GPIO_Init(GPIOB, &gpioInitStruct);
	
	
	/* Initialisation pin RX */
	gpioInitStruct.Pin = GPIO_PIN_7;
	gpioInitStruct.Mode = GPIO_MODE_AF_OD;
	gpioInitStruct.Pull = GPIO_PULLUP;
	gpioInitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	gpioInitStruct.Alternate = GPIO_AF7_USART1;
	
	__HAL_RCC_GPIOB_CLK_ENABLE();
	HAL_GPIO_Init(GPIOB, &gpioInitStruct);
	
	/* Activation clock bloc UART */
	__HAL_RCC_USART1_CLK_ENABLE();
	
	
	/* Configuration des Interruptions */
	HAL_NVIC_SetPriority(USART1_IRQn, 0, 1);
	HAL_NVIC_EnableIRQ(USART1_IRQn);
	
	
	HAL_UART_Init(&uartHandleStruct);
}
