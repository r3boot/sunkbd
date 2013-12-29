
#include "kbd/keyboard.h"
#include "kbd/sun_defs.h"
#include "kbd/uart.h"
#include "usb/extra_hid_defs.h"

void updateLEDs(void) {
    static unsigned char temp;

    //Swap CAPS LOCK and COMPOSE LED bits (bits 1 and 3);
    //TODO: temp = (CtrlTrfData[1]&(0b0101)) | ((CtrlTrfData[1]&0b1000)>>3) | ((CtrlTrfData[1]&0b0010)<<2);

    //Send LED states to keyboard
    uart_putc(COMMAND_LEDS);
    while(uart_tx_busy());
    uart_putc(temp);
}

void fillKeyArray(unsigned char val) {
    //Fill the key array with val
    unsigned char i;
    for(i=HID_ARRAY_START;i<HID_ARRAY_START+HID_MAX_KEYS;i++)
        key_buffer[i]=val;
}

unsigned char doModifiers(unsigned char key) {
	//Convert HID keycodes to HID modifiers (kept in the first byte of the output buffer).
	//Operates on HID codes so various keyboard keys can be remapped (swap meta/alt, etc.)
	//Returns 1 if a modifier was applied so the key isn't added to the pressed key list.
	switch(key)
	{
		case(HID_KEY_LEFT_SHIFT):
			key_buffer[0]^=HID_MODIFIER_LEFT_SHIFT;
			break;
		case(HID_KEY_RIGHT_SHIFT):
			key_buffer[0]^=HID_MODIFIER_RIGHT_SHIFT;
			break;
		case(HID_KEY_LEFT_CONTROL):
			key_buffer[0]^=HID_MODIFIER_LEFT_CONTROL;
			break;
		case(HID_KEY_RIGHT_CONTROL):
			key_buffer[0]^=HID_MODIFIER_RIGHT_CONTROL;
			break;
		case(HID_KEY_LEFT_ALT):
			key_buffer[0]^=HID_MODIFIER_LEFT_ALT;
			break;
		case(HID_KEY_LEFT_GUI):
			key_buffer[0]^=HID_MODIFIER_LEFT_GUI;
			break;
		case(HID_KEY_RIGHT_GUI):
			key_buffer[0]^=HID_MODIFIER_RIGHT_GUI;
			break;
		case(HID_KEY_RIGHT_ALT):
			key_buffer[0]^=HID_MODIFIER_RIGHT_ALT;
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
		case (KEY_VOL_UP):
			media_button_state ^= 0b00000001;
			break;
		case (KEY_VOL_DOWN):
			media_button_state ^= 0b00000010;
			break;
		case (KEY_VOL_MUTE):
			media_button_state ^= 0b00000100;
			break;
		default:
			return 0;

	}
	mediaButtonsChanged = 1;
	return 1;
}

unsigned char doPowerButton(unsigned char key) {
	//Sets power management report info
	//Only reacts to key press, not release
	//Returns 1 if key was power button, so key isn't converted to HID code
	if(key==KEY_POWER)
	{
		power_button_state = 0b00000001;
		return 1;
	}
	return 0;
}
