#include "reader.h"
#include "reader_utils.h"
#include "stm32f4xx_hal.h"


uint32_t READER_UTILS_ComputeBaudRate(uint32_t freq, uint32_t Fi, uint32_t Di){
	return freq / (Fi/Di);
}

uint32_t READER_UTILS_GetCardFreq(uint32_t SYSCLK, uint32_t AHB, uint32_t APB1, uint32_t value_USART_GTPR_PSC){
	return SYSCLK / AHB / APB1 / (value_USART_GTPR_PSC * 2);   /* Voir en.DM00031020 section 30.6.7  et en.DM00031020 section 7.2 figure 21 */
}

uint32_t READER_UTILS_ComputeNewBaudRate(uint32_t oldBaudRate, uint32_t oldFreq, uint32_t newFreq){
	return (newFreq / oldFreq) * oldBaudRate;
}

uint32_t READER_UTILS_ComputePrescFromFreq(uint32_t freq){
	switch(freq){
		case 4200000:
			return SMARTCARD_PRESCALER_SYSCLK_DIV10;
			break;
		case 3500000:
			return SMARTCARD_PRESCALER_SYSCLK_DIV12;
			break;
		case 3000000:
			return SMARTCARD_PRESCALER_SYSCLK_DIV14;
			break;
		case 5250000:
			return SMARTCARD_PRESCALER_SYSCLK_DIV8;
			break;
		default:
			return SMARTCARD_PRESCALER_SYSCLK_DIV10;
	}
}


uint32_t READER_UTILS_ComputeTimeoutMiliSec(uint32_t baudRate, uint32_t WT){
	return ((1000 * WT) / baudRate) + 1;
}


/**
 * \fn uint32_t READER_UTILS_ComputeWT1(uint32_t f, uint32_t Fi)
 * \brief Cette fonction permet de calculer le Wait Time (WT) en millisecondes. Il s'agit du WT défini dans la norme ISO7816-3 aux sections 7.2 et 10.2. La fonction READER_UTILS_ComputeWT1() calcule cette valeur à partir de la fréquence f et du "Clock rate conversion integer" Fi.
 * \return Timeout en millisecondes qu'il faut utiliser pour respecter la norme.
 * \param f Fréquence actuellement fournie à la carte.
 * \param Di "Clock rate adjustement integer" actuellement utilisé pour communiquer. Voir ISO7816-3 section 7.1.
 */
uint32_t READER_UTILS_ComputeWT1(uint32_t f, uint32_t Fi){
	/* WT = WI * 960 * (Fi/f)    ISO7816-3 section 10.2                    */
	/* baudRate = 1/1etu    =>   BR = 1/((F/D)*(1/F))   =>  BR = f*(D/F)   */
	/* f = BR * (F/D)                                                      */
	/* WT = (WI * 960 * D) / BR                                            */
	/* WTmili  = WT * 1000                                                 */
	
	return 1000 * READER_DEFAULT_WI * 960 * (Fi / f);
}

/**
 * \fn uint32_t READER_UTILS_ComputeWT2(uint32_t baudRate, uint32_t Di)
 * \brief Cette fonction permet de calculer le Wait Time (WT) en millisecondes. Il s'agit du WT défini dans la norme ISO7816-3 aux sections 7.2 et 10.2. La fonction READER_UTILS_ComputeWT2() calcule cette valeur à partir du baudrate et du "Clock rate adjustement integer" Fi.
 * \return Timeout en millisecondes qu'il faut utiliser pour respecter la norme.
 * \param f Fréquence actuellement fournie à la carte.
 * \param Fi "Clock rate conversion integer" actuellement utilisé pour communiquer. Voir ISO7816-3 section 7.1.
 */
uint32_t READER_UTILS_ComputeWT2(uint32_t baudRate, uint32_t Di){
	/* Voir explications de la fonction READER_UTILS_ComputeWT1()          */
	return 1000 * (READER_DEFAULT_WI * 960 * D) / baudRate;
}
