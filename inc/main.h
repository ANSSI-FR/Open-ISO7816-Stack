#include "stm32f4xx_hal.h"


#ifndef __MAIN_H__
#define __MAIN_H__


#define PIN_LED_VERTE GPIO_PIN_12       /* PD12 */
#define PIN_LED_ROUGE GPIO_PIN_14       /* PD14 */
#define PIN_BOUTON_BLEU GPIO_PIN_0      /* PA0  */



void initUart(void);

#endif
