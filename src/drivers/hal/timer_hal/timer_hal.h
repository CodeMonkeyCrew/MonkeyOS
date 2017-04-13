#ifndef SRC_DRIVERS_HAL_TIMER_HAL_TIMER_HAL_H_
#define SRC_DRIVERS_HAL_TIMER_HAL_TIMER_HAL_H_

#include "../includes/gptimer_register.h"

enum mode{
    CAPTURE, COMPARE
};

/*      Methode Definition           */
static uint32_t* getBaseAddress(const uint8_t gptimer);

int init_gptimer(const uint8_t gptimer);

int set_mode_for_gptimer(const uint8_t gptimer, enum mode);

#endif /* SRC_DRIVERS_HAL_TIMER_HAL_TIMER_HAL_H_ */
