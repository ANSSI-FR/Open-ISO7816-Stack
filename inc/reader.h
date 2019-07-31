/**
 * \file reader.h
 * \author Boris
 * This file contains the definition of the READER_Status type.
 */


#ifndef __READER_H__
#define __READER_H__


#include <stdint.h>
#include <stddef.h>




/**
 * \enum READER_Status
 * This type is used to encode the returned execution code of all the functions of the project.
 * For each function, this code indicates if the function behaved as expected or not.
 */
typedef enum READER_Status READER_Status;
enum READER_Status{
	READER_OK                       = (uint32_t)(0x00000001),
	READER_NO                       = (uint32_t)(0x00000000),
	READER_ERR                      = (uint32_t)(0x00000002),
	READER_TIMEOUT                  = (uint32_t)(0x00000003),
	READER_TIMEOUT_GOT_ONLY_SW      = (uint32_t)(0X00000004),
	READER_BUSY                     = (uint32_t)(0x00000005),
	READER_TOO_MUCH_TRIES           = (uint32_t)(0x00000006),
	READER_EMPTY                    = (uint32_t)(0x00000007),
	READER_FULL                     = (uint32_t)(0x00000008),
	READER_BAD_ARG                  = (uint32_t)(0x00000009),
	READER_TOO_LONG                 = (uint32_t)(0x0000000A),
	READER_DOESNT_EXIST             = (uint32_t)(0x0000000B),
	READER_INTEGRITY                = (uint32_t)(0x0000000C),
	READER_BLOCK_TIMEOUT            = (uint32_t)(0x0000000D),
	READER_BAD_VALUE                = (uint32_t)(0x0000000E),
	READER_INVALID_BLOCK            = (uint32_t)(0x0000000F),
	READER_OVERFLOW                 = (uint32_t)(0x00000010),
	READER_LESS_DATA_THAN_EXPECTED  = (uint32_t)(0x00000011),
	READER_TIMEOUT_ON_SW1           = (uint32_t)(0x00000012),
	READER_TIMEOUT_ON_SW2           = (uint32_t)(0x00000013),
	READER_INCORRECT_SW1            = (uint32_t)(0x00000014),
	READER_GOT_SW1                  = (uint32_t)(0x00000015),
	READER_INVALID_PROCEDURE_BYTE   = (uint32_t)(0x00000016)
};


#endif
