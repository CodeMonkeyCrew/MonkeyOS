#include "matrix_draw.h"
#include <stdio.h>
#include "../mos_sys_lib.h"

#define ROWS 32
#define COLLUMNS 32
#define BUFFERSIZE 512

static uint8_t matrix[COLLUMNS][ROWS];
static matrix0Fd;

void init_matrix(){
    mos_fs_init();
     gpiodriver_init();
     matrix_driver_init();

     matrix0Fd = mos_fs_open("matrix0");

}
void update_matrix(){
    mos_fs_write(matrix0Fd, matrix, BUFFERSIZE);
}


void draw_rect(point_t* bottom_left_corner, point_t* top_right_corner,
               color_t* color)
{
    volatile uint8_t i = 0;
    volatile uint8_t k = 0;

    for (i = 0; i <= COLLUMNS; i++)
    {
        for (k = 0; k <= ROWS; k++)
        {
            if (i >= bottom_left_corner->y && i <= top_right_corner->y)
            {
                if (k >= bottom_left_corner->x && k <= top_right_corner->x)
                {
                    matrix[k][i] |= color->rgb;
                }
            }
        }
    }
}
void draw_crosshair(point_t* coordinates, color_t* color)
{
    volatile uint8_t i = coordinates->y;
    volatile uint8_t k = coordinates->x;
    matrix[i][k] |= color->rgb;
}
