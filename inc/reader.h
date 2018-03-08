#ifndef __READER_H__
#define __READER_H__



#define READER_DEFAULT_FI         (uint32_t)(372)
#define READER_DEFAULT_DI         (uint32_t)(1)
#define READER_DEFAULT_FREQ       (uint32_t)(4200000)
#define READER_DEFAULT_GT         (uint32_t)(12)
#define READER_DEFAULT_WI         (uint32_t)(10)
#define READER_DEFAULT_WT_MILI    (uint32_t)((1000 * READER_DEFAULT_WI * 960 * READER_DEFAULT_FI) / READER_DEFAULT_FREQ)      /* Voir ISO7816-3 section 10.2 */




typedef enum READER_Status READER_Status;
enum READER_Status{
	READER_OK                   = (0x00000001),
	READER_NO                   = (0x00000000),
	READER_ERR                  = (0x00000002),
	READER_TIMEOUT              = (0x00000003),
	READER_BUSY                 = (0x00000004)
};


#endif
