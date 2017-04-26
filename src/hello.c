#include "filesystem/filesystem.h"
#include "drivers/gpio-driver/gpiodriver.h"

#pragma SWI_ALIAS(put, 0);
int put(int value);

int putISR(int value) {
    return 4711;
}

void main(void) {
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
//	int res = put(10);
}
