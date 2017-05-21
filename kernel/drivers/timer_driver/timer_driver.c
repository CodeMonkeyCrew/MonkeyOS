#include <kernel/drivers/timer_driver/hal/timer_hal.h>
#include <kernel/drivers/timer_driver/timer_driver.h>
#include "../drivertypes.h"
#include "../../filesystem/filesystemregister.h"
#include "../../filesystem/filetypes/timertypes.h"
#include <stdlib.h>


static int timerdriver_open(generic_file_t* file) {
    timer_file_t* pTimer_file = (timer_file_t*)file;
    return timer_init(pTimer_file->info.timer_number);
}

static int timerdriver_write(const void* buffer, int bufSize, generic_file_t* file){

    switch (file->f_type) {
        case TIMER: {
            timer_file_t* pTimerFile = (timer_file_t*)file;
            int state = *((int*)buffer); //0 or 1 // off or on
            if (state) {
                timer_start(pTimerFile->info.timer_number);
            } else {
                timer_stop(pTimerFile->info.timer_number);
            }
            break;
        }

        case TIMER_INT: {
            timer_interrupt_file_t* pTimerIntFile = (timer_interrupt_file_t*)file;

            //TODO it should be possible to disable interrupts depending on content (0 or 1) in buffer
            //TODO now its possible to autoreload it
            timer_enable_interrupt(pTimerIntFile->info.timer_number,1);
            break;
        }

        case TIMER_MODE: {
            timer_mode_file_t* pTimerModeFile = (timer_mode_file_t*)file;

            switch (pTimerModeFile->timer_mode) {
                case CAPTURE:
                    //TODO
                    break;
                case COMPARE:
                    timer_enable_compare_mode(pTimerModeFile->info.timer_number, *((int*)buffer));
                    break;
                case OVERFLOW:
                    //TODO
                    break;
            }

            break;
        }
    }
    return 0;
}

static int timerdriver_read(void* buffer, int bufSize, generic_file_t* file) {
    return 0;
}


static void add_drivers();
static void add_files();

//should be called at boot time
void timerdriver_init(void){
    add_drivers();
    add_files();
}

static void add_drivers() {
    //use malloc as it would otherwise be put on the stack,
    //which will be freed when the functions ends.
    //new data will then maybe override the driver
    driver_t* pTimerDriver = (driver_t*) malloc(sizeof(driver_t));
    if (pTimerDriver != NULL) {
        pTimerDriver->driver_read = timerdriver_read;
        pTimerDriver->driver_write = timerdriver_write;
        pTimerDriver->driver_open = timerdriver_open;
        register_driver(TIMER_INT, pTimerDriver);
        register_driver(TIMER_MODE, pTimerDriver);
        register_driver(TIMER, pTimerDriver);
    }
}

static void add_gptimer_2();

static void add_files() {
    add_gptimer_2();
}

static void add_gptimer_2() {
    timer_file_t* pTimerFile = (timer_file_t*)mos_fs_create_file(TIMER);
    if (pTimerFile != NULL) {
        strcpy(pTimerFile->header.name, "GPTimer_2");
        pTimerFile->info.timer_number = 2;
    }

    timer_interrupt_file_t* pTimerIntFile = (timer_interrupt_file_t*)mos_fs_create_file(TIMER_INT);
    if (pTimerIntFile != NULL) {
        strcpy(pTimerIntFile->header.name, "GPTimer_2_INT");
        pTimerIntFile->info.timer_number = 2;
    }

    timer_mode_file_t* pTimerModeFile = (timer_mode_file_t*)mos_fs_create_file(TIMER_MODE);
    if (pTimerModeFile != NULL) {
        strcpy(pTimerModeFile->header.name, "GPTimer_2_MODE");
        pTimerModeFile->info.timer_number = 2;
        pTimerModeFile->timer_mode = COMPARE;
    }
}
