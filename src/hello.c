#include <src/drivers/gpio-driver/lowLevelGPIO/gpio-LLDriver.h>
#include "src/filesystem/filesystem.h"
#include "src/filesystem/GPIO149_INFO.h"

#pragma SWI_ALIAS(put, 0);
int put(int value);

int putISR(int value) {
    return 4711;
}

void main(void) {
//	mos_fs_write_GPIO(149, 1, 1);
    mos_gpioLLD_init(149, GPIO_PORT, MUX_MODE_GPIO_149, START_BIT);
    mos_gpio_LLD_setDirection(GPIO5_OE, SHIFT, 0);
    volatile int i;
    while(1){
       mos_gpio_LLD_write(GPIO5_DATAOUT, SHIFT, 1);
       for (i = 0; i < 2000000L; i++)
               {

               }
       mos_gpio_LLD_write(GPIO5_DATAOUT, SHIFT, 0);
       for (i = 0; i < 2000000L; i++)
                    {

                    }
    }
//	int res = put(10);
}
