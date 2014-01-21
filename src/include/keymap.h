//Scancode conversion table. Format: index=SUN scancode, table[index]=HID scancode
//The keyboard sends one of these codes when a key is pressed; when the key is released it sends the code + 0x7F.
//The microcontroller then adds or removes the HID code from the pressed key list.

#include "keyboard/type5.h"

#include "include/hid_scancodes.h"

static const uint8_t keymap[]={
0,		// undefined
HID_KEY_F14,	// Stop (mapped to F14)
HID_KEY_VOL_DOWN,	// Volume down ; was 0x81
HID_KEY_F15,	// Again (mapped to F15)
HID_KEY_VOL_UP,	// Volume up ; was 0x80
0x3A,	// F1
0x3B,	// F2
0x43,	// F10
0x3C,	// F3
0x44,	// F11
0x3D,	// F4
0x45,	// F12
0x3E,	// F5
HID_KEY_RIGHT_CONTROL,	// Alt Graph
0x3F,	// F6
0,		// undefined
0x40,	// F7
0x41,	// F8
0x42,	// F9
HID_KEY_LEFT_ALT,	// Alt
0x52,	// Arrow up
0x48,	// Pause
0x46,	// Print Screen/Sysrq
0x47,	// Scroll Lock
0x50,	// Arrow left
HID_KEY_F16,	// Props (HID specs: keyboard menu) (mapped to F16)
HID_KEY_F17,	// Undo (mapped to F17)
0x51,	// Arrow down
0x4F,	// Arrow right
0x29,	// Esc
0x1E,	// 1 !
0x1F,	// 2 @
0x20,	// 3 #
0x21,	// 4 $
0x22,	// 5 %
0x23,	// 6 ^
0x24,	// 7 &
0x25,	// 8 *
0x26,	// 9 (
0x27,	// 0 )
0x2D,	// - _
0x2E,	// = +
0x35,	// ‘ ~
0x2A,	// Backspace
0x49,	// Insert
HID_KEY_VOL_MUTE,	// Mute ; was 0x7F / 0x79
0x54,	// /
0x55,	// *
HID_KEY_POWER,	// Power ; was 0x66
HID_KEY_F18,	// Front (HID specs: select) (mapped as F18)
0x63,	// Del
HID_KEY_F19,	// Copy (mapped as F19)
0x4A,	// Home
0x2B,	// Tab
0x14,	// Q
0x1A,	// W
0x08,	// E
0x15,	// R
0x17,	// T
0x1C,	// Y
0x18,	// U
0x0C,	// I
0x12,	// O
0x13,	// P
0x2F,	// [ {
0x30,	// ] }
0x4C,	// Delete
0x65,	// Compose (HID specs: app)
0x5F,	// Numpad 7
0x60,	// Numpad 8
0x61,	// Numpad PgUp
0x56,	// Numpad -
HID_KEY_F20,	// Open (HID specs: execute) (mapped as F20)
HID_KEY_F21,	// Paste (mapped as F21)
0x4D,	// End
0,		// undefined
0xE0,	// Control
0x04,	// A
0x16,	// S
0x07,	// D
0x09,	// F
0x0A,	// G
0x0B,	// H
0x0D,	// J
0x0E,	// K
0x0F,	// L
0x33,	// ; :
0x34,	// ‘ “
0x31,	// \ |
0x28,	// Return
0x58,	// Numpad Enter
0x5C,	// Numpad 4
0x5D,	// Numpad 5
0x5E,	// Numpad 6
0x62,	// Numpad 0
HID_KEY_F22,	// Find (mapped as F22)
0x4B,	// Page Up
HID_KEY_F23,	// Cut (mapped as F23)
0x53,	// Num Lock
0xE1,	// Shift
0x1D,	// Z
0x1B,	// X
0x06,	// C
0x19,	// V
0x05,	// B
0x11,	// N
0x10,	// M
0x36,	// , <
0x37,	// . >
0x38,	// / ?
0xE5,	// Shift
0,		// undefined
0x59,	// Numpad 1
0x5A,	// Numpad 2
0x5B,	// Numpad 3
0,		// undefined
0,		// undefined
0,		// undefined
HID_KEY_F13,	// Help (mapped to F13) ; was 0x68
0x39,	// Caps Lock
HID_KEY_LEFT_GUI,	// Left Meta (HID specs: left GUI)
0x2C,	// Space
HID_KEY_RIGHT_ALT,	// Right Meta (HID specs: right GUI)
0x4E,	// Page down
0,		// undefined
0x57,	// +
0		// undefined
};
