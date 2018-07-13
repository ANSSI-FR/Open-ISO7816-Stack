#ifndef __READER_H__
#define __READER_H__



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



typedef enum READER_Status READER_Status;
enum READER_Status{
	READER_OK                   = (0x00000001),
	READER_NO                   = (0x00000000),
	READER_ERR                  = (0x00000002),
	READER_TIMEOUT              = (0x00000003),
	READER_TIMEOUT_GOT_ONLY_SW  = (0X00000004),
	READER_BUSY                 = (0x00000005)
};


#endif
