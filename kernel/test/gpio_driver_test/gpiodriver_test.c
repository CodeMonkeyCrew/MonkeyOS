#include "../../drivers/gpio_driver/gpiodriver.h"
#include "../../filesystem/filesystem.h"

static void blinkLED();
static int val_1 = 1;
static int val_0 = 0;

void test_gpiodriver()
{
    blinkLED();
}

static void blinkLED()
{
    filesystem_init();
    gpiodriver_init();

      int dirFd = mos_fs_open("gpio150_DIR");
      int valFd = mos_fs_open("gpio150_VAL");
      int dirFd2 = mos_fs_open("gpio149_DIR");
      int valFd2 = mos_fs_open("gpio149_VAL");

      mos_fs_write(dirFd, &val_0, 1);
      mos_fs_write(dirFd2, &val_0, 1);
      volatile int i = 0;
      while (1)
      {
          mos_fs_write(valFd2, &val_0, 1);
          mos_fs_write(valFd, &val_0, 1);
          for (i = 0; i < 100000; i++)
              ;
          mos_fs_write(valFd2, &val_1, 1);
          mos_fs_write(valFd, &val_1, 1);
          for (i = 0; i < 100000; i++)
              ;
      }

}
