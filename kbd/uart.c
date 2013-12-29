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

#include "kbd/keyboard.h"
#include "kbd/uart.h"
#include "kbd/keymap.h"
#include "usb/usb_serial.h"

static unsigned char initialized = 0;

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
void uart_init(uint32_t baud)
{
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
void uart_putc(uint8_t c)
{
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
uint8_t uart_getc(void)
{
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

void uart_dump_hex(unsigned char key) {
    char buf[12];
    snprintf(buf, sizeof(buf), "key: 0x%02X\r\n", (int)key);
    for (int i=0; i < sizeof(buf); i++) {
        usb_serial_putchar(buf[i]);
    }
}

// Transmit Interrupt
ISR(USART1_UDRE_vect)
{
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
ISR(USART1_RX_vect)
{
    unsigned char key;

    key = UDR1;

    uart_dump_hex(key);
    // usb_print(key);

    if (initialized < 3) {
        // Handle keyboard initialization codes
        // In case of success, we will receive three codes.
        // In case of failure, two, so the initialization sequence
        // will not complete.
        if(key==RESPONSE_INIT_FAILURE_1)
            fillKeyArray(HID_ERR_POST);
        initialized++;
    } else {
        unsigned char hidKey;
        if(doPowerButton(key)) {
            usb_print(PSTR("power button\r\n"));
            //TODO: RCSTAbits.CREN=1; //enable recv
            return;
        }

        if (key < KEY_RELEASED) {
            usb_print(PSTR("key pressed\r\n"));
            // Key is pressed
            hidKey = keycode[key];
            // hidKey = key;

            if (!doModifiers(hidKey) && !doMediaButtons(key)) {
                // Handle modifier keys seperately
                unsigned char i;
                for (i=HID_ARRAY_START; i<HID_ARRAY_START+HID_MAX_KEYS; i++) {
                    // Look for a free keycode slot
                    if (key_buffer[i]==HID_NO_EVENT) {
                        key_buffer[i] = hidKey;
                        break;
                    }
                }
            }
        } else if (key > KEY_RELEASED) {
            usb_print(PSTR("key released\r\n"));
            // Key is released
            hidKey = keycode[RELEASETOPRESS(key)];
            if (!doModifiers(hidKey) && !doMediaButtons(RELEASETOPRESS(key))) {
                unsigned char i;
                for (i=HID_ARRAY_START;i<HID_ARRAY_START+HID_MAX_KEYS;i++) {
                    // Remove the released key from the buffer
                    if (key_buffer[i] == hidKey) {
                        key_buffer[i]=HID_NO_EVENT;
                        // break;
                        // don't break so if for some reason a key was in
                        // the array twice (should not happen) both instances
                        // are cleared
                    }
                }
            }
        }
    }

    /*
	uint8_t c, i;

	c = UDR1;
	i = rx_buffer_head + 1;
	if (i >= RX_BUFFER_SIZE) i = 0;
	if (i != rx_buffer_tail) {
		rx_buffer[i] = c;
		rx_buffer_head = i;
	}
    */
}

