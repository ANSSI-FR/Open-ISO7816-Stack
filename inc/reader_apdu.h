/**
 * \file reader_apdu.h
 * \copyright This file is part of the Open-ISO7816-Stack project and is distributed under the MIT license. See LICENSE file in the root directory. 
 * This file contains the definition of the APDU data structure and the prototypes of the functions to operate on the APDUs.
 */



#ifndef __READER_APDU_H__
#define __READER_APDU_H__




#define READER_APDU_INS_ENVELOPE         (uint8_t)(0xC2)
#define READER_APDU_INS_GETRESPONSE      (uint8_t)(0xC0)

#define READER_APDU_HEADER_SIZE          (uint32_t)(4)
#define READER_APDU_CMD_DATA_MAX_SIZE    (uint32_t)(400)                                   /* La norme ISO fixe un maximum de 65535 caracteres */
#define READER_APDU_RESP_MAX_SIZE        (uint32_t)(400)                                   /* La norme ISO fixe un maximum de 65536 caracteres */
#define READER_APDU_RESP_MAX_TOTALSIZE   (uint32_t)(READER_APDU_RESP_MAX_SIZE + 2)                                   /* La norme ISO fixe un maximum de 65536 caracteres */
#define READER_APDU_CMD_MAX_TOTALSIZE    (uint32_t)(READER_APDU_CMD_DATA_MAX_SIZE + 6)     /* Taille max des donnees + CLA, INS, P1, P2, Le, Lc */


/**
 * \def READER_APDU_INS_ENVELOPE
 * Is the instruction code (INS) of the ENVELOPE instruction.
 */
 
 /**
 * \def READER_APDU_INS_GETRESPONSE
 * Is the instruction code (INS) of the GET RESPONSE instruction.
 */
 
 /**
 * \def READER_APDU_CMD_DATA_MAX_SIZE
 * Is the maximum number of data bytes that can be hold into an APDU Command. 
 * The ISO7816-3 sets this value to 65535 characters. However, this memory is statically allocated in the stack when using the READER_APDU_Command data structure.
 * That's why (in order to reduce the memory footprint) it is recomended to define this value below the maximum. 
 */
  
 /**
 * \def READER_APDU_RESP_MAX_SIZE
 * Is the maximum number of data bytes that can be hold into an APDU Response. 
 * The ISO7816-3 sets this value to 65535 characters. However, this memory is statically allocated in the stack when using the READER_APDU_Command data structure.
 * That's why (in order to reduce the memory footprint) it is recomended to define this value below the maximum. 
 */
   
   
 /**
  * \def READER_APDU_RESP_MAX_TOTALSIZE
  * This macro equals to the maximum total size (in characters) on an APDU Response.
  * This size includes the maximum data bytes and the satus words (SW).
  */
  
/**
 * \def READER_APDU_CMD_MAX_TOTALSIZE
 * This macro equals to the maximum total size (in characters) on an APDU Command.
 * This size includes the maximum data bytes and all the other fields of an APDU Command.
 */



/**
 * \enum READER_APDU_ProtocolCase
 * The type READER_APDU_ProtocolCase is used to encode the protocol cases (Case 1, Case2S, etc ...) described in ISO7816-3 section 12.1.3.
 */
typedef enum READER_APDU_ProtocolCase READER_APDU_ProtocolCase;
enum READER_APDU_ProtocolCase{
	READER_APDU_CASE_ERR       =   (uint32_t)(0x00000000),
	READER_APDU_CASE_1         =   (uint32_t)(0x00000001),
	READER_APDU_CASE_2S        =   (uint32_t)(0x00000002),
	READER_APDU_CASE_2E        =   (uint32_t)(0x00000003),
	READER_APDU_CASE_3S        =   (uint32_t)(0x00000004),
	READER_APDU_CASE_3E        =   (uint32_t)(0x00000005),
	READER_APDU_CASE_4S        =   (uint32_t)(0x00000006),
	READER_APDU_CASE_4E        =   (uint32_t)(0x00000007)
};



/**
 * \struct READER_APDU_Body
 * This structure describes the body of an APDU as defined in the ISO7816-3 section 12.1.1.
 */
typedef struct READER_APDU_Body READER_APDU_Body;
struct READER_APDU_Body{
	uint32_t Nc;                                           /*!< Nc is the number of data characters embedded into this APDU Command. */
	uint32_t Ne;                                           /*!< Ne is the number of data characters expected in response to this APDU Command. */
 	uint8_t dataBytes[READER_APDU_CMD_DATA_MAX_SIZE];      /*!< dataBytes is an array of bytes which stores the data bytes embedded into this APDU Command. */
};



/**
 * \struct READER_APDU_Body
 * This structure describes the header of an APDU as defined in the ISO7816-3 section 12.1.1.
 */
typedef struct READER_APDU_Header READER_APDU_Header;
struct READER_APDU_Header{
	uint8_t CLA;                                           /*!< CLA is the instruction class. */
	uint8_t INS;                                           /*!< INS is the instruction code of the APDU Command. It indicates the operation that the card should perform when command received. */
	uint8_t P1;                                            /*!< P1 is the first parameter of the instruction INS. */
	uint8_t P2;                                            /*!< P2 is the second parameter of the instruction INS. */
};


/**
 * \struct READER_APDU_Command
 * This structure describes an APDU Command object as defined in the ISO7816-3 section 12.1.1.
 */
typedef struct READER_APDU_Command READER_APDU_Command;
struct READER_APDU_Command{
	READER_APDU_Header header;     /*!< The member header is a READER_APDU_Header structure which decribes the header of the APDU Command as defined in the ISO7816-3 section 12.1.1. */
	READER_APDU_Body body;         /*!< The member body is a READER_APDU_Body structure which decribes the body of the APDU Command as defined in the ISO7816-3 section 12.1.1. */
};


/**
 * \struct READER_APDU_Response
 * This structure describes an APDU Response object as defined in the ISO7816-3 section 12.1.1.
 */
typedef struct READER_APDU_Response READER_APDU_Response;
struct READER_APDU_Response{
	uint8_t dataBytes[READER_APDU_RESP_MAX_SIZE];   /*!< dataBytes is an array that contains the data characters of the card response. */
	uint32_t dataSize;                              /*!< dataSize indicates the number of data charaters in the card response. */
	uint8_t SW1;                                    /*!< SW1 is the first part of the card response status word. */
	uint8_t SW2;                                    /*!< SW2 is the second part of the card response status word. */
};


typedef enum READER_APDU_Protocol READER_APDU_Protocol;
enum READER_APDU_Protocol{
	READER_APDU_PROTOCOL_T0         =   (uint32_t)(0x00000000),
	READER_APDU_PROTOCOL_T1         =   (uint32_t)(0x00000001)
};





READER_Status READER_APDU_CheckCmdValidity(READER_APDU_Command *pApduCmd);
READER_APDU_ProtocolCase READER_APDU_GetProtocolCase(READER_APDU_Command *pApduCmd);



READER_Status READER_APDU_Forge(READER_APDU_Command *pApduCmd, uint8_t CLA, uint8_t INS, uint8_t P1, uint8_t P2, uint32_t Nc, uint8_t *pData, uint32_t Ne);
READER_Status READER_APDU_CopyCommand(READER_APDU_Command *pSourceApdu, READER_APDU_Command *pDestApdu);
READER_Status READER_APDU_CopyResponse(READER_APDU_Response *pSourceApdu, READER_APDU_Response *pDestApdu);
READER_Status READER_APDU_ExtractRespSW(READER_APDU_Response *pApduResp, uint8_t *pSW1, uint8_t *pSW2);
READER_Status READER_APDU_ExtractRespDataPtr(READER_APDU_Response *pApduResp, uint8_t **ppData, uint32_t *pDataSize);

uint16_t READER_APDU_NcToLc(uint16_t Nc);
uint16_t READER_APDU_NeToLe(uint16_t Ne);
uint16_t READER_APDU_LcToNc(uint16_t Lc);
uint16_t READER_APDU_LeToNe(uint16_t Le);


#endif
