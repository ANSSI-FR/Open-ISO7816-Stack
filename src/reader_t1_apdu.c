#include "reader_t1_apdu.h"






READER_Status READER_T1_APDU_Init(READER_T1_ContextHandler *pContext){
	READER_Status retVal;
	READER_ATR_Atr atr;
	
	
	/* Initialisation de la HAL ...  */
	retVal = READER_HAL_Init();
	if(retVal != READER_OK) return retVal;
	
	/* Activation de la Carte ...    */
	retVal = READER_HAL_DoColdReset();
	if(retVal != READER_OK) return retVal;
	
	/* Reception de l'ATR            */
	retVal = READER_ATR_Receive(&atr);
	if(retVal != READER_OK) return retVal;
	
	/* Reinitialisation complete des parametres de communication */
	retVal = READER_T1_CONTEXT_Init(pContext);
	if(retVal != READER_OK) return retVal;
	
	return READER_OK;
}

READER_Status READER_T1_APDU_Execute(READER_APDU_Command *pApduCmd, READER_APDU_Response *pApduResp){
	READER_Status retVal;
	READER_T1_ContextHandler context;
	READER_T1_BufferStatus buffStatus;
	READER_T1_Block blockToSend, rcvdBlock;
	
	
	/* On initialise partiellement le contexte de communication et on remplit le Buffer d'envoi a partir de la commande APDU ...  */
	/* On reinitialise tout sauf BWT, CWT, etc ...                                                                                */
	retVal = READER_T1_CONTEXT_InitContextSettings(&context);
	if(retVal != READER_OK) return retVal;
	
	retVal = READER_T1_CONTEXT_InitBuffer(&context);
	if(retVal != READER_OK) return retVal;
	
	/* On remplit le Buffer d'envoi a partir du contenu de la commande APDU ...                                                   */
	retVal = READER_T1_FORGE_FillBuffWithAPDU(&context, pApduCmd);
	if(retVal != READER_OK) return retVal;
	
	/* Tantque le Buffer d'envoi n'est pas vide, on defile et on envoie le Block ...                                              */	
	retVal = READER_T1_BUFFER_IsEmpty(&context, &buffStatus);
	if(retVal != READER_OK) return retVal;
		
	while(buffStatus == READER_T1_BUFFER_NOTEMPTY){
		retVal = READER_T1_BUFFER_Dequeue(&context, &blockToSend);
		if(retVal != READER_OK) return retVal;
		
		retVal = READER_T1_CONTROL_SendBlock(&context, &blockToSend);
		if(retVal != READER_OK) return retVal;
		
		retVal = READER_T1_CONTROL_RcvBlock(&context, &rcvdBlock);
		if(retVal != READER_OK) return retVal;
		
		retVal = READER_T1_BUFFER_IsEmpty(&context, &buffStatus);
		if(retVal != READER_OK) return retVal;
	}
	
	/* Si on est sortit de la boucle pour une autre raison que le Buffer vide, alors ca veut dire qu'on a eu une erreur interne ... */
	if(buffStatus != READER_T1_BUFFER_EMPTY){
		return READER_ERR;
	}
	
	/* A partir des donnees brutes recues en provenance de la carte, on reconstitue un APDU Response ...                            */
	retVal = READER_T1_RCPTBUFF_CreateAPDUResp(&context, pApduResp);
	if(retVal != READER_OK) return retVal;
	
	
	return READER_OK;
}
