#include <src/hal/gpio-hal.h>


void initHAL(void){
    MUX_MODE_ADDR[149] = MUX_MODE_GPIO_149;
    START_BITS[149] = 16;
    GPIO_DATAOUT[5] = GPIO5_DATAOUT;
    GPIO_OE[5] = GPIO5_OE;
}
