#include "reader.h"
#include "reader_utils.h"
#include "reader_hal_comm_settings.h"


uint32_t READER_UTILS_ComputeBaudRate(uint32_t freq, uint32_t Fi, uint32_t Di){
	return (uint32_t)((float)freq / ((float)Fi/(float)Di));
}


/**
 * \fn uint32_t READER_UTILS_GetCardFreq(uint32_t SYSCLK, uint32_t AHB, uint32_t APB1, uint32_t value_USART_GTPR_PSC)
 * \param SYSCLK is the system clock.
 * \param AHB is the value of the AHB prescaler.
 * \param APB1 is the value of the APB1 prescaler.
 * \param value_USART_GTPR_PSC is the value of the smaprtcard/usart peripheral prescaler.
 * \return Clock frequency in Hz.
 * 
 * This function computes the current frequency of the clock being shared with the smartcard.  
 * This value is computed from informations about the stm32 clock tree (system clock, AHB and APB1 prescalers and usart peripheral prescalers).
 */
uint32_t READER_UTILS_GetCardFreq(uint32_t SYSCLK, uint32_t AHB, uint32_t APB1, uint32_t value_USART_GTPR_PSC){
	return (float)SYSCLK / (float)AHB / (float)APB1 / (float)(value_USART_GTPR_PSC * 2);   /* Voir en.DM00031020 section 30.6.7  et en.DM00031020 section 7.2 figure 21 */
}


/**
 * \fn uint32_t READER_UTILS_ComputeNewBaudRate(uint32_t oldBaudRate, uint32_t oldFreq, uint32_t newFreq)
 * \param oldBaudRate is the current baudrate value being used to communicate with the smartcard.
 * \param oldFreq is the current frequency value being used to communicate with the smartcard.
 * \param newFreq is the new requested value to be used for the communication with the card.
 * \return This function returns the new baudrate value that should be applied to the USART peripheral in order to keep the same ETU value.
 * This function is used to compute the new baudrate to be applied to the usart peripheral when a change of card frequency is requested.
 * The goal is to compute the new baudrate in such a way that the ETU keeps being the same in spite of teh frequency change.
 * The reason this function exists is that the HAL librairy offered with stm32 targets doesn't offer to configure directly the ETU, it enables only setting up the baudrate.
 */
uint32_t READER_UTILS_ComputeNewBaudRate(uint32_t oldBaudRate, uint32_t oldFreq, uint32_t newFreq){
	return (uint32_t)(((float)newFreq / (float)oldFreq) * (float)oldBaudRate);
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
uint32_t READER_UTILS_ComputeWT1(uint32_t f, uint32_t Fi, uint32_t WI){
	/* WT = WI * 960 * (Fi/f)    ISO7816-3 section 10.2                    */
	/* baudRate = 1/1etu    =>   BR = 1/((F/D)*(1/F))   =>  BR = f*(D/F)   */
	/* f = BR * (F/D)                                                      */
	/* WT = (WI * 960 * D) / BR                                            */
	/* WTmili  = WT * 1000                                                 */
	/* On ajoute +1 pour eviter troncature a 0                             */
	
	return (uint32_t)(1000 * WI * 960 * ((float)Fi / (float)f)) + 1;
}

/**
 * \fn uint32_t READER_UTILS_ComputeWT2(uint32_t baudRate, uint32_t Di)
 * \brief Cette fonction permet de calculer le Wait Time (WT) en millisecondes. Il s'agit du WT défini dans la norme ISO7816-3 aux sections 7.2 et 10.2. La fonction READER_UTILS_ComputeWT2() calcule cette valeur à partir du baudrate et du "Clock rate adjustement integer" Fi.
 * \return Timeout en millisecondes qu'il faut utiliser pour respecter la norme.
 * \param f Fréquence actuellement fournie à la carte.
 * \param Fi "Clock rate conversion integer" actuellement utilisé pour communiquer. Voir ISO7816-3 section 7.1.
 */
uint32_t READER_UTILS_ComputeWT2(uint32_t baudRate, uint32_t Di, uint32_t WI){
	/* Voir explications de la fonction READER_UTILS_ComputeWT1()          */
	return (uint32_t)(1000 * (float)(WI * 960 * Di) / (float)baudRate) + 1;
}




//uint32_t READER_UTILS_ComputeBWTEtu(uint32_t BWI, uint32_t f){
//	/* Voir ISO7816-3 section 11.4.3 */
//	uint32_t i;
//	uint32_t Fd = READER_HAL_DEFAULT_FI;
//	float power = 1;
//	
//	for(i=0; i<BWI; i++){
//		power = power * 2;
//	}
//	
//	return (uint32_t)(11 + (power*960*((float)Fd / (float)f)));
//}


uint32_t READER_UTILS_ComputeBWTMili(uint32_t BWTEtu, uint32_t F, uint32_t D, uint32_t f){
	uint32_t etuMili = READER_UTILS_ComputeEtuMili(F, D, f);
	
	return BWTEtu * etuMili;
}


float READER_UTILS_ComputeEtuMiliFloat(uint32_t F, uint32_t D, uint32_t f){
	/* Voir ISO7816-3 section 7.1 */
	return (float)(   (  (float)F / (float)(D)  ) *   (   1000 / (float)(f)   )    ) ; 
}


uint32_t READER_UTILS_ComputeEtuMili(uint32_t F, uint32_t D, uint32_t f){
	/* Voir ISO7816-3 section 7.1 */
	return (uint32_t)(   (  F / (float)(D)  ) *   (   1 / (float)(f)   )    ) * 1000;
}


/* On calcule a puissance b ...  */
uint32_t READER_UTILS_Pow(uint32_t a, uint32_t b){
	if(b != 0){
		return a * READER_UTILS_Pow(a, b-1);
	}
	else{
		return 1;
	}
}
