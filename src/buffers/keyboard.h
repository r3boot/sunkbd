
// Maximum number of slots available for scancodes
#define KEYBUFFER_MAX_SLOTS 6

// Buffer in which to store scancodes
uint8_t keyboard_buffer[KEYBUFFER_MAX_SLOTS];

/*
 * Byte in which to store modifier keys
 * 1=left ctrl,    2=left shift,   4=left alt,    8=left gui
 * 16=right ctrl, 32=right shift, 64=right alt, 128=right gui
 */
uint8_t keyboard_modifiers;

/*
 * Byte in which to store the state of the LEDS of various keys
 * 1=num lock, 2=caps lock, 4=scroll lock, 8=compose, 16=kana
 */
uint8_t keyboard_leds;

/*
 * Byte in which to store remote control keys
 */
uint8_t remote_keys;

void init_kb_buffer(void);      // Initialize the keyboard buffer
uint8_t kb_add(uint8_t sc);     // Add a scancode to the keyboard buffer
uint8_t kb_del(uint8_t sc);     // Remove a scancode from the keyboard buffer
void kb_clear(void);            // Clear the keyboard buffer
uint8_t kb_empty(void);         // Checks if the keyboard buffer is empty
uint8_t kb_full(void);          // Checks if the keyboard buffer is full
void kb_dump(void);             // Prints the contents of the keyboard buffer
