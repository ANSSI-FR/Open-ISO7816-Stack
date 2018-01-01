#include "stm32f4xx_hal.h"


#ifndef _MAIN_H_
#define _MAIN_H_


#define PIN_LED_VERTE GPIO_PIN_12       /* PD12 */
#define PIN_LED_ROUGE GPIO_PIN_14       /* PD14 */
#define PIN_BOUTON_BLEU GPIO_PIN_0      /* PA0  */



void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);
void init_uart_handle(UART_HandleTypeDef *uartHandleStruct);
void init_horloge(RCC_ClkInitTypeDef *RCC_ClkInitStruct, RCC_OscInitTypeDef *RCC_OscInitStruct);
void init_smartcard_handle(SMARTCARD_HandleTypeDef *smartcardHandleStruct);






#endif
