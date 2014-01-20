/* UART Example for Teensy USB Development Board
 * http://www.pjrc.com/teensy/
 * Copyright (c) 2009 PJRC.COM, LLC
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

// Version 1.0: Initial Release
// Version 1.1: Add support for Teensy 2.0, minor optimizations


#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <stdio.h>

#include "kbd/buffers.h"
#include "kbd/keyboard.h"
#include "kbd/uart.h"
#include "kbd/keymap.h"
#include "usb/usb_keyboard.h"
#include "usb/print.h"


// Initialize the UART
void uart_init(uint32_t baud) {
	cli();
	UBRR1 = (F_CPU / 4 / baud - 1) / 2;
	UCSR1A = (1<<U2X1);
	UCSR1B = (1<<RXEN1) | (1<<TXEN1) | (1<<RXCIE1);
	UCSR1C = (1<<UCSZ11) | (1<<UCSZ10);
	sei();
}

// Transmit a byte
void uart_putc(uint8_t c) {
    tx_buffer_put(c);
	UCSR1B = (1<<RXEN1) | (1<<TXEN1) | (1<<RXCIE1) | (1<<UDRIE1);
}

// Receive a byte
uint8_t uart_getc(void) {
    if (rx_buffer_empty()) {
        print("rx_buffer is empty\n");
        return 0;
    }

    return rx_buffer_get();
}

// Return the number of bytes waiting in the receive buffer.
// Call this before uart_getchar() to check if it will need
// to wait for a byte to arrive.
uint8_t uart_available(void) {
    return rx_buffer_count() > 0;
}

uint8_t uart_tx_busy(void) {
    return rx_buffer_count() + 1;
}

// Transmit Interrupt
ISR(USART1_UDRE_vect) {
	if (tx_buffer_empty()) {
		// buffer is empty, disable transmit interrupt
		UCSR1B = (1<<RXEN1) | (1<<TXEN1) | (1<<RXCIE1);
	} else {
		UDR1 = tx_buffer_get();
	}
}

// Receive Interrupt
ISR(USART1_RX_vect) {
    uint8_t scancode;

    scancode = UDR1;

    if ((scancode == SUN_KEY_LEFT_META) || (RELEASETOPRESS(scancode) == SUN_KEY_LEFT_META))
        // TODO: l_alt+l_meta sent both at the same time
        return;

    print("received scancode\n");
    rx_buffer_put(scancode);
}
