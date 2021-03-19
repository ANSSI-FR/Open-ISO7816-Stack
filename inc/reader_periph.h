/**
 * \file reader_periph.h
 * \copyright This file is part of the Open-ISO7816-Stack project and is distributed under the MIT license. See LICENSE file in the root directory. 
 */



#ifndef __READER_PERIPH_H__
#define __READER_PERIPH_H__


#include "reader_utils.h"
#include "reader.h"


#ifndef TEST

#include "stm32f4xx_hal.h"

#define READER_PERIPH_CLK_PIN          GPIO_PIN_4
#define READER_PERIPH_IO_PIN           GPIO_PIN_2
#define READER_PERIPH_RST_PIN          GPIO_PIN_5
#define READER_PERIPH_PWR_PIN          GPIO_PIN_6

#define READER_PERIPH_CLK_PORT         GPIOA
#define READER_PERIPH_IO_PORT          GPIOA
#define READER_PERIPH_RST_PORT         GPIOA
#define READER_PERIPH_PWR_PORT         GPIOA


#define PIN_LED_VERTE GPIO_PIN_12       /* PD12 */
#define PIN_LED_ROUGE GPIO_PIN_14       /* PD14 */
#define PIN_BOUTON_BLEU GPIO_PIN_0      /* PA0  */

#endif



typedef enum READER_PERIPH_Status READER_PERIPH_Status;
enum READER_PERIPH_Status{
	READER_PERIPH_OK = 1,
	READER_PERIPH_NO = 0,
	READER_PERIPH_ERR = 2
};


READER_Status READER_PERIPH_Init(void);

READER_Status READER_PERIPH_InitIOLine(void);
READER_Status READER_PERIPH_InitClkLine(void);
READER_Status READER_PERIPH_InitRstLine(void);
READER_Status READER_PERIPH_InitPwrLine(void);


void READER_PERIPH_ErrHandler(void);


#endif
