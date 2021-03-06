/**
 * \file reader_periph.c
 * \copyright This file is part of the Open-ISO7816-Stack project and is distributed under the MIT license. See LICENSE file in the root directory. 
 * This file provides code for initializing all the hardware peripherals being in charge of providing signals to the smartcard.
 * It includes I/O, CLK, RST, VCC.
 */


#include "stm32f4xx_hal.h"
#include "reader_periph.h"
#include "reader_utils.h"
#include "reader.h"
#include "reader_hal_comm_settings.h"


extern SMARTCARD_HandleTypeDef smartcardHandleStruct;


/**
 * \fn READER_PERIPH_InitIOLine(void)
 * \return READER_Status execution code. READER_OK indicates successful execution. Any other value is an error.
 * Initializes the GPIO peripheral in charge of the I/O transmission line.
 */
READER_Status READER_PERIPH_InitIOLine(void){
	GPIO_InitTypeDef gpioInitStruct;
	
	gpioInitStruct.Pin = READER_PERIPH_IO_PIN;
	gpioInitStruct.Mode = GPIO_MODE_AF_OD;              /* Voir en.DM00105879 section 30.3.11  - TX Open-Drain */
	gpioInitStruct.Pull = GPIO_PULLUP;
	gpioInitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	gpioInitStruct.Alternate = GPIO_AF7_USART2;
	
	__HAL_RCC_GPIOA_CLK_ENABLE();
	HAL_GPIO_Init(READER_PERIPH_IO_PORT, &gpioInitStruct);
	
	return READER_OK;
}


/**
 * \fn READER_PERIPH_InitClkLine(void)
 * \return READER_Status execution code. READER_OK indicates successful execution. Any other value is an error.
 * Initializes the GPIO peripheral in charge of providing the CLK signal to the smartcard.
 */
READER_Status READER_PERIPH_InitClkLine(void){
	GPIO_InitTypeDef gpioInitStruct;
	
	gpioInitStruct.Pin = READER_PERIPH_CLK_PIN;
	gpioInitStruct.Mode = GPIO_MODE_OUTPUT_PP; 
	gpioInitStruct.Pull = GPIO_NOPULL;
	//gpioInitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
	//gpioInitStruct.Alternate = GPIO_AF7_USART2;
	
	__HAL_RCC_GPIOA_CLK_ENABLE();
	HAL_GPIO_Init(READER_PERIPH_CLK_PORT, &gpioInitStruct);
	
	return READER_OK;
}


/**
 * \fn READER_PERIPH_InitRstLine(void)
 * \return READER_Status execution code. READER_OK indicates successful execution. Any other value is an error.
 * Initializes the GPIO peripheral in charge of providing the RST signal to teh smartcard.
 */
READER_Status READER_PERIPH_InitRstLine(void){
	GPIO_InitTypeDef gpioInitStruct;
	
	gpioInitStruct.Pin = READER_PERIPH_RST_PIN;
	gpioInitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	gpioInitStruct.Pull = GPIO_NOPULL;
	gpioInitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
	
	__HAL_RCC_GPIOA_CLK_ENABLE();
	HAL_GPIO_Init(READER_PERIPH_RST_PORT, &gpioInitStruct);
	
	return READER_OK;
}


/**
 * \fn READER_PERIPH_InitPwrLine(void)
 * \return READER_Status execution code. READER_OK indicates successful execution. Any other value is an error.
 * Initializes the GPIO peripheral in charge of the power supply of the smartcard.
 */
READER_Status READER_PERIPH_InitPwrLine(void){
	GPIO_InitTypeDef gpioInitStruct;
	
	gpioInitStruct.Pin = READER_PERIPH_PWR_PIN;
	gpioInitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	gpioInitStruct.Pull = GPIO_NOPULL;
	gpioInitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
	
	__HAL_RCC_GPIOA_CLK_ENABLE();
	HAL_GPIO_Init(READER_PERIPH_PWR_PORT, &gpioInitStruct);
	
	return READER_OK;
}


/**
 * \fn READER_PERIPH_Init(void)
 * \return READER_Status execution code. READER_OK indicates successful execution. Any other value is an error.
 * Initializes all the peripherals (I/O, CLK, RST, VCC).
 */
READER_Status READER_PERIPH_Init(void){
	if(READER_PERIPH_InitIOLine() != READER_OK)  return READER_ERR;
	if(READER_PERIPH_InitClkLine() != READER_OK) return READER_ERR;
	if(READER_PERIPH_InitRstLine() != READER_OK) return READER_ERR;
	if(READER_PERIPH_InitPwrLine() != READER_OK) return READER_ERR;
	
	smartcardHandleStruct.Instance = USART2;
	smartcardHandleStruct.Init.BaudRate = READER_UTILS_ComputeBaudRate(READER_HAL_DEFAULT_FREQ, READER_HAL_DEFAULT_FI, READER_HAL_DEFAULT_DI); 
	smartcardHandleStruct.Init.WordLength = SMARTCARD_WORDLENGTH_9B;
	smartcardHandleStruct.Init.StopBits = SMARTCARD_STOPBITS_1_5;
	smartcardHandleStruct.Init.Parity = SMARTCARD_PARITY_EVEN;
	smartcardHandleStruct.Init.Mode = SMARTCARD_MODE_TX_RX;
	smartcardHandleStruct.Init.CLKPolarity = SMARTCARD_POLARITY_LOW;
	smartcardHandleStruct.Init.CLKPhase = SMARTCARD_PHASE_1EDGE;
	smartcardHandleStruct.Init.CLKLastBit = SMARTCARD_LASTBIT_ENABLE;
	smartcardHandleStruct.Init.GuardTime = 0; //READER_HAL_DEFAULT_GT;
	smartcardHandleStruct.Init.NACKState = SMARTCARD_NACK_ENABLE;
	
	smartcardHandleStruct.Init.Prescaler = READER_HAL_ComputePrescFromFreq(READER_HAL_DEFAULT_FREQ);
	
	
	__HAL_RCC_USART2_CLK_ENABLE();
	if(HAL_SMARTCARD_Init(&smartcardHandleStruct) != HAL_OK) return READER_ERR;
	
	
	return READER_OK;
}


void READER_PERIPH_ErrHandler(void){
	GPIO_InitTypeDef gpioInitStruct;
	
	gpioInitStruct.Pin = GPIO_PIN_14;
	gpioInitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	gpioInitStruct.Pull = GPIO_NOPULL;
	gpioInitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
	
	__HAL_RCC_GPIOD_CLK_ENABLE();
	HAL_GPIO_Init(GPIOD, &gpioInitStruct);

	
	while(1){
		HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_14);
		HAL_Delay(100);
	}
}
