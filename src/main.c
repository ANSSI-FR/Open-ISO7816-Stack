#include "main.h"
#include "reader_hal.h"
#include "reader_t0_apdu.h"
#include "reader_t1_apdu.h"
#include "reader_atr.h"


UART_HandleTypeDef uartHandleStruct;
extern SMARTCARD_HandleTypeDef smartcardHandleStruct;


uint8_t pSmartcardTxBuff[100];
uint8_t pSmartcardRxBuff[100];


int main(void){
	READER_Status retVal;
	READER_ATR_Atr atr;
	READER_APDU_Command apduCmd;
	READER_APDU_Response apduResp;
	READER_T1_ContextHandler context;
	
	
	initUart();
	READER_T1_APDU_Init(&context);
	//READER_T0_APDU_Init();
	
	
	READER_HAL_Delay(100);
	
	
	//READER_HAL_DoColdReset();
	//READER_ATR_Receive(&atr);
	//READER_ATR_ApplySettings(&atr);
	
	
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
	
	//pSmartcardTxBuff[0] = 0xA0;
	//pSmartcardTxBuff[1] = 0x00;
	//pSmartcardTxBuff[2] = 0x00;
	//pSmartcardTxBuff[3] = 0x00;
	//pSmartcardTxBuff[4] = 0x62;
	//pSmartcardTxBuff[5] = 0x03;
	//pSmartcardTxBuff[6] = 0x01;
	//pSmartcardTxBuff[7] = 0x0C;
	//pSmartcardTxBuff[8] = 0x01;
	//pSmartcardTxBuff[9] = 0x01;
	
	//READER_PERIPH_ErrHandler();
	
	READER_APDU_Forge(&apduCmd, 0x00, 0xA4, 0x04, 0x00, 0x0D, pSmartcardTxBuff, 0x0D);
	//READER_T0_APDU_Execute(&apduCmd, &apduResp, READER_HAL_USE_ISO_WT);
	retVal = READER_T1_APDU_Execute(&context, &apduCmd, &apduResp);
	//READER_PERIPH_ErrHandler();
	//if(retVal != READER_OK){
	//	READER_PERIPH_ErrHandler();
	//}
	//READER_PERIPH_ErrHandler();
	
	//HAL_UART_Transmit(&uartHandleStruct, apduResp.dataBytes, apduResp.dataSize, 1000);
	
	//READER_APDU_Forge(&apduCmd, 0x80, 0x10, 0x00, 0x00, 0x10, pSmartcardTxBuff, 0x00);
	//READER_T0_APDU_Execute(&apduCmd, &apduResp, READER_HAL_USE_ISO_WT);
	//
	//READER_APDU_Forge(&apduCmd, 0x80, 0x12, 0x00, 0x00, 0x00, NULL, 0x10);
	//READER_T0_APDU_Execute(&apduCmd, &apduResp, READER_HAL_USE_ISO_WT);
	
	//READER_APDU_Forge(&apduCmd, 0xA0, 0xA4, 0x00, 0x00, 0x02, pSmartcardTxBuff, 0x00);
	//READER_T0_APDU_Execute(&apduCmd, &apduResp, READER_HAL_USE_ISO_WT);
	//
	//READER_APDU_Forge(&apduCmd, 0xA0, 0xC0, 0x00, 0x00, 0x00, NULL, 0x20);
	//READER_T0_APDU_Execute(&apduCmd, &apduResp, READER_HAL_USE_ISO_WT);
	//
	//READER_APDU_Forge(&apduCmd, 0xA0, 0x88, 0x00, 0x00, 0x10, pSmartcardTxBuff, 0x00);
	//READER_T0_APDU_Execute(&apduCmd, &apduResp, READER_HAL_USE_ISO_WT);
	//
	//READER_APDU_Forge(&apduCmd, 0xA0, 0xC0, 0x00, 0x00, 0x00, NULL, 0x0C);
	//READER_T0_APDU_Execute(&apduCmd, &apduResp, READER_HAL_USE_ISO_WT);
	
	//HAL_UART_Transmit(&uartHandleStruct, apduResp.dataBytes, apduResp.dataSize, 1000);

	while(1){
		READER_HAL_GetTick();
	}

	//READER_TPDU_Forge(&tpdu, 0x00, 0xA4, 0x04, 0x00, 0x0D, pSmartcardTxBuff, 0x0D);
	//READER_TPDU_Send(&tpdu, READER_HAL_USE_ISO_WT);
	//READER_TPDU_RcvSW(&SW1, &SW2, READER_HAL_USE_ISO_WT);
	
	//READER_HAL_Delay(10);
	//
	//READER_TPDU_Forge(&tpdu, 0x00, 0xC0, 0x00, 0x00, 0x12, NULL, 0);
	//READER_TPDU_Send(&tpdu, READER_HAL_USE_ISO_WT);
	//READER_TPDU_RcvResponse(&tpduResp, 0x12, READER_HAL_USE_ISO_WT);
	
	
	
	//HAL_UART_Transmit(&uartHandleStruct, tpduResp.dataBytes, 0x12, 1000);
	//HAL_UART_Transmit(&uartHandleStruct, &(tpduResp.SW1), 0x01, 1000);
	//HAL_UART_Transmit(&uartHandleStruct, &(tpduResp.SW2), 0x01, 1000);


	
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
