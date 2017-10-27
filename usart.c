#include <avr/io.h>

#include "const_defines.h"
#include "usart.h"
#include "test.h"
//#include "buffer.h"


void usart_init(uint32_t f_card){
	usart_set_mode(USART_MODE_SYNCHRONOUS_MASTER);                                 /* Configurer le mode de l'UART                                 */	                      
	usart_set_baudrate_etu(F_DEFAULT, D_DEFAULT, f_card);                          /* Configurer le prescaler, horloge de l'UART                   */												                      
	usart_set_frame_format();                                                      /* Configurer le Frame Format                                   */
	usart_set_sampling_mode(USART_SAMPLE_ON_FALLING_EDGE);                         /* On configure le sampling mode de l'USART                     */												                      
	usart_set_receive_interrupt(ON);                                               /* Configurer les interruptions                                 */												                      													                      
	usart_set_receiver(ON);                                                        /* Enable le Receiver et le Transmiter                          */
	usart_set_transmitter(ON);
}

void usart_set_mode(uint8_t mode){
	switch(mode){
		case USART_MODE_SYNCHRONOUS_MASTER:
			UCSR0C = (UCSR0C & ~(0x01<<UMSEL01)) | (0x01<<UMSEL00);                /* Mode Synchrone UCSR0C.UMSEL0[1:0] = 0x01                                              */																               
			XCK_DDR |= (0x01<<XCK_BIT);                                            /* On mets le DDR du pin de l'horloge en sortie (section 24.4.0)                         */														               
			UCSR0A &= ~(0x01<<U2X0);                                               /* On mets U2X0 a 0 (sert uniquement pour l'asynchrone et doit etre a 0 en synchrone)    */
			break;
		case USART_MODE_SYNCHRONOUS_SLAVE:
			UCSR0C = (UCSR0C & ~(0x01<<UMSEL01)) | (0x01<<UMSEL00);                /* Mode Synchrone UCSR0C.UMSEL0[1:0] = 0x01                                              */
			XCK_DDR &= ~(0x01<<XCK_BIT);                                           /* On mets le DDR du pin de l'horloge en entree (section 24.4.0)                         */
			UCSR0A &= ~(0x01<<U2X0);                                               /* On mets U2X0 a 0 (sert uniquement pour l'asynchrone et doit etre a 0 en synchrone)    */
			break;
		case USART_MODE_ASYNCHRONOUS_SIMPLE_SPEED:
			UCSR0C = (UCSR0C & ~(0x01<<UMSEL01)) & ~(0x01<<UMSEL00);               /* Mode Asynchrone UCSR0C.UMSEL0[1:0] = 0x00                                             */
			UCSR0A &= ~(0x01<<U2X0);                                               /* UCSR01.U2X0 = 0 (Simple Speed)                                                        */
			break;                                                                                                                                                          
		case USART_MODE_ASYNCHRONOUS_DOUBLE_SPEED:                                                                                                                          
			UCSR0C = (UCSR0C & ~(0x01<<UMSEL01)) & ~(0x01<<UMSEL00);               /* Mode Asynchrone UCSR0C.UMSEL0[1:0] = 0x00                                             */
			UCSR0A |= (0x01<<U2X0);                                                /* UCSR01.U2X0 = 1 (Double Speed)                                                        */
			break;
		default:
			return;
	}
}


void usart_set_baudrate_etu(uint16_t F, uint16_t D, uint32_t f_card){
	/* Registre servant a set la valeur initiale du decompteur */
	UBRR0 = (F / D) * (F_CPU / f_card) / 2 - 1;                      // valeur initiale du decompteur qui toggle l'horloge (On toggle deux fois plus vite que la frequence (deux etats par periode))
}


void usart_set_baudrate(uint32_t bauds, uint32_t f_card){
	UBRR0 = (F_CPU/(2*bauds)) - 1;                                   // Voir Table 24-1, ligne 3 datasheet
}


void usart_set_frame_format(void){
	/* Parity Mode Even              UCSR0C.UPM0[1:0] = 0x02         */
	/* Number of Stop Bits = 1       UCSR0.USBS0 = 0x00              */
	/* Number of Data Bits = 8       UCSR0.UCSZ0[2:0] = 0x03         */
	
	UCSR0C = (((((UCSR0C | (0x01<<UPM01)) & ~(0x01<<UPM00)) & ~(0x01<<USBS0)) & ~(0x01<<UCSZ02)) | (0x01<<UCSZ01)) | (0x01<<UCSZ00);
}


void usart_set_receiver(uint8_t state){
	if(state == ON){
		UCSR0B |= (0x01<<RXEN0);
	}
	else if(state == OFF){
		UCSR0B &= ~(0x01<<RXEN0);
	}
	else{
		return;
	}
}

void usart_set_transmitter(uint8_t state){
	if(state == ON){
		UCSR0B |= (0x01<<TXEN0);
	}
	else if(state == OFF){
		UCSR0B &= ~(0x01<<TXEN0);
	}
	else{
		return;
	}
}


void usart_set_receive_interrupt(uint8_t state){
	if(state == ON){
		UCSR0B |= (0x01<<RXEN0);
	}
	else if(state == OFF){
		UCSR0B &= ~(0x01<<RXEN0);
	}
	else{
		return;
	}
}

void usart_set_sampling_mode(uint8_t mode){
	if(mode == USART_SAMPLE_ON_FALLING_EDGE){
		/* UCSR0C.UCPOL0 = 0                                 */
		UCSR0C &= ~(0x01<<UCPOL0);
	}
	else if(mode == USART_SAMPLE_ON_RISING_EDGE){
		/* UCSR0C.UCPOL0 = 1                                 */
		UCSR0C |= (0x01<<UCPOL0);
	}
	else{
		return;
	}
}


void usart_get_receiver_error_flags(uint8_t *flag_FE, uint8_t *flag_DOR, uint8_t *flag_UPE){
	uint8_t data = 0;
	
	data = UDR0;
	
	(*flag_FE) = data & (0x01<<FE0);
	(*flag_DOR) = data & (0x01<<DOR0);
	(*flag_UPE) = data & (0x01<<UPE0);
}


void usart_wait_transmitter_ready(void){
	while ( !( UCSR0A & (1<<UDRE0)) );
}


void usart_send_byte(uint8_t byte){
	usart_wait_transmitter_ready();
	UDR0 = byte;
}

void usart_send_frame(uint8_t *frame, uint16_t count){
	uint16_t i;
	
	for(i=0; i<count; i++){
		usart_send_byte(frame[i]);
	}
}

/* Interruption sur reception termine USART, no nested interrupts */
int ISR(int USART_RX_Vect, int ISR_BLOCK){
	uint8_t flag_FE, flag_DOR, flag_UPE;
	uint8_t rx_data;
	
	usart_get_receiver_error_flags(&flag_FE, &flag_DOR, &flag_UPE);
	rx_data = UDR0;                                                               // Dans tous les cas on lit le buffer pour clear le flag d'interrupt
	
	if(!flag_FE && !flag_DOR && !flag_UPE){
		rx_buffer[rx_counter] = rx_data;
		rx_counter++;
	}
	else{
		// signal d'erreur
	}
	
	return 0;
}
