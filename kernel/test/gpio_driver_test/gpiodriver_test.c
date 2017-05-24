#include "../../drivers/gpio_driver/gpiodriver.h"
#include "../../filesystem/filesystem.h"

static void blinkLED();
static int dir_fd;
static int val_fd;
static int val_1 = 1;
static void* pVal_1 = &val_1;
static int val_0 = 0;
static void* pVal_0 = &val_0;

void test_gpiodriver()
{
    blinkLED();
}

static void blinkLED()
{
    mos_fs_init();
    gpiodriver_init();

    dir_fd = mos_fs_open("gpio149_dir");
    val_fd = mos_fs_open("gpio149_val");
    volatile int i = 0;
    while (1)
    {
        mos_fs_write(dir_fd, pVal_1, 1);
        mos_fs_write(val_fd, pVal_1, 1);
        for (i = 0; i < 20000; i++)
        {

        }
        mos_fs_write(dir_fd, pVal_0, 1);
        mos_fs_write(val_fd, pVal_0, 1);
        for (i = 0; i < 20000; i++)
        {

        }
    }
}
