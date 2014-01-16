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
#include "kbd/keymap.h"

#define LED_CONFIG	(DDRD |= (1<<6))
#define LED_ON		(PORTD |= (1<<6))
#define LED_OFF		(PORTD &= ~(1<<6))
#define CPU_PRESCALE(n) (CLKPR = 0x80, CLKPR = (n))

volatile unsigned char hid_report_in[HID_INT_IN_EP_SIZE];
uint8_t use_debug = 0;

void usb_print(const char *s);
uint8_t recv_str(char *buf, uint8_t size);
void parse_and_execute_command(const char *buf, uint8_t num);

void device_init(void) {
	// set for 16 MHz clock, and turn on the LED
	CPU_PRESCALE(0);

	LED_CONFIG;
    LED_ON;

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
    // uart_putc(SUN_COMMAND_RESET);

    // Finally, display an initialization version and turn off the led
    print("Sun Type 3/4/5 USB Keyboard converter initialized\n");
    LED_OFF;
}

// Basic command interpreter for controlling port pins
int main (void) {
	uint8_t c, received_keys;

    device_init();

	while (1) {

        received_keys = 0;
        while (key_slot_available()) {
            if (uart_available()) {
                c = uart_getc();

                if (!c) {
                    break;
                }

                received_keys = 1;
                if (!update_keyboard_keys(c)) {
                    print("failed to update keyboard_keys: ");
                    phex(c);
                    print("\n");
                }
            } else {
                break;
            }
        }

        if (received_keys) {
            if (keys_pressed()) LED_ON;
            transmit_keyboard_buffer();
            if (keys_pressed()) LED_OFF;

        }

	}
}
