#include "timer_driver.h"

void mos_timer_driver_init(void){
    //use malloc as it would otherwise be put on the stack,
    //which will be freed when the functions ends.
    //new data will then maybe override the driver
    driver_t* timerdriver = (driver_t*) malloc(sizeof(driver_t));
    timerdriver->driver_read = mos_timer_read;
    timerdriver->driver_write = mos_timer_write;
    timerdriver->driver_open = mos_timer_open;
   // register_driver(GPIO_VAL, gpioDriver);
   // register_driver(GPIO_DIR, gpioDriver);
}

static void mos_timer_read(const void* buffer, int bufSize, generic_file_t* file){

}
static void mos_timer_write(const void* buffer, int bufSize, generic_file_t* file){

}
static void mos_timer_open(generic_file_t* file){

}


