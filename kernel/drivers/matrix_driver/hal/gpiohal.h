#ifndef MATRIX_GPIO_DRIVER_H
#define MATRIX_GPIO_DRIVER_H

#include <inttypes.h>

/* ---------------------------------------------------- *
 * GPIO - HAL                                           *
 * ATTENTION: ONLY WORKS FOR specific GPIO PINs:        *
 *      - Pin130 to Pin166                              *
 *      - Pin183                                        *
 * ---------------------------------------------------- */

/**
 * Set the mux mode of the gpio pin to 4 and enable the power and necessary clocks
 * @param pinNumber number of the pin on the board
 */
void matrix_gpiohal_pinInit(const uint8_t pinNumber);
/**
 * Set pin to input or output
 * @param pinNumber number of the pin on the board
 * @param direction:
 *          1 - in
 *          0 - out
 */
void matrix_gpiohal_pinSetDirection(const uint8_t pinNumber,const uint8_t direction);
/**
 * Set the value of a pin
 * @param pinNumber number of the pin on the board
 * @param value value to be set (1 or 0)
 */
void matrix_gpiohal_pinSetValue(const uint8_t pinNumber, const uint8_t value);
/**
 * Read the current state of the gpio pin
 * @param pinNumber number of the pin on the board
 */
uint8_t matrix_gpiohal_pinGetValue(const uint8_t pinNumber);

#endif
