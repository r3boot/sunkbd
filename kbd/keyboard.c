
#include "kbd/keyboard.h"
#include "kbd/sun_defs.h"
#include "kbd/uart.h"
#include "usb/usb_keyboard.h"
#include "usb/extra_hid_defs.h"

uint8_t led_state = 0;
uint8_t bell_enabled = 0;
uint8_t click_enabled = 0;

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

unsigned char doModifiers(unsigned char key) {
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

unsigned char doMediaButtons(unsigned char key) {
	//If the key pressed or released was a media key, update the media key report and return 1 (so the key isn't added to the pressed key list)
	switch(key)
	{
		case (SUN_KEY_VOL_UP):
			// media_button_state ^= 0b00000001;
			break;
		case (SUN_KEY_VOL_DOWN):
			// media_button_state ^= 0b00000010;
			break;
		case (SUN_KEY_VOL_MUTE):
			// media_button_state ^= 0b00000100;
			break;
		default:
			return 0;

	}
	// mediaButtonsChanged = 1;
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
