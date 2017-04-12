#ifndef GPIO_DRIVER_H
#define GPIO_DRIVER_H
#include "gpio-hal/gpio-hal.h"

 //f.ex gpioPinNumber=GPIO_149, outputMode=0 (input), on=1 (turn on) Interface for IO-Handler, calls the following functions
int writeGPIO(int gpioPinNumber, int outputMode, int on);


#endif
