#ifndef KERNEL_DRIVERS_LED_MATRIX_DRIVER_LED_MATRIX_DRIVER_H_
#define KERNEL_DRIVERS_LED_MATRIX_DRIVER_LED_MATRIX_DRIVER_H_
#include <inttypes.h>

struct {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
}RGB;

struct {
    uint8_t y;
    uint8_t x;
}Coordinate;

uint32_t init_matrix(const uint8_t gpio, const uint8_t[] gpio_pins);

uint32_t draw_pixel(const struct Coordinate pos, const struct RGB rgb);

uint32_t draw_rect(const struct Coordinate pos, const uint8_t length, const uint8_t height, const struct RGB rgb);


#endif /* KERNEL_DRIVERS_LED_MATRIX_DRIVER_LED_MATRIX_DRIVER_H_ */
