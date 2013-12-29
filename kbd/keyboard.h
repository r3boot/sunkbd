
#include "kbd/uart.h"
#include "kbd/sun_defs.h"
#include "usb/extra_hid_defs.h"

static unsigned char key_buffer[8]={
    HID_NO_EVENT, HID_NO_EVENT, HID_NO_EVENT,
    HID_NO_EVENT, HID_NO_EVENT, HID_NO_EVENT,
    HID_NO_EVENT, HID_NO_EVENT
};

static unsigned char media_button_state=0;
static unsigned char mediaButtonsChanged=0;
static unsigned char power_button_state=0;

void updateLEDs(void);
void fillKeyArray(unsigned char val);
unsigned char doModifiers(unsigned char key);
unsigned char doMediaButtons(unsigned char key);
unsigned char doPowerButton(unsigned char key);
