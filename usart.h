#ifndef _USART_H_
#define _USART_H_


#define USART_SAMPLE_ON_FALLING_EDGE 0
#define USART_SAMPLE_ON_RISING_EDGE 1


void usart_set_baudrate(uint16_t F, uint16_t D, uint32_t f_card);
void usart_set_mode(uint8_t mode);
void usart_set_frame_format(void);
void usart_set_receiver(uint8_t state);
void usart_set_receive_interrupt(uint8_t state);
void usart_set_sampling_mode(uint8_t mode);

#endif
