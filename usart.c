#include <avr/io.h>

#include "const_defines.h"
#include "usart.h"

uint8_t usart_buffer[100];
uint8_t usart_buffer_counter=0;

void usart_init(uint32_t f_card){
	/* Configurer le mode de l'UART                 */
	usart_set_mode(USART_MODE_SYNCHRONOUS);
	
	/* Configurer le prescaler, horloge de l'UART   */
	usart_set_baudrate(F_DEFAULT, D_DEFAULT, f_card);
	
	/* Configurer le Frame Format                   */
	usart_set_frame_format();
	
	/* On configure le sampling mode de l'USART     */
	usart_set_sampling_mode(USART_SAMPLE_ON_FALLING_EDGE);
	
	/* Configurer les interruptions                 */
	usart_set_receive_interrupt(ON);
	
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


void usart_get_receiver_error_flags(uint8_t *flag_FE, uint8_t *flag_DOR, uint8_t *flag_UPE){
	uint8_t data = 0;
	
	data = UDR0;
	
	(*flag_FE) = data & (0x01<<FE0);
	(*flag_DOR) = data & (0x01<<DOR0);
	(*flag_UPE) = data & (0x01<<UPE0);
}

/* Interruption sur reception termine USART, no nested interrupts */
int ISR(int USART_RX_Vect, int ISR_BLOCK){
	uint8_t flag_FE, flag_DOR, flag_UPE;
	uint8_t rcv_data;
	
	usart_get_receiver_error_flags(&flag_FE, &flag_DOR, &flag_UPE);
	rcv_data = UDR0;                                                               // Dans tous les cas on lit le buffer pour clear le flag d'interrupt
	
	if(!flag_FE && !flag_DOR && !flag_UPE){
		usart_buffer[usart_buffer_counter] = rcv_data;
		usart_buffer_counter++;
	}
	else{
		
	}
	
	return 0;
}
