#include "stm32f4xx_hal.h"
#include "stm32f4xx.h"

#include "main.h"



int main(void){	
	HAL_Init();
	
	while(1){
		if(HAL_GPIO_ReadPin(GPIOA, PIN_BOUTON_BLEU) == GPIO_PIN_SET){
			HAL_GPIO_WritePin(GPIOD, PIN_LED_VERTE, GPIO_PIN_SET);
		}
		else{
			HAL_GPIO_WritePin(GPIOD, PIN_LED_VERTE, GPIO_PIN_RESET);
		}
	}
	
	return 0;
}



void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
	if(GPIO_Pin == PIN_BOUTON_BLEU){
		HAL_GPIO_TogglePin(GPIOD, PIN_LED_VERTE);
	}
	else{
		return;
	}
}
