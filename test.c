/* Boris Simunovic
 * 11/10/2017
 */


#include <avr/io.h>

#include "test.h"
#include "const_defines.h"


void setup(void){	
	do_activation();
	do_cold_reset();
}


void loop(){
	
}


void setup_clock_config(void){	
	/* PD6 en sortie (pin OC0A) */
	DDRD |= (0x01 << PIN_CLK);              /* On mets DDRD6 a 1 (output)                 */
	PIND &= ~(0x01 << PIN_CLK);             /* No Toggle                                  */
																	          
	/* Configuration du timer 0 en mode CTC (TCCR0x.WGM[2:0] = 0x2)                       */
	TCCR0B &= ~(0x01<<WGM02);         /* WGM2 = 0                                         */
	TCCR0A = (TCCR0A | (0x01<<WGM01)) & ~(0x01<<WGM00);
																	          
	/* Configuration du pin OC0A en toggle (TCCRA.COM[1:0] = 0x1)             */
	TCCR0A = (TCCR0A & ~(0x01<<COM0A1)) | (0x01<<COM0A0);
																	          
	/* Comparateur A du timer 0 a 3 pour diviser clock par 8                  */
	OCR0A = 0x03;  // 2MHz
}



void set_clock(uint8_t state){
	switch(state){
		case ON: 
			/* Positionnement du prescaler du timer 0 a 1 (TCCR0B.CS0[2:0] = 0x1)      */
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
	DDRB |= (0x01 << PIN_RST);    // Output
	PINB &= ~(0x01<<PIN_RST);     // No Toggle
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


void setup_io_in(void){
	DDRB &= ~(0x01 << PIN_IO); // Input
	PORTB |= (0X01 << PIN_IO); // Pull Up
	//PORTB &= ~(0X01 << PIN_IO); // Tri-State
}

void setup_io_out(void){
	DDRB |= (0x01 << PIN_IO);   // Output
	PINB &= ~(0x01 << PIN_IO);  // No Toggle
}

void set_io_state_H(void){
	PORTB |= (0x01 << PIN_IO);
}

void set_io_state_L(void){
	PORTB &= ~(0x01 << PIN_IO);
}



void setup_pin_vcc(void){
	DDRB |= (0x01 << PIN_VCC);    // Output
	PINB &= ~(0x01 << PIN_VCC); // No Toggle
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



void wait_cycles(uint16_t nb_cycles){
	/* On stoppe le timer 1 (Prescaler = 0)                                  */
	/* TCCR1B.CS1[2:0] = 0x00 */
	TCCR1B &= ~((0x01<<CS12) | (0x01<<CS11) | (0x01<<CS10));      
	
	/* On met le timer 1 en mode CTC (Clear Timer On Compare Match)          */
	/* WGM13 = 0                                    */
	/* WGM12 = 1                                    */
	/* WGM11 = 0                                    */
	/* WGM10 = 0                                    */
	TCCR1B = ((TCCR1B & ~(0x01<<WGM13)) | (0x01<<WGM12));
	TCCR1A = ((TCCR1A & ~(0x01<<WGM11)) & ~(0x01<<WGM10));		
	
	/* On positionne le flag OCFA (Dans reg TIFR1) (Output Compare Match Flag) a 0            */
	TIFR1 |= (0x01<<OCF1A);		
	
	/* On initialise le timer 1 a 0 (TCNT1)                                  */
	TCNT1 = 0x0000;													 
																		 
	/* On fixe la valeur du comparateur A (On charge H avant L)              */
	OCR1A = nb_cycles;
																			 
																			 
	/* On demarre le compteur (Prescaler = 8)                                */
	/* CS12 = 0                                     */
	/* CS11 = 1                                     */
	/* CS10 = 0                                     */
	TCCR1B = (((TCCR1B & ~(0x01<<CS12)) | (0x01<<CS11)) & ~(0x01<<CS10));
													 
	/* On attend que OCFA (Output Compare Match Flag) passe a 1              */
	while(!(TIFR1 & (0x01<<OCF1A)));	
}



void do_activation(void){
	setup_clock_config();
	setup_pin_vcc();
	setup_pin_rst();
	//setup_io_out();
	
	
	set_clock_state(STATE_L);
	set_vcc(OFF);
	set_rst_state(STATE_L);
	//set_io_state_L();
	
	wait_cycles(50);
	
	set_vcc(ON);
	wait_cycles(2000);
	setup_io_in();
	wait_cycles(200);
	set_clock(ON);
}

void do_cold_reset(void){
	set_rst_state(STATE_L);
	wait_cycles(COLD_RESET_RST_NB_CYCLES);
	set_rst_state(STATE_H);
}

void do_warm_reset(void){
	
}


//void setup_usart_receiver(void){
	/* Setting the baudrate */
	//UBRR0 = (F_DEFAULT / D_DEFAULT) - 1;  // Table 24-1 section 24.4.1
	
	/* Setting the frame format*/
	/* Enablinf the receiver */
//}
