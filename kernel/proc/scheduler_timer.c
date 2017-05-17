#include "scheduler_timer.h"
#include "../drivers/timer_driver/hal/timer_hal.h"

#define MAXIMUM_REALITY_SCHEDULER_TIMER_INTERRUPT_DURATION 0xff

void scheduler_timer_init(void)
{
    timer_init(2);
    timer_set_clock(2, 0);
    timer_enable_compare_mode(2, MAXIMUM_REALITY_SCHEDULER_TIMER_INTERRUPT_DURATION);
    timer_enable_interrupt(2, 0);
}

void scheduler_timer_start(void)
{
    timer_start(2);
}

void scheduler_timer_clear_interrupt(void)
{
    timer_clear_interrupt(2);
}
