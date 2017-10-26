#include <avr/io.h>

#include "const_defines.h"
#include "usart.h"



void usart_init(uint32_t f_card){
	/* Configurer le mode de l'UART                 */
	usart_set_mode(USART_MODE_SYNCHRONOUS);
	
	/* Configurer le prescaler, horloge de l'UART   */
	usart_set_baudrate(F_DEFAULT, D_DEFAULT, f_card);
	
	/* Configurer le Frame Format                   */
	usart_set_frame_format();
	
	/* Configurer les interruptions                 */
	
	/* Enable le Receiver                           */
	usart_set_receiver(ON);
}

void usart_set_mode(uint8_t mode){
	if(mode == USART_MODE_SYNCHRONOUS){
		/* Mode Synchrone UCSR0C.UMSEL0[1:0] = 0x01 */
		UCSR0C = (UCSR0C & ~(0x01<<UMSEL01)) | (0x01<<UMSEL00);
	}
	else{
		return;
	}
}


void usart_set_baudrate(uint16_t F, uint16_t D, uint32_t f_card){
	/* Registre servant a set la valeur initiale du decompteur */
	UBRR0 = (F / D) * (F_CPU / f_card) - 1;
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
	else{
		UCSR0B &= ~(0x01<<RXEN0);
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