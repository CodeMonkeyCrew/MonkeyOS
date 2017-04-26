#include "drivers/hal/includes/register_util.h"
#include <inttypes.h>
#include "drivers/hal/timer_hal/timer_hal.h"
#include "drivers/hal/register/intcps_register.h"

#define PM_PWSTCTRL_PER (volatile uint32_t*)0x483070E0

openISR(){

}
closeISR(){

}

writeISR(){

}
void main(void) {
    *PM_PWSTCTRL_PER |= ((1 << 0) | (1 << 1));

    init_gptimer(2);
    enable_compare_mode(2, 0, 0xffff);
    uint32_t* unmusk_i2c = MIRn(1);
    *unmusk_i2c |= (1 << 25);
    enable_interrupt(2);

    _enable_interrupts();
    gptimer_start(2);
    while(1){

    }
}
