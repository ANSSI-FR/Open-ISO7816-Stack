/**
 * \file reader_hal.h
 * \author Boris
 */


#ifndef __READER_HAL_H__
#define __READER_HAL_H__


#include "reader.h"
#include "reader_hal_basis.h"
#include "reader_utils.h"
#include "reader_atr.h"
#include <stdint.h>



READER_Status READER_HAL_InitWithDefaults(READER_HAL_CommSettings *pSettings);
READER_Status READER_HAL_InitWithATRAndDefaults(READER_HAL_CommSettings *pSettings, READER_ATR_Atr *pAtr);
READER_Status READER_HAL_SendCharFrame(READER_HAL_CommSettings *pSettings, READER_HAL_Protocol protocol, uint8_t *frame, uint32_t frameSize, uint32_t timeout);
READER_Status READER_HAL_SendCharFrameTickstart(READER_HAL_CommSettings *pSettings, READER_HAL_Protocol protocol, uint8_t *frame, uint32_t frameSize, uint32_t timeout, uint32_t *pTickstart);
READER_Status READER_HAL_RcvCharFrame(READER_HAL_CommSettings *pSettings, READER_HAL_Protocol protocol, uint8_t *frame, uint32_t frameSize, uint32_t timeout);
READER_Status READER_HAL_RcvCharFrameCount(READER_HAL_CommSettings *pSettings, READER_HAL_Protocol protocol, uint8_t *frame, uint32_t frameSize, uint32_t *rcvCount, uint32_t timeout);
READER_Status READER_HAL_RcvCharFrameCountTickstart(READER_HAL_CommSettings *pSettings, READER_HAL_Protocol protocol, uint8_t *frame, uint32_t frameSize, uint32_t *rcvCount, uint32_t timeout, uint32_t *pTickstart);

READER_Status READER_HAL_DoColdReset(void);



void READER_HAL_ErrHandler(void);


#endif
