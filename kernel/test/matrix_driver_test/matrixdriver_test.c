#include <inttypes.h>
#include <kernel/test/matrix_driver_test/matrixdriver_test.h>
#include "../../filesystem/filesystem.h"
#include "../../drivers/gpio_driver/gpiodriver.h"
#include "../../drivers/matrix_driver/matrix_driver.h"


/* Defines and Variables*/
#define BUFFER_SIZE 512
static int dir_fd;
static int val_fd;
static int val_1 = 1;
static void* pVal_1 = &val_1;

static uint8_t buffer[BUFFER_SIZE];
static void matrixDriverTest(void);

static void test_usedGpioPins();

void test_matrixdriver()
{
    test_usedGpioPins();
    while(1){

    }
}
/*
 * Set all used pins to high and check manual
 * */
static void test_usedGpioPins()
{
    mos_fs_init();
    gpiodriver_init();

    dir_fd = mos_fs_open("gpio133_dir");
    val_fd = mos_fs_open("gpio133_val");
    mos_fs_write(dir_fd, pVal_1, 1);
    mos_fs_write(val_fd, pVal_1,1);
    dir_fd = mos_fs_open("gpio162_dir");
    val_fd = mos_fs_open("gpio162_val");
    mos_fs_write(dir_fd, pVal_1, 1);
    mos_fs_write(val_fd, pVal_1,1);
    dir_fd = mos_fs_open("gpio134_dir");
    val_fd = mos_fs_open("gpio134_val");
    mos_fs_write(dir_fd, pVal_1, 1);
    mos_fs_write(val_fd, pVal_1,1);
    dir_fd = mos_fs_open("gpio158_dir");
    val_fd = mos_fs_open("gpio158_val");
    mos_fs_write(dir_fd, pVal_1, 1);
    mos_fs_write(val_fd, pVal_1,1);
    dir_fd = mos_fs_open("gpio183_dir");
    val_fd = mos_fs_open("gpio183_val");
    mos_fs_write(dir_fd, pVal_1, 1);
    mos_fs_write(val_fd, pVal_1,1);
    dir_fd = mos_fs_open("gpio131_dir");
    val_fd = mos_fs_open("gpio131_val");
    mos_fs_write(dir_fd, pVal_1, 1);
    mos_fs_write(val_fd, pVal_1,1);
    dir_fd = mos_fs_open("gpio159_dir");
    val_fd = mos_fs_open("gpio159_val");
    mos_fs_write(dir_fd, pVal_1, 1);
    mos_fs_write(val_fd, pVal_1,1);
    dir_fd = mos_fs_open("gpio161_dir");
    val_fd = mos_fs_open("gpio161_val");
    mos_fs_write(dir_fd, pVal_1, 1);
    mos_fs_write(val_fd, pVal_1,1);
    dir_fd = mos_fs_open("gpio130_dir");
    val_fd = mos_fs_open("gpio130_val");
    mos_fs_write(dir_fd, pVal_1, 1);
    mos_fs_write(val_fd, pVal_1,1);
    dir_fd = mos_fs_open("gpio132_dir");
    val_fd = mos_fs_open("gpio132_val");
    mos_fs_write(dir_fd, pVal_1, 1);
    mos_fs_write(val_fd, pVal_1,1);
    dir_fd = mos_fs_open("gpio135_dir");
    val_fd = mos_fs_open("gpio135_val");
    mos_fs_write(dir_fd, pVal_1, 1);
    mos_fs_write(val_fd, pVal_1,1);
    dir_fd = mos_fs_open("gpio145_dir");
    val_fd = mos_fs_open("gpio145_val");
    mos_fs_write(dir_fd, pVal_1, 1);
    mos_fs_write(val_fd, pVal_1,1);
    dir_fd = mos_fs_open("gpio136_dir");
    val_fd = mos_fs_open("gpio136_val");
    mos_fs_write(dir_fd, pVal_1, 1);
    mos_fs_write(val_fd, pVal_1,1);
}

static void matrixDriverTest(void)
{
    mos_fs_init();
    gpiodriver_init();
    matrix_driver_init();

    int matrix0Fd = mos_fs_open("matrix0");

    int i;
    for (i = 0; i < BUFFER_SIZE; ++i)
    {
        buffer[i] = 0xFF;
    }

    mos_fs_write(matrix0Fd, buffer, BUFFER_SIZE);

    matrix_driver_update();
    matrix_driver_update();

    while (1)
    {
    }
}
