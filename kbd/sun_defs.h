// Definitions of SUN Keyboard constants

#define RELEASETOPRESS(x) x-(KBD_RESP_RELEASED+1) //Convert a key release code to a press code

// Keyboard commands
#define KBD_CMD_RESET           0x01
#define KBD_CMD_BELL_ON         0x02
#define KBD_CMD_BELL_OFF        0x03
#define KBD_CMD_CLICK_ON        0x0a
#define KBD_CMD_CLICK_OFF       0x0b
#define KBD_CMD_LEDS            0x0e
#define KBD_CMD_LAYOUT          0x0f

// Keyboard responses
#define KBD_RESP_INIT_SUCCESS_1 0xff
#define KBD_RESP_INIT_SUCCESS_2 0x04
#define KBD_RESP_INIT_SUCCESS_3 0x7f
#define KBD_RESP_INIT_FAILURE_1 0x7e
#define KBD_RESP_INIT_FAILURE_2 0x01
#define KBD_RESP_LAYOUT_SUCCESS 0xfe
#define KBD_RESP_RELEASED       0x7f
#define KBD_RESP_IDLE           0x7f

// Binary positions of LEDs
#define KBD_LED_NUM_LOCK        0b00000001
#define KBD_LED_COMPOSE         0b00000010
#define KBD_LED_SCROLL_LOCK     0b00000100
#define KBD_LED_CAPS_LOCK       0b00001000

// Keyboard layout responses
#define KBD_LAYOUT_UNITED_STATES    0x00
#define KBD_LAYOUT_FRENCH_BELGIUM   0x02
#define KBD_LAYOUT_CANADA_FRENCH    0x03
#define KBD_LAYOUT_DENMARK          0x04
#define KBD_LAYOUT_GERMANY          0x05
#define KBD_LAYOUT_ITALY            0x06
#define KBD_LAYOUT_JAPAN            0x20
#define KBD_LAYOUT_NETHERLANDS      0x07
#define KBD_LAYOUT_NORWAY           0x08
#define KBD_LAYOUT_PORTUGAL         0x09
#define KBD_LAYOUT_SPAIN            0x0a
#define KBD_LAYOUT_SWEDEN           0x0b
#define KBD_LAYOUT_SWISS_FRENCH     0x0c
#define KBD_LAYOUT_SWISS_GERMAN     0x0d
#define KBD_LAYOUT_UNITED_KINGDOM   0x0e

//Modifier keys
#define SUN_KEY_CAPS_LOCK	0x77
#define	SUN_KEY_LEFT_META	0x78
#define	SUN_KEY_RIGHT_META	0x7A
#define	SUN_KEY_COMPOSE		0x43
#define	SUN_KEY_ALT_GRAPH	0x0D
#define	SUN_KEY_LEFT_SHIFT	0x63
#define	SUN_KEY_ALT			0x13
#define	SUN_KEY_CONTROL		0x4C
#define SUN_KEY_RIGHT_SHIFT	0x6E


//Special keys that can be swapped to other functions
//TODO #define SUN_KEY_SCROLL_LOCK 0x17 //Used to enter keyboard settings
//TODO #define SUN_KEY_PAUSE 0x15 //Used to reset keyboard settings

#define SUN_KEY_ESC			0x1D
#define SUN_KEY_TILDE 		0x2A

#define SUN_KEY_VOL_UP 0x04 
#define SUN_KEY_VOL_DOWN 0x02
#define SUN_KEY_VOL_MUTE 0x2D
#define SUN_KEY_POWER 0x30
#define SUN_KEY_COMPOSE  0x43

#define SUN_KEY_HELP 0x76
#define SUN_KEY_STOP 0x01
#define SUN_KEY_AGAIN 0x03
#define SUN_KEY_PROPS 0x19
#define SUN_KEY_UNDO 0x1A
#define SUN_KEY_FRONT 0x31
#define SUN_KEY_COPY 0x33
#define SUN_KEY_OPEN 0x48
#define SUN_KEY_PASTE 0x49
#define SUN_KEY_FIND 0x5F
#define SUN_KEY_CUT 0x61
