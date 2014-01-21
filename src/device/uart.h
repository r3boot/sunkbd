#ifndef _uart_h_
#define _uart_h_

#include <stdint.h>

#define KBD_BAUDRATE    1200

void uart_init(uint32_t baud);
void uart_putc(uint8_t c);
uint8_t uart_getc(void);
uint8_t uart_available(void);
uint8_t uart_tx_busy(void);
void update_keyboard_buffer(unsigned char key);
void remove_keyboard_buffer(unsigned char key);

#endif
