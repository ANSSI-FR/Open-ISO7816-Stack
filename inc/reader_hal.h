#ifndef __READER_HAL_H__
#define __READER_HAL_H__


#include "stm32f4xx_hal.h"
#include "reader.h"
#include "reader_utils.h"


typedef READER_HAL_State enum READER_HAL_State;
enum READER_HAL_State{
	READER_HAL_STATE_ON = 1,
	READER_HAL_STATE_OFF = 0
}




READER_Status READER_HAL_Init(void);
READER_Status READER_HAL_SendCharFrame(uint8_t *frame, uint32_t frameSize, uint32_t timeout);
READER_Status READER_HAL_RcvCharFrame(uint8_t *frame, uint32_t frameSize, uint32_t timeout);
READER_Status READER_HAL_SetClk(uint32_t freq);
READER_Status READER_HAL_SetEtu(uint32_t Fi, uint32_t Di);

READER_Status READER_HAL_SetPwrLine(READER_HAL_State state);
READER_Status READER_HAL_SetRstLine(READER_HAL_State state);
READER_Status READER_HAL_SetClkLine(READER_HAL_State state);
READER_Status READER_HAL_SetIOLine(READER_HAL_State state);

void READER_HAL_ErrHander(void);


#endif
