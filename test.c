/* Boris Simunovic
 * 11/10/2017
 */


#include <avr/io.h>

#include "test.h"
#include "const_defines.h"
#include "usart.h"

extern uint8_t rx_buffer[100];
extern uint8_t rx_counter;

void setup(void){
	rx_counter=1;
	blink_led(3);
	
	do_activation();
	do_cold_reset();
	
	/* wait_cycles_long(1000000, F_CARD);	*/
	/* blink_led(3);                        */
	wait_cycles_long(1000000, F_CARD);
	blink_led(rx_counter);
	
	/*usart_set_baudrate(9600, F_CARD);
	
	usart_polling_send_byte(0x41);
	usart_polling_send_byte(0x42);
	usart_polling_send_byte(0x43);
	usart_polling_send_byte(0x44);
	usart_polling_send_byte(0x45);
	usart_polling_send_byte(0x0A);*/
}


void loop(){
	/*usart_polling_send_frame(rx_buffer, rx_counter);
	usart_polling_send_byte(0x45);
	usart_polling_send_byte(0x4E);
	usart_polling_send_byte(0x44);
	usart_polling_send_byte(0x0A);
	
	wait_cycles_long(2000000, F_CARD);
    */
}


void setup_clock_config(uint32_t f_card){	
	/* PD6 en sortie (pin OC0A)                                                           */
	/* DDRD |= (0x01 << PIN_CLK);              On mets DDRD6 a 1 (output)                 */
	/* PIND &= ~(0x01 << PIN_CLK);             No Toggle                                  */
																	          
	/* Configuration du timer 0 en mode CTC (TCCR0x.WGM[2:0] = 0x2)                       */
	TCCR0B &= ~(0x01<<WGM02);         /* WGM2 = 0                                         */
	TCCR0A = (TCCR0A | (0x01<<WGM01)) & ~(0x01<<WGM00);
																	          
	/* Configuration du pin OC0A en toggle (TCCRA.COM[1:0] = 0x1)                         */
	TCCR0A = (TCCR0A & ~(0x01<<COM0A1)) | (0x01<<COM0A0);
																	          
	/* Comparateur A du timer pour diviser clock                                          */
	OCR0A = F_CPU / f_card / 2 - 1;
}



void set_clock(uint8_t state){
	switch(state){
		case ON: 
			/* Positionnement du prescaler du timer 0 a 1 (TCCR0B.CS0[2:0] = 0x1)         */
			TCCR0B = ((TCCR0B & ~(0x01<<CS02)) & ~(0x01<<CS01)) | (0x01<<CS00) ;

			break;
		case OFF:
			/* Positionnement du prescaler du timer 0 a 0 (No clock Source) (TCCR0B.CS0[2:0] = 0x00)   */
			TCCR0B = ((TCCR0B & ~(0x01<<CS02)) & ~(0x01<<CS01)) & ~(0x01<<CS00);
			break;
		default:
			return;
	}	
}


void set_clock_state(uint8_t state){
	/* PD6 en sortie (pin OC0A) */
	DDRD |= (0x01 << PIN_CLK);              /* On mets DDRD6 a 1 (output)                 */
	PIND &= ~(0x01 << PIN_CLK);             /* No Toggle                                  */
	
	switch(state){
		case STATE_L:
			PORTD &= ~(0x01 << PIN_CLK);
			break;
		case STATE_H:
			PORTD |= (0x01 << PIN_CLK);
			break;
		default:
			return;
	}	
}


void setup_pin_rst(void){
	DDRB |= (0x01 << PIN_RST);    /* Output    */
	PINB &= ~(0x01<<PIN_RST);     /* No Toggle */
}


void set_rst_state(uint8_t state){
	switch(state){
		case STATE_H:
			PORTB |= (0x01 << PIN_RST);
			break;
		case STATE_L:
			PORTB &= ~(0x01 << PIN_RST);
			break;
		default:
			return;
	}	
}


void setup_pin_io(uint8_t type){
	switch(type){
		case INPUT_PIN:
			DDRD &= ~(0x01 << PIN_IO);       /* Input     */
			PORTD |= (0X01 << PIN_IO);       /* Pull Up   */
			/*PORTB &= ~(0X01 << PIN_IO);       Tri-State */
			break;
		case OUTPUT_PIN:
			DDRD |= (0x01 << PIN_IO);        /* Output    */
			PIND &= ~(0x01 << PIN_IO);       /* No Toggle */
			break;
		default:
			return;
	}	
}

void set_io_state(uint8_t state){
	switch(state){
		case STATE_H:
			PORTD |= (0x01 << PIN_IO);
			break;
		case STATE_L:
			PORTD &= ~(0x01 << PIN_IO);
			break;
		default:
			return;
	}
}


void setup_pin_vcc(void){
	DDRB |= (0x01 << PIN_VCC);    /* Output     */
	PINB &= ~(0x01 << PIN_VCC);   /* No Toggle  */
}

void set_vcc(uint8_t state){
	switch(state){
		case ON:
			PORTB |= (0x01 << PIN_VCC);
			break;
		case OFF:
			PORTB &= ~(0x01 << PIN_VCC);
			break;
		default:
			return;
	}
}


void wait_cycles(uint16_t nb_cycles, uint32_t f_card){
	/* On stoppe le timer 1 (Prescaler = 0)                                  */
	/* TCCR1B.CS1[2:0] = 0x00 */
	TCCR1B &= ~((0x01<<CS12) | (0x01<<CS11) | (0x01<<CS10));      
	
	/* On met le timer 1 en mode CTC (Clear Timer On Compare Match)          */
	/* TCCR1x.WGM[3:0] = 0x04                                                */
	TCCR1B = ((TCCR1B & ~(0x01<<WGM13)) | (0x01<<WGM12));
	TCCR1A = ((TCCR1A & ~(0x01<<WGM11)) & ~(0x01<<WGM10));		
	
	/* On clear le flag OCFA (Dans reg TIFR1) (Output Compare Match Flag) */
	TIFR1 |= (0x01<<OCF1A);		
	
	/* On initialise le timer 1 a 0 (TCNT1)                                  */
	TCNT1 = 0x0000;													 
																		 
	/* On fixe la valeur du comparateur A (On charge H avant L)              */
	OCR1A = nb_cycles * (F_CPU / f_card);
																			 																	 
	/* On demarre le compteur (Prescaler = 1)                                */
	/* TCCR1B.CS1[2:0] = 0x02                                                */
	TCCR1B = (((TCCR1B & ~(0x01<<CS12)) & ~(0x01<<CS11)) | (0x01<<CS10));
													 
	/* On attend que OCFA (Output Compare Match Flag) passe a 1              */
	while(!(TIFR1 & (0x01<<OCF1A)));	
}



void wait_cycles_long(uint32_t nb_cycles, uint32_t f_card){
	uint32_t i,n,p;
	
	n = nb_cycles / MAX_WAIT_CYCLES;
	p = nb_cycles % MAX_WAIT_CYCLES;
	
	for(i=0; i<n; i++){
		wait_cycles(MAX_WAIT_CYCLES, f_card);
	}
	wait_cycles(p, f_card);
}


void do_activation(void){
	setup_clock_config(F_CARD);
	setup_pin_vcc();
	setup_pin_rst();
	
	
	set_clock_state(STATE_L);
	set_vcc(OFF);
	set_rst_state(STATE_L);
	
	wait_cycles(50, F_CARD);
	
	set_vcc(ON);
	wait_cycles(2000, F_CARD);
	/* setup_pin_io(INPUT_PIN); */
	usart_init(F_CARD);
	wait_cycles(200, F_CARD);
	set_clock(ON);
}

void do_cold_reset(void){
	set_rst_state(STATE_L);
	wait_cycles(COLD_RESET_RST_NB_CYCLES, F_CARD);
	set_rst_state(STATE_H);
}

void do_warm_reset(void){
	
}


void blink_led(uint16_t n){
	uint8_t i;
	uint16_t j;
	
	for(j=0; j<n; j++){
		for(i=0; i<3; i++){
			DDRB = DDRB | BIT_5;                                     /* On mets DDRB5 a 1 (output)      */
			PORTB = PORTB & (~BIT_5);                                /* On mets PORTB5 a 0 (Driven Low) */
																	
			PINB = PINB & (~BIT_5);                                  /* On mets PINB5 a 0 (state L)     */
			wait_cycles_long(50000, F_CARD);
			PINB = PINB | BIT_5;
			wait_cycles_long(50000, F_CARD);
		}
		wait_cycles_long(500000, F_CARD);
	}
}
