#include "timer_hal.h"
#include "../../register/cm_register.h"
#include "../../register/intcps_register.h"

#include "../../util/registerutil.h"

uint32_t init_gptimer(const uint8_t nr)
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
    for (i = 0; i < 20000; ++i) {

    }

    response = clear_bit(get_address(GPTIMER(nr), TCLR), 0);
    response = clear_32(get_address(GPTIMER(nr), TCRR));

    return response;
}


uint32_t enable_compare_mode(const uint8_t nr, const uint8_t compareregister,
                             const uint32_t value)
{
    uint32_t response = 1;
    response = clear_bit(get_address(GPTIMER(nr), TCLR), 6);
    response = set_bit(get_address(GPTIMER(nr), TIER), 2);

    if (compareregister)
    {
        response = set_bit(get_address(GPTIMER(nr), TCLR), 13); //set TCAR2
    }
    response = clear_bit(get_address(GPTIMER(nr), TCLR), 13); //set TCAR1
    response = set_value_32(get_address(GPTIMER(nr), TMAR), value);
    return response;    //compare mode enable
}

uint32_t enable_interrupt(const uint8_t nr)
{
    uint8_t response = 1;
    //96 possible interrups, 0 - Bit0-31, 1 - Bit 32-63 -> n = 1
    //only for gtimer 2 atm
    response = set_bit(get_address(GPTIMER(nr), TIER), 1);
    response = set_bit(get_address(GPTIMER(nr), TIER), 0);
    response = set_bit(get_address(GPTIMER(nr), TISR), 1);
    response = set_bit(get_address(GPTIMER(nr), TCLR), 1);
    if (nr == 2)
    {
        response = clear_bit((uint32_t*) MIRn(1), 7);
    }

    return response;
}

uint32_t gptimer_start(const uint8_t nr)
{
    uint32_t respone = 1;
    clear_bit(get_address(GPTIMER(nr), TCLR), 0);
    set_bit(get_address(GPTIMER(nr), TCLR), 0);

    return respone;
}

uint32_t gptimer_stop(const uint8_t nr)
{
    clear_32(get_address(GPTIMER(nr), TCRR));
    clear_bit(get_address(GPTIMER(nr), TCLR), 0);
    return 1;
}
