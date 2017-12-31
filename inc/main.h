#ifndef _MAIN_H_
#define _MAIN_H_


#define PIN_LED_VERTE GPIO_PIN_12       /* PD12 */
#define PIN_LED_ROUGE GPIO_PIN_14       /* PD14 */
#define PIN_BOUTON_BLEU GPIO_PIN_0      /* PA0  */



void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);
void init_usart_handle(USART_HandleTypeDef *usartHandleStruct);
void init_horloge(RCC_ClkInitStruct *RCC_ClkInitStruct, RCC_OscInitStruct *RCC_OscInitStruct);








#endif
