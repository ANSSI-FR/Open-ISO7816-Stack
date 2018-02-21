#include "reader.h"
#include "reader_utils.h"


uint32_t READER_UTILS_ComputeBaudRate(uint32_t freq, uint32_t Fi, uint32_t Di){
	return freq / (Fi/Di);
}

uint32_t READER_UTILS_GetCardFreq(uint32_t SYSCLK, uint32_t AHB, uint32_t APB1, uint32_t USART_GTPR_PSC){
	return SYSCLK / AHB / APB1 / USART_GTPR_PSC;
}

uint32_t READER_UTILS_ComputeNewBaudRate(uint32_t oldBaudRate, uint32_t oldFreq, uint32_t newFreq){
	return (newFreq / oldFreq) * oldBaudRate;
}
