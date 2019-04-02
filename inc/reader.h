#ifndef __READER_H__
#define __READER_H__


#include <stdint.h>








typedef enum READER_Status READER_Status;
enum READER_Status{
	READER_OK                   = (uint32_t)(0x00000001),
	READER_NO                   = (uint32_t)(0x00000000),
	READER_ERR                  = (uint32_t)(0x00000002),
	READER_TIMEOUT              = (uint32_t)(0x00000003),
	READER_TIMEOUT_GOT_ONLY_SW  = (uint32_t)(0X00000004),
	READER_BUSY                 = (uint32_t)(0x00000005),
	READER_TOO_MUCH_TRIES       = (uint32_t)(0x00000006),
	READER_EMPTY                = (uint32_t)(0x00000007),
	READER_FULL                 = (uint32_t)(0x00000008),
	READER_BAD_ARG              = (uint32_t)(0x00000009),
	READER_TOO_LONG             = (uint32_t)(0x0000000A),
	READER_DOESNT_EXIST         = (uint32_t)(0x0000000B),
	READER_INTEGRITY            = (uint32_t)(0x0000000C),
	READER_BLOCK_TIMEOUT        = (uint32_t)(0x0000000D),
	READER_BAD_VALUE            = (uint32_t)(0x0000000E),
	READER_INVALID_BLOCK        = (uint32_t)(0x0000000F),
	READER_OVERFLOW             = (uint32_t)(0x00000010)
};


#endif
