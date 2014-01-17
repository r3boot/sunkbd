/* Simple example for Teensy USB Development Board
 * http://www.pjrc.com/teensy/
 * Copyright (c) 2008 PJRC.COM, LLC
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

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <util/delay.h>
#include "usb/usb_keyboard.h"
#include "usb/usb_config.h"
#include "usb/print.h"
#include "kbd/keyboard.h"
#include "kbd/uart.h"
#include "kbd/keymap.h"

#define RESET_CONFIG    (DDRD |= (0<<1))    // D1
#define RESET_PULLUP    (PORTD |= (1<<1))

#define CFG_1_CONFIG    (DDRB |= (0<<4))    // B4
#define CFG_2_CONFIG    (DDRB |= (0<<5))    // B5
#define CFG_3_CONFIG    (DDRB |= (0<<6))    // B6
#define CFG_4_CONFIG    (DDRF |= (0<<7))    // F7

#define CFG_1_PULLUP    (PORTB |= (1<<4))
#define CFG_2_PULLUP    (PORTB |= (1<<5))
#define CFG_3_PULLUP    (PORTB |= (1<<6))
#define CFG_4_PULLUP    (PORTF |= (1<<7))

#define CFG_1_ENABLED   ~(PINB & (1<<4))
#define CFG_2_ENABLED   ~(PINB & (1<<5))
#define CFG_3_ENABLED   ~(PINB & (1<<6))
#define CFG_4_ENABLED   ~(PINF & (1<<7))

#define TX_LED_CONFIG	(DDRD |= (1<<6))    // D6
#define TX_LED_ON		(PORTD |= (1<<6))
#define TX_LED_OFF		(PORTD &= ~(1<<6))

#define RX_LED_CONFIG	(DDRD |= (1<<7))    // D7
#define RX_LED_ON		(PORTD |= (1<<7))
#define RX_LED_OFF		(PORTD &= ~(1<<7))

#define CPU_PRESCALE(n) (CLKPR = 0x80, CLKPR = (n))

void( *device_reset) (void) = 0;

// Reset interrupt
ISR(INT1_vect)
{
    device_reset();
}

void device_init(void) {
	// set for 16 MHz clock, and turn on the LED
	CPU_PRESCALE(0);

    RESET_CONFIG;
    RESET_PULLUP;

    CFG_1_CONFIG;
    CFG_2_CONFIG;
    CFG_3_CONFIG;
    CFG_4_CONFIG;

    CFG_1_PULLUP;
    CFG_2_PULLUP;
    CFG_3_PULLUP;
    CFG_4_PULLUP;

	RX_LED_CONFIG;
    TX_LED_CONFIG;

    RX_LED_ON;

    // Use USB raw-hid to transfer scancodes from the keyboard
    usb_init();
    while (!usb_configured()) /* wait */ ;

    // Wait an extra second for the PC's operating system to load drivers
    // and do whatever it does to actually be ready for input
    _delay_ms(1000);

    // Configure timer 0 to generate a timer overflow interrupt every
    // 256*1024 clock cycles, or approx 61 Hz when using 16 MHz clock
    /* TCCR0A = 0x00;
    TCCR0B = 0x05;
    TIMSK0 = (1<<TOIE0); */

    // initialize the keyboard uart
    uart_init(1200);

    if (CFG_1_ENABLED) {
        print("keyclicks enabled\n");
    } else {
        print("keyclicks disabled\n");
    }

    if (CFG_4_ENABLED) {
        print("debugging enabled\n");
    } else {
        print("debugging disabled\n");
    }

    // Finally, display an initialization version and turn off the led
    print("Sun Type 3/4/5 USB Keyboard converter initialized\n");
    RX_LED_OFF;
}

// Basic command interpreter for controlling port pins
int main (void) {
	uint8_t sc, received_keys;

    // Initialize all peripherals
    device_init();

    // Reset keyboard
    reset_keyboard();

    // Enter main processing loop
	while (1) {

        received_keys = 0;
        while (key_slot_available()) {
            if (uart_available()) {
                sc = uart_getc();

                if (!sc) {
                    break;
                }

                received_keys = 1;
                if (!process_scancode(sc)) {
                    print("failed to process scancode: ");
                    phex(sc);
                    print("\n");
                }
            } else {
                break;
            }
        }

        if ((kbd_initialized) && (received_keys)) {
            if (keys_pressed()) TX_LED_ON;
            transmit_keyboard_buffer();
            if (keys_pressed()) TX_LED_OFF;

        }

	}
}
