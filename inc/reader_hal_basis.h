/**
 * \file reader_hal_basis.h
 * \author Boris
 * This file contains the prototypes of the primitives that are interacting with hardware.
 */


#ifndef __READER_HAL_BASIS_H__
#define __READER_HAL_BASIS_H__


#include "reader.h"
//#include "reader_periph.h"
#include "reader_utils.h"
#include "reader_hal_comm_settings.h"
#include <stdint.h>


/* Ici, on a uniqumeent les fonctions de la hal qui interagissent avec la hardware ...  */


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







READER_Status READER_HAL_InitHardware(void);
READER_Status READER_HAL_RcvChar(READER_HAL_CommSettings *pSettings, READER_HAL_Protocol protocol, uint8_t *character, uint32_t timeout);
READER_Status READER_HAL_SendChar(READER_HAL_CommSettings *pSettings, READER_HAL_Protocol protocol, uint8_t character, uint32_t timeout);
READER_Status READER_HAL_WaitUntilSendComplete(READER_HAL_CommSettings *pSettings);


READER_Status READER_HAL_SetPwrLine(READER_HAL_State state);
READER_Status READER_HAL_SetRstLine(READER_HAL_State state);
READER_Status READER_HAL_SetClkLine(READER_HAL_State state);
READER_Status READER_HAL_SetIOLine(READER_HAL_State state);




void READER_HAL_Delay(uint32_t tMili);
uint32_t READER_HAL_GetTick(void);


#endif
