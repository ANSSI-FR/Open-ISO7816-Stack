#include "reader_tpdu.h"
#include "reader_hal.h"



/**
 * \fn READER_APDU_ProtocolCase READER_APDU_GetProtocolCase(READER_APDU_Command *pApdu)
 */
READER_APDU_ProtocolCase READER_APDU_GetProtocolCase(READER_APDU_Command *pApdu){
	uint32_t Nc, Ne;
	
	Nc = pApdu->body.Nc;
	Ne = pApdu->body.Ne;
	
	if((Nc==0) && (Ne==0)){
		return READER_APDU_CASE_1;
	}
	else if((Nc==0) && (Ne!=0)){
		if((Ne>0) && (Ne<=256)){
			return READER_APDU_CASE_2S;
		}
		else if(Ne <= 65536){
			return READER_APDU_CASE_2E;
		}
		else{
			return READER_ERR;
		}
	}
	else if((Nc!=0) && (Ne==0)){
		if((Nc>0) && (Nc<=255)){
			return READER_APDU_CASE_3S;
		}
		else if(Nc <= 65535){
			return READER_APDU_CASE_3E;
		}
		else{
			return READER_APDU_CASE_ERR;
		}
	}
	else((Nc!=0) && (Ne!=0)){
		if((Nc <= 255) && (Ne <= 256)){
			return READER_APDU_CASE_4S;
		}
		else if((Nc <= 65535) && (Ne <= 65536)){
			return READER_APDU_CASE_4E;
		}
		else{
			return READER_APDU_CASE_ERR;
		}
	}
}



READER_Status READER_APDU_Send(READER_APDU_Command *pApdu){
	READER_APDU_ProtocolCase protocolCase;
	
	protocolCase = READER_APDU_GetProtocolCase(pApdu);
	
	switch(protocolCase){
		case READER_APDU_CASE_1:
			return READER_APDU_SendCase1(pApdu);
			break;
			
		case READER_APDU_CASE_2E:
			return READER_APDU_SendCase2E(pApdu);
			break;
			
		case READER_APDU_CASE_2S:
			return READER_APDU_SendCase2S(pApdu);
			break;
			
		case READER_APDU_CASE_3E:
			return READER_APDU_SendCase3E(pApdu);
			break;
			
		case READER_APDU_CASE_3S:
			return READER_APDU_SendCase3S(pApdu);
			break;
			
		case READER_APDU_CASE_4E:
			return READER_APDU_SendCase4E(pApdu);
			break;
			
		case READER_APDU_CASE_4S:
			return READER_APDU_SendCase4S(pApdu);
			break;
			
		case READER_APDU_CASE_ERR:
			return READER_ERR;
			break;
			
		default:
			return READER_ERR;
	}
}
