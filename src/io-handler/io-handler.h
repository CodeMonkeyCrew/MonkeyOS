#ifndef IO_HANDLER_H
#define IO_HANDLER_H
#include "../drivers/gpio-driver/gpio-driver.h"

int mos_io_handler_write(int gpioPinNumber, int outputMode, int on);

#endif