#ifndef _CONST_DEFINES_H_
#define _CONST_DEFINES_H_


#define BIT_0 0x01
#define BIT_1 0x02
#define BIT_2 0x04
#define BIT_3 0x08
#define BIT_4 0x10
#define BIT_5 0x20
#define BIT_6 0x40
#define BIT_7 0x80


#define PIN_RST 0             // PORTB   Digital pin 8
#define PIN_CLK 6             // PORTD   ie Pin OC0A !
#define PIN_IO  0             // PORTD   Digital pin 0
#define PIN_VCC 2             // PORTB   Digital pin 10


#define STATE_H 1
#define STATE_L 0
#define ON 1
#define OFF 0
#define INPUT_PIN 0
#define OUTPUT_PIN 1

#define F_CARD 2000000  // Hz
//#define F_CPU 16000000  // Hz


#define F_DEFAULT 372                // cock rate convertion integer
#define D_DEFAULT 1                  // baud rate adjustement integer
#define COLD_RESET_RST_NB_CYCLES 500 // ISO7816-3 Section 6.2.1 (400 cycles mini)


#define MAX_WAIT_CYCLES 8191


#endif
