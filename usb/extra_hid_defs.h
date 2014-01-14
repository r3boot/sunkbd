//Some extra definitions for HID stuff used by the main program, not by the USB framework.

#define HID_ARRAY_START 2
#define HID_MAX_KEYS 6 //Maximum amount of keys in HID data array

// HID Remote keys
#define HID_REMOTE_MUTE             0x10
#define HID_REMOTE_VOLUME_UP        0x20
#define HID_REMOTE_VOLUME_DOWN      0x40

//HID Modifier keys
#define HID_MODIFIER_LEFT_CONTROL	0x01
#define HID_MODIFIER_LEFT_SHIFT     0x02
#define HID_MODIFIER_LEFT_ALT       0x04
#define HID_MODIFIER_LEFT_GUI       0x08
#define HID_MODIFIER_RIGHT_CONTROL  0x10
#define HID_MODIFIER_RIGHT_SHIFT    0x20
#define HID_MODIFIER_RIGHT_ALT      0x40
#define HID_MODIFIER_RIGHT_GUI      0x80

//HID errors
#define HID_NO_EVENT 0x00
#define HID_ERR_OVERRUN 0x01
#define HID_ERR_POST 0x02

//HID keys used for keyboard settings
#define HID_KEY_LEFT_GUI 0xE3
#define HID_KEY_RIGHT_GUI 0xE7
#define HID_KEY_LEFT_ALT 0xE2
#define HID_KEY_RIGHT_ALT 0xE6
#define HID_KEY_LEFT_SHIFT 0xE1
#define HID_KEY_RIGHT_SHIFT 0xE5
#define HID_KEY_LEFT_CONTROL 0xE0
#define HID_KEY_RIGHT_CONTROL 0xE4

#define HID_KEY_CAPS_LOCK 0x39
#define HID_KEY_NUM_LOCK 0x53
#define HID_KEY_SCROLL_LOCK 0x47
#define HID_KEY_COMPOSE 0x65
#define HID_KEY_TILDE 0x35
#define HID_KEY_ESC 0x29

#define HID_KEY_APP 0x65
#define HID_KEY_VOL_UP          0x80
#define HID_KEY_VOL_DOWN        0x81
#define HID_KEY_VOL_MUTE        0x7F
#define HID_KEY_POWER           0x66

#define HID_KEY_HELP            0x7D
#define HID_KEY_PROP            0x20
#define HID_KEY_STOP            0x08
#define HID_KEY_AGAIN           0x0A
#define HID_KEY_UNDO            0x21
#define HID_KEY_SELECT          0x77    // TODO
#define HID_KEY_COPY            0x3A
#define HID_KEY_EXECUTE         0x74    // TODO
#define HID_KEY_PASTE           0x50
#define HID_KEY_FIND            0x66
#define HID_KEY_CUT             0x68
#define HID_KEY_FRONT           0x38
#define HID_KEY_OPEN            0x4F

#define HID_KEY_F13 0x68
#define HID_KEY_F14 0x69
#define HID_KEY_F15 0x6A
#define HID_KEY_F16 0x6B
#define HID_KEY_F17 0x6C
#define HID_KEY_F18 0x6D
#define HID_KEY_F19 0x6E
#define HID_KEY_F20 0x6F
#define HID_KEY_F21 0x70
#define HID_KEY_F22 0x71
#define HID_KEY_F23 0x72
#define HID_KEY_F24 0x73
