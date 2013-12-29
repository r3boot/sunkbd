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
#include <stdint.h>
#include <util/delay.h>
#include "usb/usb_serial.h"
#include "usb/usb_config.h"
#include "kbd/keyboard.h"

#define LED_CONFIG	(DDRD |= (1<<6))
#define LED_ON		(PORTD |= (1<<6))
#define LED_OFF		(PORTD &= ~(1<<6))
#define CPU_PRESCALE(n) (CLKPR = 0x80, CLKPR = (n))

volatile unsigned char hid_report_in[HID_INT_IN_EP_SIZE];
uint8_t use_debug = 0;

void usb_print(const char *s);
uint8_t recv_str(char *buf, uint8_t size);
void parse_and_execute_command(const char *buf, uint8_t num);

#if 0
// Very simple character echo test
int main(void)
{
	CPU_PRESCALE(0);
	usb_init();
	while (1) {
		int n = usb_serial_getchar();
		if (n >= 0) usb_serial_putchar(n);
	}
}

#else

// Basic command interpreter for controlling port pins
int main(void)
{
	char buf[32];
	uint8_t n;

	// set for 16 MHz clock, and turn on the LED
	CPU_PRESCALE(0);

	LED_CONFIG;
    DEBUG_CONFIG;
    if (ENABLE_DEBUG)
        use_debug = 1;

    LED_ON;

    // initialize the keyboard uart
    uart_init(1200);
    uart_putc(COMMAND_RESET);

    // initialize the USB, and then wait for the host
    // to set configuration.  If the Teensy is powered
    // without a PC connected to the USB port, this
    // will wait forever.
    if (use_debug) {
        // Use USB-Serial to debug output from keyboard
        usb_serial_init();
        while (!usb_serial_configured()) /* wait */ ;
        _delay_ms(1000);
    } else {
        // Use USB raw-hid to transfer scancodes from the keyboard
        usb_rawhid_init();
        while (!usb_rawhid_configured()) /* wait */ ;
        _delay_ms(1000);

        // Configure timer 0 to generate a timer overflow interrupt every
        // 256*1024 clock cycles, or approx 61 Hz when using 16 MHz clock
        TCCR0A = 0x00;
        TCCR0B = 0x05;
        TIMSK0 = (1<<TOIE0);
    }

    LED_OFF;
	while (1) {
        if (use_debug) {
            // wait for the user to run their terminal emulator program
            // which sets DTR to indicate it is ready to receive.
            while (!(usb_serial_get_control() & USB_SERIAL_DTR)) /* wait */ ;

            // discard anything that was received prior.  Sometimes the
            // operating system or other software will send a modem
            // "AT command", which can still be buffered.
            usb_serial_flush_input();

            // print a nice welcome message
            usb_print(PSTR("\r\nSun Type 5 Keyboard Debugger\r\n"));
        }

		// and then listen for commands and process them
		while (1) {

            if (mediaButtonsChanged) {
                // Media keys need to be sent once when they're pressed,
                // and again when they're released
                hid_report_in[0]=2; //Set report ID
                hid_report_in[1]=media_button_state;
                mediaButtonsChanged=0;
            } else if (power_button_state) {
                // Power button is sent if it's been pressed, releasing /
                // holding it is ignored: its status is reset instantly
                hid_report_in[0]=3; //Set report ID
                hid_report_in[1] = power_button_state;
                power_button_state = 0;
            } else {
                //The buffer for normal keys is sent continuously
                char i;
                for(i=0;i<8;i++) //Copy key buffer
                    hid_report_in[i+1]=key_buffer[i];
                hid_report_in[0]=1; //Set report ID
            }
        }
	}
}
#endif

// Send a string to the USB serial port.  The string must be in
// flash memory, using PSTR
//
void usb_print(const char *s)
{
    LED_ON;
	char c;
	while (1) {
		c = pgm_read_byte(s++);
		if (!c) break;
		usb_serial_putchar(c);
	}
    LED_OFF;
}

void dump_hid_report(unsigned char hid_report[HID_INT_IN_EP_SIZE]) {
    char buf[6];
    int i, j;

    usb_print(PSTR("hid_report: "));

    for (i = 0; i < sizeof(hid_report); i++) {
        snprintf(buf, sizeof(buf), " 0x02X", hid_report[i]);
        for (j = 0; j < sizeof(buf); j++) {
            usb_serial_putchar(buf[j]);
        }
    }

    usb_print(PSTR("\r\n"));
}
