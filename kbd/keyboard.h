
#include "kbd/uart.h"
#include "kbd/sun_defs.h"
#include "usb/extra_hid_defs.h"

#define POS_NUM_LOCK    0x01
#define POS_CAPS_LOCK   0x02
#define POS_SCROLL_LOCK 0x04
#define POS_COMPOSE     0x08

void updateLEDs(void);
void fillKeyArray(unsigned char val);
unsigned char doModifiers(unsigned char key);
unsigned char doMediaButtons(unsigned char key);
unsigned char doPowerButton(unsigned char key);
