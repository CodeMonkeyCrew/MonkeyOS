#include <kernel/drivers/register/cm_register.h>
#include <kernel/drivers/register/intcps_register.h>
#include <kernel/drivers/timer_driver/hal/timer_hal.h>
#include <kernel/drivers/util/registerutil.h>

uint32_t init_gptimer(const uint8_t gptimer)
{
    uint32_t response = 1;
    //enable
    if (gptimer == 1)
    {

    }
    else if (gptimer > 1 && gptimer < 10)
    {
        response = set_bit(CM_ICLKEN_PER, 3);
        response = set_bit(CM_FCLKEN_PER, 3);
    }
    response = clear_bit(
            (GPTIMER_GENERAL_BASE + (0x2000 * (gptimer - 2)) + TCLR ), 0);
    response = clear_32(
            (GPTIMER_GENERAL_BASE + (0x2000 * (gptimer - 2)) + TCRR ));
    return response;
}

uint32_t enable_compare_mode(const uint8_t gptimer,
                             const uint8_t compareregister,
                             const uint32_t value)
{
    uint32_t response = 1;
    response = set_bit((GPTIMER_GENERAL_BASE + (0x2000 * (gptimer - 2)) + TCLR ),
                      6);
    response = set_bit((GPTIMER_GENERAL_BASE + (0x2000 * (gptimer - 2)) + TIER ),
                      2);
    if (compareregister)
    {
        response = set_bit(
                (GPTIMER_GENERAL_BASE + (0x2000 * (gptimer - 2)) + TCLR ), 13); //set TCAR2
    }
    response = clear_bit(
            (GPTIMER_GENERAL_BASE + (0x2000 * (gptimer - 2)) + TCLR ), 13); //set TCAR1
    response = set_value_32(
            (GPTIMER_GENERAL_BASE + (0x2000 * (gptimer - 2)) + TMAR ), value);
    return response;    //compare mode enable
}

uint32_t enable_interrupt(const uint8_t gptimer)
{
    //96 possible interrups, 0 - Bit0-31, 1 - Bit 32-63 -> n = 1
    //only for gtimer 2 atm
    uint32_t* maskinterrupt = MIRn(1);
    *maskinterrupt &= (0 << 7);

    return 0;
}

uint32_t gptimer_start(const uint8_t gptimer)
{
    clear_bit((GPTIMER_GENERAL_BASE + (0x2000 * (gptimer - 2)) + TCLR ), 0);

    set_bit((GPTIMER_GENERAL_BASE + (0x2000 * (gptimer - 2)) + TCLR ), 0);
    return 1;
}
uint32_t gptimer_stop(const uint8_t gptimer)
{
    clear_32((GPTIMER_GENERAL_BASE + (0x2000 * (gptimer - 2)) + TCRR ));
    clear_bit((GPTIMER_GENERAL_BASE + (0x2000 * (gptimer - 2)) + TCLR ), 0);
    return 1;
}

