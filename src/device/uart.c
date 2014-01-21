#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <stdio.h>

#include "device/uart.h"
#include "buffers/queues.h"
#include "buffers/keyboard.h"

#include "include/sun_scancodes.h"
#include "debug/print.h"


// Initialize the UART
void uart_init(uint32_t baud) {
	cli();
	UBRR1 = (F_CPU / 4 / baud - 1) / 2;
	UCSR1A = (1<<U2X1);
	UCSR1B = (1<<RXEN1) | (1<<TXEN1) | (1<<RXCIE1);
	UCSR1C = (1<<UCSZ11) | (1<<UCSZ10);
	sei();
}

// Add a scancode to the transmit buffer and trigger the TX interrupt
void uart_putc(uint8_t sc) {
    q_put(TX_QUEUE, sc);
	UCSR1B = (1<<RXEN1) | (1<<TXEN1) | (1<<RXCIE1) | (1<<UDRIE1);
}

// Receive a byte from the receive buffer
uint8_t uart_getc(void) {
    if (q_empty(RX_QUEUE)) {
        print("rx_buffer is empty\n");
        return 0;
    }

    return q_get(RX_QUEUE);
}

// Return the number of bytes waiting in the receive buffer.
// Call this before uart_getchar() to check if it will need
// to wait for a byte to arrive.
uint8_t uart_available(void) {
    return q_count(RX_QUEUE) > 0;
}

// Return the number of bytes waiting in the send buffer.
uint8_t uart_tx_busy(void) {
    return q_count(TX_QUEUE) + 1;
}

// Transmit Interrupt
// Receive a byte from the tramsmit buffer and transmit it. Disable the
// transmit interrupt if the tx buffer is empty.
ISR(USART1_UDRE_vect) {
	if (q_empty(TX_QUEUE))
		// buffer is empty, disable transmit interrupt
		UCSR1B = (1<<RXEN1) | (1<<TXEN1) | (1<<RXCIE1);
	else
		UDR1 = q_get(TX_QUEUE);
}

// Receive Interrupt
// Read a byte from the uart and put it in the receive buffer.
ISR(USART1_RX_vect) {
    uint8_t sc = 0;

    sc = UDR1;

    if ((sc == SUN_KEY_LEFT_META) || (RELEASETOPRESS(sc) == SUN_KEY_LEFT_META))
        // TODO: l_alt+l_meta sent both at the same time
        return;

    q_put(RX_QUEUE, sc);
}
