#include "proc/scheduler.h"
#include "proc/mode.h"

// registers to mask/unmask interrupt lines
long * INTCPS_MIR0 = (long*) 0x48200084;

// register to control module interface clock activity
long * CM_ICLKEN_WKUP = (long*) 0x48004C10;
// register to control pad configuration (used setting mux mode and resistors)
long * CONTROL_PADCONF_SYS_BOOT2 = (long*) 0x48002A0C;

// register to set interrupt status flag
long * GPIO1_IRQSTATUS1 = (long*) 0x48310018;
// register to enable interrupt on rising edge
long * GPIO1_RISINGDETECT = (long*) 0x48310048;
// register to enable interrupts
long * GPIO1_IRQENABLE1 = (long*) 0x4831001C;
// register to enable input/output
long * GPIO1_OE = (long*) 0x48310034;

long BTN = (1 << 4);

#pragma SWI_ALIAS(put, 0);
int put(int value);

int putISR(int value) {
    return 4711;
}

void proc1(void) {
    while(1) {
        volatile int i = 0;
        for(i = 0; i < 10000; ++i) {
        }
    }
}

void main(void) {
    // unmask interrupt line 29 (GPIO 1)
    *INTCPS_MIR0 &= ~(1 << 29);

    // enable interface clock (bit 3 for GPIO 1)
    *CM_ICLKEN_WKUP |= (1 << 3);

    // set mux mode to 4 (GPIO)
    *CONTROL_PADCONF_SYS_BOOT2 &= ~(1 << 16);
    *CONTROL_PADCONF_SYS_BOOT2 &= ~(1 << 17);
    *CONTROL_PADCONF_SYS_BOOT2 |= (1 << 18);

    // enable and select pull-down resistor
    *CONTROL_PADCONF_SYS_BOOT2 |= (1 << 19);
    *CONTROL_PADCONF_SYS_BOOT2 &= ~(1 << 20);

    // clear interrupt flag
    *GPIO1_IRQSTATUS1 |= BTN;
    *GPIO1_RISINGDETECT |= BTN;
    *GPIO1_IRQENABLE1 |= BTN;

    // enable input on pin
    *GPIO1_OE |= BTN;

    scheduler_init();
    scheduler_initProc(proc1);

    // set user mode and enable interrupts
    mode_setUserMode();

    // idle loop
    while(1) {
        volatile int i = 0;
        for(i = 0; i < 10000; ++i) {
        }
    }
}
