#include "proc/scheduler.h"
#include "memorymanagement/mmu.h"
#include "filesystem/filesystem.h"
#include "drivers/uart/uartdriver.h"
#include "drivers/gpio_driver/gpiodriver.h"
#include "drivers/timer_driver/timer_driver.h"

#define PM_PWSTCTRL_PER (uint32_t*) 0x483070E0

#pragma SWI_ALIAS(execv, 5)
int execv(const char *filename, char *const argv[]);

void main(void)
{
    *PM_PWSTCTRL_PER |= ((1 << 0) | (1 << 1));

    mmu_init();
    filesystem_init();
    uartdriver_init();
    gpiodriver_init();
    gptimerdriver_init();

    scheduler_init();
    scheduler_start();

    execv("idle", NULL);
}

