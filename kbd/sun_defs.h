// Definitions of SUN Keyboard constants

#define RELEASETOPRESS(x) x-(KEY_RELEASED+1) //Convert a key release code to a press code

//Keyboard commands/responses
#define COMMAND_RESET 0x01
#define COMMAND_BELL_ON 0x02
#define COMMAND_BELL_OFF 0x03
#define COMMAND_CLICK_ON 0x0A
#define COMMAND_CLICK_OFF 0x0B
#define COMMAND_LEDS 0x0E
#define RESPONSE_INIT_SUCCESS_1 0xFF
#define RESPONSE_INIT_SUCCESS_2 0x04
#define RESPONSE_INIT_SUCCESS_3 0x7F
#define RESPONSE_INIT_FAILURE_1 0x7E
#define RESPONSE_INIT_FAILURE_2 0x01
#define LED_NUM_LOCK 0b00000001
#define LED_COMPOSE 0b00000010
#define LED_SCROLL_LOCK 0b00000100
#define LED_CAPS_LOCK 0b00001000
#define KEY_RELEASED 0x7F

//Modifier keys
#define KEY_CAPS_LOCK	0x77
#define	KEY_LEFT_META	0x78
#define	KEY_RIGHT_META	0x7A
#define	KEY_COMPOSE		0x43
#define	KEY_ALT_GRAPH	0x0D
#define	KEY_LEFT_SHIFT	0x63
#define	KEY_ALT			0x13
#define	KEY_CONTROL		0x4C
#define KEY_RIGHT_SHIFT	0x6E


//Special keys that can be swapped to other functions
#define KEY_SCROLL_LOCK 0x17 //Used to enter keyboard settings
#define KEY_PAUSE 0x15 //Used to reset keyboard settings

#define KEY_ESC			0x1D
#define KEY_TILDE 		0x2A

#define KEY_VOL_UP 0x04 
#define KEY_VOL_DOWN 0x02
#define KEY_VOL_MUTE 0x2D
#define KEY_POWER 0x30
#define KEY_COMPOSE  0x43

#define KEY_HELP 0x76
#define KEY_STOP 0x01
#define KEY_AGAIN 0x03
#define KEY_PROPS 0x19
#define KEY_UNDO 0x1A
#define KEY_FRONT 0x31
#define KEY_COPY 0x33
#define KEY_OPEN 0x48
#define KEY_PASTE 0x49
#define KEY_FIND 0x5F
#define KEY_CUT 0x61