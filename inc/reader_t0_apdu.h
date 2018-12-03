#include "reader.h"
#include "reader_apdu.h"
#include "reader_tpdu.h"
#include "reader_atr.h"



#ifndef __READER_T0_APDU_H__
#define __READER_T0_APDU_H__




READER_Status READER_T0_APDU_Init(void);
READER_Status READER_T0_APDU_Execute(READER_APDU_Command *pApduCmd, READER_APDU_Response *pApduResp, uint32_t timeout);


READER_Status READER_T0_APDU_ExecuteCase1(READER_APDU_Command *pApduCmd, READER_APDU_Response *pApduResp, uint32_t timeout);
READER_Status READER_T0_APDU_ExecuteCase2S(READER_APDU_Command *pApduCmd, READER_APDU_Response *pApduResp, uint32_t timeout);
READER_Status READER_T0_APDU_ExecuteCase2E(READER_APDU_Command *pApduCmd, READER_APDU_Response *pApduResp, uint32_t timeout);
READER_Status READER_T0_APDU_ExecuteCase3S(READER_APDU_Command *pApduCmd, READER_APDU_Response *pApduResp, uint32_t timeout);
READER_Status READER_T0_APDU_ExecuteCase3E(READER_APDU_Command *pApduCmd, READER_APDU_Response *pApduResp, uint32_t timeout);
READER_Status READER_T0_APDU_ExecuteCase4S(READER_APDU_Command *pApduCmd, READER_APDU_Response *pApduResp, uint32_t timeout);
READER_Status READER_T0_APDU_ExecuteCase4E(READER_APDU_Command *pApduCmd, READER_APDU_Response *pApduResp, uint32_t timeout);

READER_Status READER_T0_APDU_RcvSW(uint16_t *SW, uint32_t timeout);
READER_Status READER_T0_APDU_RcvResponse(uint8_t *buffer, uint32_t Ne, uint16_t *SW, uint32_t timeout);


READER_Status READER_T0_APDU_MapTpduRespToApdu(READER_TPDU_Response *pTpduResp, READER_APDU_Response *pApduResp);








#endif
