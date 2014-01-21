Sun Keyboard to USB converter
=============================

Introduction
------------
This firmware defines a USB keyboard which uses a Sun Type 5/6 keyboard as
input. The firmware works in conjunction with a PJRC Teensy 2.0 together with
some modifications for handling the transport of packets to/from the keyboard,
button debouncing, status leds and jumper based configuration.

TL;DR
-----
1. Configure the jumpers to enable bootloader support
2. Attach keyboard to controller
3. Attach controller to USB cable
4. Run the following commands:
```
 make clean
 make all
 make program
```

Hardware overview
-----------------
The Sun Type 5/6 keyboard communicates over 1200 baud ttl-232 serial, using
inverted logic levels, which need to be inverted in order to use them. Apart
from that, connecting this keyboard to a modern pc is a matter of reading,
converting and transmitting the scancodes, which keeps the amount of
modifications to a minimum.

![Schematic of controller][id]
[id]: https://raw.github.com/r3boot/sunkbd/master/eagle/sunkbd_schematic.png "Controller schematic"

The keyboards RX/TX lines are connected through a hex inverter based on a 7404N,
before being put onto the D2/D3 ports of the onboard UART. A common-anode
dual-color led is used to inform the user of RX/TX traffic, or to signal an
error. This led is connected using R2,R3 to the F0/F1 ports on the teensy. A
switch together with a capacitor based debouncing circuit (R1, C1) are fed
into a NAND Schmitt Trigger based on a 7414 to further smooth out the signal,
which is then transported to the B0 pin of the teensy. Finally, a set of
jumpers which can be bound to ground are connected to pins D1,D2,D3, to provide
a way of configuring the firmware in the field. Pins D0..D3 use internal
pull-up resistors.

Software overview
-----------------
The software consists of a couple of drivers for both the onboard UART as well
as the onboard USB chip, bound together using scancode conversion logic and
buffers for the UART and the USB keyboard.

There are various states in which the keyboard operates. At first startup,
when the keyboard isn't initialized, the firmware will send a reset code to
the keyboard. Depending on the result returned by the keyboard, the keyboard
is either initialized, or it will fail, but will keep sending scancodes.

When the keyboard is initialized, and a scancode is received, the firmware will
first determine what kind of scancode is received. Based on this, it will set
modifier and/or led status. If the key is a pc105 scancode, the key will be
added/removed from the keyboard buffer, whenever the key is pressed/released.
Whenever a multimedia key has been detected, it will be sent over a remote
control USB hid descriptor, allowing the host computer to use the keys as
multimedia keys.

Jumper based configuration
--------------------------
To allow configuration in the field, a set of jumpers has been created which
can be used to configure various settings which are defined during startup
of the firmware. The current implementation uses bit-masks for configuration
settings, allowing a maximum of 3 field-configurable options. A possible
future modification might make this an integer-based configuration allowing 
more configuration options. Below you can find a list of jumper/pin mappings
and their functions

    PIN     JUMPER  DESCRIPTION
    PB1     J0      Enable keyboard clicks. When this jumper is bridged, a sound
                    will be created by the keyboard, giving a audible feedback.
    PB2     J1      Enable debug support. Bridging this jumper will enable the
                    USB raw-hid debugging channel. Only useful during development
    PB3     J2      Enable bootloader support. By bridging this jumper, the MCU
                    will jump to the HalfKay bootloader instead of resetting when
                    the reset button is pressed.
