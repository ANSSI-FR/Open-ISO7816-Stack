/**
 * \file reader_t1_apdu.c
 * \copyright This file is part of the Open-ISO7816-Stack project and is distributed under the MIT license. See LICENSE file in the root directory. 
 * 
 * This file contains all the code of the functions which are necessary to execute APDU commands when using the T=1 underlying protocol.
 */


#include "reader_t1_apdu.h"




/**
 * \fn READER_Status READER_T1_APDU_Init(READER_T1_ContextHandler *pContext, READER_HAL_CommSettings *pSettings)
 * \return The function returns an execution code of type READER_Status that indicates if the function behaved as expected or not.
 * \param *pContext is a pointer on a READER_T1_ContextHandler data structure to be initialized by these funtion.
 * \param *pSettings is a pointer on a READER_HAL_CommSettings data structure that should already be containing the low level communications settings for the hardware abstraction layer.
 * The function have to be called before using APDUs with the T=1 protocol.
 * This function will initialize the T=1 communication context that is stored into the *pContext structure.  
 */
READER_Status READER_T1_APDU_Init(READER_T1_ContextHandler *pContext, READER_HAL_CommSettings *pSettings){
	READER_Status retVal;

	
	/* Reinitialisation complete des parametres de communication */
	retVal = READER_T1_CONTEXT_Init(pContext, pSettings);
	if(retVal != READER_OK) return retVal;
	
	return READER_OK;
}


/**
 * \fn READER_Status READER_T1_APDU_Execute(READER_T1_ContextHandler *pContext, READER_APDU_Command *pApduCmd, READER_APDU_Response *pApduResp)
 * \return The function returns an execution code of type READER_Status that indicates if the function behaved as expected or not.
 * \param *pContext is a pointer on a READER_T1_ContextHandler data structure to be initailized by these funtion.
 * \param *pApduCommand is a pointer to a READER_APDU_Command data structure. It should have been previously filled with an APDU command to be sent to the card. For example with the READER_T0_APDU_Forge() function.
 * \param *pApduResp is a pointer to a READER_APDU_Response data structure. This function will fill it with the response aswered by the card.
 * This function is used to execute an APDU command using the T=1 protocol. It means sending the APDU command to the card and getting back the response (APDU answer).
 */
READER_Status READER_T1_APDU_Execute(READER_T1_ContextHandler *pContext, READER_APDU_Command *pApduCmd, READER_APDU_Response *pApduResp){
	READER_Status retVal, retVal2;
	READER_T1_BlockType bType;
	READER_T1_MBit mBit;
	READER_T1_BufferStatus buffStatus;
	READER_T1_Block blockToSend, rcvdBlock;
	READER_T1_Block *pLastRcvd;
	
	
	/* On initialise partiellement le contexte de communication et on remplit le Buffer d'envoi a partir de la commande APDU ...  */
	/* On reinitialise tout sauf BWT, CWT, etc ...                                                                                */
	retVal = READER_T1_CONTEXT_InitContextSettings(pContext);
	if(retVal != READER_OK) return retVal;
	
	//retVal = READER_T1_CONTEXT_InitBuffer(pContext);
	//if(retVal != READER_OK) return retVal;
	
	/* On remplit le Buffer d'envoi a partir du contenu de la commande APDU ...                                                   */
	retVal = READER_T1_FORGE_FillBuffWithAPDU(pContext, pApduCmd);
	if(retVal != READER_OK) return retVal;
	
	/* Tantque le Buffer d'envoi n'est pas vide, on defile et on envoie le Block ...                                              */	
	retVal = READER_T1_BUFFER_IsEmpty(pContext, &buffStatus);
	if(retVal != READER_OK) return retVal;

	
	while(buffStatus == READER_T1_BUFFER_NOTEMPTY){
		retVal = READER_T1_BUFFER_Dequeue(pContext, &blockToSend);
		if(retVal != READER_OK) return retVal;
		
		retVal = READER_T1_CONTROL_SendBlock(pContext, &blockToSend);
		if(retVal != READER_OK) return retVal;
		
		/* On regarde si il faut recevoir un Block ...  */
		retVal = READER_T1_CONTEXT_GetLastRcvd(pContext, &pLastRcvd);
		if((retVal != READER_OK) && (retVal != READER_DOESNT_EXIST)) return retVal;
		
		if(retVal != READER_DOESNT_EXIST){
			mBit = READER_T1_GetBlockMBit(pLastRcvd);
		}
		
		retVal2 = READER_T1_CONTEXT_GetLastRcvdType(pContext, &bType);
		if((retVal2 != READER_OK) && (retVal2 != READER_DOESNT_EXIST)) return retVal2;
		
		/* On recoit si on a jamais rien recu, ou si, le dernier Block recu n'est pas un "dernier I-Block" ...  */
		if((retVal == READER_DOESNT_EXIST) || !((bType == READER_T1_IBLOCK) && (mBit == READER_T1_MBIT_ZERO))){
			retVal = READER_T1_CONTROL_RcvBlock(pContext, &rcvdBlock);
			if(retVal != READER_OK) return retVal;
		}
		
		retVal = READER_T1_BUFFER_IsEmpty(pContext, &buffStatus);
		if(retVal != READER_OK) return retVal;
	}
	
	/* Si on est sortit de la boucle pour une autre raison que le Buffer vide, alors ca veut dire qu'on a eu une erreur interne ... */
	if(buffStatus != READER_T1_BUFFER_EMPTY){
		return READER_ERR;
	}
	
	/* A partir des donnees brutes recues en provenance de la carte, on reconstitue un APDU Response ...                            */
	retVal = READER_T1_RCPTBUFF_CreateAPDUResp(pContext, pApduResp);
	if(retVal != READER_OK) return retVal;
	
	
	return READER_OK;
}
