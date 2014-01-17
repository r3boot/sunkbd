#include <stdint.h>

uint8_t kbd_initialized;

void reset_keyboard(void);
uint8_t process_scancode(uint8_t key);
uint8_t is_modifier_button(uint8_t key);
uint8_t is_remote_button(uint8_t key);
