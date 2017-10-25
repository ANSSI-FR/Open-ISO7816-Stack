#ifndef _TEST_H_
#define _TEST_H_


void loop(void);
void setup(void);

void setup_clock_config(uint32_t f_card);
void set_clock_on(void);
void set_clock_off(void);
void set_clock_state(uint8_t state);

void setup_pin_rst(void);
void set_rst_state(uint8_t state);

void setup_io_in(void);
void setup_io_out(void);
void set_io_state(uint8_t);

void setup_pin_vcc(void);
void set_vcc(uint8_t state);

void do_activation(void);
void do_cold_reset(void);
void do_warm_reset(void);


// Patiente/bloque n cycles + duree necessaire pour config le timer
// peut attendre au maximum 8191 cycles
void wait_cycles(uint16_t nb_cycles, uint32_t f_card);


void usart_set_baudrate(uint16_t F, uint16_t D, uint32_t f_card);


#endif
