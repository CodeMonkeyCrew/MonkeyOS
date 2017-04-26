#ifndef GPIO_DRIVER_H
#define GPIO_DRIVER_H

#include <inttypes.h>

void mos_gpio_LLD_init(int gpioPinNumber, int gpioPort, uint32_t* muxModeAddr, int startBit);
//direction: IN=1, OUT=0
void mos_gpio_LLD_set_direction(uint32_t* gpioOE, int shift, int direction);
//mode: ON=1, OFF=0
void mos_gpio_LLD_set_value(uint32_t* gpioDataOut, int shift, int mode);
int mos_gpio_LLD_get_value(int gpioPinNumber);

#endif
