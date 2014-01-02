// Definitions of SUN Keyboard constants

#define RELEASETOPRESS(x) x-(SUN_KEY_RELEASED+1) //Convert a key release code to a press code

//Keyboard commands/responses
#define SUN_COMMAND_RESET 0x01
#define SUN_COMMAND_BELL_ON 0x02
#define SUN_COMMAND_BELL_OFF 0x03
#define SUN_COMMAND_CLICK_ON 0x0A
#define SUN_COMMAND_CLICK_OFF 0x0B
#define SUN_COMMAND_LEDS 0x0E
#define SUN_RESPONSE_INIT_SUCCESS_1 0xFF
#define SUN_RESPONSE_INIT_SUCCESS_2 0x04
#define SUN_RESPONSE_INIT_SUCCESS_3 0x7F
#define SUN_RESPONSE_INIT_FAILURE_1 0x7E
#define SUN_RESPONSE_INIT_FAILURE_2 0x01
#define SUN_LED_NUM_LOCK 0b00000001
#define SUN_LED_COMPOSE 0b00000010
#define SUN_LED_SCROLL_LOCK 0b00000100
#define SUN_LED_CAPS_LOCK 0b00001000
#define SUN_KEY_RELEASED 0x7F

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
