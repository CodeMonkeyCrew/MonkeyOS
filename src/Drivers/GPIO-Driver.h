#ifndef GPIO_DRIVER_H
#define GPIO_DRIVER_H
#include "../HAL/GPIO-HAL.h"

 //f.ex gpioPinNumber=GPIO_149, outputMode=0 (input), on=1 (turn on) Interface for IO-Handler, calls the following functions
int writeGPIO(int gpioPinNumber, int outputMode, int on);
//f.ex GPIO:149 is in Region GPIO_5 and has shift 21
int* calcGPIORegionAndShift(int gpioPinNumber);

//powermode on, Clocks, wakeup,..
void setupSystem(int gpioRegion);
void setMuxMode(int gpioNumber);
//maybe make one function instead of 6
int setGPIO(int gpioRegion, int outputMode, int on, int shift);

#endif
