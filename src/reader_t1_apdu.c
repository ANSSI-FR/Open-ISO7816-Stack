#include "reader_t1_apdu.h"





READER_Status READER_T1_APDU_Execute(READER_APDU_Command *pApduCmd, READER_APDU_Response *pApduResp){
	READER_Status retVal;
	READER_T1_ContextHandler context;
	READER_T1_BufferStatus buffStatus;
	READER_T1_Block blockToSend, rcvdBlock;
	
	
	/* On initialise le contexte de communication et on remplit le Buffer d'envoi a partir de la commande APDU ...  */
	retVal = READER_T1_CONTEXT_Init(&context);
	if(retVal != READER_OK) return retVal;
	
	retVal = READER_T1_FORGE_FillBuffWithAPDU(&context, pApduCmd);
	if(retVal != READER_OK) return retVal;
	
	/* Tantque le Buffer d'envoi n'est pas vide, on defile et on envoie le Block ...     */	
	retVal = READER_T1_BUFFER_IsEmpty(&context, &buffStatus)
	if(retVal != READER_OK) return retVal;
		
	while(buffStatus == READER_T1_BUFFER_NOTEMPTY){
		retVal = READER_T1_BUFFER_Enqueue(&context, &blockToSend);
		if(retVal != READER_OK) return retVal;
		
		retVal = READER_T1_CONTROL_SendBlock(&context, &blockToSend);
		if(retVal != READER_OK) return retVal;
		
		retVal = READER_T1_CONTROL_RcvBlock(&context, &rcvdBlock);
		if(retVal != READER_OK) return retVal;
		
		retVal = READER_T1_BUFFER_IsEmpty(&context, &buffStatus)
		if(retVal != READER_OK) return retVal;
	}
	
	retVal = READER_T1_RCPTBUFF_CreateAPDUResp(&context, pApduResp);
	if(retVal != READER_OK) return retVal;
	
	return READER_OK;
}
