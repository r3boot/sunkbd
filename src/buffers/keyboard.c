
#include <stdint.h>

#include "buffers/keyboard.h"
#include "debug/print.h"

#include "include/hid_scancodes.h"

void init_kb_buffer(void)
{
    kb_clear();
}

uint8_t kb_add(uint8_t scancode)
{
    uint8_t i = 0;

    for (i=0; i<KEYBUFFER_MAX_SLOTS; i++) {
        if (keyboard_buffer[i] == HID_NO_EVENT) {
            keyboard_buffer[i] = scancode;
            return 1;
        }
    }

    return 0;
}

uint8_t kb_del(uint8_t scancode)
{
    uint8_t i = 0;

    for (i=0; i<KEYBUFFER_MAX_SLOTS; i++) {
        if (keyboard_buffer[i] == scancode) {
            keyboard_buffer[i] = HID_NO_EVENT;
            return 1;
        }
    }

    return 0;
}

void kb_clear(void)
{
    uint8_t i = 0;

    for (i=0; i<KEYBUFFER_MAX_SLOTS; i++) {
        keyboard_buffer[i] = HID_NO_EVENT;
    }

    keyboard_modifiers = 0;
    remote_keys = 0;
    keyboard_leds = 0;
}

uint8_t kb_empty(void)
{
    uint8_t i;

    for (i=0; i<KEYBUFFER_MAX_SLOTS; i++) {
        if (keyboard_buffer[i] != HID_NO_EVENT)
            return 0;
    }
    return 1;
}

uint8_t kb_full(void)
{
    uint8_t i;

    for (i=0; i<KEYBUFFER_MAX_SLOTS; i++) {
        if (keyboard_buffer[i] == HID_NO_EVENT)
            return 0;
    }
    return 1;
}

void kb_dump(void)
{
    uint8_t i;

    print("kb_buffer: ");
    phex(keyboard_modifiers);
    print(" 00");
    for (i=0; i<KEYBUFFER_MAX_SLOTS; i++) {
        print(" ");
        phex(keyboard_buffer[i]);
    }
    print("  ");
    phex(keyboard_leds);
    print("  ");
    phex(remote_keys);
}
