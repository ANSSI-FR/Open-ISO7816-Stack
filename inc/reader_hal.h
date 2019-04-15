/**
 * \file reader_hal.h
 * \brief Prototypes de la couche d'abstraction.
 * \author B. Simunovic
 * \date 28 mars 2018
 * 
 * Dans ce fichier son définis les prototypes de fonctions de la couche d'abstraction du matériel du lecteur.
 */



#ifndef __READER_HAL_H__
#define __READER_HAL_H__



#include "reader.h"
#include "reader_periph.h"
#include "reader_utils.h"
#include "reader_hal_comm_settings.h"
#include "reader_atr.h"
#include <stdint.h>





/**
 * \enum READER_HAL_State
 * \brief Type de données permettant d'indiquer un état. Cet état peut être ON ou OFF. Cet état est souvent passé en paramettre des fonctions servant à piloter des E/S du lecteur.
 * 
 */
typedef enum READER_HAL_State READER_HAL_State;
enum READER_HAL_State{
	READER_HAL_STATE_ON = 1,        /* !< Indique un état "ON"  */
	READER_HAL_STATE_OFF = 0,       /* !< Indique un état "OFF" */
	READER_HAL_STATE_AUTO = 2
};


typedef enum READER_HAL_Protocol READER_HAL_Protocol;
enum READER_HAL_Protocol{
	READER_HAL_PROTOCOL_T0 =  (uint32_t)(0x00000000),     
	READER_HAL_PROTOCOL_T1 =  (uint32_t)(0x00000001),
	READER_HAL_PROTOCOL_ANY = (uint32_t)(0x00000002)
};





READER_Status READER_HAL_InitWithDefaults(READER_HAL_CommSettings *pSettings);
READER_Status READER_HAL_InitWithATRAndDefaults(READER_HAL_CommSettings *pSettings, READER_ATR_Atr *pAtr);
READER_Status READER_HAL_InitHardware(void);
READER_Status READER_HAL_SendCharFrame(READER_HAL_CommSettings *pSettings, READER_HAL_Protocol protocol, uint8_t *frame, uint32_t frameSize, uint32_t timeout);
READER_Status READER_HAL_SendCharFrameTickstart(READER_HAL_CommSettings *pSettings, READER_HAL_Protocol protocol, uint8_t *frame, uint32_t frameSize, uint32_t timeout, uint32_t *pTickstart);
READER_Status READER_HAL_RcvCharFrame(READER_HAL_CommSettings *pSettings, READER_HAL_Protocol protocol, uint8_t *frame, uint32_t frameSize, uint32_t timeout);
READER_Status READER_HAL_RcvCharFrameCount(READER_HAL_CommSettings *pSettings, READER_HAL_Protocol protocol, uint8_t *frame, uint32_t frameSize, uint32_t *rcvCount, uint32_t timeout);
READER_Status READER_HAL_RcvCharFrameCountTickstart(READER_HAL_CommSettings *pSettings, READER_HAL_Protocol protocol, uint8_t *frame, uint32_t frameSize, uint32_t *rcvCount, uint32_t timeout, uint32_t *pTickstart);
READER_Status READER_HAL_RcvChar(READER_HAL_CommSettings *pSettings, READER_HAL_Protocol protocol, uint8_t *character, uint32_t timeout);
READER_Status READER_HAL_SendChar(READER_HAL_CommSettings *pSettings, READER_HAL_Protocol protocol, uint8_t character, uint32_t timeout);



READER_Status READER_HAL_SetPwrLine(READER_HAL_State state);
READER_Status READER_HAL_SetRstLine(READER_HAL_State state);
READER_Status READER_HAL_SetClkLine(READER_HAL_State state);
READER_Status READER_HAL_SetIOLine(READER_HAL_State state);

READER_Status READER_HAL_DoColdReset(void);

void READER_HAL_Delay(uint32_t tMili);
uint32_t READER_HAL_GetTick(void);
void READER_HAL_ErrHandler(void);


#endif
