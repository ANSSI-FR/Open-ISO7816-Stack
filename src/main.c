#include "stm32f4xx_hal.h"

#include "main.h"
#include "card_interfaces.h"


RCC_ClkInitTypeDef RCC_ClkInitStruct;
RCC_OscInitTypeDef RCC_OscInitStruct;
	
UART_HandleTypeDef uartHandleStruct;
uint8_t pUartTxBuff[100];
uint8_t pUartRxBuff[100];

SMARTCARD_HandleTypeDef smartcardHandleStruct;
uint8_t pSmartCardTxBuff[100];
uint8_t pSmartcardRxBuff[100];

uint8_t atrBuffer[33];



int main(void){		
	HAL_Init();
	
	/* Initialisation de l'UART */
	//init_uart_handle(&uartHandleStruct);
	//HAL_UART_Init(&uartHandleStruct);
	
	
	/* Initialisation des E/S */
	CARD_InitPwrLine();
	CARD_InitRstLine();
	CARD_InitClkLine();
	CARD_InitIOLine();
	
	/* Parametrage du périphérique SmartCard */
	CARD_InitSmartcardHandle(&smartcardHandleStruct);
	
	HAL_Delay(100);
	
	/* reveil de la carte et declenchement de l'ATR */
	CARD_SetRstLine(STATE_OFF);
	HAL_Delay(2);
	CARD_SetPwrLine(STATE_ON);
	HAL_Delay(2);
	HAL_SMARTCARD_Init(&smartcardHandleStruct);
	HAL_Delay(70);//HAL_Delay(4);
	CARD_SetRstLine(STATE_ON);
	
	/* Reception de l'ATR */
	HAL_SMARTCARD_Receive(&smartcardHandleStruct, atrBuffer, 33, 3000);    /* Reception de 1 + 32 octets et timeout */
	//HAL_UART_Transmit_IT(&uartHandleStruct, atrBuffer, 33);
	
	//HAL_Delay(300);
	
	/* Envoi d'une commande SELECT */
	
	/* Fabrication de la trame TPDU */
	//pSmartCardTxBuff[0] = 0x00;        // CLA
	//pSmartCardTxBuff[1] = 0xA4;        // INS
	//pSmartCardTxBuff[2] = 0x04;        // P1
	//pSmartCardTxBuff[3] = 0x00;        // P2
	//pSmartCardTxBuff[4] = 0x0D;        // Lc = P3
	//
	//pSmartCardTxBuff[5] = 0x48;
	//pSmartCardTxBuff[6] = 0x65;
	//pSmartCardTxBuff[7] = 0x6C;
	//pSmartCardTxBuff[8] = 0x6C;
	//pSmartCardTxBuff[9] = 0x6F;
	//pSmartCardTxBuff[10] = 0x57;
	//pSmartCardTxBuff[11] = 0x6F;
	//pSmartCardTxBuff[12] = 0x72;
	//pSmartCardTxBuff[13] = 0x6C;
	//pSmartCardTxBuff[14] = 0x64;
	//pSmartCardTxBuff[15] = 0x41;
	//pSmartCardTxBuff[16] = 0x70;
	//pSmartCardTxBuff[17] = 0x70;
	
	//HAL_SMARTCARD_Transmit(&smartcardHandleStruct, pSmartCardTxBuff, 18, 20000);
	//HAL_SMARTCARD_Receive(&smartcardHandleStruct, pSmartcardRxBuff, 1, 20000);
	
	//HAL_UART_Transmit_IT(&uartHandleStruct, pSmartcardRxBuff, 1);
	
	

	while(1){
	}
	
	return 0;
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
	  /* Enable Power Control clock */
	__HAL_RCC_PWR_CLK_ENABLE();
  
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

