#include "timer_hal.h"


int init_gptimer(const uint8_t gptimer){

}



static uint32_t* getBaseAddress(const uint8_t gptimer){
    switch(gptimer){
    case 1:
        return GPTIMER1;
        break;
    default:
        return GPTIMER_GENERAL_BASE + (0x2000*(gptimer-1));
        break;
    }
}
