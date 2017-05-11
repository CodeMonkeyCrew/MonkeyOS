#include <inttypes.h>
#include "proc/scheduler.h"
#include "proc/mode.h"
#include <kernel/drivers/gpio_driver/gpiodriver.h>
#include <kernel/drivers/timer_driver/timer-driver.h>
#include <kernel/drivers/register/intcps_register.h>
#include <kernel/drivers/timer_driver/hal/timer_hal.h>
#include <kernel/drivers/util/registerutil.h>
#include <kernel/filesystem/filesystem.h>


#define PM_PWSTCTRL_PER (volatile uint32_t*)0x483070E0

void testFromFSToDrivers();
void testTimer();
void testTimerFromFS();

void main(void) {
    scheduler_init();

    // set user mode and enable interrupts
    mode_setUserMode();

    // idle loop
    while(1) {
        volatile int i = 0;
        for(i = 0; i < 10000; ++i) {
        }
    }
}

void testFromFSToDrivers() {
    mos_fs_init();
    mos_gpio_driver_init();

    int dir_fd = mos_fs_open("gpio149_dir");
    int val_fd = mos_fs_open("gpio149_val");

    int val_1 = 1;
    void* pVal_1 = &val_1;

    mos_fs_write(dir_fd, pVal_1, 1);

    int val_0 = 0;
    void* pVal_0 = &val_0;

    volatile int i;
    while(1){
        mos_fs_write(val_fd, pVal_1, 1);
        for (i = 0; i < 20000; i++)
        {

        }
        mos_fs_write(val_fd, pVal_0, 1);
        for (i = 0; i < 20000; i++)
        {
        }
    }
}

void testTimer() {
    _disable_interrupts();
    *PM_PWSTCTRL_PER |= ((1 << 0) | (1 << 1));

    init_gptimer(2);
    enable_compare_mode(2, 0, 0xfffff);
    enable_interrupt(2);

    _enable_interrupts();
    _enable_IRQ();
    gptimer_start(2);
    while(1){

    }
}

void testTimerFromFS() {
    _disable_interrupts();
    *PM_PWSTCTRL_PER |= ((1 << 0) | (1 << 1));

    mos_fs_init();
    mos_timer_driver_init(); //creates and registers drivers and files

    int fd_timer2_int = mos_fs_open("GPTimer_2_INT");
    mos_fs_write(fd_timer2_int, 0, 0); //enables interrupt

    int fd_timer_2_int = mos_fs_open("GPTimer_2_MODE");
    int compareValue = 0xFFFFF;
    mos_fs_write(fd_timer_2_int, &compareValue, 1); //enable COMPARE mode and set compare-value

    int state = 1;
    int fd_timer2 = mos_fs_open("GPTimer_2");

    _enable_interrupts();
    _enable_IRQ();

    mos_fs_write(fd_timer2, &state, 1); //start gptimer2

    while(1){}
}