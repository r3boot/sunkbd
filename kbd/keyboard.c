
#include "kbd/keyboard.h"
#include "kbd/sun_defs.h"
#include "kbd/keymap.h"
#include "kbd/uart.h"
#include "usb/usb_keyboard.h"
#include "usb/print.h"
#include "usb/extra_hid_defs.h"

uint8_t led_state = 0;
uint8_t bell_enabled = 0;
uint8_t click_enabled = 0;
uint8_t media_button_state = 0;

void toggle_bell(void) {
    if (bell_enabled) {
        uart_putc(SUN_COMMAND_BELL_OFF);
        bell_enabled = 0;
    } else {
        uart_putc(SUN_COMMAND_BELL_ON);
        bell_enabled = 1;
    }

}

void toggle_click(void) {
    if (click_enabled) {
        uart_putc(SUN_COMMAND_CLICK_OFF);
        click_enabled = 0;
    } else {
        uart_putc(SUN_COMMAND_CLICK_ON);
        click_enabled = 1;
    }
}

uint8_t add_to_keybuffer(uint8_t key) {
    int i;

    for(i=0; i<KEYBOARD_KEYS_MAX; i++) {
        if (keyboard_keys[i] == HID_NO_EVENT) {
            keyboard_keys[i] = key;
            return 1;
        }
    }
    print("failed to add to keybuffer: ");
    phex(key);
    print("\n");
    return 0;
}

uint8_t remove_from_keybuffer(uint8_t key) {
    int i;

    for(i=0; i<KEYBOARD_KEYS_MAX; i++) {
        if (keyboard_keys[i] == key) {
            keyboard_keys[i] = HID_NO_EVENT;
            return 1;
        }
    }
    print("failed to remove from keybuffer: ");
    phex(key);
    print("\n");
    return 0;
}

uint8_t update_keyboard_keys (uint8_t key) {
    uint8_t i, hid_key;

    if (key == SUN_RESPONSE_IDLE) {
        // No keys have been pressed, clear buffers

        for(i=0; i<KEYBOARD_KEYS_MAX; i++) {
            if (keyboard_keys[i] == key) {
                keyboard_keys[i] = HID_NO_EVENT;
            }
        }

        keyboard_modifier_keys = 0;
        transmit_keyboard_buffer();
        return 1;

    } else if (key < SUN_KEY_RELEASED) {
        // Key has been pressed
        hid_key = keycode[key];

        if ((!is_modifier_button(hid_key)) && (!is_media_button(key))) {
            add_to_keybuffer(hid_key);
        }
        return 1;

    } else if (key > SUN_KEY_RELEASED) {
        // Key has been released
        hid_key = keycode[RELEASETOPRESS(key)];

        if ((!is_modifier_button(hid_key)) && (!is_media_button(key))) {
            remove_from_keybuffer(hid_key);
        }
        return 1;

    }

    return 0;
}

void updateLEDs(void) {
    static unsigned char temp;

    //Swap CAPS LOCK and COMPOSE LED bits (bits 1 and 3);
    //TODO: temp = (CtrlTrfData[1]&(0b0101)) | ((CtrlTrfData[1]&0b1000)>>3) | ((CtrlTrfData[1]&0b0010)<<2);

    //Send LED states to keyboard
    uart_putc(SUN_COMMAND_LEDS);
    while(uart_tx_busy());

    uart_putc(temp);
    while(uart_tx_busy());
}

uint8_t is_modifier_button (uint8_t key) {
	//Convert HID keycodes to HID modifiers (kept in the first byte of the output buffer).
	//Operates on HID codes so various keyboard keys can be remapped (swap meta/alt, etc.)
	//Returns 1 if a modifier was applied so the key isn't added to the pressed key list.
	switch(key)
	{
		case(HID_KEY_LEFT_SHIFT):
			keyboard_modifier_keys ^= HID_MODIFIER_LEFT_SHIFT;
			break;
		case(HID_KEY_RIGHT_SHIFT):
			keyboard_modifier_keys ^= HID_MODIFIER_RIGHT_SHIFT;
			break;
		case(HID_KEY_LEFT_CONTROL):
			keyboard_modifier_keys ^= HID_MODIFIER_LEFT_CONTROL;
			break;
		case(HID_KEY_RIGHT_CONTROL):
			keyboard_modifier_keys ^= HID_MODIFIER_RIGHT_CONTROL;
			break;
		case(HID_KEY_LEFT_ALT):
			keyboard_modifier_keys ^= HID_MODIFIER_LEFT_ALT;
			break;
		case(HID_KEY_LEFT_GUI):
			keyboard_modifier_keys ^= HID_MODIFIER_LEFT_GUI;
			break;
		case(HID_KEY_RIGHT_GUI):
			keyboard_modifier_keys ^= HID_MODIFIER_RIGHT_GUI;
			break;
		case(HID_KEY_RIGHT_ALT):
			keyboard_modifier_keys ^= HID_MODIFIER_RIGHT_ALT;
			break;
		default:
			return 0;	//No modifier key pressed
	}
	return 1;
}

uint8_t is_media_button (uint8_t key) {
	//If the key pressed or released was a media key, update the media key
    //report and return 1 (so the key isn't added to the pressed key list)
    uint8_t released = 0;

    if (RELEASETOPRESS(key) == SUN_KEY_VOL_UP) {
        key = SUN_KEY_VOL_UP;
        released = 1;
    } else if(RELEASETOPRESS(key) == SUN_KEY_VOL_DOWN) {
        key = SUN_KEY_VOL_DOWN;
        released = 1;
    } else if (RELEASETOPRESS(key) == SUN_KEY_VOL_MUTE) {
        key = SUN_KEY_VOL_MUTE;
        released = 1;
    }

    switch(key) {
		case (SUN_KEY_VOL_MUTE):
            print("volume mute\n");
            if (!released) {
                add_to_keybuffer(0xe0);
                add_to_keybuffer(0x20);
                add_to_keybuffer(0xe0);
                add_to_keybuffer(0xa0);
            } else {
                remove_from_keybuffer(0xe0);
                remove_from_keybuffer(0x20);
                remove_from_keybuffer(0xe0);
                remove_from_keybuffer(0xa0);
            }
            break;
		case (SUN_KEY_VOL_DOWN):
            print("volume down\n");
            if (!released) {
                add_to_keybuffer(0xe0);
                add_to_keybuffer(0x2e);
                add_to_keybuffer(0xe0);
                add_to_keybuffer(0xae);
            } else {
                remove_from_keybuffer(0xe0);
                remove_from_keybuffer(0x2e);
                remove_from_keybuffer(0xe0);
                remove_from_keybuffer(0xae);
            }
			break;
		case (SUN_KEY_VOL_UP):
            print("volume up\n");
            if (!released) {
                add_to_keybuffer(0xe0);
                add_to_keybuffer(0x30);
                add_to_keybuffer(0xe0);
                add_to_keybuffer(0xb0);
            } else {
                remove_from_keybuffer(0xe0);
                remove_from_keybuffer(0x30);
                remove_from_keybuffer(0xe0);
                remove_from_keybuffer(0xb0);
            }
			break;
		default:
			return 0;

	}

    transmit_keyboard_buffer();

	return 1;
}

unsigned char doPowerButton(unsigned char key) {
	//Sets power management report info
	//Only reacts to key press, not release
	//Returns 1 if key was power button, so key isn't converted to HID code
	if (key == SUN_KEY_POWER)
	{
		// power_button_state = 0b00000001;
		return 1;
	}
	return 0;
}

uint8_t doLedButtons(unsigned char key) {
    return 0;
}
