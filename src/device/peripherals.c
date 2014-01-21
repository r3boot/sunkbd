#include <avr/io.h>

#include "device/peripherals.h"

// States for the jumper based configuration
#define CFG_1_OPEN      (PINB & (1<<CFG_1_PIN))
#define CFG_2_OPEN      (PINB & (1<<CFG_2_PIN))
#define CFG_3_OPEN      (PINB & (1<<CFG_3_PIN))

// Led on/off macros
#define LED_ON(led_pin) (PORTF &= ~(1<<led_pin))
#define LED_OFF(led_pin) (PORTF |= (1<<led_pin))

uint8_t keyclick_enabled = 0;
uint8_t debug_enabled = 0;
uint8_t bootloader_enabled = 0;

// Initialize all peripheral pins
void init_peripherals(void)
{
    // Set default soft values for jumper based configuration
    keyclick_enabled = 0;
    debug_enabled = 0;
    bootloader_enabled = 0;

    // Configure RESET_PIN as an input with an internal pull-up resistor
    DDRD  |= (0<<RESET_PIN);
    PORTD |= (1<<RESET_PIN);

    // Configure CFG_*_PIN as inputs with interal pull-up resistors
    DDRB |= (0<<CFG_1_PIN);
    PORTB |= (1<<CFG_1_PIN);

    DDRB |= (0<<CFG_2_PIN);
    PORTB |= (1<<CFG_2_PIN);

    DDRB |= (0<<CFG_3_PIN);
    PORTB |= (1<<CFG_3_PIN);

    // Configure *_LED_PIN as outputs
    DDRF |= (1<<TX_LED_PIN);
    DDRF |= (1<<RX_LED_PIN);

    // Configure an interrupt on RESET_PIN
    PCICR |= (1 << PCIE0);
    PCMSK0 |= (1 << PCINT0);
}

// Parse the configuration pins and configure variables for use as configuration
// Pin configuration is as follows:
// 1 o-o    CFG_1_PIN   Enable keyclicks
// 2 o-o    CFG_2_PIN   Enable HID debugging channel
// 3 o-o    CFG_3_PIN   Enable jump to bootloader during reset
void parse_jumper_config(void)
{
    if (!CFG_1_OPEN)
        keyclick_enabled = 1;
    if (!CFG_2_OPEN)
        debug_enabled = 1;
    if (!CFG_1_OPEN)
        bootloader_enabled = 1;
}

void led_on(uint8_t led_pin)
{
    LED_ON(led_pin);
}

void led_off(uint8_t led_pin)
{
    LED_OFF(led_pin);
}
