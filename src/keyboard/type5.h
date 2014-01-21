/* Definitions for the Sun Microsystems Type 5 keyboard
 */

#include <stdint.h>

// Shorthand defines for state definitions
#define STATE_OFF   0;
#define STATE_ON    1;

// Macro for remapping the release code back to the press code
#define RELEASETOPRESS(x) x-(KBD_RESP_RELEASED+1) //Convert a key release code to a press code

// Keyboard commands
#define KBD_CMD_RESET           0x01
#define KBD_CMD_BELL_ON         0x02
#define KBD_CMD_BELL_OFF        0x03
#define KBD_CMD_KEYCLICK_ON     0x0a
#define KBD_CMD_KEYCLICK_OFF    0x0b
#define KBD_CMD_LEDS            0x0e
#define KBD_CMD_LAYOUT          0x0f

// Keyboard responses
#define KBD_RESP_INIT_SUCCESS_1 0xff
#define KBD_RESP_INIT_SUCCESS_2 0x04
#define KBD_RESP_INIT_SUCCESS_3 0x7f
#define KBD_RESP_INIT_FAILURE_1 0x7e
#define KBD_RESP_INIT_FAILURE_2 0x01
#define KBD_RESP_LAYOUT_SUCCESS 0xfe
#define KBD_RESP_RELEASED       0x7f
#define KBD_RESP_IDLE           0x7f

// Binary positions of LEDs
#define KBD_LED_NUM_LOCK        0b00000001
#define KBD_LED_COMPOSE         0b00000010
#define KBD_LED_SCROLL_LOCK     0b00000100
#define KBD_LED_CAPS_LOCK       0b00001000

// Keyboard layout responses
#define KBD_LAYOUT_UNITED_STATES    0x00
#define KBD_LAYOUT_FRENCH_BELGIUM   0x02
#define KBD_LAYOUT_CANADA_FRENCH    0x03
#define KBD_LAYOUT_DENMARK          0x04
#define KBD_LAYOUT_GERMANY          0x05
#define KBD_LAYOUT_ITALY            0x06
#define KBD_LAYOUT_JAPAN            0x20
#define KBD_LAYOUT_NETHERLANDS      0x07
#define KBD_LAYOUT_NORWAY           0x08
#define KBD_LAYOUT_PORTUGAL         0x09
#define KBD_LAYOUT_SPAIN            0x0a
#define KBD_LAYOUT_SWEDEN           0x0b
#define KBD_LAYOUT_SWISS_FRENCH     0x0c
#define KBD_LAYOUT_SWISS_GERMAN     0x0d
#define KBD_LAYOUT_UNITED_KINGDOM   0x0e

uint8_t kbd_initialized;
uint8_t kbd_bell_enabled;
uint8_t kbd_keyclick_enabled;
uint8_t kbd_current_layout;

void set_bell(uint8_t state);
void toggle_bell(void);

void set_keyclick(uint8_t state);
void toggle_keyclick(void);

void toggle_led(uint8_t led);
void reset_keyboard(void);
void request_layout(void);
uint8_t process_scancode(uint8_t sc);
