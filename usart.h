#ifndef _USART_H_
#define _USART_H_


#define USART_SAMPLE_ON_FALLING_EDGE 0
#define USART_SAMPLE_ON_RISING_EDGE 1

#define USART_MODE_SYNCHRONOUS_MASTER 0
#define USART_MODE_SYNCHRONOUS_SLAVE 1
#define USART_MODE_ASYNCHRONOUS_SIMPLE_SPEED 2
#define USART_MODE_ASYNCHRONOUS_DOUBLE_SPEED 3

#define XCK_DDR DDRD
#define XCK_BIT 4              /* Datasheet section 24.4.0 */


void usart_init(uint32_t f_card);
void usart_set_baudrate_etu(uint16_t F, uint16_t D, uint32_t f_card);
void usart_set_baudrate(uint32_t bauds, uint32_t f_card);
void usart_set_mode(uint8_t mode);
void usart_set_frame_format(void);
void usart_set_receiver(uint8_t state);
void usart_set_transmitter(uint8_t state);
void usart_set_receive_interrupt(uint8_t state);
void usart_set_sampling_mode(uint8_t mode);
void usart_get_receiver_error_flags(uint8_t *flag_FE, uint8_t *flag_DOR, uint8_t *flag_UPE);
void usart_wait_transmitter_ready(void);
void usart_polling_send_byte(uint8_t byte);
void usart_polling_send_frame(uint8_t *frame, uint16_t count);
void usart_polling_get_frame(uint32_t nb_bytes, uint8_t *bytes_buffer);


uint8_t rx_buffer[100];
uint8_t rx_counter;

#endif
