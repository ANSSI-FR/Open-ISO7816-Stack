#ifndef _TEST_H_
#define _TEST_H_


void loop(void);
void setup(void);

void setup_clock_config(void);
void set_clock_on(void);
void set_clock_off(void);
void set_clock_L(void);
void set_clock_H(void);

void setup_pin_rst(void);
void set_rst_state_H(void);
void set_rst_state_L(void);

void setup_io_in(void);
void setup_io_out(void);
void set_io_state(uint8_t);

void setup_pin_vcc(void);
void set_vcc_on(void);
void set_vcc_off(void);

void do_activation(void);
void do_cold_reset(void);
void do_warm_reset(void);


// Patiente/bloque n cycles + duree necessaire pour config le timer
void wait_cycles(uint16_t nb_cycles);


#endif
