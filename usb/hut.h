/*
 * USB HID Usage Table definitions
 *
 * HUT_UP_  HID Usage Page
 */

// Items
#define USAGE_PAGE      0x05
#define USAGE_PAGE_U16  0x06
#define USAGE           0x09
#define COLLECTION      0xa1
#define END_COLLECTION  0xc0
#define REPORT_ID       0xa1
#define REPORT_SIZE     0x75
#define REPORT_COUNT    0x95
#define USAGE_MIN       0x19
#define USAGE_MAX       0x29
#define LOGICAL_MIN     0x15
#define LOGICAL_MAX     0x25
#define LOGICAL_MAX_U16 0x25
#define INPUT           0x81
#define OUTPUT          0x91

// Usage pages
#define UP_GENERIC_DESKTOP 0x01
#define UP_KEY_CODES       0x07
#define UP_LEDS            0x08
#define UP_CONSUMER_DEV    0x0c

// Usages
#define U_REMOTE        0x01
#define U_KEYBOARD      0x06
#define U_POWER         0x30
#define U_PLAY          0xb0
#define U_PAUSE         0xb1
#define U_RECORD        0xb2
#define U_FORWARD       0xb3
#define U_REWIND        0xb4
#define U_NEXT          0xb5
#define U_PREV          0xb6
#define U_STOP          0xb7
#define U_MUTE          0xe2
#define U_VOLUME_UP     0xe9
#define U_VOLUME_DOWN   0xea

// Collections
#define C_APPLICATION     0x01

// Inputs
#define I_DATA_ARRAY      0x00
#define I_DATA_VAR_ABS    0x02
#define I_DATA_VAR_REL    0x06
#define I_CONSTANT        0x03
#define I_CONST_VAR_REL   0x07

// Outputs
#define O_CONSTANT        0x01
#define O_DATA_VAR_ABS    0x02

// Descriptor Types
#define DT_DEVICE   0x01

// Device Classes
#define DC_DEVICE       0x00
#define DC_SUB_DEVICE   0x00

// Device Protocol
#define DP_DEVICE   0x00
