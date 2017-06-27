#include "sink_the_ship.h"
#include "../lib/matrix_lib/matrix_draw.h"

#define MATRIX_DIM 32
void start_game(){
    volatile int i;
    volatile int k;
    point_t coordinates = {0,0};
    color_t color = {1,0,0};
    for(i = 0; i < MATRIX_DIM;i++){
        for(k = 0; k < MATRIX_DIM; k++){
            draw_crosshair(&coordinates, &color);
            coordinates.y = k++;
            update_matrix();
        }
        coordinates.x = i++;
    }
}
