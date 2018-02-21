


#define READER_DEFAULT_FI      (uint32_t)(372)
#define READER_DEFAULT_DI      (uint32_t)(1)
#define READER_DEFAULT_FREQ    (uint32_t)(4200000)
#define READER_DEFAULT_GT      (uint32_t)(12)
#define READER_DEAFULT_WI      (uint32_t)(10)
#define READER_DEFAULT_WT      (uint32_t)(READER_DEFAULT_WI * 960 * (READER_DEFAULT_FI / READER_DEFAULT_FREQ))   /* Voir ISO7816-3 section 10.2 */




typedef READER_Status enum READER_Status;
enum READER_Status{
	READER_OK = 1,
	READER_NO = 0
	READER_ERR = 2,
	READER_TIMEOUT = 3,
	READER_BUSY = 4
};
