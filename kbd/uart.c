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

#include "kbd/keyboard.h"
#include "kbd/uart.h"
#include "kbd/keymap.h"
#include "usb/usb_keyboard.h"
#include "usb/print.h"

// These buffers may be any size from 2 to 256 bytes.
#define RX_BUFFER_SIZE 64
#define TX_BUFFER_SIZE 40

static volatile uint8_t tx_buffer[TX_BUFFER_SIZE];
static volatile uint8_t tx_buffer_head;
static volatile uint8_t tx_buffer_tail;
static volatile uint8_t rx_buffer[RX_BUFFER_SIZE];
static volatile uint8_t rx_buffer_head;
static volatile uint8_t rx_buffer_tail;

// Initialize the UART
void uart_init(uint32_t baud) {
	cli();
	UBRR1 = (F_CPU / 4 / baud - 1) / 2;
	UCSR1A = (1<<U2X1);
	UCSR1B = (1<<RXEN1) | (1<<TXEN1) | (1<<RXCIE1);
	UCSR1C = (1<<UCSZ11) | (1<<UCSZ10);
	tx_buffer_head = tx_buffer_tail = 0;
	rx_buffer_head = rx_buffer_tail = 0;
	sei();
}

// Transmit a byte
void uart_putc(uint8_t c) {
	uint8_t i;

	i = tx_buffer_head + 1;
	if (i >= TX_BUFFER_SIZE) i = 0;
	while (tx_buffer_tail == i) ; // wait until space in buffer
	//cli();
	tx_buffer[i] = c;
	tx_buffer_head = i;
	UCSR1B = (1<<RXEN1) | (1<<TXEN1) | (1<<RXCIE1) | (1<<UDRIE1);
	//sei();
}

// Receive a byte
uint8_t uart_getc(void) {
    uint8_t c, i;

	while (rx_buffer_head == rx_buffer_tail) ; // wait for character

    i = rx_buffer_tail + 1;
    if (i >= RX_BUFFER_SIZE) i = 0;
    c = rx_buffer[i];
    rx_buffer_tail = i;
    return c;
}

// Return the number of bytes waiting in the receive buffer.
// Call this before uart_getchar() to check if it will need
// to wait for a byte to arrive.
uint8_t uart_available(void) {
	uint8_t head, tail;

	head = rx_buffer_head;
	tail = rx_buffer_tail;
	if (head >= tail) return head - tail;
	return RX_BUFFER_SIZE + head - tail;
}

uint8_t uart_tx_busy(void) {
    return tx_buffer_head + 1;
}

// Transmit Interrupt
ISR(USART1_UDRE_vect) {
	uint8_t i;

	if (tx_buffer_head == tx_buffer_tail) {
		// buffer is empty, disable transmit interrupt
		UCSR1B = (1<<RXEN1) | (1<<TXEN1) | (1<<RXCIE1);
	} else {
		i = tx_buffer_tail + 1;
		if (i >= TX_BUFFER_SIZE) i = 0;
		UDR1 = tx_buffer[i];
		tx_buffer_tail = i;
	}
}

// Receive Interrupt
ISR(USART1_RX_vect) {
    uint8_t c, i;

    c = UDR1;
    i = rx_buffer_head + 1;
    if (i >= RX_BUFFER_SIZE) i = 0;
    if (i != rx_buffer_tail) {
        rx_buffer[i] = c;
        rx_buffer_head = i;
    }

    /*
    unsigned char key;

    key = UDR1;

    unsigned char hidKey;

    if(doPowerButton(key)) {
        //TODO: RCSTAbits.CREN=1; //enable recv
        return;
    }

    if (key < SUN_KEY_RELEASED) {
        // Key is pressed
        hidKey = keycode[key];

        if (!doModifiers(hidKey) && !doMediaButtons(key)) {
            print("key pressed: ");
            phex(hidKey);
            print("\n");

            update_keyboard_buffer(hidKey);
        }
    } else if (key > SUN_KEY_RELEASED) {
        // Key is released
        hidKey = keycode[RELEASETOPRESS(key)];

        if (!doModifiers(hidKey) && !doMediaButtons(RELEASETOPRESS(key))) {
            print("key released: ");
            phex(hidKey);
            print("\n");

            remove_keyboard_buffer(hidKey);
        }
    }
    */
}

void update_keyboard_buffer(unsigned char key) {
    unsigned char i;
    cli();
    for(i=0; i<=KEYBOARD_KEYS_MAX; i++) {
        if (keyboard_keys[i] == HID_NO_EVENT) {
            keyboard_keys[i] = key;
            return;
        }
    }
    sei();
    print("keyboard_keys overflow\n");
}

void remove_keyboard_buffer(unsigned char key) {
    unsigned char i;
    cli();
    for(i=0; i<=KEYBOARD_KEYS_MAX; i++) {
        if (keyboard_keys[i] == key) {
            keyboard_keys[i] = HID_NO_EVENT;
            return;
        }
    }
    print("failed to remove keyboard key\n");
    for(i=0; i<=KEYBOARD_KEYS_MAX; i++)
        keyboard_keys[i] = HID_NO_EVENT;
    sei();
}
