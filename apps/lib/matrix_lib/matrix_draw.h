#ifndef APPS_LIB_MATRIX_LIB_MATRIX_DRAW_H_
#define APPS_LIB_MATRIX_LIB_MATRIX_DRAW_H_

/**
 * LED Matrix Driver LIB for 32x32
 */
#include <inttypes.h>
typedef struct
{
    uint8_t x;
    uint8_t y;
} point_t;

typedef union
{
    struct
    {
        uint8_t r:1;
        uint8_t g:1;
        uint8_t b:1;
    } rgb_split;
    uint8_t rgb;
} color_t;

void update_matrix();

void draw_rect(point_t* bottom_left_corner, point_t* top_right_corner,
               color_t* color);

void draw_crosshair(point_t* coordinates, color_t* color);

#endif /* APPS_LIB_MATRIX_LIB_MATRIX_DRAW_H_ */
