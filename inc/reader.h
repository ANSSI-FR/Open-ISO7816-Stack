#ifndef __READER_H__
#define __READER_H__


#include <stdint.h>



#define READER_DEFAULT_FI         (uint32_t)(372)
#define READER_DEFAULT_DI         (uint32_t)(1)
#define READER_DEFAULT_FREQ       (uint32_t)(4200000)
#define READER_DEFAULT_GT         (uint32_t)(12)
#define READER_DEFAULT_WI         (uint32_t)(10)
//#define READER_DEFAULT_WT_MILI    (uint32_t)((1000 * READER_DEFAULT_WI * 960 * READER_DEFAULT_FI) / (float)READER_DEFAULT_FREQ)      /* Voir ISO7816-3 section 10.2 */
#define READER_DEFAULT_BGT        (uint32_t)(22)      /* 22 etu, Voir ISO7816-3 section 11.2 */
#define READER_DEFAULT_BWI        (uint32_t)(4)       /* Voir ISO7816-3 section 11.4.3       */
#define READER_DEFAULT_BWT        (uint32_t)(11 + 2*2*2*2 * 960 * ((float)READER_DEFAULT_FI / (float)READER_DEFAULT_FREQ))    /* Voir ISO7816-3 section 11.4.3       */
#define READER_DEFAULT_IFSC       (uint32_t)(32)
#define READER_DEFAULT_IFSD       (uint32_t)(32)
#define READER_DEFAULT_REDUNDANCY_TYPE (uint32_t)(0x00000001)   /* Valeur definie dans reader_t1_block.h */



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
	READER_BAD_VALUE            = (uint32_t)(0x0000000E)
};


#endif
