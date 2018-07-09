#define READER_T1_SBLOCK 0


typedef enum READER_T1_BlocType READER_T1_BlocType;
enum READER_T1_BlocType{
	READER_T1_SBLOCK                   = (uint32_t)(0x00000000),
	READER_T1_RBLOCK                   = (uint32_t)(0x00000001),
	READER_T1_IBLOCK                   = (uint32_t)(0x00000002)
}
