#include "stm32f4xx_hal.h"
#include "reader.h"




READER_Status READER_HAL_SendCharFrame(uint8_t *frame, uint32_t frameSize, uint32_t timeout);
READER_Status READER_HAL_RcvCharFrame(uint8_t *frame, uint32_t frameSize, uint32_t timeout);
READER_Status READER_HAL_SetClk(uint32_t freq);
READER_Status READER_HAL_SetEtu(uint32_t F, uint32_t D);
