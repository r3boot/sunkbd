
#include <avr/io.h>
#include <avr/pgmspace.h>

#include "print.h"

void print_P(const char *s)
{
	char c;

	while (1) {
		c = pgm_read_byte(s++);
		if (!c) break;
		if (c == '\n') usb_debug_putc('\r');
		usb_debug_putc(c);
	}
}

void phex1(unsigned char c)
{
	usb_debug_putc(c + ((c < 10) ? '0' : 'A' - 10));
}

void phex(unsigned char c)
{
	phex1(c >> 4);
	phex1(c & 15);
}

void phex16(unsigned int i)
{
	phex(i >> 8);
	phex(i);
}




