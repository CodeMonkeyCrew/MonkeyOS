#include <inttypes.h>
#include <kernel/drivers/gpio_driver/gpiodriver.h>
#include <kernel/drivers/register/intcps_register.h>
#include <kernel/drivers/timer_driver/hal/timer_hal.h>
#include <kernel/drivers/util/registerutil.h>
#include <kernel/filesystem/filesystem.h>


#define PM_PWSTCTRL_PER (volatile uint32_t*)0x483070E0

openISR(){

}
closeISR(){

}

writeISR(){

}

void testFromFSToDrivers();
void testTimer();

void main(void) {

    testFromFSToDrivers();

    //testTimer();
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
    *PM_PWSTCTRL_PER |= ((1 << 0) | (1 << 1));

    init_gptimer(2);
    enable_compare_mode(2, 0, 0xffff);
    uint32_t* unmusk_i2c = (uint32_t*)MIRn(1);
    *unmusk_i2c |= (1 << 25);
    enable_interrupt(2);

    _enable_interrupts();
    gptimer_start(2);
    while(1){

    }
}
