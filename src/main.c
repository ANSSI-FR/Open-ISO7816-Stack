#include "stm32f4xx_hal.h"
#include "stm32f4xx.h"

#include "main.h"



int main(void){	
	HAL_Init();
	
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_SET);
	
	while(1);
	
	return 0;
}
