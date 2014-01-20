#include <stdint.h>

uint8_t kbd_initialized;

void toggle_bell(void);
void toggle_click(void);
void toggle_led(uint8_t led);
void reset_keyboard(void);
uint8_t process_scancode(uint8_t key);
uint8_t is_modifier_button(uint8_t key);
uint8_t is_remote_button(uint8_t key);
