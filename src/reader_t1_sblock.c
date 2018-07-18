#include "reader_t1_sblock.h"
#include "reader_t1_block.h"
#include "reader_hal.h"








READER_Status READER_T1_ForgeSBlock(READER_T1_Block *pBlock, READER_T1_SBlockType type){
	READER_T1_RedundancyType rType;
	READER_Status retVal;
	
	
	rType = READER_HAL_GetRedunancyType();
	retVal = READER_T1_ForgeBlock(pBlock, rType);
	if(retVal != READER_OK) return retVal;
	
	retVal = READER_T1_SetBlock
	
	return READER_OK;
}
