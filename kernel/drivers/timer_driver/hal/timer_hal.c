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

uint32_t set_interrupt_mode(const uint8_t nr, const uint8_t mode)
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

uint32_t enable_interrupt(const uint8_t nr, const uint8_t autoreload)
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
    switch(nr){
    case 1:
        break;
    case 2:
        response = clear_bit((uint32_t*) MIRn(1), 7);
        break;
    case 3:
        response = clear_bit((uint32_t*)MIRn(1),8);
        break;
    case 4:
        response = clear_bit((uint32_t*)MIRn(1),9);
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

