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
	//READER_ATR_Atr atr;
	//READER_APDU_Command apduCmd;
	READER_APDU_Command cmd1, cmd2, cmd3;
	READER_APDU_Response apduResp;
	READER_T1_ContextHandler context;
	
	uint8_t buff1[] = {0x45, 0x75, 0x74, 0x77, 0x74, 0x75, 0x36, 0x41, 0x70, 0x70};
	uint8_t buff2[] = {0x2b, 0x05, 0x76, 0x69, 0x2a, 0x3a, 0x1e, 0x0b, 0x53, 0xb0, 0xf2, 0xdd, 0xd6, 0x99, 0x00, 0x85, 0x92, 0x5c, 0x60, 0x95, 0x71, 0xac, 0x96, 0x37, 0xb8, 0x7c, 0xe2, 0xd8, 0x47, 0x0d, 0xae, 0x59, 0x54, 0xad, 0x76, 0x43, 0xe7, 0xe2, 0x61, 0x94, 0xbc, 0xd0, 0x00, 0x42, 0xd8, 0xac, 0x1a, 0xc4, 0x27, 0xeb, 0x53, 0xe2, 0xf1, 0xd0, 0xce, 0x22, 0x24, 0xd6, 0xc5, 0xfa, 0x4a, 0xde, 0xf9, 0x6b};
	//uint8_t buff3[] = "a017af3bd1964ec3083562bbcf33cc6b31a241ae6871e51d31801d50ef1193e9705f7998642d60bb80e3c2368f71125f3c56024fc473fbd60cfd301a5a3e73710000000000000000000000000000000000000000000000000000000000000000015686ca46f2396fde6da0dcacc1e445238419aa5201af415f4eec82ccb9ff5cdb8ef2a7a2fc7671cf639a6a3f2d1d678d2687f7c0951929b9e62167271e1f9ed200";
	
	
	
	//initUart();
	READER_T1_APDU_Init(&context);
	//READER_T0_APDU_Init();
	
	
	READER_HAL_Delay(100);
	
	
	READER_APDU_Forge(&cmd1, 0x00, 0xA4, 0x04, 0x00, 10, buff1, 0);
	READER_APDU_Forge(&cmd2, 0x00, 0x0A, 0x00, 0x00, 64, buff2, 0);
	//READER_APDU_Forge(&cmd3, 0x00, 0x00, 0x00, 0x00, 324, buff3, 400);
	
	
	READER_T1_APDU_Execute(&context, &cmd1, &apduResp);
	READER_T1_APDU_Execute(&context, &cmd2, &apduResp);
	//READER_T1_APDU_Execute(&context, &cmd3, &apduResp);
	
	
	//READER_HAL_DoColdReset();
	//READER_ATR_Receive(&atr);
	//READER_ATR_ApplySettings(&atr);
	
	
	//pSmartcardTxBuff[0] = 0x48;
	//pSmartcardTxBuff[1] = 0x65;
	//pSmartcardTxBuff[2] = 0x6C;
	//pSmartcardTxBuff[3] = 0x6C;
	//pSmartcardTxBuff[4] = 0x6F;
	//pSmartcardTxBuff[5] = 0x57;
	//pSmartcardTxBuff[6] = 0x6F;
	//pSmartcardTxBuff[7] = 0x72;
	//pSmartcardTxBuff[8] = 0x6C;
	//pSmartcardTxBuff[9] = 0x64;
	//pSmartcardTxBuff[10] = 0x41;
	//pSmartcardTxBuff[11] = 0x70;
	//pSmartcardTxBuff[12] = 0x70;
	
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
	
	//READER_APDU_Forge(&apduCmd, 0x00, 0xA4, 0x04, 0x00, 10, pSmartcardTxBuff, 16);
	//READER_T0_APDU_Execute(&apduCmd, &apduResp, READER_HAL_USE_ISO_WT);
	//retVal = READER_T1_APDU_Execute(&context, &apduCmd, &apduResp);
	//READER_PERIPH_ErrHandler();
	//if(retVal != READER_OK){
	//	READER_PERIPH_ErrHandler();
	//}
	//READER_PERIPH_ErrHandler();
	
	//READER_HAL_Delay(100);
	//
	//pSmartcardTxBuff[0] = 'A';
	//pSmartcardTxBuff[1] = 'A';
	//pSmartcardTxBuff[2] = 'A';
	//pSmartcardTxBuff[3] = 'A';
	//pSmartcardTxBuff[4] = 'A';
	//pSmartcardTxBuff[5] = 'A';
	//pSmartcardTxBuff[6] = 'A';
	//pSmartcardTxBuff[7] = 'A';
	//pSmartcardTxBuff[8] = 'A';
	//pSmartcardTxBuff[9] = 'A';
	//pSmartcardTxBuff[10] = 'A';
	//pSmartcardTxBuff[11] = 'A';
	//pSmartcardTxBuff[12] = 'A';
	//pSmartcardTxBuff[13] = 'A';
	//pSmartcardTxBuff[14] = 'A';
	//pSmartcardTxBuff[15] = 'A';
	//pSmartcardTxBuff[16] = 'A';
	//pSmartcardTxBuff[17] = 'A';
	//pSmartcardTxBuff[18] = 'A';
	//pSmartcardTxBuff[19] = 'A';
	//pSmartcardTxBuff[20] = 'B';
	//pSmartcardTxBuff[21] = 'B';
	//pSmartcardTxBuff[22] = 'B';
	//pSmartcardTxBuff[23] = 'B';
	//pSmartcardTxBuff[24] = 'B';
	//pSmartcardTxBuff[25] = 'B';
	//pSmartcardTxBuff[26] = 'B';
	//pSmartcardTxBuff[27] = 'B';
	//pSmartcardTxBuff[28] = 'B';
	//pSmartcardTxBuff[29] = 'B';
	//pSmartcardTxBuff[30] = 'B';
	//pSmartcardTxBuff[31] = 'B';
	//pSmartcardTxBuff[32] = 'B';
	//pSmartcardTxBuff[33] = 'B';
	//pSmartcardTxBuff[34] = 'B';
	//pSmartcardTxBuff[35] = 'B';
	//pSmartcardTxBuff[36] = 'B';
	//pSmartcardTxBuff[37] = 'B';
	//pSmartcardTxBuff[38] = 'B';
	//
	//
	//READER_APDU_Forge(&apduCmd, 0x00, 0xA4, 0x04, 0x00, 38, pSmartcardTxBuff, 38);
	//retVal = READER_T1_APDU_Execute(&context, &apduCmd, &apduResp);
	//
	
	//if(retVal != READER_OK){
	//	READER_PERIPH_ErrHandler();
	//}
	
	
	
	
	
	
	
	
	
	
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
