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
#include "stm32f4xx_hal.h"

#include <stdint.h>


/**
 * \def READER_HAL_USE_ISO_WT
 * \brief Valeur permettant d'indiquer à uen fonction d'envoi/reception que le timeout à appliqué est le WT défini dans la norme ISO.
 * 
 * Cette constante passée en parametre à certaines fonctions d'envoi et de réception permet d'indiquer le type de timeout à utiliser.
 * Si la valeur de timeout passée en argument à une fonction d'envoi/réception est différente de READER_HAL_USE_ISO_WT alors le timout appliqué sera cette valeur en milisecondes.
 * Si la valeur de timeout passée en argument à une fonction d'envoi/réception est READER_HAL_USE_ISO_WT alors le timeout appliqué sera le "wait time" (WT) tel que défini dans la norme ISO.
 */
#define READER_HAL_USE_ISO_WT    (uint32_t)(0x00000000)



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



typedef struct READER_HAL_CommSettings READER_HAL_CommSettings;
struct READER_HAL_CommSettings{
	uint32_t f;
	uint32_t Fi;
	uint32_t Di;
	uint32_t WI;
	uint32_t WT;
	uint32_t GT;
	uint32_t BWI;
	uint32_t BWT;
	uint32_t BGT;
	uint32_t IFSC;
	uint32_t IFSD;
	uint32_t redundancyType;
};



READER_Status READER_HAL_Init(void);
READER_Status READER_HAL_SendCharFrame(uint8_t *frame, uint32_t frameSize, uint32_t timeout);
READER_Status READER_HAL_SendCharFrameTickstart(uint8_t *frame, uint32_t frameSize, uint32_t timeout, uint32_t *pTickstart);
READER_Status READER_HAL_RcvCharFrame(uint8_t *frame, uint32_t frameSize, uint32_t timeout);
READER_Status READER_HAL_RcvCharFrameCount(uint8_t *frame, uint32_t frameSize, uint32_t *rcvCount, uint32_t timeout);
READER_Status READER_HAL_RcvCharFrameCountTickstart(uint8_t *frame, uint32_t frameSize, uint32_t *rcvCount, uint32_t timeout, uint32_t *tickstart);
READER_Status READER_HAL_RcvChar(uint8_t *character, uint32_t timeout);
READER_Status READER_HAL_SendChar(uint8_t character, uint32_t timeout);

READER_Status READER_HAL_SetFreq(uint32_t newFreq);
READER_Status READER_HAL_SetEtu(uint32_t Fi, uint32_t Di);
READER_Status READER_HAL_SetGT(uint32_t newGT);
READER_Status READER_HAL_SetWT(uint32_t newWT);
READER_Status READER_HAL_SetFi(uint32_t Fi);
READER_Status READER_HAL_SetDi(uint32_t Di);
READER_Status READER_HAL_SetIFSC(uint32_t IFSC);
READER_Status READER_HAL_SetIFSD(uint32_t IFSD);
READER_Status READER_HAL_SetBWT(uint32_t BWT);
READER_Status READER_HAL_SetBWI(uint32_t BWI);
READER_Status READER_HAL_SetBGT(uint32_t BGT);
READER_Status READER_HAL_SetRedundancyType(uint32_t rType);
READER_Status READER_HAL_SetWI(uint32_t WI);
uint32_t READER_HAL_GetWT(void);
uint32_t READER_HAL_GetGT(void);
uint32_t READER_HAL_GetGTMili(void);
uint32_t READER_HAL_GetFreq(void);
uint32_t READER_HAL_GetFi(void);
uint32_t READER_HAL_GetDi(void);
uint32_t READER_HAL_GetBWT(void);
uint32_t READER_HAL_GetBWI(void);
uint32_t READER_HAL_GetBGT(void);
uint32_t READER_HAL_GetRedunancyType(void);
uint32_t READER_HAL_GetWI(void);

READER_Status READER_HAL_SetPwrLine(READER_HAL_State state);
READER_Status READER_HAL_SetRstLine(READER_HAL_State state);
READER_Status READER_HAL_SetClkLine(READER_HAL_State state);
READER_Status READER_HAL_SetIOLine(READER_HAL_State state);

READER_Status READER_HAL_DoColdReset(void);

void READER_HAL_Delay(uint32_t tMili);
uint32_t READER_HAL_GetTick(void);
void READER_HAL_ErrHandler(void);


#endif
