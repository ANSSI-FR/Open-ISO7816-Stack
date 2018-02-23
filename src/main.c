#include "main.h"
#include "reader_hal.h"
#include "stm32f4xx_hal.h"


uint8_t pSmartCardTxBuff[100];
uint8_t pSmartcardRxBuff[100];


int main(void){	
	if(READER_HAL_Init() != READER_OK){
		READER_HAL_ErrHandler();
	}
	
	HAL_Delay(50);
	READER_HAL_SetPwrLine(READER_HAL_STATE_ON);
	HAL_Delay(1);
	READER_HAL_SetClkLine(READER_HAL_STATE_ON);
	HAL_Delay(1);
	READER_HAL_SetRstLine(READER_HAL_STATE_ON);
	
	READER_HAL_RcvCharFrame(pSmartcardRxBuff, 17);
	HAL_Delay(5);
	
	/* Fabrication du header TPDU */
	pSmartCardTxBuff[0] = 0x00;        // CLA
	pSmartCardTxBuff[1] = 0xA4;        // INS
	pSmartCardTxBuff[2] = 0x04;        // P1
	pSmartCardTxBuff[3] = 0x00;        // P2
	pSmartCardTxBuff[4] = 0x0D;        // Lc = P3
	
	READER_HAL_SendCharFrame(pSmartCardTxBuff, 5);
	
	
	/* Attente du ACK */
	do{
		READER_HAL_RcvCharFrame(pSmartcardRxBuff, 1);
	}while(pSmartcardRxBuff[0] != 0xA4);
	
	HAL_Delay(10);
	
	
	pSmartCardTxBuff[0] = 0x48;
	pSmartCardTxBuff[1] = 0x65;
	pSmartCardTxBuff[2] = 0x6C;
	pSmartCardTxBuff[3] = 0x6C;
	pSmartCardTxBuff[4] = 0x6F;
	pSmartCardTxBuff[5] = 0x57;
	pSmartCardTxBuff[6] = 0x6F;
	pSmartCardTxBuff[7] = 0x72;
	pSmartCardTxBuff[8] = 0x6C;
	pSmartCardTxBuff[9] = 0x64;
	pSmartCardTxBuff[10] = 0x41;
	pSmartCardTxBuff[11] = 0x70;
	pSmartCardTxBuff[12] = 0x70;
	
	
	READER_HAL_SendCharFrame(pSmartCardTxBuff, 0x0D);
	
	READER_HAL_SetFreq(3500000);

	while(1){
		
	}
	
	return 0;
}
