
#include <util/delay.h>

#include "keyboard/type5.h"

#include "device/uart.h"
#include "buffers/queues.h"
#include "buffers/keyboard.h"

#include "debug/print.h"

#include "include/sun_scancodes.h"
#include "include/hid_scancodes.h"
#include "include/keymap.h"

uint8_t kbd_initialized = 0;
uint8_t kbd_bell_enabled = 0;
uint8_t kbd_keyclick_enabled = 0;
uint8_t kbd_current_layout = 0;

uint8_t kbd_init_resp_1 = 0;
uint8_t kbd_init_resp_2 = 0;
uint8_t kbd_layout_resp_1 = 0;

uint8_t kbd_layout_requested = 0;
uint8_t kbd_led_state = 0;

void process_bell(void)
{
    if (kbd_bell_enabled)
        q_put(TX_QUEUE, KBD_CMD_BELL_ON);
    else
        q_put(TX_QUEUE, KBD_CMD_BELL_OFF);
}

void set_bell(uint8_t state)
{
    kbd_bell_enabled = state;
    process_bell();
}

void toggle_bell(void) {
    kbd_bell_enabled = !kbd_bell_enabled;
    process_bell();
}

void process_keyclick(void)
{
    if (kbd_keyclick_enabled)
        q_put(TX_QUEUE, KBD_CMD_KEYCLICK_ON);
    else
        q_put(TX_QUEUE, KBD_CMD_KEYCLICK_OFF);
}

void set_keyclick(uint8_t state)
{
    kbd_keyclick_enabled = state;
    process_keyclick();
}

void toggle_keyclick(void) {
    kbd_keyclick_enabled = !kbd_keyclick_enabled;
    process_keyclick();
}

void process_leds(void)
{
    q_put(TX_QUEUE, KBD_CMD_LEDS);
    q_put(TX_QUEUE, kbd_led_state);
}

void set_led(uint8_t led, uint8_t state)
{
    kbd_led_state |= (state<<led);
    process_leds();
}

void toggle_led(uint8_t led)
{
    kbd_led_state ^= led;
    process_leds();
}

void reset_keyboard(void)
{
    uart_putc(KBD_CMD_RESET);
}

void request_layout(void)
{
    kbd_layout_requested = 1;
    uart_putc(KBD_CMD_LAYOUT);
}

uint8_t process_init_request(uint8_t sc)
{
    uint8_t result = 0;

    // When we send KBD_CMD_RESET, we expect KBD_RESP_INIT_1 and
    // KBD_RESP_INIT_2, followed by KBD_RESP_IDLE.
    if (sc == KBD_RESP_INIT_SUCCESS_1) {
        kbd_init_resp_1 = 1;
        result = 1;
    } else if (sc == KBD_RESP_INIT_SUCCESS_2) {
        kbd_init_resp_2 = 1;
        result = 1;
    } else if (sc == KBD_RESP_INIT_SUCCESS_3) {
        if ((kbd_init_resp_1) && (kbd_init_resp_2)) {
            print("Keyboard initialized\n");
            kbd_initialized = 1;
            kbd_init_resp_1 = 0;
            kbd_init_resp_2 = 0;
            toggle_bell();
            _delay_ms(500);
            toggle_bell();
            if (kbd_keyclick_enabled)
                toggle_keyclick();
            result = 1;
        } else {
            // Something went wrong, fail-open silently
            kbd_initialized = 1;
            toggle_keyclick();
            result = 0;
        }
    }

    return result;
}

uint8_t process_layout_request(uint8_t sc)
{
    if (!kbd_layout_resp_1) {
        if (sc == KBD_RESP_LAYOUT_SUCCESS) {
            kbd_layout_resp_1 = 1;
        }
    } else {
        kbd_current_layout = sc;
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
        kbd_layout_requested = 0;
    }

    return kbd_current_layout;
}

void process_idle_request(void)
{
    kb_clear();
    keyboard_modifiers = 0;
    remote_keys = 0;
    keyboard_leds = 0;
}

uint8_t process_keypress(uint8_t sc)
{
    uint8_t hid_sc = 0;

    if (sc < KBD_RESP_RELEASED) {
        // Key has been pressed
        hid_sc = keymap[sc];
        kb_add(hid_sc);
        return 1;

    } else if (sc > KBD_RESP_RELEASED) {
        // Key has been released
        hid_sc = keymap[RELEASETOPRESS(sc)];
        kb_del(hid_sc);
        return 1;

    }

    return 0;
}

uint8_t process_modifier_scancode(uint8_t sc)
{
	//Convert HID keymaps to HID modifiers (kept in the first byte of the output buffer).
	//Operates on HID codes so various keyboard keys can be remapped (swap meta/alt, etc.)
	//Returns 1 if a modifier was applied so the key isn't added to the pressed key list.
	switch(sc)
	{
		case(HID_KEY_LEFT_SHIFT):
			keyboard_modifiers ^= HID_MODIFIER_LEFT_SHIFT;
			break;
		case(HID_KEY_RIGHT_SHIFT):
			keyboard_modifiers ^= HID_MODIFIER_RIGHT_SHIFT;
			break;
		case(HID_KEY_LEFT_CONTROL):
			keyboard_modifiers ^= HID_MODIFIER_LEFT_CONTROL;
			break;
		case(HID_KEY_RIGHT_CONTROL):
			keyboard_modifiers ^= HID_MODIFIER_RIGHT_CONTROL;
			break;
		case(HID_KEY_LEFT_ALT):
			keyboard_modifiers ^= HID_MODIFIER_LEFT_ALT;
			break;
		case(HID_KEY_LEFT_GUI):
			keyboard_modifiers ^= HID_MODIFIER_LEFT_GUI;
			break;
		case(HID_KEY_RIGHT_GUI):
			keyboard_modifiers ^= HID_MODIFIER_RIGHT_GUI;
			break;
		case(HID_KEY_RIGHT_ALT):
			keyboard_modifiers ^= HID_MODIFIER_RIGHT_ALT;
			break;
		default:
			return 0;	//No modifier key pressed
	}
	return 1;
}

uint8_t process_remote_scancode(uint8_t sc)
{
	//If the key pressed or released was a media key, update the media key
    //report and return 1 (so the key isn't added to the pressed key list)

    switch(sc) {
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

    return 1;
}

void process_led_keys(uint8_t sc)
{
    switch(sc) {
        case (0x39):
            toggle_led(KBD_LED_CAPS_LOCK);
            break;
        case (0x47):
            toggle_led(KBD_LED_SCROLL_LOCK);
            break;
        case (0x53):
            toggle_led(KBD_LED_NUM_LOCK);
            break;
        case (0x65):
            toggle_led(KBD_LED_COMPOSE);
            break;
    }
}

uint8_t process_scancode(uint8_t sc) {
    if (!kbd_initialized) {
        process_init_request(sc);
    } else if (kbd_layout_requested) {
        process_layout_request(sc);
    } else {
        if (sc == KBD_RESP_IDLE) {
            process_idle_request();
            return 1;
        } else {
            if (process_modifier_scancode(sc))
                return 1;
            else if (process_remote_scancode(sc))
                return 1;

            process_led_keys(sc);
            process_keypress(sc);
            return 1;
        }
    }

    return 0;
}
