
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <util/delay.h>

#include "device/peripherals.h"
#include "device/mcu.h"
#include "device/usb.h"
#include "device/uart.h"

#include "keyboard/type5.h"

#include "buffers/queues.h"
#include "buffers/keyboard.h"

#include "debug/print.h"

#include "include/keymap.h"

// Time to wait for host to load drivers and configure the USB keyboard
#define HOST_DRIVER_LOAD_TIME   1000

// Main program loop. First, intialize and configure the MCU, keyboard
// and USB, then start an infinite loop in which scancodes from the
// keyboard are converted to USB-HID scancodes and transmitted to the
// host computer.
int main (void) {
	uint8_t sc, received_keys;

    // Set internal clock to 16MHz
    cpu_prescale(0);

    // Initialize peripherals and parse jumper based configuration
    init_peripherals();
    parse_jumper_config();

    // Initialize the RX/TX FIFO queues
    init_queue(RX_QUEUE);
    init_queue(TX_QUEUE);

    // Initialize the keyboard buffer
    init_kb_buffer();

    // Initialize the onboard USB chip and wait until the host device has
    // connected. Wait a second to let the host device load the required
    // drivers and configure the USB keyboard.
    usb_init();
    while (!usb_configured())
    _delay_ms(HOST_DRIVER_LOAD_TIME);

    // Initialise the onboard UART at 1200 baud. The UART is connected to
    // the keyboard using a set of hex inverters to comply with the inverted
    // TTL levels of the keyboard
    uart_init(KBD_BAUDRATE);

    // Reset keyboard
    reset_keyboard();

    // Enter main processing loop
	while (1) {
        received_keys = 0;
        while (!kb_full()) {
            if (q_empty(RX_QUEUE))
                break;

            sc = q_get(RX_QUEUE);
            if (process_scancode(sc)) {
                received_keys = 1;
                break;
            }

            print("failed to process scancode: ");
            phex(sc);
            print("\n");
        }

        if ((kbd_initialized) && (received_keys)) {
            kb_dump();
            led_on(TX_LED);
            usb_keyboard_send();
            led_off(TX_LED);
        }

	}
}
