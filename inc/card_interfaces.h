typedef enum{
	STATE_ON = 1,
	STATE_OFF = 0
}State;



#define CARD_CLK_PIN       GPIO_PIN_4
#define CARD_IO_PIN        GPIO_PIN_2
#define CARD_RST_PIN       GPIO_PIN_5
#define CARD_PWR_PIN       GPIO_PIN_6



void CARD_InitIOLine(void);
void CARD_InitClkLine(void);
void CARD_InitRstLine(void);
void CARD_InitPwrLine(void);

void CARD_SetPwrLine(State pwrState);
void CARD_SetRstLine(State rstState);
void CARD_SetUSARTPeriph(State USARTState);

void CARD_ColdReset(void);

void CARD_ErrorHandler(void);

void CARD_InitSmartcardHandle(SMARTCARD_HandleTypeDef *smartcardHandleStruct);


// DeInit de smartcard !
// coder des les fonctions de base manipulation IO
// generation d'un delai court
// coder le declenchement de l'ATR
// verifier le code de l'UART
// verifier le code SMARTCARD
// verifier le declanchement ATR
// decoder ATR
// Automate negociation param transm



//Alluemr led errhandler
