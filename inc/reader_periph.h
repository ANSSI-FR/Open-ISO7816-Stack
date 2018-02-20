#ifndef __READER_PERIPH_H__
#define __READER_PERIPH_H__

#include "stm32f4xx_hal.h"


#define READER_PERIPH_CLK_PIN          GPIO_PIN_4
#define READER_PERIPH_IO_PIN           GPIO_PIN_2
#define READER_PERIPH_RST_PIN          GPIO_PIN_5
#define READER_PERIPH_PWR_PIN          GPIO_PIN_6



typedef READER_PERIPH_Status enum READER_PERIPH_Status;
enum READER_PERIPH_Status{
	READER_PERIPH_OK = 1,
	READER_PERIPH_NO = 0,
	READER_PERIPH_ERR = 2
};

typedef READER_PERIPH_State enum READER_PERIPH_State;
enum READER_PERIPH_State{
	READER_PERIPH_STATE_ON = 1,
	READER_PERIPH_STATE_OFF = 0
}


void READER_PERIPH_Init(void);

void READER_PERIPH_InitIOLine(void);
void READER_PERIPH_InitClkLine(void);
void READER_PERIPH_InitRstLine(void);
void READER_PERIPH_InitPwrLine(void);

void READER_PERIPH_SetPwrLine(READER_PERIPH_State state);
void READER_PERIPH_SetRstLine(READER_PERIPH_State state);
void READER_PERIPH_SetClkLine(READER_PERIPH_State state);
void READER_PERIPH_SetIOLine(READER_PERIPH_State state);

void READER_PERIPH_ErrHandler(void);


#endif
