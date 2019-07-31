/**
 * \file reader_tpdu.h
 * \author Boris
 * \brief This file contains the typedefs and the prototypes of all the functions that are dealing with TPDUs.
 */


#ifndef __READER_TPDU_H__
#define __READER_TPDU_H__


#include "reader.h"
#include "reader_hal.h"
#include <stdint.h>



#ifdef TEST
	#define READER_TPDU_MAX_DATA            (uint32_t)(256)
#else
	#define READER_TPDU_MAX_DATA            (uint32_t)(256)
#endif

#define READER_TPDU_HEADER_SIZE         (uint32_t)(0x00000005)
#define READER_TPDU_DUMMY_FALSE_INS     (uint32_t)(0x00000061)

#define READER_TPDU_ACK_NORMAL          (uint32_t)(0x00000001)
#define READER_TPDU_ACK_XORED           (uint32_t)(0x00000002)
#define READER_TPDU_ACK_SW1             (uint32_t)(0x00000003)


/**
 * \def READER_TPDU_MAX_DATA
 * Defines the maximum number of characters that can be hold into an READER_TPDU_Command or READER_TPDU_Response structure.
 * The ISO7816-3 specification indicates 256 as a maximum value.
 * When the Reader library is being used, in each TPDU (Command or Response) structure, READER_TPDU_MAX_DATA bytes of memory are statically allocated in the stack (even if ess data bytes are used).
 * The developper can change the READER_TPDU_MAX_DATA value in order to reduce the memory footprint.
 */
 
 /**
  * \def READER_TPDU_HEADER_SIZE
  * Is the size in characters of a TPDU object header.
  */



/**
 * \struct struct READER_TPDU_Command
 * This stucture describes a the Header of a TPDU Command object as described in the ISO7816-3 section 10.
 */
typedef struct READER_TPDU_Header READER_TPDU_Header;
struct READER_TPDU_Header{
	uint8_t CLA;  /*!< CLA field of the TPDU header as described in ISO7816-3 section 10.3.2. */ 
	uint8_t INS;  /*!< INS field of the TPDU header as described in ISO7816-3 section 10.3.2. */
	uint8_t P1;   /*!< P1 field of the TPDU header as described in ISO7816-3 section 10.3.2.  */
	uint8_t P2;   /*!< P2 field of the TPDU header as described in ISO7816-3 section 10.3.2.  */
	uint8_t P3;   /*!< P3 field of the TPDU header as described in ISO7816-3 section 10.3.2.  */
};


/**
 * \struct struct READER_TPDU_Command
 * This stucture describes a the Data Field of a TPDU Command object as described in the ISO7816-3 section 10.
 */
typedef struct READER_TPDU_DataField READER_TPDU_DataField;
struct READER_TPDU_DataField{
	uint32_t size;                            /*!<  Stores the current number of bytes contained in the data Field of this same structure.  */
	uint8_t data[READER_TPDU_MAX_DATA];       /*!< Stores the Data Field of the TPDU. The data characters are represented as an array of unsigned bytes. This array is statically allocated to the maximum size of the Data Field (256 characters)  */
};


/**
 * \struct struct READER_TPDU_Command
 * This stucture describes a TPDU Command object as described in the ISO7816-3 section 10.
 */
typedef struct READER_TPDU_Command READER_TPDU_Command;
struct READER_TPDU_Command{
	READER_TPDU_Header headerField;      /*!< Is a structure of READER_TPDU_Header type. It is an object representing the Header of a TPDU Command. */   
	READER_TPDU_DataField dataField;     /*!< Is a structure of READER_TPDU_DataField type. It is an object representing the Data Field of a TPDU Command. */   
};


/**
 * \struct struct READER_TPDU_Response
 * This stucture describes a TPDU Response object as described in the ISO7816-3 section 10.
 */
typedef struct READER_TPDU_Response READER_TPDU_Response;
struct READER_TPDU_Response{
	uint8_t SW1;                                 /*!< Stores the value of the first part (8 first bits) of the Status Word (SW1). */   
	uint8_t SW2;                                 /*!< Stores the value of the second part (bits 9 to 16) of the Status Word  (SW2). */ 
	uint8_t dataBytes[READER_TPDU_MAX_DATA];     /*!< Stores the Data Field of the TPDU. The data characters are represented as an array of unsigned bytes. This array is statically allocated to the maximum size of the Data Field (256 characters). */    
	uint32_t dataSize;                           /*!< Stores the number of characters actuelly contained into the dataBytes member. */
};


//READER_Status READER_TPDU_Execute(READER_TPDU_Command *pTpdu, READER_TPDU_Response *pResp, uint32_t timeout, READER_HAL_CommSettings *pSettings);

READER_Status READER_TPDU_Send(READER_TPDU_Command *tpdu, READER_TPDU_Response *pTpduResp, uint32_t timeout, READER_HAL_CommSettings *pSettings);
READER_Status READER_TPDU_SendHeader(READER_TPDU_Command *tpdu, uint32_t timeout, READER_HAL_CommSettings *pSettings);
READER_Status READER_TPDU_SendDataOneshot(READER_TPDU_Command *tpdu, uint32_t timeout, READER_HAL_CommSettings *pSettings);
READER_Status READER_TPDU_SendDataSliced(READER_TPDU_Command *tpdu, uint32_t timeout, READER_HAL_CommSettings *pSettings);

READER_Status READER_TPDU_RcvSW(uint8_t *SW1, uint8_t *SW2, uint32_t timeout, READER_HAL_CommSettings *pSettings);
//READER_Status READER_TPDU_RcvDataField(uint8_t *buffer, uint32_t Ne, uint32_t timeout, READER_HAL_CommSettings *pSettings);
READER_Status READER_TPDU_RcvResponse(READER_TPDU_Response *pResp, uint32_t expectedDataSize, uint32_t timeout, READER_HAL_CommSettings *pSettings);

//READER_Status READER_TPDU_WaitProcedureByte(uint8_t *procedureByte, uint8_t INS, uint32_t timeout, READER_HAL_CommSettings *pSettings);
READER_Status READER_TPDU_WaitACK(uint8_t INS, uint32_t *ACKType, uint8_t *pSW1, uint8_t *pSW2, uint32_t timeout, READER_HAL_CommSettings *pSettings);

READER_Status READER_TPDU_IsACK(uint8_t byte, uint8_t INS);
READER_Status READER_TPDU_IsXoredACK(uint8_t byte, uint8_t INS);
READER_Status READER_TPDU_IsNullByte(uint8_t byte);
READER_Status READER_TPDU_IsSW1(uint8_t byte);
READER_Status READER_TPDU_IsProcedureByte(uint8_t byte, uint8_t INS);

READER_Status READER_TPDU_Forge(READER_TPDU_Command *tpdu, uint8_t CLA, uint8_t INS, uint8_t P1, uint8_t P2, uint8_t P3, uint8_t *dataBuff, uint32_t dataSize);



#endif
