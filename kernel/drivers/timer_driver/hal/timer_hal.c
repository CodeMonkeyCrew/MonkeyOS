#include <kernel/drivers/timer_driver/hal/timer_hal.h>
#include "../../util/registerutil.h"
#include "timer_register.h"

void timer_init(const uint8_t nr)
{
    //enable
    if (nr == 1)
    {

    }
    else if (nr > 1 && nr < 10)
    {
        set_bit((uint32_t*) CM_FCLKEN_PER, 3);
        set_bit((uint32_t*) CM_ICLKEN_PER, 3);
    }

    //this is needed as it takes a few CPU-Cycles(?) until CM_FCLKEN_PER & CM_ICLKEN_PER
    //are active. If we do not wait, we cannot read/write into the adresses of GPTimer2
    //which will lead to isr_abort when running the statesments after the for-loop.
    //This is also the reason why it does work when stepping through the function and doesn't work
    //when running the program.
    volatile int i;
    for (i = 0; i < 20000; ++i)
    {

    }

    clear_bit(get_address(GPTIMER(nr), TCLR), 0);
    clear_32(get_address(GPTIMER(nr), TCRR));

}

void timer_enable_compare_mode(const uint8_t nr, const uint32_t value)
{

    set_bit(get_address(GPTIMER(nr), TCLR), 6); //compare enable
    set_bit(get_address(GPTIMER(nr), TIER), 0); //enable Compare (Match) Interrupt
    set_value_32(get_address(GPTIMER(nr), TMAR), value); //set value to which the timer is compared
                                                         //compare mode enable
}

void timer_set_interrupt_mode(const uint8_t nr, const uint8_t mode)
{

    if (mode < 3)
    {
        clear_bit(get_address(GPTIMER(nr), TIER), 0);
        clear_bit(get_address(GPTIMER(nr), TIER), 1);
        clear_bit(get_address(GPTIMER(nr), TIER), 2);
        set_bit(get_address(GPTIMER(nr), TIER), mode);
    }

}

void timer_enable_interrupt(const uint8_t nr, const uint8_t autoreload)
{
    //96 possible interrups, 0 - Bit0-31, 1 - Bit 32-63 -> n = 1

    //clear interrupt bits
    set_bit(get_address(GPTIMER(nr), TISR), 0);
    set_bit(get_address(GPTIMER(nr), TISR), 1);
    set_bit(get_address(GPTIMER(nr), TISR), 2);

    if (autoreload)
    {
        set_bit(get_address(GPTIMER(nr), TCLR), 1);
    }
    switch (nr)
    {
    case 1:
        break;
    case 2:
        clear_bit((uint32_t*) MIRn(1), 6);
        break;
    case 3:
        clear_bit((uint32_t*) MIRn(1), 7);
        break;
    case 4:
        clear_bit((uint32_t*) MIRn(1), 8);
        break;
    case 5:
        break;
    case 6:
        break;
    case 7:
        break;
    case 8:
        break;
    case 9:
        break;
    case 10:
        break;
    case 11:
        break;
    }
}

void timer_disable_interrupt(const uint8_t nr)
{
    switch (nr)
    {
    case 1:
        break;
    case 2:
        set_bit((uint32_t*) MIRn(1), 7);
        break;
    case 3:
        set_bit((uint32_t*) MIRn(1), 8);
        break;
    case 4:
        set_bit((uint32_t*) MIRn(1), 9);
        break;
    case 5:
        break;
    case 6:
        break;
    case 7:
        break;
    case 8:
        break;
    case 9:
        break;
    case 10:
        break;
    case 11:
        break;
    }

}
void timer_clear_interrupt(const uint8_t nr)
{
    //clear interrupt bits
    set_bit(get_address(GPTIMER(nr), TISR), 0);
    set_bit(get_address(GPTIMER(nr), TISR), 1);
    set_bit(get_address(GPTIMER(nr), TISR), 2);
    timer_stop(nr);
    timer_start(nr);
}

void timer_start(const uint8_t nr)
{
    clear_bit(get_address(GPTIMER(nr), TCLR), 0);
    set_bit(get_address(GPTIMER(nr), TCLR), 0);

}

void timer_stop(const uint8_t nr)
{
    clear_bit(get_address(GPTIMER(nr), TCLR), 0);
    clear_32(get_address(GPTIMER(nr), TCRR));
}
void timer_set_clock(const uint8_t nr, const uint8_t clock_mode)
{
    if (nr >= 2 && nr <= 9)
    {
        if (clock_mode)
        {
            set_bit((uint32_t*) CM_CLKSEL_PER, (nr - 2));
        }
        else
        {
            clear_bit((uint32_t*) CM_CLKSEL_PER, (nr - 2));
        }
    }
}
