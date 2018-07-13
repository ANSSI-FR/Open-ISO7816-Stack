#include "reader.h"
#include "reader_utils.h"
#include "stm32f4xx_hal.h"


uint32_t READER_UTILS_ComputeBaudRate(uint32_t freq, uint32_t Fi, uint32_t Di){
	return (uint32_t)((float)freq / ((float)Fi/(float)Di));
}

uint32_t READER_UTILS_GetCardFreq(uint32_t SYSCLK, uint32_t AHB, uint32_t APB1, uint32_t value_USART_GTPR_PSC){
	return (float)SYSCLK / (float)AHB / (float)APB1 / (float)(value_USART_GTPR_PSC * 2);   /* Voir en.DM00031020 section 30.6.7  et en.DM00031020 section 7.2 figure 21 */
}

uint32_t READER_UTILS_ComputeNewBaudRate(uint32_t oldBaudRate, uint32_t oldFreq, uint32_t newFreq){
	return (uint32_t)(((float)newFreq / (float)oldFreq) * (float)oldBaudRate);
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

uint32_t READER_UTILS_ComputeBestFreq(uint32_t maxFreq){
	if(5250000 <= maxFreq){
		return 5250000;
	}
	else if(4200000 <= maxFreq){
		return 4200000;
	}
	else if(3500000 <= maxFreq){
		return 3500000;
	}
	else if(3000000 <= maxFreq){
		return 3000000;
	}
	else{
		return 3000000;
	}
}

//uint32_t READER_UTILS_ComputeTimeoutMiliSec(uint32_t baudRate, uint32_t WT){
//	return ((1000 * WT) / baudRate) + 1;
//}


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
	/* On ajoute +1 pour eviter troncature a 0                             */
	
	return (uint32_t)(1000 * READER_DEFAULT_WI * 960 * ((float)Fi / (float)f)) + 1;
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
	return (uint32_t)(1000 * (float)(READER_DEFAULT_WI * 960 * Di) / (float)baudRate) + 1;
}




uint32_t READER_UTILS_ComputeBWTEtu(uint32_t BWI, uint32_t Fd, uint32_t f){
	/* Voir ISO7816-3 section 11.4.3 */
	uint32_t i;
	float power = 1;
	
	for(i=0; i<BWI; i++){
		power = power * 2;
	}
	
	return (uint32_t)(11 + (power*960*((float)Fd / (float)f)));
}


uint32_t READER_UTILS_ComputeBWTEtuMili(uint32_t BWT, uint32_t F, uint32_t D, uint32_t f){
	uint32_t etuMili = READER_UTILS_ComputeEtuMili(F, D, f);
	
	return BWT * etuMili;
}


uint32_t READER_UTILS_ComputeEtuMili(uint32_t F, uint32_t D, uint32_t f){
	/* Voir ISO7816-3 section 7.1 */
	return (uint32_t)(   (  F / (float)(D)  ) *   (   1 / (float)(f)   )    ) * 1000;
}
