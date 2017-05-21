#ifndef GPIO_DRIVER_H
#define GPIO_DRIVER_H

#include <inttypes.h>
/*
 *
 * ATTENTION: ONLY WORKS FOR specific GPIO PINs:
 *      - Pin130 to Pin166
 *
 *
 * */

/*
 * Set the mux mode of the gpio pin and enable the power and necessary clocks
 * */
void gpiohal_pinInit(const uint8_t pinNumber);
//direction: IN=1, OUT=0
void gpiohal_pinSetDirection(const uint8_t pinNumber,const uint8_t direction);
//mode: ON=1, OFF=0
void gpiohal_pinSetValue(const uint8_t pinNumber, const uint8_t value);
uint8_t gpiohal_pinGetValue(const uint8_t pinNumber);

#endif
