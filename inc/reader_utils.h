#ifndef __READER_UTILS_H__
#define __READER_UTILS_H__


uint32_t READER_UTILS_ComputeBaudRate(uint32_t freq, uint32_t Fi, uint32_t Di);
uint32_t READER_UTILS_GetCardFreq(uint32_t SYSCLK, uint32_t AHB, uint32_t APB1, uint32_t USART_GTPR_PSC);
uint32_t READER_UTILS_ComputeNewBaudRate(uint32_t oldBaudRate, uint32_t oldFreq, uint32_t newFreq);
uint32_t READER_UTILS_ComputePrescFromFreq(uint32_t freq);

#endif
