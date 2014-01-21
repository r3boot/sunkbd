#include <stdint.h>

/*
 * Settings to turn the controller into a Sun type 6 USB keyboard
 */
#define USB_MANUFACTURER    L"Sun Microsystems, Inc."
#define USB_PRODUCT         L"Type 6 Keyboard"
#define USB_VENDOR_ID       0x0430
#define USB_PRODUCT_ID      0x0005

void usb_init(void);                    // Initialize USB
uint8_t usb_configured(void);           // Returns 1 if USB is configured
uint8_t usb_keyboard_send(void);        // Send the keyboard buffer
uint8_t usb_debug_putc(uint8_t c);   // Transmit a character over the debug channel
void usb_debug_flush_output(void);      // Flush the debug channel buffer
