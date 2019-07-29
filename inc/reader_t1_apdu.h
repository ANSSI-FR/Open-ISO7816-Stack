#include "reader_t1.h"



#ifndef __READER_T1_APDU_H__
#define __READER_T1_APDU_H__



READER_Status READER_T1_APDU_Init(READER_T1_ContextHandler *pContext, READER_HAL_CommSettings *pSettings);
READER_Status READER_T1_APDU_Execute(READER_T1_ContextHandler *pContext, READER_APDU_Command *pApduCmd, READER_APDU_Response *pApduResp);







#endif
