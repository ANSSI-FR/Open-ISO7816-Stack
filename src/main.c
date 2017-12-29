#include "stm32f4xx_hal.h"
#include "stm32f4xx.h"

#include "main.h"



int main(void){
	GPIO_InitTypeDef gpio;
	
	HAL_Init();
	

	__GPIOD_CLK_ENABLE();

	gpio.Pin = GPIO_PIN_12;
	gpio.Mode = GPIO_MODE_OUTPUT_PP;
	gpio.Pull = GPIO_NOPULL;
	gpio.Speed = GPIO_SPEED_FREQ_MEDIUM;
	
	HAL_GPIO_Init(GPIOD, &gpio);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_SET);
	
	
	
	while(1);
	
	
	return 0;
}
