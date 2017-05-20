#include <kernel/drivers/timer_driver/hal/timer_hal.h>
#include "../../util/registerutil.h"
#include "timer_register.h"

uint32_t timer_init(const uint8_t nr)
{
    uint32_t response = 1;
    //enable
    if (nr == 1)
    {

    }
    else if (nr > 1 && nr < 10)
    {
        response = set_bit((uint32_t*) CM_FCLKEN_PER, 3);
        response = set_bit((uint32_t*) CM_ICLKEN_PER, 3);
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

    response = clear_bit(get_address(GPTIMER(nr), TCLR), 0);
    response = clear_32(get_address(GPTIMER(nr), TCRR));

    return response;
}

uint32_t timer_enable_compare_mode(const uint8_t nr, const uint32_t value)
{
    uint32_t response = 1;
    response = set_bit(get_address(GPTIMER(nr), TCLR), 6); //compare enable
    response = set_bit(get_address(GPTIMER(nr), TIER), 0); //enable Compare (Match) Interrupt
    response = set_value_32(get_address(GPTIMER(nr), TMAR), value); //set value to which the timer is compared
    return response;    //compare mode enable
}

uint32_t timer_set_interrupt_mode(const uint8_t nr, const uint8_t mode)
{
    uint8_t response = 1;
    if (mode < 3)
    {
        clear_bit(get_address(GPTIMER(nr), TIER), 0);
        clear_bit(get_address(GPTIMER(nr), TIER), 1);
        clear_bit(get_address(GPTIMER(nr), TIER), 2);
        response = set_bit(get_address(GPTIMER(nr), TIER), mode);
        return response;
    }
    return 0;
}

uint32_t timer_enable_interrupt(const uint8_t nr, const uint8_t autoreload)
{
    uint8_t response = 1;
    //96 possible interrups, 0 - Bit0-31, 1 - Bit 32-63 -> n = 1

    //clear interrupt bits
    response = set_bit(get_address(GPTIMER(nr), TISR), 0);
    response = set_bit(get_address(GPTIMER(nr), TISR), 1);
    response = set_bit(get_address(GPTIMER(nr), TISR), 2);

    if (autoreload)
    {
        response = set_bit(get_address(GPTIMER(nr), TCLR), 1);
    }
    switch (nr)
    {
    case 1:
        break;
    case 2:
        response = clear_bit((uint32_t*) MIRn(1), 6);
        break;
    case 3:
        response = clear_bit((uint32_t*) MIRn(1), 7);
        break;
    case 4:
        response = clear_bit((uint32_t*) MIRn(1), 8);
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
    return response;
}

uint32_t timer_disable_interrupt(const uint8_t nr)
{
    uint32_t response = 1;
    switch (nr)
    {
    case 1:
        break;
    case 2:
        response = set_bit((uint32_t*) MIRn(1), 7);
        break;
    case 3:
        response = set_bit((uint32_t*) MIRn(1), 8);
        break;
    case 4:
        response = set_bit((uint32_t*) MIRn(1), 9);
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

    return response;
}
uint32_t timer_clear_interrupt(const uint8_t nr)
{
    //clear interrupt bits
    set_bit(get_address(GPTIMER(nr), TISR), 0);
    set_bit(get_address(GPTIMER(nr), TISR), 1);
    set_bit(get_address(GPTIMER(nr), TISR), 2);
    timer_stop(nr);
    timer_start(nr);
    return 1;
}

uint32_t timer_start(const uint8_t nr)
{
    uint32_t respone = 1;
    clear_bit(get_address(GPTIMER(nr), TCLR), 0);
    set_bit(get_address(GPTIMER(nr), TCLR), 0);

    return respone;
}

uint32_t timer_stop(const uint8_t nr)
{
    clear_bit(get_address(GPTIMER(nr), TCLR), 0);
    clear_32(get_address(GPTIMER(nr), TCRR));
    return 1;
}
uint32_t timer_set_clock(const uint8_t nr, const uint8_t clock_mode)
{
    if (nr >= 2 && nr <= 9)
    {
        if (clock_mode)
        {
            return set_bit((uint32_t*)CM_CLKSEL_PER, (nr - 2));
        }
        else
        {
            return clear_bit((uint32_t*)CM_CLKSEL_PER, (nr - 2));
        }
    }
    return 0;
}
