
#include <util/delay.h>

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
uint8_t debug_enabled = 0;

uint8_t kbd_initialized = 0;
uint8_t kbd_init_resp_1 = 0;
uint8_t kbd_init_resp_2 = 0;

uint8_t layout_requested = 0;
uint8_t kbd_layout_resp_1 = 0;

void toggle_bell(void) {
    if (bell_enabled) {
        uart_putc(KBD_CMD_BELL_OFF);
        bell_enabled = 0;
    } else {
        uart_putc(KBD_CMD_BELL_ON);
        bell_enabled = 1;
    }

}

void toggle_click(void) {
    if (click_enabled) {
        uart_putc(KBD_CMD_CLICK_OFF);
        click_enabled = 0;
    } else {
        uart_putc(KBD_CMD_CLICK_ON);
        click_enabled = 1;
    }
}

void toggle_led(uint8_t led)
{
    led_state ^= led;
    uart_putc(KBD_CMD_LEDS);
    uart_putc(led_state);
}

void reset_keyboard(void)
{
    uart_putc(KBD_CMD_RESET);
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

uint8_t process_scancode(uint8_t sc) {
    uint8_t i, hid_key;

    if (!kbd_initialized) {
        // When we send KBD_CMD_RESET, we expect KBD_RESP_INIT_1 and
        // KBD_RESP_INIT_2, followed by KBD_RESP_IDLE.
        if (sc == KBD_RESP_INIT_SUCCESS_1) {
            kbd_init_resp_1 = 1;
            return 1;
        } else if (sc == KBD_RESP_INIT_SUCCESS_2) {
            kbd_init_resp_2 = 1;
        } else if (sc == KBD_RESP_IDLE) {
            if ((kbd_init_resp_1) && (kbd_init_resp_2)) {
                kbd_initialized = 1;
                kbd_init_resp_1 = 0;
                kbd_init_resp_2 = 0;
                toggle_bell();
                _delay_ms(500);
                toggle_bell();
                if (click_enabled)
                    toggle_click();
            } else {
                // Something went wrong, fail-open silently
                kbd_initialized = 1;
                toggle_click();
            }
        }

    } else if (layout_requested) {
        if (!kbd_layout_resp_1) {
            if (sc == KBD_RESP_LAYOUT_SUCCESS) {
                kbd_layout_resp_1 = 1;
            }
        } else {
            print("Keyboard layout: ");
            switch (sc) {
                case KBD_LAYOUT_UNITED_STATES:
                    print("United States\n");
                    break;
                case KBD_LAYOUT_FRENCH_BELGIUM:
                    print("French Belgium\n");
                    break;
                case KBD_LAYOUT_CANADA_FRENCH:
                    print("Canada French\n");
                    break;
                case KBD_LAYOUT_DENMARK:
                    print("Denmark\n");
                    break;
                case KBD_LAYOUT_GERMANY:
                    print("Germany\n");
                    break;
                case KBD_LAYOUT_ITALY:
                    print("Italy\n");
                    break;
                case KBD_LAYOUT_JAPAN:
                    print("Japan\n");
                    break;
                case KBD_LAYOUT_NETHERLANDS:
                    print("Netherlands\n");
                    break;
                case KBD_LAYOUT_NORWAY:
                    print("Norway\n");
                    break;
                case KBD_LAYOUT_PORTUGAL:
                    print("Portugal\n");
                    break;
                case KBD_LAYOUT_SPAIN:
                    print("Spain\n");
                    break;
                case KBD_LAYOUT_SWEDEN:
                    print("Sweden\n");
                    break;
                case KBD_LAYOUT_SWISS_FRENCH:
                    print("Swiss French\n");
                    break;
                case KBD_LAYOUT_SWISS_GERMAN:
                    print("Swiss German\n");
                    break;
                case KBD_LAYOUT_UNITED_KINGDOM:
                    print("United Kingdom\n");
                    break;
                default:
                    print("Unknown (");
                    phex(sc);
                    print(")\n");
                    break;
            }
            kbd_layout_resp_1 = 0;
            layout_requested = 0;
        }
    } else {
        if (sc == KBD_RESP_IDLE) {
            // No keys have been pressed, clear buffers

            for(i=0; i<KEYBOARD_KEYS_MAX; i++) {
                if (keyboard_keys[i] == sc) {
                    keyboard_keys[i] = HID_NO_EVENT;
                }
            }

            keyboard_modifier_keys = 0;
            transmit_keyboard_buffer();
            return 1;

        } else if (sc < KBD_RESP_RELEASED) {
            // Key has been pressed
            hid_key = keycode[sc];

            if ((!is_modifier_button(hid_key)) && (!is_remote_button(hid_key))) {
                add_to_keybuffer(hid_key);
            }
            return 1;

        } else if (sc > KBD_RESP_RELEASED) {
            // Key has been released
            hid_key = keycode[RELEASETOPRESS(sc)];

            if ((!is_modifier_button(hid_key)) && (!is_remote_button(hid_key))) {
                remove_from_keybuffer(hid_key);
            }
            return 1;

        }
    }

    return 0;
}

void updateLEDs(void) {
    static unsigned char temp;

    //Swap CAPS LOCK and COMPOSE LED bits (bits 1 and 3);
    //TODO: temp = (CtrlTrfData[1]&(0b0101)) | ((CtrlTrfData[1]&0b1000)>>3) | ((CtrlTrfData[1]&0b0010)<<2);

    //Send LED states to keyboard
    uart_putc(KBD_CMD_LEDS);
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

uint8_t is_remote_button (uint8_t key) {
	//If the key pressed or released was a media key, update the media key
    //report and return 1 (so the key isn't added to the pressed key list)

    switch(key) {
        case HID_KEY_POWER:
            remote_keys ^= HID_REMOTE_POWER;
            break;
        case HID_KEY_VOL_MUTE:
            remote_keys ^= HID_REMOTE_MUTE;
            break;
        case HID_KEY_VOL_DOWN:
            remote_keys ^= HID_REMOTE_VOLUME_DOWN;
            break;
        case HID_KEY_VOL_UP:
            remote_keys ^= HID_REMOTE_VOLUME_UP;
            break;
        default:
            return 0;
    }

    dump_keys();

    return 1;
}

uint8_t doLedButtons(unsigned char key) {
    return 0;
}
