#include <avr/interrupt.h>
#include "device/mcu.h"
#include "device/peripherals.h"

// Macro to set the cpu prescaling factor
#define CPU_PRESCALE(n) (CLKPR = 0x80, CLKPR = (n))

// Instruction for jumping to the bootloading
#define JMP_BOOTLOADER  "jmp 0x7e00"

// Instruction for resetting the MCU
#define JMP_RESET_MCU   "jmp 0x0000"


// Scale the speed of the MCU
void cpu_prescale(uint8_t speed)
{
    CPU_PRESCALE(speed);
}

// Internal shorthand function for disabling all MCU peripherals
void disable_mcu(void)
{
    // First, disable all interrupts
    cli();

    // Next, disable USB and wait 5 ms for the host device to register
    // this event and disconnect
    UDCON = 1;
    USBCON = (1<<FRZCLK);
    UCSR1B = 0;

    // Then, deconfigure all pins and interrupts
    EIMSK = 0; PCICR = 0; SPCR = 0; ACSR = 0; EECR = 0; ADCSRA = 0;
    TIMSK0 = 0; TIMSK1 = 0; TIMSK3 = 0; TIMSK4 = 0; UCSR1B = 0; TWCR = 0;
    DDRB = 0; DDRC = 0; DDRD = 0; DDRE = 0; DDRF = 0; TWCR = 0;
    PORTB = 0; PORTC = 0; PORTD = 0; PORTE = 0; PORTF = 0;
}

// Interrupt Service Routine which gets run when the reset button is
// pressed, operating on the rising edge of the signal. Depending on
// the state of bootloader_config, the MCU will either jump to 0x0000
// and reset the MCU, or it will jump to 0x07e00 and thereby jumping
// to the HalfKay bootloader.
ISR(PCINT0_vect)
{
    // Operate on the rising edge of the interrupt
    if (RESET_PRESSED) {
        disable_mcu();
        if (bootloader_enabled)
            asm volatile(JMP_BOOTLOADER);
        else
            asm volatile(JMP_RESET_MCU);
    }
}
