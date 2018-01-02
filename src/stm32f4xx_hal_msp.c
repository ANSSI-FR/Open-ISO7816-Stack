/**
  ******************************************************************************
  * @file    stm32f4xx_hal_msp_template.c
  * @author  MCD Application Team
  * @brief   This file contains the HAL System and Peripheral (PPP) MSP initialization
  *          and de-initialization functions.
  *          It should be copied to the application folder and renamed into 'stm32f4xx_hal_msp.c'.           
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2017 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "main.h"
#include "card_interfaces.h"

/** @addtogroup STM32F4xx_HAL_Driver
  * @{
  */

/** @defgroup HAL_MSP HAL MSP
  * @brief HAL MSP module.
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern RCC_ClkInitTypeDef RCC_ClkInitStruct;
extern RCC_OscInitTypeDef RCC_OscInitStruct;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/** @defgroup HAL_MSP_Private_Functions HAL MSP Private Functions
  * @{
  */

/**
  * @brief  Initializes the Global MSP.
  * @note   This function is called from HAL_Init() function to perform system
  *         level initialization (GPIOs, clock, DMA, interrupt).
  * @retval None
  */
void HAL_MspInit(void)
{	
	GPIO_InitTypeDef GPIO_LedVerteInitStruct;
	GPIO_InitTypeDef GPIO_BoutonBleuInitStruct;
	
	
	/* Initialisation de l'Horloge */
	init_horloge(&RCC_ClkInitStruct, &RCC_OscInitStruct);
	
	
	/* Initialisation du pin de la led verte (board Discovery) */
	__HAL_RCC_GPIOD_CLK_ENABLE();

	GPIO_LedVerteInitStruct.Pin = PIN_LED_VERTE;
	GPIO_LedVerteInitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_LedVerteInitStruct.Pull = GPIO_NOPULL;
	GPIO_LedVerteInitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
	
	HAL_GPIO_Init(GPIOD, &GPIO_LedVerteInitStruct);
	
	
	/* Initialisation du pin de la led rouge (board Discovery) */
	__HAL_RCC_GPIOD_CLK_ENABLE();

	GPIO_LedVerteInitStruct.Pin = PIN_LED_ROUGE;
	GPIO_LedVerteInitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_LedVerteInitStruct.Pull = GPIO_NOPULL;
	GPIO_LedVerteInitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
	
	HAL_GPIO_Init(GPIOD, &GPIO_LedVerteInitStruct);
	
	
	/* Initialisation du bouton bleu (board Discovery) */
	__HAL_RCC_GPIOA_CLK_ENABLE();

	GPIO_BoutonBleuInitStruct.Pin = PIN_BOUTON_BLEU;
	GPIO_BoutonBleuInitStruct.Mode = GPIO_MODE_IT_RISING;
	GPIO_BoutonBleuInitStruct.Pull = GPIO_PULLDOWN;
	GPIO_BoutonBleuInitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
	
	HAL_GPIO_Init(GPIOA, &GPIO_BoutonBleuInitStruct);
	
	HAL_NVIC_EnableIRQ(EXTI0_IRQn);
	HAL_NVIC_SetPriority(EXTI0_IRQn, 15, 15);
}

/**
  * @brief  DeInitializes the Global MSP.
  * @note   This functiona is called from HAL_DeInit() function to perform system
  *         level de-initialization (GPIOs, clock, DMA, interrupt).
  * @retval None
  */
void HAL_MspDeInit(void)
{

}


/**
  * @brief  Initializes the PPP MSP.
  * @note   This functiona is called from HAL_PPP_Init() function to perform 
  *         peripheral(PPP) system level initialization (GPIOs, clock, DMA, interrupt)
  * @retval None
  */
void HAL_PPP_MspInit(void)
{
	
}

/**
  * @brief  DeInitializes the PPP MSP.
  * @note   This functiona is called from HAL_PPP_DeInit() function to perform 
  *         peripheral(PPP) system level de-initialization (GPIOs, clock, DMA, interrupt)
  * @retval None
  */
void HAL_PPP_MspDeInit(void)
{

}



void HAL_UART_MspInit(UART_HandleTypeDef *huart){
	GPIO_InitTypeDef gpio;
	
	/* Voir en.DM00105879 (usermanual HAL et LL) section 68.2.1 */
	/* Initialisation de l'USART 1 */
	__HAL_RCC_USART1_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	
	/* Initialisation du pin TX */
	gpio.Pin = GPIO_PIN_6;                /* Voir : https://stm32f4-discovery.net/2014/04/library-04-connect-stm32f429-discovery-to-computer-with-usart/ */
	gpio.Mode = GPIO_MODE_AF_PP;
	gpio.Pull = GPIO_PULLUP;
	gpio.Speed = GPIO_SPEED_FREQ_HIGH;
	gpio.Alternate = GPIO_AF7_USART1;     /* Voir manuel DM00105879 section 29.3.1 */
	HAL_GPIO_Init(GPIOB, &gpio);
	
	/* Initialisation du pin RX */
	gpio.Pin = GPIO_PIN_7;
	gpio.Mode = GPIO_MODE_INPUT;
	gpio.Pull = GPIO_PULLUP;
	gpio.Speed = GPIO_SPEED_FREQ_MEDIUM;
	gpio.Alternate = GPIO_AF7_USART1;
	HAL_GPIO_Init(GPIOA, &gpio);
	
	/* Config interruptions de l'USART1 */
	HAL_NVIC_SetPriority(USART1_IRQn, 0, 1);
	HAL_NVIC_EnableIRQ(USART1_IRQn);
}


void HAL_SMARTCARD_MspInit(SMARTCARD_HandleTypeDef *hsc){	
	/* Debloquer horloge du peripherique USART2 */
	__HAL_RCC_USART2_CLK_ENABLE();

	/* Configuraion pin TX */
	CARD_InitIOLine();
	
	/* Configurer pin CK */
	CARD_InitClkLine();
	
	/* Configuration des interruptions */
	HAL_NVIC_SetPriority(USART2_IRQn, 0, 1);
	HAL_NVIC_EnableIRQ(USART2_IRQn);
}

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
