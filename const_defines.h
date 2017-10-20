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
#define PIN_IO  3             // PORTB   Digital pin 11
#define PIN_VCC 2             // PORTB   Digital pin 10


#define STATE_H 1
#define STATE_L 0

#define COLD_RESET_RST_NB_CYCLES 18000 // ISO7816-3 Section 6.2.1 (400 cycles mini)


#endif
