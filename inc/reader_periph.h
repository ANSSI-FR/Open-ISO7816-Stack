#ifndef __READER_PERIPH_H__
#define __READER_PERIPH_H__



#include "stm32f4xx_hal.h"
#include "reader_utils.h"
#include "reader.h"
#include "reader_hal_comm_settings.h"


#define READER_PERIPH_CLK_PIN          GPIO_PIN_4
#define READER_PERIPH_IO_PIN           GPIO_PIN_2
#define READER_PERIPH_RST_PIN          GPIO_PIN_5
#define READER_PERIPH_PWR_PIN          GPIO_PIN_6

#define READER_PERIPH_CLK_PORT         GPIOA
#define READER_PERIPH_IO_PORT          GPIOA
#define READER_PERIPH_RST_PORT         GPIOA
#define READER_PERIPH_PWR_PORT         GPIOA




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