#ifndef IO_HANDLER_H
#define IO_HANDLER_H
#include "../Drivers/GPIO-Driver.h"

int mos_io_handler_write(int gpioPinNumber, int outputMode, int on);

#endif
