
#include <avr/io.h>
#include <stdint.h>

// AVR pin mappings
#define RESET_PIN   PB0
#define CFG_1_PIN   PB1
#define CFG_2_PIN   PB2
#define CFG_3_PIN   PB3
#define TX_LED_PIN  PF0
#define RX_LED_PIN  PF1

// Shorthands used in functions
#define TX_LED  TX_LED_PIN
#define RX_LED  RX_LED_PIN

// Exported for usage in the MCU
#define RESET_PRESSED   (PINB & (1<<RESET_PIN))

uint8_t keyclick_enabled;
uint8_t debug_enabled;
uint8_t bootloader_enabled;

void init_peripherals(void);
void parse_jumper_config(void);
void led_on(uint8_t led_pin);
void led_off(uint8_t led_pin);
