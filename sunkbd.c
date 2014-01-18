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
#include <avr/wdt.h>
#include <stdint.h>
#include <util/delay.h>
#include "usb/usb_keyboard.h"
#include "usb/usb_config.h"
#include "usb/print.h"
#include "kbd/keyboard.h"
#include "kbd/uart.h"
#include "kbd/keymap.h"

#define RESET_CONFIG    (DDRD |= (0<<PB0))    // D0
#define RESET_PULLUP    (PORTD |= (1<<PB0))
#define RESET_PRESSED   (PINB & (1<<PB0))

#define CFG_1_CONFIG    (DDRB |= (0<<PB1))
#define CFG_2_CONFIG    (DDRB |= (0<<PB2))
#define CFG_3_CONFIG    (DDRB |= (0<<PB3))

#define CFG_1_PULLUP    (PORTB |= (1<<PB1))
#define CFG_2_PULLUP    (PORTB |= (1<<PB2))
#define CFG_3_PULLUP    (PORTB |= (1<<PB3))

#define CFG_1_OPEN      (PINB & (1<<PB1))
#define CFG_2_OPEN      (PINB & (1<<PB2))
#define CFG_3_OPEN      (PINB & (1<<PB3))

#define TX_LED_CONFIG	(DDRF |= (1<<PF0))    // B4
#define TX_LED_ON		(PORTF &= ~(1<<PF0))
#define TX_LED_OFF		(PORTF |= (1<<PF0))

#define RX_LED_CONFIG	(DDRF |= (1<<PF1))    // B5
#define RX_LED_ON		(PORTF &= ~(1<<PF1))
#define RX_LED_OFF		(PORTF |= (1<<PF1))

#define CPU_PRESCALE(n) (CLKPR = 0x80, CLKPR = (n))

volatile uint8_t click_enable = 0;
volatile uint8_t debug_enable = 0;
volatile uint8_t loader_enable = 0;

void parse_jumper_config(void)
{
    if (!CFG_1_OPEN) {
        click_enable = 1;
        print("Key clicks enabled\n");
    } else {
        click_enable = 0;
        print("Key clicks disabled\n");
    }

    if (!CFG_2_OPEN) {
        debug_enable = 1;
        print("Debugging enabled\n");
    } else {
        debug_enable = 0;
        print("Debugging disabled\n");
    }

    if (!CFG_3_OPEN) {
        loader_enable = 1;
        print("Bootloader support enabled\n");
    } else {
        loader_enable = 0;
        print("Bootloader support disabled\n");
    }
}

// Reset interrupt
ISR(PCINT0_vect)
{
    if (RESET_PRESSED) {
        cli();
        if (loader_enable) {
            cli();
            UDCON = 1;
            USBCON = (1<<FRZCLK);  // disable USB
            UCSR1B = 0;
            _delay_ms(5);
            EIMSK = 0; PCICR = 0; SPCR = 0; ACSR = 0; EECR = 0; ADCSRA = 0;
            TIMSK0 = 0; TIMSK1 = 0; TIMSK3 = 0; TIMSK4 = 0; UCSR1B = 0; TWCR = 0;
            DDRB = 0; DDRC = 0; DDRD = 0; DDRE = 0; DDRF = 0; TWCR = 0;
            PORTB = 0; PORTC = 0; PORTD = 0; PORTE = 0; PORTF = 0;
            asm volatile("jmp 0x7E00");
        } else {
            TX_LED_ON;
            print("Performing software reset\n");
            parse_jumper_config();
            print("Sun Type 3/4/5 USB Keyboard converter initialized\n");
            TX_LED_OFF;
        }
        sei();
    }
}

void device_init(void) {
	// set for 16 MHz clock, and turn on the LED
	CPU_PRESCALE(0);

    // Enable reset button
    RESET_CONFIG;
    RESET_PULLUP;

    PCICR |= (1 << PCIE0); /* Activate interrupt on enabled PCINT7-0 */
    PCMSK0 |= (1 << PCINT0); /* Enable PCINT0 */
    sei();

    CFG_1_CONFIG;
    CFG_2_CONFIG;
    CFG_3_CONFIG;

    CFG_1_PULLUP;
    CFG_2_PULLUP;
    CFG_3_PULLUP;

	RX_LED_CONFIG;
    TX_LED_CONFIG;
    RX_LED_OFF;

    PCMSK0 |= (1 << PCINT0); /* Enable PCINT0 */
    PCICR |= (1 << PCIE0); /* Activate interrupt on enabled PCINT7-0 */

    TX_LED_ON;

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

    // Parse jumper-based configuration
    parse_jumper_config();

    // Finally, display an initialization version and turn off the led
    print("Sun Type 3/4/5 USB Keyboard converter initialized\n");
    TX_LED_OFF;
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
                print("got scancode\n");

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
